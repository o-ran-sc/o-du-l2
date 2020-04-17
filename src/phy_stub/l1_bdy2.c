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

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "lphy_stub.h"

void *GenerateTicks(void *arg)
{
   uint8_t counter = 100;
   while(counter)
   {
      sleep(1);
      printf("\nPHY_STUB: SLOT indication");
      /* Send Slot indication indication to lower mac */
      buildAndSendSlotIndication();
      counter--;
   }
}

void duStartSlotIndicaion()
{
   pthread_t thread;
   int ret;

   ret = pthread_create(&thread, NULL, GenerateTicks, NULL);
   if(ret)
   {
      printf("\nPHY_STUB: Unable to create thread");
   }
}

/**********************************************************************
         End of file
**********************************************************************/
