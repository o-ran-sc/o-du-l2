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
#include <stdbool.h>
#include <string.h>
#include<ProtocolIE-Field.h>
#include "ProtocolExtensionField.h"
#include "F1AP-PDU.h"
#include "Cells-to-be-Activated-List.h"
#include "DL-CCCH-Message.h"
#include "SRB-ToAddModList.h"
#include "SRB-ToAddMod.h"
#include "RRCSetup-IEs.h"
#include "RRCSetup.h"
#include "DL-DCCH-Message.h"
#include "RRCReconfiguration-IEs.h"
#include "RRCReconfiguration.h"
#include "DRB-ToAddModList.h"
#include "DRB-ToAddMod.h"
#include "SDAP-Config.h"
#include "du_log.h"
#include "cu_stub.h"

#define ENCODE_FAIL -1
#define TRANS_ID 1
#define RRC_SIZE 1
#define SUL_BAND_COUNT 0
#define UL_SRBID        1
#define DL_SRBID        0
#define DU_ID           1
#define CU_ID           1
#define CRNTI           17017
#define CELL_INDEX      0
#define SRB0 0
#define SRB1 1
#define SRB2 2
#define DRB1 1
#define DRB2 2
#define DRB3 3
#define QCI  9
#define MAX_DRB_SET_UE_CONTEXT_SETUP_REQ 2  /*Number of DRBs to be added using UE CONTEXT SETUP procedure*/
#define MAX_DRB_SET_UE_CONTEXT_MOD_REQ 1     /*Number of DRBs to be added/setup via UE CONTEXT MODIFICATION procedure*/
#define MAX_DRB_MODIFIED_UE_MOD_REQ 1        /*Currently CU trigger Modification of 2DRBs*/
#define DRB_ID_TO_ADD_MOD MAX_DRB_SET_UE_CONTEXT_SETUP_REQ+1 /*The DRB ID: which will be added using Modification Procedure*/

/*Total num of DRBs added using UE COntext Setup and Modification procedures*/
#define MAX_NUM_DRB_ADDED_PER_UE (MAX_DRB_SET_UE_CONTEXT_SETUP_REQ + MAX_DRB_SET_UE_CONTEXT_MOD_REQ)

#define CU_UE_F1AP_ID 0
#define DU_UE_F1AP_ID 1
#define SP_CELL_ID     2
#define SERV_CELL_IDX 3
#define CU_TO_DU_RRC_INF 4

#define RRC_SETUP 1
#define REGISTRATION_ACCEPT 2
#define UE_CONTEXT_SETUP_REQ 3
#define UE_CONTEXT_SETUP_RSP 4
#define SECURITY_MODE_COMPLETE 5
#define RRC_RECONFIG 6
#define UE_CONTEXT_MOD_REQ 7
#define NUM_QOS_EXT 1


typedef struct f1apDb
{
   uint8_t dlRrcMsgCount;
   OCTET_STRING_t duToCuContainer;
}F1apMsgDb;
  
typedef struct ueInfo
{
  uint8_t ueId;
  F1apMsgDb f1apMsgDb;
}UeCb;

UeCb ueCb[MAX_NUM_UE];

void F1APMsgHdlr(Buffer *mBuf);
uint8_t BuildAndSendUeContextModificationReq(uint8_t ueId);
uint8_t BuildAndSendUeContextReleaseCommand(uint8_t cuUeF1apId, uint8_t duUeF1apId);
uint8_t BuildAndSendF1ResetReq();
/**********************************************************************
         End of file
**********************************************************************/
