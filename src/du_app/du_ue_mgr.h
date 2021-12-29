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

/* This file contains UE management handling functionality for DU APP */

#ifndef __DU_UE_MGR_H__
#define __DU_UE_MGR_H__

uint8_t duHdlRlcUlData(Pst *pst, KwuDatIndInfo* datInd, Buffer *mBuf);
uint8_t cmPkRlcDatReq(Pst * pst,RlcDatReqInfo* datReq,Buffer * mBuf);
uint8_t duBuildAndSendUeCreateReqToMac(uint16_t cellId, uint8_t ueId, uint16_t crnti, MacUeCfg *duMacUeCfg);
uint8_t duBuildAndSendUeCreateReqToRlc(uint16_t cellId, uint8_t ueId, RlcUeCfg *duRlcUeCfg);
uint8_t duCreateUeCb(UeCcchCtxt *ueCcchCtxt, uint32_t gnbCuUeF1apId);
uint8_t duProcUlCcchInd(UlCcchIndInfo *ulCcchIndInfo);
uint8_t duBuildAndSendUeDeleteReq(uint16_t cellId, uint16_t crnti);
void deleteRlcUeCfg(RlcUeCfg *ueCfg);
void freeF1UeDb(F1UeContextSetupDb *f1UeDb);
uint8_t sendUeDeleteReqToMac(uint16_t cellId, uint8_t ueId, uint16_t crnti);
uint8_t sendUeDeleteReqToRlc(uint16_t cellId, uint8_t ueId);
#endif

/**********************************************************************
         End of file
**********************************************************************/
