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

/**********************************************************************
         End of file
**********************************************************************/
