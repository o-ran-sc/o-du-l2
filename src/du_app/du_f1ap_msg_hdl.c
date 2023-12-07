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

/* This file contains F1AP message handler functions */
#include "common_def.h"
#include "du_tmr.h"
#include "ckw.h"
#include "ckw.x"
#include "kwu.h"
#include "kwu.x"
#include "lkw.h"
#include "lrg.h"
#include "legtp.h"
#include "lkw.x"
#include "lrg.x"
#include "F1AP-PDU.h"
#include "InitiatingMessage.h"
#include "SuccessfulOutcome.h"
#include "du_app_mac_inf.h"
#include "du_cfg.h"
#include "du_app_rlc_inf.h"
#include "du_e2ap_mgr.h"
#include "du_e2ap_msg_hdl.h"
#include "du_mgr_main.h"
#include "du_mgr.h"
#include "du_utils.h"
#include "du_ue_mgr.h"
#include "RAT-Type.h"
#include "NRFreqInfo.h"
#include "NRCGI.h"
#include "TDD-Info.h"
#include "NR-Mode-Info.h"
#include "ServedPLMNs-List.h"
#include "GNB-DU-Served-Cells-List.h"
#include "Served-Cell-Information.h"
#include "ProtocolExtensionContainer.h"
#include "RRC-Version.h"
#include "FDD-Info.h"
#include "FreqBandNrItem.h"
#include "ServedPLMNs-Item.h"
#include "GNB-DU-Served-Cells-Item.h"
#include "SliceSupportItem.h"
#include "FeatureSetUplinkPerCC.h"
#include "SliceSupportItem.h"
#include "Served-Cells-To-Modify-Item.h"
#include "Served-Cells-To-Delete-List.h"
#include "Served-Cells-To-Delete-Item.h"
#include "QoSInformation.h"
#include "ULUPTNLInformation-ToBeSetup-List.h"
#include "DRB-Information.h"
#include "DRBs-ToBeSetup-Item.h"
#include "DRBs-ToBeSetupMod-Item.h"
#include "DRBs-ToBeModified-Item.h"
#include "CUtoDURRCInformation.h"
#include "SCell-ToBeSetup-List.h"
#include "SRBs-ToBeSetup-List.h"
#include "DRBs-ToBeSetup-List.h"
#include "SRBs-ToBeSetup-Item.h"
#include "DRBs-ToBeModified-List.h"
#include "RRCContainer.h"
#include "UE-CapabilityRAT-ContainerList.h"
#include "DRBs-Setup-List.h"
#include "CellGroupConfig.h"
#include "ResetAll.h"
#include "ResetType.h"
#include "Cells-to-be-Activated-List.h"
#include "GNB-CU-Name.h"
#include "SRBs-SetupMod-List.h"
#include "DRBs-SetupMod-List.h"
#include "DRBs-ToBeSetupMod-List.h"
#include "PagingCell-Item.h"
#include "PagingCell-list.h"
#include "QoS-Characteristics.h"
#include "ULUPTNLInformation-ToBeSetup-Item.h"
#include "Flows-Mapped-To-DRB-Item.h"
#include "NonDynamic5QIDescriptor.h"
#include "Dynamic5QIDescriptor.h"
#include "FeatureSetDownlinkPerCC.h"
#include "FeatureSets.h"
#include "UE-NR-Capability.h"
#include "UE-CapabilityRAT-Container.h"
#include "UE-CapabilityRAT-ContainerListRRC.h"
#include "GNB-DU-System-Information.h"
#include "CellGroupConfigRrc.h"
#include "MAC-CellGroupConfig.h"
#include "SchedulingRequestConfig.h"
#include "SchedulingRequestToAddMod.h"
#include "BSR-Config.h"
#include "TAG-Config.h"
#include "TAG.h"
#include "PHR-Config.h"
#include "RLC-Config.h"
#include "UL-AM-RLC.h"
#include "DL-AM-RLC.h"
#include "LogicalChannelConfig.h"
#include "RLC-BearerConfig.h"
#include "PhysicalCellGroupConfig.h"
#include "SpCellConfig.h"
#include "TDD-UL-DL-ConfigDedicated.h"
#include "ServingCellConfig.h"
#include "ControlResourceSet.h"
#include "SearchSpace.h"
#include "PDCCH-Config.h"
#include "PDSCH-TimeDomainResourceAllocation.h"
#include "PDSCH-TimeDomainResourceAllocationList.h"
#include "PDSCH-CodeBlockGroupTransmission.h"
#include "PDSCH-ServingCellConfig.h"
#include "DMRS-DownlinkConfig.h"
#include "PDSCH-Config.h"
#include "BWP-DownlinkDedicated.h"
#include "BWP-Downlink.h"
#include "PUSCH-TimeDomainResourceAllocation.h"
#include "PUSCH-TimeDomainResourceAllocationList.h"
#include "DMRS-UplinkConfig.h"
#include "PUSCH-Config.h"
#include "SRS-ResourceId.h"
#include "SRS-Resource.h"
#include "SRS-ResourceSet.h"
#include "SRS-Config.h"
#include "BWP-UplinkDedicated.h"
#include "PUSCH-ServingCellConfig.h"
#include "UplinkConfig.h"
#include "DUtoCURRCContainer.h"
#include "GBR-QoSFlowInformation.h"
#include "QoSFlowLevelQoSParameters.h"
#include "PUCCH-Config.h"
#include "PUCCH-ResourceSet.h"
#include "PUCCH-Resource.h"
#include "PUCCH-PowerControl.h"
#include "P0-PUCCH.h"
#include "PUCCH-PathlossReferenceRS.h"
#include "PUCCH-format0.h"
#include "PUCCH-format1.h"
#include "PUCCH-format2.h"
#include "PUCCH-format3.h"
#include "PUCCH-format4.h"
#include "PUCCH-FormatConfig.h"
#include "SchedulingRequestResourceConfig.h"
#include<ProtocolIE-Field.h>
#include "ProtocolExtensionField.h"
#include "odu_common_codec.h"
#include "du_mgr.h"
#include "du_cell_mgr.h"
#include "du_f1ap_msg_hdl.h"
#include "DRBs-Setup-Item.h"
#include "DLUPTNLInformation-ToBeSetup-List.h"
#include "DLUPTNLInformation-ToBeSetup-Item.h"
#include "UPTransportLayerInformation.h"
#include "GTPTunnel.h"
#include "SupportedSULFreqBandItem.h"
#include "du_f1ap_conversions.h"
#include "CNUEPagingIdentity.h"
#include "PCCH-Config.h"
#include "SCS-SpecificCarrier.h"
#include "FrequencyInfoDL.h"
#include "DownlinkConfigCommon.h"
#include "FrequencyInfoUL.h"
#include "UplinkConfigCommon.h"
#include "TDD-UL-DL-ConfigCommon.h"
#include "RACH-ConfigDedicated.h"
#include "CFRA-SSB-Resource.h"
#include "BWP-UplinkCommon.h"
#include "ReconfigurationWithSync.h"
#include "BCCH-DL-SCH-Message.h"
#include "du_sys_info_hdl.h"
#include "DRX-ConfigRrc.h"
#include "MeasurementTimingConfigurationRrc.h"
#include "MeasurementTimingConfigurationRrc-IEs.h"
#include "MeasTimingList.h"
#include "MeasTiming.h"
#include "Cells-Status-List.h"
#include "Cells-Status-Item.h"

#ifdef O1_ENABLE
#include "CmInterface.h"
extern StartupConfig g_cfg;
#endif

DuCfgParams duCfgParam;

/******************************************************************
 *
 * @brief Function to fetch lcId based on DRB Id
 *
 * @details
 *
 *    Function : fetchLcId
 *
 *    @params[in] drbId
 *
 *    Functionality: Function to fetch lcId based on DRB Id
 *
 * Returns: lcId - SUCCESS
 *          RFAILED - FAILURE
 *****************************************************************/

uint8_t fetchLcId(uint8_t drbId)
{
   uint8_t cellIdx = 0, ueIdx = 0, lcIdx = 0, numLcs = 0, lcId = 0;

   for(cellIdx = 0; cellIdx < MAX_NUM_CELL; cellIdx++)
   {
      for(ueIdx = 0; ueIdx < MAX_NUM_UE; ueIdx++)
      {
         if(duCb.actvCellLst[cellIdx] != NULLP)
         {
            numLcs = duCb.actvCellLst[cellIdx]->ueCb[ueIdx].duRlcUeCfg.numLcs;
            for(lcIdx = 0; lcIdx < numLcs; lcIdx++)
            {
               if(duCb.actvCellLst[cellIdx]->ueCb[ueIdx].duRlcUeCfg.rlcLcCfg[lcIdx].rlcBearerCfg.rbId == drbId && \
                  duCb.actvCellLst[cellIdx]->ueCb[ueIdx].duRlcUeCfg.rlcLcCfg[lcIdx].rlcBearerCfg.rbType == RB_TYPE_DRB)
               {
                  lcId = duCb.actvCellLst[cellIdx]->ueCb[ueIdx].duRlcUeCfg.rlcLcCfg[lcIdx].rlcBearerCfg.lcId;
                  return lcId;
               }
            }
         }
      }
   }
   DU_LOG("\nERROR   -->  DU_APP: fetchLcId() failed for drbId %d", drbId);
   return RFAILED;
}

/*******************************************************************
*
* @brief Adding F1AP pdu to reserved pdu list
*
* @details
*
*    Function : addToReservedF1apPduList 
*
*    Functionality: Adding F1AP pdu to reserved pdu list.
*     These pdu are awaiting aknowledgment from CU
*
* @params[in] uint8_t transId, F1AP_PDU_t *f1apMsg
*
* @return ROK - success
*         RFAILED - failure
*
* ****************************************************************/
void addToReservedF1apPduList(uint8_t transId, F1AP_PDU_t *f1apPdu)
{
   CmLList         *node = NULLP;
   ReservedF1apPduInfo *pduInfo = NULLP;
   DU_ALLOC(pduInfo, sizeof(ReservedF1apPduInfo));
   if(pduInfo)
   {
      DU_ALLOC(node, sizeof(CmLList));
      if(node)
      {
         pduInfo->transId = transId;
         pduInfo->f1apMsg = (void*) f1apPdu;

         node->node = (PTR)pduInfo;
         cmLListAdd2Tail(&duCb.reservedF1apPduList, node);
      }
   }
}

/*******************************************************************
*
* @brief searching for F1AP pdu from ReservedF1apPduList 
*
* @details
*
*    Function : searchFromReservedF1apPduList 
*
*    Functionality: searching for F1AP pdu information
*
* @params[in] uint8_t transId
*
* @return pointer to F1AP_PDU_t
*
* ****************************************************************/

CmLList *searchFromReservedF1apPduList(uint8_t transId)
{
   CmLList         *node;
   ReservedF1apPduInfo *f1apPdu;
   if(duCb.reservedF1apPduList.count)
   {
      CM_LLIST_FIRST_NODE(&duCb.reservedF1apPduList, node);
      while(node)
      {
         f1apPdu = (ReservedF1apPduInfo*)node->node;
         if(f1apPdu->transId == transId)
         {
            return node;
         }
         node = node->next;
      }
   }
   return NULL;
}

/*******************************************************************
*
* @brief deleting F1AP pdu information from ReservedF1apPduList
*
* @details
*
*    Function : deleteFromReservedF1apPduList 
*
*    Functionality: deleting pdu information from ReservedF1apPduList
*
* @params[in] CmLList *node 
*
* @return void 
*
* ****************************************************************/

void deleteFromReservedF1apPduList(CmLList *node)
{
   ReservedF1apPduInfo *f1apPdu;

   if(node != NULL)
   {
      f1apPdu = (ReservedF1apPduInfo *)node->node;
      cmLListDelFrm(&duCb.reservedF1apPduList, node);
      DU_FREE(f1apPdu, sizeof(ReservedF1apPduInfo));
      DU_FREE(node, sizeof(CmLList));
      node = NULL;
   }
}

/*******************************************************************
 *
 * @brief Builds Uplink Info for NR 
 *
 * @details
 *
 *    Function : BuildULNRInfo
 *
 *    Functionality: Building NR Uplink Info
 *
 * @params[in] NRFreqInfo_t *ulnrfreq
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildULNRInfo(NRFreqInfo_t *ulnrfreq)
{
   uint8_t idx=0;
   ulnrfreq->nRARFCN = duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.\
		       fdd.ulNrFreqInfo.nrArfcn;
   ulnrfreq->freqBandListNr.list.count = 1;
   ulnrfreq->freqBandListNr.list.size = sizeof(FreqBandNrItem_t *);
   DU_ALLOC(ulnrfreq->freqBandListNr.list.array,ulnrfreq->freqBandListNr.list.size);
   if(ulnrfreq->freqBandListNr.list.array == NULLP)
   {
      return RFAILED;
   }
   for(idx=0;idx<ulnrfreq->freqBandListNr.list.count;idx++)
   {
      DU_ALLOC(ulnrfreq->freqBandListNr.list.array[idx],sizeof(FreqBandNrItem_t));
      if(ulnrfreq->freqBandListNr.list.array[idx] == NULLP)
      {
	 return RFAILED;
      }
   }
   ulnrfreq->freqBandListNr.list.array[0]->freqBandIndicatorNr = \
								 duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.\
								 freqBand[0].nrFreqBand;
   ulnrfreq->freqBandListNr.list.array[0]->supportedSULBandList.list.count = SUL_BAND_COUNT;
   return ROK;
}
/*******************************************************************
 *
 * @brief Builds Downlink NR Info 
 *
 * @details
 *
 *    Function : BuildDLNRInfo
 *
 *    Functionality: Building Downlink NR Info
 *    
 * @params[in] NRFreqInfo_t *dlnrfreq
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildDLNRInfo(NRFreqInfo_t *dlnrfreq)
{
   uint8_t idx=0;
   dlnrfreq->nRARFCN = duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.\
		       fdd.dlNrFreqInfo.nrArfcn;
   dlnrfreq->freqBandListNr.list.count = 1;
   dlnrfreq->freqBandListNr.list.size = sizeof(FreqBandNrItem_t *);
   DU_ALLOC(dlnrfreq->freqBandListNr.list.array,dlnrfreq->freqBandListNr.list.size);
   if(dlnrfreq->freqBandListNr.list.array == NULLP)
   {
      return RFAILED;   
   }
   for(idx=0;idx< dlnrfreq->freqBandListNr.list.count;idx++)
   {
      DU_ALLOC(dlnrfreq->freqBandListNr.list.array[idx],sizeof(FreqBandNrItem_t));
      if(dlnrfreq->freqBandListNr.list.array[idx] == NULLP)
      {
	 return RFAILED;
      }
   }   
   dlnrfreq->freqBandListNr.list.array[0]->freqBandIndicatorNr = \
								 duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.\
								 freqBand[0].nrFreqBand;
   dlnrfreq->freqBandListNr.list.array[0]->supportedSULBandList.list.count = SUL_BAND_COUNT;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds Nrcgi 
 *
 * @details
 *
 *    Function : BuildNrcgi
 *
 *    Functionality: Building the PLMN ID and NR Cell id
 *
 * @params[in] NRCGI_t *nrcgi
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildNrcgi(NRCGI_t *nrcgi)
{
   uint8_t ret;
   uint8_t byteSize = 5;
   /* Allocate Buffer Memory */
   nrcgi->pLMN_Identity.size = PLMN_SIZE * sizeof(uint8_t);
   DU_ALLOC(nrcgi->pLMN_Identity.buf, nrcgi->pLMN_Identity.size);
   if(nrcgi->pLMN_Identity.buf == NULLP)
   {
      return RFAILED;
   }
   ret = buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn,\
	 nrcgi->pLMN_Identity.buf); // Building PLMN function
   if(ret != ROK)
   {
      return RFAILED;
   }
   /*nrCellIdentity*/
   nrcgi->nRCellIdentity.size = byteSize * sizeof(uint8_t);
   DU_ALLOC(nrcgi->nRCellIdentity.buf, nrcgi->nRCellIdentity.size); 
   if(nrcgi->nRCellIdentity.buf == NULLP)
   {
      return RFAILED;
   }
   fillBitString(&nrcgi->nRCellIdentity, ODU_VALUE_FOUR, ODU_VALUE_FIVE, duCfgParam.sib1Params.cellIdentity);

   return ROK;
}
/*******************************************************************
 *
 * @brief Builds FiveGStac 
 *
 * @details
 *
 *    Function : BuildFiveGSTac
 *
 *    Functionality: Building the FiveGSTac
 *
 * @params[in] OCTET_STRING_t *fivegsTac
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildFiveGSTac(Served_Cell_Information_t *servcell)
{
   DU_ALLOC(servcell->fiveGS_TAC,sizeof(FiveGS_TAC_t));
   if(servcell->fiveGS_TAC == NULLP)
   {
      return RFAILED;
   }
   servcell->fiveGS_TAC->size = 3 * sizeof(uint8_t);
   DU_ALLOC(servcell->fiveGS_TAC->buf,\
	 sizeof(servcell->fiveGS_TAC->size));
   if(servcell->fiveGS_TAC->buf == NULLP)
   {
      return RFAILED;
   }
   servcell->fiveGS_TAC->buf[0] = 0;
   servcell->fiveGS_TAC->buf[1] = 0;
   servcell->fiveGS_TAC->buf[2] = duCfgParam.srvdCellLst[0].duCellInfo.tac;
   return ROK;  
}

/*******************************************************************
 *
 * @brief fill nr frequency information
 *
 * @details
 *
 *    Function : fillNrTddInfo 
 *
 *    Functionality: fill nr frequency information
 *
 * @params[in] NRFreqInfo_t freqInfo
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillNrTddInfo(TDD_Info_t *tddInfo)
{
   uint8_t elementCnt = 1, freqBandListIdx = 0, supportedBandIdx = 0;
   NRFreqInfo_t *freqInfo = NULLP;

   if(tddInfo == NULLP)
   {
      DU_LOG("\nERROR  --> DU APP : Null pointer received at fillNrTddInfo");
      return RFAILED;
   }
   
   freqInfo = &tddInfo->nRFreqInfo;
   freqInfo->nRARFCN = duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.nrArfcn; 

   freqInfo->freqBandListNr.list.count = elementCnt; 
   freqInfo->freqBandListNr.list.size = freqInfo->freqBandListNr.list.count  * sizeof(FreqBandNrItem_t *);
   DU_ALLOC(freqInfo->freqBandListNr.list.array, freqInfo->freqBandListNr.list.size );
   if(!freqInfo->freqBandListNr.list.array)
   {
      DU_LOG("\nERROR  --> DU APP : Memory allocation failed at fillNrTddInfo");
      return RFAILED;
   }

   for(freqBandListIdx = 0; freqBandListIdx<freqInfo->freqBandListNr.list.count; freqBandListIdx++)
   {
      DU_ALLOC(freqInfo->freqBandListNr.list.array[freqBandListIdx],  sizeof(FreqBandNrItem_t ));
      if(!freqInfo->freqBandListNr.list.array[freqBandListIdx])
      {
         DU_LOG("\nERROR  --> DU APP : Memory allocation failed at fillNrTddInfo");
         return RFAILED;
      }

      freqInfo->freqBandListNr.list.array[freqBandListIdx]->freqBandIndicatorNr = duCfgParam.srvdCellLst[0].duCellInfo.\
      f1Mode.mode.tdd.nrFreqInfo.freqBand[0].nrFreqBand;

      freqInfo->freqBandListNr.list.array[freqBandListIdx]->supportedSULBandList.list.count = elementCnt;
      freqInfo->freqBandListNr.list.array[freqBandListIdx]->supportedSULBandList.list.size = freqInfo->freqBandListNr.list.array[freqBandListIdx]->\
      supportedSULBandList.list.count * sizeof(SupportedSULFreqBandItem_t*);

      DU_ALLOC(freqInfo->freqBandListNr.list.array[freqBandListIdx]->supportedSULBandList.list.array,\
            freqInfo->freqBandListNr.list.array[freqBandListIdx]->supportedSULBandList.list.size);
      if(!freqInfo->freqBandListNr.list.array[freqBandListIdx]->supportedSULBandList.list.array)
      {
         DU_LOG("\nERROR  --> DU APP : Memory allocation failed at fillNrTddInfo");
         return RFAILED;
      }

      for(supportedBandIdx = 0; supportedBandIdx<freqInfo->freqBandListNr.list.array[freqBandListIdx]->supportedSULBandList.list.count; supportedBandIdx++)
      {
         DU_ALLOC(freqInfo->freqBandListNr.list.array[freqBandListIdx]->supportedSULBandList.list.array[supportedBandIdx],\
               sizeof(SupportedSULFreqBandItem_t));
         if(!freqInfo->freqBandListNr.list.array[freqBandListIdx]->supportedSULBandList.list.array[supportedBandIdx])
         {
            DU_LOG("\nERROR  --> DU APP : Memory allocation failed at fillNrTddInfo");
            return RFAILED;
         }

         freqInfo->freqBandListNr.list.array[freqBandListIdx]->supportedSULBandList.list.array[supportedBandIdx]->freqBandIndicatorNr =\
         duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.freqBand[0].sulBand[0];
      }
   }

   tddInfo->transmission_Bandwidth.nRSCS = duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.sulInfo.sulTxBw.nrScs;
   tddInfo->transmission_Bandwidth.nRNRB = duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.tdd.nrFreqInfo.sulInfo.sulTxBw.nrb;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds NR Mode 
 *
 * @details
 *
 *    Function : BuildNrMode
 *
 *    Functionality: Building the NR Mode
 *
 * @params[in] NR_Mode_Info_t *fdd
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildNrMode(NR_Mode_Info_t *mode)
{
   uint8_t BuildDLNRInforet=0;
   uint8_t BuildULNRInforet=0; 
   
#ifdef NR_TDD
   mode->present = NR_Mode_Info_PR_tDD;
#else
   mode->present = NR_Mode_Info_PR_fDD;
#endif   
   
   if(mode->present == NR_Mode_Info_PR_fDD)
   {
      DU_ALLOC(mode->choice.fDD,sizeof(FDD_Info_t));
      if(mode->choice.fDD == NULLP)
      {
         DU_LOG("\nERROR  --> Memory allocation failed in BuildNrMode");
         return RFAILED;
      }
      BuildULNRInforet = BuildULNRInfo(&mode->choice.fDD->uL_NRFreqInfo);
      if(BuildULNRInforet != ROK)
      {
         DU_LOG("\nERROR  --> Failed to build UlNrFreqInfo");
         return RFAILED;    
      }
      BuildDLNRInforet = BuildDLNRInfo(&mode->choice.fDD->dL_NRFreqInfo);
      if(BuildDLNRInforet != ROK)
      {
         DU_LOG("\nERROR  --> Failed to build DlNrFreqInfo");
         return RFAILED;
      }
      mode->choice.fDD->uL_Transmission_Bandwidth.nRSCS = \
                                                          duCfgParam.srvdCellLst[0].duCellInfo.\
                                                          f1Mode.mode.fdd.ulTxBw.nrScs;
      mode->choice.fDD->uL_Transmission_Bandwidth.nRNRB = \
                                                          duCfgParam.srvdCellLst[0].duCellInfo.\
                                                          f1Mode.mode.fdd.ulTxBw.nrb;
      mode->choice.fDD->dL_Transmission_Bandwidth.nRSCS = \
                                                          duCfgParam.srvdCellLst[0].duCellInfo.\
                                                          f1Mode.mode.fdd.dlTxBw.nrScs;
      mode->choice.fDD->dL_Transmission_Bandwidth.nRNRB = \
                                                          duCfgParam.srvdCellLst[0].duCellInfo.\
                                                          f1Mode.mode.fdd.dlTxBw.nrb;
   }
   else if(mode->present == NR_Mode_Info_PR_tDD) 
   {
      DU_ALLOC(mode->choice.tDD,sizeof(TDD_Info_t));
      if(mode->choice.tDD == NULLP)
      {
         DU_LOG("\nERROR  --> Memory allocation failed in BuildNrMode");
         return RFAILED;
      }

      if(fillNrTddInfo(mode->choice.tDD) != ROK)
      {
         DU_LOG("\nERROR  --> Failed to fill Nr TDD information");
         return RFAILED;
      }

   }

   return ROK;
}
/*******************************************************************
 *
 * @brief Builds IE Extensions for Served PLMNs 
 *
 * @details
 *
 *    Function : BuildExtensions
 *
 *    Functionality: Building the IE Extensions
 *
 * @params[in] struct ProtocolExtensionContainer_4624P3 *buildextend
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildExtensions(ProtocolExtensionContainer_4624P3_t **ieExtend)
{
   uint8_t idx=0, plmnidx=0, sliceLstIdx=0;
   uint8_t elementCnt=0, extensionCnt=0;

   extensionCnt=IE_EXTENSION_LIST_COUNT;
   DU_ALLOC(*ieExtend,sizeof(ProtocolExtensionContainer_4624P3_t));
   if((*ieExtend) == NULLP)
   {
      DU_LOG("ERROR  --> DU_APP : BuildExtensions(): Memory allocation failed");
      return RFAILED;
   }
   (*ieExtend)->list.count = extensionCnt;
   (*ieExtend)->list.size = \
                            extensionCnt * sizeof(ServedPLMNs_ItemExtIEs_t *);
   DU_ALLOC((*ieExtend)->list.array,(*ieExtend)->list.size);
   if((*ieExtend)->list.array == NULLP)
   {
      DU_LOG("ERROR  --> DU_APP : BuildExtensions(): Memory allocation failed");
      return RFAILED;
   }
   for(plmnidx=0;plmnidx<extensionCnt;plmnidx++)
   {
      DU_ALLOC((*ieExtend)->list.array[plmnidx],\
            sizeof(ServedPLMNs_ItemExtIEs_t));
      if((*ieExtend)->list.array[plmnidx] == NULLP)
      {
         DU_LOG("ERROR  --> DU_APP : BuildExtensions(): Memory allocation failed");
         return RFAILED;
      }
   }
   
   elementCnt = duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.srvdPlmn[0].taiSliceSuppLst.numSupportedSlices;
   idx = 0;
   (*ieExtend)->list.array[idx]->id = ProtocolIE_ID_id_TAISliceSupportList;
   (*ieExtend)->list.array[idx]->criticality = Criticality_ignore;
   (*ieExtend)->list.array[idx]->extensionValue.present = \
   ServedPLMNs_ItemExtIEs__extensionValue_PR_SliceSupportList;
   (*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
      list.count = elementCnt;
   (*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
      list.size = (*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
      list.count * sizeof(SliceSupportItem_t *);

   DU_ALLOC((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
         list.array, elementCnt * sizeof(SliceSupportItem_t *));
   if((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
         list.array == NULLP)
   {
      DU_LOG("ERROR  --> DU_APP : BuildExtensions(): Memory allocation failed");
      return RFAILED;
   }

   for(sliceLstIdx =0; sliceLstIdx<elementCnt; sliceLstIdx++)
   {
      DU_ALLOC((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
            list.array[sliceLstIdx],sizeof(SliceSupportItem_t));
      if((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
            list.array[sliceLstIdx] == NULLP) 
      {
         DU_LOG("ERROR  --> DU_APP : BuildExtensions(): Memory allocation failed");
         return RFAILED;
      }
      (*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
         list.array[sliceLstIdx]->sNSSAI.sST.size = sizeof(uint8_t);
      DU_ALLOC((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList\
            .list.array[sliceLstIdx]->sNSSAI.sST.buf,(*ieExtend)->list.array[idx]->\
            extensionValue.choice.SliceSupportList.\
            list.array[sliceLstIdx]->sNSSAI.sST.size);
      if((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList\
            .list.array[sliceLstIdx]->sNSSAI.sST.buf == NULLP)
      {
         DU_LOG("ERROR  --> DU_APP : BuildExtensions(): Memory allocation failed");
         return RFAILED;
      }
      (*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
         list.array[sliceLstIdx]->sNSSAI.sST.buf[0] = duCfgParam.srvdCellLst[0].duCellInfo.\
         cellInfo.srvdPlmn[0].taiSliceSuppLst.snssai[sliceLstIdx]->sst;
      
      DU_ALLOC((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
            list.array[sliceLstIdx]->sNSSAI.sD,sizeof(OCTET_STRING_t));
      if((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
            list.array[sliceLstIdx]->sNSSAI.sD == NULLP)
      {
         DU_LOG("ERROR  --> DU_APP : BuildExtensions(): Memory allocation failed");
         return RFAILED;
      }
      (*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
         list.array[sliceLstIdx]->sNSSAI.sD->size = 3 * sizeof(uint8_t);
      DU_ALLOC((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
            list.array[sliceLstIdx]->sNSSAI.sD->buf, (*ieExtend)->list.array[idx]->extensionValue.choice.\
            SliceSupportList.list.array[sliceLstIdx]->sNSSAI.sD->size);
      if((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
            list.array[sliceLstIdx]->sNSSAI.sD->buf == NULLP)
      {
         DU_LOG("ERROR  --> DU_APP : BuildExtensions(): Memory allocation failed");
         return RFAILED;
      }
      memcpy((*ieExtend)->list.array[idx]->extensionValue.choice.SliceSupportList.\
      list.array[sliceLstIdx]->sNSSAI.sD->buf, duCfgParam.srvdCellLst[0].duCellInfo.\
      cellInfo.srvdPlmn[0].taiSliceSuppLst.snssai[sliceLstIdx]->sd, (*ieExtend)->list.array[idx]->\
      extensionValue.choice.SliceSupportList.list.array[sliceLstIdx]->sNSSAI.sD->size);
   }
   return ROK;
}
/*******************************************************************
 *
 * @brief Builds Served PLMN 
 *
 * @details
 *
 *    Function : BuildServedPlmn
 *
 *    Functionality: Building the Served PLMN
 *
 * @params[in] GNB_DU_Served_Cells_Item_t *srvCellItem
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t  BuildServedPlmn(ServedPLMNs_List_t *srvplmn)
{  
   uint8_t  plmnidx;
   uint8_t  servPlmnCnt=1;
   uint8_t buildPlmnIdret=0;
   uint8_t BuildExtensionsret=0;
   srvplmn->list.count = servPlmnCnt;
   srvplmn->list.size = \
			servPlmnCnt*sizeof(ServedPLMNs_Item_t *);
   DU_ALLOC(srvplmn->list.array,srvplmn->list.size);
   if(srvplmn->list.array == NULLP)
   {
      return RFAILED;
   }
   for(plmnidx=0; plmnidx<servPlmnCnt; plmnidx++)
   {   
      DU_ALLOC(srvplmn->list.array[plmnidx],\
	    sizeof(ServedPLMNs_Item_t));
      if(srvplmn->list.array[plmnidx] == NULLP)
      {
	 return RFAILED;
      }  
   }
   srvplmn->list.array[0]->pLMN_Identity.size = PLMN_SIZE * sizeof(uint8_t);
   DU_ALLOC(srvplmn->list.array[0]->pLMN_Identity.buf, srvplmn->list.array[0]->pLMN_Identity.size);
   buildPlmnIdret = buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn,\
	 srvplmn->list.array[0]->pLMN_Identity.buf);
   if(buildPlmnIdret!= ROK)
   {
      return RFAILED;
   }
   BuildExtensionsret = BuildExtensions(&srvplmn->list.array[0]->iE_Extensions);
   if(BuildExtensionsret!= ROK)
   {
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Frees Measurement Timing configuration 
 *
 * @details
 *
 *    Function : FreeMeasTimingConf
 *
 *    Functionality: Frees Timing Configuration
 *
 * @params[in] MeasurementTimingConfigurationRrc_t measTimingConfRrc
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void FreeMeasTimingConf(MeasurementTimingConfigurationRrc_t   measTimingConfRrc)
{
   uint8_t measIeIdx = 0;
   MeasurementTimingConfigurationRrc_IEs_t  *measTimingConfIEs = NULLP;

   if(measTimingConfRrc.criticalExtensions.choice.c1)
   {
      if(measTimingConfRrc.criticalExtensions.choice.c1->choice.measTimingConf)
      {
         measTimingConfIEs = measTimingConfRrc.criticalExtensions.choice.c1->choice.measTimingConf;
         if(measTimingConfIEs->measTiming)
         {
            if(measTimingConfIEs->measTiming->list.array)
            {
               for(measIeIdx = 0; measIeIdx < measTimingConfIEs->measTiming->list.count; measIeIdx++)
               {
                  if(measTimingConfIEs->measTiming->list.array[measIeIdx])
                  {
                     DU_FREE(measTimingConfIEs->measTiming->list.array[measIeIdx]->frequencyAndTiming, sizeof(struct MeasTiming__frequencyAndTiming));
                     DU_FREE(measTimingConfIEs->measTiming->list.array[measIeIdx], sizeof(MeasTiming_t));
                  }
               }
               DU_FREE(measTimingConfIEs->measTiming->list.array, measTimingConfIEs->measTiming->list.size);
            }
            DU_FREE(measTimingConfIEs->measTiming, sizeof(MeasTimingList_t));
         }
         DU_FREE(measTimingConfRrc.criticalExtensions.choice.c1->choice.measTimingConf, sizeof(MeasurementTimingConfigurationRrc_IEs_t));
      }
      DU_FREE(measTimingConfRrc.criticalExtensions.choice.c1, sizeof(struct MeasurementTimingConfigurationRrc__criticalExtensions__c1));
   }
}

/*******************************************************************
 *
 * @brief Builds MEasuerment Timing Configuration
 *
 * @details
 *
 *    Function : BuildMeasTimingConf
 *
 *    Functionality: Building Measurement Timing Configuration
 *
 * @params[in] Pointer to octet string to store Measurement timing
 *             Configuration
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildMeasTimingConf(OCTET_STRING_t *measTimingConf)
{
   uint8_t ret = RFAILED;
   uint8_t elementCnt = 0;
   uint8_t measIeIdx = 0;
   asn_enc_rval_t encRetVal;
   MeasurementTimingConfigurationRrc_t   measTimingConfRrc;
   MeasurementTimingConfigurationRrc_IEs_t  *measTimingConfIEs = NULLP;
   struct MeasTiming__frequencyAndTiming  *freqAndTiming = NULLP;

   while(true)
   {
      measTimingConfRrc.criticalExtensions.present = MeasurementTimingConfigurationRrc__criticalExtensions_PR_c1;
      DU_ALLOC(measTimingConfRrc.criticalExtensions.choice.c1, sizeof(struct MeasurementTimingConfigurationRrc__criticalExtensions__c1));
      if(!measTimingConfRrc.criticalExtensions.choice.c1)
      {
         DU_LOG("ERROR  --> DU_APP : BuildMeasTimingConf(): Memory allocation failure for critical extension choice C1");
         break;
      }
      measTimingConfRrc.criticalExtensions.choice.c1->present = MeasurementTimingConfigurationRrc__criticalExtensions__c1_PR_measTimingConf;

      DU_ALLOC(measTimingConfRrc.criticalExtensions.choice.c1->choice.measTimingConf, sizeof(MeasurementTimingConfigurationRrc_IEs_t));
      if(!measTimingConfRrc.criticalExtensions.choice.c1->choice.measTimingConf)
      {
         DU_LOG("ERROR  --> DU_APP : BuildMeasTimingConf(): Memory allocation failure for measTimingConf");
         break;
      }
      measTimingConfIEs = measTimingConfRrc.criticalExtensions.choice.c1->choice.measTimingConf;

      DU_ALLOC(measTimingConfIEs->measTiming, sizeof(MeasTimingList_t));
      if(!measTimingConfIEs->measTiming)
      {
         DU_LOG("ERROR  --> DU_APP : BuildMeasTimingConf(): Memory allocation failure for MeasTimingList");
         break;
      }

      elementCnt = 1;  
      measTimingConfIEs->measTiming->list.count = elementCnt;
      measTimingConfIEs->measTiming->list.size = elementCnt * sizeof(MeasTiming_t *);
      DU_ALLOC(measTimingConfIEs->measTiming->list.array, measTimingConfIEs->measTiming->list.size);
      if(!measTimingConfIEs->measTiming->list.array)
      {
         DU_LOG("ERROR  --> DU_APP : BuildMeasTimingConf(): Memory allocation failure for MeasTimingList array");
         break;
      }

      for(measIeIdx = 0; measIeIdx < elementCnt; measIeIdx++)
      {
         DU_ALLOC(measTimingConfIEs->measTiming->list.array[measIeIdx], sizeof(MeasTiming_t));
         if(!measTimingConfIEs->measTiming->list.array[measIeIdx])
         {
            DU_LOG("ERROR  --> DU_APP : BuildMeasTimingConf(): Memory allocation failure for MeasTimingList array index %d", measIeIdx);
            break;
         }
      }
      if(measIeIdx < elementCnt)
      {
         break;
      }

      measIeIdx = 0;
      DU_ALLOC(measTimingConfIEs->measTiming->list.array[measIeIdx]->frequencyAndTiming, sizeof(struct MeasTiming__frequencyAndTiming));
      if(!measTimingConfIEs->measTiming->list.array[measIeIdx]->frequencyAndTiming)
      {
         DU_LOG("ERROR  --> DU_APP : BuildMeasTimingConf(): Memory allocation failure for frequencyAndTiming");
         break;
      }
      freqAndTiming = measTimingConfIEs->measTiming->list.array[measIeIdx]->frequencyAndTiming;
      freqAndTiming->carrierFreq = MEAS_TIMING_ARFCN;
      freqAndTiming->ssbSubcarrierSpacing = duCfgParam.macCellCfg.ssbCfg.scsCmn;
      freqAndTiming->ssb_MeasurementTimingConfiguration.periodicityAndOffset.present = duCfgParam.macCellCfg.ssbCfg.ssbPeriod + 1;
      freqAndTiming->ssb_MeasurementTimingConfiguration.periodicityAndOffset.choice.sf20 = duCfgParam.macCellCfg.ssbCfg.ssbScOffset;
      freqAndTiming->ssb_MeasurementTimingConfiguration.duration = duCfgParam.srvdCellLst[0].duCellInfo.measTimeCfgDuration;

      /* Encode the F1SetupRequest type as APER */
      xer_fprint(stdout, &asn_DEF_MeasurementTimingConfigurationRrc, &measTimingConfRrc);

      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = uper_encode(&asn_DEF_MeasurementTimingConfigurationRrc, 0, &measTimingConfRrc, PrepFinalEncBuf, encBuf);

      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {     
         DU_LOG("\nERROR  -->  F1AP : Could not encode Measurement Timing Configuration structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }     
      else  
      {     
         DU_LOG("\nDEBUG   -->  F1AP : Created APER encoded buffer for Measurement Timing Configuration \n");
#ifdef DEBUG_ASN_PRINT
         for(measIeIdx=0; measIeIdx< encBufSize; measIeIdx++)
         {
            printf("%x",encBuf[measIeIdx]);
         }
#endif

         measTimingConf->size = encBufSize;
         DU_ALLOC(measTimingConf->buf, encBufSize);
         if(measTimingConf->buf == NULLP)
         {
            DU_LOG("\nERROR  -->  F1AP : Memory allocation failed for MeasurementTimingConfiguration buffer");
            return RFAILED;
         }
         memcpy(measTimingConf->buf, &encBuf, encBufSize);

         FreeMeasTimingConf(measTimingConfRrc);

         ret = ROK;
         break;
      }
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Builds Served Cell List
 *
 * @details
 *
 *    Function : BuildServedCellList
 *
 *    Functionality: Building Served Cell List
 *
 * @params[in] PLMNID plmn
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t BuildServedCellList(GNB_DU_Served_Cells_List_t *duServedCell)
{
   uint8_t  BuildNrcgiret=0;
   uint8_t  BuildFiveGSTacret=0;
   uint8_t  BuildServedPlmnret=0;
   uint8_t  BuildNrModeret=0;
   uint8_t  idx;
   uint8_t  plmnidx;
   uint8_t  plmnCnt=1;
   GNB_DU_Served_Cells_Item_t *srvCellItem;
   duServedCell->list.size = plmnCnt * sizeof(GNB_DU_Served_Cells_ItemIEs_t *);
   duServedCell->list.count = plmnCnt;

   DU_ALLOC(duServedCell->list.array, duServedCell->list.size);
   if(duServedCell->list.array == NULLP)
   {
      return RFAILED;
   }
   for(plmnidx=0; plmnidx<plmnCnt; plmnidx++)
   {
      DU_ALLOC(duServedCell->list.array[plmnidx],\
            sizeof(GNB_DU_Served_Cells_ItemIEs_t));
      if(duServedCell->list.array[plmnidx] == NULLP)
      {
         return RFAILED;
      }
   }
   idx = 0;
   duServedCell->list.array[idx]->id = ProtocolIE_ID_id_GNB_DU_Served_Cells_Item;
   duServedCell->list.array[idx]->criticality = Criticality_reject;
   duServedCell->list.array[idx]->value.present = \
                                                  GNB_DU_Served_Cells_ItemIEs__value_PR_GNB_DU_Served_Cells_Item;
   srvCellItem = \
                 &duServedCell->list.array[idx]->value.choice.GNB_DU_Served_Cells_Item;
   /*nRCGI*/
   BuildNrcgiret = BuildNrcgi(&srvCellItem->served_Cell_Information.nRCGI);
   if(BuildNrcgiret != ROK)
   {
      return RFAILED;
   }
   /*nRPCI*/
   srvCellItem->served_Cell_Information.nRPCI = \
                                                duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrPci;

   /* fiveGS_TAC */
   BuildFiveGSTacret = BuildFiveGSTac(&srvCellItem->served_Cell_Information);
   if(BuildFiveGSTacret != ROK)
   {
      return RFAILED;
   }

   /* Served PLMNs */
   BuildServedPlmnret = BuildServedPlmn(&srvCellItem->served_Cell_Information.servedPLMNs);
   if(BuildServedPlmnret !=ROK)
   {
      return RFAILED;
   }

   /* nR Mode Info with FDD/TDD */
   BuildNrModeret = BuildNrMode(&srvCellItem->served_Cell_Information.nR_Mode_Info);
   if(BuildNrModeret != ROK)
   {
      return RFAILED;
   }

   /*Measurement timing Config*/
   if(BuildMeasTimingConf(&srvCellItem->served_Cell_Information.measurementTimingConfiguration) != ROK)
      return RFAILED;

   /* GNB DU System Information */
   DU_ALLOC(srvCellItem->gNB_DU_System_Information,
         sizeof(GNB_DU_System_Information_t));
   if(!srvCellItem->gNB_DU_System_Information)
   {
      return RFAILED;
   }
   /* MIB */
   srvCellItem->gNB_DU_System_Information->mIB_message.size = duCfgParam.srvdCellLst[0].duSysInfo.mibLen;
   DU_ALLOC(srvCellItem->gNB_DU_System_Information->mIB_message.buf,
         srvCellItem->gNB_DU_System_Information->mIB_message.size);
   if(!srvCellItem->gNB_DU_System_Information->mIB_message.buf)
   {
      return RFAILED;
   }
   memcpy(srvCellItem->gNB_DU_System_Information->mIB_message.buf, duCfgParam.srvdCellLst[0].duSysInfo.mibMsg, \
         srvCellItem->gNB_DU_System_Information->mIB_message.size);

   /* SIB1 */
   srvCellItem->gNB_DU_System_Information->sIB1_message.size =\
                                                              duCfgParam.srvdCellLst[0].duSysInfo.sib1Len;

   DU_ALLOC(srvCellItem->gNB_DU_System_Information->sIB1_message.buf,
         srvCellItem->gNB_DU_System_Information->sIB1_message.size);
   if(!srvCellItem->gNB_DU_System_Information->sIB1_message.buf)
   {
      return RFAILED;
   }
   for(int x=0; x<srvCellItem->gNB_DU_System_Information->sIB1_message.size; x++)
   {
      srvCellItem->gNB_DU_System_Information->sIB1_message.buf[x]=\
                                                                  duCfgParam.srvdCellLst[0].duSysInfo.sib1Msg[x];
   }
   return ROK; 
}                                                                                                                  
/*******************************************************************
 *
 * @brief Builds RRC Version 
 *
 * @details
 *
 *    Function : BuildRrcVer
 *
 *    Functionality: Building RRC Version
 *
 * @params[in] int idx,int elementCnt,RRC_Version_t *rrcver
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildRrcVer(RRC_Version_t *rrcVer)
{
   uint8_t rrcExt;
   uint8_t rrcLatest;
   rrcVer->latest_RRC_Version.size = sizeof(uint8_t);
   DU_ALLOC(rrcVer->latest_RRC_Version.buf,sizeof(uint8_t));
   if(rrcVer->latest_RRC_Version.buf == NULLP)
   {
      return RFAILED;
   }
   rrcVer->latest_RRC_Version.buf[0] = 0;
   rrcVer->latest_RRC_Version.bits_unused = 5;
   DU_ALLOC(rrcVer->iE_Extensions,sizeof(ProtocolExtensionContainer_4624P81_t));
   if(rrcVer->iE_Extensions == NULLP)
   {  
      return RFAILED;
   }
   rrcVer->iE_Extensions->list.count = 1;
   rrcVer->iE_Extensions->list.size = sizeof(RRC_Version_ExtIEs_t *);
   DU_ALLOC(rrcVer->iE_Extensions->list.array,rrcVer->iE_Extensions->list.size);
   if(rrcVer->iE_Extensions->list.array == NULLP)
   {
      return RFAILED;
   }
   rrcExt = 0;
   DU_ALLOC(rrcVer->iE_Extensions->list.array[0],\
	 sizeof(RRC_Version_ExtIEs_t));
   if(rrcVer->iE_Extensions->list.array[0] == NULLP)
   {
      return RFAILED;
   }
   rrcVer->iE_Extensions->list.array[rrcExt]->id = \
						   ProtocolIE_ID_id_latest_RRC_Version_Enhanced;
   rrcVer->iE_Extensions->list.array[rrcExt]->criticality = Criticality_reject;
   rrcVer->iE_Extensions->list.array[rrcExt]->extensionValue.present =\
								      RRC_Version_ExtIEs__extensionValue_PR_Latest_RRC_Version_Enhanced;
   rrcVer->iE_Extensions->list.array[rrcExt]->extensionValue.choice\
      .Latest_RRC_Version_Enhanced.size = 3*sizeof(uint8_t);
   DU_ALLOC(rrcVer->iE_Extensions->list.array[rrcExt]->extensionValue.choice\
	 .Latest_RRC_Version_Enhanced.buf,rrcVer->iE_Extensions->list.\
	 array[rrcExt]->extensionValue.choice.Latest_RRC_Version_Enhanced.size);
   if(rrcVer->iE_Extensions->list.array[rrcExt]->extensionValue.choice\
	 .Latest_RRC_Version_Enhanced.buf == NULLP)
   {
      return RFAILED;
   }
   rrcLatest = 0;
   rrcVer->iE_Extensions->list.array[rrcExt]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.buf[rrcLatest] = 15;
   rrcLatest++;
   rrcVer->iE_Extensions->list.array[rrcExt]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.buf[rrcLatest] = 5;
   rrcLatest++;
   rrcVer->iE_Extensions->list.array[rrcExt]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.buf[rrcLatest] = 0;
   return ROK;
}
/*******************************************************************
 *
 * @brief Sends F1 msg over SCTP
 *
 * @details
 *
 *    Function : sendF1APMsg
 *
 *    Functionality: Sends F1 msg over SCTP
 *
 * @params[in] Region region
 *             Pool pool
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sendF1APMsg()
{
   Buffer *mBuf = NULLP;
  
   if(ODU_GET_MSG_BUF(DU_APP_MEM_REGION, DU_POOL, &mBuf) == ROK)
   {
      if(ODU_ADD_POST_MSG_MULT((Data *)encBuf, encBufSize, mBuf) == ROK)
      {
	    ODU_PRINT_MSG(mBuf, 0,0);

	    if(sctpSend(mBuf, F1_INTERFACE) != ROK)
	    {
	       DU_LOG("\nERROR  -->  F1AP : SCTP Send failed");
	       ODU_PUT_MSG_BUF(mBuf);
	       return RFAILED;
	    }
      }
      else
      {
	 DU_LOG("\nERROR  -->  F1AP : ODU_ADD_POST_MSG_MULT failed");
	 ODU_PUT_MSG_BUF(mBuf);
	 return RFAILED;
      }
      ODU_PUT_MSG_BUF(mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  F1AP : Failed to allocate memory");
      return RFAILED;
   }
   return ROK; 
} /* sendF1APMsg */

/*******************************************************************
 *
 * @brief  deallocating the memory of function BuildAndSendF1SetupReq()
 *
 * @details
 *
 *    Function :  FreeRrcVer
 *
 *    Functionality: deallocating the memory of function BuildRrcVer
 *
 * @params[in] RRC_Version_t *rrcVer
 * 
 * @return void
 *
 *****************************************************************/
void FreeRrcVer(RRC_Version_t *rrcVer)
{
   if(rrcVer->latest_RRC_Version.buf != NULLP)
   {
      if(rrcVer->iE_Extensions != NULLP)
      {
	 if(rrcVer->iE_Extensions->list.array != NULLP)
	 {
	    if(rrcVer->iE_Extensions->list.array[0] != NULLP)
	    {
	       if(rrcVer->iE_Extensions->list.array[0]->extensionValue.choice.Latest_RRC_Version_Enhanced.buf
		     != NULLP)
	       {
		  DU_FREE(rrcVer->iE_Extensions->list.array[0]->extensionValue.choice\
			.Latest_RRC_Version_Enhanced.buf,rrcVer->iE_Extensions->list.\
			array[0]->extensionValue.choice.Latest_RRC_Version_Enhanced.size);
	       }
	       DU_FREE(rrcVer->iE_Extensions->list.array[0],sizeof(RRC_Version_ExtIEs_t));
	    }
	    DU_FREE(rrcVer->iE_Extensions->list.array,sizeof(RRC_Version_ExtIEs_t*));
	 }
	 DU_FREE(rrcVer->iE_Extensions,sizeof(ProtocolExtensionContainer_4624P81_t));
      }
      DU_FREE(rrcVer->latest_RRC_Version.buf,rrcVer->latest_RRC_Version.size);
   }
}

/*******************************************************************
 *
 * @brief Deallocating memory of TDD NrFreqInfo 
 *
 * @details
 *
 *    Function : freeTddNrFreqInfo 
 *
 *    Functionality: freeTddNrFreqInfo 
 *
 * @params[in]  F1AP_PDU_t *f1apDuCfg
 *
 * @return ROK     - void
 *
 * ****************************************************************/
void freeTddNrFreqInfo(NRFreqInfo_t *freqInfo)
{
   uint8_t freqBandListIdx = 0, supportedBandIdx = 0;

   if(freqInfo->freqBandListNr.list.array)
   {
      for(freqBandListIdx = 0; freqBandListIdx<freqInfo->freqBandListNr.list.count; freqBandListIdx++)
      {
         if(freqInfo->freqBandListNr.list.array[freqBandListIdx])
         {
            if(freqInfo->freqBandListNr.list.array[freqBandListIdx]->supportedSULBandList.list.array)
            {
               for(supportedBandIdx = 0; supportedBandIdx<freqInfo->freqBandListNr.list.array[freqBandListIdx]->supportedSULBandList.list.count; supportedBandIdx++)
               {
                  DU_FREE(freqInfo->freqBandListNr.list.array[freqBandListIdx]->supportedSULBandList.list.array[supportedBandIdx],\
                        sizeof(SupportedSULFreqBandItem_t));
               }
               DU_FREE(freqInfo->freqBandListNr.list.array[freqBandListIdx]->supportedSULBandList.list.array,\
                     freqInfo->freqBandListNr.list.array[freqBandListIdx]->supportedSULBandList.list.size);

            }
            DU_FREE(freqInfo->freqBandListNr.list.array[freqBandListIdx],  sizeof(FreqBandNrItem_t ));
         }
      }
      DU_FREE(freqInfo->freqBandListNr.list.array, freqInfo->freqBandListNr.list.size );
   }
}

/*******************************************************************
 *
 * @brief Deallocating memory allocated for Nr fdd frequencey mode 
 *
 * @details
 *
 *    Function : freeFddNrFreqInfo 
 *
 *    Functionality:Free memory allocated for Nr fdd frequencey mode 
 *
 * @params[in]  
 *
 * @return ROK     - void
 *
 * ****************************************************************/
void freeFddNrFreqInfo(FDD_Info_t *fDD)
{
   uint8_t arrIdx =0;

   if(fDD != NULLP)
   {
      if(fDD->uL_NRFreqInfo.freqBandListNr.list.array != NULLP)
      {
         DU_FREE(fDD->uL_NRFreqInfo.freqBandListNr.list.\
               array[arrIdx], sizeof(FreqBandNrItem_t));
         DU_FREE(fDD->uL_NRFreqInfo.freqBandListNr.list.array, \
               fDD->uL_NRFreqInfo.freqBandListNr.list.size);
      }

      if(fDD->dL_NRFreqInfo.freqBandListNr.list.array != NULLP)
      {
         DU_FREE(fDD->dL_NRFreqInfo.freqBandListNr.list.\
               array[arrIdx], sizeof(FreqBandNrItem_t));
         DU_FREE(fDD->dL_NRFreqInfo.freqBandListNr.list.array,\
               fDD->dL_NRFreqInfo.freqBandListNr.list.size);
      }
      DU_FREE(fDD,sizeof(FDD_Info_t));
   }
}

/*******************************************************************
 *
 * @brief  deallocating the memory of function BuildAndSendF1SetupReq()
 *
 * @details
 *
 *    Function :  FreeServedCellList
 *
 *    Functionality:  deallocating the memory of function BuildServedCellList

 *
 * @params[in] GNB_DU_Served_Cells_List_t *duServedCell
 *
 * @return void
 *
 * ****************************************************************/
void FreeServedCellList( GNB_DU_Served_Cells_List_t *duServedCell)
{
   uint8_t   plmnCnt= 1;
   uint8_t  extensionCnt=IE_EXTENSION_LIST_COUNT;
   uint8_t  plmnIdx=0, sliceIdx=0;
   GNB_DU_Served_Cells_Item_t *srvCellItem;
   ServedPLMNs_Item_t  *servedPlmnItem;
   SliceSupportItem_t  *sliceSupportItem;

   if(duServedCell->list.array!=NULLP)
   {
      if(duServedCell->list.array[0]!=NULLP)
      {
         srvCellItem= &duServedCell->list.array[0]->value.choice.GNB_DU_Served_Cells_Item;

         DU_FREE(srvCellItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
               srvCellItem->served_Cell_Information.nRCGI.pLMN_Identity.size * sizeof(uint8_t));
         DU_FREE(srvCellItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
               srvCellItem->served_Cell_Information.nRCGI.nRCellIdentity.size * sizeof(uint8_t));

         if(srvCellItem->served_Cell_Information.fiveGS_TAC!=NULLP)
         {
            DU_FREE(srvCellItem->served_Cell_Information.fiveGS_TAC->buf,\
                  sizeof(srvCellItem->served_Cell_Information.fiveGS_TAC->size));
            DU_FREE(srvCellItem->served_Cell_Information.fiveGS_TAC,sizeof(FiveGS_TAC_t));
         }

         if(srvCellItem->served_Cell_Information.servedPLMNs.list.array!=NULLP)
         {
            if(srvCellItem->served_Cell_Information.servedPLMNs.list.array[plmnIdx] != NULLP)
            {
               servedPlmnItem = srvCellItem->served_Cell_Information.servedPLMNs.list.array[plmnIdx];
               DU_FREE(servedPlmnItem->pLMN_Identity.buf, servedPlmnItem->pLMN_Identity.size);

               if(servedPlmnItem->iE_Extensions != NULLP)
               {
                  if(servedPlmnItem->iE_Extensions->list.array != NULLP)
                  {
                     if(servedPlmnItem->iE_Extensions->list.array[0] != NULLP)
                     {
                        if(servedPlmnItem->iE_Extensions->list.array[0]->extensionValue.choice.\
                              SliceSupportList.list.array != NULLP)
                        {
                           for(sliceIdx =0; sliceIdx<servedPlmnItem->iE_Extensions->list.array[0]->\
                                 extensionValue.choice.SliceSupportList.list.count; sliceIdx++)
                           {
                              if(servedPlmnItem->iE_Extensions->list.array[0]->extensionValue.choice.\
                                    SliceSupportList.list.array[sliceIdx] != NULLP)
                              {
                                 sliceSupportItem = servedPlmnItem->iE_Extensions->list.array[0]->\
                                                    extensionValue.choice.SliceSupportList.list.array[sliceIdx];

                                 DU_FREE(sliceSupportItem->sNSSAI.sST.buf, sizeof(uint8_t));

                                 if(sliceSupportItem->sNSSAI.sD != NULLP)
                                 {
                                    DU_FREE(sliceSupportItem->sNSSAI.sD->buf,\
                                          sliceSupportItem->sNSSAI.sD->size);
                                    DU_FREE(sliceSupportItem->sNSSAI.sD, sizeof(OCTET_STRING_t));
                                 }

                                 DU_FREE(servedPlmnItem->iE_Extensions->list.array[0]->extensionValue.\
                                       choice.SliceSupportList.list.array[sliceIdx], sizeof(SliceSupportItem_t));
                              }
                           }
                           DU_FREE(servedPlmnItem->iE_Extensions->list.array[0]->extensionValue.choice.\
                                 SliceSupportList.list.array, servedPlmnItem->iE_Extensions->list.array[0]->\
                                 extensionValue.choice.SliceSupportList.list.size);
                        }
                        DU_FREE(servedPlmnItem->iE_Extensions->list.array[0],\
                              sizeof(ServedPLMNs_ItemExtIEs_t));
                     }
                     DU_FREE(servedPlmnItem->iE_Extensions->list.array,\
                           extensionCnt*sizeof(ServedPLMNs_ItemExtIEs_t*));
                  }
                  DU_FREE(servedPlmnItem->iE_Extensions, sizeof(ProtocolExtensionContainer_4624P3_t));
               }
               DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array[plmnIdx],\
                     sizeof(ServedPLMNs_Item_t));
            }
            DU_FREE(srvCellItem->served_Cell_Information.servedPLMNs.list.array,\
                  sizeof(ServedPLMNs_Item_t *));
         }

         if(srvCellItem->served_Cell_Information.nR_Mode_Info.present == NR_Mode_Info_PR_fDD)
         {
            freeFddNrFreqInfo(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.fDD);
         }
         else   
         {
            if(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.tDD != NULLP)
            {
               freeTddNrFreqInfo(&srvCellItem->served_Cell_Information.nR_Mode_Info.choice.tDD->nRFreqInfo);
               DU_FREE(srvCellItem->served_Cell_Information.nR_Mode_Info.choice.tDD, sizeof(TDD_Info_t));
            }
         }
         
         DU_FREE(srvCellItem->served_Cell_Information.measurementTimingConfiguration.buf,\
               srvCellItem->served_Cell_Information.measurementTimingConfiguration.size);

         if(srvCellItem->gNB_DU_System_Information != NULLP)
         {
            if(srvCellItem->gNB_DU_System_Information->mIB_message.buf != NULLP)
            {
               DU_FREE(srvCellItem->gNB_DU_System_Information->mIB_message.buf,\
                     srvCellItem->gNB_DU_System_Information->mIB_message.size);
            }

            if(srvCellItem->gNB_DU_System_Information->sIB1_message.buf != NULLP)
            { 
               DU_FREE(srvCellItem->gNB_DU_System_Information->sIB1_message.buf,\
                     srvCellItem->gNB_DU_System_Information->sIB1_message.size);
            }

            DU_FREE(srvCellItem->gNB_DU_System_Information, sizeof(GNB_DU_System_Information_t));
         }

         DU_FREE(duServedCell->list.array[0], sizeof(GNB_DU_Served_Cells_ItemIEs_t));
      }
      DU_FREE(duServedCell->list.array, plmnCnt * sizeof(GNB_DU_Served_Cells_ItemIEs_t*));
   }
}

/*******************************************************************
 *
 * @brief  deallocating the memory of function BuildAndSendF1SetupReq()
 *
 * @details
 *
 *    Function :  FreeF1SetupReq
 *
 *    Functionality:  deallocating the memory of function BuildAndSendF1SetupReq
 *
 * @params[in] F1AP_PDU_t *f1apMsg
 *
 * @return void
 *
 * ****************************************************************/
void FreeF1SetupReq(F1AP_PDU_t *f1apMsg)
{
   uint8_t ieIdx, ieIdx2;
   F1SetupRequest_t *f1SetupReq=NULLP;

   if(f1apMsg != NULLP)
   {
      if(f1apMsg->choice.initiatingMessage != NULLP)
      {
         f1SetupReq = &f1apMsg->choice.initiatingMessage->value.choice.F1SetupRequest;
         if(f1SetupReq->protocolIEs.list.array != NULLP)
         {
            for(ieIdx = 0; ieIdx < f1SetupReq->protocolIEs.list.count; ieIdx++)
            {
               if(f1SetupReq->protocolIEs.list.array[ieIdx] != NULLP)
               {
                  switch(f1SetupReq->protocolIEs.list.array[ieIdx]->id)
                  {
                     case ProtocolIE_ID_id_TransactionID:
                        break;
                     case ProtocolIE_ID_id_gNB_DU_ID:
                        DU_FREE(f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.buf,\
                              f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.size);
                        break;
                     case ProtocolIE_ID_id_gNB_DU_Name:
                        DU_FREE(f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_Name.buf,\
                              strlen((char *)duCfgParam.duName));
                        break;
                     case ProtocolIE_ID_id_gNB_DU_Served_Cells_List:
                        FreeServedCellList(&f1SetupReq->protocolIEs.list.\
                              array[ieIdx]->value.choice.GNB_DU_Served_Cells_List);
                        break;
                     case ProtocolIE_ID_id_GNB_DU_RRC_Version:
                        FreeRrcVer(&f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.RRC_Version);
                        break;
                     default:
                        DU_LOG("\nERROR  -->  Invalid Event Type %ld", f1SetupReq->protocolIEs.list.array[ieIdx]->id);
                        break;
                  }
               }
            }
            for(ieIdx2=0; ieIdx2< ieIdx; ieIdx2++)
            {
               DU_FREE(f1SetupReq->protocolIEs.list.array[ieIdx2],sizeof(F1SetupRequestIEs_t));
            }
            DU_FREE(f1SetupReq->protocolIEs.list.array,\
                  f1SetupReq->protocolIEs.list.size);
         }
         DU_FREE(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      }
      DU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Builds and Send the F1SetupRequest
 *
 * @details
 *
 *    Function : BuildAndSendF1SetupReq
 *
 * Functionality:Fills the F1SetupRequest
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t BuildAndSendF1SetupReq()
{
   uint8_t   ret, ieIdx, elementCnt;
   F1AP_PDU_t                 *f1apMsg = NULLP;
   F1SetupRequest_t           *f1SetupReq=NULLP;
   GNB_DU_Served_Cells_List_t *duServedCell=NULLP;
   RRC_Version_t              *rrcVer=NULLP;
   asn_enc_rval_t             encRetVal;        /* Encoder return value */
   ret= RFAILED;

   DU_LOG("\nINFO   -->  F1AP : Building F1 Setup Request\n");
   do
   {
      DU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
         break;
      }
      f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
      DU_ALLOC(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      if(f1apMsg->choice.initiatingMessage == NULLP)
      {
         break;
      }
      f1apMsg->choice.initiatingMessage->procedureCode = ProcedureCode_id_F1Setup;
      f1apMsg->choice.initiatingMessage->criticality = Criticality_reject;
      f1apMsg->choice.initiatingMessage->value.present = \
                                                         InitiatingMessage__value_PR_F1SetupRequest;

      f1SetupReq = &f1apMsg->choice.initiatingMessage->value.choice.F1SetupRequest;

      elementCnt = 5;

      f1SetupReq->protocolIEs.list.count = elementCnt;
      f1SetupReq->protocolIEs.list.size = elementCnt * sizeof(F1SetupRequestIEs_t *);

      /* Initialize the F1Setup members */
      DU_ALLOC(f1SetupReq->protocolIEs.list.array,f1SetupReq->protocolIEs.list.size);
      if(f1SetupReq->protocolIEs.list.array == NULLP)
      {
         break;
      }
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
         DU_ALLOC(f1SetupReq->protocolIEs.list.array[ieIdx],\
               sizeof(F1SetupRequestIEs_t));
         if(f1SetupReq->protocolIEs.list.array[ieIdx] == NULLP)
         {
            break;
         }
      }

      ieIdx = 0;
      /*TransactionID*/
      f1SetupReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_TransactionID;
      f1SetupReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      f1SetupReq->protocolIEs.list.array[ieIdx]->value.present =\
                                                                F1SetupRequestIEs__value_PR_TransactionID;
      f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.TransactionID = \
                                                                              TRANS_ID;

      /*DU ID*/
      ieIdx++;
      f1SetupReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_gNB_DU_ID;
      f1SetupReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      f1SetupReq->protocolIEs.list.array[ieIdx]->value.present = \
                                                                 F1SetupRequestIEs__value_PR_GNB_DU_ID;
      f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.size =\
                                                                              sizeof(uint8_t);

      DU_ALLOC(f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.buf,\
            f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.size);
      if(f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.buf == \
            NULLP)
      {
         break;
      }

      f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.buf[0] =\
                                                                                duCfgParam.duId;
      /* DU name IE is of type printableString_t which wireshark is unable to decode.
       * However this string is decoded successfully on online decoders.
       * Since this is an optional IE and the value received in it are not
       * used as of now, eliminating this IE for now to avoid wireshark error.
       */
      /*DU Name*/
      if(duCfgParam.duName != NULL)
      {
         ieIdx++;
         f1SetupReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_gNB_DU_Name;
         f1SetupReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_ignore;
         f1SetupReq->protocolIEs.list.array[ieIdx]->value.present = F1SetupRequestIEs__value_PR_GNB_DU_Name;
         f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_Name.size = strlen((char *)duCfgParam.duName);
         DU_ALLOC(f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_Name.buf, \
               f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_Name.size);
         if(f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_Name.buf == NULLP)
         {
            break;
         }
         strcpy((char*)f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_Name.buf,
               (char*)duCfgParam.duName);
      }

      /*Served Cell list */
      ieIdx++;
      f1SetupReq->protocolIEs.list.array[ieIdx]->id = \
                                                      ProtocolIE_ID_id_gNB_DU_Served_Cells_List;
      f1SetupReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      f1SetupReq->protocolIEs.list.array[ieIdx]->value.present = \
                                                                 F1SetupRequestIEs__value_PR_GNB_DU_Served_Cells_List;
      duServedCell = &f1SetupReq->protocolIEs.list.\
                     array[ieIdx]->value.choice.GNB_DU_Served_Cells_List;
      if(BuildServedCellList(duServedCell))
      {
         break;
      }

      /*RRC Version*/
      ieIdx++;
      f1SetupReq->protocolIEs.list.array[ieIdx]->id = \
                                                      ProtocolIE_ID_id_GNB_DU_RRC_Version ;
      f1SetupReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      f1SetupReq->protocolIEs.list.array[ieIdx]->value.present = \
                                                                 F1SetupRequestIEs__value_PR_RRC_Version;
      rrcVer = &f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.RRC_Version;
      if(BuildRrcVer(rrcVer))
      {
         break;
      }

      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the F1SetupRequest type as APER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
            encBuf);

      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  F1AP : Could not encode F1SetupRequest structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG   -->  F1AP : Created APER encoded buffer for F1SetupRequest\n");
#ifdef DEBUG_ASN_PRINT
         for(ieIdx=0; ieIdx< encBufSize; ieIdx++)
         {
            printf("%x",encBuf[ieIdx]);
         }
#endif
         

      }
      
      /* Sending msg */
      if(sendF1APMsg() != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Sending F1 Setup request failed");
         break;
      }
      
      if(fillE2NodeComponentReqInfo(F1, duCfgParam.duId,  E2_NODE_COMPONENT_ADD, encBufSize, encBuf) !=ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Failed to add the e2 node in the list");
         break;
      }

      ret=ROK;
      break;
   }while(true);

   FreeF1SetupReq(f1apMsg);

   return ret;
}/* End of BuildAndSendF1SetupReq */

/*******************************************************************
 *
 * @brief Deallocating memory allocated for Served_Cells_To_Modify_Item_t
 *
 * @details
 *
 *    Function : freeCellsToModifyItem 
 *
 *    Functionality: Deallocating memory of variables allocated in
 *                    BuildAndSendDUConfigUpdate function
 *
 * @params[in]  Served_Cells_To_Modify_Item_t *modifyItem
 *
 * @return ROK     - void
 *
 * ****************************************************************/

void freeCellsToModifyItem(Served_Cells_To_Modify_Item_t *modifyItem)
{
   uint8_t arrIdx=0, servedPlmnIdx=0, sliceLstIdx=0;
   ServedPLMNs_Item_t *servedPlmnItem = NULLP;
   SliceSupportItem_t *sliceSupportItem = NULLP;

   DU_FREE(modifyItem->oldNRCGI.pLMN_Identity.buf, modifyItem->oldNRCGI.pLMN_Identity.size);
   DU_FREE(modifyItem->oldNRCGI.nRCellIdentity.buf, modifyItem->oldNRCGI.nRCellIdentity.size);

   DU_FREE(modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.buf,\
           modifyItem->served_Cell_Information.nRCGI.pLMN_Identity.size);
   DU_FREE(modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.buf,\
         modifyItem->served_Cell_Information.nRCGI.nRCellIdentity.size);

   if(modifyItem->served_Cell_Information.servedPLMNs.list.array != NULLP)
   {
      if(modifyItem->served_Cell_Information.servedPLMNs.list.array[arrIdx] != NULLP)
      {
         servedPlmnItem = modifyItem->served_Cell_Information.servedPLMNs.list.array[arrIdx];

         DU_FREE(servedPlmnItem->pLMN_Identity.buf,servedPlmnItem->pLMN_Identity.size);

         if(servedPlmnItem->iE_Extensions != NULLP)
         {
            if(servedPlmnItem->iE_Extensions->list.array != NULLP)
            {
               if(servedPlmnItem->iE_Extensions->list.array[arrIdx] != NULLP)
               {
                  if(servedPlmnItem->iE_Extensions->list.array[arrIdx]->extensionValue.choice.SliceSupportList.\
                        list.array != NULLP)
                  {
                     for(sliceLstIdx =0; sliceLstIdx<servedPlmnItem->iE_Extensions->list.array[arrIdx]->\
                           extensionValue.choice.SliceSupportList.list.count; sliceLstIdx++)
                     {
                        if(servedPlmnItem->iE_Extensions->list.array[arrIdx]->extensionValue.choice.SliceSupportList.\
                              list.array[sliceLstIdx] != NULLP)
                        {

                           sliceSupportItem = servedPlmnItem->iE_Extensions->list.array[arrIdx]->extensionValue.choice.\
                                              SliceSupportList.list.array[sliceLstIdx];

                           DU_FREE(sliceSupportItem->sNSSAI.sST.buf, sliceSupportItem->sNSSAI.sST.size);
                           if(sliceSupportItem->sNSSAI.sD != NULLP)
                           {
                              DU_FREE(sliceSupportItem->sNSSAI.sD->buf, sliceSupportItem->sNSSAI.sD->size);
                              DU_FREE(sliceSupportItem->sNSSAI.sD,sizeof(OCTET_STRING_t));
                           }
                           DU_FREE(servedPlmnItem->iE_Extensions->list.array[arrIdx]->extensionValue.choice.\
                                 SliceSupportList.list.array[sliceLstIdx], sizeof(SliceSupportItem_t));
                        }
                     }
                     DU_FREE(servedPlmnItem->iE_Extensions->list.array[arrIdx]->extensionValue.\
                           choice.SliceSupportList.list.array,\
                           servedPlmnItem->iE_Extensions->list.array[arrIdx]->\
                           extensionValue.choice.SliceSupportList.list.size);
                  }
               }
               for(servedPlmnIdx=0; servedPlmnIdx< servedPlmnItem->iE_Extensions->list.count ; servedPlmnIdx++)
               {
                  DU_FREE(servedPlmnItem->iE_Extensions->list.array[servedPlmnIdx], sizeof(ServedPLMNs_ItemExtIEs_t ));
               }
               DU_FREE(servedPlmnItem->iE_Extensions->list.array, servedPlmnItem->iE_Extensions->list.size);
            }
            DU_FREE(servedPlmnItem->iE_Extensions,sizeof(ProtocolExtensionContainer_4624P3_t));
         }
      }
      for(servedPlmnIdx=0; servedPlmnIdx<modifyItem->served_Cell_Information.servedPLMNs.list.count; servedPlmnIdx++)
      {
         DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array[servedPlmnIdx], sizeof(ServedPLMNs_Item_t));
      }
      DU_FREE(modifyItem->served_Cell_Information.servedPLMNs.list.array,\
         modifyItem->served_Cell_Information.servedPLMNs.list.size);
   }
   
   if(modifyItem->served_Cell_Information.nR_Mode_Info.present == NR_Mode_Info_PR_fDD)
   {
      freeFddNrFreqInfo(modifyItem->served_Cell_Information.nR_Mode_Info.choice.fDD);
   }  
   else
   {
      if(modifyItem->served_Cell_Information.nR_Mode_Info.choice.tDD)
      {
         freeTddNrFreqInfo(&modifyItem->served_Cell_Information.nR_Mode_Info.choice.tDD->nRFreqInfo);
         DU_FREE(modifyItem->served_Cell_Information.nR_Mode_Info.choice.tDD, sizeof(TDD_Info_t));
      }
   }
   DU_FREE(modifyItem->served_Cell_Information.measurementTimingConfiguration.buf,\
         modifyItem->served_Cell_Information.measurementTimingConfiguration.size);
}

/*******************************************************************
 *
 * @brief Deallocating memory of BuildAndSendDUConfigUpdate
 *
 * @details
 *
 *    Function : FreeDUConfigUpdate
 *
 *    Functionality: Deallocating memory of variables allocated in
 *                    BuildAndSendDUConfigUpdate function
 *
 * @params[in]  F1AP_PDU_t *f1apDuCfg
 *
 * @return ROK     - void
 *
 * ****************************************************************/
void FreeDUConfigUpdate(F1AP_PDU_t *f1apDuCfg)
{
   uint8_t  idx=0,ieIdx=0, cellModifyIdx=0, cellDeleteIdx=0;
   GNBDUConfigurationUpdate_t *duCfgUpdate = NULLP;
   Served_Cells_To_Modify_List_t  *cellsToModify=NULLP;
   Served_Cells_To_Delete_List_t  *cellsToDelete=NULLP;
   Served_Cells_To_Delete_Item_t  *deleteItem = NULLP;
   Served_Cells_To_Delete_ItemIEs_t *deleteItemIe = NULLP;
   Cells_Status_ItemIEs_t *cellStatusItemIE;

   if(f1apDuCfg != NULLP)
   {
      if(f1apDuCfg->choice.initiatingMessage != NULLP)
      {
         duCfgUpdate = &f1apDuCfg->choice.initiatingMessage->\
                       value.choice.GNBDUConfigurationUpdate;
         if(duCfgUpdate->protocolIEs.list.array != NULLP)
         {
            for(ieIdx=0; ieIdx<duCfgUpdate->protocolIEs.list.count; ieIdx++)
            {
               if(duCfgUpdate->protocolIEs.list.array[ieIdx] != NULLP)
               {
                  switch(duCfgUpdate->protocolIEs.list.array[ieIdx]->id)
                  {
                     case ProtocolIE_ID_id_Served_Cells_To_Modify_List:
                        {
                           cellsToModify = &duCfgUpdate->protocolIEs.list.array[ieIdx]->\
                                           value.choice.Served_Cells_To_Modify_List;
                           if(cellsToModify->list.array != NULLP)
                           {
                              for(cellModifyIdx=0; cellModifyIdx<cellsToModify->list.count ;cellModifyIdx++)
                              {
                                 if(cellsToModify->list.array[cellModifyIdx] != NULLP)
                                 {
                                    freeCellsToModifyItem(&cellsToModify->list.array[cellModifyIdx]->value.choice.\
                                          Served_Cells_To_Modify_Item);
                                    DU_FREE(cellsToModify->list.array[cellModifyIdx],\
                                          sizeof(Served_Cells_To_Modify_ItemIEs_t));
                                 }
                              }
                              DU_FREE(cellsToModify->list.array,cellsToModify->list.size);
                           }
                           break;
                        }
                     case ProtocolIE_ID_id_Served_Cells_To_Delete_List:
                        {
                           cellsToDelete = &duCfgUpdate->protocolIEs.list.array[ieIdx]->\
                                           value.choice.Served_Cells_To_Delete_List;
                           if(cellsToDelete->list.array != NULLP)
                           {
                              for(cellDeleteIdx=0; cellDeleteIdx<cellsToDelete->list.count ;cellDeleteIdx++)
                              {
                                 if(cellsToDelete->list.array[cellDeleteIdx] != NULLP)
                                 {
                                    deleteItemIe = ((Served_Cells_To_Delete_ItemIEs_t*)\
                                          cellsToDelete->list.array[cellDeleteIdx]);
                                    deleteItem=&deleteItemIe->value.choice.Served_Cells_To_Delete_Item;
                                    DU_FREE(deleteItem->oldNRCGI.pLMN_Identity.buf,\
                                          deleteItem->oldNRCGI.pLMN_Identity.size); 
                                    DU_FREE(deleteItem->oldNRCGI.nRCellIdentity.buf,\
                                          deleteItem->oldNRCGI.nRCellIdentity.size);
                                    DU_FREE(cellsToDelete->list.array[cellDeleteIdx],\
                                          sizeof(Served_Cells_To_Delete_ItemIEs_t));
                                 }
                              }
                              DU_FREE(cellsToDelete->list.array,cellsToDelete->list.size);
                           }

                           break;
                        }
                     case ProtocolIE_ID_id_gNB_DU_ID:
                        {
                           DU_FREE(duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.buf,\
                                 duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.size);
                           break;
                        }
                     case ProtocolIE_ID_id_Cells_Status_List:
                        {
                           for(idx = 0; idx < duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.Cells_Status_List.list.count; idx++)
                           {
                              cellStatusItemIE = (Cells_Status_ItemIEs_t *)duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.Cells_Status_List.list.array[idx];
                              DU_FREE(cellStatusItemIE->value.choice.Cells_Status_Item.nRCGI.nRCellIdentity.buf, cellStatusItemIE->value.choice.Cells_Status_Item.nRCGI.nRCellIdentity.size);
                              DU_FREE(cellStatusItemIE->value.choice.Cells_Status_Item.nRCGI.pLMN_Identity.buf, cellStatusItemIE->value.choice.Cells_Status_Item.nRCGI.pLMN_Identity.size);
                              DU_FREE(duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.Cells_Status_List.list.array[idx],sizeof(Cells_Status_ItemIEs_t));
                           }
                           DU_FREE(duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.Cells_Status_List.list.array,\
                                 duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.Cells_Status_List.list.size);
                        }
                  }
                  DU_FREE(duCfgUpdate->protocolIEs.list.array[ieIdx],\
                        sizeof(GNBDUConfigurationUpdateIEs_t));
               }
            }
            DU_FREE(duCfgUpdate->protocolIEs.list.array,duCfgUpdate->protocolIEs.list.size);
         }
         DU_FREE(f1apDuCfg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
      }
      DU_FREE(f1apDuCfg, (Size)sizeof(F1AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Fills Served Plmns required in ServCellInfo IE
 *
 * @details
 *
 *    Function : fillServedPlmns
 *
 *    Functionality: Fills Served Plmns required in ServCellInfo IE
 *
 * @params[in] Pointer to ServedPLMNs_List_t *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 *****************************************************************/

uint8_t fillServedPlmns(ServedPLMNs_List_t *servedPlmn)
{
   uint8_t ieIdx=0, arrayIdx=0, ieListCnt=0, elementCnt=0, sliceLstIdx=0;

   servedPlmn->list.array[arrayIdx]->pLMN_Identity.size = 3*sizeof(uint8_t);
   DU_ALLOC(servedPlmn->list.array[arrayIdx]->pLMN_Identity.buf, servedPlmn->list.\
         array[arrayIdx]->pLMN_Identity.size);
   if(servedPlmn->list.array[arrayIdx]->pLMN_Identity.buf == NULLP)
   {
      DU_LOG("ERROR  --> DU_APP : fillServedPlmns(): Memory allocation failed");
      return RFAILED;
   }
   buildPlmnId(duCfgParam.srvdCellLst[arrayIdx].duCellInfo.cellInfo.srvdPlmn[arrayIdx].plmn,\
         servedPlmn->list.array[arrayIdx]->pLMN_Identity.buf);
   DU_ALLOC(servedPlmn->list.array[arrayIdx]->iE_Extensions,sizeof(ProtocolExtensionContainer_4624P3_t));
   if(servedPlmn->list.array[arrayIdx]->iE_Extensions == NULLP)
   {
      DU_LOG("ERROR  --> DU_APP : fillServedPlmns(): Memory allocation failed");
      return RFAILED;
   }

   ieListCnt=1;
   servedPlmn->list.array[arrayIdx]->iE_Extensions->list.count = ieListCnt;
   servedPlmn->list.array[arrayIdx]->iE_Extensions->list.size = ieListCnt *sizeof(ServedPLMNs_ItemExtIEs_t *);
   DU_ALLOC(servedPlmn->list.array[arrayIdx]->iE_Extensions->list.array,servedPlmn->list.array[arrayIdx]->\
         iE_Extensions->list.size);
   if(servedPlmn->list.array[arrayIdx]->iE_Extensions->list.array == NULLP)
   {
      DU_LOG("ERROR  --> DU_APP : fillServedPlmns(): Memory allocation failed");
      return RFAILED;
   }
   for(ieIdx=arrayIdx;ieIdx<ieListCnt;ieIdx++)
   {
      DU_ALLOC(servedPlmn->list.array[arrayIdx]->iE_Extensions->list.array[ieIdx],\
            sizeof(ServedPLMNs_ItemExtIEs_t));
      if(servedPlmn->list.array[arrayIdx]->iE_Extensions->list.array[ieIdx] == NULLP)
      {
         DU_LOG("ERROR  --> DU_APP : fillServedPlmns(): Memory allocation failed");
         return RFAILED;
      }
   }
   
   ieIdx = 0;
   elementCnt = duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.srvdPlmn[0].taiSliceSuppLst.numSupportedSlices; 
   servedPlmn->list.array[arrayIdx]->iE_Extensions->list.array[ieIdx]->id =ProtocolIE_ID_id_TAISliceSupportList;
   servedPlmn->list.array[arrayIdx]->iE_Extensions->list.array[ieIdx]->criticality = Criticality_ignore;
   servedPlmn->list.array[arrayIdx]->iE_Extensions->list.array[ieIdx]->extensionValue.present = \
   ServedPLMNs_ItemExtIEs__extensionValue_PR_SliceSupportList;
   servedPlmn->list.array[arrayIdx]->iE_Extensions->list.array[ieIdx]->extensionValue.choice.SliceSupportList.\
      list.count = elementCnt;
   servedPlmn->list.array[arrayIdx]->\
      iE_Extensions->list.array[ieIdx]->extensionValue.choice.SliceSupportList.\
      list.size = elementCnt * sizeof(SliceSupportItem_t *);
   DU_ALLOC(servedPlmn->list.array[arrayIdx]->\
         iE_Extensions->list.array[ieIdx]->extensionValue.choice.SliceSupportList.\
         list.array,servedPlmn->list.array[arrayIdx]->\
         iE_Extensions->list.array[ieIdx]->extensionValue.choice.SliceSupportList.list.size);
   if(servedPlmn->list.array[arrayIdx]->\
         iE_Extensions->list.array[ieIdx]->extensionValue.choice.SliceSupportList.\
         list.array == NULLP)
   {
      DU_LOG("ERROR  --> DU_APP : fillServedPlmns(): Memory allocation failed");
      return RFAILED;
   }

   for(sliceLstIdx =0; sliceLstIdx< elementCnt; sliceLstIdx++)
   {
      DU_ALLOC(servedPlmn->list.array[arrayIdx]->\
      iE_Extensions->list.array[ieIdx]->extensionValue.choice.SliceSupportList.\
      list.array[sliceLstIdx],sizeof( SliceSupportItem_t));
      if(servedPlmn->list.array[arrayIdx]->\
      iE_Extensions->list.array[ieIdx]->extensionValue.choice.SliceSupportList.\
      list.array[sliceLstIdx] == NULLP)
      {   
         DU_LOG("ERROR  --> DU_APP : fillServedPlmns(): Memory allocation failed");
         return RFAILED;
      }
      
      servedPlmn->list.array[arrayIdx]->\
      iE_Extensions->list.array[ieIdx]->extensionValue.choice.SliceSupportList.\
      list.array[sliceLstIdx]->sNSSAI.sST.size = sizeof(uint8_t);
      DU_ALLOC(servedPlmn->list.array[arrayIdx]->\
      iE_Extensions->list.array[ieIdx]->extensionValue.choice.SliceSupportList.\
      list.array[sliceLstIdx]->sNSSAI.sST.buf,servedPlmn->list.array[arrayIdx]->\
      iE_Extensions->list.array[ieIdx]->extensionValue.choice.SliceSupportList.list.array[sliceLstIdx]->\
      sNSSAI.sST.size);
      
      if(servedPlmn->list.array[arrayIdx]->\
      iE_Extensions->list.array[ieIdx]->extensionValue.choice.SliceSupportList.\
      list.array[sliceLstIdx]->sNSSAI.sST.buf == NULLP)
      {
         DU_LOG("ERROR  --> DU_APP : fillServedPlmns(): Memory allocation failed");
         return RFAILED;
      }
      servedPlmn->list.array[arrayIdx]->\
      iE_Extensions->list.array[ieIdx]->extensionValue.choice.SliceSupportList.\
      list.array[sliceLstIdx]->sNSSAI.sST.buf[arrayIdx] =  duCfgParam.srvdCellLst[arrayIdx].duCellInfo.\
      cellInfo.srvdPlmn[arrayIdx].taiSliceSuppLst.snssai[sliceLstIdx]->sst;

      DU_ALLOC(servedPlmn->list.array[arrayIdx]->\
      iE_Extensions->list.array[ieIdx]->extensionValue.choice.SliceSupportList.\
      list.array[sliceLstIdx]->sNSSAI.sD,sizeof(OCTET_STRING_t));
      if(servedPlmn->list.array[arrayIdx]->\
      iE_Extensions->list.array[ieIdx]->extensionValue.choice.SliceSupportList.\
      list.array[sliceLstIdx]->sNSSAI.sD == NULLP)
      {
         DU_LOG("ERROR  --> DU_APP : fillServedPlmns(): Memory allocation failed");
         return RFAILED;
      }
      servedPlmn->list.array[arrayIdx]->\
      iE_Extensions->list.array[ieIdx]->extensionValue.choice.SliceSupportList.\
      list.array[sliceLstIdx]->sNSSAI.sD->size = 3 * sizeof(uint8_t);
      DU_ALLOC(servedPlmn->list.array[arrayIdx]->\
      iE_Extensions->list.array[ieIdx]->extensionValue.choice.SliceSupportList.\
      list.array[sliceLstIdx]->sNSSAI.sD->buf,servedPlmn->list.array[arrayIdx]->\
      iE_Extensions->list.array[ieIdx]->extensionValue.choice.SliceSupportList.\
      list.array[sliceLstIdx]->sNSSAI.sD->size);
      if(servedPlmn->list.array[arrayIdx]->\
      iE_Extensions->list.array[ieIdx]->extensionValue.choice.SliceSupportList.\
      list.array[sliceLstIdx]->sNSSAI.sD->buf == NULLP)
      {
         DU_LOG("ERROR  --> DU_APP : fillServedPlmns(): Memory allocation failed");
         return RFAILED;
      }
      memcpy(servedPlmn->list.array[arrayIdx]->\
      iE_Extensions->list.array[ieIdx]->extensionValue.choice.SliceSupportList.\
      list.array[sliceLstIdx]->sNSSAI.sD->buf, duCfgParam.srvdCellLst[arrayIdx].duCellInfo.\
      cellInfo.srvdPlmn[arrayIdx].taiSliceSuppLst.snssai[sliceLstIdx]->sd,\
      servedPlmn->list.array[arrayIdx]->iE_Extensions->list.array[ieIdx]->extensionValue.choice.SliceSupportList.\
      list.array[sliceLstIdx]->sNSSAI.sD->size);
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills Nr Fdd Info required in ServCellInfo IE
 *
 * @details
 *
 *    Function : fillNrFddInfo
 *
 *    Functionality: Fills Nr Fdd Info required in ServCellInfo IE
 *
 * @params[in] FDD_Info_t *fDD
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 *****************************************************************/

uint8_t fillNrFddInfo(FDD_Info_t *fDD)
{
   fDD->uL_NRFreqInfo.nRARFCN = duCfgParam.srvdCellLst[0].duCellInfo.\
      f1Mode.mode.fdd.ulNrFreqInfo.nrArfcn;
   fDD->uL_NRFreqInfo.freqBandListNr.list.count = 1;
   fDD->uL_NRFreqInfo.freqBandListNr.list.size = sizeof(FreqBandNrItem_t*);
   DU_ALLOC(fDD->uL_NRFreqInfo.freqBandListNr.list.\
	 array, fDD->uL_NRFreqInfo.freqBandListNr.list.size);
   if(fDD->uL_NRFreqInfo.freqBandListNr.list.array == NULLP)
   {
      DU_LOG("\nERROR  --> Memory allocation failed in fillNrFddInfo");
      return RFAILED;
   }

   DU_ALLOC(fDD->uL_NRFreqInfo.freqBandListNr.list.array[0], \
      sizeof(FreqBandNrItem_t));
   if(fDD->uL_NRFreqInfo.freqBandListNr.list.array[0] == NULLP)
   {
      DU_LOG("\nERROR  --> Memory allocation failed in fillNrFddInfo");
      return RFAILED;
   }
   
   fDD->uL_NRFreqInfo.freqBandListNr.list.array[0]->freqBandIndicatorNr = \
      duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.fdd.ulNrFreqInfo.\
      freqBand[0].nrFreqBand;
   fDD->uL_NRFreqInfo.freqBandListNr.list.array[0]->supportedSULBandList.list.count=0;
   fDD->dL_NRFreqInfo.nRARFCN = duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.fdd.\
      dlNrFreqInfo.nrArfcn;
   fDD->dL_NRFreqInfo.freqBandListNr.list.count = 1;
   fDD->dL_NRFreqInfo.freqBandListNr.list.size = sizeof(FreqBandNrItem_t *);
   DU_ALLOC(fDD->dL_NRFreqInfo.freqBandListNr.list.array, fDD->dL_NRFreqInfo.freqBandListNr.list.size);
   if(fDD->dL_NRFreqInfo.freqBandListNr.list.array == NULLP)
   {
      DU_LOG("\nERROR  --> Memory allocation failed in fillNrFddInfo");
      return RFAILED;
   }
   
   DU_ALLOC(fDD->dL_NRFreqInfo.freqBandListNr.list.array[0],  sizeof(FreqBandNrItem_t));
   if(fDD->dL_NRFreqInfo.freqBandListNr.list.array[0] == NULLP)
   {
      DU_LOG("\nERROR  --> Memory allocation failed in fillNrFddInfo");
      return RFAILED;
   }

   fDD->dL_NRFreqInfo.freqBandListNr.list.array[0]->freqBandIndicatorNr = \
      duCfgParam.srvdCellLst[0].duCellInfo.f1Mode.mode.fdd.dlNrFreqInfo.\
      freqBand[0].nrFreqBand;
   fDD->dL_NRFreqInfo.freqBandListNr.list.array[0]->supportedSULBandList.list.count=0;
   
   /*Transmission Bandwidth*/
   fDD->uL_Transmission_Bandwidth.nRSCS = duCfgParam.srvdCellLst[0].duCellInfo.\
      f1Mode.mode.fdd.ulTxBw.nrScs;
   fDD->uL_Transmission_Bandwidth.nRNRB = duCfgParam.srvdCellLst[0].duCellInfo.\
      f1Mode.mode.fdd.ulTxBw.nrb;
   fDD->dL_Transmission_Bandwidth.nRSCS = duCfgParam.srvdCellLst[0].duCellInfo.\
      f1Mode.mode.fdd.dlTxBw.nrScs;
   fDD->dL_Transmission_Bandwidth.nRNRB = duCfgParam.srvdCellLst[0].duCellInfo.\
      f1Mode.mode.fdd.dlTxBw.nrb;

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills ServCellInfo IE
 *
 * @details
 *
 *    Function : fillServedCellInfo
 *
 *    Functionality: Fills ServCellInfo
 *
 * @params[in] Pointer to Served_Cell_Information_t *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 *****************************************************************/

uint8_t fillServedCellInfo(Served_Cell_Information_t *srvCellInfo)
{
   uint8_t ieIdx, ieListCnt;

   /*nRCGI*/
   srvCellInfo->nRCGI.pLMN_Identity.size =3*sizeof(uint8_t);
   DU_ALLOC(srvCellInfo->nRCGI.pLMN_Identity.buf,\
	 srvCellInfo->nRCGI.pLMN_Identity.size);
   if(srvCellInfo->nRCGI.pLMN_Identity.buf == NULLP)
   {
      DU_LOG("\nERROR  --> Memory allocation failed in fillServedCellInfo");
      return RFAILED;
   }
   buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn,\
	 srvCellInfo->nRCGI.pLMN_Identity.buf);
   srvCellInfo->nRCGI.nRCellIdentity.size =5*sizeof(uint8_t);
   DU_ALLOC(srvCellInfo->nRCGI.nRCellIdentity.buf,\
	 srvCellInfo->nRCGI.nRCellIdentity.size);
   if(srvCellInfo->nRCGI.nRCellIdentity.buf == NULLP)
   {   
      DU_LOG("\nERROR  --> Memory allocation failed in fillServedCellInfo");
      return RFAILED;
   }
   
   fillBitString(&srvCellInfo->nRCGI.nRCellIdentity, ODU_VALUE_FOUR, ODU_VALUE_FIVE, duCfgParam.sib1Params.cellIdentity);
   /*nRPCI*/
   srvCellInfo->nRPCI = duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrPci;

   /*servedPLMNs*/
   ieListCnt = 1;
   srvCellInfo->servedPLMNs.list.count = ieListCnt;
   srvCellInfo->servedPLMNs.list.size = ieListCnt*sizeof(ServedPLMNs_Item_t *);
   DU_ALLOC(srvCellInfo->servedPLMNs.list.array, srvCellInfo->servedPLMNs.list.size);
   if(srvCellInfo->servedPLMNs.list.array == NULLP)
   {
      DU_LOG("\nERROR  --> Memory allocation failed in fillServedCellInfo");
      return RFAILED;
   }
   for(ieIdx=0; ieIdx < ieListCnt; ieIdx++)
   {
      DU_ALLOC(srvCellInfo->servedPLMNs.list.array[ieIdx], sizeof(ServedPLMNs_Item_t));
      if(srvCellInfo->servedPLMNs.list.array[ieIdx]== NULLP)
      {
         DU_LOG("\nERROR  --> Memory allocation failed in fillServedCellInfo");
         return RFAILED;
      }
   }
   if(fillServedPlmns(&srvCellInfo->servedPLMNs))
   {
      DU_LOG("\nERROR  --> Failed to fill Served Plmn info");
      return RFAILED;
   }

#ifndef NR_TDD
   /*nR Mode Info with FDD*/
   srvCellInfo->nR_Mode_Info.present = NR_Mode_Info_PR_fDD;
   DU_ALLOC(srvCellInfo->nR_Mode_Info.choice.fDD, sizeof(FDD_Info_t));
   if(srvCellInfo->nR_Mode_Info.choice.fDD == NULLP)
   {
      DU_LOG("\nERROR  --> Memory allocation failed in fillServedCellInfo");
      return RFAILED;
   }
   if(fillNrFddInfo(srvCellInfo->nR_Mode_Info.choice.fDD))
   {
       DU_LOG("\nERROR  --> Failed to fill the Nr FDD information");
      return RFAILED;
   }
#else
   srvCellInfo->nR_Mode_Info.present = NR_Mode_Info_PR_tDD;   
   DU_ALLOC(srvCellInfo->nR_Mode_Info.choice.tDD, sizeof(TDD_Info_t));
   if(srvCellInfo->nR_Mode_Info.choice.tDD == NULLP)
   {
      DU_LOG("\nERROR  --> Memory allocation failed in fillServedCellInfo");
      return RFAILED;
   }
   if(fillNrTddInfo(srvCellInfo->nR_Mode_Info.choice.tDD) != ROK)
   {
      DU_LOG("\nERROR  --> Failed to fill the Nr TDD information");
      return RFAILED;
   }
#endif

   /*Measurement timing Config*/
   if(BuildMeasTimingConf(&srvCellInfo->measurementTimingConfiguration) != ROK)
      return RFAILED;

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills ServCellToModItem IE
 *
 * @details
 *
 *    Function : fillServCellToModItem
 *
 *    Functionality: Fills ServCellToModItem IE
 *
 * @params[in] Pointer to Served_Cells_To_Modify_Item_t *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 *****************************************************************/

uint8_t fillServCellToModItem(Served_Cells_To_Modify_Item_t *modifyItem)
{
   /*pLMN_Identity*/
   modifyItem->oldNRCGI.pLMN_Identity.size = 3*sizeof(uint8_t);
   DU_ALLOC(modifyItem->oldNRCGI.pLMN_Identity.buf,modifyItem->oldNRCGI.pLMN_Identity.size);
   if(modifyItem->oldNRCGI.pLMN_Identity.buf == NULLP)
   {
      return RFAILED;
   }
   buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn,\
	 modifyItem->oldNRCGI.pLMN_Identity.buf);

   /*nRCellIdentity*/
   modifyItem->oldNRCGI.nRCellIdentity.size = 5*sizeof(uint8_t);
   DU_ALLOC(modifyItem->oldNRCGI.nRCellIdentity.buf,\
	 modifyItem->oldNRCGI.nRCellIdentity.size);
   if(modifyItem->oldNRCGI.nRCellIdentity.buf == NULLP)
   {
      return RFAILED;
   }
   fillBitString(&modifyItem->oldNRCGI.nRCellIdentity, ODU_VALUE_FOUR, ODU_VALUE_FIVE, duCfgParam.sib1Params.cellIdentity);

   if(fillServedCellInfo(&modifyItem->served_Cell_Information))
      return RFAILED;
   else
      return ROK;
}

/*******************************************************************
 *
 * @brief Builds ServCellToModList
 *
 * @details
 *
 *    Function : buildServCellToModList
 *
 *    Functionality: Builds the serv cell to Mod List
 *
 * @params[in] Pointer to Served_Cells_To_Modify_List_t *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 *****************************************************************/

uint8_t buildServCellToModList(Served_Cells_To_Modify_List_t *cellsToModify)
{
   uint8_t ieListCnt, ieIdx;
   Served_Cells_To_Modify_Item_t *modifyItem = NULLP;

   ieListCnt = 1;
   cellsToModify->list.count = ieListCnt;
   cellsToModify->list.size = ieListCnt*sizeof(Served_Cells_To_Modify_ItemIEs_t *);
   DU_ALLOC(cellsToModify->list.array,cellsToModify->list.size);
   if(cellsToModify->list.array == NULLP)
   {
      return RFAILED;
   }
   for(ieIdx=0; ieIdx< ieListCnt; ieIdx++)
   {
      DU_ALLOC(cellsToModify->list.array[ieIdx],sizeof(Served_Cells_To_Modify_ItemIEs_t));
      if(cellsToModify->list.array[ieIdx] == NULLP)
      {
	 return RFAILED;
      }
   }
   cellsToModify->list.array[0]->id = ProtocolIE_ID_id_Served_Cells_To_Modify_Item;
   cellsToModify->list.array[0]->criticality = Criticality_reject;
   cellsToModify->list.array[0]->value.present =\
      Served_Cells_To_Modify_ItemIEs__value_PR_Served_Cells_To_Modify_Item;
   modifyItem=&cellsToModify->list.array[0]->value.choice.Served_Cells_To_Modify_Item;

   if(fillServCellToModItem(modifyItem))
      return RFAILED;
   else
      return ROK;
}
/*******************************************************************
 *
 * @brief filling the DeleteItemList
 *
 * @details
 *
 *    Function : fillCellToDeleteItem 
 *
 *    Functionality: Filling the DeleteItemIe 
 *
 * @params[in] Pointer to Served_Cells_To_Delete_ItemIEs_t 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 *****************************************************************/
uint8_t fillCellToDeleteItem(struct Served_Cells_To_Delete_ItemIEs *deleteItemIe)
{
   Served_Cells_To_Delete_Item_t *deleteItem=NULLP;
   
   deleteItemIe->id = ProtocolIE_ID_id_Served_Cells_To_Delete_Item;
   deleteItemIe->criticality = Criticality_reject;
   deleteItemIe->value.present =\
   Served_Cells_To_Delete_ItemIEs__value_PR_Served_Cells_To_Delete_Item;
   deleteItem=&deleteItemIe->value.choice.Served_Cells_To_Delete_Item;

   /*pLMN_Identity*/
   deleteItem->oldNRCGI.pLMN_Identity.size = 3*sizeof(uint8_t);
   DU_ALLOC(deleteItem->oldNRCGI.pLMN_Identity.buf,deleteItem->oldNRCGI.pLMN_Identity.size);
   if(deleteItem->oldNRCGI.pLMN_Identity.buf == NULLP)
   {
      DU_LOG("ERROR  --> F1AP: fillCellToDeleteItem(): Failed to allocate the memory");
      return RFAILED;
   }
   buildPlmnId(duCfgParam.srvdCellLst[0].duCellInfo.cellInfo.nrCgi.plmn,\
         deleteItem->oldNRCGI.pLMN_Identity.buf);

   /*nRCellIdentity*/
   deleteItem->oldNRCGI.nRCellIdentity.size = 5*sizeof(uint8_t);
   DU_ALLOC(deleteItem->oldNRCGI.nRCellIdentity.buf,\
         deleteItem->oldNRCGI.nRCellIdentity.size);
   if(deleteItem->oldNRCGI.nRCellIdentity.buf == NULLP)
   {
      DU_LOG("ERROR  --> F1AP: fillCellToDeleteItem(): Failed to allocate the memory");
      return RFAILED;
   }
   fillBitString(&deleteItem->oldNRCGI.nRCellIdentity, ODU_VALUE_FOUR, ODU_VALUE_FIVE, duCfgParam.sib1Params.cellIdentity);
   return ROK;
} 

/*******************************************************************
 *
 * @brief Builds ServCellToDeleteList
 *
 * @details
 *
 *    Function : buildServCellToDeleteList
 *
 *    Functionality: Builds the serv cell to delete List
 *
 * @params[in] Pointer to Served_Cells_To_Delete_List_t *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 *****************************************************************/
 
uint8_t buildServCellToDeleteList(Served_Cells_To_Delete_List_t *cellsToDelete)
{
   uint8_t ieListCnt, arrIdx;
   
   ieListCnt = 1;
   cellsToDelete->list.count = ieListCnt;
   cellsToDelete->list.size = ieListCnt*sizeof(Served_Cells_To_Delete_ItemIEs_t *);
   
   DU_ALLOC(cellsToDelete->list.array,cellsToDelete->list.size);
   if(cellsToDelete->list.array == NULLP)
   {
      DU_LOG("\nERROR  --> F1AP : buildServCellToDeleteList(): Memory allocation failed");
      return RFAILED;
   }
   
   for(arrIdx=0; arrIdx< ieListCnt; arrIdx++)
   {
      DU_ALLOC(cellsToDelete->list.array[arrIdx],sizeof(Served_Cells_To_Delete_ItemIEs_t));
      if(cellsToDelete->list.array[arrIdx] == NULLP)
      {
         DU_LOG("\nERROR  --> F1AP : buildServCellToDeleteList(): Memory allocation failed");
         return RFAILED;
      }
   }
   
   arrIdx=0;
   if(fillCellToDeleteItem((Served_Cells_To_Delete_ItemIEs_t*)cellsToDelete->list.array[arrIdx]) !=ROK)
   {
      DU_LOG("\nERROR  -->  F1AP: buildServCellToDeleteList(): failed to fill Served_Cells_To_Delete_ItemIEs");
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds CellsStatusList
 *
 * @details
 *
 *    Function : buildCellsStatusList
 *
 *    Functionality: Builds the Cell Status List
 *
 * @params[in] Pointer to Cells_Status_List_t *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 *****************************************************************/
uint8_t buildCellsStatusList(Cells_Status_List_t *cellStatusList)
{
   uint8_t elementCnt = 0, idx = 0, ret = ROK;
   Cells_Status_ItemIEs_t *cellStatusItemIE;

   elementCnt = 1;
   cellStatusList->list.count = elementCnt;
   cellStatusList->list.size = elementCnt * sizeof(Cells_Status_ItemIEs_t *);
   DU_ALLOC(cellStatusList->list.array, cellStatusList->list.size);

   for(idx = 0; idx < elementCnt; idx++)
   {
      DU_ALLOC(cellStatusList->list.array[idx], sizeof(Cells_Status_ItemIEs_t));
      if(!cellStatusList->list.array[idx])
      {
         DU_LOG("ERROR  --> F1AP: buildCellsStatusList() memory allocation failure");
         return RFAILED;
      }
   }
   idx = 0;
   cellStatusItemIE = (Cells_Status_ItemIEs_t *)cellStatusList->list.array[idx];
   cellStatusItemIE->id = ProtocolIE_ID_id_Cells_Status_Item;
   cellStatusItemIE->criticality = Criticality_reject;
   cellStatusItemIE->value.present = Cells_Status_ItemIEs__value_PR_Cells_Status_Item;
   ret = BuildNrcgi(&cellStatusItemIE->value.choice.Cells_Status_Item.nRCGI);
   if(ret == RFAILED)
   {
         DU_LOG("ERROR  --> F1AP: buildCellsStatusList() NRCGI failed");
         return RFAILED;
   }
   cellStatusItemIE->value.choice.Cells_Status_Item.service_status.service_state = Service_State_in_service;
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds and sends the DUConfigUpdate
 *
 * @details
 *
 *    Function : BuildAndSendDUConfigUpdate
 *
 *    Functionality: Constructs the DU Update message and sends
 *                   it to the CU through SCTP.
 *
 * @params[in] void **buf,Buffer to which encoded pattern is written into
 * @params[in] int *size,size of buffer
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendDUConfigUpdate(ServCellAction servCellAction)
{
   uint8_t ret =0, ieIdx=0, elementCnt=0;
   bool memAlloctionFailure = false;
   F1AP_PDU_t                 *f1apDuCfg = NULLP;
   GNBDUConfigurationUpdate_t *duCfgUpdate = NULLP;
   asn_enc_rval_t encRetVal;     /* Encoder return value */
   
   memset(&encRetVal, 0, sizeof(asn_enc_rval_t));
   ret= RFAILED;

   while(true)
   {
      DU_LOG("\nINFO   -->  F1AP : Building DU config update\n");
      /* Allocate the memory for F1DuCfg */
      DU_ALLOC(f1apDuCfg, sizeof(F1AP_PDU_t));
      if(f1apDuCfg == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildAndSendDUConfigUpdate(): Memory allocation for F1AP-PDU failed");
         break;
      }

      f1apDuCfg->present = F1AP_PDU_PR_initiatingMessage;
      DU_ALLOC(f1apDuCfg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
      if(f1apDuCfg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildAndSendDUConfigUpdate(): Memory allocation for F1AP-PDU failed");
         break;
      }

      f1apDuCfg->choice.initiatingMessage->procedureCode = \
                                                           ProcedureCode_id_gNBDUConfigurationUpdate;
      f1apDuCfg->choice.initiatingMessage->criticality = Criticality_reject;
      f1apDuCfg->choice.initiatingMessage->value.present = \
                                                           InitiatingMessage__value_PR_GNBDUConfigurationUpdate;
      duCfgUpdate = &f1apDuCfg->choice.initiatingMessage->value.\
                    choice.GNBDUConfigurationUpdate;
      elementCnt = 4;
      duCfgUpdate->protocolIEs.list.count = elementCnt;
      duCfgUpdate->protocolIEs.list.size = \
                                           elementCnt * sizeof(GNBDUConfigurationUpdateIEs_t*);

      /* Initialize the F1Setup members */
      DU_ALLOC(duCfgUpdate->protocolIEs.list.array,duCfgUpdate->protocolIEs.list.size);
      if(duCfgUpdate->protocolIEs.list.array == NULLP)
      {
         DU_LOG("ERROR  -->  F1AP : BuildAndSendDUConfigUpdate(): Memory allocation failed");
         break;
      }
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
         DU_ALLOC(duCfgUpdate->protocolIEs.list.array[ieIdx],sizeof(GNBDUConfigurationUpdateIEs_t));
         if(duCfgUpdate->protocolIEs.list.array[ieIdx] == NULLP)
         {
            DU_LOG("ERROR  -->  F1AP : BuildAndSendDUConfigUpdate(): Memory allocation failed");
            memAlloctionFailure = true;
            break;
         }
      }
      
      if(memAlloctionFailure == true)
      {
         break;
      }
      /*TransactionID*/
      ieIdx = 0;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->id=ProtocolIE_ID_id_TransactionID;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->criticality= Criticality_reject;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->value.present = \
      GNBDUConfigurationUpdateIEs__value_PR_TransactionID;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.TransactionID = TRANS_ID;
      
      ieIdx++;
      if(servCellAction == SERV_CELL_TO_MODIFY)
      {
         /*Served Cell to Modify */
         duCfgUpdate->protocolIEs.list.array[ieIdx]->id = \
         ProtocolIE_ID_id_Served_Cells_To_Modify_List;
         duCfgUpdate->protocolIEs.list.array[ieIdx]->criticality =Criticality_reject;
         duCfgUpdate->protocolIEs.list.array[ieIdx]->value.present = \
         GNBDUConfigurationUpdateIEs__value_PR_Served_Cells_To_Modify_List;
         if(buildServCellToModList(&duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.\
                  Served_Cells_To_Modify_List))
         {
            DU_LOG("ERROR  --> DU APP : BuildAndSendDUConfigUpdate(): failed to build ServCellToModList");
            break;
         }
      }
      else
      {
         /*Served Cell to Delete */ 
         duCfgUpdate->protocolIEs.list.array[ieIdx]->id = \
         ProtocolIE_ID_id_Served_Cells_To_Delete_List;
         duCfgUpdate->protocolIEs.list.array[ieIdx]->criticality =Criticality_reject;
         duCfgUpdate->protocolIEs.list.array[ieIdx]->value.present = \
         GNBDUConfigurationUpdateIEs__value_PR_Served_Cells_To_Delete_List;
         if(buildServCellToDeleteList(&duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.\
         Served_Cells_To_Delete_List)!=ROK)
         {
            DU_LOG("ERROR  --> DU APP : BuildAndSendDUConfigUpdate(): failed to build ServCellToDeleteList");
            break;
         }
         
      }
      // TODO :GNB DU SYS INFO:MIB AND SIB1 INFORMATION TO BE BUILT AND FILLED HERE
      
      /*Cell Status List*/
      ieIdx++;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_Cells_Status_List;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->value.present = \
           GNBDUConfigurationUpdateIEs__value_PR_Cells_Status_List;
      ret = buildCellsStatusList(&duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.Cells_Status_List);
      if(ret == RFAILED)
      {
         DU_LOG("ERROR  --> DU APP : BuildAndSendDUConfigUpdate(): Cell Status List building failed");
         break;
      }

      /*GNB DU ID */
      ieIdx++;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_gNB_DU_ID;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->value.present = \
      GNBDUConfigurationUpdateIEs__value_PR_GNB_DU_ID;
      duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.size = sizeof(uint8_t);
      DU_ALLOC(duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.buf,\
            duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.size);
      if(duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.buf == NULLP)
      {
         DU_LOG("ERROR  --> DU APP : BuildAndSendDUConfigUpdate(): Memory allocation failed for GNB_DU_ID");
         break;
      }
      duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.buf[0] = duCfgParam.duId;

      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apDuCfg);

      /* Encode the DU Config Update type as APER */
      memset((uint8_t *)encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apDuCfg, PrepFinalEncBuf, encBuf);

      /* Checking encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("ERROR  -->  F1AP : Could not encode DUConfigUpdate structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG   -->  F1AP : Created APER encoded buffer for DUConfigUpdate\n");
#ifdef DEBUG_ASN_PRINT
         for(ieIdx =0; ieIdx < encBufSize; ieIdx++)
         {
            printf("%x",encBuf[ieIdx]);
         }
#endif
      }
      
      /* Sending msg */
      if(sendF1APMsg() != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Sending GNB-DU Config Update failed");
         break;
      }
      
      if(fillE2NodeComponentReqInfo(F1, duCfgParam.duId, E2_NODE_COMPONENT_UPDATE, encBufSize, encBuf)!=ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Failed to update the e2 node in the list");
         break;
      }

      ret = ROK;
      break;
   }
  
   addToReservedF1apPduList(TRANS_ID,f1apDuCfg);
   return ret;
}


/*******************************************************************
 *
 * @brief free the ULRRCMessageTransfer
 *
 * @details
 *
 *    Function : FreeULRRCMessageTransfer
 *
 *    Functionality: Deallocating the memory of variable allocated in
 *                      FreeULRRCMessageTransfer
 *
 * @params[in]
 *
 * @return ROK     - void
 *
 ******************************************************************/
void FreeULRRCMessageTransfer( F1AP_PDU_t *f1apMsg)
{
   uint8_t idx1;
   ULRRCMessageTransfer_t  *ulRRCMsg;

   if(f1apMsg != NULLP)
   { 
      if(f1apMsg->choice.initiatingMessage != NULLP)
      {
	 ulRRCMsg = &f1apMsg->choice.initiatingMessage->value.choice.ULRRCMessageTransfer;
	 if(ulRRCMsg->protocolIEs.list.array != NULLP)
	 {
	    for(idx1=0;idx1<ulRRCMsg->protocolIEs.list.count;idx1++)
	    {
	       if(ulRRCMsg->protocolIEs.list.array[idx1] != NULLP)
	       {
		  if(ulRRCMsg->protocolIEs.list.array[idx1]->value.present ==
			ULRRCMessageTransferIEs__value_PR_RRCContainer)
		  {
		     DU_FREE(ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.RRCContainer.buf,
			   ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.RRCContainer.size);
		  }
		  DU_FREE(ulRRCMsg->protocolIEs.list.array[idx1],sizeof(ULRRCMessageTransferIEs_t));
	       }
	    }
	    DU_FREE(ulRRCMsg->protocolIEs.list.array,ulRRCMsg->protocolIEs.list.size );
	 }
	 DU_FREE(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
      }
      DU_FREE(f1apMsg,sizeof(F1AP_PDU_t));
   }
}
/*******************************************************************
 *
 * @brief Builds and sends the ULRRCMessageTransfer 
 *
 * @details
 *
 *    Function : BuildAndSendULRRCMessageTransfer
 *
 *    Functionality: Constructs the UL RRC Message Transfer and sends
 *                   it to the CU through SCTP.
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendULRRCMessageTransfer(DuUeCb  *ueCb, uint8_t lcId, \
      uint16_t msgLen, uint8_t *rrcMsg)
{
   uint8_t                 elementCnt=0, idx1=0, idx=0;
   uint8_t                 ret = RFAILED;
   F1AP_PDU_t              *f1apMsg = NULLP;
   ULRRCMessageTransfer_t  *ulRRCMsg = NULLP;
   asn_enc_rval_t          encRetVal;        /* Encoder return value */
   
   memset(&encRetVal, 0, sizeof(asn_enc_rval_t));

   while(true)
   {
      DU_LOG("\nINFO   -->  F1AP : Building UL RRC Message Transfer Message\n");

      DU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
	 DU_LOG(" ERROR  -->  F1AP : Memory allocation for F1AP-PDU failed");
	 break;
      }
      f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
      DU_ALLOC(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
      if(f1apMsg->choice.initiatingMessage == NULLP)
      {
	 DU_LOG(" ERROR  -->  F1AP : Memory allocation for	F1AP-PDU failed");
	 break;
      }
      f1apMsg->choice.initiatingMessage->procedureCode = ProcedureCode_id_ULRRCMessageTransfer;
      f1apMsg->choice.initiatingMessage->criticality = Criticality_ignore;
      f1apMsg->choice.initiatingMessage->value.present = \
							 InitiatingMessage__value_PR_ULRRCMessageTransfer;
      ulRRCMsg =
	 &f1apMsg->choice.initiatingMessage->value.choice.ULRRCMessageTransfer;
      elementCnt = 4;
      ulRRCMsg->protocolIEs.list.count = elementCnt;
      ulRRCMsg->protocolIEs.list.size = \
					elementCnt * sizeof(ULRRCMessageTransferIEs_t *);

      /* Initialize the F1Setup members */
      DU_ALLOC(ulRRCMsg->protocolIEs.list.array, ulRRCMsg->protocolIEs.list.size);
      if(ulRRCMsg->protocolIEs.list.array == NULLP)
      {
	 DU_LOG(" ERROR  -->  F1AP : Memory allocation for UL RRC MessageTransferIEs failed");
	 break;
      }
      for(idx=0; idx<elementCnt; idx++)
      {
	 DU_ALLOC(ulRRCMsg->protocolIEs.list.array[idx],sizeof(ULRRCMessageTransferIEs_t));
	 if(ulRRCMsg->protocolIEs.list.array[idx] == NULLP)
	 {
	    break;
	 }
      }

      idx1 = 0;

      /*GNB CU UE F1AP ID*/
      ulRRCMsg->protocolIEs.list.array[idx1]->id = ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;
      ulRRCMsg->protocolIEs.list.array[idx1]->criticality = Criticality_reject;
      ulRRCMsg->protocolIEs.list.array[idx1]->value.present = \
							      ULRRCMessageTransferIEs__value_PR_GNB_CU_UE_F1AP_ID;
      ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.GNB_CU_UE_F1AP_ID = ueCb->gnbCuUeF1apId;

      /*GNB DU UE F1AP ID*/
      idx1++;
      ulRRCMsg->protocolIEs.list.array[idx1]->id = ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
      ulRRCMsg->protocolIEs.list.array[idx1]->criticality = Criticality_reject;
      ulRRCMsg->protocolIEs.list.array[idx1]->value.present = \
							      ULRRCMessageTransferIEs__value_PR_GNB_DU_UE_F1AP_ID;
      ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.GNB_DU_UE_F1AP_ID = ueCb->gnbDuUeF1apId;

      /*SRBID*/
      idx1++;
      ulRRCMsg->protocolIEs.list.array[idx1]->id = ProtocolIE_ID_id_SRBID;
      ulRRCMsg->protocolIEs.list.array[idx1]->criticality = Criticality_reject;
      ulRRCMsg->protocolIEs.list.array[idx1]->value.present = \
							      ULRRCMessageTransferIEs__value_PR_SRBID;
      ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.SRBID = lcId;

      /*RRCContainer*/
      idx1++;
      ulRRCMsg->protocolIEs.list.array[idx1]->id  = ProtocolIE_ID_id_RRCContainer;
      ulRRCMsg->protocolIEs.list.array[idx1]->criticality = Criticality_reject;
      ulRRCMsg->protocolIEs.list.array[idx1]->value.present = \
							      ULRRCMessageTransferIEs__value_PR_RRCContainer;
      ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.RRCContainer.size = msgLen;
      DU_ALLOC(ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.RRCContainer.buf,
	    ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.RRCContainer.size)
      if(!ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.RRCContainer.buf)
      {
	 DU_LOG(" ERROR  -->  F1AP : Memory allocation for BuildAndSendULRRCMessageTransfer failed");
	 break;
      }
      memset(ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.RRCContainer.buf, 0, msgLen);
      memcpy(ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.RRCContainer.buf, \
	    rrcMsg, ulRRCMsg->protocolIEs.list.array[idx1]->value.choice.RRCContainer.size);

      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the F1SetupRequest type as APER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
	    encBuf);
      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
	 DU_LOG( "\nERROR  -->  F1AP : Could not encode ULRRCMessageTransfer structure (at %s)\n",\
	       encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
	 break;
      }
      else
      {
	 DU_LOG("\nDEBUG  -->  F1AP : Created APER encoded buffer for ULRRCMessageTransfer\n");
#ifdef DEBUG_ASN_PRINT
	 for(int i=0; i< encBufSize; i++)
	 {
	    printf("%x",encBuf[i]);
	 }
#endif
      }

      /* Sending  msg  */
      if(sendF1APMsg()	!=	ROK)
      {
	 DU_LOG("\nERROR  -->   F1AP : Sending	UL RRC Message Transfer Failed");
	 break;
      }
      ret = ROK;
      break;
   }
   FreeULRRCMessageTransfer(f1apMsg);

   return ret;
}/* End of BuildAndSendULRRCMessageTransfer*/

/*******************************************************************
 *
 * @brief Builds tag config 
 *
 * @details
 *
 *    Function : BuildTagConfig 
 *
 *    Functionality: Builds tag config in MacCellGroupConfig
 *
 * @params[in] TAG_Config *tag_Config
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildTagConfig(DuUeCb *ueCb, struct TAG_Config *tagConfig)
{
   struct TAG_Config__tag_ToAddModList *tagList;
   uint8_t                     idx, elementCnt;

   tagConfig->tag_ToReleaseList = NULLP;
   tagConfig->tag_ToAddModList = NULLP;
   DU_ALLOC(tagConfig->tag_ToAddModList, sizeof(struct TAG_Config__tag_ToAddModList));
   if(!tagConfig->tag_ToAddModList)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildTagConfig");
      return RFAILED;
   }

   if(ueCb == NULLP)
      elementCnt = ODU_VALUE_ONE;
   else
      elementCnt = ueCb->duMacUeCfg.macCellGrpCfg.tagCfg.addModListCount;

   tagList = tagConfig->tag_ToAddModList;
   tagList->list.count = elementCnt;
   tagList->list.size  =  elementCnt * sizeof(struct TAG *);

   tagList->list.array = NULLP;
   DU_ALLOC(tagList->list.array, tagList->list.size);
   if(!tagList->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildTagConfig");
      return RFAILED;
   }

   for(idx=0; idx<tagList->list.count; idx++)
   {
      tagList->list.array[idx] = NULLP;
      DU_ALLOC(tagList->list.array[idx], sizeof(struct TAG));
      if(!tagList->list.array[idx])
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildTagConfig");
         return RFAILED;
      }
   }

   if(ueCb == NULLP)
   {
      idx = 0;
      tagList->list.array[idx]->tag_Id = TAG_ID;
      tagList->list.array[idx]->timeAlignmentTimer = TIME_ALIGNMENT_TMR;
   }
   else
   {
      for(idx=0; idx<tagList->list.count; idx++)
      {
         tagList->list.array[idx]->tag_Id = ueCb->duMacUeCfg.macCellGrpCfg.tagCfg.addModList[idx].tagId;
         tagList->list.array[idx]->timeAlignmentTimer = ueCb->duMacUeCfg.macCellGrpCfg.tagCfg.addModList[idx].timeAlignTimer;
      }
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds PHR Config 
 *
 * @details
 *
 *    Function : BuildPhrConfig
 *
 *    Functionality: Builds phrConfig in MacCellGroupConfig
 *
 * @params[in] PHR Config *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPhrConfig(DuUeCb *ueCb, struct MAC_CellGroupConfig__phr_Config *phrConfig)
{

   phrConfig->present = MAC_CellGroupConfig__phr_Config_PR_setup;
   phrConfig->choice.setup = NULLP;
   DU_ALLOC(phrConfig->choice.setup, sizeof(struct PHR_Config));
   if(!phrConfig->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildPhrConfig");
      return RFAILED;
   }

   if(ueCb == NULLP)
   {
      phrConfig->choice.setup->phr_PeriodicTimer        = PHR_PERIODIC_TMR;
      phrConfig->choice.setup->phr_ProhibitTimer        = PHR_PROHIBHIT_TMR;
      phrConfig->choice.setup->phr_Tx_PowerFactorChange = PHR_PWR_FACTOR_CHANGE;
      phrConfig->choice.setup->multiplePHR              = false;
      phrConfig->choice.setup->dummy                    = false;
      phrConfig->choice.setup->phr_Type2OtherCell       = false;
      phrConfig->choice.setup->phr_ModeOtherCG          = PHR_MODE_OTHER_CG;
   }
   else
   {
      phrConfig->choice.setup->phr_PeriodicTimer        = ueCb->duMacUeCfg.macCellGrpCfg.phrCfg.periodicTimer;
      phrConfig->choice.setup->phr_ProhibitTimer        = ueCb->duMacUeCfg.macCellGrpCfg.phrCfg.prohibitTimer;
      phrConfig->choice.setup->phr_Tx_PowerFactorChange = ueCb->duMacUeCfg.macCellGrpCfg.phrCfg.txPowerFactor;
      phrConfig->choice.setup->multiplePHR              = ueCb->duMacUeCfg.macCellGrpCfg.phrCfg.multiplePHR;
      phrConfig->choice.setup->dummy                    = ueCb->duMacUeCfg.macCellGrpCfg.phrCfg.dummy;
      phrConfig->choice.setup->phr_Type2OtherCell       = ueCb->duMacUeCfg.macCellGrpCfg.phrCfg.phrType2OtherCell;
      phrConfig->choice.setup->phr_ModeOtherCG          = ueCb->duMacUeCfg.macCellGrpCfg.phrCfg.phrOtherCG;
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds BSR Config 
 *
 * @details
 *
 *    Function : BuildBsrConfig
 *
 *    Functionality: Builds BuildBsrConfig in MacCellGroupConfig
 *
 * @params[in] BSR_Config *bsrConfig
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildBsrConfig(DuUeCb *ueCb, struct BSR_Config *bsrConfig)
{
   if(ueCb == NULLP)
   {
      bsrConfig->periodicBSR_Timer = PERIODIC_BSR_TMR;
      bsrConfig->retxBSR_Timer     = RETX_BSR_TMR;
      bsrConfig->logicalChannelSR_DelayTimer = NULLP;
   }
   else
   {
      bsrConfig->periodicBSR_Timer = convertBsrPeriodicTmrValueToEnum(ueCb->duMacUeCfg.macCellGrpCfg.bsrTmrCfg.periodicTimer);
      bsrConfig->retxBSR_Timer     = convertBsrRetxTmrValueToEnum(ueCb->duMacUeCfg.macCellGrpCfg.bsrTmrCfg.retxTimer);

      bsrConfig->logicalChannelSR_DelayTimer = NULLP;
      DU_ALLOC(bsrConfig->logicalChannelSR_DelayTimer, sizeof(long));
      if(bsrConfig->logicalChannelSR_DelayTimer == NULLP)
      {
         DU_LOG("\nERROR  --> DU APP: Memory allocation failed in BuildBsrConfig");
         return RFAILED;
      }
      *(bsrConfig->logicalChannelSR_DelayTimer) = convertLcSrDelayTmrValueToEnum(ueCb->duMacUeCfg.macCellGrpCfg.bsrTmrCfg.srDelayTimer);
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds scheduling request config 
 *
 * @details
 *
 *    Function : BuildSchedulingReqConfig 
 *
 *    Functionality: Builds BuildSchedulingReqConfig in MacCellGroupConfig
 *
 * @params[in] SchedulingRequestConfig *schedulingRequestConfig
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSchedulingReqConfig(DuUeCb *ueCb, struct SchedulingRequestConfig *schedulingRequestConfig)
{
   struct SchedulingRequestConfig__schedulingRequestToAddModList *schReqList;
   uint8_t                     idx, elementCnt;

   schedulingRequestConfig->schedulingRequestToAddModList = NULLP;
   DU_ALLOC(schedulingRequestConfig->schedulingRequestToAddModList,
	 sizeof(struct SchedulingRequestConfig__schedulingRequestToAddModList));
   if(!schedulingRequestConfig->schedulingRequestToAddModList)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSchedulingReqConfig");
      return RFAILED;
   }

   if(ueCb == NULLP)
      elementCnt = ODU_VALUE_ONE;
   else
      elementCnt = ueCb->duMacUeCfg.macCellGrpCfg.schReqCfg.addModListCount;

   schReqList = schedulingRequestConfig->schedulingRequestToAddModList;
   schReqList->list.count = elementCnt;
   schReqList->list.size  = elementCnt * sizeof(struct SchedulingRequestToAddMod *);

   schReqList->list.array = NULLP;
   DU_ALLOC(schReqList->list.array, schReqList->list.size);
   if(!schReqList->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSchedulingReqConfig");
      return RFAILED;
   }

   for(idx=0; idx<schReqList->list.count; idx++)
   {
      schReqList->list.array[idx] = NULLP;
      DU_ALLOC(schReqList->list.array[idx], sizeof(struct SchedulingRequestToAddMod));
      if(!schReqList->list.array[idx])
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSchedulingReqConfig");
         return RFAILED;
      }
   }

   if(ueCb == NULLP)
   {
      idx = 0;
      schReqList->list.array[idx]->schedulingRequestId = SCH_REQ_ID;

      schReqList->list.array[idx]->sr_ProhibitTimer = NULLP;
      DU_ALLOC(schReqList->list.array[idx]->sr_ProhibitTimer, sizeof(long));
      if(!schReqList->list.array[idx]->sr_ProhibitTimer)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSchedulingReqConfig");
         return RFAILED;
      }
      *(schReqList->list.array[idx]->sr_ProhibitTimer) = SR_PROHIBIT_TMR;
      schReqList->list.array[idx]->sr_TransMax = SR_TRANS_MAX;
   }
   else
   {
      for(idx=0; idx<schReqList->list.count; idx++)
      {
         schReqList->list.array[idx]->schedulingRequestId = ueCb->duMacUeCfg.macCellGrpCfg.schReqCfg.addModList[idx].schedReqId;

         schReqList->list.array[idx]->sr_ProhibitTimer = NULLP;
         DU_ALLOC(schReqList->list.array[idx]->sr_ProhibitTimer, sizeof(long));
         if(!schReqList->list.array[idx]->sr_ProhibitTimer)
         {
            DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSchedulingReqConfig");
            return RFAILED;
         }
         *(schReqList->list.array[idx]->sr_ProhibitTimer) = ueCb->duMacUeCfg.macCellGrpCfg.schReqCfg.addModList[idx].srProhibitTmr;
         schReqList->list.array[idx]->sr_TransMax = ueCb->duMacUeCfg.macCellGrpCfg.schReqCfg.addModList[idx].srTransMax;
      }
   }

   schedulingRequestConfig->schedulingRequestToReleaseList = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds RLC Configuration for AM mode
 *
 * @details
 *
 *    Function : BuildRlcConfigAm
 *
 *    Functionality: 
 *       Builds AM mode RLC Config in BuildRlcBearerToAddModList
 *
 * @params[in] AmBearerCfg *amCfg
 *             RLC_Config_t  *rlcConfig
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildRlcConfigAm(AmBearerCfg *amCfg, struct RLC_Config *rlcConfig)
{
   rlcConfig->choice.am = NULLP;
   DU_ALLOC(rlcConfig->choice.am, sizeof(struct RLC_Config__am));
   if(!rlcConfig->choice.am)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcConfigAm");
      return RFAILED;
   }

   /* Fill AM UL configuration */
   rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength = NULLP;
   DU_ALLOC(rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength, sizeof(SN_FieldLengthAM_t));
   if(!rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcConfigAm");
      return RFAILED;
   }

   /* Fill default AM UL configuration if input pointer to DU database is NULL */
   if(amCfg == NULLP)
   {
      
      *(rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength) = SN_FIELD_LEN_12BIT; /*As per Spec 38.331, "Network configures only value size12 in SN-FieldLengthAM for SRB"*/
      rlcConfig->choice.am->ul_AM_RLC.t_PollRetransmit  = T_POLL_RETRANSMIT;
      rlcConfig->choice.am->ul_AM_RLC.pollPDU           = POLL_PDU;
      rlcConfig->choice.am->ul_AM_RLC.pollByte          = POLL_BYTE;
      rlcConfig->choice.am->ul_AM_RLC.maxRetxThreshold  = MAX_RETX_THRESHOLD;
   }
   else
   {
      *(rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength) = covertAmSnLenFromIntEnumToRrcEnum(amCfg->dlAmCfg.snLenDl);
      rlcConfig->choice.am->ul_AM_RLC.t_PollRetransmit  = covertPollRetxTmrValueToEnum(amCfg->dlAmCfg.pollRetxTmr);
      rlcConfig->choice.am->ul_AM_RLC.pollPDU           = covertPollPduValueToEnum(amCfg->dlAmCfg.pollPdu);
      rlcConfig->choice.am->ul_AM_RLC.pollByte          = covertPollByteValueToEnum(amCfg->dlAmCfg.pollByte);
      rlcConfig->choice.am->ul_AM_RLC.maxRetxThreshold  = covertMaxRetxValueToEnum(amCfg->dlAmCfg.maxRetxTh);
   }

   /* Fill AM DL configuraion */
   rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength = NULLP;
   DU_ALLOC(rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength, sizeof(SN_FieldLengthAM_t)); 
   if(!rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcConfigAm");
      return RFAILED;
   }

   /* Fill default AM DL configuration if input pointer to DU database is NULL */
   if(amCfg == NULLP)
   {
      *(rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength) = SN_FIELD_LEN_12BIT; /*As per Spec 38.331, "Network configures only value size12 in SN-FieldLengthAM for SRB"*/
      rlcConfig->choice.am->dl_AM_RLC.t_Reassembly      = T_REASSEMBLY;
      rlcConfig->choice.am->dl_AM_RLC.t_StatusProhibit  = T_STATUS_PROHIBHIT;
   }
   else /* Fill AM configuration from DU database */
   {
      *(rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength) = covertAmSnLenFromIntEnumToRrcEnum(amCfg->ulAmCfg.snLenUl);
      rlcConfig->choice.am->dl_AM_RLC.t_Reassembly      = convertReasmblTmrValueToEnum(amCfg->ulAmCfg.reAssemTmr);
      rlcConfig->choice.am->dl_AM_RLC.t_StatusProhibit  = convertProhibitTmrValueToEnum(amCfg->ulAmCfg.statProhTmr);
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds RLC Config for UM Bidirection
 *
 * @details
 *
 *    Function : BuildRlcConfig UmBiDir
 *
 *    Functionality: 
 *       Builds RLC Config for UM Bidirection in BuildRlcBearerToAddModList 
 *
 * @params[in] UmBiDirBearerCfg *umBiDirCfg
 *             RLC_Config_t *rlcConfig
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildRlcConfigUmBiDir(UmBiDirBearerCfg *umBiDirCfg, struct RLC_Config *rlcConfig)
{
   rlcConfig->choice.um_Bi_Directional = NULLP;
   DU_ALLOC(rlcConfig->choice.um_Bi_Directional, sizeof(struct RLC_Config__um_Bi_Directional));
   if(rlcConfig->choice.um_Bi_Directional == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcConfigUmBiDir");
      return RFAILED;
   }

   /* Fill UM Bidirectional UL configuration */
   rlcConfig->choice.um_Bi_Directional->ul_UM_RLC.sn_FieldLength = NULLP;
   DU_ALLOC(rlcConfig->choice.um_Bi_Directional->ul_UM_RLC.sn_FieldLength, sizeof(SN_FieldLengthUM_t));
   if(rlcConfig->choice.um_Bi_Directional->ul_UM_RLC.sn_FieldLength == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcConfigUmBiDir");
      return RFAILED;
   }

   if(umBiDirCfg != NULLP)
   {
      *(rlcConfig->choice.um_Bi_Directional->ul_UM_RLC.sn_FieldLength) = covertUmSnLenFromIntEnumToRrcEnum(umBiDirCfg->dlUmCfg.snLenDlUm);     
   }

   /* Fill UM Bidirectional DL configuration */
   rlcConfig->choice.um_Bi_Directional->dl_UM_RLC.sn_FieldLength = NULLP;
   DU_ALLOC(rlcConfig->choice.um_Bi_Directional->dl_UM_RLC.sn_FieldLength, sizeof(SN_FieldLengthUM_t));
   if(rlcConfig->choice.um_Bi_Directional->dl_UM_RLC.sn_FieldLength == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcConfigUmBiDir");
      return RFAILED;
   }

   if(umBiDirCfg != NULLP)
   {
      *(rlcConfig->choice.um_Bi_Directional->dl_UM_RLC.sn_FieldLength) = covertUmSnLenFromIntEnumToRrcEnum(umBiDirCfg->ulUmCfg.snLenUlUm);     
      rlcConfig->choice.um_Bi_Directional->dl_UM_RLC.t_Reassembly = convertReasmblTmrValueToEnum(umBiDirCfg->ulUmCfg.reAssemTmr);
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds RLC Config for UM Uni directional UL
 *
 * @details
 *
 *    Function : BuildRlcConfigUmUniDirUl
 *
 *    Functionality: 
 *       Builds RLC Config for UM Unidirection UL in BuildRlcBearerToAddModList 
 *
 * @params[in] UmUniDirDlBearerCfg *umUniDirDlCfg
 *             RLC_Config_t *rlcConfig
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildRlcConfigUmUniDirUl(UmUniDirDlBearerCfg *umUniDirDlCfg, RLC_Config_t *rlcConfig)
{
   rlcConfig->choice.um_Uni_Directional_UL = NULLP;
   DU_ALLOC(rlcConfig->choice.um_Uni_Directional_UL , sizeof(struct RLC_Config__um_Uni_Directional_UL));
   if(rlcConfig->choice.um_Uni_Directional_UL == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcConfigUmUniDirUl");
      return RFAILED;
   }

   rlcConfig->choice.um_Uni_Directional_UL->ul_UM_RLC.sn_FieldLength = NULLP;
   DU_ALLOC(rlcConfig->choice.um_Uni_Directional_UL->ul_UM_RLC.sn_FieldLength, sizeof(SN_FieldLengthUM_t));
   if(rlcConfig->choice.um_Uni_Directional_UL->ul_UM_RLC.sn_FieldLength == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcConfigUmUniDirUl");
      return RFAILED;
   }

   if(umUniDirDlCfg != NULLP)
   {
      *(rlcConfig->choice.um_Uni_Directional_UL->ul_UM_RLC.sn_FieldLength) = covertUmSnLenFromIntEnumToRrcEnum(umUniDirDlCfg->dlUmCfg.snLenDlUm);
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds RLC Config for UM Uni directional DL
 *
 * @details
 *
 *    Function : BuildRlcConfigUmUniDirDl
 *
 *    Functionality: 
 *       Builds RLC Config for UM Unidirection DL in BuildRlcBearerToAddModList 
 *
 * @params[in] UmUniDirUlBearerCfg *umUniDirUlCfg
 *             RLC_Config_t *rlcConfig
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildRlcConfigUmUniDirDl(UmUniDirUlBearerCfg *umUniDirUlCfg, RLC_Config_t *rlcConfig)
{
   rlcConfig->choice.um_Uni_Directional_DL = NULLP;
   DU_ALLOC(rlcConfig->choice.um_Uni_Directional_DL , sizeof(struct RLC_Config__um_Uni_Directional_DL));
   if(rlcConfig->choice.um_Uni_Directional_DL == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcConfigUmUniDirDl");
      return RFAILED;
   }

   rlcConfig->choice.um_Uni_Directional_DL->dl_UM_RLC.sn_FieldLength = NULLP;
   DU_ALLOC(rlcConfig->choice.um_Uni_Directional_DL->dl_UM_RLC.sn_FieldLength, sizeof(SN_FieldLengthUM_t));
   if(rlcConfig->choice.um_Uni_Directional_DL->dl_UM_RLC.sn_FieldLength == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcConfigUmUniDirDl");
      return RFAILED;
   }

   if(umUniDirUlCfg != NULLP)
   {
      *(rlcConfig->choice.um_Uni_Directional_DL->dl_UM_RLC.sn_FieldLength) = covertUmSnLenFromIntEnumToRrcEnum(umUniDirUlCfg->ulUmCfg.snLenUlUm);
      rlcConfig->choice.um_Uni_Directional_DL->dl_UM_RLC.t_Reassembly = convertReasmblTmrValueToEnum(umUniDirUlCfg->ulUmCfg.reAssemTmr);
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds RLC Config
 *
 * @details
 *
 *    Function : BuildRlcConfig
 *
 *    Functionality: Builds RLC Config in BuildRlcBearerToAddModList 
 *
 * @params[in] RLC_Config_t *rlcConfig
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildRlcConfig(RlcBearerCfg *rbCfg, struct RLC_Config *rlcConfig)
{
   
   /* Fill default values if rbCfg is NULL */
   if(rbCfg == NULLP)
   {
      rlcConfig->present = RLC_Config_PR_am;
      BuildRlcConfigAm(NULLP, rlcConfig);
   }
   /* If RbCfg is present, fill RLC configurations from DU Database */
   else
   {
      rlcConfig->present = covertRlcModeFromIntEnumToRrcEnum(rbCfg->rlcMode);
      switch(rlcConfig->present)
      {
         case RLC_Config_PR_am:
            BuildRlcConfigAm(rbCfg->u.amCfg, rlcConfig);
            break;
         case RLC_Config_PR_um_Bi_Directional:
            BuildRlcConfigUmBiDir(rbCfg->u.umBiDirCfg, rlcConfig);
            break;
         case RLC_Config_PR_um_Uni_Directional_UL:
            BuildRlcConfigUmUniDirUl(rbCfg->u.umUniDirDlCfg, rlcConfig);
            break;
         case RLC_Config_PR_um_Uni_Directional_DL:
            BuildRlcConfigUmUniDirDl(rbCfg->u.umUniDirUlCfg, rlcConfig);
            break;
         case RLC_Config_PR_NOTHING:
         default:
            break;
      }
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds MAC LC Config
 *
 * @details
 *
 *    Function : BuildMacLCConfig 
 *
 *    Functionality: Builds MAC LC Config in BuildRlcBearerToAddModList 
 *
 * @params[in] struct LogicalChannelConfig macLcConfig
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildMacLCConfig(LcCfg *lcCfgDb, struct LogicalChannelConfig *macLcConfig)
{
   macLcConfig->ul_SpecificParameters = NULLP;
   DU_ALLOC(macLcConfig->ul_SpecificParameters, sizeof(struct LogicalChannelConfig__ul_SpecificParameters));
   if(!macLcConfig->ul_SpecificParameters)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildMacLCConfig");
      return RFAILED;
   }

   if(lcCfgDb == NULLP)
   {
      macLcConfig->ul_SpecificParameters->priority = MAC_LC_PRIORITY;
      macLcConfig->ul_SpecificParameters->prioritisedBitRate =	PRIORTISIED_BIT_RATE;
      macLcConfig->ul_SpecificParameters->bucketSizeDuration =	BUCKET_SIZE_DURATION;
   }
   else
   {
      macLcConfig->ul_SpecificParameters->priority = lcCfgDb->ulLcCfg.priority;
      macLcConfig->ul_SpecificParameters->prioritisedBitRate = lcCfgDb->ulLcCfg.pbr;
      macLcConfig->ul_SpecificParameters->bucketSizeDuration = lcCfgDb->ulLcCfg.bsd;
   }

   macLcConfig->ul_SpecificParameters->allowedServingCells = NULLP;
   macLcConfig->ul_SpecificParameters->allowedSCS_List = NULLP;
   macLcConfig->ul_SpecificParameters->maxPUSCH_Duration = NULLP;
   macLcConfig->ul_SpecificParameters->configuredGrantType1Allowed = NULLP;

   macLcConfig->ul_SpecificParameters->logicalChannelGroup = NULLP;
   DU_ALLOC(macLcConfig->ul_SpecificParameters->logicalChannelGroup,	sizeof(long));
   if(!macLcConfig->ul_SpecificParameters->logicalChannelGroup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildMacLCConfig");
      return RFAILED;
   }

   if(lcCfgDb == NULLP)
      *(macLcConfig->ul_SpecificParameters->logicalChannelGroup) = LC_GRP;
   else
      *(macLcConfig->ul_SpecificParameters->logicalChannelGroup) = lcCfgDb->ulLcCfg.lcGroup;

   macLcConfig->ul_SpecificParameters->schedulingRequestID = NULLP;
   DU_ALLOC(macLcConfig->ul_SpecificParameters->schedulingRequestID,	sizeof(SchedulingRequestId_t));
   if(!macLcConfig->ul_SpecificParameters->schedulingRequestID)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildMacLCConfig");
      return RFAILED;
   }

   if(lcCfgDb == NULLP)
      *(macLcConfig->ul_SpecificParameters->schedulingRequestID) = SCH_REQ_ID;
   else
      *(macLcConfig->ul_SpecificParameters->schedulingRequestID) = lcCfgDb->ulLcCfg.schReqId;

   macLcConfig->ul_SpecificParameters->logicalChannelSR_Mask = false;
   macLcConfig->ul_SpecificParameters->logicalChannelSR_DelayTimerApplied = false;
   macLcConfig->ul_SpecificParameters->bitRateQueryProhibitTimer = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds RLC Bearer to Add/Mod list
 *
 * @details
 *
 *    Function :BuildRlcBearerToAddModList 
 *
 *    Functionality: Builds RLC Bearer to Add/Mod list in DuToCuRrcContainer
 *
 * @params[in] rlc_BearerToAddModList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildRlcBearerToAddModList(DuUeCb *ueCb, struct CellGroupConfigRrc__rlc_BearerToAddModList *rlcBearerList)
{
   uint8_t  idx = 0, lcIdx=0, macLcIdx = 0, elementCnt = 0;

   if(ueCb == NULLP)
      elementCnt = 1;
   else if(ueCb->f1UeDb->actionType == UE_CTXT_CFG_QUERY)
      elementCnt = ueCb->duRlcUeCfg.numLcs;
   else
   {
      for(lcIdx = 0; lcIdx<ueCb->duRlcUeCfg.numLcs; lcIdx++)
      {
         if(ueCb->duRlcUeCfg.rlcLcCfg[lcIdx].rlcBearerCfg.isLcAddModRspSent == false)
            elementCnt++;
      }
   }
   rlcBearerList->list.count = elementCnt;
   rlcBearerList->list.size  = elementCnt * sizeof(struct RLC_BearerConfig *);

   rlcBearerList->list.array = NULLP;
   DU_ALLOC(rlcBearerList->list.array, rlcBearerList->list.size);
   if(!rlcBearerList->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcBearerToAddModList");
      return RFAILED;
   }

   for(idx=0; idx<rlcBearerList->list.count; idx++)
   {
      rlcBearerList->list.array[idx] = NULLP;
      DU_ALLOC(rlcBearerList->list.array[idx], sizeof(struct RLC_BearerConfig));
      if(!rlcBearerList->list.array[idx])
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcBearerToAddModList");
         return RFAILED;
      }
   }

   if(ueCb == NULLP)
   {
      idx=0;
      rlcBearerList->list.array[idx]->logicalChannelIdentity = SRB1_LCID;
      DU_ALLOC(rlcBearerList->list.array[idx]->servedRadioBearer, sizeof(struct RLC_BearerConfig__servedRadioBearer));
      if(!rlcBearerList->list.array[idx]->servedRadioBearer)
      {     
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcBearerToAddModList");
         return RFAILED;
      }     
      rlcBearerList->list.array[idx]->servedRadioBearer->present = RLC_BearerConfig__servedRadioBearer_PR_srb_Identity;
      rlcBearerList->list.array[idx]->servedRadioBearer->choice.srb_Identity = SRB1_LCID;
      rlcBearerList->list.array[idx]->reestablishRLC = NULLP;

      /* Fill RLC related Configurations for this Radio Bearer */
      rlcBearerList->list.array[idx]->rlc_Config = NULLP;
      DU_ALLOC(rlcBearerList->list.array[idx]->rlc_Config, sizeof(struct RLC_Config));
      if(!rlcBearerList->list.array[idx]->rlc_Config)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcBearerToAddModList");
         return RFAILED;
      }
      if(BuildRlcConfig(NULLP, rlcBearerList->list.array[idx]->rlc_Config) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildRlcConfig failed");
         return RFAILED;
      }

      /* Fill MAC related configurations for this Radio Bearer */
      rlcBearerList->list.array[idx]->mac_LogicalChannelConfig = NULLP;
      DU_ALLOC(rlcBearerList->list.array[idx]->mac_LogicalChannelConfig, sizeof(struct LogicalChannelConfig));
      if(!rlcBearerList->list.array[idx]->mac_LogicalChannelConfig)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcBearerToAddModList");
         return RFAILED;
      }
      if(BuildMacLCConfig(NULLP, rlcBearerList->list.array[idx]->mac_LogicalChannelConfig) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildMacLCConfig failed");
         return RFAILED;
      }
   }
   else
   {
      idx=0;
      for(lcIdx=0; lcIdx<ueCb->duRlcUeCfg.numLcs; lcIdx++)
      {
         if((ueCb->f1UeDb->actionType != UE_CTXT_CFG_QUERY) && (ueCb->duRlcUeCfg.rlcLcCfg[lcIdx].rlcBearerCfg.isLcAddModRspSent == true))
            continue;

         /* Fill Logical channel identity */
         rlcBearerList->list.array[idx]->logicalChannelIdentity = ueCb->duRlcUeCfg.rlcLcCfg[lcIdx].rlcBearerCfg.lcId;

         /* Fill Radio Bearer Id and type (DRB/SRB) for this logical channel */
         DU_ALLOC(rlcBearerList->list.array[idx]->servedRadioBearer, sizeof(struct RLC_BearerConfig__servedRadioBearer));
         if(!rlcBearerList->list.array[idx]->servedRadioBearer)
         {
            DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcBearerToAddModList");
            return RFAILED;
         }
         rlcBearerList->list.array[idx]->servedRadioBearer->present = \
                                                                      covertRbTypeFromIntEnumToRrcEnum(ueCb->duRlcUeCfg.rlcLcCfg[lcIdx].rlcBearerCfg.rbType);
         switch(rlcBearerList->list.array[idx]->servedRadioBearer->present)
         {
            case RLC_BearerConfig__servedRadioBearer_PR_srb_Identity: 
               rlcBearerList->list.array[idx]->servedRadioBearer->choice.srb_Identity = ueCb->duRlcUeCfg.rlcLcCfg[lcIdx].rlcBearerCfg.rbId;
               break;
            case RLC_BearerConfig__servedRadioBearer_PR_drb_Identity:
               rlcBearerList->list.array[idx]->servedRadioBearer->choice.drb_Identity = ueCb->duRlcUeCfg.rlcLcCfg[lcIdx].rlcBearerCfg.rbId;
               break;
            case RLC_BearerConfig__servedRadioBearer_PR_NOTHING:
            default:
               break;
         }
         ueCb->duRlcUeCfg.rlcLcCfg[lcIdx].rlcBearerCfg.isLcAddModRspSent = true;

         rlcBearerList->list.array[idx]->reestablishRLC = NULLP;

         /* Fill RLC related Configurations for this Radio Bearer */
         rlcBearerList->list.array[idx]->rlc_Config = NULLP;
         DU_ALLOC(rlcBearerList->list.array[idx]->rlc_Config, sizeof(struct RLC_Config));
         if(!rlcBearerList->list.array[idx]->rlc_Config)
         {
            DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcBearerToAddModList");
            return RFAILED;
         }
         if(BuildRlcConfig(&ueCb->duRlcUeCfg.rlcLcCfg[lcIdx].rlcBearerCfg, rlcBearerList->list.array[idx]->rlc_Config) != ROK)
         {
            DU_LOG("\nERROR  -->  F1AP : BuildRlcConfig failed");
            return RFAILED;
         }

         /* Fill MAC related configurations for this Radio Bearer */
         rlcBearerList->list.array[idx]->mac_LogicalChannelConfig = NULLP;
         DU_ALLOC(rlcBearerList->list.array[idx]->mac_LogicalChannelConfig, sizeof(struct LogicalChannelConfig));
         if(!rlcBearerList->list.array[idx]->mac_LogicalChannelConfig)
         {
            DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcBearerToAddModList");
            return RFAILED;
         }
         for(macLcIdx = 0; macLcIdx < ueCb->duMacUeCfg.numLcs; macLcIdx++)
         {
            if(ueCb->duMacUeCfg.lcCfgList[macLcIdx].lcConfig.lcId == ueCb->duRlcUeCfg.rlcLcCfg[lcIdx].rlcBearerCfg.lcId)
            {
               if(BuildMacLCConfig(&ueCb->duMacUeCfg.lcCfgList[macLcIdx].lcConfig, rlcBearerList->list.array[idx]->mac_LogicalChannelConfig) != ROK)
               {
                  DU_LOG("\nERROR  -->  F1AP : BuildMacLCConfig failed");
                  return RFAILED;
               }
               break;
            }
         }

         idx++;
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Build Control resource set to add/modify list 
 *
 * @details
 *
 *    Function : BuildControlRSetToAddModList
 *
 *    Functionality: Build Control resource set to add/modify list
 *
 * @params[in] 
 * struct PDCCH_Config__controlResourceSetToAddModList *controlRSetList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildControlRSetToAddModList(PdcchConfig *pdcchCfg, struct PDCCH_Config__controlResourceSetToAddModList *controlRSetList)
{
   uint8_t idx;
   uint8_t elementCnt;
   uint8_t numBytes, bitsUnused;
   struct ControlResourceSet *controlRSet;
   uint8_t freqDomainResource[FREQ_DOM_RSRC_SIZE] = {0};
   uint8_t coreset0EndPrb, coreset1StartPrb, coreset1NumPrb;

   if(pdcchCfg == NULLP)
      elementCnt = 1;
   else
      elementCnt = pdcchCfg->numCRsetToAddMod;

   controlRSetList->list.count = elementCnt;
   controlRSetList->list.size = elementCnt * sizeof(struct ControlResourceSet *);

   controlRSetList->list.array = NULLP;
   DU_ALLOC(controlRSetList->list.array, controlRSetList->list.size);
   if(!controlRSetList->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildControlRSetToAddModList");
      return RFAILED;
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
      controlRSetList->list.array[idx] = NULLP;
      DU_ALLOC(controlRSetList->list.array[idx], sizeof(struct ControlResourceSet));
      if(!controlRSetList->list.array[idx])
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildControlRSetToAddModList");
         return RFAILED;
      }
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
      controlRSet = controlRSetList->list.array[idx];

      if(pdcchCfg == NULLP)
         controlRSet->controlResourceSetId = PDCCH_CTRL_RSRC_SET_ONE_ID;
      else
         controlRSet->controlResourceSetId = pdcchCfg->cRSetToAddModList[idx].cRSetId;

      /* size 6 bytes
       * 3 LSBs unsued
       * Bit string stored ff0000000000
       */
      numBytes = 6;
      bitsUnused = 3;
      controlRSet->frequencyDomainResources.size = numBytes * sizeof(uint8_t);

      controlRSet->frequencyDomainResources.buf = NULLP;
      DU_ALLOC(controlRSet->frequencyDomainResources.buf, controlRSet->frequencyDomainResources.size);
      if(!controlRSet->frequencyDomainResources.buf)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildControlRSetToAddModList");
         return RFAILED;
      }

      memset(controlRSet->frequencyDomainResources.buf, 0, FREQ_DOM_RSRC_SIZE);

      if(pdcchCfg == NULLP)
      {
         coreset0EndPrb = CORESET0_END_PRB;
         coreset1StartPrb = coreset0EndPrb + 6;
         coreset1NumPrb = CORESET1_NUM_PRB;
         /* calculate the PRBs */
         fillCoresetFeqDomAllocMap(((coreset1StartPrb)/6), (coreset1NumPrb/6), freqDomainResource);
         memcpy(controlRSet->frequencyDomainResources.buf, freqDomainResource, FREQ_DOM_RSRC_SIZE);
         controlRSet->frequencyDomainResources.bits_unused = bitsUnused;

         controlRSet->duration = PDCCH_CTRL_RSRC_SET_ONE_DURATION;
         controlRSet->cce_REG_MappingType.present = ControlResourceSet__cce_REG_MappingType_PR_nonInterleaved;
         controlRSet->precoderGranularity = PDCCH_CTRL_RSRC_SET_ONE_PRECOD_GRANULARITY;
      }
      else
      {
         memcpy(controlRSet->frequencyDomainResources.buf, pdcchCfg->cRSetToAddModList[idx].freqDomainRsrc, FREQ_DOM_RSRC_SIZE);
         controlRSet->frequencyDomainResources.bits_unused = bitsUnused;
         controlRSet->duration = pdcchCfg->cRSetToAddModList[idx].duration;
         controlRSet->cce_REG_MappingType.present = pdcchCfg->cRSetToAddModList[idx].cceRegMappingType;
         controlRSet->precoderGranularity = pdcchCfg->cRSetToAddModList[idx].precoderGranularity;
      }
      controlRSet->tci_StatesPDCCH_ToAddList = NULLP;
      controlRSet->tci_StatesPDCCH_ToReleaseList = NULLP;
      controlRSet->tci_PresentInDCI = NULLP;

#if 0
      uint8_t tciStateIdx;

      DU_ALLOC(controlRset->tci_StatesPDCCH_ToAddList, \
            sizeof(struct ControlResourceSet__tci_StatesPDCCH_ToAddList));
      if(!controlRset->tci_StatesPDCCH_ToAddList)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildControlRSetToAddModList");
         return RFAILED;
      }

      elementCnt = 1;
      controlRset->tci_StatesPDCCH_ToAddList->list.count = elementCnt;
      controlRset->tci_StatesPDCCH_ToAddList->list.size = elementCnt * sizeof(TCI_StateId_t *);
      DU_ALLOC(controlRset->tci_StatesPDCCH_ToAddList->list.array, \
            controlRset->tci_StatesPDCCH_ToAddList->list.size)
         if(!controlRset->tci_StatesPDCCH_ToAddList->list.array)
         {
            DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildControlRSetToAddModList");
            return RFAILED;
         }

      for(tciStateIdx = 0; tciStateIdx <elementCntl; tciStateIdx++)
      {
         DU_ALLOC(controlRset->tci_StatesPDCCH_ToAddList->list.array[tciStateIdx], sizeof(TCI_StateId_t));
         if(!controlRset->tci_StatesPDCCH_ToAddList->list.array[tciStateIdx])
         {
            DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildControlRSetToAddModList");
            return RFAILED;
         }
      }

      tciStateIdx = 0;
      /* TODO */
      *(controlRset->tci_StatesPDCCH_ToAddList->list.array[tciStateIdx]);

      DU_ALLOC(controlRset->tci_PresentInDCI, sizeof(long));
      if(!controlRset->tci_PresentInDCI)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildControlRSetToAddModList");
         return RFAILED;
      }
      /* TODO */
      *(controlRset->tci_PresentInDCI);
#endif

      controlRSet->pdcch_DMRS_ScramblingID = NULLP;
      DU_ALLOC(controlRSet->pdcch_DMRS_ScramblingID, sizeof(long));
      if(!controlRSet->pdcch_DMRS_ScramblingID)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildControlRSetToAddModList");
         return RFAILED;
      }
      if(pdcchCfg == NULLP)
         *(controlRSet->pdcch_DMRS_ScramblingID) = SCRAMBLING_ID;
      else
         *(controlRSet->pdcch_DMRS_ScramblingID) = pdcchCfg->cRSetToAddModList[idx].dmrsScramblingId;
   }
   return ROK;
} /* End BuildControlRSetToAddModList */

/*******************************************************************
 *
 * @brief build SlotOffset for SearchSpace
 *
 * @details
 *
 *    Function : BuildSearchSpcSlotOffset
 *
 *    Functionality: Build Slot Offset for search space to add/modify list
 *
 * @params[in] SearchSpace__monitoringSlotPeriodicityAndOffset *mSlotPeriodicityAndOffset
 *             uint16_t slotOffset
 * @return void
 *
 * ****************************************************************/
void BuildSearchSpcSlotOffset(struct SearchSpace__monitoringSlotPeriodicityAndOffset *mSlotPeriodicityAndOffset,  uint16_t slotOffset)
{
   switch(mSlotPeriodicityAndOffset->present)
   {
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl1:
         mSlotPeriodicityAndOffset->choice.sl1 = slotOffset;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl2:
         mSlotPeriodicityAndOffset->choice.sl2 = slotOffset;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl4:
         mSlotPeriodicityAndOffset->choice.sl4 = slotOffset;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl5:
         mSlotPeriodicityAndOffset->choice.sl5 = slotOffset;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl8:
         mSlotPeriodicityAndOffset->choice.sl8 = slotOffset;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl10:
         mSlotPeriodicityAndOffset->choice.sl10 = slotOffset;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl16:
         mSlotPeriodicityAndOffset->choice.sl16 = slotOffset;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl20:
         mSlotPeriodicityAndOffset->choice.sl20 = slotOffset;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl40:
         mSlotPeriodicityAndOffset->choice.sl40 = slotOffset;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl80:
         mSlotPeriodicityAndOffset->choice.sl80 = slotOffset;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl160:
         mSlotPeriodicityAndOffset->choice.sl160 = slotOffset;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl320:
         mSlotPeriodicityAndOffset->choice.sl320 = slotOffset;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl640:
         mSlotPeriodicityAndOffset->choice.sl640 = slotOffset;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl1280:
         mSlotPeriodicityAndOffset->choice.sl1280 = slotOffset;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl2560:
         mSlotPeriodicityAndOffset->choice.sl2560 = slotOffset;
         break;
      default:
         break;
   }
}


/*******************************************************************
 *
 * @brief Build search space to add/modify list
 *
 * @details
 *
 *    Function : BuildSearchSpcToAddModList
 *
 *    Functionality: Build search space to add/modify list
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSearchSpcToAddModList(PdcchConfig *pdcchCfg, struct PDCCH_Config__searchSpacesToAddModList *searchSpcList)
{
   uint8_t idx;
   uint8_t numBytes;
   uint8_t byteIdx;
   uint8_t bitsUnused;
   uint8_t elementCnt;
   struct SearchSpace *searchSpc;

   if(pdcchCfg == NULLP)
      elementCnt = 1;
   else
      elementCnt = pdcchCfg->numSearchSpcToAddMod;

   searchSpcList->list.count = elementCnt;
   searchSpcList->list.size = elementCnt * sizeof(struct SearchSpace *);

   searchSpcList->list.array = NULLP;
   DU_ALLOC(searchSpcList->list.array, searchSpcList->list.size);
   if(!searchSpcList->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
      searchSpcList->list.array[idx] = NULLP;
      DU_ALLOC(searchSpcList->list.array[idx], sizeof(struct SearchSpace));
      if(!searchSpcList->list.array[idx])
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSearchSpcToAddModList");
         return RFAILED;
      }
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
      searchSpc = searchSpcList->list.array[idx];

      if(pdcchCfg == NULLP)
         searchSpc->searchSpaceId = PDCCH_SRCH_SPC_TWO_ID;
      else
         searchSpc->searchSpaceId = pdcchCfg->searchSpcToAddModList[idx].searchSpaceId;

      searchSpc->controlResourceSetId = NULLP;
      DU_ALLOC(searchSpc->controlResourceSetId, sizeof(ControlResourceSetId_t));
      if(!searchSpc->controlResourceSetId)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSearchSpcToAddModList");
         return RFAILED;
      }
      if(pdcchCfg == NULLP)
         *(searchSpc->controlResourceSetId) = PDCCH_CTRL_RSRC_SET_ONE_ID;
      else
         *(searchSpc->controlResourceSetId) = pdcchCfg->searchSpcToAddModList[idx].cRSetId;

      searchSpc->monitoringSlotPeriodicityAndOffset = NULLP;
      DU_ALLOC(searchSpc->monitoringSlotPeriodicityAndOffset, sizeof(struct SearchSpace__monitoringSlotPeriodicityAndOffset));
      if(!searchSpc->monitoringSlotPeriodicityAndOffset)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSearchSpcToAddModList");
         return RFAILED;
      }
      if(pdcchCfg == NULLP)
         searchSpc->monitoringSlotPeriodicityAndOffset->present = SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl1;
      else
      {
         searchSpc->monitoringSlotPeriodicityAndOffset->present = \
                     pdcchCfg->searchSpcToAddModList[idx].mSlotPeriodicityAndOffset.mSlotPeriodicity;
         BuildSearchSpcSlotOffset(searchSpc->monitoringSlotPeriodicityAndOffset, \
                     pdcchCfg->searchSpcToAddModList[idx].mSlotPeriodicityAndOffset.mSlotOffset);
      }

      searchSpc->duration = NULLP;
      searchSpc->monitoringSymbolsWithinSlot = NULLP;
      DU_ALLOC(searchSpc->monitoringSymbolsWithinSlot, sizeof(BIT_STRING_t));
      if(!searchSpc->monitoringSymbolsWithinSlot)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSearchSpcToAddModList");
         return RFAILED;
      }

      /* Values taken from reference logs :
       * size 2 bytes
       * 2 LSBs unsued
       * Bit string stores 8000
       */
      numBytes = 2;
      bitsUnused = 2;
      searchSpc->monitoringSymbolsWithinSlot->size = numBytes * sizeof(uint8_t);
      searchSpc->monitoringSymbolsWithinSlot->buf = NULLP;
      DU_ALLOC(searchSpc->monitoringSymbolsWithinSlot->buf, searchSpc->monitoringSymbolsWithinSlot->size);
      if(!searchSpc->monitoringSymbolsWithinSlot->buf)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSearchSpcToAddModList");
         return RFAILED;
      }
      if(pdcchCfg == NULLP)
      {
         byteIdx = 0;
         searchSpc->monitoringSymbolsWithinSlot->buf[byteIdx++] = PDCCH_SYMBOL_WITHIN_SLOT /* setting MSB to 128 i.e. 0x80 */;
         searchSpc->monitoringSymbolsWithinSlot->buf[byteIdx++] = 0;
      }
      else
         memcpy(searchSpc->monitoringSymbolsWithinSlot->buf, pdcchCfg->searchSpcToAddModList[idx].mSymbolsWithinSlot, numBytes);
      searchSpc->monitoringSymbolsWithinSlot->bits_unused = bitsUnused;

      searchSpc->nrofCandidates = NULLP;
      DU_ALLOC(searchSpc->nrofCandidates, sizeof(struct SearchSpace__nrofCandidates));
      if(!searchSpc->nrofCandidates)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSearchSpcToAddModList");
         return RFAILED;
      }

      if(pdcchCfg == NULLP)
      {
         searchSpc->nrofCandidates->aggregationLevel1 = PDCCH_SRCH_SPC_TWO_AGG_LVL1_CANDIDATE;
         searchSpc->nrofCandidates->aggregationLevel2 = PDCCH_SRCH_SPC_TWO_AGG_LVL2_CANDIDATE;
         searchSpc->nrofCandidates->aggregationLevel4 = PDCCH_SRCH_SPC_TWO_AGG_LVL4_CANDIDATE;
         searchSpc->nrofCandidates->aggregationLevel8 = PDCCH_SRCH_SPC_TWO_AGG_LVL8_CANDIDATE;
         searchSpc->nrofCandidates->aggregationLevel16 = PDCCH_SRCH_SPC_TWO_AGG_LVL16_CANDIDATE;
      }
      else
      {
         searchSpc->nrofCandidates->aggregationLevel1 = pdcchCfg->searchSpcToAddModList[idx].numCandidatesAggLevel1;
         searchSpc->nrofCandidates->aggregationLevel2 = pdcchCfg->searchSpcToAddModList[idx].numCandidatesAggLevel2;
         searchSpc->nrofCandidates->aggregationLevel4 = pdcchCfg->searchSpcToAddModList[idx].numCandidatesAggLevel4;
         searchSpc->nrofCandidates->aggregationLevel8 = pdcchCfg->searchSpcToAddModList[idx].numCandidatesAggLevel8;
         searchSpc->nrofCandidates->aggregationLevel16 = pdcchCfg->searchSpcToAddModList[idx].numCandidatesAggLevel16;
      }

      searchSpc->searchSpaceType = NULLP;
      DU_ALLOC(searchSpc->searchSpaceType, sizeof(struct SearchSpace__searchSpaceType));
      if(!searchSpc->searchSpaceType)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSearchSpcToAddModList");
         return RFAILED;
      }
      if(pdcchCfg == NULLP)
         searchSpc->searchSpaceType->present = SearchSpace__searchSpaceType_PR_ue_Specific;
      else
         searchSpc->searchSpaceType->present = pdcchCfg->searchSpcToAddModList[idx].searchSpaceType;

      searchSpc->searchSpaceType->choice.ue_Specific = NULLP;
      DU_ALLOC(searchSpc->searchSpaceType->choice.ue_Specific, sizeof(struct SearchSpace__searchSpaceType__ue_Specific));
      if(!searchSpc->searchSpaceType->choice.ue_Specific)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSearchSpcToAddModList");
         return RFAILED;
      }  
      if(pdcchCfg == NULLP)
         searchSpc->searchSpaceType->choice.ue_Specific->dci_Formats = PDCCH_SRCH_SPC_TWO_UE_SPEC_DCI_FORMAT;
      else
         searchSpc->searchSpaceType->choice.ue_Specific->dci_Formats = pdcchCfg->searchSpcToAddModList[idx].ueSpecificDciFormat;
   }
   return ROK;
}/* End BuildSearchSpcToAddModList */

/*******************************************************************
 *
 * @brief Builds BWP DL dedicated PDCCH config
 *
 * @details
 *
 *    Function : BuildBWPDlDedPdcchCfg
 *
 *    Functionality: Builds BWP DL dedicated PDCCH config
 *
 * @params[in] struct PDCCH_Config *pdcchCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildBWPDlDedPdcchCfg(PdcchConfig *pdcchCfgDb, struct PDCCH_Config *pdcchCfg)
{
   pdcchCfg->controlResourceSetToAddModList = NULLP;
   DU_ALLOC(pdcchCfg->controlResourceSetToAddModList, sizeof(struct PDCCH_Config__controlResourceSetToAddModList));
   if(!pdcchCfg->controlResourceSetToAddModList)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPDlDedPdcchCfg");
      return RFAILED;
   }

   if(BuildControlRSetToAddModList(pdcchCfgDb, pdcchCfg->controlResourceSetToAddModList) != ROK)
   {
      DU_LOG("\nERROR  --> F1AP : Failed in BuildControlRSetToAddModList()");
      return RFAILED;
   }

   pdcchCfg->controlResourceSetToReleaseList = NULLP;

   pdcchCfg->searchSpacesToAddModList = NULLP;
   DU_ALLOC(pdcchCfg->searchSpacesToAddModList, sizeof(struct PDCCH_Config__searchSpacesToAddModList));
   if(!pdcchCfg->searchSpacesToAddModList)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPDlDedPdcchCfg");
      return RFAILED;
   }

   if(BuildSearchSpcToAddModList(pdcchCfgDb, pdcchCfg->searchSpacesToAddModList) != ROK)
   {
      DU_LOG("\nERROR  --> F1AP : Failed in BuildSearchSpcToAddModList()");
      return RFAILED;
   }

   pdcchCfg->searchSpacesToReleaseList = NULLP;
   pdcchCfg->downlinkPreemption = NULLP;
   pdcchCfg->tpc_PUSCH = NULLP;
   pdcchCfg->tpc_PUCCH = NULLP;
   pdcchCfg->tpc_SRS = NULLP;

   return ROK;
}

/*******************************************************************
 * @brief Builds DMRS DL PDSCH Mapping type A
 *
 * @details
 *
 *    Function : BuildDMRSDLPdschMapTypeA
 *
 *    Functionality: Builds DMRS DL PDSCH Mapping type A
 *
 * @params[in]
 * struct PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA *dmrsDlCfg
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildDMRSDLPdschMapTypeA(PdschConfig *pdschCfg, struct PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA *dmrsDlCfg)
{
   dmrsDlCfg->present = PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA_PR_setup;
   dmrsDlCfg->choice.setup = NULLP;
   DU_ALLOC(dmrsDlCfg->choice.setup, sizeof(struct DMRS_DownlinkConfig));
   if(!dmrsDlCfg->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDMRSDLPdschMapTypeA");
      return RFAILED;
   }

   dmrsDlCfg->choice.setup->dmrs_Type = NULLP;
   dmrsDlCfg->choice.setup->dmrs_AdditionalPosition = NULLP;
   DU_ALLOC(dmrsDlCfg->choice.setup->dmrs_AdditionalPosition, sizeof(long));
   if(!dmrsDlCfg->choice.setup->dmrs_AdditionalPosition)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDMRSDLPdschMapTypeA");
      return RFAILED;
   }
   if(pdschCfg == NULLP)
      *(dmrsDlCfg->choice.setup->dmrs_AdditionalPosition) = DMRS_ADDITIONAL_POS;
   else
      *(dmrsDlCfg->choice.setup->dmrs_AdditionalPosition) = pdschCfg->dmrsDlCfgForPdschMapTypeA.addPos;

   dmrsDlCfg->choice.setup->maxLength = NULLP;
   dmrsDlCfg->choice.setup->scramblingID0 = NULLP;
   dmrsDlCfg->choice.setup->scramblingID1 = NULLP;
   dmrsDlCfg->choice.setup->phaseTrackingRS = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds TCI states to add/modify list
 *
 * @details
 *
 *    Function : BuildTCIStatesToAddModList
 *
 *    Functionality:Builds TCI states to add/modify list
 *
 * @params[in] 
 * struct PDSCH_Config__tci_StatesToAddModList *tciStatesList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildTCIStatesToAddModList(struct PDSCH_Config__tci_StatesToAddModList *tciStatesList)
{
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds PDSCH time domain allocation list
 *
 * @details
 *
 *    Function : BuildPdschTimeDomAllocList
 *
 *    Functionality: Builds PDSCH time domain allocation list
 *
 * @params[in] 
 * struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAllocList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPdschTimeDomAllocList(PdschConfig *pdschCfg, struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAllocList)
{
   uint8_t idx;
   uint8_t elementCnt;
   struct PDSCH_TimeDomainResourceAllocation *timeDomAlloc;

   timeDomAllocList->present = PDSCH_Config__pdsch_TimeDomainAllocationList_PR_setup;

   timeDomAllocList->choice.setup = NULLP;
   DU_ALLOC(timeDomAllocList->choice.setup, sizeof(struct PDSCH_TimeDomainResourceAllocationList));
   if(!timeDomAllocList->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschTimeDomAllocList");
      return RFAILED;
   }

if(pdschCfg == NULLP)
   elementCnt = 2;
else
elementCnt = pdschCfg->numTimeDomRsrcAlloc;
   timeDomAllocList->choice.setup->list.count = elementCnt;
   timeDomAllocList->choice.setup->list.size = elementCnt * sizeof(struct PDSCH_TimeDomainResourceAllocation *);

   timeDomAllocList->choice.setup->list.array = NULLP;
   DU_ALLOC(timeDomAllocList->choice.setup->list.array, timeDomAllocList->choice.setup->list.size);
   if(!timeDomAllocList->choice.setup->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschTimeDomAllocList");
      return RFAILED;
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
      timeDomAllocList->choice.setup->list.array[idx] = NULLP;
      DU_ALLOC(timeDomAllocList->choice.setup->list.array[idx], \
            sizeof(struct PDSCH_TimeDomainResourceAllocation));
      if(!timeDomAllocList->choice.setup->list.array[idx])
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschTimeDomAllocList");
         return RFAILED;
      }
   }

   if(pdschCfg == NULLP)
   {
      idx = 0;
      timeDomAlloc = timeDomAllocList->choice.setup->list.array[idx];
      DU_ALLOC(timeDomAlloc->k0, sizeof(long));
      if(!timeDomAlloc->k0)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschTimeDomAllocList");
         return RFAILED;
      }
      *(timeDomAlloc->k0) = 0;
      timeDomAlloc->mappingType = PDSCH_MAPPING_TYPE_A;
      timeDomAlloc->startSymbolAndLength = \
                                           calcSliv(PDSCH_START_SYMBOL, PDSCH_LENGTH_SYMBOL);

      idx++;
      timeDomAlloc = timeDomAllocList->choice.setup->list.array[idx];
      DU_ALLOC(timeDomAlloc->k0, sizeof(long));
      if(!timeDomAlloc->k0)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschTimeDomAllocList");
         return RFAILED;
      }
      *(timeDomAlloc->k0) = 1;
      timeDomAlloc->mappingType = PDSCH_MAPPING_TYPE_A;
      timeDomAlloc->startSymbolAndLength = calcSliv(PDSCH_START_SYMBOL, PDSCH_LENGTH_SYMBOL);
   }
   else
   {
      for(idx = 0; idx < elementCnt; idx++)
      {
         timeDomAlloc = timeDomAllocList->choice.setup->list.array[idx];
         if(pdschCfg->timeDomRsrcAllociList[idx].k0)
         {
            DU_ALLOC(timeDomAlloc->k0, sizeof(long));
            if(!timeDomAlloc->k0)
            {
               DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschTimeDomAllocList");
               return RFAILED;
            }
            *(timeDomAlloc->k0) = *(pdschCfg->timeDomRsrcAllociList[idx].k0);
         }
         timeDomAlloc->mappingType = pdschCfg->timeDomRsrcAllociList[idx].mappingType;
         timeDomAlloc->startSymbolAndLength = pdschCfg->timeDomRsrcAllociList[idx].startSymbolAndLength;
      }
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds PDSCH PRB Bundling type
 *
 * @details
 *
 *    Function : BuildPdschPrbBundlingType
 *
 *    Functionality: Builds PDSCH PRB Bundling type
 *
 * @params[in] 
 * struct PDSCH_Config__prb_BundlingType *prbBndlType
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPdschPrbBundlingType(PdschConfig *pdschCfg, struct PDSCH_Config__prb_BundlingType *prbBndlType)
{
   if(pdschCfg == NULLP)
      prbBndlType->present = PDSCH_Config__prb_BundlingType_PR_staticBundling;
   else
      prbBndlType->present = pdschCfg->bundlingType;

   prbBndlType->choice.staticBundling = NULLP;
   DU_ALLOC(prbBndlType->choice.staticBundling, \
	 sizeof(struct PDSCH_Config__prb_BundlingType__staticBundling));
   if(!prbBndlType->choice.staticBundling)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschPrbBundlingType");
      return RFAILED;
   }
   prbBndlType->choice.staticBundling->bundleSize = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds BWP DL dedicated PDSCH config 
 *
 * @details
 *
 *    Function : BuildBWPDlDedPdschCfg
 *
 *    Functionality: Builds BWP DL dedicated PDSCH config
 *
 * @params[in] struct PDSCH_Config *pdschCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildBWPDlDedPdschCfg(PdschConfig *pdschCfgDb, struct PDSCH_Config *pdschCfg)
{
   pdschCfg->dataScramblingIdentityPDSCH = NULLP;

   pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA = NULLP;
   DU_ALLOC(pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA, sizeof(struct PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA));
   if(!pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPDlDedPdschCfg");
      return RFAILED;
   }

   if(BuildDMRSDLPdschMapTypeA(pdschCfgDb, pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA) != ROK)
   {
      return RFAILED;
   }

   pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeB = NULLP;
   pdschCfg->tci_StatesToAddModList = NULLP;
   pdschCfg->tci_StatesToReleaseList = NULLP;
   pdschCfg->vrb_ToPRB_Interleaver = NULLP;
#if 0
   DU_ALLOC(pdschCfg->tci_StatesToAddModList, sizeof(struct PDSCH_Config__tci_StatesToAddModList));
   if(!pdschCfg->tci_StatesToAddModList)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPDlDedPdschCfg");
      return RFAILED;
   }
   if(BuildTCIStatesToAddModList(pdschCfg->tci_StatesToAddModList) != ROK)
   {
      return RFAILED;
   }
#endif

if(pdschCfgDb == NULLP)
   pdschCfg->resourceAllocation = RES_ALLOC_TYPE;
else
pdschCfg->resourceAllocation = pdschCfgDb->resourceAllocType;

   pdschCfg->pdsch_TimeDomainAllocationList = NULLP;
   DU_ALLOC(pdschCfg->pdsch_TimeDomainAllocationList, sizeof(struct PDSCH_Config__pdsch_TimeDomainAllocationList));
   if(!pdschCfg->pdsch_TimeDomainAllocationList)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPDlDedPdschCfg");
      return RFAILED;
   }
   if(BuildPdschTimeDomAllocList(pdschCfgDb, pdschCfg->pdsch_TimeDomainAllocationList) != ROK)
   {
      return RFAILED;
   }

   pdschCfg->pdsch_AggregationFactor = NULLP;
   pdschCfg->rateMatchPatternToAddModList = NULLP;
   pdschCfg->rateMatchPatternToReleaseList = NULLP;
   pdschCfg->rateMatchPatternGroup1 = NULLP;
   pdschCfg->rateMatchPatternGroup2 = NULLP;
   if(pdschCfgDb == NULLP)
      pdschCfg->rbg_Size = PDSCH_RBG_SIZE;
   else
      pdschCfg->rbg_Size = pdschCfgDb->rbgSize;
   pdschCfg->mcs_Table = NULLP;

   pdschCfg->maxNrofCodeWordsScheduledByDCI = NULLP;
   DU_ALLOC(pdschCfg->maxNrofCodeWordsScheduledByDCI, sizeof(long));
   if(!pdschCfg->maxNrofCodeWordsScheduledByDCI)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPDlDedPdschCfg");
      return RFAILED;
   }
   if(pdschCfgDb == NULLP)
      *(pdschCfg->maxNrofCodeWordsScheduledByDCI) = PDSCH_MAX_CODEWORD_SCH_BY_DCI;
   else
      *(pdschCfg->maxNrofCodeWordsScheduledByDCI) = pdschCfgDb->numCodeWordsSchByDci;

   if(BuildPdschPrbBundlingType(pdschCfgDb, &pdschCfg->prb_BundlingType) != ROK)
   {
      return RFAILED;
   }

   pdschCfg->zp_CSI_RS_ResourceToAddModList = NULLP;
   pdschCfg->zp_CSI_RS_ResourceToReleaseList = NULLP;
   pdschCfg->aperiodic_ZP_CSI_RS_ResourceSetsToAddModList = NULLP;
   pdschCfg->aperiodic_ZP_CSI_RS_ResourceSetsToReleaseList = NULLP;
   pdschCfg->sp_ZP_CSI_RS_ResourceSetsToAddModList = NULLP;
   pdschCfg->sp_ZP_CSI_RS_ResourceSetsToReleaseList = NULLP;
   pdschCfg->p_ZP_CSI_RS_ResourceSet = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds intitial DL BWP
 * @details
 *
 *    Function : BuildInitialDlBWP 
 *
 *    Functionality: Builds intitial DL BWP in spCellCfgDed
 *
 * @params[in] BWP_DownlinkDedicated_t *dlBwp
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildInitialDlBWP(InitialDlBwp *initiDlBwp, BWP_DownlinkDedicated_t *dlBwp)
{
   PdcchConfig *pdcchCfg = NULLP;
   PdschConfig *pdschCfg = NULLP;

   if(initiDlBwp)
   {
      if(initiDlBwp->pdcchPresent)
         pdcchCfg = &initiDlBwp->pdcchCfg;
      if(initiDlBwp->pdschPresent)
         pdschCfg = &initiDlBwp->pdschCfg;
   }

   dlBwp->pdcch_Config = NULLP;
   DU_ALLOC(dlBwp->pdcch_Config, sizeof(struct BWP_DownlinkDedicated__pdcch_Config));
   if(!dlBwp->pdcch_Config)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory Allocation failure in BuildInitialDlBWP");
      return RFAILED;
   }
   dlBwp->pdcch_Config->present = BWP_DownlinkDedicated__pdcch_Config_PR_setup; 

   dlBwp->pdcch_Config->choice.setup = NULLP;
   DU_ALLOC(dlBwp->pdcch_Config->choice.setup, sizeof(struct PDCCH_Config));
   if(!dlBwp->pdcch_Config->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory Allocation failure in BuildInitialDlBWP");
      return RFAILED;
   }
   if(BuildBWPDlDedPdcchCfg(pdcchCfg, dlBwp->pdcch_Config->choice.setup) != ROK)
   {
      return RFAILED;
   }

   dlBwp->pdsch_Config = NULLP;
   DU_ALLOC(dlBwp->pdsch_Config, sizeof(struct BWP_DownlinkDedicated__pdsch_Config));
   if(!dlBwp->pdsch_Config)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory Allocation failure in BuildInitialDlBWP");
      return RFAILED;
   }
   dlBwp->pdsch_Config->present = BWP_DownlinkDedicated__pdsch_Config_PR_setup;

   dlBwp->pdsch_Config->choice.setup = NULLP;
   DU_ALLOC(dlBwp->pdsch_Config->choice.setup, sizeof(struct PDSCH_Config));
   if(!dlBwp->pdsch_Config->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory Allocation failure in BuildInitialDlBWP");
      return RFAILED;
   }

   if(BuildBWPDlDedPdschCfg(pdschCfg, dlBwp->pdsch_Config->choice.setup) != ROK)
   {
      return RFAILED;
   }

   dlBwp->sps_Config = NULLP;
   dlBwp->radioLinkMonitoringConfig = NULLP; 
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds DMRS UL Pusch Mapping type A
 *
 * @details
 *
 *    Function : BuildDMRSULPuschMapTypeA
 *
 *    Functionality: Builds DMRS UL Pusch Mapping type A
 *
 * @params[in] 
 *    struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA *dmrsUlCfg
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildDMRSULPuschMapTypeA(DmrsUlCfg *ulDmrsCfgDb, struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA *dmrsUlCfg)
{
   dmrsUlCfg->present = PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA_PR_setup;
   dmrsUlCfg->choice.setup= NULLP;
   DU_ALLOC(dmrsUlCfg->choice.setup, sizeof(DMRS_UplinkConfig_t));
   if(!dmrsUlCfg->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDMRSULPuschMapTypeA");
      return RFAILED;
   }

   dmrsUlCfg->choice.setup->dmrs_Type = NULLP;
   dmrsUlCfg->choice.setup->dmrs_AdditionalPosition = NULLP;
   DU_ALLOC(dmrsUlCfg->choice.setup->dmrs_AdditionalPosition, sizeof(long));
   if(!dmrsUlCfg->choice.setup->dmrs_AdditionalPosition)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDMRSULPuschMapTypeA");
      return RFAILED;
   }
   if(ulDmrsCfgDb == NULLP)
      *(dmrsUlCfg->choice.setup->dmrs_AdditionalPosition) = DMRS_ADDITIONAL_POS; 
   else
      *(dmrsUlCfg->choice.setup->dmrs_AdditionalPosition) = ulDmrsCfgDb->addPos;

   dmrsUlCfg->choice.setup->phaseTrackingRS = NULLP;
   dmrsUlCfg->choice.setup->maxLength = NULLP;
   dmrsUlCfg->choice.setup->transformPrecodingDisabled = NULLP;
   DU_ALLOC(dmrsUlCfg->choice.setup->transformPrecodingDisabled, sizeof(struct DMRS_UplinkConfig__transformPrecodingDisabled));
   if(!dmrsUlCfg->choice.setup->transformPrecodingDisabled)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDMRSULPuschMapTypeA");
      return RFAILED;
   }

   dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0 = NULLP;
   DU_ALLOC(dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0,\
	 sizeof(long));
   if(!dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDMRSULPuschMapTypeA");
      return RFAILED;
   }
   if(ulDmrsCfgDb == NULLP)
      *(dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0) = SCRAMBLING_ID;
   else
      *(dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0) = ulDmrsCfgDb->transPrecodDisabled.scramblingId0;

   dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID1 = NULLP;
   dmrsUlCfg->choice.setup->transformPrecodingEnabled = NULLP;
   return ROK;
}

/*******************************************************************
 *
 * @brief Build PUSCH time domain allocation list
 *
 * @details
 *
 *    Function : BuildPuschTimeDomAllocList
 *
 *    Functionality: Build PUSCH time domain allocation list
 *
 * @params[in] 
 * struct PUSCH_Config__pusch_TimeDomainAllocationList *timeDomAllocList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPuschTimeDomAllocList(PuschCfg *puschCfgDb, struct PUSCH_Config__pusch_TimeDomainAllocationList *timeDomAllocList)
{
   uint8_t idx;
   uint8_t elementCnt;
   PUSCH_TimeDomainResourceAllocation_t  *timeDomAlloc;

   timeDomAllocList->present = PUSCH_Config__pusch_TimeDomainAllocationList_PR_setup;
   timeDomAllocList->choice.setup = NULLP;
   DU_ALLOC(timeDomAllocList->choice.setup, sizeof(struct PUSCH_TimeDomainResourceAllocationList));
   if(!timeDomAllocList->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPuschTimeDomAllocList");
      return RFAILED;
   }

   if(puschCfgDb == NULLP)
      elementCnt = 2;
   else
      elementCnt = puschCfgDb->numTimeDomRsrcAlloc;

   timeDomAllocList->choice.setup->list.count = elementCnt;
   timeDomAllocList->choice.setup->list.size = elementCnt * sizeof(PUSCH_TimeDomainResourceAllocation_t *);
   timeDomAllocList->choice.setup->list.array = NULLP;
   DU_ALLOC(timeDomAllocList->choice.setup->list.array, timeDomAllocList->choice.setup->list.size);
   if(!timeDomAllocList->choice.setup->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPuschTimeDomAllocList");
      return RFAILED;
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
      timeDomAllocList->choice.setup->list.array[idx] = NULLP;
      DU_ALLOC(timeDomAllocList->choice.setup->list.array[idx], sizeof(PUSCH_TimeDomainResourceAllocation_t));
      if(!timeDomAllocList->choice.setup->list.array[idx])
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPuschTimeDomAllocList");
         return RFAILED;
      }
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
      timeDomAlloc = timeDomAllocList->choice.setup->list.array[idx];
      DU_ALLOC(timeDomAlloc->k2, sizeof(long));
      if(!timeDomAlloc->k2)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPuschTimeDomAllocList");
         return RFAILED;
      }
      if(puschCfgDb == NULLP)
      {
         if(idx == 0)
            *(timeDomAlloc->k2) = PUSCH_K2_CFG1;
         else if(idx == 1)
            *(timeDomAlloc->k2) = PUSCH_K2_CFG2;

         timeDomAlloc->mappingType = PUSCH_MAPPING_TYPE_A;
         timeDomAlloc->startSymbolAndLength = calcSliv(PUSCH_START_SYMBOL, PUSCH_LENGTH_SYMBOL);
      }
      else
      {
         *(timeDomAlloc->k2) = puschCfgDb->timeDomRsrcAllocList[idx].k2;
         timeDomAlloc->mappingType = puschCfgDb->timeDomRsrcAllocList[idx].mappingType;
         timeDomAlloc->startSymbolAndLength = puschCfgDb->timeDomRsrcAllocList[idx].startSymbolAndLength;
      }
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds BWP UL dedicated PUSCH Config
 *
 * @details
 *
 *    Function : BuildBWPUlDedPuschCfg
 *
 *    Functionality:
 *      Builds BWP UL dedicated PUSCH Config
 *
 * @params[in] : PUSCH_Config_t *puschCfg
 *    
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildBWPUlDedPuschCfg(PuschCfg *puschCfgDb, PUSCH_Config_t *puschCfg)
{
   DmrsUlCfg *ulDmrsCfg = NULLP;
   
   if(puschCfgDb)
      ulDmrsCfg = &puschCfgDb->dmrsUlCfgForPuschMapTypeA;

   puschCfg->dataScramblingIdentityPUSCH = NULLP;
   DU_ALLOC(puschCfg->dataScramblingIdentityPUSCH, sizeof(long));
   if(!puschCfg->dataScramblingIdentityPUSCH)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPuschCfg");
      return RFAILED;
   }
   if(puschCfgDb == NULLP)
      *(puschCfg->dataScramblingIdentityPUSCH) = SCRAMBLING_ID;
   else
      *(puschCfg->dataScramblingIdentityPUSCH) = puschCfgDb->dataScramblingId;

   puschCfg->txConfig = NULLP;
   puschCfg->dmrs_UplinkForPUSCH_MappingTypeA = NULLP;
   DU_ALLOC(puschCfg->dmrs_UplinkForPUSCH_MappingTypeA, sizeof(struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA));
   if(!puschCfg->dmrs_UplinkForPUSCH_MappingTypeA)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPuschCfg");
      return RFAILED;
   }

   if(BuildDMRSULPuschMapTypeA(ulDmrsCfg, puschCfg->dmrs_UplinkForPUSCH_MappingTypeA) != ROK)
   {
      return RFAILED;
   }

   puschCfg->dmrs_UplinkForPUSCH_MappingTypeB = NULLP;
   puschCfg->pusch_PowerControl = NULLP;
   puschCfg->frequencyHopping = NULLP;
   puschCfg->frequencyHoppingOffsetLists = NULLP;

   if(puschCfgDb == NULLP)
      puschCfg->resourceAllocation = RES_ALLOC_TYPE;
   else
      puschCfg->resourceAllocation = puschCfgDb->resourceAllocType;

   puschCfg->pusch_TimeDomainAllocationList = NULLP;
   DU_ALLOC(puschCfg->pusch_TimeDomainAllocationList, sizeof(struct PUSCH_Config__pusch_TimeDomainAllocationList));
   if(!puschCfg->pusch_TimeDomainAllocationList)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPuschCfg");
      return RFAILED;
   }

   if(BuildPuschTimeDomAllocList(puschCfgDb, puschCfg->pusch_TimeDomainAllocationList) != ROK)
   {
      return RFAILED;
   }

   puschCfg->pusch_AggregationFactor = NULLP;
   puschCfg->mcs_Table = NULLP;
   puschCfg->mcs_TableTransformPrecoder = NULLP;
   puschCfg->transformPrecoder = NULLP;
   DU_ALLOC(puschCfg->transformPrecoder, sizeof(long));
   if(!puschCfg->transformPrecoder)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPuschCfg");
      return RFAILED;
   }
   if(puschCfgDb == NULLP)
      *(puschCfg->transformPrecoder) = PUSCH_TRANSFORM_PRECODER;
   else
      *(puschCfg->transformPrecoder) = puschCfgDb->transformPrecoder;

   puschCfg->codebookSubset = NULLP;
   puschCfg->maxRank = NULLP;
   puschCfg->rbg_Size = NULLP;
   puschCfg->uci_OnPUSCH = NULLP;
   puschCfg->tp_pi2BPSK = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds PUCCH resource set add/modify list
 *
 * @details
 *
 *    Function : BuildPucchRsrcSetAddModList
 *
 *    Functionality:
 *      Builds PUCCH resource set add/modify list
 *
 * @params[in] : PucchResrcSetCfg *rsrcSetCfgDb
 *               struct PUCCH_Config__resourceSetToAddModList *rsrcSetAddModList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPucchRsrcSetAddModList(PucchResrcSetCfg *rsrcSetCfgDb, \
   struct PUCCH_Config__resourceSetToAddModList *resourceSetToAddModList)
{
   uint8_t elementCnt = 0, rsrcSetIdx = 0, rsrcIdx = 0;
   PUCCH_ResourceSet_t *rsrcSet = NULLP;

   if(rsrcSetCfgDb == NULLP)
      elementCnt = 1;
   else
      elementCnt = rsrcSetCfgDb->resrcSetToAddModListCount;

   resourceSetToAddModList->list.count = elementCnt;
   resourceSetToAddModList->list.size = elementCnt * sizeof(PUCCH_ResourceSet_t *);
   DU_ALLOC(resourceSetToAddModList->list.array, resourceSetToAddModList->list.size);
   if(resourceSetToAddModList->list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchRsrcSetAddModList");
      return RFAILED;
   }
   for(rsrcSetIdx=0; rsrcSetIdx < resourceSetToAddModList->list.count; rsrcSetIdx++)
   {
      DU_ALLOC(resourceSetToAddModList->list.array[rsrcSetIdx], sizeof(PUCCH_ResourceSet_t));
      if(resourceSetToAddModList->list.array[rsrcSetIdx] == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchRsrcSetAddModList");
         return RFAILED;
      }
   }

   for(rsrcSetIdx=0; rsrcSetIdx < resourceSetToAddModList->list.count; rsrcSetIdx++)
   {
      rsrcSet = resourceSetToAddModList->list.array[rsrcSetIdx];

      /* Resource set Id */
      if(rsrcSetCfgDb == NULLP)
         rsrcSet->pucch_ResourceSetId = 1;
      else
         rsrcSet->pucch_ResourceSetId = rsrcSetCfgDb->resrcSetToAddModList[rsrcSetIdx].resrcSetId;
 
      /* Resource list of a resource set */
      if(rsrcSetCfgDb == NULLP)
         elementCnt = 1;
      else
         elementCnt = rsrcSetCfgDb->resrcSetToAddModList[rsrcSetIdx].resrcListCount;
      rsrcSet->resourceList.list.count = elementCnt;
      rsrcSet->resourceList.list.size = elementCnt * sizeof(PUCCH_ResourceId_t *);
      DU_ALLOC(rsrcSet->resourceList.list.array, rsrcSet->resourceList.list.size);
      if(rsrcSet->resourceList.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchRsrcSetAddModList");
         return RFAILED;
      }

      for(rsrcIdx=0; rsrcIdx < rsrcSet->resourceList.list.count; rsrcIdx++)
      {
         DU_ALLOC(rsrcSet->resourceList.list.array[rsrcIdx], sizeof(PUCCH_ResourceId_t));
         if(rsrcSet->resourceList.list.array[rsrcIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchRsrcSetAddModList");
            return RFAILED;
         }
      }
      for(rsrcIdx=0; rsrcIdx < rsrcSet->resourceList.list.count; rsrcIdx++)
      {
         if(rsrcSetCfgDb == NULLP)
            *(rsrcSet->resourceList.list.array[rsrcIdx]) = 1;
         else
            *(rsrcSet->resourceList.list.array[rsrcIdx]) = rsrcSetCfgDb->resrcSetToAddModList[rsrcSetIdx].resrcList[rsrcIdx];
      }

      /* Max payload size (minus 1) in a Resource set */
      rsrcSet->maxPayloadMinus1 = NULLP;
      if(rsrcSetCfgDb && rsrcSetCfgDb->resrcSetToAddModList[rsrcSetIdx].maxPayLoadSize)
      {
         DU_ALLOC(rsrcSet->maxPayloadMinus1, sizeof(long));
         if(rsrcSet->maxPayloadMinus1 == NULLP)
         {
            DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchRsrcSetAddModList");
            return RFAILED;
         }
         *(rsrcSet->maxPayloadMinus1) = rsrcSetCfgDb->resrcSetToAddModList[rsrcSetIdx].maxPayLoadSize;
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds PUCCH resource add/modify list
 *
 * @details
 *
 *    Function : BuildPucchRsrcAdddModList
 *
 *    Functionality:
 *      Builds PUCCH resource add/modify list
 *
 * @params[in] : PucchResrcCfg *rsrcSetCfgDb
 *               struct PUCCH_Config__resourceSetToAddModList *rsrcSetAddModList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPucchRsrcAddModList(PucchResrcCfg *rsrcCfgDb, struct PUCCH_Config__resourceToAddModList  *resourceToAddModList)
{
   uint8_t elementCnt = 0, rsrcIdx = 0;
   PUCCH_Resource_t *rsrc = NULLP;

   if(rsrcCfgDb == NULLP)
      elementCnt = 1;
   else
      elementCnt = rsrcCfgDb->resrcToAddModListCount;
   resourceToAddModList->list.count = elementCnt;
   resourceToAddModList->list.size = elementCnt * sizeof(PUCCH_Resource_t *);
   DU_ALLOC(resourceToAddModList->list.array, resourceToAddModList->list.size);
   if(resourceToAddModList->list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchRsrcAddModList");
      return RFAILED;
   }
   for(rsrcIdx=0; rsrcIdx < resourceToAddModList->list.count; rsrcIdx++)
   {
      DU_ALLOC(resourceToAddModList->list.array[rsrcIdx], sizeof(PUCCH_Resource_t));
      if(resourceToAddModList->list.array[rsrcIdx] == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchRsrcAddModList");
         return RFAILED;
      }
   }

   for(rsrcIdx=0; rsrcIdx < resourceToAddModList->list.count; rsrcIdx++)
   {
      rsrc = resourceToAddModList->list.array[rsrcIdx];

      if(rsrcCfgDb == NULLP)
      {
         rsrc->pucch_ResourceId = 1;
         rsrc->startingPRB = 0;
         rsrc->format.present = PUCCH_Resource__format_PR_format1; 
         DU_ALLOC(rsrc->format.choice.format1, sizeof(PUCCH_format1_t));
         if(rsrc->format.choice.format1 == NULLP)
         {
            DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchRsrcAddModList");
            return RFAILED;
         }  
         rsrc->format.choice.format1->initialCyclicShift = 0;
         rsrc->format.choice.format1->nrofSymbols = 4;
         rsrc->format.choice.format1->startingSymbolIndex = 0;
         rsrc->format.choice.format1->timeDomainOCC = 0;
      }
      else
      {
         rsrc->pucch_ResourceId = rsrcCfgDb->resrcToAddModList[rsrcIdx].resrcId;
         rsrc->startingPRB = rsrcCfgDb->resrcToAddModList[rsrcIdx].startPrb;
         if(rsrcCfgDb->resrcToAddModList[rsrcIdx].intraFreqHop)
         {
            DU_ALLOC(rsrc->intraSlotFrequencyHopping, sizeof(long));
            if(rsrc->intraSlotFrequencyHopping == NULLP)
            {
               DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchRsrcAddModList");
               return RFAILED;
            }
            *(rsrc->intraSlotFrequencyHopping) = rsrcCfgDb->resrcToAddModList[rsrcIdx].intraFreqHop;
         }
         else
            rsrc->intraSlotFrequencyHopping = NULLP;

         if(rsrcCfgDb->resrcToAddModList[rsrcIdx].secondPrbHop)
         {
            DU_ALLOC(rsrc->secondHopPRB, sizeof(PRB_Id_t));
            if(rsrc->secondHopPRB == NULLP)
            {
               DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchRsrcAddModList");
               return RFAILED;
            }
            *(rsrc->secondHopPRB) = rsrcCfgDb->resrcToAddModList[rsrcIdx].secondPrbHop;
         }
         else
            rsrc->secondHopPRB = NULLP;
         rsrc->format.present = covertPucchFormatIntEnumToRrcEnum(rsrcCfgDb->resrcToAddModList[rsrcIdx].pucchFormat); 

         switch(rsrc->format.present)
         {
            case PUCCH_Resource__format_PR_NOTHING:
               break;
            case PUCCH_Resource__format_PR_format0:
               {
                  DU_ALLOC(rsrc->format.choice.format0, sizeof(PUCCH_format0_t));
                  if(rsrc->format.choice.format0 == NULLP)
                  {
                     DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchRsrcAddModList");
                     return RFAILED;
                  }
                  rsrc->format.choice.format0->initialCyclicShift = rsrcCfgDb->resrcToAddModList[rsrcIdx].PucchFormat.format0->initialCyclicShift;
                  rsrc->format.choice.format0->nrofSymbols = rsrcCfgDb->resrcToAddModList[rsrcIdx].PucchFormat.format0->numSymbols;
                  rsrc->format.choice.format0->startingSymbolIndex = rsrcCfgDb->resrcToAddModList[rsrcIdx].PucchFormat.format0->startSymbolIdx;
                  break;
               }

            case PUCCH_Resource__format_PR_format1:
               {
                  DU_ALLOC(rsrc->format.choice.format1, sizeof(PUCCH_format1_t));
                  if(rsrc->format.choice.format1 == NULLP)
                  {
                     DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchRsrcAddModList");
                     return RFAILED;
                  }  
                  rsrc->format.choice.format1->initialCyclicShift = rsrcCfgDb->resrcToAddModList[rsrcIdx].PucchFormat.format1->initialCyclicShift;
                  rsrc->format.choice.format1->nrofSymbols = rsrcCfgDb->resrcToAddModList[rsrcIdx].PucchFormat.format1->numSymbols;
                  rsrc->format.choice.format1->startingSymbolIndex = rsrcCfgDb->resrcToAddModList[rsrcIdx].PucchFormat.format1->startSymbolIdx;
                  rsrc->format.choice.format1->timeDomainOCC = rsrcCfgDb->resrcToAddModList[rsrcIdx].PucchFormat.format1->timeDomOCC;
                  break;
               }

            case PUCCH_Resource__format_PR_format2:
               {
                  DU_ALLOC(rsrc->format.choice.format2, sizeof(PUCCH_format2_t));
                  if(rsrc->format.choice.format2 == NULLP)
                  {
                     DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchRsrcAddModList");
                     return RFAILED;
                  } 
                  rsrc->format.choice.format2->nrofPRBs = rsrcCfgDb->resrcToAddModList[rsrcIdx].PucchFormat.format2->numPrbs;
                  rsrc->format.choice.format2->nrofSymbols = rsrcCfgDb->resrcToAddModList[rsrcIdx].PucchFormat.format2->numSymbols;
                  rsrc->format.choice.format2->startingSymbolIndex = rsrcCfgDb->resrcToAddModList[rsrcIdx].PucchFormat.format2->startSymbolIdx;
                  break;
               }

            case PUCCH_Resource__format_PR_format3:
               {
                  DU_ALLOC(rsrc->format.choice.format3, sizeof(PUCCH_format3_t));
                  if(rsrc->format.choice.format3 == NULLP)
                  {
                     DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchRsrcAddModList");
                     return RFAILED;
                  }
                  rsrc->format.choice.format3->nrofPRBs = rsrcCfgDb->resrcToAddModList[rsrcIdx].PucchFormat.format3->numPrbs;
                  rsrc->format.choice.format3->nrofSymbols = rsrcCfgDb->resrcToAddModList[rsrcIdx].PucchFormat.format3->numSymbols;
                  rsrc->format.choice.format3->startingSymbolIndex = rsrcCfgDb->resrcToAddModList[rsrcIdx].PucchFormat.format3->startSymbolIdx;
                  break;
               }

            case PUCCH_Resource__format_PR_format4:
               {
                  DU_ALLOC(rsrc->format.choice.format4, sizeof(PUCCH_format4_t));
                  if(rsrc->format.choice.format4 == NULLP)
                  {
                     DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchRsrcAddModList");
                     return RFAILED;
                  }
                  rsrc->format.choice.format4->nrofSymbols = rsrcCfgDb->resrcToAddModList[rsrcIdx].PucchFormat.format4->numSymbols;
                  rsrc->format.choice.format4->occ_Length = rsrcCfgDb->resrcToAddModList[rsrcIdx].PucchFormat.format4->occLen;
                  rsrc->format.choice.format4->occ_Index = rsrcCfgDb->resrcToAddModList[rsrcIdx].PucchFormat.format4->occIdx;
                  rsrc->format.choice.format4->startingSymbolIndex = rsrcCfgDb->resrcToAddModList[rsrcIdx].PucchFormat.format4->startSymbolIdx;
                  break;
               }
         }
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds PUCCH format  config
 *
 * @details
 *
 *    Function : BuildPucchFormat
 *
 *    Functionality: Builds PUCCH format  config
 *
 * @params[in] : PucchFormatCfg *formatDb
 *               PUCCH_FormatConfig_t *format
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPucchFormat(PucchFormatCfg *formatDb, PUCCH_FormatConfig_t *format)
{
   /* Inter Slot Fequency hopping */
   format->interslotFrequencyHopping = NULLP;
   if((formatDb != NULLP) && (formatDb->interSlotFreqHop == true))
   {
      DU_ALLOC(format->interslotFrequencyHopping, sizeof(long));
      if(format->interslotFrequencyHopping)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchFormat");
         return RFAILED;
      }
      *(format->interslotFrequencyHopping) = PUCCH_FormatConfig__interslotFrequencyHopping_enabled;
   }

   /* Additional DMRS */
   format->additionalDMRS = NULLP;
   if((formatDb != NULLP) && (formatDb->addDmrs == true))
   {
      DU_ALLOC(format->additionalDMRS, sizeof(long));
      if(format->additionalDMRS)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchFormat");
         return RFAILED;
      }
      *(format->additionalDMRS) = PUCCH_FormatConfig__additionalDMRS_true;
   }

    /* Maximum code rate */
   format->maxCodeRate = NULLP;
   if((formatDb != NULLP) && (formatDb->maxCodeRate != 0))
   {
      DU_ALLOC(format->maxCodeRate, sizeof(long));
      if(format->maxCodeRate)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchFormat");
         return RFAILED;
      }  
      *(format->maxCodeRate) = formatDb->maxCodeRate;
   }
 
   /* Number of slots */
   format->nrofSlots = NULLP;
   if((formatDb == NULLP) || ((formatDb != NULLP) && (formatDb->numSlots != 0)))
   {
      DU_ALLOC(format->nrofSlots, sizeof(long));
      if(format->nrofSlots == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchFormat");
         return RFAILED;
      }
      if(formatDb == NULLP)
         *(format->nrofSlots) = PUCCH_FormatConfig__nrofSlots_n4;
      else
         *(format->nrofSlots) = formatDb->numSlots;
   }

   /* Pi2BPSK*/
   format->pi2BPSK = NULLP;
   if((formatDb != NULLP) && (formatDb->pi2BPSK == true))
   {
      DU_ALLOC(format->pi2BPSK, sizeof(long));
      if(format->pi2BPSK)
      {     
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchFormat");
         return RFAILED;
      }     
      *(format->pi2BPSK) = PUCCH_FormatConfig__pi2BPSK_enabled;
   }

   /* Simultaneous HARQ ACK and CSI */
   format->simultaneousHARQ_ACK_CSI = NULLP;
   if((formatDb != NULLP) && (formatDb->harqAckCSI == true))
   {
      DU_ALLOC(format->simultaneousHARQ_ACK_CSI, sizeof(long));
      if(format->simultaneousHARQ_ACK_CSI)
      {     
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPucchFormat");
         return RFAILED;
      }     
      *(format->simultaneousHARQ_ACK_CSI) = PUCCH_FormatConfig__simultaneousHARQ_ACK_CSI_true;
   }

   return ROK;
}


/*******************************************************************
 *
 * @brief Builds PUCCH scheduling request list
 *
 * @details
 *
 *    Function : BuildPucchSchReqAddModList
 *
 *    Functionality:
 *      Builds PUCCH scheduling request list
 *
 * @params[in] : PucchSchedReqCfg *schReqDb
 *               struct PUCCH_Config__schedulingRequestResourceToAddModList *schReqRsrcToAddModList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPucchSchReqAddModList(PucchSchedReqCfg *schReqDb, \
   struct PUCCH_Config__schedulingRequestResourceToAddModList *schReqRsrcToAddModList)
{
   uint8_t elementCnt = 0, schReqIdx = 0;
   SchedulingRequestResourceConfig_t *schReqRsrc;

   elementCnt = schReqDb->schedAddModListCount;
   schReqRsrcToAddModList->list.count = elementCnt;
   schReqRsrcToAddModList->list.size = elementCnt *sizeof(struct SchedulingRequestResourceConfig *);

   schReqRsrcToAddModList->list.array = NULLP;
   DU_ALLOC(schReqRsrcToAddModList->list.array, schReqRsrcToAddModList->list.size);
   if(schReqRsrcToAddModList->list.array == NULLP)
   {
      DU_LOG("\nERROR  --> DU APP: Memory allocation failed in BuildPucchSchReqAddModList");
      return RFAILED;
   }

   for(schReqIdx = 0; schReqIdx < schReqRsrcToAddModList->list.count; schReqIdx++)
   {
      DU_ALLOC(schReqRsrcToAddModList->list.array[schReqIdx], schReqRsrcToAddModList->list.size);
      if(schReqRsrcToAddModList->list.array[schReqIdx] == NULLP)
      {
         DU_LOG("\nERROR  --> DU APP: Memory allocation failed in BuildPucchSchReqAddModList");
         return RFAILED;
      }
   }

   for(schReqIdx = 0; schReqIdx < schReqRsrcToAddModList->list.count; schReqIdx++)
   {
      schReqRsrc = schReqRsrcToAddModList->list.array[schReqIdx];
      schReqRsrc->schedulingRequestResourceId = schReqDb->schedAddModList[schReqIdx].resrcId;
      schReqRsrc->schedulingRequestID = schReqDb->schedAddModList[schReqIdx].requestId;

      if(schReqDb->schedAddModList[schReqIdx].periodicity)
      {
         schReqRsrc->periodicityAndOffset = NULLP;
         DU_ALLOC(schReqRsrc->periodicityAndOffset, sizeof(struct SchedulingRequestResourceConfig__periodicityAndOffset));
         if(schReqRsrc->periodicityAndOffset == NULLP)
         {
            DU_LOG("\nERROR  --> DU APP: Memory allocation failed in BuildPucchSchReqAddModList");
            return RFAILED;
         }

         schReqRsrc->periodicityAndOffset->present = schReqDb->schedAddModList[schReqIdx].periodicity;
         switch(schReqRsrc->periodicityAndOffset->present)
         {
            case SchedulingRequestResourceConfig__periodicityAndOffset_PR_NOTHING:
               break;
            case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sym2:
               schReqRsrc->periodicityAndOffset->choice.sym2 = schReqDb->schedAddModList[schReqIdx].offset;
               break;
            case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sym6or7:
               schReqRsrc->periodicityAndOffset->choice.sym6or7 = schReqDb->schedAddModList[schReqIdx].offset;
               break;
            case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl1:
               schReqRsrc->periodicityAndOffset->choice.sl1 = schReqDb->schedAddModList[schReqIdx].offset;
               break;
            case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl2:
               schReqRsrc->periodicityAndOffset->choice.sl2 = schReqDb->schedAddModList[schReqIdx].offset;
               break;
            case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl4:
               schReqRsrc->periodicityAndOffset->choice.sl4 = schReqDb->schedAddModList[schReqIdx].offset;
               break;
            case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl5:
               schReqRsrc->periodicityAndOffset->choice.sl5 = schReqDb->schedAddModList[schReqIdx].offset;
               break;
            case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl8:
               schReqRsrc->periodicityAndOffset->choice.sl8 = schReqDb->schedAddModList[schReqIdx].offset;
               break;
            case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl10:
               schReqRsrc->periodicityAndOffset->choice.sl10 = schReqDb->schedAddModList[schReqIdx].offset;
               break;
            case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl16:
               schReqRsrc->periodicityAndOffset->choice.sl16 = schReqDb->schedAddModList[schReqIdx].offset;
               break;
            case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl20:
               schReqRsrc->periodicityAndOffset->choice.sl20 = schReqDb->schedAddModList[schReqIdx].offset;
               break;
            case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl40:
               schReqRsrc->periodicityAndOffset->choice.sl40 = schReqDb->schedAddModList[schReqIdx].offset;
               break;
            case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl80:
               schReqRsrc->periodicityAndOffset->choice.sl80 = schReqDb->schedAddModList[schReqIdx].offset;
               break;
            case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl160:
               schReqRsrc->periodicityAndOffset->choice.sl160 = schReqDb->schedAddModList[schReqIdx].offset;
               break;
            case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl320:
               schReqRsrc->periodicityAndOffset->choice.sl320 = schReqDb->schedAddModList[schReqIdx].offset;
               break;
            case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl640:
               schReqRsrc->periodicityAndOffset->choice.sl640 = schReqDb->schedAddModList[schReqIdx].offset;
               break;
         }
      }

      if(schReqDb->schedAddModList[schReqIdx].resrc)
      {
         schReqRsrc->resource = NULLP;
         DU_ALLOC(schReqRsrc->resource, sizeof(PUCCH_ResourceId_t));
         if(schReqRsrc->resource == NULLP)
         {
            DU_LOG("\nERROR  --> DU APP: Memory allocation failed in BuildPucchSchReqAddModList");
            return RFAILED;
         }
         *(schReqRsrc->resource) = schReqDb->schedAddModList[schReqIdx].resrc;

      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds PUCCH multi csi resource list
 *
 * @details
 *
 *    Function : BuildPucchMultiCsiRsrcList
 *
 *    Functionality:
 *      Builds PUCCH multi csi resource list
 *
 * @params[in] : PucchMultiCsiCfg *multiCsiDb
 *               struct PUCCH_Config__multi_CSI_PUCCH_ResourceList  *multiCsiRsrcList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPucchMultiCsiRsrcList(PucchMultiCsiCfg *multiCsiDb, struct PUCCH_Config__multi_CSI_PUCCH_ResourceList  *multiCsiRsrcList)
{
   uint8_t elementCnt = 0, rsrcIdx = 0;

   elementCnt = multiCsiDb->multiCsiResrcListCount;
   multiCsiRsrcList->list.count = elementCnt;
   multiCsiRsrcList->list.size = elementCnt * sizeof(PUCCH_ResourceId_t *);
   multiCsiRsrcList->list.array = NULLP;
   DU_ALLOC(multiCsiRsrcList->list.array, multiCsiRsrcList->list.size);
   if(multiCsiRsrcList->list.array == NULLP)
   {
      DU_LOG("\nERROR  --> DU APP: Memory allocation failed in BuildPucchMultiCsiRsrcList");
      return RFAILED;
   }

   for(rsrcIdx = 0; rsrcIdx<multiCsiRsrcList->list.count; rsrcIdx++)
   {
      DU_ALLOC(multiCsiRsrcList->list.array[rsrcIdx], sizeof(PUCCH_ResourceId_t));
      if(multiCsiRsrcList->list.array[rsrcIdx] == NULLP)
      {
         DU_LOG("\nERROR  --> DU APP: Memory allocation failed in BuildPucchMultiCsiRsrcList");
         return RFAILED;
      }
   }

   for(rsrcIdx = 0; rsrcIdx<multiCsiRsrcList->list.count; rsrcIdx++)
   {
      *(multiCsiRsrcList->list.array[rsrcIdx]) = multiCsiDb->multiCsiResrcList[rsrcIdx];
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds DL data -to- Ul Ack list
 *
 * @details
 *
 *    Function : BuildDlDataToUlAckList
 *
 *    Functionality: Builds DL data -to- Ul Ack list
 *
 * @params[in] : PucchDlDataToUlAck *dlDataToUlAckDb
 *               struct PUCCH_Config__dl_DataToUL_ACK * dlDataToUlACKList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildDlDataToUlAckList(PucchDlDataToUlAck *dlDataToUlAckDb, struct PUCCH_Config__dl_DataToUL_ACK * dlDataToUlACKList)
{
   uint8_t elementCnt = 0, arrIdx = 0;

   if(dlDataToUlAckDb == NULLP)
      elementCnt = 2;
   else
      elementCnt = dlDataToUlAckDb->dlDataToUlAckListCount;

   dlDataToUlACKList->list.count = elementCnt;
   dlDataToUlACKList->list.size = elementCnt * sizeof(long *);
   DU_ALLOC(dlDataToUlACKList->list.array, dlDataToUlACKList->list.size);
   if(dlDataToUlACKList->list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDlDataToUlAckList");
      return RFAILED;
   }   

   for(arrIdx = 0; arrIdx <  dlDataToUlACKList->list.count; arrIdx++)
   {
      DU_ALLOC(dlDataToUlACKList->list.array[arrIdx], sizeof(long));
      if(dlDataToUlACKList->list.array[arrIdx] == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDlDataToUlAckList");
         return RFAILED;
      }   
   }

   if(dlDataToUlAckDb == NULLP)
   {
      arrIdx = 0;
      *(dlDataToUlACKList->list.array[arrIdx++]) = 1;
      *(dlDataToUlACKList->list.array[arrIdx]) = 2;
   }
   else
   {
      for(arrIdx = 0; arrIdx <  dlDataToUlACKList->list.count; arrIdx++)
      {
         *(dlDataToUlACKList->list.array[arrIdx]) = dlDataToUlAckDb->dlDataToUlAckList[arrIdx];
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds BWP UL dedicated PUCCH Config
 *
 * @details
 *
 *    Function : BuildBWPUlDedPucchCfg
 *
 *    Functionality:
 *      Builds BWP UL dedicated PUCCH Config
 *
 * @params[in] : PUCCH_Config_t *pucchCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildBWPUlDedPucchCfg(PucchCfg *pucchCfgDb, PUCCH_Config_t *pucchCfg)
{
   PucchResrcSetCfg *rsrcSetCfgDb = NULLP;
   PucchResrcCfg *rsrcCfgDb = NULLP;
   PucchFormatCfg *format1Db = NULLP;
   PucchFormatCfg *format2Db = NULLP;
   PucchFormatCfg *format3Db = NULLP;
   PucchFormatCfg *format4Db = NULLP;
   PucchSchedReqCfg *schReqDb = NULLP;   
   PucchMultiCsiCfg  *multiCsiDb = NULLP;
   PucchDlDataToUlAck *dlDataToUlAckDb = NULLP;

   if(pucchCfgDb)
   {
      rsrcSetCfgDb = pucchCfgDb->resrcSet;
      rsrcCfgDb = pucchCfgDb->resrc;
      format1Db = pucchCfgDb->format1;
      format2Db = pucchCfgDb->format2;
      format3Db = pucchCfgDb->format3;
      format4Db = pucchCfgDb->format4;
      schReqDb = pucchCfgDb->schedReq;
      multiCsiDb = pucchCfgDb->multiCsiCfg;
      dlDataToUlAckDb = pucchCfgDb->dlDataToUlAck;
   }

   /* RESOURCE SET */
   DU_ALLOC(pucchCfg->resourceSetToAddModList, sizeof(struct PUCCH_Config__resourceSetToAddModList));
   if(pucchCfg->resourceSetToAddModList == NULL)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
      return RFAILED;
   }

   if(BuildPucchRsrcSetAddModList(rsrcSetCfgDb, pucchCfg->resourceSetToAddModList) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
      return RFAILED;
   }

   /* PUCCH RESOURCE */
   DU_ALLOC(pucchCfg->resourceToAddModList, sizeof(struct PUCCH_Config__resourceToAddModList));
   if(pucchCfg->resourceToAddModList == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
      return RFAILED;
   }

   if(BuildPucchRsrcAddModList(rsrcCfgDb, pucchCfg->resourceToAddModList) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
      return RFAILED;
   }

   /* PUCCH Format 1 */
   DU_ALLOC(pucchCfg->format1, sizeof(struct PUCCH_Config__format1));
   if(pucchCfg->format1 == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
      return RFAILED;
   }
   
   pucchCfg->format1->present = PUCCH_Config__format1_PR_setup;
   DU_ALLOC(pucchCfg->format1->choice.setup, sizeof(PUCCH_FormatConfig_t));
   if(pucchCfg->format1->choice.setup == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
      return RFAILED;
   }

   if(BuildPucchFormat(format1Db, pucchCfg->format1->choice.setup) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
      return RFAILED;
   }

   /* PUCCH Format 2 */
   if(format2Db)
   {
      DU_ALLOC(pucchCfg->format2, sizeof(struct PUCCH_Config__format2));
      if(pucchCfg->format2 == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
         return RFAILED;
      }

      pucchCfg->format2->present = PUCCH_Config__format2_PR_setup;
      DU_ALLOC(pucchCfg->format2->choice.setup, sizeof(PUCCH_FormatConfig_t));
      if(pucchCfg->format2->choice.setup == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
         return RFAILED;
      }

      if(BuildPucchFormat(format2Db, pucchCfg->format2->choice.setup) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
         return RFAILED;
      }
   }

   /* PUCCH Format 3 */
   if(format3Db)
   {
      DU_ALLOC(pucchCfg->format3, sizeof(struct PUCCH_Config__format3));
      if(pucchCfg->format3 == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
         return RFAILED;
      }

      pucchCfg->format3->present = PUCCH_Config__format3_PR_setup;
      DU_ALLOC(pucchCfg->format3->choice.setup, sizeof(PUCCH_FormatConfig_t));
      if(pucchCfg->format3->choice.setup == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
         return RFAILED;
      }

      if(BuildPucchFormat(format3Db, pucchCfg->format3->choice.setup) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
         return RFAILED;
      }
   }

   /* PUCCH Format 4 */
   if(format4Db)
   {
      DU_ALLOC(pucchCfg->format4, sizeof(struct PUCCH_Config__format4));
      if(pucchCfg->format4 == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
         return RFAILED;
      }

      pucchCfg->format4->present = PUCCH_Config__format4_PR_setup;
      DU_ALLOC(pucchCfg->format4->choice.setup, sizeof(PUCCH_FormatConfig_t));
      if(pucchCfg->format4->choice.setup == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
         return RFAILED;
      }

      if(BuildPucchFormat(format4Db, pucchCfg->format4->choice.setup) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
         return RFAILED;
      }
   }

   /* Scheduling Request */
   if(schReqDb && (schReqDb->schedAddModListCount != 0))
   {
      pucchCfg->schedulingRequestResourceToAddModList = NULLP;
      DU_ALLOC(pucchCfg->schedulingRequestResourceToAddModList, sizeof(struct PUCCH_Config__schedulingRequestResourceToAddModList));
      if(pucchCfg->schedulingRequestResourceToAddModList == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
         return RFAILED;
      }

      if(BuildPucchSchReqAddModList(schReqDb, pucchCfg->schedulingRequestResourceToAddModList) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
         return RFAILED;
      }
   }

   /* Multi CSI */
   if(multiCsiDb && (multiCsiDb->multiCsiResrcListCount != 0))
   {
      pucchCfg->multi_CSI_PUCCH_ResourceList = NULLP;
      DU_ALLOC(pucchCfg->multi_CSI_PUCCH_ResourceList, sizeof(struct PUCCH_Config__multi_CSI_PUCCH_ResourceList));
      if(pucchCfg->multi_CSI_PUCCH_ResourceList == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
         return RFAILED;
      }

      if(BuildPucchMultiCsiRsrcList(multiCsiDb, pucchCfg->multi_CSI_PUCCH_ResourceList) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
         return RFAILED;
      }
   }

   /* DL DATA TO UL ACK */
   DU_ALLOC(pucchCfg->dl_DataToUL_ACK, sizeof(struct PUCCH_Config__dl_DataToUL_ACK));
   if(pucchCfg->dl_DataToUL_ACK == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
      return RFAILED;
   }

   if(BuildDlDataToUlAckList(dlDataToUlAckDb, pucchCfg->dl_DataToUL_ACK) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
      return RFAILED;
   }
   
   /* TODO : spatial relation info add/mod list and power control*/

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills SRS resource to add/modify list 
 *
 * @details
 *
 *    Function : BuildSrsRsrcAddModList
 *
 *    Functionality: Fills SRS resource to add/modify list
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSrsRsrcAddModList(struct SRS_Config__srs_ResourceToAddModList *resourceList)
{
   uint8_t   elementCnt;
   uint8_t   rsrcIdx;

   elementCnt = 1;
   resourceList->list.count = elementCnt;
   resourceList->list.size = elementCnt * sizeof(SRS_Resource_t *);
   resourceList->list.array = NULLP;
   DU_ALLOC(resourceList->list.array, resourceList->list.size);
   if(!resourceList->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSrsRsrcAddModList");
      return RFAILED;
   }

   for(rsrcIdx = 0; rsrcIdx < resourceList->list.count; rsrcIdx++)
   {
      DU_ALLOC(resourceList->list.array[rsrcIdx], sizeof(SRS_Resource_t));
      if(!resourceList->list.array[rsrcIdx])
      {
	 DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSrsRsrcAddModList");
	 return RFAILED;
      }
   }

   rsrcIdx = 0;
   resourceList->list.array[rsrcIdx]->srs_ResourceId = SRS_RSRC_ID;
   resourceList->list.array[rsrcIdx]->nrofSRS_Ports = SRS_Resource__nrofSRS_Ports_port1;
   resourceList->list.array[rsrcIdx]->transmissionComb.present = SRS_Resource__transmissionComb_PR_n2;

   resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2 = NULLP;
   DU_ALLOC(resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2, \
	 sizeof(struct SRS_Resource__transmissionComb__n2));
   if(!resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSrsRsrcAddModList");
      return RFAILED;
   }
   resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2->combOffset_n2\
      = SRS_COMB_OFFSET_N2;
   resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2->cyclicShift_n2\
      = SRS_CYCLIC_SHIFT_N2;

   resourceList->list.array[rsrcIdx]->resourceMapping.startPosition = \
								      PUSCH_START_SYMBOL;
   resourceList->list.array[rsrcIdx]->resourceMapping.nrofSymbols =  \
								     SRS_Resource__resourceMapping__nrofSymbols_n1;
   resourceList->list.array[rsrcIdx]->resourceMapping.repetitionFactor = \
									 SRS_Resource__resourceMapping__repetitionFactor_n1;

   resourceList->list.array[rsrcIdx]->freqDomainPosition = SRS_FREQ_DOM_POS;
   resourceList->list.array[rsrcIdx]->freqDomainShift = SRS_FREQ_DOM_SHIFT;
   resourceList->list.array[rsrcIdx]->freqHopping.c_SRS = C_SRS;
   resourceList->list.array[rsrcIdx]->freqHopping.b_SRS = B_SRS;
   resourceList->list.array[rsrcIdx]->freqHopping.b_hop = B_HOP;
   resourceList->list.array[rsrcIdx]->groupOrSequenceHopping = \
							       SRS_Resource__groupOrSequenceHopping_neither;

   /* Setting resource type to aperiodic for intergration purposes */
   resourceList->list.array[rsrcIdx]->resourceType.present = \
							     SRS_Resource__resourceType_PR_aperiodic;
   resourceList->list.array[rsrcIdx]->resourceType.choice.aperiodic = NULLP;
   DU_ALLOC(resourceList->list.array[rsrcIdx]->resourceType.choice.aperiodic,
	 sizeof(struct SRS_Resource__resourceType__aperiodic));
   if(!resourceList->list.array[rsrcIdx]->resourceType.choice.aperiodic)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSrsRsrcAddModList");
      return RFAILED;
   }

   resourceList->list.array[rsrcIdx]->sequenceId = SRS_SEQ_ID;

   return ROK;
}

/*******************************************************************
 *
 * @brief Build SRS resource set Add/mod list
 *
 * @details
 *
 *    Function : BuildSrsRsrcSetAddModList
 *
 *    Functionality: Build SRS resource set Add/mod list
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
   uint8_t BuildSrsRsrcSetAddModList
(
 struct SRS_Config__srs_ResourceSetToAddModList *rsrcSetList
 )
{
   uint8_t  elementCnt;
   uint8_t  rSetIdx;
   uint8_t  rsrcIdx;
   struct SRS_ResourceSet__srs_ResourceIdList *rsrcIdList;

   elementCnt = 1;
   rsrcSetList->list.count = elementCnt;
   rsrcSetList->list.size = elementCnt * sizeof(SRS_ResourceSet_t *);
   rsrcSetList->list.array = NULLP;
   DU_ALLOC(rsrcSetList->list.array, rsrcSetList->list.size);
   if(!rsrcSetList->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSrsRsrcSetAddModList");
      return RFAILED;
   }

   for(rSetIdx = 0; rSetIdx < rsrcSetList->list.count; rSetIdx++)
   {
      DU_ALLOC(rsrcSetList->list.array[rSetIdx], sizeof(SRS_ResourceSet_t));
      if(!rsrcSetList->list.array[rSetIdx])
      {
	 DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSrsRsrcSetAddModList");
	 return RFAILED;
      }
   }

   rSetIdx = 0;
   rsrcSetList->list.array[rSetIdx]->srs_ResourceSetId = SRS_RSET_ID;

   /* Fill Resource Id list in resource set */
   rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList = NULLP;
   DU_ALLOC(rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList,\
	 sizeof(struct SRS_ResourceSet__srs_ResourceIdList));
   if(!rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSrsRsrcSetAddModList");
      return RFAILED;
   }

   elementCnt = 1;
   rsrcIdList = rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList;
   rsrcIdList->list.count = elementCnt;
   rsrcIdList->list.size = elementCnt * sizeof(SRS_ResourceId_t *);
   rsrcIdList->list.array = NULLP;
   DU_ALLOC(rsrcIdList->list.array, rsrcIdList->list.size);
   if(!rsrcIdList->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSrsRsrcSetAddModList");
      return RFAILED;
   }

   for(rsrcIdx = 0; rsrcIdx < rsrcIdList->list.count; rsrcIdx++)
   {
      DU_ALLOC(rsrcIdList->list.array[rsrcIdx], sizeof(SRS_ResourceId_t));
      if(!rsrcIdList->list.array[rsrcIdx])
      {
	 DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSrsRsrcSetAddModList");
	 return RFAILED;
      }
   }

   rsrcIdx = 0;
   *rsrcIdList->list.array[rsrcIdx] = SRS_RSRC_ID;

   /* Fill resource type */
   rsrcSetList->list.array[rSetIdx]->resourceType.present = \
							    SRS_ResourceSet__resourceType_PR_aperiodic;

   rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic = NULLP;
   DU_ALLOC(rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic, \
	 sizeof(struct SRS_ResourceSet__resourceType__aperiodic));
   if(!rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSrsRsrcSetAddModList");
      return RFAILED;
   }
   rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic->aperiodicSRS_ResourceTrigger \
      = APERIODIC_SRS_RESRC_TRIGGER;

   /* TODO : Fill values for below IEs as expected by Viavi */
   rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic->csi_RS = NULLP;
   rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic->slotOffset = NULLP;


   rsrcSetList->list.array[rSetIdx]->usage = SRS_ResourceSet__usage_codebook;
   rsrcSetList->list.array[rSetIdx]->alpha = NULLP;
   rsrcSetList->list.array[rSetIdx]->p0 = NULLP;
   rsrcSetList->list.array[rSetIdx]->pathlossReferenceRS = NULLP;
   rsrcSetList->list.array[rSetIdx]->srs_PowerControlAdjustmentStates = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds BWP UL dedicated SRS Config
 *
 * @details
 *
 *    Function : BuildBWPUlDedSrsCfg
 *
 *    Functionality: Builds BWP UL dedicated SRS Config
 *
 * @params[in] SRS Config 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildBWPUlDedSrsCfg(SRS_Config_t *srsCfg)
{
   srsCfg->srs_ResourceSetToReleaseList = NULLP;
   srsCfg->srs_ResourceSetToAddModList = NULLP;
   DU_ALLOC(srsCfg->srs_ResourceSetToAddModList, \
	 sizeof(struct SRS_Config__srs_ResourceSetToAddModList));
   if(!srsCfg->srs_ResourceSetToAddModList)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedSrsCfg");
      return RFAILED;
   }
   if(BuildSrsRsrcSetAddModList(srsCfg->srs_ResourceSetToAddModList) != ROK)
   {
      return RFAILED;
   }

   srsCfg->srs_ResourceToReleaseList = NULLP;

   /* Resource to Add/Modify list */
   srsCfg->srs_ResourceToAddModList = NULLP;
   DU_ALLOC(srsCfg->srs_ResourceToAddModList, \
	 sizeof(struct SRS_Config__srs_ResourceToAddModList));
   if(!srsCfg->srs_ResourceToAddModList)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedSrsCfg");
      return RFAILED;
   }

   if(BuildSrsRsrcAddModList(srsCfg->srs_ResourceToAddModList) != ROK)
   {
      return RFAILED;
   }
   srsCfg->tpc_Accumulation = NULLP;

   return ROK;
}



/*******************************************************************
 *
 * @brief Builds Pusch Serving cell Config
 *
 * @details
 *
 *    Function : BuildPuschSrvCellCfg
 *
 *    Functionality: Builds Pusch Serving cell Config
 *
 * @params[in] struct UplinkConfig__pusch_ServingCellConfig *puschCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPuschSrvCellCfg(struct UplinkConfig__pusch_ServingCellConfig *puschCfg)
{
   puschCfg->present = UplinkConfig__pusch_ServingCellConfig_PR_setup;
   puschCfg->choice.setup = NULLP;
   DU_ALLOC(puschCfg->choice.setup, sizeof(struct PUSCH_ServingCellConfig));
   if(!puschCfg->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPuschSrvCellCfg");
      return RFAILED;
   }

   puschCfg->choice.setup->codeBlockGroupTransmission = NULLP;
   puschCfg->choice.setup->rateMatching = NULLP;
   puschCfg->choice.setup->xOverhead = NULLP;
   puschCfg->choice.setup->ext1 = NULLP;

   DU_ALLOC(puschCfg->choice.setup->ext1, sizeof(struct PUSCH_ServingCellConfig__ext1));
   if(!puschCfg->choice.setup->ext1)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPuschSrvCellCfg");
      return RFAILED;
   }

   puschCfg->choice.setup->ext1->maxMIMO_Layers = NULLP;
   DU_ALLOC(puschCfg->choice.setup->ext1->maxMIMO_Layers, sizeof(long));
   if(!puschCfg->choice.setup->ext1->maxMIMO_Layers)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPuschSrvCellCfg");
      return RFAILED;
   }
   *(puschCfg->choice.setup->ext1->maxMIMO_Layers) = PUSCH_MAX_MIMO_LAYERS;

   puschCfg->choice.setup->ext1->processingType2Enabled= NULLP;
   DU_ALLOC(puschCfg->choice.setup->ext1->processingType2Enabled,sizeof(BOOLEAN_t));
   if(!puschCfg->choice.setup->ext1->processingType2Enabled)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPuschSrvCellCfg");
      return RFAILED;
   }
   *(puschCfg->choice.setup->ext1->processingType2Enabled) = PUSCH_PROCESS_TYPE2_ENABLED;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds inital UL BWP
 *
 * @details
 *
 *    Function : BuildInitialUlBWP
 *
 *    Functionality: Builds initial UL BWP
 *
 * @params[in] BWP_UplinkDedicated_t *ulBwp
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildInitialUlBWP(InitialUlBwp *initUlBwp, BWP_UplinkDedicated_t *ulBwp)
{
   PucchCfg *pucchCfg = NULLP;
   PuschCfg *puschCfg = NULLP;

   if(initUlBwp)
   {
      if(initUlBwp->pucchPresent)
         pucchCfg = &initUlBwp->pucchCfg;
      if(initUlBwp->puschPresent)
         puschCfg = &initUlBwp->puschCfg;
   }

   ulBwp->pucch_Config = NULLP;
   DU_ALLOC(ulBwp->pucch_Config, sizeof(struct BWP_UplinkDedicated__pucch_Config));
   if(!ulBwp->pucch_Config)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildInitialUlBWP");
      return RFAILED;
   }

   ulBwp->pucch_Config->present = BWP_UplinkDedicated__pucch_Config_PR_setup;
   ulBwp->pucch_Config->choice.setup = NULLP;
   DU_ALLOC(ulBwp->pucch_Config->choice.setup, sizeof(PUCCH_Config_t));
   if(!ulBwp->pucch_Config->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildInitialUlBWP");
      return RFAILED;
   }

   if(BuildBWPUlDedPucchCfg(pucchCfg, ulBwp->pucch_Config->choice.setup) != ROK)
   {
      return RFAILED;
   }

   /* Fill BWP UL dedicated PUSCH config */
   ulBwp->pusch_Config = NULLP;
   DU_ALLOC(ulBwp->pusch_Config, sizeof(struct BWP_UplinkDedicated__pusch_Config));
   if(!ulBwp->pusch_Config)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildInitialUlBWP");
      return RFAILED;
   }

   ulBwp->pusch_Config->present = BWP_UplinkDedicated__pusch_Config_PR_setup;
   ulBwp->pusch_Config->choice.setup = NULLP;
   DU_ALLOC(ulBwp->pusch_Config->choice.setup, sizeof(PUSCH_Config_t));
   if(!ulBwp->pusch_Config->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildInitialUlBWP");
      return RFAILED;
   }

   if(BuildBWPUlDedPuschCfg(puschCfg, ulBwp->pusch_Config->choice.setup) != ROK)
   {
      return RFAILED;
   }

   ulBwp->configuredGrantConfig = NULLP;

   /* Fill BPW UL dedicated SRS config */
   ulBwp->srs_Config = NULLP;
   DU_ALLOC(ulBwp->srs_Config, sizeof(struct BWP_UplinkDedicated__srs_Config));
   if(!ulBwp->srs_Config)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildInitialUlBWP");
      return RFAILED;
   }

   ulBwp->srs_Config->present = BWP_UplinkDedicated__srs_Config_PR_setup;
   ulBwp->srs_Config->choice.setup = NULLP;
   DU_ALLOC(ulBwp->srs_Config->choice.setup, sizeof(SRS_Config_t));
   if(!ulBwp->srs_Config->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildInitialUlBWP");
      return RFAILED;
   }

   if(BuildBWPUlDedSrsCfg(ulBwp->srs_Config->choice.setup) != ROK)
   {
      return RFAILED;   
   }

   ulBwp->beamFailureRecoveryConfig = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds UL config
 * @details
 *
 *    Function : BuildUlCfg 
 *
 *    Functionality: Builds UL config in spCellCfgDed
 *
 * @params[in] UplinkConfig_t *ulCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildUlCfg(ServCellRecfgInfo *servCellRecfg, UplinkConfig_t *ulCfg)
{
   InitialUlBwp *initUlBwp = NULLP;

   if(servCellRecfg)
   {
      initUlBwp = &servCellRecfg->initUlBwp;
   }

   ulCfg->initialUplinkBWP = NULLP;
   DU_ALLOC(ulCfg->initialUplinkBWP, sizeof(BWP_UplinkDedicated_t));
   if(!ulCfg->initialUplinkBWP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory Allocation failed in BuildUlCfg");
      return RFAILED;
   }

   if(BuildInitialUlBWP(initUlBwp, ulCfg->initialUplinkBWP) != ROK)
   {
      return RFAILED;
   }

   ulCfg->uplinkBWP_ToReleaseList = NULLP;
   ulCfg->uplinkBWP_ToAddModList = NULLP;
   ulCfg->firstActiveUplinkBWP_Id = NULLP;
   DU_ALLOC(ulCfg->firstActiveUplinkBWP_Id, sizeof(BWP_Id_t));
   if(!ulCfg->firstActiveUplinkBWP_Id)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory Allocation failed in BuildUlCfg");
      return RFAILED;
   }
   if(servCellRecfg == NULLP)
      *(ulCfg->firstActiveUplinkBWP_Id) = ACTIVE_UL_BWP_ID;
   else
      *(ulCfg->firstActiveUplinkBWP_Id) = servCellRecfg->firstActvUlBwpId;

   ulCfg->pusch_ServingCellConfig = NULLP;
   DU_ALLOC(ulCfg->pusch_ServingCellConfig, sizeof(struct UplinkConfig__pusch_ServingCellConfig));
   if(!ulCfg->pusch_ServingCellConfig)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory Allocation failed in BuildUlCfg");
      return RFAILED;
   }

   if(BuildPuschSrvCellCfg(ulCfg->pusch_ServingCellConfig) != ROK)
   {
      return RFAILED;
   }

   ulCfg->carrierSwitching = NULLP;
   ulCfg->ext1 = NULLP;
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds PDSCH serving cell config
 * @details
 *
 *    Function : BuildPdschSrvCellCfg
 *
 *    Functionality: Builds PDSCH serving cell config in spCellCfgDed
 *
 * @params[in] struct ServingCellConfig__pdsch_ServingCellConfig *pdschCfg 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPdschSrvCellCfg(PdschServCellCfg *pdschServCellDb, struct ServingCellConfig__pdsch_ServingCellConfig *pdschCfg)
{
   pdschCfg->present =  ServingCellConfig__pdsch_ServingCellConfig_PR_setup;
   pdschCfg->choice.setup = NULLP;
   DU_ALLOC(pdschCfg->choice.setup, sizeof( struct PDSCH_ServingCellConfig));
   if(!pdschCfg->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschSrvCellCfg");
      return RFAILED;
   }

   /* Code Block Group Transmission */
   pdschCfg->choice.setup->codeBlockGroupTransmission = NULLP;
   if(pdschServCellDb && (pdschServCellDb->maxCodeBlkGrpPerTb || pdschServCellDb->codeBlkGrpFlushInd))
   {
      DU_ALLOC(pdschCfg->choice.setup->codeBlockGroupTransmission, sizeof(struct PDSCH_ServingCellConfig__codeBlockGroupTransmission));
      if(pdschCfg->choice.setup->codeBlockGroupTransmission == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschSrvCellCfg");
         return RFAILED;
      }

      pdschCfg->choice.setup->codeBlockGroupTransmission->present = PDSCH_ServingCellConfig__codeBlockGroupTransmission_PR_setup;
      pdschCfg->choice.setup->codeBlockGroupTransmission->choice.setup = NULLP;
      DU_ALLOC(pdschCfg->choice.setup->codeBlockGroupTransmission->choice.setup, sizeof(struct PDSCH_CodeBlockGroupTransmission ));
      if(pdschCfg->choice.setup->codeBlockGroupTransmission->choice.setup == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschSrvCellCfg");
         return RFAILED;
      }

      pdschCfg->choice.setup->codeBlockGroupTransmission->choice.setup->maxCodeBlockGroupsPerTransportBlock = \
         *(pdschServCellDb->maxCodeBlkGrpPerTb);
      pdschCfg->choice.setup->codeBlockGroupTransmission->choice.setup->codeBlockGroupFlushIndicator = \
         *(pdschServCellDb->codeBlkGrpFlushInd);
   }

   /* xOverhead */
   pdschCfg->choice.setup->xOverhead = NULLP;
   if(pdschServCellDb && pdschServCellDb->xOverhead)
   {
      DU_ALLOC(pdschCfg->choice.setup->xOverhead, sizeof(long));
      if(pdschCfg->choice.setup->xOverhead == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschSrvCellCfg");
         return RFAILED;
      }
      *(pdschCfg->choice.setup->xOverhead) = *(pdschServCellDb->xOverhead);
   }

   /* Number of HARQ processes */
   pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH = NULLP;
   DU_ALLOC(pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH, sizeof(long));
   if(!pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschSrvCellCfg");
      return RFAILED;
   }

   if(pdschServCellDb == NULLP)
   *(pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH) = PDSCH_NUM_HARQ_PROC;
   else
   *(pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH) = pdschServCellDb->numHarqProcForPdsch;

   pdschCfg->choice.setup->pucch_Cell = NULLP;

   /* Extension */
   pdschCfg->choice.setup->ext1 = NULLP;
   if(pdschServCellDb && pdschServCellDb->maxMimoLayers)
   {
      DU_ALLOC(pdschCfg->choice.setup->ext1, sizeof(struct PDSCH_ServingCellConfig__ext1));
      if(pdschCfg->choice.setup->ext1 == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschSrvCellCfg");
         return RFAILED;
      }

      DU_ALLOC(pdschCfg->choice.setup->ext1->maxMIMO_Layers, sizeof(long));
      if(pdschCfg->choice.setup->ext1->maxMIMO_Layers == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschSrvCellCfg");
         return RFAILED;
      }
      *(pdschCfg->choice.setup->ext1->maxMIMO_Layers) = *(pdschServCellDb->maxMimoLayers);
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds CSI Meas config
 * @details
 *
 *    Function : BuildCsiMeasCfg 
 *
 *    Functionality: Builds CSI Meas config in spCellCfgDed
 *
 * @params[in] struct ServingCellConfig__csi_MeasConfig *csiMeasCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildCsiMeasCfg(struct ServingCellConfig__csi_MeasConfig *csiMeasCfg)
{

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds DL BWP to add/modify list
 * @details
 *
 *    Function : BuildDlBwpToAddModList
 *
 *    Functionality: Builds DL BWP to add/modify list
 *
 * @params[in] ServCellRecfgInfo *servCellRecfg, 
 *             struct ServingCellConfig__downlinkBWP_ToAddModList *dlBwpAddModList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/ 
uint8_t BuildDlBwpToAddModList(ServCellRecfgInfo *servCellRecfg, struct ServingCellConfig__downlinkBWP_ToAddModList *dlBwpAddModList)
{
   uint8_t elementCnt, idx;

   elementCnt = servCellRecfg->numDlBwpToAddOrMod;
   dlBwpAddModList->list.count = elementCnt;
   dlBwpAddModList->list.size = elementCnt * sizeof(struct BWP_Downlink *);
   dlBwpAddModList->list.array = NULLP;
   DU_ALLOC(dlBwpAddModList->list.array, dlBwpAddModList->list.size);
   if(dlBwpAddModList->list.array == NULLP)
   {
      DU_LOG("\nERROR  --> DU APP: Memory allocation failure in BuildDlBwpToAddModList");
      return RFAILED;
   }

   for(idx=0; idx<dlBwpAddModList->list.count; idx++)
   {
      DU_ALLOC(dlBwpAddModList->list.array[idx], sizeof(BWP_Downlink_t));
      if(dlBwpAddModList->list.array[idx] == NULLP)
      {
         DU_LOG("\nERROR  --> DU APP: Memory allocation failure in BuildDlBwpToAddModList");
         return RFAILED;
      }
   }

   for(idx=0; idx<dlBwpAddModList->list.count; idx++)
   {
      dlBwpAddModList->list.array[idx]->bwp_Id = servCellRecfg->dlBwpToAddOrModList[idx].bwpId;
      dlBwpAddModList->list.array[idx]->bwp_Common = NULLP;
      dlBwpAddModList->list.array[idx]->bwp_Dedicated = NULLP;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds Spcell config dedicated
 * @details
 *
 *    Function : BuildSpCellCfgDed
 *
 *    Functionality: Builds sp cell config dedicated in spCellCfg
 *
 * @params[in] ServingCellConfig_t srvCellCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSpCellCfgDed(DuUeCb *ueCb, ServingCellConfig_t *srvCellCfg)
{
   ServCellRecfgInfo *servCellRecfg = NULLP;
   InitialDlBwp *initDlBwp = NULLP;
   PdschServCellCfg *pdschServCellDb = NULLP;

   if(ueCb)
   {
      servCellRecfg = &ueCb->duMacUeCfg.spCellCfg.servCellCfg;
      initDlBwp = &servCellRecfg->initDlBwp;
      pdschServCellDb = &servCellRecfg->pdschServCellCfg;
   }

   srvCellCfg->tdd_UL_DL_ConfigurationDedicated = NULLP;

   srvCellCfg->initialDownlinkBWP = NULLP;
   DU_ALLOC(srvCellCfg->initialDownlinkBWP, sizeof(BWP_DownlinkDedicated_t));
   if(!srvCellCfg->initialDownlinkBWP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSpCellCfgDed");
      return RFAILED;
   }

   if(BuildInitialDlBWP(initDlBwp, srvCellCfg->initialDownlinkBWP) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : BuildInitialDlBWP failed");
      return RFAILED;
   }

   srvCellCfg->downlinkBWP_ToReleaseList = NULLP;

   srvCellCfg->downlinkBWP_ToAddModList = NULLP;
   if(ueCb && ueCb->duMacUeCfg.spCellCfg.servCellCfg.numDlBwpToAddOrMod)
   {
      DU_ALLOC(srvCellCfg->downlinkBWP_ToAddModList, sizeof(struct ServingCellConfig__downlinkBWP_ToAddModList));
      if(srvCellCfg->downlinkBWP_ToAddModList == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSpCellCfgDed");
         return RFAILED;
      }

      if(BuildDlBwpToAddModList(&ueCb->duMacUeCfg.spCellCfg.servCellCfg, srvCellCfg->downlinkBWP_ToAddModList) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildInitialDlBWP failed");
         return RFAILED;
      }
   }

   srvCellCfg->firstActiveDownlinkBWP_Id = NULLP;
   DU_ALLOC(srvCellCfg->firstActiveDownlinkBWP_Id, sizeof(long));
   if(!srvCellCfg->firstActiveDownlinkBWP_Id)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSpCellCfgDed");
      return RFAILED;
   }
   if(ueCb == NULLP)
      *(srvCellCfg->firstActiveDownlinkBWP_Id) = ACTIVE_DL_BWP_ID;
   else
      *(srvCellCfg->firstActiveDownlinkBWP_Id) = ueCb->duMacUeCfg.spCellCfg.servCellCfg.firstActvDlBwpId;

   srvCellCfg->bwp_InactivityTimer = NULLP;

   srvCellCfg->defaultDownlinkBWP_Id = NULLP;
   DU_ALLOC(srvCellCfg->defaultDownlinkBWP_Id, sizeof(long));
   if(!srvCellCfg->defaultDownlinkBWP_Id)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSpCellCfgDed");
      return RFAILED;
   }
   if(ueCb == NULLP)
      *(srvCellCfg->defaultDownlinkBWP_Id) = ACTIVE_DL_BWP_ID;
   else
      *(srvCellCfg->defaultDownlinkBWP_Id) = ueCb->duMacUeCfg.spCellCfg.servCellCfg.defaultDlBwpId;

   srvCellCfg->uplinkConfig = NULLP;
   DU_ALLOC(srvCellCfg->uplinkConfig, sizeof(UplinkConfig_t));
   if(!srvCellCfg->uplinkConfig)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSpCellCfgDed");
      return RFAILED;
   }

   if(BuildUlCfg(servCellRecfg, srvCellCfg->uplinkConfig) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : BuildUlCfg failed");
      return RFAILED;
   }
   srvCellCfg->supplementaryUplink = NULLP;
   srvCellCfg->pdcch_ServingCellConfig = NULLP;

   srvCellCfg->pdsch_ServingCellConfig = NULLP;
   DU_ALLOC(srvCellCfg->pdsch_ServingCellConfig, sizeof(struct	ServingCellConfig__pdsch_ServingCellConfig));
   if(!srvCellCfg->pdsch_ServingCellConfig)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSpCellCfgDed");
      return RFAILED;
   }

   if(BuildPdschSrvCellCfg(pdschServCellDb, srvCellCfg->pdsch_ServingCellConfig) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : BuildPdschSrvCellCfg failed");
      return RFAILED;
   }

   srvCellCfg->csi_MeasConfig = NULLP;
#if 0
   DU_ALLOC(srvCellCfg->csi_MeasConfig, sizeof(struct	ServingCellConfig__csi_MeasConfig))
      if(!srvCellCfg->csi_MeasConfig)
      {
	 DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSpCellCfgDed");
	 return RFAILED;
      }

   if(BuildCsiMeasCfg(srvCellCfg->csi_MeasConfig) != ROK)
   {
      DU_LOG("\nF1AP : BuildCsiMeasCfg failed");
      return RFAILED;
   }
#endif
   srvCellCfg->sCellDeactivationTimer = NULLP;
   srvCellCfg->crossCarrierSchedulingConfig = NULLP;
   srvCellCfg->tag_Id = TAG_ID;
   srvCellCfg->dummy = NULLP;
   srvCellCfg->pathlossReferenceLinking = NULLP;
   srvCellCfg->servingCellMO = NULLP;
   srvCellCfg->ext1 = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills SCS specific carrier list in DL frequency info
 *
 * @details
 *
 *    Function : BuildScsSpecificCarrierListDl
 *
 *    Functionality: Fills SCS specific carrier list in DL frequency info
 *
 * @params[in] Pointer to struct FrequencyInfoDL__scs_SpecificCarrierList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildScsSpecificCarrierListDl(struct FrequencyInfoDL__scs_SpecificCarrierList *scsCarrierList)
{
   uint8_t elementCnt = 0, listIdx = 0;
   ScsSpecCarrier duScsSpecCarrier = duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg.dlScsCarrier;

   elementCnt = ODU_VALUE_ONE;
   scsCarrierList->list.count = elementCnt;
   scsCarrierList->list.size = elementCnt * sizeof(SCS_SpecificCarrier_t *);

   DU_ALLOC(scsCarrierList->list.array, scsCarrierList->list.size);
   if(!scsCarrierList->list.array)
   {
      DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for scs carrier list array \
         in BuildScsSpecificCarrierListDl()");
      return RFAILED;
   }

   for(listIdx = 0; listIdx < elementCnt; listIdx++)
   {
      DU_ALLOC(scsCarrierList->list.array[listIdx], sizeof(SCS_SpecificCarrier_t));
      if(!scsCarrierList->list.array[listIdx])
      {    
         DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for SCS Specific Carrier list array \
            element in BuildScsSpecificCarrierListDl()");
         return RFAILED;
      }    
   }

   listIdx = 0;
   scsCarrierList->list.array[listIdx]->offsetToCarrier = duScsSpecCarrier.scsOffset;
   scsCarrierList->list.array[listIdx]->subcarrierSpacing = duScsSpecCarrier.scs;
   scsCarrierList->list.array[listIdx]->carrierBandwidth = duScsSpecCarrier.scsBw;

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills DL frequency info in DL config common
 *
 * @details
 *
 *    Function : BuildFreqInfoDl
 *
 *    Functionality: Fills DL frequency info in DL config common
 *
 * @params[in] Pointer to DownlinkConfigCommon_t
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildFreqInfoDl(FrequencyInfoDL_t *frequencyInfoDL)
{
   uint8_t freqBandIdx = 0, elementCnt = 0;
   DlCfgCommon  dlCfg = duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg;

   /* TODO : Fill SSB Absolute Frequency */
   /*
      DU_ALLOC(frequencyInfoDL->absoluteFrequencySSB, sizeof(ARFCN_ValueNR_t));
      if(!frequencyInfoDL->absoluteFrequencySSB)
      {
      DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for SSB Absolute Frequency in BuildFreqInfoDl()");
      return RFAILED;
      }
      frequencyInfoDL->absoluteFrequencySSB = ?;
      */

   /* NR Multi Frequency Band List */
   elementCnt = ODU_VALUE_ONE;
   frequencyInfoDL->frequencyBandList.list.count = elementCnt;
   frequencyInfoDL->frequencyBandList.list.size = elementCnt * sizeof(FreqBandIndicatorNR_t *);

   DU_ALLOC(frequencyInfoDL->frequencyBandList.list.array, frequencyInfoDL->frequencyBandList.list.size);
   if(!frequencyInfoDL->frequencyBandList.list.array)
   {
      DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for Frequency Band List array in BuildFreqInfoDl()");
      return RFAILED;
   }

   for(freqBandIdx = 0; freqBandIdx < elementCnt; freqBandIdx++)
   {
      DU_ALLOC(frequencyInfoDL->frequencyBandList.list.array[freqBandIdx], sizeof(FreqBandIndicatorNR_t));
      if(!frequencyInfoDL->frequencyBandList.list.array[freqBandIdx])
      {
         DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for frequency band array element in BuildFreqInfoDl()");
         return RFAILED;
      }
   }

   freqBandIdx = 0;
   *(frequencyInfoDL->frequencyBandList.list.array[freqBandIdx]) = dlCfg.freqBandInd;

   /* TODO : Absolute Frequency to Point A */
   //frequencyInfoDL->absoluteFrequencyPointA

   /* Subcarrier Spacing specifc carrier List */
   if((BuildScsSpecificCarrierListDl(&frequencyInfoDL->scs_SpecificCarrierList)) != ROK)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to fill SCS specific carrier list DL in BuildFreqInfoDl()");
      return RFAILED;
   }

   return ROK;

}

/*******************************************************************
 *
 * @brief Fills DL config common in Serving cell config common
 *
 * @details
 *
 *    Function : BuildDlConfigCommon
 *
 *    Functionality: Fills DL config common in Serving cell config common
 *
 * @params[in] Pointer to DownlinkConfigCommon_t
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildDlConfigCommon(DownlinkConfigCommon_t *dlCfgCommon)
{
   /* DL Frequency Info */
   DU_ALLOC(dlCfgCommon->frequencyInfoDL, sizeof(FrequencyInfoDL_t));
   if(!dlCfgCommon->frequencyInfoDL)
   {
      DU_LOG("\nERROR  --> DU APP : Memory allocation failed for DL frequency info in BuildDlConfigCommon()");
      return RFAILED;
   }
   if((BuildFreqInfoDl(dlCfgCommon->frequencyInfoDL))!= ROK)
   {
      DU_LOG("\nERROR  --> DU APP : Failed to fill DL frequency info in BuildDlConfigCommon()");
      return RFAILED;
   }

   /* DL BWP config common */
   DU_ALLOC(dlCfgCommon->initialDownlinkBWP, sizeof(BWP_DownlinkCommon_t));
   if(!dlCfgCommon->initialDownlinkBWP)
   {
      DU_LOG("\nERROR  --> DU APP : Memory allocation failed for DL BWP config common in BuildDlConfigCommon()");
      return RFAILED;
   }
   if((BuildBwpDlCommon(dlCfgCommon->initialDownlinkBWP)) != ROK)
   {
      DU_LOG("\nERROR  --> DU APP : Failed to fill DL DWP config common in BuildDlConfigCommon()");
      return RFAILED;
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills SCS specific carrier list in UL frequency Info
 *
 * @details
 *
 *    Function : BuildScsSpecificCarrierListUl
 *
 *    Functionality: Fills SCS specific carrier list in UL frequency Info
 *
 * @params[in] Pointer to struct FrequencyInfoUL__scs_SpecificCarrierList
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildScsSpecificCarrierListUl(struct FrequencyInfoUL__scs_SpecificCarrierList *scsCarrierList)
{
   uint8_t elementCnt = 0, listIdx = 0; 
   ScsSpecCarrier   duScsSpecCarrier = duCfgParam.sib1Params.srvCellCfgCommSib.ulCfg.ulScsCarrier;

   elementCnt = ODU_VALUE_ONE;
   scsCarrierList->list.count = elementCnt;
   scsCarrierList->list.size = elementCnt * sizeof(SCS_SpecificCarrier_t *);

   DU_ALLOC(scsCarrierList->list.array, scsCarrierList->list.size);
   if(!scsCarrierList->list.array)
   {
      DU_LOG("\nERROR  -->  DU APP : SCS Specific Carrier list memory allocation failed");
      return RFAILED;
   }

   for(listIdx = 0; listIdx < scsCarrierList->list.count; listIdx++)
   {
      DU_ALLOC(scsCarrierList->list.array[listIdx], sizeof(SCS_SpecificCarrier_t));
      if(!scsCarrierList->list.array[listIdx])
      {    
         DU_LOG("\nERROR  -->  DU APP : SCS Specific Carrier list memory allocation failed");
         return RFAILED;
      }    
   }
   listIdx = 0; 
   scsCarrierList->list.array[listIdx]->offsetToCarrier = duScsSpecCarrier.scsOffset;
   scsCarrierList->list.array[listIdx]->subcarrierSpacing = duScsSpecCarrier.scs;
   scsCarrierList->list.array[listIdx]->carrierBandwidth = duScsSpecCarrier.scsBw;

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills frequency info in UL config common
 *
 * @details
 *
 *    Function : BuildFreqInfoUl
 *
 *    Functionality: Fills frequency info in UL config common
 *
 * @params[in] Pointer to FrequencyInfoUL_t
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildFreqInfoUl(FrequencyInfoUL_t *frequencyInfoUL)
{
   uint8_t elementCnt = 0, listIdx= 0;
   UlCfgCommon  ulCfg = duCfgParam.sib1Params.srvCellCfgCommSib.ulCfg;

   /* NR Multi Frequency Band List */
   DU_ALLOC(frequencyInfoUL->frequencyBandList, sizeof(MultiFrequencyBandListNR_t));
   if(!frequencyInfoUL->frequencyBandList)
   {
      DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for frequency band list in BuildFreqInfoUl()");
      return RFAILED;
   }

   elementCnt = ODU_VALUE_ONE;
   frequencyInfoUL->frequencyBandList->list.count = elementCnt;
   frequencyInfoUL->frequencyBandList->list.size = elementCnt * sizeof(FreqBandIndicatorNR_t *);

   DU_ALLOC(frequencyInfoUL->frequencyBandList->list.array, frequencyInfoUL->frequencyBandList->list.size);
   if(!frequencyInfoUL->frequencyBandList->list.array)
   {
      DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for frequency band array in BuildFreqInfoUl()");
      return RFAILED;
   }

   for(listIdx = 0; listIdx < elementCnt; listIdx++)
   {
      DU_ALLOC(frequencyInfoUL->frequencyBandList->list.array[listIdx], sizeof(FreqBandIndicatorNR_t));
      if(!frequencyInfoUL->frequencyBandList->list.array[listIdx])
      {
         DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for frequency band array element in BuildFreqInfoUl()");
         return RFAILED;
      }
   }

   listIdx = 0;
   *(frequencyInfoUL->frequencyBandList->list.array[listIdx]) = ulCfg.freqBandInd;

   /* TODO : Fill Absolute frequency point A */
   /*
      DU_ALLOC(frequencyInfoUL->absoluteFrequencyPointA, sizeof(ARFCN_ValueNR_t));
      if(!frequencyInfoUL->absoluteFrequencyPointA)
      {
      DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for absolute frequency point A in BuildFreqInfoUl()");
      return RFAILED;
      }
    *(frequencyInfoUL->absoluteFrequencyPointA) = ?;
    */

   /* Subcarrier Spacing specifc carrier */
   if((BuildScsSpecificCarrierListUl(&frequencyInfoUL->scs_SpecificCarrierList)) != ROK) 
   {
      DU_LOG("\nERROR  --> DU APP : Failed to fill SCS Specific Carrier list UL in BuildFreqInfoUl()");
      return RFAILED;
   }

   /* P-MAX */
   DU_ALLOC(frequencyInfoUL->p_Max, sizeof(P_Max_t));
   if(!frequencyInfoUL->p_Max)
   {
      DU_LOG("\nERROR  -->  DU APP : UL Frequency Infoo  memory allocation failure");
      return RFAILED;
   }
   *frequencyInfoUL->p_Max = ulCfg.pMax;

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills UL config common in Serving cell config common
 *
 * @details
 *
 *    Function : BuildUlConfigCommon
 *
 *    Functionality: Fills UL config common in Serving cell config common
 *
 * @params[in] Pointer to UplinkConfigCommon_t
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildUlConfigCommon(UplinkConfigCommon_t *ulCfgCommon)
{
   /* UL Frequency Info */
   DU_ALLOC(ulCfgCommon->frequencyInfoUL, sizeof(FrequencyInfoUL_t));
   if(!ulCfgCommon->frequencyInfoUL)
   {
      DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for UL frequency info in BuildUlConfigCommon()");
      return RFAILED;
   }

   if((BuildFreqInfoUl(ulCfgCommon->frequencyInfoUL)) != ROK)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to fill frequency info UL in BuildUlConfigCommon()");
      return RFAILED;
   }

   /* UL BWP common */
   DU_ALLOC(ulCfgCommon->initialUplinkBWP, sizeof(BWP_UplinkCommon_t));
   if(!ulCfgCommon->initialUplinkBWP)
   {
      DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for initial UL BWP in BuildUlConfigCommon()");
      return RFAILED;
   }

   if((BuildBwpUlCommon(ulCfgCommon->initialUplinkBWP)) != ROK)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to fill BWP UL common in BuildUlConfigCommon()");
      return RFAILED;
   }

   /* Time Alignment timer */
   ulCfgCommon->dummy = duCfgParam.sib1Params.srvCellCfgCommSib.ulCfg.timeAlignTimerComm;

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills SSB position in burst in SP cell config common
 *
 * @details
 *
 *    Function : BuildSsbPosInBurst
 *
 *    Functionality: 
 *       Fills SSB position in burst in SP cell config common
 *
 * @params[in] Pointer to struct ServingCellConfigCommon__ssb_PositionsInBurst
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSsbPosInBurst(struct ServingCellConfigCommon__ssb_PositionsInBurst *ssbPosInBurst)
{
   uint8_t bitStringSizeInBytes = 0;

   ssbPosInBurst->present = ServingCellConfigCommon__ssb_PositionsInBurst_PR_mediumBitmap;

   /* As per spec 38.331,in the definition of ServingCellConfigCommon */
   bitStringSizeInBytes = 1;
   ssbPosInBurst->choice.mediumBitmap.size = bitStringSizeInBytes * sizeof(uint8_t);

   DU_ALLOC(ssbPosInBurst->choice.mediumBitmap.buf, ssbPosInBurst->choice.mediumBitmap.size);
   if(!ssbPosInBurst->choice.mediumBitmap.buf)
   {
      DU_LOG("\nERROR  -->  DU APP : Memory Allocation failed for medium bit map buffer in BuildSsbPosInBurst()");
      return RFAILED;
   }

   if((fillBitString(&ssbPosInBurst->choice.mediumBitmap, 0, bitStringSizeInBytes, \
               duCfgParam.sib1Params.srvCellCfgCommSib.ssbPosInBurst)) != ROK)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to fill medium bit map in BuildSsbPosInBurst()");
      return RFAILED;
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills SP cell config common in Reconfig with Sync
 *
 * @details
 *
 *    Function : BuildSpCellConfigCommon
 *
 *    Functionality: Fills SP cell config common in Reconfig with Sync
 *
 * @params[in] Pointer to ServingCellConfigCommon_t
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSpCellConfigCommon(ServingCellConfigCommon_t *spCellConfigCommon)
{
   /* Physical Cell Identity */
   DU_ALLOC(spCellConfigCommon->physCellId, sizeof(PhysCellId_t));
   if(!spCellConfigCommon->physCellId)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to allocate memory for physical cell id in BuildSpCellConfigCommon()");
      return RFAILED;
   } 
   *(spCellConfigCommon->physCellId) = NR_PCI;

   /* Downlink Config Common */
   DU_ALLOC(spCellConfigCommon->downlinkConfigCommon, sizeof(DownlinkConfigCommon_t));
   if(!spCellConfigCommon->downlinkConfigCommon)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to allocate memory for DL Config Common in BuildSpCellConfigCommon()");
      return RFAILED;
   }
   if((BuildDlConfigCommon(spCellConfigCommon->downlinkConfigCommon)) != ROK)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to fill DL config commin in BuildSpCellConfigCommon()");
      return RFAILED;
   }

   /* Uplinlink Config Common */
   DU_ALLOC(spCellConfigCommon->uplinkConfigCommon, sizeof(UplinkConfigCommon_t));
   if(!spCellConfigCommon->uplinkConfigCommon)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to allocate memory for UL Config Common in BuildSpCellConfigCommon()");
      return RFAILED;
   }
   if((BuildUlConfigCommon(spCellConfigCommon->uplinkConfigCommon)) != ROK)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to fill UL config commin in BuildSpCellConfigCommon()");
      return RFAILED;
   }

   /* Timing Advance offset */
   DU_ALLOC(spCellConfigCommon->n_TimingAdvanceOffset, sizeof(long));
   if(!spCellConfigCommon->n_TimingAdvanceOffset)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to allocate memory for Timing Advance Offset in BuildSpCellConfigCommon()");
      return RFAILED;
   }
   *(spCellConfigCommon->n_TimingAdvanceOffset) = ServingCellConfigCommon__n_TimingAdvanceOffset_n0;

   /* SSB Position In Burst */
   DU_ALLOC(spCellConfigCommon->ssb_PositionsInBurst, sizeof(struct ServingCellConfigCommon__ssb_PositionsInBurst));
   if(!spCellConfigCommon->ssb_PositionsInBurst)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to allocate memory for SSB Position In Burst in BuildSpCellConfigCommon()");
      return RFAILED;
   }
   if((BuildSsbPosInBurst(spCellConfigCommon->ssb_PositionsInBurst)) != ROK)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to fill SSB Position In Burst in BuildSpCellConfigCommon()");
      return RFAILED;
   }

   /* SSB Periodicity in Serving cell */
   DU_ALLOC(spCellConfigCommon->ssb_periodicityServingCell, sizeof(long));
   if(!spCellConfigCommon->ssb_periodicityServingCell)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to allocate memory for SSB Periodicity serving cell in \
         BuildSpCellConfigCommon()");
      return RFAILED;
   }
   *(spCellConfigCommon->ssb_periodicityServingCell) = \
      convertSsbPeriodicityValueToEnum(duCfgParam.sib1Params.srvCellCfgCommSib.ssbPrdServingCell);

   /* DMRS Type A position */
   spCellConfigCommon->dmrs_TypeA_Position = convertDmrsTypeAPosValueToEnum(duCfgParam.macCellCfg.ssbCfg.dmrsTypeAPos);

   /* SSB subcarrier spacing */
   DU_ALLOC(spCellConfigCommon->ssbSubcarrierSpacing, sizeof(SubcarrierSpacing_t));
   if(!spCellConfigCommon->ssbSubcarrierSpacing)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to allocate memory for sub-carrier spacing in BuildSpCellConfigCommon()");
      return RFAILED;
   }
   *(spCellConfigCommon->ssbSubcarrierSpacing) = duCfgParam.sib1Params.srvCellCfgCommSib.scs;

   /* TDD UL-DL configuration common */
   DU_ALLOC(spCellConfigCommon->tdd_UL_DL_ConfigurationCommon, sizeof(TDD_UL_DL_ConfigCommon_t));
   if(!spCellConfigCommon->tdd_UL_DL_ConfigurationCommon)
   {
      DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for TDD UL-DL config common in BuildSpCellConfigCommon()");
      return RFAILED;
   }
   if((BuildTddUlDlCfgComm(spCellConfigCommon->tdd_UL_DL_ConfigurationCommon)) != ROK)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to fill TDD UL-DL config common in BuildSpCellConfigCommon()");
      return RFAILED;
   }

   /* SS PBCH Block Power */
   spCellConfigCommon->ss_PBCH_BlockPower = duCfgParam.sib1Params.srvCellCfgCommSib.ssPbchBlockPwr;

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills dedicated RACH configuration in Reconfiguration with sync
 *
 * @details
 *
 *    Function : BuildRecfgWithSync
 *
 *    Functionality: 
 *       Fills dedicated RACH configuration in Reconfiguration with sync
 *
 * @params[in] DU UE CB
 *             Pointer to Rach config dedicated struct
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildRachConfigDedicated(DuUeCb *ueCb, struct ReconfigurationWithSync__rach_ConfigDedicated *rachCfgDed)
{
   uint8_t elementCnt = 0, listIdx = 0;
   CFRA_t *cfra = NULLP;
   struct CFRA__resources__ssb *ssbResource = NULLP;
   RachCfgCommon duRachCfg = duCfgParam.sib1Params.srvCellCfgCommSib.ulCfg.rachCfg;

   rachCfgDed->present = ReconfigurationWithSync__rach_ConfigDedicated_PR_uplink;

   /* Uplink */
   DU_ALLOC(rachCfgDed->choice.uplink, sizeof(RACH_ConfigDedicated_t));
   if(!rachCfgDed->choice.uplink)
   {
      DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for RACH uplink configuration in BuildRachConfigDedicated()");
      return RFAILED;
   }

   /* CFRA : Contention free Random Access */
   DU_ALLOC(rachCfgDed->choice.uplink->cfra, sizeof(CFRA_t));
   if(!rachCfgDed->choice.uplink->cfra)
   {
      DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for CFRA in BuildRachConfigDedicated()");
      return RFAILED;
   }
   cfra = rachCfgDed->choice.uplink->cfra;

   /* CFRA occassions */
   DU_ALLOC(cfra->occasions, sizeof(struct CFRA__occasions));
   if(!cfra->occasions)
   {
      DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for CFRA occassion in BuildRachConfigDedicated()");
      return RFAILED;
   }

   /* CFRA occassions : RACH generic configuration */
   cfra->occasions->rach_ConfigGeneric.prach_ConfigurationIndex = duRachCfg.prachCfgIdx;
   cfra->occasions->rach_ConfigGeneric.msg1_FDM = duRachCfg.msg1Fdm;
   cfra->occasions->rach_ConfigGeneric.msg1_FrequencyStart = duRachCfg.msg1FreqStart;
   cfra->occasions->rach_ConfigGeneric.zeroCorrelationZoneConfig = duRachCfg.zeroCorrZoneCfg;
   cfra->occasions->rach_ConfigGeneric.preambleReceivedTargetPower = duRachCfg.preambleRcvdTgtPwr;
   cfra->occasions->rach_ConfigGeneric.preambleTransMax = duRachCfg.preambleTransMax;
   cfra->occasions->rach_ConfigGeneric.powerRampingStep = duRachCfg.pwrRampingStep;
   cfra->occasions->rach_ConfigGeneric.ra_ResponseWindow = duRachCfg.raRspWindow;

   /* CFRA occassions : SSB per RACH occasion */
   DU_ALLOC(cfra->occasions->ssb_perRACH_Occasion, sizeof(long));
   if(!cfra->occasions->ssb_perRACH_Occasion)
   {
      DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for SSB per RACH occassion in BuildRachConfigDedicated()");
      return RFAILED;
   }
   *(cfra->occasions->ssb_perRACH_Occasion) = convertCFRASsbPerRachOccasionValueToEnum(duCfgParam.macCellCfg.prachCfg.ssbPerRach);

   /* CFRA resource */
   cfra->resources.present = CFRA__resources_PR_ssb;

   /* CFRA resource : SSB */
   DU_ALLOC(cfra->resources.choice.ssb, sizeof(struct CFRA__resources__ssb));
   if(!cfra->resources.choice.ssb)
   {
      DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for CFRA resource - SSB in BuildRachConfigDedicated()");
      return RFAILED;
   }
   ssbResource = cfra->resources.choice.ssb;

   /* CFRA SSB resource list */
   elementCnt = ueCb->cfraResource.numSsb;
   ssbResource->ssb_ResourceList.list.count = elementCnt;
   ssbResource->ssb_ResourceList.list.size = elementCnt * sizeof(CFRA_SSB_Resource_t *);

   DU_ALLOC(ssbResource->ssb_ResourceList.list.array, ssbResource->ssb_ResourceList.list.size);
   if(!ssbResource->ssb_ResourceList.list.array)
   {
      DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for SSB resource list in BuildRachConfigDedicated()");
      return RFAILED;
   }

   for(listIdx = 0; listIdx < elementCnt; listIdx++)
   {
      DU_ALLOC(ssbResource->ssb_ResourceList.list.array[listIdx], sizeof(CFRA_SSB_Resource_t));
      if(!ssbResource->ssb_ResourceList.list.array[listIdx])
      {
         DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for SSB resource list element in BuildRachConfigDedicated()");  
         return RFAILED;
      }
      ssbResource->ssb_ResourceList.list.array[listIdx]->ssb = ueCb->cfraResource.ssbResource[listIdx].ssbIdx;
      ssbResource->ssb_ResourceList.list.array[listIdx]->ra_PreambleIndex = ueCb->cfraResource.ssbResource[listIdx].raPreambleIdx;
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills reconfiguration with sync in SP cell config
 *
 * @details
 *
 *    Function : BuildRecfgWithSync
 *
 *    Functionality: Fills reconfiguration with sync in SP cell config
 *
 * @params[in] DU UE CB
 *             Pointer to ReconfigurationWithSync_t
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildRecfgWithSync(DuUeCb *ueCb, ReconfigurationWithSync_t *recfgWithSync)
{
   /* SP Cell Config Common */  
   DU_ALLOC(recfgWithSync->spCellConfigCommon, sizeof(ServingCellConfigCommon_t));
   if(!recfgWithSync->spCellConfigCommon)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to allocate memory for spCellConfigCommon in BuildRecfgWithSync()");
      return RFAILED;
   }

   if((BuildSpCellConfigCommon(recfgWithSync->spCellConfigCommon)) != ROK)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to build SpCellConfigCommon in BuildRecfgWithSync()");
      return RFAILED;
   }

   /* New UE Identity */
   recfgWithSync->newUE_Identity = ueCb->crnti;

   /* T304 timer */
   recfgWithSync->t304 = ReconfigurationWithSync__t304_ms1000;

   /* RACH configuration dedicated */
   DU_ALLOC(recfgWithSync->rach_ConfigDedicated, sizeof(struct ReconfigurationWithSync__rach_ConfigDedicated));
   if(!recfgWithSync->rach_ConfigDedicated)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to allocate memory for RACH config dedicated in BuildRecfgWithSync()");
      return RFAILED;
   }

   if((BuildRachConfigDedicated(ueCb, recfgWithSync->rach_ConfigDedicated)) != ROK)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to build RACH config dedicated in BuildRecfgWithSync()");
      return RFAILED;
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds Spcell config 
 *
 * @details
 *
 *    Function : BuildSpCellCfg 
 *
 *    Functionality: Builds sp cell config in DuToCuRrcContainer
 *
 * @params[in] SpCellConfig_t spCellCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSpCellCfg(DuUeCb *ueCb, SpCellConfig_t *spCellCfg)
{
   spCellCfg->servCellIndex = NULLP;
   DU_ALLOC(spCellCfg->servCellIndex, sizeof(long));
   if(!spCellCfg->servCellIndex)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSpCellCfg");
      return RFAILED;
   }

   if(ueCb == NULLP)
      *(spCellCfg->servCellIndex) = SERV_CELL_IDX;
   else
      *(spCellCfg->servCellIndex) = ueCb->duMacUeCfg.spCellCfg.servCellIdx;

   spCellCfg->reconfigurationWithSync = NULLP;
   if(ueCb && (ueCb->ueState == UE_HANDIN_IN_PROGRESS))
   {
      DU_ALLOC(spCellCfg->reconfigurationWithSync, sizeof(ReconfigurationWithSync_t));
      if(!spCellCfg->reconfigurationWithSync)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSpCellCfg");
         return RFAILED;
      }

      if((BuildRecfgWithSync(ueCb, spCellCfg->reconfigurationWithSync)) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Building of reconfiguration with sync failed at BuildSpCellCfg");
         return RFAILED;
      }
   }

   spCellCfg->rlf_TimersAndConstants = NULLP;
   spCellCfg->rlmInSyncOutOfSyncThreshold = NULLP;

   DU_ALLOC(spCellCfg->rlmInSyncOutOfSyncThreshold, sizeof(long));
   if(!spCellCfg->rlmInSyncOutOfSyncThreshold)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSpCellCfg");
      return RFAILED;
   }
   *(spCellCfg->rlmInSyncOutOfSyncThreshold) = RLM_SYNC_OUT_SYNC_THRESHOLD;

   spCellCfg->spCellConfigDedicated = NULLP;
   DU_ALLOC(spCellCfg->spCellConfigDedicated, sizeof(ServingCellConfig_t));
   if(!spCellCfg->spCellConfigDedicated)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSpCellCfg");
      return RFAILED;
   }
   if(BuildSpCellCfgDed(ueCb, spCellCfg->spCellConfigDedicated) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : BuildSpCellCfgDed failed");
      return RFAILED;
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Builds Phy cell group config 
 *
 * @details
 *
 *    Function : BuildPhyCellGrpCfg 
 *
 *    Functionality: Builds Phy cell group config in DuToCuRrcContainer
 *
 * @params[in] PhysicalCellGroupConfig_t *phyCellGrpCfg 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPhyCellGrpCfg(DuUeCb *ueCb, PhysicalCellGroupConfig_t *phyCellGrpCfg)
{
   phyCellGrpCfg->harq_ACK_SpatialBundlingPUCCH = NULLP;
   phyCellGrpCfg->harq_ACK_SpatialBundlingPUSCH = NULLP;

   phyCellGrpCfg->p_NR_FR1 = NULLP;
   DU_ALLOC(phyCellGrpCfg->p_NR_FR1, sizeof(long));
   if(!phyCellGrpCfg->p_NR_FR1)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildPhyCellGrpCfg");
      return RFAILED;
   }

   if(ueCb == NULLP)
   {
      *(phyCellGrpCfg->p_NR_FR1)             = P_NR_FR1;
      phyCellGrpCfg->pdsch_HARQ_ACK_Codebook = PDSCH_HARQ_ACK_CODEBOOK;
   }
   else
   {
      *(phyCellGrpCfg->p_NR_FR1) = ueCb->duMacUeCfg.phyCellGrpCfg.pNrFr1;
      phyCellGrpCfg->pdsch_HARQ_ACK_Codebook = ueCb->duMacUeCfg.phyCellGrpCfg.pdschHarqAckCodebook;
   }

   phyCellGrpCfg->tpc_SRS_RNTI = NULLP;
   phyCellGrpCfg->tpc_PUCCH_RNTI = NULLP;
   phyCellGrpCfg->tpc_PUSCH_RNTI = NULLP;
   phyCellGrpCfg->sp_CSI_RNTI = NULLP;
   phyCellGrpCfg->cs_RNTI = NULLP;
   phyCellGrpCfg->ext1 = NULLP;
   phyCellGrpCfg->ext2 = NULLP;

   return ROK;
}
#ifdef NR_DRX
/*******************************************************************
 *
 * @brief fill long cycle offset value of drx
 *
 * @details
 *
 *    Function : fillLongCycleOffsetValFromDuCb 
 *
 *    Functionality: fill long cycle offset value of drx
 *
 * @params[in] DrxLongCycleStartOffset  drxLongCycleStartOffset,
 * struct DRX_ConfigRrc__drx_LongCycleStartOffset drx_LongCycleStartOffset
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void fillLongCycleOffsetValFromDuCb(DrxLongCycleStartOffset  drxLongCycleStartOffset,\
struct DRX_ConfigRrc__drx_LongCycleStartOffset *drx_LongCycleStartOffset)
{
   
   drx_LongCycleStartOffset->present = convertLongDrxCycleLengthValueToEnum(drxLongCycleStartOffset.drxLongCycleStartOffsetChoice); 
   switch(drx_LongCycleStartOffset->present)
   {
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms10:
         {
            drx_LongCycleStartOffset->choice.ms10 = drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms20:
         {
            drx_LongCycleStartOffset->choice.ms20 = drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms32:
         {
            drx_LongCycleStartOffset->choice.ms32 = drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms40:
         {
            drx_LongCycleStartOffset->choice.ms40 = drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms60:
         {
            drx_LongCycleStartOffset->choice.ms60 = drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms64:
         {
            drx_LongCycleStartOffset->choice.ms64 = drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms70:
         {
            drx_LongCycleStartOffset->choice.ms70 = drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms80:
         {
            drx_LongCycleStartOffset->choice.ms80 = drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms128:
         {
            drx_LongCycleStartOffset->choice.ms128 = drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms160:
         {
            drx_LongCycleStartOffset->choice.ms160 = drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms256:
         {
            drx_LongCycleStartOffset->choice.ms256 = drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms320:
         {
            drx_LongCycleStartOffset->choice.ms320 = drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms512:
         {
            drx_LongCycleStartOffset->choice.ms512 = drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms640:
         {
            drx_LongCycleStartOffset->choice.ms640 = drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms1024:
         {
            drx_LongCycleStartOffset->choice.ms1024 = drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms1280:
         {
            drx_LongCycleStartOffset->choice.ms1280 = drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms2048:
         {
            drx_LongCycleStartOffset->choice.ms2048 = drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms2560:
         {
            drx_LongCycleStartOffset->choice.ms2560 = drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms5120:
         {
            drx_LongCycleStartOffset->choice.ms5120 = drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms10240:
         {
            drx_LongCycleStartOffset->choice.ms10240 = drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
            break;
         }
      default :
         break;
   }
}
/*******************************************************************
 *
 * @brief Builds drx config IE 
 *
 * @details
 *
 *    Function : BuildDrxConfigRrc 
 *
 *    Functionality: Build drx config in MacCellGrpCfg 
 *
 * @params[in] struct MAC_CellGroupConfig__drx_ConfigRrc 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildDrxConfigRrc(DuUeCb *ueCb, struct MAC_CellGroupConfig__drx_ConfigRrc *drxCfg)
{
   drxCfg->present = MAC_CellGroupConfig__drx_ConfigRrc_PR_setup;
   DU_ALLOC(drxCfg->choice.setup, sizeof(struct DRX_ConfigRrc));
   if(!drxCfg->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildDrxConfigRrc");
      return RFAILED;
   }
   if(ueCb->duMacUeCfg.macCellGrpCfg.drxCfg.drxOnDurationTimer.onDurationTimerValInMs)
   {
      drxCfg->choice.setup->drx_onDurationTimer.present = DRX_ConfigRrc__drx_onDurationTimer_PR_milliSeconds;
      drxCfg->choice.setup->drx_onDurationTimer.choice.milliSeconds = convertOnDurationTimerMilliSecondsValueToEnum(ueCb->duMacUeCfg.\
      macCellGrpCfg.drxCfg.drxOnDurationTimer.onDurationtimerValue.milliSeconds);
   }
   else
   {
      drxCfg->choice.setup->drx_onDurationTimer.present = DRX_ConfigRrc__drx_onDurationTimer_PR_subMilliSeconds;
      drxCfg->choice.setup->drx_onDurationTimer.choice.subMilliSeconds = ueCb->duMacUeCfg.macCellGrpCfg.drxCfg.\
      drxOnDurationTimer.onDurationtimerValue.subMilliSeconds;
   }
   drxCfg->choice.setup->drx_InactivityTimer = convertDrxInactivityTimerValueToEnum(ueCb->duMacUeCfg.macCellGrpCfg.drxCfg.drxInactivityTimer);
   drxCfg->choice.setup->drx_HARQ_RTT_TimerDL = ueCb->duMacUeCfg.macCellGrpCfg.drxCfg.drxHarqRttTimerDl;
   drxCfg->choice.setup->drx_HARQ_RTT_TimerUL = ueCb->duMacUeCfg.macCellGrpCfg.drxCfg.drxHarqRttTimerUl;
   drxCfg->choice.setup->drx_RetransmissionTimerDL = convertDrxRetransmissionTimerDlValueToEnum(ueCb->duMacUeCfg.macCellGrpCfg.drxCfg.\
   drxRetransmissionTimerDl);
   drxCfg->choice.setup->drx_RetransmissionTimerUL = convertDrxRetransmissionTimerUlValueToEnum(ueCb->duMacUeCfg.macCellGrpCfg.drxCfg.\
   drxRetransmissionTimerUl);
   drxCfg->choice.setup->drx_SlotOffset = ueCb->duMacUeCfg.macCellGrpCfg.drxCfg.drxSlotOffset;
   fillLongCycleOffsetValFromDuCb(ueCb->duMacUeCfg.macCellGrpCfg.drxCfg.drxLongCycleStartOffset, &drxCfg->choice.setup->drx_LongCycleStartOffset);
   
   if(ueCb->duMacUeCfg.macCellGrpCfg.drxCfg.shortDrxPres)
   {
      DU_ALLOC(drxCfg->choice.setup->shortDRX, sizeof(struct DRX_ConfigRrc__shortDRX));
      if(drxCfg->choice.setup->shortDRX)
      {
         drxCfg->choice.setup->shortDRX->drx_ShortCycle = convertShortDrxCycleLengthValueToEnum(ueCb->duMacUeCfg.macCellGrpCfg.drxCfg.\
         shortDrx.drxShortCycle);
         drxCfg->choice.setup->shortDRX->drx_ShortCycleTimer = ueCb->duMacUeCfg.macCellGrpCfg.drxCfg.shortDrx.drxShortCycleTimer;
      }
      else
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildDrxConfigRrc");
         return RFAILED;
      }
   }
   return ROK;
}
#endif
/*******************************************************************
 *
 * @brief Builds Mac cell group config 
 *
 * @details
 *
 *    Function : BuildMacCellGrpCfg 
 *
 *    Functionality: Builds Mac cell group config in DuToCuRrcContainer
 *
 * @params[in] MAC_CellGroupConfig_t *macCellGrpCfg
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildMacCellGrpCfg(DuUeCb *ueCb, MAC_CellGroupConfig_t *macCellGrpCfg)
{
   macCellGrpCfg->drx_ConfigRrc = NULLP;
#ifdef NR_DRX   
   if(ueCb)
   {
      DU_ALLOC(macCellGrpCfg->drx_ConfigRrc, sizeof(struct MAC_CellGroupConfig__drx_ConfigRrc));
      if(!macCellGrpCfg->drx_ConfigRrc)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildMacCellGrpCfg");
         return RFAILED;
      }
      if(BuildDrxConfigRrc(ueCb, macCellGrpCfg->drx_ConfigRrc) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : failed to build drx config");
         return RFAILED;
      }
   }
#endif
   macCellGrpCfg->schedulingRequestConfig = NULLP;
   DU_ALLOC(macCellGrpCfg->schedulingRequestConfig, sizeof(struct SchedulingRequestConfig));
   if(!macCellGrpCfg->schedulingRequestConfig)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildMacCellGrpCfg");
      return RFAILED;
   }

   if(BuildSchedulingReqConfig(ueCb, macCellGrpCfg->schedulingRequestConfig) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : BuildSchedulingReqConfig failed");
      return RFAILED;
   }

   macCellGrpCfg->bsr_Config = NULLP;
   DU_ALLOC(macCellGrpCfg->bsr_Config, sizeof(struct BSR_Config));
   if(!macCellGrpCfg->bsr_Config)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildMacCellGrpCfg");
      return RFAILED;
   }

   if(BuildBsrConfig(ueCb, macCellGrpCfg->bsr_Config) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : BuildBsrConfig failed");
      return RFAILED;
   }

   macCellGrpCfg->tag_Config = NULLP;
   DU_ALLOC(macCellGrpCfg->tag_Config, sizeof(struct TAG_Config));
   if(!macCellGrpCfg->tag_Config)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildMacCellGrpCfg");
      return RFAILED;
   }

   if(BuildTagConfig(ueCb, macCellGrpCfg->tag_Config) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : BuildTagConfig failed");
      return RFAILED;
   }

   macCellGrpCfg->phr_Config = NULLP;
   DU_ALLOC(macCellGrpCfg->phr_Config, sizeof(struct MAC_CellGroupConfig__phr_Config));
   if(!macCellGrpCfg->phr_Config)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildMacCellGrpCfg");
      return RFAILED;
   }

   if(BuildPhrConfig(ueCb, macCellGrpCfg->phr_Config) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : BuildPhrConfig failed");
      return RFAILED;
   }

   macCellGrpCfg->skipUplinkTxDynamic = false;
   macCellGrpCfg->ext1 = NULLP;

   return ROK;
}
/*******************************************************************
 *
 * @brief Frees memeory allocated for SearchSpcToAddModList
 *
 * @details
 *
 *    Function : FreeSearchSpcToAddModList
 *
 *    Functionality: Deallocating memory of SearchSpcToAddModList
 *
 * @params[in] struct PDCCH_Config__searchSpacesToAddModList *searchSpcList
 *
 * @return void
 *
 4221 * ****************************************************************/
void FreeSearchSpcToAddModList(struct PDCCH_Config__searchSpacesToAddModList *searchSpcList)
{
   uint8_t idx1=0;
   uint8_t idx2=0;
   struct  SearchSpace *searchSpc=NULLP;

   if(searchSpcList->list.array)
   {
      if(searchSpcList->list.array[idx2])
      {
	 searchSpc = searchSpcList->list.array[idx2];
	 if(searchSpc->controlResourceSetId)
	 {
	    if(searchSpc->monitoringSlotPeriodicityAndOffset)
	    {
	       if(searchSpc->monitoringSymbolsWithinSlot)
	       {
		  if(searchSpc->monitoringSymbolsWithinSlot->buf)
		  {
		     if(searchSpc->nrofCandidates)
		     {
			if(searchSpc->searchSpaceType)
			{
			   DU_FREE(searchSpc->searchSpaceType->choice.ue_Specific,\
				 sizeof(struct SearchSpace__searchSpaceType__ue_Specific));
			   DU_FREE(searchSpc->searchSpaceType, sizeof(struct
				    SearchSpace__searchSpaceType));
			}
			DU_FREE(searchSpc->nrofCandidates,
			      sizeof(struct SearchSpace__nrofCandidates));
		     }
		     DU_FREE(searchSpc->monitoringSymbolsWithinSlot->buf, \
			   searchSpc->monitoringSymbolsWithinSlot->size);
		  }
		  DU_FREE(searchSpc->monitoringSymbolsWithinSlot,
			sizeof(BIT_STRING_t));
	       }
	       DU_FREE(searchSpc->monitoringSlotPeriodicityAndOffset, \
		     sizeof(struct SearchSpace__monitoringSlotPeriodicityAndOffset));
	    }
	    DU_FREE(searchSpc->controlResourceSetId,
		  sizeof(ControlResourceSetId_t));
	 }
      }
      for(idx1 = 0; idx1 < searchSpcList->list.count; idx1++)
      {
	 DU_FREE(searchSpcList->list.array[idx1],
	       sizeof(struct SearchSpace));
      }
      DU_FREE(searchSpcList->list.array,searchSpcList->list.size);
   }
}
/*******************************************************************
 *
 * @brief Frees memory allocated for PdschTimeDomAllocList
 *
 * @details
 *
 *    Function : FreePdschTimeDomAllocList
 *
 *    Functionality: Deallocating memory of PdschTimeDomAllocList
 *
 * @params[in] struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAllocList
 *
 * @return void
 *
 * ****************************************************************/
void FreePdschTimeDomAllocList( struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAllocList)
{
   uint8_t idx1=0;

   if(timeDomAllocList->choice.setup)
   {
      if(timeDomAllocList->choice.setup->list.array)
      {
         for(idx1 = 0; idx1 <timeDomAllocList->choice.setup->list.count ; idx1++)
         {
            DU_FREE(timeDomAllocList->choice.setup->list.array[idx1]->k0, sizeof(long));
            DU_FREE(timeDomAllocList->choice.setup->list.array[idx1],
                  sizeof(struct PDSCH_TimeDomainResourceAllocation));
         }
         DU_FREE(timeDomAllocList->choice.setup->list.array, \
               timeDomAllocList->choice.setup->list.size);
      }
      DU_FREE(timeDomAllocList->choice.setup,\
            sizeof(struct PDSCH_TimeDomainResourceAllocationList));
   }
}
/*******************************************************************
 *
 * @brief Frees memory allocated for PuschTimeDomAllocList
 *
 *@details
 *
 *    Function : FreePuschTimeDomAllocList
 *
 *    Functionality: Deallocating memory of PuschTimeDomAllocList
 *
 * @params[in] PUSCH_Config_t *puschCfg
 *
 * @return void
 *
 * ****************************************************************/
void FreePuschTimeDomAllocList(PUSCH_Config_t *puschCfg)
{
   uint8_t rsrcListIdx=0;
   struct PUSCH_Config__pusch_TimeDomainAllocationList *timeDomAllocList_t=NULLP;

   if(puschCfg->pusch_TimeDomainAllocationList)
   {
      timeDomAllocList_t=puschCfg->pusch_TimeDomainAllocationList;
      if(timeDomAllocList_t->choice.setup)
      {
         if(timeDomAllocList_t->choice.setup->list.array)
         {
            for(rsrcListIdx = 0; rsrcListIdx<timeDomAllocList_t->choice.setup->list.count; rsrcListIdx++)
            {
               DU_FREE(timeDomAllocList_t->choice.setup->list.array[rsrcListIdx]->k2, sizeof(long));
               DU_FREE(timeDomAllocList_t->choice.setup->list.array[rsrcListIdx],\
                     sizeof(PUSCH_TimeDomainResourceAllocation_t));
            }
            DU_FREE(timeDomAllocList_t->choice.setup->list.array, \
                  timeDomAllocList_t->choice.setup->list.size);
         }
         DU_FREE(timeDomAllocList_t->choice.setup, \
               sizeof(struct PUSCH_TimeDomainResourceAllocationList));
      }
      DU_FREE(puschCfg->transformPrecoder, sizeof(long));
      DU_FREE(puschCfg->pusch_TimeDomainAllocationList, \
            sizeof(struct PUSCH_Config__pusch_TimeDomainAllocationList));
   }

}

/*******************************************************************
 *
 * @brief Frees memory allocated for Dedicated PUCCH config
 *
 * @details
 *
 *    Function : FreeBWPUlDedPucchCfg
 *
 *    Functionality: Deallocating memory of Dedicated PUCCH cfg
 *
 * @params[in] BWP_UplinkDedicated__pucch_Config *ulBwpPucchCfg
 *
 * @return void
 *
 * ****************************************************************/
void FreeBWPUlDedPucchCfg(struct BWP_UplinkDedicated__pucch_Config *ulBwpPucchCfg)
{
   uint8_t k1Idx, rsrcIdx, rsrcSetIdx;
   PUCCH_Config_t *pucchCfg = NULLP;
   PUCCH_ResourceSet_t *rsrcSet = NULLP;
   PUCCH_Resource_t *rsrc = NULLP;

   if(ulBwpPucchCfg)
   {
      if(ulBwpPucchCfg->choice.setup)
      {
         pucchCfg = ulBwpPucchCfg->choice.setup;

         //Free resource set list
         if(pucchCfg->resourceSetToAddModList)
         {
            if(pucchCfg->resourceSetToAddModList->list.array)
            {
               for(rsrcSetIdx=0; rsrcSetIdx < pucchCfg->resourceSetToAddModList->list.count; rsrcSetIdx++)
               {
                  rsrcSet = pucchCfg->resourceSetToAddModList->list.array[rsrcSetIdx];
                  if(rsrcSet->resourceList.list.array)
                  {
                     for(rsrcIdx=0; rsrcIdx < rsrcSet->resourceList.list.count; rsrcIdx++)
                     {
                        DU_FREE(rsrcSet->resourceList.list.array[rsrcIdx], sizeof(PUCCH_ResourceId_t));
                     }
                     DU_FREE(rsrcSet->resourceList.list.array, rsrcSet->resourceList.list.size);
                  }
                  DU_FREE(pucchCfg->resourceSetToAddModList->list.array[rsrcSetIdx], sizeof(PUCCH_ResourceSet_t));
               }
               DU_FREE(pucchCfg->resourceSetToAddModList->list.array, pucchCfg->resourceSetToAddModList->list.size);
            }
            DU_FREE(pucchCfg->resourceSetToAddModList, sizeof(struct PUCCH_Config__resourceSetToAddModList));
         }

         //Free resource list
         if(pucchCfg->resourceToAddModList)
         {
            if(pucchCfg->resourceToAddModList->list.array)
            {
               for(rsrcIdx=0; rsrcIdx < pucchCfg->resourceToAddModList->list.count; rsrcIdx++)
               {
                  rsrc = pucchCfg->resourceToAddModList->list.array[rsrcIdx];
                  DU_FREE(rsrc->format.choice.format1, sizeof(PUCCH_format1_t));
                  DU_FREE(pucchCfg->resourceToAddModList->list.array[rsrcIdx], sizeof(PUCCH_Resource_t));
               }
               DU_FREE(pucchCfg->resourceToAddModList->list.array, pucchCfg->resourceToAddModList->list.size);
            }
            DU_FREE(pucchCfg->resourceToAddModList, sizeof(struct PUCCH_Config__resourceToAddModList));
         }

         //PUCCH Format 1
         if(pucchCfg->format1)
         {
            if(pucchCfg->format1->choice.setup)
            {
               DU_FREE(pucchCfg->format1->choice.setup->nrofSlots, sizeof(long));
               DU_FREE(pucchCfg->format1->choice.setup, sizeof(PUCCH_FormatConfig_t));
            }
            DU_FREE(pucchCfg->format1, sizeof(struct PUCCH_Config__format1));
         }
         
         //DL DATA TO UL ACK
         if(pucchCfg->dl_DataToUL_ACK)
         {
            if(pucchCfg->dl_DataToUL_ACK->list.array)
            {
               for(k1Idx = 0; k1Idx <  pucchCfg->dl_DataToUL_ACK->list.count; k1Idx++)
               {
                  DU_FREE(pucchCfg->dl_DataToUL_ACK->list.array[k1Idx], sizeof(long));
               }
               DU_FREE(pucchCfg->dl_DataToUL_ACK->list.array, pucchCfg->dl_DataToUL_ACK->list.size);
            }
            DU_FREE(pucchCfg->dl_DataToUL_ACK, sizeof(struct PUCCH_Config__dl_DataToUL_ACK));
         }

         DU_FREE(ulBwpPucchCfg->choice.setup, sizeof(PUCCH_Config_t));
      }
      DU_FREE(ulBwpPucchCfg, sizeof(struct BWP_UplinkDedicated__pucch_Config));
   }
}

/*******************************************************************
 *
 * @brief Frees memory allocated for InitialUlBWP
 *
 * @details
 *
 *    Function : FreeInitialUlBWP
 *
 *    Functionality: Deallocating memory of InitialUlBWP
 *
 * @params[in] BWP_UplinkDedicated_t *ulBwp
 *
 * @return void
 *
 * ****************************************************************/
void FreeInitialUlBWP(BWP_UplinkDedicated_t *ulBwp)
{
   uint8_t  rSetIdx, rsrcIdx;
   SRS_Config_t   *srsCfg = NULLP;
   PUSCH_Config_t *puschCfg = NULLP;
   struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA *dmrsUlCfg = NULLP;
   struct SRS_Config__srs_ResourceSetToAddModList *rsrcSetList = NULLP;
   struct SRS_ResourceSet__srs_ResourceIdList *rsrcIdList = NULLP;
   struct SRS_Config__srs_ResourceToAddModList *resourceList = NULLP;

   FreeBWPUlDedPucchCfg(ulBwp->pucch_Config);

   if(ulBwp->pusch_Config)
   {
      if(ulBwp->pusch_Config->choice.setup)
      {
         puschCfg=ulBwp->pusch_Config->choice.setup;
         if(puschCfg->dataScramblingIdentityPUSCH)
         {
            if(puschCfg->dmrs_UplinkForPUSCH_MappingTypeA)
            {
               FreePuschTimeDomAllocList(puschCfg);
               dmrsUlCfg=puschCfg->dmrs_UplinkForPUSCH_MappingTypeA;
               if(dmrsUlCfg->choice.setup)
               {
                  if(dmrsUlCfg->choice.setup->dmrs_AdditionalPosition)
                  {
                     if(dmrsUlCfg->choice.setup->transformPrecodingDisabled)
                     {
                        DU_FREE(dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0,\
                              sizeof(long));
                        DU_FREE(dmrsUlCfg->choice.setup->transformPrecodingDisabled,
                              sizeof(struct DMRS_UplinkConfig__transformPrecodingDisabled));
                     }
                     DU_FREE(dmrsUlCfg->choice.setup->dmrs_AdditionalPosition,
                           sizeof(long));
                  }
                  DU_FREE(dmrsUlCfg->choice.setup,sizeof(DMRS_UplinkConfig_t));
               }
               DU_FREE(puschCfg->dmrs_UplinkForPUSCH_MappingTypeA, \
                     sizeof(struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA));
            }
            DU_FREE(puschCfg->dataScramblingIdentityPUSCH, sizeof(long));
         }
         DU_FREE(ulBwp->pusch_Config->choice.setup, sizeof(PUSCH_Config_t));
      }
      DU_FREE(ulBwp->pusch_Config, sizeof(struct BWP_UplinkDedicated__pusch_Config));

      /* Free SRS-Config */
      if(ulBwp->srs_Config)
      {
         if(ulBwp->srs_Config->choice.setup)
         {
            srsCfg = ulBwp->srs_Config->choice.setup;

            /* Free Resource Set to add/mod list */
            if(srsCfg->srs_ResourceSetToAddModList)
            {
               rsrcSetList = srsCfg->srs_ResourceSetToAddModList;
               if(rsrcSetList->list.array)
               {
                  rSetIdx = 0;

                  /* Free SRS resource Id list in this SRS resource set */
                  if(rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList)
                  {
                     rsrcIdList = rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList;

                     if(rsrcIdList->list.array)
                     {
                        for(rsrcIdx = 0; rsrcIdx < rsrcIdList->list.count; rsrcIdx++)
                        {
                           DU_FREE(rsrcIdList->list.array[rsrcIdx], sizeof(SRS_ResourceId_t));
                        }
                        DU_FREE(rsrcIdList->list.array, rsrcIdList->list.size);
                     }
                     DU_FREE(rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList,\
                           sizeof(struct SRS_ResourceSet__srs_ResourceIdList));
                  }

                  /* Free resource type info for this SRS resource set */
                  DU_FREE(rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic, \
                        sizeof(struct SRS_ResourceSet__resourceType__aperiodic));

                  /* Free memory for each resource set */
                  for(rSetIdx = 0; rSetIdx < rsrcSetList->list.count; rSetIdx++)
                  {
                     DU_FREE(rsrcSetList->list.array[rSetIdx], sizeof(SRS_ResourceSet_t));
                  }
                  DU_FREE(rsrcSetList->list.array, rsrcSetList->list.size); 
               }
               DU_FREE(srsCfg->srs_ResourceSetToAddModList, \
                     sizeof(struct SRS_Config__srs_ResourceSetToAddModList));
            }

            /* Free resource to add/modd list */
            if(srsCfg->srs_ResourceToAddModList)
            {
               resourceList = srsCfg->srs_ResourceToAddModList;
               if(resourceList->list.array)
               {
                  rsrcIdx = 0;
                  DU_FREE(resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2,\
                        sizeof(struct SRS_Resource__transmissionComb__n2));
                  DU_FREE(resourceList->list.array[rsrcIdx]->resourceType.choice.aperiodic,\
                        sizeof(struct SRS_Resource__resourceType__aperiodic));

                  for(rsrcIdx = 0; rsrcIdx < resourceList->list.count; rsrcIdx++)
                  {
                     DU_FREE(resourceList->list.array[rsrcIdx], sizeof(SRS_Resource_t));
                  }
                  DU_FREE(resourceList->list.array, resourceList->list.size);
               }
               DU_FREE(srsCfg->srs_ResourceToAddModList, \
                     sizeof(struct SRS_Config__srs_ResourceToAddModList));
            }

            DU_FREE(ulBwp->srs_Config->choice.setup, sizeof(SRS_Config_t));
         }
         DU_FREE(ulBwp->srs_Config, sizeof(struct BWP_UplinkDedicated__srs_Config));
      }
   }
}	
/*******************************************************************
 *
 * @brief Frees memory allocated for initialUplinkBWP
 *
 * @details
 *
 *    Function : FreeinitialUplinkBWP
 *
 *    Functionality: Deallocating memory of initialUplinkBWP
 *
 * @params[in] UplinkConfig_t *ulCfg
 *
 * @return void
 *         
 *
 * ****************************************************************/
void FreeinitialUplinkBWP(UplinkConfig_t *ulCfg)
{
   BWP_UplinkDedicated_t *ulBwp=NULLP; 
   struct UplinkConfig__pusch_ServingCellConfig *puschCfg=NULLP;

   if(ulCfg->initialUplinkBWP)
   {
      ulBwp=ulCfg->initialUplinkBWP;
      if(ulCfg->firstActiveUplinkBWP_Id)
      {
	 if(ulCfg->pusch_ServingCellConfig)
	 {
	    puschCfg=ulCfg->pusch_ServingCellConfig;
	    if(puschCfg->choice.setup)
	    {
	       if(puschCfg->choice.setup->ext1)
	       {
		  DU_FREE(puschCfg->choice.setup->ext1->\
			processingType2Enabled,sizeof(BOOLEAN_t));
		  DU_FREE(puschCfg->choice.setup->ext1->\
			maxMIMO_Layers,sizeof(long));
		  DU_FREE(puschCfg->choice.setup->ext1, \
			sizeof(struct PUSCH_ServingCellConfig__ext1));
	       }
	       DU_FREE(puschCfg->choice.setup, sizeof(struct PUSCH_ServingCellConfig));
	    }
	    DU_FREE(ulCfg->pusch_ServingCellConfig, sizeof(struct UplinkConfig__pusch_ServingCellConfig));
	 }
	 DU_FREE(ulCfg->firstActiveUplinkBWP_Id, sizeof(BWP_Id_t));
      }
      FreeInitialUlBWP(ulBwp);
      DU_FREE(ulCfg->initialUplinkBWP, sizeof(BWP_UplinkDedicated_t));
   }
}
/*******************************************************************
 *
 * @brief Frees emmory allocated for BWPDlDedPdschCfg
 *
 * @details
 *
 *    Function : FreeBWPDlDedPdschCfg
 *
 *    Functionality: Deallocating memory of BWPDlDedPdschCfg
 *
 * @params[in] BWP_DownlinkDedicated_t *dlBwp
 *
 * @return void
 *
 *
 * ****************************************************************/
void FreeBWPDlDedPdschCfg(BWP_DownlinkDedicated_t *dlBwp)
{
   struct PDSCH_Config *pdschCfg=NULLP;
   struct PDSCH_Config__prb_BundlingType *prbBndlType=NULLP;
   struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAllocList=NULLP;
   struct PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA *dmrsDlCfg=NULLP;

   if(dlBwp->pdsch_Config->choice.setup)
   {
      pdschCfg=dlBwp->pdsch_Config->choice.setup;
      if(pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA)
      {
	 if(pdschCfg->pdsch_TimeDomainAllocationList)
	 {
	    timeDomAllocList=pdschCfg->pdsch_TimeDomainAllocationList;
	    if(pdschCfg->maxNrofCodeWordsScheduledByDCI)
	    {
	       prbBndlType=&pdschCfg->prb_BundlingType;
	       DU_FREE(prbBndlType->choice.staticBundling,\
		     sizeof(struct PDSCH_Config__prb_BundlingType__staticBundling));
	       DU_FREE(pdschCfg->maxNrofCodeWordsScheduledByDCI, sizeof(long));
	    }
	    FreePdschTimeDomAllocList(timeDomAllocList);
	    DU_FREE(pdschCfg->pdsch_TimeDomainAllocationList, \
		  sizeof(struct PDSCH_Config__pdsch_TimeDomainAllocationList));
	 }
	 dmrsDlCfg=pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA;
	 if(dmrsDlCfg->choice.setup)
	 {
	    DU_FREE(dmrsDlCfg->choice.setup->dmrs_AdditionalPosition,
		  sizeof(long));
	    DU_FREE(dmrsDlCfg->choice.setup, sizeof(struct DMRS_DownlinkConfig));
	 }
	 DU_FREE(pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA, \
	       sizeof(struct PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA));
      }
      DU_FREE(dlBwp->pdsch_Config->choice.setup, sizeof(struct PDSCH_Config));
   }
}
/*******************************************************************
 *
 * @brief Frees emmory allocated for BWPDlDedPdcchCfg
 *
 * @details
 *
 *    Function : FreeBWPDlDedPdcchCfg
 *
 *    Functionality: Deallocating memory of BWPDlDedPdcchCfg
 *
 * @params[in] BWP_DownlinkDedicated_t *dlBwp
 *
 * @return void
 *         
 *
 * ****************************************************************/
void FreeBWPDlDedPdcchCfg(BWP_DownlinkDedicated_t *dlBwp)
{
   uint8_t idx1=0;
   uint8_t idx2=0;
   struct PDCCH_Config *pdcchCfg=NULLP;
   struct ControlResourceSet *controlRSet=NULLP;
   struct PDCCH_Config__controlResourceSetToAddModList *controlRSetList=NULLP;

   if(dlBwp->pdcch_Config->choice.setup)
   {
      pdcchCfg=dlBwp->pdcch_Config->choice.setup;
      if(pdcchCfg->controlResourceSetToAddModList)
      {
	 controlRSetList = pdcchCfg->controlResourceSetToAddModList;
	 if(controlRSetList->list.array)
	 {
	    controlRSet = controlRSetList->list.array[idx2];
	    if(controlRSet)
	    {
	       if(controlRSet->frequencyDomainResources.buf)
	       {
		  if(controlRSet->pdcch_DMRS_ScramblingID)
		  {
		     if(pdcchCfg->searchSpacesToAddModList)
		     {
			FreeSearchSpcToAddModList(pdcchCfg->searchSpacesToAddModList);
			DU_FREE(pdcchCfg->searchSpacesToAddModList, \
			      sizeof(struct PDCCH_Config__searchSpacesToAddModList));
		     }
		     DU_FREE(controlRSet->pdcch_DMRS_ScramblingID, sizeof(long));
		  }
		  DU_FREE(controlRSet->frequencyDomainResources.buf, \
			controlRSet->frequencyDomainResources.size);
	       }
	    }
	    for(idx1 = 0; idx1 <controlRSetList->list.count; idx1++)
	    {
	       DU_FREE(controlRSetList->list.array[idx1], sizeof(struct ControlResourceSet));
	    }
	    DU_FREE(controlRSetList->list.array, controlRSetList->list.size);
	 }
	 DU_FREE(pdcchCfg->controlResourceSetToAddModList, \
	       sizeof(struct PDCCH_Config__controlResourceSetToAddModList));
      }
      DU_FREE(dlBwp->pdcch_Config->choice.setup, sizeof(struct PDCCH_Config));
   }
}	

/*******************************************************************
 *
 * @brief Free SCS specific carrier list in DL frequency info
 *
 * @details
 *
 *    Function : FreeScsSpecificCarrierListDl
 *
 *    Functionality: Free SCS specific carrier list in DL frequency info
 *
 * @params[in] Pointer to struct FrequencyInfoDL__scs_SpecificCarrierList
 *
 * @return void
 *
 * ****************************************************************/
void FreeScsSpecificCarrierListDl(struct FrequencyInfoDL__scs_SpecificCarrierList *scsCarrierList)
{
   uint8_t listIdx = 0;

   if(!scsCarrierList->list.array)
   {
      for(listIdx = 0; listIdx < scsCarrierList->list.count; listIdx++)
      {
         DU_FREE(scsCarrierList->list.array[listIdx], sizeof(SCS_SpecificCarrier_t));
      }
      DU_FREE(scsCarrierList->list.array, scsCarrierList->list.size);
   }
}

/*******************************************************************
 *
 * @brief Free DL frequency info in DL config common
 *
 * @details
 *
 *    Function : FreeFreqInfoDl
 *
 *    Functionality: Free DL frequency info in DL config common
 *
 * @params[in] Pointer to DownlinkConfigCommon_t
 *
 * @return void
 *
 * ****************************************************************/
void FreeFreqInfoDl(FrequencyInfoDL_t *frequencyInfoDL)
{
   uint8_t freqBandIdx = 0;

   /* SSB Absolute Frequency */
   DU_FREE(frequencyInfoDL->absoluteFrequencySSB, sizeof(ARFCN_ValueNR_t));

   /* NR Multi Frequency Band List */
   if(frequencyInfoDL->frequencyBandList.list.array)
   {
      for(freqBandIdx = 0; freqBandIdx < frequencyInfoDL->frequencyBandList.list.count; freqBandIdx++)
      {
         DU_FREE(frequencyInfoDL->frequencyBandList.list.array[freqBandIdx], sizeof(FreqBandIndicatorNR_t));
      }
      DU_FREE(frequencyInfoDL->frequencyBandList.list.array, frequencyInfoDL->frequencyBandList.list.size);
   }

   /* Subcarrier Spacing specifc carrier List */
   FreeScsSpecificCarrierListDl(&frequencyInfoDL->scs_SpecificCarrierList);
}

/*******************************************************************
 *
 * @brief Free DL config common in Serving cell config common
 *
 * @details
 *
 *    Function : FreeDlConfigCommon
 *
 *    Functionality: Free DL config common in Serving cell config common
 *
 * @params[in] Pointer to DownlinkConfigCommon_t
 *
 * @return void
 *
 * ****************************************************************/
void FreeDlConfigCommon(DownlinkConfigCommon_t *dlCfgCommon)
{
   /* DL Frequency Info */
   if(dlCfgCommon->frequencyInfoDL)
   {
      FreeFreqInfoDl(dlCfgCommon->frequencyInfoDL);
      DU_FREE(dlCfgCommon->frequencyInfoDL, sizeof(FrequencyInfoDL_t));
   }

   /* DL BWP config common */
   if(dlCfgCommon->initialDownlinkBWP)
   {
      FreeBwpDlCommon(dlCfgCommon->initialDownlinkBWP);
      DU_FREE(dlCfgCommon->initialDownlinkBWP, sizeof(BWP_DownlinkCommon_t));
   }
}

/*******************************************************************
 *
 * @brief Free SCS specific carrier list in UL frequency Info
 *
 * @details
 *
 *    Function : FreeScsSpecificCarrierListUl
 *
 *    Functionality: Free SCS specific carrier list in UL frequency Info
 *
 * @params[in] Pointer to struct FrequencyInfoUL__scs_SpecificCarrierList
 *
 * @return void
 *
 * ****************************************************************/
void FreeScsSpecificCarrierListUl(struct FrequencyInfoUL__scs_SpecificCarrierList *scsCarrierList)
{
   uint8_t listIdx = 0;

   if(scsCarrierList->list.array)
   {
      for(listIdx = 0; listIdx < scsCarrierList->list.count; listIdx++)
      {
         DU_FREE(scsCarrierList->list.array[listIdx], sizeof(SCS_SpecificCarrier_t));
      }
      DU_FREE(scsCarrierList->list.array, scsCarrierList->list.size);
   }
}

/*******************************************************************
 *
 * @brief Free frequency info in UL config common
 *
 * @details
 *
 *    Function : FreeFreqInfoUl
 *
 *    Functionality: Free frequency info in UL config common
 *
 * @params[in] Pointer to FrequencyInfoUL_t
 *
 * @return void
 *
 * ****************************************************************/
void FreeFreqInfoUl(FrequencyInfoUL_t *frequencyInfoUL)
{
   uint8_t listIdx= 0;

   /* NR Multi Frequency Band List */
   if(!frequencyInfoUL->frequencyBandList)
   {
      if(frequencyInfoUL->frequencyBandList->list.array)
      {
         for(listIdx = 0; listIdx < frequencyInfoUL->frequencyBandList->list.count; listIdx++)
         {
            DU_FREE(frequencyInfoUL->frequencyBandList->list.array[listIdx], sizeof(FreqBandIndicatorNR_t));
         }
         DU_FREE(frequencyInfoUL->frequencyBandList->list.array, frequencyInfoUL->frequencyBandList->list.size);
      }
      DU_FREE(frequencyInfoUL->frequencyBandList, sizeof(MultiFrequencyBandListNR_t));
   }

   /* Absolute frequency point A */
   DU_FREE(frequencyInfoUL->absoluteFrequencyPointA, sizeof(ARFCN_ValueNR_t));

   /* Subcarrier Spacing specifc carrier */
   FreeScsSpecificCarrierListUl(&frequencyInfoUL->scs_SpecificCarrierList);

   /* P-MAX */
   DU_FREE(frequencyInfoUL->p_Max, sizeof(P_Max_t));
}

/*******************************************************************
 *
 * @brief Free UL config common in Serving cell config common
 *
 * @details
 *
 *    Function : FreeUlConfigCommon
 *
 *    Functionality: Free UL config common in Serving cell config common
 *
 * @params[in] Pointer to UplinkConfigCommon_t
 *
 * @return void
 *
 * ****************************************************************/
void FreeUlConfigCommon(UplinkConfigCommon_t *ulCfgCommon)
{
   /* UL Frequency Info */
   if(ulCfgCommon->frequencyInfoUL)
   {
      FreeFreqInfoUl(ulCfgCommon->frequencyInfoUL);
      DU_FREE(ulCfgCommon->frequencyInfoUL, sizeof(FrequencyInfoUL_t));
   }

   /* UL BWP common */
   if(ulCfgCommon->initialUplinkBWP)
   {
      FreeBwpUlCommon(ulCfgCommon->initialUplinkBWP);
      DU_FREE(ulCfgCommon->initialUplinkBWP, sizeof(BWP_UplinkCommon_t));
   }
}

/*******************************************************************
 *
 * @brief Free SP cell config common in Reconfig with Sync
 *
 * @details
 *
 *    Function : FreeSpCellConfigCommon
 *
 *    Functionality: Free SP cell config common in Reconfig with Sync
 *
 * @params[in] Pointer to ServingCellConfigCommon_t
 *
 * @return void
 *
 * ****************************************************************/
void FreeSpCellConfigCommon(ServingCellConfigCommon_t *spCellConfigCommon)
{
   /* Free Physical cell identity */
   DU_FREE(spCellConfigCommon->physCellId, sizeof(PhysCellId_t));

   /* Free Downlink Config common */
   if(spCellConfigCommon->downlinkConfigCommon)
   {
      FreeDlConfigCommon(spCellConfigCommon->downlinkConfigCommon);
      DU_FREE(spCellConfigCommon->downlinkConfigCommon, sizeof(DownlinkConfigCommon_t));
   }

   /* Free Uplink Config common */
   if(spCellConfigCommon->uplinkConfigCommon)
   {
      FreeUlConfigCommon(spCellConfigCommon->uplinkConfigCommon);
      DU_FREE(spCellConfigCommon->uplinkConfigCommon, sizeof(UplinkConfigCommon_t));
   }

   /* Free Timing Advance offset */
   DU_FREE(spCellConfigCommon->n_TimingAdvanceOffset, sizeof(long));

   /* Free SSB Position in Burst */
   if(spCellConfigCommon->ssb_PositionsInBurst)
   {
      DU_FREE(spCellConfigCommon->ssb_PositionsInBurst->choice.mediumBitmap.buf, \
         spCellConfigCommon->ssb_PositionsInBurst->choice.mediumBitmap.size);
      DU_FREE(spCellConfigCommon->ssb_PositionsInBurst, sizeof(struct ServingCellConfigCommon__ssb_PositionsInBurst));
   }

   /* Free SSB Periodicity in Serving cell */
   DU_FREE(spCellConfigCommon->ssb_periodicityServingCell, sizeof(long));

   /* Free SSB subcarrier spacing */
   DU_FREE(spCellConfigCommon->ssbSubcarrierSpacing, sizeof(SubcarrierSpacing_t));

   /* TDD UL-DL configuration common */
   DU_FREE(spCellConfigCommon->tdd_UL_DL_ConfigurationCommon, sizeof(TDD_UL_DL_ConfigCommon_t));
}

/*******************************************************************
 *
 * @brief Free dedicated RACH configuration in Reconfiguration with sync
 *
 * @details
 *
 *    Function : FreeRecfgWithSync
 *
 *    Functionality:
 *       Free dedicated RACH configuration in Reconfiguration with sync
 *
 * @params[in] Pinter to Rach config dedicated struct
 *
 * @return void
 *
 * ****************************************************************/
void FreeRachConfigDedicated(struct ReconfigurationWithSync__rach_ConfigDedicated *rachCfgDed)
{
   uint8_t listIdx = 0;
   CFRA_t *cfra = NULLP;
   struct CFRA__resources__ssb *ssbResource = NULLP;

   /* Uplink */
   if(rachCfgDed->choice.uplink)
   {
      /* CFRA : Contention free Random Access */
      if(rachCfgDed->choice.uplink->cfra)
      {
         cfra = rachCfgDed->choice.uplink->cfra;

         /* CFRA occassions */
         if(cfra->occasions)
         {
            /* CFRA occassions : SSB per RACH occasion */
            DU_FREE(cfra->occasions->ssb_perRACH_Occasion, sizeof(long));
            DU_FREE(cfra->occasions, sizeof(struct CFRA__occasions));
         }

         /* CFRA resource */
         cfra->resources.present = CFRA__resources_PR_ssb;

         /* CFRA resource : SSB */
         if(cfra->resources.choice.ssb)
         {
            ssbResource = cfra->resources.choice.ssb;

            /* CFRA SSB resource list */
            if(ssbResource->ssb_ResourceList.list.array)
            {
               for(listIdx = 0; listIdx < ssbResource->ssb_ResourceList.list.count; listIdx++)
               {
                  DU_FREE(ssbResource->ssb_ResourceList.list.array[listIdx], sizeof(CFRA_SSB_Resource_t));
               }
               DU_FREE(ssbResource->ssb_ResourceList.list.array, ssbResource->ssb_ResourceList.list.size);
            }
            DU_FREE(cfra->resources.choice.ssb, sizeof(struct CFRA__resources__ssb));
         }
         DU_FREE(rachCfgDed->choice.uplink->cfra, sizeof(CFRA_t));
      }
      DU_FREE(rachCfgDed->choice.uplink, sizeof(RACH_ConfigDedicated_t));
   }
}

/*******************************************************************
 *
 * @brief Frees reconfiguration with sync in SP cell config
 *
 * @details
 *
 *    Function : FreeRecfgWithSync
 *
 *    Functionality: Fress reconfiguration with sync in SP cell config
 *
 * @params[in] Pointer to ReconfigurationWithSync_t
 *
 * @return void
 *
 * ****************************************************************/
void FreeRecfgWithSync(ReconfigurationWithSync_t *recfgWithSync)
{
   /* Free SP Cell config common */
   if(recfgWithSync->spCellConfigCommon)
   {
      FreeSpCellConfigCommon(recfgWithSync->spCellConfigCommon);
      DU_FREE(recfgWithSync->spCellConfigCommon, sizeof(ServingCellConfigCommon_t));
   }

   /* Free Dedicated RACH configuration */
   if(recfgWithSync->rach_ConfigDedicated)
   {
      FreeRachConfigDedicated(recfgWithSync->rach_ConfigDedicated);
      DU_FREE(recfgWithSync->rach_ConfigDedicated, sizeof(struct ReconfigurationWithSync__rach_ConfigDedicated));
   }
}

/*******************************************************************
 *
 * @brief Frees emmory allocated for DUToCURRCContainer 
 *
 * @details
 *
 *    Function : FreeMemDuToCuRrcCont
 *
 *    Functionality: Deallocating memory of DuToCuRrcContainer
 *
 * @params[in] DuToCuRRCContainer, DuToCuRRCContainer
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t FreeMemDuToCuRrcCont(CellGroupConfigRrc_t *cellGrpCfg)
{
   uint8_t idx=0;
   SpCellConfig_t *spCellCfg=NULLP;
   ServingCellConfig_t *srvCellCfg=NULLP;
   BWP_DownlinkDedicated_t *dlBwp=NULLP;
   MAC_CellGroupConfig_t *macCellGrpCfg=NULLP;
   PhysicalCellGroupConfig_t *phyCellGrpCfg=NULLP;
   struct CellGroupConfigRrc__rlc_BearerToAddModList *rlcBearerList=NULLP;
   struct RLC_Config *rlcConfig=NULLP;
   struct LogicalChannelConfig *macLcConfig=NULLP;
   struct MAC_CellGroupConfig__drx_ConfigRrc *drxCfg=NULLP;
   struct SchedulingRequestConfig *schedulingRequestConfig=NULLP;
   struct SchedulingRequestConfig__schedulingRequestToAddModList *schReqList=NULLP;
   struct TAG_Config *tagConfig=NULLP;
   struct TAG_Config__tag_ToAddModList *tagList=NULLP;
   struct MAC_CellGroupConfig__phr_Config *phrConfig=NULLP;
   struct ServingCellConfig__pdsch_ServingCellConfig *pdschCfg=NULLP;

   rlcBearerList = cellGrpCfg->rlc_BearerToAddModList;
   if(rlcBearerList)
   {
      if(rlcBearerList->list.array)
      {
         for(idx=0; idx<rlcBearerList->list.count; idx++)
         {
            if(rlcBearerList->list.array[idx])
            {  
               rlcConfig   = rlcBearerList->list.array[idx]->rlc_Config;
               macLcConfig = rlcBearerList->list.array[idx]->mac_LogicalChannelConfig;
               if(rlcConfig)
               {
                  switch(rlcConfig->present)
                  {
                     case RLC_Config_PR_NOTHING:
                        break;

                     case RLC_Config_PR_am:
                        {
                           if(rlcConfig->choice.am)
                           {
                              DU_FREE(rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength, sizeof(SN_FieldLengthAM_t));
                              DU_FREE(rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength, sizeof(SN_FieldLengthAM_t)); 
                              DU_FREE(rlcConfig->choice.am, sizeof(struct RLC_Config__am));
                           }
                           break;
                        }
                     case RLC_Config_PR_um_Bi_Directional:
                        {
                           if(rlcConfig->choice.um_Bi_Directional)
                           {
                              DU_FREE(rlcConfig->choice.um_Bi_Directional->dl_UM_RLC.sn_FieldLength, sizeof(SN_FieldLengthUM_t)); 
                              DU_FREE(rlcConfig->choice.um_Bi_Directional->ul_UM_RLC.sn_FieldLength, sizeof(SN_FieldLengthUM_t));
                              DU_FREE(rlcConfig->choice.um_Bi_Directional, sizeof(struct RLC_Config__um_Bi_Directional));
                           }
                           break;
                        }
                     case RLC_Config_PR_um_Uni_Directional_UL:
                        {
                           if(rlcConfig->choice.um_Uni_Directional_UL)
                           {
                              DU_FREE(rlcConfig->choice.um_Uni_Directional_UL->ul_UM_RLC.sn_FieldLength, sizeof(SN_FieldLengthUM_t));
                              DU_FREE(rlcConfig->choice.um_Uni_Directional_UL , sizeof(struct RLC_Config__um_Uni_Directional_UL));
                           }
                           break;
                        }
                     case RLC_Config_PR_um_Uni_Directional_DL:
                        {
                           if(rlcConfig->choice.um_Uni_Directional_DL )
                           {
                              DU_FREE(rlcConfig->choice.um_Uni_Directional_DL->dl_UM_RLC.sn_FieldLength, sizeof(SN_FieldLengthUM_t));
                              DU_FREE(rlcConfig->choice.um_Uni_Directional_DL , sizeof(struct RLC_Config__um_Uni_Directional_DL));
                           }
                           break;
                        }
                  }	
                  DU_FREE(rlcConfig, sizeof(struct RLC_Config));
               }
               DU_FREE(rlcBearerList->list.array[idx]->servedRadioBearer, sizeof(struct RLC_BearerConfig__servedRadioBearer));
               if(macLcConfig)
               {
                  if(macLcConfig->ul_SpecificParameters)
                  {
                     DU_FREE(macLcConfig->ul_SpecificParameters->schedulingRequestID,	sizeof(SchedulingRequestId_t));
                     DU_FREE(macLcConfig->ul_SpecificParameters->logicalChannelGroup,	sizeof(long));
                     DU_FREE(macLcConfig->ul_SpecificParameters, sizeof(struct LogicalChannelConfig__ul_SpecificParameters));
                  }
                  DU_FREE(rlcBearerList->list.array[idx]->mac_LogicalChannelConfig, sizeof(struct LogicalChannelConfig));
               }
               DU_FREE(rlcBearerList->list.array[idx], sizeof(struct RLC_BearerConfig));
            }	
         }
         DU_FREE(rlcBearerList->list.array, rlcBearerList->list.size);
      }
      DU_FREE(cellGrpCfg->rlc_BearerToAddModList, sizeof(struct CellGroupConfigRrc__rlc_BearerToAddModList));
   }

   macCellGrpCfg = cellGrpCfg->mac_CellGroupConfig;
   if(macCellGrpCfg)
   {
      drxCfg = macCellGrpCfg->drx_ConfigRrc;

      if(drxCfg)
      {
          switch(drxCfg->present)
          {
            case MAC_CellGroupConfig__drx_ConfigRrc_PR_NOTHING:
               break;
            case MAC_CellGroupConfig__drx_ConfigRrc_PR_setup:
            {
               if(drxCfg->choice.setup)
               {
                  DU_FREE(drxCfg->choice.setup->shortDRX, sizeof(struct DRX_ConfigRrc__shortDRX));  
                  DU_FREE(drxCfg->choice.setup, sizeof(struct DRX_ConfigRrc));
               }
            }
            case MAC_CellGroupConfig__drx_ConfigRrc_PR_release:
               break;
          }
          DU_FREE(drxCfg, sizeof(struct MAC_CellGroupConfig__drx_ConfigRrc));
      }
      schedulingRequestConfig = macCellGrpCfg->schedulingRequestConfig; 
      if(schedulingRequestConfig)
      {
         schReqList = schedulingRequestConfig->schedulingRequestToAddModList;
         if(schReqList)
         {
            if(schReqList->list.array)
            {
               for(idx=0;idx<schReqList->list.count; idx++)
               {
                  if(schReqList->list.array[idx])
                  {
                     DU_FREE(schReqList->list.array[idx]->sr_ProhibitTimer, sizeof(long));
                     DU_FREE(schReqList->list.array[idx], sizeof(struct SchedulingRequestToAddMod));
                  }
               }
               DU_FREE(schReqList->list.array, schReqList->list.size);
            }
            DU_FREE(schedulingRequestConfig->schedulingRequestToAddModList,\
                  sizeof(struct SchedulingRequestConfig__schedulingRequestToAddModList));    }
            DU_FREE(macCellGrpCfg->schedulingRequestConfig, sizeof(struct SchedulingRequestConfig));
      }
      if(macCellGrpCfg->bsr_Config)
      {
         DU_FREE(macCellGrpCfg->bsr_Config->logicalChannelSR_DelayTimer, sizeof(long));
         DU_FREE(macCellGrpCfg->bsr_Config, sizeof(struct BSR_Config));
      }
      tagConfig = macCellGrpCfg->tag_Config;
      if(tagConfig)
      {
         tagList = tagConfig->tag_ToAddModList;
         if(tagList)
         {
            if(tagList->list.array)
            {
               for(idx=0; idx<tagList->list.count; idx++)
               {
                  DU_FREE(tagList->list.array[idx], sizeof(struct TAG));
               }
               DU_FREE(tagList->list.array, tagList->list.size);
            }
            DU_FREE(tagConfig->tag_ToAddModList, sizeof(struct TAG_Config__tag_ToAddModList));
         }
         DU_FREE(tagConfig, sizeof(struct TAG_Config));
      }

      phrConfig = macCellGrpCfg->phr_Config;
      if(phrConfig)
      {
         DU_FREE(phrConfig->choice.setup, sizeof(struct PHR_Config));
         DU_FREE(phrConfig, sizeof(struct MAC_CellGroupConfig__phr_Config));
      }

      DU_FREE(macCellGrpCfg, sizeof(MAC_CellGroupConfig_t));
   }

   phyCellGrpCfg = cellGrpCfg->physicalCellGroupConfig;
   if(phyCellGrpCfg)
   {
      DU_FREE(phyCellGrpCfg->p_NR_FR1, sizeof(long));
      DU_FREE(phyCellGrpCfg, sizeof(PhysicalCellGroupConfig_t));
   }

   spCellCfg = cellGrpCfg->spCellConfig;
   if(spCellCfg)
   {
      /* Free serving cell index */
      DU_FREE(spCellCfg->servCellIndex, sizeof(long));

      /* Free Reconfiguration with sync */
      if(spCellCfg->reconfigurationWithSync)
      {
         FreeRecfgWithSync(spCellCfg->reconfigurationWithSync);
         DU_FREE(spCellCfg->reconfigurationWithSync, sizeof(ReconfigurationWithSync_t));
      }

      /* Free rlmInSyncOutOfSyncThreshold */
      DU_FREE(spCellCfg->rlmInSyncOutOfSyncThreshold, sizeof(long));

      /* Free SP Cell config dedicated */
      if(spCellCfg->spCellConfigDedicated)
      {
         srvCellCfg = spCellCfg->spCellConfigDedicated;

         /* Free TDD UL-DL config dedicated */
         DU_FREE(srvCellCfg->tdd_UL_DL_ConfigurationDedicated, sizeof(TDD_UL_DL_ConfigDedicated_t));

         /* Free Initial Downlink BWP */
         if(srvCellCfg->initialDownlinkBWP)
         {
            dlBwp = srvCellCfg->initialDownlinkBWP;

            /* Free DL BWP PDCCH Config */
            if(dlBwp->pdcch_Config)
            {
               FreeBWPDlDedPdcchCfg(dlBwp);
               DU_FREE(dlBwp->pdcch_Config, sizeof(struct BWP_DownlinkDedicated__pdcch_Config));
            }

            /* Free DL BWP PDSCH config */
            if(dlBwp->pdsch_Config)
            {
               FreeBWPDlDedPdschCfg(dlBwp);
               DU_FREE(dlBwp->pdsch_Config, sizeof(struct BWP_DownlinkDedicated__pdsch_Config));
            }
            DU_FREE(srvCellCfg->initialDownlinkBWP, sizeof(BWP_DownlinkDedicated_t));
         }

         /* Free First Active Downlink BWP */
         DU_FREE(srvCellCfg->firstActiveDownlinkBWP_Id, sizeof(long));

         /* Free Default downlink BWP */
         DU_FREE(srvCellCfg->defaultDownlinkBWP_Id, sizeof(long));

         /* Free Uplink config */
         if(srvCellCfg->uplinkConfig)
         {
            FreeinitialUplinkBWP(srvCellCfg->uplinkConfig);
            DU_FREE(srvCellCfg->uplinkConfig, sizeof(UplinkConfig_t));	
         }

         /* Free PDSCH serving cell config */
         if(srvCellCfg->pdsch_ServingCellConfig)
         {
            pdschCfg= srvCellCfg->pdsch_ServingCellConfig;
            if(pdschCfg->choice.setup)
            {
               DU_FREE(pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH,sizeof(long));
               DU_FREE(pdschCfg->choice.setup, sizeof(struct PDSCH_ServingCellConfig));
            }
            DU_FREE(srvCellCfg->pdsch_ServingCellConfig, sizeof(struct ServingCellConfig__pdsch_ServingCellConfig));
         }

         DU_FREE(spCellCfg->spCellConfigDedicated, sizeof(ServingCellConfig_t));
      }
      DU_FREE(spCellCfg,sizeof(SpCellConfig_t));
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds CellGroupConfigRrc for DU To CU RRC Container 
 *
 * @details
 *
 *    Function : BuildCellGroupConfigRrc
 *
 *    Functionality: Builds and copied Cell group config buffer into 
 *       DuToCuRrcContainer
 *
 * @params[in] idx, index in F1AP msg
 *             DuToCuRRCContainer, DuToCuRRCContainer
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildCellGroupConfigRrc(DuUeCb *ueCb, OCTET_STRING_t *duToCuRrcContainer)
{
   uint8_t  ret = ROK;
   CellGroupConfigRrc_t  cellGrpCfg;
   asn_enc_rval_t        encRetVal;
   memset(&cellGrpCfg, 0, sizeof(CellGroupConfigRrc_t));
   memset(&encRetVal, 0, sizeof(asn_enc_rval_t));

   while(true)
   {
      cellGrpCfg.cellGroupId = CELL_GRP_ID;

      cellGrpCfg.rlc_BearerToAddModList = NULLP;
      DU_ALLOC(cellGrpCfg.rlc_BearerToAddModList, sizeof(struct CellGroupConfigRrc__rlc_BearerToAddModList));
      if(!cellGrpCfg.rlc_BearerToAddModList)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildDuToCuRrcContainer");
         ret = RFAILED;
         break;
      }
      if(BuildRlcBearerToAddModList(ueCb, cellGrpCfg.rlc_BearerToAddModList) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildRlcBearerToAddModList failed");
         ret = RFAILED;
         break;
      }

      cellGrpCfg.rlc_BearerToReleaseList = NULLP;
      cellGrpCfg.mac_CellGroupConfig = NULLP;
      DU_ALLOC(cellGrpCfg.mac_CellGroupConfig, sizeof(MAC_CellGroupConfig_t));
      if(!cellGrpCfg.mac_CellGroupConfig)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildDuToCuRrcContainer");
         ret = RFAILED;
         break;
      }
      if(BuildMacCellGrpCfg(ueCb, cellGrpCfg.mac_CellGroupConfig) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildMacCellGrpCfg failed");
         ret = RFAILED;
         break;
      }

      cellGrpCfg.physicalCellGroupConfig = NULLP;
      DU_ALLOC(cellGrpCfg.physicalCellGroupConfig, sizeof(PhysicalCellGroupConfig_t));
      if(!cellGrpCfg.physicalCellGroupConfig)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildDuToCuRrcContainer");
         ret = RFAILED;
         break;
      }
      if(BuildPhyCellGrpCfg(ueCb, cellGrpCfg.physicalCellGroupConfig) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildPhyCellGrpCfg failed");
         ret = RFAILED;
         break;
      }

      cellGrpCfg.spCellConfig = NULLP;
      DU_ALLOC(cellGrpCfg.spCellConfig, sizeof(SpCellConfig_t));
      if(!cellGrpCfg.spCellConfig)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildDuToCuRrcContainer");
         ret = RFAILED;
         break;
      }
      if(BuildSpCellCfg(ueCb, cellGrpCfg.spCellConfig) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildSpCellCfg failed");
         ret = RFAILED;
         break;
      }

      cellGrpCfg.sCellToAddModList = NULLP;
      cellGrpCfg.sCellToReleaseList = NULLP;
      cellGrpCfg.ext1 = NULLP;

      /* encode cellGrpCfg into duToCuRrcContainer */
      xer_fprint(stdout, &asn_DEF_CellGroupConfigRrc, &cellGrpCfg);
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = uper_encode(&asn_DEF_CellGroupConfigRrc, 0, &cellGrpCfg, PrepFinalEncBuf, encBuf);
      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG( "\nERROR  --> F1AP : Could not encode DuToCuRrcContainer (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         ret = RFAILED;
         break;
      }
      else
      {
         DU_LOG("\nDEBUG   -->  F1AP : Created APER encoded buffer for DuToCuRrcContainer\n");
#ifdef DEBUG_ASN_PRINT
         for(int i=0; i< encBufSize; i++)
         {
            printf("%x",encBuf[i]);
         }
#endif
      }

      duToCuRrcContainer->size = encBufSize;
      DU_ALLOC(duToCuRrcContainer->buf, duToCuRrcContainer->size);
      if(!duToCuRrcContainer->buf)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDuToCuRrcContainer");
         ret = RFAILED;
         break;
      }
      if(ret == ROK)
      {
         memcpy(duToCuRrcContainer->buf, encBuf, duToCuRrcContainer->size);
      }
      break;
   }
   FreeMemDuToCuRrcCont(&cellGrpCfg);
   return ret;
}

/*******************************************************************
 *
 * @brief Free memory allocated in InitialULRRCMessage
 *
 * @details
 *
 *    Function : freeInitUlRrcMsgTransfer
 *
 *    Functionality: Free memory allocated in InitialULRRCMessage
 *
 * @params[in]F1AP_PDU_t  *f1apMsg)
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t freeInitUlRrcMsgTransfer(F1AP_PDU_t  *f1apMsg)
{
   uint8_t ieIdx, arrIdx;
   InitialULRRCMessageTransfer_t *initULRRCMsg=NULLP;

   if(f1apMsg)
   {
      if(f1apMsg->choice.initiatingMessage)
      {
         initULRRCMsg = &f1apMsg->choice.initiatingMessage->value.\
	    choice.InitialULRRCMessageTransfer;
	 if(initULRRCMsg->protocolIEs.list.array)
	 {
	    for(ieIdx = 0; ieIdx < initULRRCMsg->protocolIEs.list.count; ieIdx++)
	    {
	       switch(initULRRCMsg->protocolIEs.list.array[ieIdx]->id)
	       {
	          case ProtocolIE_ID_id_NRCGI:
		  {
	             if(initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.NRCGI.pLMN_Identity.buf)
	             {
	                if(initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.NRCGI.nRCellIdentity.buf)
	                {
	                   DU_FREE(initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.NRCGI.nRCellIdentity.buf,\
	              	   initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.NRCGI.nRCellIdentity.size);
	                }
	                DU_FREE(initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.NRCGI.pLMN_Identity.buf,\
	              	initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.NRCGI.pLMN_Identity.size);
	             }
                     break;
		  }
		  case ProtocolIE_ID_id_RRCContainer:
	          {
	             if(initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.buf)
	             {
	                DU_FREE(initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.buf,\
	              	initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.size);
	             }
                     break;
		  }
		  case ProtocolIE_ID_id_DUtoCURRCContainer:
		  {
	             if(initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.DUtoCURRCContainer.buf)
	             {
	                DU_FREE(initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.DUtoCURRCContainer.buf,
	              	initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.DUtoCURRCContainer.size);
	             }
		     break;
		  }
		  default:
		     break;
	       }
	     }
	     for(arrIdx = 0; arrIdx < ieIdx; arrIdx++)
	     {
	        if(initULRRCMsg->protocolIEs.list.array[arrIdx])
		{
		   DU_FREE(initULRRCMsg->protocolIEs.list.array[arrIdx],\
		      sizeof(InitialULRRCMessageTransferIEs_t));
		}
	     }
	     DU_FREE(initULRRCMsg->protocolIEs.list.array, initULRRCMsg->protocolIEs.list.size);
	  }
	 DU_FREE(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
      }
      DU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
   }
   else
   {
      DU_LOG("\nERROR  -->  DU_APP : Recevied F1ap Pdu is null at freeInitUlRrcMsgTransfer()");
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds and sends the InitialULRRCMessage 
 *
 * @details
 *
 *    Function : BuildAndSendInitialRrcMsgTransfer 
 *
 *    Functionality: Constructs the Initial UL RRC Message Transfer and sends
 *                   it to the CU through SCTP.
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendInitialRrcMsgTransfer(uint32_t gnbDuUeF1apId, uint16_t crnti,
      uint16_t rrcContSize, uint8_t *rrcContainer)
{
   uint8_t   ret;
   uint8_t   elementCnt;
   uint8_t   ieIdx, cellIdx, ueIdx;
   DuUeCb    *duUeCb = NULLP;
   asn_enc_rval_t  encRetVal;
   F1AP_PDU_t  *f1apMsg = NULLP;
   InitialULRRCMessageTransfer_t *initULRRCMsg = NULLP;
   memset(&encRetVal, 0, sizeof(asn_enc_rval_t));

   while(true)
   {
      DU_LOG("\n INFO   -->  F1AP : Building RRC Setup Request\n");
      DU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
	 DU_LOG(" ERROR  -->  F1AP : Memory allocation for F1AP-PDU failed");
	 break;
      }
      f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
      DU_ALLOC(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
      if(f1apMsg->choice.initiatingMessage == NULLP)
      {
	 DU_LOG(" ERROR  -->  F1AP : Memory allocation for  F1AP-PDU failed");
	 break;
      }
      f1apMsg->choice.initiatingMessage->procedureCode =\
							ProcedureCode_id_InitialULRRCMessageTransfer;
      f1apMsg->choice.initiatingMessage->criticality = Criticality_ignore;
      f1apMsg->choice.initiatingMessage->value.present = \
							 InitiatingMessage__value_PR_InitialULRRCMessageTransfer;
      initULRRCMsg =\
		    &f1apMsg->choice.initiatingMessage->value.choice.InitialULRRCMessageTransfer;
      elementCnt = 5;
      initULRRCMsg->protocolIEs.list.count = elementCnt;
      initULRRCMsg->protocolIEs.list.size = \
					    elementCnt * sizeof(InitialULRRCMessageTransferIEs_t *);
      /* Initialize the F1Setup members */
      DU_ALLOC(initULRRCMsg->protocolIEs.list.array,initULRRCMsg->protocolIEs.list.size);
      if(initULRRCMsg->protocolIEs.list.array == NULLP)
      {
	 DU_LOG(" ERROR  -->  F1AP : Memory allocation for\
	       RRCSetupRequestMessageTransferIEs failed");
	 break;
      }
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
	 DU_ALLOC(initULRRCMsg->protocolIEs.list.array[ieIdx],\
	       sizeof(InitialULRRCMessageTransferIEs_t));
	 if(initULRRCMsg->protocolIEs.list.array[ieIdx] == NULLP)
	 {
	    break;
	 }
      }
      ieIdx = 0;
      /*GNB DU UE F1AP ID*/
      initULRRCMsg->protocolIEs.list.array[ieIdx]->id  = \
							ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->criticality  = Criticality_reject;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->value.present = \
								  InitialULRRCMessageTransferIEs__value_PR_GNB_DU_UE_F1AP_ID;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_UE_F1AP_ID = gnbDuUeF1apId;


      /*NRCGI*/
      ieIdx++;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->id  = \
							ProtocolIE_ID_id_NRCGI;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->criticality  =Criticality_reject;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->value.present = \
								  InitialULRRCMessageTransferIEs__value_PR_NRCGI;

      ret =\
	   BuildNrcgi(&initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.NRCGI);
      if(ret!=ROK)
      {
	 break;
      }

      /*CRNTI*/
      ieIdx++;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->id  = \
							ProtocolIE_ID_id_C_RNTI;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->value.present =\
								 InitialULRRCMessageTransferIEs__value_PR_C_RNTI;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.C_RNTI = crnti;

      /*RRCContainer*/
      ieIdx++;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->id  = \
							ProtocolIE_ID_id_RRCContainer;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->value.present =\
								 InitialULRRCMessageTransferIEs__value_PR_RRCContainer;

      initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.size = rrcContSize;
      DU_ALLOC(initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.buf,
	    initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.size)
      if(!initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.buf)
      {
         DU_LOG(" ERROR  -->  F1AP : Memory allocation for RRCSetupRequestMessageTransferIEs failed");
         break;
      
      }
      memcpy(initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.buf, rrcContainer,
	    initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.size);


      /*DUtoCURRCContainer*/
      ieIdx++;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_DUtoCURRCContainer;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      initULRRCMsg->protocolIEs.list.array[ieIdx]->value.present =\
								 InitialULRRCMessageTransferIEs__value_PR_DUtoCURRCContainer;

      for(cellIdx = 0; cellIdx < MAX_NUM_CELL; cellIdx++)
      {
         for(ueIdx = 0; ueIdx < MAX_NUM_UE; ueIdx++)
         {
            if(duCb.actvCellLst[cellIdx] && (duCb.actvCellLst[cellIdx]->ueCb[ueIdx].gnbDuUeF1apId == gnbDuUeF1apId)&&\
                  (duCb.actvCellLst[cellIdx]->ueCb[ueIdx].crnti == crnti))
            {
               duUeCb = &duCb.actvCellLst[cellIdx]->ueCb[ueIdx];
            }
         }
      }

      ret = BuildCellGroupConfigRrc(duUeCb, &initULRRCMsg->protocolIEs.list.array[ieIdx]->value.choice.DUtoCURRCContainer);
      if(ret != ROK)
      {
	 break;
      }

      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the Intial UL RRC Message transfer as APER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf, encBuf);
      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
	 DU_LOG( "\nERROR  --> F1AP : Could not encode Initial UL RRC Message Transfer\
	       structure (at %s)\n",encRetVal.failed_type ? \
	       encRetVal.failed_type->name : "unknown");
	 ret = RFAILED;
	 break;
      }
      else
      {

	 DU_LOG("\nDEBUG   -->  F1AP : Created APER encoded buffer for Initial UL RRC\
	       Message transfer\n");
#ifdef DEBUG_ASN_PRINT
	 for(int i=0; i< encBufSize; i++)
	 {
	    printf("%x",encBuf[i]);
	 }
#endif
      }
      /* Sending  msg  */
      if(sendF1APMsg() != ROK)
      {
	 DU_LOG("\nERROR  -->  F1AP : Sending Initial UL RRC Message Transfer Failed");
	 ret = RFAILED;
	 break;
      }
      break;
   }
   freeInitUlRrcMsgTransfer(f1apMsg);
   return ret;
}/* End of BuildAndSendInitialRrcMsgTransfer*/

/*****  UE SETUP REQUEST *****/

/******************************************************************
*
* @brief Function to delete the RLC Lc cfg from UE APP DB
*
* @details
*
*  Function : freeRlcLcCfg
*
*  Functionality: Function to delete the RLC Lc cfg from UE APP DB
*
*
 *****************************************************************/

void freeRlcLcCfg(RlcBearerCfg *lcCfg)
{
   switch(lcCfg->rlcMode)
   {
      case RLC_AM :
         {
            DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->u.amCfg, sizeof(AmBearerCfg));
            break;
         }
      case RLC_UM_BI_DIRECTIONAL :
         {
            DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->u.umBiDirCfg, sizeof(UmBiDirBearerCfg));
            break;
         }
      case RLC_UM_UNI_DIRECTIONAL_UL :
         {
            DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->u.umUniDirUlCfg, sizeof(UmUniDirUlBearerCfg));
            break;
         }
      case RLC_UM_UNI_DIRECTIONAL_DL :
         {
            DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->u.umUniDirDlCfg, sizeof(UmUniDirDlBearerCfg));
            break;
         }
      default:
         DU_LOG("\nERROR  -->  DU_APP: Invalid Rlc Mode %d at freeRlcLcCfg()", lcCfg->rlcMode);
         break;
   }
   DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->snssai, sizeof(Snssai));
}

/*******************************************************************
 *
 * @brief Function to free MacLcCfg
 *
 * @details
 *
 *    Function : freeMacLcCfg
 *
 *    Functionality: Function to free MacLcCfg
 *
 * @params[in] LcCfg *lcCfg,
 * @return void
 *
 * ****************************************************************/

void freeMacLcCfg(LcCfg *lcCfg)
{
    /* Deleting DRBQOS */
   if(lcCfg->drbQos)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->drbQos, sizeof(DrbQosInfo));
   }
   /* Deleting SNSSAI */
   if(lcCfg->snssai)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->snssai, sizeof(Snssai));
   }
}
/*******************************************************************
 *
 * @brief Free UE NR Capability received in UE Context setup request
 *
 * @details
 *
 *    Function : freeAperDecodeUeNrCapability
 *
 *    Functionality:  
 *       Free UE NR Capability received in UE Context setup request
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void freeAperDecodeUeNrCapability(void *ueNrCapability)
{
   uint8_t arrIdx =0;
   FeatureSets_t *featureSets =NULLP;
   UE_NR_Capability_t *ueNrCap = (UE_NR_Capability_t *)ueNrCapability;

   if(ueNrCap->rf_Parameters.supportedBandListNR.list.array)
   {
      for(arrIdx = 0; arrIdx < ueNrCap->rf_Parameters.supportedBandListNR.list.count; arrIdx++)
      {
         if(ueNrCap->rf_Parameters.supportedBandListNR.list.array[arrIdx])
            free(ueNrCap->rf_Parameters.supportedBandListNR.list.array[arrIdx]);
      }
      free(ueNrCap->rf_Parameters.supportedBandListNR.list.array);
   }

   if(ueNrCap->featureSets)
   {
      featureSets = ueNrCap->featureSets;
      if(featureSets->featureSetsDownlinkPerCC)
      {
         if(featureSets->featureSetsDownlinkPerCC->list.array)
         {
            for(arrIdx = 0; arrIdx < featureSets->featureSetsDownlinkPerCC->list.count; arrIdx++)
            {
               if(featureSets->featureSetsDownlinkPerCC->list.array[arrIdx])
               {
                  if(featureSets->featureSetsDownlinkPerCC->list.array[arrIdx]->supportedModulationOrderDL)
                     free(featureSets->featureSetsDownlinkPerCC->list.array[arrIdx]->supportedModulationOrderDL);
                  free(featureSets->featureSetsDownlinkPerCC->list.array[arrIdx]);
               }
            }
            free(featureSets->featureSetsDownlinkPerCC->list.array);
         }
         free(featureSets->featureSetsDownlinkPerCC);
      }
      if(featureSets->featureSetsUplinkPerCC)
      {
         if(featureSets->featureSetsUplinkPerCC->list.array)
         {
            for(arrIdx = 0; arrIdx < featureSets->featureSetsUplinkPerCC->list.count; arrIdx++)
            {
               if(featureSets->featureSetsUplinkPerCC->list.array[arrIdx])
               {
                  if(featureSets->featureSetsUplinkPerCC->list.array[arrIdx]->supportedModulationOrderUL)
                     free(featureSets->featureSetsUplinkPerCC->list.array[arrIdx]->supportedModulationOrderUL);
                  free(featureSets->featureSetsUplinkPerCC->list.array[arrIdx]);
               }
            }
            free(featureSets->featureSetsUplinkPerCC->list.array);
         }
         free(featureSets->featureSetsUplinkPerCC);
      }
      free(ueNrCap->featureSets);
   }   
}

/*******************************************************************
*
* @brief Function to free PdcchSearchSpcToAddModList
         where memory allocated by aper_decoder
*
* @details
*
*    Function : freeAperDecodePdcchSearchSpcToAddModList 
*
*    Functionality: Function to free PdcchSearchSpcToAddModList
*
* @params[in] struct PDCCH_Config__searchSpacesToAddModList *searchSpcList
* @return void
*
* ****************************************************************/

void freeAperDecodePdcchSearchSpcToAddModList(struct PDCCH_Config__searchSpacesToAddModList *searchSpcList)
{
   uint8_t searchSpcArrIdx=0;
   uint8_t searchSpcArrIdx1=0;
   struct  SearchSpace *searchSpc=NULLP;


   if(searchSpcList->list.array)
   {
      if(searchSpcList->list.array[searchSpcArrIdx1])
      {
	 searchSpc = searchSpcList->list.array[searchSpcArrIdx1];
	 if(searchSpc->controlResourceSetId)
	 {
	    if(searchSpc->monitoringSlotPeriodicityAndOffset)
	    {
	       if(searchSpc->monitoringSymbolsWithinSlot)
	       {
		  if(searchSpc->monitoringSymbolsWithinSlot->buf)
		  {
		     if(searchSpc->nrofCandidates)
		     {
			if(searchSpc->searchSpaceType)
			{
			   free(searchSpc->searchSpaceType->choice.ue_Specific);
			   free(searchSpc->searchSpaceType);
			}
			free(searchSpc->nrofCandidates);
		     }
		     free(searchSpc->monitoringSymbolsWithinSlot->buf);
		  }
		  free(searchSpc->monitoringSymbolsWithinSlot);
	       }
	       free(searchSpc->monitoringSlotPeriodicityAndOffset);
	    }
	    free(searchSpc->controlResourceSetId);
	 }
      }
      for(searchSpcArrIdx = 0; searchSpcArrIdx < searchSpcList->list.count; searchSpcArrIdx++)
      {
	 free(searchSpcList->list.array[searchSpcArrIdx]);
      }
      free(searchSpcList->list.array);
   }
}
/*******************************************************************
*
* @brief Function for free part for the memory allocated by aper_decoder
* 
* @details
*
*    Function : freeAperDecodeBWPDlDedPdcchConfig
*
*    Functionality: Function to free BWPDlDedPdcchConfig
*
* @params[in] 
* @return void
*
* ****************************************************************/


void freeAperDecodeBWPDlDedPdcchConfig(BWP_DownlinkDedicated_t *dlBwp)
{
   uint8_t arrIdx1=0;
   uint8_t arrIdx2=0;
   struct PDCCH_Config *pdcchCfg=NULLP;
   struct ControlResourceSet *controlRSet=NULLP;
   struct PDCCH_Config__controlResourceSetToAddModList *controlRSetList=NULLP;
   
   if(dlBwp->pdcch_Config->choice.setup)
   {
      pdcchCfg=dlBwp->pdcch_Config->choice.setup;
      if(pdcchCfg->controlResourceSetToAddModList)
      {
	 controlRSetList = pdcchCfg->controlResourceSetToAddModList;
	 if(controlRSetList->list.array)
	 {
	    controlRSet = controlRSetList->list.array[arrIdx2];
	    if(controlRSet)
	    {
	       if(controlRSet->frequencyDomainResources.buf)
	       {
		  if(controlRSet->pdcch_DMRS_ScramblingID)
		  {
		     if(pdcchCfg->searchSpacesToAddModList)
		     {
			freeAperDecodePdcchSearchSpcToAddModList(pdcchCfg->searchSpacesToAddModList);
			free(pdcchCfg->searchSpacesToAddModList);
		     }
		     free(controlRSet->pdcch_DMRS_ScramblingID);
		  }
		  free(controlRSet->frequencyDomainResources.buf);
	       }
	    }
	    for(arrIdx1 = 0; arrIdx1 <controlRSetList->list.count; arrIdx1++)
	    {
	       free(controlRSetList->list.array[arrIdx1]);
	    }
	    free(controlRSetList->list.array);
	 }
	 free(pdcchCfg->controlResourceSetToAddModList);
      }
      free(dlBwp->pdcch_Config->choice.setup);
   }
}
/*******************************************************************
*
* @brief Function to free PdschTimeDomAllocationList 
*     where the memory allocated by aper_decoder
* 
* @details
*
*    Function : freeAperDecodePdschTimeDomAllocationList
*
*    Functionality: Function to free PdschTimeDomAllocationList
*
* @params[in] struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAllocList 
* @return void
*
* ****************************************************************/


void freeAperDecodePdschTimeDomAllocationList( struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAllocList)
{
   uint8_t arrIdx=0;

   if(timeDomAllocList->choice.setup)
   {
      if(timeDomAllocList->choice.setup->list.array)
      {
	 for(arrIdx = 0; arrIdx <timeDomAllocList->choice.setup->list.count ; arrIdx++)
	 {
	    free(timeDomAllocList->choice.setup->list.array[arrIdx]);
	 }
	 free(timeDomAllocList->choice.setup->list.array);
      }
      free(timeDomAllocList->choice.setup);
   }
}

/*******************************************************************
*
* @brief Function to free BWPDlDedPdschConfig 
*        where the memory allocated by aper_decoder
*  
* @details
*
*    Function : freeAperDecodeBWPDlDedPdschConfig 
*
*    Functionality: Function to free BWPDlDedPdschConfig 
*
* @params[in] BWP_DownlinkDedicated_t *dlBwp 
* @return void
*
* ****************************************************************/


void freeAperDecodeBWPDlDedPdschConfig(BWP_DownlinkDedicated_t *dlBwp)
{
   struct PDSCH_Config *pdschCfg=NULLP;
   struct PDSCH_Config__prb_BundlingType *prbBndlType=NULLP;
   struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAllocList=NULLP;
   struct PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA *dmrsDlCfg=NULLP;

   if(dlBwp->pdsch_Config->choice.setup)
   {
      pdschCfg=dlBwp->pdsch_Config->choice.setup;
      if(pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA)
      {
	 if(pdschCfg->pdsch_TimeDomainAllocationList)
	 {
	    timeDomAllocList=pdschCfg->pdsch_TimeDomainAllocationList;
	    if(pdschCfg->maxNrofCodeWordsScheduledByDCI)
	    {
	       prbBndlType=&pdschCfg->prb_BundlingType;
	       free(prbBndlType->choice.staticBundling);
	       free(pdschCfg->maxNrofCodeWordsScheduledByDCI);
	    }
	    freeAperDecodePdschTimeDomAllocationList(timeDomAllocList);
	    free(pdschCfg->pdsch_TimeDomainAllocationList);
	 }
	 dmrsDlCfg=pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA;
	 if(dmrsDlCfg->choice.setup)
	 {
	    free(dmrsDlCfg->choice.setup->dmrs_AdditionalPosition);
	    free(dmrsDlCfg->choice.setup);
	 }
	 free(pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA);
      }
      free(dlBwp->pdsch_Config->choice.setup);
   }
}
/*******************************************************************
*
* @brief Function to free PuschTimeDomAllocListCfg
                 where the memory allocated by aper_decoder
*
* @details
*
*    Function : freeAperDecodePuschTimeDomAllocListCfg
*
*    Functionality: Function to free PuschTimeDomAllocListCfg
*
* @params[in] PUSCH_Config_t *puschCfg 
* @return void
*
* ****************************************************************/


void freeAperDecodePuschTimeDomAllocListCfg(PUSCH_Config_t *puschCfg)
{
   uint8_t arrIdx=0;
   uint8_t arrIdx1=0;
   struct PUSCH_Config__pusch_TimeDomainAllocationList *timeDomAllocList_t=NULLP;

   if(puschCfg->pusch_TimeDomainAllocationList)
   {
      timeDomAllocList_t=puschCfg->pusch_TimeDomainAllocationList;
      if(timeDomAllocList_t->choice.setup)
      {
	 if(timeDomAllocList_t->choice.setup->list.array)
	 {
	    free(timeDomAllocList_t->choice.setup->list.array[arrIdx1]->k2);
	    for(arrIdx = 0; arrIdx<timeDomAllocList_t->choice.setup->list.count; arrIdx++)
	    {
	       free(timeDomAllocList_t->choice.setup->list.array[arrIdx]);
	    }
	    free(timeDomAllocList_t->choice.setup->list.array);
	 }
	 free(timeDomAllocList_t->choice.setup);
      }
      free(puschCfg->transformPrecoder);
      free(puschCfg->pusch_TimeDomainAllocationList);
   }
}
/*******************************************************************
*
* @brief Function to free InitialUlBWPConfig where memory allocated by aper_decoder
*
* @details
*
*    Function : freeAperDecodeInitialUlBWPConfig 
*
*    Functionality: Function to free InitialUlBWPConfig
*
* @params[in]  BWP_UplinkDedicated_t *ulBwp
* @return void
*
* ****************************************************************/


void freeAperDecodeInitialUlBWPConfig(BWP_UplinkDedicated_t *ulBwp)
{
   uint8_t  rSetIdx =0;
   uint8_t  rsrcIdx =0;
   SRS_Config_t   *srsCfg = NULLP;
   PUSCH_Config_t *puschCfg = NULLP;
   struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA *dmrsUlCfg = NULLP;
   struct SRS_Config__srs_ResourceSetToAddModList *rsrcSetList = NULLP;
   struct SRS_ResourceSet__srs_ResourceIdList *rsrcIdList = NULLP;
   struct SRS_Config__srs_ResourceToAddModList *resourceList = NULLP;

   if(ulBwp->pusch_Config)
   {
      if(ulBwp->pusch_Config->choice.setup)
      {
	 puschCfg=ulBwp->pusch_Config->choice.setup;
	 if(puschCfg->dataScramblingIdentityPUSCH)
	 {
	    if(puschCfg->dmrs_UplinkForPUSCH_MappingTypeA)
	    {
	       freeAperDecodePuschTimeDomAllocListCfg(puschCfg);
	       dmrsUlCfg=puschCfg->dmrs_UplinkForPUSCH_MappingTypeA;
	       if(dmrsUlCfg->choice.setup)
	       {
		  if(dmrsUlCfg->choice.setup->dmrs_AdditionalPosition)
		  {
		     if(dmrsUlCfg->choice.setup->transformPrecodingDisabled)
		     {
			free(dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0);
			free(dmrsUlCfg->choice.setup->transformPrecodingDisabled);
		     }
		     free(dmrsUlCfg->choice.setup->dmrs_AdditionalPosition);
		  }
		  free(dmrsUlCfg->choice.setup);
	       }
	       free(puschCfg->dmrs_UplinkForPUSCH_MappingTypeA);
	    }
	    free(puschCfg->dataScramblingIdentityPUSCH);
	 }
	 free(ulBwp->pusch_Config->choice.setup);
      }
      free(ulBwp->pusch_Config);

      /* Free SRS-Config */
      if(ulBwp->srs_Config)
      {
	 if(ulBwp->srs_Config->choice.setup)
	 {
	    srsCfg = ulBwp->srs_Config->choice.setup;

	    /* Free Resource Set to add/mod list */
	    if(srsCfg->srs_ResourceSetToAddModList)
	    {
	       rsrcSetList = srsCfg->srs_ResourceSetToAddModList;
	       if(rsrcSetList->list.array)
	       {
		  rSetIdx = 0;

		  /* Free SRS resource Id list in this SRS resource set */
		  if(rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList)
		  {
		     rsrcIdList =
			rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList;

		     if(rsrcIdList->list.array)
		     {
			for(rsrcIdx = 0; rsrcIdx < rsrcIdList->list.count;
			      rsrcIdx++)
			{
			   free(rsrcIdList->list.array[rsrcIdx]);
			}
			free(rsrcIdList->list.array);
		     }
		     free(rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList);
		  }

		  /* Free resource type info for this SRS resource set */

		  free(rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic);

		  /* Free memory for each resource set */
		  for(rSetIdx = 0; rSetIdx < rsrcSetList->list.count; rSetIdx++)
		  {
		     free(rsrcSetList->list.array[rSetIdx]);
		  }
		  free(rsrcSetList->list.array);
	       }
	       free(srsCfg->srs_ResourceSetToAddModList);
	    }

	    /* Free resource to add/modd list */
	    if(srsCfg->srs_ResourceToAddModList)
	    {
	       resourceList = srsCfg->srs_ResourceToAddModList;
	       if(resourceList->list.array)
	       {
		  rsrcIdx = 0;

		  free(resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2);
		  free(resourceList->list.array[rsrcIdx]->resourceType.choice.aperiodic);

		  for(rsrcIdx = 0; rsrcIdx < resourceList->list.count; rsrcIdx++)
		  {
		     free(resourceList->list.array[rsrcIdx]);
		  }
		  free(resourceList->list.array);
	       }
	       free(srsCfg->srs_ResourceToAddModList);
	    }

	    free(ulBwp->srs_Config->choice.setup);
	 }
	 free(ulBwp->srs_Config);
      }
   }
}
/*******************************************************************
*
* @brief Function to free initialUplinkBWPConfig where memory allocated by aper_decoder
*
* @details
*
*    Function : freeAperDecodeinitialUplinkBWPConfig
*
*    Functionality: Function to free initialUplinkBWPConfig
*
* @params[in] UplinkConfig_t *ulCfg 
* @return void
*
* ****************************************************************/


void freeAperDecodeinitialUplinkBWPConfig(UplinkConfig_t *ulCfg)
{
   BWP_UplinkDedicated_t *ulBwp=NULLP;
   struct UplinkConfig__pusch_ServingCellConfig *puschCfg=NULLP;
   
   if(ulCfg->initialUplinkBWP)
   {
      ulBwp=ulCfg->initialUplinkBWP;
      if(ulCfg->firstActiveUplinkBWP_Id)
      {
	 if(ulCfg->pusch_ServingCellConfig)
	 {
	    puschCfg=ulCfg->pusch_ServingCellConfig;
	    if(puschCfg->choice.setup)
	    {
	       if(puschCfg->choice.setup->ext1)
	       {
		  free(puschCfg->choice.setup->ext1->processingType2Enabled);
		  free(puschCfg->choice.setup->ext1->maxMIMO_Layers);
		  free(puschCfg->choice.setup->ext1);
	       }
	       free(puschCfg->choice.setup);
	    }
	    free(ulCfg->pusch_ServingCellConfig);
	 }
	 free(ulCfg->firstActiveUplinkBWP_Id);
      }
      freeAperDecodeInitialUlBWPConfig(ulBwp);
      free(ulCfg->initialUplinkBWP);
   }
}

/*******************************************************************
 *
 * @brief Function to free DuUeCfg
 *
 * @details
 *
 *    Function : freeDuUeCfg
 *
 *    Functionality: Function to free DuUeCfg
 *
 * @params[in] DuUeCfg *ueCfg
 * @return void
 *
 * ****************************************************************/
void freeDuUeCfg(UeCtxtActionType actionType, DuUeCfg *ueCfg)
{
   uint8_t lcIdx = 0;
   uint8_t arrIdx = 0;
   SpCellConfig_t *spCellCfg = NULLP;
   ServingCellConfig_t *srvCellCfg = NULLP;
   BWP_DownlinkDedicated_t *dlBwp = NULLP;
   MAC_CellGroupConfig_t *macCellGrpCfg = NULLP;
   PhysicalCellGroupConfig_t *phyCellGrpCfg = NULLP;
   struct CellGroupConfigRrc__rlc_BearerToAddModList *rlcBearerList = NULLP;
   struct RLC_Config *rlcConfig = NULLP;
   struct LogicalChannelConfig *macLcConfig = NULLP;
   struct SchedulingRequestConfig *schedulingRequestConfig = NULLP;
   struct SchedulingRequestConfig__schedulingRequestToAddModList *schReqList = NULLP;
   struct TAG_Config *tagConfig = NULLP;
   struct TAG_Config__tag_ToAddModList *tagList = NULLP;
   struct MAC_CellGroupConfig__phr_Config *phrConfig = NULLP;
   struct ServingCellConfig__pdsch_ServingCellConfig *pdschCfg = NULLP;
   CellGroupConfigRrc_t *cellGrpCfg = ueCfg->cellGrpCfg;
  
   if(ueCfg->ueNrCapability)
   {
      freeAperDecodeUeNrCapability(ueCfg->ueNrCapability);
      DU_FREE(ueCfg->ueNrCapability, sizeof(UE_NR_Capability_t));
      ueCfg->ueNrCapability = NULLP;
   }

   if(ueCfg->cellGrpCfg)
   {
      rlcBearerList = cellGrpCfg->rlc_BearerToAddModList;
      if(rlcBearerList)
      {
         if(rlcBearerList->list.array)
         {
            for(arrIdx=0; arrIdx<rlcBearerList->list.count; arrIdx++)
            {
               if(rlcBearerList->list.array[arrIdx])
               {
                  rlcConfig   = rlcBearerList->list.array[arrIdx]->rlc_Config;
                  macLcConfig = rlcBearerList->list.array[arrIdx]->mac_LogicalChannelConfig;

                  if(rlcBearerList->list.array[arrIdx]->servedRadioBearer)
                  {
                     free(rlcBearerList->list.array[arrIdx]->servedRadioBearer);
                  }
                  if(rlcConfig)
                  {
                     if(rlcConfig->choice.am)
                     {
                        free(rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength);
                        free(rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength);
                        free(rlcConfig->choice.am);
                     }
                     free(rlcBearerList->list.array[arrIdx]->rlc_Config);
                  }
                  if(macLcConfig)
                  {
                     if(macLcConfig->ul_SpecificParameters)
                     {
                        free(macLcConfig->ul_SpecificParameters->schedulingRequestID);
                        free(macLcConfig->ul_SpecificParameters->logicalChannelGroup);
                        free(macLcConfig->ul_SpecificParameters);
                     }
                     free(rlcBearerList->list.array[arrIdx]->mac_LogicalChannelConfig);
                  }
                  free(rlcBearerList->list.array[arrIdx]); 
               }
            }
            free(rlcBearerList->list.array);
         }
         free(cellGrpCfg->rlc_BearerToAddModList);
      }

      macCellGrpCfg = cellGrpCfg->mac_CellGroupConfig;
      if(macCellGrpCfg)
      {
         schedulingRequestConfig = macCellGrpCfg->schedulingRequestConfig;
         if(schedulingRequestConfig)
         {
            schReqList = schedulingRequestConfig->schedulingRequestToAddModList;
            if(schReqList)
            {
               if(schReqList->list.array)
               {
                  for(arrIdx=0;arrIdx<schReqList->list.count; arrIdx++)
                  {
                     if(schReqList->list.array[arrIdx])
                     {
                        free(schReqList->list.array[arrIdx]->sr_ProhibitTimer); 
                        free(schReqList->list.array[arrIdx]);
                     }
                  }
                  free(schReqList->list.array);
               }
               free(schedulingRequestConfig->schedulingRequestToAddModList);
            }
            free(macCellGrpCfg->schedulingRequestConfig);
         }
         if(macCellGrpCfg->bsr_Config)
         {
            free(macCellGrpCfg->bsr_Config);
         }
         tagConfig = macCellGrpCfg->tag_Config;
         if(tagConfig)
         {
            tagList = tagConfig->tag_ToAddModList;
            if(tagList)
            {
               if(tagList->list.array)
               {
                  for(arrIdx=0; arrIdx<tagList->list.count; arrIdx++)
                  {
                     free(tagList->list.array[arrIdx]);
                  }
                  free(tagList->list.array);
               }
               free(tagConfig->tag_ToAddModList);
            }
            free(tagConfig); 
         }

         phrConfig = macCellGrpCfg->phr_Config;
         if(phrConfig)
         {
            free(phrConfig->choice.setup); 
            free(phrConfig); 
         }

         free(macCellGrpCfg); 
      }

      phyCellGrpCfg = cellGrpCfg->physicalCellGroupConfig;
      if(phyCellGrpCfg)
      {
         free(phyCellGrpCfg->p_NR_FR1);
         free(phyCellGrpCfg); 
      }

      spCellCfg = cellGrpCfg->spCellConfig;
      if(spCellCfg)
      {
         if(spCellCfg->servCellIndex)
         {
            if(spCellCfg->rlmInSyncOutOfSyncThreshold)
            {
               if(spCellCfg->spCellConfigDedicated)
               {
                  srvCellCfg = spCellCfg->spCellConfigDedicated;
                  if(srvCellCfg->initialDownlinkBWP)
                  {
                     dlBwp = srvCellCfg->initialDownlinkBWP;
                     if(srvCellCfg->firstActiveDownlinkBWP_Id)
                     {
                        if(srvCellCfg->defaultDownlinkBWP_Id)
                        {
                           if(srvCellCfg->uplinkConfig)
                           {

                              if(srvCellCfg->pdsch_ServingCellConfig)
                              {
                                 pdschCfg=
                                    srvCellCfg->pdsch_ServingCellConfig;
                                 if(pdschCfg->choice.setup)
                                 {

                                    free(pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH);
                                    free(pdschCfg->choice.setup);
                                 }

                                 free(srvCellCfg->pdsch_ServingCellConfig);
                              }

                              freeAperDecodeinitialUplinkBWPConfig(srvCellCfg->uplinkConfig);
                              free(srvCellCfg->uplinkConfig);
                           }
                           free(srvCellCfg->defaultDownlinkBWP_Id);
                        }

                        free(srvCellCfg->firstActiveDownlinkBWP_Id);
                     }
                     if(dlBwp->pdcch_Config)
                     {
                        if(dlBwp->pdsch_Config)
                        {
                           freeAperDecodeBWPDlDedPdschConfig(dlBwp);
                           free(dlBwp->pdsch_Config);
                        }
                        freeAperDecodeBWPDlDedPdcchConfig(dlBwp);
                        free(dlBwp->pdcch_Config);
                     }
                     free(srvCellCfg->initialDownlinkBWP);
                  }

                  free(spCellCfg->spCellConfigDedicated);
               }
               free(spCellCfg->rlmInSyncOutOfSyncThreshold);
            }
            free(spCellCfg->servCellIndex); 
         }
         free(spCellCfg);
      }
      DU_FREE(ueCfg->cellGrpCfg, sizeof(CellGroupConfigRrc_t));
      ueCfg->cellGrpCfg = NULLP;
   }

   if(ueCfg->ambrCfg)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, ueCfg->ambrCfg, sizeof(AmbrCfg));
   }

   if(actionType != UE_CTXT_CFG_QUERY)
   {
      for(lcIdx = 0; lcIdx < ueCfg->numRlcLcs; lcIdx++)
      {
         freeRlcLcCfg(&ueCfg->rlcLcCfg[lcIdx].rlcBearerCfg);
      }
   }

   for(lcIdx = 0; lcIdx < ueCfg->numMacLcs; lcIdx++)
   {
      freeMacLcCfg(&ueCfg->macLcCfg[lcIdx].lcConfig);
   }

   for(lcIdx = 0; lcIdx < ueCfg->numDrb; lcIdx++)
   {
      DU_FREE(ueCfg->upTnlInfo[lcIdx].tnlCfg1, sizeof(GtpTnlCfg));
   }
}

/*******************************************************************
 *
 * @brief Function to free UecontextSetupDb
 *
 * @details
 *
 *    Function : freeF1UeDb
 *
 *    Functionality: Function to free UecontextSetupDb
 *
 * @params[in] UecontextSetupDb *
 * @return void
 *
 * ****************************************************************/

void freeF1UeDb(F1UeContextSetupDb *f1UeDb)
{
   
   if(f1UeDb->dlRrcMsg)
   {
      if(f1UeDb->dlRrcMsg->rrcMsgPdu)
      {
        //DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL,\
	       f1UeDb->dlRrcMsg->rrcMsgPdu, f1UeDb->dlRrcMsg->rrcMsgSize);
      }
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, f1UeDb->dlRrcMsg, sizeof(F1DlRrcMsg));
   }
   freeDuUeCfg(f1UeDb->actionType, &f1UeDb->duUeCfg);
   memset(f1UeDb, 0, sizeof(F1UeContextSetupDb));
   DU_FREE(f1UeDb, sizeof(F1UeContextSetupDb));
}

/*******************************************************************
 *
 * @brief Function to build Am cfg Info
 *
 * @details
 *
 *    Function : extractRlcAmCfg
 *
 *    Functionality: Function to build Am cfg Info
 *
 * @params[in] AmBearerCfg *
 *             void *
 *
 * @return ROK/RFAILED
 *
 * ****************************************************************/

void extractRlcAmCfg(AmBearerCfg *amCfgToSet, struct RLC_Config__am *rlcAmCfg)
{
   if(rlcAmCfg)
   {
      /* UL AM */
      if(rlcAmCfg->dl_AM_RLC.sn_FieldLength)
      {
	 amCfgToSet->ulAmCfg.snLenUl = covertAmSnLenFromRrcEnumToIntEnum(*(rlcAmCfg->dl_AM_RLC.sn_FieldLength));
         /*TODO: Check the timer value when sent by real CU */
	 amCfgToSet->ulAmCfg.reAssemTmr = convertReasmblTmrEnumToValue(rlcAmCfg->dl_AM_RLC.t_Reassembly); 
	 amCfgToSet->ulAmCfg.statProhTmr = convertProhibitTmrEnumToValue(rlcAmCfg->dl_AM_RLC.t_StatusProhibit);
      }

      /* DL AM */
      if(rlcAmCfg->ul_AM_RLC.sn_FieldLength)
      {
	 amCfgToSet->dlAmCfg.snLenDl = covertAmSnLenFromRrcEnumToIntEnum(*(rlcAmCfg->ul_AM_RLC.sn_FieldLength));
	 amCfgToSet->dlAmCfg.pollRetxTmr = covertPollRetxTmrEnumToValue(rlcAmCfg->ul_AM_RLC.t_PollRetransmit);
	 amCfgToSet->dlAmCfg.pollPdu   = covertPollPduEnumToValue(rlcAmCfg->ul_AM_RLC.pollPDU);
	 amCfgToSet->dlAmCfg.pollByte  = covertPollByteEnumToValue(rlcAmCfg->ul_AM_RLC.pollByte);
	 amCfgToSet->dlAmCfg.maxRetxTh = covertMaxRetxEnumToValue(rlcAmCfg->ul_AM_RLC.maxRetxThreshold);
      }
   }
}

/*******************************************************************
 *
 * @brief Function to build Um Bi Info
 *
 * @details
 *
 *    Function : extractRlcUmBiCfg
 *
 *    Functionality: Function to build Um Bi Info
 *
 * @params[in] UmBiDirBearerCfg *
 *             void *
 *
 * @return ROK/RFAILED
 *
 * ****************************************************************/

void extractRlcUmBiCfg(UmBiDirBearerCfg *umBiCfgToSet, struct RLC_Config__um_Bi_Directional *rlcBiCfg)
{
   if(rlcBiCfg)
   {
      /* UL UM BI DIR Cfg */
      if(rlcBiCfg->dl_UM_RLC.sn_FieldLength)
      {
	 umBiCfgToSet->ulUmCfg.snLenUlUm  = covertUmSnLenFromRrcEnumToIntEnum(*(rlcBiCfg->dl_UM_RLC.sn_FieldLength));
	 umBiCfgToSet->ulUmCfg.reAssemTmr = convertReasmblTmrEnumToValue(rlcBiCfg->dl_UM_RLC.t_Reassembly);
      }

      /* DL UM BI DIR Cfg */
      if(rlcBiCfg->ul_UM_RLC.sn_FieldLength)
	 umBiCfgToSet->dlUmCfg.snLenDlUm  = covertUmSnLenFromRrcEnumToIntEnum(*(rlcBiCfg->ul_UM_RLC.sn_FieldLength));
   }
}

/*******************************************************************
 *
 * @brief Function to build Um Ul Info
 *
 * @details
 *
 *    Function : extractRlcUmUlCfg
 *
 *    Functionality: Function to build Um Ul Info
 *
 * @params[in] UmUniDirUlBearerCfg *
 *             void *
 *
 * @return ROK/RFAILED
 *
 * ****************************************************************/

void extractRlcUmUlCfg(UmUniDirUlBearerCfg *umUlCfgToSet, struct RLC_Config__um_Uni_Directional_DL *umUlCfg)
{
   if(umUlCfg)
   {
      if(umUlCfg->dl_UM_RLC.sn_FieldLength)
      {
         umUlCfgToSet->ulUmCfg.snLenUlUm  = covertUmSnLenFromRrcEnumToIntEnum(*(umUlCfg->dl_UM_RLC.sn_FieldLength));
         umUlCfgToSet->ulUmCfg.reAssemTmr = convertReasmblTmrEnumToValue(umUlCfg->dl_UM_RLC.t_Reassembly);
      }
   }
}

/*******************************************************************
 *
 * @brief Function to build Um Uni Dl Info
 *
 * @details
 *
 *    Function : extractRlcUmDlCfg
 *
 *    Functionality: Function to build Um Uni Dl Info
 *
 * @params[in] UmUniDirDlBearerCfg *
 *             void *
 *
 * @return ROK/RFAILED
 *
 * ****************************************************************/
void extractRlcUmDlCfg(UmUniDirDlBearerCfg *umDlCfgToSet, struct RLC_Config__um_Uni_Directional_UL *umDlCfg)
{
   if(umDlCfg)
   {
      if(umDlCfg->ul_UM_RLC.sn_FieldLength)
         umDlCfgToSet->dlUmCfg.snLenDlUm  = covertUmSnLenFromRrcEnumToIntEnum(*(umDlCfg->ul_UM_RLC.sn_FieldLength));
   }
}

/*******************************************************************
 *
 * @brief Function to extractRlcModeCfg
 *
 * @details
 *
 *    Function : extractRlcModeCfg
 *
 *    Functionality: Function to extractRlcModeCfg
 *
 * @params[in] RLC_Config_t *
 *             RlcBearerCfg *
 *             void  *    
 * @return ROK/RFAILED
 *
 * ****************************************************************/
void extractRlcModeCfg(uint8_t rlcMode, RlcBearerCfg *rlcDbCfg, RLC_Config_t *lcCfg)
{
   if(lcCfg)
   {
      switch(rlcMode)
      {
         case RLC_AM :
            {
               if(lcCfg->choice.am)
               {
                  DU_ALLOC_SHRABL_BUF(rlcDbCfg->u.amCfg, sizeof(AmBearerCfg));
                  if(rlcDbCfg->u.amCfg)
                     extractRlcAmCfg(rlcDbCfg->u.amCfg, lcCfg->choice.am);
               }
               break;
            }
         case RLC_UM_BI_DIRECTIONAL :
            {
               if(lcCfg->choice.um_Bi_Directional)
               {
                  DU_ALLOC_SHRABL_BUF(rlcDbCfg->u.umBiDirCfg, sizeof(UmBiDirBearerCfg));
                  if(rlcDbCfg->u.umBiDirCfg)
                     extractRlcUmBiCfg(rlcDbCfg->u.umBiDirCfg, lcCfg->choice.um_Bi_Directional);
               }
               break;
            }
         case RLC_UM_UNI_DIRECTIONAL_UL :
            {
               if(lcCfg->choice.um_Uni_Directional_DL)
               {
                  DU_ALLOC_SHRABL_BUF(rlcDbCfg->u.umUniDirUlCfg, sizeof(UmUniDirUlBearerCfg));
                  if(rlcDbCfg->u.umUniDirUlCfg)
                     extractRlcUmUlCfg(rlcDbCfg->u.umUniDirUlCfg, lcCfg->choice.um_Uni_Directional_DL);
               }
               break;
            }
         case RLC_UM_UNI_DIRECTIONAL_DL :
            {
               if(lcCfg->choice.um_Uni_Directional_UL)
               {
                  DU_ALLOC_SHRABL_BUF(rlcDbCfg->u.umUniDirDlCfg, sizeof(UmUniDirDlBearerCfg));
                  if(rlcDbCfg->u.umUniDirDlCfg)
                     extractRlcUmDlCfg(rlcDbCfg->u.umUniDirDlCfg, lcCfg->choice.um_Uni_Directional_UL);
               }
               break;
            }
         default:
            DU_LOG("\nERROR  -->  DUAPP: Invalid rlcMode %d at extractRlcCfgToAddMod()", rlcMode);
            break;
      }
   }
}

/*******************************************************************
 *
 * @brief Function to extract extractUlLcCfg
 *
 * @details
 *
 *    Function : extractUlLcCfg
 *
 *    Functionality: Function to extract extractUlLcCfg
 *
 * @params[in] UlLcCfg *f1UlLcCfg, LogicalChannelConfig_t *ulLcCfg
 * @return void
 *
 * ****************************************************************/

void extractUlLcCfg(UlLcCfg *f1UlLcCfg, LogicalChannelConfig_t *ulLcCfg)
{
   if(ulLcCfg)
   {
      if(ulLcCfg->ul_SpecificParameters)
      {
         f1UlLcCfg->priority = \
            ulLcCfg->ul_SpecificParameters->priority;
      if(ulLcCfg->ul_SpecificParameters->logicalChannelGroup)
      {
         f1UlLcCfg->lcGroup = \
           *(ulLcCfg->ul_SpecificParameters->logicalChannelGroup);
      }
      if(ulLcCfg->ul_SpecificParameters->schedulingRequestID)
      {
         f1UlLcCfg->schReqId = \
           *(ulLcCfg->ul_SpecificParameters->schedulingRequestID);
      }
      f1UlLcCfg->pbr = \
         ulLcCfg->ul_SpecificParameters->prioritisedBitRate;
      f1UlLcCfg->bsd = \
         ulLcCfg->ul_SpecificParameters->bucketSizeDuration;
      }
   }
}

/*******************************************************************
*
* @brief Function to extract Snssai Cfg Info from CU
*
* @details
*
*    Function : extractDrbSnssaiCfg
*
*    Functionality: Function to extract Drb Snssai Cfg Info from CU
*
* @params[in] DRB_Information_t *drbInfo, Snssai  *snssai
* @return ROK/RFAILED
*
* ****************************************************************/

uint8_t extractDrbSnssaiCfg(SNSSAI_t *RecvSnssai, Snssai **snssaiToBeShared)
{
   if(!(*snssaiToBeShared))
   {
      DU_ALLOC_SHRABL_BUF((*snssaiToBeShared), sizeof(Snssai));
      if(snssaiToBeShared == NULLP)
      {
         DU_LOG("\nERROR  -->  DUAPP : extractDrbSnssaiCfg(): Memory failed at allocating for SNSSAI ");
         return RFAILED;
      }
   }
   if(RecvSnssai)
   {
      memcpy(&(*snssaiToBeShared)->sst, RecvSnssai->sST.buf, RecvSnssai->sST.size);
      if(RecvSnssai->sD)
      {
         memcpy((*snssaiToBeShared)->sd, RecvSnssai->sD->buf,  RecvSnssai->sD->size);
      }
      else
      {
         DU_LOG("\nERROR  -->  DUAPP : extractDrbSnssaiCfg(): Received Null pointer of Snssai->SD");
         return RFAILED;
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Function to procRlcLcCfg
 *
 * @details
 *
 *    Function : procRlcLcCfg
 *
 *    Functionality: Function to procRlcLcCfg
 *
 * @params[in] rbId, lcId, rbType, rlcMod
 *             RLC_Config_t *, RlcBearerCfg * , 
 * @return void
 *
 * ****************************************************************/

void procRlcLcCfg(uint8_t rbId, uint8_t lcId, uint8_t rbType, uint8_t rlcMode,\
   uint8_t configType, RLC_Config_t *f1RlcCfg, DuRlcBearerCfg *lcCfg, QoSInformation_t *qoSInformation)
{
   DRB_Information_t *drbInfo;

   lcCfg->rlcBearerCfg.rbId   = rbId;
   lcCfg->configType = configType;

   if(rbType == RB_TYPE_SRB)
   {
      lcCfg->rlcBearerCfg.rbType = RB_TYPE_SRB;
      lcCfg->rlcBearerCfg.lcId   = rbId;
      lcCfg->rlcBearerCfg.lcType = LCH_DCCH;
      lcCfg->rlcBearerCfg.rlcMode = RLC_AM;
   }
   else if(rbType == RB_TYPE_DRB)
   {
      lcCfg->rlcBearerCfg.rbType = RB_TYPE_DRB;
      lcCfg->rlcBearerCfg.lcId   = lcId;
      lcCfg->rlcBearerCfg.lcType = LCH_DTCH;
      lcCfg->rlcBearerCfg.rlcMode = rlcMode;
   }
   if(f1RlcCfg) /* rlc mode config recived */
   {
      extractRlcModeCfg(lcCfg->rlcBearerCfg.rlcMode, &lcCfg->rlcBearerCfg, f1RlcCfg);
   }
   if(qoSInformation != NULLP)
   {
      if(qoSInformation->present == QoSInformation_PR_choice_extension)
      {
         if(qoSInformation->choice.choice_extension->value.present ==\
               QoSInformation_ExtIEs__value_PR_DRB_Information)
         {
            drbInfo = &qoSInformation->choice.choice_extension->value.choice.DRB_Information; 
            if(extractDrbSnssaiCfg(&drbInfo->sNSSAI, &lcCfg->rlcBearerCfg.snssai) != ROK)
            {
               DU_LOG("\nERROR  -->  DUAPP: Unable to extract Snssai information at procRlcLcCfg()");
               return;
            }
         }
      }
   }
}

/*******************************************************************
 *
 * @brief Fills DrbQos Info received by CU
 *
 * @details
 *
 *    Function : extractQosInfo
 *
 *    Functionality: Fills DrbQos Info received  by CU
 *
 * @params[in] DrbQosInfo *qosToAdd, 
 *             QoSFlowLevelQoSParameters_t *qosFlowCfg
 * @return void
 *
 * ****************************************************************/

void extractQosInfo(DrbQosInfo *qosToAdd, QoSFlowLevelQoSParameters_t *qosFlowCfg)
{
   uint8_t qosCntIdx = 0;
   ProtocolExtensionContainer_4624P74_t *qosIeExt = NULLP;

   qosToAdd->fiveQiType = qosFlowCfg->qoS_Characteristics.present;
   qosToAdd->u.nonDyn5Qi.fiveQi     =\
                                     qosFlowCfg->qoS_Characteristics.choice.non_Dynamic_5QI->fiveQI;
   if(qosFlowCfg->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow)
   {
      qosToAdd->u.nonDyn5Qi.avgWindow = \
                                        *(qosFlowCfg->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow);
   }

   if(qosFlowCfg->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume)
   {
      qosToAdd->u.nonDyn5Qi.maxDataBurstVol = \
                                              *(qosFlowCfg->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume);
   }

   if(qosFlowCfg->qoS_Characteristics.choice.non_Dynamic_5QI->qoSPriorityLevel)
   {
      qosToAdd->u.nonDyn5Qi.priorLevel = \
                                         *(qosFlowCfg->qoS_Characteristics.choice.non_Dynamic_5QI->qoSPriorityLevel);
   }
   qosToAdd->ngRanRetPri.priorityLevel = \
                                         qosFlowCfg->nGRANallocationRetentionPriority.priorityLevel; 
   qosToAdd->ngRanRetPri.preEmptionCap = \
                                         qosFlowCfg->nGRANallocationRetentionPriority.pre_emptionCapability;
   qosToAdd->ngRanRetPri.preEmptionVul = \
                                         qosFlowCfg->nGRANallocationRetentionPriority.pre_emptionVulnerability;
   if(qosFlowCfg->gBR_QoS_Flow_Information)
   {
      memcpy(&qosToAdd->grbQosInfo.maxFlowBitRateDl, \
            qosFlowCfg->gBR_QoS_Flow_Information->maxFlowBitRateDownlink.buf, \
            qosFlowCfg->gBR_QoS_Flow_Information->maxFlowBitRateDownlink.size);
      memcpy(&qosToAdd->grbQosInfo.maxFlowBitRateUl, \
            qosFlowCfg->gBR_QoS_Flow_Information->maxFlowBitRateUplink.buf, \
            qosFlowCfg->gBR_QoS_Flow_Information->maxFlowBitRateUplink.size);
      memcpy(&qosToAdd->grbQosInfo.guarFlowBitRateDl,\
            qosFlowCfg->gBR_QoS_Flow_Information->guaranteedFlowBitRateDownlink.buf, \
            qosFlowCfg->gBR_QoS_Flow_Information->guaranteedFlowBitRateDownlink.size);
      memcpy(&qosToAdd->grbQosInfo.guarFlowBitRateUl,\
            qosFlowCfg->gBR_QoS_Flow_Information->guaranteedFlowBitRateUplink.buf, \
            qosFlowCfg->gBR_QoS_Flow_Information->guaranteedFlowBitRateUplink.size);
   }
   /*Extracting PDU_SESSION_ID*/
   qosIeExt = (ProtocolExtensionContainer_4624P74_t *)qosFlowCfg->iE_Extensions;
   if(qosIeExt)
   {
      for(qosCntIdx = 0; qosCntIdx < qosIeExt->list.count; qosCntIdx++)
      {
         if(qosIeExt->list.array[qosCntIdx]->extensionValue.present == \
               QoSFlowLevelQoSParameters_ExtIEs__extensionValue_PR_PDUSessionID)
         {
            qosToAdd->pduSessionId = qosIeExt->list.array[qosCntIdx]->extensionValue.choice.PDUSessionID;
            DU_LOG("\nDEBUG -->  DU_F1AP : extractQosInfo: PDU SessionID:%d",qosToAdd->pduSessionId);
         }
      }  
   }
   qosToAdd->ulPduSessAggMaxBitRate = 0;
}

/*******************************************************************
 *
 * @brief Function to extract GTP Tunnel Info from CU
 *
 * @details
 *
 *    Function : extractUpTnlInfo
 *
 *    Functionality: Function to extract GTP Tunnel Info from CU
 *
 * @params[in] F1AP message
 * @return ROK/RFAILED
 *
 * ****************************************************************/

uint8_t extractUpTnlInfo(uint8_t drbId, uint8_t configType,\
   ULUPTNLInformation_ToBeSetup_List_t *tnlInfo, UpTnlCfg *upTnlInfo)
{
   uint8_t tnlIdx;
   uint32_t ipv4_du = 0;
   GTPTunnel_t *gtpTunnel = NULLP;

   upTnlInfo->drbId = drbId; 
   upTnlInfo->configType = configType;
#ifdef O1_ENABLE
   cmInetAddr((S8*)g_cfg.DU_IPV4_Addr, &ipv4_du);
#else
   cmInetAddr((char *)DU_IP_V4_ADDR, &ipv4_du);
#endif

   for(tnlIdx=0; tnlIdx < tnlInfo->list.count; tnlIdx++)
   {
      if(tnlInfo->list.array[tnlIdx]->uLUPTNLInformation.present == UPTransportLayerInformation_PR_gTPTunnel)
      {
	 if(tnlInfo->list.array[tnlIdx]->uLUPTNLInformation.choice.gTPTunnel)
	 {
	    gtpTunnel = tnlInfo->list.array[tnlIdx]->uLUPTNLInformation.choice.gTPTunnel;
	    DU_ALLOC(upTnlInfo->tnlCfg1, sizeof(GtpTnlCfg));
            if(upTnlInfo->tnlCfg1 == NULLP)
	    {
               DU_LOG("\nERROR  -->  F1AP : extractUpTnlInfo: Failed to allocate mmeory for tunnel cfg 1");
               return RFAILED;
	    }
	    bitStringToInt(&gtpTunnel->transportLayerAddress, &upTnlInfo->tnlCfg1->ulTnlAddress);
	    upTnlInfo->tnlCfg1->dlTnlAddress = ipv4_du;
	    if(gtpTunnel->gTP_TEID.size > 0)
	    {
	       teIdStringToInt(gtpTunnel->gTP_TEID.buf, &upTnlInfo->tnlCfg1->teId);
	    }
	 }
	 break;
      }
   }
   return ROK;
}

/*******************************************************************
*
* @brief Function to extract Drb Qos Cfg Info from CU
*
* @details
*
*    Function : extractDrbQosCfg 
*
*    Functionality: Function to extract Drb Qos Cfg Info from CU
*
* @params[in] DRB_Information_t *drbInfo, LcCfg *macLcToAdd
* @return ROK/RFAILED
*
* ****************************************************************/

uint8_t extractDrbQosCfg(DRB_Information_t *drbInfo, LcCfg *macLcToAdd ) 
{
   if(!macLcToAdd->drbQos)
   {
      DU_ALLOC_SHRABL_BUF(macLcToAdd->drbQos, sizeof(DrbQosInfo));
      if(macLcToAdd->drbQos == NULLP)
      {
         DU_LOG("\nERROR  -->  DUAPP:Memory failed at allocating DrbQos at extractDrbQosCfg()");
         return RFAILED;
      }

   }
   if(drbInfo->dRB_QoS.qoS_Characteristics.present == QoS_Characteristics_PR_non_Dynamic_5QI)
   {
      extractQosInfo(macLcToAdd->drbQos, &drbInfo->dRB_QoS);
      macLcToAdd->dlLcCfg.lcp = macLcToAdd->drbQos->ngRanRetPri.priorityLevel;
   }
   if(extractDrbSnssaiCfg(&drbInfo->sNSSAI, &macLcToAdd->snssai) != ROK)
   {
      DU_LOG("\nERROR  -->  DUAPP: Unable to extract Snssai information  at extractDrbQosCfg()");
      return RFAILED;
   }
   return ROK;
}
/*******************************************************************
 *
 * @brief Function to extract DRB info received from CU
 *
 * @details
 *
 *    Function : extractDrbCfg
 *
 *    Functionality: Function to extract DRB info received from CU
 *
 * @params[in] F1AP message
 * @return void
 *
 * ****************************************************************/
uint8_t extractDrbCfg(DRBs_ToBeSetup_Item_t *drbItem, DRBs_ToBeSetupMod_Item_t *drbSetupModItem,\
DRBs_ToBeModified_Item_t *drbModItem,  DuLcCfg *lcCfgToAdd, UpTnlCfg *upTnlInfo)
{
   DRB_Information_t *drbInfo = NULLP;

   if(drbItem != NULLP)
   {
      if(extractUpTnlInfo(drbItem->dRBID, CONFIG_ADD, &drbItem->uLUPTNLInformation_ToBeSetup_List, upTnlInfo) != ROK)
      {
         DU_LOG("\nERROR  -->  DUAPP : Failed to extract tunnel Cfg at extractDrbCfg()");
         return RFAILED;
      }
      if(drbItem->qoSInformation.present == QoSInformation_PR_choice_extension)
      {
         if(drbItem->qoSInformation.choice.choice_extension->value.present == QoSInformation_ExtIEs__value_PR_DRB_Information)
         {
            drbInfo = &drbItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information;
            if(extractDrbQosCfg(drbInfo , &lcCfgToAdd->lcConfig) != ROK)
            {
               DU_LOG("\nERROR  -->  DUAPP : Failed to extract qos Cfg at extractDrbCfg()");
               return RFAILED;
            }
         }
      }
   }
   else if(drbSetupModItem != NULLP)
   {
      if(extractUpTnlInfo(drbSetupModItem->dRBID, CONFIG_ADD, &drbSetupModItem->uLUPTNLInformation_ToBeSetup_List,\
      upTnlInfo) != ROK)
      {
         DU_LOG("\nERROR  -->  DUAPP : Failed to extract tunnel Cfg at extractDrbCfg()");
         return RFAILED;
      }
      if(drbSetupModItem->qoSInformation.present == QoSInformation_PR_choice_extension)
      {
         if(drbSetupModItem->qoSInformation.choice.choice_extension->value.present ==\
         QoSInformation_ExtIEs__value_PR_DRB_Information)
         {
            drbInfo = &drbSetupModItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information;
            if(extractDrbQosCfg(drbInfo , &lcCfgToAdd->lcConfig) != ROK)
            {
               DU_LOG("\nERROR  -->  DUAPP : Failed to extract qos Cfg at extractDrbCfg()");  
               return RFAILED;
            }

         }
      }
   }
   else if(drbModItem != NULLP)
   {
      if(extractUpTnlInfo(drbModItem->dRBID, CONFIG_MOD, &drbModItem->uLUPTNLInformation_ToBeSetup_List,\
      upTnlInfo) != ROK)
      {
         DU_LOG("\nERROR  -->  DUAPP : Failed to extract tunnel Cfg at extractDrbCfg()");
         return RFAILED;
      }
      if(drbModItem->qoSInformation != NULLP)
      {
         if(drbModItem->qoSInformation->present == QoSInformation_PR_choice_extension)
         {
            if(drbModItem->qoSInformation->choice.choice_extension->value.present ==\
                  QoSInformation_ExtIEs__value_PR_DRB_Information)
            {
               drbInfo = &drbModItem->qoSInformation->choice.choice_extension->value.choice.DRB_Information;
               if(extractDrbQosCfg(drbInfo , &lcCfgToAdd->lcConfig) != ROK)
               {
                  DU_LOG("\nERROR  -->  DUAPP : Failed to extract qos Cfg at extractDrbCfg()");  
                  return RFAILED;
               }

            }
         }
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Function to extract RB info received from CU
 *
 * @details
 *
 *    Function : extractMacRbCfg
 *
 *    Functionality: Function to extract RB info received from CU
 *
 * @params[in] F1AP message
 * @return ROK/RFAILED
 *
 * ****************************************************************/

uint8_t extractMacRbCfg(uint8_t lcId, DRBs_ToBeSetup_Item_t *drbCfg,\
DRBs_ToBeSetupMod_Item_t *drbSetupModCfg,  DRBs_ToBeModified_Item_t *drbModCfg, LogicalChannelConfig_t *ulLcCfg, DuLcCfg *lcCfg, UpTnlCfg *upTnlInfo)
{
   if(drbCfg != NULLP)
   {
      if(extractDrbCfg(drbCfg, NULL, NULL, lcCfg, upTnlInfo) != ROK)
      {
         DU_LOG("ERROR  -->  F1AP : Failed to build Drb Qos at extractMacRbCfg()");
         return RFAILED;
      }
   }
   else if(drbSetupModCfg != NULLP)
   { 
      if(extractDrbCfg(NULL, drbSetupModCfg, NULL, lcCfg, upTnlInfo) != ROK)
      {
         DU_LOG("ERROR  -->  F1AP : Failed to build Drb Qos at extractMacRbCfg()");
         return RFAILED;
      }
   }
   else if(drbModCfg != NULLP)
   { 
      if(extractDrbCfg(NULL, NULL, drbModCfg, lcCfg, upTnlInfo) != ROK)
      {
         DU_LOG("ERROR  -->  F1AP : Failed to build Drb Qos at extractMacRbCfg()");
         return RFAILED;
      }
   }
   else
   {
      lcCfg->lcConfig.drbQos = NULLP;
      lcCfg->lcConfig.snssai = NULLP;
      if(lcCfg->lcConfig.lcId == SRB2_LCID)
         lcCfg->lcConfig.dlLcCfg.lcp = LC_PRIORITY_3;
      else
         lcCfg->lcConfig.dlLcCfg.lcp = LC_PRIORITY_1;

   }
   if(ulLcCfg)
   {
      lcCfg->lcConfig.ulLcCfgPres = true;
      extractUlLcCfg(&lcCfg->lcConfig.ulLcCfg, ulLcCfg);
   }
   else
      lcCfg->lcConfig.ulLcCfgPres = false;
   return ROK;
}

/*******************************************************************
 *
 * @brief Function processing LC config info received from CU
 *
 * @details
 *
 *    Function : procMacLcCfg
 *
 *    Functionality: Function processing LC config info received from CU
 *
 * @params[in] F1AP message
 * @return ROK/RFAILED
 *
 * ****************************************************************/

uint8_t procMacLcCfg(uint8_t lcId, uint8_t rbType, uint8_t configType, DRBs_ToBeSetup_Item_t *drbItem,\
DRBs_ToBeSetupMod_Item_t *drbSetupModItem, DRBs_ToBeModified_Item_t *drbModItem, LogicalChannelConfig_t *ulLcCfg,\
DuLcCfg *lcCfg, UpTnlCfg *upTnlInfo)
{
   uint8_t ret = ROK;

   lcCfg->lcConfig.lcId = lcId;
   lcCfg->configType = configType;
   if(rbType == RB_TYPE_SRB)
   {
      ret = extractMacRbCfg(lcId, NULL, NULL, NULL, ulLcCfg, lcCfg, NULL);
   }
   else if(rbType == RB_TYPE_DRB && upTnlInfo != NULLP)
   {
      if(drbItem != NULL)
        ret = extractMacRbCfg(lcId, drbItem, NULL, NULL, ulLcCfg, lcCfg, upTnlInfo);
      else if(drbSetupModItem != NULL)
        ret = extractMacRbCfg(lcId, NULL, drbSetupModItem, NULL, ulLcCfg, lcCfg, upTnlInfo);
      else if(drbModItem != NULL)
        ret = extractMacRbCfg(lcId, NULL, NULL, drbModItem, ulLcCfg, lcCfg, upTnlInfo);
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Function to extract Rlc cfg To Add/Mod in CellGrp Info
 *
 * @details
 *
 *    Function : extractRlcCfgToAddMod
 *
 *    Functionality: Function to extract Rlc cfg To Add/Mod in CellGrp Info
 *
 * @params[in] CellGroupConfigRrc__rlc_BearerTo pointer
 *             DuUeCfg Pointer
 * @return ROK/RFAILED
 *
 * ****************************************************************/

uint8_t extractRlcCfgToAddMod(struct CellGroupConfigRrc__rlc_BearerToAddModList *lcCfg, DuUeCfg *ueCfgDb)
{
  uint8_t idx, rbId, lcId, rlcMode, rbType;
  RLC_Config_t *f1RlcCfg = NULLP;
  LogicalChannelConfig_t *macUlLcCfg = NULLP;

  for(idx = 0; idx < lcCfg->list.count; idx++)
  {
     lcId = lcCfg->list.array[idx]->logicalChannelIdentity;
     if(lcCfg->list.array[idx]->servedRadioBearer)
     {
        /* RadioBearer for SRB/DRB */
        if(lcCfg->list.array[idx]->servedRadioBearer->present ==\
        RLC_BearerConfig__servedRadioBearer_PR_srb_Identity)
        {
           rbId = lcCfg->list.array[idx]->servedRadioBearer->choice.srb_Identity;
           rbType = RB_TYPE_SRB;
        }
        else if(lcCfg->list.array[idx]->servedRadioBearer->present ==\
        RLC_BearerConfig__servedRadioBearer_PR_drb_Identity)
        {
           rbId = lcCfg->list.array[idx]->servedRadioBearer->choice.drb_Identity;
           rbType = RB_TYPE_DRB;
        }
        else
        {
           DU_LOG("\nERROR  -->  No components present in Bearer config to ADD/MOD");
           return RFAILED;
        }
        /* MAC UL LC Config */
        if(lcCfg->list.array[idx]->mac_LogicalChannelConfig)
        {
           macUlLcCfg = lcCfg->list.array[idx]->mac_LogicalChannelConfig;
        }
     }
     else
     {
        DU_LOG("\nERROR  -->  DUAPP: Received RadioBearer config is NULL");
        return RFAILED;
     }
     /* RLC Mode Config */
     if(lcCfg->list.array[idx]->rlc_Config)
     {
        rlcMode  = lcCfg->list.array[idx]->rlc_Config->present;
        f1RlcCfg = lcCfg->list.array[idx]->rlc_Config;
     }
     
     /* Filling RLC/MAC Config*/
     memset(&ueCfgDb->macLcCfg[idx], 0, sizeof(DuLcCfg));
     memset(&ueCfgDb->rlcLcCfg[idx], 0, sizeof(DuRlcBearerCfg));
     procRlcLcCfg(rbId, lcId, rbType, rlcMode, CONFIG_UNKNOWN, f1RlcCfg, &(ueCfgDb->rlcLcCfg[idx]), NULLP);
     if(procMacLcCfg(lcId, rbType, CONFIG_UNKNOWN, NULL, NULL, NULL, macUlLcCfg, &ueCfgDb->macLcCfg[idx], NULL) != ROK)
     {
        DU_LOG("\nERROR  -->  DU APP : Failed while filling MAC LC config at extractRlcCfgToAddMod()");
        return RFAILED;
     }
     (ueCfgDb->numRlcLcs)++;
     (ueCfgDb->numMacLcs)++;
     DU_LOG("\nDEBUG  -> DUAPP: extractRlcCfgToAddMod:RBType:%d, DrbID: %d,lcId:%d, [RLC,MAC,NumDrb]:[%x,%x,%x]",\
        rbType, rbId, lcId, ueCfgDb->numRlcLcs, ueCfgDb->numMacLcs,  ueCfgDb->numDrb);
  }
  //TODO: To send the failure cause in UeContextSetupRsp 
  return ROK;
}

/*******************************************************************
 *
 * @brief DeAlloc pdsch serv cell config info
 *
 * @details
 *
 *    Function : freeMacPdschServCellInfo
 *
 *    Functionality: DeAlloc pdsch serv cell config info
 *
 * @params[in] PdschServCellCfg pointer
 * @return void
 *
 * ****************************************************************/

void freeMacPdschServCellInfo(PdschServCellCfg *pdsch)
{
   if(pdsch->xOverhead)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, pdsch->xOverhead, sizeof(PdschXOverhead));
   }
   if(pdsch->codeBlkGrpFlushInd)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, pdsch->codeBlkGrpFlushInd, sizeof(bool));
   }
   if(pdsch->maxCodeBlkGrpPerTb)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, pdsch->maxCodeBlkGrpPerTb, sizeof(MaxCodeBlkGrpPerTB));
   }
   if(pdsch->maxMimoLayers)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, pdsch->maxMimoLayers, sizeof(uint8_t));
   }
}

/*******************************************************************
 *
 * @brief Free Serving cell Info
 *
 * @details
 *
 *    Function : freeMacServingCellInfo
 *
 *    Functionality: Free Serving cell Info
 *
 * @params[in] ServCellCfgInfo *srvCellCfg
 * @return void
 *
 * ****************************************************************/
void freeMacServingCellInfo(ServCellRecfgInfo *srvCellCfg)
{
   uint8_t timeDomRsrcIdx;

   if(srvCellCfg->initDlBwp.pdschPresent)
   {
      for(timeDomRsrcIdx = 0; timeDomRsrcIdx < srvCellCfg->initDlBwp.pdschCfg.numTimeDomRsrcAlloc; timeDomRsrcIdx++)
      {
         DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, \
            srvCellCfg->initDlBwp.pdschCfg.timeDomRsrcAllociList[timeDomRsrcIdx].k0, sizeof(uint8_t));
      }
   }

   freeMacPdschServCellInfo(&srvCellCfg->pdschServCellCfg);
   if(srvCellCfg->bwpInactivityTmr)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, srvCellCfg->bwpInactivityTmr, sizeof(uint8_t));
   }

   if(srvCellCfg->initUlBwp.pucchPresent)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, srvCellCfg->initUlBwp.pucchCfg.dlDataToUlAck, sizeof(PucchDlDataToUlAck));
   }
}

/*******************************************************************
 *
 * @brief Free cell Grp Cfg Info
 *
 * @details
 *
 *    Function : freeUeRecfgCellGrpInfo
 *
 *    Functionality: Free cell Grp Cfg Info
 *
 * @params[in] DuMacUeCfg*  duUeCfg
 * @return void
 *
 * ****************************************************************/

void freeUeRecfgCellGrpInfo(DuMacUeCfg *macUeCfg)
{
   freeMacServingCellInfo(&macUeCfg->spCellCfg.servCellCfg);
}

/*******************************************************************
 *
 * @brief Fills Reconfig SchReqReConfig
 *
 * @details
 *
 *    Function : extractSchReqReConfig
 *
 *    Functionality: Fills Reconfig SchReqReConfig
 *
 * @params[in] SchedulingRequestConfig_t *cuSchedReq
 *             SchedReqCfg*  macSchedReq
 * @return void
 *
 * ****************************************************************/
void extractSchReqReConfig(SchedulingRequestConfig_t *cuSchedReq, SchedReqCfg *macSchedReq)
{
   uint8_t schReqIdx = 0;
   struct SchedulingRequestConfig__schedulingRequestToAddModList *schReqListToAdd = NULLP;
   struct SchedulingRequestConfig__schedulingRequestToReleaseList *schReqListToRel = NULLP;

   if(cuSchedReq->schedulingRequestToAddModList)
   {
      schReqListToAdd = cuSchedReq->schedulingRequestToAddModList;
      if(schReqListToAdd->list.count)
      {
	 macSchedReq->addModListCount = schReqListToAdd->list.count;
	 for(schReqIdx = 0; schReqIdx < schReqListToAdd->list.count; schReqIdx++)
	 {
	    macSchedReq->addModList[schReqIdx].schedReqId = \
	       schReqListToAdd->list.array[schReqIdx]->schedulingRequestId;
	    macSchedReq->addModList[schReqIdx].srProhibitTmr = \
	       *(schReqListToAdd->list.array[schReqIdx]->sr_ProhibitTimer);
	    macSchedReq->addModList[schReqIdx].srTransMax    =\
	       schReqListToAdd->list.array[schReqIdx]->sr_TransMax;
	 }
      }
   }
   /* Scheduling Req To release */
   if(cuSchedReq->schedulingRequestToReleaseList)
   {
      schReqListToRel = cuSchedReq->schedulingRequestToReleaseList;
      if(schReqListToRel->list.count)
      {
	 macSchedReq->relListCount = schReqListToRel->list.count;
	 for(schReqIdx = 0; schReqIdx < schReqListToRel->list.count; schReqIdx++)
	 {
	    macSchedReq->relList[schReqIdx] = \
	       *schReqListToRel->list.array[schReqIdx];
	 }
      }
   }
}

/*******************************************************************
 *
 * @brief Fills TagReconfig
 *
 * @details
 *
 *    Function : extractTagReconfig
 *
 *    Functionality: Fills extractTagReconfig
 *
 * @params[in] TAG_Config_t *cuTagCfg
 *             TagCfg *macTagCfg
 * @return void
 *
 * ****************************************************************/

void extractTagReconfig(TAG_Config_t *cuTagCfg, TagCfg *macTagCfg)
{
  uint8_t tagIdx = 0;
  struct TAG_Config__tag_ToAddModList  *tagListToAddMod = NULLP;
  struct TAG_Config__tag_ToReleaseList *tagListToRel = NULLP;

  /* Tag config to AddMod */
  if(cuTagCfg->tag_ToAddModList)
  {
     tagListToAddMod = cuTagCfg->tag_ToAddModList; 
     if(tagListToAddMod->list.count)
     {
        macTagCfg->addModListCount = tagListToAddMod->list.count;
        for(tagIdx = 0; tagIdx < tagListToAddMod->list.count; tagIdx++)
	{
           macTagCfg->addModList[tagIdx].tagId =\
              tagListToAddMod->list.array[tagIdx]->tag_Id;     
           macTagCfg->addModList[tagIdx].timeAlignTimer = \

              tagListToAddMod->list.array[tagIdx]->timeAlignmentTimer;
	}
     }
  }
  /* Tag config to release */
  if(cuTagCfg->tag_ToReleaseList)
  {
     tagListToRel = cuTagCfg->tag_ToReleaseList;
     if(tagListToRel->list.count)
     {
        macTagCfg->relListCount = tagListToRel->list.count;
        for(tagIdx = 0; tagIdx < tagListToRel->list.count; tagIdx++)
	{
           macTagCfg->relList[tagIdx] = *tagListToRel->list.array[tagIdx];     
	}
     }
  }
}

/*******************************************************************
 *
 * @brief Fills SlotOffset value for SearchSpace received by CU
 *
 * @details
 *
 *    Function : extractSlotOffset
 *
 *    Functionality: Fills SlotOffset received  by CU
 *
 * @params[in] SearchSpace__monitoringSlotPeriodicityAndOffset *mSlotPeriodicityOffset,
 *             uint16_t *slotOffset
 * @return void
 *
 *
 ****************************************************************/
void extractSlotOffset(struct SearchSpace__monitoringSlotPeriodicityAndOffset *mSlotPeriodicityAndOffset, uint16_t *slotOffset)
{
   switch(mSlotPeriodicityAndOffset->present)
   {
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_NOTHING:
         *slotOffset = NULLD;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl1:
         *slotOffset = mSlotPeriodicityAndOffset->choice.sl1;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl2:
         *slotOffset = mSlotPeriodicityAndOffset->choice.sl2;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl4:
         *slotOffset = mSlotPeriodicityAndOffset->choice.sl4;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl5:
         *slotOffset = mSlotPeriodicityAndOffset->choice.sl5;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl8:
         *slotOffset = mSlotPeriodicityAndOffset->choice.sl8;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl10:
         *slotOffset = mSlotPeriodicityAndOffset->choice.sl10;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl16:
         *slotOffset = mSlotPeriodicityAndOffset->choice.sl16;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl20:
         *slotOffset = mSlotPeriodicityAndOffset->choice.sl20;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl40:
         *slotOffset = mSlotPeriodicityAndOffset->choice.sl40;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl80:
         *slotOffset = mSlotPeriodicityAndOffset->choice.sl80;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl160:
         *slotOffset = mSlotPeriodicityAndOffset->choice.sl160;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl320:
         *slotOffset = mSlotPeriodicityAndOffset->choice.sl320;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl640:
         *slotOffset = mSlotPeriodicityAndOffset->choice.sl640;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl1280:
         *slotOffset = mSlotPeriodicityAndOffset->choice.sl1280;
         break;
      case SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl2560:
         *slotOffset = mSlotPeriodicityAndOffset->choice.sl2560;
         break;
      default:
         *slotOffset = 0;
   }
}


/*******************************************************************
 *
 * @brief Fills PdcchCfg received by CU
 *
 * @details
 *
 *    Function : extractPdcchCfg
 *
 *    Functionality: Fills PdcchCfg received  by CU
 *
 * @params[in] PDCCH_Config_t *cuPdcchCfg,
 *             PdcchConfig *duPdcchCfg
 * @return void
 *
 * ****************************************************************/

void extractPdcchCfg(PDCCH_Config_t *cuPdcchCfg, PdcchConfig *macPdcchCfg)
{
   uint8_t cRsetIdx = 0;
   uint8_t srchSpcIdx = 0;

   struct PDCCH_Config__controlResourceSetToAddModList *cRsetToAddModList = NULLP;
   struct PDCCH_Config__controlResourceSetToReleaseList *cRsetToRelList = NULLP;
   struct PDCCH_Config__searchSpacesToAddModList *srchSpcToAddModList = NULLP;
   struct PDCCH_Config__searchSpacesToReleaseList *srchSpcToRelList = NULLP;


   /* Control Resource Set To Add/Mod List */
   if(cuPdcchCfg->controlResourceSetToAddModList)
   {
      cRsetToAddModList = cuPdcchCfg->controlResourceSetToAddModList;
      if(cRsetToAddModList->list.count)
      {
         macPdcchCfg->numCRsetToAddMod = cRsetToAddModList->list.count;
         for(cRsetIdx = 0; cRsetIdx < cRsetToAddModList->list.count; cRsetIdx++)
         {
            macPdcchCfg->cRSetToAddModList[cRsetIdx].cRSetId = \
               cRsetToAddModList->list.array[cRsetIdx]->controlResourceSetId;
            //freqDomRsrcBitStringToInt(&cRsetToAddModList->list.array[cRsetIdx]->frequencyDomainResources,\
                  macPdcchCfg->cRSetToAddModList[cRsetIdx].freqDomainRsrc);
            memcpy(macPdcchCfg->cRSetToAddModList[cRsetIdx].freqDomainRsrc, \
               cRsetToAddModList->list.array[cRsetIdx]->frequencyDomainResources.buf,
               cRsetToAddModList->list.array[cRsetIdx]->frequencyDomainResources.size);

            macPdcchCfg->cRSetToAddModList[cRsetIdx].duration = \
                cRsetToAddModList->list.array[cRsetIdx]->duration;

            macPdcchCfg->cRSetToAddModList[cRsetIdx].cceRegMappingType = \
               cRsetToAddModList->list.array[cRsetIdx]->cce_REG_MappingType.present;   
            if(macPdcchCfg->cRSetToAddModList[cRsetIdx].cceRegMappingType == CCE_REG_MAPPINGTYPE_PR_INTERLEAVED)
            {
               //TODO: handle the case for Interleaved
            }
            macPdcchCfg->cRSetToAddModList[cRsetIdx].precoderGranularity = \
                cRsetToAddModList->list.array[cRsetIdx]->precoderGranularity;
            if(cRsetToAddModList->list.array[cRsetIdx]->pdcch_DMRS_ScramblingID)
            {
               macPdcchCfg->cRSetToAddModList[cRsetIdx].dmrsScramblingId= \
                  *(cRsetToAddModList->list.array[cRsetIdx]->pdcch_DMRS_ScramblingID);
            }
         }
      }
   }
   /* Control Resource Set To Release List */
   if(cuPdcchCfg->controlResourceSetToReleaseList)
   {
      cRsetToRelList = cuPdcchCfg->controlResourceSetToReleaseList;
      if(cRsetToRelList->list.count)
      {
         macPdcchCfg->numCRsetToRel = cRsetToRelList->list.count;
         for(cRsetIdx = 0; cRsetIdx < cRsetToRelList->list.count; cRsetIdx++)
         {
            macPdcchCfg->cRSetToRelList[cRsetIdx] = *(cRsetToRelList->list.array[cRsetIdx]);
         }
      }
   }

   /* Search space To Add/Mod List */
   if(cuPdcchCfg->searchSpacesToAddModList)
   {
      srchSpcToAddModList = cuPdcchCfg->searchSpacesToAddModList;
      if(srchSpcToAddModList->list.count)
      {
         macPdcchCfg->numSearchSpcToAddMod = srchSpcToAddModList->list.count;
         for(srchSpcIdx = 0; srchSpcIdx < srchSpcToAddModList->list.count; srchSpcIdx++)
         {
            macPdcchCfg->searchSpcToAddModList[srchSpcIdx].searchSpaceId =\
                                                                          srchSpcToAddModList->list.array[srchSpcIdx]->searchSpaceId;
            macPdcchCfg->searchSpcToAddModList[srchSpcIdx].cRSetId =\
                                                                    *(srchSpcToAddModList->list.array[srchSpcIdx]->controlResourceSetId);
            if(srchSpcToAddModList->list.array[srchSpcIdx]->monitoringSlotPeriodicityAndOffset)
            {
               macPdcchCfg->searchSpcToAddModList[srchSpcIdx].mSlotPeriodicityAndOffset.mSlotPeriodicity =\
                                               srchSpcToAddModList->list.array[srchSpcIdx]->monitoringSlotPeriodicityAndOffset->present;
               extractSlotOffset(srchSpcToAddModList->list.array[srchSpcIdx]->monitoringSlotPeriodicityAndOffset,\
                                               &macPdcchCfg->searchSpcToAddModList[srchSpcIdx].mSlotPeriodicityAndOffset.mSlotOffset);
            }
            if(srchSpcToAddModList->list.array[srchSpcIdx]->monitoringSymbolsWithinSlot)
            {
               bitStringToInt(srchSpcToAddModList->list.array[srchSpcIdx]->monitoringSymbolsWithinSlot,\
                     macPdcchCfg->searchSpcToAddModList[srchSpcIdx].mSymbolsWithinSlot);
            }
            if(srchSpcToAddModList->list.array[srchSpcIdx]->nrofCandidates)
            {
               macPdcchCfg->searchSpcToAddModList[srchSpcIdx].numCandidatesAggLevel1 = \
                                                                                       srchSpcToAddModList->list.array[srchSpcIdx]->nrofCandidates->aggregationLevel1;
               macPdcchCfg->searchSpcToAddModList[srchSpcIdx].numCandidatesAggLevel2 = \
                                                                                       srchSpcToAddModList->list.array[srchSpcIdx]->nrofCandidates->aggregationLevel2;
               macPdcchCfg->searchSpcToAddModList[srchSpcIdx].numCandidatesAggLevel4 = \
                                                                                       srchSpcToAddModList->list.array[srchSpcIdx]->nrofCandidates->aggregationLevel4;

               macPdcchCfg->searchSpcToAddModList[srchSpcIdx].numCandidatesAggLevel8 = \
                                                                                       srchSpcToAddModList->list.array[srchSpcIdx]->nrofCandidates->aggregationLevel8;

               macPdcchCfg->searchSpcToAddModList[srchSpcIdx].numCandidatesAggLevel16 = \
                                                                                        srchSpcToAddModList->list.array[srchSpcIdx]->nrofCandidates->aggregationLevel16;
            }
            if(srchSpcToAddModList->list.array[srchSpcIdx]->searchSpaceType)
            {
               macPdcchCfg->searchSpcToAddModList[srchSpcIdx].searchSpaceType =\
                                                                               srchSpcToAddModList->list.array[srchSpcIdx]->searchSpaceType->present;
               if(macPdcchCfg->searchSpcToAddModList[srchSpcIdx].searchSpaceType == SEARCHSPACETYPE_PR_UE_SPECIFIC)
               {
                  macPdcchCfg->searchSpcToAddModList[srchSpcIdx].ueSpecificDciFormat =\
                                                                                      srchSpcToAddModList->list.array[srchSpcIdx]->searchSpaceType->choice.ue_Specific->dci_Formats;
               }

            }
         }
      }
   }
   /* Search space To Rel List */
   if(cuPdcchCfg->searchSpacesToReleaseList)
   {
      srchSpcToRelList = cuPdcchCfg->searchSpacesToReleaseList;
      if(srchSpcToRelList->list.count)
      {
         macPdcchCfg->numSearchSpcToRel = srchSpcToRelList->list.count;
         for(srchSpcIdx = 0; srchSpcIdx < srchSpcToRelList->list.count; srchSpcIdx++)
         {
            macPdcchCfg->searchSpcToRelList[srchSpcIdx] =\
                                                         *(srchSpcToRelList->list.array[srchSpcIdx]);
         }
      }
   }
}

/*******************************************************************
 *
 * @brief Fills PdschCfg received by CU
 *
 * @details
 *
 *    Function : extractPdschCfg
 *
 *    Functionality: Fills PdschCfg received  by CU
 *
 * @params[in] PDSCH_Config_t *cuPdschCfg = Information which is send by CU,
 *                   which we have stored in F1UeContextSetupDb,
 *             PdschConfig *macPdschCfg = Used to Store the information which
 *                   needs to send in other layer, as well as this can be the variable
 *                   which stores the information in DuCb,
 *             PdschConfig *storedPdschCfg =  Null in case of sending the
 *                   information to other layer else it will have stored pdsch 
 *                   configuration in copyOfmacUeCfg.
 * @return void
 *
 * ****************************************************************/

void extractPdschCfg(PDSCH_Config_t *cuPdschCfg, PdschConfig *macPdschCfg, PdschConfig *storedPdschCfg)
{
   uint8_t timeDomIdx;
   struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAlloc = NULLP;

   if(cuPdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA)
   {
      if(cuPdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA->present == \
            PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA_PR_setup)
      {
         if(cuPdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA->choice.setup)
         {
            macPdschCfg->dmrsDlCfgForPdschMapTypeA.addPos = \
               *(cuPdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA->choice.setup->dmrs_AdditionalPosition);
         }
      }
   }
   macPdschCfg->resourceAllocType = cuPdschCfg->resourceAllocation;
   if(cuPdschCfg->pdsch_TimeDomainAllocationList)
   {
      timeDomAlloc = cuPdschCfg->pdsch_TimeDomainAllocationList;
      if(timeDomAlloc->present ==\
            PDSCH_Config__pdsch_TimeDomainAllocationList_PR_setup)
      {
         if(timeDomAlloc->choice.setup)
         {
            macPdschCfg->numTimeDomRsrcAlloc  = timeDomAlloc->choice.setup->list.count;
            for(timeDomIdx = 0; timeDomIdx < timeDomAlloc->choice.setup->list.count; timeDomIdx++)
            {
               macPdschCfg->timeDomRsrcAllociList[timeDomIdx].k0 = NULLP;
               if(timeDomAlloc->choice.setup->list.array[timeDomIdx]->k0)
               {
                  if(macPdschCfg->timeDomRsrcAllociList[timeDomIdx].k0 == NULL)
                  {
                     if(storedPdschCfg)
                     {
                        if(storedPdschCfg->timeDomRsrcAllociList[timeDomIdx].k0)
                        {
                           macPdschCfg->timeDomRsrcAllociList[timeDomIdx].k0 =\
                           storedPdschCfg->timeDomRsrcAllociList[timeDomIdx].k0;
                        }
                        else
                        {
                           DU_ALLOC_SHRABL_BUF(macPdschCfg->timeDomRsrcAllociList[timeDomIdx].k0, sizeof(uint8_t));
                        }
                     }
                     else
                     {
                        DU_ALLOC_SHRABL_BUF(macPdschCfg->timeDomRsrcAllociList[timeDomIdx].k0, sizeof(uint8_t));
                     }
                     if(!macPdschCfg->timeDomRsrcAllociList[timeDomIdx].k0)
                     {
                        DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for k0 at extractPdschCfg()");
                        return;
                     }
                  }
                  *(macPdschCfg->timeDomRsrcAllociList[timeDomIdx].k0) = \
                  *(timeDomAlloc->choice.setup->list.array[timeDomIdx]->k0);
               }
               macPdschCfg->timeDomRsrcAllociList[timeDomIdx].mappingType = \
                  timeDomAlloc->choice.setup->list.array[timeDomIdx]->mappingType;
               macPdschCfg->timeDomRsrcAllociList[timeDomIdx].startSymbolAndLength = \
                  timeDomAlloc->choice.setup->list.array[timeDomIdx]->startSymbolAndLength;
            }
         }
      }
   }
   macPdschCfg->rbgSize = cuPdschCfg->rbg_Size;
   if(cuPdschCfg->maxNrofCodeWordsScheduledByDCI)
      macPdschCfg->numCodeWordsSchByDci = *(cuPdschCfg->maxNrofCodeWordsScheduledByDCI);
   if(cuPdschCfg->prb_BundlingType.present == PDSCH_Config__prb_BundlingType_PR_staticBundling)
   {
      macPdschCfg->bundlingType = cuPdschCfg->prb_BundlingType.present;
      if(cuPdschCfg->prb_BundlingType.choice.staticBundling)
      {
         if(cuPdschCfg->prb_BundlingType.choice.staticBundling->bundleSize)
         {
            macPdschCfg->bundlingInfo.StaticBundling.size = \
               *(cuPdschCfg->prb_BundlingType.choice.staticBundling->bundleSize);
         }
      }
   }
   else if(cuPdschCfg->prb_BundlingType.present == PDSCH_Config__prb_BundlingType_PR_dynamicBundling)
   {
      macPdschCfg->bundlingType = cuPdschCfg->prb_BundlingType.present;
   }

}

/*******************************************************************
 *
 * @brief Fills PdschServingCellCfg received by CU
 *
 * @details
 *
 *    Function : extractPdschServingCellCfg
 *
 *    Functionality: Fills PdschCfg received  by CU
 *
 * @params[in] PDSCH_ServingCellConfig_t *cuPdschSrvCellCfg,
 *             PdschServCellCfg *macUePdschSrvCellCfg
 * @return ROK/RFAILED
 *
 * ****************************************************************/

uint8_t extractPdschServingCellCfg(PDSCH_ServingCellConfig_t *cuPdschSrvCellCfg, PdschServCellCfg *macUePdschSrvCellCfg)
{
   if(cuPdschSrvCellCfg->codeBlockGroupTransmission)
   {
      if(cuPdschSrvCellCfg->codeBlockGroupTransmission->choice.setup)
      {
         if(macUePdschSrvCellCfg->maxCodeBlkGrpPerTb)
         {
            *(macUePdschSrvCellCfg->maxCodeBlkGrpPerTb)  = \
	       cuPdschSrvCellCfg->codeBlockGroupTransmission->choice.setup->maxCodeBlockGroupsPerTransportBlock;
	 }
	 else
	 {
            DU_ALLOC_SHRABL_BUF(macUePdschSrvCellCfg->maxCodeBlkGrpPerTb, sizeof(MaxCodeBlkGrpPerTB));
            if(macUePdschSrvCellCfg->maxCodeBlkGrpPerTb)
	    {
               *(macUePdschSrvCellCfg->maxCodeBlkGrpPerTb)  = \
	          cuPdschSrvCellCfg->codeBlockGroupTransmission->choice.setup->maxCodeBlockGroupsPerTransportBlock;
	    }
	    else
	    {
	       DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for maxCodeBlkGrpPerTb at extractPdschServingCellCfg()");
	       return RFAILED;
	    }
	 }
         if(macUePdschSrvCellCfg->codeBlkGrpFlushInd)
         {
            *(macUePdschSrvCellCfg->codeBlkGrpFlushInd)  = \
	       cuPdschSrvCellCfg->codeBlockGroupTransmission->choice.setup->codeBlockGroupFlushIndicator;
	 }
	 else
	 {
            DU_ALLOC_SHRABL_BUF(macUePdschSrvCellCfg->codeBlkGrpFlushInd , sizeof(bool));
            if(macUePdschSrvCellCfg->codeBlkGrpFlushInd)
	    {
               *(macUePdschSrvCellCfg->codeBlkGrpFlushInd)  = \
	          cuPdschSrvCellCfg->codeBlockGroupTransmission->choice.setup->codeBlockGroupFlushIndicator;
	    }
	    else
	    {
	       DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for codeBlkGrpFlushInd at extractPdschServingCellCfg()");
	       return RFAILED;
	    }
	 }
      }
   }
   if(cuPdschSrvCellCfg->nrofHARQ_ProcessesForPDSCH)
   {
      macUePdschSrvCellCfg->numHarqProcForPdsch = *(cuPdschSrvCellCfg->nrofHARQ_ProcessesForPDSCH); 
   }
   if(cuPdschSrvCellCfg->ext1)
   {
      if(cuPdschSrvCellCfg->ext1->maxMIMO_Layers)
      {
        if(macUePdschSrvCellCfg->maxMimoLayers)
	{
           *(macUePdschSrvCellCfg->maxMimoLayers)  = *(cuPdschSrvCellCfg->ext1->maxMIMO_Layers);
	}
	else
	{
           DU_ALLOC_SHRABL_BUF(macUePdschSrvCellCfg->maxMimoLayers, sizeof(uint8_t));
           if(macUePdschSrvCellCfg->maxMimoLayers)
	   {
              *(macUePdschSrvCellCfg->maxMimoLayers)  = *(cuPdschSrvCellCfg->ext1->maxMIMO_Layers);
	   }
	   else
	   {
	      DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for maxMimoLayers at extractPdschServingCellCfg()");
	      return RFAILED;
	   }
	}
      }
   }
   if(cuPdschSrvCellCfg->xOverhead)
   {
      if(macUePdschSrvCellCfg->xOverhead)
      {
         *(macUePdschSrvCellCfg->xOverhead)  = *(cuPdschSrvCellCfg->xOverhead);
      }
      else
      {
         DU_ALLOC_SHRABL_BUF(macUePdschSrvCellCfg->xOverhead, sizeof(PdschXOverhead));
         if(macUePdschSrvCellCfg->xOverhead)
         {
            *(macUePdschSrvCellCfg->xOverhead)  = *(cuPdschSrvCellCfg->xOverhead);
         }
         else
         {
            DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for xOverhead at extractPdschServingCellCfg()");
	    return RFAILED;
         }
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills PuschCfg received by CU
 *
 * @details
 *
 *    Function : extractPuschCfg
 *
 *    Functionality: Fills PuschCfg received  by CU
 *
 * @params[in] BWP_UplinkDedicated__pusch_Config *cuPuschCfg,
 *             PuschCfg *macPuschCfg
 * @return void
 *
 * ****************************************************************/

void extractPuschCfg(struct BWP_UplinkDedicated__pusch_Config *cuPuschCfg, PuschCfg *macPuschCfg)
{
   uint8_t timeDomIdx = 0;
   DMRS_UplinkConfig_t *dmrsUlCfg = NULLP;
   struct PUSCH_Config__pusch_TimeDomainAllocationList *timeDomAllocList = NULLP;

   if(cuPuschCfg->present == BWP_UplinkDedicated__pusch_Config_PR_setup)
   {
      if(cuPuschCfg->choice.setup)
      {
         if(cuPuschCfg->choice.setup->dataScramblingIdentityPUSCH)
	 {
             macPuschCfg->dataScramblingId = \
	        *(cuPuschCfg->choice.setup->dataScramblingIdentityPUSCH);
	 }
	 if(cuPuschCfg->choice.setup->dmrs_UplinkForPUSCH_MappingTypeA)
	 {
	    if(cuPuschCfg->choice.setup->dmrs_UplinkForPUSCH_MappingTypeA->present == PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA_PR_setup)
	    {
	       if(cuPuschCfg->choice.setup->dmrs_UplinkForPUSCH_MappingTypeA->choice.setup)
	       {
	          dmrsUlCfg = (cuPuschCfg->choice.setup->dmrs_UplinkForPUSCH_MappingTypeA->choice.setup);
	          if(dmrsUlCfg->dmrs_AdditionalPosition)
	          {
                     macPuschCfg->dmrsUlCfgForPuschMapTypeA.addPos =\
	                *(dmrsUlCfg->dmrs_AdditionalPosition);
	          }
                  if(dmrsUlCfg->transformPrecodingDisabled)
	          {
	             if(dmrsUlCfg->transformPrecodingDisabled->scramblingID0)
	             {
	                macPuschCfg->dmrsUlCfgForPuschMapTypeA.transPrecodDisabled.scramblingId0 = \
	                   *(dmrsUlCfg->transformPrecodingDisabled->scramblingID0);
                     }
	          }
	       }
	    }
	 }
	 /*Res Alloc Type for UL */
	 if(cuPuschCfg->choice.setup->resourceAllocation)
	 {
            macPuschCfg->resourceAllocType = \
	       cuPuschCfg->choice.setup->resourceAllocation;
	 }
	 if(cuPuschCfg->choice.setup->pusch_TimeDomainAllocationList)
	 {
	    timeDomAllocList = cuPuschCfg->choice.setup->pusch_TimeDomainAllocationList;
	    if(timeDomAllocList->present == PUSCH_Config__pusch_TimeDomainAllocationList_PR_setup)
	    {
               if(timeDomAllocList->choice.setup)
	       {
	          macPuschCfg->numTimeDomRsrcAlloc = timeDomAllocList->choice.setup->list.count;
	          for(timeDomIdx = 0; timeDomIdx <timeDomAllocList->choice.setup->list.count; timeDomIdx++)
		  {
		     macPuschCfg->timeDomRsrcAllocList[timeDomIdx].k2 = \
		        *(timeDomAllocList->choice.setup->list.array[timeDomIdx]->k2);
                     macPuschCfg->timeDomRsrcAllocList[timeDomIdx].mappingType = \
                        timeDomAllocList->choice.setup->list.array[timeDomIdx]->mappingType;
                     macPuschCfg->timeDomRsrcAllocList[timeDomIdx].startSymbolAndLength = \
		        timeDomAllocList->choice.setup->list.array[timeDomIdx]->startSymbolAndLength;
                  }
	       }
	    }
	 }
	 if(cuPuschCfg->choice.setup->transformPrecoder)
            macPuschCfg->transformPrecoder = *(cuPuschCfg->choice.setup->transformPrecoder);
      }
   }
}

/*******************************************************************
 *
 * @brief Function to fill pucch Power Control
 *
 * @details
 *
 *    Function : extractPucchPowerControl
 *
 *    Functionality: Function to fill pucch Power Control
 *
 * @params[in] PucchPowerControl *pwrCtrl,
 *             struct PUCCH_PowerControl *cuPwrCtrlCfg
 * @return void
 *
 * ****************************************************************/

void extractPucchPowerControl(PucchPowerControl *pwrCtrl, struct PUCCH_PowerControl *cuPwrCtrlCfg)
{
   uint8_t arrIdx;

   if(cuPwrCtrlCfg->deltaF_PUCCH_f0)
      pwrCtrl->deltaF_Format0 = *cuPwrCtrlCfg->deltaF_PUCCH_f0;
   if(cuPwrCtrlCfg->deltaF_PUCCH_f1)
      pwrCtrl->deltaF_Format1 = *cuPwrCtrlCfg->deltaF_PUCCH_f1;
   if(cuPwrCtrlCfg->deltaF_PUCCH_f2)
      pwrCtrl->deltaF_Format2 = *cuPwrCtrlCfg->deltaF_PUCCH_f2;
   if(cuPwrCtrlCfg->deltaF_PUCCH_f3)
      pwrCtrl->deltaF_Format3 = *cuPwrCtrlCfg->deltaF_PUCCH_f3;
   if(cuPwrCtrlCfg->deltaF_PUCCH_f4)
      pwrCtrl->deltaF_Format4 = *cuPwrCtrlCfg->deltaF_PUCCH_f4;
   if(cuPwrCtrlCfg->p0_Set)
   {
      pwrCtrl->p0SetCount = cuPwrCtrlCfg->p0_Set->list.count;
      for(arrIdx=0; arrIdx < pwrCtrl->p0SetCount; arrIdx++)
      {
         pwrCtrl->p0Set[arrIdx].p0PucchId =\
	    cuPwrCtrlCfg->p0_Set->list.array[arrIdx]->p0_PUCCH_Id;
         pwrCtrl->p0Set[arrIdx].p0PucchVal =\
	    cuPwrCtrlCfg->p0_Set->list.array[arrIdx]->p0_PUCCH_Value;
      }
   }
   if(cuPwrCtrlCfg->pathlossReferenceRSs)
   {
      pwrCtrl->pathLossRefRSListCount = cuPwrCtrlCfg->pathlossReferenceRSs->list.count;
      for(arrIdx = 0; arrIdx < pwrCtrl->pathLossRefRSListCount; arrIdx++)
      {
         pwrCtrl->pathLossRefRSList[arrIdx].pathLossRefRSId =\
	    cuPwrCtrlCfg->pathlossReferenceRSs->list.array[arrIdx]->pucch_PathlossReferenceRS_Id;
      }
   }
}
 
 /*******************************************************************
 *
 * @brief Function to extractResrcSetToAddModList sent by CU
 *
 * @details
 *
 *    Function : extractResrcSetToAddModList
 *
 *    Functionality: Fucntion to extractResrcSetToAddModList
 *
 * @params[in] PucchResrcSetCfg pointer,
 *             struct PUCCH_Config__resourceSetToAddModList pointer
 * @return void
 *
 * ****************************************************************/

void extractResrcSetToAddModList(PucchResrcSetCfg *macRsrcSetList, struct PUCCH_Config__resourceSetToAddModList *cuRsrcSetList)
{
   uint8_t arrIdx, rsrcListIdx;

   macRsrcSetList->resrcSetToAddModListCount = cuRsrcSetList->list.count; 
   for(arrIdx = 0; arrIdx < macRsrcSetList->resrcSetToAddModListCount; arrIdx++)
   {
      macRsrcSetList->resrcSetToAddModList[arrIdx].resrcSetId     =\
         cuRsrcSetList->list.array[arrIdx]->pucch_ResourceSetId;
      macRsrcSetList->resrcSetToAddModList[arrIdx].resrcListCount =\
         cuRsrcSetList->list.array[arrIdx]->resourceList.list.count;
      for(rsrcListIdx = 0; rsrcListIdx < macRsrcSetList->resrcSetToAddModList[arrIdx].resrcListCount; rsrcListIdx++)
      {
         macRsrcSetList->resrcSetToAddModList[arrIdx].resrcList[rsrcListIdx] =\
            *cuRsrcSetList->list.array[arrIdx]->resourceList.list.array[rsrcListIdx];
      }

      if(cuRsrcSetList->list.array[arrIdx]->maxPayloadMinus1)
      {
         macRsrcSetList->resrcSetToAddModList[arrIdx].maxPayLoadSize =\
            *cuRsrcSetList->list.array[arrIdx]->maxPayloadMinus1;
      }
      else
      {
         macRsrcSetList->resrcSetToAddModList[arrIdx].maxPayLoadSize = 0;
      }
   }
}/* End of extractResrcSetToAddModList */

/*******************************************************************
 *
 * @brief Fills extractResrcToAddModList sent by CU
 *
 * @details
 *
 *    Function : extractResrcToAddModList
 *
 *    Functionality: Fills extractResrcToAddModList
 *
 * @params[in] PucchResrcCfg pointer,
 *             struct PUCCH_Config__resourceToAddModList pointer
 * @return ROk/RFAILED
 *
 * ****************************************************************/

uint8_t extractResrcToAddModList(PucchResrcCfg *macResrcList, struct PUCCH_Config__resourceToAddModList *cuResrcList)
{
   uint8_t arrIdx;
   
   macResrcList->resrcToAddModListCount = cuResrcList->list.count;
   for(arrIdx = 0; arrIdx < macResrcList->resrcToAddModListCount; arrIdx++)
   {
      macResrcList->resrcToAddModList[arrIdx].resrcId      =\
        cuResrcList->list.array[arrIdx]->pucch_ResourceId; 
      macResrcList->resrcToAddModList[arrIdx].startPrb     =\
        cuResrcList->list.array[arrIdx]->startingPRB;
      if(cuResrcList->list.array[arrIdx]->intraSlotFrequencyHopping)
      {
         macResrcList->resrcToAddModList[arrIdx].intraFreqHop =\
           *cuResrcList->list.array[arrIdx]->intraSlotFrequencyHopping;
      }
      if(cuResrcList->list.array[arrIdx]->secondHopPRB)
      {
         macResrcList->resrcToAddModList[arrIdx].secondPrbHop =\
           *cuResrcList->list.array[arrIdx]->secondHopPRB;
      }
      /* PUCCH RSRC FORMAT */
      if(cuResrcList->list.array[arrIdx]->format.present == PUCCH_Resource__format_PR_format0)
      {
         macResrcList->resrcToAddModList[arrIdx].pucchFormat = PUCCH_FORMAT_0;
         if(cuResrcList->list.array[arrIdx]->format.choice.format0)
	 {
	    DU_ALLOC_SHRABL_BUF(macResrcList->resrcToAddModList[arrIdx].PucchFormat.format0, sizeof(PucchFormat0));
	    if(macResrcList->resrcToAddModList[arrIdx].PucchFormat.format0 == NULLP)
	    {
               DU_LOG("\nERROR  --> F1AP : Failed to allocate memory for Format0 in extractResrcToAddModList()");
	       return RFAILED;
	    }
	    macResrcList->resrcToAddModList[arrIdx].PucchFormat.format0->initialCyclicShift =\
               cuResrcList->list.array[arrIdx]->format.choice.format0->initialCyclicShift;
	    macResrcList->resrcToAddModList[arrIdx].PucchFormat.format0->numSymbols =\
               cuResrcList->list.array[arrIdx]->format.choice.format0->nrofSymbols;
	    macResrcList->resrcToAddModList[arrIdx].PucchFormat.format0->startSymbolIdx =\
               cuResrcList->list.array[arrIdx]->format.choice.format0->startingSymbolIndex;
	 }
      }
      else if(cuResrcList->list.array[arrIdx]->format.present == PUCCH_Resource__format_PR_format1)
      {
         macResrcList->resrcToAddModList[arrIdx].pucchFormat = PUCCH_FORMAT_1;
         if(cuResrcList->list.array[arrIdx]->format.choice.format1)
	 {
            DU_ALLOC_SHRABL_BUF(macResrcList->resrcToAddModList[arrIdx].PucchFormat.format1, sizeof(PucchFormat1));
	    if(macResrcList->resrcToAddModList[arrIdx].PucchFormat.format1 == NULLP)
	    {
               DU_LOG("\nERROR  --> F1AP : Failed to allocate memory for Format1 in extractResrcToAddModList()");
	       return RFAILED;
	    }
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format1->initialCyclicShift =\
                cuResrcList->list.array[arrIdx]->format.choice.format1->initialCyclicShift;
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format1->numSymbols =\
                cuResrcList->list.array[arrIdx]->format.choice.format1->nrofSymbols;
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format1->startSymbolIdx =\
                cuResrcList->list.array[arrIdx]->format.choice.format1->startingSymbolIndex;
	    macResrcList->resrcToAddModList[arrIdx].PucchFormat.format1->timeDomOCC =\
                cuResrcList->list.array[arrIdx]->format.choice.format1->timeDomainOCC;
	 }
      }
      else if(cuResrcList->list.array[arrIdx]->format.present == PUCCH_Resource__format_PR_format2)
      {
         macResrcList->resrcToAddModList[arrIdx].pucchFormat = PUCCH_FORMAT_2;
         if(cuResrcList->list.array[arrIdx]->format.choice.format2)
	 {
	    DU_ALLOC_SHRABL_BUF(macResrcList->resrcToAddModList[arrIdx].PucchFormat.format2, sizeof(PucchFormat2_3));
	    if(macResrcList->resrcToAddModList[arrIdx].PucchFormat.format2 == NULLP)
	    {
               DU_LOG("\nERROR  --> F1AP : Failed to allocate memory for Format2 in extractResrcToAddModList()");
	       return RFAILED;
	    }
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format2->numPrbs =\
                cuResrcList->list.array[arrIdx]->format.choice.format2->nrofPRBs;
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format2->numSymbols =\
                cuResrcList->list.array[arrIdx]->format.choice.format2->nrofSymbols;
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format2->startSymbolIdx =\
                cuResrcList->list.array[arrIdx]->format.choice.format2->startingSymbolIndex;
         }
      }
      else if(cuResrcList->list.array[arrIdx]->format.present == PUCCH_Resource__format_PR_format3)
      {
         macResrcList->resrcToAddModList[arrIdx].pucchFormat = PUCCH_FORMAT_3;
         if(cuResrcList->list.array[arrIdx]->format.choice.format3)
	 {
	    DU_ALLOC_SHRABL_BUF(macResrcList->resrcToAddModList[arrIdx].PucchFormat.format3, sizeof(PucchFormat2_3));
	    if(macResrcList->resrcToAddModList[arrIdx].PucchFormat.format3 == NULLP)
	    {
               DU_LOG("\nERROR  --> F1AP : Failed to allocate memory for Format3 in extractResrcToAddModList()");
	       return RFAILED;
	    }
	    macResrcList->resrcToAddModList[arrIdx].PucchFormat.format3->numPrbs =\
                cuResrcList->list.array[arrIdx]->format.choice.format3->nrofPRBs;
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format3->numSymbols =\
                cuResrcList->list.array[arrIdx]->format.choice.format3->nrofSymbols;
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format3->startSymbolIdx =\
                cuResrcList->list.array[arrIdx]->format.choice.format3->startingSymbolIndex;
         }
      }
      else if(cuResrcList->list.array[arrIdx]->format.present == PUCCH_Resource__format_PR_format4)
      {
         macResrcList->resrcToAddModList[arrIdx].pucchFormat = PUCCH_FORMAT_4;
         if(cuResrcList->list.array[arrIdx]->format.choice.format4)
         {
	    DU_ALLOC_SHRABL_BUF(macResrcList->resrcToAddModList[arrIdx].PucchFormat.format4, sizeof(PucchFormat4));
	    if(macResrcList->resrcToAddModList[arrIdx].PucchFormat.format4 == NULLP)
	    {
               DU_LOG("\nERROR  --> F1AP : Failed to allocate memory for Format4 in extractResrcToAddModList()");
	       return RFAILED;
	    }
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format4->numSymbols =\
                cuResrcList->list.array[arrIdx]->format.choice.format4->nrofSymbols;
	    macResrcList->resrcToAddModList[arrIdx].PucchFormat.format4->occLen =\
                cuResrcList->list.array[arrIdx]->format.choice.format4->occ_Length;
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format4->occIdx =\
                cuResrcList->list.array[arrIdx]->format.choice.format4->occ_Index;
            macResrcList->resrcToAddModList[arrIdx].PucchFormat.format4->startSymbolIdx =\
                cuResrcList->list.array[arrIdx]->format.choice.format4->startingSymbolIndex;
	 }
      }
   }
   return ROK;

}/* End of extractResrcToAddModList */

/*******************************************************************
 *
 * @brief Fills fillPucchSchedReqPeriodAndOffset sent by CU
 *
 * @details
 *
 *    Function : fillPucchSchedReqPeriodAndOffset
 *
 *    Functionality: To fillPucchSchedReqPeriodAndOffset
 *
 * @params[in] macPeriodicty,
 *     SchedulingRequestResourceConfig__periodicityAndOffset pointer
 * @return void
 *
 * ****************************************************************/

void fillPucchSchedReqPeriodAndOffset(uint8_t macPeriodicty, uint16_t  macOffset,\
   struct SchedulingRequestResourceConfig__periodicityAndOffset *cuPeriodicty)
{
   macPeriodicty = cuPeriodicty->present;
   switch(macPeriodicty)
   {
      case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sym2:
	 {
	    macOffset     = cuPeriodicty->choice.sym2;
	    break;
	 }
      case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sym6or7:
	 {
	    macOffset     = cuPeriodicty->choice.sym6or7;
	    break;
	 }
      case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl1:
	 {
	    macOffset     = cuPeriodicty->choice.sl1;
	    break;
	 }
      case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl2:
	 {
	    macOffset = cuPeriodicty->choice.sl2;
	    break;
	 }
      case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl4:
	 {
	    macOffset = cuPeriodicty->choice.sl4;
	    break;
	 }
      case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl5:
	 {
	    macOffset = cuPeriodicty->choice.sl5;
	    break;
	 }
      case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl8:
	 {
	    macOffset = cuPeriodicty->choice.sl8;
	    break;
	 }
      case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl10:
	 {
	    macOffset = cuPeriodicty->choice.sl10;
	    break;
	 }
      case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl16:
	 {
	    macOffset = cuPeriodicty->choice.sl16;
	    break;
	 }
      case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl20:
	 {
	    macOffset = cuPeriodicty->choice.sl20;
	    break;
	 }
      case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl40:
	 {
	    macOffset = cuPeriodicty->choice.sl40;
	    break;
	 }
      case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl80:
	 {
	    macOffset = cuPeriodicty->choice.sl80;
	    break;
	 }
      case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl160:
	 {
	    macOffset = cuPeriodicty->choice.sl160;
	    break;
	 }
      case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl320:
	 {
	    macOffset = cuPeriodicty->choice.sl320;
	    break;
	 }
      case SchedulingRequestResourceConfig__periodicityAndOffset_PR_sl640:
	 {
	    macOffset = cuPeriodicty->choice.sl640;
	    break;
	 }
      default :
         DU_LOG("\nERROR  -->  F1AP : Invalid periodicity %d", macPeriodicty);
   }
}

/*******************************************************************
 *
 * @brief Function to extractPucchFormatCfg sent by CU
 *
 * @details
 *
 *    Function : extractPucchFormatCfg
 *
 *    Functionality: Function to extractPucchFormatCfg
 *
 * @params[in] PucchFormatCfg pointer,
 *             PUCCH_FormatConfig_t pointer
 * @return void
 *
 * ****************************************************************/

void extractPucchFormatCfg(PucchFormatCfg *macFormatCfg, PUCCH_FormatConfig_t *cuFormatCfg)
 {
    if(cuFormatCfg->interslotFrequencyHopping)
       macFormatCfg->interSlotFreqHop = *cuFormatCfg->interslotFrequencyHopping;
    if(cuFormatCfg->additionalDMRS)  
       macFormatCfg->addDmrs = *cuFormatCfg->additionalDMRS;
    if(cuFormatCfg->maxCodeRate)
       macFormatCfg->maxCodeRate = *cuFormatCfg->maxCodeRate;
    if(cuFormatCfg->nrofSlots)  
       macFormatCfg->numSlots = *cuFormatCfg->nrofSlots;
    if(cuFormatCfg->pi2BPSK)  
       macFormatCfg->pi2BPSK = *cuFormatCfg->pi2BPSK;
    if(cuFormatCfg->simultaneousHARQ_ACK_CSI)  
       macFormatCfg->harqAckCSI = *cuFormatCfg->simultaneousHARQ_ACK_CSI;
 }/* End of extractPucchFormatCfg */

/*******************************************************************
 *
 * @brief Function to extractSchedReqCfgToAddMod sent by CU
 *
 * @details
 *
 *    Function : extractSchedReqCfgToAddMod
 *
 *    Functionality: Function to extractSchedReqCfgToAddMod
 *
 * @params[in] PucchSchedReqCfg pointer,
 *      PUCCH_Config__schedulingRequestResourceToAddModList pointer
 * @return void
 *
 * ****************************************************************/

void extractSchedReqCfgToAddMod(PucchSchedReqCfg *macSchedReqCfg, struct PUCCH_Config__schedulingRequestResourceToAddModList *cuSchedReqList)
{
   uint8_t arrIdx;

   macSchedReqCfg->schedAddModListCount = cuSchedReqList->list.count;
   for(arrIdx = 0; arrIdx <  macSchedReqCfg->schedAddModListCount; arrIdx++)
   {
      macSchedReqCfg->schedAddModList[arrIdx].resrcId =\
         cuSchedReqList->list.array[arrIdx]->schedulingRequestResourceId;
      macSchedReqCfg->schedAddModList[arrIdx].requestId =\
         cuSchedReqList->list.array[arrIdx]->schedulingRequestID;
      if(cuSchedReqList->list.array[arrIdx]->periodicityAndOffset)
      {
         fillPucchSchedReqPeriodAndOffset(macSchedReqCfg->schedAddModList[arrIdx].periodicity,\
	    macSchedReqCfg->schedAddModList[arrIdx].offset, cuSchedReqList->list.array[arrIdx]->periodicityAndOffset);
      }
      if(cuSchedReqList->list.array[arrIdx]->resource)
      {
         macSchedReqCfg->schedAddModList[arrIdx].resrc =\
    	    *cuSchedReqList->list.array[arrIdx]->resource;
      }
   }

}/* End of extractSchedReqCfgToAddMod */

 /*******************************************************************
 *
 * @brief Fills PucchCfg received by CU
 *
 * @details
 *
 *    Function : extractPucchCfg
 *
 *    Functionality: Fills PucchCfg received  by CU
 *
 * @params[in] BWP_UplinkDedicated__pucch_Config *cuPucchCfg = Information which
 *                is send by CU, which we have stored in F1UeContextSetupDb,
 *             PucchCfg *macPucchCfg = Used to Store the information which
 *                needs to send in other layer, as well as this can be the variable
 *                which stores the information in DuCb,
 *             PucchCfg *storedPucchCfg = Null in case of sending the
 *                information to other layer else it will have Pucch Cfg which
 *                we have stored in copyOfmacUeCfg.
 * @return ROK/RFAILED
 *
 * ****************************************************************/

uint8_t extractPucchCfg(struct BWP_UplinkDedicated__pucch_Config *cuPucchCfg, PucchCfg *macPucchCfg,\
PucchCfg *storedPucchCfg)        
{
   uint8_t arrIdx;

   if(cuPucchCfg->present == BWP_UplinkDedicated__pucch_Config_PR_setup)
   {
      if(cuPucchCfg->choice.setup)
      {
         /* Resource Set Cfg */ 
	 if(cuPucchCfg->choice.setup->resourceSetToAddModList)
	 {
	    DU_ALLOC_SHRABL_BUF(macPucchCfg->resrcSet, sizeof(PucchResrcSetCfg));
	    if(macPucchCfg->resrcSet == NULLP)
	    {
	       DU_LOG("\nERROR --> F1AP : Failed to extract Resrc set List in extractPucchCfg()");
	       return RFAILED;
	    }
	    memset(macPucchCfg->resrcSet, 0, sizeof(PucchResrcSetCfg));
            extractResrcSetToAddModList(macPucchCfg->resrcSet, cuPucchCfg->choice.setup->resourceSetToAddModList);
	 }
         
	 /* Resource Cfg */ 
	 if(cuPucchCfg->choice.setup->resourceToAddModList)
	 {
            DU_ALLOC_SHRABL_BUF(macPucchCfg->resrc, sizeof(PucchResrcCfg));
	    if(macPucchCfg->resrc == NULLP)
	    {
	       DU_LOG("\nERROR --> F1AP : Failed to extract Resrc List in extractPucchCfg()");
	       return RFAILED;
	    }
	    memset(macPucchCfg->resrc, 0, sizeof(PucchResrcCfg));
            extractResrcToAddModList(macPucchCfg->resrc, cuPucchCfg->choice.setup->resourceToAddModList);
	 }
         
	 /* Format 1 Cfg */ 
	 if(cuPucchCfg->choice.setup->format1)
	 {
	    DU_ALLOC_SHRABL_BUF(macPucchCfg->format1, sizeof(PucchFormatCfg));
	    if(macPucchCfg->format1 == NULLP)
	    {
	       DU_LOG("\nERROR --> F1AP : Failed to extract format 1 Cfg in  extractPucchCfg()");
	       return RFAILED;
	    }
	    memset(macPucchCfg->format1, 0, sizeof(PucchFormatCfg));
            extractPucchFormatCfg(macPucchCfg->format1,\
	       cuPucchCfg->choice.setup->format1->choice.setup);
	 }
         
	 /* Format 2 Cfg */
         if(cuPucchCfg->choice.setup->format2)
	 {
	    DU_ALLOC_SHRABL_BUF(macPucchCfg->format2, sizeof(PucchFormatCfg));
	    if(macPucchCfg->format2 == NULLP)
	    {
	       DU_LOG("\nERROR --> F1AP : Failed to extract format 2 Cfg in  extractPucchCfg()");
	       return RFAILED;
	    }
	    memset(macPucchCfg->format2, 0, sizeof(PucchFormatCfg));
            extractPucchFormatCfg(macPucchCfg->format2,\
	       cuPucchCfg->choice.setup->format2->choice.setup);
	 }
         
	 /* Format 3 Cfg */
         if(cuPucchCfg->choice.setup->format3)
	 {
	    DU_ALLOC_SHRABL_BUF(macPucchCfg->format3, sizeof(PucchFormatCfg));
	    if(macPucchCfg->format3 == NULLP)
	    {
	       DU_LOG("\nERROR --> F1AP : Failed to extract format 3 Cfg in  extractPucchCfg()");
	       return RFAILED;
	    }
	    memset(macPucchCfg->format3, 0, sizeof(PucchFormatCfg));
            extractPucchFormatCfg(macPucchCfg->format3,\
	       cuPucchCfg->choice.setup->format3->choice.setup);
	 }

         /* Format 4 Cfg */
         if(cuPucchCfg->choice.setup->format4)
	 {
	    DU_ALLOC_SHRABL_BUF(macPucchCfg->format4, sizeof(PucchFormatCfg));
	    if(macPucchCfg->format4 == NULLP)
	    {
	       DU_LOG("\nERROR --> F1AP : Failed to extract format 4 Cfg in  extractPucchCfg()");
	       return RFAILED;
	    }
	    memset(macPucchCfg->format4, 0, sizeof(PucchFormatCfg));
            extractPucchFormatCfg(macPucchCfg->format4,\
	       cuPucchCfg->choice.setup->format4->choice.setup);
	 }

         /* Sched Req List */
         if(cuPucchCfg->choice.setup->schedulingRequestResourceToAddModList)
	 {
	    DU_ALLOC_SHRABL_BUF(macPucchCfg->schedReq, sizeof(PucchSchedReqCfg));
	    if(macPucchCfg->schedReq == NULLP)
	    {
	       DU_LOG("\nERROR --> F1AP : Failed to extract schedReqCfg in  extractPucchCfg()");
	       return RFAILED;
	    }
	    memset(macPucchCfg->schedReq, 0, sizeof(PucchSchedReqCfg));
            extractSchedReqCfgToAddMod(macPucchCfg->schedReq,\
	    cuPucchCfg->choice.setup->schedulingRequestResourceToAddModList);
	 }

         /*TODO: Add support for  Spatial Info */

	 /* MultiCsiCfg */
         if(cuPucchCfg->choice.setup->multi_CSI_PUCCH_ResourceList)
	 {
	    DU_ALLOC_SHRABL_BUF(macPucchCfg->multiCsiCfg, sizeof(PucchMultiCsiCfg));
	    if(macPucchCfg->multiCsiCfg == NULLP)
	    {
	       DU_LOG("\nERROR --> F1AP : Failed to extract multiCsiCfg in  extractPucchCfg()");
	       return RFAILED;
	    }
	    memset(macPucchCfg->multiCsiCfg, 0, sizeof(PucchMultiCsiCfg));
            macPucchCfg->multiCsiCfg->multiCsiResrcListCount = cuPucchCfg->choice.setup->multi_CSI_PUCCH_ResourceList->list.count;
	    for(arrIdx =0; arrIdx < macPucchCfg->multiCsiCfg->multiCsiResrcListCount; arrIdx++)
	    {
	       macPucchCfg->multiCsiCfg->multiCsiResrcList[arrIdx] =\
	         *cuPucchCfg->choice.setup->multi_CSI_PUCCH_ResourceList->list.array[arrIdx];
	    }
	 }

         /* Dl_DataToUL_ACK */ 
	 if(cuPucchCfg->choice.setup->dl_DataToUL_ACK)
    {
       if(storedPucchCfg)
       {
          if(storedPucchCfg->dlDataToUlAck)
          {
             macPucchCfg->dlDataToUlAck = storedPucchCfg->dlDataToUlAck; 
          }
          else
          {
            DU_ALLOC_SHRABL_BUF(macPucchCfg->dlDataToUlAck, sizeof(PucchDlDataToUlAck));
          }
       }
       else
       {
          DU_ALLOC_SHRABL_BUF(macPucchCfg->dlDataToUlAck, sizeof(PucchDlDataToUlAck));
       }
       if(macPucchCfg->dlDataToUlAck == NULLP)
       {
          DU_LOG("\nERROR --> F1AP : Failed to extract Dl_DataToUL_ACK in extractPucchCfg()");
          return RFAILED;
       }
       memset(macPucchCfg->dlDataToUlAck, 0, sizeof(PucchDlDataToUlAck));
       macPucchCfg->dlDataToUlAck->dlDataToUlAckListCount = cuPucchCfg->choice.setup->dl_DataToUL_ACK->list.count;
       for(arrIdx = 0; arrIdx < macPucchCfg->dlDataToUlAck->dlDataToUlAckListCount; arrIdx++)
       {
          macPucchCfg->dlDataToUlAck->dlDataToUlAckList[arrIdx] =\
          *cuPucchCfg->choice.setup->dl_DataToUL_ACK->list.array[arrIdx];
       }
	 }

	 /* Power Control */
         if(cuPucchCfg->choice.setup->pucch_PowerControl)
	 {
            DU_ALLOC_SHRABL_BUF(macPucchCfg->powerControl, sizeof(PucchPowerControl));
	    if(macPucchCfg->powerControl == NULLP)
	    {
	       DU_LOG("\nERROR --> F1AP : Failed to extract power control in extractPucchCfg()");
	       return RFAILED;
	    }
            extractPucchPowerControl(macPucchCfg->powerControl,\
	       cuPucchCfg->choice.setup->pucch_PowerControl);
	 }
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills ServingCellReconfig received by CU
 *
 * @details
 *
 *    Function : extractSpCellDedicatedCfg
 *
 *    Functionality: Fills ServingCellReconfig received  by CU
 *
 * @params[in] ServingCellConfig_t *cuSrvCellCfg = Information which is send by
 *                  CU, which we have stored in F1UeContextSetupDb,
 *             ServCellCfgInfo *macSrvCellCfg = Used to Store the information
 *                  which  needs to send in other layer, as well as this can be the
 *                  variable which stores the information in DuCb, 
 *             ServCellCfgInfo *storedSrvCellCfg = Null in case of sending the
 *                  information to other layer else it will have ServCellCfgInfo which
 *                  we have stored in copyOfmacUeCfg.
 * @return ROK/RFAILD
 *
 * ****************************************************************/
uint8_t extractSpCellDedicatedCfg(ServingCellConfig_t *cuSrvCellCfg, ServCellRecfgInfo *macSrvCellCfg,\
ServCellRecfgInfo *storedSrvCellCfg)
{
   uint8_t ret = ROK;
   BWP_DownlinkDedicated_t *dlBwp = NULLP;
   BWP_UplinkDedicated_t   *ulBwp = NULLP;

   if(cuSrvCellCfg->initialDownlinkBWP)
   {
      dlBwp = ((BWP_DownlinkDedicated_t *)(cuSrvCellCfg->initialDownlinkBWP));
      if(dlBwp->pdcch_Config)
      {
         if(dlBwp->pdcch_Config->choice.setup)
         {
            macSrvCellCfg->initDlBwp.pdcchPresent = true;
            if(storedSrvCellCfg)
            {
               if(!storedSrvCellCfg->initDlBwp.pdcchPresent)
               {
                  extractPdcchCfg(dlBwp->pdcch_Config->choice.setup, &macSrvCellCfg->initDlBwp.pdcchCfg);
               }
               else
               {
                  extractPdcchCfg(dlBwp->pdcch_Config->choice.setup, &macSrvCellCfg->initDlBwp.pdcchCfg);
               }
            }
            else
            {
               extractPdcchCfg(dlBwp->pdcch_Config->choice.setup, &macSrvCellCfg->initDlBwp.pdcchCfg);
            }
         }
      }
      if(dlBwp->pdsch_Config)
      {
         if(dlBwp->pdsch_Config->choice.setup)
         {
            macSrvCellCfg->initDlBwp.pdschPresent = true;
            
            if(storedSrvCellCfg)
            {
               if(!storedSrvCellCfg->initDlBwp.pdschPresent)
               {
                  extractPdschCfg(dlBwp->pdsch_Config->choice.setup, &macSrvCellCfg->initDlBwp.pdschCfg, NULL);
               }
               else
               {
                  extractPdschCfg(dlBwp->pdsch_Config->choice.setup, &macSrvCellCfg->initDlBwp.pdschCfg,\
                        &storedSrvCellCfg->initDlBwp.pdschCfg);
               }
            }
            else
            {
               extractPdschCfg(dlBwp->pdsch_Config->choice.setup, &macSrvCellCfg->initDlBwp.pdschCfg, NULL);
            }
         }
      }
   }
   if(cuSrvCellCfg->firstActiveDownlinkBWP_Id)
      macSrvCellCfg->firstActvDlBwpId = *(cuSrvCellCfg->firstActiveDownlinkBWP_Id);
   if(cuSrvCellCfg->defaultDownlinkBWP_Id)
      macSrvCellCfg->defaultDlBwpId = *(cuSrvCellCfg->defaultDownlinkBWP_Id);
   if(cuSrvCellCfg->bwp_InactivityTimer)
   {
      if(macSrvCellCfg->bwpInactivityTmr)
      {
         memcpy(macSrvCellCfg->bwpInactivityTmr, cuSrvCellCfg->bwp_InactivityTimer, sizeof(uint8_t));
      }
      else
      {
         macSrvCellCfg->bwpInactivityTmr = NULLP;
         DU_ALLOC_SHRABL_BUF(macSrvCellCfg->bwpInactivityTmr, sizeof(uint8_t));
         if(macSrvCellCfg->bwpInactivityTmr)
         {
            memcpy(macSrvCellCfg->bwpInactivityTmr, cuSrvCellCfg->bwp_InactivityTimer, sizeof(uint8_t));
         }
         else
         {
            DU_LOG("\nERROR  --> F1AP : Memory Alloc failed for bwpInactivityTmr at extractSpCellDedicatedCfg()");
            return RFAILED;
         }
      }
   }
   if(cuSrvCellCfg->pdsch_ServingCellConfig)
   {
      if(cuSrvCellCfg->pdsch_ServingCellConfig->choice.setup)
      {
         ret = extractPdschServingCellCfg(cuSrvCellCfg->pdsch_ServingCellConfig->choice.setup, &macSrvCellCfg->pdschServCellCfg);
         if(ret == RFAILED)
         {
            DU_LOG("\nERROR --> F1AP : Failed at extractPdschServingCellCfg()");
            return RFAILED;
         }
      }
   }
   if(cuSrvCellCfg->uplinkConfig)
   {
      if(cuSrvCellCfg->uplinkConfig->initialUplinkBWP)
      {
         ulBwp = ((BWP_UplinkDedicated_t *)(cuSrvCellCfg->uplinkConfig->initialUplinkBWP));
         if(ulBwp->pusch_Config)
         {
            macSrvCellCfg->initUlBwp.puschPresent = true;
            extractPuschCfg(ulBwp->pusch_Config, &macSrvCellCfg->initUlBwp.puschCfg);
         }
         if(ulBwp->pucch_Config)
         {
            macSrvCellCfg->initUlBwp.pucchPresent = true;
            memset(&macSrvCellCfg->initUlBwp.pucchCfg, 0, sizeof(PucchCfg));
            if(storedSrvCellCfg)
            {
               if(!storedSrvCellCfg->initUlBwp.pucchPresent)
                  extractPucchCfg(ulBwp->pucch_Config, &macSrvCellCfg->initUlBwp.pucchCfg, NULL);
               else
                  extractPucchCfg(ulBwp->pucch_Config, &macSrvCellCfg->initUlBwp.pucchCfg,\
                  &storedSrvCellCfg->initUlBwp.pucchCfg);
            }
            else
            {
               extractPucchCfg(ulBwp->pucch_Config, &macSrvCellCfg->initUlBwp.pucchCfg, NULL);
            }
         }
      }
      if(cuSrvCellCfg->uplinkConfig->firstActiveUplinkBWP_Id)
         macSrvCellCfg->firstActvUlBwpId = *(cuSrvCellCfg->uplinkConfig->firstActiveUplinkBWP_Id);
   }
   return ret;
}

#ifdef NR_DRX

/*******************************************************************
 *
 * @brief fill long cycle offset value of drx in UE structure
 *
 * @details
 *
 *    Function : fillLongCycleOffsetValueInUeDb
 *
 *    Functionality: fill long cycle offset value of drx in UE structure
 *
 * @params[in] DrxLongCycleStartOffset  drxLongCycleStartOffset,
 * struct DRX_ConfigRrc__drx_LongCycleStartOffset drx_LongCycleStartOffset
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void fillLongCycleOffsetValueInUeDb(DrxLongCycleStartOffset *drxLongCycleStartOffset, struct DRX_ConfigRrc__drx_LongCycleStartOffset  *drx_LongCycleStartOffset)
{

   drxLongCycleStartOffset->drxLongCycleStartOffsetChoice = convertLongDrxCycleLengthEnumToValue(drx_LongCycleStartOffset->present);
   switch(drx_LongCycleStartOffset->present)
   {
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms10:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = drx_LongCycleStartOffset->choice.ms10;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms20:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = drx_LongCycleStartOffset->choice.ms20;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms32:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = drx_LongCycleStartOffset->choice.ms32;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms40:
        {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = drx_LongCycleStartOffset->choice.ms40;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms60:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = drx_LongCycleStartOffset->choice.ms60;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms64:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = drx_LongCycleStartOffset->choice.ms64;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms70:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = drx_LongCycleStartOffset->choice.ms70;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms80:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = drx_LongCycleStartOffset->choice.ms80;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms128:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = drx_LongCycleStartOffset->choice.ms128;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms160:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = drx_LongCycleStartOffset->choice.ms160;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms256:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = drx_LongCycleStartOffset->choice.ms256;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms320:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = drx_LongCycleStartOffset->choice.ms320;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms512:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = drx_LongCycleStartOffset->choice.ms512;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms640:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = drx_LongCycleStartOffset->choice.ms640;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms1024:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = drx_LongCycleStartOffset->choice.ms1024;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms1280:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = drx_LongCycleStartOffset->choice.ms1280;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms2048:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = drx_LongCycleStartOffset->choice.ms2048;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms2560:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = drx_LongCycleStartOffset->choice.ms2560;
            break;
         }
       case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms5120:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = drx_LongCycleStartOffset->choice.ms5120;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms10240:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = drx_LongCycleStartOffset->choice.ms10240;
            break;
         }
      default :
         break;
   }
}

/*******************************************************************
 *
 * @brief Fills drx configuration structure with the help of drx_ConfigRrc
 *which recive from CU or storedDrxCfg which is already stored
 *
 * @details
 *
 *    Function : extractDrxConfiguration 
 *
 *    Functionality: Fills Reconfig Cell group Info received by CU
 *   
 * @params[in] cuMacCellGrpDrxConfig  = MAC_CellGroupConfig__drx_ConfigRrc information which
 *                       is send by CU, which we have stored in F1UeContextSetupDb
 *             DrxCfg * drxCfg  = Used to Store the information,
 *                      which needs to send in other layer, as well as this can be
 *                      the variable which stores the information in DuCb,
 *             DrxCfg *storedDrxCfg = Null in case of sending the
 *                      information to other layer else it will have the drx information 
 *                      stored in the copyOfmacUeCfg which we have stored in F1UeContextSetupDb.
 *
 * @return void
 *
 * ****************************************************************/
void extractDrxConfiguration(struct MAC_CellGroupConfig__drx_ConfigRrc *cuMacCellGrpDrxConfig, DrxCfg * drxCfg, DrxCfg *storedDrxCfg)
{
   struct DRX_ConfigRrc *cuDrxConfig;

   if(storedDrxCfg)
   {
      memcpy(drxCfg, storedDrxCfg, sizeof(DrxCfg));
   }
   else
   {
      if(cuMacCellGrpDrxConfig)
      {
         switch(cuMacCellGrpDrxConfig->present)
         {
            case MAC_CellGroupConfig__drx_ConfigRrc_PR_NOTHING:
               break;

            case MAC_CellGroupConfig__drx_ConfigRrc_PR_setup:
               {
                  cuDrxConfig = cuMacCellGrpDrxConfig->choice.setup;
                  if(cuDrxConfig)
                  {
                     switch(cuDrxConfig->drx_onDurationTimer.present)
                     {
                        case DRX_ConfigRrc__drx_onDurationTimer_PR_NOTHING:
                           break;
                        case DRX_ConfigRrc__drx_onDurationTimer_PR_milliSeconds:
                           {
                              drxCfg->drxOnDurationTimer.onDurationTimerValInMs = true;
                              drxCfg->drxOnDurationTimer.onDurationtimerValue.milliSeconds=convertOnDurationTimerMilliSecondsEnumToValue(cuDrxConfig->\
                              drx_onDurationTimer.choice.milliSeconds);
                              break;
                           }
                        case DRX_ConfigRrc__drx_onDurationTimer_PR_subMilliSeconds:
                           {
                              drxCfg->drxOnDurationTimer.onDurationTimerValInMs = false;
                              drxCfg->drxOnDurationTimer.onDurationtimerValue.subMilliSeconds = cuDrxConfig->drx_onDurationTimer.choice.subMilliSeconds;
                              break;
                           }
                     }
                  }
                  fillLongCycleOffsetValueInUeDb(&drxCfg->drxLongCycleStartOffset, &cuDrxConfig->drx_LongCycleStartOffset);
                  drxCfg->drxInactivityTimer = convertDrxInactivityTimerEnumToValue(cuDrxConfig->drx_InactivityTimer);
                  drxCfg->drxHarqRttTimerDl = cuDrxConfig->drx_HARQ_RTT_TimerDL;
                  drxCfg->drxHarqRttTimerUl = cuDrxConfig->drx_HARQ_RTT_TimerUL;
                  drxCfg->drxRetransmissionTimerDl = convertDrxRetransmissionTimerDlEnumToValue(cuDrxConfig->drx_RetransmissionTimerDL);
                  drxCfg->drxRetransmissionTimerUl = convertDrxRetransmissionTimerUlEnumToValue(cuDrxConfig->drx_RetransmissionTimerUL);
                  drxCfg->drxSlotOffset = cuDrxConfig->drx_SlotOffset;
                  if(cuDrxConfig->shortDRX)
                  {
                     drxCfg->shortDrxPres=true;
                     drxCfg->shortDrx.drxShortCycle = convertShortDrxCycleLengthEnumToValue(cuDrxConfig->shortDRX->drx_ShortCycle);
                     drxCfg->shortDrx.drxShortCycleTimer = cuDrxConfig->shortDRX->drx_ShortCycleTimer;
                  }
                  else
                     drxCfg->shortDrxPres=false;

                  break;
               }

            case MAC_CellGroupConfig__drx_ConfigRrc_PR_release:
               break;
         }
      }
   }
}
#endif

/*******************************************************************
 *
 * @brief Fills Reconfig Cell group Info received by CU
 *
 * @details
 *
 *    Function : extractUeRecfgCellInfo
 *
 *    Functionality: Fills Reconfig Cell group Info received by CU
 *   
 * @params[in] CellGroupConfigRrc_t *cellGrp = CellGroupConfigRrc_t information which
 *                       is send by CU, which we have stored in F1UeContextSetupDb
 *             DuMacUeCfg *MacUeCfg = Used to Store the information,
 *                      which needs to send in other layer, as well as this can be
 *                      the variable which stores the information in DuCb,
 *             DuMacUeCfg *storedMacUeCfg = Null in case of sending the
 *                      information to other layer else it will have copyOfmacUeCfg
 *                      which we have stored in F1UeContextSetupDb.
 *
 * @return ROK/RFAILED
 *
 * ****************************************************************/
uint8_t extractUeRecfgCellInfo(CellGroupConfigRrc_t *cellGrp, DuMacUeCfg *macUeCfg, DuMacUeCfg *storedMacUeCfg)
{
   uint8_t ret = ROK;
   MAC_CellGroupConfig_t     *macCellGroup = NULLP;
   PhysicalCellGroupConfig_t *phyCellGrpCfg = NULLP;
   SpCellConfig_t            *spcellCfg = NULLP;
   ServingCellConfig_t       *servCellCfg = NULLP;

   if(cellGrp)
   {
      /* Fill MacCell Group Reconfig  */
      if(cellGrp->mac_CellGroupConfig)
      {
         macUeCfg->macCellGrpCfgPres = true;
         macCellGroup = ((MAC_CellGroupConfig_t *)(cellGrp->mac_CellGroupConfig));

#ifdef NR_DRX         
         if(macCellGroup->drx_ConfigRrc)
         {
            if(storedMacUeCfg)
            {
               if(!storedMacUeCfg->macCellGrpCfgPres) 
               {
                  extractDrxConfiguration(macCellGroup->drx_ConfigRrc, &macUeCfg->macCellGrpCfg.drxCfg,NULL);    
               }
               else
               {
                  extractDrxConfiguration(macCellGroup->drx_ConfigRrc, &macUeCfg->macCellGrpCfg.drxCfg, &storedMacUeCfg->macCellGrpCfg.drxCfg);    
               }
            }
            else
            {
               extractDrxConfiguration(macCellGroup->drx_ConfigRrc, &macUeCfg->macCellGrpCfg.drxCfg,NULL); 
            }
         }
#endif

         if(macCellGroup->schedulingRequestConfig)
         {
            extractSchReqReConfig(macCellGroup->schedulingRequestConfig, &macUeCfg->macCellGrpCfg.schReqCfg);
         }
         if(macCellGroup->tag_Config)
         {
            extractTagReconfig(macCellGroup->tag_Config, &macUeCfg->macCellGrpCfg.tagCfg);
         }
         if(macCellGroup->bsr_Config)
         {
            macUeCfg->macCellGrpCfg.bsrTmrCfg.periodicTimer = macCellGroup->bsr_Config->periodicBSR_Timer;
            macUeCfg->macCellGrpCfg.bsrTmrCfg.retxTimer     = macCellGroup->bsr_Config->retxBSR_Timer;
            if(macCellGroup->bsr_Config->logicalChannelSR_DelayTimer)
            {
               macUeCfg->macCellGrpCfg.bsrTmrCfg.srDelayTimer  =\
               *(macCellGroup->bsr_Config->logicalChannelSR_DelayTimer);
            }
         }
         if(macCellGroup->phr_Config)
         {
            if(macCellGroup->phr_Config->present == MAC_CellGroupConfig__phr_Config_PR_setup)
            {
               macUeCfg->macCellGrpCfg.phrCfgSetupPres = true;
               if(macCellGroup->phr_Config->choice.setup)
               {
                  macUeCfg->macCellGrpCfg.phrCfg.periodicTimer     = \
                  macCellGroup->phr_Config->choice.setup->phr_PeriodicTimer;
                  macUeCfg->macCellGrpCfg.phrCfg.prohibitTimer     = \
                  macCellGroup->phr_Config->choice.setup->phr_ProhibitTimer;
                  macUeCfg->macCellGrpCfg.phrCfg.txPowerFactor     = \
                  macCellGroup->phr_Config->choice.setup->phr_Tx_PowerFactorChange;
                  macUeCfg->macCellGrpCfg.phrCfg.multiplePHR       = \
                  macCellGroup->phr_Config->choice.setup->multiplePHR;
                  macUeCfg->macCellGrpCfg.phrCfg.dummy             = \
                  macCellGroup->phr_Config->choice.setup->dummy;
                  macUeCfg->macCellGrpCfg.phrCfg.phrType2OtherCell = \
                  macCellGroup->phr_Config->choice.setup->phr_Type2OtherCell;
                  macUeCfg->macCellGrpCfg.phrCfg.phrOtherCG        = \
                  macCellGroup->phr_Config->choice.setup->phr_ModeOtherCG;
               }
            }
         }
      }
      /* Fill Physical Cell Group Reconfig */
      if(cellGrp->physicalCellGroupConfig)
      {
         macUeCfg->phyCellGrpCfgPres = true;
         phyCellGrpCfg = ((PhysicalCellGroupConfig_t *)(cellGrp->physicalCellGroupConfig));
         if(phyCellGrpCfg->p_NR_FR1)
         {
            if(*(phyCellGrpCfg->p_NR_FR1) != macUeCfg->phyCellGrpCfg.pNrFr1)
               macUeCfg->phyCellGrpCfg.pNrFr1 = *(phyCellGrpCfg->p_NR_FR1);
         }
         macUeCfg->phyCellGrpCfg.pdschHarqAckCodebook = phyCellGrpCfg->pdsch_HARQ_ACK_Codebook;
      }
      /* Fill SpCell Reconfig */
      if(cellGrp->spCellConfig)
      {
         macUeCfg->spCellCfgPres = true;
         spcellCfg = ((SpCellConfig_t *)(cellGrp->spCellConfig));  
         if(spcellCfg->servCellIndex)
         {
            macUeCfg->spCellCfg.servCellIdx = *(spcellCfg->servCellIndex);
         }
         /* Fill Serving cell Reconfig info */
         if(cellGrp->spCellConfig->spCellConfigDedicated)
         {
            servCellCfg = ((ServingCellConfig_t *)(cellGrp->spCellConfig->spCellConfigDedicated));
            if(storedMacUeCfg)
            {
               if(!storedMacUeCfg->spCellCfgPres)
                  ret = extractSpCellDedicatedCfg(servCellCfg, &macUeCfg->spCellCfg.servCellCfg, NULL);
               else
                  ret = extractSpCellDedicatedCfg(servCellCfg, &macUeCfg->spCellCfg.servCellCfg,\
                        &storedMacUeCfg->spCellCfg.servCellCfg);
            }
            else
            {
               ret = extractSpCellDedicatedCfg(servCellCfg, &macUeCfg->spCellCfg.servCellCfg, NULL);
            }
            if(ret == RFAILED)
            {
               DU_LOG("\nERROR --> F1AP : Failed at extractSpCellDedicatedCfg()");
            }
         }
      }
   }
   return ret;
}
/*******************************************************************
*
* @brief free the memory allocated by decoder
*
* @details
*
*    Function : freeAperDecodeNrcgi 
*
*    Functionality: Free Nrcgi values
*
* @params[in] NRCGI_t *nrcgi
* @return void
*
* ****************************************************************/


void freeAperDecodeNrcgi(NRCGI_t *nrcgi)
{
    if(nrcgi->pLMN_Identity.buf != NULLP)
    {
       free(nrcgi->pLMN_Identity.buf);
    }
    if(nrcgi->nRCellIdentity.buf != NULLP)
    {
       free(nrcgi->nRCellIdentity.buf);
    }
}
/*******************************************************************
*
* @brief free the memory allocated by decoder
*
* @details
*
*    Function : freeAperDecodeCuToDuInfo 
*
*    Functionality:  Free Cu To Du Information
*
* @params[in] CUtoDURRCInformation_t *rrcMsg
* @return void
*
* ****************************************************************/


void freeAperDecodeCuToDuInfo(CUtoDURRCInformation_t *rrcMsg)
{
   uint8_t ieIdx =0;
   uint8_t arrIdx =0;

   if(rrcMsg->uE_CapabilityRAT_ContainerList)
   {
      if(rrcMsg->uE_CapabilityRAT_ContainerList->buf)
         free(rrcMsg->uE_CapabilityRAT_ContainerList->buf);
      free(rrcMsg->uE_CapabilityRAT_ContainerList);
   }

   if(rrcMsg->iE_Extensions)
   {
      if(rrcMsg->iE_Extensions->list.array)
      {
	 for(ieIdx= 0; ieIdx < rrcMsg->iE_Extensions->list.count; ieIdx++)
	 {
	    if(rrcMsg->iE_Extensions->list.array[ieIdx])
	    {
	       switch(rrcMsg->iE_Extensions->list.array[ieIdx]->id)
	       {
		  case ProtocolIE_ID_id_CellGroupConfig:
		     if(rrcMsg->iE_Extensions->list.array[ieIdx]->extensionValue.choice.CellGroupConfig.buf != NULLP)
		     {
			free(rrcMsg->iE_Extensions->list.array[ieIdx]->extensionValue.choice.CellGroupConfig.buf);
		     }
		     break;
		  default:
		     DU_LOG("\nERROR  -->  F1AP : Invalid Event type %ld at FreeCuToDuInfo()", \
			   rrcMsg->iE_Extensions->list.array[ieIdx]->id);
		     break;
	       }
	    }
	 }
	 for(arrIdx = 0; arrIdx < ieIdx; arrIdx++)
	 {
	    free(rrcMsg->iE_Extensions->list.array[arrIdx]);
	 }
	 free(rrcMsg->iE_Extensions->list.array);

      }

      free(rrcMsg->iE_Extensions);
   }
}
/*******************************************************************
*
* @brief free the memory allocated by decoder
*
* @details 
*
*    Function : freeAperDecodeSplCellList
*
*    Functionality: Free Spl Cell List 
                    where memory allocated by aper_decoder
*
* @params[in]  SCell_ToBeSetup_List_t *spCellLst
* @return void
*
* ****************************************************************/


void freeAperDecodeSplCellList(SCell_ToBeSetup_List_t *spCellLst)
{
    uint8_t  cellIdx =0;

    if(spCellLst->list.array != NULLP)
    {
       for(cellIdx=0; cellIdx<spCellLst->list.count; cellIdx++)
       {
          if(cellIdx==0&&spCellLst->list.array[cellIdx]!=NULLP)
          {
             freeAperDecodeNrcgi(&spCellLst->list.array[cellIdx]->value.choice.SCell_ToBeSetup_Item.sCell_ID);
          }
          if(spCellLst->list.array[cellIdx]!=NULLP)
          {
             free(spCellLst->list.array[cellIdx]);
          }
       }
       free(spCellLst->list.array);
    }
}
/*******************************************************************
*
* @brief free the memory allocated by decoder
*
* @details
*
*    Function : freeAperDecodeSRBSetup 
*
*    Functionality: added free part for the memory allocated by aper_decoder
*
* @params[in] SRBs_ToBeSetup_List_t *srbSet
* @return void
*
****************************************************************/


void freeAperDecodeSRBSetup(SRBs_ToBeSetup_List_t *srbSet)
{
    uint8_t srbIdx =0;
    if(srbSet->list.array != NULLP)
    {
       for(srbIdx=0; srbIdx<srbSet->list.count; srbIdx++)
       {
          if(srbSet->list.array[srbIdx]!=NULLP)
          {
             free(srbSet->list.array[srbIdx]);
          }
       }
       free(srbSet->list.array);
    }
}

/*******************************************************************
*
* @brief free the memory allocated by decoder
*
* @details
*
*    Function : freeAperDecodeULTnlInfo
*
*    Functionality: added free part for the memory allocated by aper_decoder
*
* @params[in] ULUPTNLInformation_ToBeSetup_List_t *ulInfo
* @return void
*
* ****************************************************************/


void freeAperDecodeULTnlInfo(ULUPTNLInformation_ToBeSetup_List_t *ulInfo)
{
   uint8_t ulIdx=0;
   if(ulInfo->list.array != NULLP)
   {
      for(ulIdx=0; ulIdx<ulInfo->list.count; ulIdx++)
      {
	 if(ulIdx==0&&ulInfo->list.array[ulIdx]!=NULLP)
	 {
	    if(ulInfo->list.array[ulIdx]->uLUPTNLInformation.choice.gTPTunnel!=NULLP)
	    {
	       if(ulInfo->list.array[ulIdx]->uLUPTNLInformation.choice.gTPTunnel->\
		     transportLayerAddress.buf != NULLP)
	       {
		  if(ulInfo->list.array[ulIdx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf\
			!=NULLP)
		  {
		     free(ulInfo->list.array[ulIdx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf);
		  }
		  free(ulInfo->list.array[ulIdx]->uLUPTNLInformation.choice.gTPTunnel->\
			transportLayerAddress.buf);
	       }
	       free(ulInfo->list.array[ulIdx]->uLUPTNLInformation.choice.gTPTunnel);
	    }
	 }
	 if(ulInfo->list.array[ulIdx]!=NULLP)
	 {
	    free(ulInfo->list.array[ulIdx]);
	 }
      }
      free(ulInfo->list.array);
   }
}
/*******************************************************************
*
* @brief free the memory allocated by decoder
*
* @details
*
*    Function : freeAperDecodeDRBSetup  
*
*    Functionality: free DRBSetup which is allocated by decoder
*
* @params[in]  DRBs_ToBeSetup_List_t *drbSet
* @return void
*
* ****************************************************************/

void freeAperDecodeDRBSetup(DRBs_ToBeSetup_List_t *drbSet)
{
   DRBs_ToBeSetup_Item_t *drbSetItem = NULLP;
   uint8_t  flowIdx =0;
   uint8_t  drbIdx =0;

   if(drbSet->list.array != NULLP)
   {
      for(drbIdx=0; drbIdx<drbSet->list.count; drbIdx++)
      {
         if(drbIdx==0&&drbSet->list.array[drbIdx] != NULLP)
         {
            drbSetItem =&drbSet->list.array[drbIdx]->value.choice.DRBs_ToBeSetup_Item;
            if(drbSetItem->qoSInformation.choice.choice_extension != NULLP)
            {
               if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
                     qoS_Characteristics.choice.non_Dynamic_5QI !=NULLP)
               {
                  if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
                        qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow!=NULLP)
                  {
                     if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
                           qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume!=NULLP)
                     {

                        if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sST.buf!=NULLP)
                        {

                           if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD!=NULLP)
                           {

                              if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD->\
                                    buf!=NULLP)
                              {

                                 if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.\
                                       flows_Mapped_To_DRB_List.list.array != NULLP)
                                 {

                                    for(flowIdx=0;flowIdx<drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
                                          DRB_Information.flows_Mapped_To_DRB_List.list.count; flowIdx++)
                                    {

                                       if(flowIdx==0&&drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
                                             DRB_Information.flows_Mapped_To_DRB_List.list.array[flowIdx]!=NULLP)
                                       {
                                          if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
                                                DRB_Information.flows_Mapped_To_DRB_List.list.array[flowIdx]->\
                                                qoSFlowLevelQoSParameters.\
                                                qoS_Characteristics.choice.non_Dynamic_5QI!=NULLP)
                                          {
                                             if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
                                                   DRB_Information.flows_Mapped_To_DRB_List.list.array[flowIdx]->\
                                                   qoSFlowLevelQoSParameters.\
                                                   qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow!=NULLP)
                                             {

                                                if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
                                                      DRB_Information.flows_Mapped_To_DRB_List.list.array[flowIdx]->\
                                                      qoSFlowLevelQoSParameters.\
                                                      qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume!=NULLP)
                                                {


                                                   free(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
                                                         DRB_Information.flows_Mapped_To_DRB_List.list.array[flowIdx]->\
                                                         qoSFlowLevelQoSParameters.\
                                                         qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume);
                                                }

                                                free(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
                                                      DRB_Information.flows_Mapped_To_DRB_List.list.array[flowIdx]->\
                                                      qoSFlowLevelQoSParameters.\
                                                      qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow);
                                             }

                                             free(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\

                                                   DRB_Information.flows_Mapped_To_DRB_List.list.array[flowIdx]->\
                                                   qoSFlowLevelQoSParameters.\
                                                   qoS_Characteristics.choice.non_Dynamic_5QI);
                                          }
                                       }
                                       if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
                                             DRB_Information.flows_Mapped_To_DRB_List.list.array[flowIdx]!=NULLP)
                                       {

                                          free(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
                                                DRB_Information.flows_Mapped_To_DRB_List.list.array[flowIdx]);
                                       }
                                    }

                                    free(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
                                          DRB_Information.flows_Mapped_To_DRB_List.list.array);
                                 }

                                 free(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
                                       DRB_Information.sNSSAI.sD->buf);
                              }

                              free(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD);
                           }

                           free(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sST.buf);

                        }

                        free(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\

                              qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume);
                     }

                     free(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
                           qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow);
                  }

                  free(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
                        qoS_Characteristics.choice.non_Dynamic_5QI);
               }
               free(drbSetItem->qoSInformation.choice.choice_extension);
            }
            freeAperDecodeULTnlInfo(&drbSetItem->uLUPTNLInformation_ToBeSetup_List);
            if(drbSetItem->uLConfiguration)
            {
               free(drbSetItem->uLConfiguration);
            }
         }
         if(drbSet->list.array[drbIdx]!=NULLP)
         {
            free(drbSet->list.array[drbIdx]);
         }
      }
      free(drbSet->list.array);
   }
}


/*******************************************************************
 *
 * @brief builds Mac Cell Cfg
 *
 * @details
 *
 *    Function : procUeRecfgCellInfo
 *
 *    Functionality: builds Mac Cell Cfg
 *
 * @params[in] DuMacUeCfg *macUeCfgToSend = Used to Store the information which
 *                       needs to send in other layer, as well as this can be
 *                       the variable which stores the information in DuCb.
 *             DuMacUeCfg *storedMacUeCfg = Null in case of sending the
 *                       information to other layer else it will have copyOfmacUeCfg  
 *                       which we have stored in F1UeContextSetupDb
 *             void *cellInfo = CellGroupConfigRrc_t information which is send
 *                        by CU, which we have stored in F1UeContextSetupDb 
 *
 * @return void 
 *
 * ****************************************************************/
uint8_t procUeRecfgCellInfo(DuMacUeCfg *macUeCfgToSend, DuMacUeCfg *storedMacUeCfg, void *cellInfo)
{
   uint8_t ret = ROK;
   CellGroupConfigRrc_t *cellGrp = NULLP;

   if(cellInfo)
   {
      cellGrp = (CellGroupConfigRrc_t *)cellInfo;
      ret = extractUeRecfgCellInfo(cellGrp, macUeCfgToSend, storedMacUeCfg);
      if(ret == RFAILED)
         DU_LOG("\nERROR  -->  F1AP : Failed at procUeRecfgCellInfo()");
   }
   if(ret == RFAILED)
   {
      freeUeRecfgCellGrpInfo(macUeCfgToSend);
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Filling modulation info in mac ue cfg
 *
 * @details
 *
 *    Function : duFillModulationDetails
 *
 *    Functionality: Filling modulation info in mac ue cfg
 *
 * @params[in] MAC UE Config to be updated
 *             Current UE configuration
 *             UE NR capability from CU
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void duFillModulationDetails(DuMacUeCfg *ueCfg, DuMacUeCfg *oldUeCfg, void *ueCap)
{
   UE_NR_Capability_t *ueNrCap=NULLP;

   if(!ueCap && oldUeCfg)
   {
      memcpy(&ueCfg->dlModInfo, &oldUeCfg->dlModInfo, sizeof(ModulationInfo));     
      memcpy(&ueCfg->ulModInfo, &oldUeCfg->ulModInfo, sizeof(ModulationInfo));
   }
   else
   {
      ueNrCap = (UE_NR_Capability_t *)ueCap;

      /* Filling DL modulation info */
      if(ueNrCap->featureSets && ueNrCap->featureSets->featureSetsDownlinkPerCC && \
         ueNrCap->featureSets->featureSetsDownlinkPerCC->list.array[0] && \
         ueNrCap->featureSets->featureSetsDownlinkPerCC->list.array[0]->supportedModulationOrderDL)
      {
         switch(*(ueNrCap->featureSets->featureSetsDownlinkPerCC->list.array[0]->supportedModulationOrderDL))
         {
            case ModulationOrder_qpsk:
               {
                  ueCfg->dlModInfo.modOrder = MOD_ORDER_QPSK;
                  break;
               }
            case ModulationOrder_qam16:
               {
                  ueCfg->dlModInfo.modOrder = MOD_ORDER_QAM16;
                  break;
               }
            case ModulationOrder_qam64:
               {
                  ueCfg->dlModInfo.modOrder = MOD_ORDER_QAM64;
                  ueCfg->dlModInfo.mcsIndex = PDSCH_MCS_INDEX;
                  ueCfg->dlModInfo.mcsTable = MCS_TABLE_QAM64;
                  break;
               }
            case ModulationOrder_qam256:
               {
                  ueCfg->dlModInfo.modOrder = MOD_ORDER_QAM256;
                  break;
               }
            default:
               {
                  DU_LOG("\nERROR  -->  DU APP: Incorrect downlink modulation order received. Reatining old modulation configuration");
                  if(oldUeCfg)
                     memcpy(&ueCfg->dlModInfo, &oldUeCfg->dlModInfo, sizeof(ModulationInfo));
                  break;
               }
         }
      }
      else
      {
         if(oldUeCfg)
            memcpy(&ueCfg->dlModInfo, &oldUeCfg->dlModInfo, sizeof(ModulationInfo));
      }

      /* Filling UL modulation info */
      if(ueNrCap->featureSets && ueNrCap->featureSets->featureSetsUplinkPerCC && \
         ueNrCap->featureSets->featureSetsUplinkPerCC->list.array[0] && \
         ueNrCap->featureSets->featureSetsUplinkPerCC->list.array[0]->supportedModulationOrderUL)
      {
         switch(*(ueNrCap->featureSets->featureSetsUplinkPerCC->list.array[0]->supportedModulationOrderUL))
         {
            case ModulationOrder_qpsk:
               {
                  ueCfg->ulModInfo.modOrder = MOD_ORDER_QPSK;
                  break;
               }
            case ModulationOrder_qam16:
               {
                  ueCfg->ulModInfo.modOrder = MOD_ORDER_QAM16;
                  ueCfg->ulModInfo.mcsIndex = PUSCH_MCS_INDEX;
                  ueCfg->ulModInfo.mcsTable = MCS_TABLE_QAM64;
                  break;
               }
            case ModulationOrder_qam64:
               {
                  ueCfg->ulModInfo.modOrder = MOD_ORDER_QAM64;
                  break;
               }
            case ModulationOrder_qam256:
               {
                  ueCfg->ulModInfo.modOrder = MOD_ORDER_QAM256;
                  break;
               }
            default:
               {
                  DU_LOG("\nERROR  -->  DU APP: Incorrect uplink modulation order received. Reatining old modulation configuration");
                  if(oldUeCfg)
                     memcpy(&ueCfg->ulModInfo, &oldUeCfg->ulModInfo, sizeof(ModulationInfo));
                  break;
               }
         }
      }
      else
      {
         if(oldUeCfg)
            memcpy(&ueCfg->ulModInfo, &oldUeCfg->ulModInfo, sizeof(ModulationInfo));
      }
   }
}

/*******************************************************************
 *
 * @brief Function to extract info from CU to DU RRC container extension
 *
 * @details
 *
 *    Function : extractCuToDuRrcInfoExt
 *
 *    Functionality: Function to extract info from CU to DU RRC container
 *    extension
 *
 * @params[in] ProtocolExtensionContainer_4624P16_t pointer
 *
 * @return ROK
 *         RFAILED
 *
 * ****************************************************************/
uint8_t extractCuToDuRrcInfoExt(ProtocolExtensionContainer_4624P16_t *protocolIeExtn, DuUeCfg *ueCfgDb)
{
   uint8_t ieIdx =0;
   uint16_t recvBufLen =0;
   CellGroupConfigRrc_t *cellGrpCfg = NULLP;
   CUtoDURRCInformation_ExtIEs_t *extIeInfo = NULLP;
   asn_dec_rval_t rval; /* Decoder return value */
   memset(&rval, 0, sizeof(asn_dec_rval_t));

   if(protocolIeExtn)
   {
      for(ieIdx = 0; ieIdx < protocolIeExtn->list.count; ieIdx++)
      {
         extIeInfo = ((CUtoDURRCInformation_ExtIEs_t *)(protocolIeExtn->list.array[ieIdx]));
         switch(extIeInfo->id)
         {
            case ProtocolIE_ID_id_CellGroupConfig:
               {
                  /* decoding the CellGroup Buf received */
                  recvBufLen = extIeInfo->extensionValue.choice.CellGroupConfig.size;
                  DU_ALLOC(cellGrpCfg, sizeof(CellGroupConfigRrc_t));
                  if(cellGrpCfg)
                  {
                     memset(cellGrpCfg, 0, sizeof(CellGroupConfigRrc_t));
                     rval = uper_decode(0, &asn_DEF_CellGroupConfigRrc, (void **)&cellGrpCfg,
                           extIeInfo->extensionValue.choice.CellGroupConfig.buf, recvBufLen, 0, 0);
                     if(rval.code == RC_FAIL || rval.code == RC_WMORE)
                     {
                        DU_LOG("\nERROR  -->  F1AP : ASN decode failed at decodeCellGrpCfg()");
                        return RFAILED;
                     }
                     xer_fprint(stdout, &asn_DEF_CellGroupConfigRrc, cellGrpCfg);

                     if(extractRlcCfgToAddMod(cellGrpCfg->rlc_BearerToAddModList, ueCfgDb))
                        return NULLP;
                     ueCfgDb->cellGrpCfg = cellGrpCfg;
                  }
                  break;
               }

            case ProtocolIE_ID_id_HandoverPreparationInformation:
               {
                  DU_LOG("\nINFO -->  F1AP : Received HANDOVER PREPARATION INFO in UE CONTEXT SETUP REQUEST");
                  break;
               }

            default:
               DU_LOG("\nERROR  -->  F1AP : Invalid IE received CUtoDURRCInformation: %ld at decodeCellGrpCfg()", extIeInfo->id);
               break;
         }
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills Srb List received by CU
 *
 * @details
 *
 *    Function : procSrbListToSetup
 *
 *    Functionality: Fills Srb List received  by CU
 *
 * @params[in] SRBs_ToBeSetup_Item_t *cuSrbItem
 *             DuLcCfg pointer
 *             RlcBearerCfg pointer
 * @return void
 *
 * ****************************************************************/
uint8_t procSrbListToSetup(SRBs_ToBeSetup_Item_t * srbItem, DuLcCfg *duMacLcToAdd, DuRlcBearerCfg *rlcLcToAdd)
{

   /* Filling RLC INFO */
   procRlcLcCfg(srbItem->sRBID, srbItem->sRBID, RB_TYPE_SRB, RLC_AM, CONFIG_ADD, NULL, rlcLcToAdd, NULL);

   /* Filling MAC INFO */
   if(procMacLcCfg(srbItem->sRBID, RB_TYPE_SRB, CONFIG_ADD, NULL,NULL, NULL, NULL, duMacLcToAdd, NULL)  != ROK)
   { 
      DU_LOG("\nERROR  -->  F1AP : Failed at MAC LC Cfg in procSrbListToSetup()");
      return RFAILED;
   }

   return ROK;
}



/*******************************************************************
 *
 * @brief extract Srb List received by CU
 *
 * @details
 *
 *    Function : extractSrbListToSetup
 *
 *    Functionality: extract Srb List received by CU
 *                   for both MAC and RLC
 *
 * @params[in] SRBs_ToBeSetup_Item_t pointer
 *             DuUeCfg pointer
 * @return ROK/RFAIED
 *
 * ****************************************************************/

uint8_t extractSrbListToSetup(SRBs_ToBeSetup_List_t *srbCfg, DuUeCfg *ueCfgDb)
{
   uint8_t ret = ROK, srbIdx = 0, rlcLcIdx = 0;
   SRBs_ToBeSetup_Item_t *srbItem = NULLP;
   DuLcCfg *macLcCtxt = NULLP;
   DuRlcBearerCfg *rlcLcCtxt = NULLP;

   if(srbCfg)
   {
      for(srbIdx = 0; srbIdx < srbCfg->list.count; srbIdx++)
      {
         macLcCtxt = NULL;
         rlcLcCtxt = NULL;

         srbItem = &srbCfg->list.array[srbIdx]->value.choice.SRBs_ToBeSetup_Item;
         if(ueCfgDb->numMacLcs > MAX_NUM_LC)
         { 
            DU_LOG("\nERROR   -->  F1AP:  MAX LC Reached in MAC");
            ret = RFAILED;
            break;
         }
         if(ueCfgDb->numRlcLcs > MAX_NUM_LC)
         {
            DU_LOG("\nERROR   -->  F1AP:  MAX LC Reached in RLC");
            ret = RFAILED;
            break;
         }

         for(rlcLcIdx = 0; rlcLcIdx < ueCfgDb->numMacLcs; rlcLcIdx++)
         {
            if(ueCfgDb->rlcLcCfg[rlcLcIdx].rlcBearerCfg.rbId == srbItem->sRBID && ueCfgDb->rlcLcCfg[rlcLcIdx].rlcBearerCfg.rbType == RB_TYPE_SRB)
            {
               macLcCtxt = &ueCfgDb->macLcCfg[rlcLcIdx];
               rlcLcCtxt = &ueCfgDb->rlcLcCfg[rlcLcIdx];
               break;
            }
         }
         if(!macLcCtxt)
         {
            memset(&ueCfgDb->macLcCfg[ueCfgDb->numMacLcs], 0, sizeof(DuLcCfg));
            macLcCtxt = &ueCfgDb->macLcCfg[ueCfgDb->numMacLcs];
            ueCfgDb->numMacLcs++;
         }
         if(!rlcLcCtxt)
         {
            memset(&ueCfgDb->rlcLcCfg[ueCfgDb->numRlcLcs], 0, sizeof(DuRlcBearerCfg));
            rlcLcCtxt = &ueCfgDb->rlcLcCfg[ueCfgDb->numRlcLcs];
            ueCfgDb->numRlcLcs++;
         }

         ret = procSrbListToSetup(srbItem, macLcCtxt, rlcLcCtxt);

         DU_LOG("\nDEBUG --> DUAPP: extractSrbListToSetup: SRBID: %ld [RLC,MAC,NumDrb]:[%x,%x,%x]",\
               srbItem->sRBID, ueCfgDb->numRlcLcs, ueCfgDb->numMacLcs,  ueCfgDb->numDrb);
         if(ret == RFAILED)
         {
            DU_LOG("\nERROR  -->  F1AP:  Failed at extractSrbListToSetup()");
            break;
         }
      }
   }
   else
      ret = RFAILED;

   return ret;
}

/*******************************************************************
 *
 * @brief Fills Drb List received by CU
 *
 * @details
 *
 *    Function : procDrbListToSetupMod
 *
 *    Functionality: Fills Drb List received by CU
 *                   for both MAC and RLC
 *
 * @params[in] DRBs_ToBeSetup_Item_t , DRBs_ToBeSetupMod_Item_t,
 *             DRBs_ToBeModified_Item_t , lcId, DuLcCfg pointer,
 *             RlcBearerCfg , UpTnlCfg, DuRlcUeCfg
 * @return void
 *
 * ****************************************************************/

uint8_t procDrbListToSetupMod(uint8_t lcId, DRBs_ToBeSetup_Item_t *drbItem,\
DRBs_ToBeSetupMod_Item_t *drbSetupModItem, DRBs_ToBeModified_Item_t *drbModItem, DuLcCfg *macLcToAdd, DuRlcBearerCfg *rlcLcToAdd, UpTnlCfg *upTnlInfo, DuRlcUeCfg *storedRlcUeCfg)
{
   uint8_t cfgIdx = 0;
   RlcMode rlcModeInfo;

   if(drbItem != NULLP)
   {
      /* Filling RLC INFO */
      procRlcLcCfg(drbItem->dRBID, lcId, RB_TYPE_DRB, drbItem->rLCMode, CONFIG_ADD, NULL, rlcLcToAdd, &drbItem->\
      qoSInformation);

      /* Filling MAC INFO */
      if(procMacLcCfg(lcId, RB_TYPE_DRB, CONFIG_ADD, drbItem, NULL, NULL, NULL, macLcToAdd, upTnlInfo) != ROK)
      { 
         DU_LOG("\nERROR  --> F1AP : Failed at RLC LC Cfg in procDrbListToSetupMod()");
         return RFAILED;
      }
   }
   else if(drbSetupModItem != NULLP)
   {
      procRlcLcCfg(drbSetupModItem->dRBID, lcId, RB_TYPE_DRB, drbSetupModItem->rLCMode, CONFIG_ADD, NULL, rlcLcToAdd, 
      &drbSetupModItem->qoSInformation);

      if(procMacLcCfg(lcId, RB_TYPE_DRB, CONFIG_ADD, NULL, drbSetupModItem, NULL, NULL, macLcToAdd, upTnlInfo) != ROK)
      {
         DU_LOG("\nERROR  --> F1AP : Failed at RLC LC Cfg in procDrbListToSetupMod()");
         return RFAILED;
      }
   }
   else if(drbModItem != NULLP)
   {
      /* Drb to Mod IEs doesnot have rlcMode to be modified
       * in ASN. Hence no change in RLC configurations */
      if(storedRlcUeCfg != NULLP)
      {
         for(cfgIdx = 0; cfgIdx < storedRlcUeCfg->numLcs; cfgIdx++)
         {
            if(storedRlcUeCfg->rlcLcCfg[cfgIdx].rlcBearerCfg.lcId == lcId)
            {
               rlcModeInfo = storedRlcUeCfg->rlcLcCfg[cfgIdx].rlcBearerCfg.rlcMode;
               break;
            }
         }
      }

      procRlcLcCfg(drbModItem->dRBID, lcId, RB_TYPE_DRB, rlcModeInfo, CONFIG_MOD, NULL, rlcLcToAdd, drbModItem->qoSInformation);
      if(procMacLcCfg(lcId, RB_TYPE_DRB, CONFIG_MOD, NULL, NULL, drbModItem, NULL, macLcToAdd, upTnlInfo) != ROK)
      {
         DU_LOG("\nERROR  --> F1AP : Failed at RLC LC Cfg in procDrbListToSetupMod()");
         return RFAILED;
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief extract Drb List received by CU
 *
 * @details
 *
 *    Function : extractDrbListToSetupMod
 *
 *    Functionality: extract Drb List received by CU
 *                   for both MAC and RLC
 *
 * @params[in] DRBs_ToBeSetup_Item_t pointer
 *             DuUeCfg pointer
 * @return ROK/RFAIED
 *
 * ****************************************************************/

uint8_t extractDrbListToSetupMod(DRBs_ToBeSetup_List_t *drbCfg, DRBs_ToBeSetupMod_List_t *drbSetupModCfg,\
 DRBs_ToBeModified_List_t *drbModCfg, uint8_t drbCount, DuUeCfg *ueCfgDb, uint32_t *drbBitMap, DuRlcUeCfg *rlcUeCfg)
{
   uint8_t ret = ROK;
   uint8_t drbIdx = 0, rlcLcIdx = 0;
   uint8_t drbId = 0, lcId = 0;
   DRBs_ToBeSetup_Item_t *drbItem = NULLP;
   DRBs_ToBeSetupMod_ItemIEs_t *drbSetupModItem = NULLP;
   DRBs_ToBeModified_ItemIEs_t *drbModItem = NULLP;
   DuLcCfg *macLcCtxt = NULLP;
   DuRlcBearerCfg *rlcLcCtxt = NULLP;

   ret = ROK;
   if(drbCount > 0)
   {
      for(drbIdx = 0; drbIdx < drbCount; drbIdx++)
      {
         macLcCtxt = NULL;
         rlcLcCtxt = NULL;

         if(ueCfgDb->numMacLcs > MAX_NUM_LC)
         { 
            DU_LOG("\nERROR  -->  F1AP :  MAX LC Reached in MAC at extractDrbListToSetupMod()");
            ret = RFAILED;
            break;
         }
         if(ueCfgDb->numRlcLcs > MAX_NUM_LC)
         {
            DU_LOG("\nERROR  -->  F1AP :  MAX LC Reached in RLC at extractDrbListToSetupMod()");
            ret = RFAILED;
            break;
         }

         if(drbModCfg != NULLP)
         {
            drbModItem = (DRBs_ToBeModified_ItemIEs_t *) drbModCfg->list.array[drbIdx];
            drbId = drbModItem->value.choice.DRBs_ToBeModified_Item.dRBID;
         }
         else if(drbCfg != NULLP)
            drbId = drbCfg->list.array[drbIdx]->value.choice.DRBs_ToBeSetup_Item.dRBID;
         else if(drbSetupModCfg != NULL)
         {
            drbSetupModItem = (DRBs_ToBeSetupMod_ItemIEs_t *) drbSetupModCfg->list.array[drbIdx];
            drbId = drbSetupModItem->value.choice.DRBs_ToBeSetupMod_Item.dRBID;
         }

         for(rlcLcIdx = 0; rlcLcIdx < ueCfgDb->numRlcLcs; rlcLcIdx++)
         {
            if(ueCfgDb->rlcLcCfg[rlcLcIdx].rlcBearerCfg.rbId == drbId && \
                   ueCfgDb->rlcLcCfg[rlcLcIdx].rlcBearerCfg.rbType == RB_TYPE_DRB)
            {
               macLcCtxt = &ueCfgDb->macLcCfg[rlcLcIdx];
               rlcLcCtxt = &ueCfgDb->rlcLcCfg[rlcLcIdx];
               break;
            }
         }
         if(!macLcCtxt)
         {
            memset(&ueCfgDb->macLcCfg[ueCfgDb->numMacLcs], 0, sizeof(DuLcCfg));
            macLcCtxt = &ueCfgDb->macLcCfg[ueCfgDb->numMacLcs];
            ueCfgDb->numMacLcs++;
         }
         if(!rlcLcCtxt)
         {
            memset(&ueCfgDb->rlcLcCfg[ueCfgDb->numRlcLcs], 0, sizeof(RlcBearerCfg));
            rlcLcCtxt = &ueCfgDb->rlcLcCfg[ueCfgDb->numRlcLcs];
            ueCfgDb->numRlcLcs++;
         }

         if(drbModCfg != NULLP)
         {
            lcId = fetchLcId(drbId);
            if(lcId < MIN_DRB_LCID)
            {
               DU_LOG("\nERROR  --> F1AP : Failed fetching LCID %d in extractDrbListToSetupMod() for Modified List", lcId);
               break;
            } 
            ret = procDrbListToSetupMod(lcId, NULL, NULL, &(drbModItem->value.choice.DRBs_ToBeModified_Item),\
            macLcCtxt, rlcLcCtxt, &ueCfgDb->upTnlInfo[ueCfgDb->numDrb], rlcUeCfg);
            if(ret == RFAILED)
            {
               DU_LOG("\nERROR  --> F1AP : Failed at extractDrbListToSetupMod() for Modified List");
               break;
            }
            ueCfgDb->numDrbModified++;
         }
         else
         {
            lcId = getDrbLcId(drbBitMap);
            if(lcId == RFAILED)
            {
               DU_LOG("\nERROR  -->  F1AP :  InCorrect LCID extractDrbListToSetupMod()");
               ret = RFAILED;
               break;
            }
            if(drbCfg != NULL)
            {
               drbItem = &drbCfg->list.array[drbIdx]->value.choice.DRBs_ToBeSetup_Item;
               ret = procDrbListToSetupMod(lcId, drbItem, NULL, NULL, macLcCtxt, rlcLcCtxt, &ueCfgDb->upTnlInfo[ueCfgDb->numDrb], rlcUeCfg);
               if(ret == RFAILED)
               {
                  DU_LOG("\nERROR  --> F1AP : Failed at extractDrbListToSetupMod() for DrbSetup List");
                  break;
               }
            }
            else if(drbSetupModCfg != NULL)
            {
               ret = procDrbListToSetupMod(lcId, NULL, &(drbSetupModItem->value.choice.DRBs_ToBeSetupMod_Item), NULL,\
                     macLcCtxt, rlcLcCtxt, &ueCfgDb->upTnlInfo[ueCfgDb->numDrb], rlcUeCfg);
               if(ret == RFAILED)
               {
                  DU_LOG("\nERROR  --> F1AP : Failed at extractDrbListToSetupMod() for DrbSetupMod List");
                  break;
               }
               ueCfgDb->numDrbSetupMod++;
            }
         }
         ueCfgDb->numDrb++;
 
         DU_LOG("\nDEBUG --> DUAPP: extractDrbListToSetupMod:lcId:%x ,BitMap:%x, [RLC,MAC,NumDrb]:[%x,%x,%x]",\
                            lcId,*drbBitMap, ueCfgDb->numRlcLcs, ueCfgDb->numMacLcs,  ueCfgDb->numDrb);
         if(ret == RFAILED)
         {
            DU_LOG("\nERROR  --> F1AP : Failed at extractDrbListToSetupMod()");
            break;
         }
      }
   }
   else
      ret = RFAILED;

   return ret;
}

/*******************************************************************
 *
 * @brief extract Drb List received from CU
 *
 * @details
 *
 *    Function : extractDrbListToRelease
 *
 *    Functionality: extract Drb List received from CU
 *                   for both MAC and RLC
 *
 * @params[in] DRBs_ToBeReleased_Item_t pointer
 *             DuUeCfg pointer
 * @return ROK/RFAIED
 *
 * ****************************************************************/

uint8_t extractDrbListToRelease(uint8_t ueId, DRBs_ToBeReleased_List_t *drbToRel, uint8_t drbCount, DuUeCfg *ueCfgDb, DuRlcUeCfg *rlcUeCfg)
{
   uint8_t ret = ROK, teIdx = 0;
   uint8_t drbIdx = 0, rlcLcIdx = 0;
   uint8_t drbId = 0, lcId = 0;
   DRBs_ToBeReleased_ItemIEs_t *drbRelItem = NULLP;
   DuLcCfg *macLcCtxt = NULLP;
   DuRlcBearerCfg *rlcLcCtxt = NULLP;

   ret = ROK;
   if(drbCount > 0)
   {
      for(drbIdx = 0; drbIdx < drbCount; drbIdx++)
      {
         macLcCtxt = NULL;
         rlcLcCtxt = NULL;

         if(drbToRel != NULLP)
         {
            drbRelItem = (DRBs_ToBeReleased_ItemIEs_t *) drbToRel->list.array[drbIdx];
            drbId = drbRelItem->value.choice.DRBs_ToBeReleased_Item.dRBID;
         }
         else
         {
            DU_LOG("ERROR  --> DU APP : DrbToRelease pointer is NULL in extractDrbListToRelease");
            return RFAILED;
         }

         for(rlcLcIdx = 0; rlcLcIdx < ueCfgDb->numRlcLcs; rlcLcIdx++)
         {
            if(ueCfgDb->rlcLcCfg[rlcLcIdx].rlcBearerCfg.rbId == drbId && ueCfgDb->rlcLcCfg[rlcLcIdx].rlcBearerCfg.rbType == RB_TYPE_DRB)
            {
               macLcCtxt = &ueCfgDb->macLcCfg[rlcLcIdx];
               rlcLcCtxt = &ueCfgDb->rlcLcCfg[rlcLcIdx];
               break;
            }
         }

         if(!macLcCtxt)
         {
            memset(&ueCfgDb->macLcCfg[ueCfgDb->numMacLcs], 0, sizeof(DuLcCfg));
            macLcCtxt = &ueCfgDb->macLcCfg[ueCfgDb->numMacLcs];
            ueCfgDb->numMacLcs++;
         }
         if(!rlcLcCtxt)
         {
            memset(&ueCfgDb->rlcLcCfg[ueCfgDb->numRlcLcs], 0, sizeof(DuRlcBearerCfg));
            rlcLcCtxt = &ueCfgDb->rlcLcCfg[ueCfgDb->numRlcLcs];
            ueCfgDb->numRlcLcs++;
         }
         lcId = fetchLcId(drbId);
         if(lcId < MIN_DRB_LCID)
         {
            DU_LOG("\nERROR  --> F1AP : Failed fetching LCID %d in extractDrbListToRelease() for Modified List", lcId);
            break;
         } 

         /* Filling RLC INFO */
         procRlcLcCfg(drbId, lcId, RB_TYPE_DRB, rlcLcCtxt->rlcBearerCfg.rlcMode, CONFIG_DEL, NULL, rlcLcCtxt, NULLP);
         /* Filling MAC INFO */
         if(procMacLcCfg(lcId, RB_TYPE_DRB, CONFIG_DEL, NULL, NULL, NULL, NULL, macLcCtxt, NULLP) != ROK)
         { 
            DU_LOG("\nERROR  --> F1AP : Failed at RLC LC Cfg in extractDrbListToRelease()");
            return RFAILED;
         }
         ueCfgDb->upTnlInfo[ueCfgDb->numDrb].configType = CONFIG_DEL;
         ueCfgDb->upTnlInfo[ueCfgDb->numDrb].drbId = drbId;
         ueCfgDb->upTnlInfo[ueCfgDb->numDrb].ueId = ueId;

         for(teIdx = 0; teIdx < duCb.numTeId; teIdx++)
         {
            if((duCb.upTnlCfg[teIdx]->ueId == ueCfgDb->upTnlInfo[ueCfgDb->numDrb].ueId) && \
                  (duCb.upTnlCfg[teIdx]->drbId == ueCfgDb->upTnlInfo[ueCfgDb->numDrb].drbId))
            {
               DU_ALLOC(ueCfgDb->upTnlInfo[ueCfgDb->numDrb].tnlCfg1, sizeof(GtpTnlCfg)); 
               memcpy(ueCfgDb->upTnlInfo[ueCfgDb->numDrb].tnlCfg1, duCb.upTnlCfg[teIdx]->tnlCfg1, sizeof(GtpTnlCfg));
               break;
            }
         }

         ueCfgDb->numDrb++;
         if(ret == RFAILED)
         {
            DU_LOG("\nERROR  --> F1AP : Failed at extractDrbListToRelease() for Modified List");
            break;
         }

         DU_LOG("\nDEBUG --> DUAPP: extractDrbListToRelease():lcId:%x, [RLC,MAC,NumDrb]:[%x,%x,%x]",\
               lcId,ueCfgDb->numRlcLcs, ueCfgDb->numMacLcs,  ueCfgDb->numDrb);
      }
   }
   else
      ret = RFAILED;

   return ret;
}

/*******************************************************************
 *
 * @brief Function to extract Dl RRC Msg received from CU
 *
 * @details
 *
 *    Function : extractDlRrcMsg
 *
 *    Functionality: Function to extract Dl RRC Msg received from CU
 *
 * @params[in] F1AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t extractDlRrcMsg(uint32_t gnbDuUeF1apId, uint32_t gnbCuUeF1apId, \
   F1DlRrcMsg *dlRrcMsg, RRCContainer_t *rrcContainer)
{
   uint8_t ret = ROK;
   dlRrcMsg->rrcMsgSize = rrcContainer->size;
   if(dlRrcMsg->rrcMsgSize > 0)
   {
      DU_ALLOC_SHRABL_BUF(dlRrcMsg->rrcMsgPdu, dlRrcMsg->rrcMsgSize);
      if(!dlRrcMsg->rrcMsgPdu)
      {
         DU_LOG("\nERROR  --> DU APP : Memory allocation failed for RRC Msg in extractDlRrcMsg()");
         ret = RFAILED;
      }
      else
      {
         dlRrcMsg->gnbDuUeF1apId = gnbDuUeF1apId;
         dlRrcMsg->gnbCuUeF1apId = gnbCuUeF1apId;
         dlRrcMsg->srbId = SRB1_LCID;
         memcpy(dlRrcMsg->rrcMsgPdu, rrcContainer->buf, dlRrcMsg->rrcMsgSize);
      }
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Extract UE capability info 
 *
 * @details
 *
 *    Function : extractUeCapability
 *
 *    Functionality: Extract UE capability info and stores in ue Cb
 *
 * @params[in] Octet string of UE capability RAT container list
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
UE_NR_Capability_t *extractUeCapability(UE_CapabilityRAT_ContainerList_t *ueCapablityListBuf, DuUeCb *ueCb)
{
   uint8_t  idx;
   uint16_t recvBufLen;
   asn_dec_rval_t rval;
   UE_NR_Capability_t  *ueNrCap = NULLP;
   UE_CapabilityRAT_ContainerListRRC_t  *ueCapRatContList = NULLP;

   /* Decoding UE Capability RAT Container List */
   recvBufLen = ueCapablityListBuf->size;
   DU_ALLOC(ueCapRatContList, sizeof(UE_CapabilityRAT_ContainerListRRC_t));
   if(!ueCapRatContList)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in extractUeCapability");
      return NULLP;
   }
   memset(ueCapRatContList, 0, sizeof(UE_CapabilityRAT_ContainerListRRC_t));
   memset(&rval, 0, sizeof(asn_dec_rval_t));
   rval = uper_decode(0, &asn_DEF_UE_CapabilityRAT_ContainerListRRC, (void **)&ueCapRatContList,
          ueCapablityListBuf->buf, recvBufLen, 0, 0);
   if(rval.code == RC_FAIL || rval.code == RC_WMORE)
   {
      DU_LOG("\nERROR  -->  F1AP : ASN decode failed at decodeCellGrpCfg()");
      return NULLP;
   }
   xer_fprint(stdout, &asn_DEF_UE_CapabilityRAT_ContainerListRRC, ueCapRatContList);

   /* Free encoded buffer after decoding */

   for(idx = 0; idx < ueCapRatContList->list.count; idx++)
   {
      if(ueCapRatContList->list.array[idx]->rat_Type == RAT_Type_nr)
      {
         /* Decoding UE NR Capability */
          recvBufLen = ueCapRatContList->list.array[idx]->ue_CapabilityRAT_Container.size;
          DU_ALLOC(ueNrCap, sizeof(UE_NR_Capability_t));
          if(!ueNrCap)
          {
             DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in extractUeCapability");
             DU_FREE(ueCapRatContList, sizeof(UE_CapabilityRAT_ContainerListRRC_t));
             return NULLP;
          } 
          memset(ueNrCap, 0, sizeof(UE_NR_Capability_t));
          memset(&rval, 0, sizeof(asn_dec_rval_t));
          rval = uper_decode(0, &asn_DEF_UE_NR_Capability, (void **)&ueNrCap,
                 ueCapRatContList->list.array[idx]->ue_CapabilityRAT_Container.buf, recvBufLen, 0, 0);
          if(rval.code == RC_FAIL || rval.code == RC_WMORE)
          {
             DU_LOG("\nERROR  -->  F1AP : ASN decode failed at decodeCellGrpCfg()");
             return NULLP;
          }
          xer_fprint(stdout, &asn_DEF_UE_NR_Capability, ueNrCap);
          
          /* Free encoded buffer after decoding */
          free(ueCapRatContList->list.array[idx]->ue_CapabilityRAT_Container.buf);
      }
      free(ueCapRatContList->list.array[idx]);
   }

   /* Free Memory*/
   free(ueCapRatContList->list.array);
   DU_FREE(ueCapRatContList, sizeof(UE_CapabilityRAT_ContainerListRRC_t));
   return ueNrCap;
}
 
/*******************************************************************
*
* @brief free UE context setup request from CU
*
* @details
*
*    Function : freeAperDecodeF1UeContextSetupReq
*
*    Functionality: freeing part for the memory allocated by aper_decoder
*
* @params[in] F1AP message
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
void freeAperDecodeF1UeContextSetupReq(UEContextSetupRequest_t   *ueSetReq)
{
   uint8_t ieIdx = 0;

   if(ueSetReq->protocolIEs.list.array != NULLP)
   {
      for(ieIdx = 0; ieIdx < ueSetReq->protocolIEs.list.count; ieIdx++)
      {
         if(ueSetReq->protocolIEs.list.array[ieIdx])
         {
            switch(ueSetReq->protocolIEs.list.array[ieIdx]->id)
            {
               case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
                  break;
               case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
                  break;
               case ProtocolIE_ID_id_SpCell_ID:
                  freeAperDecodeNrcgi(&ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.NRCGI);
                  break;
               case ProtocolIE_ID_id_ServCellIndex:
                  break;
               case ProtocolIE_ID_id_SpCellULConfigured:
                  break;
               case ProtocolIE_ID_id_CUtoDURRCInformation:

                  freeAperDecodeCuToDuInfo(&ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.CUtoDURRCInformation);
                  break;
               case ProtocolIE_ID_id_SCell_ToBeSetup_List:

                  freeAperDecodeSplCellList(&ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.SCell_ToBeSetup_List);
                  break;
               case ProtocolIE_ID_id_SRBs_ToBeSetup_List:

                  freeAperDecodeSRBSetup(&ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.SRBs_ToBeSetup_List);
                  break;
               case ProtocolIE_ID_id_DRBs_ToBeSetup_List:

                  freeAperDecodeDRBSetup(&ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.DRBs_ToBeSetup_List);
                  break;
               case ProtocolIE_ID_id_RRCContainer:
                  {

                     if(ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.buf != NULLP)
                     {

                        free(ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.buf);
                     }
                     break;
                  }
               case ProtocolIE_ID_id_RRCDeliveryStatusRequest:
                  break;
               case ProtocolIE_ID_id_GNB_DU_UE_AMBR_UL:
                  {
                     if(ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.BitRate.buf)
                     {
                        free(ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.BitRate.buf);
                     }
                     break;
                  }
#ifdef NR_DRX
               case ProtocolIE_ID_id_DRXCycle:
                  {
                     if(ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.DRXCycle.shortDRXCycleLength)
                        free(ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.DRXCycle.shortDRXCycleLength);
                     if(ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.DRXCycle.shortDRXCycleTimer)
                        free(ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.DRXCycle.shortDRXCycleTimer);
                     break;
                  }
#endif             
                default:
                  DU_LOG("\nERROR  -->  F1AP: Invalid event type %ld " ,ueSetReq->protocolIEs.list.array[ieIdx]->id);
            } 
            free(ueSetReq->protocolIEs.list.array[ieIdx]);
         }
      }
      free(ueSetReq->protocolIEs.list.array);
   }
}
/*******************************************************************
 *
 * @brief Process UE context setup request from CU
 *
 * @details
 *
 *    Function : procF1UeContextSetupReq
 *
 *    Functionality: Process UE context setup request from CU
 *
 * @params[in] F1AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procF1UeContextSetupReq(F1AP_PDU_t *f1apMsg)
{
   int8_t ueIdx = -1;
   uint8_t  ret=0, ieIdx=0, ieExtIdx = 0;
   //uint8_t servCellIdx = 0;
   bool ueCbFound = false, hoInProgress = false;
   uint16_t cellIdx=0;
   uint64_t nrCellId = 0;
   uint32_t gnbCuUeF1apId=0, gnbDuUeF1apId=0, bitRateSize=0;
   DuUeCb   *duUeCb = NULL;
   UEContextSetupRequest_t   *ueSetReq = NULL;
   DRBs_ToBeSetup_List_t *drbCfg = NULL;
   CUtoDURRCInformation_t *rrcInfo = NULL;
#ifdef NR_DRX
   DRXCycle_t *drxCycle;
#endif
   ret = ROK;

   ueSetReq = &f1apMsg->choice.initiatingMessage->value.choice.UEContextSetupRequest;
   for(ieIdx=0; (ieIdx < ueSetReq->protocolIEs.list.count && ret == ROK); ieIdx++)
   {
      switch(ueSetReq->protocolIEs.list.array[ieIdx]->id)
      {
         case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
            {
               gnbCuUeF1apId = ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_CU_UE_F1AP_ID;
               break;
            }

         case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
            {
               gnbDuUeF1apId = ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_UE_F1AP_ID;
               break;
            }

         case ProtocolIE_ID_id_SpCell_ID:
            {
               bitStringToInt(&ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.NRCGI.nRCellIdentity, &nrCellId);

               GET_CELL_IDX(nrCellId, cellIdx);
               if(!duCb.actvCellLst[cellIdx])
               {
                  DU_LOG("\nERROR  -->  F1AP : Cell Id [%lu] not found", nrCellId);
                  ret = RFAILED;
               }
               break;
            }

         case ProtocolIE_ID_id_ServCellIndex:
            {
               //servCellIdx = ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.ServCellIndex;
               break;
            }

         case ProtocolIE_ID_id_SpCellULConfigured:
            {
               /* Indicates whether the gNB-CU requests the gNB-DU to configure the uplink as no UL, 
                  UL, SUL or UL+SUL for the indicated cell for the UE */
               break;
            }

         case ProtocolIE_ID_id_CUtoDURRCInformation:
            {
               rrcInfo = &ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.CUtoDURRCInformation;

               /* Search if UE context is present */
               for(ueIdx = 0; ueIdx < MAX_NUM_UE; ueIdx++)
               {
                  if(duCb.actvCellLst[cellIdx]->ueCb[ueIdx].gnbCuUeF1apId == gnbCuUeF1apId)
                  {
                     ueCbFound = true;
                     duUeCb = &duCb.actvCellLst[cellIdx]->ueCb[ueIdx];
                     break;
                  }
               }

               /* Check if UE Handover scenario */
               if(rrcInfo->iE_Extensions)
               {
                  for(ieExtIdx = 0; ieExtIdx < rrcInfo->iE_Extensions->list.count; ieExtIdx++)
                  {
                     if(rrcInfo->iE_Extensions->list.array[ieExtIdx]->id == ProtocolIE_ID_id_HandoverPreparationInformation)
                     {
                        hoInProgress = true;
                        break;
                     }
                  }
               }
               
               /* If UE context is not present, but UE is in handover */
               if(!ueCbFound && hoInProgress)
               {
                  ueIdx = getFreeBitFromUeBitMap(nrCellId);
                  if(ueIdx != -1)
                     gnbDuUeF1apId = ueIdx +1;
                  else
                  {
                     DU_LOG("\nERROR  -->  F1AP : No free UE IDX found in UE bit map of cell Id [%lu]", nrCellId);
                     ret = RFAILED;
                     break;
                  }
                  duUeCb = &duCb.actvCellLst[cellIdx]->ueCb[ueIdx];
                  duUeCb->f1UeDb = NULL;
                  duUeCb->gnbCuUeF1apId = gnbCuUeF1apId;
                  duUeCb->gnbDuUeF1apId = gnbDuUeF1apId;
                  GET_CRNTI(duUeCb->crnti, duUeCb->gnbDuUeF1apId);
                  duUeCb->ueState = UE_HANDIN_IN_PROGRESS;
               }

               if(duUeCb)
               {
                  DU_ALLOC(duUeCb->f1UeDb, sizeof(F1UeContextSetupDb));
                  if(duUeCb->f1UeDb)
                  {
                     memset(duUeCb->f1UeDb, 0, sizeof(F1UeContextSetupDb));
                     duUeCb->f1UeDb->actionType = UE_CTXT_SETUP;
                     duUeCb->f1UeDb->cellIdx = cellIdx;
                  }
                  else
                  {
                     DU_LOG("\nERROR  -->  F1AP: Memory Alloc Failed at procF1UeContextSetupReq()");
                     ret = RFAILED;
                     break;
                  }
               }
               else
               {
                  DU_LOG("\nERROR  -->  F1AP: DuUeCb is not found at procF1UeContextSetupReq()");
                  ret = RFAILED;
                  break;
               }
                 
               /* Extract UE capability info */
               if(rrcInfo->uE_CapabilityRAT_ContainerList)
               {
                  duUeCb->f1UeDb->duUeCfg.ueNrCapability = extractUeCapability(rrcInfo->uE_CapabilityRAT_ContainerList, duUeCb);
               }

               /* Extract IE extension */
               if(rrcInfo->iE_Extensions)
               {
                  if(extractCuToDuRrcInfoExt(rrcInfo->iE_Extensions, &duUeCb->f1UeDb->duUeCfg) != ROK)
                  {
                     DU_LOG("\nERROR  -->  F1AP: Failed to extract CU to DU RRC information extension IE");
                     //TODO: Update the failure cause in ue context Setup Response
                     ret = RFAILED;
                  }
               }
               break;
            } 

#ifdef NR_DRX
         case ProtocolIE_ID_id_DRXCycle:
            {
               duUeCb->f1UeDb->duUeCfg.drxCyclePres = true;
               drxCycle = &ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.DRXCycle;
               duUeCb->f1UeDb->duUeCfg.drxCycle.drxLongCycleLength =  convertLongDrxCycleLengthEnumToValue(drxCycle->longDRXCycleLength);
               if(drxCycle->shortDRXCycleLength)
               {
                  duUeCb->f1UeDb->duUeCfg.drxCycle.shortDrxCyclePres = true;
                  duUeCb->f1UeDb->duUeCfg.drxCycle.shortDrxCycle.drxShortCycle = \
                  convertShortDrxCycleLengthEnumToValue(*(drxCycle->shortDRXCycleLength));
               }
               if(drxCycle->shortDRXCycleTimer)
                  duUeCb->f1UeDb->duUeCfg.drxCycle.shortDrxCycle.drxShortCycleTimer = *(drxCycle->shortDRXCycleTimer);   

               break;
            }

#endif
         case ProtocolIE_ID_id_SRBs_ToBeSetup_List:
            {
               if(extractSrbListToSetup(&ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.SRBs_ToBeSetup_List,\
                        &duUeCb->f1UeDb->duUeCfg))
               {
                  DU_LOG("\nERROR  -->  DU APP : Failed at extractSrbListToSetup()");
                  //TODO: Update the failure cause in ue context Setup Response
                  ret = RFAILED;
               }
               break;
            }

         case ProtocolIE_ID_id_DRBs_ToBeSetup_List:
            {
               drbCfg = &ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.DRBs_ToBeSetup_List;

               if(extractDrbListToSetupMod(drbCfg, NULL, NULL, drbCfg->list.count, &duUeCb->f1UeDb->duUeCfg, &duUeCb->drbBitMap, NULLP))
               {
                  DU_LOG("\nERROR  -->  DU APP : Failed at extractDrbListToSetupMod()");
                  //TODO: Update the failure cause in ue context Setup Response
                  ret = RFAILED;
               }
               break;
            }
         case ProtocolIE_ID_id_RRCContainer:
            {
               /* Filling Dl RRC Msg Info */
               DU_ALLOC_SHRABL_BUF(duUeCb->f1UeDb->dlRrcMsg, sizeof(F1DlRrcMsg));
               if(!duUeCb->f1UeDb->dlRrcMsg)
               {
                  DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for DL RRC Msg in procUeCtxtSetupReq()");
                  ret = RFAILED;
               }
               else
               {
                  duUeCb->f1UeDb->dlRrcMsgPres = true;
                  memset(duUeCb->f1UeDb->dlRrcMsg, 0, sizeof(F1DlRrcMsg));
                  ret = extractDlRrcMsg(gnbDuUeF1apId, gnbCuUeF1apId, duUeCb->f1UeDb->dlRrcMsg,\
                        &ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer);
               }	  
               break;
            }

         case ProtocolIE_ID_id_RRCDeliveryStatusRequest:
            {
               if(duUeCb->f1UeDb->dlRrcMsg)
               {
                  if(duUeCb->f1UeDb->dlRrcMsg->rrcMsgPdu)
                  {
                     duUeCb->f1UeDb->dlRrcMsg->deliveryStatRpt = true;
                  }
                  else
                  {
                     DU_LOG("\nERROR  -->  Ignoring delivery report, since rrcContainer is not present");
                  }
               }
               break;
            }

         case ProtocolIE_ID_id_GNB_DU_UE_AMBR_UL:
            {
               /* MaximumBitRate Uplink */
               bitRateSize = ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.BitRate.size;
               if(bitRateSize > 0)
               {
                  DU_ALLOC_SHRABL_BUF(duUeCb->f1UeDb->duUeCfg.ambrCfg, bitRateSize);
                  if(!duUeCb->f1UeDb->duUeCfg.ambrCfg)
                  {
                     DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for bitRate in procUeCtxtSetupReq");
                     ret = RFAILED;
                  }
                  else
                  {
                     memset(duUeCb->f1UeDb->duUeCfg.ambrCfg, 0, sizeof(AmbrCfg)); 
                     memcpy(&duUeCb->f1UeDb->duUeCfg.ambrCfg->ulBr,
                           ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.BitRate.buf, bitRateSize);
                  }
               }
               else
                  ret = RFAILED;
               break;
            }

         default:
            {
               break;
            }
      } /* End of switch */

      /* In case of any failure in any IE */
      if(ret == RFAILED)
      {
         // BuildAndSendUeContextSetupRsp(cellId,ueId);
         DU_LOG("\nERROR  -->  F1AP: Failed to process UE CNTXT SETUP REQ at procF1UeContextSetupReq()"); 
         break;
      }
   } /* End of for loop of IEs */

   if(ret == ROK)
      ret = duProcUeContextSetupRequest(duUeCb);

   freeAperDecodeF1UeContextSetupReq(ueSetReq); 
   return ret;

}
/*******************************************************************
 * @brief Free the memory allocated for Dl Tunnel Info
 *
 * @details
 *
 *    Function : freeDlTnlInfo
 *
 *    Functionality:
 *       Free the memory allocated for Dl Tunnel Info
 *
 * @params[in] DLUPTNLInformation_ToBeSetup_List_t *
 * @return void
 *
 * ****************************************************************/

void freeDlTnlInfo(DLUPTNLInformation_ToBeSetup_List_t *tnlInfo)
{
   uint8_t arrIdx = 0;

   if(tnlInfo)
   {
      for(arrIdx=0; arrIdx < tnlInfo->list.count; arrIdx++)
      {
         DU_FREE(tnlInfo->list.array[arrIdx]->dLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf,\
               tnlInfo->list.array[arrIdx]->dLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.size);
         DU_FREE(tnlInfo->list.array[arrIdx]->dLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf,\
               tnlInfo->list.array[arrIdx]->dLUPTNLInformation.choice.gTPTunnel->gTP_TEID.size);
         DU_FREE(tnlInfo->list.array[arrIdx]->dLUPTNLInformation.choice.gTPTunnel, sizeof(GTPTunnel_t));
         DU_FREE(tnlInfo->list.array[arrIdx], sizeof(DLUPTNLInformation_ToBeSetup_Item_t));
      }
      DU_FREE(tnlInfo->list.array, tnlInfo->list.size);
   }
}

/*******************************************************************
 * @brief Free the memory allocated for DRB setup List
 *
 * @details
 *
 *    Function : freeDrbSetupList
 *
 *    Functionality:
 *       Free the memory allocated for DRB setup list
 *
 * @params[in] DRBs_Setup_List_t *
 * @return void
 *
 * ****************************************************************/
void freeDrbSetupList(DRBs_Setup_List_t *drbSetupList)
{
   uint8_t arrIdx = 0;
   DRBs_Setup_ItemIEs_t *drbItemIe = NULLP;

   for(arrIdx = 0; arrIdx < drbSetupList->list.count; arrIdx++)
   {
      drbItemIe = ((DRBs_Setup_ItemIEs_t *)drbSetupList->list.array[arrIdx]);
      freeDlTnlInfo(&drbItemIe->value.choice.DRBs_Setup_Item.dLUPTNLInformation_ToBeSetup_List);
      DU_FREE(drbSetupList->list.array[arrIdx],  sizeof(DRBs_Setup_Item_t));
   }
   DU_FREE(drbSetupList->list.array, drbSetupList->list.size);
}

/*******************************************************************
 * @brief Free the memory allocated for UE Setup response
 *
 * @details
 *
 *    Function : FreeUeContextSetupRsp
 *
 *    Functionality:
 *       Free the memory allocated for UE Setup response
 *
 * @params[in] F1AP PDU for UE setup response
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void FreeUeContextSetupRsp(F1AP_PDU_t *f1apMsg)
{
   uint8_t idx;
   UEContextSetupResponse_t *ueSetRsp = NULLP;

   if(f1apMsg)
   {
      if(f1apMsg->choice.successfulOutcome)
      {
         ueSetRsp = &f1apMsg->choice.successfulOutcome->value.choice.\
                    UEContextSetupResponse;
         if(ueSetRsp->protocolIEs.list.array)
         {
            for(idx = 0; idx < ueSetRsp->protocolIEs.list.count; idx++)
            {
               if(ueSetRsp->protocolIEs.list.array[idx])
               {
                  switch(ueSetRsp->protocolIEs.list.array[idx]->id)
                  {
                     case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
                        break;
                     case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
                        break;
                     case ProtocolIE_ID_id_C_RNTI:
                        break;
                     case ProtocolIE_ID_id_DUtoCURRCInformation:
                        {
                           CellGroupConfig_t *cellGrpCfg = NULLP;
                           cellGrpCfg  = &ueSetRsp->protocolIEs.list.array[idx]->value.choice.\
                                         DUtoCURRCInformation.cellGroupConfig;
                           if(cellGrpCfg->buf != NULLP)
                           {
                              DU_FREE(cellGrpCfg->buf, cellGrpCfg->size);
                              cellGrpCfg = NULLP;
                           }
                           break;
                        }
                     case ProtocolIE_ID_id_DRBs_Setup_List:
                        {
                           freeDrbSetupList(&ueSetRsp->protocolIEs.list.array[idx]->value.choice.DRBs_Setup_List); 
                           break;
                        }
                     default:
                        DU_LOG("\nERROR  -->  DUAPP: Invalid Id %ld at FreeUeContextSetupRsp()",\
                              ueSetRsp->protocolIEs.list.array[idx]->id);
                        break;
                  }
                  DU_FREE(ueSetRsp->protocolIEs.list.array[idx],\
                        sizeof(UEContextSetupResponseIEs_t));
               }
            }
            DU_FREE(ueSetRsp->protocolIEs.list.array, \
                  ueSetRsp->protocolIEs.list.size);
         }
         DU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      }
      DU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Builds Ue context Setup Rsp DU To CU Info
 *
 * @details
 *
 *    Function : EncodeUeCntxtDuToCuInfo
 *
 *    Functionality: Builds Ue context Setup Rsp DU To CU Info
 *
 * @params[in] CellGroupConfig_t *, CellGroupConfigRrc_t * 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t EncodeUeCntxtDuToCuInfo(CellGroupConfig_t *duToCuCellGrp, CellGroupConfigRrc_t *duCellGrpCfg)
{
   asn_enc_rval_t        encRetVal;

   xer_fprint(stdout, &asn_DEF_CellGroupConfigRrc, duCellGrpCfg);
   memset((uint8_t *)encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = uper_encode(&asn_DEF_CellGroupConfigRrc, 0, duCellGrpCfg, PrepFinalEncBuf, encBuf);
   /* Encode results */
   if(encRetVal.encoded == ENCODE_FAIL)
   {
      DU_LOG( "\nERROR  -->  F1AP : Could not encode UeCntxtDuToCuInfo (at %s)\n",\
	    encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
      return RFAILED;
   }
   else
   {
      DU_LOG("\nDEBUG   -->  F1AP : Created APER encoded buffer for UeCntxtDuToCuInfo\n");
#ifdef DEBUG_ASN_PRINT
      for(int i=0; i< encBufSize; i++)
      {
	 printf("%x",encBuf[i]);
      }
#endif
   }
   duToCuCellGrp->size = encBufSize;
   DU_ALLOC(duToCuCellGrp->buf, duToCuCellGrp->size);
   if(!duToCuCellGrp->buf)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in UeCntxtDuToCuInfo");
   }
   memcpy(duToCuCellGrp->buf, encBuf, duToCuCellGrp->size);
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills Dl Gtp tunnel Info
 *
 * @details
 *
 *    Function : fillGtpTunnelforDl
 *
 *    Functionality: Fills Dl Gtp tunnel Info
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillGtpTunnelforDl(GTPTunnel_t *gtpDl, GtpTnlCfg *gtpUeCfg)
{
   uint8_t bufSize = 0;

   gtpDl->transportLayerAddress.size	= 4*sizeof(uint8_t);
   DU_ALLOC(gtpDl->transportLayerAddress.buf, gtpDl->transportLayerAddress.size);
   if(gtpDl->transportLayerAddress.buf == NULLP)
   {
      return RFAILED;
   }
   memcpy(gtpDl->transportLayerAddress.buf, &gtpUeCfg->dlTnlAddress, gtpDl->transportLayerAddress.size);

   /*GTP TEID*/
   gtpDl->gTP_TEID.size = 4 * sizeof(uint8_t);
   DU_ALLOC(gtpDl->gTP_TEID.buf, gtpDl->gTP_TEID.size);
   if(gtpDl->gTP_TEID.buf == NULLP)
   {
      return RFAILED;
   }
   bufSize = 3; /*forming an Octect String*/
   fillTeIdString(bufSize, gtpUeCfg->teId, gtpDl->gTP_TEID.buf);

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills DL Tunnel Setup List
 *
 * @details
 *
 *    Function : fillDlTnlSetupList
 *
 *    Functionality: Fills the DL Tunnel Setup List
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillDlTnlSetupList(DLUPTNLInformation_ToBeSetup_List_t *dlTnlInfo, UpTnlCfg *tnlCfg)
{
   uint8_t ret = ROK, arrIdx = 0, eleCount = 0;

   eleCount = 1;
   dlTnlInfo->list.count = eleCount; 
   dlTnlInfo->list.size = (eleCount * sizeof(DLUPTNLInformation_ToBeSetup_Item_t *));

   /* Initialize the DL Tnl Setup List Members */
   DU_ALLOC(dlTnlInfo->list.array, dlTnlInfo->list.size);
   if(dlTnlInfo->list.array == NULLP)
   {
      DU_LOG(" ERROR  -->  F1AP : Memory allocation for DL Tnl Setup List in fillDlTnlSetupList()");
      ret = RFAILED;
   }
   for(arrIdx=0; arrIdx < eleCount; arrIdx++)
   {
      DU_ALLOC(dlTnlInfo->list.array[arrIdx], sizeof(DLUPTNLInformation_ToBeSetup_Item_t));
      if(dlTnlInfo->list.array[arrIdx] == NULLP)
      {
         DU_LOG(" ERROR  -->  F1AP : Memory allocation for arrIdx [%d] failed in fillDlTnlSetupList()", arrIdx);
         return RFAILED;
      }
      dlTnlInfo->list.array[arrIdx]->dLUPTNLInformation.present = UPTransportLayerInformation_PR_gTPTunnel;
      DU_ALLOC(dlTnlInfo->list.array[arrIdx]->dLUPTNLInformation.choice.gTPTunnel, sizeof(GTPTunnel_t));
      if(dlTnlInfo->list.array[arrIdx]->dLUPTNLInformation.choice.gTPTunnel == NULLP)
      {
         DU_LOG(" ERROR  -->  F1AP : Memory allocation for DL tunnel info in fillDlTnlSetupList()");
         return RFAILED;
      }
      ret = fillGtpTunnelforDl(dlTnlInfo->list.array[arrIdx]->dLUPTNLInformation.choice.gTPTunnel,\
               tnlCfg->tnlCfg1);
      if(ret != ROK)
         break;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Fills the Drb Setup List for Ue Context Setup Response
 *
 * @details
 *
 *    Function : fillDrbSetupList
 *
 *    Functionality: Fills the Drb Setup List for Ue Context Setup Response
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillDrbSetupList(DRBs_Setup_List_t *drbSetupList, DuUeCfg *ueCfg)
{
   uint8_t ret = ROK, arrIdx = 0, eleCount = 0;
   DRBs_Setup_ItemIEs_t *drbItemIe = NULLP;

   eleCount = ueCfg->numDrb;
   drbSetupList->list.count = eleCount;
   drbSetupList->list.size = \
	(eleCount * sizeof(DRBs_Setup_Item_t *));

   /* Initialize the Drb Setup List Members */
   DU_ALLOC(drbSetupList->list.array, drbSetupList->list.size);
   if(drbSetupList->list.array == NULLP)
   {
      DU_LOG(" ERROR  -->  F1AP : Memory allocation for DRB Setup List in fillDrbSetupList()");
      ret = RFAILED;
   }

   for(arrIdx=0; arrIdx < eleCount; arrIdx++)
   {
      DU_ALLOC(drbSetupList->list.array[arrIdx], sizeof(DRBs_Setup_Item_t));
      if(drbSetupList->list.array[arrIdx] == NULLP)
      {
         DU_LOG(" ERROR  -->  F1AP : Memory allocation for arrIdx [%d] failed in fillDrbSetupList()", arrIdx);
         return RFAILED;
      }
      drbItemIe = ((DRBs_Setup_ItemIEs_t *)drbSetupList->list.array[arrIdx]);
      drbItemIe->id = ProtocolIE_ID_id_DRBs_Setup_Item;
      drbItemIe->criticality = Criticality_reject;
      drbItemIe->value.present = DRBs_Setup_ItemIEs__value_PR_DRBs_Setup_Item;
      drbItemIe->value.choice.DRBs_Setup_Item.dRBID = ueCfg->upTnlInfo[arrIdx].drbId;
      ret = fillDlTnlSetupList(&drbItemIe->value.choice.DRBs_Setup_Item.dLUPTNLInformation_ToBeSetup_List,\
          &ueCfg->upTnlInfo[arrIdx]);
      if(ret != ROK)
         break;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Builds and sends the UE Setup Response
 *
 * @details
 *
 *    Function : BuildAndSendUeContextSetupRsp
 *
 *    Functionality: Constructs the UE Setup Response and sends
 *                   it to the DU through SCTP.
 *
 * @params[in] uint8_t cellId,uint8_t ueId
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendUeContextSetupRsp(uint8_t cellId,uint8_t ueId)
{
   uint8_t   idx, ret, cellIdx, elementCnt;
   uint32_t  gnbCuUeF1apId;   /* gNB-CU UE F1AP Id */
   uint32_t  gnbDuUeF1apId;   /* gNB-DU UE F1AP Id */
   asn_enc_rval_t  encRetVal;        /* Encoder return value */
   F1AP_PDU_t               *f1apMsg = NULLP;
   UEContextSetupResponse_t *ueSetRsp = NULLP;
   DuUeCb                   *ueCb = NULLP;

   DU_LOG("\n INFO   -->  F1AP : Building UE Context Setup Response for cellId %d, ueId %d\n", cellId, ueId);

   while(true)
   {
      DU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
         DU_LOG(" ERROR  -->  F1AP : Memory allocation for F1AP-PDU failed");
         ret = RFAILED;
         break;
      }

      f1apMsg->present = F1AP_PDU_PR_successfulOutcome;
      DU_ALLOC(f1apMsg->choice.successfulOutcome,
            sizeof(SuccessfulOutcome_t));
      if(f1apMsg->choice.successfulOutcome == NULLP)
      {
         DU_LOG(" ERROR  -->  F1AP : Memory allocation for	F1AP-PDU failed");
         ret = RFAILED;
         break;
      }

      f1apMsg->choice.successfulOutcome->procedureCode = \
                                                         ProcedureCode_id_UEContextSetup;
      f1apMsg->choice.successfulOutcome->criticality = Criticality_reject;
      f1apMsg->choice.successfulOutcome->value.present = \
                                                         SuccessfulOutcome__value_PR_UEContextSetupResponse;

      ueSetRsp =
         &f1apMsg->choice.successfulOutcome->value.choice.UEContextSetupResponse;
      elementCnt = 5;
      ueSetRsp->protocolIEs.list.count = elementCnt;
      ueSetRsp->protocolIEs.list.size = \
                                        elementCnt * sizeof(UEContextSetupResponse_t *);

      /* Initialize the UESetup members */
      DU_ALLOC(ueSetRsp->protocolIEs.list.array, \
            ueSetRsp->protocolIEs.list.size);
      if(ueSetRsp->protocolIEs.list.array == NULLP)
      {
         DU_LOG(" ERROR  -->  F1AP : Memory allocation for UE Setup Response failed");
         ret = RFAILED;
         break;
      }

      for(idx=0; idx<elementCnt; idx++)
      {
         DU_ALLOC(ueSetRsp->protocolIEs.list.array[idx],\
               sizeof(UEContextSetupResponseIEs_t));
         if(ueSetRsp->protocolIEs.list.array[idx] == NULLP)
         {
            DU_LOG(" ERROR  -->  F1AP : Memory allocation for UE Setup Response failed");
            ret = RFAILED;
            break;
         }
      }
      /* Fetching Ue Cb Info*/
      GET_CELL_IDX(cellId, cellIdx);
      gnbDuUeF1apId = duCb.actvCellLst[cellIdx]->ueCb[ueId-1].gnbDuUeF1apId;
      gnbCuUeF1apId = duCb.actvCellLst[cellIdx]->ueCb[ueId-1].gnbCuUeF1apId;
      ueCb = &duCb.actvCellLst[cellIdx]->ueCb[ueId-1];

      idx = 0;
      /*GNB CU UE F1AP ID*/
      ueSetRsp->protocolIEs.list.array[idx]->id	= \
                                                  ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;
      ueSetRsp->protocolIEs.list.array[idx]->criticality = Criticality_reject;
      ueSetRsp->protocolIEs.list.array[idx]->value.present = \
                                                             UEContextSetupResponseIEs__value_PR_GNB_CU_UE_F1AP_ID;
      ueSetRsp->protocolIEs.list.array[idx]->value.choice.GNB_CU_UE_F1AP_ID = gnbCuUeF1apId;

      /*GNB DU UE F1AP ID*/
      idx++;
      ueSetRsp->protocolIEs.list.array[idx]->id	= \
                                                  ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
      ueSetRsp->protocolIEs.list.array[idx]->criticality = Criticality_reject;
      ueSetRsp->protocolIEs.list.array[idx]->value.present = \
                                                             UEContextSetupResponseIEs__value_PR_GNB_DU_UE_F1AP_ID;
      ueSetRsp->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID = gnbDuUeF1apId;


      /*DUtoCURRC Information */
      idx++;
      ueSetRsp->protocolIEs.list.array[idx]->id  = \
                                                   ProtocolIE_ID_id_DUtoCURRCInformation;
      ueSetRsp->protocolIEs.list.array[idx]->criticality = Criticality_reject;
      ueSetRsp->protocolIEs.list.array[idx]->value.present =\
                                                            UEContextSetupResponseIEs__value_PR_DUtoCURRCInformation;
      BuildCellGroupConfigRrc(ueCb, &ueSetRsp->protocolIEs.list.array[idx]->value.choice.DUtoCURRCInformation.cellGroupConfig);

      /* CRNTI */
      idx++;
      ueSetRsp->protocolIEs.list.array[idx]->id  = ProtocolIE_ID_id_C_RNTI;
      ueSetRsp->protocolIEs.list.array[idx]->criticality = Criticality_reject;
      ueSetRsp->protocolIEs.list.array[idx]->value.present = UEContextSetupResponseIEs__value_PR_C_RNTI;
      ueSetRsp->protocolIEs.list.array[idx]->value.choice.C_RNTI = ueCb->crnti; 


      /* Drb Setup List */
      idx++;
      ueSetRsp->protocolIEs.list.array[idx]->id  = \
                                                   ProtocolIE_ID_id_DRBs_Setup_List;
      ueSetRsp->protocolIEs.list.array[idx]->criticality = Criticality_reject;
      ueSetRsp->protocolIEs.list.array[idx]->value.present =\
                                                            UEContextSetupResponseIEs__value_PR_DRBs_Setup_List;
      ret = fillDrbSetupList(&ueSetRsp->protocolIEs.list.array[idx]->value.choice.DRBs_Setup_List,\
            &ueCb->f1UeDb->duUeCfg);
      if(ret == RFAILED)
      {
         DU_LOG("\nERROR  -->  F1AP : Failed to fillDrbSetupList in BuildAndSendUeContextSetupRsp()");
         freeF1UeDb(ueCb->f1UeDb);
         ueCb->f1UeDb = NULLP;
         break;
      }

      /* Free UeContext Db created during Ue context Req */
      freeF1UeDb(ueCb->f1UeDb);
      ueCb->f1UeDb = NULLP;

      /* TODO: To send Drb list */
      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the UE context setup response type as APER */
      memset((uint8_t *)encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
            encBuf);
      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG( "\nERROR  -->  F1AP : Could not encode UE Context Setup Response structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         ret = RFAILED;
         break;
      }
      else
      {
         DU_LOG("\nDEBUG   -->  F1AP : Created APER encoded buffer for UE Context Setup Response\n");
#ifdef DEBUG_ASN_PRINT
         for(int i=0; i< encBufSize; i++)
         {
            printf("%x",encBuf[i]);
         }
#endif
      }

      /* Sending  msg  */
      if(sendF1APMsg()	!= ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Sending UE Context Setup Response failed");
         ret = RFAILED;
         break;
      }
      break;
   }
   FreeUeContextSetupRsp(f1apMsg);
   return ret;
}/* End of BuildAndSendUeContextSetupRsp */

/*******************************************************************
*
* @brief  Build And Send Ue Context Rsp 
*
* @details
*
*    Function : BuildAndSendUeCtxtRsp 
*
*    Functionality : Build And Send Ue Context Rsp

* @params[in]
* @return sucess = ROK
*         failure = RFAILED
*
* ****************************************************************/
uint8_t BuildAndSendUeCtxtRsp(uint8_t cellId, uint8_t ueId)
{
   uint8_t cellIdx = 0, actionType = 0; 

   GET_CELL_IDX(cellId, cellIdx);
   actionType = duCb.actvCellLst[cellIdx]->ueCb[ueId-1].f1UeDb->actionType;

   switch(actionType)
   {
      case UE_CTXT_SETUP:
         {
            if((BuildAndSendUeContextSetupRsp(cellId,ueId)) != ROK)
            {
               DU_LOG("\nERROR  -->  F1AP : Failed at BuildAndSendUeContextSetupRsp()");
               return RFAILED;
            }
            break;
         }
      case UE_CTXT_MOD:
         {
            if((BuildAndSendUeContextModRsp(&duCb.actvCellLst[cellIdx]->ueCb[ueId-1])) != ROK)
            {
               DU_LOG("\nERROR  -->  F1AP : Failed at BuildAndSendUeContextModRsp");
               return RFAILED;
            }
            break;
         }
      default:
         DU_LOG("ERROR  -->  F1AP: Invalid Action Type %d at BuildAndSendUeCtxtRsp()", actionType);
         break;

   }
   return ROK;
}

/*******************************************************************
 *
 * @brief deallocating the memory of  F1reset msg
 *
 * @details
 *
 *    Function : FreeF1ResetReq
 *
 *    Functionality :
 *         - freeing memory of F1reset request msg
 *
 * @params[in]
 * @return void
 *
 *
 * ****************************************************************/
void FreeF1ResetReq(F1AP_PDU_t *f1apMsg)
{
   uint8_t idx =0 ;
   Reset_t *f1ResetMsg;

   if(f1apMsg)
   {
      if(f1apMsg->choice.initiatingMessage)
      {
	 f1ResetMsg = &f1apMsg->choice.initiatingMessage->value.choice.Reset;

	 if(f1ResetMsg->protocolIEs.list.array)
	 {
	    for(idx=0 ;idx < f1ResetMsg->protocolIEs.list.count ; idx++)
	    {
	       if(f1ResetMsg->protocolIEs.list.array[idx])
	       {
		  DU_FREE(f1ResetMsg->protocolIEs.list.array[idx],sizeof(ResetIEs_t));
	       }
	    }
	    DU_FREE(f1ResetMsg->protocolIEs.list.array,f1ResetMsg->protocolIEs.list.size);
	 }
	 DU_FREE(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      }
      DU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
   }
}
/*******************************************************************
 *
 * @brief Build and Send F1reset request 
 *
 * @details
 *
 *    Function : BuildAndSendF1ResetReq
 *
 *    Functionality:
 *         - Build and Send F1reset request msg
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendF1ResetReq()
{
   uint8_t          elementCnt=0;
   uint8_t          idx=0;
   uint8_t          ret= RFAILED;
   Reset_t          *f1ResetMsg = NULLP;
   F1AP_PDU_t       *f1apMsg = NULLP;
   asn_enc_rval_t   encRetVal;
   DU_LOG("\nINFO   -->  F1AP : Building F1 Reset request \n");
   do
   {
      DU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
	 break;
      }
      f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
      DU_ALLOC(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      if(f1apMsg->choice.initiatingMessage == NULLP)
      {
	 break;
      }
      f1apMsg->choice.initiatingMessage->procedureCode = ProcedureCode_id_Reset;
      f1apMsg->choice.initiatingMessage->criticality = Criticality_reject;
      f1apMsg->choice.initiatingMessage->value.present = InitiatingMessage__value_PR_Reset;

      f1ResetMsg = &f1apMsg->choice.initiatingMessage->value.choice.Reset;

      elementCnt = 3;
      f1ResetMsg->protocolIEs.list.count = elementCnt;
      f1ResetMsg->protocolIEs.list.size = elementCnt * sizeof(ResetIEs_t *);

      /* Initialize the F1Setup members */
      DU_ALLOC(f1ResetMsg->protocolIEs.list.array,f1ResetMsg->protocolIEs.list.size);
      if(f1ResetMsg->protocolIEs.list.array == NULLP)
      {
	 break;
      }
      for(idx=0; idx<elementCnt; idx++)
      {
	 DU_ALLOC(f1ResetMsg->protocolIEs.list.array[idx],sizeof(ResetIEs_t));
	 if(f1ResetMsg->protocolIEs.list.array[idx] == NULLP)
	 {
	    break;
	 }
      }

      /*TransactionID*/
      idx=0;
      f1ResetMsg->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_TransactionID;
      f1ResetMsg->protocolIEs.list.array[idx]->criticality = Criticality_reject;
      f1ResetMsg->protocolIEs.list.array[idx]->value.present = ResetIEs__value_PR_TransactionID;
      f1ResetMsg->protocolIEs.list.array[idx]->value.choice.TransactionID = 1;

      /*Cause*/
      idx++;
      f1ResetMsg->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_Cause;
      f1ResetMsg->protocolIEs.list.array[idx]->criticality = Criticality_ignore;
      f1ResetMsg->protocolIEs.list.array[idx]->value.present = ResetIEs__value_PR_Cause;
      f1ResetMsg->protocolIEs.list.array[idx]->value.choice.Cause.present = Cause_PR_radioNetwork;
      f1ResetMsg->protocolIEs.list.array[idx]->value.choice.Cause.choice.radioNetwork = CauseRadioNetwork_action_desirable_for_radio_reasons;

      /*Reset Type*/
      idx++;
      f1ResetMsg->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_ResetType;
      f1ResetMsg->protocolIEs.list.array[idx]->criticality = Criticality_reject;
      f1ResetMsg->protocolIEs.list.array[idx]->value.present = ResetIEs__value_PR_ResetType;
      f1ResetMsg->protocolIEs.list.array[idx]->value.choice.ResetType.present = ResetType_PR_f1_Interface;
      f1ResetMsg->protocolIEs.list.array[idx]->value.choice.ResetType.choice.f1_Interface = ResetAll_reset_all;

      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the F1SetupRequest type as APER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
	    encBuf);

      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
	 DU_LOG("\nERROR  -->  F1AP : Could not encode F1Reset structure (at %s)\n",\
	       encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
	 break;
      }
      else
      {
	 DU_LOG("\nDEBUG   -->  F1AP : Created APER encoded buffer for F1Reset\n");
#ifdef DEBUG_ASN_PRINT
	 for(idx=0; idx< encBufSize; idx++)
	 {
	    printf("%x",encBuf[idx]);
	 }
#endif
      }

      if(sendF1APMsg() != ROK)
      {
	 DU_LOG("\nERROR  -->  F1AP : Sending F1 Reset request failed");
	 break;
      }

      ret = ROK;
      break;
   }while(true);

   FreeF1ResetReq(f1apMsg);
   return ret;
}
/*******************************************************************
 *
 * @brief Build And Send F1ResetAck
 *
 * @details
 *
 *    Function : BuildAndSendF1ResetAck
 *
 *    Functionality:
 *         - Build And Send  F1ResetRSP
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void FreeF1ResetAck(F1AP_PDU_t *f1apMsg)
{
   uint8_t idx;
   ResetAcknowledge_t *f1ResetAck;

   if(f1apMsg)
   {
      if(f1apMsg->choice.successfulOutcome)
      {
	 f1ResetAck= &f1apMsg->choice.successfulOutcome->value.choice.ResetAcknowledge;

	 if(f1ResetAck->protocolIEs.list.array)
	 {
	    for(idx=0; idx<f1ResetAck->protocolIEs.list.count ; idx++)
	    {
	       if(f1ResetAck->protocolIEs.list.array[idx])
	       {
		  DU_FREE(f1ResetAck->protocolIEs.list.array[idx], sizeof(ResetAcknowledgeIEs_t));
	       }
	    }
	    DU_FREE(f1ResetAck->protocolIEs.list.array, f1ResetAck->protocolIEs.list.size );
	 }
	 DU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      }
      DU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Build And Send F1ResetAck
 *
 * @details
 *
 *    Function : BuildAndSendF1ResetAck
 *
 *    Functionality:
 *         - Build And Send  F1ResetRSP
 *
 *  @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendF1ResetAck()
{
   uint8_t                idx = 0;
   uint8_t                elementCnt = 0;
   uint8_t                ret = RFAILED;
   F1AP_PDU_t             *f1apMsg = NULL;
   ResetAcknowledge_t     *f1ResetAck = NULLP;
   asn_enc_rval_t         encRetVal;
   DU_LOG("\nINFO   -->  F1AP : Building F1 Reset Acknowledgment \n");

   do{
      /* Allocate the memory for F1ResetRequest_t */
      DU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
	 DU_LOG("\nERROR  -->  F1AP : Memory allocation for F1AP-PDU failed");
	 break;
      }

      f1apMsg->present =  F1AP_PDU_PR_successfulOutcome;

      DU_ALLOC(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      if(f1apMsg->choice.successfulOutcome == NULLP)
      {
	 DU_LOG("\nERROR  -->  F1AP : Memory allocation for F1AP-PDU failed");
	 break;
      }
      f1apMsg->choice.successfulOutcome->criticality   = Criticality_reject;
      f1apMsg->choice.successfulOutcome->value.present = SuccessfulOutcome__value_PR_ResetAcknowledge;
      f1ResetAck = &f1apMsg->choice.successfulOutcome->value.choice.ResetAcknowledge;

      elementCnt = 1;

      f1ResetAck->protocolIEs.list.count = elementCnt;
      f1ResetAck->protocolIEs.list.size = elementCnt*sizeof(ResetAcknowledgeIEs_t *);

      DU_ALLOC(f1ResetAck->protocolIEs.list.array, f1ResetAck->protocolIEs.list.size );
      if(f1ResetAck->protocolIEs.list.array == NULLP)
      {
	 DU_LOG("\nERROR  -->  F1AP : Memory allocation for F1ResetAckIEs failed");
	 break;
      }

      for(idx=0; idx<elementCnt; idx++)
      {
	 DU_ALLOC(f1ResetAck->protocolIEs.list.array[idx], sizeof(ResetAcknowledgeIEs_t));
	 if(f1ResetAck->protocolIEs.list.array[idx] == NULLP)
	 {
	    break;
	 }
      }
      /*TransactionID*/
      idx = 0;
      f1ResetAck->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_TransactionID;
      f1ResetAck->protocolIEs.list.array[idx]->criticality = Criticality_reject;
      f1ResetAck->protocolIEs.list.array[idx]->value.present = ResetAcknowledgeIEs__value_PR_TransactionID;
      f1ResetAck->protocolIEs.list.array[idx]->value.choice.TransactionID = TRANS_ID;

      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the F1SetupRequest type as UPER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf, encBuf);

      /* Check encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
	 DU_LOG("\nERROR  -->  F1AP : Could not encode F1ResetAck structure (at %s)\n",\
	       encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
	 break;
      }
      else
      {
	 DU_LOG("\nDUBUG   -->  F1AP : Created APER encoded buffer for F1ResetAck\n");
#ifdef DEBUG_ASN_PRINT
	 for(int i=0; i< encBufSize; i++)
	 {
	    printf("%x",encBuf[i]);
	 }
#endif
      }
      /* Sending msg */
      if(sendF1APMsg() != ROK)
      {
	 DU_LOG("\nERROR  -->  F1AP : Sending F1 Reset Acknowledgement failed");
	 break;
      }

      ret = ROK;
      break;
   }while(true);

   FreeF1ResetAck(f1apMsg);
   return ret;
}
/******************************************************************
*
* @brief free F1 reset msg allocated by aper_decoder 
*
* @details
*
*    Function : freeAperDecodeF1ResetMsg 
*
*    Functionality: free F1 reset msg allocated by aper_decoder 
*
* @params[in] Reset_t *f1ResetMsg 
* @return void 
*
* ****************************************************************/

void freeAperDecodeF1ResetMsg(Reset_t *f1ResetMsg)
{
   uint8_t ieIdx =0;
   if(f1ResetMsg->protocolIEs.list.array)
   {
      for(ieIdx=0 ;ieIdx < f1ResetMsg->protocolIEs.list.count ; ieIdx++)
      {
	 if(f1ResetMsg->protocolIEs.list.array[ieIdx])
	 {
	    free(f1ResetMsg->protocolIEs.list.array[ieIdx]);
	 }
      }
      free(f1ResetMsg->protocolIEs.list.array);
   }
}

/******************************************************************
 *
 * @brief Processes DL RRC Message Transfer  sent by CU
 *
 * @details
 *
 *    Function : procF1ResetReq
 *
 *    Functionality: Processes DL RRC Message Transfer sent by CU
 *
 * @params[in] F1AP_PDU_t ASN decoded F1AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procF1ResetReq(F1AP_PDU_t *f1apMsg)
{
   /* we are currently not supporting Ue release. right now we are supporting only init case of fireset */
   uint8_t       ieIdx = 0;
   uint8_t        ret = ROK;
   Reset_t       *f1ResetMsg = NULLP;

   DU_LOG("\nINFO   -->  Processing F1 reset request");
   f1ResetMsg = &f1apMsg->choice.initiatingMessage->value.choice.Reset;

   for(ieIdx=0; ieIdx<f1ResetMsg->protocolIEs.list.count; ieIdx++)
   {
      switch(f1ResetMsg->protocolIEs.list.array[ieIdx]->id)
      {
	 case ProtocolIE_ID_id_TransactionID:
	    break;

	 case ProtocolIE_ID_id_Cause:
	    break;

	 case ProtocolIE_ID_id_ResetType:
	    {
	       break;
	    }

	 default:
	    break;
      }
   }
   ret = BuildAndSendF1ResetAck();
   DU_LOG("\nINFO   -->  UE release is not supported for now");

   freeAperDecodeF1ResetMsg(f1ResetMsg);

   return ret;
}

/*******************************************************************
 *
 * @brief free the RRC delivery report
 *
 * @details
 *
 *    Function : freeRrcDeliveryReport
 *
 *    Functionality: free the RRC delivery report
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void freeRrcDeliveryReport(F1AP_PDU_t  *f1apMsg )
{
   uint8_t idx=0;
   RRCDeliveryReport_t *rrcDeliveryReport= NULLP;

   if(f1apMsg)
   {
      if(f1apMsg->choice.initiatingMessage)
      {
	 rrcDeliveryReport = &f1apMsg->choice.initiatingMessage->value.choice.RRCDeliveryReport;
	 if(rrcDeliveryReport->protocolIEs.list.array)
	 {
	    for(idx =0 ;idx <rrcDeliveryReport->protocolIEs.list.count ;
		  idx++)
	    {
	       if(rrcDeliveryReport->protocolIEs.list.array[idx])
	       {
		  DU_FREE(rrcDeliveryReport->protocolIEs.list.array[idx],
			sizeof(RRCDeliveryReportIEs_t));
	       }   
	    }
	    DU_FREE(rrcDeliveryReport->protocolIEs.list.array,
		  rrcDeliveryReport->protocolIEs.list.size);
	 }
	 DU_FREE(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
      }
      DU_FREE(f1apMsg,
	    sizeof(F1AP_PDU_t));
   }
}

/*******************************************************************
*
* @brief Builds and sends the RRC delivery report
*
* @details
*
*    Function : BuildAndSendRrcDeliveryReport
*
*    Functionality: Builds and sends the RRC delivery report
*
* @params[in]
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t BuildAndSendRrcDeliveryReport(uint32_t gnbCuUeF1apId, \
   uint32_t gnbDuUeF1apId, RrcDeliveryReport *rrcDelivery)
{
   uint8_t             ret = RFAILED;
   uint8_t             idx    = 0;
   uint8_t             idx1   = 0;
   uint8_t             elementCnt = 0;
   F1AP_PDU_t          *f1apMsg = NULLP;
   asn_enc_rval_t      encRetVal;  
   RRCDeliveryReport_t *rrcDeliveryReport= NULLP;

   do{

      DU_LOG("\nINFO   -->  F1AP : Building RRC delivery Message Transfer Message\n");
      DU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
	 DU_LOG(" ERROR  -->  F1AP : Memory allocation for F1AP-PDU failed");
	 break;
      }
      f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
      DU_ALLOC(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
      if(f1apMsg->choice.initiatingMessage == NULLP)
      {
	 DU_LOG(" ERROR  -->  F1AP : Memory allocation for  F1AP-PDU  failed");
	 break;
      }
      f1apMsg->choice.initiatingMessage->procedureCode = ProcedureCode_id_RRCDeliveryReport;
      f1apMsg->choice.initiatingMessage->criticality   = Criticality_ignore;
      f1apMsg->choice.initiatingMessage->value.present = InitiatingMessage__value_PR_RRCDeliveryReport;

      rrcDeliveryReport = &f1apMsg->choice.initiatingMessage->value.choice.RRCDeliveryReport;
      elementCnt = 4;
      rrcDeliveryReport->protocolIEs.list.count = elementCnt;
      rrcDeliveryReport->protocolIEs.list.size = elementCnt * sizeof(RRCDeliveryReportIEs_t *);

      /* Initialize the F1Setup members */
      DU_ALLOC(rrcDeliveryReport->protocolIEs.list.array, rrcDeliveryReport->protocolIEs.list.size);
      if(rrcDeliveryReport->protocolIEs.list.array == NULLP)
      {
	 DU_LOG(" ERROR  -->  F1AP : Memory allocation for RRC Delivery  failed");
	 break;
      }
      for(idx =0 ;idx <elementCnt; idx++)
      {
	 DU_ALLOC(rrcDeliveryReport->protocolIEs.list.array[idx], sizeof(RRCDeliveryReportIEs_t));
	 if(rrcDeliveryReport->protocolIEs.list.array[idx] == NULLP)
	 {
	    break;
	 }
      }

      idx1 = 0;

      /*GNB CU UE F1AP ID*/
      rrcDeliveryReport->protocolIEs.list.array[idx1]->id = ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->criticality = Criticality_reject;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->value.present = RRCDeliveryReportIEs__value_PR_GNB_CU_UE_F1AP_ID;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->value.choice.GNB_CU_UE_F1AP_ID = gnbCuUeF1apId;

      /*GNB DU UE F1AP ID*/
      idx1++;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->id = ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->criticality = Criticality_reject;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->value.present = RRCDeliveryReportIEs__value_PR_GNB_DU_UE_F1AP_ID;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->value.choice.GNB_DU_UE_F1AP_ID = gnbDuUeF1apId;

      /*RRC delivery status*/
      idx1++;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->id = ProtocolIE_ID_id_RRCDeliveryStatus;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->criticality = Criticality_ignore;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->value.present = RRCDeliveryReportIEs__value_PR_RRCDeliveryStatus;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->value.choice.RRCDeliveryStatus.delivery_status =\
      rrcDelivery->rrcDeliveryStatus.deliveryStatus;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->value.choice.RRCDeliveryStatus.triggering_message =\
      rrcDelivery->rrcDeliveryStatus.triggeringMessage;

      /* SRB ID */ 
      idx1++;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->id = ProtocolIE_ID_id_SRBID;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->criticality = Criticality_ignore;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->value.present = RRCDeliveryReportIEs__value_PR_SRBID;
      rrcDeliveryReport->protocolIEs.list.array[idx1]->value.choice.SRBID =  rrcDelivery->srbId;

      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the RRC DELIVERY REPORT type as APER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
	    encBuf);

      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
	 DU_LOG("\nERROR  -->  F1AP : Could not encode RRC Delivery Msg structure (at %s)\n",\
	       encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
	 break;
      }
      else
      {
	 DU_LOG("\nDEBUG   -->  F1AP : Created APER encoded buffer for RRC Delivery Msg \n");
#ifdef DEBUG_ASN_PRINT
	 for(idx=0; idx< encBufSize; idx++)
	 {
	    printf("%x",encBuf[idx]);
	 }
#endif
      }

      /* Sending msg */
      if(sendF1APMsg() != ROK)
      {
	 DU_LOG("\nERROR  -->  F1AP : Sending RRC delivery  msg request failed");
	 break;
      }
      ret = ROK;
      break;

   }while(true);

   freeRrcDeliveryReport(f1apMsg);
   return ret;
}

/*******************************************************************
 *
 * @brief Processes cells to be activated
 *
 * @details
 *
 *    Function : extractCellsToBeActivated
 *
 *    Functionality:
 *      - Processes cells to be activated list received in F1SetupRsp
 *
 * @params[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t extractCellsToBeActivated(Cells_to_be_Activated_List_t cellsToActivate)
{
   uint8_t  ret = ROK;
   uint16_t idx, pci = 0;
   uint64_t nci;
   Cells_to_be_Activated_List_Item_t cell;

   for(idx=0; idx<cellsToActivate.list.count; idx++)
   {
      nci = 0;
      cell = cellsToActivate.list.array[idx]->value.choice.Cells_to_be_Activated_List_Item;
      bitStringToInt(&cell.nRCGI.nRCellIdentity, &nci);

      if(cell.nRPCI)
      {
         pci = *cell.nRPCI;
      }
      ret = duProcCellsToBeActivated(cell.nRCGI.pLMN_Identity.buf, nci, pci);
   }
   return ret;
}
/******************************************************************
*
* @brief Processes F1 Setup Response allocated by aper_decoder 
*
* @details
*
*    Function : freeF1SetupRsp 
*
*    Functionality: free F1 Setup Response allocated by aper_decoder 
*
* @params[in] F1SetupResponse_t *f1SetRspMsg 
* @return void 
*
* ****************************************************************/

void freeAperDecodeF1SetupRsp(F1SetupResponse_t *f1SetRspMsg)
{
   uint8_t ieIdx =0;
   uint8_t arrIdx =0;
   Cells_to_be_Activated_List_t *cellToActivate =NULLP;
   RRC_Version_t      *rrcVer =NULLP;

   if(f1SetRspMsg->protocolIEs.list.array)
   {
      for(ieIdx=0; ieIdx<f1SetRspMsg->protocolIEs.list.count; ieIdx++)
      {
	 if(f1SetRspMsg->protocolIEs.list.array[ieIdx])
	 {
	    switch(f1SetRspMsg->protocolIEs.list.array[ieIdx]->id)
	    {
	       case ProtocolIE_ID_id_Cells_to_be_Activated_List:
		  {
		     cellToActivate =
			&f1SetRspMsg->protocolIEs.list.array[ieIdx]->value.choice.Cells_to_be_Activated_List;
		     if(cellToActivate->list.array)
		     {
			for(arrIdx=0; arrIdx<cellToActivate->list.count ; arrIdx++)
			{
			   if(cellToActivate->list.array[arrIdx])
			   {

			      if(cellToActivate->list.array[0]->value.choice.Cells_to_be_Activated_List_Item.nRCGI.\
			      pLMN_Identity.buf)
			      {
				 if(cellToActivate->list.array[0]->value.choice.\
				       Cells_to_be_Activated_List_Item.nRCGI.nRCellIdentity.buf)
				 {
				    free(cellToActivate->list.array[0]->value.choice.\
					  Cells_to_be_Activated_List_Item.nRCGI.nRCellIdentity.buf);
				 }

				 free(cellToActivate->list.array[0]->value.choice.Cells_to_be_Activated_List_Item.\
				       nRCGI.pLMN_Identity.buf);
			      }
			      free(cellToActivate->list.array[arrIdx]);
			   }
			}
			free(cellToActivate->list.array);
		     }
		     break;
		  }
	       case ProtocolIE_ID_id_TransactionID:
		  {
		     break;
		  }
	       case ProtocolIE_ID_id_gNB_CU_Name:
		  {
		     free(f1SetRspMsg->protocolIEs.list.array[ieIdx]->value.choice.GNB_CU_Name.buf);
		     break;
		  }
	       case ProtocolIE_ID_id_GNB_CU_RRC_Version:
		  {
		     rrcVer = &f1SetRspMsg->protocolIEs.list.array[ieIdx]->value.choice.RRC_Version;
		     if(rrcVer->latest_RRC_Version.buf)
		     {
			if(rrcVer->iE_Extensions)
			{
			   if(rrcVer->iE_Extensions->list.array)
			   {
			      if(rrcVer->iE_Extensions->list.array[0])
			      {
				 if(rrcVer->iE_Extensions->list.\
				       array[0]->extensionValue.choice.Latest_RRC_Version_Enhanced.buf)
				 {
				    free(rrcVer->iE_Extensions->list.\
					  array[0]->extensionValue.choice.Latest_RRC_Version_Enhanced.buf);
				 }
				 free(rrcVer->iE_Extensions->list.array[0]);
			      }
			      free(rrcVer->iE_Extensions->list.array);
			   }
			   free(rrcVer->iE_Extensions);
			}
			free(rrcVer->latest_RRC_Version.buf);
		     }
		     break;

		  }
	       default:
		  {
		     DU_LOG("\nERROR  -->  DU_APP : Invalid IE received in F1SetupRsp:%ld",
			   f1SetRspMsg->protocolIEs.list.array[ieIdx]->id);
		  }
	    }
	    free(f1SetRspMsg->protocolIEs.list.array[ieIdx]);
	 }
      }
      free(f1SetRspMsg->protocolIEs.list.array);
   }
}
/******************************************************************
 *
 * @brief Processes F1 Setup Response sent by CU
 *
 * @details
 *
 *    Function : procF1SetupRsp
 *
 *    Functionality: Processes F1 Setup Response sent by CU
 *
 * @params[in] F1AP_PDU_t ASN decoded F1AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procF1SetupRsp(F1AP_PDU_t *f1apMsg, MsgLen recvBufLen, char *recvBuf)
{
   uint8_t ret = ROK;
   uint16_t idx =0;
   F1SetupResponse_t *f1SetRspMsg = NULLP;
   GNB_CU_Name_t     *cuName = NULLP;
   F1SetupRsp  f1SetRspDb;
   RRC_Version_t      *rrcVer =NULLP;
   
   memset(&f1SetRspDb, 0, sizeof(F1SetupRsp));

   DU_LOG("\nINFO   -->  F1AP : F1 Setup Response received");
   f1SetRspMsg = &f1apMsg->choice.successfulOutcome->value.choice.F1SetupResponse;

   for(idx=0; idx<f1SetRspMsg->protocolIEs.list.count; idx++)
   {
      switch(f1SetRspMsg->protocolIEs.list.array[idx]->id)
      {
	 case ProtocolIE_ID_id_Cells_to_be_Activated_List:
	    {
	       extractCellsToBeActivated(f1SetRspMsg->protocolIEs.list.array[idx]->\
		     value.choice.Cells_to_be_Activated_List);
	       break;
	    }
	 case ProtocolIE_ID_id_TransactionID:
	    {
	       f1SetRspDb.transId = f1SetRspMsg->protocolIEs.list.array[idx]->\
				    value.choice.TransactionID;
	       break;
	    }
	 case ProtocolIE_ID_id_gNB_CU_Name:
	    {
	       cuName = &f1SetRspMsg->protocolIEs.list.array[idx]->\
			value.choice.GNB_CU_Name;
	       strcpy(f1SetRspDb.cuName, (const char*)cuName->buf);
	       break;
	    }
	 case ProtocolIE_ID_id_GNB_CU_RRC_Version:
	    {
	       rrcVer = &f1SetRspMsg->protocolIEs.list.array[idx]->value.choice.RRC_Version;
	       strcpy(f1SetRspDb.rrcVersion.rrcVer,
		     (const char*)rrcVer->latest_RRC_Version.buf);
	       break;
	    }
	 default:
	    DU_LOG("\nERROR  -->  DU_APP : Invalid IE received in F1SetupRsp:%ld",
		  f1SetRspMsg->protocolIEs.list.array[idx]->id);
      }
   }
   
   duProcF1SetupRsp();
   freeAperDecodeF1SetupRsp(f1SetRspMsg);
   
   if(fillE2NodeComponentRspInfo(F1, duCfgParam.duId, E2_NODE_COMPONENT_ADD, recvBufLen, recvBuf) !=ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : Failed to add the e2 node in the list");
      return RFAILED;
   }

   if(BuildAndSendE2SetupReq() != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : Failed to build and send E2 setup request ");
      return RFAILED;
   }
   return ret;
}
/*******************************************************************
*
* @brief free GNB DU config update ack
*
* @details
*
*    Function : freeAperDecodeGnbDuAck 
*
*    Functionality: Processes GNB DU config update ack And
*                     added free part for the memory allocated by aper_decoder
*
* @params[in] F1AP_PDU_t ASN decoded F1AP message
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

void freeAperDecodeGnbDuAck(GNBDUConfigurationUpdateAcknowledge_t *gnbDuAck)
{
   uint8_t ieIdx = 0;

   if(gnbDuAck->protocolIEs.list.array)
   {
      for(ieIdx=0; ieIdx < gnbDuAck->protocolIEs.list.count; ieIdx++)
      {
	 if(gnbDuAck->protocolIEs.list.array[ieIdx])
	 {
	    free(gnbDuAck->protocolIEs.list.array[ieIdx]);
	 }
      }
      free(gnbDuAck->protocolIEs.list.array);
   }
}

/*******************************************************************
*
* @brief Building  result of gnb-du config update ack output
*
* @details
*
*    Function : duProcGnbDuCfgUpdAckMsg 
*
*    Functionality: 
*        Building output of gnb-du config update ack 
*
* @params[in] transId
* @return void
*
* ****************************************************************/

uint8_t duProcGnbDuCfgUpdAckMsg(uint8_t transId)
{
   uint8_t  ieIdx=0, arrIdx=0,ret=ROK;
   uint8_t  ueId =0 , ueIdx =0, totalActiveUe = 0;
   uint16_t cellIdx =0, crnti=0;
   uint64_t cellId =0;
   CmLList *f1apPduNode = NULLP;
   ReservedF1apPduInfo *f1apPduInfo =NULLP;
   F1AP_PDU_t *f1apMsgPdu = NULLP;
   GNBDUConfigurationUpdate_t *gnbDuConfigUpdate = NULLP;
   BIT_STRING_t *cellIdentity=NULLP;
   struct Served_Cells_To_Delete_ItemIEs *deleteItemIe=NULLP;
   Served_Cells_To_Delete_List_t *cellsToDelete=NULLP;
   Served_Cells_To_Delete_Item_t *deleteItem=NULLP;

   DU_LOG("\nINFO   -->  DU APP: GNB-DU config update Ack received ");
   f1apPduNode = searchFromReservedF1apPduList(transId);
   f1apPduInfo = (ReservedF1apPduInfo *)f1apPduNode->node;
   f1apMsgPdu = (F1AP_PDU_t *)f1apPduInfo->f1apMsg;

   if(f1apMsgPdu)
   {
      if(f1apMsgPdu->choice.initiatingMessage)
      {
         gnbDuConfigUpdate = &f1apMsgPdu->choice.initiatingMessage->value.choice.GNBDUConfigurationUpdate;
         for(ieIdx=0; ieIdx < gnbDuConfigUpdate->protocolIEs.list.count; ieIdx++)
         {
            switch(gnbDuConfigUpdate->protocolIEs.list.array[ieIdx]->id)
            {
               case ProtocolIE_ID_id_Served_Cells_To_Delete_List:
                  {
                     cellsToDelete = &gnbDuConfigUpdate->protocolIEs.list.array[ieIdx]->value.choice.\
                                     Served_Cells_To_Delete_List;
                     if(cellsToDelete->list.array)
                     {
                        if(cellsToDelete->list.array[arrIdx])
                        {
                           deleteItemIe = (Served_Cells_To_Delete_ItemIEs_t*)cellsToDelete->list.array[arrIdx];
                           deleteItem=&deleteItemIe->value.choice.Served_Cells_To_Delete_Item;
                           if(deleteItem->oldNRCGI.nRCellIdentity.buf)
                           {
                              cellIdentity = &deleteItem->oldNRCGI.nRCellIdentity;
                              bitStringToInt(cellIdentity, &cellId);

                              GET_CELL_IDX(cellId, cellIdx);
                              if(duCb.actvCellLst[cellIdx] != NULLP)
                              {
                                 duCb.actvCellLst[cellId-1]->cellStatus = DELETION_IN_PROGRESS;
                              }
                           }
                        }
                     }

                     if(duCb.actvCellLst[cellIdx] != NULLP)
                     {
                        if(duCb.actvCellLst[cellIdx]->numActvUes == 0)
                        {
                           ret = duSendCellDeletReq(cellId);
                           if(ret == RFAILED)
                           {
                              DU_LOG("\nERROR  --> DU_APP : duProcGnbDuCfgUpdAckMsg(): Failed to send cell delete\
                              request for cellId[%lu]", cellId);
                           }
                        }
                        else
                        {
                           totalActiveUe = duCb.actvCellLst[cellIdx]->numActvUes;
                           while(totalActiveUe)
                           {
                              if(duCb.actvCellLst[cellIdx]->ueCb[ueIdx].ueState != UE_ACTIVE)
                              {
                                 ueIdx++;
                                 continue;
                              }

                              crnti = duCb.actvCellLst[cellIdx]->ueCb[ueIdx].crnti;
                              GET_UE_ID(crnti,ueId);
                              /* Sending Ue Context release request only for maximum supporting UEs */
                              ret = BuildAndSendUeContextReleaseReq(cellId, ueId);
                              if(ret == RFAILED)
                              {
                                 DU_LOG("\nERROR  --> DU_APP : duProcGnbDuCfgUpdAckMsg(): Failed to build and send UE delete\
                                 request for cellId[%lu]", cellId);
                              }
                              ueIdx++;
                              totalActiveUe--;
                           }
                        }
                     }
                     else
                     {
                        DU_LOG("\nERROR  --> DU_APP : duProcGnbDuCfgUpdAckMsg(): CellId [%lu] not found", cellId);
                        ret = RFAILED;
                     }
                     break;
                  }

               default:
                  break;
            }
         }
      }
   }
   
   FreeDUConfigUpdate(f1apMsgPdu);
   deleteFromReservedF1apPduList(f1apPduNode);
   return ret;
}

/*******************************************************************
*
* @brief Processes GNB DU config update ack
*
* @details
*
*    Function : procF1GNBDUCfgUpdAck
*
*    Functionality: added free part for the memory allocated by aper_decoder
*
* @params[in] F1AP_PDU_t *f1apMsg,  MsgLen recvBufLen, char *recvBuf 
* @return void 
*
* ****************************************************************/

uint8_t procF1GNBDUCfgUpdAck(F1AP_PDU_t *f1apMsg, MsgLen recvBufLen, char *recvBuf)
{
   uint8_t ieIdx=0,transId=0;
   GNBDUConfigurationUpdateAcknowledge_t *gnbDuAck = NULLP;

   DU_LOG("\nINFO   -->  F1AP : GNB-DU config update acknowledgment");
   gnbDuAck = &f1apMsg->choice.successfulOutcome->value.choice.GNBDUConfigurationUpdateAcknowledge;

   for(ieIdx=0; ieIdx < gnbDuAck->protocolIEs.list.count; ieIdx++)
   {
      switch(gnbDuAck->protocolIEs.list.array[ieIdx]->id)
      {
         case ProtocolIE_ID_id_TransactionID:
            {
               transId = gnbDuAck->protocolIEs.list.array[ieIdx]->value.choice.TransactionID;
               break;
            }
         case ProtocolIE_ID_id_Cells_to_be_Activated_List:
            {
               break;
            }
         default :
            {
               DU_LOG("\nERROR  -->  F1AP: procF1GNBDUCfgUpdAck(): Invalid IE Received: %ld", \
                     gnbDuAck->protocolIEs.list.array[ieIdx]->id);
               break;
            }
      }
   }
   
   duProcGnbDuCfgUpdAckMsg(transId);
    
#if 0
   /* presently we are not supporting F1 Reset from DU to CU , we are only
    * supporting F1 Reset from CU to DU */

   if(BuildAndSendF1ResetReq() != ROK)
   {
      return RFAILED;
   }
#endif

   freeAperDecodeGnbDuAck(gnbDuAck);
   
   if(fillE2NodeComponentRspInfo(F1,duCfgParam.duId, E2_NODE_COMPONENT_UPDATE, recvBufLen, recvBuf) !=ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : Failed to update the e2 node in the list");
      return RFAILED;
   }

   return ROK;
}
/******************************************************************
*
* @brief free DL RRC Message Transfer allocated by aper_decoder 
*
* @details
*
*    Function : freeAperDecodef1DlRrcMsg 
*
*    Functionality: free DL RRC Message Transfer allocated by aper_decoder 
*
* @params[in] DLRRCMessageTransfer_t *f1DlRrcMsg 
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

void freeAperDecodef1DlRrcMsg(DLRRCMessageTransfer_t *f1DlRrcMsg)
{
   uint8_t ieIdx =0;
   RRCContainer_t *rrcContainer = NULLP;

   if(f1DlRrcMsg->protocolIEs.list.array)
   {
      for(ieIdx=0; ieIdx<f1DlRrcMsg->protocolIEs.list.count; ieIdx++)
      {
	 if(f1DlRrcMsg->protocolIEs.list.array[ieIdx])
	 {
	    switch(f1DlRrcMsg->protocolIEs.list.array[ieIdx]->id)
	    {
	       case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
		  break;
	       case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
		  break;
	       case ProtocolIE_ID_id_SRBID:
		  break;
	       case ProtocolIE_ID_id_RRCContainer:
		  {
		     rrcContainer =&f1DlRrcMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer;
		     free(rrcContainer->buf);
		  }
	       case ProtocolIE_ID_id_ExecuteDuplication:
		  break;
	       case ProtocolIE_ID_id_RRCDeliveryStatusRequest:
		  break;
		  break;
	    }
	    free(f1DlRrcMsg->protocolIEs.list.array[ieIdx]);
	 }
      }
      free(f1DlRrcMsg->protocolIEs.list.array);
   }
}
/******************************************************************
 *
 * @brief Processes DL RRC Message Transfer  sent by CU
 *
 * @details
 *
 *    Function : procF1DlRrcMsgTrans
 *
 *    Functionality: Processes DL RRC Message Transfer sent by CU
 *
 * @params[in] F1AP_PDU_t ASN decoded F1AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procF1DlRrcMsgTrans(F1AP_PDU_t *f1apMsg)
{
   uint8_t  idx, ret;
   DLRRCMessageTransfer_t *f1DlRrcMsg = NULLP;
   F1DlRrcMsg dlMsg;
   memset(&dlMsg, 0, sizeof(F1DlRrcMsg));

   DU_LOG("\nINFO   -->  DU_APP : DL RRC message transfer Recevied");
   f1DlRrcMsg = &f1apMsg->choice.initiatingMessage->value.choice.DLRRCMessageTransfer;

   ret = ROK;

   for(idx=0; idx<f1DlRrcMsg->protocolIEs.list.count; idx++)
   {
      switch(f1DlRrcMsg->protocolIEs.list.array[idx]->id)
      {
	 case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
	    {
	       dlMsg.gnbCuUeF1apId = f1DlRrcMsg->protocolIEs.list.array[idx]->value.choice.GNB_CU_UE_F1AP_ID;
	       break;
	    }
	 case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
	    {
	       dlMsg.gnbDuUeF1apId = f1DlRrcMsg->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID;
	       break;
	    }
	 case ProtocolIE_ID_id_SRBID:
	    {
	       dlMsg.srbId = f1DlRrcMsg->protocolIEs.list.array[idx]->value.choice.SRBID;
	       break;
	    }
	 case ProtocolIE_ID_id_ExecuteDuplication:
	    dlMsg.execDup = true;
	    break;

	 case ProtocolIE_ID_id_RRCContainer:
	    {
	       if(f1DlRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.size > 0)
	       {
             dlMsg.rrcMsgSize = f1DlRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.size;
             DU_ALLOC_SHRABL_BUF(dlMsg.rrcMsgPdu, dlMsg.rrcMsgSize);
             if(dlMsg.rrcMsgPdu)
             {
                memcpy(dlMsg.rrcMsgPdu, f1DlRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf,\
                      dlMsg.rrcMsgSize);
             }
             else
             {
                DU_LOG("\nERROR  -->  DU APP : Memory alloc Failed at RRC Container at procF1DlRrcMsgTrans()");
                return RFAILED;
             }
          }
          else
          {
             DU_LOG("\nERROR  -->  DU_APP : RRC Container Size is invalid:%ld",\
                   f1DlRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.size);
             return RFAILED;
          }
          break;
       }
    case ProtocolIE_ID_id_RRCDeliveryStatusRequest:
       {
          dlMsg.deliveryStatRpt = true;
          break;
       }
    default:
       DU_LOG("\nERROR  -->  DU_APP : Invalid IE received in DL RRC Msg Transfer:%ld",
             f1DlRrcMsg->protocolIEs.list.array[idx]->id);
      }
   }

   ret = duProcDlRrcMsg(&dlMsg);

   freeAperDecodef1DlRrcMsg(f1DlRrcMsg);
   return ret;
}
/*******************************************************************
 *
* @brief Builds the DRB to be Setup Mod list
*
* @details
*
*    Function : 
*
*    Functionality: Constructs the DRB to be Setup Mod list
*
* @params[in] DRBs_SetupMod_List_t *drbSet
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t BuildSrbSetupModList(SRBs_SetupMod_List_t *srbList, DuUeCfg *ueCfg)
{
   uint8_t srbIdx = 0;
   struct SRBs_SetupMod_ItemIEs *srbItemIe;

   srbList->list.count = ueCfg->numRlcLcs;
   srbList->list.size = srbList->list.count * sizeof(SRBs_SetupMod_ItemIEs_t *);

   DU_ALLOC(srbList->list.array, srbList->list.size);
   if(srbList->list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  DU APP : Memory allocation failed in BuildSrbSetupModList");
      return RFAILED;
   }

   for(srbIdx = 0; srbIdx < srbList->list.count; srbIdx++)
   {
      DU_ALLOC(srbList->list.array[srbIdx], sizeof(SRBs_SetupMod_ItemIEs_t));
      if(srbList->list.array[srbIdx] == NULLP)
      {
         DU_LOG("\nERROR  -->  DU APP : Memory allocation failed in BuildSrbSetupModList");
         return RFAILED;
      }
   } 

   for(srbIdx=0; srbIdx < srbList->list.count; srbIdx++)
   {
      srbItemIe = (struct SRBs_SetupMod_ItemIEs *)srbList->list.array[srbIdx];
      srbItemIe->id = ProtocolIE_ID_id_SRBs_SetupMod_Item;
      srbItemIe->criticality = Criticality_reject;
      srbItemIe->value.present = SRBs_SetupMod_ItemIEs__value_PR_SRBs_SetupMod_Item;
      srbItemIe->value.choice.SRBs_SetupMod_Item.sRBID = ueCfg->rlcLcCfg[srbIdx].rlcBearerCfg.rbId;
      srbItemIe->value.choice.SRBs_SetupMod_Item.lCID = ueCfg->rlcLcCfg[srbIdx].rlcBearerCfg.lcId;
   }
   return ROK;
}

/*******************************************************************
 *
* @brief Builds the DRB to be Setup Mod list
*
* @details
*
*    Function : 
*
*    Functionality: Constructs the DRB to be Setup Mod list
*
* @params[in] DRBs_SetupMod_List_t *drbSet
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t BuildDrbSetupModList(DRBs_SetupMod_List_t *drbSet , DuUeCfg *ueCfg)
{
   uint8_t arrIdx =0;
   uint8_t drbCnt =0;
   struct DRBs_SetupMod_ItemIEs *drbItemIe;

   drbCnt = ueCfg->numDrbSetupMod;

   drbSet->list.count = drbCnt;
   drbSet->list.size = drbCnt * sizeof(DRBs_SetupMod_ItemIEs_t *);
   DU_ALLOC(drbSet->list.array, drbSet->list.size);
   if(drbSet->list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDrbToBeSetupModList");
      return  RFAILED;
   }
   for(arrIdx=0; arrIdx<drbCnt; arrIdx++)
   {
      DU_ALLOC(drbSet->list.array[arrIdx], sizeof(DRBs_SetupMod_ItemIEs_t));
      if(drbSet->list.array[arrIdx] == NULLP)
      {
	      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDrbToBeSetupModList");
	      return  RFAILED;
      }

      drbItemIe = (struct DRBs_SetupMod_ItemIEs *)drbSet->list.array[arrIdx];
      drbItemIe->id = ProtocolIE_ID_id_DRBs_SetupMod_Item;
      drbItemIe->criticality = Criticality_reject;
      drbItemIe->value.present = DRBs_SetupMod_ItemIEs__value_PR_DRBs_SetupMod_Item;
      drbItemIe->value.choice.DRBs_SetupMod_Item.dRBID = ueCfg->upTnlInfo[arrIdx].drbId;
      if(fillDlTnlSetupList(&drbItemIe->value.choice.DRBs_SetupMod_Item.dLUPTNLInformation_ToBeSetup_List,\
      &ueCfg->upTnlInfo[arrIdx])!= ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Failed to fill DlTnl SetupList in BuildDrbToBeSetupModList");
         return RFAILED;
      }
      
   }

   return ROK;
}
/*******************************************************************
* @brief Free the memory allocated for DRB setup List
*
* @details
*
*    Function : FreeDrbSetupModList 
*
*    Functionality:
*       Free the memory allocated for DRB setup list
*
* @params[in] DRBs_Setup_List_t *
* @return void
*
* ****************************************************************/
void FreeDrbSetupModList(DRBs_SetupMod_List_t *drbSetupList)
{
   uint8_t arrIdx = 0;
   DRBs_SetupMod_ItemIEs_t *drbItemIe = NULLP;

   for(arrIdx = 0; arrIdx < drbSetupList->list.count; arrIdx++)
   {
      drbItemIe = ((DRBs_SetupMod_ItemIEs_t *)drbSetupList->list.array[arrIdx]);
      freeDlTnlInfo(&drbItemIe->value.choice.DRBs_SetupMod_Item.dLUPTNLInformation_ToBeSetup_List);
      DU_FREE(drbSetupList->list.array[arrIdx], sizeof(DRBs_SetupMod_ItemIEs_t));
   }
   DU_FREE(drbSetupList->list.array, drbSetupList->list.size);
}

/*******************************************************************
*
* @brief Builds the DRB to be Mod list
*
* @details
*
*    Function : 
*
*    Functionality: Constructs the DRB to be Mod list
*
* @params[in] DRBs_Modified_List_t *drbModList
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t BuildDrbModList(DRBs_Modified_List_t *drbModList, DuUeCfg *ueCfg)
{
   uint8_t arrIdx =0, drbIdx = 0;
   uint8_t drbCnt =0;
   struct DRBs_Modified_ItemIEs *drbItemIe;

   drbCnt = ueCfg->numDrbModified;

   drbModList->list.count = drbCnt;
   drbModList->list.size = drbCnt * sizeof(DRBs_Modified_ItemIEs_t *);
   DU_ALLOC(drbModList->list.array, drbModList->list.size);
   if(drbModList->list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDrbModList()");
      return  RFAILED;
   }

   drbIdx = 0;
   for(arrIdx = 0; arrIdx < ueCfg->numMacLcs; arrIdx++)
   {
      if(ueCfg->macLcCfg[arrIdx].configType == CONFIG_MOD)
      {
         DU_ALLOC(drbModList->list.array[drbIdx], sizeof(DRBs_Modified_ItemIEs_t));
         if(drbModList->list.array[drbIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDrbModList");
            return  RFAILED;
         }

         drbItemIe = (struct DRBs_Modified_ItemIEs *)drbModList->list.array[drbIdx];
         drbItemIe->id = ProtocolIE_ID_id_DRBs_Modified_Item;
         drbItemIe->criticality = Criticality_reject;
         drbItemIe->value.present = DRBs_Modified_ItemIEs__value_PR_DRBs_Modified_Item;
         drbItemIe->value.choice.DRBs_Modified_Item.dRBID = ueCfg->upTnlInfo[arrIdx].drbId;
         if(fillDlTnlSetupList(&drbItemIe->value.choice.DRBs_Modified_Item.dLUPTNLInformation_ToBeSetup_List,\
                  &ueCfg->upTnlInfo[arrIdx])!= ROK)
         {
            DU_LOG("\nERROR  -->  F1AP : Failed to fill DlTnl SetupList in BuildDrbModList");
            return RFAILED;
         }
         drbIdx++;
      } 
   }

   return ROK;
}

/*******************************************************************
* @brief Free the memory allocated for DRB Mod List
*
* @details
*
*    Function : FreeDrbModList 
*
*    Functionality:
*       Free the memory allocated for DRB modified list
*
* @params[in] DRBs_Modified_List_t *
* @return void
*
* ****************************************************************/
void FreeDrbModList(DRBs_Modified_List_t *drbModList)
{
   uint8_t arrIdx = 0;
   DRBs_Modified_ItemIEs_t *drbItemIe = NULLP;

   for(arrIdx = 0; arrIdx < drbModList->list.count; arrIdx++)
   {
      drbItemIe = ((DRBs_Modified_ItemIEs_t *)drbModList->list.array[arrIdx]);
      freeDlTnlInfo(&drbItemIe->value.choice.DRBs_Modified_Item.dLUPTNLInformation_ToBeSetup_List);
      DU_FREE(drbModList->list.array[arrIdx], sizeof(DRBs_Modified_ItemIEs_t));
   }
   DU_FREE(drbModList->list.array, drbModList->list.size);
}

/*******************************************************************
* @brief Free the memory allocated for SRB setup List
*
* @details
*
*    Function : FreeSrbSetupModList 
*
*    Functionality:
*       Free the memory allocated for SRB setup list
*
* @params[in] SRBs_Setup_List_t *
* @return void
*
* ****************************************************************/
void FreeSrbSetupModList(SRBs_SetupMod_List_t *srbSetupList)
{
   uint8_t srbIdx = 0;
   
   for(srbIdx = 0; srbIdx < srbSetupList->list.count; srbIdx++)
      DU_FREE(srbSetupList->list.array[srbIdx], sizeof(SRBs_SetupMod_ItemIEs_t));
   DU_FREE(srbSetupList->list.array, srbSetupList->list.size);
}

/*******************************************************************
* @brief Free the memory allocated for UE Context Mod Response
*
* @details
*
*    Function : FreeUeContextModResp 
*
*    Functionality:
*       Free the memory allocated for UE Context Mod Response
*
* @params[in] F1AP_PDU_t *f1apMsg
* @return void
*
* ****************************************************************/

void FreeUeContextModResp(F1AP_PDU_t *f1apMsg)
{
   uint8_t ieIdx;
   UEContextModificationResponse_t *ueContextModifyRes = NULLP;
   if(f1apMsg)
   {
      if(f1apMsg->choice.successfulOutcome)
      {
         ueContextModifyRes =&f1apMsg->choice.successfulOutcome->value.choice.UEContextModificationResponse;
         if(ueContextModifyRes->protocolIEs.list.array)
         {
            for(ieIdx=0 ; ieIdx<ueContextModifyRes->protocolIEs.list.count; ieIdx++)
            {
               if(ueContextModifyRes->protocolIEs.list.array[ieIdx])
               {
                  switch(ueContextModifyRes->protocolIEs.list.array[ieIdx]->id)
                  {
                     case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
                        break;
                     case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
                        break;
                     case ProtocolIE_ID_id_DUtoCURRCInformation:
                        {
                           DU_FREE(ueContextModifyRes->protocolIEs.list.array[ieIdx]->value.choice.DUtoCURRCInformation.\
                              cellGroupConfig.buf, ueContextModifyRes->protocolIEs.list.array[ieIdx]->value.choice.\
                              DUtoCURRCInformation.cellGroupConfig.size);
                           break;
                        }
                     case ProtocolIE_ID_id_DRBs_SetupMod_List:
                        {
                            FreeDrbSetupModList(&(ueContextModifyRes->protocolIEs.list.array[ieIdx]->\
                            value.choice.DRBs_SetupMod_List));
                            break;
                        }
                     case ProtocolIE_ID_id_DRBs_Modified_List:
                        {
                            FreeDrbModList(&(ueContextModifyRes->protocolIEs.list.array[ieIdx]->\
                            value.choice.DRBs_Modified_List));
                            break;
                        }
                     case ProtocolIE_ID_id_SRBs_SetupMod_List:
                        {
                           FreeSrbSetupModList(&(ueContextModifyRes->protocolIEs.list.array[ieIdx]->value.choice.\
                              SRBs_SetupMod_List));
                           break; 
                        }
                  }
                  DU_FREE(ueContextModifyRes->protocolIEs.list.array[ieIdx], sizeof(UEContextModificationResponse_t));
               }

            }
            DU_FREE(ueContextModifyRes->protocolIEs.list.array, ueContextModifyRes->protocolIEs.list.size);
         }
         DU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      }
      DU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
   }
}

/*****************************************************************i
*
* @brief Creating the ue context modifcation response and sending
*
* @details
*
*    Function : BuildAndSendUeContextModRsp 
*
*    Functionality:
*         - Creating the ue context modifcation response 
*
* @params[in] uint8_t cellId,uint8_t ueId
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t BuildAndSendUeContextModRsp(DuUeCb *ueCb)
{
   uint8_t   ieIdx = 0, tnlIdx = 0, rbIdx = 0;
   uint8_t   elementCnt = 0;
   uint8_t   ret = RFAILED;
   F1AP_PDU_t *f1apMsg = NULLP;
   asn_enc_rval_t  encRetVal;
   UEContextModificationResponse_t *ueContextModifyRes = NULLP;

   DU_LOG("\nINFO  -->  F1AP : Building UE context modification response\n");

   while(true)
   {
      DU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation for F1AP-PDU failed Ue context modification");
         break;
      }

      f1apMsg->present =  F1AP_PDU_PR_successfulOutcome;

      DU_ALLOC(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      if(f1apMsg->choice.successfulOutcome == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation for F1AP-PDU failed Ue context modification");
         break;
      }
      f1apMsg->choice.successfulOutcome->procedureCode = ProcedureCode_id_UEContextModification;
      f1apMsg->choice.successfulOutcome->criticality = Criticality_reject;
      f1apMsg->choice.successfulOutcome->value.present = SuccessfulOutcome__value_PR_UEContextModificationResponse;

      ueContextModifyRes =&f1apMsg->choice.successfulOutcome->value.choice.UEContextModificationResponse;
  
      if(ueCb->f1UeDb->actionType == UE_CTXT_MOD)
      {
         elementCnt = 2;
         if(ueCb->f1UeDb->duUeCfg.numDrbSetupMod)
            elementCnt++;
         if(ueCb->f1UeDb->duUeCfg.numDrbModified)
            elementCnt++; 
      }
      if(ueCb->f1UeDb->actionType == UE_CTXT_CFG_QUERY)
      {
         elementCnt = 5;
      }
      if(ueCb->f1UeDb->actionType == UE_CTXT_RRC_RECFG_COMPLETE)
         elementCnt = 2;
      ueContextModifyRes->protocolIEs.list.count = elementCnt;
      ueContextModifyRes->protocolIEs.list.size = elementCnt*sizeof(UEContextModificationResponse_t*);

      /* Initialize the UE context modification members */
      DU_ALLOC(ueContextModifyRes->protocolIEs.list.array, ueContextModifyRes->protocolIEs.list.size);
      if(ueContextModifyRes->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation for UE context modifcation Request failed");
         break;
      }

      for(ieIdx=0 ; ieIdx<elementCnt; ieIdx++)
      {
         DU_ALLOC(ueContextModifyRes->protocolIEs.list.array[ieIdx], sizeof(UEContextModificationResponse_t));
         if(ueContextModifyRes->protocolIEs.list.array[ieIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  F1AP : Memory allocation for UE context modifcation Request failed");
            break;
         }
      }

      ieIdx=0;
      ueContextModifyRes->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;
      ueContextModifyRes->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      ueContextModifyRes->protocolIEs.list.array[ieIdx]->value.present =\
      UEContextModificationResponseIEs__value_PR_GNB_CU_UE_F1AP_ID;
      ueContextModifyRes->protocolIEs.list.array[ieIdx]->value.choice.GNB_CU_UE_F1AP_ID = ueCb->gnbCuUeF1apId;

      ieIdx++;
      ueContextModifyRes->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
      ueContextModifyRes->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      ueContextModifyRes->protocolIEs.list.array[ieIdx]->value.present=\
      UEContextModificationResponseIEs__value_PR_GNB_DU_UE_F1AP_ID;
      ueContextModifyRes->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_UE_F1AP_ID = ueCb->gnbDuUeF1apId;

      if(ueCb->f1UeDb->actionType == UE_CTXT_CFG_QUERY)
      {
         ieIdx++;
         ueContextModifyRes->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_DUtoCURRCInformation;
         ueContextModifyRes->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
         ueContextModifyRes->protocolIEs.list.array[ieIdx]->value.present = \
         UEContextModificationResponseIEs__value_PR_DUtoCURRCInformation;
         BuildCellGroupConfigRrc(ueCb, &ueContextModifyRes->protocolIEs.list.array[ieIdx]->value.choice.DUtoCURRCInformation.cellGroupConfig);
      }

      if((ueCb->f1UeDb->actionType == UE_CTXT_CFG_QUERY) || \
           ((ueCb->f1UeDb->actionType == UE_CTXT_MOD) && (ueCb->f1UeDb->duUeCfg.numDrbSetupMod)))
      { 
         ieIdx++;
         ueContextModifyRes->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_DRBs_SetupMod_List;
         ueContextModifyRes->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
         ueContextModifyRes->protocolIEs.list.array[ieIdx]->value.present =\
                                                                           UEContextModificationResponseIEs__value_PR_DRBs_SetupMod_List;
         if(ueCb->f1UeDb->actionType == UE_CTXT_CFG_QUERY)
         {
            for(tnlIdx = 0; tnlIdx < duCb.numTeId; tnlIdx++)
            {
               if(duCb.upTnlCfg[tnlIdx]->ueId == ueCb->gnbDuUeF1apId)
               {
                  memcpy(&ueCb->f1UeDb->duUeCfg.upTnlInfo[ueCb->f1UeDb->duUeCfg.numDrbSetupMod++], duCb.upTnlCfg[tnlIdx], sizeof(UpTnlCfg));
               }
            }
         }
         ret = BuildDrbSetupModList(&(ueContextModifyRes->protocolIEs.list.array[ieIdx]->\
                  value.choice.DRBs_SetupMod_List) , &ueCb->f1UeDb->duUeCfg);
         if(ret != ROK)
         {
            DU_LOG( "\nERROR  -->  F1AP : Failed to build DRB setupmod List ");
            break;
         }
      }

      if((ueCb->f1UeDb->actionType == UE_CTXT_MOD) && (ueCb->f1UeDb->duUeCfg.numDrbModified))
      { 
         ieIdx++;
         ueContextModifyRes->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_DRBs_Modified_List;
         ueContextModifyRes->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
         ueContextModifyRes->protocolIEs.list.array[ieIdx]->value.present =\
                                                                    UEContextModificationResponseIEs__value_PR_DRBs_Modified_List;
         ret = BuildDrbModList(&(ueContextModifyRes->protocolIEs.list.array[ieIdx]->\
                                 value.choice.DRBs_Modified_List) , &ueCb->f1UeDb->duUeCfg);
         if(ret != ROK)
         {
            DU_LOG( "\nERROR  -->  F1AP : Failed to build DRB Modified List ");
            break;
         }
      }

      if(ueCb->f1UeDb->actionType == UE_CTXT_CFG_QUERY)
      {
         ieIdx++;
         ueContextModifyRes->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_SRBs_SetupMod_List;
         ueContextModifyRes->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
         ueContextModifyRes->protocolIEs.list.array[ieIdx]->value.present = \
                                                                            UEContextModificationResponseIEs__value_PR_SRBs_SetupMod_List;
         for(rbIdx = 0; rbIdx < ueCb->duRlcUeCfg.numLcs; rbIdx++)
         {
            if(ueCb->duRlcUeCfg.rlcLcCfg[rbIdx].rlcBearerCfg.rbType == RB_TYPE_SRB)
            {
               memcpy(&ueCb->f1UeDb->duUeCfg.rlcLcCfg[ueCb->f1UeDb->duUeCfg.numRlcLcs++], &ueCb->duRlcUeCfg.rlcLcCfg[rbIdx],\
                     sizeof(DuRlcBearerCfg));
            }
         }
         ret = BuildSrbSetupModList(&ueContextModifyRes->protocolIEs.list.array[ieIdx]->value.choice.SRBs_SetupMod_List, \
               &ueCb->f1UeDb->duUeCfg);
         if(ret != ROK)
         {
            DU_LOG( "\nERROR  -->  F1AP : Failed to build SRB setupmod List ");
            break;
         }
      }

      freeF1UeDb(ueCb->f1UeDb);
      ueCb->f1UeDb = NULLP;

      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the F1SetupRequest type as APER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,encBuf);

      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG( "\nERROR  -->  F1AP : Could not encode UE Modification Response structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         ret = RFAILED;
         break;
      }
      else
      {
         DU_LOG("\nDEBUG   -->  F1AP : Created APER encoded buffer for UE Context Modification Response\n");
#ifdef DEBUG_ASN_PRINT
         for(int i=0; i< encBufSize; i++)
         {
            printf("%x",encBuf[i]);
         }
#endif
      }

      /* Sending  msg  */
      if(sendF1APMsg() != ROK && (ret == ROK))
      {
         DU_LOG("\nERROR  -->  F1AP : Sending UE Modification Res Failed");
         ret = RFAILED;
         break;
      }

      ret = ROK;
      break;
   }

   FreeUeContextModResp(f1apMsg);
   return ret;
}

/*******************************************************************
 *
 * @brief Deallocating the memory allocated by the aper decoder
 *          for QOSInfo
 *
 * @details
 *
 *    Function : freeAperDecodeQosInfo
 *
 *    Functionality:  Deallocating the memory allocated for QOSInfo
 *
 * @params[in] QoSFlowLevelQoSParameters_t *drbQos
 *
 * @return void
 *
 * ****************************************************************/

void freeAperDecodeQosInfo(QoSFlowLevelQoSParameters_t *drbQos)
{
   if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI != NULLP)
   {
      if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow!=NULLP)
      {
         if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume!=NULLP)
         {
            free(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume);
         }
         free(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow);
      }
      free(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI);
   }
}
/*******************************************************************
 *
 * @brief Deallocating the memory allocated by the aper decoder
 *          for UlTnlInfoforDrb
 *
 * @details
 *
 *    Function : freeAperDecodeUlTnlInfoforDrbSetupMod 
 *
 *    Functionality:  Deallocating memory allocated for UlTnlInfoforDrb
 *
 * @params[in] ULUPTNLInformation_ToBeSetup_List_t *ulInfo
 *
 * @return void
 *
 * ****************************************************************/
void freeAperDecodeUlTnlInfoforDrbSetupMod(ULUPTNLInformation_ToBeSetup_List_t *ulInfo)
{
   uint8_t arrIdx =0;

   if(ulInfo->list.array)
   {
      for(arrIdx=0; arrIdx<ulInfo->list.count ; arrIdx++)
      {
         if(ulInfo->list.array[arrIdx])
         {
            if(ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel )
            {
               if(ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf)
               {
                  if(ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf)
                  {
                     free(ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->\
                           gTP_TEID.buf);
                  }
                  free(ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->\
                        transportLayerAddress.buf);
               }
               free(ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel);
            }
            free(ulInfo->list.array[arrIdx]);
         }
      }
      free(ulInfo->list.array);
   }
}

/*******************************************************************
 *
 * @brief Deallocating the memory allocated by the aper decoder
 *          for DrbSetupModItem  
 *
 * @details
 *
 *    Function : freeAperDecodeDrbSetupModItem 
 *
 *    Functionality:  Deallocating memory allocated for DrbSetupModItem
 *
 * @params[in] DRBs_ToBeSetupMod_Item_t *drbItem
 *
 * @return void
 *
 * ****************************************************************/

void freeAperDecodeDrbSetupModItem(DRBs_ToBeSetupMod_Item_t *drbItem)
{
   uint8_t arrIdx =0;
   SNSSAI_t *snssai =NULLP;
   Flows_Mapped_To_DRB_List_t *flowMap = NULLP;

   drbItem->qoSInformation.present = QoSInformation_PR_choice_extension;
   switch(drbItem->qoSInformation.present)
   {
      case QoSInformation_PR_NOTHING:
         break;
      case QoSInformation_PR_eUTRANQoS:
         {
            if(drbItem->qoSInformation.choice.eUTRANQoS)
            {
               free(drbItem->qoSInformation.choice.eUTRANQoS);
            }
            break;
         }
      case QoSInformation_PR_choice_extension:
         {
            if(drbItem->qoSInformation.choice.choice_extension)
            {
               freeAperDecodeQosInfo(&drbItem->qoSInformation.choice.choice_extension->value.choice.\
                     DRB_Information.dRB_QoS);
               snssai = &drbItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI;
               if(snssai->sST.buf)
               {
                  free(snssai->sST.buf);
               }
               if(snssai->sD)
               {
                  if(snssai->sD->buf)
                  {
                     free(snssai->sD->buf);
                  }
                  free(snssai->sD);
               }

               flowMap = &drbItem->qoSInformation.choice.choice_extension->value.choice.\
                         DRB_Information.flows_Mapped_To_DRB_List;
               if(flowMap->list.array)
               {
                  for(arrIdx=0; arrIdx<flowMap->list.count; arrIdx++)
                  {
                     if(flowMap->list.array[arrIdx] )
                     {
                        freeAperDecodeQosInfo(&flowMap->list.array[arrIdx]->qoSFlowLevelQoSParameters);
                        free(flowMap->list.array[arrIdx]);
                     }
                  }
                  free(flowMap->list.array);
               }

               free(drbItem->qoSInformation.choice.choice_extension);
            }
            break;
         }

   }
   freeAperDecodeUlTnlInfoforDrbSetupMod(&drbItem->uLUPTNLInformation_ToBeSetup_List);
   if(drbItem->uLConfiguration)
   {
      free(drbItem->uLConfiguration);
   }
}

/*******************************************************************
 *
 * @brief Deallocating the memory allocated by the aper decoder
 *          for DrbToBeSetupModList
 *
 * @details
 *
 *    Function : freeAperDecodeDrbToBeSetupModList
 *
 *    Functionality:  Deallocating memory allocated for DrbToBeSetupModList
 *
 * @params[in] DRBs_ToBeSetupMod_List_t *drbSet
 *
 * @return void
 *
 * ****************************************************************/

void freeAperDecodeDrbToBeSetupModList(DRBs_ToBeSetupMod_List_t *drbSet)
{
   uint8_t arrIdx =0;
   struct DRBs_ToBeSetupMod_ItemIEs *drbItemIe;

   if(drbSet->list.array)
   {
      for(arrIdx=0; arrIdx<drbSet->list.count ; arrIdx++)
      {
         if(drbSet->list.array[arrIdx] != NULLP)
         {
            if(arrIdx == 0)
            {
               drbItemIe = (DRBs_ToBeSetupMod_ItemIEs_t *)drbSet->list.array[arrIdx];
               freeAperDecodeDrbSetupModItem(&(drbItemIe->value.choice.DRBs_ToBeSetupMod_Item));
            }
            free(drbSet->list.array[arrIdx]);
         }
      }
      free(drbSet->list.array);
   }

}

/*******************************************************************
 *
 * @brief Deallocating the memory allocated by the aper decoder
 *          for DrbSetupModItem  
 *
 * @details
 *
 *    Function : freeAperDecodeDrbModifiedItem 
 *
 *    Functionality:  Deallocating memory allocated for DrbModifedItem
 *
 * @params[in] DRBs_ToBeModified_Item_t *drbItem
 *
 * @return void
 *
 * ****************************************************************/

void freeAperDecodeDrbModifiedItem(DRBs_ToBeModified_Item_t *drbItem)
{
   uint8_t arrIdx =0;
   SNSSAI_t *snssai =NULLP;
   Flows_Mapped_To_DRB_List_t *flowMap = NULLP;

   if(drbItem->qoSInformation != NULLP)
   {
      drbItem->qoSInformation->present = QoSInformation_PR_choice_extension;
      switch(drbItem->qoSInformation->present)
      {
         case QoSInformation_PR_NOTHING:
            break;
         case QoSInformation_PR_eUTRANQoS:
            {
               if(drbItem->qoSInformation->choice.eUTRANQoS)
               {
                  free(drbItem->qoSInformation->choice.eUTRANQoS);
               }
               break;
            }
         case QoSInformation_PR_choice_extension:
            {
               if(drbItem->qoSInformation->choice.choice_extension)
               {
                  freeAperDecodeQosInfo(&drbItem->qoSInformation->choice.choice_extension->value.choice.\
                        DRB_Information.dRB_QoS);
                  snssai = &drbItem->qoSInformation->choice.choice_extension->value.choice.DRB_Information.sNSSAI;
                  if(snssai->sST.buf)
                  {
                     free(snssai->sST.buf);
                  }
                  if(snssai->sD)
                  {
                     if(snssai->sD->buf)
                     {
                        free(snssai->sD->buf);
                     }
                     free(snssai->sD);
                  }

                  flowMap = &drbItem->qoSInformation->choice.choice_extension->value.choice.\
                            DRB_Information.flows_Mapped_To_DRB_List;
                  if(flowMap->list.array)
                  {
                     for(arrIdx=0; arrIdx<flowMap->list.count; arrIdx++)
                     {
                        if(flowMap->list.array[arrIdx] )
                        {
                           freeAperDecodeQosInfo(&flowMap->list.array[arrIdx]->qoSFlowLevelQoSParameters);
                           free(flowMap->list.array[arrIdx]);
                        }
                     }
                     free(flowMap->list.array);
                  }

                  free(drbItem->qoSInformation->choice.choice_extension);
               }
               break;
            }
      }
      free(drbItem->qoSInformation);
   }
   freeAperDecodeUlTnlInfoforDrbSetupMod(&drbItem->uLUPTNLInformation_ToBeSetup_List);
   if(drbItem->uLConfiguration)
   {
      free(drbItem->uLConfiguration);
   }
}

/*******************************************************************
 *
 * @brief Deallocating the memory allocated by the aper decoder
 *          for DrbToBeSetupModList
 *
 * @details
 *
 *    Function : freeAperDecodeDrbToBeModifiedList
 *
 *    Functionality:  Deallocating memory allocated for DrbToBeModifiedList
 *
 * @params[in] DRBs_ToBeModified_List_t *drbSet
 *
 * @return void
 *
 * ****************************************************************/

void freeAperDecodeDrbToBeModifiedList(DRBs_ToBeModified_List_t *drbSet)
{
   uint8_t arrIdx =0;
   struct DRBs_ToBeModified_ItemIEs *drbItemIe;

   if(drbSet->list.array)
   {
      for(arrIdx=0; arrIdx<drbSet->list.count ; arrIdx++)
      {
         if(drbSet->list.array[arrIdx] != NULLP)
         {
            if(arrIdx == 0)
            {
               drbItemIe = (DRBs_ToBeModified_ItemIEs_t *)drbSet->list.array[arrIdx];
               freeAperDecodeDrbModifiedItem(&(drbItemIe->value.choice.DRBs_ToBeModified_Item));
            }
            free(drbSet->list.array[arrIdx]);
         }
      }
      free(drbSet->list.array);
   }

}

/*******************************************************************
 *
 * @brief Deallocating the memory allocated by the aper decoder
 *          for DrbToBeSetupModList
 *
 * @details
 *
 *    Function : freeAperDecodeDrbToBeReleasedList
 *
 *    Functionality:  Deallocating memory allocated for DrbToBeReleasedList
 *
 * @params[in] DRBs_ToBeReleased_List_t *drbSet
 *
 * @return void
 *
 * ****************************************************************/

void freeAperDecodeDrbToBeReleasedList(DRBs_ToBeReleased_List_t *drbSet)
{
   uint8_t arrIdx =0;

   if(drbSet->list.array)
   {
      for(arrIdx=0; arrIdx<drbSet->list.count ; arrIdx++)
      {
         if(drbSet->list.array[arrIdx] != NULLP)
         {
            free(drbSet->list.array[arrIdx]);
         }
      }
      free(drbSet->list.array);
   }

}
/*******************************************************************
 *
 * @brief Deallocating the memory allocated by the aper decoder
 *          for UeContextModificationReqMsg
 *
 * @details
 *
 *    Function : freeAperDecodeUeContextModificationReqMsg
 *
 *    Functionality:  Deallocating memory allocated for
 *                  UeContextModificationReqMsg
 *
 * @params[in] UEContextModificationRequest_t *UeContextModifyReq
 *
 * @return void
 *
 * ****************************************************************/
void freeAperDecodeUeContextModificationReqMsg(UEContextModificationRequest_t *ueContextModifyReq )
{
   uint8_t arrIdx, ieId;

   if(ueContextModifyReq->protocolIEs.list.array)
   {
      for( arrIdx = 0 ; arrIdx<ueContextModifyReq->protocolIEs.list.count ; arrIdx++)
      {
         if(ueContextModifyReq->protocolIEs.list.array[arrIdx])
         {
            ieId = ueContextModifyReq->protocolIEs.list.array[arrIdx]->id;
            switch(ieId)
            {
               case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
                  break;
               case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
                  break;
               case ProtocolIE_ID_id_DRBs_ToBeSetupMod_List:
                  {
                     freeAperDecodeDrbToBeSetupModList(&ueContextModifyReq->protocolIEs.list.array[arrIdx]->\
                           value.choice.DRBs_ToBeSetupMod_List);
                     break;
                  }
               case ProtocolIE_ID_id_DRBs_ToBeModified_List:
                  {
                     freeAperDecodeDrbToBeModifiedList(&ueContextModifyReq->protocolIEs.list.array[arrIdx]->\
                           value.choice.DRBs_ToBeModified_List);
                     break;
                  }
               case ProtocolIE_ID_id_DRBs_ToBeReleased_List:
                  {
                     freeAperDecodeDrbToBeReleasedList(&ueContextModifyReq->protocolIEs.list.array[arrIdx]->\
                           value.choice.DRBs_ToBeReleased_List);
                     break;
                  }
               case ProtocolIE_ID_id_TransmissionActionIndicator:
                  break;
               case ProtocolIE_ID_id_RRCContainer:
                  {
                     free(ueContextModifyReq->protocolIEs.list.array[arrIdx]->value.choice.RRCContainer.buf);
                  }
            }
            free(ueContextModifyReq->protocolIEs.list.array[arrIdx]);
         }
      }
      free(ueContextModifyReq->protocolIEs.list.array);
   }
}
/*******************************************************************
 *
 * @brief processing the F1 UeContextModificationReq
 *
 * @details
 *
 *    Function : procF1UeContextModificationReq
 *
 *    Functionality:  processing the F1 UeContextModificationReq
 *
 * @params[in] F1AP_PDU_t *f1apMsg
 *
 * @return
 * ****************************************************************/
uint8_t procF1UeContextModificationReq(F1AP_PDU_t *f1apMsg)
{
   UEContextModificationRequest_t *ueContextModifyReq = NULLP;
   uint8_t  ret = ROK, ieIdx = 0, cellIdx=0;
   DuUeCb   *duUeCb = NULLP;
   DRBs_ToBeSetupMod_List_t *drbSetupModCfg = NULLP;
   DRBs_ToBeModified_List_t *drbModifiedCfg = NULLP;
   DRBs_ToBeReleased_List_t *drbToRelease = NULLP;
   uint32_t gnbCuUeF1apId, gnbDuUeF1apId;

   ueContextModifyReq = &f1apMsg->choice.initiatingMessage->value.choice.UEContextModificationRequest;
   for(ieIdx=0; (ieIdx < ueContextModifyReq->protocolIEs.list.count && ret == ROK); ieIdx++)
   {
      switch(ueContextModifyReq->protocolIEs.list.array[ieIdx]->id)
      {
         case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
            {
               gnbCuUeF1apId = ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_CU_UE_F1AP_ID;
               break;
            }
         case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
            {
               gnbDuUeF1apId = ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_UE_F1AP_ID;
               for(cellIdx = 0; cellIdx < MAX_NUM_CELL; cellIdx++)
               {
                  if(duCb.actvCellLst[cellIdx])
                  {
                     if((duCb.actvCellLst[cellIdx]->ueCb[gnbDuUeF1apId-1].gnbDuUeF1apId == gnbDuUeF1apId)&&\
                           (duCb.actvCellLst[cellIdx]->ueCb[gnbDuUeF1apId-1].gnbCuUeF1apId == gnbCuUeF1apId))
                     {
                        duUeCb = &duCb.actvCellLst[cellIdx]->ueCb[gnbDuUeF1apId-1];
                        if(duUeCb->f1UeDb == NULLP)
                        {
                           DU_ALLOC(duUeCb->f1UeDb, sizeof(F1UeContextSetupDb));
                           duUeCb->f1UeDb->cellIdx = cellIdx;
                           duUeCb->f1UeDb->actionType = UE_CTXT_MOD;
                        }
                        break;
                     }
                  }
               }
               if(duUeCb == NULLP)
               {
                  DU_LOG("\nERROR  -->  DU APP : wrong values of gnbCuUeF1apId and gnbDuUeF1apId ");
                  ret = RFAILED;
               }
               break;
            }

         case ProtocolIE_ID_id_RRCContainer:
            {
               /* Filling Dl RRC Msg Info */
               DU_ALLOC_SHRABL_BUF(duUeCb->f1UeDb->dlRrcMsg, sizeof(F1DlRrcMsg));
               if(!duUeCb->f1UeDb->dlRrcMsg)
               {
                  DU_LOG("\nERROR  -->  DU APP : procF1UeContextReleaseCommand(): \
                        Memory allocation failed ");
                  ret = RFAILED;
               }
               else
               {
                  duUeCb->f1UeDb->dlRrcMsgPres = true;
                  memset(duUeCb->f1UeDb->dlRrcMsg, 0, sizeof(F1DlRrcMsg));
                  ret = extractDlRrcMsg(gnbDuUeF1apId, gnbCuUeF1apId, duUeCb->f1UeDb->dlRrcMsg,\
                        &ueContextModifyReq->protocolIEs.list.array[ieIdx]->\
                        value.choice.RRCContainer);
               }

               break;
            }

         case ProtocolIE_ID_id_DRBs_ToBeSetupMod_List:
         case ProtocolIE_ID_id_DRBs_ToBeModified_List:
         case ProtocolIE_ID_id_DRBs_ToBeReleased_List:
            {
               if(duUeCb->f1UeDb)
               {
                  /*DRBs to be Added*/
                  if(ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.present ==\
                        UEContextModificationRequestIEs__value_PR_DRBs_ToBeSetupMod_List)
                  {
                     drbSetupModCfg = &ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.\
                                      choice.DRBs_ToBeSetupMod_List;

                     if(extractDrbListToSetupMod(NULL, drbSetupModCfg, NULL, drbSetupModCfg->list.count,\
                              &duUeCb->f1UeDb->duUeCfg, &duUeCb->drbBitMap, NULL))
                     {
                        DU_LOG("\nERROR  -->  DU APP : Failed at extractDrbListToSetupMod() for DrbSetupModList");
                        ret = RFAILED;
                     }
                  }

                  /*DRBs to be Modified*/
                  if(ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.present == \
                        UEContextModificationRequestIEs__value_PR_DRBs_ToBeModified_List)

                  {
                     drbModifiedCfg = &ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.\
                                      choice.DRBs_ToBeModified_List;
                     if(extractDrbListToSetupMod(NULL, NULL, drbModifiedCfg, drbModifiedCfg->list.count,\
                              &duUeCb->f1UeDb->duUeCfg, &duUeCb->drbBitMap, &duUeCb->duRlcUeCfg))
                     {
                        DU_LOG("\nERROR  -->  DU APP : Failed at extractDrbListToSetupMod() for DrbModifiedList");
                        ret = RFAILED;
                     }
                  }
                  /*DRBs to be Released*/
                  if(ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.present == \
                        UEContextModificationRequestIEs__value_PR_DRBs_ToBeReleased_List)

                  {
                     drbToRelease = &ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.\
                                      choice.DRBs_ToBeReleased_List;
                     if(extractDrbListToRelease(gnbDuUeF1apId, drbToRelease, drbToRelease->list.count,\
                              &duUeCb->f1UeDb->duUeCfg, &duUeCb->duRlcUeCfg))
                     {
                        DU_LOG("\nERROR  -->  DU APP : Failed at extractDrbListToSetupMod() for DrbModifiedList");
                        ret = RFAILED;
                     }
                  }
               }
               break;
            }

         case ProtocolIE_ID_id_GNB_DUConfigurationQuery:
            {
               DU_LOG("\nINFO  -->  DU APP : Received GNB DU Configuration Query in UE Context Modification Request from CU");
               if(duUeCb->f1UeDb)
               {
                  duUeCb->f1UeDb->actionType = UE_CTXT_CFG_QUERY;
               }
               break;
            }

         case ProtocolIE_ID_id_RRCReconfigurationCompleteIndicator:
            {
               if(ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.choice.RRCReconfigurationCompleteIndicator == \
                  RRCReconfigurationCompleteIndicator_true)
               {
                  duUeCb->f1UeDb->actionType = UE_CTXT_RRC_RECFG_COMPLETE;
               }
               break;
            }
         case ProtocolIE_ID_id_TransmissionActionIndicator:
            {
               if(duUeCb->f1UeDb)
               {
                  if(ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.choice.TransmissionActionIndicator  == TransmissionActionIndicator_stop)
                  {
                     duUeCb->f1UeDb->duUeCfg.dataTransmissionAction = STOP_TRANSMISSION; 
                  }
                  else 
                  {
                     duUeCb->f1UeDb->duUeCfg.dataTransmissionAction = RESTART_TRANSMISSION; 
                  }
               }
               break;
            }

         case ProtocolIE_ID_id_RRCDeliveryStatusRequest:
            {
               if(duUeCb->f1UeDb->dlRrcMsg->rrcMsgPdu)
               {
                  duUeCb->f1UeDb->dlRrcMsg->deliveryStatRpt = true;
               }
               break;
            }
#ifdef NR_DRX
         case ProtocolIE_ID_id_DRXConfigurationIndicator:
            {
               duUeCb->f1UeDb->duUeCfg.drxConfigIndicatorRelease = true;
               break;
            }
#endif
              
      }
   }

   if(ret != RFAILED) 
   {
      ret = duProcUeContextModReq(duUeCb);
   }
   freeAperDecodeUeContextModificationReqMsg(ueContextModifyReq);
   return ret; 
}

/*****************************************************************i
*
* @brief Free memory allocated for UE Context Release Request
*
* @details
*
*    Function : FreeUeContextReleaseReq
*
*    Functionality:
*         - Free memory allocated for UE Context Release Request
*
* @params[in] F1AP_PDU_t *f1apMsg
* @return void 
*
* *************************************************************/
void FreeUeContextReleaseReq(F1AP_PDU_t *f1apMsg)
{
   uint8_t ieIdx;
   UEContextReleaseRequest_t *ueReleaseReq = NULLP;
   
   if(f1apMsg)
   {
      if(f1apMsg->choice.initiatingMessage)
      {
         ueReleaseReq =&f1apMsg->choice.initiatingMessage->value.choice.UEContextReleaseRequest;
         if(ueReleaseReq->protocolIEs.list.array)
         {
            for(ieIdx=0 ; ieIdx<ueReleaseReq->protocolIEs.list.count; ieIdx++)
            {
               DU_FREE(ueReleaseReq->protocolIEs.list.array[ieIdx], sizeof(UEContextReleaseRequest_t));
            }
            DU_FREE(ueReleaseReq->protocolIEs.list.array, ueReleaseReq->protocolIEs.list.size);
         }
         DU_FREE(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      }
      DU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
   }

}
/*****************************************************************i
*
* @brief Build and Send UE Context Release Request  
*
* @details
*
*    Function : BuildAndSendUeContextReleaseReq
*
*    Functionality:
*         - Build and Send UE Context Release Request 
*
* @params[in]
* @return ROK     - success
*         RFAILED - failure
*
* *************************************************************/
uint8_t BuildAndSendUeContextReleaseReq(uint16_t cellId, uint8_t ueId)
{
   bool memAllocFail = false;
   uint8_t ieIdx =0;
   uint8_t ret = RFAILED;
   uint16_t cellIdx =0;
   uint16_t crnti = 0;
   uint8_t  elementCnt = 0;
   uint32_t  gnbCuUeF1apId =0;   /* gNB-CU UE F1AP Id */
   uint32_t  gnbDuUeF1apId =0;   /* gNB-DU UE F1AP Id */
   asn_enc_rval_t encRetVal; 
   F1AP_PDU_t *f1apMsg = NULLP;
   UEContextReleaseRequest_t *ueReleaseReq = NULLP;

   DU_LOG("\nINFO  --> Building the UE Context Release Request");
   do
   {
      DU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildAndSendUeContextReleaseReq(): Memory allocation failed for f1apMsg");
         break;
      }

      f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
      DU_ALLOC(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      if(f1apMsg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildAndSendUeContextReleaseReq(): Memory allocation failed for\
         initiatingMessage");   
         break;
      }
      f1apMsg->choice.initiatingMessage->procedureCode = ProcedureCode_id_UEContextReleaseRequest;
      f1apMsg->choice.initiatingMessage->criticality = Criticality_reject;
      f1apMsg->choice.initiatingMessage->value.present = \
      InitiatingMessage__value_PR_UEContextReleaseRequest;

      ueReleaseReq = &f1apMsg->choice.initiatingMessage->value.choice.UEContextReleaseRequest;

      elementCnt = 2;

      ueReleaseReq->protocolIEs.list.count = elementCnt;
      ueReleaseReq->protocolIEs.list.size = elementCnt * sizeof(UEContextReleaseRequest_t *);

      /* Initialize the F1Setup members */
      DU_ALLOC(ueReleaseReq->protocolIEs.list.array,ueReleaseReq->protocolIEs.list.size);
      if(ueReleaseReq->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildAndSendUeContextReleaseReq(): Memory allocation failed for IE array");
         break;
      }
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
         DU_ALLOC(ueReleaseReq->protocolIEs.list.array[ieIdx],\
               sizeof(UEContextReleaseRequest_t));
         if(ueReleaseReq->protocolIEs.list.array[ieIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  F1AP : BuildAndSendUeContextReleaseReq(): Memory allocation failed for IE elements");
            memAllocFail = true;  
            break;
         }
      }
      if(memAllocFail == true)
         break;

      /* Fetching Ue Cb Info*/
      GET_CELL_IDX(cellId, cellIdx);
      if(duCb.actvCellLst[cellIdx] == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildAndSendUeContextReleaseReq(): CellId[%d] does not exist", cellId);
         break;
      }
      else
      {
         GET_CRNTI(crnti, ueId);
         if(duCb.actvCellLst[cellIdx]->ueCb[ueId-1].crnti != crnti)
         {
            DU_LOG("\nERROR  -->  F1AP : BuildAndSendUeContextReleaseReq(): crnti[%d] does not exist", crnti);
            break;
         }
         gnbDuUeF1apId = duCb.actvCellLst[cellIdx]->ueCb[ueId-1].gnbDuUeF1apId;
         gnbCuUeF1apId = duCb.actvCellLst[cellIdx]->ueCb[ueId-1].gnbCuUeF1apId;
      }

      ieIdx=0; 
      ueReleaseReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID; 
      ueReleaseReq->protocolIEs.list.array[ieIdx]->criticality= Criticality_reject;
      ueReleaseReq->protocolIEs.list.array[ieIdx]->value.present = \
      UEContextReleaseRequestIEs__value_PR_GNB_CU_UE_F1AP_ID;
      ueReleaseReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_CU_UE_F1AP_ID =gnbCuUeF1apId;
      
      ieIdx++;
      ueReleaseReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
      ueReleaseReq->protocolIEs.list.array[ieIdx]->criticality= Criticality_reject;
      ueReleaseReq->protocolIEs.list.array[ieIdx]->value.present =\
      UEContextReleaseRequestIEs__value_PR_GNB_DU_UE_F1AP_ID;
      ueReleaseReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_UE_F1AP_ID =gnbDuUeF1apId;
      
      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the F1SetupRequest type as APER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf, encBuf);
      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  F1AP : Could not encode UEContextReleaseRequest structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG   -->  F1AP : Created APER encoded buffer for UEContextReleaseRequest\n");
#ifdef DEBUG_ASN_PRINT
         for(ieIdx=0; ieIdx< encBufSize; ieIdx++)
         {
            printf("%x",encBuf[ieIdx]);
         }
#endif
      }

      /* Sending msg */
      if(sendF1APMsg() != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Sending UE Context Release Request failed");
         break;
      }
      ret = ROK;
      break;
   }while(true);

   FreeUeContextReleaseReq(f1apMsg);
   return ret;
}
/*****************************************************************i
 *
 * @brief Free memory allocated for UE Context Release Complete
 *
 * @details
 *
 *    Function : FreeUeContextReleaseComplete
 *
 *    Functionality:
 *         - Free memory allocated for UE Context Release Complete
 *
 * @params[in] F1AP_PDU_t *f1apMsg
 * @return void
 *
 * *************************************************************/
void FreeUeContextReleaseComplete(F1AP_PDU_t *f1apMsg)
{
   uint8_t ieIdx;
   UEContextReleaseComplete_t *ueReleaseComplete = NULLP;

   if(f1apMsg)
   {
      if(f1apMsg->choice.successfulOutcome)
      {
         ueReleaseComplete =&f1apMsg->choice.successfulOutcome->value.choice.UEContextReleaseComplete;
         if(ueReleaseComplete->protocolIEs.list.array)
         {
            for(ieIdx=0 ; ieIdx<ueReleaseComplete->protocolIEs.list.count; ieIdx++)
            {
               DU_FREE(ueReleaseComplete->protocolIEs.list.array[ieIdx], sizeof(UEContextReleaseComplete_t));
            }
            DU_FREE(ueReleaseComplete->protocolIEs.list.array, ueReleaseComplete->protocolIEs.list.size);
         }
         DU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      }
      DU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
   }

}
/*****************************************************************i
 *
 * @brief Build and Send UE Context Release Complete
 *
 * @details
 *
 *    Function : BuildAndSendUeContextReleaseComplete
 *
 *    Functionality:
 *         - Build and Send UE Context Release Complete
 *
 * @params[in]  cellId,  gnbCuUeF1apId, gnbDuUeF1apId
 * @return ROK     - success
 *         RFAILED - failure
 *
 * *************************************************************/
uint8_t BuildAndSendUeContextReleaseComplete(uint16_t cellId, uint32_t gnbCuUeF1apId, uint32_t gnbDuUeF1apId)
{
   bool memAllocFail = false;
   uint8_t ieIdx =0, ret = RFAILED, elementCnt = 0;
   asn_enc_rval_t encRetVal;
   F1AP_PDU_t *f1apMsg = NULLP;
   UEContextReleaseComplete_t *ueReleaseComplete = NULLP;

   DU_LOG("\nINFO  --> Building the UE Context Release Complete");
   do
   {
      DU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildAndSendUeContextReleaseComplete(): Memory allocation failed for f1apMsg");
         break;
      }

      f1apMsg->present = F1AP_PDU_PR_successfulOutcome;
      DU_ALLOC(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      if(f1apMsg->choice.successfulOutcome == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildAndSendUeContextReleaseComplete(): Memory allocation failed for\
               successfulOutcome");
         break;
      }
      f1apMsg->choice.successfulOutcome->procedureCode = ProcedureCode_id_UEContextRelease;
      f1apMsg->choice.successfulOutcome->criticality = Criticality_reject;
      f1apMsg->choice.successfulOutcome->value.present = \
      SuccessfulOutcome__value_PR_UEContextReleaseComplete;

      ueReleaseComplete = &f1apMsg->choice.successfulOutcome->value.choice.UEContextReleaseComplete;

      elementCnt = 2;
      ueReleaseComplete->protocolIEs.list.count = elementCnt;
      ueReleaseComplete->protocolIEs.list.size = elementCnt * sizeof(UEContextReleaseComplete_t *);

      /* Initialize the UE Release Complete members */
      DU_ALLOC(ueReleaseComplete->protocolIEs.list.array,ueReleaseComplete->protocolIEs.list.size);
      if(ueReleaseComplete->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildAndSendUeContextReleaseComplete(): Memory allocation failed for IE array");
         break;
      }
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
         DU_ALLOC(ueReleaseComplete->protocolIEs.list.array[ieIdx],\
               sizeof(UEContextReleaseComplete_t));
         if(ueReleaseComplete->protocolIEs.list.array[ieIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  F1AP : BuildAndSendUeContextReleaseComplete(): Memory allocation failed for IE\
            elements");
            memAllocFail = true;
            break;
         }
      }
      if(memAllocFail == true)
         break;


      ieIdx=0;
      ueReleaseComplete->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;
      ueReleaseComplete->protocolIEs.list.array[ieIdx]->criticality= Criticality_reject;
      ueReleaseComplete->protocolIEs.list.array[ieIdx]->value.present = \
      UEContextReleaseCompleteIEs__value_PR_GNB_CU_UE_F1AP_ID;
      ueReleaseComplete->protocolIEs.list.array[ieIdx]->value.choice.GNB_CU_UE_F1AP_ID =gnbCuUeF1apId;

      ieIdx++;
      ueReleaseComplete->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
      ueReleaseComplete->protocolIEs.list.array[ieIdx]->criticality= Criticality_reject;
      ueReleaseComplete->protocolIEs.list.array[ieIdx]->value.present =\
      UEContextReleaseCompleteIEs__value_PR_GNB_DU_UE_F1AP_ID;
      ueReleaseComplete->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_UE_F1AP_ID =gnbDuUeF1apId;

      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the F1SetupComplete type as APER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf, encBuf);
      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  F1AP : Could not encode UEContextReleaseComplete structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG   -->  F1AP : Created APER encoded buffer for UEContextReleaseComplete\n");
#ifdef DEBUG_ASN_PRINT
         for(ieIdx=0; ieIdx< encBufSize; ieIdx++)
         {
            printf("%x",encBuf[ieIdx]);
         }
#endif
      }

      /* Sending msg */
      if(sendF1APMsg() != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Sending UE Context Release Complete failed");
         break;
      }
      ret = ROK;
      break;
   }while(true);
   
   if((ret == ROK) && (duCb.actvCellLst[cellId-1]) && (duCb.actvCellLst[cellId-1]->cellStatus == DELETION_IN_PROGRESS) 
         && (duCb.actvCellLst[cellId-1]->numActvUes == 0))
   {
      ret = duSendCellDeletReq(cellId);
      if(ret != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP: BuildAndSendUeContextReleaseComplete(): Failed to process cell\
               Delete req for CellId");
      }
   }
   FreeUeContextReleaseComplete(f1apMsg);
   return ret;

}

/*******************************************************************
*
* @brief added free part for the memory allocated by aper_decoder 
*
* @details
*
*    Function : freeAperDecodeUeContextReleaseCommand 
*
*    Functionality: added free part for the memory allocated by aper_decoder
*
* @params[in] F1AP_PDU_t *f1apMsg
* @return void
*
* ****************************************************************/
void freeAperDecodeUeContextReleaseCommand(F1AP_PDU_t *f1apMsg)
{
   uint8_t ieIdx=0;
   UEContextReleaseCommand_t *ueContextReleaseCommand = NULLP;

   ueContextReleaseCommand = &f1apMsg->choice.initiatingMessage->value.choice.UEContextReleaseCommand;
   
   if(ueContextReleaseCommand->protocolIEs.list.array)
   {
      for(ieIdx=0; ieIdx < ueContextReleaseCommand->protocolIEs.list.count; ieIdx++)
      {
         if(ueContextReleaseCommand->protocolIEs.list.array[ieIdx])
         {
            switch(ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->id)
            {
               case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
                  break;
               case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
                  break;
               case ProtocolIE_ID_id_Cause:
                  break;
               case ProtocolIE_ID_id_RRCContainer:
               {
                  if(ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.buf)
                  {
                     free(ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.buf);
                  }
                  break;
               }
               default :
                  DU_LOG("\nERROR  -->  F1AP: freeAperDecodeUeContextReleaseCommand():Invalid IE Received: %ld"\
                       ,ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->id);
                  break;
            }
         }
         free(ueContextReleaseCommand->protocolIEs.list.array[ieIdx]);
      }
      free(ueContextReleaseCommand->protocolIEs.list.array);
   }
}
/*******************************************************************
*
* @brief processing of UE Context Release Command
*
* @details
*
*    Function : procF1UeContextReleaseCommand 
*
*    Functionality: processing of UE Context Release Command
*
* @params[in] F1AP_PDU_t *f1apMsg
* @return void
*
* ****************************************************************/
uint8_t procF1UeContextReleaseCommand(F1AP_PDU_t *f1apMsg)
{
   uint8_t  ieIdx=0, ret=ROK, ueIdx=0;
   uint16_t cellIdx =0, cellId = 0;
   bool ueIdxFound = false;
   uint32_t gnbCuUeF1apId=0, gnbDuUeF1apId=0;
   DuUeCb   *duUeCb = NULLP;
   UEContextReleaseCommand_t *ueContextReleaseCommand = NULLP;

   ueContextReleaseCommand = &f1apMsg->choice.initiatingMessage->value.choice.UEContextReleaseCommand;

   if(ueContextReleaseCommand->protocolIEs.list.array)
   {
      for(ieIdx=0; ieIdx < ueContextReleaseCommand->protocolIEs.list.count; ieIdx++)
      {
         if(ueContextReleaseCommand->protocolIEs.list.array[ieIdx])
         {
            switch(ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->id)
            {
               case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
                  {
                     gnbCuUeF1apId= ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->\
                                    value.choice.GNB_CU_UE_F1AP_ID;
                     break;
                  }

               case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
                  {
                     gnbDuUeF1apId = ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->\
                                     value.choice.GNB_DU_UE_F1AP_ID;
                     break;
                  }

               case ProtocolIE_ID_id_Cause:
                  {
                     for(cellIdx = 0; cellIdx < MAX_NUM_CELL; cellIdx++)
                     {
                        for(ueIdx = 0; ueIdx < MAX_NUM_UE; ueIdx++)
                        {
                           if(duCb.actvCellLst[cellIdx] && (duCb.actvCellLst[cellIdx]->ueCb[ueIdx].gnbDuUeF1apId == gnbDuUeF1apId)&&\
                                 (duCb.actvCellLst[cellIdx]->ueCb[ueIdx].gnbCuUeF1apId == gnbCuUeF1apId))
                           {
                              cellId = duCb.actvCellLst[cellIdx]->cellId;
                              duUeCb = &duCb.actvCellLst[cellIdx]->ueCb[ueIdx];
                              ueIdxFound = true;
                              break;
                           }
                        }
                        if(ueIdxFound == true)
                        {
                           break;
                        }
                     }
                     
                     if(!ueIdxFound)
                     {
                        DU_LOG("\nERROR  -->  F1AP: procF1UeContextReleaseCommand(): Ue Information is not available");
                        ret = RFAILED;
                     }
                     break;
                  }

               case ProtocolIE_ID_id_RRCContainer:
                  {
                     if(ueIdxFound == true)  
                     {
                        DU_ALLOC(duUeCb->f1UeDb, sizeof(F1UeContextSetupDb));
                        if(duUeCb->f1UeDb)
                        {
                           memset(duUeCb->f1UeDb, 0, sizeof(F1UeContextSetupDb));
                           duUeCb->f1UeDb->actionType = UE_CTXT_RELEASE;
                           duUeCb->f1UeDb->cellIdx = cellIdx;
                           /* Filling Dl RRC Msg Info */
                           DU_ALLOC_SHRABL_BUF(duUeCb->f1UeDb->dlRrcMsg, sizeof(F1DlRrcMsg));
                           if(!duUeCb->f1UeDb->dlRrcMsg)
                           {
                              DU_LOG("\nERROR  -->  DU APP : procF1UeContextReleaseCommand(): \
                                    Memory allocation failed ");
                              ret = RFAILED;
                           }
                           else
                           {
                              duUeCb->f1UeDb->dlRrcMsgPres = true;
                              memset(duUeCb->f1UeDb->dlRrcMsg, 0, sizeof(F1DlRrcMsg));
                              ret = extractDlRrcMsg(gnbDuUeF1apId, gnbCuUeF1apId, duUeCb->f1UeDb->dlRrcMsg,\
                                    &ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->\
                                    value.choice.RRCContainer);
                           }

                        }
                        else
                        {
                           DU_LOG("\nERROR  -->  DU APP : procF1UeContextReleaseCommand(): \
                                 Memory allocation failed ");
                           ret = RFAILED;

                        }
                     }
                     break;
                  }
               default :
                  DU_LOG("\nERROR  -->  F1AP: freeAperDecodeUeContextReleaseCommand():Invalid IE Received: %ld"\
                        ,ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->id);
                  break;
            }
         }
      }
   }
   if(ret != RFAILED)
   {
      duProcUeContextReleaseCommand(cellId, duUeCb);
   }
   freeAperDecodeUeContextReleaseCommand(f1apMsg);
   return ret;
}

/**************************************************************
 *
 * @brief free the memory allocated by aper decoder for paging
 *
 * @details
 *
 *    Function : freeAperDecodePagingMsg
 *
 *    Functionality:
 *         - free the memory allocated by aper decoder for
 *         the paging f1ap msg
 *
 * @params[in] Paging_t   *paging
 * @return ROK     - success
 *         RFAILED - failure
 *
 ****************************************************************/
void freeAperDecodePagingMsg(Paging_t   *paging)
{
   uint8_t ieIdx, cellIdx;
   PagingCell_ItemIEs_t *pagingCellItemIes;
   PagingCell_Item_t *pagingCellItem;
   PagingCell_list_t  *pagingCelllist;

   if(paging)
   {
      if(paging->protocolIEs.list.array)
      {
         for(ieIdx=0 ; ieIdx<paging->protocolIEs.list.count; ieIdx++)
         {
            if(paging->protocolIEs.list.array[ieIdx])
            {
               switch(paging->protocolIEs.list.array[ieIdx]->id)
               {
                  case ProtocolIE_ID_id_UEIdentityIndexValue:
                     {
                        free(paging->protocolIEs.list.array[ieIdx]->value.choice.UEIdentityIndexValue.choice.indexLength10.buf);
                        break;
                     }
                  case ProtocolIE_ID_id_PagingIdentity:
                     {
                        if(paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.present == PagingIdentity_PR_cNUEPagingIdentity)
                        {
                           if(paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.choice.cNUEPagingIdentity)
                           {
                              if(paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.choice.cNUEPagingIdentity->present == \
                                    CNUEPagingIdentity_PR_fiveG_S_TMSI)
                              {
                                 free(paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.choice.cNUEPagingIdentity->choice.fiveG_S_TMSI.buf);
                              }
                              free(paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.choice.cNUEPagingIdentity);
                           }
                        }
                        break;
                     }
                  case ProtocolIE_ID_id_PagingCell_List:
                     {
                        pagingCelllist = &paging->protocolIEs.list.array[ieIdx]->value.choice.PagingCell_list;
                        if(pagingCelllist->list.array)
                        {
                           for(cellIdx = 0; cellIdx < pagingCelllist->list.count; cellIdx++)
                           {
                              if(pagingCelllist->list.array[cellIdx])
                              {
                                 pagingCellItemIes = (PagingCell_ItemIEs_t *)pagingCelllist->list.array[cellIdx];
                                 if(pagingCellItemIes->id == ProtocolIE_ID_id_PagingCell_Item)
                                 {
                                    pagingCellItem = &pagingCellItemIes->value.choice.PagingCell_Item;
                                    free(pagingCellItem->nRCGI.pLMN_Identity.buf);
                                    free(pagingCellItem->nRCGI.nRCellIdentity.buf);
                                 }
                                 free(pagingCelllist->list.array[cellIdx]);
                              }
                           }
                           free(pagingCelllist->list.array);
                        }
                        break;
                     }
               }
               free(paging->protocolIEs.list.array[ieIdx]);
            }
         }
         free(paging->protocolIEs.list.array);

      }
   }
}

/**************************************************************
 *
 * @brief processing the paging f1ap msg received from CU 
 *
 * @details
 *
 *    Function : procPagingMsg
 *
 *    Functionality:
 *         - processing the paging f1ap msg received from CU
 *
 * @params[in] F1AP_PDU_t *f1apMsg
 * @return ROK     - success
 *         RFAILED - failure
 *
 *
 ****************************************************************/
uint8_t procPagingMsg(F1AP_PDU_t *f1apMsg) 
{
   uint8_t ieIdx = 0, cellListIdx = 0;
   uint64_t cellId = 0;
   Paging_t   *paging = NULLP;
   PagingCell_list_t  *pagingCelllist = NULLP;
   PagingCell_ItemIEs_t *pagingCellItemIes = NULLP;
   PagingCell_Item_t *pagingCellItem = NULLP;
   DuPagingMsg *tmpPagingParam = NULLP;

   paging = &f1apMsg->choice.initiatingMessage->value.choice.Paging;
   if(paging)
   {
      if(paging->protocolIEs.list.array)
      {
         DU_ALLOC(tmpPagingParam, sizeof(DuPagingMsg));
         if(tmpPagingParam == NULLP)
         {
            DU_LOG("\nERROR  --> DU APP : Memory Allocation Failure in procPagingMsg");
            freeAperDecodePagingMsg(paging);
            return RFAILED;
         }
         for(ieIdx=0 ; ieIdx<paging->protocolIEs.list.count; ieIdx++)
         {
            if(paging->protocolIEs.list.array[ieIdx])
            {
               switch(paging->protocolIEs.list.array[ieIdx]->id)
               {
                  case ProtocolIE_ID_id_UEIdentityIndexValue:
                     {
                        bitStringToInt(&paging->protocolIEs.list.array[ieIdx]->value.choice.UEIdentityIndexValue.choice.indexLength10,\
                                         &tmpPagingParam->pagUeId);
                        break;
                     }

                  case ProtocolIE_ID_id_PagingIdentity:
                     {
                        switch(paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.present)
                        {
                           case PagingIdentity_PR_cNUEPagingIdentity: 
                              {
                                 if(paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.choice.cNUEPagingIdentity)  
                                 {
                                    bitStringToInt(&paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.choice.\
                                          cNUEPagingIdentity->choice.fiveG_S_TMSI, &tmpPagingParam->sTmsi);

                                 }
                                 break;
                              }
                            case PagingIdentity_PR_rANUEPagingIdentity:
                               {
                                  /*TODO: This Identifier is specific to RAN Initiated Paging – Connected Mode Paging*/
                                  break;
                               }
                            default:
                               {
                                  DU_LOG("ERROR  -->  DU APP: Invalid UE Paging Identity, Skipping this Paging Message:");
                                  continue;
                               }
                        }
                     }

                  case ProtocolIE_ID_id_PagingDRX:
                     {
                        tmpPagingParam->pagingDrxPres = TRUE;
                        tmpPagingParam->pagingDrx = convertPagingCycleEnumToValue(paging->protocolIEs.list.array[ieIdx]->value.choice.PagingDRX);
                        break;
                     }

                  case ProtocolIE_ID_id_PagingPriority:
                     {
                        tmpPagingParam->pagPriority = paging->protocolIEs.list.array[ieIdx]->value.choice.PagingPriority;
                        break;
                     }

                  case ProtocolIE_ID_id_PagingCell_List:
                     {
                        pagingCelllist = &paging->protocolIEs.list.array[ieIdx]->value.choice.PagingCell_list; 
                        if(pagingCelllist->list.array)
                        {
                           for(cellListIdx = 0; cellListIdx < pagingCelllist->list.count; cellListIdx++)
                           {
                              if(pagingCelllist->list.array[cellListIdx])
                              {
                                 pagingCellItemIes = (PagingCell_ItemIEs_t *)pagingCelllist->list.array[cellListIdx];
                                 pagingCellItem = &pagingCellItemIes->value.choice.PagingCell_Item;
                                 bitStringToInt(&pagingCellItem->nRCGI.nRCellIdentity, &cellId);
                                 if(processPagingMsg(cellId, tmpPagingParam) != ROK)
                                 {
                                    DU_LOG("\nERROR  --> DU APP : Paging Processing Failed at CellId:%lu",cellId);
                                    continue;
                                 }
                              }
                           }
                        }
                        break;
                     }
                   default:
                     {
                         DU_LOG("\nERROR  --> F1AP : Incorrect Paging IE option");
                         break;
                     }
               }
            }
         }
      }
   }
   DU_FREE(tmpPagingParam, sizeof(DuPagingMsg));
   freeAperDecodePagingMsg(paging);
  
   return ROK;
}

/**************************************************************
 *
 * @brief Handles received F1AP message and sends back response  
 *
 * @details
 *
 *    Function : F1APMsgHdlr
 *
 *    Functionality:
 *         - Decodes received F1AP control message
 *         - Prepares response message, encodes and sends to SCTP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void F1APMsgHdlr(Buffer *mBuf)
{
   int i =0;
   char *recvBuf =NULLP;
   MsgLen copyCnt =0;
   MsgLen recvBufLen =0;
   F1AP_PDU_t *f1apMsg =NULLP;
   asn_dec_rval_t rval; /* Decoder return value */
   F1AP_PDU_t f1apasnmsg ;
   DU_LOG("\nINFO  -->  F1AP : Received F1AP message buffer");
   ODU_PRINT_MSG(mBuf, 0,0);

   /* Copy mBuf into char array to decode it */
   ODU_GET_MSG_LEN(mBuf, &recvBufLen);
   DU_ALLOC(recvBuf, (Size)recvBufLen);

   if(recvBuf == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed");
      return;
   }
   if(ODU_COPY_MSG_TO_FIX_BUF(mBuf, 0, recvBufLen, (Data *)recvBuf, &copyCnt) != ROK)
   {
      DU_LOG("\nERROR   -->  F1AP : Failed while copying %d", copyCnt);
      return;
   }

#ifdef DEBUG_ASN_PRINT
   printf("\nDEBUG   -->  F1AP : Received flat buffer to be decoded : ");
   for(i=0; i< recvBufLen; i++)
   {
      printf("%x",recvBuf[i]);
   }
#endif

   /* Decoding flat buffer into F1AP messsage */
   f1apMsg = &f1apasnmsg;
   memset(f1apMsg, 0, sizeof(F1AP_PDU_t));

   rval = aper_decode(0, &asn_DEF_F1AP_PDU, (void **)&f1apMsg, recvBuf, recvBufLen, 0, 0);

   if(rval.code == RC_FAIL || rval.code == RC_WMORE)
   {
      DU_LOG("\nERROR  -->  F1AP : ASN decode failed");
      return;
   }
   printf("\n");
   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

   switch(f1apMsg->present)
   {
      case F1AP_PDU_PR_successfulOutcome:
         {
            switch(f1apMsg->choice.successfulOutcome->value.present)
            {
               case SuccessfulOutcome__value_PR_ResetAcknowledge:
                  {
                     DU_LOG("\nINFO   -->  F1AP : F1ResetAcknowledge is received successfully ");
                     break;
                  }
               case SuccessfulOutcome__value_PR_F1SetupResponse:
                  {				
#ifndef ODU_TEST_STUB
                     procF1SetupRsp(f1apMsg, recvBufLen, recvBuf);
#endif
                     break;
                  }

               case SuccessfulOutcome__value_PR_GNBDUConfigurationUpdateAcknowledge:
                  {
                     procF1GNBDUCfgUpdAck(f1apMsg, recvBufLen, recvBuf);
                     break;
                  }

               default:
                  {
                     DU_LOG("\nERROR  -->  F1AP : Invalid type of SuccessfulOutcome__value_PR_ResetAcknowledge [%d]",\
                           f1apMsg->choice.successfulOutcome->value.present);
                     return;
                  }
            }/* End of switch(successfulOutcome) */
            free(f1apMsg->choice.successfulOutcome);
            break;
         }
      case F1AP_PDU_PR_initiatingMessage:
         {
            switch(f1apMsg->choice.initiatingMessage->value.present)
            {
               case InitiatingMessage__value_PR_Reset:
                  {
                     procF1ResetReq(f1apMsg);
                     break;
                  }
               case InitiatingMessage__value_PR_DLRRCMessageTransfer:
                  {
                     procF1DlRrcMsgTrans(f1apMsg);
                     break;
                  }
               case InitiatingMessage__value_PR_UEContextSetupRequest:
                  {
                     procF1UeContextSetupReq(f1apMsg);
                     break;
                  }
               case InitiatingMessage__value_PR_UEContextModificationRequest:
                  {
                     procF1UeContextModificationReq(f1apMsg);
                     break;
                  }
               case InitiatingMessage__value_PR_UEContextReleaseCommand:
                  {
                      procF1UeContextReleaseCommand(f1apMsg);
                      break;
                  }
               case InitiatingMessage__value_PR_Paging:
                  {
                     procPagingMsg(f1apMsg);
                     break;
                  }
               default:
                  {
                     DU_LOG("\nERROR  -->  F1AP : Invalid type of F1AP_PDU_PR_initiatingMessage [%d]",
                           f1apMsg->choice.initiatingMessage->value.present);
                     return;
                  }
            }/* End of switch(initiatingMessage) */
            free(f1apMsg->choice.initiatingMessage);
            break;
         }

      default:
         {
            DU_LOG("\nERROR  -->  F1AP : Invalid type of f1apMsg->present [%d]",f1apMsg->present);
            return;
         }
         free(f1apMsg);

   }/* End of switch(f1apMsg->present) */
   
   DU_FREE(recvBuf, (Size)recvBufLen);
} /* End of F1APMsgHdlr */

/**********************************************************************
  End of file
 **********************************************************************/
