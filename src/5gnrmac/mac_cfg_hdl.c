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
#include "rlc_mac_inf.h"
#include "mac_upr_inf_api.h"
#include "lwr_mac.h"
#include "lwr_mac_fsm.h"
#include "mac_utils.h"
#include "lwr_mac_phy.h"

uint8_t ssbPeriodicity[6] = {5, 10, 20, 40, 80, 160};

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

MacSchCellDeleteReqFunc macSchCellDeleteReqOpts[]=
{
   packMacSchCellDeleteReq,    /* packing for loosely coupled */
   MacSchCellDeleteReq,        /* packing for tightly coupled */
   packMacSchCellDeleteReq     /* packing for light weight loosely coupled */
};

MacDuCellDeleteRspFunc macDuCellDeleteRspOpts[] =
{
   packDuMacCellDeleteRsp,   /* packing for loosely coupled */
   DuProcMacCellDeleteRsp,   /* packing for tightly coupled */
   packDuMacCellDeleteRsp   /* packing for light weight loosly coupled */
};

MacDuSliceCfgRspFunc macDuSliceCfgRspOpts[] =
{
   packDuMacSliceCfgRsp,   /* packing for loosely coupled */
   DuProcMacSliceCfgRsp,   /* packing for tightly coupled */
   packDuMacSliceCfgRsp   /* packing for light weight loosly coupled */
};

MacDuSliceReCfgRspFunc macDuSliceReCfgRspOpts[] =
{
   packDuMacSliceReCfgRsp,   /* packing for loosely coupled */
   DuProcMacSliceReCfgRsp,   /* packing for tightly coupled */
   packDuMacSliceReCfgRsp   /* packing for light weight loosly coupled */
};

MacSchPagingIndFunc macSchPagingIndOpts[] = 
{
   packMacSchPagingInd,   /* packing for loosely coupled */
   MacSchPagingInd,       /* packing for tightly coupled */
   packMacSchPagingInd    /* packing for light weight loosely coupled */
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
uint8_t MacSchGenCfgReq(Pst *pst, RgMngmt *cfg)
{
   Pst schPst;

   printf("\nReceived Scheduler gen config at MAC");
   memset(&schPst, 0, sizeof(Pst));
   FILL_PST_MAC_TO_SCH(schPst, EVENT_SCH_GEN_CFG);
   SchProcGenCfgReq(&schPst, cfg);

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
uint8_t SchSendCfgCfm(Pst *pst, RgMngmt  *cfm)
{
   DU_LOG("\nDEBUG  -->  Sending Scheduler config confirm to DU APP");
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
   uint8_t ret = ROK, sliceIdx = 0;
   MacCellCb     *macCellCb;

   memset((uint8_t *)&cfmPst, 0, sizeof(Pst));

   MAC_ALLOC(macCellCb, sizeof(MacCellCb));
   if(macCellCb == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : macCellCb is NULL at handling of macCellCfg\n");
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
      DU_LOG("\nERROR  -->  MAC : macCellCb is NULL at handling of sib1Pdu of macCellCfg\n");
      return RFAILED;
   }
   memcpy(macCb.macCell[cellIdx]->macCellCfg.sib1Cfg.sib1Pdu, macCellCfg->sib1Cfg.sib1Pdu, \
	 macCb.macCell[cellIdx]->macCellCfg.sib1Cfg.sib1PduLen);
   
   macCb.macCell[cellIdx]->macCellCfg.plmnInfoList.numSupportedSlice = macCellCfg->plmnInfoList.numSupportedSlice;
   MAC_ALLOC(macCb.macCell[cellIdx]->macCellCfg.plmnInfoList.snssai, macCb.macCell[cellIdx]->macCellCfg.plmnInfoList.numSupportedSlice\
         * sizeof(Snssai*));
   if(macCb.macCell[cellIdx]->macCellCfg.plmnInfoList.snssai == NULLP)
   {
      DU_LOG("\nERROR  --> MAC: Memory allocation failed at MacProcCellCfgReq");
      return RFAILED;
   }

   if(macCb.macCell[cellIdx]->macCellCfg.plmnInfoList.snssai)
   {
      for(sliceIdx=0; sliceIdx<macCb.macCell[cellIdx]->macCellCfg.plmnInfoList.numSupportedSlice; sliceIdx++)
      {
         if(macCellCfg->plmnInfoList.snssai[sliceIdx])
         {
            MAC_ALLOC(macCb.macCell[cellIdx]->macCellCfg.plmnInfoList.snssai[sliceIdx], sizeof(Snssai));
            if(!macCb.macCell[cellIdx]->macCellCfg.plmnInfoList.snssai[sliceIdx])
            {
               DU_LOG("\nERROR  --> MAC: Memory allocation failed at MacProcCellCfgReq");
               return RFAILED;
            }
            memcpy(macCb.macCell[cellIdx]->macCellCfg.plmnInfoList.snssai[sliceIdx], macCellCfg->plmnInfoList.snssai[sliceIdx],\
            sizeof(Snssai));
         }
      }
   }

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
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, macCellCfg ,sizeof(MacCellCfg));
   }
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
   uint8_t    ssbMaskIdx = 0, rsrcListIdx = 0, ret=0, sliceIdx=0;

   memset(&cfgPst, 0, sizeof(Pst));
   memset(&schCellCfg, 0, sizeof(SchCellCfg));
   schCellCfg.cellId = macCellCfg->cellId;
   schCellCfg.phyCellId = macCellCfg->phyCellId;
   schCellCfg.numerology = macCellCfg->numerology;
   schCellCfg.dupMode = macCellCfg->dupType;
   schCellCfg.bandwidth = macCellCfg->dlCarrCfg.bw;
   schCellCfg.dlFreq = macCellCfg->dlCarrCfg.freq;
   schCellCfg.ulFreq = macCellCfg->ulCarrCfg.freq;

   /* fill ssb scheduler parameters */
   schCellCfg.ssbSchCfg.ssbPbchPwr = macCellCfg->ssbCfg.ssbPbchPwr;
   schCellCfg.ssbSchCfg.scsCommon = macCellCfg->ssbCfg.scsCmn;
   schCellCfg.ssbSchCfg.ssbOffsetPointA = macCellCfg->ssbCfg.ssbOffsetPointA;
   schCellCfg.ssbSchCfg.ssbPeriod = ssbPeriodicity[macCellCfg->ssbCfg.ssbPeriod];
   schCellCfg.ssbSchCfg.ssbSubcOffset = macCellCfg->ssbCfg.ssbScOffset;
   for(ssbMaskIdx=0; ssbMaskIdx<SSB_MASK_SIZE; ssbMaskIdx++)
   {
      schCellCfg.ssbSchCfg.nSSBMask[ssbMaskIdx] = macCellCfg->ssbCfg.ssbMask[ssbMaskIdx];
   }
   schCellCfg.ssbSchCfg.totNumSsb = countSetBits(schCellCfg.ssbSchCfg.nSSBMask[0]);

   /* fill SIB1 scheduler parameters */
   schCellCfg.sib1SchCfg.sib1PduLen = macCellCfg->sib1Cfg.sib1PduLen;
   schCellCfg.sib1SchCfg.sib1RepetitionPeriod = macCellCfg->sib1Cfg.sib1RepetitionPeriod;
   schCellCfg.sib1SchCfg.coresetZeroIndex = macCellCfg->sib1Cfg.coresetZeroIndex;
   schCellCfg.sib1SchCfg.searchSpaceZeroIndex = macCellCfg->sib1Cfg.searchSpaceZeroIndex;
   schCellCfg.sib1SchCfg.sib1Mcs = macCellCfg->sib1Cfg.sib1Mcs;
   schCellCfg.sib1SchCfg.pageCfg.numPO  =  macCellCfg->sib1Cfg.pagingCfg.numPO;
   schCellCfg.sib1SchCfg.pageCfg.poPresent = macCellCfg->sib1Cfg.pagingCfg.poPresent;

   if(schCellCfg.sib1SchCfg.pageCfg.poPresent)
   {
      memcpy(schCellCfg.sib1SchCfg.pageCfg.pagingOcc, macCellCfg->sib1Cfg.pagingCfg.pagingOcc, MAX_PO_PER_PF);
   }

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
   schCellCfg.schRachCfg.totalNumRaPreamble = macCellCfg->prachCfg.totalNumRaPreamble;
   schCellCfg.schRachCfg.ssbPerRach    = macCellCfg->prachCfg.ssbPerRach;
   schCellCfg.schRachCfg.numCbPreamblePerSsb = macCellCfg->prachCfg.numCbPreamblePerSsb;
   schCellCfg.schRachCfg.prachMultCarrBand = macCellCfg->prachCfg.prachMultCarrBand;
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
   schCellCfg.schInitialDlBwp.pdschCommon.numTimeDomAlloc = macCellCfg->initialDlBwp.pdschCommon.numTimeDomAlloc;
   for(rsrcListIdx = 0; rsrcListIdx<macCellCfg->initialDlBwp.pdschCommon.numTimeDomAlloc; rsrcListIdx++)
   {
      schCellCfg.schInitialDlBwp.pdschCommon.timeDomRsrcAllocList[rsrcListIdx].k0 = 
         macCellCfg->initialDlBwp.pdschCommon.timeDomRsrcAllocList[rsrcListIdx].k0;
      schCellCfg.schInitialDlBwp.pdschCommon.timeDomRsrcAllocList[rsrcListIdx].mappingType =
         macCellCfg->initialDlBwp.pdschCommon.timeDomRsrcAllocList[rsrcListIdx].mappingType;
      schCellCfg.schInitialDlBwp.pdschCommon.timeDomRsrcAllocList[rsrcListIdx].startSymbol =
         macCellCfg->initialDlBwp.pdschCommon.timeDomRsrcAllocList[rsrcListIdx].startSymbol;
      schCellCfg.schInitialDlBwp.pdschCommon.timeDomRsrcAllocList[rsrcListIdx].lengthSymbol =
         macCellCfg->initialDlBwp.pdschCommon.timeDomRsrcAllocList[rsrcListIdx].lengthSymbol;
   }

   /* fill initial UL BWP */
   schCellCfg.schInitialUlBwp.bwp.freqAlloc.startPrb = macCellCfg->initialUlBwp.bwp.firstPrb;
   schCellCfg.schInitialUlBwp.bwp.freqAlloc.numPrb = macCellCfg->initialUlBwp.bwp.numPrb;
   schCellCfg.schInitialUlBwp.bwp.scs = macCellCfg->initialUlBwp.bwp.scs;
   schCellCfg.schInitialUlBwp.bwp.cyclicPrefix = macCellCfg->initialUlBwp.bwp.cyclicPrefix;
   schCellCfg.schInitialUlBwp.puschCommon.numTimeDomRsrcAlloc = \
      macCellCfg->initialUlBwp.puschCommon.numTimeDomRsrcAlloc;
   for(rsrcListIdx = 0; rsrcListIdx < macCellCfg->initialUlBwp.puschCommon.numTimeDomRsrcAlloc; rsrcListIdx++)
   {
      schCellCfg.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[rsrcListIdx].k2 = 
         macCellCfg->initialUlBwp.puschCommon.timeDomRsrcAllocList[rsrcListIdx].k2;
      schCellCfg.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[rsrcListIdx].mappingType =
         macCellCfg->initialUlBwp.puschCommon.timeDomRsrcAllocList[rsrcListIdx].mappingType;
      schCellCfg.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[rsrcListIdx].startSymbol =
         macCellCfg->initialUlBwp.puschCommon.timeDomRsrcAllocList[rsrcListIdx].startSymbol;
      schCellCfg.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[rsrcListIdx].symbolLength =
         macCellCfg->initialUlBwp.puschCommon.timeDomRsrcAllocList[rsrcListIdx].symbolLength;
   }

   if(macCellCfg->plmnInfoList.snssai) 
   {
      schCellCfg.plmnInfoList.numSliceSupport = macCellCfg->plmnInfoList.numSupportedSlice;
      MAC_ALLOC(schCellCfg.plmnInfoList.snssai, schCellCfg.plmnInfoList.numSliceSupport * sizeof(Snssai*));
      if(!schCellCfg.plmnInfoList.snssai)
      {
         DU_LOG("\nERROR  --> MAC: Memory allocation failed at MacSchCellCfgReq");
         return RFAILED;
      }
      for(sliceIdx=0; sliceIdx<schCellCfg.plmnInfoList.numSliceSupport; sliceIdx++)
      {
         if(macCellCfg->plmnInfoList.snssai[sliceIdx])
         {
            MAC_ALLOC(schCellCfg.plmnInfoList.snssai[sliceIdx], sizeof(Snssai));
            if(!schCellCfg.plmnInfoList.snssai[sliceIdx])
            {
               DU_LOG("\nERROR  --> MAC: Memory allocation failed at MacSchCellCfgReq");
               return RFAILED;
            }
            memcpy(schCellCfg.plmnInfoList.snssai[sliceIdx], macCellCfg->plmnInfoList.snssai[sliceIdx],  sizeof(Snssai));
         }
      }
   }

#ifdef NR_TDD
   memcpy(&schCellCfg.tddCfg, &macCellCfg->tddCfg, sizeof(TDDCfg));
#endif

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

   memset(&pst, 0, sizeof(Pst));

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

#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTSCH -> ENTMAC : EVENT_SCH_CELL_CFG_CFM\n");
#endif

   if(schCellCfgCfm->rsp == RSP_OK)
   {
      cellId = &schCellCfgCfm->cellId;
#ifdef INTEL_TIMER_MODE
      sendToLowerMac(UL_IQ_SAMPLE, 0, (void *)cellId);
#else
      sendToLowerMac(CONFIG_REQUEST, 0, (void *)cellId);
#endif
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

/*******************************************************************
 *
 * @brief Fill and Send Cell Delete response from MAC to DU APP
 *
 * @details
 *
 *    Function : MacSendCellDeleteRsp
 *
 *    Functionality: Fill and Send Cell Delete response from MAC to DUAPP
 *
 * @params[in] MAC Cell delete result
 *             SCH Cell delete response
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacSendCellDeleteRsp(CellDeleteStatus result, uint8_t cellId)
{
   MacCellDeleteRsp *deleteRsp=NULLP;
   Pst            rspPst;

   MAC_ALLOC_SHRABL_BUF(deleteRsp, sizeof(MacCellDeleteRsp));
   if(!deleteRsp)
   {
      DU_LOG("\nERROR  -->  MAC : MacSendCellDeleteRsp(): Memory allocation for Cell delete response failed");
      return RFAILED;
   }

   /* Filling CELL delete response */
   
   memset(deleteRsp, 0, sizeof(MacCellDeleteRsp));
   deleteRsp->cellId = cellId;
   deleteRsp->result = result;

   /* Fill Post structure and send CELL delete response*/
   memset(&rspPst, 0, sizeof(Pst));
   FILL_PST_MAC_TO_DUAPP(rspPst, EVENT_MAC_CELL_DELETE_RSP);
   return (*macDuCellDeleteRspOpts[rspPst.selector])(&rspPst, deleteRsp);
}

/*******************************************************************
 *
 * @brief  Processes CELL delete response from scheduler
 *
 * @details
 *
 *    Function : MacProcSchCellDeleteRsp
 *
 *    Functionality:
 *      Processes CELL delete from scheduler
 *
 * @params[in] Pst : Post structure
 *             schCellDelRsp : Scheduler CELL delete respons
 * @return ROK     - success
 *         RFAILED - failure
 *
 * * ****************************************************************/
uint8_t MacProcSchCellDeleteRsp(Pst *pst, SchCellDeleteRsp *schCellDelRsp)
{
   uint8_t  ret = ROK, sliceIdx = 0;
   uint16_t cellIdx=0;
   CellDeleteStatus status;
   
#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTSCH -> ENTMAC : EVENT_CELL_DELETE_RSP_TO_MAC\n");
#endif  

   if(schCellDelRsp)
   {
      if(schCellDelRsp->rsp == RSP_OK)
      {
         DU_LOG("\nINFO   -->  MAC : SCH CELL Delete response for cellId[%d] is successful ", \
         schCellDelRsp->cellId);
         GET_CELL_IDX(schCellDelRsp->cellId, cellIdx);
         if(macCb.macCell[cellIdx])
         {
            if(macCb.macCell[cellIdx]->cellId == schCellDelRsp->cellId)
            {
               status  = SUCCESSFUL_RSP;
               if(macCb.macCell[cellIdx]->macCellCfg.plmnInfoList.snssai)
               {
                  for(sliceIdx = 0; sliceIdx<macCb.macCell[cellIdx]->macCellCfg.plmnInfoList.numSupportedSlice; sliceIdx++)
                  {
                     MAC_FREE(macCb.macCell[cellIdx]->macCellCfg.plmnInfoList.snssai[sliceIdx], sizeof(Snssai));
                  }
                  MAC_FREE(macCb.macCell[cellIdx]->macCellCfg.plmnInfoList.snssai, macCb.macCell[cellIdx]->macCellCfg.plmnInfoList.\
                  numSupportedSlice * sizeof(Snssai*));
               }
               MAC_FREE(macCb.macCell[cellIdx]->macCellCfg.sib1Cfg.sib1Pdu, \
                  macCb.macCell[cellIdx]->macCellCfg.sib1Cfg.sib1PduLen);
               MAC_FREE(macCb.macCell[cellIdx], sizeof(MacCellCb));
            }
            else
            {
                DU_LOG("ERROR  -->  MAC : MacProcSchCellDeleteRsp(): CellId[%d] does not exists", schCellDelRsp->cellId);
                status = CELL_ID_INVALID;
                ret = RFAILED;
            }
         }
         else
         {
            DU_LOG("ERROR  -->  MAC : MacProcSchCellDeleteRsp(): CellId[%d] does not exists", schCellDelRsp->cellId);
            status = CELL_ID_INVALID;
            ret = RFAILED;
         }
      }
      else
      {
         DU_LOG("ERROR  -->  MAC : MacProcSchCellDeleteRsp(): CellId[%d] does not exists", schCellDelRsp->cellId);
         status = CELL_ID_INVALID;
         ret = RFAILED;
      }
      if(MacSendCellDeleteRsp(status, schCellDelRsp->cellId) != ROK)
      {
         DU_LOG("\nERROR  -->  MAC: MacProcSchCellDeleteRsp(): Failed to send CELL delete response");
         ret = RFAILED;
      }

   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: MacProcSchCellDeleteRsp(): schCellDelRsp is NULL");
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Sends Cell delete req to Scheduler
 *
 * @details
 *
 *    Function : sendCellDelReqToSch
 *
 *    Functionality: sends Cell delete req to Scheduler
 *
 * @params[in]  SchCellDelete *schCellDel
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t sendCellDelReqToSch(SchCellDelete *schCellDel)
{
   Pst schPst;
   FILL_PST_MAC_TO_SCH(schPst, EVENT_CELL_DELETE_REQ_TO_SCH);
   return(*macSchCellDeleteReqOpts[schPst.selector])(&schPst, schCellDel);
}

/*******************************************************************
 *
 * @brief Handles CELL Delete requst from DU APP
 *
 * @details
 *
 *    Function : MacProcCellDeleteReq
 *
 *    Functionality: Handles CELL Delete requst from DU APP
 *
 * @params[in] Pst *pst, MacCellDelete *cellDelete
 * @return ROK     - success
 *         RFAILED - failure
 *
 *
 * ****************************************************************/
uint8_t MacProcCellDeleteReq(Pst *pst, MacCellDelete *cellDelete)
{
   uint8_t ret = ROK, cellIdx=0;
   SchCellDelete schCellDelete;

   DU_LOG("\nINFO   -->  MAC : Cell Delete Request received for cellId[%d]", cellDelete->cellId);

   if(cellDelete)
   {
      GET_CELL_IDX(cellDelete->cellId, cellIdx);
      if(macCb.macCell[cellIdx])
      {
         if(macCb.macCell[cellIdx]->cellId == cellDelete->cellId)
         {
            memset(&schCellDelete, 0, sizeof(SchCellDelete));
            schCellDelete.cellId =  cellDelete->cellId;
            ret = sendCellDelReqToSch(&schCellDelete);
            if(ret != ROK)
            {
               DU_LOG("\nERROR  -->  MAC : MacProcCellDeleteReq(): Failed to send UE Delete Request to SCH");
               ret = RFAILED;
            }
         }
         else
         {
            DU_LOG("\nERROR  -->  MAC : MacProcCellDeleteReq(): Failed to find the MacUeCb of CellId = %d",\
            cellDelete->cellId);
            ret = RFAILED;
         }
      }
      else
      {
         DU_LOG("\nERROR  -->  MAC : MacProcCellDeleteReq(): Failed to find the MacUeCb of CellId = %d",\
               cellDelete->cellId);
         ret = RFAILED;
      }

      if(ret == RFAILED)
      {
          DU_LOG("\nERROR  -->  MAC : MacProcCellDeleteReq(): Sending failure response to DU");
          if(MacSendCellDeleteRsp(CELL_ID_INVALID, cellDelete->cellId) != ROK)
          {
             DU_LOG("\nERROR  -->  MAC : MacProcCellDeleteReq(): failed to send cell delete rsp for cellID[%d]",\
             cellDelete->cellId);
          }

      }
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, cellDelete, sizeof(MacCellDelete));
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC : MacProcCellDeleteReq(): Received MacCellDelete is NULL");
      ret = RFAILED;
   }
   return ret;
}

/**
 * @brief free the temporary slice cfg stored in macCb.
 *
 * @details
 *
 *     Function : freeMacSliceCfgReq 
 *
 *   free the temporary slice cfg stored in macCb
 *
 *  @param[in]  
 *  @return  int
 *      -# ROK
 **/
void freeMacSliceCfgReq(MacSliceCfgReq *cfgReq,Pst *pst)
{
   uint8_t cfgIdx = 0;
   
   if(cfgReq)
   {
      if(cfgReq->numOfConfiguredSlice)
      {
         for(cfgIdx = 0; cfgIdx<cfgReq->numOfConfiguredSlice; cfgIdx++)
         {
            if(cfgReq->listOfSliceCfg[cfgIdx])
            {
               MAC_FREE_SHRABL_BUF(pst->region, pst->pool, cfgReq->listOfSliceCfg[cfgIdx]->rrmPolicyRatio, sizeof(RrmPolicyRatio)); 
            }
            MAC_FREE_SHRABL_BUF(pst->region, pst->pool, cfgReq->listOfSliceCfg[cfgIdx], sizeof(MacSliceRrmPolicy));
         }
         MAC_FREE_SHRABL_BUF(pst->region, pst->pool, cfgReq->listOfSliceCfg, cfgReq->numOfConfiguredSlice * sizeof(MacSliceRrmPolicy*));
      }
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, cfgReq, sizeof(MacSliceCfgReq));
   }
}
/**
 * @brief fill Mac Slice Config Rsp
 *
 * @details
 *
 *     Function : fillMacSliceCfgRsp 
 *
 *     This function   fill Mac Slice Config Rsp
 *
 *  @param[in]  SchSliceCfgRsp *sliceCfgRsp, MacSliceCfgRsp *macSliceCfgRsp,
 *  uint8_t *count
 *  @return  int
 *      -# ROK
 **/
uint8_t fillMacSliceCfgRsp(SchSliceCfgRsp *schSliceCfgRsp, MacSliceCfgRsp *macSliceCfgRsp)
{
   
    bool sliceFound = false;
    uint8_t cfgIdx = 0;

    macSliceCfgRsp->numSliceCfgRsp  = schSliceCfgRsp->numSliceCfgRsp;
    MAC_ALLOC_SHRABL_BUF(macSliceCfgRsp->listOfSliceCfgRsp,  macSliceCfgRsp->numSliceCfgRsp* sizeof(MacSliceRsp*));
    if(macSliceCfgRsp->listOfSliceCfgRsp == NULLP)
    {
       DU_LOG("\nERROR  -->  MAC : Memory allocation failedi in fillMacSliceCfgRsp");
       return RFAILED;
    }

    for(cfgIdx = 0; cfgIdx<schSliceCfgRsp->numSliceCfgRsp; cfgIdx++)
    {
       sliceFound = false;
       if(schSliceCfgRsp->listOfSliceCfgRsp[cfgIdx]->rsp == RSP_OK)
       {
          sliceFound = true;
       }

       MAC_ALLOC_SHRABL_BUF(macSliceCfgRsp->listOfSliceCfgRsp[cfgIdx], sizeof(SliceRsp));
       if(macSliceCfgRsp->listOfSliceCfgRsp[cfgIdx] == NULLP)
       {
          DU_LOG("\nERROR  -->  MAC : Memory allocation failedi in fillMacSliceCfgRsp");
          return RFAILED;
       }

       macSliceCfgRsp->listOfSliceCfgRsp[cfgIdx]->snssai = schSliceCfgRsp->listOfSliceCfgRsp[cfgIdx]->snssai;
       if(sliceFound == true)
          macSliceCfgRsp->listOfSliceCfgRsp[cfgIdx]->rsp    = MAC_DU_APP_RSP_OK;
       else
       {
          macSliceCfgRsp->listOfSliceCfgRsp[cfgIdx]->rsp    = MAC_DU_APP_RSP_NOK;
          macSliceCfgRsp->listOfSliceCfgRsp[cfgIdx]->cause  = SLICE_NOT_PRESENT;
       }
    }
    return ROK;
}

/**
 * @brief send slice cfg response to duapp.
 *
 * @details
 *
 *     Function : MacSendSliceConfigRsp
 *
 *   sends  slice cfg response to duapp
 *
 *  @param[in] MacSliceCfgRsp macSliceCfgRsp 
 *  @return  int
 *      -# ROK
 **/
uint8_t MacSendSliceConfigRsp(MacSliceCfgRsp *macSliceCfgRsp)
{
    Pst  rspPst;
    
    memset(&rspPst, 0, sizeof(Pst));
    FILL_PST_MAC_TO_DUAPP(rspPst, EVENT_MAC_SLICE_CFG_RSP);
    return (*macDuSliceCfgRspOpts[rspPst.selector])(&rspPst, macSliceCfgRsp);

}
/**
 * @brief free the slice cfg rsp received from sch.
 *
 * @details
 *
 *     Function : freeSchSliceCfgRsp 
 *
 *     This free the slice cfg rsp received from sch
 *
 *  @param[in]  SchSliceCfgRsp *sliceCfgrsp
 *  @return  int
 *      -# ROK
 **/
void freeSchSliceCfgRsp(SchSliceCfgRsp *schSliceCfgRsp)
{
   uint8_t cfgIdx = 0;

   if(schSliceCfgRsp)
   {
      if(schSliceCfgRsp->numSliceCfgRsp)
      {
         for(cfgIdx = 0; cfgIdx< schSliceCfgRsp->numSliceCfgRsp; cfgIdx++)
         {
            MAC_FREE(schSliceCfgRsp->listOfSliceCfgRsp[cfgIdx], sizeof(SliceRsp));
         }
         MAC_FREE(schSliceCfgRsp->listOfSliceCfgRsp, schSliceCfgRsp->numSliceCfgRsp * sizeof(SliceRsp*));
      }
   }
}

/**
 * @brief Mac process the slice cfg rsp received from sch.
 *
 * @details
 *
 *     Function : MacProcSchSliceCfgRsp 
 *
 *     This function  process the slice cfg rsp received from sch
 *
 *  @param[in]  Pst           *pst
 *  @param[in]  SchSliceCfgRsp *sliceCfgrsp
 *  @return  int
 *      -# ROK
 **/
uint8_t MacProcSchSliceCfgRsp(Pst *pst, SchSliceCfgRsp *schSliceCfgRsp)
{
   MacSliceCfgRsp *macSliceCfgRsp = NULLP;

   if(schSliceCfgRsp)
   {
      MAC_ALLOC_SHRABL_BUF(macSliceCfgRsp, sizeof(MacSliceCfgRsp));
      if(macSliceCfgRsp == NULLP)
      {
          DU_LOG("\nERROR  -->  MAC : Failed to allocate memory in MacProcSchSliceCfgRsp");
          return RFAILED;
      }
      if(schSliceCfgRsp->listOfSliceCfgRsp)
      {
         if(fillMacSliceCfgRsp(schSliceCfgRsp, macSliceCfgRsp) != ROK)
         {
            DU_LOG("\nERROR  -->  MAC : Failed to fill the slice cfg response");
            return RFAILED;
         }
         MacSendSliceConfigRsp(macSliceCfgRsp);
      }
      freeSchSliceCfgRsp(schSliceCfgRsp);
   }
   return ROK;
}

/**
* @brief send slice cfg response to duapp.
*
* @details
*
*     Function : MacSendSliceReconfigRsp 
*
*   sends  slice cfg response to duapp
*
*  @param[in] MacSliceCfgRsp macSliceRecfgRsp
*  @return  int
*      -# ROK
**/
uint8_t MacSendSliceReconfigRsp(MacSliceCfgRsp *macSliceRecfgRsp)
{
   Pst  rspPst;

   memset(&rspPst, 0, sizeof(Pst));
   FILL_PST_MAC_TO_DUAPP(rspPst, EVENT_MAC_SLICE_RECFG_RSP);
   return (*macDuSliceReCfgRspOpts[rspPst.selector])(&rspPst, macSliceRecfgRsp);

}

/**
 * @brief Mac process the slice cfg rsp received from sch.
 *
 * @details
 *
 *     Function : MacProcSchSliceReCfgRsp 
 *
 *     This function  process the slice cfg rsp received from sch
 *
 *  @param[in]  Pst           *pst
 *  @param[in]  SchSliceCfgRsp *schSliceRecfgRsp
 *  @return  int
 *      -# ROK
 **/
uint8_t MacProcSchSliceReCfgRsp(Pst *pst, SchSliceCfgRsp *schSliceRecfgRsp)
{
   MacSliceCfgRsp *macSliceReCfgRsp = NULLP;

   if(schSliceRecfgRsp)
   {
      MAC_ALLOC_SHRABL_BUF(macSliceReCfgRsp, sizeof(MacSliceCfgRsp));
      if(macSliceReCfgRsp == NULLP)
      {
          DU_LOG("\nERROR  -->  MAC : Failed to allocate memory in MacProcSchSliceReCfgRsp");
          return RFAILED;
      }

      if(schSliceRecfgRsp->listOfSliceCfgRsp)
      {
         if(fillMacSliceCfgRsp(schSliceRecfgRsp, macSliceReCfgRsp) != ROK)
         {
            DU_LOG("\nERROR  -->  MAC : Failed to fill the slice Recfg response");
            return RFAILED;
         }
         MacSendSliceReconfigRsp(macSliceReCfgRsp);
      }
      freeSchSliceCfgRsp(schSliceRecfgRsp);
   }
   return ROK;
}

/**
 * @brief Mac process the downlink pcch indication received from DUAPP
 *
 * @details
 *
 *     Function : MacProcDlPcchInd 
 *
 *     This function process the downlink pcch indication received from DUAPP
 *
 *  @param[in]  Pst           *pst
 *  @param[in]  MacPcchInd    *pcchInd 
 *  @return  int
 *      -# ROK
 **/
uint8_t MacProcDlPcchInd(Pst *pst, MacPcchInd *pcchInd)
{
   uint8_t ret = RFAILED;
   uint16_t cellIdx = 0;
   Pst       schPst;
   SchPageInd *schPageInd = NULLP;

   if(pcchInd)
   {
      DU_LOG("\nINFO   -->  MAC : Received DL PCCH IND from DU_APP for cellId[%d]", pcchInd->cellId);
      
      GET_CELL_IDX(pcchInd->cellId, cellIdx);

      if(macCb.macCell[cellIdx] == NULLP || macCb.macCell[cellIdx]->cellId != pcchInd->cellId)
      {
         DU_LOG("\nERROR  -->  MAC : MacProcDlPcchInd(): CellId[%d] does not exist", pcchInd->cellId);
      }
      else
      {
         if((pcchInd->pcchPdu == NULLP) || (pcchInd->pduLen <= 0))
         {
            DU_LOG("\nERROR  -->  MAC : MacProcDlPcchInd(): Received Pcch pdu is null");
         }
         else
         {
            MAC_ALLOC(schPageInd, sizeof(SchPageInd));
            if(schPageInd == NULLP)
            {
               DU_LOG("\nERROR  -->  MAC : MacProcDlPcchInd(): Failed to allocate memory");
            }
            else
            {
               schPageInd->cellId = pcchInd->cellId;
               schPageInd->pf = pcchInd->pf;
               schPageInd->i_s = pcchInd->i_s;
               schPageInd->pduLen = pcchInd->pduLen;
               
               MAC_ALLOC(schPageInd->pagePdu, pcchInd->pduLen);
               if(schPageInd->pagePdu == NULLP)
               {
                  DU_LOG("\nERROR  -->  MAC : MacProcDlPcchInd(): Failed to allocate memory");
                  MAC_FREE(schPageInd, sizeof(SchPageInd));
               }
               else
               {
                  memcpy(schPageInd->pagePdu, pcchInd->pcchPdu, pcchInd->pduLen);

                  FILL_PST_MAC_TO_SCH(schPst, EVENT_PAGING_IND_TO_SCH);
                  ret = (*macSchPagingIndOpts[schPst.selector])(&schPst, schPageInd);
               }
            }
         }
      }
      if((pcchInd->pcchPdu) && (pcchInd->pduLen > 0))
      {
         MAC_FREE_SHRABL_BUF(pst->region, pst->pool, pcchInd->pcchPdu, pcchInd->pduLen);
      }
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, pcchInd, sizeof(MacPcchInd));
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC : MacProcDlPcchInd(): Received Null pointer");
   }
   return ret;
}
/**********************************************************************
  End of file
 **********************************************************************/
