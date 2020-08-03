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
/* This file contains defines for RLC layer */

/* Memory */
#define RLC_MEM_REGION_UL    1
#define RLC_MEM_REGION_DL    4
#define RLC_POOL 1

/* Inst */
#define RLC_UL_INST   0
#define RLC_DL_INST   1

/* Fill Pst structure for sending msg from RLC to DUAPP */
#define FILL_PST_RLC_TO_DUAPP(_pst, _procId, _srcInst, _event) \
{                                                              \
   _pst.selector    = ODU_SELECTOR_LWLC;                       \
   _pst.srcEnt      = ENTKW;                                   \
   _pst.dstEnt      = ENTDUAPP;                                \
   _pst.dstInst     = 0;                                       \
   _pst.srcInst     = _srcInst;                                \
   _pst.dstProcId   = _procId;                                 \
   _pst.srcProcId   = _procId;                                 \
   if(_srcInst == RLC_UL_INST)                                 \
   {                                                           \
      _pst.region   = RLC_MEM_REGION_UL;                       \
   }                                                           \
   else if(_srcInst == RLC_DL_INST)                            \
   {                                                           \
      _pst.region   = RLC_MEM_REGION_DL;                       \
   }                                                           \
   _pst.pool        = RLC_POOL;                                \
   _pst.event       = _event;                                  \
   _pst.route       = 0;                                       \
   _pst.prior       = 0;                                       \
   _pst.intfVer     = 0;                                       \
}

/**********************************************************************
         End of file
**********************************************************************/
