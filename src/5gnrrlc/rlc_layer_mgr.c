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

        Name:    NR RLC - Layer Manager file

        Type:    C source file

        Desc:    It contains the following common  functions for processing
                 the all the external interfaces.
                    -- RlcMiRlcConfigReq
                    -- RlcMiRlcConfigCfm 
                    -- RlcMiLkwCntrlReq
                    -- RlcMiLkwCntrlCfm
                    -- RlcMiLkwStaReq
                    -- RlcMiLkwStaCfm
                    -- RlcMiLkwStaInd
                    -- RlcMiLkwStsReq
                    -- RlcMiLkwStsCfm
                    -- RlcMiLkwTrcInd 

        File:     kw_lmm.c

************************************************************************/
/** @file rlc_layer_mgr.c
@brief RLC Layer Manager Module
**/


/* header (.h) include files */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "rlc_err.h"        /* Err defines */
#include "rlc_env.h"        /* RLC environment options */


/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "rlc_utils.h"            /* RLC defines */
#include "rlc_dl_ul_inf.h"
#include "rlc_dl.h"
#include "rlc_ul.h"

#define RLC_MODULE RLC_DBGMASK_LMM

RLCStats gRlcStats;

/*********************************************************************
 *             Forward Declaration of LKW Porting Functions
 ********************************************************************/
RlcCb *rlcCb[MAX_RLC_INSTANCES];
S16 rlcActvTmr ARGS ((Ent ent, Inst inst));

static Void rlcLmmSendCfm ARGS ((RlcCb *gCb,Pst *pst,RlcMngmt *cfm,uint8_t type,
                                 Header *hdr));
static S16 rlcLmmGenCfg ARGS ((RlcCb *gCb, RlcGenCfg *cfg));
static S16 rlcLmmCfgKwuSap ARGS ((RlcCb *gCb,RlcSapCfg *cfg));
static S16 rlcLmmCfgUdxSap ARGS ((RlcCb *gCb,RlcSapCfg *cfg));
static S16 rlcLmmCfgCkwSap ARGS ((RlcCb *gCb,RlcSapCfg   *cfg));
static S16 rlcLmmCfgRguSap ARGS ((RlcCb *gCb,RlcSapCfg   *cfg));
static S16 rlcLmmGenCntrl ARGS ((RlcCb *gCb,RlcMngmt *cntrl));
static S16 rlcLmmUdxSapCntrl ARGS ((RlcCb *gCb,RlcMngmt *cntrl));
static S16 rlcLmmLSapCntrl ARGS ((RlcCb *gCb,RlcMngmt *cntrl));
static S16 rlcLmmGetKwuSapSta ARGS ((RlcCb *gCb,RlcKwuSapSta *sta));
static S16 rlcLmmGetRguSapSta ARGS ((RlcCb *gCb,RlcRguSapSta *sta));
static S16 rlcLmmGetCkwCntSapSta ARGS ((RlcCb *gCb,RlcCkwCntSapSta *sta));
static S16 rlcLmmGetGenSts ARGS ((RlcCb *gCb,RlcGenSts *sts,Action action));
static S16 rlcLmmGetSapSts ARGS ((RlcCb *gCb,RlcMngmt *sts,Elmnt elmnt,Action 
                                  action));


/*********************************************************************
 *             Primitives for RGU interface 
 ********************************************************************/
/**
 * @brief
 *    Stores the general RLC configuration. 
 * @details
 *    It calculates the memory requirement of the Saps  
 *    It also initializes the control blocks, the hash lists and registers
 *    the timers. In case of error it rolls back and reports error in the 
 *    confirm.If the general configuration is already done it is treated as 
 *    reconfiguration, but right now reconfiguration is not supported.
 * 
 * @param[in]  gCb   RLC Instance Control Block
 * @param[in]  cfm   RLC LM general config structure
 * 
 * @return  S16
 *    LCM_REASON_NOT_APPL  (SUCCESS) 
 *    LCM_REASON_MEM_NOAVAIL
 *    LCM_REASON_REGTMR_FAIL
 *    LCM_REASON_INVALID_PAR_VAL
 *    LCM_REASON_HASHING_FAILED
 */
static S16 rlcLmmGenCfg(RlcCb  *gCb,RlcGenCfg *cfg)
{
   Size     rlcSapSize;
   Size     rlcUdxSapSize;
   S16      ret;
   CmTqCp   *rlcTqCp;
   Size     rguSapSize;

   if(cfg->maxUe > RLC_MAX_UE) 
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      DU_LOG("\nERROR  --> RLC : Invalid maxUe : Max [%lu] Received [%lu]", 
            RLC_MAX_UE, 
            cfg->maxUe);
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      return (LCM_REASON_INVALID_PAR_VAL);
   }

   if(cfg->maxKwuSaps > RLC_MAX_KWUSAPS)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      DU_LOG("\nERROR  --> RLC : Invalid maxKwuSaps : Max [%lu] Received [%lu]", 
            RLC_MAX_KWUSAPS, 
            cfg->maxKwuSaps);
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      return (LCM_REASON_INVALID_PAR_VAL);
   }

   if(cfg->maxUdxSaps > RLC_MAX_UDXSAPS)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      DU_LOG("\nERROR  --> RLC : Invalid maxUduSaps : Max [%lu] Received [%lu]", 
            RLC_MAX_UDXSAPS, 
            cfg->maxUdxSaps);
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      return (LCM_REASON_INVALID_PAR_VAL);
   }
   
   if((cfg->maxRguSaps == 0) || (cfg->maxRguSaps > RLC_MAX_RGUSAPS))
   {
      

      return (LCM_REASON_INVALID_PAR_VAL);
   }
   if(gCb->init.cfgDone == TRUE)
   {
      /* reconfiguration not supported */
      return (LCM_REASON_RECONFIG_FAIL);
   }

   gCb->genCfg = *cfg;

   /* Timer Queue Control point initialization */
   rlcTqCp         = &(gCb->rlcTqCp);
   rlcTqCp->tmrLen = RLC_TMR_LEN;
   rlcTqCp->nxtEnt = 0;

   gCb->rlcThpt.inst = gCb->init.inst;
   gCb->rlcThpt.ueTputInfo.ueThptTmr.tmrEvnt = TMR_NONE;
   gCb->rlcThpt.ueTputInfo.numActvUe = 0;
   memset(gCb->rlcThpt.ueTputInfo.thptPerUe, 0, MAX_NUM_UE * sizeof(RlcThptPerUe));

   gCb->rlcThpt.snssaiTputInfo.snssaiThptTmr.tmrEvnt = TMR_NONE;
   
   if(gCb->genCfg.rlcMode == LKW_RLC_MODE_DL)
   {
      RLC_ALLOC(gCb,gCb->u.dlCb, sizeof (RlcDlCb));
      if (gCb->u.dlCb == NULLP)
      {  
         DU_LOG("\nERROR  --> RLC : Memory Allocation failed");   
         return (LCM_REASON_MEM_NOAVAIL);
      
      }
      /* allocate memory to the KWU sap's */
      rlcSapSize = (Size)((Size) gCb->genCfg.maxKwuSaps *
                   (Size)sizeof(RlcKwuSapCb));

      RLC_ALLOC(gCb,gCb->u.dlCb->rlcKwuDlSap, rlcSapSize);

#if (ERRCLASS & ERRCLS_INT_PAR)
      if(gCb->u.dlCb->rlcKwuDlSap == NULLP)
      {
         RLC_FREE(gCb,gCb->u.dlCb, sizeof (RlcDlCb));
         DU_LOG("\nERROR  --> RLC : Memory Allocation failed");   
         return (LCM_REASON_MEM_NOAVAIL);
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

      gCb->u.dlCb->numKwuSaps = gCb->genCfg.maxKwuSaps;

      rlcUdxSapSize = (Size)((Size) gCb->genCfg.maxUdxSaps *
                   (Size)sizeof(RlcUdxDlSapCb));

      RLC_ALLOC(gCb,gCb->u.dlCb->udxDlSap, rlcUdxSapSize);
#if (ERRCLASS & ERRCLS_INT_PAR)
      if(gCb->u.dlCb->udxDlSap == NULLP)
      {
         RLC_FREE(gCb,gCb->u.dlCb->rlcKwuDlSap, rlcSapSize);
         RLC_FREE(gCb,gCb->u.dlCb, sizeof (RlcDlCb));
         DU_LOG("\nERROR  --> RLC : Memory Allocation failed");   
         return (LCM_REASON_MEM_NOAVAIL);
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

      rguSapSize = (Size)((Size) gCb->genCfg.maxRguSaps *
                   (Size)sizeof(RlcRguSapCb));
      RLC_ALLOC(gCb,gCb->u.dlCb->rguDlSap, rguSapSize);
#if (ERRCLASS & ERRCLS_INT_PAR)
      if(gCb->u.dlCb->rguDlSap == NULLP)
      {
         RLC_FREE(gCb,gCb->u.dlCb->udxDlSap, rlcSapSize);
         RLC_FREE(gCb,gCb->u.dlCb->rlcKwuDlSap, rlcSapSize);
         RLC_FREE(gCb,gCb->u.dlCb, sizeof (RlcDlCb));
      
         RLCLOGERROR(gCb,ERRCLS_INT_PAR, EKW043, (ErrVal) cfg->maxUe,
                   "rlcLmmGenCfg: SgetSBuf Failed for rguSap...!");
         return (LCM_REASON_MEM_NOAVAIL);
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

      
      /* Initialize the Ue and Cell hash list */
      ret = rlcDbmDlInit(gCb);
      if (ret != ROK)
      {
         RLC_FREE(gCb,gCb->u.dlCb->udxDlSap, rlcSapSize);
         RLC_FREE(gCb,gCb->u.dlCb->rlcKwuDlSap, rlcSapSize);
         RLC_FREE(gCb,gCb->u.dlCb->rguDlSap, rguSapSize);
         RLC_FREE(gCb,gCb->u.dlCb, sizeof (RlcDlCb));
         DU_LOG("\nERROR  --> RLC_DL : RLC DL Initialization failed");   
         return (LCM_REASON_MEM_NOAVAIL);
      }

      /* Register the timer */
      if(ODU_REG_TMR_MT(gCb->init.ent, gCb->init.inst, (uint16_t)cfg->timeRes,
              rlcActvTmr) != ROK)
      {
         RLC_FREE(gCb,gCb->u.dlCb->udxDlSap, rlcUdxSapSize);
         RLC_FREE(gCb,gCb->u.dlCb->rlcKwuDlSap, rlcSapSize);
         RLC_FREE(gCb,gCb->u.dlCb->rguDlSap, rguSapSize);
         RLC_FREE(gCb,gCb->u.dlCb, sizeof (RlcDlCb));

         return (LCM_REASON_REGTMR_FAIL);
      }

      /* initializations for background processing of freeing memory */
      rlcUtlInitToBeFreed(gCb, &(gCb->u.dlCb->toBeFreed));
      rlcUtlInitializeSelfPst(gCb);

      if(ODU_GET_MSG_BUF(gCb->init.region, gCb->init.pool, &(gCb->u.dlCb->selfPstMBuf)) != ROK)
      {
         RLC_FREE(gCb,gCb->u.dlCb->udxDlSap, rlcSapSize);
         RLC_FREE(gCb,gCb->u.dlCb->rlcKwuDlSap, rlcSapSize);
         RLC_FREE(gCb,gCb->u.dlCb->rguDlSap, rguSapSize);
         RLC_FREE(gCb,gCb->u.dlCb, sizeof (RlcDlCb));

         return (LCM_REASON_MEM_NOAVAIL);
      
      }
#ifdef LTE_L2_MEAS
      rlcUtlL2MeasDlInit(gCb);
#endif
   }
   else if(gCb->genCfg.rlcMode == LKW_RLC_MODE_UL)
   {
      RLC_ALLOC(gCb,gCb->u.ulCb, sizeof (RlcUlCb));
      if (gCb->u.ulCb == NULLP)
      {     
         DU_LOG("\nERROR  --> RLC : Memory Allocation failed");   
         return (LCM_REASON_MEM_NOAVAIL);
      
      }
      /* allocate memory to the KWU sap's */
      rlcSapSize = (Size)((Size) gCb->genCfg.maxKwuSaps *
                   (Size)sizeof(RlcKwuSapCb));

      RLC_ALLOC(gCb,gCb->u.ulCb->rlcKwuUlSap, rlcSapSize);

#if (ERRCLASS & ERRCLS_INT_PAR)
      if(gCb->u.ulCb->rlcKwuUlSap == NULLP)
      {
         RLC_FREE(gCb,gCb->u.ulCb, sizeof (RlcUlCb));
         DU_LOG("\nERROR  --> RLC : Memory Allocation failed");   
         return (LCM_REASON_MEM_NOAVAIL);
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

      gCb->u.ulCb->numKwuSaps = gCb->genCfg.maxKwuSaps;

      /* allocate memory to the KWU sap's */
      rlcUdxSapSize = (Size)((Size) gCb->genCfg.maxUdxSaps *
                   (Size)sizeof(RlcUdxUlSapCb));

      RLC_ALLOC(gCb,gCb->u.ulCb->udxUlSap, rlcUdxSapSize);

#if (ERRCLASS & ERRCLS_INT_PAR)
      if(gCb->u.ulCb->rlcKwuUlSap == NULLP)
      {
         RLC_FREE(gCb,gCb->u.ulCb->rlcKwuUlSap, rlcSapSize);
         RLC_FREE(gCb,gCb->u.ulCb, sizeof (RlcUlCb));
         DU_LOG("\nERROR  --> RLC : Memory Allocation failed");   
         return (LCM_REASON_MEM_NOAVAIL);
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

      rguSapSize = (Size)((Size) gCb->genCfg.maxRguSaps *
                   (Size)sizeof(RlcRguSapCb));
      RLC_ALLOC(gCb,gCb->u.ulCb->rguUlSap, rguSapSize);
#if (ERRCLASS & ERRCLS_INT_PAR)
      if(gCb->u.ulCb->rguUlSap == NULLP)
      {
         RLC_FREE(gCb,gCb->u.ulCb->rlcKwuUlSap, rlcSapSize);
         RLC_FREE(gCb,gCb->u.ulCb->rguUlSap, rlcSapSize);
         RLC_FREE(gCb,gCb->u.ulCb, sizeof (RlcUlCb));
      
         RLCLOGERROR(gCb,ERRCLS_INT_PAR, EKW043, (ErrVal) cfg->maxUe,
                   "rlcLmmGenCfg: SgetSBuf Failed for rguSap...!");
         return (LCM_REASON_MEM_NOAVAIL);
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

      /* Initialize the Ue and Cell hash list */
      ret = rlcDbmUlInit(gCb);
      if (ret != ROK)
      {
         RLC_FREE(gCb,gCb->u.ulCb->udxUlSap, rlcUdxSapSize);
         RLC_FREE(gCb,gCb->u.ulCb->rlcKwuUlSap, rlcSapSize);
         RLC_FREE(gCb,gCb->u.ulCb->rguUlSap, rlcSapSize);
         RLC_FREE(gCb,gCb->u.ulCb, sizeof (RlcUlCb));
         DU_LOG("\nERROR  --> RLC_DL : RLC DL Initialization failed");   
      }

      /* Register the timer */
      if(ODU_REG_TMR_MT(gCb->init.ent, gCb->init.inst, (uint16_t)cfg->timeRes,
              rlcActvTmr) != ROK)
      {
         RLC_FREE(gCb,gCb->u.ulCb->udxUlSap, rlcUdxSapSize);
         RLC_FREE(gCb,gCb->u.ulCb->rlcKwuUlSap, rlcSapSize);
         RLC_FREE(gCb,gCb->u.ulCb->rguUlSap, rlcSapSize);
         RLC_FREE(gCb,gCb->u.ulCb, sizeof (RlcUlCb));

         return (LCM_REASON_REGTMR_FAIL);
      }

#ifdef LTE_L2_MEAS
      rlcUtlL2MeasUlInit(gCb);
#endif
   }
   else
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      DU_LOG("\nERROR  --> RLC : Received Invalid RLC Mode");
#endif /* ERRCLASS & ERRCLS_INT_PAR */

      return (LCM_REASON_INVALID_PAR_VAL);
   }
   /* Timer Initialization  */
   gCb->rlcTqCp.tmrLen = RLC_TMR_LEN;

   memset(gCb->rlcTq, NULLP, sizeof(CmTqType) * RLC_TMR_LEN);

   RLC_MEM_CPY(&(gCb->init.lmPst), &cfg->lmPst, sizeof(Pst));

   gCb->init.lmPst.srcProcId = gCb->init.procId;
   gCb->init.lmPst.srcEnt = gCb->init.ent;
   gCb->init.lmPst.srcInst = gCb->init.inst;
   gCb->init.lmPst.event = EVTNONE;

   /* kw002.201 For multi core and multi region,no need to reinitiailize the 
    * region again . This would be done with kwActvInit from SSI */
#if !defined(SS_MULTICORE_SUPPORT) && !defined(SS_M_PROTO_REGION)
   gCb->init.region = cfg->lmPst.region;
#endif /* !defined(SS_MULTICORE_SUPPORT) && !defined(SS_M_PROTO_REGION) */
   gCb->init.pool   = cfg->lmPst.pool;

   gCb->init.cfgDone = TRUE;

   /* TODO : make this generic for DL and UL after support for UL throughput calculation is added */
   if(gCb->genCfg.rlcMode == LKW_RLC_MODE_DL)
   {
      /* Starting timer to print throughput */
      if((rlcChkTmr(gCb, (PTR)(&gCb->rlcThpt), EVENT_RLC_UE_THROUGHPUT_TMR)) == FALSE)
      {
         DU_LOG("\nINFO   --> RLC_DL : Starting UE Throughput timer");
         rlcStartTmr(gCb, (PTR)(&gCb->rlcThpt), EVENT_RLC_UE_THROUGHPUT_TMR);
      }
   }
      /* Starting timer to print throughput */
      if((rlcChkTmr(gCb, (PTR)(&gCb->rlcThpt), EVENT_RLC_SNSSAI_THROUGHPUT_TMR)) == FALSE)
      {
         DU_LOG("\nINFO   --> RLC_DL : Starting SNSSAI Throughput timer");
         rlcStartTmr(gCb, (PTR)(&gCb->rlcThpt), EVENT_RLC_SNSSAI_THROUGHPUT_TMR);
      }

   
   return (LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *     Sends confirmation message to LM Called by RlcMiRlcConfigReq function
 * 
 * @param[in] gCb    RLC Instance Control Block
 * @param[in] pst    Post structure
 * @param[in] cfm    RLC LM structure 
 * @param[in] type   Type of LM message
 * @param[in] hdr    Message header
 * 
 * @return  S16
 *     -# ROK
 */
static Void rlcLmmSendCfm
(
RlcCb      *gCb,
Pst        *pst,                
RlcMngmt   *cfm,               
uint8_t    type,              
Header     *hdr              
)
{
   Pst   rPst;   /* Reply post structure */

   RLC_MEM_SET(&rPst, 0, sizeof(Pst));   
 
   /* reply post structure for confirmation */
   if (gCb)
   {
      rPst.srcEnt = gCb->init.ent;
      rPst.srcInst = gCb->init.inst;
      cfm->hdr.entId.ent = gCb->init.ent;
      cfm->hdr.entId.inst = gCb->init.inst;
   }
   else
   {
      rPst.srcEnt = pst->dstEnt;
      rPst.srcInst = pst->dstInst;
      cfm->hdr.entId.ent = pst->dstEnt;
      cfm->hdr.entId.inst = pst->dstInst;
   }
   rPst.srcProcId = SFndProcId();
   rPst.dstEnt = pst->srcEnt;
   rPst.dstInst = pst->srcInst;
   rPst.dstProcId = pst->srcProcId;
   rPst.selector = hdr->response.selector;
   rPst.prior = hdr->response.prior;
   rPst.route = hdr->response.route;
   rPst.region = hdr->response.mem.region;
   rPst.pool= hdr->response.mem.pool;

   /* Fill the reply header */
   cfm->hdr.elmId.elmnt = hdr->elmId.elmnt;
   cfm->hdr.transId = hdr->transId;

   switch(type)
   {
      case TCFG:
         RlcMiRlcConfigCfm(&rPst,cfm);
         break;
      case TCNTRL:
         SGetDateTime(&(cfm->t.cntrl.dt));
         RlcMiLkwCntrlCfm(&rPst,cfm);
         break;
      case TSTS:
         SGetDateTime(&(cfm->t.sts.dt));
         RlcMiLkwStsCfm(&rPst,0,cfm);
         break;
      case TSSTA:
         SGetDateTime(&(cfm->t.ssta.dt));
         RlcMiLkwStaCfm(&rPst,cfm);
         break;
      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         DU_LOG("\nERROR  --> RLC : Received Invalid Message Type");
         if(!gCb)
         {
            fflush(stdout);
         }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         break;
   }

   return; 
} 


/**
 * @brief
 *    This function processes configuration requests received from the layer 
 *    manager.
 *
 * @details
 *    The configuration requests can be for general configuration, or 
 *    configuration of RGU, KWU, and CKW SAPs. The type of the configuration 
 *    request can be determined from the elmId field in the header of the layer
 *    manager message.This function is called by the layer manager to configure
 *    RLC. 
 *
 *    This function implements the following logic: 
 *
 *    - if genCfg is not done, send negative Cfm to the layer manager; 
 *
 *    - switch (cfg->hdr.elmId.elmnt)
 *       - case STGEN 
 *          - update the genCfg field in RlCb; 
 *          - allocate the maximum static memory required by the RLC product; 
 *          - register the timer service by calling SReqTmr; 
 *          - set CfgDone for not configuring again; 
 *       - case STKWUSAP 
 *          - if all the parameters are valid, then allocate the KwuSap control 
 *            block; 
 *          - update the SAP control block with the information provided in the 
 *          - configuration request; 
 *          - send configuration confirm with the status, success; 
 *       - case STCKWSAP 
 *          - if all the parametes are valid, then allocate the CkwSap control 
 *            block; 
 *          - update the SAP control block with the information provided in the
 *            configuration request; 
 *          - send configuration confirm with the status, success; 
 *       - case STRGUSAP 
 *          - if all the parametes are valid, then allocate the rguSap control 
 *            block; 
 *          - update the SAP control block with the information provided in the
 *            configuration request; 
 *          - send configuration confirm with the status, success; 
 *       - case STUDXSAP 
 *          - if all the parametes are valid, then allocate the udxSap control 
 *            block; 
 *          - update the SAP control block with the information provided in the
 *            configuration request; 
 *          - send configuration confirm with the status, success; 
 *    - end switch;
 *     
 * @param[in] pst   post structure 
 * @param[in] cfg   LM management structure 
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */

uint8_t RlcMiRlcConfigReq (Pst *pst,RlcMngmt *cfg)
{
   Reason   reason;   /* failure reason */
   RlcCb     *tRlcCb=NULLP;

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check if the instance is configured */
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      cfg->cfm.status = LCM_PRIM_NOK;
      cfg->cfm.reason = LCM_REASON_INVALID_INSTANCE;
      rlcLmmSendCfm(tRlcCb,pst, cfg, TCFG, &cfg->hdr);     
      return (RFAILED);
   }
#endif
   
   tRlcCb =  RLC_GET_RLCCB(pst->dstInst);

   if (!tRlcCb)
   {
      cfg->cfm.status = LCM_PRIM_NOK;
      cfg->cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      rlcLmmSendCfm(tRlcCb,pst, cfg, TCFG, &cfg->hdr);     
      return (RFAILED);
   }
   /* check configuration is done or not */
   if ((tRlcCb->init.cfgDone != TRUE) && 
       (cfg->hdr.elmId.elmnt != STGEN))
   {
      /*
       * if general config is not over then use pst structure
       * in primitive to communicate to stack manager
       */

      cfg->cfm.status = LCM_PRIM_NOK;
      cfg->cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      rlcLmmSendCfm(tRlcCb,pst, cfg, TCFG, &cfg->hdr);     
      return (RFAILED);
   }

   DU_LOG("\nDEBUG  --> RLC : RlcMiRlcConfigReq elmId(%d)", cfg->hdr.elmId.elmnt);

   switch(cfg->hdr.elmId.elmnt)
   {
      case STGEN:
      {
         reason = rlcLmmGenCfg(tRlcCb,&cfg->t.cfg.s.gen);
         break;
      }
      case STKWUSAP:
      {
         reason = rlcLmmCfgKwuSap(tRlcCb,&cfg->t.cfg.s.sap);
         break;
      }
      case STCKWSAP:
      {
         reason = rlcLmmCfgCkwSap(tRlcCb,&cfg->t.cfg.s.sap);
         break;
      }
      case STRGUSAP:
      {
         reason = rlcLmmCfgRguSap(tRlcCb,&cfg->t.cfg.s.sap);
         break;
      }
      case STUDXSAP:
      {
         reason = rlcLmmCfgUdxSap(tRlcCb,&cfg->t.cfg.s.sap);
         break;
      }
      default:
      {
         reason = LCM_REASON_INVALID_ELMNT;
         break;
      }   
   }

   if (reason == LCM_REASON_NOT_APPL)
   {
      cfg->cfm.status = LCM_PRIM_OK;
      cfg->cfm.reason = LCM_REASON_NOT_APPL;
      rlcLmmSendCfm(tRlcCb,pst, cfg, TCFG, &cfg->hdr);
      return (ROK);
   }
   else
   {
      cfg->cfm.status = LCM_PRIM_NOK;
      cfg->cfm.reason = reason;
      rlcLmmSendCfm(tRlcCb,pst, cfg, TCFG, &cfg->hdr);
      return (RFAILED);
   }
} 

/**
 * @brief 
 *    The layer manager initiates and uses the management-control procedure to 
 *    control RLC elements.The RLC control request primitive (RlcMiLkwCntrlReq) 
 *    can be called more than once and at any time after the 
 *    management-configuration procedure.The control request primitive 
 *    is confirmed by a RlcMiLkwCntrlCfm primitive.
 *
 * @details
 *    This function implements the following logic: 
 *  
 *    - if(cfgDone = FALSE) 
 *       - send negative confirmation to the layer manager; 
 *       - return; 
 *    - else 
 *    - switch(cntrl->hdr.elmId.elmnt) 
 *       -  case STGEN 
 *           - switch (action) 
 *           - case AENA 
 *                - switch(subAction) 
 *                - case SAUSTA 
 *                     - enable the unsolicited status flag; 
 *                     - send the control Cfm with success; 
 *                - case SADBG 
 *                     - set the debug mask; 
 *                     - send the control Cfm with success; 
 *                - case SATRC 
 *                     - enable the trace flag; 
 *                     - send the control Cfm with success; 
 *            - case ADISIMM 
 *               - switch(subAction) 
 *               - case SAUSTA 
 *                    - disable the unsolicited status flag; 
 *                    - send the control Cfm with success; 
 *               - case SATRC 
 *                    - disable the trace flag; 
 *                    - send the control Cfm with success; 
 *          - case ASHUTDOWN 
 *     - case STRGUSAP 
 *          - Call LSapCntrl
 *     - case STUDXSAP 
 *          - Call UdxSapCntrl
 *     - default  
 *    - end else  
 * 
 * @param[in] pst     post structure
 * @param[in] cntrl   LM management structure
 * 
 * @return S16
 *    -# ROK
 *    -# RFAILED
 */
uint8_t RlcMiLkwCntrlReq(Pst *pst, RlcMngmt *cntrl)
{
   Reason   reason;   /* failure reason */
   RlcCb     *tRlcCb=NULLP;

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check if the instance is configured */
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      cntrl->cfm.status = LCM_PRIM_NOK;
      cntrl->cfm.reason =  LCM_REASON_INVALID_INSTANCE;

      rlcLmmSendCfm(tRlcCb,pst, cntrl, TCNTRL, &cntrl->hdr);
      return (RFAILED);
   }
#endif
   
   tRlcCb =  RLC_GET_RLCCB(pst->dstInst);

   if(!tRlcCb)
   {
      cntrl->cfm.status = LCM_PRIM_NOK;
      cntrl->cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      rlcLmmSendCfm(tRlcCb,pst, cntrl, TCNTRL, &cntrl->hdr);
      return (RFAILED);
   }

   if (!(tRlcCb->init.cfgDone))
   {
      cntrl->cfm.status = LCM_PRIM_NOK;
      cntrl->cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      rlcLmmSendCfm(tRlcCb,pst, cntrl, TCNTRL, &cntrl->hdr);
      return (RFAILED);
   }
   DU_LOG("\nDEBUG  --> RLC : RlcMiLkwCntrlReq(elmId(%d))", cntrl->hdr.elmId.elmnt);

   /* In normal cases, LCM_REASON_NOT_APPL is returned in cfm.
    * In all error cases appropriate reason is returned
    * by the functions below
    */

   switch (cntrl->hdr.elmId.elmnt)
   {
      case STGEN:
      {
         /* general control */
         reason = rlcLmmGenCntrl(tRlcCb,cntrl);
         break;
      }
      case STRGUSAP:
      {
         /* Lower SAP control */
         reason = rlcLmmLSapCntrl(tRlcCb,cntrl);
         break;
      }
      case STUDXSAP:
      {
         /* Udx Sap cntrl */
         reason = rlcLmmUdxSapCntrl(tRlcCb,cntrl);
         break;
      }
      default:
      {
         reason = LCM_REASON_INVALID_ELMNT;
         break;
      }
   } /* end switch */
   
   if (reason == LCM_REASON_NOT_APPL)
   {
      cntrl->cfm.status = LCM_PRIM_OK;
      cntrl->cfm.reason = LCM_REASON_NOT_APPL;
   }
   else
   {
      cntrl->cfm.status = LCM_PRIM_NOK;
      cntrl->cfm.reason = reason;
   }

   rlcLmmSendCfm(tRlcCb,pst, cntrl, TCNTRL, &cntrl->hdr);

   return ROK;
} 

/**
 * @brief 
 *    This function processes solicited status requests received from the layer
 *    manager. The layer manager can request status information regarding the
 *    system ID, RGUSAP,KWUSAP, or CKWSAP.
 *
 * @param[in] pst   post structure
 * @param[in] sta   LM management structure
 * @return S16
 *    -# ROK (SUCCESS)
 *    -# RFAILED
 */
S16 RlcMiLkwStaReq(Pst *pst,RlcMngmt *sta)
{
   RlcMngmt   rSta;     /* Status */
   Reason    reason;   /* Failure reason */
   RlcCb      *tRlcCb=NULLP;

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check if the instance is configured */
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      sta->cfm.status = LCM_PRIM_NOK;
      sta->cfm.reason = LCM_REASON_INVALID_INSTANCE;

      rlcLmmSendCfm(tRlcCb,pst, sta, TSSTA, &sta->hdr);
      return (RFAILED);
   }
#endif
   
   tRlcCb =  RLC_GET_RLCCB(pst->dstInst);
   if (!tRlcCb)
   {
      sta->cfm.status = LCM_PRIM_NOK;
      sta->cfm.reason = LCM_REASON_GENCFG_NOT_DONE;

      rlcLmmSendCfm(tRlcCb,pst, sta, TSSTA, &sta->hdr);
      return (RFAILED);
   }

   DU_LOG("\nDEBUG  --> RLC : Status request for elmId(%d))", sta->hdr.elmId.elmnt);

   reason = LCM_REASON_NOT_APPL;

   RLC_MEM_SET(&rSta, 0, sizeof(RlcMngmt));
   switch (sta->hdr.elmId.elmnt)
   {
      case STSID:
      {
         reason = LCM_REASON_NOT_APPL;
         rlcGetSId(&rSta.t.ssta.s.sysId);
         break;
      }
      case STKWUSAP:
      {
         RLC_MEM_CPY (&rSta.t.ssta.s.rlckwuSap, 
                     &sta->t.ssta.s.rlckwuSap, 
                     sizeof (RlcKwuSapSta));

         reason = rlcLmmGetKwuSapSta (tRlcCb,&rSta.t.ssta.s.rlckwuSap);
         break;
      }
      case STRGUSAP:
      {
         RLC_MEM_CPY (&rSta.t.ssta.s.rguSap, 
                     &sta->t.ssta.s.rguSap,
                     sizeof (RlcRguSapSta));

         reason = rlcLmmGetRguSapSta (tRlcCb,&rSta.t.ssta.s.rguSap);
         break;
      }
      case STCKWSAP:
      {
         RLC_MEM_CPY (&rSta.t.ssta.s.ckwSap, 
                     &sta->t.ssta.s.ckwSap,
                     sizeof (RlcCkwCntSapSta));

         reason = rlcLmmGetCkwCntSapSta (tRlcCb,&rSta.t.ssta.s.ckwSap);
         break;
      }
      default:
      {
         reason = LCM_REASON_INVALID_ELMNT;
         break;
      }
   } /* end of switch */

   if (reason == LCM_REASON_NOT_APPL)
   {
      rSta.cfm.status = LCM_PRIM_OK;
      rSta.cfm.reason = LCM_REASON_NOT_APPL;
   }
   else
   {
      rSta.cfm.status = LCM_PRIM_NOK;
      rSta.cfm.reason = reason;
   }
   rlcLmmSendCfm(tRlcCb,pst, &rSta, TSSTA, &sta->hdr);

   return ROK;
} 

/**
 * @brief 
 *    This function processes statistics requests received from the layer
 *    manager.
 *
 * @details  
 *    After collecting the statistics, ir calls the statistics confirm function
 *    to send the statistics to the layer manager.
 *
 *    - switch(sts->hdr.elmId.elmnt) 
 *    - case STGEN 
 *       - get the general statistics from the KW control block; 
 *       - if (action = RESET) 
 *          - reset the general statistic field in the RlCb; 
 *          - call RlcMiLkwStsCfm to send statistics back to layer manager; 
 *    - case STCKWSAP 
 *    - case STKWUSAP 
 *    - case STRGUSAP 
 *        - get the SAP specific statistics from KW control block; 
 *        - if (action = RESET) 
 *       - reset the general statistic field in the RlCb; 
 *       - call RlcMiLkwStsCfm to send statistics to the layer manager; 
 *  -end switch;
 *
 * @param[in] pst      post structure
 * @param[in] action   action 
 * @param[in] sts      LM management structure
 * 
 * @return S16
 *    -# ROK
 *    -# RFAILED
 */
S16 RlcMiLkwStsReq (Pst *pst, Action action,RlcMngmt *sts)
{
   RlcMngmt   rSts;     /* Statistics */
   Reason    reason;   /* Reason for failure */
   RlcCb     *tRlcCb=NULLP;

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check if the instance is configured */
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      rSts.cfm.status = LCM_PRIM_NOK;
      rSts.cfm.reason = LCM_REASON_INVALID_INSTANCE;

      rlcLmmSendCfm(tRlcCb,pst, &rSts, TCNTRL, &sts->hdr);
      return (RFAILED);
   }
#endif
   
   tRlcCb =  RLC_GET_RLCCB(pst->dstInst);
   if (!tRlcCb)
   {
      rSts.cfm.status = LCM_PRIM_NOK;
      rSts.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;

      rlcLmmSendCfm(tRlcCb,pst, &rSts, TCNTRL, &sts->hdr);
      return (RFAILED);
   }

   DU_LOG("\nDEBUG  --> RLC : RlcMiLkwStsReq(elmId(%d),action(%d))", 
                       action, 
                       sts->hdr.elmId.elmnt);

   RLC_MEM_SET(&rSts, 0, sizeof(RlcMngmt));

   switch (sts->hdr.elmId.elmnt)
   {
      case STGEN:
      {
         reason = rlcLmmGetGenSts(tRlcCb,&rSts.t.sts.s.gen, action);
         break;
      }
      case STKWUSAP:
      case STCKWSAP:
      {
         if( sts->hdr.elmId.elmnt == STKWUSAP)
         {
            /* kw005.201, modified the element of rlckwuSap from suId to spId */
            rSts.t.sts.s.rlckwuSap.spId = sts->t.sts.s.rlckwuSap.spId;
         }
         reason = rlcLmmGetSapSts(tRlcCb,&rSts, sts->hdr.elmId.elmnt, action);
         break;
      }
      default:
      {
         reason = LCM_REASON_INVALID_ELMNT;
         break;
      }
   } /* end switch */

   if (reason == LCM_REASON_NOT_APPL)
   {
      rSts.cfm.status = LCM_PRIM_OK;
      rSts.cfm.reason = LCM_REASON_NOT_APPL;
   }
   else
   {
      rSts.cfm.status = LCM_PRIM_NOK;
      rSts.cfm.reason = reason;
   }
   rlcLmmSendCfm(tRlcCb,pst, &rSts, TSTS, &sts->hdr);

   return ROK;
} 

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
/**
@brief 
This function processes L2 Measurement requests received from the layer manager.
The L2 Measurement is start for a time period and after the timer expiry, the 
measurement confirm is sent.

-  Accept only one set of measurements.
-  Allocate and initialise RlcL2MeasEvtCb.
-  Validate if the measurement is already in progress.
   -  Loop through the existing measEvtCb with matching measType
      -  Check if measurement is running for any qci present in the measReq
      -  If present, move the qCi to list of invalid qCIs.
-  Set RlcL2MeasEvtCb.measOn to TRUE for QCIs to be measured.
-  For the number of measurements requested.
   -  For all Ues with RBs matching qCI.
      -  Add RbCb to the RB linked list.
      -  Set rlcRbCb.measOn to measurement type.
      -  If Meas type is DL_DELAY
         -  Update COUNT to startCount.
-  Start l2Timer

 *  @param[in] pst      post structure
 *  @param[in] action   action 
 *  @param[in] cfg      LM management structure
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/
S16 RlcMiLkwL2MeasReq(Pst *pst, RlcL2MeasReqEvt *measReqEvt)
{
   S16            ret = ROK;
   uint16_t       cntr;
   uint8_t        measType;
   RlcL2MeasCfmEvt measCfmEvt;

   RlcCb     *tRlcCb;

   tRlcCb =  RLC_GET_RLCCB(pst->dstInst);

   /* Initialize measCfmEvt */
   RLC_MEM_ZERO(&measCfmEvt, sizeof(RlcL2MeasCfmEvt));
   /* validate the received measReqEvt */
   /*LTE_L2_MEAS_PHASE2*/
   if(measReqEvt->measPeriod != 0)
   {
      /* L2 MEAS AGHOSH */
      if((measReqEvt->measReq.measType & LKW_L2MEAS_DL_IP)|| 
            (measReqEvt->measReq.measType & LKW_L2MEAS_UL_IP))
      {
         ret = rlcUtlValidateIpThL2Meas(measReqEvt, &measCfmEvt);
      }

      if(ret != ROK)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
#endif /* ERRCLASS & ERRCLS_ADD_RES */
         rlcUtlSndUlL2MeasNCfm(tRlcCb, measReqEvt, &measCfmEvt);
         RLC_FREE(tRlcCb, measReqEvt, sizeof(RlcL2MeasReqEvt))
         return RFAILED;
      }
   }

   measType = measReqEvt->measReq.measType;

   if((measType != LKW_L2MEAS_DL_IP) &&
      (measType !=  LKW_L2MEAS_ACT_UE) &&
      (measType !=  LKW_L2MEAS_DL_DISC) &&
      (measType != LKW_L2MEAS_DL_DELAY) && 
      (measType != LKW_L2MEAS_UU_LOSS) &&
      (measType !=  LKW_L2MEAS_UL_IP))/* || (qci > LKW_MAX_QCI)*/
   {
      measCfmEvt.transId = measReqEvt->transId;
      measCfmEvt.measType = measType;
      measCfmEvt.status.status = LCM_PRIM_NOK;
      measCfmEvt.status.reason = LKW_CAUSE_INVALID_MEASTYPE;
      rlcUtlSndDlL2MeasNCfm(tRlcCb, measReqEvt, &measCfmEvt);
      RLC_FREE(tRlcCb, measReqEvt, sizeof(RlcL2MeasReqEvt))
      return ROK;
   }

   /* for UL IP throughput meas enable for all QCIs */
   if(measType & LKW_L2MEAS_UL_IP)
   {
      for(cntr = 0; cntr < LKW_MAX_QCI; cntr++)
      {
         tRlcCb->u.ulCb->rlcL2Cb.measOn[cntr] |= LKW_L2MEAS_UL_IP;
      }
   }
  
   if(measType & 
      (LKW_L2MEAS_DL_IP | LKW_L2MEAS_DL_DISC | 
      LKW_L2MEAS_DL_DELAY | LKW_L2MEAS_UU_LOSS))
   {
      RlcL2MeasReqEvt *measEvt;
      Pst      *udxPst;

      udxPst = &(RLC_GET_UDX_SAP(tRlcCb)->pst);
      
      RLC_ALLOC_SHRABL_BUF(udxPst->region, 
			  udxPst->pool,
			  measEvt, 
			  sizeof(RlcL2MeasReqEvt));

#if (ERRCLASS & ERRCLS_ADD_RES) /* KW_FIX */ 
      if(measEvt == NULLP)
      {
         DU_LOG("\nERROR  --> RLC : Memory Allocation failed");   
         return RFAILED;
      }   
#endif      
      memcpy(measEvt, measReqEvt, sizeof(RlcL2MeasReqEvt));
      /*Redirect the request to DL task */
      /* NOTE:As of today, there are no cases where the Req will fail at DL
         as long as it reached the DL, so we don't wait for a confirmation from
         DL to send the confirmation to LM*/
      /* The interface for sending a confirmation back does not exist today;
         it needs to be created when the need arises */
      rlcUlUdxL2MeasReq(&(RLC_GET_UDX_SAP(tRlcCb)->pst),measEvt);
   }

   /* We need to copy the transId for sending back confirms later */
   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      RlcL2MeasEvtCb* measEvtCb = &(tRlcCb->u.ulCb->rlcL2Cb.rlcL2EvtCb[cntr]);
      if(measEvtCb->measCb.measType & measType)
      {
         measEvtCb->transId= measReqEvt->transId;
      }
   }
   /*RLC_FREE(tRlcCb, measReqEvt, sizeof(RlcL2MeasReqEvt));*/

   return (ret);
} /* RlcMiLkwL2MeasReq */

/**
@brief 
This function processes L2 Measurement stop request received from the layer manager.
After receving this request, RLC stops L2 Measurement
 *  @param[in] pst      post structure
 *  @param[in] measType meas Type 
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/

S16 RlcMiLkwL2MeasStopReq(Pst *pst,uint8_t measType)
{
   S16 ret = ROK;
   RlcL2MeasEvtCb *measEvtCb = NULLP;

   uint16_t            cntr;
   uint8_t             status = ROK;
   RlcCb     *tRlcCb;

   tRlcCb =  RLC_GET_RLCCB(pst->dstInst);

   /* reset the counter values for the measurement that is stopped */
   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      measEvtCb = &(tRlcCb->u.ulCb->rlcL2Cb.rlcL2EvtCb[cntr]);
      if(measEvtCb->measCb.measType & measType)
      {
         rlcUtlResetUlL2MeasInRlcRb(tRlcCb, &measEvtCb->measCb, measType);

      }
   }   

   /* for UL IP throughput meas disable for all QCIs */
   if(measType & LKW_L2MEAS_UL_IP)
   {
      for(cntr = 0; cntr < LKW_MAX_QCI; cntr++)
      {
         tRlcCb->u.ulCb->rlcL2Cb.measOn[cntr] &= ~LKW_L2MEAS_UL_IP;
      }
   }
   
   if((measType & LKW_L2MEAS_DL_IP) || (measType & LKW_L2MEAS_DL_DISC)
       || (measType & LKW_L2MEAS_DL_DELAY))
   {
      /*Redirect the request to DL task */
      rlcUlUdxL2MeasStopReq(&(RLC_GET_UDX_SAP(tRlcCb)->pst),measType);
      /*return ROK;*/
   }
   /*memset(&measCfmEvt, 0, sizeof(RlcL2MeasCfmEvt)); */

   status = LCM_PRIM_OK; 
   RlcMiLkwL2MeasStopCfm(&tRlcCb->genCfg.lmPst, measType,status); 
   
   return (ret);
}
/**
@brief 
This function processes L2 Measurement Send request received from the layer manager.
After receving this request, RLC sends L2 Measurement
 *  @param[in] pst      post structure
 *  @param[in] measType meas Type 
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/

S16 RlcMiLkwL2MeasSendReq(Pst *pst,uint8_t measType)
{
   /*S16 ret = ROK;*/
   RlcL2MeasEvtCb *measEvtCb = NULLP;
   uint16_t            cntr;
   RlcCb     *tRlcCb;

   tRlcCb =  RLC_GET_RLCCB(pst->dstInst);
   
   /* In case of addition of any new measType here ,appropriate handling 
    * has to be done in RLC DL (rlcUtlSndDlL2MeasCfm)*/
   if(measType & 
      (LKW_L2MEAS_DL_DISC | LKW_L2MEAS_DL_DELAY 
       | LKW_L2MEAS_UU_LOSS| LKW_L2MEAS_DL_IP))
   {
      /*Redirect the request to DL task */
      rlcUlUdxL2MeasSendReq(&(RLC_GET_UDX_SAP(tRlcCb)->pst),measType);
      /* L2 MEAS AGHOSH */
      /*return ROK;*/
   }

   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      measEvtCb = &(tRlcCb->u.ulCb->rlcL2Cb.rlcL2EvtCb[cntr]);
      /* L2 MEAS AGHOSH */
      if(measEvtCb->measCb.measType & measType)
      {
         rlcUtlHdlL2TmrExp(tRlcCb, measEvtCb);
      }
   }

   return ROK;
}
#endif /* LTE_L2_MEAS */

/**
 * @brief
 *    This function configures the RLC data sap
 *
 * @param[in]  gCb   RLC Instance Control Block
 * @param[in]  cfg   RLC LM Sap configuration structure
 *
 * @return  S16
 *    -# LCM_REASON_NOT_APPL  (SUCCESS)
 *    -# LCM_REASON_INVALID_SAP 
 *    -# LCM_REASON_MEM_NOAVAIL 
 *    -# LCM_REASON_RECONFIG_FAIL 
 */
static S16 rlcLmmCfgKwuSap(RlcCb *gCb,RlcSapCfg *cfg)
{
   RlcKwuSapCb   *rlcKwuSapCb;

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Validate the protocol parameters */
   if((cfg->sapId >= (S16)gCb->genCfg.maxKwuSaps) || (cfg->sapId < 0))
   {
      return (LCM_REASON_INVALID_PAR_VAL);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   rlcKwuSapCb = (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL) ?
              (RlcKwuSapCb*)(gCb->u.dlCb->rlcKwuDlSap + cfg->sapId):
              (RlcKwuSapCb*)(gCb->u.ulCb->rlcKwuUlSap + cfg->sapId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check for reconfiguration */
   if (rlcKwuSapCb->state != RLC_SAP_NOT_CFG)
   {
      DU_LOG("\nERROR  --> RLC : RLC Mode [%d] : Invalid rlckwuSap State [%d]",
            gCb->genCfg.rlcMode, rlcKwuSapCb->state);
      /* reconfiguration not allowed */
      return (LCM_REASON_RECONFIG_FAIL);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   /* Fill the parameters */
   RLC_FILL_SAP_HELPER(rlcKwuSapCb, cfg, gCb);

   return (LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    This function configures the RLC control sap
 *
 * @param[in]  gCb   RLC Instance Control Block
 * @param[in]  cfg   RLC LM Sap configuration structure
 * 
 * @return  S16
 *    -# LCM_REASON_NOT_APPL (SUCCESS)
 *    -# LCM_REASON_INVALID_SAP
 *    -# LCM_REASON_INVALID_SAP 
 *    -# LCM_REASON_RECONFIG_FAIL 
 */
static S16 rlcLmmCfgCkwSap(RlcCb  *gCb,RlcSapCfg *cfg)
{
   RlcCkwSapCb  *ckwSap;
   
#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Validate config parameters */
   if ((cfg->sapId >= RLC_MAX_CKWSAPS) || (cfg->sapId < 0) )
   {
      return (LCM_REASON_INVALID_PAR_VAL);
   }

   if (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL)
   {
      /* Get Sap control block */
      return (LCM_REASON_INVALID_PAR_VAL);
   }
#endif

   ckwSap = &(gCb->u.ulCb->ckwSap);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check for reconfiguration */
   if(ckwSap->state  != RLC_SAP_NOT_CFG)
   {
      DU_LOG("\nERROR  --> RLC : Invalid rlckwuSap State [%d]",ckwSap->state);
      return (LCM_REASON_RECONFIG_FAIL);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   
   /* Fill the parameters */
   RLC_FILL_SAP_HELPER(ckwSap, cfg, gCb);

   return (LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    This function configures the UDX sap
 *
 * @param[in]  gCb   RLC Instance Control Block
 * @param[in]  cfg   RLC LM Sap configuration structure
 *
 * @return  S16
 *    -# LCM_REASON_NOT_APPL   (SUCCESS)
 *    -# LCM_REASON_INVALID_SAP
 *    -# LCM_REASON_INVALID_SAP
 *    -# LCM_REASON_RECONFIG_FAIL
 */
static S16 rlcLmmCfgUdxSap(RlcCb *gCb,RlcSapCfg *cfg)
{
   RlcUdxDlSapCb   *udxDlSap;
   RlcUdxUlSapCb   *udxUlSap;

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Validate the protocol parameters */
   if((cfg->sapId >= RLC_MAX_UDXSAPS) || (cfg->sapId < 0))
   {
      return (LCM_REASON_INVALID_PAR_VAL);
   }
#endif
   /* Get Sap control block */
   if (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL)
   {
      udxDlSap = (gCb->u.dlCb->udxDlSap + cfg->sapId);
#if (ERRCLASS & ERRCLS_INT_PAR)
      /* Check for reconfiguration */
      if(udxDlSap->state != RLC_SAP_NOT_CFG)
      {
         DU_LOG("\nERROR  --> RLC : Invalid udxDlSap State [%d]",udxDlSap->state);
         return (LCM_REASON_RECONFIG_FAIL);
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

      /* Fill the parameters */
      RLC_FILL_SAP_HELPER(udxDlSap, cfg, gCb);
   }
   else
   {
      udxUlSap = (gCb->u.ulCb->udxUlSap + cfg->sapId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check for reconfiguration */
      if(udxUlSap->state != RLC_SAP_NOT_CFG)
      {
         DU_LOG("\nERROR  --> RLC : Invalid udxUlSap State [%d]", udxUlSap->state);
         return (LCM_REASON_RECONFIG_FAIL);
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

      /* Fill the parameters */
      RLC_FILL_SAP_HELPER(udxUlSap, cfg, gCb);

      udxUlSap->bndTmrInt = cfg->bndTmrIntvl;
      udxUlSap->retryCnt = 0;
      cmInitTimers(&(udxUlSap->bndTmr), 1);
   }

   return (LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    This function configures the RGU sap
 *
 * @param[in]  gCb   RLC Instance Control Block
 * @param[in]  cfg   RLC LM Sap configuration structure
 * @return  S16
 *    -# LCM_REASON_NOT_APPL    (SUCCESS)
 *    -# LCM_REASON_INVALID_SAP
 *    -# LCM_REASON_INVALID_SAP
 *    -# LCM_REASON_RECONFIG_FAIL
 */
static S16 rlcLmmCfgRguSap(RlcCb *gCb,RlcSapCfg *cfg)
{
   RlcRguSapCb   *rguSap;

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Validate the protocol parameters */
   if((cfg->sapId >= gCb->genCfg.maxRguSaps) || \
      (cfg->sapId < 0))
   {
      RLCLOGERROR(gCb,ERRCLS_INT_PAR, EKW047, (ErrVal)cfg->sapId,
               "rlcLmmCfgRguSap: Invalid RGU sapId\n");
      return (LCM_REASON_INVALID_PAR_VAL);
   }
#endif

   rguSap = (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL) ?
            &(gCb->u.dlCb->rguDlSap[cfg->sapId]): &(gCb->u.ulCb->rguUlSap[cfg->sapId]);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check for reconfiguration */
   if(rguSap->state != RLC_SAP_NOT_CFG)
   {
      DU_LOG("\nERROR  --> RLC : RLC Mode [%d]: Invalid rguSap State [%d]",
            gCb->genCfg.rlcMode, rguSap->state);
      return (LCM_REASON_RECONFIG_FAIL);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   /* Fill the parameters */
   RLC_FILL_SAP_HELPER(rguSap, cfg, gCb);
   rguSap->suId = cfg->sapId;
   rguSap->bndTmrInt = cfg->bndTmrIntvl;
   rguSap->retryCnt = 0;
   rguSap->state = RLC_SAP_CFG;

   cmInitTimers(&(rguSap->bndTmr), 1);

   return (LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    Validates the RLC general control parameters 
 *
 * @param[in] cntrl   RLC LM structure 
 * 
 * @return  S16
 *    -# LCM_REASON_NOT_APPL (SUCCESS)
 *    -# LCM_REASON_INVALID_SUBACTION
 *    -# LCM_REASON_INVALID_ACTION
 */
static S16 rlcLmmValidateGenCntrl(RlcMngmt   *cntrl)
{
   S16     reason;    /* reason for failure */
   uint8_t sAction;   /* subaction field */

   reason = LCM_REASON_NOT_APPL;
   sAction = cntrl->t.cntrl.subAction;

   switch (cntrl->t.cntrl.action)
   {
      case AENA:
      case ADISIMM:
         if ((sAction != SAUSTA) && 
             (sAction != SADBG) && 
             (sAction != SATRC))
         {
            reason = LCM_REASON_INVALID_SUBACTION;
         }
         break;
      case ASHUTDOWN:
         break;
      default:
         reason = LCM_REASON_INVALID_ACTION;
         break;
   }
   
   return (reason);
} 

/**
 * @brief
 *    it deregisters the timers and deregisters the rlckwuSap Control blocks 
 *
 * @param[in]  gCb   RLC Instance Control Block
 * 
 * @return  Void
 */
static Void rlcLmmCleanGblRsrcs(RlcCb *gCb)
{
   Size   rlcSapSize;
   Size   rlcUdxSapSize;
 
   if (gCb->init.cfgDone)
   {
      /* Deregister the timers */
/*Pradeep: changing the SDeregTmrMt() to SDeregTmr*/
      (Void) SDeregTmrMt(gCb->init.ent, gCb->init.inst,
                        (S16)gCb->genCfg.timeRes, rlcActvTmr);
      /*(Void) SDeregTmr(gCb->init.ent, gCb->init.inst,
                        (S16)gCb->genCfg.timeRes, rlcActvTmr);*/

      rlcSapSize = (Size)(gCb->genCfg.maxKwuSaps * sizeof(RlcKwuSapCb));

      if (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL)
      {
         rlcUdxSapSize = (Size)(gCb->genCfg.maxUdxSaps * sizeof(RlcUdxDlSapCb));
         if (gCb->u.dlCb)
         {
            gCb->u.dlCb->shutdownReceived = TRUE;
            rlcUtlFreeDlMemory(gCb); 
            if (gCb->u.dlCb->rlcKwuDlSap != NULLP)
            {
               RLC_FREE(gCb,gCb->u.dlCb->rlcKwuDlSap, rlcSapSize);
            }
            
            if(gCb->u.dlCb->udxDlSap != NULLP)
            {
               RLC_FREE(gCb,gCb->u.dlCb->udxDlSap, rlcUdxSapSize);
            }

            if(gCb->u.dlCb->rguDlSap != NULLP)
            {
               RLC_FREE(gCb,gCb->u.dlCb->rguDlSap, gCb->genCfg.maxRguSaps);
               gCb->genCfg.maxRguSaps = 0;
            }
             if (gCb->u.dlCb->shutdownReceived) 
            {
               if (gCb->u.dlCb->selfPstMBuf != NULL)
               {
                  ODU_PUT_MSG_BUF(gCb->u.dlCb->selfPstMBuf);
               }
               RLC_FREE(gCb,gCb->u.dlCb, sizeof (RlcDlCb));
            }
         }
         
      }
      else
      {
         rlcUdxSapSize = (Size)(gCb->genCfg.maxUdxSaps * sizeof(RlcUdxUlSapCb));
         if (gCb->u.ulCb)
         {
            if (gCb->u.ulCb->rlcKwuUlSap != NULLP)
            {
               RLC_FREE(gCb,gCb->u.ulCb->rlcKwuUlSap, rlcSapSize);
            }
            
            if(gCb->u.ulCb->udxUlSap != NULLP)
            {
               RLC_FREE(gCb,gCb->u.ulCb->udxUlSap, rlcUdxSapSize);
            }
         
            if(gCb->u.ulCb->rguUlSap != NULLP)
            {
               RLC_FREE(gCb,gCb->u.ulCb->rguUlSap, gCb->genCfg.maxRguSaps);
               gCb->genCfg.maxRguSaps = 0;
            }
         
            RLC_FREE(gCb,gCb->u.ulCb, sizeof (RlcUlCb));
         }
      }

      gCb->init.cfgDone = FALSE;
      gCb->init.acnt = FALSE;
      gCb->init.trc = FALSE;

      gCb->init.usta = FALSE;
   }

   return;
}

/**
 * @brief
 *    Delete all SAPs and Control Blocks
 *
 * @details
 *    Shutdown of RLC happens at modules level, tm, um and am modules will be
 *    shutdown using utility functin and dbm shutdown will clean up the SAPs,
 *    control blocks and lists. 
 *
 * @params[in] gCb   RLC instance control block   
 *
 * @return  S16
 *    -# LCM_REASON_NOT_APPL (SUCCESS)
 *    -# LCM_REASON_HASHING_FAILED 
 */
static S16 rlcLmmShutdown(RlcCb *gCb)
{
   RlcRguSapCb   *rguSap;
   uint32_t          idx;

   if (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL)
   {
      for (idx = 0; idx < gCb->genCfg.maxRguSaps; idx++)
      {
         rguSap = &(gCb->u.dlCb->rguDlSap[idx]);
         if ((rguSap->state == RLC_SAP_BND) || (rguSap->state == RLC_SAP_BINDING))
         {
            RlcLiRguUbndReq(&(rguSap->pst), rguSap->spId, 0);
         }
      }
   }
   else
   {
      for (idx = 0; idx < gCb->genCfg.maxRguSaps; idx++)
      {
         rguSap = &(gCb->u.ulCb->rguUlSap[idx]); 
         if ((rguSap->state == RLC_SAP_BND) || (rguSap->state == RLC_SAP_BINDING))
         {
            RlcLiRguUbndReq(&(rguSap->pst), rguSap->spId, 0);
         }
      }
   }

   if(gCb->genCfg.rlcMode == LKW_RLC_MODE_DL)
   {
      rlcDbmDlShutdown(gCb); 
      rlcDelTputSnssaiList(gCb, DIR_DL);
   }
   else
   {
      rlcDbmUlShutdown(gCb);
      rlcDelTputSnssaiList(gCb, DIR_UL);
   }

   rlcLmmCleanGblRsrcs(gCb);

   RLC_MEM_SET (&(gCb->genSts), 0, sizeof (RlcGenSts));

   return (LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    Function processes the general control request 
 *
 * @param[in] gCb     RLC instance control block
 * @param[in] cntrl   RLC LM structure
 *
 * @return  S16
 *    -# LCM_REASON_NOT_APPL (SUCCESS)
 *    -# LCM_REASON_INVALID_SUBACTION
 *    -# LCM_REASON_INVALID_ACTION
 */
static S16 rlcLmmGenCntrl(RlcCb *gCb,RlcMngmt *cntrl)
{
   RlcTrcCntrl *trcCntrl; /* trace */
#ifdef DEBUGP
   RlcDbgCntrl *dbgCntrl; /* debug */
#endif /* DEBUGP */
   S16   reason;         /* reason for failure */
   
   /* Validate control parameters */
   reason = rlcLmmValidateGenCntrl (cntrl);

   if (reason != LCM_REASON_NOT_APPL)
   {
      return (reason);
   }

   switch(cntrl->t.cntrl.action)
   {
      case AENA:
      {
         switch(cntrl->t.cntrl.subAction)
         {
            case SAUSTA:
            {
               RLC_SET_USTA_FLAG(gCb, TRUE);
               break;
            }
            case SADBG:
            {
#ifdef DEBUGP
               dbgCntrl = &cntrl->t.cntrl.s.dbgCntrl;
               RLC_GET_DBG_MASK(gCb) |= dbgCntrl->dbgMask;
#endif /* DEBUGP */
               break;
            }
            case SATRC:
            {
               trcCntrl = &cntrl->t.cntrl.s.trcCntrl;
               gCb->init.trc = TRUE;
               gCb->trcLen = trcCntrl->trcLen;
               (gCb->trcMask) |= trcCntrl->trcMask;
                break;
            }
         }
         break;
      }
      case ADISIMM:
      {
         switch(cntrl->t.cntrl.subAction)
         {
            case SAUSTA:
            {
               gCb->init.usta = FALSE;
            }
            break;
            case SADBG:
            {
#ifdef DEBUGP
               dbgCntrl = &cntrl->t.cntrl.s.dbgCntrl;
               RLC_GET_DBG_MASK(gCb) &= ~(dbgCntrl->dbgMask);
#endif /* DEBUGP */
            }
            break;
            case SATRC:
            {
               trcCntrl = &cntrl->t.cntrl.s.trcCntrl;
               gCb->init.trc = FALSE;
               gCb->trcMask &= ~(trcCntrl->trcMask);
            }
            break;
         }
         break;
      }
      case ASHUTDOWN:
      {
         reason = rlcLmmShutdown(gCb);
         break;
      }
   }
   return (reason);
} 

/**
 * @brief
 *    Bind/Unbind RLC UDX sap 
 *
 * @param[in]  gCb     RLC Instance Control Block
 * @param[in]  cntrl   RLC LM structure
 * 
 * @return  S16
 *    -# LCM_REASON_NOT_/PPL (SUCCESS)    
 *    -# LCM_REASON_INVALID_SAP 
 *    -# LCM_REASON_INVALID_STATE
 *    -# LCM_REASON_INVALID_ACTION 
 */
static S16 rlcLmmUdxSapCntrl(RlcCb *gCb,RlcMngmt *cntrl)
{

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL)
   {
      return (LCM_REASON_INVALID_SAP);
   }
   /* validate SuId */
   if((cntrl->t.cntrl.s.sapCntrl.suId < 0) || 
      (cntrl->t.cntrl.s.sapCntrl.suId >= RLC_MAX_UDXSAPS))
   {
      return (LCM_REASON_INVALID_SAP);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

#define UDX_SAP  gCb->u.ulCb->udxUlSap[cntrl->t.cntrl.s.sapCntrl.suId]
#if (ERRCLASS & ERRCLS_INT_PAR)
   /* validate SAP */
   if(UDX_SAP.state == RLC_SAP_NOT_CFG)
   {
      DU_LOG("\nERROR  --> RLC : udxUlSap not configured yet");
      return (LCM_REASON_INVALID_STATE);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   switch(cntrl->t.cntrl.action)
   {
      case ABND:
      {
         if(UDX_SAP.state != RLC_SAP_BND)
         {
            /* start timer to wait for bind confirm */
            rlcStartTmr(gCb,(PTR)(&UDX_SAP), EVENT_RLC_WAIT_BNDCFM);
            UDX_SAP.state = RLC_SAP_BINDING;
            rlcUlUdxBndReq(&(UDX_SAP.pst), UDX_SAP.suId, UDX_SAP.spId);
         }
         else
         {
            /* control request received for an already bound SAP */
            return (LCM_REASON_INVALID_STATE);
         }
      }
      break;
      case AUBND:
      {
         /* make the state of UDXSAP configured but not bound */
         UDX_SAP.state = RLC_SAP_CFG;
         rlcUlUdxUbndReq(&(UDX_SAP.pst), UDX_SAP.spId, 0);
      }
      break;
      default:
         return (LCM_REASON_INVALID_ACTION);
         break;
   } /* end of switch */
#undef UDX_SAP
   return (LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    Bind/Unbind RLC lower sap 
 *
 * @param[in]  gCb     RLC Instance Control Block
 * @param[in]  cntrl   RLC LM structure
 * @return  S16
 *    -# LCM_REASON_NOT_APPL       (SUCCESS)
 *    -# LCM_REASON_INVALID_SAP 
 *    -# LCM_REASON_INVALID_STATE
 *    -# LCM_REASON_INVALID_ACTION 
 */
static S16 rlcLmmLSapCntrl(RlcCb *gCb,RlcMngmt *cntrl)
{
   RlcRguSapCb   *rguSap;   /* rgu sap pointer */

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* validate SuId */
   if((cntrl->t.cntrl.s.sapCntrl.suId < 0) || 
      (cntrl->t.cntrl.s.sapCntrl.suId >= gCb->genCfg.maxRguSaps))
   {
      RLCLOGERROR(gCb,ERRCLS_INT_PAR, EKW047, (ErrVal)cntrl->t.cntrl.s.sapCntrl.suId,
               "rlcLmmLSapCntrl: Invalid RGU suId\n");
      return (LCM_REASON_INVALID_SAP);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   rguSap = (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL) ?
            &(gCb->u.dlCb->rguDlSap[cntrl->t.cntrl.s.sapCntrl.suId]): \
            &(gCb->u.ulCb->rguUlSap[cntrl->t.cntrl.s.sapCntrl.suId]);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* validate SAP */
   if(rguSap->state == RLC_SAP_NOT_CFG)
   {
      DU_LOG("\nERROR  --> RLC : RLC Mode [%d]:rguSap not configured yet", gCb->genCfg.rlcMode);
      return (LCM_REASON_INVALID_STATE);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   switch(cntrl->t.cntrl.action)
   {
      case ABND:
      {
         if(rguSap->state != RLC_SAP_BND)
         {
            /* start timer to wait for bind confirm */
            rlcStartTmr(gCb,(PTR)(rguSap), EVENT_RLC_WAIT_BNDCFM);
            rguSap->state = RLC_SAP_BINDING;
            rguSap->spId = cntrl->t.cntrl.s.sapCntrl.spId;
            rguSap->suId = cntrl->t.cntrl.s.sapCntrl.suId;
            RlcLiRguBndReq(&(rguSap->pst), rguSap->suId, rguSap->spId);
         }
         else
         {
            /* control request received for an already bound SAP */
            return (LCM_REASON_INVALID_STATE);
         }
      }
      break;
      case AUBND:
      {
         /* make the state of RGUSAP is configure but not bound */
         rguSap->state = RLC_SAP_CFG;
         RlcLiRguUbndReq(&(rguSap->pst), rguSap->spId, 0);
      }
      break;
      default:
         return (LCM_REASON_INVALID_ACTION);
         break;
   } /* end of switch */

   return (LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    Function gather the general KWU SAP status 
 *
 * @param[in] gCb   RLC Instance Control Block
 * @param[in] sta   LM KWU Sap Status structure 
 * 
 * @return  S16
 *    -# LCM_REASON_NOT_APPL       (SUCCESS)
 *    -# LCM_REASON_INVALID_PAR_VAL(FAILURE)
 */
static S16 rlcLmmGetKwuSapSta (RlcCb *gCb,RlcKwuSapSta *sta)
{
   RlcKwuSapCb   *rlcKwSapCb;
   
#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Validate the protocol parameters */
   if ((sta->spId >= (S16)gCb->genCfg.maxKwuSaps)||
       (sta->spId < 0))
   {
      return (LCM_REASON_INVALID_PAR_VAL);
   }
#endif

   /* Get Sap control block */
   rlcKwSapCb = (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL)? 
             (RlcKwuSapCb*)(gCb->u.dlCb->rlcKwuDlSap + sta->spId):
             (RlcKwuSapCb*)(gCb->u.ulCb->rlcKwuUlSap + sta->spId);

   sta->state = rlcKwSapCb->state;

   return (LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    Function gather the general RGU SAP status
 *
 * @param[in]  gCb   RLC Instance Control Block
 * @param[in]  sta   LM RGU Sap Status structure
 * @return  S16
 *    -# LCM_REASON_NOT_APPL       (SUCCESS) 
 *    -# LCM_REASON_INVALID_PAR_VAL(FAILURE) 
 */
static S16 rlcLmmGetRguSapSta(RlcCb *gCb,RlcRguSapSta *sta)
{

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Validate the protocol parameters */
   if((sta->suId >= RLC_MAX_RGUSAPS) || (sta->suId < 0))
   {
      return (LCM_REASON_INVALID_PAR_VAL);
   }
#endif

   sta->state = (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL) ? 
                 gCb->u.dlCb->rguDlSap[sta->suId].state : 
                 gCb->u.ulCb->rguUlSap[sta->suId].state ;

   return (LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    Function gather the general CKW SAP status
 *
 * @param[in]  gCb   RLC Instance Control Block
 * @param[in]  sta   LM CKW Sap Status structure
 * @return  S16
 *    -# LCM_REASON_NOT_APPL       (SUCCESS)
 *    -# LCM_REASON_INVALID_PAR_VAL(FAILURE)
 */
static S16 rlcLmmGetCkwCntSapSta(RlcCb *gCb,RlcCkwCntSapSta *sta)
{

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Validate config parameters */
   if ((sta->spId >= RLC_MAX_CKWSAPS ) ||
       (sta->spId < 0 ) ||
       (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL))
   {
      return (LCM_REASON_INVALID_PAR_VAL);
   }
#endif
   sta->state =  gCb->u.ulCb->ckwSap.state;

   return (LCM_REASON_NOT_APPL);

} 

/**
 * @brief
 *    Gather the general statistics 
 *
 * @param[in]  gCb      RLC Instance Control Block
 * @param[in]  sts      LM general statistics structure
 * @param[in]  action   action
 * @return  S16
 *    -# LCM_REASON_NOT_APPL       (SUCCESS)
 *    -# LCM_REASON_INVALID_PAR_VAL(FAILURE)
 */
static S16 rlcLmmGetGenSts(RlcCb *gCb,RlcGenSts *sts,Action action)
{

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Validate protocol parameters */
   if ((action != LKW_ZEROSTS) && (action != LKW_NOZEROSTS))
   {
      return (LCM_REASON_INVALID_PAR_VAL);
   }
#endif

   *sts = gCb->genSts;
   
   if(action == LKW_ZEROSTS)
   {
      RLC_MEM_SET (&(gCb->genSts), 0, sizeof (RlcGenSts));
   }

   return (LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    Gather the SAP statistics
 *
 * @param[in]  gCb      RLC Instance Control Block
 * @param[in]  sts      LM general statistics structure
 * @param[in]  elmnt    element 
 * @param[in]  action   action 
 * 
 * @return  S16
 *    -# LCM_REASON_NOT_APPL (SUCCESS)
 *    -# LCM_REASON_INVALID_PAR_VAL (FAIL)
 */
static S16 rlcLmmGetSapSts(RlcCb *gCb,RlcMngmt *sts,Elmnt elmnt, Action action)
{

   /* Validate protocol parameters */
   if (action != LKW_ZEROSTS && action != LKW_NOZEROSTS)
   {
      return (LCM_REASON_INVALID_PAR_VAL);
   }
   switch(elmnt)
   {
      case STKWUSAP:
      {
         RlcKwuSapSts  *rlckwuSap; 
         RlcKwuSapCb   *rlcKwuSapCb;    
         rlckwuSap = &sts->t.sts.s.rlckwuSap;

         /* Get Sap control block */
         /* kw005.201, modified the element of rlckwuSap from suId to spId */
         rlcKwuSapCb = (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL)? 
              (RlcKwuSapCb*)(gCb->u.dlCb->rlcKwuDlSap + rlckwuSap->spId):
              (RlcKwuSapCb*)(gCb->u.ulCb->rlcKwuUlSap + rlckwuSap->spId);


         *rlckwuSap = rlcKwuSapCb->sts;

         if (action == LKW_ZEROSTS)
         {
            RLC_MEM_SET (&rlcKwuSapCb->sts, 0, sizeof (RlcKwuSapSts));
         }
         break;
      }
      /* kw005.201 removed RGUSAP statistics support */
      case STCKWSAP:
      {
         RlcCkwCntSts  *ckwSap;  
         ckwSap = &sts->t.sts.s.ckwSap;
         if (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL) 
         {
            return (LCM_REASON_INVALID_PAR_VAL);
         }

         ckwSap->statMsgs = gCb->u.ulCb->ckwSap.sts.statMsgs;

         if (action == LKW_ZEROSTS)
         {
            RLC_MEM_SET (&(gCb->u.ulCb->ckwSap.sts), 0, sizeof (RlcCkwCntSts));
         }
         break;
      }
      default:
         return (LCM_REASON_INVALID_ELMNT);
   }

   SGetDateTime(&sts->t.sts.dt);

   return (LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    This function sends Unsolicited Status Indication to the Layer Management
 *    Entity. 
 *
 * @param[in]  gCb        RLC Instance Control Block
 * @param[in]  category   Category 
 * @param[in]  event      event
 * @param[in]  cause      cause
 * @param[in]  UeId       ueId 
 * @param[in]  Qci        qci 
 * 
 * @return  Void
 */
#ifdef LTE_L2_MEAS
Void rlcLmmSendAlarm
(
RlcCb      *gCb,
uint16_t    category,  
uint16_t    event,    
uint16_t    cause,   
SuId        suId,   
uint32_t    ueId,  
uint8_t     qci   
)
#else /* LTE_L2_MEAS */
Void rlcLmmSendAlarm
(
RlcCb       *gCb,
uint16_t    category, 
uint16_t    event,   
uint16_t    cause,  
SuId        suId,  
uint32_t    ueId  
)
#endif /* LTE_L2_MEAS */
{
   RlcMngmt   usta;   /* Rlc Management Structure */

   if(gCb->init.usta == FALSE)
   {
      return;
   }

   usta.hdr.elmId.elmnt = STGEN;
   usta.hdr.entId.ent = gCb->init.ent;
   usta.hdr.entId.inst = gCb->init.inst;

   /* fill in the event and category */
   usta.t.usta.alarm.category = category;
   usta.t.usta.alarm.event = event;
   usta.t.usta.alarm.cause = cause;

   /* set the suId and ueId */
   usta.t.usta.ueId = ueId;
   usta.t.usta.suId = suId;
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   if(event == LKW_EVT_MEAS_HALT)
   {
      usta.t.usta.qci = qci;
   }
#endif /* LTE_L2_MEAS */
   /* update the date and time */
   (Void) SGetDateTime(&usta.t.usta.alarm.dt);

   RlcMiLkwStaInd(&(gCb->init.lmPst), &usta);

   return;

} 

/**
 * @brief
 *    This function sends trace indication to LM 
 *
 * @param[in]  gCb     RLC Instance Control Block
 * @param[in]  event   event
 * @param[in]  mBuf    meessage buffer
 *
 * @return S16
 *    -# ROK 
 *    -# RFAILED 
 */
S16 rlcLmmSendTrc(RlcCb *gCb,Event event,Buffer *mBuf)
{
   RlcMngmt   trc;              /* RLC management control block */
   Buffer    *dstMbuf;   
   MsgLen    bufLen;
   Data      *tempBuf;
   MsgLen    tempCnt;
   Pst       pst;

   DU_LOG("\nDEBUG  --> RLC : rlcLmmSendTrc(): Trace for event=%d, gCb->trcLen=%d",
                     event,
                     gCb->trcLen);

   memset(&trc, 0, sizeof(RlcMngmt));

   pst = gCb->init.lmPst;

   trc.t.trc.event = event;
   SGetDateTime(&trc.t.trc.dt);
   trc.cfm.status = LCM_PRIM_OK;
   trc.cfm.reason = LCM_REASON_NOT_APPL;
         
   if(mBuf != NULLP)
   {
     /* Check if the whole buffer is to be sent in Trace indication */
     if(gCb->trcLen == LKW_FULL_TRACE)
     {
        if (SCpyMsgMsg(mBuf, pst.region, pst.pool, &dstMbuf)
            != ROK)
        {
           /* rg005.201 removed SPutSBuf on error */
           DU_LOG("\nERROR  --> RLC : SCpyMsgMsg Failed");   
           return RFAILED;
        }
        /* Send Trace Indication to Layer manager */
        RlcMiLkwTrcInd(&pst, &trc, dstMbuf);
     }
     /* check if only a specified number of bytes are to be sent */
     else if(gCb->trcLen > 0)
     {
        /* Get the length of the recvd message buffer */
        if (SFndLenMsg(mBuf, &bufLen) != ROK)
        {
           DU_LOG("\nERROR  --> RLC : SFndLenMsg Failed");   
           return RFAILED;
        }
        /* Check if the recvd buffer size is less than request trace len */
        if(bufLen < gCb->trcLen)
        {
           /* Copy the whole of the recvd buffer in trace indication */
      
           if (SCpyMsgMsg(mBuf, pst.region, pst.pool, &dstMbuf)
              != ROK)
           {
              DU_LOG("\nERROR  --> RLC : SCpyMsgMsg Failed");   
              return RFAILED;
           }
         
           /* Send Trace Indication to Layer manager */
           RlcMiLkwTrcInd(&pst, &trc, dstMbuf);
        }
        else
        {
           /* if the recvd buffer size is greater than request trace len */
           /* Get a temporary buffer to store the msg */
           RLC_ALLOC(gCb,tempBuf, gCb->trcLen);

#if (ERRCLASS & ERRCLS_INT_PAR)
           if(tempBuf == NULLP)
           {
              (Void) SPutSMem(RLC_GET_MEM_REGION(gCb), RLC_GET_MEM_POOL(gCb));
      
               DU_LOG("\nERROR  --> RLC : Memory Allocation failed");   
               return (LCM_REASON_MEM_NOAVAIL);
           }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         
           /* Copy trcLen nos of bytes from the recvd message */
           if (SCpyMsgFix(mBuf,0,gCb->trcLen,tempBuf,&tempCnt) != ROK)   
           {
              DU_LOG("\nERROR  --> RLC : SCpyMsgFix Failed");   
              return RFAILED;
           }

           if (ODU_GET_MSG_BUF(pst.region, pst.pool, &dstMbuf) != ROK)
           {
              DU_LOG("\nERROR  --> RLC : Memory Allocation failed");   
              return RFAILED;
           }
           /* Copy the tempBuf data to dst mBuf */
           if (SCpyFixMsg(tempBuf,dstMbuf,0,gCb->trcLen,&tempCnt) != ROK)
           {
              DU_LOG("\nERROR  --> RLC : SCpyMsgFix Failed");   
              return RFAILED;
           }
  
           /* Free the memory allocated for tempBuf */
           RLC_FREE(gCb,tempBuf, gCb->trcLen);
           /* Send Trace Indication to Layer manager */
           RlcMiLkwTrcInd(&pst, &trc, dstMbuf);
        }
     }
   }
   else
   {
      RlcMiLkwTrcInd(&pst, &trc, mBuf);
   }

   return ROK;
} 

  
/*
 * @brief
 *    Activate Task - timer
 *
 * @details     
 *    Invoked by system services to activate a task with a timer tick.
 *
 * @param[in] ent    entity
 * @param[in] inst   instance
 *
 * @return S16
 *    -# ROK
 *    -# RFAILED
 *
 */
 
S16 rlcActvTmr(Ent ent,Inst inst)
{
   RlcCb   *gCb; 

   if (inst >= MAX_RLC_INSTANCES)
   {
      return  (RFAILED);
   }
   gCb = RLC_GET_RLCCB(inst); 
   cmPrcTmr(&(gCb->rlcTqCp), gCb->rlcTq, (PFV) rlcTmrExpiry);
   return (ROK);

} /* end of rlcActvTmr */

 
/********************************************************************30**

         End of file
**********************************************************************/
