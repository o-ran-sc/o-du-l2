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
#include<stdio.h>
#include<sys/types.h>
#include<string.h>
#include<ProtocolIE-FieldE2.h>
#include "E2AP-PDU.h"
#include "PLMN-IdentityE2.h"
#include "RICeventTriggerDefinition.h"
#include "odu_common_codec.h"

#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "ss_queue.h"
#include "ss_task.h"
#include "ss_msg.h"

#include "gen.x"           /* General */
#include "ssi.x"   /* System services */
#include "ss_queue.x"
#include "ss_task.x"
#include "ss_msg.x"

#include "du_log.h"

#define ENCODE_FAIL -1

S16 sctpSend(Buffer *mBuf, U8 itfType);

typedef struct e2apDb
{
  U16     ricId;                    
  PLMN_IdentityE2_t plmn;
  U32     ricReqId;
  U32     ricInstanceId;
  U32     ranFuncId;
  RICeventTriggerDefinition_t ricEventTrigger;
  U32     ricActionId;
  U32     ricActionType;
}E2apMsgDb;

S16 BuildAndSendE2SetupReq();
S16 SendE2APMsg(Region , Pool );
void E2APMsgHdlr(Buffer *mBuf);

/**********************************************************************
         End of file
**********************************************************************/
