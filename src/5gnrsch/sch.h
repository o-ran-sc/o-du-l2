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

/* macros */
#define SCH_MU0_NUM_SLOTS 10 
#define SCH_MU1_NUM_SLOTS 20 
#define SCH_MU2_NUM_SLOTS 30 
#define SCH_MU3_NUM_SLOTS 40 
#define SCH_MU4_NUM_SLOTS 50 
#define SCH_MAX_SFN 1024
#define SCH_MIB_TRANS 8  /* MIB transmission as per 38.331 is every 80 ms */
#define SCH_SIB1_TRANS 16 /* SIB1 transmission as per 38.331 is every 160 ms */
#define SCH_NUM_SC_PRB 12 /* number of SCs in a PRB */
#define SCH_MAX_SSB_BEAM 8 /* since we are supporting only SCS=15KHz and 30KHz */
#define SCH_SSB_NUM_SYMB 4
#define SCH_SSB_NUM_PRB 21 /* One extra PRB as buffer */
#define SCHED_DELTA 1
#define BO_DELTA 1
#define RAR_DELAY   2
#define MSG4_DELAY  1
#define PDSCH_START_RB 10
/* Considering pdsch region from 3 to 13, DMRS exclued.
 * Overlapping of PDSCH DRMS and PDSCH not supported by Intel L1 */
#define NUM_PDSCH_SYMBOL 11
#define PUSCH_START_RB 15
#define PUCCH_NUM_PRB_FORMAT_0_1_4 1  /* number of PRBs in freq domain, spec 38.213 - 9.2.1 */
#define SI_RNTI 0xFFFF
#define P_RNTI  0xFFFE
#define DMRS_MAP_TYPE_A 1
#define NUM_DMRS_SYMBOLS 1
#define DMRS_ADDITIONAL_POS 0
#define SCH_DEFAULT_K1 1
#define SSB_IDX_SUPPORTED 1

#define CRC_FAILED 0
#define CRC_PASSED 1

#define MAC_HDR_SIZE  3   /* 3 bytes of MAC Header */
#define UL_GRANT_SIZE 224

#define PRB_BITMAP_IDX_LEN 64
#define PRB_BITMAP_MAX_IDX ((MAX_NUM_RB + PRB_BITMAP_IDX_LEN-1) / PRB_BITMAP_IDX_LEN)

#define SCH_MAX_NUM_UL_HQ_PROC 16
#define SCH_MAX_NUM_DL_HQ_PROC 16
#define SCH_MAX_NUM_MSG3_TX 2
#define SCH_MAX_NUM_DL_HQ_TX 3
#define SCH_MAX_NUM_UL_HQ_TX 3
#define SCH_MAX_NUM_MSG4_TX 2
#define HQ_ACK 0
#define HQ_NACK 1
#define HQ_DTX 2
#define ROOT_SEQ_LEN_1 139
#define ROOT_SEQ_LEN_2 839

#ifdef NR_DRX
/* As per 38.331 the largest offset which can be used in of size 10240.
 * But using this much size of array can cause memory related issue so thats why
 * taking this size which are a multiple of the larger size */
#define MAX_DRX_SIZE 512
#endif

#define NUM_SCH_TYPE 2  /*Supported number of Scheduler Algorithm types*/

#define SCH_TQ_SIZE 10

/*3GPP 38.331,'frequencyDomainResources' :Number of PRBs per Resource Block Group*/
#define NUM_PRBS_PER_RBG 6 

/*3GPP 38.214 Table 5.2.2.1-2*/
#define MAX_NUM_CQI_IDX 16

/*3GPP 38.211 Table 7.3.2.1-1*/
#define MAX_NUM_AGG_LVL 5

typedef struct schDlHqProcCb SchDlHqProcCb;
typedef struct schUlHqEnt SchUlHqEnt;
typedef struct schRaReq SchRaReq;
typedef struct schDlHqEnt SchDlHqEnt;
typedef struct schCellCb SchCellCb;
typedef struct schUeCb SchUeCb;

typedef enum
{
   SCH_FCFS,
   SCH_SLICE_BASED
}SchType;

typedef enum
{
   SCH_NUMEROLOGY_0,
   SCH_NUMEROLOGY_1,
   SCH_NUMEROLOGY_2,
   SCH_NUMEROLOGY_3,
   SCH_NUMEROLOGY_4
}SchNumerology;

typedef enum
{
   SCH_UE_STATE_INACTIVE,
   SCH_UE_STATE_ACTIVE,
   SCH_UE_HANDIN_IN_PROGRESS
}SchUeState;

typedef enum
{
   SCH_RA_STATE_MSG2_HANDLE,
   SCH_RA_STATE_MSG3_PENDING,
   SCH_RA_STATE_MSG4_PENDING,
   SCH_RA_STATE_MSG4_DONE
}SchRaState;

typedef enum
{
   SCH_LC_STATE_INACTIVE,
   SCH_LC_STATE_ACTIVE
}SchLcState;

typedef enum
{
   WINDOW_YET_TO_START,
   WITHIN_WINDOW,
   WINDOW_EXPIRED
}RaRspWindowStatus;

typedef enum
{
   HQ_TB_ACKED=0,
   HQ_TB_NACKED,
   HQ_TB_WAITING
}SchHqTbState;

#ifdef NR_TDD
typedef enum
{
   DL_SLOT,
   UL_SLOT,
   FLEXI_SLOT
}SlotConfig;

typedef enum
{
   DL_SYMBOL,
   UL_SYMBOL,
   FLEXI_SYMBOL
}SchSymbolConfig;
#endif

/*Following structures to keep record and estimations of PRB allocated for each
 * LC taking into consideration the RRM policies*/
typedef struct lcInfo
{
   uint8_t  lcId;     /*LCID for which BO are getting recorded*/
   uint32_t reqBO;    /*Size of the BO requested/to be allocated for this LC*/
   uint32_t allocBO;  /*TBS/BO Size which is actually allocated*/
   uint8_t  allocPRB; /*PRB count which is allocated based on RRM policy/FreePRB*/
}LcInfo;

typedef struct schUlHqTbCb
{
   uint32_t               tbSzReq;
   uint32_t               tbSzAllc;
   uint8_t                ndi;
   uint8_t                rv;
   uint8_t                rvIdx;
   uint8_t                qamOrder;
   SchMcsTable            mcsTable;
   uint8_t                iMcs;
   uint8_t                iMcsInDci;
   uint8_t                numLyrs;
   uint8_t                txCntr;
   SchHqTbState           state;
   uint8_t                cntrRetxAllocFail;
   uint8_t                statsBitmap;
}SchUlHqTbCb;

typedef struct schDlHqTbCb
{
   uint8_t                tbIdx;
   Bool                   isEnabled;
   uint32_t               tbSzReq;
   uint8_t                txCntr;
   uint8_t                ndi;
   uint8_t                rv;
   uint8_t                rvIdx;
   uint8_t                iMcs;
   uint8_t                iMcsInDci;
   uint8_t                numLyrs;
   SchHqTbState           state;
   uint8_t                isAckNackDtx;
   uint8_t                cntrRetxAllocFail;
   //InfUeTbInfo          tbCompInfo;
   uint8_t                statsBitmap;
}SchDlHqTbCb;

#ifdef NR_DRX
typedef struct schDrxHarqCb
{
   uint32_t     rttExpIndex;
   CmLList      *rttExpNode;
   uint32_t     retxStrtIndex; 
   CmLList      *retxStrtNode;
   uint32_t     retxExpIndex;
   CmLList      *retxExpNode;
}SchDrxHarqCb;
#endif

typedef struct schUlHqProcCb
{
   uint8_t           procId;       /*!< HARQ Process ID */
   SchUlHqEnt        *hqEnt;
   uint8_t           maxHqTxPerHqP;
   SchUlHqTbCb       tbInfo;
   CmLList           ulHqEntLnk;
   CmLList           ulSlotLnk;
   uint8_t           strtSymbl;
   uint8_t           numSymbl;
   void              *schSpcUlHqProcCb;  /*!< Scheduler specific HARQ Proc CB */
   CmLList           ulHqProcLink;
   uint8_t           puschResType;       /*!< Resource allocation type */
   uint16_t          puschStartPrb;
   uint16_t          puschNumPrb;
   uint8_t           dmrsMappingType;
   uint8_t           nrOfDmrsSymbols;
   uint8_t           dmrsAddPos;
   SlotTimingInfo    puschTime;
#ifdef NR_DRX
   SchDrxHarqCb      ulDrxHarqCb;
#endif
}SchUlHqProcCb;

struct schDlHqProcCb
{
   uint8_t           procId;       /*!< HARQ Process ID */
   SchDlHqEnt        *hqEnt;
   uint8_t           maxHqTxPerHqP;
   CmLList           dlHqEntLnk;
   CmLList           ulSlotLnk;
   SchDlHqTbCb       tbInfo[2];
   uint8_t           k1;
   void              *schSpcDlHqProcCb;  /*!< Scheduler specific HARQ Proc CB */
   CmLList           dlHqProcLink;
   SlotTimingInfo    pucchTime;
#ifdef NR_DRX
   SchDrxHarqCb      dlDrxHarqCb;
#endif
};
struct schUlHqEnt
{
   SchCellCb      *cell;     /*!< Contains the pointer to cell*/
   SchUeCb        *ue;       /*!< Contains the pointer to ue*/
   CmLListCp      free;      /*!< List of free HARQ processes */
   CmLListCp      inUse;     /*!< List of in-use HARQ processes */
   uint8_t        maxHqTx;   /*!< Maximum number of harq re-transmissions */
   uint8_t        numHqPrcs; /*!< Number of HARQ Processes */
   SchUlHqProcCb  procs[SCH_MAX_NUM_UL_HQ_PROC]; /*!< Uplink harq process info */
};
struct schDlHqEnt
{
   SchCellCb      *cell;     /*!< Contains the pointer to cell */
   SchUeCb        *ue;       /*!< Contains the pointer to UE */
   CmLListCp      free;      /*!< List of free HARQ processes */
   CmLListCp      inUse;     /*!< List of in-use HARQ processes */
   uint8_t        maxHqTx;   /*!< Maximum number of harq transmissions */
   uint8_t        numHqPrcs; /*!< Number of HARQ Processes */
   SchDlHqProcCb  procs[SCH_MAX_NUM_DL_HQ_PROC];/*!< Downlink harq processes */
};

/**
 * @brief
 * Structure holding LTE MAC's General Configuration information.
 */
typedef struct schGenCb
{
   uint8_t         startCellId;      /*!< Starting Cell Id */
#ifdef LTE_ADV
   bool            forceCntrlSrbBoOnPCel; /*!< value 1 means force scheduling
					    of RLC control BO and SRB BO on
					    PCell. val 0 means don't force*/
   bool            isSCellActDeactAlgoEnable; /*!< TRUE will enable activation/deactivation algo at Schd */
#endif
}SchGenCb;

typedef struct freePrbBlock
{
   uint16_t numFreePrb;
   uint16_t startPrb;
   uint16_t endPrb;
}FreePrbBlock;

/**
 * @brief
 * PRB allocations for a symbol within a slot
 */
typedef struct schPrbAlloc
{
   CmLListCp freePrbBlockList;           /*!< List of continuous blocks for available PRB */
   uint64_t  prbBitMap[ MAX_SYMB_PER_SLOT][PRB_BITMAP_MAX_IDX];  /*!< BitMap to store the allocated PRBs */
   uint16_t  numPrbAlloc;
}SchPrbAlloc;


typedef struct schPdcchAllocInfo
{
   uint8_t  cRSetId;
   uint8_t  ssId;
   uint8_t  aggLvl;
   uint16_t cceIndex; 
}SchPdcchAllocInfo;

/**
 * @brief
 * scheduler allocationsfor DL per cell.
 */
typedef struct schDlSlotInfo
{
   SchPrbAlloc  prbAlloc;                 /*!< PRB allocated/available in this slot */
   bool         ssbPres;                  /*!< Flag to determine if SSB is present in this slot */
   uint8_t      ssbIdxSupported;          /*!< Max SSB index */
   SsbInfo      ssbInfo[MAX_SSB_IDX];     /*!< SSB info */
   bool         sib1Pres;                 /*!< Flag to determine if SIB1 is present in this slot */
   uint8_t      pdcchUe;                  /*!< UE for which PDCCH Common is scheduled in this slot */
   RarAlloc     *rarAlloc[MAX_NUM_UE];    /*!< RAR allocation per UE*/
   DciInfo      *ulGrant;
   DlMsgSchInfo *dlMsgAlloc[MAX_NUM_UE];  /*!< Dl msg allocation per UE*/
}SchDlSlotInfo;

typedef struct schRaCb
{
   uint8_t   ueId;
   bool      msg4recvd;
   uint16_t  tcrnti;
   uint16_t  dlMsgPduLen;
   SchUlHqProcCb msg3HqProc;
   SchUlHqProcCb *retxMsg3HqProc;
   SchRaState raState;
   SchCellCb *cell;
   SchRaReq  *raReq;
}SchRaCb;

/**
 * @brief
 * scheduler allocationsfor UL per cell.
 */
typedef struct schUlSlotInfo
{
   SchPrbAlloc  prbAlloc;         /*!< PRB allocated/available per symbol */
   uint8_t      puschCurrentPrb;  /*!< Current PRB for PUSCH allocation */
   bool         puschPres;        /*!< PUSCH presence field */
   SchPuschInfo *schPuschInfo;    /*!< PUSCH info */
   bool         pucchPres;        /*!< PUCCH presence field */
   SchPucchInfo schPucchInfo;     /*!< PUCCH info */
   uint8_t      pucchUe;          /*!< Store UE id for which PUCCH is scheduled */
   uint8_t      puschUe;          /*!< Store UE id for which PUSCH is scheduled */
}SchUlSlotInfo;

/**
@brief
* BSR info per slot per UE.
*/
typedef struct bsrInfo
{
   uint8_t    priority;  /* CG priority */
   uint32_t   dataVol;   /* Data volume requested in bytes */
}BsrInfo;

typedef struct schLcCtxt
{
   uint8_t lcId;     // logical Channel ID
   uint8_t lcp;      // logical Channel Prioritization
   SchLcState lcState;
   uint32_t bo;
   uint16_t   pduSessionId; /*Pdu Session Id*/
   Snssai  *snssai;      /*S-NSSAI assoc with LCID*/
   bool isDedicated;     /*Flag containing Dedicated S-NSSAI or not*/
   uint16_t rsvdDedicatedPRB;
}SchDlLcCtxt;

typedef struct schDlCb
{
   SchDlLcCtxt   dlLcCtxt[MAX_NUM_LC];
}SchDlCb;

typedef struct schUlLcCtxt
{
   SchLcState  lcState;
   uint8_t lcId;
   uint8_t priority;
   uint8_t lcGroup;
   uint8_t schReqId;
   uint8_t pbr;        // prioritisedBitRate
   uint8_t bsd;        // bucketSizeDuration
   uint16_t   pduSessionId; /*Pdu Session Id*/
   Snssai  *snssai;      /*S-NSSAI assoc with LCID*/
   bool isDedicated;     /*Flag containing Dedicated S-NSSAI or not*/
   uint16_t rsvdDedicatedPRB;
}SchUlLcCtxt;

typedef struct schUlCb
{
   SchUlLcCtxt ulLcCtxt[MAX_NUM_LC];
}SchUlCb;

typedef struct schUeCfgCb
{
   uint16_t        cellId;
   uint8_t         ueId;
   uint16_t        crnti;
   bool macCellGrpCfgPres;
   SchMacCellGrpCfg   macCellGrpCfg;
   bool phyCellGrpCfgPres;
   SchPhyCellGrpCfg   phyCellGrpCfg;
   bool spCellCfgPres;
   SchSpCellRecfg       spCellCfg;
   SchAmbrCfg         *ambrCfg;
   SchModulationInfo  dlModInfo;
   SchModulationInfo  ulModInfo;
   SchDataTransmission dataTransmissionAction;
}SchUeCfgCb;

typedef struct schHqDlMap
{
   CmLListCp hqList;
}SchHqDlMap;

typedef struct schHqUlMap
{
   CmLListCp hqList;
}SchHqUlMap;

#ifdef NR_DRX
typedef struct  schDrxUeCb
{
   bool      drxDlUeActiveStatus;       /* Final Dl Ue status which is marked as true if drxDlUeActiveMask or drxDlUeActiveMaskForHarq is present */
   bool      drxUlUeActiveStatus;       /* Final Ul Ue status which is marked as true if drxUlUeActiveMask or drxUlUeActiveMaskForHarq is present */
   uint32_t  drxDlUeActiveMask;          /* variable is used to store the status about downlink active status of Ue for On-duration, inactive timer*/
   uint32_t  drxUlUeActiveMask;          /* variable is used to store the status about uplink active status for on-duration inactive timer*/
   uint32_t  drxDlUeActiveMaskForHarq;   /* variable is used to store the status about downlink active status for harq*/
   uint32_t  drxUlUeActiveMaskForHarq;   /* variable is used to store the status about uplink active status for harq */
   uint32_t  onDurationLen;          /* length of on duration which is received from ue cfg/recfg in form of ms and subms, informs about after how many slots on duration gets expire */
   uint32_t  inActvTimerLen;         /* length of inActvTimer value received from ue cfg/recfg in form of ms, informs about after how many slots in active gets expire */
   uint8_t   harqRttDlTimerLen;      /* length of harqRttDlTimer received from ue cfg/recfg in form of symbols, inform about after how many slots on the harq drx-HARQ-RTT-TimerDL expire */
   uint8_t   harqRttUlTimerLen;      /* length of harqRttUlTimer received from ue cfg/recfg in form of symbols,informs about after how many slots on harq drx-HARQ-RTT-TimerUL expire*/
   uint32_t  retransDlTimerLen;      /* length of retransDlTimer received from ue cfg/recfg in form of slot, informs about after how many slots on harq RetransmissionTimer dl timer expire*/
   uint32_t  retransUlTimerLen;      /* length of retransUlTimer received from ue cfg/recfg in form of slot, informs about after how many slots on harq RetransmissionTimer ul timer expire*/
   uint32_t  longCycleLen;           /* length of long Cycle value received from ue cfg/recfg in form of ms*/
   bool      longCycleToBeUsed;      /* long cycle should be used once the short cycle gets expires */
   uint32_t  drxStartOffset;         /* length of drxStartOffset value received from ue cfg/recfg in form of ms, which helps in getting on duration start point*/
   bool      shortCyclePresent;      /* set this value if shortCycle is Present */
   uint32_t  shortCycleLen;          /* length of short Cycle value received from ue cfg/recfg in form of ms*/
   uint32_t  shortCycleTmrLen;       /* value shortCycleTmr is the multiple of shortCycle which is received from ue cfg/recfg in form of integer*/
   uint32_t  drxSlotOffset;          /* drxSlotOffset value received from ue cfg/recfg which is used to delay before starting the drx-onDuration*/
   uint32_t  onDurationStartIndex;   /* Index at which UE is stored in onDuration starts list */
   uint32_t  onDurationExpiryIndex;  /* Index at which UE is stored in onDuration expires in the list */ 
   uint32_t  inActvExpiryIndex;      /* Index at which UE is stored in inActvTimer expires in the list */
   uint32_t  shortCycleExpiryIndex;  /* Index at which UE is stored in shortCycle expires in the list */
   int32_t   shortCycleDistance;     /* Distance after how many slot short cycle tmr gets expire */ 
   int32_t   onDurationStartDistance;/* Distance after how many slot on Duration Start tmr gets expire */
   int32_t   onDurationExpiryDistance;/* Distance after how many slot on Duration tmr gets expire */
   int32_t   inActiveTmrExpiryDistance;/* Distance after how many slot inActive tmr gets expire */
   CmLList   *onDurationStartNodeInfo; /* Node present in on duration start list*/
   CmLList   *onDurationExpiryNodeInfo;/* Node present in on duration exp list*/
   CmLList   *inActvTimerExpiryNodeInfo; /* Node present in in active exp list*/
   CmLList   *shortCycleTmrExpiryNodeInfo; /* Node present in short cycle exp list*/
}SchDrxUeCb;
#endif

typedef struct schPdcchInfo
{
   SchControlRsrcSet *cRSetRef; /*Coreset Cfg reference from SchUeCfgCb*/
   SchSearchSpace    *ssRef;    /*SearchSpace Cfg reference from SchUeCfgCb*/
   uint16_t          totalPrbs; /*Total PRBs configured for this CORESET*/
   uint8_t           nrOfPRBPerCce; /*CCE Size*/
   uint8_t           totalCceCount; /*Count of CCE in this CORESET*/
   uint8_t           cqiIndxAggLvlMap[MAX_NUM_CQI_IDX];/*Agg Level to be used for each CQI Index*/
   uint32_t          *y; /*Coefficient variable to calculate CCE Index as per 3gpp Spec 38.213 Sec 10.1*/
}SchPdcchInfo;

/**
 * @brief
 * UE control block
 */
typedef struct schUeCb
{
   uint16_t   ueId;
   uint16_t   crnti;
   SchUeCfgCb ueCfg;
   SchUeState state;
   SchCellCb  *cellCb;
   SchCfraResource cfraResource;
   bool       srRcvd;
   bool       bsrRcvd;
   BsrInfo    bsrInfo[MAX_NUM_LOGICAL_CHANNEL_GROUPS];
   SchUlCb    ulInfo;
   SchDlCb    dlInfo;
   SchUlHqEnt ulHqEnt;
   SchDlHqEnt dlHqEnt;
   SchDlHqProcCb *msg4HqProc;
   SchDlHqProcCb *retxMsg4HqProc;
   SchHqDlMap    **hqDlmap;
   SchHqUlMap    **hqUlmap;
   void          *schSpcUeCb;
#ifdef NR_DRX
   bool          ueDrxInfoPres;
   SchDrxUeCb    drxUeCb;
#endif
   bool                 k0K1TblPrsnt;
   SchK0K1TimingInfoTbl k0K1InfoTbl;
   bool                 k2TblPrsnt;
   SchK2TimingInfoTbl   k2InfoTbl;
   SchPdcchInfo         pdcchInfo[MAX_NUM_CRSET];
}SchUeCb;

/**
 * @brief
 * RA Request Info
 */
typedef struct schRaReq
{
   uint32_t        raRnti;
   RachIndInfo     *rachInd;
   bool            isCFRA;
   SchUeCb         *ueCb;          /* Filled only if isCFRA = true */
   SlotTimingInfo  winStartTime;
   SlotTimingInfo  winEndTime;
}SchRaReq;

typedef struct schPageInfo
{
  uint16_t       pf;          /*Value of Paging Frame received from DUAPP*/
  uint8_t        i_s;         /*Value of Paging Occ Index received from DUAPP*/
  SlotTimingInfo pageTxTime;  /*Start Paging window*/
  uint8_t        mcs;         /*MCS index*/
  uint16_t       msgLen;      /*Pdu length */
  uint8_t       *pagePdu;     /*RRC Page PDU bit string*/
}SchPageInfo;

typedef struct schPagingOcc
{
  uint8_t frameOffset;
  uint8_t pagingOccSlot;
}SchPagingOcc;

typedef struct schPageCb
{
   CmLListCp    pageIndInfoRecord[MAX_SFN]; /*List of Page Records received which are stored per sfn*/
   SchPagingOcc pagMonOcc[MAX_PO_PER_PF];   /*Paging Occasion Slot/FrameOffset are stored*/ 
}SchPageCb;

#ifdef NR_DRX
typedef struct schDrxCb
{
   CmLListCp   onDurationStartList;   /*!< Tracks the start of onDuration Timer. */
   CmLListCp   onDurationExpiryList;   /*!< Tracks the Expiry of onDuration Timer. */
   CmLListCp   inActvTmrExpiryList;   /*!< Tracks the Expiry of drx-InactivityTimer. */
   CmLListCp   shortCycleExpiryList;   /*!< Tracks the Expiry of DRX Short Cycle. */
   CmLListCp   dlHarqRttExpiryList;   /*!< Tracks the Expiry of DL HARQ RTT timer. */
   CmLListCp   dlRetransExpiryList;   /*!< Tracks the Expiry of DL Re-Transmission timer. */
   CmLListCp   ulHarqRttExpiryList;   /*!< Tracks the Expiry of UL HARQ RTT timer. */
   CmLListCp   ulRetransExpiryList;   /*!< Tracks the Expiry of UL Re-Transmission timer. */
   CmLListCp   dlRetransTmrStartList; /*!< It has list of DL harq procs for */
   CmLListCp   ulRetransTmrStartList; /*!< It has list of UL harq procs for */
}SchDrxCb;
#endif

typedef struct schAllApis
{
   uint8_t (* SchCellCfgReq)(SchCellCb *cellCb);
   void (* SchCellDeleteReq)(SchCellCb *cellCb);
   uint8_t (* SchAddUeConfigReq)(SchUeCb  *ueCb);
   void (* SchModUeConfigReq)(SchUeCb  *ueCb);
   void (* SchUeDeleteReq)(SchUeCb  *ueCb);
   void (* SchDlHarqInd)();
   void (* SchPagingInd)();
   void (* SchRachRsrcReq)();
   void (* SchRachRsrcRel)();
   void (* SchCrcInd)(SchCellCb *cellCb, uint16_t ueId);
   void (* SchRachInd)(SchCellCb *cellCb, uint16_t ueId);
   void (* SchDlRlcBoInfo)(SchCellCb *cellCb, uint16_t ueId);
   void (* SchSrUciInd)(SchCellCb *cellCb, uint16_t ueId);
   void (* SchBsr)(SchCellCb *cellCb, uint16_t ueId);
   void (* SchHandleLcList)(void *ptr, CmLList *node, ActionTypeLL action);
   void (* SchAddToDlHqRetxList)(SchDlHqProcCb *hqP);
   void (* SchAddToUlHqRetxList)(SchUlHqProcCb *hqP);
   void (* SchRemoveFrmDlHqRetxList)(SchUeCb *ueCb, CmLList *node);
   void (* SchRemoveFrmUlHqRetxList)(SchUeCb *ueCb, CmLList *node);
   uint8_t (* SchAddUeToSchedule)(SchCellCb *cellCb, uint16_t ueId);
   void (* SchRemoveUeFrmScheduleLst)(SchCellCb *cell, CmLList *node);
   uint8_t (* SchInitDlHqProcCb)(SchDlHqProcCb *hqP);
   uint8_t (* SchInitUlHqProcCb)(SchUlHqProcCb *hqP);
   void (* SchFreeDlHqProcCb)(SchDlHqProcCb *hqP);
   void (* SchFreeUlHqProcCb)(SchUlHqProcCb *hqP);
   void (* SchDeleteDlHqProcCb)(SchDlHqProcCb *hqP);
   void (* SchDeleteUlHqProcCb)(SchUlHqProcCb *hqP);
   void (* SchScheduleSlot)(SchCellCb *cell, SlotTimingInfo *slotInd, Inst schInst);
   uint32_t (* SchScheduleDlLc)(SlotTimingInfo pdcchTime, SlotTimingInfo pdschTime, uint8_t pdschNumSymbols, \
      uint16_t *startPrb, bool isRetx, SchDlHqProcCb **hqP);
   uint8_t (* SchScheduleUlLc)(SlotTimingInfo dciTime, SlotTimingInfo puschTime, uint8_t startStmb, \
      uint8_t symbLen, bool isRetx, SchUlHqProcCb **hqP);
}SchAllApis;

typedef struct schHqCfgParam
{
   uint8_t maxDlDataHqTx;
   uint8_t maxMsg4HqTx;
   uint8_t maxUlDataHqTx;
}SchHqCfg;

typedef struct
{
   /* parameters derived in scheduler */
   uint8_t   n0;
   BwpCfg    bwp;
   PdcchCfg  sib1PdcchCfg;
   PdschCfg  sib1PdschCfg;
}SchSib1Cfg;

typedef struct dlTotalPrbUsage
{
   uint16_t numPrbUsedForTx;
   uint16_t totalPrbAvailForTx;
}TotalPrbUsage;

typedef struct 
{
   TotalPrbUsage *dlTotalPrbUsage;
   TotalPrbUsage *ulTotalPrbUsage;
}SchKpiSupported;

typedef struct
{
   CmLListCp  dlTotPrbUseList;
   CmLListCp  ulTotPrbUseList;
}SchKpiActive;

typedef struct schStatsGrp
{
   Inst       schInst;
   uint64_t   subscriptionId;
   uint8_t    groupId;
   uint16_t   periodicity;  /* In milliseconds */
   CmTimer    periodTimer;
   SchKpiSupported kpiStats;
}SchStatsGrp;

typedef struct schStatistics
{
   CmLListCp     statsGrpList;
   SchKpiActive  activeKpiList;
}SchStatistics;

/**
 * @brief
 * Cell Control block per cell.
 */
typedef struct schCellCb
{
   uint16_t      cellId;                            /*!< Cell ID */
   Inst          instIdx;                           /*!< Index of the scheduler instance */
   Inst          macInst;                           /*!< Index of the MAC instance */
   uint16_t       numSlots;                          /*!< Number of slots in current frame */
   SlotTimingInfo   slotInfo;                          /*!< SFN, Slot info being processed*/
   SchDlSlotInfo **schDlSlotInfo;                   /*!< SCH resource allocations in DL */
   SchUlSlotInfo **schUlSlotInfo;                   /*!< SCH resource allocations in UL */
   SchCellCfg    cellCfg;                           /*!< Cell ocnfiguration */
   uint8_t       numerology;
   bool          firstSsbTransmitted;
   bool          firstSib1Transmitted;
   uint8_t       ssbStartSymbArr[SCH_MAX_SSB_BEAM]; /*!< start symbol per SSB beam */
   uint64_t      dedPreambleBitMap;                 /*!< Bit map to find used/free preambles index */
   SchRaReq      *raReq[MAX_NUM_UE];                /*!< Pending RA request */
   SchRaCb       raCb[MAX_NUM_UE];                  /*!< RA Cb */
   uint16_t      numActvUe;                         /*!< Number of active UEs */
   uint32_t      actvUeBitMap;                      /*!< Bit map to find active UEs */
   uint32_t      boIndBitMap;                       /*!< Bit map to indicate UEs that have recevied BO */
   SchUeCb       ueCb[MAX_NUM_UE];                  /*!< Pointer to UE contexts of this cell */
   SchPageCb     pageCb;                            /*!< Page Record at Schedular*/
#ifdef NR_TDD
   uint8_t       numSlotsInPeriodicity;             /*!< number of slots in configured periodicity and SCS */
   uint32_t      slotFrmtBitMap;                    /*!< 2 bits must be read together to determine D/U/S slots. 00-D, 01-U, 10-S */
   SchSymbolConfig       slotCfg[MAX_TDD_PERIODICITY_SLOTS][MAX_SYMB_PER_SLOT];
#endif
#ifdef NR_DRX
   SchDrxCb      drxCb[MAX_DRX_SIZE];                           /*!< Drx cb*/
#endif
   SchType       schAlgoType;                       /*!< The scheduler type which the cell is configured with.*/
   SchAllApis    *api;                             /*!< Reference of sch APIs for this cell based on the SchType*/
   void          *schSpcCell;                       /*Ref of Scheduler specific structure*/
   SchHqCfg             schHqCfg;
   SchK0K1TimingInfoTbl k0K1InfoTbl;
   SchK2TimingInfoTbl   msg3K2InfoTbl;
   SchK2TimingInfoTbl   k2InfoTbl;
   SchSib1Cfg           sib1SchCfg;       /* SIB1 config */
   uint8_t              maxMsg3Tx;         /* MAximum num of msg3 tx*/
}SchCellCb;

typedef struct schTimer
{
   CmTqCp       tmrTqCp;               /*!< Timer Task Queue Cntrl Point */
   CmTqType     tmrTq[SCH_TQ_SIZE];    /*!< Timer Task Queue */
   uint8_t      tmrRes;              /*!< Timer resolution */
}SchTimer;

/**
 * @brief
 * Control block for sch
 */
typedef struct schCb
{
   TskInit                schInit;               /*!< Task Init info */
   SchGenCb               genCfg;                /*!< General Config info */
   SchTimer               schTimersInfo;         /*!< Sch timer queues and resolution */
   SchAllApis             allApis[NUM_SCH_TYPE]; /*!<List of All Scheduler Type dependent Function pointers*/
   SchCellCb              *cells[MAX_NUM_CELL];  /* Array to store cellCb ptr */
   CmLListCp              sliceCfg;              /* Linklist to Store Slice configuration */
   SchStatistics          statistics;            /* Statistics configuration and calculated values */
}SchCb;

/* Declaration for scheduler control blocks */
SchCb schCb[SCH_MAX_INST];

/* function declarations */
short int schActvTmr(Ent ent,Inst inst);
void SchFillCfmPst(Pst *reqPst,Pst *cfmPst,RgMngmt *cfm);

/* Configuration related function declarations */
void schInitUlSlot(SchUlSlotInfo *schUlSlotInfo);
void schInitDlSlot(SchDlSlotInfo *schDlSlotInfo);
void BuildK0K1Table(SchCellCb *cell, SchK0K1TimingInfoTbl *k0K1InfoTbl, bool pdschCfgCmnPres, \
   SchPdschCfgCmn pdschCmnCfg,SchPdschConfig pdschDedCfg, uint8_t ulAckListCount, uint8_t *UlAckTbl);
void BuildK2InfoTable(SchCellCb *cell, SchPuschTimeDomRsrcAlloc timeDomRsrcAllocList[], \
   uint16_t puschSymTblSize, SchK2TimingInfoTbl *msg3K2InfoTbl, SchK2TimingInfoTbl *k2InfoTbl);
uint8_t SchSendCfgCfm(Pst *pst, RgMngmt *cfm);
SchUeCb* schGetUeCb(SchCellCb *cellCb, uint16_t crnti);
uint8_t addUeToBeScheduled(SchCellCb *cell, uint8_t ueId);

/* Incoming message handler function declarations */
uint8_t SchProcCellCfgReq(Pst *pst, SchCellCfg *schCellCfg);
uint8_t SchProcSlotInd(Pst *pst, SlotTimingInfo *slotInd);
uint8_t SchProcRachInd(Pst *pst, RachIndInfo *rachInd);
uint8_t SchProcCrcInd(Pst *pst, CrcIndInfo *crcInd);
uint8_t SchProcUlCqiInd(Pst *pst, SchUlCqiInd *ulCqiInd);
uint8_t SchProcDlCqiInd(Pst *pst, SchDlCqiInd *dlCqiInd);
uint8_t SchProcPhrInd(Pst *pst, SchPwrHeadroomInd *schPhrInd);
uint8_t SchProcDlRlcBoInfo(Pst *pst, DlRlcBoInfo *dlBoInfo);
uint8_t SchAddUeConfigReq(Pst *pst, SchUeCfgReq *ueCfgToSch);
uint8_t SchProcBsr(Pst *pst, UlBufferStatusRptInd *bsrInd);
uint8_t SchProcSrUciInd(Pst *pst, SrUciIndInfo *uciInd);
uint8_t SchModUeConfigReq(Pst *pst, SchUeRecfgReq *ueRecfgToSch);
uint8_t SchProcUeDeleteReq(Pst *pst, SchUeDelete  *ueDelete);
uint8_t SchProcCellDeleteReq(Pst *pst, SchCellDeleteReq  *schCellDelete);
uint8_t SchProcSliceCfgReq(Pst *pst, SchSliceCfgReq *schSliceCfgReq);
uint8_t SchProcSliceRecfgReq(Pst *pst, SchSliceRecfgReq *schSliceRecfgReq);
uint8_t SchProcRachRsrcReq(Pst *pst, SchRachRsrcReq *schRachRsrcReq);
uint8_t SchProcRachRsrcRel(Pst *pst, SchRachRsrcRel *schRachRsrcRel);
uint8_t SchProcPagingInd(Pst *pst,  SchPageInd *pageInd);
uint8_t SchProcDlHarqInd(Pst *pst, DlHarqInd *dlHarqInd);

/* DL scheduling related function declarations */
PduTxOccsaion schCheckSsbOcc(SchCellCb *cell, SlotTimingInfo slotTime);
PduTxOccsaion schCheckSib1Occ(SchCellCb *cell, SlotTimingInfo slotTime);
uint8_t schBroadcastSsbAlloc(SchCellCb *cell, SlotTimingInfo slotTime, DlBrdcstAlloc *dlBrdcstAlloc);
uint8_t schBroadcastSib1Alloc(SchCellCb *cell, SlotTimingInfo slotTime, DlBrdcstAlloc *dlBrdcstAlloc);
bool schProcessRaReq(Inst schInst, SchCellCb *cellCb, SlotTimingInfo currTime, uint8_t ueId);
uint8_t schProcessMsg4Req(SchCellCb *cell, SlotTimingInfo currTime, uint8_t ueId,bool isRetxMsg4, SchDlHqProcCb **hqP);
uint8_t schFillRar(SchCellCb *cell, SlotTimingInfo rarTime, uint16_t ueId, RarAlloc *rarAlloc, uint8_t k0Index);
bool schFillBoGrantDlSchedInfo(SchCellCb *cell, SlotTimingInfo currTime, uint8_t ueId, bool isRetx, SchDlHqProcCb **hqP);
uint8_t schDlRsrcAllocDlMsg(SchCellCb *cell, SlotTimingInfo slotTime, uint16_t crnti, uint32_t tbSize, DlMsgSchInfo *dlMsgAlloc,\
   uint16_t startPRB, uint8_t pdschStartSymbol, uint8_t pdschNumSymbols,bool isRetx, SchDlHqProcCb* hqP, SchPdcchAllocInfo pdcchAllocInfo);
uint8_t schDlRsrcAllocMsg4(SchCellCb *cell, SlotTimingInfo msg4Time, uint8_t ueId, DlMsgSchInfo *msg4Alloc,\
uint8_t pdschStartSymbol, uint8_t pdschNumSymbols, bool isRetx, SchDlHqProcCb *hqP);
uint8_t allocatePrbDl(SchCellCb *cell, SlotTimingInfo slotTime, uint8_t startSymbol, uint8_t symbolLength, \
   uint16_t *startPrb, uint16_t numPrb);
void fillDlMsgInfo(DlMsgSchInfo *dlMsgInfo, uint16_t crnti, bool isRetx, SchDlHqProcCb* hqP); /*AS per 38.473 V15.3.0, Section 9.3.1.32 crnti value range is b/w 0..65535*/
bool findValidK0K1Value(SchCellCb *cell, SlotTimingInfo currTime, uint8_t ueId, bool dedMsg, uint8_t *pdschStartSymbol, uint8_t *pdschSymblLen,\
        SlotTimingInfo *pdcchTime,  SlotTimingInfo *pdschTime, SlotTimingInfo *pucchTime, bool isRetx, SchDlHqProcCb *hqP, SchPdcchAllocInfo *pdcchAllocInfo);
RaRspWindowStatus isInRaRspWindow(SchRaReq *raReq, SlotTimingInfo frameToCheck, uint16_t numSlotsPerSystemFrame);

/* UL scheduling related function declarations */
uint8_t schUlResAlloc(SchCellCb *cell, Inst schInst);
bool schCheckPrachOcc(SchCellCb *cell, SlotTimingInfo prachOccasionTimingInfo);
uint8_t schCalcPrachNumRb(SchCellCb *cell);
void schPrachResAlloc(SchCellCb *cell, UlSchedInfo *ulSchedInfo, SlotTimingInfo prachOccasionTimingInfo);
uint16_t schAllocPucchResource(SchCellCb *cell, SlotTimingInfo pucchTime, uint16_t crnti,SchUeCb *ueCb, bool isRetx, SchDlHqProcCb *hqP);
uint8_t schFillUlDci(SchUeCb *ueCb, SchPuschInfo *puschInfo, DciInfo *dciInfo, bool isRetx, SchUlHqProcCb *hqP);
uint8_t schFillPuschAlloc(SchUeCb *ueCb, SlotTimingInfo puschTime, uint32_t tbSize,
                            uint8_t startSymb, uint8_t symbLen, uint16_t startPrb, bool isRetx, SchUlHqProcCb *hqP);
uint8_t allocatePrbUl(SchCellCb *cell, SlotTimingInfo slotTime, uint8_t startSymbol, uint8_t symbolLength, \
   uint16_t *startPrb, uint16_t numPrb);
bool schProcessSrOrBsrReq(SchCellCb *cell, SlotTimingInfo currTime, uint8_t ueId, bool isRetx, SchUlHqProcCb **hqP);

/*Generic Functions*/
void updateGrantSizeForBoRpt(CmLListCp *lcLL, DlMsgSchInfo *dlMsgAlloc, BsrInfo *bsrInfo, uint32_t *accumalatedBOSize);
uint16_t searchLargestFreeBlock(SchCellCb *cell, SlotTimingInfo slotTime,uint16_t *startPrb, Direction dir);
LcInfo* handleLcLList(CmLListCp *lcLL, uint8_t lcId, ActionTypeLL action);
void prbAllocUsingRRMPolicy(CmLListCp *lcLL, bool dedicatedPRB, uint16_t mcsIdx,uint8_t numSymbols,\
                      uint16_t *sharedPRB, uint16_t *reservedPRB, bool *isTxPayloadLenAdded, bool *srRcvd);
void updateBsrAndLcList(CmLListCp *lcLL, BsrInfo *bsrInfo, uint8_t status);
uint8_t fillUeCoresetAndSsInfo(SchUeCb *ue);
bool schDlCandidateSelection(SchUeCb *ue,  SlotTimingInfo slotTime, SchPdcchAllocInfo *pdcchAllocInfo);

/*Paging Functions*/
void schProcPagingCfg(SchCellCb *cell);
void schCfgPdcchMonOccOfPO(SchCellCb *cell);
void schIncrSlot(SlotTimingInfo *timingInfo, uint8_t incr, uint16_t numSlotsPerRF);
uint8_t schFillPagePdschCfg(SchCellCb *cell, PageDlSch *pageDlSch, SlotTimingInfo slotTime, \
                             uint16_t tbSize, uint8_t mcs, uint16_t startPrb);
/*DL HARQ Functions*/
void schDlHqEntInit(SchCellCb *cellCb, SchUeCb *ueCb);
void schMsg4FeedbackUpdate(SchDlHqProcCb *hqP, uint8_t fdbk);
void schDlHqFeedbackUpdate(SchDlHqProcCb *hqP, uint8_t fdbk1, uint8_t fdbk2);
uint8_t schDlGetAvlHqProcess(SchCellCb *cellCb, SchUeCb *ueCb, SchDlHqProcCb **hqP);
void schDlReleaseHqProcess(SchDlHqProcCb *hqP);
void schDlHqEntDelete(SchUeCb *ueCb);

/*UL HARQ Functions*/
void schUlHqEntInit(SchCellCb *cellCb, SchUeCb *ueCb);
uint8_t schMsg3RetxSchedulingForUe(SchRaCb *raCb);
void schUlHqProcessNack(SchUlHqProcCb *hqP);
void schUlHqProcessAck(SchUlHqProcCb *hqP);
uint8_t schUlGetAvlHqProcess(SchCellCb *cellCb, SchUeCb *ueCb, SchUlHqProcCb **hqP);
void schUlReleaseHqProcess(SchUlHqProcCb *hqP, Bool togNdi);
void schUlHqEntDelete(SchUeCb *ueCb);

/* UE Manager HARQ Fun*/
void schUpdateHarqFdbk(SchUeCb *ueCb, uint8_t numHarq, uint8_t *harqPayload,SlotTimingInfo *slotInd);

/* Round Robbin Scheduler funtions*/
uint8_t schFillUlDciForMsg3Retx(SchRaCb *raCb, SchPuschInfo *puschInfo, DciInfo *dciInfo);
bool schGetMsg3K2(SchCellCb *cell, SchUlHqProcCb* msg3HqProc, uint16_t dlTime, SlotTimingInfo *msg3Time, bool isRetx);
void schMsg4Complete(SchUeCb *ueCb);

/* Statistics Function */
uint8_t SchProcStatsReq(Pst *pst, SchStatsReq *statsReq);
uint8_t SchSendStatsIndToMac(Inst inst, SchStatsInd  *statsInd);
uint8_t schCalcAndSendGrpStats(SchStatsGrp *grpInfo);
uint8_t SchProcStatsDeleteReq(Pst *pst, SchStatsDeleteReq *statsDeleteReq);
uint8_t SchProcStatsModificationReq(Pst *pst, SchStatsModificationReq *statsModificationReq);
void deleteStatsGrpInfo(Inst inst, SchStatsGrp *statsGrpInfo);
/**********************************************************************
  End of file
 **********************************************************************/
