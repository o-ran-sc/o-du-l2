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
/**
 * @file
 * This file consist of FAPI configuration APIs macros, structure typedefs and
 * prototypes.
 *
 **/

#ifndef _FAPI_H_
#define _FAPI_H_

#include <stdint.h>

#include "fapi_interface.h"
//#include "fapi_vendor_common_defs.h"

#define RELEASE_15                                  0x0001

// Datatypes typedefs - end

typedef enum {
    FAPI_SUCCESS = 0,
    FAPI_FAILURE
} fapiStatus_t;
// Updated per 5G FAPI



// Updated per 5G FAPI    
typedef enum {
    FAPI_UL_TTI_REQ_PRACH_PDU_TYPE = 0,
    FAPI_UL_TTI_REQ_PUSCH_PDU_TYPE,
    FAPI_UL_TTI_REQ_PUCCH_PDU_TYPE,
    FAPI_UL_TTI_REQ_SRS_PDU_TYPE
}fapiULTtiReqPduType_e;
// Updated per 5G FAPI
typedef enum {
    FAPI_UCI_IND_ON_PUSCH_PDU_TYPE = 0,
    FAPI_UCI_IND_ON_PUCCH_FMT_0_1_PDU_TYPE,
    FAPI_UCI_IND_ON_PUCCH_FMT_2_3_4_PDU_TYPE
}fapiUciIndPdu_Type_e;
    
// CRC
enum {
    FAPI_CRC_CORRECT = 0,
    FAPI_CRC_ERROR = 1
};

//------------------------------------------------------------------------------
// Fapi Infra Declarations
//------------------------------------------------------------------------------
// Release/Features support
typedef enum {
    FAPI_NOT_SUPPORTED = 0,
    FAPI_SUPPORTED,
} fapiSupport_t;

// FAPI States
/**
 * FAPI state is maintained per fapi instance. If FAPI messages are received in
 * wrong state an ERROR.indication message will be sent by FAPI.
 */
typedef enum fapiStates
{
    FAPI_STATE_IDLE = 0,
    FAPI_STATE_CONFIGURED,
    FAPI_STATE_RUNNING
} fapiStates_t;

// Information of optional and mandatory status for a TLV
typedef enum {
    FAPI_IDLE_STATE_ONLY_OPTIONAL = 0,
    FAPI_IDLE_STATE_ONLY_MANDATORY,
    FAPI_IDLE_AND_CONFIGURED_STATES_OPTIONAL,
    FAPI_IDLE_STATE_MANDATORY_CONFIGURED_STATE_OPTIONAL,
    FAPI_IDLE_CONFIGURED_AND_RUNNING_STATES_OPTIONAL,
    FAPI_IDLE_STATE_MANDATORY_CONFIGURED_AND_RUNNING_STATES_OPTIONAL
} fapiTlvStatus_t;

// PARAMETERS INFORMATION

#define FAPI_NORMAL_CYCLIC_PREFIX_MASK              0x01
#define FAPI_EXTENDED_CYCLIC_PREFIX_MASK            0x02

// In 5G FAPI FrameDuplexType as part of Cell Configuration
typedef enum
{
    TDD_DUPLEX = 0,
    FDD_DUPLEX
} modes;     //Defined now

// Subcarrier spacing information
#define FAPI_15KHZ_MASK                             0x01
#define FAPI_30KHZ_MASK                             0x02
#define FAPI_60KHZ_MASK                             0x04
#define FAPI_120KHZ_MASK                            0x08

// Bandwitdth information
#define FAPI_5MHZ_BW_MASK                           0x0001
#define FAPI_10MHZ_BW_MASK                          0x0002
#define FAPI_15MHZ_BW_MASK                          0x0004
#define FAPI_20MHZ_BW_MASK                          0x0010
#define FAPI_40MHZ_BW_MASK                          0x0020
#define FAPI_50MHZ_BW_MASK                          0x0040
#define FAPI_60MHZ_BW_MASK                          0x0080
#define FAPI_70MHZ_BW_MASK                          0x0100
#define FAPI_80MHZ_BW_MASK                          0x0200
#define FAPI_90MHZ_BW_MASK                          0x0400
#define FAPI_100MHZ_BW_MASK                         0x0800
#define FAPI_200MHZ_BW_MASK                         0x1000
#define FAPI_400MHZ_BW_MASK                         0x2000


// PDCCH Information
#define FAPI_CCE_MAPPING_INTERLEAVED_MASK           0x01
#define FAPI_CCE_MAPPING_NONINTERLVD_MASK           0x02
// Upper Bound for PDCCH Channels per Slot
#define FAPI_MAX_PDCCHS_PER_SLOT_MASK               0xff

// PUCCH Information
#define FAPI_FORMAT_0_MASK                          0x01
#define FAPI_FORMAT_1_MASK                          0x02
#define FAPI_FORMAT_2_MASK                          0x04
#define FAPI_FORMAT_3_MASK                          0x08
#define FAPI_FORMAT_4_MASK                          0x10
// Upper Bound for PUCCH Channels per Slot
#define FAPI_MAX_PUCCHS_PER_SLOT_MASK               0xff

// PDSCH Information
#define FAPI_PDSCH_MAPPING_TYPE_A_MASK              0x01
#define FAPI_PDSCH_MAPPING_TYPE_B_MASK              0x02
#define FAPI_PDSCH_ALLOC_TYPE_0_MASK                0x01
#define FAPI_PDSCH_ALLOC_TYPE_1_MASK                0x02
#define FAPI_PDSCH_VRB_TO_PRB_MAP_NON_INTLV_MASK    0x01
#define FAPI_PDSCH_VRB_TO_PRB_MAP_INTLVD_MASK       0x02
#define FAPI_PDSCH_DMRS_CONFIG_TYPE_1_MASK          0x01
#define FAPI_PDSCH_DMRS_CONFIG_TYPE_2_MASK          0x02
#define FAPI_PDSCH_DMRS_MAX_LENGTH_1                0
#define FAPI_PDSCH_DMRS_MAX_LENGTH_2                1
#define FAPI_DMRS_ADDITIONAL_POS_0_MASK             0x01
#define FAPI_DMRS_ADDITIONAL_POS_1_MASK             0x02
#define FAPI_DMRS_ADDITIONAL_POS_2_MASK             0x04
#define FAPI_DMRS_ADDITIONAL_POS_3_MASK             0x08
//Upper Limit for PDSCHS TBs per Slot
#define FAPI_MAX_PDSCHS_TBS_PER_SLOT_MASK           0xff
#define FAPI_MAX_NUMBERMIMO_LAYERS_PDSCH            2

typedef enum modulationOrder {
    FAPI_QPSK                                       = 0,
    FAPI_16QAM,
    FAPI_64QAM,
    FAPI_256QAM
} fapiModOrder_t;

#define FAPI_MAX_MUMIMO_USERS_MASK                  0xff


// PUSCH Parameters

#define FAPI_PUSCH_MAPPING_TYPE_A_MASK              0x01
#define FAPI_PUSCH_MAPPING_TYPE_B_MASK              0x02
#define FAPI_PUSCH_ALLOC_TYPE_0_MASK                0x01
#define FAPI_PUSCH_ALLOC_TYPE_1_MASK                0x02
#define FAPI_PUSCH_VRB_TO_PRB_MAP_NON_INTLV_MASK    0x01
#define FAPI_PUSCH_VRB_TO_PRB_MAP_INTLVD_MASK       0x02
#define FAPI_PUSCH_DMRS_CONFIG_TYPE_1_MASK          0x01
#define FAPI_PUSCH_DMRS_CONFIG_TYPE_2_MASK          0x02
#define FAPI_PUSCH_DMRS_MAX_LENGTH_1                0
#define FAPI_PUSCH_DMRS_MAX_LENGTH_2                1
// Upper limit for PUSCHMAXPTRSPORTS
#define FAPI_PUSCH_MAX_PTRS_PORTS_UB                2
//Upper Limit for PDSCHS TBs per Slot
#define FAPI_MAX_PUSCHS_TBS_PER_SLOT_MASK           0xff

typedef enum aggregationFactor
{
    FAPI_PUSCH_AGG_FACTOR_1                         = 0,
    FAPI_PUSCH_AGG_FACTOR_2,
    FAPI_PUSCH_AGG_FACTOR_4,
    FAPI_PUSCH_AGG_FACTOR_8
} fapiPuschAggFactor_t;

// PRACH Parameters
#define FAPI_PRACH_LF_FORMAT_0_MASK                 0x01
#define FAPI_PRACH_LF_FORMAT_1_MASK                 0x02
#define FAPI_PRACH_LF_FORMAT_2_MASK                 0x04
#define FAPI_PRACH_LF_FORMAT_3_MASK                 0x08

#define FAPI_PRACH_SF_FORMAT_A1_MASK                0x01
#define FAPI_PRACH_SF_FORMAT_A2_MASK                0x02
#define FAPI_PRACH_SF_FORMAT_A3_MASK                0x04
#define FAPI_PRACH_SF_FORMAT_B1_MASK                0x08
#define FAPI_PRACH_SF_FORMAT_B2_MASK                0x10
#define FAPI_PRACH_SF_FORMAT_B3_MASK                0x20
#define FAPI_PRACH_SF_FORMAT_B4_MASK                0x40
#define FAPI_PRACH_SF_FORMAT_C0_MASK                0x80
#define FAPI_PRACH_SF_FORMAT_C2_MASK                0x100

typedef enum {
    FAPI_MAX_PRACH_FD_OCC_IN_A_SLOT_1               = 0,
    FAPI_MAX_PRACH_FD_OCC_IN_A_SLOT_2,
    FAPI_MAX_PRACH_FD_OCC_IN_A_SLOT_4,
    FAPI_MAX_PRACH_FD_OCC_IN_A_SLOT_8
}  fapi_prachMaxFdOccasionsPerSlot_t;      

// Measurement Parameters
#define FAPI_RSSI_REPORT_IN_DBM_MASK                0x01
#define FAPI_RSSI_REPORT_IN_DBFS_MASK               0x02

// CONFIGURATION INFORMATION
// CARRIER CONFIGURATION
// BANDWIDTH
#define FAPI_BANDWIDTH_5_MHZ                        5
#define FAPI_BANDWIDTH_10_MHZ                       10
#define FAPI_BANDWIDTH_15_MHZ                       15
#define FAPI_BANDWIDTH_20_MHZ                       20
#define FAPI_BANDWIDTH_25_MHZ                       25
#define FAPI_BANDWIDTH_30_MHZ                       30
#define FAPI_BANDWIDTH_40_MHZ                       40
#define FAPI_BANDWIDTH_50_MHZ                       50
#define FAPI_BANDWIDTH_60_MHZ                       60
#define FAPI_BANDWIDTH_70_MHZ                       70
#define FAPI_BANDWIDTH_80_MHZ                       80
#define FAPI_BANDWIDTH_90_MHZ                       90
#define FAPI_BANDWIDTH_100_MHZ                      100
#define FAPI_BANDWIDTH_200_MHZ                      200
#define FAPI_BANDWIDTH_400_MHZ                      400

// Frequency needs to track 38.104 Section 5.2 and 38.211 Section 5.3.1
// Lower Bound KHz
#define FAPI_MIN_FREQUENCY_PT_A                     450000
// Upper Bound KHz
#define FAPI_MAX_FREQUENCY_PT_A                     52600000
// dlk0, ulk0 per 38.211 Section 5.3.1
// Upper Bound
#define FAPI_K0_MAX                                 23699
// dlGridSize, ulGridSize per 38.211 Section 4.4.2
// Upper Bound
#define FAPI_GRIDSIZE_MAX                           275
// Number of Transmit Antennas
// Define upper mask based on variable type
#define FAPI_NUM_ANT_MASK                           0xffff

// CELL CONFIGURATION
// Physical Cell ID from 38.211 Section 7.4.2.1
// Upper Bound
#define FAPI_MAX_CELL_ID                            1007

// SSB CONFIGURATION
// SSB POWER RANGE in dBm
#define FAPI_SS_PBCH_LOWEST_POWER                   -60
#define FAPI_SS_PBCH_MAX_POWER                      50
// BCH PAYLOAD  for 5G the MAC always generates the BCH Payload
#define FAPI_BCH_PAYLOAD_GEN_BY_MAC                 0
#define FAPI_BCH_PAYLOAD_WITH_PHY_GEN_TIMING        1
#define FAPI_BCH_PAYLOAD_ENTIRELY_GEN_BY_PHY        2
// ScsCommon
#define FAPI_SCSCOMMON_MASK                         0x03

// PRACH CONFIGURATION
#define FAPI_PRACH_LONG_SEQUENCE                    0
#define FAPI_PRACH_SHORT_SEQUENCE                   1
#define FAPI_PRACH_SUBC_SPACING_MAX                 4
// Restricted Set Configuration
#define FAPI_PRACH_RESTRICTED_SET_UNRESTRICTED      0
#define FAPI_PRACH_RESTRICTED_SET_TYPE_A            1
#define FAPI_PRACH_RESTRICTED_SET_TYPE_B            2    
// Root Sequence Index
// Upper Bound
#define FAPI_PRACH_ROOT_SEQ_INDEX_MAX               837         
// k1
// Upper Bound
#define FAPI_K1_UPPER_BOUND                         272
// PRACH Zero Corr Configuration
// Upper Bound
#define FAPI_PRACHZEROCORRCONF_MASK                 0x0f
// Number of Unused Root Sequences Mask
#define FAPI_UNUSEDROOTSEQUENCES_MASK               0x0f
// SSBPERRACH
typedef enum 
{
    FAPI_SSB_PER_RACH_1_OVER_8                      = 0,
    FAPI_SSB_PER_RACH_1_OVER_4,
    FAPI_SSB_PER_RACH_1_OVER_2,
    FAPI_SSB_PER_RACH_1,
    FAPI_SSB_PER_RACH_2,
    FAPI_SSB_PER_RACH_4,
    FAPI_SSB_PER_RACH_8,
    FAPI_SSB_PER_RACH_16
} fapiSsbPerRach_t;     

// SSB Table
// Ssb Offset Point A max
#define FAPI_SSB_OFFSET_POINTA_MAX                  2199
// betaPSS  i.e. PSS EPRE to SSS EPRE in a SS/PBCH Block per 38.213 Section 4.1
#define FAPI_BETAPSS_0_DB                           0
#define FAPI_BETAPSS_3_DB                           1
// SSB Period in ms
#define FAPI_SSB_PERIOD_5_MS                        0
#define FAPI_SSB_PERIOD_10_MS                       1
#define FAPI_SSB_PERIOD_20_MS                       2
#define FAPI_SSB_PERIOD_40_MS                       3
#define FAPI_SSB_PERIOD_80_MS                       4
#define FAPI_SSB_PERIOD_160_MS                      5

// Ssb Subcarrier Offset    per 38.211 Section 7.4.3.1
// SsbSubcarrierOffset mask
#define FAPI_SSB_SUBCARRIER_OFFSET_MASK             0x1f
// MIB PAYLOAD MASK
#define MIB_PAYLOAD_MASK                            0xfff0
// BEAM ID MASK
#define FAPI_BEAM_ID_MASK                           0x3f

// TDD Table
// TDD Period
#define FAPI_TDD_PERIOD_0_P_5_MS                        0
#define FAPI_TDD_PERIOD_0_P_625_MS                      1
#define FAPI_TDD_PERIOD_1_MS                            2
#define FAPI_TDD_PERIOD_1_P_25_MS                       3
#define FAPI_TDD_PERIOD_2_MS                            4
#define FAPI_TDD_PERIOD_2_P_5_MS                        5
#define FAPI_TDD_PERIOD_5_MS                            6
#define FAPI_TDD_PERIOD_10_MS                           7

// Slot Configuration
#define FAPI_DL_SLOT                                    0
#define FAPI_UL_SLOT                                    1
#define FAPI_GUARD_SLOT                                 2

// Measurement configuration
#define FAPI_NO_RSSI_REPORTING                          0
#define FAPI_RSSI_REPORTED_IN_DBM                       1
#define FAPI_RSSI_REPORTED_IN_DBFS                      2

// Error Indication
#define FAPI_SFN_MASK                                   0x03ff

// Status and Error Codes for either .response or ERROR.indication
// Updated per 5g FAPI Table 3-31
/*
typedef enum {
    MSG_OK = 0,
    MSG_INVALID_STATE,
    MSG_INVALID_CONFIG,
    SFN_OUT_OF_SYNC,
    MSG_SLOT_ERR,
    MSG_BCH_MISSING,
    MSG_INVALID_SFN,
    MSG_UL_DCI_ERR, 
    MSG_TX_ERR
 }fapiStatusAndErrorCodes_e;
*/
 // Digital Beam Table (DBT) PDU
 // Number of Digital Beam Mask
 // Number of TX RUS Mask
 // Beam Index Mask
 // Digital Beam Index weights Real and Imaginary Mask
 
 // Precoding Matrix (PM) PDU
 // Precoding Matrix ID Mask
 // Number of Layers Mask
 // Number of Antenna Ports at the precoder output Mask
 // Precoder Weights Real and Imaginary Mask
 #define FAPI_U16_MASK                                  0xffff
 
 // Slot Indication
 
 #define FAPI_SLOT_MAX_VALUE                            319
 
 // DL_TTI.request
 // nPDUS mask
 // nGroup mask
 #define FAPI_U8_MASK                                   0xff
 
 typedef enum {
    FAPI_DL_TTI_REQ_PDCCH_PDU_TYPE = 0,
    FAPI_DL_TTI_REQ_PDSCH_PDU_TYPE,
    FAPI_DL_TTI_REQ_CSI_RS_PDU_TYPE,
    FAPI_DL_TTI_REQ_SSB_PDU_TYPE
}fapiDlTtiReqPduType_e; 

// nUe
// Define Maximum number of Ues per Group
#define FAPI_MAX_NUMBER_OF_UES_PER_GROUP                12

// PDCCH PDU
#define FAPI_BWPSIZE_MAX                                275
#define FAPI_BWPSIZE_START_MAX                          274
#define FAPI_SUBCARRIER_SPACING_MAX                     4
#define FAPI_CYCLIC_PREFIX_NORMAL                       0
#define FAPI_CYCLIC_PREFIX_EXTENDED                     1
#define FAPI_MAX_SYMBOL_START_INDEX                     13

#define FAPI_CORESET_DURATION_1_SYMBOL                  1
#define FAPI_CORESET_DURATION_2_SYMBOLS                 2
#define FAPI_CORESET_DURATION_3_SYMBOLS                 3

#define FAPI_CCE_REG_MAPPING_TYPE_NON_INTERLEAVED       0
#define FAPI_CCE_REG_MAPPING_TYPE_INTERLEAVED           1
#define FAPI_REG_BUNDLE_SIZE_2                          2
#define FAPI_REG_BUNDLE_SIZE_3                          3
#define FAPI_REG_BUNDLE_SIZE_6                          6

#define FAPI_INTERLEAVER_SIZE_2                         2
#define FAPI_INTERLEAVER_SIZE_3                         3
#define FAPI_INTERLEAVER_SIZE_6                         6

#define FAPI_CORESET_TYPE_0_CONF_BY_PBCH_OR_SIB1        0
#define FAPI_CORESET_TYPE_1                             1

#define FAPI_PREC_GRANULARITY_SAME_AS_REG_BUNDLE        0
#define FAPI_PREC_GRANULARITY_ALL_CONTIG_RBS            1

#define FAPI_CCE_INDEX_MAX                              135
#define FAPI_PDCCH_AGG_LEVEL_1                          1
#define FAPI_PDCCH_AGG_LEVEL_2                          2
#define FAPI_PDCCH_AGG_LEVEL_4                          4
#define FAPI_PDCCH_AGG_LEVEL_8                          8
#define FAPI_PDCCH_AGG_LEVEL_16                         16

#define FAPI_BETA_PDCCH_1_0_MAX                         17

#define FAPI_POWER_CTRL_OFF_SS_MINUS_3_DB               0
#define FAPI_POWER_CTRL_OFF_SS_0_DB                     1
#define FAPI_POWER_CTRL_OFF_SS_3_DB                     2
#define FAPI_POWER_CTRL_OFF_SS_6_DB                     3

#define FAPI_MAX_NUMBER_OF_CODEWORDS                    2

#define FAPI_MAX_MCS_INDEX                              31
#define FAPI_MCS_INDEX_MASK                             0x1f

#define FAPI_MCS_TABLE_NOT_QAM_256                      0
#define FAPI_MCS_TABLE_QAM_256                          1
#define FAPI_MCS_TABLE_QAM_64_LOW_SE                    2

#define FAPI_REDUNDANCY_INDEX_MASK                      0x03
#define FAPI_MAX_DL_LAYERS                              8

#define FAPI_TRANSMISSION_SCHEME_1                      1

#define FAPI_REF_POINT_FOR_PDSCH_DMRS_AT_PT_A           0
#define FAPI_REF_POINT_FOR_PDSCH_DMRS_AT_LOWEST_ALLOC   1

#define FAPI_DL_DMRS_SYMB_POS_MASK                      0x3fff

#define FAPI_MAX_DMRS_CDM_GRPS_WO_DATA                  3

#define FAPI_DMRS_PORTS_MASK                            0x0fff

#define FAPI_RES_ALLOC_TYPE_0                           0
#define FAPI_RES_ALLOC_TYPE_1                           1

#define FAPI_VRB_TO_PRB_MAP_NON_INTERLVD                0
#define FAPI_VRB_TO_PRB_MAP_INTERLVD_RB_SIZE_2          1
#define FAPI_VRB_TO_PRB_MAP_INTERLVD_RB_SIZE_4          2

#define FAPI_MAX_START_SYMBOL_INDEX                     13
#define FAPI_MAX_NR_OF_SYMBOLS                          14
#define FAPI_PTRS_PORT_INDEX_MASK                       0x3f
#define FAPI_PTRS_TIME_DENSITY_1                        0
#define FAPI_PTRS_TIME_DENSITY_2                        1
#define FAPI_PTRS_TIME_DENSITY_4                        2
#define FAPI_PTRS_FREQ_DENSITY_2                        0
#define FAPI_PTRS_FREQ_DENSITY_4                        1
#define FAPI_PTRS_RE_OFFSET_MASK                        0x03
#define FAPI_EPRE_RATIO_PDSCH_PTRS_MASK                 0x03
// PDSCH Power Control Offset
#define FAPI_PWR_CTRL_OFFSET_MINUS_8_DB                 0
#define FAPI_PWR_CTRL_OFFSET_MINUS_7_DB                 1
#define FAPI_PWR_CTRL_OFFSET_MINUS_6_DB                 2
#define FAPI_PWR_CTRL_OFFSET_MINUS_5_DB                 3
#define FAPI_PWR_CTRL_OFFSET_MINUS_4_DB                 4
#define FAPI_PWR_CTRL_OFFSET_MINUS_3_DB                 5
#define FAPI_PWR_CTRL_OFFSET_MINUS_2_DB                 6
#define FAPI_PWR_CTRL_OFFSET_MINUS_1_DB                 7
#define FAPI_PWR_CTRL_OFFSET_0_DB                       8
#define FAPI_PWR_CTRL_OFFSET_1_DB                       9
#define FAPI_PWR_CTRL_OFFSET_2_DB                       10
#define FAPI_PWR_CTRL_OFFSET_3_DB                       11
#define FAPI_PWR_CTRL_OFFSET_4_DB                       12
#define FAPI_PWR_CTRL_OFFSET_5_DB                       13
#define FAPI_PWR_CTRL_OFFSET_6_DB                       14
#define FAPI_PWR_CTRL_OFFSET_7_DB                       15
#define FAPI_PWR_CTRL_OFFSET_8_DB                       16
#define FAPI_PWR_CTRL_OFFSET_9_DB                       17
#define FAPI_PWR_CTRL_OFFSET_10_DB                      18
#define FAPI_PWR_CTRL_OFFSET_11_DB                      19
#define FAPI_PWR_CTRL_OFFSET_12_DB                      20
#define FAPI_PWR_CTRL_OFFSET_13_DB                      21
#define FAPI_PWR_CTRL_OFFSET_14_DB                      22
#define FAPI_PWR_CTRL_OFFSET_15_DB                      23
// Power Control Offset SS
#define FAPI_PWR_CTRL_OFFSET_SS_MINUS_3_DB              0
#define FAPI_PWR_CTRL_OFFSET_SS_0_DB                    1
#define FAPI_PWR_CTRL_OFFSET_SS_3_DB                    2
#define FAPI_PWR_CTRL_OFFSET_SS_6_DB                    3
// CSI Type
#define FAPI_CSI_TRS                                    0
#define FAPI_CSI_NON_ZERO_POWER                         1
#define FAPI_CSI_ZERO_POWER                             2
// Row entry into CSI Resource Location Table
#define FAPI_CSIRLT_ROW_MAX_VALUE                       18
#define FAPI_CSI_FREQ_DOMAIN_MASK                       0x0fff
#define FAPI_CSI_SYMB_L1_MIN                            2
#define FAPI_CSI_SYMB_L1_MAX                            12
// CDM Type
#define FAPI_CDM_TYPE_NO_CDM                            0
#define FAPI_CDM_TYPE_FD_CDM                            1
#define FAPI_CDM_TYPE_CDM4_FD2_TD2                      2
#define FAPI_CDM_TYPE_CDM8_FD2_TD4                      3
// Frequency Density
#define FAPI_FD_DOT5_EVEN_RB                            0
#define FAPI_FD_DOT5_ODD_RB                             1
#define FAPI_FD_ONE                                     2
#define FAPI_FD_THREE                                   3

// SSB
#define FAPI_SSB_BLOCK_INDEX_MASK                       0x3f
#define FAPI_SSB_SC_OFFSET_MASK                         0x1f

// UL TTI REQUEST
#define FAPI_MAX_NUM_UE_GROUPS_INCLUDED                 8
#define FAPI__MAX_NUM_UE_IN_GROUP                       6
// PRACH PDU
#define FAPI_MAX_NUM_PRACH_OCAS                         7
// PRACH FORMAT
#define FAPI_PRACH_FORMAT_A1                            0
#define FAPI_PRACH_FORMAT_A2                            1
#define FAPI_PRACH_FORMAT_A3                            2
#define FAPI_PRACH_FORMAT_B1                            3
#define FAPI_PRACH_FORMAT_B2                            4
#define FAPI_PRACH_FORMAT_B3                            5
#define FAPI_PRACH_FORMAT_B4                            6
#define FAPI_PRACH_FORMAT_C0                            7
#define FAPI_PRACH_FORMAT_C2                            8

#define FAPI_MAX_PRACH_FD_OCCASION_INDEX                7
#define FAPI_MAX_ZC_ZONE_CONFIG_NUMBER                  419

// PUSCH PDU
#define FAPI_PUSCH_BIT_DATA_PRESENT_MASK                0x0001
#define FAPI_PUSCH_UCI_DATA_PRESENT_MASK                0x0002
#define FAPI_PUSCH_PTRS_INCLUDED_FR2_MASK               0x0004
#define FAPI_PUSCH_DFTS_OFDM_TX_MASK                    0x0008

#define FAPI_MAX_QAM_MOD_ORDER                          8
#define FAPI_MCS_INDEX_MASK                             0x1f

#define FAPI_MCS_TABLE_NOT_QAM256                       0
#define FAPI_MCS_TABLE_QAM256                           1
#define FAPI_MCS_TABLE_QAM64_LOWSE                      2
#define FAPI_MCS_TABLE_NOT_QAM256_W_XFRM_PRECOD         3
#define FAPI_MCS_TABLE_QAM64_LOWSE_W_XFRM_PRECOD        4
#define FAPI_PUSCH_MAX_NUM_LAYERS                       4
// DMRS
#define FAPI_UL_DMRS_SYMB_POS_MASK                      0x3fff
#define FAPI_UL_DMRS_CONFIG_TYPE_1                      0
#define FAPI_UL_DMRS_CONFIG_TYPE_2                      1
#define FAPI_MAX_DMRS_CDM_GRPS_NO_DATA                  3
#define FAPI_UL_DMRS_PORTS_MASK                         0x07ff
#define FAPI_UL_TX_DIRECT_CURR_LOCATION_MAX             3299
#define FAPI_UL_TX_DIRECT_CURR_LOC_OUTSIDE_CARRIER      3300
#define FAPI_UL_TX_DIRECT_CURR_LOC_UNDETERMINED         3301
// PUSCH DATA
#define FAPI_RV_INDEX_MASK                              0x03
#define FAPI_HARQ_PROCESS_ID_MASK                       0x0f
// PUSCH UCI INFO
#define FAPI_HARQ_ACK_CSI_PX_BIT_LEN_SMALL_BLOCK_MAX    11
#define FAPI_HARQ_ACK_CSI_PX_BIT_LEN_POLAR_MAX          1706
// ALPHA SCALING
#define FAPI_ALPHA_SCALE_0_5                            0
#define FAPI_ALPHA_SCALE_0_65                           1
#define FAPI_ALPHA_SCALE_0_8                            2
#define FAPI_ALPHA_SCALE_1_0                            3
// BETA OFFSET HARQ ACK
#define FAPI_BETA_OFFSET_HARQ_ACK_MAX                   15
#define FAPI_BETA_OFFSET_CSIX_MAX                       18

// PUSCH PTRS INFORMATION 38.212 Section 7.3.1.1.2
#define FAPI_MAX_NUMBER_PTRS_PORT_INDEX                 11  // 0..11
// UL PTRS POWER 5G FAPI Table 3-49
#define FAPI_UL_PTRS_PWR_0_DB                           0
#define FAPI_UL_PTRS_PWR_3_DB                           1
#define FAPI_UL_PTRS_PWR_4_77_DB                        2
#define FAPI_UL_PTRS_PWR_6_DB                           3
// DFTSOFDM INFO 5g FAPI Table 3-50
#define FAPI_MAX_LOW_PAPR_GROUP_NUMBER                  29  // 0..29
#define FAPI_MAX_LOW_PAPR_SEQ_NUMBER                    87  // 3*LOW_PAPR_GRP_NUM
#define FAPI_MAX_UL PTRS_SAMP_DENSITY                   8
#define FAPI_MAX_UL_PTRS_TD_XFRM_PRECOD                 4

// PUCCH PDU Table 3-51
#define FAPI_MAX_PUCCH_FORMAT_TYPE                      4
#define FAPI_MULTI_SLOT_TX_IND_NO_MULTI_SLOT            0
#define FAPI_MULTI_SLOT_TX_IND_TX_START                 1
#define FAPI_MULTI_SLOT_TX_IND_TX_CONT                  2
#define FAPI_MULTI_SLOT_TX_IND_TX_END                   3
#define FAPI_MAX_NUM_PRB_FOR_A_PUCCH                    16
#define FAPI_MAX_PUCCH_DUR_F0_AND_F2                    2
#define FAPI_MIN_PUCCH_DUR_F1_F3_F4                     4
#define FAPI_MAX_PUCCH_DUR_F1_F3_F4                     14
#define FAPI_MAX_INIT_CYCLIC_SHIFT_F0_F1_F3_F4          11
#define FAPI_MAX_OCC_INDEX_F1                           6
#define FAPI_MAX_PRE_DFT_OCC_IDX_F4                     3
#define FAPI_MAX_PRE_DFT_OCC_LEN_F4                     4
#define FAPI_MAX_DMRS_CYC_SHIFT_F4                      9
#define FAPI_BIT_LEN_HARQ_PL_ZERO                       0
#define FAPI_BIT_LEN_HARQ_PL_F0_F1_2_BITS               1
#define FAPI_BIT_LEN_HARQ_PL_F2_F3_F4_1706_BITS         2
#define FAPI_BIT_LEN_CSI_PX_PL_NO_CSI                   0
#define FAPI_BIT_LEN_CSI_PX_PL_1706_BITS                1

// SRS PDU
#define FAPI_1_SRS_ANT_PORT                             0
#define FAPI_2_SRS_ANT_PORTS                            1
#define FAPI_4_SRS_ANT_PORTS                            2
#define FAPI_SRS_NO_REPETITIONS                         0
#define FAPI_SRS_2_REPETITIONS                          2
#define FAPI_SRS_4_REPETITIONS                          4
#define FAPI_SRS_CONFIG_INDEX_MASK                      0x3f
#define FAPI_SRS_BW_INDEX_MASK                          0x03
#define FAPI_TX_COMB_SIZE_2                             0
#define FAPI_TX_COMB_SIZE_4                             1
#define FAPI_MAX_SRS_FREQ_POSITION                      67
#define FAPI_MAX_SRS_FD_SHIFT                           268
#define FAPI_SRS_FREQ_HOPPING_MASK                      0x03
#define FAPI_SRS_NO_HOPPING                             0
#define FAPI_SRS_GRP_OR_SEQ_HOPPING                     1
#define FAPI_SRS_SEQ_HOPPING                            2
#define FAPI_SRS_RES_ALLOC_APERIODIC                    0
#define FAPI_SRS_RES_ALLOC_SEMI_PERSISTENT              1
#define FAPI_SRS_RES_ALLOC_PERIODIC                     2
#define FAPI_MAX_LSOT_OFFSET_VALUE                      2559

// RX_DATA Indication
#define FAPI_UL_CQI_INVALID                             255
#define FAPI_TIMING_ADVANCE_INVALID                     0xffff
#define FAPI_MAX_TIMING_ADVANCE                         63
#define FAPI_MAX_RSSI                                   1280


// RACH Indication
#define FAPI_RACH_FREQ_INDEX_MAX                        7
#define FAPI_RACH_DETECTED_PREAMBLES_MASK               0x3f
#define FAPI_RACH_TIMING_ADVANCE_MAX                    3846
#define FAPI_RACH_PREAMBLE_POWER_INVALID                0xffffffff
#define FAPI_RACH_PREAMBLE_TIMING_ADVANCE_INVALID       0xffff
#define FAPI_RACH_PREAMBLE_POWER_MAX                    170000

// SR, HARQ, and CSI Part 1/2 PDUs Table 3-66
#define FAPI_SR_MASK                                    0x01
#define FAPI_HARQ_MASK                                  0x02
#define FAPI_CSI_PART1                                  0x04
#define FAPI_CSI_PART2                                  0x08
#define FAPI_PUCCH_FORMAT2                              0
#define FAPI_PUCCH_FORMAT3                              1
#define FAPI_PUCCH_FORMAT4                              2
#define FAPI_PUCCH_FORMAT_MASK                          0x03

// SR PDU For Format 0 or 1 Table 3-67
#define FAPI_SR_CONFIDENCE_LEVEL_GOOD                   0
#define FAPI_SR_CONFIDENCE_LEVEL_BAD                    1
#define FAPI_SR_CONFIDENCE_LEVEL_INVALID                0xff

// HARQ PDU for Format 0 or 1 Table 3-68
#define FAPI_HARQ_VALUE_PASS                            0
#define FAPI_HARQ_VALUE_FAIL                            1
#define FAPI_HARQ_VALUE_NOT_PRESENT                     2

// SR PDU for Format 2,3 or 4 Table 3-69
#define FAPI_SR_PAYLOAD_MAX                             1

// HARQ PDU for Format 2,3 or 4 Table 3-70
#define FAPI_HARQ_CRC_PASS                              0
#define FAPI_HARQ_CRC_FAIL                              1
#define FAPI_HARQ_CRC_NOT_PRESENT                       2
#define FAPI_HARQ_PAYLOAD_MAX                           214


// CSI Part 1 PDU Table 3-71 and 3-72
#define FAPI_CSI_PARTX_CRC_PASS                         0
#define FAPI_CSI_PARTX_CRC_FAIL                         1
#define FAPI_CSI_PARTX_CRC_NOT_PRESENT                  2
#define FAPI_CSI_PARTX_PAYLOAD_MAX                      214

#if 0
//------------------------------------------------------------------------------
// FAPI callback functions to be implemented by the user
//------------------------------------------------------------------------------
/**
 *  fapi callback structure is passed as part of ``fapi_create``. FAPI will call
 *  these functions in response to any received request message.
 *
 *  *Note: vendor specific callbacks are only valid in TIMER_MODE. Must be set
 *  to NULL in RADIO mode.*
 */
typedef struct {
    void  (*fapi_param_response)   (fapiInstanceHdl_t  fapiHdl,
                                        pfapiParamResp_t    resp);
    void  (*fapi_config_response)  (fapiInstanceHdl_t  fapiHdl,
                                        pfapiConfigResp_t   resp);
    void  (*fapi_stop_ind)         (fapiInstanceHdl_t  fapiHdl,
                                        pfapiStopInd_t      resp);
    void  (*fapi_error_ind)        (fapiInstanceHdl_t  fapiHdl,
                                        pfapiErrorInd_t     ind);
    void  (*fapi_subframe_ind)     (fapiInstanceHdl_t  fapiHdl,
                                        pfapiSubframeInd_t  ind);
    void  (*fapi_harq_ind)         (fapiInstanceHdl_t  fapiHdl,
                                        pfapiHarqInd_t      ind);
    void  (*fapi_crc_ind)          (fapiInstanceHdl_t  fapiHdl,
                                        pfapiCrcInd_t       ind);
    void  (*fapi_rx_ulsch_ind)     (fapiInstanceHdl_t  fapiHdl,
                                        pfapiRxUlschInd_t   ind);
    void  (*fapi_rx_cqi_ind)       (fapiInstanceHdl_t  fapiHdl,
                                        pfapiRxCqiInd_t     ind);
    void  (*fapi_rx_sr_ind)        (fapiInstanceHdl_t  fapiHdl,
                                        pfapiRxSrInd_t      ind);
    void  (*fapi_rach_ind)         (fapiInstanceHdl_t  fapiHdl,
                                        pfapiRachInd_t      ind);
    void  (*fapi_srs_ind)          (fapiInstanceHdl_t  fapiHdl,
                                        pfapiSrsInd_t       ind);
//------------------------------------------------------------------------------
// Vendor Specific Callbacks
//------------------------------------------------------------------------------
    void  (*fapi_rip_measurement)       (fapiInstanceHdl_t  fapiHdl,
                                            pfapiMeasReport_t   pMeasReport);
    void  (*fapi_start_phy_shutdown)    (fapiInstanceHdl_t  fapiHdl,
                                            void           *pMsgInd);
    void  (*fapi_shutdown_resp)         (fapiInstanceHdl_t  fapiHdl,
                                            void           *pMsgInd);
    void  (*fapi_start_cnf)             (fapiInstanceHdl_t  fapiHdl,
                                            void           *pMsgInd);
    void  (*fapi_ul_iq_samples)         (fapiInstanceHdl_t  fapiHdl,
                                            void           *pMsgInd);
    void  (*fapi_dl_iq_samples)         (fapiInstanceHdl_t  fapiHdl,
                                            void           *pMsgInd);
    void  (*fapi_ul_copy_results_ind)   (fapiInstanceHdl_t  fapiHdl,
                                            void           *pMsgInd);

    void  (*fapi_endof_phy2mac_processing)    (fapiInstanceHdl_t  fapiHdl,
                                            void           *pMsgInd);
} fapiCb_t, *pfapiCb_t;

//------------------------------------------------------------------------------

fapiStatus_t      fapi_init(pfapiInitConfig_t pinitConfig);
fapiStatus_t      fapi_destroy(void);
fapiInstanceHdl_t fapi_create(pfapiCb_t callbacks,
                      pfapiCreateConfig_t pCreateConfig);
fapiStatus_t      fapi_delete(fapiInstanceHdl_t fapiHdl);

//------------------------------------------------------------------------------
// Fapi P5 Messages
//------------------------------------------------------------------------------
fapiStatus_t    fapi_param_request(fapiInstanceHdl_t fapiHdl,
                      pfapiParamReq_t req);
fapiStatus_t    fapi_config_request(fapiInstanceHdl_t fapiHdl,
                      pfapiConfigReq_t req);
fapiStatus_t    fapi_start_request(fapiInstanceHdl_t fapiHdl,
                      pfapiStartReq_t req);
fapiStatus_t    fapi_stop_request(fapiInstanceHdl_t fapiHdl,
                      pfapiStopReq_t req);
//------------------------------------------------------------------------------
// Fapi P7 Messages
//------------------------------------------------------------------------------
fapiStatus_t    fapi_dl_config_request(fapiInstanceHdl_t fapiHdl,
                      pfapiDlConfigReq_t req);
fapiStatus_t    fapi_ul_config_request(fapiInstanceHdl_t fapiHdl,
                      pfapiUlConfigReq_t req);
fapiStatus_t    fapi_hi_dci0_request(fapiInstanceHdl_t fapiHdl,
                      pfapiHiDci0Req_t req);
fapiStatus_t    fapi_tx_request(fapiInstanceHdl_t fapiHdl, pfapiTxReq_t
                      req);
#endif
#endif //_FAPI_H_

