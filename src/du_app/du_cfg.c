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
#include "legtp.h"
#include "lrg.h"
#include "lkw.x"
#include "lrg.x"
#include "du_app_mac_inf.h"
#include "du_app_rlc_inf.h"
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

#ifdef NR_TDD
/*******************************************************************
 *
 * @brief Fills the Slot configuration 
 *
 * @details
 *
 *    Function : FillSlotConfig
 *
 *    Functionality:Fill the Slot configuration values
 * 
 *  @params[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void FillSlotConfig()
{
   uint8_t slot = 0;
   uint8_t symbol =0;
   
   memset(duCfgParam.macCellCfg.tddCfg.slotCfg, 0, sizeof(duCfgParam.macCellCfg.tddCfg.slotCfg[slot][symbol]* \
			   MAX_TDD_PERIODICITY_SLOTS*MAX_SYMB_PER_SLOT));
   
   //Filling the DL Slots and initializing flexi slot
   for(slot = 0; slot <= NUM_DL_SLOTS; slot++)
   {
      for(symbol =0; symbol < MAX_SYMB_PER_SLOT; symbol++)
      {
	 duCfgParam.macCellCfg.tddCfg.slotCfg[slot][symbol] = DL_SLOT;
      }
   }
  
   //Filling UL Slots
   for(slot = NUM_DL_SLOTS+1; slot <= NUM_DL_SLOTS+NUM_UL_SLOTS; slot++)
   {
      for(symbol = 0; symbol < MAX_SYMB_PER_SLOT; symbol++)
	 duCfgParam.macCellCfg.tddCfg.slotCfg[slot][symbol] = UL_SLOT;
   } 
   //Updating Flexi Slot
   slot = NUM_DL_SLOTS;
   duCfgParam.macCellCfg.tddCfg.slotCfg[slot][12] = FLEXI_SLOT;
   duCfgParam.macCellCfg.tddCfg.slotCfg[slot][13] = UL_SLOT;

}
#endif

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
   uint8_t idx=0, sliceIdx=0;
   F1TaiSliceSuppLst *taiSliceSuppLst;

   duCfgParam.macCellCfg.carrierId = CARRIER_IDX;

   /* Cell configuration */
   duCfgParam.macCellCfg.numerology = NR_NUMEROLOGY;
   duCfgParam.macCellCfg.dupType = DUPLEX_MODE;

   /* DL carrier configuration */
   duCfgParam.macCellCfg.dlCarrCfg.pres = TRUE;
#ifdef O1_ENABLE
   duCfgParam.macCellCfg.cellId = cellParams.cellLocalId;
   duCfgParam.macCellCfg.phyCellId = cellParams.nRPCI;
   duCfgParam.macCellCfg.dlCarrCfg.bw = cellParams.bSChannelBwUL;
   duCfgParam.macCellCfg.dlCarrCfg.freq = cellParams.bSChannelBwDL;
#else
   duCfgParam.macCellCfg.cellId = NR_CELL_ID;
   duCfgParam.macCellCfg.phyCellId = NR_PCI;
   duCfgParam.macCellCfg.dlCarrCfg.bw = NR_BANDWIDTH;
   duCfgParam.macCellCfg.dlCarrCfg.freq = NR_DL_ARFCN;
#endif
   
   duCfgParam.macCellCfg.dlCarrCfg.k0[0] = 1;
   duCfgParam.macCellCfg.dlCarrCfg.k0[1] = 1;
   duCfgParam.macCellCfg.dlCarrCfg.k0[2] = 1;
   duCfgParam.macCellCfg.dlCarrCfg.k0[3] = 1;
   duCfgParam.macCellCfg.dlCarrCfg.k0[4] = 1;
   duCfgParam.macCellCfg.dlCarrCfg.gridSize[0] = 273;
   duCfgParam.macCellCfg.dlCarrCfg.gridSize[1] = 1;
   duCfgParam.macCellCfg.dlCarrCfg.gridSize[2] = 1;
   duCfgParam.macCellCfg.dlCarrCfg.gridSize[3] = 1;
   duCfgParam.macCellCfg.dlCarrCfg.gridSize[4] = 1;
   duCfgParam.macCellCfg.dlCarrCfg.numAnt = NUM_TX_ANT;

   /* UL Carrier configuration */
   duCfgParam.macCellCfg.ulCarrCfg.pres = TRUE;
#ifdef O1_ENABLE
   duCfgParam.macCellCfg.ulCarrCfg.bw = cellParams.bSChannelBwUL;
   duCfgParam.macCellCfg.ulCarrCfg.freq = cellParams.bSChannelBwDL;
#else   
   duCfgParam.macCellCfg.ulCarrCfg.bw = NR_BANDWIDTH;
   duCfgParam.macCellCfg.ulCarrCfg.freq =  NR_UL_ARFCN;
#endif   
   duCfgParam.macCellCfg.ulCarrCfg.k0[0] = 1;
   duCfgParam.macCellCfg.ulCarrCfg.k0[1] = 1;
   duCfgParam.macCellCfg.ulCarrCfg.k0[2] = 1;
   duCfgParam.macCellCfg.ulCarrCfg.k0[3] = 1;
   duCfgParam.macCellCfg.ulCarrCfg.k0[4] = 1;
   duCfgParam.macCellCfg.ulCarrCfg.gridSize[0] = 1;
   duCfgParam.macCellCfg.ulCarrCfg.gridSize[1] = 1;
   duCfgParam.macCellCfg.ulCarrCfg.gridSize[2] = 1;
   duCfgParam.macCellCfg.ulCarrCfg.gridSize[3] = 1;
   duCfgParam.macCellCfg.ulCarrCfg.gridSize[4] = 1;
   duCfgParam.macCellCfg.ulCarrCfg.numAnt = NUM_RX_ANT;

   duCfgParam.macCellCfg.freqShft = FREQ_SHIFT_7P5KHZ;

   /* SSB configuration */
   duCfgParam.macCellCfg.ssbCfg.ssbPbchPwr = SSB_PBCH_PWR;
   duCfgParam.macCellCfg.ssbCfg.bchPayloadFlag = BCH_PAYLOAD;
   duCfgParam.macCellCfg.ssbCfg.ssbOffsetPointA = OFFSET_TO_POINT_A;
   duCfgParam.macCellCfg.ssbCfg.betaPss = BETA_PSS;
#ifdef O1_ENABLE
   duCfgParam.macCellCfg.ssbCfg.scsCmn = convertScsValToScsEnum(cellParams.ssbSubCarrierSpacing);
   duCfgParam.macCellCfg.ssbCfg.ssbPeriod = convertScsPeriodicityToEnum(cellParams.ssbPeriodicity);
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
   duCfgParam.macCellCfg.ssbCfg.multCarrBand = SSB_MULT_CARRIER_BAND;
   duCfgParam.macCellCfg.ssbCfg.multCellCarr = MULT_CELL_CARRIER;

   /* PRACH configuration */
   duCfgParam.macCellCfg.prachCfg.pres = TRUE;
   duCfgParam.macCellCfg.prachCfg.prachCfgIdx = PRACH_CONFIG_IDX;
   duCfgParam.macCellCfg.prachCfg.prachSeqLen = PRACH_SEQ_LEN;
   duCfgParam.macCellCfg.prachCfg.prachSubcSpacing = convertScsEnumValToScsVal(PRACH_SUBCARRIER_SPACING);
   duCfgParam.macCellCfg.prachCfg.prachRstSetCfg = PRACH_RESTRICTED_SET_CFG;
   duCfgParam.macCellCfg.prachCfg.msg1Fdm = NUM_PRACH_FDM;
   duCfgParam.macCellCfg.prachCfg.msg1FreqStart = PRACH_FREQ_START;
   duCfgParam.macCellCfg.prachCfg.rootSeqLen    = ROOT_SEQ_LEN;
   duCfgParam.macCellCfg.prachCfg.fdm[0].rootSeqIdx = ROOT_SEQ_IDX;
   duCfgParam.macCellCfg.prachCfg.fdm[0].numRootSeq = NUM_ROOT_SEQ;
   duCfgParam.macCellCfg.prachCfg.fdm[0].k1 = 0;
   duCfgParam.macCellCfg.prachCfg.fdm[0].zeroCorrZoneCfg = ZERO_CORRELATION_ZONE_CFG;
   duCfgParam.macCellCfg.prachCfg.fdm[0].numUnusedRootSeq = NUM_UNUSED_ROOT_SEQ;
   if(duCfgParam.macCellCfg.prachCfg.fdm[0].numUnusedRootSeq != 0)
   {
	DU_ALLOC_SHRABL_BUF(duCfgParam.macCellCfg.prachCfg.fdm[0].unsuedRootSeq, 
	NUM_UNUSED_ROOT_SEQ * sizeof(uint8_t));
        if(duCfgParam.macCellCfg.prachCfg.fdm[0].unsuedRootSeq == NULLP)
	{
	    DU_LOG("\nERROR  -->  DU_APP : Memory allocation failed at readMacCfg");
	    return RFAILED;
	}
	*(duCfgParam.macCellCfg.prachCfg.fdm[0].unsuedRootSeq) = UNUSED_ROOT_SEQ;
    }

   duCfgParam.macCellCfg.prachCfg.totalNumRaPreamble = NUM_RA_PREAMBLE;
   duCfgParam.macCellCfg.prachCfg.ssbPerRach = SSB_PER_RACH;
   duCfgParam.macCellCfg.prachCfg.numCbPreamblePerSsb = CB_PREAMBLE_PER_SSB;
   duCfgParam.macCellCfg.prachCfg.prachMultCarrBand = PRACH_MULT_CARRIER_BAND;
   duCfgParam.macCellCfg.prachCfg.raContResTmr = RA_CONT_RES_TIMER;
   duCfgParam.macCellCfg.prachCfg.rsrpThreshSsb = RSRP_THRESHOLD_SSB;
   duCfgParam.macCellCfg.prachCfg.raRspWindow = RA_RSP_WINDOW;
   duCfgParam.macCellCfg.prachCfg.prachRestrictedSet = PRACH_RESTRICTED_SET;
#ifdef NR_TDD   
   /* TDD configuration */
   duCfgParam.macCellCfg.tddCfg.pres = TRUE;
   duCfgParam.macCellCfg.tddCfg.tddPeriod = TDD_PERIODICITY;

   FillSlotConfig();

#endif
   /* RSSI Measurement configuration */
   duCfgParam.macCellCfg.rssiUnit = RSS_MEASUREMENT_UNIT;

   /* fill SIB1 configuration */
   duCfgParam.macCellCfg.sib1Cfg.sib1PduLen = duCfgParam.srvdCellLst[0].duSysInfo.sib1Len;
   DU_ALLOC_SHRABL_BUF(duCfgParam.macCellCfg.sib1Cfg.sib1Pdu,duCfgParam.srvdCellLst[0].duSysInfo.sib1Len);
   memcpy(duCfgParam.macCellCfg.sib1Cfg.sib1Pdu, duCfgParam.srvdCellLst[0].duSysInfo.sib1Msg, \
	 duCfgParam.srvdCellLst[0].duSysInfo.sib1Len);
   duCfgParam.macCellCfg.sib1Cfg.sib1RepetitionPeriod = SIB1_REPETITION_PERIOD;
   duCfgParam.macCellCfg.sib1Cfg.coresetZeroIndex = CORESET_0_INDEX;
   duCfgParam.macCellCfg.sib1Cfg.searchSpaceZeroIndex = SEARCHSPACE_0_INDEX;
   duCfgParam.macCellCfg.sib1Cfg.sib1Mcs = DEFAULT_MCS;
  
   duCfgParam.macCellCfg.sib1Cfg.pagingCfg.numPO = duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg.pcchCfg.ns;
   if((duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg.pcchCfg.firstPDCCHMontioringType != \
             PCCH_Config__firstPDCCH_MonitoringOccasionOfPO_PR_NOTHING) && (duCfgParam.macCellCfg.sib1Cfg.pagingCfg.numPO != 0))
   {
      duCfgParam.macCellCfg.sib1Cfg.pagingCfg.poPresent = TRUE;
      memcpy(duCfgParam.macCellCfg.sib1Cfg.pagingCfg.pagingOcc, 
              duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg.pcchCfg.firstPDCCHMontioringInfo,MAX_PO_PER_PF);
   }
   else
   {
      duCfgParam.macCellCfg.sib1Cfg.pagingCfg.poPresent = FALSE;
   }

   /* fill Intial DL BWP */
   duCfgParam.macCellCfg.initialDlBwp.bwp.firstPrb = 0;
   duCfgParam.macCellCfg.initialDlBwp.bwp.numPrb = TOTAL_PRB_20MHZ_MU0; /* configured to total BW */
   duCfgParam.macCellCfg.initialDlBwp.bwp.scs = duCfgParam.macCellCfg.ssbCfg.scsCmn;
   duCfgParam.macCellCfg.initialDlBwp.bwp.cyclicPrefix = NORMAL_CYCLIC_PREFIX;
   duCfgParam.macCellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.searchSpaceId = SEARCHSPACE_1_INDEX;
   duCfgParam.macCellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.coresetId = CORESET_0_INDEX;
   duCfgParam.macCellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.monitoringSlot =
      SS_MONITORING_SLOT_SL1; /* sl1 - all slots */
   duCfgParam.macCellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.duration = 0;
   duCfgParam.macCellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.monitoringSymbol =
      SS_MONITORING_SYMBOL;
   duCfgParam.macCellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.
      candidate.aggLevel1	= 8;
   duCfgParam.macCellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.
      candidate.aggLevel2	= 4;
   duCfgParam.macCellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.
      candidate.aggLevel4	= 2;
   duCfgParam.macCellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.
      candidate.aggLevel8	= 1;
   duCfgParam.macCellCfg.initialDlBwp.pdcchCommon.commonSearchSpace.
      candidate.aggLevel16	= 0;

   duCfgParam.macCellCfg.initialDlBwp.pdschCommon.numTimeDomAlloc = NUM_TIME_DOM_RSRC_ALLOC;
   idx = 0;
   duCfgParam.macCellCfg.initialDlBwp.pdschCommon.timeDomRsrcAllocList[idx].k0 = PDSCH_K0_CFG1;
   duCfgParam.macCellCfg.initialDlBwp.pdschCommon.timeDomRsrcAllocList[idx].mappingType = 
      PDSCH_MAPPING_TYPE_A;
   duCfgParam.macCellCfg.initialDlBwp.pdschCommon.timeDomRsrcAllocList[idx].startSymbol = 
      PDSCH_START_SYMBOL;
   duCfgParam.macCellCfg.initialDlBwp.pdschCommon.timeDomRsrcAllocList[idx].lengthSymbol =
      PDSCH_LENGTH_SYMBOL;
 
   idx++;
   duCfgParam.macCellCfg.initialDlBwp.pdschCommon.timeDomRsrcAllocList[idx].k0 = PDSCH_K0_CFG2;
   duCfgParam.macCellCfg.initialDlBwp.pdschCommon.timeDomRsrcAllocList[idx].mappingType = 
      PDSCH_MAPPING_TYPE_A;
   duCfgParam.macCellCfg.initialDlBwp.pdschCommon.timeDomRsrcAllocList[idx].startSymbol = 
      PDSCH_START_SYMBOL;
   duCfgParam.macCellCfg.initialDlBwp.pdschCommon.timeDomRsrcAllocList[idx].lengthSymbol =
      PDSCH_LENGTH_SYMBOL;

   /* ra-searchSpace ID is set to 1 */
   duCfgParam.macCellCfg.initialDlBwp.pdcchCommon.raSearchSpaceId = SEARCHSPACE_1_INDEX;

   /* fill Intial UL BWP */
   duCfgParam.macCellCfg.initialUlBwp.bwp.firstPrb = 0;
   duCfgParam.macCellCfg.initialUlBwp.bwp.numPrb = TOTAL_PRB_20MHZ_MU0; /* configured to total BW */
   duCfgParam.macCellCfg.initialUlBwp.bwp.scs = duCfgParam.macCellCfg.ssbCfg.scsCmn;
   duCfgParam.macCellCfg.initialUlBwp.bwp.cyclicPrefix = NORMAL_CYCLIC_PREFIX;
   duCfgParam.macCellCfg.initialUlBwp.puschCommon.numTimeDomRsrcAlloc = 2;
   duCfgParam.macCellCfg.initialUlBwp.puschCommon.timeDomRsrcAllocList[0].k2 = PUSCH_K2_CFG1;
   duCfgParam.macCellCfg.initialUlBwp.puschCommon.timeDomRsrcAllocList[0].mappingType = 
      PUSCH_MAPPING_TYPE_A;
   duCfgParam.macCellCfg.initialUlBwp.puschCommon.timeDomRsrcAllocList[0].startSymbol = 
      PUSCH_START_SYMBOL;
   duCfgParam.macCellCfg.initialUlBwp.puschCommon.timeDomRsrcAllocList[0].symbolLength =
      PUSCH_LENGTH_SYMBOL;

   duCfgParam.macCellCfg.initialUlBwp.puschCommon.timeDomRsrcAllocList[1].k2 = PUSCH_K2_CFG2;
   duCfgParam.macCellCfg.initialUlBwp.puschCommon.timeDomRsrcAllocList[1].mappingType = 
      PUSCH_MAPPING_TYPE_A;
   duCfgParam.macCellCfg.initialUlBwp.puschCommon.timeDomRsrcAllocList[1].startSymbol = 
      PUSCH_START_SYMBOL;
   duCfgParam.macCellCfg.initialUlBwp.puschCommon.timeDomRsrcAllocList[1].symbolLength =
      PUSCH_LENGTH_SYMBOL;

   duCfgParam.macCellCfg.dmrsTypeAPos = DMRS_TYPE_A_POS; 

   /* fill PUCCH config common */
   duCfgParam.macCellCfg.initialUlBwp.pucchCommon.pucchResourceCommon = PUCCH_RSRC_COMMON;
   duCfgParam.macCellCfg.initialUlBwp.pucchCommon.pucchGroupHopping = PUCCH_GROUP_HOPPING;
   
   /* Plmn And SNSSAI Configuration */
   memcpy(&duCfgParam.macCellCfg.plmnInfoList.plmn, &duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.srvdPlmn[0].plmn,\
   sizeof(Plmn));
   taiSliceSuppLst = &duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.srvdPlmn[0].taiSliceSuppLst;
   duCfgParam.macCellCfg.plmnInfoList.numSupportedSlice = taiSliceSuppLst->numSupportedSlices;
   if(taiSliceSuppLst->snssai)
   {
      DU_ALLOC_SHRABL_BUF(duCfgParam.macCellCfg.plmnInfoList.snssai, (duCfgParam.macCellCfg.plmnInfoList.numSupportedSlice) * sizeof(Snssai*));
      if(duCfgParam.macCellCfg.plmnInfoList.snssai == NULLP)
      {
         DU_LOG("\nERROR  --> DU_APP: Memory allocation failed at readMacCfg");
         return RFAILED;
      }
   }
   for(sliceIdx=0; sliceIdx<taiSliceSuppLst->numSupportedSlices; sliceIdx++)
   {
      if(taiSliceSuppLst->snssai[sliceIdx] != NULLP)
      {
         DU_ALLOC_SHRABL_BUF(duCfgParam.macCellCfg.plmnInfoList.snssai[sliceIdx], sizeof(Snssai));
         if(duCfgParam.macCellCfg.plmnInfoList.snssai[sliceIdx] == NULLP)
         {
            DU_LOG("\nERROR  --> DU_APP: Memory allocation failed at readMacCfg");
            return RFAILED;
         }
         memcpy(duCfgParam.macCellCfg.plmnInfoList.snssai[sliceIdx], taiSliceSuppLst->snssai[sliceIdx], sizeof(Snssai));
      }
   }

#ifndef O1_ENABLE

   /*Note: Static Configuration, when O1 is not configuring the RRM policy*/
   RrmPolicyList rrmPolicy;
   rrmPolicy.id[0] = 1;
   rrmPolicy.resourceType = PRB;
   rrmPolicy.rRMMemberNum = 1;
   memcpy(rrmPolicy.rRMPolicyMemberList[0].mcc,duCfgParam.macCellCfg.plmnInfoList.plmn.mcc, 3*sizeof(uint8_t));
   memcpy(rrmPolicy.rRMPolicyMemberList[0].mnc,duCfgParam.macCellCfg.plmnInfoList.plmn.mnc, 3*sizeof(uint8_t));
   rrmPolicy.rRMPolicyMemberList[0].sst = 1;
   rrmPolicy.rRMPolicyMemberList[0].sd[0] = 2;
   rrmPolicy.rRMPolicyMemberList[0].sd[1] = 3;
   rrmPolicy.rRMPolicyMemberList[0].sd[2] = 4;
   rrmPolicy.rRMPolicyMaxRatio = 90;
   rrmPolicy.rRMPolicyMinRatio = 30;
   rrmPolicy.rRMPolicyDedicatedRatio = 10;

   cpyRrmPolicyInDuCfgParams(&rrmPolicy, 1, &duCfgParam.tempSliceCfg);
  
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
   duPort[F1_INTERFACE]   = DU_PORT;     /* DU Port idx  0 38472 */
   duPort[E2_INTERFACE]   = RIC_PORT;    /* RIC Port idx 1 38482 */
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
   uint8_t srvdCellIdx, bandIdx, sliceIdx, plmnIdx;
   uint8_t brdcstPlmnIdx, freqBandIdx, srvdPlmnIdx;
   uint32_t ipv4_du, ipv4_cu, ipv4_ric;
   MibParams mib;
   Sib1Params sib1;
   F1TaiSliceSuppLst *taiSliceSuppLst;

#ifndef O1_ENABLE
   /* Note: Added these below variable for local testing*/
   Snssai snssai[NUM_OF_SUPPORTED_SLICE] = {{1,{2,3,4}},{5,{6,7,8}}};
#endif

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

   duCfgParam.sctpParams.cuPort = CU_PORT;
   duCfgParam.sctpParams.ricPort = RIC_PORT;
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
   duCfgParam.egtpParams.localPort = DU_EGTP_PORT;
   duCfgParam.egtpParams.destIp.ipV4Pres = TRUE;
   duCfgParam.egtpParams.destIp.ipV4Addr = ipv4_cu;
   duCfgParam.egtpParams.destPort = CU_EGTP_PORT;
   duCfgParam.egtpParams.minTunnelId = MIN_TEID;
   duCfgParam.egtpParams.maxTunnelId = MAX_TEID;

   duCfgParam.maxUe = 32; //TODO: Check
   /* DU Info */	
   duCfgParam.duId = DU_ID;	
   strcpy((char*)duCfgParam.duName,DU_NAME);

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
   mib.intraFreqReselection =
      MIB__intraFreqReselection_notAllowed;
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
      DU_LOG("\n*********DEBUG --> DU_APP: readCfg(): OAM CellLocalId=%d", \
              sib1.cellIdentity);
#else
   sib1.tac = DU_TAC;
   sib1.cellIdentity = CELL_IDENTITY * NR_CELL_ID;
#endif
   sib1.cellResvdForOpUse = PLMN_IdentityInfo__cellReservedForOperatorUse_notReserved;
   sib1.connEstFailCnt = ConnEstFailureControl__connEstFailCount_n3;
   sib1.connEstFailOffValidity = ConnEstFailureControl__connEstFailOffsetValidity_s120;
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
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.sulInfo.sulTxBw.nrScs = convertScsPeriodicityToEnum(cellParams.ssbPeriodicity); 		
#else
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.nrArfcn = NR_UL_ARFCN;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.sulInfo.sulArfcn = SUL_ARFCN;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.sulInfo.sulTxBw.nrScs = SSB_PRDCTY_MS_20; 		
#endif      
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.sulInfo.sulTxBw.nrb = NRB_106;	       

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
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.ulTxBw.nrScs = convertScsPeriodicityToEnum(cellParams.ssbPeriodicity);
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.sulInfo.sulTxBw.nrScs = convertScsPeriodicityToEnum(cellParams.ssbPeriodicity);
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.nrArfcn = cellParams.arfcnDL;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.sulInfo.sulArfcn = cellParams.arfcnSUL;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.sulInfo.sulTxBw.nrScs = convertScsPeriodicityToEnum(cellParams.ssbPeriodicity);
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.dlTxBw.nrScs = convertScsPeriodicityToEnum(cellParams.ssbPeriodicity);
#else
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.nrArfcn = NR_UL_ARFCN;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.sulInfo.sulArfcn = SUL_ARFCN;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.ulTxBw.nrScs = SSB_PRDCTY_MS_20;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.sulInfo.sulTxBw.nrScs = SSB_PRDCTY_MS_20;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.nrArfcn = NR_DL_ARFCN;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.sulInfo.sulArfcn = SUL_ARFCN;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.sulInfo.sulTxBw.nrScs = SSB_PRDCTY_MS_20;
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.f1Mode.mode.fdd.dlTxBw.nrScs = SSB_PRDCTY_MS_20;
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
      duCfgParam.srvdCellLst[srvdCellIdx].duCellInfo.measTimeCfg = TIME_CFG; 

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
uint8_t cpyRrmPolicyInDuCfgParams(RrmPolicyList rrmPolicy[], uint8_t policyNum, CopyOfRecvdSliceCfg *tempSliceCfg)
{
   uint8_t policyIdx = 0, memberListIdx = 0, count = 0;

   if(policyNum)
   {
      tempSliceCfg->totalRrmPolicy = policyNum;
      DU_ALLOC(tempSliceCfg->rrmPolicy, tempSliceCfg->totalRrmPolicy * sizeof(RrmPolicy*));
      if(tempSliceCfg->rrmPolicy == NULLP)
      {
         DU_LOG("\nERROR  --> DU APP : Memory allocation failed in cpyRrmPolicyInDuCfgParams");
         return RFAILED;
      }

      for(policyIdx = 0; policyIdx<tempSliceCfg->totalRrmPolicy; policyIdx++)
      {
         DU_ALLOC(tempSliceCfg->rrmPolicy[policyIdx],  sizeof(RrmPolicy));
         if(tempSliceCfg->rrmPolicy[policyIdx] == NULLP)
         {
            DU_LOG("\nERROR  --> DU APP : Memory allocation failed in cpyRrmPolicyInDuCfgParams");
            return RFAILED;
         }
         
         if(rrmPolicy[policyIdx].rRMMemberNum)
         {
            tempSliceCfg->rrmPolicy[policyIdx]->numMemberList = rrmPolicy[policyIdx].rRMMemberNum;  
            DU_ALLOC(tempSliceCfg->rrmPolicy[policyIdx]->memberList, tempSliceCfg->rrmPolicy[policyIdx]->numMemberList * sizeof(PolicyMemberList*))
            if(tempSliceCfg->rrmPolicy[policyIdx]->memberList == NULLP)
            {
               DU_LOG("\nERROR  --> DU APP : Memory allocation failed in cpyRrmPolicyInDuCfgParams");
               return RFAILED;
            }

            for(memberListIdx = 0; memberListIdx<tempSliceCfg->rrmPolicy[policyIdx]->numMemberList; memberListIdx++)
            {
               DU_ALLOC(tempSliceCfg->rrmPolicy[policyIdx]->memberList[memberListIdx], sizeof(PolicyMemberList))
               if(tempSliceCfg->rrmPolicy[policyIdx]->memberList[memberListIdx] == NULLP)
               {
                  DU_LOG("\nERROR  --> DU APP : Memory allocation failed in cpyRrmPolicyInDuCfgParams");
                  return RFAILED;
               }
               memcpy(&tempSliceCfg->rrmPolicy[policyIdx]->memberList[memberListIdx]->snssai.sd, &rrmPolicy[policyIdx].rRMPolicyMemberList[memberListIdx].sd, 3 * sizeof(uint8_t));
               memcpy(&tempSliceCfg->rrmPolicy[policyIdx]->memberList[memberListIdx]->snssai.sst, &rrmPolicy[policyIdx].rRMPolicyMemberList[memberListIdx].sst, sizeof(uint8_t));
               memcpy(&tempSliceCfg->rrmPolicy[policyIdx]->memberList[memberListIdx]->plmn.mcc, &rrmPolicy[policyIdx].rRMPolicyMemberList[memberListIdx].mcc, 3 * sizeof(uint8_t));
               memcpy(&tempSliceCfg->rrmPolicy[policyIdx]->memberList[memberListIdx]->plmn.mnc, &rrmPolicy[policyIdx].rRMPolicyMemberList[memberListIdx].mnc, 3 * sizeof(uint8_t));
               count++;
            }
         }
         
         tempSliceCfg->rrmPolicy[policyIdx]->rsrcType  = rrmPolicy[policyIdx].resourceType;
         tempSliceCfg->rrmPolicy[policyIdx]->policyMaxRatio = rrmPolicy[policyIdx].rRMPolicyMaxRatio;
         tempSliceCfg->rrmPolicy[policyIdx]->policyMinRatio = rrmPolicy[policyIdx].rRMPolicyMinRatio;
         tempSliceCfg->rrmPolicy[policyIdx]->policyDedicatedRatio = rrmPolicy[policyIdx].rRMPolicyDedicatedRatio;

      }
      tempSliceCfg->totalSliceCount = count;
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

