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

/* This file stores defines used at RLC interface */
uint8_t fillRlcUeCreateRsp(RlcUeCreateRsp *rlcCfgRsp, RlcCfgCfmInfo *rlcCRsp);
uint8_t SendRlcUeCreateRspToDu(Pst *pst, RlcCfgCfmInfo *cfgRsp);
uint8_t SendRlcUeReconfigRspToDu(Pst *pst, RlcCfgCfmInfo *cfgRsp);
uint8_t sendRlcUeDeleteRspToDu(uint16_t cellId, uint8_t ueId, CauseOfResult  status);
uint8_t sendRlcUeReestablishRspToDu(uint16_t cellId,uint8_t ueId, CauseOfResult  status);
/**********************************************************************
         End of file
**********************************************************************/
