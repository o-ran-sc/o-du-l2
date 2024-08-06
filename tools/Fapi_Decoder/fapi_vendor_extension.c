#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include "../../src/intel_fapi/fapi_interface.h"
#include "../../src/intel_fapi/fapi_vendor_extension.h"
#include "../../src/intel_fapi/nfapi_interface.h"
#include "../../src/intel_fapi/nr5g_fapi_common_types.h"
#include "../../src/intel_fapi/nr5g_fapi_internal.h"
#include "fapi_interface.c"

void decode_fapi_api_queue_elem_t( fapi_api_queue_elem_t* elem) 
{
    if (elem == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_api_queue_elem_t \t");
    printf("msg_type= %u\t", elem->msg_type);
    printf("num_message_in_block= %u\t", elem->num_message_in_block);
    printf("msg_len= %u\t", elem->msg_len);
    printf("align_offset = %u\t", elem->align_offset);
    printf("time_stamp= %llu\t", elem->time_stamp);
    printf("\n");
}

void decode_fapi_config_req_vendor_msg_t( fapi_config_req_vendor_msg_t* config_req) 
{
    if (config_req == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_config_req_vendor_msg_t \t");
    printf("hopping_id= %u\t", config_req->hopping_id);
    printf("carrier_aggregation_level= %u\t", config_req->carrier_aggregation_level);
    printf("group_hop_flag= %u\t", config_req->group_hop_flag);
    printf("sequence_hop_flag= %u\t", config_req->sequence_hop_flag);
    printf("prach_nr_of_rx_ru= %u\t", config_req->prach_nr_of_rx_ru);
    printf("nr_of_dl_ports= %u\t", config_req->nr_of_dl_ports);
    printf("nr_of_ul_ports= %u\t", config_req->nr_of_ul_ports);
    printf("urllc_capable= %u\t", config_req->urllc_capable);
    printf("urllc_mini_slot_mask= %u\t", config_req->urllc_mini_slot_mask);
    printf("ssb_subc_spacing= %u\t", config_req->ssb_subc_spacing);
    printf("use_vendor_EpreXSSB= %u\t", config_req->use_vendor_EpreXSSB);
    printf("pad= %u %u\t", config_req->pad[0], config_req->pad[1]);
    printf("\n");
}

void decode_fapi_start_req_vendor_msg_t( fapi_start_req_vendor_msg_t* start_req) 
{
    if (start_req == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_start_req_vendor_msg_t \t");
    printf("sfn= %u\t", start_req->sfn);
    printf("slot = %u\t", start_req->slot);
    printf("mode= %u\t", start_req->mode);
#ifdef DEBUG_MODE
    printf("count = %u\t", start_req->count);
    printf("period= %u\t", start_req->period);
#endif
    printf("\n");
}

void decode_fapi_stop_req_vendor_msg_t( fapi_stop_req_vendor_msg_t* stop_req) 
{
    if (stop_req == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_stop_req_vendor_msg_t \t");
    printf("sfn= %u\t", stop_req->sfn);
    printf("slot = %u\t", stop_req->slot);
    printf("\n");
}

void decode_fapi_vendor_ul_pusch_pdu_t( fapi_vendor_ul_pusch_pdu_t* pusch_pdu) 
{
    if (pusch_pdu == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ul_pusch_pdu_t \t");
    printf("nr_of_antenna_ports= %u\t", pusch_pdu->nr_of_antenna_ports);
    printf("nr_of_rx_ru= %u\t", pusch_pdu->nr_of_rx_ru);
    printf("pad= %u %u\t", pusch_pdu->pad[0], pusch_pdu->pad[1]);
    printf("rx_ru_idx = ");
    for (int i = 0; i < pusch_pdu->nr_of_rx_ru; ++i) {
        printf("%u ", pusch_pdu->rx_ru_idx[i]);
    }
    printf("\n");
}

void decode_fapi_vendor_ul_pucch_pdu_t( fapi_vendor_ul_pucch_pdu_t* pucch_pdu) 
{
    if (pucch_pdu == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ul_pucch_pdu_t \t");
    printf("nr_of_rx_ru= %u\t", pucch_pdu->nr_of_rx_ru);
    printf("pad= %u\t", pucch_pdu->pad[0]);
    printf("group_id= %u\t", pucch_pdu->group_id);
    printf("rx_ru_idx = ");
    for (int i = 0; i < pucch_pdu->nr_of_rx_ru; ++i) {
        printf("%u ", pucch_pdu->rx_ru_idx[i]);
    }
    printf("\n");
}

void decode_fapi_vendor_ul_srs_pdu_t( fapi_vendor_ul_srs_pdu_t* srs_pdu) 
{
    if (srs_pdu == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ul_srs_pdu_t \t");
    printf("nr_of_rx_ru= %u\t", srs_pdu->nr_of_rx_ru);
    printf("pad= %u %u %u\t", srs_pdu->pad[0], srs_pdu->pad[1], srs_pdu->pad[2]);
    printf("rx_ru_idx = ");
    for (int i = 0; i < srs_pdu->nr_of_rx_ru; ++i) {
        printf("%u ", srs_pdu->rx_ru_idx[i]);
    }
    printf("\n");
}

void decode_fapi_vendor_ul_tti_req_pdu_t( fapi_vendor_ul_tti_req_pdu_t* ul_tti_req_pdu) 
{
    if (ul_tti_req_pdu == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ul_tti_req_pdu_t \t");
    printf("pdu_type= %u\t", ul_tti_req_pdu->pdu_type);
    printf("pad= %u\t", ul_tti_req_pdu->pad);
    switch (ul_tti_req_pdu->pdu_type) 
    {
        case FAPI_PUSCH_PDU_TYPE: 
            printf("pusch_pdu\n");
            decode_fapi_vendor_ul_pusch_pdu_t(&ul_tti_req_pdu->pdu.pusch_pdu);
            break;
        case FAPI_PUCCH_PDU_TYPE: 
            printf("pucch_pdu\n");
            decode_fapi_vendor_ul_pucch_pdu_t(&ul_tti_req_pdu->pdu.pucch_pdu);
            break;
        case FAPI_SRS_PDU_TYPE: 
            printf("srs_pdu\n");
            decode_fapi_vendor_ul_srs_pdu_t(&ul_tti_req_pdu->pdu.srs_pdu);
            break;
        default:
            printf("Unknown PDU type\n");
            break;
    }
    printf("\n");
}

void decode_fapi_vendor_ul_tti_req_t( fapi_vendor_ul_tti_req_t* ul_tti_req) 
{
    if (ul_tti_req == NULL) 
    {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ul_tti_req_t \t");
    printf("sym= %u\t", ul_tti_req->sym);
    printf("num_ul_pdu= %u\t", ul_tti_req->num_ul_pdu);
    printf("pad= %u %u %u\t", ul_tti_req->pad[0], ul_tti_req->pad[1], ul_tti_req->pad[2]);

    for (int i = 0; i < ul_tti_req->num_ul_pdu; ++i) 
    {
        printf("ul_pdus[%d]:\n", i);
        decode_fapi_vendor_ul_tti_req_pdu_t(&ul_tti_req->ul_pdus[i]);
    }
    printf("\n");
}


void decode_fapi_vendor_dl_dci_t( fapi_vendor_dl_dci_t* dl_dci) 
{
    if (dl_dci == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_dl_dci_t \t");
    printf("epre_ratio_of_pdcch_to_ssb= %u\t", dl_dci->epre_ratio_of_pdcch_to_ssb);
    printf("epre_ratio_of_dmrs_to_ssb= %u\t", dl_dci->epre_ratio_of_dmrs_to_ssb);
    printf("\n");
}

void decode_fapi_vendor_dl_pdcch_pdu_t( fapi_vendor_dl_pdcch_pdu_t* dl_pdcch_pdu) {
    if (dl_pdcch_pdu == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_dl_pdcch_pdu_t \t");
    printf("num_dl_dci= %u\t", dl_pdcch_pdu->num_dl_dci);
    printf("pad= %u %u\t", dl_pdcch_pdu->pad[0], dl_pdcch_pdu->pad[1]);

    for (int i = 0; i < dl_pdcch_pdu->num_dl_dci; ++i) {
        printf("dl_dci[%d]:\n", i);
        decode_fapi_vendor_dl_dci_t(&dl_pdcch_pdu->dl_dci[i]);
    }
    printf("\n");
}

void decode_fapi_vendor_dl_pdsch_pdu_t( fapi_vendor_dl_pdsch_pdu_t* pdsch_pdu) {
    if (pdsch_pdu == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_dl_pdsch_pdu_t \t");
    printf("epre_ratio_of_pdsch_to_ssb= %u\t", pdsch_pdu->epre_ratio_of_pdsch_to_ssb);
    printf("epre_ratio_of_dmrs_to_ssb= %u\t", pdsch_pdu->epre_ratio_of_dmrs_to_ssb);
    printf("nr_of_antenna_ports= %u\t", pdsch_pdu->nr_of_antenna_ports);
    printf("pad= %u %u %u\t", pdsch_pdu->pad[0], pdsch_pdu->pad[1], pdsch_pdu->pad[2]);
    printf("tx_ru_idx = ");
    for (int i = 0; i < FAPI_VENDOR_MAX_TXRU_NUM; ++i) {
        printf("%u ", pdsch_pdu->tx_ru_idx[i]);
    }
    printf("\n");
}

void decode_fapi_vendor_csi_rs_pdu_t( fapi_vendor_csi_rs_pdu_t* csi_rs_pdu) {
    if (csi_rs_pdu == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_csi_rs_pdu_t \t");
    printf("epre_ratio_to_ssb= %u\t", csi_rs_pdu->epre_ratio_to_ssb);
    printf("pad= %u %u\t", csi_rs_pdu->pad[0], csi_rs_pdu->pad[1]);
    printf("\n");
}

void decode_fapi_vendor_dl_tti_req_pdu_t( fapi_vendor_dl_tti_req_pdu_t* dl_tti_req_pdu) {
    if (dl_tti_req_pdu == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_dl_tti_req_pdu_t \t");
    printf("pdu_type= %u\t", dl_tti_req_pdu->pdu_type);
    printf("pdu_size= %u\t", dl_tti_req_pdu->pdu_size);
    switch (dl_tti_req_pdu->pdu_type) 
    {
        case FAPI_PDCCH_PDU_TYPE: 
            printf("pdcch_pdu:\n");
            decode_fapi_vendor_dl_pdcch_pdu_t(&dl_tti_req_pdu->pdu.pdcch_pdu);
            break;
        case FAPI_PDSCH_PDU_TYPE: 
            printf("pdsch_pdu:\n");
            decode_fapi_vendor_dl_pdsch_pdu_t(&dl_tti_req_pdu->pdu.pdsch_pdu);
            break;
        case FAPI_CSIRS_PDU_TYPE: 
            printf("csi_rs_pdu:\n");
            decode_fapi_vendor_csi_rs_pdu_t(&dl_tti_req_pdu->pdu.csi_rs_pdu);
            break;
        default :
            printf("Unknown PDU type\n");
            break;
    }
    printf("\n");
}

void decode_fapi_vendor_dl_tti_req_t( fapi_vendor_dl_tti_req_t* dl_tti_req) {
    if (dl_tti_req == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_dl_tti_req_t \t");
    printf("sym= %u\t", dl_tti_req->sym);
    printf("lte_crs_carrier_freq_dl= %u\t", dl_tti_req->lte_crs_carrier_freq_dl);
    printf("lte_crs_present = %u\t", dl_tti_req->lte_crs_present);
    printf("lte_crs_carrier_bandwidth_dl= %u\t", dl_tti_req->lte_crs_carrier_bandwidth_dl);
    printf("lte_crs_nr_of_crs_ports= %u\t", dl_tti_req->lte_crs_nr_of_crs_ports);
    printf("lte_crs_v_shift = %u\t", dl_tti_req->lte_crs_v_shift);
    printf("pdcch_precoder_en= %u\t", dl_tti_req->pdcch_precoder_en);
    printf("ssb_precoder_en= %u\t", dl_tti_req->ssb_precoder_en);
    printf("num_pdus= %u\t", dl_tti_req->num_pdus);
    printf("pad= %u %u %u\t", dl_tti_req->pad[0], dl_tti_req->pad[1], dl_tti_req->pad[2]);

    for (int i = 0; i < dl_tti_req->num_pdus; ++i) {
        printf("pdus[%d]:\t", i);
        decode_fapi_vendor_dl_tti_req_pdu_t(&dl_tti_req->pdus[i]);
    }
    printf("\n");
}

void decode_fapi_vendor_dci_pdu_t( fapi_vendor_dci_pdu_t* dci_pdu) {
    if (dci_pdu == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_dci_pdu_t \t");
    printf("pdu_type= %u\t", dci_pdu->pdu_type);
    printf("pdu_size= %u\t", dci_pdu->pdu_size);
    printf("pdcch_pdu_config:\n");
    decode_fapi_vendor_dl_pdcch_pdu_t(&dci_pdu->pdcch_pdu_config);
    printf("\n");
}

void decode_fapi_vendor_ul_dci_req_t( fapi_vendor_ul_dci_req_t* ul_dci_req) {
    if (ul_dci_req == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ul_dci_req_t \t");
    printf("sym= %u\t", ul_dci_req->sym);
    printf("num_pdus= %u\t", ul_dci_req->num_pdus);
    printf("pad= %u %u %u\t", ul_dci_req->pad[0], ul_dci_req->pad[1], ul_dci_req->pad[2]);

    for (int i = 0; i < ul_dci_req->num_pdus; ++i) {
        printf("pdus[%d]:\n", i);
        decode_fapi_vendor_dci_pdu_t(&ul_dci_req->pdus[i]);
    }
    printf("\n");
}

void decode_fapi_vendor_tx_data_req_t( fapi_vendor_tx_data_req_t* tx_data_req) {
    if (tx_data_req == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_tx_data_req_t \t");
    printf("sym= %u\t", tx_data_req->sym);
    printf("\n");
}

void decode_fapi_vendor_p7_msg_t( fapi_vendor_p7_msg_t* p7_msg) {
    if (p7_msg == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_p7_msg_t \n");
    decode_fapi_vendor_dl_tti_req_t(&p7_msg->dl_tti_req);
    decode_fapi_vendor_ul_tti_req_t(&p7_msg->ul_tti_req);
    decode_fapi_vendor_ul_dci_req_t(&p7_msg->ul_dci_req);
    decode_fapi_vendor_tx_data_req_t(&p7_msg->tx_data_req);
    printf("\n");
}

void decode_fapi_vendor_msg_t( fapi_vendor_msg_t* vendor_msg) {
    if (vendor_msg == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_msg_t \n");
    printf("header:\n");
    decode_fapi_msg(&vendor_msg->header);
    decode_fapi_config_req_vendor_msg_t(&vendor_msg->config_req_vendor);
    decode_fapi_start_req_vendor_msg_t(&vendor_msg->start_req_vendor);
    decode_fapi_stop_req_vendor_msg_t(&vendor_msg->stop_req_vendor);
    decode_fapi_vendor_p7_msg_t(&vendor_msg->p7_req_vendor);
    printf("\n");
}

void decode_fapi_vendor_ext_shutdown_req_t( fapi_vendor_ext_shutdown_req_t* shutdown_req) {
    if (shutdown_req == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ext_shutdown_req_t \t");
    printf("sfn= %u\t", shutdown_req->sfn);
    printf("slot = %u\t", shutdown_req->slot);
    printf("test_type= %u\t", shutdown_req->test_type);
    printf("\n");
}

void decode_fapi_vendor_ext_shutdown_res_t( fapi_vendor_ext_shutdown_res_t* shutdown_res) {
    if (shutdown_res == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ext_shutdown_res_t \t");
    printf("sfn= %u\t", shutdown_res->sfn);
    printf("slot = %u\t", shutdown_res->slot);
    printf("nStatus= %u\t", shutdown_res->nStatus);
    printf("\n");
}

void decode_fapi_vendor_ext_snr_t( fapi_vendor_ext_snr_t* snr) {
    if (snr == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ext_snr_t \t");
    for (int i = 0; i < MAX_SNR_COUNT; ++i) {
        printf("nSNR[%d]= %d\t", i, snr->nSNR[i]);
    }
    printf("pad= %d\t", snr->pad);
    printf("\n");
}

void decode_fapi_vendor_ext_srs_pdu_t( fapi_vendor_ext_srs_pdu_t* srs_pdu) {
    if (srs_pdu == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ext_srs_pdu_t \t");
    printf("nr_of_port = %u\t", srs_pdu->nr_of_port);
    printf("nr_of_rx_ant = %u\t", srs_pdu->nr_of_rx_ant);
    printf("nr_of_rbs= %u\t", srs_pdu->nr_of_rbs);
    printf("is_chan_est_pres= %u\t", srs_pdu->is_chan_est_pres);
    printf("pad= %u %u %u\t", srs_pdu->pad[0], srs_pdu->pad[1], srs_pdu->pad[2]);

    for (int i = 0; i < srs_pdu->nr_of_port; ++i) {
        for (int j = 0; j < srs_pdu->nr_of_rx_ant; ++j) {
            printf("p_srs_chan_est[%d][%d]= %d\t", i, j, srs_pdu->p_srs_chan_est[i][j]);
        }
    }
    printf("\n");
}

void decode_fapi_vendor_ext_srs_ind_t( fapi_vendor_ext_srs_ind_t* srs_ind) {
    if (srs_ind == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ext_srs_ind_t \t");
    printf("num_pdus= %u\t", srs_ind->num_pdus);
    printf("pad= %u %u %u\t", srs_ind->pad[0], srs_ind->pad[1], srs_ind->pad[2]);

    for (int i = 0; i < srs_ind->num_pdus; ++i) {
        printf("srs_pdus[%d]:\n", i);
        decode_fapi_vendor_ext_srs_pdu_t(&srs_ind->srs_pdus[i]);
    }
    printf("\n");
}

void decode_fapi_vendor_ext_slot_ind_t( fapi_vendor_ext_slot_ind_t* slot_ind) {
    if (slot_ind == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ext_slot_ind_t \t");
    printf("carrier_idx= %u\t", slot_ind->carrier_idx);
    printf("sym= %u\t", slot_ind->sym);
    printf("\n");
}

void decode_fapi_vendor_ext_rx_data_ind_t( fapi_vendor_ext_rx_data_ind_t* rx_data_ind) {
    if (rx_data_ind == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ext_rx_data_ind_t \t");
    printf("carrier_idx= %u\t", rx_data_ind->carrier_idx);
    printf("sym= %u\t", rx_data_ind->sym);
    printf("\n");
}

void decode_fapi_vendor_ext_crc_ind_t( fapi_vendor_ext_crc_ind_t* crc_ind) {
    if (crc_ind == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ext_crc_ind_t \t");
    printf("carrier_idx= %u\t", crc_ind->carrier_idx);
    printf("sym= %u\t", crc_ind->sym);
    printf("\n");
}

void decode_fapi_vendor_ext_uci_ind_t( fapi_vendor_ext_uci_ind_t* uci_ind) {
    if (uci_ind == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ext_uci_ind_t \t");
    printf("carrier_idx= %u\t", uci_ind->carrier_idx);
    printf("sym= %u\t", uci_ind->sym);
    printf("\n");
}

void decode_fapi_vendor_p7_ind_msg_t(fapi_vendor_p7_ind_msg_t* msg) 
{
    if (msg== NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_p7_ind_msg_t \n");
    decode_fapi_msg(&msg->header);
   decode_fapi_vendor_ext_snr_t(&msg->crc_snr);
   decode_fapi_vendor_ext_snr_t(&msg->uci_snr);
   decode_fapi_vendor_ext_srs_ind_t(&msg->srs_ind);
   decode_fapi_vendor_ext_slot_ind_t(&msg->slot_ind);
   decode_fapi_vendor_ext_rx_data_ind_t(&msg->rx_data_ind);
   decode_fapi_vendor_ext_crc_ind_t(&msg->crc_ind);
   decode_fapi_vendor_ext_uci_ind_t(&msg->uci_ind);
     
}

#ifdef DEBUG_MODE
void decode_fapi_vendor_ext_iq_samples_info_t( fapi_vendor_ext_iq_samples_info_t* iq_samples_info) {
    if (iq_samples_info == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ext_iq_samples_info_t \n");
    printf("carrNum= %u\n", iq_samples_info->carrNum);
    printf("numSubframes= %u\n", iq_samples_info->numSubframes);
    printf("nIsRadioMode= %u\n", iq_samples_info->nIsRadioMode);
    printf("timerModeFreqDomain= %u\n", iq_samples_info->timerModeFreqDomain);
    printf("phaseCompensationEnable= %u\n", iq_samples_info->phaseCompensationEnable);
    printf("startFrameNum= %u\n", iq_samples_info->startFrameNum);
    printf("startSlotNum= %u\n", iq_samples_info->startSlotNum);
    printf("startSymNum= %u\n", iq_samples_info->startSymNum);

    printf("filename_in_ul_iq:\n");
    for (int i = 0; i < FAPI_MAX_IQ_SAMPLE_UL_VIRTUAL_PORTS; ++i) {
        printf("  [%d]= %s\n", i, iq_samples_info->filename_in_ul_iq[i]);
    }

    printf("filename_in_ul_iq_compressed:\n");
    for (int i = 0; i < FAPI_MAX_IQ_SAMPLE_UL_VIRTUAL_PORTS; ++i) {
        printf("  [%d]= %s\n", i, iq_samples_info->filename_in_ul_iq_compressed[i]);
    }

    printf("filename_in_prach_iq:\n");
    for (int i = 0; i < FAPI_MAX_IQ_SAMPLE_UL_VIRTUAL_PORTS; ++i) {
        printf("  [%d]= %s\n", i, iq_samples_info->filename_in_prach_iq[i]);
    }

    printf("filename_in_srs_iq:\n");
    for (int i = 0; i < FAPI_MAX_IQ_SAMPLE_UL_ANTENNA; ++i) {
        printf("  [%d]= %s\n", i, iq_samples_info->filename_in_srs_iq[i]);
    }

    printf("filename_out_dl_iq= %s\n", iq_samples_info->filename_out_dl_iq);

    printf("filename_out_dl_beam:\n");
    for (int i = 0; i < FAPI_MAX_IQ_SAMPLE_DL_PORTS; ++i) {
        printf("  [%d]= %s\n", i, iq_samples_info->filename_out_dl_beam[i]);
    }

    printf("filename_out_ul_beam:\n");
    for (int i = 0; i < FAPI_MAX_IQ_SAMPLE_UL_VIRTUAL_PORTS; ++i) {
        printf("  [%d]= %s\n", i, iq_samples_info->filename_out_ul_beam[i]);
    }

    printf("filename_out_dl_iq_compressed= %s\n", iq_samples_info->filename_out_dl_iq_compressed);

    printf("nDLCompressionIdx= %u\n", iq_samples_info->nDLCompressionIdx);
    printf("nDLCompiqWidth= %u\n", iq_samples_info->nDLCompiqWidth);
    printf("nDLCompScaleFactor= %u\n", iq_samples_info->nDLCompScaleFactor);
    printf("nDLCompreMask= %u\n", iq_samples_info->nDLCompreMask);

    printf("nULDecompressionIdx= %u\n", iq_samples_info->nULDecompressionIdx);
    printf("nULDecompiqWidth= %u\n", iq_samples_info->nULDecompiqWidth);

    printf("buffer size= %lu\n", sizeof(iq_samples_info->buffer));
}

void decode_fapi_vendor_ext_iq_samples_req_t( fapi_vendor_ext_iq_samples_req_t* req) {
    if (req == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ext_iq_samples_req_t \n");
    decode_fapi_msg(&req->header);
    decode_fapi_vendor_ext_iq_samples_info_t(&req->iq_samples_info);
}

void decode_fapi_vendor_ext_dl_iq_samples_res_t( fapi_vendor_ext_dl_iq_samples_res_t* res) {
    if (res == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ext_dl_iq_samples_res_t \n");
    decode_fapi_msg(&res->header);
}

void decode_fapi_vendor_ext_ul_iq_samples_res_t( fapi_vendor_ext_ul_iq_samples_res_t* res) 
{
    if (res == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ext_ul_iq_samples_res_t \n");
    decode_fapi_msg(&res->header);
}

void decode_fapi_vendor_ext_start_response_t( fapi_vendor_ext_start_response_t* res) 
{
    if (res == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ext_start_response_t \n");
    decode_fapi_msg(&res->header);
}

void decode_fapi_vendor_ext_add_remove_core_info_t( fapi_vendor_ext_add_remove_core_info_t* info) 
{
    if (info == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ext_add_remove_core_info_t \n");
    printf("eOption= %u\n", info->eOption);

    printf("nCoreMask:\n");
    for (int i = 0; i < FAPI_MAX_MASK_OPTIONS; ++i) {
        for (int j = 0; j < FAPI_MAX_NUM_SET_CORE_MASK; ++j) {
            printf("  [%d][%d]= %llu\n", i, j, info->nCoreMask[i][j]);
        }
    }

    printf("nMacOptions:\n");
    for (int i = 0; i < FAPI_NUM_SPLIT_OPTIONS; ++i) {
        printf("  [%d]= %u\n", i, info->nMacOptions[i]);
    }
}

void decode_fapi_vendor_ext_add_remove_core_msg_t( fapi_vendor_ext_add_remove_core_msg_t* msg) 
{
    if (msg == NULL) {
        printf("Error == Null pointer received.\n");
        return;
    }
    printf("fapi_vendor_ext_add_remove_core_msg_t \n");
    decode_fapi_msg(&msg->header);
    decode_fapi_vendor_ext_add_remove_core_info_t(&msg->add_remove_core_info);
}
#endif
