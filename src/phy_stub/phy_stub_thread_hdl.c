/*******************************************************************************
################################################################################
#   Copyright (c) [2017-2019] [Radisys]                                        #
#                                                                              #
#   Licensed under the Apache License, Version 2.0 (the "License");            #
#   you may not use this file except in compliance with the License.           #
#   You may obtain a copy of the License at                                    #
#                                                                              #
#       http://www.apache.org/licenses/LICENSE-2.0                             #
#                                                                              #
#   Unless required by applicable law or agreed to in writing, software        #
#   distributed under the License is distributed on an "AS IS" BASIS,          #
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
#   See the License for the specific language governing permissions and        #
#   limitations under the License.                                             #
################################################################################
*******************************************************************************/

/* This file handles slot indication */

#include "common_def.h"
#include "phy_stub_utils.h"
#ifdef INTEL_FAPI
#include "fapi.h"
#include "fapi_vendor_extension.h"
#endif
#include "phy_stub.h"
#include "mac_sch_interface.h"

extern uint16_t l1BuildAndSendBSR(uint8_t ueIdx, BsrType bsrType,\
             LcgBufferSize lcgBsIdx[MAX_NUM_LOGICAL_CHANNEL_GROUPS]);
pthread_t thread = 0;
int socket_fd =0;
/*******************************************************************
 *
 * @brief Generates slot indications
 *
 * @details
 *
 *    Function : GenerateTicks
 *
 *    Functionality: Generates slot indications
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void GenerateTicks()
{
#ifdef NR_TDD
   float     milisec = 0.5;        /* 0.5ms */
#else
   float     milisec = 1;          /* 1ms */
#endif
   struct timespec req = {0};
   uint8_t ratio = 2;

   slotIndicationStarted = true;
   req.tv_sec = 0;

   /* Currently the code takes longer that one slot indication to execute.
    * Hence, multiplying slot time interval by 2 in order to give enough time 
    * for L2 to complete one slot processing.
    * The ratio must be removed once code optimization is complete */
   req.tv_nsec = milisec * 1000000L * ratio;

   DU_LOG("\nPHY_STUB : GenerateTicks : Starting to generate slot indications");

   while(slotIndicationStarted)
   {
      clock_nanosleep(CLOCK_REALTIME, 0, &req, NULL); 
      /* Send Slot indication indication to lower mac */
      if(l1BuildAndSendSlotIndication() != ROK)
      {
         DU_LOG("\nERROR  --> PHY_STUB : GenerateTicks(): Failed to build and send Slot Indication");
         return;
      }
   }

   DU_LOG("\nINFO  --> PHY_STUB : Slot indication stopped");

   /* Initialize all global variables */
   sfnValue = 0;
   slotValue = 0;
   memset(&ueDb, 0, sizeof(UeDb));

   /* Send Stop indication to MAC */
   sleep(1);
   l1BuildAndSendStopInd();
}

/*******************************************************************
 *
 * @brief Create/cancel thread for generating slot indication 
 *
 * @details
 *
 *    Function : l1HdlSlotIndicaion
 *
 *    Functionality: Create/cancel thread for generating slot indication
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void l1HdlSlotIndicaion(bool stopSlotInd)
{
   Pst pst;
   Buffer *mBuf = NULLP;

   if(!stopSlotInd)
   {
      DU_LOG("\nPHY_STUB: Sending start slot indication event to self");
      memset(&pst, 0, sizeof(Pst));
      FILL_PST_PHY_TO_PHY(pst, EVT_PHY_START_SLOT_IND);
      ODU_GET_MSG_BUF(pst.region, pst.pool, &mBuf);
      ODU_POST_TASK(&pst, mBuf);
   }
   else
   {
      slotIndicationStarted = false;
   }
}

/*******************************************************************
 *
 * @brief Handles Console input
 *
 * @details
 *
 *    Function : l1ConsoleHandler
 *
 *    Functionality: Handles Console input
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void *l1ConsoleHandler(void *args)
{
   char ch, ch1;
   uint8_t drbIdx = 0, lcgIdx = 0, ueIdx = 0;
   LcgBufferSize lcgBS[MAX_NUM_LOGICAL_CHANNEL_GROUPS];
   /* The below variable is taken for sending specific number of UL Packets  
    * For sendind 4500 Ul packets for three UEs the calculation of
    * [counter * NUM_DRB_TO_PUMP_DATA * MAX_NUM_UE * NUM_UL_PACKETS] must be equal to 4500 */
   uint32_t counter=500; 

   while(true)
   {
      /* Send UL user data to DU when user enters 'd' on console */
      ch = getchar();
      if(ch == 'd')
      {
         while(counter)
         {
            /* Start Pumping data from PHY stub to DU */
            for(ueIdx=0; ueIdx < MAX_NUM_UE; ueIdx++)
            {
               for(drbIdx = 0; drbIdx < NUM_DRB_TO_PUMP_DATA; drbIdx++) //Number of DRB times the loop will run
               {
                  DU_LOG("\nDEBUG  --> PHY STUB: Sending UL User Data[DrbId:%d] for UEIdx %d\n",drbIdx,ueIdx);
                  l1SendUlUserData(drbIdx,ueIdx);
                  /* TODO :- sleep(1) will be removed once we will be able to
                   * send continuous data packet */
                  sleep(1);
               }
            }
            counter--;
         }
      }
      else if(ch =='c')
      {
         /* Send Control PDU from PHY stub to DU */
         DU_LOG("\nDEBUG  --> PHY STUB: Sending Status PDU");
         l1SendStatusPdu();
      }
      else if(ch == 'b')
      {
         memset(lcgBS, 0, (MAX_NUM_LOGICAL_CHANNEL_GROUPS * sizeof(LcgBufferSize)));
         /* Send Control PDU from PHY stub to DU */
         ch1 = getchar();
         for(ueIdx = 0; ueIdx < MAX_NUM_UE; ueIdx++)
         {
            if(ch1 == 'l')
            {
               for(lcgIdx = 0; lcgIdx < NUM_DRB_TO_PUMP_DATA; lcgIdx++)
               {
                  lcgBS[lcgIdx].lcgId = MIN_DRB_LCID + lcgIdx;
                  lcgBS[lcgIdx].bsIdx = lcgIdx + 1;
               }
               l1BuildAndSendBSR(ueIdx, LONG_BSR, lcgBS);
            }
            else if(ch1 == 's')
            {
               lcgIdx = 0;

               lcgBS[lcgIdx].lcgId = MIN_DRB_LCID + lcgIdx;
               lcgBS[lcgIdx].bsIdx = lcgIdx + 1;
               l1BuildAndSendBSR(ueIdx, SHORT_BSR, lcgBS);
            }
         }
      }
      DU_LOG("\n");
      continue;
   }
}

/*******************************************************************
 *
 * @brief Creates thread for handling console input 
 *
 * @details
 *
 *    Function : l1StartConsoleHandler
 *
 *    Functionality: Creates thread for handling console input
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void l1StartConsoleHandler()
{
   uint8_t retVal, threadIdx;
   pthread_t conThrdId[NUM_THREADS];
   pthread_attr_t attr;

   /* Start thread to receive console input */
   pthread_attr_init(&attr);
   pthread_attr_setstacksize(&attr, (size_t)NULLD);
   pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   for(threadIdx =0 ; threadIdx <NUM_THREADS; threadIdx++)
   {
      if(threadIdx == 0)
      {
         retVal = pthread_create(&conThrdId[threadIdx], &attr, l1ConsoleHandler, NULLP);
      }
      else
      {
         retVal = pthread_create(&conThrdId[threadIdx], &attr, startCommunicationBtwPhy, NULLP);
      }
      
      if(retVal != 0)
      {
         DU_LOG("\nERROR  -->  PHY STUB : Thread creation failed. Cause %d", retVal);
      }
   }
   pthread_attr_destroy(&attr);
}

/*******************************************************************
 *
 * @brief function reads the recevied information send by peer PHY 
 *
 * @details
 *
 *    Function : readMsg
 *
 *    Functionality: This function reads the recevied information 
 *    and prints at console
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void readMsg()
{
   char buffer[1024] = {0};

   if(read(socket_fd, buffer, 1024)>0)
   {
      DU_LOG("\n");
      DU_LOG("%s\n",buffer);
      if (strncmp("HANDOVER_IN_PROCESS", buffer, 19) == 0) 
      {
          DU_LOG("\nINFO  --> PHY_STUB : Communication completed in between the source and destination PHY\n");
         //TODO: Trigger for other handover process in target PHY
      }
   }

}

/*******************************************************************
 *
 * @brief function build the message which need to send to target PHY 
 *
 * @details
 *
 *    Function : sendMsg
 *
 *    Functionality: function build the message which need to send 
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void sendMsg()
{
   char *msgToDestinationPhy = "HANDOVER_IN_PROCESS";

   send(socket_fd, msgToDestinationPhy , strlen(msgToDestinationPhy) , 0 );
}

/*******************************************************************
 *
 * @brief This function handles the server functionalities like 
 * binding socket, listen and accept
 *
 * @details
 *
 *    Function : serverPhyConnection
 *
 *    Functionality: This function handles the server functionalities like
 *     binding socket, listen and accept
 *
 * @params[in] struct sockaddr_in serverPhy, struct sockaddr_in
 * clientPhy
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int8_t serverPhyConnection(struct sockaddr_in serverPhy, struct sockaddr_in clientPhy)
{
   int addrlen= sizeof(struct sockaddr_in);

   if (bind(socket_fd, (struct sockaddr *)&serverPhy, sizeof(struct sockaddr_in))<0)
   {
      DU_LOG("\nERROR  --> PHY_STUB : bind failed");
      return RFAILED;
   }
   if (listen(socket_fd, 3) < 0)
   {
      DU_LOG("\nERROR  --> PHY_STUB : listen failed");
      return RFAILED;
   }
   while(1)
   {
      if ((socket_fd = accept(socket_fd, (struct sockaddr *)&clientPhy,
                  (socklen_t*)&addrlen))<0)
      {
         DU_LOG("\nINFO  --> PHY_STUB : Server is waiting");
      }
      else
      {
         DU_LOG("\nINFO  --> PHY_STUB : Server Connected");
         break;
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief this function includes all the functionalities of client side
 * like binding the cliend side socket and connecting to the server
 *
 * @details
 *
 *    Function : clientPhyConnection
 *
 *    Functionality: this function includes all the functionalities of client
 *    side like binding the cliend side socket and connecting to the server
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int8_t clientPhyConnection(struct sockaddr_in serverPhy, struct sockaddr_in  destinationPhy)
{
   if (bind(socket_fd, (struct sockaddr *)&serverPhy, sizeof(struct sockaddr_in ))<0)
   {
      DU_LOG("\nERROR  --> PHY_STUB : bind failed");
      return RFAILED;
   }

   while(1)
   {
      if (connect(socket_fd, (struct sockaddr *)&destinationPhy, sizeof(struct sockaddr_in)) < 0)
      {
         DU_LOG("\nERROR  --> PHY_STUB : Connection Failed");
      }
      else
      {
         DU_LOG("\nINFO  --> PHY_STUB : finally connected");
         break;
      }
   }
   return ROK;

}

/*******************************************************************
 *
 * @brief this function creates the socket for commincation between source and 
 * target phy
 *
 * @details
 *
 *    Function : communicationBtwPhy
 *
 *    Functionality: creates the socket for commincation between source and
 *    target PHY, allocated the ip addresses and sends the request for connection
 *    establisshement and sends the information to each other
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int8_t communicationBtwPhy()
{
   struct sockaddr_in sourcePhy, destinationPhy;

   if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
   {
      DU_LOG("\nERROR  --> PHY_STUB : socket failed");
      return RFAILED; 
   }
   
   sourcePhy.sin_family = AF_INET;
   sourcePhy.sin_port = htons(8080);
   if(inet_pton(AF_INET, SOURCE_DU_IPV4_ADDR, &sourcePhy.sin_addr)<=0)
   {
      DU_LOG("\nERROR  --> PHY_STUB : Invalid sourcePhy/ Address not supported");
      return RFAILED;
   }

   destinationPhy.sin_family = AF_INET;
   destinationPhy.sin_port = htons(8080);
   if(inet_pton(AF_INET, DESTINATION_DU_IPV4_ADDR, &destinationPhy.sin_addr)<=0)
   {
      DU_LOG("\nERROR  --> PHY_STUB : Invalid address/ Address not supported");
      return RFAILED;
   }

   if(strcmp(SOURCE_DU_IPV4_ADDR, "192.168.130.81") == 0)
   {
      if(serverPhyConnection(sourcePhy, destinationPhy) != ROK)
      {
         DU_LOG("\nERROR  --> PHY_STUB : Failed to connect to the server");
         return RFAILED;
      }
   }
   else
   {
      if(clientPhyConnection(sourcePhy, destinationPhy) != ROK)
      {
         DU_LOG("\nERROR  --> PHY_STUB : Failed to connect to the server");
         return RFAILED;
      }
   }

   DU_LOG("\nINFO  --> PHY_STUB : Connection established");
   while(1)
   {
      readMsg();
   }
   return ROK;
}
/*******************************************************************
 *
 * @brief  This function start the process of establishing
 *    communication between the phy stubs
 *
 * @details
 *
 *    Function : startCommunicationBtwPhy 
 *
 *    Functionality: This function start the process of establishing
 *    communication between the phy stubs
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void *startCommunicationBtwPhy(void *args)
{
   if(communicationBtwPhy() != ROK)
   {
      DU_LOG("\nERROR  --> PHY_STUB : Issue in establishing connection between PHY");
   }
   return ROK;
}
/**********************************************************************
         End of file
**********************************************************************/
