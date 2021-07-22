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

uint8_t l1SendUlUserData();
uint8_t l1SendStatusPdu();
uint16_t l1BuildAndSendSlotIndication();
uint16_t l1BuildAndSendStopInd();
pthread_t thread = 0;

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
   int     milisec = 0.5;        /* 0.5ms */
#else
   int     milisec = 1;          /* 1ms */
#endif
   struct timespec req = {0};

   slotIndicationStarted = true;
   req.tv_sec = 0;
   req.tv_nsec = milisec * 1000000L;

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
   char ch;
   while(true)
   {
      /* Send UL user data to DU when user enters 'd' on console */
      if((ch = getchar()) == 'd')
      {
         /* Start Pumping data from PHY stub to DU */
         DU_LOG("\nDEBUG  --> PHY STUB: Sending UL User Data");
         l1SendUlUserData();
      }
      else if((ch = getchar()) == 'c')
      {
         /* Send Control PDU from PHY stub to DU */
	 DU_LOG("\nDEBUG  --> PHY STUB: Sending Status PDU");
	 l1SendStatusPdu();
      }
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
   uint8_t retVal;
   pthread_t conThrdId;
   pthread_attr_t attr;

   /* Start thread to receive console input */
   pthread_attr_init(&attr);
   pthread_attr_setstacksize(&attr, (size_t)NULLD);
   pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   retVal = pthread_create(&conThrdId, &attr, l1ConsoleHandler, NULLP);
   if(retVal != 0)
   {
      DU_LOG("\nERROR  -->  PHY STUB : Thread creation failed. Cause %d", retVal);
   }
   pthread_attr_destroy(&attr);

}

/**********************************************************************
         End of file
**********************************************************************/
