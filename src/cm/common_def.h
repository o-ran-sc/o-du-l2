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
#define MAX_DRB_LCID 32

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
   UE_CREATE_COMPLETE,
   UE_RECFG_COMPLETE
}UeCfgState;

typedef enum
{
   CONFIG_UNKNOWN,
   CONFIG_ADD,
   CONFIG_MOD,
   CONFIG_DEL
}ConfigType;

void freqDomRscAllocType0(uint16_t startPrb, uint16_t prbSize, uint8_t *freqDomain);
void oduCpyFixBufToMsg(uint8_t *fixBuf, Buffer *mBuf, uint16_t len);
uint8_t buildPlmnId(Plmn plmn, uint8_t *buf);

#endif

/**********************************************************************
  End of file
***********************************************************************/
