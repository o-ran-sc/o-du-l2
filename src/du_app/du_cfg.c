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

/* This file contains all utility functions */
#include "common_def.h"
#include "du_tmr.h"
#include "legtp.h"
#include "lrg.h"
#include "lkw.x"
#include "lrg.x"
#include "du_app_mac_inf.h"
#include "du_app_rlc_inf.h"
#include "du_e2ap_mgr.h"
#include "du_cfg.h"
#include "du_mgr.h"
#include "du_utils.h"
#include "du_f1ap_conversions.h"
#include "OCTET_STRING.h"
#include "BIT_STRING.h"
#include "odu_common_codec.h"
#include "MIB.h"
#include "SearchSpace.h"
#include "SIB-TypeInfo.h"
#include "SchedulingInfo.h"
#include "SI-SchedulingInfo.h"
#include "ConnEstFailureControl.h"
#include "PLMN-IdentityInfo.h"
#include "PDSCH-TimeDomainResourceAllocation.h"
#include "BCCH-Config.h"
#include "PagingCycle.h"
#include "PCCH-Config.h"
#include "TimeAlignmentTimer.h"
#include "BCCH-DL-SCH-Message.h"
#include "RACH-ConfigGeneric.h"
#include "PUSCH-TimeDomainResourceAllocation.h"
#include "PUCCH-ConfigCommon.h"
#include "SubcarrierSpacing.h"
#include "TDD-UL-DL-Pattern.h"
#include "RACH-ConfigCommon.h"
#include "BWP-DownlinkCommon.h"
#include "BWP-UplinkCommon.h"
#include "TDD-UL-DL-ConfigCommon.h"
#include "du_sys_info_hdl.h"

#ifdef O1_ENABLE
#include "CmInterface.h"
extern StartupConfig g_cfg;
extern NRCellDU cellParams;
#endif

char encBuf[ENC_BUF_MAX_LEN];


/* Filling Slot configuration as :
 * Slot Sym 0 Sym 1 Sym 2 Sym 3 Sym 4 Sym 5 Sym 6 Sym 7 Sym 8 Sym 9 Sym10 Sym11 Sym12 Sym13
 *   0   DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL
 *   1   DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL
 *   2   DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL
     3   DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL
     4   DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL
     5   DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL
     6   DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL
     7   DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    DL    F     UL
     8   UL    UL    UL    UL    UL    UL    UL    UL    UL    UL    UL    UL    UL    UL
     9   UL    UL    UL    UL    UL    UL    UL    UL    UL    UL    UL    UL    UL    UL  
 */

/*******************************************************************
 * @brief Reads the CL Configuration.
 *
 * @details
 *
 *    Function : readMacCfg
 *
 *    Functionality:
 *       - Fills up the cell configuration for CL.
 *       - Calls FillSlotConfig()  
 *
 * @params[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t readMacCfg()
{
   uint8_t idx=0, sliceIdx=0,plmnIdx = 0;
   F1TaiSliceSuppLst *taiSliceSuppLst;

   /* DL carrier configuration */
#ifdef O1_ENABLE
   duCfgParam.macCellCfg.cellId = cellParams.cellLocalId;
   duCfgParam.macCellCfg.carrCfg.dlBw = cellParams.bSChannelBwDL;
   duCfgParam.macCellCfg.carrCfg.dlFreq = convertArfcnToFreqKhz(cellParams.arfcnDL);
#else
   duCfgParam.macCellCfg.cellId = NR_CELL_ID;
   duCfgParam.macCellCfg.carrCfg.dlBw = NR_BANDWIDTH;
   duCfgParam.macCellCfg.carrCfg.dlFreq = convertArfcnToFreqKhz(NR_DL_ARFCN);
#endif
   duCfgParam.macCellCfg.carrCfg.numTxAnt = NUM_TX_ANT;
   /* UL Carrier configuration */
#ifdef O1_ENABLE
   duCfgParam.macCellCfg.carrCfg.ulBw = cellParams.bSChannelBwUL;
   duCfgParam.macCellCfg.carrCfg.ulFreq = convertArfcnToFreqKhz(cellParams.arfcnUL);
#else   
   duCfgParam.macCellCfg.carrCfg.ulBw = NR_BANDWIDTH;
   duCfgParam.macCellCfg.carrCfg.ulFreq =  convertArfcnToFreqKhz(NR_UL_ARFCN);
#endif   
   duCfgParam.macCellCfg.carrCfg.numRxAnt = NUM_RX_ANT;

   /* Cell configuration */
#ifdef O1_ENABLE
   duCfgParam.macCellCfg.cellCfg.opState    = cellParams.operationalState;
   duCfgParam.macCellCfg.cellCfg.adminState = cellParams.administrativeState;
   duCfgParam.macCellCfg.cellCfg.cellState  = cellParams.cellState;
   duCfgParam.macCellCfg.cellCfg.phyCellId  = cellParams.nRPCI;
   duCfgParam.macCellCfg.cellCfg.tac        = cellParams.nRTAC;
   duCfgParam.macCellCfg.cellCfg.ssbFreq    = cellParams.ssbFrequency;
#else
   duCfgParam.macCellCfg.cellCfg.opState    = OP_DISABLED; 
   duCfgParam.macCellCfg.cellCfg.adminState = ADMIN_UNLOCKED;
   duCfgParam.macCellCfg.cellCfg.cellState  = CELL_INACTIVE;
   duCfgParam.macCellCfg.cellCfg.phyCellId  = NR_PCI;
   duCfgParam.macCellCfg.cellCfg.tac        = DU_TAC; 
   duCfgParam.macCellCfg.cellCfg.ssbFreq    = SSB_FREQUENCY;
#endif
   /* Plmn And SNSSAI Configuration */
   for(plmnIdx = 0; plmnIdx < MAX_PLMN; plmnIdx++)
   {
      memcpy(&duCfgParam.macCellCfg.cellCfg.plmnInfoList[plmnIdx].plmn, &duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.srvdPlmn[plmnIdx].plmn,\
            sizeof(Plmn));
      taiSliceSuppLst = &duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.srvdPlmn[plmnIdx].taiSliceSuppLst;
      duCfgParam.macCellCfg.cellCfg.plmnInfoList[plmnIdx].numSupportedSlice = taiSliceSuppLst->numSupportedSlices;
      if(taiSliceSuppLst->snssai)
      {
         DU_ALLOC_SHRABL_BUF(duCfgParam.macCellCfg.cellCfg.plmnInfoList[plmnIdx].snssai, (duCfgParam.macCellCfg.cellCfg.plmnInfoList[plmnIdx].numSupportedSlice) * sizeof(Snssai*));
         if(duCfgParam.macCellCfg.cellCfg.plmnInfoList[plmnIdx].snssai == NULLP)
         {
            DU_LOG("\nERROR  --> DU_APP: Memory allocation failed at readMacCfg");
            return RFAILED;
         }
      }
      for(sliceIdx=0; sliceIdx < taiSliceSuppLst->numSupportedSlices; sliceIdx++)
      {
         if(taiSliceSuppLst->snssai[sliceIdx] != NULLP)
         {
            DU_ALLOC_SHRABL_BUF(duCfgParam.macCellCfg.cellCfg.plmnInfoList[plmnIdx].snssai[sliceIdx], sizeof(Snssai));
            if(duCfgParam.macCellCfg.cellCfg.plmnInfoList[plmnIdx].snssai[sliceIdx] == NULLP)
            {
               DU_LOG("\nERROR  --> DU_APP: Memory allocation failed at readMacCfg");
               return RFAILED;
            }
            memcpy(duCfgParam.macCellCfg.cellCfg.plmnInfoList[plmnIdx].snssai[sliceIdx], taiSliceSuppLst->snssai[sliceIdx], sizeof(Snssai));
         }
      }
   }
   duCfgParam.macCellCfg.cellCfg.subCarrSpacing = NR_SCS;
   duCfgParam.macCellCfg.cellCfg.dupType    = DUPLEX_MODE;

   /* SSB configuration */
   duCfgParam.macCellCfg.ssbCfg.ssbPbchPwr = SSB_PBCH_PWR;
   duCfgParam.macCellCfg.ssbCfg.bchPayloadFlag = BCH_PAYLOAD;
   duCfgParam.macCellCfg.ssbCfg.ssbOffsetPointA = OFFSET_TO_POINT_A;
   duCfgParam.macCellCfg.ssbCfg.betaPss = BETA_PSS;
#ifdef O1_ENABLE
   duCfgParam.macCellCfg.ssbCfg.scsCmn = convertScsValToScsEnum(cellParams.ssbSubCarrierSpacing);
   duCfgParam.macCellCfg.ssbCfg.ssbPeriod = convertSSBPeriodicityToEnum(cellParams.ssbPeriodicity);
   duCfgParam.macCellCfg.ssbCfg.ssbScOffset = cellParams.ssbOffset;
#else
   duCfgParam.macCellCfg.ssbCfg.scsCmn = NR_SCS;
   duCfgParam.macCellCfg.ssbCfg.ssbPeriod = SSB_PRDCTY_MS_20;
   duCfgParam.macCellCfg.ssbCfg.ssbScOffset = SSB_SUBCARRIER_OFFSET;
#endif
   duCfgParam.macCellCfg.ssbCfg.ssbMask[0] = 1; /* only one SSB is transmitted */
   if(BuildMibPdu() != ROK)
   {
      DU_LOG("\nERROR  -->  Failed to build MIB PDU");
      memset(&duCfgParam.macCellCfg.ssbCfg.mibPdu, 0, 3*sizeof(uint8_t));
   }
   else
   {
      memcpy(&duCfgParam.macCellCfg.ssbCfg.mibPdu, encBuf,encBufSize);
   }

   /* PRACH configuration */
   duCfgParam.macCellCfg.prachCfg.prachSeqLen = PRACH_SEQ_LEN;
   duCfgParam.macCellCfg.prachCfg.prachSubcSpacing = convertScsEnumValToScsVal(PRACH_SUBCARRIER_SPACING);
   duCfgParam.macCellCfg.prachCfg.prachCfgIdx = PRACH_CONFIG_IDX;
   duCfgParam.macCellCfg.prachCfg.msg1Fdm = NUM_PRACH_FDM;
   duCfgParam.macCellCfg.prachCfg.fdm[0].rootSeqIdx = ROOT_SEQ_IDX;
   duCfgParam.macCellCfg.prachCfg.fdm[0].numRootSeq = NUM_ROOT_SEQ;
   duCfgParam.macCellCfg.prachCfg.fdm[0].k1 = 0;
   duCfgParam.macCellCfg.prachCfg.fdm[0].zeroCorrZoneCfg = ZERO_CORRELATION_ZONE_CFG;
   duCfgParam.macCellCfg.prachCfg.prachRstSetCfg = PRACH_RESTRICTED_SET_CFG;
   duCfgParam.macCellCfg.prachCfg.ssbPerRach = SSB_PER_RACH;
   duCfgParam.macCellCfg.prachCfg.msg1FreqStart = PRACH_FREQ_START;

   duCfgParam.macCellCfg.prachCfg.totalNumRaPreamble = NUM_RA_PREAMBLE;
   duCfgParam.macCellCfg.prachCfg.numCbPreamblePerSsb = CB_PREAMBLE_PER_SSB;
   duCfgParam.macCellCfg.prachCfg.raRspWindow = RA_RSP_WINDOW;
   
   /* TDD configuration */
#ifdef NR_TDD   
   duCfgParam.macCellCfg.tddCfg.tddPeriod = TDD_PERIODICITY;
   duCfgParam.macCellCfg.tddCfg.nrOfDlSlots = NUM_DL_SLOTS;
   duCfgParam.macCellCfg.tddCfg.nrOfDlSymbols = NUM_DL_SYMBOLS;
   duCfgParam.macCellCfg.tddCfg.nrOfUlSlots = NUM_UL_SLOTS;
   duCfgParam.macCellCfg.tddCfg.nrOfUlSymbols = NUM_UL_SYMBOLS;

   //FillSlotConfig();

#endif

   /* fill SIB1 configuration */
   duCfgParam.macCellCfg.cellCfg.sib1Cfg.sib1PduLen = duCfgParam.srvdCellLst[0].duSysInfo.sib1Len;
   DU_ALLOC_SHRABL_BUF(duCfgParam.macCellCfg.cellCfg.sib1Cfg.sib1Pdu,duCfgParam.srvdCellLst[0].duSysInfo.sib1Len);
   memcpy(duCfgParam.macCellCfg.cellCfg.sib1Cfg.sib1Pdu, duCfgParam.srvdCellLst[0].duSysInfo.sib1Msg, \
         duCfgParam.srvdCellLst[0].duSysInfo.sib1Len);
   duCfgParam.macCellCfg.cellCfg.sib1Cfg.pdcchCfgSib1.coresetZeroIndex = CORESET_0_INDEX;
   duCfgParam.macCellCfg.cellCfg.sib1Cfg.pdcchCfgSib1.searchSpaceZeroIndex = SEARCHSPACE_0_INDEX;

   duCfgParam.macCellCfg.cellCfg.sib1Cfg.pagingCfg.numPO = duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg.pcchCfg.ns;
   if((duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg.pcchCfg.firstPDCCHMontioringType != \
            PCCH_Config__firstPDCCH_MonitoringOccasionOfPO_PR_NOTHING) && (duCfgParam.macCellCfg.cellCfg.sib1Cfg.pagingCfg.numPO != 0))
   {
      duCfgParam.macCellCfg.cellCfg.sib1Cfg.pagingCfg.poPresent = TRUE;
      memcpy(duCfgParam.macCellCfg.cellCfg.sib1Cfg.pagingCfg.pagingOcc, 
            duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg.pcchCfg.firstPDCCHMontioringInfo,MAX_PO_PER_PF);
   }
   else
   {
      duCfgParam.macCellCfg.cellCfg.sib1Cfg.pagingCfg.poPresent = FALSE;
   }

   /* fill Intial DL BWP */
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.bwp.firstPrb = 0;
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.bwp.numPrb = TOTAL_PRB_20MHZ_MU0; /* configured to total BW */
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.bwp.scs = duCfgParam.macCellCfg.ssbCfg.scsCmn;
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.bwp.cyclicPrefix = NORMAL_CYCLIC_PREFIX;
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.searchSpaceId = SEARCHSPACE_1_INDEX;
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.coresetId = CORESET_0_INDEX;
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.monitoringSlot =
      SS_MONITORING_SLOT_SL1; /* sl1 - all slots */
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.duration = 0;
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.monitoringSymbol =
      SS_MONITORING_SYMBOL;
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.
      candidate.aggLevel1	= 8;
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.
      candidate.aggLevel2	= 4;
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.
      candidate.aggLevel4	= 2;
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.
      candidate.aggLevel8	= 1;
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.
      candidate.aggLevel16	= 0;

   duCfgParam.macCellCfg.cellCfg.initialDlBwp.pdschCommon.numTimeDomAlloc = NUM_TIME_DOM_RSRC_ALLOC;
   idx = 0;
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.pdschCommon.timeDomRsrcAllocList[idx].k0 = PDSCH_K0_CFG1;
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.pdschCommon.timeDomRsrcAllocList[idx].mappingType = 
      PDSCH_MAPPING_TYPE_A;
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.pdschCommon.timeDomRsrcAllocList[idx].startSymbol = 
      PDSCH_START_SYMBOL;
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.pdschCommon.timeDomRsrcAllocList[idx].lengthSymbol =
      PDSCH_LENGTH_SYMBOL;

   idx++;
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.pdschCommon.timeDomRsrcAllocList[idx].k0 = PDSCH_K0_CFG2;
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.pdschCommon.timeDomRsrcAllocList[idx].mappingType = 
      PDSCH_MAPPING_TYPE_A;
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.pdschCommon.timeDomRsrcAllocList[idx].startSymbol = 
      PDSCH_START_SYMBOL;
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.pdschCommon.timeDomRsrcAllocList[idx].lengthSymbol =
      PDSCH_LENGTH_SYMBOL;

   /* ra-searchSpace ID is set to 1 */
   duCfgParam.macCellCfg.cellCfg.initialDlBwp.pdcchCommon.raSearchSpaceId = SEARCHSPACE_1_INDEX;

   /* fill Intial UL BWP */
   duCfgParam.macCellCfg.cellCfg.initialUlBwp.bwp.firstPrb = 0;
   duCfgParam.macCellCfg.cellCfg.initialUlBwp.bwp.numPrb = TOTAL_PRB_20MHZ_MU0; /* configured to total BW */
   duCfgParam.macCellCfg.cellCfg.initialUlBwp.bwp.scs = duCfgParam.macCellCfg.ssbCfg.scsCmn;
   duCfgParam.macCellCfg.cellCfg.initialUlBwp.bwp.cyclicPrefix = NORMAL_CYCLIC_PREFIX;
   duCfgParam.macCellCfg.cellCfg.initialUlBwp.puschCommon.numTimeDomRsrcAlloc = 2;
   duCfgParam.macCellCfg.cellCfg.initialUlBwp.puschCommon.timeDomRsrcAllocList[0].k2 = PUSCH_K2_CFG1;
   duCfgParam.macCellCfg.cellCfg.initialUlBwp.puschCommon.timeDomRsrcAllocList[0].mappingType = 
      PUSCH_MAPPING_TYPE_A;
   duCfgParam.macCellCfg.cellCfg.initialUlBwp.puschCommon.timeDomRsrcAllocList[0].startSymbol = 
      PUSCH_START_SYMBOL;
   duCfgParam.macCellCfg.cellCfg.initialUlBwp.puschCommon.timeDomRsrcAllocList[0].symbolLength =
      PUSCH_LENGTH_SYMBOL;

   duCfgParam.macCellCfg.cellCfg.initialUlBwp.puschCommon.timeDomRsrcAllocList[1].k2 = PUSCH_K2_CFG2;
   duCfgParam.macCellCfg.cellCfg.initialUlBwp.puschCommon.timeDomRsrcAllocList[1].mappingType = 
      PUSCH_MAPPING_TYPE_A;
   duCfgParam.macCellCfg.cellCfg.initialUlBwp.puschCommon.timeDomRsrcAllocList[1].startSymbol = 
      PUSCH_START_SYMBOL;
   duCfgParam.macCellCfg.cellCfg.initialUlBwp.puschCommon.timeDomRsrcAllocList[1].symbolLength =
      PUSCH_LENGTH_SYMBOL;

   duCfgParam.macCellCfg.ssbCfg.dmrsTypeAPos = DMRS_TYPE_A_POS; 

   /* fill PUCCH config common */
   duCfgParam.macCellCfg.cellCfg.initialUlBwp.pucchCommon.pucchResourceCommon = PUCCH_RSRC_COMMON;
   duCfgParam.macCellCfg.cellCfg.initialUlBwp.pucchCommon.pucchGroupHopping = PUCCH_GROUP_HOPPING;


#ifndef O1_ENABLE

   /*Note: Static Configuration, when O1 is not configuring the RRM policy*/
   RrmPolicyList rrmPolicy[NUM_OF_SUPPORTED_SLICE];
   rrmPolicy[0].id[0] = 1;
   rrmPolicy[0].resourceType = PRB;
   rrmPolicy[0].rRMMemberNum = 1;
   memcpy(rrmPolicy[0].rRMPolicyMemberList[0].mcc,duCfgParam.macCellCfg.cellCfg.plmnInfoList[0].plmn.mcc, 3*sizeof(uint8_t));
   memcpy(rrmPolicy[0].rRMPolicyMemberList[0].mnc,duCfgParam.macCellCfg.cellCfg.plmnInfoList[0].plmn.mnc, 3*sizeof(uint8_t));
   rrmPolicy[0].rRMPolicyMemberList[0].sst = 1;
   rrmPolicy[0].rRMPolicyMemberList[0].sd[0] = 2;
   rrmPolicy[0].rRMPolicyMemberList[0].sd[1] = 3;
   rrmPolicy[0].rRMPolicyMemberList[0].sd[2] = 4;
   rrmPolicy[0].rRMPolicyMaxRatio = 100;
   rrmPolicy[0].rRMPolicyMinRatio = 50;
   rrmPolicy[0].rRMPolicyDedicatedRatio = 10;

   rrmPolicy[1].id[0] = 2;
   rrmPolicy[1].resourceType = PRB;
   rrmPolicy[1].rRMMemberNum = 1;
   memcpy(rrmPolicy[1].rRMPolicyMemberList[0].mcc,duCfgParam.macCellCfg.cellCfg.plmnInfoList[0].plmn.mcc, 3*sizeof(uint8_t));
   memcpy(rrmPolicy[1].rRMPolicyMemberList[0].mnc,duCfgParam.macCellCfg.cellCfg.plmnInfoList[0].plmn.mnc, 3*sizeof(uint8_t));
   rrmPolicy[1].rRMPolicyMemberList[0].sst = 2;
   rrmPolicy[1].rRMPolicyMemberList[0].sd[0] = 3;
   rrmPolicy[1].rRMPolicyMemberList[0].sd[1] = 3;
   rrmPolicy[1].rRMPolicyMemberList[0].sd[2] = 4;
   rrmPolicy[1].rRMPolicyMaxRatio = 100;
   rrmPolicy[1].rRMPolicyMinRatio = 50;
   rrmPolicy[1].rRMPolicyDedicatedRatio = 10;

   // rrmPolicy[2].id[0] = 3;
   // rrmPolicy[2].resourceType = PRB;
   // rrmPolicy[2].rRMMemberNum = 1;
   // memcpy(rrmPolicy[2].rRMPolicyMemberList[0].mcc,duCfgParam.macCellCfg.cellCfg.plmnInfoList[0].plmn.mcc, 3*sizeof(uint8_t));
   // memcpy(rrmPolicy[2].rRMPolicyMemberList[0].mnc,duCfgParam.macCellCfg.cellCfg.plmnInfoList[0].plmn.mnc, 3*sizeof(uint8_t));
   // rrmPolicy[2].rRMPolicyMemberList[0].sst = 3;
   // rrmPolicy[2].rRMPolicyMemberList[0].sd[0] = 4;
   // rrmPolicy[2].rRMPolicyMemberList[0].sd[1] = 3;
   // rrmPolicy[2].rRMPolicyMemberList[0].sd[2] = 4;
   // rrmPolicy[2].rRMPolicyMaxRatio = 100;
   // rrmPolicy[2].rRMPolicyMinRatio = 33;
   // rrmPolicy[2].rRMPolicyDedicatedRatio = 10;

   // rrmPolicy[3].id[0] = 4;
   // rrmPolicy[3].resourceType = PRB;
   // rrmPolicy[3].rRMMemberNum = 1;
   // memcpy(rrmPolicy[3].rRMPolicyMemberList[0].mcc,duCfgParam.macCellCfg.cellCfg.plmnInfoList[0].plmn.mcc, 3*sizeof(uint8_t));
   // memcpy(rrmPolicy[3].rRMPolicyMemberList[0].mnc,duCfgParam.macCellCfg.cellCfg.plmnInfoList[0].plmn.mnc, 3*sizeof(uint8_t));
   // rrmPolicy[3].rRMPolicyMemberList[0].sst = 3;
   // rrmPolicy[3].rRMPolicyMemberList[0].sd[0] = 4;
   // rrmPolicy[3].rRMPolicyMemberList[0].sd[1] = 3;
   // rrmPolicy[3].rRMPolicyMemberList[0].sd[2] = 4;
   // rrmPolicy[3].rRMPolicyMaxRatio = 100;
   // rrmPolicy[3].rRMPolicyMinRatio = 25;
   // rrmPolicy[3].rRMPolicyDedicatedRatio = 10;

   cpyRrmPolicyInDuCfgParams(rrmPolicy, NUM_OF_SUPPORTED_SLICE, &duCfgParam.tempSliceCfg);

#endif

   return ROK;
}

/*******************************************************************
 *
 * @brief Configures the DU Parameters
 *
 * @details
 *
 *    Function : fillDuPort
 *
 *    Functionality:
 *       - fills the DU Ports.  
 *
 * @params[in] duPort array to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillDuPort(uint16_t *duPort)
{

#ifdef O1_ENABLE
   duPort[F1_INTERFACE]   = g_cfg.DU_Port;
   duPort[E2_INTERFACE]   = g_cfg.RIC_Port;
#else
   duPort[F1_INTERFACE]   = F1_SCTP_PORT;     /* DU Port idx  0 38472 */
   duPort[E2_INTERFACE]   = E2_SCTP_PORT;    /* RIC Port idx 1 36421 */
#endif
   return ROK;
}

/*******************************************************************
 *
 * @brief Configures the DU Parameters
 *
 * @details
 *
 *    Function : calcSliv
 *
 *    Functionality:
 *       - calculate SLIV value from start and length field
 *
 * @params[in] start symbol
 * @params[in] length of symbols
 * @return SLIV value
 *
 * ****************************************************************/
uint16_t calcSliv(uint8_t startSymbol, uint8_t lengthSymbol)
{
   uint16_t sliv = 0;
   if((lengthSymbol-1) <= 7)
   {
      sliv = NUM_SYMBOLS_PER_SLOT * (lengthSymbol-1) + startSymbol;
   }
   else
   {
      sliv = NUM_SYMBOLS_PER_SLOT * (NUM_SYMBOLS_PER_SLOT - lengthSymbol + 1) \
	     + (NUM_SYMBOLS_PER_SLOT - 1 - startSymbol);
   }
   return sliv;
}


/*******************************************************************
 *
 * @brief Configures serving cell config common in sib1
 *
 * @details
 *
 *    Function : fillServCellCfgCommSib
 *
 *    Functionality:
 *       - fills Serving cell config common for SIB1
 *
 * @params[in] SrvCellCfgCommSib pointer
 * @return ROK     - success
 *         RFAILED - failure
 * 
 ** ****************************************************************/
uint8_t fillServCellCfgCommSib(SrvCellCfgCommSib *srvCellCfgComm)
{
   PdcchCfgCommon   pdcchCfg;
   PdschCfgCommon   pdschCfg;
   PcchCfg          pcchCfg;
   RachCfgCommon    rachCfg;
   PuschCfgCommon   puschCfg;
   PucchCfgCommon   pucchCfg;
   TddUlDlCfgCommon   tddCfg;

#ifdef O1_ENABLE
   srvCellCfgComm->scs = convertScsValToScsEnum(cellParams.ssbSubCarrierSpacing);
#else
   srvCellCfgComm->scs = NR_SCS;
#endif

   /* Configuring DL Config Common for SIB1*/
   srvCellCfgComm->dlCfg.freqBandInd = NR_FREQ_BAND; 
   srvCellCfgComm->dlCfg.offsetToPointA = OFFSET_TO_POINT_A;
#ifdef O1_ENABLE
   srvCellCfgComm->dlCfg.dlScsCarrier.scsOffset =  cellParams.ssbOffset;
   srvCellCfgComm->dlCfg.dlScsCarrier.scs = convertScsValToScsEnum(cellParams.ssbSubCarrierSpacing);
   srvCellCfgComm->dlCfg.dlScsCarrier.scsBw =  cellParams.bSChannelBwUL;
#else
   srvCellCfgComm->dlCfg.dlScsCarrier.scsOffset = SSB_SUBCARRIER_OFFSET;
   srvCellCfgComm->dlCfg.dlScsCarrier.scs = NR_SCS;
   srvCellCfgComm->dlCfg.dlScsCarrier.scsBw = NR_BANDWIDTH;
#endif   
   srvCellCfgComm->dlCfg.locAndBw = FREQ_LOC_BW;

   /* Configuring PDCCH Config Common For SIB1 */
   pdcchCfg.present = BWP_DownlinkCommon__pdcch_ConfigCommon_PR_setup;
   pdcchCfg.ctrlRsrcSetZero = CORESET_0_INDEX;
   pdcchCfg.searchSpcZero = SEARCHSPACE_0_INDEX;
   pdcchCfg.searchSpcId = PDCCH_SEARCH_SPACE_ID;
   pdcchCfg.ctrlRsrcSetId = PDCCH_CTRL_RSRC_SET_ID;
   pdcchCfg.monitorSlotPrdAndOffPresent = SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl1;
   //pdcchCfg.monitorSlotPrdAndOff = \
   SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl1;
   pdcchCfg.monitorSymbolsInSlot[0] = 128;
   pdcchCfg.monitorSymbolsInSlot[1] = 0;
   pdcchCfg.numCandAggLvl1 = SearchSpace__nrofCandidates__aggregationLevel1_n8;
   pdcchCfg.numCandAggLvl2 = SearchSpace__nrofCandidates__aggregationLevel2_n4;
   pdcchCfg.numCandAggLvl4 = SearchSpace__nrofCandidates__aggregationLevel4_n2;
   pdcchCfg.numCandAggLvl8 = SearchSpace__nrofCandidates__aggregationLevel8_n1;
   pdcchCfg.numCandAggLvl16 = SearchSpace__nrofCandidates__aggregationLevel16_n0;
   pdcchCfg.searchSpcType = SearchSpace__searchSpaceType_PR_common;
   pdcchCfg.commSrchSpcDciFrmt = PDCCH_SERACH_SPACE_DCI_FORMAT;
   pdcchCfg.searchSpcSib1 = PDCCH_SEARCH_SPACE_ID_SIB1;
   pdcchCfg.pagingSearchSpc = PDCCH_SEARCH_SPACE_ID_PAGING;
   pdcchCfg.raSearchSpc = PDCCH_SEARCH_SPACE_ID_PAGING;
   srvCellCfgComm->dlCfg.pdcchCfg = pdcchCfg;

   /* Configuring PDSCH Config Common For SIB1 */
   pdschCfg.present = BWP_DownlinkCommon__pdsch_ConfigCommon_PR_setup;
   pdschCfg.numTimeDomRsrcAlloc = 2;
   pdschCfg.timeDomAlloc[0].k0 = PDSCH_K0_CFG1;
   pdschCfg.timeDomAlloc[0].mapType = PDSCH_TimeDomainResourceAllocation__mappingType_typeA;
   pdschCfg.timeDomAlloc[0].sliv = calcSliv(PDSCH_START_SYMBOL,PDSCH_LENGTH_SYMBOL);

   pdschCfg.timeDomAlloc[1].k0 = PDSCH_K0_CFG2;
   pdschCfg.timeDomAlloc[1].mapType = PDSCH_TimeDomainResourceAllocation__mappingType_typeA;
   pdschCfg.timeDomAlloc[1].sliv = calcSliv(PDSCH_START_SYMBOL,PDSCH_LENGTH_SYMBOL);

   srvCellCfgComm->dlCfg.pdschCfg = pdschCfg;

   /* Configuring BCCH Config for SIB1 */
   srvCellCfgComm->dlCfg.bcchCfg.modPrdCoeff = BCCH_Config__modificationPeriodCoeff_n16;

   /* Configuring PCCH Config for SIB1 */
   pcchCfg.dfltPagingCycle = convertPagingCycleEnumToValue(PagingCycle_rf256);
   pcchCfg.nAndPagingFrmOffsetType = PCCH_Config__nAndPagingFrameOffset_PR_oneT;
   pcchCfg.pageFrameOffset = 0;
   pcchCfg.ns = convertNsEnumToValue(PCCH_Config__ns_one);
   pcchCfg.firstPDCCHMontioringType = PCCH_Config__firstPDCCH_MonitoringOccasionOfPO_PR_sCS30KHZoneT_SCS15KHZhalfT;
   memset(pcchCfg.firstPDCCHMontioringInfo, 0, sizeof(uint16_t));
   pcchCfg.firstPDCCHMontioringInfo[0] = 44;
   
   srvCellCfgComm->dlCfg.pcchCfg = pcchCfg;


   /* Configuring UL Config Common */
#ifdef O1_ENABLE
   srvCellCfgComm->ulCfg.ulScsCarrier.scsOffset =  cellParams.ssbOffset;
   srvCellCfgComm->ulCfg.ulScsCarrier.scs = convertScsValToScsEnum(cellParams.ssbSubCarrierSpacing);
   srvCellCfgComm->ulCfg.ulScsCarrier.scsBw = cellParams.bSChannelBwUL; 
#else
   srvCellCfgComm->ulCfg.ulScsCarrier.scsOffset = SSB_SUBCARRIER_OFFSET;
   srvCellCfgComm->ulCfg.ulScsCarrier.scs = NR_SCS;
   srvCellCfgComm->ulCfg.ulScsCarrier.scsBw = NR_BANDWIDTH;
#endif   
   srvCellCfgComm->ulCfg.freqBandInd = NR_FREQ_BAND;
   srvCellCfgComm->ulCfg.pMax = UL_P_MAX;
   srvCellCfgComm->ulCfg.locAndBw = FREQ_LOC_BW;
   srvCellCfgComm->ulCfg.timeAlignTimerComm = TimeAlignmentTimer_infinity;

   /* Configuring RACH Config Common for SIB1 */
   rachCfg.present = BWP_UplinkCommon__rach_ConfigCommon_PR_setup;
   rachCfg.prachCfgIdx = PRACH_CONFIG_IDX;
   rachCfg.msg1Fdm = RACH_ConfigGeneric__msg1_FDM_one;
   rachCfg.msg1FreqStart = PRACH_FREQ_START;
   rachCfg.zeroCorrZoneCfg = ZERO_CORRELATION_ZONE_CFG;
   rachCfg.preambleRcvdTgtPwr = PRACH_PREAMBLE_RCVD_TGT_PWR;
   rachCfg.preambleTransMax = RACH_ConfigGeneric__preambleTransMax_n200;
   rachCfg.pwrRampingStep = RACH_ConfigGeneric__powerRampingStep_dB2;
   rachCfg.raRspWindow = RACH_ConfigGeneric__ra_ResponseWindow_sl10;
   rachCfg.numRaPreamble = NUM_RA_PREAMBLE;
   rachCfg.numSsbPerRachOcc = RACH_ConfigCommon__ssb_perRACH_OccasionAndCB_PreamblesPerSSB_PR_one;
   rachCfg.numCbPreamblePerSsb = CB_PREAMBLE_PER_SSB;
   rachCfg.contResTimer = RACH_ConfigCommon__ra_ContentionResolutionTimer_sf64;
   rachCfg.rsrpThreshSsb = RSRP_THRESHOLD_SSB;
   rachCfg.rootSeqIdxPresent = RACH_ConfigCommon__prach_RootSequenceIndex_PR_l139;
   rachCfg.rootSeqIdx = ROOT_SEQ_IDX;
   rachCfg.msg1Scs = PRACH_SUBCARRIER_SPACING ;
   rachCfg.restrictedSetCfg = PRACH_RESTRICTED_SET_CFG;
   srvCellCfgComm->ulCfg.rachCfg = rachCfg;

   /* Configuring PUSCH Config Common for SIB1 */
   puschCfg.puschCfgPresent = BWP_UplinkCommon__pusch_ConfigCommon_PR_setup;
   puschCfg.numTimeDomRsrcAlloc = 2;
   puschCfg.timeDomAllocList[0].k2 = PUSCH_K2_CFG1;
   puschCfg.timeDomAllocList[0].mapType = PUSCH_TimeDomainResourceAllocation__mappingType_typeA;
   puschCfg.timeDomAllocList[0].sliv = calcSliv(PUSCH_START_SYMBOL,PUSCH_LENGTH_SYMBOL);
   puschCfg.timeDomAllocList[1].k2 = PUSCH_K2_CFG2;
   puschCfg.timeDomAllocList[1].mapType = PUSCH_TimeDomainResourceAllocation__mappingType_typeA;
   puschCfg.timeDomAllocList[1].sliv = calcSliv(PUSCH_START_SYMBOL,PUSCH_LENGTH_SYMBOL);
   puschCfg.msg3DeltaPreamble = PUSCH_MSG3_DELTA_PREAMBLE;
   puschCfg.p0NominalWithGrant = PUSCH_P0_NOMINAL_WITH_GRANT;
   srvCellCfgComm->ulCfg.puschCfg = puschCfg;

   /* Configuring PUCCH Config Common for SIB1 */
   pucchCfg.present = BWP_UplinkCommon__pucch_ConfigCommon_PR_setup;
   pucchCfg.rsrcComm = PUCCH_RSRC_COMMON;
   pucchCfg.grpHop = PUCCH_ConfigCommon__pucch_GroupHopping_neither;
   pucchCfg.p0Nominal = PUCCH_P0_NOMINAL;
   srvCellCfgComm->ulCfg.pucchCfg = pucchCfg;

   /* Configuring TDD UL DL config common */
   tddCfg.refScs = SubcarrierSpacing_kHz30;
   tddCfg.txPrd = TDD_UL_DL_Pattern__dl_UL_TransmissionPeriodicity_ms5;
   tddCfg.numDlSlots = NUM_DL_SLOTS;
   tddCfg.numDlSymbols = NUM_DL_SYMBOLS;
   tddCfg.numUlSlots = NUM_UL_SLOTS;
   tddCfg.numUlSymbols = NUM_UL_SYMBOLS;
   srvCellCfgComm->tddCfg = tddCfg;

   srvCellCfgComm->ssbPosInBurst = 192;
   srvCellCfgComm->ssbPrdServingCell = SSB_PERIODICITY;
   srvCellCfgComm->ssPbchBlockPwr = SSB_PBCH_PWR;

   return ROK;
}

/*******************************************************************
 *
 * @brief Configures the DU Parameters
 *
 * @details
 *
 *    Function : readCfg
 *
 *    Functionality:
 *       - Initializes the DuCfg members.  
 *       - Calls readMacCfg()  
 *
 * @params[in] system task ID
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t readCfg()
{
   CmLList *node;
   uint8_t srvdCellIdx, bandIdx, sliceIdx, plmnIdx, ranFuncIdx, eventTriggerStyleIdx, reportStyleIdx, tnlAssocIdx;
   uint8_t brdcstPlmnIdx, freqBandIdx, srvdPlmnIdx;
   uint32_t ipv4_du, ipv4_cu, ipv4_ric;
   MibParams mib;
   Sib1Params sib1;
   F1TaiSliceSuppLst *taiSliceSuppLst;
   uint8_t measurementInfoIdx =0, measurementInfoLen=0;
   char shortName[] = SHORT_NAME;
   char serviceModelOID[]= SERVICE_MODEL_OID;
   char  description[] = DESCRIPTION;
   char event_trigger_style_name[]= EVENT_TRIGGER_STYLE_NAME;
   char ric_report_style_name[]= REPORT_STYLE_NAME;

#ifndef O1_ENABLE
   /* Note: Added these below variable for local testing*/
   Snssai snssai[NUM_OF_SUPPORTED_SLICE] = {{1,{2,3,4}},{2,{3,3,4}},{3,{4,3,4}},{4,{5,3,4}}};
#endif

   /* Gnb Id */
   duCb.gnbId = GNB_ID;
   
#ifdef O1_ENABLE
   if( getStartupConfig(&g_cfg) != ROK )
   {
      RETVALUE(RFAILED);
   }
   cmInetAddr((S8*)g_cfg.DU_IPV4_Addr, &ipv4_du);
   cmInetAddr((S8*)g_cfg.CU_IPV4_Addr, &ipv4_cu);
   cmInetAddr((S8*)g_cfg.RIC_IPV4_Addr, &ipv4_ric);

   duCfgParam.sctpParams.cuPort = g_cfg.CU_Port;
   duCfgParam.sctpParams.ricPort = g_cfg.RIC_Port;
#else   
   cmInetAddr((S8*)DU_IP_V4_ADDR, &ipv4_du);
   cmInetAddr((S8*)CU_IP_V4_ADDR, &ipv4_cu);
   cmInetAddr((S8*)RIC_IP_V4_ADDR, &ipv4_ric);

   duCfgParam.sctpParams.cuPort = F1_SCTP_PORT;
   duCfgParam.sctpParams.ricPort = E2_SCTP_PORT;
#endif

   fillDuPort(duCfgParam.sctpParams.duPort);
   
   /* F1 DU IP Address and Port*/
   duCfgParam.sctpParams.duIpAddr.ipV4Addr = ipv4_du;

   /* F1 CU IP Address and Port*/
   duCfgParam.sctpParams.cuIpAddr.ipV4Addr = ipv4_cu;

   /* Fill RIC Params */
   duCfgParam.sctpParams.ricIpAddr.ipV4Addr = ipv4_ric;

   /* EGTP Parameters */
   duCfgParam.egtpParams.localIp.ipV4Pres = TRUE;
   duCfgParam.egtpParams.localIp.ipV4Addr = ipv4_du;
   duCfgParam.egtpParams.localPort = F1_EGTP_PORT;
   duCfgParam.egtpParams.destIp.ipV4Pres = TRUE;
   duCfgParam.egtpParams.destIp.ipV4Addr = ipv4_cu;
   duCfgParam.egtpParams.destPort = F1_EGTP_PORT;
   duCfgParam.egtpParams.minTunnelId = MIN_TEID;
   duCfgParam.egtpParams.maxTunnelId = MAX_TEID;

   duCfgParam.maxUe = 32; //TODO: Check
   
   /* DU Info */
   duCfgParam.duId = DU_ID;
   DU_ALLOC(duCfgParam.duName, sizeof(DU_NAME));
   if(!duCfgParam.duName)
   {
      DU_LOG("\nDEBUG --> DU_APP: readCfg(): Memory allocation failure for DU name");   
      return RFAILED;
   }
   strcpy((char*)duCfgParam.duName,DU_NAME);

   memset(&duCb.e2apDb, 0, sizeof(E2apDb));
   duCb.e2apDb.e2NodeId =  DU_ID;
   duCb.e2apDb.e2TransInfo.transIdCounter = 0;

   duCb.e2apDb.numOfTNLAssoc = 1;
   for(tnlAssocIdx =0; tnlAssocIdx<duCb.e2apDb.numOfTNLAssoc; tnlAssocIdx++)
   {
      duCb.e2apDb.tnlAssoc[tnlAssocIdx].localIpAddress.ipV4Pres = duCfgParam.sctpParams.duIpAddr.ipV4Pres;
      duCb.e2apDb.tnlAssoc[tnlAssocIdx].localIpAddress.ipV4Addr = duCfgParam.sctpParams.duIpAddr.ipV4Addr;
      duCb.e2apDb.tnlAssoc[tnlAssocIdx].localPort = duCfgParam.sctpParams.duPort[E2_INTERFACE];
      duCb.e2apDb.tnlAssoc[tnlAssocIdx].destIpAddress.ipV4Pres = duCfgParam.sctpParams.ricIpAddr.ipV4Pres;
      duCb.e2apDb.tnlAssoc[tnlAssocIdx].destIpAddress.ipV4Addr = duCfgParam.sctpParams.ricIpAddr.ipV4Addr;
      duCb.e2apDb.tnlAssoc[tnlAssocIdx].destPort = duCfgParam.sctpParams.ricPort;
      duCb.e2apDb.tnlAssoc[tnlAssocIdx].usage = BOTH_FUNCTIONALITY;
   }
   duCb.e2apDb.numOfRanFunction = 1;
   for(ranFuncIdx =0; ranFuncIdx<duCb.e2apDb.numOfRanFunction; ranFuncIdx++)
   {
      duCb.e2apDb.ranFunction[ranFuncIdx].id = ranFuncIdx + 1;
      memcpy(&duCb.e2apDb.ranFunction[ranFuncIdx].name.shortName, shortName, sizeof(shortName));
      memcpy(&duCb.e2apDb.ranFunction[ranFuncIdx].name.serviceModelOID, serviceModelOID, sizeof(serviceModelOID));
      memcpy(&duCb.e2apDb.ranFunction[ranFuncIdx].name.description, description, sizeof(description));
      duCb.e2apDb.ranFunction[ranFuncIdx].revisionCounter = 0;
      
      duCb.e2apDb.ranFunction[ranFuncIdx].numOfEventTriggerStyleSupported  = NUM_OF_EVENT_TRIGGER_STYLE_SUPPORTED;
      for(eventTriggerStyleIdx=0; eventTriggerStyleIdx<duCb.e2apDb.ranFunction[ranFuncIdx].numOfEventTriggerStyleSupported; eventTriggerStyleIdx++)
      {
         duCb.e2apDb.ranFunction[ranFuncIdx].eventTriggerStyleList[eventTriggerStyleIdx].styleType = EVENT_TRIGGER_STYLE_TYPE;
         duCb.e2apDb.ranFunction[ranFuncIdx].eventTriggerStyleList[eventTriggerStyleIdx].formatType = EVENT_TRIGGER_STYLE_FORMAT_TYPE;
         memcpy(duCb.e2apDb.ranFunction[ranFuncIdx].eventTriggerStyleList[eventTriggerStyleIdx].name, event_trigger_style_name, sizeof(event_trigger_style_name));
      }
      
      duCb.e2apDb.ranFunction[ranFuncIdx].numOfReportStyleSupported= NUM_OF_RIC_REPORT_STYLE_SUPPORTED;
      for(reportStyleIdx=0; reportStyleIdx<duCb.e2apDb.ranFunction[ranFuncIdx].numOfReportStyleSupported; reportStyleIdx++)
      {
         duCb.e2apDb.ranFunction[ranFuncIdx].reportStyleList[reportStyleIdx].reportStyle.styleType = REPORT_STYLE_TYPE;
         duCb.e2apDb.ranFunction[ranFuncIdx].reportStyleList[reportStyleIdx].reportStyle.formatType = REPORT_ACTION_FORMAT_TYPE;
         memcpy(duCb.e2apDb.ranFunction[ranFuncIdx].reportStyleList[reportStyleIdx].reportStyle.name, ric_report_style_name, sizeof(ric_report_style_name));
         
         for(measurementInfoIdx =0; measurementInfoIdx<NUM_OF_MEASUREMENT_INFO_SUPPORTED; measurementInfoIdx++)
         {
            measurementInfoLen= strlen(MEASUREMENT_TYPE_NAME[measurementInfoIdx]);
            MeasurementInfoForAction *measurementInfoForAction;
            DU_ALLOC(measurementInfoForAction, sizeof(MeasurementInfoForAction));
            if(measurementInfoForAction)
            {
               measurementInfoForAction->measurementTypeId = measurementInfoIdx+1;
               memcpy(measurementInfoForAction->measurementTypeName, MEASUREMENT_TYPE_NAME[measurementInfoIdx], measurementInfoLen+1);
            }
            DU_ALLOC(node, sizeof(CmLList));
            if(node)
            {
               node->node = (PTR) measurementInfoForAction;
               cmLListAdd2Tail(&duCb.e2apDb.ranFunction[ranFuncIdx].reportStyleList[reportStyleIdx].measurementInfoList, node);
            }
            else
            {

               DU_FREE(measurementInfoForAction, sizeof(MeasurementInfoForAction));
            }
         }
      }
      
      duCb.e2apDb.ranFunction[ranFuncIdx].ricIndicationHeaderFormat = RIC_INDICATION_HEADER_FORMAT;
      duCb.e2apDb.ranFunction[ranFuncIdx].ricIndicationMessageFormat = RIC_INDICATION_MESSAGE_FORMAT; 
   }
   memset(duCb.e2apDb.e2TransInfo.onGoingTransaction, 0, MAX_NUM_TRANSACTION * sizeof(E2TransInfo));
   
   /* Mib Params */
   mib.sysFrmNum = SYS_FRAME_NUM;
#ifdef NR_TDD
   mib.subCarrierSpacingCommon = MIB__subCarrierSpacingCommon_scs30or120;
#else
   mib.subCarrierSpacingCommon = MIB__subCarrierSpacingCommon_scs15or60;
#endif
   mib.ssb_SubcarrierOffset = SSB_SC_OFFSET; //Kssb
   mib.dmrs_TypeA_Position = MIB__dmrs_TypeA_Position_pos2;
   mib.controlResourceSetZero = CORESET_0_INDEX;
   mib.searchSpaceZero = SEARCHSPACE_0_INDEX;
   mib.cellBarred = MIB__cellBarred_notBarred;
   mib.intraFreqReselection = MIB__intraFreqReselection_notAllowed;
   duCfgParam.mibParams = mib;

   /* SIB1 Params */
   memset(&sib1.plmn, 0, sizeof(Plmn));
   sib1.plmn.mcc[0] = PLMN_MCC0;
   sib1.plmn.mcc[1] = PLMN_MCC1;
   sib1.plmn.mcc[2] = PLMN_MCC2;
   sib1.plmn.mnc[0] = PLMN_MNC0;
   sib1.plmn.mnc[1] = PLMN_MNC1;
   sib1.ranac = DU_RANAC;

#ifdef O1_ENABLE
   sib1.tac =  cellParams.nRTAC;
   sib1.cellIdentity =  CELL_IDENTITY *  cellParams.cellLocalId;
   DU_LOG("\nDEBUG --> DU_APP: readCfg(): OAM CellLocalId=%d", \
              sib1.cellIdentity);
#else
   sib1.tac = DU_TAC;
   sib1.cellIdentity = CELL_IDENTITY * NR_CELL_ID;
#endif
   sib1.cellResvdForOpUse = PLMN_IdentityInfo__cellReservedForOperatorUse_notReserved;
   sib1.connEstFailCnt = ConnEstFailureControl__connEstFailCount_n3;
   sib1.connEstFailOffValidity = ConnEstFailureControl__connEstFailOffsetValidity_s900;
   sib1.connEstFailOffset = 15;
   sib1.siSchedInfo.winLen = SI_SchedulingInfo__si_WindowLength_s5;
   sib1.siSchedInfo.broadcastSta = SchedulingInfo__si_BroadcastStatus_broadcasting;
   sib1.siSchedInfo.preiodicity = SchedulingInfo__si_Periodicity_rf8;
   sib1.siSchedInfo.sibType = SIB_TypeInfo__type_sibType2;
   sib1.siSchedInfo.sibValTag = SIB1_VALUE_TAG;

   fillServCellCfgCommSib(&sib1.srvCellCfgCommSib);

   duCfgParam.sib1Params = sib1;

   for(srvdCellIdx=0; srvdCellIdx<DEFAULT_CELLS; srvdCellIdx++)
   { 
      memset(&duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.nrCgi.plmn, 0, sizeof(Plmn));
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.nrCgi.plmn.mcc[0] = PLMN_MCC0;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.nrCgi.plmn.mcc[1] = PLMN_MCC1;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.nrCgi.plmn.mcc[2] = PLMN_MCC2;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.nrCgi.plmn.mnc[0] = PLMN_MNC0;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.nrCgi.plmn.mnc[1] = PLMN_MNC1;

      /*Cell ID */
 #ifdef O1_ENABLE
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.nrPci = cellParams.nRPCI;
 #else
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.nrPci = NR_PCI;
#endif
      /* List of Available PLMN */
      for(srvdPlmnIdx=0; srvdPlmnIdx<MAX_PLMN; srvdPlmnIdx++)
      {
         /* As per spec 38.473, Plmn identity consists of 3 digit from mcc
          * followed by either 2 digit or 3 digits of mnc */ 

         memset(&duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.srvdPlmn[srvdPlmnIdx].plmn, 0,\
         sizeof(Plmn));
 #ifdef O1_ENABLE
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.srvdPlmn[srvdPlmnIdx].plmn.mcc[0] = cellParams.plmnList[srvdPlmnIdx].mcc[0];
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.srvdPlmn[srvdPlmnIdx].plmn.mcc[1] = cellParams.plmnList[srvdPlmnIdx].mcc[1];
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.srvdPlmn[srvdPlmnIdx].plmn.mcc[2] = cellParams.plmnList[srvdPlmnIdx].mcc[2];
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.srvdPlmn[srvdPlmnIdx].plmn.mnc[0] = cellParams.plmnList[srvdPlmnIdx].mnc[0];
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.srvdPlmn[srvdPlmnIdx].plmn.mnc[1] = cellParams.plmnList[srvdPlmnIdx].mnc[1];
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.srvdPlmn[srvdPlmnIdx].plmn.mnc[2] = cellParams.plmnList[srvdPlmnIdx].mnc[2];
 #else
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.srvdPlmn[srvdPlmnIdx].plmn.mcc[0] = PLMN_MCC0;
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.srvdPlmn[srvdPlmnIdx].plmn.mcc[1] = PLMN_MCC1;
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.srvdPlmn[srvdPlmnIdx].plmn.mcc[2] = PLMN_MCC2;
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.srvdPlmn[srvdPlmnIdx].plmn.mnc[0] = PLMN_MNC0;
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.srvdPlmn[srvdPlmnIdx].plmn.mnc[1] = PLMN_MNC1;
#endif
      }
      /* List of Extended PLMN */
      for(srvdPlmnIdx=0; srvdPlmnIdx<MAX_PLMN; srvdPlmnIdx++)
      {
         /* As per spec 38.473, Plmn identity consists of 3 digit from mcc
          * followed by either 2 digit or 3 digits of mnc */ 

         memset(&duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.srvdPlmn[srvdPlmnIdx].extPlmn, 0, sizeof(Plmn));
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.srvdPlmn[srvdPlmnIdx].extPlmn.mcc[0] = PLMN_MCC0;
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.srvdPlmn[srvdPlmnIdx].extPlmn.mcc[1] = PLMN_MCC1;
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.srvdPlmn[srvdPlmnIdx].extPlmn.mcc[2] = PLMN_MCC2;
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.srvdPlmn[srvdPlmnIdx].extPlmn.mnc[0] = PLMN_MNC0;
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.srvdPlmn[srvdPlmnIdx].extPlmn.mnc[1] = PLMN_MNC1;
      } 
      /* List of Supporting Slices */
      for(srvdPlmnIdx=0; srvdPlmnIdx<MAX_PLMN; srvdPlmnIdx++)
      {
         taiSliceSuppLst = &duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellInfo.srvdPlmn[srvdPlmnIdx].\
         taiSliceSuppLst;
         
         /* TODO Calculte the exact number of supported slices once will get
          * cell configuration from O1 */
         taiSliceSuppLst->numSupportedSlices = NUM_OF_SUPPORTED_SLICE;
         if(taiSliceSuppLst->numSupportedSlices > MAX_NUM_OF_SLICE_ITEMS)
         {
            DU_LOG("\nERROR --> DU_APP: readCfg(): Number of supported slice [%d] is more than 1024",\
            taiSliceSuppLst->numSupportedSlices);
            return RFAILED;
         }

         DU_ALLOC(taiSliceSuppLst->snssai, taiSliceSuppLst->numSupportedSlices*sizeof(Snssai*));
         if(taiSliceSuppLst->snssai == NULLP)
         {
            DU_LOG("\nERROR --> DU_APP: readCfg():Memory allocation failed");
            return RFAILED;
         }
         
         for(sliceIdx=0; sliceIdx<taiSliceSuppLst->numSupportedSlices; sliceIdx++)
         {
            DU_ALLOC(taiSliceSuppLst->snssai[sliceIdx], sizeof(Snssai));
            if(taiSliceSuppLst->snssai[sliceIdx] == NULLP)
            {
               DU_LOG("\nERROR --> DU_APP: readCfg():Memory allocation failed");
               return RFAILED;
            }
#ifdef O1_ENABLE
            memcpy(taiSliceSuppLst->snssai[sliceIdx]->sd, cellParams.plmnList[sliceIdx].sd, \
                  SD_SIZE*sizeof(uint8_t));
            taiSliceSuppLst->snssai[sliceIdx]->sst = cellParams.plmnList[sliceIdx].sst;
#else
            memcpy(taiSliceSuppLst->snssai[sliceIdx], &snssai[sliceIdx], sizeof(Snssai));
#endif
         }
      }

      /* NR Mode info */
#ifdef NR_TDD
      /* NR TDD Mode info */
#ifdef O1_ENABLE      
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.nrArfcn = cellParams.arfcnUL;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.sulInfo.sulArfcn = cellParams.arfcnSUL;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.sulInfo.sulTxBw.nrScs = convertScsValToScsEnum(cellParams.ssbSubCarrierSpacing); 		
#else
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.nrArfcn = NR_UL_ARFCN;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.sulInfo.sulArfcn = SUL_ARFCN;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.sulInfo.sulTxBw.nrScs = NR_SCS; 		
#endif      
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.sulInfo.sulTxBw.nrb = NRB_273;	       

      for(freqBandIdx=0; freqBandIdx<MAX_NRCELL_BANDS; freqBandIdx++)
      {
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.freqBand[freqBandIdx].nrFreqBand =\
                                                                                                                     NR_FREQ_BAND;
         for(bandIdx=0; bandIdx<MAX_NRCELL_BANDS; bandIdx++)
         {
            duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.freqBand[freqBandIdx].sulBand[bandIdx]\
               = SUL_BAND; 	
         }
      }
#else
      /* NR FDD Mode info */
#ifdef O1_ENABLE
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.nrArfcn = cellParams.arfcnUL;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.sulInfo.sulArfcn = cellParams.arfcnSUL;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.ulTxBw.nrScs = convertScsValToScsEnum(cellParams.ssbSubCarrierSpacing);
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.sulInfo.sulTxBw.nrScs = convertScsValToScsEnum(cellParams.ssbSubCarrierSpacing);
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.nrArfcn = cellParams.arfcnDL;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.sulInfo.sulArfcn = cellParams.arfcnSUL;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.sulInfo.sulTxBw.nrScs = convertScsValToScsEnum(cellParams.ssbSubCarrierSpacing);
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.dlTxBw.nrScs = convertScsValToScsEnum(cellParams.ssbSubCarrierSpacing);
#else
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.nrArfcn = NR_UL_ARFCN;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.sulInfo.sulArfcn = SUL_ARFCN;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.ulTxBw.nrScs = NR_SCS;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.sulInfo.sulTxBw.nrScs = NR_SCS;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.nrArfcn = NR_DL_ARFCN;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.sulInfo.sulArfcn = SUL_ARFCN;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.sulInfo.sulTxBw.nrScs = NR_SCS;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.dlTxBw.nrScs = NR_SCS;
#endif
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.sulInfo.sulTxBw.nrb = NRB_106;
      for(freqBandIdx=0; freqBandIdx<MAX_NRCELL_BANDS; freqBandIdx++)
      {
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.freqBand[freqBandIdx].\
         nrFreqBand = NR_FREQ_BAND;
         for(bandIdx=0; bandIdx<MAX_NRCELL_BANDS; bandIdx++)
         {
            duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.freqBand[freqBandIdx].\
            sulBand[bandIdx] = SUL_BAND;
         }
      }
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.sulInfo.sulTxBw.nrb = NRB_106;
      for(freqBandIdx=0; freqBandIdx<MAX_NRCELL_BANDS; freqBandIdx++)
      {
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.freqBand[freqBandIdx].\
         nrFreqBand = NR_FREQ_BAND;
         for(bandIdx=0; bandIdx<MAX_NRCELL_BANDS; bandIdx++)
         {
            duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.freqBand[freqBandIdx].\
            sulBand[bandIdx] = SUL_BAND;
         }
      }

      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.ulTxBw.nrb = NRB_106;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.dlTxBw.nrb = NRB_106;
#endif

      /*Measurement Config and Cell Config */ 
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.measTimeCfgDuration = TIME_CFG; 

      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellDir = DL_UL; 

      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.cellType=CELL_TYPE;

      /* Broadcast PLMN Identity */
      for(brdcstPlmnIdx=0; brdcstPlmnIdx<MAX_BPLMN_NRCELL_MINUS_1; brdcstPlmnIdx++)
      { 
         for(plmnIdx=0; plmnIdx<MAX_PLMN; plmnIdx++)
         {
            memset(&duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.brdcstPlmnInfo[brdcstPlmnIdx].plmn[plmnIdx],\
            0, sizeof(Plmn));
            duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.brdcstPlmnInfo[brdcstPlmnIdx].plmn[plmnIdx].mcc[0] =\
            PLMN_MCC0;
            duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.brdcstPlmnInfo[brdcstPlmnIdx].plmn[plmnIdx].mcc[1] =\
            PLMN_MCC1;
            duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.brdcstPlmnInfo[brdcstPlmnIdx].plmn[plmnIdx].mcc[2] =\
            PLMN_MCC2;
            duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.brdcstPlmnInfo[brdcstPlmnIdx].plmn[plmnIdx].mnc[0] =\
            PLMN_MNC0;
            duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.brdcstPlmnInfo[brdcstPlmnIdx].plmn[plmnIdx].mnc[1] =\
            PLMN_MNC1;
         }
         /* Extended PLMN List */	 
         for(plmnIdx=0; plmnIdx<MAX_PLMN; plmnIdx++)
         {
            memset(&duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.brdcstPlmnInfo[brdcstPlmnIdx].\
            extPlmn[plmnIdx], 0, sizeof(Plmn));
            duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.brdcstPlmnInfo[brdcstPlmnIdx].\
            extPlmn[plmnIdx].mcc[0] = PLMN_MCC0;
            duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.brdcstPlmnInfo[brdcstPlmnIdx].\
            extPlmn[plmnIdx].mcc[1] = PLMN_MCC1;
            duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.brdcstPlmnInfo[brdcstPlmnIdx].\
            extPlmn[plmnIdx].mcc[2] = PLMN_MCC2;
            duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.brdcstPlmnInfo[brdcstPlmnIdx].\
            extPlmn[plmnIdx].mnc[0] = PLMN_MNC0;
            duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.brdcstPlmnInfo[brdcstPlmnIdx].\
            extPlmn[plmnIdx].mnc[1] = PLMN_MNC1;
         }
#ifdef O1_ENABLE
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.epsTac = cellParams.nRTAC; //TODO : to check and fill
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.tac = cellParams.nRTAC;
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.brdcstPlmnInfo[brdcstPlmnIdx].tac = cellParams.nRTAC;
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.brdcstPlmnInfo[brdcstPlmnIdx].nrCellId = cellParams.cellLocalId;
#else       
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.epsTac = DU_TAC; //TODO : to check and fill
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.tac = DU_TAC;
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.brdcstPlmnInfo[brdcstPlmnIdx].tac = DU_TAC;
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.brdcstPlmnInfo[brdcstPlmnIdx].nrCellId = NR_CELL_ID;
#endif     
         duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.brdcstPlmnInfo[brdcstPlmnIdx].ranac = NR_RANAC;
      }

      /*gnb DU System Info mib msg*/
      BuildMibMsg();
      DU_ALLOC(duCfgParam.srvdCellLst[srvdCellIdx].duSysInfo.mibMsg, encBufSize);
      if(!(duCfgParam.srvdCellLst[srvdCellIdx].duSysInfo.mibMsg))
      {
         DU_LOG("\nERROR  -->  DU APP : Memory allocation failure at readCfg");
         return RFAILED;
      }
      memcpy(duCfgParam.srvdCellLst[srvdCellIdx].duSysInfo.mibMsg, encBuf, encBufSize);
      duCfgParam.srvdCellLst[srvdCellIdx].duSysInfo.mibLen = encBufSize;

      /*gnb DU System Info mib msg*/
      BuildSib1Msg();
      DU_ALLOC(duCfgParam.srvdCellLst[srvdCellIdx].duSysInfo.sib1Msg,\
            encBufSize);
      if(!(duCfgParam.srvdCellLst[srvdCellIdx].duSysInfo.sib1Msg))
      {
         DU_LOG("\nERROR  -->  DU APP : Memory allocation failure at readCfg");
         return RFAILED;
      }
      memcpy(duCfgParam.srvdCellLst[srvdCellIdx].duSysInfo.sib1Msg,\
            encBuf,encBufSize);
      duCfgParam.srvdCellLst[srvdCellIdx].duSysInfo.sib1Len = encBufSize;

   }

   if(readMacCfg() != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : Failed while reading MAC config");
      return RFAILED;
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Copy Slice Cfg in temp structre in duCfgParams 
 *
 * @details
 *
 *    Function : cpyRrmPolicyInDuCfgParams
 *
 *    Functionality:
 *      - Copy Slice Cfg in temp structre in duCfgParams 
 *
 * @params[in] RrmPolicy rrmPolicy[], uint8_t policyNum, uint8_t memberList
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t cpyRrmPolicyInDuCfgParams(RrmPolicyList rrmPolicy[], uint8_t policyNum, MacSliceCfgReq *tempSliceCfg)
{
   uint8_t policyIdx = 0, memberListIdx = 0;
   if(policyNum)
   {
      tempSliceCfg->numOfRrmPolicy = policyNum;
      DU_ALLOC_SHRABL_BUF(tempSliceCfg->listOfRrmPolicy, tempSliceCfg->numOfRrmPolicy  * sizeof(MacSliceRrmPolicy*));
      if(!tempSliceCfg->listOfRrmPolicy)
      {
         DU_LOG("\nERROR  --> DU APP : Memory allocation failed in cpyRrmPolicyInDuCfgParams");
         return RFAILED;
      }

      for(policyIdx = 0; policyIdx<tempSliceCfg->numOfRrmPolicy; policyIdx++)
      {
         DU_ALLOC_SHRABL_BUF(tempSliceCfg->listOfRrmPolicy[policyIdx], sizeof(MacSliceRrmPolicy));
         if(!tempSliceCfg->listOfRrmPolicy[policyIdx])
         {
            DU_LOG("\nERROR  --> DU APP : Memory allocation failed in cpyRrmPolicyInDuCfgParams");
            return RFAILED;
         }

         tempSliceCfg->listOfRrmPolicy[policyIdx]->resourceType = rrmPolicy[policyIdx].resourceType;

         tempSliceCfg->listOfRrmPolicy[policyIdx]->numOfRrmPolicyMem = rrmPolicy[policyIdx].rRMMemberNum;

         if(tempSliceCfg->listOfRrmPolicy[policyIdx]->numOfRrmPolicyMem)
         {
            DU_ALLOC_SHRABL_BUF(tempSliceCfg->listOfRrmPolicy[policyIdx]->rRMPolicyMemberList,\
            tempSliceCfg->listOfRrmPolicy[policyIdx]->numOfRrmPolicyMem * sizeof(RrmPolicyMemberList*));

            if(!tempSliceCfg->listOfRrmPolicy[policyIdx]->rRMPolicyMemberList)
            {
               DU_LOG("\nERROR  --> DU APP : Memory allocation failed in cpyRrmPolicyInDuCfgParams");
               return RFAILED;
            }


            for(memberListIdx = 0; memberListIdx<tempSliceCfg->listOfRrmPolicy[policyIdx]->numOfRrmPolicyMem; memberListIdx++)
            {
               DU_ALLOC_SHRABL_BUF(tempSliceCfg->listOfRrmPolicy[policyIdx]->rRMPolicyMemberList[memberListIdx], sizeof(RrmPolicyMemberList));
               if(!tempSliceCfg->listOfRrmPolicy[policyIdx]->rRMPolicyMemberList[memberListIdx])
               {
                  DU_LOG("\nERROR  --> DU APP : Memory allocation failed in cpyRrmPolicyInDuCfgParams");
                  return RFAILED;
               }
               memcpy(&tempSliceCfg->listOfRrmPolicy[policyIdx]->rRMPolicyMemberList[memberListIdx]->snssai.sd,\
               &rrmPolicy[policyIdx].rRMPolicyMemberList[memberListIdx].sd, 3 * sizeof(uint8_t));
               memcpy(&tempSliceCfg->listOfRrmPolicy[policyIdx]->rRMPolicyMemberList[memberListIdx]->snssai.sst,\
               &rrmPolicy[policyIdx].rRMPolicyMemberList[memberListIdx].sst, sizeof(uint8_t));
               memcpy(&tempSliceCfg->listOfRrmPolicy[policyIdx]->rRMPolicyMemberList[memberListIdx]->plmn.mcc,\
               &rrmPolicy[policyIdx].rRMPolicyMemberList[memberListIdx].mcc, 3 * sizeof(uint8_t));
               memcpy(&tempSliceCfg->listOfRrmPolicy[policyIdx]->rRMPolicyMemberList[memberListIdx]->plmn.mnc,\
               &rrmPolicy[policyIdx].rRMPolicyMemberList[memberListIdx].mnc, 3 * sizeof(uint8_t));
            }
            tempSliceCfg->listOfRrmPolicy[policyIdx]->policyRatio.maxRatio = rrmPolicy[policyIdx].rRMPolicyMaxRatio;
            tempSliceCfg->listOfRrmPolicy[policyIdx]->policyRatio.minRatio = rrmPolicy[policyIdx].rRMPolicyMinRatio;
            tempSliceCfg->listOfRrmPolicy[policyIdx]->policyRatio.dedicatedRatio = rrmPolicy[policyIdx].rRMPolicyDedicatedRatio;
         }
      }
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Reads config and posts message to du_app on completion
 *
 * @details
 *
 *    Function : main
 *
 *    Functionality:
 *      - Calls readCfg()
 *      - Post to du_app for further processing
 *
 * @params[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duReadCfg()
{
   Pst pst;
   Buffer *mBuf;

   memset(&duCfgParam, 0, sizeof(DuCfgParams));

   //Read configs into duCfgParams
   if(readCfg() != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : Reading configuration failed");
      return RFAILED;
   }

   //Fill pst structure
   memset(&(pst), 0, sizeof(Pst));
   pst.srcEnt = (Ent)ENTDUAPP;
   pst.srcInst = (Inst)DU_INST;
   pst.srcProcId = DU_PROC;
   pst.dstEnt = pst.srcEnt;
   pst.dstInst = pst.srcInst;
   pst.dstProcId = pst.srcProcId;
   pst.event = EVTCFG;
   pst.selector = ODU_SELECTOR_TC;
   pst.pool= DU_POOL;

   /* Initialize the timer blocks */
   cmInitTimers(&(duCb.e2apDb.e2TimersInfo.e2Timers.e2SetupTimer), 1);

   /* Initialzie the timer queue */   
   memset(&(duCb.duTimersInfo.tmrTq), 0, sizeof(CmTqType) * DU_TQ_SIZE);
   
   /* Initialize the timer control point */
   memset(&(duCb.duTimersInfo.tmrTqCp), 0, sizeof(CmTqCp));
   duCb.duTimersInfo.tmrTqCp.tmrLen = DU_TQ_SIZE;
   
   /* Initialize the timer resolution */
   duCb.duTimersInfo.tmrRes = DU_TIMER_RESOLUTION;
   
   /* Timer Registration request to system services */
   if (ODU_REG_TMR_MT(pst.srcEnt, pst.srcInst, duCb.duTimersInfo.tmrRes, duActvTmr) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : Failed to register timer");
      return RFAILED;
   }   
              
   if(ODU_GET_MSG_BUF(DFLT_REGION, DU_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : Memory allocation failed in duReadCfg");
      return RFAILED;
   }

   if (ODU_POST_TASK(&pst, mBuf) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : ODU_POST_TASK failed in duReadCfg");
      return RFAILED;
   }

   return ROK;
}

/**********************************************************************
  End of file
 **********************************************************************/

