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

/********************************************************************20**
  
     Name:     common memory allocation library 
  
     Type:     C include file
  
     Desc:     Hash Defines 
 
     File:     cm_mblk.h
 
*********************************************************************21*/
  
#ifndef __CMMBLKH__
#define __CMMBLKH__

/* Hash defines for alignment bytes */
#define CM_ALIGNBOUNDARY_FOUR     4   /* Align 32 bit */ 
#define CM_ALIGNBOUNDARY_EIGHT    8   /* Align 64 bit */

/* Macro Definitions */

#define CM_INIT_MEMCP(_memCp,_maxSize,_sMem) \
{ \
  _memCp->first = NULLP; \
  _memCp->last = NULLP; \
  _memCp->count = 0;\
  _memCp->memCb.maxSize = _maxSize;\
  _memCp->memCb.memAllocated = 0; \
  _memCp->memCb.initPtr = NULLP; \
  _memCp->memCb.runPtr = NULLP; \
  _memCp->memCb.sMem.region = _sMem->region; \
  _memCp->memCb.sMem.pool = _sMem->pool; \
}

#define CM_INIT_MEMCPVAR(_memCp) \
{ \
  _memCp->first = NULLP; \
  _memCp->last = NULLP; \
  _memCp->count = 0;\
  _memCp->memCb.memAllocated = 0; \
  _memCp->memCb.initPtr = NULLP; \
  _memCp->memCb.runPtr = NULLP; \
}

#ifdef ALIGN_64BIT
#define CM_ALIGN_SIZE(_size) \
{ \
  if( (_size % CM_ALIGNBOUNDARY_EIGHT) ) \
     _size = _size + ( CM_ALIGNBOUNDARY_EIGHT -  \
                  (_size % CM_ALIGNBOUNDARY_EIGHT)); \
}
#else
#define CM_ALIGN_SIZE(_size) \
{ \
  if( (_size % CM_ALIGNBOUNDARY_FOUR) ) \
    _size = _size + ( CM_ALIGNBOUNDARY_FOUR -  \
                  (_size % CM_ALIGNBOUNDARY_FOUR)); \
}
#endif  /* ALIGN_64BIT */

#define CMCHKUNPKPTR(func, val, ptr, mBuf) \
   { \
      S16 ret; \
      if ((ret = func(val, ptr, mBuf)) != ROK) \
         return (ret); \
   }

#define CMGETMBLK(ptr, size, pptr) \
   { \
      S16 ret; \
      ret = cmGetMem( ptr, size, pptr); \
      if (ret != ROK) \
      { \
          return RFAILED; \
      } \
   }

#endif /* __CMMBLKH__ */

 
/********************************************************************30**
  
         End of file
**********************************************************************/
