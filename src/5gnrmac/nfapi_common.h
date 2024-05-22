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

/*Reference: SCF225_5G_NFAPI_SPECIFICATION, v225.2.1, Issue Date: 23 Nov 2021*/

#ifndef _NFAPI_COMMON_H
#define _NFAPI_COMMON_H

#include "nfapi_vnf_fsm.h"

#define NFAPI_P5_PHY_ID    0
#define NFAPI_P5_TIMESTAMP 0 /*As per SCF225,Table 2-3:Set to zero on Tx and ignored on Rx */
#define NFAPI_P5_MORE         0
#define NFAPI_P5_SEG_NUM      0
#define NFAPI_P5_P7_SRU_TYPE  0x01 /*SCF225,Table 2-4: 0x01 for P5/P7 messages.*/

#define NFAPI_MORE_SEG_NUM(_moreField, _segStart) ((_moreField << 7) | (_segStart & 0x07))

#define NFAPI_UDP_P7_MEM_REGION   5
#define NFAPI_UDP_P7_POOL         1
#define NFAPI_UDP_P7_INST         0
#define NFAPI_UDP_P7_PROC         0
#define EVTSTARTNFAPIP7POLL       2

#ifdef ODU_MEMORY_DEBUG_LOG
#define NFAPI_UDP_P7_MEM_LOG(_macro, _file, _line, _func, _size, _datPtr)\
{\
   printf("\n%s,=== %s +%d, %s, %lu, %p \n",           \
         _macro, _file, _line, _func, (uint64_t)_size, _datPtr); \
}
#else
#define NFAPI_UDP_P7_MEM_LOG(_macro, _file, _line, _func, _size, _dataPtr) {}
#endif

/* allocate and zero out a NFAPI_UDP_P7 static buffer */
#define NFAPI_UDP_P7_ALLOC(_datPtr, _size)                            \
{                                                            \
   uint8_t _ret;                                             \
   _ret = SGetSBuf(NFAPI_UDP_P7_MEM_REGION, NFAPI_UDP_P7_POOL,                 \
	 (Data **)&_datPtr, _size);                               \
   if(_ret == ROK)                                           \
   {                                                         \
      memset(_datPtr, 0, _size);                             \
      NFAPI_UDP_P7_MEM_LOG("NFAPI_UDP_P7,ALLOC", __FILE__, __LINE__, __FUNCTION__, _size, _datPtr);\
   }                                                         \
   else                                                      \
   {                                                         \
      _datPtr = NULLP;                                       \
   }                                                         \
}

/* free a static buffer */
#define NFAPI_UDP_P7_FREE(_datPtr, _size)                             \
{                                                            \
   if(_datPtr)                                               \
   {                                                         \
      NFAPI_UDP_P7_MEM_LOG("NFAPI_UDP_P7,FREE", __FILE__, __LINE__, __FUNCTION__, _size, _datPtr);\
      SPutSBuf(NFAPI_UDP_P7_MEM_REGION, NFAPI_UDP_P7_POOL,                     \
	    (Data *)_datPtr, _size);                              \
      _datPtr = NULLP;                                       \
   }                                                         \
}

/* Allocate shared memory to be used for LWLC
 * during inter-layer communication */
#define NFAPI_UDP_P7_ALLOC_SHRABL_BUF(_buf, _size)                    \
{                                                            \
   if(SGetStaticBuffer(NFAPI_UDP_P7_MEM_REGION, NFAPI_UDP_P7_POOL,             \
	    (Data **)&_buf, (Size) _size, 0) == ROK)              \
   {                                                         \
      NFAPI_UDP_P7_MEM_LOG("NFAPI_UDP_P7,ALLOC_SHRABL_BUF", __FILE__, __LINE__, __FUNCTION__, _size, _buf);\
      memset((_buf), 0, _size);                              \
   }                                                         \
   else                                                      \
   {                                                         \
      (_buf) = NULLP;                                        \
   }                                                         \
}

/* Free shared memory, received through LWLC */
#define NFAPI_UDP_P7_FREE_SHRABL_BUF(_region, _pool,_buf, _size)      \
{                                                            \
   if (_buf != NULLP)                                        \
   {                                                         \
      NFAPI_UDP_P7_MEM_LOG("NFAPI_UDP_P7,FREE_SHRABL_BUF", __FILE__, __LINE__, __FUNCTION__, _size, _buf);\
      (Void) SPutStaticBuffer(_region, _pool,                \
	    (Data *) _buf, (Size) _size, 0);                 \
      _buf = NULLP;                                          \
   }                                                         \
}


/*Common Functions*/
void nfapiFillP5Hdr(Buffer *mBuf);
void nfapiFillMsgHdr(Buffer *mBuf, uint8_t phyId, uint16_t msgId, uint32_t msglen);

void nFapiExtractP5Hdr(nFapi_p5_hdr *p5Hdr, Buffer *mBuf);
void nFapiExtractMsgHdr(nFapi_msg_header *msgHdr, Buffer *mBuf);

NfapiPnfEvent convertNfapiP5TagValToMsgId(uint16_t tagVal);
uint8_t nfapiP7UdpRecvMsg();

#endif
