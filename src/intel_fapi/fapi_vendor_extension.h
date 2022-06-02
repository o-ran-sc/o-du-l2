/******************************************************************************
*
*   Copyright (c) 2019 Intel.
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
*******************************************************************************/

#ifndef _FAPI_VENDOR_EXTENSION_H_
#define _FAPI_VENDOR_EXTENSION_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

#include "fapi_interface.h"

#define FAPI_VENDOR_MESSAGE                                 0x10
#define FAPI_VENDOR_EXT_SHUTDOWN_REQUEST                    0x11
#define FAPI_VENDOR_EXT_SHUTDOWN_RESPONSE                   0x12

#ifdef DEBUG_MODE
#define FAPI_VENDOR_EXT_DL_IQ_SAMPLES                       0x13
#define FAPI_VENDOR_EXT_UL_IQ_SAMPLES                       0x14
#define FAPI_VENDOR_EXT_START_RESPONSE                      0x15
#define FAPI_VENDOR_EXT_ADD_REMOVE_CORE                     0x16
#endif

#define FAPI_VENDOR_EXT_P7_IND                              0x17

/* ----- WLS Operation --- */
#define FAPI_VENDOR_MSG_HEADER_IND                          0x1A

// PDSCH Payload
#define FAPI_VENDOR_MSG_PHY_ZBC_BLOCK_REQ                   0x1B

#define MAX_SNR_COUNT                                       (255)
#define FAPI_VENDOR_MAX_RXRU_NUM                            16
#define FAPI_VENDOR_MAX_TXRU_NUM                            4
#define FAPI_VENDOR_MAX_SRS_PORT_PER_UE                     2
#define FAPI_VENDOR_MAX_NUM_ANT                             64

enum {
    USE_VENDOR_EPREXSSB = 1
};

// Linked list header present at the top of all messages
    typedef struct _fapi_api_queue_elem {
        struct _fapi_api_queue_elem *p_next;
        // p_tx_data_elm_list used for TX_DATA.request processing
        struct _fapi_api_queue_elem *p_tx_data_elm_list;
        uint8_t msg_type;
        uint8_t num_message_in_block;
        uint32_t msg_len;
        uint32_t align_offset;
        uint64_t time_stamp;
    } fapi_api_queue_elem_t,
    *p_fapi_api_queue_elem_t;
/* ----------------------- */

#ifdef DEBUG_MODE
#define FAPI_MAX_IQ_SAMPLE_FILE_SIZE                        576
#define FAPI_MAX_IQ_SAMPLE_DL_PORTS                          16
#define FAPI_MAX_IQ_SAMPLE_UL_PORTS                           2
#define FAPI_MAX_IQ_SAMPLE_UL_VIRTUAL_PORTS                  16
#define FAPI_MAX_IQ_SAMPLE_UL_ANTENNA                        64
#define FAPI_MAX_IQ_SAMPLE_BUFFER_SIZE                     4096

#define FAPI_MAX_NUM_SET_CORE_MASK ( 4 )
#define FAPI_MAX_MASK_OPTIONS      ( 4 )
#define FAPI_NUM_SPLIT_OPTIONS     ( 22 )
#endif

    typedef struct {
        uint16_t hopping_id;
        uint8_t carrier_aggregation_level;
        uint8_t group_hop_flag;
        uint8_t sequence_hop_flag;
        // uint8_t                     nDMRS_type_A_pos;
        uint8_t prach_nr_of_rx_ru;
        uint8_t nr_of_dl_ports;
        uint8_t nr_of_ul_ports;
        uint16_t urllc_capable;
        uint16_t urllc_mini_slot_mask;
        uint8_t ssb_subc_spacing;
        uint8_t use_vendor_EpreXSSB;  // values: USE_VENDOR_EPREXSSB - use; else don't use
        uint8_t pad[2];
    } fapi_config_req_vendor_msg_t;

    typedef struct {
        uint16_t sfn;
        uint16_t slot;
        uint32_t mode;
#ifdef DEBUG_MODE
        uint32_t count;
        uint32_t period;
#endif
    } fapi_start_req_vendor_msg_t;

    typedef struct {
        uint16_t sfn;
        uint16_t slot;
    } fapi_stop_req_vendor_msg_t;

// P7 vendor extensions
    typedef struct {
        uint8_t nr_of_antenna_ports;
        uint8_t nr_of_rx_ru;
        uint8_t pad[2];
        uint8_t rx_ru_idx[FAPI_VENDOR_MAX_RXRU_NUM];
        // open for extension for new fields from ULSCHPDUStruct
    } fapi_vendor_ul_pusch_pdu_t;

    typedef struct {
        uint8_t nr_of_rx_ru;
        uint8_t pad[1];
        uint16_t group_id;
        uint8_t rx_ru_idx[FAPI_VENDOR_MAX_RXRU_NUM];
        // open for extension for new fields from ULCCHUCIPDUStruct
    } fapi_vendor_ul_pucch_pdu_t;

    typedef struct {
        uint8_t nr_of_rx_ru;
        uint8_t pad[3];
        uint8_t rx_ru_idx[FAPI_VENDOR_MAX_RXRU_NUM];
        // open for extension for new fields from SRSPDUStruct
    } fapi_vendor_ul_srs_pdu_t;

    typedef struct {
        uint16_t pdu_type;
        uint16_t pad;
        union {
            fapi_vendor_ul_pusch_pdu_t pusch_pdu;
            fapi_vendor_ul_pucch_pdu_t pucch_pdu;
            fapi_vendor_ul_srs_pdu_t srs_pdu;
            // open for extension for prach vendor type (as in fapi_ul_tti_req_pdu_t)
        } pdu;
    } fapi_vendor_ul_tti_req_pdu_t;

    typedef struct {
        fapi_vendor_ul_tti_req_pdu_t ul_pdus[FAPI_MAX_NUMBER_UL_PDUS_PER_TTI];
        uint8_t num_ul_pdu;
        uint8_t pad[3];
        uint32_t sym;
        // open for extension for new fields from PULConfigRequestStruct
    } fapi_vendor_ul_tti_req_t;

    typedef struct {
        uint16_t epre_ratio_of_pdcch_to_ssb;
        uint16_t epre_ratio_of_dmrs_to_ssb;
        // open for extension for new fields from DCIPDUStruct
    } fapi_vendor_dl_dci_t;

    typedef struct {
        uint16_t num_dl_dci;
        uint8_t pad[2];
        fapi_vendor_dl_dci_t dl_dci[FAPI_MAX_NUMBER_DL_DCI];
    } fapi_vendor_dl_pdcch_pdu_t;

   typedef struct {
        uint16_t epre_ratio_of_pdsch_to_ssb;
        uint16_t epre_ratio_of_dmrs_to_ssb;
        uint8_t nr_of_antenna_ports;
        uint8_t pad[3];
        uint8_t tx_ru_idx[FAPI_VENDOR_MAX_TXRU_NUM];
        // open for extension for new fields from DLSCHPDUStruct
    } fapi_vendor_dl_pdsch_pdu_t;

    typedef struct {
        uint16_t epre_ratio_to_ssb;
        uint8_t pad[2];
    } fapi_vendor_csi_rs_pdu_t;

    typedef struct {
        uint16_t pdu_type;
        uint16_t pdu_size;
        union {
            fapi_vendor_dl_pdcch_pdu_t pdcch_pdu;
            fapi_vendor_dl_pdsch_pdu_t pdsch_pdu;
            fapi_vendor_csi_rs_pdu_t csi_rs_pdu;
            // open for extension for ssb vendor types (as in fapi_dl_tti_req_pdu_t)
        } pdu;
    } fapi_vendor_dl_tti_req_pdu_t;

    typedef struct {
        uint32_t sym;
        uint16_t lte_crs_carrier_freq_dl;
        uint8_t lte_crs_present;
        uint8_t lte_crs_carrier_bandwidth_dl;
        uint8_t lte_crs_nr_of_crs_ports;
        uint8_t lte_crs_v_shift;
        uint8_t pdcch_precoder_en;
        uint8_t ssb_precoder_en;
        uint8_t num_pdus;
        uint8_t pad[3];
        fapi_vendor_dl_tti_req_pdu_t pdus[FAPI_MAX_PDUS_PER_SLOT];
        // open for extension for new fields from DLConfigRequestStruct
    } fapi_vendor_dl_tti_req_t;

    typedef struct {
        uint16_t pdu_type;
        uint16_t pdu_size;
        fapi_vendor_dl_pdcch_pdu_t pdcch_pdu_config;
    } fapi_vendor_dci_pdu_t;

    typedef struct {
        uint32_t sym;
        uint8_t num_pdus;
        uint8_t pad[3];
        fapi_vendor_dci_pdu_t pdus[FAPI_MAX_NUMBER_UCI_PDUS_PER_SLOT];
        // open for extension for new fields from ULDCIRequestStruct
    } fapi_vendor_ul_dci_req_t;

    typedef struct {
        uint32_t sym;
        // open for extension for new fields from TXRequestStruct
    } fapi_vendor_tx_data_req_t;

    typedef struct {
        fapi_vendor_dl_tti_req_t dl_tti_req;
        fapi_vendor_ul_tti_req_t ul_tti_req;
        fapi_vendor_ul_dci_req_t ul_dci_req;
        fapi_vendor_tx_data_req_t tx_data_req;
    } fapi_vendor_p7_msg_t;

// P7 vendor extensions end

    typedef struct {
        fapi_msg_t header;
        fapi_config_req_vendor_msg_t config_req_vendor;
        fapi_start_req_vendor_msg_t start_req_vendor;
        fapi_stop_req_vendor_msg_t stop_req_vendor;
        fapi_vendor_p7_msg_t p7_req_vendor;
    } fapi_vendor_msg_t;

    typedef struct {
        fapi_msg_t header;
        uint16_t sfn;
        uint16_t slot;
        uint32_t test_type;
    } fapi_vendor_ext_shutdown_req_t;

    typedef struct {
        fapi_msg_t header;
        uint16_t sfn;
        uint16_t slot;
        uint32_t nStatus;
    } fapi_vendor_ext_shutdown_res_t;

    typedef struct {
        int16_t nSNR[MAX_SNR_COUNT];
        int16_t pad;
    } fapi_vendor_ext_snr_t;

    typedef struct {
        uint8_t nr_of_port;
        uint8_t nr_of_rx_ant;
        uint16_t nr_of_rbs;
        uint8_t is_chan_est_pres;
        uint8_t pad[3];
        int16_t *p_srs_chan_est[FAPI_VENDOR_MAX_SRS_PORT_PER_UE]
            [FAPI_VENDOR_MAX_NUM_ANT];
    } fapi_vendor_ext_srs_pdu_t;

    typedef struct {
        uint8_t num_pdus;
        uint8_t pad[3];
        fapi_vendor_ext_srs_pdu_t srs_pdus[FAPI_MAX_NUMBER_SRS_PDUS_PER_SLOT];
    } fapi_vendor_ext_srs_ind_t;

    typedef struct {
        uint32_t carrier_idx;
        uint32_t sym;
    } fapi_vendor_ext_slot_ind_t;

    typedef struct {
        uint32_t carrier_idx;
        uint32_t sym;
    } fapi_vendor_ext_rx_data_ind_t;

    typedef struct {
        uint32_t carrier_idx;
        uint32_t sym;
    } fapi_vendor_ext_crc_ind_t;

    typedef struct {
        uint32_t carrier_idx;
        uint32_t sym;
    } fapi_vendor_ext_uci_ind_t;

    typedef struct {
        fapi_msg_t header;
        fapi_vendor_ext_snr_t crc_snr;
        fapi_vendor_ext_snr_t uci_snr;
        fapi_vendor_ext_srs_ind_t srs_ind;
        fapi_vendor_ext_slot_ind_t slot_ind;
        fapi_vendor_ext_rx_data_ind_t rx_data_ind;        
        fapi_vendor_ext_crc_ind_t crc_ind;
        fapi_vendor_ext_uci_ind_t uci_ind;
    } fapi_vendor_p7_ind_msg_t;

#ifdef DEBUG_MODE
    typedef struct {
        uint32_t carrNum;
        uint32_t numSubframes;
        uint32_t nIsRadioMode;
        uint32_t timerModeFreqDomain;
        uint32_t phaseCompensationEnable;
        uint32_t startFrameNum;
        uint32_t startSlotNum;
        uint32_t startSymNum;
        char filename_in_ul_iq[FAPI_MAX_IQ_SAMPLE_UL_VIRTUAL_PORTS]
            [FAPI_MAX_IQ_SAMPLE_FILE_SIZE];
        char filename_in_ul_iq_compressed[FAPI_MAX_IQ_SAMPLE_UL_VIRTUAL_PORTS]
            [FAPI_MAX_IQ_SAMPLE_FILE_SIZE];
        char filename_in_prach_iq[FAPI_MAX_IQ_SAMPLE_UL_VIRTUAL_PORTS]
            [FAPI_MAX_IQ_SAMPLE_FILE_SIZE];
        char filename_in_srs_iq[FAPI_MAX_IQ_SAMPLE_UL_ANTENNA]
            [FAPI_MAX_IQ_SAMPLE_FILE_SIZE];
        char filename_out_dl_iq[FAPI_MAX_IQ_SAMPLE_FILE_SIZE];
        char filename_out_dl_beam[FAPI_MAX_IQ_SAMPLE_DL_PORTS]
            [FAPI_MAX_IQ_SAMPLE_FILE_SIZE];
        char filename_out_ul_beam[FAPI_MAX_IQ_SAMPLE_UL_VIRTUAL_PORTS]
            [FAPI_MAX_IQ_SAMPLE_FILE_SIZE];
        char filename_out_dl_iq_compressed[FAPI_MAX_IQ_SAMPLE_FILE_SIZE];

        /* DL Compression add */
        uint16_t nDLCompressionIdx;
        uint16_t nDLCompiqWidth;
        uint16_t nDLCompScaleFactor;
        uint16_t nDLCompreMask;

        /*nULDecompressionIdx, determine the UL Decompression method, Value:0->4*/
        /*0:NONE,  1:BLKFLOAT,  2:BLKSCALE,  3:ULAW,  4:MODULATION*/
        uint16_t nULDecompressionIdx;
        uint16_t nULDecompiqWidth;

        uint8_t buffer[FAPI_MAX_IQ_SAMPLE_BUFFER_SIZE];
    } fapi_vendor_ext_iq_samples_info_t;

    typedef struct {
        fapi_msg_t header;
        fapi_vendor_ext_iq_samples_info_t iq_samples_info;
    } fapi_vendor_ext_iq_samples_req_t;

    typedef struct {
        fapi_msg_t header;
    } fapi_vendor_ext_dl_iq_samples_res_t;

    typedef struct {
        fapi_msg_t header;
    } fapi_vendor_ext_ul_iq_samples_res_t;

    typedef struct {
        fapi_msg_t header;
    } fapi_vendor_ext_start_response_t;

    typedef struct {
        uint32_t eOption;
        uint64_t nCoreMask[FAPI_MAX_MASK_OPTIONS][FAPI_MAX_NUM_SET_CORE_MASK];
        uint32_t nMacOptions[FAPI_NUM_SPLIT_OPTIONS];
    } fapi_vendor_ext_add_remove_core_info_t;

    typedef struct {
        fapi_msg_t header;
        fapi_vendor_ext_add_remove_core_info_t add_remove_core_info;
    } fapi_vendor_ext_add_remove_core_msg_t;
#endif

#if defined(__cplusplus)
}
#endif
#endif                          // FAPI_VENDOR_EXTENSION_H_
