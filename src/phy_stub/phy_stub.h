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

/* Changes the IP information 
* At DU1, SOURCE_DU_IPV4_ADDR = 192.168.130.81 and DESTINATION_DU_IPV4_ADDR = 192.168.130.83 
* At DU2, SOURCE_DU_IPV4_ADDR = 192.168.130.83 and DESTINATION_DU_IPV4_ADDR = 192.168.130.81 */
#define SOURCE_DU_IPV4_ADDR      "192.168.130.81"
#define DESTINATION_DU_IPV4_ADDR "192.168.130.83"
#define PORT_NUMBER 8080
#define NUM_THREADS 1

#ifdef NR_TDD
#define MAX_SLOT_VALUE   19
#else
#define MAX_SLOT_VALUE   9
#endif
#define MAX_SFN_VALUE    1023
#define NR_PCI            1
#define SLOT_DELAY       3
#define NUM_DRB_TO_PUMP_DATA  2
#define NUM_UL_PACKETS   1

/*UE Ids for RACH IND*/
#define UE_IDX_0     0
#define UE_IDX_1     1
#define UE_IDX_2     2

/* Default RA Preamble index to be used when Rach Indication is triggered from
 * PHY stub */
#define CB_RA_PREAMBLE_IDX 3  /* For contention based RA */
#define CF_RA_PREAMBLE_IDX 8  /* For contention free RA */

bool     slotIndicationStarted;
uint16_t sfnValue;
uint16_t slotValue;

/*Mapping between LCG ID and BSR Idx*/
typedef struct lcgBufferSize
{
   uint8_t lcgId;   /*LCG ID for which BSR will be requested*/
   /*BufferStatus Index. This is as per Spec 38.321, BSR will be reported as Index 
    * Refer table Table 6.1.3.1-1:Short_BSR and Table 6.1.3.1-2:Long_BSR*/
   uint8_t bsIdx;
}LcgBufferSize;

/* UE specific information */
typedef struct ueCb
{
   uint8_t  ueId;
   uint16_t crnti;
   bool     rachIndSent;
   bool     isCFRA;
   bool     msg3Sent;
   bool     msg5ShortBsrSent;
   bool     msg5Sent;
   bool     dlDedMsg;
   bool     msgNasAuthenticationComp;
   bool     msgNasSecurityModeComp;
   bool     msgRrcSecurityModeComp;
   bool     msgRrcReconfigComp;
   bool     msgRegistrationComp;
   uint8_t  rlcSnForSrb1;           /* Sequence number of PDU at RLC for AM mode */
   uint8_t  pdcpSn;                 /* Sequence number of PDU at PDCP */
}UeCb;

/* Database to store information for all UE */
typedef struct ueDb
{
   uint8_t   numActvUe;
   UeCb      ueCb[MAX_NUM_UE];
}UeDb;

typedef struct ipCfg
{
   uint32_t sourceDu;
   uint32_t destinationDu;
   uint16_t portNumber;
}IpCfg;

typedef struct phyDb
{
   bool   isServer;
   IpCfg  ipCfgInfo;  
   UeDb    ueDb;
}PhyDb;

PhyDb phyDb;

typedef enum
{
   MSG_TYPE_MSG3,
   MSG_TYPE_SHORT_BSR,
   MSG_TYPE_MSG5,
   MSG_TYPE_NAS_AUTHENTICATION_COMPLETE,
   MSG_TYPE_NAS_SECURITY_MODE_COMPLETE,
   MSG_TYPE_RRC_SECURITY_MODE_COMPLETE,
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
uint8_t l1SendUlUserData(uint8_t drbId, uint8_t ueIdx);
uint8_t l1SendStatusPdu();
uint16_t l1BuildAndSendSlotIndication();
uint16_t l1BuildAndSendStopInd();
int inet_pton(int af, const char *sourc, void *dst);
void *establishConnectionWithPeerL1(void *args);
uint16_t l1BuildAndSendRachInd(uint16_t slot, uint16_t sfn, uint8_t raPreambleIdx);

/**********************************************************************
         End of file
**********************************************************************/
