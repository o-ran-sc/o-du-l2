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

/* This file contains all F1AP message handler related functionality */

#define TRANS_ID 1
#define RRC_SIZE 1
#define SUL_BAND_COUNT 0
#define UL_SRBID        1
#define DL_SRBID        0
#define DU_ID           1
#define CU_ID           1
#define CRNTI           17017
#define CELL_INDEX      0

void F1APMsgHdlr(Buffer *mBuf);
uint8_t BuildAndSendF1SetupReq();
uint8_t BuildAndSendDUConfigUpdate();
uint8_t BuildAndSendInitialRrcMsgTransfer(uint32_t gnbDuUeF1apId, uint16_t crnti, uint16_t rrcContSize, 
          uint8_t *rrcContainer);
uint8_t BuildAndSendULRRCMessageTransfer(DuUeCb  ueCb, uint8_t lcId,  uint16_t msgLen, uint8_t *rrcMsg);
uint8_t procUeReCfgCellInfo(MacUeCfg *macUeCfg, MacUeCfg *storedMacUeCfg, void *cellGrp);
void freeUeReCfgCellGrpInfo(MacUeCfg *macUeCfg);
uint8_t BuildAndSendUeCtxtRsp(uint8_t cellId, uint8_t ueIdx);
uint8_t getDrbLcId(uint32_t *drbBitMap);
void  freeMacLcCfg(LcCfg *lcCfg);
void  freeRlcLcCfg(RlcBearerCfg *lcCfg);
uint8_t BuildAndSendRrcDeliveryReport(uint32_t gnbCuUeF1apId, uint32_t gnbDuUeF1apId, RrcDeliveryReport *rrcDelivery);
uint8_t sctpSend(Buffer *mBuf, uint8_t itfType);
uint8_t duProcDlRrcMsg(F1DlRrcMsg *dlRrcMsg);
uint8_t duProcUeContextSetupRequest(DuUeCb *ueCb);
void duFillModulationDetails(MacUeCfg *ueCfg, MacUeCfg *oldUeCfg, void *ueCap);
int8_t getReAsmblTmr(uint8_t reAsmblTmrCfg);
int16_t getStatProhTmr(uint8_t statProhTmrCfg);
uint8_t duProcUeContextModReq(DuUeCb *ueCb);
uint8_t BuildAndSendUeContextModRsp(uint8_t cellId,uint8_t ueIdx);
uint8_t duBuildAndSendUeContextModReq(uint16_t cellId, uint16_t crnti, DuUeCfg *duUeCfg);
uint8_t BuildAndSendUeContextReleaseReq(uint16_t cellId, uint8_t ueIdx);
uint8_t BuildAndSendUeContextReleaseComplete(uint16_t cellId, uint32_t  gnbCuUeF1apId, uint32_t  gnbDuUeF1apId);
uint8_t duSendCellDeletReq(uint16_t cellId); 
uint8_t duProcUeContextReleaseCommand(DuUeCb *duUeCb);
/**********************************************************************
         End of file
**********************************************************************/
