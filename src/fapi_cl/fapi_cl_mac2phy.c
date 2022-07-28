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

/* This file converts FAPI messages sent from MAC to UE simulator/PHY */

#include "common_def.h"
#include "fapi_cl.h"
#include "fapi_cl_utils.h"
#include "fapi_interface_ue_sim.h"
#include "fapi_interface.h"
#include "fapi_vendor_extension.h"

/*******************************************************************
 *
 * @brief Converts START.Request
 *
 * @details
 *
 *    Function :  convertStartRequest
 *
 *    Functionality:
 *       - Converts START.request from MAC's FAPI version to
 *         UE Simulator's FAPI version.
 *
 * @params[in] Message pointer
 *
 * @return void
 *
 * ****************************************************************/
void convertStartRequest(uint8_t handle, void *msg, uint16_t msgLen)
{
   p_fapi_api_queue_elem_t startReqElem = (p_fapi_api_queue_elem_t)msg;
   fapi_start_req_t *macStartReq = (fapi_start_req_t *)(startReqElem +1);
   fapi_start_req_t_ue_sim ueSimStartReq;

   DU_LOG("\nDEBUG  -->  FAPI_CL: Received START.request from MAC");

   memset(&ueSimStartReq, 0, sizeof(fapi_start_req_t_ue_sim));
   ueSimStartReq.header.numberOfMessagesIncluded = 1;
   ueSimStartReq.header.handle = handle;
   ueSimStartReq.header.msg_details.message_type_id = macStartReq->header.msg_id;
   ueSimStartReq.header.msg_details.length = sizeof(fapi_start_req_t_ue_sim) - sizeof(fapi_msg_header_t_ue_sim);

   FAPI_CL_FREE(msg, msgLen);

   sendMsgToUeSim(EVT_CNTL_REQUEST, (Data *)&ueSimStartReq, sizeof(fapi_start_req_t_ue_sim));
}

/*******************************************************************
 *
 * @brief Converts CONFIG.Request
 *
 * @details
 *
 *    Function :  convertConfigRequest
 *
 *    Functionality:
 *       - Converts CONFIG.request from MAC's FAPI version to
 *         UE Simulator's FAPI version.
 *
 * @params[in] Message pointer
 *
 * @return void
 *
 * ****************************************************************/
void convertConfigRequest(uint8_t handle, void *msg, uint16_t msgLen)
{
   uint8_t idx = 0;
   p_fapi_api_queue_elem_t configReqElem = (p_fapi_api_queue_elem_t)msg;
   fapi_config_req_t *macConfigReq = (fapi_config_req_t *)(configReqElem +1);
   fapi_config_req_t_ue_sim ueSimConfigReq;

   DU_LOG("\nDEBUG  -->  FAPI_CL: Received CONFIG.request from MAC");

   /* Before processing CONFIG.request msg, configure FAPI CL to communicate
    * with UE simulator */
   configureFapiCl();

   memset(&ueSimConfigReq, 0, sizeof(fapi_config_req_t_ue_sim));
   ueSimConfigReq.header.numberOfMessagesIncluded = 1;
   ueSimConfigReq.header.handle = handle;
   ueSimConfigReq.header.msg_details.message_type_id = macConfigReq->header.msg_id;
   ueSimConfigReq.header.msg_details.length = sizeof(fapi_config_req_t_ue_sim) - sizeof(fapi_msg_header_t_ue_sim);
   ueSimConfigReq.number_of_tlvs = macConfigReq->number_of_tlvs;

   /* Copying all TLVs
    * Issues:
    * a. DL Frequency value gets truncated when copied to a uint16_t TLV
    * b. UL Frequency value gets truncated when copied to a uint16_t TLV
    * c. UE simulator FAPI interface file does not support DMRS_Type_A_Pos tag
    */
   for(idx = 0; idx < ueSimConfigReq.number_of_tlvs; idx++)
   {
      ueSimConfigReq.tlvs[idx].tl.tag = macConfigReq->tlvs[idx].tl.tag;
      ueSimConfigReq.tlvs[idx].tl.length = macConfigReq->tlvs[idx].tl.length;
      ueSimConfigReq.tlvs[idx].value = macConfigReq->tlvs[idx].value;
   }

   FAPI_CL_FREE(msg, msgLen);

   sendMsgToUeSim(EVT_CNTL_REQUEST, (Data *)&ueSimConfigReq, sizeof(fapi_config_req_t_ue_sim));
}

/*******************************************************************
 *
 * @brief Converts codeword Info structure
 *
 * @details
 *
 *    Function :  convertCodewordInfo
 *
 *    Functionality:
 *       - Converts codeword info from MAC's FAPI version to
 *         UE Simulator's FAPI version.
 *
 * @params[in] MAC's Codeword info
 *             UE sim's codeword info
 *             Offset of codeword info struct in its parent struct
 *
 * @return void
 *
 * ****************************************************************/
void convertCodewordInfo(fapi_codeword_pdu_t macCwInfo, fapi_codeword_pdu_t_ue_sim *cwInfo, uint16_t *pduIdx)
{
   uint16_t cwIdx = 0;

   FILL_PARAM(macCwInfo.targetCodeRate, cwInfo->target_code_rate, cwIdx)
   FILL_PARAM(macCwInfo.qamModOrder, cwInfo->qam_mod_order, cwIdx)
   FILL_PARAM(macCwInfo.mcsIndex, cwInfo->mcs_index, cwIdx)
   FILL_PARAM(macCwInfo.mcsTable, cwInfo->mcs_table, cwIdx)
   FILL_PARAM(macCwInfo.rvIndex, cwInfo->rv_index, cwIdx)
   FILL_PARAM(macCwInfo.tbSize, cwInfo->tb_size, cwIdx)

   *pduIdx += cwIdx;
}

/*******************************************************************
 *
 * @brief Converts Precoding and Beamforming struct
 *
 * @details
 *
 *    Function :  convertPreCodingAndBeamforming
 *
 *    Functionality:
 *       - Converts Precondig and beamforming struct from MAC's FAPI 
 *         version to UE Simulator's FAPI version.
 *
 * @params[in] MAC's Precoding and beamforming struct
 *             UE sim's Precoding and beamforming struct
 *             Offset of precoding and beamforming struct in its parent struct
 *
 * @return void
 *
 * ****************************************************************/
void convertPreCodingAndBeamforming(fapi_precoding_bmform_t macPcAndBf, fapi_precoding_bmform_t_ue_sim *pcAndBf, uint16_t *pduIdx)
{
   uint16_t pcBfIdx = 0, itr = 0;
   fapi_bmi_t_ue_sim *bmi = NULLP;

   FILL_PARAM(macPcAndBf.numPrgs, pcAndBf->num_prgs, pcBfIdx);
   FILL_PARAM(macPcAndBf.prgSize, pcAndBf->prg_size, pcBfIdx);
   FILL_PARAM(macPcAndBf.digBfInterfaces, pcAndBf->dig_bf_interfaces, pcBfIdx);
   FILL_PARAM(macPcAndBf.pmi_bfi[0].pmIdx, pcAndBf->pmi_bfi[0].pm_idx, pcBfIdx);

   for(itr = 0; itr < macPcAndBf.digBfInterfaces; itr++)
   {
      bmi = (void *)pcAndBf + pcBfIdx; 
      FILL_PARAM(macPcAndBf.pmi_bfi[0].beamIdx[itr].beamidx, bmi->beamidx, pcBfIdx);
   }
   *pduIdx += pcBfIdx;
}

/*******************************************************************
 *
 * @brief Converts DL DCI struct
 *
 * @details
 *
 *    Function :  convertDlDci
 *
 *    Functionality:
 *       - Converts DL DCI struct from MAC's FAPI 
 *         version to UE Simulator's FAPI version.
 *
 * @params[in] MAC's DL DCI struct
 *             UE sim's DCI struct
 *             Offset of DL DCI struct in its parent struct
 *
 * @return void
 *
 * ****************************************************************/
void convertDlDci(fapi_dl_dci_t macDlDci, fapi_dl_dci_t_ue_sim *dlDci, uint16_t *pduIdx)
{
   uint8_t itr = 0, dlDciPayloadSizeBytes = 0;
   uint8_t *dlDciPayloadByte = NULLP;
   uint16_t dlDciIdx = 0;

   FILL_PARAM(macDlDci.rnti, dlDci->rnti, dlDciIdx);
   FILL_PARAM(macDlDci.scramblingId, dlDci->nIdPdcchData, dlDciIdx);
   FILL_PARAM(macDlDci.scramblingRnti, dlDci->nRntiPdcchData, dlDciIdx);
   FILL_PARAM(macDlDci.cceIndex, dlDci->cce_index, dlDciIdx);
   FILL_PARAM(macDlDci.aggregationLevel, dlDci->aggregation_level, dlDciIdx);

   convertPreCodingAndBeamforming(macDlDci.pc_and_bform, &dlDci->pc_and_bform, &dlDciIdx);
   
   FILL_PARAM(macDlDci.beta_pdcch_1_0, dlDci->tx_power_info.beta_pdcch_1_0, dlDciIdx);
   FILL_PARAM(macDlDci.powerControlOffsetSS, dlDci->tx_power_info.powerControlOffsetSSProfileNR, dlDciIdx);
   
   FILL_PARAM(macDlDci.payloadSizeBits, dlDci->payload_size_bits, dlDciIdx);
   if(dlDci->payload_size_bits % 8 == 0)  
      dlDciPayloadSizeBytes = (dlDci->payload_size_bits/8);
   else
      dlDciPayloadSizeBytes = (dlDci->payload_size_bits/8)+1;

   for(itr = 0; itr < dlDciPayloadSizeBytes; itr++)
   {
      dlDciPayloadByte = (void *)dlDci + dlDciIdx;
      FILL_PARAM(macDlDci.payload[itr], *dlDciPayloadByte, dlDciIdx);
   }

   *pduIdx += dlDciIdx;
}

/*******************************************************************
 *
 * @brief Converts PDCCH PDU
 *
 * @details
 *
 *    Function :  convertDlPdcchPdu
 *
 *    Functionality:
 *       - Converts PDCCH PDU struct from MAC's FAPI 
 *         version to UE Simulator's FAPI version.
 *
 * @params[in] MAC's PDCCH PDU struct
 *             UE sim's PDCCH PDU struct
 *             Offset of PDCCH PDU struct in its parent struct
 *
 * @return void
 *
 * ****************************************************************/
void convertDlPdcchPdu(fapi_dl_pdcch_pdu_t macPdcchPdu, fapi_dl_pdcch_pdu_t_ue_sim *pdcchPdu, uint16_t *pduIndex)
{
   uint8_t itr = 0;
   uint16_t pdcchPduIdx = 0;
   fapi_dl_dci_t_ue_sim *dlDci = NULLP;

   FILL_PARAM(macPdcchPdu.bwpSize, pdcchPdu->bwp.bwp_size, pdcchPduIdx);
   FILL_PARAM(macPdcchPdu.bwpStart, pdcchPdu->bwp.bwp_start, pdcchPduIdx);
   FILL_PARAM(macPdcchPdu.subCarrierSpacing, pdcchPdu->bwp.sub_carrier_spacing, pdcchPduIdx);
   FILL_PARAM(macPdcchPdu.cyclicPrefix, pdcchPdu->bwp.cyclic_prefix, pdcchPduIdx);

   FILL_PARAM(macPdcchPdu.startSymbolIndex, pdcchPdu->coreset.start_symbol_index, pdcchPduIdx);
   FILL_PARAM(macPdcchPdu.durationSymbols, pdcchPdu->coreset.duration_symbols, pdcchPduIdx);
   for(itr = 0; itr < FAPI_MAX_FREQ_DOMAIN_RES; itr++)
      FILL_PARAM(macPdcchPdu.freqDomainResource[itr], pdcchPdu->coreset.freq_domain_resource[itr], pdcchPduIdx);
   FILL_PARAM(macPdcchPdu.cceRegMappingType, pdcchPdu->coreset.cce_reg_mapping_type, pdcchPduIdx);
   FILL_PARAM(macPdcchPdu.regBundleSize, pdcchPdu->coreset.reg_bundle_size, pdcchPduIdx);
   FILL_PARAM(macPdcchPdu.interleaverSize, pdcchPdu->coreset.interleaver_size, pdcchPduIdx);
   FILL_PARAM(macPdcchPdu.coreSetType, pdcchPdu->coreset.core_set_type, pdcchPduIdx);
   FILL_PARAM(macPdcchPdu.shiftIndex, pdcchPdu->coreset.shift_index, pdcchPduIdx);
   FILL_PARAM(macPdcchPdu.precoderGranularity, pdcchPdu->coreset.precoder_granularity, pdcchPduIdx);

   FILL_PARAM(macPdcchPdu.numDlDci, pdcchPdu->num_dl_dci, pdcchPduIdx);
   for(itr = 0; itr <macPdcchPdu.numDlDci; itr++)
   {
      dlDci = (void *)pdcchPdu + pdcchPduIdx;
      convertDlDci(macPdcchPdu.dlDci[itr], dlDci, &pdcchPduIdx);
   }

   //unused in UE sim : pdcchPdu->pdcchMntParms
   pdcchPduIdx += sizeof(fapi_pdcch_mnt_parms_t);

   *pduIndex += pdcchPduIdx;
}

/*******************************************************************
 *
 * @brief Converts PDSCH PDU
 *
 * @details
 *
 *    Function :  convertDlTtiPdschPdu
 *
 *    Functionality:
 *       - Converts PDSCH PDU struct from MAC's FAPI 
 *         version to UE Simulator's FAPI version.
 *
 * @params[in] MAC's PDSCH PDU struct
 *             UE sim's PDSCH PDU struct
 *             Offset of PDSCH PDU struct in its parent struct
 *
 * @return void
 *
 * ****************************************************************/
void convertDlTtiPdschPdu(fapi_dl_pdsch_pdu_t macPdschPdu, fapi_dl_pdsch_pdu_t_ue_sim *pdschPdu, uint16_t *pduIndex)
{
   uint8_t itr = 0;
   uint16_t pdschPduIdx = 0;

   FILL_PARAM(macPdschPdu.pduBitMap, pdschPdu->pdu_bit_map, pdschPduIdx);
   FILL_PARAM(macPdschPdu.rnti, pdschPdu->rnti, pdschPduIdx);
   FILL_PARAM(macPdschPdu.pdu_index, pdschPdu->pdu_index, pdschPduIdx);

   FILL_PARAM(macPdschPdu.bwpSize, pdschPdu->bwp.bwp_size, pdschPduIdx);
   FILL_PARAM(macPdschPdu.bwpStart, pdschPdu->bwp.bwp_start, pdschPduIdx);
   FILL_PARAM(macPdschPdu.subCarrierSpacing, pdschPdu->bwp.sub_carrier_spacing, pdschPduIdx);
   FILL_PARAM(macPdschPdu.cyclicPrefix, pdschPdu->bwp.cyclic_prefix, pdschPduIdx);

   FILL_PARAM(macPdschPdu.nrOfCodeWords, pdschPdu->nr_of_code_words, pdschPduIdx);
   for(itr = 0; itr < pdschPdu->nr_of_code_words; itr++)
      convertCodewordInfo(macPdschPdu.cwInfo[itr], &pdschPdu->cw_info[itr], &pdschPduIdx);

   FILL_PARAM(macPdschPdu.dataScramblingId, pdschPdu->nIdPdsch, pdschPduIdx);
   FILL_PARAM(macPdschPdu.nrOfLayers, pdschPdu->nr_of_layers, pdschPduIdx);
   FILL_PARAM(macPdschPdu.transmissionScheme, pdschPdu->transmission_scheme, pdschPduIdx);
   FILL_PARAM(macPdschPdu.refPoint, pdschPdu->ref_point, pdschPduIdx);

   FILL_PARAM(macPdschPdu.dlDmrsSymbPos, pdschPdu->dmrs.dl_dmrs_symb_pos, pdschPduIdx);
   FILL_PARAM(macPdschPdu.dmrsConfigType, pdschPdu->dmrs.dmrs_config_type, pdschPduIdx);
   FILL_PARAM(macPdschPdu.dlDmrsScramblingId, pdschPdu->dmrs.dl_dmrs_scrambling_id, pdschPduIdx);
   FILL_PARAM(macPdschPdu.scid, pdschPdu->dmrs.scid, pdschPduIdx);
   FILL_PARAM(macPdschPdu.numDmrsCdmGrpsNoData, pdschPdu->dmrs.num_dmrs_cdm_grps_no_data, pdschPduIdx);
   FILL_PARAM(macPdschPdu.dmrsPorts, pdschPdu->dmrs.dmrs_ports, pdschPduIdx);

   FILL_PARAM(macPdschPdu.resourceAlloc, pdschPdu->pdsch_freq_dom_alloc.resource_alloc, pdschPduIdx);
   memcpy(pdschPdu->pdsch_freq_dom_alloc.rb_bitmap, macPdschPdu.rbBitmap, 36*sizeof(uint8_t));
   pdschPduIdx += 36*sizeof(uint8_t);
   FILL_PARAM(macPdschPdu.rbStart, pdschPdu->pdsch_freq_dom_alloc.rb_start, pdschPduIdx);
   FILL_PARAM(macPdschPdu.rbSize, pdschPdu->pdsch_freq_dom_alloc.rb_size, pdschPduIdx);
   FILL_PARAM(macPdschPdu.vrbToPrbMapping, pdschPdu->pdsch_freq_dom_alloc.vrb_to_prb_mapping, pdschPduIdx);

   FILL_PARAM(macPdschPdu.startSymbIndex, pdschPdu->pdsch_time_dom_alloc.start_symb_index, pdschPduIdx);
   FILL_PARAM(macPdschPdu.nrOfSymbols, pdschPdu->pdsch_time_dom_alloc.nr_of_symbols, pdschPduIdx);

   FILL_PARAM(macPdschPdu.ptrsPortIndex, pdschPdu->pdsch_ptrs_info.ptrs_port_index, pdschPduIdx);
   FILL_PARAM(macPdschPdu.ptrsTimeDensity, pdschPdu->pdsch_ptrs_info.ptrs_time_density, pdschPduIdx);
   FILL_PARAM(macPdschPdu.ptrsFreqDensity, pdschPdu->pdsch_ptrs_info.ptrs_freq_density, pdschPduIdx);
   FILL_PARAM(macPdschPdu.ptrsReOffset, pdschPdu->pdsch_ptrs_info.ptrs_re_offset, pdschPduIdx);
   FILL_PARAM(macPdschPdu.nEpreRatioOfPdschToPtrs, pdschPdu->pdsch_ptrs_info.n_epre_ratio_of_pdsch_to_ptrs, pdschPduIdx);

   convertPreCodingAndBeamforming(macPdschPdu.preCodingAndBeamforming, &pdschPdu->pre_coding_and_beamforming, &pdschPduIdx);

   FILL_PARAM(macPdschPdu.powerControlOffset, pdschPdu->tx_pwr_info.powerControlOffsetProfileNR, pdschPduIdx);
   FILL_PARAM(macPdschPdu.powerControlOffsetSS, pdschPdu->tx_pwr_info.powerControlOffsetSSProfileNR, pdschPduIdx);

   FILL_PARAM(macPdschPdu.isLastCbPresent, pdschPdu->cbg_info.is_last_cb_present, pdschPduIdx);
   FILL_PARAM(macPdschPdu.isInlineTbCrc, pdschPdu->cbg_info.is_inline_tb_crc, pdschPduIdx);
   FILL_PARAM(macPdschPdu.dlTbCrc, pdschPdu->cbg_info.dlTbCrc[0], pdschPduIdx);
   FILL_PARAM(macPdschPdu.dlTbCrc, pdschPdu->cbg_info.dlTbCrc[0], pdschPduIdx);

   // unsused fapi_dl_pdsch_mnt_parms_t        dlPdschMntparms;
   pdschPduIdx += sizeof(fapi_dl_pdsch_mnt_parms_t);
   // unused fapi_dl_pdsch_ptrs_mnt_parms_t   dlPdschPtrsMntParms;
   pdschPduIdx += sizeof(fapi_dl_pdsch_ptrs_mnt_parms_t);
   // unused fapi_dl_rel16_pdsch_parms_t      rel16PdschParms;
   pdschPduIdx += sizeof(fapi_dl_rel16_pdsch_parms_t);

   *pduIndex += pdschPduIdx;
}

/*******************************************************************
 *
 * @brief Converts PBCH PDU
 *
 * @details
 *
 *    Function :  convertDlTtiPbchPdu
 *
 *    Functionality:
 *       - Converts PBCH PDU struct from MAC's FAPI 
 *         version to UE Simulator's FAPI version.
 *
 * @params[in] MAC's PBCH PDU struct
 *             UE sim's PBCH PDU struct
 *             Offset of PBCH PDU struct in its parent struct
 *
 * @return void
 *
 * ****************************************************************/
void convertDlTtiPbchPdu(fapi_dl_ssb_pdu_t macSsbPdu, fapi_dl_ssb_pdu_t_ue_sim *ssbPdu, uint16_t *pduIndex)
{
   uint16_t ssbPduIdx = 0;
   
   FILL_PARAM(macSsbPdu.physCellId, ssbPdu->phys_cell_id, ssbPduIdx);
   FILL_PARAM(macSsbPdu.betaPss, ssbPdu->betaPssProfileNR, ssbPduIdx);
   FILL_PARAM(macSsbPdu.ssbBlockIndex, ssbPdu->ssb_block_index, ssbPduIdx);
   FILL_PARAM(macSsbPdu.ssbSubCarrierOffset, ssbPdu->ssb_sub_carrier_offset, ssbPduIdx);
   FILL_PARAM(macSsbPdu.ssbOffsetPointA, ssbPdu->ssb_offset_point_a, ssbPduIdx);
   FILL_PARAM(macSsbPdu.bchPayloadFlag, ssbPdu->bch_payload_flag, ssbPduIdx);
   FILL_PARAM(macSsbPdu.bchPayload.bchPayload, ssbPdu->bch_payload.u.bch_payload, ssbPduIdx);
   convertPreCodingAndBeamforming(macSsbPdu.preCodingAndBeamforming, &ssbPdu->pre_coding_and_beamforming, &ssbPduIdx);

   // unused fapi_dl_ssb_pbch_mnt_parms_t  ssbPbchMntParms;
   ssbPduIdx += sizeof(fapi_dl_ssb_pbch_mnt_parms_t);
   *pduIndex += ssbPduIdx;
}

/*******************************************************************
 *
 * @brief Converts CSI-RS PDU
 *
 * @details
 *
 *    Function :  convertDlTtiCsiRsPdu
 *
 *    Functionality:
 *       - Converts CSI-RS PDU struct from MAC's FAPI 
 *         version to UE Simulator's FAPI version.
 *
 * @params[in] MAC's CSI-RS PDU struct
 *             UE sim's CSI-RS PDU struct
 *             Offset of CSI-RS PDU struct in its parent struct
 *
 * @return void
 *
 * ****************************************************************/
void convertDlTtiCsiRsPdu(fapi_dl_csi_rs_pdu_t macCsiRsPdu, fapi_dl_csi_rs_pdu_t_ue_sim *csiRsPdu, uint16_t *pduIndex)
{
   uint16_t csiRsIdx = 0;

   FILL_PARAM(macCsiRsPdu.subCarrierSpacing, csiRsPdu->subCarrierSpacing, csiRsIdx);
   FILL_PARAM(macCsiRsPdu.cyclicPrefix, csiRsPdu->cyclicPrefix, csiRsIdx);
   FILL_PARAM(macCsiRsPdu.startRb, csiRsPdu->start_rb, csiRsIdx);
   FILL_PARAM(macCsiRsPdu.nrOfRbs, csiRsPdu->nr_of_rbs, csiRsIdx);
   FILL_PARAM(macCsiRsPdu.csiType, csiRsPdu->csi_type, csiRsIdx);
   FILL_PARAM(macCsiRsPdu.row, csiRsPdu->row, csiRsIdx);
   FILL_PARAM(macCsiRsPdu.freqDomain, csiRsPdu->freq_domain, csiRsIdx);
   FILL_PARAM(macCsiRsPdu.symbL0, csiRsPdu->symb_l0, csiRsIdx);
   FILL_PARAM(macCsiRsPdu.symbL1, csiRsPdu->symb_l1, csiRsIdx);
   FILL_PARAM(macCsiRsPdu.cdmType, csiRsPdu->cdm_type, csiRsIdx);
   FILL_PARAM(macCsiRsPdu.freqDensity, csiRsPdu->freq_density, csiRsIdx);
   FILL_PARAM(macCsiRsPdu.scramId, csiRsPdu->scram_id, csiRsIdx);
   FILL_PARAM(macCsiRsPdu.powerControlOffset, csiRsPdu->tx_pwr_info.powerControlOffsetProfileNR, csiRsIdx);
   FILL_PARAM(macCsiRsPdu.powerControlOffsetSs, csiRsPdu->tx_pwr_info.powerControlOffsetSSProfileNR, csiRsIdx);
   convertPreCodingAndBeamforming(macCsiRsPdu.preCodingAndBeamforming, &csiRsPdu->pre_coding_and_beamforming, &csiRsIdx);
   //unused fapi_dl_csi_rs_mnt_parms_t  csiRsMntParms;
   csiRsIdx += sizeof(fapi_dl_csi_rs_mnt_parms_t);

   *pduIndex += csiRsIdx;
}

/*******************************************************************
 *
 * @brief Converts DL TTI PDU
 *
 * @details
 *
 *    Function :  convertDlTtiPdu
 *
 *    Functionality:
 *       - Converts DL TTI PDU struct from MAC's FAPI 
 *         version to UE Simulator's FAPI version.
 *
 * @params[in] MAC's DL TTI PDU struct
 *             UE sim's DL TTI PDU struct
 *             Offset of DL TTI PDU struct in its parent struct
 *
 * @return void
 *
 * ****************************************************************/
void convertDlTtiPdu(fapi_dl_tti_req_pdu_t macDlTtiPdu, fapi_dl_tti_req_pdu_t_ue_sim *dlTtiPdu, uint16_t *index)
{
   uint16_t pduIndex = 0;

   FILL_PARAM(macDlTtiPdu.pduType, dlTtiPdu->pdu_type, pduIndex);
   pduIndex += sizeof(dlTtiPdu->pdu_size);
   switch(macDlTtiPdu.pduType)
   {
      case FAPI_PDCCH_PDU_TYPE:
         convertDlPdcchPdu(macDlTtiPdu.pdu.pdcch_pdu, &dlTtiPdu->u.pdcch_pdu, &pduIndex);
         break;
      case FAPI_PDSCH_PDU_TYPE:
         convertDlTtiPdschPdu(macDlTtiPdu.pdu.pdsch_pdu, &dlTtiPdu->u.pdsch_pdu, &pduIndex);
         break;
      case FAPI_CSIRS_PDU_TYPE:
         convertDlTtiCsiRsPdu(macDlTtiPdu.pdu.csi_rs_pdu, &dlTtiPdu->u.csi_rs_pdu, &pduIndex);
         break;
      case FAPI_PBCH_PDU_TYPE:
         convertDlTtiPbchPdu(macDlTtiPdu.pdu.ssb_pdu, &dlTtiPdu->u.ssb_pdu, &pduIndex);
         break;
   }
   dlTtiPdu->pdu_size = pduIndex;
   *index += pduIndex;
}

/*******************************************************************
 *
 * @brief Converts UE Group Info
 *
 * @details
 *
 *    Function :  convertUeGrpInfo
 *
 *    Functionality:
 *       - Converts UE group info struct from MAC's FAPI 
 *         version to UE Simulator's FAPI version.
 *
 * @params[in] MAC's UE group info PDU struct
 *             UE sim's UE group info PDU struct
 *             Offset of UE group info struct in its parent struct
 *
 * @return void
 *
 * ****************************************************************/
void convertUeGrpInfo(fapi_ue_info_t macUeInfo, fapi_ue_info_t_ue_sim *ueInfo, uint16_t *pduIndex)
{
   uint8_t idx = 0, *pduIdx = NULLP;
   uint16_t ueInfoIdx = 0;

   FILL_PARAM(macUeInfo.nUe, ueInfo->n_ue, ueInfoIdx);
   for(idx = 0; idx < macUeInfo.nUe; idx++)
   {
      pduIdx = (void *)ueInfo + ueInfoIdx;  
      FILL_PARAM(macUeInfo.pduIdx[idx], *pduIdx, ueInfoIdx);
   }

   *pduIndex += ueInfoIdx;
}

/*****************************************************************
 * @brief Converts DL_TTI.Request
 *
 * @details
 *
 *    Function :  convertDlTtiRequest
 *
 *    Functionality:
 *       - Converts DL_TTI.request from MAC's FAPI version to
 *         UE Simulator's FAPI version.
 *
 * @params[in] Message pointer
 *
 * @return void
 *
 * ****************************************************************/
void convertDlTtiRequest(uint8_t handle, void *msg, uint16_t msgLen)
{
   uint8_t idx = 0;
   uint16_t index = 0, totalLength = 0;
   p_fapi_api_queue_elem_t dlTtiReqElem = (p_fapi_api_queue_elem_t)msg;
   fapi_dl_tti_req_t *macDlTtiReq = (fapi_dl_tti_req_t *)(dlTtiReqElem +1);
   fapi_dl_tti_req_t_ue_sim *dlTtiReq = NULLP;
   fapi_dl_tti_req_pdu_t_ue_sim *dlTtiPdu = NULLP;
   fapi_ue_info_t_ue_sim *ueInfo = NULLP;

   DU_LOG("\nDEBUG  -->  FAPI_CL: Received DL_TTI.request from MAC");

   FAPI_CL_ALLOC(dlTtiReq, MAX_PDU_SIZE);
   if(!dlTtiReq)
   {
      DU_LOG("\nERROR  -->  FAPI_CL: Memory allocation failed for DL_TTI.request");
      FAPI_CL_FREE(msg, msgLen);
   }

   memset(dlTtiReq, 0, MAX_PDU_SIZE);
   FILL_PARAM(1, dlTtiReq->header.numberOfMessagesIncluded, index);
   FILL_PARAM(handle, dlTtiReq->header.handle, index);
   FILL_PARAM(macDlTtiReq->header.msg_id, dlTtiReq->header.msg_details.message_type_id, index);
   index += sizeof(dlTtiReq->header.msg_details.length);

   FILL_PARAM(macDlTtiReq->sfn, dlTtiReq->sfn, index);
   FILL_PARAM(macDlTtiReq->slot, dlTtiReq->slot, index);
   FILL_PARAM(macDlTtiReq->nPdus, dlTtiReq->n_pdus, index);
   FILL_PARAM(MAX_NDLTYPES_DL_TTI_REQ, dlTtiReq->nDlTypes, index);
   for(idx=0; idx < dlTtiReq->nDlTypes; idx++)
      FILL_PARAM(0, dlTtiReq->nPDUsOfEachType[idx], index);
   FILL_PARAM(macDlTtiReq->nGroup, dlTtiReq->n_group, index);

   for(idx = 0; idx < macDlTtiReq->nPdus; idx++)
   {
      dlTtiPdu = (void *)dlTtiReq + index;
      convertDlTtiPdu(macDlTtiReq->pdus[idx], dlTtiPdu, &index);
   }
   for(idx = 0; idx < macDlTtiReq->nGroup; idx++)
   {
      ueInfo = (void *)dlTtiReq + index;
      convertUeGrpInfo(macDlTtiReq->ue_grp_info[idx], ueInfo, &index);
   }
   totalLength = index;
   dlTtiReq->header.msg_details.length = totalLength - sizeof(fapi_msg_header_t_ue_sim);

   sendMsgToUeSim(EVT_CNTL_REQUEST, (Data *)dlTtiReq, totalLength);
   FAPI_CL_FREE(msg, msgLen);
   FAPI_CL_FREE(dlTtiReq, MAX_PDU_SIZE);
}

/*******************************************************************
 *
 * @brief Convert UL Beamforming PDU
 *
 * @details
 *
 *    Function :  convertUlRxBfPdu
 *
 *    Functionality:
 *       - Converts UL Rx Beamforming PDU struct from MAC's FAPI 
 *         version to UE Simulator's FAPI version.
 *
 * @params[in] MAC's UL Rx Beamformimg PDU struct
 *             UE sim's UL Baemforming struct
 *             Offset of UL Beamforming struct in its parent struct
 *
 * @return void
 *
 * ****************************************************************/
void convertUlRxBfPdu(fapi_ul_rx_bmform_pdu_t macBfPdu, fapi_bmform_t *bfPdu, uint16_t *pduIdx)
{
   uint8_t itr = 0;
   uint16_t bfIdx = 0, *beamIdx = NULLP;

   FILL_PARAM(macBfPdu.numPrgs, bfPdu->num_prgs, bfIdx);
   FILL_PARAM(macBfPdu.prgSize, bfPdu->prg_size, bfIdx);
   FILL_PARAM(macBfPdu.digBfInterface, bfPdu->dig_bf_interfaces, bfIdx);
   for(itr = 0; itr < bfPdu->num_prgs; itr++)
   {
      beamIdx = (void *)bfPdu + bfIdx;
      FILL_PARAM(macBfPdu.rx_bfi[itr].beamIdx[0].beamidx, *beamIdx, bfIdx);
   }
   
   *pduIdx += bfIdx;
}

/*******************************************************************
 *
 * @brief Converts UL TTI PRACH PDU
 *
 * @details
 *
 *    Function :  convertUlPrachTtiPdu
 *
 *    Functionality:
 *       - Converts UL TTI PRACH PDU struct from MAC's FAPI 
 *         version to UE Simulator's FAPI version.
 *
 * @params[in] MAC's UL TTI PRACH PDU struct
 *             UE sim's UL TTI PRACH PDU struct
 *             Offset of UL TTI PRACH PDU struct in its parent struct
 *
 * @return void
 *
 * ****************************************************************/
void convertUlTtiPrachPdu(fapi_ul_prach_pdu_t macPrachPdu, fapi_ul_prach_pdu_t_ue_sim *prachPdu, uint16_t *pduIndex)
{
   uint16_t prachPduIdx = 0;

   FILL_PARAM(macPrachPdu.physCellId, prachPdu->phys_cell_id, prachPduIdx);
   FILL_PARAM(macPrachPdu.numPrachOcas, prachPdu->num_prach_ocas, prachPduIdx);
   FILL_PARAM(macPrachPdu.prachFormat, prachPdu->prach_format, prachPduIdx);
   FILL_PARAM(macPrachPdu.numRa, prachPdu->indexFdRa, prachPduIdx);
   FILL_PARAM(macPrachPdu.prachStartSymbol, prachPdu->prach_start_symbol, prachPduIdx);
   FILL_PARAM(macPrachPdu.numCs, prachPdu->num_cs, prachPduIdx);
   convertUlRxBfPdu(macPrachPdu.beamforming, &prachPdu->beamforming, &prachPduIdx);
   //unused fapi_pRACH_Mntnc_param_t    pRACH_Mntnc;
   prachPduIdx += sizeof(fapi_pRACH_Mntnc_param_t);

   *pduIndex += prachPduIdx;
}

/*******************************************************************
 *
 * @brief Converts UL TTI PUSCH PDU
 *
 * @details
 *
 *    Function :  convertUlTtiPusch Pdu
 *
 *    Functionality:
 *       - Converts UL TTI PUSCH PDU struct from MAC's FAPI 
 *         version to UE Simulator's FAPI version.
 *
 * @params[in] MAC's UL TTI PDU struct
 *             UE sim's UL TTI PUSCH PDU struct
 *             Offset of UL TTI PUSCH PDU struct in its parent struct
 *
 * @return void
 *
 * ****************************************************************/
void convertUlTtiPuschPdu(fapi_ul_pusch_pdu_t macPuschPdu, fapi_ul_pusch_pdu_t_ue_sim *puschPdu, uint16_t *pduIndex)
{
   uint16_t puschPduIdx = 0;

   FILL_PARAM(macPuschPdu.pduBitMap, puschPdu->pdu_bitmap, puschPduIdx);
   FILL_PARAM(macPuschPdu.rnti, puschPdu->rnti, puschPduIdx);
   FILL_PARAM(macPuschPdu.handle, puschPdu->handle, puschPduIdx);
   FILL_PARAM(macPuschPdu.bwpSize, puschPdu->bwp_size, puschPduIdx);
   FILL_PARAM(macPuschPdu.bwpStart, puschPdu->bwp_start, puschPduIdx);
   FILL_PARAM(macPuschPdu.subCarrierSpacing, puschPdu->sub_carrier_spacing, puschPduIdx);
   FILL_PARAM(macPuschPdu.cyclicPrefix, puschPdu->cyclic_prefix, puschPduIdx);
   FILL_PARAM(macPuschPdu.targetCodeRate, puschPdu->target_code_rate, puschPduIdx);
   FILL_PARAM(macPuschPdu.qamModOrder, puschPdu->qam_mod_order, puschPduIdx);
   FILL_PARAM(macPuschPdu.mcsIndex, puschPdu->mcs_index, puschPduIdx);
   FILL_PARAM(macPuschPdu.mcsTable, puschPdu->mcs_table, puschPduIdx);
   FILL_PARAM(macPuschPdu.transformPrecoding, puschPdu->transform_precoding, puschPduIdx);
   FILL_PARAM(macPuschPdu.dataScramblingId, puschPdu->nIdPusch, puschPduIdx);
   FILL_PARAM(macPuschPdu.nrOfLayers, puschPdu->nr_of_layers, puschPduIdx);
   FILL_PARAM(macPuschPdu.ulDmrsSymbPos, puschPdu->ul_dmrs_symb_pos, puschPduIdx);
   FILL_PARAM(macPuschPdu.dmrsConfigType, puschPdu->dmrs_config_type, puschPduIdx);
   FILL_PARAM(macPuschPdu.ulDmrsScramblingId, puschPdu->ul_dmrs_scrambling_id, puschPduIdx);
   FILL_PARAM(macPuschPdu.nTpPuschId, puschPdu->pusch_identity, puschPduIdx);
   FILL_PARAM(macPuschPdu.scid, puschPdu->nScid, puschPduIdx);
   FILL_PARAM(macPuschPdu.numDmrsCdmGrpsNoData, puschPdu->num_dmrs_cdm_grps_no_data, puschPduIdx);
   FILL_PARAM(macPuschPdu.dmrsPorts, puschPdu->dmrs_ports, puschPduIdx);
   FILL_PARAM(macPuschPdu.resourceAlloc, puschPdu->resource_alloc, puschPduIdx);
   memcpy(puschPdu->rb_bitmap, macPuschPdu.rbBitmap, 36*sizeof(uint8_t));
   puschPduIdx += 36*sizeof(uint8_t);
   FILL_PARAM(macPuschPdu.rbStart, puschPdu->rb_start, puschPduIdx);
   FILL_PARAM(macPuschPdu.rbSize, puschPdu->rb_size, puschPduIdx);
   FILL_PARAM(macPuschPdu.vrbToPrbMapping, puschPdu->vrb_to_prb_mapping, puschPduIdx);
   FILL_PARAM(macPuschPdu.frequencyHopping, puschPdu->IntraSlotFrequencyHopping, puschPduIdx);
   FILL_PARAM(macPuschPdu.txDirectCurrentLocation, puschPdu->tx_direct_current_location, puschPduIdx);
   FILL_PARAM(macPuschPdu.uplinkFrequencyShift7p5khz, puschPdu->uplink_frequency_shift7p5khz, puschPduIdx);
   FILL_PARAM(macPuschPdu.startSymbIndex, puschPdu->start_symb_index, puschPduIdx);
   FILL_PARAM(macPuschPdu.nrOfSymbols, puschPdu->nr_of_symbols, puschPduIdx);

   /* PUSCH Data Info */
   FILL_PARAM(macPuschPdu.puschData.rvIndex, puschPdu->pusch_data.rv_index, puschPduIdx);
   FILL_PARAM(macPuschPdu.puschData.harqProcessId, puschPdu->pusch_data.harq_process_id, puschPduIdx);
   FILL_PARAM(macPuschPdu.puschData.newDataIndicator, puschPdu->pusch_data.new_data_indicator, puschPduIdx);
   FILL_PARAM(macPuschPdu.puschData.tbSize, puschPdu->pusch_data.tb_size, puschPduIdx);
   FILL_PARAM(macPuschPdu.puschData.numCb, puschPdu->pusch_data.num_cb, puschPduIdx);
   FILL_PARAM(macPuschPdu.puschData.cbPresentAndPosition[0], puschPdu->pusch_data.cb_present_and_position[0], puschPduIdx);

   /* PUSCH UCI */
   FILL_PARAM(macPuschPdu.puschUci.harqAckBitLength, puschPdu->pusch_uci.harq_ack_bit_length, puschPduIdx);
   FILL_PARAM(macPuschPdu.puschUci.csiPart1BitLength, puschPdu->pusch_uci.csi_part1_bit_length, puschPduIdx);
   FILL_PARAM(macPuschPdu.puschUci.csiPart2BitLength, puschPdu->pusch_uci.flagCsiPart2, puschPduIdx);
   FILL_PARAM(macPuschPdu.puschUci.alphaScaling, puschPdu->pusch_uci.alpha_scaling, puschPduIdx);
   FILL_PARAM(macPuschPdu.puschUci.betaOffsetHarqAck, puschPdu->pusch_uci.beta_offset_harq_ack, puschPduIdx);
   FILL_PARAM(macPuschPdu.puschUci.betaOffsetCsi1, puschPdu->pusch_uci.beta_offset_csi1, puschPduIdx);
   FILL_PARAM(macPuschPdu.puschUci.betaOffsetCsi2, puschPdu->pusch_uci.beta_offset_csi2, puschPduIdx);

   /* PUSCH PTRS */
   FILL_PARAM(macPuschPdu.puschPtrs.numPtrsPorts, puschPdu->pusch_ptrs.num_ptrs_ports, puschPduIdx);
   FILL_PARAM(macPuschPdu.puschPtrs.ptrsInfo[0].ptrsPortIndex, puschPdu->pusch_ptrs.ptrs_info[0].ptrs_port_index, puschPduIdx);
   FILL_PARAM(macPuschPdu.puschPtrs.ptrsInfo[0].ptrsDmrsPort, puschPdu->pusch_ptrs.ptrs_info[0].ptrs_dmrs_port, puschPduIdx);
   FILL_PARAM(macPuschPdu.puschPtrs.ptrsInfo[0].ptrsReOffset, puschPdu->pusch_ptrs.ptrs_info[0].ptrs_re_offset, puschPduIdx);
   FILL_PARAM(macPuschPdu.puschPtrs.ptrsInfo[1].ptrsPortIndex, puschPdu->pusch_ptrs.ptrs_info[1].ptrs_port_index, puschPduIdx);
   FILL_PARAM(macPuschPdu.puschPtrs.ptrsInfo[1].ptrsDmrsPort, puschPdu->pusch_ptrs.ptrs_info[1].ptrs_dmrs_port, puschPduIdx);
   FILL_PARAM(macPuschPdu.puschPtrs.ptrsInfo[1].ptrsReOffset, puschPdu->pusch_ptrs.ptrs_info[1].ptrs_re_offset, puschPduIdx);
   puschPduIdx += 10* sizeof(fapi_ptrs_info_t);
   FILL_PARAM(macPuschPdu.puschPtrs.ptrsTimeDensity, puschPdu->pusch_ptrs.ptrs_time_density, puschPduIdx);
   FILL_PARAM(macPuschPdu.puschPtrs.ptrsFreqDensity, puschPdu->pusch_ptrs.ptrs_freq_density, puschPduIdx);
   FILL_PARAM(macPuschPdu.puschPtrs.ulPtrsPower, puschPdu->pusch_ptrs.ul_ptrs_power, puschPduIdx);

   /* DFTS OFDM */
   FILL_PARAM(macPuschPdu.dftsOfdm.lowPaprGroupNumber, puschPdu->dfts_ofdm.low_papr_group_number, puschPduIdx);
   FILL_PARAM(macPuschPdu.dftsOfdm.lowPaprSequenceNumber, puschPdu->dfts_ofdm.low_papr_sequence_number, puschPduIdx);
   FILL_PARAM(macPuschPdu.dftsOfdm.ulPtrsSampleDensity, puschPdu->dfts_ofdm.ul_ptrs_sample_density, puschPduIdx);
   FILL_PARAM(macPuschPdu.dftsOfdm.ulPtrsTimeDensityTransformPrecoding, puschPdu->dfts_ofdm.ul_ptrs_time_density_transform_precoding, puschPduIdx);

   /* Beamforming */
   convertUlRxBfPdu(macPuschPdu.beamforming, &puschPdu->beamforming, &puschPduIdx);

   //Unused fapi_pusch_mnt_parms_t      puschMntParms;
   puschPduIdx += sizeof(fapi_pusch_mnt_parms_t);
   //Unsued fapi_optional_pusch_uci_t   optionalPuschUci
   puschPduIdx += sizeof(fapi_optional_pusch_uci_t);

   *pduIndex += puschPduIdx; 
}

/*******************************************************************
 *
 * @brief Converts UL TTI PUCCH PDU
 *
 * @details
 *
 *    Function :  convertUlTtiPucchPdu
 *
 *    Functionality:
 *       - Converts UL TTI PUCCH PDU struct from MAC's FAPI 
 *         version to UE Simulator's FAPI version.
 *
 * @params[in] MAC's UL TTI PUCCH PDU struct
 *             UE sim's UL TTI PUCCH PDU struct
 *             Offset of UL TTI PUCCH PDU struct in its parent struct
 *
 * @return void
 *
 * ****************************************************************/
void convertUlTtiPucchPdu(fapi_ul_pucch_pdu_t macPucchPdu, fapi_ul_pucch_pdu_t_ue_sim *pucchPdu, uint16_t *pduIndex)
{
   uint16_t pucchPduIdx = 0;

   FILL_PARAM(macPucchPdu.rnti, pucchPdu->rnti, pucchPduIdx);
   FILL_PARAM(macPucchPdu.handle, pucchPdu->handle, pucchPduIdx);
   FILL_PARAM(macPucchPdu.bwpSize, pucchPdu->bwp_size, pucchPduIdx);
   FILL_PARAM(macPucchPdu.bwpStart, pucchPdu->bwp_start, pucchPduIdx);
   FILL_PARAM(macPucchPdu.subCarrierSpacing, pucchPdu->sub_carrier_spacing, pucchPduIdx);
   FILL_PARAM(macPucchPdu.cyclicPrefix, pucchPdu->cyclic_prefix, pucchPduIdx);
   FILL_PARAM(macPucchPdu.formatType, pucchPdu->format_type, pucchPduIdx);
   FILL_PARAM(macPucchPdu.multiSlotTxIndicator, pucchPdu->multi_slot_tx_indicator, pucchPduIdx);
   FILL_PARAM(macPucchPdu.pi2Bpsk, pucchPdu->pi2_bpsk, pucchPduIdx);
   FILL_PARAM(macPucchPdu.prbStart, pucchPdu->prb_start, pucchPduIdx);
   FILL_PARAM(macPucchPdu.prbSize, pucchPdu->prb_size, pucchPduIdx);
   FILL_PARAM(macPucchPdu.startSymbolIndex, pucchPdu->start_symbol_index, pucchPduIdx);
   FILL_PARAM(macPucchPdu.nrOfSymbols, pucchPdu->nr_of_symbols, pucchPduIdx);
   FILL_PARAM(macPucchPdu.freqHopFlag, pucchPdu->intraSlotFrequencyHopping, pucchPduIdx);
   FILL_PARAM(macPucchPdu.secondHopPrb, pucchPdu->second_hop_prb, pucchPduIdx);
   FILL_PARAM(macPucchPdu.groupHopFlag, pucchPdu->pucchGroupHopping, pucchPduIdx);
   //Obsolete 8 bit
   pucchPduIdx += sizeof(uint8_t);
   FILL_PARAM(macPucchPdu.hoppingId, pucchPdu->nIdPucchHopping, pucchPduIdx);
   FILL_PARAM(macPucchPdu.initialCyclicShift, pucchPdu->initial_cyclic_shift, pucchPduIdx);
   FILL_PARAM(macPucchPdu.dataScramblingId, pucchPdu->nIdPucchScrambling, pucchPduIdx);
   FILL_PARAM(macPucchPdu.timeDomainOccIdx, pucchPdu->time_domain_occ_idx, pucchPduIdx);
   FILL_PARAM(macPucchPdu.preDftOccIdx, pucchPdu->pre_dft_occ_idx, pucchPduIdx);
   FILL_PARAM(macPucchPdu.preDftOccLen, pucchPdu->pre_dft_occ_len, pucchPduIdx);
   FILL_PARAM(macPucchPdu.addDmrsFlag, pucchPdu->add_dmrs_flag, pucchPduIdx);
   FILL_PARAM(macPucchPdu.dmrsScramblingId, pucchPdu->dmrs_scrambling_id, pucchPduIdx);
   FILL_PARAM(macPucchPdu.dmrsCyclicShift, pucchPdu->dmrs_cyclic_shift, pucchPduIdx);
   FILL_PARAM(macPucchPdu.srFlag, pucchPdu->sr_flag, pucchPduIdx);
   FILL_PARAM(macPucchPdu.bitLenHarq, pucchPdu->bit_len_harq, pucchPduIdx);
   FILL_PARAM(macPucchPdu.bitLenCsiPart1, pucchPdu->csiPart1BitLength, pucchPduIdx);

   convertUlRxBfPdu(macPucchPdu.beamforming, &pucchPdu->beamforming, &pucchPduIdx);

   //Unsued fapi_pucch_mntnnc_params_t  pucch_basicextension
   pucchPduIdx += sizeof(fapi_pucch_mntnnc_params_t);
   //Unused fapi_uci_part1_part2_t      uci_part1_part2;
   pucchPduIdx += sizeof(fapi_uci_part1_part2_t);

   *pduIndex += pucchPduIdx;
}

/*******************************************************************
 *
 * @brief Converts UL TTI SRS PDU
 *
 * @details
 *
 *    Function :  convertUlTtiSrsPdu
 *
 *    Functionality:
 *       - Converts UL TTI SRS PDU struct from MAC's FAPI 
 *         version to UE Simulator's FAPI version.
 *
 * @params[in] MAC's UL TTI SRS PDU struct
 *             UE sim's UL TTI SRS PDU struct
 *             Offset of SRS PDU struct in its parent struct
 *
 * @return void
 *
 * ****************************************************************/
void convertUlTtiSrsPdu(fapi_ul_srs_pdu_t macSrsPdu, fapi_ul_srs_pdu_t_ue_sim *srsPdu, uint16_t *pduIndex)
{
   uint16_t srsPduIdx = 0;

   FILL_PARAM(macSrsPdu.rnti, srsPdu->rnti, srsPduIdx);
   FILL_PARAM(macSrsPdu.handle, srsPdu->handle, srsPduIdx);
   FILL_PARAM(macSrsPdu.bwpSize, srsPdu->bwp_size, srsPduIdx);
   FILL_PARAM(macSrsPdu.bwpStart, srsPdu->bwp_start, srsPduIdx);
   FILL_PARAM(macSrsPdu.subCarrierSpacing, srsPdu->sub_carrier_spacing, srsPduIdx);
   FILL_PARAM(macSrsPdu.cyclicPrefix, srsPdu->cyclic_prefix, srsPduIdx);
   FILL_PARAM(macSrsPdu.numAntPorts, srsPdu->num_ant_ports, srsPduIdx);
   FILL_PARAM(macSrsPdu.numSymbols, srsPdu->num_symbols, srsPduIdx);
   FILL_PARAM(macSrsPdu.numRepetitions, srsPdu->num_repetitions, srsPduIdx);
   FILL_PARAM(macSrsPdu.timeStartPosition, srsPdu->time_start_position, srsPduIdx);
   FILL_PARAM(macSrsPdu.configIndex, srsPdu->config_index, srsPduIdx);
   FILL_PARAM(macSrsPdu.sequenceId, srsPdu->sequence_id, srsPduIdx);
   FILL_PARAM(macSrsPdu.bandwidthIndex, srsPdu->bandwidth_index, srsPduIdx);
   FILL_PARAM(macSrsPdu.combSize, srsPdu->comb_size, srsPduIdx);
   FILL_PARAM(macSrsPdu.combOffset, srsPdu->comb_offset, srsPduIdx);
   FILL_PARAM(macSrsPdu.cyclicShift, srsPdu->cyclic_shift, srsPduIdx);
   FILL_PARAM(macSrsPdu.frequencyPosition, srsPdu->frequency_position, srsPduIdx);
   FILL_PARAM(macSrsPdu.frequencyShift, srsPdu->frequency_shift, srsPduIdx);
   FILL_PARAM(macSrsPdu.frequencyHopping, srsPdu->frequency_hopping, srsPduIdx);
   FILL_PARAM(macSrsPdu.groupOrSequenceHopping, srsPdu->group_or_sequence_hopping, srsPduIdx);
   FILL_PARAM(macSrsPdu.resourceType, srsPdu->resource_type, srsPduIdx);
   FILL_PARAM(macSrsPdu.tSrs, srsPdu->t_srs, srsPduIdx);
   FILL_PARAM(macSrsPdu.tOffset, srsPdu->t_offset, srsPduIdx);

   convertUlRxBfPdu(macSrsPdu.beamforming, &srsPdu->beamforming, &srsPduIdx);

   *pduIndex += srsPduIdx;
}

/*******************************************************************
 *
 * @brief Converts UL TTI PDU
 *
 * @details
 *
 *    Function :  convertUlTtiPdu
 *
 *    Functionality:
 *       - Converts UL TTI PDU struct from MAC's FAPI 
 *         version to UE Simulator's FAPI version.
 *
 * @params[in] MAC's UL TTI PDU struct
 *             UE sim's UL TTI PDU struct
 *             Offset of UL TTI PDU struct in its parent struct
 *
 * @return void
 *
 * ****************************************************************/
void convertUlTtiPdu(fapi_ul_tti_req_pdu_t macUlTtiPdu, fapi_ul_tti_req_pdu_t_ue_sim *ulTtiPdu, uint16_t *index)
{
   uint16_t pduIndex = 0;

   FILL_PARAM(macUlTtiPdu.pduType, ulTtiPdu->pdu_type, pduIndex);
   pduIndex += sizeof(ulTtiPdu->pdu_size);
   switch(macUlTtiPdu.pduType)
   {
      case FAPI_PRACH_PDU_TYPE:
         convertUlTtiPrachPdu(macUlTtiPdu.pdu.prach_pdu, &ulTtiPdu->u.prach_pdu, &pduIndex);
         break;
      case FAPI_PUSCH_PDU_TYPE:
         convertUlTtiPuschPdu(macUlTtiPdu.pdu.pusch_pdu, &ulTtiPdu->u.pusch_pdu, &pduIndex);
         break;
      case FAPI_PUCCH_PDU_TYPE:
         convertUlTtiPucchPdu(macUlTtiPdu.pdu.pucch_pdu, &ulTtiPdu->u.pucch_pdu, &pduIndex);
         break;
      case FAPI_SRS_PDU_TYPE:
         convertUlTtiSrsPdu(macUlTtiPdu.pdu.srs_pdu, &ulTtiPdu->u.srs_pdu, &pduIndex);
         break;
   }
   ulTtiPdu->pdu_size = pduIndex;
   *index += pduIndex;
}

/*****************************************************************
 * @brief Converts UL_TTI.Request
 *
 * @details
 *
 *    Function :  convertUlTtiRequest
 *
 *    Functionality:
 *       - Converts UL_TTI.request from MAC's FAPI version to
 *         UE Simulator's FAPI version.
 *
 * @params[in] Message pointer
 *
 * @return void
 *
 * ****************************************************************/
void convertUlTtiRequest(uint8_t handle, void *msg, uint16_t msgLen)
{
   uint8_t idx = 0;
   uint16_t index = 0, totalLength = 0;
   p_fapi_api_queue_elem_t ulTtiReqElem = (p_fapi_api_queue_elem_t)msg;
   fapi_ul_tti_req_t *macUlTtiReq = (fapi_ul_tti_req_t *)(ulTtiReqElem +1);
   fapi_ul_tti_req_t_ue_sim *ulTtiReq = NULLP;
   fapi_ul_tti_req_pdu_t_ue_sim *ulTtiPdu = NULLP;

   DU_LOG("\nDEBUG  -->  FAPI_CL: Received UL_TTI.request from MAC");

   FAPI_CL_ALLOC(ulTtiReq, MAX_PDU_SIZE);
   if(!ulTtiReq)
   {
      DU_LOG("\nERROR  -->  FAPI_CL: Memory allocation failed for UL_TTI.request");
      FAPI_CL_FREE(msg, msgLen);
   }

   memset(ulTtiReq, 0, MAX_PDU_SIZE);
   FILL_PARAM(1, ulTtiReq->header.numberOfMessagesIncluded, index);
   FILL_PARAM(handle, ulTtiReq->header.handle, index);
   FILL_PARAM(macUlTtiReq->header.msg_id, ulTtiReq->header.msg_details.message_type_id, index);
   index += sizeof(ulTtiReq->header.msg_details.length);

   FILL_PARAM(macUlTtiReq->sfn, ulTtiReq->sfn, index);
   FILL_PARAM(macUlTtiReq->slot, ulTtiReq->slot, index);
   FILL_PARAM(macUlTtiReq->nPdus, ulTtiReq->n_pdus, index);
   FILL_PARAM(MAX_NUM_UL_TYPES, ulTtiReq->nUlTypes, index);
   for(idx=0; idx < ulTtiReq->nUlTypes; idx++)
      FILL_PARAM(0, ulTtiReq->nPDUsOfEachType[idx], index);
   FILL_PARAM(macUlTtiReq->nGroup, ulTtiReq->n_group, index);

   for(idx = 0; idx < macUlTtiReq->nPdus; idx++)
   {
      ulTtiPdu = (void *)ulTtiReq + index;
      convertUlTtiPdu(macUlTtiReq->pdus[idx], ulTtiPdu, &index);
   }
   totalLength = index;
   ulTtiReq->header.msg_details.length = totalLength - sizeof(fapi_msg_header_t_ue_sim);

   sendMsgToUeSim(EVT_CNTL_REQUEST, (Data *)ulTtiReq, totalLength);
   FAPI_CL_FREE(msg, msgLen);
   FAPI_CL_FREE(ulTtiReq, MAX_PDU_SIZE);
}

/*******************************************************************
 *
 * @brief Converts UL_DCI PDU
 *
 * @details
 *
 *    Function :  convertUlDciPdu
 *
 *    Functionality:
 *       -Converts Ul Dci Pdu struct from MAC's FAPI 
 *         version to UE Simulator's FAPI version. 
 *
 * @params[in] MAC's UL DCI PDU struct
 *             UE sim's UL DCI PDU struct
 *             Offset of PDU in its parent structure
 *
 * @return void
 *
 * ****************************************************************/
void convertUlDciPdu(fapi_dci_pdu_t macUlDciPdu, fapi_dci_pdu_t_ue_sim *ulDciPdu, uint16_t *index)
{
   uint16_t dciPduIdx = 0;

   FILL_PARAM(macUlDciPdu.pduType, ulDciPdu->pdu_type, dciPduIdx);
   dciPduIdx += sizeof(ulDciPdu->pdu_size);
   convertDlPdcchPdu(macUlDciPdu.pdcchPduConfig, &ulDciPdu->pdcch_pdu, &dciPduIdx);
   ulDciPdu->pdu_size = dciPduIdx;

   *index += dciPduIdx;
}

/* ****************************************************************
 * @details Converts UL_DCI.request
 *
 *    Function :  convertUlDciRequest
 *
 *    Functionality:
 *       -Converts UL_DCI.request from MAC's FAPI 
 *         version to UE Simulator's FAPI version. 
 *
 * @params[in] Cell ID
 *             Message Pointer
 *             Message Length
 *
 * @return void
 *
 * ****************************************************************/
void convertUlDciRequest(uint8_t handle, void *msg, uint16_t msgLen)
{
   uint8_t idx = 0;
   uint16_t index = 0, totalLength = 0;
   p_fapi_api_queue_elem_t ulDciReqElem = (p_fapi_api_queue_elem_t)msg;
   fapi_ul_dci_req_t *macUlDciReq = (fapi_ul_dci_req_t *)(ulDciReqElem +1);
   fapi_ul_dci_req_t_ue_sim *ulDciReq = NULLP;
   fapi_dci_pdu_t_ue_sim *ulDciPdu = NULLP;

   DU_LOG("\nDEBUG  -->  FAPI_CL: Received UL_DCI.request from MAC");

   FAPI_CL_ALLOC(ulDciReq, MAX_PDU_SIZE);
   if(!ulDciReq)
   {   
      DU_LOG("\nERROR  -->  FAPI_CL: Memory allocation failed for UL_DCI.request");
      FAPI_CL_FREE(msg, msgLen);
   }   

   memset(ulDciReq, 0, MAX_PDU_SIZE);
   FILL_PARAM(1, ulDciReq->header.numberOfMessagesIncluded, index);
   FILL_PARAM(handle, ulDciReq->header.handle, index);
   FILL_PARAM(macUlDciReq->header.msg_id, ulDciReq->header.msg_details.message_type_id, index);
   index += sizeof(ulDciReq->header.msg_details.length);

   FILL_PARAM(macUlDciReq->sfn, ulDciReq->sfn, index);
   FILL_PARAM(macUlDciReq->slot, ulDciReq->slot, index);
   FILL_PARAM(macUlDciReq->numPdus, ulDciReq->num_pdus, index);
   FILL_PARAM(MAX_NDLTYPES_UL_DCI_REQ, ulDciReq->nDlTypes, index);
   for(idx=0; idx < ulDciReq->nDlTypes; idx++)
      FILL_PARAM(0, ulDciReq->nPDUsOfEachType[idx], index);

   for(idx = 0; idx < macUlDciReq->numPdus; idx++)
   {
      ulDciPdu = (void *)ulDciReq + index;
      convertUlDciPdu(macUlDciReq->pdus[idx], ulDciPdu, &index);
   }
   totalLength = index;
   ulDciReq->header.msg_details.length = totalLength - sizeof(fapi_msg_header_t_ue_sim);

   sendMsgToUeSim(EVT_CNTL_REQUEST, (Data *)ulDciReq, totalLength);
   FAPI_CL_FREE(msg, msgLen);
   FAPI_CL_FREE(ulDciReq, MAX_PDU_SIZE);
}

/*******************************************************************
 *
 * @brief Converts TX_DATA PDU's TLV
 *
 * @details
 *
 *    Function :  convertTxDataTlv
 *
 *    Functionality:
 *       -Converts TxData Pdu Tlv struct from MAC's FAPI 
 *         version to UE Simulator's FAPI version. 
 *
 * @params[in] MAC's Tx Data PDU TLV struct
 *             UE sim's Tx Data PDU TLV struct
 *             Offset of TLV in its parent structure
 *
 * @return void
 *
 * ****************************************************************/
void convertTxDataTlv(fapi_uint8_ptr_tlv_t macTxDataTlv, fapi_tx_dat_tlv_t *txDataTlv, uint16_t *pduIndex)
{
   uint16_t tlvIndex = 0;
   uint32_t *tlvValue = NULLP;

   FILL_PARAM(macTxDataTlv.tl.tag, txDataTlv->tag, tlvIndex);
   FILL_PARAM(macTxDataTlv.tl.length, txDataTlv->length, tlvIndex);
   tlvValue = (void *)txDataTlv + tlvIndex;
   memcpy(tlvValue, macTxDataTlv.value, txDataTlv->length);
   tlvIndex += txDataTlv->length;

   FAPI_CL_FREE(macTxDataTlv.value, txDataTlv->length);

   *pduIndex += tlvIndex;
}

/*******************************************************************
 *
 * @brief Converts TX_DATA PDU
 *
 * @details
 *
 *    Function :  convertTxDataPdu
 *
 *    Functionality:
 *       -Converts TxData Pdu struct from MAC's FAPI 
 *         version to UE Simulator's FAPI version. 
 *
 * @params[in] MAC's Tx Data PDU struct
 *             UE sim's Tx Data PDU struct
 *             Offset of PDU in its parent structure
 *
 * @return void
 *
 * ****************************************************************/
void convertTxDataPdu(fapi_tx_pdu_desc_t macTxDataPdu, fapi_tx_pdu_desc_t_ue_sim *txDataPdu, uint16_t *index)
{
   uint8_t idx = 0;
   uint16_t pduIndex = 0;
   fapi_tx_dat_tlv_t *tlv = NULLP;

   pduIndex += sizeof(txDataPdu->pduLength); // value of txDataPdu->pduLength will be filled at the end
   FILL_PARAM(macTxDataPdu.pdu_index, txDataPdu->pduIndex, pduIndex);
   FILL_PARAM(0, txDataPdu->cwIndex, pduIndex);
   FILL_PARAM(macTxDataPdu.num_tlvs, txDataPdu->numTlvs, pduIndex);

   for(idx = 0; idx < macTxDataPdu.num_tlvs; idx++)
   {
      tlv = (void *)txDataPdu + pduIndex;
      convertTxDataTlv(macTxDataPdu.tlvs[idx], tlv, &pduIndex);
   }
   txDataPdu->pduLength = pduIndex;
   *index += pduIndex;
}

/*******************************************************************
 *
 * @brief Converts TX_DATA.request
 *
 * @details
 *
 *    Function :  convertTxDataRequest
 *
 *    Functionality:
 *       -Converts TX_DATA.request from MAC's FAPI 
 *         version to UE Simulator's FAPI version. 
 *
 * @params[in] Cell ID
 *             Message Pointer
 *             Message Length
 *
 * @return void
 *
 * ****************************************************************/
void convertTxDataRequest(uint8_t handle, void *msg, uint16_t msgLen)
{
   uint8_t idx = 0;
   uint16_t index = 0, totalLength = 0;
   p_fapi_api_queue_elem_t txDataReqElem = (p_fapi_api_queue_elem_t)msg;
   fapi_tx_data_req_t *macTxDataReq = (fapi_tx_data_req_t *)(txDataReqElem +1);
   fapi_tx_data_req_t_ue_sim *txDataReq = NULLP;
   fapi_tx_pdu_desc_t_ue_sim *txDataPdu = NULLP;

   DU_LOG("\nDEBUG  -->  FAPI_CL: Received TX_DATA.request from MAC");

   FAPI_CL_ALLOC(txDataReq, MAX_PDU_SIZE);
   if(!txDataReq)
   {   
      DU_LOG("\nERROR  -->  FAPI_CL: Memory allocation failed for TX_DATA.request");
      FAPI_CL_FREE(msg, msgLen);
   }   

   memset(txDataReq, 0, MAX_PDU_SIZE);
   FILL_PARAM(1, txDataReq->header.numberOfMessagesIncluded, index);
   FILL_PARAM(handle, txDataReq->header.handle, index);
   FILL_PARAM(macTxDataReq->header.msg_id, txDataReq->header.msg_details.message_type_id, index);
   index += sizeof(txDataReq->header.msg_details.length);

   FILL_PARAM(macTxDataReq->sfn, txDataReq->sfn, index);
   FILL_PARAM(macTxDataReq->slot, txDataReq->slot, index);
   FILL_PARAM(macTxDataReq->num_pdus, txDataReq->numPdus, index);

   for(idx = 0; idx < macTxDataReq->num_pdus; idx++)
   {
      txDataPdu = (void *)txDataReq + index;
      convertTxDataPdu(macTxDataReq->pdu_desc[idx], txDataPdu, &index);
   }
   totalLength = index;
   txDataReq->header.msg_details.length = totalLength - sizeof(fapi_msg_header_t_ue_sim);

   sendMsgToUeSim(EVT_DATA_REQUEST, (Data *)txDataReq, totalLength);
   FAPI_CL_FREE(msg, msgLen);
   FAPI_CL_FREE(txDataReq, MAX_PDU_SIZE);
}

/*******************************************************************
 *
 * @brief Converts STOP.request
 *
 * @details
 *
 *    Function :  convertStopRequest
 *
 *    Functionality:
 *       -Converts STOP.request from MAC's FAPI 
 *         version to UE Simulator's FAPI version. 
 *
 * @params[in] Cell ID
 *             Message Pointer
 *             Message Length
 *
 * @return void
 *
 * ****************************************************************/
void convertStopRequest(uint8_t handle, void *msg, uint16_t msgLen)
{
   p_fapi_api_queue_elem_t stopReqElem = (p_fapi_api_queue_elem_t)msg;
   fapi_stop_req_t *macStopReq = (fapi_stop_req_t *)(stopReqElem +1);
   fapi_stop_req_t_ue_sim stopReq;

   DU_LOG("\nDEBUG  -->  FAPI_CL: Received STOP.request from MAC");

   memset(&stopReq, 0, sizeof(fapi_stop_req_t_ue_sim));
   stopReq.header.numberOfMessagesIncluded = 1;
   stopReq.header.handle = handle;
   stopReq.header.msg_details.message_type_id = macStopReq->header.msg_id;
   stopReq.header.msg_details.length = sizeof(fapi_stop_req_t_ue_sim) - sizeof(fapi_msg_header_t_ue_sim);

   FAPI_CL_FREE(msg, msgLen);

   sendMsgToUeSim(EVT_CNTL_REQUEST, (Data *)&stopReq, sizeof(fapi_stop_req_t_ue_sim));
}

/*******************************************************************
 *
 * @brief Receives message from MAC and call appropriate handler
 *
 * @details
 *
 *    Function :  fapiClProcessMac2UeSimFapiMsg
 *
 *    Functionality:
 *       - Receives message from MAC and calls appropriate handler
 *
 * @params[in] Message pointer
 *
 * @return void
 *
 * ****************************************************************/

void fapiClProcessMac2UeSimFapiMsg(void *msg)
{
   uint8_t handle = 0;
   uint16_t msgLen =0;
   p_fapi_api_queue_elem_t currMsg = NULLP;
   p_fapi_api_queue_elem_t nextMsg = NULLP;

   currMsg = (p_fapi_api_queue_elem_t)msg;
   while(currMsg)
   {   
      nextMsg = currMsg->p_next;
      msgLen = currMsg->msg_len + sizeof(fapi_api_queue_elem_t);
      if(currMsg->msg_type == FAPI_VENDOR_MSG_HEADER_IND)
      {
         handle = ((fapi_msg_header_t *)(currMsg +1))->handle;
         FAPI_CL_FREE(currMsg, msgLen);
      }
      else if(currMsg->msg_type == FAPI_VENDOR_MESSAGE)
      {
         FAPI_CL_FREE(currMsg, msgLen);   
      }   
      else
      {
         switch(currMsg->msg_type)
         {
            case FAPI_CONFIG_REQUEST:
               convertConfigRequest(handle, currMsg, msgLen);
               break;
            case FAPI_START_REQUEST:
               convertStartRequest(handle, currMsg, msgLen);
               break;
            case FAPI_DL_TTI_REQUEST:
               convertDlTtiRequest(handle, currMsg, msgLen);
               break;
            case FAPI_UL_TTI_REQUEST:
               convertUlTtiRequest(handle, currMsg, msgLen);
               break;
            case FAPI_UL_DCI_REQUEST:
               convertUlDciRequest(handle, currMsg, msgLen);
               break;
            case FAPI_TX_DATA_REQUEST:
               convertTxDataRequest(handle, currMsg, msgLen);
               break;
            case FAPI_STOP_REQUEST:
               clCb.slotIndInfo.slotIndicationStarted = false;
               FAPI_CL_FREE(currMsg, msgLen);
               break;
            default:
               DU_LOG("\nERROR  -->  FAPI_CL: Invalid message id [%d] from MAC", currMsg->msg_type);
         }
      }
      currMsg = nextMsg;
   } 
}

/**********************************************************************
  End of file
 **********************************************************************/
