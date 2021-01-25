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

/* Memory */
#define RLC_MEM_REGION_UL    1
#define RLC_MEM_REGION_DL    4
#define RLC_POOL 1

/* Inst */
#define RLC_UL_INST   0
#define RLC_DL_INST   1

#define PDCP_SN 1

/* Fill Pst structure for sending msg from RLC to DUAPP */
#define FILL_PST_RLC_TO_DUAPP(_pst, _srcInst, _event) \
{                                                              \
   _pst.selector    = ODU_SELECTOR_LWLC;                       \
   _pst.srcEnt      = ENTRLC;                                  \
   _pst.dstEnt      = ENTDUAPP;                                \
   _pst.dstInst     = 0;                                       \
   _pst.srcInst     = _srcInst;                                \
   _pst.dstProcId   = ODU_GET_PROCID();                        \
   _pst.srcProcId   = ODU_GET_PROCID();                        \
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

#define FILL_PST_RLC_TO_MAC(_pst, _srcInst, _event)    \
{                                                      \
   pst.selector  = ODU_SELECTOR_LWLC;                  \
   pst.srcEnt    = ENTRLC;                             \
   pst.dstEnt    = ENTMAC;                             \
   pst.dstInst   = 0;                                  \
   pst.srcInst   = _srcInst;                           \
   pst.dstProcId = ODU_GET_PROCID();                   \
   pst.srcProcId = ODU_GET_PROCID();                   \
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

uint16_t getTransId();

/**********************************************************************
         End of file
**********************************************************************/
