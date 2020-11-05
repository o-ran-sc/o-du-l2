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

#ifndef _FAPI_INTERFACE_H_
#define _FAPI_INTERFACE_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include "stdint.h"

#define FAPI_PARAM_REQUEST									0x00
#define FAPI_PARAM_RESPONSE									0x01
#define FAPI_CONFIG_REQUEST									0x02
#define FAPI_CONFIG_RESPONSE								0x03
#define FAPI_START_REQUEST									0x04
#define FAPI_STOP_REQUEST									0x05
#define FAPI_STOP_INDICATION								0x06
#define FAPI_ERROR_INDICATION                               0x07
// Reserved 0x08 - 0x0F and 0x02 - 0x7f
// 0x01 - 0x1F is used in vendor extension

#define FAPI_DL_TTI_REQUEST		    						0x80
#define FAPI_UL_TTI_REQUEST 								0x81
#define FAPI_SLOT_INDICATION								0x82
#define FAPI_UL_DCI_REQUEST						    		0x83
#define FAPI_TX_DATA_REQUEST    							0x84
#define FAPI_RX_DATA_INDICATION								0x85
#define FAPI_CRC_INDICATION 								0x86
#define FAPI_UCI_INDICATION     							0x87
#define FAPI_SRS_INDICATION 								0x88
#define FAPI_RACH_INDICATION								0x89
// Reserved 0x8a -0xff

// Tags per 5G FAPI
// Cell Parameters
#define FAPI_RELEASE_CAPABILITY_TAG			    			0x0001
#define FAPI_PHY_STATE_TAG				                    0x0002
#define FAPI_SKIP_BLANK_DL_CONFIG_TAG						0x0003
#define FAPI_SKIP_BLANK_UL_CONFIG_TAG				        0x0004
#define FAPI_NUM_CONFIG_TLVS_TO_REPORT_TYPE_TAG				0x0005
#define FAPI_CYCLIC_PREFIX_TAG                              0x0006
// PDCCH Parameters
#define FAPI_SUPPORTED_SUBCARRIER_SPACING_DL_TAG		    0x0007
#define FAPI_SUPPORTED_BANDWIDTH_DL_TAG					    0x0008
#define FAPI_SUPPORTED_SUBCARRIER_SPACING_UL_TAG			0x0009
#define FAPI_SUPPORTED_BANDWIDTH_UL_TAG						0x000A
#define FAPI_CCE_MAPPING_TYPE_TAG   						0x000B
#define FAPI_CORESET_OUTSIDE_FIRST_3_OFDM_SYMS_OF_SLOT_TAG	0x000c
#define FAPI_PRECODER_GRANULARITY_CORESET_TAG				0x000d
#define FAPI_PDCCH_MU_MIMO_TAG  							0x000e
#define FAPI_PDCCH_PRECODER_CYCLING_TAG					    0x000f
#define FAPI_MAX_PDCCHS_PER_SLOT_TAG		        		0x0010
// PUCCH Parameters
#define FAPI_PUCCH_FORMATS_TAG						        0x0011
#define FAPI_MAX_PUCCHS_PER_SLOT_TAG					    0x0012
// PDSCH Parameters
#define FAPI_PDSCH_MAPPING_TYPE_TAG     					0x0013
#define FAPI_PDSCH_ALLOCATION_TYPES_TAG                 	0x0014
#define FAPI_PDSCH_VRB_TO_PRB_MAPPING_TAG					0x0015
#define FAPI_PDSCH_CBG_TAG						            0x0016
#define FAPI_PDSCH_DMRS_CONFIG_TYPES_TAG					0x0017
#define FAPI_PDSCH_DMRS_MAX_LENGTH_TAG						0x0018
#define FAPI_PDSCH_DMRS_ADDITIONAL_POS_TAG					0x0019
#define FAPI_MAX_PDSCHS_TBS_PER_SLOT_TAG					0x001a
#define FAPI_MAX_NUMBER_MIMO_LAYERS_PDSCH_TAG				0x001b
#define FAPI_SUPPORTED_MAX_MODULATION_ORDER_DL_TAG		    0x001c
#define FAPI_MAX_MU_MIMO_USERS_DL_TAG						0x001d
#define FAPI_PDSCH_DATA_IN_DMRS_SYMBOLS_TAG				    0x001e
#define FAPI_PREMPTIONSUPPORT_TAG							0x001f
#define FAPI_PDSCH_NON_SLOT_SUPPORT_TAG					    0x0020
// PUSCH Parameters
#define FAPI_UCI_MUX_ULSCH_IN_PUSCH_TAG			            0x0021
#define FAPI_UCI_ONLY_PUSCH_TAG				                0x0022
#define FAPI_PUSCH_FREQUENCY_HOPPING_TAG				    0x0023
#define FAPI_PUSCH_DMRS_CONFIG_TYPES_TAG			        0x0024
#define FAPI_PUSCH_DMRS_MAX_LEN_TAG     					0x0025
#define FAPI_PUSCH_DMRS_ADDITIONAL_POS_TAG				    0x0026
#define FAPI_PUSCH_CBG_TAG                                  0x0027
#define FAPI_PUSCH_MAPPING_TYPE_TAG                         0x0028
#define FAPI_PUSCH_ALLOCATION_TYPES_TAG                     0x0029
#define FAPI_PUSCH_VRB_TO_PRB_MAPPING_TAG                   0x002a
#define FAPI_PUSCH_MAX_PTRS_PORTS_TAG                       0x002b
#define FAPI_MAX_PDUSCHS_TBS_PER_SLOT_TAG                   0x002c
#define FAPI_MAX_NUMBER_MIMO_LAYERS_NON_CB_PUSCH_TAG        0x002d
#define FAPI_SUPPORTED_MODULATION_ORDER_UL_TAG              0x002e
#define FAPI_MAX_MU_MIMO_USERS_UL_TAG                       0x002f
#define FAPI_DFTS_OFDM_SUPPORT_TAG                          0x0030
#define FAPI_PUSCH_AGGREGATION_FACTOR_TAG                   0x0031
// PRACH Parameters
#define FAPI_PRACH_LONG_FORMATS_TAG                         0x0032
#define FAPI_PRACH_SHORT_FORMATS_TAG                        0x0033
#define FAPI_PRACH_RESTRICTED_SETS_TAG                      0x0034
#define FAPI_MAX_PRACH_FD_OCCASIONS_IN_A_SLOT_TAG           0x0035
// Measurement Parameters
#define FAPI_RSSI_MEASUREMENT_SUPPORT_TAG                   0x0036

// CONFIG TLV TAGS per 5G FAPI
// Carrier Configuration
#define FAPI_DL_BANDWIDTH_TAG                               0x1001
#define FAPI_DL_FREQUENCY_TAG                               0x1002
#define FAPI_DL_K0_TAG                                      0x1003
#define FAPI_DL_GRIDSIZE_TAG                                0x1004
#define FAPI_NUM_TX_ANT_TAG                                 0x1005
#define FAPI_UPLINK_BANDWIDTH_TAG                           0x1006
#define FAPI_UPLINK_FREQUENCY_TAG                           0x1007
#define FAPI_UL_K0_TAG                                      0x1008
#define FAPI_UL_GRID_SIZE_TAG                               0x1009
#define FAPI_NUM_RX_ANT_TAG                                 0x100a
#define FAPI_FREQUENCY_SHIFT_7P5_KHZ_TAG                    0x100b
// Cell Configuration
#define FAPI_PHY_CELL_ID_TAG                                0x100c
#define FAPI_FRAME_DUPLEX_TYPE_TAG                          0x100d
// SSB Configuration
#define FAPI_SS_PBCH_POWER_TAG                              0x100e
#define FAPI_BCH_PAYLOAD_TAG                                0x100f
#define FAPI_SCS_COMMON_TAG                                 0x1010
// PRACH Configuration
#define FAPI_PRACH_SEQUENCE_LENGTH_TAG                      0x1011
#define FAPI_PRACH_SUBC_SPACING_TAG                         0x1012
#define FAPI_RESTRICTED_SET_CONFIG_TAG                      0x1013
#define FAPI_NUM_PRACH_FD_OCCASIONS_TAG                     0x1014
#define FAPI_PRACH_ROOT_SEQUENCE_INDEX_TAG                  0x1015
#define FAPI_NUM_ROOT_SEQUENCES_TAG                         0x1016
#define FAPI_K1_TAG                                         0x1017
#define FAPI_PRACH_ZERO_CORR_CONF_TAG                       0x1018
#define FAPI_NUM_UNUSED_ROOT_SEQUENCES_TAG                  0x1019
#define FAPI_UNUSED_ROOT_SEQUENCES_TAG                      0x101a
#define FAPI_SSB_PER_RACH_TAG                               0x101b
#define FAPI_PRACH_MULTIPLE_CARRIERS_IN_A_BAND_TAG          0x101c
// SSB Table
#define FAPI_SSB_OFFSET_POINT_A_TAG                         0x101d
#define FAPI_BETA_PSS_TAG                                   0x101e
#define FAPI_SSB_PERIOD_TAG                                 0x101f
#define FAPI_SSB_SUBCARRIER_OFFSET_TAG                      0x1020
#define FAPI_MIB_TAG                                        0x1021
#define FAPI_SSB_MASK_TAG                                   0x1022
#define FAPI_BEAM_ID_TAG                                    0x1023
#define FAPI_SS_PBCH_MULTIPLE_CARRIERS_IN_A_BAND_TAG        0x1024
#define FAPI_MULTIPLE_CELLS_SS_PBCH_IN_A_CARRIER_TAG        0x1025
// TDD Table
#define FAPI_TDD_PERIOD_TAG                                 0x1026
#define FAPI_SLOT_CONFIG_TAG                                0x1027
// Measurement Configuration
#define FAPI_RSSI_MEASUREMENT_TAG                           0x1028
// Prach Configuration Index
#define FAPI_PRACH_CONFIG_INDEX_TAG							0x1029
// DMRS-TypeA Pos
#define FAPI_DMRS_TYPE_A_POS_TAG							0x102A

// Error Codes updated per 5G FAPI Table 3-31
#define FAPI_MSG_OK											0x0
#define FAPI_MSG_INVALID_STATE								0x1
#define FAPI_MSG_INVALID_CONFIG								0x2
#define FAPI_MSG_SFN_OUT_OF_SYNC							0x3
#define FAPI_MSG_SLOT_ERR   								0x4
#define FAPI_MSG_BCH_MISSING								0x5
#define FAPI_MSG_INVALID_SFN								0x6
#define FAPI_MSG_UL_DCI_ERR									0x7
#define FAPI_MSG_TX_ERR										0x8

// 5G FAPI Definitions
#define FAPI_NUMEROLOGIES                                   5
#define FAPI_MAX_NUM_UNUSED_ROOT_SEQUENCES                  63  // 38.331 page 383
#define FAPI_MAX_NUM_PRACH_FD_OCCASIONS                     64  // 38.331 page 383
#define FAPI_MAX_NUM_OF_SYMBOLS_PER_SLOT                    14
#define FAPI_MAX_TDD_PERIODICITY                            160 // 38.212 11.1 for u=4 and P=10 ms
// m=p*q with p number of panels and q number of TxRU/RxRU per panel,
// depends on the RF configuration, currently n=m=4, q=1, p=4 and k=21
// (number of beams per pannel). n number of antenna ports
#define FAPI_MAX_NUMBER_TX_RUS                              4
#define FAPI_MAX_NUMBER_OF_BEAMS                            64  // Intel API Page 27
#define FAPI_MAX_NUM_ANT_PORTS                              8   // Based on current RF
#define FAPI_MAX_NUM_LAYERS                                 8   // 38.211 Table 7.3.1.3-1
#define FAPI_MAX_NUM_TLVS_CELL_PARMS	                    1505    // 5G FAPI Table 3-9  (A)
#define FAPI_MAX_NUM_TLVS_CARRIER_PARMS				        27  // 5G FAPI Table 3-10 (B)
#define FAPI_MAX_NUM_TLVS_PDCCH_PARMS	                    6   // 5G FAPI Table 3-11 (C)
#define FAPI_MAX_NUM_TLVS_PUCCH_PARMS	                    2   // 5G FAPI Table 3-12 (D)
#define FAPI_MAX_NUM_TLVS_PDSCH_PARMS	                    14  // 5G FAPI Table 3-13 (E)
#define FAPI_MAX_NUM_TLVS_PUSCH_PARMS	                    17  // 5G FAPI Table 3-14 (F)
#define FAPI_MAX_NUM_TLVS_PRACH_PARMS	                    4   // 5G FAPI Table 3-15 (G)
#define FAPI_MAX_NUM_TLVS_MEAS_PARMS	                    1   // 5G FAPI Table 3-16 (H)
#define FAPI_MAX_NUM_TLVS_PARAMS                            1576    //  A+B+C+D+E+F+G+H + Padding
// Carrier Table 11 + Cell Config 2 + SSB Config 3 + PRACH Config 414 + SSB
// Table 71 + TDD Table  141 + Measurements 1
#define FAPI_MAX_NUM_TLVS_CONFIG							1500

#define FAPI_UL_DCI_REQUEST_PDCCH_PDU	        			0

#define FAPI_MAX_NUMBER_UNSUPPORTED_TLVS                    74
#define FAPI_MAX_NUMBER_OF_INVALID_IDLE_ONLY_TLVS           74
#define FAPI_MAX_NUMBER_OF_INVALID_RUNNING_ONLY_TLVS        74
#define FAPI_MAX_NUMBER_OF_MISSING_TLVS                     74
#define FAPI_MAX_NUM_DIGBFINTERFACES                        4   // Based on RF, 5G FAPI says {0,255}
#define FAPI_MAX_NUM_PRGS_PER_TTI                           4   // Based on 38.214 5.1.2.3
#define FAPI_DCI_PAYLOAD_BYTE_LEN                           32  // Based on Intel API MAX_DCI_BIT_BYTE_LEN
#define FAPI_MAX_NUMBER_DL_DCI                              1   // Based on Intel API MAX_NUM_PDCCH
#define FAPI_MAX_NUMBER_OF_CODEWORDS_PER_PDU                2   // Based on MAX_DL_CODEWORD
// Based on (MAX_NUM_PDSCH*MAX_DL_CODEWORD + MAX_NUM_PDCCH + MAX_NUM_SRS +
// 1 PBCH/SLOT)
#define FAPI_MAX_NUMBER_DL_PDUS_PER_TTI                     129

#define FAPI_MAX_NUMBER_OF_UES_PER_TTI                      16  // Per common_ran_parameters.h
// Based on Max Tb size of 1376264 bits + 24 crc over (8848-24) and O/H
#define FAPI_MAX_NUM_CB_PER_TTI_IN_BYTES                    192

#define FAPI_MAX_NUM_PTRS_PORTS                             2   // Per 3GPP 38.212 Table 7.3.1.1.2-21
#define FAPI_MAX_NUMBER_OF_GROUPS_PER_TTI                   16  // FlexRAN API Table 33
#define FAPI_MAX_NUMBER_UL_PDUS_PER_TTI                     328 // (MAX_NUM_PUSCH+MAX_NUM_PUCCH+MAX_NUM_SRS+MAX_NUM_PRACH_DET)
#define FAPI_MAX_NUMBER_DCI_PDUS_PER_TTI                    32  // Based on MAX_NUM_PDCCH
#define FAPI_MAX_NUMBER_OF_TLVS_PER_PDU                     1   // one for each codeword
#define FAPI_MAX_NUMBER_TX_PDUS_PER_TTI                     129 // Same as FAPI_MAX_NUMBER_DL_PDUS_PER_TTI
// Based on 38.214 5.1.3.4, the TBS is 1376264 bits and divided by 8 and
// aligned to 64 bytes
#define FAPI_MAX_PDU_LENGTH                                 172096
#define FAPI_TX_DATA_PAYLOAD                                0x00
#define FAPI_TX_DATA_PTR_TO_PAYLOAD_32                      0x01
#define FAPI_TX_DATA_OFFSET_TO_PAYLOAD                      0x02
#define FAPI_TX_DATA_PTR_TO_PAYLOAD_64                      0x03

#define FAPI_MAX_NUMBER_OF_PDUS_PER_TTI                     129 // Same as FAPI_MAX_NUMBER_DL_PDUS_PER_TTI
#define FAPI_MAX_NUMBER_OF_ULSCH_PDUS_PER_SLOT              64  // NUM_PUSCH_CHAN*FAPI_MAX_NUMBER_OF_CODEWORDS_PER_PDU
#define FAPI_MAX_NUMBER_OF_CRCS_PER_SLOT                    32  // Based on MAX_NUM_UL_CHAN
#define FAPI_MAX_HARQ_INFO_LEN_BYTES                        214 // Based on 5G FAPI Table 3-70
#define FAPI_MAX_CSI_PART1_DATA_BYTES                       214 // Based on 5G FAPI Table 3-71
#define FAPI_MAX_CSI_PART2_DATA_BYTES                       214 // Based on 5G FAPI Table 3-72
#define FAPI_MAX_NUMBER_OF_HARQS_PER_IND                    2   // Based on 5G FAPI Table 3-68
#define FAPI_MAX_SR_PAYLOAD_SIZE                            1   // Based on 5G FAPI Table 3-69
#define FAPI_MAX_HARQ_PAYLOAD_SIZE                          214 // Based on 5G FAPI Table 3-70
#define FAPI_MAX_NUMBER_UCI_PDUS_PER_SLOT                   200 // Based on MAX_NUM_PUCCH
#define FAPI_MAX_NUMBER_RBS                                 273 // Based on MAX_NUM_OF_PRB_IN_FULL_BAND
#define FAPI_MAX_NUMBER_OF_REP_SYMBOLS                      4   // Based on 5g FAPI Table 3-73
#define FAPI_MAX_NUMBER_SRS_PDUS_PER_SLOT                   32  // Based on MAX_NUM_SRS
#define FAPI_MAX_NUM_PREAMBLES_PER_SLOT                     64  // Based on MAX_NUM_PRACH_DET
#define FAPI_MAX_NUMBER_RACH_PDUS_PER_SLOT                  64  // Based on MAX_NUM_PRACH_DET
#define FAPI_MAX_PDUS_PER_SLOT								64
#define FAPI_MAX_NUM_TLVS_START                             3   // Based on Timer Mode requirement.
#define FAPI_MAX_NUM_TLVS_SHUTDOWN                          1   // Based on Timer Mode requirement.
#define FAPI_MAX_UCI_BIT_BYTE_LEN                           256

    enum ul_tti_pdu_type_e {
        FAPI_PRACH_PDU_TYPE = 0,
        FAPI_PUSCH_PDU_TYPE,
        FAPI_PUCCH_PDU_TYPE,
        FAPI_SRS_PDU_TYPE,
        FAPI_UL_TTI_PDU_TYPE_MAX
    };

    enum dl_tti_pdu_type_e {
        FAPI_PDCCH_PDU_TYPE = 0,
        FAPI_PDSCH_PDU_TYPE,
        FAPI_CSIRS_PDU_TYPE,
        FAPI_PBCH_PDU_TYPE,
        FAPI_DL_TTI_PDU_TYPE_MAX
    };

//------------------------------------------------------------------------------------------------------------
// Updated per 5G FAPI
    typedef struct {
        uint8_t num_msg;
        // Can be used for Phy Id or Carrier Id  5G FAPI Table 3-2
        uint8_t handle;
        uint8_t pad[2];
    } fapi_msg_header_t,
    *p_fapi_msg_header_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t msg_id;
        uint16_t pad;
        uint32_t length;        // Length of the message body in bytes  5G FAPI Table 3-3
    } fapi_msg_t;
// Updated per 5G FAPI
    typedef struct {
        uint16_t tag;
        uint16_t length;        // 5G FAPI Table 3-7 Fixed part
    } fapi_tl_t;
// Updated per 5G FAPI
    typedef struct {
        fapi_tl_t tl;
        uint8_t value;          // TLV with byte value
        uint8_t rsv[3];         // Per 5g FAPI 3.3.1.4 the lenght of the value parameter must be 32 bits
    } fapi_uint8_tlv_t;
    typedef struct {
        fapi_tl_t tl;
        uint8_t *value;         // TLV with unsigned 32 bit value
    } fapi_uint8_ptr_tlv_t;
// Updated per 5G FAPI
    typedef struct {
        fapi_tl_t tl;
        uint16_t value;         // TLV with unsigned 16 bit value
        uint8_t rsv[2];         // Per 5g FAPI 3.3.1.4 the lenght of the value parameter must be 32 bits
    } fapi_uint16_tlv_t;
// Updated per 5G FAPI
    typedef struct {
        fapi_tl_t tl;
        int16_t value;          // TLV with signed 16 bit value
        uint8_t rsv[2];         // Per 5g FAPI 3.3.1.4 the lenght of the value parameter must be 32 bits
    } fapi_int16_tlv_t;
// Updated per 5G FAPI
    typedef struct {
        fapi_tl_t tl;
        uint32_t value;         // TLV with unsigned 32 bit value
    } fapi_uint32_tlv_t;
// Updated per 5G FAPI
    typedef struct {
        uint16_t tag;           // In 5G FAPI for Cell Params inside Table 3-9 under NumConfigTLVsToReport Loop
        uint8_t length;
        uint8_t value;
    } fapi_config_tlv_t;

    typedef struct {
        fapi_tl_t tl;
        uint16_t value[FAPI_NUMEROLOGIES];
        uint16_t rsv;           // To be 32-bit aligned, if FAPI_NUMEROLOGIES changes to some other value than 5 please ensure 32 bit alignment
    } fapi_config_num_tlv_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_msg_t header;      // For PARAM.req message length in fapi_msg_t is zero per 5G FAPI 3.3.1.1
    } fapi_param_req_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_uint16_tlv_t releaseCapability;
        fapi_uint16_tlv_t phyState;
        fapi_uint8_tlv_t skipBlankDlConfig;
        fapi_uint8_tlv_t skipBlankUlConfig;
        fapi_uint16_tlv_t numTlvsToReport;
        fapi_config_tlv_t tlvStatus[FAPI_MAX_NUM_TLVS_CONFIG];  // 5G FAPI Table 3-9
    } fapi_cell_parms_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_uint8_tlv_t cyclicPrefix;
        fapi_uint8_tlv_t supportedSubcarrierSpacingDl;
        fapi_uint16_tlv_t supportedBandwidthDl;
        fapi_uint8_tlv_t supportedSubcarrierSpecingsUl;
        fapi_uint16_tlv_t supportedBandwidthUl; // 5G FAPI Table 3-10
    } fapi_carrier_parms_t;

// Updated per 5G FAPI    
    typedef struct {
        fapi_uint8_tlv_t cceMappingType;
        fapi_uint8_tlv_t coresetOutsideFirst3OfdmSymsOfSlot;
        fapi_uint8_tlv_t precoderGranularityCoreset;
        fapi_uint8_tlv_t pdcchMuMimo;
        fapi_uint8_tlv_t pdcchPrecoderCycling;
        fapi_uint8_tlv_t maxPdcchsPerSlot;  // 5G FAPI Table 3-11
    } fapi_pdcch_parms_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_uint8_tlv_t pucchFormats;
        fapi_uint8_tlv_t maxPucchsPerSlot;  // 5G FAPI Table 3-12
    } fapi_pucch_parms_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_uint8_tlv_t pdschMappingType;
        fapi_uint8_tlv_t pdschAllocationTypes;
        fapi_uint8_tlv_t pdschVrbToPrbMapping;
        fapi_uint8_tlv_t pdschCbg;
        fapi_uint8_tlv_t pdschDmrsConfigTypes;
        fapi_uint8_tlv_t pdschDmrsMaxLength;
        fapi_uint8_tlv_t pdschDmrsAdditionalPos;
        fapi_uint8_tlv_t maxPdschsTBsPerSlot;
        fapi_uint8_tlv_t maxNumberMimoLayersPdsch;
        fapi_uint8_tlv_t supportedMaxModulationOrderDl;
        fapi_uint8_tlv_t maxMuMimoUsersDl;
        fapi_uint8_tlv_t pdschDataInDmrsSymbols;
        fapi_uint8_tlv_t premptionSupport;
        fapi_uint8_tlv_t pdschNonSlotSupport;   // 5G FAPI Table 3-13
    } fapi_pdsch_parms_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_uint8_tlv_t uciMuxUlschInPusch;
        fapi_uint8_tlv_t uciOnlyPusch;
        fapi_uint8_tlv_t puschFrequencyHopping;
        fapi_uint8_tlv_t puschDmrsConfigTypes;
        fapi_uint8_tlv_t puschDmrsMaxLen;
        fapi_uint8_tlv_t puschDmrsAdditionalPos;
        fapi_uint8_tlv_t puschCbg;
        fapi_uint8_tlv_t puschMappingType;
        fapi_uint8_tlv_t puschAllocationTypes;
        fapi_uint8_tlv_t puschVrbToPrbMapping;
        fapi_uint8_tlv_t puschMaxPtrsPorts;
        fapi_uint8_tlv_t maxPduschsTBsPerSlot;
        fapi_uint8_tlv_t maxNumberMimoLayersNonCbPusch;
        fapi_uint8_tlv_t supportedModulationOrderUl;
        fapi_uint8_tlv_t maxMuMimoUsersUl;
        fapi_uint8_tlv_t dftsOfdmSupport;
        fapi_uint8_tlv_t puschAggregationFactor;    // 5G FAPI Table 3-14
    } fapi_pusch_parms_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_uint8_tlv_t prachLongFormats;
        fapi_uint16_tlv_t prachShortFormats;
        fapi_uint8_tlv_t prachRestrictedSets;
        fapi_uint8_tlv_t maxPrachFdOccasionsInASlot;    // 5G FAPI Table 3-15
    } fapi_prach_parms_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_uint8_tlv_t rssiMeasurementSupport;    // 5G FAPI Table 3-16
    } fapi_meas_parms_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_cell_parms_t cell_parms;
        fapi_carrier_parms_t carr_parms;
        fapi_pdcch_parms_t pdcch_parms;
        fapi_pucch_parms_t pucch_parms;
        fapi_pdsch_parms_t pdsch_parms;
        fapi_pusch_parms_t pusch_parms;
        fapi_prach_parms_t prach_parms;
        fapi_meas_parms_t meas_parms;   // 5G FAPI Table 3-8
    } fapi_params_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_msg_t header;
        uint8_t error_code;
        uint8_t number_of_tlvs;
        uint8_t pad[2];
        fapi_uint16_tlv_t tlvs[FAPI_MAX_NUM_TLVS_PARAMS];   // 5G FAPI Table 3-5
    } fapi_param_resp_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_uint16_tlv_t dlBandwidth;
        fapi_uint32_tlv_t dlFrequency;
        fapi_config_num_tlv_t dlk0;
        fapi_config_num_tlv_t dlGridSize;
        fapi_uint16_tlv_t numTxAnt;
        fapi_uint16_tlv_t uplinkBandwidth;
        fapi_uint32_tlv_t uplinkFrequency;
        fapi_config_num_tlv_t ulk0;
        fapi_config_num_tlv_t ulGridSize;
        fapi_uint16_tlv_t numRxAnt;
        fapi_uint8_tlv_t frequencyShift7p5KHz;  // 5G FAPI Table 3-21
    } fapi_carrier_config_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_uint8_tlv_t phyCellId;
        fapi_uint8_tlv_t frameDuplexType;   // 5G FAPI Table 3-22
    } fapi_cell_config_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_uint32_tlv_t ssPbchPower;
        fapi_uint8_tlv_t bchPayload;
        fapi_uint8_tlv_t scsCommon; // 5G FAPI Table 3-23
    } fapi_ssb_config_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_uint16_tlv_t prachRootSequenceIndex;
        fapi_uint8_tlv_t numRootSequences;
        fapi_uint16_tlv_t k1;
        fapi_uint8_tlv_t prachZeroCorrConf;
        fapi_uint16_tlv_t numUnusedRootSequences;   // 5G FAPI Table 3-24 Subset
        fapi_uint16_tlv_t
            unusedRootSequences[FAPI_MAX_NUM_UNUSED_ROOT_SEQUENCES];
    } fapi_prachFdOccasion_t;

// Updated per 5G FAPI_
    typedef struct {
        fapi_uint8_tlv_t prachSequenceLength;
        fapi_uint8_tlv_t prachSubCSpacing;
        fapi_uint8_tlv_t restrictedSetConfig;
        fapi_uint8_tlv_t numPrachFdOccasions;
        fapi_uint8_tlv_t prachConfigIndex;
        fapi_prachFdOccasion_t prachFdOccasion[FAPI_MAX_NUM_PRACH_FD_OCCASIONS];
        fapi_uint8_tlv_t ssbPerRach;
        fapi_uint8_tlv_t prachMultipleCarriersInABand;  // 5G FAPI Table 3-24
    } fapi_prach_configuration_t;

//Updated per 5G FAPI
    typedef struct {
        fapi_uint16_tlv_t ssbOffsetPointA;
        fapi_uint8_tlv_t betaPss;
        fapi_uint8_tlv_t ssbPeriod;
        fapi_uint8_tlv_t ssbSubCarrierOffset;
        fapi_uint32_tlv_t mib;
        fapi_uint32_tlv_t ssbMask[2];
        fapi_uint8_tlv_t beamId[64];
        fapi_uint8_tlv_t ssPbchMultipleCarriersInABand;
        fapi_uint8_tlv_t multipleCellsSsPbchInACarrier; // 5G FAPI Table 3-25
    } fapi_ssb_table_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_uint8_tlv_t slotConfig[FAPI_MAX_NUM_OF_SYMBOLS_PER_SLOT];  // 5G FAPI Table 3-26 Subset
    } fapi_slotconfig_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_uint8_tlv_t tddPeriod;
        fapi_slotconfig_t slotConfig[FAPI_MAX_TDD_PERIODICITY]; // 5G FAPI Table 3-26
    } fapi_tdd_table_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_uint8_tlv_t rssiMeasurement;   // 5G FAPI Table 3-27
    } fapi_meas_config_t;

// Updated per 5G FAPI
    typedef struct {
        int16_t digBeamWeightRe;
        int16_t digBeamWeightIm;    // 5G FAPI Table 3-32 Subset
    } fapi_dig_beam_weight_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t beamIdx;
        fapi_dig_beam_weight_t digBeamWeight[FAPI_MAX_NUMBER_TX_RUS];   // 5G FAPI Table 3-32 Subset
    } fapi_dig_beam_config_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t numDigBeams;
        uint16_t numTxRus;
        fapi_dig_beam_config_t digBeam[FAPI_MAX_NUMBER_OF_BEAMS];   // 5G FAPI Table 3-32
    } fapi_beamforming_table_t;

// Updated per 5G FAPI
    typedef struct {
        int16_t preCoderWeightRe;
        int16_t preCoderWeightIm;   // 5G FAPI Table 3-33 Subset
    } fapi_precoderWeight_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_precoderWeight_t precoder_weight[FAPI_MAX_NUM_ANT_PORTS];  // 5G FAPI Table 3-33 Subset
    } fapi_precoder_weight_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t pmIdx;
        uint16_t numLayers;
        uint16_t numAntPorts;
        uint16_t pad[1];
        fapi_precoder_weight_t precoderWeight[FAPI_MAX_NUM_LAYERS]; // 5G FAPI Table 3-33
    } fapi_precoding_table_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_carrier_config_t carrierConfig;
        fapi_cell_config_t cellConfig;
        fapi_ssb_config_t ssbConfig;
        fapi_prach_configuration_t prachConfig;
        fapi_ssb_table_t ssbTable;
        fapi_tdd_table_t tddTable;
        fapi_meas_config_t measConfig;
        fapi_beamforming_table_t beamformingTable;
        fapi_precoding_table_t precodingTable;  // 5G FAPI Table 3-20
    } fapi_config_t;

// Updated per 5G FAPI 
    typedef struct {
        fapi_msg_t header;
        uint8_t number_of_tlvs;
        uint8_t pad[3];
        fapi_uint32_tlv_t tlvs[FAPI_MAX_NUM_TLVS_CONFIG];   // 5G FAPI Table 3-17
    } fapi_config_req_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_msg_t header;
        uint8_t error_code;
        uint8_t number_of_invalid_tlvs;
        uint8_t number_of_inv_tlvs_idle_only;
        uint8_t number_of_inv_tlvs_running_only;
        uint8_t number_of_missing_tlvs;
        uint8_t pad[3];
        fapi_uint16_tlv_t tlvs[4 * FAPI_MAX_NUM_TLVS_CONFIG];   // 5G FAPI Table 3-18
        //   fapi_uint16_tlv_t unsupported_or_invalid_tlvs[FAPI_MAX_NUMBER_UNSUPPORTED_TLVS];
        //   fapi_uint16_tlv_t invalid_idle_only_tlvs[FAPI_MAX_NUMBER_OF_INVALID_IDLE_ONLY_TLVS];
        //   fapi_uint16_tlv_t invalid_running_only_tlvs[FAPI_MAX_NUMBER_OF_INVALID_RUNNING_ONLY_TLVS];
        //   fapi_uint16_tlv_t missing_tlvs[FAPI_MAX_NUMBER_OF_MISSING_TLVS];            
    } fapi_config_resp_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_msg_t header;
    } fapi_start_req_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_msg_t header;      // Message Length is zero for STOP.request
    } fapi_stop_req_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_msg_t header;      // Message Length is zero for STOP.indication
    } fapi_stop_ind_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_msg_t header;
        uint16_t sfn;
        uint16_t slot;
        uint8_t message_id;
        uint8_t error_code;     // 5G FAPI Table 3-30
        uint8_t pad[2];
    } fapi_error_ind_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_msg_t header;
        uint16_t sfn;
        uint16_t slot;          // 5G FAPI Table 3-34
    } fapi_slot_ind_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t beamidx;       // 5G FAPI Table 3-43 subset
    } fapi_bmi_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t pmIdx;
        uint8_t pad[2];
        fapi_bmi_t beamIdx[FAPI_MAX_NUM_DIGBFINTERFACES];   // 5G FAPI Table 3-43 subset
    } fapi_pmi_bfi_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t numPrgs;
        uint16_t prgSize;
        uint8_t digBfInterfaces;
        uint8_t pad[3];
        fapi_pmi_bfi_t pmi_bfi[FAPI_MAX_NUM_PRGS_PER_TTI];  // 5G FAPI Table 3-43
    } fapi_precoding_bmform_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t rnti;
        uint16_t scramblingId;
        uint16_t scramblingRnti;
        uint8_t cceIndex;
        uint8_t aggregationLevel;
        fapi_precoding_bmform_t pc_and_bform;
        uint8_t beta_pdcch_1_0;
        uint8_t powerControlOffsetSS;
        uint16_t payloadSizeBits;
        uint8_t payload[FAPI_DCI_PAYLOAD_BYTE_LEN]; // 5G FAPI Table 3-37
    } fapi_dl_dci_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t bwpSize;
        uint16_t bwpStart;
        uint8_t subCarrierSpacing;
        uint8_t cyclicPrefix;
        uint8_t startSymbolIndex;
        uint8_t durationSymbols;
        uint8_t freqDomainResource[6];
        uint8_t cceRegMappingType;
        uint8_t regBundleSize;
        uint8_t interleaverSize;
        uint8_t coreSetType;
        uint16_t shiftIndex;
        uint8_t precoderGranularity;
        uint8_t pad;
        uint16_t numDlDci;      // 5G FAPI Table 3-36
        fapi_dl_dci_t dlDci[FAPI_MAX_NUMBER_DL_DCI];
    } fapi_dl_pdcch_pdu_t;

    // Updated per 5G FAPI
    typedef struct {
        uint16_t targetCodeRate;
        uint8_t qamModOrder;
        uint8_t mcsIndex;
        uint8_t mcsTable;
        uint8_t rvIndex;
        uint8_t pad[2];
        uint32_t tbSize;        // 5G FAPI Table 3-38 Subset
    } fapi_codeword_pdu_t;

    // Updated per 5G FAPI
    typedef struct {
        uint16_t pduBitMap;
        uint16_t rnti;
        uint16_t pdu_index;
        uint16_t bwpSize;
        uint16_t bwpStart;
        uint8_t subCarrierSpacing;
        uint8_t cyclicPrefix;
        uint8_t nrOfCodeWords;
        uint8_t pad[3];
        fapi_codeword_pdu_t cwInfo[FAPI_MAX_NUMBER_OF_CODEWORDS_PER_PDU];
        uint16_t dataScramblingId;
        uint8_t nrOfLayers;
        uint8_t transmissionScheme;
        uint8_t refPoint;
        uint8_t dmrsConfigType;
        uint16_t dlDmrsSymbPos;
        uint8_t scid;
        uint8_t numDmrsCdmGrpsNoData;
        uint8_t resourceAlloc;
        uint8_t pad1;
        uint16_t dlDmrsScramblingId;
        uint16_t dmrsPorts;
        uint16_t rbStart;
        uint16_t rbSize;
        uint8_t rbBitmap[36];
        uint8_t vrbToPrbMapping;
        uint8_t startSymbIndex;
        uint8_t nrOfSymbols;
        uint8_t ptrsPortIndex;
        uint8_t ptrsTimeDensity;
        uint8_t ptrsFreqDensity;
        uint8_t ptrsReOffset;
        uint8_t nEpreRatioOfPdschToPtrs;
        fapi_precoding_bmform_t preCodingAndBeamforming;
        uint8_t powerControlOffset;
        uint8_t powerControlOffsetSS;
        uint8_t isLastCbPresent;
        uint8_t isInlineTbCrc;
        uint32_t dlTbCrc;       // 5G FAPI Table 3-38
        uint8_t mappingType;
        uint8_t nrOfDmrsSymbols;
        uint8_t dmrsAddPos;
        uint8_t pad2;
    } fapi_dl_pdsch_pdu_t;

    // Updated per 5G FAPI
    typedef struct {
        uint16_t bwpSize;
        uint16_t bwpStart;
        uint8_t subCarrierSpacing;
        uint8_t cyclicPrefix;
        uint16_t startRb;
        uint16_t nrOfRbs;
        uint8_t csiType;
        uint8_t row;
        uint16_t freqDomain;
        uint8_t symbL0;
        uint8_t symbL1;
        uint8_t cdmType;
        uint8_t freqDensity;
        uint16_t scramId;
        uint8_t powerControlOffset;
        uint8_t powerControlOffsetSs;
        uint8_t pad[2];
        fapi_precoding_bmform_t preCodingAndBeamforming;    // 5G FAPI Table 3-39
    } fapi_dl_csi_rs_pdu_t;

// Updated per 5G FAPI 
    typedef struct {
        uint8_t dmrsTypeAPosition;
        uint8_t pdcchConfigSib1;
        uint8_t cellBarred;
        uint8_t intraFreqReselection;   // 5G FAPI Table 3-42
    } fapi_phy_mib_pdu_t;

// Updated per 5G FAPI 
    typedef struct {
        union {
            uint32_t bchPayload;
            fapi_phy_mib_pdu_t phyMibPdu;   // 5G FAPI Table 3-40 Subset
        };
    } fapi_bch_payload_t;

    // Updated per 5G FAPI
    typedef struct {
        uint16_t physCellId;
        uint8_t betaPss;
        uint8_t ssbBlockIndex;
        uint8_t ssbSubCarrierOffset;
        uint8_t bchPayloadFlag;
        uint16_t ssbOffsetPointA;
        fapi_bch_payload_t bchPayload;
        fapi_precoding_bmform_t preCodingAndBeamforming;    // 5G FAPI Table 3-40
    } fapi_dl_ssb_pdu_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t pduType;
        uint16_t pduSize;
        union {
            fapi_dl_pdcch_pdu_t pdcch_pdu;
            fapi_dl_pdsch_pdu_t pdsch_pdu;
            fapi_dl_csi_rs_pdu_t csi_rs_pdu;
            fapi_dl_ssb_pdu_t ssb_pdu;  // 5G FAPI Table 3-35 Subset
        } pdu;
    } fapi_dl_tti_req_pdu_t;

// Updated per 5G FAPI
    typedef struct {
        uint8_t nUe;
        uint8_t pad[3];
        uint8_t pduIdx[FAPI_MAX_NUMBER_OF_UES_PER_TTI]; // 5G FAPI Subset Table 3-35 and Table 3-44
    } fapi_ue_info_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_msg_t header;
        uint16_t sfn;
        uint16_t slot;
        uint8_t nPdus;
        uint8_t nGroup;
        uint8_t pad[2];
        fapi_dl_tti_req_pdu_t pdus[FAPI_MAX_PDUS_PER_SLOT]; // 5G FAPI Table 3-35
        fapi_ue_info_t ue_grp_info[FAPI_MAX_NUMBER_OF_GROUPS_PER_TTI];
    } fapi_dl_tti_req_t;

// Updated per 5G FAPI
    typedef struct {
        uint8_t rvIndex;
        uint8_t harqProcessId;
        uint8_t newDataIndicator;
        uint8_t pad;
        uint32_t tbSize;
        uint16_t numCb;         // 5G FAPI Table 3-47
        uint8_t cbPresentAndPosition[2];    // Since the maximum number of Code Blocks per TCB in a CBG is 8 for 1 CW or 4 for 2CW and this is a bit field with pading to align to 32 bits
    } fapi_pusch_data_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t harqAckBitLength;
        uint16_t csiPart1BitLength;
        uint16_t csiPart2BitLength;
        uint8_t alphaScaling;
        uint8_t betaOffsetHarqAck;
        uint8_t betaOffsetCsi1;
        uint8_t betaOffsetCsi2; // 5G FAPI Table 3-48
        uint8_t pad[2];
    } fapi_pusch_uci_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t ptrsPortIndex;
        uint8_t ptrsDmrsPort;
        uint8_t ptrsReOffset;   // 5G FAPI Table 3-49 Subset
    } fapi_ptrs_info_t;

// Updated per 5G FAPI
    typedef struct {
        uint8_t numPtrsPorts;
        uint8_t ptrsTimeDensity;
        uint8_t ptrsFreqDensity;    // 5G FAPI Table 3-49 Subset
        uint8_t ulPtrsPower;
        fapi_ptrs_info_t ptrsInfo[FAPI_MAX_NUM_PTRS_PORTS];
    } fapi_pusch_ptrs_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t lowPaprSequenceNumber;
        uint8_t lowPaprGroupNumber;
        uint8_t ulPtrsSampleDensity;
        uint8_t ulPtrsTimeDensityTransformPrecoding;
        uint8_t pad;            // 5G FAPI Table 3-50
    } fapi_dfts_ofdm_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_bmi_t beamIdx[FAPI_MAX_NUM_DIGBFINTERFACES];
    } fapi_rx_bfi_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t numPrgs;
        uint16_t prgSize;
        uint8_t digBfInterface;
        uint8_t pad[3];
        fapi_rx_bfi_t rx_bfi[FAPI_MAX_NUM_PRGS_PER_TTI];    // 5G FAPI Table 3-53
    } fapi_ul_rx_bmform_pdu_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t physCellId;
        uint8_t numPrachOcas;
        uint8_t prachFormat;
        uint8_t numRa;
        uint8_t prachStartSymbol;
        uint16_t numCs;
        fapi_ul_rx_bmform_pdu_t beamforming;
    } fapi_ul_prach_pdu_t;      // 5G FAPI Table 3-45

// Updated per 5G FAPI
    typedef struct {
        uint16_t pduBitMap;
        uint16_t rnti;
        uint32_t handle;
        uint16_t bwpSize;
        uint16_t bwpStart;
        uint8_t subCarrierSpacing;
        uint8_t cyclicPrefix;
        uint8_t mcsIndex;
        uint8_t mcsTable;
        uint16_t targetCodeRate;
        uint8_t qamModOrder;
        uint8_t transformPrecoding;
        uint16_t dataScramblingId;
        uint8_t nrOfLayers;
        uint8_t dmrsConfigType;
        uint16_t ulDmrsSymbPos;
        uint16_t ulDmrsScramblingId;
        uint8_t scid;
        uint8_t numDmrsCdmGrpsNoData;
        uint16_t dmrsPorts;
        uint16_t nTpPuschId;
        uint16_t tpPi2Bpsk;
        uint8_t rbBitmap[36];
        uint16_t rbStart;
        uint16_t rbSize;
        uint8_t vrbToPrbMapping;
        uint8_t frequencyHopping;
        uint16_t txDirectCurrentLocation;
        uint8_t resourceAlloc;
        uint8_t uplinkFrequencyShift7p5khz;
        uint8_t startSymbIndex;
        uint8_t nrOfSymbols;
        uint8_t mappingType;
        uint8_t nrOfDmrsSymbols;
        uint8_t dmrsAddPos;
        uint8_t pad;

        fapi_pusch_data_t puschData;
        fapi_pusch_uci_t puschUci;
        fapi_pusch_ptrs_t puschPtrs;
        fapi_dfts_ofdm_t dftsOfdm;
        fapi_ul_rx_bmform_pdu_t beamforming;    // 5G FAPI Table 3-46
    } fapi_ul_pusch_pdu_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t rnti;
        uint8_t pad1[2];
        uint32_t handle;
        uint16_t bwpSize;
        uint16_t bwpStart;
        uint8_t subCarrierSpacing;
        uint8_t cyclicPrefix;
        uint8_t formatType;
        uint8_t multiSlotTxIndicator;
        uint8_t pi2Bpsk;
        uint8_t pad2;
        uint16_t prbStart;
        uint16_t prbSize;
        uint8_t startSymbolIndex;
        uint8_t nrOfSymbols;
        uint8_t freqHopFlag;
        uint8_t groupHopFlag;
        uint8_t sequenceHopFlag;
        uint8_t pad3;
        uint16_t secondHopPrb;
        uint16_t hoppingId;
        uint16_t initialCyclicShift;
        uint16_t dataScramblingId;
        uint8_t timeDomainOccIdx;
        uint8_t preDftOccIdx;
        uint8_t preDftOccLen;
        uint8_t addDmrsFlag;
        uint16_t dmrsScramblingId;
        uint8_t dmrsCyclicShift;
        uint8_t srFlag;
        uint16_t bitLenHarq;
        uint8_t pad4[2];
        uint16_t bitLenCsiPart1;
        uint16_t bitLenCsiPart2;
        fapi_ul_rx_bmform_pdu_t beamforming;    // 5G FAPI Table 3-51
    } fapi_ul_pucch_pdu_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t rnti;
        uint8_t pad[2];
        uint32_t handle;
        uint16_t bwpSize;
        uint16_t bwpStart;
        uint8_t subCarrierSpacing;
        uint8_t cyclicPrefix;
        uint8_t numAntPorts;
        uint8_t numSymbols;
        uint8_t numRepetitions;
        uint8_t timeStartPosition;
        uint8_t configIndex;
        uint8_t bandwidthIndex;
        uint16_t sequenceId;
        uint8_t combSize;
        uint8_t combOffset;
        uint8_t cyclicShift;
        uint8_t frequencyPosition;
        uint16_t frequencyShift;
        uint8_t frequencyHopping;
        uint8_t groupOrSequenceHopping;
        uint8_t resourceType;
        uint8_t pad1[2];
        uint16_t tSrs;
        uint16_t tOffset;
        fapi_ul_rx_bmform_pdu_t beamforming;    // 5G FAPI Table 3-52
    } fapi_ul_srs_pdu_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t pduType;
        uint16_t pduSize;
        union {
            fapi_ul_prach_pdu_t prach_pdu;
            fapi_ul_pusch_pdu_t pusch_pdu;
            fapi_ul_pucch_pdu_t pucch_pdu;
            fapi_ul_srs_pdu_t srs_pdu;
        } pdu;
    } fapi_ul_tti_req_pdu_t;    // 5G FAPI Subset Table 3-44

// Updated per 5G FAPI
    typedef struct {
        fapi_msg_t header;
        uint16_t sfn;
        uint16_t slot;
        uint8_t nPdus;
        uint8_t rachPresent;
        uint8_t nUlsch;
        uint8_t nUlcch;
        uint8_t nGroup;
        uint8_t pad[3];
        fapi_ul_tti_req_pdu_t pdus[FAPI_MAX_NUMBER_UL_PDUS_PER_TTI];    // 5G FAPI Table 3-44
        fapi_ue_info_t ueGrpInfo[FAPI_MAX_NUMBER_OF_GROUPS_PER_TTI];
    } fapi_ul_tti_req_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t pduType;
        uint16_t pduSize;
        fapi_dl_pdcch_pdu_t pdcchPduConfig; // 5G FAPI Table 3-54 Subset
    } fapi_dci_pdu_t;

    // Updated per 5G FAPI
    typedef struct {
        fapi_msg_t header;
        uint16_t sfn;
        uint16_t slot;
        uint8_t numPdus;
        uint8_t pad[3];
        fapi_dci_pdu_t pdus[FAPI_MAX_NUMBER_UCI_PDUS_PER_SLOT]; // 5G FAPI Table 3-54
    } fapi_ul_dci_req_t;

// Updated per 5G FAPI
    typedef struct {
        uint32_t pdu_length;
        uint16_t pdu_index;
        uint16_t num_tlvs;
        fapi_uint8_ptr_tlv_t tlvs[FAPI_MAX_NUMBER_OF_TLVS_PER_PDU]; // 5G FAPI Table 3-58 Subset
    } fapi_tx_pdu_desc_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_msg_t header;
        uint16_t sfn;
        uint16_t slot;
        uint16_t num_pdus;
        uint8_t pad[2];
        fapi_tx_pdu_desc_t pdu_desc[FAPI_MAX_NUMBER_DL_PDUS_PER_TTI];   // 5G FAPI Table 3-58
    } fapi_tx_data_req_t;

// Updated per 5G FAPI
    typedef struct {
        uint32_t handle;
        uint16_t rnti;
        uint8_t harqId;
        uint8_t ul_cqi;
        uint16_t timingAdvance;
        uint16_t rssi;
        uint16_t pdu_length;
        uint8_t pad[2];
        void *pduData;          // 5G FAPI Table 3-61 Subset
    } fapi_pdu_ind_info_t;

    // Updated per 5G FAPI
    typedef struct {
        fapi_msg_t header;
        uint16_t sfn;
        uint16_t slot;
        uint16_t numPdus;
        uint8_t pad[2];
        fapi_pdu_ind_info_t pdus[FAPI_MAX_NUMBER_OF_ULSCH_PDUS_PER_SLOT];   // 5G FAPI Table 3-61
    } fapi_rx_data_indication_t;

// Updated per 5G FAPI
    typedef struct {
        uint32_t handle;
        uint16_t rnti;
        uint8_t harqId;
        uint8_t tbCrcStatus;
        uint8_t ul_cqi;
        uint8_t pad;
        uint16_t numCb;
        uint16_t timingAdvance;
        uint16_t rssi;
        uint8_t cbCrcStatus[FAPI_MAX_NUM_CB_PER_TTI_IN_BYTES];  // 5G FAPI Table 3-62 subset
    } fapi_crc_ind_info_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_msg_t header;
        uint16_t sfn;
        uint16_t slot;
        uint16_t numCrcs;
        uint8_t pad[2];
        fapi_crc_ind_info_t crc[FAPI_MAX_NUMBER_OF_CRCS_PER_SLOT];  // 5G FAPI Table 3-62
    } fapi_crc_ind_t;

// Updated per 5G FAPI
    typedef struct {
        uint8_t harqCrc;
        uint8_t pad;
        uint16_t harqBitLen;
        uint8_t harqPayload[FAPI_MAX_HARQ_INFO_LEN_BYTES];  // 5G FAPI Table 3-70
    } fapi_harq_info_t;

// Updated per 5G FAPI
    typedef struct {
        uint8_t csiPart1Crc;
        uint8_t pad;
        uint16_t csiPart1BitLen;
        uint8_t csiPart1Payload[FAPI_MAX_CSI_PART1_DATA_BYTES]; // 5G FAPI Table 3-71
    } fapi_csi_p1_info_t;

// Updated per 5G FAPI
    typedef struct {
        uint8_t csiPart2Crc;
        uint8_t pad;
        uint16_t csiPart2BitLen;
        uint8_t csiPart2Payload[FAPI_MAX_CSI_PART2_DATA_BYTES]; // 5G FAPI Table 3-72
    } fapi_csi_p2_info_t;

// Updated per 5G FAPI
    typedef struct {

        uint32_t handle;
        uint8_t pduBitmap;
        uint8_t ul_cqi;
        uint16_t rnti;
        uint16_t timingAdvance;
        uint16_t rssi;          // 5G FAPI Table 3-64
        fapi_harq_info_t harqInfo;  // This is included if indicated by the pduBitmap
        fapi_csi_p1_info_t csiPart1info;    // This is included if indicated by the pduBitmap
        fapi_csi_p2_info_t csiPart2info;    // This is included if indicated by the pduBitmap
    } fapi_uci_o_pusch_t;

// Updated per 5G FAPI
    typedef struct {
        uint8_t srIndication;
        uint8_t srConfidenceLevel;  // 5G FAPI Table 3-67
        uint8_t pad[2];
    } fapi_sr_f0f1_info_t;

// Updated per 5G FAPI
    typedef struct {
        uint8_t numHarq;
        uint8_t harqConfidenceLevel;
        uint8_t harqValue[FAPI_MAX_NUMBER_OF_HARQS_PER_IND];    // 5G FAPI Table 3-68
    } fapi_harq_f0f1_info_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t srBitlen;
        uint8_t srPayload[FAPI_MAX_SR_PAYLOAD_SIZE + 1];    // 5G FAPI Table 3-69
    } fapi_sr_f2f3f4_info_t;

// Updated per 5G FAPI
    typedef struct {
        uint8_t harqCrc;
        uint8_t pad;
        uint16_t harqBitLen;
        uint8_t harqPayload[FAPI_MAX_HARQ_PAYLOAD_SIZE + 2];    // 5G FAPI Table 3-70
    } fapi_harq_f2f3f4_info_t;

// Updated per 5G FAPI
    typedef struct {
        uint32_t handle;
        uint8_t pduBitmap;
        uint8_t pucchFormat;
        uint8_t ul_cqi;
        uint8_t pad;
        uint16_t rnti;
        uint16_t timingAdvance;
        uint16_t rssi;          // 5G FAPI Table 3-66
        uint16_t num_uci_bits;
        uint8_t uciBits[FAPI_MAX_UCI_BIT_BYTE_LEN];
    } fapi_uci_o_pucch_f2f3f4_t;

// Updated per 5G FAPI
    typedef struct {
        uint32_t handle;
        uint8_t pduBitmap;
        uint8_t pucchFormat;
        uint8_t ul_cqi;
        uint8_t pad;
        uint16_t rnti;
        uint16_t timingAdvance;
        uint16_t rssi;          // 5G FAPI Table 3-65
        uint8_t pad1[2];
        fapi_sr_f0f1_info_t srInfo; // This is included if indicated by the pduBitmap
        fapi_harq_f0f1_info_t harqInfo; // This is included if indicated by the pduBitmap
    } fapi_uci_o_pucch_f0f1_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t pduType;
        uint16_t pduSize;
        union {
            fapi_uci_o_pusch_t uciPusch;
            fapi_uci_o_pucch_f0f1_t uciPucchF0F1;
            fapi_uci_o_pucch_f2f3f4_t uciPucchF2F3F4;   // 5G FAPI Table 3-63 subset
        } uci;
    } fapi_uci_pdu_info_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_msg_t header;
        uint16_t sfn;
        uint16_t slot;
        uint16_t numUcis;       // 5G FAPI Table 3-63
        uint8_t pad[2];
        fapi_uci_pdu_info_t uciPdu[FAPI_MAX_NUMBER_UCI_PDUS_PER_SLOT];
    } fapi_uci_indication_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t numRbs;
        uint8_t pad[2];
        uint8_t rbSNR[FAPI_MAX_NUMBER_RBS]; // 5G FAPI Table 3-73 Subset
    } fapi_symb_snr_t;

// Updated per 5G FAPI
    typedef struct {
        uint32_t handle;
        uint16_t rnti;
        uint16_t timingAdvance;
        uint8_t numSymbols;
        uint8_t wideBandSnr;
        uint8_t numReportedSymbols;
        uint8_t pad;
        fapi_symb_snr_t symbSnr[FAPI_MAX_NUMBER_OF_REP_SYMBOLS];    // 5G FAPI Table 3-73 subset
    } fapi_srs_pdu_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_msg_t header;
        uint16_t sfn;
        uint16_t slot;
        uint8_t numPdus;
        uint8_t pad[3];
        fapi_srs_pdu_t srsPdus[FAPI_MAX_NUMBER_SRS_PDUS_PER_SLOT];  // 5G FAPI Table 3-73
    } fapi_srs_indication_t;

// Updated per 5G FAPI
    typedef struct {
        uint8_t preambleIndex;
        uint8_t pad;
        uint16_t timingAdvance;
        uint32_t preamblePwr;   // 5G FAPI Table 3-74 Subset
    } fapi_preamble_info_t;

// Updated per 5G FAPI
    typedef struct {
        uint16_t phyCellId;
        uint8_t symbolIndex;
        uint8_t slotIndex;
        uint8_t freqIndex;
        uint8_t avgRssi;
        uint8_t avgSnr;
        uint8_t numPreamble;
        fapi_preamble_info_t preambleInfo[FAPI_MAX_NUM_PREAMBLES_PER_SLOT]; // 5G FAPI Table 3-74 Subset
    } fapi_rach_pdu_t;

// Updated per 5G FAPI
    typedef struct {
        fapi_msg_t header;
        uint16_t sfn;
        uint16_t slot;
        uint8_t numPdus;
        uint8_t pad[3];
        fapi_rach_pdu_t rachPdu[FAPI_MAX_NUMBER_RACH_PDUS_PER_SLOT];    // 5G FAPI Table 3-74
    } fapi_rach_indication_t;

//------------------------------------------------------------------------------

#if defined(__cplusplus)
}
#endif
#endif
