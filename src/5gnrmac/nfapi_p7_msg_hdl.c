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
   uint8_t  scs = 0;
   Buffer   *mBuf = NULLP;
   
   if(ODU_GET_MSG_BUF(MAC_MEM_REGION, MAC_POOL, &mBuf) != ROK)
   {
      DU_LOG("ERROR  --> NFAPI_VNF : Memory allocation failed in packPnfParamReq");
      return RFAILED;
   }
  
   
   vnfDb.vnfP7Info.p7SyncInfo.prev_t1 = CALC_TIME_USEC_FROM_SFNSLOT(vnfDb.vnfP7Info.p7SyncInfo.frameInfo);
   scs = 15 * (pow(2, vnfDb.numerology));
  
   nfapiFillP7Hdr(mBuf,( sizeof(nFapi_dl_node_sync_info) + sizeof(nFapi_msg_header)), 0, 0);
   nfapiFillMsgHdr(mBuf, vnfDb.vnfP7Info.p7SyncInfo.phyId, TAG_NFAPI_DL_NODE_SYNC, sizeof(nFapi_dl_node_sync_info));
   CMCHKPK(oduPackPostUInt32, vnfDb.vnfP7Info.p7SyncInfo.prev_t1, mBuf);
   CMCHKPK(SPkPostS32, vnfDb.vnfP7Info.p7SyncInfo.delta_sfn_slot, mBuf);
   CMCHKPK(oduPackPostUInt8, scs, mBuf);

   memset(&vnfDb.vnfP7Info.p7SyncInfo.delta_sfn_slot, 0, sizeof(int32_t)); 
   return(nfapiP7UdpSendMsg(mBuf));
}

/*******************************************************************
 *
 * @brief Validating T1 received from PNF@ UL_NODE_SYNC
 *
 * @details
 *
 *    Function : nfapiValidatingT1
 *
 *    Functionality:
 *           Matching the T1 received from UL Node Sync to validate that
 *           correct UL Node Sync or not
 *
 * @params[in] T1 received from UL_NODE_SYNC
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t nfapiValidatingT1(uint32_t ulNodeSyncT1)
{
   DU_LOG("DEBUG  --> NFAPI_VNF: t1@VNF:%u, t1_from_pnf:%u",\
            vnfDb.vnfP7Info.p7SyncInfo.prev_t1, ulNodeSyncT1);
   
   if(vnfDb.vnfP7Info.p7SyncInfo.prev_t1 == ulNodeSyncT1)
   {
      DU_LOG("INFO   --> NFAPI_VNF: T1 matching so this UL Node Sync can be processed");
      return ROK;
   }
   else
   {
      DU_LOG("ERROR  --> NFAPI_VNF: Mismatch T1");
      return RFAILED;
   }
}


/*******************************************************************
 *
 * @brief Processes and handles UL_NODE_SYNC
 *
 * @details
 *
 *    Function : nfapiP7ProcUlNodeSync
 *
 *    Functionality:
 *           Processes UL Node Sync and determine if VNF and PNF are in Sync
 *           If in un-sync, calculate the slot difference.
 *
 * @params[in] Buffer *mBuf 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
/*Following parameters can be referred to Table 4-2 Ul Node Sync Parameters*/
uint8_t nfapiP7ProcUlNodeSync(Buffer *mBuf)
{
   int8_t                  cmpStatus = 0;
   uint8_t                 ret = ROK;
   uint16_t                numSlotDiff = 0;
   NfapiFrameInfo          t3_sfnSlot;
   nFapi_ul_node_sync_info ulNodeSyncInfo; 

   CMCHKPK(oduUnpackUInt32, &(ulNodeSyncInfo.t1), mBuf);
   CMCHKPK(oduUnpackUInt32, &(ulNodeSyncInfo.t2), mBuf);
   CMCHKPK(oduUnpackUInt32, &(ulNodeSyncInfo.t3), mBuf);

   if(nfapiValidatingT1(ulNodeSyncInfo.t1) == ROK)
   {
      memset(&vnfDb.vnfP7Info.p7SyncInfo.delta_sfn_slot, 0, sizeof(int32_t)); 
      vnfDb.vnfP7Info.p7SyncInfo.prev_t2 = ulNodeSyncInfo.t2;
      vnfDb.vnfP7Info.p7SyncInfo.prev_t3 = ulNodeSyncInfo.t3;
   
      EXTRACT_SFN_SLOT_FROM_TIME(vnfDb.vnfP7Info.p7SyncInfo.prev_t3, t3_sfnSlot);
      CMP_INFO(t3_sfnSlot, vnfDb.vnfP7Info.p7SyncInfo.frameInfo, cmpStatus);
      if(cmpStatus == 0)
      {
         vnfDb.vnfP7Info.p7SyncInfo.inSync = TRUE;
      }
      else
      {
         vnfDb.vnfP7Info.p7SyncInfo.inSync = FALSE;
         if(cmpStatus == -1)
         { /*PNF is ahead.*/
            DU_LOG("INFO   --> NFAPI_VNF: PNF is ahead.");
            CALC_TIME_DIFF(t3_sfnSlot, vnfDb.vnfP7Info.p7SyncInfo.frameInfo, numSlotDiff);
         }
         else if(cmpStatus == 1)
         {
            DU_LOG("INFO   --> NFAPI_VNF: VNF is ahead.");
            CALC_TIME_DIFF(vnfDb.vnfP7Info.p7SyncInfo.frameInfo, t3_sfnSlot, numSlotDiff);
         }
         vnfDb.vnfP7Info.p7SyncInfo.delta_sfn_slot = cmpStatus * numSlotDiff;
         ret = nfapiBuildAndSendDlNodeSync();
      }
      DU_LOG("DEBUG  --> NFAPI_VNF: delta:%d insyn:%d", vnfDb.vnfP7Info.p7SyncInfo.delta_sfn_slot, vnfDb.vnfP7Info.p7SyncInfo.inSync);
      return ret;
   }
   else
   {
      DU_LOG("ERROR  --> NFAPI_VNF: T1 Validation failed");
      return RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Processed the NFAPI P7 message from UDP socket 
 *
 * @details
 *
 *    Function : nfapiP7MsgHandler
 *
 *    Functionality:
 *      Extracts the Hdr of P7 msgs and re-direct msgs to its particular
 *      handling.
 *      
 *
 * @params[in] Buffer received in UDP
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t  nfapiP7MsgHandler(Buffer *mBuf)
{
   nFapi_p7_hdr p7Hdr;
   nFapi_msg_header msgHdr;

   nFapiExtractP7Hdr(&p7Hdr, mBuf);
   nFapiExtractMsgHdr(&msgHdr, mBuf);

   switch(msgHdr.msg_id)
   {
      case TAG_NFAPI_UL_NODE_SYNC:
      {
         DU_LOG("INFO  --> NFAPI_VNF: Received UL Node Synch");
         nfapiP7ProcUlNodeSync(mBuf);
         break;
      }

      default:
      {
         DU_LOG("ERROR --> NFAPI_VNF: Wrong MsgId:%d", msgHdr.msg_id);
         return RFAILED;
      }
   }

   return ROK;
}

#endif
