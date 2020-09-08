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

#include "common_def.h"
#include "du_app_rlc_inf.h"
#include "rlc_utils.h"
#include "kwu.h"
#include "kw.h"
#include "kwu.x"

uint64_t sduId = 0;

/*******************************************************************
 *
 * @brief Process the DL RRC Message from DU APP
 *
 * @details
 *
 *    Function : RlcProcDlRrcMsgTrans
 *
 *    Functionality: Process the DL RRC Message from DU APP
 *
 * @params[in] Post structure
 *             DL RRC Message info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t RlcProcDlRrcMsgTrans(Pst *pst, RlcDlRrcMsgInfo *dlRrcMsgInfo)
{
   uint16_t      copyLen;
   Buffer        *mBuf;
   KwuDatReqInfo *datReqInfo;

   KW_SHRABL_STATIC_BUF_ALLOC(RLC_MEM_REGION_DL, RLC_POOL, datReqInfo, sizeof(KwuDatReqInfo));
   if(!datReqInfo)
   {
      DU_LOG("\nRLC : Memory allocation failed in RlcProcDlRrcMsgTrans");
      return RFAILED;
   }

   datReqInfo->rlcId.rbId = dlRrcMsgInfo->rbId;
   datReqInfo->rlcId.rbType = dlRrcMsgInfo->rbType;
   datReqInfo->rlcId.ueId = dlRrcMsgInfo->ueIdx;
   datReqInfo->rlcId.cellId = dlRrcMsgInfo->cellId;
   datReqInfo->lcType = dlRrcMsgInfo->lcType;
   datReqInfo->sduId = ++sduId;

   /* Copy fixed buffer to message */
   if(SGetMsg(RLC_MEM_REGION_UL, RLC_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nRLC : Memory allocation failed at RlcMacProcUlData");
      return RFAILED;
   }
   reverseFixBuf(dlRrcMsgInfo->rrcMsg, dlRrcMsgInfo->msgLen);
   SCpyFixMsg(dlRrcMsgInfo->rrcMsg, mBuf, 0, dlRrcMsgInfo->msgLen, \
	 (MsgLen *)&copyLen);

   KwUiKwuDatReq(pst, datReqInfo, mBuf);

   /* Free memory allocated by du app */
   KW_SHRABL_STATIC_BUF_FREE(RLC_MEM_REGION_DL, RLC_POOL, datReqInfo, sizeof(KwuDatReqInfo));
   KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, dlRrcMsgInfo->rrcMsg, dlRrcMsgInfo->msgLen);
   KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, dlRrcMsgInfo, sizeof(RlcDlRrcMsgInfo));
   return ROK;
}

/**********************************************************************
         End of file
**********************************************************************/
