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

#define FILL_PST_RLC_TO_MAC(_pst, _procId, _srcInst, _event)    \
{                                                      \
   pst.selector  = ODU_SELECTOR_LWLC;                  \
   pst.srcEnt    = ENTKW;                              \
   pst.dstEnt    = ENTRG;                              \
   pst.dstInst   = 0;                                  \
   pst.srcInst   = _srcInst;                           \
   pst.dstProcId = _procId;                            \
   pst.srcProcId = _procId;                            \
   if(_srcInst == RLC_UL_INST)                         \
   {                                                   \
      pst.region    = RLC_MEM_REGION_UL;               \
   }                                                   \
   else if(_srcInst == RLC_DL_INST)                    \
   {                                                   \
      pst.region    = RLC_MEM_REGION_DL;               \
   }                                                   \
   pst.pool      = RLC_POOL;                           \
   pst.event     = _event;                             \
   pst.route     = 0;                                  \
   pst.prior     = 0;                                  \
   pst.intfVer   = 0;                                  \
}

/**********************************************************************
         End of file
**********************************************************************/
