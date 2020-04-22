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
static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_FILE_ID=220;
static int RLOG_MODULE_ID=4096;

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_lte.h"        /* Common LTE Defines */
#include "rg_env.h"        /* MAC Environment Defines */
#include "crg.h"           /* CRG Interface defines */
#include "rgu.h"           /* RGU Interface defines */
#include "tfu.h"           /* RGU Interface defines */
#include "rg_sch_inf.h"    /* RGR Interface defines */
#include "lrg.h"           /* LRG Interface defines */
#include "rgr.h"           /* LRG Interface defines */
#include "rg.h"            /* MAC defines */
#include "rg_err.h"        /* MAC error defines */
#include "du_log.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"       /* common memory link list library */
#include "cm_lte.x"        /* Common LTE Defines */
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

#include "mac.h"
#include "lwr_mac_phy.h"
#include "lwr_mac_fsm.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
EXTERN Void rgGetSId ARGS((SystemId *s));
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* Public variable declaration */
ClCb   clGlobalCp;
MacCb  macCb;

/* forward references */
PRIVATE U16 rgLMMGenCfg ARGS((
   Inst           inst,
   RgCfg          *cfg           
));

PRIVATE U16 rgLMMSapCfg ARGS((
   Inst           inst,
   RgCfg          *cfg,
   Elmnt          sapType
));

PRIVATE Void rgLMMShutdown ARGS((
   Inst           inst
));

PRIVATE Void rgLMMFillCfmPst ARGS((
   Pst           *reqPst,
   Pst           *cfmPst,
   RgMngmt       *cfm
));

PRIVATE Void rgLMMGenCntrl ARGS((
RgMngmt       *cntrl,
RgMngmt       *cfm,
Pst           *cfmPst
));

PRIVATE Void rgLMMSapCntrl ARGS((
RgMngmt       *cntrl,
RgMngmt       *cfm,
Pst           *cfmPst
));

extern U16 packMacCellCfgCfm(Pst *pst, MacCellCfgCfm *macCellCfgCfm);

packMacCellCfgConfirm packMacCellCfmOpts[] =
{
   packMacCellCfgCfm,      /* packing for loosely coupled */
   duHandleMacCellCfgCfm,      /* packing for tightly coupled */
   packMacCellCfgCfm,    /* packing for light weight loosly coupled */
};

SchCellCfgFunc SchCellCfgOpts[] = 
{
   packSchCellCfg,   /* packing for loosely coupled */
	SchHdlCellCfgReq, /* packing for tightly coupled */
   packSchCellCfg    /* packing for light weight loosly coupled */
};


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
#ifdef ANSI
PUBLIC S16 rgActvInit
(
Ent entity,            /* entity */
Inst inst,             /* instance */
Region region,         /* region */
Reason reason          /* reason */
)
#else
PUBLIC S16 rgActvInit(entity, inst, region, reason)
Ent entity;            /* entity */
Inst inst;             /* instance */
Region region;         /* region */
Reason reason;         /* reason */
#endif
{
   Inst macInst ;
   TRC2(rgActvInit);

   RG_IS_INST_VALID(inst);

   macInst = inst - RG_INST_START;
   /* Initialize the MAC TskInit structure to zero */
   cmMemset ((U8 *)&rgCb[macInst], 0, sizeof(RgCb));

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

   /* Initializing CL control block */
   clGlobalCp.region = region;
   clGlobalCp.pool = 0;
   clGlobalCp.clCfgDone = FALSE;
   clGlobalCp.numOfCells = 0;
   clGlobalCp.phyState = PHY_STATE_IDLE; 

   if( cmHashListInit(&clGlobalCp.cellCbLst, MAX_NUM_CELL_SUPP, 0x0, FALSE, 
                  CM_HASH_KEYTYPE_DEF, clGlobalCp.region, clGlobalCp.pool ) != ROK )
   {
      printf("\n Cellcb hash list initialization failed for MAC CL");
      RETVALUE(RFAILED);
   }

   /* Initialize Scheduler as well */
   schActvInit(ENTRG, (DEFAULT_CELLS + SCH_INST_START), DFLT_REGION, PWR_UP);

   /* Initialize lower mac */
   lwrMacInit();

   RETVALUE(ROK);

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
#ifdef ANSI
PUBLIC S16 RgMiLrgCfgReq
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfg     /* config structure  */
)
#else
PUBLIC S16 RgMiLrgCfgReq(pst, cfg)
Pst      *pst;    /* post structure  */
RgMngmt  *cfg;    /* config structure  */
#endif    
{
   U16       ret = LCM_PRIM_OK;
   U16       reason = LCM_REASON_NOT_APPL;
   RgMngmt   cfm;
   Pst       cfmPst;
   Inst      inst;

   TRC2(RgMiLrgCfgReq)
   

   RG_DIAG_LVL0(inst,0x0a0b0001, RG_DIAG_NA, SS_DIAG_INV_ARG,
              "Received CfgReq for MAC layer, Entity = %d, Instance = %d\n",
              pst->srcEnt, pst->srcInst,0,0);

   RG_IS_INST_VALID(pst->dstInst);
   inst = pst->dstInst - RG_INST_START;

   /* Fill the post structure for sending the confirmation */
   rgLMMFillCfmPst(pst, &cfmPst, cfg);

   cmMemset((U8 *)&cfm, 0, sizeof(RgMngmt));

#ifdef LMINT3
   cfm.hdr.transId = cfg->hdr.transId;
#endif


   cfm.hdr.elmId.elmnt = cfg->hdr.elmId.elmnt;
   switch(cfg->hdr.elmId.elmnt)
   {
      case STGEN:
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
         RLOG1(L_ERROR, "Invalid Elmnt=%d",
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
   
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 RgMiLrgStsReq
(
Pst      *pst,    /* post structure  */
RgMngmt  *sts     /* statistics structure  */
)
#else
PUBLIC S16 RgMiLrgStsReq(pst, sts)
Pst      *pst;    /* post structure  */
RgMngmt  *sts;    /* statistics structure  */
#endif    
{
   Pst       cfmPst;
   RgMngmt   cfm;
   Inst      inst;

   TRC2(RgMiLrgStsReq)
   

   RG_IS_INST_VALID(pst->dstInst);
   inst = pst->dstInst - RG_INST_START;
   /* Fill the post structure for sending the confirmation */
   rgLMMFillCfmPst(pst, &cfmPst, sts);

   cmMemset((U8 *)&cfm, 0, sizeof(RgMngmt));

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
      RLOG0(L_ERROR, "Gen Cfg not done");
      RETVALUE(ROK);
   }

   switch(sts->hdr.elmId.elmnt)
   {
      case STGEN:
         {
            cmMemcpy((U8 *)&(cfm.t.sts.s.genSts), (U8 *)&rgCb[inst].genSts,
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
               U8       cqi = 0;
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
                  cmMemset((U8 *)&hqRetxStats, 0, \
                        sizeof(RgSchHqRetxStats));
                  cmMemset((U8 *)&hqFailStats, 0, \
                        sizeof(RgSchNackAckStats));
               }
            }
#endif /* MAC_SCH_STATS*/
         }
         break;
      case STRGUSAP:
         cmMemcpy((U8 *)&(cfm.t.sts.s.rguSts), (U8 *)&rgCb[inst].rguSap[sts->t.sts.sapInst].sapSts,
                  sizeof(RgSapSts));

         /* check if action is read and reset */
         if(sts->t.sts.action == ARST)
            cmMemset((U8 *)&rgCb[inst].rguSap[sts->t.sts.sapInst].sapSts, 0, sizeof(RgSapSts));

         break;
      case STCRGSAP:
         cmMemcpy((U8 *)&(cfm.t.sts.s.crgSts), (U8 *)&rgCb[inst].crgSap.sapSts,
                  sizeof(RgSapSts));

         /* check if action is read and reset */
         if(sts->t.sts.action == ARST)
            cmMemset((U8 *)&rgCb[inst].crgSap.sapSts, 0, sizeof(RgSapSts));

         break;
      case STTFUSAP:
         cmMemcpy((U8 *)&(cfm.t.sts.s.tfuSts), (U8 *)&rgCb[inst].tfuSap.sapSts,
                  sizeof(RgSapSts));

         /* check if action is read and reset */
         if(sts->t.sts.action == ARST)
            cmMemset((U8 *)&rgCb[inst].tfuSap.sapSts, 0, sizeof(RgSapSts));

         break;
      default:
         cfm.cfm.status = LCM_PRIM_NOK;
         cfm.cfm.reason = LCM_REASON_INVALID_ELMNT;
         RLOG1(L_ERROR, "Invalid Elmnt = %d",sts->hdr.elmId.elmnt);
         break;     
   }
   RgMiLrgStsCfm(&cfmPst,&cfm);
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 RgMiLrgStaReq
(
Pst      *pst,    /* post structure  */
RgMngmt  *sta     /* status structure  */
)
#else
PUBLIC S16 RgMiLrgStaReq(pst, sta)
Pst      *pst;    /* post structure  */
RgMngmt  *sta;    /* status structure  */
#endif    
{
   Pst       cfmPst;
   RgMngmt   cfm;
   Inst      inst ;

   TRC2(RgMiLrgStaReq)
   

   RG_IS_INST_VALID(pst->dstInst);
   inst = pst->dstInst - RG_INST_START;


   /* Fill the post structure for sending the confirmation */
   rgLMMFillCfmPst(pst, &cfmPst, sta);

   if (sta->t.ssta.s.sysId.ptNmb != NULLP)
   {
      SPutSBuf(pst->region, pst->pool, (Data *)sta->t.ssta.s.sysId.ptNmb, LRG_MAX_PT_NUM_SIZE);
   }
   
   cmMemset((U8 *)&cfm, 0, sizeof(RgMngmt));
   cfm.hdr.elmId.elmnt = sta->hdr.elmId.elmnt;

#ifdef LMINT3
   cfm.hdr.transId = sta->hdr.transId;
#endif
   /* Check if General Config Done */
   if(rgCb[inst].rgInit.cfgDone != TRUE) 
   {
      SGetDateTime(&cfm.t.ssta.dt);
      if (SGetSBuf(cfmPst.region, cfmPst.pool, 
          (Data **)&(cfm.t.ssta.s.sysId.ptNmb), LRG_MAX_PT_NUM_SIZE)
         != ROK)
      {
         RLOG0(L_ERROR, "Memory Unavailable for Confirmation");
         RETVALUE(ROK);
      } 
      cmMemset((U8 *)(cfm.t.ssta.s.sysId.ptNmb), 0, LRG_MAX_PT_NUM_SIZE);
      rgGetSId(&cfm.t.ssta.s.sysId);
      cfm.cfm.status = LCM_PRIM_NOK;
      cfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      cfm.hdr.elmId.elmnt = sta->hdr.elmId.elmnt;
      RgMiLrgStaCfm(&cfmPst, &cfm);
      RLOG0(L_ERROR, "Gen Cfg not done");
      RETVALUE(ROK);
   }

   switch(sta->hdr.elmId.elmnt)
   {
      case STGEN:
         SGetDateTime(&cfm.t.ssta.dt);
         if (SGetSBuf(cfmPst.region, cfmPst.pool, 
             (Data **)&(cfm.t.ssta.s.sysId.ptNmb), LRG_MAX_PT_NUM_SIZE)
            != ROK)
         {
            RLOG0(L_ERROR, "Memory Unavailable for Confirmation");
            RETVALUE(ROK);
         } 
         cmMemset((U8 *)(cfm.t.ssta.s.sysId.ptNmb), 0, LRG_MAX_PT_NUM_SIZE);
         rgGetSId(&cfm.t.ssta.s.sysId);
         cfm.cfm.status = LCM_PRIM_OK;
         cfm.cfm.reason = LCM_REASON_NOT_APPL;
         RgMiLrgStaCfm(&cfmPst, &cfm);
         break;
      case STRGUSAP:
         cfm.cfm.status = LCM_PRIM_OK;
         cfm.cfm.reason = LCM_REASON_NOT_APPL;
         SGetDateTime(&cfm.t.ssta.dt);
         cmMemcpy((U8 *)&(cfm.t.ssta.s.rguSapSta), 
				(U8 *)&rgCb[inst].rguSap[sta->t.ssta.sapInst].sapSta,
            sizeof(RgSapSta));
         RgMiLrgStaCfm(&cfmPst, &cfm);
         break;
      case STCRGSAP:
         cfm.cfm.status = LCM_PRIM_OK;
         cfm.cfm.reason = LCM_REASON_NOT_APPL;
         SGetDateTime(&cfm.t.ssta.dt);
         cmMemcpy((U8 *)&(cfm.t.ssta.s.crgSapSta), (U8 *)&rgCb[inst].crgSap.sapSta,
         sizeof(RgSapSta));
         RgMiLrgStaCfm(&cfmPst, &cfm);
         break;
      case STTFUSAP:
         cfm.cfm.status = LCM_PRIM_OK;
         cfm.cfm.reason = LCM_REASON_NOT_APPL;
         SGetDateTime(&cfm.t.ssta.dt);
         cmMemcpy((U8 *)&(cfm.t.ssta.s.tfuSapSta), (U8 *)&rgCb[inst].tfuSap.sapSta,
         sizeof(RgSapSta));
         RgMiLrgStaCfm(&cfmPst, &cfm);
         break;
      default:
         cfm.cfm.status = LCM_PRIM_NOK;
         cfm.cfm.reason = LCM_REASON_INVALID_ELMNT;
         RgMiLrgStaCfm(&cfmPst, &cfm);
         RLOG1(L_ERROR, "Invalid elmnt=%d",sta->hdr.elmId.elmnt);
         break;     
   }
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 RgMiLrgCntrlReq
(
Pst      *pst,    /* post structure  */
RgMngmt  *cntrl   /* control structure  */
)
#else
PUBLIC S16 RgMiLrgCntrlReq(pst, cntrl)
Pst      *pst;    /* post structure  */
RgMngmt  *cntrl;  /* control structure  */
#endif    
{
   S16       ret = ROK;            /* return value */
   Pst       cfmPst;
   RgMngmt   cfm;
   Inst      inst;
   
   TRC2(RgMiLrgCntrlReq)
   
   /* Fill the post structure for sending the confirmation */

   RG_IS_INST_VALID(pst->dstInst);
   inst = pst->dstInst - RG_INST_START;

   rgLMMFillCfmPst(pst, &cfmPst, cntrl);

   cmMemset((U8 *)&cfm, 0, sizeof(RgMngmt));
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
      RLOG0(L_ERROR, "Gen Cfg not done");
      RETVALUE(ROK);
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
         RLOG1(L_ERROR, "invalid elmnt=%d",cntrl->hdr.elmId.elmnt);
         break;
   }
   RETVALUE(ret);
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
 *  @return  U16
 *      -# LCM_REASON_GENCFG_NOT_DONE
 *      -# LCM_REASON_INVALID_SAP
 *      -# LCM_REASON_NOT_APPL
 **/
#ifdef ANSI
PRIVATE U16 rgLMMSapCfg
(
Inst  inst,
RgCfg *cfg,            /* Configuaration information */
Elmnt sapType             /* Sap Type */
)
#else
PRIVATE U16 rgLMMSapCfg(inst,cfg,sapType)
Inst  inst;
RgCfg *cfg;            /* Configuaration information */
Elmnt sapType;            /* Sap Type */
#endif
{
   U16               ret = LCM_REASON_NOT_APPL;
   RgLowSapCfgInfo   *lowSapCfg = NULLP;
   RgUpSapCfgInfo    *upSapCfg = NULLP;
   RgUpSapCb          *upSapCb  = NULLP;

   TRC2(rgLMMSapCfg)

   /* Check if Gen Config has been done */
   if(rgCb[inst].rgInit.cfgDone != TRUE)
      RETVALUE(LCM_REASON_GENCFG_NOT_DONE);

   switch(sapType)
   {   
      case STRGUSAP:
         if ((cfg->s.rguSap.spId > LRG_MAX_RGU_SAPS) &&
             (cfg->s.rguSap.selector != RGU_SEL_TC) &&
             (cfg->s.rguSap.selector != RGU_SEL_LC))
         {
            ret = LCM_REASON_INVALID_PAR_VAL;
            RLOG0(L_ERROR, "unsupported Selector value for RGU");
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
         if ((cfg->s.crgSap.selector != CRG_SEL_TC) &&
             (cfg->s.crgSap.selector != CRG_SEL_LC))
         {
            ret = LCM_REASON_INVALID_PAR_VAL;
            RLOG0(L_ERROR, "unsupported Selector value for CRG");
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
         if ((cfg->s.tfuSap.selector != TFU_SEL_TC) &&
             (cfg->s.tfuSap.selector != TFU_SEL_LC))
         {
            ret = LCM_REASON_INVALID_PAR_VAL;
            RLOG0(L_ERROR, "unsupported Selector value for TFU");
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
         cmMemcpy((U8 *)&lowSapCfg->bndTmr, (U8 *)&cfg->s.tfuSap.bndTmr,
                   sizeof(TmrCfg));
         break;
      default:
         /* would never reach here */
         break;
   }
   RETVALUE(ret);
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
 *  @return  U16
 *      -# LCM_REASON_NOT_APPL 
 *      -# LCM_REASON_INVALID_MSGTYPE
 *      -# LCM_REASON_MEM_NOAVAIL
 **/
#ifdef ANSI
PRIVATE U16 rgLMMGenCfg
(
Inst inst,
RgCfg *cfg            /* Configuaration information */
)
#else
PRIVATE U16 rgLMMGenCfg(inst,cfg)
Inst inst;
RgCfg *cfg;            /* Configuaration information */
#endif
{
   U16    ret = LCM_REASON_NOT_APPL;

   TRC2(rgLMMGenCfg)

   /* Check if General Configuration is done already */
   if (rgCb[inst].rgInit.cfgDone == TRUE)
   {
      RETVALUE(LCM_REASON_INVALID_MSGTYPE);
   }
   if ((cfg->s.genCfg.lmPst.selector != LRG_SEL_TC) &&
       (cfg->s.genCfg.lmPst.selector != LRG_SEL_LC))
   {
      RLOG0(L_ERROR, "unsupported Selector value for RGU");
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   /* Update the Pst structure for LM interface */
   cmMemcpy((U8 *)&rgCb[inst].rgInit.lmPst, (U8 *)&cfg->s.genCfg.lmPst,
             sizeof(Pst));

   rgCb[inst].rgInit.lmPst.srcProcId = rgCb[inst].rgInit.procId;
   rgCb[inst].rgInit.lmPst.srcEnt = rgCb[inst].rgInit.ent;
   rgCb[inst].rgInit.lmPst.srcInst = rgCb[inst].rgInit.inst;
   rgCb[inst].rgInit.lmPst.event = EVTNONE;

   rgCb[inst].rgInit.region = cfg->s.genCfg.mem.region;
   rgCb[inst].rgInit.pool = cfg->s.genCfg.mem.pool;
   rgCb[inst].genCfg.tmrRes = cfg->s.genCfg.tmrRes;

   macCb.macInst = rgCb[inst].rgInit.inst;

   /* Initialize SAP States */
   rgCb[inst].crgSap.sapSta.sapState = LRG_NOT_CFG;

   if(cfg->s.genCfg.numRguSaps == 0)
   {
      RGDBGERRNEW(inst,(rgPBuf(inst), "\nrgGenCfg(): Invalid numRguSap.\n"));
      RETVALUE(RFAILED);
   }

   /* allocate RGR saps */
   if (SGetSBuf(rgCb[inst].rgInit.region,
                rgCb[inst].rgInit.pool,
                (Data **)&rgCb[inst].rguSap,
                (sizeof(RgUpSapCb) * cfg->s.genCfg.numRguSaps)) != ROK)
   {
      RGDBGERRNEW(inst,(rgPBuf(inst), "\nrgGenCfg(): Failed to allocate mem for RGU SAP's.\n"));
      RETVALUE(RFAILED);
   }
   rgCb[inst].numRguSaps = cfg->s.genCfg.numRguSaps;

   for (int idx = 0; idx < rgCb[inst].numRguSaps; idx++)
   {
      rgCb[inst].rguSap[idx].sapSta.sapState = LRG_NOT_CFG;
      cmMemset((U8 *)&rgCb[inst].rguSap[idx], 0, sizeof(RgUpSapCb));
   }
   rgCb[inst].tfuSap.sapSta.sapState = LRG_NOT_CFG;
   /* Initialize the timer blocks */
   cmInitTimers(rgCb[inst].tmrBlk, RG_MAX_TIMER);
   /* Initialzie the timer queue */   
   cmMemset((U8 *)&rgCb[inst].tmrTq, 0, sizeof(CmTqType)*RG_TQ_SIZE);
   /* Initialize the timer control point */
   cmMemset((U8 *)&rgCb[inst].tmrTqCp, 0, sizeof(CmTqCp));
   rgCb[inst].tmrTqCp.tmrLen = RG_TQ_SIZE;
#if 0
   /* Timer Registration request to SSI */
   if (SRegTmrMt(rgCb[inst].rgInit.ent, rgCb[inst].rgInit.inst,
            (S16)rgCb[inst].genCfg.tmrRes, rgActvTmr) != ROK)
   {
      
      RLOG0(L_ERROR, "Failed to register timer");

      SPutSBuf(rgCb[inst].rgInit.region,
                rgCb[inst].rgInit.pool,
                (Data *)rgCb[inst].rguSap,
                (sizeof(RgUpSapCb) * cfg->s.genCfg.numRguSaps));

      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }
#endif
   /* Set Config done in TskInit */
   rgCb[inst].rgInit.cfgDone = TRUE;

   RETVALUE(ret);
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
#ifdef ANSI
PRIVATE Void rgLMMShutdown
(
Inst inst
)
#else
PRIVATE Void rgLMMShutdown(inst)
Inst inst;
#endif
{
   RgCellCb   *cell = rgCb[inst].cell;
   U8 idx;

   TRC2(rgLMMShutdown)

   /* Unbind the TFU Sap */
   if(rgCb[inst].tfuSap.sapSta.sapState == LRG_WAIT_BNDCFM)
   {
      rgLIMTfuUbndReq(inst,rgCb[inst].tfuSap.sapCfg.spId, LRG_UNBND);
      if (rgCb[inst].tfuSap.sapCfg.bndTmr.enb == TRUE)
      {
         rgLMMStopTmr(inst,RG_BNDREQ_TMR, (PTR)&rgCb[inst].tfuSap); 
      }
      rgCb[inst].tfuSap.sapSta.sapState = LRG_UNBND;
   }
   if(rgCb[inst].tfuSap.sapSta.sapState == LRG_BND)
   {
      rgLIMTfuUbndReq(inst,rgCb[inst].tfuSap.sapCfg.spId, LRG_UNBND);
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
   SPutSBuf(rgCb[inst].rgInit.region,
                rgCb[inst].rgInit.pool,
                (Data *)rgCb[inst].rguSap,
                (sizeof(RgUpSapCb) * rgCb[inst].numRguSaps));
   rgCb[inst].rguSap = NULLP;

   rgCb[inst].inactiveCell = NULLP;
   rgCb[inst].cell         = NULLP;

   /* De-register the Timer Service */
   (Void) SDeregTmrMt(rgCb[inst].rgInit.ent, rgCb[inst].rgInit.inst,
                     (S16)rgCb[inst].genCfg.tmrRes, rgActvTmr); 

   /* call back the task initialization function to intialize
    * the global RgCb Struct */
   rgActvInit(rgCb[inst].rgInit.ent, rgCb[inst].rgInit.inst, rgCb[inst].rgInit.region, 
              rgCb[inst].rgInit.reason);

   RETVOID;
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
#ifdef ANSI
PRIVATE Void rgLMMGenCntrl 
(
RgMngmt       *cntrl,
RgMngmt       *cfm,
Pst           *cfmPst
)
#else
PRIVATE Void rgLMMGenCntrl(cntrl, cfm, cfmPst)
RgMngmt       *cntrl;
RgMngmt       *cfm;
Pst           *cfmPst;
#endif
{
   Inst      inst = (cfmPst->srcInst - RG_INST_START);
   TRC2(rgLMMGenCntrl)

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
               cmMemcpy((U8 *)&rgCb[inst].genCfg.trcResp.response, 
               (U8 *)&cntrl->hdr.response, sizeof(Resp));
               rgCb[inst].genCfg.trcResp.transId = cntrl->hdr.transId;
               
               break;
            case SAUSTA:   
            /* Enable Unsolicited Status (alarms) */
               rgCb[inst].rgInit.usta = TRUE;
               /*Store the response and TransId for sending the Alarms */
               cmMemcpy((U8 *)&rgCb[inst].genCfg.ustaResp.response, 
               (U8 *)&cntrl->hdr.response, sizeof(Resp));
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
               RLOG1(L_ERROR, "invalid subaction=%d",cntrl->t.cntrl.subAction);
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
               RLOG1(L_ERROR, "invalid subaction=%d",cntrl->t.cntrl.subAction);
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
         RLOG1(L_ERROR, "invalid action=%d",cntrl->t.cntrl.action);
         break;
   }
   RgMiLrgCntrlCfm(cfmPst, cfm);
   RETVOID;
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
#ifdef ANSI
PRIVATE Void rgLMMSapCntrl 
(
RgMngmt       *cntrl,
RgMngmt       *cfm,
Pst           *cfmPst
)
#else
PRIVATE Void rgLMMSapCntrl(cntrl, cfm, cfmPst)
RgMngmt       *cntrl;
RgMngmt       *cfm;
Pst           *cfmPst;
#endif
{
   Inst      inst = cfmPst->srcInst - RG_INST_START;
   TRC2(rgLMMSapCntrl)

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
                  cmMemcpy((U8 *)&rgCb[inst].genCfg.bndCfmResp.response,
                           (U8 *)&cntrl->hdr.response, sizeof(Resp));
                  rgCb[inst].genCfg.bndCfmResp.transId = cntrl->hdr.transId;

                  /* Sending Status Indication to Layer Manager */
                  cfm->cfm.status = LCM_PRIM_OK_NDONE;
                  cfm->cfm.reason = LCM_REASON_NOT_APPL;
                  RgMiLrgCntrlCfm(cfmPst, cfm);

                  rgLIMTfuBndReq(inst,rgCb[inst].tfuSap.sapCfg.suId,
                                      rgCb[inst].tfuSap.sapCfg.spId);
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
                  rgLIMTfuUbndReq(inst,rgCb[inst].tfuSap.sapCfg.spId, TFU_UBNDREQ_MNGMT);
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
                  rgLIMTfuUbndReq(inst,rgCb[inst].tfuSap.sapCfg.spId, TFU_UBNDREQ_MNGMT);
                  if (rgCb[inst].tfuSap.sapCfg.bndTmr.enb == TRUE)
                  {
                     rgLMMStopTmr(inst,RG_BNDREQ_TMR, (PTR)&rgCb[inst].tfuSap);
                  }
               }
               cmMemset((U8 *)&rgCb[inst].tfuSap, 0, sizeof(RgLowSapCb));
               rgCb[inst].tfuSap.sapSta.sapState = LRG_NOT_CFG;
               cfm->cfm.status = LCM_PRIM_OK;
               cfm->cfm.reason = LCM_REASON_NOT_APPL;
               break;
            default:
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
               RGDBGERRNEW(inst,(rgPBuf(inst), "\nrgLMMSapCntrl(): invalid action=%d",
               cntrl->t.cntrl.action));
               break;
         }
         break;
      case STRGUSAP:
         switch(cntrl->t.cntrl.action)
         {
            case ADEL:
               cmMemset((U8 *)&rgCb[inst].rguSap[cntrl->t.cntrl.instId], 0, sizeof(RgUpSapCb));
               rgCb[inst].rguSap[cntrl->t.cntrl.instId].sapSta.sapState = LRG_NOT_CFG;
               cfm->cfm.status = LCM_PRIM_OK;
               cfm->cfm.reason = LCM_REASON_NOT_APPL;
               break;
            default:
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
               RGDBGERRNEW(inst,(rgPBuf(inst), "\nrgLMMSapCntrl(): invalid action=%d",
               cntrl->t.cntrl.action));
               break;
         }
         break;
      case STCRGSAP:
         switch(cntrl->t.cntrl.action)
         {
            case ADEL:
               cmMemset((U8 *)&rgCb[inst].crgSap, 0, sizeof(RgUpSapCb));
               rgCb[inst].crgSap.sapSta.sapState = LRG_NOT_CFG;
               cfm->cfm.status = LCM_PRIM_OK;
               cfm->cfm.reason = LCM_REASON_NOT_APPL;
               break;
            default:
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
               RLOG1(L_ERROR, "invalid action=%d",cntrl->t.cntrl.action);
               
               break;
         }
         break;
      default:
         /* Would never here. */
         RETVOID;
   }
   RgMiLrgCntrlCfm(cfmPst, cfm);
   RETVOID;
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
#ifdef ANSI
PRIVATE Void rgLMMFillCfmPst
(
Pst           *reqPst,
Pst           *cfmPst,
RgMngmt       *cfm
)
#else
PRIVATE Void rgLMMFillCfmPst(reqPst, cfmPst, cfm)
Pst           *reqPst;
Pst           *cfmPst;
RgMngmt       *cfm;
#endif
{
   Inst inst;
   TRC2(rgLMMFillCfmPst)
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

   RETVOID;
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
 *  @param[in]  U32   tmrVal,  the Wait Time
 *  @param[in]  PTR   cb,  Entry for which Timer expired
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgLMMStartTmr
(
Inst               inst,
S16                tmrEvnt,            /* Timer Event */
U32                tmrVal,             /* Wait Time */
PTR                cb                  /* Entry for which Timer Expired */
)
#else
PUBLIC S16 rgLMMStartTmr(tmrEvnt, tmrVal, cb)
Inst               inst;
S16                tmrEvnt;            /* Timer Event */
U32                tmrVal;             /* Wait Time */
PTR                cb;                 /* Entry for which Timer Expired */
#endif
{
   CmTmrArg    arg;

   TRC2(rgLMMStartTmr)

   UNUSED(tmrEvnt);

   /* Initialize the arg structure */
   cmMemset((U8 *)&arg, 0, sizeof(CmTmrArg));

   arg.tqCp = &rgCb[inst].tmrTqCp;
   arg.tq = rgCb[inst].tmrTq;
   arg.timers = rgCb[inst].tmrBlk;
   arg.cb = cb;
   arg.tNum = 0;
   arg.max = RG_MAX_TIMER;
   arg.evnt = RG_BNDREQ_TMR;
   arg.wait = tmrVal;      
   cmPlcCbTq(&arg);

   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 rgLMMStopTmr
(
Inst               inst,             /* Scheduler instance */
S16                tmrEvnt,            /* Timer Event */
PTR                cb                  /* Entry for which Timer Expired */
)
#else
PUBLIC S16 rgLMMStopTmr(inst,tmrEvnt, cb)
Inst               inst;             /* Scheduler instance */
S16                tmrEvnt;            /* Timer Event */
PTR                cb;                 /* Entry for which Timer Expired */
#endif
{
   CmTmrArg   arg;
   U8         i;
   S16        ret; 

   TRC2(rgLMMStopTmr)

   ret = RFAILED;

   for(i=0;i<RG_MAX_TIMER;i++)
   {
      /* Search for the Timer Blocks */
      if(rgCb[inst].tmrBlk[i].tmrEvnt == tmrEvnt)
      {
         /* Initialize the arg structure */
         cmMemset((U8 *)&arg, 0, sizeof(CmTmrArg));

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


   RETVALUE(ret);
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
#ifdef ANSI
PUBLIC S16 rgLMMTmrExpiry
(
PTR cb,               /* Pointer to timer control block */
S16 tmrEvnt           /* Timer Event */
)
#else
PUBLIC S16 rgLMMTmrExpiry(cb,tmrEvnt)
PTR cb;               /* Pointer to timer control block */
S16 tmrEvnt;          /* Timer Event */
#endif
{
   S16        ret = ROK;
   RgLowSapCb *tfuSap = (RgLowSapCb *)cb;
   Inst          inst = tfuSap->sapCfg.sapPst.srcInst - RG_INST_START;

   TRC2(rgLMMTmrExpiry)

   
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
            rgLIMTfuBndReq(inst,rgCb[inst].tfuSap.sapCfg.suId,
                                rgCb[inst].tfuSap.sapCfg.spId);
         }
         break;
      default:
         RLOG1(L_ERROR, "Invalid tmrEvnt=%d",tmrEvnt);
         ret = RFAILED;
         break;
   }
   RETVALUE(ret);
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
 *  @param[in]  U16 category, the Alarm category
 *  @param[in]  U16 event, the Alarm event
 *  @param[in]  U16 cause, the cause of the Alarm
 *  @param[in]  RgUstaDgn *dgn, Alarm Diagonostics
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgLMMStaInd
(
Inst inst,
U16 category,
U16 event,
U16 cause,
RgUstaDgn *dgn
)
#else
PUBLIC S16 rgLMMStaInd(inst,category, event, cause, dgn) 
Inst inst;
U16 category;
U16 event;
U16 cause;
RgUstaDgn *dgn;
#endif
{
   RgMngmt    usta;

   TRC2(rgLMMStaInd)

   if(rgCb[inst].rgInit.usta == FALSE)
   {
      RETVALUE(ROK);
   }

   cmMemset((U8 *)&usta, 0, sizeof(RgMngmt));

   SGetDateTime(&usta.t.usta.cmAlarm.dt);
   usta.t.usta.cmAlarm.category = category;
   usta.t.usta.cmAlarm.event = event;
   usta.t.usta.cmAlarm.cause = cause;
   if (dgn != NULLP)
   {
      cmMemcpy((U8 *)&usta.t.usta.dgn, (U8 *)dgn, sizeof(RgUstaDgn));
   }

   rgCb[inst].rgInit.lmPst.selector = rgCb[inst].genCfg.ustaResp.response.selector;
   rgCb[inst].rgInit.lmPst.prior = rgCb[inst].genCfg.ustaResp.response.prior;
   rgCb[inst].rgInit.lmPst.route = rgCb[inst].genCfg.ustaResp.response.route;
   rgCb[inst].rgInit.lmPst.region = rgCb[inst].genCfg.ustaResp.response.mem.region;
   rgCb[inst].rgInit.lmPst.pool = rgCb[inst].genCfg.ustaResp.response.mem.pool;
   usta.hdr.transId = rgCb[inst].genCfg.ustaResp.transId;

   RETVALUE(RgMiLrgStaInd(&rgCb[inst].rgInit.lmPst, &usta));
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
 *  @param[in]   U8 event, the trace event.
 *  @return Void 
 **/
#ifdef ANSI
PUBLIC Void rgLMMTrcInd
(
Inst   inst,
Buffer *srcMbuf,    /* Message Buffer */
U8 event            /* event */
)
#else
PUBLIC Void rgLMMTrcInd(inst,srcMbuf,event)
Inst   inst;
Buffer *srcMbuf;    /* Message Buffer */
U8 event;           /* event */
#endif
{
   Buffer   *dstMbuf = NULLP;   
   MsgLen   bufLen  = 0;
   Data     *tempBuf;
   MsgLen   tempCnt;
   RgMngmt  trc;
   Pst      pst;

   TRC2(rgLMMTrcInd)


   if ((rgCb[inst].trcLen == LRG_NO_TRACE) || (srcMbuf == NULLP))
   {
      RLOG0(L_ERROR, "Trace Disabled.");
      RETVOID;
   }
   
   cmMemset((U8 *)&trc, 0, sizeof(RgMngmt));

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
         RLOG0(L_ERROR, "SCpyMsgMsg Failed.");
         RETVOID;
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
         RLOG0(L_ERROR, "SFndLenMsg Failed.");
         RETVOID;
      }
      /* Check if the recvd buffer size is less than request trace len */
      if(bufLen < rgCb[inst].trcLen)
      {
         /* Copy the whole of the recvd buffer in trace indication */
      
         if (SCpyMsgMsg(srcMbuf, pst.region, pst.pool, &dstMbuf)
            != ROK)
         {
            RLOG0(L_ERROR, "SCpyMsgMsg Failed.");
            RETVOID;
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
            RLOG0(L_ERROR, "rgAllocSBuf Failed.");
            RETVOID;
         }
         
         /* Copy trcLen nos of bytes from the recvd message */
         if (SCpyMsgFix(srcMbuf,0,rgCb[inst].trcLen,tempBuf,&tempCnt) != ROK)   
         {
            RLOG0(L_ERROR, "SCpyMsgFix Failed.");
            RETVOID;
         }

         if (SGetMsg(pst.region, pst.pool, &dstMbuf) != ROK)
         {
            RLOG0(L_ERROR, "dstMbuf Allocation Failed");
            RETVOID;
         }
         /* Copy the tempBuf data to dst mBuf */
         if (SCpyFixMsg(tempBuf,dstMbuf,0,rgCb[inst].trcLen,&tempCnt) != ROK)
         {
            RLOG0(L_ERROR, "SCpyFixMsg Failed.");
            RETVOID;
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
   RETVOID;
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
 *  @param[in]   U8 status, Status
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgLMMBndCfm
(
Pst *pst,               /* Post Structure */
SuId suId,              /* Service user ID */
U8 status               /* Status */
)
#else
PUBLIC S16 rgLMMBndCfm(pst,suId,status)
Pst *pst;               /* Post Structure */
SuId suId;              /* Service user ID */
U8 status;              /* Status */
#endif
{
   Inst      inst = pst->dstInst - RG_INST_START;
   S16       ret = ROK;
   RgMngmt   cntrlCfm;
   Pst       cfmPst;

   TRC3(rgLMMBndCfm)

   UNUSED(pst);

   /* Check if the suId is valid */
   if(rgCb[inst].tfuSap.sapCfg.suId != suId)
   {
      RLOG0(L_ERROR, "Invalid SuId");
      RETVALUE(RFAILED);
   }

   /* check the Sap State */
   switch(rgCb[inst].tfuSap.sapSta.sapState)
   {
      case LRG_WAIT_BNDCFM:
         break;
      case LRG_BND:
         /* SAP is already bound */
         RETVALUE(ROK);
      default:
         RETVALUE(RFAILED);
   }

   cfmPst = rgCb[inst].rgInit.lmPst;
   cfmPst.selector = rgCb[inst].genCfg.bndCfmResp.response.selector;
   cfmPst.prior = rgCb[inst].genCfg.bndCfmResp.response.prior;
   cfmPst.route = rgCb[inst].genCfg.bndCfmResp.response.route;
   cfmPst.region = rgCb[inst].genCfg.bndCfmResp.response.mem.region;
   cfmPst.pool = rgCb[inst].genCfg.bndCfmResp.response.mem.pool;
   
   cmMemset((U8 *)&cntrlCfm, 0, sizeof(RgMngmt));

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

   RETVALUE(ret);
}


/**
 * @brief LTE MAC timer call back function registered with SSI. 
 *
 * @details
 *
 *     Function :  rgActvTmr
 *     
 *     This function is invoked by SSI for every timer activation
 *     period expiry.
 *     
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgActvTmr
(
 Ent     ent,
 Inst    inst
)
#else
PUBLIC S16 rgActvTmr(ent, inst)
Ent     ent;
Inst    inst;
#endif
{
   Inst macInst = (inst  - RG_INST_START);
   TRC3(rgActvTmr)

   /* Check if any MAC timer has expired */ 
   cmPrcTmr(&rgCb[macInst].tmrTqCp, rgCb[macInst].tmrTq, (PFV) rgLMMTmrExpiry);
 
   RETVALUE(ROK);
 
} /* end of rgActvTmr */

/**
 * @brief Layer Manager  Configuration request handler for Scheduler
 *
 * @details
 *
 *     Function : MacSchGenCfgReq
 *     
 *     This function receives general configurations for Scheduler
 *     from DU APP and forwards to Scheduler.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfg, the configuration parameter's structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 MacSchGenCfgReq
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfg     /* config structure  */
)
#else
PUBLIC S16 MacSchGenCfgReq(pst, cfg)
Pst      *pst;    /* post structure  */
RgMngmt  *cfg;    /* config structure  */
#endif    
{
   printf("\nReceived Scheduler gen config at MAC");
   pst->dstInst = DEFAULT_CELLS + 1;
   HandleSchGenCfgReq(pst, cfg);

   return ROK;
}

/**
 * @brief Layer Manager Configuration response from Scheduler
 *
 * @details
 *
 *     Function : SchSendCfgCfm
 *     
 *     This function sends general configurations response from
 *     Scheduler to DU APP.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the configuration confirm structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 SchSendCfgCfm 
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfm     /* config confirm structure  */
)
#else
PUBLIC S16 SchSendCfgCfm(pst, cfm)
Pst      *pst;    /* post structure  */
RgMngmt  *cfm;    /* config confirm structure  */
#endif    
{
   printf("\nSending Scheduler config confirm to DU APP");
	pst->dstEnt = ENTDUAPP;
	pst->dstInst = 0;
	pst->srcInst = 0;
	pst->selector = MAC_SCH_LC_SELECTOR; 
   RgMiLrgSchCfgCfm(pst, cfm);
   
   RETVALUE(ROK);
}


/***********************************************************
 *
 *     Func : macCellCfgFillCfmPst 
 *        
 *
 *     Desc : Fills the Confirmation Post Structure cfmPst 
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : rg_lmm.c 
 *
 **********************************************************/
Void macCellCfgFillCfmPst
(
Pst           *reqPst,
Pst           *cfmPst
)
{
   Inst inst;
   inst = reqPst->dstInst;

   cfmPst->srcEnt    = rgCb[inst].rgInit.ent;
   cfmPst->srcInst   = rgCb[inst].rgInit.inst;
   cfmPst->srcProcId = rgCb[inst].rgInit.procId;

   cfmPst->dstEnt    = ENTDUAPP;
   cfmPst->dstInst   = 0;
   cfmPst->dstProcId = cfmPst->srcProcId;

   cfmPst->selector  = LRG_SEL_LC;
   cfmPst->prior     = reqPst->prior;
   cfmPst->route     = reqPst->route;
   cfmPst->region    = reqPst->region;
   cfmPst->pool      = reqPst->pool;
   cfmPst->event     = EVENT_MAC_CELL_CONFIG_CFM;

   RETVOID;
}

/**
 * @brief Layer Manager Configuration request handler. 
 *
 * @details
 *
 *     Function : MacHdlCellCfgReq 
 *     
 *     This function handles the gNB and cell configuration
 *     request received from DU APP.
 *     This API unapcks and forwards the config towards SCH
 *     
 *  @param[in]  Pst           *pst
 *  @param[in]  MacCellCfg    *macCellCfg 
 *  @return  S16
 *      -# ROK
 **/
int MacHdlCellCfgReq
(
 Pst           *pst,
 MacCellCfg    *macCellCfg
)
{
   Pst cfmPst;
   int ret = ROK;
   RgCellCb      *cellCb;
	MacCellCb     *macCellCb;
   Inst inst = pst->dstInst;

   cmMemset((U8 *)&cfmPst, 0, sizeof(Pst));
   MAC_ALLOC(cellCb,sizeof(RgCellCb));

   if(cellCb == NULLP)
   {
      DU_LOG("\nMAC : cellCb is NULL at handling of macCellCfg\n");
      return RFAILED;
   }

   memcpy(&cellCb->macCellCfg,macCellCfg,sizeof(MacCellCfg));
   rgCb[inst].cell = cellCb;

   MAC_ALLOC(macCellCb,sizeof(MacCellCb));
	if(macCellCb == NULLP)
	{
      DU_LOG("\nMAC : macCellCb is NULL at handling of macCellCfg\n");
      return RFAILED;
   }
   macCb.macCell = macCellCb;
   macCb.macCell->cellId = macCellCfg->cellId;
   /* Send cell cfg to scheduler */
   ret = MacSchCellCfgReq(pst, macCellCfg);
	if(ret != ROK)
	{
		MacCellCfgCfm macCellCfgCfm;
		macCellCfgCfm.rsp = RSP_NOK;
		macCellCfgCfm.transId = macCellCfg->transId;
		macCellCfgFillCfmPst(pst,&cfmPst);
		ret = (*packMacCellCfmOpts[cfmPst.selector])(&cfmPst,&macCellCfgCfm);
	}
#ifdef INTEL_WLS
   LwrMacEnqueueWlsBlock();
#endif
   return ret;
} /* end of MacHdlCellCfgReq */

/**
 * @brief Layer Manager Configuration request handler. 
 *
 * @details
 *
 *     Function : MacSchCellCfgReq 
 *     
 *     This function sends cell configuration to SCH
 *     
 *  @param[in]  Pst           *pst
 *  @param[in]  MacCellCfg    *macCellCfg 
 *  @return  S16
 *      -# ROK
 **/
int MacSchCellCfgReq
(
 Pst           *pst,
 MacCellCfg    *macCellCfg
)
{
   SchCellCfg schCellCfg;
	Pst        cfgPst;
	int ret;

   cmMemset((U8 *)&cfgPst, 0, sizeof(Pst));
	schCellCfg.cellId = macCellCfg->cellId;
	schCellCfg.phyCellId = macCellCfg->phyCellId;
	schCellCfg.bandwidth = macCellCfg->dlCarrCfg.bw;
	schCellCfg.dupMode = macCellCfg->dupType;

	/* fill ssb scheduler parameters */
	schCellCfg.ssbSchCfg.ssbPbchPwr = macCellCfg->ssbCfg.ssbPbchPwr;
	schCellCfg.ssbSchCfg.scsCommon = macCellCfg->ssbCfg.scsCmn;
	schCellCfg.ssbSchCfg.ssbOffsetPointA = macCellCfg->ssbCfg.ssbOffsetPointA;
	schCellCfg.ssbSchCfg.ssbPeriod = macCellCfg->ssbCfg.ssbPeriod;
	schCellCfg.ssbSchCfg.ssbSubcOffset = macCellCfg->ssbCfg.ssbScOffset;
	for(uint8_t idx=0; idx<SSB_MASK_SIZE; idx++)
	{
      schCellCfg.ssbSchCfg.nSSBMask[idx] = macCellCfg->ssbCfg.ssbMask[idx]; 
	}

	/* fill SIB1 scheduler parameters */
	schCellCfg.sib1SchCfg.sib1PduLen = macCellCfg->sib1Cfg.sib1PduLen; 
	schCellCfg.sib1SchCfg.sib1NewTxPeriod = macCellCfg->sib1Cfg.sib1NewTxPeriod; 
	schCellCfg.sib1SchCfg.sib1RepetitionPeriod = macCellCfg->sib1Cfg.sib1RepetitionPeriod; 
	schCellCfg.sib1SchCfg.coresetZeroIndex = macCellCfg->sib1Cfg.coresetZeroIndex; 
	schCellCfg.sib1SchCfg.searchSpaceZeroIndex = macCellCfg->sib1Cfg.searchSpaceZeroIndex; 
	schCellCfg.sib1SchCfg.sib1Mcs = macCellCfg->sib1Cfg.sib1Mcs; 

   cfgPst.srcProcId = pst->dstProcId;
	cfgPst.dstProcId = pst->srcProcId;
	cfgPst.srcEnt    = ENTRG;
	cfgPst.srcInst   = 0;
	cfgPst.dstEnt    = ENTRG;
	cfgPst.dstInst   = 1;
	cfgPst.selector  = MAC_SCH_TC_SELECTOR;
	cfgPst.event     = EVENT_SCH_CELL_CFG;

	ret = (*SchCellCfgOpts[cfgPst.selector])(&cfgPst, &schCellCfg);
	return ret;
} /* end of MacSchCellCfgReq */


/*******************************************************************
 *
 * @brief Sends Cell config confirm to DU APP
 *
 * @details
 *
 *    Function : MacSendCellCfgCfm 
 *
 *    Functionality:
 *      Sends Cell config confirm to DU APP
 *
 * @params[in] Response status
 * @return void
 *
 * ****************************************************************/
void MacSendCellCfgCfm(uint8_t response)
{
   Pst pst;
   RgCellCb      *cellCb;
   MacCellCfgCfm macCellCfgCfm;
 
   cmMemset((U8 *)&pst, 0, sizeof(Pst));
   cellCb = rgCb[macCb.macInst].cell;
 
   macCellCfgCfm.transId = cellCb->macCellCfg.transId;
   macCellCfgCfm.rsp = response;

   memcpy((void *)&pst, (void *)&rgCb[macCb.macInst].rgInit.lmPst, sizeof(Pst));
   pst.event = EVENT_MAC_CELL_CONFIG_CFM;
   (*packMacCellCfmOpts[pst.selector])(&pst,&macCellCfgCfm);
}


/**
 * @brief Layer Manager Configuration response handler. 
 *
 * @details
 *
 *     Function : MacProcSchCellCfgCfm
 *     
 *     This function processes cell configuration to SCH
 *     
 *  @param[in]  Pst           *pst
 *  @param[in]  SchCellCfgCfm *schCellCfgCfm
 *  @return  int
 *      -# ROK
 **/
int MacProcSchCellCfgCfm 
(
 Pst           *pst,
 SchCellCfgCfm *schCellCfgCfm
)
{
	if(schCellCfgCfm->rsp == RSP_OK)
	{
      sendToLowerMac(PARAM_REQUEST, 0, (void *)NULL);
	}
	else
	{
      MacSendCellCfgCfm(RSP_NOK);
	}
   return ROK;	
}

/**********************************************************************
 
         End of file
**********************************************************************/
