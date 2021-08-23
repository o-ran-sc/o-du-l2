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
#include "envopt.h"
#include "envdep.h"
#include "envind.h"
#include "gen.h" 
#include "ssi.h" 
#include "cm_math.h"
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
#include "cm_tpt.x"
#include "cm_tkns.x" 
#include "cm_mblk.x"
#include "cm_llist.x"
#include "cm5.x" 
#include "cm_hash.x"
#include "cm_lte.x"
#include "cm_lib.x"
#include "du_log.h"

#define RADIO_FRAME_DURATION 10 /* Time duration of a radio frame in ms */
/* MAX values */
#define MAX_NUM_CELL 1
#define MAX_NUM_UE   2
#define MAX_NUM_UE_PER_TTI 1
#define MAX_NUM_LC   11
#define MAX_NUM_SRB  3    /* Max. no of Srbs */
#define MAX_NUM_DRB  29   /* spec 38.331, maxDRB */

/* 5G ORAN phy delay */
#define PHY_DELTA_DL 1
#define PHY_DELTA_UL 0

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
#define PUCCH_FORMAT_0 0
#define PUCCH_FORMAT_1 1
#define PUCCH_FORMAT_2 2
#define PUCCH_FORMAT_3 3 
#define PUCCH_FORMAT_4 4

#define BANDWIDTH_20MHZ 20
#define BANDWIDTH_100MHZ 100

/* PRB allocation as per 38.101, Section 5.3.2 */
#define TOTAL_PRB_20MHZ_MU0 106
#define TOTAL_PRB_100MHZ_MU1 273

#define ODU_THROUGHPUT_PRINT_TIME_INTERVAL  5 /* in milliseconds */

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
#define ODU_SET_THREAD_AFFINITY SSetAffinity
#define ODU_CREATE_TASK SCreateSTsk

#ifdef NR_TDD
/* Maximum slots for max periodicity and highest numerology is 320.
 * However, aligning to fapi_interface.h, setting this macro to 160 */
#define MAX_TDD_PERIODICITY_SLOTS 160 
#define MAX_SYMB_PER_SLOT 14 
#endif

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

typedef enum
{
   UE_CFG_INACTIVE,
   UE_CFG_INPROGRESS,
   UE_CREATE_COMPLETE,
   UE_DELETE_COMPLETE,
   UE_RECFG_COMPLETE
}UeCfgState;

typedef enum
{
   CONFIG_UNKNOWN,
   CONFIG_ADD,
   CONFIG_MOD,
   CONFIG_DEL
}ConfigType;

#ifdef NR_TDD
typedef enum
{
   DL_SLOT,
   UL_SLOT,
   FLEXI_SLOT
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
#endif

typedef enum
{
   SCS_15KHZ,
   SCS_30KHZ,
   SCS_60KHZ,
   SCS_120KHZ,
   SCS_240KHZ
}SCS;

typedef enum
{
   CELL_UP,
   CELL_DOWN
}OduCellStatus;

typedef struct slotTimingInfo
{
   uint16_t cellId;
   uint16_t sfn;
   uint16_t slot;
}SlotTimingInfo;

typedef struct PlmnIdentity
{
   uint8_t mcc[3];
   uint8_t mnc[3];
}Plmn;

typedef struct oduCellId
{
   uint16_t cellId;
}OduCellId;

#ifdef NR_TDD
typedef struct tddCfg
{
   bool               pres;
   DlUlTxPeriodicity  tddPeriod;      /* DL UL Transmission periodicity */
   SlotConfig         slotCfg[MAX_TDD_PERIODICITY_SLOTS][MAX_SYMB_PER_SLOT]; 
}TDDCfg;
#endif

OduCellStatus gCellStatus;
uint64_t gSlotCount;
uint64_t gDlDataRcvdCnt;   /* Number of DL data received at EGTP */

void freqDomRscAllocType0(uint16_t startPrb, uint16_t prbSize, uint8_t *freqDomain);
void oduCpyFixBufToMsg(uint8_t *fixBuf, Buffer *mBuf, uint16_t len);
uint8_t buildPlmnId(Plmn plmn, uint8_t *buf);

uint8_t SGetSBufNewForDebug(char *file, char *func, char *line, Region region, Pool pool, Data **ptr, Size size);
uint8_t SPutSBufNewForDebug(char *file, char *func, char *line, Region region, Pool pool, Data *ptr, Size size);
uint8_t SGetStaticBufNewForDebug(char *file, char *func, char *line, \
Region region, Pool pool, Data **ptr, Size size, uint8_t memType);
uint8_t SPutStaticBufNewForDebug(char *file, char *func, char *line, \
Region region, Pool pool, Data *ptr, Size size, uint8_t memType);

#endif

/**********************************************************************
  End of file
***********************************************************************/
