// This file has been modified by Radisys in order to support 5G FAPI:PHY API Specification
// Document 222.10.01 dated June 2019
// Changes made by luis.farias@intel.com
// Document 222.10.03 dated May 2021

#include <stdint.h>

#ifndef _FAPI_INTERFACE_UE_SIM_H_
#define _FAPI_INTERFACE_UE_SIM_H_

#ifndef PACK_STRUCT
#define PACK_STRUCT                 __attribute__((packed)) // Packed compiler instruction.
#endif //PACK_STRUCT

#if defined(__cplusplus)
extern "C" {
#endif

typedef signed char     int8_t;
typedef unsigned char   uint8_t;
typedef int16_t int16_t;
typedef uint16_t    uint16_t;
typedef int32_t     int32_t;
typedef uint32_t    uint32_t;
#ifdef VZ_VERIPHY_INTEG
typedef uint64_t    uint64_t;
#endif

// Update for 5G FAPI
#define FAPI_PARAM_REQUEST                                  0x00
#define FAPI_PARAM_RESPONSE                                 0x01
#define FAPI_CONFIG_REQUEST                                 0x02
#define FAPI_CONFIG_RESPONSE                                0x03
#define FAPI_START_REQUEST                                  0x04
#ifdef FAPI_222_10_03
#define FAPI_START_RESPONSE                                 0x0108   //Ref: SCF-225
#endif
#define FAPI_STOP_REQUEST                                   0x05
#define FAPI_STOP_INDICATION                                0x06
#define FAPI_ERROR_INDICATION                               0x07
// Reserved 0x08 - 0x7f
#define FAPI_DL_TTI_REQUEST                                 0x80
#define FAPI_UL_TTI_REQUEST                                 0x81
#define FAPI_SLOT_INDICATION                                0x82
#define FAPI_UL_DCI_REQUEST                                 0x83
#define FAPI_TX_DATA_REQUEST                                0x84
#define FAPI_RX_DATA_INDICATION                             0x85
#define FAPI_CRC_INDICATION                                 0x86
#define FAPI_UCI_INDICATION                                 0x87
#define FAPI_SRS_INDICATION                                 0x88
#define FAPI_RACH_INDICATION                                0x89
// Reserved 0x8a -0xff
#ifdef FAPI_222_10_03
#define FAPI_DL_NODE_SYNC                                   0x0180   //Ref: SCF-225
#define FAPI_UL_NODE_SYNC                                   0x0181   //Ref: SCF-225
#define FAPI_TIMING_INFO                                    0x0182   //Ref: SCF-225
#endif
// Tags per 5G FAPI
// Cell Parameters
#define FAPI_RELEASE_CAPABILITY_TAG                         0x0001
#define FAPI_PHY_STATE_TAG                                  0x0002
#define FAPI_SKIP_BLANK_DL_CONFIG_TAG                       0x0003
#define FAPI_SKIP_BLANK_UL_CONFIG_TAG                       0x0004
#define FAPI_NUM_CONFIG_TLVS_TO_REPORT_TYPE_TAG             0x0005
#define FAPI_CYCLIC_PREFIX_TAG                              0x0006
// PDCCH Parameters
#define FAPI_SUPPORTED_SUBCARRIER_SPACING_DL_TAG            0x0007
#define FAPI_SUPPORTED_BANDWIDTH_DL_TAG                     0x0008
#define FAPI_SUPPORTED_SUBCARRIER_SPACING_UL_TAG            0x0009
#define FAPI_SUPPORTED_BANDWIDTH_UL_TAG                     0x000A
#define FAPI_CCE_MAPPING_TYPE_TAG                           0x000B
#define FAPI_CORESET_OUTSIDE_FIRST_3_OFDM_SYMS_OF_SLOT_TAG  0x000c
#define FAPI_PRECODER_GRANULARITY_CORESET_TAG               0x000d
#define FAPI_PDCCH_MU_MIMO_TAG                              0x000e
#define FAPI_PDCCH_PRECODER_CYCLING_TAG                     0x000f
#define FAPI_MAX_PDCCHS_PER_SLOT_TAG                        0x0010
// PUCCH Parameters
#define FAPI_PUCCH_FORMATS_TAG                              0x0011
#define FAPI_MAX_PUCCHS_PER_SLOT_TAG                        0x0012
// PDSCH Parameters
#define FAPI_PDSCH_MAPPING_TYPE_TAG                         0x0013
#define FAPI_PDSCH_ALLOCATION_TYPES_TAG                     0x0014
#define FAPI_PDSCH_VRB_TO_PRB_MAPPING_TAG                   0x0015
#define FAPI_PDSCH_CBG_TAG                                  0x0016
#define FAPI_PDSCH_DMRS_CONFIG_TYPES_TAG                    0x0017
#define FAPI_PDSCH_DMRS_MAX_LENGTH_TAG                      0x0018
#define FAPI_PDSCH_DMRS_ADDITIONAL_POS_TAG                  0x0019
#define FAPI_MAX_PDSCHS_TBS_PER_SLOT_TAG                    0x001a
#define FAPI_MAX_NUMBER_MIMO_LAYERS_PDSCH_TAG               0x001b
#define FAPI_SUPPORTED_MAX_MODULATION_ORDER_DL_TAG          0x001c
#define FAPI_MAX_MU_MIMO_USERS_DL_TAG                       0x001d
#define FAPI_PDSCH_DATA_IN_DMRS_SYMBOLS_TAG                 0x001e
#define FAPI_PREMPTIONSUPPORT_TAG                           0x001f
#define FAPI_PDSCH_NON_SLOT_SUPPORT_TAG                     0x0020
// PUSCH Parameters
#define FAPI_UCI_MUX_ULSCH_IN_PUSCH_TAG                     0x0021
#define FAPI_UCI_ONLY_PUSCH_TAG                             0x0022
#define FAPI_PUSCH_FREQUENCY_HOPPING_TAG                    0x0023
#define FAPI_PUSCH_DMRS_CONFIG_TYPES_TAG                    0x0024
#define FAPI_PUSCH_DMRS_MAX_LEN_TAG                         0x0025
#define FAPI_PUSCH_DMRS_ADDITIONAL_POS_TAG                  0x0026
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
#ifdef FAPI_222_10_03
// PHY Configuration
#define FAPI_PHY_PROFILE_ID_TAG                             0x102a
#define FAPI_INDICATION_INSTANCES_PER_SLOT_TAG              0x102b
#define FAPI_REQUEST_INSTANCES_PER_SLOT_TAG                 0x102c
// Carrier Configuration
#define FAPI_CARRIER_CONFIGURATION_TAG                      0x102d
#else
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
#endif
// Cell Configuration
#define FAPI_PHY_CELL_ID_TAG                                0x100c
#define FAPI_FRAME_DUPLEX_TYPE_TAG                          0x100d
#ifdef FAPI_222_10_03
#define FAPI_PDSCH_TRANS_TYPE_VALIDITY_TAG                  0x102e
#define FAPI_PUSCH_TRANS_TYPE_VALIDITY_TAG                  0x102f
#endif
// SSB Configuration
#define FAPI_SS_PBCH_POWER_TAG                              0x100e
#ifdef FAPI_222_10_03
#define FAPI_SS_PBCH_BLOCK_POWER_SCALING_TAG                0x1030
#define FAPI_BCH_PAYLOAD_FLAG_TAG                           0x100f
#else
#define FAPI_BCH_PAYLOAD_TAG                                0x100f
#define FAPI_SCS_COMMON_TAG                                 0x1010
#endif
// PRACH Configuration
#ifdef FAPI_222_10_03
#define FAPI_PRACH_CONFIGURATION_TAG                        0x1031
#else
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
#endif
#ifdef FAPI_222_10_03
//Multi PRACH Configurations
#define FAPI_MULTI_PRACH_CONFIGURATION_TAG                  0x1032
// SSB Table
#define FAPI_SSB_RESOURCE_CONFIGURATION_TAG                 0x1033
#else
#define FAPI_SSB_OFFSET_POINT_A_TAG                         0x101d
#define FAPI_BETA_PSS_TAG                                   0x101e
#define FAPI_SSB_PERIOD_TAG                                 0x101f
#define FAPI_SSB_SUBCARRIER_OFFSET_TAG                      0x1020
#define FAPI_MIB_TAG                                        0x1021
#define FAPI_SSB_MASK_TAG                                   0x1022
#define FAPI_BEAM_ID_TAG                                    0x1023
#define FAPI_SS_PBCH_MULTIPLE_CARRIERS_IN_A_BAND_TAG        0x1024
#define FAPI_MULTIPLE_CELLS_SS_PBCH_IN_A_CARRIER_TAG        0x1025
#endif

#ifdef FAPI_222_10_03
//Multi SSB Resource Configurations
#define FAPI_MULTI_SSB_RESOURCE_CONFIGURATION_TAG           0x1034

// TDD Table
#define FAPI_TDD_PERIOD_TAG                                 0x1035
#else
#define FAPI_TDD_PERIOD_TAG                                 0x1026
#define FAPI_SLOT_CONFIG_TAG                                0x1027
#endif
// Measurement Configuration
#define FAPI_RSSI_MESUREMENT_TAG                            0x1028
#define FAPI_PRACH_CONFIG_INDEX_TAG                         0x1029
#ifdef FAPI_222_10_03
// UCI Configuration
#define FAPI_UCI_CONFIGURATION_TAG                          0x1036
// PRB-Symbol Rate Match Patterns
#define FAPI_PRB_SYMBOL_RATE_MATCH_PATTERN_TAG              0x0137
// LTE-CRS Rate Match Pattern
#define FAPI_LTE_CRS_RATE_MATCH_PATTERN_TAG                 0x0138
// PUCCH semi-static configuration
#define FAPI_PUCCH_SEMI_STATIC_CONFIG_TAG                   0x1039
// PDSCH Semi-Static Configuration
#define FAPI_PDSCH_CBG_SCHEME                               0x103a
// Delay Management Configuration
#define FAPI_TIMING_WINDOW_TAG                              0x011e
#define FAPI_TIMING_INFO_MODE_TAG                           0x011f
#define FAPI_TIMING_INFO_PERIOD_TAG                         0x0120
// reserve 0x0f00
//Rel-16 mTRP Configurations
#define FAPI_NUM_TX_PORTS_TRP1_TAG                          0x103b
#define FAPI_NUM_RX_PORTS_TRP1_TAG                          0x103c
#endif
//Vendor Specific Tags
#define FAPI_NUM_DIG_BEAMS                                  0xA000
#define FAPI_NUM_TX_RUS                                     0xA001
#define FAPI_BEAM_IDX                                       0xA002
#define FAPI_DIG_BEAM_WEIGHT_RE                             0xA003
#define FAPI_DIG_BEAM_WEIGHT_IM                             0xA004
#define FAPI_PM_IDX                                         0xA005
#define FAPI_NUM_LAYERS                                     0xA006
#define FAPI_NUM_ANT_PORTS                                  0xA007
#define FAPI_PRECODER_WEIGHT_RE                             0xA008
#define FAPI_PRECODER_WEIGHT_IM                             0xA009
// Error Codes updated per 5G FAPI Table 3-31
#define FAPI_MSG_OK                                         0x0
#define FAPI_MSG_INVALID_STATE                              0x1
#define FAPI_MSG_INVALID_CONFIG                             0x2
#define FAPI_MSG_SFN_OUT_OF_SYNC                            0x3
#define FAPI_MSG_SLOT_ERR                                   0x4
#define FAPI_MSG_BCH_MISSING                                0x5
#define FAPI_MSG_INVALID_SFN                                0x6
#define FAPI_MSG_UL_DCI_ERR                                 0x7
#define FAPI_MSG_TX_ERR                                     0x8
#ifdef FAPI_222_10_03
#define MSG_INVALID_PHY_ID				    0x9
#define MSG_UNINSTANTIATED_PHY				    0xA
#define MSG_INVALID_DFE_Profile 			    0xB
#define PHY_Profile_Selection_incompatible                  0xC
#endif
#define FAPI_MAX_NUM_CW                                     1
#define FAPI_MAX_FREQ_DOMAIN_RES                            6

#ifdef FAPI_222_10_03
#define FAPI_PROTOCOLVERSION_TAG			    0X0037
#define FAPI_POWER_PROFILES_SUPPORTED_TAG		    0X0038
#define FAPI_MAXNUMPDUS_INDL_TTI_TAG			    0X0039
#define FAPI_MAXNUMPDUS_INUL_TTI_TAG			    0X003A
#define FAPI_MAXNUMPDUS_INUL_DCI_TAG			    0X003B
#define FAPI_SSPBCH_MULTIPLE_CARRIERS_IN_A_BAND_TAG         0x003C 
#define FAPI_MULTIPLE_CELLS_SSPBCH_IN_A_CARRIER_TAG         0x003D
#define FAPI_PUCCHGROUP_AND_SEQUENCE_HOPPING_TAG            0x003E
#define FAPI_MAXNUM_UlBWPIDS_TAG			    0x003F
#define FAPI_PUCCH_AGGREGATION_TAG			    0x0040 
#define FAPI_SSB_RATE_MATCH_TAG				    0x0041
#define FAPI_SUPPORTED_RATE_MATCH_PATTERN_TYPE_TAG	    0x0042
#define FAPI_PDCCH_RATE_MATCH_TAG			    0x0043
#define FAPI_NUM_OF_RATE_MATCH_PATTERNLTECRCPERSLOT_TAG	    0x0044
#define FAPI_NUMOFRATE_MATCH_PATTERN_LTECRC_INPHY_TAG	    0x0045
#define FAPI_CSI_RS_RATE_MATCH_TAG			    0x0046
#define FAPI_PDSCH_TRANS_TYPE_SUPPORT_TAG                   0x0047
#define FAPI_PDSCH_MAC_PDU_BIT_ALIGNMENT_TAG                0x0048
#define FAPI_MAX_NUMBER_MIMO_LAYERS_CB_PUSCH_TAG	    0x0049
#define FAPI_PUSCH_LBRM_SUPPORT_TAG			    0x004A
#define FAPI_PUSCH_TRANS_TYPE_SUPPORT_TAG		    0x004B
#define FAPI_PUSCH_MAC_PDU_BIT_ALIGNMENT_TAG		    0x004C
#define FAPI_MAXNUM_PRACH_CONFIGURATIONS_TAG		    0x004D
#define FAPI_PRACH_MULTIPLE_CARRIERS_IN_A_Band_TAG          0x004E
#define FAPI_MAX_NUM_UCIMAPS_TAG			    0x004F
#define FAPI_CAPABILITY_VALIDITY_TAG			    0x0050
#define FAPI_PHY_PARAMS_TAG				    0x0051
#define FAPI_TIME_MANAGEMENT_TAG			    0x0052
#define	FAPI_PHY_PROTOCOL_VERSION_TAG			    0x0053
#define FAPI_DFE_PROFILE_TAG				    0x0054
#define FAPI_MORE_THAN_ONE_INDICATION_PER_SLOT_TAG	    0x0055
#define FAPI_MORE_THAN_ONE_REQUEST_PER_SLOT_TAG		    0x0056
#define FAPI_MTRP_SUPPORT_TAG				    0x0057
#define FAPI_DL_TTI_TIMING_OFFSET_TAG			    0X0106
#define FAPI_UL_TTI_TIMING_OFFSET_TAG                       0X0107
#define FAPI_UL_DCI_TIMING_OFFSET_TAG                       0X0108
#define FAPI_TX_DATA_TIMING_OFFSET_TAG                      0X0109
#endif
// TODO : Work out what the correct maximums should be// Needs Review for 5G
#if 0
// Number of UL/DL configurations, I, as defined by 36.212 section 5.3.3.1.4
// todo : work out what the max is
#define FAPI_MAX_UL_DL_CONFIGURATIONS                       4
#define FAPI_MAX_NUM_PHYSICAL_ANTENNAS                      4
#define FAPI_MAX_NUM_SCHEDULED_UES                          8
#define FAPI_MAX_NUM_SUBBANDS                               8
#define FAPI_MAX_ANTENNA_PORT_COUNT                         2
#endif

// 5G FAPI Definitions
#define NUMEROLOGIES                                        5
#define MAX_NUM_UNUSED_ROOT_SEQUENCES                       63 // 38.331 page 383
#define MAX_NUM_PRACH_FD_OCCASIONS                          64 // 38.331 page 383
#define MAX_NUM_OF_SYMBOLS_PER_SLOT                         14
#define MAX_TDD_PERIODICITY                                 160// 38.212 11.1 for u = 4 and P=10 ms
#define MAX_NUMBER_TX_RUS                                   4  // m = p*q with p number of panels and q number of TxRU/RxRU per panel, depends on
// the RF configuration, currently n = m = 4, q = 1, p = 4 and k = 21 (number of beams per pannel). n number of antenna ports
#define MAX_NUMBER_OF_BEAMS                                 64 // Intel API Page 27
#define MAX_NUM_ANT_PORTS                                   8  // Based on current RF
#define MAX_NUM_LAYERS                                      8  // 38.211 Table 7.3.1.3-1
#ifdef VZ_VERIPHY_INTEG
#define MAX_NUM_TLVS_CELL_CONFIG                            46  // 5G FAPI Table 3-9  (A)
#else
#define MAX_NUM_TLVS_CELL_CONFIG                            2  // 5G FAPI Table 3-9  (A)
#endif
#define MAX_NUM_TLVS_CARRIER_CONFIG                         27 // 5G FAPI Table 3-10 (B)
#define MAX_NUM_TLVS_PDCCH_CONFIG                           6  // 5G FAPI Table 3-11 (C)
#define MAX_NUM_TLVS_PUCCH_CONFIG                           2  // 5G FAPI Table 3-12 (D)
#define MAX_NUM_TLVS_PDSCH_CONFIG                           14 // 5G FAPI Table 3-13 (E)
#define MAX_NUM_TLVS_PUSCH_CONFIG                           17 // 5G FAPI Table 3-14 (F)
#define MAX_NUM_TLVS_PRACH_CONFIG                           4  // 5G FAPI Table 3-15 (G)
#define MAX_NUM_TLVS_MEAS_CONFIG                            1  // 5G FAPI Table 3-16 (H)
#ifdef VZ_VERIPHY_INTEG
#define MAX_NUM_TLVS_CONFIG                                 117 //  A+B+C+D+E+F+G+H + Padding
#else
#define MAX_NUM_TLVS_CONFIG                                 74 //  A+B+C+D+E+F+G+H + Padding
#endif
#define MAX_NUMBER_UNSUPPORTED_TLVS                         74
#define MAX_NUMBER_OF_INVALID_IDLE_ONLY_TLVS                74
#define MAX_NUMBER_OF_INVALID_RUNNING_ONLY_TLVS             74
#define MAX_NUMBER_OF_MISSING_TLVS                          74
#define MAX_NUM_DIGBFINTERFACES                             4 // Based on RF, 5G FAPI says {0, 255}
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
#define TA_AVG_TIME                                         32
#define TA_AVG_TIMElog2                                     5
#ifdef FAPI_222_10_03
#define MAX_NDLTYPES_DL_TTI_REQ                             5
#define MAX_NDLTYPES_UL_DCI_REQ                             2
#define MAX_SIGNAL_INDEX                                    7
#define MAX_NUM_UL_TYPES                                    5
#endif

typedef enum nr_ssb_periodicity
{
    FAPI_NR_SSB_PERIODICITY_5MS,
    FAPI_NR_SSB_PERIODICITY_10MS,
    FAPI_NR_SSB_PERIODICITY_20MS,
    FAPI_NR_SSB_PERIODICITY_40MS,
    FAPI_NR_SSB_PERIODICITY_80MS,
    FAPI_NR_SSB_PERIODICITY_160MS

} nr_ssb_periodicity_t;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_msg
{
    uint16_t                     message_type_id;
    uint32_t                    length; // Length of the message body in bytes
} fapi_msg_t_ue_sim;
// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_msg_header
{
    uint8_t                     numberOfMessagesIncluded;
    uint8_t                     handle;   // Can be used for Phy Id or Carrier Id
    fapi_msg_t_ue_sim           msg_details;
} fapi_msg_header_t_ue_sim;
// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_tl
{
    uint16_t                    tag;
    uint16_t                    length;
} fapi_tl_t_ue_sim;
// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_uint8_tlv
{
    fapi_tl_t_ue_sim            tl;
    uint8_t                     value;
#ifdef VZ_VERIPHY_INTEG
    uint8_t                     rsv[3];
#endif
} fapi_uint8_tlv_t_ue_sim;
// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_uint16_tlv
{
    fapi_tl_t_ue_sim            tl;
    uint16_t                    value;
#ifdef VZ_VERIPHY_INTEG
    uint8_t rsv[2];
#endif
} fapi_uint16_tlv_t_ue_sim;
// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_int16_tlv
{
    fapi_tl_t_ue_sim            tl;
    int16_t                     value;
#ifdef VZ_VERIPHY_INTEG
    uint8_t rsv[2];
#endif
} fapi_int16_tlv_t_ue_sim;
// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_uint32_tlv
{
    fapi_tl_t_ue_sim            tl;
    uint32_t                    value;
} fapi_uint32_tlv_t_ue_sim;
// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_tx_dat_tlv
{
    uint16_t                    tag;
    //Currently DU is not supporting more than 1 TLVs. So length type changed to uint32_t as per DU team. To be modified once it is resolved.
    uint32_t                    length;
#ifdef VZ_VERIPHY_INTEG	
    uint64_t                    valueOrOffset;         // TLV with unsigned 32 bit value
#else
    uint32_t                    value[];
#endif

} fapi_tx_dat_tlv_t;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_config_tlv
{
    uint16_t                    tag;                // In 5G FAPI for Cell Params
    uint8_t                     length;
    uint8_t                     value;
} fapi_config_tlv_t_ue_sim;

#ifdef VZ_VERIPHY_INTEG
// Updated per 5G FAPI
typedef struct {
	fapi_tl_t_ue_sim            tl;
	uint16_t                    value[NUMEROLOGIES];
	uint16_t                    rsv;			// To be 32-bit aligned, if FAPI_NUMEROLOGIES changes to some other value than 5 please ensure 32 bit alignment
} fapi_config_num_tlv_t;
#endif

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_param_req
{
    fapi_msg_header_t_ue_sim    header;  // For PARAM.req message length in fapi_msg_t_ue_sim is zero
#ifdef FAPI_222_10_03
    fapi_uint8_tlv_t_ue_sim            protocolVersion;
#endif
} fapi_param_req_t_ue_sim;

#ifdef FAPI_222_10_03
typedef struct PACK_STRUCT fapi_signal_index
{
    uint8_t                     referenceRS;
    int16_t                     powerOffsetMin;
    int16_t                     powerOffsetMax;
} fapi_signal_index_t;
#endif
// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_cell_params
{
    fapi_uint16_tlv_t_ue_sim           releaseCapability;
    fapi_uint16_tlv_t_ue_sim           phyState;
    fapi_uint8_tlv_t_ue_sim            skipBlankDlConfig;
    fapi_uint8_tlv_t_ue_sim            skipBlankUlConfig;
    fapi_uint16_tlv_t_ue_sim           numTlvsToReport;
    //fapi_param_tlv_t            tlvStatus[MAX_NUMBER_OF_CONFIG_PARMS];          // Need to define this value based on 5G FAPI
#ifdef FAPI_222_10_03
    fapi_uint8_tlv_t_ue_sim            powerProfilesSupported;
    fapi_signal_index_t         signalIndex[MAX_SIGNAL_INDEX];
    fapi_uint16_tlv_t_ue_sim           maxNumPDUsInDL_TTI;
    fapi_uint16_tlv_t_ue_sim           maxNumPDUsInUL_TTI;
    fapi_uint16_tlv_t_ue_sim           maxNumPDUsInUL_DCI; 
#endif
} fapi_cell_parms_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_carrier_parms
{
    fapi_uint8_tlv_t_ue_sim            cyclicPrefix;
    fapi_uint8_tlv_t_ue_sim            supportedSubcarrierSpacingDl;
    fapi_uint16_tlv_t_ue_sim           supportedBandwidthDl;
    fapi_uint8_tlv_t_ue_sim            supportedSubcarrierSpecingsUl;
    fapi_uint16_tlv_t_ue_sim           supportedBandwidthUl;
#ifdef FAPI_222_10_03
    fapi_uint8_tlv_t_ue_sim            ssPbchMultipleCarriersInABand;
    fapi_uint8_tlv_t_ue_sim            multipleCellsSsPbchInACarrier;
#endif
}   fapi_carrier_parms_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_pdcch_parms
{
    fapi_uint8_tlv_t_ue_sim            cceMappingType;
    fapi_uint8_tlv_t_ue_sim            coresetOutsideFirst3OfdmSymsOfSlot;
    fapi_uint8_tlv_t_ue_sim            precoderGranularityCoreset;
    fapi_uint8_tlv_t_ue_sim            pdcchMuMimo;
    fapi_uint8_tlv_t_ue_sim            pdcchPrecoderCycling;
    fapi_uint8_tlv_t_ue_sim            maxPdcchsPerSlot;
}   fapi_pdcch_parms_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_pucch_parms
{
    fapi_uint8_tlv_t_ue_sim            pucchFormats;
    fapi_uint8_tlv_t_ue_sim            maxPucchsPerSlot;
#ifdef FAPI_222_10_03
    fapi_uint8_tlv_t_ue_sim            pucchGroupAndSequenceHopping;
    fapi_uint8_tlv_t_ue_sim            maxNumUlBwpIds;
    fapi_uint8_tlv_t_ue_sim            pucchAggregation;
#endif
}   fapi_pucch_parms_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_pdsch_parms
{
    fapi_uint8_tlv_t_ue_sim            pdschMappingType;
    fapi_uint8_tlv_t_ue_sim            pdschAllocationTypes;
    fapi_uint8_tlv_t_ue_sim            pdschVrbToPrbMapping;
    fapi_uint8_tlv_t_ue_sim            pdschCbg;
    fapi_uint8_tlv_t_ue_sim            pdschDmrsConfigTypes;
    fapi_uint8_tlv_t_ue_sim            pdschDmrsMaxLength;
    fapi_uint8_tlv_t_ue_sim            pdschDmrsAdditionalPos;
    fapi_uint8_tlv_t_ue_sim            maxPdschsTBsPerSlot;
    fapi_uint8_tlv_t_ue_sim            maxNumberMimoLayersPdsch;
    fapi_uint8_tlv_t_ue_sim            supportedMaxModulationOrderDl;
    fapi_uint8_tlv_t_ue_sim            maxMuMimoUsersDl;
    fapi_uint8_tlv_t_ue_sim            pdschDataInDmrsSymbols;
    fapi_uint8_tlv_t_ue_sim            premptionSupport;
    fapi_uint8_tlv_t_ue_sim            pdschNonSlotSupport;
#ifdef FAPI_222_10_03
    fapi_uint8_tlv_t_ue_sim		ssbRateMatch;
    fapi_uint8_tlv_t_ue_sim		supportedRateMatchPatternType;
    fapi_uint8_tlv_t_ue_sim		pdcchRateMatch; 
    fapi_uint8_tlv_t_ue_sim		numOfRateMatchPatternLTECrsPerSlot;
    fapi_uint8_tlv_t_ue_sim		numOfRateMatchPatternLTECrsInPhy;
    fapi_uint8_tlv_t_ue_sim		csiRsRateMatch;
    fapi_uint8_tlv_t_ue_sim		pdschTransTypeSupport;
    fapi_uint8_tlv_t_ue_sim		pdschMacPduBitAlignment;
#endif
}   fapi_pdsch_parms_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_pusch_parms
{
    fapi_uint8_tlv_t_ue_sim            uciMuxUlschInPusch;
    fapi_uint8_tlv_t_ue_sim            uciOnlyPusch;
    fapi_uint8_tlv_t_ue_sim            puschFrequencyHopping;
    fapi_uint8_tlv_t_ue_sim            puschDmrsConfigTypes;
    fapi_uint8_tlv_t_ue_sim            puschDmrsMaxLen;
    fapi_uint8_tlv_t_ue_sim            puschDmrsAditionalPos;
    fapi_uint8_tlv_t_ue_sim            puschCbg;
    fapi_uint8_tlv_t_ue_sim            puschMappingType;
    fapi_uint8_tlv_t_ue_sim            puschAllocationTypes;
    fapi_uint8_tlv_t_ue_sim            puschVrbToPrbMapping;
    fapi_uint8_tlv_t_ue_sim            puschMaxPtrsPorts;
    fapi_uint8_tlv_t_ue_sim            maxPduschsTBsPerSlot;
    fapi_uint8_tlv_t_ue_sim            maxNumberMimoLayersnonCbPusch;
#ifdef FAPI_222_10_03
    fapi_uint8_tlv_t_ue_sim		maxNumberMimoLayersCbPusch;
#endif
    fapi_uint8_tlv_t_ue_sim            supportedModulationOrderUl;
    fapi_uint8_tlv_t_ue_sim            maxMuMimoUsersUl;
    fapi_uint8_tlv_t_ue_sim            dftsOfdmSupport;
    fapi_uint8_tlv_t_ue_sim            puschAggregationFactor;
#ifdef FAPI_222_10_03
    fapi_uint8_tlv_t_ue_sim            puschLbrmSupport;
    fapi_uint8_tlv_t_ue_sim            puschTransTypeSupport;
    fapi_uint8_tlv_t_ue_sim            puschMacPduBitAlignment;
#endif   
}fapi_pusch_parms_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_prach_parms
{
    fapi_uint8_tlv_t_ue_sim            prachLongFormats;
    fapi_uint8_tlv_t_ue_sim            prachShortFormats;
    fapi_uint8_tlv_t_ue_sim            prachRestrictedSets;
    fapi_uint8_tlv_t_ue_sim            maxPrachFdOccasionsInASlot;
#ifdef FAPI_222_10_03
    fapi_uint16_tlv_t_ue_sim           maxNumPrachConfigurations;
    fapi_uint8_tlv_t_ue_sim            prachMultipleCarriersInABand;
#endif
}fapi_prach_parms_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_meas_parms
{
    fapi_uint8_tlv_t_ue_sim            rssiMeasurementSupport;
}   fapi_meas_parms_t_ue_sim;
#ifdef FAPI_222_10_03
typedef struct PACK_STRUCT fapi_uci_parms
{
    fapi_uint16_tlv_t_ue_sim           maxNumUciMaps;
}   fapi_uci_parms_t;
typedef struct PACK_STRUCT fapi_capability
{
    uint16_t	                capabilityTag;
    uint8_t	                validityScope;
}   fapi_capability_t;
typedef struct PACK_STRUCT fapi_capability_validity
{
    fapi_tl_t_ue_sim            fapi_capability_validity;
    uint16_t                    numCapabilities;
    fapi_capability_t	        capability[];
}   fapi_capability_validity_t;
typedef struct PACK_STRUCT fapi_phy_port
{
    uint8_t                     numDlPortRanges; /* Number of DL port ranges for this PHYs */
    uint16_t                    dlPortRangeStart[0];  /* Start of each range of DL ports, depends on Number of DL port ranges for this PHYs*/
    uint16_t                    dlPortRangeLen[0];
    uint8_t                     numUlPortRanges;
    uint16_t                    ulPortRangeStart[0];
    uint16_t                    ulPortRangeLen[0]; 
}fapi_phy_port_t;
typedef struct PACK_STRUCT fapi_phy_profile
{
    uint8_t		        maxNumPhys;
    fapi_phy_port_t	        phy_port[0];
}fapi_phy_profile_t;
typedef struct PACK_STRUCT fapi_phy_parms
{
    fapi_tl_t_ue_sim      phySupport;
    uint16_t		        numPhyProfiles;
    uint16_t		        numDlBbPorts;
    uint16_t	           numUlBbPorts;
    fapi_phy_profile_t    phyProfiles[0];
    fapi_uint8_tlv_t_ue_sim	     timeManagement;
    fapi_tl_t_ue_sim		  protocolVersion;
    uint8_t		           phyFapiProtocolVersion;
    uint8_t		           phyFapiNegotiatedProtocolVersion;
    fapi_uint8_tlv_t_ue_sim	     moreThanOneIndicationPerSlot[6];
    fapi_uint8_tlv_t_ue_sim	     moreThanOneRequestPerSlot[4];         
}fapi_phy_support_t;
typedef struct PACK_STRUCT fapi_dfe_profile
{
    fapi_tl_t_ue_sim	       dfe_profile;
    uint16_t	             numPhyProfiles;
    uint16_t	             numDfeProfiles;
    uint8_t	                profileValidityMap;
}fapi_dfe_profile_t;
typedef struct PACK_STRUCT fapi_delay_management_parms
{
    fapi_uint32_tlv_t_ue_sim	        dl_tti_timing_offset;
    fapi_uint32_tlv_t_ue_sim           ul_tti_timing_offset;
    fapi_uint32_tlv_t_ue_sim           ul_dci_timing_offset;
    fapi_uint32_tlv_t_ue_sim           tx_data_timing_offset;
    fapi_uint16_tlv_t_ue_sim           timing_window;
    fapi_uint8_tlv_t_ue_sim            timing_info_period;
}fapi_delay_management_parms_t;
typedef struct PACK_STRUCT fapi_rel16_mTRP_parameters
{
    fapi_uint32_tlv_t_ue_sim	        mTRP_Support;	
}fapi_rel16_mTRP_parameters_t;
#endif
// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_params
{
    fapi_cell_parms_t_ue_sim           	cellParms;
    fapi_carrier_parms_t_ue_sim        	carrParms;
    fapi_pdcch_parms_t_ue_sim          	pdcchParms;
    fapi_pucch_parms_t_ue_sim          	pucchParms;
    fapi_pdsch_parms_t_ue_sim          	pdschParms;
    fapi_pusch_parms_t_ue_sim          	puschParms;
    fapi_prach_parms_t_ue_sim          	prachParms;
    fapi_meas_parms_t_ue_sim           	measParms;
#ifdef FAPI_222_10_03
    fapi_uci_parms_t                    uciParms;
    fapi_capability_validity_t  	capabilityValidity;
    fapi_phy_support_t                  phySupport;
    fapi_dfe_profile_t                  dfeProfile;
    fapi_delay_management_parms_t	delayManagementParms;
    fapi_rel16_mTRP_parameters_t	rel16mTRPParameters;
#endif
} fapi_params_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_param_resp
{
    fapi_msg_header_t_ue_sim    header;
    uint8_t                     error_code;
    uint8_t                     number_of_tlvs;
    fapi_uint16_tlv_t_ue_sim           tlvs[MAX_NUM_TLVS_CONFIG];
} fapi_param_resp_t_ue_sim;

// Updated per 5G FAPI
#ifdef FAPI_222_10_03
typedef struct PACK_STRUCT fapi_phy_config
{
    fapi_uint16_tlv_t_ue_sim           phyProfileId;
    fapi_uint8_tlv_t_ue_sim            indicationInstancePerSlot[6];
    fapi_uint8_tlv_t_ue_sim            requestInstancePerSlot[4];
}fapi_phy_config_t;
#endif
// Updated per 5G FAPI
#ifdef FAPI_222_10_03
typedef struct PACK_STRUCT fapi_carrier_config
{
    fapi_tl_t_ue_sim            carrierConfig;
    uint16_t                    dlBandwidth;
    uint32_t                    dlFrequency;
#ifdef VZ_VERIPHY_INTEG
    fapi_config_num_tlv_t       dlk0;
    fapi_config_num_tlv_t       dlGridSize;
#else
    uint16_t                    dlk0[NUMEROLOGIES];
    uint16_t                    dlGridSize[NUMEROLOGIES];
#endif
    uint16_t                    numTxAnt;
    uint16_t                    uplinkBandwidth;
    uint32_t                    uplinkFrequency;
#ifdef VZ_VERIPHY_INTEG
    fapi_config_num_tlv_t       ulk0;
    fapi_config_num_tlv_t       ulGridSize;
#else
    uint16_t                    ulk0[NUMEROLOGIES];
    uint16_t                    ulGridSize[NUMEROLOGIES];
#endif	
    uint16_t                    numRxAnt;
    uint8_t                     frequencyShift7p5KHz;
    uint8_t                     powerProfile;
    uint8_t                     powerOffsetRsIndex;
} fapi_carrier_config_t_ue_sim;
#else
typedef struct PACK_STRUCT fapi_carrier_config
{
    fapi_uint16_tlv_t_ue_sim           dlBandwidth;
    fapi_uint32_tlv_t_ue_sim           dlFrequency;
#ifdef VZ_VERIPHY_INTEG
    fapi_config_num_tlv_t	dlk0;
    fapi_config_num_tlv_t	dlGridSize;
#else
    fapi_uint16_tlv_t_ue_sim           dlk0[NUMEROLOGIES];
    fapi_uint16_tlv_t_ue_sim           dlGridSize[NUMEROLOGIES];
#endif
    fapi_uint16_tlv_t_ue_sim           numTxAnt;
    fapi_uint16_tlv_t_ue_sim           uplinkBandwidth;
    fapi_uint32_tlv_t_ue_sim           uplinkFrequency;
#ifdef VZ_VERIPHY_INTEG
    fapi_config_num_tlv_t       ulk0;
    fapi_config_num_tlv_t       ulGridSize;
#else
    fapi_uint16_tlv_t_ue_sim           ulk0[NUMEROLOGIES];
    fapi_uint16_tlv_t_ue_sim           ulGridSize[NUMEROLOGIES];
#endif	
    fapi_uint16_tlv_t_ue_sim           numRxAnt;
    fapi_uint8_tlv_t_ue_sim            frequencyShift7p5KHz;
} fapi_carrier_config_t_ue_sim;
#endif

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_cell_config
{
    fapi_uint16_tlv_t_ue_sim           phyCellId;      //in legacy data type mentioned was uint8_t, though in spec it was uint16_t
    fapi_uint8_tlv_t_ue_sim            frameDuplexType;
#ifdef FAPI_222_10_03
    fapi_uint8_tlv_t_ue_sim            pdschTransTypeValidity;
    fapi_uint8_tlv_t_ue_sim            puschTransTypeValidity;
#endif
} fapi_cell_config_t_ue_sim;

// Updated per 5G FAPI
#ifdef FAPI_222_10_03
typedef struct PACK_STRUCT fapi_ssb_pwr_pbch_config
{
    fapi_uint32_tlv_t_ue_sim           ssPbchPower;
    fapi_int16_tlv_t_ue_sim            ssPbchBlockPowerScaling;
    fapi_uint8_tlv_t_ue_sim            bchPayloadFlag;
}fapi_ssb_pwr_pbch_config_t;
#else
typedef struct PACK_STRUCT fapi_ssb_config
{
    fapi_uint32_tlv_t_ue_sim           ssPbchPower;
    fapi_uint8_tlv_t_ue_sim            bchPayload;
    fapi_uint8_tlv_t_ue_sim            scsCommon;
} fapi_ssb_config_t_ue_sim;
#endif

// Updated per 5G FAPI
#ifdef FAPI_222_10_03
typedef struct PACK_STRUCT fapi_prachFdOccasion
{
    uint16_t                    prachRootSequenceIndex;
    uint8_t                     numRootSequences;
    int16_t                     k1;
    uint8_t                     prachZeroCorrConf;
    uint16_t                    numUnusedRootSequences;   // 5G FAPI Table 3-24 Subset
    uint16_t                    unusedRootSequences[MAX_NUM_UNUSED_ROOT_SEQUENCES];
} fapi_prachFdOccasion_t_ue_sim;
#else
typedef struct PACK_STRUCT fapi_prachFdOccasion
{
    fapi_uint16_tlv_t_ue_sim           prachRootSequenceIndex;
    fapi_uint8_tlv_t_ue_sim            numRootSequences;
    fapi_uint16_tlv_t_ue_sim           k1;
    fapi_uint8_tlv_t_ue_sim            prachZeroCorrConf;
    fapi_uint16_tlv_t_ue_sim           numUnusedRootSequences;   // 5G FAPI Table 3-24 Subset
    fapi_uint8_tlv_t_ue_sim            unusedRootSequences[MAX_NUM_UNUSED_ROOT_SEQUENCES];
} fapi_prachFdOccasion_t_ue_sim;
#endif
// Updated per 5G FAPI
#ifdef FAPI_222_10_03
typedef struct PACK_STRUCT fapi_prach_configuration
{
    fapi_tl_t_ue_sim            prachConfig;
    uint16_t                    prachResConfigIndex;
    uint8_t                     prachSequenceLength;
    uint8_t                     prachSubCSpacing;
    uint8_t                     ulBwpPuschScs;
    uint8_t                     restrictedSetConfig;
    uint8_t                     numPrachFdOccasions;
    uint8_t                     prachConfigIndex;
    fapi_prachFdOccasion_t_ue_sim      prachFdOccasion[MAX_NUM_PRACH_FD_OCCASIONS];
    uint8_t                     ssbPerRach;
} fapi_prach_configuration_t_ue_sim;
#else
typedef struct PACK_STRUCT fapi_prach_configuration
{
    fapi_uint8_tlv_t_ue_sim            prachSequenceLength;
    fapi_uint8_tlv_t_ue_sim            prachSubCSpacing;
    fapi_uint8_tlv_t_ue_sim            restrictedSetConfig;
    fapi_uint8_tlv_t_ue_sim            numPrachFdOccasions;
    fapi_uint8_tlv_t_ue_sim            prachConfigIndex;
    fapi_prachFdOccasion_t_ue_sim      prachFdOccasion[MAX_NUM_PRACH_FD_OCCASIONS];
    fapi_uint8_tlv_t_ue_sim            ssbPerRach;
    fapi_uint8_tlv_t_ue_sim            prachMultipleCarriersInABand;
} fapi_prach_configuration_t_ue_sim;
#endif

#ifdef FAPI_222_10_03
// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_multi_prach_config
{
    fapi_tl_t_ue_sim            multiPrachConfig;
    uint16_t                    numPrachConfigurations;
    fapi_prach_configuration_t_ue_sim  numPrachConfigurationsTlv[]; 
}fapi_multi_prach_config_t;
#endif

//Updated per 5G FAPI
#ifdef FAPI_222_10_03
typedef struct PACK_STRUCT fapi_ssb_res_config_table
{
    fapi_tl_t_ue_sim            ssbTableConfig;
    uint16_t                    ssbConfigIndex;
    uint16_t                    ssbOffsetPointA;
    uint8_t                     betaPssProfileNR;
    int16_t                     betaPssProfileSSS;
    uint8_t                     ssbPeriod;
    uint8_t                     ssbSubcarrierOffset;
    uint8_t                     cases;
    uint8_t                     subCarrierSpacing;
    uint8_t                     subCarrierSpacingCommon;
    uint32_t                    ssbMask[2];
    uint8_t                     beamId[64];
    uint8_t                     imax;
    uint8_t                     rmsiPresence;
}fapi_ssb_res_config_table_t;
#else
typedef struct PACK_STRUCT fapi_ssb_table
{
    fapi_uint16_tlv_t_ue_sim           ssbOffsetPointA;
    fapi_uint8_tlv_t_ue_sim            betaPss;
    fapi_uint8_tlv_t_ue_sim            ssbPeriod;
    fapi_uint8_tlv_t_ue_sim            ssbSubCarrierOffset;
    fapi_uint32_tlv_t_ue_sim           mib;
    fapi_uint32_tlv_t_ue_sim           ssbMask[2];
    fapi_uint8_tlv_t_ue_sim            beamId[64];
    fapi_uint8_tlv_t_ue_sim            ssPbchMultipleCarriersInABand;
    fapi_uint8_tlv_t_ue_sim            multipleCellsSsPbchInACarrier;
} fapi_ssb_table_t_ue_sim;
#endif

#ifdef FAPI_222_10_03
//Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_multi_ssb_resource_config_table
{
    fapi_tl_t_ue_sim            multiSsbResourceConfig;
    uint8_t                     numSsbConfigurations;
    fapi_ssb_res_config_table_t ssbConfigurationsTLVs[];
}fapi_multi_ssb_resource_config_table_t;
#endif

// Updated per 5G FAPI
#ifdef FAPI_222_10_03
typedef struct PACK_STRUCT fapi_slotconfig
{
    uint8_t                     slotConfig[MAX_NUM_OF_SYMBOLS_PER_SLOT];
} fapi_slotconfig_t_ue_sim;
#else
typedef struct PACK_STRUCT fapi_slotconfig
{
    fapi_uint8_tlv_t_ue_sim            slotConfig[MAX_NUM_OF_SYMBOLS_PER_SLOT];
} fapi_slotconfig_t_ue_sim;
#endif

// Updated per 5G FAPI
#ifdef FAPI_222_10_03
typedef struct PACK_STRUCT fapi_tdd_table
{
    fapi_tl_t_ue_sim            tddTableConfig;
    uint8_t                     tddPeriod;
    fapi_slotconfig_t_ue_sim           slotConfig[MAX_TDD_PERIODICITY];
} fapi_tdd_table_t_ue_sim;
#else
typedef struct PACK_STRUCT fapi_tdd_table
{
    fapi_uint8_tlv_t_ue_sim            tddPeriod;
    fapi_slotconfig_t_ue_sim           slotConfig[MAX_TDD_PERIODICITY];
} fapi_tdd_table_t_ue_sim;
#endif

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_meas_config
{
    fapi_uint8_tlv_t_ue_sim            rssiMeasurement;
} fapi_meas_config_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_dig_beam_weight
{
    int16_t                     digBeamWeightRe;
    int16_t                     digBeamWeightIm;
} fapi_dig_beam_weight_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_dig_beam_config
{
    uint16_t                    beamIdx;
    fapi_dig_beam_weight_t_ue_sim      digBeamWeight[MAX_NUMBER_TX_RUS];
} fapi_dig_beam_config_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_beamforming_table
{
    uint16_t                    numDigBeams;
#ifndef FAPI_222_10_03
    uint16_t                    numTxRus;
#else
    uint16_t                    numBasebandPorts;
#endif
    fapi_dig_beam_config_t_ue_sim      digBeam[MAX_NUMBER_OF_BEAMS];
} fapi_beamforming_table_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_precoderWeight
{
    int16_t                     preCoderWeightRe;
    int16_t                     preCoderWeightIm;
} fapi_precoderWeight_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_precoder_weight
{
    fapi_precoderWeight_t_ue_sim       precoder_weight[MAX_NUM_ANT_PORTS];
} fapi_precoder_weight_t_ue_sim;

#ifdef FAPI_222_10_03
typedef struct PACK_STRUCT fapi_map_index
{
    uint8_t                     numPart1Params;
    uint8_t                     sizePart1Params[0]; /* Shall Not exced 12 */
    uint16_t                    map[0];
}fapi_map_index_t;
// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_uci_config
{
    fapi_tl_t_ue_sim            uciConfig;
    uint16_t                    numUci2Maps;    
    fapi_map_index_t            mapIndex[];
}fapi_uci_config_t;

// Updated per 5G FAPI

typedef struct PACK_STRUCT fapi_prb_sym_rate_match_pattern
{
    uint8_t                     prbSymbRateMatchPatternID;
    uint8_t                     freqDomainRB[35];
    uint8_t                     oneOrTwoSlots;
    uint32_t                    symbolsInRB;
    uint8_t                     timeDomainPeriodicity;
    uint8_t                     timeDomainPattern[5];
    uint8_t                     subCarrierSpacing;
}fapi_prb_sym_rate_match_pattern_t;

typedef struct PACK_STRUCT fapi_prb_symbol_rate_match_pattern
{
    fapi_tl_t_ue_sim                    prbSymRateMatchPattern;
    uint8_t                             numPrbSymbRatePattern;
    fapi_prb_sym_rate_match_pattern_t   prbSymbRateMatchPatterns[];
}fapi_prb_symbol_rate_match_pattern_t;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_mbsfn_sf_config_list
{
    uint8_t                     radioframeAllocationPeriod;
    uint8_t                     radioframeAllocationOffset;
    uint8_t                     lteFrameStructureType;
    uint8_t                     subframeAllocLength;
    uint32_t                    subframeAllocationBitmap; /* Ref: Table 3-42 LTE-CRS rate match patterns configuration*/
}fapi_mbsfn_sf_config_list_t;

typedef struct PACK_STRUCT fapi_lte_crs_rm_ptrn    
{
    uint8_t                     crsRateMatchPatternID;
    uint16_t                    carrierFreqDL;
    uint8_t                     carrierBandwidthDL;
    uint8_t                     nrCrsPorts;
    uint8_t                     vShift;
    uint8_t                     sizeMbsfnSubframeConfigList;
    fapi_mbsfn_sf_config_list_t mbsfnSFConfigList[0];
}fapi_lte_crs_rm_ptrn_t;   

typedef struct PACK_STRUCT fapi_lte_crs_rm_pattern
{
    fapi_tl_t_ue_sim            lteCrsRmPattern;
    uint8_t                     numLteCrsRmPattern;
    fapi_lte_crs_rm_ptrn_t      lteCrsRmPtrn[0];
}fapi_lte_crs_rm_pattern_t;

// Updated per 5G Fapi
typedef struct PACK_STRUCT fapi_ul_bwp_id
{
    uint8_t                     pucchGroupHopping;
    uint16_t                    nIdPucchHopping;
}fapi_ul_bwp_id_t;

typedef struct PACK_STRUCT fapi_pucch_semi_static_config
{
    fapi_uint8_tlv_t_ue_sim            numUlBwpIds;
    fapi_ul_bwp_id_t            ulBwpId[0];
}fapi_pucch_semi_static_config_t;

// Updated per 5G Fapi
typedef struct PACK_STRUCT fapi_pdsch_config
{
    fapi_uint8_tlv_t_ue_sim            pdschCbgScheme;
}fapi_pdsch_config_t;

// Updated per 5G Fapi
typedef struct PACK_STRUCT fapi_delay_mgmt_config
{
    fapi_uint16_tlv_t_ue_sim           timingWindow;
    fapi_uint8_tlv_t_ue_sim            timingInfoMode;
    fapi_uint8_tlv_t_ue_sim            timingInfoPeriod;
}fapi_delay_mgmt_config_t;

// Updated per 5G Fapi
typedef struct PACK_STRUCT fapi_rel16_mtrp_config
{
    fapi_uint8_tlv_t_ue_sim            numTxPortsTRP1;
    fapi_uint8_tlv_t_ue_sim            numRxPortsTRP1;
}fapi_rel16_mtrp_config_t;
#endif
// Updated per 5G FAPI
typedef struct PACK_STRUCT
{
    uint16_t                    pmIdx;
    uint16_t                    numLayers;
    uint16_t                    numAntPorts;
    fapi_precoder_weight_t_ue_sim      precoderWeight[MAX_NUM_LAYERS];
} fapi_precoding_table_t_ue_sim;

// Updated per 5G FAPI
#ifdef FAPI_222_10_03
typedef struct PACK_STRUCT fapi_config
{
    fapi_phy_config_t                      phyConfig;
    fapi_carrier_config_t_ue_sim                  carrierConfig;
    fapi_cell_config_t_ue_sim                     cellConfig;
    fapi_ssb_pwr_pbch_config_t             ssbPowerPbchConfig;
    fapi_prach_configuration_t_ue_sim             prachConfig;
    fapi_multi_prach_config_t              multiPrachConfig;
    fapi_ssb_res_config_table_t            ssbResConfigTable;
    fapi_multi_ssb_resource_config_table_t multiSsbResourceConfigTable;
    fapi_tdd_table_t_ue_sim                       tddTable;
    fapi_meas_config_t_ue_sim                     measConfig;
    fapi_beamforming_table_t_ue_sim               beamformingTable;
    fapi_precoding_table_t_ue_sim                 precodingTable;
    fapi_uci_config_t                      uciConfig;
    fapi_prb_symbol_rate_match_pattern_t   prbSymRmPtrn;
    fapi_lte_crs_rm_pattern_t              lteCrsRmPtrn;
    fapi_pucch_semi_static_config_t        pucchSemiStaticConfig;
    fapi_pdsch_config_t                    pdschConfig;
    fapi_delay_mgmt_config_t               delayMgmtConfig;
    fapi_rel16_mtrp_config_t               rel16MtrpConfig;
}fapi_config_t_ue_sim;
#else
typedef struct PACK_STRUCT fapi_config
{
    fapi_carrier_config_t_ue_sim       carrierConfig;
    fapi_cell_config_t_ue_sim          cellConfig;
    fapi_ssb_config_t_ue_sim           ssbConfig;
    //fapi_prach_config_t         prachConfig;
    fapi_prach_configuration_t_ue_sim  prachConfig;
    fapi_ssb_table_t_ue_sim            ssbTable;
    fapi_tdd_table_t_ue_sim            tddTable;
    fapi_meas_config_t_ue_sim          measConfig;
    fapi_beamforming_table_t_ue_sim    beamformingTable;
    fapi_precoding_table_t_ue_sim      precodingTable;
}fapi_config_t_ue_sim;
#endif
// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_config_req
{
    fapi_msg_header_t_ue_sim    header;
    uint8_t                     number_of_tlvs;
    fapi_uint16_tlv_t_ue_sim           tlvs[MAX_NUM_TLVS_CONFIG];
} fapi_config_req_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_config_resp
{
    fapi_msg_header_t_ue_sim    header;
    uint8_t                     error_code;
    uint8_t                     number_of_invalid_tlvs;
    uint8_t                     number_of_inv_tlvs_idle_only;
    uint8_t                     number_of_inv_tlvs_running_only;	
    uint8_t                     number_of_missing_tlvs;
    fapi_uint16_tlv_t_ue_sim           tlvs[4 * MAX_NUM_TLVS_CONFIG];
//   fapi_uint16_tlv_t_ue_sim           unsupported_or_invalid_tlvs[MAX_NUMBER_UNSUPPORTED_TLVS];
//   fapi_uint16_tlv_t_ue_sim           invalid_idle_only_tlvs[MAX_NUMBER_OF_INVALID_IDLE_ONLY_TLVS];
//   fapi_uint16_tlv_t_ue_sim           invalid_running_only_tlvs[MAX_NUMBER_OF_INVALID_RUNNING_ONLY_TLVS];
//   fapi_uint16_tlv_t_ue_sim           missing_tlvs[MAX_NUMBER_OF_MISSING_TLVS];
} fapi_config_resp_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_start_req
{
    fapi_msg_header_t_ue_sim           header;  // Message Length is zero for START.request
} fapi_start_req_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_stop_req
{
    fapi_msg_header_t_ue_sim           header;  // Message Length is zero for STOP.request
} fapi_stop_req_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_stop_ind
{
    fapi_msg_header_t_ue_sim           header; // Message Length is zero for STOP.indication
} fapi_stop_ind_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_error_ind
{
    fapi_msg_header_t_ue_sim    header;
    uint16_t                    sfn;
    uint16_t                    slot;
    uint8_t                     message_id;
    uint8_t                     error_code;
#ifdef FAPI_222_10_03
    uint16_t			expectedSFN;
    uint16_t		        expectedSlot;
#endif
} fapi_error_ind_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_slot_ind
{
    fapi_msg_header_t_ue_sim    header;
    uint16_t                    sfn;
    uint16_t                    slot;
} fapi_slot_ind_t_ue_sim;

#ifdef FAPI_222_10_03
/* Ref: section 3.1.9 of* SCF-225 */
typedef struct PACK_STRUCT fapi_start_resp
{
  fapi_msg_header_t_ue_sim    header;
  uint8_t                     errorCode;
}fapi_start_resp_t;
#endif
// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_bmi
{
    uint16_t                    beamidx;
} fapi_bmi_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_pmi_bfi
{
    uint16_t                    pm_idx;
    fapi_bmi_t_ue_sim                  beam_idx[];
} fapi_pmi_bfi_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_bmform
{
#ifndef FAPI_222_10_03
    uint8_t                     trpScheme;
#endif
    uint16_t                    num_prgs;
    uint16_t                    prg_size;
    uint8_t                     dig_bf_interfaces;
    uint16_t                    beam_idx[][1];
} fapi_bmform_t;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_precoding_bmform
{
#ifndef FAPI_222_10_03
    uint8_t                     trpScheme;
#endif
    uint16_t                    num_prgs;
    uint16_t                    prg_size;
    uint8_t                     dig_bf_interfaces;
    fapi_pmi_bfi_t_ue_sim              pmi_bfi[1];
} fapi_precoding_bmform_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_tx_power_info
{
    uint8_t                     beta_pdcch_1_0;
#ifndef FAPI_222_10_03
    uint8_t                     power_control_ofsset_ss;
#else
    int8_t                      powerControlOffsetSSProfileNR;
#endif
} fapi_tx_power_info_t;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_bwp
{
    uint16_t                    bwp_size;   /* BWP configuration Start */
    uint16_t                    bwp_start;
    uint8_t                     sub_carrier_spacing;
    uint8_t                     cyclic_prefix;    /* BWP Configuration End*/
}fapi_bwp_t;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_coreset
{
    uint8_t                     start_symbol_index;  /* Coreset Configuration Start */
    uint8_t                     duration_symbols;
    uint8_t                     freq_domain_resource[FAPI_MAX_FREQ_DOMAIN_RES];
    uint8_t                     cce_reg_mapping_type;
    uint8_t                     reg_bundle_size;
    uint8_t                     interleaver_size;
    uint8_t                     core_set_type;
    uint16_t                    shift_index;
    uint8_t                     precoder_granularity;   /* Coreset Configuration End */
}fapi_coreset_t;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_dl_dci
{
    uint16_t                    rnti;
#ifdef FAPI_222_10_03
    uint16_t                    nIdPdcchData;
    uint16_t                    nRntiPdcchData;
#else
    uint16_t                    scrambling_id;
    uint16_t                    scrambling_rnti;
#endif
    uint8_t                     cce_index;
    uint8_t                     aggregation_level;
    fapi_precoding_bmform_t_ue_sim     pc_and_bform;
    fapi_tx_power_info_t        tx_power_info;
    uint16_t                    payload_size_bits;
    uint8_t                     payload[];
} fapi_dl_dci_t_ue_sim;
#ifdef FAPI_222_10_03
typedef struct PACK_STRUCT fapi_coreset_parms
{
    uint16_t                    pdcchPduIndex;  
    uint16_t                    nIdPdcchDmrs;  
} fapi_coreset_parms_t;

typedef struct PACK_STRUCT fapi_dci_specific_parms
{
    uint16_t                     dciIndex;
    uint8_t                      collocatedAl16Candidate;
    int16_t                      pdcchDmrsPowerOffsetProfileSSS;
    int16_t                      pdcchDataPowerOffsetProfileSSS;  
} fapi_dci_specific_parms_t;

typedef struct PACK_STRUCT fapi_pdcch_mnt_parms 
{
    fapi_coreset_parms_t       coresetParms;
    fapi_dci_specific_parms_t  dciSpecificParms;
} fapi_pdcch_mnt_parms_t;
#endif
// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_dl_pdcch_pdu
{
   fapi_bwp_t                  bwp;
   fapi_coreset_t              coreset;
   uint16_t                    num_dl_dci; /*Number of DCIs in this CORESET */      
   fapi_dl_dci_t_ue_sim               dl_dci[0];
#ifdef FAPI_222_10_03
   fapi_pdcch_mnt_parms_t      pdcchMntParms;
#endif
} fapi_dl_pdcch_pdu_t_ue_sim;
// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_codeword_pdu
{
    uint16_t                    target_code_rate;
    uint8_t                     qam_mod_order;
    uint8_t                     mcs_index;
    uint8_t                     mcs_table;
    uint8_t                     rv_index;
    uint32_t                    tb_size;
} fapi_codeword_pdu_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_dmrs
{
    uint16_t                    dl_dmrs_symb_pos; /*SCF: DMRS Config Start */
    uint8_t                     dmrs_config_type;
    uint16_t                    dl_dmrs_scrambling_id;
    uint8_t                     scid;
    uint8_t                     num_dmrs_cdm_grps_no_data;
    uint16_t                    dmrs_ports;   /*SCF: DMRS Config End */
}  fapi_dmrs_t;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_pdsch_freq_dom_alloc
{
    uint8_t                     resource_alloc; /*SCF: PDSCH Allocation in Frequency Domain Start */
    uint8_t                     rb_bitmap[36];
    uint16_t                    rb_start;
    uint16_t                    rb_size;    
    uint8_t                     vrb_to_prb_mapping;/*SCF : PDSCH Allocation in Frequency Domain End */
}  fapi_pdsch_freq_dom_alloc_t;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_pdsch_time_dom_alloc
{
    uint8_t                     start_symb_index;/*SCF : PDSCH Allocation in Time Domain Start */
    uint8_t                     nr_of_symbols;  /*SCF : PDSCH Allocation in Time Domain End */
}  fapi_pdsch_time_dom_alloc_t;

typedef struct PACK_STRUCT  fapi_dl_pdsch_ptrs_info
{
    uint8_t                     ptrs_port_index;
    uint8_t                     ptrs_time_density;
    uint8_t                     ptrs_freq_density;
    uint8_t                     ptrs_re_offset;
    uint8_t                     n_epre_ratio_of_pdsch_to_ptrs;

} fapi_dl_pdsch_ptrs_info_t;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_tx_pwr_info
{
#ifndef FAPI_222_10_03
    uint8_t                     power_control_offset; /*SCF : TX Power Info Start */
    uint8_t                     power_control_offset_ss; /*SCF : Tx Power Info End */
#else
    uint8_t                     powerControlOffsetProfileNR; /*SCF : TX Power Info Start */
    uint8_t                     powerControlOffsetSSProfileNR; /*SCF : Tx Power Info End */
#endif
}  fapi_tx_pwr_info_t;

typedef struct PACK_STRUCT fapi_dl_pdsch_cbg_info
{
   uint8_t                     is_last_cb_present;
   uint8_t                     is_inline_tb_crc;
#ifndef FAPI_222_10_03
   uint32_t                    dl_tb_crc;
#else
   uint32_t                    dlTbCrc[2];
#endif

}  fapi_dl_pdsch_cbg_info_t;

#ifdef FAPI_222_10_03
typedef struct PACK_STRUCT fapi_dl_prb_sym_rm_patrn_by_val
{
    uint8_t                    freqDomainRB[35];
    uint16_t                   symbolsInRB;  
} fapi_dl_prb_sym_rm_patrn_by_val_t;

typedef struct PACK_STRUCT fapi_dl_coreset_rm_patrn
{
    uint8_t                    freqDomainResources[6];
    uint16_t                   symbolsPatrn;
} fapi_dl_coreset_rm_patrn_t;


typedef struct PACK_STRUCT fapi_dl_rate_match_ref
{
    uint8_t                            ssbPdusForRateMatching[2];
    uint8_t                            ssbConfigForRateMatching;
    uint8_t                            prbSymRmPatrnBitmapSizeByRef;
    uint8_t                            prbSymRmPatrnBitmapByRef[0];  // side depends on ceil(prbSymRmPatrnBitmapSizeByRef / 8) 
    uint8_t                            numPrbSymRmPatrnByVal; 
    fapi_dl_prb_sym_rm_patrn_by_val_t  prbSymRmPatrnByValue[0];
    uint8_t                            numCoresetRmPatrn;
    fapi_dl_coreset_rm_patrn_t         coresetRmPatrn[0];
    uint16_t                           pdcchPduIndex;
    uint16_t                           dciIndex;
    uint8_t                            lteCrsRaMaPatrnBitmapSize;
    uint8_t                            lteCrsRaMaPatrn[0]; // side depends on ceil(lteCrsRaMaPatrnBitmapSize / 8)
    uint8_t                            numCsiRsForRaMa;
    uint16_t                           csiRsForRaMa[0];  // side depends on numCsiRsForRaMa   
    
} fapi_dl_rate_match_ref_t;

typedef struct PACK_STRUCT fapi_dl_code_word
{
    uint8_t                    cbgTxInfo;
} fapi_dl_code_word_t;

typedef struct PACK_STRUCT fapi_dl_tx_pwr_info
{
    uint16_t                   pdschDmrsPwrOffsetPrfSSS;
    uint16_t                   pdschDataPwrOffsetPrfSSS;
    uint8_t                    maxNumCbgPerTb;
    fapi_dl_code_word_t        codeWord[0];
} fapi_dl_tx_pwr_info_t;

typedef struct PACK_STRUCT fapi_dl_codeword_info
{
    uint8_t                    ldpcBaseGraph;
    uint32_t                   tbSizeLbrmBytes;
    uint8_t                    tbCrcRequired;

} fapi_dl_codeword_info_t;

typedef struct PACK_STRUCT fapi_dl_pdsch_mnt_parms
{
    uint8_t                    pdschTransType;
    uint16_t                   coresetStartPoint;
    uint16_t                   initialDlBwpSize;
    fapi_dl_codeword_info_t    codewordInfo;
    fapi_dl_rate_match_ref_t   rateMatchRef;
    fapi_dl_tx_pwr_info_t      txPwrInfo;
} fapi_dl_pdsch_mnt_parms_t;

typedef struct PACK_STRUCT fapi_dl_ptrs_tx_pwr_info
{ 
    uint16_t                    pdschPtrsPwrOffsetPrfSSS;  
}fapi_dl_ptrs_tx_pwr_info_t; 

typedef struct PACK_STRUCT  fapi_dl_pdsch_ptrs_mnt_parms
{
    fapi_dl_ptrs_tx_pwr_info_t  txPwrInfo;  
} fapi_dl_pdsch_ptrs_mnt_parms_t;

typedef struct PACK_STRUCT fapi_dl_pdsch_ptrs2
{
    uint8_t                      ptrsPortIndex;
    uint8_t                      ptrsTimeDensity;
    uint8_t                      ptrsFreqDensity; 
    uint8_t                      ptrsReOffset; 
    uint8_t                      nEpreRatOfPdschToPtrsProfNR; 
} fapi_dl_pdsch_ptrs2_t;

typedef struct PACK_STRUCT fapi_dl_pdsch_ptrsv3
{
    fapi_dl_ptrs_tx_pwr_info_t  txPwrInfo;  
}fapi_dl_pdsch_ptrsv3_t;

typedef struct PACK_STRUCT fapi_dl_rel16_pdsch_parms_t
{
    uint8_t                     repetitionScheme;
    fapi_dl_pdsch_ptrs2_t       pdschPtrs2;
    fapi_dl_pdsch_ptrsv3_t      pdschPtrsV3;
} fapi_dl_rel16_pdsch_parms_t;
#endif
// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_dl_pdsch_pdu
{
    uint16_t                         pdu_bit_map; /* Bit 0: pdschPtrs - Indicates PTRS included (FR2)
                                                Bit 1:cbgRetxCtrl (Present when CBG based retransmit is used) */
    uint16_t                         rnti;
    uint16_t                         pdu_index;
    fapi_bwp_t                       bwp;
    uint8_t                          nr_of_code_words; /* SCF : Code Word Info Start */
    fapi_codeword_pdu_t_ue_sim              cw_info[FAPI_MAX_NUM_CW];       /* SCF : Code Word Info End */
#ifdef FAPI_222_10_03
    uint16_t                         nIdPdsch;
#else 
    uint16_t                         data_scrambling_id;
#endif
    uint8_t                          nr_of_layers;
    uint8_t                          transmission_scheme;
    uint8_t                          ref_point;
    fapi_dmrs_t                      dmrs;
    fapi_pdsch_freq_dom_alloc_t      pdsch_freq_dom_alloc;
    fapi_pdsch_time_dom_alloc_t      pdsch_time_dom_alloc; 
    fapi_dl_pdsch_ptrs_info_t        pdsch_ptrs_info;
    fapi_precoding_bmform_t_ue_sim          pre_coding_and_beamforming;
    fapi_tx_pwr_info_t               tx_pwr_info;
    fapi_dl_pdsch_cbg_info_t         cbg_info;
#ifdef FAPI_222_10_03
    fapi_dl_pdsch_mnt_parms_t        dlPdschMntparms;
    fapi_dl_pdsch_ptrs_mnt_parms_t   dlPdschPtrsMntParms;
    fapi_dl_rel16_pdsch_parms_t      rel16PdschParms;
#endif
} fapi_dl_pdsch_pdu_t_ue_sim;

#ifdef FAPI_222_10_03
typedef struct PACK_STRUCT fapi_basic_parms
{
    uint16_t         csiRsPduIndex;            
} fapi_basic_parms_t;

typedef struct PACK_STRUCT ifapi_csi_rs_tx_pwr_info
{
    uint16_t         csiRsPowerOffsetProfileSSS; 
} fapi_csi_rs_tx_pwr_info_t;


typedef struct PACK_STRUCT fapi_dl_csi_rs_mnt_parms
{
    fapi_basic_parms_t          basic;
    fapi_csi_rs_tx_pwr_info_t   txPwrInfo;
}fapi_dl_csi_rs_mnt_parms_t;
#endif

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_dl_csi_rs_pdu
{
#ifdef FAPI_222_10_03
    uint8_t                     subCarrierSpacing;
    uint8_t                     cyclicPrefix;    /* BWP Configuration End*/
#else
    fapi_bwp_t                  bwp;
#endif
    uint16_t                    start_rb;
    uint16_t                    nr_of_rbs;
    uint8_t                     csi_type;
    uint8_t                     row;
    uint16_t                    freq_domain;
    uint8_t                     symb_l0;
    uint8_t                     symb_l1;
    uint8_t                     cdm_type;
    uint8_t                     freq_density;
    uint16_t                    scram_id;
    fapi_tx_pwr_info_t          tx_pwr_info;
    fapi_precoding_bmform_t_ue_sim     pre_coding_and_beamforming;
#ifdef FAPI_222_10_03
    fapi_dl_csi_rs_mnt_parms_t  csiRsMntParms;
#endif
} fapi_dl_csi_rs_pdu_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_phy_mib_pdu
{
    uint8_t                     dmrs_type_a_position;
    uint8_t                     pdcch_config_sib1;
    uint8_t                     cell_barred;
    uint8_t                     intra_freq_reselction;
} fapi_phy_mib_pdu_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_bch_payload
{
    union
    {
        uint32_t                bch_payload;
        fapi_phy_mib_pdu_t_ue_sim      phy_mib_pdu;
    } u;
} fapi_bch_payload_t_ue_sim;

#ifdef FAPI_222_10_03
typedef struct PACK_STRUCT fapi_ssb_basic_parms
{
    uint8_t                     ssbPduIndex;
    uint8_t                     cases;
    uint8_t                     subcarrierSpacing;
    uint8_t                     lMax;
} fapi_ssb_basic_parms_t;

typedef struct PACK_STRUCT fapi_ssb_tx_pwr_info
{
    uint16_t                    ssPbchBlockPowerScaling;
    uint16_t                    betaPSSProfileSSS;
} fapi_ssb_tx_pwr_info_t;

typedef struct PACK_STRUCT fapi_dl_ssb_pbch_mnt_parms
{
    fapi_ssb_basic_parms_t            basicParams;
    fapi_ssb_tx_pwr_info_t            tx_pwr_info;
}fapi_dl_ssb_pbch_mnt_parms_t;

#endif

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_dl_ssb_pdu
{

    uint16_t                      phys_cell_id;
#ifdef FAPI_222_10_03
    uint8_t                       betaPssProfileNR;
#else
    uint8_t                       beta_pss;
#endif
    uint8_t                       ssb_block_index;
    uint8_t                       ssb_sub_carrier_offset;
    uint16_t                      ssb_offset_point_a;
    uint8_t                       bch_payload_flag;
    fapi_bch_payload_t_ue_sim            bch_payload;
    fapi_precoding_bmform_t_ue_sim       pre_coding_and_beamforming;
#ifdef FAPI_222_10_03
    fapi_dl_ssb_pbch_mnt_parms_t  ssbPbchMntParms;
#endif 
}  fapi_dl_ssb_pdu_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_dl_tti_req_pdu
{
    uint16_t                    pdu_type;
    uint16_t                    pdu_size;
    union
    {
        fapi_dl_pdcch_pdu_t_ue_sim        pdcch_pdu;
        fapi_dl_pdsch_pdu_t_ue_sim        pdsch_pdu;
        fapi_dl_csi_rs_pdu_t_ue_sim       csi_rs_pdu;
        fapi_dl_ssb_pdu_t_ue_sim          ssb_pdu;
    }u;
} fapi_dl_tti_req_pdu_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_ue_info
{
    uint8_t                     n_ue;
    uint8_t                     pdu_idx[];
} fapi_ue_info_t_ue_sim;	

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_dl_tti_req
{
    fapi_msg_header_t_ue_sim    header;
    uint16_t                    sfn;
    uint16_t                    slot;
#ifdef FAPI_222_10_03
    uint16_t                    n_pdus;
    uint8_t			nDlTypes;
    uint16_t			nPDUsOfEachType[MAX_NDLTYPES_DL_TTI_REQ]; 
#else
    uint8_t                     n_pdus;
#endif
    uint8_t                     n_group;
    fapi_dl_tti_req_pdu_t_ue_sim       pdus[0];
    fapi_ue_info_t_ue_sim              ueGrpInfo[0];
} fapi_dl_tti_req_t_ue_sim;
#ifdef FAPI_222_10_03
typedef struct PACK_STRUCT fapi_pRACH_Mntnc_param
{
   uint32_t                     handle;   
   uint8_t                      prachConfigScope;
   uint16_t                     prachResConfigIndex;
   uint8_t                      numFdRa;
   uint8_t                      startPreambleIndex;
   uint8_t                      numPreambleIndices;
}fapi_pRACH_Mntnc_param_t;
#endif
// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_ul_prach_pdu
{
    uint16_t                    phys_cell_id;
    uint8_t                     num_prach_ocas;
    uint8_t                     prach_format;
#ifdef FAPI_222_10_03
    uint8_t                     indexFdRa;
#else
    uint8_t                     num_ra;
#endif
    uint8_t                     prach_start_symbol;
    uint16_t                    num_cs;
    fapi_bmform_t               beamforming;
#ifdef FAPI_222_10_03
    fapi_pRACH_Mntnc_param_t    pRACH_Mntnc;       
#endif        
} fapi_ul_prach_pdu_t_ue_sim;


// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_pusch_data
{
    uint8_t                     rv_index;
    uint8_t                     harq_process_id;
    uint8_t                     new_data_indicator;
    uint32_t                    tb_size;
    uint16_t                    num_cb;
    uint8_t                     cb_present_and_position[1];
} fapi_pusch_data_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_pusch_uci
{
    uint16_t                    harq_ack_bit_length;
    uint16_t                    csi_part1_bit_length;
#ifdef FAPI_222_10_03
    uint16_t                    flagCsiPart2;
#else
    uint16_t                    csi_part2_bit_length;
#endif
    uint8_t                     alpha_scaling;
    uint8_t                     beta_offset_harq_ack;
    uint8_t                     beta_offset_csi1;
    uint8_t                     beta_offset_csi2;
} fapi_pusch_uci_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_ptrs_info
{
    uint16_t                    ptrs_port_index;
    uint8_t                     ptrs_dmrs_port;
    uint8_t                     ptrs_re_offset;
} fapi_ptrs_info_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_pusch_ptrs
{
    uint8_t                     num_ptrs_ports;
    fapi_ptrs_info_t_ue_sim            ptrs_info[MAX_NUM_PTRS_PORTS];
    uint8_t                     ptrs_time_density;
    uint8_t                     ptrs_freq_density;
    uint8_t                     ul_ptrs_power;
} fapi_pusch_ptrs_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_dfts_ofdm
{
    uint8_t                     low_papr_group_number;
    uint16_t                    low_papr_sequence_number;
    uint8_t                     ul_ptrs_sample_density;
    uint8_t                     ul_ptrs_time_density_transform_precoding;
} fapi_dfts_ofdm_t_ue_sim;

#ifdef FAPI_222_10_03
typedef struct PACK_STRUCT fapi_pusch_mnt_parms
{
    uint8_t                     puschTransType;
    uint16_t                    deltaBwp0StartFromActiveBwp;
    uint16_t                    initialUlBwpSize;
    uint8_t                     groupOrSequenceHopping;
    uint16_t                    puschSecondHopPRB;
    uint8_t                     ldpcBaseGraph;
    uint32_t                    tbSizeLbrmBytes; 
} fapi_pusch_mnt_parms_t;

typedef struct PACK_STRUCT fapi_part2_parms
{
    uint16_t                    priority;
    uint8_t                     numPart1Params;
    uint16_t                    paramOffsets[0];
    uint8_t                     paramSizes[0];
    uint16_t                    part2SizeMapIndex;      
} fapi_part2_parms_t;

typedef struct PACK_STRUCT fapi_optional_pusch_uci
{
    uint16_t                    numPart2s;
    fapi_part2_parms_t          part2_parms[0]; 
} fapi_optional_pusch_uci_t;
#endif

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_ul_pusch_pdu
{
    uint16_t                    pdu_bitmap;
    uint16_t                    rnti;
    uint32_t                    handle;
#ifdef 	VZ_VERIPHY_INTEG
    fapi_bwp_t                  bwp;
#else
    uint16_t                    bwp_size;
    uint16_t                    bwp_start;
    uint8_t                     sub_carrier_spacing;
    uint8_t                     cyclic_prefix;
#endif	
    uint16_t                    target_code_rate;
    uint8_t                     qam_mod_order;
    uint8_t                     mcs_index;
    uint8_t                     mcs_table;
    uint8_t                     transform_precoding;
#ifdef FAPI_222_10_03
    uint16_t                    nIdPusch;
#else
    uint16_t                    data_scrambling_id;
#endif
    uint8_t                     nr_of_layers;
    uint16_t                    ul_dmrs_symb_pos;
    uint8_t                     dmrs_config_type;
    uint16_t                    ul_dmrs_scrambling_id;
    uint16_t                    pusch_identity;
#ifdef FAPI_222_10_03
    uint8_t                     nScid;
#else
    uint8_t                     scid;
#endif
    uint8_t                     num_dmrs_cdm_grps_no_data;
    uint16_t                    dmrs_ports;
    uint8_t                     resource_alloc;
    uint8_t                     rb_bitmap[36];
    uint16_t                    rb_start;
    uint16_t                    rb_size;
    uint8_t                     vrb_to_prb_mapping;
#ifdef FAPI_222_10_03
    uint8_t                     IntraSlotFrequencyHopping;
#else
    uint8_t                     frequency_hopping;
#endif
    uint16_t                    tx_direct_current_location;
    uint8_t                     uplink_frequency_shift7p5khz;
    uint8_t                     start_symb_index;
    uint8_t                     nr_of_symbols;
    fapi_pusch_data_t_ue_sim           pusch_data;
    fapi_pusch_uci_t_ue_sim            pusch_uci;
    fapi_pusch_ptrs_t_ue_sim           pusch_ptrs;
    fapi_dfts_ofdm_t_ue_sim            dfts_ofdm;
    fapi_bmform_t               beamforming;
#ifdef FAPI_222_10_03
    fapi_pusch_mnt_parms_t      puschMntParms;
    fapi_optional_pusch_uci_t   optionalPuschUci; 
#endif
} fapi_ul_pusch_pdu_t_ue_sim;
#ifdef FAPI_222_10_03
typedef struct PACK_STRUCT fapi_pUCCH_Mntnnc_params
{
    uint8_t                     maxCodeRate;
    uint8_t                     ulBwpId;
} fapi_pucch_mntnnc_params_t;
typedef struct PACK_STRUCT fapi_uci_part2
{
    uint16_t                    priority;
    uint8_t                     numPart1Params;
    uint16_t                    paramOffsets[0];
    uint8_t                     paramSizes[0];
    uint8_t                     part2SizeMapIndex;
}fapi_uci_part2_t;
typedef struct PACK_STRUCT fapi_uci_part1_part2
{
    uint16_t                    numPart2s;
    fapi_uci_part2_t            uci_part2[0];
} fapi_uci_part1_part2_t;
#endif
// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_ul_pucch_pdu
{
    uint16_t                    rnti;
    uint32_t                    handle;
#ifdef VZ_VERIPHY_INTEG
    fapi_bwp_t                  bwp;
#else
    uint16_t                    bwp_size;
    uint16_t                    bwp_start;
    uint8_t                     sub_carrier_spacing;
    uint8_t                     cyclic_prefix;
#endif	
    uint8_t                     format_type;
    uint8_t                     multi_slot_tx_indicator;
    uint8_t                     pi2_bpsk;
    uint16_t                    prb_start;
    uint16_t                    prb_size;
    uint8_t                     start_symbol_index;
    uint8_t                     nr_of_symbols;
#ifdef FAPI_222_10_03
    uint8_t                     intraSlotFrequencyHopping;
#else
    uint8_t                     freq_hop_flag;
#endif
    uint16_t                    second_hop_prb;
#ifdef FAPI_222_10_03
    uint8_t                     pucchGroupHopping;
    uint8_t                     obsolete8bit;
    uint8_t                     nIdPucchHopping;
#else
    uint8_t                     group_hop_flag;
    uint8_t                     sequence_hop_flag;
    uint16_t                    hopping_id;
#endif
    uint16_t                    initial_cyclic_shift;
#ifdef FAPI_222_10_03
    uint16_t                    nIdPucchScrambling;
#else
    uint16_t                    data_scrambling_id;
#endif
    uint8_t                     time_domain_occ_idx;
    uint8_t                     pre_dft_occ_idx;
    uint8_t                     pre_dft_occ_len;
    uint8_t                     add_dmrs_flag;
    uint16_t                    dmrs_scrambling_id;
    uint8_t                     dmrs_cyclic_shift;
    uint8_t                     sr_flag;
    uint16_t                    bit_len_harq;
#ifdef FAPI_222_10_03
    uint16_t                    csiPart1BitLength;
#else
    uint16_t                    bit_len_csi_part1;
    uint16_t                    bit_len_csi_part2;
#endif
    fapi_bmform_t               beamforming;
#ifdef FAPI_222_10_03
    fapi_pucch_mntnnc_params_t  pucch_basicextension;
    fapi_uci_part1_part2_t      uci_part1_part2;
#endif
} fapi_ul_pucch_pdu_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_ul_srs_pdu
{
    uint16_t                    rnti;
    uint32_t                    handle;
#ifdef VZ_VERIPHY_INTEG	
    fapi_bwp_t                  bwp;
#else
    uint16_t                    bwp_size;
    uint16_t                    bwp_start;
    uint8_t                     sub_carrier_spacing;
    uint8_t                     cyclic_prefix;
#endif	
    uint8_t                     num_ant_ports;
    uint8_t                     num_symbols;
    uint8_t                     num_repetitions;
    uint8_t                     time_start_position;
    uint8_t                     config_index;
    uint16_t                    sequence_id;
    uint8_t                     bandwidth_index;
    uint8_t                     comb_size;
    uint8_t                     comb_offset;
    uint8_t                     cyclic_shift;
    uint8_t                     frequency_position;
    uint16_t                    frequency_shift;
    uint8_t                     frequency_hopping;
    uint8_t                     group_or_sequence_hopping;
    uint8_t                     resource_type;
    uint16_t                    t_srs;
    uint16_t                    t_offset;
    fapi_bmform_t               beamforming;
} fapi_ul_srs_pdu_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_ul_tti_req_pdu
{
    uint16_t                    pdu_type;
    uint16_t                    pdu_size;
    union
    {
        fapi_ul_prach_pdu_t_ue_sim        prach_pdu;
        fapi_ul_pusch_pdu_t_ue_sim        pusch_pdu;
        fapi_ul_pucch_pdu_t_ue_sim        pucch_pdu;
        fapi_ul_srs_pdu_t_ue_sim          srs_pdu;
        //fapi_ul_rx_bmform_pdu_t    rx_beamforming_pdu;
    }u;
} fapi_ul_tti_req_pdu_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_ul_tti_req
{
    fapi_msg_header_t_ue_sim    header;
    uint16_t                    sfn;
    uint16_t                    slot;
#ifdef FAPI_222_10_03
    uint16_t                    n_pdus;
    uint8_t                     nUlTypes;
/* [0]: number of PRACH PDUs [1]: number of PUSCH PDUs [2]: number of Format 0/1 PUCCH PDUs [3]: number of Format 2/3/4 PUCCH PDUs [4]: number of SRS PDUs */  
    uint16_t                    nPDUsOfEachType[MAX_NUM_UL_TYPES];   
#else
    /* Indicates if a RACH PDU will be included in this message.
     * 0: no RACH in this slot
     * 1: RACH in this slot */
    uint8_t                     rach_present;
    uint8_t                     n_ulsch; /* Number of ULSCH PDUs that are included in this message.*/
    uint8_t                     n_ulcch; /* Number of ULCCH PDUs that are included in this message.*/
    uint8_t                     n_pdus;
#endif
    uint8_t                     n_group; /* Number of UE Groups included in this message.*/
    fapi_ul_tti_req_pdu_t_ue_sim       pdus[0];
    fapi_ue_info_t_ue_sim              ueGrpInfo[0];
} fapi_ul_tti_req_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_dci_pdu
{
    uint16_t                    pdu_type;
    uint16_t                    pdu_size;
    fapi_dl_pdcch_pdu_t_ue_sim         pdcch_pdu;
} fapi_dci_pdu_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_ul_dci_req
{
    fapi_msg_header_t_ue_sim    header;
    uint16_t                    sfn;
    uint16_t                    slot;
    uint8_t                     num_pdus;
#ifdef FAPI_222_10_03
    uint8_t			nDlTypes;
    uint16_t			nPDUsOfEachType[MAX_NDLTYPES_UL_DCI_REQ];
#endif
    fapi_dci_pdu_t_ue_sim              pdus[];
} fapi_ul_dci_req_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_tx_pdu_desc
{
    uint32_t                    pduLength;
    uint16_t                    pduIndex;
#ifdef FAPI_222_10_03
    uint8_t                     cwIndex; /* Ref: Table 3-90 Tx_Data.request message */
#endif
    uint32_t                    numTlvs;
    fapi_tx_dat_tlv_t           tlvs[];
} fapi_tx_pdu_desc_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_tx_data_req
{
    fapi_msg_header_t_ue_sim    header;
    uint16_t                    sfn;
    uint16_t                    slot;
    uint16_t                    numPdus;
#ifdef VZ_VERIPHY_INTEG
    fapi_tx_pdu_desc_t_ue_sim          pduDesc[];
#else
    fapi_tx_pdu_desc_t_ue_sim          pduDesc[];
#endif
} fapi_tx_data_req_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_pdu_ind_info
{
    uint32_t                    handle;
    uint16_t                    rnti;
    uint8_t                     harq_id;
#ifndef VZ_VERIPHY_INTEG	
    uint16_t                    pdu_length;
#endif
    uint8_t                     ul_cqi;
    uint16_t                    timing_advance;
    uint16_t                    rssi;
#ifdef VZ_VERIPHY_INTEG
    uint16_t                    pduTag;
	uint32_t					pdu_length;
	uint64_t                    pduDataOrOffset; //Shared memory offset  // 5G FAPI Table 3-61 Subset
#else
    void*                       pdu_data;
#endif
} fapi_pdu_ind_info_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_rx_data_indication
{
    fapi_msg_header_t_ue_sim    header;
    uint16_t                    sfn;
    uint16_t                    slot;
    uint16_t                    num_pdus;
    fapi_pdu_ind_info_t_ue_sim         pdus[MAX_NUMBER_OF_ULSCH_PDUS_PER_TTI];
} fapi_rx_data_indication_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_crc_ind_info
{
    uint32_t                    handle;
    uint16_t                    rnti;
    uint8_t                     harq_id;
    uint8_t                     tb_crc_status;
    uint16_t                    num_cb;
    uint8_t                     cb_crc_status[MAX_NUM_CB_PER_TTI_IN_BYTES];
    uint8_t                     ul_cqi;
    uint16_t                    timing_advance;
    uint16_t                    rssi;
} fapi_crc_ind_info_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_crc_ind
{
    fapi_msg_header_t_ue_sim    header;
    uint16_t                    sfn;
    uint16_t                    slot;
    uint16_t                    num_crcs;
    fapi_crc_ind_info_t_ue_sim         crc[MAX_NUMBER_OF_CRCS_PER_SLOT];
} fapi_crc_ind_t_ue_sim;

typedef enum PACK_STRUCT fapi_uci_mux_typ
{
   FAPI_UCI_SR      = 1,
   FAPI_UCI_HQ      = 2,
   FAPI_UCI_CSI_P1   = 4,
   FAPI_UCI_CSI_P2   = 8,
   FAPI_UCI_MUX_TYP_SR_HQ       = 3, 
   FAPI_UCI_MUX_TYP_SR_CSI_1    = 5, 
   FAPI_UCI_MUX_TYP_HQ_CSI_1    = 6,
   FAPI_UCI_MUX_TYP_SR_HQ_CSI_1 = 7
}fapi_uci_mux_typ_t;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_harq_info
{
    uint8_t                      harq_crc;
    uint16_t                     harq_bit_len;
    uint8_t                      harq_payload[MAX_HARQ_INFO_LEN_BYTES];
} fapi_harq_info_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_csi_p1_info
{
    uint8_t                     csi_part1_crc;
    uint16_t                    csi_part1_bit_len;
    uint8_t                     csi_part1_payload[MAX_CSI_PART1_DATA_BYTES];
} fapi_csi_p1_info_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_csi_p2_info
{
    uint8_t                     csi_part2_crc;
    uint16_t                    csi_part2_bit_len;
    uint8_t                     csi_part2_payload[MAX_CSI_PART2_DATA_BYTES];
} fapi_csi_p2_info_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_uci_o_pusch
{
    uint8_t                     pdu_bitmap;
    uint32_t                    handle;
    uint16_t                    rnti;
    uint8_t                     ul_cqi;
    uint16_t                    timing_advance;
    uint16_t                    rssi;
    fapi_harq_info_t_ue_sim            harqInfo; // This is included if indicated by the pduBitmap
    fapi_csi_p1_info_t_ue_sim          csiPart1info;  // This is included if indicated by the pduBitmap
    fapi_csi_p2_info_t_ue_sim          csiPart2info;  // This is included if indicated by the pduBitmap
} fapi_uci_o_pusch_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_sr_f0f1_info
{
    uint8_t                     sr_indication;
    uint8_t                     sr_confidence_level;
} fapi_sr_f0f1_info_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_harq_f0f1_info
{
    uint8_t                     num_harq;
    uint8_t                     harq_confidence_level;
    uint8_t                     harq_value[MAX_NUMBER_OF_HARQS_PER_IND];
} fapi_harq_f0f1_info_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_sr_f2f3f4_info
{
    uint16_t                    sr_bitlen;
    uint8_t                     sr_payload[MAX_SR_PAYLOAD_SIZE];
} fapi_sr_f2f3f4_info_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_harq_f2f3f4_info
{
    uint8_t                     harq_crc;
    uint16_t                    harq_bit_len;
    uint8_t                     harq_payload[MAX_HARQ_PAYLOAD_SIZE];
} fapi_harq_f2f3f4_info_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_uci_o_pucch_f2f3f4
{
    uint8_t                     pdu_bitmap;
    uint32_t                    handle;
    uint16_t                    rnti;
    uint8_t                     pucch_format;
    uint8_t                     ul_cqi;
    uint16_t                    timing_advance;
    uint16_t                    rssi;
    fapi_sr_f2f3f4_info_t_ue_sim       srInfo; // This is included if indicated by the pduBitmap
    fapi_harq_f2f3f4_info_t_ue_sim     harqInfo; // This is included if indicated by the pduBitmap
    fapi_csi_p1_info_t_ue_sim          csiPart1Info; // This is included if indicated by the pduBitmap
    fapi_csi_p2_info_t_ue_sim          csiPart2Info; // This is included if indicated by the pduBitmap
} fapi_uci_o_pucch_f2f3f4_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_uci_o_pucch_f0f1
{
    uint8_t                     pdu_bitmap;
    uint32_t                    handle;
    uint16_t                    rnti;
    uint8_t                     pucch_format;
    uint8_t                     ul_cqi;
    uint16_t                    timing_advance;
    uint16_t                    rssi;
    fapi_sr_f0f1_info_t_ue_sim         srInfo;  // This is included if indicated by the pduBitmap
    fapi_harq_f0f1_info_t_ue_sim       harqInfo;  // This is included if indicated by the pduBitmap
} fapi_uci_o_pucch_f0f1_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_uci_pdu_info
{
    uint16_t                    pdu_type;
    uint16_t                    pdu_size;
    union
    {
        fapi_uci_o_pusch_t_ue_sim         uci_pusch;
        fapi_uci_o_pucch_f0f1_t_ue_sim    uci_pucch_f0f1;
        fapi_uci_o_pucch_f2f3f4_t_ue_sim  uci_pucch_f2f3f4;
    }u;
} fapi_uci_pdu_info_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_uci_indication
{
    fapi_msg_header_t_ue_sim    header;
    uint16_t                    sfn;
    uint16_t                    slot;
    uint16_t                    num_ucis;
    fapi_uci_pdu_info_t_ue_sim         uci_pdu[] ;//max length of the array MAX_NUMBER_UCI_PDUS_PER_SLOT;
} fapi_uci_indication_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_symb_snr
{
    uint16_t                    numRbs;
    uint8_t                     rbSNR[MAX_NUMBER_RBS];
} fapi_symb_snr_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_srs_pdu
{
    uint32_t                    handle;
    uint16_t                    rnti;
    uint16_t                    timingAdvance;
    uint8_t                     numSymbols;
    uint8_t                     wideBandSnr;
    uint8_t                     numReportedSymbols;
    fapi_symb_snr_t_ue_sim             symbSnr[MAX_NUMBER_OF_REP_SYMBOLS];
} fapi_srs_pdu_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_srs_indication
{
    fapi_msg_header_t_ue_sim    header;
    uint16_t                    sfn;
    uint16_t                    slot;
    uint8_t                     numPdus;
    fapi_srs_pdu_t_ue_sim              srsPdus[MAX_NUMBER_SRS_PDUS_PER_SLOT];
} fapi_srs_indication_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_preamble_info
{
    uint8_t                     preambleIndex;
    uint16_t                    timingAdvance;
    uint32_t                    premblePwr;
#ifdef FAPI_222_10_03
    uint8_t		        preambleSnr;
#endif
} fapi_preamble_info_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_rach_pdu
{
#ifdef FAPI_222_10_03
    uint16_t                    handle;
#else
    uint16_t                    physCellId;
#endif
    uint8_t                     symbolIndex;
    uint8_t                     slotIndex;
#ifdef FAPI_222_10_03
    uint8_t                     raIndex;
#else
    uint8_t                     freqIndex;
#endif
#ifdef VZ_VERIPHY_INTEG
    uint8_t                     ulCarrierId;
#endif
#ifdef FAPI_222_10_03
   uint16_t                     avgRssi;
#else
   uint8_t                      avgRssi;
#endif
   uint8_t                      avgSnr;       
   uint8_t                      numPreamble;
   fapi_preamble_info_t_ue_sim         preambleInfo[MAX_NUM_PREAMBLES_PER_SLOT];
} fapi_rach_pdu_t_ue_sim;

// Updated per 5G FAPI
typedef struct PACK_STRUCT fapi_rach_indication
{
    fapi_msg_header_t_ue_sim    header;
    uint16_t                    sfn;
    uint16_t                    slot;
    uint8_t                     numPdus;
    fapi_rach_pdu_t_ue_sim             rachPdu[MAX_NUMBER_RACH_PDUS_PER_SLOT];
} fapi_rach_indication_t_ue_sim;

/*!
 ** \struct      pkd_fapi_l1api_msg_frmt
 ** \brief       This structure is FAPI L1API message format towards L1.
 **/
typedef struct PACK_STRUCT  pkd_fapi_l1api_msg_frmt
{
    /*!
     *         Message Type ID
     **/
    uint8_t    msg_id;
    /**
     *         Length of vendor-specific message body (bytes)
     **/
    uint8_t    vendor_spec_msg_len;
    /**
     *         Length of message body (bytes)
     **/
    uint16_t   msg_len;
    /**
     *         Message Body.(Dummy Field)
     **/
    uint8_t*   msg_body;
    /**
     *         Vendor message Body.(Dummy Field)
     **/
    uint8_t*   vendor_spec_msg_body;

} pkd_fapi_l1api_msg_frmt_t;




//------------------------------------------------------------------------------

#if defined(__cplusplus)
}
#endif
#endif
