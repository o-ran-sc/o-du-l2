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
#ifdef NFAPI_ENABLED

#include "common_def.h"
#include "mac_utils.h"
#include "lwr_mac.h"
#include "nfapi_interface.h"
#include "nfapi_common.h"
#include "nfapi_udp_p7.h"

extern NfapiVnfDb vnfDb;

/*******************************************************************
 *
 * @brief Build and Sends DL_NODE_SYNC
 *
 * @details
 *
 *    Function : nfapiBuildAndSendDlNodeSync
 *
 *    Functionality:
 *           Build and Sends DL Node Synch
 *        (Params can be referred to Table 4-1 Dl Node Sync Parameters)
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t nfapiBuildAndSendDlNodeSync()
{
   uint32_t t1 = 0; /*Offset from VNF SFN/Slot 0/0 to the DL Node Sync Tx*/
   uint8_t  scs = 0;
   Buffer   *mBuf = NULLP;
   
   if(ODU_GET_MSG_BUF(MAC_MEM_REGION, MAC_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> NFAPI_VNF : Memory allocation failed in packPnfParamReq");
      return RFAILED;
   }
  
   
   t1 = CALC_TIME_USEC_FROM_SFNSLOT(vnfDb.vnfP7Info.p7SyncInfo.frameInfo);
   scs = 15 * (pow(2, vnfDb.numerology));
  
   nfapiFillP7Hdr(mBuf,( sizeof(nFapi_dl_node_sync_info) + sizeof(nFapi_msg_header)), 0, 0);
   nfapiFillMsgHdr(mBuf, vnfDb.vnfP7Info.p7SyncInfo.phyId, TAG_NFAPI_DL_NODE_SYNC, sizeof(nFapi_dl_node_sync_info));
   CMCHKPK(oduPackPostUInt32, t1, mBuf);
   CMCHKPK(SPkPostS32, vnfDb.vnfP7Info.p7SyncInfo.delta_sfn_slot, mBuf);
   CMCHKPK(oduPackPostUInt8, scs, mBuf);

   return(nfapiP7UdpSendMsg(mBuf));
}

#endif
