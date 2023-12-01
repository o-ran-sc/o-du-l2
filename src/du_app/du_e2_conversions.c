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
#include "du_app_mac_inf.h"
#include "du_e2ap_mgr.h"
#include "E2nodeComponentInterfaceType.h"

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

/*******************************************************************
 *
 * @brief Converts DU specific failure cause to E2 interface
 *        failure cause
 *
 * @details
 *
 *    Function : convertDuCauseToE2Cause
 *
 *    Functionality: Converts DU specific failure cause to E2 
 *       interface failure cause
 *
 * @params[in] DU specific failure cause
 *             E2 specific failure cause
 *
 * @return void
 *
 * ****************************************************************/
void convertDuCauseToE2Cause(CauseOfResult l2Cause, E2FailureCause *failureCause)
{
   switch(l2Cause)
   {
      case STATS_ID_NOT_FOUND:
      case PARAM_INVALID:
         {
            failureCause->causeType = E2_RIC_REQUEST;
            failureCause->cause = E2_ACTION_NOT_SUPPORTED;
            break;
         }
      case RESOURCE_UNAVAILABLE:
         {
            failureCause->causeType = E2_RIC_REQUEST;
            failureCause->cause = E2_FUNCTION_RESOURCE_LIMIT;
            break;
         }
      default:
         {
            failureCause->causeType = E2_RIC_REQUEST;
            failureCause->cause = E2_RIC_REQUEST_CAUSE_UNSPECIFIED;
            break;
         }
   }
}

/************************************************************************
 *
 * @brief Converts InterfaceType into E2nodeComponentInterfaceType value 
 *
 * @details
 *
 *    Function : convertInterfaceToE2ComponentInterfaceType
 *
 *    Functionality: Convert InterfaceType into E2nodeComponentInterfaceType
 *    value
 *
 * @params[in] Actual value of interface
 * @return Enum value of e2 interface
 *
 * **********************************************************************/

uint8_t convertInterfaceToE2ComponentInterfaceType(uint8_t interface)
{
   switch(interface)
   {
      case NG:
         return E2nodeComponentInterfaceType_ng;

      case XN:
         return E2nodeComponentInterfaceType_xn;

      case E1:
         return E2nodeComponentInterfaceType_e1;

      case F1:
         return E2nodeComponentInterfaceType_f1;

      case W1:
         return E2nodeComponentInterfaceType_w1;

      case S1:
         return E2nodeComponentInterfaceType_s1;

      case X2:
         return E2nodeComponentInterfaceType_x2;
   }
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Converts MAC-specific Measurement type to E2 interface
 *        specific measurement type
 *
 * @details
 *
 *    Function : convertMacMeasTypeToE2MeasType
 *
 *    Functionality: Converts MAC-specific Measurement type to E2 
 *       interface specific measurement type
 *
 * @params[in] MAC specific measurement type
 *             E2 interface specific measurement type
 *
 * @return uint8_t
 *
 * ****************************************************************/
uint8_t convertMacMeasTypeToE2MeasType(MacMeasurementType macMeasName, char *e2MeasName)
{
   uint8_t measNameLen = 0;

   switch(macMeasName)
   {
      case MAC_DL_TOTAL_PRB_USAGE:
         {
            measNameLen = strlen("RRU.PrbTotDl");
            memcpy(e2MeasName, "RRU.PrbTotDl", measNameLen+1);
            break;
         }
      case MAC_UL_TOTAL_PRB_USAGE:
         {
            measNameLen = strlen("RRU.PrbTotUl");
            memcpy(e2MeasName, "RRU.PrbTotUl", measNameLen+1);
            break;
         }
      default:
         {
             DU_LOG("\nERROR  -->  E2AP : MAC mesurement type [%d] not supported", macMeasName);
             return RFAILED;
         }
   }
   return ROK;
}

/**********************************************************************
  End of file
 **********************************************************************/
