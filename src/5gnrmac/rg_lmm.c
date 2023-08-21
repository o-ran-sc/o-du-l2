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

static Void rgLMMFillCfmPst ARGS((
   Pst           *reqPst,
   Pst           *cfmPst,
   RgMngmt       *cfm
));


/**
 * @brief Task Initiation callback function. 
 *
 * @details
 *
 *     Function : macActvInit
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
uint8_t macActvInit
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

   /* Initialize lower mac */
   lwrMacLayerInit(region, 0);

   return ROK;

} /* macActvInit */

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



   DU_LOG("\nINFO    -->  MAC : Received CfgReq for MAC layer, Entity = %d, Instance = %d", pst->srcEnt, pst->srcInst);

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

/**********************************************************************

  End of file
 **********************************************************************/
