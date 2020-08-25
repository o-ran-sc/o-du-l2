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
/* This file stores handler for MAC and SCH configuration requests */
/* header include files (.h) */
#include "common_def.h"
#include "lrg.h"
#include "lrg.x"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "lwr_mac_upr_inf.h"
#include "mac.h"
#include "lwr_mac.h"
#include "lwr_mac_fsm.h"
#include "mac_utils.h"

uint16_t gCrntiCount;
uint8_t MacSchCellCfgReq(Pst *pst, MacCellCfg  *macCellCfg);

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
 *  @return 
 *      -# ROK
 **/
PUBLIC int MacSchGenCfgReq(Pst *pst, RgMngmt *cfg)
{
   printf("\nReceived Scheduler gen config at MAC");
   pst->dstInst = DEFAULT_CELLS + 1;
   SchProcGenCfgReq(pst, cfg);

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
 *  @return 
 *      -# ROK
 **/
PUBLIC uint8_t SchSendCfgCfm(Pst *pst, RgMngmt  *cfm)
{
   printf("\nSending Scheduler config confirm to DU APP");
   pst->dstEnt = ENTDUAPP;
   pst->dstInst = 0;
   pst->srcInst = 0;
   pst->selector = ODU_SELECTOR_LC;
   RgMiLrgSchCfgCfm(pst, cfm);

   return ROK;
}

/**
 * @brief Layer Manager Configuration request handler.
 *
 * @details
 *
 *     Function : MacProcCellCfgReq
 *
 *     This function handles the gNB and cell configuration
 *     request received from DU APP.
 *     This API unapcks and forwards the config towards SCH
 *
 *  @param[in]  Pst           *pst
 *  @param[in]  MacCellCfg    *macCellCfg
 *  @return  
 *      -# ROK
 **/
uint8_t MacProcCellCfgReq(Pst *pst, MacCellCfg *macCellCfg)
{
   Pst cfmPst;
   uint16_t cellIdx;
   uint16_t ret = ROK;
   MacCellCb     *macCellCb;

   cmMemset((uint8_t *)&cfmPst, 0, sizeof(Pst));

   MAC_ALLOC(macCellCb, sizeof(MacCellCb));
   if(macCellCb == NULLP)
   {
      DU_LOG("\nMAC : macCellCb is NULL at handling of macCellCfg\n");
      return RFAILED;
   }
   memset(macCellCb, 0, sizeof(MacCellCb));

   GET_CELL_IDX(macCellCfg->cellId, cellIdx);
   macCb.macCell[cellIdx] = macCellCb;
   macCb.macCell[cellIdx]->cellId = macCellCfg->cellId;
   memcpy(&macCb.macCell[cellIdx]->macCellCfg, macCellCfg, sizeof(MacCellCfg));

   MAC_ALLOC(macCb.macCell[cellIdx]->macCellCfg.sib1Cfg.sib1Pdu, \
	 macCb.macCell[cellIdx]->macCellCfg.sib1Cfg.sib1PduLen);
   if(macCb.macCell[cellIdx]->macCellCfg.sib1Cfg.sib1Pdu == NULLP)
   {
      DU_LOG("\nMAC : macCellCb is NULL at handling of sib1Pdu of macCellCfg\n");
      return RFAILED;
   }
   memcpy(macCb.macCell[cellIdx]->macCellCfg.sib1Cfg.sib1Pdu, macCellCfg->sib1Cfg.sib1Pdu, \
	 macCb.macCell[cellIdx]->macCellCfg.sib1Cfg.sib1PduLen);
   
   /* Initializing global variable */
   gCrntiCount = ODU_START_CRNTI;

   /* Send cell cfg to scheduler */
   ret = MacSchCellCfgReq(pst, macCellCfg);
   if(ret != ROK)
   {
      MacCellCfgCfm macCellCfgCfm;
      macCellCfgCfm.rsp = RSP_NOK;
      macCellCfgCfm.cellId = macCellCfg->cellId;
      /* Fill Pst */
      FILL_PST_MAC_TO_DUAPP(cfmPst, EVENT_MAC_CELL_CONFIG_CFM);
      cfmPst.selector  = ODU_SELECTOR_LC;

      ret = (*packMacCellCfmOpts[cfmPst.selector])(&cfmPst,&macCellCfgCfm);
   }
   else
   {
      if(macCellCfg->prachCfg.fdm[0].numUnusedRootSeq != 0)
      {
	 MAC_FREE_SHRABL_BUF(pst->region, pst->pool, macCellCfg->prachCfg.fdm[0].unsuedRootSeq,
	       macCellCfg->prachCfg.fdm[0].numUnusedRootSeq* sizeof(uint8_t));
      }
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, macCellCfg->sib1Cfg.sib1Pdu, macCellCfg->sib1Cfg.sib1PduLen);
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, macCellCfg ,sizeof(MacCellCfg));
   }
#ifdef INTEL_WLS
   LwrMacEnqueueWlsBlock();
#endif
   return ret;
} /* end of MacProcCellCfgReq */

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
 *  @return  
 *      -# ROK
 **/
uint8_t MacSchCellCfgReq(Pst *pst, MacCellCfg *macCellCfg)
{
   SchCellCfg schCellCfg;
   Pst        cfgPst;
   int ret;

   cmMemset((uint8_t *)&cfgPst, 0, sizeof(Pst));
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

   /* fill RACH config params */
   schCellCfg.schRachCfg.prachCfgIdx = macCellCfg->prachCfg.prachCfgIdx;
   schCellCfg.schRachCfg.prachSubcSpacing = \
					    macCellCfg->prachCfg.prachSubcSpacing;
   schCellCfg.schRachCfg.msg1FreqStart = macCellCfg->prachCfg.msg1FreqStart;
   schCellCfg.schRachCfg.msg1Fdm       = macCellCfg->prachCfg.msg1Fdm;
   schCellCfg.schRachCfg.rootSeqLen    = macCellCfg->prachCfg.rootSeqLen;
   schCellCfg.schRachCfg.rootSeqIdx    = macCellCfg->prachCfg.fdm[0].rootSeqIdx;
   schCellCfg.schRachCfg.numRootSeq    = macCellCfg->prachCfg.fdm[0].numRootSeq;
   schCellCfg.schRachCfg.k1            = macCellCfg->prachCfg.fdm[0].k1;
   schCellCfg.schRachCfg.ssbPerRach    = macCellCfg->prachCfg.ssbPerRach;
   schCellCfg.schRachCfg.prachMultCarrBand = \
					     macCellCfg->prachCfg.prachMultCarrBand;
   schCellCfg.schRachCfg.raContResTmr  = macCellCfg->prachCfg.raContResTmr;
   schCellCfg.schRachCfg.rsrpThreshSsb = macCellCfg->prachCfg.rsrpThreshSsb;
   schCellCfg.schRachCfg.raRspWindow   = macCellCfg->prachCfg.raRspWindow;

   /* fill initial DL BWP */
   schCellCfg.schInitialDlBwp.bwp.freqAlloc.startPrb = macCellCfg->initialDlBwp.bwp.firstPrb;
   schCellCfg.schInitialDlBwp.bwp.freqAlloc.numPrb = macCellCfg->initialDlBwp.bwp.numPrb;
   schCellCfg.schInitialDlBwp.bwp.scs = macCellCfg->initialDlBwp.bwp.scs;
   schCellCfg.schInitialDlBwp.bwp.cyclicPrefix = macCellCfg->initialDlBwp.bwp.cyclicPrefix;
   schCellCfg.schInitialDlBwp.pdcchCommon.commonSearchSpace.searchSpaceId =
      macCellCfg->initialDlBwp.pdcchCommon.commonSearchSpace.searchSpaceId;
   schCellCfg.schInitialDlBwp.pdcchCommon.commonSearchSpace.coresetId =
      macCellCfg->initialDlBwp.pdcchCommon.commonSearchSpace.coresetId;
   schCellCfg.schInitialDlBwp.pdcchCommon.commonSearchSpace.monitoringSlot =
      macCellCfg->initialDlBwp.pdcchCommon.commonSearchSpace.monitoringSlot;
   schCellCfg.schInitialDlBwp.pdcchCommon.commonSearchSpace.duration =
      macCellCfg->initialDlBwp.pdcchCommon.commonSearchSpace.duration;
   schCellCfg.schInitialDlBwp.pdcchCommon.commonSearchSpace.monitoringSymbol =
      macCellCfg->initialDlBwp.pdcchCommon.commonSearchSpace.monitoringSymbol;
   schCellCfg.schInitialDlBwp.pdcchCommon.commonSearchSpace.candidate.aggLevel1 =
      macCellCfg->initialDlBwp.pdcchCommon.commonSearchSpace.candidate.aggLevel1;
   schCellCfg.schInitialDlBwp.pdcchCommon.commonSearchSpace.candidate.aggLevel2 =
      macCellCfg->initialDlBwp.pdcchCommon.commonSearchSpace.candidate.aggLevel2;
   schCellCfg.schInitialDlBwp.pdcchCommon.commonSearchSpace.candidate.aggLevel4 =
      macCellCfg->initialDlBwp.pdcchCommon.commonSearchSpace.candidate.aggLevel4;
   schCellCfg.schInitialDlBwp.pdcchCommon.commonSearchSpace.candidate.aggLevel8 =
      macCellCfg->initialDlBwp.pdcchCommon.commonSearchSpace.candidate.aggLevel8;
   schCellCfg.schInitialDlBwp.pdcchCommon.commonSearchSpace.candidate.aggLevel16 =
      macCellCfg->initialDlBwp.pdcchCommon.commonSearchSpace.candidate.aggLevel16;
   schCellCfg.schInitialDlBwp.pdschCommon.k0 = macCellCfg->initialDlBwp.pdschCommon.k0;
   schCellCfg.schInitialDlBwp.pdschCommon.mappingType =
      macCellCfg->initialDlBwp.pdschCommon.mappingType;
   schCellCfg.schInitialDlBwp.pdschCommon.startSymbol =
      macCellCfg->initialDlBwp.pdschCommon.startSymbol;
   schCellCfg.schInitialDlBwp.pdschCommon.lengthSymbol =
      macCellCfg->initialDlBwp.pdschCommon.lengthSymbol;

   /* fill initial UL BWP */
   schCellCfg.schInitialUlBwp.bwp.freqAlloc.startPrb = macCellCfg->initialUlBwp.bwp.firstPrb;
   schCellCfg.schInitialUlBwp.bwp.freqAlloc.numPrb = macCellCfg->initialUlBwp.bwp.numPrb;
   schCellCfg.schInitialUlBwp.bwp.scs = macCellCfg->initialUlBwp.bwp.scs;
   schCellCfg.schInitialUlBwp.bwp.cyclicPrefix = macCellCfg->initialUlBwp.bwp.cyclicPrefix;
   schCellCfg.schInitialUlBwp.puschCommon.k2 = macCellCfg->initialUlBwp.puschCommon.k2;
   schCellCfg.schInitialUlBwp.puschCommon.mappingType =
      macCellCfg->initialUlBwp.puschCommon.mappingType;
   schCellCfg.schInitialUlBwp.puschCommon.startSymbol =
      macCellCfg->initialUlBwp.puschCommon.startSymbol;
   schCellCfg.schInitialUlBwp.puschCommon.lengthSymbol =
      macCellCfg->initialUlBwp.puschCommon.lengthSymbol;

   FILL_PST_MAC_TO_SCH(cfgPst, EVENT_SCH_CELL_CFG);

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
void MacSendCellCfgCfm(uint16_t cellId, uint8_t response)
{
   Pst pst;
   uint16_t   cellIdx;
   MacCellCfgCfm macCellCfgCfm;

   cmMemset((uint8_t *)&pst, 0, sizeof(Pst));

   GET_CELL_IDX(cellId, cellIdx);
   macCellCfgCfm.cellId = macCb.macCell[cellIdx]->macCellCfg.cellId;
   macCellCfgCfm.rsp = response;

   /* Fill Pst */
   FILL_PST_MAC_TO_DUAPP(pst, EVENT_MAC_CELL_CONFIG_CFM);
   pst.selector  = ODU_SELECTOR_LC;

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
uint8_t MacProcSchCellCfgCfm(Pst *pst, SchCellCfgCfm *schCellCfgCfm)
{
   uint16_t *cellId = NULLP;

   if(schCellCfgCfm->rsp == RSP_OK)
   {
      cellId = &schCellCfgCfm->cellId;
      sendToLowerMac(CONFIG_REQUEST, 0, (void *)cellId);
   }
   else
   {
      MacSendCellCfgCfm(schCellCfgCfm->cellId, RSP_NOK);
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief MAC handler for config response from PHY
 *
 * @details
 *
 *    Function : fapiMacConfigRsp
 *
 *    Functionality:
 *     Processes config response from PHY and sends cell config
 *     confirm to DU APP
 *
 * @params[in]
 * @return void
 *
 * ****************************************************************/
void fapiMacConfigRsp(uint16_t cellId)
{
   /* TODO : Processing of config response from PHY */

   /* Send cell config cfm to DU APP */
   MacSendCellCfgCfm(cellId, RSP_OK);
}

/**********************************************************************
  End of file
 **********************************************************************/
