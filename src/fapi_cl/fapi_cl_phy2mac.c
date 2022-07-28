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

/* This file converts FAPI messages sent from UE simulator/PHY to MAC */

#include "common_def.h"
#include "fapi_cl.h"
#include "fapi_cl_utils.h"
#include "fapi_interface_ue_sim.h"
#include "fapi_interface.h"
#include "fapi_vendor_extension.h"
#include "lwr_mac_fapi_cl_inf.h"

void procPhyMessages(uint16_t msgType, uint32_t msgSize, void *msg);

/*******************************************************************
 *
 * @brief Converts CONFIG.Response
 *
 * @details
 *
 *    Function :  convertConfigResponse
 *
 *    Functionality:
 *       - Converts CONFIG.response from UE sim's FAPI version to
 *         Mac's FAPI version.
 *
 * @params[in] Message pointer
 *
 * @return void
 *
 * ****************************************************************/
void convertConfigResponse(void *msg)
{
   uint16_t idx, numTlvs;
   fapi_config_resp_t *macCfgRsp = NULLP;
   fapi_config_resp_t_ue_sim *cfgRsp = (fapi_config_resp_t_ue_sim *)msg;

   DU_LOG("\nINFO  -->  FAPI_CL: Received CONFIG.response from UE simulator");

   FAPI_CL_ALLOC(macCfgRsp, sizeof(fapi_config_resp_t));
   if(!macCfgRsp)
   {
      DU_LOG("\nERROR  -->  FAPI_CL: Failed to allocate memory for config response");
      return;
   }
   macCfgRsp->header.msg_id = FAPI_CONFIG_RESPONSE;
   macCfgRsp->header.length = sizeof(fapi_config_resp_t) - sizeof(fapi_msg_t);
   macCfgRsp->error_code = cfgRsp->error_code;
   macCfgRsp->number_of_invalid_tlvs = cfgRsp->number_of_invalid_tlvs;
   macCfgRsp->number_of_inv_tlvs_idle_only = cfgRsp->number_of_inv_tlvs_idle_only;
   macCfgRsp->number_of_inv_tlvs_running_only = cfgRsp->number_of_inv_tlvs_running_only;
   macCfgRsp->number_of_missing_tlvs = cfgRsp->number_of_missing_tlvs;
   numTlvs = macCfgRsp->number_of_invalid_tlvs + macCfgRsp->number_of_inv_tlvs_idle_only + \
      macCfgRsp->number_of_inv_tlvs_running_only + macCfgRsp->number_of_missing_tlvs;
   for(idx = 0; idx < numTlvs; idx++)
   {
      macCfgRsp->tlvs[idx].tl.tag = cfgRsp->tlvs[idx].tl.tag;
      macCfgRsp->tlvs[idx].tl.length = cfgRsp->tlvs[idx].tl.length;
      macCfgRsp->tlvs[idx].value = cfgRsp->tlvs[idx].value;
   }
   procPhyMessages(macCfgRsp->header.msg_id, sizeof(fapi_config_resp_t), (void *)macCfgRsp);
   FAPI_CL_FREE(macCfgRsp, sizeof(fapi_config_resp_t));
}

/*******************************************************************
*
* @brief Builds and sends FAPI Slot indication to MAC
*
* @details
*
*    Function : buildAndSendSlotIndication
*
*    Functionality:
*       - Builds and sends FAPI Slot indication to MAC
*
* @params[in] 
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t buildAndSendSlotIndication()
{
   Pst pst; 
   Buffer *mBuf;
   fapi_slot_ind_t *slotIndMsg;

   FAPI_CL_ALLOC_SHRABL_BUF(slotIndMsg, sizeof(fapi_slot_ind_t));
   if(!slotIndMsg)
   {
      DU_LOG("\nERROR  -->  FAPI_CL: Memory allocation failed for slot Indication Message");
      return RFAILED;
   }
   else 
   {
      memset(slotIndMsg, 0, sizeof(fapi_slot_ind_t));
      slotIndMsg->sfn = clCb.slotIndInfo.currSfn;
      slotIndMsg->slot = clCb.slotIndInfo.currSlot;
      slotIndMsg->header.msg_id = FAPI_SLOT_INDICATION;
      slotIndMsg->header.length = sizeof(fapi_slot_ind_t) - sizeof(fapi_msg_t);

#ifdef ODU_SLOT_IND_DEBUG_LOG
      DU_LOG("\n\nDEBUG  -->  FAPI_CL: Sending Slot Indication [%d : %d] to MAC", slotIndMsg->sfn, slotIndMsg->slot);
#endif

      /* increment for the next TTI */
      clCb.slotIndInfo.currSlot++;
      if(clCb.slotIndInfo.currSlot >= MAX_SLOTS)
      {
         clCb.slotIndInfo.currSlot = 0;
         clCb.slotIndInfo.currSfn++;
         if(clCb.slotIndInfo.currSfn >= MAX_SFN)
            clCb.slotIndInfo.currSfn = 0;
      }

      memset(&pst, 0, sizeof(Pst));
      FILL_PST_FAPI_CL_TO_LWR_MAC(pst, FAPI_CL_INST_1, 0, EVT_FAPI_CL_SLOT_IND);
      ODU_GET_MSG_BUF(pst.region, pst.pool, &mBuf);
      if(!mBuf)
      {
         DU_LOG("\nERROR  --> FAPI_CL: Failed to allocate memory for slot indication buffer");
         FAPI_CL_FREE_SHRABL_BUF(pst.region, pst.pool, slotIndMsg, sizeof(fapi_slot_ind_t));
         return RFAILED;
      }
      CMCHKPK(oduPackPointer, (PTR)slotIndMsg, mBuf);
      ODU_POST_TASK(&pst, mBuf);
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Converts STOP.indication
 *
 * @details
 *
 *    Function :  convertStopIndication
 *
 *    Functionality:
 *       - Converts STOP.indication from UE sim's FAPI version to
 *         Mac's FAPI version.
 *
 * @params[in] Message pointer
 *
 * @return void
 *
 * ****************************************************************/
void buildAndSendStopIndication()
{
   Pst pst; 
   Buffer *mBuf;
   fapi_stop_ind_t *macStopInd = NULLP;

   DU_LOG("\nINFO  -->  FAPI_CL: Sending STOP.indication to MAC");

   FAPI_CL_ALLOC_SHRABL_BUF(macStopInd, sizeof(fapi_stop_ind_t));
   if(!macStopInd)
   {
      DU_LOG("\nERROR  -->  FAPI_CL: Memory allocation failed for stop Indication Message");
      return;
   }
   else 
   {
      memset(macStopInd, 0, sizeof(fapi_stop_ind_t));
      macStopInd->header.msg_id = FAPI_STOP_INDICATION;
      macStopInd->header.length = sizeof(fapi_stop_ind_t) - sizeof(fapi_msg_t);

      memset(&pst, 0, sizeof(Pst));
      FILL_PST_FAPI_CL_TO_LWR_MAC(pst, FAPI_CL_INST_1, 0, EVT_FAPI_CL_STOP_IND);
      ODU_GET_MSG_BUF(pst.region, pst.pool, &mBuf);
      if(!mBuf)
      {
         DU_LOG("\nERROR  --> FAPI_CL: Failed to allocate memory for stop indication buffer");
         FAPI_CL_FREE_SHRABL_BUF(pst.region, pst.pool, macStopInd, sizeof(fapi_stop_ind_t));
         return;
      }
      CMCHKPK(oduPackPointer, (PTR)macStopInd, mBuf);
      ODU_POST_TASK(&pst, mBuf);
   }
}

/*******************************************************************
*
* @brief Generates ticks periodically
*
* @details
*
*    Function : generateTicks
*
*    Functionality:
*       - Generates ticks periodically
*
* @params[in] 
* @return NULL
*
* ****************************************************************/
void* generateTicks()
{
   float   milisec = 1;          /* 1ms */
   struct  timespec req = {0};
   uint8_t ratio = 2;
   uint16_t counter = 0;

   clCb.slotIndInfo.currSfn = 0;
   clCb.slotIndInfo.currSlot = 0;
   clCb.slotIndInfo.slotIndicationStarted = true;
   req.tv_sec = 0;

   /* Currently the code takes longer that one slot indication to execute.
    * Hence, multiplying slot time interval by 2 in order to give enough time 
    * for L2 to complete one slot processing.
    * The ratio must be removed once code optimization is complete */
   req.tv_nsec = milisec * 1000000L * ratio;

   DU_LOG("\nINFO  -->  FAPI_CL: Starting to generate slot indications");

   while(clCb.slotIndInfo.slotIndicationStarted)
   //while(counter <=100)
   {   
      clock_nanosleep(CLOCK_REALTIME, 0, &req, NULL); 
      /* Send Slot indication indication to lower mac */
      if(buildAndSendSlotIndication() != ROK)
      {   
         DU_LOG("\nERROR  --> FAPI_CL: generateTicks(): Failed to build and send Slot Indication");
         break;
      }  
      counter++;
   }   

   DU_LOG("\nINFO  --> FAPI_CL: Slot indication stopped");
   buildAndSendStopIndication();
   return 0;
}

/*******************************************************************
*
* @brief Sends Start_slot_ind request to self
*
* @details
*
*    Function : sendStartSlotIndReq
*
*    Functionality:
*       - Sends Start_slot_ind request to self
*
* @params[in] 
* @return
*
* ****************************************************************/
void sendStartSlotIndReq()
{
   Pst pst;
   Buffer *mBuf = NULLP;

   DU_LOG("\nINFO  --> FAPI_CL: Sending start slot indication event to self");
   memset(&pst, 0, sizeof(Pst));
   FILL_PST_CL_TO_CL(pst, FAPI_CL_INST_0, FAPI_CL_INST_1, EVT_FAPI_CL_START_SLOT_IND);
   ODU_GET_MSG_BUF(pst.region, pst.pool, &mBuf);
   ODU_POST_TASK(&pst, mBuf);
}

/*******************************************************************
 *
 * @brief Process START.Response
 *
 * @details
 *
 *    Function :  processStartResponse
 *
 *    Functionality:
 *       - Calls the handler to start sending slot indication to MAC
 *
 * @params[in] Message pointer
 *
 * @return void
 *
 * ****************************************************************/
void convertStartResponse(void *msg)
{  
   DU_LOG("\nINFO  -->  FAPI_CL: Received START.response from UE simulator");
   sendStartSlotIndReq();
}

/*******************************************************************
 *
 * @brief Process RACH.Indication
 *
 * @details
 *
 *    Function :  convertRachIndication
 *
 *    Functionality:
 *       - Converts STOP.indication from UE sim's FAPI version to
 *    Mac's FAPI version
 *
 * @params[in] Message pointer
 *
 * @return void
 *
 * ****************************************************************/
void convertRachIndication(void *msg)
{
   uint8_t pduIdx = 0, preambleIdx = 0;
   fapi_rach_indication_t *macRachInd = NULLP;
   fapi_rach_indication_t_ue_sim *rachInd = (fapi_rach_indication_t_ue_sim *)msg;

   DU_LOG("\nINFO  -->  FAPI_CL: Received RACH.indication from UE simulator");

   FAPI_CL_ALLOC(macRachInd, sizeof(fapi_rach_indication_t));
   if(!macRachInd)
   {   
      DU_LOG("\nERROR  -->  FAPI_CL: Failed to allocate memory for RACH.indication");
      return;
   }   

   macRachInd->header.msg_id = FAPI_RACH_INDICATION;
   macRachInd->header.length = sizeof(fapi_rach_indication_t) - sizeof(fapi_msg_t);
   macRachInd->sfn =  rachInd->sfn;
   macRachInd->slot = rachInd->slot;
   macRachInd->numPdus = rachInd->numPdus;

   for(pduIdx = 0; pduIdx < rachInd->numPdus; pduIdx++)
   {
      macRachInd->rachPdu[pduIdx].phyCellId = rachInd->rachPdu[pduIdx].handle;
      macRachInd->rachPdu[pduIdx].symbolIndex = rachInd->rachPdu[pduIdx].symbolIndex;
      macRachInd->rachPdu[pduIdx].slotIndex = rachInd->rachPdu[pduIdx].slotIndex;
      macRachInd->rachPdu[pduIdx].freqIndex = rachInd->rachPdu[pduIdx].raIndex;
      macRachInd->rachPdu[pduIdx].avgRssi = rachInd->rachPdu[pduIdx].avgRssi;
      macRachInd->rachPdu[pduIdx].avgSnr = rachInd->rachPdu[pduIdx].avgSnr;
      macRachInd->rachPdu[pduIdx].numPreamble = rachInd->rachPdu[pduIdx].numPreamble;
      for(preambleIdx = 0; preambleIdx<macRachInd->rachPdu[pduIdx].numPreamble; preambleIdx++)
      {
         macRachInd->rachPdu[pduIdx].preambleInfo[preambleIdx].preambleIndex = rachInd->rachPdu[pduIdx].preambleInfo[preambleIdx].preambleIndex;
         macRachInd->rachPdu[pduIdx].preambleInfo[preambleIdx].timingAdvance = rachInd->rachPdu[pduIdx].preambleInfo[preambleIdx].timingAdvance;
         macRachInd->rachPdu[pduIdx].preambleInfo[preambleIdx].preamblePwr = rachInd->rachPdu[pduIdx].preambleInfo[preambleIdx].premblePwr;
      }
   }

   procPhyMessages(macRachInd->header.msg_id, sizeof(fapi_rach_indication_t), (void *)macRachInd);
   FAPI_CL_FREE(macRachInd, sizeof(fapi_rach_indication_t));
}

/*******************************************************************
 *
 * @brief Process CRC.Indication
 *
 * @details
 *
 *    Function :  convertCrcIndication
 *
 *    Functionality:
 *       - Converts CRC.indication from UE sim's FAPI version to
 *    Mac's FAPI version
 *
 * @params[in] Message pointer
 *
 * @return void
 *
 * ****************************************************************/
void convertCrcIndication(void *msg)
{
   uint8_t crcIdx = 0, cbIdx = 0;
   fapi_crc_ind_t *macCrcInd = NULLP;
   fapi_crc_ind_t_ue_sim *crcInd = (fapi_crc_ind_t_ue_sim *)msg;

   DU_LOG("\nINFO  -->  FAPI_CL: Received CRC.indication from UE simulator");

   FAPI_CL_ALLOC(macCrcInd, sizeof(fapi_crc_ind_t));
   if(!macCrcInd)
   {   
      DU_LOG("\nERROR  -->  FAPI_CL: Failed to allocate memory for CRC.indication");
      return;
   }   

   macCrcInd->header.msg_id = FAPI_CRC_INDICATION;
   macCrcInd->header.length = sizeof(fapi_crc_ind_t) - sizeof(fapi_msg_t);
   macCrcInd->sfn =  crcInd->sfn;
   macCrcInd->slot = crcInd->slot;
   macCrcInd->numCrcs = crcInd->num_crcs;
   
   for(crcIdx = 0; crcIdx < macCrcInd->numCrcs; crcIdx++)
   {
      macCrcInd->crc[crcIdx].handle = crcInd->crc[crcIdx].handle;
      macCrcInd->crc[crcIdx].rnti = crcInd->crc[crcIdx].rnti;
      macCrcInd->crc[crcIdx].harqId = crcInd->crc[crcIdx].harq_id;
      macCrcInd->crc[crcIdx].tbCrcStatus = crcInd->crc[crcIdx].tb_crc_status;
      macCrcInd->crc[crcIdx].ul_cqi = crcInd->crc[crcIdx].ul_cqi;
      macCrcInd->crc[crcIdx].numCb = crcInd->crc[crcIdx].num_cb;
      macCrcInd->crc[crcIdx].timingAdvance = crcInd->crc[crcIdx].timing_advance;
      macCrcInd->crc[crcIdx].rssi = crcInd->crc[crcIdx].rssi;
      for(cbIdx = 0; cbIdx < macCrcInd->crc[crcIdx].numCb; cbIdx++)
         macCrcInd->crc[crcIdx].cbCrcStatus[cbIdx] = crcInd->crc[crcIdx].cb_crc_status[cbIdx];
   }

   procPhyMessages(macCrcInd->header.msg_id, sizeof(fapi_crc_ind_t), (void *)macCrcInd);
   FAPI_CL_FREE(macCrcInd, sizeof(fapi_crc_ind_t));
}

/*******************************************************************
 *
 * @brief Process UCI PUSCH
 *
 * @details
 *
 *    Function :  convertUciPusch
 *
 *    Functionality:
 *       - Converts UCI PUSCH from UE sim's FAPI version to
 *    Mac's FAPI version
 *
 * @params[in] MAC's UCI PUSCH structure
 *             UE Sim's UCI PUSCH structure
 *             Offset of UCI PUSCH structure in its parent struct
 *
 * @return void
 *
 * ****************************************************************/
void convertUciPusch(fapi_uci_o_pusch_t *macUciPusch, fapi_uci_o_pusch_t_ue_sim uciPusch, uint16_t *pduSize)
{
   uint8_t byteLen = 0;
   uint16_t uciPuschIdx = 0;

   FILL_PARAM(uciPusch.handle, macUciPusch->handle, uciPuschIdx);
   FILL_PARAM(uciPusch.pdu_bitmap, macUciPusch->pduBitmap, uciPuschIdx);
   FILL_PARAM(uciPusch.ul_cqi, macUciPusch->ul_cqi, uciPuschIdx);
   FILL_PARAM(uciPusch.rnti, macUciPusch->rnti, uciPuschIdx);
   FILL_PARAM(uciPusch.timing_advance, macUciPusch->timingAdvance, uciPuschIdx);
   FILL_PARAM(uciPusch.rssi, macUciPusch->rssi, uciPuschIdx);

   FILL_PARAM(uciPusch.harqInfo.harq_crc, macUciPusch->harqInfo.harqCrc, uciPuschIdx);
   FILL_PARAM(uciPusch.harqInfo.harq_bit_len, macUciPusch->harqInfo.harqBitLen, uciPuschIdx);
   byteLen = macUciPusch->harqInfo.harqBitLen/8;
   if((macUciPusch->harqInfo.harqBitLen % 8) != 0)
      byteLen += 1;
   memcpy(macUciPusch->harqInfo.harqPayload, uciPusch.harqInfo.harq_payload, byteLen);
   uciPuschIdx += byteLen;

   FILL_PARAM(uciPusch.csiPart1info.csi_part1_crc, macUciPusch->csiPart1info.csiPart1Crc, uciPuschIdx);
   FILL_PARAM(uciPusch.csiPart1info.csi_part1_bit_len, macUciPusch->csiPart1info.csiPart1BitLen, uciPuschIdx);
   byteLen = macUciPusch->csiPart1info.csiPart1BitLen/8;
   if((macUciPusch->csiPart1info.csiPart1BitLen % 8) != 0)
      byteLen += 1;
   memcpy(macUciPusch->csiPart1info.csiPart1Payload, uciPusch.csiPart1info.csi_part1_payload, byteLen);
   uciPuschIdx += byteLen;

   FILL_PARAM(uciPusch.csiPart2info.csi_part2_crc, macUciPusch->csiPart2info.csiPart2Crc, uciPuschIdx);
   FILL_PARAM(uciPusch.csiPart2info.csi_part2_bit_len, macUciPusch->csiPart2info.csiPart2BitLen, uciPuschIdx);
   byteLen = macUciPusch->csiPart2info.csiPart2BitLen/8;
   if((macUciPusch->csiPart2info.csiPart2BitLen % 8) != 0)
      byteLen += 1;
   memcpy(macUciPusch->csiPart2info.csiPart2Payload, uciPusch.csiPart2info.csi_part2_payload, byteLen);
   uciPuschIdx += byteLen;

   *pduSize += uciPuschIdx;
}

/*******************************************************************
 *
 * @brief Process UCI PUCCH F0 F1
 *
 * @details
 *
 *    Function :  convertUciPucchF0F1
 *
 *    Functionality:
 *       - Converts UCI PUCCH F0 F1 from UE sim's FAPI version to
 *    Mac's FAPI version
 *
 * @params[in] MAC's UCI PUCCH F0 F1 structure
 *             UE Sim's UCI PUCCH F0 F1 structure
 *             Offset of UCI PUCCH F0 F1 in its parent struct
 *
 * @return void
 *
 * ****************************************************************/
void convertUciPucchF0F1(fapi_uci_o_pucch_f0f1_t *macUciPucchF0F1, fapi_uci_o_pucch_f0f1_t_ue_sim uciPucchF0F1, uint16_t *pduSize)
{
   uint16_t uciPucchIdx = 0;
 
   FILL_PARAM(uciPucchF0F1.handle, macUciPucchF0F1->handle, uciPucchIdx);
   FILL_PARAM(uciPucchF0F1.pdu_bitmap, macUciPucchF0F1->pduBitmap, uciPucchIdx);
   FILL_PARAM(uciPucchF0F1.pucch_format, macUciPucchF0F1->pucchFormat, uciPucchIdx);
   FILL_PARAM(uciPucchF0F1.ul_cqi, macUciPucchF0F1->ul_cqi, uciPucchIdx);
   FILL_PARAM(uciPucchF0F1.rnti, macUciPucchF0F1->rnti, uciPucchIdx);
   FILL_PARAM(uciPucchF0F1.timing_advance, macUciPucchF0F1->timingAdvance, uciPucchIdx);
   FILL_PARAM(uciPucchF0F1.rssi, macUciPucchF0F1->rssi, uciPucchIdx);

   FILL_PARAM(uciPucchF0F1.srInfo.sr_indication, macUciPucchF0F1->srInfo.srIndication, uciPucchIdx);
   FILL_PARAM(uciPucchF0F1.srInfo.sr_confidence_level, macUciPucchF0F1->srInfo.srConfidenceLevel, uciPucchIdx);

   FILL_PARAM(uciPucchF0F1.harqInfo.num_harq, macUciPucchF0F1->harqInfo.numHarq, uciPucchIdx);
   FILL_PARAM(uciPucchF0F1.harqInfo.harq_confidence_level, macUciPucchF0F1->harqInfo.harqConfidenceLevel, uciPucchIdx);
   memcpy(macUciPucchF0F1->harqInfo.harqValue, uciPucchF0F1.harqInfo.harq_value, sizeof(uint8_t) * macUciPucchF0F1->harqInfo.numHarq);
   uciPucchIdx += sizeof(uint8_t) * macUciPucchF0F1->harqInfo.numHarq;

   *pduSize += uciPucchIdx;
}

/*******************************************************************
 *
 * @brief Process UCI PUCCH F2 F3 F4
 *
 * @details
 *
 *    Function :  convertUciPucchF2F3F4
 *
 *    Functionality:
 *       - Converts UCI PUCCH F2F3F4 from UE sim's FAPI version to
 *    Mac's FAPI version
 *
 * @params[in] MAC's UCI PUCCH F2F3F4 structure
 *             UE Sim's UCI PUCCH F2F3F4 structure
 *             Offset of UCI PUCCH F2F3F4 structure in its parent struct
 *
 * @return void
 *
 * ****************************************************************/
void convertUciPucchF2F3F4(fapi_uci_o_pucch_f2f3f4_t *macUciPucchF2F3F4, fapi_uci_o_pucch_f2f3f4_t_ue_sim uciPucchF2F3F4, uint16_t *pduSize)
{
   uint16_t uciPucchIdx = 0;

   FILL_PARAM(uciPucchF2F3F4.handle, macUciPucchF2F3F4->handle, uciPucchIdx);
   FILL_PARAM(uciPucchF2F3F4.pdu_bitmap, macUciPucchF2F3F4->pduBitmap, uciPucchIdx);
   FILL_PARAM(uciPucchF2F3F4.pucch_format, macUciPucchF2F3F4->pucchFormat, uciPucchIdx);
   FILL_PARAM(uciPucchF2F3F4.ul_cqi, macUciPucchF2F3F4->ul_cqi, uciPucchIdx);
   FILL_PARAM(uciPucchF2F3F4.rnti, macUciPucchF2F3F4->rnti, uciPucchIdx);
   FILL_PARAM(uciPucchF2F3F4.timing_advance, macUciPucchF2F3F4->timingAdvance, uciPucchIdx);
   FILL_PARAM(uciPucchF2F3F4.rssi, macUciPucchF2F3F4->rssi, uciPucchIdx);

   // TODO : Fill UCI Bits 

   *pduSize += uciPucchIdx;
}

/*******************************************************************
 *
 * @brief Process UCI.Indication
 *
 * @details
 *
 *    Function :  convertUciIndication
 *
 *    Functionality:
 *       - Converts UCI.indication from UE sim's FAPI version to
 *    Mac's FAPI version
 *
 * @params[in] Message pointer
 *
 * @return void
 *
 * ****************************************************************/
void convertUciIndication(void *msg)
{
   uint8_t uciIdx = 0;
   uint16_t pduSize = 0;
   uint32_t offset = 0;
   fapi_uci_indication_t *macUciInd = NULLP;
   fapi_uci_indication_t_ue_sim *uciInd = (fapi_uci_indication_t_ue_sim *)msg;
   fapi_uci_pdu_info_t_ue_sim *pdu = NULLP;

   DU_LOG("\nINFO  -->  FAPI_CL: Received UCI.indication from UE simulator");

   FAPI_CL_ALLOC(macUciInd, sizeof(fapi_uci_indication_t));
   if(!macUciInd)
   {   
      DU_LOG("\nERROR  -->  FAPI_CL: Failed to allocate memory for UCI.indication");
      return;
   }   

   macUciInd->header.msg_id = FAPI_UCI_INDICATION;
   macUciInd->header.length = sizeof(fapi_uci_indication_t) - sizeof(fapi_msg_t);
   macUciInd->sfn =  uciInd->sfn;
   macUciInd->slot = uciInd->slot;
   macUciInd->numUcis = uciInd->num_ucis;
   offset = sizeof(fapi_uci_indication_t); 

   for(uciIdx = 0; uciIdx < macUciInd->numUcis; uciIdx++)
   {
      pdu = (void *)uciInd + offset;
      FILL_PARAM(pdu->pdu_type, macUciInd->uciPdu[uciIdx].pduType, pduSize);
      pduSize += sizeof(macUciInd->uciPdu[uciIdx].pduSize);
      switch (macUciInd->uciPdu[uciIdx].pduType)
      {
         case UCI_PUSCH:
            convertUciPusch(&macUciInd->uciPdu[uciIdx].uci.uciPusch, pdu->u.uci_pusch, &pduSize);
            break;
         case UCI_PUCCH_F0_F1:
            convertUciPucchF0F1(&macUciInd->uciPdu[uciIdx].uci.uciPucchF0F1, pdu->u.uci_pucch_f0f1, &pduSize);
            break;
         case UCI_PUCCH_F2_F3_F4:
            convertUciPucchF2F3F4(&macUciInd->uciPdu[uciIdx].uci.uciPucchF2F3F4, pdu->u.uci_pucch_f2f3f4, &pduSize);
            break;
      }
      macUciInd->uciPdu[uciIdx].pduSize = pduSize;
      offset += pdu->pdu_size;
   }

   procPhyMessages(macUciInd->header.msg_id, sizeof(fapi_uci_indication_t), (void *)macUciInd);
   FAPI_CL_FREE(macUciInd, sizeof(fapi_uci_indication_t));
}

/*******************************************************************
 *
 * @brief Process SRS.Indication
 *
 * @details
 *
 *    Function :  convertSrsIndication
 *
 *    Functionality:
 *       - Converts SRS.indication from UE sim's FAPI version to
 *    Mac's FAPI version
 *
 * @params[in] Message pointer
 *
 * @return void
 *
 * ****************************************************************/
void convertSrsIndication(void *msg)
{
   uint8_t srsIdx = 0, symbIdx = 0, rbIdx = 0;
   fapi_srs_indication_t *macSrsInd = NULLP;
   fapi_srs_indication_t_ue_sim *srsInd = (fapi_srs_indication_t_ue_sim *)msg;

   DU_LOG("\nINFO  -->  FAPI_CL: Received SRS.indication from UE simulator");

   FAPI_CL_ALLOC(macSrsInd, sizeof(fapi_srs_indication_t));
   if(!macSrsInd)
   {   
      DU_LOG("\nERROR  -->  FAPI_CL: Failed to allocate memory for SRS.indication");
      return;
   }   

   macSrsInd->header.msg_id = FAPI_SRS_INDICATION;
   macSrsInd->header.length = sizeof(fapi_srs_indication_t) - sizeof(fapi_msg_t);
   macSrsInd->sfn =  srsInd->sfn;
   macSrsInd->slot = srsInd->slot;
   macSrsInd->numPdus = srsInd->numPdus;
   
   for(srsIdx = 0; srsIdx < macSrsInd->numPdus; srsIdx++)
   {
      macSrsInd->srsPdus[srsIdx].handle = srsInd->srsPdus[srsIdx].handle;
      macSrsInd->srsPdus[srsIdx].rnti = srsInd->srsPdus[srsIdx].rnti;
      macSrsInd->srsPdus[srsIdx].timingAdvance = srsInd->srsPdus[srsIdx].timingAdvance;
      macSrsInd->srsPdus[srsIdx].numSymbols = srsInd->srsPdus[srsIdx].numSymbols;
      macSrsInd->srsPdus[srsIdx].wideBandSnr = srsInd->srsPdus[srsIdx].wideBandSnr;
      macSrsInd->srsPdus[srsIdx].numReportedSymbols = srsInd->srsPdus[srsIdx].numReportedSymbols;
      for(symbIdx = 0; symbIdx < macSrsInd->srsPdus[srsIdx].numReportedSymbols; symbIdx++)
      {
         macSrsInd->srsPdus[srsIdx].symbSnr[symbIdx].numRbs = srsInd->srsPdus[srsIdx].symbSnr[symbIdx].numRbs;
         for(rbIdx = 0; rbIdx < macSrsInd->srsPdus[srsIdx].symbSnr[symbIdx].numRbs; rbIdx++)
            macSrsInd->srsPdus[srsIdx].symbSnr[symbIdx].rbSNR[rbIdx] = srsInd->srsPdus[srsIdx].symbSnr[symbIdx].rbSNR[rbIdx];
      }
   }

   procPhyMessages(macSrsInd->header.msg_id, sizeof(fapi_srs_indication_t), (void *)macSrsInd);
   FAPI_CL_FREE(macSrsInd, sizeof(fapi_srs_indication_t));
}


/*******************************************************************
 *
 * @brief Process RX_DATA.Indication
 *
 * @details
 *
 *    Function :  convertRxDataIndication
 *
 *    Functionality:
 *       - Converts RX_DATA.indication from UE sim's FAPI version to
 *    Mac's FAPI version
 *
 * @params[in] Message pointer
 *
 * @return void
 *
 * ****************************************************************/
void convertRxDataIndication(void *msg)
{
   uint8_t pduIdx = 0;
   fapi_rx_data_indication_t *macRxDataInd = NULLP;
   fapi_rx_data_indication_t_ue_sim *rxDataInd = (fapi_rx_data_indication_t_ue_sim *)msg;

   DU_LOG("\nINFO  -->  FAPI_CL: Received RX_DATA.indication from UE simulator");

   FAPI_CL_ALLOC(macRxDataInd, sizeof(fapi_rx_data_indication_t));
   if(!macRxDataInd)
   {   
      DU_LOG("\nERROR  -->  FAPI_CL: Failed to allocate memory for RX_DATA.indication");
      return;
   }   

   macRxDataInd->header.msg_id = FAPI_RX_DATA_INDICATION;
   macRxDataInd->header.length = sizeof(fapi_rx_data_indication_t) - sizeof(fapi_msg_t);
   macRxDataInd->sfn =  rxDataInd->sfn;
   macRxDataInd->slot = rxDataInd->slot;
   macRxDataInd->numPdus = rxDataInd->num_pdus;
   
   for(pduIdx = 0; pduIdx < macRxDataInd->numPdus; pduIdx++)
   {
      macRxDataInd->pdus[pduIdx].handle = rxDataInd->pdus[pduIdx].handle;
      macRxDataInd->pdus[pduIdx].rnti = rxDataInd->pdus[pduIdx].rnti;
      macRxDataInd->pdus[pduIdx].harqId = rxDataInd->pdus[pduIdx].harq_id;
      macRxDataInd->pdus[pduIdx].ul_cqi = rxDataInd->pdus[pduIdx].ul_cqi;
      macRxDataInd->pdus[pduIdx].timingAdvance = rxDataInd->pdus[pduIdx].timing_advance;
      macRxDataInd->pdus[pduIdx].rssi = rxDataInd->pdus[pduIdx].rssi;
      macRxDataInd->pdus[pduIdx].pdu_length = rxDataInd->pdus[pduIdx].pdu_length;
      FAPI_CL_ALLOC(macRxDataInd->pdus[pduIdx].pduData, macRxDataInd->pdus[pduIdx].pdu_length);
      memcpy(macRxDataInd->pdus[pduIdx].pduData, rxDataInd->pdus[pduIdx].pdu_data, macRxDataInd->pdus[pduIdx].pdu_length);
   }

   procPhyMessages(macRxDataInd->header.msg_id, sizeof(fapi_rx_data_indication_t), (void *)macRxDataInd);
   FAPI_CL_FREE(macRxDataInd, sizeof(fapi_rx_data_indication_t));
}

/*******************************************************************
 *
 * @brief Call appropriate handler for FAPI message from UE sim
 *
 * @details
 *
 *    Function :  fapiClProcessUeSim2MacFapiMsg
 *
 *    Functionality:
 *       - Calls appropriate handler based on message type for 
 *         message sent from UE simulator to MAC.
 *
 * @params[in] Message pointer
 *
 * @return void
 *
 * ****************************************************************/
void fapiClProcessUeSim2MacFapiMsg(Data *msg)
{
   uint8_t numMsg = 0;
   fapi_msg_header_t_ue_sim *msgHdr = (fapi_msg_header_t_ue_sim *)msg;

   while(numMsg < msgHdr->numberOfMessagesIncluded)
   {
      switch(msgHdr->msg_details.message_type_id)
      {
         case FAPI_CONFIG_RESPONSE:
            convertConfigResponse(msg);      
            break;
         case FAPI_START_RESPONSE:
            convertStartResponse(msg);
            break;
         case FAPI_RACH_INDICATION:
            convertRachIndication(msg);
            break;
         case FAPI_CRC_INDICATION:
            convertCrcIndication(msg);
            break;
         case FAPI_UCI_INDICATION:
            convertUciIndication(msg);
            break;
         case FAPI_SRS_INDICATION:
            convertSrsIndication(msg);
            break;
         case FAPI_STOP_INDICATION:
            clCb.slotIndInfo.slotIndicationStarted = false;
            break;
         default:
            DU_LOG("\nERROR  -->  FAPI_CL: Invalid message type id [%d] in header", msgHdr->msg_details.message_type_id);
      }
      numMsg++;
   }
}

/**********************************************************************
  End of file
 **********************************************************************/
