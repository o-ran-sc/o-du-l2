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

#define SCH_INST_START 1
#define SCH_MAX_INST 1

#define RADIO_FRAME_DURATION 10 /* Time duration of a radio frame in ms */
/* MAX values */
#define MAX_NUM_CELL 2 /* Changed to 2 to support cell Id 2 even if there is only one cell in DU */
#define MAX_NUM_MU   4
#define MAX_NUM_UE   3
#define MAX_NUM_UE_PER_TTI 2
#define MAX_NUM_LC   MAX_DRB_LCID + 1   /*Spec 38.331: Sec 6.4: maxLC-ID Keyword*/
#define MAX_NUM_SRB  3    /* Max. no of Srbs */
#define MAX_NUM_DRB  29   /* spec 38.331, maxDRB */
#define MAX_NUM_SSB  64   /* spec 28.331, maxNrofSSBs */
#define MAX_NUM_HARQ_PROC 16 /* spec 38.331, nrofHARQ-ProcessesForPDSCH */
#define MAX_NUM_TB_PER_UE 2  /* spec 38.331, maxNrofCodeWordsScheduledByDCI */

/* 5G ORAN phy delay */
#ifdef NR_TDD
#define PHY_DELTA_DL 2
#define PHY_DELTA_UL 0
#else
#define PHY_DELTA_DL 1
#define PHY_DELTA_UL 0
#endif

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

#define DEFAULT_MCS 4

#define BANDWIDTH_20MHZ 20
#define BANDWIDTH_100MHZ 100

/* PRB allocation as per 38.101, Section 5.3.2 */
#define TOTAL_PRB_20MHZ_MU0 106
#define TOTAL_PRB_100MHZ_MU1 273
#ifdef NR_TDD
#define MAX_NUM_RB TOTAL_PRB_100MHZ_MU1  /* value for numerology 1, 100 MHz */
#else
#define MAX_NUM_RB TOTAL_PRB_20MHZ_MU0 /* value for numerology 0, 20 MHz */
#endif

#define ODU_UE_THROUGHPUT_PRINT_TIME_INTERVAL      5     /* in milliseconds */
#define ODU_SNSSAI_THROUGHPUT_PRINT_TIME_INTERVAL  60000 /* in milliseconds */

/*Spec 38.331 Sec 6.4: Maximum number of paging occasion per paging frame*/
#define MAX_PO_PER_PF 4

#ifdef NR_TDD
#define MAX_SLOTS 20
#else
#define MAX_SLOTS 10
#endif

#define MAX_SFN   1024

/*First SCS in kHz as per 3gpp spec 38.211 Table 4.2-1 */
#define BASE_SCS 15

#define MAX_NUM_STATS_CFG 2 /* Max number of statistics configuration/Subscription supported */
#define MAX_NUM_STATS_GRP 5 /* Max number of statistics group per configuration request */
#define MAX_NUM_STATS 10    /* Max number of statistics per group */

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
#define ODU_REM_POST_MSG_MULT SRemPstMsgMult
#define ODU_REG_TMR_MT SRegTmrMt
#define ODU_SEGMENT_MSG SSegMsg
#define ODU_CAT_MSG SCatMsg
#define ODU_GET_PROCID SFndProcId
#define ODU_SET_THREAD_AFFINITY SSetAffinity
#define ODU_CREATE_TASK SCreateSTsk

#define MAX_SYMB_PER_SLOT 14 

/* Slice */
#define SD_SIZE 3

#ifdef NR_TDD
/* Maximum slots for max periodicity and highest numerology is 320.
 * However, aligning to fapi_interface.h, setting this macro to 160.
 * TODO : To support 160, FAPI_MAX_NUM_TLVS_CONFIG in fapi_interface.h
 * of Intel L1 must be incremented to a higher number */
#define MAX_TDD_PERIODICITY_SLOTS 10 
#endif

#define GET_UE_ID( _crnti,_ueId)           \
{                                          \
   _ueId = _crnti - ODU_START_CRNTI + 1;  \
}

#define GET_CRNTI( _crnti, _ueId)          \
{                                          \
   _crnti = _ueId + ODU_START_CRNTI - 1;  \
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

/* MACRO for checking CRNTI range*/
#define CHECK_CRNTI(_crnti, _isCrntiValid)                                         \
{                                                                   \
  _isCrntiValid = ((_crnti >= ODU_START_CRNTI && _crnti <= ODU_END_CRNTI ) ? 1 : 0); \
}

#define CHECK_LCID(_lcId, _isLcidValid)      \
{\
   _isLcidValid = ((_lcId >= SRB0_LCID && _lcId <= MAX_DRB_LCID) ? 1 : 0);\
}

/**
 * @def TMR_CALCUATE_WAIT
 *
 *    This macro calculates and assigns wait time based on the value of the
 *    timer and the timer resolution. Timer value of 0 signifies that the
 *    timer is not configured
 *
 * @param[out] _wait   Time for which to arm the timer changed to proper
 *                     value according to the resolution
 * @param[in] _tmrVal   Value of the timer
 * @param[in] _timerRes   Resolution of the timer
 *
*/
#define TMR_CALCUATE_WAIT(_wait, _tmrVal, _timerRes)          \
{                                                             \
   (_wait) = ((_tmrVal) * SS_TICKS_SEC)/((_timerRes) * 1000); \
   if((0 != (_tmrVal)) && (0 == (_wait)))                     \
   {                                                          \
      (_wait) = 1;                                            \
   }                                                          \
}

typedef enum
{
   SUCCESSFUL, 
   CELLID_INVALID, 
   UEID_INVALID, 
   RESOURCE_UNAVAILABLE,  
   SLICE_NOT_FOUND,
   DUPLICATE_ENTRY,
   PARAM_INVALID,
   STATS_ID_NOT_FOUND
}CauseOfResult ;

typedef enum
{
   UE_CFG_INACTIVE,
   UE_CFG_INPROGRESS,
   UE_CREATE_COMPLETE,
   UE_DELETE_COMPLETE,
   UE_RECFG_COMPLETE,
   UE_RESET_COMPLETE
}UeCfgState;

typedef enum
{
   CONFIG_UNKNOWN,
   CONFIG_ADD,
   CONFIG_MOD,
   CONFIG_DEL,
   CONFIG_REESTABLISH
}ConfigType;

#ifdef NR_TDD
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
   SSB_5MS,
   SSB_10MS,
   SSB_20MS,
   SSB_40MS,
   SSB_80MS,
   SSB_160MS
}SSBPeriodicity;

typedef enum
{
   CELL_UP,
   CELL_DOWN
}OduCellStatus;


typedef enum
{
   DIR_NONE,
   DIR_UL,
   DIR_DL,
   DIR_BOTH
}Direction;

typedef enum 
{
   SEARCH,
   CREATE,
   DELETE,
   PRINT,
   TRAVERSE_ALL
}ActionTypeLL;

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

typedef struct snssai
{
   uint8_t   sst;
   uint8_t   sd[SD_SIZE];
}Snssai;

typedef struct supportedSliceList
{
   uint8_t    numSupportedSlices;
   Snssai    **snssai;
}SupportedSliceList;

typedef struct oduCellId
{
   uint16_t cellId;
}OduCellId;

#ifdef NR_TDD
typedef struct tddCfg
{
   DlUlTxPeriodicity  tddPeriod;     /*DL UL Transmission periodicity */
   uint8_t            nrOfDlSlots;   /*No. of consecultive full DL slots at beginning of DL-UL pattern*/
   uint8_t            nrOfDlSymbols; /*No. of consecultive DL symbol at beginning of slot after last full DL slot*/ 
   uint8_t            nrOfUlSlots;   /*No. of consecutive full UL slots at the end of each DL-UL pattern*/
   uint8_t            nrOfUlSymbols; /*No. of consecutive UL symbols in the end of the slot before the first full UL slot*/
}TDDCfg;
#endif

OduCellStatus gCellStatus;
uint64_t gSlotCount;
uint64_t gDlDataRcvdCnt;   /* Number of DL data received at EGTP */

void fillCoresetFeqDomAllocMap(uint16_t startPrb, uint16_t prbSize, uint8_t *freqDomain);
void oduCpyFixBufToMsg(uint8_t *fixBuf, Buffer *mBuf, uint16_t len);
uint8_t buildPlmnId(Plmn plmn, uint8_t *buf);
uint16_t convertScsEnumValToScsVal(uint8_t scsEnumValue);
uint8_t convertScsValToScsEnum(uint32_t num);
uint8_t convertSSBPeriodicityToEnum(uint32_t num);

uint8_t SGetSBufNewForDebug(char *file, const char *func, int line, Region region, Pool pool, Data **ptr, Size size);
uint8_t SPutSBufNewForDebug(char *file, const char *func, int line, Region region, Pool pool, Data *ptr, Size size);
uint8_t SGetStaticBufNewForDebug(char *file, const char *func, int line, \
Region region, Pool pool, Data **ptr, Size size, uint8_t memType);
uint8_t SPutStaticBufNewForDebug(char *file, const char *func, int line, \
Region region, Pool pool, Data *ptr, Size size, uint8_t memType);
uint8_t countSetBits(uint32_t num);
uint32_t convertArfcnToFreqKhz(uint32_t arfcn);
#endif

/**********************************************************************
  End of file
***********************************************************************/
