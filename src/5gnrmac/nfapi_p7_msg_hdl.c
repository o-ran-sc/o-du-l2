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

/* This file is the entry point for P7 Msg Handling */
/*Reference SCF225 version: 225.2.1 */

#include "common_def.h"
#include "nfapi_interface.h"
#include "nfapi_common.h"
#include "nfapi_vnf_fsm.h"

extern NfapiVnfDb vnfDb;

/*Following parameters can be referred to Table 4-1 Dl Node Sync Parameters*/
void buildAndSendDlNodeSync()
{
   uint32_t t1 = 0; /*Offset from VNF SFN/Slot 0/0 to the DL Node Sync Tx*/
   int32_t  delta_sfn_slot = 0;

   
     
}

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
void nFapiGenerateTicks()
{
   float  slotDur_ms = 1/pow(2,vnfDb.numerology);
   struct timespec currTime = {0};
   uint8_t ratio = 2;
   uint32_t currTime_ns = 0;
   static uint32_t nextTime_ns = 0;

   /* Currently the code takes longer that one slot indication to execute.
    * Hence, multiplying slot time interval by 2 in order to give enough time 
    * for L2 to complete one slot processing.
    * The ratio must be removed once code optimization is complete */

   clock_gettime(CLOCK_REALTIME, &currTime);
   currTime_ns = currTime.tv_sec * 1000000000 + currTime.tv_nsec;

      //DU_LOG("\nVNF_NFAPI : Sangeeta SFN/Slot:%d,%d",\
                          vnfDb.vnfP7Info.p7SyncInfo.sfn, vnfDb.vnfP7Info.p7SyncInfo.slot);
    DU_LOG("\nVNF_NFAPI : Sangeeta CurrTime :%llu", currTime_ns);
   if((vnfDb.vnfP7Info.p7SyncInfo.slot == 0xFF && vnfDb.vnfP7Info.p7SyncInfo.sfn == 0xFFFF) || (nextTime_ns == currTime_ns))
   {
      if(vnfDb.vnfP7Info.p7SyncInfo.slot == 0xFF && vnfDb.vnfP7Info.p7SyncInfo.sfn == 0xFFFF)
      {
         vnfDb.vnfP7Info.t_ref_ns = currTime_ns;
         DU_LOG("\nVNF_NFAPI : Starting to generate slot indications t_ref:%llu, slotDur:%f",\
                          vnfDb.vnfP7Info.t_ref_ns, slotDur_ms);
      }
      vnfDb.vnfP7Info.p7SyncInfo.sfn++;
      vnfDb.vnfP7Info.p7SyncInfo.slot++;
      //sendSlotIndToMac;
      nextTime_ns = currTime_ns + (slotDur_ms * 1000000L * ratio);
      DU_LOG("\nVNF_NFAPI : NextTime:%llu, SFN/Slot:%d,%d",\
                           nextTime_ns, vnfDb.vnfP7Info.p7SyncInfo.sfn, vnfDb.vnfP7Info.p7SyncInfo.slot);
   }
}

