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

#include <stdint.h>

#include "envdep.h"
#include "gen.h"
#include "ssi.h"

#include "gen.x"
#include "ssi.x"

#include "lwr_mac.h"
#include "lwr_mac_phy.h"
#ifdef FAPI
#include "fapi.h"
#endif
#include "lphy_stub.h"
#include "du_log.h"

#define MAX_SLOT_VALUE   9
#define MAX_SFN_VALUE    1023

uint16_t sfnValue = 0;
uint16_t slotValue = 0;
EXTERN void phyToMac ARGS((uint16_t msgType, uint32_t msgLen,void *msg));
#ifdef FAPI
EXTERN void fillTlvs ARGS((fapi_uint16_tlv_t *tlv, uint16_t tag, uint16_t
length, uint16_t value, uint32_t *msgLen));
EXTERN void fillMsgHeader ARGS((fapi_msg_t *hdr, uint16_t msgType, uint16_t msgLen));
#endif
EXTERN void handlePhyMessages(uint16_t msgType, uint32_t msgSize, void *msg);

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
#ifdef FAPI
   uint8_t index = 0;
   uint32_t msgLen = 0;
	fapi_param_resp_t *fapiParamRsp = (fapi_param_resp_t *)msg;

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
  msgLen = msgLen + sizeof(fapi_param_resp_t);

  fillMsgHeader(&fapiParamRsp->header, FAPI_PARAM_RESPONSE, msgLen);
  fapiParamRsp->error_code = MSG_OK;
  DU_LOG("\nPHY_STUB: Sending Param Request to Lower Mac");
  handlePhyMessages(fapiParamRsp->header.message_type_id, sizeof(fapi_param_resp_t), (void *)fapiParamRsp);
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
#ifdef FAPI
   uint32_t msgLen = 0;
   fapi_config_resp_t *fapiConfigRsp = (fapi_config_resp_t *)msg;

   if(fapiConfigRsp != NULL)
   {
      fapiConfigRsp->number_of_invalid_tlvs = NULLP;
      fapiConfigRsp->number_of_inv_tlvs_idle_only = NULLP;
      fapiConfigRsp->number_of_missing_tlvs = NULLP;
      fapiConfigRsp->error_code = MSG_OK;
      msgLen += sizeof(fapi_config_resp_t);
      fillMsgHeader(&fapiConfigRsp->header, FAPI_CONFIG_RESPONSE, msgLen);
      DU_LOG("\nPHY_STUB: Sending Config Response to Lower Mac");
      handlePhyMessages(fapiConfigRsp->header.message_type_id, sizeof(fapi_config_resp_t), (void *)fapiConfigRsp);
      return ROK;
   }
#else
   return ROK;
#endif
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
   DU_LOG("\nPHY_STUB: Received Param Request in PHY");
   /* Handling PARAM RESPONSE */
   if(l1BldAndSndParamRsp(msg)!= ROK)
   {
      DU_LOG("\nPHY_STUB: Failed Sending Param Response");
   }
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
   DU_LOG("\nPHY_STUB: Received Config Request in PHY");

   /* Handling CONFIG RESPONSE */
   if(l1BldAndSndConfigRsp(msg)!= ROK)
   {
      printf("\nPHY_STUB: Failed Sending config Response");
   }
}

/*******************************************************************
 *
 * @brief Builds and Send the Slot Indication message to MAC
 *
 * @details
 *
 *    Function : buildAndSendSlotIndication
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
PUBLIC S16 buildAndSendSlotIndication()
{
#ifdef FAPI
   fapi_slot_ind_t *slotIndMsg;
   if(SGetSBuf(0, 0, (Data **)&slotIndMsg, sizeof(slotIndMsg)) != ROK)
   {
       DU_LOG("\nPHY_STUB: Memory allocation failed for slot Indication Message");
       return RFAILED;
   }
   else
   {
      slotValue++;
      if(sfnValue > MAX_SFN_VALUE && slotValue > MAX_SLOT_VALUE)
      {
         sfnValue = 0;
         slotValue = 0;
      }
      else if(slotValue > MAX_SLOT_VALUE)
      {
         sfnValue++;
         slotValue = 0;
      }
      slotIndMsg->sfn = sfnValue;
      slotIndMsg->slot = slotValue;
      fillMsgHeader(&slotIndMsg->header, FAPI_SLOT_INDICATION, sizeof(fapi_slot_ind_t));
      DU_LOG("\nPHY_STUB [%d:%d] ",sfnValue,slotValue);
      handlePhyMessages(slotIndMsg->header.message_type_id, sizeof(fapi_slot_ind_t), (void*)slotIndMsg);
      SPutSBuf(0, 0, (Data *)slotIndMsg, sizeof(slotIndMsg));
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
   if(clGlobalCp.phyState == PHY_STATE_CONFIGURED)
   {
      duStartSlotIndicaion();
#ifdef FAPI
      SPutSBuf(0, 0, (Data *)msg, sizeof(fapi_start_req_t));
#endif
      return ROK;
   }
   else
   {
      DU_LOG("\n PHY_STUB: Received Start Req in PHY State %d", clGlobalCp.phyState);
      return RFAILED;
   }
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
#ifdef FAPI
   fapi_dl_tti_req_t *dlTtiReq;
   dlTtiReq = (fapi_dl_tti_req_t *)msg;
#if 0
   printf("\nPHY_STUB:  Received DL TTI Request in PHY");
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
   uint8_t numPdus = dlTtiReq->nPdus;
	if(numPdus == 0)
	{
		DU_LOG("\nNo PDU \n");
   }
	while(numPdus)
	{
		if(dlTtiReq->pdus->pduType == 3) //SSB_PDU_TYPE
			DU_LOG("\nSSB PDU\n");
		else if(dlTtiReq->pdus->pduType == 0)
			DU_LOG("\nSIB1 PDCCH PDU\n");
		else if(dlTtiReq->pdus->pduType == 1)
		   DU_LOG("\nSIB1 PDSCH PDU\n");

		numPdus--;
	}

   SPutSBuf(0, 0, (Data *)dlTtiReq, sizeof(fapi_dl_tti_req_t));
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
#ifdef FAPI
   fapi_ul_tti_req_t *ulTtiReq;
   ulTtiReq = (fapi_ul_tti_req_t *)msg;
   uint8_t numPdus = ulTtiReq->nPdus;
	if(numPdus == 0)
	{
		DU_LOG("\nPHY STUB: No PDU in UL TTI \n");
   }
	while(numPdus)
	{
		if(ulTtiReq->pdus->pduType == 0)
			DU_LOG("\n PHY STUB: PRACH PDU\n");
		numPdus--;
	}
   SPutSBuf(0, 0, (Data *)ulTtiReq, sizeof(fapi_ul_tti_req_t));
#endif
   return ROK;
}

/*******************************************************************
 *
 * @brief Receives message from MAC
 *
 * @details
 *
 *    Function : processFapiRequest
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

void processFapiRequest(uint8_t msgType, uint32_t msgLen, void *msg)
{
   switch(msgType)
   {
#ifdef FAPI
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
      case FAPI_UL_TTI_REQUEST:
         l1HdlUlTtiReq(msgLen, msg);
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
