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

/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     rg_sch_gom.c 
  
**********************************************************************/

/** @file rg_sch_gom.c 
@brief This module does processing related to handling of upper interface APIs 
invoked by RRM towards MAC.
*/


/* header include files -- defines (.h) */
#include "common_def.h"
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch.h"
#include "rg_sch_err.h"
#include "sch_utils.h"

/* header/extern include files (.x) */
#include "lrg.x"
#include "rgr.x"
#include "tfu.x"
#include "rg_sch_inf.x"
#include "rg_sch.x"
#include "mac_sch_interface.h"

/* local defines */
static S16 rgSCHGomHndlCfgReq ARGS((RgSchCb *instCb, SpId spId,
         RgrCfg  *cfg, RgSchErrInfo *errInfo));
static S16 rgSCHGomHndlRecfgReq ARGS((RgSchCb *instCb, SpId spId,
         RgrRecfg  *recfg, RgSchErrInfo *errInfo));
static S16 rgSCHGomHndlResetReq ARGS((RgSchCb *instCb,SpId spId,RgrRst *reset,
         RgSchErrInfo  *errInfo));
static S16 rgSCHGomGetCellIdFrmCfgReq ARGS((RgrCfgReqInfo *rgrCfgReq,
         CmLteCellId *cellId));
static S16 rgSCHGomCfgReq ARGS((Region reg, Pool pool, RgSchCb *instCb,
         RgrCfgTransId transId, RgrCfgReqInfo *cfgReqInfo));
static S16 rgSCHGomEnqCfgReq ARGS((Region reg, Pool pool, RgSchCellCb *cell,
         RgrCfgTransId transId, RgrCfgReqInfo *rgrCfgReq));
static S16 rgSCHGomHndlDelReq ARGS((RgSchCb  *instCb,SpId spId, 
         RgrDel  *del,RgSchErrInfo  *errInfo));
#ifdef LTE_ADV
static S16 rgSCHGomHndlSCellActDeactReq ARGS((RgSchCb *instCb, SpId spId,
         RgrSCellActDeactEvnt  *sCellActDeactEvnt, RgSchErrInfo *errInfo, uint8_t action));
#endif /* LTE_ADV */
#ifdef EMTC_ENABLE
S16 rgSchEmtcGetSiWinPerd ARGS((
RgSchCellCb   *cell,
uint16_t  *siWinSize,
uint16_t  *minPeriod 
));
extern S16 rgSCHEmtcUtlCalMcsAndNPrb
(
 RgSchCellCb         *cell,
 uint8_t                  cfgType,
 MsgLen              msgLen,
 uint8_t                  siId
 );

S32 rgSCHEmtcUtlGetAllwdCchTbSzForSI ARGS(
(
uint32_t bo
));

Void rgSCHEmtcWarningSiCfg ARGS(
(
RgSchCellCb             *cell,
RgrWarningSiCfgReqInfo  *warningSiCfgReqInfo,
uint16_t                     idx
));
#endif


/* local typedefs */
 
/* local externs */
 
/* forward references */



/**
 * @brief Handler for config request from RRM to Schedular.
 *
 * @details
 *
 *     Function: rgSCHGomHndlCfg
 *     
 *     This API is called from schedulers UIM and it handles config request
 *     from RRM to Scheduler.
 *     
 *     Processing Steps:
 *      - If the request is for the inactive cell, 
 *       - Handle request.Call rgSCHGomCfgReq.
 *      - Else,
 *       - Enqueue the request. Call rgSCHGomEnqCfgReq.
 *         
 *  @param[in]  Pst           *pst
 *  @param[in]  RgSchCb       *instCb
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  RgrCfgReqInfo *cfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHGomHndlCfg(Pst *pst,RgSchCb  *instCb,RgrCfgTransId transId,RgrCfgReqInfo *cfgReqInfo)
{
   S16           ret;
   SpId          spId = 0;
   CmLteCellId   cellId;
   RgSchCellCb   *cell = NULLP;    
   uint8_t       cfmStatus = RGR_CFG_CFM_NOK;

   /* Apply the configuration for Cell Configuration or Delete */
   if (cfgReqInfo->action != RGR_RECONFIG)
   {
      ret = rgSCHGomCfgReq (pst->region, pst->pool, instCb, 
            transId, cfgReqInfo);
      return (ret);
   }

   /* Fetch the cell Id for the recieved request */
   if((rgSCHGomGetCellIdFrmCfgReq(cfgReqInfo, &cellId)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Action.Config Type Error");

      SCH_FREE(cfgReqInfo, 
            (Size)sizeof(*cfgReqInfo));
      cfgReqInfo = NULLP;
      schSendCfgCfm(pst->region, pst->pool, transId, cfmStatus); 
      return RFAILED;
   }
   /* Extract the cell and Enquee Config Request */
   if(NULLP != instCb->rgrSap[spId].cell)
   {
      if(cellId != instCb->rgrSap[spId].cell->cellId) 
      {
         DU_LOG("\nERROR  -->  SCH : Cell with Id %d already exists "
           "on sap %d", instCb->rgrSap[spId].cell->cellId, spId);  

         SCH_FREE(cfgReqInfo, 
               (Size)sizeof(*cfgReqInfo));
         cfgReqInfo = NULLP;
         schSendCfgCfm(pst->region, pst->pool, transId, cfmStatus); 
         return RFAILED;
      }
      cell = instCb->rgrSap[spId].cell;

      /* Enqueue the configuration */
      ret = rgSCHGomEnqCfgReq(pst->region, pst->pool, cell, transId, cfgReqInfo);
      if (ret != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : rgSCHGomHndlCfg: Enqueuing CfgReq "
              "Failed ");

         SCH_FREE(cfgReqInfo, 
               (Size)sizeof(*cfgReqInfo));
         cfgReqInfo = NULLP;
         schSendCfgCfm(pst->region, pst->pool, transId, cfmStatus); 
         return RFAILED;
      }

      return ROK;
   }
   SCH_FREE(cfgReqInfo, 
         (Size)sizeof(*cfgReqInfo));
   cfgReqInfo = NULLP;
   schSendCfgCfm(pst->region, pst->pool, transId, cfmStatus); 
   return RFAILED;

}/* rgSCHGomHndlCfg */


/**
 * @brief Handler to handle config request from RRM to Scheduler.
 *
 * @details
 *
 *     Function: rgSCHGomCfgReq
 *     
 *     This API handles processing for config request from RRM to Scheduler.
 *     
 *     Processing Steps: 
 *      - If Configuration request, call rgSCHGomHndlCfgReq.
 *      - Else if Reconfiguration request, call rgSCHGomHndlRecfgReq.
 *      - If successful, send configuration confirm to RRM.
 *        Call schSendCfgCfm else FAIL.
 *
 *  @param[in]  Region        reg
 *  @param[in]  Pool          pool
 *  @param[in]  RgSchCb       *instCb
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  RgrCfgReqInfo *cfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgSCHGomCfgReq(Region  reg,Pool  pool,RgSchCb *instCb,RgrCfgTransId transId,RgrCfgReqInfo *cfgReqInfo)
{
   SpId            spId = 0;
   uint8_t         cfmStatus = RGR_CFG_CFM_OK;
   S16             ret;
   RgSchErrInfo    errInfo;
#ifdef EMTC_ENABLE
printf("\nDEBUG  -->  SCH: AT MAC rgSCHGomCfgReq \n");
#endif

   /* Process Config/Reconfig/Delete request from RRM */
   switch (cfgReqInfo->action)
   {
      case SCH_CONFIG:
         {
            ret = rgSCHGomHndlCfgReq(instCb, spId, 
                             &cfgReqInfo->u.cfgInfo, &errInfo);
            break;
         }
      case RGR_RECONFIG:
         {
            ret = rgSCHGomHndlRecfgReq(instCb, spId, 
                             &cfgReqInfo->u.recfgInfo, &errInfo);
            break;
         }
      case RGR_RESET:
         {
            ret = rgSCHGomHndlResetReq(instCb, spId, 
                             &cfgReqInfo->u.rstInfo, &errInfo);
            break;
         }
      case RGR_DELETE:
         {
            ret = rgSCHGomHndlDelReq(instCb, spId,
                             &cfgReqInfo->u.delInfo, &errInfo);
            break;
         } 
#ifdef LTE_ADV
      case RGR_SCELL_ACT:
      case RGR_SCELL_DEACT: 
      case RGR_SCELL_READY: 
         {
            ret = rgSCHGomHndlSCellActDeactReq(instCb, spId,
                             &cfgReqInfo->u.sCellActDeactEvnt, &errInfo, cfgReqInfo->action);
            break;
         }
#endif /* LTE_ADV */
      default:
         {
            DU_LOG("\nERROR  -->  SCH : Invalid configuration "
               "action %d", cfgReqInfo->action);
            ret = RFAILED;
         }
   } /* End of switch */

   if (ret != ROK)
   {
      cfmStatus = RGR_CFG_CFM_NOK;
   }

   SCH_FREE(cfgReqInfo, 
         (Size)sizeof(*cfgReqInfo));
   cfgReqInfo = NULLP;
#ifdef EMTC_ENABLE
printf("\nDEBUG  -->  SCH: AT MAC sending RGR cfg cfm \n");
#endif

   /* Send back confirmation status to RRM */   
   schSendCfgCfm(reg, pool, transId, cfmStatus); 
#ifdef EMTC_ENABLE
printf("\nDEBUG  -->  SCH: AT MAC RGR cfg cfm sent\n");
#endif

   return (ret);
}  /* rgSCHGomCfgReq */


/**
 * @brief Handler to enqueuing config request from RRM to Scheduler.
 *
 * @details
 *
 *     Function: rgSCHGomEnqCfgReq
 *     
 *     This API enqueues config request from RRM to MAC. 
 *     
 *     Processing Steps:
 *      - Allocate the configuration request element.
 *      - Copy the contents of the recieved configuration to config request
 *        element and free the recieved configuration pointer.
 *      - If the configuration is without activation time,
 *         - Enqueue the request in crntRgrCfgLst of the cell at the end of 
 *           the list.
 *      - Else
 *         - Enqueue the request in pndngRgrCfgLst of the cell.
 *         
 *  @param[in]  Region        reg,
 *  @param[in]  Pool          pool
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  RgrCfgReqInfo *rgrCfgReq
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgSCHGomEnqCfgReq(Region reg,Pool pool,RgSchCellCb *cell,RgrCfgTransId transId,RgrCfgReqInfo *rgrCfgReq)
{
   S16                ret;
   uint32_t           sfDiff;
   RgSchCfgElem       *rgrCfgElem  = NULLP;
   CmLteTimingInfo    actvTime; 
   Inst               inst = cell->instIdx;

   /* Allocate memory for config Element */
   ret = rgSCHUtlAllocSBuf(inst, (Data **)&rgrCfgElem, sizeof(RgSchCfgElem));
   if ((ret != ROK) || ((uint8_t *)rgrCfgElem == NULLP))
   {
      return RFAILED;
   }

   /* Initialize the configuration element */
   memcpy(rgrCfgElem->rgrCfg.transId.trans,transId.trans,
         sizeof(transId.trans));
   rgrCfgElem->rgrCfg.reg = reg;
   rgrCfgElem->rgrCfg.pool = pool;
   rgrCfgElem->rgrCfg.rgrCfgReq = rgrCfgReq;
   rgrCfgElem->cfgReqLstEnt.prev = NULLP;
   rgrCfgElem->cfgReqLstEnt.next = NULLP;
   rgrCfgElem->cfgReqLstEnt.node = (PTR )rgrCfgElem;

   /* Add configuration element to current/pending cfgLst */
   if (((rgrCfgReq->action == RGR_RECONFIG) &&
            (rgrCfgReq->u.recfgInfo.recfgType == RGR_CELL_CFG) &&
            (rgrCfgReq->u.recfgInfo.u.cellRecfg.recfgActvTime.pres == TRUE)))

   {
      actvTime = 
         rgrCfgReq->u.recfgInfo.u.cellRecfg.recfgActvTime.actvTime;

      /* Check if the activation time is valid */
      if (actvTime.sfn >= RGSCH_MAX_SFN 
            || actvTime.slot >= RGSCH_NUM_SUB_FRAMES_5G)
      {
         DU_LOG("\nERROR  -->  SCH : Invalid activation time for RGR "
           "config request: activation sfn %d activation slot %d current "
           "sfn %d current slot %d", actvTime.sfn, actvTime.slot, 
           cell->crntTime.sfn, cell->crntTime.slot);
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(inst, (Data **)&rgrCfgElem, sizeof(*rgrCfgElem));
         return RFAILED;
      }

      sfDiff = RGSCH_CALC_SF_DIFF(actvTime, cell->crntTime);

      if (sfDiff > (RGR_ACTV_WIN_SIZE * RGSCH_NUM_SUB_FRAMES_5G))
      {
         DU_LOG("\nERROR  -->  SCH : Invalid activation time for RGR"
              " config request: activation sfn %d activation slot %d "
              "current sfn %d current slot %d", actvTime.sfn,
              actvTime.slot, cell->crntTime.sfn, cell->crntTime.slot);
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(inst, (Data **)&rgrCfgElem, sizeof(*rgrCfgElem));
         return RFAILED;
      }

      if (sfDiff)
      {
         /* Add to pending cfgReqLst */
         rgrCfgElem->actvTime = actvTime; 
         rgSCHDbmInsPndngRgrCfgElem(cell, rgrCfgElem);
         /* Cfm to be sent only after applying request */
         return ROK;
      }
   }

   /* Add to current cfgReq list */
   rgSCHDbmInsCrntRgrCfgElem(cell, rgrCfgElem);
   /* Cfm to be sent only after applying request */
   return ROK;
}  /* rgSCHGomEnqCfgReq */


/**
 * @brief Handler for TTI processing for configurations recieved from RRM.
 *
 * @details
 *
 *     Function: rgSCHGomTtiHndlr
 *     
 *     This API does TTI processing for configurations recieved from RRM.
 *     
 *     Processing Steps:
 *      - It dequeues config request from the current configuration list.
 *        For each config request in the list: 
 *        - Processes the request. Call rgSCHGomCfgReq.
 *      - It dequeues config request for the current tti from the pending 
 *        configuration list. For each config request in the list:
 *        - Processes the request. Call rgSCHGomCfgReq.
 *
 *  @param[in]  RgSchCellCb *cell
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHGomTtiHndlr(RgSchCellCb *cell,SpId  spId)
{
   RgSchCfgElem       *cfgElem;
   Inst               inst= cell->instIdx;

   /* Dequeue from current config list */
   while ((cfgElem = rgSCHDbmGetNextCrntRgrCfgElem(cell, NULLP)) != NULLP)
   {
      rgSCHDbmDelCrntRgrCfgElem(cell, cfgElem);
      rgSCHGomCfgReq(cfgElem->rgrCfg.reg,cfgElem->rgrCfg.pool,
            &rgSchCb[inst], cfgElem->rgrCfg.transId, 
            cfgElem->rgrCfg.rgrCfgReq);
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)&cfgElem, sizeof(*cfgElem));
   }

   /* Handle config requests from pending config list */
   while((cfgElem = rgSCHDbmGetPndngRgrCfgElemByKey(cell, cell->crntTime)) != NULLP)
   {
      rgSCHDbmDelPndngRgrCfgElem(cell, cfgElem);
      rgSCHGomCfgReq(cfgElem->rgrCfg.reg, cfgElem->rgrCfg.pool, 
            &rgSchCb[inst], cfgElem->rgrCfg.transId, 
            cfgElem->rgrCfg.rgrCfgReq);
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)&cfgElem, sizeof(*cfgElem));
   } 

   return ROK;
}


/**
 * @brief Handler to handle configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function: rgSCHGomHndlCfgReq
 *     
 *     This API handles processing for configuration request from RRM to MAC.
 *     
 *     - Processing Steps: 
 *        - Validate configuration request parameters at CFG module. 
 *          Call rgSCHCfgVldtRgrCellCfg for cell configuration.
 *        - If validated successfully, send configuration request to CFG.
 *          Call rgSCHCfgRgrCellCfg else FAIL.
 *          
 *  @param[in]  RgSchCb      *instCb
 *  @param[in]  SpId         spId
 *  @param[in]  RgrCfg       *cfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgSCHGomHndlCfgReq(RgSchCb *instCb,SpId spId,RgrCfg *cfg,RgSchErrInfo *errInfo)
{
   S16          ret;
   RgSchCellCb  *cell = instCb->rgrSap[spId].cell;
   Inst         inst = (instCb->rgSchInit.inst );
   RgSchUeCb    *ue;

   errInfo->errType = RGSCHERR_GOM_CFG_REQ;
   
   /* Validate and process the configuration request */ 
   switch (cfg->cfgType)
   {
      case RGR_CELL_CFG:
      {
         ret = rgSCHCfgVldtRgrCellCfg(inst, &cfg->u.cellCfg, cell, errInfo);
            if (ret != ROK)
            {
               DU_LOG("\nERROR  -->  SCH : Rgr Cell configuration "
                  "validation FAILED: Cell %d", cfg->u.cellCfg.cellId);
               return RFAILED;
            }
         ret = rgSCHCfgRgrCellCfg(instCb, spId, &cfg->u.cellCfg, errInfo);
         break;
      }
      case RGR_UE_CFG:
      case RGR_SCELL_UE_CFG:
      {
         ret = rgSCHCfgVldtRgrUeCfg(inst, &cfg->u.ueCfg, &cell, errInfo);
         if (ret != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Ue configuration validation"
               " FAILED: CRNTI:%d", cfg->u.ueCfg.crnti);
            return RFAILED;
         }
         ret = rgSCHCfgRgrUeCfg(cell, &cfg->u.ueCfg, errInfo);
         break;
      }
      case RGR_LCH_CFG:
      {
         ret = rgSCHCfgVldtRgrLcCfg(inst, &cfg->u.lchCfg, &cell, &ue, errInfo);
         if (ret != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : LC configuration validation "
              "FAILED: LCID:%d", cfg->u.lchCfg.lcId);
            return RFAILED;
         }
         ret = rgSCHCfgRgrLchCfg(cell, ue, &cfg->u.lchCfg, errInfo); 
         break;
      }
      case RGR_LCG_CFG:
      {
         ret = rgSCHCfgVldtRgrLcgCfg(inst, &cfg->u.lcgCfg, &cell, &ue, errInfo);
         if (ret != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : LCG configuration validation "
              "FAILED: LCGID:%d", cfg->u.lcgCfg.ulInfo.lcgId);
            return RFAILED;
         }
         ret = rgSCHCfgRgrLcgCfg(cell, ue, &cfg->u.lcgCfg, errInfo); 
         break;
      }
      case MAC_GNB_CFG:
      {
         ret = rgSCHCfgVldtRgrSchedEnbCfg(inst, &cfg->u.schedEnbCfg, errInfo);
         if (ret != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : SCH ENB configuration validation FAILED: \n");
            return RFAILED;
         }
         ret = rgSCHCfgRgrSchedEnbCfg(inst, spId, &cfg->u.schedEnbCfg, errInfo); 
         break;
      }
      default:
      {
#if(ERRCLASS & ERRCLS_INT_PAR)
            DU_LOG("\nERROR  -->  SCH : Should never come here: "
                "cfgType %d", cfg->cfgType);
#endif
            return RFAILED;
      }
   }
   
   return (ret);
}  /* rgSCHGomHndlCfgReq */

#ifdef LTE_ADV
/**
 * @brief Handler to handle re-configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function: rgSCHGomHndlSCellActDeactReq
 *     
 *     This API handles processing for SCell Activation Request from RRM to SCH.
 *     
 *     - Processing Steps: 
 *        - Validate sCell Actication request parameters at CFG module. 
 *        - If validated successfully, send configuration request to CFG.
 *        - call activation function for each SCells configured
 *
 *  @param[in]  RgSchCb      *instCb
 *  @param[in]  SpId         spId
 *  @param[in]  RgrSCellActDeactEvnt  *sCellActDeactEvnt
 *  @param[in]  uint8_t action
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgSCHGomHndlSCellActDeactReq(RgSchCb  *instCb,SpId  spId,RgrSCellActDeactEvnt  *sCellActDeactEvnt,RgSchErrInf *errInfo,uint8_t action)
{
   RgSchUeCb    *ue = NULLP;
   uint16_t     idx = 0;
   uint16_t     sCellIdx = 0;
   RgSchCellCb  *cell = instCb->rgrSap[spId].cell;
   Inst         inst = (instCb->rgSchInit.inst);

   DU_LOG(("\nDEBUG  -->  SCH : Processing RGR SCell Actication request:"
                  "%d\n", sCellActDeactEvnt->crnti));

   errInfo->errType = RGSCHERR_GOM_SCELL_REQ;

   /* Fetch the Ue */
   if ((ue = rgSCHDbmGetUeCb(cell, sCellActDeactEvnt->crnti)) == NULLP)
   {
      DU_LOG(("\nERROR  -->  SCH : [%d]UE: does not exist\n", 
            sCellActDeactEvnt->crnti));
      return RFAILED;
   }
   
   for(idx = 0; idx < sCellActDeactEvnt->numOfSCells; idx++)
   {
      sCellIdx = sCellActDeactEvnt->sCellActDeactInfo[idx].sCellIdx;

      if (ROK != (rgSCHSCellTrigActDeact(cell, ue, sCellIdx, action)))
      {
            DU_LOG("\nERROR  -->  SCH : SCell Actication failed"
                     "for UE [%d] with SCellIdx [%d]\n", 
                     sCellActDeactEvnt->crnti, idx);
            return RFAILED;

      }

   DU_LOG("\nINFO  -->  SCH : RGR Reconfiguration processed\n");
   return ROK;
}  /* rgSCHGomHndlSCellActDeactReq */

#endif /* LTE_ADV */
/**
 * @brief Handler to handle re-configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function: rgSCHGomHndlRecfgReq
 *     
 *     This API handles processing for re-configuration request from RRM to MAC.
 *     
 *     - Processing Steps: 
 *        - Validate re-configuration request parameters at CFG module. 
 *          Call rgSCHCfgVldtRgrCellRecfg for cell re-configuration.
 *        - If validated successfully, send configuration request to CFG.
 *          Call rgSCHCfgRgrCellRecfg else FAIL.
 *
 *  @param[in]  RgSchCb      *instCb
 *  @param[in]  SpId         spId
 *  @param[in]  RgrRecfg     *recfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgSCHGomHndlRecfgReq(RgSchCb  *instCb,SpId  spId,RgrRecfg *recfg,RgSchErrInfo  *errInfo)
{
   RgSchUeCb    *ue = NULLP;
   RgSchDlLcCb  *dlLc = NULLP; /* PURIFY_FIX:UMR */
   S16          ret; 
   RgSchCellCb  *cell = instCb->rgrSap[spId].cell;
   Inst         inst = (instCb->rgSchInit.inst );

   errInfo->errType = RGSCHERR_GOM_RECFG_REQ;
   
   /* Validate and process the re-configuration request */
   switch (recfg->recfgType)
   {
      case RGR_CELL_CFG:
      {
         ret = rgSCHCfgVldtRgrCellRecfg(inst, &recfg->u.cellRecfg, &cell,
               errInfo);
         if (ret != ROK) 
         {
            DU_LOG("\nERROR  -->  SCH : Rgr Cell Recfg Validation "
                     "FAILED");
            return RFAILED;
         }
         ret = rgSCHCfgRgrCellRecfg(cell, &recfg->u.cellRecfg, errInfo);
         break;
      }
      case RGR_UE_CFG:
      case RGR_SCELL_UE_CFG:
      {
         ret = rgSCHCfgVldtRgrUeRecfg(inst, &recfg->u.ueRecfg, &cell, &ue, errInfo);
         if ( ret != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Ue Recfg Validation FAILED"
                      "OLD CRNTI:%d",recfg->u.ueRecfg.oldCrnti);
            return RFAILED;
         }
         ret = rgSCHCfgRgrUeRecfg(cell, ue, &recfg->u.ueRecfg, errInfo);
         break;
      }
      case RGR_LCH_CFG:
      {
         ret = rgSCHCfgVldtRgrLchRecfg(inst, &recfg->u.lchRecfg, &cell, &ue, 
               &dlLc, errInfo);
         if (ret != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Lc Recfg Validation FAILED"
                      "LCID:%d",recfg->u.lchRecfg.lcId);
            return RFAILED;
         }
         ret = rgSCHCfgRgrLchRecfg(cell, ue, dlLc, &recfg->u.lchRecfg, errInfo);
         break;
      }
      case RGR_LCG_CFG:
      {
         ret = rgSCHCfgVldtRgrLcgRecfg(inst, &recfg->u.lcgRecfg, cell, &ue,
               errInfo);
         if (ret != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Lcg Recfg Validation FAILED"
                      "LCGID:%d",recfg->u.lcgRecfg.ulRecfg.lcgId);
            return RFAILED;
         }
         ret = rgSCHCfgRgrLcgRecfg(cell, ue, &recfg->u.lcgRecfg, errInfo);
         break;
      }
      default:
      {
#if(ERRCLASS & ERRCLS_INT_PAR)
         DU_LOG("\nERROR  -->  SCH : Should never come here: recfgType %d", recfg->recfgType);
#endif
         return RFAILED;
      }
   }

   return (ret);
}  /* rgSCHGomHndlRecfgReq */

/**
 * @brief Handler to handle UE reset request from RRM to Scheduler.
 *
 * @details
 *
 *     Function: rgSCHGomHndlResetReq
 *     
 *     This API handles processing for UE reset request from RRM to Scheduler.
 *     
 *     - Processing Steps: 
 *        - Validate UE reset request parameters at CFG module. 
 *          Call rgSCHCfgVldtRgrUeReset for UE reset.
 *        - If validated successfully, send UE reset request to CFG.
 *          Call rgSCHCfgRgrUeReset else FAIL.
 *
 *  @param[in]  RgrRst   *rstInfo
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgSCHGomHndlResetReq(RgSchCb  *instCb,SpId spId,RgrRst *reset,RgSchErrInfo  *errInfo)
{
   S16          ret; 
   RgSchCellCb  *cell= instCb->rgrSap[spId].cell;
   Inst         inst = (instCb->rgSchInit.inst );
   RgSchUeCb    *ue = NULLP;

   errInfo->errType = RGSCHERR_GOM_RESET_REQ;
   
   /* Validate and process the UE reset request */
   ret = rgSCHCfgVldtRgrUeReset(inst, reset, cell, &ue, errInfo);
   if (ret != ROK) 
   {
      DU_LOG("\nERROR  -->  SCH : Rgr UE Reset Validation FAILED"
               "CRNTI:%d",reset->crnti);
      return RFAILED;
   }
   
   ret = rgSCHCfgRgrUeReset(cell, ue, reset, errInfo);
   if (ret != ROK) 
   {
      DU_LOG("\nERROR  -->  SCH : Rgr UE Reset FAILED"
               "CRNTI:%d",reset->crnti);
      return RFAILED;
   }

   return (ret);
}  /* rgSCHGomHndlResetReq */


/**
 * @brief Handler for processing Cell/Ue/Logical channel delete request
 * recieved from RRM.
 *
 * @details
 *
 *     Function: rgSCHGomHndlDelReq
 *     
 *     This API handles processing of delete request from RRM to MAC. 
 *     
 *     Processing Steps:
 *        - Fetch corresponding control block and pass it to CFG module.
 *        - If control block does not exist, FAIL.
 *
 *  @param[in]  RgSchCb      *instCb
 *  @param[in]  SpId         spId
 *  @param[in]  RgrDel       *del
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgSCHGomHndlDelReq(RgSchCb *instCb,SpId spId,RgrDel *del,RgSchErrInfo  *errInfo)
{

   S16       ret;
   volatile uint32_t     startTime=0;

   errInfo->errType = RGSCHERR_GOM_DEL_REQ;

   if(instCb->rgrSap[spId].cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : Cell doesnt exist");
      return RFAILED; 
   }
   
   /* Process the delete request */ 
   switch (del->delType)
   {
      case RGR_CELL_CFG:
      {
         ret = rgSCHCfgRgrCellDel(instCb->rgrSap[spId].cell, del, errInfo);
         if(ret == ROK)
         {
            /* TODO::Needs to be revisited after tti flow CaDev Start */
            uint8_t idx = (uint8_t)((instCb->rgrSap[spId].cell->cellId - instCb->genCfg.startCellId)&(CM_LTE_MAX_CELLS-1));
            instCb->cells[idx] = NULLP;
            /* CaDev End */
            instCb->rgrSap[spId].cell = NULLP;
            instCb->tfuSap[spId].cell = NULLP;
         }
         break;
      }
      case RGR_UE_CFG:
      case RGR_SCELL_UE_CFG:
      {

         /*starting Task*/
         SStartTask(&startTime, PID_SCH_UE_DEL);

         ret = rgSCHCfgRgrUeDel(instCb->rgrSap[spId].cell, del, errInfo);

         /*stoping Task*/
         SStopTask(startTime, PID_SCH_UE_DEL);

         break;
      }
      case RGR_LCH_CFG:
      {
         ret = rgSCHCfgRgrLcDel(instCb->rgrSap[spId].cell, del, errInfo);
         break;
      }
      case RGR_LCG_CFG:
      {
         ret = rgSCHCfgRgrLcgDel(instCb->rgrSap[spId].cell, del, errInfo);
         break;
      }
      default:
      {
#if(ERRCLASS & ERRCLS_INT_PAR)
         DU_LOG("\nERROR  -->  SCH : Should never come here: delType %d", del->delType);
#endif
         return RFAILED;
      }
   }

   return (ret);
}  /* rgSCHGomHndlDelReq */




/***********************************************************
 *
 *     Func : rgSCHGomGetCellIdFrmCfgReq
 *        
 *
 *     Desc : 
 *     - Processing Steps: 
 *        - Retrieves the cell Id for a config request.
 *
 *  @param[in]  RgrCfgReqInfo *rgrCfgReq
 *  @param[out] CmLteCellId   *cellId
 *     Ret  : ROK on fetching cellId
 *            RFAILED on failure
 *
 *     Notes: 
 *
 *     File : rg_sch_gom.c 
 *
 **********************************************************/
static S16 rgSCHGomGetCellIdFrmCfgReq(RgrCfgReqInfo *rgrCfgReq,CmLteCellId   *cellId)
{

   /* Extract CellId depending on the action and Config Type in the Request 
    * As of now this function is called for only re configuration so removed
    * othe CASES below if needed we can add them*/
   switch (rgrCfgReq->action)
   {
      case RGR_RECONFIG:
         {
            if (rgrCfgReq->u.recfgInfo.recfgType ==RGR_CELL_CFG)
            {
               *cellId = rgrCfgReq->u.recfgInfo.u.cellRecfg.cellId;
            }
            else if ((rgrCfgReq->u.recfgInfo.recfgType ==  RGR_SCELL_UE_CFG) || 
                  (rgrCfgReq->u.recfgInfo.recfgType == RGR_UE_CFG))
            {
               *cellId = rgrCfgReq->u.recfgInfo.u.ueRecfg.cellId;
            }
            else if (rgrCfgReq->u.recfgInfo.recfgType == RGR_LCH_CFG)
            {
               *cellId = rgrCfgReq->u.recfgInfo.u.lchRecfg.cellId;
            }
            else if (rgrCfgReq->u.recfgInfo.recfgType == RGR_LCG_CFG) 
            {
               *cellId = rgrCfgReq->u.recfgInfo.u.lcgRecfg.cellId;
            }
            else
            {
               return RFAILED;
            }
            break;
         }
      default:
      {
         return RFAILED;
      }
   }  /* End of Switch */

   return ROK;
}  /* rgSCHGomGetCellIdFrmCfgReq */

#ifdef RGR_SI_SCH
/**
 * @brief Handler to handle SI configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function: rgSCHGomHndlSiCfg
 *     
 *     This API handles processing for SI configuration request from RRM to MAC.
 *     
 *     - Processing Steps: 
 *        - Validate SI configuration request parameters at CFG module. 
 *          Call rgSCHCfgVldtSiCfg for SI configuration.
 *        - If validated successfully, send configuration request to CFG.
 *          Call rgSCHCfgRgrCellCfg else FAIL.
 *          
 *  @param[in]  Region        reg
 *  @param[in]  Pool          pool
 *  @param[in]  RgSchCb       *instCb
 *  @param[in]  SpId          spId
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  RgrSiCfgReqInfo *cfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHGomHndlSiCfg(Region reg,Pool pool,RgSchCb *instCb,SpId spId,RgrCfgTransId transId,RgrSiCfgReqInfo *cfgReqInfo)
{
   S16          ret;
   RgSchCellCb  *cell = instCb->rgrSap[spId].cell;
   Inst         inst = (instCb->rgSchInit.inst );
   RgSchErrInfo errInfo;
   uint8_t      cfmStatus = RGR_CFG_CFM_NOK;
   MsgLen       msgLen = 0, pduLen;
   S32          tbSz   = 0;
   uint8_t      nPrb   = 0; 
   uint8_t      mcs    = 0;


   /* check if cell does not exists */
   if (((uint8_t *)cell == NULLP) || (cell->cellId != cfgReqInfo->cellId))
   {
      DU_LOG("\nERROR  -->  SCH : Cell Control block does not exist");
      RGSCH_FREE_MSG(cfgReqInfo->pdu);
      SCH_FREE(cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
      cfgReqInfo = NULLP;
      rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 
      return RFAILED;
   }

   /*Validate the received SI configuration */
   ret = rgSCHCfgVldtRgrSiCfg(inst, cfgReqInfo, cell, &errInfo);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Rgr SI configuration "
               "validation FAILED");
      RGSCH_FREE_MSG(cfgReqInfo->pdu);
      SCH_FREE(cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
      cfgReqInfo = NULLP;
      rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 
      return RFAILED;
   }
   /*ccpu00140789: Stopping SI scheduling*/
   if(RGR_SI_STOP == cfgReqInfo->cfgType)
   {
      if((cell->siCb.crntSiInfo.siInfo[cfgReqInfo->siId-1].si != NULLP)&&
                      (cell->siCb.siArray[cfgReqInfo->siId-1].si != NULLP))
      {
         cell->siCb.crntSiInfo.siInfo[cfgReqInfo->siId-1].si = NULLP;
         RGSCH_FREE_MSG(cell->siCb.siArray[cfgReqInfo->siId-1].si);
         cell->siCb.siArray[cfgReqInfo->siId-1].si = NULLP;
         if(cell->siCb.newSiInfo.siInfo[cfgReqInfo->siId-1].si != NULLP)
         {
            RGSCH_FREE_MSG(cell->siCb.newSiInfo.siInfo[cfgReqInfo->siId-1].si);
            cell->siCb.newSiInfo.siInfo[cfgReqInfo->siId-1].si = NULLP;
         }
         SCH_FREE(cfgReqInfo, (Size)sizeof(*cfgReqInfo));
         cfgReqInfo = NULLP;   
         cfmStatus = RGR_CFG_CFM_OK;
         rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus);
         return ROK; 
       }
       else
       {
          SCH_FREE(cfgReqInfo, (Size)sizeof(*cfgReqInfo));
          cfgReqInfo = NULLP;
          rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus);
          return RFAILED; 
       }
   }

   /* Check if the pdu sent from application
    * matches a transport block size. if not,
    * add padding bytes. This is usually done
    * by RRC but since we are bypassing RRC,
    * MAC is taking over that responsibility
    */
   if ( RGR_SI_CFG_TYPE_MIB != cfgReqInfo->cfgType )
   {
      SFndLenMsg(cfgReqInfo->pdu, &msgLen);

      /* check if the application pdu matches a tb size */
      tbSz = rgSCHUtlGetAllwdCchTbSz(msgLen*8, &nPrb, &mcs);

      if ( tbSz != (msgLen*8) )
      {
         MsgLen  nmPadBytes = 0;
         Data*   padding    = NULLP;

         /* need to add padding bytes */
         nmPadBytes = (tbSz - (msgLen*8))/8;

         SCH_ALLOC(padding,nmPadBytes);
         if(padding == NULLP)
         {
            DU_LOG("\nERROR  -->  SCH : Rgr SI configuration "
                     " failed to allocate memory for padding");
            SCH_FREE(cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
            cfgReqInfo = NULLP;
            rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, 
                                                           cfmStatus); 
            return RFAILED;
         }

         memset(padding,0,nmPadBytes);

#ifdef MS_MBUF_CORRUPTION 
   MS_BUF_ADD_ALLOC_CALLER();
#endif 
         if ( SAddPstMsgMult((Data*)padding,nmPadBytes,cfgReqInfo->pdu) != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Rgr SI configuration "
                     "Failed to add padding bytes");
            SCH_FREE(cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
            cfgReqInfo = NULLP;
            SCH_FREE(padding,(Size)nmPadBytes);
            padding = NULLP;
            rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, 
                                                            cfmStatus); 
            return RFAILED;
         }
         SCH_FREE(padding,(Size)nmPadBytes);
         padding = NULLP;
      }/* if (tbSz != ...*/
   }/* if (RGR_SI_CFG_TYPE_SI...*/

   /*Set the received pdu at the appropriate place */
   switch(cfgReqInfo->cfgType)
   {
      case RGR_SI_CFG_TYPE_MIB:   /* SI CFG Type MIB */
         RGSCHCHKNUPDSIPDU(cell->siCb.crntSiInfo.mib, 
               cell->siCb.newSiInfo.mib, 
               cfgReqInfo->pdu, cell->siCb.siBitMask, 
               RGSCH_SI_MIB_UPD);
         break;

      case RGR_SI_CFG_TYPE_SIB1_PWS:
         {
            SFndLenMsg(cfgReqInfo->pdu, &pduLen);
            ret = rgSCHUtlCalMcsAndNPrb(cell, cfgReqInfo->cfgType, pduLen,0);
            if (ret != ROK)
            {
               DU_LOG("\nERROR  -->  SCH : Failed to get MCS and NPRB" 
                     "value");
               RGSCH_FREE_MSG(cfgReqInfo->pdu);
               SCH_FREE(cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
               cfgReqInfo = NULLP;
               rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, 
                     cfmStatus); 
               return RFAILED;
            }

            RGSCHCHKNUPDSIPDU(cell->siCb.crntSiInfo.sib1Info.sib1, 
                  cell->siCb.newSiInfo.sib1Info.sib1, 
                  cfgReqInfo->pdu, cell->siCb.siBitMask, 
                  RGSCH_SI_SIB1_PWS_UPD);
         }
         break;

      case RGR_SI_CFG_TYPE_SIB1:
         SFndLenMsg(cfgReqInfo->pdu, &pduLen);
         ret = rgSCHUtlCalMcsAndNPrb(cell, cfgReqInfo->cfgType, pduLen,0);
         if (ret != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Failed to get MCS and NPRB" 
                     "value");
            RGSCH_FREE_MSG(cfgReqInfo->pdu);
            SCH_FREE(cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
            cfgReqInfo = NULLP;
            rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, 
                                                                 cfmStatus); 
            return RFAILED;
         }
         RGSCHCHKNUPDSIPDU(cell->siCb.crntSiInfo.sib1Info.sib1, 
               cell->siCb.newSiInfo.sib1Info.sib1, 
               cfgReqInfo->pdu, cell->siCb.siBitMask, 
               RGSCH_SI_SIB1_UPD);
         break;

      case RGR_SI_CFG_TYPE_SI:    /* SI CFG TYPE SI */
         SFndLenMsg(cfgReqInfo->pdu, &pduLen);
         ret = rgSCHUtlCalMcsAndNPrb(cell, cfgReqInfo->cfgType, pduLen, 
                                                     cfgReqInfo->siId);
         if (ret != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Failed to get MCS and NPRB" 
                     "value");
            RGSCH_FREE_MSG(cfgReqInfo->pdu);
            SCH_FREE(cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
            cfgReqInfo = NULLP;
            rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, 
                                                                 cfmStatus); 
            return RFAILED;
         }
         /* Si recfg, where numSi changes */
         if (cell->siCb.siBitMask & RGSCH_SI_SICFG_UPD) 
         {
            Buffer **newSiPdu = &cell->siCb.newSiInfo.siInfo[cfgReqInfo->siId-1].si;
            if(*newSiPdu != NULLP)
            {
               RGSCH_FREE_MSG(*newSiPdu);
            }
            *newSiPdu = (Buffer *)cfgReqInfo->pdu;
            cell->siCb.siBitMask |= RGSCH_SI_SI_UPD;
         }
         else /* Initial Si cfg  or si recfg where numSi did not change */
         {
            uint8_t bitMask;
            /* Initial Si cfg */
            if (cell->siCb.crntSiInfo.siInfo[cfgReqInfo->siId-1].si == NULLP)
            {
               cell->siCb.siArray[cfgReqInfo->siId-1].si = cfgReqInfo->pdu;
               cell->siCb.siArray[cfgReqInfo->siId-1].isWarningSi = FALSE;
               bitMask = RGSCH_SI_DFLT;
            }
            else
            {
               bitMask = RGSCH_SI_SI_UPD;
            }

            RGSCHCHKNUPDSIPDU(cell->siCb.crntSiInfo.siInfo[cfgReqInfo->siId-1].si, 
                cell->siCb.newSiInfo.siInfo[cfgReqInfo->siId-1].si, 
                cfgReqInfo->pdu, 
                cell->siCb.siBitMask, bitMask);
         }
         break;

      case RGR_SI_CFG_TYPE_SIB8_CDMA:    /* SI CFG TYPE SIB 8 CDMA */
         SFndLenMsg(cfgReqInfo->pdu, &pduLen);
         ret = rgSCHUtlCalMcsAndNPrb(cell, cfgReqInfo->cfgType, pduLen, 
                                                     cfgReqInfo->siId);
         if (ret != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Failed to get MCS and NPRB" 
                     "value");
            RGSCH_FREE_MSG(cfgReqInfo->pdu);
            SCH_FREE(cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
            cfgReqInfo = NULLP;
            rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, 
                                                                 cfmStatus); 
            return RFAILED;
         }
         /* No need to wait for Modification period boundary */
         cell->siCb.siArray[cfgReqInfo->siId-1].si = cfgReqInfo->pdu;
         RGSCH_SET_SI_INFO(cell->siCb.crntSiInfo.siInfo[cfgReqInfo->siId-1].si,
               cfgReqInfo->pdu);
         cell->siCb.siArray[cfgReqInfo->siId-1].isWarningSi = FALSE;
         break;
      default:
         DU_LOG("\nERROR  -->  SCH : Invalid cfgType "
                  "parameter value");
         RGSCH_FREE_MSG(cfgReqInfo->pdu);
         SCH_FREE(cfgReqInfo, 
               (Size)sizeof(*cfgReqInfo)); 
         cfgReqInfo = NULLP;
         rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, 
               cfmStatus); 
         return RFAILED;
   }

   SCH_FREE(cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
   cfgReqInfo = NULLP;
   cfmStatus = RGR_CFG_CFM_OK;
   rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 


   return ROK;
}  /* rgSCHGomHndlSiCfg */


/**
 * @brief Handler to handle Warning SI configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function: rgSCHGomHndlWarningSiCfg
 *     
 *     This API handles processing for Warning SI configuration request from 
 *     RRM to MAC.
 *     
 *          
 *  @param[in]  Region        reg
 *  @param[in]  Pool          pool
 *  @param[in]  RgSchCb       *instCb
 *  @param[in]  SpId          spId
 *  @param[in]  RgrWarningSiCfgReqInfo  *warningSiCfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHGomHndlWarningSiCfg
(
Region        reg,
Pool          pool,
RgSchCb       *instCb,
SpId          spId,
RgrCfgTransId transId,
RgrWarningSiCfgReqInfo *warningSiCfgReqInfo
)
{
   RgSchCellCb       *cell = instCb->rgrSap[spId].cell;
   uint8_t           cfmStatus = RGR_CFG_CFM_NOK;
   uint16_t          idx;
   uint8_t           siId = warningSiCfgReqInfo->siId; 
   uint8_t           j, mcs=0, nPrb=0;
   RgSchWarningSiSeg *warningSiMsg;
   RgSchWarningSiPdu *pduNode;
   CmLList           *node;
   MsgLen            msgLen = 0;
   Bool              freeNodeFound = FALSE;
   uint16_t          siWinSize = 0;
   uint16_t          minPeriod = 0;
#ifdef EMTC_ENABLE
   uint8_t           isEmtc = warningSiCfgReqInfo->emtcEnable;
#endif

#ifdef EMTC_ENABLE
   if(TRUE == isEmtc)
   {
      rgSchEmtcGetSiWinPerd(cell, &siWinSize, &minPeriod);
   }
   else
#endif
   {
      siWinSize = cell->siCfg.siWinSize;
      minPeriod = cell->siCfg.minPeriodicity;
   }
   /* check if cell does not exists */
   if (((uint8_t *)cell == NULLP) || 
         (cell->cellId != warningSiCfgReqInfo->cellId) ||
         (warningSiCfgReqInfo->siId > 
          ((minPeriod * 10)/siWinSize)))
   {
      DU_LOG("\nERROR  -->  SCH : Warning SI Cfg Failed for siId = %d"
                "warning cellID:%d",warningSiCfgReqInfo->siId,warningSiCfgReqInfo->cellId);
      rgSCHUtlFreeWarningSiSeg(reg, pool, &warningSiCfgReqInfo->siPduLst);
      SCH_FREE(warningSiCfgReqInfo, 
            sizeof(RgrWarningSiCfgReqInfo)); 
      warningSiCfgReqInfo = NULLP;
      rgSCHUtlRgrWarningSiCfgCfm(instCb->rgSchInit.inst, spId, siId, transId, 
            cfmStatus); 
      return RFAILED;
   }

   /* Search for free index in WarningSi */
   for(idx = 0; idx < RGR_MAX_NUM_WARNING_SI; idx++)
   {
      if((cell->siCb.warningSi[idx].siId == 0 ||
               cell->siCb.warningSi[idx].siId == warningSiCfgReqInfo->siId))
      {
         warningSiMsg = (RgSchWarningSiSeg *)&cell->siCb.warningSi[idx].warningSiMsg; 

         /* Search for free SI node */
         /* ccpu00136659: CMAS ETWS design changes */
         if (warningSiMsg->segLstCp.first == NULLP) /* Free SI Node */
         {
            warningSiMsg->transId = transId;
            pduNode = (RgSchWarningSiPdu *)&cell->siCb.warningSi[idx].
               warningSiMsg.pduNode;
            CM_LLIST_FIRST_NODE(&warningSiCfgReqInfo->siPduLst, node);
            j = 0;

            /* Get the PDUs one by one from the received pduLst of warning 
             * message and calculate the MCS and nPrb of each pdu once.
             * Store the pdu in warningSiMsg pduLst, which will be scheduled 
             * later while sending warning message as part of SIB11/SIB12 
             */   
            while((node != NULLP) && (j < RGR_MAX_WARNING_SI_SEG))

            {
               pduNode[j].pdu = (Buffer *)node->node;
               if(pduNode[j].pdu != NULLP)
               {
                  SFndLenMsg(pduNode[j].pdu, &msgLen);
                  /*Get the nPrb and mcs parametr values */
#ifdef EMTC_ENABLE
                  if (rgSCHEmtcUtlGetAllwdCchTbSzForSI(msgLen*8) != (msgLen*8))
#else
                  if (rgSCHUtlGetAllwdCchTbSz(msgLen*8, &nPrb, &mcs) != (msgLen*8))
#endif

                  {
                     DU_LOG("\nERROR  -->  SCH : rgSCHGomHndlWarningSiCfg():msgLen does not match\
                           any valid TB Size.");
                     DU_LOG("\nERROR  -->  SCH : Warning SI Cfg Failed" 
                           "for siId = %d", warningSiCfgReqInfo->siId);
                     rgSCHUtlFreeWarningSiSeg(reg, pool, 
                           &warningSiCfgReqInfo->siPduLst);
                     SCH_FREE(warningSiCfgReqInfo, 
                           sizeof(RgrWarningSiCfgReqInfo)); 
                     warningSiCfgReqInfo = NULLP;
                     rgSCHUtlRgrWarningSiCfgCfm(instCb->rgSchInit.inst, spId,
                           siId, transId,cfmStatus); 
                     return RFAILED;

                  }
               }
               pduNode[j].mcs = mcs;
               pduNode[j].nPrb = nPrb;
               pduNode[j].msgLen = msgLen;
               /* ccpu00136659: CMAS ETWS design changes */
               cmLListAdd2Tail(&warningSiMsg->segLstCp, &pduNode[j].lnk);
               pduNode[j].lnk.node = (PTR)&pduNode[j];
               j++;
               node = node->next;
            }

            /* ccpu00132385-  nodes in received SI config linked list should 
             * be freed after processing the config.*/
            while(warningSiCfgReqInfo->siPduLst.first != NULLP)
            {
               node = warningSiCfgReqInfo->siPduLst.first;
               cmLListDelFrm(&(warningSiCfgReqInfo->siPduLst), node);
               SCH_FREE(node,sizeof(CmLList));
               node = NULLP;
            } 

            cell->siCb.warningSi[idx].siId = warningSiCfgReqInfo->siId;
            cell->siCb.warningSi[idx].idx = idx;
#ifdef EMTC_ENABLE
            if(TRUE == isEmtc)
            {
               rgSCHEmtcWarningSiCfg(cell,warningSiCfgReqInfo,idx);
            }
            else
#endif
            {
               cell->siCb.siArray[warningSiCfgReqInfo->siId-1].si = 
                  &cell->siCb.warningSi[idx];
               cell->siCb.siArray[warningSiCfgReqInfo->siId-1].isWarningSi =
                  TRUE;
            }
            freeNodeFound = TRUE;
            break;
         }
      }
   }

   if (freeNodeFound == FALSE)
   {
      DU_LOG("\nDEBUG  -->  SCH : No SI Index is free");
      rgSCHUtlFreeWarningSiSeg(reg, pool, &warningSiCfgReqInfo->siPduLst);
      SCH_FREE(warningSiCfgReqInfo, 
            sizeof(RgrWarningSiCfgReqInfo)); 
      warningSiCfgReqInfo = NULLP;
      rgSCHUtlRgrWarningSiCfgCfm(instCb->rgSchInit.inst, spId, siId, transId, 
            cfmStatus); 
      return RFAILED;
   }

   SCH_FREE(warningSiCfgReqInfo, 
         sizeof(RgrWarningSiCfgReqInfo)); 
   warningSiCfgReqInfo = NULLP;
   return ROK;
}


/**
 * @brief Handler to handle SI Stop request from RRM to MAC.
 *
 * @details
 *
 *     Function: rgSCHGomHndlWarningSiStopReq
 *     
 *     This API handles processing for SI stop request from RRM to MAC.
 *     
 *  @param[in]  Region        reg
 *  @param[in]  Pool          pool
 *  @param[in]  RgSchCb       *instCb
 *  @param[in]  SpId          siId
 *  @return  void
 **/
Void rgSCHGomHndlWarningSiStopReq(Region reg,Pool pool,RgSchCb *instCb,uint8_t siId,RgrCfgTransId transId,SpId spId)
{
   RgSchCellCb        *cell = instCb->rgrSap[spId].cell;
   uint16_t           idx;
   CmLList            *node;
   RgSchWarningSiPdu  *warningSiPdu;
   Buffer             *pdu;

   for(idx = 0; idx < RGR_MAX_NUM_WARNING_SI; idx++)
   {
      if(cell->siCb.warningSi[idx].siId == siId)
      {
         if ((cmMemcmp ((uint8_t *)&cell->siCb.warningSi[idx].warningSiMsg.transId, 
                     (uint8_t *)&transId, sizeof(RgrCfgTransId))) == 0)
         {
            /* ccpu00136659: CMAS ETWS design changes */
            CM_LLIST_FIRST_NODE(&cell->siCb.warningSi[idx].warningSiMsg.segLstCp, node);
            while(node != NULLP)
            {
               /* On receiving the warning stop message, remove one by one
                * each PDU from the warning SI list
                */  
               /* ccpu00136659: CMAS ETWS design changes */
               node = (CmLList *)&cell->siCb.warningSi[idx].warningSiMsg.segLstCp.first;
               warningSiPdu = (RgSchWarningSiPdu *)node->node;
               pdu = warningSiPdu->pdu;
               cmLListDelFrm(&cell->siCb.warningSi[idx].warningSiMsg.segLstCp, node);
               RGSCH_FREE_MSG(pdu);
               node = node->next;
            }
         }
      }
   }
   return;
}

#endif/*RGR_SI_SCH */

/* LTE_ADV_FLAG_REMOVED_START */

/**
 * @brief This function sets the Phigh range for CC users corresponding to the CC Pool
 * @details
 *
 *     Function: rgSchUpdtRNTPInfo
 *
 *     Invoked by: rgSCHGomHndlLoadInf
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSubFrm*     subFrm
 *  @param[in]  RgrLoadInfReqInfo   *loadInfReq
 *  @return  S16
 *
 **/
S16 rgSchUpdtRNTPInfo(RgSchCellCb *cell,RgSchDlSf *sf,RgrLoadInfReqInfo *loadInfReq)
{
   /*  Initialise the variables */
   RgSchSFRPoolInfo *sfrCCPool;
   CmLListCp   *l;
   CmLList     *n;
   S16 ret = RFAILED;

   l = &sf->sfrTotalPoolInfo.ccPool;

   /*Get the first node from the CC Pool*/
   n = cmLListFirst(l);
   while(n)
   {
      sfrCCPool = (RgSchSFRPoolInfo*)n->node;
      if (sfrCCPool->poolendRB == loadInfReq->rgrCcPHighEndRb)
      {
         sfrCCPool->pwrHiCCRange.endRb   = loadInfReq->rgrCcPHighEndRb;
         sfrCCPool->pwrHiCCRange.startRb = loadInfReq->rgrCcPHighStartRb;
         return ROK;
      }
      else
      {
         n = cmLListNext(l);
      }
   }
   return (ret);
}
/**
 * @brief Handler to handle LOAD INF request from RRM to MAC.
 *
 * @details
 *
 *     Function: rgSCHGomHndlLoadInf
 *
 *     This API handles processing for LOAD INF request from RRM to MAC.
 *
 *     - Processing Steps:
 *        - Validate LOAD INF request parameters at CFG module.
 *          Call rgSCHCfgVldtRgrLoadInf for SI configuration.
 *        - If validated successfully, send configuration request.
 *
 *  @param[in]  Region            reg
 *  @param[in]  Pool              pool
 *  @param[in]  RgSchCb           *instCb
 *  @param[in]  SpId              spId
 *  @param[in]  RgrCfgTransId     transId
 *  @param[in]  RgrLoadInfReqInfo *loadInfReq
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHGomHndlLoadInf(Region reg,Pool pool,RgSchCb *instCb,SpId spId,RgrCfgTransId transId,RgrLoadInfReqInfo *loadInfReq)
{
   S16          ret;
   RgSchCellCb  *cell = instCb->rgrSap[spId].cell;
   Inst         inst  = (instCb->rgSchInit.inst );
   RgSchErrInfo errInfo;
   uint16_t i;

   /* check if cell does not exists */
   if (((uint8_t *)cell == NULLP) || (cell->cellId != loadInfReq->cellId))
   {
      DU_LOG("\nERROR  -->  SCH : Cell Control block does not exist"
             "for load cellId:%d",loadInfReq->cellId);
      SCH_FREE(loadInfReq, (Size)sizeof(*loadInfReq));
      return RFAILED;
   }

   if (cell->lteAdvCb.dsfrCfg.status == RGR_DISABLE)
   {
      DU_LOG("\nERROR  -->  SCH : rgSCHGomHndlLoadInf(): DSFR Feature not enabled");
      SCH_FREE(loadInfReq, (Size)sizeof(*loadInfReq));
      return RFAILED;
   }
   /* Validate the received LOAD INF Configuration */
   ret = rgSCHCfgVldtRgrLoadInf(inst, loadInfReq, cell, &errInfo);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Rgr LOAD INF Configuration "
               "validation FAILED");
      SCH_FREE(loadInfReq, (Size)sizeof(*loadInfReq));
      return RFAILED;
   }
   /* Update the RNTP info rcvd in the respective cell centre pool so that Phigh can be
      sent for the UEs scheduled in that particular RB range*/
   for(i = 0; i < RGSCH_NUM_DL_slotS; i++)
   {
      if((rgSchUpdtRNTPInfo(cell, cell->subFrms[i], loadInfReq) != ROK))
      {
         return RFAILED;
      }
   }

   SCH_FREE(loadInfReq, (Size)sizeof(*loadInfReq));


   return ROK;
}  /* rgSCHGomHndlLoadInf */
/* LTE_ADV_FLAG_REMOVED_END */

/**********************************************************************
 
         End of file
**********************************************************************/
