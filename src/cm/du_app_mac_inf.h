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
#define SIB1_MCS  4

/* Macro for Ue Context */
#define MAX_NUM_SR_CFG_PER_CELL_GRP 8   /* Max number of scheduling request config per cell group */
#define MAC_NUM_TAGS 4                  /* Max number of timing advance groups */
#define MAX_NUM_BWP  4                  /* Max number of BWP per serving cell */
#define MAX_NUM_CRSET  3                /* Max number of control resource set in add/modify/release list */
#define MAX_NUM_SEARCH_SPC  10          /* Max number of search space in add/modify/release list */
#define MONITORING_SYMB_WITHIN_SLOT_SIZE 2  /* i.e. 2 bytes because size of monitoring symbols within slot is 14 bits */
#define MAX_NUM_DL_ALLOC 16             /* Max number of pdsch time domain downlink allocation */
#define MAX_NUM_UL_ALLOC 16             /* Max number of pusch time domain uplink allocation */
#define SD_SIZE   3                     /* Max size of Slice Differentiator in S-NSSAI */

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

#define BSR_PERIODIC_TIMER_SF_10 10
#define BSR_RETX_TIMER_SF_320 320
#define BSR_SR_DELAY_TMR_2560 2560

typedef enum
{
   MAC_DU_APP_RSP_NOK,
   MAC_DU_APP_RSP_OK
}MacRsp;

typedef enum
{
   SUCCESS,
   CELLID_INVALID,
   UEIDX_INVALID
}UeDeleteStatus;

typedef enum
{
   SUCCESSFUL_RSP,
   CELL_ID_INVALID
}CellDeleteStatus;

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
   BETA_PSS_1DB
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
   SLOTPERIODICITYANDOFFSET_PR_SL1 = 1,
   SLOTPERIODICITYANDOFFSET_PR_SL2,
   SLOTPERIODICITYANDOFFSET_PR_SL4,
   SLOTPERIODICITYANDOFFSET_PR_SL5,
   SLOTPERIODICITYANDOFFSET_PR_SL8,
   SLOTPERIODICITYANDOFFSET_PR_SL10,
   SLOTPERIODICITYANDOFFSET_PR_SL16,
   SLOTPERIODICITYANDOFFSET_PR_SL20,
   SLOTPERIODICITYANDOFFSET_PR_SL40,
   SLOTPERIODICITYANDOFFSET_PR_SL80,
   SLOTPERIODICITYANDOFFSET_PR_SL160,
   SLOTPERIODICITYANDOFFSET_PR_SL320,
   SLOTPERIODICITYANDOFFSET_PR_SL640,
   SLOTPERIODICITYANDOFFSET_PR_SL1280,
   SLOTPERIODICITYANDOFFSET_PR_SL2560
}MSlotPeriodAndOffset;

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

typedef struct failureCause
{
   CauseGrp   type;
   union
   {
      RadioNwLyrCause   radioNwCause;
      TransLyrCause     transportCause;
      ProtCause         protcolCause;
      MiscFailCause     miscCause;
   }u;
}FailureCause;

typedef struct carrierCfg
{
   bool  pres;
   uint32_t   bw;             /* DL/UL bandwidth */
   uint32_t   freq;           /* Absolute frequency of DL/UL point A in KHz */
   uint16_t   k0[NUM_NUMEROLOGY];          /* K0 for DL/UL */
   uint16_t   gridSize[NUM_NUMEROLOGY];    /* DL/UL Grid size for each numerologies */
   uint16_t   numAnt;         /* Number of Tx/Rx antennas */
}CarrierCfg;

typedef struct ssbCfg
{
   uint32_t    ssbPbchPwr;       /* SSB block power */
   BchPduOpt   bchPayloadFlag;   /* Options for generation of payload */
   uint8_t     scsCmn;           /* subcarrier spacing for common */
   uint16_t    ssbOffsetPointA;  /* SSB subcarrier offset from point A */
   BetaPss     betaPss;
   SSBPeriod   ssbPeriod;        /* SSB Periodicity in msec */
   uint8_t     ssbScOffset;       /* Subcarrier Offset */
   uint8_t     mibPdu[3];           /* MIB payload */
   uint32_t    ssbMask[SSB_MASK_SIZE];      /* Bitmap for actually transmitted SSB. */
   uint8_t     beamId[NUM_SSB];
   bool        multCarrBand;     /* Multiple carriers in a band */
   bool        multCellCarr;     /* Multiple cells in single carrier */
}SsbCfg;

typedef struct fdmInfo
{
   uint16_t   rootSeqIdx;        /* Root sequence index */
   uint8_t    numRootSeq;        /* Number of root sequences required for FD */
   uint16_t   k1;                /* Frequency Offset for each FD */
   uint8_t    zeroCorrZoneCfg;   /* Zero correlation zone cofig */
   uint8_t    numUnusedRootSeq;  /* Number of unused root sequence */
   uint8_t    *unsuedRootSeq;     /* Unused root sequence per FD */
}PrachFdmInfo;

typedef struct prachCfg
{
   bool          pres;
   uint8_t       prachCfgIdx;         /* PRACH Cfg Index */
   PrachSeqLen   prachSeqLen;         /* RACH Sequence length: Long/short */
   uint8_t       prachSubcSpacing;    /* Subcarrier spacing of RACH */
   RstSetCfg     prachRstSetCfg;      /* PRACH restricted set config */
   uint16_t      msg1FreqStart;       /* Msg1-FrequencyStart */
   uint8_t       msg1Fdm;             /* PRACH FDM (1,2,4,8) */
   uint8_t       rootSeqLen;          /* Root sequence length */
   PrachFdmInfo  fdm[8];              /* FDM info */
   uint8_t       ssbPerRach;          /* SSB per RACH occassion */
   bool          prachMultCarrBand;   /* Multiple carriers in Band */
   uint8_t       prachRestrictedSet; /* Support for PRACH restricted set */
   uint8_t       raContResTmr;        /* RA Contention Resoultion Timer */
   uint8_t       rsrpThreshSsb;       /* RSRP Threshold SSB */
   uint8_t       raRspWindow;         /* RA Response Window */
}PrachCfg;

typedef struct sib1CellCfg
{
   uint8_t  *sib1Pdu;
   uint16_t sib1PduLen;
   uint16_t sib1RepetitionPeriod;
   uint8_t coresetZeroIndex;     /* derived from 4 LSB of pdcchSib1 present in MIB */
   uint8_t searchSpaceZeroIndex; /* derived from 4 MSB of pdcchSib1 present in MIB */
   uint16_t sib1Mcs;
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
   uint8_t   k2;
   CommonMappingType   mappingType;
   uint8_t           startSymbol;
   uint8_t           symbolLength;
   uint8_t   startSymbolAndLength;
}PuschTimeDomRsrcAlloc;


typedef struct puschConfigCommon
{
   /* PUSCH-TimeDomainResourceAllocation info */
   uint8_t                 numTimeDomRsrcAlloc;
   PuschTimeDomRsrcAlloc   timeDomRsrcAllocList[MAX_NUM_UL_ALLOC]; /* PUSCH time domain UL resource allocation list */
}PuschConfigCommon;

typedef struct bwpDlConfig
{
   BwpParams      bwp;
   PdcchConfigCommon pdcchCommon;
   PdschConfigCommon pdschCommon;
}BwpDlConfig;

typedef struct bwpUlConfig
{
   BwpParams      bwp;
   // rach config common sent in PrachCfg
   PucchConfigCommon pucchCommon;
   PuschConfigCommon puschCommon;
}BwpUlConfig;

typedef struct macCellCfg
{
   uint16_t       cellId;           /* Cell Id */
   uint8_t        carrierId;        /* Carrired Index */
   uint16_t       phyCellId;        /* Physical cell id */
   uint8_t        numerology;       /* Supported numerology */
   DuplexMode     dupType;          /* Duplex type: TDD/FDD */
   CarrierCfg     dlCarrCfg;        /* DL Carrier configuration */
   CarrierCfg     ulCarrCfg;        /* UL Carrier configuration */
   bool           freqShft;         /* Indicates presence of 7.5kHz frequency shift */
   SsbCfg         ssbCfg;           /* SSB configuration */          
   PrachCfg       prachCfg;         /* PRACH Configuration */
#ifdef NR_TDD
   TDDCfg         tddCfg;           /* TDD periodicity and slot configuration */
#endif
   RSSIMeasUnit   rssiUnit;         /* RSSI measurement unit */
   Sib1CellCfg    sib1Cfg;          /* SIB1 config */
   BwpDlConfig    initialDlBwp;     /* Initial DL BWP */
   BwpUlConfig    initialUlBwp;     /* Initial UL BWP */
   uint8_t        dmrsTypeAPos;     /* DMRS Type A position */
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

/* BWP Downlink common */
typedef struct bwpDlCommon
{
}BwpDlCommon;

/* Downlink BWP information */
typedef struct dlBwpInfo
{
   uint8_t          bwpId;
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
   uint8_t interSlotFreqHop;
   uint8_t addDmrs;
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

/* Uplink BWP information */
typedef struct ulBwpInfo
{
   uint8_t        bwpId;
}UlBwpInfo;

/* Serving cell configuration */
typedef struct servCellCfgInfo
{
   InitialDlBwp       initDlBwp;
   uint8_t            numDlBwpToAdd;
   DlBwpInfo          DlBwpToAddList[MAX_NUM_BWP];
   uint8_t            firstActvDlBwpId;
   uint8_t            defaultDlBwpId;
   uint8_t            *bwpInactivityTmr;
   PdschServCellCfg   pdschServCellCfg;
   InitialUlBwp       initUlBwp;
   uint8_t            numUlBwpToAdd;
   UlBwpInfo          UlBwpToAddList[MAX_NUM_BWP];
   uint8_t            firstActvUlBwpId;
}ServCellCfgInfo;

/* Special cell configuration */
typedef struct spCellCfg
{
   uint8_t           servCellIdx;
   ServCellCfgInfo   servCellCfg;
}SpCellCfg;

typedef struct ambrCfg
{
   uint32_t ulBr;   /* UL Bit rate */
}AmbrCfg;

/* Single Network Slice Selection assistance Info */
typedef struct snssai
{
   uint8_t sst;                /* Slice Type */
   uint8_t sd[SD_SIZE];        /* Slice Differentiator */
}Snssai;

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

typedef struct duLcCfg
{
   LcPriority lcp;      // logical Channel Prioritization
}DlLcCfg;

typedef struct lcCfg
{
   ConfigType configType;
   uint8_t lcId;
   DrbQosInfo *drbQos; 
   Snssai  *snssai;
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

typedef struct macUeCfg
{
   uint16_t cellId;
   uint8_t  ueIdx;
   uint16_t crnti;
   bool macCellGrpCfgPres;
   MacCellGrpCfg macCellGrpCfg;
   bool phyCellGrpCfgPres;
   PhyCellGrpCfg phyCellGrpCfg;
   bool spCellCfgPres;
   SpCellCfg spCellCfg;
   AmbrCfg   *ambrCfg;
   ModulationInfo dlModInfo;    /* DL modulation info */
   ModulationInfo ulModInfo;    /* UL modulation info */
   uint8_t numLcs;
   LcCfg lcCfgList[MAX_NUM_LC];
   UeCfgState macUeCfgState;    /* InActive / Completed */
}MacUeCfg;

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

typedef struct ueCfgRsp
{
   uint16_t       cellId;
   uint16_t       ueIdx;
   MacRsp         result;
   uint8_t        numSRBFailed;   /* valid values : 0 to MAX_NUM_SRB */ 
   SRBFailInfo    *failedSRBlisti;
   uint8_t        numDRBFailed;   /* valid values : 0 to MAX_NUM_DRB */
   DRBFailInfo    *failedDRBlist;
   uint8_t        numSCellFailed; /* valid values : 0 to MAX_NUM_SCELL */
   SCellFailInfo  *failedSCellList;
}MacUeCfgRsp;

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
   UeDeleteStatus result;
}MacUeDeleteRsp;

typedef struct macCellDelete
{
   uint16_t cellId;
}MacCellDelete;

typedef struct macCellDeleteRsp
{
   uint16_t cellId;
   CellDeleteStatus result;
}MacCellDeleteRsp;

/* Functions for slot Ind from MAC to DU APP*/
typedef uint8_t (*DuMacCellUpInd) ARGS((
	 Pst       *pst,
	 OduCellId *cellId ));

/* Functions for stop Ind from MAC to DU APP*/
typedef uint8_t (*DuMacStopInd) ARGS((
	 Pst        *pst,
	 OduCellId  *cellId ));

/* Functions for mac cell start req */
typedef uint8_t (*DuMacCellStart) ARGS((
	 Pst        *pst, 
	 OduCellId  *cellId));

/* Functions for mac cell stop request */
typedef uint8_t (*DuMacCellStop) ARGS((
	 Pst        *pst,
	 OduCellId  *cellId ));

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
	 MacUeCfg      *ueCfg ));

/* UE create Response from MAC to DU APP */
typedef uint8_t (*MacDuUeCfgRspFunc) ARGS((
	 Pst           *pst, 
	 MacUeCfgRsp   *cfgRsp));

/* UE Reconfig Request from DU APP to MAC*/
typedef uint8_t (*DuMacUeReconfigReq) ARGS((
	 Pst           *pst,
	 MacUeCfg      *ueCfg ));

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
     MacCellDelete *cellDelete ));

/* Cell Delete Response from MAC to DU APP*/
typedef uint8_t (*MacDuCellDeleteRspFunc) ARGS((
     Pst            *pst,
     MacCellDeleteRsp *cellDeleteRsp));

uint8_t packMacCellUpInd(Pst *pst, OduCellId *cellId);
uint8_t unpackMacCellUpInd(DuMacCellUpInd func, Pst *pst, Buffer *mBuf);
uint8_t duHandleCellUpInd(Pst *pst, OduCellId *cellId);
uint8_t packMacCellStart(Pst *pst, OduCellId *cellId);
uint8_t unpackMacCellStart(DuMacCellStart func, Pst *pst, Buffer *mBuf);
uint8_t MacProcCellStart(Pst *pst, OduCellId *cellId);
uint8_t packMacCellStop(Pst *pst, OduCellId *cellId);
uint8_t unpackMacCellStop(DuMacCellStop func, Pst *pst, Buffer *mBuf);
uint8_t MacProcCellStop(Pst *pst, OduCellId *cellId);
uint8_t packMacCellCfg(Pst *pst, MacCellCfg *macCellCfg);
uint8_t unpackDuMacCellCfg(DuMacCellCfgReq func,  Pst *pst,  Buffer *mBuf);
uint8_t MacProcCellCfgReq(Pst *pst, MacCellCfg *macCellCfg);
uint8_t packMacCellCfgCfm(Pst *pst, MacCellCfgCfm *macCellCfgCfm);
uint8_t unpackMacCellCfgCfm(DuMacCellCfgCfm func, Pst *pst, Buffer *mBuf);
uint8_t duHandleMacCellCfgCfm(Pst *pst, MacCellCfgCfm *macCellCfgCfm);
uint8_t packMacStopInd(Pst *pst, OduCellId *cellId);
uint8_t unpackMacStopInd(DuMacStopInd func, Pst *pst, Buffer *mBuf);
uint8_t duHandleStopInd(Pst *pst, OduCellId *cellId);
uint8_t packMacUlCcchInd(Pst *pst, UlCcchIndInfo *ulCcchIndInfo);
uint8_t unpackMacUlCcchInd(DuMacUlCcchInd func, Pst *pst, Buffer *mBuf);
uint8_t duHandleUlCcchInd(Pst *pst, UlCcchIndInfo *ulCcchIndInfo);
uint8_t packMacDlCcchInd(Pst *pst, DlCcchIndInfo *dlCcchIndInfo);
uint8_t unpackMacDlCcchInd(DuMacDlCcchInd func, Pst *pst, Buffer *mBuf);
uint8_t MacProcDlCcchInd(Pst *pst, DlCcchIndInfo *dlCcchIndInfo);
uint8_t packDuMacUeCreateReq(Pst *pst, MacUeCfg *ueCfg);
uint8_t unpackMacUeCreateReq(DuMacUeCreateReq func, Pst *pst, Buffer *mBuf);
uint8_t MacProcUeCreateReq(Pst *pst, MacUeCfg *ueCfg);
uint8_t sendStopIndMacToDuApp(uint16_t cellId);
uint8_t packDuMacUeCfgRsp(Pst *pst, MacUeCfgRsp *cfgRsp);
uint8_t unpackDuMacUeCfgRsp(MacDuUeCfgRspFunc func, Pst *pst, Buffer *mBuf);
uint8_t DuProcMacUeCfgRsp(Pst *pst, MacUeCfgRsp *cfgRsp);
uint8_t packDuMacUeReconfigReq(Pst *pst, MacUeCfg *ueCfg);
uint8_t unpackMacUeReconfigReq(DuMacUeReconfigReq func, Pst *pst, Buffer *mBuf);
uint8_t MacProcUeReconfigReq(Pst *pst, MacUeCfg *ueCfg);
uint8_t packDuMacUeDeleteReq(Pst *pst, MacUeDelete *ueDelete);
uint8_t MacProcUeDeleteReq(Pst *pst,  MacUeDelete *ueDelete);
uint8_t unpackMacUeDeleteReq(DuMacUeDeleteReq func, Pst *pst, Buffer *mBuf);
uint8_t packDuMacUeDeleteRsp(Pst *pst, MacUeDeleteRsp *deleteRsp);
uint8_t DuProcMacUeDeleteRsp(Pst *pst, MacUeDeleteRsp *deleteRsp);
uint8_t unpackDuMacUeDeleteRsp(MacDuUeDeleteRspFunc func, Pst *pst, Buffer *mBuf);
uint8_t packDuMacCellDeleteReq(Pst *pst, MacCellDelete *cellDelete);
uint8_t MacProcCellDeleteReq(Pst *pst, MacCellDelete *cellDelete);
uint8_t unpackMacCellDeleteReq(DuMacCellDeleteReq func, Pst *pst, Buffer *mBuf);
uint8_t packDuMacCellDeleteRsp(Pst *pst, MacCellDeleteRsp *cellDeleteRsp);
uint8_t DuProcMacCellDeleteRsp(Pst *pst, MacCellDeleteRsp *cellDeleteRsp);
uint8_t unpackDuMacCellDeleteRsp(MacDuCellDeleteRspFunc func, Pst *pst, Buffer *mBuf);

#endif


/**********************************************************************
  End of file
 **********************************************************************/
