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

/* This file contains message handling functionality for DU APP */
#ifndef __DU_CELL_MGR_H__
#define __DU_CELL_MGR_H__

uint8_t duBuildAndSendMacCellCfg(uint16_t cellId);
uint8_t duProcCellsToBeActivated(uint8_t *plmnStr, uint16_t nci, uint16_t nRPci);
void duProcF1SetupRsp();
uint8_t duGetCellCb(uint16_t cellId, DuCellCb **cellCb);
uint8_t BuildAndSendSliceConfigReq();
uint8_t BuildAndSendSliceRecfgReq();
uint8_t processPagingMsg(uint16_t cellId, DuPagingMsg *rcvdF1apPagingParam);
uint8_t BuildAndSendDlPcchIndToMac(uint16_t cellId, uint16_t pf, uint8_t i_s, CmLListCp *pageUeLL);
#endif
/**********************************************************************
  End of file
 **********************************************************************/
