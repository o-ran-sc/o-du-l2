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

uint16_t l1BuildAndSendSlotIndication();
pthread_t thread = 0;

void *GenerateTicks(void *arg)
{
   int     milisec = 1;        /* 1ms */
   struct timespec req = {0};

   req.tv_sec = 0;
   req.tv_nsec = milisec * 1000000L;

   while(1)
   {
      nanosleep(&req, (struct timespec *)NULL);
      
      /* Send Slot indication indication to lower mac */
      l1BuildAndSendSlotIndication();
   }
   return((void *)NULLP);
}

void l1HdlSlotIndicaion(bool stopSlotInd)
{
   int ret;

   if(!stopSlotInd)
   {
      ret = pthread_create(&thread, NULL, GenerateTicks, NULL);
      if(ret)
      {
         DU_LOG("\nPHY_STUB: Unable to create thread");
      }
   }
   else
   {
      ret = pthread_cancel(thread);
      if(ret)
      {
         DU_LOG("\nPHY_STUB: Unable to stop thread");
      }
   }
}

/**********************************************************************
         End of file
**********************************************************************/
