
/*******************************************************************************
 * ################################################################################
 * #   Copyright (c) [2017-2019] [Radisys]
 * #
 * #
 * #
 * #   Licensed under the Apache License, Version 2.0 (the "License");
 * #
 * #   you may not use this file except in compliance with the License.
 * #
 * #   You may obtain a copy of the License at
 * #
 * #
 * #
 * #       http://www.apache.org/licenses/LICENSE-2.0
 * #
 * #
 * #
 * #   Unless required by applicable law or agreed to in writing, software
 * #
 * #   distributed under the License is distributed on an "AS IS" BASIS,
 * #
 * #   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * #
 * #   See the License for the specific language governing permissions and
 * #
 * #   limitations under the License.
 * #
 * ################################################################################
 * *******************************************************************************/

/* This functions contains main() for pnf simulator */
#include "common_def.h"
#include <unistd.h>
#include "pnf_stub_p5_msg_hdl.h"
#include "nfapi_interface.h"
#include "pnf_stub_sctp.h"
#include "pnf_stub_p7_udp.h"
#include "pnf_stub.h"

/*********************************************************************************
 *
 * @Function Name: buildAndSendPnfReadyInd 
 *
 *
 * @Functionality: 
 *   Builds and Sends PNF_READY_IND(Ref: SCF 225, Sec 3.1.0)
 *
 *
 * *******************************************************************************/
uint8_t buildAndSendPnfReadyInd()
{
   uint8_t ret = ROK;
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("ERROR  --> NFAPI_PNF : Memory allocation failed in pnf_readyInd");
      return RFAILED;
   }
   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, 0, TAG_NFAPI_PNF_READY_IND, 0);
   CMCHKPK(oduPackPostUInt32, 123, mBuf); //version
   ret = pnfP5SctpSend(mBuf);
   if(ret == RFAILED)
   {
      ODU_PUT_MSG_BUF(mBuf);
   }
   return ret;
}

/*********************************************************************************
 *
 * @Function Name: buildAndSendPnfParamResp 
 *
 * @Functionality: 
 *    Build Pnf Param rsp as per  5G nFAPI Specification 3.1.2 PNF_PARAM.response 
 *    And Send to VNF
 *
 * *******************************************************************************/

uint8_t buildAndSendPnfParamResp()
{
   uint8_t ret = ROK,idx=0;
   Buffer *mBuf = NULLP;
   nFapi_pnf_param_general paramGeneral;

   memset(&paramGeneral, 0, sizeof(nFapi_pnf_param_general));
   paramGeneral.max_num_phy=1;

   if (ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("ERROR  --> NFAPI_PNF : Memory allocation failed in pnf_readyInd");
      return RFAILED;
   }
   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, 0, TAG_NFAPI_PNF_PARAM_RESP, 0);
   CMCHKPK(oduPackPostUInt8, 0, mBuf); 
   CMCHKPK(oduPackPostUInt8, 1, mBuf); 
   CMCHKPK(oduPackPostUInt16, TAG_NFAPI_PNF_PARAM_GENERAL, mBuf);
   CMCHKPK(oduPackPostUInt16, sizeof(nFapi_pnf_param_general), mBuf);
   CMCHKPK(oduPackPostUInt8, paramGeneral.nFapi_sync_mode, mBuf); 
   CMCHKPK(oduPackPostUInt8, paramGeneral.loc_mode, mBuf);
   CMCHKPK(oduPackPostUInt16, paramGeneral.max_num_phy, mBuf); 
   for(idx =0; idx < 3; idx++)
   {
      CMCHKPK(oduPackPostUInt8, paramGeneral.oui[idx], mBuf); 
   }
   CMCHKPK(oduPackPostUInt16, paramGeneral.numRfInstances, mBuf); 
   CMCHKPK(oduPackPostUInt16, paramGeneral.numDfeInstances, mBuf); 

   ret = pnfP5SctpSend(mBuf);
   if(ret == RFAILED)
   {
      ODU_PUT_MSG_BUF(mBuf);
   }
   return ret;
}

/*********************************************************************************
 *
 * @Function Name: buildAndSendPnfConfigResp
 *
 *
 * @Functionality:
 *   Builds and Sends PNF_CONFIG_RSP(Ref: SCF 225, Sec 3.1.4)
 *
 *
 * *******************************************************************************/
uint8_t buildAndSendPnfConfigResp()
{
   uint8_t ret = ROK;
   Buffer *mBuf = NULLP;

   DU_LOG("INFO   --> NFAPI_PNF: Building PNF_CONFIG_RSP");
   pnfCb.pnfCfgParams.pnfState=PNF_CONFIGURED;
   
   if(ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("ERROR  --> NFAPI_PNF : Memory allocation failed in pnf config rsp");
      return RFAILED;
   }
   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, 0, TAG_NFAPI_PNF_CONFIG_RESP, 0);
   CMCHKPK(oduPackPostUInt32, 0, mBuf); //Error Code
   ret = pnfP5SctpSend(mBuf);
   if(ret == RFAILED)
   {
      ODU_PUT_MSG_BUF(mBuf);
   }
   return ret;
}

/*********************************************************************************
 *
 * @Function Name: buildAndSendPnfStartResp
 *
 *
 * @Functionality:
 *   Builds and Sends PNF_START_RSP(Ref: SCF 225, Sec 3.1.6)
 *
 *
 * *******************************************************************************/
uint8_t buildAndSendPnfStartResp()
{
   uint8_t ret = ROK;
   Buffer *mBuf = NULLP;

   DU_LOG("INFO   --> NFAPI_PNF: Building PNF_START_RSP");
   pnfCb.pnfCfgParams.pnfState=PNF_RUNNING;
   
   if(ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("ERROR  --> NFAPI_PNF : Memory allocation failed in pnf start rsp");
      return RFAILED;
   }
   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, 0, TAG_NFAPI_PNF_START_RESP, 0);
   CMCHKPK(oduPackPostUInt32, 0, mBuf); //Error Code
   ret = pnfP5SctpSend(mBuf);
   if(ret == RFAILED)
   {
      ODU_PUT_MSG_BUF(mBuf);
   }
   return ret;
}

/*********************************************************************************
 *
 * @Function Name: buildAndSendPnfStopResp
 *
 *
 * @Functionality:
 *   Builds and Sends PNF_STOP_RSP(Ref: SCF 225, Sec 3.1.8)
 *
 *
 * *******************************************************************************/
uint8_t buildAndSendPnfStopResp()
{
   uint8_t ret = ROK;
   Buffer *mBuf = NULLP;

   DU_LOG("INFO   --> NFAPI_PNF: Building PNF_STOP_RSP");
   pnfCb.pnfCfgParams.pnfState=PNF_CONFIGURED;

   if(ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("ERROR  --> NFAPI_PNF : Memory allocation failed in pnf stop rsp");
      return RFAILED;
   }
   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, 0, TAG_NFAPI_PNF_STOP_RESP, 0);
   CMCHKPK(oduPackPostUInt32, 0, mBuf); //Error Code
   ret = pnfP5SctpSend(mBuf);
   if(ret == RFAILED)
   {
      ODU_PUT_MSG_BUF(mBuf);
   }
   return ret;
}

/*********************************************************************************
 *
 * @Function Name: buildAndSendParamResp 
 *
 * @Functionality: 
 *    Build  Param rsp as per  5G nFAPI Specification 3.1.2 PARAM.response 
 *    And Send to VNF
 *
 * *******************************************************************************/

uint8_t buildAndSendParamResp()
{
   uint8_t pnfAdd[4];
   Buffer *mBuf = NULLP;
   uint32_t len = 20+sizeof(fapi_param_resp_t); //As per 5G nFAPI Specification, figure 2–21 Combined P5 message (PARAM.response),
                                                //where value 20 is calculated based on = 2 + 8 + 6 + ( 4 + L1 )

   if (ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("ERROR  --> NFAPI_PNF : Memory allocation failed in pnf_readyInd");
      return RFAILED;
   }

   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, 0, FAPI_PARAM_RESPONSE, len);

   //As per 5G nFAPI Specification, section 3.2.2 PARAM.response
   CMCHKPK(oduPackPostUInt8, 0, mBuf);  //Error Code
   CMCHKPK(oduPackPostUInt8, 3, mBuf);  //Num TLVs = 3 
   
   /* As per table 3-15 nFAPI TLVs included in PARAM.response when the PHY instance is in IDLE state 
    * Since we are only supporting ipv4 that's why fiiling only P7 PNF Address
    * Ipv4 and P7 PNF Port */
   convertIpStringToUInt8(LOCAL_IP_PNF, pnfAdd);
   fillTlvOfArrayOfUint8(mBuf, TAG_NFAPI_P7_PNF_ADD_IPV4, sizeof(pnfAdd), pnfAdd);  /* P7 PNF Address Ipv4 */
   fillTlvOfSizeUint16(mBuf, TAG_NFAPI_P7_PNF_PORT, sizeof(uint16_t), PNF_P7_UDP_PORT); /* P7 PNF Port */
   
   /* filling 5G_FAPI_MSG_BODY */
   CMCHKPK(oduPackPostUInt16, TAG_NFAPI_5G_FAPI_MSG_BODY, mBuf); //5G_FAPI_MSG_BODY tag
   CMCHKPK(oduPackPostUInt16, sizeof(fapi_param_resp_t), mBuf); // 5G_FAPI_MSG_BODY length

   //As per 5G FAPI: PHY API, section 3.3.1.2 PARAM.response, Table 3-5
   //PARAM.response message body
   CMCHKPK(oduPackPostUInt8, 0, mBuf);  //Error Code
   CMCHKPK(oduPackPostUInt8, 54, mBuf);  //Num TLVs = 54

   
   //As per 5G FAPI: PHY API, section 3.3.1.2 PARAM.response, Table 3-8 PARAM TLVs
   /* Cell Params */
   fillTlvOfSizeUint16(mBuf,FAPI_RELEASE_CAPABILITY_TAG, sizeof(uint16_t), 1);
   fillTlvOfSizeUint16(mBuf,FAPI_PHY_STATE_TAG, sizeof(uint16_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_SKIP_BLANK_DL_CONFIG_TAG, sizeof(uint8_t),  0);
   fillTlvOfSizeUint8(mBuf, FAPI_SKIP_BLANK_UL_CONFIG_TAG,  sizeof(uint8_t),  0);
   fillTlvOfSizeUint8(mBuf, FAPI_NUM_CONFIG_TLVS_TO_REPORT_TYPE_TAG,  sizeof(uint8_t),  0);

   /* Carrier Params */
   fillTlvOfSizeUint8(mBuf, FAPI_CYCLIC_PREFIX_TAG, sizeof(uint8_t),  1);
   fillTlvOfSizeUint8(mBuf, FAPI_SUPPORTED_SUBCARRIER_SPACING_DL_TAG,  sizeof(uint8_t),  1);
   fillTlvOfSizeUint16(mBuf,FAPI_SUPPORTED_BANDWIDTH_DL_TAG, sizeof(uint16_t), 1);
   fillTlvOfSizeUint8(mBuf, FAPI_SUPPORTED_SUBCARRIER_SPACING_UL_TAG, sizeof(uint8_t),  0);
   fillTlvOfSizeUint16(mBuf,FAPI_SUPPORTED_BANDWIDTH_UL_TAG, sizeof(uint16_t), 0);

   /* PDCCH Param*/
   fillTlvOfSizeUint8(mBuf, FAPI_CCE_MAPPING_TYPE_TAG,  sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_CORESET_OUTSIDE_FIRST_3_OFDM_SYMS_OF_SLOT_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PRECODER_GRANULARITY_CORESET_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PDCCH_MU_MIMO_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PDCCH_PRECODER_CYCLING_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_MAX_PDCCHS_PER_SLOT_TAG,  sizeof(uint8_t), 0);

   /* PUCCH Param */
   fillTlvOfSizeUint8(mBuf, FAPI_PUCCH_FORMATS_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_MAX_PUCCHS_PER_SLOT_TAG, sizeof(uint8_t), 0);

   /* PDSCH Param */
   fillTlvOfSizeUint8(mBuf, FAPI_PDSCH_MAPPING_TYPE_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PDSCH_ALLOCATION_TYPES_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PDSCH_VRB_TO_PRB_MAPPING_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PDSCH_CBG_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PDSCH_DMRS_CONFIG_TYPES_TAG,  sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PDSCH_DMRS_MAX_LENGTH_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PDSCH_DMRS_ADDITIONAL_POS_TAG,  sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_MAX_PDSCHS_TBS_PER_SLOT_TAG,  sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_MAX_NUMBER_MIMO_LAYERS_PDSCH_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_SUPPORTED_MAX_MODULATION_ORDER_DL_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_MAX_MU_MIMO_USERS_DL_TAG,  sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PDSCH_DATA_IN_DMRS_SYMBOLS_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PREMPTIONSUPPORT_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PDSCH_NON_SLOT_SUPPORT_TAG, sizeof(uint8_t), 0);

   /* PUSCH Param */
   fillTlvOfSizeUint8(mBuf, FAPI_UCI_MUX_ULSCH_IN_PUSCH_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_UCI_ONLY_PUSCH_TAG,  sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PUSCH_FREQUENCY_HOPPING_TAG,  sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PUSCH_DMRS_CONFIG_TYPES_TAG,  sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PUSCH_DMRS_MAX_LEN_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PUSCH_DMRS_ADDITIONAL_POS_TAG,  sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PUSCH_CBG_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PUSCH_MAPPING_TYPE_TAG,  sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PUSCH_ALLOCATION_TYPES_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PUSCH_VRB_TO_PRB_MAPPING_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PUSCH_MAX_PTRS_PORTS_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_MAX_PDUSCHS_TBS_PER_SLOT_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_MAX_NUMBER_MIMO_LAYERS_NON_CB_PUSCH_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_SUPPORTED_MODULATION_ORDER_UL_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_MAX_MU_MIMO_USERS_UL_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_DFTS_OFDM_SUPPORT_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PUSCH_AGGREGATION_FACTOR_TAG,  sizeof(uint8_t), 0);

   /* PRACH Params */
   fillTlvOfSizeUint8(mBuf, FAPI_PRACH_LONG_FORMATS_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PRACH_SHORT_FORMATS_TAG, sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_PRACH_RESTRICTED_SETS_TAG,  sizeof(uint8_t), 0);
   fillTlvOfSizeUint8(mBuf, FAPI_MAX_PRACH_FD_OCCASIONS_IN_A_SLOT_TAG, sizeof(uint8_t), 0);

   /* MEASUREMENT TAG */
   fillTlvOfSizeUint8(mBuf, FAPI_RSSI_MEASUREMENT_SUPPORT_TAG,  sizeof(uint8_t), 0);

   DU_LOG("INFO   --> NFAPI_PNF: Sending PARAM_RESP ");
   if(pnfP5SctpSend(mBuf) == RFAILED)
   {
      ODU_PUT_MSG_BUF(mBuf);
   }
   return ROK;
}

/*********************************************************************************
 *
 * @Function Name: buildAndSendConfigResp
 *
 * @Functionality:
 *    Build  Config rsp as per 5G nFAPI Specification 3.2.4 CONFIG.response
 *    And Send to VNF
 *
 * *******************************************************************************/

uint8_t buildAndSendConfigResp()
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("ERROR  --> NFAPI_PNF : Memory allocation failed in %s",__func__);
      return RFAILED;
   }

   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, 0, FAPI_CONFIG_RESPONSE, 0);

   //As per 5G nFAPI Specification, section 3.2.4 CONFIG.response
   CMCHKPK(oduPackPostUInt8, 0, mBuf);  //Error Code

   DU_LOG("INFO   --> NFAPI_PNF: Sending CONFIG_RESP ");
   if(pnfP5SctpSend(mBuf) == RFAILED)
   {
      ODU_PUT_MSG_BUF(mBuf);
   }
   return ROK;
}

/*********************************************************************************
 *
 * @Function Name: buildAndSendStartResp
 *
 * @Functionality:
 *    Build  Start rsp as per 5G nFAPI Specification 3.1.9 START.response
 *    And Send to VNF
 *
 *
 *******************************************************************************/

uint8_t buildAndSendStartResp()
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("ERROR  --> NFAPI_PNF : Memory allocation failed in start response");
      return RFAILED;
   }

   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, 0, TAG_NFAPI_START_RESPONSE, 0);

   CMCHKPK(oduPackPostUInt8, 0, mBuf);  //Error Code

   DU_LOG("INFO   --> NFAPI_PNF: Sending START_RESP ");
   if(pnfP5SctpSend(mBuf) == RFAILED)
   {
      ODU_PUT_MSG_BUF(mBuf);
   }
   return ROK;
}
/*********************************************************************************
 *
 * @Function Name: p5MsgHandlerAtPnf
 *
 *
 * @Functionality: 
 *    Handles the P5 MEssages at PNF SIM
 *
 * @Params [IN]: Message Buffer received at SCTP NFAPI P5 Interface
 *
 * *******************************************************************************/
uint8_t  p5MsgHandlerAtPnf(Buffer *mBuf)
{
   nFapi_p5_hdr p5Hdr;
   nFapi_msg_header msgHdr;
   uint8_t ret = ROK;

   nFapiExtractP5Hdr(&p5Hdr, mBuf);
   nFapiExtractMsgHdr(&msgHdr, mBuf);

   switch(msgHdr.msg_id)
   {
      case TAG_NFAPI_PNF_PARAM_REQ:
         {
            DU_LOG("INFO   --> NFAPI_PNF: PNF_PARAM_REQ recevied.");
            ret = buildAndSendPnfParamResp();
            break;
         }
      case TAG_NFAPI_PNF_CONFIG_REQ:
         {
            DU_LOG("INFO   --> NFAPI_PNF: PNF_CONFIG_REQ recevied.");
            ret = buildAndSendPnfConfigResp();
            break;
         }
      case TAG_NFAPI_PNF_START_REQ:
         {
            DU_LOG("INFO   --> NFAPI_PNF: PNF_START_REQ recevied.");
            ret = buildAndSendPnfStartResp();
            break;
         }
      case TAG_NFAPI_PNF_STOP_REQ:
         {
            DU_LOG("INFO   --> NFAPI_PNF: PNF_STOP_REQ recevied.");
            ret = buildAndSendPnfStopResp();
            break;
         }
      case FAPI_PARAM_REQUEST:
         {
            sleep(1);
            DU_LOG("INFO   --> NFAPI_PNF: PHY_PARAM_REQ recevied.");
            ret = buildAndSendParamResp();
            break;
         }
      case FAPI_CONFIG_REQUEST:
         {
            DU_LOG("INFO   --> NFAPI_PNF: PHY_CONFIG_REQ recevied.");
            ret = buildAndSendConfigResp();
            break;
         }
      case FAPI_START_REQUEST:
         {
            DU_LOG("INFO   --> NFAPI_PNF: PHY_START_REQ recevied.");
            ret = buildAndSendStartResp();
            break;
         }
      default:
      {
         DU_LOG("ERROR  --> NFAPI_PNF: Wrong MSGID of NFAPI P5 Message:%d",msgHdr.msg_id);
      }
   }

   if(ret == RFAILED)
   {
      return RFAILED;
   }
   return ret;
}

/**********************************************************************
End of file
**********************************************************************/
