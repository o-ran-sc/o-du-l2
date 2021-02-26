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
#include "lphy_stub.h"
#include "du_log.h"

uint8_t l1SendUlUserData();
uint8_t l1SendStatusPdu();
uint16_t l1BuildAndSendSlotIndication();
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
void *GenerateTicks(void *arg)
{
#ifdef NR_TDD
   int     milisec = 0.5;        /* 0.5ms */
#else
   int     milisec = 1;          /* 1ms */
#endif
   struct timespec req = {0};

   req.tv_sec = 0;
   req.tv_nsec = milisec * 1000000L;

   while(1)
   {
      clock_nanosleep(CLOCK_REALTIME, 0, &req, NULL); 
      /* Send Slot indication indication to lower mac */
      l1BuildAndSendSlotIndication();
   }
   return((void *)NULLP);
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
   int ret;

   if(!stopSlotInd)
   {
      ret = pthread_create(&thread, NULL, GenerateTicks, NULL);
      if(ret)
      {
         DU_LOG("\nERROR  -->  PHY_STUB: Unable to create thread");
      }
   }
   else
   {
      ret = pthread_cancel(thread);
      if(ret)
      {
         DU_LOG("\nERROR  -->  PHY_STUB: Unable to stop thread");
      }
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
