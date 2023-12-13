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

/* Defines APIs exchanged between du_app and cl module of MAC */
#ifndef __MACINT_H__
#define __MACINT_H__

#define NUM_NUMEROLOGY 5  /* Number of numerology */
#define MAX_SI_MESSAGE 32 /* As per 138 331 V15.3, RRC Multiplicity and type constraint definitions maxSI-Message = 32 */ 
#define NUM_SIB 32         /* As per 138 331 V15.3, RRC Multiplicity and type constraint definitions  maxSIB = 32 */ 
#define NUM_SSB		1	/* max value is 64 */
#define SSB_MASK_SIZE	1	/* SSB mask size is 32bit for sub6 */
#define SIB1_REPETITION_PERIOD   20
#define CORESET_0_INDEX      0
#define CORESET_1_INDEX      1
#define CORESET_2_INDEX      2
#define CORESET_3_INDEX      3
#define CORESET_4_INDEX      4
#define SEARCHSPACE_0_INDEX   0
#define SEARCHSPACE_1_INDEX   1
#define SEARCHSPACE_2_INDEX   2
#define SEARCHSPACE_3_INDEX   3
#define SEARCHSPACE_4_INDEX   4
#define SS_MONITORING_SLOT_SL1   0 /* all slots */
#define SS_MONITORING_SYMBOL     0x2000; /* symbol-0, set 14th bit */

/* Macro for Ue Context */
#define MAX_NUM_SR_CFG_PER_CELL_GRP 8   /* Max number of scheduling request config per cell group */
#define MAC_NUM_TAGS 4                  /* Max number of timing advance groups */
#define MAX_NUM_BWP  4                  /* Max number of BWP per serving cell */
#define MAX_NUM_CRSET  3                /* Max number of control resource set in add/modify/release list */
#define MAX_NUM_SEARCH_SPC  10          /* Max number of search space in add/modify/release list */
#define MONITORING_SYMB_WITHIN_SLOT_SIZE 2  /* i.e. 2 bytes because size of monitoring symbols within slot is 14 bits */
#define MAX_NUM_DL_ALLOC 16             /* Max number of pdsch time domain downlink allocation */
#define MAX_NUM_UL_ALLOC 16             /* Max number of pusch time domain uplink allocation */
#define MAX_NUM_SCELL  32

/* PUCCH Configuration Macro */
#define MAX_NUM_PUCCH_RESRC 128
#define MAX_NUM_PUCCH_RESRC_SET 4
#define MAX_NUM_PUCCH_PER_RESRC_SET 32
#define MAX_NUM_SPATIAL_RELATIONS 8
#define MAX_NUM_PUCCH_P0_PER_SET 8
#define MAX_NUM_PATH_LOSS_REF_RS 4
#define MAX_NUM_DL_DATA_TO_UL_ACK 15

/* Event IDs */
#define EVENT_MAC_CELL_CONFIG_REQ    200
#define EVENT_MAC_CELL_CONFIG_CFM    201
#define EVENT_MAC_CELL_START         202
#define EVENT_MAC_CELL_STOP          203
#define EVENT_MAC_CELL_UP_IND        204
#define EVENT_MAC_STOP_IND           205
#define EVENT_MAC_UL_CCCH_IND        206
#define EVENT_MAC_DL_CCCH_IND        207
#define EVENT_MAC_UE_CREATE_REQ      208
#define EVENT_MAC_UE_CREATE_RSP      209
#define EVENT_MAC_UE_RECONFIG_REQ    210
#define EVENT_MAC_UE_RECONFIG_RSP    211
#define EVENT_MAC_UE_DELETE_REQ      212
#define EVENT_MAC_UE_DELETE_RSP      213
#define EVENT_MAC_CELL_DELETE_REQ    214
#define EVENT_MAC_CELL_DELETE_RSP    215
#define EVENT_MAC_SLICE_CFG_REQ      216
#define EVENT_MAC_SLICE_CFG_RSP      217
#define EVENT_MAC_SLICE_RECFG_REQ    218
#define EVENT_MAC_SLICE_RECFG_RSP    219
#define EVENT_MAC_SLOT_IND           220
#define EVENT_MAC_RACH_RESOURCE_REQ  221
#define EVENT_MAC_RACH_RESOURCE_RSP  222
#define EVENT_MAC_RACH_RESOURCE_REL  223
#define EVENT_MAC_DL_PCCH_IND        224
#define EVENT_MAC_UE_RESET_REQ       225
#define EVENT_MAC_UE_RESET_RSP       226
#define EVENT_MAC_UE_SYNC_STATUS_IND 227
#define EVENT_MAC_DL_BROADCAST_REQ   228
#define EVENT_MAC_STATISTICS_REQ     229
#define EVENT_MAC_STATISTICS_RSP     230
#define EVENT_MAC_STATISTICS_IND     231
#define EVENT_MAC_STATS_DELETE_REQ   232
#define EVENT_MAC_STATS_DELETE_RSP   233
#define EVENT_MAC_STATISTICS_MODIFY_REQ     234
#define EVENT_MAC_STATISTICS_MODIFY_RSP     235

#define BSR_PERIODIC_TIMER_SF_10 10
#define BSR_RETX_TIMER_SF_320 320
#define BSR_SR_DELAY_TMR_2560 2560

#define PAGING_SCHED_DELTA  4
#define MAX_PLMN 2

/********************* Global Variable ********************/
uint64_t ueBitMapPerCell[MAX_NUM_CELL]; /* Bit Map to store used/free UE-IDX per Cell */

/********************* Interface structure definition ********************/
typedef enum
{
   SIB_TYPE2,
   SIB_TYPE3,
   SIB_TYPE4,
   SIB_TYPE5,
   SIB_TYPE6,
   SIB_TYPE7,
   SIB_TYPE8,
   SIB_TYPE9,
   SPARE
}SibType;

typedef enum
{
   SSB_PER_RACH_OCCASION_ONE_EIGHTH,
   SSB_PER_RACH_OCCASION_ONE_FOURTH,
   SSB_PER_RACH_OCCASION_ONE_HALF,
   SSB_PER_RACH_OCCASION_ONE,
   SSB_PER_RACH_OCCASION_TWO,
   SSB_PER_RACH_OCCASION_FOUR,
   SSB_PER_RACH_OCCASION_EIGHT,
   SSB_PER_RACH_OCCASION_SIXTEEN
}SsbPerRachOccasion;

typedef enum
{
   BROADCASTING,
   NOTBROADCASTING,
}SiBroadcastStatus;

typedef enum
{
   RF8,
   RF16,
   RF32,
   RF64,
   RF128,
   RF256,
   RF512
}SiPeriodicity;

typedef enum
{
   S5,
   S10,
   S20, 
   S40, 
   S80, 
   S160, 
   S320, 
   S640, 
   S1280
}SiWindowLength;

typedef enum
{
   SI_REQ_PERIOD_1,
   SI_REQ_PERIOD_2,
   SI_REQ_PERIOD_4,
   SI_REQ_PERIOD_6,
   SI_REQ_PERIOD_8,
   SI_REQ_PERIOD_10,
   SI_REQ_PERIOD_12,
   SI_REQ_PERIOD_16
}SiRequestPeriod;

typedef enum
{
   MAC_DU_APP_RSP_NOK,
   MAC_DU_APP_RSP_OK
}MacRsp;

typedef enum
{
   IN_SYNC,
   OUT_OF_SYNC,
   OUT_OF_SUNC_MAX_RETRIES
}SyncStatus;

typedef enum
{
   DUP_MODE_FDD,
   DUP_MODE_TDD
}DuplexMode;

typedef enum
{
   MAC_GEN_FULL_PBCH_PAYLD,   /* MAC generated the full PBCH Payload */
   PHY_GEN_TIMING_PBCH_BIT,   /* PHY generates the timing PBCH bits */
   PHY_GEN_FULL_PBCH_PAYLD    /* PHY generates full PBCH payload */
}BchPduOpt;

typedef enum
{
   LONG_SEQUENCE,
   SHORT_SEQUENCE
}PrachSeqLen;

typedef enum
{
   UNRESTRICTED,
   RESTRICTED_SET_TYPE_A,
   RESTRICTED_SET_TYPE_B
}RstSetCfg;

typedef enum
{
   DONT_REPORT_RSSI,
   RSSI_UNIT_DBM,
   RSSI_UNIT_DBFS
}RSSIMeasUnit;

typedef enum
{
   BETA_PSS_0DB,
   BETA_PSS_3DB
}BetaPss;

typedef enum 
{
   SSB_PRDCTY_MS_5,
   SSB_PRDCTY_MS_10,
   SSB_PRDCTY_MS_20,
   SSB_PRDCTY_MS_40,
   SSB_PRDCTY_MS_80,
   SSB_PRDCTY_MS_160
}SSBPeriod;

typedef enum
{
   RRC_REJECT,
   RRC_SETUP
}DlCcchMsgType;

/* Enums for Ue Create Request */

/* SR PROHIBIT TIMER */
typedef enum
{
   SR_PROHIBIT_MS_1,
   SR_PROHIBIT_MS_2,
   SR_PROHIBIT_MS_4,
   SR_PROHIBIT_MS_8,
   SR_PROHIBIT_MS_16,
   SR_PROHIBIT_MS_32,
   SR_PROHIBIT_MS_64,
   SR_PROHIBIT_MS_128
}SrProhibitTimer;

typedef enum
{
   SR_TRANS_MAX_N_4,
   SR_TRANS_MAX_N_8,
   SR_TRANS_MAX_N_16,
   SR_TRANS_MAX_N_32,
   SR_TRANS_MAX_N_64,
   SR_TRANS_MAX_spare3,
   SR_TRANS_MAX_spare2,
   SR_TRANS_MAX_spare1
}SrTransMax;

typedef enum
{
   TIME_ALIGNMENT_TIMER_MS_500,
   TIME_ALIGNMENT_TIMER_MS_750,
   TIME_ALIGNMENT_TIMER_MS_1280,
   TIME_ALIGNMENT_TIMER_MS_1920,
   TIME_ALIGNMENT_TIMER_MS_2560,
   TIME_ALIGNMENT_TIMER_MS_5120,
   TIME_ALIGNMENT_TIMER_MS_10240,
   TIME_ALIGNMENT_TIMER_INFINITY
}TimeAlignmentTimer;

typedef enum
{
   PHR_PERIODIC_TIMER_SF_10,
   PHR_PERIODIC_TIMER_SF_20,
   PHR_PERIODIC_TIMER_SF_50,
   PHR_PERIODIC_TIMER_SF_100,
   PHR_PERIODIC_TIMER_SF_200,
   PHR_PERIODIC_TIMER_SF_500,
   PHR_PERIODIC_TIMER_SF_1000,
   PHR_PERIODIC_TIMER_INFINITY
}PhrPeriodicTimer;

typedef enum
{
   PHR_PROHIBIT_TIMER_SF_0,
   PHR_PROHIBIT_TIMER_SF_10,
   PHR_PROHIBIT_TIMER_SF_20,
   PHR_PROHIBIT_TIMER_SF_50,
   PHR_PROHIBIT_TIMER_SF_100,
   PHR_PROHIBIT_TIMER_SF_200,
   PHR_PROHIBIT_TIMER_SF_500,
   PHR_PROHIBIT_TIMER_SF_1000
}PhrProhibitTimer;

typedef enum
{
   PHR_TX_PWR_FACTOR_CHANGE_DB_1,
   PHR_TX_PWR_FACTOR_CHANGE_DB_3,
   PHR_TX_PWR_FACTOR_CHANGE_DB_6,
   PHR_TX_PWR_FACTOR_CHANGE_INFINITY
}PhrTxPwrFactorChange;

typedef enum
{
   PHR_MODE_OTHER_CG_REAL,
   PHR_MODE_OTHER_CG_VIRTUAL
}PhrModeOtherCG;

typedef enum
{
   PDSCH_HARQ_ACK_CODEBOOK_SEMISTATIC,
   PDSCH_HARQ_ACK_CODEBOOK_DYNAMIC
}PdschHarqAckCodebook;

typedef enum
{
   NUM_HARQ_PROC_FOR_PDSCH_N_2,
   NUM_HARQ_PROC_FOR_PDSCH_N_4,
   NUM_HARQ_PROC_FOR_PDSCH_N_6,
   NUM_HARQ_PROC_FOR_PDSCH_N_10,
   NUM_HARQ_PROC_FOR_PDSCH_N_16
}NumHarqProcForPdsch;

typedef enum
{
   MAX_CODE_BLOCK_GROUP_PER_TB_N_2,
   MAX_CODE_BLOCK_GROUP_PER_TB_N_4,
   MAX_CODE_BLOCK_GROUP_PER_TB_N_6,
   MAX_CODE_BLOCK_GROUP_PER_TB_N_8
}MaxCodeBlkGrpPerTB;

typedef enum
{
   PDSCH_X_OVERHEAD_6,
   PDSCH_X_OVERHEAD_12,
   PDSCH_X_OVERHEAD_18
}PdschXOverhead;

typedef enum
{
   TRANSFORM_PRECODER_ENABLED,
   TRANSFORM_PRECODER_DISABLED
}TransformPrecoder;

typedef enum 
{
   CCE_REG_MAPPINGTYPE_PR_INTERLEAVED = 1,
   CCE_REG_MAPPINGTYPE_PR_NONINTERLEAVED
}REGMappingType;

typedef enum
{
   SLOTPERIODICITY_PR_SL1 = 1,
   SLOTPERIODICITY_PR_SL2,
   SLOTPERIODICITY_PR_SL4,
   SLOTPERIODICITY_PR_SL5,
   SLOTPERIODICITY_PR_SL8,
   SLOTPERIODICITY_PR_SL10,
   SLOTPERIODICITY_PR_SL16,
   SLOTPERIODICITY_PR_SL20,
   SLOTPERIODICITY_PR_SL40,
   SLOTPERIODICITY_PR_SL80,
   SLOTPERIODICITY_PR_SL160,
   SLOTPERIODICITY_PR_SL320,
   SLOTPERIODICITY_PR_SL640,
   SLOTPERIODICITY_PR_SL1280,
   SLOTPERIODICITY_PR_SL2560
}MSlotPeriodicity;

typedef enum
{
   SAMEASREG_BUNDLE,
   ALL_CONTIGUOUS_RBS
}PrecoderGranul;

typedef enum
{
   SEARCHSPACETYPE_PR_COMMON = 1,
   SEARCHSPACETYPE_PR_UE_SPECIFIC
}SearchSpaceType;

typedef enum
{
   QOS_NON_DYNAMIC = 1,
   QOS_DYNAMIC
}QosType;

typedef enum
{
   AGGREGATIONLEVEL_N0	= 0,  
   AGGREGATIONLEVEL_N1	= 1,
   AGGREGATIONLEVEL_N2	= 2,
   AGGREGATIONLEVEL_N3	= 3,
   AGGREGATIONLEVEL_N4	= 4,
   AGGREGATIONLEVEL_N5	= 5,
   AGGREGATIONLEVEL_N6	= 6,
   AGGREGATIONLEVEL_N8	= 7
}AggrLevel;

typedef enum
{
   ADDITIONALPOSITION_POS0,
   ADDITIONALPOSITION_POS1,
   ADDITIONALPOSITION_POS3
}AddPosType;

typedef enum
{
   MAPPING_TYPEA,
   MAPPING_TYPEB
}CommonMappingType;

typedef enum
{
   RESOURCEALLOCATION_TYPE0,
   RESOURCEALLOCATION_TYPE1,
   RESOURCEALLOCATION_DYNAMICSWITCH
}ResAllocType;

typedef enum
{
   RBG_SIZE_CONFIG1,
   RBG_SIZE_CONFIG2
}RBGSize;

typedef enum
{
   CODEWORDS_SCHED_BY_DCI_N1,
   CODEWORDS_SCHED_BY_DCI_N2
}CodeWordsSchedDci;

typedef enum
{
   TYPE_STATIC_BUNDLING = 1,
   TYPE_DYNAMIC_BUNDLING
}BundlingType;

typedef enum
{
   SET2_SIZE_N4,
   SET2_SIZE_WIDEBAND
}BundlingSizeSet2;

typedef enum
{
   SET1_SIZE_N4,
   SET1_SIZE_WIDEBAND,
   SET1_SIZE_N2_WIDEBAND,
   SET1_SIZE_N4_WIDEBAND
}BundlingSizeSet1;

typedef enum
{
   LC_PRIORITY_1 = 1,
   LC_PRIORITY_2,
   LC_PRIORITY_3
}LcPriority;

typedef enum
{
  PBR_KBPS_0,
  PBR_KBPS_8,
  PBR_KBPS_16,
  PBR_KBPS_32,
  PBR_KBPS_64,
  PBR_KBPS_128,
  PBR_KBPS_256,
  PBR_KBPS_512,
  PBR_KBPS_1024,
  PBR_KBPS_2048,
  PBR_KBPS_4096,
  PBR_KBPS_8192,
  PBR_KBPS_16384,
  PBR_KBPS_32768,
  PBR_KBPS_65536,
  PBR_KBPS_INFINITY
}PBitRate;

typedef enum
{
   BSD_MS_5,
   BSD_MS_10,
   BSD_MS_20,
   BSD_MS_50,
   BSD_MS_100,
   BSD_MS_150,
   BSD_MS_300,
   BSD_MS_500,
   BSD_MS_1000,
   BSD_SPARE_7,
   BSD_SPARE_6,
   BSD_SPARE_5,
   BSD_SPARE_4,
   BSD_SPARE_3,
   BSD_SPARE_2,
   BSD_SPARE_1

}BucketSizeDur;

typedef enum
{
   RADIO_NW_LAYER_FAIL,
   TRANSPORT_LAYER_FAIL,
   PROTOCOL_FAIL,
   MISCELLANEOUS
}CauseGrp;

typedef enum
{
   UNSPECIFIED_RADIO_NW_CAUSE,
   RL_FAIL_RLC,
   UNKNOWN_GNB_CU_UE_F1AP_ID,
   ALREADY_ALLOCATED_GNB_CU_UE_F1AP_ID,
   UNKNOWN_GNB_DU_UE_F1AP_ID,
   ALREADY_ALLOCATED_GNB_DU_UE_F1AP_ID,
   UNKNOWN_UE_F1AP_ID_PAIR,
   INCONSISTENT_UE_F1AP_ID_PAIR,
   INTERACTION_WITH_OTHER_PROCEDURE,
   UNSUPPORTED_QCI,
   ACTION_REQUIRED_FOR_RADIO_REASONS,
   RADIO_RESOURCES_UNAVAILABLE,
   CANCELLED_PROCEDURE,
   RELEASE_NORMAL,
   CELL_UNAVAILABLE,
   OTHER_RL_FAILURE,
   UE_REJECTION,
   RESOURCES_UNAVAILABLE_FOR_SLICE
}RadioNwLyrCause;

typedef enum
{
   UNSPECIFIED_TRANSPORT_LAYER_CAUSE,
   TRANSPORT_RESOURCE_UNAVAILABLE
}TransLyrCause;

typedef enum
{
   TRANSFER_SYNTAX_ERROR,
   ABSTRACT_SYNTAX_ERROR_REJECT,
   ABSTRACT_SYNTAX_ERROR_IGNORE_AND_REJECT,
   INCOMPATIBLE_MESSAGE_FOR_RECEIVER_STATE,
   SEMANTIC_ERR,
   ABSTRAXCT_SYNTAX_ERROR_FALSELY_CONSTRUCTED_MSG,
   UNSPECIFIED_PROTOCOL_CAUSE
}ProtCause;

typedef enum
{
   CONTROL_PROCESSING_OVERLOAD,
   NOT_ENOUGH_USER,
   PLANE_PROCESSING_RESOURCES,
   HARDWARE_FAIL,
   INTERVENTION_BY_O_AND_M,
   UNSPECIFIED_MISC_CAUSE
}MiscFailCause;

typedef enum
{
   MCS_TABLE_QAM64,
   MCS_TABLE_QAM256,
   MCS_TABLE_QAM64_LOW_SE
}McsTable;

typedef enum
{
   RSRC_PRB,
   RSRC_DRB,
   RSRC_RRC_CONNECTED_USERS
}ResourceType;

typedef enum
{
   TRANSMISSION_ALLOWED,
   STOP_TRANSMISSION,
   RESTART_TRANSMISSION
}DataTransmissionAction;

/* Performance measurements from 3GPP TS 28.552 Release 15 */
typedef enum
{
   MAC_DL_TOTAL_PRB_USAGE,
   MAC_UL_TOTAL_PRB_USAGE
}MacMeasurementType;

typedef struct failureCause 
{
   CauseGrp   type;
   union
   {
      RadioNwLyrCause   radioNwResult;
      TransLyrCause     transportResult;
      ProtCause         protcolResult;
      MiscFailCause     miscResult;
   }u;
}FailureCause;

typedef struct carrierCfg
{
   uint32_t   dlBw;                   /* DL bandwidth */
   uint32_t   arfcnDL;                 /* Absolute frequency Number of DL */
   uint32_t   ulBw;                   /* UL bandwidth */
   uint32_t   arfcnUL;                 /* Absolute frequency Number of UL */
   uint16_t   numTxAnt;               /* Number of Tx antennas */
   uint16_t   numRxAnt;               /* Number of Rx antennas */
}CarrierCfg;

typedef enum
{
   OP_DISABLED,     
   OP_ENABLED,     
}MacOpState;

typedef enum
{
   ADMIN_LOCKED,     
   ADMIN_UNLOCKED,  
   ADMIN_SHUTTING_DOWN, 
}MacAdminState;

typedef enum
{
   CELL_IDLE,
   CELL_INACTIVE,
   CELL_ACTIVE,
}MacCellState;

/*Spec Ref: 38.331: RadioLinkMonitoringConfig*/
typedef enum
{
   BeamFailure,
   Rlf,
   Both
}PurposeOfFailureDet;

typedef struct plmnInfoList
{
   Plmn           plmn;
   SupportedSliceList suppSliceList;
}PlmnInfoList;

typedef struct schPageCfg
{
  uint8_t  numPO;                    /*Derived from Ns*/
  bool     poPresent;                /*Whether FirstPDCCH-MonitoringPO is present or not*/
  uint16_t pagingOcc[MAX_PO_PER_PF]; /*FirstPDCCH-Monitoring Paging Occasion*/
}SchPageCfg;

typedef struct pdcchConfigSib1
{
   uint8_t coresetZeroIndex;     /* derived from 4 LSB of pdcchSib1 present in MIB */
   uint8_t searchSpaceZeroIndex; /* derived from 4 MSB of pdcchSib1 present in MIB */
}PdcchConfigSib1;

typedef struct sib1CellCfg
{
   PdcchConfigSib1  pdcchCfgSib1;  /*Freq pos where UE may find SS/PBCH block with SIB1*/
   uint8_t          *sib1Pdu;
   uint16_t         sib1PduLen;
   SchPageCfg       pagingCfg;
} Sib1CellCfg; 

typedef struct bwpParams
{
   uint16_t firstPrb;
   uint16_t numPrb;
   uint8_t  scs;
   uint8_t  cyclicPrefix;
}BwpParams;

typedef struct candidatesInfo
{
   uint8_t aggLevel1;
   uint8_t aggLevel2;
   uint8_t aggLevel4;
   uint8_t aggLevel8;
   uint8_t aggLevel16;
}CandidatesInfo;

typedef struct searchSpaceCfg
{
   uint8_t searchSpaceId;
   uint8_t coresetId;
   uint16_t monitoringSlot;
   uint16_t duration;
   uint16_t monitoringSymbol;
   CandidatesInfo candidate;
}SearchSpaceCfg;

typedef struct pdcchConfigCommon
{
   /* only one search space is configured during SIB1 */
   SearchSpaceCfg commonSearchSpace;
   uint8_t raSearchSpaceId;
}PdcchConfigCommon;

typedef struct pdschCfgCommTimeDomRsrcAlloc
{
   uint8_t k0;
   uint8_t mappingType;
   uint8_t startSymbol;
   uint8_t lengthSymbol;
}PdschCfgCommTimeDomRsrcAlloc;

typedef struct pdschConfigCommon
{
   uint8_t  numTimeDomAlloc;
   /* PDSCH time domain DL resource allocation list */
   PdschCfgCommTimeDomRsrcAlloc   timeDomRsrcAllocList[MAX_NUM_DL_ALLOC]; 
}PdschConfigCommon;

typedef struct pucchConfigCommon
{
   uint8_t pucchResourceCommon;
   uint8_t pucchGroupHopping;
}PucchConfigCommon;

/* PUSCH Time Domain Resource Allocation */
typedef struct puschTimeDomRsrcAlloc
{
   uint8_t             k2;
   CommonMappingType   mappingType;
   uint8_t             startSymbol;
   uint8_t             symbolLength;
   uint8_t             startSymbolAndLength;
}PuschTimeDomRsrcAlloc;

typedef struct puschConfigCommon
{
   /* PUSCH-TimeDomainResourceAllocation info */
   uint8_t                 numTimeDomRsrcAlloc;
   PuschTimeDomRsrcAlloc   timeDomRsrcAllocList[MAX_NUM_UL_ALLOC]; /* PUSCH time domain UL resource allocation list */
}PuschConfigCommon;

typedef struct bwpDlConfig
{
   BwpParams         bwp;
   PdcchConfigCommon pdcchCommon;
   PdschConfigCommon pdschCommon;
}BwpDlConfig;

typedef struct bwpUlConfig
{
   BwpParams         bwp;
   // rach config common sent in PrachCfg
   PucchConfigCommon pucchCommon;
   PuschConfigCommon puschCommon;
}BwpUlConfig;


typedef struct cellCfg
{
   MacOpState      opState;
   MacAdminState   adminState;
   MacCellState    cellState;
   PlmnInfoList plmnInfoList[MAX_PLMN];   /* Consits of PlmnId and Snssai list */
   uint32_t     phyCellId;                /* Physical cell id */
   uint32_t     tac;
   uint32_t     ssbFreq;
   uint16_t     subCarrSpacing;
   DuplexMode   dupType;          /* Duplex type: TDD/FDD */
   Sib1CellCfg  sib1Cfg;          /* SIB1 config */
   BwpDlConfig  initialDlBwp;     /* Initial DL BWP */
   BwpUlConfig  initialUlBwp;     /* Initial UL BWP */
}CellCfg;

typedef struct ssbCfg
{
   uint32_t    ssbPbchPwr;       /* SSB block power */
   uint8_t     scsCmn;           /* subcarrier spacing for common */
   uint16_t    ssbOffsetPointA;  /* SSB subcarrier offset from point A */
   SSBPeriod   ssbPeriod;        /* SSB Periodicity in msec */
   uint8_t     ssbScOffset;       /* Subcarrier Offset */
   uint32_t    ssbMask[SSB_MASK_SIZE];      /* Bitmap for actually transmitted SSB. */
   uint8_t     beamId[NUM_SSB];
   BetaPss     betaPss;
   BchPduOpt   bchPayloadFlag;   /* Options for generation of payload */
   uint8_t     mibPdu[3];           /* MIB payload */
   uint8_t     dmrsTypeAPos;     /* DMRS Type A position */
}SsbCfg;

typedef struct fdmInfo
{
   uint16_t   rootSeqIdx;        /* Root sequence index */
   uint8_t    numRootSeq;        /* Number of root sequences required for FD */
   uint16_t   k1;                /* Frequency Offset for each FD */
   uint8_t    zeroCorrZoneCfg;   /* Zero correlation zone cofig */
}PrachFdmInfo;

typedef struct prachCfg
{
   PrachSeqLen   prachSeqLen;         /* RACH Sequence length: Long/short */
   uint8_t       prachSubcSpacing;    /* Subcarrier spacing of RACH */
   uint8_t       msg1Fdm;             /* Number of RACH frequency domain occasions/ PRACH FDM (1,2,4,8) */
   uint8_t       prachCfgIdx;         /* PRACH Cfg Index */
   PrachFdmInfo  fdm[8];              /* FDM info */
   RstSetCfg     prachRstSetCfg;      /* PRACH restricted set config */
   uint8_t       ssbPerRach;          /* SSB per RACH occassion */
   uint8_t       totalNumRaPreamble;  /* Total number of RA preambles */
   uint8_t       numCbPreamblePerSsb; /* Number of CB preamble per SSB */
   uint16_t      msg1FreqStart;       /* Msg1-FrequencyStart */
   uint8_t       raContResTmr;        /* RA Contention Resoultion Timer */
   uint8_t       rsrpThreshSsb;       /* RSRP Threshold SSB */
   uint8_t       raRspWindow;         /* RA Response Window */
}PrachCfg;

#ifdef NR_DRX
/* The following list of structures is taken from the DRX-Config section of specification 33.331. */
typedef struct drxOnDurationTimer
{
   bool  onDurationTimerValInMs;
   union
   {
      uint8_t  subMilliSeconds;
      uint16_t milliSeconds;
   }onDurationtimerValue;
}DrxOnDurationTimer;

typedef struct drxLongCycleStartOffset
{
   uint16_t drxLongCycleStartOffsetChoice;
   uint16_t drxLongCycleStartOffsetVal;
}DrxLongCycleStartOffset;

typedef struct shortDrx
{
   uint16_t   drxShortCycle;
   uint8_t    drxShortCycleTimer;
}ShortDrx;

typedef struct drxCfg
{
   DrxOnDurationTimer       drxOnDurationTimer;   
   uint16_t                 drxInactivityTimer;
   uint8_t                  drxHarqRttTimerDl;
   uint8_t                  drxHarqRttTimerUl;
   uint16_t                 drxRetransmissionTimerDl;
   uint16_t                 drxRetransmissionTimerUl;
   DrxLongCycleStartOffset  drxLongCycleStartOffset;
   bool                     shortDrxPres;
   ShortDrx                 shortDrx;
   uint8_t                  drxSlotOffset;
}DrxCfg;
#endif

typedef struct precodingConf
{
   uint16_t numLayers;
   uint16_t numAntPorts;
}PrecodingConf;

typedef struct beamformingConf
{    
   uint16_t numOfBeams;
   uint16_t numTxRUs;
   uint16_t beamIdx;
   uint16_t beamType;
   uint32_t beamAzimuth;
   uint32_t beamTilt;
   uint32_t beamHorizWidth;
   uint32_t beamVertWidth;
   uint32_t coverageShape;
   uint32_t digitalTilt;
   uint32_t digitalAzimuth;    
}BeamformingConf;

typedef struct csiRsCfg
{
   uint8_t   *csiFreqDomainAlloc;
   uint8_t    csiNrofPorts;
   uint8_t    csirsfirstOFDMSymbolInTimeDomain;
   uint8_t    csirsfirstOFDMSymbolInTimeDomain2;
   uint8_t    csirscdmType;
   uint8_t    csirsdensity;
   uint8_t    csirsdensitydot5;
   uint8_t    powerControlOffset;
   uint8_t    powerControlOffsetSS;
   uint16_t   periodicityAndOffset;
}CsiRsCfg;

typedef struct macCellCfg
{
   uint16_t         cellId;           /* Cell Id */
   CarrierCfg       carrCfg;          /* Carrier configuration */
   CellCfg          cellCfg;          /* Cell Configuration*/
   SsbCfg           ssbCfg;           /* SSB configuration */          
   CsiRsCfg         csiRsCfg;         /*Reference: 38.331 CSI-MeasConfig*/
   PrachCfg         prachCfg;         /* PRACH Configuration */
#ifdef NR_TDD
   TDDCfg           tddCfg;           /* TDD periodicity and slot configuration */
#endif
   PrecodingConf    precodingConf;
   BeamformingConf  beamCfg;
}MacCellCfg;

typedef struct macCellCfgCfm
{
   uint16_t       cellId;
   uint8_t        rsp; 
}MacCellCfgCfm;

typedef struct ulCcchInd
{
   uint16_t cellId;
   uint16_t crnti;
   uint16_t ulCcchMsgLen;
   uint8_t  *ulCcchMsg;
}UlCcchIndInfo;

typedef struct dlCcchInd
{
   uint16_t      cellId;
   uint16_t      crnti;
   DlCcchMsgType msgType;
   uint16_t      dlCcchMsgLen;
   uint8_t       *dlCcchMsg;
}DlCcchIndInfo;

typedef struct bsrTmrCfg
{
   uint16_t     periodicTimer;
   uint16_t     retxTimer;
   uint16_t     srDelayTimer;
}BsrTmrCfg;


/* Info of Scheduling Request to Add/Modify */
typedef struct schedReqInfo
{
   uint8_t         schedReqId;
   SrProhibitTimer srProhibitTmr;
   SrTransMax      srTransMax;
}SchedReqInfo;

/* Scheduling Request Configuration */
typedef struct schedReqCfg
{
   uint8_t       addModListCount;
   SchedReqInfo  addModList[MAX_NUM_SR_CFG_PER_CELL_GRP];   /* List of Scheduling req to be added/modified */
   uint8_t       relListCount;
   uint8_t       relList[MAX_NUM_SR_CFG_PER_CELL_GRP];      /* list of scheduling request Id to be deleted */
}SchedReqCfg;

typedef struct tagInfo
{
   uint8_t tagId;
   TimeAlignmentTimer timeAlignTimer;
}TagInfo;

typedef struct tagCfg
{
   uint8_t      addModListCount;
   TagInfo      addModList[MAC_NUM_TAGS];  /* List of Tag to Add/Modify */
   uint8_t      relListCount;
   uint8_t      relList[MAC_NUM_TAGS];     /* list of Tag Id to release */
}TagCfg;

typedef struct phrCfg
{
   PhrPeriodicTimer periodicTimer;
   PhrProhibitTimer prohibitTimer; 
   PhrTxPwrFactorChange txPowerFactor;
   bool     multiplePHR;       
   bool     dummy;
   bool     phrType2OtherCell;
   PhrModeOtherCG phrOtherCG;
}PhrCfg;

typedef struct macCellGrpCfg
{
   SchedReqCfg schReqCfg;
   TagCfg      tagCfg;
   BsrTmrCfg   bsrTmrCfg;
   bool        phrCfgSetupPres;   /* true/false: phrCfgSetup/phrCfgRelease */
   PhrCfg      phrCfg;
#ifdef NR_DRX
   bool        drxCfgPresent;
   DrxCfg      drxCfg;
#endif
}MacCellGrpCfg;

typedef struct phyCellGrpCfg
{
   PdschHarqAckCodebook  pdschHarqAckCodebook;
   uint8_t    pNrFr1;
}PhyCellGrpCfg;

/* Control resource set info */
typedef struct controlRsrcSet
{
   uint8_t     cRSetId;                /* Control resource set id */
   uint8_t     freqDomainRsrc[FREQ_DOM_RSRC_SIZE];  /* Frequency domain resource */
   uint8_t     duration;
   REGMappingType cceRegMappingType;
   PrecoderGranul precoderGranularity;
   uint16_t    dmrsScramblingId;
}ControlRsrcSet;

typedef struct mSlotPeriodAndOffset
{
   MSlotPeriodicity  mSlotPeriodicity;
   uint16_t          mSlotOffset;
}MSlotPeriodAndOffset;

/* Search Space info */
typedef struct searchSpace
{
   uint8_t     searchSpaceId;
   uint8_t     cRSetId;
   MSlotPeriodAndOffset  mSlotPeriodicityAndOffset;
   uint8_t     mSymbolsWithinSlot[MONITORING_SYMB_WITHIN_SLOT_SIZE];
   AggrLevel   numCandidatesAggLevel1;      /* Number of candidates for aggregation level 1 */
   AggrLevel   numCandidatesAggLevel2;      /* Number of candidates for aggregation level 2 */
   AggrLevel   numCandidatesAggLevel4;      /* Number of candidates for aggregation level 4 */
   AggrLevel   numCandidatesAggLevel8;      /* Number of candidates for aggregation level 8 */
   AggrLevel   numCandidatesAggLevel16;     /* Number of candidates for aggregation level 16 */
   SearchSpaceType searchSpaceType;
   uint8_t     ueSpecificDciFormat;
}SearchSpace;

/* PDCCH cofniguration */
typedef struct pdcchConfig
{
   uint8_t           numCRsetToAddMod;
   ControlRsrcSet    cRSetToAddModList[MAX_NUM_CRSET];           /* List of control resource set to add/modify */
   uint8_t           numCRsetToRel;
   uint8_t           cRSetToRelList[MAX_NUM_CRSET];              /* List of control resource set to release */
   uint8_t           numSearchSpcToAddMod;
   SearchSpace       searchSpcToAddModList[MAX_NUM_SEARCH_SPC];  /* List of search space to add/modify */
   uint8_t           numSearchSpcToRel;
   uint8_t           searchSpcToRelList[MAX_NUM_SEARCH_SPC];     /* List of search space to release */
}PdcchConfig;

/* PDSCH time domain resource allocation */
typedef struct pdschTimeDomRsrcAlloc
{
   uint8_t           *k0;
   CommonMappingType mappingType;
   uint8_t           startSymbol;
   uint8_t           symbolLength;
   uint8_t           startSymbolAndLength;
}PdschTimeDomRsrcAlloc;


typedef struct pdschBundling
{
   struct staticBundling
   {
     BundlingSizeSet2 size;
   }StaticBundling;
   struct dynamicBundling
   {
     BundlingSizeSet1 sizeSet1;
     BundlingSizeSet2 sizeSet2;
   }DynamicBundling;
}PdschBundling;

/* DMRS downlink configuration */
typedef struct dmrsDlCfg
{
   AddPosType  addPos;       /* DMRS additional position */
}DmrsDlCfg;

/* PDSCH Configuration */
typedef struct pdschConfig
{
   DmrsDlCfg               dmrsDlCfgForPdschMapTypeA;
   ResAllocType            resourceAllocType;
   uint8_t                 numTimeDomRsrcAlloc;
   PdschTimeDomRsrcAlloc   timeDomRsrcAllociList[MAX_NUM_DL_ALLOC]; /* PDSCH time domain DL resource allocation list */
   RBGSize                 rbgSize;
   CodeWordsSchedDci       numCodeWordsSchByDci;                    /* Number of code words scheduled by DCI */
   BundlingType            bundlingType;
   PdschBundling           bundlingInfo;
}PdschConfig;

/* Initial Downlink BWP */
typedef struct initialDlBwp
{
   bool          pdcchPresent;
   PdcchConfig   pdcchCfg;
   bool          pdschPresent;
   PdschConfig   pdschCfg;
}InitialDlBwp;

/*Spec 38.331 "SPS-Config'*/
typedef struct spsConfig
{
   uint16_t     periodicity;
   uint8_t      numOfHqProcess;
   uint8_t      n1PucchAN;
   McsTable     mcsTable;
}SpsConfig;

typedef uint8_t RadioLinkMonitoringRsId;

typedef struct radioLinkMonRS
{
   RadioLinkMonitoringRsId radioLinkMonitoringRsId;
   PurposeOfFailureDet      purpose; 
   union
   {
      uint8_t ssbIndx;
      uint8_t nzpCsiRsResId;
   }detectionRes;
}RadioLinkMonRS;

typedef struct radioLinkConfig
{
   RadioLinkMonRS             failurDetResAddModList[1];
   RadioLinkMonitoringRsId   failurDetResRelList[1];
   uint8_t                    beamFailureInstanceMaxCount;
   uint8_t                    beamFailureDetectionTimer;
}RadioLinkConfig;

/* Spec 38.331, 'BWP-DownlinkDedicated'*/
typedef struct bwpDlCfgDed
{
  PdcchConfig     pdcchCfgDed;
  PdschConfig     pdschCfgDed;
  SpsConfig       spsCfgDed;
  RadioLinkConfig radioLnkMonCfgDed;
}BwpDlCfgDed;

/* Downlink BWP information */
typedef struct dlBwpInfo
{
   uint8_t          bwpId;
   BwpDlConfig      bwpCommon;
   BwpDlCfgDed      bwpDedicated;
}DlBwpInfo;

/* PDCCH Serving Cell configuration */
typedef struct pdschServCellCfg
{
   uint8_t              *maxMimoLayers;           
   NumHarqProcForPdsch  numHarqProcForPdsch;
   MaxCodeBlkGrpPerTB   *maxCodeBlkGrpPerTb;
   bool                 *codeBlkGrpFlushInd;
   PdschXOverhead       *xOverhead;
}PdschServCellCfg;

/* PUCCH Configuration */

typedef struct pucchResrcSetInfo
{
   uint8_t resrcSetId;
   uint8_t resrcListCount;
   uint8_t resrcList[MAX_NUM_PUCCH_PER_RESRC_SET];
   uint8_t maxPayLoadSize;
}PucchResrcSetInfo;

typedef struct pucchResrcSetCfg
{
   uint8_t resrcSetToAddModListCount;
   PucchResrcSetInfo resrcSetToAddModList[MAX_NUM_PUCCH_RESRC_SET];
   uint8_t resrcSetToRelListCount;
   uint8_t resrcSetToRelList[MAX_NUM_PUCCH_RESRC];
}PucchResrcSetCfg;

typedef struct pucchFormat0
{
   uint8_t initialCyclicShift;
   uint8_t numSymbols;
   uint8_t startSymbolIdx;
}PucchFormat0;

typedef struct pucchFormat1
{
   uint8_t initialCyclicShift;
   uint8_t numSymbols;
   uint8_t startSymbolIdx;
   uint8_t timeDomOCC;
}PucchFormat1;

typedef struct pucchFormat2_3
{
   uint8_t numPrbs;
   uint8_t numSymbols;
   uint8_t startSymbolIdx;
}PucchFormat2_3;

typedef struct pucchFormat4
{
   uint8_t numSymbols;
   uint8_t occLen;
   uint8_t occIdx;
   uint8_t startSymbolIdx;
}PucchFormat4;

typedef struct pucchResrcInfo
{
   uint8_t  resrcId;
   uint16_t startPrb;
   uint8_t  intraFreqHop;
   uint16_t secondPrbHop;
   uint8_t  pucchFormat;
   union
   {
      PucchFormat0   *format0; 
      PucchFormat1   *format1;
      PucchFormat2_3 *format2;
      PucchFormat2_3 *format3;
      PucchFormat4   *format4;
   }PucchFormat;
}PucchResrcInfo;

typedef struct pucchResrcCfg
{
   uint8_t resrcToAddModListCount;
   PucchResrcInfo resrcToAddModList[MAX_NUM_PUCCH_RESRC];
   uint8_t resrcToRelListCount;
   uint8_t resrcToRelList[MAX_NUM_PUCCH_RESRC];
}PucchResrcCfg;

typedef struct pucchFormatCfg
{
   bool interSlotFreqHop;
   bool addDmrs;
   uint8_t maxCodeRate;
   uint8_t numSlots;
   bool    pi2BPSK;
   bool    harqAckCSI;
}PucchFormatCfg;

typedef struct schedReqResrcInfo
{
   uint8_t resrcId;
   uint8_t requestId;
   uint8_t periodicity;
   uint16_t offset;
   uint8_t resrc;
}SchedReqResrcInfo;

typedef struct pucchSchedReqCfg
{
   uint8_t           schedAddModListCount;
   SchedReqResrcInfo schedAddModList[MAX_NUM_SR_CFG_PER_CELL_GRP];
   uint8_t           schedRelListCount;
   uint8_t           schedRelList[MAX_NUM_SR_CFG_PER_CELL_GRP];
}PucchSchedReqCfg;

typedef struct spatialRelationInfo
{
   uint8_t spatialRelationId;
   uint8_t servCellIdx;
   uint8_t pathLossRefRSId;
   uint8_t p0PucchId;
   uint8_t closeLoopIdx;
}SpatialRelationInfo;

typedef struct pucchSpatialCfg
{
   uint8_t spatialAddModListCount;
   SpatialRelationInfo spatialAddModList[MAX_NUM_SPATIAL_RELATIONS];
   uint8_t spatialRelListCount;
   uint8_t spatialRelList[MAX_NUM_SPATIAL_RELATIONS];
}PucchSpatialCfg;

typedef struct p0PucchCfg
{
   uint8_t p0PucchId;
   int     p0PucchVal;
}P0PucchCfg;

typedef struct pathLossRefRSCfg
{
   uint8_t pathLossRefRSId;
}PathLossRefRSCfg;

typedef struct pucchMultiCsiCfg
{
   uint8_t  multiCsiResrcListCount;
   uint8_t  multiCsiResrcList[MAX_NUM_PUCCH_RESRC-1];
}PucchMultiCsiCfg;

typedef struct pucchDlDataToUlAck
{
   uint8_t  dlDataToUlAckListCount;
   uint8_t  dlDataToUlAckList[MAX_NUM_DL_DATA_TO_UL_ACK];
}PucchDlDataToUlAck;

typedef struct pucchPowerControl
{
   int deltaF_Format0;
   int deltaF_Format1;
   int deltaF_Format2;
   int deltaF_Format3;
   int deltaF_Format4;
   uint8_t p0SetCount;
   P0PucchCfg p0Set[MAX_NUM_PUCCH_P0_PER_SET];
   uint8_t pathLossRefRSListCount;
   PathLossRefRSCfg pathLossRefRSList[MAX_NUM_PATH_LOSS_REF_RS];
}PucchPowerControl;

typedef struct pucchCfg
{
   PucchResrcSetCfg  *resrcSet;
   PucchResrcCfg     *resrc;
   PucchFormatCfg    *format1; 
   PucchFormatCfg    *format2; 
   PucchFormatCfg    *format3; 
   PucchFormatCfg    *format4;
   PucchSchedReqCfg  *schedReq;
   PucchMultiCsiCfg  *multiCsiCfg;
   PucchSpatialCfg   *spatialInfo;
   PucchDlDataToUlAck *dlDataToUlAck;
   PucchPowerControl *powerControl;
}PucchCfg;

/* Transform precoding disabled */
typedef struct transPrecodDisabled
{
   uint16_t   scramblingId0;
}TransPrecodDisabled;

/* DMRS Uplink configuration */
typedef struct dmrsUlCfg
{
   AddPosType            addPos;               /* DMRS additional position */
   TransPrecodDisabled   transPrecodDisabled;  /* Transform precoding disabled */
}DmrsUlCfg;

/* PUSCH Configuration */
typedef struct puschCfg
{
   uint8_t                 dataScramblingId;
   DmrsUlCfg               dmrsUlCfgForPuschMapTypeA;
   ResAllocType            resourceAllocType;
   uint8_t                 numTimeDomRsrcAlloc;
   PuschTimeDomRsrcAlloc   timeDomRsrcAllocList[MAX_NUM_UL_ALLOC]; /* PUSCH time domain UL resource allocation list */
   TransformPrecoder       transformPrecoder;
}PuschCfg;

/* Initial Uplink BWP */
typedef struct initialUlBwp
{
   bool       pucchPresent;
   PucchCfg   pucchCfg; 
   bool       puschPresent;
   PuschCfg   puschCfg;
}InitialUlBwp;

typedef struct bwpUlCfgDed
{
   PucchCfg   pucchCfg;
   PuschCfg   puschCfg;
}BwpUlCfgDed;

/* Uplink BWP information */
typedef struct ulBwpInfo
{
   uint8_t          bwpId;
   BwpUlConfig      bwpCommon;
   BwpUlCfgDed      bwpDed;
}UlBwpInfo;

typedef struct rachCfgGeneric
{
   uint8_t      prachCfgIdx;       /* PRACH config idx */
   uint8_t      msg1Fdm;           /* PRACH FDM (1,2,4,8) */
   uint16_t     msg1FreqStart;     /* Msg1-FrequencyStart */
   uint8_t      zeroCorrZoneCfg;   /* Zero correlation zone cofig */
   int16_t      preambleRcvdTargetPower; /*Prach Target power received*/
   uint8_t      preambleTransMax;  /*Preamble Transmission Max power*/
   uint8_t      pwrRampingStep;    /*Power Ramping Step*/
   uint8_t      raRspWindow;       /* RA Response Window */
}RachCfgGeneric;

typedef struct raPrioritization
{
   uint8_t powerRampingStepHighPriority;
   uint8_t scalingFactorBI;
}RaPrioritization;

typedef struct bfrCsiRsRes
{
   uint8_t csrRsIndex;
   uint8_t raOccList;
   uint8_t raPreambleIndex;
}BfrCsiRsRes;

typedef struct bfrSsbRes
{
   uint16_t ssbIndex;
   uint8_t raPreambleIndex;
}BfrSsbRes;

typedef struct prachResDedBfr
{
   BfrSsbRes    ssb;
   BfrCsiRsRes  csiRs;
}PrachResDedBfr;

/*Spec 38.331 'BeamFailureRecoveryConfig' */
typedef struct beamFailRecoveryCfg
{
   uint8_t             rootSeqIndexBfr;
   RachCfgGeneric      rachCfgBfr;
   uint8_t             rsrpThreshSsbBfr;
   PrachResDedBfr      candidteBeamRSList;
   uint8_t             ssbPerachBfr;
   uint8_t             raSsbOccMaskIndex;
   uint8_t             recoverySearchSpaceId;
   RaPrioritization    raPrioBfr;
   uint16_t            bfrTimer;
   uint8_t             msg1SubcSpacing;
}BeamFailRecoveryCfg;

/* Serving cell configuration */
typedef struct servCellCfgInfo
{
   InitialDlBwp         initDlBwp;
   RadioLinkConfig      radioLinkMonConfig;
   uint8_t              numDlBwpToAdd;
   DlBwpInfo            dlBwpToAddList[MAX_NUM_BWP];
   uint8_t              firstActvDlBwpId;
   uint8_t              defaultDlBwpId;
   uint8_t              *bwpInactivityTmr;
   PdschServCellCfg     pdschServCellCfg;
   InitialUlBwp         initUlBwp;
   BeamFailRecoveryCfg  beamFailureRecoveryCfg;
   uint8_t              numUlBwpToAdd;
   UlBwpInfo            ulBwpToAddList[MAX_NUM_BWP];
   uint8_t              firstActvUlBwpId;
}ServCellCfgInfo;

/* Special cell configuration */
typedef struct spCellCfg
{
   uint8_t           servCellIdx;
   ServCellCfgInfo   servCellCfg;
}SpCellCfg;

typedef struct bwpRelInfo
{
   uint8_t bwpId;
}BwpRelInfo;

/* Serving cell Re-configuration */
typedef struct servCellRecfgInfo
{
   InitialDlBwp       initDlBwp;
   RadioLinkConfig    radioLinkMonConfig;
   uint8_t            numDlBwpToAddOrMod;
   DlBwpInfo          dlBwpToAddOrModList[MAX_NUM_BWP];
   uint8_t            numDlBwpToRel;
   BwpRelInfo         dlBwpToRelList[MAX_NUM_BWP];
   uint8_t            firstActvDlBwpId;
   uint8_t            defaultDlBwpId;
   uint8_t            *bwpInactivityTmr;
   PdschServCellCfg   pdschServCellCfg;
   InitialUlBwp       initUlBwp;
   uint8_t            numUlBwpToAddOrMod;
   UlBwpInfo          ulBwpToAddOrModList[MAX_NUM_BWP];
   uint8_t            numUlBwpToRel;
   BwpRelInfo         ulBwpToRelList[MAX_NUM_BWP];
   uint8_t            firstActvUlBwpId;
}ServCellRecfgInfo;

/* Special cell Re-configuration */
typedef struct spCellRecfg 
{
   uint8_t             servCellIdx;
   ServCellRecfgInfo   servCellCfg;
}SpCellRecfg;

typedef struct ambrCfg
{
   uint32_t ulBr;   /* UL Bit rate */
}AmbrCfg;

typedef struct nonDynFiveQi
{
   uint16_t   fiveQi;
   uint8_t    priorLevel;
   uint16_t   avgWindow;
   uint16_t   maxDataBurstVol;
}NonDynFiveQi;

typedef struct dynFiveQi
{
   uint8_t    priorLevel;
   uint16_t   packetDelayBudget;
   uint8_t    packetErrRateScalar;
   uint8_t    packetErrRateExp;
   uint16_t   fiveQi;
   uint8_t    delayCritical;
   uint16_t   avgWindow;
   uint16_t   maxDataBurstVol;
}DynFiveQi;

typedef struct ngRanAllocAndRetPri
{
   uint8_t priorityLevel;
   uint8_t preEmptionCap;
   uint8_t preEmptionVul;
}NgRanAllocAndRetPri;

typedef struct grbQosInfo
{
   uint32_t maxFlowBitRateDl;
   uint32_t maxFlowBitRateUl;
   uint32_t guarFlowBitRateDl;
   uint32_t guarFlowBitRateUl;
}GrbQosInfo;

typedef struct drbQos
{
   QosType  fiveQiType;   /* Dynamic or non-dynamic */ 
   union
   {
      NonDynFiveQi   nonDyn5Qi;
      DynFiveQi      dyn5Qi;
   }u;
   NgRanAllocAndRetPri ngRanRetPri;
   GrbQosInfo grbQosInfo;
   uint16_t             pduSessionId;
   uint32_t             ulPduSessAggMaxBitRate;
}DrbQosInfo;

typedef struct ulLcCfg
{
   uint8_t priority;
   uint8_t lcGroup;
   uint8_t schReqId;
   PBitRate pbr;        // prioritisedBitRate
   BucketSizeDur bsd;        // bucketSizeDuration
}UlLcCfg;

typedef struct dlLcCfg
{
   LcPriority lcp;      // logical Channel Prioritization
}DlLcCfg;

typedef struct lcCfg
{
   uint8_t lcId;
   Snssai  *snssai;
   DrbQosInfo *drbQos; 
   bool ulLcCfgPres;
   UlLcCfg ulLcCfg;
   DlLcCfg dlLcCfg;
}LcCfg;

typedef struct modulationInfo
{
   uint8_t     modOrder;    /* Modulation order */
   uint8_t     mcsIndex;    /* MCS Index */
   McsTable    mcsTable;    /* MCS table */
}ModulationInfo;

typedef struct macUeCreateReq
{
   uint16_t               cellId;
   uint8_t                ueId;
   uint8_t                beamIdx;
   uint16_t               crnti;
   bool                   macCellGrpCfgPres;
   MacCellGrpCfg          macCellGrpCfg;
   bool                   phyCellGrpCfgPres;
   PhyCellGrpCfg          phyCellGrpCfg;
   bool                   spCellCfgPres;
   SpCellCfg              spCellCfg;
   AmbrCfg                *ambrCfg;
   ModulationInfo         dlModInfo;    /* DL modulation info */ 
   ModulationInfo         ulModInfo;    /* UL modulation info */
   uint8_t                numLcs;
   LcCfg                  lcCfgList[MAX_NUM_LC];
}MacUeCreateReq;

/* UE Re-configuration */
typedef struct macUeRecfg
{
   uint16_t        cellId;
   uint8_t         ueId;
   uint8_t         beamIdx;
   uint16_t        crnti;
   bool            macCellGrpRecfgPres;
   MacCellGrpCfg   macCellGrpRecfg;
   bool            phyCellGrpRecfgPres;
   PhyCellGrpCfg   phyCellGrpRecfg;
   bool            spCellRecfgPres;
   SpCellRecfg     spCellRecfg;
   AmbrCfg         *ambrRecfg;
   ModulationInfo  dlModInfo;
   ModulationInfo  ulModInfo;
   uint8_t         numLcsToAdd;
   LcCfg           lcCfgAdd[MAX_NUM_LC];
   uint8_t         numLcsToDel;
   uint8_t         lcIdToDel[MAX_NUM_LC];
   uint8_t         numLcsToMod;
   LcCfg           lcCfgMod[MAX_NUM_LC];
   DataTransmissionAction transmissionAction;
#ifdef NR_DRX   
   bool            drxConfigIndicatorRelease;
#endif
}MacUeRecfg;

typedef struct nrcgi
{
   Plmn      plmn;
   uint16_t  cellId;
}Nrcgi;

typedef struct srbFailInfo
{
   uint8_t       srbId;
   FailureCause  cause;
}SRBFailInfo;

typedef struct drbFailInfo
{
   uint8_t       drbId;
   FailureCause  cause;
}DRBFailInfo;

typedef struct sCellFailInfo
{
   Nrcgi         nrcgi;
   FailureCause  cause;
}SCellFailInfo;

typedef struct macUeCreateRsp
{
   uint16_t       cellId;
   uint16_t       ueId;
   MacRsp         result;
   uint8_t        numSRBFailed;   /* valid values : 0 to MAX_NUM_SRB */ 
   SRBFailInfo    *failedSRBlisti;
   uint8_t        numDRBFailed;   /* valid values : 0 to MAX_NUM_DRB */
   DRBFailInfo    *failedDRBlist;
   uint8_t        numSCellFailed; /* valid values : 0 to MAX_NUM_SCELL */
   SCellFailInfo  *failedSCellList;
   uint8_t        numDRBModFailed;   /* valid values : 0 to MAX_NUM_DRB */
   DRBFailInfo    *failedDRBModlist;
}MacUeCreateRsp;

typedef struct macUeCreateRsp MacUeRecfgRsp;

typedef struct rachRsrcReq
{
   uint16_t cellId;
   uint16_t ueId;
   uint8_t  numSsb;
   uint8_t  ssbIdx[MAX_NUM_SSB];
}MacRachRsrcReq;

typedef struct macCfraSsbResource
{
   uint8_t  ssbIdx;
   uint8_t  raPreambleIdx;
}MacCfraSsbResource;

typedef struct macCfraResource
{
   uint8_t   numSsb;
   MacCfraSsbResource ssbResource[MAX_NUM_SSB];
}MacCfraResource;

typedef struct macRachRsrcRsp
{
   uint16_t   cellId;
   uint16_t   ueId;
   MacRsp     result;
   uint16_t   newCrnti;     /* This parameter maps to 3GPP TS 38.331 newUE-Identity */
   MacCfraResource  cfraResource;
}MacRachRsrcRsp;

typedef struct macRachRsrcRel
{
   uint16_t cellId;
   uint16_t ueId;
   uint16_t crnti;
}MacRachRsrcRel;

typedef struct ueDelete
{
    uint16_t cellId;
    uint8_t  ueId;
    uint16_t crnti;
}MacUeDelete;

typedef struct ueDeleteRsp
{
   uint16_t cellId;
   uint8_t  ueId;
   CauseOfResult  status;
}MacUeDeleteRsp;

typedef struct macCellDeleteReq
{
   uint16_t cellId;
}MacCellDeleteReq;

typedef struct macCellDeleteRsp
{
   uint16_t cellId;
   CauseOfResult  status;
}MacCellDeleteRsp;

typedef struct macSliceCfgRsp 
{
   Snssai     snssai;
   MacRsp     rsp;
   CauseOfResult  cause;  
}MacSliceCfgRsp;

typedef struct rrmPolicyRatio
{
   uint8_t maxRatio;
   uint8_t minRatio;
   uint8_t dedicatedRatio;
}RrmPolicyRatio;

typedef struct rrmPolicyMemberList
{
   Plmn    plmn;
   Snssai  snssai;
}RrmPolicyMemberList;

typedef struct macSliceRrmPolicy
{
   ResourceType        resourceType;
   uint8_t             numOfRrmPolicyMem;
   RrmPolicyMemberList **rRMPolicyMemberList;
   RrmPolicyRatio      policyRatio;
}MacSliceRrmPolicy;

typedef struct macSliceCfgReq
{
   uint8_t           numOfRrmPolicy;
   MacSliceRrmPolicy **listOfRrmPolicy;
}MacSliceCfgReq;

/*As per ORAN-WG8, Slice Cfg and ReCfg are same structures*/
typedef struct macSliceCfgReq MacSliceRecfgReq;
typedef struct macSliceCfgRsp MacSliceRecfgRsp;

/*  Ref: ORAN_WG8.V7.0.0 Sec 1.1.1.17 DL PCCH Indication */
typedef struct dlPcchInd
{
   uint16_t  cellId;
   uint16_t  pf;
   uint8_t   i_s;
   uint16_t  pduLen;
   uint8_t  *pcchPdu;
}DlPcchInd;


/*  Ref: ORAN_WG8.V7.0.0 Sec 1.1.1.1 Cell Start */
typedef struct cellInfo
{
    SlotTimingInfo slotInfo;
    uint16_t       cellId;
}CellInfo;

typedef struct cellInfo CellStartInfo;
typedef struct cellInfo CellStopInfo;

/*  Ref: ORAN_WG8.V7.0.0 Sec 1.1.1.12 UE Reset Request */
typedef struct ueReset
{
    uint16_t cellId;
    uint8_t  ueId;
}MacUeResetReq;

/*  Ref: ORAN_WG8.V7.0.0 Sec 1.1.1.13 UE Reset Response */
typedef struct ueResetRsp
{
   uint16_t cellId;
   uint8_t  ueId;
   CauseOfResult  status;
}MacUeResetRsp;

/*  Ref: ORAN_WG8.V7.0.0 Sec 1.1.1.14 UE Sync Status Indication */
typedef struct ueSyncStatusInd
{
   uint16_t   cellId;
   uint8_t    ueId;
   SyncStatus status;
}MacUeSyncStatusInd;

/* The following list of structures is taken from the SI-SchedulingInfo section of specification 33.331. */
typedef struct sibTypeInfo
{
   SibType sibType;
   uint8_t valueTag;
   bool    areaScope;
}SibTypeInfo;

typedef struct sibMappingInfo
{
   uint8_t      numSibTypeInfo;
   SibTypeInfo  sibTypeInfo[NUM_SIB];
}SibMappingInfo;

typedef struct schedulingInfo 
{
   SiBroadcastStatus siBroadcastStatus;
   SiPeriodicity     siPeriodicity;
   SibMappingInfo    sibMappingInfo;
}SchedulingInfo;

typedef struct rachOccasionsSi
{
   RachCfgGeneric     rachConfigSi;
   SsbPerRachOccasion ssbPerRachOccasion;
}RachOccasionsSi;

typedef struct siReqRsrc 
{
   uint8_t raPreambleStartIndex;
   uint8_t raAssociationPeriodIndex;
   uint8_t raSsbOccasionMaskIndex; 
}SiReqRsrc;

typedef struct siRequestResource
{
   uint8_t    numOfSiReqRsrc;
   SiReqRsrc  siReqRsrc[MAX_SI_MESSAGE];
}SiRequestResource;

typedef struct siRequestConfig
{
   RachOccasionsSi    rachOccasionsSi;
   SiRequestPeriod    siRequestPeriod;
   SiRequestResource  siRequestResource;
}SiRequestConfig;

typedef struct schedulingInfoList
{
   uint8_t        numSchInfo;
   SchedulingInfo schedulingInfo[MAX_SI_MESSAGE];
}SchedulingInfoList;

typedef struct siSchedulingInfo
{
   SchedulingInfoList schInfoList;
   SiWindowLength     siWindowLength;
   SiRequestConfig    siRequestConfig;
   SiRequestConfig    siRequestConfigSUL;
   uint8_t            *siAreaID;
}SiSchedulingInfo;

/*  Ref: ORAN_WG8.V7.0.0 Sec 1.1.1.18 DL Broadcast Request  */
typedef struct macDlBroadcastReq
{
    uint16_t         cellId;
    uint8_t          numSiBlock;
    SiSchedulingInfo **siSchedulingInfo;
}MacDlBroadcastReq;

typedef struct macStatsGrpInfo
{
   uint8_t   groupId;
   uint16_t  periodicity;  /* In milliseconds */
   uint8_t   numStats;
   MacMeasurementType statsList[MAX_NUM_STATS];
}MacStatsGrpInfo;

typedef struct macStatsReq
{
   uint64_t          subscriptionId;
   uint8_t           numStatsGroup;
   MacStatsGrpInfo   statsGrpList[MAX_NUM_STATS_GRP];
}MacStatsReq;

typedef struct macStatsGrpRejected
{
   uint8_t  groupId;
   CauseOfResult cause;
}MacStatsGrpRejected;

typedef struct macStatsRsp
{
   uint64_t             subscriptionId;
   uint8_t              numGrpAccepted;
   uint8_t              statsGrpAcceptedList[MAX_NUM_STATS_GRP];
   uint8_t              numGrpRejected;
   MacStatsGrpRejected  statsGrpRejectedList[MAX_NUM_STATS_GRP];
}MacStatsRsp;

typedef struct macStats
{
   MacMeasurementType type;
   double value;
}MacStats;

typedef struct macStatsInd
{
   uint64_t    subscriptionId;
   uint8_t     groupId;
   uint8_t     numStats;
   MacStats    measuredStatsList[MAX_NUM_STATS];
}MacStatsInd;

typedef struct macStatsDeleteReq
{
   uint64_t    subscriptionId;
   uint8_t     numStatsGroupToBeDeleted;
   uint8_t     statsGrpIdToBeDelList[MAX_NUM_STATS_GRP];
}MacStatsDeleteReq;

typedef struct macStatsDeleteInfo
{
   uint8_t       groupId;
   MacRsp        statsGrpDelRsp;
   CauseOfResult statsGrpDelCause;
}MacStatsDeleteInfo;

typedef struct macStatsDeleteRsp
{
   uint64_t           subscriptionId; /* subscription Id */
   MacRsp             subsDelRsp;    /* deletion status of all statsGrp with given subscriptionId */
   CauseOfResult      subsDelCause;  /* cause of failure in deletion of all statsGrp with given subscriptionId */
   uint8_t            numStatsGroupDeleted; /* number of actions to deleted */ 
   MacStatsDeleteInfo statsGrpDelInfo[MAX_NUM_STATS_GRP]; /*list of the deletion statuses for specific actions */
}MacStatsDeleteRsp;

typedef struct macStatsReq MacStatsModificationReq;
typedef struct macStatsRsp MacStatsModificationRsp;

/****************** FUNCTION POINTERS ********************************/

/* DL broadcast req from DU APP to MAC*/
typedef uint8_t (*DuMacDlBroadcastReq) ARGS((
         Pst           *pst,
         MacDlBroadcastReq *dlBroadcast));

/* Functions for CellUp Ind from MAC to DU APP*/
typedef uint8_t (*DuMacCellUpInd) ARGS((
	 Pst       *pst,
	 OduCellId *cellId ));

/* Functions for slot Ind from MAC to DU APP*/
typedef uint8_t (*DuMacSlotInd) ARGS((
	 Pst       *pst,
	 SlotTimingInfo *slotIndInfo));

/* Functions for stop Ind from MAC to DU APP*/
typedef uint8_t (*DuMacStopInd) ARGS((
	 Pst        *pst,
	 OduCellId  *cellId ));

/* Functions for mac cell start req */
typedef uint8_t (*DuMacCellStart) ARGS((
	 Pst        *pst, 
	 CellStartInfo  *cellId));

/* Functions for mac cell stop request */
typedef uint8_t (*DuMacCellStop) ARGS((
	 Pst        *pst,
	 CellStopInfo  *cellId ));

/* Function pointers for packing macCellCfg Request and Confirm */
typedef uint8_t (*packMacCellCfgReq) ARGS((
	 Pst           *pst,
	 MacCellCfg    *macCellCfg ));

typedef uint8_t (*packMacCellCfgConfirm) ARGS((
	 Pst              *pst,
	 MacCellCfgCfm    *macCellCfgCfm ));

typedef uint8_t (*DuMacCellCfgReq) ARGS((
	 Pst        *pst,        
	 MacCellCfg *macCellCfg));

typedef uint8_t (*DuMacCellCfgCfm) ARGS((
	 Pst        *pst,        
	 MacCellCfgCfm *macCellCfgCfm ));

/* Functions for UL CCCH Ind from MAC to DU APP*/
typedef uint8_t (*DuMacUlCcchInd) ARGS((
	 Pst           *pst,
	 UlCcchIndInfo *ulCcchIndInfo ));

/* Functions for DL CCCH Ind from DU APP to MAC*/
typedef uint8_t (*DuMacDlCcchInd) ARGS((
	 Pst           *pst,
	 DlCcchIndInfo *dlCcchIndInfo ));

/* UE create Request from DU APP to MAC*/
typedef uint8_t (*DuMacUeCreateReq) ARGS((
	 Pst           *pst,
	 MacUeCreateReq      *ueCfg ));

/* UE create Response from MAC to DU APP */
typedef uint8_t (*MacDuUeCreateRspFunc) ARGS((
	 Pst           *pst, 
	 MacUeCreateRsp   *cfgRsp));

/* UE Reconfig Request from DU APP to MAC */
typedef uint8_t (*DuMacUeReconfigReq) ARGS((
	 Pst           *pst,
	 MacUeRecfg      *ueCfg ));

/* UE Reconfig Response from MAC to DU APP */
typedef uint8_t (*MacDuUeRecfgRspFunc) ARGS((
	 Pst           *pst, 
	 MacUeRecfgRsp   *cfgRsp));

/* RACH Resource Request from DU APP to MAC */
typedef uint8_t (*DuMacRachRsrcReq) ARGS((
    Pst            *pst,
    MacRachRsrcReq *rachRsrcReq));

/* RACH Resource Response from MAC to DU APP */
typedef uint8_t (*MacDuRachRsrcRspFunc) ARGS((
    Pst            *pst,
    MacRachRsrcRsp *rachRsrcRsp));

/* RACH Resource Release from DU APP to MAC */
typedef uint8_t (*DuMacRachRsrcRel) ARGS((
    Pst            *pst,
    MacRachRsrcRel *rachRsrcRel));

/* UE Delete Request from DU APP to MAC*/
typedef uint8_t (*DuMacUeDeleteReq) ARGS((
     Pst           *pst,
     MacUeDelete   *ueDel ));

/* UE Delete Response from MAC to DU APP*/
typedef uint8_t (*MacDuUeDeleteRspFunc) ARGS((
     Pst            *pst,
     MacUeDeleteRsp *deleteRsp));

/* Cell Delete Request from DU APP to MAC*/
typedef uint8_t (*DuMacCellDeleteReq) ARGS((
     Pst           *pst,
     MacCellDeleteReq *cellDelete ));

/* Cell Delete Response from MAC to DU APP*/
typedef uint8_t (*MacDuCellDeleteRspFunc) ARGS((
     Pst            *pst,
     MacCellDeleteRsp *cellDeleteRsp));

/* Slice Cfg Request from DU APP to MAC*/
typedef uint8_t (*DuMacSliceCfgReq) ARGS((
     Pst           *pst,
     MacSliceCfgReq *CfgReq));

/* Slice Cfg Response from MAC to DU APP */
typedef uint8_t (*MacDuSliceCfgRspFunc) ARGS((
	 Pst           *pst, 
	 MacSliceCfgRsp   *cfgRsp));

/* Slice ReReCfg Request from DU APP to MAC*/
typedef uint8_t (*DuMacSliceRecfgReq) ARGS((
     Pst           *pst,
     MacSliceRecfgReq *CfgReq));

/* Slice ReReCfg Response from MAC to DU APP */
typedef uint8_t (*MacDuSliceRecfgRspFunc) ARGS((
        Pst           *pst,
        MacSliceRecfgRsp   *cfgRsp));

/* Pcch indication from DU APP to MAC*/
typedef uint8_t (*DuMacDlPcchInd) ARGS((
     Pst        *pst,
     DlPcchInd *pcchInd));

/* UE Reset Request from DU APP to MAC*/
typedef uint8_t (*DuMacUeResetReq) ARGS((
     Pst           *pst,
     MacUeResetReq *ueReset ));

/* UE Reset Response from MAC to DU APP*/
typedef uint8_t (*MacDuUeResetRspFunc) ARGS((
     Pst            *pst,
     MacUeResetRsp *resetRsp));

/* UE sync status indication from MAC to DU APP*/
typedef uint8_t (*MacDuUeSyncStatusIndFunc) ARGS((
        Pst            *pst,
        MacUeSyncStatusInd *syncStatusInd));

/* Statitics Request from DU APP to MAC */
typedef uint8_t (*DuMacStatsReqFunc) ARGS((
      Pst *pst,
      MacStatsReq *statsReq));

/* Statistics Response from MAC to DU APP */
typedef uint8_t (*MacDuStatsRspFunc) ARGS((
      Pst *pst,
      MacStatsRsp *statsRsp));

/* Statistics Ind from MAC to DU APP */
typedef uint8_t (*MacDuStatsIndFunc) ARGS((
      Pst *pst,
      MacStatsInd *statsInd));

/* Statitics Delete Request from DU APP to MAC */
typedef uint8_t (*DuMacStatsDeleteReqFunc) ARGS((
         Pst *pst,
         MacStatsDeleteReq *statsDeleteReq));

/* Statistics Delete Response from MAC to DU APP */
typedef uint8_t (*MacDuStatsDeleteRspFunc) ARGS((
         Pst *pst,
         MacStatsDeleteRsp *statsDeleteRsp));

/* Statitics Modification Request from DU APP to MAC */
typedef uint8_t (*DuMacStatsModificationReqFunc) ARGS((
      Pst *pst,
      MacStatsModificationReq *statsModificationReq));

/* Statistics Modification Response from MAC to DU APP */
typedef uint8_t (*MacDuStatsModificationRspFunc) ARGS((
      Pst *pst,
      MacStatsModificationRsp *statsModificationRsp));

/******************** FUNCTION DECLARATIONS ********************************/
uint8_t packMacCellUpInd(Pst *pst, OduCellId *cellId);
uint8_t unpackMacCellUpInd(DuMacCellUpInd func, Pst *pst, Buffer *mBuf);
uint8_t duHandleCellUpInd(Pst *pst, OduCellId *cellId);

uint8_t packMacCellStart(Pst *pst, CellStartInfo *cellStartInfo);
uint8_t unpackMacCellStart(DuMacCellStart func, Pst *pst, Buffer *mBuf);
uint8_t MacProcCellStart(Pst *pst, CellStartInfo *cellStartInfo);

uint8_t packMacCellStop(Pst *pst, CellStopInfo *cellStopInfo);
uint8_t unpackMacCellStop(DuMacCellStop func, Pst *pst, Buffer *mBuf);
uint8_t MacProcCellStop(Pst *pst, CellStopInfo *cellStopInfo);

uint8_t packMacCellCfg(Pst *pst, MacCellCfg *macCellCfg);
uint8_t unpackDuMacCellCfg(DuMacCellCfgReq func,  Pst *pst,  Buffer *mBuf);
uint8_t MacProcCellCfgReq(Pst *pst, MacCellCfg *macCellCfg);

uint8_t packMacCellCfgCfm(Pst *pst, MacCellCfgCfm *macCellCfgCfm);
uint8_t unpackMacCellCfgCfm(DuMacCellCfgCfm func, Pst *pst, Buffer *mBuf);
uint8_t duHandleMacCellCfgCfm(Pst *pst, MacCellCfgCfm *macCellCfgCfm);

uint8_t packMacStopInd(Pst *pst, OduCellId *cellId);
uint8_t unpackMacStopInd(DuMacStopInd func, Pst *pst, Buffer *mBuf);
uint8_t duHandleStopInd(Pst *pst, OduCellId *cellId);
uint8_t sendStopIndMacToDuApp(uint16_t cellId);

uint8_t packMacUlCcchInd(Pst *pst, UlCcchIndInfo *ulCcchIndInfo);
uint8_t unpackMacUlCcchInd(DuMacUlCcchInd func, Pst *pst, Buffer *mBuf);
uint8_t duHandleUlCcchInd(Pst *pst, UlCcchIndInfo *ulCcchIndInfo);

uint8_t packMacDlCcchInd(Pst *pst, DlCcchIndInfo *dlCcchIndInfo);
uint8_t unpackMacDlCcchInd(DuMacDlCcchInd func, Pst *pst, Buffer *mBuf);
uint8_t MacProcDlCcchInd(Pst *pst, DlCcchIndInfo *dlCcchIndInfo);

uint8_t packDuMacUeCreateReq(Pst *pst, MacUeCreateReq *ueCfg);
uint8_t unpackMacUeCreateReq(DuMacUeCreateReq func, Pst *pst, Buffer *mBuf);
uint8_t MacProcUeCreateReq(Pst *pst, MacUeCreateReq *ueCfg);

uint8_t packDuMacUeCreateRsp(Pst *pst, MacUeCreateRsp *cfgRsp);
uint8_t unpackDuMacUeCreateRsp(MacDuUeCreateRspFunc func, Pst *pst, Buffer *mBuf);
uint8_t DuProcMacUeCreateRsp(Pst *pst, MacUeCreateRsp *cfgRsp);

uint8_t packDuMacUeReconfigReq(Pst *pst, MacUeRecfg *ueRecfg);
uint8_t unpackMacUeReconfigReq(DuMacUeReconfigReq func, Pst *pst, Buffer *mBuf);
uint8_t MacProcUeReconfigReq(Pst *pst, MacUeRecfg *ueRecfg);

uint8_t packDuMacUeRecfgRsp(Pst *pst, MacUeRecfgRsp *recfgRsp);
uint8_t unpackDuMacUeRecfgRsp(MacDuUeRecfgRspFunc func, Pst *pst, Buffer *mBuf);
uint8_t DuProcMacUeRecfgRsp(Pst *pst, MacUeRecfgRsp *recfgRsp);

uint8_t packDuMacRachRsrcReq(Pst *pst, MacRachRsrcReq *rachRsrcReq);
uint8_t unpackMacRachRsrcReq(DuMacRachRsrcReq func, Pst *pst, Buffer *mBuf);
uint8_t MacProcRachRsrcReq(Pst *pst, MacRachRsrcReq *rachRsrcReq);

uint8_t packDuMacRachRsrcRsp(Pst *pst, MacRachRsrcRsp *rachRsrcRsp);
uint8_t unpackDuMacRachRsrcRsp(MacDuRachRsrcRspFunc func, Pst *pst, Buffer *mBuf);
uint8_t DuProcMacRachRsrcRsp(Pst *pst, MacRachRsrcRsp *rachRsrcRsp);

uint8_t packDuMacRachRsrcRel(Pst *pst, MacRachRsrcRel *rachRsrcRel);
uint8_t unpackMacRachRsrcRel(DuMacRachRsrcRel func, Pst *pst, Buffer *mBuf);
uint8_t MacProcRachRsrcRel(Pst *pst, MacRachRsrcRel *rachRsrcRel);

uint8_t packDuMacUeDeleteReq(Pst *pst, MacUeDelete *ueDelete);
uint8_t MacProcUeDeleteReq(Pst *pst,  MacUeDelete *ueDelete);
uint8_t unpackMacUeDeleteReq(DuMacUeDeleteReq func, Pst *pst, Buffer *mBuf);

uint8_t packDuMacUeDeleteRsp(Pst *pst, MacUeDeleteRsp *deleteRsp);
uint8_t DuProcMacUeDeleteRsp(Pst *pst, MacUeDeleteRsp *deleteRsp);
uint8_t unpackDuMacUeDeleteRsp(MacDuUeDeleteRspFunc func, Pst *pst, Buffer *mBuf);

uint8_t packDuMacCellDeleteReq(Pst *pst, MacCellDeleteReq *cellDelete);
uint8_t MacProcCellDeleteReq(Pst *pst, MacCellDeleteReq *cellDelete);
uint8_t unpackMacCellDeleteReq(DuMacCellDeleteReq func, Pst *pst, Buffer *mBuf);

uint8_t packDuMacCellDeleteRsp(Pst *pst, MacCellDeleteRsp *cellDeleteRsp);
uint8_t DuProcMacCellDeleteRsp(Pst *pst, MacCellDeleteRsp *cellDeleteRsp);
uint8_t unpackDuMacCellDeleteRsp(MacDuCellDeleteRspFunc func, Pst *pst, Buffer *mBuf);

uint8_t packDuMacSliceCfgReq(Pst *pst, MacSliceCfgReq *sliceCfgReq);
uint8_t MacProcSliceCfgReq(Pst *pst, MacSliceCfgReq *sliceCfgReq);
uint8_t unpackMacSliceCfgReq(DuMacSliceCfgReq func, Pst *pst, Buffer *mBuf);

uint8_t DuProcMacSliceCfgRsp(Pst *pst,  MacSliceCfgRsp *cfgRsp);
uint8_t packDuMacSliceCfgRsp(Pst *pst, MacSliceCfgRsp *cfgRsp);
uint8_t unpackDuMacSliceCfgRsp(MacDuSliceCfgRspFunc func, Pst *pst, Buffer *mBuf);

uint8_t packDuMacSliceRecfgReq(Pst *pst, MacSliceRecfgReq *sliceRecfgReq);
uint8_t MacProcSliceRecfgReq(Pst *pst, MacSliceRecfgReq *sliceRecfgReq);
uint8_t unpackMacSliceRecfgReq(DuMacSliceRecfgReq func, Pst *pst, Buffer *mBuf);

uint8_t DuProcMacSliceRecfgRsp(Pst *pst,  MacSliceRecfgRsp *sliceRecfgRsp);
uint8_t packDuMacSliceRecfgRsp(Pst *pst, MacSliceRecfgRsp *sliceRecfgRsp);
uint8_t unpackDuMacSliceRecfgRsp(MacDuSliceRecfgRspFunc func, Pst *pst, Buffer *mBuf);

uint8_t duHandleSlotInd(Pst *pst, SlotTimingInfo *slotIndInfo);
uint8_t packMacSlotInd(Pst *pst, SlotTimingInfo *slotIndInfo);
uint8_t unpackDuMacSlotInd(DuMacSlotInd func, Pst *pst, Buffer *mBuf);

uint8_t packDuMacDlPcchInd(Pst *pst, DlPcchInd *pcchInd);
uint8_t MacProcDlPcchInd(Pst *pst, DlPcchInd *pcchInd);
uint8_t unpackMacDlPcchInd(DuMacDlPcchInd func, Pst *pst, Buffer *mBuf);

int8_t getFreeBitFromUeBitMap(uint16_t cellId);
void unsetBitInUeBitMap(uint16_t cellId, uint8_t bitPos);

uint8_t packDuMacUeResetReq(Pst *pst, MacUeResetReq *ueReset);
uint8_t MacProcUeResetReq(Pst *pst,  MacUeResetReq *ueReset);
uint8_t unpackMacUeResetReq(DuMacUeResetReq func, Pst *pst, Buffer *mBuf);

uint8_t packDuMacUeResetRsp(Pst *pst, MacUeResetRsp *resetRsp);
uint8_t DuProcMacUeResetRsp(Pst *pst, MacUeResetRsp *resetRsp);
uint8_t unpackDuMacUeResetRsp(MacDuUeResetRspFunc func, Pst *pst, Buffer *mBuf);

uint8_t packDuMacUeSyncStatusInd(Pst *pst, MacUeSyncStatusInd *ueSyncStatusInd);
uint8_t DuProcMacUeSyncStatusInd(Pst *pst, MacUeSyncStatusInd *ueSyncStatusInd);
uint8_t unpackDuMacUeSyncStatusInd(MacDuUeSyncStatusIndFunc func, Pst *pst, Buffer *mBuf);

uint8_t packDuMacDlBroadcastReq(Pst *pst, MacDlBroadcastReq *dlBroadcastReq);
uint8_t MacProcDlBroadcastReq(Pst *pst,  MacDlBroadcastReq *dlBroadcastReq);
uint8_t unpackMacDlBroadcastReq(DuMacDlBroadcastReq func, Pst *pst, Buffer *mBuf);

uint8_t packDuMacStatsReq(Pst *pst, MacStatsReq *statsReq);
uint8_t MacProcStatsReq(Pst *pst, MacStatsReq *statsReq);
uint8_t unpackMacStatsReq(DuMacStatsReqFunc func, Pst *pst, Buffer *mBuf);

uint8_t packDuMacStatsRsp(Pst *pst, MacStatsRsp *statsRsp);
uint8_t DuProcMacStatsRsp(Pst *pst, MacStatsRsp *statsRsp);
uint8_t unpackDuMacStatsRsp(MacDuStatsRspFunc func, Pst *pst, Buffer *mBuf);

uint8_t packDuMacStatsInd(Pst *pst, MacStatsInd *statsRsp);
uint8_t DuProcMacStatsInd(Pst *pst, MacStatsInd *statsRsp);
uint8_t unpackDuMacStatsInd(MacDuStatsIndFunc func, Pst *pst, Buffer *mBuf);

uint8_t packDuMacStatsDeleteReq(Pst *pst, MacStatsDeleteReq *statsDeleteReq);
uint8_t MacProcStatsDeleteReq(Pst *pst, MacStatsDeleteReq *statsDeleteReq);
uint8_t unpackMacStatsDeleteReq(DuMacStatsDeleteReqFunc func, Pst *pst, Buffer *mBuf);

uint8_t packDuMacStatsDeleteRsp(Pst *pst, MacStatsDeleteRsp *statsDeleteRsp);
uint8_t DuProcMacStatsDeleteRsp(Pst *pst, MacStatsDeleteRsp *statsDeleteRsp);
uint8_t unpackDuMacStatsDeleteRsp(MacDuStatsDeleteRspFunc func, Pst *pst, Buffer *mBuf);

uint8_t packDuMacStatsModificationReq(Pst *pst, MacStatsModificationReq *statsModificationReq);
uint8_t MacProcStatsModificationReq(Pst *pst, MacStatsModificationReq *statsModificationReq);
uint8_t unpackMacStatsModificationReq(DuMacStatsModificationReqFunc func, Pst *pst, Buffer *mBuf);

uint8_t packDuMacStatsModificationRsp(Pst *pst, MacStatsModificationRsp *statsModificationRsp);
uint8_t DuProcMacStatsModificationRsp(Pst *pst, MacStatsModificationRsp *statsModificationRsp);
uint8_t unpackDuMacStatsModificationRsp(MacDuStatsModificationRspFunc func, Pst *pst, Buffer *mBuf);


#endif


/**********************************************************************
  End of file
 **********************************************************************/
