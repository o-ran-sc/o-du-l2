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

void schFcfsAllApisInit(SchAllApis *allFcfsApi);
uint8_t schFcfsProcessSlotInd(Pst *pst, SlotTimingInfo *slotInd);
uint8_t SchFcfsAddUeConfigReq(Pst *pst, SchUeCfgReq *ueCfg);
uint8_t schProcessRachInd(Pst *pst, RachIndInfo *rachInd);
uint8_t schProcessCrcInd(Pst *pst, CrcIndInfo *crcInd);
uint8_t SchFcfsModUeConfigReq(Pst *pst, SchUeRecfgReq *ueRecfg);
uint8_t SchFcfsUeDeleteReq(Pst *pst, SchUeDelete  *ueDelete);
uint8_t SchFcfsDlHarqInd(Pst *pst, DlHarqInd *dlHarqInd);
uint8_t schFcfsProcessCrcInd(Pst *pst, CrcIndInfo *crcInd);
uint8_t schFcfsProcessRachInd(Pst *pst, RachIndInfo *rachInd);
uint8_t schFcfsRachRsrcReq(Pst *pst, SchRachRsrcReq *schRachRsrcReq);
uint8_t schFcfsRachRsrcRel(Pst *pst, SchRachRsrcRel *schRachRsrcRel);

/**********************************************************************
  End of file
 **********************************************************************/
