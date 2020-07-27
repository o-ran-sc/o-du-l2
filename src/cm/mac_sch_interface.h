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
#define EVENT_SCH_CELL_CFG      1
#define EVENT_SCH_CELL_CFG_CFM  2
#define EVENT_DL_ALLOC          3 
#define EVENT_UL_SCH_INFO       4 
#define EVENT_RACH_IND_TO_SCH        5
#define EVENT_CRC_IND_TO_SCH         6
#define EVENT_DL_RLC_BO_INFO_TO_SCH  7


/*macros*/
#define NO_SSB 0
#define SSB_TRANSMISSION 1
#define SSB_REPEAT 2
#define MAX_SSB_IDX 1 /* forcing it as 1 for now. Right value is 64 */

#define NO_SIB1 0
#define SIB1_TRANSMISSION 1
#define SIB1_REPITITION 2

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
#define MAX_NUM_LOGICAL_CHANNELS   11
/* can we have a common numslot numscs between mac sch */
#define MAX_SLOTS 10
#define MAX_SFN   1024
#define CCCH_LCID  0
#define ADD_DELTA_TO_TIME(crntTime, toFill, incr)          \
   if ((crntTime.slot + incr) > (MAX_SLOTS - 1))   \
      toFill.sfn = (crntTime.sfn + 1);      \
   else                                                  \
      toFill.sfn = crntTime.sfn;                              \
   toFill.slot = (crntTime.slot + incr) % MAX_SLOTS; \
   if (toFill.sfn >= MAX_SFN) \
   { \
      toFill.sfn%=MAX_SFN; \
   } \

/*structures*/


typedef enum
{
	RSP_OK,
	RSP_NOK
}schMacRsp;

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
   SSBPeriod   ssbPeriod;        /* SSB Periodicity in msec */
   uint8_t     ssbSubcOffset;    /* Subcarrier Offset(Kssb) */
   uint32_t    nSSBMask[SSB_MASK_SIZE];      /* Bitmap for actually transmitted SSB. */
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
} DmrsInfo;

typedef struct pdschFreqAlloc
{
   uint8_t  resourceAllocType;
   /* since we are using type-1, hence rbBitmap excluded */
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
   uint8_t coreSet0Size;
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
   CoresetCfg coreset0Cfg;

   uint16_t numDlDci;
   DlDCI    dci; /* as of now its only one DCI, later it will be numDlCi */
} PdcchCfg;
/* end of SIB1 PDCCH structures */

typedef struct
{
   /* parameters recieved from DU-APP */
   uint16_t sib1PduLen;
   uint16_t sib1NewTxPeriod;
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
   uint8_t searchSpaceId;
	uint8_t coresetId;
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

typedef struct schPdschCfgCmn
{
   uint8_t k0;
   uint8_t mappingType;
   uint8_t startSymbol;
   uint8_t lengthSymbol;
}SchPdschCfgCmn;

typedef struct schPucchCfgCmn
{
   uint8_t pucchResourceCommon;
   uint8_t pucchGroupHopping;
}SchPucchCfgCmn;

typedef struct schPuschCfgCmn
{
   uint8_t k2;
   uint8_t mappingType;
   uint8_t startSymbol;
   uint8_t lengthSymbol;
}SchPuschCfgCmn;

typedef struct schBwpDlCfg
{
   SchBwpParams   bwp;
	SchPdcchCfgCmn pdcchCommon;
	SchPdschCfgCmn pdschCommon;
}SchBwpDlCfg;

typedef struct schBwpUlCfg
{
   SchBwpParams   bwp;
	SchPucchCfgCmn pucchCommon;
	SchPuschCfgCmn puschCommon;
}SchBwpUlCfg;

typedef struct schCellCfg
{
   uint16_t    cellId;           /* Cell Id */
   uint16_t    phyCellId;        /* Physical cell id */
	uint8_t     bandwidth;        /* Supported B/W */
   DuplexMode  dupMode;          /* Duplex type: TDD/FDD */
	SchSsbCfg   ssbSchCfg;        /* SSB config */
	SchSib1Cfg  sib1SchCfg;       /* SIB1 config */
   SchRachCfg  schRachCfg;       /* PRACH config */
	SchBwpDlCfg schInitialDlBwp;  /* Initial DL BWP */
	SchBwpUlCfg schInitialUlBwp;  /* Initial UL BWP */
	uint8_t     puschMu;          /* PUSCH MU */
}SchCellCfg;

typedef struct schCellCfgCfm
{
   U16         cellId;     /* Cell Id */
   schMacRsp   rsp;   
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
	U8 sib1Trans;
	Sib1AllocInfo sib1Alloc;
}DlBrdcstAlloc;

typedef struct rarInfo
{
   uint16_t        raRnti;
	uint8_t         RAPID;
	uint16_t        ta;
	FreqDomainAlloc msg3FreqAlloc;
	uint16_t        tcrnti;
	uint8_t         rarPdu[8];
	uint8_t         rarPduLen;
}RarInfo;

typedef struct rarAlloc
{
   RarInfo rarInfo;
   BwpCfg  bwp;
   PdcchCfg rarPdcchCfg;
   PdschCfg rarPdschCfg;
}RarAlloc;

typedef struct msg4Info
{
   uint16_t crnti;
	uint8_t  ndi;
	uint8_t  harqProcNum;
	uint8_t  dlAssignIdx;
	uint8_t  pucchTpc;
	uint8_t  pucchResInd;
	uint8_t  harqFeedbackInd;
	uint8_t  dciFormatId;
   uint8_t  *msg4Pdu;
   uint16_t  msg4PduLen;
}Msg4Info;

typedef struct msg4Alloc
{
   Msg4Info msg4Info;
   BwpCfg bwp;
   PdcchCfg msg4PdcchCfg;
   PdschCfg msg4PdschCfg;
}Msg4Alloc;

typedef struct schSlotValue
{
	SlotIndInfo currentTime;
	SlotIndInfo broadcastTime;
	SlotIndInfo rarTime;
	SlotIndInfo msg4Time;
	SlotIndInfo dlMsgTime;
}SchSlotValue;

typedef struct dlSchedInfo
{
   uint16_t cellId;  /* Cell Id */
	SchSlotValue schSlotValue;

	/* Allocation for broadcast messages */
   bool isBroadcastPres;
	DlBrdcstAlloc brdcstAlloc;

	/* Allocation for RAR message */
	//uint8_t isRarPres;
	RarAlloc *rarAlloc;

   /* Allocation from MSG4 */
   Msg4Alloc *msg4Alloc;
}DlSchedInfo;

typedef struct tbInfo
{
   uint8_t  mcs;    /* MCS */
   uint8_t  ndi;    /* NDI */
   uint8_t  rv;     /* Redundancy Version */
   uint16_t tbSize; /* TB Size */
}TbInfo;

typedef struct schPuschInfo
{
   uint8_t          harqProcId;   /* HARQ Process ID */
   uint8_t          resAllocType; /* Resource allocation type */
   FreqDomainAlloc  fdAlloc;      /* Freq domain allocation */
   TimeDomainAlloc  tdAlloc;      /* Time domain allocation */
   TbInfo           tbInfo;       /* TB info */
}SchPuschInfo;

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
}SchPucchInfo;

typedef struct ulSchedInfo
{
   uint16_t      cellId;         /* Cell Id */
	uint16_t      crnti;          /* CRNI */
	SlotIndInfo   slotIndInfo;    /* Slot Info: sfn, slot number */
	uint8_t       dataType;       /* Type of info being scheduled */
	PrachSchInfo  prachSchInfo;   /* Prach scheduling info */
	SchPuschInfo  schPuschInfo;   /* Pusch scheduling info */
	SchPucchInfo  schPucchInfo;   /* Pusch scheduling info */
}UlSchedInfo;

typedef struct rachIndInfo
{
   uint16_t    cellId;
   uint16_t    crnti;
   SlotIndInfo timingInfo;
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
   SlotIndInfo timingInfo;
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
   uint16_t    numLc;
   BOInfo      boInfo[MAX_NUM_LOGICAL_CHANNELS];
}DlRlcBOInfo;


/* function pointers */

typedef int (*SchCellCfgCfmFunc)    ARGS((
   Pst            *pst,           /* Post Structure */                         
   SchCellCfgCfm  *schCellCfgCfm  /* Cell Cfg Cfm */
));

typedef int (*SchCellCfgFunc)    ARGS((
   Pst            *pst,           /* Post Structure */                         
   SchCellCfg  *schCellCfg     /* Cell Cfg  */
));

typedef int (*SchMacDlAllocFunc)     ARGS((                     
   Pst            *pst,       /* Post Structure */                         
   DlSchedInfo    *dlSchedInfo    /* dl allocation Info */                      
));

typedef int (*SchMacUlSchInfoFunc)     ARGS((                     
   Pst         *pst,           /* Post Structure */                         
   UlSchedInfo *ulSchedInfo         /* UL Alloc Sch  Info */                      
));

/* function declarations */
int packMacSchSlotInd(Pst *pst, SlotIndInfo *slotInd);
int packSchMacDlAlloc(Pst *pst, DlSchedInfo  *dlSchedInfo);
int packSchMacUlSchInfo(Pst *pst, UlSchedInfo *ulSchedInfo);
EXTERN int packSchCellCfg(Pst *pst, SchCellCfg  *schCellCfg);
EXTERN int packSchCellCfgCfm(Pst *pst, SchCellCfgCfm  *schCellCfgCfm);

EXTERN int MacProcDlAlloc(Pst *pst, DlSchedInfo *dlSchedInfo);
EXTERN int MacProcSchCellCfg(Pst *pst, SchCellCfg  *schCellCfg);
EXTERN int MacProcSchCellCfgCfm(Pst *pst, SchCellCfgCfm  *schCellCfgCfm);
EXTERN int SchHdlCellCfgReq(Pst *pst, SchCellCfg *schCellCfg);
EXTERN int schActvInit(Ent entity, Inst instId, Region region, Reason reason);
EXTERN S16 SchSendCfgCfm(Pst *pst, RgMngmt *cfm);
EXTERN int MacProcUlSchInfo(Pst *pst, UlSchedInfo *ulSchedInfo);
typedef int (*MacSchRachIndFunc)(Pst *pst, RachIndInfo *rachInd);
int packMacSchRachInd(Pst *pst, RachIndInfo *rachInd);
int macSchRachInd(Pst *pst, RachIndInfo *rachInd);
typedef int (*MacSchCrcIndFunc)(Pst *pst, CrcIndInfo *crcInd);
int packMacSchCrcInd(Pst *pst, CrcIndInfo *crcInd);
int macSchCrcInd(Pst *pst, CrcIndInfo *crcInd);
typedef uint8_t (*MacSchDlRlcBoInfoFunc)(Pst *pst, DlRlcBOInfo *dlBoInfo);
uint8_t packMacSchDlRlcBoInfo(Pst *pst, DlRlcBOInfo *dlBoInfo);
uint8_t macSchDlRlcBoInfo(Pst *pst, DlRlcBOInfo *dlBoInfo);


/**********************************************************************
  End of file
 **********************************************************************/

