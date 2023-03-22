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
#include "rlc_env.h"        /* RLC environment options */
#include "rlc_err.h"


/* header/extern include files (.x) */
#include "rgu.x"           
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */

#include "rlc_utils.h"            /* RLC defines */
#include "rlc_dl_ul_inf.h"
#include "rlc_dl.h"
#include "rlc_ul.h"
#include "rlc_mac_inf.h"
#include "du_app_rlc_inf.h"
#include "rlc_upr_inf_api.h"
#include "rlc_mgr.h"

/*******************************************************************
 *
 * @brief building and sending UE max retransmission info to DU 
 *
 * @details
 *
 *    Function : BuildAndSendRlcMaxRetransIndToDu 
 *
 *    Functionality:
 *      Building and sending UE  max retransmission information to DU 
 *
 * @params[in] uint8_t cellId, uint8_t ueId, CauseOfResult  status 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendRlcMaxRetransIndToDu(uint16_t cellId,uint8_t ueId, uint8_t lcId, uint8_t lcType)
{
   Pst pst;  
   RlcMaxRetransInfo *maxRetransInfo = NULLP;
   
   FILL_PST_RLC_TO_DUAPP(pst, RLC_UL_INST, EVENT_RLC_MAX_RETRANSMISSION);

   RLC_ALLOC_SHRABL_BUF(pst.region, pst.pool, maxRetransInfo, sizeof(RlcMaxRetransInfo));
   if(!maxRetransInfo)
   {
      DU_LOG("\nERROR  -->  RLC: sendRlcMaxRetransIndToDu(): Memory allocation failed ");
      return RFAILED;
   }
   else
   {
      maxRetransInfo->cellId = cellId;
      maxRetransInfo->ueId = ueId;
      maxRetransInfo->lcId = lcId;
      maxRetransInfo->lcType = lcType;
  
      if(rlcSendMaxRetransIndToDu(&pst, maxRetransInfo) == ROK)
      {
         DU_LOG("\nDEBUG  -->  RLC: UE [ %d] max retransmission information sent successfully",ueId);
      }
      else
      {
         DU_LOG("\nERROR  -->  RLC: SendRlcMaxRetransIndToDu(): fail to send UeId's[%d] maximum retransmission information", ueId);
         RLC_FREE_SHRABL_BUF(pst.region, pst.pool, maxRetransInfo, sizeof(RlcMaxRetransInfo));
         return RFAILED;
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills RLC UL UE Cfg Rsp from RlcCRsp 
 *
 * @details
 *
 *    Function : fillRlcUeCreateRsp
 *
 *    Functionality: 
 *     Fills RLC UL UE Cfg Rsp from RlcCRsp
 * 
 *  @params[in]  Pointer to RlcCfgCfm
 *               Pointer to RlcUeCreateRsp
 *
 *  @return ROK/RFAILED
 * 
 *****************************************************************/

uint8_t fillRlcUeCreateRsp(RlcUeCreateRsp *rlcCfgRsp, RlcCfgCfmInfo *rlcCRsp)
{
   uint8_t idx;
   uint8_t ret = ROK;
 
   rlcCfgRsp->cellId = rlcCRsp->cellId;
   rlcCfgRsp->ueId   = rlcCRsp->ueId;
   rlcCfgRsp->result = RLC_DU_APP_RSP_OK;
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
         *entMode   = RLC_MODE_AM;
         *direction = RLC_CFG_DIR_BOTH;
         break;
      case RLC_UM_BI_DIRECTIONAL:
         *entMode = RLC_MODE_UM;
         *direction = RLC_CFG_DIR_BOTH;
         break;
      case RLC_UM_UNI_DIRECTIONAL_UL:
         *entMode = RLC_MODE_UM;
         *direction = RLC_CFG_DIR_UL;
         break;
      case RLC_UM_UNI_DIRECTIONAL_DL:
         *entMode = RLC_MODE_UM;
         *direction = RLC_CFG_DIR_DL;
         break;
      default : 
         DU_LOG("\nERROR  -->  RLC: Rlc Mode invalid %d", rlcMode);
    break;
   }
}

/*******************************************************************
 *
 * @brief fills LC Cfgs to be Added in RLC
 *
 * @details
 *
 *    Function : fillLcCfg
 *
 *    Functionality:
 *      fills LC Cfgs to be Added in RLC
 *
 * @params[in] 
 *             RlcEntCfgInfo pointer
 *             RlcBearerCfg pointer
 *             Config Type 
 * @return ROK -  SUCCESS
 *         RFAILED - FAILURE
 *
 * ****************************************************************/
uint8_t fillLcCfg(RlcCb *gCb, RlcEntCfgInfo *rlcUeCfg, RlcBearerCfg *duRlcUeCfg)
{
   uint8_t lChRbIdx = 0;

   rlcUeCfg->rbId                  = duRlcUeCfg->rbId;
   rlcUeCfg->rbType                = duRlcUeCfg->rbType;   // SRB or DRB
   rlcUeCfg->lCh[lChRbIdx].lChId   = duRlcUeCfg->lcId;   
   rlcUeCfg->lCh[lChRbIdx].type    = duRlcUeCfg->lcType;
   if(duRlcUeCfg->snssai)
   {
      RLC_ALLOC(gCb, rlcUeCfg->snssai, sizeof(Snssai));
      if(rlcUeCfg->snssai == NULLP)
      {
         DU_LOG("\nERROR  --> RLC : fillLcCfg(): Failed to allocate memory for snssai");
         return RFAILED;
      }
      memcpy(rlcUeCfg->snssai, duRlcUeCfg->snssai, sizeof(Snssai));
   }
   fillEntModeAndDir(&rlcUeCfg->entMode, &rlcUeCfg->dir, duRlcUeCfg->rlcMode);
   switch(rlcUeCfg->entMode)
   {

      case RLC_MODE_AM:
         {
            /* DL AM INFO */
            rlcUeCfg->m.amInfo.dl.snLen       = duRlcUeCfg->u.amCfg->dlAmCfg.snLenDl; 
            rlcUeCfg->m.amInfo.dl.pollRetxTmr = duRlcUeCfg->u.amCfg->dlAmCfg.pollRetxTmr;
            rlcUeCfg->m.amInfo.dl.pollPdu     = duRlcUeCfg->u.amCfg->dlAmCfg.pollPdu; 
            rlcUeCfg->m.amInfo.dl.pollByte    = duRlcUeCfg->u.amCfg->dlAmCfg.pollByte; 
            rlcUeCfg->m.amInfo.dl.maxRetx     = duRlcUeCfg->u.amCfg->dlAmCfg.maxRetxTh;

            /* UL AM INFO */
            lChRbIdx++;   //lChRbIdx = 1, indicates UL AM
            rlcUeCfg->lCh[lChRbIdx].lChId    = duRlcUeCfg->lcId;   
            rlcUeCfg->lCh[lChRbIdx].type     = duRlcUeCfg->lcType;
            rlcUeCfg->m.amInfo.ul.snLen      = duRlcUeCfg->u.amCfg->ulAmCfg.snLenUl; 
            rlcUeCfg->m.amInfo.ul.staProhTmr = duRlcUeCfg->u.amCfg->ulAmCfg.statProhTmr;
            rlcUeCfg->m.amInfo.ul.reAsmblTmr   = duRlcUeCfg->u.amCfg->ulAmCfg.reAssemTmr;
            break;
         }
      case RLC_MODE_UM:
         {
            /* UL UM CONFIG */
            rlcUeCfg->m.umInfo.ul.snLen      = duRlcUeCfg->u.umBiDirCfg->ulUmCfg.snLenUlUm; 
            rlcUeCfg->m.umInfo.ul.reAsmblTmr = duRlcUeCfg->u.umBiDirCfg->ulUmCfg.reAssemTmr;

            /* DL UM CONFIG */
            rlcUeCfg->m.umInfo.dl.snLen = duRlcUeCfg->u.umBiDirCfg->dlUmCfg.snLenDlUm; 
            break;
         }
      default:
         break;
   }/* End of switch(entMode) */
   return ROK;
}

/*******************************************************************
 *
 * @brief fills LC Cfgs to be Added in RLC
 *
 * @details
 *
 *    Function : fillRlcCfg
 *
 *    Functionality:
 *      fills LC Cfgs to be Add in RLC
 *
 * @params[in] 
 *             RlcEntCfgInfo pointer
 *             RlcBearerCfg pointer
 * @return ROK - Success
 *          RFAILED - Failure
 *
 ******************************************************************/

uint8_t fillRlcCfg(RlcCb *gCb, RlcCfgInfo *rlcUeCfg, RlcUeCreate *ueCfg)
{
   uint8_t lcIdx;
   
   rlcUeCfg->ueId    = ueCfg->ueId;
   rlcUeCfg->cellId  = ueCfg->cellId;
   rlcUeCfg->numEnt  = ueCfg->numLcsToAdd;
   rlcUeCfg->transId = getTransId();
   for(lcIdx = 0; lcIdx < rlcUeCfg->numEnt; lcIdx++)
   {
      if(fillLcCfg(gCb, &rlcUeCfg->entCfg[lcIdx], &ueCfg->rlcLcCfgAdd[lcIdx]) != ROK)
      {
          DU_LOG("\nERROR  --> RLC : fillRlcCfg(): Failed to fill LC configuration");
          return RFAILED;
      }
      rlcUeCfg->entCfg[lcIdx].cfgType = CONFIG_ADD; 
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief updates LC Cfgs to be Added/Mod/Rel in RLC
 *
 * @details
 *
 *    Function : updateRlcCfg
 *
 *    Functionality:
 *      updates LC Cfgs to be Add/Mod/Del in RLC
 *
 * @params[in] 
 *             RlcEntCfgInfo pointer
 *             RlcBearerCfg pointer
 * @return ROK - Success
 *          RFAILED - Failure
 *
 ******************************************************************/

uint8_t updateRlcCfg(RlcCb *gCb, RlcCfgInfo *rlcUeCfg, RlcUeRecfg *ueRecfg)
{
   uint8_t lcIdx = 0;
   
   rlcUeCfg->ueId    = ueRecfg->ueId;
   rlcUeCfg->cellId  = ueRecfg->cellId;
   rlcUeCfg->transId = getTransId();
   
   rlcUeCfg->numEnt = 0;
   for(lcIdx = 0; lcIdx < ueRecfg->numLcsToAdd; lcIdx++)
   {
      if(fillLcCfg(gCb, &rlcUeCfg->entCfg[rlcUeCfg->numEnt], &ueRecfg->rlcLcCfgAdd[lcIdx]) != ROK)
      {
          DU_LOG("\nERROR  --> RLC : fillRlcCfg(): Failed to fill LC configuration");
          return RFAILED;
      }
      rlcUeCfg->entCfg[rlcUeCfg->numEnt].cfgType = CONFIG_ADD;
      rlcUeCfg->numEnt++;
   }
   for(lcIdx = 0; lcIdx < ueRecfg->numLcsToMod; lcIdx++)
   {
      if(fillLcCfg(gCb, &rlcUeCfg->entCfg[rlcUeCfg->numEnt], &ueRecfg->rlcLcCfgMod[lcIdx]) != ROK)
      {
          DU_LOG("\nERROR  --> RLC : fillRlcCfg(): Failed to fill LC configuration");
          return RFAILED;
      }
      rlcUeCfg->entCfg[rlcUeCfg->numEnt].cfgType = CONFIG_MOD;
      rlcUeCfg->numEnt++;
   }
   for(lcIdx = 0; lcIdx < ueRecfg->numLcsToRel; lcIdx++)
   {
      if(fillLcCfg(gCb, &rlcUeCfg->entCfg[rlcUeCfg->numEnt], &ueRecfg->rlcLcCfgRel[lcIdx]) != ROK)
      {
          DU_LOG("\nERROR  --> RLC : fillRlcCfg(): Failed to fill LC configuration");
          return RFAILED;
      }
      rlcUeCfg->entCfg[rlcUeCfg->numEnt].cfgType = CONFIG_DEL;
      rlcUeCfg->numEnt++;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill RlcCfgCfmInfo structure for sending failure response to DU
 *
 * @details
 *
 *    Function : fillRlcCfgFailureRsp
 *
 *    Functionality:
 *      Fill RlcCfgCfmInfo structure for sending failure response to DU
 *
 * @params[in] RlcCfgCfmInfo *cfgRsp, RlcUeCreate *ueCfg
 *             
 * @return void 
 *
 * ****************************************************************/
void fillRlcCfgFailureRsp(RlcCfgCfmInfo *cfgRsp, RlcUeCreate *ueCfg)
{
   uint8_t cfgIdx =0;

   cfgRsp->ueId = ueCfg->ueId;
   cfgRsp->cellId = ueCfg->cellId;
   cfgRsp->numEnt = ueCfg->numLcsToAdd;
   for(cfgIdx =0; cfgIdx<ueCfg->numLcsToAdd; cfgIdx++)
   {
      cfgRsp->entCfgCfm[cfgIdx].rbId = ueCfg->rlcLcCfgAdd[cfgIdx].rbId;
      cfgRsp->entCfgCfm[cfgIdx].rbType = ueCfg->rlcLcCfgAdd[cfgIdx].rbType;
      cfgRsp->entCfgCfm[cfgIdx].status.status = RLC_DU_APP_RSP_NOK;
      cfgRsp->entCfgCfm[cfgIdx].status.reason = CKW_CFG_REAS_NONE;
   }
}

/*******************************************************************
 *
 * @brief Fill RlcCfgCfmInfo structure for sending failure response to DU
 *
 * @details
 *
 *    Function : fillRlcRecfgFailureRsp
 *
 *    Functionality:
 *      Fill RlcCfgCfmInfo structure for sending failure response to DU
 *
 * @params[in] RlcCfgCfmInfo *cfgRsp, RlcUeCreate *ueCfg
 *             
 * @return void 
 *
 * ****************************************************************/
void fillRlcRecfgFailureRsp(RlcCfgCfmInfo *cfgRsp, RlcUeRecfg *ueRecfg)
{
   uint8_t cfgIdx =0;

   cfgRsp->ueId = ueRecfg->ueId;
   cfgRsp->cellId = ueRecfg->cellId;
   cfgRsp->numEnt = 0;
   for(cfgIdx =0; cfgIdx<ueRecfg->numLcsToAdd; cfgIdx++)
   {
      cfgRsp->entCfgCfm[cfgRsp->numEnt].rbId = ueRecfg->rlcLcCfgAdd[cfgIdx].rbId;
      cfgRsp->entCfgCfm[cfgRsp->numEnt].rbType = ueRecfg->rlcLcCfgAdd[cfgIdx].rbType;
      cfgRsp->entCfgCfm[cfgRsp->numEnt].status.status = RLC_DU_APP_RSP_NOK;
      cfgRsp->entCfgCfm[cfgRsp->numEnt].status.reason = CKW_CFG_REAS_NONE;
      cfgRsp->numEnt++;
   }
   for(cfgIdx =0; cfgIdx<ueRecfg->numLcsToMod; cfgIdx++)
   {
      cfgRsp->entCfgCfm[cfgRsp->numEnt].rbId = ueRecfg->rlcLcCfgMod[cfgIdx].rbId;
      cfgRsp->entCfgCfm[cfgRsp->numEnt].rbType = ueRecfg->rlcLcCfgMod[cfgIdx].rbType;
      cfgRsp->entCfgCfm[cfgRsp->numEnt].status.status = RLC_DU_APP_RSP_NOK;
      cfgRsp->entCfgCfm[cfgRsp->numEnt].status.reason = CKW_CFG_REAS_NONE;
      cfgRsp->numEnt++;
   }
   for(cfgIdx =0; cfgIdx<ueRecfg->numLcsToRel; cfgIdx++)
   {
      cfgRsp->entCfgCfm[cfgRsp->numEnt].rbId = ueRecfg->rlcLcCfgRel[cfgIdx].rbId;
      cfgRsp->entCfgCfm[cfgRsp->numEnt].rbType = ueRecfg->rlcLcCfgRel[cfgIdx].rbType;
      cfgRsp->entCfgCfm[cfgRsp->numEnt].status.status = RLC_DU_APP_RSP_NOK;
      cfgRsp->entCfgCfm[cfgRsp->numEnt].status.reason = CKW_CFG_REAS_NONE;
      cfgRsp->numEnt++;
   }
}

/*******************************************************************
 *
 * @brief Handles Ue Create Request from DU APP
 *
 * @details
 *
 *    Function : RlcProcUeCreateReq
 *
 *    Functionality:
 *      Handles Ue create Request from DU APP
 *
 * @params[in] Post structure pointer
 *             RlcUeCreate pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t RlcProcUeCreateReq(Pst *pst, RlcUeCreate *ueCfg)
{
   uint8_t ret = ROK;
   RlcCb *gCb;
   RlcCfgInfo *rlcUeCfg = NULLP;
   RlcCfgCfmInfo cfgRsp;
   Pst rspPst;

   gCb = RLC_GET_RLCCB(pst->dstInst);
   RLC_ALLOC(gCb, rlcUeCfg, sizeof(RlcCfgInfo));
   if(rlcUeCfg == NULLP)
   {
      DU_LOG("\nERROR  -->  RLC: Failed to allocate memory at RlcProcUeCreateReq()");
      ret = RFAILED;
   }
   else
   {
      memset(rlcUeCfg, 0, sizeof(RlcCfgInfo));
      ret = fillRlcCfg(gCb, rlcUeCfg, ueCfg); 
      if(ret != ROK)
      {
         DU_LOG("\nERROR  -->  RLC: Failed to fill configuration at RlcProcUeCreateReq()");
         FILL_PST_RLC_TO_DUAPP(rspPst, RLC_UL_INST, EVENT_RLC_UE_CREATE_RSP);
         fillRlcCfgFailureRsp(&cfgRsp, ueCfg);
         SendRlcUeCreateRspToDu(&rspPst, &cfgRsp);

      }
      else
      {
         ret = RlcProcCfgReq(pst, rlcUeCfg);
         if(ret != ROK)
            DU_LOG("\nERROR  -->  RLC: Failed to configure Add/Mod/Del entities at RlcProcUeCreateReq()");
      }
   }
   RLC_FREE_SHRABL_BUF(pst->region, pst->pool, ueCfg, sizeof(RlcUeCreate));
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

    DU_LOG("\nINFO  -->  RLC : Filling RRC Delivery Report");
    RLC_ALLOC_SHRABL_BUF(RLC_MEM_REGION_UL, RLC_POOL, rrcDelivery, sizeof(RrcDeliveryReport));

    if(rrcDelivery)
    {
       rrcDelivery->cellId = dlRrcMsgInfo->cellId;
       rrcDelivery->ueId  = dlRrcMsgInfo->ueId;
       rrcDelivery->srbId  = dlRrcMsgInfo->lcId ;
       rrcDelivery->rrcDeliveryStatus.deliveryStatus    = PDCP_SN;
       rrcDelivery->rrcDeliveryStatus.triggeringMessage = PDCP_SN;

       /* Sending UL RRC Message transfeer to DU APP */
       memset(&pst, 0, sizeof(Pst));
       FILL_PST_RLC_TO_DUAPP(pst, RLC_UL_INST, EVENT_RRC_DELIVERY_MSG_TRANS_TO_DU);
       rlcSendRrcDeliveryReportToDu(&pst, rrcDelivery);
    }
    else
    {
       DU_LOG("\nERROR  -->  RLC : Memory allocation failed");
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
   Buffer        *mBuf;
   RlcDatReqInfo *datReqInfo;

   RLC_SHRABL_STATIC_BUF_ALLOC(RLC_MEM_REGION_DL, RLC_POOL, datReqInfo, sizeof(RlcDatReqInfo));
   if(!datReqInfo)
   {
      DU_LOG("\nERROR  -->  RLC : Memory allocation failed in RlcProcDlRrcMsgTransfer");
      RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, dlRrcMsgInfo->rrcMsg, dlRrcMsgInfo->msgLen);
      RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, dlRrcMsgInfo, sizeof(RlcDlRrcMsgInfo));
      return RFAILED;
   }

   datReqInfo->rlcId.rbType = RB_TYPE_SRB;
   datReqInfo->rlcId.rbId = dlRrcMsgInfo->lcId;
   datReqInfo->rlcId.ueId = dlRrcMsgInfo->ueId;
   datReqInfo->rlcId.cellId = dlRrcMsgInfo->cellId;
   datReqInfo->lcType = LCH_DCCH;
   datReqInfo->sduId = ++(rlcCb[pst->dstInst]->dlSduId);

   /* Copy fixed buffer to message */
   if(ODU_GET_MSG_BUF(RLC_MEM_REGION_UL, RLC_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  -->  RLC : Memory allocation failed at RlcMacProcUlData");
      RLC_SHRABL_STATIC_BUF_FREE(RLC_MEM_REGION_DL, RLC_POOL, datReqInfo, sizeof(RlcDatReqInfo));
      RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, dlRrcMsgInfo->rrcMsg, dlRrcMsgInfo->msgLen);
      RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, dlRrcMsgInfo, sizeof(RlcDlRrcMsgInfo));
      return RFAILED;
   }
   oduCpyFixBufToMsg(dlRrcMsgInfo->rrcMsg, mBuf, dlRrcMsgInfo->msgLen);

   if(rlcProcDlData(pst, datReqInfo, mBuf) != ROK)
   {
      return RFAILED;
   }

   /* RRC Delivery report is only send when RRC Delivery status report is true in DL RRC Message */
   if(dlRrcMsgInfo->deliveryStaRpt)
   {
      BuildAndSendRrcDeliveryReportToDu(dlRrcMsgInfo);
   }

   /* Free memory allocated by du app */
   RLC_SHRABL_STATIC_BUF_FREE(RLC_MEM_REGION_DL, RLC_POOL, datReqInfo, sizeof(RlcDatReqInfo));
   RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, dlRrcMsgInfo->rrcMsg, dlRrcMsgInfo->msgLen);
   RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, dlRrcMsgInfo, sizeof(RlcDlRrcMsgInfo));
   return ROK;
}

/*******************************************************************
 *
 * @brief Process UL data from UE
 *
 * @details
 *
 *    Function : RlcProcUlData
 *
 *    Functionality:
 *       This function receives the PDU from MAC.
 *       seggregates common and dedicated logical channel
 *       PDU and call respective handler.
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t RlcProcUlData(Pst *pst, RlcUlData *ulData)
{
   uint8_t         ret = ROK;
   uint8_t         idx, pduIdx;
   uint8_t         lcId;                    /* Logical Channel */
   uint8_t         numDLch = 0;             /* Number of dedicated logical channel */
   bool            dLchPduPres;             /* PDU received on dedicated logical channel */
   RguLchDatInd    dLchData[MAX_NUM_LC];    /* PDU info on dedicated logical channel */
   RguDDatIndInfo  *dLchUlDat;              /* UL data on dedicated logical channel */

   /* Initializing dedicated logical channel Database */
   DU_LOG("\nDEBUG  -->  RLC: Received UL Data request from MAC");
   for(idx = 0; idx < MAX_NUM_LC; idx++)
   {
      dLchData[idx].lcId = idx;
      dLchData[idx].pdu.numPdu = 0;
   }
   dLchPduPres = FALSE;

   /* Seggregate PDUs received on common and dedicated channels
    * and call common channel's handler */
   for(idx = 0; idx< ulData->numPdu; idx++)
   {
      if(!dLchPduPres)
      {
         RLC_SHRABL_STATIC_BUF_ALLOC(RLC_MEM_REGION_UL, RLC_POOL, dLchUlDat, \
               sizeof(RguDDatIndInfo));
         if(!dLchUlDat)
         {
            DU_LOG("\nERROR  -->  RLC : Memory allocation failed at RlcMacProcUlData");
            ret = RFAILED;
            break;
         }
         dLchPduPres = TRUE;
      }

      /* Copy fixed buffer to message */
      lcId = ulData->pduInfo[idx].lcId;
      if(ODU_GET_MSG_BUF(RLC_MEM_REGION_UL, RLC_POOL, \
               &dLchData[lcId].pdu.mBuf[dLchData[lcId].pdu.numPdu]) != ROK)
      {
         DU_LOG("\nERROR  -->  RLC : Memory allocation failed at RlcMacProcUlData");
         for(pduIdx=0; pduIdx < dLchData[lcId].pdu.numPdu; pduIdx++)
         {
            ODU_PUT_MSG_BUF(dLchData[lcId].pdu.mBuf[dLchData[lcId].pdu.numPdu]);
         }
         RLC_SHRABL_STATIC_BUF_FREE(RLC_MEM_REGION_UL, RLC_POOL, dLchUlDat, \
               sizeof(RguDDatIndInfo));
         ret = RFAILED;
         break;
      }
      oduCpyFixBufToMsg(ulData->pduInfo[idx].pduBuf, \
            dLchData[lcId].pdu.mBuf[dLchData[lcId].pdu.numPdu],\
            ulData->pduInfo[idx].pduLen);

      dLchData[lcId].pdu.numPdu++;
   }

   /* If any PDU received on dedicated logical channel, copy into RguDDatIndInfo
    * and call its handler */
   if(ret == ROK)
   {
      if(dLchPduPres)
      {
         dLchUlDat->cellId = ulData->cellId;
         GET_UE_ID(ulData->rnti, dLchUlDat->rnti);

         for(idx = 0; idx < MAX_NUM_LC; idx++)
         {
            if(dLchData[idx].pdu.numPdu)
            {
               memcpy(&dLchUlDat->lchData[numDLch], &dLchData[idx], sizeof(RguLchDatInd));
               numDLch++;
            }
         }
         dLchUlDat->numLch = numDLch;
         rlcProcDedLcUlData(pst, 0, dLchUlDat);
      }
   }

   for(pduIdx = 0; pduIdx < ulData->numPdu; pduIdx++)
   {
      RLC_FREE_SHRABL_BUF(pst->region, pst->pool, ulData->pduInfo[pduIdx].pduBuf, \
            ulData->pduInfo[pduIdx].pduLen);
   }
   RLC_FREE_SHRABL_BUF(pst->region, pst->pool, ulData, sizeof(RlcUlData));
   return ROK;

}/* End of RlcProcUlData */

/*******************************************************************
 *
 * @brief Handler for extracting common and dedicated channel
 *      Scheduling result report.
 *
 * @details
 *
 *    Function : RlcProcSchedResultRpt
 *
 *    Functionality:
 *     Handler for extracting common and dedicated channel
 *      Scheduling result report
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t RlcProcSchedResultRpt(Pst *pst, RlcSchedResultRpt *schRep)
{
   uint8_t ret = ROK;
   uint8_t idx;                     /* Iterator */
   uint8_t nmbDLch = 0;                 /* Number of dedicated logical channles */
   RguDStaIndInfo   *dLchSchInfo;  /* Dedicated logical channel scheduling result */

   DU_LOG("\nDEBUG  -->  RLC : Received scheduling report from MAC");
   for(idx=0; idx < schRep->numLc; idx++)
   {
      /* Fill status info structure if at least one channel's scheduling report is received */
      if(nmbDLch == 0)
      {
         RLC_SHRABL_STATIC_BUF_ALLOC(RLC_MEM_REGION_DL, RLC_POOL, dLchSchInfo, \
               sizeof(RguDStaIndInfo));
         if(!dLchSchInfo)
         {
            DU_LOG("\nERROR  -->  RLC: RlcProcSchedResultRpt: Memory allocation failed for dLchSchInfo");
            ret = RFAILED;
            break;
         }

         dLchSchInfo->cellId = schRep->cellId;
         dLchSchInfo->nmbOfUeGrantPerTti = 1;
         /* MAC sends Scheduling report for one UE at a time. Hence filling
            only the 0th index of staInd */
         dLchSchInfo->staInd[0].rnti = schRep->rnti;

         /* Storing sfn/slot into a single 32-bit variable to be used later*/
         dLchSchInfo->staInd[0].transId = schRep->slotInfo.sfn;
         dLchSchInfo->staInd[0].transId = \
                                          (dLchSchInfo->staInd[0].transId << 16) | schRep->slotInfo.slot; 
         dLchSchInfo->staInd[0].nmbOfTbs = 1;
         dLchSchInfo->staInd[0].fillCtrlPdu = true; 
      }

      /* Fill logical channel scheduling info */
      dLchSchInfo->staInd[0].staIndTb[0].lchStaInd[nmbDLch].lcId = \
                                                                   schRep->lcSch[idx].lcId;
      dLchSchInfo->staInd[0].staIndTb[0].lchStaInd[nmbDLch].totBufSize = \
                                                                         schRep->lcSch[idx].bufSize;
      nmbDLch++;
   }

   /* Calling handler for all dedicated channels scheduling*/
   if(ret == ROK)
   {
      if(nmbDLch)
      {
         dLchSchInfo->staInd[0].staIndTb[0].nmbLch = nmbDLch;
         rlcProcDedLcSchedRpt(pst, 0, dLchSchInfo);
      }
   }

   RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, schRep, sizeof(RlcSchedResultRpt));
   return ret;
}


/*******************************************************************
 *
 * @brief Handles Ue Reconfig Request from DU APP
 *
 * @details
 *
 *    Function : RlcProcUeReconfigReq
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
uint8_t RlcProcUeReconfigReq(Pst *pst, RlcUeRecfg *ueRecfg)
{
   uint8_t ret = ROK;
   RlcCfgInfo *rlcUeCfg = NULLP; //Seed code Rlc cfg struct
   RlcCb *rlcUeCb = NULLP;
   RlcCfgCfmInfo cfgRsp; 
   Pst rspPst;
   
   DU_LOG("\nDEBUG  -->  RLC: UE reconfig request received. CellID[%d] UEID[%d]",ueRecfg->cellId, ueRecfg->ueId);

   rlcUeCb = RLC_GET_RLCCB(pst->dstInst);
   RLC_ALLOC(rlcUeCb, rlcUeCfg, sizeof(RlcCfgInfo));
   if(rlcUeCfg == NULLP)
   {
      DU_LOG("\nERROR  -->  RLC: Failed to allocate memory at RlcProcUeReconfigReq()");
      ret = RFAILED;
   }
   else
   {
      memset(rlcUeCfg, 0, sizeof(RlcCfgInfo));
      ret = updateRlcCfg(rlcUeCb, rlcUeCfg, ueRecfg);
      if(ret != ROK)
      {
         DU_LOG("\nERROR  -->  RLC: Failed to fill configuration at RlcProcUeReconfigReq()");
         FILL_PST_RLC_TO_DUAPP(rspPst, RLC_UL_INST, EVENT_RLC_UE_RECONFIG_RSP);
         fillRlcRecfgFailureRsp(&cfgRsp, ueRecfg);
         SendRlcUeReconfigRspToDu(&rspPst, &cfgRsp);
      }
      else
      {
         ret = RlcProcCfgReq(pst, rlcUeCfg);
         if(ret != ROK)
            DU_LOG("\nERROR  -->  RLC: Failed to configure Add/Mod/Del entities at RlcProcUeReconfigReq()");
      }
   }
   
   RLC_FREE_SHRABL_BUF(pst->region, pst->pool, ueRecfg, sizeof(RlcUeRecfg));
   return ret;
}

/* ****************************************************************
 *
 * @brief Process the DL Data transfer from DU APP
 *
 * @details
 *
 *    Function : RlcProcDlUserDataTransfer
 *
 *    Functionality: Process the DL transfer from DU APP
 *
 * @params[in] Post structure
 *             DL RRC Message info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t RlcProcDlUserDataTransfer(Pst *pst, RlcDlUserDataInfo *dlDataMsgInfo)
{
   Buffer        *mBuf = NULLP;
   RlcDatReqInfo *datReqInfo = NULLP;

   if(dlDataMsgInfo->dlMsg == NULLP)
   {
      DU_LOG("\nERROR  -->  RLC_DL : Received DL message is NULLP in RlcProcDlUserDataTransfer()");
      return RFAILED;
   }
   RLC_SHRABL_STATIC_BUF_ALLOC(RLC_MEM_REGION_DL, RLC_POOL, datReqInfo, sizeof(RlcDatReqInfo));
   if(!datReqInfo)
   {
      DU_LOG("\nERROR  -->  RLC_DL : Memory allocation failed for DatReq in RlcProcDlUserDataTransfer()");
      ODU_PUT_MSG_BUF(dlDataMsgInfo->dlMsg);
      RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, dlDataMsgInfo, sizeof(RlcDlUserDataInfo));
      return RFAILED;
   }

   datReqInfo->rlcId.rbType = RB_TYPE_DRB;
   datReqInfo->rlcId.rbId   = dlDataMsgInfo->rbId;
   datReqInfo->rlcId.ueId   = dlDataMsgInfo->ueId;
   datReqInfo->rlcId.cellId = dlDataMsgInfo->cellId;
   datReqInfo->lcType       = LCH_DTCH;
   datReqInfo->sduId        = ++(rlcCb[pst->dstInst]->dlSduId);
   mBuf = dlDataMsgInfo->dlMsg;
   if(rlcProcDlData(pst, datReqInfo, mBuf) != ROK)
   {
      return RFAILED;
   }
    
   /* Free memory allocated by du app */
   RLC_SHRABL_STATIC_BUF_FREE(RLC_MEM_REGION_DL, RLC_POOL, datReqInfo, sizeof(RlcDatReqInfo));
   RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, dlDataMsgInfo, sizeof(RlcDlUserDataInfo));
   return ROK;
}

/*******************************************************************
 *
 * @brief sending UE delete response to DU 
 *
 * @details
 *
 *    Function : sendRlcUeDeleteRspToDu 
 *
 *    Functionality:
 *      sending UE delete response to DU 
 *
 * @params[in] uint8_t cellId, uint8_t ueId, CauseOfResult  status 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sendRlcUeDeleteRspToDu(uint16_t cellId,uint8_t ueId, CauseOfResult  status)
{
   Pst pst;  
   RlcUeDeleteRsp *ueDeleteRsp = NULLP;
   
   FILL_PST_RLC_TO_DUAPP(pst, RLC_UL_INST, EVENT_RLC_UE_DELETE_RSP);

   RLC_ALLOC_SHRABL_BUF(pst.region, pst.pool, ueDeleteRsp, sizeof(RlcUeDeleteRsp));
   if(!ueDeleteRsp)
   {
      DU_LOG("\nERROR  -->  RLC: sendRlcUeDeleteRspToDu(): Memory allocation failed ");
      return RFAILED;
   }
   else
   {
      ueDeleteRsp->cellId = cellId;
      ueDeleteRsp->ueId = ueId;
      ueDeleteRsp->status = status;
  
      if(rlcSendUeDeleteRspToDu(&pst, ueDeleteRsp) == ROK)
      {
         DU_LOG("\nDEBUG  -->  RLC: UE Delete response send successfully");
      }
      else
      {
         DU_LOG("\nERROR  -->  RLC: SendRlcUeDeleteRspToDu():Failed to send UE Delete response to DU");
         RLC_FREE_SHRABL_BUF(pst.region, pst.pool, ueDeleteRsp, sizeof(RlcUeDeleteRsp));
         return RFAILED;
      }
   }
   return ROK;
}

/*******************************************************************
*
* @brief Handles Ue delete Request from DU APP
*
* @details
*
*    Function : RlcProcUeDeleteReq
*
*    Functionality:
*      Handles Ue delete Request from DU APP
*
* @params[in] Post structure pointer
*             RlcUeDelete pointer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t RlcProcUeDeleteReq(Pst *pst, RlcUeDelete *ueDelete)
{
   uint8_t ret = ROK;
   RlcCb *gRlcCb = NULLP;
   RlcUlUeCb *ueCb = NULLP;
   CauseOfResult  status =SUCCESSFUL;

   DU_LOG("\nDEBUG  -->  RLC: UE Delete request received. CellID[%d] UEID[%d]",ueDelete->cellId, ueDelete->ueId);

   if(ueDelete != NULLP)
   {
      gRlcCb = RLC_GET_RLCCB(pst->dstInst);
      rlcDbmFetchUlUeCb(gRlcCb,ueDelete->ueId, ueDelete->cellId, &ueCb);
      if(ueCb != NULLP)
      {
         if(ueDelete->cellId == ueCb->cellId)
         {
            memcpy(&ueCb->ueDeleteInfo.pst, pst, sizeof(Pst));
            if((rlcChkTmr(gRlcCb, (PTR)ueCb, EVENT_RLC_UE_DELETE_TMR)) == FALSE)
            {
               rlcStartTmr(gRlcCb,(PTR)ueCb, EVENT_RLC_UE_DELETE_TMR);
            }
         }
         else
         {
            status = CELLID_INVALID;
         }
      }
      else
      {
         status = UEID_INVALID;
      }

      if(status != SUCCESSFUL)
      {
         ret = sendRlcUeDeleteRspToDu(ueDelete->cellId, ueDelete->ueId, status);
         if(ret != ROK)
         {
            DU_LOG("\nERROR  -->  RLC: RlcProcUeDeleteReq():Failed to send UE Delete response to DU");
         }
      }
      RLC_FREE_SHRABL_BUF(pst->region, pst->pool, ueDelete, sizeof(RlcUeDelete));
   }
   else
   {
      DU_LOG("\nERROR  -->  RLC: RlcProcUeDeleteReq(): Recieved NULL pointer UE Delete ");
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
*
* @brief Send the Slice Metrics to  DU APP
*
* @details
*
*    Function : sendSlicePmToDu
*
*    Functionality:
*      Handles the sending of Slice Metrics to  DU APP
*
* @params[in] Post structure pointer
*             SlicePmList *sliceStats pointer
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t sendSlicePmToDu(SlicePmList *sliceStats)
{
   Pst pst;  
   
   FILL_PST_RLC_TO_DUAPP(pst, RLC_UL_INST, EVENT_RLC_SLICE_PM_TO_DU);

   if(!sliceStats)
   {
      DU_LOG("\nERROR  -->  RLC: sendSlicePmToDu(): Memory allocation failed ");
      return RFAILED;
   }
   else
   {
      if(rlcSendSlicePmToDu(&pst, sliceStats) == ROK)
      {
         DU_LOG("\nDEBUG  -->  RLC: Slice PM send successfully");
      }
      else
      {
         DU_LOG("\nERROR  -->  RLC: sendSlicePmToDu():Failed to send Slice PM to DU");
         RLC_FREE_SHRABL_BUF(pst.region, pst.pool, sliceStats, sizeof(SlicePmList));
         return RFAILED;
      }
   }
   return ROK;
}

/**
 * @brief 
 *    Handler for searching the Slice Entry in Slice Metrics structure
 *
 * @details
 *    This func finds the slice entry in the SliceMetric record structure and
 *    return the index of the slice sot hat Tput entries can be done
 *
 * @param[in] snssaiVal : Snssai Val to be searched
 *            *snssaiIdx : O/P : Index of the Slice in Slice Metrics record
 *            sliceStats : Pointer of Slice metrics record list
 *
 * @return bool: True: If slice found in the record
 *               False: If Slice not found; thus parent function will create the
 *               recpord of this snssai
 *   
 */
bool rlcFindSliceEntry(SliceIdentifier snssaiVal, uint8_t *snssaiIdx, SlicePmList *sliceStats)
{
   uint8_t cntSlices = sliceStats->numSlice;

   for(*snssaiIdx = 0;(*snssaiIdx) < cntSlices; (*snssaiIdx)++)
   {
      if((snssaiVal.sst == sliceStats->sliceRecord[*snssaiIdx].networkSliceIdentifier.sst)&&
            (snssaiVal.sd == sliceStats->sliceRecord[*snssaiIdx].networkSliceIdentifier.sd))
      {
         return TRUE;
      }
   }
   return FALSE;
}


/*******************************************************************
*
* @brief Builds the Slice Performance Metrics structure to be sent to DU
*
* @details
*
*    Function : BuildSliceReportToDu
*
*    Functionality:
*      Builds the Slice Performance Metrics structure to be sent to DU
*
* @params[in] uint8_t snssaiCnt
*             
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t BuildSliceReportToDu(uint8_t snssaiCnt)
{
   CmLList  *node = NULLP;
   RlcTptPerSnssai *snssaiNode = NULLP;
   Direction dir = DIR_UL;
   SlicePmList *sliceStats = NULLP;   /*Slice metric */
   SliceIdentifier snssaiVal ;
   uint8_t snssaiIdx = 0;

   if(snssaiCnt == 0)
   {
      DU_LOG("\nERROR  -->  RLC: No SNSSAI to send the SLice PM");
      return RFAILED;
   }

   RLC_ALLOC_SHRABL_BUF(RLC_MEM_REGION_UL, RLC_POOL, sliceStats, sizeof(SlicePmList));
   if(sliceStats == NULLP)
   {
      DU_LOG("\nERROR  -->  RLC: Memory Allocation Failed");
      return RFAILED;
   }
   RLC_ALLOC_SHRABL_BUF(RLC_MEM_REGION_UL, RLC_POOL, sliceStats->sliceRecord, snssaiCnt * (sizeof(SlicePm)));

   if(sliceStats->sliceRecord == NULLP)
   {
      DU_LOG("\nERROR  -->  RLC: Memory Allocation Failed");
      RLC_FREE_SHRABL_BUF(RLC_MEM_REGION_UL, RLC_POOL, sliceStats, sizeof(SlicePmList));
      return RFAILED;
   }
   while(dir < DIR_BOTH)
   {
      node = arrTputPerSnssai[dir]->first;
      if(node == NULLP)
      {
         DU_LOG("\nERROR  -->  RLC: No SNSSAI in list");
         RLC_FREE_SHRABL_BUF(RLC_MEM_REGION_UL, RLC_POOL, sliceStats, sizeof(SlicePmList));
         RLC_FREE_SHRABL_BUF(RLC_MEM_REGION_UL, RLC_POOL, sliceStats->sliceRecord, (snssaiCnt * (sizeof(SlicePm))));
         return RFAILED;
      }

      while(node)
      {
         snssaiIdx = 0;
         snssaiNode = (RlcTptPerSnssai *)node->node;

         snssaiVal.sst = snssaiNode->snssai->sst;
         snssaiVal.sd = snssaiNode->snssai->sd[2]+snssaiNode->snssai->sd[1]*10+snssaiNode->snssai->sd[0]*100;
         if(rlcFindSliceEntry(snssaiVal, &snssaiIdx, sliceStats) == FALSE)
         {
            sliceStats->sliceRecord[snssaiIdx].networkSliceIdentifier = snssaiVal;
            sliceStats->numSlice++;
         }
         if(dir == DIR_UL)
         {
            sliceStats->sliceRecord[snssaiIdx].ThpUl = snssaiNode->tpt;
         }
         else
         {
            sliceStats->sliceRecord[snssaiIdx].ThpDl = snssaiNode->tpt;
         }
         node = node->next;
      }
      dir++;
   }
   
   sendSlicePmToDu(sliceStats);
   return ROK;
}

/*******************************************************************
 *
 * @brief sending UE reestablishment response to DU 
 *
 * @details
 *
 *    Function : sendRlcUeReestablishRspToDu 
 *
 *    Functionality:
 *      sending UE reestablishment response to DU 
 *
 * @params[in] uint8_t cellId, uint8_t ueId, CauseOfResult  status 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sendRlcUeReestablishRspToDu(uint16_t cellId,uint8_t ueId, CauseOfResult  status)
{
   Pst pst;  
   RlcUeReestablishRsp *ueReestablishRsp = NULLP;
   
   FILL_PST_RLC_TO_DUAPP(pst, RLC_UL_INST, EVENT_RLC_UE_REESTABLISH_RSP);

   RLC_ALLOC_SHRABL_BUF(pst.region, pst.pool, ueReestablishRsp, sizeof(RlcUeReestablishRsp));
   if(!ueReestablishRsp)
   {
      DU_LOG("\nERROR  -->  RLC: sendRlcUeReestablishRspToDu(): Memory allocation failed ");
      return RFAILED;
   }
   else
   {
      ueReestablishRsp->cellId = cellId;
      ueReestablishRsp->ueId = ueId;
      ueReestablishRsp->status = status;
  
      if(rlcSendUeReestablishRspToDu(&pst, ueReestablishRsp) == ROK)
      {
         DU_LOG("\nDEBUG  -->  RLC: UE Reestablishment response send successfully");
      }
      else
      {
         DU_LOG("\nERROR  -->  RLC: SendRlcUeReestablishRspToDu():Failed to send UE Reestablishment response to DU");
         RLC_FREE_SHRABL_BUF(pst.region, pst.pool, ueReestablishRsp, sizeof(RlcUeReestablishRsp));
         return RFAILED;
      }
   }
   return ROK;
}

/*******************************************************************
*
* @brief Handles Ue reestablishment Request from DU APP
*
* @details
*
*    Function : RlcProcUeReestablishReq
*
*    Functionality:
*      Handles Ue reestablishment Request from DU APP
*
* @params[in] Post structure pointer
*             RlcUeReestablishReq pointer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t RlcProcUeReestablishReq(Pst *pst, RlcUeReestablishReq *ueReestablishReq)
{
   uint8_t ret = RFAILED;
   RlcCb *gRlcCb = NULLP;
   RlcUlUeCb *ueCb = NULLP;
   CauseOfResult  status =SUCCESSFUL;

   if(ueReestablishReq != NULLP)
   {
      gRlcCb = RLC_GET_RLCCB(pst->dstInst);
      rlcDbmFetchUlUeCb(gRlcCb,ueReestablishReq->ueId, ueReestablishReq->cellId, &ueCb);
      if(ueCb != NULLP)
      {
         if(ueReestablishReq->cellId == ueCb->cellId)
         {
            /* TODO : 
             * Step 1: Fill the RlcCfgInfo structure with data from the ueReestablishReq, just as we did in fillRlcCfg function and set 
             * ConfigType = CONFIG_REESTABLISH
             * Step 2: To finish processing of Ue Reestablishment, call the RlcProcCfgReq function */
         }
         else
         {
            status = CELLID_INVALID;
            DU_LOG("\nERROR  -->  SCH : RlcProcUeReestablishReq(): cell Id[%d] not found", ueReestablishReq->cellId);
         }
      }
      else
      {
         status = UEID_INVALID;
         DU_LOG("\nERROR  -->  SCH : RlcProcUeReestablishReq(): ue Id[%d] not found", ueReestablishReq->cellId);
      }

      if(status != SUCCESSFUL)
      {
         if(sendRlcUeReestablishRspToDu(ueReestablishReq->cellId, ueReestablishReq->ueId, status)  != ROK)
         {
            DU_LOG("\nERROR  -->  RLC: RlcProcUeReestablishReq():Failed to send UE Reestablishment response to DU");
         }
      }
      RLC_FREE_SHRABL_BUF(pst->region, pst->pool, ueReestablishReq, sizeof(RlcUeDelete));
   }
   else
   {
      DU_LOG("\nERROR  -->  RLC: RlcProcUeReestablishReq(): Recieved NULL pointer UE Reestablishment ");
   }
   return ret;
}
/**********************************************************************
  End of file
 **********************************************************************/
