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
  
     File:     rg_sch.c
  
**********************************************************************/

/** @file rg_sch.c
@brief This file implements the schedulers main access to MAC layer code.
*/


/* header include files -- defines (.h) */
#include "common_def.h"
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rgm.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch.h"
#include "rg_sch_err.h"
#include "rg_sch_cmn.h"
#include "sch_utils.h"

/* header/extern include files (.x) */
#include "tfu.x"           /* TFU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"         /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */
#include "rg_sch_cmn.x"
#include "mac_sch_interface.h"

#ifdef EMTC_ENABLE
S16 rgEmtcMacSchUeDelInd ARGS((RgSchCellCb *cell,RgInfUeDelInd *ueDelInd));
S16 rgSCHGomEmtcHndlSiCfg ARGS((
Region        reg,
Pool          pool,
RgSchCb       *instCb,
SpId          spId,
RgrCfgTransId transId,
RgrSiCfgReqInfo *cfgReqInfo
));
S16 rgSCHGomEmtcHndlWarningSiCfg ARGS((
Region        reg,
Pool          pool,
RgSchCb       *instCb,
SpId          spId,
RgrCfgTransId transId,
RgrWarningSiCfgReqInfo *warningSiCfgReqInfo
));
#endif
/* local defines */
/************** LRG Interface ****************/

/**
 * @brief Layer Manager Control request handler. 
 *
 * @details
 *
 *     Function : RgMiLrgSchCntrlReq
 *     
 *     This function handles the control
 *     request received from the Layer Manager.
 *      -# Based on cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action
 *      and cntrl->t.cntrl.subAction, it performs the appropriate control action
 *      of SAP (enable/disable) and layer shutdown.
 *      -# Invokes the RgMiLrgSchCntrlCfm to send back the confirmation to LM.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cntrl, the control parameter's structure
 *  @return  S16
 *      -# ROK
 **/
S16 RgMiLrgSchCntrlReq
(
Pst      *pst,    /* post structure  */
RgMngmt  *cntrl   /* control structure  */
)
{
   S16       ret = ROK;            /* return value */
   Pst       cfmPst;
   RgMngmt   cfm;
   
   Inst      inst = (pst->dstInst - SCH_INST_START); /* Scheduler instance Id */

   /* Fill the post structure for sending the confirmation */
   SchFillCfmPst(pst, &cfmPst, cntrl);

   /* Initialize the cfg cfm structure 
   SCH_ALLOC(cfm, sizeof(RgMngmt));
   if(cfm   == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : Memory Unavailable for Confirmation");
      SCH_FREE(pst->region, pst->pool, cntrl, sizeof(RgMngmt));
      return ROK;
   } */
   memset(&cfm, 0, sizeof(RgMngmt));

#ifdef LMINT3
   cfm.hdr.transId =
      cntrl->hdr.transId;
#endif

   cfm.hdr.elmId.elmnt = cntrl->hdr.elmId.elmnt;
   cfm.t.cntrl.action = cntrl->t.cntrl.action;
   cfm.t.cntrl.subAction = cntrl->t.cntrl.subAction;

   /* Check if General Config Done*/
   if(rgSchCb[inst].rgSchInit.cfgDone != TRUE)
   {
      cfm.cfm.status = LCM_PRIM_NOK;
      cfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      cfm.hdr.elmId.elmnt = cntrl->hdr.elmId.elmnt;
      RgMiLrgSchCntrlCfm(&cfmPst, &cfm);
      DU_LOG("\nERROR  -->  SCH : Gen Cfg not done.");
      /*      SCH_FREE(pst->region, pst->pool, cntrl, sizeof(RgMngmt)); */
      return ROK;
   }

   /* General Config done, process the Control request */   
   switch(cntrl->hdr.elmId.elmnt)
   {
      case STGEN:
         rgSCHLmmGenCntrl(cntrl, &cfm, &cfmPst);
         break;
      case STTFUSAP:
      case STRGRSAP:
         rgSCHLmmSapCntrl(cntrl, &cfm, &cfmPst);
         break;
      default:
         cfm.cfm.status = LCM_PRIM_NOK;
         cfm.cfm.reason = LCM_REASON_INVALID_PAR_VAL;
         RgMiLrgSchCntrlCfm(&cfmPst, &cfm);
         DU_LOG("\nERROR  -->  SCH : invalid elmnt=%d",
                  cntrl->hdr.elmId.elmnt);
         break;
   }
   /*   SCH_FREE(pst->region, pst->pool, cntrl, sizeof(RgMngmt)); */
   return (ret);
}/*-- RgMiLrgSchCntrlReq --*/

#ifdef LTE_L2_MEAS
/**
 * @brief Layer Manager L2 Measurement request handler. 
 *
 * @details
 *
 *     Function : RgMiLrgSchL2MeasReq
 *     
 *     This function handles the control
 *     measurement request received from the Layer Manager.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  LrgSchMeasReqInfo *measInfo, measurement request info
 *  @return  S16
 *      -# ROK
 **/
S16 RgMiLrgSchL2MeasReq
(
Pst               *pst,     /* post structure  */
LrgSchMeasReqInfo *measInfo /* Meas Req Info */
)
{
   Pst                 cfmPst;
   RgSchCellCb         *cell;
   RgSchErrInfo        err;
   S16                 ret = ROK; 
   RgSchCb             *instCb =  &rgSchCb[(pst->dstInst - SCH_INST_START)];
#if (ERRCLASS & ERRCLS_ADD_RES) 
   CmLList             *lnk;
#endif
   uint32_t            idx;
   RgSchL2MeasCb       *measCb = NULLP;
#ifdef DEBUGP
   Inst      inst = (pst->dstInst - SCH_INST_START); /* Scheduler instance Id */
#endif

   err.errType  = 0;
   err.errCause = 0;


   /* Find the cellCb using cellId in measInfo. Iterate through all cells
    * in rgrsapCb in RgschCb */
   cell = NULLP;
   for (idx = 0; idx < instCb->numSaps; idx++)
   {
      if ( instCb->rgrSap[idx].cell->cellId == measInfo->cellId)
      {
         /* got the cell break the loop */
         cell = instCb->rgrSap[idx].cell;
         break;
      }
   }
   /* If no cellCb return Err with Invalid Cell Id */
   if (cell == NULLP)
   {
      rgSchL2mFillCfmPst(pst, &cfmPst, measInfo);
      RGSCHFILLERR(err, RGSCHERR_L2M_MEASREQ, RGSCHERR_SCH_INVALID_CELLID);
      rgSchL2mSndCfm(&cfmPst, NULLP, measInfo, TRUE);
      DU_LOG("\nERROR  -->  SCH : Meas req Failed.Invalid Cell Id errType(%d) errCause(%d)",
               err.errType, err.errCause);
      return RFAILED;
   }
   /* Validate for Meas Types */
   if ( (ret = rgSCHUtlValidateMeasReq(cell, measInfo, &err)) != ROK)
   {
      rgSchL2mFillCfmPst(pst, &cfmPst, measInfo);
      RGSCHFILLERR(err, RGSCHERR_L2M_MEASREQ, RGSCHERR_SCH_INVALID_MEASTYPE);
      rgSchL2mSndCfm(&cfmPst, NULLP, measInfo, TRUE);
      DU_LOG("\nERROR  -->  SCH : Meas req Failed.Invalid Measurement Type"
               "errCasue(%d) errType(%d)", err.errType, err.errCause);
      return RFAILED;
   }
   /* Iterate through all meas requests in l2mList in cellCb */
#if (ERRCLASS & ERRCLS_ADD_RES) 
   lnk = cell->l2mList.first;
   while(lnk != NULLP)
   {
      /* Get the MeasCb : RgSchL2MeasCb */
      measCb = (RgSchL2MeasCb *)lnk->node;
      lnk = lnk->next;
      if (measCb->measReq.hdr.transId == measInfo->hdr.transId)
      {
         rgSchL2mFillCfmPst(pst, &cfmPst, measInfo);
         RGSCHFILLERR(err, RGSCHERR_L2M_MEASREQ, RGSCHERR_SCH_DUP_TRANSID);
         rgSchL2mSndCfm(&cfmPst, measCb, measInfo, TRUE);
         DU_LOG("\nERROR  -->  SCH : Meas req Failed Duplicate TransId"
                  "errType(%d) errCause(%d)", err.errType, err.errCause);
         return RFAILED;
      }
   }
#endif
   /* Call L2M Function to store Meas req */
   ret = rgSchL2mMeasReq(cell, measInfo, err);
   if (ret != ROK)
   {
      rgSchL2mFillCfmPst(pst, &cfmPst, measInfo);
      RGSCHFILLERR(err, RGSCHERR_L2M_MEASREQ, RGSCHERR_SCH_L2MEAS_FAILED);
      rgSchL2mSndCfm(&cfmPst, measCb, measInfo, TRUE);
      DU_LOG("\nERROR  -->  SCH : Meas req Failed.errType(%d) errCause(%d)",
               err.errType, err.errCause);
      return RFAILED;
   }
   return (ret);
} /* -- RRgMiLrgSchL2MeasReq-- */

/**
 * @brief Layer Manager L2 Measurement Stop request handler. 
 *
 * @details
 *
 *     Function : RgMiLrgSchL2MeasStopReq
 *     
 *     This function handles the control
 *     measurement stop request received from the Layer Manager.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  LrgSchMeasReqInfo *measInfo, measurement stop request info
 *  @return  S16
 *      -# ROK
 **/
S16 RgMiLrgSchL2MeasStopReq
(
Pst               *pst,     /* post structure  */
LrgSchMeasStopReqInfo *measInfo /* Meas Req Info */
)
{
   S16               ret = ROK;
   RgSchCellCb       *cell = NULLP;
   RgSchCb           *instCb =  &rgSchCb[(pst->dstInst - SCH_INST_START)];
   CmLList           *node = NULLP;
   RgSchL2MeasCb     *measCb = NULLP;
   LrgSchMeasCfmInfo measCfm;
   uint8_t           idx;

   for (idx = 0; idx < instCb->numSaps; idx++)
   {
      if ( instCb->rgrSap[idx].cell->cellId == measInfo->cellId)
      {
         /* got the cell break the loop */
         cell = instCb->rgrSap[idx].cell;
         break;
      }
   }
   if (cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : Stop req Failed.Invalid Cell Id ");
      return RFAILED;
   }
   memset(&measCfm, 0, sizeof(LrgSchMeasCfmInfo));
   node = cell->l2mList.first;
   while(node != NULLP)
   {
      measCb = (RgSchL2MeasCb *)(node)->node;

      node = (node)->next;
      cmLListDelFrm(&cell->l2mList, &measCb->measLnk);
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)&measCb,
            sizeof(RgSchL2MeasCb));    
   }

   if(measInfo->measType & LRG_L2MEAS_AVG_PRB_PER_QCI_UL)
   {
      RgInfL2MeasStopReq measStopReq;
      Pst               pstMac;
      memset(&measStopReq, 0, sizeof(RgInfL2MeasStopReq));
      measStopReq.transId  = measInfo->hdr.transId;
      measStopReq.measType = measInfo->measType;
      /* measReq.timePrd  = measInfo->timePrd; */
      measStopReq.cellId   = measInfo->cellId;
      rgSCHUtlGetPstToLyr(&pstMac, &rgSchCb[cell->instIdx], cell->macInst);
      RgSchMacL2MeasStop(&pstMac,&measStopReq);
   }
   else
   { 
      RgMiLrgSchL2MeasStopCfm(&(rgSchCb[cell->instIdx].rgSchInit.lmPst),
            &measCfm);
   }
   return (ret);
}/*RgMiLrgSchL2MeasStopReq*/
/**
 * @brief Layer Manager L2 Measurement request handler. 
 *        for Send l2 measurement req
 * @details
 *
 *     Function : RgMiLrgSchL2MeasSendReq
 *     
 *     This function handles the control
 *     measurement send request received from the Layer Manager.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  LrgSchMeasReqInfo *measInfo, measurement stop request info
 *  @return  S16
 *      -# ROK
 **/
S16 RgMiLrgSchL2MeasSendReq
(
Pst               *pst,     /* post structure  */
LrgSchMeasSndReqInfo *measInfo /* Meas Req Info */
)
{
   S16                 ret = ROK;
   RgSchCellCb         *cell;
   RgSchCb             *instCb =  &rgSchCb[(pst->dstInst - SCH_INST_START)];
   uint8_t             idx;

   cell = NULLP;
   for (idx = 0; idx < instCb->numSaps; idx++)
   {
      if ( instCb->rgrSap[idx].cell->cellId == measInfo->cellId)
      {
         /* got the cell break the loop */
         cell = instCb->rgrSap[idx].cell;
         break;
      }
   }
   if (cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : Send req Failed.Invalid Cell Id");
      return RFAILED;
   }

   if(measInfo->measType & LRG_L2MEAS_AVG_PRB_PER_QCI_UL)
   {
      RgInfL2MeasSndReq measSndReq;
      Pst               pstMac;
      memset(&measSndReq, 0, sizeof(RgInfL2MeasSndReq));
      measSndReq.transId  = measInfo->hdr.transId;
      measSndReq.measType = measInfo->measType;
      measSndReq.timePrd  = measInfo->timePrd;
      measSndReq.cellId   = measInfo->cellId;
      rgSCHUtlGetPstToLyr(&pstMac, &rgSchCb[cell->instIdx], cell->macInst);
      RgSchMacL2MeasSend(&pstMac, &measSndReq);
   }
   else
   { 
      cell->sndL2Meas = TRUE;
   }

   return (ret);
}/*RgMiLrgSchL2MeasSendReq*/
#endif /* LTE_L2_MEAS */




/************* RGR  Interface ****************/
/**
 * @brief API for bind request from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgrBndReq
 *     
 *     This API is invoked by RRM towards MAC to bind RGR SAP. 
 *     These API validates the Pst, spId, suId and sends the bind confirm to
 *     RRM.
 *
 *           
 *  @param[in]  Pst   *pst
 *  @param[in]  SuId  suId
 *  @param[in]  SpId  spId
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgUiRgrBndReq(Pst   *pst, SuId  suId, SpId  spId)
{
   S16       ret = ROK;
   Pst       tmpPst;   /* Temporary Post Structure */
   Inst      instId = pst->dstInst-SCH_INST_START;
   RgUstaDgn dgn;      /* Alarm diagnostics structure */

   tmpPst.prior       = pst->prior;
   tmpPst.route       = pst->route;
   tmpPst.selector    = pst->selector;
   tmpPst.region      = rgSchCb[instId].rgSchInit.region;
   tmpPst.pool        = rgSchCb[instId].rgSchInit.pool;
   tmpPst.srcProcId   = rgSchCb[instId].rgSchInit.procId;
   tmpPst.srcEnt      = rgSchCb[instId].rgSchInit.ent;
   tmpPst.srcInst     = rgSchCb[instId].rgSchInit.inst + SCH_INST_START;
   tmpPst.event       = EVTNONE;
   tmpPst.dstProcId   = pst->srcProcId;
   tmpPst.dstEnt      = pst->srcEnt;
   tmpPst.dstInst     = pst->srcInst;

   if(spId < rgSchCb[instId].numSaps)
   {
      /* Check the state of the SAP */
      switch (rgSchCb[instId].rgrSap[spId].sapSta.sapState)
      {
         /* This case might not be needed if SAP not configured then it will go
          * to else of above if condition */
         case LRG_UNBND: /* SAP is not bound */
            DU_LOG("\nDEBUG  -->  SCH : SAP Not yet bound");
            rgSchCb[instId].rgrSap[spId].sapSta.sapState = LRG_BND;
            rgSchCb[instId].rgrSap[spId].sapCfg.suId = suId;
            /* Send Bind Confirm with status as SUCCESS */
            ret = rgSCHUtlRgrBndCfm(instId, suId, CM_BND_OK);
            /*Indicate to Layer manager  */
            rgSCHUtlFillDgnParams(instId, &dgn, LRG_USTA_DGNVAL_MEM); 
            ret = rgSCHLmmStaInd(instId, LCM_CATEGORY_INTERFACE,
                  LRG_EVENT_RGRSAP_ENB, LCM_CAUSE_UNKNOWN, &dgn);
            break;
         case LRG_BND: /* SAP is already bound*/
            DU_LOG("\nDEBUG  -->  SCH : SAP is already bound");
            ret = rgSCHUtlRgrBndCfm(instId, suId, CM_BND_OK);
            break;
         default: /* Should Never Enter here */
#if (ERRCLASS & ERRCLS_ADD_RES) 
            DU_LOG("\nERROR  -->  SCH : Invalid SAP State:RgUiRrgBndReq failed\n");
#endif
            ret = rgSCHUtlRgrBndCfm(instId, suId, CM_BND_NOK);
            break;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      /* ccpu00117035 - MOD - Changed ErrVal argument from accessing sap state 
         to spId to avoid seg fault due to invalid sapID */
          DU_LOG("\nERROR  -->  SCH : Invalid SAP Id:RgUiRrgBndReq failed\n");
#endif
      ret = RgUiRgrBndCfm(&tmpPst, suId, CM_BND_NOK);
   }
   return (ret);
}  /* RgUiRgrBndReq */

/**
 * @brief API for unbind request from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgrUbndReq
 *     
 *     This API is invoked by RRM towards MAC to unbind RGR SAP. 
 *     These API validates the Pst, spId, suId and transfers the unbind request 
 *     specific information to corresponding ownership module (GOM) API.
 *
 *           
 *  @param[in]  Pst    *pst
 *  @param[in]  SuId   suId
 *  @param[in]  Reason reason
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgUiRgrUbndReq(Pst *pst,SpId spId,Reason reason)
{
   Inst instId = pst->dstInst-SCH_INST_START;

   /* SAP Id validation */
   if (spId < rgSchCb[instId].numSaps)
   {
      switch(rgSchCb[instId].rgrSap[spId].sapSta.sapState)
      {
         case LRG_BND: /* SAP is already bound*/
            DU_LOG("\nDEBUG  -->  SCH : SAP is already bound");
            /* setting SAP state to UN BOUND */
            rgSchCb[instId].rgrSap[spId].sapSta.sapState = LRG_UNBND;
            break;
         default:
#if (ERRCLASS & ERRCLS_ADD_RES)      
            DU_LOG("\nERROR  -->  SCH : Invalid SAP State RgUiRgrUbndReq failed\n");
#endif
            return RFAILED;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      DU_LOG("\nERROR  -->  SCH : Invalid SAP Id RgUiRgrUbndReq failed\n");
#endif
      return RFAILED;
   }
   return ROK;
}  /* RgUiRgrUbndReq */

#ifdef RGR_SI_SCH
/**
 * @brief API for SI configuration request from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgrSiCfgReq
 *     
 *     This API is invoked by RRM towards MAC to configure SI at MAC. 
 *     These API validates the Pst, spId, suId and transfers the 
 *     config request specific information to corresponding ownership 
 *     module (GOM) API.
 *
 *           
 *  @param[in]  Pst           *pst
 *  @param[in]  SpId          spId
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  RgrSiCfgReqInfo *cfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgUiRgrSiCfgReq(Pst *pst, SpId  spId,RgrCfgTransId transId,RgrSiCfgReqInfo *cfgReqInfo)
{
   S16       ret       = ROK;
   uint8_t   cfmStatus = RGR_CFG_CFM_NOK;
   uint8_t   prntTrans[RGR_CFG_TRANSID_SIZE+1];
   Inst      instId = pst->dstInst-SCH_INST_START;

   memcpy(prntTrans, transId.trans, RGR_CFG_TRANSID_SIZE);
   prntTrans[RGR_CFG_TRANSID_SIZE] = '\0';


   if (cfgReqInfo == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : Input Message Buffer is NULL");
      rgSCHUtlRgrSiCfgCfm(instId, spId, transId, cfmStatus); 
      return RFAILED;
   }

   if (spId < rgSchCb[instId].numSaps)
   {
      if(LRG_BND != rgSchCb[instId].rgrSap[spId].sapSta.sapState)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         DU_LOG("\nERROR  -->  SCH : Invalid SAP State: RgUiRgrSiCfgReq failed\n");
#endif
         SCH_FREE(cfgReqInfo, (Size)sizeof(*cfgReqInfo));
         rgSCHUtlRgrSiCfgCfm(instId, spId, transId, cfmStatus); 
         return RFAILED;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      DU_LOG("\nERROR  -->  SCH : Invalid SAP Id:RgUiRgrSiCfgReq failed\n");
#endif
      SCH_FREE(cfgReqInfo, (Size)sizeof(*cfgReqInfo));
      rgSCHUtlRgrSiCfgCfm(instId, spId, transId, cfmStatus); 
      return RFAILED;
   }

   /* Handle configuration */
#ifdef EMTC_ENABLE
if(rgSchCb[instId].rgrSap[spId].cell->emtcEnable)
{
   ret = rgSCHGomEmtcHndlSiCfg(pst->region, pst->pool,
            &rgSchCb[instId], spId, transId, 
                        cfgReqInfo);
}
else
{
   ret = rgSCHGomHndlSiCfg(pst->region, pst->pool,
            &rgSchCb[instId], spId, transId, 
                        cfgReqInfo);
}
 #else
   ret = rgSCHGomHndlSiCfg(pst->region, pst->pool,
            &rgSchCb[instId], spId, transId, 
                        cfgReqInfo);
 #endif
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : RgUiRgrSiCfgReq:"
                   "Configuration Request Handling Failed");
      return RFAILED;
   }

   return ROK;
}  /* RgUiRgrSiCfgReq */


/**
 * @brief API for Warning SI configuration request from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgrWarningSiCfgReq
 *     
 *     This API is invoked by RRM towards MAC to configure SI at MAC. 
 *     These API validates the Pst, spId, suId and transfers the 
 *     config request specific information to corresponding ownership 
 *     module (GOM) API.
 *
 *           
 *  @param[in]  Pst           *pst
 *  @param[in]  SpId          spId
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  RgrWarningSiCfgReqInfo *warningSiCfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgUiRgrWarningSiCfgReq(Pst *pst, SpId spId,RgrCfgTransId transId,RgrWarningSiCfgReqInfo *warningSiCfgReqInfo)
{
   Inst     instId = pst->dstInst-SCH_INST_START;
   S16      ret    = ROK;
   uint8_t  cfmStatus = RGR_CFG_CFM_NOK;
   uint8_t  prntTrans[RGR_CFG_TRANSID_SIZE+1];

   memcpy(prntTrans, transId.trans, RGR_CFG_TRANSID_SIZE);
   prntTrans[RGR_CFG_TRANSID_SIZE] = '\0';

   if (warningSiCfgReqInfo == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : Input Message Buffer "
               "is NULL");
      rgSCHUtlRgrWarningSiCfgCfm(instId, spId, 0, transId, cfmStatus);
      return RFAILED;
   }

   if (spId < rgSchCb[instId].numSaps)
   {
      if(LRG_BND != rgSchCb[instId].rgrSap[spId].sapSta.sapState)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         DU_LOG("\nERROR  -->  SCH : Invalid SAP State: warningSiCfgReqInfo failed\n");
#endif
         rgSCHUtlFreeWarningSiSeg(pst->region, pst->pool, 
               &warningSiCfgReqInfo->siPduLst);
         SCH_FREE(warningSiCfgReqInfo, sizeof(RgrWarningSiCfgReqInfo));
         rgSCHUtlRgrWarningSiCfgCfm(instId, spId, warningSiCfgReqInfo->siId, 
               transId, cfmStatus);
         return RFAILED;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
       DU_LOG("\nERROR  -->  SCH : Invalid SAP Id:warningSiCfgReqInfo failed\n");
#endif
      rgSCHUtlFreeWarningSiSeg(pst->region, pst->pool, 
            &warningSiCfgReqInfo->siPduLst);
      SCH_FREE(warningSiCfgReqInfo,sizeof(RgrWarningSiCfgReqInfo));
      rgSCHUtlRgrWarningSiCfgCfm(instId, spId, warningSiCfgReqInfo->siId, 
            transId, cfmStatus);
      return RFAILED;
   }

   /* Handle configuration */
#ifdef EMTC_ENABLE
if(rgSchCb[instId].rgrSap[spId].cell->emtcEnable)
{
   ret = rgSCHGomEmtcHndlWarningSiCfg(pst->region, pst->pool,
         &rgSchCb[instId], spId, transId, warningSiCfgReqInfo);
}
else
{
   ret = rgSCHGomHndlWarningSiCfg(pst->region, pst->pool,
         &rgSchCb[instId], spId, transId, warningSiCfgReqInfo);
}
#else
   ret = rgSCHGomHndlWarningSiCfg(pst->region, pst->pool,
         &rgSchCb[instId], spId, transId, warningSiCfgReqInfo);
#endif
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Configuration Request Handling Failed");
      return RFAILED;
   }

   return ROK;
}


/**
 * @brief API for Warning SI Stop  request from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgrWarningSiStopReq
 *     
 *     This API is invoked by RRM towards MAC to Stop warning SI at MAC. 
 *     These API validates the Pst, spId, suId and transfers the 
 *     stop request specific information to corresponding ownership 
 *     module (GOM) API.
 *
 *           
 *  @param[in]  Pst           *pst
 *  @param[in]  SpId          spId
 *  @param[in]  RgrWarningSiCfgReqInfo *warningSiCfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgUiRgrWarningSiStopReq(Pst *pst,SpId  spId,RgrCfgTransId transId,uint8_t siId)
{         
   Inst         instId = pst->dstInst-SCH_INST_START;

   if (spId < rgSchCb[instId].numSaps)
   {
      if(LRG_BND != rgSchCb[instId].rgrSap[spId].sapSta.sapState)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         DU_LOG("\nERROR  -->  SCH : Invalid SAP State: RgUiRgrWarningSiStopReq failed\n");
#endif
         return RFAILED;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      DU_LOG("\nERROR  -->  SCH : Invalid SAP Id:RgUiRgrWarningSiStopReq failed\n");
#endif
      return RFAILED;
   }

   rgSCHGomHndlWarningSiStopReq(pst->region, pst->pool,
         &rgSchCb[instId], siId, transId, spId);

   return ROK;
}
#endif /*RGR_SI_SCH */

/* LTE_ADV_FLAG_REMOVED_START */

/**
 * @brief API for LOAD INF request from RRM towards MAC.
 *
 * @details
 *
 *     Function: RgUiRgrLoadInfReq
 *
 *     This API is invoked by RRM towards MAC to configure LOAD INF Parameters at MAC.
 *     These API validates the Pst, spId, suId and transfers the
 *     LOAD INF request to corresponding ownership
 *     module (GOM) API.
 *
 *
 *  @param[in]  Pst               *pst
 *  @param[in]  SpId              spId
 *  @param[in]  RgrCfgTransId     transId
 *  @param[in]  RgrLoadInfReqInfo *loadInfReq
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 RgUiRgrLoadInfReq(Pst *pst, SpId spId, RgrCfgTransId  transId,RgrLoadInfReqInfo *loadInfReq)
{
   S16       ret       = ROK;
   uint8_t   prntTrans[RGR_CFG_TRANSID_SIZE+1];
   Inst      instId = pst->dstInst-SCH_INST_START;

   memcpy(prntTrans, transId.trans, RGR_CFG_TRANSID_SIZE);
   prntTrans[RGR_CFG_TRANSID_SIZE] = '\0';


   if (loadInfReq == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : Input Message Buffer "
               "is NULL");
      return RFAILED;
   }

   if (spId < rgSchCb[instId].numSaps)
   {
      if(LRG_BND != rgSchCb[instId].rgrSap[spId].sapSta.sapState)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
         DU_LOG("\nERROR  -->  SCH : Invalid SAP State: RgUiRgrLoadInfReq failed\n");
#endif
         SCH_FREE(loadInfReq, (Size)sizeof(*loadInfReq));
         return RFAILED;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      DU_LOG("\nERROR  -->  SCH : Invalid SAP Id:RgUiRgrLoadInfReq failed\n");
#endif
      SCH_FREE(loadInfReq, (Size)sizeof(*loadInfReq));
      return RFAILED;
   }

   /* Handle configuration */
   ret = rgSCHGomHndlLoadInf(pst->region, pst->pool,
         &rgSchCb[(pst->dstInst - SCH_INST_START)], spId, transId, 
         loadInfReq);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Configuration Request Handling Failed");
      return RFAILED;
   }

   return ROK;
}  /* RgUiRgrLoadInfReq */
/* LTE_ADV_FLAG_REMOVED_END */

/************** MAC SCH Interface **************/ 
/**
 * @brief Function for updating dedicated channel BO at scheduler from MAC.
 *
 * @details
 *
 *     Function : rgMacSchDedBoUpdtReq
 *     
 *     This function shall be invoked whenever MAC gets staRsp from RLC
 *     Scheduler shall update BO in its dedicated logical channel control block.
 *           
 *  @param[in] Pst*           pst
 *  @param[in] S16            cellSapId
 *  @param[in] RgInfDedBoRpt  *boRpt
 *  @return  S16
 *      -# ROK 
 **/
S16 RgMacSchDedBoUpdtReq(Pst*  pst,RgInfDedBoRpt  *boRpt)
{
   RgSchCellCb   *cell;
   RgSchUeCb     *ue;
#ifdef SCH_STATS
   RgSchCmnDlUe  *dlUe;
#endif

   Inst          inst = (pst->dstInst - SCH_INST_START);
   S16           cellSapId = boRpt->cellSapId;
/*
   DU_LOG("\nDEBUG  -->  SCH : rgMacSchDedBoUpdtReq():"
            " boRpt->rnti = %u  boRpt->lcId = %u",boRpt->rnti, boRpt->lcId);
*/
   /* No need to chk for cell being NULL as MAC wouldn't have found instance if
    * it doesnt exist */
   cell = rgSchCb[inst].rgrSap[cellSapId].cell;

#ifndef NO_ERRCLS
   if (cell->cellId != boRpt->cellId)
   {
      /* Handle Cell fetch failure */
      DU_LOG("\nERROR  -->  SCH : rgMacSchDedBoUpdtReq(): Invalid cell Id");
      return RFAILED;
   }
#endif

   /* Update Bo in the given logical channel of the UE */
   if ((ue = rgSCHDbmGetUeCb(cell, boRpt->rnti)) != NULLP)
   {
      RgSchDlLcCb   *dlLc;
      /* Honor BO Reports only from PCELL */
#ifdef LTE_ADV
      if (cell != ue->cell)
      {
         return RFAILED;
      }
#endif
      if ((dlLc = rgSCHDbmGetDlDedLcCb(ue, boRpt->lcId)) != NULLP)
      {
#ifdef LTE_L2_MEAS
         if(dlLc->lcType == CM_LTE_LCH_DTCH)
         {
            if((dlLc->bo == 0) &&(boRpt->bo != 0))
            {
               /* UE is active */
               if(!(ue->qciActiveLCs[dlLc->qciCb->qci]))
               {
                  dlLc->qciCb->dlUeCount++;
               }
               ue->qciActiveLCs[dlLc->qciCb->qci]++;
            }
            else if((dlLc->bo != 0) && (boRpt->bo == 0) && (dlLc->qciCb->dlUeCount))
            {
               /* UE is inactive */
               if (ue->qciActiveLCs[dlLc->qciCb->qci])
               {
                  ue->qciActiveLCs[dlLc->qciCb->qci]--;
                  if (!(ue->qciActiveLCs[dlLc->qciCb->qci]))
                  {
                     dlLc->qciCb->dlUeCount--;
                  }
               }
            }
        }
#endif
#ifdef SCH_STATS
         dlUe = RG_SCH_CMN_GET_DL_UE(ue, cell);
         if (boRpt->bo > dlLc->bo)
         {
            dlUe->boReported += (boRpt->bo - dlLc->bo);
         }
#endif
#ifdef TENB_STATS
         if (boRpt->bo > dlLc->bo)
         {
            ue->tenbStats->stats.nonPersistent.sch[RG_SCH_CELLINDEX(cell)].dlBo += ((boRpt->bo - dlLc->bo)<<3);
         }
#endif
         /* RLC provides cumulative BO for each LC.
          * Reduce the left out unscheduled bo from total bo and 
          * update the new BO to the total bo */
         if(ue->totalBo >= dlLc->bo)
         {
            ue->totalBo -= dlLc->bo;
         }
         else
         {
            ue->totalBo = 0; /* this case should not arise
                             * Resetting for safety */
         }

         ue->totalBo +=  boRpt->bo;
         dlLc->bo = boRpt->bo;
         dlLc->oldestSduArrTime = boRpt->oldestSduArrTime;
         dlLc->staPduBo = boRpt->staPduBo;
         
         dlLc->setMaxUlPrio = boRpt->setMaxUlPrio;
         dlLc->setMaxDlPrio = boRpt->setMaxDlPrio;
         /* Update the scheduler */
         rgSCHUtlDlDedBoUpd(cell, ue, dlLc);
         return ROK;
      }
   }
   return RFAILED;

} /* end of rgMacSchDedBoUpdtReq */


/**
 * @brief Function for updating common channel BO at scheduler from MAC.
 *
 * @details
 *
 *     Function : RgMacSchCmnBoUpdtReq
 *     
 *     This function shall be invoked whenever MAC gets staRsp from RLC
 *     Scheduler shall update BO in its common logical channel control block.
 *           
 *  @param[in] Pst*            pst
 *  @param[in] S16             cellSapId
 *  @param[in] RgInfCmnBoRpt  *boRpt
 *  @return  S16
 *      -# ROK 
 **/
S16 RgMacSchCmnBoUpdtReq(Pst* pst,RgInfCmnBoRpt  *boRpt)
{
   RgSchCellCb   *cell;
   Inst          inst = (pst->dstInst - SCH_INST_START);
   S16           cellSapId = boRpt->cellSapId;


   /* No need to chk for cell being NULL as MAC would not have found instance if
    * it doesnt exist */
   cell = rgSchCb[inst].rgrSap[cellSapId].cell;
   if (cell->cellId != boRpt->cellId)
   {
      DU_LOG("\nERROR  -->  SCH : RgMacSchCmnBoUpdtReq():"
               "Invalid boRpt cell Id:%d",boRpt->cellId);
      return RFAILED;
   }

   /* handle status response on CCCH */
   if(boRpt->lcId == cell->dlCcchId)
   {
      DU_LOG("\nDEBUG  -->  SCH : RgMacSchCmnBoUpdtReq():"
               " BO update for CCCH");
      rgSCHUtlHndlCcchBoUpdt(cell, boRpt); 
   }
   else
   {
      rgSCHUtlHndlBcchPcchBoUpdt(cell, boRpt); 
   }
   
   return ROK;
} /* end of RgMacSchCmnBoUpdtReq */
/*Fix: start: Inform UE delete to scheduler*/
/**
 * @brief This API is used to send data indication to Scheduler instance from MAC.
 *
 * @details
 *
 *     Function : rgMacSchUeDelInd
 *     
 *     This function shall be invoked whenever MAC gets Ue delete request. 
 *      
 *           
 *  @param[in] Pst*            pst
 *  @param[in] RgInfUeDelInd   *ueDelInd
 *  @return  S16
 *      -# ROK 
 **/
S16 RgMacSchUeDelInd(Pst* pst,RgInfUeDelInd *ueDelInd)
{
   RgSchCellCb       *cell;
   Inst              inst = (pst->dstInst - SCH_INST_START);
   S16               cellSapId = ueDelInd->cellSapId;
   CmLList           *tmp;
   RgSchRntiLnk      *rntiLnk=NULL;
   
   if (rgSchCb[inst].rgrSap == NULLP || rgSchCb[inst].rgrSap[cellSapId].cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : rgrSap or cell is not configured");
      return ROK;
   }
   cell = rgSchCb[inst].rgrSap[cellSapId].cell;
#ifndef NO_ERRCLS
   if (cell->cellId != ueDelInd->cellId)
   {
      /* Handle Cell fetch failure */
      DU_LOG("\nERROR  -->  SCH : rgMacSchUeDelInd(): Invalid ueDelInd cell Id:%d",
		         ueDelInd->cellId);
      return ROK;
   }
#endif
      
   CM_LLIST_FIRST_NODE(&cell->rntiDb.rntiGuardPool, tmp);

   while(tmp)
   {
      rntiLnk = (RgSchRntiLnk *)(tmp->node);
      if(rntiLnk->rnti == ueDelInd->rnti)
      {
         cmLListDelFrm(&cell->rntiDb.rntiGuardPool, tmp);
         tmp->node = NULLP;
         rgSCHDbmRlsRnti(cell, rntiLnk);
         DU_LOG("\nDEBUG  -->  SCH : RNTI:%d Released from the Guard pool(%d)",
		  ueDelInd->rnti, cell->rntiDb.rntiGuardPool.count);
	 
         break;
      }
      CM_LLIST_NEXT_NODE(&cell->rntiDb.rntiGuardPool, tmp);
   }

#ifdef EMTC_ENABLE
 rgEmtcMacSchUeDelInd(cell,ueDelInd);  
#endif

   if(tmp == NULLP)
   {
      /* Fix : syed HO UE does not have a valid ue->rntiLnk */
      DU_LOG("\nINFO  -->  SCH : HO CRNTI:%d not present in the"
           "Guard Pool:%d", ueDelInd->rnti, cell->rntiDb.rntiGuardPool.count);
   } 
   
   return ROK;
}  /* rgMacSchUeDelInd */
/*Fix: end: Inform UE delete to scheduler*/
/**
 * @brief This API is used to send data indication to Scheduler instance from MAC.
 *
 * @details
 *
 *     Function : rgMacSchSfRecpInd
 *     
 *     This function shall be invoked whenever MAC gets datInd on TFU
 *     Scheduler shall act on the CEs and data that are received as part of
 *     datInd.
 *           
 *  @param[in] Pst*            pst
 *  @param[in] S16             cellSapId
 *  @param[in] CmLteRnti       rnti
 *  @param[in] DatIndInfo      *datInd
 *  @return  S16
 *      -# ROK 
 **/
S16 RgMacSchSfRecpInd(Pst*  pst,RgInfSfDatInd *subfrmInfo)
{
   S16               ret = RFAILED;
   RgSchErrInfo      err;
   RgSchUeCb         *ue;
   RgInfUeDatInd     *datInd;
   RgSchCellCb       *cell;
   Inst              inst = (pst->dstInst - SCH_INST_START);
   CmLteRnti         rnti;
   CmLListCp         *lnkLst;
   CmLList           *tmp;
   S16               cellSapId = subfrmInfo->cellSapId;
   RgrUeStaIndInfo   *ueStaInd;
#ifdef RG_UNUSED
//#ifdef LTEMAC_SPS
   RgSchCmnUlUeSpsInfo   *ulSpsUe ;
#endif
   /* RRM_RBC_X */
   uint32_t                   idx;
   /* RRM_RBC_Y */
   
#ifdef LTE_L2_MEAS
   uint8_t               qci;
   uint16_t              datIndActQci = 0;
   uint16_t              combDatIndActQci = 0; /* Prev and Latest Dat Ind combined */
   uint16_t              tempUeActQci = 0; /* UE specific Active QCIs */
   uint16_t              diffBits = 0;
   uint8_t               lcCount;
#endif

   /* No need to chk for cell being NULL as MAC wouldn't have found instance if
    * it doesnt exist */
   cell = rgSchCb[inst].rgrSap[cellSapId].cell;

   /* lnkLst assignment */
   lnkLst = &(subfrmInfo->ueLst);

   CM_LLIST_FIRST_NODE(lnkLst, tmp);

   while((NULLP != tmp) && ((RgInfUeDatInd *)tmp->node != NULLP))
   {
      ue = NULLP;
#ifdef LTE_L2_MEAS
      qci = 0;
#endif
      datInd   = (RgInfUeDatInd *)tmp->node;
      rnti     = datInd->rnti;

      /* We shall try and find
       * out the RaCb based on the following - 
       * 1. If the incoming PDU contained a CCCH SDU i.e. this is message 3.
       * 2. If the incoming PDU contained a CRNTI control element, i.e. we should
       * have a ueCb also for this 
       */
      
      /* It could be that a non-msg3 pdu contains a CRNTI Control element. We
       * should check for CRNTI CE and if it exists the UECb must exist, also an
       * if the CRNTI in the CE and the one with which the message came in are
       * different we shall look for an raCb as well. 
       */
      if (datInd->ceInfo.bitMask & RGSCH_CRNTI_CE_PRSNT)
      {
         /* SR_RACH_STATS : CRNTI CE*/
         rgNumMsg3CrntiCE++;

         if (datInd->ceInfo.bitMask & RGSCH_CCCH_SDU_PRSNT)
         {
            RGSCH_FREE_MEM(subfrmInfo);
            err.errType = RGSCHERR_TOM_DATIND;
            DU_LOG("\nDEBUG  -->  SCH : Received MSG3 with CRNTI:%d and also CCCH ", 
                     datInd->ceInfo.ces.cRnti);
            return RFAILED;
         }
         ue = rgSCHDbmGetUeCb (cell, datInd->ceInfo.ces.cRnti);
         if (ue == NULLP)
         {
            /* SR_RACH_STATS : CRNTI CE UECB NOT FOUND*/
            rgNumCrntiCeCrntiNotFound++;
            /* ccpu00141318 - Removed condition for SPS rnti checking*/
            RGSCH_FREE_MEM(subfrmInfo);
            err.errType = RGSCHERR_TOM_DATIND;
            DU_LOG("\nERROR  -->  SCH : Received MSG3 "
                     "with CRNTI:%d unable to find ueCb", 
                     datInd->ceInfo.ces.cRnti);
            return RFAILED;
         }

         if ((ret = rgSCHUtlProcMsg3 (subfrmInfo, cell, ue, 
               rnti, datInd, &err)) != ROK)
         { 
            RGSCH_FREE_MEM(subfrmInfo);
            err.errType = RGSCHERR_TOM_DATIND;
            DU_LOG("\nERROR  -->  SCH : Processing for MSG3 failed for CRNTI:%d", 
                     datInd->rnti);
            return RFAILED;
         }
         
#ifdef LTEMAC_SPS
         rgSCHUtlHdlCrntiCE (cell, ue);
#endif
         ret = rgSCHUtlAllocSBuf (cell->instIdx,(Data**)&ueStaInd,
                                  sizeof(RgrUeStaIndInfo));
         if(ret == ROK)
         {
            ueStaInd->status = RGR_UESTA_MAC_CRNTI_CE_RECVD;
#ifdef RG_UNUSED
//#ifdef LTEMAC_SPS
            ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
            if(ulSpsUe->isUlSpsActv)
            {
               ueStaInd->status = RGR_UESTA_MAC_CRNTI_CE_RECVD_IN_SPS_ACTIVE;
               ue->ul.ulSpsCfg.isLcSRMaskEnab = FALSE;
            }
#endif
            ret = rgSCHUtlFillSndUeStaInd(cell, ue, ueStaInd);
            if(ret != ROK)
            {
               DU_LOG("\nERROR  -->  SCH : Could not Send Ue Sta Ind UEID:%d",ue->ueId);
            }
         }
         CM_LLIST_NEXT_NODE(lnkLst, tmp);
         continue;
      } /* end of CRNTI based message */
      else if (datInd->ceInfo.bitMask & RGSCH_CCCH_SDU_PRSNT)
      {
         /* SR_RACH_STATS : CCCH SDU */
         rgNumMsg3CCCHSdu++;
         /* SR_RACH_STATS : CCCH SDU RACB NOT FOUND*/
         if (NULLP == rgSCHDbmGetRaCb (cell, rnti))
         {
            rgNumCCCHSduCrntiNotFound++;
         }

         if ((ret = rgSCHUtlProcMsg3 (subfrmInfo, cell, ue, 
               rnti, datInd, &err)) != ROK)
         { 
            RGSCH_FREE_MEM(subfrmInfo);
            err.errType = RGSCHERR_TOM_DATIND;
            DU_LOG("\nERROR  -->  SCH : Processing for MSG3 failed for CRNTI:%d", 
                     datInd->rnti);
            return RFAILED;
         }
         /* fix */
          CM_LLIST_NEXT_NODE(lnkLst, tmp);
          continue;
      } /* end of Msg3 processing */

      if (ue == NULLP)
      {
         ue = rgSCHDbmGetUeCb (cell, datInd->rnti);
         if (ue == NULLP)
         {
#ifdef LTEMAC_SPS 
         if((ue = rgSCHDbmGetSpsUeCb (cell, datInd->rnti)) == NULLP)
#endif
            {
               RGSCH_FREE_MEM(subfrmInfo);
               err.errType = RGSCHERR_TOM_DATIND;
               DU_LOG("\nERROR  -->  SCH : Unable to get the UE CB for CRNTI:%d", 
               datInd->rnti);
               return RFAILED;
            }
         }
      }
/* L2_COUNTERS */
#ifdef LTE_L2_MEAS
      /* The LCs for which data is received at MAC is provided to SCH.
         This information is used to estimate the Active LCs at UE
         since estimates based on only BSR is not accurate */
      if (datInd->ceInfo.bitMask & RGSCH_ACTIVE_LC_PRSNT)
      {
      
        /* Compose a Bitmask with for the QCI's for which Data
           is received */
        for (lcCount = 0; lcCount < RGINF_MAX_NUM_DED_LC; lcCount++)
        {
           if ((datInd->ceInfo.ulActLCs[lcCount] == TRUE) && (TRUE == ue->ul.lcCb[lcCount].isValid))
           {
              datIndActQci |= (1 << (ue->ul.lcCb[lcCount].qciCb->qci -1));
           }
        }
        if (ue->ulActiveLCs && ue->lastDatIndLCs)
        {
           /* Combine previous Dat Ind and current Dat Ind to
              estimate active LCs at UE */
           combDatIndActQci = ue->lastDatIndLCs | datIndActQci;
           tempUeActQci = ue->ulActiveLCs;
           ue->ulActiveLCs = combDatIndActQci;
           diffBits = combDatIndActQci ^ tempUeActQci;
           while (diffBits)
           {
              qci++;
              if (0x1 & diffBits)
              {
                 if (0x1 & tempUeActQci)
                 {
                    /* An active QCI has become inactive */
                    cell->qciArray[qci].ulUeCount--;
                 }
                 else
                 {
                    /* An Inactive QCI has become active */
                    cell->qciArray[qci].ulUeCount++;
                 }
              }
              diffBits >>= 1;
              tempUeActQci >>= 1;
           }
        }
        ue->lastDatIndLCs = datIndActQci;
      
      }

#endif /* LTE_L2_MEAS */
      /* Just copy the timing information from the dat indication into the one
       * stored in the UE CB, will be later utilized to handle Timing advance 
       */

      if ((ret = rgSCHUtlUpdSch (subfrmInfo, cell, ue, datInd, &err)) != ROK)
      {
         RGSCH_FREE_MEM(subfrmInfo);
         err.errType = RGSCHERR_TOM_DATIND;
         DU_LOG("\nERROR  -->  SCH : Unable to handle Data"
                   " Indication for UEID:%d",ue->ueId);
         return RFAILED;
      }

      CM_LLIST_NEXT_NODE(lnkLst, tmp);
   }
   /* RRM_RBC_X */
   /* update the UL PRB usage for all GBR QCIs*/
   for (idx = 0; idx < RGM_MAX_QCI_REPORTS; idx++)
   {
      cell->prbUsage.qciPrbRpts[idx].ulTotPrbUsed += subfrmInfo->qcisUlPrbCnt[idx];
   }
   /* RRM_RBC_Y */

   /* chk if Sch needs to dealloc datInd after unpk */
   RGSCH_FREE_MEM(subfrmInfo);
   return (ret);
}  /* rgMacSchSfRecpInd */

#ifdef LTEMAC_SPS
/**
 * @brief Function to handle relInd from MAC for a UE
 *
 * @details
 *
 *     Function: RgMacSchSpsRelInd
 *
 *     Handler for processing relInd for UL SPS of a UE
 *
 *     Invoked by: 
 *         MAC
 *
 *     Processing Steps:
 *           
 *  @param[in] Pst               *pst
 *  @param[in] RgInfSpsRelInfo   *relInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgMacSchSpsRelInd(Pst *pst,RgInfSpsRelInfo *relInfo)
{
   RgSchUeCb       *ue;
   RgSchCellCb     *cell;
   Inst            inst = (pst->dstInst - SCH_INST_START);


   /* No need to chk for cell being NULL as MAC wouldn't have found instance if
    * it doesnt exist */
   cell = rgSchCb[inst].rgrSap[relInfo->cellSapId].cell;

   if ((ue = rgSCHDbmGetUeCb(cell, relInfo->cRnti)) == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : No Ue exists with CRNTI:%d",relInfo->cRnti);
      return RFAILED;
   }

   if ((rgSCHUtlSpsRelInd(cell, ue, relInfo->isExplRel)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : RelInd processing for CRNTI:%d failed",relInfo->cRnti);
      return RFAILED;
   }
   return ROK;
} /* end of RgMacSchSpsRelInd */
#endif /* LTEMAC_SPS */

#ifdef LTE_L2_MEAS

/**
 * @brief Function to handle L2MeasCfm from MAC
 *
 * @details
 *
 *     Function: RgMacSchL2MeasCfm
 *
 *     Handler for processing L2 measurement confirm 
 *
 *     Invoked by: 
 *         MAC
 *
 *     Processing Steps:
 *           
 *  @param[in] Pst               *pst
 *  @param[in] RgInfL2MeasCfm    *measCfm
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgMacSchL2MeasCfm(Pst *pst, RgInfL2MeasCfm *measCfm)
{
   RgSchCellCb       *cell = NULLP;
   Inst              inst = (pst->dstInst - SCH_INST_START);
   CmLList           *lnk;
   RgSchL2MeasCb     *measCb = NULLP;
   RgSchCb           *instCb =  &rgSchCb[inst];
   uint32_t          idx;
   LrgSchMeasCfmInfo schMeasCfm;
   uint8_t           qciVal;
   uint8_t           idx1; /*LTE_L2_MEAS_PHASE2*/ 
   uint8_t           qciVal1;

   /* Find the cellCb using cellId in measInfo. Iterate through all cells
    * in rgrsapCb in RgschCb */
   for (idx = 0; idx < instCb->numSaps; idx++)
   {
      if ( instCb->rgrSap[idx].cell->cellId == measCfm->cellId)
      {
         /* got the cell break the loop */
         cell = instCb->rgrSap[idx].cell;
         break;
      }
   }
   /* If no cellCb return Err with Invalid Cell Id */
   if (cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : Meas Cfm Failed.Invalid Cell Id");
      return RFAILED;
   }
   

   /* Iterate through all meas requests in l2mList in cellCb */
   lnk = cell->l2mList.first;
   while(lnk != NULLP)
   {
      /* Get the MeasCb : RgSchL2MeasCb */
      measCb = (RgSchL2MeasCb *)lnk->node;
      lnk = lnk->next;
      if (measCb->measReq.hdr.transId == measCfm->transId)
      {
         break;
      }
   }
   if ( measCb == NULLP )
   {
      return ( RFAILED );
   }


   if(measCfm->cfm.status != LCM_PRIM_OK)
   {
      for (idx = 0; idx < measCb->measReq.avgPrbQciUl.numQci; idx++)
      {
        qciVal = measCb->measReq.avgPrbQciUl.qci[idx];
        cell->qciArray[qciVal].qci = 0;
      }
      /* Delete this measCb, send the negative confirmation to
       * stack manager */
      cmLListDelFrm(&cell->l2mList, &measCb->measLnk);
      /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)&measCb, sizeof(RgSchL2MeasCb));
      memset(&schMeasCfm, 0, sizeof(LrgSchMeasCfmInfo));
      schMeasCfm.measType     = measCfm->measType;
      schMeasCfm.cfm          = measCfm->cfm;
      schMeasCfm.hdr.transId  = measCfm->transId;
      schMeasCfm.cellId       = measCfm->cellId;
      RgMiLrgSchL2MeasCfm(&(instCb->rgSchInit.lmPst), &schMeasCfm);
      return ROK;
   }
   for(idx = 0; idx < measCfm->u.prbCfm.numQci; idx++)
   {
      measCb->avgPrbQciUl.prbUsage[idx].prbUsage = measCfm->u.prbCfm.prbUsage[idx].prbUsage;
      measCb->avgPrbQciUl.prbUsage[idx].qciValue = measCfm->u.prbCfm.prbUsage[idx].qciValue;
      /*LTE_L2_MEAS_PHASE2*/
      qciVal1 = measCfm->u.prbCfm.prbUsage[idx].qciValue;
      for(idx1=0;idx1<measCb->measReq.avgPrbQciUl.numQci;idx1++)
      {
         if(measCb->measReq.avgPrbQciUl.qci[idx1] == qciVal1)
         {
            break;
         }
      }
      if(idx1 == measCb->measReq.avgPrbQciUl.numQci)
      {
         measCb->measReq.avgPrbQciUl.qci[measCb->measReq.avgPrbQciUl.numQci++] = qciVal1;
      }
   }
   measCb->avgPrbQciUl.numQci = measCfm->u.prbCfm.numQci;
   measCb->cfmRcvd = TRUE;
   cell->sndL2Meas = TRUE;
   return ROK;
} /* end of RgMacSchL2MeasCfm */

/**
 * @brief Function to handle L2MeasStopCfm from MAC
 *
 * @details
 *
 *     Function: RgMacSchL2MeasStopCfm
 *
 *     Handler for processing L2 measurement confirm 
 *
 *     Invoked by: 
 *         MAC
 *
 *     Processing Steps:
 *           
 *  @param[in] Pst               *pst
 *  @param[in] RgInfL2MeasCfm    *measCfm
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgMacSchL2MeasStopCfm(Pst *pst,RgInfL2MeasCfm  *measCfm)
{
   LrgSchMeasCfmInfo schMeasCfm;
   Inst              inst = (pst->dstInst - SCH_INST_START);
   RgSchCb           *instCb =  &rgSchCb[inst];

   memset(&schMeasCfm, 0, sizeof(LrgSchMeasCfmInfo));
   schMeasCfm.measType     = measCfm->measType;
   schMeasCfm.cfm          = measCfm->cfm;
   schMeasCfm.hdr.transId  = measCfm->transId;
   schMeasCfm.cellId       = measCfm->cellId;
   RgMiLrgSchL2MeasStopCfm(&(instCb->rgSchInit.lmPst), &schMeasCfm);
   return ROK;
}
#endif

/************** TFU Interface *************/

/**
 * @brief Bind confirm API for TFU SAP on scheduler instance. 
 *
 * @details
 *
 *     Function : RgLiTfuSchBndCfm
 *      
 *      This API is invoked by PHY to confirm TFU SAP bind. 
 *     
 *           
 *  @param[in]  Pst   *pst 
 *  @param[in]  SuId  suId 
 *  @param[in]  uint8_t    status
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgLiTfuSchBndCfm (Pst *pst,SuId suId, uint8_t status)
{
   S16 ret;
   RgSchLowSapCb  *tfuSap;
   Inst  instId = pst->dstInst - SCH_INST_START;

   if(suId >= rgSchCb[instId].numSaps)
   {
      DU_LOG("\nERROR  -->  SCH : Incorrect SuId");
      return RFAILED;
   }
   /* Lets validate suId first */
   tfuSap = &(rgSchCb[instId].tfuSap[suId]);

   if (suId != tfuSap->sapCfg.suId)
   {
      DU_LOG("\nERROR  -->  SCH : Incorrect SuId. Configured (%d)"
            "Recieved (%d)", tfuSap->sapCfg.suId, suId);
      return RFAILED;
   }
   ret = rgSCHLmmBndCfm (pst, suId, status);
   return (ret);
}  /* RgLiTfuSchBndCfm */

/**
 * @brief Random Access Request indication from PHY.
 *
 * @details
 *
 *     Function : RgLiTfuRaReqInd
 *      
 *      This API is invoked by PHY to send Random Access Request to Scheduler.
 *      This API contains information for Random Access Request including 
 *      raRnti, list of associated RAPIDs and related information.
 *           
 *  @param[in]  Pst              *pst 
 *  @param[in]  SuId             suId 
 *  @param[in]  TfuRaReqIndInfo  *raReqInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgLiTfuRaReqInd(Pst *pst,SuId suId, TfuRaReqIndInfo *raReqInd)
{
   S16   ret;
   Inst  inst = pst->dstInst-SCH_INST_START;

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : SAP Validation failed SuId(%d)", suId);
      /* Free up the memory for the request structure */
      RGSCH_FREE_MEM(raReqInd);
      return (ret);
   }

   if(raReqInd == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : Invalid input pointer for raReqInd Failed");
      return RFAILED;
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : No cell exists");
      return RFAILED;
   }

   ret = rgSCHTomRaReqInd(rgSchCb[inst].tfuSap[suId].cell, raReqInd);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(raReqInd);
   /*SCH_FREE(pst->region, pst->pool, (Data *)raReqInd,
         sizeof(TfuRaReqIndInfo)); */
   return (ret);
}  /* RgLiTfuRaReqInd */

/**
 * @brief Uplink CQI indication from PHY.
 *
 * @details
 *
 *     Function : RgLiTfuUlCqiInd
 *      
 *      This API is invoked by PHY to send Uplink CQI to Scheduler.
 *      This API contains Uplink CQI information reported per UE.
 *           
 *  @param[in]  Pst               *pst 
 *  @param[in]  SuId              suId 
 *  @param[in]  TfuUlCqiIndInfo   *ulCqiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgLiTfuUlCqiInd(Pst *pst, SuId suId, TfuUlCqiIndInfo *ulCqiInd)
{
   S16   ret;
   Inst  inst = pst->dstInst-SCH_INST_START;

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : SAP Validation failed");
      /* Free up the memory for the request structure */
      RGSCH_FREE_MEM(ulCqiInd);
      return (ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : No cell exists");
      /* Free up the memory for the request structure */
      RGSCH_FREE_MEM(ulCqiInd);
      return RFAILED;
   }
   ret = rgSCHTomUlCqiInd (rgSchCb[inst].tfuSap[suId].cell, ulCqiInd);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(ulCqiInd);
   return (ret);
}  /* RgLiTfuUlCqiInd */

/**
 * @brief PUCCH power adjustment indication.
 *
 * @details
 *
 *     Function : RgLiTfuPucchDeltaPwrInd
 *
 *      This API is invoked by PHY to send PUCCH power adjustment
 *      indication.
 *
 *  @param[in]  Pst                     *pst 
 *  @param[in]  SuId                    suId 
 *  @param[in]  TfuPucchDeltaPwrIndInfo *pucchDeltaPwr
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgLiTfuPucchDeltaPwrInd(Pst *pst,SuId suId,TfuPucchDeltaPwrIndInfo *pucchDeltaPwr)
{
   S16   ret;
   Inst  inst = pst->dstInst-SCH_INST_START;

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : SAP Validation failed");
      /* Free up the memory for the request structure */
      RGSCH_FREE_MEM(pucchDeltaPwr);
      return (ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : No cell exists");
      return RFAILED;
   }
   ret = rgSCHTomPucchDeltaPwrInd (rgSchCb[inst].tfuSap[suId].cell, pucchDeltaPwr);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(pucchDeltaPwr);
   return (ret);
}  /* RgLiTfuPucchDeltaPwrInd */


/**
 * @brief HARQ ACK indication from PHY for Downlink transmissions.
 *
 * @details
 *
 *     Function : RgLiTfuHqInd
 *      
 *      This API is invoked by PHY to send HARQ ACK information to Scheduler
 *      on recieving HARQ ACK/NACK from UEs.
 *      This API contains HARQ ACK information recieved by PHY for downlink
 *      transmissions.
 *           
 *  @param[in]  Pst                *pst
 *  @param[in]  SuId               suId 
 *  @param[in]  TfuHqIndInfo       *harqAckInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgLiTfuHqInd(Pst *pst, SuId suId, TfuHqIndInfo *harqAckInd)
{
   S16   ret;
   Inst  inst = (pst->dstInst - SCH_INST_START);


#ifndef NO_ERRCLS
   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : SAP Validation failed");
      RGSCH_FREE_MEM(harqAckInd);
      return (ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : No cell exists");
      return RFAILED;
   }
#endif

   /* Now call the TOM (Tfu ownership module) primitive to process further */
   ret = rgSCHTomHarqAckInd (rgSchCb[inst].tfuSap[suId].cell, harqAckInd);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(harqAckInd);
   return (ret);
}  /* RgLiTfuHqInd */


/**
 * @brief Scheduling request(SR) indication from PHY for an UE.
 *
 * @details
 *
 *     Function : RgLiTfuSrInd
 *      
 *      This API is invoked by PHY to send Scheduling request information to
 *      Scheduler on recieving SR from a list of UEs.
 *      This API contains scheduling request information recieved by PHY for UEs.
 *           
 *  @param[in]  Pst           *pst
 *  @param[in]  SuId          suId 
 *  @param[in]  TfuSrIndInfo  *srInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgLiTfuSrInd(Pst *pst, SuId suId, TfuSrIndInfo *srInd)
{
   S16   ret;
   Inst  inst = pst->dstInst-SCH_INST_START;

#ifndef NO_ERRCLS
   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH :  SAP Validation failed");
      RGSCH_FREE_MEM(srInd);
      return (ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : RgLiTfuSrInd()No cell exists");
      return RFAILED;
   }
#endif
   /* Now call the TOM (Tfu ownership module) primitive to process further */
   ret = rgSCHTomSrInd (rgSchCb[inst].tfuSap[suId].cell, srInd);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(srInd);
   return (ret);
}  /* RgLiTfuSrInd */


/**
 * @brief Downlink CQI indication from PHY for an UE.
 *
 * @details
 *
 *     Function : RgLiTfuDlCqiInd
 *      
 *      This API is invoked by PHY to send Downlink CQI indication to Scheduler
 *      on recieving downlink CQI from UE.
 *      This API contains downlink CQI information recieved by PHY for an UE.
 *           
 *  @param[in]  Pst              *pst
 *  @param[in]  SuId             suId 
 *  @param[in]  TfuDlCqiIndInfo  *dlCqiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgLiTfuDlCqiInd(Pst *pst, SuId suId, TfuDlCqiIndInfo *dlCqiInd)
{
   S16   ret;
   Inst  inst = pst->dstInst-SCH_INST_START;

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : SAP Validation failed");
      /* Free up the memory for the request structure */
      RGSCH_FREE_MEM(dlCqiInd);
      return (ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : No cell exists");
      return RFAILED;
   }
   ret = rgSCHTomDlCqiInd (rgSchCb[inst].tfuSap[suId].cell, dlCqiInd);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(dlCqiInd);
   return (ret);
}  /* RgLiTfuDlCqiInd */
#ifdef TFU_UPGRADE

/**
 * @brief Raw CQI indication from PHY for an UE.
 *
 * @details
 *
 *     Function : RgLiTfuRawCqiInd
 *      
 *      This API is invoked by PHY to send Raw CQI indication to Scheduler
 *      on receiving Raw CQI from UE.
 *      This API contains Raw CQI information recieved by PHY for an UE.
 *           
 *  @param[in]  Pst              *pst
 *  @param[in]  SuId             suId 
 *  @param[in]  TfuRawCqiIndInfo  *rawCqiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgLiTfuRawCqiInd(Pst *pst, SuId suId, TfuRawCqiIndInfo *rawCqiInd)
{
   S16   ret;
   Inst  inst = pst->dstInst-SCH_INST_START;

#ifdef NO_ERRCLS
   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : SAP Validation failed");
      /* Free up the memory for the request structure */
      RGSCH_FREE_MEM(rawCqiInd);
      return (ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : No cell exists");
      return RFAILED;
   }
#endif
   ret = rgSCHTomRawCqiInd (rgSchCb[inst].tfuSap[suId].cell, rawCqiInd);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(rawCqiInd);
   return (ret);
}  /* RgLiTfuRawCqiInd */

/**
 * @brief SRS indication from PHY for an UE.
 *
 * @details
 *
 *     Function : RgLiTfuSrsInd
 *      
 *      This API is invoked by PHY to send UL SRS indication to Scheduler
 *      on receiving a SRS from UE.
 *      This API contains SRS information recieved by PHY for an UE.
 *           
 *  @param[in]  Pst              *pst
 *  @param[in]  SuId             suId 
 *  @param[in]  TfuSrsIndInfo  *srsInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgLiTfuSrsInd(Pst *pst, SuId suId, TfuSrsIndInfo *srsInd)
{
   S16   ret;
   Inst  inst = pst->dstInst-SCH_INST_START;

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH :  SAP Validation failed");
      /* Free up the memory for the request structure */
      RGSCH_FREE_MEM(srsInd);
      return (ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : No cell exists");
      return RFAILED;
   }
   ret = rgSCHTomSrsInd (rgSchCb[inst].tfuSap[suId].cell, srsInd);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(srsInd);
   return (ret);
}  /* RgLiTfuSrsInd */

#endif 

/**
 * @brief DOA indication from PHY for an UE.
 *
 * @details
 *
 *     Function : RgLiTfuDoaInd
 *      
 *      This API is invoked by PHY to send Direction Of Arrival to Scheduler
 *      on calculating DOA at PHYSICAL LAYER for an UE.
 *      This API contains DOA information calculated by PHY for an UE.
 *           
 *  @param[in]  Pst              *pst
 *  @param[in]  SuId             suId 
 *  @param[in]  TfuDoaIndInfo    *doaInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgLiTfuDoaInd(Pst *pst, SuId suId, TfuDoaIndInfo *doaInd)
{
   S16   ret;
   Inst  inst = pst->dstInst-SCH_INST_START;

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : SAP Validation failed");
      /* Free up the memory for the request structure */
      RGSCH_FREE_MEM(doaInd);
      return (ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : No cell exists");
      return RFAILED;
   }
   ret = rgSCHTomDoaInd (rgSchCb[inst].tfuSap[suId].cell, doaInd);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(doaInd);
   return (ret);
}  /* RgLiTfuDlCqiInd */

/**
 * @brief CRC indication from PHY.
 *
 * @details
 *
 *     Function : RgLiTfuCrcInd
 *      
 *      This API is invoked by PHY to give CRC indication to scheduler.
 *           
 *  @param[in]  Pst               *pst
 *  @param[in]  SuId              suId 
 *  @param[in]  TfuCrcIndInfo *crcInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgLiTfuCrcInd(Pst *pst, SuId suId, TfuCrcIndInfo  *crcInd)
{
   S16              ret;
   Inst             inst      = pst->dstInst-SCH_INST_START;

#ifdef XEON_SPECIFIC_CHANGES
struct timeval start6, end6;
gettimeofday(&start6, NULL);
#endif
#ifndef NO_ERRCLS
   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : SAP Validation failed");
      RGSCH_FREE_MEM(crcInd);
      return (ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : No cell exists");
      return RFAILED;
   }
#endif

   /* Now call the TOM (Tfu ownership module) primitive to process further */
   ret = rgSCHTomCrcInd(rgSchCb[inst].tfuSap[suId].cell, crcInd);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(crcInd);
#ifdef XEON_SPECIFIC_CHANGES
gettimeofday(&end6, NULL);
#endif
   return (ret);
}  /* RgLiTfuCrcInd */

/**
 * @brief Timing Advance indication from PHY.
 *
 * @details
 *
 *     Function : RgLiTfuTimingAdvInd
 *      
 *      This API is invoked by PHY to indicate timing advance to Scheduler for
 *       an UE.
 *           
 *  @param[in]  Pst                  *pst
 *  @param[in]  SuId                 suId 
 *  @param[in]  TfuTimingAdvIndInfo  *timingAdvInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgLiTfuTimingAdvInd(Pst *pst, SuId suId, TfuTimingAdvIndInfo *timingAdvInd)
{
   S16   ret;
   Inst  inst = pst->dstInst-SCH_INST_START;

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : SAP Validation failed");
      /* Free up the memory for the request structure */
      RGSCH_FREE_MEM(timingAdvInd);
      return (ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : No cell exists");
      return RFAILED;
   }
   /* Now call the TOM (Tfu ownership module) primitive to process further */
   ret = rgSCHTomTimingAdvInd(rgSchCb[inst].tfuSap[suId].cell, timingAdvInd);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(timingAdvInd);
   return (ret);
}  /* RgLiTfuTimingAdvInd */

/************* RGM  Interface ****************/
/**
 * @brief API for bind request from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgmBndReq
 *     
 *     This API is invoked by RRM towards MAC to bind RGM SAP. 
 *     These API validates the Pst, spId, suId and sends the bind confirm to
 *     RRM.
 *
 *           
 *  @param[in]  Pst   *pst
 *  @param[in]  SuId  suId
 *  @param[in]  SpId  spId
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgUiRgmBndReq(Pst *pst,SuId  suId,SpId  spId)
{
   S16       ret = ROK;
   Pst       tmpPst;   /* Temporary Post Structure */
   Inst      instId = pst->dstInst-SCH_INST_START;

   tmpPst.prior       = pst->prior;
   tmpPst.route       = pst->route;
   tmpPst.selector    = pst->selector;
   tmpPst.region      = rgSchCb[instId].rgSchInit.region;
   tmpPst.pool        = rgSchCb[instId].rgSchInit.pool;

   tmpPst.srcProcId   = rgSchCb[instId].rgSchInit.procId;
   tmpPst.srcEnt      = rgSchCb[instId].rgSchInit.ent;
   tmpPst.srcInst     = rgSchCb[instId].rgSchInit.inst + SCH_INST_START;

   tmpPst.event       = EVTNONE;

   tmpPst.dstProcId   = pst->srcProcId;
   tmpPst.dstEnt      = pst->srcEnt;
   tmpPst.dstInst     = pst->srcInst;

   /*TODO remove follo statement*/
   rgSchCb[instId].rgmSap[spId].sapSta.sapState = LRG_UNBND;

   if(spId < rgSchCb[instId].numSaps)
   {
      /* Check the state of the SAP */
      switch (rgSchCb[instId].rgmSap[spId].sapSta.sapState)
      {
         /* This case might not be needed if SAP not configured then it will go
          * to else of above if condition */
         case LRG_UNBND: /* SAP is not bound */
	    DU_LOG("\nDEBUG  -->  SCH : SAP is not yet bound");
            rgSchCb[instId].rgmSap[spId].sapSta.sapState = LRG_BND;
            rgSchCb[instId].rgmSap[spId].sapCfg.suId = suId;
            /* Send Bind Confirm with status as SUCCESS */
            ret = rgSCHUtlRgmBndCfm(instId, suId, CM_BND_OK);
             /*Indicate to Layer manager  */
            break;
         case LRG_BND: /* SAP is already bound*/
	    DU_LOG("\nDEBUG  -->  SCH : SAP is already bound");
            ret = rgSCHUtlRgmBndCfm(instId, suId, CM_BND_OK);
            break;
         default: /* Should Never Enter here */
#if (ERRCLASS & ERRCLS_ADD_RES) 
            DU_LOG("\nERROR  -->  SCH : Invalid SAP State:RgUiRgmBndReq failed\n");
#endif
            ret = rgSCHUtlRgmBndCfm(instId, suId, CM_BND_NOK);
            break;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
/* ccpu00117035 - MOD - Changed ErrVal argument from accessing sap state 
   to spId to avoid seg fault due to invalid sapID */
      DU_LOG("\nERROR  -->  SCH : Invalid SAP Id:RgUiRrmBndReq failed\n");
#endif
      ret = RgUiRgmBndCfm(&tmpPst, suId, CM_BND_NOK);
   }
   return (ret);
}  /* RgUiRgmBndReq */

/**
 * @brief API for unbind request from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgmUbndReq
 *     
 *     This API is invoked by RRM towards MAC to unbind RGM SAP. 
 *     These API validates the Pst, spId, suId and transfers the unbind request 
 *     specific information to corresponding ownership module (GOM) API.
 *
 *           
 *  @param[in]  Pst    *pst
 *  @param[in]  SuId   suId
 *  @param[in]  Reason reason
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgUiRgmUbndReq(Pst    *pst,SpId   spId,Reason reason)
{
   Inst instId = pst->dstInst-SCH_INST_START;

   /* SAP Id validation */
   if (spId < rgSchCb[instId].numSaps)
   {
      switch(rgSchCb[instId].rgmSap[spId].sapSta.sapState)
      {
         case LRG_BND: /* SAP is already bound*/
            /* setting SAP state to UN BOUND */
	         DU_LOG("\nDEBUG  -->  SCH : SAP is already bound");
            rgSchCb[instId].rgmSap[spId].sapSta.sapState = LRG_UNBND;
            break;
         default:
#if (ERRCLASS & ERRCLS_ADD_RES)      
            DU_LOG("\nERROR  -->  SCH : Invalid SAP State: RgUiRgmUbndReq failed\n");
#endif
            return RFAILED;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      DU_LOG("\nERROR  -->  SCH : Invalid SAP Id:RgUiRgmUbndReq failed\n");
#endif
      return RFAILED;
   }
   return ROK;
}  /* RgUiRgmUbndReq */


/**
 * @brief API for start or stop PRB reporting from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgmCfgPrbRprt
 *     
 *     This API is invoked by RRM towards MAC to start or stop calculating
 *     Average PRB usage in downlink and uplink. The average PRB usage will
 *     be reported to RRM based on confiured periodicity.
 *
 *           
 *  @param[in]  Pst   *pst
 *  @param[in]  SuId  suId
 *  @param[in]  SpId  spId
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgUiRgmCfgPrbRprt(Pst   *pst, SpId  spId,RgmPrbRprtCfg *prbRprtCfg)
{
/* Initalize*/
   RgSchCellCb   *cell;
   RgSchPrbUsage *prbUsage;
   Inst          inst = (pst->dstInst  - SCH_INST_START);

   cell = rgSchCb[inst].rgmSap[spId].cell;
   prbUsage = &cell->prbUsage;
   prbUsage->prbRprtEnabld = prbRprtCfg->bConfigType;
   prbUsage->rprtPeriod = prbRprtCfg->usPrbAvgPeriodicty;
   RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, prbUsage->startTime, 1);

   /* clear the qciPrbRpts for all GBR QCIs */
   memset(&prbUsage->qciPrbRpts[0], 0, 
             (RGM_MAX_QCI_REPORTS * sizeof(RgSchQciPrbUsage)));
   DU_LOG("\nDEBUG  -->  SCH : RgUiRgmCfgPrbRprt config type %d with the report period %d",
	     prbUsage->prbRprtEnabld,prbUsage->rprtPeriod);

   /* ccpu00134393 : mem leak fix */
      SCH_FREE(prbRprtCfg, sizeof(RgmPrbRprtCfg));
  
   return ROK;
}
/**
 * @brief ERROR INDICATION from PHY for the i failed unlicensed Scell transmission. 
 *
 * @details
 *
 *     Function : RgLiTfuErrInd
 *      
 *      This API is invoked by PHY to send ERROR INDICATION to scheduler 
 *      Currently invoked in the cases when the Unlicensed SCell transmission
 *      fails.
 *      This API contains the Cell and subframe information for which the
 *      transmission failed. 
 *           
 *  @param[in]  Pst                *pst
 *  @param[in]  SuId               suId 
 *  @param[in]  TfuErrIndInfo      *errIndInfo 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 RgLiTfuErrInd(Pst *pst, SuId suId, TfuErrIndInfo  *errInd)
{
   S16   ret = ROK;
#ifdef LTE_ADV
   Inst  inst = (pst->dstInst - SCH_INST_START);
#endif

#ifndef NO_ERRCLS
   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {

      DU_LOG("\nERROR  -->  SCH : SAP Validation failed");
      return (ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : No cell exists");
      return RFAILED;
   }
#endif

#ifdef LTE_ADV
   /* Now call the TOM (Tfu ownership module) primitive to process further */
   ret = rgSCHLaaErrInd(rgSchCb[inst].tfuSap[suId].cell, errInd);
#endif
   return (ret);
}  /* RgLiTfuErrInd */



/**********************************************************************
 
         End of file
**********************************************************************/
