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
#define EVENT_SCH_CELL_CFG           1
#define EVENT_SCH_CELL_CFG_CFM       2
#define EVENT_DL_SCH_INFO            3 
#define EVENT_UL_SCH_INFO            4 
#define EVENT_RACH_IND_TO_SCH        5
#define EVENT_CRC_IND_TO_SCH         6
#define EVENT_DL_RLC_BO_INFO_TO_SCH  7
#define EVENT_ADD_UE_CONFIG_REQ_TO_SCH   8
#define EVENT_UE_CONFIG_RSP_TO_MAC   9
#define EVENT_SLOT_IND_TO_SCH        10
#define EVENT_SHORT_BSR              11
#define EVENT_UCI_IND_TO_SCH         12
#define EVENT_MODIFY_UE_CONFIG_REQ_TO_SCH 13
#define EVENT_UE_RECONFIG_RSP_TO_MAC 14
#define EVENT_UE_DELETE_REQ_TO_SCH   15
#define EVENT_UE_DELETE_RSP_TO_MAC   16
#define EVENT_CELL_DELETE_REQ_TO_SCH   17
#define EVENT_CELL_DELETE_RSP_TO_MAC   18
#define EVENT_LONG_BSR              19
#define EVENT_SLICE_CFG_REQ_TO_SCH  20
#define EVENT_SLICE_CFG_RSP_TO_MAC  21
#define EVENT_SLICE_RECFG_REQ_TO_SCH  22
#define EVENT_SLICE_RECFG_RSP_TO_MAC  23

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
#define MAX_NUM_LOGICAL_CHANNEL_GROUPS 8
/* can we have a common numslot numscs between mac sch */
#ifdef NR_TDD
#define MAX_SLOTS 20
#else
#define MAX_SLOTS 10
#endif
#define MAX_SFN   1024
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

#define ADD_DELTA_TO_TIME(crntTime, toFill, incr)          \
{                                                          \
   if ((crntTime.slot + incr) > (MAX_SLOTS - 1))           \
   {                                                       \
      toFill.sfn = (crntTime.sfn + 1);                     \
   }                                                       \
   else                                                    \
   {                                                       \
      toFill.sfn = crntTime.sfn;                           \
   }                                                       \
   toFill.slot = (crntTime.slot + incr) % MAX_SLOTS;       \
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
   SLICE_FOUND,
   SLICE_NOT_FOUND
}RspCause;

typedef enum
{
   NO_TRANSMISSION,
   NEW_TRANSMISSION,
   REPEATITION 
}PduTxOccsaion;

typedef enum
{
   UNSPECIFIED_CAUSE,
   INVALID_PARAM_VALUE,
   RESOURCE_UNAVAILABLE,
   SYSTEM_ERROR
}SchFailureCause;

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
   NOT_APPLICABLE,
   INVALID_CELLID,
   INVALID_UEID
}ErrorCause;

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
   SLOT_PERIODICITY_AND_OFFSET_SL_1 = 1,
   SLOT_PERIODICITY_AND_OFFSET_SL_2,
   SLOT_PERIODICITY_AND_OFFSET_SL_4,
   SLOT_PERIODICITY_AND_OFFSET_SL_5,
   SLOT_PERIODICITY_AND_OFFSET_SL_8,
   SLOT_PERIODICITY_AND_OFFSET_SL_10,
   SLOT_PERIODICITY_AND_OFFSET_SL_16,
   SLOT_PERIODICITY_AND_OFFSET_SL_20,
   SLOT_PERIODICITY_AND_OFFSET_SL_40,
   SLOT_PERIODICITY_AND_OFFSET_SL_80,
   SLOT_PERIODICITY_AND_OFFSET_SL_160,
   SLOT_PERIODICITY_AND_OFFSET_SL_320,
   SLOT_PERIODICITY_AND_OFFSET_SL_640,
   SLOT_PERIODICITY_AND_OFFSET_SL_1280,
   SLOT_PERIODICITY_AND_OFFSET_SL_2560
}SchMSlotPeriodAndOffset;

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

/*structures*/
typedef struct timeDomainAlloc
{
   uint16_t startSymb;
   uint16_t numSymb;
}TimeDomainAlloc;

typedef struct freqDomainAlloc
{
   uint16_t startPrb;
   uint16_t numPrb;
}FreqDomainAlloc;


typedef struct
{
   uint32_t    ssbPbchPwr;       /* SSB block power */
   uint8_t     scsCommon;           /* subcarrier spacing for common [0-3]*/
   uint8_t     ssbOffsetPointA;  /* SSB sub carrier offset from point A */
   SchSSBPeriod   ssbPeriod;        /* SSB Periodicity in msec */
   uint8_t     ssbSubcOffset;    /* Subcarrier Offset(Kssb) */
   uint32_t    nSSBMask[SCH_SSB_MASK_SIZE];      /* Bitmap for actually transmitted SSB. */
}SchSsbCfg;

typedef struct bwpCfg
{
   uint8_t         subcarrierSpacing;
   uint8_t         cyclicPrefix;
   FreqDomainAlloc freqAlloc;	
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
   FreqDomainAlloc freqAlloc;
   uint8_t  vrbPrbMapping;
} PdschFreqAlloc;

typedef struct pdschTimeAlloc
{
   uint8_t         rowIndex;
   TimeDomainAlloc timeAlloc;
} PdschTimeAlloc;

typedef struct txPowerPdschInfo
{
   uint8_t powerControlOffset;
   uint8_t powerControlOffsetSS;
} TxPowerPdschInfo;

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

typedef struct coresetCfg
{
   uint8_t coreSetSize;
   uint8_t startSymbolIndex;
   uint8_t durationSymbols;
   uint8_t freqDomainResource[6];
   uint8_t cceRegMappingType;
   uint8_t regBundleSize;
   uint8_t interleaverSize;
   uint8_t coreSetType;
   uint16_t shiftIndex;
   uint8_t precoderGranularity;
   uint8_t cceIndex;
   uint8_t aggregationLevel;
} CoresetCfg;

typedef struct txPowerPdcchInfo
{
   uint8_t powerValue;
   uint8_t powerControlOffsetSS;
} TxPowerPdcchInfo;

typedef struct dlDCI
{
   uint16_t rnti;
   uint16_t scramblingId;
   uint16_t scramblingRnti;
   uint8_t cceIndex;
   uint8_t aggregLevel;
   BeamformingInfo beamPdcchInfo;
   TxPowerPdcchInfo txPdcchPower;
   PdschCfg     *pdschCfg;
} DlDCI;

typedef struct pdcchCfg
{
   /* coreset-0 configuration */
   CoresetCfg coresetCfg;

   uint16_t numDlDci;
   DlDCI    dci; /* as of now its only one DCI, later it will be numDlCi */
} PdcchCfg;
/* end of SIB1 PDCCH structures */

typedef struct
{
   /* parameters recieved from DU-APP */
   uint16_t sib1PduLen;
   uint16_t sib1RepetitionPeriod;
   uint8_t  coresetZeroIndex;     /* derived from 4 LSB of pdcchSib1 present in MIB */
   uint8_t  searchSpaceZeroIndex; /* derived from 4 MSB of pdcchSib1 present in MIB */
   uint16_t sib1Mcs;

   /* parameters derived in scheduler */
   uint8_t n0;
   BwpCfg bwp;
   PdcchCfg sib1PdcchCfg;
   PdschCfg sib1PdschCfg;
}SchSib1Cfg;

typedef struct schRachCfg
{
   uint8_t      prachCfgIdx;       /* PRACH config idx */
   uint8_t      prachSubcSpacing;  /* Subcarrier spacing of RACH */
   uint16_t     msg1FreqStart;     /* Msg1-FrequencyStart */
   uint8_t      msg1Fdm;           /* PRACH FDM (1,2,4,8) */
   uint8_t      rootSeqLen;        /* root sequence length */
   uint16_t     rootSeqIdx;        /* Root sequence index */
   uint8_t      numRootSeq;        /* Number of root sequences required for FD */
   uint16_t     k1;                /* Frequency Offset for each FD */
   uint8_t      ssbPerRach;        /* SSB per RACH occassion */
   uint8_t      prachMultCarrBand; /* Presence of Multiple carriers in Band */
   uint8_t      raContResTmr;      /* RA Contention Resoultion Timer */
   uint8_t      rsrpThreshSsb;     /* RSRP Threshold SSB */
   uint8_t      raRspWindow;       /* RA Response Window */
}SchRachCfg;

typedef struct schBwpParams
{
   FreqDomainAlloc freqAlloc;
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
   SchK0K1TimingInfoTbl k0K1InfoTbl;
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
   SchPucchCfgCmn pucchCommon;
   SchPuschCfgCmn puschCommon;
   SchK2TimingInfoTbl msg3K2InfoTbl;
   SchK2TimingInfoTbl k2InfoTbl;
}SchBwpUlCfg;

typedef struct schPlmnInfoList
{
   Plmn           plmn;
   uint8_t        numSliceSupport; /* Total slice supporting */
   Snssai         **snssai;         /* List of supporting snssai*/
}SchPlmnInfoList;

typedef struct schCellCfg
{
   uint16_t       cellId;           /* Cell Id */
   uint16_t       phyCellId;        /* Physical cell id */
   uint8_t        numerology;       /* Supported numerology */
   SchDuplexMode  dupMode;          /* Duplex type: TDD/FDD */
   uint8_t        bandwidth;        /* Supported B/W */
   uint32_t       dlFreq;           /* DL Frequency */
   uint32_t       ulFreq;           /* UL Frequency */
   SchSsbCfg      ssbSchCfg;        /* SSB config */
   SchSib1Cfg     sib1SchCfg;       /* SIB1 config */
   SchRachCfg     schRachCfg;       /* PRACH config */
   SchBwpDlCfg    schInitialDlBwp;  /* Initial DL BWP */
   SchBwpUlCfg    schInitialUlBwp;  /* Initial UL BWP */
   SchPlmnInfoList plmnInfoList;     /* Consits of PlmnId and Snssai list */
#ifdef NR_TDD
   TDDCfg         tddCfg;           /* TDD Cfg */ 
#endif   
}SchCellCfg;

typedef struct schCellCfgCfm
{
   uint16_t         cellId;     /* Cell Id */
   SchMacRsp   rsp;   
}SchCellCfgCfm;

typedef struct ssbInfo
{
   uint8_t         ssbIdx;          /* SSB Index */
   TimeDomainAlloc tdAlloc; /* Time domain allocation */
   FreqDomainAlloc fdAlloc; /* Freq domain allocation */
}SsbInfo;

typedef struct sib1AllocInfo
{
   BwpCfg bwp;
   PdcchCfg sib1PdcchCfg;
   PdschCfg sib1PdschCfg;
} Sib1AllocInfo;

typedef struct prachSchInfo
{
   uint8_t  numPrachOcas;   /* Num Prach Ocassions */
   uint8_t  prachFormat;    /* PRACH Format */
   uint8_t  numRa;          /* Freq domain ocassion */
   uint8_t  prachStartSymb; /* Freq domain ocassion */
}PrachSchInfo;

/* Interface structure signifying DL broadcast allocation for SSB, SIB1 */
typedef struct dlBrdcstAlloc
{
   /* Ssb transmission is determined as follows:
    * 0 : No tranamission
    * 1 : SSB Transmission
    * 2 : SSB Repetition */
   uint8_t ssbTrans;
   uint8_t ssbIdxSupported;
   SsbInfo ssbInfo[MAX_SSB_IDX];
   /* Sib1 transmission is determined as follows:
    * 0 : No tranamission
    * 1 : SIB1 Transmission
    * 2 : SIB1 Repetition */
   uint8_t sib1Trans;
   Sib1AllocInfo sib1Alloc;
}DlBrdcstAlloc;

typedef struct msg3UlGrant
{
   uint8_t         freqHopFlag;
   uint16_t        bwpSize;
   FreqDomainAlloc msg3FreqAlloc;
   uint8_t         k2Index;
   uint8_t         mcs;
   uint8_t         tpc;
   uint8_t         csiReq;
}Msg3UlGrant;

typedef struct rarInfo
{
   uint16_t        raRnti;
   uint8_t         RAPID;
   uint16_t        ta;
   Msg3UlGrant     ulGrant;
   uint16_t        tcrnti;
   uint8_t         rarPdu[RAR_PAYLOAD_SIZE];
   uint8_t         rarPduLen;
}RarInfo;

typedef struct rarAlloc
{
   DlPduType  pduPres;
   uint8_t    pdschSlot;
   RarInfo rarInfo;
   BwpCfg  bwp;
   PdcchCfg rarPdcchCfg;
   PdschCfg rarPdschCfg;
}RarAlloc;

typedef struct dlMsgInfo
{
   uint16_t crnti;
   uint8_t  ndi;
   uint8_t  harqProcNum;
   uint8_t  dlAssignIdx;
   uint8_t  pucchTpc;
   uint8_t  pucchResInd;
   uint8_t  harqFeedbackInd;
   uint8_t  dciFormatId;
   bool     isMsg4Pdu;
   uint16_t  dlMsgPduLen;
   uint8_t  *dlMsgPdu;
}DlMsgInfo;

typedef struct lcSchInfo
{
   uint8_t   lcId;
   uint32_t  schBytes; /* Number of scheduled bytes */
}LcSchInfo;

typedef struct dlMsgSchedInfo
{
   uint8_t    numLc;
   LcSchInfo  lcSchInfo[MAX_NUM_LC]; /* Scheduled LC info */
   BwpCfg     bwp;
   PdcchCfg   dlMsgPdcchCfg;
   PdschCfg   dlMsgPdschCfg;
   DlPduType  pduPres;
   uint8_t    pdschSlot;
   DlMsgInfo  dlMsgInfo;
}DlMsgSchInfo;

typedef struct dlMsgAlloc
{
   uint16_t     crnti;
   uint8_t      numSchedInfo;
   DlMsgSchInfo dlMsgSchedInfo[2];
}DlMsgAlloc;

typedef struct schSlotValue
{
   SlotTimingInfo currentTime;
   SlotTimingInfo broadcastTime;
   SlotTimingInfo rarTime;
   SlotTimingInfo dlMsgTime;
   SlotTimingInfo ulDciTime;
}SchSlotValue;

typedef struct format0_0
{
   uint8_t         resourceAllocType;
   /* since we are using type-1, hence rbBitmap excluded */
   FreqDomainAlloc freqAlloc;
   TimeDomainAlloc timeAlloc;
   uint16_t        rowIndex;
   uint8_t         mcs;
   uint8_t         harqProcId;   /* HARQ Process ID */
   bool            puschHopFlag;
   bool            freqHopFlag;
   uint8_t         ndi;    /* NDI */
   uint8_t         rv;     /* Redundancy Version */
   uint8_t         tpcCmd;
   bool            sUlCfgd;
}Format0_0;

typedef struct format0_1
{
/* TBD */

}Format0_1;

typedef struct format1_0
{
/* TBD */

}Format1_0;

typedef struct format1_1
{
/* TBD */
}Format1_1;

typedef struct dciInfo
{
   uint16_t      cellId;   
   uint16_t      crnti;          /* CRNI */
   SlotTimingInfo   slotIndInfo;    /* Slot Info: sfn, slot number */
   BwpCfg        bwpCfg;         /* BWP Cfg */
   CoresetCfg    coresetCfg;     /* Coreset1 Cfg */
   FormatType    formatType;     /* DCI Format */
   union
   {
      Format0_0  format0_0;      /* Format 0_0 */
      Format0_1  format0_1;      /* Format 0_1 */
      Format1_0  format1_0;      /* Format 1_0 */
      Format1_1  format1_1;      /* Format 1_1 */
   }format;
   DlDCI    dciInfo;
}DciInfo;


typedef struct dlSchedInfo
{
   uint16_t cellId;  /* Cell Id */
   SchSlotValue schSlotValue;

   /* Allocation for broadcast messages */
   bool isBroadcastPres;
   DlBrdcstAlloc brdcstAlloc;

   /* Allocation for RAR message */
   RarAlloc *rarAlloc[MAX_NUM_UE];

   /* UL grant in response to BSR */
   DciInfo    *ulGrant;

   /* Allocation from dedicated DL msg */
   DlMsgAlloc *dlMsgAlloc[MAX_NUM_UE];

}DlSchedInfo;

typedef struct tbInfo
{
   uint8_t  qamOrder;  /* Modulation Order */
   uint8_t  mcs;       /* MCS */
   SchMcsTable  mcsTable;  /* MCS Table */
   uint8_t  ndi;       /* NDI */
   uint8_t  rv;        /* Redundancy Version */
   uint16_t tbSize;    /* TB Size */
}TbInfo;

typedef struct schPuschInfo
{
   uint16_t         crnti;
   uint8_t          harqProcId;   /* HARQ Process ID */
   uint8_t          resAllocType; /* Resource allocation type */
   FreqDomainAlloc  fdAlloc;      /* Freq domain allocation */
   TimeDomainAlloc  tdAlloc;      /* Time domain allocation */
   TbInfo           tbInfo;       /* TB info */
   uint8_t          dmrsMappingType;
   uint8_t          nrOfDmrsSymbols;
   uint8_t          dmrsAddPos;
}SchPuschInfo;

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
   uint16_t         rnti;
   uint8_t          pucchFormat;
   FreqDomainAlloc  fdAlloc;      /* Freq domain allocation */
   TimeDomainAlloc  tdAlloc;      /* Time domain allocation */
   uint8_t          srFlag;
   uint8_t          harqFlag;
   uint8_t          numHarqBits;
   uint8_t          uciFlag;
   uint8_t          numUciBits;
   uint8_t          intraFreqHop;
   uint16_t         secondPrbHop;
   uint8_t          initialCyclicShift;
   uint8_t          occLen;
   uint8_t          occIdx;
   uint8_t          timeDomOCC;
   SchPucchFormatCfg cmnFormatCfg;
}SchPucchInfo;

typedef struct ulSchedInfo
{
   uint16_t      cellId;         /* Cell Id */
   uint16_t      crnti;          /* CRNI */
   SlotTimingInfo   slotIndInfo;    /* Slot Info: sfn, slot number */
   uint8_t       dataType;       /* Type of info being scheduled */
   PrachSchInfo  prachSchInfo;   /* Prach scheduling info */
   SchPuschInfo  schPuschInfo;   /* Pusch scheduling info */
   SchPucchInfo  schPucchInfo;   /* Pusch scheduling info */
}UlSchedInfo;

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


typedef struct crcIndInfo
{
   uint16_t    cellId;
   uint16_t    crnti;
   SlotTimingInfo timingInfo;
   uint16_t    numCrcInd;
   uint8_t     crcInd[MAX_NUMBER_OF_CRC_IND_BITS];
}CrcIndInfo;

typedef struct boInfo
{
   uint8_t   lcId;
   uint32_t  dataVolume;
}BOInfo;

typedef struct dlRlcBOInfo
{
   uint16_t    cellId;
   uint16_t    crnti;
   uint8_t     lcId;
   uint32_t    dataVolume;
}DlRlcBoInfo;

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
   bool             k0K1TblPrsnt;
   SchK0K1TimingInfoTbl k0K1InfoTbl;
}SchInitalDlBwp;

/* BWP Downlink common */
typedef struct schBwpDlCommon
{
}SchBwpDlCommon;

/* Downlink BWP information */
typedef struct schDlBwpInfo
{
   uint8_t          bwpId;
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
   bool          k2TblPrsnt;
   SchK2TimingInfoTbl k2InfoTbl;
}SchInitialUlBwp;

/* Uplink BWP information */
typedef struct schUlBwpInfo
{
   uint8_t        bwpId;
}SchUlBwpInfo;

/* Serving cell configuration */
typedef struct schServCellCfgInfo
{
   SchInitalDlBwp        initDlBwp;
   uint8_t               numDlBwpToAdd;
   SchDlBwpInfo          DlBwpToAddList[MAX_NUM_BWP];
   uint8_t               firstActvDlBwpId;
   uint8_t               defaultDlBwpId;
   uint8_t               *bwpInactivityTmr;
   SchPdschServCellCfg   pdschServCellCfg;
   SchInitialUlBwp       initUlBwp;
   uint8_t               numUlBwpToAdd;
   SchUlBwpInfo          UlBwpToAddList[MAX_NUM_BWP];
   uint8_t               firstActvUlBwpId;
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
   ConfigType     configType;
   uint8_t        lcId;
   SchDrbQosInfo  *drbQos;
   Snssai         *snssai;
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

/* UE configuration */
typedef struct schUeCfg
{
   uint16_t        cellId;
   uint16_t        crnti;
   bool macCellGrpCfgPres;
   SchMacCellGrpCfg   macCellGrpCfg;
   bool phyCellGrpCfgPres;
   SchPhyCellGrpCfg   phyCellGrpCfg;
   bool spCellCfgPres;
   SchSpCellCfg       spCellCfg;
   SchAmbrCfg         *ambrCfg;
   SchModulationInfo  dlModInfo;
   SchModulationInfo  ulModInfo;
   uint8_t            numLcs;
   SchLcCfg           schLcCfg[MAX_NUM_LC];
}SchUeCfg;

typedef struct schUeCfgRsp
{
   uint16_t   ueId;
   uint16_t   cellId;
   uint16_t   crnti;
   SchMacRsp  rsp;
   SchFailureCause cause;
}SchUeCfgRsp;

typedef struct schUeDelete
{
   uint16_t   cellId;
   uint16_t   crnti;
}SchUeDelete;

typedef struct schUeDeleteRsp
{
   uint16_t   cellId;
   uint16_t   crnti;
   SchMacRsp  rsp;
   ErrorCause cause;
}SchUeDeleteRsp;

typedef struct schCellDelete 
{
   uint16_t   cellId;
}SchCellDelete;


typedef struct schCellDeleteRsp
{
   uint16_t   cellId;
   SchMacRsp  rsp;
}SchCellDeleteRsp;

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

typedef struct srUciIndInfo
{
   uint16_t    cellId;
   uint16_t    crnti;
   SlotTimingInfo slotInd;
   uint8_t     numSrBits;
   uint8_t     srPayload[MAX_SR_BITS_IN_BYTES];
}SrUciIndInfo;

typedef struct schRrmPolicyRatio
{
   uint8_t policyMaxRatio;
   uint8_t policyMinRatio;
   uint8_t policyDedicatedRatio;
}SchRrmPolicyRatio;

typedef struct schRrmPolicyOfSlice
{
   Snssai  snssai;
   SchRrmPolicyRatio *rrmPolicyRatioInfo;
}SchRrmPolicyOfSlice;

typedef struct schSliceCfgReq
{
   uint8_t  numOfConfiguredSlice;
   SchRrmPolicyOfSlice **listOfConfirguration;
}SchSliceCfgReq;

typedef struct sliceRsp
{
   Snssai     snssai;
   SchMacRsp  rsp;
   RspCause   cause;
}SliceRsp;

typedef struct schSliceRsp
{
   uint8_t    numSliceCfgRsp;
   SliceRsp   **listOfSliceCfgRsp;
}SchSliceCfgRsp;

/* function pointers */
typedef uint8_t (*SchCellCfgCfmFunc)    ARGS((
	 Pst            *pst,           /* Post Structure */                         
	 SchCellCfgCfm  *schCellCfgCfm  /* Cell Cfg Cfm */
	 ));

typedef uint8_t (*SchCellCfgFunc)    ARGS((
	 Pst         *pst,           /* Post Structure */                         
	 SchCellCfg  *schCellCfg     /* Cell Cfg  */
	 ));

typedef uint8_t (*SchMacDlAllocFunc)     ARGS((                     
	 Pst            *pst,          /* Post Structure */                         
	 DlSchedInfo    *dlSchedInfo   /* dl allocation Info */                      
	 ));

typedef uint8_t (*SchMacUlSchInfoFunc)     ARGS((                     
	 Pst         *pst,           /* Post Structure */                         
	 UlSchedInfo *ulSchedInfo    /* UL Alloc Sch  Info */                      
	 ));

typedef uint8_t (*MacSchRachIndFunc) ARGS((
	 Pst         *pst,         /* Post structure */
	 RachIndInfo *rachInd));    /* Rach Indication Info */

typedef uint8_t (*MacSchCrcIndFunc) ARGS(( 
	 Pst         *pst,         /* Post structure */
	 CrcIndInfo  *crcInd));     /* CRC Info */

typedef uint8_t (*MacSchDlRlcBoInfoFunc) ARGS((
	 Pst         *pst,         /* Post structure */
	 DlRlcBoInfo *dlBoInfo));   /* DL BO Info */

typedef uint8_t (*MacSchAddUeConfigReqFunc) ARGS((
	 Pst         *pst,           /* Post structure */
	 SchUeCfg    *ueCfgToSch));   /* Scheduler UE Cfg */

typedef uint8_t (*SchUeCfgRspFunc) ARGS((
	 Pst         *pst,           /* Post structure */
	 SchUeCfgRsp *cfgRsp));       /* Scheduler UE Cfg response */

typedef uint8_t (*MacSchSlotIndFunc) ARGS((
         Pst         *pst,          /* Post structure */
	 SlotTimingInfo *slotInd));    /* Slot Info */

typedef uint8_t (*MacSchBsrFunc)       ARGS((
   Pst                  *pst,
   UlBufferStatusRptInd *bsrInd
));

typedef uint8_t (*MacSchSrUciIndFunc) ARGS(( 
	 Pst         *pst,         /* Post structure */
	 SrUciIndInfo  *uciInd));    /* UCI IND Info */

typedef uint8_t (*MacSchModUeConfigReqFunc) ARGS((
	 Pst         *pst,           /* Post structure */
	 SchUeCfg    *ueCfgToSch));   /* Scheduler UE Cfg */

typedef uint8_t (*SchUeReCfgRspFunc) ARGS((
	 Pst         *pst,           /* Post structure */
	 SchUeCfgRsp *cfgRsp));       /* Scheduler UE Cfg response */

typedef uint8_t (*MacSchUeDeleteReqFunc) ARGS((
   Pst         *pst,           /* Post structure */
   SchUeDelete *schUeDel)); /*Scheduler UE Del*/

typedef uint8_t (*SchUeDeleteRspFunc) ARGS((
   Pst          *pst,           /* Post structure */
   SchUeDeleteRsp *delRsp));       /* Scheduler UE delete response */

typedef uint8_t (*MacSchCellDeleteReqFunc) ARGS((
   Pst         *pst,           /* Post structure */
   SchCellDelete *schCellDelete)); /*Scheduler UE Del*/

typedef uint8_t (*SchCellDeleteRspFunc) ARGS((
   Pst          *pst,           /* Post structure */
   SchCellDeleteRsp *schCellDeleteRsp));       /* Scheduler UE delete response */

typedef uint8_t (*MacSchSliceCfgReqFunc) ARGS((
   Pst          *pst,           /* Post structure */
   SchSliceCfgReq *schSliceCfgReq));  /* Scheduler Slice Cfg Req */

typedef uint8_t (*SchSliceCfgRspFunc)    ARGS((
	 Pst            *pst,            /* Post Structure */                         
	 SchSliceCfgRsp  *schSliceCfgRsp /* Cell Cfg Cfm */
	 ));

typedef uint8_t (*MacSchSliceReCfgReqFunc) ARGS((
   Pst          *pst,           /* Post structure */
   SchSliceCfgReq *schSliceReCfgReq));  /* Scheduler Slice ReCfg Req */

typedef uint8_t (*SchSliceReCfgRspFunc)    ARGS((
	 Pst            *pst,            /* Post Structure */                         
	 SchSliceCfgRsp  *schSliceReCfgRsp /* Cell ReCfg Cfm */
	 ));
/* function declarations */
uint8_t packMacSchSlotInd(Pst *pst, SlotTimingInfo *slotInd);
uint8_t packSchMacDlAlloc(Pst *pst, DlSchedInfo  *dlSchedInfo);
uint8_t packSchMacUlSchInfo(Pst *pst, UlSchedInfo *ulSchedInfo);
uint8_t packSchCellCfg(Pst *pst, SchCellCfg  *schCellCfg);
uint8_t packSchCellCfgCfm(Pst *pst, SchCellCfgCfm  *schCellCfgCfm);
uint8_t MacProcDlAlloc(Pst *pst, DlSchedInfo *dlSchedInfo);
uint8_t MacProcSchCellCfg(Pst *pst, SchCellCfg  *schCellCfg);
uint8_t MacProcSchCellCfgCfm(Pst *pst, SchCellCfgCfm  *schCellCfgCfm);
uint8_t SchHdlCellCfgReq(Pst *pst, SchCellCfg *schCellCfg);
uint8_t schActvInit(Ent entity, Inst instId, Region region, Reason reason);
uint8_t MacProcUlSchInfo(Pst *pst, UlSchedInfo *ulSchedInfo);
uint8_t packMacSchRachInd(Pst *pst, RachIndInfo *rachInd);
uint8_t MacSchRachInd(Pst *pst, RachIndInfo *rachInd);
uint8_t packMacSchCrcInd(Pst *pst, CrcIndInfo *crcInd);
uint8_t MacSchCrcInd(Pst *pst, CrcIndInfo *crcInd);
uint8_t packMacSchDlRlcBoInfo(Pst *pst, DlRlcBoInfo *dlBoInfo);
uint8_t MacSchDlRlcBoInfo(Pst *pst, DlRlcBoInfo *dlBoInfo);
uint8_t packMacSchAddUeConfigReq(Pst *pst, SchUeCfg *ueCfgToSch);
uint8_t MacSchAddUeConfigReq(Pst *pst, SchUeCfg *ueCfgToSch);
uint8_t packSchUeCfgRsp(Pst *pst, SchUeCfgRsp *cfgRsp);
uint8_t MacProcSchUeCfgRsp(Pst *pst, SchUeCfgRsp *cfgRsp);
uint8_t MacSchSlotInd ARGS((Pst * pst, SlotTimingInfo * slotInd));
uint8_t packMacSchSlotInd(Pst * pst, SlotTimingInfo * slotInd);
uint8_t unpackMacSchSlotInd(MacSchSlotIndFunc func, Pst *pst, Buffer  *mBuf);
uint8_t packMacSchBsr(Pst *pst, UlBufferStatusRptInd *bsrInd);
uint8_t MacSchBsr(Pst *pst, UlBufferStatusRptInd *bsrInd);
uint8_t packMacSchSrUciInd(Pst *pst, SrUciIndInfo *uciInd);
uint8_t MacSchSrUciInd(Pst *pst, SrUciIndInfo *uciInd);
uint8_t packMacSchModUeConfigReq(Pst *pst, SchUeCfg *ueCfgToSch);
uint8_t MacSchModUeConfigReq(Pst *pst, SchUeCfg *ueCfgToSch);
uint8_t packSchUeReconfigRsp(Pst *pst, SchUeCfgRsp *cfgRsp);
uint8_t MacProcSchUeReconfigRsp(Pst *pst, SchUeCfgRsp *cfgRsp);
uint8_t packMacSchUeDeleteReq(Pst *pst,  SchUeDelete *schUeDel);
uint8_t MacSchUeDeleteReq(Pst *pst, SchUeDelete  *ueDelete);
uint8_t packSchUeDeleteRsp(Pst *pst, SchUeDeleteRsp  *delRsp);
uint8_t MacProcSchUeDeleteRsp(Pst *pst, SchUeDeleteRsp *schUeDelRsp);
uint8_t packMacSchCellDeleteReq(Pst *pst,  SchCellDelete *schCellDelete);
uint8_t MacSchCellDeleteReq(Pst *pst, SchCellDelete  *schCellDelete);
uint8_t packSchCellDeleteRsp(Pst *pst, SchCellDeleteRsp  *schCellDeleteRsp);
uint8_t MacProcSchCellDeleteRsp(Pst *pst, SchCellDeleteRsp *schCellDeleteRsp);
uint8_t packMacSchSliceCfgReq(Pst *pst, SchSliceCfgReq *cfgReq);
uint8_t MacSchSliceCfgReq(Pst *pst, SchSliceCfgReq *schSliceCfgReq);
uint8_t packSchSliceCfgRsp(Pst *pst, SchSliceCfgRsp *cfgRsp);
uint8_t MacProcSchSliceCfgRsp(Pst *pst, SchSliceCfgRsp *cfgRsp);
uint8_t packMacSchSliceReCfgReq(Pst *pst, SchSliceCfgReq *cfgReq);
uint8_t MacSchSliceReCfgReq(Pst *pst, SchSliceCfgReq *schSliceCfgReq);
uint8_t packSchSliceReCfgRsp(Pst *pst, SchSliceCfgRsp *cfgRsp);
uint8_t MacProcSchSliceReCfgRsp(Pst *pst, SchSliceCfgRsp *sliceReCfgrsp);
/**********************************************************************
  End of file
 **********************************************************************/

