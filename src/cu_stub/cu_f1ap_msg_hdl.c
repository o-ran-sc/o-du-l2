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
#include "OCTET_STRING.h"
#include "BIT_STRING.h"
#include "odu_common_codec.h"
#include "ProtocolIE-Field.h"
#include "ProtocolExtensionField.h"
#include "ProtocolIE-SingleContainer.h"
#include "F1SetupResponse.h"
#include "SuccessfulOutcome.h"
#include "InitiatingMessage.h"
#include "NonDynamic5QIDescriptor.h"
#include "AveragingWindow.h"
#include "MaxDataBurstVolume.h"
#include "Flows-Mapped-To-DRB-Item.h"
#include "SliceSupportItem.h"
#include "ServedPLMNs-Item.h"
#include "F1AP-PDU.h"
#include "ULUPTNLInformation-ToBeSetup-Item.h"
#include "ULConfiguration.h"
#include "ModulationOrder.h"
#include "BandNR.h"
#include "UE-CapabilityRAT-Container.h"
#include "UE-CapabilityRAT-ContainerList.h"
#include "UE-CapabilityRAT-ContainerListRRC.h"
#include "HandoverPreparationInformationRrc-IEs.h"
#include "HandoverPreparationInformationRrc.h"
#include "SupportedBandwidth.h"
#include "FeatureSetUplinkPerCC.h"
#include "FeatureSetDownlinkPerCC.h"
#include "FeatureSets.h"
#include "RF-Parameters.h"
#include "UE-NR-Capability.h"
#include "ProtocolExtensionContainer.h"
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
#include "ServingCellConfig.h"
#include "ControlResourceSet.h"
#include "SearchSpace.h"
#include "PDCCH-Config.h"
#include "PDSCH-TimeDomainResourceAllocation.h"
#include "PDSCH-TimeDomainResourceAllocationList.h"
#include "DMRS-DownlinkConfig.h"
#include "PDSCH-Config.h"
#include "BWP-DownlinkDedicated.h"
#include "PUSCH-TimeDomainResourceAllocation.h"
#include "PUSCH-TimeDomainResourceAllocationList.h"
#include "DMRS-UplinkConfig.h"
#include "PUSCH-Config.h"
#include "SRS-ResourceId.h"
#include "SRS-Resource.h"
#include "SRS-ResourceSet.h"
#include "SRS-Config.h"
#include "PUCCH-Config.h"
#include "PUCCH-ResourceSet.h"
#include "PUCCH-Resource.h"
#include "PUCCH-format1.h"
#include "PUCCH-FormatConfig.h"
#include "BWP-UplinkDedicated.h"
#include "PUSCH-ServingCellConfig.h"
#include "UplinkConfig.h"
#include "PDSCH-ServingCellConfig.h"
#include "EUTRANQoS.h"
#include "GBR-QosInformation.h"
#include "DRBs-ToBeSetupMod-List.h"
#include "DRBs-ToBeSetupMod-Item.h"
#include "DRBs-Setup-Item.h"
#include "DLUPTNLInformation-ToBeSetup-List.h"
#include "DLUPTNLInformation-ToBeSetup-Item.h"
#include "UPTransportLayerInformation.h"
#include "GTPTunnel.h"
#include "QoSInformation.h"
#include "Cells-to-be-Activated-List.h"
#include "DL-CCCH-Message.h"
#include "SRB-ToAddModList.h"
#include "SRB-ToAddMod.h"
#include "RRCSetup-IEs.h"
#include "RRCSetup.h"
#include "DL-DCCH-Message.h"
#include "RRCReconfiguration-IEs.h"
#include "RRCReconfiguration.h"
#include "DRB-ToAddModList.h"
#include "DRB-ToAddMod.h"
#include "SDAP-Config.h"
#include "SSB-MTC.h"
#include "MeasTiming.h"
#include "MeasTimingList.h"
#include "MeasurementTimingConfigurationRrc-IEs.h"
#include "MeasurementTimingConfigurationRrc.h"
#include "PDCP-Config.h"
#include "RSRP-Range.h"
#include "RSRQ-Range.h"
#include "SINR-Range.h"
#include "ThresholdNR.h"
#include "MeasObjectToAddMod.h"
#include "MeasObjectNR.h"
#include "MeasObjectToAddModList.h"
#include "EventTriggerConfig.h"
#include "ReportConfigNR.h"
#include "ReportConfigToAddMod.h"
#include "ReportConfigToAddModList.h"
#include "MeasIdToAddMod.h"
#include "MeasIdToAddModList.h"
#include "FilterCoefficient.h"
#include "QuantityConfigNR.h"
#include "QuantityConfig.h"
#include "MeasConfigRrc.h"
#include "AS-Config.h"
#include "RRCReconfiguration-v1530-IEs.h"
#include "CNUEPagingIdentity.h"
#include "PagingCell-Item.h"
#include "UL-DCCH-Message.h"
#include "DRX-ConfigRrc.h"

#include "cu_stub_sctp.h"
#include "cu_stub_egtp.h"
#include "cu_f1ap_msg_hdl.h"
#include "cu_stub.h"

uint8_t fillCellGrpCfg(CuUeCb *ueCb, OCTET_STRING_t *cellGrp, bool updateAllRbCfg);
uint8_t fillRrcReconfig(CuUeCb *ueCb, RRCReconfiguration_t *rrcReconfig, bool updateAllRbCfg);

/*******************************************************************
 *
 * @brief Sends F1 msg over SCTP
 *
 * @details
 *
 *    Function : SendF1APMsg
 *
 *    Functionality: Sends F1 msg over SCTP
 *
 * @params[in] Region region
 *             Pool pool
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 SendF1APMsg(Region region, Pool pool, uint32_t duId)
{
   Buffer *mBuf = NULLP;

   if(ODU_GET_MSG_BUF(region, pool, &mBuf) == ROK)
   {
      if(ODU_ADD_POST_MSG_MULT((Data *)encBuf, encBufSize, mBuf) == ROK)
      {
         ODU_PRINT_MSG(mBuf, 0,0);

         if(sctpSend(F1_INTERFACE, duId, mBuf) != ROK)
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
} /* SendF1APMsg */

/********************************************************************
 *
 * @brief Builds and sends the F1SetupResponse
 *
 * @details
 *
 *    Function : BuildAndSendF1SetupRsp
 *
 *    Functionality: Constructs the F1SetupResponse message and sends
 *                   it back to the DU through SCTP.
 *
 * @params[in] void **buf,Buffer to which encoded pattern is written into
 * @params[in] int *size,size of buffer
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendF1SetupRsp(uint32_t duId, BIT_STRING_t *nrcellId)
{
   uint8_t    idx,ieIdx;
   uint8_t    elementCnt,cellCnt;
   F1AP_PDU_t         *f1apMsg = NULL;
   F1SetupResponse_t  *f1SetupRsp = NULL;
   GNB_CU_Name_t      *cuName = NULL;
   Cells_to_be_Activated_List_t *cellToActivate;
   RRC_Version_t      *rrcVer = NULL;
   asn_enc_rval_t     encRetVal; 
   DU_LOG("\nINFO  -->  F1AP : Building F1 Setup Response\n");

   /* Allocate the memory for F1SetupRequest_t */
   CU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t)); 
   if(f1apMsg == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation for F1AP-PDU failed");
      return RFAILED;
   }
   f1apMsg->present =  F1AP_PDU_PR_successfulOutcome;

   CU_ALLOC(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
   if(f1apMsg->choice.successfulOutcome == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation for F1AP-PDU failed");
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;  
   }

   f1apMsg->choice.successfulOutcome->procedureCode = ProcedureCode_id_F1Setup;
   f1apMsg->choice.successfulOutcome->criticality = Criticality_reject;
   f1apMsg->choice.successfulOutcome->value.present = \
                                                      SuccessfulOutcome__value_PR_F1SetupResponse;
   f1SetupRsp = &f1apMsg->choice.successfulOutcome->value.choice.F1SetupResponse;

   elementCnt = 3;
   f1SetupRsp->protocolIEs.list.count = elementCnt;
   f1SetupRsp->protocolIEs.list.size = elementCnt*sizeof(F1SetupResponseIEs_t *);

   CU_ALLOC(f1SetupRsp->protocolIEs.list.array, \
         elementCnt * sizeof(F1SetupResponseIEs_t *));
   if(f1SetupRsp->protocolIEs.list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation for F1ResponseIEs failed");
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }

   for(idx=0; idx<elementCnt; idx++)
   {
      CU_ALLOC(f1SetupRsp->protocolIEs.list.array[idx], \
            sizeof(F1SetupResponseIEs_t)); 
      if(f1SetupRsp->protocolIEs.list.array[idx] == NULLP)
      {  
         CU_FREE(f1SetupRsp->protocolIEs.list.array,\
               elementCnt * sizeof(F1SetupResponseIEs_t *));
         CU_FREE(f1apMsg->choice.successfulOutcome, \
               sizeof(SuccessfulOutcome_t));
         CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
         return RFAILED;
      }    
   }

   /*TransactionID*/
   idx = 0;
   f1SetupRsp->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_TransactionID;
   f1SetupRsp->protocolIEs.list.array[idx]->criticality = Criticality_reject;
   f1SetupRsp->protocolIEs.list.array[idx]->value.present = \
                                                            F1SetupResponseIEs__value_PR_TransactionID;
   f1SetupRsp->protocolIEs.list.array[idx]->value.choice.TransactionID =\
                                                                        TRANS_ID;
#if 0
   /* CU name IE is of type printableString_t which wireshark is unable to decode.
    * However this string is decoded successfully on online decoders.
    * Since this is an optional IE and the value received in it are not
    * used as of now, eliminating this IE for now to avoid wireshark error.
    */
   /*CU Name*/
   idx++;
   f1SetupRsp->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_gNB_CU_Name;
   f1SetupRsp->protocolIEs.list.array[idx]->criticality = Criticality_ignore;
   f1SetupRsp->protocolIEs.list.array[idx]->value.present = F1SetupResponseIEs__value_PR_GNB_CU_Name;
   cuName = &f1SetupRsp->protocolIEs.list.array[idx]->value.choice.GNB_CU_Name;
   cuName->size = strlen((char *)cuCb.cuCfgParams.cuName);

   CU_ALLOC(cuName->buf, cuName->size); 
   if(cuName->buf == NULLP)
   {
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
         CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], sizeof(F1SetupResponseIEs_t));
      }
      CU_FREE(f1SetupRsp->protocolIEs.list.array, elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   strcpy((char*)cuName->buf, (char*)cuCb.cuCfgParams.cuName);
#endif

   /*Cells to be activated list*/
   idx++;
   f1SetupRsp->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_Cells_to_be_Activated_List ;
   f1SetupRsp->protocolIEs.list.array[idx]->criticality = Criticality_reject;
   f1SetupRsp->protocolIEs.list.array[idx]->value.present = F1SetupResponseIEs__value_PR_Cells_to_be_Activated_List;
   cellToActivate = &f1SetupRsp->protocolIEs.list.array[idx]->value.choice.Cells_to_be_Activated_List;

   cellCnt=1;
   cellToActivate->list.count = cellCnt;
   cellToActivate->list.size = cellCnt * sizeof(struct Cells_to_be_Activated_List_ItemIEs  *);
   CU_ALLOC(cellToActivate->list.array, sizeof(struct Cells_to_be_Activated_List_ItemIEs  *));
   if(cellToActivate->list.array == NULLP)
   {
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
         CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], sizeof(F1SetupResponseIEs_t));
      }
      CU_FREE(f1SetupRsp->protocolIEs.list.array, elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   for(ieIdx=0; ieIdx<cellCnt; ieIdx++)
   {
      CU_ALLOC(cellToActivate->list.array[ieIdx],sizeof(struct Cells_to_be_Activated_List_ItemIEs ));
      if(cellToActivate->list.array[ieIdx] == NULLP)
      {
         CU_FREE(cellToActivate->list.array, sizeof(struct Cells_to_be_Activated_List_ItemIEs  *));
         CU_FREE(cuName->buf, sizeof(cuName->size));
         for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
         {
            CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], sizeof(F1SetupResponseIEs_t));
         }
         CU_FREE(f1SetupRsp->protocolIEs.list.array, elementCnt * sizeof(F1SetupResponseIEs_t *));
         CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
         CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
         return RFAILED;
      }
   }
   cellToActivate->list.array[0]->id = \
                                       ProtocolIE_ID_id_Cells_to_be_Activated_List_Item;
   cellToActivate->list.array[0]->criticality = Criticality_ignore;
   cellToActivate->list.array[0]->value.present = \
                                                  Cells_to_be_Activated_List_ItemIEs__value_PR_Cells_to_be_Activated_List_Item;
   cellToActivate->list.array[0]->value.choice.Cells_to_be_Activated_List_Item.\
      nRCGI.pLMN_Identity.size = 3*sizeof(uint8_t);
   CU_ALLOC(cellToActivate->list.array[0]->\
         value.choice.Cells_to_be_Activated_List_Item.nRCGI.pLMN_Identity.buf,\
         3*sizeof(uint8_t));
   if(cellToActivate->list.array[0]->value.choice.\
         Cells_to_be_Activated_List_Item.nRCGI.pLMN_Identity.buf == NULLP)
   {

      for(ieIdx=0; ieIdx<cellCnt; ieIdx++)
      {
         CU_FREE(cellToActivate->list.array[ieIdx],\
               sizeof(struct Cells_to_be_Activated_List_ItemIEs ));
      }

      CU_FREE(cellToActivate->list.array,\
            sizeof(struct Cells_to_be_Activated_List_ItemIEs  *));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
         CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
               sizeof(F1SetupResponseIEs_t));
      }
      CU_FREE(f1SetupRsp->protocolIEs.list.array, \
            elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, \
            sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   buildPlmnId(cuCb.cuCfgParams.plmn , cellToActivate->list.array[0]->value.choice.\
         Cells_to_be_Activated_List_Item.nRCGI.pLMN_Identity.buf);
   cellToActivate->list.array[0]->value.choice.Cells_to_be_Activated_List_Item.\
      nRCGI.nRCellIdentity.size = 5*sizeof(uint8_t);
   CU_ALLOC(cellToActivate->list.array[0]->value.choice.\
         Cells_to_be_Activated_List_Item.nRCGI.nRCellIdentity.buf,\
         cellToActivate->list.array[0]->value.choice.Cells_to_be_Activated_List_Item.\
	 nRCGI.nRCellIdentity.size);
   if(cellToActivate->list.array[0]->value.choice.\
         Cells_to_be_Activated_List_Item.nRCGI.nRCellIdentity.buf == NULLP)
   {
      CU_FREE(cellToActivate->list.array[0]->\
            value.choice.Cells_to_be_Activated_List_Item.nRCGI.pLMN_Identity.buf,\
            3*sizeof(uint8_t));
      for(ieIdx=0; ieIdx<cellCnt; ieIdx++)
      {
         CU_FREE(cellToActivate->list.array[ieIdx],\
               sizeof(struct Cells_to_be_Activated_List_ItemIEs ));
      }

      CU_FREE(cellToActivate->list.array,\
            sizeof(struct Cells_to_be_Activated_List_ItemIEs  *));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
         CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
               sizeof(F1SetupResponseIEs_t));
      }
      CU_FREE(f1SetupRsp->protocolIEs.list.array, \
            elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, \
            sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   memcpy(&cellToActivate->list.array[0]->value.choice.Cells_to_be_Activated_List_Item.nRCGI.nRCellIdentity, nrcellId, sizeof(BIT_STRING_t)); 

   /* RRC Version */
   idx++;
   f1SetupRsp->protocolIEs.list.array[idx]->id = \
                                                 ProtocolIE_ID_id_GNB_CU_RRC_Version;
   f1SetupRsp->protocolIEs.list.array[idx]->criticality = Criticality_reject;
   f1SetupRsp->protocolIEs.list.array[idx]->value.present = \
                                                            F1SetupResponseIEs__value_PR_RRC_Version;
   rrcVer = &f1SetupRsp->protocolIEs.list.array[idx]->value.choice.RRC_Version;
   rrcVer->latest_RRC_Version.size = RRC_SIZE; 

   CU_ALLOC(rrcVer->latest_RRC_Version.buf, sizeof(uint8_t));
   if(rrcVer->latest_RRC_Version.buf == NULLP)
   {  
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; idx++)
      {
         CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
               sizeof(F1SetupResponseIEs_t));
      } 
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
            elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }

   /* Need to check RRC Version */
   rrcVer->latest_RRC_Version.buf[0] = cuCb.cuCfgParams.rrcVersion.rrcVer; 
   rrcVer->latest_RRC_Version.bits_unused = 5; //TODO: pick from cuCfgParam. If not present, add it
   CU_ALLOC(rrcVer->iE_Extensions,sizeof(ProtocolExtensionContainer_4624P81_t));
   if(rrcVer->iE_Extensions == NULLP)
   {
      CU_FREE(rrcVer->latest_RRC_Version.buf, sizeof(uint8_t));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
         CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
               sizeof(F1SetupResponseIEs_t));
      } 
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
            elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   rrcVer->iE_Extensions->list.count = 1;
   rrcVer->iE_Extensions->list.size = sizeof(struct RRC_Version_ExtIEs *);
   CU_ALLOC(rrcVer->iE_Extensions->list.array,\
         sizeof(struct RRC_Version_ExtIEs *));
   if(rrcVer->iE_Extensions->list.array == NULLP)
   {
      CU_FREE(rrcVer->iE_Extensions,\
            sizeof(ProtocolExtensionContainer_4624P81_t));
      CU_FREE(rrcVer->latest_RRC_Version.buf, sizeof(uint8_t));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
         CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
               sizeof(F1SetupResponseIEs_t));
      } 
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
            elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   CU_ALLOC(rrcVer->iE_Extensions->list.array[0],\
         sizeof(struct RRC_Version_ExtIEs));
   if(rrcVer->iE_Extensions->list.array[0] == NULLP)
   {
      CU_FREE(rrcVer->iE_Extensions->list.array,\
            sizeof(struct RRC_Version_ExtIEs *));
      CU_FREE(rrcVer->iE_Extensions,\
            sizeof(ProtocolExtensionContainer_4624P81_t));
      CU_FREE(rrcVer->latest_RRC_Version.buf, sizeof(uint8_t));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
         CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
               sizeof(F1SetupResponseIEs_t));
      } 
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
            elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   rrcVer->iE_Extensions->list.array[0]->id = \
                                              ProtocolIE_ID_id_latest_RRC_Version_Enhanced;
   rrcVer->iE_Extensions->list.array[0]->criticality = Criticality_reject;
   rrcVer->iE_Extensions->list.array[0]->extensionValue.present = \
                                                                  RRC_Version_ExtIEs__extensionValue_PR_Latest_RRC_Version_Enhanced;
   rrcVer->iE_Extensions->list.array[0]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.size = 3*sizeof(uint8_t);
   CU_ALLOC(rrcVer->iE_Extensions->list.\
         array[0]->extensionValue.choice.Latest_RRC_Version_Enhanced.buf,\
         3*sizeof(uint8_t));
   if(rrcVer->iE_Extensions->list.\
         array[0]->extensionValue.choice.Latest_RRC_Version_Enhanced.buf == NULLP)
   {
      CU_FREE(rrcVer->iE_Extensions->list.array[0],\
            sizeof(struct RRC_Version_ExtIEs));
      CU_FREE(rrcVer->iE_Extensions->list.array,\
            sizeof(struct RRC_Version_ExtIEs *));
      CU_FREE(rrcVer->iE_Extensions,\
            sizeof(ProtocolExtensionContainer_4624P81_t));
      CU_FREE(rrcVer->latest_RRC_Version.buf, sizeof(uint8_t));
      CU_FREE(cuName->buf, sizeof(cuName->size));
      for(ieIdx=0; ieIdx<elementCnt; ieIdx++)
      {
         CU_FREE(f1SetupRsp->protocolIEs.list.array[ieIdx], \
               sizeof(F1SetupResponseIEs_t));
      } 
      CU_FREE(f1SetupRsp->protocolIEs.list.array,\
            elementCnt * sizeof(F1SetupResponseIEs_t *));
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }
   rrcVer->iE_Extensions->list.array[0]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.buf[0] = 0;
   rrcVer->iE_Extensions->list.array[0]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.buf[1] = 5;
   rrcVer->iE_Extensions->list.array[0]->extensionValue.choice.\
      Latest_RRC_Version_Enhanced.buf[2] = 15;

   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

   /* Encode the F1SetupRequest type as UPER */
   memset(encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf, encBuf);

   /* Clean up */
   if(rrcVer)
      CU_FREE(rrcVer->latest_RRC_Version.buf, sizeof(uint8_t));
   if(cuName)
      CU_FREE(cuName->buf, cuName->size);
   for(idx=0; idx<elementCnt; idx++)
   {
      CU_FREE(f1SetupRsp->protocolIEs.list.array[idx], sizeof(F1SetupResponseIEs_t));
   }             
   CU_FREE(f1SetupRsp->protocolIEs.list.array, elementCnt * sizeof(F1SetupResponseIEs_t *));
   CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
   CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));

   /* Check encode results */
   if(encRetVal.encoded == ENCODE_FAIL)
   {
      DU_LOG("\nERROR  -->  F1AP : Could not encode F1SetupResponse structure (at %s)\n",\
            encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
      return RFAILED;   
   } 
   else 
   {
      DU_LOG("\nDEBUG  -->  F1AP : Created APER encoded buffer for F1SetupResponse\n");
      for(int i=0; i< encBufSize; i++)
      {
         DU_LOG("%x",encBuf[i]);
      } 
   }

   /* Sending msg */
   if(SendF1APMsg(CU_APP_MEM_REG, CU_POOL, duId) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : Sending F1 Setup Response failed");      
      return RFAILED;
   }

   return ROK;
}/* End of BuildAndSendF1SetupRsp */

/*******************************************************************
 *
 * @brief Builds and sends the DUUpdateAcknowledge
 *
 * @details
 *
 *    Function : BuildAndSendDUUpdateAck
 *
 *    Functionality: Constructs the DU Update Acknowledge message and sends
 *                   it to the DU through SCTP.
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t BuildAndSendDUUpdateAck(uint32_t duId)
{
   uint8_t   idx;
   uint8_t   elementCnt;
   F1AP_PDU_t *f1apMsg = NULL;
   GNBDUConfigurationUpdateAcknowledge_t *gNBDuCfgAck;
   asn_enc_rval_t enRetVal; /* Encoder return value */

   DU_LOG("\nINFO  -->  F1AP : Building GNB-DU Config Update Ack\n");

   /* Allocate the memory for F1SetupRequest_t */
   CU_ALLOC(f1apMsg, (Size)sizeof(F1AP_PDU_t));
   if(f1apMsg == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation for F1AP-PDU failed");
      return RFAILED;
   }

   f1apMsg->present =  F1AP_PDU_PR_successfulOutcome;

   CU_ALLOC(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
   if(f1apMsg->choice.successfulOutcome == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation for F1AP-PDU failed");
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }

   f1apMsg->choice.successfulOutcome->procedureCode = ProcedureCode_id_gNBDUConfigurationUpdate;
   f1apMsg->choice.successfulOutcome->criticality = Criticality_reject;
   f1apMsg->choice.successfulOutcome->value.present = SuccessfulOutcome__value_PR_GNBDUConfigurationUpdateAcknowledge;
   gNBDuCfgAck = &f1apMsg->choice.successfulOutcome->value.choice.GNBDUConfigurationUpdateAcknowledge;

   elementCnt = 1;
   gNBDuCfgAck->protocolIEs.list.count = elementCnt;
   gNBDuCfgAck->protocolIEs.list.size = elementCnt*sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t);

   /* Initialize the F1Setup members */
   CU_ALLOC(gNBDuCfgAck->protocolIEs.list.array, elementCnt * sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t *));
   if(gNBDuCfgAck->protocolIEs.list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation for DuUpdateAcknowledgeIEs failed");
      CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      CU_FREE(f1apMsg,(Size)sizeof(F1AP_PDU_t));
      return RFAILED;
   }

   for(idx=0; idx<elementCnt; idx++)
   {
      CU_ALLOC(gNBDuCfgAck->protocolIEs.list.array[idx], sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t));
      if(gNBDuCfgAck->protocolIEs.list.array[idx] == NULLP)
      {
         CU_FREE(gNBDuCfgAck->protocolIEs.list.array, elementCnt * sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t *));
         CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
         CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
         return RFAILED;
      }
   }

   /*TransactionID*/ 
   idx = 0;
   gNBDuCfgAck->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_TransactionID ;
   gNBDuCfgAck->protocolIEs.list.array[idx]->criticality = Criticality_reject;
   gNBDuCfgAck->protocolIEs.list.array[idx]->value.present =\
   GNBDUConfigurationUpdateAcknowledgeIEs__value_PR_TransactionID;
   gNBDuCfgAck->protocolIEs.list.array[idx]->value.choice.TransactionID = TRANS_ID;

   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

   /* Encode the F1SetupRequest type as UPER */
   memset(encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   enRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf, encBuf);

   /* Clean up */
   for(idx=0; idx<elementCnt; idx++)
   {
      CU_FREE(gNBDuCfgAck->protocolIEs.list.array[idx], sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t));
   }
   CU_FREE(gNBDuCfgAck->protocolIEs.list.array, elementCnt * sizeof(GNBDUConfigurationUpdateAcknowledgeIEs_t *));
   CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
   CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));

   /* Checking encode results */
   if(enRetVal.encoded == ENCODE_FAIL) 
   {
      DU_LOG("\nERROR  -->  F1AP : Could not encode DUConfigUpdateAcknowledge structure (at %s)",\
      enRetVal.failed_type ? enRetVal.failed_type->name : "unknown");
      return RFAILED; 
   } 
   else 
   {
      DU_LOG("\nDEBUG  -->  F1AP : Created APER encoded buffer for DuConfigUpdateAcknowledge\n");  
      for(int i=0; i< encBufSize; i++)
      {
         DU_LOG("%x",encBuf[i]);
      } 
   }

   /* Sending msg */
   /* TODO : Hardcoding DU ID to 1 for messages other than F1 Setup Response. This will be made generic in future gerrit */
   if(SendF1APMsg(CU_APP_MEM_REG, CU_POOL, duId) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : Sending GNB-DU Config Update Ack failed");
      return RFAILED;
   }

   return ROK;

}/* End of BuildAndSendDUUpdateAck*/

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
   uint8_t idx = 0;
   Reset_t *f1ResetMsg = NULLP;

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
		  CU_FREE(f1ResetMsg->protocolIEs.list.array[idx],sizeof(ResetIEs_t));
	       }
	    }
	    CU_FREE(f1ResetMsg->protocolIEs.list.array,f1ResetMsg->protocolIEs.list.size);
	 }
	 CU_FREE(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      }
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
   }
}
/*******************************************************************
 *
 * @brief build ansld ans send f1reset msg 
 *
 * @details
 *
 *    Function : BuildAndSendF1ResetReq
 *
 *    Functionality: build and send f1reset msg 
 *
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
   DU_LOG("\nINFO  -->  F1AP : Building F1 Reset request \n");
   do
   {
      CU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation for the BuildAndSendF1ResetReq's F1AP-PDU failed"); 
         break;
      }
      f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
      CU_ALLOC(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      if(f1apMsg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation for BuildAndSendF1ResetReq failed");
         break;
      }
      f1apMsg->choice.initiatingMessage->procedureCode = ProcedureCode_id_Reset;
      f1apMsg->choice.initiatingMessage->criticality = Criticality_reject;
      f1apMsg->choice.initiatingMessage->value.present = InitiatingMessage__value_PR_Reset;

      f1ResetMsg = &f1apMsg->choice.initiatingMessage->value.choice.Reset;

      elementCnt = 3;
      f1ResetMsg->protocolIEs.list.count = elementCnt;
      f1ResetMsg->protocolIEs.list.size = elementCnt * sizeof(ResetIEs_t *);

      /* Initialize the F1Reset members */
      CU_ALLOC(f1ResetMsg->protocolIEs.list.array,f1ResetMsg->protocolIEs.list.size);
      if(f1ResetMsg->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed for BuildAndSendF1ResetReq");
         break;
      }
      for(idx=0; idx<elementCnt; idx++)
      {
         CU_ALLOC(f1ResetMsg->protocolIEs.list.array[idx],sizeof(ResetIEs_t));
         if(f1ResetMsg->protocolIEs.list.array[idx] == NULLP)
         {
            DU_LOG("\nERROR  -->  F1AP : Memory allocation failed for BuildAndSendF1ResetReq msg array");
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
         DU_LOG("\nDEBUG  -->  F1AP : Created APER encoded buffer for F1Reset\n");
         for(idx=0; idx< encBufSize; idx++)
         {
            DU_LOG("%x",encBuf[idx]);
         }
      }

      /* TODO : Hardcoding DU ID to 1 for messages other than F1 Setup Response. This will be made generic in future gerrit */
      if(SendF1APMsg(CU_APP_MEM_REG, CU_POOL, DU_ID) != ROK)
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
 * @brief Fills Radio Bearer Config 
 *
 * @details
 *
 *    Function : fillSrbCfg
 *
 *    Functionality: Fills Radio Bearer Config
 *
 * @params[in] SRB_ToAddModList *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillSrbCfg(CuUeCb *ueCb, SRB_ToAddModList_t *bearerCfg)
{
   uint8_t elementCnt = 0;
   uint8_t idx, ieId, srbIdx = 0;

   for(srbIdx=0; srbIdx < ueCb->numSrb; srbIdx++)
   {
      if(ueCb->srbList[srbIdx].cfgSentToUe == false)
         elementCnt++;
   }

   if(bearerCfg != NULLP)
   {
      bearerCfg->list.count = elementCnt;
      bearerCfg->list.size = elementCnt * sizeof(SRB_ToAddMod_t *);
      CU_ALLOC(bearerCfg->list.array, bearerCfg->list.size);
      if(bearerCfg->list.array != NULLP)
      {
         for(idx = 0; idx < elementCnt; idx++)
         {
            CU_ALLOC(bearerCfg->list.array[idx], sizeof(SRB_ToAddMod_t));
            if(bearerCfg->list.array[idx] == NULLP)
            {
               for(ieId = 0; ieId < idx; ieId++)
               {
                  CU_FREE(bearerCfg->list.array[ieId], sizeof(SRB_ToAddMod_t));
               }
               CU_FREE(bearerCfg->list.array, bearerCfg->list.size);
               return RFAILED;
            }
         }
      }
      else
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed for bearer list arry in fillSrbCfg");
         return RFAILED;
      }

      idx = 0;
      for(srbIdx=0; srbIdx < ueCb->numSrb; srbIdx++)
      {
         if(ueCb->srbList[srbIdx].cfgSentToUe == false)
            bearerCfg->list.array[idx++]->srb_Identity = ueCb->srbList[srbIdx].srbId;
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills Master CellGroup Info 
 *
 * @details
 *
 *    Function : fillMasterCellGroup
 *
 *    Functionality: Fills Master Cell Group IE
 *
 * @params[in] RRCSetup_IEs_t *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillMasterCellGroup(CuUeCb *ueCb, OCTET_STRING_t *masterCellGroup)
{
   uint8_t ret = ROK;
   masterCellGroup->buf = NULLP;
   if(ueCb->f1apMsgDb.duToCuContainer.buf)
   {
      masterCellGroup->size = ueCb->f1apMsgDb.duToCuContainer.size;
      CU_ALLOC(masterCellGroup->buf, masterCellGroup->size);
      if(masterCellGroup->buf != NULLP)
      {
         memcpy(masterCellGroup->buf, ueCb->f1apMsgDb.duToCuContainer.buf, masterCellGroup->size);
      }
      else
      {
         DU_LOG( "\nERROR  -->  F1AP : Master Cell Group buffer is NULL");
         ret = RFAILED;
      }
   }
   else
   {
      ret =  RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Fills RRC setup IE 
 *
 * @details
 *
 *    Function : fillRRCSetupIE
 *
 *    Functionality: Fills RRC Setup IE
 *
 * @params[in] RRCSetup_IEs_t *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillRRCSetupIE(CuUeCb *ueCb, RRCSetup_IEs_t *rrcSetupIE)
{
   uint8_t ret = ROK, srbIdx = 0;
   if(rrcSetupIE)
   {
      CU_ALLOC(rrcSetupIE->radioBearerConfig.srb_ToAddModList, sizeof(SRB_ToAddModList_t));
      if(rrcSetupIE->radioBearerConfig.srb_ToAddModList != NULLP)
      {
         ret = fillSrbCfg(ueCb, rrcSetupIE->radioBearerConfig.srb_ToAddModList);
      }		
      if(ret == ROK)
      {
         ret = fillCellGrpCfg(ueCb, &rrcSetupIE->masterCellGroup, false);
      }
      else
      {
         CU_FREE(rrcSetupIE->radioBearerConfig.srb_ToAddModList, sizeof(SRB_ToAddModList_t));
         ret = RFAILED;
      }
      
      /* If SRB configuration are filled successfully in RRC Setup, mark these
       * configurartion as sent to UE */
      for(srbIdx=0; srbIdx < ueCb->numSrb; srbIdx++)
      {
         if(ueCb->srbList[srbIdx].cfgSentToUe == false)
            ueCb->srbList[srbIdx].cfgSentToUe = true;
      }
   }
   return ret;
}
/*******************************************************************
 *
 * @brief Fills DL DCCCH Message required for DLRRCMessageTransfer
 *
 * @details
 *
 *    Function : fillDlCcchRrcMsg
 *
 *    Functionality: Fills DL DCCCH Message required for 
 *                   DLRRCMessageTransfer
 *
 * @params[in] RRCContainer_t *rrcContainer
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillDlCcchRrcMsg(CuUeCb *ueCb, RRCContainer_t *rrcContainer)
{
   uint8_t ret = ROK;
   uint16_t idx2;
   DL_CCCH_Message_t dl_CCCH_Msg;
   asn_enc_rval_t    encRetVal;

   if(rrcContainer != NULLP)
   {
      dl_CCCH_Msg.message.present = DL_CCCH_MessageType_PR_c1;

      CU_ALLOC(dl_CCCH_Msg.message.choice.c1 , sizeof(DL_CCCH_MessageType_t));
      if(dl_CCCH_Msg.message.choice.c1 != NULLP)
      {
         dl_CCCH_Msg.message.choice.c1->present = DL_CCCH_MessageType__c1_PR_rrcSetup;
         CU_ALLOC(dl_CCCH_Msg.message.choice.c1->choice.rrcSetup, sizeof(RRCSetup_t));
         if(dl_CCCH_Msg.message.choice.c1->choice.rrcSetup != NULLP)
         {
            dl_CCCH_Msg.message.choice.c1->choice.rrcSetup->rrc_TransactionIdentifier = 0;
            dl_CCCH_Msg.message.choice.c1->choice.rrcSetup->criticalExtensions.present = \
               RRCSetup__criticalExtensions_PR_rrcSetup;

            /* Fill RRC Setup IE */
            CU_ALLOC(dl_CCCH_Msg.message.choice.c1->choice.rrcSetup->criticalExtensions.choice.rrcSetup, \
               sizeof(RRCSetup_IEs_t));
            if(dl_CCCH_Msg.message.choice.c1->choice.rrcSetup->criticalExtensions.choice.rrcSetup != NULLP)
            {
               ret = fillRRCSetupIE(ueCb, dl_CCCH_Msg.message.choice.c1->choice.rrcSetup->criticalExtensions.choice.rrcSetup);

               if(ret == ROK)
               {
                  /* encode DL-CCCH message into RRC Container */
                  xer_fprint(stdout, &asn_DEF_DL_CCCH_MessageType, &dl_CCCH_Msg);
                  memset(encBuf, 0, ENC_BUF_MAX_LEN);
                  encBufSize = 0;
                  encRetVal = uper_encode(&asn_DEF_DL_CCCH_MessageType, 0, &dl_CCCH_Msg, PrepFinalEncBuf, encBuf);
                  /* Encode results */
                  if(encRetVal.encoded == ENCODE_FAIL)
                  {
                     DU_LOG( "\nERROR  -->  F1AP : Could not encode RRCContainer for DL-CCCH Msg(at %s)\n",\
                           encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
                     return RFAILED;
                  }
                  else
                  {
                     DU_LOG("\nDEBUG  -->  F1AP : Created APER encoded buffer for RRCContainer for DL-CCCH Msg\n");
                     for(int i = 0; i< encBufSize; i++)
                     {
                        DU_LOG("%x",encBuf[i]);
                     }
                     rrcContainer->size = encBufSize;
                     CU_ALLOC(rrcContainer->buf, rrcContainer->size);
                     if(rrcContainer->buf != NULLP)
                     {
                        memset(rrcContainer->buf, 0, encBufSize);
                        for(idx2 = 0; idx2 < encBufSize; idx2++)
                        {
                           rrcContainer->buf[idx2] =	encBuf[idx2];
                        }
                     }
                  }
               }
               else
               {
                  ret = RFAILED;
               }
            }
            else
            {
               DU_LOG("\nERROR  -->  F1AP: Memory Alloc failed for RRC Setup Msg at fillDlCcchRrcMsg()");
               ret = RFAILED;
            }
         }
         else
         {
            DU_LOG("\nERROR  -->  F1AP: Memory Alloc failed for RRC Msg at fillDlCcchRrcMsg()");
            ret = RFAILED;
         }
      }
      else
      {
	 DU_LOG("\nF1AP: Memory Alloc failed for DL Ccch Msg choice at fillDlCcchRrcMsg()");
	 ret = RFAILED;
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  F1AP: RRC Container is NULLP at fillDlCcchRrcMsg()");
      ret = RFAILED;
   }
}

/*******************************************************************
 *
 * @brief Fills QOS flow configuration  
 *
 * @details
 *
 *    Function : fillQosFlowsToAdd
 *
 *    Functionality: Fills QOS flow configuration
 *
 * @params[in] struct SDAP_Config__mappedQoS_FlowsToAdd *qosFlow 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillQosFlowsToAdd(struct SDAP_Config__mappedQoS_FlowsToAdd *qosFlow)
{
   uint8_t idx, ied, elementCnt;

   elementCnt = 1;
   qosFlow->list.count = elementCnt;
   qosFlow->list.size  = elementCnt * sizeof(QFI_t *);
   CU_ALLOC(qosFlow->list.array, qosFlow->list.size);
   if(qosFlow->list.array != NULLP)
   {
      for(idx = 0; idx < elementCnt; idx++)
      {
	 CU_ALLOC(qosFlow->list.array[idx], sizeof(QFI_t));
	 if(qosFlow->list.array[idx] == NULLP)
	 {
	    for(ied = 0; ied < idx; ied++)
	    {
	       CU_FREE(qosFlow->list.array[idx], sizeof(QFI_t));
	    }
	    CU_FREE(qosFlow->list.array, qosFlow->list.size);
	    return RFAILED;
	 }
      }
   }
   idx = 0;
   *qosFlow->list.array[idx] = 9;
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills CN Assoc for Drb to Add/Mod List
 *
 * @details
 *
 *    Function : fillCnAssoc
 *
 *    Functionality: Fills CN Assoc for Drb to Add/Mod List
 *
 * @params[in] struct DRB_ToAddMod__cnAssociation *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillCnAssoc(struct DRB_ToAddMod__cnAssociation *cnAssoc)
{
   uint8_t ret = ROK;

   cnAssoc->present = DRB_ToAddMod__cnAssociation_PR_sdap_Config;
   if(cnAssoc->present == DRB_ToAddMod__cnAssociation_PR_eps_BearerIdentity)
   {
      cnAssoc->choice.eps_BearerIdentity = 5;
   }
   if(cnAssoc->present == DRB_ToAddMod__cnAssociation_PR_sdap_Config)
   {
      CU_ALLOC(cnAssoc->choice.sdap_Config, sizeof(SDAP_Config_t));
      if(cnAssoc->choice.sdap_Config)
      {
	 cnAssoc->choice.sdap_Config->pdu_Session = 5;
	 cnAssoc->choice.sdap_Config->sdap_HeaderDL = 0;
	 cnAssoc->choice.sdap_Config->sdap_HeaderUL = 0;
	 cnAssoc->choice.sdap_Config->defaultDRB = true;
	 cnAssoc->choice.sdap_Config->mappedQoS_FlowsToAdd = NULLP;
	 cnAssoc->choice.sdap_Config->mappedQoS_FlowsToRelease = NULLP;
	 CU_ALLOC(cnAssoc->choice.sdap_Config->mappedQoS_FlowsToAdd, \
	       sizeof(struct SDAP_Config__mappedQoS_FlowsToAdd));
	 if(cnAssoc->choice.sdap_Config->mappedQoS_FlowsToAdd)
	 {
	    ret = fillQosFlowsToAdd(cnAssoc->choice.sdap_Config->mappedQoS_FlowsToAdd);
	 }
	 else
	 {
	    DU_LOG("\nERROR  -->  F1AP: Memory alloc failed at mappedQoS_FlowsToAdd in fillCnAssoc()");
	    CU_FREE(cnAssoc->choice.sdap_Config, sizeof(SDAP_Config_t));
	    ret = RFAILED;
	 }
      }
      else
      {
	 DU_LOG("\nERROR  -->  F1AP: Mem alloc failed at fillCnAssoc()");
	 ret = RFAILED;
      }
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Fills Radio Bearer Config for Drb 
 *
 * @details
 *
 *    Function : fillDrbCfg
 *
 *    Functionality: Fills Radio Bearer Config for Drb
 *
 * @params[in] drbId, DRB_ToAddModList *
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillDrbCfg(uint8_t drbId, DRB_ToAddModList_t *drbCfg)
{
   uint8_t idx, ied, ret, elementCnt;

   ret = ROK;
   if(drbCfg != NULLP)
   {
      elementCnt = 1;
      drbCfg->list.count = elementCnt;
      drbCfg->list.size =\
			 elementCnt * sizeof(DRB_ToAddMod_t *);
      CU_ALLOC(drbCfg->list.array, drbCfg->list.size);
      if(drbCfg->list.array != NULLP)
      {
	 for(idx = 0; idx < elementCnt; idx++)
	 {
	    CU_ALLOC(drbCfg->list.array[idx], sizeof(DRB_ToAddMod_t));
	    if(drbCfg->list.array[idx] == NULLP)
	    {
	       for(ied = 0; ied < idx; ied++)
	       {
		  CU_FREE(drbCfg->list.array[idx], sizeof(DRB_ToAddMod_t));
	       }
	       CU_FREE(drbCfg->list.array, drbCfg->list.size);
	       return RFAILED;
	    }
	 }
      }
      else
      {
	 return RFAILED;
      }
      idx = 0;
      /* CN ASSOCIATION */
      CU_ALLOC(drbCfg->list.array[idx]->cnAssociation, sizeof(struct DRB_ToAddMod__cnAssociation));
      if(drbCfg->list.array[idx]->cnAssociation)
      {
	 ret = fillCnAssoc(drbCfg->list.array[idx]->cnAssociation);
      }
      /* DRB */
      drbCfg->list.array[idx]->drb_Identity = drbId;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Fills RRC Reconfig Message required for DLRRCMessageTransfer
 *
 * @details
 *
 *    Function : fillRrcReconfigIE
 *
 *    Functionality: Fills RRC Reconfig Message required for 
 *                   DLRRCMessageTransfer
 *
 * @params[in] RRCReconfiguration_IEs_t* rrcReconfig
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillRrcReconfigIE(RRCReconfiguration_IEs_t *rrcReconfigMsg)
{
   uint8_t ret = ROK;
   CU_ALLOC(rrcReconfigMsg->radioBearerConfig, sizeof(RadioBearerConfig_t));
   if(rrcReconfigMsg->radioBearerConfig)
   {
      CU_ALLOC(rrcReconfigMsg->radioBearerConfig->srb_ToAddModList, sizeof(SRB_ToAddModList_t));
      if(rrcReconfigMsg->radioBearerConfig->srb_ToAddModList != NULLP)
      {
	 ret = fillSrbCfg(SRB2, rrcReconfigMsg->radioBearerConfig->srb_ToAddModList);

      }
      if(ret == ROK)
      {
	 CU_ALLOC(rrcReconfigMsg->radioBearerConfig->drb_ToAddModList, sizeof(DRB_ToAddModList_t));
	 if(rrcReconfigMsg->radioBearerConfig->drb_ToAddModList != NULLP)
	 {
	    ret = fillDrbCfg(DRB1, rrcReconfigMsg->radioBearerConfig->drb_ToAddModList);
	    if(ret == RFAILED)
	    {
	       DU_LOG("\nERROR  -->  F1AP : Failed to fill DrbCfg at fillRrcReconfigIE()");
	       CU_FREE(rrcReconfigMsg->radioBearerConfig->srb_ToAddModList, sizeof(SRB_ToAddModList_t));
	       CU_FREE(rrcReconfigMsg->radioBearerConfig->drb_ToAddModList, sizeof(DRB_ToAddModList_t));
	    }
	 }
      }
      else
      {
	 DU_LOG("\nERROR  -->  F1AP : memory Alloc failed at fillRrcReconfigIE()");
	 CU_FREE(rrcReconfigMsg->radioBearerConfig->srb_ToAddModList, sizeof(SRB_ToAddModList_t));
      }
   }

   return ret;
}
/*******************************************************************
 *
 * @brief Fills DL DCCH Message required for DLRRCMessageTransfer
 *
 * @details
 *
 *    Function : fillDlDcchRrcMsg
 *
 *    Functionality: Fills DL DCCH Message required for 
 *                   DLRRCMessageTransfer
 *
 * @params[in] RRCContainer_t *rrcContainer
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillDlDcchRrcMsg(CuUeCb *ueCb, RRCContainer_t *rrcContainer, bool updateAllRbCfg)
{
   uint8_t ret = ROK;
   uint16_t idx2 = 0, drbIdx = 0, srbIdx = 0;
   DL_DCCH_Message_t dl_DCCH_Msg;
   memset(&dl_DCCH_Msg, 0, sizeof(DL_DCCH_Message_t));
   asn_enc_rval_t        encRetVal;

   if(rrcContainer != NULLP)
   {
      dl_DCCH_Msg.message.present = DL_DCCH_MessageType_PR_c1;

      CU_ALLOC(dl_DCCH_Msg.message.choice.c1 , sizeof(DL_DCCH_MessageType_t));
      if(dl_DCCH_Msg.message.choice.c1 != NULLP)
      {
         dl_DCCH_Msg.message.choice.c1->present = DL_DCCH_MessageType__c1_PR_rrcReconfiguration;
         CU_ALLOC(dl_DCCH_Msg.message.choice.c1->choice.rrcReconfiguration, sizeof(RRCReconfiguration_t));
         if(dl_DCCH_Msg.message.choice.c1->choice.rrcReconfiguration != NULLP)
         {
            DU_LOG("\nDEBUG --> F1AP : Filling DL DCCH RRC Reconfiguration Message ");
            fillRrcReconfig(ueCb, dl_DCCH_Msg.message.choice.c1->choice.rrcReconfiguration, updateAllRbCfg);
            if(ret == ROK)
            {
               /* If RB configuration are filled successfully in RRC Reconfiguration, mark these
                * configurartion as sent to UE */
               for(srbIdx=0; srbIdx < ueCb->numSrb; srbIdx++)
               {     
                  if(ueCb->srbList[srbIdx].cfgSentToUe == false)
                     ueCb->srbList[srbIdx].cfgSentToUe = true; 
               }
               for(drbIdx=0; drbIdx < ueCb->numDrb; drbIdx++)
               {
                  if(ueCb->drbList[drbIdx].cfgSentToUe == false)
                     ueCb->drbList[drbIdx].cfgSentToUe = true;
               }

               /* encode DL-DCCH message into RRC Container */
               xer_fprint(stdout, &asn_DEF_DL_DCCH_MessageType, &dl_DCCH_Msg);
               memset(encBuf, 0, ENC_BUF_MAX_LEN);
               encBufSize = 0;
               encRetVal = uper_encode(&asn_DEF_DL_DCCH_MessageType, 0, &dl_DCCH_Msg, PrepFinalEncBuf, encBuf);
               /* Encode results */
               if(encRetVal.encoded == ENCODE_FAIL)
               {
                  DU_LOG( "\nERROR  -->  F1AP : Could not encode RRCContainer for DL-DCCH Msg (at %s)\n",\
                        encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
                  return RFAILED;
               }
               else
               {
                  DU_LOG("\nDEBUG  -->  F1AP : Created APER encoded buffer for RRCContainer for DL-DCCH Msg\n");
                  for(int i = 0; i< encBufSize; i++)
                  {
                     DU_LOG("%x",encBuf[i]);
                  }
                  rrcContainer->size = encBufSize;
                  CU_ALLOC(rrcContainer->buf, rrcContainer->size);
                  if(rrcContainer->buf != NULLP)
                  {
                     memset(rrcContainer->buf, 0, encBufSize);
                     for(idx2 = 0; idx2 < encBufSize; idx2++)
                     {
                        rrcContainer->buf[idx2] =	encBuf[idx2];
                     }
                  }
               }
            }
         }
         else
         {
            DU_LOG("\nERROR  -->  F1AP: Memory Alloc failed for RRC Msg at fillDlDcchRrcMsg()");
            ret = RFAILED;
         }
      }
      else
      {
         DU_LOG("\nERROR  -->  F1AP: Memory Alloc failed for DL Dcch Msg choice at fillDlDcchRrcMsg()");
         ret = RFAILED;
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  F1AP: RRC Container is NULLP at fillDlDcchRrcMsg()");
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Builds RRC Container IE required for DLRRCMessageTransfer
 *
 * @details
 *
 *    Function : BuildDLRRCContainer
 *
 *    Functionality: Builds RRC Container IE required for 
 *                   DLRRCMessageTransfer
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t	BuildDLRRCContainer(CuUeCb *ueCb, uint8_t rrcMsgType, RRCContainer_t *rrcContainer)
{
   uint8_t ret, bufLen;

   ret =ROK;
   if(rrcMsgType == RRC_SETUP)
   { 
      ret = fillDlCcchRrcMsg(ueCb, rrcContainer);
      if(ret == RFAILED)
         DU_LOG("\nERROR  -->  F1AP: Failed to fill DL-CCCH Msg at RRC SETUP");
   }
   else if(rrcMsgType == RRC_SETUP_COMPLETE)
   {
      DU_LOG("\nINFO --> F1AP : Sending NAS Authentication Request");
      char authReqBuf[51] = {0x00, ueCb->pdcpSn++, 0x2a, 0x85, 0x4f, 0xc0, 0x0a, 0xc0, 0x40, 0x40, 0x00, 0x04, 0x2f, 0x58, 0x39, 0x12,\
                             0x37, 0x15, 0x74, 0x16, 0x4c, 0x8d, 0xd3, 0x95, 0xa6, 0x9e, 0x94, 0x03, 0x24, 0x02, 0x00, 0x6f,\
                             0x0b, 0x39, 0x5e, 0xbc, 0xb0, 0x00, 0x1a, 0xb1, 0xc1, 0x3f, 0x84, 0x4f, 0x77, 0xe3, 0x20, 0x00,\
                             0x00, 0x00, 0x00};
      bufLen = 51;
      rrcContainer->size = bufLen;
      CU_ALLOC(rrcContainer->buf, rrcContainer->size);
      if(rrcContainer->buf != NULLP)
      {
         memset(rrcContainer->buf, 0, bufLen); 
         memcpy(rrcContainer->buf, authReqBuf, bufLen);
      }
      else
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure for RRC Container buffer");
         ret = RFAILED;

      }     
   }
   else if(rrcMsgType == NAS_AUTHENTICATION_RSP)
   {
      DU_LOG("\nINFO --> F1AP : Sending NAS Security mode command");
      char secModeBuf[30]={0x00, ueCb->pdcpSn++, 0x2e, 0x82, 0xaf, 0xc0, 0x7d, 0x1c, 0x4e, 0xfc, 0x80, 0x0f, 0xc0, 
                          0x0b, 0xa0, 0x20, 0x40, 0x9e, 0x0e, 0x1e, 0x0e, 0x1c, 0x26, 0xc0, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00};
      bufLen =30;
      rrcContainer->size = bufLen;
      CU_ALLOC(rrcContainer->buf, rrcContainer->size);
      if(rrcContainer->buf != NULLP)
      {     
         memset(rrcContainer->buf, 0, bufLen);
         memcpy(rrcContainer->buf, secModeBuf, bufLen);
      }
      else
      {     
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure for RRC Container buffer");
         ret = RFAILED;
      }     
   }
   else if(rrcMsgType == NAS_SECURITY_MODE_COMPLETE)
   {
      DU_LOG("\nINFO --> F1AP : Sending RRC Security mode command");
      char secModeBuf[9]={0x00, ueCb->pdcpSn++, 0x22, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00};
      bufLen =9;
      rrcContainer->size = bufLen;
      CU_ALLOC(rrcContainer->buf, rrcContainer->size);
      if(rrcContainer->buf != NULLP)
      {
         memset(rrcContainer->buf, 0, bufLen);
         memcpy(rrcContainer->buf, secModeBuf, bufLen);
      }
      else
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure for RRC Container buffer");
         ret = RFAILED;
      }
   }
   else if(rrcMsgType == RRC_SECURITY_MODE_COMPLETE)
   {
      /*Hardcoded RRC Container from reference logs*/
      DU_LOG("\nINFO --> F1AP : Sending Registration accept");
      char buf[14] ={0x00, ueCb->pdcpSn++, 0x2a, 0x80, 0xaf, 0xc0, 0x08, 0x40, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00};
      bufLen =14;
      rrcContainer->size = bufLen;
      CU_ALLOC(rrcContainer->buf, rrcContainer->size);
      if(rrcContainer->buf != NULLP)
      {
         memset(rrcContainer->buf, 0, bufLen);
         memcpy(rrcContainer->buf, buf, bufLen);
      }
      else
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure for RRC Container buffer");
         ret = RFAILED;
      }
   }
   else if(rrcMsgType == UE_CONTEXT_SETUP_RSP)
   {
      uint16_t tmpBufIdx = 0, bufIdx = 0;
      RRCContainer_t rrcContainerTmp;
      DU_LOG("\nINFO --> F1AP : Filling DL DCCH RRC Message for RRC Reconfiguration ");
      ret = fillDlDcchRrcMsg(ueCb, &rrcContainerTmp, false);
      rrcContainer->size = rrcContainerTmp.size + 2;
      CU_ALLOC(rrcContainer->buf, rrcContainer->size);
      if(rrcContainer->buf != NULLP)
      {
         memset(rrcContainer->buf, 0, rrcContainer->size);
         rrcContainer->buf[0] = 0x00;
         rrcContainer->buf[1] = ueCb->pdcpSn++; //PDCP SN
         for(bufIdx = 2, tmpBufIdx = 0; bufIdx < rrcContainer->size; bufIdx++, tmpBufIdx++)
         {
            rrcContainer->buf[bufIdx] = rrcContainerTmp.buf[tmpBufIdx];
         }
      }
      if(ret == RFAILED)
         DU_LOG("\nERROR  -->  F1AP: Failed to fill DL-DCCH Msg for RRC Reconfiguration");
   }

   return ret;
}

/*******************************************************************
 *
 * @brief Frees the DLRRCMessageTransfer 
 *
 * @details
 *
 *    Function : freeDlRrcMessageTransfer
 *
 *    Functionality: Frees the DLRRCMessageTransfer 
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void freeDlRrcMessageTransfer(F1AP_PDU_t *f1apMsg)
{
   uint8_t idx=0;
   DLRRCMessageTransfer_t  *dlRRCMsg = NULLP;

   if(f1apMsg)
   {
      dlRRCMsg = &f1apMsg->choice.initiatingMessage->value.choice.DLRRCMessageTransfer;
      if(dlRRCMsg->protocolIEs.list.array)
      {
         idx = 3;
         CU_FREE(dlRRCMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf, \
            dlRRCMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.size);
         for(idx=0; idx<dlRRCMsg->protocolIEs.list.count; idx++)
         {
            CU_FREE(dlRRCMsg->protocolIEs.list.array[idx], sizeof(DLRRCMessageTransferIEs_t));
         }
         CU_FREE(dlRRCMsg->protocolIEs.list.array, dlRRCMsg->protocolIEs.list.size);
      }
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Builds and sends the DLRRCMessageTransfer 
 *
 * @details
 *
 *    Function : BuildAndSendDLRRCMessageTransfer
 *
 *    Functionality: Constructs the DL RRC Message Transfer and sends
 *                   it to the CU through SCTP.
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendDLRRCMessageTransfer(uint32_t duId, CuUeCb *ueCb, uint8_t srbId, uint8_t rrcMsgType)
{
   uint8_t   elementCnt = 0;
   uint8_t  ieId;
   uint8_t  idx;
   F1AP_PDU_t  *f1apMsg = NULLP;
   DLRRCMessageTransfer_t  *dlRRCMsg = NULLP;
   asn_enc_rval_t   encRetVal;        /* Encoder return value */

   DU_LOG("\nINFO  -->  F1AP : Building DL RRC Message Transfer Message\n");

   CU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
   if(f1apMsg == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation for F1AP-PDU failed");
      return RFAILED;
   }

   f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
   CU_ALLOC(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
   if(f1apMsg->choice.initiatingMessage == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation for  F1AP-PDU failed");
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }

   f1apMsg->choice.initiatingMessage->procedureCode = ProcedureCode_id_DLRRCMessageTransfer;
   f1apMsg->choice.initiatingMessage->criticality = Criticality_ignore;
   f1apMsg->choice.initiatingMessage->value.present = InitiatingMessage__value_PR_DLRRCMessageTransfer;
   dlRRCMsg = &f1apMsg->choice.initiatingMessage->value.choice.DLRRCMessageTransfer;

   elementCnt = 4;
   dlRRCMsg->protocolIEs.list.count = elementCnt;
   dlRRCMsg->protocolIEs.list.size = elementCnt * sizeof(DLRRCMessageTransferIEs_t *);

   /* Initialize the F1Setup members */
   CU_ALLOC(dlRRCMsg->protocolIEs.list.array, dlRRCMsg->protocolIEs.list.size);
   if(dlRRCMsg->protocolIEs.list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation for DL RRC MessageTransferIEs failed");
      CU_FREE(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
      return RFAILED;
   }

   for(idx=0; idx<elementCnt; idx++)
   {
      CU_ALLOC(dlRRCMsg->protocolIEs.list.array[idx], sizeof(DLRRCMessageTransferIEs_t));
      if(dlRRCMsg->protocolIEs.list.array[idx] == NULLP)
      {
         for(ieId=0; ieId<idx; ieId++)
         {
            CU_FREE(dlRRCMsg->protocolIEs.list.array[ieId], sizeof(DLRRCMessageTransferIEs_t));
         }
         CU_FREE(dlRRCMsg->protocolIEs.list.array, dlRRCMsg->protocolIEs.list.size);
         CU_FREE(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
         CU_FREE(f1apMsg,sizeof(F1AP_PDU_t));
         return RFAILED;
      }
   }

   /* GNB CU UE F1AP ID */
   idx = 0;
   dlRRCMsg->protocolIEs.list.array[idx]->id  = ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;
   dlRRCMsg->protocolIEs.list.array[idx]->criticality  = Criticality_reject;
   dlRRCMsg->protocolIEs.list.array[idx]->value.present = DLRRCMessageTransferIEs__value_PR_GNB_CU_UE_F1AP_ID;
   dlRRCMsg->protocolIEs.list.array[idx]->value.choice.GNB_CU_UE_F1AP_ID = ueCb->gnbCuUeF1apId;

   /* GNB DU UE F1AP ID */
   idx++;
   dlRRCMsg->protocolIEs.list.array[idx]->id  = ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
   dlRRCMsg->protocolIEs.list.array[idx]->criticality  = Criticality_reject;
   dlRRCMsg->protocolIEs.list.array[idx]->value.present = DLRRCMessageTransferIEs__value_PR_GNB_DU_UE_F1AP_ID;
   dlRRCMsg->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID = ueCb->gnbDuUeF1apId;;

   /* SRBID */
   idx++;
   dlRRCMsg->protocolIEs.list.array[idx]->id  = ProtocolIE_ID_id_SRBID;
   dlRRCMsg->protocolIEs.list.array[idx]->criticality  = Criticality_reject;
   dlRRCMsg->protocolIEs.list.array[idx]->value.present = DLRRCMessageTransferIEs__value_PR_SRBID;
   dlRRCMsg->protocolIEs.list.array[idx]->value.choice.SRBID = srbId;

   /* RRCContainer */
   idx++;
   dlRRCMsg->protocolIEs.list.array[idx]->id  = ProtocolIE_ID_id_RRCContainer;
   dlRRCMsg->protocolIEs.list.array[idx]->criticality = Criticality_reject;
   dlRRCMsg->protocolIEs.list.array[idx]->value.present = DLRRCMessageTransferIEs__value_PR_RRCContainer;
   BuildDLRRCContainer(ueCb, rrcMsgType, &dlRRCMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer);

   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

   /* Encode the F1SetupRequest type as APER */
   memset(encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
         encBuf);
   /* Encode results */
   if(encRetVal.encoded == ENCODE_FAIL)
   {
      DU_LOG( "\nERROR  -->  F1AP : Could not encode DL RRC Message Transfer structure (at %s)\n",\
            encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
      return RFAILED;
   }
   else
   {
      DU_LOG("\nDEBUG  -->  F1AP : Created APER encoded buffer for DL RRC Message transfer\n");
      for(int i=0; i< encBufSize; i++)
      {
         DU_LOG("%x",encBuf[i]);
      }
   }

   /* Sending  msg  */
   if(SendF1APMsg(CU_APP_MEM_REG,CU_POOL, duId)  !=  ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : Sending  DL RRC Message Transfer Failed");
      return RFAILED;
   }
   freeDlRrcMessageTransfer(f1apMsg);
   return ROK;
}/* End of BuildAndSendDLRRCMessageTransfer */

/*******************************************************************
 *
 * @brief Function to set the Dl RRC Msg Type
 *
 * @details
 *
 *    Function : setDlRRCMsgType
 *
 *    Functionality: Constructs the UE Setup Response and sends
 *                   it to the DU through SCTP.
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t setDlRRCMsgType(CuUeCb *ueCb)
{
   uint8_t rrcMsgType = 0;
   switch(ueCb->f1apMsgDb.dlRrcMsgCount)
   {
      case RRC_SETUP:
         rrcMsgType = RRC_SETUP;
         break;
      case RRC_SETUP_COMPLETE:
         rrcMsgType = RRC_SETUP_COMPLETE;
         break;
      case NAS_AUTHENTICATION_RSP:
         rrcMsgType = NAS_AUTHENTICATION_RSP;
         break;
      case NAS_SECURITY_MODE_COMPLETE:
         rrcMsgType = NAS_SECURITY_MODE_COMPLETE;
         break;
      case RRC_SECURITY_MODE_COMPLETE:
         rrcMsgType = RRC_SECURITY_MODE_COMPLETE;
         break;
      case REGISTRATION_COMPLETE:
         rrcMsgType = REGISTRATION_COMPLETE;
         break;
      case UE_CONTEXT_SETUP_RSP:
         rrcMsgType = UE_CONTEXT_SETUP_RSP;
         break;
      case RRC_RECONFIG_COMPLETE:
         rrcMsgType = RRC_RECONFIG_COMPLETE;
         break;
      default:
         break;
   }
   return rrcMsgType;   
}

#ifdef NR_DRX
/*******************************************************************
 *
 * @brief fill long cycle offset value of drx
 *
 * @details
 *
 *    Function : fillLongCycleOffsetValue
 *
 *    Functionality: fill long cycle offset value of drx
 *
 * @params[in] DrxLongCycleStartOffset  drxLongCycleStartOffset,
 * struct DRX_ConfigRrc__drx_LongCycleStartOffset recvedLongCycleOffsetVal 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void fillLongCycleOffsetValue(DrxLongCycleStartOffset *drxLongCycleStartOffset, struct DRX_ConfigRrc__drx_LongCycleStartOffset *recvedLongCycleOffsetVal)
{

   drxLongCycleStartOffset->drxLongCycleStartOffsetChoice = recvedLongCycleOffsetVal->present;
   switch(recvedLongCycleOffsetVal->present)
   {
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms10:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = recvedLongCycleOffsetVal->choice.ms10;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms20:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = recvedLongCycleOffsetVal->choice.ms20;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms32:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = recvedLongCycleOffsetVal->choice.ms32;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms40:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = recvedLongCycleOffsetVal->choice.ms40;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms60:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = recvedLongCycleOffsetVal->choice.ms60;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms64:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = recvedLongCycleOffsetVal->choice.ms64;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms70:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = recvedLongCycleOffsetVal->choice.ms70;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms80:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = recvedLongCycleOffsetVal->choice.ms80;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms128:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = recvedLongCycleOffsetVal->choice.ms128;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms160:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = recvedLongCycleOffsetVal->choice.ms160;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms256:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = recvedLongCycleOffsetVal->choice.ms256;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms320:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = recvedLongCycleOffsetVal->choice.ms320;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms512:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = recvedLongCycleOffsetVal->choice.ms512;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms640:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = recvedLongCycleOffsetVal->choice.ms640;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms1024:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = recvedLongCycleOffsetVal->choice.ms1024;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms1280:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = recvedLongCycleOffsetVal->choice.ms1280;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms2048:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = recvedLongCycleOffsetVal->choice.ms2048;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms2560:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = recvedLongCycleOffsetVal->choice.ms2560;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms5120:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = recvedLongCycleOffsetVal->choice.ms5120;
            break;
         }
      case DRX_ConfigRrc__drx_LongCycleStartOffset_PR_ms10240:
         {
            drxLongCycleStartOffset->drxLongCycleStartOffsetVal = recvedLongCycleOffsetVal->choice.ms10240;
            break;
         }
      default :
         break;
   }
}

/*******************************************************************
 *
 * @brief Extract configuration from DRX_ConfigRrc 
 *    and store the drx configuration in UeCb
 *
 * @details
 *
 *    Function : storeDrxCfgInUeCb 
 *
 *    Functionality: Store drx configuration in UeCb 
 *
 * @params[in] (struct DRX_ConfigRrc *setup, DrxCfg *drxCfg) 
 *
 * @return void 
 * ****************************************************************/
void storeDrxCfgInUeCb(struct DRX_ConfigRrc *drxSetup, DrxCfg *drxCfg)
{
   switch(drxSetup->drx_onDurationTimer.present)
   {
      case DRX_ConfigRrc__drx_onDurationTimer_PR_NOTHING:
         break;
      case DRX_ConfigRrc__drx_onDurationTimer_PR_milliSeconds:
         {
            drxCfg->drxOnDurationTimer.onDurationTimerValInMs = true;
            drxCfg->drxOnDurationTimer.onDurationtimerValue.milliSeconds=drxSetup->drx_onDurationTimer.choice.milliSeconds;
            break;
         }
      case DRX_ConfigRrc__drx_onDurationTimer_PR_subMilliSeconds:
         {
            drxCfg->drxOnDurationTimer.onDurationTimerValInMs = false;
            drxCfg->drxOnDurationTimer.onDurationtimerValue.subMilliSeconds = drxSetup->drx_onDurationTimer.choice.subMilliSeconds;
            break;
         }
   }
   fillLongCycleOffsetValue(&drxCfg->drxLongCycleStartOffset, &drxSetup->drx_LongCycleStartOffset);
   drxCfg->drxInactivityTimer = drxSetup->drx_InactivityTimer;
   drxCfg->drxHarqRttTimerDl = drxSetup->drx_HARQ_RTT_TimerDL;
   drxCfg->drxHarqRttTimerUl = drxSetup->drx_HARQ_RTT_TimerUL;
   drxCfg->drxRetransmissionTimerDl = drxSetup->drx_RetransmissionTimerDL;
   drxCfg->drxRetransmissionTimerUl = drxSetup->drx_RetransmissionTimerUL;
   drxCfg->drxSlotOffset = drxSetup->drx_SlotOffset;
   if(drxSetup->shortDRX) 
   {
      drxCfg->shortDrxPres=true;
      drxCfg->shortDrx.drxShortCycle = drxSetup->shortDRX->drx_ShortCycle;
      drxCfg->shortDrx.drxShortCycleTimer = drxSetup->shortDRX->drx_ShortCycleTimer;
   }
   else
      drxCfg->shortDrxPres=false;
}
#endif

/*******************************************************************
 *
 * @brief Extract configuration from CellGroupConfig
 *
 * @details
 *
 *    Function : extractCellGroupConfig
 *
 *    Functionality: Extract configuration from CellGroupConfig
 *        and store in local database
 *
 * @params[in] UE control block
 *             Cell Group Config 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t extractCellGroupConfig(CuUeCb *ueCb, CellGroupConfigRrc_t *cellGrpCfg)
{
   uint8_t rbIdx, srbIdx, drbIdx;
   bool    srbFound, drbFound;
   SrbInfo *srbCfgDb = NULLP;
   DrbInfo *drbCfgDb = NULLP;
   RlcLcCfg *rlcLcCfgDb = NULLP;
   MacLcCfg *macLcCfgDb = NULLP;
   RLC_BearerConfig_t *rlcCfg = NULLP;
   RLC_Config_t *rlcLcCfg = NULLP;
   LogicalChannelConfig_t *macLcCfg = NULLP;
#ifdef NR_DRX
   DrxCfg    drxCfg;
#endif

   if(ueCb == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP: extractCellGroupConfig(): UE Cb is NULL");
      return RFAILED;
   }

   if(cellGrpCfg == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP: extractCellGroupConfig(): cellGrpCfg is NULL");
      return RFAILED;
   }

#ifdef NR_DRX
   if(cellGrpCfg->mac_CellGroupConfig)
   {
      if(cellGrpCfg->mac_CellGroupConfig->drx_ConfigRrc)
      {
         switch(cellGrpCfg->mac_CellGroupConfig->drx_ConfigRrc->present)
         {
            case MAC_CellGroupConfig__drx_ConfigRrc_PR_NOTHING:
               break;

            case MAC_CellGroupConfig__drx_ConfigRrc_PR_setup:
            {
               if(cellGrpCfg->mac_CellGroupConfig->drx_ConfigRrc->choice.setup)
               {
                  ueCb->drxCfgPresent = true;  
                  storeDrxCfgInUeCb(cellGrpCfg->mac_CellGroupConfig->drx_ConfigRrc->choice.setup, &ueCb->drxCfg);
               }
               break;
            }

            case MAC_CellGroupConfig__drx_ConfigRrc_PR_release:
               break;
         }
      }
   }
#endif

   for(rbIdx = 0; rbIdx < cellGrpCfg->rlc_BearerToAddModList->list.count; rbIdx++)
   {
      srbFound = false;
      drbFound = false;

      rlcCfg = cellGrpCfg->rlc_BearerToAddModList->list.array[rbIdx];

      /* Update SRB configuration in local DB */
      if(rlcCfg->servedRadioBearer->present == RLC_BearerConfig__servedRadioBearer_PR_srb_Identity)
      {
         /* Search if SRB entry is already present in DB */
         for(srbIdx = 0; srbIdx < ueCb->numSrb; srbIdx++)
         {
            if(ueCb->srbList[srbIdx].srbId == rlcCfg->servedRadioBearer->choice.srb_Identity)
            {
              srbCfgDb = &ueCb->srbList[srbIdx];
              srbFound = true; 
              break;
            }
         }

         /* If not, add SRB to UE CB's SRB list */
         if(!srbFound)
         {
            ueCb->srbList[ueCb->numSrb].srbId = rlcCfg->servedRadioBearer->choice.srb_Identity;
            srbCfgDb = &ueCb->srbList[ueCb->numSrb];
            ueCb->numSrb++;
         }

         srbCfgDb->lcId = rlcCfg->logicalChannelIdentity;
         srbCfgDb->cfgSentToUe = false;
         rlcLcCfgDb = &srbCfgDb->rlcLcCfg;
         macLcCfgDb = &srbCfgDb->macLcCfg;
      }

      /* Update DRB configuration in local DB */
      if(rlcCfg->servedRadioBearer->present == RLC_BearerConfig__servedRadioBearer_PR_drb_Identity)
      {     
         /* Search if DRB entry is already present in DB */
         for(drbIdx = 0; drbIdx < ueCb->numDrb; drbIdx++)
         {
            if(ueCb->drbList[drbIdx].drbId == rlcCfg->servedRadioBearer->choice.drb_Identity)
            {
              drbCfgDb = &ueCb->drbList[drbIdx];
              drbFound = true; 
              break;
            }
         }

         /* If not, add DRB to UE CB's SRB list */
         if(!drbFound)
         {
            ueCb->drbList[ueCb->numDrb].drbId = rlcCfg->servedRadioBearer->choice.drb_Identity;
            drbCfgDb = &ueCb->drbList[ueCb->numDrb];
            ueCb->numDrb++;
         }

         drbCfgDb->lcId = rlcCfg->logicalChannelIdentity;
         drbCfgDb->cfgSentToUe = false;
         rlcLcCfgDb = &drbCfgDb->rlcLcCfg;
         macLcCfgDb = &drbCfgDb->macLcCfg;
      }


      /* Update RLC configuration for this RB */
      rlcLcCfg = rlcCfg->rlc_Config;
      rlcLcCfgDb->rlcMode = rlcLcCfg->present;
      switch(rlcLcCfgDb->rlcMode)
      {
         case RLC_Config_PR_am:
            {
               rlcLcCfgDb->u.amCfg.ulAmCfg.snLenUl = *(rlcLcCfg->choice.am->ul_AM_RLC.sn_FieldLength);
               rlcLcCfgDb->u.amCfg.ulAmCfg.pollRetxTmr = rlcLcCfg->choice.am->ul_AM_RLC.t_PollRetransmit ;
               rlcLcCfgDb->u.amCfg.ulAmCfg.pollPdu = rlcLcCfg->choice.am->ul_AM_RLC.pollPDU ;
               rlcLcCfgDb->u.amCfg.ulAmCfg.pollByte = rlcLcCfg->choice.am->ul_AM_RLC.pollByte ;
               rlcLcCfgDb->u.amCfg.ulAmCfg.maxRetxTh = rlcLcCfg->choice.am->ul_AM_RLC.maxRetxThreshold ;

               rlcLcCfgDb->u.amCfg.dlAmCfg.snLenDl = *(rlcLcCfg->choice.am->dl_AM_RLC.sn_FieldLength);
               rlcLcCfgDb->u.amCfg.dlAmCfg.reAssemTmr = rlcLcCfg->choice.am->dl_AM_RLC.t_Reassembly;
               rlcLcCfgDb->u.amCfg.dlAmCfg.statProhTmr = rlcLcCfg->choice.am->dl_AM_RLC.t_StatusProhibit;
               break;
            }

         case RLC_Config_PR_um_Bi_Directional:
            {
               rlcLcCfgDb->u.umBiDirCfg.ulUmCfg.snLenUlUm = *(rlcLcCfg->choice.um_Bi_Directional->ul_UM_RLC.sn_FieldLength);

               rlcLcCfgDb->u.umBiDirCfg.dlUmCfg.snLenDlUm = *(rlcLcCfg->choice.um_Bi_Directional->dl_UM_RLC.sn_FieldLength);
               rlcLcCfgDb->u.umBiDirCfg.dlUmCfg.reAssemTmr = rlcLcCfg->choice.um_Bi_Directional->dl_UM_RLC.t_Reassembly;
               break;
            }
      }

      /* Update MAC configuration for this LC */
      macLcCfg = rlcCfg->mac_LogicalChannelConfig;
      macLcCfgDb->priority = macLcCfg->ul_SpecificParameters->priority ;
      macLcCfgDb->lcGroup = *(macLcCfg->ul_SpecificParameters->logicalChannelGroup) ;
      macLcCfgDb->schReqId = *(macLcCfg->ul_SpecificParameters->schedulingRequestID) ;
      macLcCfgDb->pbr = macLcCfg->ul_SpecificParameters->prioritisedBitRate ;
      macLcCfgDb->bsd = macLcCfg->ul_SpecificParameters->bucketSizeDuration ;
   }
   return ROK;   
}

/*******************************************************************
 *
 * @brief Function to decode DU to CU RRC container
 *
 * @details
 *
 *    Function : extractDuToCuRrcCont
 *
 *    Functionality: Function to decode DU to CU RRC container
 *
 * @params[in] UE Cb
 *             RRC conatiner octect string to be decoded
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t extractDuToCuRrcCont(CuUeCb *ueCb, OCTET_STRING_t rrcCont)
{
   CellGroupConfigRrc_t  cellGrpCfg, *cellGrpCfgMsg = NULLP;
   asn_dec_rval_t rval; /* Decoder return value */

   /* Copy the received container to UeCb */
   memcpy(&ueCb->f1apMsgDb.duToCuContainer, &rrcCont, sizeof(OCTET_STRING_t));

   /* Decoding DU to CU RRC container octet string to cell group config */
   cellGrpCfgMsg = &cellGrpCfg;
   memset(cellGrpCfgMsg, 0, sizeof(CellGroupConfigRrc_t));

   rval = uper_decode(0, &asn_DEF_CellGroupConfigRrc, (void **)&cellGrpCfgMsg, rrcCont.buf, rrcCont.size, 0, 0);

   if(rval.code == RC_FAIL || rval.code == RC_WMORE)
   {
      DU_LOG("\nERROR  -->  F1AP : ASN decode failed in extractDuToCuRrcCont");
      return RFAILED;
   }
   printf("\n");
   xer_fprint(stdout, &asn_DEF_CellGroupConfigRrc, cellGrpCfgMsg);

   if((extractCellGroupConfig(ueCb, cellGrpCfgMsg)) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : Failed to extract cell group config");
      return RFAILED;
   }
  
   return ROK;
}

/*******************************************************************
 *
 * @brief Function to build Initial UL RRC Message
 *
 * @details
 *
 *    Function : procInitULRRCMsg
 *
 *    Functionality: Function to build Initial UL RRC Message
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t procInitULRRCMsg(uint32_t duId, F1AP_PDU_t *f1apMsg)
{
   uint8_t idx = 0, duIdx=0, rrcMsgType=0, gnbDuUeF1apId=0;
   uint8_t ret =ROK;
   uint16_t cellIdx=0, nrCellId = 0;
   uint32_t crnti;
   DuDb     *duDb;
   CuCellCb *cellCb;
   CuUeCb   *ueCb;
   InitialULRRCMessageTransfer_t *initULRRCMsg = NULLP;

   DU_LOG("\nINFO   -->  F1AP : filling the required values in DB in procInitULRRCMsg");
   
   SEARCH_DU_DB(duIdx, duId, duDb); 
   initULRRCMsg = &f1apMsg->choice.initiatingMessage->value.choice.InitialULRRCMessageTransfer;

   for(idx=0; idx < initULRRCMsg->protocolIEs.list.count; idx++)
   {
      switch(initULRRCMsg->protocolIEs.list.array[idx]->id)
      {
         case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
            gnbDuUeF1apId = initULRRCMsg->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID;
            break;

         case ProtocolIE_ID_id_NRCGI:
            bitStringToInt(&initULRRCMsg->protocolIEs.list.array[idx]->value.choice.NRCGI.nRCellIdentity, &nrCellId);
            SEARCH_CELL_DB(cellIdx, duDb, nrCellId, cellCb);
            if(cellCb == NULLP)
               return RFAILED;
            break;

         case ProtocolIE_ID_id_C_RNTI:
            crnti = initULRRCMsg->protocolIEs.list.array[idx]->value.choice.C_RNTI;
            if(duDb->ueCb[gnbDuUeF1apId-1].gnbDuUeF1apId == 0)
            {
               ueCb = &duDb->ueCb[gnbDuUeF1apId-1];
               memset(ueCb, 0, sizeof(CuUeCb));
               ueCb->cellCb = cellCb;
               ueCb->crnti = crnti;
               ueCb->gnbDuUeF1apId = gnbDuUeF1apId;
               ueCb->gnbCuUeF1apId = ++cuCb.gnbCuUeF1apIdGenerator;
               ueCb->state = UE_ATTACH_IN_PROGRESS;
               (duDb->numUe)++;

               cellCb->ueCb[cellCb->numUe] = ueCb;
               cellCb->numUe++;
            }
            break;

         case ProtocolIE_ID_id_RRCContainer:
            break;

         case ProtocolIE_ID_id_DUtoCURRCContainer:
            {
               if((initULRRCMsg->protocolIEs.list.array[idx]->value.choice.DUtoCURRCContainer.size > 0) && \
                  (initULRRCMsg->protocolIEs.list.array[idx]->value.choice.DUtoCURRCContainer.buf != NULLP))
               {
                  DU_LOG("\nINFO  -->  Received Du to Cu RRC Container ");
                  ueCb->f1apMsgDb.duToCuContainer.size = initULRRCMsg->protocolIEs.list.array[idx]->value.choice.DUtoCURRCContainer.size;
                  CU_ALLOC(ueCb->f1apMsgDb.duToCuContainer.buf, ueCb->f1apMsgDb.duToCuContainer.size);
                  if(ueCb->f1apMsgDb.duToCuContainer.buf != NULLP)
                  { 
                     memcpy(ueCb->f1apMsgDb.duToCuContainer.buf, \
                        initULRRCMsg->protocolIEs.list.array[idx]->value.choice.DUtoCURRCContainer.buf, \
                        ueCb->f1apMsgDb.duToCuContainer.size);
                  }
                  if((extractDuToCuRrcCont(ueCb, initULRRCMsg->protocolIEs.list.array[idx]->value.choice.DUtoCURRCContainer)) != ROK)
                  {
                     DU_LOG("\nERROR  --> F1AP : Failed to extract DU to CU RRC Container ");
                     ret = RFAILED;
                  }
               }
               else
               {
                  DU_LOG("\nERROR  -->  Failed to receive Du to Cu RRC Container ");
                  ret = RFAILED;
               }
               break;
            }

         default:
            DU_LOG("\nERROR  -->  Invalid Event %ld", initULRRCMsg->protocolIEs.list.array[idx]->id);
            break;
      }
      if(ret == RFAILED)
         break;
   }

   if(ret == ROK)
   {
      ueCb->f1apMsgDb.dlRrcMsgCount++;
      rrcMsgType = setDlRRCMsgType(ueCb);
      ret = BuildAndSendDLRRCMessageTransfer(duId, ueCb, SRB0, rrcMsgType);
   }
   return ret;
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
uint8_t BuildNrcgi(NRCGI_t *nrcgi, uint32_t nrCellId)
{
   uint8_t ret;
   uint8_t unused_bits = 4;
   uint8_t byteSize = 5;

   /* Allocate Buffer Memory */
   nrcgi->pLMN_Identity.size = 3 * sizeof(uint8_t);
   CU_ALLOC(nrcgi->pLMN_Identity.buf, nrcgi->pLMN_Identity.size);
   if(nrcgi->pLMN_Identity.buf == NULLP)
   {
      return RFAILED;
   }
   ret = buildPlmnId(cuCb.cuCfgParams.plmn , nrcgi->pLMN_Identity.buf);

   if(ret != ROK)
   {
      return RFAILED;
   }
   nrcgi->nRCellIdentity.size = byteSize * sizeof(uint8_t);
   CU_ALLOC(nrcgi->nRCellIdentity.buf, nrcgi->nRCellIdentity.size); 
   if(nrcgi->nRCellIdentity.buf == NULLP)
   {
      return RFAILED;
   }
   fillBitString(&nrcgi->nRCellIdentity, unused_bits, byteSize, nrCellId);

   return ROK;
}
/*******************************************************************
 *
 * @brief Builds Special cell list for UE Setup Request 
 *
 * @details
 *
 *    Function : BuildSplCellList
 *
 *    Functionality: Constructs the Special Cell list for UESetReq
 *
 * @params[in] SCell_ToBeSetup_List_t *spCellLst
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSplCellList(CuUeCb *ueCb, SCell_ToBeSetup_List_t *spCellLst)
{
   uint8_t  cellCnt;
   uint8_t  idx;
   uint8_t  ret;
   cellCnt = 1;
   spCellLst->list.count = cellCnt;
   spCellLst->list.size = cellCnt * sizeof(SCell_ToBeSetup_ItemIEs_t *);
   CU_ALLOC(spCellLst->list.array,spCellLst->list.size);
   if(spCellLst->list.array == NULLP)
   {
      return RFAILED;
   }
   for(idx=0; idx<cellCnt; idx++)
   {
      CU_ALLOC(spCellLst->list.array[idx],sizeof(SCell_ToBeSetup_ItemIEs_t));
      if(spCellLst->list.array[idx] == NULLP)
      {
	 return RFAILED;
      }
   }
   idx = 0;
   spCellLst->list.array[idx]->id = ProtocolIE_ID_id_SCell_ToBeSetup_Item;
   spCellLst->list.array[idx]->criticality = Criticality_ignore;
   spCellLst->list.array[idx]->value.present = SCell_ToBeSetup_ItemIEs__value_PR_SCell_ToBeSetup_Item;

   /* Special Cell ID -NRCGI */
   ret = BuildNrcgi(&spCellLst->list.array[idx]->value.choice.SCell_ToBeSetup_Item.sCell_ID, ueCb->cellCb->nrCellId);
   if(ret != ROK)
   {
      return RFAILED;
   }
   /*Special Cell Index*/
   spCellLst->list.array[idx]->value.choice.SCell_ToBeSetup_Item.sCellIndex = 1;
   return ROK;	
}/* End of BuildSplCellList*/

/*******************************************************************
 *
 * @brief Builds SRBS to be setup 
 *
 * @details
 *
 *    Function : BuildSRBSetup
 *
 *    Functionality: Constructs the SRB's for UESetReq
 *
 * @params[in] SRBs_ToBeSetup_List_t *srbSet
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSRBSetup(CuUeCb *ueCb, SRBs_ToBeSetup_List_t *srbSet)
{
   uint8_t idx;
   uint8_t srbCnt;

   if(ueCb->state == UE_HANDOVER_IN_PROGRESS)
      srbCnt = ueCb->numSrb;
   else
      srbCnt = 1;
   srbSet->list.count = srbCnt;
   srbSet->list.size = srbCnt*sizeof(SRBs_ToBeSetup_ItemIEs_t *);
   CU_ALLOC(srbSet->list.array,srbSet->list.size);
   if(srbSet->list.array == NULLP)
   {
      DU_LOG("\nERROR  --> F1AP : BuildSRBSetup() : Memory allocation failed for SRB to be setup list's array");
      return RFAILED;
   }

   for(idx=0; idx<srbCnt; idx++)
   {
      CU_ALLOC(srbSet->list.array[idx],sizeof(SRBs_ToBeSetup_ItemIEs_t));
      if(srbSet->list.array[idx] == NULLP)
      {
         DU_LOG("\nERROR  --> F1AP : BuildSRBSetup() : Memory allocation failed for SRB to be setup list's array element");
         return RFAILED;
      }
   }

   if(ueCb->state != UE_HANDOVER_IN_PROGRESS)
   {
      idx = 0;
      srbSet->list.array[idx]->id = ProtocolIE_ID_id_SRBs_ToBeSetup_Item;
      srbSet->list.array[idx]->criticality = Criticality_ignore;
      srbSet->list.array[idx]->value.present = SRBs_ToBeSetup_ItemIEs__value_PR_SRBs_ToBeSetup_Item;
      srbSet->list.array[idx]->value.choice.SRBs_ToBeSetup_Item.sRBID = 2;
      ueCb->srbList[ueCb->numSrb].srbId = srbSet->list.array[idx]->value.choice.SRBs_ToBeSetup_Item.sRBID;
      ueCb->numSrb++;
   }
   else
   {
      for(idx=0; idx<srbCnt; idx++)
      {
         srbSet->list.array[idx]->id = ProtocolIE_ID_id_SRBs_ToBeSetup_Item;
         srbSet->list.array[idx]->criticality = Criticality_ignore;
         srbSet->list.array[idx]->value.present = SRBs_ToBeSetup_ItemIEs__value_PR_SRBs_ToBeSetup_Item;
         srbSet->list.array[idx]->value.choice.SRBs_ToBeSetup_Item.sRBID = ueCb->srbList[idx].srbId; 
      }
   }
   return ROK;
}/* End of BuildSRBSetup*/

/*******************************************************************
 *
 * @brief Builds QOS Info for DRB Setum Item 
 *
 * @details
 *
 *    Function : BuildQOSInfo
 *
 *    Functionality: Constructs the QOS Info for DRB Setup Item
 *
 * @params[in] QoSInformation_t *qosinfo
 *             int16_t pduSessionID
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildQOSInfo(QosInfo *qosInfo, QoSFlowLevelQoSParameters_t *drbQos, uint8_t actionType, int16_t pduSessionID, bool hoInProgress)
{
   uint8_t elementCnt = 0, qosCntIdx = 0;
   ProtocolExtensionContainer_4624P74_t *qosIeExt = NULLP;

   /* NonDynamic5QIDescriptor */
   drbQos->qoS_Characteristics.present = QoS_Characteristics_PR_non_Dynamic_5QI;
   CU_ALLOC(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI,sizeof(NonDynamic5QIDescriptor_t));
   if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI == NULLP)
   {
      return RFAILED;
   }
   
   if(hoInProgress == Inter_DU_HO)
      drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->fiveQI = qosInfo->nonDynFiveQI ;
   else if(hoInProgress == false || hoInProgress == Xn_Based_Inter_CU_HO)
   {
      /*FiveQI*/
      if(actionType == ProtocolIE_ID_id_DRBs_ToBeModified_Item)
         drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->fiveQI = FIVE_QI_VALUE8;
      else
         drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->fiveQI = FIVE_QI_VALUE9;

      qosInfo->nonDynFiveQI = drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->fiveQI;
   }

   if(!hoInProgress)
   {
      /*AveragingWindow*/
      CU_ALLOC(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow, sizeof(AveragingWindow_t));
      if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow == NULLP)
      {
         return RFAILED;
      }
      *(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow) = 0;
      qosInfo->avgWindow = *(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow);

      /*MaxDataBurstVolume*/
      CU_ALLOC(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume, sizeof(MaxDataBurstVolume_t));
      if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume == NULLP)
      {
         return RFAILED;
      }
      *(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume) = 0;
      qosInfo->maxBurstDataVol = *(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume);
   }

   /*nRGRAN Allocation Retention Priority*/
   if(hoInProgress)
   {
      drbQos->nGRANallocationRetentionPriority.priorityLevel = qosInfo->priorityLevel;
      drbQos->nGRANallocationRetentionPriority.pre_emptionCapability = qosInfo->preemptionCapability;
      drbQos->nGRANallocationRetentionPriority.pre_emptionVulnerability = qosInfo->preemptionVulnerability;
   }
   else if((hoInProgress == false) || (hoInProgress == Xn_Based_Inter_CU_HO))
   {
      drbQos->nGRANallocationRetentionPriority.priorityLevel = PriorityLevel_lowest;
      drbQos->nGRANallocationRetentionPriority.pre_emptionCapability = Pre_emptionCapability_may_trigger_pre_emption;
      drbQos->nGRANallocationRetentionPriority.pre_emptionVulnerability = Pre_emptionVulnerability_not_pre_emptable;

      qosInfo->priorityLevel = PriorityLevel_lowest;
      qosInfo->preemptionCapability = Pre_emptionCapability_may_trigger_pre_emption;
      qosInfo->preemptionVulnerability = Pre_emptionVulnerability_not_pre_emptable;
   }

   /* PDU session ID */
   if(!hoInProgress)
   {
      /*If PDU Session ID is INVALID thus not to be included in Qos IE, skip the PDU Session IE */
      if(pduSessionID <= INVALID_PDU_SESSION_ID)
      {
         DU_LOG("\nINFO  --> F1AP : Invalid PDU_SESSION_ID");
         return ROK;
      }

      CU_ALLOC(drbQos->iE_Extensions, sizeof(ProtocolExtensionContainer_4624P74_t));
      qosIeExt = (ProtocolExtensionContainer_4624P74_t *)drbQos->iE_Extensions;

      if(qosIeExt)
      {
         elementCnt = NUM_QOS_EXT;
         qosIeExt->list.count = elementCnt;
         qosIeExt->list.size = elementCnt * sizeof(QoSFlowLevelQoSParameters_ExtIEs_t *);

         /*Initialize QoSFlowLevelQoSParameters_ExtIEs_t*/
         CU_ALLOC(qosIeExt->list.array, qosIeExt->list.size);

         if(qosIeExt->list.array == NULLP)
         {
            DU_LOG("\nERROR  -->	F1AP : Memory allocation for QoSFlowLevelQoSParameters_ExtIEs_t failed");
            return  RFAILED;
         }

         for(qosCntIdx=0; qosCntIdx < elementCnt; qosCntIdx++)
         {
            CU_ALLOC(qosIeExt->list.array[qosCntIdx], sizeof(QoSFlowLevelQoSParameters_ExtIEs_t));
            if(qosIeExt->list.array[qosCntIdx] == NULLP)
            {
               DU_LOG("\nERROR  -->	F1AP : Memory allocation for QoSFlowLevelQoSParameters_ExtIEs_t array failed");
               return  RFAILED;
            }
            /*Filling QoSFlowLevelQoSParameters_ExtIEs_t*/
            qosIeExt->list.array[qosCntIdx]->id = ProtocolIE_ID_id_PDUSessionID;
            /*Below Criticality mentioned in Spec38.473, 15.4.1 and later*/
            qosIeExt->list.array[qosCntIdx]->criticality = Criticality_ignore; 
            qosIeExt->list.array[qosCntIdx]->extensionValue.present = \
                                                                      QoSFlowLevelQoSParameters_ExtIEs__extensionValue_PR_PDUSessionID;
            qosIeExt->list.array[qosCntIdx]->extensionValue.choice.PDUSessionID = (PDUSessionID_t)pduSessionID;
            qosInfo->pduSessionId = pduSessionID;
         }
      }
      else
      {
         DU_LOG("\nERROR  -->	F1AP : Memory allocation for QosIE_extension failed");
         return RFAILED;
      }
   }
   return ROK;
}/*End of BuildQOSInfo*/

/*******************************************************************
 *
 * @brief Builds SNSSAI  
 *
 * @details
 *
 *    Function : BuildSNSSAI
 *
 *    Functionality: Constructs the SNSSAI For DRB list
 *
 * @params[in] SNSSAI_t *snssai
 *             Snssai  *snssaiToCopy S-NSSAI from CuCfgParam to be copied
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildSNSSAI(DrbInfo *drbInfo, SNSSAI_t *snssai, Snssai *snssaiToCopy, bool hoInProgress)
{
   /*SNSSAI*/
   /* In case of non-HO UE context creation and Xn Based HO, DRB's SNSSAI
    * configuration is not known beforehand. In these 2 case, the following
    * condition will hit */
   if(!hoInProgress || (hoInProgress == Xn_Based_Inter_CU_HO))
      drbInfo->snssai = snssaiToCopy;

   /*ssT*/
   snssai->sST.size = sizeof(uint8_t);
   CU_ALLOC(snssai->sST.buf, snssai->sST.size);
   if(snssai->sST.buf == NULLP)
   {
      return RFAILED;
   }
   memcpy(snssai->sST.buf, &drbInfo->snssai->sst, snssai->sST.size);

   /*sD*/
   CU_ALLOC(snssai->sD, sizeof(OCTET_STRING_t));
   if(snssai->sD == NULLP)
   {
      return RFAILED;
   }
   snssai->sD->size = 3 * sizeof(uint8_t);
   CU_ALLOC(snssai->sD->buf, snssai->sD->size);
   if(snssai->sD->buf == NULLP)
   {
      return RFAILED;
   }
   memcpy(snssai->sD->buf, drbInfo->snssai->sd, snssai->sD->size);

   return ROK;
}/*End of BuildSNSSAI*/

/*******************************************************************
 *
 * @brief Builds the flow map.  
 *
 * @details
 *
 *    Function : BuildFlowsMap
 *
 *    Functionality: Constructs the flowmap For DRB list
 *
 * @params[in] Flows_Mapped_To_DRB_List_t *flowMap
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildFlowsMap(DrbInfo *drbInfo, Flows_Mapped_To_DRB_List_t *flowMap , uint8_t actionType, bool hoInProgress)
{
   uint8_t  ret = ROK, idx = 0, flowCnt = 0, flowIdx = 0;
   FlowsMapped *qosFlow;

   /* In case of non-HO UE context creation and Xn Based HO, DRB's SNSSAI
    * configuration is not known beforehand. In these 2 case, the following
    * condition will hit */
   if(!hoInProgress || (hoInProgress == Xn_Based_Inter_CU_HO))
      flowCnt = 1;
   else
      flowCnt = drbInfo->numFlowMap;

   flowMap->list.count = flowCnt;
   flowMap->list.size = flowCnt * sizeof(Flows_Mapped_To_DRB_Item_t *);
   CU_ALLOC(flowMap->list.array,flowMap->list.size);
   if(flowMap->list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed for array in BuildFlowsMap()");
      return RFAILED;
   }
   for(idx=0; idx<flowCnt; idx++)
   {
      CU_ALLOC(flowMap->list.array[idx],sizeof(Flows_Mapped_To_DRB_Item_t));
      if(flowMap->list.array[idx] == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed for arrayIdx[%d] in BuildFlowsMap()", idx);
         return RFAILED;
      }
      
      if(!hoInProgress || (hoInProgress == Xn_Based_Inter_CU_HO))
      {
         flowMap->list.array[idx]->qoSFlowIdentifier = 0;
         if(actionType == ProtocolIE_ID_id_DRBs_ToBeModified_Item)
         {
            for(flowIdx =0; flowIdx < drbInfo->numFlowMap; flowIdx++)
            {
               if(drbInfo->flowMapList[flowIdx].qosFlowId == flowMap->list.array[idx]->qoSFlowIdentifier)
               {
                  qosFlow = &drbInfo->flowMapList[flowIdx];
                  break;
               }
            }
         }
         else
         {
            qosFlow = &drbInfo->flowMapList[drbInfo->numFlowMap];
            qosFlow->qosFlowId = flowMap->list.array[idx]->qoSFlowIdentifier;
         }
      }
      else
      {
         qosFlow = &drbInfo->flowMapList[idx];
         flowMap->list.array[idx]->qoSFlowIdentifier = qosFlow->qosFlowId;
      }

      ret = BuildQOSInfo(&qosFlow->qos, &flowMap->list.array[idx]->qoSFlowLevelQoSParameters,\
            actionType, INVALID_PDU_SESSION_ID, hoInProgress);
      if(ret != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Failed to Build QOS Info in BuildFlowsMap()");
         return RFAILED;
      }

      if(((!hoInProgress) && (actionType != ProtocolIE_ID_id_DRBs_ToBeModified_Item)) || (hoInProgress == Xn_Based_Inter_CU_HO))
         drbInfo->numFlowMap++;
   }
   return ROK;
}/*End of BuildFlowsMap*/

/*******************************************************************
 *
 * @brief Builds the Uplink Tunnel Info  
 *
 * @details
 *
 *    Function : BuildULTnlInfo
 *
 *    Functionality: Constructs the UL TnlInfo For DRB list
 *
 * @params[in]  UPTNLInformation_ToBeSetup_List_t *ulInfo
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildULTnlInfo(uint8_t duId, TnlInfo *ulUpTnlInfo, ULUPTNLInformation_ToBeSetup_List_t *ulInfo, bool hoInProgress)
{
   uint8_t idx;
   uint8_t ulCnt;

   ulCnt = 1;
   ulInfo->list.count = ulCnt;
   ulInfo->list.size = ulCnt * sizeof(ULUPTNLInformation_ToBeSetup_Item_t *);
   CU_ALLOC(ulInfo->list.array,ulInfo->list.size);
   if(ulInfo->list.array == NULLP)
   {  
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed for array in BuildULTnlInfo()");
      return RFAILED;
   }
   for(idx=0; idx<ulCnt; idx++)
   {
      CU_ALLOC(ulInfo->list.array[idx],sizeof(ULUPTNLInformation_ToBeSetup_Item_t));
      if(ulInfo->list.array[idx] == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed for arrayIdx [%d] in BuildULTnlInfo()", idx);
	      return RFAILED;
      }
   }
   idx = 0;
   ulInfo->list.array[idx]->uLUPTNLInformation.present = UPTransportLayerInformation_PR_gTPTunnel;
   /*GTP TUNNEL*/
   CU_ALLOC(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel, sizeof(GTPTunnel_t));
   if(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed for gTPTunnel in BuildULTnlInfo()");
      return RFAILED;
   }
   ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.size	= 4*sizeof(uint8_t);
   CU_ALLOC(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf, \
      ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.size);
   if(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed for gtp tunnel arrayIdx[%d] in BuildULTnlInfo()", idx);
      return RFAILED;
   }

   if((!hoInProgress) || (hoInProgress == Xn_Based_Inter_CU_HO))
   {
      /* NOTE: Below IP address must be changed if running on different IP configuration */
      ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf[0] = 192;
      ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf[1] = 168;
      ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf[2] = 130;
      ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf[3] = 82;
      ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.bits_unused = 0;

      ulUpTnlInfo->address[0] = ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf[0];
      ulUpTnlInfo->address[1] = ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf[1];
      ulUpTnlInfo->address[2] = ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf[2];
      ulUpTnlInfo->address[3] = ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf[3];
   }
   else
   {
      ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf[0] = ulUpTnlInfo->address[0];
      ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf[1] = ulUpTnlInfo->address[1];
      ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf[2] = ulUpTnlInfo->address[2];
      ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf[3] = ulUpTnlInfo->address[3];
      ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.bits_unused = 0;
   }

   /*GTP TEID*/
   ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.size = 4 * sizeof(uint8_t);
   CU_ALLOC(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf, \
      ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.size);
   if(ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed for gtp tunnel buffer in BuildULTnlInfo()");
      return RFAILED;
   }
   
   if((!hoInProgress) || (hoInProgress == Xn_Based_Inter_CU_HO))
   {
      ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf[0] = 0;
      ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf[1] = 0;
      ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf[2] = 0;
      ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf[3] = cuCb.cuCfgParams.egtpParams.currTunnelId++;

      ulUpTnlInfo->teId[0] = ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf[0];
      ulUpTnlInfo->teId[1] = ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf[1];
      ulUpTnlInfo->teId[2] = ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf[2];
      ulUpTnlInfo->teId[3] = ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf[3];
   }
   else
   {
      ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf[0] = ulUpTnlInfo->teId[0];
      ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf[1] = ulUpTnlInfo->teId[1];
      ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf[2] = ulUpTnlInfo->teId[2];
      ulInfo->list.array[idx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf[3] = ulUpTnlInfo->teId[3];
   }
   return ROK;
}/*End of BuildULTnlInfo*/

/*******************************************************************
 *
 * @brief Builds DRBS to be setup 
 *
 * @details
 *
 *    Function : BuildDRBSetup
 *
 *    Functionality: Constructs the DRB's for UESetReq
 *
 * @params[in] DRBs_ToBeSetup_List_t *drbSet
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildDRBSetup(uint32_t duId, CuUeCb *ueCb, DRBs_ToBeSetup_List_t *drbSet)
{
   uint16_t snssaiIdx=0;
   uint8_t idx = 0, extIeIdx = 0;
   uint8_t elementCnt = 0, drbCnt = 0;
   uint8_t BuildQOSInforet = 0,BuildSNSSAIret = 0;
   uint8_t BuildFlowsMapret =0, BuildULTnlInforet =0;
   DRBs_ToBeSetup_Item_t *drbSetItem;
   ProtocolExtensionContainer_4624P33_t *drbToBeSetupExt;
   DRBs_ToBeSetup_ItemExtIEs_t *drbToBeSetupExtIe = NULLP;
   
   if(ueCb->state == UE_HANDOVER_IN_PROGRESS)
      drbCnt = ueCb->numDrb;
   else
      drbCnt = MAX_DRB_SET_UE_CONTEXT_SETUP_REQ;
   drbSet->list.count = drbCnt;

   drbSet->list.size = drbCnt*sizeof(DRBs_ToBeSetup_ItemIEs_t *);
   CU_ALLOC(drbSet->list.array,drbSet->list.size);
   if(drbSet->list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDRBSetup");
      return RFAILED;
   }

   for(idx=0; idx<drbCnt; idx++)
   {
      CU_ALLOC(drbSet->list.array[idx],sizeof(DRBs_ToBeSetup_ItemIEs_t));
      if(drbSet->list.array[idx] == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDRBSetup for arry idx [%d]", idx);
         return RFAILED;
      }

      drbSet->list.array[idx]->id = ProtocolIE_ID_id_DRBs_ToBeSetup_Item;
      drbSet->list.array[idx]->criticality = Criticality_ignore;
      drbSet->list.array[idx]->value.present = DRBs_ToBeSetup_ItemIEs__value_PR_DRBs_ToBeSetup_Item;
      drbSetItem = &drbSet->list.array[idx]->value.choice.DRBs_ToBeSetup_Item;
      /*dRBID*/
      if(ueCb->state != UE_HANDOVER_IN_PROGRESS)
      {
         drbSetItem->dRBID = idx + 1;
         ueCb->drbList[ueCb->numDrb].drbId = drbSetItem->dRBID;
      }
      else
         drbSetItem->dRBID = ueCb->drbList[idx].drbId;

      /*qoSInformation*/
      drbSetItem->qoSInformation.present = QoSInformation_PR_choice_extension;
      CU_ALLOC(drbSetItem->qoSInformation.choice.choice_extension,sizeof(QoSInformation_ExtIEs_t));
      if(drbSetItem->qoSInformation.choice.choice_extension == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed for QOS choice extension in BuildDRBSetup");
         return RFAILED;
      }
      drbSetItem->qoSInformation.choice.choice_extension->id = ProtocolIE_ID_id_DRB_Information;
      drbSetItem->qoSInformation.choice.choice_extension->criticality = Criticality_ignore;
      drbSetItem->qoSInformation.choice.choice_extension->value.present = QoSInformation_ExtIEs__value_PR_DRB_Information;
      if(ueCb->state != UE_HANDOVER_IN_PROGRESS)
         BuildQOSInforet =  BuildQOSInfo(&ueCb->drbList[ueCb->numDrb].qos, &drbSetItem->qoSInformation.choice.\
               choice_extension->value.choice.DRB_Information.dRB_QoS, ProtocolIE_ID_id_DRBs_ToBeSetup_Item, PDU_SESSION_ID_1, FALSE);
      else
         BuildQOSInforet =  BuildQOSInfo(&ueCb->drbList[idx].qos, &drbSetItem->qoSInformation.choice.\
               choice_extension->value.choice.DRB_Information.dRB_QoS, ProtocolIE_ID_id_DRBs_ToBeSetup_Item, PDU_SESSION_ID_1, ueCb->hoInfo.HOType);
      if(BuildQOSInforet != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Failed to build QOS Info in BuildDRBSetup");
         return RFAILED;
      }

      /*SNSSAI*/
      snssaiIdx = (idx% cuCb.numSnssaiSupported);
      if(ueCb->state != UE_HANDOVER_IN_PROGRESS)
         BuildSNSSAIret = BuildSNSSAI(&ueCb->drbList[ueCb->numDrb], &drbSetItem->qoSInformation.choice.\
               choice_extension->value.choice.DRB_Information.sNSSAI, cuCb.snssaiList[snssaiIdx], FALSE);
      else
         BuildSNSSAIret = BuildSNSSAI(&ueCb->drbList[idx], &drbSetItem->qoSInformation.choice.\
               choice_extension->value.choice.DRB_Information.sNSSAI, cuCb.snssaiList[snssaiIdx], ueCb->hoInfo.HOType);
      if(BuildSNSSAIret != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Failed to build SNSSAI Info in BuildDRBSetup");
         return RFAILED;
      }

      /*Flows mapped to DRB List*/
      if(ueCb->state != UE_HANDOVER_IN_PROGRESS)
         BuildFlowsMapret = BuildFlowsMap(&ueCb->drbList[ueCb->numDrb], &drbSetItem->qoSInformation.choice.choice_extension->\
               value.choice.DRB_Information.flows_Mapped_To_DRB_List, ProtocolIE_ID_id_DRBs_ToBeSetup_Item, FALSE);
      else
         BuildFlowsMapret = BuildFlowsMap(&ueCb->drbList[idx], &drbSetItem->qoSInformation.choice.choice_extension->\
               value.choice.DRB_Information.flows_Mapped_To_DRB_List, ProtocolIE_ID_id_DRBs_ToBeSetup_Item, ueCb->hoInfo.HOType);
      if(BuildFlowsMapret != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Failed to build Flow Map Info in BuildDRBSetup");
         return RFAILED;
      }

      /*ULUPTNLInformation To Be Setup List*/
      if(ueCb->state != UE_HANDOVER_IN_PROGRESS)
         BuildULTnlInforet = BuildULTnlInfo(duId, &ueCb->drbList[ueCb->numDrb].ulUpTnlInfo, &drbSetItem->uLUPTNLInformation_ToBeSetup_List,\
               FALSE);
      else
         BuildULTnlInforet = BuildULTnlInfo(duId, &ueCb->drbList[idx].ulUpTnlInfo, &drbSetItem->uLUPTNLInformation_ToBeSetup_List,\
               ueCb->hoInfo.HOType);
      if(BuildULTnlInforet != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Failed to build tunnel Info in BuildDRBSetup");
         return RFAILED;
      }

      /*RLCMode*/
      if((ueCb->state != UE_HANDOVER_IN_PROGRESS) || (ueCb->hoInfo.HOType == Xn_Based_Inter_CU_HO))
      {
         drbSetItem->rLCMode = RLCMode_rlc_um_bidirectional;
         ueCb->drbList[ueCb->numDrb].rlcMode = drbSetItem->rLCMode;
      }
      else
         drbSetItem->rLCMode = ueCb->drbList[idx].rlcMode;

      /* DL PDCP SN Length */
      if(ueCb->state == UE_HANDOVER_IN_PROGRESS)
      {
         CU_ALLOC(drbToBeSetupExt, sizeof(ProtocolExtensionContainer_4624P33_t));
         if(!drbToBeSetupExt)
         {
            DU_LOG("\nERROR  -->  F1AP : Failed to allocate memory for extension IE list in BuildDRBSetup");
            return RFAILED;
         }

         elementCnt = 1;
         drbToBeSetupExt->list.count = elementCnt;
         drbToBeSetupExt->list.size = drbToBeSetupExt->list.count * sizeof(DRBs_ToBeSetup_ItemExtIEs_t *);

         CU_ALLOC(drbToBeSetupExt->list.array, drbToBeSetupExt->list.size);
         if(!drbToBeSetupExt->list.array)
         {
             DU_LOG("\nERROR  -->  F1AP : Failed to allocated memory for extension array in BuildDRBSetup");
             return RFAILED;
         }

         for(extIeIdx=0; extIeIdx < drbToBeSetupExt->list.count; extIeIdx++)
         {
            CU_ALLOC(drbToBeSetupExt->list.array[extIeIdx], sizeof(DRBs_ToBeSetup_ItemExtIEs_t));
            if(!drbToBeSetupExt->list.array[extIeIdx])
            {
               DU_LOG("\nERROR  -->  F1AP : Failed to allocated memory for extension array element in BuildDRBSetup");
               return RFAILED;
            }
         }
 
         extIeIdx = 0;
         drbToBeSetupExtIe = drbToBeSetupExt->list.array[extIeIdx];

         drbToBeSetupExtIe->id = ProtocolIE_ID_id_DLPDCPSNLength;
         drbToBeSetupExtIe->criticality = Criticality_ignore;
         drbToBeSetupExtIe->extensionValue.present = DRBs_ToBeSetup_ItemExtIEs__extensionValue_PR_PDCPSNLength;
         drbToBeSetupExtIe->extensionValue.choice.PDCPSNLength = PDCPSNLength_twelve_bits;
         drbSetItem->iE_Extensions = drbToBeSetupExt;
      }

      if(ueCb->state != UE_HANDOVER_IN_PROGRESS)
         ueCb->numDrb++;
   }
   return ROK;
}/* End of BuildDRBSetup*/

/*******************************************************************
 *
 * @brief Deallocating memory of function BuildAndSendUESetReq
 *
 * @details
 *
 *    Function : FreeNrcgi
 *
 *    Functionality: Deallocating memory for function BuildNrcgi
 *
 * @params[in] NRCGI_t *nrcgi
 *
 * @return void
 *
 *******************************************************************/
void FreeNrcgi(NRCGI_t *nrcgi)
{
   if(nrcgi->pLMN_Identity.buf != NULLP)
   {
      if(nrcgi->nRCellIdentity.buf != NULLP)
      {
	 CU_FREE(nrcgi->nRCellIdentity.buf, nrcgi->nRCellIdentity.size); 
      }
      CU_FREE(nrcgi->pLMN_Identity.buf, nrcgi->pLMN_Identity.size);
   }
}
/*******************************************************************
 *
 * @brief  Deallocating memory of function BuildAndSendUESetReq
 *
 * @details
 *
 *    Function : FreeSplCellList
 *
 *    Functionality: Deallocating memory for function BuildSplCellList
 *
 * @params[in] SCell_ToBeSetup_List_t *spCellLst
 *
 * @return void
 *      
 *
 * *****************************************************************/
void FreeSplCellList(SCell_ToBeSetup_List_t *spCellLst)
{
   uint8_t  cellidx;
   if(spCellLst->list.array != NULLP)
   {
      for(cellidx=0; cellidx<spCellLst->list.count; cellidx++)
      {
	 if(cellidx==0&&spCellLst->list.array[cellidx]!=NULLP)
	 {
	    FreeNrcgi(&spCellLst->list.array[cellidx]->value.choice.SCell_ToBeSetup_Item.sCell_ID);
	 }
	 if(spCellLst->list.array[cellidx]!=NULLP)
	 {
	    CU_FREE(spCellLst->list.array[cellidx],sizeof(SCell_ToBeSetup_ItemIEs_t));
	 }
      }
      CU_FREE(spCellLst->list.array,spCellLst->list.size);
   }
}
/*******************************************************************
 *
 * @brief Deallocating memory of function BuildAndSendUESetReq
 *
 * @details
 *
 *    Function : FreeSRBSetup
 *
 *    Functionality: Deallocating memory for function BuildSRBSetup
 *
 * @params[in] SRBs_ToBeSetup_List_t *srbSet
 *
 * @return void
 *        
 *
 * ******************************************************************/
void FreeSRBSetup(SRBs_ToBeSetup_List_t *srbSet)
{
   uint8_t srbidx;
   if(srbSet->list.array != NULLP)
   {
      for(srbidx=0; srbidx<srbSet->list.count; srbidx++)
      {
	 if(srbSet->list.array[srbidx]!=NULLP)
	 {
	    CU_FREE(srbSet->list.array[srbidx],sizeof(SRBs_ToBeSetup_ItemIEs_t));
	 }
      }
      CU_FREE(srbSet->list.array,srbSet->list.size);
   }
}
/*******************************************************************
 *
 * @brief Deallocating memory of function BuildAndSendUESetReq
 *
 * @details
 *
 *    Function : FreeQOSInfo
 *
 *    Functionality:  Deallocating memory for function BuildQOSInfo
 *
 * @params[in] QoSFlowLevelQoSParameters_t *drbQos
 *
 * @return void
 *          
 * ****************************************************************/
void FreeQOSInfo(QoSFlowLevelQoSParameters_t *drbQos)
{
   ProtocolExtensionContainer_4624P74_t *qosIeExt = NULLP;
   uint8_t qosCntIdx = 0;

   if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI != NULLP)
   {
      if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow!=NULLP)
      {
         if(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume!=NULLP)
         {
            CU_FREE(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume,\
                  sizeof(MaxDataBurstVolume_t));
         }
         CU_FREE(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow,\
               sizeof(AveragingWindow_t));
      }
      CU_FREE(drbQos->qoS_Characteristics.choice.non_Dynamic_5QI,\
            sizeof(NonDynamic5QIDescriptor_t));
   }
   if(drbQos->iE_Extensions)
   {
      qosIeExt = (ProtocolExtensionContainer_4624P74_t *)drbQos->iE_Extensions;
      if(qosIeExt->list.array != NULLP)
      {
         for(qosCntIdx=0; qosCntIdx <  qosIeExt->list.count; qosCntIdx++)
         {
            if(qosIeExt->list.array[qosCntIdx])
            {
               CU_FREE(qosIeExt->list.array[qosCntIdx], sizeof(QoSFlowLevelQoSParameters_ExtIEs_t));
            }
         }
         CU_FREE(qosIeExt->list.array, qosIeExt->list.size);
      }

      CU_FREE(drbQos->iE_Extensions, sizeof(ProtocolExtensionContainer_4624P74_t));
   }
}
/*******************************************************************
 *
 * @brief Deallocating memory of function BuildAndSendUESetReq
 *
 * @details
 *
 *    Function : FreeULTnlInfo
 *
 *    Functionality:  Deallocating memory for function BuildULTnlInfo
 *
 * @params[in] ULUPTNLInformation_ToBeSetup_List_t *ulInfo
 *
 * @return void
 *         

 * ****************************************************************/
void FreeULTnlInfo(ULUPTNLInformation_ToBeSetup_List_t *ulInfo)
{
   uint8_t ulidx=0;
   if(ulInfo->list.array != NULLP)
   {
      for(ulidx=0; ulidx<ulInfo->list.count; ulidx++)
      {
	 if(ulidx==0&&ulInfo->list.array[ulidx]!=NULLP)
	 {
	    if(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel!=NULLP)
	    {
	       if(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel->\
		     transportLayerAddress.buf != NULLP)
	       {
		  if(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf\
			!=NULLP)
		  {
		     CU_FREE(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel->\
			   gTP_TEID.buf,ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.\
			   gTPTunnel->gTP_TEID.size);
		  }
		  CU_FREE(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel->\
			transportLayerAddress.buf,ulInfo->list.array[ulidx]->\
			uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.size);
	       }
	       CU_FREE(ulInfo->list.array[ulidx]->uLUPTNLInformation.choice.gTPTunnel,\
		     sizeof(GTPTunnel_t));
	    }
	 }
	 if(ulInfo->list.array[ulidx]!=NULLP)
	 {
	    CU_FREE(ulInfo->list.array[ulidx],sizeof(ULUPTNLInformation_ToBeSetup_Item_t));
	 }
      }
      CU_FREE(ulInfo->list.array,ulInfo->list.size);
   }
}
/*******************************************************************
 *
 * @brief Deallocating memory for BuildAndSendUESetReq
 *
 * @details
 *
 *    Function : FreeDRBSetup
 *
 *    Functionality:  Deallocating memory for BuildDRBSetup
 *
 * @params[in] DRBs_ToBeSetup_List_t *drbSet
 *
 * @return void
 *
 * ****************************************************************/
void FreeDRBSetup(DRBs_ToBeSetup_List_t *drbSet)
{
   DRBs_ToBeSetup_Item_t *drbSetItem;
   ProtocolExtensionContainer_4624P74_t *qosIeExt = NULLP;
   
   uint8_t  flowidx = 0, drbidx = 0, qosCntIdx = 0;

   /*BUG: Need to check drbSet->list.array is not Empty to procced with Deletion*/
   if(drbSet->list.array != NULLP)
   {
      for(drbidx=0; drbidx<drbSet->list.count; drbidx++)
      {
	      if(drbSet->list.array[drbidx] != NULLP)
	      {
	         drbSetItem =&drbSet->list.array[drbidx]->value.choice.DRBs_ToBeSetup_Item;
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
			                     if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD->buf!=NULLP)
			                     {
				                     if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.\
				                            flows_Mapped_To_DRB_List.list.array != NULLP)
				                     {
				                         for(flowidx=0;flowidx<drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.\
					                              flows_Mapped_To_DRB_List.list.count; flowidx++)
				                         {
				                             if(flowidx==0&&drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
					                                   DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]!=NULLP)
				                             {
					                             if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
						                                DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
						                                qoS_Characteristics.choice.non_Dynamic_5QI!=NULLP)
					                             {
					                                 if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
						                                   DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
						                                   qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow!=NULLP)
					                                 {
						                                 if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
						                                 DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
						                                  qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume!=NULLP)
						                                  {	
						                                      FreeULTnlInfo(&drbSetItem->uLUPTNLInformation_ToBeSetup_List);
						                                      CU_FREE(drbSetItem->uLConfiguration,sizeof(ULConfiguration_t));

						                                      CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
							                                           DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
							                                           qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume,\
							                                           sizeof(MaxDataBurstVolume_t));
						                                  }
	                                                 CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
						                                           DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
						                                           qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow,sizeof(AveragingWindow_t));
					                                 }
					                                 CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
						                                       DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]->qoSFlowLevelQoSParameters.\
						                                       qoS_Characteristics.choice.non_Dynamic_5QI,sizeof(NonDynamic5QIDescriptor_t));
					                             }
				                            }
				                            if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
					                             DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx]!=NULLP)
				                            {
					                            CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.\
						                                  DRB_Information.flows_Mapped_To_DRB_List.list.array[flowidx],sizeof(Flows_Mapped_To_DRB_Item_t));
				                            }
				                        }
				                        CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.\
					                              flows_Mapped_To_DRB_List.list.array,drbSetItem->qoSInformation.choice.choice_extension->value.\
					                              choice.DRB_Information.flows_Mapped_To_DRB_List.list.size);
				                     }
				                     CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD->buf,\
				                             drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD->size);
			                    }
			                    CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sD,\
				                              sizeof(OCTET_STRING_t));
			               }
			               CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sST.buf,\
				                    drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI.sST.size);
			            }
			            CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
			                    qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume,sizeof(MaxDataBurstVolume_t));
		            }
	               CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
			                 qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow,sizeof(AveragingWindow_t));
		         }
		         CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
			               qoS_Characteristics.choice.non_Dynamic_5QI, sizeof(NonDynamic5QIDescriptor_t));
             }
             if(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
                        iE_Extensions != NULLP)
             {
                qosIeExt = (ProtocolExtensionContainer_4624P74_t *)drbSetItem->qoSInformation.choice.\
                                    choice_extension->value.choice.DRB_Information.dRB_QoS.iE_Extensions;
                 if(qosIeExt->list.array != NULLP)
                 {
                   for(qosCntIdx=0; qosCntIdx < qosIeExt->list.count; qosCntIdx++)
                   {
                      if(qosIeExt->list.array[qosCntIdx] != NULLP)
                      {
                         CU_FREE(qosIeExt->list.array[qosCntIdx], sizeof(QoSFlowLevelQoSParameters_ExtIEs_t));
                      }
                    }
                    CU_FREE(qosIeExt->list.array, qosIeExt->list.size);
                  }
                  CU_FREE(drbSetItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS.\
                                   iE_Extensions, sizeof(ProtocolExtensionContainer_4624P74_t));
              }
                  
	          CU_FREE(drbSetItem->qoSInformation.choice.choice_extension,sizeof(QoSInformation_ExtIEs_t));
	        }
	        CU_FREE(drbSet->list.array[drbidx],sizeof(DRBs_ToBeSetup_ItemIEs_t));
	      }
      }
      CU_FREE(drbSet->list.array,drbSet->list.size);
   }
}


/*******************************************************************
 *
 * @brief Free the UE Setup Request
 *
 * @details
 *
 *    Function : FreeUeContextSetupReq
 *
 *    Functionality: Deallocate the memory of BuildUESetReq
 *
 * @params[in]  F1AP_PDU_t  *f1apMsg
 *
 * @return void
 *
 *
 * ****************************************************************/
void FreeUeContextSetupReq(F1AP_PDU_t  *f1apMsg)
{
   uint8_t idx, ieId;
   UEContextSetupRequest_t  *ueSetReq = NULLP;

   if(f1apMsg != NULLP)
   {
      if(f1apMsg->choice.initiatingMessage != NULLP)
      {
         ueSetReq = &f1apMsg->choice.initiatingMessage->value.choice.UEContextSetupRequest;
         if(ueSetReq->protocolIEs.list.array != NULLP)
         {
            for(idx = 0; idx < ueSetReq->protocolIEs.list.count; idx++)
            {
               if(ueSetReq->protocolIEs.list.array[idx])
               {
                  switch(ueSetReq->protocolIEs.list.array[idx]->id)
                  {
                     case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
                        break;
                     case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
                        break;
                     case ProtocolIE_ID_id_SpCell_ID:
                        FreeNrcgi(&ueSetReq->protocolIEs.list.array[idx]->value.choice.NRCGI);
                        break;
                     case ProtocolIE_ID_id_ServCellIndex:
                        break;
                     case ProtocolIE_ID_id_SpCellULConfigured:
                        break;
                     case ProtocolIE_ID_id_CUtoDURRCInformation:
                        FreeCuToDuInfo(&ueSetReq->protocolIEs.list.array[idx]->value.choice.CUtoDURRCInformation);
                        break;
                     case ProtocolIE_ID_id_SCell_ToBeSetup_List:
                        FreeSplCellList(&ueSetReq->protocolIEs.list.array[idx]->value.choice.SCell_ToBeSetup_List);
                        break;
                     case ProtocolIE_ID_id_SRBs_ToBeSetup_List:
                        FreeSRBSetup(&ueSetReq->protocolIEs.list.array[idx]->value.choice.SRBs_ToBeSetup_List);
                        break;
                     case ProtocolIE_ID_id_DRBs_ToBeSetup_List:
                        FreeDRBSetup(&ueSetReq->protocolIEs.list.array[idx]->value.choice.DRBs_ToBeSetup_List);
                        break;
                     case ProtocolIE_ID_id_RRCContainer:
                        if(ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf != NULLP)
                        {
                           CU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf, \
                                 ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCContainer.size);
                        }
                        break;
                     case ProtocolIE_ID_id_GNB_DU_UE_AMBR_UL:
                        CU_FREE(ueSetReq->protocolIEs.list.array[idx]->value.choice.BitRate.buf,\
                              ueSetReq->protocolIEs.list.array[idx]->value.choice.BitRate.size);
                        break;
                     default:
                        DU_LOG("\nERROR  -->  F1AP: Invalid event type %ld", ueSetReq->protocolIEs.list.array[idx]->id);
                        break;
                  }
               }
               /*BUG: Break is causing to exit the for Loop before complete traversing and freeing of each IE*/
            }
            for(ieId=0; ieId<idx; ieId++)
            {
               if(ueSetReq->protocolIEs.list.array[ieId] != NULLP)
               {
                  CU_FREE(ueSetReq->protocolIEs.list.array[ieId],sizeof(UEContextSetupRequestIEs_t));
               }
            }
            CU_FREE(ueSetReq->protocolIEs.list.array,ueSetReq->protocolIEs.list.size);
         }
         CU_FREE(f1apMsg->choice.initiatingMessage,sizeof(InitiatingMessage_t));
      }
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
   }
}

/**Filling cell group info **/
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
   uint8_t BuildControlRSetToAddModList
(
 struct PDCCH_Config__controlResourceSetToAddModList *controlRSetList
 )
{
   uint8_t idx;
   uint8_t elementCnt;
   uint8_t numBytes, bitsUnused;
   struct ControlResourceSet *controlRSet;
   uint8_t freqDomainResource[FREQ_DOM_RSRC_SIZE] = {0};
   uint8_t coreset0EndPrb, coreset1StartPrb, coreset1NumPrb;

   elementCnt = 1;
   controlRSetList->list.count = elementCnt;
   controlRSetList->list.size = \
				elementCnt * sizeof(struct ControlResourceSet *);

   controlRSetList->list.array = NULLP;
   CU_ALLOC(controlRSetList->list.array, controlRSetList->list.size);
   if(!controlRSetList->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildControlRSetToAddModList");
      return RFAILED;
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
      controlRSetList->list.array[idx] = NULLP;
      CU_ALLOC(controlRSetList->list.array[idx], sizeof(struct ControlResourceSet));
      if(!controlRSetList->list.array[idx])
      {
	 DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildControlRSetToAddModList");
	 return RFAILED;
      }
   }

   idx=0;
   controlRSet = controlRSetList->list.array[idx];
   controlRSet->controlResourceSetId = PDCCH_CTRL_RSRC_SET_ONE_ID;

   /* Values harcoded according to our design:
    * size 6 bytes
    * 3 LSBs unsued
    * Bit string stored ff0000000000
    */
   numBytes = 6;
   bitsUnused = 3;
   controlRSet->frequencyDomainResources.size = numBytes * sizeof(uint8_t);
   controlRSet->frequencyDomainResources.buf = NULLP;
   CU_ALLOC(controlRSet->frequencyDomainResources.buf, \
	 controlRSet->frequencyDomainResources.size);
   if(!controlRSet->frequencyDomainResources.buf)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildControlRSetToAddModList");
      return RFAILED;
   }

   memset(controlRSet->frequencyDomainResources.buf, 0, FREQ_DOM_RSRC_SIZE);
   coreset0EndPrb = CORESET0_END_PRB;
   coreset1StartPrb = coreset0EndPrb + 6;
   coreset1NumPrb = CORESET1_NUM_PRB;
   /* calculate the PRBs */
   fillCoresetFeqDomAllocMap(((coreset1StartPrb)/6), (coreset1NumPrb/6), freqDomainResource);
   memcpy(controlRSet->frequencyDomainResources.buf, freqDomainResource, FREQ_DOM_RSRC_SIZE);
   controlRSet->frequencyDomainResources.bits_unused = bitsUnused;

   controlRSet->duration = PDCCH_CTRL_RSRC_SET_ONE_DURATION;
   controlRSet->cce_REG_MappingType.present = \
					      ControlResourceSet__cce_REG_MappingType_PR_nonInterleaved;

   controlRSet->precoderGranularity = PDCCH_CTRL_RSRC_SET_ONE_PRECOD_GRANULARITY;
   controlRSet->tci_StatesPDCCH_ToAddList = NULLP;
   controlRSet->tci_StatesPDCCH_ToReleaseList = NULLP;
   controlRSet->tci_PresentInDCI = NULLP;
#if 0
   uint8_t tciStateIdx;

   CU_ALLOC(controlRset->tci_StatesPDCCH_ToAddList, \
	 sizeof(struct ControlResourceSet__tci_StatesPDCCH_ToAddList));
   if(!controlRset->tci_StatesPDCCH_ToAddList)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildControlRSetToAddModList");
      return RFAILED;
   }

   elementCnt = 1;
   controlRset->tci_StatesPDCCH_ToAddList->list.count = elementCnt;
   controlRset->tci_StatesPDCCH_ToAddList->list.size = elementCnt * sizeof(TCI_StateId_t *);
   CU_ALLOC(controlRset->tci_StatesPDCCH_ToAddList->list.array, \
	 controlRset->tci_StatesPDCCH_ToAddList->list.size)
      if(!controlRset->tci_StatesPDCCH_ToAddList->list.array)
      {
	 DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildControlRSetToAddModList");
	 return RFAILED;
      }

   for(tciStateIdx = 0; tciStateIdx <elementCntl; tciStateIdx++)
   {
      CU_ALLOC(controlRset->tci_StatesPDCCH_ToAddList->list.array[tciStateIdx], sizeof(TCI_StateId_t));
      if(!controlRset->tci_StatesPDCCH_ToAddList->list.array[tciStateIdx])
      {
	 DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildControlRSetToAddModList");
	 return RFAILED;
      }
   }

   tciStateIdx = 0;
   /* TODO */
   *(controlRset->tci_StatesPDCCH_ToAddList->list.array[tciStateIdx]);

   CU_ALLOC(controlRset->tci_PresentInDCI, sizeof(long));
   if(!controlRset->tci_PresentInDCI)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildControlRSetToAddModList");
      return RFAILED;
   }
   /* TODO */
   *(controlRset->tci_PresentInDCI);
#endif

   controlRSet->pdcch_DMRS_ScramblingID = NULLP;
   CU_ALLOC(controlRSet->pdcch_DMRS_ScramblingID, sizeof(long));
   if(!controlRSet->pdcch_DMRS_ScramblingID)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildControlRSetToAddModList");
      return RFAILED;
   }
   *(controlRSet->pdcch_DMRS_ScramblingID) = SCRAMBLING_ID;

   return ROK;
} /* End BuildControlRSetToAddModList */

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
   uint8_t BuildSearchSpcToAddModList
(
 struct PDCCH_Config__searchSpacesToAddModList *searchSpcList
 )
{
   uint8_t idx;
   uint8_t numBytes;
   uint8_t byteIdx;
   uint8_t bitsUnused;
   uint8_t elementCnt;
   struct SearchSpace *searchSpc;

   elementCnt = 1;
   searchSpcList->list.count = elementCnt;
   searchSpcList->list.size = elementCnt * sizeof(struct SearchSpace *);

   searchSpcList->list.array = NULLP;
   CU_ALLOC(searchSpcList->list.array, searchSpcList->list.size);
   if(!searchSpcList->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
      searchSpcList->list.array[idx] = NULLP;
      CU_ALLOC(searchSpcList->list.array[idx], sizeof(struct SearchSpace));
      if(!searchSpcList->list.array[idx])
      {
	 DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSearchSpcToAddModList");
	 return RFAILED;
      }
   }

   idx = 0;
   searchSpc = searchSpcList->list.array[idx];

   searchSpc->searchSpaceId = PDCCH_SRCH_SPC_TWO_ID;

   searchSpc->controlResourceSetId = NULLP;
   CU_ALLOC(searchSpc->controlResourceSetId, sizeof(ControlResourceSetId_t));
   if(!searchSpc->controlResourceSetId)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }
   *(searchSpc->controlResourceSetId) = PDCCH_CTRL_RSRC_SET_ONE_ID;

   searchSpc->monitoringSlotPeriodicityAndOffset = NULLP;
   CU_ALLOC(searchSpc->monitoringSlotPeriodicityAndOffset, \
	 sizeof(struct SearchSpace__monitoringSlotPeriodicityAndOffset));
   if(!searchSpc->monitoringSlotPeriodicityAndOffset)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }
   searchSpc->monitoringSlotPeriodicityAndOffset->present = \
							    SearchSpace__monitoringSlotPeriodicityAndOffset_PR_sl1;

   searchSpc->duration = NULLP;
   searchSpc->monitoringSymbolsWithinSlot = NULLP;
   CU_ALLOC(searchSpc->monitoringSymbolsWithinSlot, sizeof(BIT_STRING_t));
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
   CU_ALLOC(searchSpc->monitoringSymbolsWithinSlot->buf, \
	 searchSpc->monitoringSymbolsWithinSlot->size);
   if(!searchSpc->monitoringSymbolsWithinSlot->buf)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }

   byteIdx = 0;
   searchSpc->monitoringSymbolsWithinSlot->buf[byteIdx++] = \
							    PDCCH_SYMBOL_WITHIN_SLOT /* setting MSB to 128 i.e. 0x80 */;
   searchSpc->monitoringSymbolsWithinSlot->buf[byteIdx++] = 0;
   searchSpc->monitoringSymbolsWithinSlot->bits_unused = bitsUnused;

   searchSpc->nrofCandidates = NULLP;
   CU_ALLOC(searchSpc->nrofCandidates, sizeof(struct SearchSpace__nrofCandidates));
   if(!searchSpc->nrofCandidates)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }

   searchSpc->nrofCandidates->aggregationLevel1 = \
						  PDCCH_SRCH_SPC_TWO_AGG_LVL1_CANDIDATE;
   searchSpc->nrofCandidates->aggregationLevel2 = \
						  PDCCH_SRCH_SPC_TWO_AGG_LVL2_CANDIDATE;
   searchSpc->nrofCandidates->aggregationLevel4 = \
						  PDCCH_SRCH_SPC_TWO_AGG_LVL4_CANDIDATE;
   searchSpc->nrofCandidates->aggregationLevel8 = \
						  PDCCH_SRCH_SPC_TWO_AGG_LVL8_CANDIDATE;
   searchSpc->nrofCandidates->aggregationLevel16 = \
						   PDCCH_SRCH_SPC_TWO_AGG_LVL16_CANDIDATE;

   searchSpc->searchSpaceType = NULLP;
   CU_ALLOC(searchSpc->searchSpaceType, sizeof(struct SearchSpace__searchSpaceType));
   if(!searchSpc->searchSpaceType)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }

   searchSpc->searchSpaceType->present = SearchSpace__searchSpaceType_PR_ue_Specific;

   searchSpc->searchSpaceType->choice.ue_Specific = NULLP;
   CU_ALLOC(searchSpc->searchSpaceType->choice.ue_Specific, \
	 sizeof(struct SearchSpace__searchSpaceType__ue_Specific));
   if(!searchSpc->searchSpaceType->choice.ue_Specific)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildSearchSpcToAddModList");
      return RFAILED;
   }  
   searchSpc->searchSpaceType->choice.ue_Specific->dci_Formats = \
								 PDCCH_SRCH_SPC_TWO_UE_SPEC_DCI_FORMAT;

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
uint8_t BuildBWPDlDedPdcchCfg(struct PDCCH_Config *pdcchCfg)
{
   pdcchCfg->controlResourceSetToAddModList = NULLP;
   CU_ALLOC(pdcchCfg->controlResourceSetToAddModList, \
	 sizeof(struct PDCCH_Config__controlResourceSetToAddModList));
   if(!pdcchCfg->controlResourceSetToAddModList)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPDlDedPdcchCfg");
      return RFAILED;
   }

   if(BuildControlRSetToAddModList(pdcchCfg->controlResourceSetToAddModList) != ROK)
   {
      return RFAILED;
   }

   pdcchCfg->controlResourceSetToReleaseList = NULLP;

   pdcchCfg->searchSpacesToAddModList = NULLP;
   CU_ALLOC(pdcchCfg->searchSpacesToAddModList, \
	 sizeof(struct PDCCH_Config__searchSpacesToAddModList));
   if(!pdcchCfg->searchSpacesToAddModList)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPDlDedPdcchCfg");
      return RFAILED;
   }

   if(BuildSearchSpcToAddModList(pdcchCfg->searchSpacesToAddModList) != ROK)
   {
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
 *
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
   uint8_t BuildDMRSDLPdschMapTypeA
(
 struct PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA *dmrsDlCfg
 )
{
   dmrsDlCfg->present = PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA_PR_setup;
   dmrsDlCfg->choice.setup = NULLP;
   CU_ALLOC(dmrsDlCfg->choice.setup, sizeof(struct DMRS_DownlinkConfig));
   if(!dmrsDlCfg->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPDlDedPdschCfg");
      return RFAILED;
   }

   dmrsDlCfg->choice.setup->dmrs_Type = NULLP;
   dmrsDlCfg->choice.setup->dmrs_AdditionalPosition = NULLP;
   CU_ALLOC(dmrsDlCfg->choice.setup->dmrs_AdditionalPosition, sizeof(long));
   if(!dmrsDlCfg->choice.setup->dmrs_AdditionalPosition)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDMRSDLPdschMapTypeA");
      return RFAILED;
   }
   *(dmrsDlCfg->choice.setup->dmrs_AdditionalPosition) = DMRS_ADDITIONAL_POS;

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
   uint8_t BuildPdschTimeDomAllocList
(
 struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAllocList
 )
{
   uint8_t idx;
   uint8_t elementCnt;
   struct PDSCH_TimeDomainResourceAllocation *timeDomAlloc;

   timeDomAllocList->present = \
			       PDSCH_Config__pdsch_TimeDomainAllocationList_PR_setup;

   timeDomAllocList->choice.setup = NULLP;
   CU_ALLOC(timeDomAllocList->choice.setup, \
	 sizeof(struct PDSCH_TimeDomainResourceAllocationList));
   if(!timeDomAllocList->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschTimeDomAllocList");
      return RFAILED;
   }

   elementCnt = 2;
   timeDomAllocList->choice.setup->list.count = elementCnt;
   timeDomAllocList->choice.setup->list.size = \
					       elementCnt * sizeof(struct PDSCH_TimeDomainResourceAllocation *);

   timeDomAllocList->choice.setup->list.array = NULLP;
   CU_ALLOC(timeDomAllocList->choice.setup->list.array, \
	 timeDomAllocList->choice.setup->list.size);
   if(!timeDomAllocList->choice.setup->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschTimeDomAllocList");
      return RFAILED;
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
      timeDomAllocList->choice.setup->list.array[idx] = NULLP;
      CU_ALLOC(timeDomAllocList->choice.setup->list.array[idx], \
	    sizeof(struct PDSCH_TimeDomainResourceAllocation));
      if(!timeDomAllocList->choice.setup->list.array[idx])
      {
	 DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschTimeDomAllocList");
	 return RFAILED;
      }
   }

   idx = 0;
   timeDomAlloc = timeDomAllocList->choice.setup->list.array[idx];
   CU_ALLOC(timeDomAlloc->k0, sizeof(long));
   if(!timeDomAlloc->k0)
   {
       DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschTimeDomAllocList");
       return RFAILED;
   }
   *(timeDomAlloc->k0) = 0;
   timeDomAlloc->mappingType = PDSCH_MAPPING_TYPE_A;
   timeDomAlloc->startSymbolAndLength = 66;

   idx++;
   timeDomAlloc = timeDomAllocList->choice.setup->list.array[idx];
   CU_ALLOC(timeDomAlloc->k0, sizeof(long));
   if(!timeDomAlloc->k0)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschTimeDomAllocList");
      return RFAILED;
   }
   *(timeDomAlloc->k0) = 1;
   timeDomAlloc->mappingType = PDSCH_MAPPING_TYPE_A;
   timeDomAlloc->startSymbolAndLength = 66;

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
   uint8_t BuildPdschPrbBundlingType
(
 struct PDSCH_Config__prb_BundlingType *prbBndlType
 )
{
   prbBndlType->present = PDSCH_Config__prb_BundlingType_PR_staticBundling;

   prbBndlType->choice.staticBundling = NULLP;
   CU_ALLOC(prbBndlType->choice.staticBundling, \
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
uint8_t BuildBWPDlDedPdschCfg(struct PDSCH_Config *pdschCfg)
{
   pdschCfg->dataScramblingIdentityPDSCH = NULLP;

   pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA = NULLP;
   CU_ALLOC(pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA, \
	 sizeof(struct PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA));
   if(!pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPDlDedPdschCfg");
      return RFAILED;
   }

   if(BuildDMRSDLPdschMapTypeA(pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA) != ROK)
   {
      return RFAILED;
   }

   pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeB = NULLP;
   pdschCfg->tci_StatesToAddModList = NULLP;
   pdschCfg->tci_StatesToReleaseList = NULLP;
   pdschCfg->vrb_ToPRB_Interleaver = NULLP;
#if 0
   CU_ALLOC(pdschCfg->tci_StatesToAddModList, sizeof(struct PDSCH_Config__tci_StatesToAddModList));
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

   pdschCfg->resourceAllocation = RES_ALLOC_TYPE;

   pdschCfg->pdsch_TimeDomainAllocationList = NULLP;
   CU_ALLOC(pdschCfg->pdsch_TimeDomainAllocationList, \
	 sizeof(struct PDSCH_Config__pdsch_TimeDomainAllocationList));
   if(!pdschCfg->pdsch_TimeDomainAllocationList)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPDlDedPdschCfg");
      return RFAILED;
   }
   if(BuildPdschTimeDomAllocList(pdschCfg->pdsch_TimeDomainAllocationList) != ROK)
   {
      return RFAILED;
   }
   pdschCfg->pdsch_AggregationFactor = NULLP;
   pdschCfg->rateMatchPatternToAddModList = NULLP;
   pdschCfg->rateMatchPatternToReleaseList = NULLP;
   pdschCfg->rateMatchPatternGroup1 = NULLP;
   pdschCfg->rateMatchPatternGroup2 = NULLP;
   pdschCfg->rbg_Size = PDSCH_RBG_SIZE;
   pdschCfg->mcs_Table = NULLP;

   pdschCfg->maxNrofCodeWordsScheduledByDCI = NULLP;
   CU_ALLOC(pdschCfg->maxNrofCodeWordsScheduledByDCI, sizeof(long));
   if(!pdschCfg->maxNrofCodeWordsScheduledByDCI)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPDlDedPdschCfg");
      return RFAILED;
   }
   *(pdschCfg->maxNrofCodeWordsScheduledByDCI) = PDSCH_MAX_CODEWORD_SCH_BY_DCI;

   if(BuildPdschPrbBundlingType(&pdschCfg->prb_BundlingType) != ROK)
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
uint8_t BuildInitialDlBWP(BWP_DownlinkDedicated_t *dlBwp)
{
   dlBwp->pdcch_Config = NULLP;
   CU_ALLOC(dlBwp->pdcch_Config, sizeof(struct BWP_DownlinkDedicated__pdcch_Config));
   if(!dlBwp->pdcch_Config)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory Allocation failure in BuildInitialDlBWP");
      return RFAILED;
   }
   dlBwp->pdcch_Config->present = BWP_DownlinkDedicated__pdcch_Config_PR_setup; 

   dlBwp->pdcch_Config->choice.setup = NULLP;
   CU_ALLOC(dlBwp->pdcch_Config->choice.setup, sizeof(struct PDCCH_Config));
   if(!dlBwp->pdcch_Config->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory Allocation failure in BuildInitialDlBWP");
      return RFAILED;
   }
   if(BuildBWPDlDedPdcchCfg(dlBwp->pdcch_Config->choice.setup) != ROK)
   {
      return RFAILED;
   }

   dlBwp->pdsch_Config = NULLP;
   CU_ALLOC(dlBwp->pdsch_Config, sizeof(struct BWP_DownlinkDedicated__pdsch_Config));
   if(!dlBwp->pdsch_Config)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory Allocation failure in BuildInitialDlBWP");
      return RFAILED;
   }
   dlBwp->pdsch_Config->present = BWP_DownlinkDedicated__pdsch_Config_PR_setup;

   dlBwp->pdsch_Config->choice.setup = NULLP;
   CU_ALLOC(dlBwp->pdsch_Config->choice.setup, sizeof(struct PDSCH_Config));
   if(!dlBwp->pdsch_Config->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory Allocation failure in BuildInitialDlBWP");
      return RFAILED;
   }

   if(BuildBWPDlDedPdschCfg(dlBwp->pdsch_Config->choice.setup) != ROK)
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
   uint8_t BuildDMRSULPuschMapTypeA
(
 struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA *dmrsUlCfg
 )
{
   dmrsUlCfg->present = PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA_PR_setup;
   dmrsUlCfg->choice.setup= NULLP;
   CU_ALLOC(dmrsUlCfg->choice.setup, sizeof(DMRS_UplinkConfig_t));
   if(!dmrsUlCfg->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDMRSULPuschMapTypeA");
      return RFAILED;
   }

   dmrsUlCfg->choice.setup->dmrs_Type = NULLP;
   dmrsUlCfg->choice.setup->dmrs_AdditionalPosition = NULLP;
   CU_ALLOC(dmrsUlCfg->choice.setup->dmrs_AdditionalPosition, sizeof(long));
   if(!dmrsUlCfg->choice.setup->dmrs_AdditionalPosition)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDMRSULPuschMapTypeA");
      return RFAILED;
   }
   *(dmrsUlCfg->choice.setup->dmrs_AdditionalPosition) = DMRS_ADDITIONAL_POS; 

   dmrsUlCfg->choice.setup->phaseTrackingRS = NULLP;
   dmrsUlCfg->choice.setup->maxLength = NULLP;
   dmrsUlCfg->choice.setup->transformPrecodingDisabled = NULLP;
   CU_ALLOC(dmrsUlCfg->choice.setup->transformPrecodingDisabled, \
	 sizeof(struct DMRS_UplinkConfig__transformPrecodingDisabled));
   if(!dmrsUlCfg->choice.setup->transformPrecodingDisabled)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDMRSULPuschMapTypeA");
      return RFAILED;
   }

   dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0 = NULLP;
   CU_ALLOC(dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0,\
	 sizeof(long));
   if(!dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDMRSULPuschMapTypeA");
      return RFAILED;
   }
   *(dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0) = SCRAMBLING_ID;

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
   uint8_t BuildPuschTimeDomAllocList
(
 struct PUSCH_Config__pusch_TimeDomainAllocationList *timeDomAllocList
 )
{
   uint8_t idx;
   uint8_t elementCnt;
   PUSCH_TimeDomainResourceAllocation_t  *timeDomAlloc;

   timeDomAllocList->present = PUSCH_Config__pusch_TimeDomainAllocationList_PR_setup;
   timeDomAllocList->choice.setup = NULLP;
   CU_ALLOC(timeDomAllocList->choice.setup, \
	 sizeof(struct PUSCH_TimeDomainResourceAllocationList));
   if(!timeDomAllocList->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPuschTimeDomAllocList");
      return RFAILED;
   }

   elementCnt = 2;
   timeDomAllocList->choice.setup->list.count = elementCnt;
   timeDomAllocList->choice.setup->list.size = \
					       elementCnt * sizeof(PUSCH_TimeDomainResourceAllocation_t *);
   timeDomAllocList->choice.setup->list.array = NULLP;
   CU_ALLOC(timeDomAllocList->choice.setup->list.array, \
	 timeDomAllocList->choice.setup->list.size);
   if(!timeDomAllocList->choice.setup->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPuschTimeDomAllocList");
      return RFAILED;
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
      timeDomAllocList->choice.setup->list.array[idx] = NULLP;
      CU_ALLOC(timeDomAllocList->choice.setup->list.array[idx],\
	    sizeof(PUSCH_TimeDomainResourceAllocation_t));
      if(!timeDomAllocList->choice.setup->list.array[idx])
      {
	 DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPuschTimeDomAllocList");
	 return RFAILED;
      }
   }

   idx = 0;
   timeDomAlloc = timeDomAllocList->choice.setup->list.array[idx];
   CU_ALLOC(timeDomAlloc->k2, sizeof(long));
   if(!timeDomAlloc->k2)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPuschTimeDomAllocList");
      return RFAILED;
   }
   *(timeDomAlloc->k2) = PUSCH_K2_CFG1;
   timeDomAlloc->mappingType = PUSCH_MAPPING_TYPE_A;
   timeDomAlloc->startSymbolAndLength = 66; 

   idx++;
   timeDomAlloc = timeDomAllocList->choice.setup->list.array[idx];
   CU_ALLOC(timeDomAlloc->k2, sizeof(long));
   if(!timeDomAlloc->k2)
   {
       DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPuschTimeDomAllocList");
       return RFAILED;
   }
   *(timeDomAlloc->k2) = PUSCH_K2_CFG2;
   timeDomAlloc->mappingType = PUSCH_MAPPING_TYPE_A;
   timeDomAlloc->startSymbolAndLength = 66;

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
uint8_t BuildBWPUlDedPuschCfg(PUSCH_Config_t *puschCfg)
{
   puschCfg->dataScramblingIdentityPUSCH = NULLP;
   CU_ALLOC(puschCfg->dataScramblingIdentityPUSCH, sizeof(long));
   if(!puschCfg->dataScramblingIdentityPUSCH)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPuschCfg");
      return RFAILED;
   }
   *(puschCfg->dataScramblingIdentityPUSCH) = SCRAMBLING_ID;

   puschCfg->txConfig = NULLP;
   puschCfg->dmrs_UplinkForPUSCH_MappingTypeA = NULLP;
   CU_ALLOC(puschCfg->dmrs_UplinkForPUSCH_MappingTypeA, \
	 sizeof(struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA));
   if(!puschCfg->dmrs_UplinkForPUSCH_MappingTypeA)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPuschCfg");
      return RFAILED;
   }

   if(BuildDMRSULPuschMapTypeA(puschCfg->dmrs_UplinkForPUSCH_MappingTypeA) != ROK)
   {
      return RFAILED;
   }

   puschCfg->dmrs_UplinkForPUSCH_MappingTypeB = NULLP;
   puschCfg->pusch_PowerControl = NULLP;
   puschCfg->frequencyHopping = NULLP;
   puschCfg->frequencyHoppingOffsetLists = NULLP;
   puschCfg->resourceAllocation = RES_ALLOC_TYPE;

   puschCfg->pusch_TimeDomainAllocationList = NULLP;
   CU_ALLOC(puschCfg->pusch_TimeDomainAllocationList, \
	 sizeof(struct PUSCH_Config__pusch_TimeDomainAllocationList));
   if(!puschCfg->pusch_TimeDomainAllocationList)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPuschCfg");
      return RFAILED;
   }

   if(BuildPuschTimeDomAllocList(puschCfg->pusch_TimeDomainAllocationList) != ROK)
   {
      return RFAILED;
   }

   puschCfg->pusch_AggregationFactor = NULLP;
   puschCfg->mcs_Table = NULLP;
   puschCfg->mcs_TableTransformPrecoder = NULLP;
   puschCfg->transformPrecoder = NULLP;
   CU_ALLOC(puschCfg->transformPrecoder, sizeof(long));
   if(!puschCfg->transformPrecoder)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPuschCfg");
      return RFAILED;
   }
   *(puschCfg->transformPrecoder) = PUSCH_TRANSFORM_PRECODER;

   puschCfg->codebookSubset = NULLP;
   puschCfg->maxRank = NULLP;
   puschCfg->rbg_Size = NULLP;
   puschCfg->uci_OnPUSCH = NULLP;
   puschCfg->tp_pi2BPSK = NULLP;

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
uint8_t BuildBWPUlDedPucchCfg(PUCCH_Config_t *pucchCfg)
{
   uint8_t arrIdx, elementCnt;
   uint8_t rsrcIdx, rsrcSetIdx;
   PUCCH_ResourceSet_t *rsrcSet = NULLP;
   PUCCH_Resource_t *rsrc = NULLP;

   //RESOURCE SET
   elementCnt = 1;
   CU_ALLOC(pucchCfg->resourceSetToAddModList, sizeof(struct PUCCH_Config__resourceSetToAddModList));
   pucchCfg->resourceSetToAddModList->list.count = elementCnt;
   pucchCfg->resourceSetToAddModList->list.size = elementCnt * sizeof(PUCCH_ResourceSet_t *);
   CU_ALLOC(pucchCfg->resourceSetToAddModList->list.array, pucchCfg->resourceSetToAddModList->list.size);
   for(rsrcSetIdx=0; rsrcSetIdx < pucchCfg->resourceSetToAddModList->list.count; rsrcSetIdx++)
   {
      CU_ALLOC(pucchCfg->resourceSetToAddModList->list.array[rsrcSetIdx], sizeof(PUCCH_ResourceSet_t));
   }
   rsrcSetIdx = 0;
   rsrcSet = pucchCfg->resourceSetToAddModList->list.array[rsrcSetIdx];
   rsrcSet->pucch_ResourceSetId = 1;
   elementCnt = 1;
   rsrcSet->resourceList.list.count = elementCnt;
   rsrcSet->resourceList.list.size = elementCnt * sizeof(PUCCH_ResourceId_t *);
   CU_ALLOC(rsrcSet->resourceList.list.array, rsrcSet->resourceList.list.size);
   for(rsrcIdx=0; rsrcIdx < rsrcSet->resourceList.list.count; rsrcIdx++)
   {
      CU_ALLOC(rsrcSet->resourceList.list.array[rsrcIdx], sizeof(PUCCH_ResourceId_t));
   }
   rsrcIdx = 0;
   *(rsrcSet->resourceList.list.array[rsrcIdx]) = 1;

   //RESOURCE
   elementCnt = 1;
   CU_ALLOC(pucchCfg->resourceToAddModList, sizeof(struct PUCCH_Config__resourceToAddModList));
   pucchCfg->resourceToAddModList->list.count = elementCnt;
   pucchCfg->resourceToAddModList->list.size = elementCnt * sizeof(PUCCH_Resource_t *);
   CU_ALLOC(pucchCfg->resourceToAddModList->list.array, pucchCfg->resourceToAddModList->list.size);
   for(rsrcIdx=0; rsrcIdx < pucchCfg->resourceToAddModList->list.count; rsrcIdx++)
   {
      CU_ALLOC(pucchCfg->resourceToAddModList->list.array[rsrcIdx], sizeof(PUCCH_Resource_t));
   }
   rsrcIdx = 0;
   rsrc = pucchCfg->resourceToAddModList->list.array[rsrcIdx];
   rsrc->pucch_ResourceId = 1;
   rsrc->startingPRB = 0;
   rsrc->format.present = PUCCH_Resource__format_PR_format1; 
   CU_ALLOC(rsrc->format.choice.format1, sizeof(PUCCH_format1_t));
   rsrc->format.choice.format1->initialCyclicShift = 0;
   rsrc->format.choice.format1->nrofSymbols = 4;
   rsrc->format.choice.format1->startingSymbolIndex = 0;
   rsrc->format.choice.format1->timeDomainOCC = 0;

   //PUCCH Format 1
   CU_ALLOC(pucchCfg->format1, sizeof(struct PUCCH_Config__format1));
   pucchCfg->format1->present = PUCCH_Config__format1_PR_setup;
   CU_ALLOC(pucchCfg->format1->choice.setup, sizeof(PUCCH_FormatConfig_t));
   CU_ALLOC(pucchCfg->format1->choice.setup->nrofSlots, sizeof(long));
   *(pucchCfg->format1->choice.setup->nrofSlots) = PUCCH_FormatConfig__nrofSlots_n4;

   //DL DATA TO UL ACK
   CU_ALLOC(pucchCfg->dl_DataToUL_ACK, sizeof(struct PUCCH_Config__dl_DataToUL_ACK));
   if(pucchCfg->dl_DataToUL_ACK == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
      return RFAILED;
   }

   elementCnt = 2;
   pucchCfg->dl_DataToUL_ACK->list.count = elementCnt;
   pucchCfg->dl_DataToUL_ACK->list.size = elementCnt * sizeof(long *);
   CU_ALLOC(pucchCfg->dl_DataToUL_ACK->list.array, pucchCfg->dl_DataToUL_ACK->list.size);
   if(pucchCfg->dl_DataToUL_ACK->list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
      return RFAILED;
   }

   for(arrIdx = 0; arrIdx <  pucchCfg->dl_DataToUL_ACK->list.count; arrIdx++)
   {
      CU_ALLOC(pucchCfg->dl_DataToUL_ACK->list.array[arrIdx], sizeof(long));
      if(pucchCfg->dl_DataToUL_ACK->list.array[arrIdx] == NULLP)
      {
          DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildBWPUlDedPucchCfg");
          return RFAILED;
      }
   }

   arrIdx = 0;
   *(pucchCfg->dl_DataToUL_ACK->list.array[arrIdx++]) = 4;
   *(pucchCfg->dl_DataToUL_ACK->list.array[arrIdx]) = 5;

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
   CU_ALLOC(resourceList->list.array, resourceList->list.size);
   if(!resourceList->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed in BuildSrsRsrcAddModList");
      return RFAILED;
   }

   for(rsrcIdx = 0; rsrcIdx < resourceList->list.count; rsrcIdx++)
   {
      CU_ALLOC(resourceList->list.array[rsrcIdx], sizeof(SRS_Resource_t));
      if(!resourceList->list.array[rsrcIdx])
      {
	 DU_LOG("\nERROR  -->  F1AP: Memory allocation failed in BuildSrsRsrcAddModList");
	 return RFAILED;
      }
   }

   rsrcIdx = 0;
   resourceList->list.array[rsrcIdx]->srs_ResourceId = SRS_RSRC_ID;
   resourceList->list.array[rsrcIdx]->nrofSRS_Ports = SRS_Resource__nrofSRS_Ports_port1;
   resourceList->list.array[rsrcIdx]->transmissionComb.present = SRS_Resource__transmissionComb_PR_n2;

   resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2 = NULLP;
   CU_ALLOC(resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2, \
	 sizeof(struct SRS_Resource__transmissionComb__n2));
   if(!resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed in BuildSrsRsrcAddModList");
      return RFAILED;
   }
   resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2->combOffset_n2\
      = SRS_COMB_OFFSET_N2;
   resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2->cyclicShift_n2\
      = SRS_CYCLIC_SHIFT_N2;

   resourceList->list.array[rsrcIdx]->resourceMapping.startPosition = PUSCH_START_SYMBOL;
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
   CU_ALLOC(resourceList->list.array[rsrcIdx]->resourceType.choice.aperiodic,
	 sizeof(struct SRS_Resource__resourceType__aperiodic));
   if(!resourceList->list.array[rsrcIdx]->resourceType.choice.aperiodic)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed in BuildSrsRsrcAddModList");
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
   CU_ALLOC(rsrcSetList->list.array, rsrcSetList->list.size);
   if(!rsrcSetList->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed in BuildSrsRsrcSetAddModList");
      return RFAILED;
   }

   for(rSetIdx = 0; rSetIdx < rsrcSetList->list.count; rSetIdx++)
   {
      CU_ALLOC(rsrcSetList->list.array[rSetIdx], sizeof(SRS_ResourceSet_t));
      if(!rsrcSetList->list.array[rSetIdx])
      {
	 DU_LOG("\nERROR  -->  F1AP: Memory allocation failed in BuildSrsRsrcSetAddModList");
	 return RFAILED;
      }
   }

   rSetIdx = 0;
   rsrcSetList->list.array[rSetIdx]->srs_ResourceSetId = SRS_RSET_ID;

   /* Fill Resource Id list in resource set */
   rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList = NULLP;
   CU_ALLOC(rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList,\
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
   CU_ALLOC(rsrcIdList->list.array, rsrcIdList->list.size);
   if(!rsrcIdList->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed in BuildSrsRsrcSetAddModList");
      return RFAILED;
   }

   for(rsrcIdx = 0; rsrcIdx < rsrcIdList->list.count; rsrcIdx++)
   {
      CU_ALLOC(rsrcIdList->list.array[rsrcIdx], sizeof(SRS_ResourceId_t));
      if(!rsrcIdList->list.array[rsrcIdx])
      {
	 DU_LOG("\nERROR  -->  F1AP: Memory allocation failed in BuildSrsRsrcSetAddModList");
	 return RFAILED;
      }
   }

   rsrcIdx = 0;
   *rsrcIdList->list.array[rsrcIdx] = SRS_RSRC_ID;

   /* Fill resource type */
   rsrcSetList->list.array[rSetIdx]->resourceType.present = \
							    SRS_ResourceSet__resourceType_PR_aperiodic;

   rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic = NULLP;
   CU_ALLOC(rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic, \
	 sizeof(struct SRS_ResourceSet__resourceType__aperiodic));
   if(!rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed in BuildSrsRsrcSetAddModList");
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
   CU_ALLOC(srsCfg->srs_ResourceSetToAddModList, \
	 sizeof(struct SRS_Config__srs_ResourceSetToAddModList));
   if(!srsCfg->srs_ResourceSetToAddModList)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed in BuildBWPUlDedSrsCfg");
      return RFAILED;
   }
   if(BuildSrsRsrcSetAddModList(srsCfg->srs_ResourceSetToAddModList) != ROK)
   {
      return RFAILED;
   }

   srsCfg->srs_ResourceToReleaseList = NULLP;

   /* Resource to Add/Modify list */
   srsCfg->srs_ResourceToAddModList = NULLP;
   CU_ALLOC(srsCfg->srs_ResourceToAddModList, \
	 sizeof(struct SRS_Config__srs_ResourceToAddModList));
   if(!srsCfg->srs_ResourceToAddModList)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed in BuildBWPUlDedSrsCfg");
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
uint8_t BuildInitialUlBWP(BWP_UplinkDedicated_t *ulBwp)
{
   ulBwp->pucch_Config = NULLP;
   ulBwp->pucch_Config = NULLP;
   CU_ALLOC(ulBwp->pucch_Config, sizeof(struct BWP_UplinkDedicated__pucch_Config));
   if(!ulBwp->pucch_Config)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildInitialUlBWP");
      return RFAILED;
   }

   ulBwp->pucch_Config->present = BWP_UplinkDedicated__pucch_Config_PR_setup;
   ulBwp->pucch_Config->choice.setup = NULLP;
   CU_ALLOC(ulBwp->pucch_Config->choice.setup, sizeof(PUCCH_Config_t));
   if(!ulBwp->pucch_Config->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildInitialUlBWP");
      return RFAILED;
   }

   if(BuildBWPUlDedPucchCfg(ulBwp->pucch_Config->choice.setup) != ROK)
   {
      return RFAILED;
   }

   /* Fill BWP UL dedicated PUSCH config */
   ulBwp->pusch_Config = NULLP;
   CU_ALLOC(ulBwp->pusch_Config, sizeof(struct BWP_UplinkDedicated__pusch_Config));
   if(!ulBwp->pusch_Config)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildInitialUlBWP");
      return RFAILED;
   }

   ulBwp->pusch_Config->present = BWP_UplinkDedicated__pusch_Config_PR_setup;
   ulBwp->pusch_Config->choice.setup = NULLP;
   CU_ALLOC(ulBwp->pusch_Config->choice.setup, sizeof(PUSCH_Config_t));
   if(!ulBwp->pusch_Config->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildInitialUlBWP");
      return RFAILED;
   }

   if(BuildBWPUlDedPuschCfg(ulBwp->pusch_Config->choice.setup) != ROK)
   {
      return RFAILED;
   }

   ulBwp->configuredGrantConfig = NULLP;

   /* Fill BPW UL dedicated SRS config */
   ulBwp->srs_Config = NULLP;
   CU_ALLOC(ulBwp->srs_Config, sizeof(struct BWP_UplinkDedicated__srs_Config));
   if(!ulBwp->srs_Config)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildInitialUlBWP");
      return RFAILED;
   }

   ulBwp->srs_Config->present = BWP_UplinkDedicated__srs_Config_PR_setup;
   ulBwp->srs_Config->choice.setup = NULLP;
   CU_ALLOC(ulBwp->srs_Config->choice.setup, sizeof(SRS_Config_t));
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
   CU_ALLOC(puschCfg->choice.setup, sizeof(struct PUSCH_ServingCellConfig));
   if(!puschCfg->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPuschSrvCellCfg");
      return RFAILED;
   }

   puschCfg->choice.setup->codeBlockGroupTransmission = NULLP;
   puschCfg->choice.setup->rateMatching = NULLP;
   puschCfg->choice.setup->xOverhead = NULLP;
   puschCfg->choice.setup->ext1 = NULLP;
   CU_ALLOC(puschCfg->choice.setup->ext1, sizeof(struct PUSCH_ServingCellConfig__ext1));
   if(!puschCfg->choice.setup->ext1)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPuschSrvCellCfg");
      return RFAILED;
   }

   puschCfg->choice.setup->ext1->maxMIMO_Layers = NULLP;
   CU_ALLOC(puschCfg->choice.setup->ext1->maxMIMO_Layers, sizeof(long));
   if(!puschCfg->choice.setup->ext1->maxMIMO_Layers)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPuschSrvCellCfg");
      return RFAILED;
   }
   *(puschCfg->choice.setup->ext1->maxMIMO_Layers) = PUSCH_MAX_MIMO_LAYERS;

   puschCfg->choice.setup->ext1->processingType2Enabled= NULLP;
   CU_ALLOC(puschCfg->choice.setup->ext1->processingType2Enabled,sizeof(BOOLEAN_t));
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
uint8_t BuildUlCfg(UplinkConfig_t *ulCfg)
{
   ulCfg->initialUplinkBWP = NULLP;
   CU_ALLOC(ulCfg->initialUplinkBWP, sizeof(BWP_UplinkDedicated_t));
   if(!ulCfg->initialUplinkBWP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory Allocation failed in BuildUlCfg");
      return RFAILED;
   }

   if(BuildInitialUlBWP(ulCfg->initialUplinkBWP) != ROK)
   {
      return RFAILED;
   }

   ulCfg->uplinkBWP_ToReleaseList = NULLP;
   ulCfg->uplinkBWP_ToAddModList = NULLP;
   ulCfg->firstActiveUplinkBWP_Id = NULLP;
   CU_ALLOC(ulCfg->firstActiveUplinkBWP_Id, sizeof(BWP_Id_t));
   if(!ulCfg->firstActiveUplinkBWP_Id)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory Allocation failed in BuildUlCfg");
      return RFAILED;
   }
   *(ulCfg->firstActiveUplinkBWP_Id) = ACTIVE_UL_BWP_ID;

   ulCfg->pusch_ServingCellConfig = NULLP;
   CU_ALLOC(ulCfg->pusch_ServingCellConfig, \
	 sizeof(struct UplinkConfig__pusch_ServingCellConfig));
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
uint8_t BuildPdschSrvCellCfg(struct ServingCellConfig__pdsch_ServingCellConfig *pdschCfg)
{
   pdschCfg->present =  ServingCellConfig__pdsch_ServingCellConfig_PR_setup;
   pdschCfg->choice.setup = NULLP;
   CU_ALLOC(pdschCfg->choice.setup, sizeof( struct PDSCH_ServingCellConfig));
   if(!pdschCfg->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschSrvCellCfg");
      return RFAILED;
   }

   pdschCfg->choice.setup->codeBlockGroupTransmission = NULLP;
   pdschCfg->choice.setup->xOverhead = NULLP;
   pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH = NULLP;
   CU_ALLOC(pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH, sizeof(long));
   if(!pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildPdschSrvCellCfg");
      return RFAILED;
   }
   *(pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH)= PDSCH_NUM_HARQ_PROC;
   pdschCfg->choice.setup->pucch_Cell = NULLP;
   pdschCfg->choice.setup->ext1 = NULLP;

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
uint8_t BuildSpCellCfgDed(ServingCellConfig_t *srvCellCfg)
{
   srvCellCfg->tdd_UL_DL_ConfigurationDedicated = NULLP;

   srvCellCfg->initialDownlinkBWP = NULLP;
   CU_ALLOC(srvCellCfg->initialDownlinkBWP, sizeof(BWP_DownlinkDedicated_t));
   if(!srvCellCfg->initialDownlinkBWP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSpCellCfgDed");
      return RFAILED;
   }

   if(BuildInitialDlBWP(srvCellCfg->initialDownlinkBWP) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : BuildInitialDlBWP failed");
      return RFAILED;
   }
   srvCellCfg->downlinkBWP_ToReleaseList = NULLP;
   srvCellCfg->downlinkBWP_ToAddModList = NULLP;

   srvCellCfg->firstActiveDownlinkBWP_Id = NULLP;
   CU_ALLOC(srvCellCfg->firstActiveDownlinkBWP_Id, sizeof(long));
   if(!srvCellCfg->firstActiveDownlinkBWP_Id)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSpCellCfgDed");
      return RFAILED;
   }
   *(srvCellCfg->firstActiveDownlinkBWP_Id) = ACTIVE_DL_BWP_ID;

   srvCellCfg->bwp_InactivityTimer = NULLP;

   srvCellCfg->defaultDownlinkBWP_Id = NULLP;
   CU_ALLOC(srvCellCfg->defaultDownlinkBWP_Id, sizeof(long));
   if(!srvCellCfg->defaultDownlinkBWP_Id)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSpCellCfgDed");
      return RFAILED;
   }
   *(srvCellCfg->defaultDownlinkBWP_Id) = ACTIVE_DL_BWP_ID;

   srvCellCfg->uplinkConfig = NULLP;
   CU_ALLOC(srvCellCfg->uplinkConfig, sizeof(UplinkConfig_t));
   if(!srvCellCfg->uplinkConfig)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSpCellCfgDed");
      return RFAILED;
   }

   if(BuildUlCfg(srvCellCfg->uplinkConfig) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : BuildUlCfg failed");
      return RFAILED;
   }
   srvCellCfg->supplementaryUplink = NULLP;
   srvCellCfg->pdcch_ServingCellConfig = NULLP;

   srvCellCfg->pdsch_ServingCellConfig = NULLP;
   CU_ALLOC(srvCellCfg->pdsch_ServingCellConfig, sizeof(struct	ServingCellConfig__pdsch_ServingCellConfig));
   if(!srvCellCfg->pdsch_ServingCellConfig)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSpCellCfgDed");
      return RFAILED;
   }

   if(BuildPdschSrvCellCfg(srvCellCfg->pdsch_ServingCellConfig) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : BuildPdschSrvCellCfg failed");
      return RFAILED;
   }

   srvCellCfg->csi_MeasConfig = NULLP;
#if 0
   CU_ALLOC(srvCellCfg->csi_MeasConfig, sizeof(struct	ServingCellConfig__csi_MeasConfig))
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
uint8_t BuildSpCellCfg(SpCellConfig_t *spCellCfg)
{

   spCellCfg->servCellIndex = NULLP;
   CU_ALLOC(spCellCfg->servCellIndex, sizeof(long));
   if(!spCellCfg->servCellIndex)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSpCellCfg");
      return RFAILED;
   }
   *(spCellCfg->servCellIndex) = SERV_CELL_IDX;

   spCellCfg->reconfigurationWithSync = NULLP;
   spCellCfg->rlf_TimersAndConstants = NULLP;
   spCellCfg->rlmInSyncOutOfSyncThreshold = NULLP;
   CU_ALLOC(spCellCfg->rlmInSyncOutOfSyncThreshold, sizeof(long));
   if(!spCellCfg->rlmInSyncOutOfSyncThreshold)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSpCellCfg");
      return RFAILED;
   }
   *(spCellCfg->rlmInSyncOutOfSyncThreshold) = RLM_SYNC_OUT_SYNC_THRESHOLD;

   spCellCfg->spCellConfigDedicated = NULLP;
   CU_ALLOC(spCellCfg->spCellConfigDedicated, sizeof(ServingCellConfig_t));
   if(!spCellCfg->spCellConfigDedicated)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSpCellCfg");
      return RFAILED;
   }
   if(BuildSpCellCfgDed(spCellCfg->spCellConfigDedicated) != ROK)
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
uint8_t BuildPhyCellGrpCfg(PhysicalCellGroupConfig_t *phyCellGrpCfg)
{
   phyCellGrpCfg->harq_ACK_SpatialBundlingPUCCH = NULLP;
   phyCellGrpCfg->harq_ACK_SpatialBundlingPUSCH = NULLP;

   phyCellGrpCfg->p_NR_FR1 = NULLP;
   CU_ALLOC(phyCellGrpCfg->p_NR_FR1, sizeof(long));
   if(!phyCellGrpCfg->p_NR_FR1)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildPhyCellGrpCfg");
      return RFAILED;
   }
   *(phyCellGrpCfg->p_NR_FR1)             = P_NR_FR1;
   phyCellGrpCfg->pdsch_HARQ_ACK_Codebook = PDSCH_HARQ_ACK_CODEBOOK;
   phyCellGrpCfg->tpc_SRS_RNTI = NULLP;
   phyCellGrpCfg->tpc_PUCCH_RNTI = NULLP;
   phyCellGrpCfg->tpc_PUSCH_RNTI = NULLP;
   phyCellGrpCfg->sp_CSI_RNTI = NULLP;
   phyCellGrpCfg->cs_RNTI = NULLP;
   phyCellGrpCfg->ext1 = NULLP;
   phyCellGrpCfg->ext2 = NULLP;

   return ROK;
}

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
uint8_t BuildTagConfig(struct TAG_Config *tagConfig)
{
   struct TAG_Config__tag_ToAddModList *tagList;
   uint8_t                     idx, elementCnt;

   tagConfig->tag_ToReleaseList = NULLP;
   tagConfig->tag_ToAddModList = NULLP;
   CU_ALLOC(tagConfig->tag_ToAddModList, sizeof(struct TAG_Config__tag_ToAddModList));
   if(!tagConfig->tag_ToAddModList)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildTagConfig");
      return RFAILED;
   }

   elementCnt = 1; //ODU_VALUE_ONE;
   tagList = tagConfig->tag_ToAddModList;
   tagList->list.count = elementCnt;
   tagList->list.size  =  elementCnt * sizeof(struct TAG *);

   tagList->list.array = NULLP;
   CU_ALLOC(tagList->list.array, tagList->list.size);
   if(!tagList->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildTagConfig");
      return RFAILED;
   }

   for(idx=0; idx<tagList->list.count; idx++)
   {
      tagList->list.array[idx] = NULLP;
      CU_ALLOC(tagList->list.array[idx], sizeof(struct TAG));
      if(!tagList->list.array[idx])
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildTagConfig");
         return RFAILED;
      }
   }

   idx = 0;
   tagList->list.array[idx]->tag_Id = TAG_ID;
   tagList->list.array[idx]->timeAlignmentTimer = TIME_ALIGNMENT_TMR;

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
uint8_t BuildPhrConfig(struct MAC_CellGroupConfig__phr_Config *phrConfig)
{

   phrConfig->present = MAC_CellGroupConfig__phr_Config_PR_setup;
   phrConfig->choice.setup = NULLP;
   CU_ALLOC(phrConfig->choice.setup, sizeof(struct PHR_Config));
   if(!phrConfig->choice.setup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildPhrConfig");
      return RFAILED;
   }

   phrConfig->choice.setup->phr_PeriodicTimer        = PHR_PERIODIC_TMR;
   phrConfig->choice.setup->phr_ProhibitTimer        = PHR_PROHIBHIT_TMR;
   phrConfig->choice.setup->phr_Tx_PowerFactorChange = PHR_PWR_FACTOR_CHANGE;
   phrConfig->choice.setup->multiplePHR              = false;
   phrConfig->choice.setup->dummy                    = false;
   phrConfig->choice.setup->phr_Type2OtherCell       = false;
   phrConfig->choice.setup->phr_ModeOtherCG          = PHR_MODE_OTHER_CG;

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
uint8_t BuildBsrConfig(struct BSR_Config *bsrConfig)
{
   bsrConfig->periodicBSR_Timer = PERIODIC_BSR_TMR;
   bsrConfig->retxBSR_Timer     = RETX_BSR_TMR;
   bsrConfig->logicalChannelSR_DelayTimer = NULLP;

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
uint8_t BuildSchedulingReqConfig(struct SchedulingRequestConfig *schedulingRequestConfig)
{
   struct SchedulingRequestConfig__schedulingRequestToAddModList *schReqList;
   uint8_t                     idx, elementCnt;

   schedulingRequestConfig->schedulingRequestToAddModList = NULLP;
   CU_ALLOC(schedulingRequestConfig->schedulingRequestToAddModList,
	 sizeof(struct SchedulingRequestConfig__schedulingRequestToAddModList));
   if(!schedulingRequestConfig->schedulingRequestToAddModList)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSchedulingReqConfig");
      return RFAILED;
   }

   elementCnt = 1; //ODU_VALUE_ONE;
   schReqList = schedulingRequestConfig->schedulingRequestToAddModList;
   schReqList->list.count = elementCnt;
   schReqList->list.size  = elementCnt * sizeof(struct SchedulingRequestToAddMod *);

   schReqList->list.array = NULLP;
   CU_ALLOC(schReqList->list.array, schReqList->list.size);
   if(!schReqList->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSchedulingReqConfig");
      return RFAILED;
   }

   for(idx=0;idx<schReqList->list.count; idx++)
   {
      schReqList->list.array[idx] = NULLP;
      CU_ALLOC(schReqList->list.array[idx], sizeof(struct SchedulingRequestToAddMod));
      if(!schReqList->list.array[idx])
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSchedulingReqConfig");
         return RFAILED;
      }
   }

   idx = 0;
   schReqList->list.array[idx]->schedulingRequestId = SCH_REQ_ID;

   schReqList->list.array[idx]->sr_ProhibitTimer = NULLP;
   CU_ALLOC(schReqList->list.array[idx]->sr_ProhibitTimer, sizeof(long));
   if(!schReqList->list.array[idx]->sr_ProhibitTimer)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildSchedulingReqConfig");
      return RFAILED;
   }
   *(schReqList->list.array[idx]->sr_ProhibitTimer) = SR_PROHIBIT_TMR;
   schReqList->list.array[idx]->sr_TransMax = SR_TRANS_MAX;
   schedulingRequestConfig->schedulingRequestToReleaseList = NULLP;

   return ROK;
}
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
uint8_t BuildMacCellGrpCfg(MAC_CellGroupConfig_t *macCellGrpCfg)
{
   macCellGrpCfg->drx_ConfigRrc = NULLP;
   macCellGrpCfg->schedulingRequestConfig = NULLP;
   CU_ALLOC(macCellGrpCfg->schedulingRequestConfig, sizeof(struct SchedulingRequestConfig));
   if(!macCellGrpCfg->schedulingRequestConfig)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildMacCellGrpCfg");
      return RFAILED;
   }

   if(BuildSchedulingReqConfig(macCellGrpCfg->schedulingRequestConfig) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : BuildSchedulingReqConfig failed");
      return RFAILED;
   }

   macCellGrpCfg->bsr_Config = NULLP;
   CU_ALLOC(macCellGrpCfg->bsr_Config, sizeof(struct BSR_Config));
   if(!macCellGrpCfg->bsr_Config)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildMacCellGrpCfg");
      return RFAILED;
   }

   if(BuildBsrConfig(macCellGrpCfg->bsr_Config) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : BuildBsrConfig failed");
      return RFAILED;
   }

   macCellGrpCfg->tag_Config = NULLP;
   CU_ALLOC(macCellGrpCfg->tag_Config, sizeof(struct TAG_Config));
   if(!macCellGrpCfg->tag_Config)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildMacCellGrpCfg");
      return RFAILED;
   }

   if(BuildTagConfig(macCellGrpCfg->tag_Config) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : BuildTagConfig failed");
      return RFAILED;
   }

   macCellGrpCfg->phr_Config = NULLP;
   CU_ALLOC(macCellGrpCfg->phr_Config, sizeof(struct MAC_CellGroupConfig__phr_Config));
   if(!macCellGrpCfg->phr_Config)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildMacCellGrpCfg");
      return RFAILED;
   }

   if(BuildPhrConfig(macCellGrpCfg->phr_Config) != ROK)
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
			   CU_FREE(searchSpc->searchSpaceType->choice.ue_Specific,\
				 sizeof(struct SearchSpace__searchSpaceType__ue_Specific));
			   CU_FREE(searchSpc->searchSpaceType, sizeof(struct
				    SearchSpace__searchSpaceType));
			}
			CU_FREE(searchSpc->nrofCandidates,
			      sizeof(struct SearchSpace__nrofCandidates));
		     }
		     CU_FREE(searchSpc->monitoringSymbolsWithinSlot->buf, \
			   searchSpc->monitoringSymbolsWithinSlot->size);
		  }
		  CU_FREE(searchSpc->monitoringSymbolsWithinSlot,
			sizeof(BIT_STRING_t));
	       }
	       CU_FREE(searchSpc->monitoringSlotPeriodicityAndOffset, \
		     sizeof(struct SearchSpace__monitoringSlotPeriodicityAndOffset));
	    }
	    CU_FREE(searchSpc->controlResourceSetId,
		  sizeof(ControlResourceSetId_t));
	 }
      }
      for(idx1 = 0; idx1 < searchSpcList->list.count; idx1++)
      {
	 CU_FREE(searchSpcList->list.array[idx1],
	       sizeof(struct SearchSpace));
      }
      CU_FREE(searchSpcList->list.array,searchSpcList->list.size);
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
 4221 * ****************************************************************/
void FreePdschTimeDomAllocList( struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAllocList)
{
   uint8_t idx1=0;

   if(timeDomAllocList->choice.setup)
   {
      if(timeDomAllocList->choice.setup->list.array)
      {
	 for(idx1 = 0; idx1 <timeDomAllocList->choice.setup->list.count ; idx1++)
	 {
	    CU_FREE(timeDomAllocList->choice.setup->list.array[idx1],
		  sizeof(struct PDSCH_TimeDomainResourceAllocation));
	 }
	 CU_FREE(timeDomAllocList->choice.setup->list.array, \
	       timeDomAllocList->choice.setup->list.size);
      }
      CU_FREE(timeDomAllocList->choice.setup,\
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
 ***********************************************************************/
void FreePuschTimeDomAllocList(PUSCH_Config_t *puschCfg)
{
   uint8_t idx1=0;
   uint8_t idx2=0;
   struct PUSCH_Config__pusch_TimeDomainAllocationList *timeDomAllocList_t=NULLP;

   if(puschCfg->pusch_TimeDomainAllocationList)
   {
      timeDomAllocList_t=puschCfg->pusch_TimeDomainAllocationList;
      if(timeDomAllocList_t->choice.setup)
      {
	 if(timeDomAllocList_t->choice.setup->list.array)
	 {
	    CU_FREE(timeDomAllocList_t->choice.setup->list.array[idx2]->k2, sizeof(long));
	    for(idx1 = 0; idx1<timeDomAllocList_t->choice.setup->list.count; idx1++)
	    {
	       CU_FREE(timeDomAllocList_t->choice.setup->list.array[idx1],\
		     sizeof(PUSCH_TimeDomainResourceAllocation_t));
	    }
	    CU_FREE(timeDomAllocList_t->choice.setup->list.array, \
		  timeDomAllocList_t->choice.setup->list.size);
	 }
	 CU_FREE(timeDomAllocList_t->choice.setup, \
	       sizeof(struct PUSCH_TimeDomainResourceAllocationList));
      }
      CU_FREE(puschCfg->transformPrecoder, sizeof(long));
      CU_FREE(puschCfg->pusch_TimeDomainAllocationList, \
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
   uint8_t arrIdx, rsrcIdx, rsrcSetIdx;
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
                        CU_FREE(rsrcSet->resourceList.list.array[rsrcIdx], sizeof(PUCCH_ResourceId_t));
                     }
                     CU_FREE(rsrcSet->resourceList.list.array, rsrcSet->resourceList.list.size);
                  }
                  CU_FREE(pucchCfg->resourceSetToAddModList->list.array[rsrcSetIdx], sizeof(PUCCH_ResourceSet_t));
               }
               CU_FREE(pucchCfg->resourceSetToAddModList->list.array, pucchCfg->resourceSetToAddModList->list.size);
            }
            CU_FREE(pucchCfg->resourceSetToAddModList, sizeof(struct PUCCH_Config__resourceSetToAddModList));
         }

         //Free resource list
         if(pucchCfg->resourceToAddModList)
         {
            if(pucchCfg->resourceToAddModList->list.array)
            {
               for(rsrcIdx=0; rsrcIdx < pucchCfg->resourceToAddModList->list.count; rsrcIdx++)
               {
                  rsrc = pucchCfg->resourceToAddModList->list.array[rsrcIdx];
                  CU_FREE(rsrc->format.choice.format1, sizeof(PUCCH_format1_t));
                  CU_FREE(pucchCfg->resourceToAddModList->list.array[rsrcIdx], sizeof(PUCCH_Resource_t));
               }
               CU_FREE(pucchCfg->resourceToAddModList->list.array, pucchCfg->resourceToAddModList->list.size);
            }
            CU_FREE(pucchCfg->resourceToAddModList, sizeof(struct PUCCH_Config__resourceToAddModList));
         }
         
         //PUCCH Format 1
         if(pucchCfg->format1)
         {
            if(pucchCfg->format1->choice.setup)
            {
               CU_FREE(pucchCfg->format1->choice.setup->nrofSlots, sizeof(long));
               CU_FREE(pucchCfg->format1->choice.setup, sizeof(PUCCH_FormatConfig_t));
            }
            CU_FREE(pucchCfg->format1, sizeof(struct PUCCH_Config__format1));
         }

         //DL DATA TO UL ACK
         if(pucchCfg->dl_DataToUL_ACK)
         {
            if(pucchCfg->dl_DataToUL_ACK->list.array)
            {
               for(arrIdx = 0; arrIdx <  pucchCfg->dl_DataToUL_ACK->list.count; arrIdx++)
               {
                  CU_FREE(pucchCfg->dl_DataToUL_ACK->list.array[arrIdx], sizeof(long));
               }
               CU_FREE(pucchCfg->dl_DataToUL_ACK->list.array, pucchCfg->dl_DataToUL_ACK->list.size);
            }
            CU_FREE(pucchCfg->dl_DataToUL_ACK, sizeof(struct PUCCH_Config__dl_DataToUL_ACK));
         }

         CU_FREE(ulBwpPucchCfg->choice.setup, sizeof(PUCCH_Config_t));
      }
      CU_FREE(ulBwpPucchCfg, sizeof(struct BWP_UplinkDedicated__pucch_Config));
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
                        CU_FREE(dmrsUlCfg->choice.setup->transformPrecodingDisabled->scramblingID0,\
                              sizeof(long));
                        CU_FREE(dmrsUlCfg->choice.setup->transformPrecodingDisabled,
                              sizeof(struct DMRS_UplinkConfig__transformPrecodingDisabled));
                     }
                     CU_FREE(dmrsUlCfg->choice.setup->dmrs_AdditionalPosition,
                           sizeof(long));
                  }
                  CU_FREE(dmrsUlCfg->choice.setup,sizeof(DMRS_UplinkConfig_t));
               }
               CU_FREE(puschCfg->dmrs_UplinkForPUSCH_MappingTypeA, \
                     sizeof(struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA));
            }
            CU_FREE(puschCfg->dataScramblingIdentityPUSCH, sizeof(long));
         }
         CU_FREE(ulBwp->pusch_Config->choice.setup, sizeof(PUSCH_Config_t));
      }
      CU_FREE(ulBwp->pusch_Config, sizeof(struct BWP_UplinkDedicated__pusch_Config));

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
                           CU_FREE(rsrcIdList->list.array[rsrcIdx], sizeof(SRS_ResourceId_t));
                        }
                        CU_FREE(rsrcIdList->list.array, rsrcIdList->list.size);
                     }
                     CU_FREE(rsrcSetList->list.array[rSetIdx]->srs_ResourceIdList,\
                           sizeof(struct SRS_ResourceSet__srs_ResourceIdList));
                  }

                  /* Free resource type info for this SRS resource set */
                  CU_FREE(rsrcSetList->list.array[rSetIdx]->resourceType.choice.aperiodic, \
                        sizeof(struct SRS_ResourceSet__resourceType__aperiodic));

                  /* Free memory for each resource set */
                  for(rSetIdx = 0; rSetIdx < rsrcSetList->list.count; rSetIdx++)
                  {
                     CU_FREE(rsrcSetList->list.array[rSetIdx], sizeof(SRS_ResourceSet_t));
                  }
                  CU_FREE(rsrcSetList->list.array, rsrcSetList->list.size); 
               }
               CU_FREE(srsCfg->srs_ResourceSetToAddModList, \
                     sizeof(struct SRS_Config__srs_ResourceSetToAddModList));
            }

            /* Free resource to add/modd list */
            if(srsCfg->srs_ResourceToAddModList)
            {
               resourceList = srsCfg->srs_ResourceToAddModList;
               if(resourceList->list.array)
               {
                  rsrcIdx = 0;
                  CU_FREE(resourceList->list.array[rsrcIdx]->transmissionComb.choice.n2,\
                        sizeof(struct SRS_Resource__transmissionComb__n2));
                  CU_FREE(resourceList->list.array[rsrcIdx]->resourceType.choice.aperiodic,\
                        sizeof(struct SRS_Resource__resourceType__aperiodic));

                  for(rsrcIdx = 0; rsrcIdx < resourceList->list.count; rsrcIdx++)
                  {
                     CU_FREE(resourceList->list.array[rsrcIdx], sizeof(SRS_Resource_t));
                  }
                  CU_FREE(resourceList->list.array, resourceList->list.size);
               }
               CU_FREE(srsCfg->srs_ResourceToAddModList, \
                     sizeof(struct SRS_Config__srs_ResourceToAddModList));
            }

            CU_FREE(ulBwp->srs_Config->choice.setup, sizeof(SRS_Config_t));
         }
         CU_FREE(ulBwp->srs_Config, sizeof(struct BWP_UplinkDedicated__srs_Config));
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
		  CU_FREE(puschCfg->choice.setup->ext1->\
			processingType2Enabled,sizeof(BOOLEAN_t));
		  CU_FREE(puschCfg->choice.setup->ext1->\
			maxMIMO_Layers,sizeof(long));
		  CU_FREE(puschCfg->choice.setup->ext1, \
			sizeof(struct PUSCH_ServingCellConfig__ext1));
	       }
	       CU_FREE(puschCfg->choice.setup, sizeof(struct PUSCH_ServingCellConfig));
	    }
	    CU_FREE(ulCfg->pusch_ServingCellConfig, sizeof(struct UplinkConfig__pusch_ServingCellConfig));
	 }
	 CU_FREE(ulCfg->firstActiveUplinkBWP_Id, sizeof(BWP_Id_t));
      }
      FreeInitialUlBWP(ulBwp);
      CU_FREE(ulCfg->initialUplinkBWP, sizeof(BWP_UplinkDedicated_t));
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
	       CU_FREE(prbBndlType->choice.staticBundling,\
		     sizeof(struct PDSCH_Config__prb_BundlingType__staticBundling));
	       CU_FREE(pdschCfg->maxNrofCodeWordsScheduledByDCI, sizeof(long));
	    }
	    FreePdschTimeDomAllocList(timeDomAllocList);
	    CU_FREE(pdschCfg->pdsch_TimeDomainAllocationList, \
		  sizeof(struct PDSCH_Config__pdsch_TimeDomainAllocationList));
	 }
	 dmrsDlCfg=pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA;
	 if(dmrsDlCfg->choice.setup)
	 {
	    CU_FREE(dmrsDlCfg->choice.setup->dmrs_AdditionalPosition,
		  sizeof(long));
	    CU_FREE(dmrsDlCfg->choice.setup, sizeof(struct DMRS_DownlinkConfig));
	 }
	 CU_FREE(pdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA, \
	       sizeof(struct PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA));
      }
      CU_FREE(dlBwp->pdsch_Config->choice.setup, sizeof(struct PDSCH_Config));
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
			CU_FREE(pdcchCfg->searchSpacesToAddModList, \
			      sizeof(struct PDCCH_Config__searchSpacesToAddModList));
		     }
		     CU_FREE(controlRSet->pdcch_DMRS_ScramblingID, sizeof(long));
		  }
		  CU_FREE(controlRSet->frequencyDomainResources.buf, \
			controlRSet->frequencyDomainResources.size);
	       }
	    }
	    for(idx1 = 0; idx1 <controlRSetList->list.count; idx1++)
	    {
	       CU_FREE(controlRSetList->list.array[idx1], sizeof(struct ControlResourceSet));
	    }
	    CU_FREE(controlRSetList->list.array, controlRSetList->list.size);
	 }
	 CU_FREE(pdcchCfg->controlResourceSetToAddModList, \
	       sizeof(struct PDCCH_Config__controlResourceSetToAddModList));
      }
      CU_FREE(dlBwp->pdcch_Config->choice.setup, sizeof(struct PDCCH_Config));
   }
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
 * @params[in] RLC_Config *rlcConfig
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildRlcConfig(RlcLcCfg rlcLcCfgDb, struct RLC_Config *rlcConfig)
{
   rlcConfig->present = rlcLcCfgDb.rlcMode;

   switch(rlcConfig->present)
   {
      case RLC_Config_PR_am:
         {
            rlcConfig->choice.am = NULLP;
            CU_ALLOC(rlcConfig->choice.am, sizeof(struct RLC_Config__am));
            if(!rlcConfig->choice.am)
            {
               DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcConfig");
               return RFAILED;
            }

            /* UL */
            rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength = NULLP;
            CU_ALLOC(rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength, sizeof(SN_FieldLengthAM_t));
            if(!rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength)
            {
               DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcConfig");
               return RFAILED;
            }
            *(rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength) = rlcLcCfgDb.u.amCfg.ulAmCfg.snLenUl;
            rlcConfig->choice.am->ul_AM_RLC.t_PollRetransmit  = rlcLcCfgDb.u.amCfg.ulAmCfg.pollRetxTmr;
            rlcConfig->choice.am->ul_AM_RLC.pollPDU           = rlcLcCfgDb.u.amCfg.ulAmCfg.pollPdu;
            rlcConfig->choice.am->ul_AM_RLC.pollByte          = rlcLcCfgDb.u.amCfg.ulAmCfg.pollByte;
            rlcConfig->choice.am->ul_AM_RLC.maxRetxThreshold  = rlcLcCfgDb.u.amCfg.ulAmCfg.maxRetxTh;

            /* DL */
            rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength = NULLP;
            CU_ALLOC(rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength, sizeof(SN_FieldLengthAM_t)); 
            if(!rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength)
            {
               DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcConfig");
               return RFAILED;
            }
            *(rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength) = rlcLcCfgDb.u.amCfg.dlAmCfg.snLenDl;
            rlcConfig->choice.am->dl_AM_RLC.t_Reassembly      = rlcLcCfgDb.u.amCfg.dlAmCfg.reAssemTmr;
            rlcConfig->choice.am->dl_AM_RLC.t_StatusProhibit  = rlcLcCfgDb.u.amCfg.dlAmCfg.statProhTmr;

            break;
         }

      case RLC_Config_PR_um_Bi_Directional:
         {
            rlcConfig->choice.um_Bi_Directional = NULLP;
            CU_ALLOC(rlcConfig->choice.um_Bi_Directional, sizeof(struct RLC_Config__um_Bi_Directional));
            if(!rlcConfig->choice.um_Bi_Directional)
            {
               DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcConfig");
               return RFAILED;
            }

            /* UL */
            rlcConfig->choice.um_Bi_Directional->ul_UM_RLC.sn_FieldLength = NULLP;
            CU_ALLOC(rlcConfig->choice.um_Bi_Directional->ul_UM_RLC.sn_FieldLength, sizeof(SN_FieldLengthUM_t));
            if(!rlcConfig->choice.um_Bi_Directional->ul_UM_RLC.sn_FieldLength)
            {
               DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcConfig");
               return RFAILED;
            }
            *(rlcConfig->choice.um_Bi_Directional->ul_UM_RLC.sn_FieldLength) = rlcLcCfgDb.u.umBiDirCfg.ulUmCfg.snLenUlUm;

            /* DL */
            rlcConfig->choice.um_Bi_Directional->dl_UM_RLC.sn_FieldLength = NULLP;
            CU_ALLOC(rlcConfig->choice.um_Bi_Directional->dl_UM_RLC.sn_FieldLength, sizeof(SN_FieldLengthUM_t));
            if(!rlcConfig->choice.um_Bi_Directional->dl_UM_RLC.sn_FieldLength)
            {
               DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcConfig");
               return RFAILED;
            }
            *(rlcConfig->choice.um_Bi_Directional->dl_UM_RLC.sn_FieldLength) = rlcLcCfgDb.u.umBiDirCfg.dlUmCfg.snLenDlUm;
            rlcConfig->choice.um_Bi_Directional->dl_UM_RLC.t_Reassembly = rlcLcCfgDb.u.umBiDirCfg.dlUmCfg.reAssemTmr;
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
uint8_t BuildMacLCConfig(MacLcCfg macLcCfgDb, struct LogicalChannelConfig *macLcConfig)
{

   macLcConfig->ul_SpecificParameters = NULLP;
   CU_ALLOC(macLcConfig->ul_SpecificParameters, sizeof(struct LogicalChannelConfig__ul_SpecificParameters));
   if(!macLcConfig->ul_SpecificParameters)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildMacLCConfig");
      return RFAILED;
   }

   macLcConfig->ul_SpecificParameters->priority = macLcCfgDb.priority;
   macLcConfig->ul_SpecificParameters->prioritisedBitRate =	macLcCfgDb.pbr;
   macLcConfig->ul_SpecificParameters->bucketSizeDuration =	macLcCfgDb.bsd;
   macLcConfig->ul_SpecificParameters->allowedServingCells = NULLP;
   macLcConfig->ul_SpecificParameters->allowedSCS_List = NULLP;
   macLcConfig->ul_SpecificParameters->maxPUSCH_Duration = NULLP;
   macLcConfig->ul_SpecificParameters->configuredGrantType1Allowed = NULLP;

   macLcConfig->ul_SpecificParameters->logicalChannelGroup = NULLP;
   CU_ALLOC(macLcConfig->ul_SpecificParameters->logicalChannelGroup,	sizeof(long));
   if(!macLcConfig->ul_SpecificParameters->logicalChannelGroup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildMacLCConfig");
      return RFAILED;
   }
   *(macLcConfig->ul_SpecificParameters->logicalChannelGroup) = macLcCfgDb.lcGroup;

   macLcConfig->ul_SpecificParameters->schedulingRequestID = NULLP;
   CU_ALLOC(macLcConfig->ul_SpecificParameters->schedulingRequestID,	sizeof(SchedulingRequestId_t));
   if(!macLcConfig->ul_SpecificParameters->schedulingRequestID)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildMacLCConfig");
      return RFAILED;
   }
   *(macLcConfig->ul_SpecificParameters->schedulingRequestID) = macLcCfgDb.schReqId;

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
uint8_t BuildRlcBearerToAddModList(CuUeCb *ueCb, struct CellGroupConfigRrc__rlc_BearerToAddModList *rlcBearerList, bool updateAllRbCfg)
{
   uint8_t idx, srbIdx = 0, drbIdx = 0, elementCnt=0;

   if(updateAllRbCfg)
      elementCnt = ueCb->numSrb + ueCb->numDrb;
   else
   {
      for(srbIdx = 0; srbIdx< ueCb->numSrb; srbIdx++)
      {
         if(ueCb->srbList[srbIdx].cfgSentToUe == false)
            elementCnt++;
      }

      for(drbIdx = 0; drbIdx< ueCb->numDrb; drbIdx++)
      {
         if(ueCb->drbList[drbIdx].cfgSentToUe == false)
            elementCnt++;
      }
   }

   if(!elementCnt)
   {
      DU_LOG("INFO  --> F1AP : No  RLC Bearer available to add or modify");
      return ROK;
   }
   CU_ALLOC(rlcBearerList, sizeof(struct CellGroupConfigRrc__rlc_BearerToAddModList));
   if(!rlcBearerList)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in CellGrpConfig");
      return RFAILED;
   }
   rlcBearerList->list.count = elementCnt;
   rlcBearerList->list.size  = elementCnt * sizeof(struct RLC_BearerConfig *);

   rlcBearerList->list.array = NULLP;
   CU_ALLOC(rlcBearerList->list.array, rlcBearerList->list.size);
   if(!rlcBearerList->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcBearerToAddModList");
      return RFAILED;
   }

   for(idx=0; idx<rlcBearerList->list.count; idx++)
   {
      rlcBearerList->list.array[idx] = NULLP;
      CU_ALLOC(rlcBearerList->list.array[idx], sizeof(struct RLC_BearerConfig));
      if(!rlcBearerList->list.array[idx])
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcBearerToAddModList");
         return RFAILED;
      }
   }

   idx = 0;

   for(srbIdx=0; srbIdx < ueCb->numSrb; srbIdx++)
   {
      if(!updateAllRbCfg && ueCb->srbList[srbIdx].cfgSentToUe)
         continue;

      rlcBearerList->list.array[idx]->logicalChannelIdentity = ueCb->srbList[srbIdx].lcId;

      CU_ALLOC(rlcBearerList->list.array[idx]->servedRadioBearer, sizeof(struct RLC_BearerConfig__servedRadioBearer));
      if(!rlcBearerList->list.array[idx]->servedRadioBearer)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcBearerToAddModList");
         return RFAILED;
      }

      rlcBearerList->list.array[idx]->servedRadioBearer->present = RLC_BearerConfig__servedRadioBearer_PR_srb_Identity;
      rlcBearerList->list.array[idx]->servedRadioBearer->choice.srb_Identity = ueCb->srbList[srbIdx].srbId;

      rlcBearerList->list.array[idx]->reestablishRLC = NULLP;
      rlcBearerList->list.array[idx]->rlc_Config = NULLP;
      CU_ALLOC(rlcBearerList->list.array[idx]->rlc_Config, sizeof(struct RLC_Config));
      if(!rlcBearerList->list.array[idx]->rlc_Config)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcBearerToAddModList");
         return RFAILED;
      }

      if(BuildRlcConfig(ueCb->srbList[srbIdx].rlcLcCfg, rlcBearerList->list.array[idx]->rlc_Config) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildRlcConfig failed");
         return RFAILED;
      }

      rlcBearerList->list.array[idx]->mac_LogicalChannelConfig = NULLP;
      CU_ALLOC(rlcBearerList->list.array[idx]->mac_LogicalChannelConfig, sizeof(struct LogicalChannelConfig));
      if(!rlcBearerList->list.array[idx]->mac_LogicalChannelConfig)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcBearerToAddModList");
         return RFAILED;
      }

      if(BuildMacLCConfig(ueCb->srbList[srbIdx].macLcCfg, rlcBearerList->list.array[idx]->mac_LogicalChannelConfig) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildMacLCConfig failed");
         return RFAILED;
      }
      idx++;
   }

   for(drbIdx=0; drbIdx < ueCb->numDrb; drbIdx++)
   {
      if(!updateAllRbCfg && ueCb->drbList[drbIdx].cfgSentToUe)
         continue;

      rlcBearerList->list.array[idx]->logicalChannelIdentity = ueCb->drbList[drbIdx].lcId;

      CU_ALLOC(rlcBearerList->list.array[idx]->servedRadioBearer, sizeof(struct RLC_BearerConfig__servedRadioBearer));
      if(!rlcBearerList->list.array[idx]->servedRadioBearer)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcBearerToAddModList");
         return RFAILED;
      }

      rlcBearerList->list.array[idx]->servedRadioBearer->present = RLC_BearerConfig__servedRadioBearer_PR_drb_Identity;
      rlcBearerList->list.array[idx]->servedRadioBearer->choice.drb_Identity = ueCb->drbList[drbIdx].drbId;

      rlcBearerList->list.array[idx]->reestablishRLC = NULLP;
      rlcBearerList->list.array[idx]->rlc_Config = NULLP;
      CU_ALLOC(rlcBearerList->list.array[idx]->rlc_Config, sizeof(struct RLC_Config));
      if(!rlcBearerList->list.array[idx]->rlc_Config)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcBearerToAddModList");
         return RFAILED;
      }

      if(BuildRlcConfig(ueCb->drbList[drbIdx].rlcLcCfg, rlcBearerList->list.array[idx]->rlc_Config) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildRlcConfig failed");
         return RFAILED;
      }

      rlcBearerList->list.array[idx]->mac_LogicalChannelConfig = NULLP;
      CU_ALLOC(rlcBearerList->list.array[idx]->mac_LogicalChannelConfig, sizeof(struct LogicalChannelConfig));
      if(!rlcBearerList->list.array[idx]->mac_LogicalChannelConfig)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildRlcBearerToAddModList");
         return RFAILED;
      }

      if(BuildMacLCConfig(ueCb->drbList[drbIdx].macLcCfg, rlcBearerList->list.array[idx]->mac_LogicalChannelConfig) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildMacLCConfig failed");
         return RFAILED;
      }
      idx++;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Free memory allocated for CellGroupConfig 
 *
 * @details
 *
 *    Function : FreeMemCellGrpCfg
 *
 *    Functionality: Deallocating memory of CellGroupConfig
 *
 * @params[in] pointer to CellGroupConfigRrc_t
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t FreeMemCellGrpCfg(CellGroupConfigRrc_t *cellGrpCfg)
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
		  if(rlcConfig->choice.am)
		  {
		     CU_FREE(rlcConfig->choice.am->ul_AM_RLC.sn_FieldLength, sizeof(SN_FieldLengthAM_t));
		     CU_FREE(rlcConfig->choice.am->dl_AM_RLC.sn_FieldLength, sizeof(SN_FieldLengthAM_t)); 
		     CU_FREE(rlcConfig->choice.am, sizeof(struct RLC_Config__am));
		  }	
		  CU_FREE(rlcConfig, sizeof(struct RLC_Config));
	       }
	       CU_FREE(rlcBearerList->list.array[idx]->servedRadioBearer, sizeof(struct RLC_BearerConfig__servedRadioBearer));
	       if(macLcConfig)
	       {
		  if(macLcConfig->ul_SpecificParameters)
		  {
		     CU_FREE(macLcConfig->ul_SpecificParameters->schedulingRequestID,	sizeof(SchedulingRequestId_t));
		     CU_FREE(macLcConfig->ul_SpecificParameters->logicalChannelGroup,	sizeof(long));
		     CU_FREE(macLcConfig->ul_SpecificParameters, sizeof(struct LogicalChannelConfig__ul_SpecificParameters));
		  }
		  CU_FREE(rlcBearerList->list.array[idx]->mac_LogicalChannelConfig, sizeof(struct LogicalChannelConfig));
	       }
	       CU_FREE(rlcBearerList->list.array[idx], sizeof(struct RLC_BearerConfig));
	    }	
	 }
	 CU_FREE(rlcBearerList->list.array, rlcBearerList->list.size);
      }
      CU_FREE(cellGrpCfg->rlc_BearerToAddModList, sizeof(struct CellGroupConfigRrc__rlc_BearerToAddModList));
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
	       for(idx=0;idx<schReqList->list.count; idx++)
	       {
		  if(schReqList->list.array[idx])
		  {
		     CU_FREE(schReqList->list.array[idx]->sr_ProhibitTimer, sizeof(long));
		     CU_FREE(schReqList->list.array[idx], sizeof(struct SchedulingRequestToAddMod));
		  }
	       }
	       CU_FREE(schReqList->list.array, schReqList->list.size);
	    }
	    CU_FREE(schedulingRequestConfig->schedulingRequestToAddModList,\
		  sizeof(struct SchedulingRequestConfig__schedulingRequestToAddModList));    }
	    CU_FREE(macCellGrpCfg->schedulingRequestConfig, sizeof(struct SchedulingRequestConfig));
      }
      if(macCellGrpCfg->bsr_Config)
      {
	 CU_FREE(macCellGrpCfg->bsr_Config, sizeof(struct BSR_Config));
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
		  CU_FREE(tagList->list.array[idx], sizeof(struct TAG));
	       }
	       CU_FREE(tagList->list.array, tagList->list.size);
	    }
	    CU_FREE(tagConfig->tag_ToAddModList, sizeof(struct TAG_Config__tag_ToAddModList));
	 }
	 CU_FREE(tagConfig, sizeof(struct TAG_Config));
      }

      phrConfig = macCellGrpCfg->phr_Config;
      if(phrConfig)
      {
	 CU_FREE(phrConfig->choice.setup, sizeof(struct PHR_Config));
	 CU_FREE(phrConfig, sizeof(struct MAC_CellGroupConfig__phr_Config));
      }

      CU_FREE(macCellGrpCfg, sizeof(MAC_CellGroupConfig_t));
   }

   phyCellGrpCfg = cellGrpCfg->physicalCellGroupConfig;
   if(phyCellGrpCfg)
   {
      CU_FREE(phyCellGrpCfg->p_NR_FR1, sizeof(long));
      CU_FREE(phyCellGrpCfg, sizeof(PhysicalCellGroupConfig_t));
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
			      pdschCfg= srvCellCfg->pdsch_ServingCellConfig;
			      if(pdschCfg->choice.setup)
			      {
				 CU_FREE(pdschCfg->choice.setup->nrofHARQ_ProcessesForPDSCH,sizeof(long));
				 CU_FREE(pdschCfg->choice.setup, sizeof( struct PDSCH_ServingCellConfig));
			      }
			      CU_FREE(srvCellCfg->pdsch_ServingCellConfig, sizeof(struct
				       ServingCellConfig__pdsch_ServingCellConfig));
			   }
			   FreeinitialUplinkBWP(srvCellCfg->uplinkConfig);
			   CU_FREE(srvCellCfg->uplinkConfig, sizeof(UplinkConfig_t));	
			}
			CU_FREE(srvCellCfg->defaultDownlinkBWP_Id, sizeof(long));
		     }
		     CU_FREE(srvCellCfg->firstActiveDownlinkBWP_Id, sizeof(long));
		  }
		  if(dlBwp->pdcch_Config)
		  {
		     if(dlBwp->pdsch_Config)
		     {
			FreeBWPDlDedPdschCfg(dlBwp);
			CU_FREE(dlBwp->pdsch_Config, sizeof(struct BWP_DownlinkDedicated__pdsch_Config));
		     }
		     FreeBWPDlDedPdcchCfg(dlBwp);
		     CU_FREE(dlBwp->pdcch_Config, sizeof(struct BWP_DownlinkDedicated__pdcch_Config));
		  }
		  CU_FREE(srvCellCfg->initialDownlinkBWP, sizeof(BWP_DownlinkDedicated_t));
	       }
	       CU_FREE(spCellCfg->spCellConfigDedicated, sizeof(ServingCellConfig_t));
	    }
	    CU_FREE(spCellCfg->rlmInSyncOutOfSyncThreshold, sizeof(long));
	 }
	 CU_FREE(spCellCfg->servCellIndex, sizeof(long));
      }
      CU_FREE(spCellCfg,sizeof(SpCellConfig_t));
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills CellGroupConfig 
 *
 * @details
 *
 *    Function : fillCellGrpCfg
 *
 *    Functionality: Fills CellGroupConfig
 *
 * @params[in] pointer to CellGroupConfigRrc_t
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t fillCellGrpCfg(CuUeCb *ueCb, OCTET_STRING_t *cellGrp, bool updateAllRbCfg)
{
   uint8_t               ret = RFAILED;
   CellGroupConfigRrc_t  cellGrpCfg;
   asn_enc_rval_t        encRetVal;

   while(true)
   {
      cellGrpCfg.cellGroupId = CELL_GRP_ID;

      cellGrpCfg.rlc_BearerToAddModList = NULLP;
      
      if(BuildRlcBearerToAddModList(ueCb, cellGrpCfg.rlc_BearerToAddModList, updateAllRbCfg) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : fillCellGrpCfg failed");
         break;
      }

      cellGrpCfg.rlc_BearerToReleaseList = NULLP;
      cellGrpCfg.mac_CellGroupConfig = NULLP;
      CU_ALLOC(cellGrpCfg.mac_CellGroupConfig, sizeof(MAC_CellGroupConfig_t));
      if(!cellGrpCfg.mac_CellGroupConfig)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in fillCellGrpCfg");
         break;
      }
      if(BuildMacCellGrpCfg(cellGrpCfg.mac_CellGroupConfig) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildMacCellGrpCfg failed");
         break;
      }

      cellGrpCfg.physicalCellGroupConfig = NULLP;
      CU_ALLOC(cellGrpCfg.physicalCellGroupConfig, sizeof(PhysicalCellGroupConfig_t));
      if(!cellGrpCfg.physicalCellGroupConfig)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildDuToCuRrcContainer");
         break;
      }
      if(BuildPhyCellGrpCfg(cellGrpCfg.physicalCellGroupConfig) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildPhyCellGrpCfg failed");
         break;
      }

      cellGrpCfg.spCellConfig = NULLP;
      CU_ALLOC(cellGrpCfg.spCellConfig, sizeof(SpCellConfig_t));
      if(!cellGrpCfg.spCellConfig)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failure in BuildDuToCuRrcContainer");
         break;
      }
      if(BuildSpCellCfg(cellGrpCfg.spCellConfig) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildSpCellCfg failed");
         break;
      }

      cellGrpCfg.sCellToAddModList = NULLP;
      cellGrpCfg.sCellToReleaseList = NULLP;
      cellGrpCfg.ext1 = NULLP;

      /* encode cellGrpCfg into duToCuRrcContainer */
      xer_fprint(stdout, &asn_DEF_CellGroupConfigRrc, &cellGrpCfg);
      cmMemset((uint8_t *)encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = uper_encode(&asn_DEF_CellGroupConfigRrc, 0, &cellGrpCfg, PrepFinalEncBuf, encBuf);
      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG( "\nERROR  -->  F1AP : Could not encode DuToCuRrcContainer (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  F1AP : Created APER encodedbuffer for DuToCuRrcContainer\n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         }
      }

      cellGrp->size = encBufSize;
      CU_ALLOC(cellGrp->buf, cellGrp->size);
      if(!cellGrp->buf)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDuToCuRrcContainer");
         break;
      }
      memcpy(cellGrp->buf, encBuf, cellGrp->size);
      ret = ROK;
      break;
   }
   FreeMemCellGrpCfg(&cellGrpCfg);
   return ret;
}

/*******************************************************************
 *
 * @brief Free UE Capability RAT container
 *
 * @details
 *
 *    Function : freeUeCapRatCont
 *
 *    Functionality:
 *       Free UE Capability RAT conatiner
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void freeUeCapRatCont(UE_NR_Capability_t *ueNrCap)
{
   uint8_t idx;
   FeatureSets_t *featureSets;

   if(ueNrCap->rf_Parameters.supportedBandListNR.list.array)
   {
      for(idx = 0; idx < ueNrCap->rf_Parameters.supportedBandListNR.list.count; idx++)
      {
         if(ueNrCap->rf_Parameters.supportedBandListNR.list.array[idx])
            CU_FREE(ueNrCap->rf_Parameters.supportedBandListNR.list.array[idx], sizeof(struct BandNR));
      }
      CU_FREE(ueNrCap->rf_Parameters.supportedBandListNR.list.array, ueNrCap->rf_Parameters.supportedBandListNR.list.size);
   }

   if(ueNrCap->featureSets)
   {
      featureSets = ueNrCap->featureSets;
      if(featureSets->featureSetsDownlinkPerCC)
      {
         if(featureSets->featureSetsDownlinkPerCC->list.array)
         {
            for(idx = 0; idx < featureSets->featureSetsDownlinkPerCC->list.count; idx++)
            {
                  if(featureSets->featureSetsDownlinkPerCC->list.array[idx]->supportedModulationOrderDL)
                     CU_FREE(featureSets->featureSetsDownlinkPerCC->list.array[idx]->supportedModulationOrderDL, \
                        sizeof(ModulationOrder_t));
                  CU_FREE(featureSets->featureSetsDownlinkPerCC->list.array[idx], sizeof(struct FeatureSetDownlinkPerCC));
            }
            CU_FREE(featureSets->featureSetsDownlinkPerCC->list.array, featureSets->featureSetsDownlinkPerCC->list.size);
         }
         CU_FREE(featureSets->featureSetsDownlinkPerCC, sizeof(struct FeatureSets__featureSetsDownlinkPerCC));
      }

      if(featureSets->featureSetsUplinkPerCC)
      {
         if(featureSets->featureSetsUplinkPerCC->list.array)
         {
            for(idx = 0; idx < featureSets->featureSetsUplinkPerCC->list.count; idx++)
            {
               if(featureSets->featureSetsUplinkPerCC->list.array[idx])
               {
                  if(featureSets->featureSetsUplinkPerCC->list.array[idx]->supportedModulationOrderUL)
                     CU_FREE(featureSets->featureSetsUplinkPerCC->list.array[idx]->supportedModulationOrderUL,\
                         sizeof(ModulationOrder_t));
                  CU_FREE(featureSets->featureSetsUplinkPerCC->list.array[idx], sizeof(struct FeatureSetUplinkPerCC));
               }
            }
            CU_FREE(featureSets->featureSetsUplinkPerCC->list.array,  featureSets->featureSetsUplinkPerCC->list.size);
         }
         CU_FREE(featureSets->featureSetsUplinkPerCC, sizeof(struct FeatureSets__featureSetsUplinkPerCC));
      }
      CU_FREE(ueNrCap->featureSets, sizeof(struct FeatureSets));
   }
}

/*******************************************************************
 *
 * @brief Free UE capability RAT container list
 *
 * @details
 *
 *    Function : freeUeCapRatContList
 *
 *    Functionality: Free UE capability RAT container list
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void freeUeCapRatContList(UE_CapabilityRAT_ContainerListRRC_t *ueCapablityList)
{
   uint8_t idx;
   if(ueCapablityList->list.array)
   {
      for(idx = 0; idx < ueCapablityList->list.count; idx++)
      {
         if(ueCapablityList->list.array[idx])
            CU_FREE(ueCapablityList->list.array[idx], sizeof(UE_CapabilityRAT_Container_t));
      }
      CU_FREE(ueCapablityList->list.array, ueCapablityList->list.size);
   }
}

/*******************************************************************
 *
 * @brief Free Handover preparation information
 *
 * @details
 *
 *    Function : freeHOPreparationInfo
 *
 *    Functionality: Free Handover preparation information
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void freeHOPreparationInfo(HandoverPreparationInformationRrc_t *hoPrep)
{
   HandoverPreparationInformationRrc_IEs_t *hoPrepInfoIe;

   if(hoPrep->criticalExtensions.choice.c1)
   {
      if(hoPrep->criticalExtensions.choice.c1->choice.handoverPreparationInformation)
      {
         hoPrepInfoIe = hoPrep->criticalExtensions.choice.c1->choice.handoverPreparationInformation;
         freeUeCapRatContList(&hoPrepInfoIe->ue_CapabilityRAT_List);
         CU_FREE(hoPrep->criticalExtensions.choice.c1->choice.handoverPreparationInformation , \
               sizeof(HandoverPreparationInformationRrc_IEs_t));
      }
      CU_FREE(hoPrep->criticalExtensions.choice.c1, sizeof(struct HandoverPreparationInformationRrc__criticalExtensions__c1));
   }
}

/*******************************************************************
 *
 * @brief Fill feature sets
 *
 * @details
 *
 *    Function : fillFeatureSets
 *
 *    Functionality: Fill feature sets
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillFeatureSets(FeatureSets_t *featureSets)
{
   uint8_t idx, elementCnt;

   featureSets->featureSetsDownlink = NULLP;
   CU_ALLOC(featureSets->featureSetsDownlinkPerCC, sizeof(struct FeatureSets__featureSetsDownlinkPerCC));
   if(!featureSets->featureSetsDownlinkPerCC)
   {
      DU_LOG("\nERROR  --> F1AP : Memory allocation failed in fillFeatureSets");
      return RFAILED;
   }

   elementCnt = 1;
   featureSets->featureSetsDownlinkPerCC->list.count = elementCnt;
   featureSets->featureSetsDownlinkPerCC->list.size = elementCnt * sizeof(struct FeatureSetDownlinkPerCC *);
   CU_ALLOC(featureSets->featureSetsDownlinkPerCC->list.array, featureSets->featureSetsDownlinkPerCC->list.size);
   if(!featureSets->featureSetsDownlinkPerCC->list.array)
   {
      DU_LOG("\nERROR  --> F1AP : Memory allocation failed in fillFeatureSets");
      return RFAILED;
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
      CU_ALLOC(featureSets->featureSetsDownlinkPerCC->list.array[idx], sizeof(struct FeatureSetDownlinkPerCC));
      if(!featureSets->featureSetsDownlinkPerCC->list.array[idx])
      {
	 DU_LOG("\nERROR  --> F1AP : Memory allocation failed in fillFeatureSets");
	 return RFAILED;
      }
   }

   idx = 0;
   featureSets->featureSetsDownlinkPerCC->list.array[idx]->supportedSubcarrierSpacingDL = SubcarrierSpacing_kHz15;
   featureSets->featureSetsDownlinkPerCC->list.array[idx]->supportedBandwidthDL.present = SupportedBandwidth_PR_fr1;
   featureSets->featureSetsDownlinkPerCC->list.array[idx]->supportedBandwidthDL.choice.fr1 = SupportedBandwidth__fr1_mhz20;
   featureSets->featureSetsDownlinkPerCC->list.array[idx]->channelBW_90mhz = NULLP;
   featureSets->featureSetsDownlinkPerCC->list.array[idx]->maxNumberMIMO_LayersPDSCH = NULLP;

   CU_ALLOC(featureSets->featureSetsDownlinkPerCC->list.array[idx]->supportedModulationOrderDL, sizeof(ModulationOrder_t));
   if(!featureSets->featureSetsDownlinkPerCC->list.array[idx]->supportedModulationOrderDL)
   {
      DU_LOG("\nERROR  --> F1AP : Memory allocation failed in fillFeatureSets");
      return RFAILED;
   }
   *(featureSets->featureSetsDownlinkPerCC->list.array[idx]->supportedModulationOrderDL) = ModulationOrder_qam64;

   featureSets->featureSetsUplink = NULLP;
   CU_ALLOC(featureSets->featureSetsUplinkPerCC, sizeof(struct FeatureSets__featureSetsUplinkPerCC));
   if(!featureSets->featureSetsUplinkPerCC)
   {
      DU_LOG("\nERROR  --> F1AP : Memory allocation failed in fillFeatureSets");
      return RFAILED;
   }

   elementCnt = 1;
   featureSets->featureSetsUplinkPerCC->list.count = elementCnt;
   featureSets->featureSetsUplinkPerCC->list.size = elementCnt * sizeof(struct FeatureSetUplinkPerCC *);
   CU_ALLOC(featureSets->featureSetsUplinkPerCC->list.array, featureSets->featureSetsUplinkPerCC->list.size);
   if(!featureSets->featureSetsUplinkPerCC->list.array)
   {
      DU_LOG("\nERROR  --> F1AP : Memory allocation failed in fillFeatureSets");
      return RFAILED;
   }

   for(idx = 0; idx < elementCnt; idx++)
   {
      CU_ALLOC(featureSets->featureSetsUplinkPerCC->list.array[idx], sizeof(struct FeatureSetUplinkPerCC));
      if(!featureSets->featureSetsUplinkPerCC->list.array[idx])
      {
         DU_LOG("\nERROR  --> F1AP : Memory allocation failed in fillFeatureSets");
         return RFAILED;
      }
   }

   idx = 0;
   featureSets->featureSetsUplinkPerCC->list.array[idx]->supportedSubcarrierSpacingUL = SubcarrierSpacing_kHz15;
   featureSets->featureSetsUplinkPerCC->list.array[idx]->supportedBandwidthUL.present = SupportedBandwidth_PR_fr1;
   featureSets->featureSetsUplinkPerCC->list.array[idx]->supportedBandwidthUL.choice.fr1 = SupportedBandwidth__fr1_mhz20;
   featureSets->featureSetsUplinkPerCC->list.array[idx]->channelBW_90mhz = NULLP;
   featureSets->featureSetsUplinkPerCC->list.array[idx]->mimo_CB_PUSCH = NULLP;
   featureSets->featureSetsUplinkPerCC->list.array[idx]->maxNumberMIMO_LayersNonCB_PUSCH = NULLP;

   CU_ALLOC(featureSets->featureSetsUplinkPerCC->list.array[idx]->supportedModulationOrderUL, sizeof(ModulationOrder_t));
   if(!featureSets->featureSetsUplinkPerCC->list.array[idx]->supportedModulationOrderUL)
   {
      DU_LOG("\nERROR  --> F1AP : Memory allocation failed in fillFeatureSets");
      return RFAILED;
   }
   *(featureSets->featureSetsUplinkPerCC->list.array[idx]->supportedModulationOrderUL) = ModulationOrder_qam16;

   return ROK;
}

/*******************************************************************
 *
 * @brief Fill UE capability RAT container
 *
 * @details
 *
 *    Function : fillUeCapRatCont 
 *
 *    Functionality: Fill UE capability RAT container
 *
 * @params[in] UE Capability RAT container buffer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillUeCapRatCont(OCTET_STRING_t *ueCapRatContBuf)
{
   uint8_t             ret = ROK;
   uint8_t             idx, elementCnt;
   asn_enc_rval_t      encRetVal;
   UE_NR_Capability_t  ueNrCap;

   while(true)
   {
      ueNrCap.accessStratumRelease = AccessStratumRelease_rel15;

      /* Filling PDCP parameters */
      ueNrCap.pdcp_Parameters.supportedROHC_Profiles.profile0x0000 = false;
      ueNrCap.pdcp_Parameters.supportedROHC_Profiles.profile0x0001 = false;
      ueNrCap.pdcp_Parameters.supportedROHC_Profiles.profile0x0002 = false;
      ueNrCap.pdcp_Parameters.supportedROHC_Profiles.profile0x0003 = false;
      ueNrCap.pdcp_Parameters.supportedROHC_Profiles.profile0x0004 = false;
      ueNrCap.pdcp_Parameters.supportedROHC_Profiles.profile0x0006 = false;
      ueNrCap.pdcp_Parameters.supportedROHC_Profiles.profile0x0101 = false;
      ueNrCap.pdcp_Parameters.supportedROHC_Profiles.profile0x0102 = false;
      ueNrCap.pdcp_Parameters.supportedROHC_Profiles.profile0x0103 = false;
      ueNrCap.pdcp_Parameters.supportedROHC_Profiles.profile0x0104 = false;
      ueNrCap.pdcp_Parameters.maxNumberROHC_ContextSessions = PDCP_Parameters__maxNumberROHC_ContextSessions_cs2;
      ueNrCap.pdcp_Parameters.uplinkOnlyROHC_Profiles = NULLP;
      ueNrCap.pdcp_Parameters.continueROHC_Context = NULLP;
      ueNrCap.pdcp_Parameters.outOfOrderDelivery = NULLP;
      ueNrCap.pdcp_Parameters.shortSN = NULLP;
      ueNrCap.pdcp_Parameters.pdcp_DuplicationSRB = NULLP;
      ueNrCap.pdcp_Parameters.pdcp_DuplicationMCG_OrSCG_DRB = NULLP;

      ueNrCap.rlc_Parameters = NULLP;
      ueNrCap.mac_Parameters = NULLP;

      /* Filling PHY parameters */
      ueNrCap.phy_Parameters.phy_ParametersCommon = NULLP;
      ueNrCap.phy_Parameters.phy_ParametersXDD_Diff = NULLP;
      ueNrCap.phy_Parameters.phy_ParametersFRX_Diff = NULLP;
      ueNrCap.phy_Parameters.phy_ParametersFR1 = NULLP;
      ueNrCap.phy_Parameters.phy_ParametersFR2 = NULLP;

      /* Filling RF parameters */
      elementCnt = 1;
      ueNrCap.rf_Parameters.supportedBandListNR.list.count = elementCnt;
      ueNrCap.rf_Parameters.supportedBandListNR.list.size = elementCnt * sizeof(struct BandNR *);
      CU_ALLOC(ueNrCap.rf_Parameters.supportedBandListNR.list.array, ueNrCap.rf_Parameters.supportedBandListNR.list.size);
      if(!ueNrCap.rf_Parameters.supportedBandListNR.list.array)
      {
         DU_LOG("\nERROR  --> F1AP : Memory allocation failed in fillUeCapRatCont");
         ret = RFAILED;
         break;
      }

      for(idx = 0; idx < elementCnt; idx++)
      {
         CU_ALLOC(ueNrCap.rf_Parameters.supportedBandListNR.list.array[idx], sizeof(struct BandNR));
         if(!ueNrCap.rf_Parameters.supportedBandListNR.list.array[idx])
         {
            ret = RFAILED;
            break;
         }
      }
      if(ret == RFAILED)
         break;
      
      idx = 0;
      ueNrCap.rf_Parameters.supportedBandListNR.list.array[idx]->bandNR = 1;
      ueNrCap.rf_Parameters.supportedBandCombinationList = NULLP;
      ueNrCap.rf_Parameters.appliedFreqBandListFilter = NULLP;

      ueNrCap.measAndMobParameters = NULLP;
      ueNrCap.fdd_Add_UE_NR_Capabilities = NULLP;
      ueNrCap.tdd_Add_UE_NR_Capabilities = NULLP;
      ueNrCap.fr1_Add_UE_NR_Capabilities = NULLP;
      ueNrCap.fr2_Add_UE_NR_Capabilities = NULLP;
      ueNrCap.featureSets = NULLP;

      CU_ALLOC(ueNrCap.featureSets, sizeof(struct FeatureSets));
      if(!ueNrCap.featureSets)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in fillUeCapRatCont");
	 ret = RFAILED;
	 break;
      }

      if(fillFeatureSets(ueNrCap.featureSets) != ROK)
      {
         DU_LOG("\nERROR  -->  fillDLFeatureSets() failed ");
         ret = RFAILED;
         break;
      }

      ueNrCap.featureSetCombinations = NULLP;
      ueNrCap.lateNonCriticalExtension = NULLP;
      ueNrCap.nonCriticalExtension = NULLP;

      /* encode UE Capability RAT Container List into duToCuRrcContainer */
      xer_fprint(stdout, &asn_DEF_UE_NR_Capability, &ueNrCap);
      cmMemset((uint8_t *)encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = uper_encode(&asn_DEF_UE_NR_Capability, 0, &ueNrCap, PrepFinalEncBuf, encBuf);
   
      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG( "\nERROR  -->  F1AP : Could not encode UE Capability RAT Container (at %s)\n",\
            encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  F1AP : Created APER encodedbuffer for UE Capability RAT Container\n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         }
      }

      ueCapRatContBuf->size = encBufSize;
      CU_ALLOC(ueCapRatContBuf->buf, ueCapRatContBuf->size);
      if(!ueCapRatContBuf->buf)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in fillUeCapabilityContainer");
         break;
      }
      memcpy(ueCapRatContBuf->buf, encBuf, ueCapRatContBuf->size);
      ret = ROK;
      break;
   }
   freeUeCapRatCont(&ueNrCap);
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill UE Capability RAT container list
 *
 * @details
 *
 *    Function : fillUeCapRatContList
 *
 *    Functionality: Fill UE Capability RAT container list
 
 *
 * @params[in] UE capability RAT container list
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillUeCapRatContList(UE_CapabilityRAT_ContainerListRRC_t  *ueCapablityList)
{
    uint8_t          ret = RFAILED;
    uint8_t          idx, elementCnt;

    while(true)
    {
       elementCnt = 1;
       ueCapablityList->list.count = elementCnt;
       ueCapablityList->list.size = elementCnt * sizeof(UE_CapabilityRAT_Container_t *);

       CU_ALLOC(ueCapablityList->list.array, ueCapablityList->list.size);
       if(!ueCapablityList->list.array)
       {
          DU_LOG("\nERROR  -->  Memory allocation failed in fillUeCapRatContList");
          ret = RFAILED;
          break;
       }

       for(idx=0; idx<elementCnt; idx++)
       {
          CU_ALLOC(ueCapablityList->list.array[idx], sizeof(UE_CapabilityRAT_Container_t));
          if(ueCapablityList->list.array[idx] == NULLP)
          {
             DU_LOG("\nERROR  -->  Memory allocation failed in fillUeCapRatContList");
             ret = RFAILED;
             break;
          }
       }
       idx = 0;
       ueCapablityList->list.array[idx]->rat_Type = RAT_Type_nr;
       if(fillUeCapRatCont(&ueCapablityList->list.array[idx]->ue_CapabilityRAT_Container) != ROK)
       {
          DU_LOG("\nERROR  -->  F1AP : Failed to fill UE capability RAT Conatiner");
          ret = RFAILED;
          break;
       }

       ret = ROK;
       break;
    }
    return ret;
}

/*******************************************************************
 *
 * @brief Fill UE Capability RAT container list octet string
 *
 * @details
 *
 *    Function : fillUeCapRatContListBuf
 *
 *    Functionality: Fill UE Capability RAT container list octet string
 
 *
 * @params[in] UE capability RAT container list buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillUeCapRatContListBuf(UE_CapabilityRAT_ContainerList_t *ueCapablityListBuf)
{
   uint8_t          ret = RFAILED;
   asn_enc_rval_t   encRetVal;
   UE_CapabilityRAT_ContainerListRRC_t  ueCapablityList;

   while(true)
   {
      ret = fillUeCapRatContList(&ueCapablityList);
      if(ret != ROK)
      {
         DU_LOG( "\nERROR  -->  F1AP : Failed to fill UE Capability RAT container list");
         break;
      }

      /* encode UE Capability RAT Container List into duToCuRrcContainer */
      xer_fprint(stdout, &asn_DEF_UE_CapabilityRAT_ContainerListRRC, &ueCapablityList);
      cmMemset((uint8_t *)encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = uper_encode(&asn_DEF_UE_CapabilityRAT_ContainerListRRC, 0, \
            &ueCapablityList, PrepFinalEncBuf, encBuf);

      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG( "\nERROR  -->  F1AP : Could not encode UE Capability RAT Container (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  F1AP : Created APER encodedbuffer for UE Capability RAT Container\n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         }
      }

      ueCapablityListBuf->size = encBufSize;
      CU_ALLOC(ueCapablityListBuf->buf, ueCapablityListBuf->size);
      if(!ueCapablityListBuf->buf)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in fillUeCapabilityContainer");
         break;
      }
      memcpy(ueCapablityListBuf->buf, encBuf, ueCapablityListBuf->size);
      ret = ROK;
      break;
   }
   freeUeCapRatContList(&ueCapablityList);
   return ret;
}

/*******************************************************************
 *
 * @brief Free Measurement Timing Configuration
 *
 * @details
 *
 *    Function : freeMeasuementTimingConfig
 *
 *    Functionality: Free Measurement Timing Configuration
 *
 * @params[in] MeasurementTimingConfiguration_IEs_t measTimingCfg
 * @return void
 *
 * ****************************************************************/
void freeMeasuementTimingConfig(MeasurementTimingConfigurationRrc_t measTimingConfig)
{
   uint8_t measCfgIdx;
   MeasurementTimingConfigurationRrc_IEs_t *measTimingCfg = NULLP;
   MeasTiming_t *measTiming = NULLP;

   if(measTimingConfig.criticalExtensions.choice.c1)
   {
      if(measTimingConfig.criticalExtensions.choice.c1->choice.measTimingConf)
      {
         measTimingCfg = measTimingConfig.criticalExtensions.choice.c1->choice.measTimingConf;
         if(measTimingCfg->measTiming)
         {
            if(measTimingCfg->measTiming->list.array)
            {
               for(measCfgIdx = 0; measCfgIdx < measTimingCfg->measTiming->list.count; measCfgIdx++)
               {
                  measTiming = measTimingCfg->measTiming->list.array[measCfgIdx];
                  CU_FREE(measTiming->frequencyAndTiming, sizeof(struct MeasTiming__frequencyAndTiming));
                  CU_FREE(measTimingCfg->measTiming->list.array[measCfgIdx], sizeof(MeasTiming_t));
               }
               CU_FREE(measTimingCfg->measTiming->list.array, measTimingCfg->measTiming->list.size);
            }
            CU_FREE(measTimingCfg->measTiming, sizeof(MeasTimingList_t));
         }
         CU_FREE(measTimingConfig.criticalExtensions.choice.c1->choice.measTimingConf, sizeof(struct MeasurementTimingConfigurationRrc_IEs));
      }
      CU_FREE(measTimingConfig.criticalExtensions.choice.c1, sizeof(struct MeasurementTimingConfigurationRrc__criticalExtensions__c1));
   }
}

/*******************************************************************
 *
 * @brief Fill Measurement Timing Configuration
 *
 * @details
 *
 *    Function : fillMeasTimingCfg
 *
 *    Functionality: Fill Measurement Timing Configuration
 *
 * @params[in] MeasurementTimingConfiguration_IEs_t measTimingCfg
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillMeasTimingCfg(MeasurementTimingConfigurationRrc_IEs_t *measTimingCfg)
{
   uint8_t elementCnt = 0;
   uint8_t measCfgIdx = 0; 
   MeasTiming_t *measTiming;
   SSB_MTC_t *smtc;

   CU_ALLOC(measTimingCfg->measTiming, sizeof(MeasTimingList_t));
   if(!measTimingCfg->measTiming)
   {
      DU_LOG( "\nERROR  -->  F1AP : Failed to allocated memory  for measTiming in fillMeasTimingCfg");
      return RFAILED;
   }

   elementCnt = 1;
   measTimingCfg->measTiming->list.count = elementCnt;
   measTimingCfg->measTiming->list.size = measTimingCfg->measTiming->list.count * sizeof(MeasTiming_t *);
   CU_ALLOC(measTimingCfg->measTiming->list.array, measTimingCfg->measTiming->list.size);
   if(!measTimingCfg->measTiming->list.array)
   {
      DU_LOG( "\nERROR  -->  F1AP : Failed to allocated memory for measTiming array in fillMeasTimingCfg");
      return RFAILED;
   }

   for(measCfgIdx = 0; measCfgIdx < measTimingCfg->measTiming->list.count; measCfgIdx++)
   {
      CU_ALLOC(measTimingCfg->measTiming->list.array[measCfgIdx], sizeof(MeasTiming_t));
      if(!measTimingCfg->measTiming->list.array[measCfgIdx])
      {
         DU_LOG( "\nERROR  -->  F1AP : Failed to allocated memory for measTiming array element in fillMeasTimingCfg");
         return RFAILED;
      }
   }

   measCfgIdx = 0;
   measTiming = measTimingCfg->measTiming->list.array[measCfgIdx];
   CU_ALLOC(measTiming->frequencyAndTiming, sizeof(struct MeasTiming__frequencyAndTiming));
   if(!measTiming->frequencyAndTiming)
   {
      DU_LOG( "\nERROR  -->  F1AP : Failed to allocated memory for frequency and timing IE in fillMeasTimingCfg");
      return RFAILED;
   }

   measTiming->frequencyAndTiming->carrierFreq =  623800; /* This is a randomly chosen value since ARFCN configured in DU is 623400 */
   measTiming->frequencyAndTiming->ssbSubcarrierSpacing = SubcarrierSpacing_kHz15;

   smtc = &measTiming->frequencyAndTiming->ssb_MeasurementTimingConfiguration;
   smtc->periodicityAndOffset.present = SSB_MTC__periodicityAndOffset_PR_sf20;
   smtc->periodicityAndOffset.choice.sf20 = 0;
   smtc->duration = SSB_MTC__duration_sf1;
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill Measurement Timing Configuration Octet string
 *
 * @details
 *
 *    Function : fillMeasConfigBuf
 *
 *    Functionality: Fill Measurement Timing Configuration Octet string
 
 *
 * @params[in] MeasConfig_t *measConfgBuf
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillMeasTimingConfigBuf(MeasConfig_t *measTimingConfigBuf)
{
   uint8_t          ret = RFAILED;
   asn_enc_rval_t   encRetVal;
   MeasurementTimingConfigurationRrc_t measTimingConfig;

   while(true)
   {
      measTimingConfig.criticalExtensions.present = MeasurementTimingConfigurationRrc__criticalExtensions_PR_c1;
      CU_ALLOC(measTimingConfig.criticalExtensions.choice.c1, sizeof(struct MeasurementTimingConfigurationRrc__criticalExtensions__c1));
      if(!measTimingConfig.criticalExtensions.choice.c1)
      {
         DU_LOG( "\nERROR  -->  F1AP : Failed to allocated memory for measurement configuration extension");
         return RFAILED;
      } 
      measTimingConfig.criticalExtensions.choice.c1->present = MeasurementTimingConfigurationRrc__criticalExtensions__c1_PR_measTimingConf;

      CU_ALLOC(measTimingConfig.criticalExtensions.choice.c1->choice.measTimingConf, sizeof(struct MeasurementTimingConfigurationRrc_IEs));
      if(!measTimingConfig.criticalExtensions.choice.c1->choice.measTimingConf)
      {
         DU_LOG( "\nERROR  -->  F1AP : Failed to allocated memory for measurement timing configuration IE");
         return RFAILED;
      }

      ret = fillMeasTimingCfg(measTimingConfig.criticalExtensions.choice.c1->choice.measTimingConf);
      if(ret != ROK)
      {
         DU_LOG( "\nERROR  -->  F1AP : Failed to fill measurement timing configuration IE");
         break;
      }

      /* Encode measurement timing configuration into octet string */
      xer_fprint(stdout, &asn_DEF_MeasurementTimingConfigurationRrc, &measTimingConfig);
      cmMemset((uint8_t *)encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = uper_encode(&asn_DEF_MeasurementTimingConfigurationRrc, 0, &measTimingConfig, PrepFinalEncBuf, encBuf);

      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG( "\nERROR  -->  F1AP : Could not encode Measurement Timing Configuration (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  F1AP : Created APER encodedbuffer for Measurement timing configuration\n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         }
      }

      measTimingConfigBuf->size = encBufSize;
      CU_ALLOC(measTimingConfigBuf->buf, measTimingConfigBuf->size);
      if(!measTimingConfigBuf->buf)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed fillMeasTimingConfigBuf");
         break;
      }
      memcpy(measTimingConfigBuf->buf, encBuf, measTimingConfigBuf->size);
      ret = ROK;
      break;
   }
   freeMeasuementTimingConfig(measTimingConfig);
   return ret;
}

/******************************************************************
 *
 * @brief Free RRC reconfiguration non-critical extension
 *
 * @details
 *
 *    Function : freeRrcReconfigNonCriticalExt
 *
 *    Functionality: Free RRC reconfiguration non-critical extension
 *
 * @params[in] RRC reconfiguration IE
 * @return void
 *
 * ****************************************************************/
void freeRrcReconfigNonCriticalExt(RRCReconfiguration_v1530_IEs_t *rrcRecfg)
{
   if(rrcRecfg->masterCellGroup)
   {
      CU_FREE(rrcRecfg->masterCellGroup, sizeof(OCTET_STRING_t));
   }
}

/******************************************************************
 *
 * @brief Free measurement object addmod list
 *
 * @details
 *
 *    Function : freeMeasObjToAddModList
 *
 *    Functionality: Free measurement object add mod list
 *
 * @params[in] Measurement object add/mod list
 * @return void
 *
 * ****************************************************************/
void freeMeasObjToAddModList(MeasObjectToAddModList_t *measObjList)
{
   uint8_t objIdx;
   MeasObjectNR_t *measObject;

   if(measObjList->list.array)
   {
      for(objIdx = 0; objIdx < measObjList->list.size; objIdx++)
      {
         if(measObjList->list.array[objIdx])
         {
            if(measObjList->list.array[objIdx]->measObject.choice.measObjectNR)
            {
               measObject = measObjList->list.array[objIdx]->measObject.choice.measObjectNR;
               CU_FREE(measObject->ssbFrequency, sizeof(ARFCN_ValueNR_t));
               CU_FREE(measObject->ssbSubcarrierSpacing, sizeof(SubcarrierSpacing_t));
               CU_FREE(measObject->smtc1 , sizeof(SSB_MTC_t));
               if(measObject->absThreshSS_BlocksConsolidation)
               {
                  CU_FREE(measObject->absThreshSS_BlocksConsolidation->thresholdRSRP, sizeof(RSRP_Range_t));
                  CU_FREE(measObject->absThreshSS_BlocksConsolidation->thresholdRSRQ, sizeof(RSRQ_Range_t));
                  CU_FREE(measObject->absThreshSS_BlocksConsolidation->thresholdSINR, sizeof(SINR_Range_t));
                  CU_FREE(measObject->absThreshSS_BlocksConsolidation, sizeof(ThresholdNR_t));
               }
               CU_FREE(measObject->nrofSS_BlocksToAverage, sizeof(long));
               CU_FREE(measObject->offsetMO.rsrpOffsetSSB, sizeof(Q_OffsetRange_t));
               CU_FREE(measObject->offsetMO.rsrqOffsetSSB, sizeof(Q_OffsetRange_t));
               CU_FREE(measObject->offsetMO.sinrOffsetSSB, sizeof(Q_OffsetRange_t));
               CU_FREE(measObjList->list.array[objIdx]->measObject.choice.measObjectNR, sizeof(MeasObjectNR_t));
            }
            CU_FREE(measObjList->list.array[objIdx], sizeof(MeasObjectToAddMod_t));
         }
      }
      CU_FREE(measObjList->list.array, measObjList->list.size);
   }
}

/******************************************************************
 *
 * @brief Free report config add mod list
 *
 * @details
 *
 *    Function : freeReportCfgToAddModList
 *
 *    Functionality: Free report config add mod list
 *
 * @params[in] Report config list
 * @return void
 *
 * ****************************************************************/
void freeReportCfgToAddModList(ReportConfigToAddModList_t *reportCfgList)
{
   uint8_t reportCfgIdx;
   ReportConfigToAddMod_t *reportCfg;
   ReportConfigNR_t *reportCfgNr;
   EventTriggerConfig_t *eventTriggCfg;

   if(reportCfgList->list.array)
   {
      for(reportCfgIdx=0; reportCfgIdx < reportCfgList->list.count; reportCfgIdx++)
      {
         if(reportCfgList->list.array[reportCfgIdx])
         {
            reportCfg = reportCfgList->list.array[reportCfgIdx];
            if(reportCfg->reportConfig.choice.reportConfigNR)
            {
               reportCfgNr = reportCfg->reportConfig.choice.reportConfigNR;
               if(reportCfgNr->reportType.choice.eventTriggered)
               {
                  eventTriggCfg = reportCfgNr->reportType.choice.eventTriggered;
                  CU_FREE(eventTriggCfg->eventId.choice.eventA3, sizeof(struct EventTriggerConfig__eventId__eventA3));
                  CU_FREE(eventTriggCfg->reportQuantityRS_Indexes, sizeof(MeasReportQuantity_t));
                  CU_FREE(eventTriggCfg->maxNrofRS_IndexesToReport, sizeof(long));
                  CU_FREE(reportCfgNr->reportType.choice.eventTriggered, sizeof(EventTriggerConfig_t));
               }
            }
         }
         CU_FREE(reportCfgList->list.array[reportCfgIdx], sizeof(ReportConfigToAddMod_t));
      }
      CU_FREE(reportCfgList->list.array, reportCfgList->list.size);
   }
}

/******************************************************************
 *
 * @brief Free measurement id to add mod list
 *
 * @details
 *
 *    Function : freeMeasIdToAddModList
 *
 *    Functionality: Free measurement id to add mod list
 *
 * @params[in] Measurement id to add mod list
 * @return void
 *
 * ****************************************************************/
void freeMeasIdToAddModList(MeasIdToAddModList_t *measIdList)
{
   uint8_t measIdIdx;
   if(measIdList->list.array)
   {
      for(measIdIdx=0; measIdIdx < measIdList->list.count; measIdIdx++)
      {
         CU_FREE(measIdList->list.array[measIdIdx], sizeof(MeasIdToAddMod_t));
      }
      CU_FREE(measIdList->list.array, measIdList->list.size);
   }
}

/*******************************************************************
 *
 * @brief Free quantity config
 *
 * @details
 *
 *    Function : freeQunatityConfig
 *
 *    Functionality: Free quantity config
 *
 * @params[in] Quantity Config
 * @return void
 *
 * ****************************************************************/
void freeQuantityConfig(QuantityConfig_t *quantityCfg)
{
   uint8_t quanCfgIdx;
   QuantityConfigNR_t *quantityCfgNr;

   if(quantityCfg->quantityConfigNR_List)
   {
      if(quantityCfg->quantityConfigNR_List->list.array)
      {
         for(quanCfgIdx = 0; quanCfgIdx < quantityCfg->quantityConfigNR_List->list.count; quanCfgIdx++)
         {
            if(quantityCfg->quantityConfigNR_List->list.array[quanCfgIdx])
            {
               quantityCfgNr = quantityCfg->quantityConfigNR_List->list.array[quanCfgIdx];
               CU_FREE(quantityCfgNr->quantityConfigCell.ssb_FilterConfig.filterCoefficientRSRP, sizeof(FilterCoefficient_t));
               CU_FREE(quantityCfgNr->quantityConfigCell.ssb_FilterConfig.filterCoefficientRSRQ, sizeof(FilterCoefficient_t));
               CU_FREE(quantityCfgNr->quantityConfigCell.ssb_FilterConfig.filterCoefficientRS_SINR, sizeof(FilterCoefficient_t));
               if(quantityCfgNr->quantityConfigRS_Index)
               {
                  CU_FREE(quantityCfgNr->quantityConfigRS_Index->ssb_FilterConfig.filterCoefficientRSRP, sizeof(FilterCoefficient_t));
                  CU_FREE(quantityCfgNr->quantityConfigRS_Index->ssb_FilterConfig.filterCoefficientRSRQ, sizeof(FilterCoefficient_t));
                  CU_FREE(quantityCfgNr->quantityConfigRS_Index->ssb_FilterConfig.filterCoefficientRS_SINR, sizeof(FilterCoefficient_t));
                  CU_FREE(quantityCfgNr->quantityConfigRS_Index, sizeof(QuantityConfigRS_t));
               }
               CU_FREE(quantityCfg->quantityConfigNR_List->list.array[quanCfgIdx], sizeof(QuantityConfigNR_t));
            }
         }
         CU_FREE(quantityCfg->quantityConfigNR_List->list.array, quantityCfg->quantityConfigNR_List->list.size);
      }
      CU_FREE(quantityCfg->quantityConfigNR_List, sizeof(struct QuantityConfig__quantityConfigNR_List));
   }
}

/******************************************************************
 *
 * @brief Free measurement Config
 *
 * @details
 *
 *    Function : freeMeasConfig
 *
 *    Functionality: Free measurement config
 *
 * @params[in] Measurement config
 * @return void
 *
 * ****************************************************************/
void freeMeasConfig(MeasConfigRrc_t *measConfig)
{
   if(measConfig->measObjectToAddModList)
   {
      freeMeasObjToAddModList(measConfig->measObjectToAddModList);
      CU_FREE(measConfig->measObjectToAddModList, sizeof(MeasObjectToAddModList_t));
   }
   if(measConfig->reportConfigToAddModList)
   {
      freeReportCfgToAddModList(measConfig->reportConfigToAddModList);
      CU_FREE(measConfig->reportConfigToAddModList, sizeof(ReportConfigToAddModList_t));
   }
   if(measConfig->measIdToAddModList)
   {
      freeMeasIdToAddModList(measConfig->measIdToAddModList);
      CU_FREE(measConfig->measIdToAddModList, sizeof(MeasIdToAddModList_t));
   }
   if(measConfig->s_MeasureConfig)
   {
      CU_FREE(measConfig->s_MeasureConfig, sizeof(struct MeasConfigRrc__s_MeasureConfig));
   }
   if(measConfig->quantityConfig)
   {
      freeQuantityConfig(measConfig->quantityConfig);
      CU_FREE(measConfig->quantityConfig, sizeof(QuantityConfig_t));
   }
}
/******************************************************************
 *
 * @brief Free DRB to AddMod List
 *
 * @details
 *
 *    Function : freeDrbToAddModList
 *
 *    Functionality: Free SRB to AddMod List
 *
 * @params[in] SBR to add/mod list
 * @return void
 *
 * ****************************************************************/
void freeDrbToAddModList(DRB_ToAddModList_t *drbToAddList)
{
   uint8_t drbIdx;
   if(drbToAddList->list.array)
   {
      for(drbIdx = 0; drbIdx < drbToAddList->list.count; drbIdx++)
      {
         if(drbToAddList->list.array[drbIdx]->pdcp_Config)
         {
            if(drbToAddList->list.array[drbIdx]->pdcp_Config->drb)
            {
               CU_FREE(drbToAddList->list.array[drbIdx]->pdcp_Config->drb->discardTimer, sizeof(long));
               CU_FREE(drbToAddList->list.array[drbIdx]->pdcp_Config->drb->pdcp_SN_SizeUL, sizeof(long));
               CU_FREE(drbToAddList->list.array[drbIdx]->pdcp_Config->drb->pdcp_SN_SizeDL, sizeof(long));
               CU_FREE(drbToAddList->list.array[drbIdx]->pdcp_Config->drb, sizeof(struct PDCP_Config__drb));
            }
            CU_FREE(drbToAddList->list.array[drbIdx]->pdcp_Config->t_Reordering, sizeof(long));
            CU_FREE(drbToAddList->list.array[drbIdx]->pdcp_Config, sizeof(PDCP_Config_t));
         }
         CU_FREE(drbToAddList->list.array[drbIdx], sizeof(DRB_ToAddMod_t));
      }
      CU_FREE(drbToAddList->list.array, drbToAddList->list.size);
   }
}

/******************************************************************
 *
 * @brief Free SRB to AddMod List
 *
 * @details
 *
 *    Function : freeSrbToAddModList
 *
 *    Functionality: Free SRB to AddMod List
 *
 * @params[in] SBR to add/mod list
 * @return void
 *
 * ****************************************************************/
void freeSrbToAddModList(SRB_ToAddModList_t *srbToAddList)
{
   uint8_t srbIdx;
   if(srbToAddList->list.array)
   {
      for(srbIdx = 0; srbIdx < srbToAddList->list.count; srbIdx++)
      {
         CU_FREE(srbToAddList->list.array[srbIdx]->reestablishPDCP, sizeof(long));
         if(srbToAddList->list.array[srbIdx]->pdcp_Config)
         {
            CU_FREE(srbToAddList->list.array[srbIdx]->pdcp_Config->t_Reordering, sizeof(long));
            CU_FREE(srbToAddList->list.array[srbIdx]->pdcp_Config, sizeof(PDCP_Config_t));
         }

         CU_FREE(srbToAddList->list.array[srbIdx], sizeof(SRB_ToAddMod_t));
      }
      CU_FREE(srbToAddList->list.array, srbToAddList->list.size);
   }
}

/******************************************************************
 *
 * @brief Free Radio Bearer Config
 *
 * @details
 *
 *    Function : freeRadioBearerConfig 
 *
 *    Functionality: Free Radio Bearer config
 *
 * @params[in] Radio bearer config
 * @return void
 *
 * ****************************************************************/
void freeRadioBearerConfig(RadioBearerConfig_t *radioBearerConfig)
{
   if(radioBearerConfig->srb_ToAddModList)
   {
      freeSrbToAddModList(radioBearerConfig->srb_ToAddModList);
      CU_FREE(radioBearerConfig->srb_ToAddModList, sizeof(SRB_ToAddModList_t));
   }
   if(radioBearerConfig->drb_ToAddModList)
   {
      freeDrbToAddModList(radioBearerConfig->drb_ToAddModList);
      CU_FREE(radioBearerConfig->drb_ToAddModList, sizeof(DRB_ToAddModList_t));
   }
}

/******************************************************************
 *
 * @brief Free reconfiguration message
 *
 * @details
 *
 *    Function : freeRrcReconfig
 *
 *    Functionality: Free reconfiguration message
 *
 * @params[in] RRC Reconfiguration message
 * @return void
 *
 * ****************************************************************/
void freeRrcReconfig(RRCReconfiguration_t *rrcReconfig)
{
   if(rrcReconfig->criticalExtensions.choice.rrcReconfiguration)
   {
      if(rrcReconfig->criticalExtensions.choice.rrcReconfiguration->radioBearerConfig)
      {
         freeRadioBearerConfig(rrcReconfig->criticalExtensions.choice.rrcReconfiguration->radioBearerConfig);
         CU_FREE(rrcReconfig->criticalExtensions.choice.rrcReconfiguration->radioBearerConfig, sizeof(RadioBearerConfig_t));
      }
      if(rrcReconfig->criticalExtensions.choice.rrcReconfiguration->measConfig)
      {
         freeMeasConfig(rrcReconfig->criticalExtensions.choice.rrcReconfiguration->measConfig);
         CU_FREE(rrcReconfig->criticalExtensions.choice.rrcReconfiguration->measConfig, sizeof(MeasConfigRrc_t));
      }
      if(rrcReconfig->criticalExtensions.choice.rrcReconfiguration->nonCriticalExtension)
      {
         freeRrcReconfigNonCriticalExt(rrcReconfig->criticalExtensions.choice.rrcReconfiguration->nonCriticalExtension);
         CU_FREE(rrcReconfig->criticalExtensions.choice.rrcReconfiguration->nonCriticalExtension, sizeof(struct RRCReconfiguration_v1530_IEs));
      }
      CU_FREE(rrcReconfig->criticalExtensions.choice.rrcReconfiguration, sizeof(RRCReconfiguration_IEs_t));
   }
}

/******************************************************************
 *
 * @brief Fill SRB To Add Mod list
 *
 * @details
 *
 *    Function : fillSrbToAddModList
 *
 *    Functionality: fill SRB to Add Mod list
 *
 * @params[in] UE control block
 *             SRB to Add/Mod list
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillSrbToAddModList(CuUeCb *ueCb, SRB_ToAddModList_t *srbToAddList, bool updateAllRbCfg)
{
   uint8_t srbIdx, srbDbIdx, elementCnt = 0;

   if(updateAllRbCfg)
      elementCnt = ueCb->numSrb;
   else
   {
      for(srbDbIdx=0; srbDbIdx < ueCb->numSrb; srbDbIdx++)
      {
         if(ueCb->srbList[srbDbIdx].cfgSentToUe == false)
            elementCnt++;
      }
   }

   if(!elementCnt)
   {
      DU_LOG("INFO  --> F1AP : No SRB available to add or modify");
      return ROK;
   }

   srbToAddList->list.count = elementCnt;
   srbToAddList->list.size = srbToAddList->list.count * sizeof(SRB_ToAddMod_t *);

   CU_ALLOC(srbToAddList->list.array, srbToAddList->list.size);
   if(!srbToAddList->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for SRB to AddMod list array in fillSrbToAddModList");
      return RFAILED;
   }

   srbIdx = 0;
   for(srbDbIdx = 0; srbDbIdx < ueCb->numSrb; srbDbIdx++)
   {
      if(!updateAllRbCfg && ueCb->srbList[srbDbIdx].cfgSentToUe)
         continue;

      CU_ALLOC(srbToAddList->list.array[srbIdx], sizeof(SRB_ToAddMod_t));
      if(!srbToAddList->list.array[srbIdx])
      {
         DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for SRB to AddMod list element in fillSrbToAddModList");
         return RFAILED;
      }

      srbToAddList->list.array[srbIdx]->srb_Identity = ueCb->srbList[srbDbIdx].srbId;

      /* Reestablish PDCP */
      CU_ALLOC(srbToAddList->list.array[srbIdx]->reestablishPDCP, sizeof(long));
      if(!srbToAddList->list.array[srbIdx]->reestablishPDCP)
      {
         DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for reestablish PDCP IE in fillSrbToAddModList");
         return RFAILED;
      }
      *(srbToAddList->list.array[srbIdx]->reestablishPDCP) = SRB_ToAddMod__reestablishPDCP_true;

      /* PDCP configuration */
      CU_ALLOC(srbToAddList->list.array[srbIdx]->pdcp_Config, sizeof(PDCP_Config_t));
      if(!srbToAddList->list.array[srbIdx]->pdcp_Config)
      {
         DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for PDCP config IE in fillSrbToAddModList");
         return RFAILED;
      }

      /* Reordering timer */
      CU_ALLOC(srbToAddList->list.array[srbIdx]->pdcp_Config->t_Reordering, sizeof(long));
      if(!srbToAddList->list.array[srbIdx]->pdcp_Config->t_Reordering)
      {
         DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for reordering timer in fillSrbToAddModList");
         return RFAILED;
      }
      *(srbToAddList->list.array[srbIdx]->pdcp_Config->t_Reordering) = PDCP_Config__t_Reordering_ms40;
      
      srbIdx++;
   }
   return ROK;
}

/******************************************************************
 *
 * @biief Fill DRBeTo Add Mod list
 *
 * @details
 *
 *    Function : fillDrbToAddModList
 *
 *    Functionality: fill DRB to Add Mod list
 *
 * @params[in] UE control block
 *             DRB to Add/Mod list
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillDrbToAddModList(CuUeCb *ueCb, DRB_ToAddModList_t *drbToAddList, bool updateAllRbCfg)
{
   uint8_t drbIdx, drbDbIdx, elementCnt = 0;

   if(updateAllRbCfg)
      elementCnt = ueCb->numDrb;
   else
   {
      for(drbDbIdx=0; drbDbIdx < ueCb->numDrb; drbDbIdx++)
      {     
         if(ueCb->drbList[drbDbIdx].cfgSentToUe == false)
            elementCnt++;
      }     
   }

   if(!elementCnt)
   {
      DU_LOG("INFO  --> F1AP : No DRB available to add or modify");
      return ROK;
   }
   

   drbToAddList->list.count = elementCnt;
   drbToAddList->list.size = drbToAddList->list.count * sizeof(DRB_ToAddMod_t *);

   CU_ALLOC(drbToAddList->list.array, drbToAddList->list.size);
   if(!drbToAddList->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for DRB to AddMod list array in fillDrbToAddModList");
      return RFAILED;
   }

   drbIdx = 0;
   for(drbDbIdx=0; drbDbIdx < ueCb->numDrb; drbDbIdx++)
   {
      if(!updateAllRbCfg && ueCb->drbList[drbDbIdx].cfgSentToUe)
         continue;

      CU_ALLOC(drbToAddList->list.array[drbIdx], sizeof(DRB_ToAddMod_t));
      if(!drbToAddList->list.array[drbIdx])
      {
         DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for DRB to AddMod list elements in fillDrbToAddModList");
         return RFAILED;
      }

      /* DRB Id */
      drbToAddList->list.array[drbIdx]->drb_Identity = ueCb->drbList[drbDbIdx].drbId;

      /* PDCP Config */
      CU_ALLOC(drbToAddList->list.array[drbIdx]->pdcp_Config, sizeof(PDCP_Config_t));
      if(!drbToAddList->list.array[drbIdx]->pdcp_Config)
      {
         DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for PDCP config IE in fillDrbToAddModList");
         return RFAILED;
      }

      /* PDCP Config -> DRB */
      CU_ALLOC(drbToAddList->list.array[drbIdx]->pdcp_Config->drb, sizeof(struct PDCP_Config__drb));
      if(!drbToAddList->list.array[drbIdx]->pdcp_Config->drb)
      {
         DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for PDCP config drb in fillDrbToAddModList");
         return RFAILED;
      }

      /* DRB -> Discard Timer */
      CU_ALLOC(drbToAddList->list.array[drbIdx]->pdcp_Config->drb->discardTimer, sizeof(long));
      if(!drbToAddList->list.array[drbIdx]->pdcp_Config->drb->discardTimer)
      {
         DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for DRB discard timer in fillDrbToAddModList");
         return RFAILED;
      }
      *(drbToAddList->list.array[drbIdx]->pdcp_Config->drb->discardTimer) = PDCP_Config__drb__discardTimer_infinity;

      /* UL PDCP SN length */
      CU_ALLOC(drbToAddList->list.array[drbIdx]->pdcp_Config->drb->pdcp_SN_SizeUL, sizeof(long));
      if(!drbToAddList->list.array[drbIdx]->pdcp_Config->drb->pdcp_SN_SizeUL)
      {
         DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for UL SN length in fillDrbToAddModList");
         return RFAILED;
      }
      *(drbToAddList->list.array[drbIdx]->pdcp_Config->drb->pdcp_SN_SizeUL) = PDCP_Config__drb__pdcp_SN_SizeUL_len12bits;

      /* DL PDCP SN length */
      CU_ALLOC(drbToAddList->list.array[drbIdx]->pdcp_Config->drb->pdcp_SN_SizeDL, sizeof(long));
      if(!drbToAddList->list.array[drbIdx]->pdcp_Config->drb->pdcp_SN_SizeDL)
      {
         DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for DL SN length in fillDrbToAddModList");
         return RFAILED;
      }
      *(drbToAddList->list.array[drbIdx]->pdcp_Config->drb->pdcp_SN_SizeDL) = PDCP_Config__drb__pdcp_SN_SizeDL_len12bits;

      /* Header Compression */
      drbToAddList->list.array[drbIdx]->pdcp_Config->drb->headerCompression.present = PDCP_Config__drb__headerCompression_PR_notUsed;
       drbToAddList->list.array[drbIdx]->pdcp_Config->drb->headerCompression.choice.notUsed = NULLP;

      /* Reordering timer */
      CU_ALLOC(drbToAddList->list.array[drbIdx]->pdcp_Config->t_Reordering, sizeof(long));
      if(!drbToAddList->list.array[drbIdx]->pdcp_Config->t_Reordering)
      {
         DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for reordering timer in fillDrbToAddModList");
         return RFAILED;
      }
      *(drbToAddList->list.array[drbIdx]->pdcp_Config->t_Reordering) = PDCP_Config__t_Reordering_ms40;

      drbIdx++;
   }

   return ROK;
}

/******************************************************************
 *
 * @brief Fill Radio bearer configuration
 *
 * @details
 *
 *    Function : fillRadioBearerConfig
 *
 *    Functionality: Fill Radio bearer configuration
 *
 * @params[in] UE control block
 *             Radio bearer config pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillRadioBearerConfig(CuUeCb *ueCb, RadioBearerConfig_t *radioBearerConfig, bool updateAllRbCfg)
{
   /* SRB To Add/Mod List */
   CU_ALLOC(radioBearerConfig->srb_ToAddModList, sizeof(SRB_ToAddModList_t));
   if(!radioBearerConfig->srb_ToAddModList)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for SRB to AddMod List in fillRadioBearerConfig");
      return RFAILED;
   }
   if(fillSrbToAddModList(ueCb, radioBearerConfig->srb_ToAddModList, updateAllRbCfg) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP: failed to fill SRB to AddMod List");
      return RFAILED;
   }

   /* DRB To Add/Mod List */
   CU_ALLOC(radioBearerConfig->drb_ToAddModList, sizeof(DRB_ToAddModList_t));
   if(!radioBearerConfig->drb_ToAddModList)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for DRB to AddMod List in fillRadioBearerConfig");
      return RFAILED;
    }
   if(fillDrbToAddModList(ueCb, radioBearerConfig->drb_ToAddModList, updateAllRbCfg) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP: failed to fill DRB to AddMod List ");
      return RFAILED;
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Fill measurement object to add/mod list
 *
 * @details
 *
 *    Function : fillMeasObjToAddModList
 *
 *    Functionality: Fill measurement object to add/mod list
 *
 * @params[in] Measurement object to add/mod list
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillMeasObjToAddModList(MeasObjectToAddModList_t *measObjList)
{
   uint8_t elementCnt, objIdx;
   MeasObjectNR_t *measObject;

   elementCnt = 1;
   measObjList->list.count = elementCnt;
   measObjList->list.size = measObjList->list.count * sizeof(MeasObjectToAddMod_t *);

   CU_ALLOC(measObjList->list.array, measObjList->list.size);
   if(!measObjList->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for measurement object list's array");
      return RFAILED;
   }

   for(objIdx = 0; objIdx < measObjList->list.size; objIdx++)
   {
      CU_ALLOC(measObjList->list.array[objIdx], sizeof(MeasObjectToAddMod_t));
      if(!measObjList->list.array[objIdx])
      {
         DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for measurement object list's array element");
         return RFAILED;
      }
   }

   objIdx = 0;
   measObjList->list.array[objIdx]->measObjectId = 1;
   measObjList->list.array[objIdx]->measObject.present = MeasObjectToAddMod__measObject_PR_measObjectNR;
   CU_ALLOC(measObjList->list.array[objIdx]->measObject.choice.measObjectNR, sizeof(MeasObjectNR_t));
   if(!measObjList->list.array[objIdx]->measObject.choice.measObjectNR)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for mesurement object NR");
      return RFAILED;
   }

   measObject = measObjList->list.array[objIdx]->measObject.choice.measObjectNR;

   /* SSB frequency */
   CU_ALLOC(measObject->ssbFrequency, sizeof(ARFCN_ValueNR_t));
   if(!measObject->ssbFrequency)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for SSB frequency in fillMeasObjToAddModList"); 
      return RFAILED;
   }
   *(measObject->ssbFrequency) = 620736; /* Value picked from reference PCAP logs */

   /* Subcarrier spacing */
   CU_ALLOC(measObject->ssbSubcarrierSpacing, sizeof(SubcarrierSpacing_t));
   if(!measObject->ssbSubcarrierSpacing)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for SSB subcarrier spacing in fillMeasObjToAddModList");
      return RFAILED;
   }
   *(measObject->ssbSubcarrierSpacing) = SubcarrierSpacing_kHz15;

   /* SMTC1 */
   CU_ALLOC(measObject->smtc1 , sizeof(SSB_MTC_t));
   if(!measObject->smtc1)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for SMTC1 in fillMeasObjToAddModList");
      return RFAILED;
   }
   measObject->smtc1->duration = SSB_MTC__duration_sf1;
   measObject->smtc1->periodicityAndOffset.present = SSB_MTC__periodicityAndOffset_PR_sf20;
   measObject->smtc1->periodicityAndOffset.choice.sf20 = 0;

   /* Absoulute threshold SSB consolidation */
   CU_ALLOC(measObject->absThreshSS_BlocksConsolidation, sizeof(ThresholdNR_t));
   if(!measObject->absThreshSS_BlocksConsolidation)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for absolute threshold SSB consolidation in fillMeasObjToAddModList");
      return RFAILED;
   }

   /* RSRP threshold */
   CU_ALLOC(measObject->absThreshSS_BlocksConsolidation->thresholdRSRP, sizeof(RSRP_Range_t));
   if(!measObject->absThreshSS_BlocksConsolidation->thresholdRSRP)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for threshold RSRP in fillMeasObjToAddModList");
      return RFAILED;
   }
   *(measObject->absThreshSS_BlocksConsolidation->thresholdRSRP) = 1;

   /* RSRQ threshold */
   CU_ALLOC(measObject->absThreshSS_BlocksConsolidation->thresholdRSRQ, sizeof(RSRQ_Range_t));
   if(!measObject->absThreshSS_BlocksConsolidation->thresholdRSRQ)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for threshold RSRQ in fillMeasObjToAddModList");
      return RFAILED;
   }
   *(measObject->absThreshSS_BlocksConsolidation->thresholdRSRQ) = 1;

   /* SINR threshold */
   CU_ALLOC(measObject->absThreshSS_BlocksConsolidation->thresholdSINR, sizeof(SINR_Range_t));
   if(!measObject->absThreshSS_BlocksConsolidation->thresholdSINR)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for threshold SINR in fillMeasObjToAddModList");
      return RFAILED;
   }
   *(measObject->absThreshSS_BlocksConsolidation->thresholdSINR) = 1;

   /* Number of SSBs to average */
   CU_ALLOC(measObject->nrofSS_BlocksToAverage, sizeof(long));
   if(!measObject->nrofSS_BlocksToAverage)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for number of SSB to average in fillMeasObjToAddModList");
      return RFAILED;
   }
   *(measObject->nrofSS_BlocksToAverage) = 2;

   /* Quantity Config index */
   measObject->quantityConfigIndex = 1;

   /* Offset MO */
   /* RSRP offset for SSB */
   CU_ALLOC(measObject->offsetMO.rsrpOffsetSSB, sizeof(Q_OffsetRange_t));
   if(!measObject->offsetMO.rsrpOffsetSSB)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for SSB RSRP offset in fillMeasObjToAddModList");
      return RFAILED;
   }
   *(measObject->offsetMO.rsrpOffsetSSB) = Q_OffsetRange_dB0;

   /* RSRQ offset for SSB */
   CU_ALLOC(measObject->offsetMO.rsrqOffsetSSB, sizeof(Q_OffsetRange_t));
   if(!measObject->offsetMO.rsrqOffsetSSB)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for SSB RSRQ offset in fillMeasObjToAddModList");
      return RFAILED;
   }
   *(measObject->offsetMO.rsrqOffsetSSB) = Q_OffsetRange_dB0;

   /* SINR offset for SSB */
   CU_ALLOC(measObject->offsetMO.sinrOffsetSSB, sizeof(Q_OffsetRange_t));
   if(!measObject->offsetMO.sinrOffsetSSB)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for SSB SINR offset in fillMeasObjToAddModList");
      return RFAILED;
   }
   *(measObject->offsetMO.sinrOffsetSSB) = Q_OffsetRange_dB0;

   return ROK;
}

/*******************************************************************
 *
 * @brief Fill Report configuration to Add/mod list
 *
 * @details
 *
 *    Function : fillReportCfgToAddModList
 *
 *    Functionality: Fill Report configuration to Add/mod list
 *
 * @params[in] Report Config To Add/Mod List
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillReportCfgToAddModList(ReportConfigToAddModList_t *reportCfgList)
{
   uint8_t elementCnt;
   uint8_t reportCfgIdx;
   ReportConfigToAddMod_t *reportCfg;
   ReportConfigNR_t *reportCfgNr;
   EventTriggerConfig_t *eventTriggCfg;

   elementCnt = 1;
   reportCfgList->list.count = elementCnt;
   reportCfgList->list.size = reportCfgList->list.count * sizeof(ReportConfigToAddMod_t *);

   CU_ALLOC(reportCfgList->list.array, reportCfgList->list.size);
   if(!reportCfgList->list.array)
   {
      DU_LOG("\nERROR  -->  F1AP: fillReportCfgToAddModList() :Memory allocation failed for report config list's array");
      return RFAILED;
   }

   for(reportCfgIdx=0; reportCfgIdx < reportCfgList->list.count; reportCfgIdx++)
   {
      CU_ALLOC(reportCfgList->list.array[reportCfgIdx], sizeof(ReportConfigToAddMod_t));
      if(!reportCfgList->list.array[reportCfgIdx])
      {
         DU_LOG("\nERROR  -->  F1AP: fillReportCfgToAddModList(): Memory allocation failed for report config list's array element");
         return RFAILED;
      }
   }

   reportCfgIdx = 0;
   reportCfg = reportCfgList->list.array[reportCfgIdx];
   reportCfg->reportConfigId = 1;
   reportCfg->reportConfig.present = ReportConfigToAddMod__reportConfig_PR_reportConfigNR;

   /* Report Configuration for NR */
   CU_ALLOC(reportCfg->reportConfig.choice.reportConfigNR, sizeof(ReportConfigNR_t));
   if(!reportCfg->reportConfig.choice.reportConfigNR)
   {
      DU_LOG("\nERROR  -->  F1AP: fillReportCfgToAddModList(): Memory allocation failed for report config NR");
      return RFAILED;
   }
   reportCfgNr = reportCfg->reportConfig.choice.reportConfigNR;

   /* Report Type */
   reportCfgNr->reportType.present = ReportConfigNR__reportType_PR_eventTriggered;
   CU_ALLOC(reportCfgNr->reportType.choice.eventTriggered, sizeof(EventTriggerConfig_t));
   if(!reportCfgNr->reportType.choice.eventTriggered)
   {
      DU_LOG("\nERROR  -->  F1AP: fillReportCfgToAddModList(): Memory allocation failed for event triggerred");
      return RFAILED;
   }
   eventTriggCfg = reportCfgNr->reportType.choice.eventTriggered;

   /* Event 3 */
   eventTriggCfg->eventId.present = EventTriggerConfig__eventId_PR_eventA3;
   CU_ALLOC(eventTriggCfg->eventId.choice.eventA3, sizeof(struct EventTriggerConfig__eventId__eventA3));
   if(!eventTriggCfg->eventId.choice.eventA3)
   {
      DU_LOG("\nERROR  -->  F1AP: fillReportCfgToAddModList(): Memory allocation failed for event A3");
      return RFAILED;
   }

   eventTriggCfg->eventId.choice.eventA3->a3_Offset.present = MeasTriggerQuantityOffset_PR_rsrp;
   eventTriggCfg->eventId.choice.eventA3->a3_Offset.choice.rsrp = 6; /* i.e. 3.0db . From Reference pcap log */
   eventTriggCfg->eventId.choice.eventA3->reportOnLeave = false;
   eventTriggCfg->eventId.choice.eventA3->hysteresis = 0;
   eventTriggCfg->eventId.choice.eventA3->timeToTrigger = TimeToTrigger_ms40;
   eventTriggCfg->eventId.choice.eventA3->useWhiteCellList = false;

   /* Reference Signal Type */
   eventTriggCfg->rsType = NR_RS_Type_ssb;

   /* Report Interval */
   eventTriggCfg->reportInterval = ReportInterval_ms1024;

   /* Report Amount */
   eventTriggCfg->reportAmount = EventTriggerConfig__reportAmount_r8;

   /* Report Quantity cell */
   eventTriggCfg->reportQuantityCell.rsrp = true;
   eventTriggCfg->reportQuantityCell.rsrq = false;
   eventTriggCfg->reportQuantityCell.sinr = false;

   /* Maximum reported cells */
   eventTriggCfg->maxReportCells = 3;

   /* Report qunatity RS Indexes */
   CU_ALLOC(eventTriggCfg->reportQuantityRS_Indexes, sizeof(MeasReportQuantity_t));
   if(!eventTriggCfg->reportQuantityRS_Indexes)
   {
      DU_LOG("\nERROR  -->  F1AP: fillReportCfgToAddModList(): Memory allocation failed for report qunatity RS indexes");
      return RFAILED;
   }
   eventTriggCfg->reportQuantityRS_Indexes->rsrp = true;
   eventTriggCfg->reportQuantityRS_Indexes->rsrq = false;
   eventTriggCfg->reportQuantityRS_Indexes->sinr = false;

   /* Maximum number of RS indexes to report */
   CU_ALLOC(eventTriggCfg->maxNrofRS_IndexesToReport, sizeof(long));
   if(!eventTriggCfg->maxNrofRS_IndexesToReport)
   {
      DU_LOG("\nERROR  -->  F1AP: fillReportCfgToAddModList(): Memory allocation failed for max number of RS indexes to report");
      return RFAILED;
   }
   *(eventTriggCfg->maxNrofRS_IndexesToReport) = 1;

   /* Include Beam measurement */
   eventTriggCfg->includeBeamMeasurements = false;

   return ROK;
}

/*******************************************************************
 *
 * @brief Fill measurement Id to add/mod list
 
 * @details
 *
 *    Function : fillMeasIdToAddModList
 *
 *    Functionality: Fill measurement Id to add/mod list
 *
 * @params[in] Measurement Id to add/mod list
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillMeasIdToAddModList(MeasIdToAddModList_t *measIdList)
{
   uint8_t elementCnt;
   uint8_t measIdIdx;

   elementCnt = 1;
   measIdList->list.count = elementCnt;
   measIdList->list.size = measIdList->list.count *sizeof(MeasIdToAddMod_t *);

   CU_ALLOC(measIdList->list.array, measIdList->list.size);
   if(!measIdList->list.array)
   {
      return RFAILED;
   }

   for(measIdIdx=0; measIdIdx < measIdList->list.count; measIdIdx++)
   {
      CU_ALLOC(measIdList->list.array[measIdIdx], sizeof(MeasIdToAddMod_t));
      if(!measIdList->list.array[measIdIdx])
      {
         return RFAILED;
      }

      measIdIdx=0;
      measIdList->list.array[measIdIdx]->measId = 1;
      measIdList->list.array[measIdIdx]->measObjectId = 1;
      measIdList->list.array[measIdIdx]->reportConfigId = 1;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill s-measurement configuration
 *
 * @details
 *
 *    Function : fillSMeasConfig
 *
 *    Functionality: Fill s-measurement configuration
 *
 * @params[in] s-Measurement config
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillSMeasConfig(struct MeasConfigRrc__s_MeasureConfig *sMeasCfg)
{
   sMeasCfg->present = MeasConfigRrc__s_MeasureConfig_PR_ssb_RSRP;
   sMeasCfg->choice.ssb_RSRP = 100; /* Value taken from reference PCAP log */

   return ROK;
}

/*******************************************************************
 *
 * @brief Fill quantity config
 *
 * @details
 *
 *    Function : fillQunatityConfig
 *
 *    Functionality: Fill quantity config
 *
 * @params[in] Quantity Config
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillQuantityConfig(QuantityConfig_t *quantityCfg)
{
   uint8_t elementCnt = 0;
   uint8_t quanCfgIdx = 0;
   QuantityConfigNR_t *quantityCfgNr;

   CU_ALLOC(quantityCfg->quantityConfigNR_List, sizeof(struct QuantityConfig__quantityConfigNR_List));
   if(!quantityCfg->quantityConfigNR_List)
   {
      return RFAILED;
   }

   elementCnt = 1;
   quantityCfg->quantityConfigNR_List->list.count = elementCnt;
   quantityCfg->quantityConfigNR_List->list.size = elementCnt * sizeof(QuantityConfigNR_t *);

   CU_ALLOC(quantityCfg->quantityConfigNR_List->list.array, quantityCfg->quantityConfigNR_List->list.size);
   if(!quantityCfg->quantityConfigNR_List->list.array)
   {
      return RFAILED;
   }

   for(quanCfgIdx = 0; quanCfgIdx < quantityCfg->quantityConfigNR_List->list.count; quanCfgIdx++)
   {
      CU_ALLOC(quantityCfg->quantityConfigNR_List->list.array[quanCfgIdx], sizeof(QuantityConfigNR_t));
      if(!quantityCfg->quantityConfigNR_List->list.array[quanCfgIdx])
      {
         return RFAILED;
      }
   }

   quanCfgIdx = 0;
   quantityCfgNr = quantityCfg->quantityConfigNR_List->list.array[quanCfgIdx];

   /* Quantity Config of Reference signal */
   CU_ALLOC(quantityCfgNr->quantityConfigCell.ssb_FilterConfig.filterCoefficientRSRP, sizeof(FilterCoefficient_t));
   if(!quantityCfgNr->quantityConfigCell.ssb_FilterConfig.filterCoefficientRSRP)
   {
      return RFAILED;
   }
   *(quantityCfgNr->quantityConfigCell.ssb_FilterConfig.filterCoefficientRSRP) = FilterCoefficient_fc4;

   CU_ALLOC(quantityCfgNr->quantityConfigCell.ssb_FilterConfig.filterCoefficientRSRQ, sizeof(FilterCoefficient_t));
   if(!quantityCfgNr->quantityConfigCell.ssb_FilterConfig.filterCoefficientRSRQ)
   {
      return RFAILED;
   }
   *(quantityCfgNr->quantityConfigCell.ssb_FilterConfig.filterCoefficientRSRQ) = FilterCoefficient_fc4;

   CU_ALLOC(quantityCfgNr->quantityConfigCell.ssb_FilterConfig.filterCoefficientRS_SINR, sizeof(FilterCoefficient_t));
   if(!quantityCfgNr->quantityConfigCell.ssb_FilterConfig.filterCoefficientRS_SINR)
   {
      return RFAILED;
   }
   *(quantityCfgNr->quantityConfigCell.ssb_FilterConfig.filterCoefficientRS_SINR) = FilterCoefficient_fc4;

   /* Quantity Config RS index */
   CU_ALLOC(quantityCfgNr->quantityConfigRS_Index, sizeof(QuantityConfigRS_t));
   if(!quantityCfgNr->quantityConfigRS_Index)
   {
      return RFAILED;
   }

   CU_ALLOC(quantityCfgNr->quantityConfigRS_Index->ssb_FilterConfig.filterCoefficientRSRP, sizeof(FilterCoefficient_t));
   if(!quantityCfgNr->quantityConfigRS_Index->ssb_FilterConfig.filterCoefficientRSRP)
   {
      return RFAILED;
   }
   *(quantityCfgNr->quantityConfigRS_Index->ssb_FilterConfig.filterCoefficientRSRP) = FilterCoefficient_fc4;

   CU_ALLOC(quantityCfgNr->quantityConfigRS_Index->ssb_FilterConfig.filterCoefficientRSRQ, sizeof(FilterCoefficient_t));
   if(!quantityCfgNr->quantityConfigRS_Index->ssb_FilterConfig.filterCoefficientRSRQ)
   {
      return RFAILED;
   }
   *(quantityCfgNr->quantityConfigRS_Index->ssb_FilterConfig.filterCoefficientRSRQ) = FilterCoefficient_fc4;

   CU_ALLOC(quantityCfgNr->quantityConfigRS_Index->ssb_FilterConfig.filterCoefficientRS_SINR, sizeof(FilterCoefficient_t));
   if(!quantityCfgNr->quantityConfigRS_Index->ssb_FilterConfig.filterCoefficientRS_SINR)
   {
      return RFAILED;
   }
   *(quantityCfgNr->quantityConfigRS_Index->ssb_FilterConfig.filterCoefficientRS_SINR) = FilterCoefficient_fc4;

   return ROK;
}

/*******************************************************************
 *
 * @brief Fill measurement configuration
 *
 * @details
 *
 *    Function : fillMeasConfig
 *
 *    Functionality: Fill measurement configuration
 *
 * @params[in] Measurement config
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillMeasConfig(MeasConfigRrc_t *measConfig)
{
   /* Measurement object to add/mod list */
   CU_ALLOC(measConfig->measObjectToAddModList, sizeof(MeasObjectToAddModList_t));
   if(!measConfig->measObjectToAddModList)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for measurement object list in fillMeasConfig");
      return RFAILED;
   }
   if(fillMeasObjToAddModList(measConfig->measObjectToAddModList) != ROK)
   {   
      DU_LOG("\nERROR  -->  F1AP: Failure in fillMeasObjToAddModList");
      return RFAILED;
   }

   /* Report Config To add/mod list */
   CU_ALLOC(measConfig->reportConfigToAddModList, sizeof(ReportConfigToAddModList_t));
   if(!measConfig->reportConfigToAddModList)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for report config list in  fillMeasConfig");
      return RFAILED;
   }
   if(fillReportCfgToAddModList(measConfig->reportConfigToAddModList) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP: Failure in fillReportCfgToAddModList");
      return RFAILED;
   }

   /* Measurement Id to add/mod list */
   CU_ALLOC(measConfig->measIdToAddModList, sizeof(MeasIdToAddModList_t));
   if(!measConfig->measIdToAddModList)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for measuerment id list in  fillMeasConfig");
      return RFAILED;
   }
   if(fillMeasIdToAddModList(measConfig->measIdToAddModList) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP: Failure in fillMeasIdToAddModList");
      return RFAILED;
   }

   /* S-Measurement config */
   CU_ALLOC(measConfig->s_MeasureConfig, sizeof(struct MeasConfigRrc__s_MeasureConfig));
   if(!measConfig->s_MeasureConfig)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for s measuerment config in  fillMeasConfig");
      return RFAILED;
   }
   if(fillSMeasConfig(measConfig->s_MeasureConfig) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP: Failure in fillSMeasConfig");
      return RFAILED;
   }

   /* Qunatity Config */
   CU_ALLOC(measConfig->quantityConfig, sizeof(QuantityConfig_t));
   if(!measConfig->quantityConfig)
   {
      DU_LOG("\nERROR  -->  F1AP: Memory allocation failed for quantity config in  fillMeasConfig");
      return RFAILED;
   }
   if(fillQuantityConfig(measConfig->quantityConfig) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP: Failure in fillQuantityConfig");
      return RFAILED;
   }

return ROK;
}

/*******************************************************************
 *
 * @brief Fill RRC reconfiguration non-critical extension IE
 *
 * @details
 *
 *    Function : fillRrcReconfigNonCriticalExt
 *
 *    Functionality: Fill RRC reconfiguration non-critical extension
 *
 * @params[in] RRC Reconfig Non-critical extension
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillRrcReconfigNonCriticalExt(CuUeCb *ueCb, RRCReconfiguration_v1530_IEs_t *rrcRecfg, bool updateAllRbCfg)
{
   CU_ALLOC(rrcRecfg->masterCellGroup, sizeof(OCTET_STRING_t));
   if(!rrcRecfg->masterCellGroup)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in fillRrcReconfigNonCriticalExt");
      return RFAILED;
   }

   rrcRecfg->masterCellGroup->size = ueCb->f1apMsgDb.duToCuContainer.size;
   CU_ALLOC(rrcRecfg->masterCellGroup->buf, rrcRecfg->masterCellGroup->size);
   if(!rrcRecfg->masterCellGroup->buf)
   {     
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in fillRrcReconfigNonCriticalExt");
      return RFAILED;
   }     
   memcpy(rrcRecfg->masterCellGroup->buf, ueCb->f1apMsgDb.duToCuContainer.buf, rrcRecfg->masterCellGroup->size);

#if 0
   /* Use below code if masterCEllGroup is to be filled explicitly at CU rather than copying from DUToCURRCContainer 
    * received from DU */
   if(fillCellGrpCfg(ueCb, rrcRecfg->masterCellGroup, updateAllRbCfg) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : Failed to fill CellGroupCfg in fillRrcReconfigNonCriticalExt");
      return RFAILED;
   }
#endif

   return ROK;
}

/*******************************************************************
 *
 * @brief Fill RRC reconfiguration structure
 *
 * @details
 *
 *    Function : fillRrcReconfig
 *
 *    Functionality: Fill RRC reconfiguration
 
 *
 * @params[in] UE Cb
 *             RRC reconfiguration structure
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillRrcReconfig(CuUeCb *ueCb, RRCReconfiguration_t *rrcReconfig, bool updateAllRbCfg)
{
   memset(rrcReconfig, 0, sizeof(RRCReconfiguration_t));

   rrcReconfig->rrc_TransactionIdentifier = 1;
   rrcReconfig->criticalExtensions.present = RRCReconfiguration__criticalExtensions_PR_rrcReconfiguration;

   CU_ALLOC(rrcReconfig->criticalExtensions.choice.rrcReconfiguration, sizeof(RRCReconfiguration_IEs_t)); 
   if(!rrcReconfig->criticalExtensions.choice.rrcReconfiguration)
   {
      DU_LOG("\nERROR  -->  F1AP : Memormy allocation failed for RRC reconfiguration IE in fillRrcReconfig");
      return RFAILED;
   }

   /* Radio Bearer Configuration */
   CU_ALLOC(rrcReconfig->criticalExtensions.choice.rrcReconfiguration->radioBearerConfig, sizeof(RadioBearerConfig_t));
   if(!rrcReconfig->criticalExtensions.choice.rrcReconfiguration->radioBearerConfig)
   {
      DU_LOG("\nERROR  -->  F1AP : Memormy allocation failed for radio bearer config in fillRrcReconfig");
      return RFAILED;
   }
   if(fillRadioBearerConfig(ueCb, rrcReconfig->criticalExtensions.choice.rrcReconfiguration->radioBearerConfig, updateAllRbCfg) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : Failed to fill radio bearer config in fillRrcReconfig");
      return RFAILED;
   }

   /* Measurement Configuration */
   CU_ALLOC(rrcReconfig->criticalExtensions.choice.rrcReconfiguration->measConfig, sizeof(MeasConfigRrc_t));
   if(!rrcReconfig->criticalExtensions.choice.rrcReconfiguration->measConfig)
   {
      DU_LOG("\nERROR  -->  F1AP : Memormy allocation failed for Measurement config IE in fillRrcReconfig");
      return RFAILED;
   }
   if(fillMeasConfig(rrcReconfig->criticalExtensions.choice.rrcReconfiguration->measConfig) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : Failed to fill measurement config in fillRrcReconfig");
      return RFAILED;
   }

   /* Non Critical extension */
   CU_ALLOC(rrcReconfig->criticalExtensions.choice.rrcReconfiguration->nonCriticalExtension, sizeof(struct RRCReconfiguration_v1530_IEs));
   if(!rrcReconfig->criticalExtensions.choice.rrcReconfiguration->nonCriticalExtension)
   {
      DU_LOG("\nERROR  -->  F1AP : Memormy allocation failed for non critical extension IE in fillRrcReconfig");
      return RFAILED;
   }
   if(fillRrcReconfigNonCriticalExt(ueCb, rrcReconfig->criticalExtensions.choice.rrcReconfiguration->nonCriticalExtension, updateAllRbCfg) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : Failed to fill non critical extension in fillRrcReconfig");
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill RRC reconfiguration Octet string
 *
 * @details
 *
 *    Function : fillRrcReconfigBuf
 *
 *    Functionality: Fill RRC reconfiguration octet string
 
 *
 * @params[in] OCTET_STRING_t buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillRrcReconfigBuf(CuUeCb *ueCb, OCTET_STRING_t  *rrcReconfigBuf, bool updateAllRbCfg)
{
   uint8_t          ret = RFAILED;
   asn_enc_rval_t   encRetVal;
   RRCReconfiguration_t rrcRecfg, *rrcReconfig = NULLP;
   rrcReconfig = &rrcRecfg;

   while(true)
   {
      if(fillRrcReconfig(ueCb, rrcReconfig, updateAllRbCfg) != ROK)
      {
         DU_LOG( "\nERROR  -->  F1AP : Failed to fill RRC Reconfiguration in fillRrcReconfigBuf");
         break; 
      }

      /* Encode RRC Reconfiguration */
      xer_fprint(stdout, &asn_DEF_RRCReconfiguration, rrcReconfig);
      cmMemset((uint8_t *)encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = uper_encode(&asn_DEF_RRCReconfiguration, 0, rrcReconfig, PrepFinalEncBuf, encBuf);

      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {     
         DU_LOG( "\nERROR  -->  F1AP : Could not encode RRC Reconfiguration (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }     
      else  
      {     
         DU_LOG("\nDEBUG  -->  F1AP : Created APER encoded buffer for RRC Reconfiguration\n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         }
      }     

      rrcReconfigBuf->size = encBufSize;
      CU_ALLOC(rrcReconfigBuf->buf, rrcReconfigBuf->size);
      if(!rrcReconfigBuf->buf)
      {     
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in fillRrcReconfigBuf");
         break;
      }     
      memcpy(rrcReconfigBuf->buf, encBuf, rrcReconfigBuf->size);
      ret = ROK;
      break;
   }

   freeRrcReconfig(rrcReconfig);
   return ret;
}

/*******************************************************************
 *
 * @brief Fill HO preparation information Octet string
 *
 * @details
 *
 *    Function : fillHOPreparationInfoBuf
 *
 *    Functionality: Fill HO preparation information Octet string
 
 *
 * @params[in] HandoverPreparationInformation_t buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillHOPreparationInfoBuf(CuUeCb *ueCb, HandoverPreparationInformation_t *hoPrepInfoBuf)
{
   uint8_t          ret = RFAILED;
   asn_enc_rval_t   encRetVal;
   HandoverPreparationInformationRrc_t hoPrepInfo;
   HandoverPreparationInformationRrc_IEs_t *hoPrepInfoIe;

   while(true)
   {
   
      hoPrepInfo.criticalExtensions.present = HandoverPreparationInformationRrc__criticalExtensions_PR_c1;
      CU_ALLOC(hoPrepInfo.criticalExtensions.choice.c1, sizeof(struct HandoverPreparationInformationRrc__criticalExtensions__c1));
      if(!hoPrepInfo.criticalExtensions.choice.c1)
      {
         DU_LOG( "\nERROR  -->  F1AP : Failed to allocate memory for c1 in fillHOPreparationInfo");
         break;
      }
      hoPrepInfo.criticalExtensions.choice.c1->present = \
         HandoverPreparationInformationRrc__criticalExtensions__c1_PR_handoverPreparationInformation;
      
      CU_ALLOC(hoPrepInfo.criticalExtensions.choice.c1->choice.handoverPreparationInformation , \
         sizeof(HandoverPreparationInformationRrc_IEs_t));
      if(!hoPrepInfo.criticalExtensions.choice.c1->choice.handoverPreparationInformation)
      {
         DU_LOG( "\nERROR  -->  F1AP : Failed to allocate memory for handover preparation information IE in fillHOPreparationInfo");
         break;
      }
      hoPrepInfoIe = hoPrepInfo.criticalExtensions.choice.c1->choice.handoverPreparationInformation;
   
      /* Fill UE Capability RAT container list */
      ret = fillUeCapRatContList(&hoPrepInfoIe->ue_CapabilityRAT_List);
      if(ret != ROK)
      {
         DU_LOG( "\nERROR  -->  F1AP : Failed to fill UE Capability RAT container list");
         break;
      }

      /* Fill Source config */
      hoPrepInfoIe->sourceConfig = NULLP;
      CU_ALLOC(hoPrepInfoIe->sourceConfig, sizeof(AS_Config_t));
      if(!hoPrepInfoIe->sourceConfig)
      {
         DU_LOG( "\nERROR  -->  F1AP : Failed to allocate memory for source config in fillHOPreparationInfo");
         return RFAILED;
      }
      ret = fillRrcReconfigBuf(ueCb, &hoPrepInfoIe->sourceConfig->rrcReconfiguration, true); 
      
      if(ret != ROK)
      {
         DU_LOG( "\nERROR  -->  F1AP : Failed to fill Rrc reconfiguration buffer");
         return RFAILED;
      }

      hoPrepInfoIe->rrm_Config = NULLP;
      hoPrepInfoIe->as_Context = NULLP;
      hoPrepInfoIe->nonCriticalExtension = NULLP;

      /* encode UE Capability RAT Container List into duToCuRrcContainer */
      xer_fprint(stdout, &asn_DEF_HandoverPreparationInformationRrc, &hoPrepInfo);
      cmMemset((uint8_t *)encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = uper_encode(&asn_DEF_HandoverPreparationInformationRrc, 0, \
            &hoPrepInfo, PrepFinalEncBuf, encBuf);

      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG( "\nERROR  -->  F1AP : Could not encode UE Capability RAT Container (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  F1AP : Created APER encodedbuffer for UE Capability RAT Container\n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         }
      }

      hoPrepInfoBuf->size = encBufSize;
      CU_ALLOC(hoPrepInfoBuf->buf, hoPrepInfoBuf->size);
      if(!hoPrepInfoBuf->buf)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in fillUeCapabilityContainer");
         break;
      }
      memcpy(hoPrepInfoBuf->buf, encBuf, hoPrepInfoBuf->size);
      ret = ROK;
      break;
   }
   freeHOPreparationInfo(&hoPrepInfo);
   return ret;
}

/*******************************************************************
 *
 * @brief Fills CuToDuContainer 
 *
 * @details
 *
 *    Function : fillCuToDuContainer
 *
 *    Functionality: Fills CuToDuContainer
 *
 * @params[in] pointer to CUtoDURRCInformation_t
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t fillCuToDuContainer(CuUeCb *ueCb, CUtoDURRCInformation_t *rrcMsg)
{
   uint8_t elementCnt = 0;
   uint8_t ret = ROK;
   uint8_t idx;

   if((ueCb->state != UE_HANDOVER_IN_PROGRESS) || ((ueCb->state == UE_HANDOVER_IN_PROGRESS) && (ueCb->hoInfo.HOType == Inter_DU_HO)))
   {
      /* UE Capabulity RAT Container List */
      CU_ALLOC(rrcMsg->uE_CapabilityRAT_ContainerList, sizeof(UE_CapabilityRAT_ContainerList_t));
      if(!rrcMsg->uE_CapabilityRAT_ContainerList)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation for UE capability RAT container list failed");
         return RFAILED;
      }
      ret = fillUeCapRatContListBuf(rrcMsg->uE_CapabilityRAT_ContainerList);
   }

#if 0

   /* Commenting this because:
    * CUToDURRCInformation->MeasConfig contains measurement gap configurations.
    * Howeever measurement gap is not supported in our code. Measurement Gap will
    * be required if we want to support inter-RAT handover or handover to
    * neighbouring cells operating on a different frequency than serving cell.
    *
    * In case we plan to use this IE in future, following fixes are required:
    * As of now, we are filling MeasurementTimingConfigurationRrc_t into rrcMsg->measConfig.
    * This is incorrect. We must fill MeasConfigRrc_t in rrcMsg->measConfig.
    * MeasurementTimingConfigurationRrc_t should be filled in 
    * rrcMsg->iE_Extensions->MeasurementTimingConfiguration, if required.
    */

   CU_ALLOC(rrcMsg->measConfig, sizeof(MeasConfig_t));
   if(!rrcMsg->measConfig)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation for measurement configuration failed");
      return RFAILED;
   }
   ret = fillMeasTimingConfigBuf(rrcMsg->measConfig);
#endif

   if(ueCb->state == UE_HANDOVER_IN_PROGRESS)
   {
      /* IE extensions */
      CU_ALLOC(rrcMsg->iE_Extensions, sizeof(ProtocolExtensionContainer_4624P16_t));
      if(rrcMsg->iE_Extensions)
      {
         elementCnt = 1;
         rrcMsg->iE_Extensions->list.count = elementCnt;
         rrcMsg->iE_Extensions->list.size = elementCnt * sizeof(CUtoDURRCInformation_ExtIEs_t *);

         /* Initialize the CUtoDURRCInformation_ExtIEs */
         CU_ALLOC(rrcMsg->iE_Extensions->list.array, rrcMsg->iE_Extensions->list.size);

         if(rrcMsg->iE_Extensions->list.array == NULLP)
         {
            DU_LOG("\nERROR  -->  F1AP : Memory allocation for CUtoDURRCInformation_ExtIEs failed");
            ret = RFAILED;
         }

         for(idx=0; idx<elementCnt; idx++)
         {
            CU_ALLOC(rrcMsg->iE_Extensions->list.array[idx], sizeof(CUtoDURRCInformation_ExtIEs_t));
            if(rrcMsg->iE_Extensions->list.array[idx] == NULLP)
            {
               DU_LOG("\nERROR  -->  F1AP : Memory allocation for array elements failed");
               ret = RFAILED;
            }
         }

         idx = 0;
#if 0
         /* Cell Group Configuration */
         rrcMsg->iE_Extensions->list.array[idx]->id = ProtocolIE_ID_id_CellGroupConfig;
         rrcMsg->iE_Extensions->list.array[idx]->criticality = Criticality_ignore; 
         rrcMsg->iE_Extensions->list.array[idx]->extensionValue.present  =\
                                                                          CUtoDURRCInformation_ExtIEs__extensionValue_PR_CellGroupConfig;
         ret = fillCellGrpCfg(ueCb, &rrcMsg->iE_Extensions->list.array[idx]->extensionValue.choice.CellGroupConfig, true);
         idx++;
#endif
         /* Handover Preparation Information */
         rrcMsg->iE_Extensions->list.array[idx]->id = ProtocolIE_ID_id_HandoverPreparationInformation; 
         rrcMsg->iE_Extensions->list.array[idx]->criticality = Criticality_ignore;
         rrcMsg->iE_Extensions->list.array[idx]->extensionValue.present  = \
                                                                           CUtoDURRCInformation_ExtIEs__extensionValue_PR_HandoverPreparationInformation;
         ret = fillHOPreparationInfoBuf(ueCb, &rrcMsg->iE_Extensions->list.array[idx]->extensionValue.choice.HandoverPreparationInformation);
      }
   }
   return ret;
}
/*******************************************************************
 *
 * @brief Build the drx cycle  
 *
 * @details
 *
 *    Function : BuildDrxCycle
 *
 *    Functionality: Build drx cycle IE
 *
 * @params[in] pointer to DRXCycle_t
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t BuildDrxCycle(DRXCycle_t *drxCycle)
{
   drxCycle->longDRXCycleLength = LongDRXCycleLength_ms80;
   CU_ALLOC(drxCycle->shortDRXCycleLength, sizeof(ShortDRXCycleLength_t));
   if(!drxCycle->shortDRXCycleLength)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed for shortDRXCycleLength");
      return RFAILED;
   }
   *(drxCycle->shortDRXCycleLength) = ShortDRXCycleLength_ms4;
   
   CU_ALLOC(drxCycle->shortDRXCycleTimer, sizeof(ShortDRXCycleTimer_t));
   if(!drxCycle->shortDRXCycleTimer)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed for shortDRXCycleTimer");
      return RFAILED;
   }
   *(drxCycle->shortDRXCycleTimer) = 4;
   return ROK;
}
/*******************************************************************
 *
 * @brief Free CuToDuContainer 
 *
 * @details
 *
 *    Function : FreeCuToDuInfo
 *
 *    Functionality: Free CuToDuContainer
 *
 * @params[in] pointer to CUtoDURRCInformation_t
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

void FreeCuToDuInfo(CUtoDURRCInformation_t *rrcMsg)
{
   uint8_t idx, idx2;

   if(rrcMsg->uE_CapabilityRAT_ContainerList)
   {
      if(rrcMsg->uE_CapabilityRAT_ContainerList->buf)
         CU_FREE(rrcMsg->uE_CapabilityRAT_ContainerList->buf, rrcMsg->uE_CapabilityRAT_ContainerList->size);      
      CU_FREE(rrcMsg->uE_CapabilityRAT_ContainerList, sizeof(UE_CapabilityRAT_ContainerList_t));
   }
   if(rrcMsg->iE_Extensions)
   {
      if(rrcMsg->iE_Extensions->list.array)
      {
	 for(idx= 0; idx < rrcMsg->iE_Extensions->list.count; idx++)
	 {
	    if(rrcMsg->iE_Extensions->list.array[idx])
	    {
	       switch(rrcMsg->iE_Extensions->list.array[idx]->id)
	       {
		  case ProtocolIE_ID_id_CellGroupConfig:
		     if(rrcMsg->iE_Extensions->list.array[idx]->extensionValue.choice.CellGroupConfig.buf != NULLP)
		     {
			CU_FREE(rrcMsg->iE_Extensions->list.array[idx]->extensionValue.choice.CellGroupConfig.buf,\
			      rrcMsg->iE_Extensions->list.array[idx]->extensionValue.choice.CellGroupConfig.size);

		     }
		     break;
		  default:
		     DU_LOG("\nERROR  -->  F1AP : Invalid Event type %ld at FreeCuToDuInfo()", \
			   rrcMsg->iE_Extensions->list.array[idx]->id);
		     break;
	       }
	    }
	    break;
	 }
	 for(idx2 = 0; idx2 < idx; idx2++)
	 {
	    CU_FREE(rrcMsg->iE_Extensions->list.array[idx2], sizeof(CUtoDURRCInformation_ExtIEs_t));
	 }
	 CU_FREE(rrcMsg->iE_Extensions->list.array, rrcMsg->iE_Extensions->list.size);

      }

      CU_FREE(rrcMsg->iE_Extensions, sizeof(ProtocolExtensionContainer_4624P16_t));
   }
}
/*******************************************************************
 *
 * @brief Builds and sends the UE Setup Request 
 *
 * @details
 *
 *    Function : BuildAndSendUeContextSetupReq
 *
 *    Functionality: Constructs the UE Setup Request and sends
 *                   it to the CU through SCTP.
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendUeContextSetupReq(uint32_t duId, CuUeCb *ueCb)
{
   uint8_t   Nrcgiret, SplCellListret, SrbSetupret;
   uint8_t   ret= RFAILED, ret1;
   uint8_t   elementCnt;
   uint8_t   idx, idx1, bufLen, duIdx;
   uint32_t  spCellId;
   uint32_t  targetDuId;
   DuDb      *targetDuDb = NULLP;
   F1AP_PDU_t      	*f1apMsg = NULLP;
   UEContextSetupRequest_t *ueSetReq = NULLP;
   asn_enc_rval_t encRetVal;        /* Encoder return value */
   memset(&encRetVal, 0, sizeof(asn_enc_rval_t));

   while(true)
   {
      DU_LOG("\nINFO  -->  F1AP : Building UE Context Setup Request\n");

      CU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation for F1AP-PDU failed");
         break;
      }

      f1apMsg->present = F1AP_PDU_PR_initiatingMessage;
      CU_ALLOC(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      if(f1apMsg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation for	F1AP-PDU failed");
         break;
      }

      f1apMsg->choice.initiatingMessage->procedureCode = ProcedureCode_id_UEContextSetup;
      f1apMsg->choice.initiatingMessage->criticality = Criticality_reject;
      f1apMsg->choice.initiatingMessage->value.present = InitiatingMessage__value_PR_UEContextSetupRequest;

      ueSetReq = &f1apMsg->choice.initiatingMessage->value.choice.UEContextSetupRequest;

      if(ueCb->state == UE_HANDOVER_IN_PROGRESS)
         elementCnt = 7;
      else
      {
#ifdef NR_DRX
         elementCnt = 12;
#else
         elementCnt = 11;
#endif
      }
      ueSetReq->protocolIEs.list.count = elementCnt;
      ueSetReq->protocolIEs.list.size = elementCnt * sizeof(UEContextSetupRequestIEs_t *);

      /* Initialize the UESetup members */
      CU_ALLOC(ueSetReq->protocolIEs.list.array,ueSetReq->protocolIEs.list.size);

      if(ueSetReq->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation for UE Context SetupRequest failed");
         break;
      }

      for(idx1=0; idx1<elementCnt; idx1++)
      {
         CU_ALLOC(ueSetReq->protocolIEs.list.array[idx1],sizeof(UEContextSetupRequestIEs_t));
         if(ueSetReq->protocolIEs.list.array[idx1] == NULLP)
         {
            break;
         }
      }

      idx = 0;

      /*GNB CU UE F1AP ID*/
      ueSetReq->protocolIEs.list.array[idx]->id	= ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;
      ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
      ueSetReq->protocolIEs.list.array[idx]->value.present = UEContextSetupRequestIEs__value_PR_GNB_CU_UE_F1AP_ID;
      ueSetReq->protocolIEs.list.array[idx]->value.choice.GNB_CU_UE_F1AP_ID = ueCb->gnbCuUeF1apId;

      if(ueCb->state != UE_HANDOVER_IN_PROGRESS)
      {
         /*GNB DU UE F1AP ID*/
         idx++;
         ueSetReq->protocolIEs.list.array[idx]->id	= ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
         ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_ignore;
         ueSetReq->protocolIEs.list.array[idx]->value.present = UEContextSetupRequestIEs__value_PR_GNB_DU_UE_F1AP_ID;
         ueSetReq->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID = ueCb->gnbDuUeF1apId;
      }

      /*Special Cell ID*/
      idx++;
      ueSetReq->protocolIEs.list.array[idx]->id	= ProtocolIE_ID_id_SpCell_ID;
      ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
      ueSetReq->protocolIEs.list.array[idx]->value.present = UEContextSetupRequestIEs__value_PR_NRCGI;

      /* Spec 38.473 Sec 9.2.2.1 : For handover case, this IE shall be considered as target cell. */
      if(ueCb->state == UE_HANDOVER_IN_PROGRESS)
      {
         if(ueCb->hoInfo.HOType == Inter_DU_HO)
            targetDuId = ueCb->hoInfo.tgtNodeId;
         else
            targetDuId = duId;

         SEARCH_DU_DB(duIdx, targetDuId, targetDuDb);
         /* Since we are supporting only one cell per DU, accessing 0th index to
          * get target cell info */
         spCellId = targetDuDb->cellCb[0].nrCellId;
      }
      else
         spCellId = ueCb->cellCb->nrCellId;

      Nrcgiret = BuildNrcgi(&ueSetReq->protocolIEs.list.array[idx]->value.choice.NRCGI, spCellId);
      if(Nrcgiret != ROK)
      {
         break;
      }

      /*Served Cell Index*/
      idx++;
      ueSetReq->protocolIEs.list.array[idx]->id	= ProtocolIE_ID_id_ServCellIndex;
      ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
      ueSetReq->protocolIEs.list.array[idx]->value.present = UEContextSetupRequestIEs__value_PR_ServCellIndex;
      ueSetReq->protocolIEs.list.array[idx]->value.choice.ServCellIndex = CELL_INDEX;

      if(ueCb->state != UE_HANDOVER_IN_PROGRESS)
      {
         /*CellULConfigured*/
         idx++;
         ueSetReq->protocolIEs.list.array[idx]->id	= ProtocolIE_ID_id_SpCellULConfigured;
         ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_ignore;
         ueSetReq->protocolIEs.list.array[idx]->value.present = UEContextSetupRequestIEs__value_PR_CellULConfigured;
         ueSetReq->protocolIEs.list.array[idx]->value.choice.CellULConfigured = CellULConfigured_none;
      }

      /*CUtoDURRCContainer*/
      idx++;
      ueSetReq->protocolIEs.list.array[idx]->id	= ProtocolIE_ID_id_CUtoDURRCInformation;
      ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
      ueSetReq->protocolIEs.list.array[idx]->value.present = UEContextSetupRequestIEs__value_PR_CUtoDURRCInformation;
      if(fillCuToDuContainer(ueCb, &ueSetReq->protocolIEs.list.array[idx]->value.choice.CUtoDURRCInformation))
      {
         break;
      }

      if(ueCb->state != UE_HANDOVER_IN_PROGRESS)
      {
         /*Drx cycle*/
#ifdef NR_DRX
         idx++;
         ueSetReq->protocolIEs.list.array[idx]->id	= ProtocolIE_ID_id_DRXCycle;
         ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_ignore;
         ueSetReq->protocolIEs.list.array[idx]->value.present = UEContextSetupRequestIEs__value_PR_DRXCycle;
         if(BuildDrxCycle(&ueSetReq->protocolIEs.list.array[idx]->value.choice.DRXCycle) != ROK)
         {
            DU_LOG("\nERROR  -->  F1AP : Failed to build drx cycle");
            break;
         }
#endif         
         /*Special Cells to be SetupList*/
         idx++;
         ueSetReq->protocolIEs.list.array[idx]->id	= ProtocolIE_ID_id_SCell_ToBeSetup_List;
         ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_ignore;
         ueSetReq->protocolIEs.list.array[idx]->value.present = UEContextSetupRequestIEs__value_PR_SCell_ToBeSetup_List;
         SplCellListret = BuildSplCellList(ueCb, &ueSetReq->protocolIEs.list.array[idx]->value.choice.SCell_ToBeSetup_List);
         if(SplCellListret != ROK)
         {  
            break;
         }
      }

      /*SRBs To Be Setup List*/
      idx++;
      ueSetReq->protocolIEs.list.array[idx]->id	= ProtocolIE_ID_id_SRBs_ToBeSetup_List;
      ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
      ueSetReq->protocolIEs.list.array[idx]->value.present = UEContextSetupRequestIEs__value_PR_SRBs_ToBeSetup_List;
      SrbSetupret = BuildSRBSetup(ueCb, &ueSetReq->protocolIEs.list.array[idx]->value.choice.SRBs_ToBeSetup_List);
      if(SrbSetupret != ROK)
      {        
         break;
      }

      /*DRBs to Be Setup List*/
      idx++;
      ueSetReq->protocolIEs.list.array[idx]->id	= ProtocolIE_ID_id_DRBs_ToBeSetup_List;
      ueSetReq->protocolIEs.list.array[idx]->criticality	= 	Criticality_reject;
      ueSetReq->protocolIEs.list.array[idx]->value.present = UEContextSetupRequestIEs__value_PR_DRBs_ToBeSetup_List;
      ret1 = BuildDRBSetup(duId, ueCb, &ueSetReq->protocolIEs.list.array[idx]->value.choice.DRBs_ToBeSetup_List);
      if(ret1 != ROK)
      {	
         break;
      }

      if(ueCb->state != UE_HANDOVER_IN_PROGRESS)
      {
         /* RRC delivery status request */
         idx++;
         ueSetReq->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_RRCDeliveryStatusRequest;
         ueSetReq->protocolIEs.list.array[idx]->criticality = Criticality_ignore;
         ueSetReq->protocolIEs.list.array[idx]->value.present = UEContextSetupRequestIEs__value_PR_RRCDeliveryStatusRequest;
         ueSetReq->protocolIEs.list.array[idx]->value.choice.RRCDeliveryStatusRequest = RRCDeliveryStatusRequest_true;
      }

      /* GNB-DU UE Aggregate Maximum Bit Rate hardcoded as in reference logs */
      idx++;
      ueSetReq->protocolIEs.list.array[idx]->id = ProtocolIE_ID_id_GNB_DU_UE_AMBR_UL;
      ueSetReq->protocolIEs.list.array[idx]->criticality = Criticality_ignore;
      ueSetReq->protocolIEs.list.array[idx]->value.present = UEContextSetupRequestIEs__value_PR_BitRate;

      char bitRateBuf[4]= {0x3B, 0x37, 0xF4, 0xCD};
      bufLen = 4;
      ueSetReq->protocolIEs.list.array[idx]->value.choice.BitRate.size = bufLen;
      CU_ALLOC(ueSetReq->protocolIEs.list.array[idx]->value.choice.BitRate.buf,\
            ueSetReq->protocolIEs.list.array[idx]->value.choice.BitRate.size);
      if(!ueSetReq->protocolIEs.list.array[idx]->value.choice.BitRate.buf)
      {
         DU_LOG("\nERROR  -->  F1AP : Failed to allocate memory for Bit Rate in BuildAndSendUeContextSetupReq()");
         break;
      }
      memset(ueSetReq->protocolIEs.list.array[idx]->value.choice.BitRate.buf, 0, bufLen);
      memcpy(ueSetReq->protocolIEs.list.array[idx]->value.choice.BitRate.buf, bitRateBuf, bufLen);

      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the F1SetupRequest type as APER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf, encBuf);

      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG( "\nERROR  -->  F1AP : Could not encode UE Context Setup Request structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  F1AP : Created APER encodedbuffer for UE Context Setup Request\n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         }
      }

      /* Sending  msg  */
      if(SendF1APMsg(CU_APP_MEM_REG, CU_POOL, duId)	!=	ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Sending UE Context Setup Request Failed");
         break;
      }
      ret = ROK;
      break;
   }
   FreeUeContextSetupReq(f1apMsg);

   return ret;
}/* End of BuildAndSendUeContextSetupReq*/

/**********************************************************************
 * @brief Function to extractTeId received in UE context setup Response 
 *
 * @details
 *
 *    Function : extractTeId
 *    
 *    Functionality:
 *         - Function to extract TeId
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 **********************************************************************/
uint8_t extractTeId(DLUPTNLInformation_ToBeSetup_List_t *dlTnlInfo)
{
   uint8_t arrIdx = 0;
   uint32_t teId = 0;
   GTPTunnel_t *gtpDl = NULLP;

   for(arrIdx =0; arrIdx < dlTnlInfo->list.count; arrIdx++)
   {
      if(dlTnlInfo->list.array[arrIdx]->dLUPTNLInformation.present == UPTransportLayerInformation_PR_gTPTunnel)
      {
         if(dlTnlInfo->list.array[arrIdx]->dLUPTNLInformation.choice.gTPTunnel != NULLP)
         {
            gtpDl = dlTnlInfo->list.array[arrIdx]->dLUPTNLInformation.choice.gTPTunnel;
            if(gtpDl->gTP_TEID.size > 0)
            {
               teIdStringToInt(gtpDl->gTP_TEID.buf, &teId); 
            }
            else
               DU_LOG("\nERROR  -->  EGTP: No TeId received");
            return(teId);
         }
      }
   }
   return teId;
}

/****************************************************************
 * @brief Function to add Drb tunnels 
 *
 * @details
 *
 *    Function : addDrbTunnels
 *    
 *    Functionality:
 *         - Function to add Drb tunnels
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t addDrbTunnels(uint32_t duId, uint8_t teId)
{
   uint8_t ret = ROK;
   EgtpTnlEvt tnlEvt;

   if(teId > MAX_TEID || teId < MIN_TEID)
   {
      DU_LOG("\nERROR  -->  EGTP : TEID(%x) OUT Of Range",teId);
   }
   memset(&tnlEvt, 0, sizeof(EgtpTnlEvt));
   tnlEvt.action = EGTP_TNL_MGMT_ADD;
   tnlEvt.lclTeid = teId;
   tnlEvt.remTeid = teId;
   ret = cuEgtpTnlMgmtReq(duId, tnlEvt);
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  EGTP : Tunnel management request failed for teId %x", teId);
   }
   return ROK;
}

/****************************************************************
 * @brief Function to process Drb Setup List 
 *
 * @details
 *
 *    Function : procDrbSetupList
 *    
 *    Functionality:
 *         - Function to process DRB Setup List
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procDrbSetupList(uint32_t duId, CuUeCb *ueCb, DRBs_Setup_List_t *drbSetupList)
{
   uint8_t arrIdx = 0, drbIdx = 0;
   uint32_t teId = 0;
   DRBs_Setup_ItemIEs_t *drbItemIe = NULLP;

   if(drbSetupList != NULLP)
   {
      for(arrIdx = 0; arrIdx < drbSetupList->list.count; arrIdx++)
      {
         drbItemIe = ((DRBs_Setup_ItemIEs_t *)drbSetupList->list.array[arrIdx]);
         if(drbItemIe->value.present == DRBs_Setup_ItemIEs__value_PR_DRBs_Setup_Item)
         {
            /* extracting teId */
            teId  = extractTeId(&drbItemIe->value.choice.DRBs_Setup_Item.dLUPTNLInformation_ToBeSetup_List);
            if(teId > 0)
            {
               if(addDrbTunnels(duId, teId)== ROK)
               {
                  DU_LOG("\nDEBUG  --> EGTP: Tunnel Added for TeId %d", teId);
               }
               /* As per Spec 38.473, in UE COntext Response, Tunnel information
                * are sent to CU for setting up of Tunnels in DL direction.
                * Search for DRB ID in CU databse */
               for(drbIdx = 0; drbIdx < ueCb->numDrb; drbIdx++)
               {
                  if(ueCb->drbList[drbIdx].drbId == drbItemIe->value.choice.DRBs_Setup_Item.dRBID)
                  {
                     fillTeIdString(3, teId, ueCb->drbList[drbIdx].dlUpTnlInfo.teId);
                     break;
                  }
               }
            }
            else
               return RFAILED;
         }
      }
   }
   return ROK;
}

/****************************************************************
 * @brief Function to process Ue Context Setup Response 
 *
 * @details
 *
 *    Function : procUeContextSetupResponse
 *    
 *    Functionality:
 *         - Function to process Ue Context Setup Response
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procUeContextSetupResponse(uint32_t duId, F1AP_PDU_t *f1apMsg, char *recvBuf, MsgLen recvBufLen)
{
   uint8_t duIdx = 0, idx = 0, ueIdx = 0, rrcMsgType=0;
   uint8_t duUeF1apId = 0, cuUeF1apId = 0;
   DuDb *duDb = NULLP;
   CuUeCb *ueCb = NULLP;
   UEContextSetupResponse_t *ueCtxtSetupRsp = NULLP;
   OCTET_STRING_t *duToCuRrcContainer;

   SEARCH_DU_DB(duIdx, duId, duDb);
   ueCtxtSetupRsp = &f1apMsg->choice.successfulOutcome->value.choice.UEContextSetupResponse;
   
   for(idx=0; idx < ueCtxtSetupRsp->protocolIEs.list.count; idx++)
   {
      switch(ueCtxtSetupRsp->protocolIEs.list.array[idx]->id)
      {
          case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
             {
               cuUeF1apId = ueCtxtSetupRsp->protocolIEs.list.array[idx]->value.choice.GNB_CU_UE_F1AP_ID;
               break;
             }
          case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
             {
                duUeF1apId = ueCtxtSetupRsp->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID;
                ueCb = &duDb->ueCb[duUeF1apId-1];
                /* If ue context is not present in du db, then create UE context
                 * here. This flow is hit in case of UE handover where UE
                 * context is created before UE performs RACH on target DU */
                if(ueCb->gnbDuUeF1apId == 0)
                {
                   /* Creating UE context in target DU */
                   memset(ueCb, 0, sizeof(CuUeCb));

                   /* Check if UE is under Inter-CU handover */
                   if(duDb->tempUeCtxtInHo && (duDb->tempUeCtxtInHo->gnbCuUeF1apId == cuUeF1apId))
                   {
                      memcpy(ueCb, duDb->tempUeCtxtInHo, sizeof(CuUeCb));
                      ueCb->gnbDuUeF1apId = duUeF1apId;
                      CU_FREE(duDb->tempUeCtxtInHo, sizeof(CuUeCb));
                   }
                   else
                   {
                      /* In case of Inter DU Handover */
                      ueCb->cellCb = &duDb->cellCb[0];
                      ueCb->gnbDuUeF1apId = duUeF1apId;
                      ueCb->gnbCuUeF1apId = cuUeF1apId;
                      ueCb->state = UE_HANDOVER_IN_PROGRESS;
                      ueCb->hoInfo.HOType = Inter_DU_HO;
                      ueCb->hoInfo.tgtNodeId = duId; 
                   }
                   (duDb->numUe)++;

                   ueCb->cellCb->ueCb[ueCb->cellCb->numUe] = ueCb;
                   ueCb->cellCb->numUe++;
                }
                break;
             }
          case ProtocolIE_ID_id_C_RNTI:
             {
                ueCb->crnti = ueCtxtSetupRsp->protocolIEs.list.array[idx]->value.choice.C_RNTI;
                break;
             }
          case ProtocolIE_ID_id_DRBs_Setup_List:
             {
                /* Adding Tunnels for successful DRB */
                procDrbSetupList(duId, ueCb, &ueCtxtSetupRsp->protocolIEs.list.array[idx]->value.choice.DRBs_Setup_List);
                break; 
             }
         case ProtocolIE_ID_id_DUtoCURRCInformation:
             {
                DU_LOG("\nINFO  -->  Received Du to Cu RRC Information ");
                duToCuRrcContainer = &ueCtxtSetupRsp->protocolIEs.list.array[idx]->value.choice.\
                   DUtoCURRCInformation.cellGroupConfig;
                if((extractDuToCuRrcCont(ueCb, ueCtxtSetupRsp->protocolIEs.list.array[idx]->value.choice.\
                   DUtoCURRCInformation.cellGroupConfig)) != ROK)
                {
                   DU_LOG("\nERROR  -->  F1AP: Failed to extract Du to Cu RRC Information ");
                   return RFAILED;
                }
                break;
             }
      }
   }
   
   if(ueCb->state != UE_HANDOVER_IN_PROGRESS)
   {
      ueCb->f1apMsgDb.dlRrcMsgCount++;
      rrcMsgType = setDlRRCMsgType(ueCb);

      DU_LOG("\nINFO  -->  F1AP: Sending DL RRC MSG for RRC reconfiguration");
      if(BuildAndSendDLRRCMessageTransfer(duId, ueCb, SRB1, rrcMsgType) != ROK)
      {     
         DU_LOG("\nINFO  -->  F1AP: Failed to build and send DL RRC MSG for RRC reconfiguration");
         return RFAILED;
      }     
   }
   else
   {
      if(ueCb->hoInfo.HOType == Inter_DU_HO)
      {
         /* If the UE is in Inter-DU handover, UE context modification request is to be sent to
          * source DU once UE context setup response is received from target DU */

         DuDb *srcDuDb = NULLP;
         CuUeCb *ueCbInSrcDu = NULLP;

         /* Since Source DU Id and DU UE F1AP ID assigned to UE by source DU is not known here, we
          * need to find Source DU and UE CB in source DU using CU UE F1AP ID */
         for(duIdx=0; duIdx < cuCb.numDu; duIdx++)
         {
            /* UE context setup response is received from target DU. Search all
             * DUs to find source DU except this target DU Id.*/
            if(cuCb.duInfo[duIdx].duId != duId)
            {
               for(ueIdx = 0; ueIdx < MAX_NUM_UE; ueIdx++)
               {
                  /* Check following:
                   * 1. CU UE F1AP ID in srcDU->ueCb should be same as cuUeF1apId
                   * received in UE context setup response since CU UE F1AP ID does not
                   * change for UE in handover.
                   * 2. srcDU->UeCb->uestate should be UE_HANDOVER_IN_PROGRESS
                   */
                  if((cuCb.duInfo[duIdx].ueCb[ueIdx].gnbCuUeF1apId == cuUeF1apId) &&
                        (cuCb.duInfo[duIdx].ueCb[ueIdx].state == UE_HANDOVER_IN_PROGRESS))
                  {
                     srcDuDb = &cuCb.duInfo[duIdx];
                     ueCbInSrcDu = &cuCb.duInfo[duIdx].ueCb[ueIdx];

                     /* Store source DU info in the new UE context created in
                      * tareget DU */
                     ueCb->hoInfo.srcNodeId = srcDuDb->duId;

                     /* Copy the received container to UeCb */
                     memcpy(&ueCbInSrcDu->f1apMsgDb.duToCuContainer, duToCuRrcContainer, sizeof(OCTET_STRING_t));

                     if(BuildAndSendUeContextModificationReq(srcDuDb->duId, ueCbInSrcDu, STOP_DATA_TX) != ROK)
                     {
                        DU_LOG("\nERROR  ->  F1AP : Failed at BuildAndSendUeContextModificationReq()");
                        return RFAILED;
                     }
                     break;
                  }
               }
            }
            if(srcDuDb && ueCbInSrcDu)
               break;
         }
      }
      else if(ueCb->hoInfo.HOType == Xn_Based_Inter_CU_HO)
      {
         BuildAndSendHOReqAck(ueCb, duToCuRrcContainer->buf, duToCuRrcContainer->size);
      }
   }

   return ROK;
}

/****************************************************************
 * @brief Function to process Ul Rrc Msg received from DU 
 *
 * @details
 *
 *    Function : procUlRrcMsg
 *
 *    Functionality:
 *         - Function to process Ul Rrc Msg received from DU
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t procUlRrcMsg(uint32_t duId, F1AP_PDU_t *f1apMsg)
{
   uint8_t  idx = 0, ret = ROK, srbId = 0, rrcMsgType = 0, duIdx=0;
   uint8_t  *rrcContainer = NULLP;
   uint16_t rrcContLen = 0;
   uint32_t cuUeF1apId = 0, duUeF1apId = 0;
   DuDb     *duDb = NULLP;
   CuUeCb   *ueCb = NULLP;
   ULRRCMessageTransfer_t *ulRrcMsg = NULLP;

   ret = ROK;
   SEARCH_DU_DB(duIdx, duId, duDb);
   ulRrcMsg = &f1apMsg->choice.initiatingMessage->value.choice.ULRRCMessageTransfer;

   for(idx=0; idx < ulRrcMsg->protocolIEs.list.count; idx++)
   {
      switch(ulRrcMsg->protocolIEs.list.array[idx]->id)
      {
         case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
            {
               cuUeF1apId = ulRrcMsg->protocolIEs.list.array[idx]->value.choice.GNB_CU_UE_F1AP_ID;
               break;
            }
         case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
            {
               duUeF1apId = ulRrcMsg->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID;
               break;
            }
         case ProtocolIE_ID_id_SRBID:
            srbId = ulRrcMsg->protocolIEs.list.array[idx]->value.choice.SRBID;
            break;

         case ProtocolIE_ID_id_RRCContainer:
            {
               rrcContLen = ulRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.size;
               CU_ALLOC(rrcContainer, rrcContLen);
               if(!rrcContainer)
               {
                  DU_LOG("\nERROR  -->  F1AP : Failed to allocated memory in procUlRrcMsg");
                  return RFAILED;
               }
               memcpy(rrcContainer, ulRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf, rrcContLen);

               if(duDb->ueCb[duUeF1apId-1].state == UE_HANDOVER_IN_PROGRESS)
               {
                  if(duDb->ueCb[duUeF1apId-1].hoInfo.HOType == Inter_DU_HO)
                  {
                     uint8_t ueIdx = 0;
                     uint8_t srcDuId = duDb->ueCb[duUeF1apId-1].hoInfo.srcNodeId;
                     DuDb *srcDuDb = NULLP;

                     /* Release UE context in source DU because the UE is now
                      * attached to target DU */
                     SEARCH_DU_DB(duIdx, srcDuId, srcDuDb);
                     for(ueIdx = 0; ueIdx < srcDuDb->numUe; ueIdx++)
                     {
                        if(srcDuDb->ueCb[ueIdx].gnbCuUeF1apId == cuUeF1apId)
                        {
                           ret = BuildAndSendUeContextReleaseCommand(srcDuId, srcDuDb->ueCb[ueIdx].gnbCuUeF1apId, srcDuDb->ueCb[ueIdx].gnbDuUeF1apId); 
                           if(ret != ROK)
                           {
                              DU_LOG("\nINFO  -->  F1AP: Failed to build and send UE context release command to source DU Id [%d]", srcDuId);
                           }
                           break;
                        }
                     }
                  }
                  else 
                  {
                     BuildAndSendUeContextRelease(&duDb->ueCb[duUeF1apId-1]);
                  }

                  /* In target DU DB, mark UE as active and delete HO info */
                  duDb->ueCb[duUeF1apId-1].state = UE_ACTIVE;
                  memset(&duDb->ueCb[duUeF1apId-1].hoInfo, 0, sizeof(HandoverInfo));
                  return ret;
               }
               break;
            }

         default:
            DU_LOG("\nERROR  -->  F1AP : Invalid Event %ld", ulRrcMsg->protocolIEs.list.array[idx]->id);
            break;
      }
   }

   if(srbId == 1)
   {
      ueCb = &duDb->ueCb[duUeF1apId-1];
      ueCb->f1apMsgDb.dlRrcMsgCount++;
      rrcMsgType = setDlRRCMsgType(ueCb);
      if(rrcMsgType == REGISTRATION_COMPLETE)
      {
         DU_LOG("\nINFO  -->  F1AP: Sending Ue Context Setup Request"); 
         ret = BuildAndSendUeContextSetupReq(duId, ueCb);
      }
      else if(rrcMsgType == RRC_RECONFIG_COMPLETE)
      {
         DU_LOG("\nINFO  -->  F1AP: Sending UE Context Modification Request");
         BuildAndSendUeContextModificationReq(duId, ueCb, RRC_RECONFIG_COMPLETE_IND);
      }
      else
      {
         /* In case rrcMsgType is RRC_SETUP_COMPLETE / NAS_AUTHENTICATION_RSP / NAS_SECURITY_MODE_COMPLETE / RRC_SECURITY_MODE_COMPLETE */
         BuildAndSendDLRRCMessageTransfer(duId, ueCb, srbId, rrcMsgType);
      }
   }
   return ret;
}

/****************************************************************
 * @brief Build And Send F1ResetAck 
 *
 * @details
 *
 *    Function : FreeF1ResetAck
 *
 *    Functionality:
 *         - Build And Send  F1ResetRSP
 *
 *  @params[in]
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
		  CU_FREE(f1ResetAck->protocolIEs.list.array[idx], sizeof(ResetAcknowledgeIEs_t));
	       }
	    }
	    CU_FREE(f1ResetAck->protocolIEs.list.array, f1ResetAck->protocolIEs.list.size );
	 }
	 CU_FREE(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      }
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
   }
}

/****************************************************************
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
   uint8_t  	          elementCnt = 0;
   uint8_t                ret = RFAILED;
   F1AP_PDU_t             *f1apMsg = NULL;
   ResetAcknowledge_t     *f1ResetAck = NULLP;
   asn_enc_rval_t         encRetVal;
   DU_LOG("\nINFO  -->  F1AP : Building F1 Reset Acknowledgment \n");

   do{
      /* Allocate the memory for F1ResetRequest_t */
      CU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation for F1AP-PDU failed");
         break;
      }

      f1apMsg->present =  F1AP_PDU_PR_successfulOutcome;

      CU_ALLOC(f1apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcome_t));
      if(f1apMsg->choice.successfulOutcome == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation for F1AP-PDU failed");
         break;
      }

      f1apMsg->choice.successfulOutcome->procedureCode = ProcedureCode_id_Reset;
      f1apMsg->choice.successfulOutcome->criticality   = Criticality_reject;
      f1apMsg->choice.successfulOutcome->value.present = SuccessfulOutcome__value_PR_ResetAcknowledge;
      f1ResetAck = &f1apMsg->choice.successfulOutcome->value.choice.ResetAcknowledge;

      elementCnt = 1;

      f1ResetAck->protocolIEs.list.count = elementCnt;
      f1ResetAck->protocolIEs.list.size = elementCnt*sizeof(ResetAcknowledgeIEs_t *);

      CU_ALLOC(f1ResetAck->protocolIEs.list.array, f1ResetAck->protocolIEs.list.size ); 
      if(f1ResetAck->protocolIEs.list.array == NULLP) 
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation for F1ResetAckIEs failed");
         break;
      }

      for(idx=0; idx<elementCnt; idx++)     
      {
         CU_ALLOC(f1ResetAck->protocolIEs.list.array[idx], sizeof(ResetAcknowledgeIEs_t));
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
         DU_LOG("\nDEBUG  -->  F1AP : Created APER encodedbuffer for F1ResetAck \n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         }
      }

      /* Sending msg */
      /* TODO : Hardcoding DU ID to 1 for messages other than F1 Setup Response. This will be made generic in future gerrit */
      if(SendF1APMsg(CU_APP_MEM_REG, CU_POOL, DU_ID) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Sending F1 Reset Response failed");
         break;
      }

      ret = ROK;
      break;
   }while(true);

   FreeF1ResetAck(f1apMsg);
   return ret;
}

void FreeUlTnlInfoforDrb2(ULUPTNLInformation_ToBeSetup_List_t *ulInfo)
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
		     CU_FREE(ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->\
		     gTP_TEID.buf,ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.\
		     gTPTunnel->gTP_TEID.size);
		  }
	          CU_FREE(ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->\
		  transportLayerAddress.buf,ulInfo->list.array[arrIdx]->\
		  uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.size);
	       }
	       CU_FREE(ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel,\
	       sizeof(GTPTunnel_t));
	    }
	    CU_FREE(ulInfo->list.array[arrIdx],sizeof(ULUPTNLInformation_ToBeSetup_Item_t));
	 }
      }
      CU_FREE(ulInfo->list.array,ulInfo->list.size);
   }
}

/*******************************************************************
*
* @brief Deletes the EGTP tunnel
*
* @details
*
*    Function : deleteEgtpTunnel 
*
*    Functionality: Deletes the EGTP tunnel
*
* @params[in] uint8_t *buf
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t deleteEgtpTunnel(uint32_t duId, uint8_t *buf)
{
   uint32_t teId = 0;
   EgtpTnlEvt tnlEvt;

   teIdStringToInt(buf, &teId); 
   if(teId > MAX_TEID || teId < MIN_TEID)
   {
      DU_LOG("\nERROR  -->  EGTP : TEID(%d) OUT Of Range", teId);
      return RFAILED;
   }
   memset(&tnlEvt, 0, sizeof(EgtpTnlEvt));
   tnlEvt.action = EGTP_TNL_MGMT_DEL;
   tnlEvt.lclTeid = teId;
   tnlEvt.remTeid = teId;
   if((cuEgtpTnlMgmtReq(duId, tnlEvt)) != ROK)
   {
      DU_LOG("\nERROR  -->  EGTP : Failed to delete tunnel Id %d", teId);
   }
   return ROK;
}

/*******************************************************************
*
* @brief Builds the Uplink Tunnel Info
*
* @details
*
*    Function : BuildUlTnlInfoforSetupMod 
*
*    Functionality: Constructs the UL TnlInfo For DRB list
*
* @params[in] ULUPTNLInformation_ToBeSetup_List_t *ulInfo
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t BuildUlTnlInfoforSetupMod(uint32_t duId, uint8_t ueId, uint8_t drbId, TnlInfo *ulTnlInfo, ULUPTNLInformation_ToBeSetup_List_t *ulInfo, uint8_t actionType)
{
   uint8_t arrIdx;
   uint8_t ulCnt;

   ulCnt = 1;
   ulInfo->list.count = ulCnt;
   ulInfo->list.size = ulCnt * sizeof(ULUPTNLInformation_ToBeSetup_Item_t *);
   CU_ALLOC(ulInfo->list.array,ulInfo->list.size);
   if(ulInfo->list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildUlTnlInfoforSetupMod");
      return RFAILED;
   }
   for(arrIdx=0; arrIdx<ulCnt; arrIdx++)
   {
      CU_ALLOC(ulInfo->list.array[arrIdx],sizeof(ULUPTNLInformation_ToBeSetup_Item_t));
      if(ulInfo->list.array[arrIdx] == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildUlTnlInfoforSetupMod");
	 return RFAILED;
      }
   }
   
   arrIdx = 0;
   ulInfo->list.array[arrIdx]->uLUPTNLInformation.present = \
   UPTransportLayerInformation_PR_gTPTunnel;
   
   /*GTP TUNNEL*/
   CU_ALLOC(ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel,\
	 sizeof(GTPTunnel_t));
   if(ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildUlTnlInfoforSetupMod");
      return RFAILED;
   }
   ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->\
      transportLayerAddress.size        = 4*sizeof(uint8_t);
   CU_ALLOC(ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->\
	 transportLayerAddress.buf,ulInfo->list.array[arrIdx]->\
	 uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.size);
   if(ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->\
	 transportLayerAddress.buf == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildUlTnlInfoforSetupMod");
      return RFAILED;
   }
   
   ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->\
      transportLayerAddress.buf[0] = 192;
   ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->\
      transportLayerAddress.buf[1] = 168;
   ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->\
      transportLayerAddress.buf[2] = 130;
   ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->\
      transportLayerAddress.buf[3] = 82;
   ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->\
      transportLayerAddress.bits_unused = 0;

   ulTnlInfo->address[0] = ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf[0];
   ulTnlInfo->address[1] = ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf[1];
   ulTnlInfo->address[2] = ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf[2];
   ulTnlInfo->address[3] = ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress.buf[3];
   
   /*GTP TEID*/
   ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.size\
      = 4 * sizeof(uint8_t);
   CU_ALLOC(ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->\
	 gTP_TEID.buf,ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.\
	 gTPTunnel->gTP_TEID.size);
   if(ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf\
	 == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildUlTnlInfoforSetupMod");
      return RFAILED;
   }
   ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->\
      gTP_TEID.buf[0] = 0;
   ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->\
      gTP_TEID.buf[1] = 0;
   ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->\
      gTP_TEID.buf[2] = 0;
   if(actionType == ProtocolIE_ID_id_DRBs_ToBeModified_Item)
   {
     /*TODO: DRB context to be stored in CU STUB so that tunnel Id can be easily
      * fetched based on the Drb Id */

     /*For For UE Id=1, DRB=2 is modified. For UE Id = 2, DRB=5 is modified and so on.*/
      ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf[3]=\
                                                        (MAX_NUM_DRB_ADDED_PER_UE *(ueId  - 1)) + drbId;
   }
   else
   {
      ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->\
        gTP_TEID.buf[3] = cuCb.cuCfgParams.egtpParams.currTunnelId++;
   }

   ulTnlInfo->teId[0] = ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf[0];
   ulTnlInfo->teId[1] = ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf[1];
   ulTnlInfo->teId[2] = ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf[2];
   ulTnlInfo->teId[3] = ulInfo->list.array[arrIdx]->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf[3];
   return ROK;
}/*End of BuildULTnlInfo*/

/*******************************************************************
*
* @brief freeing the DRB item
*
* @details
*
*    Function : FreeDrbItem 
*
*    Functionality: freeing the DRB item
*
* @params[in] DRBs_ToBeSetupMod_Item_t *drbItem
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

void FreeDrbItem(DRBs_ToBeSetupMod_Item_t *drbItem)
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
	       CU_FREE(drbItem->qoSInformation.choice.eUTRANQoS, sizeof(EUTRANQoS_t));
	    }
	    break;
	 }
      case QoSInformation_PR_choice_extension:
	 {
	    if(drbItem->qoSInformation.choice.choice_extension)
	    {
	       FreeQOSInfo(&drbItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS);

	       snssai = &drbItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.sNSSAI;
	       if(snssai->sST.buf)
	       {
		  CU_FREE(snssai->sST.buf,snssai->sST.size);
	       }
	       if(snssai->sD)
	       {
	          if(snssai->sD->buf)
		  {
		     CU_FREE(snssai->sD->buf,snssai->sD->size);
		  }
		  CU_FREE(snssai->sD,sizeof(OCTET_STRING_t));
	       }

	       flowMap = &drbItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.flows_Mapped_To_DRB_List;
	       if(flowMap->list.array)
	       {
		  for(arrIdx=0; arrIdx<flowMap->list.count; arrIdx++)
		  {
		     if(flowMap->list.array[arrIdx] )
		     {
			FreeQOSInfo(&flowMap->list.array[arrIdx]->qoSFlowLevelQoSParameters);
			CU_FREE(flowMap->list.array[arrIdx],sizeof(Flows_Mapped_To_DRB_Item_t));
		     }
		  }
		  CU_FREE(flowMap->list.array,flowMap->list.size);
	       }

	       CU_FREE(drbItem->qoSInformation.choice.choice_extension,sizeof(QoSInformation_ExtIEs_t));
	    }
	    break;
	 }

   }
   FreeUlTnlInfoforDrb2(&drbItem->uLUPTNLInformation_ToBeSetup_List);
   if(drbItem->uLConfiguration)
   {
      CU_FREE(drbItem->uLConfiguration,sizeof(ULConfiguration_t));
   }
}

/*******************************************************************
*
* @brief filling the DRB setup Mod item 
*
* @details
*
*    Function : FillDrbItemToSetupMod 
*
*    Functionality: filling the DRB setup Mod item
*    
*    
* @params[in] DRBs_ToBeSetupMod_Item_t *drbItem 
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t FillDrbItemToSetupMod(uint32_t duId, CuUeCb *ueCb, uint8_t arrIdx, DRBs_ToBeSetupMod_Item_t *drbItem)
{
   uint8_t ret = ROK;

   /*Drb Id */
   drbItem->dRBID = arrIdx + DRB_ID_TO_ADD_MOD;
   ueCb->drbList[ueCb->numDrb].drbId = drbItem->dRBID;
   
   /*qoSInformation*/
   drbItem->qoSInformation.present = QoSInformation_PR_choice_extension;
  
   switch(drbItem->qoSInformation.present)
   {
      case QoSInformation_PR_NOTHING:
      {
         break;
      }
      case QoSInformation_PR_eUTRANQoS:
      {

	 CU_ALLOC(drbItem->qoSInformation.choice.eUTRANQoS, sizeof(EUTRANQoS_t));
	 if(drbItem->qoSInformation.choice.eUTRANQoS)
	 {  
	    DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in FillDrbItemToSetupMod");
	    return RFAILED;
	 }
	 drbItem->qoSInformation.choice.eUTRANQoS->qCI = QCI;
	 drbItem->qoSInformation.choice.eUTRANQoS->allocationAndRetentionPriority.priorityLevel =
	    PriorityLevel_no_priority;

	 drbItem->qoSInformation.choice.eUTRANQoS->allocationAndRetentionPriority.pre_emptionCapability =
	    Pre_emptionCapability_may_trigger_pre_emption;

	 drbItem->qoSInformation.choice.eUTRANQoS->allocationAndRetentionPriority.pre_emptionVulnerability =
	    Pre_emptionVulnerability_pre_emptable;

	 break;
      }
      case QoSInformation_PR_choice_extension:
      {
	 CU_ALLOC(drbItem->qoSInformation.choice.choice_extension,sizeof(QoSInformation_ExtIEs_t));
	 if(drbItem->qoSInformation.choice.choice_extension == NULLP)
	 {
	    DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in FillDrbItemToSetupMod"); 
	    return RFAILED;
	 }

	 drbItem->qoSInformation.choice.choice_extension->id = ProtocolIE_ID_id_DRB_Information;
	 drbItem->qoSInformation.choice.choice_extension->criticality = Criticality_ignore;
	 drbItem->qoSInformation.choice.choice_extension->value.present = QoSInformation_ExtIEs__value_PR_DRB_Information;
	 ret =  BuildQOSInfo(&ueCb->drbList[ueCb->numDrb].qos, &drbItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information.dRB_QoS,\
           ProtocolIE_ID_id_DRBs_ToBeSetupMod_Item, PDU_SESSION_ID_2, FALSE);
	 if(ret != ROK)
	 {
	    DU_LOG("\nERROR  -->  F1AP : BuildQOSInfo failed");
	    return RFAILED;
	 }
	 
	 /*SNSSAI*/
	 ret = BuildSNSSAI(&ueCb->drbList[ueCb->numDrb], &drbItem->qoSInformation.choice.\
	       choice_extension->value.choice.DRB_Information.sNSSAI, cuCb.snssaiList[1], FALSE);
	 if(ret != ROK)
	 {
	    DU_LOG("\nERROR  -->  F1AP : BuildSNSSAI failed");
	    return RFAILED;
	 }
	 
	 /*Flows mapped to DRB List*/
         ret = BuildFlowsMap(&ueCb->drbList[ueCb->numDrb], &drbItem->qoSInformation.choice.\
	       choice_extension->value.choice.DRB_Information.flows_Mapped_To_DRB_List,\ 
          ProtocolIE_ID_id_DRBs_ToBeSetupMod_Item, FALSE);
	 if(ret != ROK)
	 {
	    DU_LOG("\nERROR  -->  F1AP : BuildFlowsMap failed");
	    return RFAILED;
	 }
      }
   }
   
   /*ULUPTNLInformation To Be Setup List*/
   ret = BuildUlTnlInfoforSetupMod(duId, ueCb->gnbCuUeF1apId, drbItem->dRBID, &ueCb->drbList[ueCb->numDrb].ulUpTnlInfo, \
      &drbItem->uLUPTNLInformation_ToBeSetup_List, ProtocolIE_ID_id_DRBs_ToBeSetupMod_Item);
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : BuildUlTnlInfoforSetupMod failed");
      return RFAILED;
   }

   /*RLCMode*/
   drbItem->rLCMode = RLCMode_rlc_um_bidirectional;
   ueCb->drbList[ueCb->numDrb].rlcMode = drbItem->rLCMode;

   ueCb->numDrb++;
   return ROK;
}

/*******************************************************************
*
* @brief Builds the DRB to be Setup Mod ItemIes
*
* @details
*
*    Function : FillDrbItemList 
*
*    Functionality: Constructs the DRB to be Setup Mod Item Ies
*
* @params[in] struct DRBs_ToBeSetupMod_ItemIEs *drbItemIe 
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t FillDrbItemList(uint32_t duId, CuUeCb *ueCb, uint8_t arrIdx, struct DRBs_ToBeSetupMod_ItemIEs *drbItemIe)
{
   drbItemIe->id = ProtocolIE_ID_id_DRBs_ToBeSetupMod_Item;
   drbItemIe->criticality = Criticality_reject;
   drbItemIe->value.present = DRBs_ToBeSetupMod_ItemIEs__value_PR_DRBs_ToBeSetupMod_Item;

   if(FillDrbItemToSetupMod(duId, ueCb, arrIdx, (&(drbItemIe->value.choice.DRBs_ToBeSetupMod_Item))) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : FillDrbItemToSetupMod failed"); 
      return RFAILED;
   }
   return ROK;
}
/*******************************************************************
*
* @brief free the DRB to be Setup Mod list
*
* @details
*
*    Function : FreeDrbToBeSetupModList
*
*    Functionality: free the DRB to be Setup Mod list
*
* @params[in] DRBs_ToBeSetupMod_List_t *drbSet
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
void FreeDrbToBeSetupModList(DRBs_ToBeSetupMod_List_t *drbSet)
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
	       FreeDrbItem(&(drbItemIe->value.choice.DRBs_ToBeSetupMod_Item));
	    }
	    CU_FREE(drbSet->list.array[arrIdx], sizeof(DRBs_ToBeSetupMod_ItemIEs_t));
	 }
      }
      CU_FREE(drbSet->list.array, drbSet->list.size);
   }
   
}

/*******************************************************************
*
* @brief Builds the DRB to be Setup Mod list 
*
* @details
*
*    Function : BuildDrbToBeSetupList 
*
*    Functionality: Constructs the DRB to be Setup Mod list
*
* @params[in] DRBs_ToBeSetupMod_List_t *drbSet 
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t BuildDrbToBeSetupList(uint32_t duId, CuUeCb *ueCb, DRBs_ToBeSetupMod_List_t *drbSet)
{
   uint8_t ret = ROK;
   uint8_t arrIdx =0;
   uint8_t drbCnt =0;

   drbCnt = MAX_DRB_SET_UE_CONTEXT_MOD_REQ;
   drbSet->list.count = drbCnt;
   drbSet->list.size = drbCnt * sizeof(DRBs_ToBeSetupMod_ItemIEs_t *);
   CU_ALLOC(drbSet->list.array, drbSet->list.size);
   if(drbSet->list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDrbToBeSetupList");
      return  RFAILED;
   }

   for(arrIdx=0; arrIdx<drbCnt; arrIdx++)
   {
      CU_ALLOC(drbSet->list.array[arrIdx], sizeof(DRBs_ToBeSetupMod_ItemIEs_t));
      if(drbSet->list.array[arrIdx] == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDrbToBeSetupList for array idx [%d]", arrIdx);
         return  RFAILED;
      }

      ret = FillDrbItemList(duId, ueCb, arrIdx, (DRBs_ToBeSetupMod_ItemIEs_t *)drbSet->list.array[arrIdx]);
      if(ret != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : FillDrbItemList failed");
      }
   }

   return ret;
}

/*******************************************************************
*
* @brief Filling the DRB to be modified item 
*
* @details
*
*    Function : FillDrbToBeModItem
*
*    Functionality: filling the DRB to be modified item
*
* @params[in] DRBs_ToBeModified_Item_t *drbItem 
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t FillDrbToBeModItem(uint32_t duId, CuUeCb *ueCb, uint8_t arrIdx, DRBs_ToBeModified_Item_t *drbItem)
{
   uint8_t ret = ROK;
   uint drbIdx=0;
   DrbInfo *drbToBeMod;

   /*Drb Id */
   drbItem->dRBID = DRB2 + arrIdx;

   /* Search for DRB ID in CU databse */
   for(drbIdx = 0; drbIdx < ueCb->numDrb; drbIdx++)
   {
      if(ueCb->drbList[drbIdx].drbId == drbItem->dRBID)
      {
         drbToBeMod = &ueCb->drbList[drbIdx];
         break;
      }
   }

   /*qoSInformation*/
   drbItem->qoSInformation = NULLP;
   CU_ALLOC(drbItem->qoSInformation, sizeof(QoSInformation_t));
   if(drbItem->qoSInformation != NULLP)
   {
      drbItem->qoSInformation->present = QoSInformation_PR_choice_extension;

      switch(drbItem->qoSInformation->present)
      {
         case QoSInformation_PR_NOTHING:
            {
               break;
            }
         case QoSInformation_PR_eUTRANQoS:
            {

               CU_ALLOC(drbItem->qoSInformation->choice.eUTRANQoS, sizeof(EUTRANQoS_t));
               if(drbItem->qoSInformation->choice.eUTRANQoS)
               {  
                  DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in FillDrbToBeModItem");
                  return RFAILED;
               }
               drbItem->qoSInformation->choice.eUTRANQoS->qCI = QCI;
               drbItem->qoSInformation->choice.eUTRANQoS->allocationAndRetentionPriority.priorityLevel =
                  PriorityLevel_no_priority;

               drbItem->qoSInformation->choice.eUTRANQoS->allocationAndRetentionPriority.pre_emptionCapability =
                  Pre_emptionCapability_may_trigger_pre_emption;

               drbItem->qoSInformation->choice.eUTRANQoS->allocationAndRetentionPriority.pre_emptionVulnerability =
                  Pre_emptionVulnerability_pre_emptable;

               break;
            }
         case QoSInformation_PR_choice_extension:
            {
               CU_ALLOC(drbItem->qoSInformation->choice.choice_extension,sizeof(QoSInformation_ExtIEs_t));
               if(drbItem->qoSInformation->choice.choice_extension == NULLP)
               {
                  DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in FillDrbItemToSetupMod"); 
                  return RFAILED;
               }

               drbItem->qoSInformation->choice.choice_extension->id = ProtocolIE_ID_id_DRB_Information;
               drbItem->qoSInformation->choice.choice_extension->criticality = Criticality_ignore;
               drbItem->qoSInformation->choice.choice_extension->value.present = QoSInformation_ExtIEs__value_PR_DRB_Information;
               ret =  BuildQOSInfo(&drbToBeMod->qos, &drbItem->qoSInformation->choice.choice_extension->value.choice.DRB_Information.dRB_QoS,\
                     ProtocolIE_ID_id_DRBs_ToBeModified_Item, INVALID_PDU_SESSION_ID, FALSE);
               if(ret != ROK)
               {
                  DU_LOG("\nERROR  -->  F1AP : BuildQOSInfo failed");
                  return RFAILED;
               }

               /*SNSSAI*/
               ret = BuildSNSSAI(drbToBeMod, &drbItem->qoSInformation->choice.\
                     choice_extension->value.choice.DRB_Information.sNSSAI, cuCb.snssaiList[0], FALSE);
               if(ret != ROK)
               {
                  DU_LOG("\nERROR  -->  F1AP : BuildSNSSAI failed");
                  return RFAILED;
               }

               /*Flows mapped to DRB List*/
               ret = BuildFlowsMap(drbToBeMod, &drbItem->qoSInformation->choice.\
                     choice_extension->value.choice.DRB_Information.flows_Mapped_To_DRB_List,\
                     ProtocolIE_ID_id_DRBs_ToBeModified_Item, FALSE);
               if(ret != ROK)
               {
                  DU_LOG("\nERROR  -->  F1AP : BuildFlowsMap failed");
                  return RFAILED;
               }
            }
      }
   }/* End of QoS */

   /*ULUPTNLInformation To Be Setup List*/
   ret = BuildUlTnlInfoforSetupMod(duId, ueCb->gnbCuUeF1apId, drbItem->dRBID, &drbToBeMod->ulUpTnlInfo, &drbItem->uLUPTNLInformation_ToBeSetup_List,\
            ProtocolIE_ID_id_DRBs_ToBeModified_Item);
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : BuildUlTnlInfoforSetupMod failed");
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
*
* @brief Builds the DRB to be modified Item IE
*
* @details
*
*    Function : FillDrbToBeModItemList
*
*    Functionality: Constructs the DRB to be modified Mod Item Ies
*
* @params[in] struct DRBs_ToBeModified_ItemIEs *drbItemIe
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t FillDrbToBeModItemList(uint32_t duId, CuUeCb *ueCb, uint8_t arrIdx, struct DRBs_ToBeModified_ItemIEs *drbItemIe)
{
   drbItemIe->id = ProtocolIE_ID_id_DRBs_ToBeModified_Item;
   drbItemIe->criticality = Criticality_reject;
   drbItemIe->value.present = DRBs_ToBeModified_ItemIEs__value_PR_DRBs_ToBeModified_Item;
   if(FillDrbToBeModItem(duId, ueCb, arrIdx, &(drbItemIe->value.choice.DRBs_ToBeModified_Item)) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : FillDrbToBeModItem failed"); 
      return RFAILED;
   }

   return ROK;
}

/*******************************************************************
*
* @brief Builds the DRB to be modified list 
*
* @details
*
*    Function : BuildDrbToBeModList 
*
*    Functionality: Constructs the DRB to be modified list
*
* @params[in] DRBs_ToBeModified_List_t *drbSet 
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t BuildDrbToBeModifiedList(uint32_t duId, CuUeCb *ueCb, DRBs_ToBeModified_List_t *drbSet)
{
   uint8_t ret = ROK;
   uint8_t arrIdx =0;
   uint8_t drbCnt =0;

   drbCnt = MAX_DRB_MODIFIED_UE_MOD_REQ;
   drbSet->list.count = drbCnt;
   drbSet->list.size = drbCnt * sizeof(DRBs_ToBeModified_ItemIEs_t *);
   CU_ALLOC(drbSet->list.array, drbSet->list.size);
   if(drbSet->list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDrbToBeModifiedList");
      return  RFAILED;
   }
   for(arrIdx=0; arrIdx<drbCnt; arrIdx++)
   {
      CU_ALLOC(drbSet->list.array[arrIdx], sizeof(DRBs_ToBeModified_ItemIEs_t));
      if(drbSet->list.array[arrIdx] == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDrbToBeSetupList");
         return  RFAILED;
      }

      ret = FillDrbToBeModItemList(duId, ueCb, arrIdx, (DRBs_ToBeModified_ItemIEs_t *)drbSet->list.array[arrIdx]);
      if(ret != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : FillDrbToBeModItemList failed");
      }
   }

   return ret;
}

/*******************************************************************
*
* @brief Builds the DRB to be released Item IE
*
* @details
*
*    Function : FillDrbToBeRelItemList
*
*    Functionality: Constructs the DRB to be modified Mod Item Ies
*
* @params[in] struct DRBs_ToBeReleased_ItemIEs *drbItemIe
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t FillDrbToBeRelItemList(uint32_t duId, CuUeCb *ueCb, uint8_t arrIdx, struct DRBs_ToBeReleased_ItemIEs *drbItemIe)
{
   uint8_t drbIdx;

   drbItemIe->id = ProtocolIE_ID_id_DRBs_ToBeReleased_Item;
   drbItemIe->criticality = Criticality_reject;
   drbItemIe->value.present = DRBs_ToBeReleased_ItemIEs__value_PR_DRBs_ToBeReleased_Item;
   drbItemIe->value.choice.DRBs_ToBeReleased_Item.dRBID = DRB1 + arrIdx;

   for(drbIdx = 0; drbIdx < ueCb->numDrb; drbIdx++)
   {
      if(ueCb->drbList[drbIdx].drbId == drbItemIe->value.choice.DRBs_ToBeReleased_Item.dRBID)
      {
         deleteEgtpTunnel(duId, ueCb->drbList[drbIdx].dlUpTnlInfo.teId);
         CU_FREE(ueCb->drbList[drbIdx].snssai, sizeof(Snssai));
         break;
      }
   }
   return ROK;
}

/*******************************************************************
*
* @brief Builds the DRB to be released list 
*
* @details
*
*    Function : BuildDrbToBeReleasedList 
*
*    Functionality: Constructs the DRB to be released list
*
* @params[in] DRBs_ToBeReleased_List_t *drbSet 
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t BuildDrbToBeReleasedList(uint32_t duId, CuUeCb *ueCb, DRBs_ToBeReleased_List_t *drbSet)
{
   uint8_t ret = ROK;
   uint8_t arrIdx =0;
   uint8_t drbCnt =0;

   drbCnt = MAX_DRB_MODIFIED_UE_MOD_REQ;
   drbSet->list.count = drbCnt;
   drbSet->list.size = drbCnt * sizeof(DRBs_ToBeReleased_ItemIEs_t *);
   CU_ALLOC(drbSet->list.array, drbSet->list.size);
   if(drbSet->list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDrbToBeReleasedList");
      return  RFAILED;
   }
   for(arrIdx=0; arrIdx<drbCnt; arrIdx++)
   {
      CU_ALLOC(drbSet->list.array[arrIdx], sizeof(DRBs_ToBeReleased_ItemIEs_t));
      if(drbSet->list.array[arrIdx] == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation failed in BuildDrbToBeReleasedList");
         return  RFAILED;
      }

      ret = FillDrbToBeRelItemList(duId, ueCb, arrIdx, (DRBs_ToBeReleased_ItemIEs_t *)drbSet->list.array[arrIdx]);
      if(ret != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : FillDrbToBeRelItemList failed");
      }
   }

   return ret;
}

/*******************************************************************
*
* @brief freeing the DRB  item
*
* @details
*
*    Function : FreeModifiedDrbItem
*
*    Functionality: freeing the DRB 2 item
*
* @params[in] DRBs_ToBeSetupMod_Item_t *drbItem
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

void FreeModifiedDrbItem(DRBs_ToBeModified_Item_t *drbItem)
{
   uint8_t arrIdx =0;
   SNSSAI_t *snssai =NULLP;
   Flows_Mapped_To_DRB_List_t *flowMap = NULLP;

   if(drbItem->qoSInformation != NULLP)
   { 
      switch(drbItem->qoSInformation->present)
      {
         case QoSInformation_PR_NOTHING:
            break;
         case QoSInformation_PR_eUTRANQoS:
            {
               if(drbItem->qoSInformation->choice.eUTRANQoS)
               {
                  CU_FREE(drbItem->qoSInformation->choice.eUTRANQoS, sizeof(EUTRANQoS_t));
               }
               break;
            }
         case QoSInformation_PR_choice_extension:
            {
               if(drbItem->qoSInformation->choice.choice_extension)
               {
                  FreeQOSInfo(&drbItem->qoSInformation->choice.choice_extension->value.choice.DRB_Information.dRB_QoS);

                  snssai = &drbItem->qoSInformation->choice.choice_extension->value.choice.DRB_Information.sNSSAI;
                  if(snssai->sST.buf)
                  {
                     CU_FREE(snssai->sST.buf,snssai->sST.size);
                  }
                  if(snssai->sD)
                  {
                     if(snssai->sD->buf)
                     {
                        CU_FREE(snssai->sD->buf,snssai->sD->size);
                     }
                     CU_FREE(snssai->sD,sizeof(OCTET_STRING_t));
                  }

                  flowMap = &drbItem->qoSInformation->choice.choice_extension->value.choice.DRB_Information.flows_Mapped_To_DRB_List;
                  if(flowMap->list.array)
                  {
                     for(arrIdx=0; arrIdx<flowMap->list.count; arrIdx++)
                     {
                        if(flowMap->list.array[arrIdx] )
                        {
                           FreeQOSInfo(&flowMap->list.array[arrIdx]->qoSFlowLevelQoSParameters);
                           CU_FREE(flowMap->list.array[arrIdx],sizeof(Flows_Mapped_To_DRB_Item_t));
                        }
                     }
                     CU_FREE(flowMap->list.array,flowMap->list.size);
                  }

                  CU_FREE(drbItem->qoSInformation->choice.choice_extension,sizeof(QoSInformation_ExtIEs_t));
               }
               break;
            }
      }
   }
   FreeUlTnlInfoforDrb2(&drbItem->uLUPTNLInformation_ToBeSetup_List);
   if(drbItem->uLConfiguration)
   {
      CU_FREE(drbItem->uLConfiguration,sizeof(ULConfiguration_t));
   }
}

/*******************************************************************
*
* @brief free the DRB to be modfified list
*
* @details
*
*    Function : FreeDrbToBeModifiedList
*
*    Functionality: free the DRB to be Setup Mod list
*
* @params[in] FreeDrbToBeModifiedList_t *drbSet
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
void FreeDrbToBeModifiedList(DRBs_ToBeModified_List_t *drbSet)
{
   uint8_t arrIdx =0;
   struct DRBs_ToBeModified_ItemIEs *drbItemIe;

   if(drbSet->list.array)
   {
      for(arrIdx=0; arrIdx<drbSet->list.count ; arrIdx++)
      {
         if(drbSet->list.array[arrIdx] != NULLP)
         {
            drbItemIe = (struct DRBs_ToBeModified_ItemIEs *)drbSet->list.array[arrIdx];
            FreeModifiedDrbItem(&(drbItemIe->value.choice.DRBs_ToBeModified_Item));
            CU_FREE(drbSet->list.array[arrIdx], sizeof(DRBs_ToBeModified_ItemIEs_t));
         }
      }
      CU_FREE(drbSet->list.array, drbSet->list.size);
   }

}

/*******************************************************************
*
* @brief free the DRB to be modfified list
*
* @details
*
*    Function : FreeDrbToBeReleasedList
*
*    Functionality: free the DRB to be Release list
*
* @params[in] FreeDrbToBeReleasedList_t *drbSet
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
void FreeDrbToBeReleasedList(DRBs_ToBeReleased_List_t *drbSet)
{
   uint8_t arrIdx =0;
   struct DRBs_ToBeReleased_ItemIEs *drbItemIe;

   if(drbSet->list.array)
   {
      for(arrIdx=0; arrIdx<drbSet->list.count ; arrIdx++)
      {
         if(drbSet->list.array[arrIdx] != NULLP)
         {
            CU_FREE(drbSet->list.array[arrIdx], sizeof(DRBs_ToBeReleased_ItemIEs_t));
         }
      }
      CU_FREE(drbSet->list.array, drbSet->list.size);
   }
}

/*******************************************************************
 *
 * @brief  free the UeContextModification Request 
 *
 * @details
 *
 *    Function : FreeUeContextModicationRequest 
 *
 *    Functionality : deallocation of memory allocated in UeContextModiification
 request
 *
 * @params[in] F1AP_PDU_t *f1apMsg 
 *
 * @return void 
*
* ****************************************************************/
void FreeUeContextModicationRequest(F1AP_PDU_t *f1apMsg)
{
   uint8_t arrIdx =0 , ieId=0; 
   UEContextModificationRequest_t *ueContextModifyReq = NULLP;

   if(f1apMsg)
   {
      if(f1apMsg->choice.initiatingMessage)
      {
         ueContextModifyReq =&f1apMsg->choice.initiatingMessage->value.choice.UEContextModificationRequest;
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
                           FreeDrbToBeSetupModList(&ueContextModifyReq->protocolIEs.list.array[arrIdx]->value.\
                                 choice.DRBs_ToBeSetupMod_List);
                           break;
                        }
                     case ProtocolIE_ID_id_DRBs_ToBeModified_List:
                        {
                           FreeDrbToBeModifiedList(&ueContextModifyReq->protocolIEs.list.array[arrIdx]->value.\
                                 choice.DRBs_ToBeModified_List);
                           break;
                        }
                     case ProtocolIE_ID_id_DRBs_ToBeReleased_List:
                        {
                           FreeDrbToBeReleasedList(&ueContextModifyReq->protocolIEs.list.array[arrIdx]->value.\
                                 choice.DRBs_ToBeReleased_List);
                           break;
                        }
                    case ProtocolIE_ID_id_TransmissionActionIndicator:
                        break;
                    case ProtocolIE_ID_id_RRCContainer:
                    {
                        CU_FREE(ueContextModifyReq->protocolIEs.list.array[arrIdx]->value.choice.RRCContainer.buf,\
                        ueContextModifyReq->protocolIEs.list.array[arrIdx]->value.choice.RRCContainer.size);
                    }

                  }
                  CU_FREE(ueContextModifyReq->protocolIEs.list.array[arrIdx], sizeof(UEContextModificationRequest_t));
               }	  
            }
            CU_FREE(ueContextModifyReq->protocolIEs.list.array, ueContextModifyReq->protocolIEs.list.size);
         }
         CU_FREE(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      }
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Builds the Ue Context Modification Req 
 *
 * @details
 *
 *    Function : BuildAndSendUeContextModificationReq 
 *
 *    Functionality: Constructs the Ue Context Modification Req
 *
 * @params[in] 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendUeContextModificationReq(uint32_t duId, void *cuUeCb, UeCtxtModAction action)
{
   uint8_t    ieIdx = 0;
   uint8_t    elementCnt = 0;
   uint8_t    ret = RFAILED;
   uint16_t   tmpBufIdx = 0, bufIdx = 0;
   CuUeCb     *ueCb = (CuUeCb *)cuUeCb;
   F1AP_PDU_t *f1apMsg = NULLP;
   UEContextModificationRequest_t *ueContextModifyReq = NULLP;
   RRCContainer_t rrcContainerTmp, *rrcContainer = NULLP;
   asn_enc_rval_t         encRetVal;
   DU_LOG("\nINFO  -->  F1AP : Building Ue context modification request\n");
   while(1)
   {
      CU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation for F1AP-PDU failed Ue context modification");
         break;
      }

      f1apMsg->present =  F1AP_PDU_PR_initiatingMessage;

      CU_ALLOC(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      if(f1apMsg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : Memory allocation for F1AP-PDU failed Ue context modification");
         break;
      }
      f1apMsg->choice.initiatingMessage->procedureCode = ProcedureCode_id_UEContextModification;
      f1apMsg->choice.initiatingMessage->criticality = Criticality_reject;
      f1apMsg->choice.initiatingMessage->value.present = InitiatingMessage__value_PR_UEContextModificationRequest;

      ueContextModifyReq =&f1apMsg->choice.initiatingMessage->value.choice.UEContextModificationRequest;

      if(action == MODIFY_UE)
         elementCnt = 5;
      else if(action == QUERY_CONFIG)
         elementCnt = 3;
      else if(action == RRC_RECONFIG_COMPLETE_IND)
         elementCnt = 3;
      else if((action == STOP_DATA_TX) || (action == RESTART_DATA_TX)) 
      {
         if(ueCb->state != UE_HANDOVER_IN_PROGRESS)
            elementCnt = 5;
         else
            elementCnt = 4;
      }
      
#ifdef NR_DRX
      if(DRX_TO_BE_RELEASE && ueCb->drxCfgPresent)
         elementCnt++;
#endif      
      ueContextModifyReq->protocolIEs.list.count = elementCnt;
      ueContextModifyReq->protocolIEs.list.size = elementCnt*sizeof(UEContextModificationRequest_t *);

      /* Initialize the UE context modification members */
      CU_ALLOC(ueContextModifyReq->protocolIEs.list.array, ueContextModifyReq->protocolIEs.list.size);
      if(ueContextModifyReq->protocolIEs.list.array == NULLP)
      { 
         DU_LOG("\nERROR  -->  F1AP : Memory allocation for UE context modifcation Request failed");
         break;
      }

      for(ieIdx=0 ; ieIdx<elementCnt; ieIdx++)
      {
         CU_ALLOC(ueContextModifyReq->protocolIEs.list.array[ieIdx], sizeof(UEContextModificationRequest_t));
         if(ueContextModifyReq->protocolIEs.list.array[ieIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  F1AP : Memory allocation for UE context modifcation Request failed");
            break;
         }
      }

      ieIdx=0;
      ueContextModifyReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;
      ueContextModifyReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.present = \
                                                                         UEContextModificationRequestIEs__value_PR_GNB_CU_UE_F1AP_ID;
      ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_CU_UE_F1AP_ID = ueCb->gnbCuUeF1apId;

      ieIdx++;
      ueContextModifyReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
      ueContextModifyReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.present=\
                                                                       UEContextModificationRequestIEs__value_PR_GNB_DU_UE_F1AP_ID;
      ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_UE_F1AP_ID = ueCb->gnbDuUeF1apId;

      if(action == MODIFY_UE)
      {
         /* DRB to be setup list */
         ieIdx++;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_DRBs_ToBeSetupMod_List;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.present =\
                                                                           UEContextModificationRequestIEs__value_PR_DRBs_ToBeSetupMod_List;
         ret = BuildDrbToBeSetupList(duId, ueCb, &(ueContextModifyReq->protocolIEs.list.array[ieIdx]->\
                  value.choice.DRBs_ToBeSetupMod_List));

         /* DRB to be modified list */
         ieIdx++;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_DRBs_ToBeModified_List;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.present =\
                                                                           UEContextModificationRequestIEs__value_PR_DRBs_ToBeModified_List;
         ret = BuildDrbToBeModifiedList(duId, ueCb, &(ueContextModifyReq->protocolIEs.list.array[ieIdx]->\
                  value.choice.DRBs_ToBeModified_List));

         if(ret != ROK)
         {
            DU_LOG("\nERROR  -->  F1AP : BuildAndSendUeContextModificationReq(): Failed to build drb to be modified list");
            break;
         }

         /* DRB to be released list */
         ieIdx++;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_DRBs_ToBeReleased_List;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.present =\
                                                                 UEContextModificationRequestIEs__value_PR_DRBs_ToBeReleased_List;
         ret = BuildDrbToBeReleasedList(duId, ueCb, &(ueContextModifyReq->protocolIEs.list.array[ieIdx]->\
                  value.choice.DRBs_ToBeReleased_List));

         if(ret != ROK)
         {
            DU_LOG("\nERROR  -->  F1AP : BuildAndSendUeContextModificationReq(): Failed to build drb to be deleted list");
            break;
         }
      }
      else if(action == QUERY_CONFIG)
      {
         ieIdx++;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_GNB_DUConfigurationQuery;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.present = \
            UEContextModificationRequestIEs__value_PR_GNB_DUConfigurationQuery;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DUConfigurationQuery = GNB_DUConfigurationQuery_true;
      }
      else if(action == RRC_RECONFIG_COMPLETE_IND)
      {
         ieIdx++;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_RRCReconfigurationCompleteIndicator;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.present = \
            UEContextModificationRequestIEs__value_PR_RRCReconfigurationCompleteIndicator;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.choice.RRCReconfigurationCompleteIndicator = \
            RRCReconfigurationCompleteIndicator_true;
      }
      else if((action == STOP_DATA_TX) || (action == RESTART_DATA_TX))
      {
         ieIdx++;
         if(action == STOP_DATA_TX)
         {
            ueContextModifyReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_TransmissionActionIndicator;
            ueContextModifyReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
            ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.present = \
            UEContextModificationRequestIEs__value_PR_TransmissionActionIndicator;
            ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.choice.TransmissionActionIndicator = \
            TransmissionActionIndicator_stop;
         }
         else if (action == RESTART_DATA_TX)
         {
            ueContextModifyReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_TransmissionActionIndicator;
            ueContextModifyReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
            ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.present = \
            UEContextModificationRequestIEs__value_PR_TransmissionActionIndicator;
            ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.choice.TransmissionActionIndicator = \
            TransmissionActionIndicator_restart;
         }

         ieIdx++;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_RRCContainer;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.present = UEContextModificationRequestIEs__value_PR_RRCContainer;
         if(fillDlDcchRrcMsg(ueCb, &rrcContainerTmp, true) != ROK)
         {
            DU_LOG( "\nERROR  -->  F1AP : Failed to fill Rrc reconfiguration buffer");
            return RFAILED;
         }

         rrcContainer = &ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer;
         rrcContainer->size = rrcContainerTmp.size + 2; /* 2 bytes of PDCP SN */
         CU_ALLOC(rrcContainer->buf, rrcContainer->size);
         memset(rrcContainer->buf, 0, rrcContainer->size);
         rrcContainer->buf[0] = 0x00;
         rrcContainer->buf[1] = ueCb->pdcpSn; //PDCP SN
         for(bufIdx = 2, tmpBufIdx = 0; bufIdx < rrcContainer->size; bufIdx++, tmpBufIdx++)
         {
            rrcContainer->buf[bufIdx] = rrcContainerTmp.buf[tmpBufIdx];
         }

         /* RRC delivery status request */
         if(ueCb->state != UE_HANDOVER_IN_PROGRESS)
         {
            ieIdx++;
            ueContextModifyReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_RRCDeliveryStatusRequest;
            ueContextModifyReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_ignore;
            ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.present = UEContextModificationRequestIEs__value_PR_RRCDeliveryStatusRequest;
            ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.choice.RRCDeliveryStatusRequest = RRCDeliveryStatusRequest_true;
         }
      }

#ifdef NR_DRX
      if(DRX_TO_BE_RELEASE && ueCb->drxCfgPresent)
      {
         /* DRX Configuration Indicator */
         ieIdx++;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_DRXConfigurationIndicator;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->criticality = Criticality_ignore;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.present = UEContextModificationRequestIEs__value_PR_DRXConfigurationIndicator;
         ueContextModifyReq->protocolIEs.list.array[ieIdx]->value.choice.DRXConfigurationIndicator = DRXConfigurationIndicator_release;
         ueCb->drxCfgPresent = false;
         memset(&ueCb->drxCfg, 0, sizeof(DrxCfg));
      }
#endif

      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the F1SetupRequest type as APER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf, encBuf);

      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  F1AP : Could not encode ueContextModifyReq structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  F1AP : Created APER encodedbuffer for ueContextModifyReq\n");
#if 0        
         /* This for loop was going into an infinite loop even though encBufSize
          * has a small value. Hence commented this
          */
         for(ieIdx=0; ieIdx< encBufSize; ieIdx++)
         {
            DU_LOG("%x",encBuf[ieIdx]);
         }
#endif
      }

      /* TODO : Hardcoding DU ID to 1 for messages other than F1 Setup Response. This will be made generic in future gerrit */
      if(SendF1APMsg(CU_APP_MEM_REG, CU_POOL, duId) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Sending Ue context modification request  failed");
         break;
      }

      ret = ROK;
      break;

   }
   FreeUeContextModicationRequest(f1apMsg);
   return ret;
}

/*****************************************************************i
 *
* @brief Free memory allocated for UE Context Release Command  
*
* @details
*
*    Function : FreeUeContextReleaseCommand
*
*    Functionality:
*         - Free memory allocated for UE Context Release Command 
*
* @params[in] F1AP_PDU_t *f1apMsg
* @return void
*
* *************************************************************/
void FreeUeContextReleaseCommand(F1AP_PDU_t *f1apMsg)
{
   uint8_t ieIdx;
   UEContextReleaseCommand_t *ueReleaseCommand = NULLP;

   if(f1apMsg)
   {
      if(f1apMsg->choice.initiatingMessage)
      {
         ueReleaseCommand =&f1apMsg->choice.initiatingMessage->value.choice.UEContextReleaseCommand;
         if(ueReleaseCommand->protocolIEs.list.array)
         {
            for(ieIdx=0 ; ieIdx<ueReleaseCommand->protocolIEs.list.count; ieIdx++)
            {
               CU_FREE(ueReleaseCommand->protocolIEs.list.array[ieIdx], sizeof(UEContextReleaseCommand_t));
            }
            CU_FREE(ueReleaseCommand->protocolIEs.list.array, ueReleaseCommand->protocolIEs.list.size);
         }
         CU_FREE(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      }
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
   }
}
/*******************************************************************
 *
 * @brief Builds the Ue Context Release Command 
 *
 * @details
*
*    Function : BuildAndSendUeContextReleaseCommand
*
*    Functionality: Constructs the Ue Context Release Command 
*
* @params[in]
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t BuildAndSendUeContextReleaseCommand(uint32_t duId, uint8_t cuUeF1apId, uint8_t duUeF1apId)
{
   bool       memAllocFailed = false;
   uint8_t  duIdx = 0, ieIdx = 0,elementCnt = 0, ret = RFAILED, bufLen=0;
   DuDb *duDb;
   CuUeCb *ueCb;
   F1AP_PDU_t *f1apMsg = NULLP;
   UEContextReleaseCommand_t *ueContextReleaseCommand = NULLP;

   asn_enc_rval_t         encRetVal;
   DU_LOG("\nINFO  -->  F1AP : Building Ue context release command\n");

   while(true)
   {
      CU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildAndSendUeContextReleaseCommand(): Memory allocation for F1AP-PDU");
         break;
      }

      f1apMsg->present =  F1AP_PDU_PR_initiatingMessage;

      CU_ALLOC(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      if(f1apMsg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildAndSendUeContextReleaseCommand(): Memory allocation for F1AP-PDU failed ");
         break;
      }
      f1apMsg->choice.initiatingMessage->procedureCode = ProcedureCode_id_UEContextRelease;
      f1apMsg->choice.initiatingMessage->criticality = Criticality_reject;
      f1apMsg->choice.initiatingMessage->value.present = InitiatingMessage__value_PR_UEContextReleaseCommand;

      ueContextReleaseCommand =&f1apMsg->choice.initiatingMessage->value.choice.UEContextReleaseCommand;

      SEARCH_DU_DB(duIdx, duId, duDb); 
      ueCb = &duDb->ueCb[duUeF1apId-1];
      if(ueCb->state == UE_HANDOVER_IN_PROGRESS)
         elementCnt = 3;
      else
         elementCnt = 4;
     
      ueContextReleaseCommand->protocolIEs.list.count = elementCnt;
      ueContextReleaseCommand->protocolIEs.list.size = elementCnt*sizeof(UEContextReleaseCommand_t*);

      /* Initialize the UE context modification members */
      CU_ALLOC(ueContextReleaseCommand->protocolIEs.list.array, ueContextReleaseCommand->protocolIEs.list.size);
      if(ueContextReleaseCommand->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildAndSendUeContextReleaseCommand():Memory allocation failed");
         break;
      }

      for(ieIdx=0 ; ieIdx<elementCnt; ieIdx++)
      {
         CU_ALLOC(ueContextReleaseCommand->protocolIEs.list.array[ieIdx], sizeof(UEContextReleaseCommand_t));
         if(ueContextReleaseCommand->protocolIEs.list.array[ieIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  F1AP : BuildAndSendUeContextReleaseCommand(): Memory allocation failed ");
            memAllocFailed = true;  
            break;
         }
      }
      
      if(memAllocFailed == true)
      {
         break;
      }

      ieIdx=0;
      ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;
      ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->value.present = \
      UEContextReleaseCommandIEs__value_PR_GNB_CU_UE_F1AP_ID;
      ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->value.choice.GNB_CU_UE_F1AP_ID =cuUeF1apId;

      ieIdx++;
      ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;
      ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->value.present=\
      UEContextReleaseCommandIEs__value_PR_GNB_DU_UE_F1AP_ID;
      ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_UE_F1AP_ID =duUeF1apId;

      /* Cause of UE context release */
      ieIdx++;
      ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_Cause;
      ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->criticality = Criticality_ignore;
      ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->value.present=\
                                                                            UEContextReleaseCommandIEs__value_PR_Cause;
      ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->value.choice.Cause.present = Cause_PR_radioNetwork;
      ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->value.choice.Cause.choice.radioNetwork=\
                                                                                                     CauseRadioNetwork_normal_release;
      if(ueCb->state != UE_HANDOVER_IN_PROGRESS)
      {
         /* RRC Container for RRC release */
         ieIdx++;
         ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_RRCContainer;
         ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->criticality = Criticality_ignore;
         ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->value.present = \
                                                                                 UEContextReleaseCommandIEs__value_PR_RRCContainer;
         char secModeBuf[7]={ 0x00, 0x05, 0x13, 0x00, 0x00, 0x00, 0x00};
         bufLen =7;
         ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.size = bufLen;
         CU_ALLOC(ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.buf,
               ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.size);
         if(!ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.buf)
         {
            DU_LOG("\nERROR  -->  F1AP : Memory allocation for BuildAndSendUeContextReleaseCommand failed");
            break;
         }
         memset(ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.buf, 0, bufLen);
         memcpy(ueContextReleaseCommand->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.buf, secModeBuf, bufLen);
      }
      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the UE Context Release Command type as APER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
            encBuf);

      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  F1AP : Could not encode Release Command structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  F1AP : Created APER encoded buffer for Ue Context Release Command\n");
         for(ieIdx=0; ieIdx< encBufSize; ieIdx++)
         {
            DU_LOG("%x",encBuf[ieIdx]);
         }
      }

      /* TODO : Hardcoding DU ID to 1 for messages other than F1 Setup Response. This will be made generic in future gerrit */
      if(SendF1APMsg(CU_APP_MEM_REG, CU_POOL, duId) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Sending Ue context Release Command failed");
         break;
      }

      ret = ROK;
      break;

   }
   FreeUeContextReleaseCommand(f1apMsg);
   return ret;
}
/*******************************************************************
*
* @brief process Ue context release request 
*
* @details
*
*    Function : procUeContextReleaseReq 
*
*    Functionality:
*         - process Ue context release request 
*
* @params[in] F1AP_PDU_t *f1apMsg
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t procUeContextReleaseReq(uint32_t duId, F1AP_PDU_t *f1apMsg) 
{
   uint8_t ieIdx=0, duUeF1apId=0,cuUeF1apId=0;

   UEContextReleaseRequest_t *ueReleaseReq = NULLP;
   ueReleaseReq = &f1apMsg->choice.initiatingMessage->value.choice.UEContextReleaseRequest;
   
   for(ieIdx=0; ieIdx < ueReleaseReq->protocolIEs.list.count; ieIdx++)
   {
      switch(ueReleaseReq->protocolIEs.list.array[ieIdx]->id)
      {
         case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
            {
               cuUeF1apId = ueReleaseReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_CU_UE_F1AP_ID;
               break;
            }
         case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
            {
               duUeF1apId = ueReleaseReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_UE_F1AP_ID;
               break;
            }
         default:
              break;
      }
   }

   if(BuildAndSendUeContextReleaseCommand(duId, cuUeF1apId, duUeF1apId) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : procUeContextReleaseReq(): Failed to build Ue Context Release Command ");
      return RFAILED;
   }
   return ROK;
}
/*******************************************************************
*
* @brief processing of Gnb-DU config update 
*
* @details
*
*    Function : procGnbDuUpdate 
*
*    Functionality:
*         - processing of Gnb-DU config update 
*
* @params[in] F1AP_PDU_t *f1apMsg
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t procGnbDuUpdate(uint32_t duId, F1AP_PDU_t *f1apMsg)
{
   bool cellToBeDelete = false;
   uint8_t ieIdx = 0, ueIdx = 0, duIdx = 0, cellIdx=0;
   uint16_t nrCellId;
   DuDb *duDb;
   CuCellCb *cellCb;
   GNBDUConfigurationUpdate_t *duCfgUpdate = NULLP;

   duCfgUpdate = &f1apMsg->choice.initiatingMessage->value.choice.GNBDUConfigurationUpdate;
   for(ieIdx=0; ieIdx < duCfgUpdate->protocolIEs.list.count; ieIdx++)
   {
      switch(duCfgUpdate->protocolIEs.list.array[ieIdx]->id)
      {
         case ProtocolIE_ID_id_TransactionID:
            break;
         case ProtocolIE_ID_id_Served_Cells_To_Modify_List:
            break;
         case ProtocolIE_ID_id_Served_Cells_To_Delete_List:
            {
               struct Served_Cells_To_Delete_ItemIEs *deleteItemIe = \
                  (struct Served_Cells_To_Delete_ItemIEs *)duCfgUpdate->protocolIEs.list.array[ieIdx]->value.choice.\
                  Served_Cells_To_Delete_List.list.array[0];
               bitStringToInt(&deleteItemIe->value.choice.Served_Cells_To_Delete_Item.oldNRCGI.nRCellIdentity, &nrCellId); 
               cellToBeDelete = true;
               break;
            }
         case ProtocolIE_ID_id_gNB_DU_ID:
            break;
      }
   }
   if(BuildAndSendDUUpdateAck(duId) != ROK)
   {
      DU_LOG("ERROR  -->  F1AP : Failed to build and send DUUpdateAck");  
      return RFAILED;
   }
#if 0
   /* We don't require F1 Reset message in Cell Up and Broadcast Procedure flow, So that's why
    * commented this trigger for now */

   if(cellToBeDelete == false)
   {
      DU_LOG("\nINFO  -->  F1AP : Sending F1 reset request");
      if(BuildAndSendF1ResetReq() != ROK)
      {
         DU_LOG("ERROR  -->  F1AP : Failed to build and send F1 reset request");
         return RFAILED;
      }
   }
#endif
   if(cellToBeDelete == true) 
   {
      SEARCH_DU_DB(duIdx, duId, duDb);
      SEARCH_CELL_DB(cellIdx, duDb, nrCellId, cellCb);
      if(cellCb->numUe == 0)
      {
         memset(cellCb, 0, sizeof(CuCellCb));
         duDb->numCells--;
      }
      else
         cellCb->cellStatus = CELL_DELETION_IN_PROGRESS;
   }
   return ROK;
}

/*******************************************************************
*
* @brief storing slice list in CU database
*
* @details
*
*    Function : buildSliceList
*
*    Functionality:
*         - storing slice list in CU database 
*
* @params[in] SliceSupportList_t *sliceSupportList
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t buildSliceList(SliceSupportList_t *sliceSupportList)
{
   uint8_t sliceListIdx = 0;

   if(sliceSupportList)
   {
      if(sliceSupportList->list.array)
      {
         cuCb.numSnssaiSupported = sliceSupportList->list.count;
         for(sliceListIdx=0; sliceListIdx<sliceSupportList->list.count; sliceListIdx++)
         {
            if(sliceSupportList->list.array[sliceListIdx])
            {
               CU_ALLOC(cuCb.snssaiList[sliceListIdx], sizeof(Snssai)); 
               if(cuCb.snssaiList[sliceListIdx] == NULLP)
               {
                   DU_LOG("\nERROR  --> CU_STUB: buildSliceList(): Memory allocation failed");
                   return RFAILED;
               }
               if(sliceSupportList->list.array[sliceListIdx]->sNSSAI.sST.buf)
               {
                  memcpy(&cuCb.snssaiList[sliceListIdx]->sst, sliceSupportList->list.array[sliceListIdx]->\
                  sNSSAI.sST.buf, sliceSupportList->list.array[sliceListIdx]->sNSSAI.sST.size);
               }
               if(sliceSupportList->list.array[sliceListIdx]->sNSSAI.sD->size)
               {
                  memcpy(&cuCb.snssaiList[sliceListIdx]->sd,\
                  sliceSupportList->list.array[sliceListIdx]->sNSSAI.sD->buf,\
                  sliceSupportList->list.array[sliceListIdx]->sNSSAI.sD->size);
               }
            }
         }
      }
   }
   return ROK;
}

/****************************************************************
 * @brief Function to process Srb Setup Mod List 
 *
 * @details
 *
 *    Function : procSrbSetupModList
 *    
 *    Functionality:
 *         - Function to process SRB Setup Mod List
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procSrbSetupModList(CuUeCb *ueCb, SRBs_SetupMod_List_t *srbSetupList)
{
   uint8_t arrIdx = 0, srbIdx;
   struct SRBs_SetupMod_ItemIEs *srbItemIe = NULLP;

   if(srbSetupList != NULLP)
   {
      for(arrIdx = 0; arrIdx < srbSetupList->list.count; arrIdx++)
      {     
         srbItemIe = ((struct SRBs_SetupMod_ItemIEs *)srbSetupList->list.array[arrIdx]);
         if(srbItemIe->value.present == SRBs_SetupMod_ItemIEs__value_PR_SRBs_SetupMod_Item)
         {
            for(srbIdx = 0; srbIdx < ueCb->numSrb; srbIdx++)
            {
               if(ueCb->srbList[srbIdx].srbId == srbItemIe->value.choice.SRBs_SetupMod_Item.sRBID)
               {
                  ueCb->srbList[srbIdx].lcId = srbItemIe->value.choice.SRBs_SetupMod_Item.lCID;
                  break;
               }
            }
         }
        
      }
   }
   return ROK;
}


/****************************************************************
 * @brief Function to process Drb Setup Mod List 
 *
 * @details
 *
 *    Function : procDrbSetupModList
 *    
 *    Functionality:
 *         - Function to process DRB Setup Mod List
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procDrbSetupModList(uint32_t duId, CuUeCb *ueCb, DRBs_SetupMod_List_t *drbSetupList)
{
   uint8_t arrIdx = 0, drbIdx;
   uint32_t teId = 0;
   struct DRBs_SetupMod_ItemIEs *drbItemIe = NULLP;

   if(drbSetupList != NULLP)
   {
      for(arrIdx = 0; arrIdx < drbSetupList->list.count; arrIdx++)
      {
         drbItemIe = ((struct DRBs_SetupMod_ItemIEs *)drbSetupList->list.array[arrIdx]);
         if(drbItemIe->value.present == DRBs_SetupMod_ItemIEs__value_PR_DRBs_SetupMod_Item)
         {
            for(drbIdx = 0; drbIdx < ueCb->numDrb; drbIdx++)
            {
               if(ueCb->drbList[drbIdx].drbId == drbItemIe->value.choice.DRBs_SetupMod_Item.dRBID)
               {
                  if(drbItemIe->value.choice.DRBs_SetupMod_Item.lCID)
                     ueCb->drbList[drbIdx].lcId = *(drbItemIe->value.choice.DRBs_SetupMod_Item.lCID);
                  break;
               }
            }

            if(ueCb->state != UE_HANDOVER_IN_PROGRESS)
            {
               /* extracting teId */
               teId  = extractTeId(&drbItemIe->value.choice.DRBs_SetupMod_Item.dLUPTNLInformation_ToBeSetup_List);
               if(teId > 0)
               {
                  if(addDrbTunnels(duId, teId)== ROK)
                  {
                     DU_LOG("\nDEBUG  --> EGTP: Tunnel Added for TeId %d", teId);
                  }
               }
               else
                  return RFAILED;
            }
         }
      }
   }
   return ROK;
}

/*******************************************************************
*
* @brief processing of GNB_DU_Served_Cells Plmn list  information
*
* @details
*
*    Function : procServedCellPlmnList
*
*    Functionality:
*         - processing of GNB_DU_Served_Cells Plmn list information for storing
*         SNSSAI list
*
* @params[in] F1AP_PDU_t *f1apMsg
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t procServedCellPlmnList(ServedPLMNs_List_t *srvPlmn)
{
   uint8_t srvPlmnIdx=0, ieExtensionsLstIdx=0;
   ProtocolExtensionContainer_4624P3_t **ieExtend;

   if(srvPlmn->list.array)
   {
      for(srvPlmnIdx=0; srvPlmnIdx<srvPlmn->list.count; srvPlmnIdx++)
      {
         if(srvPlmn->list.array[srvPlmnIdx])
         {
            ieExtend =  &srvPlmn->list.array[srvPlmnIdx]->iE_Extensions;
            if(*ieExtend)
            {
               if((*ieExtend)->list.array)
               {
                  for(ieExtensionsLstIdx = 0; ieExtensionsLstIdx<(*ieExtend)->list.count; ieExtensionsLstIdx++)
                  {
                     if((*ieExtend)->list.array[ieExtensionsLstIdx])
                     {
                        switch((*ieExtend)->list.array[ieExtensionsLstIdx]->id )
                        {
                           case ProtocolIE_ID_id_TAISliceSupportList:
                              {
                                 if(buildSliceList(&(*ieExtend)->list.array[ieExtensionsLstIdx]->\
                                          extensionValue.choice.SliceSupportList) != ROK)
                                 {
                                    DU_LOG("\nERROR  --> CU_STUB: procServedCellPlmnList(): Failed to build slice List");
                                    return RFAILED;
                                 }
                              }
                        }
                     }
                  }
               }
            }
         }
      }
   }
   return ROK;
}

/****************************************************************
 * @brief Function to process Ue Context Modification Response 
 *
 * @details
 *
 *    Function : procUeContextModificationResponse
 *    
 *    Functionality:
 *         - Function to process Ue Context Modification Response
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procUeContextModificationResponse(uint32_t duId, F1AP_PDU_t *f1apMsg, char *recvBuf, MsgLen recvBufLen)
{
   uint8_t idx=0, duIdx=0;
   uint8_t duUeF1apId = 0, cuUeF1apId = 0;
   DuDb *duDb = NULLP;
   CuUeCb *ueCb = NULLP;
   UEContextModificationResponse_t *ueCtxtModRsp = NULLP;

   SEARCH_DU_DB(duIdx, duId, duDb);
   ueCtxtModRsp = &f1apMsg->choice.successfulOutcome->value.choice.UEContextModificationResponse;

   /* In case of Inter-CU Handover request received from peer CU */
   if(duDb->tempUeCtxtInHo)
      ueCb = duDb->tempUeCtxtInHo;
   
   for(idx=0; idx < ueCtxtModRsp->protocolIEs.list.count; idx++)
   {
      switch(ueCtxtModRsp->protocolIEs.list.array[idx]->id)
      {
          case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
             {
                if(ueCb == NULLP)
                {
                   cuUeF1apId = ueCtxtModRsp->protocolIEs.list.array[idx]->value.choice.GNB_CU_UE_F1AP_ID;
                }
                else
                {
                   /* In case of Inter-CU Handover request received from peer CU */
                   cuUeF1apId = ueCb->gnbCuUeF1apId;
                }
                break;
             }
          case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
             {
                if(!ueCb)
                {
                   duUeF1apId = ueCtxtModRsp->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID;
                   ueCb = &duDb->ueCb[duUeF1apId-1];

                   /* In case UE context modification response is received at source GNB CU from source GNB DU 
                    * for a UE in handover, send HO request to target GNB only if not sent already.
                    * If HO Req is already sent to target GNB, and an HO Req Ack is received, then 
                    * ueCb->hoInfo.cuUeF1apIdTgt will be non-zero
                    */
                   if((ueCb->state == UE_HANDOVER_IN_PROGRESS) && (ueCb->hoInfo.HOType == Xn_Based_Inter_CU_HO) && \
                      (ueCb->hoInfo.cuUeF1apIdTgt == 0))
                   {
                      BuildAndSendHOReq(ueCb, recvBuf, recvBufLen);
                   }
                }
                break;
             }
          case ProtocolIE_ID_id_DRBs_SetupMod_List:
             {
                /* Adding Tunnels for successful DRB */
                procDrbSetupModList(duId, ueCb, &ueCtxtModRsp->protocolIEs.list.array[idx]->value.choice.DRBs_SetupMod_List);
                break; 

             }
             case ProtocolIE_ID_id_DRBs_Modified_List:
             {
                DU_LOG("\nINFO  -->  Received DRBs Modified List");
                break;
             }
          case ProtocolIE_ID_id_SRBs_SetupMod_List:
             {
                procSrbSetupModList(ueCb, &ueCtxtModRsp->protocolIEs.list.array[idx]->value.choice.SRBs_SetupMod_List);
                break;
             }
         case ProtocolIE_ID_id_DUtoCURRCInformation:
             {
                DU_LOG("\nINFO  -->  Received Du to Cu RRC Information ");
                if((extractDuToCuRrcCont(ueCb, ueCtxtModRsp->protocolIEs.list.array[idx]->value.choice.\
                   DUtoCURRCInformation.cellGroupConfig)) != ROK)
                {
                   DU_LOG("\nERROR  -->  F1AP : Failed to extract Du to Cu RRC Information");
                   return RFAILED;
                }
                break;
             }

      }
   }

   /* If UE is in handover and UE context is not yet created at target DU, then send
    * UE context setup request to target DU */
   if(ueCb->state == UE_HANDOVER_IN_PROGRESS)
   {
      uint8_t ueIdx = 0;
      uint8_t tgtDuId = 0;
      DuDb *tgtDuDb = NULLP;
      CuUeCb *ueCbInTgtDu = NULLP;

      if(ueCb->hoInfo.HOType == Inter_DU_HO)
      {
         tgtDuId = ueCb->hoInfo.tgtNodeId;
      }
      else if (ueCb->hoInfo.HOType == Xn_Based_Inter_CU_HO)
      {
         tgtDuId =  duId;
      }

      SEARCH_DU_DB(duIdx, tgtDuId, tgtDuDb);
      if(tgtDuDb)
      {
         /* Since DU UE F1AP ID assigned by target DU to this UE in handover is
          * not known here, using CU UE F1AP ID to search for UE Cb in target DU
          * DB */
         for(ueIdx = 0; ueIdx < MAX_NUM_UE; ueIdx++)
         {
            if(tgtDuDb->ueCb[ueIdx].gnbCuUeF1apId == cuUeF1apId)
            {
               ueCbInTgtDu = &tgtDuDb->ueCb[ueIdx];
               break;
            }
         }

         /* If UE context is not found in Target DU, send UE context setup
          * request */
         if(ueCbInTgtDu == NULLP)
         {
            if((BuildAndSendUeContextSetupReq(tgtDuId, ueCb)) != ROK)
            {
               DU_LOG("\nERROR  ->  F1AP : Failed at BuildAndSendUeContextSetupReq");
               return RFAILED;
            }
         }
      }
   }

#ifdef START_DL_UL_DATA
   startDlData();
#endif

   return ROK;
}

/*******************************************************************
*
* @brief processing of F1 setup request
*
* @details
*
*    Function : procF1SetupReq 
*
*    Functionality:
*         - processing of  F1 setup request
*
* @params[in] F1AP_PDU_t *f1apMsg
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
void procF1SetupReq(uint32_t *destDuId, F1AP_PDU_t *f1apMsg)
{
   uint8_t ieIdx = 0, plmnidx=0, duIdx = 0, ret=ROK, cellIdx = 0;
   uint32_t duId = 0;
   uint64_t nrCellId = 0;
   DuDb     *duDb = NULLP;
   CuCellCb *cellCb = NULLP;
   BIT_STRING_t nrcellIdentity;
   F1SetupRequest_t *f1SetupReq = NULLP;
   GNB_DU_Served_Cells_Item_t *srvCellItem = NULLP; 
   GNB_DU_Served_Cells_List_t *duServedCell = NULLP;

   /* Triggering XN setup request before F1 setup establishment */
   if(LOCAL_NODE_TYPE == CLIENT)
      BuildAndSendXnSetupReq();

   f1SetupReq = &f1apMsg->choice.initiatingMessage->value.choice.F1SetupRequest;
   for(ieIdx=0; ieIdx < f1SetupReq->protocolIEs.list.count; ieIdx++)
   {
      switch(f1SetupReq->protocolIEs.list.array[ieIdx]->id)
      {
         case ProtocolIE_ID_id_gNB_DU_ID:
           {
              duId = f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_ID.buf[0];
              SEARCH_DU_DB(duIdx, duId, duDb); 
              if(duDb == NULLP)
              {
                 duDb = &cuCb.duInfo[cuCb.numDu];
                 cuCb.numDu++;
              }
              memset(duDb, 0, sizeof(DuDb));
              duDb->duId = duId;
              *destDuId = duId;
              break;
           }
         case ProtocolIE_ID_id_gNB_DU_Name:
           {
              strcpy((char *)duDb->duName, (char*)f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_Name.buf);
              break;
           }
         case ProtocolIE_ID_id_gNB_DU_Served_Cells_List:
           {
               duServedCell = &f1SetupReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_Served_Cells_List;
               if(duServedCell->list.array)
               {
                  for(plmnidx=0; plmnidx<duServedCell->list.count; plmnidx++)
                  {
                     if(duServedCell->list.array[plmnidx])
                     {
                        switch(duServedCell->list.array[plmnidx]->id)
                        {
                           case ProtocolIE_ID_id_GNB_DU_Served_Cells_Item:
                           {
                              srvCellItem =  &duServedCell->list.array[plmnidx]->value.choice.GNB_DU_Served_Cells_Item;
                              ret = procServedCellPlmnList(&srvCellItem->served_Cell_Information.servedPLMNs);
                              memcpy(&nrcellIdentity, &srvCellItem->served_Cell_Information.nRCGI.nRCellIdentity, sizeof(BIT_STRING_t));
                              
                              bitStringToInt(&nrcellIdentity, &nrCellId);
                              SEARCH_CELL_DB(cellIdx, duDb, nrCellId, cellCb);
                              if(cellCb == NULLP)
                              {
                                 cellCb = &duDb->cellCb[duDb->numCells];
                                 memset(cellCb, 0, sizeof(CuCellCb));
                                 cellCb->nrCellId = nrCellId;
                                 cellCb->cellStatus = CELL_ACTIVE;
                                 duDb->numCells++;
                              }
                           }
                        }
                     }
                  }
               }
           }
      }
   }
   if(ret == ROK)
   {
      BuildAndSendF1SetupRsp(duId, &nrcellIdentity);
   }
   else
   {
       DU_LOG("\nERROR  --> CU_STUB: procF1SetupReq(): Failed to process F1 setup request");
   }
}

/****************************************************************
*
* @brief processing of UE Context Release Complete
*
* @details
*
*    Function : procUeContextReleaseComplete
*
*    Functionality:
*         - processing of UE Context Release Complete
*
* @params[in] F1AP_PDU_t *f1apMsg
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
void procUeContextReleaseComplete(uint32_t duId, F1AP_PDU_t *f1apMsg)
{
   uint8_t duIdx = 0, ieIdx = 0, ueIdx = 0, drbIdx = 0;
   uint8_t gnbDuUeF1apId = 0, gnbCuUeF1apId = 0;
   DuDb *duDb = NULLP;
   CuUeCb *ueCb = NULLP;
   UEContextReleaseComplete_t *ueReleaseComplete = NULLP;

   SEARCH_DU_DB(duIdx, duId, duDb);
   if(!duDb)
   {
      DU_LOG("\nERROR  -->  F1AP : No entry found for DU ID [%d]", duId);
      return;
   }

   ueReleaseComplete = &f1apMsg->choice.successfulOutcome->value.choice.UEContextReleaseComplete;
   for(ieIdx=0; ieIdx < ueReleaseComplete->protocolIEs.list.count; ieIdx++)
   {
      switch(ueReleaseComplete->protocolIEs.list.array[ieIdx]->id)
      {
         case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
            {
               gnbCuUeF1apId = ueReleaseComplete->protocolIEs.list.array[ieIdx]->value.choice.GNB_CU_UE_F1AP_ID;
               break;
            }
         case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
            {
               gnbDuUeF1apId = ueReleaseComplete->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_UE_F1AP_ID;
               ueCb = &duDb->ueCb[gnbDuUeF1apId-1];
               for(ueIdx = 0; ueIdx <MAX_NUM_UE; ueIdx++)
               {
                  if(ueCb->cellCb && ueCb->cellCb->ueCb[ueIdx])
                  {
                     if((ueCb->cellCb->ueCb[ueIdx]->gnbCuUeF1apId == gnbCuUeF1apId) &&
                           (ueCb->cellCb->ueCb[ueIdx]->gnbDuUeF1apId == gnbDuUeF1apId))
                     {
                        for(drbIdx = 0; drbIdx < ueCb->numDrb; drbIdx++)
                        {
                           deleteEgtpTunnel(duId, ueCb->drbList[drbIdx].dlUpTnlInfo.teId);
                        }
                        ueCb->cellCb->ueCb[ueIdx] = NULLP;
                        ueCb->cellCb->numUe--;
                        if((ueCb->cellCb->numUe == 0) && (ueCb->cellCb->cellStatus == CELL_DELETION_IN_PROGRESS))
                        {
                           memset(ueCb->cellCb, 0, sizeof(CuCellCb));
                           duDb->numCells--;
                        }
                        break;
                     }
                  }
               }
               memset(ueCb, 0, sizeof(CuUeCb));
               duDb->numUe--;
               break;
            }
      }
   }
}

/*******************************************************************
 *
 * @brief Builds the Paging cell list 
 *
 * @details
 *
 *    Function : BuildPagingCellList
 *
 *    Functionality: Build the paging cell list 
 *
 * @params[in] PagingCell_list_t  *pagingCelllist,  
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildPagingCellList(PagingCell_list_t *pagingCelllist, uint8_t numCells, CuCellCb *cellCb)
{
   uint8_t cellIdx =0;
   PagingCell_ItemIEs_t *pagingCellItemIes; 
   PagingCell_Item_t *pagingCellItem;

   pagingCelllist->list.count = numCells;
   pagingCelllist->list.size = pagingCelllist->list.count * (sizeof(PagingCell_ItemIEs_t*));
   CU_ALLOC(pagingCelllist->list.array, pagingCelllist->list.size);
   if(pagingCelllist->list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : BuildPagingCellList(): Memory allocation failed ");
      return RFAILED;
   }

   for(cellIdx = 0; cellIdx < pagingCelllist->list.count; cellIdx++)
   {
      CU_ALLOC(pagingCelllist->list.array[cellIdx], sizeof(PagingCell_ItemIEs_t));
      if(pagingCelllist->list.array[cellIdx] == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildPagingCellList(): Memory allocation failed ");
         return RFAILED;
      }
   }
   
   for(cellIdx = 0; cellIdx < pagingCelllist->list.count; cellIdx++)
   {
      pagingCellItemIes = (PagingCell_ItemIEs_t *)pagingCelllist->list.array[cellIdx];
      pagingCellItemIes->id =  ProtocolIE_ID_id_PagingCell_Item;
      pagingCellItemIes->criticality = Criticality_ignore;
      pagingCellItemIes->value.present = PagingCell_ItemIEs__value_PR_PagingCell_Item; 
      pagingCellItem = &pagingCellItemIes->value.choice.PagingCell_Item;
   
      /* Fill NrCgi Information */
      BuildNrcgi(&pagingCellItem->nRCGI, cellCb[cellIdx].nrCellId);
   }
   
   return ROK;
}

/*******************************************************************
 *
 * @brief Deallocation of memory allocated in paging msg
 *
 * @details
 *
 *    Function :FreePagingMsg 
 *
 *    Functionality: Deallocation of memory allocated in paging msg
 *
 * @params[in] F1AP_PDU_t *f1apMsg
 *
 * @return void 
 *
 * ****************************************************************/
void FreePagingMsg(F1AP_PDU_t *f1apMsg)
{
   uint8_t ieIdx, cellIdx;
   Paging_t   *paging;
   PagingCell_ItemIEs_t *pagingCellItemIes;
   PagingCell_Item_t *pagingCellItem;
   PagingCell_list_t  *pagingCelllist;

   if(f1apMsg)
   {
      if(f1apMsg->choice.initiatingMessage)
      {
         paging = &f1apMsg->choice.initiatingMessage->value.choice.Paging;
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
                        CU_FREE(paging->protocolIEs.list.array[ieIdx]->value.choice.UEIdentityIndexValue.choice.indexLength10.buf,\
                        paging->protocolIEs.list.array[ieIdx]->value.choice.UEIdentityIndexValue.choice.indexLength10.size);
                        break;
                     }
                     
                     case ProtocolIE_ID_id_PagingIdentity:
                     {
                        if(paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.present == PagingIdentity_PR_cNUEPagingIdentity)
                        {
                           if(paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.choice.cNUEPagingIdentity)
                           {  
                              if(paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.choice.cNUEPagingIdentity->present == CNUEPagingIdentity_PR_fiveG_S_TMSI)
                              {
                                 CU_FREE(paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.choice.cNUEPagingIdentity->choice.fiveG_S_TMSI.buf,\
                                 paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.choice.cNUEPagingIdentity->choice.fiveG_S_TMSI.size);
                              }
                                CU_FREE(paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.choice.cNUEPagingIdentity,\
                                sizeof(struct CNUEPagingIdentity));
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
                                     CU_FREE(pagingCellItem->nRCGI.pLMN_Identity.buf, pagingCellItem->nRCGI.pLMN_Identity.size);
                                     CU_FREE(pagingCellItem->nRCGI.nRCellIdentity.buf, pagingCellItem->nRCGI.nRCellIdentity.size);
                                  }
                                  CU_FREE(pagingCelllist->list.array[cellIdx], sizeof(PagingCell_ItemIEs_t));
                              }
                           }
                           CU_FREE(pagingCelllist->list.array, pagingCelllist->list.size);
                        }
                        break;
                     }
                  }
                  CU_FREE(paging->protocolIEs.list.array[ieIdx], sizeof(Paging_t));
               }
            }
            CU_FREE(paging->protocolIEs.list.array, paging->protocolIEs.list.size);
         }
         CU_FREE(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      }
      CU_FREE(f1apMsg, sizeof(F1AP_PDU_t));
   }
}
/*******************************************************************
 *
 * @brief Builds and sends the paging message if UE is in idle mode
 *
 * @details
 *
 *    Function : BuildAndSendPagingMsg
 *
 *    Functionality: Builds and sends the paging message
 *
 * @params[in] uint32_t duId, uint8_t gsTmsi
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendPagingMsg(uint64_t gsTmsi, uint8_t duId)
{
   bool       memAllocFailed = false;
   uint8_t    ieIdx = 0, elementCnt = 0, ret = RFAILED;
   uint16_t   ueId = 0, duIdx = 0;

   /*As per 38.473 Sec 9.3.1.39, UE Identity Index Value (10bits) > 2 Bytes + 6 Unused Bits
    *5G-S-TMSI :48 Bits  >> 6 Bytes and 0 UnusedBits */
   uint8_t    totalByteInUeId = 2, totalByteInTmsi = 6;
   uint8_t    unusedBitsInUeId = 6, unusedBitsInTmsi = 0;

   F1AP_PDU_t *f1apMsg = NULLP;
   Paging_t   *paging = NULLP;
   DuDb       *duDb;
   asn_enc_rval_t         encRetVal;

   DU_LOG("\nINFO  -->  F1AP : Building PAGING Message command\n");

   SEARCH_DU_DB(duIdx, duId, duDb);
   if(duDb == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : BuildAndSendPagingMsg(): DuDb is empty");
      return ret; 
   }
   if(duDb->numCells == 0)
   {
      DU_LOG("\nERROR  -->  F1AP : BuildAndSendPagingMsg(): No CELL is UP!");
      return ret; 
   }

   while(true)
   {
      CU_ALLOC(f1apMsg, sizeof(F1AP_PDU_t));
      if(f1apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildAndSendPagingMsg(): Memory allocation for F1AP-PDU");
         break;
      }

      f1apMsg->present =  F1AP_PDU_PR_initiatingMessage;

      CU_ALLOC(f1apMsg->choice.initiatingMessage, sizeof(InitiatingMessage_t));
      if(f1apMsg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildAndSendPagingMsg(): Memory allocation for F1AP-PDU failed ");
         break;
      }
      f1apMsg->choice.initiatingMessage->procedureCode = ProcedureCode_id_Paging;
      f1apMsg->choice.initiatingMessage->criticality = Criticality_reject;
      f1apMsg->choice.initiatingMessage->value.present = InitiatingMessage__value_PR_Paging;

      paging = &f1apMsg->choice.initiatingMessage->value.choice.Paging;

      elementCnt = 5;
      paging->protocolIEs.list.count = elementCnt;
      paging->protocolIEs.list.size = elementCnt * sizeof(Paging_t*);

      /* Initialize the Paging Message members */
      CU_ALLOC(paging->protocolIEs.list.array, paging->protocolIEs.list.size);
      if(paging->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildAndSendPagingMsg():Memory allocation failed");
         break;
      }

      for(ieIdx=0 ; ieIdx<elementCnt; ieIdx++)
      {
         CU_ALLOC(paging->protocolIEs.list.array[ieIdx], sizeof(Paging_t));
         if(paging->protocolIEs.list.array[ieIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  F1AP : BuildAndSendPagingMsg(): Memory allocation failed ");
            memAllocFailed = true;  
            break;
         }
      }

      if(memAllocFailed == true)
      {
         break;
      }

      /* UE Identity Index Value */
      ieIdx=0;
      paging->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_UEIdentityIndexValue;
      paging->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      paging->protocolIEs.list.array[ieIdx]->value.present = PagingIEs__value_PR_UEIdentityIndexValue;
      paging->protocolIEs.list.array[ieIdx]->value.choice.UEIdentityIndexValue.present = UEIdentityIndexValue_PR_indexLength10;
      paging->protocolIEs.list.array[ieIdx]->value.choice.UEIdentityIndexValue.choice.indexLength10.size = totalByteInUeId*sizeof(uint8_t);
      CU_ALLOC(paging->protocolIEs.list.array[ieIdx]->value.choice.UEIdentityIndexValue.choice.indexLength10.buf,\
            paging->protocolIEs.list.array[ieIdx]->value.choice.UEIdentityIndexValue.choice.indexLength10.size);
      if(paging->protocolIEs.list.array[ieIdx]->value.choice.UEIdentityIndexValue.choice.indexLength10.buf == NULLP)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildAndSendPagingMsg(): Memory allocation failed ");
         break;
      }

      /*As per 3gpp Spec 38.304 Sec 7.1: UE_ID: 5G-S-TMSI mod 1024*/
      ueId = gsTmsi % 1024;
      fillBitString(&paging->protocolIEs.list.array[ieIdx]->value.choice.UEIdentityIndexValue.choice.indexLength10, unusedBitsInUeId, totalByteInUeId, ueId);

      /* Paging Identity */
      ieIdx++;
      paging->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_PagingIdentity;
      paging->protocolIEs.list.array[ieIdx]->criticality = Criticality_reject;
      paging->protocolIEs.list.array[ieIdx]->value.present = PagingIEs__value_PR_PagingIdentity;
      paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.present = \
                                                                                   PagingIdentity_PR_cNUEPagingIdentity;
      CU_ALLOC(paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.choice.cNUEPagingIdentity, \
            sizeof(struct CNUEPagingIdentity));
      if(!paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.choice.cNUEPagingIdentity)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildAndSendPagingMsg(): Memory allocation failed ");
         break;
      }

      paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.choice.cNUEPagingIdentity->present = \
                                                                                                              CNUEPagingIdentity_PR_fiveG_S_TMSI;

      paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.choice.cNUEPagingIdentity->choice.fiveG_S_TMSI.size = totalByteInTmsi*sizeof(uint8_t);
      CU_ALLOC(paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.choice.cNUEPagingIdentity->choice.fiveG_S_TMSI.buf,\
            paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.choice.cNUEPagingIdentity->choice.fiveG_S_TMSI.size);
      if(!paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.choice.cNUEPagingIdentity->choice.fiveG_S_TMSI.buf)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildAndSendPagingMsg(): Memory allocation failed ");
         break;
      }

      fillBitString(&paging->protocolIEs.list.array[ieIdx]->value.choice.PagingIdentity.choice.cNUEPagingIdentity->choice.fiveG_S_TMSI,\
            unusedBitsInTmsi, totalByteInTmsi, gsTmsi);

      /* Paging Drx */
      ieIdx++;
      paging->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_PagingDRX;
      paging->protocolIEs.list.array[ieIdx]->criticality = Criticality_ignore;
      paging->protocolIEs.list.array[ieIdx]->value.present = PagingIEs__value_PR_PagingDRX;
      paging->protocolIEs.list.array[ieIdx]->value.choice.PagingDRX = PagingDRX_v32;

      /* Paging Priority */
      ieIdx++;
      paging->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_PagingPriority;
      paging->protocolIEs.list.array[ieIdx]->criticality = Criticality_ignore;
      paging->protocolIEs.list.array[ieIdx]->value.present = PagingIEs__value_PR_PagingPriority;
      paging->protocolIEs.list.array[ieIdx]->value.choice.PagingPriority = PagingPriority_priolevel2;

      /* Paging Cell List */
      ieIdx++;
      paging->protocolIEs.list.array[ieIdx]->id = ProtocolIE_ID_id_PagingCell_List;
      paging->protocolIEs.list.array[ieIdx]->criticality = Criticality_ignore;
      paging->protocolIEs.list.array[ieIdx]->value.present = PagingIEs__value_PR_PagingCell_list;
      if(BuildPagingCellList(&paging->protocolIEs.list.array[ieIdx]->value.choice.PagingCell_list, duDb->numCells, duDb->cellCb) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : BuildAndSendPagingMsg(): Failed to build Paging cell list "); 
         break;
      }

      xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

      /* Encode the UE Context Release Command type as APER */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_F1AP_PDU, 0, f1apMsg, PrepFinalEncBuf,\
            encBuf);

      /* Encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  F1AP : Could not encode Release Command structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  F1AP : Created APER encoded buffer for Paging\n");
         for(ieIdx=0; ieIdx< encBufSize; ieIdx++)
         {
            DU_LOG("%x",encBuf[ieIdx]);
         }
      }

      if(SendF1APMsg(CU_APP_MEM_REG, CU_POOL, duId) != ROK)
      {
         DU_LOG("\nERROR  -->  F1AP : Sending Ue context Release Command failed");
         break;
      }

      ret = ROK;
      break;

   }

   FreePagingMsg(f1apMsg); 
   return ret;
}

/*******************************************************************
 *
 * @brief Decode received character string into F1AP message 
 *
 * @details
 *
 *    Function : F1APMsgHdlr
 *
 *    Functionality:
 *         - Decodes received F1AP control message
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t F1APDecodeMsg(F1AP_PDU_t *f1apMsg, Buffer *mBuf, char **recvBuf, MsgLen *recvBufLen)
{
   int i;
   MsgLen copyCnt;
   asn_dec_rval_t rval; /* Decoder return value */

   /* Copy mBuf into char array to decode it */
   ODU_GET_MSG_LEN(mBuf, recvBufLen);
   CU_ALLOC(*recvBuf, (Size)(*recvBufLen));
   if(*recvBuf == NULLP)
   {
      DU_LOG("\nERROR  -->  F1AP : Memory allocation failed");
      return RFAILED;
   }
   if(ODU_COPY_MSG_TO_FIX_BUF(mBuf, 0, *recvBufLen, (Data *)*recvBuf, &copyCnt) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : Failed while copying %d", copyCnt);
      return RFAILED;
   }

   DU_LOG("\nDEBUG  -->  F1AP : Received flat buffer to be decoded : \n");
   for(i=0; i< *recvBufLen; i++)
   {
      DU_LOG("%x ",(*recvBuf)[i]);
   }

   /* Decoding flat buffer into F1AP messsage */
   rval = aper_decode(0, &asn_DEF_F1AP_PDU, (void **)&f1apMsg, *recvBuf, *recvBufLen, 0, 0);
   if(rval.code == RC_FAIL || rval.code == RC_WMORE)
   {
      DU_LOG("\nERROR  -->  F1AP : ASN decode failed");
      return RFAILED;
   }

   /* Printing the decoded F1AP PDU */
   DU_LOG("\n");
   xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);
   return ROK;
}

/*******************************************************************
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
void F1APMsgHdlr(uint32_t *duId, Buffer *mBuf)
{
   char *recvBuf;
   MsgLen recvBufLen;
   F1AP_PDU_t *f1apMsg = NULLP;
   F1AP_PDU_t f1apasnmsg ;

   DU_LOG("\nINFO  -->  F1AP : Received F1AP message buffer");
   ODU_PRINT_MSG(mBuf, 0,0);

   f1apMsg = &f1apasnmsg;
   memset(f1apMsg, 0, sizeof(F1AP_PDU_t));
   if(F1APDecodeMsg(f1apMsg, mBuf, &recvBuf, &recvBufLen) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : F1AP PDU decode failed");
      return;
   }

   switch(f1apMsg->present)
   {
      case F1AP_PDU_PR_initiatingMessage:
         {
            switch(f1apMsg->choice.initiatingMessage->value.present)
            {
               case InitiatingMessage__value_PR_Reset:
                  {
                     DU_LOG("\nINFO  -->  F1AP : F1 reset request received ");
                     BuildAndSendF1ResetAck();
                     break;
                  }

               case InitiatingMessage__value_PR_F1SetupRequest:
                  {
                     DU_LOG("\nINFO  -->  F1AP : F1 setup request received");
                     procF1SetupReq(duId, f1apMsg);
                     break;
                  }

               case InitiatingMessage__value_PR_GNBDUConfigurationUpdate:
                  {
                     DU_LOG("\nINFO  -->  F1AP : GNB-DU config update received");
                     procGnbDuUpdate(*duId, f1apMsg);
                     break;
                  }
               case InitiatingMessage__value_PR_InitialULRRCMessageTransfer:
                  {
                     DU_LOG("\nINFO  -->  F1AP : Received InitialULRRCMessageTransfer");
                     procInitULRRCMsg(*duId, f1apMsg);
                     break;
                  }
               case InitiatingMessage__value_PR_ULRRCMessageTransfer:
                  {
                     DU_LOG("\nINFO  -->  F1AP : Received ULRRCMessageTransfer");
                     procUlRrcMsg(*duId, f1apMsg);
                     break;
                  }

               case InitiatingMessage__value_PR_RRCDeliveryReport:
                  {
                     DU_LOG("\nINFO  -->  F1AP : Received RRC delivery report");
                     break;
                  }
               case InitiatingMessage__value_PR_UEContextReleaseRequest:
                  {
                     DU_LOG("\nINFO  -->  F1AP : Received UE Context Release Request");
                     procUeContextReleaseReq(*duId, f1apMsg);
                     break;
                  }
               default:
                  {
                     DU_LOG("\nERROR  -->  F1AP : Invalid type of intiating message [%d]",\
                           f1apMsg->choice.initiatingMessage->value.present);
                     return;
                  }
            }/* End of switch(initiatingMessage) */
            break;
         }

      case F1AP_PDU_PR_successfulOutcome:
         {
            switch(f1apMsg->choice.successfulOutcome->value.present)
            {
               case SuccessfulOutcome__value_PR_ResetAcknowledge:
                  {
                     DU_LOG("\nINFO  -->  F1Reset Acknowledgement is received successfully ");
                     break;
                  }
               case SuccessfulOutcome__value_PR_UEContextSetupResponse:
                  {
                     DU_LOG("\nINFO  -->  F1AP : UE ContextSetupResponse received");
                     procUeContextSetupResponse(*duId, f1apMsg, recvBuf, recvBufLen);
                     break;
                  }
               case SuccessfulOutcome__value_PR_UEContextModificationResponse:
                  {
                     DU_LOG("\nINFO  -->  F1AP : UE Context Modification Response received");
                     procUeContextModificationResponse(*duId, f1apMsg, recvBuf, recvBufLen);
                     break;
                  }
               case SuccessfulOutcome__value_PR_UEContextReleaseComplete:
                  {
                      DU_LOG("\nINFO  -->  F1AP : UE Context release complete received");
                      procUeContextReleaseComplete(*duId, f1apMsg);
                      break;
                  }
               default:
                  {
                     DU_LOG("\nERROR  -->  F1AP : Invalid type of successful outcome message [%d]",\
                           f1apMsg->choice.successfulOutcome->value.present);
                     return;
                  }
            }/* End of switch(successfulOutcome) */
            break;
         } 
      default:
         {
            DU_LOG("\nERROR  -->  F1AP : Invalid type of f1apMsg->present [%d]",f1apMsg->present);
            return;
         }
   }/* End of switch(f1apMsg->present) */

   CU_FREE(recvBuf, (Size)(recvBufLen));
} /* End of F1APMsgHdlr */

/**********************************************************************
  End of file
 **********************************************************************/
