/*******************************************************************************
################################################################################
#   Copyright (c) [2020] [HCL Technologies Ltd.]                               #
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

/* This file contains C interface for ODU and stubs to get startup
   configuration
*/

#ifndef __CM_INTERFACE_H__
#define __CM_INTERFACE_H__

#include <stdint.h>
#include <CommonMessages.h>

#define IPV4_LEN 16
#define PORT_LEN 10



//RRM POLICY MACRO

#define MAX_MEMBER_LIST 2
#define ID_MAX_LEN 64
#define MAX_LEN 100
#define MAX_POLICY 2
#define MAX_POLICY_LIST 4
#define MCC_LEN 3
#define MNC_LEN 3
#define SD_LEN 3

//NRCELLDU MACRO
#define MAX_SUPPORTED_PLMN 2

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
   char DU_IPV4_Addr[IPV4_LEN];
   char CU_IPV4_Addr[IPV4_LEN];
   char RIC_IPV4_Addr[IPV4_LEN];
   uint16_t CU_Port;
   uint16_t DU_Port;
   uint16_t RIC_Port;
}StartupConfig;

//RRM POLICY STRUCT

typedef struct rRMPolicyMemberList
{
   uint8_t mcc[MCC_LEN];
   uint8_t mnc[MNC_LEN];
   uint8_t sd[SD_LEN];
   uint8_t sst;
}RRMPolicyMemberList;

typedef enum
{
  PRB,
  PRB_UL,
  PRB_DL,
  RRC,
  DRB
}RrmResourceType;

typedef struct rrmPolicyList
{
   char id[ID_MAX_LEN];
   RrmResourceType resourceType;
   uint8_t rRMMemberNum;
   RRMPolicyMemberList rRMPolicyMemberList[MAX_MEMBER_LIST];
   uint8_t rRMPolicyMaxRatio;
   uint8_t rRMPolicyMinRatio;
   uint8_t rRMPolicyDedicatedRatio;
}RrmPolicyList;

//NRCELLDU STRUCT

typedef enum
{
   DISABLED,      //value 0
   ENABLED,       //value 1
}OpState;

typedef enum
{
   LOCKED,         //value 0
   UNLOCKED,       //value 1
   SHUTTING_DOWN,   //value 2
}AdminState;

typedef enum
{
   IDLE,
   INACTIVE,
   ACTIVE,
}CellState;

typedef struct plmnInfo
{
   uint8_t mcc[MCC_LEN];
   uint8_t mnc[MNC_LEN];
   uint8_t sd[SD_LEN];
   uint8_t sst;
}PlmnInfo;


typedef struct nRCellDU
{
   uint32_t             cellLocalId;
   OpState              operationalState;
   AdminState           administrativeState;
   CellState            cellState;
   PlmnInfo             plmnList[MAX_SUPPORTED_PLMN];
   uint32_t             nRPCI;
   uint32_t             nRTAC;
   uint32_t             arfcnDL;
   uint32_t             arfcnUL;
   uint32_t             arfcnSUL;
   uint32_t             ssbFrequency;
   uint32_t             ssbPeriodicity;
   uint32_t             ssbSubCarrierSpacing;
   uint32_t             ssbOffset;
   uint32_t             ssbDuration;
   uint32_t             bSChannelBwUL;
   uint32_t             bSChannelBwDL;
   uint32_t             bSChannelBwSUL;
}NRCellDU;

//ME, GNB and NRCELLDU ID's struct
typedef struct managedElement
{
   char meId[ID_MAX_LEN];
   char gnbId[ID_MAX_LEN];
   char nrCellDuId[ID_MAX_LEN];
}ManagedElement;


uint8_t getStartupConfig(StartupConfig *cfg);
uint8_t getStartupConfigForStub(StartupConfig *cfg);
bool setCellOpState(uint16_t cellId, OpState opState, \
                             CellState cellState);

#ifndef ODU_TEST_STUB
//Defined in odu high
bool bringCellUp(uint16_t cellId);
bool bringCellDown(uint16_t cellId);
uint8_t setRrmPolicy(RrmPolicyList rrmPolicy[],uint8_t policyNum);
uint8_t setCellParam();
#endif //ODU_TEST_STUB
#ifdef __cplusplus
}
#endif

#endif

/**********************************************************************
         End of file
**********************************************************************/
