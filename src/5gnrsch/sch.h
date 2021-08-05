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
#define SCH_INST_START 1
#define SCH_MAX_INST 1
#define SCH_MU0_NUM_SLOTS 10 
#define SCH_MU1_NUM_SLOTS 20 
#define SCH_MU2_NUM_SLOTS 30 
#define SCH_MU3_NUM_SLOTS 40 
#define SCH_MU4_NUM_SLOTS 50 
#define SCH_MAX_SFN 1024
#ifdef NR_TDD
#define MAX_NUM_RB 275 /* value for numerology 1, 100 MHz */
#else
#define MAX_NUM_RB 106 /* value for numerology 0, 20 MHz */
#endif
#define SCH_MIB_TRANS 8  /* MIB transmission as per 38.331 is every 80 ms */
#define SCH_SIB1_TRANS 16 /* SIB1 transmission as per 38.331 is every 160 ms */
#define SCH_NUM_SC_PRB 12 /* number of SCs in a PRB */
#define SCH_MAX_SSB_BEAM 8 /* since we are supporting only SCS=15KHz and 30KHz */
#define SCH_SYMBOL_PER_SLOT 14
#define SCH_SSB_NUM_SYMB 4
#define SCH_SSB_NUM_PRB 20
#define SCHED_DELTA 1
#define BO_DELTA 1
#define RAR_DELAY   2
#define MSG4_DELAY  1
#define PDSCH_START_RB 10
#define PUSCH_START_RB 15
#define PUCCH_NUM_PRB_FORMAT_0_1_4 1  /* number of PRBs in freq domain, spec 38.213 - 9.2.1 */
#define SI_RNTI 0xFFFF
#define P_RNTI  0xFFFE
#define DMRS_MAP_TYPE_A 1
#define NUM_DMRS_SYMBOLS 1
#define DMRS_ADDITIONAL_POS 0
#define SCH_DEFAULT_K1 1
#define SCH_TQ_SIZE 10

#define CRC_FAILED 0
#define CRC_PASSED 1

#define MAC_HDR_SIZE  3   /* 3 bytes of MAC Header */
#define UL_GRANT_SIZE 224

typedef struct schCellCb SchCellCb;
typedef struct schUeCb SchUeCb;

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
   SCH_UE_STATE_ACTIVE
}SchUeState;

typedef enum
{
   SCH_LC_STATE_INACTIVE,
   SCH_LC_STATE_ACTIVE
}SchLcState;

/**
 * @brief
 * Structure holding LTE MAC's General Configuration information.
 */
typedef struct schGenCb
{
   uint8_t         tmrRes;           /*!< Timer resolution */
   uint8_t         startCellId;      /*!< Starting Cell Id */
#ifdef LTE_ADV
   bool            forceCntrlSrbBoOnPCel; /*!< value 1 means force scheduling
					    of RLC control BO and SRB BO on
					    PCell. val 0 means don't force*/
   bool            isSCellActDeactAlgoEnable; /*!< TRUE will enable activation/deactivation algo at Schd */
#endif
}SchGenCb;

/**
 * @brief
 * scheduler allocationsfor DL per cell.
 */
typedef struct schDlSlotInfo
{
   uint16_t  totalPrb;                          /*!< Number of RBs in the cell */
   uint16_t  assignedPrb[SCH_SYMBOL_PER_SLOT];  /*!< Num RBs and corresponding symbols allocated */
   uint16_t  resAllocBitMap;                    /*!< Resource allocation bitmap */
   bool      ssbPres;                           /*!< Flag to determine if SSB is present in this slot */
   uint8_t   ssbIdxSupported;                   /*!< Max SSB index */
   SsbInfo   ssbInfo[MAX_SSB_IDX];              /*!< SSB info */
   bool      sib1Pres;                          /*!< Flag to determine if SIB1 is present in this slot */
   RarInfo   *rarInfo;                          /*!< RAR info */
   DlMsgInfo *dlMsgInfo;                       /*!< DL dedicated Msg info */
}SchDlSlotInfo;

typedef struct schRaCb
{
   uint16_t tcrnti;
}SchRaCb;

/**
 * @brief
 * scheduler allocationsfor UL per cell.
 */
typedef struct schUlSlotInfo
{
   uint16_t     totalPrb;  /*!< Number of RBs in the cell */
   uint16_t     assignedPrb[SCH_SYMBOL_PER_SLOT]; /*!< Num RBs and corresponding symbols allocated */
   uint16_t     resAllocBitMap;                    /*!< Resource allocation bitmap */
   uint8_t      puschCurrentPrb; /* Current PRB for PUSCH allocation */
   bool         puschPres; /*!< PUSCH presence field */
   SchPuschInfo *schPuschInfo; /*!< PUSCH info */
   bool         pucchPres; /*!< PUCCH presence field */
   SchPucchInfo schPucchInfo; /*!< PUCCH info */
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
}SchDlLcCtxt;

typedef struct schDlCb
{
   uint8_t       numDlLc;
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
}SchUlLcCtxt;

typedef struct schUlCb
{
   uint8_t     numUlLc;
   SchUlLcCtxt ulLcCtxt[MAX_NUM_LC];
}SchUlCb;

typedef struct schUeCfgCb
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
}SchUeCfgCb;

/**
 * @brief
 * UE control block
 */
typedef struct schUeCb
{
   uint16_t   ueIdx;
   uint16_t   crnti;
   SchUeCfgCb ueCfg;
   SchUeState state;
   SchCellCb  *cellCb;
   bool       srRcvd;
   BsrInfo    bsrInfo[MAX_NUM_LOGICAL_CHANNEL_GROUPS];
   SchUlCb    ulInfo;
   SchDlCb    dlInfo;
}SchUeCb;

/**
 * @brief
 * RA Request Info
 */
typedef struct schRaReq
{
   uint32_t        raRnti;
   RachIndInfo     *rachInd;
   SlotTimingInfo  winStartTime;
   SlotTimingInfo  winEndTime;
}SchRaReq;

/**
 * @brief
 * Cell Control block per cell.
 */
typedef struct schCellCb
{
   uint16_t      cellId;                            /*!< Cell ID */
   Inst          instIdx;                           /*!< Index of the scheduler instance */
   Inst          macInst;                           /*!< Index of the MAC instance */
   uint8_t       numSlots;                          /*!< Number of slots in current frame */
   SlotTimingInfo   slotInfo;                          /*!< SFN, Slot info being processed*/
   SchDlSlotInfo **schDlSlotInfo;                   /*!< SCH resource allocations in DL */
   SchUlSlotInfo **schUlSlotInfo;                   /*!< SCH resource allocations in UL */
   SchCellCfg    cellCfg;                           /*!< Cell ocnfiguration */
   bool          firstSsbTransmitted;
   bool          firstSib1Transmitted;
   uint8_t       ssbStartSymbArr[SCH_MAX_SSB_BEAM]; /*!<start symbol per SSB beam */
   SchRaReq      *raReq[MAX_NUM_UE];                /*!< Pending RA request */
   SchRaCb       raCb[MAX_NUM_UE];                  /*!< RA Cb */
   uint16_t      numActvUe;                         /*!<Number of active UEs */
   uint32_t      actvUeBitMap;                      /*!<Bit map to find active UEs */
   uint32_t      boIndBitMap;                       /*!<Bit map to indicate UEs that have recevied BO */
   SchUeCb       ueCb[MAX_NUM_UE];                  /*!<Pointer to UE contexts of this cell */
#ifdef NR_TDD
   uint8_t       numSlotsInPeriodicity;             /*!< number of slots in configured periodicity and SCS */
   uint32_t      slotFrmtBitMap;                    /*!< 2 bits must be read together to determine D/U/S slots. 00-D, 01-U, 10-S */
   uint32_t      symbFrmtBitMap;                    /*!< 2 bits must be read together to determine D/U/S symbols. 00-D, 01-U, 10-S */
#endif
}SchCellCb;

/**
 * @brief
 * Control block for sch
 */
typedef struct schCb
{
   TskInit       schInit;               /*!< Task Init info */
   SchGenCb      genCfg;                /*!< General Config info */
   CmTqCp        tmrTqCp;               /*!< Timer Task Queue Cntrl Point */
   CmTqType      tmrTq[SCH_TQ_SIZE];    /*!< Timer Task Queue */
   SchCellCb     *cells[MAX_NUM_CELL];  /* Array to store cellCb ptr */  
}SchCb;

/* Declaration for scheduler control blocks */
SchCb schCb[SCH_MAX_INST];

/* function declarations */
SchUeCb* schGetUeCb(SchCellCb *cellCb, uint16_t crnti);
void schInitUlSlot(SchUlSlotInfo *schUlSlotInfo);
void schInitDlSlot(SchDlSlotInfo *schDlSlotInfo);
uint8_t SchSendCfgCfm(Pst *pst, RgMngmt *cfm);
short int schActvTmr(Ent ent,Inst inst);
uint8_t schBroadcastAlloc(SchCellCb *cell, DlBrdcstAlloc *dlBrdcstAlloc,uint16_t slot);
uint8_t schProcessSlotInd(SlotTimingInfo *slotInd, Inst inst);
uint8_t schUlResAlloc(SchCellCb *cell, Inst schInst);
uint8_t schDlRsrcAllocMsg4(DlMsgAlloc *msg4Alloc, SchCellCb *cell, uint16_t slot, bool ssbPresent, bool sib1Present);
uint16_t schCalcTbSize(uint32_t payLoadSize);
uint16_t schCalcNumPrb(uint16_t tbSize, uint16_t mcs, uint8_t numSymbols);
uint16_t schAllocPucchResource(SchCellCb *cell, uint16_t crnti, uint16_t slot);
uint8_t schProcessRachInd(RachIndInfo *rachInd, Inst schInst);
uint8_t schFillUlDci(SchUeCb *ueCb, SchPuschInfo puschInfo, DciInfo *dciInfo);
uint8_t schFillPuschAlloc(SchUeCb *ueCb, uint16_t pdcchSlot, uint32_t dataVol, SchPuschInfo *puschInfo);
uint8_t schDlRsrcAllocDlMsg(DlMsgAlloc *dlMsgAlloc, SchCellCb *cell, uint16_t crnti,
   uint32_t *accumalatedSize, uint16_t slot);
uint16_t schAccumalateLcBoSize(SchCellCb *cell, uint16_t ueIdx);
uint8_t schFillRar(RarAlloc *rarAlloc, uint16_t raRnti, uint16_t pci, uint8_t offsetPointA, bool ssbPresent, bool sib1Present);
void BuildK0K1Table(SchCellCb *cell, SchK0K1TimingInfoTbl *k0K1InfoTbl, bool pdschCfgCmnPres, SchPdschCfgCmn pdschCmnCfg,\
SchPdschConfig pdschDedCfg, uint8_t ulAckListCount, uint8_t *UlAckTbl);
void schProcessRaReq(SlotTimingInfo currTime, SchCellCb *cellCb);
void BuildK2InfoTable(SchCellCb *cell, SchPuschTimeDomRsrcAlloc timeDomRsrcAllocList[], uint16_t puschSymTblSize,\
SchK2TimingInfoTbl *k2InfoTbl);
/**********************************************************************
  End of file
 **********************************************************************/
