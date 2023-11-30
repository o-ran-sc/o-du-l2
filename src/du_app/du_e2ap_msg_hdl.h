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

/* This file contains all E2AP message handler related functionality */

#define ENCODE_FAIL -1

void    E2APMsgHdlr(Buffer *mBuf);
uint8_t BuildAndSendE2SetupReq();
uint8_t BuildAndSendE2ResetRequest(E2FailureCause resetCause);
uint8_t BuildAndSendE2NodeConfigUpdate(E2NodeConfigList *recvList);
uint8_t BuildAndSendRicServiceUpdate(RicServiceUpdate serviceUpdate);
uint8_t duSendE2NodeConfigurationUpdate();

uint8_t BuildAndSendStatsReq(RicSubscription *ricSubscriptionInfo);
uint8_t BuildAndSendRicSubscriptionRsp(PendingSubsRspInfo *subsRspInfo);
uint8_t BuildAndSendRicSubscriptionFailure(RicRequestId ricReqId, uint16_t ranFuncId, E2FailureCause failureCause);
uint8_t BuildAndSendRicIndication(RicSubscription *ricSubscriptionInfo, ActionInfo *actionInfo);
uint8_t BuildAndSendRicSubscriptionDeleteResponse(uint16_t ranFuncId,  RicRequestId requestId);
uint8_t BuildAndSendRicSubscriptionDeleteFailure(uint16_t ranFuncId,  RicRequestId requestId, E2FailureCause failureCause);
uint8_t BuildAndSendRicSubscriptionModificationFailure(uint16_t ranFuncId,  RicRequestId requestId, E2FailureCause failureCause);
uint8_t BuildAndSendRicSubscriptionModificationResponse(PendingSubsModRspInfo *pendingSubsModRsp);

/**********************************************************************
  End of file
 **********************************************************************/
