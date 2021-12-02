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

uint8_t rlcSendUlRrcMsgToDu(Pst *pst, RlcUlRrcMsgInfo *ulRrcMsgInfo);
uint8_t rlcSendRrcDeliveryReportToDu(Pst *pst, RrcDeliveryReport *rrcDelivery);
uint8_t rlcSendDlRrcMsgRspToDu(Pst *pst, RlcDlRrcMsgRsp *dlRrcMsgRsp);
uint8_t rlcSendUlUserDataToDu(Pst *pst, RlcUlUserDatInfo *ulUserData);
uint8_t rlcSendUeDeleteRspToDu(Pst *pst, RlcUeDeleteRsp *ueDeleteRsp);
uint8_t rlcSendSlicePmToDu(Pst *pst, SlicePmList *sliceStats);
/**********************************************************************
         End of file
**********************************************************************/
