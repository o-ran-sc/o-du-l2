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
#include <unistd.h>

#ifdef NR_TDD
#define MAX_SLOT_VALUE   19
#else
#define MAX_SLOT_VALUE   9
#endif
#define MAX_SFN_VALUE    1023
#define NR_PCI            1
#define SLOT_DELAY       3
#define NUMDRBS_TO_PUMP  2
/*UE Ids for RACH IND*/
#define UE_IDX_0     0
#define UE_IDX_1     1

bool     slotIndicationStarted;
uint16_t sfnValue;
uint16_t slotValue;

/* UE specific information */
typedef struct ueCb
{
   uint8_t  ueId;
   uint16_t crnti;
   bool     rachIndSent;
   bool     msg3Sent;
   bool     msg5ShortBsrSent;
   bool     msg5Sent;
   bool     dlDedMsg;
   bool     msgSecurityModeComp;
   bool 	   msgRrcReconfiguration;
   bool 	   msgRegistrationComp;
}UeCb;

/* Database to store information for all UE */
typedef struct ueDb
{
   uint8_t   numActvUe;
   UeCb      ueCb[MAX_NUM_UE];
}UeDb;

UeDb ueDb;

typedef enum
{
   MSG_TYPE_MSG3,
   MSG_TYPE_SHORT_BSR,
   MSG_TYPE_MSG5,
   MSG_TYPE_SECURITY_MODE_COMPLETE,
   MSG_TYPE_REGISTRATION_COMPLETE,
   MSG_TYPE_RRC_RECONFIG_COMPLETE
}MsgType;

void phyToMac ARGS((uint16_t msgType, uint32_t msgLen,void *msg));
#ifdef INTEL_FAPI
void fillTlvs ARGS((fapi_uint16_tlv_t *tlv, uint16_t tag, uint16_t
length, uint16_t value, uint32_t *msgLen));
void fillMsgHeader ARGS((fapi_msg_t *hdr, uint16_t msgType, uint16_t msgLen));
#endif
void procPhyMessages(uint16_t msgType, uint32_t msgSize, void *msg);
void l1StartConsoleHandler();
void l1HdlSlotIndicaion(bool);
void GenerateTicks();

/**********************************************************************
         End of file
**********************************************************************/
