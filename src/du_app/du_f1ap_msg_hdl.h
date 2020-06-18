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
#include<string.h>
#include<ProtocolIE-Field.h>
#include "ProtocolExtensionField.h"
#include "F1AP-PDU.h"
#include "Cells-to-be-Activated-List.h"

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
#include "odu_common_codec.h"

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
S16 procGNBDUCfgUpdAck(F1AP_PDU_t *f1apMsg);
uint8_t procDlRrcMsgTrans(F1AP_PDU_t *f1apMsg);
uint8_t BuildAndSendInitialRrcMsgTransfer(uint32_t gnbDuUeF1apId, uint16_t crnti, uint8_t *rrcContainer);
void FreeDUConfigUpdate(F1AP_PDU_t *f1apDuCfg);
/**********************************************************************
         End of file
**********************************************************************/
