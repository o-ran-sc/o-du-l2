#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "queue_elem.h"
#include <stdbool.h>
#include "/root/pborla/29July/l2/src/intel_fapi/fapi_interface.h"
#include "/root/pborla/29July/l2/src/intel_fapi/fapi_vendor_extension.h"
#include "/root/pborla/29July/l2/src/intel_fapi/nfapi_interface.h"
#include "/root/pborla/29July/l2/src/intel_fapi/nr5g_fapi_common_types.h"
#include "/root/pborla/29July/l2/src/intel_fapi/nr5g_fapi_internal.h"

void print_fapiSupport_t(fapiSupport_t support) {
    switch (support) {
        case FAPI_NOT_SUPPORTED:
            printf("FAPI_NOT_SUPPORTED\n");
            break;
        case FAPI_SUPPORTED:
            printf("FAPI_SUPPORTED\n");
            break;
        default:
            printf("Unknown fapiSupport_t value\n");
    }
}

void print_fapiTlvStatus_t(fapiTlvStatus_t status) {
    switch (status) {
        case FAPI_IDLE_STATE_ONLY_OPTIONAL:
            printf("FAPI_IDLE_STATE_ONLY_OPTIONAL\n");
            break;
        case FAPI_IDLE_STATE_ONLY_MANDATORY:
            printf("FAPI_IDLE_STATE_ONLY_MANDATORY\n");
            break;
        case FAPI_IDLE_AND_CONFIGURED_STATES_OPTIONAL:
            printf("FAPI_IDLE_AND_CONFIGURED_STATES_OPTIONAL\n");
            break;
        case FAPI_IDLE_STATE_MANDATORY_CONFIGURED_STATE_OPTIONAL:
            printf("FAPI_IDLE_STATE_MANDATORY_CONFIGURED_STATE_OPTIONAL\n");
            break;
        case FAPI_IDLE_CONFIGURED_AND_RUNNING_STATES_OPTIONAL:
            printf("FAPI_IDLE_CONFIGURED_AND_RUNNING_STATES_OPTIONAL\n");
            break;
        case FAPI_IDLE_STATE_MANDATORY_CONFIGURED_AND_RUNNING_STATES_OPTIONAL:
            printf("FAPI_IDLE_STATE_MANDATORY_CONFIGURED_AND_RUNNING_STATES_OPTIONAL\n");
            break;
        default:
            printf("Unknown fapiTlvStatus_t value\n");
    }
}

void print_fapiModOrder_t(fapiModOrder_t modOrder) {
    switch (modOrder) {
        case FAPI_QPSK:
            printf("FAPI_QPSK\n");
            break;
        case FAPI_16QAM:
            printf("FAPI_16QAM\n");
            break;
        case FAPI_64QAM:
            printf("FAPI_64QAM\n");
            break;
        case FAPI_256QAM:
            printf("FAPI_256QAM\n");
            break;
        default:
            printf("Unknown fapiModOrder_t value\n");
    }
}

void print_fapiSsbPerRach_t(fapiSsbPerRach_t ssbPerRach) {
    switch (ssbPerRach) {
        case FAPI_SSB_PER_RACH_1_OVER_8:
            printf("FAPI_SSB_PER_RACH_1_OVER_8\n");
            break;
        case FAPI_SSB_PER_RACH_1_OVER_4:
            printf("FAPI_SSB_PER_RACH_1_OVER_4\n");
            break;
        case FAPI_SSB_PER_RACH_1_OVER_2:
            printf("FAPI_SSB_PER_RACH_1_OVER_2\n");
            break;
        case FAPI_SSB_PER_RACH_1:
            printf("FAPI_SSB_PER_RACH_1\n");
            break;
        case FAPI_SSB_PER_RACH_2:
            printf("FAPI_SSB_PER_RACH_2\n");
            break;
        case FAPI_SSB_PER_RACH_4:
            printf("FAPI_SSB_PER_RACH_4\n");
            break;
        case FAPI_SSB_PER_RACH_8:
            printf("FAPI_SSB_PER_RACH_8\n");
            break;
        case FAPI_SSB_PER_RACH_16:
            printf("FAPI_SSB_PER_RACH_16\n");
            break;
        default:
            printf("Unknown fapiSsbPerRach_t value\n");
    }
}

void print_fapi_config_req_vendor_msg_t(const fapi_config_req_vendor_msg_t *msg) {
    printf("fapi_config_req_vendor_msg_t:\n");
    printf("  hopping_id: %u\n", msg->hopping_id);
    printf("  carrier_aggregation_level: %u\n", msg->carrier_aggregation_level);
    printf("  group_hop_flag: %u\n", msg->group_hop_flag);
    printf("  sequence_hop_flag: %u\n", msg->sequence_hop_flag);
    printf("  prach_nr_of_rx_ru: %u\n", msg->prach_nr_of_rx_ru);
    printf("  nr_of_dl_ports: %u\n", msg->nr_of_dl_ports);
    printf("  nr_of_ul_ports: %u\n", msg->nr_of_ul_ports);
    printf("  urllc_capable: %u\n", msg->urllc_capable);
    printf("  urllc_mini_slot_mask: %u\n", msg->urllc_mini_slot_mask);
    printf("  ssb_subc_spacing: %u\n", msg->ssb_subc_spacing);
    printf("  use_vendor_EpreXSSB: %u\n", msg->use_vendor_EpreXSSB);
}

void print_fapi_start_req_vendor_msg_t(const fapi_start_req_vendor_msg_t *msg) {
    printf("fapi_start_req_vendor_msg_t:\n");
    printf("  sfn: %u\n", msg->sfn);
    printf("  slot: %u\n", msg->slot);
    printf("  mode: %u\n", msg->mode);
#ifdef DEBUG_MODE
    printf("  count: %u\n", msg->count);
    printf("  period: %u\n", msg->period);
#endif
}

void print_fapi_stop_req_vendor_msg_t(const fapi_stop_req_vendor_msg_t *msg) {
    printf("fapi_stop_req_vendor_msg_t:\n");
    printf("  sfn: %u\n", msg->sfn);
    printf("  slot: %u\n", msg->slot);
}

void print_fapi_vendor_ul_pusch_pdu_t(const fapi_vendor_ul_pusch_pdu_t *pdu) {
    printf("fapi_vendor_ul_pusch_pdu_t:\n");
    printf("  nr_of_antenna_ports: %u\n", pdu->nr_of_antenna_ports);
    printf("  nr_of_rx_ru: %u\n", pdu->nr_of_rx_ru);
    for (int i = 0; i < FAPI_VENDOR_MAX_RXRU_NUM; i++) {
        printf("  rx_ru_idx[%d]: %u\n", i, pdu->rx_ru_idx[i]);
    }
}

void print_fapi_vendor_ul_pucch_pdu_t(const fapi_vendor_ul_pucch_pdu_t *pdu) {
    printf("fapi_vendor_ul_pucch_pdu_t:\n");
    printf("  nr_of_rx_ru: %u\n", pdu->nr_of_rx_ru);
    printf("  group_id: %u\n", pdu->group_id);
    for (int i = 0; i < FAPI_VENDOR_MAX_RXRU_NUM; i++) {
        printf("  rx_ru_idx[%d]: %u\n", i, pdu->rx_ru_idx[i]);
    }
}

void print_fapi_vendor_ul_srs_pdu_t(const fapi_vendor_ul_srs_pdu_t *pdu) {
    printf("fapi_vendor_ul_srs_pdu_t:\n");
    printf("  nr_of_rx_ru: %u\n", pdu->nr_of_rx_ru);
    for (int i = 0; i < FAPI_VENDOR_MAX_RXRU_NUM; i++) {
        printf("  rx_ru_idx[%d]: %u\n", i, pdu->rx_ru_idx[i]);
    }
}

void print_fapi_vendor_ul_tti_req_pdu_t(const fapi_vendor_ul_tti_req_pdu_t *pdu) {
    printf("fapi_vendor_ul_tti_req_pdu_t:\n");
    printf("  pdu_type: %u\n", pdu->pdu_type);
    switch (pdu->pdu_type) {
        case 0:  // Assuming 0 is for PUSCH PDU
            print_fapi_vendor_ul_pusch_pdu_t(&pdu->pusch_pdu);
            break;
        case 1:  // Assuming 1 is for PUCCH PDU
            print_fapi_vendor_ul_pucch_pdu_t(&pdu->pucch_pdu);
            break;
        case 2:  // Assuming 2 is for SRS PDU
            print_fapi_vendor_ul_srs_pdu_t(&pdu->srs_pdu);
            break;
        default:
            printf("Unknown PDU type\n");
    }
}

void print_fapi_vendor_ul_tti_req_t(const fapi_vendor_ul_tti_req_t *req) {
    printf("fapi_vendor_ul_tti_req_t:\n");
    for (int i = 0; i < req->num_ul_pdu; i++) {
        print_fapi_vendor_ul_tti_req_pdu_t(&req->ul_pdus[i]);
    }
    printf("  num_ul_pdu: %u\n", req->num_ul_pdu);
    printf("  sym: %u\n", req->sym);
}

void print_fapi_vendor_dl_dci_t(const fapi_vendor_dl_dci_t *dci) {
    printf("fapi_vendor_dl_dci_t:\n");
    printf("  epre_ratio_of_pdcch_to_ssb: %u\n", dci->epre_ratio_of_pdcch_to_ssb);
    printf("  epre_ratio_of_dmrs_to_ssb: %u\n", dci->epre_ratio_of_dmrs_to_ssb);
}
void print_fapi_vendor_dl_pdsch_pdu_t(const fapi_vendor_dl_pdsch_pdu_t *pdsch) {
    printf("fapi_vendor_dl_pdsch_pdu_t:\n");
    printf("  nr_of_rx_ru: %u\n", pdsch->nr_of_rx_ru);
    for (int i = 0; i < FAPI_VENDOR_MAX_RXRU_NUM; i++) {
        printf("  rx_ru_idx[%d]: %u\n", i, pdsch->rx_ru_idx[i]);
    }
    printf("  number_of_re_tx: %u\n", pdsch->number_of_re_tx);
}

void print_fapi_vendor_dl_pbch_pdu_t(const fapi_vendor_dl_pbch_pdu_t *pbch) {
    printf("fapi_vendor_dl_pbch_pdu_t:\n");
    printf("  nr_of_rx_ru: %u\n", pbch->nr_of_rx_ru);
    for (int i = 0; i < FAPI_VENDOR_MAX_RXRU_NUM; i++) {
        printf("  rx_ru_idx[%d]: %u\n", i, pbch->rx_ru_idx[i]);
    }
    printf("  pbch_payload_size: %u\n", pbch->pbch_payload_size);
}

void print_fapi_vendor_pdu_request_t(const fapi_vendor_pdu_request_t *request) {
    printf("fapi_vendor_pdu_request_t:\n");
    printf("  num_ul_pdu: %u\n", request->num_ul_pdu);
    printf("  num_dl_pdu: %u\n", request->num_dl_pdu);
    for (int i = 0; i < request->num_ul_pdu; i++) {
        print_fapi_vendor_ul_tti_req_pdu_t(&request->ul_pdus[i]);
    }
    for (int i = 0; i < request->num_dl_pdu; i++) {
        print_fapi_vendor_dl_tti_req_pdu_t(&request->dl_pdus[i]);
    }
}

void print_fapi_vendor_message_t(const fapi_vendor_message_t *msg) {
    printf("fapi_vendor_message_t:\n");
    printf("  message_type: %u\n", msg->message_type);
    switch (msg->message_type) {
        case FAPI_VENDOR_MSG_UL_TTI_REQ:
            print_fapi_vendor_ul_tti_req_t(&msg->ul_tti_req);
            break;
        case FAPI_VENDOR_MSG_DL_TTI_REQ:
            print_fapi_vendor_dl_tti_req_t(&msg->dl_tti_req);
            break;
        case FAPI_VENDOR_MSG_CONFIG_REQ:
            print_fapi_config_req_vendor_msg_t(&msg->config_req);
            break;
        case FAPI_VENDOR_MSG_START_REQ:
            print_fapi_start_req_vendor_msg_t(&msg->start_req);
            break;
        case FAPI_VENDOR_MSG_STOP_REQ:
            print_fapi_stop_req_vendor_msg_t(&msg->stop_req);
            break;
        default:
            printf("Unknown message type\n");
    }
}
