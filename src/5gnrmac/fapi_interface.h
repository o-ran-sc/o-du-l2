/******************************************************************************
*   Copyright 2017 Cisco Systems, Inc.
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
// This file has been modified by Intel in order to support 5G FAPI:PHY API Specification
// Document 222.10.01 dated June 2019
// Changes made by luis.farias@intel.com

#ifndef _FAPI_INTERFACE_H_
#define _FAPI_INTERFACE_H_

#if defined(__cplusplus)
extern "C" {
#endif

typedef signed char		int8_t;
typedef unsigned char	uint8_t;
typedef int16_t	int16_t;
typedef uint16_t	uint16_t;
typedef int32_t		int32_t;
typedef uint32_t	uint32_t;

// Update for 5G FAPI
#define FAPI_PARAM_REQUEST									0x00
#define FAPI_PARAM_RESPONSE									0x01
#define FAPI_CONFIG_REQUEST									0x02
#define FAPI_CONFIG_RESPONSE								0x03
#define FAPI_START_REQUEST									0x04
#define FAPI_STOP_REQUEST									0x05
#define FAPI_STOP_INDICATION								0x06
#define FAPI_ERROR_INDICATION                               0x07
// Reserved 0x08 - 0x7f
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
#define FAPI_RSSI_MESUREMENT_TAG                            0x1028

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


// TODO : Work out what the correct maximums should be// Needs Review for 5G
#if 0
// Number of UL/DL configurations, I, as defined by 36.212 section 5.3.3.1.4
// todo : work out what the max is
#define FAPI_MAX_UL_DL_CONFIGURATIONS						4
#define FAPI_MAX_NUM_PHYSICAL_ANTENNAS						4
#define FAPI_MAX_NUM_SCHEDULED_UES							8
#define FAPI_MAX_NUM_SUBBANDS								8
#define FAPI_MAX_ANTENNA_PORT_COUNT							2
#endif

// 5G FAPI Definitions
#define NUMEROLOGIES                                        5
#define MAX_NUM_UNUSED_ROOT_SEQUENCES                       63 // 38.331 page 383
#define MAX_NUM_PRACH_FD_OCCASIONS                          64 // 38.331 page 383
#define MAX_NUM_OF_SYMBOLS_PER_SLOT                         14
#define MAX_TDD_PERIODICITY                                 160// 38.212 11.1 for u=4 and P=10 ms
#define MAX_NUMBER_TX_RUS                                   4  // m=p*q with p number of panels and q number of TxRU/RxRU per panel, depends on
                                                               // the RF configuration, currently n=m=4, q=1, p=4 and k=21 (number of beams per pannel). n number of antenna ports
#define MAX_NUMBER_OF_BEAMS                                 64 // Intel API Page 27
#define MAX_NUM_ANT_PORTS                                   8  // Based on current RF
#define MAX_NUM_LAYERS                                      8  // 38.211 Table 7.3.1.3-1
#define MAX_NUM_TLVS_CELL_CONFIG                            2  // 5G FAPI Table 3-9  (A)
#define MAX_NUM_TLVS_CARRIER_CONFIG                         27 // 5G FAPI Table 3-10 (B)
#define MAX_NUM_TLVS_PDCCH_CONFIG                           6  // 5G FAPI Table 3-11 (C)
#define MAX_NUM_TLVS_PUCCH_CONFIG                           2  // 5G FAPI Table 3-12 (D)
#define MAX_NUM_TLVS_PDSCH_CONFIG                           14 // 5G FAPI Table 3-13 (E)
#define MAX_NUM_TLVS_PUSCH_CONFIG                           17 // 5G FAPI Table 3-14 (F)
#define MAX_NUM_TLVS_PRACH_CONFIG                           4  // 5G FAPI Table 3-15 (G)
#define MAX_NUM_TLVS_MEAS_CONFIG                            1  // 5G FAPI Table 3-16 (H)
#define MAX_NUM_TLVS_CONFIG                                 74 //  A+B+C+D+E+F+G+H + Padding
#define MAX_NUMBER_UNSUPPORTED_TLVS                         74     
#define MAX_NUMBER_OF_INVALID_IDLE_ONLY_TLVS                74
#define MAX_NUMBER_OF_INVALID_RUNNING_ONLY_TLVS             74
#define MAX_NUMBER_OF_MISSING_TLVS                          74
#define MAX_NUM_DIGBFINTERFACES                             4 // Based on RF, 5G FAPI says {0,255}         
#define MAX_NUM_PRGS_PER_TTI                                4 // Based on 38.214 5.1.2.3
#define DCI_PAYLOAD_BYTE_LEN                                32 // Based on Intel API MAX_DCI_BIT_BYTE_LEN     
#define MAX_NUMBER_DL_DCI                                   32 // Based on Intel API MAX_NUM_PDCCH                               
#define MAX_NUMBER_OF_CODEWORDS_PER_PDU                     2 // Based on MAX_DL_CODEWORD
#define MAX_NUMBER_DL_PDUS_PER_TTI                          129 // Based on (MAX_NUM_PDSCH*MAX_DL_CODEWORD + MAX_NUM_PDCCH + MAX_NUM_SRS + 1 PBCH/SLOT)                       
#define MAX_NUMBER_OF_UES_PER_TTI                           16  // Per common_ran_parameters.h                                            
#define MAX_NUM_CB_PER_TTI_IN_BYTES                         192 // Based on Max Tb size of 1376264 bits + 24 crc over (8848-24) and O/H
#define MAX_NUM_PTRS_PORTS                                  12  // Per 3GPP 38.212 Table 7.3.1.1.2-21
#define MAX_NUMBER_OF_GROUPS_PER_TTI                        8  // FlexRAN API Table 33
#define MAX_NUMBER_UL_PDUS_PER_TTI                          328 // (MAX_NUM_PUSCH+MAX_NUM_PUCCH+MAX_NUM_SRS+MAX_NUM_PRACH_DET)
#define MAX_NUMBER_DCI_PDUS_PER_TTI                         32 // Based on MAX_NUM_PDCCH
#define MAX_NUMBER_OF_TLVS_PER_PDU                          32 // Based on FAPI/nFAPI implementation
#define MAX_NUMBER_TX_PDUS_PER_TTI                          129 // Same as MAX_NUMBER_DL_PDUS_PER_TTI
#define MAX_PDU_LENGTH                                   172096 // Based on 38.214 5.1.3.4, the TBS is 1376264 bits and divided by 8 and aligned to 64 bytes
#define MAX_NUMBER_OF_PDUS_PER_TTI                          129 // Same as MAX_NUMBER_DL_PDUS_PER_TTI
#define MAX_NUMBER_OF_ULSCH_PDUS_PER_TTI                    64  // NUM_PUSCH_CHAN*MAX_NUMBER_OF_CODEWORDS_PER_PDU
#define MAX_NUMBER_OF_CRCS_PER_SLOT                         32  // Based on MAX_NUM_UL_CHAN                      
#define MAX_HARQ_INFO_LEN_BYTES                             214  // Based on 5G FAPI Table 3-70
#define MAX_CSI_PART1_DATA_BYTES                            214  // Based on 5G FAPI Table 3-71
#define MAX_CSI_PART2_DATA_BYTES                            214  // Based on 5G FAPI Table 3-72
#define MAX_NUMBER_OF_HARQS_PER_IND                         2    // Based on 5G FAPI Table 3-68
#define MAX_SR_PAYLOAD_SIZE                                 1    // Based on 5G FAPI Table 3-69
#define MAX_HARQ_PAYLOAD_SIZE                               214  // Based on 5G FAPI Table 3-70
#define MAX_NUMBER_UCI_PDUS_PER_SLOT                        200  // Based on MAX_NUM_PUCCH
#define MAX_NUMBER_RBS                                      273  // Based on MAX_NUM_OF_PRB_IN_FULL_BAND
#define MAX_NUMBER_OF_REP_SYMBOLS                           4    // Based on 5g FAPI Table 3-73
#define MAX_NUMBER_SRS_PDUS_PER_SLOT                        32   // Based on MAX_NUM_SRS
#define MAX_NUM_PREAMBLES_PER_SLOT                          64   // Based on MAX_NUM_PRACH_DET
#define MAX_NUMBER_RACH_PDUS_PER_SLOT                       64   // Based on MAX_NUM_PRACH_DET

// Updated per 5G FAPI
typedef struct {
    uint8_t                     numberOfMessagesIncluded;
    uint8_t                     handle;   // Can be used for Phy Id or Carrier Id
} fapi_msg_header_t;
// Updated per 5G FAPI
typedef struct {
	uint8_t                     message_type_id;
	uint32_t                    length; // Length of the message body in bytes
} fapi_msg_t;
// Updated per 5G FAPI
typedef struct {
	uint16_t                    tag;
	uint16_t                    length;
} fapi_tl_t;
// Updated per 5G FAPI
typedef struct {
    fapi_tl_t                   tl;
    uint8_t                     value;
} fapi_uint8_tlv_t;
// Updated per 5G FAPI
typedef struct {
    fapi_tl_t                   tl;
    uint16_t                    value;
} fapi_uint16_tlv_t;
// Updated per 5G FAPI
typedef struct {
    fapi_tl_t                   tl;
    int16_t                     value;
} fapi_int16_tlv_t;
// Updated per 5G FAPI
typedef struct {
    fapi_tl_t                   tl;
    uint32_t                    value;
} fapi_uint32_tlv_t;
// Updated per 5G FAPI
typedef struct {
    uint16_t                    tag;                // In 5G FAPI for Cell Params
    uint8_t                     length;
    uint8_t                     value;
} fapi_config_tlv_t;
    
// Updated per 5G FAPI
typedef struct {
	fapi_msg_t                  header;  // For PARAM.req message length in fapi_msg_t is zero
} fapi_param_req_t;

// Updated per 5G FAPI
typedef struct {
    fapi_uint16_tlv_t           releaseCapability;
    fapi_uint16_tlv_t           phyState;
    fapi_uint8_tlv_t            skipBlankDlConfig;
    fapi_uint8_tlv_t            skipBlankUlConfig;
    fapi_uint16_tlv_t           numTlvsToReport;
    //fapi_param_tlv_t            tlvStatus[MAX_NUMBER_OF_CONFIG_PARMS];          // Need to define this value based on 5G FAPI
} fapi_cell_parms_t;

// Updated per 5G FAPI
typedef struct {
    fapi_uint8_tlv_t            cyclicPrefix;
    fapi_uint8_tlv_t            supportedSubcarrierSpacingDl;
    fapi_uint16_tlv_t           supportedBandwidthDl;
    fapi_uint8_tlv_t            supportedSubcarrierSpecingsUl;
    fapi_uint16_tlv_t           supportedBandwidthUl;
}   fapi_carrier_parms_t;

// Updated per 5G FAPI    
typedef struct {
    fapi_uint8_tlv_t            cceMappingType;
    fapi_uint8_tlv_t            coresetOutsideFirst3OfdmSymsOfSlot;
    fapi_uint8_tlv_t            precoderGranularityCoreset;
    fapi_uint8_tlv_t            pdcchMuMimo;
    fapi_uint8_tlv_t            pdcchPrecoderCycling;
    fapi_uint8_tlv_t            maxPdcchsPerSlot;
}   fapi_pdcch_parms_t;

// Updated per 5G FAPI
typedef struct {
    fapi_uint8_tlv_t            pucchFormats;
    fapi_uint8_tlv_t            maxPucchsPerSlot;
}   fapi_pucch_parms_t;

// Updated per 5G FAPI
typedef struct {
    fapi_uint8_tlv_t            pdschMappingType;
    fapi_uint8_tlv_t            pdschAllocationTypes;
    fapi_uint8_tlv_t            pdschVrbToPrbMapping;
    fapi_uint8_tlv_t            pdschCbg;
    fapi_uint8_tlv_t            pdschDmrsConfigTypes;
    fapi_uint8_tlv_t            pdschDmrsMaxLength;
    fapi_uint8_tlv_t            pdschDmrsAdditionalPos;
    fapi_uint8_tlv_t            maxPdschsTBsPerSlot;
    fapi_uint8_tlv_t            maxNumberMimoLayersPdsch;
    fapi_uint8_tlv_t            supportedMaxModulationOrderDl;
    fapi_uint8_tlv_t            maxMuMimoUsersDl;
    fapi_uint8_tlv_t            pdschDataInDmrsSymbols;
    fapi_uint8_tlv_t            premptionSupport;
    fapi_uint8_tlv_t            pdschNonSlotSupport;
}   fapi_pdsch_parms_t;

// Updated per 5G FAPI
typedef struct {
    fapi_uint8_tlv_t            uciMuxUlschInPusch;
    fapi_uint8_tlv_t            uciOnlyPusch;
    fapi_uint8_tlv_t            puschFrequencyHopping;
    fapi_uint8_tlv_t            puschDmrsConfigTypes;
    fapi_uint8_tlv_t            puschDmrsMaxLen;
    fapi_uint8_tlv_t            puschDmrsAditionalPos;
    fapi_uint8_tlv_t            puschCbg;
    fapi_uint8_tlv_t            puschMappingType;     
    fapi_uint8_tlv_t            puschAllocationTypes;
    fapi_uint8_tlv_t            puschVrbToPrbMapping;
    fapi_uint8_tlv_t            puschMaxPtrsPorts;
    fapi_uint8_tlv_t            maxPduschsTBsPerSlot;
    fapi_uint8_tlv_t            maxNumberMimoLayersnonCbPusch;
    fapi_uint8_tlv_t            supportedModulationOrderUl;
    fapi_uint8_tlv_t            maxMuMimoUsersUl;
    fapi_uint8_tlv_t            dftsOfdmSupport;
    fapi_uint8_tlv_t            puschAggregationFactor;
}   fapi_pusch_parms_t;

// Updated per 5G FAPI
typedef struct {
    fapi_uint8_tlv_t            prachLongFormats;
    fapi_uint8_tlv_t            prachShortFormats;
    fapi_uint8_tlv_t            prachRestrictedSets;
    fapi_uint8_tlv_t            maxPrachFdOccasionsInASlot;
}   fapi_prach_parms_t;

// Updated per 5G FAPI
typedef struct {
    fapi_uint8_tlv_t            rssiMeasurementSupport;
}   fapi_meas_parms_t;

// Updated per 5G FAPI
typedef struct {
    fapi_cell_parms_t           cell_parms;
    fapi_carrier_parms_t        carr_parms;
    fapi_pdcch_parms_t          pdcch_parms;
    fapi_pucch_parms_t          pucch_parms;
    fapi_pdsch_parms_t          pdsch_parms;
    fapi_pusch_parms_t          pusch_parms;
    fapi_prach_parms_t          prach_parms;
    fapi_meas_parms_t           meas_parms;
} fapi_params_t;

// Updated per 5G FAPI
typedef struct {
	fapi_msg_t                  header;
	uint8_t                     error_code;
	uint8_t                     number_of_tlvs;
    fapi_uint16_tlv_t           tlvs[MAX_NUM_TLVS_CONFIG];
} fapi_param_resp_t;

// Updated per 5G FAPI
typedef struct {
    fapi_uint16_tlv_t           dlBandwidth;
    fapi_uint32_tlv_t           dlFrequency;
    fapi_uint16_tlv_t           dlk0[NUMEROLOGIES];
    fapi_uint16_tlv_t           dlGridSize[NUMEROLOGIES];
    fapi_uint16_tlv_t           numTxAnt;
    fapi_uint16_tlv_t           uplinkBandwidth;
    fapi_uint32_tlv_t           uplinkFrequency;
    fapi_uint16_tlv_t           ulk0[NUMEROLOGIES];
    fapi_uint16_tlv_t           ulGridSize[NUMEROLOGIES];
    fapi_uint16_tlv_t           numRxAnt;
    fapi_uint8_tlv_t            frequencyShift7p5KHz;
} fapi_carrier_config_t;

// Updated per 5G FAPI
typedef struct {
    fapi_uint8_tlv_t            phyCellId;
    fapi_uint8_tlv_t            frameDuplexType;
} fapi_cell_config_t;

// Updated per 5G FAPI
typedef struct {
    fapi_uint32_tlv_t           ssPbchPower;
    fapi_uint8_tlv_t            bchPayload;
    fapi_uint8_tlv_t            scsCommon;
} fapi_ssb_config_t;

// Updated per 5G FAPI
typedef struct {
    fapi_uint16_tlv_t           prachRootSequenceIndex;
    fapi_uint8_tlv_t            numRootSequences;
    fapi_uint8_tlv_t            unusedRootSequences[MAX_NUM_UNUSED_ROOT_SEQUENCES];
} fapi_prachFdOccasion_t;

// Updated per 5G FAPI
typedef struct {
    fapi_uint8_tlv_t            prachSequenceLength;
    fapi_uint8_tlv_t            prachSubCSpacing;
    fapi_uint8_tlv_t            restrictedSetConfig;
    fapi_prachFdOccasion_t      prachFdOccasion[MAX_NUM_PRACH_FD_OCCASIONS];
    fapi_uint8_tlv_t            ssbPerRach;
    fapi_uint8_tlv_t            prachMultipleCarriersInABand;
} fapi_prach_configuration_t;

//Updated per 5G FAPI
typedef struct {
    fapi_uint16_tlv_t           ssbOffsetPointA;
    fapi_uint8_tlv_t            betaPss;
    fapi_uint8_tlv_t            ssbPeriod;
    fapi_uint8_tlv_t            ssbSubCarrierOffset;
    fapi_uint32_tlv_t           mib;
    fapi_uint32_tlv_t           ssbMask[2];
    fapi_uint8_tlv_t            beamId[64];
    fapi_uint8_tlv_t            ssPbchMultipleCarriersInABand;
    fapi_uint8_tlv_t            multipleCellsSsPbchInACarrier;
} fapi_ssb_table_t;

// Updated per 5G FAPI
typedef struct {
    fapi_uint8_tlv_t            slotConfig[MAX_NUM_OF_SYMBOLS_PER_SLOT];
} fapi_slotconfig_t;

// Updated per 5G FAPI
typedef struct {
    fapi_uint8_tlv_t            tddPeriod;
    fapi_slotconfig_t           slotConfig[MAX_TDD_PERIODICITY];
} fapi_tdd_table_t;

// Updated per 5G FAPI
typedef struct {
    fapi_uint8_tlv_t            rssiMeasurement;
} fapi_meas_config_t;

// Updated per 5G FAPI
typedef struct {
    int16_t                     digBeamWeightRe;
    int16_t                     digBeamWeightIm;
} fapi_dig_beam_weight_t;

// Updated per 5G FAPI
typedef struct {
    uint16_t                    beamIdx;
    fapi_dig_beam_weight_t      digBeamWeight[MAX_NUMBER_TX_RUS];
} fapi_dig_beam_config_t;

// Updated per 5G FAPI
typedef struct {
    uint16_t                    numDigBeams;
    uint16_t                    numTxRus;
    fapi_dig_beam_config_t      digBeam[MAX_NUMBER_OF_BEAMS];
} fapi_beamforming_table_t;

// Updated per 5G FAPI
typedef struct {
    int16_t                     preCoderWeightRe;
    int16_t                     preCoderWeightIm;
} fapi_precoderWeight_t;

// Updated per 5G FAPI
typedef struct {
    fapi_precoderWeight_t       precoder_weight[MAX_NUM_ANT_PORTS];
} fapi_precoder_weight_t;

// Updated per 5G FAPI
typedef struct {
    uint16_t                    pmIdx;
    uint16_t                    numLayers;
    uint16_t                    numAntPorts;
    fapi_precoder_weight_t      precoderWeight[MAX_NUM_LAYERS];
} fapi_precoding_table_t;

// Updated per 5G FAPI
typedef struct {
    fapi_carrier_config_t       carrierConfig;
    fapi_cell_config_t          cellConfig;
    fapi_ssb_config_t           ssbConfig; 
    //fapi_prach_config_t         prachConfig;    //To be defined
    fapi_ssb_table_t            ssbTable;
    fapi_tdd_table_t            tddTable;
    fapi_meas_config_t          measConfig;
    fapi_beamforming_table_t    beamformingTable;
    fapi_precoding_table_t      precodingTable;
 } fapi_config_t;

// Updated per 5G FAPI 
typedef struct {
	fapi_msg_t                  header;
	uint8_t                     number_of_tlvs;
    fapi_uint16_tlv_t           tlvs[MAX_NUM_TLVS_CONFIG];
} fapi_config_req_t;

// Updated per 5G FAPI
typedef struct {
	fapi_msg_t                  header;
	uint8_t                     error_code;
	uint8_t                     number_of_invalid_tlvs;
    uint8_t                     number_of_inv_tlvs_idle_only;
    uint8_t                     number_of_missing_tlvs;
    fapi_uint16_tlv_t           tlvs[4*MAX_NUM_TLVS_CONFIG];
 //   fapi_uint16_tlv_t           unsupported_or_invalid_tlvs[MAX_NUMBER_UNSUPPORTED_TLVS];
 //   fapi_uint16_tlv_t           invalid_idle_only_tlvs[MAX_NUMBER_OF_INVALID_IDLE_ONLY_TLVS];
 //   fapi_uint16_tlv_t           invalid_running_only_tlvs[MAX_NUMBER_OF_INVALID_RUNNING_ONLY_TLVS];
 //   fapi_uint16_tlv_t           missing_tlvs[MAX_NUMBER_OF_MISSING_TLVS];
} fapi_config_resp_t;

// Updated per 5G FAPI
typedef struct {
	fapi_msg_t                  header;  // Message Length is zero for START.request
} fapi_start_req_t;

// Updated per 5G FAPI
typedef struct {
	fapi_msg_t                  header;  // Message Length is zero for STOP.request
} fapi_stop_req_t;

// Updated per 5G FAPI
typedef struct {
	fapi_msg_t                  header; // Message Length is zero for STOP.indication
} fapi_stop_ind_t;

// Updated per 5G FAPI
typedef struct {
	fapi_msg_t                  header;
    uint16_t                    sfn;
    uint16_t                    slot;
	uint8_t                     message_id;
	uint8_t                     error_code;
} fapi_error_ind_t;

// Updated per 5G FAPI
typedef struct {
	fapi_msg_t                  header;
	uint16_t                    sfn;
    uint16_t                    slot;
} fapi_slot_ind_t;

// Updated per 5G FAPI
typedef struct {
    uint16_t                    beamidx;
} fapi_bmi_t;

// Updated per 5G FAPI
typedef struct {
    uint16_t                    pmIdx;
    fapi_bmi_t                  beamIdx[MAX_NUM_DIGBFINTERFACES];
} fapi_pmi_bfi_t;

// Updated per 5G FAPI
typedef struct {   
    uint16_t                    numPrgs;
    uint16_t                    prgSize;
    uint8_t                     digBfInterfaces;
    fapi_pmi_bfi_t              pmi_bfi[MAX_NUM_PRGS_PER_TTI];
} fapi_precoding_bmform_t;

// Updated per 5G FAPI
typedef struct {
    uint16_t                    rnti;
    uint16_t                    scramblingId;             
    uint16_t                    scramblingRnti;
    uint8_t                     cceIndex;
    uint8_t                     aggregationLevel;
    fapi_precoding_bmform_t     pc_and_bform;
    uint8_t                     beta_pdcch_1_0;
    uint8_t                     powerControlOfssetSS;
    uint16_t                    payloadSizeBits;
    uint8_t                     payload[DCI_PAYLOAD_BYTE_LEN];
} fapi_dl_dci_t;

// Updated per 5G FAPI
typedef struct {
    uint16_t                    bwpSize;
    uint16_t                    bwpPart;
    uint8_t                     subCarrierSpacing;
    uint8_t                     cyclicPrefix;
    uint8_t                     startSymbolIndex;
    uint8_t                     durationSymbols;
    uint8_t                     freqDomainResource[6];
    uint8_t                     cceRegMappingType;
    uint8_t                     regBundleSize;
    uint8_t                     interleaverSize;
    uint8_t                     coreSetSize;
    uint16_t                    shiftIndex;                  
    uint8_t                     precoderGranularity;
    uint16_t                    numDlDci;
    fapi_dl_dci_t*              dlDci;
 } fapi_dl_pddch_pdu_t;
 
 // Updated per 5G FAPI
 typedef struct {
    uint16_t                    targetCodeRate;                     
    uint8_t                     qamModOrder;
    uint8_t                     mcsIndex;
    uint8_t                     mcsTable;
    uint8_t                     rvIndex;
    uint32_t                    tbSize;
 } fapi_codeword_pdu_t;
 
 // Updated per 5G FAPI
 typedef struct {
    uint16_t                    pduBitMap; 
    uint16_t                    rnti;
    uint16_t                    pduIndex;
    uint16_t                    bwpSize;
    uint16_t                    bwpStart;
    uint8_t                     subCarrierSpacing;
    uint8_t                     cyclicPrefix;                   
    uint8_t                     nrOfCodeWords;
    fapi_codeword_pdu_t         cwInfo[MAX_NUMBER_OF_CODEWORDS_PER_PDU]; 
    uint16_t                    dataScramblingId;          
    uint8_t                     nrOfLayers;
    uint8_t                     transmissionScheme;
    uint8_t                     refPoint;
    uint16_t                    dlDmrsSymbPos;
    uint8_t                     dmrsConfigType;
    uint16_t                    dlDmrsScramblingId;
    uint8_t                     scid;
    uint8_t                     numDmrsCdmGrpsNoData;
    uint16_t                    dmrsPorts;
    uint8_t                     resourceAlloc;
    uint8_t                     rbBitmap;
    uint16_t                    rbStart;
    uint16_t                    rbSize;
    uint8_t                     vrbToPrbMapping;
    uint8_t                     startSymbIndex;
    uint8_t                     nrOfSymbols;
    uint8_t                     ptrsPortIndex;
    uint8_t                     ptrsTimeDensity;
    uint8_t                     ptrsFreqDensity;
    uint8_t                     ptrsReOffset;
    uint8_t                     nEpreRatioOfPdschToPtrs;
    fapi_precoding_bmform_t     preCodingAndBeamforming;
    uint8_t                     powerControlOffset;             
    uint8_t                     powerControlOffsetSS;
    uint8_t                     isLastCbPresent;
    uint8_t                     isInlineTbCrc;
    uint32_t                    dlTbCrc;
 } fapi_dl_pdsch_pdu_t;
 
 // Updated per 5G FAPI
 typedef struct {
    uint16_t                    bwpSize;
    uint16_t                    bwpStart;
    uint8_t                     subCarrierSpacing;
    uint8_t                     cyclicPrefix;
    uint16_t                    startRb;
    uint16_t                    nrOfRbs;
    uint8_t                     csiType;
    uint8_t                     row;
    uint16_t                    freqDomain;
    uint8_t                     symbL0;
    uint8_t                     symbL1;
    uint8_t                     cdmType;               
    uint8_t                     freqDensity;
    uint16_t                    scramId;
    uint8_t                     powerControlOffset;
    uint8_t                     powerControlOffsetSs;
    fapi_precoding_bmform_t     preCodingAndBeamforming;
 } fapi_dl_csi_rs_pdu_t;

// Updated per 5G FAPI 
 typedef struct {
    uint8_t                     dmrsTypeAPosition;
    uint8_t                     pdcchConfigSib1;
    uint8_t                     cellBarred;
    uint8_t                     intraFreqReselction;
 } fapi_phy_mib_pdu_t;

// Updated per 5G FAPI 
 typedef struct {
    union
    {
        uint32_t                bchPayload;
        fapi_phy_mib_pdu_t      phyMibPdu;
    }u;
 } fapi_bch_payload_t;
 
 // Updated per 5G FAPI
 typedef struct {
    
    uint16_t                    physCellId;
    uint8_t                     betaPss;
    uint8_t                     ssbBlockIndex;
    uint16_t                    ssbSubCarrierOffset;
    uint8_t                     ssbOffsetPointA;
    uint8_t                     bchPayloadFlag;
    fapi_bch_payload_t          bchPayload;
    fapi_precoding_bmform_t     preCodingAndBeamforming;
 }  fapi_dl_ssb_pdu_t;
 
// Updated per 5G FAPI
 typedef struct {
    uint16_t                    pduType;
    uint16_t                    pduSize;
    union
    {
     fapi_dl_pddch_pdu_t        pdcch_pdu;
     fapi_dl_pdsch_pdu_t        pdsch_pdu;
     fapi_dl_csi_rs_pdu_t       csi_rs_pdu;
     fapi_dl_ssb_pdu_t          ssb_pdu;
    }u;
} fapi_dl_tti_req_pdu_t;

// Updated per 5G FAPI
typedef struct {
    fapi_msg_t                  header;
    uint16_t                    sfn;
    uint16_t                    slot;
    uint8_t                     nPdus;
    uint8_t                     nGroup;
    fapi_dl_tti_req_pdu_t*      pdus;
} fapi_dl_tti_req_t;

// Updated per 5G FAPI
typedef struct {  
    uint8_t                     physCellId;
    uint8_t                     numPrachOcas;
    uint8_t                     prachFormat;
    uint8_t                     numRa;
    uint8_t                     prachStartSymbol;
    uint8_t                     numCs;
    fapi_precoding_bmform_t     beamforming;
} fapi_ul_prach_pdu_t;

// Updated per 5G FAPI
typedef struct {
    uint8_t                     nUe;
    uint8_t                     pduIdx[MAX_NUMBER_OF_UES_PER_TTI];
} fapi_ue_info_t;

// Updated per 5G FAPI
typedef struct {
    uint8_t                     rvIndex;
    uint8_t                     harqProcessId;
    uint8_t                     newDataIndicator;
    uint32_t                    tbSize;
    uint16_t                    numCb;
    uint8_t                     cbPresentAndPosition[MAX_NUM_CB_PER_TTI_IN_BYTES];
}fapi_pusch_data_t;

// Updated per 5G FAPI
typedef struct {
    uint16_t                    harqAckBitLength;
    uint16_t                    csiPart1BitLength;
    uint16_t                    csiPart2BitLength;
    uint8_t                     alphaScaling;
    uint8_t                     betaOffsetHarqAck;
    uint8_t                     betaOffsetCsi1;
    uint8_t                     betaOffsetCsi2;
} fapi_pusch_uci_t;

// Updated per 5G FAPI
typedef struct {
    uint16_t                    ptrsPortIndex;
    uint8_t                     ptrsDmrsPort;
    uint8_t                     ptrsReOffset;
} fapi_ptrs_info_t;

// Updated per 5G FAPI
typedef struct {
    uint8_t                     numPtrsPorts;
    fapi_ptrs_info_t            ptrsInfo[MAX_NUM_PTRS_PORTS];
    uint8_t                     ptrsTimeDensity;
    uint8_t                     ptrsFreqDensity;
    uint8_t                     ulPtrsPower;
} fapi_pusch_ptrs_t;

// Updated per 5G FAPI
typedef struct {
    uint8_t                     lowPaprGroupNumber;
    uint16_t                    lowPaprSequenceNumber;     
    uint8_t                     ulPtrsSampleDensity;
    uint8_t                     ulPtrsTimeDensityTransformPrecoding;
} fapi_dfts_ofdm_t;

// Updated per 5G FAPI
typedef struct { 
    uint16_t                    pduBitMap;
    uint16_t                    rnti;
    uint32_t                    handle;
    uint16_t                    bwpSize;
    uint16_t                    bwpStart;
    uint8_t                     subCarrierSpacing;
    uint8_t                     cyclicPrefix;
    uint16_t                    targetCodeRate;
    uint8_t                     qamModOrder;
    uint8_t                     mcsIndex;
    uint8_t                     mcsTable;
    uint8_t                     transformPrecoding;
    uint16_t                    dataScramblingId;
    uint8_t                     nrOfLayers;
    uint16_t                    ulDmrsSymbPos;
    uint8_t                     dmrsConfigType;
    uint16_t                    ulDmrsScramblingId;
    uint8_t                     scid;
    uint8_t                     numDmrsCdmGrpsNoData;
    uint16_t                    dmrsPorts;
    uint8_t                     resourceAlloc;
    uint8_t                     rbBitmap[36];
    uint16_t                    rbStart;
    uint16_t                    rbSize;
    uint8_t                     vrbToPrbMapping;
    uint8_t                     frequencyHopping;
    uint16_t                    txDirectCurrentLocation;
    uint8_t                     uplinkFrequencyShift7p5khz;
    uint8_t                     startSymbIndex;
    uint8_t                     nrOfSymbols;
    fapi_pusch_data_t           puschData;
    fapi_pusch_uci_t            puschUci;
    fapi_pusch_ptrs_t           puschPtrs;
    fapi_dfts_ofdm_t            dftsOfdm;
    fapi_precoding_bmform_t     beamforming;
} fapi_ul_pusch_pdu_t;

// Updated per 5G FAPI
typedef struct {  
    uint16_t                    rnti;
    uint32_t                    handle;
    uint16_t                    bwpSize;
    uint16_t                    bwpStart;
    uint8_t                     subCarrierSpacing;
    uint8_t                     cyclicPrefix;
    uint8_t                     formatType;
    uint8_t                     multiSlotTxIndicator;
    uint8_t                     pi2Bpsk;
    uint16_t                    prbStart;
    uint16_t                    prbSize;
    uint8_t                     startSymbolIndex;
    uint8_t                     nrOfSymbols;
    uint8_t                     freqHopFlag;
    uint16_t                    secondHopPrb;
    uint8_t                     groupHopFlag;          
    uint8_t                     sequenceHopFlag;
    uint16_t                    hoppingId;
    uint16_t                    initialCyclicShift;
    uint16_t                    dataScramblingId;
    uint8_t                     timeDomainOccIdx;
    uint8_t                     preDftOccIdx;
    uint8_t                     preDftOccLen;
    uint8_t                     addDmrsFlag;
    uint16_t                    dmrsScramblingId;
    uint8_t                     dmrsCyclicShift;
    uint8_t                     srFlag;
    uint8_t                     bitLenHarq;
    uint16_t                    bitLenCsiPart1;
    uint16_t                    bitLenCsiPart2;
    fapi_precoding_bmform_t     beamforming;
} fapi_ul_pucch_pdu_t;

// Updated per 5G FAPI
typedef struct {
    uint16_t                    rnti;
    uint32_t                    handle;
    uint16_t                    bwpsize;
    uint16_t                    bwpstart;
    uint8_t                     subCarrierSpacing;
    uint8_t                     cyclicPrefix;
    uint8_t                     numAntPorts;
    uint8_t                     numSymbols;
    uint8_t                     numRepetitions;
    uint8_t                     timeStartPosition;
    uint8_t                     configIndex;
    uint16_t                    sequenceId;
    uint8_t                     bandwidthIndex;
    uint8_t                     combSize;             
    uint8_t                     combOffset;
    uint8_t                     cyclicShift;
    uint8_t                     frequencyPosition;
    uint8_t                     frequencyShift;
    uint8_t                     frequencyHopping;
    uint8_t                     groupOrSequenceHopping;
    uint8_t                     resourceType;
    uint16_t                    tSrs;
    uint16_t                    tOffset;
    fapi_precoding_bmform_t     beamforming;
} fapi_ul_srs_pdu_t;

// Updated per 5G FAPI
typedef struct {
    uint16_t                    pduType;
    uint16_t                    pduSize;
    union
    {
     fapi_ul_prach_pdu_t        prach_pdu;
     fapi_ul_pusch_pdu_t        pusch_pdu;
     fapi_ul_pucch_pdu_t        pucch_pdu;
     fapi_ul_srs_pdu_t          srs_pdu;
     //fapi_ul_rx_bmform_pdu_t    rx_beamforming_pdu;    //To be defined
    };
    fapi_ue_info_t              ueGrpInfo[MAX_NUMBER_OF_GROUPS_PER_TTI];
} fapi_ul_tti_req_pdu_t;

// Updated per 5G FAPI
typedef struct {
    fapi_msg_t                  header;
    uint16_t                    sfn;
    uint16_t                    slot;
    uint8_t                     nPdus;
    uint8_t                     rachPresent;
    uint8_t                     nUlsch;
    uint8_t                     nUlcch;
    uint8_t                     nGroup;
    fapi_ul_tti_req_pdu_t*      pdus;
} fapi_ul_tti_req_t;

// Updated per 5G FAPI
typedef struct {
    uint16_t                    pduType;
    uint16_t                    pduSize;
    //fapi_dl_pdcch_pdu_t         pdcchPduConfig;      //To be defined
 } fapi_dci_pdu_t; 

 // Updated per 5G FAPI
typedef struct {
    fapi_msg_t                  header;
    uint16_t                    sfn;
    uint16_t                    slot;
    uint8_t                     numPdus;
    fapi_dci_pdu_t*             pdus;
} fapi_ul_dci_req_t;

// Updated per 5G FAPI
typedef struct {
    uint16_t                    pduLength;
    uint16_t                    pduIndex;
    uint32_t                    numTlvs;
    fapi_uint32_tlv_t           tlvs[MAX_NUMBER_OF_TLVS_PER_PDU];
} fapi_tx_pdu_desc_t;

// Updated per 5G FAPI
typedef struct {
    fapi_msg_t                  header; 
    uint16_t                    sfn;
    uint16_t                    slot;
    uint16_t                    numPdus;
    fapi_tx_pdu_desc_t*         pduDesc;
} fapi_tx_data_req_t;

// Updated per 5G FAPI
typedef struct {
    uint32_t                    handle;
    uint16_t                    rnti;
    uint8_t                     harqId;
    uint16_t                    pduLength;
    uint8_t                     ul_cqi;
    uint16_t                    timingAdvance;
    uint16_t                    rssi;
    void*                       pduData;
} fapi_pdu_ind_info_t;
 
 // Updated per 5G FAPI
typedef struct {
    fapi_msg_t                  header;
    uint16_t                    sfn;
    uint16_t                    slot;
    uint16_t                    numPdus;
    fapi_pdu_ind_info_t         pdus[MAX_NUMBER_OF_ULSCH_PDUS_PER_TTI];
} fapi_rx_data_indication_t;

// Updated per 5G FAPI
typedef struct {
    uint32_t                    handle;
    uint16_t                    rnti;
    uint8_t                     harqId;
    uint8_t                     tbCrcStatus;
    uint16_t                    numCb;
    uint8_t                     cbCrcStatus[MAX_NUM_CB_PER_TTI_IN_BYTES];
    uint8_t                     ul_cqi;
    uint16_t                    timingAdvance;
    uint16_t                    rssi;
} fapi_crc_ind_info_t;

// Updated per 5G FAPI
typedef struct {
    fapi_msg_t                  header; 
    uint16_t                    sfn;
    uint16_t                    slot;
    uint16_t                    numCrcs;
    fapi_crc_ind_info_t         crc[MAX_NUMBER_OF_CRCS_PER_SLOT];
} fapi_crc_ind_t;

// Updated per 5G FAPI
typedef struct {
   uint8_t                      harqCrc;
   uint16_t                     harqBitLen;
   uint8_t                      harqPayload[MAX_HARQ_INFO_LEN_BYTES];
} fapi_harq_info_t;

// Updated per 5G FAPI
typedef struct {
    uint8_t                     csiPart1Crc;
    uint16_t                    csiPart1BitLen;
    uint8_t                     csiPart1Payload[MAX_CSI_PART1_DATA_BYTES];
} fapi_csi_p1_info_t;

// Updated per 5G FAPI
typedef struct {
    uint8_t                     csiPart2Crc;
    uint16_t                    csiPart2BitLen;
    uint8_t                     csiPart2Payload[MAX_CSI_PART2_DATA_BYTES];
} fapi_csi_p2_info_t;

// Updated per 5G FAPI
typedef struct {
    uint8_t                     pduBitmap;
    uint32_t                    handle;
    uint16_t                    rnti; 
    uint8_t                     ul_cqi;
    uint16_t                    timingAdvance; 
    uint16_t                    rssi;
//    fapi_harq_info_t            harqInfo; // This is included if indicated by the pduBitmap
//    fapi_csi_p1_info_t          csiPart1info;  // This is included if indicated by the pduBitmap
//    fapi_csi_p2_info_t          csiPart2info;  // This is included if indicated by the pduBitmap
} fapi_uci_o_pusch_t;

// Updated per 5G FAPI
typedef struct {
    uint8_t                     srIndication;
    uint8_t                     srConfidenceLevel;
} fapi_sr_f0f1_info_t;

// Updated per 5G FAPI
typedef struct {
    uint8_t                     numHarq;
    uint8_t                     harqConfidenceLevel;
    uint8_t                     harqValue[MAX_NUMBER_OF_HARQS_PER_IND];
} fapi_harq_f0f1_info_t;

// Updated per 5G FAPI
typedef struct {
    uint16_t                    srBitlen;
    uint8_t                     srPayload[MAX_SR_PAYLOAD_SIZE];
} fapi_sr_f2f3f4_info_t;

// Updated per 5G FAPI
typedef struct {
    uint16_t                    harqCrc;
    uint16_t                    harqBitLen;
    uint8_t                     harqPayload[MAX_HARQ_PAYLOAD_SIZE];
} fapi_harq_f2f3f4_info_t;

// Updated per 5G FAPI
typedef struct { 
    uint8_t                     pduBitmap;
    uint32_t                    handle;
    uint16_t                    rnti;
    uint8_t                     pucchFormat;
    uint8_t                     ul_cqi;
    uint16_t                    timingAdvance;
    uint16_t                    rssi;
 //   fapi_sr_f2f3f4_info_t       srInfo; // This is included if indicated by the pduBitmap
 //   fapi_harq_f2f3f4_info_t     harqInfo; // This is included if indicated by the pduBitmap
 //   fapi_csi_p1_info_t          csiPart1Info; // This is included if indicated by the pduBitmap
 //   fapi_csi_p2_info_t          csiPart2Info; // This is included if indicated by the pduBitmap
} fapi_uci_o_pucch_f2f3f4_t;

// Updated per 5G FAPI
typedef struct {
    uint8_t                     pduBitmap;
    uint32_t                    handle;
    uint16_t                    rnti;
    uint8_t                     pucchFormat;
    uint8_t                     ul_cqi;
    uint16_t                    timingAdvance;
    uint16_t                    rssi;
 //   fapi_sr_f0f1_info_t         srInfo;  // This is included if indicated by the pduBitmap
 //   fapi_harq_f0f1_info_t       harqInfo;  // This is included if indicated by the pduBitmap
} fapi_uci_o_pucch_f0f1_t;

// Updated per 5G FAPI
typedef struct {
    uint16_t                    pduType;
    uint16_t                    pduSize;
    union 
    {
     fapi_uci_o_pusch_t         uciPusch;
     fapi_uci_o_pucch_f0f1_t    uciPucchF0F1;
     fapi_uci_o_pucch_f2f3f4_t  uciPucchF2F3F4;
    };
} fapi_uci_pdu_info_t;

// Updated per 5G FAPI
typedef struct {
    fapi_msg_t                  header;
    uint16_t                    sfn;
    uint16_t                    slot;
    uint16_t                    numUcis;
    fapi_uci_pdu_info_t         uciPdu[MAX_NUMBER_UCI_PDUS_PER_SLOT];
} fapi_uci_indication_t;

// Updated per 5G FAPI
typedef struct {
    uint16_t                    numRbs;
    uint8_t                     rbSNR[MAX_NUMBER_RBS];
} fapi_symb_snr_t;

// Updated per 5G FAPI
typedef struct {
    uint32_t                    handle;
    uint16_t                    rnti;
    uint16_t                    timingAdvance;
    uint8_t                     numSymbols;
    uint8_t                     wideBandSnr;
    uint8_t                     numReportedSymbols;
    fapi_symb_snr_t             symbSnr[MAX_NUMBER_OF_REP_SYMBOLS];
} fapi_srs_pdu_t;

// Updated per 5G FAPI
typedef struct {
    fapi_msg_t                  header;
    uint16_t                    sfn;
    uint16_t                    slot;
    uint8_t                     numPdus;
    fapi_srs_pdu_t              srsPdus[MAX_NUMBER_SRS_PDUS_PER_SLOT];
} fapi_srs_indication_t;

// Updated per 5G FAPI
typedef struct {
    uint8_t                     preambleIndex;
    uint16_t                    timingAdvance;
    uint32_t                    premblePwr;
} fapi_preamble_info_t;

// Updated per 5G FAPI
typedef struct { 
    uint16_t                    physCellId;
    uint8_t                     symbolIndex;
    uint8_t                     slotIndex;
    uint8_t                     freqIndex;
    uint8_t                     avgRssi;
    uint8_t                     avgSnr;
    uint8_t                     numPreamble;
    fapi_preamble_info_t        preambleInfo[MAX_NUM_PREAMBLES_PER_SLOT];  
} fapi_rach_pdu_t;

// Updated per 5G FAPI
typedef struct {
    fapi_msg_t                  header;
    uint16_t                    sfn;
    uint16_t                    slot;
    uint8_t                     numPdus;
    fapi_rach_pdu_t             rachPdu[MAX_NUMBER_RACH_PDUS_PER_SLOT];
} fapi_rach_indication_t;


//------------------------------------------------------------------------------

#if defined(__cplusplus)
}
#endif

#endif
