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
#include<stdio.h>
#include<sys/types.h>
#include<ProtocolIE-Field.h>
#include<string.h>
#include "ProtocolExtensionField.h"
#include "F1AP-PDU.h"

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

#define ENC_BUF_MAX_LEN 100

extern char encBuf[ENC_BUF_MAX_LEN];
int  encBufSize;

S16 BuildF1SetupReq();
void F1InmsgHdlr(Buffer *mBuf);

#define ENCODE_FAIL -1
#define TRANS_ID 1
#define RRC_SIZE 1

/**********************************************************************
         End of file
**********************************************************************/
