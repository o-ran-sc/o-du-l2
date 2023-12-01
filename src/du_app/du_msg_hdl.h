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


uint8_t packRlcConfigReq(Pst *pst, RlcMngmt *cfg);
uint8_t cmPkLkwCntrlReq(Pst *pst, RlcMngmt *cfg);
uint8_t cmPkLrgCfgReq(Pst *pst, RgMngmt *cfg);
uint8_t egtpHdlDatInd(EgtpMsg egtpMsg);
uint8_t BuildAndSendDUConfigUpdate();
uint16_t getTransId();
uint8_t cmPkLrgSchCfgReq(Pst * pst,RgMngmt * cfg);
uint8_t sendCellDeleteReqToMac(uint16_t cellId);
uint8_t BuildAndSendStatsDeleteReq(RicSubscription *ricSubscriptionInfo, bool deleteAllStats);
uint8_t BuildAndSendStatsModificationReqToMac(RicSubscription *ricSubscriptionInfo);
uint8_t BuildAndSendStatsModificationReq(RicSubscription *ricSubscriptionInfo);


/**********************************************************************
  End of file
 **********************************************************************/
