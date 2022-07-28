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

#define CL_MEM_REGION   8
#define CL_POOL         0

#ifdef ODU_MEMORY_DEBUG_LOG
#define FAPI_CL_MEM_LOG(_macro, _file, _line, _func, _size, _datPtr)\
{\
   printf("\n%s=== %s +%d, %s, %d, %p\n",           \
      _macro, _file, _line, _func, _size, _datPtr); \
}
#else
#define FAPI_CL_MEM_LOG(_macro, _file, _line, _func, _size, _dataPtr) {}
#endif

/* allocate and zero out a MAC static buffer */
#define FAPI_CL_ALLOC(_datPtr, _size)                        \
{                                                            \
   uint8_t _ret;                                             \
   _ret = SGetSBuf(CL_MEM_REGION, CL_POOL,                   \
    (Data **)&_datPtr, _size);                               \
   if(_ret == ROK)                                           \
   {                                                         \
      memset(_datPtr, 0, _size);                             \
      FAPI_CL_MEM_LOG("FAPI_CL_ALLOC", __FILE__, __LINE__, __FUNCTION__, _size, _datPtr);\
   }                                                         \
   else                                                      \
   {                                                         \
      _datPtr = NULLP;                                       \
   }                                                         \
}

/* free a static buffer */
#define FAPI_CL_FREE(_datPtr, _size)                         \
{                                                            \
   if(_datPtr)                                               \
   {                                                         \
      FAPI_CL_MEM_LOG("FAPI_CL_FREE", __FILE__, __LINE__, __FUNCTION__, _size, _datPtr);\
      SPutSBuf(CL_MEM_REGION, CL_POOL,                       \
       (Data *)_datPtr, _size);                              \
      _datPtr = NULLP;                                       \
   }                                                         \
}

/* Allocate shared memory to be used for LWLC
 * during inter-layer communication */
#define FAPI_CL_ALLOC_SHRABL_BUF(_buf, _size)                \
{                                                            \
   if(SGetStaticBuffer(CL_MEM_REGION, CL_POOL,               \
       (Data **)&_buf, (Size) _size, 0) == ROK)              \
   {                                                         \
      FAPI_CL_MEM_LOG("FAPI_CL_ALLOC_SHRABL_BUF", __FILE__, __LINE__, __FUNCTION__, _size, _buf);\
      memset((_buf), 0, _size);                              \
   }                                                         \
   else                                                      \
   {                                                         \
      (_buf) = NULLP;                                        \
   }                                                         \
}

/* Free shared memory, received through LWLC */
#define FAPI_CL_FREE_SHRABL_BUF(_region, _pool,_buf, _size)      \
{                                                            \
   if (_buf != NULLP)                                        \
   {                                                         \
      FAPI_CL_MEM_LOG("FAPI_CL_FREE_SHRABL_BUF", __FILE__, __LINE__, __FUNCTION__, _size, _buf);\
      (Void) SPutStaticBuffer(_region, _pool,                \
       (Data *) _buf, (Size) _size, 0);                 \
      _buf = NULLP;                                          \
   }                                                         \
}

/* Fill Pst structure for sending msg from FAPI CL to Lower MAC */
#define FILL_PST_FAPI_CL_TO_LWR_MAC(_pst, _srcInst, _dstInst, _event)           \
{                                                           \
   _pst.selector  = ODU_SELECTOR_LWLC;                      \
   _pst.srcEnt    = ENTFAPICL;                              \
   _pst.dstEnt    = ENTLWRMAC;                              \
   _pst.dstInst   = _dstInst;                               \
   _pst.srcInst   = _srcInst;                               \
   _pst.dstProcId = ODU_GET_PROCID();                       \
   _pst.srcProcId = ODU_GET_PROCID();                       \
   _pst.region = CL_MEM_REGION;                             \
   _pst.pool = CL_POOL;                                     \
   _pst.event = _event;                                     \
   _pst.route = 0;                                          \
   _pst.prior = 0;                                          \
   _pst.intfVer = 0;                                        \
}

/* Fill Pst structure for sending msg from fapi cl to self */
#define FILL_PST_CL_TO_CL(_pst, _srcInst, _dstInst, _event)                     \
{                                                           \
   _pst.selector  = ODU_SELECTOR_LWLC;                      \
   _pst.srcEnt    = ENTFAPICL;                              \
   _pst.dstEnt    = ENTFAPICL;                              \
   _pst.dstInst   = _dstInst;                               \
   _pst.srcInst   = _srcInst;                               \
   _pst.dstProcId = ODU_GET_PROCID();                       \
   _pst.srcProcId = ODU_GET_PROCID();                       \
   _pst.region = CL_MEM_REGION;                             \
   _pst.pool = CL_POOL;                                     \
   _pst.event = _event;                                     \
   _pst.route = 0;                                          \
   _pst.prior = 0;                                          \
   _pst.intfVer = 0;                                        \
}

/**********************************************************************
                               End of file
 **********************************************************************/
