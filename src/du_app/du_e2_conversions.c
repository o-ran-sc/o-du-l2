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

/* This file contains functions that maps values received in F1AP message with
 * its corresponding values used in DU and vice-versa */

#include "common_def.h"
#include "TimeToWaitE2.h"

/************************************************************************
 *
 * @brief Converts enum values into actual value of E2 wait timer
 *
 * @details
 *
 *    Function : convertE2WaitTimerEnumToValue
 *
 *    Functionality: Converts enum values into actual value of E2 wait timer 
 *
 * @params[in] Enum value of e2 wait timer 
 * @return Actual value of e2 wait timer
 *
 * **********************************************************************/

uint8_t convertE2WaitTimerEnumToValue(uint8_t timerToWait)
{
   switch(timerToWait)
   {
      case TimeToWaitE2_v1s:
         return 1;
         
      case TimeToWaitE2_v2s:
         return 2;
         
      case TimeToWaitE2_v5s:
         return 5;
         
      case TimeToWaitE2_v10s:
         return 10;
         
      case TimeToWaitE2_v20s:
         return 20;
         
      case TimeToWaitE2_v60s:
         return 60;
         
      default:
         DU_LOG("\nERROR  -->  F1AP: Invalid value of E2 Wait timer");
   }
   return RFAILED;
}

/**********************************************************************
  End of file
 **********************************************************************/
