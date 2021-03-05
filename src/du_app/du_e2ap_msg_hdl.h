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

#include "E2setupRequest.h"
#define ENCODE_FAIL -1

uint8_t sctpSend(Buffer *mBuf, uint8_t itfType);

typedef struct e2apDb
{
   uint16_t     ricId;                    
   uint8_t      *plmn;
   uint32_t     ricReqId;
   uint32_t     ricInstanceId;
   uint32_t     ranFuncId;
   uint8_t     *ricEventTrigger;
   uint32_t     ricActionId;
   uint32_t     ricActionType;
}E2apMsgDb;

E2apMsgDb e2apMsgDb;
uint8_t BuildAndSendE2SetupReq();
uint8_t SendE2APMsg(Region , Pool );
void E2APMsgHdlr(Buffer *mBuf);
uint8_t deAllocateE2SetupReqMsg(E2AP_PDU_t *e2apMsg, E2setupRequest_t *e2SetupReq, uint8_t idx);
/**********************************************************************
  End of file
 **********************************************************************/
