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

/* events */
#define EVENT_SCH_GEN_CFG            1
#define EVENT_SCH_CELL_CFG           2
#define EVENT_SCH_CELL_CFG_CFM       3
#define EVENT_DL_SCH_INFO            4 
#define EVENT_UL_SCH_INFO            5 
#define EVENT_RACH_IND_TO_SCH        6
#define EVENT_CRC_IND_TO_SCH         7
#define EVENT_DL_RLC_BO_INFO_TO_SCH  8
#define EVENT_ADD_UE_CONFIG_REQ_TO_SCH   9
#define EVENT_UE_CONFIG_RSP_TO_MAC   10
#define EVENT_SLOT_IND_TO_SCH        11
#define EVENT_SHORT_BSR              12
#define EVENT_UCI_IND_TO_SCH         13
#define EVENT_MODIFY_UE_CONFIG_REQ_TO_SCH 14
#define EVENT_UE_RECONFIG_RSP_TO_MAC 15
#define EVENT_UE_DELETE_REQ_TO_SCH   16
#define EVENT_UE_DELETE_RSP_TO_MAC   17
#define EVENT_CELL_DELETE_REQ_TO_SCH 18
#define EVENT_CELL_DELETE_RSP_TO_MAC 19
#define EVENT_LONG_BSR               20
#define EVENT_SLICE_CFG_REQ_TO_SCH   21
#define EVENT_SLICE_CFG_RSP_TO_MAC   22
#define EVENT_SLICE_RECFG_REQ_TO_SCH 23
#define EVENT_SLICE_RECFG_RSP_TO_MAC 24
#define EVENT_RACH_RESOURCE_REQUEST_TO_SCH  25
#define EVENT_RACH_RESOURCE_RESPONSE_TO_MAC 26
#define EVENT_RACH_RESOURCE_RELEASE_TO_SCH  27
#define EVENT_PAGING_IND_TO_SCH      28
#define EVENT_DL_PAGING_ALLOC        29
#define EVENT_DL_REL_HQ_PROC         30 
#define EVENT_DL_HARQ_IND_TO_SCH     31
#define EVENT_DL_CQI_TO_SCH          32
#define EVENT_UL_CQI_TO_SCH          33
#define EVENT_PHR_IND_TO_SCH         34
#define EVENT_STATISTICS_REQ_TO_SCH  35
#define EVENT_STATISTICS_RSP_TO_MAC  36
#define EVENT_STATISTICS_IND_TO_MAC  37
#define EVENT_STATISTICS_DELETE_REQ_TO_SCH  38
#define EVENT_STATISTICS_DELETE_RSP_TO_MAC  39
#define EVENT_STATISTICS_MODIFY_REQ_TO_SCH  40
#define EVENT_STATISTICS_MODIFY_RSP_TO_MAC  41

/*macros*/
#define MAX_SSB_IDX 1 /* forcing it as 1 for now. Right value is 64 */
#define SCH_SSB_MASK_SIZE   1

#define MAX_NUM_PRG     1 /* max value should be later 275 */
#define MAX_DIG_BF_INTERFACES 0 /* max value should be later 255 */
#define MAX_CODEWORDS  1  /* max should be 2 */
#define SCH_HARQ_PROC_ID 1 /* harq proc id */
#define SCH_ALLOC_TYPE_1 1 /*sch res alloc type */

/* Datatype in UL SCH Info */
#define SCH_DATATYPE_PUSCH 1
#define SCH_DATATYPE_PUSCH_UCI 2
#define SCH_DATATYPE_UCI 4
#define SCH_DATATYPE_SRS 8
#define SCH_DATATYPE_PRACH 16

#define MAX_NUMBER_OF_CRC_IND_BITS 1
#define MAX_NUMBER_OF_UCI_IND_BITS 1
#define MAX_SR_BITS_IN_BYTES       1
#define MAX_HARQ_BITS_IN_BYTES     1
#define MAX_NUM_LOGICAL_CHANNEL_GROUPS 8
#define MAX_NUM_SR_CFG_PER_CELL_GRP 8   /* Max number of scheduling request config per cell group */
#define MAX_NUM_TAGS 4                  /* Max number of timing advance groups */
#define MAX_NUM_BWP  4                  /* Max number of BWP per serving cell */
#define MAX_NUM_CRSET  3                /* Max number of control resource set in add/modify/release list */
#define MAX_NUM_SEARCH_SPC  10          /* Max number of search space in add/modify/release list */
#define FREQ_DOM_RSRC_SIZE  6           /* i.e. 6 bytes because Size of frequency domain resource is 45 bits */
#define MONITORING_SYMB_WITHIN_SLOT_SIZE 2  /* i.e. 2 bytes because size of monitoring symbols within slot is 14 bits */
#define MAX_NUM_DL_ALLOC 16             /* Max number of pdsch time domain downlink allocation */
#define MAX_NUM_UL_ALLOC 16             /* Max number of pusch time domain uplink allocation */

/* PUCCH Configuration Macro */
#define MAX_NUM_PUCCH_RESRC 128
#define MAX_NUM_PUCCH_RESRC_SET 4
#define MAX_NUM_PUCCH_PER_RESRC_SET 32
#define MAX_NUM_SPATIAL_RELATIONS 8
#define MAX_NUM_PUCCH_P0_PER_SET 8
#define MAX_NUM_PATH_LOSS_REF_RS 4
#define MAX_NUM_DL_DATA_TO_UL_ACK 15
#define QPSK_MODULATION 2

#define RAR_PAYLOAD_SIZE 10             /* As per spec 38.321, sections 6.1.5 and 6.2.3, RAR PDU is 8 bytes long and 2 bytes of padding */
#define TX_PAYLOAD_HDR_LEN 32           /* Intel L1 requires adding a 32 byte header to transmitted payload */
#define UL_TX_BUFFER_SIZE 5

#define MAX_NUM_CONFIG_SLOTS 160  /*Max number of slots as per the numerology*/
#define MAX_NUM_K0_IDX 16 /* Max number of pdsch time domain downlink allocation */
#define MAX_NUM_K1_IDX 8  /* As per spec 38.213 section 9.2.3 Max number of PDSCH-to-HARQ resource indication */
#define MIN_NUM_K1_IDX 4  /* Min K1 values */
#define MAX_NUM_K2_IDX 16 /* PUSCH time domain UL resource allocation list */
#define DEFAULT_K0_VALUE 0 /* As per 38.331, PDSCH-TimeDomainResourceAllocation field descriptions */
/* As per 38.331, PUSCH-TimeDomainResourceAllocationList field descriptions */
#define DEFAULT_K2_VALUE_FOR_SCS15  1 
#define DEFAULT_K2_VALUE_FOR_SCS30  1 
#define DEFAULT_K2_VALUE_FOR_SCS60  2
#define DEFAULT_K2_VALUE_FOR_SCS120 3 

#define MAX_PLMN 2
#define DL_DMRS_SYMBOL_POS 4 /* Bitmap value 00000000000100 i.e. using 3rd symbol for PDSCH DMRS */

#define MAX_PHR_REPORT 1 /*TODO: Range of PHR reports in multiple PHR.*/
#define MAX_FAILURE_DET_RESOURCES 10 /*Spec 38.331 'maxNrofFailureDetectionResources'*/

/*As per SCF222_5GFAPI, 'MaxDciPerSlot' defines this value but this parameter value is missing in Spec.*/
#ifdef INTEL_FAPI
   #define MAX_NUM_PDCCH 1
#else
   #define MAX_NUM_PDCCH 2 
#endif

#define ADD_DELTA_TO_TIME(crntTime, toFill, incr, numOfSlot)          \
{                                                          \
   if ((crntTime.slot + incr) > (numOfSlot - 1))           \
   {                                                       \
      toFill.sfn = (crntTime.sfn + 1);                     \
   }                                                       \
   else                                                    \
   {                                                       \
      toFill.sfn = crntTime.sfn;                           \
   }                                                       \
   toFill.slot = (crntTime.slot + incr) % numOfSlot;       \
   if (toFill.sfn >= MAX_SFN)                              \
   {                                                       \
      toFill.sfn%=MAX_SFN;                                 \
   }                                                       \
}

typedef enum
{
   PRB_RSRC,
   DRB_RSRC,
   RRC_CONNECTED_USERS_RSRC
}SchResourceType;

typedef enum
{
   NO_TRANSMISSION,
   NEW_TRANSMISSION,
   REPEATITION 
}PduTxOccsaion;

typedef enum
{
   SR_PROHIBIT_MS1,
   SR_PROHIBIT_MS2,
   SR_PROHIBIT_MS4,
   SR_PROHIBIT_MS8,
   SR_PROHIBIT_MS16,
   SR_PROHIBIT_MS32,
   SR_PROHIBIT_MS64,
   SR_PROHIBIT_MS128
}SchSrProhibitTimer;

typedef enum
{
   SR_TRANS_MAX_N4,
   SR_TRANS_MAX_N8,
   SR_TRANS_MAX_N16,
   SR_TRANS_MAX_N32,
   SR_TRANS_MAX_N64,
   SR_TRANS_MAX_SPARE3,
   SR_TRANS_MAX_SPARE2,
   SR_TRANS_MAX_SPARE1
}SchSrTransMax;

typedef enum
{
   TIME_ALIGNMENT_TIMER_MS500,
   TIME_ALIGNMENT_TIMER_MS750,
   TIME_ALIGNMENT_TIMER_MS1280,
   TIME_ALIGNMENT_TIMER_MS1920,
   TIME_ALIGNMENT_TIMER_MS2560,
   TIME_ALIGNMENT_TIMER_MS5120,
   TIME_ALIGNMENT_TIMER_MS10240,
   TIME_ALIGNMENT_TIMER_INFINITE
}SchTimeAlignmentTimer;

typedef enum
{
   PHR_PERIODIC_TIMER_SF10,
   PHR_PERIODIC_TIMER_SF20,
   PHR_PERIODIC_TIMER_SF50,
   PHR_PERIODIC_TIMER_SF100,
   PHR_PERIODIC_TIMER_SF200,
   PHR_PERIODIC_TIMER_SF500,
   PHR_PERIODIC_TIMER_SF1000,
   PHR_PERIODIC_TIMER_INFINITE
}SchPhrPeriodicTimer;

typedef enum
{
   PHR_PROHIBIT_TIMER_SF0,
   PHR_PROHIBIT_TIMER_SF10,
   PHR_PROHIBIT_TIMER_SF20,
   PHR_PROHIBIT_TIMER_SF50,
   PHR_PROHIBIT_TIMER_SF100,
   PHR_PROHIBIT_TIMER_SF200,
   PHR_PROHIBIT_TIMER_SF500,
   PHR_PROHIBIT_TIMER_SF1000
}SchPhrProhibitTimer;

typedef enum
{
   PHR_TX_PWR_FACTOR_CHANGE_DB1,
   PHR_TX_PWR_FACTOR_CHANGE_DB3,
   PHR_TX_PWR_FACTOR_CHANGE_DB6,
   PHR_TX_PWR_FACTOR_CHANGE_INFINITE
}SchPhrTxPwrFactorChange;

typedef enum
{
   PHR_MODE_REAL,
   PHR_MODE_VIRTUAL
}SchPhrModeOtherCG;

typedef enum
{
   HARQ_ACK_CODEBOOK_SEMISTATIC,
   HARQ_ACK_CODEBOOK_DYNAMIC
}SchPdschHarqAckCodebook;

typedef enum
{
   NUM_HARQ_PROC_FOR_PDSCH_N2,
   NUM_HARQ_PROC_FOR_PDSCH_N4,
   NUM_HARQ_PROC_FOR_PDSCH_N6,
   NUM_HARQ_PROC_FOR_PDSCH_N10,
   NUM_HARQ_PROC_FOR_PDSCH_N16
}SchNumHarqProcForPdsch;

typedef enum
{
   MAX_CODE_BLOCK_GROUP_PER_TB_N2,
   MAX_CODE_BLOCK_GROUP_PER_TB_N4,
   MAX_CODE_BLOCK_GROUP_PER_TB_N6,
   MAX_CODE_BLOCK_GROUP_PER_TB_N8
}SchMaxCodeBlkGrpPerTB;

typedef enum
{
   PDSCH_X_OVERHEAD_XOH_6,
   PDSCH_X_OVERHEAD_XOH_12,
   PDSCH_X_OVERHEAD_XOH_18
}SchPdschXOverhead;

typedef enum
{
   DMRS_ADDITIONAL_POS0,
   DMRS_ADDITIONAL_POS1,
   DMRS_ADDITIONAL_POS3
}SchDmrsAdditionPosition;

typedef enum
{
   RESOURCE_ALLOCTION_TYPE_0,
   RESOURCE_ALLOCTION_TYPE_1,
   RESOURCE_ALLOCTION_DYN_SWITCH
}SchResourceAllocType;

typedef enum
{
   TIME_DOMAIN_RSRC_ALLOC_MAPPING_TYPE_A,
   TIME_DOMAIN_RSRC_ALLOC_MAPPING_TYPE_B
}SchTimeDomRsrcAllocMappingType;

typedef enum
{
   ENABLED_TRANSFORM_PRECODER,
   DISABLED_TRANSFORM_PRECODER
}SchTransformPrecoder;

typedef enum
{
   INTERLEAVED_CCE_REG_MAPPING = 1,
   NONINTERLEAVED_CCE_REG_MAPPING
}SchREGMappingType;

typedef enum
{
   SLOT_PERIODICITY_SL_1 = 1,
   SLOT_PERIODICITY_SL_2,
   SLOT_PERIODICITY_SL_4,
   SLOT_PERIODICITY_SL_5,
   SLOT_PERIODICITY_SL_8,
   SLOT_PERIODICITY_SL_10,
   SLOT_PERIODICITY_SL_16,
   SLOT_PERIODICITY_SL_20,
   SLOT_PERIODICITY_SL_40,
   SLOT_PERIODICITY_SL_80,
   SLOT_PERIODICITY_SL_160,
   SLOT_PERIODICITY_SL_320,
   SLOT_PERIODICITY_SL_640,
   SLOT_PERIODICITY_SL_1280,
   SLOT_PERIODICITY_SL_2560
}SchMSlotPeriodicity;

typedef enum
{
   SAME_AS_REG_BUNDLE,
   ALL_CONTIGUOUS_RB
}SchPrecoderGranul;

typedef enum
{
   SEARCH_SPACE_TYPE_COMMON = 1,
   SEARCH_SPACE_TYPE_UE_SPECIFIC
}SchSearchSpaceType;

typedef enum
{
   SCH_QOS_NON_DYNAMIC = 1,
   SCH_QOS_DYNAMIC
}SchQosType;

typedef enum
{
   AGGREGATION_LEVEL_N0,
   AGGREGATION_LEVEL_N1,
   AGGREGATION_LEVEL_N2,
   AGGREGATION_LEVEL_N3,
   AGGREGATION_LEVEL_N4,
   AGGREGATION_LEVEL_N5,
   AGGREGATION_LEVEL_N6,
   AGGREGATION_LEVEL_N8
}SchAggrLevel;

typedef enum
{
   RBG_SIZE_CONFIG_1,
   RBG_SIZE_CONFIG_2
}SchRBGSize;

typedef enum
{
   CODE_WORDS_SCHED_BY_DCI_N1,
   CODE_WORDS_SCHED_BY_DCI_N2
}SchCodeWordsSchedByDci;

typedef enum
{
   STATIC_BUNDLING_TYPE = 1,
   DYNAMIC_BUNDLING_TYPE
}SchBundlingType;

typedef enum
{
   SCH_SET1_SIZE_N4,
   SCH_SET1_SIZE_WIDEBAND,
   SCH_SET1_SIZE_N2_WIDEBAND,
   SCH_SET1_SIZE_N4_WIDEBAND
}SchBundlingSizeSet1;

typedef enum
{
   SCH_SET2_SIZE_N4,
   SCH_SET2_SIZE_WIDEBAND
}SchBundlingSizeSet2;

typedef enum
{
   DUPLEX_MODE_FDD,
   DUPLEX_MODE_TDD
}SchDuplexMode;

typedef enum
{
   SSB_PRDCTY_MS5,
   SSB_PRDCTY_MS10,
   SSB_PRDCTY_MS20,
   SSB_PRDCTY_MS40,
   SSB_PRDCTY_MS80,
   SSB_PRDCTY_MS160
}SchSSBPeriod;

typedef enum
{
   RSP_OK,
   RSP_NOK
}SchMacRsp;

typedef enum
{
   SHORT_BSR,
   LONG_BSR,
   SHORT_TRUNCATED_BSR,
   LONG_TRUNCATED_BSR
}BsrType;

typedef enum
{
   SINGLE_ENTRY_PHR,
   MULTIPLE_ENTRY_PHR
}PhrType;

typedef enum
{
  PH_TYPE_1,
  PH_TYPE_2
}PhType;

typedef enum
{
   FORMAT0_0,
   FORMAT0_1,
   FORMAT1_0,
   FORMAT1_1
}FormatType;

typedef enum
{
   SCH_MCS_TABLE_QAM_64,
   SCH_MCS_TABLE_QAM_256,
   SCH_MCS_TABLE_QAM_64_LOW_SE
}SchMcsTable;

typedef enum
{
   NONE,
   PDCCH_PDU,
   PDSCH_PDU,
   BOTH
}DlPduType;

typedef enum
{
   DATA_TRANSMISSION_ALLOWED,
   STOP_DATA_TRANSMISSION,
   RESTART_DATA_TRANSMISSION
}SchDataTransmission;

typedef enum
{
   SchBeamFailure,
   SchRlf,
   SchBoth
}SchPurposeOfFailureDet;

typedef enum
{
   CQI_PUCCH = 1,
   CQI_PUSCH
}CqiUlReportType;

/* Performance measurements from 3GPP TS 28.552 Release 15 */
typedef enum
{
   SCH_DL_TOTAL_PRB_USAGE,
   SCH_UL_TOTAL_PRB_USAGE
}SchMeasurementType;

/*structures*/
typedef struct timeDomainAlloc
{
   uint16_t startSymb;
   uint16_t numSymb;
}TimeDomainAlloc;

typedef struct resAllocType0 
{
   uint8_t rbBitmap[36];
}ResAllocType0;

typedef struct resAllocType1 
{
   uint16_t startPrb;
   uint16_t numPrb;
}ResAllocType1;

typedef struct resAllocType1 FreqDomainRsrc;

/* Reference -> O-RAN.WG8.AAD.0-v07.00, Table 9-32 BWP Information  */
typedef struct bwpCfg
{
   uint8_t         subcarrierSpacing;
   uint8_t         cyclicPrefix;
   FreqDomainRsrc  freqAlloc;	
}BwpCfg;

typedef struct prg
{
   uint16_t pmIdx;
   uint16_t beamIdx[MAX_DIG_BF_INTERFACES];
} Prg;

typedef struct beamformingInfo
{
   uint16_t numPrgs;
   uint16_t prgSize;
   uint8_t  digBfInterfaces;
   Prg  prg[MAX_NUM_PRG];
} BeamformingInfo;

/* SIB1 PDSCH structures */

typedef struct codewordinfo
{
   uint16_t targetCodeRate;
   uint8_t  qamModOrder;
   uint8_t  mcsIndex;
   uint8_t  mcsTable;
   uint8_t  rvIndex;
   uint32_t tbSize;
} CodewordInfo;

typedef struct dmrsInfo
{
   uint16_t dlDmrsSymbPos;
   uint8_t  dmrsConfigType;
   uint16_t dlDmrsScramblingId;
   uint8_t  scid;
   uint8_t  numDmrsCdmGrpsNoData;
   uint16_t dmrsPorts;
   uint8_t mappingType;
   uint8_t nrOfDmrsSymbols;
   uint8_t dmrsAddPos;
} DmrsInfo;

typedef struct pdschFreqAlloc
{
   uint8_t  resourceAllocType;
   /* since we are using type-1, rbBitmap excluded */
   uint8_t  rbBitmap[36];
   uint16_t startPrb;
   uint16_t numPrb;
   uint8_t  vrbPrbMapping;
}PdschFreqAlloc;

typedef struct pdschTimeAlloc
{
   uint8_t  rowIndex; 
   uint16_t startSymb;
   uint16_t numSymb;
} PdschTimeAlloc;

typedef struct txPowerPdschInfo
{
   uint8_t powerControlOffset;
   uint8_t powerControlOffsetSS;
} TxPowerPdschInfo;

/* Reference -> O-RAN.WG8.AAD.0-v07.00, Table 9-43 PDSCH Configuration */
typedef struct pdschCfg
{
   uint16_t         pduBitmap;
   uint16_t         rnti;
   uint16_t         pduIndex;
   uint8_t          numCodewords;
   CodewordInfo     codeword[MAX_CODEWORDS];
   uint16_t         dataScramblingId;
   uint8_t          numLayers;
   uint8_t          transmissionScheme;
   uint8_t          refPoint;
   DmrsInfo         dmrs;
   PdschFreqAlloc   pdschFreqAlloc;
   PdschTimeAlloc   pdschTimeAlloc;
   BeamformingInfo  beamPdschInfo;
   TxPowerPdschInfo txPdschPower;
} PdschCfg;
/* SIB1 PDSCH structures end */

/* SIB1 interface structure */

/* Reference -> O-RAN.WG8.AAD.0-v07.00, Table 9-35 CORESET Configuration */
typedef struct coresetCfg
{
   uint8_t coreSetSize;
   uint8_t startSymbolIndex;
   uint8_t durationSymbols;
   uint8_t freqDomainResource[6];
   uint8_t cceRegMappingType;
   uint8_t regBundleSize;
   uint8_t interleaverSize;
   uint16_t shiftIndex;
   uint8_t coreSetType;
   uint8_t coresetPoolIndex;
   uint8_t precoderGranularity;
   uint8_t cceIndex;
   uint8_t aggregationLevel;
} CoresetCfg;

typedef struct txPowerPdcchInfo
{
   uint8_t beta_pdcch_1_0;
   uint8_t powerControlOffsetSS;
}TxPowerPdcchInfo;

/* Reference -> O-RAN.WG8.AAD.0-v07.00, Table 9-42 DL-DCI Configuration */
typedef struct dlDCI
{
   uint16_t rnti;
   uint16_t scramblingId;
   uint16_t scramblingRnti;
   uint8_t cceIndex;
   uint8_t aggregLevel;
   BeamformingInfo beamPdcchInfo;
   TxPowerPdcchInfo txPdcchPower;
   PdschCfg     pdschCfg;
}DlDCI;

typedef struct pdcchCfg
{
   /* coreset-0 configuration */
   CoresetCfg coresetCfg;
   uint16_t   numDlDci;
   DlDCI      dci[MAX_NUM_PDCCH]; 
} PdcchCfg;
/* end of SIB1 PDCCH structures */

typedef struct schPcchCfg
{
   uint8_t  numPO;                    /*Derived from Ns*/
   bool     poPresent;                /*FirstPDCCH-MonitoringPO is present or not*/
   uint16_t pagingOcc[MAX_PO_PER_PF]; /*FirstPDCCH-Monitoring Paging Occasion*/
}SchPcchCfg;

typedef struct schPdcchConfigSib1
{
   uint8_t  coresetZeroIndex;     /* derived from 4 LSB of pdcchSib1 present in MIB */
   uint8_t  searchSpaceZeroIndex; /* derived from 4 MSB of pdcchSib1 present in MIB */
}SchPdcchConfigSib1;

typedef struct schRachCfgGeneric
{
   uint8_t      prachCfgIdx;       /* PRACH config idx */
   uint8_t      msg1Fdm;           /* PRACH FDM (1,2,4,8) */
   uint16_t     msg1FreqStart;     /* Msg1-FrequencyStart */
   uint8_t      zeroCorrZoneCfg;   /* Zero correlation zone cofig */
   int16_t      preambleRcvdTargetPower;
   uint8_t      preambleTransMax;
   uint8_t      pwrRampingStep;
   uint8_t      raRspWindow;       /* RA Response Window */
}SchRachCfgGeneric;

typedef struct schRachCfg
{
   SchRachCfgGeneric  prachCfgGeneric;
   uint8_t      totalNumRaPreamble; /* Total number of RA preambles */
   uint8_t      ssbPerRach;        /* SSB per RACH occassion */
   uint8_t      numCbPreamblePerSsb; /* Number of CB preamble per SSB */
   uint8_t      raContResTmr;      /* RA Contention Resoultion Timer */
   uint8_t      rsrpThreshSsb;     /* RSRP Threshold SSB */
   uint16_t     rootSeqIdx;        /* Root sequence index */
   uint16_t     rootSeqLen;        /* root sequence length */
   uint8_t      numRootSeq;        /* Number of root sequences required for FD */
   uint8_t      msg1SubcSpacing;  /* Subcarrier spacing of RACH */
}SchRachCfg;

typedef struct schBwpParams
{
   FreqDomainRsrc  freqAlloc;
   uint8_t         scs;
   uint8_t         cyclicPrefix;
}SchBwpParams;

typedef struct schCandidatesInfo
{
   uint8_t aggLevel1;
   uint8_t aggLevel2;
   uint8_t aggLevel4;
   uint8_t aggLevel8;
   uint8_t aggLevel16;
}SchCandidatesInfo;

typedef struct schSearchSpaceCfg
{
   uint8_t  searchSpaceId;
   uint8_t  coresetId;
   uint8_t  freqDomainRsrc[FREQ_DOM_RSRC_SIZE];  /* Frequency domain resource */
   uint16_t monitoringSlot;
   uint16_t duration;
   uint16_t monitoringSymbol;
   SchCandidatesInfo candidate;
}SchSearchSpaceCfg;

typedef struct schPdcchCfgCmn
{
   SchSearchSpaceCfg commonSearchSpace;
   uint8_t raSearchSpaceId;
}SchPdcchCfgCmn;

typedef struct schPdschCfgCmnTimeDomRsrcAlloc
{
   uint8_t k0;
   uint8_t mappingType;
   uint8_t startSymbol;
   uint8_t lengthSymbol;
}SchPdschCfgCmnTimeDomRsrcAlloc;

typedef struct schPdschCfgCmn
{
   uint8_t   numTimeDomAlloc;
   SchPdschCfgCmnTimeDomRsrcAlloc timeDomRsrcAllocList[MAX_NUM_DL_ALLOC];
}SchPdschCfgCmn;

typedef struct schPucchCfgCmn
{
   uint8_t pucchResourceCommon;
   uint8_t pucchGroupHopping;
}SchPucchCfgCmn;

/* PUSCH Time Domain Resource Allocation */
typedef struct schPuschTimeDomRsrcAlloc
{
   uint8_t   k2;
   SchTimeDomRsrcAllocMappingType   mappingType;
   uint8_t   startSymbol;
   uint8_t   symbolLength;
}SchPuschTimeDomRsrcAlloc;

typedef struct schPuschCfgCmn
{
   uint8_t                    numTimeDomRsrcAlloc;
   SchPuschTimeDomRsrcAlloc   timeDomRsrcAllocList[MAX_NUM_UL_ALLOC]; /* PUSCH time domain UL resource allocation list */
}SchPuschCfgCmn;

typedef struct schK1TimingInfo
{
   uint8_t numK1;
   uint8_t k1Indexes[MAX_NUM_K1_IDX];
}SchK1TimingInfo;

typedef struct schK0TimingInfo
{
   uint8_t k0Index;
   SchK1TimingInfo k1TimingInfo;
}SchK0TimingInfo;

typedef struct schK0K1TimingInfo
{
   uint8_t numK0;
   SchK0TimingInfo k0Indexes[MAX_NUM_K0_IDX];
}SchK0K1TimingInfo;

typedef struct schK0K1TimingInfoTbl
{
   uint16_t tblSize;
   SchK0K1TimingInfo k0k1TimingInfo[MAX_NUM_CONFIG_SLOTS];
}SchK0K1TimingInfoTbl;

typedef struct schBwpDlCfg
{
   SchBwpParams   bwp;
   SchPdcchCfgCmn pdcchCommon;
   SchPdschCfgCmn pdschCommon;
}SchBwpDlCfg;

typedef struct schK2TimingInfo
{
   uint8_t numK2; 
   uint8_t k2Indexes[MAX_NUM_K2_IDX]; 
}SchK2TimingInfo;

typedef struct schK2TimingInfoTbl
{
   uint16_t tblSize; 
   SchK2TimingInfo k2TimingInfo[MAX_NUM_CONFIG_SLOTS]; 
}SchK2TimingInfoTbl;

typedef struct schBwpUlCfg
{
   SchBwpParams   bwp;
   SchRachCfg     schRachCfg;       /* PRACH config */
   SchPucchCfgCmn pucchCommon;
   SchPuschCfgCmn puschCommon;
}SchBwpUlCfg;

typedef struct schPlmnInfoList
{
   Plmn           plmn;
   SupportedSliceList suppSliceList;
}SchPlmnInfoList;

#ifdef NR_DRX
/* The following list of structures is taken from the DRX-Config section of specification 33.331. */

typedef struct schDrxOnDurationTimer
{
   bool     onDurationTimerValInMs;
   union
   {
      uint8_t  subMilliSeconds;
      uint16_t milliSeconds;
   }onDurationtimerValue;
}SchDrxOnDurationTimer;

typedef struct schDrxLongCycleStartOffset
{
   uint16_t drxLongCycleStartOffsetChoice;
   uint16_t drxLongCycleStartOffsetVal;
}SchDrxLongCycleStartOffset;

typedef struct schShortDrx
{
   uint16_t   drxShortCycle;
   uint8_t    drxShortCycleTimer;
}SchShortDrx;

typedef struct schDrxCfg
{
   SchDrxOnDurationTimer       drxOnDurationTimer;
   uint16_t                    drxInactivityTimer;
   uint8_t                     drxHarqRttTimerDl;
   uint8_t                     drxHarqRttTimerUl;
   uint16_t                    drxRetransmissionTimerDl;
   uint16_t                    drxRetransmissionTimerUl;
   SchDrxLongCycleStartOffset  drxLongCycleStartOffset;
   bool                        shortDrxPres;
   SchShortDrx                 shortDrx;
   uint8_t                     drxSlotOffset;
}SchDrxCfg;
#endif

/*Spec 38.331 'NrNsPmaxList'*/
typedef struct schNrNsPmaxList
{
   long additionalPMax;
   long additionalSpectrumEmission;
}SchNrNsPmaxList;

/*Spec 38.331 'FrequencyInfoDL-SIB'*/
typedef struct schMultiFreqBandListSib
{
   long freqBandIndNr;
   SchNrNsPmaxList nrNsPmaxList[1];
}SchMultiFreqBandListSib;

/*Spec 38.331 'SCS-SpecificCarrier'*/
typedef struct schScsSpecificCarrier
{
  uint16_t offsetToCarrier;
  uint8_t  subCarrierSpacing;
  uint16_t carrierBw;
  uint16_t txDirectCurrentLoc;
}SchScsSpecificCarrier;

/*Spec 38.331 'FrequencyInfoDL-SIB'*/
typedef struct schFreqInfoDlSib
{
   SchMultiFreqBandListSib mutiFreqBandList[1];
   uint16_t                offsetToPointA;
   SchScsSpecificCarrier   schSpcCarrier[1];
}SchFreqInfoDlSib;

typedef struct schBcchCfg
{
   long modPeriodCoeff;
}SchBcchCfg;

/*Spec 38.331 'DownlinkConfigCommonSIB'*/
typedef struct schDlCfgCommon
{
   SchFreqInfoDlSib  schFreqInfoDlSib;
   SchBwpDlCfg       schInitialDlBwp;  /* Initial DL BWP */
   SchBcchCfg        schBcchCfg;
   SchPcchCfg        schPcchCfg;
}SchDlCfgCommon;

/*Spec 38.331 'FrequencyInfoUL-SIB'*/
typedef struct schFreqInfoUlSib
{
   SchMultiFreqBandListSib mutiFreqBandList[1];
   uint16_t                absoluteFreqPointA;
   SchScsSpecificCarrier   schSpcCarrier[1];
   int8_t                  schPMax;
   bool                    frequencyShift7p5khz;
}SchFreqInfoUlSib;

/*Spec 38.331 'UplinkConfigCommonSIB '*/
typedef struct schUlCfgCommon
{
   SchFreqInfoUlSib  schFreqInfoUlSib;
   SchBwpUlCfg       schInitialUlBwp;  /* Initial DL BWP */
   uint16_t          schTimeAlignTimer;
}SchUlCfgCommon;

/*Ref: ORAN_WG8.V7.0.0 Sec 11.2.4.2.1 Cell Conf Request*/
typedef struct schCellCfg
{
   uint16_t        cellId;                 /* Cell Id */
   uint8_t         numOfBeams;
   uint8_t         numLayers;
   uint8_t         numAntPorts;
   uint16_t        phyCellId;               /* Physical cell id */
   SchPlmnInfoList plmnInfoList[MAX_PLMN];  /* Consits of PlmnId and Snssai list */
   SchDuplexMode   dupMode;                 /* Duplex type: TDD/FDD */
   uint8_t         dlBandwidth;             /* Supported B/W */
   uint8_t         ulBandwidth;             /* Supported B/W */
   SchDlCfgCommon  dlCfgCommon;             /*Spec 38.331 DownlinkConfigCommonSIB*/
   SchUlCfgCommon  ulCfgCommon;             /*Spec 38.331 UplinkConfigCommonSIB*/
#ifdef NR_TDD
   TDDCfg          tddCfg;                  /* Spec 38.331 tdd-UL-DL-ConfigurationCommon */ 
#endif 

  /*Ref:Spec 38.331 "ssb-PositionsInBurst", Value 0 in Bitmap => corresponding SS/PBCH block is not transmitted
    *value 1 => corresponding SS/PBCH block is transmitted*/
   uint32_t            ssbPosInBurst[SCH_SSB_MASK_SIZE];  /* Bitmap for actually transmitted SSB. */
   SchSSBPeriod        ssbPeriod;        /* SSB Periodicity in msec */
   uint32_t            ssbFrequency;     /* SB frequency in kHz*/
   uint8_t             dmrsTypeAPos;
   uint8_t             ssbScs;           /* SSB subcarrier spacing*/
   SchPdcchConfigSib1  pdcchCfgSib1;     /* Req to configure CORESET#0 and SearchSpace#0*/
   uint32_t            ssbPbchPwr;       /* SSB block power */
   uint8_t             ssbSubcOffset;    /* Subcarrier Offset(Kssb) */
   uint16_t            sib1PduLen;
}SchCellCfg;

/*Ref: ORAN_WG8.V7.0.0 Sec 11.2.4.3.1 Cell Config Response*/
typedef struct schCellCfgCfm
{
   uint16_t         cellId;     /* Cell Id */
   SchMacRsp        rsp;   
   CauseOfResult    cause;
}SchCellCfgCfm;

/*Ref: ORAN_WG8.V7.0.0 Sec 11.2.4.2.2 Cell Del Req*/
typedef struct schCellDeleteReq 
{
   uint16_t   cellId;
}SchCellDeleteReq;

/*Ref: ORAN_WG8.V7.0.0 Sec 11.2.4.3.2 Cell Del Response*/
typedef struct schCellDeleteRsp
{
   uint16_t        cellId;
   SchMacRsp       rsp;
   CauseOfResult   cause;
}SchCellDeleteRsp;

/*Ref: ORAN_WG8.V7.0.0 Sec 11.2.4.2.3*/
typedef struct schRrmPolicyRatio
{
   uint8_t maxRatio;
   uint8_t minRatio;
   uint8_t dedicatedRatio;
}SchRrmPolicyRatio;

typedef struct schRrmPolicyOfSlice
{
   Snssai  snssai;
   SchRrmPolicyRatio rrmPolicyRatioInfo;
}SchRrmPolicyOfSlice;

typedef struct schSliceCfgReq
{
   uint8_t  numOfConfiguredSlice;
   SchRrmPolicyOfSlice **listOfSlices;
}SchSliceCfgReq;

/*Ref: ORAN_WG8.V7.0.0 Sec 11.2.4.3.3 Slice Cfg Response*/
typedef struct schSliceCfgRsp
{
   Snssai     snssai;
   SchMacRsp  rsp;
   CauseOfResult cause;
}SchSliceCfgRsp;

/*As per ORAN-WG8 V7.0.0 Sec 11.2.4.3.4 , Slice Cfg and Recfg are same structures*/
typedef struct schSliceCfgReq SchSliceRecfgReq;
typedef struct schSliceCfgRsp SchSliceRecfgRsp;

typedef struct ssbInfo
{
   uint8_t         ssbIdx;          /* SSB Index */
   TimeDomainAlloc tdAlloc;         /* Time domain allocation */
   FreqDomainRsrc  fdAlloc;         /* Freq domain allocation */
}SsbInfo;

typedef struct sib1AllocInfo
{
   BwpCfg bwp;
   PdcchCfg *sib1PdcchCfg;
}Sib1AllocInfo;

typedef struct prachSchInfo
{
   uint8_t  numPrachOcas;   /* Num Prach Ocassions */
   uint8_t  prachFormat;    /* PRACH Format */
   uint8_t  numRa;          /* Freq domain ocassion */
   uint8_t  prachStartSymb; /* Freq domain ocassion */
}SchPrachInfo;

/* Interface structure signifying DL broadcast allocation for SSB, SIB1 */
typedef struct dlBrdcstAlloc
{
   uint16_t     crnti; /* SI-RNTI */
   /* Ssb transmission is determined as follows:
    * 0 : No tranamission
    * 1 : SSB Transmission
    * 2 : SSB Repetition */
   uint8_t ssbTransmissionMode;
   uint8_t ssbIdxSupported;
   SsbInfo ssbInfo[MAX_SSB_IDX];
   bool    systemInfoIndicator;
   uint8_t *siContent; 
   /* Sib1 transmission is determined as follows:
    * 0 : No tranamission
    * 1 : SIB1 Transmission
    * 2 : SIB1 Repetition */
   uint8_t sib1TransmissionMode;
   Sib1AllocInfo sib1Alloc;
}DlBrdcstAlloc;

typedef struct msg3UlGrant
{
   uint8_t         freqHopFlag;
   uint16_t        bwpSize;
   FreqDomainRsrc  msg3FreqAlloc;
   uint8_t         k2Index;
   uint8_t         mcs;
   uint8_t         tpc;
   uint8_t         csiReq;
}Msg3UlGrant;

typedef struct rarInfo
{
   uint8_t         RAPID;
   uint16_t        ta;
   Msg3UlGrant     ulGrant;
   uint16_t        tcrnti;
   uint8_t         rarPdu[RAR_PAYLOAD_SIZE];
   uint8_t         rarPduLen;
}RarInfo;

typedef struct rarAlloc
{
   uint16_t   raRnti;
   RarInfo    rarInfo;
   BwpCfg     bwp;
   PdcchCfg   *rarPdcchCfg;
   PdschCfg   *rarPdschCfg;
}RarAlloc;

typedef struct lcSchInfo
{
   uint8_t   lcId;
   uint32_t  schBytes; 
}LcSchInfo;

typedef struct ceSchInfo
{
   uint8_t   ceLcId;
   uint8_t   *ceContent; 
}CeSchInfo;

typedef struct freqDomainAlloc
{
   uint8_t          resAllocType; /* Resource allocation type */
   union
   {
      ResAllocType0    type0;
      ResAllocType1    type1;
   }resAlloc;
}FreqDomainAlloc;

typedef struct transportBlock
{
   uint8_t    mcs;       
   bool       ndi;       
   uint8_t    rv;        
   uint16_t   tbSize;    
   uint8_t    numCe;
   CeSchInfo  ceSchInfo[MAX_NUM_LC]; 
   uint8_t    numLc;
   LcSchInfo  lcSchInfo[MAX_NUM_LC]; 
}TransportBlock;

typedef struct dlMsgSchedInfo
{
   uint16_t        crnti;
   uint8_t         dciFormatId;
   uint8_t         harqProcNum;
   bool            vrbPrbMapping;
   uint8_t         dlAssignIdx;
   uint8_t         pucchTpc;
   uint8_t         pucchResInd;
   uint8_t         harqFeedbackInd;
   uint16_t        dlMsgPduLen;
   uint8_t         *dlMsgPdu;
   FreqDomainAlloc freqAlloc;
   TimeDomainAlloc timeAlloc;
   uint8_t         numOfTbs;
   TransportBlock  transportBlock[2];
   BwpCfg          bwp;
   PdcchCfg        *dlMsgPdcchCfg;
   PdschCfg        *dlMsgPdschCfg;
}DlMsgSchInfo;

typedef struct schSlotValue
{
   SlotTimingInfo currentTime;
   SlotTimingInfo broadcastTime;
   SlotTimingInfo rarTime;
   SlotTimingInfo dlMsgTime;
   SlotTimingInfo ulDciTime;
}SchSlotValue;


/* Reference -> O-RAN.WG8.AAD.0-v07.00, Table 9-36 DCI Format0_0 Configuration */
typedef struct format0_0
{
   uint8_t         resourceAllocType;
   FreqDomainAlloc freqAlloc;
   TimeDomainAlloc timeAlloc;
   uint16_t        rowIndex;
   uint8_t         mcs;
   uint8_t         harqProcId;  
   bool            freqHopFlag;
   uint8_t         ndi;    
   uint8_t         rvIndex;     
   uint8_t         tpcCmd;
   bool            sulIndicator;
}Format0_0;

/* Reference -> O-RAN.WG8.AAD.0-v07.00, Table 9-40 DCI Format 0_1 Configuration */
typedef struct format0_1
{
   uint8_t         carrierIndicator;
   bool            sulIndicator;
   uint8_t         bwpIndicator;
   uint8_t         resourceAlloc;
   FreqDomainRsrc  freqAlloc;
   TimeDomainAlloc timeAlloc;
   uint16_t        rowIndex;
   uint8_t         mcs;
   uint8_t         harqProcId;  
   bool            freqHopFlag;
   uint8_t         ndi;   
   uint8_t         rvIndex;    
   uint8_t         firstDownlinkAssignmentIndex;
   uint8_t         secondDownlinkAssignmentIndex;
   uint8_t         tpcCommand;
   uint8_t         srsResourceSetIndicator;
   uint8_t         srsResourceIndicator;
   uint8_t         tpmi;
   uint8_t         antennaPorts;
   uint8_t         srsRequest;
   uint8_t         csiRequest;
   uint8_t         cbgTransmissionInfo;
   uint8_t         ptrsDmrs;
   uint8_t         betaOffsetIndicator;
   bool            dmrsSequenceInitialization;
   bool            ulschIndicatior;
}Format0_1;

typedef struct dciFormat
{
   FormatType     formatType;     /* DCI Format */
   union
   {
      Format0_0  format0_0;      /* Format 0_0 */
      Format0_1  format0_1;      /* Format 0_1 */
   }format;
}DciFormat;

typedef struct dciInfo
{
   uint16_t       crnti;          /* CRNTI */
   BwpCfg         bwpCfg;         /* BWP Cfg */
   CoresetCfg     coresetCfg;     /* Coreset1 Cfg */
   DciFormat      dciFormatInfo;  /* Dci Format */
   DlDCI          dciInfo;        /* DlDCI */
}DciInfo;


/* Reference -> O-RAN.WG8.AAD.0-v07.00, Section 11.2.4.3.8 DL Scheduling Information */
typedef struct dlSchedInfo
{
   uint16_t     cellId;  /* Cell Id */
   SchSlotValue schSlotValue;

   /* Allocation for broadcast messages */
   bool isBroadcastPres;
   DlBrdcstAlloc brdcstAlloc;

   /* Allocation for RAR message */
   RarAlloc *rarAlloc[MAX_NUM_UE];

   /* UL grant in response to BSR */
   DciInfo    *ulGrant;

   /* Allocation from dedicated DL msg */
   DlMsgSchInfo *dlMsgAlloc[MAX_NUM_UE];

}DlSchedInfo;

/*Reference: O-RAN.WG8.AAD.v7.0.0, Sec 11.2.4.3.13 Downlink Paging Allocation*/
typedef struct interleaved_t
{
   uint8_t regBundleSize;
   uint8_t interleaverSize;
   uint16_t shiftIndex;
}Interleaved;

typedef struct pageDlDci
{
   uint8_t    freqDomainResource[6];
   uint8_t    durationSymbols;
   uint8_t    cceRegMappingType;
   union
   {
      Interleaved  interleaved;
      uint8_t      nonInterleaved;
   }cceReg;
   uint8_t    ssStartSymbolIndex;
   uint8_t    cceIndex;
   uint8_t    aggregLevel;
   uint8_t    precoderGranularity;
   uint8_t    coreSetSize;
}PageDlDci;

typedef struct resAllocType1 PageFreqDomainAlloc;

typedef struct pageTimeDomainAlloc
{
   uint8_t mappingType;
   uint16_t startSymb;
   uint16_t numSymb;
}PageTimeDomainAlloc;

typedef struct pageDmrsConfig
{
   uint8_t dmrsType;
   uint8_t dmrsAddPos;
   uint8_t nrOfDmrsSymbols;
}PageDmrsConfig;

typedef struct pageTbInfo
{
   uint8_t         mcs;
   uint32_t        tbSize;
}PageTbInfo;

typedef struct pageDlSch
{
   PageFreqDomainAlloc  freqAlloc;
   PageTimeDomainAlloc  timeAlloc;
   PageDmrsConfig       dmrs;
   uint8_t              vrbPrbMapping;
   PageTbInfo           tbInfo;
   uint8_t              tbScaling;
   uint16_t             dlPagePduLen;
   uint8_t             *dlPagePdu;
}PageDlSch;

typedef struct dlPageAlloc
{
   uint16_t       cellId;
   SlotTimingInfo dlPageTime;
   uint8_t        ssbIdx;
   bool           shortMsgInd;
   uint8_t        shortMsg;
   BwpCfg         bwp;
   PageDlDci      pageDlDci;
   PageDlSch      pageDlSch;
}DlPageAlloc;

typedef struct tbInfo
{
   uint8_t      mcs;       /* MCS */
   uint8_t      ndi;       /* NDI */
   uint8_t      rv;        /* Redundancy Version */
   uint16_t     tbSize;    /* TB Size */
   uint8_t      qamOrder;  /* Modulation Order */
   SchMcsTable  mcsTable;  /* MCS Table */
}TbInfo;

typedef struct schPuschInfo
{
   uint8_t          harqProcId;   /* HARQ Process ID */
   FreqDomainAlloc  fdAlloc;      /* Freq domain allocation */
   TimeDomainAlloc  tdAlloc;      /* Time domain allocation */
   TbInfo           tbInfo;       /* TB info */
#ifdef INTEL_FAPI    
   uint8_t          dmrsMappingType;
   uint8_t          nrOfDmrsSymbols;
   uint8_t          dmrsAddPos;
#endif
}SchPuschInfo;

typedef struct harqInfo
{
   uint16_t    harqAckBitLength;
   uint8_t     betaOffsetHarqAck;
}HarqInfo;

typedef struct csiInfo
{
   uint16_t csiBits;
   uint8_t  betaOffsetCsi;
}CsiInfo;

typedef struct harqFdbkInfo
{
   uint16_t    harqBitLength;
}HarqFdbkInfo;

typedef struct csiFdbkInfo
{
   uint16_t csiBits;
}CsiFdbkInfo;

typedef struct schPucchFormatCfg
{
   uint8_t interSlotFreqHop;
   uint8_t addDmrs;
   uint8_t maxCodeRate;
   uint8_t numSlots;
   bool    pi2BPSK;
   bool    harqAckCSI;
}SchPucchFormatCfg;

typedef struct schPucchInfo
{
   FreqDomainRsrc   fdAlloc;      
   TimeDomainAlloc  tdAlloc;      
   uint8_t          srFlag;
   HarqFdbkInfo     harqInfo;
   CsiFdbkInfo      csiInfo;
   BeamformingInfo  beamPucchInfo;
   uint8_t          pucchFormat;
   uint8_t          intraFreqHop;
   uint16_t         secondPrbHop;
   uint8_t          initialCyclicShift;
   uint8_t          occLen;
   uint8_t          occIdx;
   uint8_t          timeDomOCC;
   uint8_t          addDmrs;
   bool             pi2BPSK;
}SchPucchInfo;

typedef struct schPuschUci
{
   uint8_t          harqProcId;   /* HARQ Process ID */
   FreqDomainAlloc  fdAlloc;      /* Freq domain allocation */
   TimeDomainAlloc  tdAlloc;      /* Time domain allocation */
   TbInfo           tbInfo;       /* TB information */
   HarqInfo         harqInfo;    /* Harq Information */
   CsiInfo          csiInfo;    /* Csi information*/
}SchPuschUci;

/* Reference -> O-RAN.WG8.AAD.0-v07.00, Section 11.2.4.3.9 UL Scheduling Information */
typedef struct ulSchedInfo
{
   uint16_t         cellId;         /* Cell Id */
   uint16_t         crnti;          /* CRNI */
   SlotTimingInfo   slotIndInfo;    /* Slot Info: sfn, slot number */
   uint8_t          dataType;       /* Type of info being scheduled */
   SchPrachInfo     prachSchInfo;   /* Prach scheduling info */
   SchPuschInfo     schPuschInfo;   /* Pusch scheduling info */
   SchPuschUci      schPuschUci;    /* Pusch Uci */
   SchPucchInfo     schPucchInfo;   /* Pucch and Uci scheduling info */
}UlSchedInfo;

/* Info of Scheduling Request to Add/Modify */
typedef struct schSchedReqInfo
{
   uint8_t              schedReqId;
   SchSrProhibitTimer   srProhibitTmr;
   SchSrTransMax        srTransMax;
}SchSchedReqInfo;

/* Scheduling Request Configuration */
typedef struct schSchedReqCfg
{
   uint8_t          addModListCount;
   SchSchedReqInfo  addModList[MAX_NUM_SR_CFG_PER_CELL_GRP];   /* List of Scheduling req to be added/modified */
   uint8_t          relListCount;
   uint8_t          relList[MAX_NUM_SR_CFG_PER_CELL_GRP];      /* list of scheduling request Id to be deleted */
}SchSchedReqCfg;

/* Info of Tag to Add/Modify */
typedef struct schTagInfo
{
   uint8_t       tagId;
   SchTimeAlignmentTimer  timeAlignmentTmr;
}SchTagInfo;

/* Timing Advance Group Configuration */
typedef struct schTagCfg
{
   uint8_t      addModListCount;
   SchTagInfo   addModList[MAX_NUM_TAGS];    /* List of Tag to Add/Modify */
   uint8_t      relListCount;
   uint8_t      relList[MAX_NUM_TAGS];       /* list of Tag Id to release */
}SchTagCfg;

/* Configuration for Power headroom reporting */
typedef struct schPhrCfg
{
   SchPhrPeriodicTimer       periodicTmr;
   SchPhrProhibitTimer       prohibitTmr;
   SchPhrTxPwrFactorChange   txpowerFactorChange;
   bool                      multiplePhr;
   bool                      dummy;
   bool                      type2OtherCell;
   SchPhrModeOtherCG         modeOtherCG;
}SchPhrCfg;

/* MAC cell Group configuration */
typedef struct schMacCellGrpCfg
{
   SchSchedReqCfg   schedReqCfg;
   SchTagCfg        tagCfg;
   SchPhrCfg        phrCfg;             /* To be used only if phrCfgSetupPres is true */      
#ifdef NR_DRX
   bool             drxCfgPresent;
   SchDrxCfg        drxCfg;          /* Drx configuration */
#endif
}SchMacCellGrpCfg;

/* Physical Cell Group Configuration */
typedef struct schPhyCellGrpCfg
{
   SchPdschHarqAckCodebook    pdschHarqAckCodebook;
   int8_t     pNrFr1;
}SchPhyCellGrpCfg;

/* Control resource set info */
typedef struct schControlRsrcSet
{
   uint8_t             cRSetId;                /* Control resource set id */
   uint8_t             freqDomainRsrc[FREQ_DOM_RSRC_SIZE];  /* Frequency domain resource */
   uint8_t             duration;
   SchREGMappingType   cceRegMappingType;
   SchPrecoderGranul   precoderGranularity;
   uint16_t            dmrsScramblingId;
}SchControlRsrcSet;

/*Slot Perioicity and Offset*/
typedef struct schMSlotPeriodAndOffset
{
   SchMSlotPeriodicity  mSlotPeriodicity;
   uint16_t             mSlotOffset;
}SchMSlotPeriodAndOffset;

/* Search Space info */
typedef struct schSearchSpace
{
   uint8_t                  searchSpaceId;
   uint8_t                  cRSetId;
   SchMSlotPeriodAndOffset  mSlotPeriodicityAndOffset;
   uint8_t                  mSymbolsWithinSlot[MONITORING_SYMB_WITHIN_SLOT_SIZE];
   SchAggrLevel             numCandidatesAggLevel1;      /* Number of candidates for aggregation level 1 */
   SchAggrLevel             numCandidatesAggLevel2;      /* Number of candidates for aggregation level 2 */
   SchAggrLevel             numCandidatesAggLevel4;      /* Number of candidates for aggregation level 4 */
   SchAggrLevel             numCandidatesAggLevel8;      /* Number of candidates for aggregation level 8 */
   SchAggrLevel             numCandidatesAggLevel16;     /* Number of candidates for aggregation level 16 */
   SchSearchSpaceType       searchSpaceType;
   uint8_t                  ueSpecificDciFormat;
}SchSearchSpace;

/* PDCCH cofniguration */
typedef struct schPdcchConfig
{
   uint8_t           numCRsetToAddMod;
   SchControlRsrcSet  cRSetToAddModList[MAX_NUM_CRSET];           /* List of control resource set to add/modify */
   uint8_t           numCRsetToRel;
   uint8_t           cRSetToRelList[MAX_NUM_CRSET];              /* List of control resource set to release */
   uint8_t           numSearchSpcToAddMod;
   SchSearchSpace    searchSpcToAddModList[MAX_NUM_SEARCH_SPC];  /* List of search space to add/modify */
   uint8_t           numSearchSpcToRel;
   uint8_t           searchSpcToRelList[MAX_NUM_SEARCH_SPC];     /* List of search space to release */
}SchPdcchConfig;

/* PDSCH time domain resource allocation */
typedef struct schPdschTimeDomRsrcAlloc
{
   uint8_t    *k0;
   SchTimeDomRsrcAllocMappingType    mappingType;
   uint8_t    startSymbol;
   uint8_t    symbolLength;
}SchPdschTimeDomRsrcAlloc;


typedef struct schPdschBundling
{
   struct schStaticBundling
   {
     SchBundlingSizeSet2 size;
   }SchStaticBundling;
   struct schDynamicBundling
   {
     SchBundlingSizeSet1 sizeSet1;
     SchBundlingSizeSet2 sizeSet2;
   }SchDynamicBundling;
}SchPdschBundling;

/* DMRS downlink configuration */
typedef struct schDmrsDlCfg
{
   SchDmrsAdditionPosition   addPos;       /* DMRS additional position */
}SchDmrsDlCfg;

/* PDSCH Configuration */
typedef struct schPdschConfig
{
   SchDmrsDlCfg               dmrsDlCfgForPdschMapTypeA;
   SchResourceAllocType       resourceAllocType;
   uint8_t                    numTimeDomRsrcAlloc;
   SchPdschTimeDomRsrcAlloc   timeDomRsrcAllociList[MAX_NUM_DL_ALLOC]; /* PDSCH time domain DL resource allocation list */
   SchRBGSize                 rbgSize;
   SchCodeWordsSchedByDci     numCodeWordsSchByDci;                    /* Number of code words scheduled by DCI */
   SchBundlingType            bundlingType;
   SchPdschBundling           bundlingInfo;
}SchPdschConfig;

/* Initial Downlink BWP */
typedef struct schInitalDlBwp
{
   bool             pdcchCfgPres;
   SchPdcchConfig   pdcchCfg;
   bool             pdschCfgPres;
   SchPdschConfig   pdschCfg;
}SchInitalDlBwp;

/*Spec 38.331 'RadioLinkMonitoringConfig'*/
typedef uint8_t SchRadioLinkMonitoringRsId;

typedef struct schRadioLinkMonRS
{
   SchRadioLinkMonitoringRsId  radioLinkMonitoringRsId;
   SchPurposeOfFailureDet      purpose; 
   union
   {
      uint8_t ssbIndx;
      uint8_t nzpCsiRsResId;
   }SchDetectionRes;
}SchRadioLinkMonRS;

typedef struct schRadioLinkConfig
{
   SchRadioLinkMonRS           failurDetResAddModList[MAX_FAILURE_DET_RESOURCES];
   SchRadioLinkMonitoringRsId  failurDetResRelList[MAX_FAILURE_DET_RESOURCES];
   uint8_t                     beamFailureInstanceMaxCount;
   uint8_t                     beamFailureDetectionTimer;
}SchRadioLinkConfig;

/*Spec 38.331 "SPS-Config'*/
typedef struct schSpsConfig
{
   uint16_t     periodicity;
   uint8_t      numOfHqProcess;
   uint8_t      n1PucchAN;
   SchMcsTable  mcsTable;
}SchSpsConfig;

/* Spec 38.331, 'BWP-DownlinkDedicated'*/
typedef struct schBwpDlCfgDed
{
  SchPdcchConfig     pdcchCfgDed;
  SchPdschConfig     pdschCfgDed;
  SchSpsConfig       spsCfgDed;
  SchRadioLinkConfig radioLnkMonCfgDed;
}SchBwpDlCfgDed;

/* Spec 38.331, 'BWP-Downlink' Downlink BWP information */
typedef struct schDlBwpInfo
{
   uint8_t          bwpId;
   SchBwpDlCfg      bwpCommon;
   SchBwpDlCfgDed   bwpDedicated;
}SchDlBwpInfo;

/* PDCCH Serving Cell configuration */
typedef struct schPdschServCellCfg
{
   uint8_t                  *maxMimoLayers;           
   SchNumHarqProcForPdsch   numHarqProcForPdsch;
   SchMaxCodeBlkGrpPerTB    *maxCodeBlkGrpPerTb;
   bool                     *codeBlkGrpFlushInd;
   SchPdschXOverhead        *xOverhead;
}SchPdschServCellCfg;

typedef struct schRaPrioritization
{
   uint8_t powerRampingStepHighPriority;
   uint8_t scalingFactorBI;
}SchRaPrioritization;

typedef struct schBfrCsiRsRes
{
   uint8_t csrRsIndex;
   uint8_t raOccList;
   uint8_t raPreambleIndex;
}SchBfrCsiRsRes;

typedef struct schBfrSsbRes
{
   uint16_t ssbIndex;
   uint8_t raPreambleIndex;
}SchBfrSsbRes;

typedef struct schPrachResDedBfr
{
   SchBfrSsbRes    ssb;
   SchBfrCsiRsRes  csiRs;
}SchPrachResDedBfr;

/*Spec 38.331 'BeamFailureRecoveryConfig' */
typedef struct schBeamFailRecoveryCfg
{
   uint8_t             rootSeqIndexBfr;
   SchRachCfgGeneric   rachCfgBfr;
   uint8_t             rsrpThreshSsbBfr;     /* RSRP Threshold SSB */
   SchPrachResDedBfr   candidateBeamRSList;
   uint8_t             ssbPerRachBfr;        /* SSB per RACH occassion */
   uint8_t             raSsbOccMaskIndex;
   uint8_t             recoverySearchSpaceId;
   SchRaPrioritization raPrioBfr;
   uint16_t            bfrTimer;
   uint8_t             msg1SubcSpacing;  /* Subcarrier spacing of RACH */
}SchBeamFailRecoveryCfg;

/* PUCCH Configuration */
typedef struct schPucchResrcSetInfo
{
   uint8_t resrcSetId;
   uint8_t resrcListCount;
   uint8_t resrcList[MAX_NUM_PUCCH_PER_RESRC_SET];
   uint8_t maxPayLoadSize;
}SchPucchResrcSetInfo;

typedef struct schPucchResrcSetCfg
{
   uint8_t resrcSetToAddModListCount;
   SchPucchResrcSetInfo resrcSetToAddModList[MAX_NUM_PUCCH_RESRC_SET];
   uint8_t resrcSetToRelListCount;
   uint8_t resrcSetToRelList[MAX_NUM_PUCCH_RESRC];
}SchPucchResrcSetCfg;

typedef struct schPucchFormat0
{
   uint8_t initialCyclicShift;
   uint8_t numSymbols;
   uint8_t startSymbolIdx;
}SchPucchFormat0;

typedef struct schPucchFormat1
{
   uint8_t initialCyclicShift;
   uint8_t numSymbols;
   uint8_t startSymbolIdx;
   uint8_t timeDomOCC;
}SchPucchFormat1;

typedef struct schPucchFormat2_3
{
   uint8_t numPrbs;
   uint8_t numSymbols;
   uint8_t startSymbolIdx;
}SchPucchFormat2_3;

typedef struct schPucchFormat4
{
   uint8_t numSymbols;
   uint8_t occLen;
   uint8_t occIdx;
   uint8_t startSymbolIdx;
}SchPucchFormat4;

typedef struct schPucchResrcInfo
{
   uint8_t  resrcId;
   uint16_t startPrb;
   uint8_t  intraFreqHop;
   uint16_t secondPrbHop;
   uint8_t  pucchFormat;
   union{
      SchPucchFormat0   *format0; 
      SchPucchFormat1   *format1;
      SchPucchFormat2_3 *format2;
      SchPucchFormat2_3 *format3;
      SchPucchFormat4   *format4;
   }SchPucchFormat;
}SchPucchResrcInfo;

typedef struct schPucchResrcCfg
{
   uint8_t resrcToAddModListCount;
   SchPucchResrcInfo resrcToAddModList[MAX_NUM_PUCCH_RESRC];
   uint8_t resrcToRelListCount;
   uint8_t resrcToRelList[MAX_NUM_PUCCH_RESRC];
}SchPucchResrcCfg;


typedef struct schSchedReqResrcInfo
{
   uint8_t resrcId;
   uint8_t requestId;
   uint8_t periodicity;
   uint16_t offset;
   uint8_t resrc;
}SchSchedReqResrcInfo;

typedef struct schPucchSchedReqCfg
{
   uint8_t           schedAddModListCount;
   SchSchedReqResrcInfo schedAddModList[MAX_NUM_SR_CFG_PER_CELL_GRP];
   uint8_t           schedRelListCount;
   uint8_t           schedRelList[MAX_NUM_SR_CFG_PER_CELL_GRP];
}SchPucchSchedReqCfg;

typedef struct schSpatialRelationInfo
{
   uint8_t spatialRelationId;
   uint8_t servCellIdx;
   uint8_t pathLossRefRSId;
   uint8_t p0PucchId;
   uint8_t closeLoopIdx;
}SchSpatialRelationInfo;

typedef struct schPucchSpatialCfg
{
   uint8_t spatialAddModListCount;
   SchSpatialRelationInfo spatialAddModList[MAX_NUM_SPATIAL_RELATIONS];
   uint8_t spatialRelListCount;
   uint8_t spatialRelList[MAX_NUM_SPATIAL_RELATIONS];
}SchPucchSpatialCfg;

typedef struct schP0PucchCfg
{
   uint8_t p0PucchId;
   int     p0PucchVal;
}SchP0PucchCfg;

typedef struct schPathLossRefRSCfg
{
   uint8_t pathLossRefRSId;
}SchPathLossRefRSCfg;

typedef struct schPucchMultiCsiCfg
{
   uint8_t  multiCsiResrcListCount;
   uint8_t  multiCsiResrcList[MAX_NUM_PUCCH_RESRC-1];
}SchPucchMultiCsiCfg;

typedef struct schPucchDlDataToUlAck
{
   uint8_t  dlDataToUlAckListCount;
   uint8_t  dlDataToUlAckList[MAX_NUM_DL_DATA_TO_UL_ACK];
}SchPucchDlDataToUlAck;

typedef struct schPucchPowerControl
{
   int deltaF_Format0;
   int deltaF_Format1;
   int deltaF_Format2;
   int deltaF_Format3;
   int deltaF_Format4;
   uint8_t p0SetCount;
   SchP0PucchCfg p0Set[MAX_NUM_PUCCH_P0_PER_SET];
   uint8_t pathLossRefRSListCount;
   SchPathLossRefRSCfg pathLossRefRSList[MAX_NUM_PATH_LOSS_REF_RS];
}SchPucchPowerControl;

typedef struct schPucchCfg
{
   SchPucchResrcSetCfg   *resrcSet;
   SchPucchResrcCfg      *resrc;
   SchPucchFormatCfg     *format1; 
   SchPucchFormatCfg     *format2; 
   SchPucchFormatCfg     *format3; 
   SchPucchFormatCfg     *format4;
   SchPucchSchedReqCfg   *schedReq;
   SchPucchMultiCsiCfg   *multiCsiCfg;
   SchPucchSpatialCfg    *spatialInfo;
   SchPucchDlDataToUlAck *dlDataToUlAck;
   SchPucchPowerControl  *powerControl;
}SchPucchCfg;

/* Transform precoding disabled */
typedef struct schTransPrecodDisabled
{
   uint16_t   scramblingId0;
}SchTransPrecodDisabled;

/* DMRS Uplink configuration */
typedef struct SchDmrsUlCfg
{
   SchDmrsAdditionPosition    addPos;               /* DMRS additional position */
   SchTransPrecodDisabled     transPrecodDisabled;  /* Transform precoding disabled */
}SchDmrsUlCfg;

/* PUSCH Configuration */
typedef struct schPuschCfg
{
   uint8_t                    dataScramblingId;
   SchDmrsUlCfg               dmrsUlCfgForPuschMapTypeA;
   SchResourceAllocType       resourceAllocType;
   uint8_t                    numTimeDomRsrcAlloc;
   SchPuschTimeDomRsrcAlloc   timeDomRsrcAllocList[MAX_NUM_UL_ALLOC]; /* PUSCH time domain UL resource allocation list */
   SchTransformPrecoder       transformPrecoder;
}SchPuschCfg;

/* Initial Uplink BWP */
typedef struct schInitialUlBwp
{
   bool          pucchCfgPres;
   SchPucchCfg   pucchCfg;
   bool          puschCfgPres;
   SchPuschCfg   puschCfg;
}SchInitialUlBwp;

typedef struct schBwpCfgDedicated
{
   SchPucchCfg   pucchCfg;
   SchPuschCfg   puschCfg;
}SchBwpCfgDedicated;

/* Uplink BWP information */
typedef struct schUlBwpInfo
{
   uint8_t             bwpId;
   SchBwpUlCfg         bwpCommon;
   SchBwpCfgDedicated  bwpDed;
}SchUlBwpInfo;

typedef struct schBwpRelInfo
{
   uint8_t bwpId;
}SchBwpRelInfo;

/* Serving cell configuration */
typedef struct schServCellRecfgInfo
{
   SchInitalDlBwp          initDlBwp;
   SchRadioLinkConfig      radioLinkMonConfig;
   uint8_t                 numDlBwpToAddOrMod;
   SchDlBwpInfo            dlBwpToAddOrModList[MAX_NUM_BWP];
   uint8_t                 numDlBwpToRel;
   SchBwpRelInfo           dlBwpToRelList[MAX_NUM_BWP];
   uint8_t                 firstActvDlBwpId;
   uint8_t                 defaultDlBwpId;
   uint8_t                 *bwpInactivityTmr;
   SchPdschServCellCfg     pdschServCellCfg;
   SchInitialUlBwp         initUlBwp;
   SchBeamFailRecoveryCfg  beamFailureRecoveryCfg;
   uint8_t                 numUlBwpToAddOrMod;
   SchUlBwpInfo            ulBwpToAddOrModList[MAX_NUM_BWP];
   uint8_t                 numUlBwpToRel;
   SchBwpRelInfo           ulBwpToRelList[MAX_NUM_BWP];
   uint8_t                 firstActvUlBwpId;
}SchServCellRecfgInfo;

/* Serving cell configuration */
typedef struct schServCellCfgInfo
{
   SchInitalDlBwp        initDlBwp;
   SchRadioLinkConfig    radioLinkMonConfig;
   uint8_t               numDlBwpToAdd;
   SchDlBwpInfo          dlBwpToAddList[MAX_NUM_BWP];
   uint8_t               firstActvDlBwpId;
   uint8_t               defaultDlBwpId;
   uint8_t               *bwpInactivityTmr;
   SchPdschServCellCfg     pdschServCellCfg;
   SchInitialUlBwp         initUlBwp;
   SchBeamFailRecoveryCfg  beamFailureRecoveryCfg;
   uint8_t                 numUlBwpToAdd;
   SchUlBwpInfo            ulBwpToAddList[MAX_NUM_BWP];
   uint8_t                 firstActvUlBwpId;
}SchServCellCfgInfo;

typedef struct schNonDynFiveQi
{
   uint16_t   fiveQi;
   uint8_t    priorLevel;
   uint16_t   avgWindow;
   uint16_t   maxDataBurstVol;
}SchNonDynFiveQi;

typedef struct schDynFiveQi
{
   uint8_t    priorLevel;
   uint16_t   packetDelayBudget;
   uint8_t    packetErrRateScalar;
   uint8_t    packetErrRateExp;
   uint16_t   fiveQi;
   uint8_t    delayCritical;
   uint16_t   avgWindow;
   uint16_t   maxDataBurstVol;
}SchDynFiveQi;

typedef struct schNgRanAllocAndRetPri
{
   uint8_t priorityLevel;
   uint8_t preEmptionCap;
   uint8_t preEmptionVul;
}SchNgRanAllocAndRetPri;

typedef struct schGrbQosFlowInfo
{
   uint32_t maxFlowBitRateDl;
   uint32_t maxFlowBitRateUl;
   uint32_t guarFlowBitRateDl;
   uint32_t guarFlowBitRateUl;
}SchGrbQosFlowInfo;

/* DRB QoS */
typedef struct schDrbQos
{
   SchQosType  fiveQiType;   /* Dynamic or non-dynamic */ 
   union
   {
      SchNonDynFiveQi   nonDyn5Qi;
      SchDynFiveQi      dyn5Qi;
   }u;
   SchNgRanAllocAndRetPri  ngRanRetPri;
   SchGrbQosFlowInfo       grbQosFlowInfo;
   uint16_t                pduSessionId;
   uint32_t                ulPduSessAggMaxBitRate;   /* UL PDU Session Aggregate max bit rate */
}SchDrbQosInfo;

/* Special cell configuration */
typedef struct schSpCellCfg
{
   uint8_t           servCellIdx;
   SchServCellCfgInfo   servCellCfg;
}SchSpCellCfg;

/* Special cell Reconfiguration */
typedef struct schSpCellRecfg
{
   uint8_t           servCellIdx;
   SchServCellRecfgInfo   servCellRecfg;
}SchSpCellRecfg;

/* Uplink logical channel configuration */
typedef struct SchUlLcCfg
{
   uint8_t priority;
   uint8_t lcGroup;
   uint8_t schReqId;
   uint8_t pbr;        // prioritisedBitRate
   uint8_t bsd;        // bucketSizeDuration
}SchUlLcCfg;

/* Downlink logical channel configuration */
typedef struct schDlLcCfg
{
   uint8_t lcp;      // logical Channel Prioritization
}SchDlLcCfg;

/* Logical Channel configuration */
typedef struct schLcCfg
{
   uint8_t        lcId;
   Snssai         *snssai;
   SchDrbQosInfo  *drbQos;
   SchDlLcCfg     dlLcCfg;
   SchUlLcCfg     ulLcCfg;
}SchLcCfg;

/* Aggregate max bit rate */
typedef struct schAmbrCfg
{
   uint32_t   ulBr;   /* Ul BitRate */
}SchAmbrCfg;

typedef struct schModulationInfo
{
   uint8_t      modOrder;
   uint8_t      mcsIndex;
   SchMcsTable  mcsTable;
}SchModulationInfo;

/*Spec O-RAN, WG8, V7.0.0, '11.2.4.2.5' UE Configuration Request*/
typedef struct schUeCfgReq
{
   uint16_t           cellId;
   uint8_t            beamIdx; 
   uint16_t           crnti;
   bool               macCellGrpCfgPres;
   SchMacCellGrpCfg   macCellGrpCfg;
   bool               phyCellGrpCfgPres;
   SchPhyCellGrpCfg   phyCellGrpCfg;
   bool               spCellCfgPres;
   SchSpCellCfg       spCellCfg;
   SchAmbrCfg         *ambrCfg;
   SchModulationInfo  dlModInfo;
   SchModulationInfo  ulModInfo;
   uint8_t            numLcsToAdd;
   SchLcCfg           schLcCfg[MAX_NUM_LC];
}SchUeCfgReq;

/*Spec O-RAN, WG8, V7.0.0, '11.2.4.2.6' UE Reconfiguration Request*/
typedef struct schUeRecfgReq
{
   uint16_t         cellId;
   uint8_t          beamIdx;
   uint16_t         crnti;
   bool             macCellGrpRecfgPres;
   SchMacCellGrpCfg macCellGrpRecfg;
   bool             phyCellGrpRecfgPres;
   SchPhyCellGrpCfg phyCellGrpRecfg;
   bool             spCellRecfgPres;
   SchSpCellRecfg   spCellRecfg;
   SchAmbrCfg       *ambrRecfg;
   SchModulationInfo  dlModInfo;
   SchModulationInfo  ulModInfo;
   uint8_t            numLcsToAdd;
   SchLcCfg           schLcCfgAdd[MAX_NUM_LC];
   uint8_t            numLcsToDel;
   uint8_t            lcIdToDel[MAX_NUM_LC];
   uint8_t            numLcsToMod;
   SchLcCfg           schLcCfgMod[MAX_NUM_LC];
   SchDataTransmission dataTransmissionInfo;
#ifdef NR_DRX   
   bool     drxConfigIndicatorRelease;
#endif
}SchUeRecfgReq;

/*Ref: ORAN_WG8.V7.0.0 Sec 11.2.4.3.5 UE Confg Response*/
typedef struct schUeCfgRsp
{
   uint16_t   cellId;
   uint8_t    beamIdx;
   uint16_t   crnti;
   SchMacRsp  rsp;
   CauseOfResult cause;
}SchUeCfgRsp;

/*As per WG8 V7.0.0 Sec 11.2.4.3.6, UE ReCFG and UECFG have same structure definition*/
typedef struct schUeCfgRsp SchUeRecfgRsp;

/*Spec O-RAN, WG8, V7.0.0, '11.2.4.2.7' Delete UE Request*/
typedef struct schUeDelete
{
   uint16_t   cellId;
   uint16_t   crnti;
}SchUeDelete;

/*Ref: ORAN_WG8.V7.0.0 Sec 11.2.4.3.7*/
typedef struct schUeDeleteRsp
{
   uint16_t   cellId;
   uint16_t   crnti;
   SchMacRsp  rsp;
   CauseOfResult cause;
}SchUeDeleteRsp;

/*Spec O-RAN, WG8, V7.0.0, '11.2.4.2.8' DL HARQ Indication*/
typedef struct dlHarqInd
{
   uint16_t       cellId;
   uint16_t       crnti;
   SlotTimingInfo slotInd;
   uint8_t        numHarq;
   uint8_t        harqPayload[MAX_HARQ_BITS_IN_BYTES];
}DlHarqInd;

/*Spec O-RAN, WG8, V7.0.0, '11.2.4.2.9' UL HARQ (CRC) Indication*/
typedef struct crcIndInfo
{
   uint16_t       cellId;
   uint16_t       crnti;
   SlotTimingInfo timingInfo;
   uint16_t       numCrcInd;
   uint8_t        crcInd[MAX_NUMBER_OF_CRC_IND_BITS];
}CrcIndInfo;

/*Spec O-RAN, WG8, V7.0.0, '11.2.4.2.10' UL Channel Quality Indication*/
typedef struct ulCqiReport
{
   CqiUlReportType  reportType;
   uint16_t ulCqi;
   uint8_t  timingAdv;
}UlCqiReport;

typedef struct schUlCqiInd
{
   uint16_t       cellId;
   uint16_t       crnti;
   SlotTimingInfo timingInfo;
   uint8_t        numUlCqiReported;
   UlCqiReport    ulCqiRpt;
}SchUlCqiInd;

/*Spec O-RAN, WG8, V7.0.0, '11.2.4.2.11' DL Channel Quality Indication*/
typedef struct dlCqiReport
{
   uint8_t  reportType; /*Bitmap for CQI, PMI, RI, CRI report*/
   uint16_t cqi;
   uint16_t pmi;
   uint16_t cri;
   uint16_t ri;
}DlCqiReport;

typedef struct schDlCqiInd
{
   uint16_t       cellId;
   uint16_t       crnti;
   SlotTimingInfo timingInfo;
   uint8_t        numDlCqiReported;
   DlCqiReport    dlCqiRpt;
}SchDlCqiInd;

/*Spec O-RAN WG8 v7.0.0, '11.2.4.2.12' Rach Ind contents*/
typedef struct rachIndInfo
{
   uint16_t    cellId;
   uint16_t    crnti;
   SlotTimingInfo timingInfo;
   uint8_t     slotIdx;
   uint8_t     symbolIdx;
   uint8_t     freqIdx;
   uint8_t     preambleIdx;
   uint16_t    timingAdv;
}RachIndInfo;

/*Spec O-RAN WG8 v7.0.0, '11.2.4.2.13' Paging Ind contents*/
typedef struct schPageInd
{
   uint16_t  cellId;
   uint16_t  pf;
   uint8_t   i_s;
   uint16_t  pduLen;
   uint8_t  *pagePdu;
}SchPageInd;

/*ORAN WG8 v7.0.0, Sec 11.2.4.2.14 Rach Res Request*/
typedef struct schRachRsrcReq
{
   SlotTimingInfo slotInd;
   uint16_t       cellId;
   uint16_t       crnti;
   uint8_t        numSsb;
   uint8_t        ssbIdx[MAX_NUM_SSB];
}SchRachRsrcReq;

typedef struct schCfraSsbResource
{
   uint8_t  ssbIdx;
   uint8_t  raPreambleIdx;
}SchCfraSsbResource;

typedef struct schCfraRsrc
{
   uint8_t   numSsb;
   SchCfraSsbResource ssbResource[MAX_NUM_SSB];
}SchCfraResource;

typedef struct schRachRsrcRsp
{
   uint16_t   cellId;
   uint16_t   crnti;
   SchMacRsp  result;
   SchCfraResource  cfraResource;
}SchRachRsrcRsp;

/*ORAN WG8 v7.0.0, Sec 11.2.4.2.15 Rach Res Release*/
typedef struct schRachRsrcRel
{
   SlotTimingInfo slotInd;
   uint16_t   cellId;
   uint16_t   crnti;
   SchCfraResource  cfraResource;
}SchRachRsrcRel;

/*O-RAN WG* v7.0.0 Sec 11.2.4.2.16 DL RLC Buffer Status Information*/
typedef struct dlRlcBOInfo
{
   uint16_t    cellId;
   uint16_t    crnti;
   uint8_t     lcId;
   uint32_t    dataVolume;
}DlRlcBoInfo;

/*O-RAN WG8 v7.0.0 Sec 11.2.4.2.17 Scheduling Request Indication*/
typedef struct srUciIndInfo
{
   uint16_t    cellId;
   uint16_t    crnti;
   SlotTimingInfo slotInd;
   uint8_t     numSrBits;
   uint8_t     srPayload[MAX_SR_BITS_IN_BYTES];
}SrUciIndInfo;

/*O-RAN WG* v7.0.0 Sec 11.2.4.2.18 UL RLC Buffer Status Information*/
typedef struct dataVolInfo
{
   uint8_t  lcgId;
   uint32_t dataVol;
}DataVolInfo;

typedef struct ulBufferStatusRptInd
{
   uint16_t    cellId;
   uint16_t    crnti;
   BsrType     bsrType;
   uint8_t     numLcg;
   DataVolInfo dataVolInfo[MAX_NUM_LOGICAL_CHANNEL_GROUPS];
}UlBufferStatusRptInd;

/**O-RAN WG* v7.0.0 Sec 11.2.4.2.19 Power Headroom Indication*/
typedef struct phrData /*Spec 38.321 Sec 6.1.3.8*/
{
   uint8_t phr;
   uint8_t pcmax_f_c;
}PhrData;

typedef struct singlePhrInfo
{
   PhrData  phrData;
}SinglePhrInfo;

typedef struct multiPhr
{
   PhType   phType;
   PhrData  phrData;
}MultiPhr;

typedef struct multiplePhrInfo /*Spec 38.321 Sec 6.1.3.9*/
{
   uint8_t  numPhrReported;
   MultiPhr multiPhrStat[MAX_PHR_REPORT];
}MultiplePhrInfo;

typedef struct schPwrHeadroomInd
{
   uint16_t    cellId;
   uint16_t    crnti;
   PhrType     phrType;
   union
   {
      SinglePhrInfo   singlePhr;
      MultiplePhrInfo multiPhr;
   }phrInfo;
}SchPwrHeadroomInd;

typedef struct schUeHqInfo
{
   uint16_t  crnti;
   uint8_t   hqProcId;
}SchUeHqInfo;

typedef struct schRlsHqInfo
{
   uint16_t     cellId;
   uint8_t      numUes;
   SchUeHqInfo  *ueHqInfo;
}SchRlsHqInfo;

/* Statistics Request from MAC to SCH */
typedef struct schStatsGrpInfo
{
   uint8_t   groupId;
   uint16_t  periodicity;  /* In milliseconds */
   uint8_t   numStats;
   SchMeasurementType statsList[MAX_NUM_STATS];
}SchStatsGrpInfo;

typedef struct schStatsReq
{
   uint64_t  subscriptionId;
   uint8_t   numStatsGroup;
   SchStatsGrpInfo   statsGrpList[MAX_NUM_STATS_GRP];
}SchStatsReq;

typedef struct schStatsReq SchStatsModificationReq;
typedef struct schStatsRsp SchStatsModificationRsp;

/* Statistics Response from SCH to MAC */
typedef struct schStatsGrpRejected
{
   uint8_t   groupId;
   CauseOfResult cause;
}SchStatsGrpRejected;

typedef struct schStatsRsp
{
   uint64_t             subscriptionId;
   uint8_t              numGrpAccepted;
   uint8_t              statsGrpAcceptedList[MAX_NUM_STATS_GRP];
   uint8_t              numGrpRejected;
   SchStatsGrpRejected  statsGrpRejectedList[MAX_NUM_STATS_GRP];
}SchStatsRsp;

/* Statistics Indication from SCH to MAC */
typedef struct schStats
{
   SchMeasurementType type;
   double value;
}SchStats;

typedef struct schStatsInd
{
   uint64_t    subscriptionId;
   uint8_t     groupId;
   uint8_t     numStats;
   SchStats    measuredStatsList[MAX_NUM_STATS];
}SchStatsInd;

typedef struct schStatsDeleteReq
{
   uint64_t  subscriptionId;
   uint8_t   numStatsGroupToBeDeleted;
   uint8_t   statsGrpIdToBeDelList[MAX_NUM_STATS_GRP];
}SchStatsDeleteReq;

typedef struct statsDeleteResult
{
   uint8_t       groupId;
   SchMacRsp     statsGrpDelRsp;
   CauseOfResult statsGrpDelCause;
}StatsDeleteResult;

typedef struct schStatsDeleteRsp
{
   uint64_t          subscriptionId; /* subscription Id */
   SchMacRsp         subsDelRsp;    /* deletion status of all statsGrp with given subscriptionId */
   CauseOfResult     subsDelCause;  /* cause of failure in deletion of all statsGrp with given subscriptionId*/
   uint8_t           numStatsGroupDeleted; /* num of action deleted */ 
   StatsDeleteResult statsGrpDelInfo[MAX_NUM_STATS_GRP]; /* list of the deletion status for specific actions */
}SchStatsDeleteRsp;

/* function declarations */
uint8_t MacMessageRouter(Pst *pst, void *msg);
uint8_t SchMessageRouter(Pst *pst, void *msg);

/**********************************************************************
  End of file
 **********************************************************************/

