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

#ifdef XML_BASED_CONFIG
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <arpa/inet.h>
#endif

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
   SupportedSliceList *taiSliceSuppLst;

   /* DL carrier configuration */
#ifdef O1_ENABLE
   duCfgParam.macCellCfg.cellId = cellParams.cellLocalId;
   duCfgParam.macCellCfg.carrCfg.dlBw = cellParams.bSChannelBwDL;
   duCfgParam.macCellCfg.carrCfg.arfcnDL = cellParams.arfcnDL;
#else
   duCfgParam.macCellCfg.cellId = NR_CELL_ID;
   duCfgParam.macCellCfg.carrCfg.dlBw = NR_BANDWIDTH;
   duCfgParam.macCellCfg.carrCfg.arfcnDL  = NR_DL_ARFCN;
#endif
   duCfgParam.macCellCfg.carrCfg.numTxAnt = NUM_TX_ANT;
   /* UL Carrier configuration */
#ifdef O1_ENABLE
   duCfgParam.macCellCfg.carrCfg.ulBw = cellParams.bSChannelBwUL;
   duCfgParam.macCellCfg.carrCfg.arfcnUL = cellParams.arfcnUL;
#else   
   duCfgParam.macCellCfg.carrCfg.ulBw = NR_BANDWIDTH;
   duCfgParam.macCellCfg.carrCfg.arfcnUL =  NR_UL_ARFCN;
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
      duCfgParam.macCellCfg.cellCfg.plmnInfoList[plmnIdx].suppSliceList.numSupportedSlices = taiSliceSuppLst->numSupportedSlices;
      if(taiSliceSuppLst->snssai)
      {
         DU_ALLOC_SHRABL_BUF(duCfgParam.macCellCfg.cellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai, (duCfgParam.macCellCfg.cellCfg.plmnInfoList[plmnIdx].suppSliceList.numSupportedSlices) * sizeof(Snssai*));
         if(duCfgParam.macCellCfg.cellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai == NULLP)
         {
            DU_LOG("\nERROR  --> DU_APP: Memory allocation failed at readMacCfg");
            return RFAILED;
         }
      }
      for(sliceIdx=0; sliceIdx < taiSliceSuppLst->numSupportedSlices; sliceIdx++)
      {
         if(taiSliceSuppLst->snssai[sliceIdx] != NULLP)
         {
            DU_ALLOC_SHRABL_BUF(duCfgParam.macCellCfg.cellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai[sliceIdx], sizeof(Snssai));
            if(duCfgParam.macCellCfg.cellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai[sliceIdx] == NULLP)
            {
               DU_LOG("\nERROR  --> DU_APP: Memory allocation failed at readMacCfg");
               return RFAILED;
            }
            memcpy(duCfgParam.macCellCfg.cellCfg.plmnInfoList[plmnIdx].suppSliceList.snssai[sliceIdx], taiSliceSuppLst->snssai[sliceIdx], sizeof(Snssai));
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
   RrmPolicyList rrmPolicy;
   rrmPolicy.id[0] = 1;
   rrmPolicy.resourceType = PRB;
   rrmPolicy.rRMMemberNum = 1;
   memcpy(rrmPolicy.rRMPolicyMemberList[0].mcc,duCfgParam.macCellCfg.cellCfg.plmnInfoList[0].plmn.mcc, 3*sizeof(uint8_t));
   memcpy(rrmPolicy.rRMPolicyMemberList[0].mnc,duCfgParam.macCellCfg.cellCfg.plmnInfoList[0].plmn.mnc, 3*sizeof(uint8_t));
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
   SupportedSliceList *taiSliceSuppLst;
   uint8_t measurementInfoIdx =0, measurementInfoLen=0;
   char shortName[] = SHORT_NAME;
   char serviceModelOID[]= SERVICE_MODEL_OID;
   char  description[] = DESCRIPTION;
   char event_trigger_style_name[]= EVENT_TRIGGER_STYLE_NAME;
   char ric_report_style_name[]= REPORT_STYLE_NAME;

#ifndef O1_ENABLE
   /* Note: Added these below variable for local testing*/
   Snssai snssai[NUM_OF_SUPPORTED_SLICE] = {{1,{2,3,4}},{5,{6,7,8}}};
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
         
         for(measurementInfoIdx =0; measurementInfoIdx<NUM_OF_MEASUREMENT_INFO_SUPPORTED(CONFIG_ADD); measurementInfoIdx++)
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

      cmLListInit(&duCb.e2apDb.ranFunction[ranFuncIdx].subscriptionList);
   }
   memset(duCb.e2apDb.e2TransInfo.e2InitTransaction, 0, MAX_NUM_TRANSACTION * sizeof(E2TransInfo));
   memset(duCb.e2apDb.e2TransInfo.ricInitTransaction, 0, MAX_NUM_TRANSACTION * sizeof(E2TransInfo));
   
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

#ifdef XML_BASED_CONFIG
/*******************************************************************
 *
 * @brief Fill SCTP Parameters
 *
 * @details
 *
 *    Function : parseSctpParams
 *
 *    Functionality: Fill SCTP Parameters
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseSctpParams(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, SctpParams *sctp)
{
   uint8_t max_du_port;
   uint16_t f1_sctp_port;
   uint16_t e2_sctp_port;

   memset(sctp, 0, sizeof(SctpParams));
   cur = cur->xmlChildrenNode;
   while (cur != NULL) 
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MAX_DU_PORT")) && (cur->ns == ns))
      {
         max_du_port = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
         if (max_du_port == 2 )
         {
            sctp->duPort[F1_INTERFACE] = f1_sctp_port;     /* DU Port idx  0  */
            sctp->duPort[E2_INTERFACE] = e2_sctp_port;    /* RIC Port idx 1  */
         }
      }
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_SCTP_PORT")) && (cur->ns == ns))
      {
         f1_sctp_port = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"E2_SCTP_PORT")) && (cur->ns == ns))
      {
         e2_sctp_port = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

#ifdef O1_ENABLE
      sctp->cuPort = g_cfg.CU_Port;
      sctp->ricPort = g_cfg.RIC_Port;
#else
      sctp->cuPort = f1_sctp_port;
      sctp->ricPort = e2_sctp_port;
#endif
      cur = cur->next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill EGTP Parameters
 *
 * @details
 *
 *    Function : parseEgtpParams
 *
 *    Functionality: Fill EGTP Parmeters
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseEgtpParams(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, F1EgtpParams *egtp) 
{
   memset(egtp, 0, sizeof(F1EgtpParams));
   cur = cur->xmlChildrenNode;
   while (cur != NULL) 
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"LOCAL_F1_EGTP_PORT")) && (cur->ns == ns))
      {
         egtp->localPort = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"DEST_F1_EGTP_PORT")) && (cur->ns == ns))
      {
         egtp->destPort = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MIN_TEID")) && (cur->ns == ns))
      {
         egtp->minTunnelId = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill MIB configuration 
 *
 * @details
 *
 *    Function : parseMibParams
 *
 *    Functionality: Fill MIB configuration
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseMibParams(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, MibParams *mib)
{
   memset(mib, 0, sizeof(MibParams));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SYS_FRAME_NUM")) && (cur->ns == ns))
      {
         mib->sysFrmNum = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SUB_CARR_SPACE")) && (cur->ns == ns))
      {
         mib->subCarrierSpacingCommon = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SSB_SC_OFFSET")) && (cur->ns == ns))
      {
         mib->ssb_SubcarrierOffset = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"DMRS_TYPEA_POSITION")) && (cur->ns == ns))
      {
         mib->dmrs_TypeA_Position = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CORESET_0_INDEX")) && (cur->ns == ns))
      {
         mib->controlResourceSetZero = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SEARCHSPACE_0_INDEX")) && (cur->ns == ns))
      {
         mib->searchSpaceZero = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CELL_BARRED")) && (cur->ns == ns))
      {
         mib->cellBarred = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"INTRA_FREQ_RESELECT")) && (cur->ns == ns))
      {
         mib->intraFreqReselection = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill RRC version 
 *
 * @details
 *
 *    Function : parseF1RrcVersion
 *
 *    Functionality: Fill RRC version
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseF1RrcVersion(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,F1RrcVersion *f1RrcVersion)
{
   memset(f1RrcVersion, 0, sizeof(F1RrcVersion));
   cur = cur->xmlChildrenNode;
   while(cur != NULL)
   {
      if((!xmlStrcmp(cur->name, (const xmlChar *)"RRC_VER")) && (cur->ns == ns))
      {
         strcpy((char*)f1RrcVersion->rrcVer, (char*)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if((!xmlStrcmp(cur->name, (const xmlChar *)"EXT_RRC_VER")) && (cur->ns == ns))
      {
         f1RrcVersion->extRrcVer = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill PLMN ID
 *
 * @details
 *
 *    Function : parsePlmn
 *
 *    Functionality: Fill PLMN ID
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parsePlmn(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, Plmn *plmn)
{
   xmlNodePtr child = NULLP;

   memset(plmn, 0, sizeof(Plmn));
   cur = cur->xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MCC")) && (cur->ns == ns))
      {
         child = cur->xmlChildrenNode;
         while (child != NULL)
         {
            if ((!xmlStrcmp(child->name, (const xmlChar *)"plmn_MCC0")) && (child->ns == ns))
            {
               plmn->mcc[0] = atoi((char *)xmlNodeListGetString(doc, child->xmlChildrenNode, 1));
            }

            if ((!xmlStrcmp(child->name, (const xmlChar *)"plmn_MCC1")) && (child->ns == ns))
            {
               plmn->mcc[1] = atoi((char *)xmlNodeListGetString(doc, child->xmlChildrenNode, 1));
            }

            if ((!xmlStrcmp(child->name, (const xmlChar *)"plmn_MCC2")) && (child->ns == ns))
            {
               plmn->mcc[2] = atoi((char *)xmlNodeListGetString(doc, child->xmlChildrenNode, 1));
            }

            child = child->next;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MNC")) && (cur->ns == ns))
      {
         child = cur->xmlChildrenNode;
         while (child != NULL)
         {
            if ((!xmlStrcmp(child->name, (const xmlChar *)"plmn_MNC0")) && (child->ns == ns))
            {
               plmn->mnc[0] = atoi((char *)xmlNodeListGetString(doc, child->xmlChildrenNode, 1));  
            }

            if ((!xmlStrcmp(child->name, (const xmlChar *)"plmn_MNC1")) && (child->ns == ns))
            {
               plmn->mnc[1] = atoi((char *)xmlNodeListGetString(doc, child->xmlChildrenNode, 1));
            }

            if ((!xmlStrcmp(child->name, (const xmlChar *)"plmn_MNC2")) && (child->ns == ns))
            {
               plmn->mnc[2] = atoi((char *)xmlNodeListGetString(doc, child->xmlChildrenNode, 1));
            }

            child = child->next;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill NR CGI
 *
 * @details
 *
 *    Function : parseNrCgi
 *
 *    Functionality: Fill NR CGI
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseNrCgi(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, NrEcgi *nrCgi)
{
   memset(nrCgi, 0, sizeof(NrEcgi));
   cur = cur->xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PLMN")) && (cur->ns == ns))
      {
         if(parsePlmn(doc, ns, cur, &nrCgi->plmn) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CELL_ID")) && (cur->ns == ns))
      {
         nrCgi-> cellId = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill SNSSAI
 *
 * @details
 *
 *    Function : parseSnssai
 *
 *    Functionality: Fill SNSSAI
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseSnssai(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, Snssai *snssai)
{
   uint8_t sdIdx = 0;
   xmlNodePtr child;

   memset(snssai, 0, sizeof(Snssai));
   cur = cur ->xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SST")) && (cur->ns == ns))
      {
         snssai->sst = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SD_SIZE")) && (cur->ns == ns))
      {
         child = cur->xmlChildrenNode;
         while(child != NULL)
         {
            if ((!xmlStrcmp(child->name, (const xmlChar *)"SD")) && (child->ns == ns))
            {
               snssai->sd[sdIdx] = atoi((char *)xmlNodeListGetString(doc, child->xmlChildrenNode, 1));
               sdIdx++;
            }
            child = child -> next;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill Supported Slice List
 *
 * @details
 *
 *    Function : parseSupportedSliceList
 *
 *    Functionality: Fill Supported Slice List
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseSupportedSliceList(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, SupportedSliceList *sliceSuppLst)
{
   uint8_t sliceIdx = 0;
   xmlNodePtr child = NULLP;
   xmlNodePtr snssaiNode = NULLP;

   memset(sliceSuppLst, 0, sizeof(SupportedSliceList));
   cur = cur->xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_SUPPORT_SLICE")) && (cur->ns == ns))
      {
         sliceSuppLst->numSupportedSlices = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
         if(sliceSuppLst->numSupportedSlices > MAX_NUM_OF_SLICE_ITEMS)
         {
            DU_LOG("\nERROR --> DU_APP: %s: Number of supported slice [%d] is more than 1024",\
                  __func__, sliceSuppLst->numSupportedSlices);
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SNSSAI_LIST")) && (cur->ns == ns))
      {
         DU_ALLOC_SHRABL_BUF(sliceSuppLst->snssai, (sliceSuppLst->numSupportedSlices) * sizeof(Snssai*));
         if (sliceSuppLst->snssai == NULLP)
         {
            DU_LOG("\nERROR  --> DU_APP: %s: Memory allocation failed at line %d", __func__, __LINE__);
            return RFAILED;
         }

         for (sliceIdx = 0; sliceIdx < sliceSuppLst->numSupportedSlices; sliceIdx++)
         {
            DU_ALLOC_SHRABL_BUF(sliceSuppLst->snssai[sliceIdx], sizeof(Snssai));
            if (sliceSuppLst->snssai[sliceIdx] == NULLP)
            {
               DU_LOG("\nERROR  --> DU_APP: %s: Memory allocation failed at line %d", __func__, __LINE__);
               return RFAILED;
            }
            else
            {
#ifdef O1_ENABLE
               memcpy(sliceSuppLst->snssai[sliceIdx]->sd, cellParams.plmnList[sliceIdx].sd, SD_SIZE*sizeof(uint8_t));
               sliceSuppLst->snssai[sliceIdx]->sst = cellParams.plmnList[sliceIdx].sst;
#endif
            }
         }

#ifndef O1_ENABLE
         child = cur->xmlChildrenNode;
         while (child != NULL)
         {
            if ((!xmlStrcmp(child->name, (const xmlChar *)"LIST")) && (child->ns == ns))
            {     
               sliceIdx = 0;
               snssaiNode = child->xmlChildrenNode;
               while (snssaiNode != NULL)
               {
                  if ((!xmlStrcmp(snssaiNode->name, (const xmlChar *)"SNSSAI")) && (snssaiNode->ns == ns))
                  {
                     if(parseSnssai(doc, ns, snssaiNode, sliceSuppLst->snssai[sliceIdx]) != ROK)
                     {
                        return RFAILED;
                     }
                     sliceIdx++;
                  }
                  snssaiNode = snssaiNode->next;
               }
            }
            child = child->next;
         }
#endif
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill Served PLMN
 *
 * @details
 *
 *    Function : parseF1SrvdPlmn
 *
 *    Functionality: Fill Served PLMN
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseF1SrvdPlmn(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, F1SrvdPlmn *srvdPlmn, uint8_t srvdPlmnIdx)
{
   memset(srvdPlmn, 0, sizeof(F1SrvdPlmn));
   cur = cur->xmlChildrenNode;
   while(cur != NULL)
   {
#ifdef O1_ENABLE
      fillPlmnFromO1(&srvdPlmn ->plmn, srvdPlmnIdx);
#else
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PLMN")) && (cur->ns == ns))
      {
         if(parsePlmn(doc, ns, cur, &srvdPlmn->plmn) != ROK)
         {
            return RFAILED;
         }
      }
#endif 

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"EXT_PLMN")) && (cur->ns == ns))
      {
         if(parsePlmn(doc, ns, cur, &srvdPlmn->extPlmn) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_SLICE_SUPP_LST")) && (cur->ns == ns))
      {
         if(parseSupportedSliceList(doc, ns, cur, &srvdPlmn->taiSliceSuppLst) != ROK)
         {
            return RFAILED;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill cell information
 *
 * @details
 *
 *    Function : parseF1CellInfo
 *
 *    Functionality: Fill cell information
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseF1CellInfo(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, F1CellInfo *f1CellInfo)
{
   memset(f1CellInfo, 0, sizeof(F1CellInfo));
   cur = cur->xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NR_CGI")) && (cur->ns == ns))
      {
         if(parseNrCgi(doc, ns, cur, &f1CellInfo->nrCgi) != ROK)
         {
            return RFAILED;
         }
      }

#ifdef O1_ENABLE
      f1CellInfo->nrPci = cellParams.nRPCI;
#else
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NR_PCI")) && (cur->ns == ns))
      {
         f1CellInfo->nrPci = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
#endif   

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_SRVD_PLMN")) && (cur->ns == ns))
      {
         if(parseF1SrvdPlmn(doc, ns, cur, &f1CellInfo->srvdPlmn[0], 0) != ROK)
         {
            return RFAILED;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill Frequency Band
 *
 * @details
 *
 *    Function : parseF1FreqBand
 *
 *    Functionality: Fill Frequency Band
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseF1FreqBand(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, F1FreqBand *freqBand)
{
   uint8_t sulIdx = 0;
   uint16_t sulValue = 0;
   xmlNodePtr child;
   xmlNodePtr sulChild;

   memset(freqBand, 0, sizeof(F1FreqBand));
   cur = cur->xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NR_FREQ_BAND")) && (cur->ns == ns))
      {
         freqBand->nrFreqBand = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SUL_BAND_LIST")) && (cur->ns == ns))
      {
         child = cur->xmlChildrenNode;
         while (child != NULL) 
         {
            if ((!xmlStrcmp(child->name, (const xmlChar *)"LIST")) && (child->ns == ns)) 
            {
               sulChild = child->xmlChildrenNode;
               sulIdx = 0;
               while (sulChild != NULL) 
               {
                  if ((!xmlStrcmp(sulChild->name, (const xmlChar *)"SUL_BAND")) && (sulChild->ns == ns)) 
                  {
                     sulValue = atoi((char *)xmlNodeListGetString(doc, sulChild->xmlChildrenNode, 1));
                     if (sulIdx < MAX_NRCELL_BANDS) 
                     {
                        freqBand->sulBand[sulIdx] = sulValue;
                        sulIdx++;
                     } 
                     else 
                     {
                        DU_LOG("ERROR  -->  DU_APP : %s :  SUL_BAND array overflow\n", __func__);
                        return RFAILED;
                     }
                  }
                  sulChild = sulChild->next;
               }
            }
            child = child->next;
         }
      }
      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill Frequency Band List
 *
 * @details
 *
 *    Function : parseF1FreqBandList
 *
 *    Functionality: Fill Frequency Band List
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseF1FreqBandList(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, F1NrFreqInfo *nrFreqInfo)
{
   uint8_t idx = 0;

   xmlNodePtr child;
   cur = cur->xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"LIST")) && (cur->ns == ns))
      {
         child = cur->xmlChildrenNode;
         while(child != NULL)
         {
            if ((!xmlStrcmp(child->name, (const xmlChar *)"F1_FREQ_BAND")) && (child->ns == ns))
            {
               if(parseF1FreqBand(doc, ns, child, &nrFreqInfo->freqBand[idx]) != ROK)
               {
                  return RFAILED;
               }
               idx++;
            }
            child = child -> next;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill Transmission Bandwidth
 *
 * @details
 *
 *    Function : parseF1TxBw
 *
 *    Functionality: Fill Transmission Bandwidth
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseF1TxBw(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, F1TxBw *txBw)
{
   memset(txBw, 0, sizeof(F1TxBw));
   cur = cur->xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_NR_SCS")) && (cur->ns == ns))
      {
         txBw->nrScs = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_NRB")) && (cur->ns == ns))
      {
         txBw->nrb =  atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill SUL Info
 *
 * @details
 *
 *    Function : parseF1SulInfo
 *
 *    Functionality: Fill SUL Info
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseF1SulInfo(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, F1SulInfo *sulInfo)
{
   memset(sulInfo, 0, sizeof(F1SulInfo));
   cur = cur->xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SUL_ARFCN")) && (cur->ns == ns))
      {
         sulInfo->sulArfcn = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_TX_BW")) && (cur->ns == ns))
      {
         if(parseF1TxBw(doc, ns, cur, &sulInfo->sulTxBw) != ROK)
         {
            return RFAILED;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill NR Frequency Info 
 *
 * @details
 *
 *    Function : parseF1NrFreqInfo
 *
 *    Functionality: Fill NR Frequency Info
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseF1NrFreqInfo(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, F1NrFreqInfo *nrFreqInfo)
{
   memset(nrFreqInfo, 0, sizeof(F1NrFreqInfo));
   cur = cur->xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NR_ARFCN")) && (cur->ns == ns))
      {
         nrFreqInfo->nrArfcn = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_SUL_INFO")) && (cur->ns == ns))
      {
         if(parseF1SulInfo(doc, ns, cur, &nrFreqInfo->sulInfo) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_FREQ_BAND_LIST")) && (cur->ns == ns))
      {
         if(parseF1FreqBandList(doc, ns, cur, nrFreqInfo) != ROK)
         {
            return RFAILED;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill NR FDD Info
 *
 * @details
 *
 *    Function : parseF1NrFddInfo 
 *
 *    Functionality: Fill NR FDD Info
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseF1NrFddInfo(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, F1NrFddInfo *fddInfo)
{
   memset(fddInfo, 0, sizeof(F1NrFddInfo));
   cur = cur->xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_NR_FREQ_INFO_UL")) && (cur->ns == ns))
      {
         if(parseF1NrFreqInfo(doc, ns, cur, &fddInfo->ulNrFreqInfo) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_NR_FREQ_INFO_DL")) && (cur->ns == ns))
      {
         if(parseF1NrFreqInfo(doc, ns, cur, &fddInfo->dlNrFreqInfo) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_TX_BW_UL")) && (cur->ns == ns))
      {
         if(parseF1TxBw(doc, ns, cur, &fddInfo->ulTxBw) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_TX_BW_DL")) && (cur->ns == ns))
      {
         if(parseF1TxBw(doc, ns, cur, &fddInfo->dlTxBw) != ROK)
         {
            return RFAILED;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill NR TDD Info
 *
 * @details
 *
 *    Function : parseF1NrTddInfo
 *
 *    Functionality: Fill NR TDD Info
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseF1NrTddInfo(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, F1NrTddInfo *tddInfo)
{
   memset(tddInfo, 0, sizeof(F1NrTddInfo));
   cur = cur->xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_NR_FREQ_INFO")) && (cur->ns == ns))
      {
         if(parseF1NrFreqInfo(doc, ns, cur, &tddInfo->nrFreqInfo) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_TX_BW")) && (cur->ns == ns))
      {
         if(parseF1TxBw(doc, ns, cur, &tddInfo->nrTxBw) != ROK)
         {
            return RFAILED;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill NR Mode Info
 *
 * @details
 *
 *    Function : parseNrModeInfo
 *
 *    Functionality: Fill NR Mode Info
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseNrModeInfo(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, NrModeInfo *nrModeInfo)
{
   char  modeCfg[10];

   memset(nrModeInfo, 0, sizeof(NrModeInfo));
   cur = cur->xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NR_MODE")) && (cur->ns == ns))
      {
         strcpy((char*)modeCfg, (char*)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));      
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_NR_FDD_INFO")) && (cur->ns == ns))
      {
         if(strcmp(modeCfg, "FDD") == 0)
         {
            if(parseF1NrFddInfo(doc, ns, cur, &nrModeInfo->mode.fdd) != ROK)
            {
               return RFAILED;
            }
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_NR_TDD_INFO")) && (cur->ns == ns))
      {
         if(strcmp(modeCfg, "TDD") == 0)
         {
            if(parseF1NrTddInfo(doc, ns, cur, &nrModeInfo->mode.tdd) != ROK)
            {
               return RFAILED;
            }
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill Broadcast PLMN Information
 *
 * @details
 *
 *    Function : parseF1BrdcstPlmnInfo
 *
 *    Functionality: Fill Broadcast PLMN Information
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseF1BrdcstPlmnInfo(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, F1BrdcstPlmnInfo *brdcstPlmnInfo)
{
   memset(brdcstPlmnInfo, 0, sizeof(F1BrdcstPlmnInfo));
   cur = cur->xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PLMN")) && (cur->ns == ns))
      {
         if(parsePlmn(doc, ns, cur, &brdcstPlmnInfo->plmn[0]) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"EXT_PLMN")) && (cur->ns == ns))
      {
         if(parsePlmn(doc, ns, cur, &brdcstPlmnInfo->extPlmn[0]) != ROK)
         {
            return RFAILED;
         }
      } 

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"TAC")) && (cur->ns == ns))
      {
         brdcstPlmnInfo->tac = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NR_CELL_ID")) && (cur->ns == ns))
      {
         brdcstPlmnInfo->nrCellId = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NR_RANAC")) && (cur->ns == ns))
      {
         brdcstPlmnInfo->ranac = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill DU Cell Information 
 *
 * @details
 *
 *    Function : parseF1DuCellInfo
 *
 *    Functionality: Fill DU Cell Information
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseF1DuCellInfo(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, F1DuCellInfo *duCellInfo)
{
   memset(duCellInfo, 0, sizeof(F1DuCellInfo));
   cur = cur->xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_CELL_INFO")) && (cur->ns == ns))
      {
         if(parseF1CellInfo(doc, ns, cur, &duCellInfo->cellInfo) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"TAC")) && (cur->ns == ns))
      {
         duCellInfo->tac = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"EPS_TAC")) && (cur->ns == ns))
      {
         duCellInfo->epsTac = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NR_MODE_INFO")) && (cur->ns == ns))
      {
         if(parseNrModeInfo(doc, ns, cur, &duCellInfo->f1Mode) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"TIME_CFG")) && (cur->ns == ns))
      {
         duCellInfo->measTimeCfgDuration = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_CELL_DIR")) && (cur->ns == ns))
      {
         duCellInfo->cellDir = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_CELL_TYPE")) && (cur->ns == ns))
      {
         duCellInfo->cellType = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_BRDCST_PLMN_INFO")) && (cur->ns == ns))
      {
         if(parseF1BrdcstPlmnInfo(doc, ns, cur, &duCellInfo->brdcstPlmnInfo[0]) != ROK)
         {
            return RFAILED;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill DU served cell information 
 *
 * @details
 *
 *    Function : parseF1DuServedCellInfo
 *
 *    Functionality: Fill DU served cell information
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseF1DuServedCellInfo(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, F1DuSrvdCellInfo *srvdCellInfo)
{
   memset(srvdCellInfo, 0, sizeof(F1DuSrvdCellInfo));
   cur = cur->xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_DU_CELL_INFO")) && (cur->ns == ns))
      {
         if(parseF1DuCellInfo(doc, ns, cur, &srvdCellInfo->duCellInfo) != ROK)
         {
            return RFAILED;
         }
      }
      cur = cur -> next;
   }

   if(fillDuSrvdCellSysInfo(&srvdCellInfo->duSysInfo) != ROK)
   {
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill DU Served Cell System Information
 *
 * @details
 *
 *    Function : fillDuSrvdCellSysInfo
 *
 *    Functionality: Fill DU Served Cell System Information
 *
 * @params[in] Served Cell System Information
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillDuSrvdCellSysInfo(F1DuSysInfo *sysInfo)
{
   /* GNB DU System Info MIB msg */
   BuildMibMsg();
   DU_ALLOC(sysInfo->mibMsg, encBufSize);
   if(!(sysInfo->mibMsg))
   {
      DU_LOG("\nERROR  -->  DU APP : %s : Memory allocation failure at line %d", __func__, __LINE__);
      return RFAILED;
   }
   memcpy(sysInfo->mibMsg, encBuf, encBufSize);
   sysInfo->mibLen = encBufSize;

   /* GNB DU System Info SIB1 msg */
   BuildSib1Msg();
   DU_ALLOC(sysInfo->sib1Msg, encBufSize);
   if(!(sysInfo->sib1Msg))
   {
      DU_LOG("\nERROR  -->  DU APP : %s : Memory allocation failure at line %d", __func__, __LINE__);
      return RFAILED;
   }
   memcpy(sysInfo->sib1Msg, encBuf,encBufSize);
   sysInfo->sib1Len = encBufSize;

   return ROK;
}

#ifdef O1_ENABLED
/*******************************************************************
 *
 * @brief Fill PLMN received from O1 interface
 *
 * @details
 *
 *    Function : fillPlmnFromO1
 *
 *    Functionality: Fill PLMN received from O1 interface
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void fillPlmnFromO1(Plmn *PLMN, uint8_t srvdPlmnIdx)
{
   PLMN->mcc[0] = cellParams.plmnList[srvdPlmnIdx].mcc[0];
   PLMN->mcc[1] = cellParams.plmnList[srvdPlmnIdx].mcc[1];
   PLMN->mcc[2] = cellParams.plmnList[srvdPlmnIdx].mcc[2];
   PLMN->mnc[0] = cellParams.plmnList[srvdPlmnIdx].mnc[0];
   PLMN->mnc[1] = cellParams.plmnList[srvdPlmnIdx].mnc[1];
   PLMN->mnc[2] = cellParams.plmnList[srvdPlmnIdx].mnc[2];
}
#endif

/*******************************************************************
 *
 * @brief Fill Beamforming Configuration
 *
 * @details
 *
 *    Function : parseBeamformingConfig
 *
 *    Functionality: Fill Beamforming Configuration
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseBeamformingConfig(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, BeamformingConf *beamformingCfg)
{
   memset(beamformingCfg, 0, sizeof(BeamformingConf));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_OF_BEAMS")) && (cur->ns == ns))
      {
         beamformingCfg->numOfBeams = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_TX_RUS")) && (cur->ns == ns))
      {
         beamformingCfg->numTxRUs = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"BEAM_IDX")) && (cur->ns == ns))
      {
         beamformingCfg->beamIdx = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"BEAM_TYPE")) && (cur->ns == ns))
      {
         beamformingCfg->beamType = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"BEAM_AZIMUTH")) && (cur->ns == ns))
      {
         beamformingCfg->beamAzimuth = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"BEAM_TILT")) && (cur->ns == ns))
      {
         beamformingCfg->beamTilt = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"BEAM_HORIZ_WIDTH")) && (cur->ns == ns))
      {
         beamformingCfg->beamHorizWidth = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"BEAM_VERT_WIDTH")) && (cur->ns == ns))
      {
         beamformingCfg->beamVertWidth = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"COVER_SHAPE")) && (cur->ns == ns))
      {
         beamformingCfg->coverageShape = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"DIGI_TILT")) && (cur->ns == ns))
      {
         beamformingCfg->digitalTilt = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"DIGI_AZIMUTH")) && (cur->ns == ns))
      {
         beamformingCfg->digitalAzimuth = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill Precoding Configuration
 *
 * @details
 *
 *    Function : parsePrecodingConfig
 *
 *    Functionality:  Fill Precoding Configuration
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parsePrecodingConfig(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, PrecodingConf *precodCfg)
{
   memset(precodCfg, 0, sizeof(PrecodingConf));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_LAYERS")) && (cur->ns == ns))
      {
         precodCfg->numLayers = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_ANT_PORTS")) && (cur->ns == ns))
      {
         precodCfg->numAntPorts  = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill PRACH FDM Information
 *
 * @details
 *
 *    Function : parsePrachFdmInfo
 *
 *    Functionality: Fill PRACH FDM Information
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parsePrachFdmInfo(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,PrachFdmInfo *prachFdmInfo)
{
   memset(prachFdmInfo, 0, sizeof(PrachFdmInfo));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"ROOT_SEQ_IDX")) && (cur->ns == ns))
      {
         prachFdmInfo->rootSeqIdx = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_ROOT_SEQ")) && (cur->ns == ns))
      {
         prachFdmInfo->numRootSeq = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"K1")) && (cur->ns == ns))
      {
         prachFdmInfo->k1 = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"ZERO_CORR_ZONE_CFG")) && (cur->ns == ns))
      {
         prachFdmInfo->zeroCorrZoneCfg = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill PRACH configuration
 *
 * @details
 *
 *    Function : parsePrachCfg
 *
 *    Functionality: Fill PRACH configuration
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parsePrachCfg(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, PrachCfg *prachCfg)
{
   xmlNodePtr child;
   uint8_t fdmIdx = 0;
   uint8_t maxNumRbs = 0;
   uint8_t prachMaxPrb = 0;

   memset(prachCfg, 0, sizeof(PrachCfg));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PRACH_SEQ_LEN")) && (cur->ns == ns))
      {
         prachCfg->prachSeqLen = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NR_SCS")) && (cur->ns == ns))
      {
         prachCfg->prachSubcSpacing = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PRACH_CONFIG_IDX")) && (cur->ns == ns))
      {
         prachCfg->prachCfgIdx = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_PRACH_FDM")) && (cur->ns == ns))
      {
         prachCfg->msg1Fdm = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"FDM_LIST")) && (cur->ns == ns))
      {
         child = cur->xmlChildrenNode;
         while(child != NULL)
         {
            if ((!xmlStrcmp(child->name, (const xmlChar *)"FDM_INFO")) && (child->ns == ns))
            {
               if(parsePrachFdmInfo(doc, ns, child, &prachCfg->fdm[fdmIdx]) != ROK)
               {
                  return RFAILED;
               }
               fdmIdx++;
            }
            child = child -> next;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PRACH_RESTRICTED_SET_CFG")) && (cur->ns == ns))
      {
         prachCfg->prachRstSetCfg = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SSB_PER_RACH")) && (cur->ns == ns))
      {
         prachCfg->ssbPerRach = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_RA_PREAMBLE")) && (cur->ns == ns))
      {
         prachCfg->totalNumRaPreamble = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CB_PREAMBLE_PER_SSB")) && (cur->ns == ns))
      {
         prachCfg->totalNumRaPreamble = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MAX_NUM_RB")) && (cur->ns == ns))
      {
         maxNumRbs = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PRACH_MAX_PRB")) && (cur->ns == ns))
      {
         prachMaxPrb = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"RSRP_THRESHOLD_SSB")) && (cur->ns == ns))
      {
         prachCfg->rsrpThreshSsb = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"RA_RSP_WINDOW")) && (cur->ns == ns))
      {
         prachCfg->raRspWindow = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }

   prachCfg->msg1FreqStart = maxNumRbs - prachMaxPrb;
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill CSI RS configuration
 *
 * @details
 *
 *    Function : parseCsiRsCfg
 *
 *    Functionality: Fill CSI RS configuration
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseCsiRsCfg(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, CsiRsCfg *csiRsCfg)
{
   memset(csiRsCfg, 0, sizeof(CsiRsCfg));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CSIRS_FREQ")) && (cur->ns == ns))
      {
         csiRsCfg->csiFreqDomainAlloc = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CSIRS_PORTS")) && (cur->ns == ns))
      {
         csiRsCfg->csiNrofPorts = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CSIRS_OFDM_PORT")) && (cur->ns == ns))
      {
         csiRsCfg->csirsfirstOFDMSymbolInTimeDomain = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CSIRS_OFDM_PORT_2")) && (cur->ns == ns))
      {
         csiRsCfg->csirsfirstOFDMSymbolInTimeDomain2 = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CSIRS_DM_TYPE")) && (cur->ns == ns))
      {
         csiRsCfg->csirscdmType = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CSIRS_DENSITY")) && (cur->ns == ns))
      {
         csiRsCfg->csirsdensity = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CSIRS_DENSITY_DOT_5")) && (cur->ns == ns))
      {
         csiRsCfg->csirsdensitydot5 = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"POWER_CONTROL_OFFSET")) && (cur->ns == ns))
      {
         csiRsCfg->powerControlOffset = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"POWER_CONTROL_OFFSET_SS")) && (cur->ns == ns))
      {
         csiRsCfg->powerControlOffsetSS = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PERIODICITY_OFFSET")) && (cur->ns == ns))
      {
         csiRsCfg->periodicityAndOffset = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill SSB Configuration
 *
 * @details
 *
 *    Function : parseSsbCfg
 *
 *    Functionality: Fill SSB Configuration
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseSsbCfg(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, SsbCfg *ssbCfg)
{
   xmlNodePtr child;
   uint8_t ssbMaskIdx = 0;

   memset(ssbCfg, 0, sizeof( SsbCfg));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SSB_PBSC_PWR")) && (cur->ns == ns))
      {
         ssbCfg->ssbPbchPwr = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SCS_CMN")) && (cur->ns == ns))
      {
         ssbCfg->scsCmn = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SSB_OFF_PT_A")) && (cur->ns == ns))
      {
         ssbCfg->ssbOffsetPointA = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SSB_PERIOD")) && (cur->ns == ns))
      {
         ssbCfg->ssbPeriod = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SSB_SC_OFF")) && (cur->ns == ns))
      {
         ssbCfg->ssbScOffset = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SSB_LIST")) && (cur->ns == ns))
      {
         child = cur -> xmlChildrenNode;
         while(child != NULL)
         {
            if ((!xmlStrcmp(child->name, (const xmlChar *)"SSB_MASK")) && (child->ns == ns))
            {
               ssbCfg->ssbMask[ssbMaskIdx] = atoi((char *)xmlNodeListGetString(doc, child->xmlChildrenNode, 1));
               ssbMaskIdx++;
            }
            child = child -> next;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"BEAM_ID")) && (cur->ns == ns))
      {
         ssbCfg->beamId[0] = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"BETA_PSS")) && (cur->ns == ns))
      {
         ssbCfg->betaPss = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"BCH_PAY_FLAG")) && (cur->ns == ns))
      {
         ssbCfg->bchPayloadFlag = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"DMRS_TYPE_A_PROS")) && (cur->ns == ns))
      {
         ssbCfg->dmrsTypeAPos = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill Carrier Configuration
 *
 * @details
 *
 *    Function : parseCarrierCfg
 *
 *    Functionality: Fill Carrier Configuration
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseCarrierCfg(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,CarrierCfg *carrierCfg)
{
   memset(carrierCfg, 0, sizeof(CarrierCfg));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"DL_BW")) && (cur->ns == ns))
      {
         carrierCfg->dlBw = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NR_DL_ARFCN")) && (cur->ns == ns))
      {
         carrierCfg->arfcnDL = convertArfcnToFreqKhz(atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1)));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"UL_BW")) && (cur->ns == ns))
      {
         carrierCfg->ulBw = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NR_UL_ARFCN")) && (cur->ns == ns))
      {
         carrierCfg->arfcnUL = convertArfcnToFreqKhz(atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1)));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_TX_ANT")) && (cur->ns == ns))
      {
         carrierCfg->numTxAnt = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_RX_ANT")) && (cur->ns == ns))
      {
         carrierCfg->numRxAnt = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill PLMN Information List
 *
 * @details
 *
 *    Function : parsePlmnInfo
 *
 *    Functionality: Fill PLMN Information List
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parsePlmnInfo(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,PlmnInfoList *plmnInfoList)
{

   memset(plmnInfoList, 0, sizeof(PlmnInfoList));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PLMN")) && (cur->ns == ns))
      {
         if(parsePlmn(doc, ns, cur,&plmnInfoList->plmn) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_SLICE_SUPP_LST")) && (cur->ns == ns))
      {
         if(parseSupportedSliceList(doc, ns, cur,&plmnInfoList -> suppSliceList) != ROK)
         {
            return RFAILED;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill PUCCH Configuration Common
 *
 * @details
 *
 *    Function : parsePucchConfigCommon
 *
 *    Functionality: Fill PUCCH Configuration Common
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parsePucchConfigCommon(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, PucchConfigCommon *pucchCfgCmn)
{
   memset(pucchCfgCmn, 0, sizeof(PucchConfigCommon));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PUCCH_RSRC_COMMON")) && (cur->ns == ns))
      {
         pucchCfgCmn->pucchResourceCommon = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PUCCH_GROUP_HOPPING")) && (cur->ns == ns))
      {
         pucchCfgCmn->pucchGroupHopping = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill PUSCH Common Time Allocation
 *
 * @details
 *
 *    Function :  parsePuschTimeDomRsrcAlloc
 *
 *    Functionality: Fill PUSCH Common Time Allocation
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parsePuschTimeDomRsrcAlloc(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,PuschTimeDomRsrcAlloc *puschTimeDomRsrsAlloc)
{
   memset(puschTimeDomRsrsAlloc, 0, sizeof(PuschTimeDomRsrcAlloc));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PUSCH_K2_CFG")) && (cur->ns == ns))
      {
         puschTimeDomRsrsAlloc->k2 = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PUSCH_MAPPING_TYPE")) && (cur->ns == ns))
      {
         puschTimeDomRsrsAlloc->mappingType = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PUSCH_START_SYMBOL")) && (cur->ns == ns))
      {
         puschTimeDomRsrsAlloc->startSymbol = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PUSCH_LENGTH_SYMBOL")) && (cur->ns == ns))
      {
         puschTimeDomRsrsAlloc->symbolLength= atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }

   puschTimeDomRsrsAlloc -> startSymbolAndLength = \
      calcSliv(puschTimeDomRsrsAlloc->startSymbol, puschTimeDomRsrsAlloc->symbolLength);
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill PUSCH Configuration Common 
 *
 * @details
 *
 *    Function : parsePuschConfigCommon
 *
 *    Functionality: Fill PUSCH Configuration Common
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parsePuschConfigCommon(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,PuschConfigCommon *puschCfgCmn)
{
   uint8_t idx = 0;
   xmlNodePtr child = NULLP;
   xmlNodePtr pdschNode = NULLP;

   memset(puschCfgCmn, 0, sizeof(PuschConfigCommon));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_TIME_DOM_RSRC_ALLOC")) && (cur->ns == ns))
      {
         puschCfgCmn->numTimeDomRsrcAlloc = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PUSCH_COMM_TIME_ALLOC_LIST")) && (cur->ns == ns))
      {
         child = cur->xmlChildrenNode;
         while(child != NULL)
         {
            if ((!xmlStrcmp(cur->name, (const xmlChar *)"LIST")) && (cur->ns == ns))
            {
               pdschNode = child->xmlChildrenNode;
               while(pdschNode != NULL)
               {
                  if ((!xmlStrcmp(pdschNode->name, (const xmlChar *)"PUSCH_COMM_TIME_ALLOC")) && (pdschNode->ns == ns))
                  {
                     if(parsePuschTimeDomRsrcAlloc(doc, ns, child,&puschCfgCmn->timeDomRsrcAllocList[idx]) != ROK)
                     {
                        return RFAILED;
                     }
                     idx++;
                  }
                  pdschNode = pdschNode -> next;
               }
            }

            child = child -> next;
         }
      }
      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill BWP Configuration
 *
 * @details
 *
 *    Function : parseBwp
 *
 *    Functionality: Fill BWP Configuration
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseBwp(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,BwpParams *bwp)
{
   memset(bwp, 0, sizeof(BwpParams));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"FIRST_PRB")) && (cur->ns == ns))
      {
         bwp->firstPrb = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_PRB")) && (cur->ns == ns))
      {
         bwp->numPrb = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NR_SCS")) && (cur->ns == ns))
      {
         bwp->scs = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NORMAL_CYCLIC_PREFIX")) && (cur->ns == ns))
      {
         bwp->cyclicPrefix = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill UL BWP Configuration
 *
 * @details
 *
 *    Function : parseBwpULConfig 
 *
 *    Functionality: Fill UL BWP Configuration
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseBwpULConfig(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,BwpUlConfig *bwpUlCfg)
{
   memset(bwpUlCfg, 0, sizeof(BwpUlConfig));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"BWP_PARAMS")) && (cur->ns == ns))
      {
         if(parseBwp(doc, ns, cur, &bwpUlCfg->bwp) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PUCCH_CFG_COMMON")) && (cur->ns == ns))
      {
         if(parsePucchConfigCommon(doc, ns, cur, &bwpUlCfg->pucchCommon) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PUSCH_CFG_COMMON")) && (cur->ns == ns))
      {
         if(parsePuschConfigCommon(doc, ns, cur, &bwpUlCfg->puschCommon) != ROK)
         {
            return RFAILED;
         }
      }
      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill Page Configuration
 *
 * @details
 *
 *    Function : parsePageCfg
 *
 *    Functionality: Fill Page Configuration
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parsePageCfg(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, SchPageCfg *pageCfg)
{
   char *poPresent;

   memset(pageCfg, 0, sizeof(SchPageCfg));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_PO")) && (cur->ns == ns))
      {
         pageCfg->numPO = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PO_PRESENT")) && (cur->ns == ns))
      {
         poPresent = (char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
         if(!strcmp(poPresent, "TRUE"))
         {
            pageCfg->poPresent = true;
         }
         else
         {
            pageCfg->poPresent = false;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PAGING_OCC")) && (cur->ns == ns))
      {
         pageCfg->pagingOcc[0] = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill SIB1 PDCCH Configuration
 *
 * @details
 *
 *    Function : parsePdcchCfgSib1
 *
 *    Functionality: Fill SIB1 PDCCH Configuration
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parsePdcchCfgSib1(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,PdcchConfigSib1 *pdcchConfigSib1)
{
   memset(pdcchConfigSib1, 0, sizeof(PdcchConfigSib1));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CORESET_ZERO_INDEX")) && (cur->ns == ns))
      {
         pdcchConfigSib1->coresetZeroIndex = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SEARCH_SPACE_ZERO_INDEX")) && (cur->ns == ns))
      {
         pdcchConfigSib1->searchSpaceZeroIndex = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill SIB1 Cell Configuration
 *
 * @details
 *
 *    Function : parseSib1CellCfg 
 *
 *    Functionality: Fill SIB1 Cell Configuration
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseSib1CellCfg(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, Sib1CellCfg *sib1CellCfg)
{
   memset(sib1CellCfg, 0, sizeof( Sib1CellCfg));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SCH_PAGE_CFG")) && (cur->ns == ns))
      {
         if(parsePageCfg(doc, ns, cur, &sib1CellCfg->pagingCfg) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PDCCH_CONFIG_SIB1")) && (cur->ns == ns))
      {
         if(parsePdcchCfgSib1(doc, ns, cur, &sib1CellCfg->pdcchCfgSib1) != ROK)
         {
            return RFAILED;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill Aggregation Level Candidates Information
 *
 * @details
 *
 *    Function : parseCandidateInfo 
 *
 *    Functionality: Fill Aggregation Level Candidates Information
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseCandidateInfo(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,CandidatesInfo *candInfo)
{
   memset(candInfo, 0, sizeof(CandidatesInfo));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"AGG_LEVEL1")) && (cur->ns == ns))
      {
         candInfo->aggLevel1 = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"AGG_LEVEL2")) && (cur->ns == ns))
      {
         candInfo->aggLevel2 = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"AGG_LEVEL4")) && (cur->ns == ns))
      {
         candInfo->aggLevel4 = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"AGG_LEVEL8")) && (cur->ns == ns))
      {
         candInfo->aggLevel8 = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"AGG_LEVEL16")) && (cur->ns == ns))
      {
         candInfo->aggLevel16 = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill Search Space Connfiguration
 *
 * @details
 *
 *    Function : parseSearchSpaceCfg
 *
 *    Functionality: Fill Search Space Configuration
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseSearchSpaceCfg(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,SearchSpaceCfg *searchSpaceCfg)
{
   memset(searchSpaceCfg, 0, sizeof(SearchSpaceCfg));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SEARCHSPACE_1_INDEX")) && (cur->ns == ns))
      {
         searchSpaceCfg->searchSpaceId = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CORESET_0_INDEX")) && (cur->ns == ns))
      {
         searchSpaceCfg->coresetId = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SS_MONITORING_SLOT_SL1")) && (cur->ns == ns))
      {
         searchSpaceCfg->monitoringSlot = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"DURATION")) && (cur->ns == ns))
      {
         searchSpaceCfg->duration = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SS_MONITORING_SYMBOL")) && (cur->ns == ns))
      {
         searchSpaceCfg->monitoringSymbol = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CANDIDATE_INFO")) && (cur->ns == ns))
      {
         if(parseCandidateInfo(doc, ns, cur, &searchSpaceCfg->candidate) != ROK)
         {
            return RFAILED;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill PDCCH Configuration Common
 *
 * @details
 *
 *    Function : parsePdcchCfgCommon
 *
 *    Functionality: Fill PDCCH Configuration Common
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parsePdcchCfgCommon(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,PdcchConfigCommon *pdcchCfgCm)
{
   memset(pdcchCfgCm, 0, sizeof(PdcchConfigCommon));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SEARCH_SPACE_CFG")) && (cur->ns == ns))
      {
         if(parseSearchSpaceCfg(doc, ns, cur, &pdcchCfgCm->commonSearchSpace) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SEARCHSPACE_1_INDEX")) && (cur->ns == ns))
      {
         pdcchCfgCm->raSearchSpaceId = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Fill PDSCH Common Time Domain Resource Allocation
 *
 * @details
 *
 *    Function : parsePdschCmnTimeDomRsrcAlloc
 *
 *    Functionality: Fill PDSCH Common Time Domain Resource Allocation
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parsePdschCmnTimeDomRsrcAlloc(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,\
      PdschCfgCommTimeDomRsrcAlloc *pdschTimeDomRsrcAlloc)
{
   memset(pdschTimeDomRsrcAlloc, 0, sizeof(PdschCfgCommTimeDomRsrcAlloc));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PDSCH_K0_CFG")) && (cur->ns == ns))
      {
         pdschTimeDomRsrcAlloc->k0 = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PDSCH_MAPPING_TYPE")) && (cur->ns == ns))
      {
         pdschTimeDomRsrcAlloc->mappingType = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PDSCH_START_SYMBOL")) && (cur->ns == ns))
      {
         pdschTimeDomRsrcAlloc->startSymbol = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PDSCH_LENGTH_SYMBOL")) && (cur->ns == ns))
      {
         pdschTimeDomRsrcAlloc->lengthSymbol = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill PDSCH Configuration Common
 *
 * @details
 *
 *    Function : parsePdschConfigCommon
 *
 *    Functionality: Fill PDSCH Configuration Common
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parsePdschConfigCommon(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, PdschConfigCommon *pdschCfgCmn)
{
   uint8_t idx = 0;
   xmlNodePtr child = NULLP;
   xmlNodePtr pdschNode = NULLP;

   memset(pdschCfgCmn, 0, sizeof(PdschConfigCommon));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_TIME_DOM_RSRC_ALLOC")) && (cur->ns == ns))
      {
         pdschCfgCmn->numTimeDomAlloc = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PDSCH_COMM_TIME_ALLOC_LIST")) && (cur->ns == ns))
      {
         child = cur->xmlChildrenNode;
         while(child != NULL)  
         {
            if ((!xmlStrcmp(cur->name, (const xmlChar *)"LIST")) && (cur->ns == ns))
            {
               pdschNode = child->xmlChildrenNode;
               while(pdschNode != NULL)
               {
                  if ((!xmlStrcmp(pdschNode->name, (const xmlChar *)"PDSCH_COMM_TIME_ALLOC")) && (pdschNode->ns == ns))
                  {   
                     if(parsePdschCmnTimeDomRsrcAlloc(doc, ns, child, &pdschCfgCmn->timeDomRsrcAllocList[idx]) != ROK)
                     {
                        return RFAILED;
                     }
                     idx++;
                  }
                  pdschNode = pdschNode -> next;
               }
            }

            child = child -> next;
         }
      }
      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill DL BWP Configuration
 *
 * @details
 *
 *    Function : parseBwpDLConfig
 *
 *    Functionality: Fill DL BWP Configuration
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseBwpDLConfig(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,BwpDlConfig *bwpDlCfg)
{
   memset(bwpDlCfg, 0, sizeof(BwpDlConfig));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"BWP_PARAMS")) && (cur->ns == ns))
      {
         if(parseBwp(doc, ns, cur, &bwpDlCfg->bwp) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PDCCH_CFG_COMMON")) && (cur->ns == ns))
      {
         if(parsePdcchCfgCommon(doc, ns, cur, &bwpDlCfg->pdcchCommon) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PDSCH_CFG_COMMON")) && (cur->ns == ns))
      {
         if(parsePdschConfigCommon(doc, ns, cur, &bwpDlCfg->pdschCommon) != ROK)
         {
            return RFAILED;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill Cell Configuration
 *
 * @details
 *
 *    Function : parseCellCfg
 *
 *    Functionality: Fill Cell Configuration
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseCellCfg(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,CellCfg *cellCfg)
{
   memset(cellCfg, 0, sizeof(CellCfg));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MAC_OP_STATE")) && (cur->ns == ns))
      {
         cellCfg->opState = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MAC_ADMIN_STATE")) && (cur->ns == ns))
      {
         cellCfg->adminState = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MAC_CELL_STATE")) && (cur->ns == ns))
      {
         cellCfg->cellState = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PLMN_INFO")) && (cur->ns == ns))
      {
         if(parsePlmnInfo(doc, ns, cur, &cellCfg->plmnInfoList[0]) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NR_PCI")) && (cur->ns == ns))
      {
         cellCfg->phyCellId = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"TAC")) && (cur->ns == ns))
      {
         cellCfg->tac = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SSB_FREQUENCY")) && (cur->ns == ns))
      {
         cellCfg->ssbFreq = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NR_SCS")) && (cur->ns == ns))
      {
         cellCfg->subCarrSpacing  = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"DUPLEX_MODE")) && (cur->ns == ns))
      {
         cellCfg->dupType  = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SIB1_CELL_CFG")) && (cur->ns == ns))
      {
         if(parseSib1CellCfg(doc, ns, cur, &cellCfg->sib1Cfg) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"BWP_DL_CFG")) && (cur->ns == ns))
      {
         if(parseBwpDLConfig(doc, ns, cur, &cellCfg->initialDlBwp) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"BWP_UL_CFG")) && (cur->ns == ns))
      {
         if(parseBwpULConfig(doc, ns, cur, &cellCfg->initialUlBwp) != ROK)
         {
            return RFAILED;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

#ifdef NR_TDD
/*******************************************************************
 *
 * @brief Fill TDD slot configuration
 *
 * @details
 *
 *    Function : parseTddCfg
 *
 *    Functionality: Fill TDD slot configuration
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseTddCfg(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, TDDCfg *tddCfg)
{
   memset(tddCfg, 0, sizeof(TDDCfg));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"TDD_PERIODICITY")) && (cur->ns == ns))
      {
         tddCfg->tddPeriod = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_DL_SLOTS")) && (cur->ns == ns))
      {
         tddCfg->nrOfDlSlots = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_DL_SYMBOLS")) && (cur->ns == ns))
      {
         tddCfg->nrOfDlSymbols = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_UL_SLOTS")) && (cur->ns == ns))
      {
         tddCfg->nrOfUlSlots = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_UL_SYMBOLS")) && (cur->ns == ns))
      {
         tddCfg->nrOfUlSymbols = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}
#endif

/*******************************************************************
 *
 * @brief Fill MAC Cell Configuration
 *
 * @details
 *
 *    Function : parseMacCellCfg
 *
 *    Functionality: Fill MAC Cell Configuration
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseMacCellCfg(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,MacCellCfg *macCellCfg)
{
   memset(macCellCfg, 0, sizeof(MacCellCfg));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CELL_ID")) && (cur->ns == ns))
      {
         macCellCfg->cellId = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CARRIER_CFG")) && (cur->ns == ns))
      {
         if(parseCarrierCfg(doc, ns, cur, &macCellCfg->carrCfg) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CELL_CFG")) && (cur->ns == ns))
      {
         if(parseCellCfg(doc, ns, cur, &macCellCfg->cellCfg) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SSB_CFG")) && (cur->ns == ns))
      {
         if(parseSsbCfg(doc, ns, cur, &macCellCfg->ssbCfg) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CSIRS_CFG")) && (cur->ns == ns))
      {
         if(parseCsiRsCfg(doc, ns, cur, &macCellCfg->csiRsCfg) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PRACH_CFG")) && (cur->ns == ns))
      {
         if(parsePrachCfg(doc, ns, cur, &macCellCfg->prachCfg) != ROK)
         {
            return RFAILED;
         }
      }

#ifdef NR_TDD
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"TDD_CFG")) && (cur->ns == ns))
      {
         if(parseTddCfg(doc, ns, cur, &macCellCfg->tddCfg) != ROK)
         {
            return RFAILED;
         }
      }
#endif 

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PRE_CODE_CFG")) && (cur->ns == ns))
      {
         if(parsePrecodingConfig(doc, ns, cur, &macCellCfg->precodingConf) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"BEAM_FORM_CFG")) && (cur->ns == ns))
      {
         if(parseBeamformingConfig(doc, ns, cur, &macCellCfg->beamCfg) != ROK)
         {
            return RFAILED;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill PUSCH Configuration Common Time Domain 
 *    Resource Allocation
 *
 * @details
 *
 *    Function : parsePuschCmnTimeDomRsrcAlloc
 *
 *    Functionality: Fill PUSCH Configuration Common Time Domain
 *       Resource Allocation
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parsePuschCmnTimeDomRsrcAlloc(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,PuschCfgCmnTimeDomAlloc *puschCmnTimeDomAlloc)
{
   uint16_t startSymbol;
   uint16_t lenSymbol;

   memset(puschCmnTimeDomAlloc, 0, sizeof(PuschCfgCmnTimeDomAlloc));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"K2")) && (cur->ns == ns))
      {
         puschCmnTimeDomAlloc->k2 = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MAP_TYPE")) && (cur->ns == ns))
      {
         puschCmnTimeDomAlloc->mapType = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PUSCH_START_SYMBOL")) && (cur->ns == ns))
      {
         startSymbol = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PUSCH_LENGTH_SYMBOL")) && (cur->ns == ns))
      {
         lenSymbol = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      cur = cur -> next;
   }
   puschCmnTimeDomAlloc->sliv = calcSliv(startSymbol, lenSymbol);
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill PUSCH Configuration Common
 *
 * @details
 *
 *    Function : parsePuschCfgCommon
 *
 *    Functionality: Fill PUSCH Configuration Common
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parsePuschCfgCommon(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,PuschCfgCommon *puschCfgCmn)
{
   uint8_t rsrcIdx = 0;
   xmlNodePtr child = NULLP;
   xmlNodePtr rsrcNode = NULLP;

   memset(puschCfgCmn, 0, sizeof(PuschCfgCommon));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PUSCH_CFG_PRESENT")) && (cur->ns == ns))
      {
         puschCfgCmn->puschCfgPresent = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PUSCH_MSG3_DELTA_PREAMBLE")) && (cur->ns == ns))
      {
         puschCfgCmn->msg3DeltaPreamble = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PUSCH_P0_NOMINAL_WITH_GRANT")) && (cur->ns == ns))
      {
         puschCfgCmn->p0NominalWithGrant = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_TIME_DOM_RSRC_ALLOC")) && (cur->ns == ns))
      {
         puschCfgCmn->numTimeDomRsrcAlloc = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PUSCH_TIME_DOM_RSRC_ALLOC_LIST")) && (cur->ns == ns))
      {
         child = cur->xmlChildrenNode;
         while (child != NULL)
         {
            if ((!xmlStrcmp(child->name, (const xmlChar *)"LIST")) && (child->ns == ns))
            {
               rsrcNode = child->xmlChildrenNode;
               while (rsrcNode != NULL)
               {
                  if ((!xmlStrcmp(rsrcNode->name, (const xmlChar *)"PUSCH_TIME_DOM_RSRC_ALLOC")) \
                     && (rsrcNode->ns == ns))
                  {
                     if(parsePuschCmnTimeDomRsrcAlloc(doc, ns, rsrcNode, &puschCfgCmn->timeDomAllocList[rsrcIdx]) != ROK)
                     {
                        return RFAILED;
                     }
                     rsrcIdx++;
                  }
                  rsrcNode = rsrcNode->next;
               }
            }
            child = child->next;
         }
      }
      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill PUCCH Configuration Common 
 *
 * @details
 *
 *    Function : parsePucchCfgCommon
 *
 *    Functionality: Fill PUCCH Configuration Common
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parsePucchCfgCommon(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,PucchCfgCommon *pucchCfgCmn)
{
   memset(pucchCfgCmn, 0, sizeof(PucchCfgCommon));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PRESENT")) && (cur->ns == ns))
      {
         pucchCfgCmn->present = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PUCCH_RSRC_COMMON")) && (cur->ns == ns))
      {
         pucchCfgCmn->rsrcComm = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"GRP_HOP")) && (cur->ns == ns))
      {
         pucchCfgCmn->grpHop = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PUCCH_P0_NOMINAL")) && (cur->ns == ns))
      {
         pucchCfgCmn->p0Nominal = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill RACH Configuration Common
 *
 * @details
 *
 *    Function : parseRachCfgCommon
 *
 *    Functionality: Fill RACH Configuration Common
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseRachCfgCommon(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, RachCfgCommon *rachCfgCmn)
{
   long   maxNumRb;
   long   prachMaxPrb;

   memset(rachCfgCmn, 0, sizeof(RachCfgCommon));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PRESENT")) && (cur->ns == ns))
      {
         rachCfgCmn->present = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PRACH_CONFIG_IDX")) && (cur->ns == ns))
      {
         rachCfgCmn->prachCfgIdx = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MSG_1_FDM")) && (cur->ns == ns))
      {
         rachCfgCmn->msg1Fdm = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MAX_NUM_RB")) && (cur->ns == ns))
      {
         maxNumRb = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PRACH_MAX_PRB")) && (cur->ns == ns))
      {
         prachMaxPrb = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"ZERO_CORRELATION_ZONE_CFG")) && (cur->ns == ns))
      {
         rachCfgCmn->zeroCorrZoneCfg = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PRACH_PREAMBLE_RCVD_TGT_PWR")) && (cur->ns == ns))
      {
         rachCfgCmn->preambleRcvdTgtPwr = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PREAMBLE_TRANS_MAX")) && (cur->ns == ns))
      {
         rachCfgCmn->preambleTransMax = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PWR_RAMPING_STEP")) && (cur->ns == ns))
      {
         rachCfgCmn->pwrRampingStep = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"RA_RSP_WINDOW")) && (cur->ns == ns))
      {
         rachCfgCmn->raRspWindow = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_RA_PREAMBLE")) && (cur->ns == ns))
      {
         rachCfgCmn->numRaPreamble = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_SSB_PER_RACH_OCC")) && (cur->ns == ns))
      {
         rachCfgCmn->numSsbPerRachOcc = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CB_PREAMBLE_PER_SSB")) && (cur->ns == ns))
      {
         rachCfgCmn->numCbPreamblePerSsb = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CONT_RES_TIMER")) && (cur->ns == ns))
      {
         rachCfgCmn->contResTimer = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"RSRP_THRESHOLD_SSB")) && (cur->ns == ns))
      {
         rachCfgCmn->rsrpThreshSsb = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"ROOT_SEQ_IDX_PRESENT")) && (cur->ns == ns))
      {
         rachCfgCmn->rootSeqIdxPresent = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"ROOT_SEQ_IDX")) && (cur->ns == ns))
      {
         rachCfgCmn->rootSeqIdx = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PRACH_SUBCARRIER_SPACING")) && (cur->ns == ns))
      {
         rachCfgCmn->msg1Scs = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PRACH_RESTRICTED_SET_CFG")) && (cur->ns== ns))
      {
         rachCfgCmn->restrictedSetCfg = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }

   rachCfgCmn->msg1FreqStart = ((maxNumRb) - (prachMaxPrb));
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill SCS Specific Carrier
 *
 * @details
 *
 *    Function : parseScsSpecCarrier
 *
 *    Functionality: Fill SCS Specific Carrier
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseScsSpecCarrier(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,ScsSpecCarrier *scsSpecCrr)
{
   memset(scsSpecCrr, 0, sizeof(ScsSpecCarrier));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SSB_SUBCARRIER_OFFSET")) && (cur->ns == ns))
      {
         scsSpecCrr->scsOffset = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NR_SCS")) && (cur->ns == ns))
      {
         scsSpecCrr->scs = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SCS_BW")) && (cur->ns == ns))
      {
         scsSpecCrr->scsBw = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill UL Config Common 
 *
 * @details
 *
 *    Function : parseUlCfgCommon
 *
 *    Functionality: Fill UL Config Common
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseUlCfgCommon(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,UlCfgCommon *ulCfgCmn)
{
   memset(ulCfgCmn, 0, sizeof(UlCfgCommon));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NR_FREQ_BAND")) && (cur->ns == ns))
      {
         ulCfgCmn->freqBandInd = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"UL_P_MAX")) && (cur->ns == ns))
      {
         ulCfgCmn->pMax = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"FREQ_LOC_BW")) && (cur->ns == ns))
      {
         ulCfgCmn->locAndBw = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"TIME_ALLIGN_TIMER_COMM")) && (cur->ns == ns))
      {
         ulCfgCmn->timeAlignTimerComm = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SCS_SPEC_CARRIER")) && (cur->ns == ns))
      {
         if(parseScsSpecCarrier(doc, ns, cur, &ulCfgCmn->ulScsCarrier) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"RACH_CFG_COMMON")) && (cur->ns == ns))
      {
         if(parseRachCfgCommon(doc, ns, cur, &ulCfgCmn->rachCfg) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PUSCH_CFG_COMMON")) && (cur->ns == ns))
      {
         if(parsePuschCfgCommon(doc, ns, cur, &ulCfgCmn->puschCfg) != ROK)
         {
            return RFAILED;
         }
      }
      
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PUCCH_CFG_COMMON")) && (cur->ns == ns))
      {
         if(parsePucchCfgCommon(doc, ns, cur, &ulCfgCmn->pucchCfg) != ROK)
         {
            return RFAILED;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill TDD UL DL Configuration Common
 *
 * @details
 *
 *    Function : parseTddUlDlCfgCommon
 *
 *    Functionality: Fill TDD UL DL Configuration Common
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseTddUlDlCfgCommon(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, TddUlDlCfgCommon *tddUlDlCfgCmn)
{
   memset(tddUlDlCfgCmn, 0, sizeof(TddUlDlCfgCommon));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"REF_SCS")) && (cur->ns == ns))
      {
         tddUlDlCfgCmn->refScs = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"TX_PRD")) && (cur->ns == ns))
      {
         tddUlDlCfgCmn->txPrd = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_DL_SLOTS")) && (cur->ns == ns))
      {
         tddUlDlCfgCmn->numDlSlots = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_DL_SYMBOLS")) && (cur->ns == ns))
      {
         tddUlDlCfgCmn->numDlSymbols = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_UL_SLOTS")) && (cur->ns == ns))
      {
         tddUlDlCfgCmn->numUlSlots = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_UL_SYMBOLS")) && (cur->ns == ns))
      {
         tddUlDlCfgCmn->numUlSymbols = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill PCCH Configuration 
 *
 * @details
 *
 *    Function : parsePcchCfg
 *
 *    Functionality: Fill PCCH Configuration
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parsePcchCfg(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,PcchCfg *pcchCfg)
{
   xmlNodePtr child = NULLP;
   xmlNodePtr firstPdcchNode = NULLP;
   uint8_t  idx = 0;

   memset(pcchCfg, 0, sizeof(PcchCfg));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"DFLT_PAGING_CYCLE")) && (cur->ns == ns))
      {
         pcchCfg->dfltPagingCycle = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NAND_PAGING_FRM_OFFSET")) && (cur->ns == ns))
      {
         pcchCfg->nAndPagingFrmOffsetType = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PAGE_FRM_OFFSET")) && (cur->ns == ns))
      {
         pcchCfg->pageFrameOffset = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NS")) && (cur->ns == ns))
      {
         pcchCfg->ns = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"FIRST_PDCCH_MONITORING_TYPE")) && (cur->ns == ns))
      {
         pcchCfg->firstPDCCHMontioringType = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"LIST")) && (cur->ns == ns))
      {
         child = cur->xmlChildrenNode;
         while(child != NULL)
         {
            if ((!xmlStrcmp(child->name, (const xmlChar *)"FIRST_PDCCH_LIST")) && (child->ns == ns))
            {
               firstPdcchNode = child->xmlChildrenNode;
               while (firstPdcchNode != NULL)
               {
                  if ((!xmlStrcmp(firstPdcchNode->name, (const xmlChar *)"FIRST_PDCCH_MONITORING_INFO")) && (firstPdcchNode->ns == ns))
                  {
                     pcchCfg->firstPDCCHMontioringInfo[idx] = atoi((char *)xmlNodeListGetString(doc, firstPdcchNode->xmlChildrenNode, 1));
                     idx++;
                  }
                  firstPdcchNode = firstPdcchNode->next;
               }
            }
            child = child -> next;
         }
      }
      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill PDSCH Time Domain Resource Allocation
 *
 * @details
 *
 *    Function : parsePdschTimeDomRsrcAlloc
 *
 *    Functionality: Fill PDSCH Time Domain Resource Allocation
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parsePdschTimeDomRsrcAlloc(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,PdschTimeDomainRsrcAlloc *pdschTimeDomRsrcAlloc)
{
   uint16_t startSymbol;
   uint16_t lenSymbol;

   memset(pdschTimeDomRsrcAlloc, 0, sizeof(PdschTimeDomainRsrcAlloc));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"K0")) && (cur->ns == ns))
      {
         pdschTimeDomRsrcAlloc->k0 = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MAP_TYPE")) && (cur->ns == ns))
      {
         pdschTimeDomRsrcAlloc->mapType = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PDSCH_START_SYMBOL")) && (cur->ns == ns))
      {
         startSymbol = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PDSCH_LENGTH_SYMBOL")) && (cur->ns == ns))
      {
         lenSymbol = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   pdschTimeDomRsrcAlloc->sliv = calcSliv(startSymbol, lenSymbol);
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill PDSCH Configuration Common
 *
 * @details
 *
 *    Function : parsePdschCfgCommon
 *
 *    Functionality: Fill PDSCH Configuration Common
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parsePdschCfgCommon(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,PdschCfgCommon *pdschCfgCmn)
{
   uint8_t     idx = 0;
   xmlNodePtr  child = NULLP; 
   xmlNodePtr  listChild = NULLP; 

   memset(pdschCfgCmn, 0, sizeof(PdschCfgCommon));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PRESENT")) && (cur->ns == ns))
      {
         pdschCfgCmn->present = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_TIME_DOM_RSRS_ALLOC")) && (cur->ns == ns))
      {
         pdschCfgCmn->numTimeDomRsrcAlloc = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PDSCH_TIME_DOM_RSRC_ALLOC_LIST")) && (cur->ns == ns))
      {
         child = cur->xmlChildrenNode;
         while (child != NULL)
         {
            if ((!xmlStrcmp(child->name, (const xmlChar *)"LIST")) && (child->ns == ns))
            {
               listChild = child->xmlChildrenNode;
               while (listChild != NULL)
               {
                  if ((!xmlStrcmp(listChild->name, (const xmlChar *)"PDSCH_TIME_DOM_RSRC_ALLOC")) && (listChild->ns == ns))
                  {
                     if(parsePdschTimeDomRsrcAlloc(doc, ns, listChild, &pdschCfgCmn->timeDomAlloc[idx]) != ROK)
                     {
                        return RFAILED;
                     }
                     idx++;
                  }
                  listChild = listChild->next;
               }
            }
            child = child->next;
         }
      }
      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill BCCH Configuration
 *
 * @details
 *
 *    Function : parseBcchCfg
 *
 *    Functionality: Fill BCCH Configuration
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseBcchCfg(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, BcchCfg *bcchCfg)
{
   memset(bcchCfg, 0, sizeof(BcchCfg));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MOB_PRD_COEFF")) && (cur->ns == ns))
      {
         bcchCfg->modPrdCoeff = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }
      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill PDCCH Configuration Common
 *
 * @details
 *
 *    Function : parsePdcchConfigCommon
 *
 *    Functionality: Fill PDCCH Configuration Common
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parsePdcchConfigCommon(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, PdcchCfgCommon *pdccgCfgCmn)
{
   uint8_t idx = 0;
   xmlNodePtr child = NULLP;
   xmlNodePtr listChild = NULLP;

   memset(pdccgCfgCmn, 0, sizeof( PdcchCfgCommon));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PRESENT")) && (cur->ns == ns))
      {
         pdccgCfgCmn->present = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CORESET_0_INDEX")) && (cur->ns == ns))
      {
         pdccgCfgCmn->ctrlRsrcSetZero = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SEARCHSPACE_0_INDEX")) && (cur->ns == ns))
      {
         pdccgCfgCmn->searchSpcZero = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PDCCH_SEARCH_SPACE_ID")) && (cur->ns == ns))
      {
         pdccgCfgCmn->searchSpcId = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PDCCH_CTRL_RSRC_SET_ID")) && (cur->ns == ns))
      {
         pdccgCfgCmn->ctrlRsrcSetId = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MONITOR_SLOT_PRD_OFFPRESENT")) && (cur->ns == ns))
      {
         pdccgCfgCmn->monitorSlotPrdAndOffPresent = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"LIST")) && (cur->ns == ns))
      {
         child = cur->xmlChildrenNode;
         while (child)
         {
            if ((!xmlStrcmp(child->name, (const xmlChar *)"MONITOR_LIST")) && (child->ns == ns))
            {
               listChild = child->xmlChildrenNode;
               while (listChild != NULL)
               {
                  if ((!xmlStrcmp(listChild->name, (const xmlChar *)"MONITOR_SYMBOL_INSLOT")) && (listChild->ns == ns))
                  {
                     pdccgCfgCmn->monitorSymbolsInSlot[idx] = atoi((char *)xmlNodeListGetString(doc, listChild->xmlChildrenNode, 1));
                     idx++;
                  }
                  listChild = listChild->next;
               }
            }
            child = child->next;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUMC_AGG_LVL1")) && (cur->ns == ns))
      {
         pdccgCfgCmn->numCandAggLvl1 = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUMC_AGG_LVL2")) && (cur->ns == ns))
      {
         pdccgCfgCmn->numCandAggLvl2 = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUMC_AGG_LVL4")) && (cur->ns == ns))
      {
         pdccgCfgCmn->numCandAggLvl4 = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUMC_AGG_LVL8")) && (cur->ns == ns))
      {
         pdccgCfgCmn->numCandAggLvl8 = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUMC_AGG_LVL16")) && (cur->ns == ns))
      {
         pdccgCfgCmn->numCandAggLvl16 = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SEARCH_SPC_TYPE")) && (cur->ns == ns))
      {
         pdccgCfgCmn->searchSpcType = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PDCCH_SERACH_SPACE_DCI_FORMAT")) && (cur->ns == ns))
      {
         pdccgCfgCmn->commSrchSpcDciFrmt = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PDCCH_SEARCH_SPACE_ID_SIB1")) && (cur->ns == ns))
      {
         pdccgCfgCmn->searchSpcSib1 = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PDCCH_SEARCH_SPACE_ID_PAGING")) && (cur->ns == ns))
      {
         pdccgCfgCmn->pagingSearchSpc = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"RA_PDCCH_SEARCH_SPACE_ID_PAGING")) && (cur->ns == ns))
      {
         pdccgCfgCmn->raSearchSpc = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill DL Configuration Common
 *
 * @details
 *
 *    Function : parseDlCfgCommon
 *
 *    Functionality: Fill DL Configuration Common
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseDlCfgCommon(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,DlCfgCommon *dlCfgCmn)
{
   memset(dlCfgCmn, 0, sizeof(DlCfgCommon));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NR_FREQ_BAND")) && (cur->ns == ns))
      {
         dlCfgCmn->freqBandInd = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"OFFSET_TO_POINT_A")) && (cur->ns == ns))
      {
         dlCfgCmn->offsetToPointA = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"FREQ_LOC_BW")) && (cur->ns == ns))
      {
         dlCfgCmn->locAndBw = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SCS_SPEC_CARRIER")) && (cur->ns == ns))
      {
         if(parseScsSpecCarrier(doc, ns, cur, &dlCfgCmn->dlScsCarrier) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PDCCH_CFG_COMMON")) && (cur->ns == ns))
      {
         if(parsePdcchConfigCommon(doc, ns, cur, &dlCfgCmn->pdcchCfg) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PDSCH_CFG_COMMON")) && (cur->ns == ns))
      {
         if(parsePdschCfgCommon(doc, ns, cur, &dlCfgCmn->pdschCfg) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"BCCH_CFG")) && (cur->ns == ns))
      {
         if(parseBcchCfg(doc, ns, cur, &dlCfgCmn->bcchCfg) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PCCH_CFG")) && (cur->ns == ns))
      {
         if(parsePcchCfg(doc, ns, cur, &dlCfgCmn->pcchCfg) != ROK)
         {
            return RFAILED;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}
/*******************************************************************
 *
 * @brief Fill Serving Cell Config Common SIB
 *
 * @details
 *
 *    Function : parseSrvCellCfgCmnSib
 *
 *    Functionality: Fill Serving Cell Config Common SIB
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseSrvCellCfgCmnSib(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,SrvCellCfgCommSib *srvCellCfgCmnSib)
{
   memset(srvCellCfgCmnSib, 0, sizeof(SrvCellCfgCommSib));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NR_SCS")) && (cur->ns == ns))
      {
         srvCellCfgCmnSib->scs = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SSB_POS_INBURST")) && (cur->ns == ns))
      {
         srvCellCfgCmnSib->ssbPosInBurst = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SSB_PERIODICITY")) && (cur->ns == ns))
      {
         srvCellCfgCmnSib->ssbPrdServingCell = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SSB_PBCH_PWR")) && (cur->ns == ns))
      {
         srvCellCfgCmnSib->ssPbchBlockPwr = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"DL_CFG_COMMON")) && (cur->ns == ns))
      {
         if(parseDlCfgCommon(doc, ns, cur, &srvCellCfgCmnSib->dlCfg) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"UL_CFG_COMMON")) && (cur->ns == ns))
      {
         if(parseUlCfgCommon(doc, ns, cur, &srvCellCfgCmnSib->ulCfg) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"TDD_UL_DL_CFG_COMMON")) && (cur->ns == ns))
      {
         if(parseTddUlDlCfgCommon(doc, ns, cur, &srvCellCfgCmnSib->tddCfg) != ROK)
         {
            return RFAILED;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill SI Scheduling Information 
 *
 * @details
 *
 *    Function : parseSiSchedInfo
 *
 *    Functionality: Fill SI Scheduling Information
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseSiSchedInfo(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,SiSchedInfo *siSchedInfo)
{
   memset(siSchedInfo, 0, sizeof(SiSchedInfo));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"WIN_LEN")) && (cur->ns == ns))
      {
         siSchedInfo->winLen = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"BROADCAST_STA")) && (cur->ns == ns))
      {
         siSchedInfo->broadcastSta = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PERIODICITY")) && (cur->ns == ns))
      {
         siSchedInfo->preiodicity = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SIB_TYPE")) && (cur->ns == ns))
      {
         siSchedInfo->sibType = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SIB1_VAL_TAG")) && (cur->ns == ns))
      {
         siSchedInfo->sibValTag = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill SIB1 Parameters
 *
 * @details
 *
 *    Function : parseSib1Params
 *
 *    Functionality: Fill SIB1 Parameters
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseSib1Params(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,Sib1Params *sib1Params)
{
   memset(sib1Params, 0, sizeof(Sib1Params));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PLMN")) && (cur->ns == ns))
      {
         if(parsePlmn(doc, ns, cur, &sib1Params->plmn) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"TAC")) && (cur->ns == ns))
      {
         sib1Params-> tac = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"RANAC")) && (cur->ns == ns))
      {
         sib1Params->ranac = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CELL_IDENTITY")) && (cur->ns == ns))
      {
         sib1Params->cellIdentity = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CELL_RESVD_OPUSE")) && (cur->ns == ns))
      {
         sib1Params->cellResvdForOpUse = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CONN_EST_FAIL_CNT")) && (cur->ns == ns))
      {
         sib1Params->connEstFailCnt = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CONN_EST_FAIL_OFF_VALID")) && (cur->ns == ns))
      {
         sib1Params->connEstFailOffValidity = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CONN_EST_FAIL_OFFSET")) && (cur->ns == ns))
      {
         sib1Params->connEstFailOffset = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SI_SHED_INFO")) && (cur->ns == ns))
      {
         if(parseSiSchedInfo(doc, ns, cur, &sib1Params->siSchedInfo) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SRV_CELLCFG_COM_SIB")) && (cur->ns == ns))
      {
         if(parseSrvCellCfgCmnSib(doc, ns, cur, &sib1Params->srvCellCfgCommSib) != ROK)
         {
            return RFAILED;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill RRM Policy List
 *
 * @details
 *
 *    Function : parseRrmPolicyList
 *
 *    Functionality: Fill RRM Policy List
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseRrmPolicyList(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,RrmPolicyMemberList *rrmPolicyMemberList)
{
   memset(rrmPolicyMemberList, 0, sizeof(RrmPolicyMemberList));
   cur = cur->xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"PLMN")) && (cur->ns == ns))
      {
         if(parsePlmn(doc, ns, cur, &rrmPolicyMemberList->plmn) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SNSSAI")) && (cur->ns == ns))
      {
         if(parseSnssai(doc, ns, cur, &rrmPolicyMemberList->snssai) != ROK)
         {
            return RFAILED;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill RRM Policy Ratio
 *
 * @details
 *
 *    Function : parseRrmPolicyRatio
 *
 *    Functionality: Fill RRM Policy Ratio
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseRrmPolicyRatio(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, RrmPolicyRatio *rrmPolicyRatio)
{
   memset(rrmPolicyRatio, 0, sizeof(RrmPolicyRatio));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MAX_RATIO")) && (cur->ns == ns))
      {
         rrmPolicyRatio->maxRatio = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MIN_RATIO")) && (cur->ns == ns))
      {
         rrmPolicyRatio->minRatio = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"DEDICATED_RATIO")) && (cur->ns == ns))
      {
         rrmPolicyRatio->dedicatedRatio = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
/*******************************************************************
 *
 * @brief Fill MAC Slice RRM Policy
 *
 * @details
 *
 *    Function : parseMacSliceRrmPolicy
 *
 *    Functionality: Fill MAC Slice RRM Policy
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseMacSliceRrmPolicy(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur, MacSliceRrmPolicy *rrmPolicy)
{
   uint8_t memIdx = 0;

   memset(rrmPolicy, 0, sizeof(MacSliceRrmPolicy));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"RESOURCE_TYPE")) && (cur->ns == ns))
      {
         rrmPolicy->resourceType = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_RRC_POLICY_MEM")) && (cur->ns == ns))
      {
         rrmPolicy->numOfRrmPolicyMem = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"RRM_POLICY_MUM_LIST")) && (cur->ns == ns))
      {
         DU_ALLOC_SHRABL_BUF(rrmPolicy->rRMPolicyMemberList,\
               rrmPolicy->numOfRrmPolicyMem * sizeof(RrmPolicyMemberList*));

         if(!rrmPolicy->rRMPolicyMemberList)
         {
            DU_LOG("\nERROR  --> DU APP : %s: Memory allocation failed at line %d", __func__, __LINE__);
            return RFAILED;
         }

         for(memIdx = 0; memIdx < rrmPolicy->numOfRrmPolicyMem; memIdx++)
         {
            DU_ALLOC_SHRABL_BUF(rrmPolicy->rRMPolicyMemberList[memIdx], sizeof(RrmPolicyMemberList));
            if (rrmPolicy->rRMPolicyMemberList[memIdx] == NULLP)
            {
               DU_LOG("\nERROR  --> DU_APP: %s: Memory allocation failed at line %d", __func__, __LINE__);
               return RFAILED;
            }

            if(parseRrmPolicyList(doc, ns, cur, rrmPolicy->rRMPolicyMemberList[memIdx]) != ROK)
            {
               return RFAILED;
            }
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"RRM_POLICY_RATIO")) && (cur->ns == ns))
      {
         if(parseRrmPolicyRatio(doc, ns, cur, &rrmPolicy->policyRatio) != ROK)
         {
            return RFAILED;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill MAC Slice Configuration
 *
 * @details
 *
 *    Function : parseMacSliceCfgReq
 *
 *    Functionality: Fill MAC Slice Configuration
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 *             Pointer to structure to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseMacSliceCfgReq(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,MacSliceCfgReq *macSliceCfgReq)
{
   uint8_t policyIdx = 0;
   memset(macSliceCfgReq, 0, sizeof(MacSliceCfgReq));
   cur = cur -> xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"NUM_RRC_POLICY")) && (cur->ns == ns))
      {
         macSliceCfgReq->numOfRrmPolicy = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MAC_SLICE_RRM_POLICY")) && (cur->ns == ns))
      {
         DU_ALLOC_SHRABL_BUF(macSliceCfgReq->listOfRrmPolicy,  macSliceCfgReq->numOfRrmPolicy * sizeof(MacSliceRrmPolicy*));
         if(!macSliceCfgReq->listOfRrmPolicy)
         {
            DU_LOG("\nERROR  --> DU APP: %s: Memory allocation failed at line %d", __func__, __LINE__);
            return RFAILED;
         }

         for(policyIdx = 0; policyIdx < macSliceCfgReq->numOfRrmPolicy; policyIdx++)
         {
            DU_ALLOC_SHRABL_BUF(macSliceCfgReq->listOfRrmPolicy[policyIdx], sizeof(MacSliceRrmPolicy));
            if (macSliceCfgReq->listOfRrmPolicy[policyIdx] == NULLP)
            {
               DU_LOG("\nERROR  --> DU_APP: %s: Memory allocation failed at line %d", __func__, __LINE__);
            }
            
            if(parseMacSliceRrmPolicy(doc, ns, cur, macSliceCfgReq->listOfRrmPolicy[policyIdx]) != ROK)
            {
               return RFAILED;
            }
         }
      }
      cur = cur -> next;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill DU Config Parmeters 
 *
 * @details
 *
 *    Function : parseDuCfgParams
 *
 *    Functionality:
 *
 * @params[in] XML document pointer
 *             XML namespace
 *             Current node in XML
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t parseDuCfgParams(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur)
{
   char *tempDuName = "";
   char *duIpV4Addr;
   char *cuIpV4Addr;
   char *ricIpV4Addr;
   CmInetIpAddr duIp;
   CmInetIpAddr cuIp;
   CmInetIpAddr ricIp;

   memset(&duCfgParam, 0, sizeof(struct duCfgParams));
   cur = cur->xmlChildrenNode;
   while(cur != NULL)
   {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MAX_NUM_DRB")) && (cur->ns == ns))
      {
         duCfgParam.maxNumDrb = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MAX_NUM_UE_SUPPORTED")) && (cur->ns == ns))
      {
         duCfgParam.maxSupportedUes = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"DU_ID")) && (cur->ns == ns))
      {
         duCfgParam.duId = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"DU_NAME")) && (cur->ns == ns))
      {
         tempDuName =(char*) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
         DU_ALLOC(duCfgParam.duName, strlen(tempDuName));
         if(!duCfgParam.duName)
         {
            DU_LOG("\nERROR --> DU_APP: %s: Memory allocation failed at line %d", __func__, __LINE__);
            return RFAILED;
         }
         strcpy((char*)duCfgParam.duName, tempDuName);
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MAX_NUM_UE")) && (cur->ns == ns))
      {
         duCfgParam.maxUe = atoi((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
      }

#ifdef O1_ENABLE
      if( getStartupConfig(&g_cfg) != ROK )
      {
         RETVALUE(RFAILED);
      }
      cmInetAddr((S8*)g_cfg.DU_IPV4_Addr, &duIp);
      cmInetAddr((S8*)g_cfg.CU_IPV4_Addr, &cuIp);
      cmInetAddr((S8*)g_cfg.RIC_IPV4_Addr, &ricIp);

#else
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"DU_IP_V4_ADDR")) && (cur->ns == ns))
      {
         duIpV4Addr = (char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
         cmInetAddr(duIpV4Addr, &(duIp));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"CU_IP_V4_ADDR")) && (cur->ns == ns))
      {
         cuIpV4Addr = (char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
         cmInetAddr(cuIpV4Addr, &(cuIp));
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"RIC_IP_V4_ADDR")) && (cur->ns == ns))
      {
         ricIpV4Addr = (char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
         cmInetAddr(ricIpV4Addr, &(ricIp));
      }
#endif

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SCTP")) && (cur->ns == ns))
      {
         if(parseSctpParams(doc, ns, cur, &duCfgParam.sctpParams) != ROK)
         {
            return RFAILED;
         }
         duCfgParam.sctpParams.duIpAddr.ipV4Pres = true;
         duCfgParam.sctpParams.duIpAddr.ipV4Addr = duIp;
         duCfgParam.sctpParams.cuIpAddr.ipV4Pres = true;
         duCfgParam.sctpParams.cuIpAddr.ipV4Addr = cuIp;
         duCfgParam.sctpParams.ricIpAddr.ipV4Pres = true;
         duCfgParam.sctpParams.ricIpAddr.ipV4Addr = ricIp;
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"EGTP")) && (cur->ns == ns))
      {
         if(parseEgtpParams(doc, ns, cur, &duCfgParam.egtpParams) != ROK)
         {
            return RFAILED;
         }
         duCfgParam.egtpParams.localIp.ipV4Addr = duIp;
         duCfgParam.egtpParams.localIp.ipV4Pres = true;
         duCfgParam.egtpParams.destIp.ipV4Pres = true;
         duCfgParam.egtpParams.destIp.ipV4Addr = cuIp;
         duCfgParam.egtpParams.maxTunnelId = duCfgParam.maxNumDrb * duCfgParam.maxSupportedUes; 
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MIB_PARAMS")) && (cur->ns == ns))
      {
         if(parseMibParams(doc, ns, cur, &duCfgParam.mibParams) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_RRC_VERSION")) && (cur->ns == ns))
      {
         if(parseF1RrcVersion(doc, ns, cur, &duCfgParam.rrcVersion) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"F1_DU_SRVD_CELL_INFO")) && (cur->ns == ns))
      {
         if(parseF1DuServedCellInfo(doc, ns, cur, &duCfgParam.srvdCellLst[0]) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"MAC_CELL_CFG")) && (cur->ns == ns))
      {
         if(parseMacCellCfg(doc, ns, cur, &duCfgParam.macCellCfg) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SIB1_PARAMS")) && (cur->ns == ns))
      {
         if(parseSib1Params(doc, ns, cur, &duCfgParam.sib1Params) != ROK)
         {
            return RFAILED;
         }
      }

      if ((!xmlStrcmp(cur->name, (const xmlChar *)"SLICE_CFG")) && (cur->ns == ns))
      {
         if(parseMacSliceCfgReq(doc, ns, cur, &duCfgParam.tempSliceCfg) != ROK)
         {
            return RFAILED;
         }
      }

      cur = cur -> next;
   }
   return ROK;
}
#endif

/*******************************************************************
 *
 * @brief Reads config and posts message to du_app on completion
 *
 * @details
 *
 *    Function : duReadCfg
 *
 *    Functionality:
 *      - Reads configuration from xml file and stores in DB
 *      - Post to du_app for further processing
 *
 * @params[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duReadCfg()
{
#ifdef XML_BASED_CONFIG
   const char *filename = "../build/config/odu_config.xml";
   xmlDocPtr doc = NULLP;
   xmlNodePtr cur = NULLP;
   xmlNsPtr ns = NULLP;

   doc = xmlParseFile(filename);
   if(doc == NULL)
   {
      DU_LOG("\nERROR  --> DU_APP: Failed to parse the XML file");
      return RFAILED;
   }

   cur = xmlDocGetRootElement(doc);
   ns = xmlSearchNsByHref(doc, cur, (const xmlChar *)"odu_config.xml");
   if(ns == NULL)
   {
      DU_LOG("\nERROR  --> DU_APP: XML Namespace not found.\n");
      xmlFreeDoc(doc);
      xmlCleanupParser();
      return RFAILED;
   }

   parseDuCfgParams(doc, ns, cur);

   xmlFreeDoc(doc);
   xmlCleanupParser();
#endif

   Pst pst;
   Buffer *mBuf;

   memset(&duCfgParam, 0, sizeof(DuCfgParams));

   /* Read configs into duCfgParams */
   if(readCfg() != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : Reading configuration failed");
      return RFAILED;
   }
   printDuConfig();

   /* Fill pst structure */
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
   cmInitTimers(&(duCb.e2apDb.e2TimersInfo.e2Timers.ricServiceUpdateTimer.timer), 1);
   cmInitTimers(&(duCb.e2apDb.e2TimersInfo.e2Timers.e2NodeConfigUpdate.timer), 1);

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

/*******************************************************************
 *
 * @brief Prints all DU Configuration
 *
 * @details
 *
 *    Function : printDuConfig
 *
 *    Functionality:
 *      - Prints all DU configurations
 *
 * @params[in] void
 * @return void
 *
 * ****************************************************************/
void printDuConfig()
{
   uint8_t sliceIdx = 0, pfIdx = 0, rsrcIdx = 0, ssbMaskIdx = 0, fdmIdx = 0;
   uint8_t monitoringSymbIdx = 0, poIdx = 0, policyIdx = 0, memIdx = 0;
   __attribute__((unused)) SctpParams *sctp;
   __attribute__((unused)) F1EgtpParams *egtp;
   __attribute__((unused)) MibParams *mib;
   __attribute__((unused)) F1RrcVersion *f1RrcVersion;
   F1DuCellInfo *f1DuCellInfo;
   F1CellInfo *f1CellInfo;
   __attribute__((unused)) F1DuSysInfo *duSysInfo;
   __attribute__((unused)) F1BrdcstPlmnInfo *brdcstPlmnInfo;
   NrModeInfo *nrModeInfo; 
   __attribute__((unused)) F1NrFddInfo *f1NrFddInfo;
   F1SrvdPlmn *srvdPlmn;
   SupportedSliceList *sliceSuppLst;
   MacCellCfg *macCellCfg;
   __attribute__((unused)) CarrierCfg *carrierCfg;
   CellCfg *cellCfg;
   PlmnInfoList *plmnInfoList;
   Sib1CellCfg *sib1Cfg;
   SchPageCfg *pageCfg;
   __attribute__((unused)) PdcchConfigSib1 *pdcchConfigSib1;
   BwpDlConfig *bwpDlCfg;
   __attribute__((unused)) BwpParams *bwp;
   PdcchConfigCommon *pdcchCfgCmn;
   SearchSpaceCfg *searchSpaceCfg;
   __attribute__((unused)) CandidatesInfo *candidateInfo;
   PdschConfigCommon *pdschCfgCmn;
   __attribute__((unused)) PdschCfgCommTimeDomRsrcAlloc *pdschCmnTimeDomRsrcAlloc;
   BwpUlConfig *bwpUlCfg;
   __attribute__((unused)) PucchConfigCommon *pucchCfgCmn;
   PuschConfigCommon *puschCfgCmn;
   __attribute__((unused)) PuschTimeDomRsrcAlloc *puschTimeDomRsrcAlloc;
   __attribute__((unused)) SsbCfg *ssbCfg;
   __attribute__((unused)) CsiRsCfg *csiRsCfg;
   PrachCfg *prachCfg;
   __attribute__((unused)) PrachFdmInfo *prachFdmInfo;
   __attribute__((unused)) PrecodingConf *precodCfg;
   __attribute__((unused)) BeamformingConf *beamFormingCfg;
   Sib1Params *sib1Params;
   __attribute__((unused)) SiSchedInfo *siSchedInfo;
   SrvCellCfgCommSib *srvCellCfgCmnSib;
   DlCfgCommon *dlCfgCmn;
   PdcchCfgCommon *pdcchCfgCommon;
   PdschCfgCommon *pdschCfgCommon;
   __attribute__((unused)) PdschTimeDomainRsrcAlloc *pdschTimeDomRsrcAlloc;
   __attribute__((unused)) BcchCfg *bcchCfg;
   __attribute__((unused)) PcchCfg *pcchCfg;
   UlCfgCommon *ulCfgCmn;
   RachCfgCommon *rachCfgCmn;
   PuschCfgCommon *puschCfgCommon;
   __attribute__((unused)) PuschCfgCmnTimeDomAlloc *puschCmnTimeDomRsrsAlloc;
   PucchCfgCommon *pucchCfgCommon;
   __attribute__((unused)) TddUlDlCfgCommon *tddUlDlCfgCmn;
   MacSliceCfgReq *macSliceCfg;
   MacSliceRrmPolicy *rrmPolicy;
   __attribute__((unused)) RrmPolicyRatio *rrmPolicyRatio;
   __attribute__((unused)) RrmPolicyMemberList *rrmPolicyMemberList;

#ifdef NR_TDD
   F1NrTddInfo *f1NrTddInfo;
   TDDCfg *tddCfg;
#endif

   DU_LOG("\n ** DU CONFIGURATION ** \n");
   DU_LOG("DU ID %d\n", duCfgParam.duId);
   DU_LOG("DU Name %s\n", duCfgParam.duName);
   DU_LOG("MAX NUM DRB %d\n", duCfgParam.maxNumDrb);
   DU_LOG("MAX SUPPORTED UE %d\n", duCfgParam.maxSupportedUes);
   DU_LOG("MAX UE %d\n",duCfgParam.maxUe);

   sctp = &duCfgParam.sctpParams;
   DU_LOG("\n ** SCTP PARAMETER ** \n");
   DU_LOG("DU IPv4 Address present %u\n", sctp->duIpAddr.ipV4Pres);
   DU_LOG("DU IP Address %u\n", sctp->duIpAddr.ipV4Addr);
   DU_LOG("CU IPv4 Address present %u\n", sctp->cuIpAddr.ipV4Pres);
   DU_LOG("CU IP Address %u\n", sctp->cuIpAddr.ipV4Addr);
   DU_LOG("RIC IPv4 Address present %u\n", sctp->ricIpAddr.ipV4Pres);
   DU_LOG("RIC IP Address %u\n", sctp->ricIpAddr.ipV4Addr);
   DU_LOG("SCTP Port at DU for F1 Interface %d\n", sctp->duPort[F1_INTERFACE]);
   DU_LOG("SCTP Port at CU for F1 Interface %d\n", sctp->cuPort);
   DU_LOG("SCTP Port at DU for E2 Interface %d\n", sctp->duPort[E2_INTERFACE]);
   DU_LOG("SCTP Port at RIC for E2 Interface %d\n", sctp->ricPort);

   egtp = &duCfgParam.egtpParams;
   DU_LOG("\n ** EGTP PARAMETER ** \n");
   DU_LOG("DU IP Address %d\n", egtp->localIp.ipV4Addr);
   DU_LOG("CU IP Address %d\n", egtp->destIp.ipV4Addr);
   DU_LOG("EGTP Port at DU %d\n", egtp->localPort);
   DU_LOG("EGTP Port at CU %d\n", egtp->destPort);
   DU_LOG("Minimum Tunnel ID %d\n", egtp->minTunnelId);
   DU_LOG("Maximum Tunnel ID %d\n",egtp->maxTunnelId);

   mib = &duCfgParam.mibParams;
   DU_LOG("\n ** MIB PARAMETER ** \n");
   DU_LOG("System Frame Number %d\n", mib->sysFrmNum);
   DU_LOG("Subcarrier Spacing Common %ld\n", mib->subCarrierSpacingCommon);
   DU_LOG("SSB Subcarrier Offset %ld\n", mib->ssb_SubcarrierOffset);
   DU_LOG("DMRS Type-A Position %ld\n", mib->dmrs_TypeA_Position);
   DU_LOG("Control Resource Set Zero %ld\n", mib->controlResourceSetZero);
   DU_LOG("Search Space Zero %ld\n", mib->searchSpaceZero);
   DU_LOG("Cell Barred %ld\n", mib->cellBarred);
   DU_LOG("Intra Frequency Reselection %ld\n", mib->intraFreqReselection);
   
   f1RrcVersion = &duCfgParam.rrcVersion;   
   DU_LOG("\n ** RRC Version ** \n");
   DU_LOG("RRC Version %s\n", f1RrcVersion->rrcVer);
   DU_LOG("Extended RRC Version %d\n", f1RrcVersion->extRrcVer);

   duSysInfo = &duCfgParam.srvdCellLst[0].duSysInfo;
   DU_LOG("\n ** DU System Information ** \n");
   DU_LOG("MIB length %d\n", duSysInfo->mibLen);
   DU_LOG("MIB message  %s\n", duSysInfo->mibMsg);
   DU_LOG("SIB1 length  %d\n", duSysInfo->sib1Len);
   DU_LOG("SIB1 message  %s\n", duSysInfo->sib1Msg);
   
   f1DuCellInfo = &duCfgParam.srvdCellLst[0].duCellInfo;
   DU_LOG("\n ** DU Cell Information ** \n");

   f1CellInfo = &f1DuCellInfo->cellInfo;
   DU_LOG("NR PCI %d\n", f1CellInfo->nrPci);
   DU_LOG("DU TAC %d\n", f1DuCellInfo->tac);   
   DU_LOG("EPS TAC %d\n", f1DuCellInfo->epsTac);
   DU_LOG("Measurement Time Configuration Duration %d\n", f1DuCellInfo->measTimeCfgDuration);
   DU_LOG("Cell Direction %d\n", f1DuCellInfo->cellDir);
   DU_LOG("Cell Type %d\n", f1DuCellInfo->cellType);

   DU_LOG("\n ** Cell  Info : NR CGI ** \n");
   DU_LOG("Cell Id %d\n",f1CellInfo->nrCgi.cellId);
   DU_LOG("PLMN : mcc[0] %d\n", f1CellInfo->nrCgi.plmn.mcc[0]);
   DU_LOG("PLMN : mcc[1] %d\n", f1CellInfo->nrCgi.plmn.mcc[1]);
   DU_LOG("PLMN : mcc[2] %d\n", f1CellInfo->nrCgi.plmn.mcc[2]);
   DU_LOG("PLMN : mnc[0] %d\n", f1CellInfo->nrCgi.plmn.mnc[0]);
   DU_LOG("PLMN : mnc[1] %d\n", f1CellInfo->nrCgi.plmn.mnc[1]);
   DU_LOG("PLMN : mnc[2] %d\n", f1CellInfo->nrCgi.plmn.mnc[2]);

   srvdPlmn = &f1CellInfo->srvdPlmn[0];
   DU_LOG("\n ** Cell  Info : Served PLMN ** \n");
   DU_LOG("PLMN : mcc[0] %d\n", srvdPlmn->plmn.mcc[0]);
   DU_LOG("PLMN : mcc[1] %d\n", srvdPlmn->plmn.mcc[1]);
   DU_LOG("PLMN : mcc[2] %d\n", srvdPlmn->plmn.mcc[2]);
   DU_LOG("PLMN : mnc[0] %d\n", srvdPlmn->plmn.mnc[0]);
   DU_LOG("PLMN : mnc[1] %d\n", srvdPlmn->plmn.mnc[1]);
   DU_LOG("PLMN : mnc[2] %d\n", srvdPlmn->plmn.mnc[2]);
   DU_LOG("EXT_PLMN : mcc[0] %d\n", srvdPlmn->extPlmn.mcc[0]);
   DU_LOG("EXT_PLMN : mcc[1] %d\n", srvdPlmn->extPlmn.mcc[1]);
   DU_LOG("EXT_PLMN : mcc[2] %d\n", srvdPlmn->extPlmn.mcc[2]);
   DU_LOG("EXT_PLMN : mnc[0] %d\n", srvdPlmn->extPlmn.mnc[0]);
   DU_LOG("EXT_PLMN : mnc[1] %d\n", srvdPlmn->extPlmn.mnc[1]);
   DU_LOG("EXT_PLMN : mnc[2] %d\n", srvdPlmn->extPlmn.mnc[2]);

   sliceSuppLst = &srvdPlmn->taiSliceSuppLst;
   DU_LOG("\n ** Cell  Info : Served PLMN : Supported Slice List ** \n");
   DU_LOG("Number of Slices supported %d\n", sliceSuppLst->numSupportedSlices);
   for (sliceIdx = 0; sliceIdx < sliceSuppLst->numSupportedSlices; sliceIdx++)
   {
      DU_LOG("Slice\n");
      DU_LOG("\tSST %d\n", (*sliceSuppLst->snssai[sliceIdx]).sst);
      DU_LOG("\tSD %d %d %d\n", (*sliceSuppLst->snssai[sliceIdx]).sd[0], (*sliceSuppLst->snssai[sliceIdx]).sd[1], \
         (*sliceSuppLst->snssai[sliceIdx]).sd[2]);
   }
   
   brdcstPlmnInfo = &f1DuCellInfo->brdcstPlmnInfo[0];
   DU_LOG("\n ** DU Cell Information : Broadcast PLMN Info ** \n");
   DU_LOG("TAC %d\n", brdcstPlmnInfo->tac);
   DU_LOG("NR Cell Id %d\n", brdcstPlmnInfo->nrCellId);
   DU_LOG("RANAC %d\n", brdcstPlmnInfo->ranac);

   DU_LOG("PLMN : mcc[0] %d\n", brdcstPlmnInfo->plmn[0].mcc[0]);
   DU_LOG("PLMN : mcc[1] %d\n", brdcstPlmnInfo->plmn[0].mcc[1]);
   DU_LOG("PLMN : mcc[2] %d\n", brdcstPlmnInfo->plmn[0].mcc[2]);
   DU_LOG("PLMN : mnc[0] %d\n", brdcstPlmnInfo->plmn[0].mnc[0]);
   DU_LOG("PLMN : mnc[1] %d\n", brdcstPlmnInfo->plmn[0].mnc[1]);
   DU_LOG("PLMN : mnc[2] %d\n", brdcstPlmnInfo->plmn[0].mnc[2]);
    
   DU_LOG("EXT_PLMN : mcc[0] %d\n", brdcstPlmnInfo->extPlmn[0].mcc[0]);
   DU_LOG("EXT_PLMN : mcc[1] %d\n", brdcstPlmnInfo->extPlmn[0].mcc[1]);
   DU_LOG("EXT_PLMN : mcc[2] %d\n", brdcstPlmnInfo->extPlmn[0].mcc[2]);
   DU_LOG("EXT_PLMN : mnc[0] %d\n", brdcstPlmnInfo->extPlmn[0].mnc[0]);
   DU_LOG("EXT_PLMN : mnc[1] %d\n", brdcstPlmnInfo->extPlmn[0].mnc[1]);
   DU_LOG("EXT_PLMN : mnc[2] %d\n", brdcstPlmnInfo->extPlmn[0].mnc[2]);

   nrModeInfo = &f1DuCellInfo->f1Mode;
   f1NrFddInfo = &nrModeInfo->mode.fdd;
   DU_LOG("\n ** DU Cell Information : NR Mode Info ** \n");
#ifndef NR_TDD   
   DU_LOG("\n ** NR Mode Info : FDD ** \n");
   DU_LOG("\n ** NR Mode Info : FDD : UL NR Frequency ** \n");
   DU_LOG("NR ARFCN %d\n", f1NrFddInfo->ulNrFreqInfo.nrArfcn);
   DU_LOG("SUL : ARFCN %d\n",f1NrFddInfo->ulNrFreqInfo.sulInfo.sulArfcn);
   DU_LOG("SUL : Transmission BW Subcarrier Spacing %d\n", f1NrFddInfo->ulNrFreqInfo.sulInfo.sulTxBw.nrScs);
   DU_LOG("SUL : Number of RBs in Transmission BW %d\n", f1NrFddInfo->ulNrFreqInfo.sulInfo.sulTxBw.nrb);
   DU_LOG("NR Frequency Band %d\n", f1NrFddInfo->ulNrFreqInfo.freqBand[0].nrFreqBand);
   DU_LOG("SUL Band %d\n", f1NrFddInfo->ulNrFreqInfo.freqBand[0].sulBand[0]);

   DU_LOG("\n ** NR Mode Info : FDD : DL NR Frequency ** \n");
   DU_LOG("NR ARFCN %d\n", f1NrFddInfo->dlNrFreqInfo.nrArfcn);
   DU_LOG("SUL : ARFCN %d\n",f1NrFddInfo->dlNrFreqInfo.sulInfo.sulArfcn);
   DU_LOG("SUL : Transmission BW Subcarrier Sapcing %d\n", f1NrFddInfo->dlNrFreqInfo.sulInfo.sulTxBw.nrScs);
   DU_LOG("SUL : Number of RBs in Transmission BW %d\n", f1NrFddInfo->dlNrFreqInfo.sulInfo.sulTxBw.nrb);
   DU_LOG("NR Frequency Band %d\n", f1NrFddInfo->dlNrFreqInfo.freqBand[0].nrFreqBand);
   DU_LOG("SUL Band %d\n", f1NrFddInfo->dlNrFreqInfo.freqBand[0].sulBand[0]);

   DU_LOG("\n ** NR Mode Info : FDD : UL Transmission Bandwidth ** \n");
   DU_LOG("Subscarrier spacing %d\n", f1NrFddInfo->ulTxBw.nrScs);
   DU_LOG("Number of RBs %d\n", f1NrFddInfo->ulTxBw.nrb);
  
   DU_LOG("\n ** NR Mode Info : FDD : DL Transmission Bandwidth ** \n");
   DU_LOG("Subcarrier Spacing %d\n", f1NrFddInfo->dlTxBw.nrScs);
   DU_LOG("Number of RBs %d\n", f1NrFddInfo->dlTxBw.nrb);

#else

   f1NrTddInfo = &nrModeInfo ->mode.tdd; 

   DU_LOG("\n ** NR Mode Info : TDD : NR Frequency ** \n");
   DU_LOG("NR ARFCN %d\n", f1NrTddInfo->nrFreqInfo.nrArfcn);
   DU_LOG("SUL : ARFCN %d\n",f1NrTddInfo->nrFreqInfo.sulInfo.sulArfcn);
   DU_LOG("SUL : Transmission BW Subscarrier Spacing %d\n", f1NrTddInfo->nrFreqInfo.sulInfo.sulTxBw.nrScs);
   DU_LOG("SUL : Number of RBs in Transmission BW %d\n", f1NrTddInfo->nrFreqInfo.sulInfo.sulTxBw.nrb);
   DU_LOG("NR Frequency Band %d\n", f1NrTddInfo->nrFreqInfo.freqBand[0].nrFreqBand);
   DU_LOG("SUL Band %d\n", f1NrTddInfo->nrFreqInfo.freqBand[0].sulBand[0]);

   DU_LOG("\n ** NR Mode Info : TDD : NR Transmission Bandwidth ** \n");
   DU_LOG("Subcarrier Spacing %d\n", f1NrTddInfo->nrTxBw.nrScs);
   DU_LOG("Number of RBs %d\n", f1NrTddInfo->nrTxBw.nrb);
#endif

   macCellCfg = &duCfgParam.macCellCfg;
   DU_LOG("\n ** MAC CELL CONFIGURATION ** \n");
   DU_LOG("Cell Id %d\n", macCellCfg->cellId);

   DU_LOG("\n ** MAC Cell Configuration : Carrier Configuration ** \n");
   carrierCfg = &macCellCfg ->carrCfg;
   DU_LOG("DL Bandwidth %d\n", carrierCfg->dlBw);
   DU_LOG("DL ARFCN %d\n", carrierCfg->arfcnDL);
   DU_LOG("UL Bandwidth %d\n", carrierCfg->ulBw);
   DU_LOG("UL ARFCN %d\n", carrierCfg->arfcnUL);
   DU_LOG("Number of Transmission Antenna %d\n", carrierCfg->numTxAnt);
   DU_LOG("Number of Received Antenna %d\n", carrierCfg->numRxAnt);

   DU_LOG("\n ** MAC Cell Configuration : Cell Configuration ** \n");
   cellCfg = &macCellCfg ->cellCfg;
   DU_LOG("Operational State %d\n", cellCfg->opState);
   DU_LOG("Administration State %d\n", cellCfg->adminState);
   DU_LOG("Cell Current State %d\n", cellCfg->cellState);
   DU_LOG("NR PCI %d\n", cellCfg->phyCellId);
   DU_LOG("TAC: %d\n", cellCfg->tac);
   DU_LOG("SSB Freqeuncy %d\n", cellCfg->ssbFreq);
   DU_LOG("Subcarrier Spacing: %d\n", cellCfg->subCarrSpacing);
   DU_LOG("Duplex Mode %d\n", cellCfg->dupType);

   DU_LOG("\n ** MAC Cell Configuration : Cell Configuration : PLMN Info List ** \n");
   plmnInfoList = &cellCfg ->plmnInfoList[0];
   DU_LOG("PLMN : mcc[0] %d\n", plmnInfoList->plmn.mcc[0]);
   DU_LOG("PLMN : mcc[1] %d\n", plmnInfoList->plmn.mcc[1]);
   DU_LOG("PLMN : mcc[2] %d\n", plmnInfoList->plmn.mcc[2]);
   DU_LOG("PLMN : mnc[0] %d\n", plmnInfoList->plmn.mnc[0]);
   DU_LOG("PLMN : mnc[1] %d\n", plmnInfoList->plmn.mnc[1]);
   DU_LOG("PLMN : mnc[2] %d\n", plmnInfoList->plmn.mnc[2]);

   DU_LOG("Number of Slice supported %d\n", plmnInfoList->suppSliceList.numSupportedSlices);
   for(sliceIdx = 0; sliceIdx < plmnInfoList->suppSliceList.numSupportedSlices; sliceIdx++)
   {
      DU_LOG("Slice\n");
      DU_LOG("\nSST %d\n", (*plmnInfoList->suppSliceList.snssai[sliceIdx]).sst);
      DU_LOG("\nSD %d %d %d\n", (*plmnInfoList->suppSliceList.snssai[sliceIdx]).sd[0], \
         (*plmnInfoList->suppSliceList.snssai[sliceIdx]).sd[1],(*plmnInfoList->suppSliceList.snssai[sliceIdx]).sd[2]);
   }
   
   DU_LOG("\n ** MAC Cell Configuration : Cell Configuration : SIB1 ** \n");
   sib1Cfg = &cellCfg->sib1Cfg;
   DU_LOG("SIB1 PDU Length %d", sib1Cfg->sib1PduLen);
   DU_LOG("SIB1 PDU %s", sib1Cfg->sib1Pdu);

   DU_LOG("\n ** MAC Cell Configuration : Cell Configuration : SIB1 : Paging Configuration ** \n");
   pageCfg = &sib1Cfg->pagingCfg;

   DU_LOG("Number of Paging Occassion %d\n", pageCfg->numPO);
   if (pageCfg->poPresent == true)
      DU_LOG("FirstPDCCH-Monitoring Paging Occassion present : TRUE\n");
   else 
      DU_LOG("FirstPDCCH-Monitoring Paging Occassion present : FALSE\n");

   for(pfIdx = 0; pfIdx < MAX_PO_PER_PF; pfIdx++)
   {
      DU_LOG("Paging Occassion at Paging Frame [%d] : %d\n", pfIdx, pageCfg->pagingOcc[pfIdx]);
   }

   pdcchConfigSib1 = &sib1Cfg->pdcchCfgSib1;
   DU_LOG("\n ** MAC Cell Configuration : Cell Configuration : SIB1 : PDCCH ** \n");
   DU_LOG("Coreset Zero Index %d\n", pdcchConfigSib1->coresetZeroIndex);
   DU_LOG("Search Space Zero Index %d\n", pdcchConfigSib1->searchSpaceZeroIndex);

   bwpDlCfg = &cellCfg->initialDlBwp;
   DU_LOG("\n ** MAC Cell Configuration : Cell Configuration : DL BWP Configuration ** \n");

   DU_LOG("\n ** MAC Cell Configuration : Cell Configuration : DL BWP : BWP ** \n");
   bwp = &bwpDlCfg->bwp;
   DU_LOG("First PRB %d\n", bwp->firstPrb);
   DU_LOG("Number of PRBs %d\n", bwp->numPrb);
   DU_LOG("Subcarrier Spacing %d\n", bwp->scs);
   DU_LOG("Cyclic Perfix %d\n", bwp->cyclicPrefix);

   DU_LOG("\n ** MAC Cell Configuration : Cell Configuration : DL BWP : PDCCH Config Common ** \n");
   pdcchCfgCmn = &bwpDlCfg->pdcchCommon;
   DU_LOG("RA Search Space ID %d\n", pdcchCfgCmn->raSearchSpaceId);

   DU_LOG("\n ** MAC Cell Configuration : Cell Configuration : DL BWP : PDCCH Config Common : Search Space ** \n");
   searchSpaceCfg = &pdcchCfgCmn->commonSearchSpace;
   DU_LOG("Search Space ID %d\n", searchSpaceCfg->searchSpaceId);
   DU_LOG("Coreset ID %d\n", searchSpaceCfg->coresetId);
   DU_LOG("Monitoring Slot %d\n", searchSpaceCfg->monitoringSlot);
   DU_LOG("Duration %d\n", searchSpaceCfg->duration);
   DU_LOG("Monitoring Symbol %x\n", searchSpaceCfg->monitoringSymbol);

   candidateInfo = &searchSpaceCfg->candidate;
   DU_LOG("Number of Candidates in Aggregation Level_1 %d\n", candidateInfo->aggLevel1);
   DU_LOG("Number of Candidates in Aggregation Level_2 %d\n", candidateInfo->aggLevel2);
   DU_LOG("Number of Candidates in Aggregation Level_4 %d\n", candidateInfo->aggLevel4);
   DU_LOG("Number of Candidates in Aggregation Level_8 %d\n", candidateInfo->aggLevel8);
   DU_LOG("Number of Candidates in Aggregation Level_16 %d\n", candidateInfo->aggLevel16);

   DU_LOG("\n ** MAC Cell Configuration : Cell Configuration : DL BWP : PDSCH Config Common ** \n");
   pdschCfgCmn = &bwpDlCfg->pdschCommon;
   DU_LOG("Number of Time Domain Resource Allocation %d\n", pdschCfgCmn->numTimeDomAlloc);
  
   for (rsrcIdx = 0; rsrcIdx <  pdschCfgCmn->numTimeDomAlloc; rsrcIdx++)
   {
      DU_LOG("Time Domain Resource Allocation Idx %d\n", rsrcIdx);
      pdschCmnTimeDomRsrcAlloc = &pdschCfgCmn->timeDomRsrcAllocList[rsrcIdx];
      DU_LOG("\tK0 %d\n", pdschCmnTimeDomRsrcAlloc->k0);
      DU_LOG("\tPDSCH Mapping Type %d\n", pdschCmnTimeDomRsrcAlloc->mappingType);
      DU_LOG("\tPDSCH Start Symbol %d\n", pdschCmnTimeDomRsrcAlloc->startSymbol);
      DU_LOG("\tPDSCH Symbol Length %d\n", pdschCmnTimeDomRsrcAlloc->lengthSymbol);
   }

   DU_LOG("\n ** MAC Cell Configuration : Cell Configuration : UL BWP Configuration ** \n");
   bwpUlCfg = &cellCfg->initialUlBwp;

   DU_LOG("\n ** MAC Cell Configuration : Cell Configuration : UL BWP : BWP ** \n");
   DU_LOG("First PRB %d\n", bwpUlCfg->bwp.firstPrb);
   DU_LOG("Number of PRBs %d\n", bwpUlCfg->bwp.numPrb);
   DU_LOG("Subcarrier Spacing %d\n", bwpUlCfg->bwp.scs);
   DU_LOG("Cyclic Prefix %d\n", bwpUlCfg->bwp.cyclicPrefix);
   
   DU_LOG("\n ** MAC Cell Configuration : Cell Configuration : UL BWP : PUCCH Config Common ** \n");
   pucchCfgCmn = &bwpUlCfg->pucchCommon;
   DU_LOG("PUCCH Resource Common %d\n", pucchCfgCmn->pucchResourceCommon);
   DU_LOG("Group Hopping %d\n", pucchCfgCmn->pucchGroupHopping);

   DU_LOG("\n ** MAC Cell Configuration : Cell Configuration : UL BWP : PUSCH Config Common ** \n");
   puschCfgCmn = &bwpUlCfg ->puschCommon;
   DU_LOG("Number of Time Domain Resource Allocation %d\n", puschCfgCmn->numTimeDomRsrcAlloc);
   for (rsrcIdx = 0; rsrcIdx <  puschCfgCmn->numTimeDomRsrcAlloc; rsrcIdx++)
   {
      DU_LOG("Time Domain Resource Allocation Idx %d\n", rsrcIdx);
      puschTimeDomRsrcAlloc = &puschCfgCmn->timeDomRsrcAllocList[rsrcIdx];
      DU_LOG("\tK2 %d\n", puschTimeDomRsrcAlloc->k2);
      DU_LOG("\tPUSCH Mapping Type %d\n", puschTimeDomRsrcAlloc->mappingType);
      DU_LOG("\tPUSCH Start Symbol %d\n", puschTimeDomRsrcAlloc->startSymbol);
      DU_LOG("\tPUSCH Symbol Length %d\n", puschTimeDomRsrcAlloc->symbolLength);
      DU_LOG("\tPUSCH Start Symbol And Length %d\n", puschTimeDomRsrcAlloc->startSymbolAndLength);
   }

   DU_LOG("\n ** MAC Cell Configuration : SSB Configuration** \n");
   ssbCfg = &macCellCfg ->ssbCfg;
   DU_LOG("SSB PDCH Power %d\n", ssbCfg->ssbPbchPwr);
   DU_LOG("Subcarrier Spacing Common %d\n", ssbCfg->scsCmn);
   DU_LOG("SSB Offset from Point A %d\n", ssbCfg->ssbOffsetPointA);
   DU_LOG("SSB Periodicity in msec %d\n", ssbCfg->ssbPeriod);
   DU_LOG("SSB Subcarrier Offset %d\n", ssbCfg->ssbScOffset);
   for (ssbMaskIdx = 0; ssbMaskIdx < SSB_MASK_SIZE; ssbMaskIdx++)
   {
      DU_LOG("SSB Mask[%d] :%d\n", ssbMaskIdx, ssbCfg->ssbMask[ssbMaskIdx]);
   }
   DU_LOG("Beam ID %d\n", ssbCfg->beamId[0]);
   DU_LOG("BETA PSS %d\n", ssbCfg->betaPss);
   DU_LOG("BCH Payloag Flag %d\n", ssbCfg->bchPayloadFlag);
   DU_LOG("MIB PDU %d %d %d", ssbCfg->mibPdu[0], ssbCfg->mibPdu[1], ssbCfg->mibPdu[2]);
   DU_LOG("DMRS Type-A Position %d\n", ssbCfg->dmrsTypeAPos);

   DU_LOG("\n ** MAC Cell Configuration : CSI RS Configuration ** \n");
   csiRsCfg = &macCellCfg->csiRsCfg;
   DU_LOG("Frequency Domain Allocation %s\n", csiRsCfg->csiFreqDomainAlloc);
   DU_LOG("Number of Ports %d\n", csiRsCfg->csiNrofPorts);
   DU_LOG("First OFDM Symbol in Time Domain %d\n", csiRsCfg->csirsfirstOFDMSymbolInTimeDomain);
   DU_LOG("First OFDM Symbol in Timer Domain 2 %d\n", csiRsCfg->csirsfirstOFDMSymbolInTimeDomain2);
   DU_LOG("CDM Type %d\n", csiRsCfg->csirscdmType);
   DU_LOG("Density %d\n", csiRsCfg->csirsdensity);
   DU_LOG("Density dot 5:%d\n", csiRsCfg->csirsdensitydot5);
   DU_LOG("Power Control Offset %d\n", csiRsCfg->powerControlOffset);
   DU_LOG("Power Control Offset Search Space %d\n", csiRsCfg->powerControlOffsetSS);
   DU_LOG("Periodicity And Offset %d\n", csiRsCfg->periodicityAndOffset);

   DU_LOG("\n ** MAC Cell Configuration : PRACH Configuration ** \n");
   prachCfg = &macCellCfg->prachCfg;
   DU_LOG("PRACH Sequence Length %d\n", prachCfg->prachSeqLen);
   DU_LOG("Subcarrier Spacing %d\n", prachCfg->prachSubcSpacing);
   DU_LOG("Number of PRACH FDM %d\n", prachCfg->msg1Fdm);
   DU_LOG("PRACH Configuration Index %d\n", prachCfg->prachCfgIdx);
   DU_LOG("PRACH Restricted Set Configuration %d\n", prachCfg->prachRstSetCfg);
   DU_LOG("Number of SSB per RACH occassion %d\n", prachCfg->ssbPerRach);
   DU_LOG("Number of RA preambles %d\n", prachCfg->totalNumRaPreamble);
   DU_LOG("Number of Contention Based Preamble per SSB %d\n", prachCfg->numCbPreamblePerSsb);
   DU_LOG("PRACH Frequeny Start Offset %d\n", prachCfg->msg1FreqStart);
   DU_LOG("RA Contention Resolution Timer %d\n", prachCfg->raContResTmr);
   DU_LOG("RSRP Threshold %d\n", prachCfg->rsrpThreshSsb);
   DU_LOG("RA Response Window %d\n", prachCfg->raRspWindow); 
   for(fdmIdx = 0; fdmIdx < prachCfg->msg1Fdm; fdmIdx++)
   {
      prachFdmInfo = &prachCfg->fdm[fdmIdx];
      DU_LOG("FDM[%d]\n", fdmIdx);
      DU_LOG("\tRoot Sequence Index %d\n", prachFdmInfo->rootSeqIdx);
      DU_LOG("\tNumber of Root Sequence %d\n", prachFdmInfo->numRootSeq);
      DU_LOG("\tK1 %d\n", prachFdmInfo->k1);
      DU_LOG("\tZero Correlation Zone Configuration %d\n", prachFdmInfo->zeroCorrZoneCfg);   
   }

#ifdef NR_TDD
   DU_LOG("\n ** MAC Cell Configuration : TDD Configuration ** \n");
   tddCfg = &macCellCfg->tddCfg;
   DU_LOG("TDD Slot Periodicity %d\n", tddCfg->tddPeriod);
   DU_LOG("Number of DL Slots %d\n", tddCfg->nrOfDlSlots);
   DU_LOG("Number of DL Symbols %d\n", tddCfg->nrOfDlSymbols);
   DU_LOG("Number of UL Slots %d\n", tddCfg->nrOfUlSlots);
   DU_LOG("Number of UL Symbols %d\n", tddCfg->nrOfUlSymbols);
#endif

   DU_LOG("\n ** MAC Cell Configuration : Precoding Configuration ** \n");
   precodCfg = &macCellCfg->precodingConf;
   DU_LOG("Number of Layers %d\n", precodCfg->numLayers);
   DU_LOG("Number of Antenna Ports %d\n", precodCfg->numAntPorts);

   DU_LOG("\n ** MAC Cell Config : BEAM FORMING  CFG ** \n");
   beamFormingCfg = &macCellCfg ->beamCfg;
   DU_LOG("Number of Beams %d\n", beamFormingCfg->numOfBeams);
   DU_LOG("Number of Tx RUs %d\n", beamFormingCfg->numTxRUs);
   DU_LOG("Beam Index %d\n", beamFormingCfg->beamIdx);
   DU_LOG("Beam Type %d\n", beamFormingCfg->beamType);
   DU_LOG("Beam Azimuth %d\n", beamFormingCfg->beamAzimuth);
   DU_LOG("Beam Tilt %d\n", beamFormingCfg->beamTilt);
   DU_LOG("Beam Horizontal Width %d\n", beamFormingCfg->beamHorizWidth);
   DU_LOG("Beam Vertical Width %d\n", beamFormingCfg->beamVertWidth);
   DU_LOG("Coverage Shape %d\n", beamFormingCfg->coverageShape);
   DU_LOG("Digitak Tilt %d\n",beamFormingCfg ->digitalTilt);
   DU_LOG("Digital Azimuth %d\n", beamFormingCfg->digitalAzimuth);

   DU_LOG("\n ** SIB1 Configuration ** \n");
   sib1Params = &duCfgParam.sib1Params;
   DU_LOG("TAC %d\n", sib1Params->tac);
   DU_LOG("RANAC %ld\n", sib1Params->ranac);
   DU_LOG("Cell ID %d\n", sib1Params->cellIdentity);
   DU_LOG("Cell Reserved for Operational Use %ld\n", sib1Params->cellResvdForOpUse);
   DU_LOG("Connection Establishment Failure Count %ld\n", sib1Params->connEstFailCnt);
   DU_LOG("Connection Establishment Failure Offset Valid %ld\n", sib1Params->connEstFailOffValidity);
   DU_LOG("Connection Establishment Failure Offset %ld\n", sib1Params->connEstFailOffset);
   DU_LOG("PLMN : mcc[0] %d\n", sib1Params->plmn.mcc[0]);
   DU_LOG("PLMN : mcc[1] %d\n", sib1Params->plmn.mcc[1]);
   DU_LOG("PLMN : mcc[2] %d\n", sib1Params->plmn.mcc[2]);
   DU_LOG("PLMN : mnc[0] %d\n", sib1Params->plmn.mnc[0]);
   DU_LOG("PLMN : mnc[1] %d\n", sib1Params->plmn.mnc[1]);
   DU_LOG("PLMN : mnc[2] %d\n", sib1Params->plmn.mnc[2]);

   DU_LOG("\n ** SIB1 : SI Scheduling Info ** \n");
   siSchedInfo = &sib1Params ->siSchedInfo;
   DU_LOG("Windown Length %ld\n", siSchedInfo->winLen);
   DU_LOG("Broadcast Status %ld\n", siSchedInfo->broadcastSta);
   DU_LOG("Periodicity %ld\n", siSchedInfo->preiodicity);
   DU_LOG("SIB Type %ld\n", siSchedInfo->sibType);
   DU_LOG("SIB Value Tag %ld\n", siSchedInfo->sibValTag);

   DU_LOG("\n ** SIB1 : Serving Cell Configuration Common SIB ** \n");
   srvCellCfgCmnSib = &sib1Params->srvCellCfgCommSib;
   DU_LOG("Subcarrier Spacing %ld\n", srvCellCfgCmnSib->scs);
   DU_LOG("SSB Position in Burst %d\n", srvCellCfgCmnSib->ssbPosInBurst);
   DU_LOG("SSB Periodicity %ld\n", srvCellCfgCmnSib->ssbPrdServingCell);
   DU_LOG("SS PBCH Power %ld\n", srvCellCfgCmnSib->ssPbchBlockPwr);

   DU_LOG("\n ** SIB1 : Serving Cell Config Common SIB : DL Config Common ** \n");
   dlCfgCmn = &srvCellCfgCmnSib->dlCfg;
   DU_LOG("Frequency Band Indicator %ld\n", dlCfgCmn->freqBandInd);
   DU_LOG("Offset to Point A %ld\n", dlCfgCmn->offsetToPointA);
   DU_LOG("Frequency Location And Bandwidth %ld\n", dlCfgCmn->locAndBw);
   DU_LOG("Offset to Point A considering Subcarrier Spacing %ld\n", dlCfgCmn->dlScsCarrier.scsOffset);
   DU_LOG("Subcarrier Spacing %ld\n", dlCfgCmn->dlScsCarrier.scs);
   DU_LOG("Bandwidth considering Subcarrier Spacing %ld\n", dlCfgCmn->dlScsCarrier.scsBw);

   DU_LOG("\n ** SIB1 : Serving Cell Config Common SIB : DL Config Common : PDCCH Config Common ** \n");
   pdcchCfgCommon = &dlCfgCmn->pdcchCfg;
   if(pdcchCfgCommon->present)
   {
      DU_LOG("Control Resource Set Zero Index %ld\n", pdcchCfgCommon->ctrlRsrcSetZero);
      DU_LOG("Search Space Zero Index %ld\n", pdcchCfgCommon->searchSpcZero);
      DU_LOG("Search Space ID %ld\n", pdcchCfgCommon->searchSpcId);
      DU_LOG("Control Resource Set ID %ld\n", pdcchCfgCommon->ctrlRsrcSetId);
      DU_LOG("Monitoring Slot Periodicity and Offset Present %d\n", pdcchCfgCommon->monitorSlotPrdAndOffPresent);
      DU_LOG("Monitoring Slot Periodicity and Offset %ld\n", pdcchCfgCommon->monitorSlotPrdAndOff);
      for (monitoringSymbIdx = 0; monitoringSymbIdx < 2; monitoringSymbIdx++)
      {
         DU_LOG("Monitoring Symbols in Slot [%d] %d\n",monitoringSymbIdx,  pdcchCfgCommon->monitorSymbolsInSlot[monitoringSymbIdx]);
      }
      DU_LOG("Number of Candidates for Aggregation Level_1 %ld\n", pdcchCfgCommon->numCandAggLvl1);
      DU_LOG("Number of Candidates for Aggregation Level_2 %ld\n", pdcchCfgCommon->numCandAggLvl2);
      DU_LOG("Number of Candidates for Aggregation Level_4 %ld\n", pdcchCfgCommon->numCandAggLvl4);
      DU_LOG("Number of Candidates for Aggregation Level_8 %ld\n", pdcchCfgCommon->numCandAggLvl8);
      DU_LOG("Number of Candidates for Aggregation Level_16 %ld\n", pdcchCfgCommon->numCandAggLvl16);
      DU_LOG("Seach Space Type %d\n", pdcchCfgCommon->searchSpcType);
      DU_LOG("Common Search Space DCI Format %d\n", pdcchCfgCommon->commSrchSpcDciFrmt);
      DU_LOG("SIB1 Search Space ID %ld\n", pdcchCfgCommon->searchSpcSib1);
      DU_LOG("Paging Search Space ID %ld\n", pdcchCfgCommon->pagingSearchSpc);
      DU_LOG("RA Search Space ID %ld\n", pdcchCfgCommon->raSearchSpc);
   }
   else
   {
      DU_LOG("PDCCH Config Common not Present");
   }

   DU_LOG("\n ** SIB1 : Serving Cell Config Common : DL Config Common : PDSCH Config Common ** \n");
   pdschCfgCommon = &dlCfgCmn->pdschCfg;
   DU_LOG("PDSCH Config Common Present %d\n", pdschCfgCommon->present);
   if(pdschCfgCommon->present)
   {
      DU_LOG("Number of Time Domain Resource Allocation %d\n", pdschCfgCommon->numTimeDomRsrcAlloc);
      for (rsrcIdx = 0; rsrcIdx < pdschCfgCommon->numTimeDomRsrcAlloc; rsrcIdx++)
      {
         pdschTimeDomRsrcAlloc = &pdschCfgCommon->timeDomAlloc[rsrcIdx];
         DU_LOG("PDSCH Timer Domain Resource Allocation [%d]", rsrcIdx);
         DU_LOG("K0 %ld\n", pdschTimeDomRsrcAlloc->k0);
         DU_LOG("MappingType %ld\n", pdschTimeDomRsrcAlloc->mapType);
         DU_LOG("SLIV %d\n", pdschTimeDomRsrcAlloc->sliv);
      }
   }
   else
   {
      DU_LOG("PDSCH Config Common not Present");
   }

   DU_LOG("\n ** SIB1 : Serving Cell Config Common : DL Config Common : BCCH Config Common ** \n");
   bcchCfg = &dlCfgCmn->bcchCfg;
   DU_LOG("Modification Period Coefficient %ld\n", bcchCfg->modPrdCoeff);

   DU_LOG("\n ** SIB1 : Serving Cell Config Common : DL Config Common : PCCH Config Common ** \n");
   pcchCfg = &dlCfgCmn->pcchCfg;
   DU_LOG("Default Paging Cycle %d\n", pcchCfg->dfltPagingCycle);
   DU_LOG("Number of PF in Paging Cycle %d\n", pcchCfg->nAndPagingFrmOffsetType);
   DU_LOG("PF Offset %d\n", pcchCfg->pageFrameOffset);
   DU_LOG("NS - Number of P0 in PF %d\n", pcchCfg->ns);
   DU_LOG("First PDCCH Monitoring Occassion of each P0 Type %d\n", pcchCfg->firstPDCCHMontioringType);
   for (poIdx = 0; poIdx < MAX_PO_PER_PF; poIdx++)
   {
      DU_LOG("First PDCCH Monitoring Occassion in P0 [%d] %d\n", poIdx, pcchCfg->firstPDCCHMontioringInfo[poIdx]);
   }

   DU_LOG("\n ** SIB1 : Serving Cell Config Common : UL Config Common ** \n");
   ulCfgCmn = &srvCellCfgCmnSib->ulCfg;
   DU_LOG("Frequency Band Indicator %ld\n", ulCfgCmn->freqBandInd);
   DU_LOG("Maximum Transmission Power %ld\n", ulCfgCmn->pMax);
   DU_LOG("Frequency Location and Bandwidth %ld\n", ulCfgCmn->locAndBw);
   DU_LOG("Time Alignment Timer %ld\n", ulCfgCmn->timeAlignTimerComm);
   DU_LOG("Offset to Point A considering Subcarrier Spacing %ld\n", ulCfgCmn->ulScsCarrier.scsOffset);
   DU_LOG("Subcarrier Spacing %ld\n", ulCfgCmn->ulScsCarrier.scs);
   DU_LOG("Carrier BW considering Subcarrier Spacing %ld\n", ulCfgCmn->ulScsCarrier.scsBw);

   DU_LOG("\n ** SIB1 : Serving Cell Config Common : UL Config Common : RACH Config Common ** \n");
   rachCfgCmn = &ulCfgCmn ->rachCfg;
   if(rachCfgCmn->present)
   {
      DU_LOG("PRACH Configuration Index %ld\n", rachCfgCmn->prachCfgIdx);
      DU_LOG("Number of PRACH FDM %ld\n", rachCfgCmn->msg1Fdm);
      DU_LOG("PRACH Frequeny Start Offset %ld\n", rachCfgCmn->msg1FreqStart);
      DU_LOG("Zero Correaltion Zone Configuration %ld\n", rachCfgCmn->zeroCorrZoneCfg);
      DU_LOG("Target Power Level of Received Preamble %ld\n", rachCfgCmn->preambleRcvdTgtPwr);
      DU_LOG("Maximum number of Preamble Transmission %ld\n", rachCfgCmn->preambleTransMax);
      DU_LOG("Power Ramping Step %ld\n", rachCfgCmn->pwrRampingStep);
      DU_LOG("RA Response Window %ld\n", rachCfgCmn->raRspWindow);
      DU_LOG("Total Number of RA Preambles %ld\n", rachCfgCmn->numRaPreamble);
      DU_LOG("Number of SSB per RACH Occassion %ld\n", rachCfgCmn->numSsbPerRachOcc);
      DU_LOG("Number of Contention Based Preamble per SSB %ld\n", rachCfgCmn->numCbPreamblePerSsb);
      DU_LOG("Contention Resolution Timer %ld\n", rachCfgCmn->contResTimer);
      DU_LOG("RSRP Threshold %ld\n", rachCfgCmn->rsrpThreshSsb);
      DU_LOG("Root Sequence Index Present %d\n", rachCfgCmn->rootSeqIdxPresent);
      DU_LOG("Root Sequence Index %ld\n", rachCfgCmn->rootSeqIdx);
      DU_LOG("Subcarrier Spacing %ld\n", rachCfgCmn->msg1Scs);
      DU_LOG("Restricted Set Configuration %ld\n", rachCfgCmn->restrictedSetCfg);
   }
   else
   {
      DU_LOG("RACH Config Common not present\n");
   }

   DU_LOG("\n ** SIB1 : Serving Cell Config Common : UL Config Common : PUSCH Config Common ** \n");
   puschCfgCommon = &ulCfgCmn ->puschCfg;
   if(puschCfgCommon->puschCfgPresent)
   {
      DU_LOG("MSG3 Delta from RACH Preamble %ld\n", puschCfgCommon->msg3DeltaPreamble);
      DU_LOG("P0 Nominal With Grant %ld\n", puschCfgCommon->p0NominalWithGrant);
      DU_LOG("Number of Time Domain Resource Allocation %d\n", puschCfgCommon->numTimeDomRsrcAlloc);
      for(rsrcIdx = 0; rsrcIdx < puschCfgCommon->numTimeDomRsrcAlloc; rsrcIdx++)
      {
         puschCmnTimeDomRsrsAlloc = &puschCfgCommon ->timeDomAllocList[rsrcIdx];
         DU_LOG("Time Domain Resource Alloc [%d]", rsrcIdx);
         DU_LOG("\tK2 %ld\n", puschCmnTimeDomRsrsAlloc->k2);
         DU_LOG("\tMapping Type %ld\n", puschCmnTimeDomRsrsAlloc->mapType);
         DU_LOG("\tSLIV %d\n", puschCmnTimeDomRsrsAlloc->sliv);
      }
   }
   else
   {
      DU_LOG("PUSCH Config Common not present\n");
   }
   
   DU_LOG("\n ** SIB1 : Serving Cell Config Common : UL Config Common : PUCCH Config Common **\n");
   pucchCfgCommon = &ulCfgCmn->pucchCfg;
   if(pucchCfgCommon->present)
   {
      DU_LOG("Resource Common %ld\n", pucchCfgCommon->rsrcComm);
      DU_LOG("Group Hopping %ld\n", pucchCfgCommon->grpHop);
      DU_LOG("P0 Nominal %ld\n", pucchCfgCommon->p0Nominal);
   }
   else
   {
      DU_LOG("PUCCH Config Common not present\n");
   }
 
   DU_LOG("\n ** SIB1 : Serving Cell Config Common : TDD UL DL Config Common ** \n");
   tddUlDlCfgCmn = &srvCellCfgCmnSib->tddCfg;
   DU_LOG("Reference Subcarrier Spacing %ld\n", tddUlDlCfgCmn->refScs);
   DU_LOG("Transmission Periodicity %ld\n", tddUlDlCfgCmn->txPrd);
   DU_LOG("Number of DL Slots %ld\n", tddUlDlCfgCmn->numDlSlots);
   DU_LOG("Number of DL Symbols %ld\n", tddUlDlCfgCmn->numDlSymbols);
   DU_LOG("Number of UL Slots %ld\n", tddUlDlCfgCmn->numUlSlots);
   DU_LOG("Number of UL Symbols %ld\n", tddUlDlCfgCmn->numUlSymbols);

   DU_LOG("\n ** MAC SLICE CONFIG REQUEST ** \n");
   macSliceCfg = &duCfgParam.tempSliceCfg;
   DU_LOG("Number of RRM Policy %d\n",macSliceCfg->numOfRrmPolicy);

   for(policyIdx = 0; policyIdx < macSliceCfg->numOfRrmPolicy; policyIdx++)
   {
      rrmPolicy = macSliceCfg->listOfRrmPolicy[policyIdx];
      DU_LOG("RRM Policy [%d]", policyIdx);
      DU_LOG("\tResource Type %d\n", rrmPolicy->resourceType);

      rrmPolicyRatio = &rrmPolicy ->policyRatio;
      DU_LOG("\tPolicy Maximum Ratio %d\n", rrmPolicyRatio->maxRatio);
      DU_LOG("\tPolicy Minimum Ratio %d\n", rrmPolicyRatio->minRatio);
      DU_LOG("\tPolicy Deidcated Ration %d\n", rrmPolicyRatio->dedicatedRatio);
      DU_LOG("\tNumber of RRM Policy Member %d\n", rrmPolicy->numOfRrmPolicyMem);   
      
      for(memIdx = 0; memIdx < rrmPolicy->numOfRrmPolicyMem; memIdx++)
      {
         rrmPolicyMemberList = rrmPolicy->rRMPolicyMemberList[memIdx];
         DU_LOG("\tRRM Policy Member [%d]", memIdx);
         DU_LOG("\t\tPLMN : mcc[0] %d\n", rrmPolicyMemberList->plmn.mcc[0]);
         DU_LOG("\t\tPLMN : mcc[1] %d\n", rrmPolicyMemberList->plmn.mcc[1]);
         DU_LOG("\t\tPLMN : mcc[2] %d\n", rrmPolicyMemberList->plmn.mcc[2]);
         DU_LOG("\t\tPLMN : mnc[0] %d\n", rrmPolicyMemberList->plmn.mnc[0]);
         DU_LOG("\t\tPLMN : mnc[1] %d\n", rrmPolicyMemberList->plmn.mnc[1]);
         DU_LOG("\t\tPLMN : mnc[2] %d\n", rrmPolicyMemberList->plmn.mnc[2]);
         DU_LOG("\t\tSST %d\n",rrmPolicyMemberList->snssai.sst);
         DU_LOG("\t\tSD %d %d %d\n",rrmPolicyMemberList->snssai.sd[0],rrmPolicyMemberList->snssai.sd[1],rrmPolicyMemberList->snssai.sd[2]);
      }
   }
}

/**********************************************************************
  End of file
 **********************************************************************/

