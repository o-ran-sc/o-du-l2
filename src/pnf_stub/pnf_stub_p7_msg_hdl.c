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
 *
 * @Function Name: pnfDlNodeSyncHandler
 *
 *
 * @Functionality: 
 *    Processes DL Node Sync i.e. Extracts the DL_NODE_SYNC and will generate 
 *     UL_NODE_SYNC
 *
 * @Params [IN]: Message Buffer received at UDP NFAPI P7 Interface
 *
 * *******************************************************************************/
uint8_t pnfDlNodeSyncHandler(Buffer *mBuf)
{
    nFapi_dl_node_sync_info dlNodeSync;
    PnfSlotInfo             vnfFrameInfo;

    CMCHKPK(oduUnpackUInt32, &(dlNodeSync.t1), mBuf);
    CMCHKPK(SUnpkS32, &(dlNodeSync.delta_sfnSlot), mBuf);
    CMCHKPK(oduUnpackUInt8, &(dlNodeSync.scs), mBuf);

    DU_LOG("\n PNF_NFAPI: t1:%u, delta:%d, scs:%d",dlNodeSync.t1, dlNodeSync.delta_sfnSlot, dlNodeSync.scs);

    EXTRACT_SFN_SLOT_FROM_TIME(dlNodeSync.t1, vnfFrameInfo);
    DU_LOG("\n dl node sync at VNF SFN:SLOT:%d/%d",vnfFrameInfo.sfn, vnfFrameInfo.slot);
    //buildAndSendUlNodeSync(dlNodeSync.t1, dlNodeSync.delta_sfnSlot);
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
