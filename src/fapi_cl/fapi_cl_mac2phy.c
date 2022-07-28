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
   p_fapi_api_queue_elem_t configReqElem = (p_fapi_api_queue_elem_t)msg;
   fapi_start_req_t *macStartReq = (fapi_start_req_t *)(configReqElem +1);
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
 *    Function :  convertDlTtiPdcchPdu
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
void convertDlTtiPdcchPdu(fapi_dl_pdcch_pdu_t macPdcchPdu, fapi_dl_pdcch_pdu_t_ue_sim *pdcchPdu, uint16_t *pduIndex)
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
void convertCsiRsPdu(fapi_dl_csi_rs_pdu_t macCsiRsPdu, fapi_dl_csi_rs_pdu_t_ue_sim *csiRsPdu, uint16_t *pduIndex)
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
         convertDlTtiPdcchPdu(macDlTtiPdu.pdu.pdcch_pdu, &dlTtiPdu->u.pdcch_pdu, &pduIndex);
         break;
      case FAPI_PDSCH_PDU_TYPE:
         convertDlTtiPdschPdu(macDlTtiPdu.pdu.pdsch_pdu, &dlTtiPdu->u.pdsch_pdu, &pduIndex);
         break;
      case FAPI_CSIRS_PDU_TYPE:
         convertCsiRsPdu(macDlTtiPdu.pdu.csi_rs_pdu, &dlTtiPdu->u.csi_rs_pdu, &pduIndex);
         break;
      case FAPI_PBCH_PDU_TYPE:
         convertDlTtiPbchPdu(macDlTtiPdu.pdu.ssb_pdu, &dlTtiPdu->u.ssb_pdu, &pduIndex);
         break;
   }
   dlTtiPdu->pdu_size = pduIndex;
   *index += pduIndex;
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
   totalLength = index;
   dlTtiReq->header.msg_details.length = totalLength - sizeof(fapi_msg_header_t_ue_sim);

   sendMsgToUeSim(EVT_CNTL_REQUEST, (Data *)dlTtiReq, totalLength);
   FAPI_CL_FREE(msg, msgLen);
   FAPI_CL_FREE(dlTtiReq, MAX_PDU_SIZE);
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
