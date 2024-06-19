/*******************************************************************************
 * ################################################################################
 * #   Copyright (c) [2017-2019] [Radisys]
 * #
 * #
 * #
 * #   Licensed under the Apache License, Version 2.0 (the "License");
 * #
 * #   you may not use this file except in compliance with the License.
 * #
 * #   You may obtain a copy of the License at
 * #
 * #
 * #
 * #       http://www.apache.org/licenses/LICENSE-2.0
 * #
 * #
 * #
 * #   Unless required by applicable law or agreed to in writing, software
 * #
 * #   distributed under the License is distributed on an "AS IS" BASIS,
 * #
 * #   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * #
 * #   See the License for the specific language governing permissions and
 * #
 * #   limitations under the License.
 * #
 * ################################################################################
 * *******************************************************************************/

/* This functions contains main() for pnf simulator */
#include "common_def.h"
#include <unistd.h>
#include "pnf_stub_p5_msg_hdl.h"
#include "nfapi_interface.h"
#include "pnf_stub_sctp.h"
#include "pnf_stub_p7_udp.h"
#include "pnf_stub.h"

extern PnfGlobalCb pnfCb;

/*********************************************************************************
 * @Brief: Building of Ul Node Sync
 *
 * @Function: buildAndSendUlNodeSync
 *
 * @Description: At PNF , Calculate t1,t2,t3 and builing UL Node Sync
 *
 * @Params [IN]: Buffer 
 * [OUT]: ROK/RFAILED
 *
 * ******************************************************************************/

void fillUlNodeSync(int32_t delta_sfnSlot, nFapi_ul_node_sync_info *ulSyncInfo)
{
    PnfSlotInfo  deltaSfnSlot;

    if(delta_sfnSlot != 0)
    {
       EXTRACT_SFN_SLOT_FROM_DELTA(abs(delta_sfnSlot), deltaSfnSlot);
       if(delta_sfnSlot < 0)
       {
          pnfCb.pnfSlotInfo.sfn -= deltaSfnSlot.sfn;
          pnfCb.pnfSlotInfo.slot -= deltaSfnSlot.slot;
       }
       else
       {
          pnfCb.pnfSlotInfo.sfn += deltaSfnSlot.sfn;
          pnfCb.pnfSlotInfo.slot += deltaSfnSlot.slot;
       }
    }
    else
    {
       DU_LOG("\nINFO   --> NFAPI_PNF: No Delta between PNF and VNF");
    }

    ulSyncInfo->t3 = CALC_TIME_USEC_FROM_SFNSLOT(pnfCb.pnfSlotInfo);
    return;
}

/*********************************************************************************
 * @Brief: Building and Sending Ul Node Sync
 *
 * @Function: buildAndSendUlNodeSync
 *
 * @Description: At PNF , encode all the parameters of UL Node sync and Send to
 *                 VNF via UDP
 *
 * @Params [IN]: Ptr to ulSyncInfo
 * [OUT]: ROK/RFAILED
 *
 * ******************************************************************************/

uint8_t buildAndSendUlNodeSync(nFapi_ul_node_sync_info *ulSyncInfo)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> NFAPI_PNF : Memory allocation failed in start response");
      return RFAILED;
   }
   nfapiFillP7Hdr(mBuf, (sizeof(nFapi_ul_node_sync_info) + sizeof(nFapi_msg_header)), 0, 0);
   nfapiFillMsgHdr(mBuf, 1, TAG_NFAPI_UL_NODE_SYNC, sizeof(nFapi_ul_node_sync_info));

   CMCHKPK(oduPackPostUInt32, ulSyncInfo->t1, mBuf);
   CMCHKPK(oduPackPostUInt32, ulSyncInfo->t2, mBuf);
   CMCHKPK(oduPackPostUInt32, ulSyncInfo->t3, mBuf);

   if(pnfP7UdpSendMsg(mBuf) != ROK)
   { 
      return RFAILED;
   }
   return ROK;
}

/*********************************************************************************
 * @Brief: Process and Handling of Dl Node Sync
 *
 * @Function: pnfDlNodeSyncHandler
 *
 * @Description: At PNF , extract all the parameters of DL Node sync and uses
 *                t1, delta_sfnSlot  while processing and builing UL Node Sync
 *
 * @Params [IN]: Buffer 
 * [OUT]: ROK/RFAILED
 *
 * ******************************************************************************/

uint8_t pnfDlNodeSyncHandler(Buffer *mBuf)
{
    nFapi_dl_node_sync_info dlNodeSync;
    nFapi_ul_node_sync_info ulSyncInfo;
    PnfSlotInfo             vnfFrameInfo;

    CMCHKPK(oduUnpackUInt32, &(dlNodeSync.t1), mBuf);
    CMCHKPK(SUnpkS32, &(dlNodeSync.delta_sfnSlot), mBuf);
    CMCHKPK(oduUnpackUInt8, &(dlNodeSync.scs), mBuf);

    EXTRACT_SFN_SLOT_FROM_TIME(dlNodeSync.t1, vnfFrameInfo);

    ulSyncInfo.t1 = dlNodeSync.t1;
    ulSyncInfo.t2 = CALC_TIME_USEC_FROM_SFNSLOT(pnfCb.pnfSlotInfo);
    
    fillUlNodeSync(dlNodeSync.delta_sfnSlot, &ulSyncInfo);
    
    buildAndSendUlNodeSync(&ulSyncInfo);
    return ROK;
}

/*********************************************************************************
 *
 * @Function Name: pnfP7MsgHandler
 *
 *
 * @Functionality: 
 *    Handles the P7 MEssages at PNF SIM
 *
 * @Params [IN]: Message Buffer received at UDP NFAPI P7 Interface
 *
 * *******************************************************************************/
uint8_t  pnfP7MsgHandler(Buffer *mBuf)
{
   nFapi_p7_hdr p7Hdr;
   nFapi_msg_header msgHdr;
   uint8_t ret = ROK;

   nFapiExtractP7Hdr(&p7Hdr, mBuf);
   nFapiExtractMsgHdr(&msgHdr, mBuf);

   switch(msgHdr.msg_id)
   {
      case TAG_NFAPI_DL_NODE_SYNC:
         {
            DU_LOG("\nINFO   --> NFAPI_PNF: DL_NODE_SYNC recevied.");
            ret = pnfDlNodeSyncHandler(mBuf);
            break;
         }
      default:
      {
         DU_LOG("\nERROR  --> NFAPI_PNF: Wrong MSGID of NFAPI P7 Message:%d",msgHdr.msg_id);
         ret = RFAILED;
         break;
      }
   }

   if(ret == RFAILED)
   {
      return RFAILED;
   }
   return ret;
}

/**********************************************************************
End of file
**********************************************************************/
