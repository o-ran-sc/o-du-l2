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

#ifdef INTEL_WLS_MEM
#define LWR_MAC_WLS_BUF_SIZE   32000      /* Size of WLS memory block */
#define EVT_START_WLS_RCVR     1

/* allocate static buffer from WLS memory */
#define WLS_MEM_ALLOC(_datPtr, _size)                        \
{                                                            \
   uint8_t _ret;                                             \
   _ret = SGetSBufWls(0, 0, (Data **)&_datPtr, _size);       \
   if(_ret == ROK)                                           \
      memset(_datPtr, 0, _size);                      \
   else                                                      \
      _datPtr = NULLP;                                       \
}                                                              

/* free a static buffer from WLS memory*/
#define WLS_MEM_FREE(_datPtr, _size)                         \
   if(_datPtr)                                               \
   {                                                         \
      SPutSBufWls(0, 0, (Data *)_datPtr, _size);             \
      _datPtr = NULLP;                                       \
   }

typedef struct wlsBlockToFree
{
   void     *ptr;
   uint32_t size;
}WlsBlockToFree;

CmLListCp wlsBlockToFreeList[WLS_MEM_FREE_PRD];

void freeWlsBlockList(uint8_t idx);
void LwrMacEnqueueWlsBlock();
void LwrMacRecvPhyMsg();
void LwrMacStartWlsRcvr();
#endif /* INTEL_WLS_MEM */

uint8_t LwrMacSendToL1(void *msg);
#endif

/**********************************************************************
  End of file
 **********************************************************************/

