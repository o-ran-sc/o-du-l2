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

#define MAX_NUM_TRANSACTION 256 /* As per, O-RAN WG3 E2AP v3.0, section 9.2.33 */

typedef enum
{
   RIC_INITIATED,
   E2_NODE_INITIATED
}E2ProcedureDirection;

typedef enum
{
   E2_RAN_FUNCTION_ID_INVALID,
   E2_ACTION_NOT_SUPPORTED,
   E2_EXECESSIVE_ACTIONS,
   E2_DUPLICATE_ACTION,
   E2_DUPLICATE_EVENT_TRIGGER,
   E2_FUNCTION_RESOURCE_LIMIT,
   E2_REQUEST_ID_UNKNOWN,
   E2_INCONSISTENT_ACTION_SUBSEQUENT_ACTION_SEQUENCE,
   E2_CONTROL_MESSAGE_INVALID,
   E2_RIC_CALL_PROCESS_ID_INVALID,
   E2_CONTROL_TIMER_EXPIRED,
   E2_CONTROL_FAILED_TO_EXECUTE,
   E2_SYSTEM_NOT_READY,
   E2_RIC_REQUEST_CAUSE_UNSPECIFIED,
   E2_RIC_SUBSCRIPTION_END_TIME_EXPIRED,
   E2_RIC_SUBSCRIPTION_END_TIME_INVALID,
   E2_DUPLICATE_RIC_REQUEST_ID,
   E2_EVENT_TRIGGER_NOT_SUPPORTED,
   E2_REQUEST_INFORMATION_UNAVAILABLE,
   E2_INVALID_INFORMATION_REQUEST
}E2CauseRicRequest;

typedef enum
{
   E2_RAN_FUNCTION_NOT_SUPPORTED,
   E2_EXCESSIVE_FUNCTIONS,
   E2_RIC_RESOURCE_LIMIT,
}E2CauseRicService;

typedef enum
{
   E2_NODE_COMPONENT_UNKNOWN
}E2CauseE2Node;

typedef enum
{
   E2_TRANSPORT_CAUSE_UNSPECIFIED,
   E2_TRANSPORT_RESOURCE_UNAVAILABLE
}E2CauseTransport;

typedef enum
{
   E2_TRANSFER_SYNTAX_ERROR,
   E2_ABSTRACT_SYNTAX_ERROR_REJECT,
   E2_ABSTRACT_SYNTAX_ERROR_IGNORE_AND_NOTIFY,
   E2_MESSAGE_NOT_COMPATIBLE_WITH_RECEIVER_STATE,
   E2_SEMANTIC_ERROR,
   E2_ABSTRACT_SYNTAX_ERROR_FALSELY_CONSTRUCTED_MESSAGE,
   E2_PROTOCOL_CAUSE_UNSPECIFIED
}E2CauseProtocol;

typedef enum
{
   E2_CONTROL_PROCESSING_OVERLOAD,
   E2_HARDWARE_FAILURE,
   E2_OM_INTERVENTION,
   E2_MISCELLANEOUS_CAUSE_UNSPECIFIED
}E2CauseMisc;

typedef enum 
{
   E2_NOTHING,
   E2_RIC_REQUEST,
   E2_RIC_SERVICE,
   E2_NODE,
   E2_TRANSPORT,
   E2_PROTOCOL,
   E2_MISCELLANEOUS
}E2CauseType;

typedef uint8_t E2Cause;

typedef struct
{
   uint8_t transactionId;
   uint8_t procedureCode;
}E2TransInfo;

typedef struct e2apDb
{
   uint16_t     ricId;
   uint8_t      transIdCounter;
   E2TransInfo  onGoingTransaction[MAX_NUM_TRANSACTION];
   uint8_t      *plmn;
   uint32_t     ricReqId;
   uint32_t     ricInstanceId;
   uint32_t     ranFuncId;
   uint8_t     *ricEventTrigger;
   uint32_t     ricActionId;
   uint32_t     ricActionType;
}E2apDb;

uint8_t assignTransactionId();
uint8_t ResetE2Request(E2ProcedureDirection dir, E2CauseType type, E2Cause cause);
uint8_t SendE2APMsg(Region region, Pool pool, char *encBuf, int encBufSize);
/**********************************************************************
  End of file
 **********************************************************************/
