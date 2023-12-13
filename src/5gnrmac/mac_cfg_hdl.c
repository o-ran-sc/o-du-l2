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

MacDuSliceRecfgRspFunc macDuSliceRecfgRspOpts[] =
{
   packDuMacSliceRecfgRsp,   /* packing for loosely coupled */
   DuProcMacSliceRecfgRsp,   /* packing for tightly coupled */
   packDuMacSliceRecfgRsp    /* packing for light weight loosly coupled */
};

MacDuStatsRspFunc macDuStatsRspOpts[] =
{
   packDuMacStatsRsp,   /* packing for loosely coupled */
   DuProcMacStatsRsp,   /* packing for tightly coupled */
   packDuMacStatsRsp   /* packing for light weight loosly coupled */
};

MacDuStatsDeleteRspFunc macDuStatsDeleteRspOpts[] =
{
   packDuMacStatsDeleteRsp,   /* packing for loosely coupled */
   DuProcMacStatsDeleteRsp,   /* packing for tightly coupled */
   packDuMacStatsDeleteRsp   /* packing for light weight loosly coupled */
};

MacDuStatsModificationRspFunc macDuStatsModificationRspOpts[] =
{
   packDuMacStatsModificationRsp,   /* packing for loosely coupled */
   DuProcMacStatsModificationRsp,   /* packing for tightly coupled */
   packDuMacStatsModificationRsp   /* packing for light weight loosly coupled */
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

   DU_LOG("\nINFO  -->  MAC : Received Scheduler gen config at MAC");
   memset(&schPst, 0, sizeof(Pst));
   FILL_PST_MAC_TO_SCH(schPst, EVENT_SCH_GEN_CFG);
   
   return(SchMessageRouter(&schPst, (void *)cfg));
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
   DU_LOG("\nDEBUG  -->  MAC : Sending Scheduler config confirm to DU APP");
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
   uint16_t cellIdx, scsInKhz = 0;
   uint8_t ret = ROK, plmnIdx = 0,sliceIdx = 0;
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
   scsInKhz = convertScsEnumValToScsVal(macCellCfg->cellCfg.subCarrSpacing);
   
   /*Ref : 3GPP 38.211 Table 4.2-1: SCS = (2 ^ numerology * 15kHz)*/
   macCb.macCell[cellIdx]->numerology = log2(scsInKhz/BASE_SCS);
   macCb.macCell[cellIdx]->numOfSlots = 10 * (1 << (macCb.macCell[cellIdx]->numerology));
   memcpy(&macCb.macCell[cellIdx]->macCellCfg, macCellCfg, sizeof(MacCellCfg));

   MAC_ALLOC(macCb.macCell[cellIdx]->macCellCfg.cellCfg.sib1Cfg.sib1Pdu, \
         macCb.macCell[cellIdx]->macCellCfg.cellCfg.sib1Cfg.sib1PduLen);
   if(macCb.macCell[cellIdx]->macCellCfg.cellCfg.sib1Cfg.sib1Pdu == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : macCellCb is NULL at handling of sib1Pdu of macCellCfg\n");
      return RFAILED;
   }
   memcpy(macCb.macCell[cellIdx]->macCellCfg.cellCfg.sib1Cfg.sib1Pdu, macCellCfg->cellCfg.sib1Cfg.sib1Pdu, \
         macCb.macCell[cellIdx]->macCellCfg.cellCfg.sib1Cfg.sib1PduLen);

   for(plmnIdx = 0; plmnIdx < MAX_PLMN; plmnIdx++)
   {
      macCb.macCell[cellIdx]->macCellCfg.cellCfg.plmnInfoList[plmnIdx].suppSliceList.numSupportedSlices = macCellCfg->cellCfg.plmnInfoList[plmnIdx].suppSliceList.numSupportedSlices;
      MAC_ALLOC(macCb.macCell[cellIdx]->macCellCfg.cellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai, macCb.macCell[cellIdx]->macCellCfg.cellCfg.plmnInfoList[plmnIdx].suppSliceList.numSupportedSlices\
            * sizeof(Snssai*));
      if(macCb.macCell[cellIdx]->macCellCfg.cellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai == NULLP)
      {
         DU_LOG("\nERROR  --> MAC: Memory allocation failed at MacProcCellCfgReq");
         return RFAILED;
      }

      if(macCb.macCell[cellIdx]->macCellCfg.cellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai)
      {
         for(sliceIdx=0; sliceIdx<macCb.macCell[cellIdx]->macCellCfg.cellCfg.plmnInfoList[plmnIdx].suppSliceList.numSupportedSlices; sliceIdx++)
         {
            if(macCellCfg->cellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai[sliceIdx])
            {
               MAC_ALLOC(macCb.macCell[cellIdx]->macCellCfg.cellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai[sliceIdx], sizeof(Snssai));
               if(!macCb.macCell[cellIdx]->macCellCfg.cellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai[sliceIdx])
               {
                  DU_LOG("\nERROR  --> MAC: Memory allocation failed at MacProcCellCfgReq");
                  return RFAILED;
               }
               memcpy(macCb.macCell[cellIdx]->macCellCfg.cellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai[sliceIdx], macCellCfg->cellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai[sliceIdx],\
                     sizeof(Snssai));
            }
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
   uint8_t    ssbMaskIdx = 0, rsrcListIdx = 0, sliceIdx=0, plmnIdx = 0;

   memset(&cfgPst, 0, sizeof(Pst));
   memset(&schCellCfg, 0, sizeof(SchCellCfg));
   schCellCfg.cellId = macCellCfg->cellId;
   schCellCfg.phyCellId = macCellCfg->cellCfg.phyCellId;

   for(plmnIdx = 0; plmnIdx < MAX_PLMN; plmnIdx++)
   {
      schCellCfg.plmnInfoList[plmnIdx].plmn = macCellCfg->cellCfg.plmnInfoList[plmnIdx].plmn;
      if(macCellCfg->cellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai) 
      {
         schCellCfg.plmnInfoList[plmnIdx].suppSliceList.numSupportedSlices = macCellCfg->cellCfg.plmnInfoList[plmnIdx].suppSliceList.numSupportedSlices;
         MAC_ALLOC(schCellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai, schCellCfg.plmnInfoList[plmnIdx].suppSliceList.numSupportedSlices * sizeof(Snssai*));
         if(!schCellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai)
         {
            DU_LOG("\nERROR  --> MAC: Memory allocation failed at MacSchCellCfgReq");
            return RFAILED;
         }
         for(sliceIdx=0; sliceIdx<schCellCfg.plmnInfoList[plmnIdx].suppSliceList.numSupportedSlices; sliceIdx++)
         {
            if(macCellCfg->cellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai[sliceIdx])
            {
               MAC_ALLOC(schCellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai[sliceIdx], sizeof(Snssai));
               if(!schCellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai[sliceIdx])
               {
                  DU_LOG("\nERROR  --> MAC: Memory allocation failed at MacSchCellCfgReq");
                  return RFAILED;
               }
               memcpy(schCellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai[sliceIdx], macCellCfg->cellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai[sliceIdx],  sizeof(Snssai));
            }
         }
      }
   }

   schCellCfg.dupMode = macCellCfg->cellCfg.dupType;
   schCellCfg.dlBandwidth = macCellCfg->carrCfg.dlBw;
   schCellCfg.ulBandwidth = macCellCfg->carrCfg.ulBw;

   schCellCfg.dlCfgCommon.schFreqInfoDlSib.offsetToPointA = macCellCfg->ssbCfg.ssbOffsetPointA;
   schCellCfg.dlCfgCommon.schFreqInfoDlSib.schSpcCarrier[0].subCarrierSpacing = macCellCfg->ssbCfg.scsCmn;

   /* fill initial DL BWP */
   schCellCfg.dlCfgCommon.schInitialDlBwp.bwp.freqAlloc.startPrb = macCellCfg->cellCfg.initialDlBwp.bwp.firstPrb;
   schCellCfg.dlCfgCommon.schInitialDlBwp.bwp.freqAlloc.numPrb = macCellCfg->cellCfg.initialDlBwp.bwp.numPrb;
   schCellCfg.dlCfgCommon.schInitialDlBwp.bwp.scs = macCellCfg->cellCfg.initialDlBwp.bwp.scs;
   schCellCfg.dlCfgCommon.schInitialDlBwp.bwp.cyclicPrefix = macCellCfg->cellCfg.initialDlBwp.bwp.cyclicPrefix;
   
   schCellCfg.dlCfgCommon.schInitialDlBwp.pdcchCommon.commonSearchSpace.searchSpaceId =
      macCellCfg->cellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.searchSpaceId;
   schCellCfg.dlCfgCommon.schInitialDlBwp.pdcchCommon.commonSearchSpace.coresetId =
      macCellCfg->cellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.coresetId;
   schCellCfg.dlCfgCommon.schInitialDlBwp.pdcchCommon.commonSearchSpace.monitoringSlot =
      macCellCfg->cellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.monitoringSlot;
   schCellCfg.dlCfgCommon.schInitialDlBwp.pdcchCommon.commonSearchSpace.duration =
      macCellCfg->cellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.duration;
   schCellCfg.dlCfgCommon.schInitialDlBwp.pdcchCommon.commonSearchSpace.monitoringSymbol =
      macCellCfg->cellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.monitoringSymbol;
   schCellCfg.dlCfgCommon.schInitialDlBwp.pdcchCommon.commonSearchSpace.candidate.aggLevel1 =
      macCellCfg->cellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.candidate.aggLevel1;
   schCellCfg.dlCfgCommon.schInitialDlBwp.pdcchCommon.commonSearchSpace.candidate.aggLevel2 =
      macCellCfg->cellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.candidate.aggLevel2;
   schCellCfg.dlCfgCommon.schInitialDlBwp.pdcchCommon.commonSearchSpace.candidate.aggLevel4 =
      macCellCfg->cellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.candidate.aggLevel4;
   schCellCfg.dlCfgCommon.schInitialDlBwp.pdcchCommon.commonSearchSpace.candidate.aggLevel8 =
      macCellCfg->cellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.candidate.aggLevel8;
   schCellCfg.dlCfgCommon.schInitialDlBwp.pdcchCommon.commonSearchSpace.candidate.aggLevel16 =
      macCellCfg->cellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.candidate.aggLevel16;
   
   schCellCfg.dlCfgCommon.schInitialDlBwp.pdschCommon.numTimeDomAlloc = macCellCfg->cellCfg.initialDlBwp.pdschCommon.numTimeDomAlloc;
   for(rsrcListIdx = 0; rsrcListIdx<macCellCfg->cellCfg.initialDlBwp.pdschCommon.numTimeDomAlloc; rsrcListIdx++)
   {
      schCellCfg.dlCfgCommon.schInitialDlBwp.pdschCommon.timeDomRsrcAllocList[rsrcListIdx].k0 = 
         macCellCfg->cellCfg.initialDlBwp.pdschCommon.timeDomRsrcAllocList[rsrcListIdx].k0;
      schCellCfg.dlCfgCommon.schInitialDlBwp.pdschCommon.timeDomRsrcAllocList[rsrcListIdx].mappingType =
         macCellCfg->cellCfg.initialDlBwp.pdschCommon.timeDomRsrcAllocList[rsrcListIdx].mappingType;
      schCellCfg.dlCfgCommon.schInitialDlBwp.pdschCommon.timeDomRsrcAllocList[rsrcListIdx].startSymbol =
         macCellCfg->cellCfg.initialDlBwp.pdschCommon.timeDomRsrcAllocList[rsrcListIdx].startSymbol;
      schCellCfg.dlCfgCommon.schInitialDlBwp.pdschCommon.timeDomRsrcAllocList[rsrcListIdx].lengthSymbol =
         macCellCfg->cellCfg.initialDlBwp.pdschCommon.timeDomRsrcAllocList[rsrcListIdx].lengthSymbol;
   }

   /* fill SIB1 scheduler parameters */
   schCellCfg.dlCfgCommon.schPcchCfg.numPO  =  macCellCfg->cellCfg.sib1Cfg.pagingCfg.numPO;
   schCellCfg.dlCfgCommon.schPcchCfg.poPresent = macCellCfg->cellCfg.sib1Cfg.pagingCfg.poPresent;
   if(schCellCfg.dlCfgCommon.schPcchCfg.poPresent)
   {
      memcpy(schCellCfg.dlCfgCommon.schPcchCfg.pagingOcc, macCellCfg->cellCfg.sib1Cfg.pagingCfg.pagingOcc, MAX_PO_PER_PF);
   }

   /* fill initial UL BWP */
   schCellCfg.ulCfgCommon.schInitialUlBwp.bwp.freqAlloc.startPrb = macCellCfg->cellCfg.initialUlBwp.bwp.firstPrb;
   schCellCfg.ulCfgCommon.schInitialUlBwp.bwp.freqAlloc.numPrb = macCellCfg->cellCfg.initialUlBwp.bwp.numPrb;
   schCellCfg.ulCfgCommon.schInitialUlBwp.bwp.scs = macCellCfg->cellCfg.initialUlBwp.bwp.scs;
   schCellCfg.ulCfgCommon.schInitialUlBwp.bwp.cyclicPrefix = macCellCfg->cellCfg.initialUlBwp.bwp.cyclicPrefix;
   /* fill RACH config params */
   schCellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.prachCfgGeneric.prachCfgIdx   = macCellCfg->prachCfg.prachCfgIdx;
   schCellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.prachCfgGeneric.msg1Fdm       = macCellCfg->prachCfg.msg1Fdm;
   schCellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.prachCfgGeneric.msg1FreqStart = macCellCfg->prachCfg.msg1FreqStart;
   schCellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.prachCfgGeneric.zeroCorrZoneCfg = macCellCfg->prachCfg.fdm[0].zeroCorrZoneCfg; 
   schCellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.prachCfgGeneric.raRspWindow = macCellCfg->prachCfg.raRspWindow;

   schCellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.totalNumRaPreamble = macCellCfg->prachCfg.totalNumRaPreamble;
   schCellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.ssbPerRach    = macCellCfg->prachCfg.ssbPerRach;
   schCellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.numCbPreamblePerSsb = macCellCfg->prachCfg.numCbPreamblePerSsb;
   schCellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.raContResTmr = macCellCfg->prachCfg.raContResTmr;
   schCellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.rsrpThreshSsb = macCellCfg->prachCfg.rsrpThreshSsb;
   schCellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.rootSeqIdx    = macCellCfg->prachCfg.fdm[0].rootSeqIdx;
   schCellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.numRootSeq    = macCellCfg->prachCfg.fdm[0].numRootSeq;
   schCellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.msg1SubcSpacing = \
                                            macCellCfg->prachCfg.prachSubcSpacing;

   schCellCfg.ulCfgCommon.schInitialUlBwp.pucchCommon.pucchResourceCommon = \
                                                                macCellCfg->cellCfg.initialUlBwp.pucchCommon.pucchResourceCommon;
   schCellCfg.ulCfgCommon.schInitialUlBwp.pucchCommon.pucchGroupHopping = \
                                                                macCellCfg->cellCfg.initialUlBwp.pucchCommon.pucchGroupHopping;
   schCellCfg.ulCfgCommon.schInitialUlBwp.puschCommon.numTimeDomRsrcAlloc = \
                                                                macCellCfg->cellCfg.initialUlBwp.puschCommon.numTimeDomRsrcAlloc;
   for(rsrcListIdx = 0; rsrcListIdx < macCellCfg->cellCfg.initialUlBwp.puschCommon.numTimeDomRsrcAlloc; rsrcListIdx++)
   {
      schCellCfg.ulCfgCommon.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[rsrcListIdx].k2 = 
         macCellCfg->cellCfg.initialUlBwp.puschCommon.timeDomRsrcAllocList[rsrcListIdx].k2;
      schCellCfg.ulCfgCommon.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[rsrcListIdx].mappingType =
         macCellCfg->cellCfg.initialUlBwp.puschCommon.timeDomRsrcAllocList[rsrcListIdx].mappingType;
      schCellCfg.ulCfgCommon.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[rsrcListIdx].startSymbol =
         macCellCfg->cellCfg.initialUlBwp.puschCommon.timeDomRsrcAllocList[rsrcListIdx].startSymbol;
      schCellCfg.ulCfgCommon.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[rsrcListIdx].symbolLength =
         macCellCfg->cellCfg.initialUlBwp.puschCommon.timeDomRsrcAllocList[rsrcListIdx].symbolLength;
   }

#ifdef NR_TDD
   memcpy(&schCellCfg.tddCfg, &macCellCfg->tddCfg, sizeof(TDDCfg));
#endif

   /* fill ssb scheduler parameters */
   for(ssbMaskIdx=0; ssbMaskIdx<SSB_MASK_SIZE; ssbMaskIdx++)
   {
      schCellCfg.ssbPosInBurst[ssbMaskIdx] = macCellCfg->ssbCfg.ssbMask[ssbMaskIdx];
   }
   schCellCfg.ssbPeriod    = ssbPeriodicity[macCellCfg->ssbCfg.ssbPeriod];
   schCellCfg.ssbFrequency = macCellCfg->cellCfg.ssbFreq;
   schCellCfg.dmrsTypeAPos = macCellCfg->ssbCfg.dmrsTypeAPos;
   schCellCfg.ssbScs       = macCellCfg->cellCfg.subCarrSpacing;
   schCellCfg.pdcchCfgSib1.coresetZeroIndex = macCellCfg->cellCfg.sib1Cfg.pdcchCfgSib1.coresetZeroIndex;
   schCellCfg.pdcchCfgSib1.searchSpaceZeroIndex = macCellCfg->cellCfg.sib1Cfg.pdcchCfgSib1.searchSpaceZeroIndex;
   schCellCfg.ssbPbchPwr = macCellCfg->ssbCfg.ssbPbchPwr;
   schCellCfg.ssbSubcOffset = macCellCfg->ssbCfg.ssbScOffset;
   schCellCfg.sib1PduLen = macCellCfg->cellCfg.sib1Cfg.sib1PduLen;
   
   FILL_PST_MAC_TO_SCH(cfgPst, EVENT_SCH_CELL_CFG);

   return(SchMessageRouter(&cfgPst, (void *)&schCellCfg));
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
uint8_t MacSendCellDeleteRsp(CauseOfResult  status, uint8_t cellId)
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
   deleteRsp->status = status;

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
   uint8_t  ret = ROK, sliceIdx = 0, plmnIdx = 0;
   uint16_t cellIdx=0;
   CauseOfResult  cause;

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
               cause = SUCCESSFUL;
               for(plmnIdx = 0; plmnIdx < MAX_PLMN; plmnIdx++)
               {
                  if(macCb.macCell[cellIdx]->macCellCfg.cellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai)
                  {
                     for(sliceIdx = 0; sliceIdx<macCb.macCell[cellIdx]->macCellCfg.cellCfg.plmnInfoList[plmnIdx].suppSliceList.numSupportedSlices; sliceIdx++)
                     {
                        MAC_FREE(macCb.macCell[cellIdx]->macCellCfg.cellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai[sliceIdx], sizeof(Snssai));
                     }
                     MAC_FREE(macCb.macCell[cellIdx]->macCellCfg.cellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai, macCb.macCell[cellIdx]->macCellCfg.cellCfg.plmnInfoList[plmnIdx].\
                           suppSliceList.numSupportedSlices * sizeof(Snssai*));
                  }
               }
               MAC_FREE(macCb.macCell[cellIdx]->macCellCfg.cellCfg.sib1Cfg.sib1Pdu, \
                     macCb.macCell[cellIdx]->macCellCfg.cellCfg.sib1Cfg.sib1PduLen);
               MAC_FREE(macCb.macCell[cellIdx], sizeof(MacCellCb));
            }
            else
            {
               DU_LOG("ERROR  -->  MAC : MacProcSchCellDeleteRsp(): CellId[%d] does not exists", schCellDelRsp->cellId);
               cause = CELLID_INVALID;
               ret = RFAILED;
            }
         }
         else
         {
            DU_LOG("ERROR  -->  MAC : MacProcSchCellDeleteRsp(): CellId[%d] does not exists", schCellDelRsp->cellId);
            cause = CELLID_INVALID;
            ret = RFAILED;
         }
      }
      else
      {
         DU_LOG("ERROR  -->  MAC : MacProcSchCellDeleteRsp(): CellId[%d] does not exists", schCellDelRsp->cellId);
         cause = CELLID_INVALID;
         ret = RFAILED;
      }
      if(MacSendCellDeleteRsp(cause, schCellDelRsp->cellId) != ROK)
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

uint8_t sendCellDelReqToSch(SchCellDeleteReq *schCellDelReq)
{
   Pst schPst;
   FILL_PST_MAC_TO_SCH(schPst, EVENT_CELL_DELETE_REQ_TO_SCH);
   return(SchMessageRouter(&schPst, (void *)schCellDelReq));
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
uint8_t MacProcCellDeleteReq(Pst *pst, MacCellDeleteReq *cellDelete)
{
   uint8_t ret = ROK, cellIdx=0;
   SchCellDeleteReq schCellDelete;

   DU_LOG("\nINFO   -->  MAC : Cell Delete Request received for cellId[%d]", cellDelete->cellId);

   if(cellDelete)
   {
      GET_CELL_IDX(cellDelete->cellId, cellIdx);
      if(macCb.macCell[cellIdx])
      {
         if(macCb.macCell[cellIdx]->cellId == cellDelete->cellId)
         {
            memset(&schCellDelete, 0, sizeof(SchCellDeleteReq ));
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
          if(MacSendCellDeleteRsp(CELLID_INVALID, cellDelete->cellId) != ROK)
          {
             DU_LOG("\nERROR  -->  MAC : MacProcCellDeleteReq(): failed to send cell delete rsp for cellID[%d]",\
             cellDelete->cellId);
          }

      }
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, cellDelete, sizeof(MacCellDeleteReq));
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
    uint8_t policyIdx = 0, memberListIdx=0;

    if(cfgReq->numOfRrmPolicy)
    {
       if(cfgReq->listOfRrmPolicy)
       {
          for(policyIdx = 0; policyIdx<cfgReq->numOfRrmPolicy; policyIdx++)
          {
             if(cfgReq->listOfRrmPolicy[policyIdx])
             {
                if(cfgReq->listOfRrmPolicy[policyIdx]->numOfRrmPolicyMem)
                {
                   if(cfgReq->listOfRrmPolicy[policyIdx]->rRMPolicyMemberList)
                   {
                      for(memberListIdx = 0; memberListIdx<cfgReq->listOfRrmPolicy[policyIdx]->numOfRrmPolicyMem; memberListIdx++)
                      {
                         MAC_FREE_SHRABL_BUF(pst->region, pst->pool, cfgReq->listOfRrmPolicy[policyIdx]->rRMPolicyMemberList[memberListIdx], sizeof(RrmPolicyMemberList));
                      }
                      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, cfgReq->listOfRrmPolicy[policyIdx]->rRMPolicyMemberList,\
                      cfgReq->listOfRrmPolicy[policyIdx]->numOfRrmPolicyMem * sizeof(RrmPolicyMemberList*));
                   }
                }
                MAC_FREE_SHRABL_BUF(pst->region, pst->pool, cfgReq->listOfRrmPolicy[policyIdx], sizeof(MacSliceRrmPolicy));
             }
          }
          MAC_FREE_SHRABL_BUF(pst->region, pst->pool, cfgReq->listOfRrmPolicy, cfgReq->numOfRrmPolicy  * sizeof(MacSliceRrmPolicy*));
       }
    }

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
      macSliceCfgRsp->snssai = schSliceCfgRsp->snssai;
      if(schSliceCfgRsp->rsp == RSP_OK)
         macSliceCfgRsp->rsp    = MAC_DU_APP_RSP_OK;
      else
      {
         macSliceCfgRsp->rsp    = MAC_DU_APP_RSP_NOK;
      }
      macSliceCfgRsp->cause  = schSliceCfgRsp->cause;
      MacSendSliceConfigRsp(macSliceCfgRsp);
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
 *  @param[in] MacSliceRecfgRsp macSliceRecfgRsp
 *  @return  int
 *      -# ROK
 **/
uint8_t MacSendSliceReconfigRsp(MacSliceRecfgRsp *macSliceRecfgRsp)
{
   Pst  rspPst;

   memset(&rspPst, 0, sizeof(Pst));
   FILL_PST_MAC_TO_DUAPP(rspPst, EVENT_MAC_SLICE_RECFG_RSP);
   return (*macDuSliceRecfgRspOpts[rspPst.selector])(&rspPst, macSliceRecfgRsp);

}

/**
 * @brief Mac process the slice cfg rsp received from sch.
 *
 * @details
 *
 *     Function : MacProcSchSliceRecfgRsp 
 *
 *     This function  process the slice cfg rsp received from sch
 *
 *  @param[in]  Pst           *pst
 *  @param[in]  SchSliceCfgRsp *schSliceRecfgRsp
 *  @return  int
 *      -# ROK
 **/
uint8_t MacProcSchSliceRecfgRsp(Pst *pst, SchSliceRecfgRsp *schSliceRecfgRsp)
{
   MacSliceRecfgRsp *macSliceRecfgRsp = NULLP;

   if(schSliceRecfgRsp)
   {
      MAC_ALLOC_SHRABL_BUF(macSliceRecfgRsp, sizeof(MacSliceRecfgRsp));
      if(macSliceRecfgRsp == NULLP)
      {
          DU_LOG("\nERROR  -->  MAC : Failed to allocate memory in MacProcSchSliceRecfgRsp");
          return RFAILED;
      }

      macSliceRecfgRsp->snssai = schSliceRecfgRsp->snssai;
      if(schSliceRecfgRsp->rsp == RSP_OK)
         macSliceRecfgRsp->rsp    = MAC_DU_APP_RSP_OK;
      else
      {
         macSliceRecfgRsp->rsp    = MAC_DU_APP_RSP_NOK;
      }
      macSliceRecfgRsp->cause  = schSliceRecfgRsp->cause;
      MacSendSliceReconfigRsp(macSliceRecfgRsp);
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
 *  @param[in]  DlPcchInd    *pcchInd 
 *  @return  int
 *      -# ROK
 **/
uint8_t MacProcDlPcchInd(Pst *pst, DlPcchInd *pcchInd)
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
                  ret = SchMessageRouter(&schPst, (void *)schPageInd);
               }
            }
         }
      }
      if((pcchInd->pcchPdu) && (pcchInd->pduLen > 0))
      {
         MAC_FREE_SHRABL_BUF(pst->region, pst->pool, pcchInd->pcchPdu, pcchInd->pduLen);
      }
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, pcchInd, sizeof(DlPcchInd));
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC : MacProcDlPcchInd(): Received Null pointer");
   }
   return ret;
}

/**
 * @brief Mac process the downlink Broadcast Req received from DUAPP
 *
 * @details
 *
 *     Function : MacProcDlBroadcastReq 
 *
 *     This function process the downlink Broadcast Req received from DUAPP
 *
 *  @param[in]  Pst           *pst
 *  @param[in]  DlBroadcastReq    *dlBroadcastReq 
 *  @return  int
 *      -# ROK
 **/
uint8_t MacProcDlBroadcastReq(Pst *pst, MacDlBroadcastReq *dlBroadcastReq)
{
   uint8_t ret = ROK, idx=0;
   uint16_t cellIdx = 0;

   if(dlBroadcastReq)
   {
      DU_LOG("\nINFO   -->  MAC : Received DL braodcast req from DU_APP for cellId[%d]", dlBroadcastReq->cellId);
      
      GET_CELL_IDX(dlBroadcastReq->cellId, cellIdx);

      if(macCb.macCell[cellIdx] == NULLP || macCb.macCell[cellIdx]->cellId != dlBroadcastReq->cellId)
      {
         ret = RFAILED;
         DU_LOG("\nERROR  -->  MAC : MacProcDlBroadcastReq(): CellId[%d] does not exist", dlBroadcastReq->cellId);
      }
      else
      {
         /*TODO - Complete the processing of DL Broadcast Request*/
      }
      for(idx = 0; idx<dlBroadcastReq->numSiBlock; idx++)
      {
         MAC_FREE_SHRABL_BUF(pst->region, pst->pool, dlBroadcastReq->siSchedulingInfo[idx]->siAreaID, sizeof(uint8_t));
         MAC_FREE_SHRABL_BUF(pst->region, pst->pool, dlBroadcastReq->siSchedulingInfo[idx], sizeof(SiSchedulingInfo));
      }
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, dlBroadcastReq, sizeof(MacDlBroadcastReq));
   }
   else
   {
      ret = RFAILED;
      DU_LOG("\nERROR  -->  MAC : MacProcDlBroadcastReq(): Received Null pointer");
   }
   return ret;
}

/**
 * @brief Fill and send statistics response to DU APP
 *
 * @details
 *
 *     Function : MacSendStatsRspToDuApp
 *
 *     Fill and send statistics response to DU APP
 *
 *  @param[in]  Response
 *  @param[in]  Cause of response
 *  @return  int
 *      -# ROK
 **/
uint8_t MacSendStatsRspToDuApp(MacStatsRsp *statsRsp)
{
   uint8_t ret = ROK;
   Pst  pst;
   MacStatsRsp *macStatsRsp = NULLP;

    DU_LOG("\nINFO  -->  MAC : MacSendStatsRspToDuApp: Sending Statistics Response to DU APP");

   /* Workaround : To skip corrupted memory, allocating a pointer that will
    * remain unused */
   uint8_t *dummyPtr = NULLP;
   MAC_ALLOC_SHRABL_BUF(dummyPtr, sizeof(uint8_t));

   MAC_ALLOC_SHRABL_BUF(macStatsRsp, sizeof(MacStatsRsp));
   if(macStatsRsp == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : Failed to allocate memory in MacProcSchStatsRsp");
      ret = RFAILED;
   }
   else
   {
      memcpy(macStatsRsp, statsRsp, sizeof(MacStatsRsp));
      memset(statsRsp, 0, sizeof(MacStatsRsp));

      memset(&pst, 0, sizeof(Pst));
      FILL_PST_MAC_TO_DUAPP(pst, EVENT_MAC_STATISTICS_RSP);
      if(((*macDuStatsRspOpts[pst.selector])(&pst, macStatsRsp))!= ROK)
      {
         DU_LOG("\nERROR  -->  MAC : Failed to send statistics response to DU APP");
         MAC_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, macStatsRsp, sizeof(MacStatsRsp));
         ret = RFAILED;
      }
   }

   /* Workaround : Freeing the dummy pointer */
   MAC_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, dummyPtr, sizeof(uint8_t));
   return ret;
}

/*******************************************************************
 *
 * @brief Rejects all statistics group requested by DU APP
 *
 * @details
 *
 *    Function : MacRejectAllStats
 *
 *    Functionality: Add all statistics group received in statistics
 *       request from DU APP, to Reject-Stats-Group-List in statistics
 *       response to DU APP
 *
 * @params[in]  Statistics request from DU APP
 *              Cause of rejection
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacRejectAllStats(MacStatsReq *macStatsReq, CauseOfResult cause)
{
   uint8_t grpIdx = 0;
   MacStatsRsp macStatsRsp;

   memset(&macStatsRsp, 0, sizeof(MacStatsRsp));

   /* Copying all stats group from stats request to stats response */
   macStatsRsp.subscriptionId = macStatsReq->subscriptionId;
   for(grpIdx = 0; grpIdx < macStatsReq->numStatsGroup; grpIdx++)
   {
      macStatsRsp.statsGrpRejectedList[grpIdx].groupId = macStatsReq->statsGrpList[grpIdx].groupId;
      macStatsRsp.statsGrpRejectedList[grpIdx].cause = cause;
   }
   macStatsRsp.numGrpRejected = macStatsReq->numStatsGroup;

   return MacSendStatsRspToDuApp(&macStatsRsp);
}

/**
 * @brief Mac process the statistics Req received from DUAPP
 *
 * @details
 *
 *     Function : MacProcStatsReq
 *
 *     This function process the statistics request from duapp:
 *     [Step 1] Basic validation. If fails, all stats group in stats request are
 *     rejected.
 *     [Step 2] If basic validations passed, traverse all stats group and
 *     validate each measurement types in each group.
 *     [Step 3] If any measurement type validation fails in a group, that group 
 *     is not configured and it is added to stats-group-rejected-list in
 *     mac-stats-response message.
 *     [Step 4] Even if one group passes all validation, it is sent to SCH in
 *     statistics request. The mac-stats-response message is added to
 *     pending-response list. This will be sent to DU APP after stats response
 *     is received from SCH.
 *     [Step 5] If none of the groups passes all validation, mac-stats-response
 *     is sent to du app with all group as part of stats-group-rejected-list.
 *
 *  @param[in]  Pst      *pst
 *  @param[in]  StatsReq *statsReq
 *  @return  int
 *      -# ROK
 **/
uint8_t MacProcStatsReq(Pst *pst, MacStatsReq *macStatsReq)
{
   uint8_t       macStatsGrpIdx = 0, macStatsIdx = 0, schStatsGrpIdx = 0, schStatsIdx = 0;
   uint8_t       ret = RFAILED;
   bool          measTypeInvalid = false;
   Pst           schPst;
   MacStatsGrpInfo *macStatsGrp = NULLP;
   SchStatsReq     *schStatsReq = NULLP;
   MacStatsRsp     *macStatsRsp = NULLP;

   DU_LOG("\nINFO   -->  MAC : Received Statistics Request from DU_APP");

   if(macStatsReq == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : MacProcStatsReq(): Received Null pointer");
      return RFAILED;
   }
   
   /* [Step 1] Basic validation. If fails, statistics response is sent to DU APP
    * that rejectes all stats */

   /* If number of statistics request for which response is still pending
    * towards DU APP has reached its maximum limit */
   if(macCb.statistics.numPendingStatsRsp >= MAX_PENDING_STATS_RSP)
   {
      DU_LOG("\nERROR  -->  MAC : MacProcStatsReq: Maximum number of statistics response is pending. \
         Cannot process new request."); 
      MacRejectAllStats(macStatsReq, RESOURCE_UNAVAILABLE);
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, macStatsReq, sizeof(MacStatsReq));
      return RFAILED;
   }

   /* If memory resources are unavailable */
   MAC_ALLOC(schStatsReq, sizeof(SchStatsReq));
   if(schStatsReq == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : MacProcStatsReq: Failed to allocate memory");
      MacRejectAllStats(macStatsReq, RESOURCE_UNAVAILABLE);
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, macStatsReq, sizeof(MacStatsReq));
      return RFAILED;
   }

   /* Add stats response to pending response list */ 
   macStatsRsp = &macCb.statistics.pendingStatsRsp[macCb.statistics.numPendingStatsRsp];
   memset(macStatsRsp, 0, sizeof(MacStatsRsp));

   /* [Step 2] Traverse all stats group and validate each measurement types in each group */
   schStatsReq->subscriptionId = macStatsReq->subscriptionId;
   schStatsReq->numStatsGroup = 0;
   for(macStatsGrpIdx = 0; macStatsGrpIdx < macStatsReq->numStatsGroup; macStatsGrpIdx++)
   {
      measTypeInvalid = false;
      schStatsIdx = 0;
      macStatsGrp = &macStatsReq->statsGrpList[macStatsGrpIdx];

      for(macStatsIdx=0; macStatsIdx < macStatsGrp->numStats; macStatsIdx++)
      {
         /* Validate each measurement type */
         switch(macStatsGrp->statsList[macStatsIdx])
         {
            case MAC_DL_TOTAL_PRB_USAGE:
               {
                  schStatsReq->statsGrpList[schStatsGrpIdx].statsList[schStatsIdx] = SCH_DL_TOTAL_PRB_USAGE;
                  break;
               }
            case MAC_UL_TOTAL_PRB_USAGE:
               {
                  schStatsReq->statsGrpList[schStatsGrpIdx].statsList[schStatsIdx] = SCH_UL_TOTAL_PRB_USAGE;
                  break;
               }
            default:
               {
                  DU_LOG("\nERROR  -->  MAC : MacProcStatsReq: Invalid measurement type [%d]", \
                     macStatsGrp->statsList[macStatsIdx]);
                  measTypeInvalid = true;
               }
         }

         /* Even if one measurement type is invalid, this group is rejected */
         if(measTypeInvalid)
         {
            memset(&schStatsReq->statsGrpList[schStatsGrpIdx], 0, sizeof(SchStatsGrpInfo));
            break;
         }
         
         schStatsIdx++;
      }

      /* If all measurement type is valid, add group info to send to SCH */
      if(!measTypeInvalid)
      {
         schStatsReq->statsGrpList[schStatsGrpIdx].groupId = macStatsGrp->groupId;
         schStatsReq->statsGrpList[schStatsGrpIdx].periodicity = macStatsGrp->periodicity;
         schStatsReq->statsGrpList[schStatsGrpIdx].numStats = schStatsIdx;
         schStatsGrpIdx++;
      }
      else
      {
         /* [Step 3] If any measurement type validation fails in a group, that group 
          * is not configured and it is added to stats-group-rejected-list in
          * mac-stats-response message */
         macStatsRsp->statsGrpRejectedList[macStatsRsp->numGrpRejected].groupId = macStatsGrp->groupId;
         macStatsRsp->statsGrpRejectedList[macStatsRsp->numGrpRejected].cause = PARAM_INVALID;
         macStatsRsp->numGrpRejected++;
      }
   }
   schStatsReq->numStatsGroup = schStatsGrpIdx;

   macStatsRsp->subscriptionId = macStatsReq->subscriptionId;

   if(schStatsReq->numStatsGroup)
   {
      /* [Step 4] Even if one group passes all validation, it is sent to SCH in
       * statistics request. The mac-stats-response message is added to
       * pending-response list. */     
      macCb.statistics.numPendingStatsRsp++;

      FILL_PST_MAC_TO_SCH(schPst, EVENT_STATISTICS_REQ_TO_SCH);
      ret = SchMessageRouter(&schPst, (void *)schStatsReq);
   }
   else
   {
      /* [Step 5] If none of the groups passes all validation, mac-stats-response
       * is sent to du app with all group as part of stats-group-rejected-list. */
      DU_LOG("\nERROR  -->  MAC : MacProcStatsReq: All statistics group found invalid");
      MAC_FREE(schStatsReq, sizeof(SchStatsReq));
      ret = MacSendStatsRspToDuApp(macStatsRsp);
   }

   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, macStatsReq, sizeof(MacStatsReq));
   return ret;
}

/**
 * @brief Mac process the statistics rsp received from sch.
 *
 * @details
 *
 *     Function : MacProcSchStatsRsp
 *
 *     This function  process the statistics response received from sch
 *
 *  @param[in]  Pst           *pst
 *  @param[in]  SchStatsRsp *schStatsRsp
 *  @return  int
 *      -# ROK
 **/
uint8_t MacProcSchStatsRsp(Pst *pst, SchStatsRsp *schStatsRsp)
{
   uint8_t idx = 0, accptdIdx = 0, rjctdIdx = 0;
   uint8_t ret = RFAILED;
   MacStatsRsp *macStatsRsp = NULLP;

   if(schStatsRsp)
   {
      /* Fetch pointer to statistics response from pending list saved at MAC 
       * during processing statistics request from DU APP */ 
      for(idx = 0; idx < macCb.statistics.numPendingStatsRsp; idx++)
      {
         if(macCb.statistics.pendingStatsRsp[idx].subscriptionId == schStatsRsp->subscriptionId)
         {
            macStatsRsp = &macCb.statistics.pendingStatsRsp[idx];
            break;
         }
      }

      if(macStatsRsp == NULLP)
      {
         MAC_FREE(schStatsRsp, sizeof(SchStatsRsp));
         return RFAILED;
      }
      
      /* Copy Stats-group-accpeted list received from SCH */
      for(accptdIdx = 0; accptdIdx<schStatsRsp->numGrpAccepted && macStatsRsp->numGrpAccepted<MAX_NUM_STATS_GRP; accptdIdx++)
      {
         macStatsRsp->statsGrpAcceptedList[macStatsRsp->numGrpAccepted++] = schStatsRsp->statsGrpAcceptedList[accptdIdx];
      }

      /* List together all stats group rejected by MAC and by SCH */
      for(rjctdIdx = 0; rjctdIdx < schStatsRsp->numGrpRejected && macStatsRsp->numGrpRejected<MAX_NUM_STATS_GRP; rjctdIdx++)
      {
         macStatsRsp->statsGrpRejectedList[macStatsRsp->numGrpRejected].groupId = \
            schStatsRsp->statsGrpRejectedList[rjctdIdx].groupId;
         macStatsRsp->statsGrpRejectedList[macStatsRsp->numGrpRejected].cause = \
            schStatsRsp->statsGrpRejectedList[rjctdIdx].cause;
         macStatsRsp->numGrpRejected++;
      }
      
      /* Send statistics response to DU APP */
      ret = MacSendStatsRspToDuApp(macStatsRsp);
   }
   MAC_FREE(schStatsRsp, sizeof(SchStatsRsp));
   return ret;
}

/**
 * @brief Fill and send statistics delete response to DU APP
 *
 * @details
 *
 *     Function : MacSendStatsDeleteRspToDuApp
 *
 *     Fill and send statistics delete response to DU APP
 *  [Step-1] In SCH delete response is received, fill MAC stats delete 
 *    response with info from SCH stats delete response.
 *  [Step-2] If a failure occurs while processing MAC stats delete request, 
 *    then fill MAC stats delete rsp with failure using info from MAC stats delete request.
 *  [Step-3] Else if numStatsGroup > 0, then send stats delete response 
 *    with deletion status of each stats group.
 *  [Step-4] If numStatsGroup = 0, send Stats delete response 
 *    for deletion of complete subscription.
 *
 *  @param[in]  SchStatsDeleteRsp
 *  @param[in]  MacStatsDeleteReq
 *  @return  int
 *      -# ROK
 **/
uint8_t MacSendStatsDeleteRspToDuApp(SchStatsDeleteRsp *schStatsDeleteRsp,  MacStatsDeleteReq *macStatsDeleteReq)
{
   Pst  pst;
   uint8_t ret = ROK, idx=0;
   MacStatsDeleteRsp *macStatsDeleteRsp=NULLP;

   MAC_ALLOC_SHRABL_BUF(macStatsDeleteRsp, sizeof(MacStatsDeleteRsp));
   if(macStatsDeleteRsp == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : Failed to allocate memory in MacProcSchStatsDeleteRsp");
      ret = RFAILED;
   }
   
   if(schStatsDeleteRsp)
   {
      /* [Step-1] */ 
      macStatsDeleteRsp->subscriptionId = schStatsDeleteRsp->subscriptionId;
      macStatsDeleteRsp->numStatsGroupDeleted = schStatsDeleteRsp->numStatsGroupDeleted;

      if(macStatsDeleteRsp->numStatsGroupDeleted)
      {
         /* [Step-3] */ 
         for(idx =0;idx<macStatsDeleteRsp->numStatsGroupDeleted; idx++)
         {
            if(schStatsDeleteRsp->statsGrpDelInfo[idx].statsGrpDelRsp == RSP_OK)
            {
               macStatsDeleteRsp->statsGrpDelInfo[idx].statsGrpDelRsp =MAC_DU_APP_RSP_OK;
               macStatsDeleteRsp->statsGrpDelInfo[idx].statsGrpDelCause =schStatsDeleteRsp->statsGrpDelInfo[idx].statsGrpDelCause;
            }
            else
            {
               macStatsDeleteRsp->statsGrpDelInfo[idx].statsGrpDelRsp =MAC_DU_APP_RSP_NOK;
               macStatsDeleteRsp->statsGrpDelInfo[idx].statsGrpDelCause =schStatsDeleteRsp->statsGrpDelInfo[idx].statsGrpDelCause;
            }
         }
      }
      else
      {
         /* [Step-4] */ 
         if(schStatsDeleteRsp->subsDelRsp == RSP_OK)
            macStatsDeleteRsp->subsDelRsp = MAC_DU_APP_RSP_OK;
         else
            macStatsDeleteRsp->subsDelRsp = MAC_DU_APP_RSP_NOK;
         macStatsDeleteRsp->subsDelCause = schStatsDeleteRsp->subsDelCause;
      }
   }
   else
   {
      /* [Step-2] */ 
      macStatsDeleteRsp->subscriptionId = macStatsDeleteReq->subscriptionId;
      macStatsDeleteRsp->numStatsGroupDeleted= macStatsDeleteReq->numStatsGroupToBeDeleted;
      
      if(macStatsDeleteRsp->numStatsGroupDeleted)
      {
         /* [Step-3] */ 
         for(idx =0;idx<macStatsDeleteRsp->numStatsGroupDeleted; idx++)
         {
            macStatsDeleteRsp->statsGrpDelInfo[idx].statsGrpDelRsp =MAC_DU_APP_RSP_NOK;
            macStatsDeleteRsp->statsGrpDelInfo[idx].statsGrpDelCause = RESOURCE_UNAVAILABLE;
         }
      }
      else
      {
         /* [Step-4] */ 
         macStatsDeleteRsp->subsDelRsp = MAC_DU_APP_RSP_NOK;
         macStatsDeleteRsp->subsDelCause = RESOURCE_UNAVAILABLE;
      }
   }

   DU_LOG("\nINFO  -->  MAC : MacSendStatsDeleteRspToDuApp: Sending Delete Statistics Response to DU APP");

   memset(&pst, 0, sizeof(Pst));
   FILL_PST_MAC_TO_DUAPP(pst, EVENT_MAC_STATS_DELETE_RSP);
   if(((*macDuStatsDeleteRspOpts[pst.selector])(&pst, macStatsDeleteRsp))!= ROK)
   {
      DU_LOG("\nERROR  -->  MAC : Failed to send statistics delete response to DU APP");
      MAC_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, macStatsDeleteRsp, sizeof(MacStatsDeleteRsp));
      ret = RFAILED;
   }

   return ret;
}
                 
/**
 * @brief Mac process the statistics delete rsp received from sch.
 *
 * @details
 *
 *     Function : MacProcSchStatsDeleteRsp
 *
 *     This function  process the statistics delete response received from sch
 *
 *  @param[in]  Pst           *pst
 *  @param[in]  SchStatsDeleteRsp *schStatsDeleteRsp
 *  @return  int
 *      -# ROK
 **/
uint8_t MacProcSchStatsDeleteRsp(Pst *pst, SchStatsDeleteRsp *schStatsDeleteRsp)
{
   uint8_t ret = RFAILED;

   if(schStatsDeleteRsp)
   {
      ret = MacSendStatsDeleteRspToDuApp(schStatsDeleteRsp, NULLP);
      MAC_FREE(schStatsDeleteRsp, sizeof(SchStatsDeleteRsp));
   }
   return ret;
}

/**
 * @brief Mac process the statistics delete Req received from DUAPP
 *
 * @details
 *
 *     Function : MacProcStatsDeleteReq
 *
 *     Functionality: Process the statistics delete request from duapp
 *  @param[in]  Pst      *pst
 *  @param[in]  StatsDeleteReq *statsReq
 *  @return  int
 *      -# ROK
 **/

uint8_t MacProcStatsDeleteReq(Pst *pst, MacStatsDeleteReq *macStatsDeleteReq)
{
   Pst       schPst;
   uint8_t   ret = RFAILED;
   SchStatsDeleteReq  *schStatsDeleteReq = NULLP;
 
   DU_LOG("\nINFO   -->  MAC : Received Statistics delete Request from DU_APP");

   if(macStatsDeleteReq == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : MacProcStatsDeleteReq(): Received Null pointer");
      return RFAILED;
   }
   
   MAC_ALLOC(schStatsDeleteReq, sizeof(SchStatsDeleteReq));
   if(schStatsDeleteReq == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : MacProcStatsDeleteReq: Failed to allocate memory");
   }
   else
   {
      /* fill all the information in schStatsDeleteReq structure */
      schStatsDeleteReq->subscriptionId = macStatsDeleteReq->subscriptionId;
      schStatsDeleteReq->numStatsGroupToBeDeleted = macStatsDeleteReq->numStatsGroupToBeDeleted;
      memcpy(&schStatsDeleteReq->statsGrpIdToBeDelList, &macStatsDeleteReq->statsGrpIdToBeDelList, schStatsDeleteReq->numStatsGroupToBeDeleted*sizeof(uint8_t)); 
      FILL_PST_MAC_TO_SCH(schPst, EVENT_STATISTICS_DELETE_REQ_TO_SCH);
      ret = SchMessageRouter(&schPst, (void *)schStatsDeleteReq);
   }

   if(ret != ROK)
   {
      ret = MacSendStatsDeleteRspToDuApp(NULLP, macStatsDeleteReq);
   }

   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, macStatsDeleteReq, sizeof(MacStatsDeleteReq));
   return ret;
}

/**
 * @brief Fill and send statistics modification response to DU APP
 *
 * @details
 *
 *     Function : MacSendStatsModificationRspToDuApp 
 *
 *     Fill and send statistics modification response to DU APP
 *
 *  @param[in]  Stats modification Response
 *  @return  int
 *      -# ROK
 **/
uint8_t MacSendStatsModificationRspToDuApp(MacStatsModificationRsp *tmpMacStatsModRsp)
{
   Pst  pst;
   uint8_t ret = ROK;
   MacStatsModificationRsp *macStatsModificationRsp = NULLP;

   DU_LOG("\nINFO  -->  MAC : MacSendStatsModificationRspToDuApp: Sending Statistics Modification Response to DU APP");


   MAC_ALLOC_SHRABL_BUF(macStatsModificationRsp, sizeof(MacStatsModificationRsp));
   if(macStatsModificationRsp == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : Failed to allocate memory in MacProcSchStatsModificationRsp");
      ret = RFAILED;
   }
   else
   {
      memcpy(macStatsModificationRsp, tmpMacStatsModRsp, sizeof(MacStatsModificationRsp));
      memset(tmpMacStatsModRsp, 0, sizeof(MacStatsModificationRsp));

      memset(&pst, 0, sizeof(Pst));
      FILL_PST_MAC_TO_DUAPP(pst, EVENT_MAC_STATISTICS_MODIFY_RSP);
      if(((*macDuStatsModificationRspOpts[pst.selector])(&pst, macStatsModificationRsp))!= ROK)
      {
         DU_LOG("\nERROR  -->  MAC : Failed to send statistics modification response to DU APP");
         MAC_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, macStatsModificationRsp, sizeof(MacStatsModificationRsp));
         ret = RFAILED;
      }
   }

   return ret;
}

/**
 * @brief Mac process the statistics modification rsp received from sch.
 *
 * @details
 *
 *     Function : MacProcSchStatsModificationRsp
 *
 *     This function  process the statistics modification response received from sch
 *     [Step -1] Fetch pointer to statistics response from pending list saved at
 *     MAC during processing statistics request from DU APP
 *     [Step -2] Fill the list of accepted list
 *     [Step -3] Fill the list of rejected list
 *     [Step -4] Send statistics modification response to DU APP
 *
 *  @param[in]  Pst           *pst
 *  @param[in]  SchStatsModificationRsp *schStatsModificationRsp
 *  @return  int
 *      -# ROK
 **/
uint8_t MacProcSchStatsModificationRsp(Pst *pst, SchStatsModificationRsp *schStatsModificationRsp)
{
   uint8_t ret = RFAILED;
   uint8_t idx = 0, accptdIdx = 0, rjctdIdx = 0;
   MacStatsModificationRsp *macStatsModificationRsp = NULLP;

   if(schStatsModificationRsp)
   {
      /* [Step -1] */
      for(idx = 0; idx < macCb.statistics.numPendingStatsRsp; idx++)
      {
         if(macCb.statistics.pendingStatsRsp[idx].subscriptionId == schStatsModificationRsp->subscriptionId)
         {
            macStatsModificationRsp = &macCb.statistics.pendingStatsRsp[idx];
            break;
         }
      }

      if(macStatsModificationRsp == NULLP)
      {
         MAC_FREE(schStatsModificationRsp, sizeof(SchStatsModificationRsp));
         return RFAILED;
      }
      
      /* [Step -2] */
      for(accptdIdx = 0; accptdIdx<schStatsModificationRsp->numGrpAccepted && macStatsModificationRsp->numGrpAccepted<MAX_NUM_STATS_GRP; accptdIdx++)
      {
         macStatsModificationRsp->statsGrpAcceptedList[macStatsModificationRsp->numGrpAccepted++] = schStatsModificationRsp->statsGrpAcceptedList[accptdIdx];
      }

      /* [Step -3] */
      for(rjctdIdx = 0; rjctdIdx < schStatsModificationRsp->numGrpRejected && macStatsModificationRsp->numGrpRejected<MAX_NUM_STATS_GRP; rjctdIdx++)
      {
         macStatsModificationRsp->statsGrpRejectedList[macStatsModificationRsp->numGrpRejected].groupId = \
            schStatsModificationRsp->statsGrpRejectedList[rjctdIdx].groupId;
         macStatsModificationRsp->statsGrpRejectedList[macStatsModificationRsp->numGrpRejected].cause = \
            schStatsModificationRsp->statsGrpRejectedList[rjctdIdx].cause;
         macStatsModificationRsp->numGrpRejected++;
      }

      /* [Step -4] */
      ret = MacSendStatsModificationRspToDuApp(macStatsModificationRsp);
   }
   MAC_FREE(schStatsModificationRsp, sizeof(SchStatsModificationRsp));
   return ret;
}

/*******************************************************************
 *
 * @brief Rejects all statistics modification group requested by DU APP
 *
 * @details
 *
 *    Function : MacRejectAllStatsModification
 *
 *    Functionality: Add all statistics modification group received in statistics
 *       request from DU APP, to Reject-StatsModification-Group-List in statistics
 *       response to DU APP
 *
 * @params[in]  Statistics request from DU APP
 *              Cause of rejection
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacRejectAllStatsModification(MacStatsModificationReq *macStatsModificationReq, CauseOfResult cause)
{
   uint8_t grpIdx = 0;
   MacStatsModificationRsp macStatsModificationRsp;

   memset(&macStatsModificationRsp, 0, sizeof(MacStatsModificationRsp));

   /* fill the subscriptionId and the rejected list in stats modification rsp */
   macStatsModificationRsp.subscriptionId = macStatsModificationReq->subscriptionId;
   for(grpIdx = 0; grpIdx < macStatsModificationReq->numStatsGroup; grpIdx++)
   {
      macStatsModificationRsp.statsGrpRejectedList[grpIdx].groupId = macStatsModificationReq->statsGrpList[grpIdx].groupId;
      macStatsModificationRsp.statsGrpRejectedList[grpIdx].cause = cause;
   }
   macStatsModificationRsp.numGrpRejected = macStatsModificationReq->numStatsGroup;

   return MacSendStatsModificationRspToDuApp(&macStatsModificationRsp);
}

/**
 * @brief Mac process the statistics Modification Req received from DUAPP
 *
 * @details
 *
 *     Function : MacProcStatsModificationReq
 *
 *     This function process the statistics Modification request from duapp:
 *     [Step 1] Basic validation. If fails, all stats group in stats request are
 *     rejected.
 *     [Step 2] If basic validations passed, traverse all stats group and
 *     validate each measurement types in each group.
 *     [Step 3] If any measurement type validation fails in a group, that group
 *     is not configured and it is added to stats-group-rejected-list in
 *     mac-stats-response message.
 *     [Step 4] Even if one group passes all validation, it is sent to SCH in
 *     statistics request. The mac-stats-response message is added to
 *     pending-response list. This will be sent to DU APP after stats response
 *     is received from SCH.
 *     [Step 5] If none of the groups passes all validation, mac-stats-response
 *     is sent to du app with all group as part of stats-group-rejected-list.
 *
 *  @param[in]  Pst      *pst
 *  @param[in]  StatsModificationReq *statsModificationReq
 *  @return  int
 *      -# ROK
 **/
uint8_t MacProcStatsModificationReq(Pst *pst, MacStatsModificationReq *macStatsModificationReq)
{
   Pst           schPst;
   uint8_t       ret = RFAILED;
   bool          measTypeInvalid = false;
   uint8_t       macStatsGrpIdx = 0, macStatsIdx = 0;
   uint8_t       schStatsGrpIdx = 0, schStatsIdx = 0;
   MacStatsGrpInfo          *macStatsGrp = NULLP;
   SchStatsModificationReq  *schStatsModificationReq = NULLP;
   MacStatsModificationRsp  *macStatsModificationRsp = NULLP;

   DU_LOG("\nINFO   -->  MAC : Received Statistics Modification Request from DU_APP");

   if(macStatsModificationReq == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : MacProcStatsModificationReq(): Received Null pointer");
      return RFAILED;
   }

   /* [Step -1] */
   if(macCb.statistics.numPendingStatsRsp >= MAX_PENDING_STATS_RSP)
   {
      DU_LOG("\nERROR  -->  MAC : MacProcStatsModificationReq: Maximum number of statistics response is pending. \
         Cannot process new request.");
      MacRejectAllStatsModification(macStatsModificationReq, RESOURCE_UNAVAILABLE);
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, macStatsModificationReq, sizeof(MacStatsModificationReq));
      return RFAILED;
   }

   MAC_ALLOC(schStatsModificationReq, sizeof(SchStatsModificationReq));
   if(schStatsModificationReq == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : MacProcStatsModificationReq: Failed to allocate memory");
      MacRejectAllStatsModification(macStatsModificationReq, RESOURCE_UNAVAILABLE);
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, macStatsModificationReq, sizeof(MacStatsModificationReq));
      return RFAILED;
   }

   macStatsModificationRsp = &macCb.statistics.pendingStatsRsp[macCb.statistics.numPendingStatsRsp];
   memset(macStatsModificationRsp, 0, sizeof(MacStatsModificationRsp));

   /* [Step 2]  */
   schStatsModificationReq->subscriptionId = macStatsModificationReq->subscriptionId;
   schStatsModificationReq->numStatsGroup = 0;
   for(macStatsGrpIdx = 0; macStatsGrpIdx < macStatsModificationReq->numStatsGroup; macStatsGrpIdx++)
   {
      measTypeInvalid = false;
      schStatsIdx = 0;
      macStatsGrp = &macStatsModificationReq->statsGrpList[macStatsGrpIdx];

      for(macStatsIdx=0; macStatsIdx < macStatsGrp->numStats; macStatsIdx++)
      {
         switch(macStatsGrp->statsList[macStatsIdx])
         {
            case MAC_DL_TOTAL_PRB_USAGE:
               {
                  schStatsModificationReq->statsGrpList[schStatsGrpIdx].statsList[schStatsIdx] = SCH_DL_TOTAL_PRB_USAGE;
                  break;
               }
            case MAC_UL_TOTAL_PRB_USAGE:
               {
                  schStatsModificationReq->statsGrpList[schStatsGrpIdx].statsList[schStatsIdx] = SCH_UL_TOTAL_PRB_USAGE;
                  break;
               }
            default:
               {
                  DU_LOG("\nERROR  -->  MAC : MacProcStatsModificationReq: Invalid measurement type [%d]", \
                     macStatsGrp->statsList[macStatsIdx]);
                  measTypeInvalid = true;
               }
         }

         if(measTypeInvalid)
         {
            memset(&schStatsModificationReq->statsGrpList[schStatsGrpIdx], 0, sizeof(SchStatsGrpInfo));
            break;
         }

         schStatsIdx++;
      }

      if(!measTypeInvalid)
      {
         schStatsModificationReq->statsGrpList[schStatsGrpIdx].groupId = macStatsGrp->groupId;
         schStatsModificationReq->statsGrpList[schStatsGrpIdx].periodicity = macStatsGrp->periodicity;
         schStatsModificationReq->statsGrpList[schStatsGrpIdx].numStats = schStatsIdx;
         schStatsGrpIdx++;
      }
      else
      {
         /* [Step 3] */
         macStatsModificationRsp->statsGrpRejectedList[macStatsModificationRsp->numGrpRejected].groupId = macStatsGrp->groupId;
         macStatsModificationRsp->statsGrpRejectedList[macStatsModificationRsp->numGrpRejected].cause = PARAM_INVALID;
         macStatsModificationRsp->numGrpRejected++;
      }
   }
   schStatsModificationReq->numStatsGroup = schStatsGrpIdx;

   macStatsModificationRsp->subscriptionId = macStatsModificationReq->subscriptionId;
   if(schStatsModificationReq->numStatsGroup)
   {
      /* [Step 4] */
      macCb.statistics.numPendingStatsRsp++;

      FILL_PST_MAC_TO_SCH(schPst, EVENT_STATISTICS_MODIFY_REQ_TO_SCH);
      ret = SchMessageRouter(&schPst, (void *)schStatsModificationReq);
   }
   else
   {
      /* [Step 5] */
      DU_LOG("\nERROR  -->  MAC : MacProcStatsModificationReq: All statistics group found invalid");
      MAC_FREE(schStatsModificationReq, sizeof(SchStatsModificationReq));
      ret = MacSendStatsModificationRspToDuApp(macStatsModificationRsp);
   }

   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, macStatsModificationReq, sizeof(MacStatsModificationReq));
   return ret;
}

/**********************************************************************
  End of file
 **********************************************************************/
