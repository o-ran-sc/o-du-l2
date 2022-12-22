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
#include "lwr_mac_fsm.h"
#include "lwr_mac_phy.h"
#ifdef INTEL_FAPI
#include "nr5g_fapi_internal.h"
#include "fapi_vendor_extension.h"
#endif
#include "lwr_mac_upr_inf.h"
#include "mac_utils.h"
#include "phy_stub.h"
#include "phy_stub_utils.h"
#include "lwr_mac_phy_stub_inf.h"

/*******************************************************************
 *
 * @brief start the uplink data
 *
 * @details
 *
 *    Function : startUlData 
 *
 *    Functionality: start the uplink data
 *
 * @params[in]   
 *
 * @return void
 *
 * ****************************************************************/

void startUlData()
{
   uint8_t ueIdx=0, drbIdx=0;

   /* Start Pumping data from PHY stub to DU */
   for(ueIdx=0; ueIdx < phyDb.ueDb.numActvUe; ueIdx++)
   {
      for(drbIdx = 0; drbIdx < NUM_DRB_TO_PUMP_DATA; drbIdx++) //Number of DRB times the loop will run
      {
         DU_LOG("\nDEBUG  --> PHY STUB: Sending UL User Data[DrbId:%d] for UEIdx %d\n",drbIdx,ueIdx);
         l1SendUlUserData(drbIdx,ueIdx);
      }
   } 
}

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
      DU_LOG("\nERROR  -->  PHY STUB: Memory allocation failed");
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

   DU_LOG("\nINFO   -->  PHY_STUB: Sending Param Request to Lower Mac");
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
      DU_LOG("\nERROR  -->  PHY STUB: Memory allocation failed");
      return RFAILED;
   }
   memset(fapiConfigRsp, 0, sizeof(fapi_config_resp_t));
   fapiConfigRsp->number_of_invalid_tlvs = NULLP;
   fapiConfigRsp->number_of_inv_tlvs_idle_only = NULLP;
   fapiConfigRsp->number_of_missing_tlvs = NULLP;
   fapiConfigRsp->error_code = MSG_OK;
   msgLen = sizeof(fapi_config_resp_t) - sizeof(fapi_msg_t);
   fillMsgHeader(&fapiConfigRsp->header, FAPI_CONFIG_RESPONSE, msgLen);

   DU_LOG("\nINFO   -->  PHY_STUB: Sending Config Response to Lower Mac");

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

void l1HdlParamReq(uint32_t msgLen, void *msg)
{
#ifdef INTEL_FAPI
   DU_LOG("\nINFO   -->  PHY_STUB: Received Param Request in PHY");

   /* Build and send PARAM RESPONSE */
   if(l1BldAndSndParamRsp(msg)!= ROK)
   {
      DU_LOG("\nERROR  -->  PHY_STUB: Failed Sending Param Response");
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

void l1HdlConfigReq(uint32_t msgLen, void *msg)
{
   memset(&phyDb.ueDb, 0, sizeof(UeDb));
   cmInetAddr((char *)SOURCE_DU_IPV4_ADDR, &phyDb.ipCfgInfo.sourceDu);
   cmInetAddr((char *)DESTINATION_DU_IPV4_ADDR, &phyDb.ipCfgInfo.destinationDu);
   phyDb.ipCfgInfo.portNumber = PORT_NUMBER;
   phyDb.isServer = true;

#ifdef INTEL_FAPI
   p_fapi_api_queue_elem_t configReqElem = (p_fapi_api_queue_elem_t)msg;
   fapi_config_req_t *configReq = (fapi_config_req_t *)(configReqElem +1);

   DU_LOG("\nINFO   -->  PHY_STUB: Received Config Request in PHY");

   /* Handling CONFIG RESPONSE */
   if(l1BldAndSndConfigRsp(configReq)!= ROK)
   {
      DU_LOG("\nERROR  -->  PHY_STUB: Failed Sending config Response");
   }

   MAC_FREE(msg, msgLen);
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
uint16_t l1BuildAndSendCrcInd(uint16_t slot, uint16_t sfn, fapi_ul_pusch_pdu_t puschPdu)
{
   uint8_t result[]={0,//MSG3
                     0,//BSR
                     0,//MSG5 RRC Setup Complete
                     0,//Security Mode Complete
                     0,//Registraion Complete
                     0,//RRC Reconfiguration Complete
                     0,//UL DATA -1
                     0,//UL DATA -2
                     0,//UL DATA -3
                     0,//UL DATA -4
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

#ifdef INTEL_FAPI
   uint8_t idx = 0;
   static uint8_t ind=0;
   uint16_t ret = ROK;
   fapi_crc_ind_t  *crcInd;

   MAC_ALLOC(crcInd, sizeof(fapi_crc_ind_t));
   if(!crcInd)
   {
      DU_LOG("\nERROR  -->  PHY_STUB: Memory allocation failed for CRC Indication Message");
      return RFAILED;
   }
   memset(crcInd, 0, sizeof(fapi_crc_ind_t));

   /* TODO: Fill the required values. As of now only 1 CRC status PASS is filled */
   crcInd->sfn = sfn;
   crcInd->slot = slot;
   crcInd->numCrcs = 1;

   crcInd->crc[idx].handle = puschPdu.handle;
   crcInd->crc[idx].rnti = puschPdu.rnti;
   crcInd->crc[idx].harqId = puschPdu.puschData.harqProcessId;
   crcInd->crc[idx].tbCrcStatus = 0;
   crcInd->crc[idx].numCb = 1;
   crcInd->crc[idx].cbCrcStatus[0] = result[ind%50];
   ret = (0== crcInd->crc[idx].cbCrcStatus[0])?ROK:RFAILED;
   /*TBD: To use crc ind with random number and percentage */
   //crcInd->crc[idx].cbCrcStatus[0] = (crcPassPer >= rand()%(100))?0:1;
   
   ind++;
   crcInd->crc[idx].ul_cqi = 0;
   crcInd->crc[idx].timingAdvance = 0;
   crcInd->crc[idx].rssi = 0;

   fillMsgHeader(&crcInd->header, FAPI_CRC_INDICATION, \
	 sizeof(fapi_crc_ind_t) - sizeof(fapi_msg_t));

   /* Sending CRC indication to MAC */
   DU_LOG("\nINFO   -->  PHY STUB: Sending CRC Indication to MAC");
   procPhyMessages(crcInd->header.msg_id, sizeof(fapi_crc_ind_t), (void *)crcInd);
   MAC_FREE(crcInd, sizeof(fapi_crc_ind_t));
#endif
   return ret;
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
   uint8_t idx = 0, ueId = 0;
   fapi_rx_data_indication_t *rxDataInd =NULLP;
   fapi_pdu_ind_info_t       *pduInfo =NULLP;
   uint8_t  *pdu = NULLP;
   uint16_t byteIdx = 0;
   uint32_t msgLen = 0;
   MsgType type = 0;

   GET_UE_ID(puschPdu.rnti, ueId);
   if(phyDb.ueDb.ueCb[ueId-1].isCFRA)
   {
      if(!phyDb.ueDb.ueCb[ueId-1].msgRrcReconfigComp)
      {
         /* In CF-RA in case of handover, RRC Reconfiguration Complete is sent
          * by UE once RAR is received from DU */
         phyDb.ueDb.ueCb[ueId-1].ueId = ueId;
         phyDb.ueDb.ueCb[ueId-1].crnti = puschPdu.rnti;
         phyDb.ueDb.ueCb[ueId-1].msgRrcReconfigComp = true;
         type = MSG_TYPE_RRC_RECONFIG_COMPLETE;
      }
      else
         return ROK; 
   }
   else
   {
      if(!phyDb.ueDb.ueCb[ueId-1].msg3Sent)
      {
         phyDb.ueDb.ueCb[ueId-1].ueId = ueId;
         phyDb.ueDb.ueCb[ueId-1].crnti = puschPdu.rnti;
         phyDb.ueDb.ueCb[ueId-1].msg3Sent = true;
         type = MSG_TYPE_MSG3;
         sleep(2);
      }
      else if(!phyDb.ueDb.ueCb[ueId-1].msg5ShortBsrSent)
      {
         phyDb.ueDb.ueCb[ueId-1].msg5ShortBsrSent = true;
         type = MSG_TYPE_SHORT_BSR;
      }
      else if(!phyDb.ueDb.ueCb[ueId-1].msg5Sent)
      {
         phyDb.ueDb.ueCb[ueId-1].msg5Sent = true;
         type = MSG_TYPE_MSG5;
      }
      else if(!phyDb.ueDb.ueCb[ueId-1].msgNasAuthenticationComp)
      {
        phyDb.ueDb.ueCb[ueId-1].msgNasAuthenticationComp = true;
        type = MSG_TYPE_NAS_AUTHENTICATION_COMPLETE;
      }
      else if(!phyDb.ueDb.ueCb[ueId-1].msgNasSecurityModeComp)
      {
         phyDb.ueDb.ueCb[ueId-1].msgNasSecurityModeComp = true;
         type = MSG_TYPE_NAS_SECURITY_MODE_COMPLETE;
      }
      else if(!phyDb.ueDb.ueCb[ueId-1].msgRrcSecurityModeComp)
      {
         phyDb.ueDb.ueCb[ueId-1].msgRrcSecurityModeComp = true;
         type = MSG_TYPE_RRC_SECURITY_MODE_COMPLETE;
      }
      else if(!phyDb.ueDb.ueCb[ueId-1].msgRegistrationComp)
      {
         phyDb.ueDb.ueCb[ueId-1].msgRegistrationComp = true;
         type = MSG_TYPE_REGISTRATION_COMPLETE; 
      }
      else if(!phyDb.ueDb.ueCb[ueId-1].msgRrcReconfigComp)
      {
         phyDb.ueDb.ueCb[ueId-1].msgRrcReconfigComp = true;
         type = MSG_TYPE_RRC_RECONFIG_COMPLETE;
      }
      else
         return ROK;
   }

   MAC_ALLOC(rxDataInd, sizeof(fapi_rx_data_indication_t));
   if(!rxDataInd)
   {
      DU_LOG("\nERROR  -->  PHY_STUB: Memory allocation failed for Rx Data Indication");
      return RFAILED;
   }
   memset(rxDataInd, 0, sizeof(fapi_rx_data_indication_t));
   
   msgLen = sizeof(fapi_rx_data_indication_t) - sizeof(fapi_msg_t);
   rxDataInd->sfn = sfn;
   rxDataInd->slot = slot;
   rxDataInd->numPdus = 1;

   pduInfo = &rxDataInd->pdus[idx];
   pduInfo->handle = puschPdu.handle;
   pduInfo->rnti = puschPdu.rnti;
   pduInfo->harqId = puschPdu.puschData.harqProcessId;
   pduInfo->pdu_length = puschPdu.puschData.tbSize ;
   pduInfo->ul_cqi = 0;
   pduInfo->timingAdvance = 0;
   pduInfo->rssi = 0;

   /* Filling pdu with random values for testing */
   pduInfo->pduData = NULL;
   MAC_ALLOC(pduInfo->pduData, pduInfo->pdu_length);
   if(!pduInfo->pduData)
   {
      DU_LOG("\nERROR  -->  PHY_STUB: Memory allocation failed for Rx Data Pdu");
      return RFAILED;
   }
   memset(pduInfo->pduData, 0, pduInfo->pdu_length);
   /* Filling PDU */
   pdu = (uint8_t *)pduInfo->pduData;

   switch(type)
   {
      case MSG_TYPE_MSG3: 
         {
            DU_LOG("\nDEBUG  -->  PHY_STUB: Forming MSG3 PDU ");
            /* For Initial RRC setup Request,
               MAC subheader format is R/R/LCId (1byte)
               LCId is CCCH(0)
               From 38.321 section 6.1.1
               */
            pdu[byteIdx++] = 0;
            /* Hardcoding MAC PDU */
            pdu[byteIdx++] = 16;
            pdu[byteIdx++] = 00;
            pdu[byteIdx++] = 00;
            pdu[byteIdx++] = 00;
            pdu[byteIdx++] = 00;
            pdu[byteIdx++] = 103;

            break;
         }
      case MSG_TYPE_SHORT_BSR:
         {
            DU_LOG("\nDEBUG  -->  PHY_STUB: Forming SHORT BSR PDU ");
            uint8_t lcgId = 0;
            uint8_t bufferSizeIdx = 6;

            /* For Short BSR
               MAC subheader format is R/R/LcId (1Byte)
               LCId is 61
               From 38.321 section 6.1.1
               */
            pdu[byteIdx++] = 61;    // LCID
            pdu[byteIdx++] = (lcgId << 5) | bufferSizeIdx;

            break;
         }

      case MSG_TYPE_MSG5:
      {
         /* For RRC setup complete
          *
          * MAC subheader format is R/F/LCId/L (2/3 bytes)
          * LCId is 1 for SRB1
          * L is length of PDU i.e 6bytes here 
          * From 38.321 section 6.1.1
          *
          * RLC subheader for AM PDU is D/C/P/SI/SN (2 bytes for 12-bit SN)
          * From 38.322, section 6.2.2.4
          */
         DU_LOG("\nDEBUG  -->  PHY_STUB: Forming MSG5 PDU");
         uint8_t  msg5PduLen = 33; /* Length of MSG5 */
         msg5PduLen += 2; /* RLC subheader */
         uint8_t msg5[] = {1, msg5PduLen, 128, phyDb.ueDb.ueCb[ueId-1].rlcSnForSrb1++, 0, phyDb.ueDb.ueCb[ueId-1].pdcpSn++, 16, 0, \
            5, 223, 128, 16, 94, 64, 3, 64, 68, 252, 97, 0, 0, 0, 0, 4, 0, 0, 4, 68, 11, 128, 184, 56, 0, 0, 0, 0, 0};

         msg5PduLen += 2;  /* 2 bytes of MAC header */
         memcpy(pdu, &msg5, msg5PduLen);
         byteIdx += msg5PduLen; /* 4 bytes of header : MAC+RLC */
         break;
      }

      case MSG_TYPE_NAS_AUTHENTICATION_COMPLETE:
      {
        /* For Authentication response where RRC Container is dummy
          *
          * MAC subheader format is R/F/LCId/L (2/3 bytes)
          * LCId is 1 for SRB1
          * L is length of PDU i.e 6bytes here 
          * From 38.321 section 6.1.1
          *
          * RLC subheader for AM PDU is D/C/P/SI/SN (2 bytes for 12-bit SN)
          * From 38.322, section 6.2.2.4
          */
         DU_LOG("\nDEBUG  -->  PHY_STUB: Forming AUTHENTICATION RESPONSE PDU");
         uint8_t  pduLen = 37; /* Length of PDU */
         pduLen += 2; /* RLC subheader */
         uint8_t msg[] = {1, pduLen, 128, phyDb.ueDb.ueCb[ueId-1].rlcSnForSrb1++, 0, phyDb.ueDb.ueCb[ueId-1].pdcpSn++, 0x3a, \
                          0x0e, 0x3f, 0x00, 0xca, 0x95, 0xe9, 0x19, 0x41, 0x3f, 0x00, 0x2b, 0x96, 0x88, 0x06, 0xd7, 0x16, 0xc6, \
                          0x8b, 0xea, 0xae, 0x45, 0xd1, 0x01, 0xfd, 0x34, 0xd4, 0xfd, 0xd5, 0x71, 0x00, 0x00, 0x00, 0x00, 0x00};

         pduLen += 2;  /* 2 bytes of MAC header */
         memcpy(pdu, &msg, pduLen);
         byteIdx += pduLen; /* 4 bytes of header : MAC+RLC */
         break;
      }
      
      case MSG_TYPE_NAS_SECURITY_MODE_COMPLETE:
      {
        /* For NAS security mode complete where RRC Container is dummy
          *
          * MAC subheader format is R/F/LCId/L (2/3 bytes)
          * LCId is 1 for SRB1
          * L is length of PDU i.e 6bytes here 
          * From 38.321 section 6.1.1
          *
          * RLC subheader for AM PDU is D/C/P/SI/SN (2 bytes for 12-bit SN)
          * From 38.322, section 6.2.2.4
          */
         DU_LOG("\nDEBUG  -->  PHY_STUB: Forming NAS SECURITY MODE COMPLETE PDU");
         uint8_t  pduLen = 93; /* Length of PDU */
         pduLen += 2; /* RLC subheader */
         uint8_t msg[] = {1, pduLen, 128, phyDb.ueDb.ueCb[ueId-1].rlcSnForSrb1++, 0, phyDb.ueDb.ueCb[ueId-1].pdcpSn++, 0x3a, 0x2a, 0x3f, 
                          0x02, 0x75, 0xa0, 0xa0, 0xc0, 0x80, 0x3f, 0x00, 0x2f, 0x3b, 0x80, 0x04, 0x9a, 0xa2, 0x81, 0x09, 0x80, 0xc0, 
                          0x28, 0x04, 0xf8, 0xb8, 0x80, 0x1d, 0xbf, 0x00, 0x20, 0x8c, 0x80, 0x05, 0xf9, 0x00, 0x78, 0x88, 0x7a, 0x88, 
                          0xd9, 0x00, 0x00, 0x00, 0x03, 0x08, 0x00, 0x81, 0x97, 0x02, 0x78, 0x38, 0x78, 0x38, 0x17, 0x82, 0x82, 0x00, 
                          0x80, 0x00, 0x00, 0xa9, 0x00, 0x78, 0x88, 0x00, 0x00, 0x00, 0x8b, 0x83, 0xf8, 0x38, 0x60, 0x20, 0x0c, 0xc0, 
                          0x50, 0x0c, 0x00, 0x80, 0x3a, 0x00, 0x00, 0x48, 0x29, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00};

         pduLen += 2;  /* 2 bytes of MAC header */
         memcpy(pdu, &msg, pduLen);
         byteIdx += pduLen; /* 4 bytes of header : MAC+RLC */
         break;
      }

      case MSG_TYPE_RRC_SECURITY_MODE_COMPLETE:
      {
         /* For security mode complete where RRC Container is dummy
          *
          * MAC subheader format is R/F/LCId/L (2/3 bytes)
          * LCId is 1 for SRB1
          * L is length of PDU i.e 6bytes here 
          * From 38.321 section 6.1.1
          *
          * RLC subheader for AM PDU is D/C/P/SI/SN (2 bytes for 12-bit SN)
          * From 38.322, section 6.2.2.4
          */
         DU_LOG("\nDEBUG  -->  PHY_STUB: Forming RRC SECURITY MODE COMPLETE PDU");
         uint8_t  pduLen = 12; /* Length of PDU */
         pduLen += 2; /* RLC subheader */
         uint8_t msg[] = {1, pduLen, 128, phyDb.ueDb.ueCb[ueId-1].rlcSnForSrb1++, 0, phyDb.ueDb.ueCb[ueId-1].pdcpSn++, 0x2a, 0x40, \
            0, 0, 0, 0, 0, 0, 0, 0};

         pduLen += 2;  /* 2 bytes of MAC header */
         memcpy(pdu, &msg, pduLen);
         byteIdx += pduLen; /* 4 bytes of header : MAC+RLC */
         break;
      }

      case MSG_TYPE_REGISTRATION_COMPLETE:
      {
         /* For rrc reconfig complete where RRC Container is dummy
          *
          * MAC subheader format is R/F/LCId/L (2/3 bytes)
          * LCId is 1 for SRB1
          * L is length of PDU i.e 6bytes here
          * From 38.321 section 6.1.1
          * 
          * RLC subheader for AM PDU is D/C/P/SI/SN (2 bytes for 12-bit SN)
          * From 38.322, section 6.2.2.4
          */
         DU_LOG("\nDEBUG  -->  PHY_STUB: Forming RRC REGISTRATION COMPLETE PDU");
         uint8_t  pduLen = 12; /* Length of PDU */
         pduLen += 2; /* RLC subheader */
         uint8_t msg[] = {1, pduLen, 128, phyDb.ueDb.ueCb[ueId-1].rlcSnForSrb1++, 0, phyDb.ueDb.ueCb[ueId-1].pdcpSn++, 0x3a, 0x81, \
            0xbf, 0, 0x21, 0x80, 0, 0, 0, 0};

         pduLen += 2;  /* 2 bytes of MAC header */
         memcpy(pdu, &msg, pduLen);
         byteIdx += pduLen; /* 4 bytes of header : MAC+RLC */
         break;
      }

      case MSG_TYPE_RRC_RECONFIG_COMPLETE:
      {
         /* For rrc reconfig complete where RRC Container is dummy
          *
          * MAC subheader format is R/F/LCId/L (2/3 bytes)
          * LCId is 1 for SRB1
          * L is length of PDU i.e 6bytes here
          * From 38.321 section 6.1.1
          *
          * RLC subheader for AM PDU is D/C/P/SI/SN (2 bytes for 12-bit SN)
          * From 38.322, section 6.2.2.4
          */
         DU_LOG("\nDEBUG  -->  PHY_STUB: Forming RRC RECONFIGURATION COMPLETE PDU");
         uint8_t  pduLen = 13; /* PDU length */
         pduLen += 2; /* RLC sub header */
         uint8_t msg[] = {1, pduLen, 128, phyDb.ueDb.ueCb[ueId-1].rlcSnForSrb1++, 0, phyDb.ueDb.ueCb[ueId-1].pdcpSn++, 8, 64, 0, 0,\
            0, 0, 0, 0, 0, 0, 0};

         pduLen += 2;  /* 2bytes of MAC header */
         memcpy(pdu, &msg, pduLen);
         byteIdx += pduLen; /* 4 bytes of header : MAC+RLC*/
         break;

      }

      default:
      break;
   } /* End of switch(type) */

   /* Filling MAC SDU for Padding bytes*/
   if(byteIdx < pduInfo->pdu_length)
   {
      /* For Padding
         MAC subheader format is R/R/LCId (1byte)
         LCId is 63 for padding
         From 38.321 section 6.1.1
         */
      pdu[byteIdx++] = 63;

      for(; byteIdx < pduInfo->pdu_length; byteIdx++)
         pdu[byteIdx] = 0;
   }
   msgLen += pduInfo->pdu_length;

   fillMsgHeader(&rxDataInd->header, FAPI_RX_DATA_INDICATION, msgLen);

   /* Sending Rx data indication to MAC */
   DU_LOG("\nINFO   -->  PHY STUB: Sending Rx data Indication to MAC");
   procPhyMessages(rxDataInd->header.msg_id, sizeof(fapi_rx_data_indication_t), (void *)rxDataInd);

   if(pduInfo->pdu_length)
      MAC_FREE(pduInfo->pduData, pduInfo->pdu_length);
   MAC_FREE(rxDataInd, sizeof(fapi_rx_data_indication_t));

#ifdef START_DL_UL_DATA 
      if(phyDb.ueDb.ueCb[ueId-1].msgRrcReconfigComp == true)
      {
         startUlData();
      }
#endif
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
uint16_t l1BuildAndSendRachInd(uint16_t slot, uint16_t sfn, uint8_t raPreambleIdx)
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
      DU_LOG("\nERROR  -->  PHY_STUB: Memory allocation failed for Rach Indication Message");
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

   rachPdu->preambleInfo[preamIdx].preambleIndex = raPreambleIdx;
   rachPdu->preambleInfo[preamIdx].timingAdvance = 0;
   rachPdu->preambleInfo[preamIdx].preamblePwr = 0;

   fillMsgHeader(&rachInd->header, FAPI_RACH_INDICATION, \
	 sizeof(fapi_rach_indication_t) - sizeof(fapi_msg_t));

   /* Sending RACH indication to MAC */
   DU_LOG("\nINFO   -->  PHY STUB: Sending RACH Indication to MAC");
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
uint16_t l1BuildAndSendSlotIndication()
{
   Pst pst;
   Buffer *mBuf;

#ifdef INTEL_FAPI
   fapi_slot_ind_t *slotIndMsg;

   MAC_ALLOC_SHRABL_BUF(slotIndMsg, sizeof(fapi_slot_ind_t));
   if(!slotIndMsg)
   {
      DU_LOG("\nERROR  -->  PHY_STUB: Memory allocation failed for slot Indication Message");
      return RFAILED;
   }
   else
   {
      memset(slotIndMsg, 0, sizeof(fapi_slot_ind_t));
      slotIndMsg->sfn = sfnValue;
      slotIndMsg->slot = slotValue;

#ifdef ODU_SLOT_IND_DEBUG_LOG
      DU_LOG("\n\nDEBUG  -->  PHY_STUB: Sending Slot Indication [%d : %d] to MAC", sfnValue, slotValue);
#endif

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

      memset(&pst, 0, sizeof(Pst));
      FILL_PST_PHY_STUB_TO_LWR_MAC(pst, EVT_PHY_STUB_SLOT_IND);
      ODU_GET_MSG_BUF(pst.region, pst.pool, &mBuf);
      if(!mBuf)
      {
         DU_LOG("\nERROR  --> PHY_STUB: Failed to allocate memory for slot indication buffer");
         MAC_FREE_SHRABL_BUF(pst.region, pst.pool, slotIndMsg, sizeof(fapi_slot_ind_t));
         return RFAILED;
      }
      CMCHKPK(oduPackPointer, (PTR)slotIndMsg, mBuf);
      ODU_POST_TASK(&pst, mBuf);

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

S16 l1HdlStartReq(uint32_t msgLen, void *msg)
{
#ifdef INTEL_FAPI
   if(lwrMacCb.phyState == PHY_STATE_CONFIGURED)
   {
      l1HdlSlotIndicaion(FALSE);
      l1StartConsoleHandler();
      MAC_FREE(msg, msgLen);
   }
   else
   {
      DU_LOG("\nINFO   -->  PHY_STUB: Received Start Req in PHY State %d", lwrMacCb.phyState);
      MAC_FREE(msg, msgLen);
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

S16 l1HdlDlTtiReq(uint16_t msgLen, void *msg)
{
#ifdef INTEL_FAPI
   p_fapi_api_queue_elem_t dlTtiElem = (p_fapi_api_queue_elem_t)msg;
   fapi_dl_tti_req_t *dlTtiReq = (fapi_dl_tti_req_t *)(dlTtiElem +1);
   
   uint8_t pduCount = 0;

#ifdef ODU_SLOT_IND_DEBUG_LOG
   if(dlTtiReq->nPdus == 0)
   {
      DU_LOG("\nDEBUG  -->  PHY_STUB: No PDU in DL TTI Request");
   }
   else
   {  
     DU_LOG("\nDEBUG  -->  PHY STUB: DL TTI Request at sfn=%d slot=%d",dlTtiReq->sfn,dlTtiReq->slot);
   }
#endif 

   for(pduCount=0; pduCount<dlTtiReq->nPdus; pduCount++)
   {
      if(dlTtiReq->pdus[pduCount].pduType == 3) //SSB_PDU_TYPE
      {
         DU_LOG("\nINFO   -->  PHY_STUB: SSB PDU");
      }
      else if(dlTtiReq->pdus[pduCount].pduType == 0)
      {
         DU_LOG("\nINFO   -->  PHY_STUB: PDCCH PDU");
      }
      else if(dlTtiReq->pdus[pduCount].pduType == 1)
      {
         DU_LOG("\nINFO   -->  PHY_STUB: PDSCH PDU");
      }
   }

   /* Free FAPI message */
   MAC_FREE(msg, msgLen);
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

S16 l1HdlTxDataReq(uint16_t msgLen, void *msg)
{
#ifdef INTEL_FAPI
   p_fapi_api_queue_elem_t txDataElem = (p_fapi_api_queue_elem_t)msg;
   fapi_tx_data_req_t *txDataReq = (fapi_tx_data_req_t *)(txDataElem +1);

   DU_LOG("\nINFO   -->  PHY STUB: TX DATA Request at sfn=%d slot=%d",txDataReq->sfn,txDataReq->slot);
/*
   if(dlDedMsg)
   {
      DU_LOG("\nINFO   -->  PHY_STUB: TxDataPdu for DED MSG sent");
      dlDedMsg = false;
   }
*/
   MAC_FREE(msg, msgLen);
#endif
   return ROK;
}

#ifdef INTEL_FAPI
/*******************************************************************
 *
 * @brief Fills Uci Ind Pdu Info carried on Pucch Format 0/Format 1
 *
 * @details
 *
 *    Function : fillPucchF0F1PduInfo
 *
 *    Functionality:
 *       Fills Uci Ind Pdu Info carried on Pucch Format 0/Format 1 
 *
 * @params[in] fapi_uci_o_pucch_f0f1_t *
 *             pucchPdu
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillPucchF0F1PduInfo(fapi_uci_o_pucch_f0f1_t *pduInfo, fapi_ul_pucch_pdu_t pucchPdu)
{
   uint8_t idx = 0;
   static uint8_t ind=0;
   uint8_t result[]={0,//msg4
                     0,//Security Mode Command
                     0,//Registration Accept
                     0,//RRC Reconfiguration
                     0,//Data 1
                     0,//Data 2
                     0,//Data 3
                     0,//Data 4
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,};

   pduInfo->handle = pucchPdu.handle;
   pduInfo->pduBitmap = 1;  //hardcoded for SR
   if (pucchPdu.bitLenHarq)
   {
      pduInfo->pduBitmap |= HARQ_PDU_BITMASK;
   }
   pduInfo->pucchFormat = pucchPdu.formatType;
   pduInfo->ul_cqi = 0;
   pduInfo->rnti = pucchPdu.rnti;
   pduInfo->timingAdvance = 0;
   pduInfo->rssi = 0;
   if(pduInfo->pduBitmap & SR_PDU_BITMASK)
   {
      if (result[ind%50] == 0)
      {
         pduInfo->srInfo.srIndication = SR_DETECTED;
         pduInfo->srInfo.srConfidenceLevel = CONFDC_LEVEL_GOOD;
      }
   }
   if(pduInfo->pduBitmap & HARQ_PDU_BITMASK)
   {
      pduInfo->harqInfo.numHarq++;
      pduInfo->harqInfo.harqConfidenceLevel = CONFDC_LEVEL_GOOD;
      for(idx = 0; idx < pduInfo->harqInfo.numHarq; idx++)
      {
         pduInfo->harqInfo.harqValue[idx] = result[ind%50];
         ind++;
         /*TBD: To use harq ind with random number and percentage*/
         //pduInfo->harqInfo.harqValue[idx] = (dlHqPassPer >= rand()%(100))?HARQ_PASS:HARQ_FAIL;
      }
   }
   return ROK;
}
/*******************************************************************
 *
 * @brief Fills UCI Pdu Information
 *
 * @details
 *
 *    Function : fillUciPduInfo
 *
 *    Functionality:
 *       Fills UCI Pdu Information
 *
 * @params[in] Pointer to uciPdu
 *             pucchPdu
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillUciPduInfo(fapi_uci_pdu_info_t *uciPdu, fapi_ul_pucch_pdu_t pucchPdu)
{
   uint8_t ret = ROK;

   /*TODO: The pduType is hardcoded here to support 
     UCI Ind for PUCCH forat0/format1. This is to be
     modified when we get SR form UE */
   uciPdu->pduType = UCI_IND_PUCCH_F0F1;
   switch(uciPdu->pduType)
   {
      case UCI_IND_PUSCH:
         break;
      case UCI_IND_PUCCH_F0F1:
         {
            fapi_uci_o_pucch_f0f1_t *pduInfo = NULLP;

            pduInfo = &uciPdu->uci.uciPucchF0F1;
            ret = fillPucchF0F1PduInfo(pduInfo, pucchPdu);
            uciPdu->pduSize = sizeof(fapi_uci_o_pucch_f0f1_t);
         }
         break;
      case UCI_IND_PUCCH_F2F3F4:
         break;
      default:
         DU_LOG("\nERROR  -->  PHY_STUB: Invalid Pdu Type %d", uciPdu->pduType);
         break;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Build and send Uci indication
 *
 * @details
 *
 *    Function : l1BuildAndSendUciInd
 *
 *    Functionality:
 *       Build and send Uci indication
 *
 * @params[in] SFN
 *             Slot
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t l1BuildAndSendUciInd(uint16_t slot, uint16_t sfn, fapi_ul_pucch_pdu_t pucchPdu)
{
   uint8_t uciIdx = 0;
   uint8_t nUciPdus = 0;
   uint8_t ret = ROK;
   uint32_t msgLen = 0;
   fapi_uci_indication_t *uciInd =  NULLP;

   MAC_ALLOC(uciInd, sizeof(fapi_uci_indication_t));
   if(!uciInd)
   {
      DU_LOG("\nERROR  -->  PHY_STUB: Memory allocation failed for UCI Indication");
      return RFAILED;
   }
   memset(uciInd, 0, sizeof(fapi_uci_indication_t));
   uciInd->sfn = sfn;
   uciInd->slot = slot;
   uciInd->numUcis = 1;   //consdering the UCI Ind for SR
   nUciPdus = uciInd->numUcis;
   while(nUciPdus)
   {
      ret = fillUciPduInfo(&uciInd->uciPdu[uciIdx], pucchPdu);
      uciIdx++;
      nUciPdus--;
   }
   if(ret == ROK)
   {
      msgLen = sizeof(fapi_uci_indication_t)- sizeof(fapi_msg_t);
      fillMsgHeader(&uciInd->header, FAPI_UCI_INDICATION, msgLen);

      /* Sending UCI indication to MAC */
      DU_LOG("\nINFO   -->  PHY STUB: Sending UCI Indication to MAC");
      procPhyMessages(uciInd->header.msg_id, sizeof(fapi_uci_indication_t), (void *)uciInd);
   }
   MAC_FREE(uciInd, sizeof(fapi_uci_indication_t));
   return ret;
}
#endif

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

S16 l1HdlUlTtiReq(uint16_t msgLen, void *msg)
{
#ifdef INTEL_FAPI
   p_fapi_api_queue_elem_t ulTtiElem = (p_fapi_api_queue_elem_t)msg;
   fapi_ul_tti_req_t *ulTtiReq = (fapi_ul_tti_req_t *)(ulTtiElem +1);
   uint8_t numPdus = ulTtiReq->nPdus;

#ifdef ODU_SLOT_IND_DEBUG_LOG
   if(numPdus == 0)
   {
      DU_LOG("\nINFO   -->  PHY STUB: No PDU received in UL TTI Req");
   }
   else
   {
      DU_LOG("\nINFO   -->  PHY STUB: Received UL TTI Request");
   }
#endif 

   while(numPdus)
   {
      if(ulTtiReq->pdus[numPdus-1].pduType == 0)
      {
         DU_LOG("\nINFO   -->  PHY STUB: PRACH PDU");

         /* Send RACH Ind to L2 for first UE */
         if(phyDb.ueDb.ueCb[UE_IDX_0].rachIndSent == false)
         {
            phyDb.ueDb.ueCb[UE_IDX_0].isCFRA = false;
            phyDb.ueDb.ueCb[UE_IDX_0].rachIndSent = true;
            l1BuildAndSendRachInd(ulTtiReq->slot, ulTtiReq->sfn, CB_RA_PREAMBLE_IDX);
            phyDb.ueDb.numActvUe++;
         }
#if 0
         /* Send RACH Ind to L2 for second UE */
         if(phyDb.ueDb.ueCb[UE_IDX_1].rachIndSent == false && phyDb.ueDb.ueCb[UE_IDX_0].msgRrcReconfigComp == true)
         {
            phyDb.ueDb.ueCb[UE_IDX_0].isCFRA = false;
            phyDb.ueDb.ueCb[UE_IDX_1].rachIndSent = true;
            l1BuildAndSendRachInd(ulTtiReq->slot, ulTtiReq->sfn, CB_RA_PREAMBLE_IDX);
            phyDb.ueDb.numActvUe++;
         }

         /* Send RACH Ind to L2 for third UE */
         if(phyDb.ueDb.ueCb[UE_IDX_2].rachIndSent == false && phyDb.ueDb.ueCb[UE_IDX_1].msgRrcReconfigComp == true)
         {
            phyDb.ueDb.ueCb[UE_IDX_0].isCFRA = false;
            phyDb.ueDb.ueCb[UE_IDX_2].rachIndSent = true;
            l1BuildAndSendRachInd(ulTtiReq->slot, ulTtiReq->sfn, CB_RA_PREAMBLE_IDX);
            phyDb.ueDb.numActvUe++;
         }
#endif       
      }
      if(ulTtiReq->pdus[numPdus-1].pduType == 1)
      {
         DU_LOG("\nINFO   -->  PHY STUB: PUSCH PDU");
         if (ROK == l1BuildAndSendCrcInd(ulTtiReq->slot, ulTtiReq->sfn,ulTtiReq->pdus[numPdus-1].pdu.pusch_pdu))
         {
            l1BuildAndSendRxDataInd(ulTtiReq->slot, ulTtiReq->sfn, ulTtiReq->pdus[numPdus-1].pdu.pusch_pdu); 
         }
      }
      if(ulTtiReq->pdus[numPdus-1].pduType == 2)
      {
         DU_LOG("\nINFO   -->  PHY STUB: PUCCH PDU");

         fapi_ul_tti_req_t ulTtiSlotInd;
         memset(&ulTtiSlotInd, 0, sizeof(fapi_ul_tti_req_t));
         ulTtiSlotInd.slot = ulTtiReq->slot;
         ulTtiSlotInd.sfn  = ulTtiReq->sfn;
         ADD_DELTA_TO_TIME(ulTtiSlotInd, ulTtiSlotInd, SLOT_DELAY, MAX_SLOTS);
         l1BuildAndSendUciInd(ulTtiSlotInd.slot, ulTtiSlotInd.sfn, ulTtiReq->pdus[numPdus-1].pdu.pucch_pdu);
      }
      numPdus--;
   }

   MAC_FREE(msg, msgLen);
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
uint16_t l1BuildAndSendStopInd()
{
#ifdef INTEL_FAPI
   Pst pst;
   Buffer *mBuf = NULLP;
   fapi_stop_ind_t *stopIndMsg = NULLP;
   uint32_t msgLen = 0;

   MAC_ALLOC_SHRABL_BUF(stopIndMsg, sizeof(fapi_stop_ind_t));
   if(!stopIndMsg)
   {
      DU_LOG("\nERROR  -->  PHY_STUB: Memory allocation failed for stop Indication Message");
      return RFAILED;
   }
   else
   {
      fillMsgHeader(&stopIndMsg->header, FAPI_STOP_INDICATION, msgLen);
      DU_LOG("\n\nINFO   -->  PHY_STUB: Processing Stop indication to MAC");

      memset(&pst, 0, sizeof(Pst));
      FILL_PST_PHY_STUB_TO_LWR_MAC(pst, EVT_PHY_STUB_STOP_IND);
      ODU_GET_MSG_BUF(pst.region, pst.pool, &mBuf);
      if(!mBuf)
      {
         DU_LOG("\nERROR  --> PHY_STUB: Failed to allocate memory for slot indication buffer");
         MAC_FREE_SHRABL_BUF(pst.region, pst.pool, stopIndMsg, sizeof(fapi_stop_ind_t));
         return RFAILED;
      }
      CMCHKPK(oduPackPointer, (PTR)stopIndMsg, mBuf);
      ODU_POST_TASK(&pst, mBuf);
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

S16 l1HdlStopReq(uint32_t msgLen, void *msg)
{
#ifdef INTEL_FAPI
   if(lwrMacCb.phyState == PHY_STATE_RUNNING)
   {
      l1HdlSlotIndicaion(TRUE);

      /* Initialize all global variables */
      sfnValue = 0;
      slotValue = 0;
      memset(&phyDb.ueDb, 0, sizeof(UeDb));

      DU_LOG("\nINFO   -->  PHY_STUB: Slot Indication is stopped successfully");
      MAC_FREE(msg, msgLen);
   }
   else
   {
      DU_LOG("\nINFO  -->  PHY_STUB: Received Stop Req in PHY State %d", lwrMacCb.phyState);
      MAC_FREE(msg, msgLen);
      return RFAILED;
   }
#endif
   return ROK;
}

#if 0
/*******************************************************************
 *
 * @brief Build And Send Rx Data Ind for Msg5
 *
 * @details
 *
 *    Function : l1BuildAndSendMsg5
 *
 *    Functionality: Build And Send Rx Data Ind for Msg5
 *
 * @params[in] SFN
 *             Slot
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t l1BuildAndSendMsg5(uint16_t sfn, uint16_t slot)
{   
#ifdef INTEL_FAPI
   uint8_t idx = 0;
   fapi_rx_data_indication_t *rxDataInd;
   fapi_pdu_ind_info_t       *pduInfo;
   uint8_t  *pdu;
   uint16_t byteIdx = 0;
   uint32_t msgLen = 0;

   MAC_ALLOC(rxDataInd, sizeof(fapi_rx_data_indication_t));
   if(!rxDataInd)
   {
      DU_LOG("\nERROR  -->  PHY_STUB: Memory allocation failed for Rx Data Indication for msg5");
      return RFAILED;
   }
   memset(rxDataInd, 0, sizeof(fapi_rx_data_indication_t));

   /* TODO: Fill the required values */
   msgLen = sizeof(fapi_rx_data_indication_t) - sizeof(fapi_msg_t);
   rxDataInd->sfn = sfn;
   rxDataInd->slot = slot;
   rxDataInd->numPdus = 1;

   pduInfo = &rxDataInd->pdus[idx];
   pduInfo->handle = 100;
   pduInfo->rnti = 100;
   pduInfo->harqId = 1;
   pduInfo->pdu_length = 25;
   pduInfo->ul_cqi = 0;
   pduInfo->timingAdvance = 0;
   pduInfo->rssi = 0;

   /* Filling pdu with random values for testing */
   pduInfo->pduData = NULL;
   MAC_ALLOC(pduInfo->pduData, pduInfo->pdu_length);
   if(!pduInfo->pduData)
   {
      DU_LOG("\nERROR  -->  PHY_STUB: Memory allocation failed for Rx Data Pdu");
      return RFAILED;
   }

   /* Filling PDU */
   pdu = (uint8_t *)pduInfo->pduData;

   uint8_t  msg5PduLen = 33;
   /* For RRC setup complete
      MAC subheader format is R/F/LCId/L (2/3 bytes)
      LCId is 1 for SRB1
      L is length of PDU i.e 6bytes here
      From 38.321 section 6.1.1
    */
   uint8_t msg5[] = {1, msg5PduLen, 0, 0, 16, 0, 5, 223, 128, 16, 94, \
      64, 3, 64, 89, 61, 138, 64, 0, 0, 0, 4, 0, 0, 4, 68, 11, 128, \
	 184, 56, 0, 0, 0, 0, 0};

   msg5PduLen += 2;  /* 2bytes of header */
   memcpy(pdu, &msg5, msg5PduLen);
   byteIdx += msg5PduLen; /* 2 bytes of header */

   /* Filling MAC SDU for Padding bytes*/
   if(byteIdx < pduInfo->pdu_length)
   {
      /* For Padding
	 MAC subheader format is R/R/LCId (1byte)
	 LCId is 63 for padding
	 From 38.321 section 6.1.1
       */
      pdu[byteIdx++] = 63;

      for(; byteIdx < pduInfo->pdu_length; byteIdx++)
	 pdu[byteIdx] = 0;
   }
   msgLen += pduInfo->pdu_length;

   fillMsgHeader(&rxDataInd->header, FAPI_RX_DATA_INDICATION, msgLen);

   /* Sending Rx data indication to MAC */
   DU_LOG("\nINFO   -->  PHY STUB: Sending Rx data Indication to MAC");
   procPhyMessages(rxDataInd->header.msg_id, sizeof(fapi_rx_data_indication_t), (void *)rxDataInd);

   if(pduInfo->pdu_length)
      MAC_FREE(pduInfo->pduData, pduInfo->pdu_length);
   MAC_FREE(rxDataInd, sizeof(fapi_rx_data_indication_t));
#endif
   return ROK;
}
#endif

/*******************************************************************
 *
 * @brief Handles Ul Dci request received from MAC
 *
 * @details
 *
 *    Function : l1HdlUlDciReq
 *
 *    Functionality:
 *          -Handles Ul Dci request received from MAC
 *
 * @params[in]   Message length
 *               Ul Dci request message pointer
 *
 * @return void
 *
 * ****************************************************************/

S16 l1HdlUlDciReq(uint16_t msgLen, void *msg)
{
#ifdef INTEL_FAPI
   p_fapi_api_queue_elem_t ulDciElem = (p_fapi_api_queue_elem_t)msg;
   fapi_ul_dci_req_t *ulDciReq = (fapi_ul_dci_req_t *)(ulDciElem +1);
   uint8_t numPdus = ulDciReq->numPdus;

   while(numPdus)
   {
      if(ulDciReq->pdus[numPdus-1].pduType == 0)
      {
         DU_LOG("\nINFO   -->  PHY STUB: Received UL DCI Request for PDCCH PDU");
	 //l1BuildAndSendMsg5(ulDciReq->sfn, ulDciReq->slot);
	 //msg5Sent = true;
      }
      numPdus--;
   }

   MAC_FREE(msg, msgLen);
#endif
   return ROK;
}

/*******************************************************************
 *
 * @brief Send UL user data to DU
 *
 * @details
 *
 *    Function : l1SendUlUserData
 *
 *    Functionality: Send UL user data to DU
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t l1SendUlUserData(uint8_t drbId, uint8_t ueIdx)
{
   uint8_t cnt = 0;
   fapi_rx_data_indication_t *rxDataInd;
   fapi_pdu_ind_info_t       *pduInfo;
   uint8_t  *pdu = NULLP;
   uint16_t byteIdx = 0;
   uint32_t msgLen = 0;
   uint8_t idx = 0;
   uint8_t lcId = 0;

   MAC_ALLOC(rxDataInd, sizeof(fapi_rx_data_indication_t));
   if(!rxDataInd)
   {
      DU_LOG("\nERROR  -->  PHY_STUB: Memory allocation failed for Rx Data Indication");
      return RFAILED;
   }
   memset(rxDataInd, 0, sizeof(fapi_rx_data_indication_t));

   msgLen = sizeof(fapi_rx_data_indication_t) - sizeof(fapi_msg_t);
   rxDataInd->sfn = 0;
   rxDataInd->slot = 0;
   rxDataInd->numPdus = 1;

   /* TODO : Fill pduInfo using PUSCH PDU. Currently hardcoding */
   pduInfo = &rxDataInd->pdus[idx];
   pduInfo->handle = ueIdx + ODU_START_CRNTI;
   pduInfo->rnti = ueIdx + ODU_START_CRNTI;
   pduInfo->harqId = 1;
   /* Since user data size = 50bytes and 2 bytes of MAC header and 3 byte of RLC header, 
    * setting tbsize = 56 from Table 5.1.3.2-1 spec 38.214 */
   pduInfo->pdu_length = 56;
   pduInfo->ul_cqi = 0;
   pduInfo->timingAdvance = 0;
   pduInfo->rssi = 0;

   /* Filling pdu with random values for testing */
   pduInfo->pduData = NULL;
   MAC_ALLOC(pduInfo->pduData, pduInfo->pdu_length);
   if(!pduInfo->pduData)
   {
      DU_LOG("\nERROR  -->  PHY_STUB: Memory allocation failed for Rx Data Pdu");
      return RFAILED;
   }

   /* Filling PDU */
   pdu = (uint8_t *)pduInfo->pduData;
   msgLen = 52;

   /* For UL User data
      MAC subheader format is R/F/LCId/L (2/3 bytes)
      LCId is 4 for DRB1
      L is length of PDU i.e 50 bytes
      From 38.321 section 6.1.1
      From 38.322 section 6.2.2.4, 6.2.2.3 for AM, UM Header
    */

    /* Below ulMsg supports 12bit SN for UM mode */
                               /*  SI  SN */
    lcId = MIN_DRB_LCID + drbId;
    uint8_t ulMsg[] = {lcId, msgLen,   0, 0, 0, 0, 0, 50, 0, 0, 0, 0, 0, 1, 0, 0, 192, 168, 130, 81, 192, 168, 130, 82, 84, 104,
    105, 115, 32, 105, 115, 32, 69, 71, 84, 80, 32, 100, 97, 116, 97, 32, 102, 114, 111, 109, 32, 68, 85, 0, 0, 0, 0, 0};
    msgLen += 2;  /* 2bytes of header */
    memcpy(pdu, &ulMsg, msgLen);
    byteIdx += msgLen; /* 2 bytes of header */

   /* Filling MAC SDU for Padding bytes*/
   if(byteIdx < pduInfo->pdu_length)
   {
      /* For Padding
         MAC subheader format is R/R/LCId (1byte)
         LCId is 63 for padding
         From 38.321 section 6.1.1
       */
      pdu[byteIdx++] = 63;

      for(; byteIdx < pduInfo->pdu_length; byteIdx++)
         pdu[byteIdx] = 0;
   }
   msgLen += pduInfo->pdu_length;

   fillMsgHeader(&rxDataInd->header, FAPI_RX_DATA_INDICATION, msgLen);

    /* Send Message to peer */
    while(cnt < NUM_UL_PACKETS)
    {
       DU_LOG("\nDEBUG  -->  PHY STUB : Sending UL User Data[%d][LCID:%d] at sfn %d slot %d", cnt+1, lcId, sfnValue, slotValue);
       /* Sending Rx data indication to MAC */
       rxDataInd->sfn = sfnValue;
       rxDataInd->slot = slotValue;
       procPhyMessages(rxDataInd->header.msg_id, sizeof(fapi_rx_data_indication_t), (void *)rxDataInd);
       cnt++;
    }

    if(pduInfo->pdu_length)
       MAC_FREE(pduInfo->pduData, pduInfo->pdu_length);
    MAC_FREE(rxDataInd, sizeof(fapi_rx_data_indication_t));
    return ROK;
}

/*******************************************************************
 *
 * @brief Sends RLC Status PDU to DU
 *
 * @details
 *
 *    Function : l1SendStatusPdu
 *
 *    Functionality: Send RLC Status PDU to DU
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t l1SendStatusPdu()
{
   fapi_rx_data_indication_t *rxDataInd;
   fapi_pdu_ind_info_t       *pduInfo;
   uint8_t  *pdu = NULLP;
   uint16_t byteIdx = 0;
   uint32_t msgLen = 0;
   uint8_t idx = 0;

   MAC_ALLOC(rxDataInd, sizeof(fapi_rx_data_indication_t));
   if(!rxDataInd)
   {
      printf("\nERROR  -->  PHY_STUB: Memory allocation failed for Rx Data Indication");
      return RFAILED;
   }
   memset(rxDataInd, 0, sizeof(fapi_rx_data_indication_t));

   msgLen = sizeof(fapi_rx_data_indication_t) - sizeof(fapi_msg_t);
   rxDataInd->sfn = 0;
   rxDataInd->slot = 0;
   rxDataInd->numPdus = 1;

   /* TODO : Fill pduInfo using PUSCH PDU. Currently hardcoding */
   pduInfo = &rxDataInd->pdus[idx];
   pduInfo->handle = 100;
   pduInfo->rnti = 100;
   pduInfo->harqId = 1;
   /* Since status pdu size = 3bytes and 2 bytes of MAC header,
    * setting tbsize = 24 from Table 5.1.3.2-1 spec 38.214 */
   pduInfo->pdu_length = 24;
   pduInfo->ul_cqi = 0;
   pduInfo->timingAdvance = 0;
   pduInfo->rssi = 0;

   /* Filling pdu with random values for testing */
   pduInfo->pduData = NULL;
   MAC_ALLOC(pduInfo->pduData, pduInfo->pdu_length);
   if(!pduInfo->pduData)
   {
      printf("\nERROR  -->  PHY_STUB: Memory allocation failed for Rx Data Pdu");
      return RFAILED;
   }

   /* Filling PDU */
   pdu = (uint8_t *)pduInfo->pduData;
   msgLen = 3;

   /* For RLC Status PDU
    * MAC subheader format is R/F/LCId/L (2/3 bytes)
    * LCId is 4 for DRB1
    * L is length of Status PDU i.e 3 bytes
    * From 38.321 section 6.1.1
    *
    * RLC Status PDU format :
    * ---------------------------------
    * |D/C|CPT|    ACK_SN             |
    * ---------------------------------
    * |            ACK_SN             |
    * ---------------------------------
    * |E1 | R | R | R | R | R | R | R |
    * ---------------------------------
    *  
    * Here, D/C bit = 0 for Control PDU
    * CPT bit = 0 since control pdu type is Status PDU
    * ACK_SN = 0 as of now, this can be changed based on the 
    * scenario being tested
    * E1 = 0 because no NACK_SN, E1, E2 and E3 follows it
    * R = 0 i.e. reserved bits
    * From Spec 38.322 section 6.2.2.5
    */
   uint8_t statusPdu[] = {4, msgLen, 0, 0, 0};
   msgLen += 2;  /* 2bytes of header */
   memcpy(pdu, &statusPdu, msgLen);
   byteIdx += msgLen; /* 2 bytes of header */


   /* Filling MAC SDU for Padding bytes*/
   if(byteIdx < pduInfo->pdu_length)
   {
      /* For Padding
         MAC subheader format is R/R/LCId (1byte)
         LCId is 63 for padding
         From 38.321 section 6.1.1
       */
      pdu[byteIdx++] = 63;

      for(; byteIdx < pduInfo->pdu_length; byteIdx++)
         pdu[byteIdx] = 0;
   }
   msgLen += pduInfo->pdu_length;

   fillMsgHeader(&rxDataInd->header, FAPI_RX_DATA_INDICATION, msgLen);

    /* Send Message to peer */
    DU_LOG("\nDEBUG  -->  PHY STUB : Sending RLC status pdu at sfn %d slot %d", sfnValue, slotValue);
    /* Sending Rx data indication to MAC */
    rxDataInd->sfn = sfnValue;
    rxDataInd->slot = slotValue;
    procPhyMessages(rxDataInd->header.msg_id, sizeof(fapi_rx_data_indication_t), (void *)rxDataInd);

    if(pduInfo->pdu_length)
       MAC_FREE(pduInfo->pduData, pduInfo->pdu_length);
    MAC_FREE(rxDataInd, sizeof(fapi_rx_data_indication_t));
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
      case FAPI_UL_DCI_REQUEST:
         l1HdlUlDciReq(msgLen, msg);
         break;
      default:
         DU_LOG("\nERROR  -->  PHY_STUB: Invalid message type[%x] received at PHY", msgType);
         break;
#endif
   }
}

#ifdef INTEL_FAPI
/*******************************************************************
 *
 * @brief Builds and Send the BSR message to MAC
 *
 * @details
 *
 *    Function : l1BuildAndSendBSR
 *
 *   Functionality:
 *          -Send the BSR Message to MAC
 *
 * @params[in]  BSR type 
 *              array of LCGID and BSIdx
 * @return void
 *
 *****************************************************************/
uint16_t l1BuildAndSendBSR(uint8_t ueIdx, BsrType bsrType,\
             LcgBufferSize lcgBsIdx[MAX_NUM_LOGICAL_CHANNEL_GROUPS])
{
   fapi_rx_data_indication_t *rxDataInd;
   fapi_pdu_ind_info_t       *pduInfo;
   uint8_t  *pdu = NULLP;
   uint16_t byteIdx = 0;
   uint32_t msgLen = 0;
   uint8_t pduIdx = 0, lcgIdx = 0, lcgIdxPos = 0;

   MAC_ALLOC(rxDataInd, sizeof(fapi_rx_data_indication_t));
   if(!rxDataInd)
   {
      DU_LOG("\nERROR  -->  PHY_STUB: Memory allocation failed for Rx Data Indication");
      return RFAILED;
   }
   memset(rxDataInd, 0, sizeof(fapi_rx_data_indication_t));

   msgLen = sizeof(fapi_rx_data_indication_t) - sizeof(fapi_msg_t);
   rxDataInd->sfn = 0;
   rxDataInd->slot = 0;
   rxDataInd->numPdus = 1;

   pduInfo = &rxDataInd->pdus[pduIdx];
   pduInfo->handle = (ODU_START_CRNTI + ueIdx);
   pduInfo->rnti = (ODU_START_CRNTI + ueIdx);
   pduInfo->harqId = 1;

   /* Since status pdu size = 3bytes and 2 bytes of MAC header,
    * setting tbsize = 24 from Table 5.1.3.2-1 spec 38.214 */
   pduInfo->pdu_length = 24;
   pduInfo->ul_cqi = 0;
   pduInfo->timingAdvance = 0;
   pduInfo->rssi = 0;

   /* Filling pdu with random values for testing */
   pduInfo->pduData = NULL;
   MAC_ALLOC(pduInfo->pduData, pduInfo->pdu_length);
   if(!pduInfo->pduData)
   {
      DU_LOG("\nERROR  -->  PHY_STUB: Memory allocation failed for Rx Data Pdu");
      MAC_FREE(rxDataInd, sizeof(fapi_rx_data_indication_t));
      return RFAILED;
   }

   /* Filling PDU */
   pdu = (uint8_t *)pduInfo->pduData;

   switch(bsrType)
   {
      case SHORT_BSR:
         {
            DU_LOG("\nDEBUG  -->  PHY_STUB: Forming SHORT BSR PDU ");

            /* For Short BSR
             * MAC subheader format is R/R/LcId (1Byte)
             * LCId is 61
             * From 38.321 section 6.1.1
             */
            pdu[byteIdx++] = 61;    // LCID
            pdu[byteIdx++] = (lcgBsIdx[0].lcgId << 5) | lcgBsIdx[0].bsIdx;
            break;
         }

      case LONG_BSR:
         {
            DU_LOG("\nDEBUG  -->  PHY_STUB: Forming LONG BSR PDU ");

            /* For Long BSR
             * MAC subheader format is R/R/LcId (1Byte)
             * LCId is 62
             * From 38.321 section 6.1.1
             */
            pdu[byteIdx++] = 62;    // LCID

            /*Octet where lcgId bitmap will be present*/
            lcgIdxPos = byteIdx;
            byteIdx++;
            for(lcgIdx = 0; lcgIdx < MAX_NUM_LOGICAL_CHANNEL_GROUPS; lcgIdx++)
            {
               if(lcgBsIdx[lcgIdx].bsIdx > 0)
               {
                  pdu[lcgIdxPos] |= 1 << lcgBsIdx[lcgIdx].lcgId;
                  pdu[byteIdx++] = lcgBsIdx[lcgIdx].bsIdx;
               }
            }

            break;
         }

      default:
         {
            DU_LOG("\nERROR  -->  PHY_STUB: Incorrect BSR type:%d!", bsrType);
            if(pduInfo->pdu_length)
               MAC_FREE(pduInfo->pduData, pduInfo->pdu_length);
            MAC_FREE(rxDataInd, sizeof(fapi_rx_data_indication_t));
            return RFAILED;
         }
   }
   /* Filling MAC SDU for Padding bytes*/
   if(byteIdx < pduInfo->pdu_length)
   {
      /* For Padding
       * MAC subheader format is R/R/LCId (1byte)
       * LCId is 63 for padding
       * From 38.321 section 6.1.1
       */
      pdu[byteIdx++] = 63;

      for(; byteIdx < pduInfo->pdu_length; byteIdx++)
         pdu[byteIdx] = 0;
   }
   msgLen += pduInfo->pdu_length;
   fillMsgHeader(&rxDataInd->header, FAPI_RX_DATA_INDICATION, msgLen);

   /* Sending Rx data indication to MAC */
   DU_LOG("\nINFO   -->  PHY STUB: Sending Rx data Indication to MAC");
   procPhyMessages(rxDataInd->header.msg_id, sizeof(fapi_rx_data_indication_t), (void *)rxDataInd);

   if(pduInfo->pdu_length)
      MAC_FREE(pduInfo->pduData, pduInfo->pdu_length);
   MAC_FREE(rxDataInd, sizeof(fapi_rx_data_indication_t));
   return ROK;
}
#endif

/**********************************************************************
  End of file
 **********************************************************************/
