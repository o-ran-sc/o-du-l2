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
#define DEFAULT_UL_ACK_LIST_COUNT 8 /* Max number of pusch time domain uplink allocation */
#define MASK_BIT64_ON 0xFFFFFFFFFFFFFFFF
#define MIN_PRB 1

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
#ifdef ODU_MEMORY_DEBUG_LOG
#define SCH_MEM_LOG(_macro, _file, _line, _func, _size, _datPtr)\
{\
   printf("\n%s=== %s +%d, %s, %d, %p\n",           \
         _macro, _file, _line, _func, _size, _datPtr); \
}
#else
#define SCH_MEM_LOG(_macro, _file, _line, _func, _size, _dataPtr) {}
#endif

#define SCH_ALLOC(_datPtr, _size)                               \
{                                                               \
   uint8_t _ret;                                                    \
   _ret = SGetSBuf(SCH_MEM_REGION, SCH_POOL,                    \
          (Data **)&_datPtr, _size);                             \
   if(_ret == ROK)                                              \
   {  \
      SCH_MEM_LOG("SCH_ALLOC", __FILE__, __LINE__, __FUNCTION__, _size, _datPtr);\
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
   {\
      SCH_MEM_LOG("SCH_FREE", __FILE__, __LINE__, __FUNCTION__, _size, _datPtr);\
      SPutSBuf(SCH_MEM_REGION, SCH_POOL,                        \
            (Data *)_datPtr,(Size) _size);                            \
      _datPtr = NULLP;                                          \
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

/* Table array declarations */
int8_t coresetIdxTable[MAX_CORESET_INDEX][4];
int8_t searchSpaceIdxTable[MAX_SEARCH_SPACE_INDEX][4];
uint8_t defaultUlAckTbl[DEFAULT_UL_ACK_LIST_COUNT];
uint8_t minMsg3SchTime[MAX_NUM_MU];
uint8_t pucchResourceSet[MAX_PUCCH_RES_SET_IDX][4];
uint8_t puschDeltaTable[MAX_MU_PUSCH];
uint16_t prachCfgIdxTable[MAX_PRACH_CONFIG_IDX][8];
uint16_t numRbForPrachTable[MAX_RACH_NUM_RB_IDX][5];

/* Functions declarations : Linked list handler */
uint8_t addNodeToLList(CmLListCp *llist, void *blockToAdd, CmLList *currNode);
uint8_t deleteNodeFromLList(CmLListCp *llist, CmLList *node);

/* Functions declarations : Slot format handler */
#ifdef NR_TDD
SlotConfig schGetSlotSymbFrmt(uint16_t slot, uint32_t bitMap);
uint8_t calculateSlotPatternLength(uint8_t scs, uint8_t periodicity);
#endif

/* Functions declarations : Resource allocation handler */
uint16_t schCalcTbSize(uint32_t payLoadSize);
uint16_t schCalcNumPrb(uint16_t tbSize, uint16_t mcs, uint8_t numSymbols);
uint16_t schCalcTbSizeFromNPrb(uint16_t numPrb, uint16_t mcs, uint8_t numSymbols);
bool fillPrbBitmap(uint64_t *prbBitmap, uint16_t startPrb, uint16_t numPrb);
CmLList* isPrbAvailable(CmLListCp *freePrbBlockList, uint16_t startPrb, uint16_t numPrb);
void removeAllocatedPrbFromFreePrbList(CmLListCp *freePrbBlockList, CmLList *node, \
   uint16_t startPrb, uint16_t numPrb);
uint8_t findDmrsStartSymbol(uint16_t dlDmrsSymbBitMap);
void covertFreqDomRsrcMapToIAPIFormat(uint8_t *sourceBitMap, uint8_t *destBitMap);

uint8_t updateLcListReqPRB(CmLListCp *lcLL, uint8_t lcId, uint32_t payloadSize);
uint32_t calculateEstimateTBSize(uint32_t reqBO, uint16_t mcsIdx,uint8_t numSymbols,\
                                   uint16_t maxPRB, uint16_t *estPrb);
void deleteLcLL(CmLListCp *lcLL);
#if 0
/*Will be enabled for debugging*/
void printLcLL(CmLListCp *lcLL);
#endif
/**********************************************************************
  End of file
 **********************************************************************/
