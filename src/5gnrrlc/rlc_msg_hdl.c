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
#include "rlc_utils.h"
#include "rlc_upr_inf_api.h"
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
 *  @return ROK/RFAILED
 * 
 *****************************************************************/

uint8_t fillRlcUlUeCfgRsp(RlcUeCfgRsp *rlcCfgRsp, RlcCfgCfmInfo *rlcCRsp)
{
   uint8_t idx;
   uint8_t ret = ROK;
 
   rlcCfgRsp->cellId = rlcCRsp->cellId;
   rlcCfgRsp->ueIdx  = rlcCRsp->ueId;
   for(idx = 0; idx < rlcCRsp->numEnt; idx++)
   {
      if(rlcCRsp->entCfgCfm[idx].status.status == CKW_CFG_CFM_OK)
      {
         rlcCfgRsp->result = RLC_DU_APP_RSP_OK;
	 rlcCfgRsp->reason = rlcCRsp->entCfgCfm[idx].status.reason;
	 ret = ROK;
      }
      else
      {
         rlcCfgRsp->result = RLC_DU_APP_RSP_NOK;
	 rlcCfgRsp->reason = rlcCRsp->entCfgCfm[idx].status.reason;
	 ret = RFAILED;
	 break;
      }
   }
   return ret;
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
 * @brief fills LC Cfgs to be Added in RLC
 *
 * @details
 *
 *    Function : fillRlcLcCfgList
 *
 *    Functionality:
 *      fills LC Cfgs to be Added in RLC
 *
 * @params[in] 
 *             RlcBearerCfg pointer
 *             RlcEntCfgInfo pointer
 *             Config Type 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillRlcLcCfgList(RlcBearerCfg *duRlcUeCfg, RlcEntCfgInfo *rlcUeCfg, uint8_t cfgType)
{
   uint8_t lChRbIdx = 0;

   rlcUeCfg->rbId                  = duRlcUeCfg->rbId;
   rlcUeCfg->rbType                = duRlcUeCfg->rbType;   // SRB or DRB
   rlcUeCfg->lCh[lChRbIdx].lChId   = duRlcUeCfg->lcId;   
   rlcUeCfg->lCh[lChRbIdx].type    = duRlcUeCfg->lcType;
   fillEntModeAndDir(&rlcUeCfg->entMode, &rlcUeCfg->dir, duRlcUeCfg->rlcMode);
   rlcUeCfg->cfgType               = cfgType;
   switch(rlcUeCfg->entMode)
   {

      case CM_LTE_MODE_AM:
         {
            /* DL AM INFO */
            rlcUeCfg->m.amInfo.dl.snLen       = duRlcUeCfg->u.amCfg.dlAmCfg.snLenDl; 
            rlcUeCfg->m.amInfo.dl.pollRetxTmr = duRlcUeCfg->u.amCfg.dlAmCfg.pollRetxTmr;
            rlcUeCfg->m.amInfo.dl.pollPdu     = duRlcUeCfg->u.amCfg.dlAmCfg.pollPdu; 
            rlcUeCfg->m.amInfo.dl.pollByte    = duRlcUeCfg->u.amCfg.dlAmCfg.pollByte; 
            rlcUeCfg->m.amInfo.dl.maxRetx     = duRlcUeCfg->u.amCfg.dlAmCfg.maxRetxTh;

            /* UL AM INFO */
            lChRbIdx++;   //lChRbIdx = 1, indicates UL AM
            rlcUeCfg->lCh[lChRbIdx].lChId    = duRlcUeCfg->lcId;   
            rlcUeCfg->lCh[lChRbIdx].type     = duRlcUeCfg->lcType;
            rlcUeCfg->m.amInfo.ul.snLen      = duRlcUeCfg->u.amCfg.ulAmCfg.snLenUl; 
            rlcUeCfg->m.amInfo.ul.staProhTmr = duRlcUeCfg->u.amCfg.ulAmCfg.statProhTmr;
            rlcUeCfg->m.amInfo.ul.reOrdTmr   = duRlcUeCfg->u.amCfg.ulAmCfg.reAssemTmr;
            break;
         }
      case CM_LTE_MODE_UM:
         {
            /* UL UM CONFIG */
            rlcUeCfg->m.umInfo.ul.snLen    = duRlcUeCfg->u.umBiDirCfg.ulUmCfg.snLenUlUm; 
            rlcUeCfg->m.umInfo.ul.reOrdTmr = duRlcUeCfg->u.umBiDirCfg.ulUmCfg.reAssemTmr;

            /* DL UM CONFIG */
            rlcUeCfg->m.umInfo.dl.snLen = duRlcUeCfg->u.umBiDirCfg.dlUmCfg.snLenDlUm; 
            break;
         }
      default:
         break;
   }/* End of switch(entMode) */
   return ROK;
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
   uint8_t ret = ROK;
   uint8_t lcIdx;
   RlcCfgInfo *rlcUeCfg = NULLP;

   RlcCb *rlcUeCb = NULLP;
   rlcUeCb = RLC_GET_RLCCB(pst->dstInst);
   RLC_ALLOC(rlcUeCb, rlcUeCfg, sizeof(RlcCfgInfo));
   if(rlcUeCfg)
   {
      memset(rlcUeCfg, 0, sizeof(RlcCfgInfo));

      rlcUeCfg->ueId    = ueCfg->ueIdx;
      rlcUeCfg->cellId  = ueCfg->cellId;
      rlcUeCfg->numEnt  = ueCfg->numLcsToAdd;
      rlcUeCfg->transId = getTransId();

      if(ueCfg->numLcsToAdd > 0 )
      {
         for(lcIdx = 0; (lcIdx < rlcUeCfg->numEnt && lcIdx < ueCfg->numLcsToAdd); lcIdx++)
	 {
            ret  = fillRlcLcCfgList(&ueCfg->rlcBearerCfgToAdd[lcIdx], &rlcUeCfg->entCfg[lcIdx], CKW_CFG_ADD);
	    if(ret)
	    {
               DU_LOG("\nRLC: Failed at fillLcsToAdd ");
	    }
	 }
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
 * @brief Handles Ue Reconfig Request from DU APP
 *
 * @details
 *
 *    Function : RlcUlProcUeReconfigReq
 *
 *    Functionality:
 *      Handles Ue Reconfig Request from DU APP
 *
 * @params[in] Post structure pointer
 *             RlcUeCfg pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t RlcUlProcUeReconfigReq(Pst *pst, RlcUeCfg *ueCfg)
{
   uint8_t ret = ROK;
   uint8_t lcIdx = 0, lcModIdx, lcDelIdx;
   RlcCfgInfo *rlcUeCfg = NULLP;

   RlcCb *rlcUeCb = NULLP;
   rlcUeCb = RLC_GET_RLCCB(pst->dstInst);
   RLC_ALLOC(rlcUeCb, rlcUeCfg, sizeof(RlcCfgInfo));
   if(rlcUeCfg)
   {
      memset(rlcUeCfg, 0, sizeof(RlcCfgInfo));

      rlcUeCfg->ueId    = ueCfg->ueIdx;
      rlcUeCfg->cellId  = ueCfg->cellId;
      rlcUeCfg->numEnt  = (ueCfg->numLcsToAdd + ueCfg->numLcsToMod + ueCfg->numLcsToDel);
      rlcUeCfg->transId = getTransId();

      if(ueCfg->numLcsToAdd > 0 )
      {
         for(lcIdx = 0; (lcIdx < rlcUeCfg->numEnt && lcIdx < ueCfg->numLcsToAdd); lcIdx++)
         {
            ret  = fillRlcLcCfgList(&ueCfg->rlcBearerCfgToAdd[lcIdx], &rlcUeCfg->entCfg[lcIdx], CKW_CFG_ADD);
	    if(ret)
	    {
               DU_LOG("\nRLC: Failed while adding Lcs ");
	    }
	 }
      }
      if(ueCfg->numLcsToMod > 0)
      {
         for(lcModIdx = 0; (lcIdx < rlcUeCfg->numEnt && lcModIdx < ueCfg->numLcsToMod); lcIdx++,lcModIdx++)
	 {
            ret  = fillRlcLcCfgList(&ueCfg->rlcBearerCfgToMod[lcModIdx], &rlcUeCfg->entCfg[lcIdx], CKW_CFG_MODIFY);
	    if(ret)
	    {
               DU_LOG("\nRLC: Failed while modifying Lcs ");
	    }
	 }
      }
      if(ueCfg->numLcsToDel > 0)
      {
         for(lcDelIdx = 0; (lcIdx < rlcUeCfg->numEnt && lcDelIdx < ueCfg->numLcsToDel); lcIdx++,lcModIdx++)
	 {
            ret  = fillRlcLcCfgList(&ueCfg->rlcBearerCfgToDel[lcDelIdx], &rlcUeCfg->entCfg[lcIdx], CKW_CFG_DELETE);
	    if(ret)
	    {
               DU_LOG("\nRLC: Failed while deleting Lcs ");
	    }
	 }
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
* @brief filling the structure of rrc delivery msg info
*
* @details
*
*    Function : BuildAndSendRrcDeliveryReportToDu
*
*    Functionality: filling the structure of rrc delivery msg info
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t BuildAndSendRrcDeliveryReportToDu( RlcDlRrcMsgInfo *dlRrcMsgInfo )
{
    Pst             pst;
    RrcDeliveryReport *rrcDelivery;
    
    DU_LOG("\nRLC : Filling the RRC Delivery Report");
    RLC_ALLOC_SHRABL_BUF(RLC_MEM_REGION_UL, RLC_POOL, rrcDelivery, sizeof(RrcDeliveryReport));

    if(rrcDelivery)
    {
       rrcDelivery->cellId = dlRrcMsgInfo->cellId;
       rrcDelivery->ueIdx  = dlRrcMsgInfo->ueIdx;
       rrcDelivery->srbId  = dlRrcMsgInfo->lcId ;
       rrcDelivery->rrcDeliveryStatus.deliveryStatus    = PDCP_SN;
       rrcDelivery->rrcDeliveryStatus.triggeringMessage = PDCP_SN;

       /* Sending UL RRC Message transfeer to DU APP */
       memset(&pst, 0, sizeof(Pst));
       FILL_PST_RLC_TO_DUAPP(pst, SFndProcId(), RLC_UL_INST, EVENT_RRC_DELIVERY_MSG_TRANS_TO_DU);
       rlcSendRrcDeliveryReportToDu(&pst, rrcDelivery);
    }
    else
    {
       DU_LOG("\nRLC : Memory allocation failed");
    }

   return ROK;
}
/* ****************************************************************
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
   
   /* RRC Delivery report is only send when RRC Delivery status report is true in DL RRC Message */
   if(dlRrcMsgInfo->deliveryStaRpt)
   {
      BuildAndSendRrcDeliveryReportToDu(dlRrcMsgInfo);
   }

   /* Free memory allocated by du app */
   RLC_SHRABL_STATIC_BUF_FREE(RLC_MEM_REGION_DL, RLC_POOL, datReqInfo, sizeof(KwuDatReqInfo));
   RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, dlRrcMsgInfo->rrcMsg, dlRrcMsgInfo->msgLen);
   RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, dlRrcMsgInfo, sizeof(RlcDlRrcMsgInfo));
   return ROK;
}
/**********************************************************************
         End of file
**********************************************************************/
