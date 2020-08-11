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

/*This file contains stub for PHY to handle messages to/from MAC CL */

#include "common_def.h"
#include "lrg.h"
#include "lrg.x"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "lwr_mac.h"
#include "lwr_mac_phy.h"
#ifdef INTEL_FAPI
#include "fapi.h"
#endif
#include "lphy_stub.h"
#include "lwr_mac_upr_inf.h"
#include "mac.h"

#define MAX_SLOT_VALUE   9
#define MAX_SFN_VALUE    1023
#define NR_PCI            1

uint16_t sfnValue = 0;
uint16_t slotValue = 0;
bool     rachIndSent = false;

EXTERN void phyToMac ARGS((uint16_t msgType, uint32_t msgLen,void *msg));
#ifdef INTEL_FAPI
EXTERN void fillTlvs ARGS((fapi_uint16_tlv_t *tlv, uint16_t tag, uint16_t
length, uint16_t value, uint32_t *msgLen));
EXTERN void fillMsgHeader ARGS((fapi_msg_t *hdr, uint16_t msgType, uint16_t msgLen));
#endif
EXTERN void procPhyMessages(uint16_t msgType, uint32_t msgSize, void *msg);

/*******************************************************************
 *
 * @brief Builds and sends param response to MAC CL
 *
 * @details
 *
 *    Function : l1BldAndSndParamRsp
 *
 *    Functionality:
 *          - Builds and sends param response to MAC
 *
 * @params[in] Config request message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 l1BldAndSndParamRsp(void *msg)
{
#ifdef INTEL_FAPI
   uint8_t index = 0;
   uint32_t msgLen = 0;
   fapi_param_resp_t *fapiParamRsp;

   MAC_ALLOC(fapiParamRsp, sizeof(fapi_param_resp_t));
   if(!fapiParamRsp)
   {
      DU_LOG("PHY STUB: Memory allocation failed");
      return RFAILED;
   }

   /* Cell Params */
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_RELEASE_CAPABILITY_TAG,			        sizeof(uint16_t), 1, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PHY_STATE_TAG,				        sizeof(uint16_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_SKIP_BLANK_DL_CONFIG_TAG,		        sizeof(uint8_t),  0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_SKIP_BLANK_UL_CONFIG_TAG,			sizeof(uint8_t),  0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_NUM_CONFIG_TLVS_TO_REPORT_TYPE_TAG,              sizeof(uint8_t),  0, &msgLen);

   /* Carrier Params */
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_CYCLIC_PREFIX_TAG,                               sizeof(uint8_t),  1, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_SUPPORTED_SUBCARRIER_SPACING_DL_TAG,	        sizeof(uint8_t),  1, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_SUPPORTED_BANDWIDTH_DL_TAG,			sizeof(uint16_t), 1, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_SUPPORTED_SUBCARRIER_SPACING_UL_TAG,		sizeof(uint8_t),  0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_SUPPORTED_BANDWIDTH_UL_TAG,			sizeof(uint16_t), 0, &msgLen);

   /* PDCCH Param*/
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_CCE_MAPPING_TYPE_TAG,   			        sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_CORESET_OUTSIDE_FIRST_3_OFDM_SYMS_OF_SLOT_TAG,   sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PRECODER_GRANULARITY_CORESET_TAG,		sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDCCH_MU_MIMO_TAG,  				sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDCCH_PRECODER_CYCLING_TAG,			sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_MAX_PDCCHS_PER_SLOT_TAG,	        	        sizeof(uint8_t), 0, &msgLen);

   /* PUCCH Param */
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUCCH_FORMATS_TAG,		                sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_MAX_PUCCHS_PER_SLOT_TAG,                         sizeof(uint8_t), 0, &msgLen);

   /* PDSCH Param */
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDSCH_MAPPING_TYPE_TAG,     			sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDSCH_ALLOCATION_TYPES_TAG,                      sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDSCH_VRB_TO_PRB_MAPPING_TAG,			sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDSCH_CBG_TAG,				        sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDSCH_DMRS_CONFIG_TYPES_TAG,			sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDSCH_DMRS_MAX_LENGTH_TAG,			sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDSCH_DMRS_ADDITIONAL_POS_TAG,		        sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_MAX_PDSCHS_TBS_PER_SLOT_TAG,			sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_MAX_NUMBER_MIMO_LAYERS_PDSCH_TAG,		sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_SUPPORTED_MAX_MODULATION_ORDER_DL_TAG,	        sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_MAX_MU_MIMO_USERS_DL_TAG,			sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDSCH_DATA_IN_DMRS_SYMBOLS_TAG,		        sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PREMPTIONSUPPORT_TAG,				sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDSCH_NON_SLOT_SUPPORT_TAG,                      sizeof(uint8_t), 0, &msgLen);

   /* PUSCH Param */
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_UCI_MUX_ULSCH_IN_PUSCH_TAG,			sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_UCI_ONLY_PUSCH_TAG,				sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUSCH_FREQUENCY_HOPPING_TAG,			sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUSCH_DMRS_CONFIG_TYPES_TAG,			sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUSCH_DMRS_MAX_LEN_TAG,     			sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUSCH_DMRS_ADDITIONAL_POS_TAG,		        sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUSCH_CBG_TAG,                       	        sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUSCH_MAPPING_TYPE_TAG,                          sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUSCH_ALLOCATION_TYPES_TAG,          		sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUSCH_VRB_TO_PRB_MAPPING_TAG,        	        sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUSCH_MAX_PTRS_PORTS_TAG,                        sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_MAX_PDUSCHS_TBS_PER_SLOT_TAG,        		sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_MAX_NUMBER_MIMO_LAYERS_NON_CB_PUSCH_TAG,         sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_SUPPORTED_MODULATION_ORDER_UL_TAG,               sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_MAX_MU_MIMO_USERS_UL_TAG,                        sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_DFTS_OFDM_SUPPORT_TAG,                           sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUSCH_AGGREGATION_FACTOR_TAG,                    sizeof(uint8_t), 0, &msgLen);

   /* PRACH Params */
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PRACH_LONG_FORMATS_TAG,              	        sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PRACH_SHORT_FORMATS_TAG,                         sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PRACH_RESTRICTED_SETS_TAG,                       sizeof(uint8_t), 0, &msgLen);
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_MAX_PRACH_FD_OCCASIONS_IN_A_SLOT_TAG,            sizeof(uint8_t), 0, &msgLen);

   /* MEASUREMENT TAG */
   fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_RSSI_MEASUREMENT_SUPPORT_TAG,                    sizeof(uint8_t), 0, &msgLen);

   fapiParamRsp->number_of_tlvs = index;
   msgLen += sizeof(fapi_param_resp_t) - sizeof(fapi_msg_t);
   fillMsgHeader(&fapiParamRsp->header, FAPI_PARAM_RESPONSE, msgLen);
   fapiParamRsp->error_code = MSG_OK;

   DU_LOG("\nPHY_STUB: Sending Param Request to Lower Mac");
   procPhyMessages(fapiParamRsp->header.msg_id, sizeof(fapi_param_resp_t), (void *)fapiParamRsp);
   MAC_FREE(fapiParamRsp, sizeof(fapi_param_resp_t));
#endif
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds and sends config response to lower mac
 *
 * @details
 *
 *    Function : l1BldAndSndConfigRsp
 *
 *    Functionality:
 *          - Builds and sends config response to MAC
 *
 * @params[in] Config request message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 l1BldAndSndConfigRsp(void *msg)
{
#ifdef INTEL_FAPI
   uint32_t msgLen = 0;
   fapi_config_resp_t *fapiConfigRsp;

   MAC_ALLOC(fapiConfigRsp, sizeof(fapi_config_resp_t));
   if(!fapiConfigRsp)
   {
      DU_LOG("PHY STUB: Memory allocation failed");
      return RFAILED;
   }
   memset(fapiConfigRsp, 0, sizeof(fapi_config_resp_t));
   fapiConfigRsp->number_of_invalid_tlvs = NULLP;
   fapiConfigRsp->number_of_inv_tlvs_idle_only = NULLP;
   fapiConfigRsp->number_of_missing_tlvs = NULLP;
   fapiConfigRsp->error_code = MSG_OK;
   msgLen = sizeof(fapi_config_resp_t) - sizeof(fapi_msg_t);
   fillMsgHeader(&fapiConfigRsp->header, FAPI_CONFIG_RESPONSE, msgLen);

   DU_LOG("\nPHY_STUB: Sending Config Response to Lower Mac");
   procPhyMessages(fapiConfigRsp->header.msg_id, \
	 sizeof(fapi_config_resp_t), (void *)fapiConfigRsp);
   MAC_FREE(fapiConfigRsp, sizeof(fapi_config_resp_t));
#endif
   return ROK;
}
/*******************************************************************
 *
 * @brief Handles param request received from MAC
 *
 * @details
 *
 *    Function : l1HdlParamReq
 *
 *    Functionality:
 *          -Handles param request received from MAC
 *
 * @params[in]   Message length
 *               Param request message pointer
 *
 * @return void
 *
 * ****************************************************************/

PUBLIC void l1HdlParamReq(uint32_t msgLen, void *msg)
{
#ifdef INTEL_FAPI
   DU_LOG("\nPHY_STUB: Received Param Request in PHY");

   /* Build and send PARAM RESPONSE */
   if(l1BldAndSndParamRsp(msg)!= ROK)
   {
      DU_LOG("\nPHY_STUB: Failed Sending Param Response");
   }
   MAC_FREE(msg, sizeof(fapi_param_req_t));
#endif
} 

/*******************************************************************
 *
 * @brief Handles config request received from MAC
 *
 * @details
 *
 *    Function : l1HdlConfigReq
 *
 *    Functionality:
 *          -Handles config request received from MAC
 *
 * @params[in]   Message length
 *               config request message pointer
 *
 * @return void
 *
 * ****************************************************************/

PUBLIC void l1HdlConfigReq(uint32_t msgLen, void *msg)
{
#ifdef INTEL_FAPI
   fapi_config_req_t *configReq = (fapi_config_req_t *)msg;

   DU_LOG("\nPHY_STUB: Received Config Request in PHY");

   /* Handling CONFIG RESPONSE */
   if(l1BldAndSndConfigRsp(msg)!= ROK)
   {
      printf("\nPHY_STUB: Failed Sending config Response");
   }

   MAC_FREE(configReq, msgLen);
#endif

}

/*******************************************************************
 *
 * @brief Build and Send CRC Indication
 *
 * @details
 *
 *    Function : l1BuildAndSendCrcInd
 *
 *    Functionality:
 *      Build and Send CRC Indication
 *
 * @params[in] Slot
 *             SFN 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t l1BuildAndSendCrcInd(uint16_t slot, uint16_t sfn)
{
#ifdef INTEL_FAPI
   uint8_t idx = 0;
   fapi_crc_ind_t  *crcInd;

   MAC_ALLOC(crcInd, sizeof(fapi_crc_ind_t));
   if(!crcInd)
   {
      printf("\nPHY_STUB: Memory allocation failed for CRC Indication Message");
      return RFAILED;
   }
   memset(crcInd, 0, sizeof(fapi_crc_ind_t));

   /* TODO: Fill the required values. As of now only 1 CRC status PASS is filled */
   crcInd->sfn = sfn;
   crcInd->slot = slot;
   crcInd->numCrcs = 1;

   crcInd->crc[idx].handle = 0;
   crcInd->crc[idx].rnti = 0;
   crcInd->crc[idx].harqId = 0;
   crcInd->crc[idx].tbCrcStatus = 0;
   crcInd->crc[idx].numCb = 1;
   crcInd->crc[idx].cbCrcStatus[0] = 0;
   crcInd->crc[idx].ul_cqi = 0;
   crcInd->crc[idx].timingAdvance = 0;
   crcInd->crc[idx].rssi = 0;

   fillMsgHeader(&crcInd->header, FAPI_CRC_INDICATION, \
	 sizeof(fapi_crc_ind_t) - sizeof(fapi_msg_t));

   /* Sending RACH indication to MAC */
   DU_LOG("\nPHY STUB: Sending CRC Indication to MAC");
   procPhyMessages(crcInd->header.msg_id, sizeof(fapi_crc_ind_t), (void *)crcInd);
   MAC_FREE(crcInd, sizeof(fapi_crc_ind_t));
#endif
   return ROK;
} /* l1BuildAndSendCrcInd */

#ifdef INTEL_FAPI
/*******************************************************************
 *
 * @brief Build and send Rx data indication
 *
 * @details
 *
 *    Function : l1BuildAndSendRxDataInd
 *
 *    Functionality:
 *       Build and send Rx data indication
 *
 * @params[in] SFN
 *             Slot
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t l1BuildAndSendRxDataInd(uint16_t slot, uint16_t sfn, fapi_ul_pusch_pdu_t puschPdu)
{
   uint8_t idx = 0;
   fapi_rx_data_indication_t *rxDataInd;
   fapi_pdu_ind_info_t       *pduInfo;
   uint8_t  *pdu;
   uint16_t byteIdx = 0;
   uint32_t msgLen = 0;

   MAC_ALLOC(rxDataInd, sizeof(fapi_rx_data_indication_t));
   if(!rxDataInd)
   {
      printf("\nPHY_STUB: Memory allocation failed for Rx Data Indication");
      return RFAILED;
   }
   memset(rxDataInd, 0, sizeof(fapi_rx_data_indication_t));

   /* TODO: Fill the required values */
   msgLen = sizeof(fapi_rx_data_indication_t) - sizeof(fapi_msg_t);
   rxDataInd->sfn = sfn;
   rxDataInd->slot = slot;
   rxDataInd->numPdus = 1;

   pduInfo = &rxDataInd->pdus[idx];
   pduInfo->handle = puschPdu.handle;
   pduInfo->rnti = puschPdu.rnti;
   pduInfo->harqId = puschPdu.puschData.harqProcessId;
   pduInfo->pdu_length = puschPdu.puschData.tbSize;
   pduInfo->ul_cqi = 0;
   pduInfo->timingAdvance = 0;
   pduInfo->rssi = 0;

   /* Filling pdu with random values for testing */
   pduInfo->pduData = NULL;
   MAC_ALLOC(pduInfo->pduData, pduInfo->pdu_length);
   if(!pduInfo->pduData)
   {
      printf("\nPHY_STUB: Memory allocation failed for Rx Data Pdu");
      return RFAILED;
   }

   /* Harcoded Initial RRC setup Request */
   pdu = (uint8_t *)pduInfo->pduData;
   pdu[byteIdx++] = 0;
   pdu[byteIdx++] = 181;
   pdu[byteIdx++] = 99;
   pdu[byteIdx++] = 20;
   pdu[byteIdx++] = 170;
   pdu[byteIdx++] = 132;
   pdu[byteIdx++] = 96;

   /* Harcoding the pad bytes */
   pdu[byteIdx++] = 63;

   for(; byteIdx < pduInfo->pdu_length; byteIdx++)
      pdu[byteIdx] = 0;
   msgLen += pduInfo->pdu_length;

   fillMsgHeader(&rxDataInd->header, FAPI_RX_DATA_INDICATION, msgLen);

   /* Sending Rx data indication to MAC */
   DU_LOG("\nPHY STUB: Sending Rx data Indication to MAC");
   procPhyMessages(rxDataInd->header.msg_id, sizeof(fapi_rx_data_indication_t), (void *)rxDataInd);

   if(pduInfo->pdu_length)
      MAC_FREE(pduInfo->pduData, pduInfo->pdu_length);
   MAC_FREE(rxDataInd, sizeof(fapi_rx_data_indication_t));
   return ROK;
}
#endif

/*******************************************************************
 *
 * @brief Builds and Sends RACH indication to MAC 
 *
 * @details
 *
 *    Function : l1BuildAndSendRachInd 
 *
 *    Functionality:
 *      Builds and Sends RACH indication to MAC
 *
 * @params[in] SFN value 
 *             slot value
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t l1BuildAndSendRachInd(uint16_t slot, uint16_t sfn)
{
#ifdef INTEL_FAPI
   uint8_t   rachPduIdx = 0; 
   uint8_t   preamIdx = 0;
   fapi_rach_pdu_t  *rachPdu;
   fapi_rach_indication_t  *rachInd;

   /* Building RACH indication */
   MAC_ALLOC(rachInd, sizeof(fapi_rach_indication_t));
   if(!rachInd)
   {
      printf("\nPHY_STUB: Memory allocation failed for Rach Indication Message");
      return RFAILED;
   }
   memset(rachInd, 0, sizeof(fapi_rach_indication_t)); 
   rachInd->sfn = sfn;
   rachInd->slot = slot;
   rachInd->numPdus = 1;

   rachPdu = &rachInd->rachPdu[rachPduIdx];
   rachPdu->phyCellId = NR_PCI;
   rachPdu->symbolIndex = 0;
   rachPdu->slotIndex = slot;
   rachPdu->freqIndex = 0;
   rachPdu->avgRssi = 0;
   rachPdu->avgSnr = 0;
   rachPdu->numPreamble = 1;

   rachPdu->preambleInfo[preamIdx].preambleIndex = 3;
   rachPdu->preambleInfo[preamIdx].timingAdvance = 0;
   rachPdu->preambleInfo[preamIdx].preamblePwr = 0;

   fillMsgHeader(&rachInd->header, FAPI_RACH_INDICATION, \
	 sizeof(fapi_rach_indication_t) - sizeof(fapi_msg_t));

   /* Sending RACH indication to MAC */
   DU_LOG("\nPHY STUB: Sending RACH Indication to MAC");
   procPhyMessages(rachInd->header.msg_id, sizeof(fapi_rach_indication_t), (void *)rachInd);
   MAC_FREE(rachInd, sizeof(fapi_rach_indication_t));
#endif
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds and Send the Slot Indication message to MAC
 *
 * @details
 *
 *    Function : l1BuildAndSendSlotIndication
 *
 *    Functionality:
 *          -Send the Slot indication Message to MAC
 *
 * @params[in]   Message length
 *               config request message pointer
 *
 * @return void
 *
 * ****************************************************************/
PUBLIC uint16_t l1BuildAndSendSlotIndication()
{
#ifdef INTEL_FAPI
   fapi_slot_ind_t *slotIndMsg;

   MAC_ALLOC(slotIndMsg, sizeof(fapi_slot_ind_t));
   if(!slotIndMsg)
   {
      DU_LOG("\nPHY_STUB: Memory allocation failed for slot Indication Message");
      return RFAILED;
   }
   else
   {
      memset(slotIndMsg, 0, sizeof(fapi_slot_ind_t));
      slotIndMsg->sfn = sfnValue;
      slotIndMsg->slot = slotValue;
      DU_LOG("\n\nPHY_STUB: SLOT indication [%d:%d]",sfnValue,slotValue);

      /* increment for the next TTI */
      slotValue++;
      if(sfnValue >= MAX_SFN_VALUE && slotValue > MAX_SLOT_VALUE)
      {
	 sfnValue = 0;
	 slotValue = 0;
      }
      else if(slotValue > MAX_SLOT_VALUE)
      {
	 sfnValue++;
	 slotValue = 0;
      }
      fillMsgHeader(&slotIndMsg->header, FAPI_SLOT_INDICATION, \
	    sizeof(fapi_slot_ind_t) - sizeof(fapi_msg_t));
      procPhyMessages(slotIndMsg->header.msg_id, sizeof(fapi_slot_ind_t), (void*)slotIndMsg);
      MAC_FREE(slotIndMsg, sizeof(fapi_slot_ind_t));
   }
#endif
   return ROK;
}

/*******************************************************************
 *
 * @brief Handles start request received from MAC
 *
 * @details
 *
 *    Function : l1HdlStartReq
 *
 *    Functionality:
 *          -Handles start request received from MAC
 *
 * @params[in]   Message length
 *               config request message pointer
 *
 * @return void
 *
 * ****************************************************************/

PUBLIC S16 l1HdlStartReq(uint32_t msgLen, void *msg)
{
#ifdef INTEL_FAPI
   fapi_start_req_t *startReq = (fapi_start_req_t *)msg;

   if(lwrMacCb.phyState == PHY_STATE_CONFIGURED)
   {
      l1HdlSlotIndicaion(FALSE);
      MAC_FREE(startReq, sizeof(fapi_start_req_t));
   }
   else
   {
      DU_LOG("\nPHY_STUB: Received Start Req in PHY State %d", lwrMacCb.phyState);
      return RFAILED;
   }
#endif
   return ROK;
}

/*******************************************************************
 *
 * @brief Handles Dl Tti request received from MAC
 *
 * @details
 *
 *    Function : l1HdlDlTtiReq
 *
 *    Functionality:
 *          -Handles Dl Tti request received from MAC
 *
 * @params[in]   Message length
 *               Dl Tti request message pointer
 *
 * @return void
 *
 * ****************************************************************/

PUBLIC S16 l1HdlDlTtiReq(uint16_t msgLen, void *msg)
{
#ifdef INTEL_FAPI
   fapi_dl_tti_req_t *dlTtiReq;
   dlTtiReq = (fapi_dl_tti_req_t *)msg;

   printf("\nPHY STUB: DL TTI Request at sfn=%d slot=%d",dlTtiReq->sfn,dlTtiReq->slot);
#if 0
   printf("\nPHY_STUB:  SFN     %d", dlTtiReq->sfn);
   printf("\nPHY_STUB:  SLOT    %d", dlTtiReq->slot);
   printf("\nPHY_STUB:  nPdus   %d", dlTtiReq->nPdus);
   printf("\nPHY_STUB:  nGroup  %d", dlTtiReq->nGroup);
   /* Printing SSB CONFIGURED VALUES */
   printf("\nPHY_STUB: physCellId   %d", dlTtiReq->pdus->u.ssb_pdu.physCellId);
   printf("\nPHY_STUB: betaPss      %d", dlTtiReq->pdus->u.ssb_pdu.betaPss);
   printf("\nPHY_STUB: ssbBlockIndex %d",	dlTtiReq->pdus->u.ssb_pdu.ssbBlockIndex);
   printf("\nPHY_STUB: ssbSubCarrierOffset %d",	dlTtiReq->pdus->u.ssb_pdu.ssbSubCarrierOffset);
   printf("\nPHY_STUB: ssbOffsetPointA     %d",	dlTtiReq->pdus->u.ssb_pdu.ssbOffsetPointA);
   printf("\nPHY_STUB: bchPayloadFlag      %d",	dlTtiReq->pdus->u.ssb_pdu.bchPayloadFlag);
   printf("\nPHY_STUB: bchPayload          %x",	dlTtiReq->pdus->u.ssb_pdu.bchPayload);
#endif
   uint8_t pduCount = 0;
   if(dlTtiReq->nPdus == 0)
   {
      DU_LOG("\nPHY_STUB: No PDU in DL TTI Request");
   }
   for(pduCount=0; pduCount<dlTtiReq->nPdus; pduCount++)
   {
      if(dlTtiReq->pdus[pduCount].pduType == 3) //SSB_PDU_TYPE
	 DU_LOG("\nPHY_STUB: SSB PDU");
      else if(dlTtiReq->pdus[pduCount].pduType == 0)
	 DU_LOG("\nPHY_STUB: PDCCH PDU");
      else if(dlTtiReq->pdus[pduCount].pduType == 1)
	 DU_LOG("\nPHY_STUB: PDSCH PDU");
   }

   /* Free FAPI message */
   MAC_FREE(dlTtiReq, msgLen);

#endif
   return ROK;
}

/*******************************************************************
 *
 * @brief Handles tx_data request received from MAC
 *
 * @details
 *
 *    Function : l1HdlTxDataReq
 *
 *    Functionality:
 *          -Handles tx_data request received from MAC
 *
 * @params[in]   Message length
 *               tx_data request message pointer
 *
 * @return void
 *
 * ****************************************************************/

PUBLIC S16 l1HdlTxDataReq(uint16_t msgLen, void *msg)
{
#ifdef INTEL_FAPI
   fapi_tx_data_req_t *txDataReq;
   txDataReq = (fapi_tx_data_req_t *)msg;

   DU_LOG("\nPHY STUB: TX DATA Request at sfn=%d slot=%d",txDataReq->sfn,txDataReq->slot);

   MAC_FREE(txDataReq, msgLen);
#endif
   return ROK;
}
/*******************************************************************
 *
 * @brief Handles Ul Tti request received from MAC
 *
 * @details
 *
 *    Function : l1HdlUlTtiReq
 *
 *    Functionality:
 *          -Handles Ul Tti request received from MAC
 *
 * @params[in]   Message length
 *               Ul Tti request message pointer
 *
 * @return void
 *
 * ****************************************************************/

PUBLIC S16 l1HdlUlTtiReq(uint16_t msgLen, void *msg)
{
#ifdef INTEL_FAPI
   fapi_ul_tti_req_t *ulTtiReq;

   DU_LOG("\nPHY STUB: Received UL TTI Request");

   ulTtiReq = (fapi_ul_tti_req_t *)msg;
   uint8_t numPdus = ulTtiReq->nPdus;

   if(numPdus == 0)
   {
      DU_LOG("\nPHY STUB: No PDU in UL TTI");
   }
   while(numPdus)
   {
      if(ulTtiReq->pdus[numPdus-1].pduType == 0)
      {
	 DU_LOG("\nPHY STUB: PRACH PDU");
      }
      if(ulTtiReq->pdus[numPdus-1].pduType == 1)
      {
	 DU_LOG("\nPHY STUB: PUSCH PDU");			
	 l1BuildAndSendRxDataInd(ulTtiReq->slot, ulTtiReq->sfn, \
	       ulTtiReq->pdus[numPdus-1].pdu.pusch_pdu); 
      }
      if(ulTtiReq->pdus[numPdus-1].pduType == 2)
      {
	 DU_LOG("\nPHY STUB: PUCCH PDU");
      }
      numPdus--;
   }

   if(rachIndSent == false && ulTtiReq->sfn == 2 && ulTtiReq->slot == 6)
   {
      rachIndSent = true;
      l1BuildAndSendRachInd(ulTtiReq->slot, ulTtiReq->sfn);
   }

   MAC_FREE(ulTtiReq, msgLen);
#endif
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds and Send the stop Indication message to MAC
 *
 * @details
 *
 *    Function : l1BuildAndSendStopInd
 *
 *    Functionality:
 *          -Send the Stop indication Message to MAC
 *
 *
 * @return void
 *
 * ****************************************************************/
PUBLIC uint16_t l1BuildAndSendStopInd()
{
#ifdef INTEL_FAPI
   fapi_stop_ind_t *stopIndMsg = NULLP;
   uint32_t msgLen = 0;

   MAC_ALLOC(stopIndMsg, sizeof(fapi_stop_ind_t));
   if(!stopIndMsg)
   {
      DU_LOG("\nPHY_STUB: Memory allocation failed for stop Indication Message");
      return RFAILED;
   }
   else
   {
      fillMsgHeader(&stopIndMsg->header, FAPI_STOP_INDICATION, msgLen);
      DU_LOG("\n\nPHY_STUB: Processing Stop indication to MAC");
      procPhyMessages(stopIndMsg->header.msg_id,\
	    sizeof(fapi_stop_ind_t), (void*)stopIndMsg);
      MAC_FREE(stopIndMsg, sizeof(fapi_stop_ind_t));
   }
#endif
   return ROK;
}

/*******************************************************************
 *
 * @brief Handles stop request received from MAC
 *
 * @details
 *
 *    Function : l1HdlStopReq
 *
 *    Functionality:
 *          -Handles stop request received from MAC
 *
 * @params[in]   Message length
 *               stop request message pointer
 *
 * @return void
 *
 * ****************************************************************/

PUBLIC S16 l1HdlStopReq(uint32_t msgLen, void *msg)
{
#ifdef INTEL_FAPI
   fapi_stop_req_t *stopReq = (fapi_stop_req_t *)msg;

   if(lwrMacCb.phyState == PHY_STATE_RUNNING)
   {
      l1HdlSlotIndicaion(TRUE);
      DU_LOG("\nPHY_STUB: Slot Indication is stopped successfully");
      l1BuildAndSendStopInd();
      MAC_FREE(stopReq, msgLen);
   }
   else
   {
      DU_LOG("\nPHY_STUB: Received Stop Req in PHY State %d", lwrMacCb.phyState);
      return RFAILED;
   }
#endif
   return ROK;
}

/*******************************************************************
 *
 * @brief Receives message from MAC
 *
 * @details
 *
 *    Function :  l1ProcessFapiRequest
 *
 *    Functionality:
 *       - Receives message from MAC and calls handler
 *
 * @params[in] Message type
 *             Message length
 *             Message pointer
 *
 * @return void
 *
 * ****************************************************************/

void l1ProcessFapiRequest(uint8_t msgType, uint32_t msgLen, void *msg)
{
   switch(msgType)
   {
#ifdef INTEL_FAPI
      case FAPI_PARAM_REQUEST:
	 l1HdlParamReq(msgLen, msg);
	 break;
      case FAPI_CONFIG_REQUEST:
	 l1HdlConfigReq(msgLen, msg);
	 break;
      case FAPI_START_REQUEST:
	 l1HdlStartReq(msgLen, msg);
	 break;
      case FAPI_DL_TTI_REQUEST:
	 l1HdlDlTtiReq(msgLen, msg);
	 break;
      case FAPI_TX_DATA_REQUEST:
	 l1HdlTxDataReq(msgLen, msg);
	 break;
      case FAPI_UL_TTI_REQUEST:
	 l1HdlUlTtiReq(msgLen, msg);
	 break;
      case FAPI_STOP_REQUEST:
	 l1HdlStopReq(msgLen, msg);
	 break;
      default:
	 DU_LOG("\nPHY_STUB: Invalid message type[%x] received at PHY", msgType);
	 break;
#endif
   }
}
/**********************************************************************
  End of file
 **********************************************************************/
