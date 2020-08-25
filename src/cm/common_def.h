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

/* 5G ORAN phy delay */
#define PHY_DELTA 2

 /* SELECTORS */ 
#define ODU_SELECTOR_LC 0
#define ODU_SELECTOR_TC 1
#define ODU_SELECTOR_LWLC 2

#define ODU_START_CRNTI   100
#define ODU_END_CRNTI     500

#define GET_NEW_CRNTI(_crnti)         \
{                                     \
   _crnti =  gCrntiCount;             \
   gCrntiCount++;                     \
}

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

/* BIT wise operation MACRO */

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

/* ue bit map */
extern uint32_t gActvUeBitMap;
extern uint32_t gBoIndBitMap;
extern uint16_t gCrntiCount;

/**********************************************************************
  End of file
 **********************************************************************/
