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
#define EVENT_SCH_CELL_CFG 1
#define EVENT_SCH_CELL_CFG_CFM 2

/* selector */
#define MAC_SCH_LC_SELECTOR 0
#define MAC_SCH_TC_SELECTOR 1
#define MAC_SCH_LWLC_SELECTOR 2

/*macros*/
#define NO_SSB 0
#define SSB_TRANSMISSION 1
#define SSB_REPEAT 2
#define MAX_SSB_IDX 1 /* forcing it as 1 for now. Right value is 64 */

#define NO_SIB1 0
#define SIB1_TRANSMISSION 1
#define SIB1_REPITITION 2

#define MAX_PRG     1 /* max value should be later 275 */
#define MAX_DIG_BF_INTERFACES 0 /* max value should be later 255 */
#define MAX_CODEWORDS  1  /* max should be 2 */

/*structures*/


typedef enum
{
	RSP_OK,
	RSP_NOK
}schMacRsp;

typedef struct
{
   uint32_t    ssbPbchPwr;       /* SSB block power */
   uint8_t     scsCommon;           /* subcarrier spacing for common [0-3]*/
   uint8_t     ssbOffsetPointA;  /* SSB sub carrier offset from point A */
   SSBPeriod   ssbPeriod;        /* SSB Periodicity in msec */
   uint8_t     ssbSubcOffset;    /* Subcarrier Offset(Kssb) */
   uint32_t    nSSBMask[SSB_MASK_SIZE];      /* Bitmap for actually transmitted SSB. */
} SsbSchCfg;

/* SIB1 interface structure */
typedef struct bwpCfg
{
   uint16_t BWPSize;
   uint16_t BWPStart;
}BwpCfg;

typedef struct coresetCfg
{
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

typedef struct prgInfo
{
   uint16_t pmIdx;
   uint16_t beamIdx[MAX_DIG_BF_INTERFACES];
} PrgInfo;

typedef struct beamformingInfo
{
   uint16_t numPrgs;
   uint16_t prgSize;
   uint8_t  digBfInterfaces;
   PrgInfo  prg[MAX_PRG];
} BeamformingInfo;

typedef struct txPowerPdcchInfo
{
   uint8_t powerValue;
   uint8_t powerControlOffsetSS;
} TxPowerPdcchInfo;

typedef struct dlDci
{
   uint16_t rnti;
   uint16_t scramblingId;
   uint16_t scramblingRnti;
   uint8_t cceIndex;
   uint8_t aggregLevel;
   BeamformingInfo beamPdcchInfo;
   TxPowerPdcchInfo txPdcchPower;
} DlDci;

typedef struct sib1PdcchCfg
{
   BwpCfg sib1PdcchBwpCfg;

   uint8_t subcarrierSpacing;
   uint8_t cyclicPrefix;

   /* coreset-0 configuration */
   CoresetCfg sib1Coreset0Cfg;

   uint16_t numDlDci;
   DlDci    sib1DlDci; /* as of now its only one DCI, later it will be numDlCi */
} Sib1PdcchCfg;
/* end of SIB1 PDCCH structures */

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
   uint8_t  resourceAlloc;
   /* since we are using type-1, hence rbBitmap excluded */
   uint16_t rbStart;
   uint16_t rbSize;
   uint8_t  vrbPrbMapping;
} PdschFreqAlloc;

typedef struct pdschTimeAlloc
{
   uint8_t startSymbolIndex;
   uint8_t numSymbols;
} PdschTimeAlloc;

typedef struct txPowerPdschInfo
{
   uint8_t powerControlOffset;
   uint8_t powerControlOffsetSS;
} TxPowerPdschInfo;

typedef struct sib1PdschCfg
{
   uint16_t pduBitmap;
   uint16_t rnti;
   uint16_t pduIndex;
   BwpCfg sib1PdschBwpCfg;
   uint8_t numCodewords;
   CodewordInfo codeword[MAX_CODEWORDS];
   uint16_t dataScramblingId;
   uint8_t  numLayers;
   uint8_t  transmissionScheme;
   uint8_t  refPoint;
   DmrsInfo dmrs;
   PdschFreqAlloc sib1FreqAlloc;
   PdschTimeAlloc sib1TimeAlloc;
   BeamformingInfo beamPdschInfo;
   TxPowerPdschInfo txPdschPower;
} Sib1PdschCfg;
/* SIB1 PDSCH structures end */

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
   Sib1PdcchCfg sib1PdcchCfg;
   Sib1PdschCfg sib1PdschCfg;
} Sib1SchCfg;

typedef struct schCellCfg
{
   U16         cellId;     /* Cell Id */
   U16         phyCellId;  /* Physical cell id */
	U8          bandwidth;  /* Supported B/W */
   DuplexMode  dupMode;    /* Duplex type: TDD/FDD */
	SsbSchCfg   ssbSchCfg;
	Sib1SchCfg  sib1SchCfg;
}SchCellCfg;

typedef struct schCellCfgCfm
{
   U16         cellId;     /* Cell Id */
   schMacRsp   rsp;   
}SchCellCfgCfm;

typedef struct timeDomainAlloc
{
   uint16_t ssbStartSymbIdx;
	uint16_t ssbSymbolDuration;
}TimeDomainAlloc;

typedef struct freqDomainAlloc
{
   uint16_t ssbStartPrbIdx;
   uint16_t ssbPrbDuration;
}FreqDomainAlloc;

typedef struct ssbInfo
{
   uint8_t ssbIdx;          /* SSB Index */
	TimeDomainAlloc tdAlloc; /* Time domain allocation */
	FreqDomainAlloc fdAlloc; /* Freq domain allocation */
}SsbInfo;

typedef struct sib1AllocInfo
{
   Sib1PdcchCfg sib1PdcchCfg;
   Sib1PdschCfg sib1PdschCfg;
} Sib1AllocInfo;

/* Interface structure signifying DL broadcast allocation for SSB, SIB1 */
typedef struct dlBrdcstAlloc
{
   uint16_t cellId;  /* Cell Id */
	SlotIndInfo slotIndInfo; /* Slot Info: sfn, slot number */
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

/* function pointers */

typedef int (*SchMacDlBrdcstAllocFunc)     ARGS((                     
   Pst            *pst,           /* Post Structure */                         
   DlBrdcstAlloc  *dlBrdcstAlloc    /* slot ind Info */                      
));

typedef int (*SchCellCfgCfmFunc)    ARGS((
   Pst            *pst,           /* Post Structure */                         
   SchCellCfgCfm  *schCellCfgCfm  /* Cell Cfg Cfm */
));

typedef int (*SchCellCfgFunc)    ARGS((
   Pst            *pst,           /* Post Structure */                         
   SchCellCfg  *schCellCfg     /* Cell Cfg  */
));
/* function declarations */
int packMacSchSlotInd(Pst *pst, SlotIndInfo *slotInd);
int packSchMacDlBrdcstAlloc(Pst *pst, DlBrdcstAlloc  *dlBrdcstAlloc);
EXTERN int packSchCellCfg(Pst *pst, SchCellCfg  *schCellCfg);
EXTERN int packSchCellCfgCfm(Pst *pst, SchCellCfgCfm  *schCellCfgCfm);

EXTERN int MacProcDlBrdcstAlloc(Pst *pst, DlBrdcstAlloc *dlBrdcstAlloc);
EXTERN int MacProcSchCellCfg(Pst *pst, SchCellCfg  *schCellCfg);
EXTERN int MacProcSchCellCfgCfm(Pst *pst, SchCellCfgCfm  *schCellCfgCfm);
EXTERN int SchHdlCellCfgReq(Pst *pst, SchCellCfg *schCellCfg);
EXTERN int schActvInit(Ent entity, Inst instId, Region region, Reason reason);
EXTERN S16 SchSendCfgCfm(Pst *pst, RgMngmt *cfm);

/**********************************************************************
  End of file
 **********************************************************************/

