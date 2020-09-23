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
#define MAXIMUM_TDD_PERIODICITY 5
#define MAX_SYMB_PER_SLOT 14 

#define NUM_SSB		1	/* max value is 64 */
#define SSB_MASK_SIZE	1	/* SSB mask size is 32bit for sub6 */
#define SIB1_NEW_TX_PERIOD      160
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

#define SRB_ID_0 0
#define SRB_ID_1 1
#define SRB_ID_2 2
#define SRB_ID_3 3

/* Macro for Ue Context */
#define MAX_NUM_LOGICAL_CHANNELS 11
#define MAX_NUM_SR_CFG_PER_CELL_GRP 8   /* Max number of scheduling request config per cell group */
#define MAC_NUM_TAGS 4                  /* Max number of timing advance groups */
#define MAX_NUM_BWP  4                  /* Max number of BWP per serving cell */
#define MAX_NUM_CRSET  3                /* Max number of control resource set in add/modify/release list */
#define MAX_NUM_SEARCH_SPC  10          /* Max number of search space in add/modify/release list */
#define FREQ_DOM_RSRC_SIZE  6           /* i.e. 6 bytes because Size of frequency domain resource is 45 bits */
#define MONITORING_SYMB_WITHIN_SLOT_SIZE 2  /* i.e. 2 bytes because size of monitoring symbols within slot is 14 bits */
#define MAX_NUM_DL_ALLOC 16             /* Max number of pdsch time domain downlink allocation */
#define MAX_NUM_UL_ALLOC 16             /* Max number of pusch time domain uplink allocation */
#define SD_SIZE   3                     /* Max size of Slice Differentiator in S-NSSAI */

#define MAX_NUM_SRB    8
#define MAX_NUM_DRB    64
#define MAX_NUM_SCELL  32

/* Event IDs */
#define EVENT_MAC_CELL_CONFIG_REQ    200
#define EVENT_MAC_CELL_CONFIG_CFM    201
#define EVENT_MAC_CELL_START_REQ     202
#define EVENT_MAC_CELL_STOP_REQ      203
#define EVENT_MAC_SLOT_IND           204
#define EVENT_MAC_STOP_IND           205
#define EVENT_MAC_UL_CCCH_IND        206
#define EVENT_MAC_DL_CCCH_IND        207
#define EVENT_MAC_UE_CREATE_REQ      208
#define EVENT_MAC_UE_CREATE_RSP      209
#define EVENT_MAC_UE_RECONFIG_REQ    210
#define EVENT_MAC_UE_RECONFIG_RSP    211

typedef enum
{
   MAC_DU_APP_RSP_NOK,
   MAC_DU_APP_RSP_OK
}MacRsp;

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
   DL_SLOT,
   UL_SLOT,
   GUARD_SLOT
}SlotConfig;

typedef enum
{
   TX_PRDCTY_MS_0P5,
   TX_PRDCTY_MS_0P625,
   TX_PRDCTY_MS_1,
   TX_PRDCTY_MS_1P25,
   TX_PRDCTY_MS_2,
   TX_PRDCTY_MS_2P5,
   TX_PRDCTY_MS_5,
   TX_PRDCTY_MS_10
}DlUlTxPeriodicity;

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
   BSR_PERIODIC_TIMER_SF_1,
   BSR_PERIODIC_TIMER_SF_5,
   BSR_PERIODIC_TIMER_SF_10,
   BSR_PERIODIC_TIMER_SF_16,
   BSR_PERIODIC_TIMER_SF_20,
   BSR_PERIODIC_TIMER_SF_32,
   BSR_PERIODIC_TIMER_SF_40,
   BSR_PERIODIC_TIMER_SF_64,
   BSR_PERIODIC_TIMER_SF_80,
   BSR_PERIODIC_TIMER_SF_128,
   BSR_PERIODIC_TIMER_SF_160,
   BSR_PERIODIC_TIMER_SF_320,
   BSR_PERIODIC_TIMER_SF_640,
   BSR_PERIODIC_TIMER_SF_1280,
   BSR_PERIODIC_TIMER_SF_2560,
   BSR_PERIODIC_TIMER_INFINITY
}BsrPeriodicTimer;

typedef enum
{
   BSR_RETX_TIMER_SF_10,
   BSR_RETX_TIMER_SF_20,
   BSR_RETX_TIMER_SF_40,
   BSR_RETX_TIMER_SF_80,
   BSR_RETX_TIMER_SF_160,
   BSR_RETX_TIMER_SF_320,
   BSR_RETX_TIMER_SF_640,
   BSR_RETX_TIMER_SF_1280,
   BSR_RETX_TIMER_SF_2560,
   BSR_RETX_TIMER_SF_5120,
   BSR_RETX_TIMER_SF_10240,
   BSR_RETX_TIMER_SPARE_5,
   BSR_RETX_TIMER_SPARE_4,
   BSR_RETX_TIMER_SPARE_3,
   BSR_RETX_TIMER_SPARE_2,
   BSR_RETX_TIMER_SPARE_1
}BsrReTxTimer;

typedef enum
{
   BSR_SR_DELAY_TMR_20,
   BSR_SR_DELAY_TMR_40,
   BSR_SR_DELAY_TMR_64,
   BSR_SR_DELAY_TMR_128,
   BSR_SR_DELAY_TMR_512,
   BSR_SR_DELAY_TMR_1024,
   BSR_SR_DELAY_TMR_2560,
   BSR_SR_DELAY_TMR_SPARE_1
}BsrSrDelayTimer;

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
   uint16_t   bw;             /* DL/UL bandwidth */
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

typedef struct tddCfg
{
   bool               pres;
   DlUlTxPeriodicity  tddPeriod;      /* DL UL Transmission periodicity */
   SlotConfig         slotCfg[MAXIMUM_TDD_PERIODICITY][MAX_SYMB_PER_SLOT]; 
}TDDCfg;

typedef struct sib1CellCfg
{
   uint8_t  *sib1Pdu;
   uint16_t sib1PduLen;
   uint16_t sib1NewTxPeriod;
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

typedef struct pdschConfigCommon
{
   uint8_t k0;
   uint8_t mappingType;
   uint8_t startSymbol;
   uint8_t lengthSymbol;
}PdschConfigCommon;

typedef struct pucchConfigCommon
{
   uint8_t pucchResourceCommon;
   uint8_t pucchGroupHopping;
}PucchConfigCommon;

typedef struct puschConfigCommon
{
   /* PUSCH-TimeDomainResourceAllocation info */
   uint8_t k2;
   uint8_t mappingType;
   uint8_t startSymbol;
   uint8_t lengthSymbol;
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
   uint16_t            cellId;           /* Cell Id */
   uint8_t             numTlv;           /* Number of configuration TLVs */
   uint8_t             carrierId;        /* Carrired Index */
   uint16_t            phyCellId;        /* Physical cell id */
   DuplexMode     dupType;          /* Duplex type: TDD/FDD */
   CarrierCfg     dlCarrCfg;        /* DL Carrier configuration */
   CarrierCfg     ulCarrCfg;        /* UL Carrier configuration */
   bool           freqShft;         /* Indicates presence of 7.5kHz frequency shift */
   SsbCfg         ssbCfg;           /* SSB configuration */          
   PrachCfg       prachCfg;         /* PRACH Configuration */
   TDDCfg         tddCfg;           /* TDD periodicity and slot configuration */
   RSSIMeasUnit   rssiUnit;         /* RSSI measurement unit */
   Sib1CellCfg    sib1Cfg;          /* SIB1 config */
   BwpDlConfig    initialDlBwp;     /* Initial DL BWP */
   BwpUlConfig    initialUlBwp;     /* Initial UL BWP */
   uint8_t        dmrsTypeAPos;     /* DMRS Type A position */
}MacCellCfg;

typedef struct macCellCfgCfm
{
   uint16_t            cellId;
   uint8_t        rsp; 
}MacCellCfgCfm;

typedef struct macCellStartInfo
{
   uint16_t cellId;
}MacCellStartInfo;

typedef struct macCellStopInfo
{
   uint16_t cellId;
}MacCellStopInfo;

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
   BsrPeriodicTimer periodicTimer;
   BsrReTxTimer retxTimer;
   BsrSrDelayTimer srDelayTimer;
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
typedef struct pucchCfg
{
   /* TODO : Not used currently */ 
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

/* PUSCH Time Domain Resource Allocation */
typedef struct puschTimeDomRsrcAlloc
{
   uint8_t   k2;
   CommonMappingType   mappingType;
   uint8_t           startSymbol;
   uint8_t           symbolLength;
   uint8_t   startSymbolAndLength;
}PuschTimeDomRsrcAlloc;

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

typedef struct maxAggrBitRate
{
   uint32_t ulBits;
   uint32_t dlBits;
}MaxAggrBitRate;

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
   uint8_t lcId;
   DrbQosInfo *drbQos; 
   Snssai  *snssai;
   UlLcCfg ulLcCfg;
   DlLcCfg dlLcCfg;
}LcCfg;

typedef struct macUeCfg
{
   uint16_t cellId;
   uint8_t  ueIdx;
   uint16_t crnti;
   MacCellGrpCfg macCellGrpCfg;
   PhyCellGrpCfg phyCellGrpCfg;
   SpCellCfg spCellCfg;
   MaxAggrBitRate *maxAggrBitRate;
   uint8_t numLcsToAdd;
   LcCfg lcCfgToAddList[MAX_NUM_LOGICAL_CHANNELS];
   uint8_t numLcsToMod;
   LcCfg lcCfgToModList[MAX_NUM_LOGICAL_CHANNELS];
   uint8_t numLcsToDel;
   LcCfg lcCfgToDelList[MAX_NUM_LOGICAL_CHANNELS];
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

/* Functions for slot Ind from MAC to DU APP*/
typedef uint8_t (*DuMacSlotInd) ARGS((
	 Pst       *pst,
	 SlotIndInfo  *slotInfo ));

/* Functions for stop Ind from MAC to DU APP*/
typedef uint8_t (*DuMacStopInd) ARGS((
	 Pst       *pst,
	 MacCellStopInfo  *cellId ));

/* Functions for mac cell start req */
typedef uint8_t (*DuMacCellStartReq) ARGS((
	 Pst               *pst, 
	 MacCellStartInfo  *cellStartInfo ));

/* Functions for mac cell stop request */
typedef uint8_t (*DuMacCellStopReq) ARGS((
	 Pst               *pst,
	 MacCellStopInfo  *cellStopInfo ));

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

uint8_t packMacSlotInd(Pst *pst, SlotIndInfo *slotInfo );
uint8_t unpackMacSlotInd(DuMacSlotInd func, Pst *pst, Buffer *mBuf);
uint8_t duHandleSlotInd(Pst *pst, SlotIndInfo *slotInfo);
uint8_t packMacCellStartReq(Pst *pst, MacCellStartInfo *cellStartInfo);
uint8_t unpackMacCellStartReq(DuMacCellStartReq func, Pst *pst, Buffer *mBuf);
uint8_t MacProcCellStartReq(Pst *pst, MacCellStartInfo  *cellStartInfo);
uint8_t packMacCellStopReq(Pst *pst, MacCellStopInfo  *cellStopInfo);
uint8_t unpackMacCellStopReq(DuMacCellStopReq func, Pst *pst, Buffer *mBuf);
uint8_t MacProcCellStopReq(Pst *pst, MacCellStopInfo  *cellStopInfo);
uint8_t packMacCellCfg(Pst *pst, MacCellCfg *macCellCfg);
uint8_t unpackDuMacCellCfg(DuMacCellCfgReq func,  Pst *pst,  Buffer *mBuf);
uint8_t MacProcCellCfgReq(Pst *pst, MacCellCfg *macCellCfg);
uint8_t packMacCellCfgCfm(Pst *pst, MacCellCfgCfm *macCellCfgCfm);
uint8_t unpackMacCellCfgCfm(DuMacCellCfgCfm func, Pst *pst, Buffer *mBuf);
uint8_t duHandleMacCellCfgCfm(Pst *pst, MacCellCfgCfm *macCellCfgCfm);
uint8_t packMacStopInd(Pst *pst, MacCellStopInfo *cellId);
uint8_t unpackMacStopInd(DuMacStopInd func, Pst *pst, Buffer *mBuf);
uint8_t duHandleStopInd(Pst *pst, MacCellStopInfo *cellId);
uint8_t packMacUlCcchInd(Pst *pst, UlCcchIndInfo *ulCcchIndInfo);
uint8_t unpackMacUlCcchInd(DuMacUlCcchInd func, Pst *pst, Buffer *mBuf);
uint8_t duHandleUlCcchInd(Pst *pst, UlCcchIndInfo *ulCcchIndInfo);
uint8_t packMacDlCcchInd(Pst *pst, DlCcchIndInfo *dlCcchIndInfo);
uint8_t unpackMacDlCcchInd(DuMacDlCcchInd func, Pst *pst, Buffer *mBuf);
uint8_t MacProcDlCcchInd(Pst *pst, DlCcchIndInfo *dlCcchIndInfo);
uint8_t packDuMacUeCreateReq(Pst *pst, MacUeCfg *ueCfg);
uint8_t unpackMacUeCreateReq(DuMacUeCreateReq func, Pst *pst, Buffer *mBuf);
uint8_t MacProcUeCreateReq(Pst *pst, MacUeCfg *ueCfg);
uint8_t packDuMacUeCfgRsp(Pst *pst, MacUeCfgRsp *cfgRsp);
uint8_t unpackDuMacUeCfgRsp(MacDuUeCfgRspFunc func, Pst *pst, Buffer *mBuf);
uint8_t DuProcMacUeCfgRsp(Pst *pst, MacUeCfgRsp *cfgRsp);
uint8_t sendStopIndMacToDuApp(uint16_t cellId);
uint8_t packDuMacUeReconfigReq(Pst *pst, MacUeCfg *ueCfg);
uint8_t unpackMacUeReconfigReq(DuMacUeReconfigReq func, Pst *pst, Buffer *mBuf);
uint8_t MacProcUeReconfigReq(Pst *pst, MacUeCfg *ueCfg);
void fillDefUlLcCfg(UlLcCfg *ulLcCfg);
void fillDefMacCellGrpInfo(MacCellGrpCfg *cellGrp);
void fillDefPhyCellGrpInfo(PhyCellGrpCfg *cellGrp);
void fillDefSpCellGrpInfo(SpCellCfg *spCell);
void fillDefInitUlBwp(InitialUlBwp *initUlBwp);
void fillDefInitDlBwp(InitialDlBwp *initDlBwp);
#endif

/**********************************************************************
  End of file
 **********************************************************************/
