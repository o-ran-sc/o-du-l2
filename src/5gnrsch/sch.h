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
#define SCH_MAX_CELLS 1
#define SCH_MAX_INST 1
#define SCH_NUM_SLOTS 10 /*forcing this to 10 */
#define SCH_MAX_SFN 1024
#define MAX_NUM_RB 106 /* value for numerology 0 15Khz */
#define SCH_MIB_TRANS 80 
#define SCH_NUM_SC_PRB 12 /* number of SCs in a PRB */
#define SCH_SCS_15KHZ 0 /* numerology 0 and 15Khz */
#define SCH_SYMBOL_PER_SLOT 14
#define SCH_SSB_NUM_SYMB 4
#define SCH_SSB_NUM_PRB 20
#define SCH_MAX_SSB_BEAM 4 /* since we are supporting only SCS=15KHz */
#define SCH_MEM_REGION     4
#define SCH_POOL           1
#define SCHED_DELTA 1
#define BO_DELTA 1
#define RAR_DELAY   2
#define MSG4_DELAY  1
#define SCH_MAX_UE  1
#define PUSCH_START_RB 15
#define SI_RNTI 0xFFFF
#define P_RNTI  0xFFFE

#define CRC_FAILED 0
#define CRC_PASSED 1

/* allocate and zero out a static buffer */
#define SCH_ALLOC(_datPtr, _size)                                \
{                                                               \
   S16 _ret;                                                    \
   _ret = SGetSBuf(SCH_MEM_REGION, SCH_POOL,                  \
                    (Data **)&_datPtr, _size);                  \
   if(_ret == ROK)                                              \
      cmMemset((U8*)_datPtr, 0, _size);                         \
   else                                                         \
      _datPtr = NULLP;                                          \
}

/* free a static buffer */
#define SCH_FREE(_datPtr, _size)                                 \
   if(_datPtr)                                                  \
      SPutSBuf(SCH_MEM_REGION, SCH_POOL,                      \
         (Data *)_datPtr, _size);

#define SCH_FILL_RSP_PST(_rspPst, _inst)\
{                                  \
   _rspPst.srcProcId = SFndProcId(); \
   _rspPst.dstProcId = SFndProcId();\
   _rspPst.srcEnt    = ENTRG;\
   _rspPst.dstEnt    = ENTRG;\
   _rspPst.srcInst   = 1;\
   _rspPst.dstInst   = 0;\
	_rspPst.selector  = MAC_SELECTOR_TC;\
}	
extern uint8_t schProcessRachInd(RachIndInfo *rachInd, Inst schInst);
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
   bool      ssbPres;                           /*!< Flag to determine if SSB is present in this slot */
   uint8_t   ssbIdxSupported;                   /*!< Max SSB index */
	SsbInfo   ssbInfo[MAX_SSB_IDX];              /*!< SSB info */
	bool      sib1Pres;                          /*!< Flag to determine if SIB1 is present in this slot */
	RarInfo   *rarInfo;                          /*!< RAR info */
   Msg4Info  *msg4Info;                         /*!< msg4 info */
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
	bool         puschPres; /*!< PUSCH presence field */
	SchPuschInfo *schPuschInfo; /*!< PUSCH info */
}SchUlSlotInfo;

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
   SlotIndInfo   slotInfo;                          /*!< SFN, Slot info being processed*/
   SchDlSlotInfo *schDlSlotInfo[SCH_NUM_SLOTS];     /*!< SCH resource allocations in DL */
   SchUlSlotInfo *schUlSlotInfo[SCH_NUM_SLOTS];     /*!< SCH resource allocations in UL */
	SchCellCfg    cellCfg;                           /*!< Cell ocnfiguration */
	uint8_t       ssbStartSymbArr[SCH_MAX_SSB_BEAM]; /*!<start symbol per SSB beam */
	SchRaCb       raCb[SCH_MAX_UE];                  /*!< Rach Cb */
}SchCellCb;

/**
  * @brief
  * Control block for sch
  */
typedef struct schCb
{
   TskInit       schInit;              /*!< Task Init info */
   SchGenCb      genCfg;                 /*!< General Config info */
   SchCellCb     *cells[SCH_MAX_CELLS];  /* Array to store cellCb ptr */  
}SchCb;

/* Declaration for scheduler control blocks */
SchCb schCb[SCH_MAX_INST];

/* function declarations */
uint8_t schBroadcastAlloc(SchCellCb *cell, DlBrdcstAlloc *dlBrdcstAlloc,uint16_t slot);
uint8_t schProcessSlotInd(SlotIndInfo *slotInd, Inst inst);
uint8_t schUlResAlloc(SchCellCb *cell, Inst schInst);
uint8_t schDlRsrcAllocMsg4(Msg4Alloc *msg4Alloc, SchCellCb *cell, uint16_t slot);


/**********************************************************************
         End of file
**********************************************************************/
