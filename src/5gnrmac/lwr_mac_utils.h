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

/* Fill coupling type for sending msg from Lower MAC to MAC.
 * Using TC for phy stub data and LWLC with Intel L1 */
#ifdef INTEL_WLS_MEM
#define FILL_LWR_MAC_TO_MAC_SEL(_pst) _pst.selector = ODU_SELECTOR_LWLC
#else
#define FILL_LWR_MAC_TO_MAC_SEL(_pst) _pst.selector = ODU_SELECTOR_TC
#endif

/* Fill Pst structure for sending msg from Lower MAC to MAC */
#define FILL_PST_LWR_MAC_TO_MAC(_pst, _event)               \
{                                                           \
   FILL_LWR_MAC_TO_MAC_SEL(_pst);                           \
   _pst.srcEnt    = ENTLWRMAC;                              \
   _pst.dstEnt    = ENTMAC;                                 \
   _pst.dstInst   = 0;                                      \
   _pst.srcInst   = 0;                                      \
   _pst.dstProcId = ODU_GET_PROCID();                       \
   _pst.srcProcId = ODU_GET_PROCID();                       \
   _pst.region = MAC_MEM_REGION;                            \
   _pst.pool = MAC_POOL;                                    \
   _pst.event = _event;                                     \
   _pst.route = 0;                                          \
   _pst.prior = 0;                                          \
   _pst.intfVer = 0;                                        \
}

/* Fill Pst structure for sending msg from Lower MAC to itself */
#define FILL_PST_LWR_MAC_TO_LWR_MAC(_pst, _event)           \
{                                                           \
   _pst.selector  = ODU_SELECTOR_LWLC;                      \
   _pst.srcEnt    = ENTLWRMAC;                              \
   _pst.dstEnt    = ENTLWRMAC;                              \
   _pst.dstInst   = 0;                                      \
   _pst.srcInst   = 0;                                      \
   _pst.dstProcId = ODU_GET_PROCID();                       \
   _pst.srcProcId = ODU_GET_PROCID();                       \
   _pst.region = MAC_MEM_REGION;                            \
   _pst.pool = MAC_POOL;                                    \
   _pst.event = _event;                                     \
   _pst.route = 0;                                          \
   _pst.prior = 0;                                          \
   _pst.intfVer = 0;                                        \
}
 
/**********************************************************************
         End of file
**********************************************************************/
