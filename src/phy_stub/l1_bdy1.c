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

#include "envdep.h"
#include "gen.h"
#include "ssi.h"

#include "gen.x"
#include "ssi.x"

#include "rg_cl_phy.h"
#include "lwr_mac.h"
#include "fapi.h"
#include "lphy_stub.h"

EXTERN void phyToMac ARGS((U16 msgType, U32 msgLen,void *msg));
EXTERN void fillTlvs ARGS((fapi_uint16_tlv_t *tlv, U16 tag, U16 length, U16 value, U16 *msgLen));
EXTERN void fillMsgHeader ARGS((fapi_msg_t *hdr, U16 msgType, U16 msgLen));
EXTERN S16 sendToLowerMac ARGS((U16 msgType, U32 msgLen,void *msg));

/*******************************************************************
 *
 * @brief Builds and sends param response to MAC CL
 *
 * @details
 *
 *    Function : lwrMacBldAndSndParamRsp
 *
 *    Functionality:
 *          - Builds and sends param response to MAC
 *
 * @params[in] Config request message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 lwrMacBldAndSndParamRsp(fapi_param_resp_t *fapiParamRsp)
{
   uint8_t index = 0;
   uint32_t msgLen = 0;

  /* Cell Params */
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_RELEASE_CAPABILITY_TAG,			        sizeof(U16), 1, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PHY_STATE_TAG,				        sizeof(U16), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_SKIP_BLANK_DL_CONFIG_TAG,		        sizeof(U8),  0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_SKIP_BLANK_UL_CONFIG_TAG,			sizeof(U8),  0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_NUM_CONFIG_TLVS_TO_REPORT_TYPE_TAG,              sizeof(U8),  0, &msgLen);

  /* Carrier Params */
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_CYCLIC_PREFIX_TAG,                               sizeof(U8),  1, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_SUPPORTED_SUBCARRIER_SPACING_DL_TAG,	        sizeof(U8),  1, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_SUPPORTED_BANDWIDTH_DL_TAG,			sizeof(U16), 1, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_SUPPORTED_SUBCARRIER_SPACING_UL_TAG,		sizeof(U8),  0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_SUPPORTED_BANDWIDTH_UL_TAG,			sizeof(U16), 0, &msgLen);

  /* PDCCH Param*/
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_CCE_MAPPING_TYPE_TAG,   			        sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_CORESET_OUTSIDE_FIRST_3_OFDM_SYMS_OF_SLOT_TAG,   sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PRECODER_GRANULARITY_CORESET_TAG,		sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDCCH_MU_MIMO_TAG,  				sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDCCH_PRECODER_CYCLING_TAG,			sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_MAX_PDCCHS_PER_SLOT_TAG,	        	        sizeof(U8), 0, &msgLen);

  /* PUCCH Param */
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUCCH_FORMATS_TAG,		                sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_MAX_PUCCHS_PER_SLOT_TAG,                         sizeof(U8), 0, &msgLen);

  /* PDSCH Param */
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDSCH_MAPPING_TYPE_TAG,     			sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDSCH_ALLOCATION_TYPES_TAG,                      sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDSCH_VRB_TO_PRB_MAPPING_TAG,			sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDSCH_CBG_TAG,				        sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDSCH_DMRS_CONFIG_TYPES_TAG,			sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDSCH_DMRS_MAX_LENGTH_TAG,			sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDSCH_DMRS_ADDITIONAL_POS_TAG,		        sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_MAX_PDSCHS_TBS_PER_SLOT_TAG,			sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_MAX_NUMBER_MIMO_LAYERS_PDSCH_TAG,		sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_SUPPORTED_MAX_MODULATION_ORDER_DL_TAG,	        sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_MAX_MU_MIMO_USERS_DL_TAG,			sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDSCH_DATA_IN_DMRS_SYMBOLS_TAG,		        sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PREMPTIONSUPPORT_TAG,				sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PDSCH_NON_SLOT_SUPPORT_TAG,                      sizeof(U8), 0, &msgLen);

  /* PUSCH Param */
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_UCI_MUX_ULSCH_IN_PUSCH_TAG,			sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_UCI_ONLY_PUSCH_TAG,				sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUSCH_FREQUENCY_HOPPING_TAG,			sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUSCH_DMRS_CONFIG_TYPES_TAG,			sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUSCH_DMRS_MAX_LEN_TAG,     			sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUSCH_DMRS_ADDITIONAL_POS_TAG,		        sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUSCH_CBG_TAG,                       	        sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUSCH_MAPPING_TYPE_TAG,                          sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUSCH_ALLOCATION_TYPES_TAG,          		sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUSCH_VRB_TO_PRB_MAPPING_TAG,        	        sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUSCH_MAX_PTRS_PORTS_TAG,                        sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_MAX_PDUSCHS_TBS_PER_SLOT_TAG,        		sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_MAX_NUMBER_MIMO_LAYERS_NON_CB_PUSCH_TAG,         sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_SUPPORTED_MODULATION_ORDER_UL_TAG,               sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_MAX_MU_MIMO_USERS_UL_TAG,                        sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_DFTS_OFDM_SUPPORT_TAG,                           sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PUSCH_AGGREGATION_FACTOR_TAG,                    sizeof(U8), 0, &msgLen);

  /* PRACH Params */
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PRACH_LONG_FORMATS_TAG,              	        sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PRACH_SHORT_FORMATS_TAG,                         sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_PRACH_RESTRICTED_SETS_TAG,                       sizeof(U8), 0, &msgLen);
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_MAX_PRACH_FD_OCCASIONS_IN_A_SLOT_TAG,            sizeof(U8), 0, &msgLen);

  /* MEASUREMENT TAG */
  fillTlvs(&fapiParamRsp->tlvs[index++],  FAPI_RSSI_MEASUREMENT_SUPPORT_TAG,                    sizeof(U8), 0, &msgLen);

  fapiParamRsp->number_of_tlvs = index;
  msgLen = msgLen + sizeof(fapi_param_resp_t);

  fillMsgHeader(&fapiParamRsp->header, FAPI_PARAM_RESPONSE, msgLen);
  fapiParamRsp->error_code = MSG_OK;
  printf("\nSending Param Request to Lower Mac");
  sendToLowerMac(fapiParamRsp->header.message_type_id, sizeof(fapi_param_resp_t), (void *)fapiParamRsp);
  return ROK;
}

/*******************************************************************
 *
 * @brief Builds and sends config response to MAC CL
 *
 * @details
 *
 *    Function : lwrMacBldAndSndConfigRsp
 *
 *    Functionality:
 *          - Builds and sends config response to MAC
 *
 * @params[in] Config request message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 lwrMacBldAndSndConfigRsp(fapi_config_resp_t *fapiConfigRsp)
{
   uint8_t index = 0;
   uint32_t msgLen = 0;

   if(fapiConfigRsp != NULL)
   {
      fapiConfigRsp->number_of_invalid_tlvs = NULLP;
      fapiConfigRsp->number_of_inv_tlvs_idle_only = NULLP;
      fapiConfigRsp->number_of_missing_tlvs = NULLP;
      fapiConfigRsp->error_code = MSG_OK;
      msgLen += sizeof(fapi_config_resp_t);
      fillMsgHeader(&fapiConfigRsp->header, FAPI_CONFIG_RESPONSE, msgLen);
      printf("\nSending Config Response to Lower Mac");
      sendToLowerMac(fapiConfigRsp->header.message_type_id, sizeof(fapi_config_resp_t), (void *)fapiConfigRsp);
      return ROK;
   }
}

/*******************************************************************
 *
 * @brief Handles config request received from MAC
 *
 * @details
 *
 *    Function : l1HndlConfigReq
 *
 *    Functionality:
 *          -Handles config request received from MAC
 *
 * @params[in]   Message length
 *               Config request message pointer
 *
 * @return void
 *
 * ****************************************************************/

void l1HndlConfigReq(U16 msgLen, void *msg)
{
    printf("\nReceived configuration request");

    /* TO DO : validate all received TLVs and send back any unsupported/missing TLV */
}

/*******************************************************************
 *
 * @brief Handles param request received from MAC
 *
 * @details
 *
 *    Function : lwrMacHdlParamReq
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

PUBLIC void lwrMacHdlParamReq(U16 msgLen, void *msg)
{
   printf("\n Received Param Request in PHY");

   /* Handling PARAM RESPONSE */
   if(lwrMacBldAndSndParamRsp(msg)!= ROK)
   {
      printf("\n Failed Sending Param Response");
   }
} 

/*******************************************************************
 *
 * @brief Handles config request received from MAC
 *
 * @details
 *
 *    Function : lwrMacHdlConfigReq
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

PUBLIC void lwrMacHdlConfigReq(U16 msgLen, void *msg)
{
   printf("\n Received Config Request in PHY");

   /* Handling CONFIG RESPONSE */
   if(lwrMacBldAndSndConfigRsp(msg)!= ROK)
   {
      printf("\n Failed Sending config Response");
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
PUBLIC void buildAndSendSlotIndication()
{
   fapi_slot_ind_t *slotIndMsg;
   if(SGetSBuf(0, 0, (Data **)&slotIndMsg, sizeof(slotIndMsg)) != ROK)
   {
       printf("\nMemory allocation failed for slot Indication Message");
       return RFAILED;
   }
   else
   {
      slotIndMsg->sfn = 2;
      slotIndMsg->slot = 4;
      fillMsgHeader(&slotIndMsg->header, FAPI_SLOT_INDICATION, sizeof(fapi_slot_ind_t));
      printf("\n Sending Slot Indication Msg to Lower Mac");
      sendToLowerMac(SLOT_INDICATION, sizeof(fapi_slot_ind_t), (void*)slotIndMsg);
		SPutSBuf(0, 0, (Data *)slotIndMsg, sizeof(slotIndMsg));
		return ROK;
   }
}

/*******************************************************************
 *
 * @brief Handles start request received from MAC
 *
 * @details
 *
 *    Function : lwrMacHdlStartReq
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

PUBLIC void lwrMacHdlStartReq(U16 msgLen, void *msg)
{

   if(clGlobalCp.phyState == PHY_STATE_CONFIGURED)
   {
      duStartTtiThread();
		SPutSBuf(0, 0, (Data *)msg, sizeof(fapi_start_req_t));
		return ROK;
   }
   else
   {
     printf("\n Received Start Req in PHY State", clGlobalCp.phyState);
	  return RFAILED;
   }
}

/*******************************************************************
 *
 * @brief Receives message from MAC
 *
 * @details
 *
 *    Function : macToPhy
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

void macToPhy(U16 msgType, U32 msgLen, void *msg)
{
   switch(msgType)
   {
      case MSG_TYPE_CONFIG_REQ:
         l1HndlConfigReq(msgLen, msg);
         break;
      default:
         printf("\nInvalid message type[%x] received at PHY", msgType);
   }
}

/*******************************************************************
 *
 * @brief Receives message from MAC
 *
 * @details
 *
 *    Function : processRequest
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

void processRequest(U16 msgType, U32 msgLen, void *msg)
{
   switch(msgType)
   {
      case FAPI_PARAM_REQUEST:
         lwrMacHdlParamReq(msgLen, msg);
         break;
      case FAPI_CONFIG_REQUEST:
         lwrMacHdlConfigReq(msgLen, msg);
         break;
      case FAPI_START_REQUEST:
         lwrMacHdlStartReq(msgLen, msg);
         break;
      case FAPI_DL_TTI_REQUEST:
         //lwrMacHdlDlTtiReq(msgLen, msg);
         break;
      default:
         printf("\n Invalid message type[%x] received at PHY", msgType);
         break;
   }
}
/**********************************************************************
         End of file
**********************************************************************/
