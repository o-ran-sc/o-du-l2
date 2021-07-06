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
  
     Desc:     C source code for Layer Manager Interface Module 
  
     File:     rg_lmm.c 
  
**********************************************************************/

/** @file rg_lmm.c
@brief This file contains the Layer Management interface module implementation.
       The functions for the configuration, control, status and statistics 
       request primitives are defined here.
*/

/* header include files (.h) */
#include "common_def.h"
#include "rg_env.h"        /* MAC Environment Defines */
#include "crg.h"           /* CRG Interface defines */
#include "rgu.h"           /* RGU Interface defines */
#include "tfu.h"           /* RGU Interface defines */
#include "rg_sch_inf.h"    /* RGR Interface defines */
#include "lrg.h"           /* LRG Interface defines */
#include "rgr.h"           /* LRG Interface defines */
#include "rg.h"            /* MAC defines */
#include "rg_err.h"        /* MAC error defines */
#include "mac_utils.h"

/* header/extern include files (.x) */
#include "crg.x"           /* CRG Interface includes */
#include "rgu.x"           /* RGU Interface includes */
#include "tfu.x"           /* RGU Interface includes */
#include "rg_sch_inf.x"    /* SCH Interface includes */
#include "rg_prg.x"    /* PRG Interface includes */
#include "lrg.x"           /* LRG Interface includes */
#include "rgr.x"           /* LRG Interface includes */
#include "du_app_mac_inf.h"
#include "rg.x"            /* MAC includes */
#ifdef SS_DIAG
#include "ss_diag.h"        /* Common log file */
#endif
#include "ss_rbuf.h"
#include "ss_rbuf.x"
#include "mac_sch_interface.h"
#include "lwr_mac_upr_inf.h"
#include "mac.h"
#include "lwr_mac_fsm.h"
#include "lwr_mac_phy.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
Void rgGetSId ARGS((SystemId *s));
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* Public variable declaration */
MacCb  macCb;

/* forward references */
static uint16_t rgLMMGenCfg ARGS((
   Inst           inst,
   RgCfg          *cfg           
));

static uint16_t rgLMMSapCfg ARGS((
   Inst           inst,
   RgCfg          *cfg,
   Elmnt          sapType
));

static Void rgLMMShutdown ARGS((
   Inst           inst
));

static Void rgLMMFillCfmPst ARGS((
   Pst           *reqPst,
   Pst           *cfmPst,
   RgMngmt       *cfm
));

static Void rgLMMGenCntrl ARGS((
RgMngmt       *cntrl,
RgMngmt       *cfm,
Pst           *cfmPst
));

static Void rgLMMSapCntrl ARGS((
RgMngmt       *cntrl,
RgMngmt       *cfm,
Pst           *cfmPst
));


/**
 * @brief Task Initiation callback function. 
 *
 * @details
 *
 *     Function : rgActvInit
 *     
 *     This function is supplied as one of parameters during MAC's 
 *     task registration. SSI will invoke this function once, after
 *     it creates and attaches this TAPA Task to a system task.
 *     
 *  @param[in]  Ent entity, the entity ID of this task.     
 *  @param[in]  Inst inst, the instance ID of this task.
 *  @param[in]  Region region, the region ID registered for memory 
 *              usage of this task.
 *  @param[in]  Reason reason.
 *  @return  S16
 *      -# ROK
 **/
S16 rgActvInit
(
Ent entity,            /* entity */
Inst inst,             /* instance */
Region region,         /* region */
Reason reason          /* reason */
)
{
   Inst macInst ;

   RG_IS_INST_VALID(inst);

   macInst = inst - RG_INST_START;
   /* Initialize the MAC TskInit structure to zero */
   memset (&rgCb[macInst], 0, sizeof(RgCb));

   /* Initialize the MAC TskInit with received values */
   rgCb[macInst].rgInit.ent = entity;
   rgCb[macInst].rgInit.inst = inst;
   rgCb[macInst].rgInit.region = region;
   rgCb[macInst].rgInit.pool = 0;
   rgCb[macInst].rgInit.reason = reason;
   rgCb[macInst].rgInit.cfgDone = FALSE;
   rgCb[macInst].rgInit.acnt = FALSE;
   rgCb[macInst].rgInit.usta = FALSE;
   rgCb[macInst].rgInit.trc = FALSE;
   rgCb[macInst].trcLen = 0; 
#ifdef DEBUGP
#ifdef RG_DEBUG
   /* disabling debugs by default */
   rgCb[macInst].rgInit.dbgMask = 0xffffffff; 
#endif
#endif /* DEBUGP */
#ifdef SS_DIAG
   rgCb[macInst].rgInit.logMask = 0x0;
#endif
   rgCb[macInst].rgInit.procId = SFndProcId();
   rgCb[macInst].tfuSap.numBndRetries = 0;

   /* Initialize Sap state */
   rgCb[macInst].tfuSap.sapSta.sapState = LRG_NOT_CFG;
   rgCb[macInst].crgSap.sapSta.sapState = LRG_NOT_CFG;
   rgCb[macInst].rguSap = NULLP;
   rgCb[macInst].crgSap.sapSta.sapState = LRG_NOT_CFG;

   rgCb[macInst].inactiveCell = NULLP;
   rgCb[macInst].cell         = NULLP;
#ifdef SS_RBUF
   SAttachSRngBuf(SS_RNG_BUF_ULMAC_TO_ULRLC, SS_RBUF_ENT_ULMAC,SS_RNG_TX);
   SAttachSRngBuf(SS_RNG_BUF_ULMAC_TO_ULRLC, SS_RBUF_ENT_ULRLC,SS_RNG_RX);
#endif

   /* Initialize Scheduler as well */
   schActvInit(ENTMAC, (DEFAULT_CELLS + SCH_INST_START), DFLT_REGION, PWR_UP);

   /* Initialize lower mac */
   lwrMacLayerInit(region, 0);

   return ROK;

} /* rgActvInit */

/**
 * @brief Layer Manager Configuration request handler. 
 *
 * @details
 *
 *     Function : RgMiLrgCfgReq
 *     
 *     This function handles the configuration
 *     request received from the Layer Manager.
 *     -# Based on the cfg->hdr.elmId.elmnt value it invokes one of the
 *        functions rgHdlGenCfg() or rgHdlSapCfg().
 *     -# Invokes RgMiLrgCfgCfm() to send back the confirmation to the LM.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfg, the configuration parameter's structure
 *  @return  S16
 *      -# ROK
 **/
S16 RgMiLrgCfgReq
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfg     /* config structure  */
)
{
   uint16_t  ret = LCM_PRIM_OK;
   uint16_t  reason = LCM_REASON_NOT_APPL;
   RgMngmt   cfm;
   Pst       cfmPst;
   Inst      inst;



   DU_LOG("\nINFO  -->  MAC : Received CfgReq for MAC layer, Entity = %d, Instance = %d", pst->srcEnt, pst->srcInst);

   RG_IS_INST_VALID(pst->dstInst);
   inst = pst->dstInst - RG_INST_START;

   /* Fill the post structure for sending the confirmation */
   rgLMMFillCfmPst(pst, &cfmPst, cfg);

   memset(&cfm, 0, sizeof(RgMngmt));

#ifdef LMINT3
   cfm.hdr.transId = cfg->hdr.transId;
#endif


   cfm.hdr.elmId.elmnt = cfg->hdr.elmId.elmnt;
   switch(cfg->hdr.elmId.elmnt)
   {
      case STGEN:
#ifdef INTEL_WLS_MEM
         /* Start WLS message receiver thread */
         LwrMacStartWlsRcvr();
#endif
	 reason = rgLMMGenCfg(inst,&cfg->t.cfg); 
	 break;
      case STRGUSAP:
      case STCRGSAP:
      case STTFUSAP:
	 reason = rgLMMSapCfg(inst,&cfg->t.cfg, cfg->hdr.elmId.elmnt);
	 break;
      default:
	 ret = LCM_PRIM_NOK;
	 reason = LCM_REASON_INVALID_ELMNT;
	 DU_LOG("\nERROR  -->  MAC : Invalid Elmnt=%d",
	       cfg->hdr.elmId.elmnt);
	 break;
   }

   if (reason != LCM_REASON_NOT_APPL)
   {
      ret = LCM_PRIM_NOK;
   }

   cfm.cfm.status = ret;
   cfm.cfm.reason = reason;

   RgMiLrgCfgCfm(&cfmPst, &cfm);

   return ROK;
}/*-- RgMiLrgCfgReq --*/


/**
 * @brief Layer Manager Statistics request handler. 
 *
 * @details
 *
 *     Function : RgMiLrgStsReq
 *     
 *     This function handles the statistics
 *     request received from the Layer Manager.
 *      -# Based on sts->hdr.elmId.elmnt, it retrieves either general or SAP
 *      statistics from the rgCb global control block.
 *      -# If action=ARST, it will reset the statistics parameters in rgCb to 0.
 *      -# Invokes the RgMiLrgStsCfm to send back the confirmation to LM.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *sts, the statistics parameter's structure
 *  @return  S16
 *      -# ROK
 **/
S16 RgMiLrgStsReq
(
Pst      *pst,    /* post structure  */
RgMngmt  *sts     /* statistics structure  */
)
{
   Pst       cfmPst;
   RgMngmt   cfm;
   Inst      inst;

   RG_IS_INST_VALID(pst->dstInst);
   inst = pst->dstInst - RG_INST_START;
   /* Fill the post structure for sending the confirmation */
   rgLMMFillCfmPst(pst, &cfmPst, sts);

   memset(&cfm, 0, sizeof(RgMngmt));

#ifdef LMINT3
   cfm.hdr.transId = sts->hdr.transId;
#endif
   SGetDateTime(&cfm.t.sts.dt);
   cfm.cfm.status = LCM_PRIM_OK;
   cfm.cfm.reason = LCM_REASON_NOT_APPL;
   cfm.hdr.elmId.elmnt = sts->hdr.elmId.elmnt;
   cfm.t.sts.action = sts->t.sts.action;

   /* Check if General Config Done */
   if(rgCb[inst].rgInit.cfgDone != TRUE) 
   {
      cfm.cfm.status = LCM_PRIM_NOK;
      cfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      RgMiLrgStsCfm(&cfmPst,&cfm);
      DU_LOG("\nERROR  -->  MAC : Gen Cfg not done");
      return ROK;
   }

   switch(sts->hdr.elmId.elmnt)
   {
      case STGEN:
         {
            memcpy(&(cfm.t.sts.s.genSts), &rgCb[inst].genSts,
                  sizeof(RgGenSts));
            /* check if action is read and reset */
            if(sts->t.sts.action == ARST)
            {
               rgCb[inst].genSts.numHarqFail = 0;
            }
            /* L2 statistics */
#ifdef MAC_SCH_STATS
            {
               RgGenSts *genSts = &(cfm.t.sts.s.genSts);
               uint8_t       cqi = 0;
               for(cqi=0; cqi <= 14; cqi++)
               {
                  /* Filling DL ACK/NACK stats */
                  genSts->nackAckStats.dlCqiStat[cqi].mcs = \
                     hqFailStats.dlCqiStat[cqi].mcs;
                  genSts->nackAckStats.dlCqiStat[cqi].numOfNacks = \
                     hqFailStats.dlCqiStat[cqi].numOfNacks;
                  genSts->nackAckStats.dlCqiStat[cqi].numOfAcks = 
                     hqFailStats.dlCqiStat[cqi].numOfAcks;

                  /* Filling UL ACK/NACK stats */
                  genSts->nackAckStats.ulCqiStat[cqi].mcs = \
                     hqFailStats.ulCqiStat[cqi].mcs;
                  genSts->nackAckStats.ulCqiStat[cqi].numOfNacks = \
                     hqFailStats.ulCqiStat[cqi].numOfNacks;
                  genSts->nackAckStats.ulCqiStat[cqi].numOfAcks = \
                     hqFailStats.ulCqiStat[cqi].numOfAcks;

                  /* Filling DL HQ Retx stats */
                  genSts->hqRetxStats.dlCqiStat[cqi].mcs = \
                     hqRetxStats.dlCqiStat[cqi].mcs;
                  genSts->hqRetxStats.dlCqiStat[cqi].numOfHQ_1 = \
                     hqRetxStats.dlCqiStat[cqi].numOfHQ_1;
                  genSts->hqRetxStats.dlCqiStat[cqi].numOfHQ_2 = \
                     hqRetxStats.dlCqiStat[cqi].numOfHQ_2;
                  genSts->hqRetxStats.dlCqiStat[cqi].numOfHQ_3 = \
                     hqRetxStats.dlCqiStat[cqi].numOfHQ_3;
                  genSts->hqRetxStats.dlCqiStat[cqi].numOfHQ_4 = \
                     hqRetxStats.dlCqiStat[cqi].numOfHQ_4;
                  genSts->hqRetxStats.dlCqiStat[cqi].totalTx = \
                     hqRetxStats.dlCqiStat[cqi].totalTx;

                  /* Filling UL HQ Retx stats */
                  genSts->hqRetxStats.ulCqiStat[cqi].mcs = \
                     hqRetxStats.ulCqiStat[cqi].mcs;
                  genSts->hqRetxStats.ulCqiStat[cqi].numOfHQ_1 = \
                     hqRetxStats.ulCqiStat[cqi].numOfHQ_1;
                  genSts->hqRetxStats.ulCqiStat[cqi].numOfHQ_2 = \
                     hqRetxStats.ulCqiStat[cqi].numOfHQ_2;
                  genSts->hqRetxStats.ulCqiStat[cqi].numOfHQ_3 = \
                     hqRetxStats.ulCqiStat[cqi].numOfHQ_3;
                  genSts->hqRetxStats.ulCqiStat[cqi].numOfHQ_4 = \
                     hqRetxStats.ulCqiStat[cqi].numOfHQ_4;
                  genSts->hqRetxStats.ulCqiStat[cqi].totalTx = \
                     hqRetxStats.ulCqiStat[cqi].totalTx;
               }
               /* Reset statistics */
               if(sts->t.sts.action == ZEROSTS)
               {
                  memset(&hqRetxStats, 0, \
                        sizeof(RgSchHqRetxStats));
                  memset(&hqFailStats, 0, \
                        sizeof(RgSchNackAckStats));
               }
            }
#endif /* MAC_SCH_STATS*/
	 }
	 break;
      case STRGUSAP:
         memcpy(&(cfm.t.sts.s.rguSts), &rgCb[inst].rguSap[sts->t.sts.sapInst].sapSts,
                  sizeof(RgSapSts));

         /* check if action is read and reset */
         if(sts->t.sts.action == ARST)
            memset(&rgCb[inst].rguSap[sts->t.sts.sapInst].sapSts, 0, sizeof(RgSapSts));

	 break;
      case STCRGSAP:
         memcpy(&(cfm.t.sts.s.crgSts), &rgCb[inst].crgSap.sapSts,
                  sizeof(RgSapSts));

         /* check if action is read and reset */
         if(sts->t.sts.action == ARST)
            memset(&rgCb[inst].crgSap.sapSts, 0, sizeof(RgSapSts));

	 break;
      case STTFUSAP:
         memcpy(&(cfm.t.sts.s.tfuSts), &rgCb[inst].tfuSap.sapSts,
                  sizeof(RgSapSts));

         /* check if action is read and reset */
         if(sts->t.sts.action == ARST)
            memset(&rgCb[inst].tfuSap.sapSts, 0, sizeof(RgSapSts));

	 break;
      default:
	 cfm.cfm.status = LCM_PRIM_NOK;
	 cfm.cfm.reason = LCM_REASON_INVALID_ELMNT;
	 DU_LOG("\nERROR  -->  MAC : Invalid Elmnt = %d",sts->hdr.elmId.elmnt);
	 break;     
   }
   RgMiLrgStsCfm(&cfmPst,&cfm);
   return ROK;
}/*-- RgMiLrgStsReq --*/


/**
 * @brief Layer Manager Status request handler. 
 *
 * @details
 *
 *     Function : RgMiLrgStaReq
 *     
 *     This function handles the solicited status
 *     request received from the Layer Manager.
 *      -# Based on sta->hdr.elmId.elmnt, it retrieves the status of a
 *      particular SAP from the rgCb global control block.
 *      -# Invokes the RgMiLrgStaCfm to send back the confirmation to LM.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *sta, the status parameter's structure
 *  @return  S16
 *      -# ROK
 **/
S16 RgMiLrgStaReq
(
Pst      *pst,    /* post structure  */
RgMngmt  *sta     /* status structure  */
)
{
   Pst       cfmPst;
   RgMngmt   cfm;
   Inst      inst ;


   RG_IS_INST_VALID(pst->dstInst);
   inst = pst->dstInst - RG_INST_START;


   /* Fill the post structure for sending the confirmation */
   rgLMMFillCfmPst(pst, &cfmPst, sta);

   if (sta->t.ssta.s.sysId.ptNmb != NULLP)
   {
      MAC_FREE(sta->t.ssta.s.sysId.ptNmb, LRG_MAX_PT_NUM_SIZE);
   }
   
   memset(&cfm, 0, sizeof(RgMngmt));
   cfm.hdr.elmId.elmnt = sta->hdr.elmId.elmnt;

#ifdef LMINT3
   cfm.hdr.transId = sta->hdr.transId;
#endif
   /* Check if General Config Done */
   if(rgCb[inst].rgInit.cfgDone != TRUE) 
   {
      SGetDateTime(&cfm.t.ssta.dt);
      MAC_ALLOC(cfm.t.ssta.s.sysId.ptNmb, LRG_MAX_PT_NUM_SIZE);
	   if(cfm.t.ssta.s.sysId.ptNmb == NULLP)
      {
	 DU_LOG("\nERROR  -->  MAC : Memory Unavailable for Confirmation");
	 return ROK;
      } 
      memset((cfm.t.ssta.s.sysId.ptNmb), 0, LRG_MAX_PT_NUM_SIZE);
      rgGetSId(&cfm.t.ssta.s.sysId);
      cfm.cfm.status = LCM_PRIM_NOK;
      cfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      cfm.hdr.elmId.elmnt = sta->hdr.elmId.elmnt;
      RgMiLrgStaCfm(&cfmPst, &cfm);
      DU_LOG("\nERROR  -->  MAC : Gen Cfg not done");
      return ROK;
   }

   switch(sta->hdr.elmId.elmnt)
   {
      case STGEN:
         SGetDateTime(&cfm.t.ssta.dt);
         MAC_ALLOC(cfm.t.ssta.s.sysId.ptNmb, LRG_MAX_PT_NUM_SIZE);
         if(cfm.t.ssta.s.sysId.ptNmb  == NULLP)
         {
            DU_LOG("\nERROR  -->  MAC : Memory Unavailable for Confirmation");
            return ROK;
         } 
         memset((cfm.t.ssta.s.sysId.ptNmb), 0, LRG_MAX_PT_NUM_SIZE);
         rgGetSId(&cfm.t.ssta.s.sysId);
         cfm.cfm.status = LCM_PRIM_OK;
         cfm.cfm.reason = LCM_REASON_NOT_APPL;
         RgMiLrgStaCfm(&cfmPst, &cfm);
         break;
      case STRGUSAP:
         cfm.cfm.status = LCM_PRIM_OK;
         cfm.cfm.reason = LCM_REASON_NOT_APPL;
         SGetDateTime(&cfm.t.ssta.dt);
         memcpy(&(cfm.t.ssta.s.rguSapSta), 
				&rgCb[inst].rguSap[sta->t.ssta.sapInst].sapSta,
            sizeof(RgSapSta));
         RgMiLrgStaCfm(&cfmPst, &cfm);
         break;
      case STCRGSAP:
         cfm.cfm.status = LCM_PRIM_OK;
         cfm.cfm.reason = LCM_REASON_NOT_APPL;
         SGetDateTime(&cfm.t.ssta.dt);
         memcpy(&(cfm.t.ssta.s.crgSapSta), &rgCb[inst].crgSap.sapSta,
         sizeof(RgSapSta));
         RgMiLrgStaCfm(&cfmPst, &cfm);
         break;
      case STTFUSAP:
         cfm.cfm.status = LCM_PRIM_OK;
         cfm.cfm.reason = LCM_REASON_NOT_APPL;
         SGetDateTime(&cfm.t.ssta.dt);
         memcpy(&(cfm.t.ssta.s.tfuSapSta), &rgCb[inst].tfuSap.sapSta,
         sizeof(RgSapSta));
         RgMiLrgStaCfm(&cfmPst, &cfm);
         break;
      default:
	 cfm.cfm.status = LCM_PRIM_NOK;
	 cfm.cfm.reason = LCM_REASON_INVALID_ELMNT;
	 RgMiLrgStaCfm(&cfmPst, &cfm);
	 DU_LOG("\nERROR  -->  MAC : Invalid elmnt=%d",sta->hdr.elmId.elmnt);
	 break;     
   }
   return ROK;
}/*-- RgMiLrgStaReq --*/


/**
 * @brief Layer Manager Control request handler. 
 *
 * @details
 *
 *     Function : RgMiLrgCntrlReq
 *     
 *     This function handles the control
 *     request received from the Layer Manager.
 *      -# Based on cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action
 *      and cntrl->t.cntrl.subAction, it performs the appropriate control action
 *      of SAP (enable/disable), Debug (enable/disable), Trace (enable/disable)
 *      and layer shutdown.
 *      -# Invokes the RgMiLrgCntrlCfm to send back the confirmation to LM.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cntrl, the control parameter's structure
 *  @return  S16
 *      -# ROK
 **/
S16 RgMiLrgCntrlReq
(
Pst      *pst,    /* post structure  */
RgMngmt  *cntrl   /* control structure  */
)
{
   S16       ret = ROK;            /* return value */
   Pst       cfmPst;
   RgMngmt   cfm;
   Inst      inst;
   /* Fill the post structure for sending the confirmation */

   RG_IS_INST_VALID(pst->dstInst);
   inst = pst->dstInst - RG_INST_START;

   rgLMMFillCfmPst(pst, &cfmPst, cntrl);

   memset(&cfm, 0, sizeof(RgMngmt));
#ifdef LMINT3
   cfm.hdr.transId = cntrl->hdr.transId;
#endif
   cfm.hdr.elmId.elmnt = cntrl->hdr.elmId.elmnt;
   cfm.t.cntrl.action = cntrl->t.cntrl.action;
   cfm.t.cntrl.subAction = cntrl->t.cntrl.subAction;

   /* Check if General Config Done*/
   if(rgCb[inst].rgInit.cfgDone != TRUE)
   {
      cfm.cfm.status = LCM_PRIM_NOK;
      cfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      cfm.hdr.elmId.elmnt = cntrl->hdr.elmId.elmnt;
      RgMiLrgCntrlCfm(&cfmPst, &cfm);
      DU_LOG("\nERROR  -->  MAC : Gen Cfg not done");
      return ROK;
   }

   /* General Config done, process the Control request */   
   switch(cntrl->hdr.elmId.elmnt)
   {
      case STGEN:
	 rgLMMGenCntrl(cntrl, &cfm, &cfmPst);
	 break;
      case STTFUSAP:
      case STRGUSAP:
      case STCRGSAP:
	 rgLMMSapCntrl(cntrl, &cfm, &cfmPst);
	 break;
      default:
	 cfm.cfm.status = LCM_PRIM_NOK;
	 cfm.cfm.reason = LCM_REASON_INVALID_PAR_VAL;
	 RgMiLrgCntrlCfm(&cfmPst, &cfm);
	 DU_LOG("\nERROR  -->  MAC : invalid elmnt=%d",cntrl->hdr.elmId.elmnt);
	 break;
   }
   return (ret);
}/*-- RgMiLrgCntrlReq --*/


/**
 * @brief SAP Configuration Handler. 
 *
 * @details
 *
 *     Function : rgLMMSapCfg
 *     
 *     This function in called by RgMiLrgCfgReq(). It handles the
 *     interface SAP configuration of the LTE MAC layer. It 
 *     initializes the sapState to LRG_UNBND. Returns
 *     reason for success/failure of this function.
 *     
 *  @param[in]  Inst        inst
 *  @param[in]  RgCfg *cfg, the Configuaration information 
 *  @return  uint16_t
 *      -# LCM_REASON_GENCFG_NOT_DONE
 *      -# LCM_REASON_INVALID_SAP
 *      -# LCM_REASON_NOT_APPL
 **/
static uint16_t rgLMMSapCfg
(
Inst  inst,
RgCfg *cfg,            /* Configuaration information */
Elmnt sapType             /* Sap Type */
)
{
   uint16_t          ret = LCM_REASON_NOT_APPL;
   RgLowSapCfgInfo   *lowSapCfg = NULLP;
   RgUpSapCfgInfo    *upSapCfg = NULLP;
   RgUpSapCb         *upSapCb  = NULLP;


      /* Check if Gen Config has been done */
   if(rgCb[inst].rgInit.cfgDone != TRUE)
	 return (LCM_REASON_GENCFG_NOT_DONE);

   switch(sapType)
   {   
      case STRGUSAP:
	 if ((cfg->s.rguSap.spId > LRG_MAX_RGU_SAPS) &&
	       (cfg->s.rguSap.selector != ODU_SELECTOR_TC) &&
	       (cfg->s.rguSap.selector != ODU_SELECTOR_LC))
	 {
	    ret = LCM_REASON_INVALID_PAR_VAL;
	    DU_LOG("\nERROR  -->  MAC : unsupported Selector value for RGU");
	    break;
	 }
	 upSapCb = &(rgCb[inst].rguSap[cfg->s.rguSap.spId]);
	 if(upSapCb->sapSta.sapState == LRG_NOT_CFG)
	 { 
	    upSapCb->sapSta.sapState = LRG_UNBND;
	 }
	 upSapCfg = &(upSapCb->sapCfg);
	 upSapCfg->sapPst.dstEnt = cfg->s.rguSap.ent;
	 upSapCfg->sapPst.dstInst = cfg->s.rguSap.inst;
	 upSapCfg->sapPst.dstProcId = cfg->s.rguSap.procId;
	 upSapCfg->sapPst.srcEnt = rgCb[inst].rgInit.ent;
	 upSapCfg->sapPst.srcInst = rgCb[inst].rgInit.inst;
	 upSapCfg->sapPst.srcProcId = rgCb[inst].rgInit.procId;
	 upSapCfg->sapPst.region = cfg->s.rguSap.mem.region;
	 upSapCfg->sapPst.pool = cfg->s.rguSap.mem.pool;
	 upSapCfg->sapPst.selector = cfg->s.rguSap.selector;
	 upSapCfg->sapPst.route = cfg->s.rguSap.route;
	 upSapCfg->sapPst.intfVer = 0; 
	 upSapCfg->sapPst.prior = cfg->s.rguSap.prior;
	 upSapCfg->suId = cfg->s.rguSap.suId;
	 upSapCfg->spId = cfg->s.rguSap.spId;
	 /*T2K uses 2 saps, T3K uses 1 sap. change the rgRguDlSap to 1 only if
	  * there is cfg request with sap is 1*/
	 break;
      case STCRGSAP:
	 if ((cfg->s.crgSap.selector != ODU_SELECTOR_TC) &&
	       (cfg->s.crgSap.selector != ODU_SELECTOR_LC))
	 {
	    ret = LCM_REASON_INVALID_PAR_VAL;
	    DU_LOG("\nERROR  -->  MAC : unsupported Selector value for CRG");
	    break;
	 }
	 if(rgCb[inst].crgSap.sapSta.sapState == LRG_NOT_CFG)
	 { 
	    rgCb[inst].crgSap.sapSta.sapState = LRG_UNBND;
	 }
	 upSapCfg = &rgCb[inst].crgSap.sapCfg;

	 upSapCfg->sapPst.dstEnt = cfg->s.crgSap.ent;
	 upSapCfg->sapPst.dstInst = cfg->s.crgSap.inst;
	 upSapCfg->sapPst.dstProcId = cfg->s.crgSap.procId;
	 upSapCfg->sapPst.srcEnt = rgCb[inst].rgInit.ent;
	 upSapCfg->sapPst.srcInst = rgCb[inst].rgInit.inst;
	 upSapCfg->sapPst.srcProcId = rgCb[inst].rgInit.procId;
	 upSapCfg->sapPst.region = cfg->s.crgSap.mem.region;
	 upSapCfg->sapPst.pool = cfg->s.crgSap.mem.pool;
	 upSapCfg->sapPst.selector = cfg->s.crgSap.selector;
	 upSapCfg->sapPst.route = cfg->s.crgSap.route;
	 upSapCfg->sapPst.intfVer = 0; 
	 upSapCfg->sapPst.prior = cfg->s.crgSap.prior;
	 upSapCfg->suId = cfg->s.crgSap.suId;
	 upSapCfg->spId = cfg->s.crgSap.spId;
	 break;
      case STTFUSAP:
#ifndef CL_MAC_LWLC 
	 if ((cfg->s.tfuSap.selector != ODU_SELECTOR_TC) &&
	       (cfg->s.tfuSap.selector != ODU_SELECTOR_LC))
	 {
	    ret = LCM_REASON_INVALID_PAR_VAL;
	    DU_LOG("\nERROR  -->  MAC : unsupported Selector value for TFU");
	    break;
	 }
#endif
         if (rgCb[inst].tfuSap.sapSta.sapState == LRG_NOT_CFG) 
         { 
            rgCb[inst].tfuSap.sapSta.sapState = LRG_UNBND;
         }
         lowSapCfg = &rgCb[inst].tfuSap.sapCfg;

         lowSapCfg->sapPst.dstEnt = cfg->s.tfuSap.ent;
         lowSapCfg->sapPst.dstInst = cfg->s.tfuSap.inst;
         lowSapCfg->sapPst.dstProcId = rgCb[inst].rgInit.procId;
         lowSapCfg->sapPst.srcEnt = rgCb[inst].rgInit.ent;
         lowSapCfg->sapPst.srcInst = rgCb[inst].rgInit.inst;
         lowSapCfg->sapPst.srcProcId = rgCb[inst].rgInit.procId;
         lowSapCfg->sapPst.region = cfg->s.tfuSap.mem.region;
         lowSapCfg->sapPst.pool = cfg->s.tfuSap.mem.pool;
         lowSapCfg->sapPst.selector = cfg->s.tfuSap.selector;
         lowSapCfg->sapPst.route = cfg->s.tfuSap.route;
         lowSapCfg->sapPst.intfVer = 0; 
         lowSapCfg->sapPst.prior = cfg->s.tfuSap.prior;
         lowSapCfg->suId = cfg->s.tfuSap.suId;
         lowSapCfg->spId = cfg->s.tfuSap.spId;
         memcpy(&lowSapCfg->bndTmr, &cfg->s.tfuSap.bndTmr,
                   sizeof(TmrCfg));
         break;
      default:
	 /* would never reach here */
	 break;
   }
   return (ret);
}


/**
 * @brief General Configuration Handler. 
 *
 * @details
 *
 *     Function : rgLMMGenCfg
 *     
 *     This function in called by RgMiLrgCfgReq(). It handles the
 *     general configuration of the LTE MAC layer. It initializes 
 *     the hash lists of RgCb. Returns
 *     reason for success/failure of this function.
 *     
 *  @param[in]  Inst        inst
 *  @param[in]  RgCfg *cfg, the Configuaration information 
 *  @return  uint16_t
 *      -# LCM_REASON_NOT_APPL 
 *      -# LCM_REASON_INVALID_MSGTYPE
 *      -# LCM_REASON_MEM_NOAVAIL
 **/
static uint16_t rgLMMGenCfg
(
Inst inst,
RgCfg *cfg            /* Configuaration information */
)
{
   uint16_t    ret = LCM_REASON_NOT_APPL;


   /* Check if General Configuration is done already */
   if (rgCb[inst].rgInit.cfgDone == TRUE)
   {
      return (LCM_REASON_INVALID_MSGTYPE);
   }
   if ((cfg->s.genCfg.lmPst.selector != ODU_SELECTOR_TC) &&
	 (cfg->s.genCfg.lmPst.selector != ODU_SELECTOR_LC))
   {
      DU_LOG("\nERROR  -->  MAC : unsupported Selector value for RGU");
      return (LCM_REASON_INVALID_PAR_VAL);
   }
   /* Update the Pst structure for LM interface */
   memcpy(&rgCb[inst].rgInit.lmPst, &cfg->s.genCfg.lmPst,
             sizeof(Pst));

   rgCb[inst].rgInit.lmPst.srcProcId = rgCb[inst].rgInit.procId;
   rgCb[inst].rgInit.lmPst.srcEnt = rgCb[inst].rgInit.ent;
   rgCb[inst].rgInit.lmPst.srcInst = rgCb[inst].rgInit.inst;
   rgCb[inst].rgInit.lmPst.event = EVTNONE;

   rgCb[inst].rgInit.region = cfg->s.genCfg.mem.region;
   rgCb[inst].rgInit.pool = cfg->s.genCfg.mem.pool;
   rgCb[inst].genCfg.tmrRes = cfg->s.genCfg.tmrRes;

   macCb.tmrRes = cfg->s.genCfg.tmrRes;
   macCb.macInst = rgCb[inst].rgInit.inst;
   macCb.procId = rgCb[inst].rgInit.procId;

   /* Initialize SAP States */
   rgCb[inst].crgSap.sapSta.sapState = LRG_NOT_CFG;

   if(cfg->s.genCfg.numRguSaps == 0)
   {
      DU_LOG("\nERROR  -->  MAC : rgGenCfg(): Invalid numRguSap.\n");
      return RFAILED;
   }

   /* allocate RGR saps */
   MAC_ALLOC(rgCb[inst].rguSap, sizeof(RgUpSapCb) * cfg->s.genCfg.numRguSaps);
   if(rgCb[inst].rguSap == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : rgGenCfg(): Failed to allocate mem for RGU SAP's.\n");
      return RFAILED;
   }
   rgCb[inst].numRguSaps = cfg->s.genCfg.numRguSaps;

   for (int idx = 0; idx < rgCb[inst].numRguSaps; idx++)
   {
      rgCb[inst].rguSap[idx].sapSta.sapState = LRG_NOT_CFG;
      memset(&rgCb[inst].rguSap[idx], 0, sizeof(RgUpSapCb));
   }
   rgCb[inst].tfuSap.sapSta.sapState = LRG_NOT_CFG;
   /* Initialize the timer blocks */
   cmInitTimers(macCb.tmrBlk, MAX_NUM_TIMER);
   /* Initialzie the timer queue */   
   memset(&macCb.tmrTq, 0, sizeof(CmTqType) * MAC_TQ_SIZE);
   /* Initialize the timer control point */
   memset(&macCb.tmrTqCp, 0, sizeof(CmTqCp));
   macCb.tmrTqCp.tmrLen = MAC_TQ_SIZE;

   /* Timer Registration request to SSI */
   if(ODU_REG_TMR_MT(ENTMAC, macCb.macInst, macCb.tmrRes, macActvTmr) != ROK)
   {

      DU_LOG("\nERROR  -->  MAC : Failed to register timer");

      MAC_FREE(rgCb[inst].rguSap,
	    (sizeof(RgUpSapCb) * cfg->s.genCfg.numRguSaps));

      return (LCM_REASON_MEM_NOAVAIL);
   }

   /* Set Config done in TskInit */
   rgCb[inst].rgInit.cfgDone = TRUE;

   return (ret);
}


/***********************************************************
 *
 *     Func : rgLMMShutdown
 *        
 *
 *     Desc : Handles the MAC layer shutdown request. Calls 
 *     rgCFGFreeCellCb(RgCellCb*) to handle each cellCb deallocation.
 *            
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : rg_lmm.c 
 *
 **********************************************************/
static Void rgLMMShutdown(Inst inst)
{
   RgCellCb   *cell = rgCb[inst].cell;
   uint8_t    idx;


    /* Unbind the TFU Sap */
   if(rgCb[inst].tfuSap.sapSta.sapState == LRG_WAIT_BNDCFM)
   {
      //rgLIMTfuUbndReq(inst,rgCb[inst].tfuSap.sapCfg.spId, LRG_UNBND);
      if (rgCb[inst].tfuSap.sapCfg.bndTmr.enb == TRUE)
      {
         rgLMMStopTmr(inst,RG_BNDREQ_TMR, (PTR)&rgCb[inst].tfuSap); 
      } 
      rgCb[inst].tfuSap.sapSta.sapState = LRG_UNBND;
   }
   if(rgCb[inst].tfuSap.sapSta.sapState == LRG_BND)
   {
      //rgLIMTfuUbndReq(inst,rgCb[inst].tfuSap.sapCfg.spId, LRG_UNBND);
      rgCb[inst].tfuSap.sapSta.sapState = LRG_UNBND;
   }


   if(cell != NULLP)
   {
      for(idx=0;idx < RG_NUM_SUB_FRAMES; idx++)
      {
	 rgTOMRlsSf(inst,&cell->subFrms[idx]);
      }

      rgCFGFreeCellCb(cell);
   }

   /* Deleting the RGU SAPs */
    MAC_FREE(rgCb[inst].rguSap,
	 (sizeof(RgUpSapCb) * rgCb[inst].numRguSaps));

   rgCb[inst].inactiveCell = NULLP;
   rgCb[inst].cell         = NULLP;

   /* De-register the Timer Service */
   (Void) SDeregTmrMt(rgCb[inst].rgInit.ent, rgCb[inst].rgInit.inst,
	 (S16)rgCb[inst].genCfg.tmrRes, macActvTmr); 

   /* call back the task initialization function to intialize
    * the global RgCb Struct */
   rgActvInit(rgCb[inst].rgInit.ent, rgCb[inst].rgInit.inst, rgCb[inst].rgInit.region, 
	 rgCb[inst].rgInit.reason);

   return;
}


/***********************************************************
 *
 *     Func : rgLMMGenCntrl 
 *        
 *
 *     Desc : Processes the LM control request for STGEN elmnt.
 *            
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : rg_lmm.c 
 *
 **********************************************************/
static Void rgLMMGenCntrl 
(
RgMngmt       *cntrl,
RgMngmt       *cfm,
Pst           *cfmPst
)
{
   Inst      inst = (cfmPst->srcInst - RG_INST_START);

   cfm->cfm.status = LCM_PRIM_OK;
   cfm->cfm.reason = LCM_REASON_NOT_APPL;


   switch(cntrl->t.cntrl.action)
   {
      case AENA:
         /* Action is Enable */
         switch(cntrl->t.cntrl.subAction)
         {
            case SATRC:
            /* Enable Traces */
               rgCb[inst].rgInit.trc = TRUE;
               rgCb[inst].trcLen = cntrl->t.cntrl.s.trcLen;
               /*Store the response and TransId for sending the Traces */
               memcpy(&rgCb[inst].genCfg.trcResp.response, 
               &cntrl->hdr.response, sizeof(Resp));
               rgCb[inst].genCfg.trcResp.transId = cntrl->hdr.transId;
               
               break;
            case SAUSTA:   
            /* Enable Unsolicited Status (alarms) */
               rgCb[inst].rgInit.usta = TRUE;
               /*Store the response and TransId for sending the Alarms */
               memcpy(&rgCb[inst].genCfg.ustaResp.response, 
               &cntrl->hdr.response, sizeof(Resp));
               rgCb[inst].genCfg.ustaResp.transId = cntrl->hdr.transId;
               break;
            case SADBG:
            /* Enable Debug Printing */
#ifdef DEBUGP
	       rgCb[inst].rgInit.dbgMask |= cntrl->t.cntrl.s.rgDbgCntrl.dbgMask;
#endif
	       break;
#ifdef SS_DIAG
	    case SALOG:
	       rgCb[inst].rgInit.logMask = cntrl->t.cntrl.s.logMask;
	       break;
#endif

	    default:
	       cfm->cfm.status = LCM_PRIM_NOK;
	       cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
	       DU_LOG("\nERROR  -->  MAC : invalid subaction=%d",cntrl->t.cntrl.subAction);
	       break;
	 }
	 break;
      case ADISIMM:
	 /* Action is Diable immidiately */
	 switch(cntrl->t.cntrl.subAction)
	 {
	    case SATRC:
	       /* Disable Traces */
	       rgCb[inst].rgInit.trc = FALSE;
	       break;
	    case SAUSTA:
	       /* Disable Unsolicited Status (alarms) */
	       rgCb[inst].rgInit.usta = FALSE;
	       break;
	    case SADBG:
	       /* Disable Debug Printing */
#ifdef DEBUGP
	       rgCb[inst].rgInit.dbgMask &=~cntrl->t.cntrl.s.rgDbgCntrl.dbgMask;
#endif
	       break;
#ifdef SS_DIAG
	    case SALOG:
	       rgCb[inst].rgInit.logMask = cntrl->t.cntrl.s.logMask;
	       break;
#endif

	    default:
	       cfm->cfm.status = LCM_PRIM_NOK;
	       cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
	       DU_LOG("\nERROR  -->  MAC : invalid subaction=%d",cntrl->t.cntrl.subAction);
	       break;
	 }
	 break;
      case ASHUTDOWN:
	 /* Free all the memory dynamically allocated by MAC */
	 rgLMMShutdown(inst);
	 break;
      default:
	 cfm->cfm.status = LCM_PRIM_NOK;
	 cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
	 DU_LOG("\nERROR  -->  MAC : invalid action=%d",cntrl->t.cntrl.action);
	 break;
   }
   RgMiLrgCntrlCfm(cfmPst, cfm);
   return;
}


/***********************************************************
 *
 *     Func : rgLMMSapCntrl 
 *        
 *
 *     Desc : Processes the LM control request for STxxxSAP elmnt.
 *            
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : rg_lmm.c 
 *
 **********************************************************/
static Void rgLMMSapCntrl 
(
RgMngmt       *cntrl,
RgMngmt       *cfm,
Pst           *cfmPst
)
{
   Inst      inst = cfmPst->srcInst - RG_INST_START;

   /* Only TFU Sap can be controlled by LM */
   switch(cntrl->hdr.elmId.elmnt)
   {
      case STTFUSAP:
         switch(cntrl->t.cntrl.action)
         {
            case ABND:
            /* Bind Enable Request */
               if ((rgCb[inst].tfuSap.sapSta.sapState == LRG_NOT_CFG) ||
                   (rgCb[inst].tfuSap.sapSta.sapState == LRG_BND))
               {
                  cfm->cfm.status = LCM_PRIM_NOK;
                  cfm->cfm.reason = LCM_REASON_INVALID_SAP;
               }
               else
               {
                  if (rgCb[inst].tfuSap.sapCfg.bndTmr.enb == TRUE)
                  {
                     rgLMMStartTmr(inst,RG_BNDREQ_TMR, rgCb[inst].tfuSap.sapCfg.bndTmr.val, 
                     (PTR)&rgCb[inst].tfuSap);
                  }
                  /* Change SAP state */
                  rgCb[inst].tfuSap.sapSta.sapState = LRG_WAIT_BNDCFM;
                  rgCb[inst].tfuSap.numBndRetries++;
                  /* Store the response and TransId for sending 
                   * the Control confirm */
                  memcpy(&rgCb[inst].genCfg.bndCfmResp.response,
                           &cntrl->hdr.response, sizeof(Resp));
                  rgCb[inst].genCfg.bndCfmResp.transId = cntrl->hdr.transId;

                  /* Sending Status Indication to Layer Manager */
                  cfm->cfm.status = LCM_PRIM_OK_NDONE;
                  cfm->cfm.reason = LCM_REASON_NOT_APPL;
                  RgMiLrgCntrlCfm(cfmPst, cfm);

                  //rgLIMTfuBndReq(inst,rgCb[inst].tfuSap.sapCfg.suId,
                                      //rgCb[inst].tfuSap.sapCfg.spId);
                  RETVOID;
               }
               break;
            case AUBND:
            /* Unbind request */

               /* Check if the SAP is configured */
               if( (rgCb[inst].tfuSap.sapSta.sapState == LRG_NOT_CFG) ||
                     (rgCb[inst].tfuSap.sapSta.sapState == LRG_UNBND))
               {
                  cfm->cfm.status = LCM_PRIM_NOK;
                  cfm->cfm.reason = LCM_REASON_INVALID_MSGTYPE;
               }
               else
               {
                  //rgLIMTfuUbndReq(inst,rgCb[inst].tfuSap.sapCfg.spId, TFU_UBNDREQ_MNGMT);
                  if (rgCb[inst].tfuSap.sapCfg.bndTmr.enb == TRUE)
                  {
                     rgLMMStopTmr(inst,RG_BNDREQ_TMR, (PTR)&rgCb[inst].tfuSap);
                  }
                  /* Change SAP state */
                  rgCb[inst].tfuSap.sapSta.sapState = LRG_UNBND;
                  cfm->cfm.status = LCM_PRIM_OK;
                  cfm->cfm.reason = LCM_REASON_NOT_APPL;
               }
               break;
            case ADEL:
               /* Delete SAP, does initialization of SAP */
               if ((rgCb[inst].tfuSap.sapSta.sapState == LRG_WAIT_BNDCFM) ||
                   (rgCb[inst].tfuSap.sapSta.sapState == LRG_BND))
               {
                  //rgLIMTfuUbndReq(inst,rgCb[inst].tfuSap.sapCfg.spId, TFU_UBNDREQ_MNGMT);
                  if (rgCb[inst].tfuSap.sapCfg.bndTmr.enb == TRUE)
                  {
                     rgLMMStopTmr(inst,RG_BNDREQ_TMR, (PTR)&rgCb[inst].tfuSap);
                  }
               }
               memset(&rgCb[inst].tfuSap, 0, sizeof(RgLowSapCb));
               rgCb[inst].tfuSap.sapSta.sapState = LRG_NOT_CFG;
               cfm->cfm.status = LCM_PRIM_OK;
               cfm->cfm.reason = LCM_REASON_NOT_APPL;
               break;
            default:
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
               DU_LOG("\nERROR  -->  MAC : rgLMMSapCntrl(): invalid action=%d",
               cntrl->t.cntrl.action);
               break;
         }
         break;
      case STRGUSAP:
         switch(cntrl->t.cntrl.action)
         {
            case ADEL:
               memset(&rgCb[inst].rguSap[cntrl->t.cntrl.instId], 0, sizeof(RgUpSapCb));
               rgCb[inst].rguSap[cntrl->t.cntrl.instId].sapSta.sapState = LRG_NOT_CFG;
               cfm->cfm.status = LCM_PRIM_OK;
               cfm->cfm.reason = LCM_REASON_NOT_APPL;
               break;
            default:
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
               DU_LOG("\nERROR  -->  MAC : rgLMMSapCntrl(): invalid action=%d",
               cntrl->t.cntrl.action);
               break;
         }
         break;
      case STCRGSAP:
         switch(cntrl->t.cntrl.action)
         {
            case ADEL:
               memset(&rgCb[inst].crgSap, 0, sizeof(RgUpSapCb));
               rgCb[inst].crgSap.sapSta.sapState = LRG_NOT_CFG;
               cfm->cfm.status = LCM_PRIM_OK;
               cfm->cfm.reason = LCM_REASON_NOT_APPL;
               break;
            default:
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
               DU_LOG("\nERROR  -->  MAC : invalid action=%d",cntrl->t.cntrl.action);
               
               break;
         }
         break;
      default:
         /* Would never here. */
         RETVOID;
   }
   RgMiLrgCntrlCfm(cfmPst, cfm);
   return;
}


/***********************************************************
 *
 *     Func : rgLMMFillCfmPst 
 *        
 *
 *     Desc : Fills the Confirmation Post Structure cfmPst using the reqPst 
 *            and the cfm->hdr.response.
 *            
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : rg_lmm.c 
 *
 **********************************************************/
static Void rgLMMFillCfmPst
(
Pst           *reqPst,
Pst           *cfmPst,
RgMngmt       *cfm
)
{
   Inst inst;
   inst = (reqPst->dstInst - RG_INST_START);

   cfmPst->srcEnt    = rgCb[inst].rgInit.ent;
   cfmPst->srcInst   = rgCb[inst].rgInit.inst;
   cfmPst->srcProcId = rgCb[inst].rgInit.procId;
   cfmPst->dstEnt    = reqPst->srcEnt;
   cfmPst->dstInst   = reqPst->srcInst;
   cfmPst->dstProcId = reqPst->srcProcId;

   cfmPst->selector  = cfm->hdr.response.selector;
   cfmPst->prior     = cfm->hdr.response.prior;
   cfmPst->route     = cfm->hdr.response.route;
   cfmPst->region    = cfm->hdr.response.mem.region;
   cfmPst->pool      = cfm->hdr.response.mem.pool;

   return;
}


/**
 * @brief Timer start handler. 
 *
 * @details
 *
 *     Function : rgLMMStartTmr
 *     
 *     This function based on the input parameters starts the timer for 
 *     "tmrVal" duration. As of now MAC uses the timer functionality for 
 *     BndReq only. Hence there is no conditional code based on "tmrEvnt".
 *     
 *  @param[in]  Inst        inst
 *  @param[in]  S16   tmrEvnt, the Timer Event    
 *  @param[in]  uint32_t   tmrVal,  the Wait Time
 *  @param[in]  PTR   cb,  Entry for which Timer expired
 *  @return  S16
 *      -# ROK
 **/
S16 rgLMMStartTmr
(
Inst               inst,
S16                tmrEvnt,            /* Timer Event */
uint32_t           tmrVal,             /* Wait Time */
PTR                cb                  /* Entry for which Timer Expired */
)
{
   CmTmrArg    arg;


      UNUSED(tmrEvnt);

   /* Initialize the arg structure */
   memset(&arg, 0, sizeof(CmTmrArg));

   arg.tqCp = &rgCb[inst].tmrTqCp;
   arg.tq = rgCb[inst].tmrTq;
   arg.timers = rgCb[inst].tmrBlk;
   arg.cb = cb;
   arg.tNum = 0;
   arg.max = RG_MAX_TIMER;
   arg.evnt = RG_BNDREQ_TMR;
   arg.wait = tmrVal;      
   cmPlcCbTq(&arg);

   return ROK;
}


/**
 * @brief Timer stop handler. 
 *
 * @details
 *
 *     Function : rgLMMStopTmr
 *     
 *     This function based on the input parameters stops the timer for 
 *     "tmrEvnt". As of now MAC uses the timer functionality for 
 *     BndReq only. Hence there is no conditional code based on "tmrEvnt".
 *     Once the bind happens and this timer is stopped, the timer functionality
 *     is deregistered with SSI. As there is no further use of timer processing.
 *     
 *  @param[in]  Inst        inst
 *  @param[in]  S16   tmrEvnt, the Timer Event    
 *  @param[in]  PTR   cb,  Entry for which Timer expired
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgLMMStopTmr
(
Inst               inst,             /* Scheduler instance */
S16                tmrEvnt,            /* Timer Event */
PTR                cb                  /* Entry for which Timer Expired */
)
{
   CmTmrArg   arg;
   uint8_t      i;
   S16        ret; 


   ret = RFAILED;

   for(i=0;i<RG_MAX_TIMER;i++)
   {
      /* Search for the Timer Blocks */
      if(rgCb[inst].tmrBlk[i].tmrEvnt == tmrEvnt)
      {
         /* Initialize the arg structure */
         memset(&arg, 0, sizeof(CmTmrArg));

         arg.tqCp = &rgCb[inst].tmrTqCp;
         arg.tq = rgCb[inst].tmrTq;
         arg.timers = rgCb[inst].tmrBlk;
         arg.cb = cb;
         arg.max = RG_MAX_TIMER;
         arg.evnt = NOTUSED;
         arg.wait = NOTUSED;

         arg.tNum = i;   
         cmRmvCbTq(&arg);
         ret = ROK;
         break;
      }

   }


   return (ret);
}


/**
 * @brief Timer Expiry handler. 
 *
 * @details
 *
 *     Function : rgLMMTmrExpiry
 *     
 *     This is a callback function used as an input parameter to cmPrcTmr()
 *     to check expiry of any timer. In this function, the only concern is
 *     about tmrEvnt=Bind timer.
 *     
 *  @param[in]  PTR   cb,  Entry for which Timer expired
 *  @param[in]  S16   tmrEvnt, the Timer Event    
 *  @return  S16
 *      -# ROK
 **/
S16 rgLMMTmrExpiry
(
PTR cb,               /* Pointer to timer control block */
S16 tmrEvnt           /* Timer Event */
)
{
   S16        ret = ROK;
   RgLowSapCb *tfuSap = (RgLowSapCb *)cb;
   Inst          inst = tfuSap->sapCfg.sapPst.srcInst - RG_INST_START;

   switch(tmrEvnt)
   {
      case RG_BNDREQ_TMR:
         tfuSap->numBndRetries++;
         if(tfuSap->numBndRetries > RG_MAX_BNDRETRY)
         {
            rgLMMStaInd(inst,LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
                        LCM_CAUSE_TMR_EXPIRED, NULLP);
         }
         else
         {
            /* Restart the bind timer */
            if (tfuSap->sapCfg.bndTmr.enb == TRUE)
            {
               ret = rgLMMStartTmr(inst,RG_BNDREQ_TMR, tfuSap->sapCfg.bndTmr.val,
               cb);
            }

            /* Send bind request */
            //rgLIMTfuBndReq(inst,rgCb[inst].tfuSap.sapCfg.suId,
                                //rgCb[inst].tfuSap.sapCfg.spId);
         }
         break;
      default:
         DU_LOG("\nERROR  -->  MAC : Invalid tmrEvnt=%d",tmrEvnt);
         ret = RFAILED;
         break;
   }
   return (ret);
}



/**
 * @brief Layer Manager Unsolicited Status Indication generation. 
 *
 * @details
 *
 *     Function : rgLMMStaInd 
 *     
 *     This API is used by the other modules of MAC to send a unsolicited
 *     status indication to the Layer Manager.
 *     
 *  @param[in]  Inst        inst
 *  @param[in]  uint16_t category, the Alarm category
 *  @param[in]  uint16_t event, the Alarm event
 *  @param[in]  uint16_t cause, the cause of the Alarm
 *  @param[in]  RgUstaDgn *dgn, Alarm Diagonostics
 *  @return  S16
 *      -# ROK
 **/
S16 rgLMMStaInd
(
Inst inst,
uint16_t category,
uint16_t event,
uint16_t cause,
RgUstaDgn *dgn
)
{
   RgMngmt    usta;
   if(rgCb[inst].rgInit.usta == FALSE)
   {
      return ROK;
   }

   memset(&usta, 0, sizeof(RgMngmt));

   SGetDateTime(&usta.t.usta.cmAlarm.dt);
   usta.t.usta.cmAlarm.category = category;
   usta.t.usta.cmAlarm.event = event;
   usta.t.usta.cmAlarm.cause = cause;
   if (dgn != NULLP)
   {
      memcpy(&usta.t.usta.dgn, dgn, sizeof(RgUstaDgn));
   }

   rgCb[inst].rgInit.lmPst.selector = rgCb[inst].genCfg.ustaResp.response.selector;
   rgCb[inst].rgInit.lmPst.prior = rgCb[inst].genCfg.ustaResp.response.prior;
   rgCb[inst].rgInit.lmPst.route = rgCb[inst].genCfg.ustaResp.response.route;
   rgCb[inst].rgInit.lmPst.region = rgCb[inst].genCfg.ustaResp.response.mem.region;
   rgCb[inst].rgInit.lmPst.pool = rgCb[inst].genCfg.ustaResp.response.mem.pool;
   usta.hdr.transId = rgCb[inst].genCfg.ustaResp.transId;

   return (RgMiLrgStaInd(&rgCb[inst].rgInit.lmPst, &usta));
}


/**
 * @brief Layer Manager Trace Indication generation. 
 *
 * @details
 *
 *     Function : rgLMMTrcInd 
 *     
 *     This API is used by the other modules of MAC to send a 
 *     Trace indication to the Layer Manager.
 *     
 *  @param[in]  Inst        inst
 *  @param[in]   Buffer *srcMbuf, the Message Buffer .
 *  @param[in]   uint8_t event, the trace event.
 *  @return Void 
 **/
Void rgLMMTrcInd
(
Inst   inst,
Buffer *srcMbuf,    /* Message Buffer */
uint8_t event            /* event */
)
{
   Buffer   *dstMbuf = NULLP;   
   MsgLen   bufLen  = 0;
   Data     *tempBuf;
   MsgLen   tempCnt;
   RgMngmt  trc;
   Pst      pst;



   if ((rgCb[inst].trcLen == LRG_NO_TRACE) || (srcMbuf == NULLP))
   {
      DU_LOG("\nERROR  -->  MAC : Trace Disabled.");
      return;
   }
   
   memset(&trc, 0, sizeof(RgMngmt));

   pst = rgCb[inst].rgInit.lmPst;
   pst.selector = rgCb[inst].genCfg.trcResp.response.selector;
   pst.prior = rgCb[inst].genCfg.trcResp.response.prior;
   pst.route = rgCb[inst].genCfg.trcResp.response.route;
   pst.region = rgCb[inst].genCfg.trcResp.response.mem.region;
   pst.pool = rgCb[inst].genCfg.trcResp.response.mem.pool;

   trc.hdr.transId = rgCb[inst].genCfg.trcResp.transId;

   SGetDateTime(&trc.t.trc.dt);

   /* Check if the whole buffer is to be sent in Trace indication */
   if(rgCb[inst].trcLen == LRG_FULL_TRACE)
   {
      if (SCpyMsgMsg(srcMbuf, pst.region, pst.pool, &dstMbuf)
	    != ROK)
      {
	 DU_LOG("\nERROR  -->  MAC : SCpyMsgMsg Failed.");
	 return;
      }
      trc.cfm.status = LCM_PRIM_OK;
      trc.cfm.reason = LCM_REASON_NOT_APPL;
      trc.t.trc.evnt = event;

      /* Send Trace Indication to Layer manager */
      RgMiLrgTrcInd(&pst, &trc, dstMbuf);
   }
   /* check if only a specified number of bytes are to be sent */
   else if(rgCb[inst].trcLen > 0)
   {
      /* Get the length of the recvd message buffer */
      if (SFndLenMsg(srcMbuf, &bufLen) != ROK)
      {
	 DU_LOG("\nERROR  -->  MAC : SFndLenMsg Failed.");
	 return;
      }
      /* Check if the recvd buffer size is less than request trace len */
      if(bufLen < rgCb[inst].trcLen)
      {
	 /* Copy the whole of the recvd buffer in trace indication */

	 if (SCpyMsgMsg(srcMbuf, pst.region, pst.pool, &dstMbuf)
	       != ROK)
	 {
	    DU_LOG("\nERROR  -->  MAC : SCpyMsgMsg Failed.");
	    return;
	 }

	 trc.cfm.status = LCM_PRIM_OK;
	 trc.cfm.reason = LCM_REASON_NOT_APPL;
	 trc.t.trc.evnt = event;

	 /* Send Trace Indication to Layer manager */
	 RgMiLrgTrcInd(&pst, &trc, dstMbuf);
      }
      /* if the recvd buffer size is greater than request trace len */
      if(bufLen >= rgCb[inst].trcLen)
      {
	 /* Get a temporary buffer to store the msg */
	 if (rgAllocSBuf(inst,&tempBuf, rgCb[inst].trcLen) != ROK)
	 {
	    DU_LOG("\nERROR  -->  MAC : rgAllocSBuf Failed.");
	    return;
	 }

	 /* Copy trcLen nos of bytes from the recvd message */
	 if (SCpyMsgFix(srcMbuf,0,rgCb[inst].trcLen,tempBuf,&tempCnt) != ROK)   
	 {
	    DU_LOG("\nERROR  -->  MAC : SCpyMsgFix Failed.");
	    return;
	 }

	 if (ODU_GET_MSG_BUF(pst.region, pst.pool, &dstMbuf) != ROK)
	 {
	    DU_LOG("\nERROR  -->  MAC : dstMbuf Allocation Failed");
	    return;
	 }
	 /* Copy the tempBuf data to dst mBuf */
	 if (SCpyFixMsg(tempBuf,dstMbuf,0,rgCb[inst].trcLen,&tempCnt) != ROK)
	 {
	    DU_LOG("\nERROR  -->  MAC : SCpyFixMsg Failed.");
	    return;
	 }

	 /*ccpu00117052 - MOD - Passing double pointer for proper NULLP 
	   assignment */
	 /* Free the memory allocated for tempBuf */
	 rgFreeSBuf(inst,&tempBuf, rgCb[inst].trcLen);

	 trc.cfm.status = LCM_PRIM_OK;
	 trc.cfm.reason = LCM_REASON_NOT_APPL;
	 trc.t.trc.evnt = event;

	 /* Send Trace Indication to Layer manager */
	 RgMiLrgTrcInd(&pst, &trc, dstMbuf);
      }
   }
   return;
}


/**
 * @brief Layer Manager Control Confirm generation handler
 *        for Bind Confirm reception at TFU interface.
 *        RgLiTfuBndCfm() forwards the confirmation to this 
 *        function. All SAP state related handling is restricted
 *        to LMM modules, hence the cfm forwarding.
 *
 * @details
 *
 *     Function : rgLMMBndCfm 
 *     
 *     This API is used by the LIM module of MAC to forward
 *     the Bind Confirm it receives over the TFU interface.
 *     
 *  @param[in]   Pst *pst, Post Structure
 *  @param[in]   SuId suId, Service user ID
 *  @param[in]   uint8_t status, Status
 *  @return  S16
 *      -# ROK
 **/
S16 rgLMMBndCfm
(
Pst *pst,               /* Post Structure */
SuId suId,              /* Service user ID */
uint8_t status               /* Status */
)
{
   Inst      inst = pst->dstInst - RG_INST_START;
   S16       ret = ROK;
   RgMngmt   cntrlCfm;
   Pst       cfmPst;


   UNUSED(pst);

   /* Check if the suId is valid */
   if(rgCb[inst].tfuSap.sapCfg.suId != suId)
   {
      DU_LOG("\nERROR  -->  MAC : Invalid SuId");
      return RFAILED;
   }

   /* check the Sap State */
   switch(rgCb[inst].tfuSap.sapSta.sapState)
   {
      case LRG_WAIT_BNDCFM:
	 break;
      case LRG_BND:
	 /* SAP is already bound */
	 return ROK;
      default:
	 return RFAILED;
   }

   cfmPst = rgCb[inst].rgInit.lmPst;
   cfmPst.selector = rgCb[inst].genCfg.bndCfmResp.response.selector;
   cfmPst.prior = rgCb[inst].genCfg.bndCfmResp.response.prior;
   cfmPst.route = rgCb[inst].genCfg.bndCfmResp.response.route;
   cfmPst.region = rgCb[inst].genCfg.bndCfmResp.response.mem.region;
   cfmPst.pool = rgCb[inst].genCfg.bndCfmResp.response.mem.pool;
   
   memset(&cntrlCfm, 0, sizeof(RgMngmt));

   switch(status)
   {
      case CM_BND_OK: /* status is OK */
	 /* Change SAP state to Bound */
	 rgCb[inst].tfuSap.sapSta.sapState = LRG_BND;
	 if (rgCb[inst].tfuSap.sapCfg.bndTmr.enb == TRUE)
	 {
	    ret = rgLMMStopTmr(inst,RG_BNDREQ_TMR, (PTR)&rgCb[inst].tfuSap);
	 }
	 /* Send Control Confirm with status as OK to Layer Manager */
	 cntrlCfm.cfm.status = LCM_PRIM_OK;
	 cntrlCfm.cfm.reason = LCM_REASON_NOT_APPL;
	 break;

      default:
	 /* Change SAP state to UnBound */
	 rgCb[inst].tfuSap.sapSta.sapState = LRG_UNBND;
	 if (rgCb[inst].tfuSap.sapCfg.bndTmr.enb == TRUE)
	 {
	    ret = rgLMMStopTmr(inst,RG_BNDREQ_TMR, (PTR)&rgCb[inst].tfuSap);
	 }
	 /* Send Control Confirm with status as NOK to Layer Manager */
	 cntrlCfm.cfm.status = LCM_PRIM_NOK;
	 cntrlCfm.cfm.reason = LCM_REASON_NEG_CFM;
	 break;
   }
   rgCb[inst].tfuSap.numBndRetries = 0;
   cntrlCfm.hdr.elmId.elmnt = STTFUSAP;
   cntrlCfm.hdr.transId = rgCb[inst].genCfg.bndCfmResp.transId;

   ret = RgMiLrgCntrlCfm(&cfmPst, &cntrlCfm);

   return (ret);
}

/**********************************************************************

  End of file
 **********************************************************************/
