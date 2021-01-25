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

#define FAPI_VENDOR_MESSAGE                                 0x10
#define FAPI_VENDOR_EXT_SHUTDOWN_REQUEST                    0x11
#define FAPI_VENDOR_EXT_SHUTDOWN_RESPONSE                   0x12

#ifdef DEBUG_MODE
#define FAPI_VENDOR_EXT_DL_IQ_SAMPLES                       0x13
#define FAPI_VENDOR_EXT_UL_IQ_SAMPLES                       0x14
#define FAPI_VENDOR_EXT_START_RESPONSE                      0x15
#endif

/* ----- WLS Operation --- */
#define FAPI_VENDOR_MSG_HEADER_IND                          0x1A

// PDSCH Payload
#define FAPI_VENDOR_MSG_PHY_ZBC_BLOCK_REQ                   0x1B

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
#define FAPI_MAX_IQ_SAMPLE_UL_VIRTUAL_PORTS                   8
#define FAPI_MAX_IQ_SAMPLE_UL_ANTENNA                        64
#define FAPI_MAX_IQ_SAMPLE_BUFFER_SIZE                     4096
#endif

    typedef struct {
        uint16_t hopping_id;
        uint8_t carrier_aggregation_level;
        uint8_t group_hop_flag;
        uint8_t sequence_hop_flag;
        // uint8_t                     nDMRS_type_A_pos;
        uint8_t pad[3];
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

    typedef struct {
        fapi_msg_t header;
        fapi_config_req_vendor_msg_t config_req_vendor;
        fapi_start_req_vendor_msg_t start_req_vendor;
        fapi_stop_req_vendor_msg_t stop_req_vendor;
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
        char filename_in_prach_iq[FAPI_MAX_IQ_SAMPLE_UL_VIRTUAL_PORTS]
            [FAPI_MAX_IQ_SAMPLE_FILE_SIZE];
        char filename_in_srs_iq[FAPI_MAX_IQ_SAMPLE_UL_ANTENNA]
            [FAPI_MAX_IQ_SAMPLE_FILE_SIZE];
        char filename_out_dl_iq[FAPI_MAX_IQ_SAMPLE_FILE_SIZE];
        char filename_out_dl_beam[FAPI_MAX_IQ_SAMPLE_DL_PORTS]
            [FAPI_MAX_IQ_SAMPLE_FILE_SIZE];
        char filename_out_ul_beam[FAPI_MAX_IQ_SAMPLE_UL_VIRTUAL_PORTS]
            [FAPI_MAX_IQ_SAMPLE_FILE_SIZE];
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
#endif

#if defined(__cplusplus)
}
#endif
#endif                          // FAPI_VENDOR_EXTENSION_H_
