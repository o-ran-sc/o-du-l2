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

/* Contains common utility definitions to be used at MAC */

#define MAC_MEM_REGION   4
#define MAC_POOL         1

/* allocate and zero out a MAC static buffer */
#define MAC_ALLOC(_datPtr, _size)                            \
{                                                            \
   uint8_t _ret;                                             \
   _ret = SGetSBuf(MAC_MEM_REGION, MAC_POOL,                 \
	 (Data **)&_datPtr, _size);                          \
   if(_ret == ROK)                                           \
   {                                                         \
      cmMemset((uint8_t *)_datPtr, 0, _size);                \
   }                                                         \
   else                                                      \
   {                                                         \
      _datPtr = NULLP;                                       \
   }                                                         \
}

/* free a static buffer */
#define MAC_FREE(_datPtr, _size)                             \
{                                                            \
   if(_datPtr)                                               \
   {                                                         \
      SPutSBuf(MAC_MEM_REGION, MAC_POOL,                     \
	    (Data *)_datPtr, _size);                         \
   }                                                         \
}

/* Allocate shared memory to be used for LWLC
 * during inter-layer communication */
#define MAC_ALLOC_SHRABL_BUF(_buf, _size)                    \
{                                                            \
   if(SGetStaticBuffer(MAC_MEM_REGION, MAC_POOL,             \
	    (Data **)&_buf, (Size) _size, 0) == ROK)         \
   {                                                         \
      cmMemset((uint8_t *)(_buf), 0, _size);                 \
   }                                                         \
   else                                                      \
   {                                                         \
      (_buf) = NULLP;                                        \
   }                                                         \
}

/* Free shared memory, received through LWLC */
#define MAC_FREE_SHRABL_BUF(_region, _pool,_buf, _size)      \
{                                                            \
   if (_buf != NULLP)                                        \
   {                                                         \
      (Void) SPutStaticBuffer(_region, _pool,                \
	    (Data *) _buf, (Size) _size, 0);                 \
      _buf = NULLP;                                          \
   }                                                         \
}

/* Fill Pst structure for sending msg from MAC to DU APP */
#define FILL_PST_MAC_TO_DUAPP(_pst, _event)                     \
{                                                           \
   _pst.selector  = ODU_SELECTOR_LWLC;                      \
   _pst.srcEnt    = ENTRG;                                  \
   _pst.dstEnt    = ENTDUAPP;                               \
   _pst.dstInst   = 0;                                      \
   _pst.srcInst   = macCb.macInst;                          \
   _pst.dstProcId = macCb.procId;                           \
   _pst.srcProcId = macCb.procId;                           \
   _pst.region = MAC_MEM_REGION;                            \
   _pst.pool = MAC_POOL;                                    \
   _pst.event = _event;                                     \
   _pst.route = 0;                                          \
   _pst.prior = 0;                                          \
   _pst.intfVer = 0;                                        \
}

/* Fill Pst structure for sending msg from MAC to SCH */
#define FILL_PST_MAC_TO_SCH(_pst, _event)                       \
{                                                           \
   _pst.selector  = ODU_SELECTOR_TC;                        \
   _pst.srcEnt    = ENTRG;                                  \
   _pst.dstEnt    = ENTRG;                                  \
   _pst.dstInst   = 1;                                      \
   _pst.srcInst   = macCb.macInst;                          \
   _pst.dstProcId = macCb.procId;                           \
   _pst.srcProcId = macCb.procId;                           \
   _pst.region = MAC_MEM_REGION;                            \
   _pst.pool = MAC_POOL;                                    \
   _pst.event = _event;                                     \
   _pst.route = 0;                                          \
   _pst.prior = 0;                                          \
   _pst.intfVer = 0;                                        \
}

/* Fills Pst to send msg to RLC */
#define FILL_PST_MAC_TO_RLC(_pst, _dstInst, _event)        \
{                                                      \
   pst.selector  = ODU_SELECTOR_LWLC;                  \
   pst.srcEnt    = ENTRG;                              \
   pst.dstEnt    = ENTKW;                              \
   pst.dstInst   = _dstInst;                           \
   pst.srcInst   = macCb.macInst;                      \
   pst.dstProcId = macCb.procId;                       \
   pst.srcProcId = macCb.procId;                       \
   pst.region    = MAC_MEM_REGION;                     \
   pst.pool      = MAC_POOL;                           \
   pst.event     = _event;                             \
   pst.route     = 0;                                  \
   pst.prior     = 0;                                  \
   pst.intfVer   = 0;                                  \
}

/**********************************************************************
         End of file
**********************************************************************/
