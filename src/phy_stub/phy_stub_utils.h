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

#define PHY_STUB_MEM_REGION  0
#define PHY_STUB_POOL  0

#define EVT_PHY_START_SLOT_IND 1

/* Fill Pst structure for sending msg from phy stub to self */
#define FILL_PST_PHY_TO_PHY(_pst, _event)                   \
{                                                           \
   _pst.selector  = ODU_SELECTOR_LWLC;                      \
   _pst.srcEnt    = ENTPHYSTUB;                             \
   _pst.dstEnt    = ENTPHYSTUB;                             \
   _pst.dstInst   = 0;                                      \
   _pst.srcInst   = 0;                                      \
   _pst.dstProcId = ODU_GET_PROCID();                       \
   _pst.srcProcId = ODU_GET_PROCID();                       \
   _pst.region = PHY_STUB_MEM_REGION;                       \
   _pst.pool = PHY_STUB_POOL;                               \
   _pst.event = _event;                                     \
   _pst.route = 0;                                          \
   _pst.prior = 0;                                          \
   _pst.intfVer = 0;                                        \
}

/* Fill Pst structure for sending msg from PHY stub to Lower MAC */
#define FILL_PST_PHY_STUB_TO_LWR_MAC(_pst, _event)                   \
{                                                           \
   _pst.selector  = ODU_SELECTOR_LWLC;                      \
   _pst.srcEnt    = ENTPHYSTUB;                             \
   _pst.dstEnt    = ENTLWRMAC;                              \
   _pst.dstInst   = 0;                                      \
   _pst.srcInst   = 0;                                      \
   _pst.dstProcId = ODU_GET_PROCID();                       \
   _pst.srcProcId = ODU_GET_PROCID();                       \
   _pst.region = PHY_STUB_MEM_REGION;                       \
   _pst.pool = PHY_STUB_POOL;                               \
   _pst.event = _event;                                     \
   _pst.route = 0;                                          \
   _pst.prior = 0;                                          \
   _pst.intfVer = 0;                                        \
}
/**********************************************************************
  End of file
 **********************************************************************/
