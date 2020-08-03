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

/*********************************************************************
  
        Name:     RLC file 
    
        Type:     C include file
  
        Desc:     This file contain the hash definations for RLC 
 
        File:     rlc.h
  
**********************************************************************/
/** @file rlc.h
@brief RLC Hash definitions
*/

#ifndef __RLC_H__
#define __RLC_H__


#include "rl_interface.h"
#include "rl_common.h"
#include "du_log.h"

#define RLC_UL_MEM_REGION    1
#define RLC_DL_MEM_REGION    4
#define RLC_POOL 1

/*******************************************************************************
 *                              Memory related Defines 
 ******************************************************************************/

/* allocate and zero out a RLC static buffer */
#define RLC_UL_ALLOC(_datPtr, _size)                         \
{                                                            \
   S16 _ret;                                                 \
   _ret = SGetSBuf(RLC_UL_MEM_REGION, RLC_POOL,              \
                    (Data **)&_datPtr, _size);               \
   if(_ret == ROK)                                           \
      cmMemset((U8*)_datPtr, 0, _size);                      \
   else                                                      \
      _datPtr = NULLP;                                       \
}

#define RLC_DL_ALLOC(_datPtr, _size)                         \
{                                                            \
   S16 _ret;                                                 \
   _ret = SGetSBuf(RLC_DL_MEM_REGION, RLC_POOL,              \
                    (Data **)&_datPtr, _size);               \
   if(_ret == ROK)                                           \
      cmMemset((U8*)_datPtr, 0, _size);                      \
   else                                                      \
      _datPtr = NULLP;                                       \
}

/* free a static buffer */
#define RLC_UL_FREE(_datPtr, _size)                          \
   if(_datPtr)                                               \
      SPutSBuf(RLC_UL_MEM_REGION, RLC_POOL,                  \
         (Data *)_datPtr, _size);

#define RLC_DL_FREE(_datPtr, _size)                          \
   if(_datPtr)                                               \
      SPutSBuf(RLC_DL_MEM_REGION, RLC_POOL,                  \
         (Data *)_datPtr, _size);

/* Allocate shared memory to be used for LWLC 
 * during inter-layer communication */
#define RLC_UL_ALLOC_SHRABL_BUF(_buf, _size)                 \
{                                                            \
   if(SGetStaticBuffer(RLC_UL_MEM_REGION, RLC_POOL,          \
      (Data **)&_buf, (Size) _size, 0) == ROK)               \
   {                                                         \
      cmMemset((U8 *)(_buf), 0, _size);                      \
   }                                                         \
   else                                                      \
   {                                                         \
      (_buf) = NULLP;                                        \
   }                                                         \
}

/* Free shared memory, received through LWLC */
#define RLC_UL_FREE_SHRABL_BUF(_region, _pool,_buf, _size)   \
{                                                            \
   if (_buf != NULLP)                                        \
   {                                                         \
      (Void) SPutStaticBuffer(_region, _pool,                \
            (Data *) _buf, (Size) _size, 0);                 \
       _buf = NULLP;                                         \
   }                                                         \
}

#define RLC_DL_ALLOC_SHRABL_BUF(_buf, _size)                 \
{                                                            \
   if(SGetStaticBuffer(RLC_DL_MEM_REGION, RLC_POOL,          \
      (Data **)&_buf, (Size) _size, 0) == ROK)               \
   {                                                         \
      cmMemset((U8 *)(_buf), 0, _size);                      \
   }                                                         \
   else                                                      \
   {                                                         \
      (_buf) = NULLP;                                        \
   }                                                         \
}

/* Free shared memory, received through LWLC */
#define RLC_DL_FREE_SHRABL_BUF(_region, _pool,_buf, _size)   \
{                                                            \
   if (_buf != NULLP)                                        \
   {                                                         \
      (Void) SPutStaticBuffer(_region, _pool,                \
            (Data *) _buf, (Size) _size, 0);                 \
       _buf = NULLP;                                         \
   }                                                         \
}

#endif /* __RLC_H__ */

/*********************************************************************
  
         End of file
**********************************************************************/
