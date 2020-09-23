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
#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "cm_mem.h"
#include "cm_math.h"
#include "envopt.h"
#include "envdep.h"
#include "envind.h"
#include "gen.h" 
#include "ssi.h" 
#include "cm5.h"
#include "cm_tkns.h"
#include "cm_mblk.h"
#include "cm_llist.h"
#include "cm_hash.h"
#include "cm_lte.h"
#include "cm_err.h"
#include "cm_tpt.h"
#include "cm.h"
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_math.x"
#include "cm_tpt.x"
#include "cm_tkns.x" 
#include "cm_mblk.x"
#include "cm_llist.x"
#include "cm5.x" 
#include "cm_hash.x"
#include "cm_lte.x"
#include "cm_lib.x"
#include "du_log.h"

/* MAX values */
#define MAX_NUM_CELL 1
#define MAX_NUM_UE   1
#define MAX_NUM_LC   11

/* 5G ORAN phy delay */
#define PHY_DELTA 2

 /* SELECTORS */ 
#define ODU_SELECTOR_LC 0
#define ODU_SELECTOR_TC 1
#define ODU_SELECTOR_LWLC 2

#define ODU_START_CRNTI   100
#define ODU_END_CRNTI     500

/* LCID */
#define SRB0_LCID  0
#define SRB1_LCID  1
#define SRB2_LCID  2
#define SRB3_LCID  3
#define MIN_DRB_LCID 4
#define MAX_DRB_LCID 30

#define FREQ_DOM_RSRC_SIZE  6      /* i.e. 6 bytes because Size of frequency domain resource is 45 bits */

/* Defining macros for common utility functions */
#define ODU_GET_MSG_BUF SGetMsg
#define ODU_PUT_MSG_BUF SPutMsg
#define ODU_ADD_PRE_MSG_MULT SAddPreMsgMult
#define ODU_ADD_PRE_MSG_MULT_IN_ORDER SAddPreMsgMultInOrder
#define ODU_ADD_POST_MSG_MULT SAddPstMsgMult
#define ODU_START_TASK SStartTask
#define ODU_STOP_TASK SStopTask
#define ODU_ATTACH_TTSK SAttachTTsk
#define ODU_POST_TASK SPstTsk
#define ODU_COPY_MSG_TO_FIX_BUF SCpyMsgFix
#define ODU_COPY_FIX_BUF_TO_MSG SCpyFixMsg
#define ODU_REG_TTSK SRegTTsk
#define ODU_SET_PROC_ID SSetProcId
#define ODU_GET_MSG_LEN SFndLenMsg
#define ODU_EXIT_TASK SExitTsk
#define ODU_PRINT_MSG SPrntMsg
#define ODU_REM_PRE_MSG SRemPreMsg
#define ODU_REM_PRE_MSG_MULT SRemPreMsgMult
#define ODU_REG_TMR_MT SRegTmrMt
#define ODU_SEGMENT_MSG SSegMsg
#define ODU_CAT_MSG SCatMsg
#define ODU_GET_PROCID SFndProcId

/* Macro definitions for DUtoCuRrcContainer */
#define CELL_GRP_ID 1
#define SCH_REQ_ID  0
#define SR_PROHIBIT_TMR 5
#define SR_TRANS_MAX 2
#define PERIODIC_BSR_TMR 2
#define RETX_BSR_TMR     5
#define SR_DELAY_TMR     6
#define TAG_ID 0
#define TIME_ALIGNMENT_TMR 7
#define PHR_PERIODIC_TMR 7
#define PHR_PROHIBHIT_TMR 0
#define PHR_PWR_FACTOR_CHANGE 3
#define PHR_MODE_OTHER_CG 0
#define RLC_LCID 1 
#define SN_FIELD_LEN 0
#define T_POLL_RETRANSMIT 8 
#define POLL_PDU 0
#define POLL_BYTE 43
#define MAX_RETX_THRESHOLD 5
#define T_REASSEMBLY 8
#define T_STATUS_PROHIBHIT 7
#define MAC_LC_PRIORITY 1
#define PRIORTISIED_BIT_RATE 15
#define BUCKET_SIZE_DURATION 5
#define LC_GRP 0
#define P_NR_FR1 0
#define PDSCH_HARQ_ACK_CODEBOOK 1
#define SERV_CELL_IDX 0
#define RLM_SYNC_OUT_SYNC_THRESHOLD 0
#define ACTIVE_DL_BWP_ID 0
#define ACTIVE_UL_BWP_ID 0
#define SCRAMBLING_ID  1
#define DMRS_ADDITIONAL_POS  0          /* DMRS Additional poistion */
#define RES_ALLOC_TYPE       1          /* Resource allocation type */

/* MACRCO Ddefine for PDCCH Configuration */
#define PDCCH_CTRL_RSRC_SET_ZERO   13   /* Control resouce set zero */
#define PDCCH_SEARCH_SPACE_ZERO    0    /* Search space zero */
#define PDCCH_SEARCH_SPACE_ID      1    /* Common search space id */
#define PDCCH_CTRL_RSRC_SET_ID     0    /* Control resource set id */
#define PDCCH_SEARCH_SPACE_ID_SIB1 0    /* Search space id for sib1 */
#define PDCCH_SEARCH_SPACE_ID_PAGING 1  /* Search space id for paging */
#define PDCCH_SEARCH_SPACE_ID_RA   1    /* Search spaced id for random access */
#define PDCCH_SERACH_SPACE_DCI_FORMAT 0
#define PDCCH_CTRL_RSRC_SET_ONE_ID  1
#define PDCCH_CTRL_RSRC_SET_ONE_DURATION 2  /* Duration for control resource set id i */
#define PDCCH_CTRL_RSRC_SET_ONE_PRECOD_GRANULARITY 1   /* Precoded granularity */
#define PDCCH_SRCH_SPC_TWO_ID 2
#define PDCCH_SRCH_SPC_TWO_AGG_LVL1_CANDIDATE 7   /* Num of candidate at aggregation level 1 */
#define PDCCH_SRCH_SPC_TWO_AGG_LVL2_CANDIDATE 7   /* Num of candidate at aggregation level 2 */
#define PDCCH_SRCH_SPC_TWO_AGG_LVL4_CANDIDATE 4   /* Num of candidate at aggregation level 4 */
#define PDCCH_SRCH_SPC_TWO_AGG_LVL8_CANDIDATE 2   /* Num of candidate at aggregation level 8 */
#define PDCCH_SRCH_SPC_TWO_AGG_LVL16_CANDIDATE 1  /* Num of candidate at aggregation level 16 */
#define PDCCH_SRCH_SPC_TWO_UE_SPEC_DCI_FORMAT  0  /* format 0-0 and 1-0 */
#define PDCCH_SYMBOL_WITHIN_SLOT 128     /* Symbol within Slot Value */
#define NUM_SYMBOLS_PER_SLOT 14       /* Number of symbols within a slot */
#define CORESET0_END_PRB   48
#define CORESET1_NUM_PRB   24

/* MACRO Ddefine for PDSCH Configuration */
#define PDSCH_K0  0
#define PDSCH_START_SYMBOL  2
#define PDSCH_LENGTH_SYMBOL 12
#define PDSCH_RES_ALLOC_TYPE       1          /* Resource allocation type */
#define PDSCH_MAX_CODEWORD_SCH_BY_DCI 0       /* Max num of codewords scheduled by DCI */
#define PDSCH_RBG_SIZE   0                    /* 0: config1 */
#define PDSCH_NUM_HARQ_PROC 5

/* MACRO Define for PUSCH Configuration */
#define PUSCH_K2  3
#define PUSCH_START_SYMBOL  0
#define PUSCH_LENGTH_SYMBOL 14

#define GET_UE_IDX( _crnti,_ueIdx)         \
{                                          \
   _ueIdx = _crnti - ODU_START_CRNTI + 1;  \
}

#define GET_CRNTI( _crnti,_ueIdx)          \
{                                          \
   _crnti = _ueIdx + ODU_START_CRNTI - 1;  \
}

/* Calculates cellIdx from cellId */
#define GET_CELL_IDX(_cellId, _cellIdx)   \
{                                         \
   _cellIdx = _cellId - 1;                \
}

#define SET_BITS_MSB(_startBit, _numBits, _byte) \
{                                                \
   _byte = (~((0xFF) >> _numBits));              \
       _byte >>= _startBit;                          \
}

#define SET_BITS_LSB(_startBit, _numBits, _byte) \
{                                                \
   _byte = (~((0xFF) << _numBits));              \
       _byte <<= _startBit;                          \
}

/* this MACRO set 1 bit at the bit position */
#define SET_ONE_BIT(_bitPos, _out)            \
{                                             \
   _out = ((1<<_bitPos) | _out);              \
}

/* this MACRO un-set 1 bit at the bit position */
#define UNSET_ONE_BIT(_bitPos, _out)            \
{                                               \
   _out = (~(1<<_bitPos) & _out);               \
}

/* this MACRO finds the index of the rightmost set bit */
#define GET_RIGHT_MOST_SET_BIT( _in,_bitPos)        \
{                                                \
   _bitPos = __builtin_ctz(_in);                 \
}

typedef struct slotIndInfo
{
   uint16_t cellId;
   uint16_t sfn;
   uint16_t slot;
}SlotIndInfo;

typedef struct PlmnIdentity
{
   uint8_t mcc[3];
   uint8_t mnc[3];
}Plmn;

typedef enum
{
   UE_CFG_INACTIVE,
   UE_CREATE_COMPLETED,
   UE_RECFG_COMPLETED
}UeCfgState;

void freqDomRscAllocType0(uint16_t startPrb, uint16_t prbSize, uint8_t *freqDomain);
uint8_t getDrbLcId();
uint16_t calcSliv(uint8_t startSymbol, uint8_t lengthSymbol);
void oduCpyFixBufToMsg(uint8_t *fixBuf, Buffer *mBuf, uint16_t len);
#endif

/**********************************************************************
  End of file
***********************************************************************/
