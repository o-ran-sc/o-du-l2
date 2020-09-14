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

/* header include files (.h) */
#include "common_def.h"
#include "rgu.h"           /* RGU interface includes*/
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */


/* header/extern include files (.x) */
#include "rgu.x"           
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "kw_err.h"
#include "kw.x"

#include "du_app_rlc_inf.h"
#include "rlc.h"
#include "rlc_utils.h"

/*******************************************************************
 *
 * @brief Fills RLC UL UE Cfg Rsp from RlcCRsp 
 *
 * @details
 *
 *    Function : fillRlcUlUeCfgRsp
 *
 *    Functionality: 
 *     Fills RLC UL UE Cfg Rsp from RlcCRsp
 * 
 *  @params[in]  Pointer to RlcCfgCfm
 *               Pointer to RlcUeCfgRsp
 *
 *  @return void
 * 
 *****************************************************************/

void fillRlcUlUeCfgRsp(RlcUeCfgRsp *rlcCfgRsp, RlcCfgCfmInfo *rlcCRsp)
{
   uint8_t idx;
 
   rlcCfgRsp->cellId = rlcCRsp->cellId;
   rlcCfgRsp->ueIdx  = rlcCRsp->ueId;
   for(idx = 0; idx < rlcCRsp->numEnt; idx++)
   {
      if(rlcCRsp->entCfgCfm[idx].status.status == CKW_CFG_CFM_OK)
      {
         rlcCfgRsp->result = RLC_DU_APP_RSP_OK;
	 rlcCfgRsp->reason = rlcCRsp->entCfgCfm[idx].status.reason;
      }
      else
      {
         rlcCfgRsp->result = RLC_DU_APP_RSP_NOK;
	 rlcCfgRsp->reason = rlcCRsp->entCfgCfm[idx].status.reason;
      }
   }
}

/*******************************************************************
 *
 * @brief Fills the entity mode and direction compiling to seed code
 *
 * @details
 *
 *    Function : fillEntModeAndDir
 *
 *    Functionality:
 *      Fills the entity mode and direction compiling to seed code
 *
 * @params[in] Pointer to entMode
 *             Pointer to direction
 *             RlcMode Param 
 * @return void
 * ****************************************************************/

void fillEntModeAndDir(uint8_t *entMode, uint8_t *direction, RlcMode rlcMode)
{
   switch(rlcMode)
   {
      case RLC_AM:
         *entMode   = CM_LTE_MODE_AM;
         *direction = RLC_CFG_DIR_BOTH;
         break;
      case RLC_UM_BI_DIRECTIONAL:
         *entMode = CM_LTE_MODE_UM;
         *direction = RLC_CFG_DIR_BOTH;
         break;
      case RLC_UM_UNI_DIRECTIONAL_UL:
         *entMode = CM_LTE_MODE_UM;
         *direction = RLC_CFG_DIR_UL;
         break;
      case RLC_UM_UNI_DIRECTIONAL_DL:
         *entMode = CM_LTE_MODE_UM;
         *direction = RLC_CFG_DIR_DL;
         break;
      default : 
         DU_LOG("\nRLC: Rlc Mode invalid %d", rlcMode);
    break;
   }
}
/*******************************************************************
 *
 * @brief Handles Ue Create Request from DU APP
 *
 * @details
 *
 *    Function : RlcUlProcUeCreateReq
 *
 *    Functionality:
 *      Handles Ue create Request from DU APP
 *
 * @params[in] Post structure pointer
 *             RlcUeCfg pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t RlcUlProcUeCreateReq(Pst *pst, RlcUeCfg *ueCfg)
{
   uint8_t idx;
   uint8_t ret = ROK;
   uint8_t lChRbIdx;
   RlcCfgInfo *rlcUeCfg = NULLP;

   RlcCb *rlcUeCb = NULLP;
   rlcUeCb = RLC_GET_RLCCB(pst->dstInst);
   RLC_ALLOC(rlcUeCb, rlcUeCfg, sizeof(RlcCfgInfo));
   if(rlcUeCfg)
   {
      memset(rlcUeCfg, 0, sizeof(RlcCfgInfo));

      rlcUeCfg->ueId    = ueCfg->ueIdx;
      rlcUeCfg->cellId  = ueCfg->cellId;
      rlcUeCfg->numEnt  = ueCfg->numLcs;
      rlcUeCfg->transId = getTransId();
 
      for(idx = 0; idx < ueCfg->numLcs; idx++)
      {
         lChRbIdx = 0;
         rlcUeCfg->entCfg[idx].rbId           = ueCfg->rlcBearerCfg[idx].rbId;
         rlcUeCfg->entCfg[idx].rbType         = ueCfg->rlcBearerCfg[idx].rbType;   // SRB or DRB
         rlcUeCfg->entCfg[idx].lCh[lChRbIdx].lChId   = ueCfg->rlcBearerCfg[idx].lcId;   
         rlcUeCfg->entCfg[idx].lCh[lChRbIdx].type    = ueCfg->rlcBearerCfg[idx].lcType;
         fillEntModeAndDir(&rlcUeCfg->entCfg[idx].entMode, &rlcUeCfg->entCfg[idx].dir,\
            ueCfg->rlcBearerCfg[idx].rlcMode);
         rlcUeCfg->entCfg[idx].cfgType        = CKW_CFG_ADD;
         switch(rlcUeCfg->entCfg[idx].entMode)
         {

            case CM_LTE_MODE_AM:
            {
               /* DL AM INFO */
               rlcUeCfg->entCfg[idx].m.amInfo.dl.snLen = ueCfg->rlcBearerCfg[idx].u.amCfg.dlAmCfg.snLenDl; 
               rlcUeCfg->entCfg[idx].m.amInfo.dl.pollRetxTmr = ueCfg->rlcBearerCfg[idx].u.amCfg.dlAmCfg.pollRetxTmr;
               rlcUeCfg->entCfg[idx].m.amInfo.dl.pollPdu = ueCfg->rlcBearerCfg[idx].u.amCfg.dlAmCfg.pollPdu; 
               rlcUeCfg->entCfg[idx].m.amInfo.dl.pollByte = ueCfg->rlcBearerCfg[idx].u.amCfg.dlAmCfg.pollByte; 
               rlcUeCfg->entCfg[idx].m.amInfo.dl.maxRetx = ueCfg->rlcBearerCfg[idx].u.amCfg.dlAmCfg.maxRetxTh;

               /* UL AM INFO */
	       lChRbIdx++;   //lChRbIdx = 1, indicates UL AM
               rlcUeCfg->entCfg[idx].lCh[lChRbIdx].lChId   = ueCfg->rlcBearerCfg[idx].lcId;   
               rlcUeCfg->entCfg[idx].lCh[lChRbIdx].type    = ueCfg->rlcBearerCfg[idx].lcType;
               rlcUeCfg->entCfg[idx].m.amInfo.ul.snLen = ueCfg->rlcBearerCfg[idx].u.amCfg.ulAmCfg.snLenUl; 
               rlcUeCfg->entCfg[idx].m.amInfo.ul.staProhTmr = ueCfg->rlcBearerCfg[idx].u.amCfg.ulAmCfg.statProhTmr;
               rlcUeCfg->entCfg[idx].m.amInfo.ul.reOrdTmr = ueCfg->rlcBearerCfg[idx].u.amCfg.ulAmCfg.reAssemTmr;
               break;
            }
            case CM_LTE_MODE_UM:
            {
               /* UL UM CONFIG */
               rlcUeCfg->entCfg[idx].m.umInfo.ul.snLen = ueCfg->rlcBearerCfg[idx].u.umBiDirCfg.ulUmCfg.snLenUlUm; 
               rlcUeCfg->entCfg[idx].m.umInfo.ul.reOrdTmr = ueCfg->rlcBearerCfg[idx].u.umBiDirCfg.ulUmCfg.reAssemTmr;

               /* DL UM CONFIG */
               rlcUeCfg->entCfg[idx].m.umInfo.dl.snLen = ueCfg->rlcBearerCfg[idx].u.umBiDirCfg.dlUmCfg.snLenDlUm; 
               break;
            }
            default:
               break;
         }/* End of switch(entMode) */
      }
      ret = RlcProcCfgReq(pst, rlcUeCfg);
      }
      else
      {
         DU_LOG("\nRLC: Failed to allocate memory ");
         ret = RFAILED;
      }
      RLC_FREE_SHRABL_BUF(pst->region, pst->pool, ueCfg, sizeof(RlcUeCfg));
      return ret;
}

/*******************************************************************
 *
 * @brief Process the DL RRC Message from DU APP
 *
 * @details
 *
 *    Function : RlcProcDlRrcMsgTransfer
 *
 *    Functionality: Process the DL RRC Message from DU APP
 *
 * @params[in] Post structure
 *             DL RRC Message info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t RlcProcDlRrcMsgTransfer(Pst *pst, RlcDlRrcMsgInfo *dlRrcMsgInfo)
{
   uint16_t      copyLen;
   Buffer        *mBuf;
   KwuDatReqInfo *datReqInfo;

   RLC_SHRABL_STATIC_BUF_ALLOC(RLC_MEM_REGION_DL, RLC_POOL, datReqInfo, sizeof(KwuDatReqInfo));
   if(!datReqInfo)
   {
      DU_LOG("\nRLC : Memory allocation failed in RlcProcDlRrcMsgTransfer");
      return RFAILED;
   }

   datReqInfo->rlcId.rbType = dlRrcMsgInfo->rbType;
   datReqInfo->rlcId.rbId = dlRrcMsgInfo->rbId;
   datReqInfo->rlcId.ueId = dlRrcMsgInfo->ueIdx;
   datReqInfo->rlcId.cellId = dlRrcMsgInfo->cellId;
   datReqInfo->lcType = dlRrcMsgInfo->lcType;
   datReqInfo->sduId = ++(rlcCb[pst->dstInst]->dlSduId);

   /* Copy fixed buffer to message */
   if(ODU_GET_MSG_BUF(RLC_MEM_REGION_UL, RLC_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nRLC : Memory allocation failed at RlcMacProcUlData");
      RLC_SHRABL_STATIC_BUF_FREE(RLC_MEM_REGION_DL, RLC_POOL, datReqInfo, sizeof(KwuDatReqInfo));
      return RFAILED;
   }
   /* ODU_COPY_FIX_BUF_TO_MSG copies fixed buffer in reverse order. Hence reversing the
    * fixed buffer before copying in order to maintain the actual order*/
   reverseFixBuf(dlRrcMsgInfo->rrcMsg, dlRrcMsgInfo->msgLen);
   ODU_COPY_FIX_BUF_TO_MSG(dlRrcMsgInfo->rrcMsg, mBuf, 0, dlRrcMsgInfo->msgLen, \
	 (MsgLen *)&copyLen);

   RlcUiKwuDatReq(pst, datReqInfo, mBuf);

   /* Free memory allocated by du app */
   RLC_SHRABL_STATIC_BUF_FREE(RLC_MEM_REGION_DL, RLC_POOL, datReqInfo, sizeof(KwuDatReqInfo));
   RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, dlRrcMsgInfo->rrcMsg, dlRrcMsgInfo->msgLen);
   RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, dlRrcMsgInfo, sizeof(RlcDlRrcMsgInfo));
   return ROK;
}

/**********************************************************************
         End of file
**********************************************************************/
