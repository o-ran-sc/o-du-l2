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
#ifndef _LWR_MAC_PHY_H_
#define _LWR_MAC_PHY_H_

#include "common_def.h"

typedef enum
{
   MSG_OK,
   MSG_INVALID_STATE,
   MSG_INVALID_CONFIG,
   SFN_OUT_OF_SYNC,
   MSG_SLOT_ERR,
   MSG_BCH_MISSING,
   MSG_INVALID_SFN,
   MSG_UL_DCI_ERR,
   MSG_TX_ERR
}ErrorCode;

#ifdef INTEL_WLS
#define WLS_MEM_FREE_PRD       10        /* Free memory after 10 slot ind */
#define LWR_MAC_WLS_BUF_SIZE   8192      /* Size of WLS memory block */

/* allocate static buffer from WLS memory */
#define WLS_MEM_ALLOC(_datPtr, _size)                        \
{                                                            \
   int16_t _ret;                                             \
   _ret = SGetSBufWls(0, 0, (Data **)&_datPtr, _size);       \
   if(_ret == ROK)                                           \
      cmMemset((U8*)_datPtr, 0, _size);                      \
   else                                                      \
      _datPtr = NULLP;                                       \
}                                                              

/* free a static buffer from WLS memory*/
#define WLS_MEM_FREE(_datPtr, _size)                         \
   if(_datPtr)                                               \
      SPutSBufWls(0, 0, (Data *)_datPtr, _size);

typedef struct wlsBlockToFree
{
   void *ptr;
   uint32_t size;
}WlsBlockToFree;

CmLListCp wlsBlockToFreeList[WLS_MEM_FREE_PRD];
extern uint8_t slotIndIdx;

EXTERN void freeWlsBlockList(uint8_t idx);
EXTERN void LwrMacEnqueueWlsBlock();
#endif /* INTEL_WLS */

EXTERN uint16_t LwrMacSendToPhy(uint8_t msgType, uint32_t msgLen, void *msg);
#endif

/**********************************************************************
         End of file
**********************************************************************/

