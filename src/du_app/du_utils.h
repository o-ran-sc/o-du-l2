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

/* Utility definitions to be used in du app */

/* Memory related configs */
#define DU_APP_MEM_REGION    0
#define RLC_UL_MEM_REGION    1
#define RLC_DL_MEM_REGION    4
#define MAC_MEM_REGION       4

#define DU_POOL  1
#define RLC_POOL  1
#define MAC_POOL 1

/* Events */
#define EVTCFG 0
#define EVTSCTPSTRT  1
#define EVENT_CU_DATA  2
#define EVENT_SCTP_NTFY  3
#define EVTSRVOPENREQ  4
#define EVTSRVOPENCFM  5
#define EVTTNLMGMTREQ  6
#define EVTTNLMGMTCFM  7
#define EVTSLOTIND    8
#define EVTSTARTPOLL  9
#define EVENT_RIC_DATA  10

/* allocate and zero out a static buffer */
#define DU_ALLOC(_datPtr, _size)                                \
{                                                               \
   int _ret;                                                    \
   _ret = SGetSBuf(DU_APP_MEM_REGION, DU_POOL,                  \
                    (Data **)&_datPtr, _size);                  \
   if(_ret == ROK)                                              \
      cmMemset((U8*)_datPtr, 0, _size);                         \
   else                                                         \
      _datPtr = NULLP;                                          \
}

/* free a static buffer */
#define DU_FREE(_datPtr, _size)                                 \
   if(_datPtr)                                                  \
      SPutSBuf(DU_APP_MEM_REGION, DU_POOL,                      \
         (Data *)_datPtr, _size);

/* Allocate shared memory to be used for LWLC
 * during inter-layer communication */
#define DU_ALLOC_SHRABL_BUF(_buf, _size)                     \
{                                                            \
   if(SGetStaticBuffer(DU_APP_MEM_REGION, DU_POOL,           \
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
#define DU_FREE_SHRABL_BUF(_region, _pool,_buf, _size)          \
{                                                               \
   if (_buf != NULLP)                                           \
   {                                                            \
      (Void) SPutStaticBuffer(_region, _pool,                   \
           (Data *) _buf, (Size) _size, 0);                     \
       _buf = NULLP;                                            \
   }                                                            \
}

/* Fill Pst structure for sending msg from DU APP to MAC */
#define FILL_PST_DUAPP_TO_MAC(_pst, _event)                 \
{                                                           \
   _pst.selector  = ODU_SELECTOR_LWLC;                      \
   _pst.srcEnt    = ENTDUAPP;                               \
   _pst.dstEnt    = ENTRG;                                  \
   _pst.dstInst   = 0;                                      \
   _pst.srcInst   = 0;                                      \
   _pst.dstProcId = DU_PROC;                                \
   _pst.srcProcId = DU_PROC;                                \
   _pst.region = DU_APP_MEM_REGION;                         \
   _pst.pool = DU_POOL;                                     \
   _pst.event = _event;                                     \
   _pst.route = 0;                                          \
   _pst.prior = 0;                                          \
   _pst.intfVer = 0;                                        \
}

/* Fill Pst structure for sending msg from DU_APP to RLC */
#define FILL_PST_DUAPP_TO_RLC(_pst, _dstInst, _event)       \
{                                                           \
   _pst.selector  = ODU_SELECTOR_LWLC;                      \
   _pst.srcEnt    = ENTDUAPP;                               \
   _pst.dstEnt    = ENTKW;                                  \
   _pst.dstInst   = _dstInst;                               \
   _pst.srcInst   = DU_INST;                                \
   _pst.dstProcId = DU_PROC;                                \
   _pst.srcProcId = DU_PROC;                                \
   _pst.region = DU_APP_MEM_REGION;                         \
   _pst.pool = DU_POOL;                                     \
   _pst.event = _event;                                     \
   _pst.route = 0;                                          \
   _pst.prior = 0;                                          \
   _pst.intfVer = 0;                                        \
}

/**********************************************************************
         End of file
**********************************************************************/
