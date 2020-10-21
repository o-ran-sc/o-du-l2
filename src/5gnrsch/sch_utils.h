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

#include <stdint.h>
#include <math.h>

/* Memory info */
#define SCH_MEM_REGION     4
#define SCH_POOL           1

/* macros */
#define MAX_CORESET_INDEX      16
#define MAX_SEARCH_SPACE_INDEX 16
#define MAX_RACH_NUM_RB_IDX    16
#define MAX_PUCCH_RES_SET_IDX  16
#define MAX_PRACH_CONFIG_IDX   256
#define MAX_MU_PUSCH           4
#define TOTAL_TBSIZE_VALUES    93

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

/* allocate and zero out a static buffer */
#define SCH_ALLOC(_datPtr, _size)                               \
{                                                               \
   int _ret;                                                    \
   _ret = SGetSBuf(SCH_MEM_REGION, SCH_POOL,                    \
          (Data **)&_datPtr, _size);                             \
   if(_ret == ROK)                                              \
   {                                                            \
      memset(_datPtr, 0, _size);                         \
   }                                                            \
   else                                                         \
   {                                                            \
      _datPtr = NULLP;                                          \
   }                                                            \
}

/* free a static buffer */
#define SCH_FREE(_datPtr, _size)                                \
{                                                               \
   if(_datPtr)                                                  \
   {                                                            \
      SPutSBuf(SCH_MEM_REGION, SCH_POOL,                        \
            (Data *)_datPtr, _size);                            \
   }                                                            \
}
                          
/* Fill post structure for msg from SCH to MAC */
#define FILL_PST_SCH_TO_MAC(_pst, _inst)                     \
{                                                            \
   _pst.srcProcId = ODU_GET_PROCID();                       \
   _pst.dstProcId = ODU_GET_PROCID();                       \
   _pst.srcEnt    = ENTMAC;                                  \
   _pst.dstEnt    = ENTMAC;                                  \
   _pst.srcInst   = 1;                                       \
   _pst.dstInst   = 0;                                       \
   _pst.selector  = ODU_SELECTOR_TC;                         \
}

extern int8_t coresetIdxTable[MAX_CORESET_INDEX][4];
extern int8_t searchSpaceIdxTable[MAX_SEARCH_SPACE_INDEX][4];

/* functions declarations */
void freqDomRscAllocType0(uint16_t startPrb, uint16_t prbSize, uint8_t *freqDomain);
uint16_t schCalcTbSize(uint16_t payLoadSize);
uint16_t schCalcNumPrb(uint16_t tbSize, uint16_t mcs, uint8_t numSymbols);
uint16_t schCalcTbSizeFromNPrb(uint16_t numPrb, uint16_t mcs, uint8_t numSymbols);
SchUeCb* schGetUeCb(SchCellCb *cellCb, uint16_t crnti);
void schInitUlSlot(SchUlSlotInfo *schUlSlotInfo);
void schInitDlSlot(SchDlSlotInfo *schDlSlotInfo);


/**********************************************************************
  End of file
 **********************************************************************/
