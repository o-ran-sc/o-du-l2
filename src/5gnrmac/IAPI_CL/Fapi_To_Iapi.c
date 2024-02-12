#include "common_def.h"
#ifdef INTEL_FAPI
#include "nr5g_fapi_internal.h"
#include "fapi_vendor_extension.h"
#endif
#ifdef INTEL_WLS_MEM
#include "wls_lib.h"
#endif
#ifdef INTEL_API
#include "common_mac_phy_api.h"
#include "nr5g_mac_phy_api.h"
#endif
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "lwr_mac_upr_inf.h"
#include "mac.h"
#include "lwr_mac.h"
#include "lwr_mac_fsm.h"
#include "lwr_mac_phy.h"
#include "utilities.h"

#define WLS_BUFFER_SIZE 68000

uint32_t scsCommon;

void convertConfigReqFapiToIapi(uint8_t phyHandle, p_fapi_api_queue_elem_t cfgReqElem, \
   uint32_t l2ToL1MsgLen, PMAC2PHY_QUEUE_EL iapiQueueElem)
{
   uint8_t slotIdx = 0, symbolIdx = 0, beamIdx = 0;
   uint16_t csiSize = 0;
   uint16_t part2Size = 0;
   uint16_t idx = 0;
   uint32_t mib = 0;
   PCONFIGREQUESTStruct phyCfgReq = NULLP;

   fapi_config_req_t *cfgReq = NULLP;
   fapi_vendor_msg_t *vendorMsg = NULLP;
   p_fapi_api_queue_elem_t vendorMsgElem = NULLP;

   /* Fetch FAPI message header, config request and vendor messages */
   if(cfgReqElem)
   {
      cfgReq = (fapi_config_req_t *)(cfgReqElem + 1);
      vendorMsgElem = cfgReqElem->p_next;
      if(vendorMsgElem)
      {
         vendorMsg = (fapi_vendor_msg_t *)(vendorMsgElem + 1);
      }
   }

   /* Fill IAPI Queue element */
   memset(iapiQueueElem, 0, l2ToL1MsgLen);
   iapiQueueElem->nAlignOffset = sizeof(CONFIGREQUESTStruct);
   iapiQueueElem->pNext = NULLP;
   iapiQueueElem->nNumMessageInBlock = 1;
   iapiQueueElem->nMessageType = MSG_TYPE_PHY_CONFIG_REQ;
   iapiQueueElem->nMessageLen = l2ToL1MsgLen;

   /* Fill IAPI config request */
   phyCfgReq = (PCONFIGREQUESTStruct)(iapiQueueElem+1);
   phyCfgReq->sMsgHdr.nMessageType = MSG_TYPE_PHY_CONFIG_REQ;
   phyCfgReq->sMsgHdr.nMessageLen = sizeof(CONFIGREQUESTStruct);

   for(idx = 0; idx<cfgReq->number_of_tlvs; idx++)
   {
      switch(cfgReq->tlvs[idx].tl.tag)
      {
         case FAPI_DL_BANDWIDTH_TAG:
            {
               phyCfgReq->nDLBandwidth = cfgReq->tlvs[idx].value;
               break;
            }
         case FAPI_DL_FREQUENCY_TAG:
            {
               phyCfgReq->nDLAbsFrePointA = cfgReq->tlvs[idx].value;
               break;
            }
         case FAPI_NUM_TX_ANT_TAG:
            {
               phyCfgReq->nNrOfTxAnt = cfgReq->tlvs[idx].value;
               break;
            }
         case FAPI_UPLINK_BANDWIDTH_TAG:
            {
               phyCfgReq->nULBandwidth = cfgReq->tlvs[idx].value;
               break;
            }
         case FAPI_UPLINK_FREQUENCY_TAG:
            {
               phyCfgReq->nULAbsFrePointA = cfgReq->tlvs[idx].value;
               break;
            }
         case FAPI_NUM_RX_ANT_TAG:
            {
               phyCfgReq->nNrOfRxAnt = cfgReq->tlvs[idx].value;
               break;

            }
         case FAPI_PHY_CELL_ID_TAG:
            {
               phyCfgReq->nPhyCellId = cfgReq->tlvs[idx].value;
               break;
            }
         case FAPI_FRAME_DUPLEX_TYPE_TAG:
            {
               phyCfgReq->nFrameDuplexType = cfgReq->tlvs[idx].value;
               break;
            }
         case FAPI_SS_PBCH_POWER_TAG:
            {
               /* In stack code, ysUtlCalcSSPbchPowerVal() always returns 0. 0 is sent to PHY
                  In ODU High code, we send value 0, which gets converted to 1 at FAPI Translator. 1 is sent to PHY*/
               phyCfgReq->nSSBPwr = cfgReq->tlvs[idx].value;
               break;
            }
         case FAPI_SCS_COMMON_TAG:
            {
               phyCfgReq->nSubcCommon = cfgReq->tlvs[idx].value;
               scsCommon = phyCfgReq->nSubcCommon;
               break;
            }
         case FAPI_PRACH_SUBC_SPACING_TAG:
            {
               phyCfgReq->nPrachSubcSpacing = cfgReq->tlvs[idx].value;
               break;
            }
         case FAPI_RESTRICTED_SET_CONFIG_TAG:
            {
               phyCfgReq->nPrachRestrictSet = cfgReq->tlvs[idx].value;
               break;
            }
         case FAPI_NUM_PRACH_FD_OCCASIONS_TAG:
            {
               phyCfgReq->nPrachFdm = cfgReq->tlvs[idx].value;
               break;
            }
         case FAPI_PRACH_CONFIG_INDEX_TAG:
            {
               phyCfgReq->nPrachConfIdx = cfgReq->tlvs[idx].value;
               break;
            }
         case FAPI_PRACH_ROOT_SEQUENCE_INDEX_TAG:
            {
               phyCfgReq->nPrachRootSeqIdx = cfgReq->tlvs[idx].value;
               break;
            }
         case FAPI_K1_TAG:
            {
               phyCfgReq->nPrachFreqStart = cfgReq->tlvs[idx].value;
               break;
            }
         case FAPI_PRACH_ZERO_CORR_CONF_TAG:
            {
               phyCfgReq->nPrachZeroCorrConf = cfgReq->tlvs[idx].value;
               break;
            }
         case FAPI_SSB_PER_RACH_TAG:
            {
               phyCfgReq->nPrachSsbRach = cfgReq->tlvs[idx].value;
               break;
            }
         case FAPI_SSB_OFFSET_POINT_A_TAG:
            {
               /* The below formula is used in FAPI translator to conver ssbOffsetPointA to SSB PRB offset */
               phyCfgReq->nSSBPrbOffset = cfgReq->tlvs[idx].value / (pow(2, phyCfgReq->nSubcCommon));
               break;
            }
         case FAPI_SSB_PERIOD_TAG:
            {
               phyCfgReq->nSSBPeriod = cfgReq->tlvs[idx].value;
               break;
            }
         case FAPI_SSB_SUBCARRIER_OFFSET_TAG:
            {
               phyCfgReq->nSSBSubcOffset = cfgReq->tlvs[idx].value;
               break;
            }
         case FAPI_MIB_TAG:
            {
               mib = cfgReq->tlvs[idx].value;  
               phyCfgReq->nMIB[0] = mib >> 24;
               phyCfgReq->nMIB[1] = mib >> 16;
               phyCfgReq->nMIB[2] = mib >> 8;
               break;
            }
         case FAPI_SSB_MASK_TAG:
            {
               phyCfgReq->nSSBMask[0] = cfgReq->tlvs[idx].value;
               phyCfgReq->nSSBMask[1] = 0;
               break;
            }
         case FAPI_BEAM_ID_TAG:
            {
               /* In stack code, beamId is not filled.
                * In ODU-High, this is filled to 0. */
               phyCfgReq->nBeamId[beamIdx] = cfgReq->tlvs[idx].value;
               beamIdx++;
               break;
            }
         case FAPI_DMRS_TYPE_A_POS_TAG:
            {
               phyCfgReq->nDMRSTypeAPos = cfgReq->tlvs[idx].value;
               break;
            }
#ifdef NR_TDD
         case FAPI_TDD_PERIOD_TAG:
            {
               phyCfgReq->nTddPeriod = cfgReq->tlvs[idx].value;
               break;
            }
         case FAPI_SLOT_CONFIG_TAG:
            {
               /* HLAL FIX in ODU High:
                *
                * As per section 6.1.1 of 5g_FAPI_IAPI_Translator_Module doc,
                * FAPI Translator implementation and Stack code implementation:
                * The number of slots need to be equal to nTddPeriod
                *
                * However, in ODU-High, we have filled upto
                * MAX_TDD_PERIODICITY_SLOTS */
               phyCfgReq->sSlotConfig[slotIdx].nSymbolType[symbolIdx] = cfgReq->tlvs[idx].value;
               symbolIdx++;
               if(symbolIdx == MAX_NUM_OF_SYMBOL_PER_SLOT)
               {
                  symbolIdx = 0;
                  slotIdx++;
               }
               break;
            }
#endif
      }
   }

   phyCfgReq->nCarrierIdx = phyHandle;

   /* Filled in stack code.
    * Filled in ODU-High code as part of ConfigReq->vendorMessage
    * Filled in FAPI Translator from ConfigReq->vendorMessage sent by ODU High
    */
   phyCfgReq->nSSBSubcSpacing = vendorMsg->config_req_vendor.ssb_subc_spacing;
   phyCfgReq->nNrOfDLPorts = vendorMsg->config_req_vendor.nr_of_dl_ports;
   phyCfgReq->nNrOfULPorts = vendorMsg->config_req_vendor.nr_of_ul_ports; 
   phyCfgReq->nCarrierAggregationLevel = vendorMsg->config_req_vendor.carrier_aggregation_level;
   phyCfgReq->nPrachNrofRxRU = vendorMsg->config_req_vendor.prach_nr_of_rx_ru;
   phyCfgReq->nGroupHopFlag = vendorMsg->config_req_vendor.group_hop_flag;
   phyCfgReq->nSequenceHopFlag = vendorMsg->config_req_vendor.sequence_hop_flag;
   phyCfgReq->nHoppingId = vendorMsg->config_req_vendor.hopping_id;
   phyCfgReq->nUrllcCapable = vendorMsg->config_req_vendor.urllc_capable;

   /* Not filled in stack code
    * Filled in ODU-High code as part of ConfigReq->vendorMessage
    * Filled in FAPI Translator from ConfigReq->vendorMessage sent by ODU High
    */
   phyCfgReq->nUrllcMiniSlotMask = vendorMsg->config_req_vendor.urllc_mini_slot_mask;

   /* Filled in stack code
    * Not filled in ODU-High code
    * Not filled in FAPI Translator
    * As per 5g_FAPI_IAPI_Translator_Module doc : section 6.1.1:
    *    Not present in FAPI
    *    Dont care at L1 
    */
   //phyCfgReq->nSSBAbsFre;
   //phyCfgReq->nDLK0 = cellCb->cellCfg.nDLK0;
   //phyCfgReq->nULK0 = cellCb->cellCfg.nULK0;

   /* Filled in stack code
    * Not filled in ODU-High code
    * Filled in FAPI Translator : 
    *    Derived/calculated at FAPI translator
    * As per 5g_FAPI_IAPI_Translator_Module doc : section 6.1.1,  
    *    Not present in FAPI.
    */
   phyCfgReq->nDLFftSize = nr5g_fapi_calc_fft_size(phyCfgReq->nSubcCommon, phyCfgReq->nDLBandwidth);
   phyCfgReq->nULFftSize = nr5g_fapi_calc_fft_size(phyCfgReq->nSubcCommon, phyCfgReq->nULBandwidth);

   /* Filled in stack code
    * Not filled in ODU-High code
    * Not filled in FAPI Translator
    * As per 5g_FAPI_IAPI_Translator_Module doc : section 6.1.1,
    *    Only part of PARAM.request
    */
   //phyCfgReq->nCyclicPrefix;
   
   /* Filled in stack code
    * Not filled in ODU-High code
    * Not filled in FAPI Translator
    */
#ifdef INTEL_L1_V22_11
   phyCfgReq->RFInfoVal = 0;
   phyCfgReq->rsv[0] = 0;
   phyCfgReq->rsv[1] = 0;
   phyCfgReq->rsv[2] = 0;
   phyCfgReq->stOranCfgInfo.nRUPortNum = 0;
   phyCfgReq->stOranCfgInfo.nCCID = 0;
   phyCfgReq->stOranCfgInfo.RadioTypeIndex = 0;
   phyCfgReq->stOranCfgInfo.TadvCpDl = 0;
   phyCfgReq->stOranCfgInfo.T1aMaxCpDl = 0;
   phyCfgReq->stOranCfgInfo.T1aMinCpDl = 0;
   phyCfgReq->stOranCfgInfo.T1aMaxCpUl = 0;
   phyCfgReq->stOranCfgInfo.T1aMinCpUl = 0;
   phyCfgReq->stOranCfgInfo.T1aMaxUp = 0;
   phyCfgReq->stOranCfgInfo.T1aMinUp = 0;
   phyCfgReq->stOranCfgInfo.Ta4Max = 0;
   phyCfgReq->stOranCfgInfo.Ta4Min = 0;
#endif
/* HLAL 
 * Following is used when INTEL_L1_V22_07 is defined
 * Do we need to add this?
 */
#if 0
    phyCfgReq->nUci2Maps = 0;
    phyCfgReq->nNrOfDLCSIRSPorts =  phyCfgReq->nNrOfDLPorts;
    phyCfgReq->rsv2[0] = 0;
    for(csiSize = 0; csiSize < phyCfgReq->nUci2Maps; csiSize++)
    {
       for(part2Size = 0; part2Size < MAX_SIZE_CSI_PART2_MAP; part2Size++)
       {
          phyCfgReq->sPart2SizeMap[csiSize].nPart2Size[part2Size] = 0;
       }
    }
#endif

   /* Not filled in stack code
    * Not filled in ODU High code
    * Not filled in FAPI Translator
    * As per 5g_FAPI_IAPI_Translator_Module doc : section 6.1.1 : 
    *    "Need to define Vendor Specific Command to convey this information"
    *    Parameter not present in FAPI vendor specific message structure by Intel
    */
   //phyCfgReq->nNrofTxRUPerBeam;
   //phyCfgReq->nTxRUIdx;
}

void convertStartReqFapiToIapi(uint8_t phyHandle, p_fapi_api_queue_elem_t startReqElem, \
   uint32_t l2ToL1MsgLen, PMAC2PHY_QUEUE_EL iapiQueueElem)
{
   PSTARTREQUESTStruct phyStartReq = NULLP;

   fapi_start_req_t *startReq = NULLP;
   fapi_vendor_msg_t *vendorMsg = NULLP;
   p_fapi_api_queue_elem_t vendorMsgElem = NULLP;
   
   /* Fetch FAPI message header, start request and vendor messages */
   if(startReqElem)
   {
      startReq = (fapi_start_req_t *)(startReqElem + 1);
      vendorMsgElem = startReqElem->p_next;
      if(vendorMsgElem)
      {
         vendorMsg = (fapi_vendor_msg_t *)(vendorMsgElem + 1);
      }
   }

   /* Fill IAPI Queue element */
   memset(iapiQueueElem, 0, l2ToL1MsgLen);
   iapiQueueElem->nAlignOffset = sizeof(STARTREQUESTStruct);
   iapiQueueElem->pNext = NULLP;
   iapiQueueElem->nNumMessageInBlock = 1;
   iapiQueueElem->nMessageType = MSG_TYPE_PHY_START_REQ;
   iapiQueueElem->nMessageLen = l2ToL1MsgLen;

   /* Fill IAPI start request */
   phyStartReq = (PSTARTREQUESTStruct)(iapiQueueElem+1);
   phyStartReq->sMsgHdr.nMessageType = MSG_TYPE_PHY_START_REQ;
   phyStartReq->sMsgHdr.nMessageLen = sizeof(STARTREQUESTStruct);
   phyStartReq->sSFN_Slot.nSFN = vendorMsg->start_req_vendor.sfn;
   phyStartReq->sSFN_Slot.nSlot = vendorMsg->start_req_vendor.slot;
   phyStartReq->sSFN_Slot.nCarrierIdx = phyHandle;
   phyStartReq->sSFN_Slot.nRsv = 0;
   phyStartReq->nMode = vendorMsg->start_req_vendor.mode;
#ifndef DEBUG_MODE   
   phyStartReq->nCount = 0;
   phyStartReq->nPeriod = 0;
#else
   /* For timer mode */
   /* Value of count is set to 0 by ODU-high
    * In Stack code, in ysUtlFillStartReq(), 0x3FFFFFFF is mentioned in comments
    * */
   phyStartReq->nCount = vendorMsg->start_req_vendor.count;
   phyStartReq->nPeriod = vendorMsg->start_req_vendor.period;
#endif
}

void convertStopReqFapiToIapi(uint8_t phyHandle, p_fapi_api_queue_elem_t stopReqElem,  uint32_t l2ToL1MsgLen, PMAC2PHY_QUEUE_EL iapiQueueElem)
{
   PSTOPREQUESTStruct phyStopReq = NULLP;

   fapi_stop_req_t *stopReq = NULLP;
   fapi_vendor_msg_t *vendorMsg = NULLP;
   p_fapi_api_queue_elem_t vendorMsgElem = NULLP;

   /* Fetch FAPI message header, stop request and vendor messages */
   if(stopReqElem)
   {
      stopReq = (fapi_stop_req_t *)(stopReqElem + 1);
      vendorMsgElem = stopReqElem->p_next;
      if(vendorMsgElem)
      {
         vendorMsg = (fapi_vendor_msg_t *)(vendorMsgElem + 1);
      }
   }

   /* Fill IAPI Queue element */
   memset(iapiQueueElem, 0, l2ToL1MsgLen);
   iapiQueueElem->nAlignOffset = sizeof(STOPREQUESTStruct);
   iapiQueueElem->pNext = NULLP;
   iapiQueueElem->nNumMessageInBlock = 1;
   iapiQueueElem->nMessageType = MSG_TYPE_PHY_STOP_REQ;
   iapiQueueElem->nMessageLen = l2ToL1MsgLen;

   /* Fill IAPI stop request */
   phyStopReq = (PSTOPREQUESTStruct)(iapiQueueElem+1);
   phyStopReq->sMsgHdr.nMessageType = MSG_TYPE_PHY_STOP_REQ;
   phyStopReq->sMsgHdr.nMessageLen = sizeof(STOPREQUESTStruct);
   phyStopReq->sSFN_Slot.nSFN = vendorMsg->stop_req_vendor.sfn;
   phyStopReq->sSFN_Slot.nSlot = vendorMsg->stop_req_vendor.slot;
   phyStopReq->sSFN_Slot.nCarrierIdx = phyHandle;
   phyStopReq->sSFN_Slot.nRsv = 0;
}

void fillBchPdu(BCHPDUStruct *iapiBchPdu, fapi_dl_ssb_pdu_t *fapiSsbPdu)
{
   uint8_t *bchPayload = (uint8_t *)fapiSsbPdu->bchPayload.bchPayload;

   iapiBchPdu->sPDUHdr.nPDUType = DL_PDU_TYPE_PBCH;
   iapiBchPdu->sPDUHdr.nPDUSize = sizeof(BCHPDUStruct);
   iapiBchPdu->nSSBSubcOffset = fapiSsbPdu->ssbSubCarrierOffset/(1<<macCb.macCell[0]->numerology) ;
   iapiBchPdu->nMIB[0] = bchPayload[0];
   iapiBchPdu->nMIB[1] = bchPayload[1];
   iapiBchPdu->nMIB[2] = bchPayload[2];
   iapiBchPdu->nSSBPrbOffset = fapiSsbPdu->ssbOffsetPointA/pow(2, scsCommon);
}

void fillDciPdu(DCIPDUStruct *iapiDciPdu, fapi_dl_pdcch_pdu_t *fapiPdcchPdu, \
   fapi_vendor_dl_pdcch_pdu_t *fapiVendorPdcchPdu)
{
   iapiDciPdu->sPDUHdr.nPDUType = DL_PDU_TYPE_DCI;
   iapiDciPdu->sPDUHdr.nPDUSize = sizeof(DCIPDUStruct);
   iapiDciPdu->nRNTI = fapiPdcchPdu->dlDci[0].rnti;
   iapiDciPdu->nBWPSize = fapiPdcchPdu->bwpSize;
   iapiDciPdu->nBWPStart = fapiPdcchPdu->bwpStart ;
   iapiDciPdu->nSubcSpacing = fapiPdcchPdu->subCarrierSpacing ;
   iapiDciPdu->nCpType = fapiPdcchPdu->cyclicPrefix ;
   iapiDciPdu->nFreqDomain[0] = fapiPdcchPdu->freqDomainResource[0] | \
      (fapiPdcchPdu->freqDomainResource[1] << 8)  | \
      (fapiPdcchPdu->freqDomainResource[2] << 16) | \
      (fapiPdcchPdu->freqDomainResource[3] << 24) ;
   iapiDciPdu->nFreqDomain[1] = fapiPdcchPdu->freqDomainResource[4] |
      (fapiPdcchPdu->freqDomainResource[5] << 8) ;
   iapiDciPdu->nStartSymbolIndex = fapiPdcchPdu->startSymbolIndex ;
   iapiDciPdu->nNrOfSymbols = fapiPdcchPdu->durationSymbols ;
   iapiDciPdu->nCCEToREGType = fapiPdcchPdu->cceRegMappingType ;
   iapiDciPdu->nREGBundleSize = fapiPdcchPdu->regBundleSize ;
   iapiDciPdu->nShift = fapiPdcchPdu->shiftIndex ;
   iapiDciPdu->nScid = fapiPdcchPdu->dlDci[0].scramblingId ;
   iapiDciPdu->nCCEStartIndex = fapiPdcchPdu->dlDci[0].cceIndex ;
   iapiDciPdu->nAggrLvl = fapiPdcchPdu->dlDci[0].aggregationLevel ;
   iapiDciPdu->nInterleaveSize = fapiPdcchPdu->interleaverSize ;
   iapiDciPdu->nCoreSetType = fapiPdcchPdu->coreSetType ;
   iapiDciPdu->nRNTIScramb = fapiPdcchPdu->dlDci[0].scramblingRnti ;
   iapiDciPdu->nTotalBits = fapiPdcchPdu->dlDci[0].payloadSizeBits ;
   iapiDciPdu->nEpreRatioOfPDCCHToSSB = fapiVendorPdcchPdu->dl_dci[0].epre_ratio_of_pdcch_to_ssb ;
   iapiDciPdu->nEpreRatioOfDmrsToSSB = fapiVendorPdcchPdu->dl_dci[0].epre_ratio_of_dmrs_to_ssb ;
    memcpy(iapiDciPdu->nDciBits, fapiPdcchPdu->dlDci[0].payload, iapiDciPdu->nTotalBits) ;
   iapiDciPdu->nID = fapiPdcchPdu->dlDci[0].scramblingId ;

   /* Following are not mapped to any paramateres in FAPI interface.
    * These are also not filled in FAPI translator module
    *
    * iapiDciPdu->nBeamId;
    * iapiDciPdu->nNrofTxRU;
    * iapiDciPdu->nTxRUIdx[];
   */
}

void fillCsiRsPdu(CSIRSPDUStruct *iapiCsiRsPdu, fapi_dl_csi_rs_pdu_t *fapiCsiRsPdu, \
   fapi_vendor_csi_rs_pdu_t *fapiVendorCsiRsPdu)
{
   iapiCsiRsPdu->sPDUHdr.nPDUType = DL_PDU_TYPE_CSIRS;
   iapiCsiRsPdu->sPDUHdr.nPDUSize = sizeof(CSIRSPDUStruct);
   iapiCsiRsPdu->nBWPStart = fapiCsiRsPdu->bwpStart;
   iapiCsiRsPdu->nBWPSize = fapiCsiRsPdu->bwpSize;
   iapiCsiRsPdu->nSubcSpacing = fapiCsiRsPdu->subCarrierSpacing;
   iapiCsiRsPdu->nCpType = fapiCsiRsPdu->cyclicPrefix;
   iapiCsiRsPdu->nStartRB = fapiCsiRsPdu->startRb;
   iapiCsiRsPdu->nNrOfRBs = fapiCsiRsPdu->nrOfRbs;
   iapiCsiRsPdu->nCSIType = fapiCsiRsPdu->csiType;
   iapiCsiRsPdu->nRow = fapiCsiRsPdu->row;
   iapiCsiRsPdu->nFreqDomain = fapiCsiRsPdu->freqDomain;
   //iapiCsiRsPdu->nNrOfPorts;
   iapiCsiRsPdu->nCDMType = fapiCsiRsPdu->cdmType;
   iapiCsiRsPdu->nSymbL0 = fapiCsiRsPdu->symbL0;
   iapiCsiRsPdu->nSymbL1 = fapiCsiRsPdu->symbL1;
   iapiCsiRsPdu->nFreqDensity = fapiCsiRsPdu->freqDensity;
   //iapiCsiRsPdu->nCombOffset;
   iapiCsiRsPdu->nScrambId = fapiCsiRsPdu->scramId;
   iapiCsiRsPdu->nEpreRatioToSSB = fapiVendorCsiRsPdu->epre_ratio_to_ssb;
   //iapiCsiRsPdu->nNrofTxRU;
   //iapiCsiRsPdu->nTxRUIdx[];
}

void fillDlSchPdu(DLSCHPDUStruct *iapiDlSchPdu, fapi_dl_pdsch_pdu_t *fapiPdschPdu, \
   fapi_vendor_dl_pdsch_pdu_t *fapiVendorPdschPdu)
{
   uint8_t idx = 0, portIdx = 0;

   iapiDlSchPdu->sPDUHdr.nPDUType = DL_PDU_TYPE_DLSCH;
   iapiDlSchPdu->sPDUHdr.nPDUSize = sizeof(DLSCHPDUStruct);

   iapiDlSchPdu->nUEId = fapiPdschPdu->pdu_index;
   iapiDlSchPdu->nRNTI = fapiPdschPdu->rnti;
   iapiDlSchPdu->nBWPSize = fapiPdschPdu->bwpSize;
   iapiDlSchPdu->nBWPStart = fapiPdschPdu->bwpStart;
   iapiDlSchPdu->nSubcSpacing = fapiPdschPdu->subCarrierSpacing;
   iapiDlSchPdu->nCpType = fapiPdschPdu->cyclicPrefix;
   iapiDlSchPdu->nNrOfCodeWords = fapiPdschPdu->nrOfCodeWords;
   iapiDlSchPdu->nMcsTable = fapiPdschPdu->cwInfo[0].mcsTable;
   for(idx = 0; idx < iapiDlSchPdu->nNrOfCodeWords; idx++)
   {
      iapiDlSchPdu->nMCS[idx] = fapiPdschPdu->cwInfo[idx].mcsIndex;
      iapiDlSchPdu->nRV[idx] = fapiPdschPdu->cwInfo[idx].rvIndex;
      iapiDlSchPdu->nTBSize[idx] = fapiPdschPdu->cwInfo[idx].tbSize;
      /* HLAL :
       * Not filled in FAPI translator
       * Not present in FAPI interface either
       * Not present in FAPI-IAPI translaort pdf doc : section 6.1.4.2
       *  As per FAPI-IAPI excel doc, this parameter is not cared for at transmitter.
       *  At received, NDI info will be retrieved from DCI instead.
       */
      //iapiDlSchPdu->nNDI[idx];
   }

   iapiDlSchPdu->nNrOfLayers = fapiPdschPdu->nrOfLayers;
   iapiDlSchPdu->nNrOfAntennaPorts = fapiVendorPdschPdu->nr_of_antenna_ports;

   for(idx = 0; idx < FAPI_MAX_DMRS_PORTS; idx++)
   {
      if((fapiPdschPdu->dmrsPorts >> idx) & 0x0001)
         iapiDlSchPdu->nPortIndex[portIdx++] = idx;;
   }

   /* HLAL:
    * Not filled in FAPI translator
    * Not present in FAPI interface
    * As per FAPI-IAPI translator excel doc: This is a don't care at the
    * transmitter side. The harq id is sent via the DCI Format 1_0 or Format 1_1
    * to the UE.
    */
   //iapiDlSchPdu->nHARQID = fapiPdschPdu->;

   iapiDlSchPdu->nSCID = fapiPdschPdu->scid;
   iapiDlSchPdu->nNIDnSCID = fapiPdschPdu->dlDmrsScramblingId;
   iapiDlSchPdu->nVRBtoPRB = fapiPdschPdu->vrbToPrbMapping;

   /* HLAL :
    * Not filled in FAPI translator
    * Not supported at FAPI interface
    * As per FAPI-IAPI translator pdf, this parameter is not used by IAPI hence it can be set to 0
    */
   //iapiDlSchPdu->nRBBundleSize = 0;

   iapiDlSchPdu->nStartSymbolIndex = fapiPdschPdu->startSymbIndex;
   iapiDlSchPdu->nNrOfSymbols = fapiPdschPdu->nrOfSymbols;
   iapiDlSchPdu->nRBStart = fapiPdschPdu->rbStart;
   iapiDlSchPdu->nRBSize = fapiPdschPdu->rbSize;
   iapiDlSchPdu->nResourceAllocType = fapiPdschPdu->resourceAlloc;

   if(iapiDlSchPdu->nBWPSize>=1 && iapiDlSchPdu->nBWPSize<=36)
      iapiDlSchPdu->nRBGSize = 2;
   else if(iapiDlSchPdu->nBWPSize>=37 && iapiDlSchPdu->nBWPSize<=72)
      iapiDlSchPdu->nRBGSize = 4;
   else if(iapiDlSchPdu->nBWPSize>=73 && iapiDlSchPdu->nBWPSize<=144)
      iapiDlSchPdu->nRBGSize = 8;
   else if(iapiDlSchPdu->nBWPSize>145 && iapiDlSchPdu->nBWPSize<=275)
      iapiDlSchPdu->nRBGSize = 16;
   else
      iapiDlSchPdu->nRBGSize = 0;

   /* Not filled in FAPI TM
    * COnversion relation unknown
    */
   //iapiDlSchPdu->nNrOfRBGs = fapiPdschPdu->;

   /* Following are used for resource allocation type 0
    * As of now FlexRAN supports only resource allocation type 1
    * Hence skipping these for now
    */
   //iapiDlSchPdu->nRBGSize = fapiPdschPdu->;
   //iapiDlSchPdu->nRBGIndex = fapiPdschPdu->;

   iapiDlSchPdu->nMappingType = fapiPdschPdu->mappingType;
   iapiDlSchPdu->nDMRSConfigType = fapiPdschPdu->dmrsConfigType;
   iapiDlSchPdu->nNrOfCDMs = fapiPdschPdu->numDmrsCdmGrpsNoData;
   iapiDlSchPdu->nNrOfDMRSSymbols = fapiPdschPdu->nrOfDmrsSymbols;
   iapiDlSchPdu->nDMRSAddPos = fapiPdschPdu->dmrsAddPos;

   if(fapiPdschPdu->pduBitMap & 0x0001)
      iapiDlSchPdu->nPTRSPresent = 1;
   else
      iapiDlSchPdu->nPTRSPresent = 0;
   iapiDlSchPdu->nNrOfPTRSPorts = 0;
   for(idx = 0; idx < MAX_DL_PER_UE_PTRS_PORT_NUM; idx++)
   {
      if((fapiPdschPdu->ptrsPortIndex >> idx) && 0x01)
         iapiDlSchPdu->nPTRSPortIndex[iapiDlSchPdu->nNrOfPTRSPorts++] = idx;
   }

   if(iapiDlSchPdu->nPTRSPresent)
   {
      iapiDlSchPdu->nPTRSTimeDensity = pow(2, fapiPdschPdu->ptrsTimeDensity);
      if(fapiPdschPdu->ptrsFreqDensity == 0)
         iapiDlSchPdu->nPTRSFreqDensity = 2;
      else if(fapiPdschPdu->ptrsFreqDensity == 1)
         iapiDlSchPdu->nPTRSFreqDensity = 4;
   }
   iapiDlSchPdu->nNrOfDMRSAssPTRS[0] = 0x1;
   iapiDlSchPdu->nNrOfDMRSAssPTRS[1] = 0x1;
   iapiDlSchPdu->nPTRSReOffset = fapiPdschPdu->ptrsReOffset;
   iapiDlSchPdu->nEpreRatioOfPDSCHToPTRS = fapiPdschPdu->nEpreRatioOfPdschToPtrs;
   iapiDlSchPdu->nTransmissionScheme = fapiPdschPdu->transmissionScheme;

   /* HLAL :
    * Not fill in FAPI TM
    * Not found in FAPI Spec doc
    */
   //iapiDlSchPdu->nCodebookType = fapiPdschPdu->;
   //iapiDlSchPdu->nCodebookMode = fapiPdschPdu->;

   if(fapiPdschPdu->preCodingAndBeamforming.numPrgs > 0)
      iapiDlSchPdu->nPMI = fapiPdschPdu->preCodingAndBeamforming.pmi_bfi[0].pmIdx;
   else
      iapiDlSchPdu->nPMI = 0;

   iapiDlSchPdu->n1n2 = 0x02;
   iapiDlSchPdu->nEpreRatioOfDmrsToSSB = fapiVendorPdschPdu->epre_ratio_of_dmrs_to_ssb;
   iapiDlSchPdu->nNid = fapiPdschPdu->dataScramblingId;
   iapiDlSchPdu->nNrofTxRU = fapiVendorPdschPdu->nr_of_antenna_ports;
   iapiDlSchPdu->nEpreRatioOfPDSCHToSSB = fapiVendorPdschPdu->epre_ratio_of_pdsch_to_ssb;
   for(idx = 0; idx < MAX_TXRU_NUM;idx++)
   {
      iapiDlSchPdu->nTxRUIdx[idx] = fapiVendorPdschPdu->tx_ru_idx[idx];
   }

   /* Filled if MIMO mode is enabled */
   /*
   if(iapiDlSchPdu->nMMimoMode)
   {
      iapiDlSchPdu->nPortInfoBitMap = fapiPdschPdu->;
      iapiDlSchPdu->nSRSChanEstBufferIndx = fapiPdschPdu->;
   }
   */
}

void convertDlTtiReqFapiToIapi(uint8_t phyHandle, p_fapi_api_queue_elem_t dlTtiReqElem, \
   uint32_t l2ToL1MsgLen, PMAC2PHY_QUEUE_EL iapiQueueElem)
{
   uint8_t ueIdx = 0;
   uint8_t pduIdx = 0;
   uint16_t grpIdx = 0;
   PDSCHGroupInfoStruct  *pdschGrp = NULL;
   DCIPDUStruct          *dlDciPdu;
   CSIRSPDUStruct        *csiRsPdu;
   DLSCHPDUStruct        *dlSchPdu;
   BCHPDUStruct          *bchPdu;
   PDUStruct             *pduInfo;
   PDLConfigRequestStruct dlConfig = NULLP;
   fapi_dl_tti_req_t *dlTtiReq = NULLP;

   fapi_vendor_msg_t *vendorMsg;
   p_fapi_api_queue_elem_t  vendorMsgQElem = NULLP;
   p_fapi_api_queue_elem_t  nextElem = NULLP;

   if(dlTtiReqElem)
   {
      nextElem = dlTtiReqElem;
      while(nextElem)
      {
         if(nextElem->msg_type == FAPI_VENDOR_MESSAGE)
         {
            vendorMsgQElem = nextElem;
            vendorMsg = (fapi_vendor_msg_t *)(vendorMsgQElem + 1);
            break;
         }
         nextElem = nextElem->p_next;
      }

      dlTtiReq = (fapi_dl_tti_req_t *)(dlTtiReqElem + 1);
   }

   /* Fill IAPI Queue element */
   memset(iapiQueueElem, 0, l2ToL1MsgLen);
   iapiQueueElem->nAlignOffset = sizeof(DLConfigRequestStruct);
   iapiQueueElem->nNumMessageInBlock = 1;
   iapiQueueElem->nMessageType = MSG_TYPE_PHY_DL_CONFIG_REQ;
   iapiQueueElem->nMessageLen = l2ToL1MsgLen;
   iapiQueueElem->pNext = NULLP;

   /* Fill IAPI DL TTI Request */
   dlConfig = (PDLConfigRequestStruct)(iapiQueueElem+1);
   dlConfig->sMsgHdr.nMessageType = MSG_TYPE_PHY_DL_CONFIG_REQ;
   dlConfig->sMsgHdr.nMessageLen = sizeof(DLConfigRequestStruct) - sizeof(L1L2MessageHdr);
   dlConfig->sSFN_Slot.nSFN = dlTtiReq->sfn;
   dlConfig->sSFN_Slot.nSlot = dlTtiReq->slot;
   dlConfig->sSFN_Slot.nCarrierIdx = phyHandle;
   dlConfig->sSFN_Slot.nSym = 0;
   dlConfig->sSFN_Slot.nRsv = 0;

   dlConfig->nGroup = dlTtiReq->nGroup;
   dlConfig->nPDU = dlTtiReq->nPdus;
   dlConfig->nDCI = 0; // will be calculated when reading pdcch pdu
   if(vendorMsg)
   {
      dlConfig->nLte_CRS_Present = vendorMsg->p7_req_vendor.dl_tti_req.lte_crs_present;
      dlConfig->nLte_CRS_carrierFreqDL = vendorMsg->p7_req_vendor.dl_tti_req.lte_crs_carrier_freq_dl;
      dlConfig->nLte_CRS_carrierBandwidthDL = vendorMsg->p7_req_vendor.dl_tti_req.lte_crs_carrier_bandwidth_dl;
      dlConfig->nLte_CRS_nrofCRS_Ports = vendorMsg->p7_req_vendor.dl_tti_req.lte_crs_nr_of_crs_ports;
      dlConfig->nLte_CRS_v_shift = vendorMsg->p7_req_vendor.dl_tti_req.lte_crs_v_shift;
      dlConfig->nPdcchPrecoderEn = vendorMsg->p7_req_vendor.dl_tti_req.pdcch_precoder_en;
      dlConfig->nSSBPrecoderEn = vendorMsg->p7_req_vendor.dl_tti_req.ssb_precoder_en;
   }
   dlConfig->nRsv[0] = 0;

   pduInfo = (PDUStruct *) ((uint8_t *)dlConfig->sDLPDU); /*PDU Struct starts from here*/

   for (grpIdx = 0 ; grpIdx < dlTtiReq->nGroup ; grpIdx++)
   {
      dlConfig->sPDSCHGroupInfoStruct[grpIdx].nUE = dlTtiReq->ue_grp_info[grpIdx].nUe;
      for(ueIdx=0; ueIdx < dlTtiReq->ue_grp_info[grpIdx].nUe; ueIdx++)
      {
         dlConfig->sPDSCHGroupInfoStruct[grpIdx].nPduIdx[ueIdx] = dlTtiReq->ue_grp_info[grpIdx].pduIdx[ueIdx];
      }
   }

   for(pduIdx = 0; pduIdx < dlTtiReq->nPdus; pduIdx++)
   {
      switch(dlTtiReq->pdus[pduIdx].pduType)
      {
         case FAPI_PDCCH_PDU_TYPE:
            {
               dlDciPdu = (DCIPDUStruct *)((uint8_t *)pduInfo);
               fillDciPdu(dlDciPdu, &dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu,
                  &vendorMsg->p7_req_vendor.dl_tti_req.pdus[pduIdx].pdu.pdcch_pdu);
               break;
            }

         case FAPI_PDSCH_PDU_TYPE:
            {
               dlSchPdu = (DLSCHPDUStruct *)((uint8_t *)pduInfo);
               fillDlSchPdu(dlSchPdu, &dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu, \
                  &vendorMsg->p7_req_vendor.dl_tti_req.pdus[pduIdx].pdu.pdsch_pdu);
               break;
            }

         case FAPI_CSIRS_PDU_TYPE:
            {
               csiRsPdu = (CSIRSPDUStruct *)((uint8_t *)pduInfo);
               fillCsiRsPdu(csiRsPdu, &dlTtiReq->pdus[pduIdx].pdu.csi_rs_pdu, \
                  &vendorMsg->p7_req_vendor.dl_tti_req.pdus[pduIdx].pdu.csi_rs_pdu);
               break;
            }

         case FAPI_PBCH_PDU_TYPE:
            {
               bchPdu = (BCHPDUStruct *)((uint8_t *)pduInfo);
               fillBchPdu(bchPdu, &dlTtiReq->pdus[pduIdx].pdu.ssb_pdu);
               break;
            }
      }

      pduInfo = (PDUStruct *)((uint8_t *)pduInfo + pduInfo->nPDUSize);
   }
}

void convertTxDataReqFapiToIapi(uint8_t phyHandle, p_fapi_api_queue_elem_t fapiQueueElem, \
   uint32_t l2ToL1MsgLen, PMAC2PHY_QUEUE_EL iapiQueueElem)
{
   uint8_t idx = 0, pduIdx = 0;
   bool pduFound = false;
   DLPDUDataStruct *pduList = NULLP;
   DLPDUDataStruct *pduInfo = NULLP;
   PTXRequestStruct iapiTxReq = NULLP;
   fapi_tx_data_req_t *fapiTxDataReq = NULLP;
   fapi_vendor_msg_t *vendorMsg;
   p_fapi_api_queue_elem_t  vendorMsgQElem = NULLP;
   p_fapi_api_queue_elem_t  nextElem = NULLP;

   if(fapiQueueElem)
   {
      nextElem = fapiQueueElem;
      while(nextElem)
      {
         if(nextElem->msg_type == FAPI_VENDOR_MESSAGE)
         {
            vendorMsgQElem = nextElem;
            vendorMsg = (fapi_vendor_msg_t *)(vendorMsgQElem + 1);
            break;
         }
         nextElem = nextElem->p_next;
      }

      fapiTxDataReq = (fapi_tx_data_req_t *)(fapiQueueElem + 1);
   }

   /* Fill IAPI Queue element */
   memset(iapiQueueElem, 0, l2ToL1MsgLen);
   iapiQueueElem->nAlignOffset = sizeof(TXRequestStruct);
   iapiQueueElem->nNumMessageInBlock = 1;
   iapiQueueElem->nMessageType = MSG_TYPE_PHY_TX_REQ;
   iapiQueueElem->nMessageLen = l2ToL1MsgLen;
   iapiQueueElem->pNext = NULLP;
 
   /* Fill IAPI DL TTI Request */
   iapiTxReq = (PTXRequestStruct)(iapiQueueElem+1);
   iapiTxReq->sMsgHdr.nMessageType = MSG_TYPE_PHY_TX_REQ;
   iapiTxReq->sMsgHdr.nMessageLen = sizeof(TXRequestStruct) - sizeof(L1L2MessageHdr);
   iapiTxReq->sSFN_Slot.nSFN = fapiTxDataReq->sfn;
   iapiTxReq->sSFN_Slot.nSlot = fapiTxDataReq->slot;
   iapiTxReq->sSFN_Slot.nCarrierIdx = phyHandle;
   iapiTxReq->sSFN_Slot.nSym = 0;
   iapiTxReq->sSFN_Slot.nRsv = 0;

   iapiTxReq->nPDU = 0;
   pduList = pduInfo = (DLPDUDataStruct *) ((uint8_t *)iapiTxReq->sDLPDUDataStruct); /*PDU Struct starts from here*/
   for(idx = 0; idx < fapiTxDataReq->num_pdus; idx++)
   {
      pduFound = false;
      for(pduIdx = 0; pduIdx < iapiTxReq->nPDU; pduIdx++)
      {
         if(pduList[pduIdx].nPduIdx == fapiTxDataReq->pdu_desc[idx].pdu_index)
         {
            pduFound = true;
            break;
         }
      }

      if(!pduFound)
      {
         pduInfo->nPduIdx = fapiTxDataReq->pdu_desc[idx].pdu_index;
         pduInfo->nPduLen1 = ((fapiTxDataReq->pdu_desc[idx].tlvs[0].tl.tag & 0xff00)<<8) | \
              fapiTxDataReq->pdu_desc[idx].tlvs[0].tl.length;
         pduInfo->pPayload1 = fapiTxDataReq->pdu_desc[idx].tlvs[0].value; 
         iapiTxReq->nPDU++;
         pduInfo++;
      }
      else
      {
         pduList[pduIdx].nPduLen2 = ((fapiTxDataReq->pdu_desc[idx].tlvs[0].tl.tag & 0xff00)<<8) | \
            fapiTxDataReq->pdu_desc[idx].tlvs[0].tl.length;
         pduList[pduIdx].pPayload2 = fapiTxDataReq->pdu_desc[idx].tlvs[0].value;
      }
   }
}

void convertUlDciReqFapiToIapi(uint8_t phyHandle, p_fapi_api_queue_elem_t fapiQueueElem, \
   uint32_t l2ToL1MsgLen, PMAC2PHY_QUEUE_EL iapiQueueElem)
{
   uint8_t idx = 0;
   PDCIPDUStruct dciPduInfo = NULLP;
   PULDCIRequestStruct iapiUlDciReq = NULLP;
   fapi_ul_dci_req_t *fapiUlDciReq = NULLP;
   fapi_vendor_msg_t *vendorMsg;
   p_fapi_api_queue_elem_t  vendorMsgQElem = NULLP;
   p_fapi_api_queue_elem_t  nextElem = NULLP;

   if(fapiQueueElem)
   {
      nextElem = fapiQueueElem;
      while(nextElem)
      {
         if(nextElem->msg_type == FAPI_VENDOR_MESSAGE)
         {
            vendorMsgQElem = nextElem;
            vendorMsg = (fapi_vendor_msg_t *)(vendorMsgQElem + 1);
            break;
         }
         nextElem = nextElem->p_next;
      }

      fapiUlDciReq = (fapi_ul_dci_req_t *)(fapiQueueElem + 1);
   }

   /* Fill IAPI Queue element */
   memset(iapiQueueElem, 0, l2ToL1MsgLen);
   iapiQueueElem->nAlignOffset = sizeof(ULDCIRequestStruct);
   iapiQueueElem->nNumMessageInBlock = 1;
   iapiQueueElem->nMessageType = MSG_TYPE_PHY_UL_DCI_REQ;
   iapiQueueElem->nMessageLen = l2ToL1MsgLen;
   iapiQueueElem->pNext = NULLP;
 
   /* Fill IAPI DL TTI Request */
   iapiUlDciReq = (PTXRequestStruct)(iapiQueueElem+1);
   iapiUlDciReq->sMsgHdr.nMessageType = MSG_TYPE_PHY_UL_DCI_REQ;
   iapiUlDciReq->sMsgHdr.nMessageLen = sizeof(ULDCIRequestStruct) - sizeof(L1L2MessageHdr);
   iapiUlDciReq->sSFN_Slot.nSFN = fapiUlDciReq->sfn;
   iapiUlDciReq->sSFN_Slot.nSlot = fapiUlDciReq->slot;
   iapiUlDciReq->sSFN_Slot.nCarrierIdx = phyHandle;
   iapiUlDciReq->sSFN_Slot.nSym = 0;
   iapiUlDciReq->sSFN_Slot.nRsv = 0;

   iapiUlDciReq->nDCI = fapiUlDciReq->numPdus;
   dciPduInfo = (PDCIPDUStruct) ((uint8_t *)iapiUlDciReq->sULDCIPDU); /*PDU Struct starts from here*/
   for(idx = 0; idx < fapiUlDciReq->numPdus; idx++)
   {
      dciPduInfo->sPDUHdr.nPDUType = DL_PDU_TYPE_DCI;
      dciPduInfo->sPDUHdr.nPDUSize = sizeof(DCIPDUStruct);
      dciPduInfo->nRNTI = fapiUlDciReq->pdus[idx].pdcchPduConfig.dlDci[0].rnti;
      dciPduInfo->nBWPSize = fapiUlDciReq->pdus[idx].pdcchPduConfig.bwpSize;
      dciPduInfo->nBWPStart = fapiUlDciReq->pdus[idx].pdcchPduConfig.bwpStart;
      dciPduInfo->nSubcSpacing = fapiUlDciReq->pdus[idx].pdcchPduConfig.subCarrierSpacing;
      dciPduInfo->nCpType = fapiUlDciReq->pdus[idx].pdcchPduConfig.cyclicPrefix;
      dciPduInfo->nFreqDomain[0] = fapiUlDciReq->pdus[idx].pdcchPduConfig.freqDomainResource[0] | \
                                   (fapiUlDciReq->pdus[idx].pdcchPduConfig.freqDomainResource[1] << 8)  | \
                                   (fapiUlDciReq->pdus[idx].pdcchPduConfig.freqDomainResource[2] << 16) | \
                                   (fapiUlDciReq->pdus[idx].pdcchPduConfig.freqDomainResource[3] << 24) ;
      dciPduInfo->nFreqDomain[1] = fapiUlDciReq->pdus[idx].pdcchPduConfig.freqDomainResource[4] |\
                                   (fapiUlDciReq->pdus[idx].pdcchPduConfig.freqDomainResource[5] << 8) ;
      dciPduInfo->nStartSymbolIndex = fapiUlDciReq->pdus[idx].pdcchPduConfig.startSymbolIndex;
      dciPduInfo->nNrOfSymbols = fapiUlDciReq->pdus[idx].pdcchPduConfig.durationSymbols;
      dciPduInfo->nCCEToREGType = fapiUlDciReq->pdus[idx].pdcchPduConfig.cceRegMappingType;
      dciPduInfo->nREGBundleSize = fapiUlDciReq->pdus[idx].pdcchPduConfig.regBundleSize;
      dciPduInfo->nShift = fapiUlDciReq->pdus[idx].pdcchPduConfig.shiftIndex;
      dciPduInfo->nScid = fapiUlDciReq->pdus[idx].pdcchPduConfig.dlDci[0].scramblingId;
      dciPduInfo->nCCEStartIndex = fapiUlDciReq->pdus[idx].pdcchPduConfig.dlDci[0].cceIndex;
      dciPduInfo->nAggrLvl = fapiUlDciReq->pdus[idx].pdcchPduConfig.dlDci[0].aggregationLevel;
      dciPduInfo->nInterleaveSize = fapiUlDciReq->pdus[idx].pdcchPduConfig.interleaverSize;
      dciPduInfo->nCoreSetType = fapiUlDciReq->pdus[idx].pdcchPduConfig.coreSetType;
      dciPduInfo->nRNTIScramb = fapiUlDciReq->pdus[idx].pdcchPduConfig.dlDci[0].scramblingRnti;
      dciPduInfo->nTotalBits = fapiUlDciReq->pdus[idx].pdcchPduConfig.dlDci[0].payloadSizeBits;
      dciPduInfo->nEpreRatioOfPDCCHToSSB = \
         vendorMsg->p7_req_vendor.ul_dci_req.pdus[idx].pdcch_pdu_config.dl_dci[0].epre_ratio_of_pdcch_to_ssb;
      dciPduInfo->nEpreRatioOfDmrsToSSB = \
         vendorMsg->p7_req_vendor.ul_dci_req.pdus[idx].pdcch_pdu_config.dl_dci[0].epre_ratio_of_dmrs_to_ssb;
      memcpy(dciPduInfo->nDciBits, fapiUlDciReq->pdus[idx].pdcchPduConfig.dlDci[0].payload, dciPduInfo->nTotalBits) ;
      dciPduInfo->nBeamId = 0;
      dciPduInfo->nNrofTxRU = 0;
      dciPduInfo->nID = fapiUlDciReq->pdus[idx].pdcchPduConfig.dlDci[0].scramblingId;
      for(uint8_t ruIdx=0; ruIdx < MAX_TXRU_NUM; ruIdx++)
         dciPduInfo->nTxRUIdx[ruIdx] = 0;

      dciPduInfo += sizeof(DCIPDUStruct);  
   }
}

void fillPrachPduInfo(PRACHPDUStruct *iapiPrachPdu, fapi_ul_prach_pdu_t *fapiPrachpdu, RachInfo *rachInfo)
{
   iapiPrachPdu->sPDUHdr.nPDUType = UL_PDU_TYPE_PRACH;
   iapiPrachPdu->sPDUHdr.nPDUSize =  sizeof(PRACHPDUStruct);
   iapiPrachPdu->nNrofPrachOcas = fapiPrachpdu->numPrachOcas;
   rachInfo->phyCellId = fapiPrachpdu->physCellId;

/* PBORLA 
 * nNrofBeamPerOcas -> relationship cannt found
 * Following parameter does not supported in the current release
 * nBeamIdPerSlot [MAX_PANEL_NUM]
 * nNrofRxRuPerBeam [MAX_PANEL_NUM]
 * nRxRUIdx[MAX_PANEL_NUM][MAX_RxRU_NUM]
 *
 */
}

void fillSrsPduInfo(SRSPDUStruct *iapiSrsPdu, fapi_ul_srs_pdu_t *fapiSrspdu, fapi_vendor_ul_srs_pdu_t *fapiVendorSrsPdu)
{
   uint8_t i;

   iapiSrsPdu->sPDUHdr.nPDUType = UL_PDU_TYPE_SRS;
   iapiSrsPdu->sPDUHdr.nPDUSize = sizeof(SRSPDUStruct);
   iapiSrsPdu->nRNTI = fapiSrspdu->rnti;
   iapiSrsPdu->nUEId = fapiSrspdu->handle;
   //PBORLA - Following parameter from 763 to 766 does not supported in the current release
   iapiSrsPdu->nBWPSize = fapiSrspdu->bwpSize;
   iapiSrsPdu->nBWPStart = fapiSrspdu->bwpStart;
   iapiSrsPdu->nSubcSpacing = fapiSrspdu->subCarrierSpacing;
   iapiSrsPdu->nCpType = fapiSrspdu->cyclicPrefix;

   iapiSrsPdu->nStartPos = fapiSrspdu->timeStartPosition;
   iapiSrsPdu->nNrOfSymbols = pow(2,fapiSrspdu->numSymbols);
   if(fapiSrspdu->combSize)
      iapiSrsPdu->nComb = 4;
   else
      iapiSrsPdu->nComb = 2;
   iapiSrsPdu->nCombOffset  = fapiSrspdu->combOffset;
   iapiSrsPdu->nNrOfSrsPorts  = pow(2,fapiSrspdu->numAntPorts);
   iapiSrsPdu->nCyclicShift  = fapiSrspdu->cyclicShift;
   iapiSrsPdu->nBsrs  = fapiSrspdu->bandwidthIndex;
   iapiSrsPdu->nCsrs  = fapiSrspdu->configIndex;
   iapiSrsPdu->nBHop  = fapiSrspdu->frequencyHopping;
   iapiSrsPdu->nHopping  = fapiSrspdu->groupOrSequenceHopping;
   iapiSrsPdu->nFreqPos  = fapiSrspdu->frequencyPosition;
   iapiSrsPdu->nResourceType  = fapiSrspdu->resourceType;
   iapiSrsPdu->nFreqShift  = fapiSrspdu->frequencyShift;
   iapiSrsPdu->nSrsId  = fapiSrspdu->sequenceId;
   iapiSrsPdu->nRepetition  = pow(2,fapiSrspdu->numRepetitions);
   iapiSrsPdu->nTsrs  = fapiSrspdu->tSrs;
   iapiSrsPdu->nToffset  = fapiSrspdu->tOffset;
   //PBORLA - nNrofRxRU does not supported in the current release
   iapiSrsPdu->nNrofRxRU  = fapiVendorSrsPdu->nr_of_rx_ru;
   /* PBORLA 
      iapiSrsPdu->nPortInfoBitMap unable to find the mapping
   */
   //PBORLA - nRxRUIdx,nBeamId does not supported in the current release
   for(i =0; i< iapiSrsPdu->nNrofRxRU; i++)
   {
      iapiSrsPdu->nRxRUIdx[i]= fapiVendorSrsPdu->rx_ru_idx[i]; 
   }
   iapiSrsPdu->nBeamId[0]= fapiSrspdu->beamforming.rx_bfi[0].beamIdx[0].beamidx; 

}


void fillPuschPduInfo(ULSCHPDUStruct *iapiPuschPdu, fapi_ul_pusch_pdu_t *fapiPuschpdu, \
   fapi_vendor_ul_pusch_pdu_t *fapiVendorPuschPdu, PuschInfo *puschInfo)
{
   
   uint8_t i=0, idx = 0, portIdx = 0;

   iapiPuschPdu->sPDUHdr.nPDUType  = UL_PDU_TYPE_ULSCH;
   iapiPuschPdu->sPDUHdr.nPDUSize = sizeof(ULSCHPDUStruct);
   iapiPuschPdu->nRNTI = fapiPuschpdu->rnti;
   puschInfo->handle = iapiPuschPdu->nUEId = fapiPuschpdu->handle;
   //PBORLA - Following parameter from 808 to 812 does not supported in the current release
   iapiPuschPdu->nBWPSize= fapiPuschpdu->bwpSize;
   iapiPuschPdu->nBWPStart = fapiPuschpdu->bwpStart;
   iapiPuschPdu->nSubcSpacing = fapiPuschpdu->subCarrierSpacing;
   iapiPuschPdu->nCpType = fapiPuschpdu->cyclicPrefix;
   iapiPuschPdu->nULType = 0; //PBORLA

   iapiPuschPdu->nMcsTable = fapiPuschpdu->mcsTable; 
   iapiPuschPdu->nMCS = fapiPuschpdu->mcsIndex;
   iapiPuschPdu->nTransPrecode = !(fapiPuschpdu->transformPrecoding);
   iapiPuschPdu->nTransmissionScheme= 0; 
   iapiPuschPdu->nNrOfLayers = fapiPuschpdu->nrOfLayers;
   for(idx = 0; idx < FAPI_MAX_DMRS_PORTS; idx++)
   {
      if((fapiPuschpdu->dmrsPorts >> idx) & 0x0001)
         iapiPuschPdu->nPortIndex[portIdx++] = idx;
   }
   iapiPuschPdu->nSCID = fapiPuschpdu->scid;
   iapiPuschPdu->nNIDnSCID = fapiPuschpdu->ulDmrsScramblingId;
   iapiPuschPdu->nNrOfAntennaPorts = fapiVendorPuschPdu->nr_of_antenna_ports;
   iapiPuschPdu->nVRBtoPRB = fapiPuschpdu->vrbToPrbMapping;
   
   /* PBORLA :
    * Not filled in FAPI translator
    * Not supported at FAPI interface
    * As per FAPI-IAPI translator pdf, nRBBundleSize  is not used by IAPI hence it can be set to 0 */
   iapiPuschPdu->nRBBundleSize = 0; 
   
   iapiPuschPdu->nPMI = 0;
   iapiPuschPdu->nStartSymbolIndex = fapiPuschpdu->startSymbIndex;
   iapiPuschPdu->nNrOfSymbols = fapiPuschpdu->startSymbIndex;
   iapiPuschPdu->nResourceAllocType = fapiPuschpdu->resourceAlloc;
   /* PBORLA 
    * Following are used for resource allocation type 0
    * As of now FlexRAN supports only resource allocation type 1
    * Hence skipping these for now
      iapiPuschPdu->nRBGSize 
    */

   iapiPuschPdu->nRBStart = fapiPuschpdu->rbStart;
   iapiPuschPdu->nRBSize = fapiPuschpdu->rbSize;

   /* PBORLA 
    * Following are used for resource allocation type 0
    * As of now FlexRAN supports only resource allocation type 1
    * Hence skipping these for now
      iapiPuschPdu->nRBGIndex = fapiPuschpdu->;//PBORLA
    */

   iapiPuschPdu->nTBSize = fapiPuschpdu->puschData.tbSize;
   /* Not filled in FAPI TM
    * Conversion relation unknown
      iapiPuschPdu->nNrOfRBGs = fapiPuschpdu->;//PBORLA
    */
   iapiPuschPdu->nRV = fapiPuschpdu->puschData.rvIndex;
   puschInfo->harqId = iapiPuschPdu->nHARQID = fapiPuschpdu->puschData.harqProcessId;
   iapiPuschPdu->nNDI= fapiPuschpdu->puschData.newDataIndicator;
   iapiPuschPdu->nMappingType= fapiPuschpdu->mappingType;
   iapiPuschPdu->nDMRSConfigType= fapiPuschpdu->dmrsConfigType;
   iapiPuschPdu->nNrOfCDMs= fapiPuschpdu->numDmrsCdmGrpsNoData;
   iapiPuschPdu->nNrOfDMRSSymbols= fapiPuschpdu->nrOfDmrsSymbols;
   iapiPuschPdu->nDMRSAddPos= fapiPuschpdu->dmrsAddPos;
   if(fapiPuschpdu->pduBitMap & 0x0004)
      iapiPuschPdu->nPTRSPresent = 1;
   else
      iapiPuschPdu->nPTRSPresent = 0;
   iapiPuschPdu->nNrOfPTRSPorts= fapiPuschpdu->puschPtrs.numPtrsPorts;
   
   if(fapiPuschpdu->puschPtrs.ptrsTimeDensity == 0)
      iapiPuschPdu->nPTRSTimeDensity= 1;
   else if (fapiPuschpdu->puschPtrs.ptrsTimeDensity == 1)
      iapiPuschPdu->nPTRSTimeDensity= 2;
   else if (fapiPuschpdu->puschPtrs.ptrsTimeDensity == 2)
      iapiPuschPdu->nPTRSTimeDensity= 4;
   else
      iapiPuschPdu->nPTRSTimeDensity= 0;

   for(idx = 0; idx < MAX_DL_PER_UE_PTRS_PORT_NUM; idx++)
   {
      if((fapiPuschpdu->dmrsPorts>> idx) && 0x01)
         iapiPuschPdu->nPTRSPortIndex[i++] = idx;
   }
   
   if(fapiPuschpdu->puschPtrs.ptrsFreqDensity ==0)
      iapiPuschPdu->nPTRSFreqDensity= 2;
   else if(fapiPuschpdu->puschPtrs.ptrsFreqDensity ==1)
      iapiPuschPdu->nPTRSFreqDensity= 4;
   else
      iapiPuschPdu->nPTRSFreqDensity= 0;

   iapiPuschPdu->nPTRSReOffset= fapiPuschpdu->puschPtrs.ptrsInfo[0].ptrsReOffset;
   iapiPuschPdu->nTpPi2BPSK= fapiPuschpdu->tpPi2Bpsk;
   iapiPuschPdu->nNid= fapiPuschpdu->dataScramblingId;
   iapiPuschPdu->nAlphaScaling= fapiPuschpdu->puschUci.alphaScaling;
   iapiPuschPdu->nBetaOffsetACKIndex= fapiPuschpdu->puschUci.betaOffsetHarqAck;
   iapiPuschPdu->nAck= fapiPuschpdu->puschUci.harqAckBitLength;
   iapiPuschPdu->nBetaOffsetCSIP1Index= fapiPuschpdu->puschUci.betaOffsetCsi1;
   iapiPuschPdu->nBetaOffsetCSIP2Index= fapiPuschpdu->puschUci.betaOffsetCsi2;
   iapiPuschPdu->nCSIPart1= fapiPuschpdu->puschUci.csiPart1BitLength;
   iapiPuschPdu->nCSIPart2= fapiPuschpdu->puschUci.csiPart2BitLength;
   /* PBORLA :
    * nNrofRxRU  is not supported in the current release*/
   iapiPuschPdu->nNrofRxRU = fapiVendorPuschPdu->nr_of_rx_ru;

   iapiPuschPdu->nTPPuschID= fapiPuschpdu->nTpPuschId;
   /* PBORLA :
    * Not filled in FAPI translator
    * Not supported at FAPI interface, So setting it as 0 */
   iapiPuschPdu->nMMimoMode= 0;


   /* Filled if MIMO mode is enabled */
   /*
   if(iapiPuschPdu->nMMimoMode ==1)
   {
      iapiPuschPdu->nPortInfoBitMap = fapiPuschpdu->;
      iapiPuschPdu->nSRSChanEstBufferIndx  = fapiPuschpdu->;
   }
   */

   /* PBORLA :
    * Not filled in FAPI translator
    * Not supported at FAPI interface
    iapiPuschPdu->nDmrsGroupSeqHop= 0;
   */
   
   for(i =0; i< iapiPuschPdu->nNrofRxRU; i++)
   {
      iapiPuschPdu->nRxRUIdx[i]= fapiVendorPuschPdu->rx_ru_idx[i];
   }

   /* PBORLA :
    * Not filled in FAPI translator
    * Not supported at FAPI interface
    iapiPuschPdu->isLBRMSupp = 0;
    iapiPuschPdu->nTBSlbrm = 0;
   */
   iapiPuschPdu->nBeamId[0]= fapiPuschpdu->beamforming.rx_bfi[0].beamIdx[0].beamidx;
   
   /* PBORLA :
    * stBeamMatrixInfo  is not supported in the current release 
    * iapiPuschPdu->stBeamMatrixInfo  =0*/

   /*
    
   if(iapiPuschPdu->nMMimoMode ==2)
   {
      iapiPuschPdu->nRsvForBeamMatrixStruct = fapiPuschpdu->;
      iapiPuschPdu->stBeamMatrixInfo->nType= fapiPuschpdu->;
      iapiPuschPdu->stBeamMatrixInfo->nMatrixBeamID= fapiPuschpdu->;
      iapiPuschPdu->stBeamMatrixInfo->nrsv[0]= fapiPuschpdu->;
      iapiPuschPdu->stBeamMatrixInfo->nNumPRG= fapiPuschpdu->;
      iapiPuschPdu->stBeamMatrixInfo->nPRGSize= fapiPuschpdu->;
      iapiPuschPdu->stBeamMatrixInfo->pMatrixBeamID= fapiPuschpdu->;
      iapiPuschPdu->stBeamMatrixInfo->pMatrixBuff = fapiPuschpdu->;
   }
   */
}

void fillPucchPduInfo(ULCCHUCIPDUStruct *iapiPucchPdu, fapi_ul_pucch_pdu_t *fapiPucchpdu, \
   fapi_vendor_ul_pucch_pdu_t *fapiVendorPucchPdu, PucchInfo *pucchInfo)
{
   iapiPucchPdu->sPDUHdr.nPDUType  = UL_PDU_TYPE_ULCCH_UCI;
   iapiPucchPdu->sPDUHdr.nPDUSize = sizeof(ULCCHUCIPDUStruct);
   iapiPucchPdu->nRNTI = fapiPucchpdu->rnti;
   pucchInfo->handle = iapiPucchPdu->nUEId = fapiPucchpdu->handle;
   
   //PBORLA - Following parameter from 959 to 963 does not supported in the current release
   iapiPucchPdu->nBWPSize = fapiPucchpdu->bwpSize;
   iapiPucchPdu->nBWPStart = fapiPucchpdu->bwpStart;
   iapiPucchPdu->nSubcSpacing = fapiPucchpdu->subCarrierSpacing;
   iapiPucchPdu->nCpType = fapiPucchpdu->cyclicPrefix;
   iapiPucchPdu->nULType = 0;

   pucchInfo->pucchFormat = iapiPucchPdu->nFormat = fapiPucchpdu->formatType;
   iapiPucchPdu->nID = fapiPucchpdu->dataScramblingId;

   //PBORLA nScramID is Using for reference signal scrambling valid for Format 2
   iapiPucchPdu->nScramID = 0;

   iapiPucchPdu->nSRPriodAriv = fapiPucchpdu->srFlag;
   iapiPucchPdu->nStartSymbolx = fapiPucchpdu->startSymbolIndex;
   iapiPucchPdu->nStartPRB = fapiPucchpdu->prbStart;
   iapiPucchPdu->n2ndHopPRB = fapiPucchpdu->secondHopPrb;
   iapiPucchPdu->nPRBs = fapiPucchpdu->prbSize;
   iapiPucchPdu->nSymbols = fapiPucchpdu->nrOfSymbols;
   iapiPucchPdu->nFreqHopFlag = fapiPucchpdu->freqHopFlag;
   iapiPucchPdu->nM0 = fapiPucchpdu->initialCyclicShift;
   iapiPucchPdu->nAddDmrsFlag = fapiPucchpdu->addDmrsFlag;
   
   //PBORLA nMaxCodeRate does not supported in the current release
#ifdef INTEL_L1_V22_07
   //iapiPucchPdu->nMaxCodeRate = 0;
#endif

   iapiPucchPdu->nFmt1OrthCCodeIdx = fapiPucchpdu->timeDomainOccIdx;
   iapiPucchPdu->nFmt4OrthCCodeIdx = fapiPucchpdu->preDftOccIdx;
   iapiPucchPdu->nFmt4OrthCCodeLength = fapiPucchpdu->preDftOccLen;
   if(fapiPucchpdu->pi2Bpsk)
   {
      iapiPucchPdu->modType = 1;
   }
   else
   {
      iapiPucchPdu->modType = 2;
   }

   iapiPucchPdu->nBitLenUci = fapiPucchpdu->bitLenHarq;

   /* PBORLA :
    * nNrofRxRU  is not supported in the current release*/
   iapiPucchPdu->nNrofRxRU = 1; //PBORLA
   iapiPucchPdu->nGroupId = fapiVendorPucchPdu->group_id;

   /* PBORLA :
    * nRxRUIdx is not supported in the current release*/
   for(int i=0;i<iapiPucchPdu->nNrofRxRU;i++)
   {
      iapiPucchPdu->nRxRUIdx[i] = fapiVendorPucchPdu->rx_ru_idx[i]; 
   }
   iapiPucchPdu->nCSIPart2 = fapiPucchpdu->bitLenCsiPart2; 

   /* PBORLA :
    * nBeamId is not supported in the current release*/
   iapiPucchPdu->nBeamId[0] = fapiPucchpdu->beamforming.rx_bfi[0].beamIdx[0].beamidx;

   /* PBORLA
    * iapiPucchPdu->rsv2
    * iapiPucchPdu->sCSIPart2
    **/
}

void convertUlTtiReqFapiToIapi(uint8_t phyHandle, p_fapi_api_queue_elem_t ulTtiReqElem, \
      uint32_t l2ToL1MsgLen, PMAC2PHY_QUEUE_EL iapiQueueElem)
{
   uint8_t ueIdx = 0;
   uint8_t pduIdx = 0;
   uint16_t grpIdx = 0;
   uint8_t count =0;
   PDUStruct   *pduInfo;
   fapi_vendor_msg_t *vendorMsg;
   PRACHPDUStruct *prachPdu;
   ULSCHPDUStruct *ulSchPdu;
   ULCCHUCIPDUStruct *ulCchPdu;
   SRSPDUStruct *srsPdu;
   PULConfigRequestStruct ulConfig = NULLP;
   fapi_ul_tti_req_t *ulTtiReq = NULLP;
   p_fapi_api_queue_elem_t ulTtiElem;
   p_fapi_api_queue_elem_t  nextElem = NULLP;
   UlSlotInfo *ulSlotInfo = NULLP;

   if(ulTtiReqElem)
   {
      nextElem = ulTtiReqElem;
      while(nextElem)
      {
         if(nextElem->msg_type == FAPI_VENDOR_MESSAGE)
         {
            ulTtiElem = nextElem;
            vendorMsg = (fapi_vendor_msg_t *)(ulTtiElem + 1);
            break;
         }
         nextElem = nextElem->p_next;
      }

      ulTtiReq = (fapi_ul_tti_req_t *)(ulTtiReqElem + 1);
   }


   /* Fill IAPI Queue element */
   memset(iapiQueueElem, 0, l2ToL1MsgLen);
   iapiQueueElem->nAlignOffset = sizeof(ULConfigRequestStruct);
   iapiQueueElem->nNumMessageInBlock = 1;
   iapiQueueElem->nMessageType = MSG_TYPE_PHY_UL_CONFIG_REQ;
   iapiQueueElem->nMessageLen = l2ToL1MsgLen;
   iapiQueueElem->pNext = NULLP;

   /* Fill IAPI UL TTI Request */
   ulConfig = (PULConfigRequestStruct)(iapiQueueElem+1);
   ulConfig->sMsgHdr.nMessageType = MSG_TYPE_PHY_UL_CONFIG_REQ;
   ulConfig->sMsgHdr.nMessageLen = sizeof(ULConfigRequestStruct) - sizeof(L1L2MessageHdr);
   ulConfig->sSFN_Slot.nSFN = ulTtiReq->sfn;
   ulConfig->sSFN_Slot.nSlot = ulTtiReq->slot;
   ulConfig->sSFN_Slot.nCarrierIdx = phyHandle;
   ulConfig->sSFN_Slot.nSym = 0;
   ulConfig->sSFN_Slot.nRsv = 0;
   
   ulSlotInfo = &gUlSlotInfo[ulConfig->sSFN_Slot.nSFN][ulConfig->sSFN_Slot.nSlot];
   setUlSlotInfo(ulConfig->sSFN_Slot.nSFN, ulConfig->sSFN_Slot.nSlot, ulSlotInfo); 

   ulConfig->nGroup = ulTtiReq->nGroup;
   ulConfig->nPDU = ulTtiReq->nPdus;
   ulConfig->nUlsch = ulTtiReq->nUlsch;
   ulConfig->nUlcch =ulTtiReq->nUlcch;
   ulConfig->nRachPresent =ulTtiReq->rachPresent;
   
   /* calculated from number of SRS PDUs present in pdu list */
   for(pduIdx = 0; pduIdx < ulTtiReq->nPdus; pduIdx++)
   {
      switch(ulTtiReq->pdus[pduIdx].pduType)
      {
         case FAPI_SRS_PDU_TYPE:
            {
               count++;
               break;
            }
      }
   }
   ulConfig->nUlsrs = count;
   
   for (grpIdx = 0 ; grpIdx < ulTtiReq->nGroup ; grpIdx++)
   {
      ulConfig->sPUSCHGroupInfoStruct[grpIdx].nUE = ulTtiReq->ueGrpInfo[grpIdx].nUe;
      for(ueIdx=0; ueIdx < ulConfig->sPUSCHGroupInfoStruct[grpIdx].nUE; ueIdx++)
      {
         ulConfig->sPUSCHGroupInfoStruct[grpIdx].nPduIdx[ueIdx] = ulTtiReq->ueGrpInfo[grpIdx].pduIdx[ueIdx];
      }
   }

   pduInfo = (PDUStruct *) ((uint8_t *)ulConfig->sULPDU); /*PDU Struct starts from here*/
   for(pduIdx = 0; pduIdx < ulTtiReq->nPdus; pduIdx++)
   {
      switch(ulTtiReq->pdus[pduIdx].pduType)
      {
         case FAPI_PRACH_PDU_TYPE:
            {
               prachPdu =(PRACHPDUStruct*) ((uint8_t *)pduInfo);
               fillPrachPduInfo(prachPdu, &ulTtiReq->pdus[pduIdx].pdu.prach_pdu, &ulSlotInfo->rachInfo);
               ulSlotInfo->rachPresent = true;
               break;
            }

         case FAPI_PUSCH_PDU_TYPE:
            {
               ulSchPdu = (ULSCHPDUStruct*)((uint8_t *)pduInfo);
               fillPuschPduInfo(ulSchPdu, &ulTtiReq->pdus[pduIdx].pdu.pusch_pdu, \
                  &vendorMsg->p7_req_vendor.ul_tti_req.ul_pdus[pduIdx].pdu.pusch_pdu, \
                  &ulSlotInfo->puschInfo[ulSlotInfo->numPusch]);
               ulSlotInfo->numPusch++;
               break;
            }

         case FAPI_PUCCH_PDU_TYPE:
            {
               ulCchPdu = (ULCCHUCIPDUStruct*)((uint8_t *)pduInfo);
               fillPucchPduInfo(ulCchPdu, &ulTtiReq->pdus[pduIdx].pdu.pucch_pdu, \
                  &vendorMsg->p7_req_vendor.ul_tti_req.ul_pdus[pduIdx].pdu.pucch_pdu, \
                  &ulSlotInfo->pucchInfo[ulSlotInfo->numPucch]);
                  ulSlotInfo->numPucch++;
               break;
            }

         case FAPI_SRS_PDU_TYPE:
            {
               srsPdu = ( SRSPDUStruct*)((uint8_t *)pduInfo);
               fillSrsPduInfo(srsPdu, &ulTtiReq->pdus[pduIdx].pdu.srs_pdu, &vendorMsg->p7_req_vendor.ul_tti_req.ul_pdus[pduIdx].pdu.srs_pdu);
               break;
            }
      }

      pduInfo = (PDUStruct *)((uint8_t *)pduInfo + pduInfo->nPDUSize);
   }
}

void SendIapiToL1(PMAC2PHY_QUEUE_EL iapiQueueElem)
{
   uint8_t ret = ROK;
   uint16_t msgLen =0;
   void * wlsHdlr = NULLP;
   PMAC2PHY_QUEUE_EL currMsg = NULLP;

   mtGetWlsHdl(&wlsHdlr);
   if(iapiQueueElem)
   {
      /* Sending first block */
      currMsg = iapiQueueElem;
      msgLen = currMsg->nMessageLen;
      addWlsBlockToFree(currMsg, msgLen, (lwrMacCb.phySlotIndCntr-1));
      ret = WLS_Put(wlsHdlr, WLS_VA2PA(wlsHdlr, currMsg), msgLen, currMsg->nMessageType, WLS_SG_FIRST);
      if(ret != 0)
      {
         DU_LOG("\nERROR  -->  LWR MAC : Failure in sending message to PHY");
         return RFAILED;
      }
      currMsg = currMsg->pNext;

      while(currMsg)
      {
         /* Sending the next msg */
         msgLen = currMsg->nMessageLen;
         addWlsBlockToFree(currMsg, msgLen, (lwrMacCb.phySlotIndCntr-1));
         if(currMsg->pNext != NULLP)
         {
            ret = WLS_Put(wlsHdlr, WLS_VA2PA(wlsHdlr, currMsg), msgLen, currMsg->nMessageType, WLS_SG_NEXT);
            if(ret != 0)
            {
               DU_LOG("\nERROR  -->  LWR MAC : Failure in sending message to PHY");
               return RFAILED;
            }
            currMsg = currMsg->pNext;
         }
         else
         {
            /* Sending last msg */
            ret = WLS_Put(wlsHdlr, WLS_VA2PA(wlsHdlr, currMsg), msgLen, currMsg->nMessageType, WLS_SG_LAST);
            if(ret != 0)
            {
               DU_LOG("\nERROR  -->  LWR MAC : Failure in sending message to PHY");
               return RFAILED;
            }
            currMsg = NULLP;
         }
      }
   }
}

void FapiToIapiConvertAndSend(p_fapi_api_queue_elem_t headerElem)
{
   uint32_t l2ToL1MsgLen;
   PMAC2PHY_QUEUE_EL iapiQueueElem = NULLP, prevIapiQueueElem = NULLP;
   PMAC2PHY_QUEUE_EL dlCfgReqQueueElem = NULLP, ulCfgReqQueueElem = NULLP;
   PMAC2PHY_QUEUE_EL txDataReqQueueElem = NULLP, ulDciReqQueueElem = NULLP;

   p_fapi_api_queue_elem_t  fapiQueueElem = NULLP;
   fapi_msg_header_t *msgHeader = NULLP;

   /* Fetch FAPI API queue element from header element */
   if(headerElem)
   {
      msgHeader = (fapi_msg_header_t *)(headerElem + 1);
      fapiQueueElem = headerElem->p_next;
   }

   /* Based on message type, call respective handler */
   while(fapiQueueElem)
   {
      switch(fapiQueueElem->msg_type)
      {
         case FAPI_PARAM_REQUEST:
            {
               break;
            }
         case FAPI_CONFIG_REQUEST:         
            {
               l2ToL1MsgLen = (sizeof(CONFIGREQUESTStruct) + sizeof(MAC2PHY_QUEUE_EL));
               LWR_MAC_ALLOC(iapiQueueElem, l2ToL1MsgLen);
               if(iapiQueueElem)
               {
                  convertConfigReqFapiToIapi(msgHeader->handle, fapiQueueElem, l2ToL1MsgLen, iapiQueueElem);
               }
               break;
            }
         case FAPI_START_REQUEST:
            {
               l2ToL1MsgLen = (sizeof(STARTREQUESTStruct) + sizeof(MAC2PHY_QUEUE_EL));
               LWR_MAC_ALLOC(iapiQueueElem, l2ToL1MsgLen);
               if(iapiQueueElem)
               {
                  convertStartReqFapiToIapi(msgHeader->handle, fapiQueueElem, l2ToL1MsgLen, iapiQueueElem);
               }
               break;
            }
         case FAPI_DL_TTI_REQUEST:
            {
               l2ToL1MsgLen = (sizeof(DLConfigRequestStruct) + sizeof(MAC2PHY_QUEUE_EL));
               LWR_MAC_ALLOC(dlCfgReqQueueElem, WLS_BUFFER_SIZE);
               if(dlCfgReqQueueElem)
               {
                  convertDlTtiReqFapiToIapi(msgHeader->handle, fapiQueueElem, l2ToL1MsgLen, dlCfgReqQueueElem);
                  prevIapiQueueElem = iapiQueueElem = dlCfgReqQueueElem;
               }
               break;
            }
         case FAPI_UL_TTI_REQUEST:
            {
               l2ToL1MsgLen = (sizeof(ULConfigRequestStruct) + sizeof(MAC2PHY_QUEUE_EL));
               LWR_MAC_ALLOC(ulCfgReqQueueElem, WLS_BUFFER_SIZE);
               if(ulCfgReqQueueElem)
               {
                  convertUlTtiReqFapiToIapi(msgHeader->handle, fapiQueueElem, l2ToL1MsgLen, ulCfgReqQueueElem);
                  prevIapiQueueElem->pNext = ulCfgReqQueueElem;
                  prevIapiQueueElem = ulCfgReqQueueElem;
               }
               break;
            }
         case FAPI_UL_DCI_REQUEST:
            {
               l2ToL1MsgLen = (sizeof(ULDCIRequestStruct) + sizeof(MAC2PHY_QUEUE_EL));
               LWR_MAC_ALLOC(ulDciReqQueueElem, WLS_BUFFER_SIZE);
               if(ulDciReqQueueElem)
               {
                  convertUlDciReqFapiToIapi(msgHeader->handle, fapiQueueElem, l2ToL1MsgLen, ulDciReqQueueElem);
                  prevIapiQueueElem->pNext = ulDciReqQueueElem;
                  prevIapiQueueElem = ulDciReqQueueElem;
               }
               break;
            }
         case FAPI_TX_DATA_REQUEST:
            {
               l2ToL1MsgLen = (sizeof(TXRequestStruct) + sizeof(MAC2PHY_QUEUE_EL));
               LWR_MAC_ALLOC(txDataReqQueueElem, WLS_BUFFER_SIZE);
               if(txDataReqQueueElem)
               {
                  convertTxDataReqFapiToIapi(msgHeader->handle, fapiQueueElem, l2ToL1MsgLen, txDataReqQueueElem);
                  prevIapiQueueElem->pNext = txDataReqQueueElem;
                  prevIapiQueueElem = txDataReqQueueElem;
               }
               break;
            }
         case FAPI_STOP_REQUEST:
            {
               l2ToL1MsgLen = (sizeof(STOPREQUESTStruct) + sizeof(MAC2PHY_QUEUE_EL));
               LWR_MAC_ALLOC(iapiQueueElem, l2ToL1MsgLen);
               if(iapiQueueElem)
               {
                  convertStopReqFapiToIapi(msgHeader->handle, fapiQueueElem, l2ToL1MsgLen, iapiQueueElem);
               }
               break;
            }
      }
      fapiQueueElem = fapiQueueElem->p_next;
   }

   /* Send to L1 */
   SendIapiToL1(iapiQueueElem);
}
