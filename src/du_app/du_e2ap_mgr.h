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
#define MAX_E2_SETUP_TMR 1
#define MAX_E2_NODE_CONFIG_UPDATE_TMR 1
#define MAX_RIC_SERVICE_UPDATE_TMR 1
#define MAX_RIC_SUBSCRIPTION_REPORTING_TMR 1

#define EVENT_E2_SETUP_TMR 1
#define EVENT_RIC_SERVICE_UPDATE_TMR 2
#define EVENT_E2_NODE_CONFIG_UPDATE_TMR 3
#define EVENT_RIC_SUBSCRIPTION_REPORTING_TMR 4

#define MAX_NUM_TRANSACTION 256 /* As per, O-RAN WG3 E2AP v3.0, section 9.2.33 */
#define MAX_RAN_FUNCTION 256        /* O-RAN.WG3.E2AP-R003-v03.00 : Section 9.1.2.2 : maxofRANfunctionID */
#define MAX_E2_NODE_COMPONENT 1024     /* O-RAN.WG3.E2AP-R003-v03.00 : Section 9.1.2.2 : maxofE2nodeComponents */
#define MAX_TNL_ASSOCIATION 32         /* O-RAN.WG3.E2AP-R003-v03.00 : Section 9.1.2.11 : maxofTNLA */
#define MAX_RIC_STYLES 64           /* O-RAN.WG3.E2SM-KPM-R003-v03.00 : Section 8.2.2.1 : maxnoofRICstyle */
#define MAX_MEASUREMENT_INFO 65535     /* O-RAN.WG3.E2SM-KPM-R003-v03.00 : Section 8.2.2.1 : maxnoofMeasurementInfo */
#define MAX_LABEL_INFO  2147483648     /* O-RAN.WG3.E2SM-KPM-R003-v03.00 : Section 8.2.1.2.1 : maxnoofLabelInfo */
#define MAX_RIC_ACTION  16          /* O-RAN.WG3.E2AP-R003-v03.00 : Section 9.1.1.1 : maxofRICActionID */
#define MAX_RIC_REQUEST 65535       /* O-RAN.WG3.E2AP-R003-v03.00 : Section 9.2.7 */
#define MAX_PENDING_SUBSCRIPTION_RSP 5  /* Number of statistics request in processing */

#define STRING_SIZE_150_BYTES 150
#define STRING_SIZE_1000_BYTES 1000

#define SHORT_NAME "ORAN-E2SM-KPM"
#define SERVICE_MODEL_OID  "1.3.6.1.4.1.53148.1.2.2.2"
#define DESCRIPTION "KPM Monitor"
#define NUM_OF_EVENT_TRIGGER_STYLE_SUPPORTED 1
#define EVENT_TRIGGER_STYLE_NAME "Periodic Report"
#define EVENT_TRIGGER_STYLE_TYPE 1
#define EVENT_TRIGGER_STYLE_FORMAT_TYPE 1
#define NUM_OF_RIC_REPORT_STYLE_SUPPORTED 1
#define REPORT_STYLE_NAME "E2 Node Measurement"
#define REPORT_STYLE_TYPE 1
#define REPORT_ACTION_FORMAT_TYPE 1
#define MEASUREMENT_TYPE_NAME (char*[]) {"RRU.PrbTotDl", "RRU.PrbTotUl"}
#define RIC_INDICATION_HEADER_FORMAT 1
#define RIC_INDICATION_MESSAGE_FORMAT 1

/* O-RAN.WG3.E2AP-R003-v03.00 : Section 9.2.26 */
typedef enum
{
   NG,
   XN,
   E1,
   F1,
   W1,
   S1,
   X2
}InterfaceType;

typedef enum
{
   E2_NODE_COMPONENT_ADD,
   E2_NODE_COMPONENT_UPDATE,
   E2_NODE_COMPONENT_DEL
}ComponentActionType;


/* O-RAN.WG3.E2AP-R003-v03.00 : Section 9.2.30 */
typedef enum
{
   RIC_SERVICE,
   SUPPORT_FUNCTIONS,
   BOTH_FUNCTIONALITY
}AssocUsage;

/* O-RAN.WG3.E2AP-R003-v03.00 : Section 9.2.11 */
typedef enum
{
   REPORT,
   INSERT,
   POLICY
}ActionType;

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

typedef struct e2FailureCause
{
   E2CauseType causeType;
   uint8_t     cause;
}E2FailureCause;

typedef struct
{
   uint8_t transactionId;
   uint8_t procedureCode;
}E2TransInfo;

typedef struct e2Transcation
{
   uint8_t     transIdCounter; /* counting the total number of DU initiated transaction */
   E2TransInfo e2InitTransaction[MAX_NUM_TRANSACTION]; /* Storing DU-initiated transactions information */
   E2TransInfo ricInitTransaction[MAX_NUM_TRANSACTION]; /* Storing RIC-initiated transactions information */
}E2Transaction;

typedef struct
{
   uint16_t   id;
   uint16_t   revisionCounter;
}RanFuncInfo;

typedef struct
{
   uint8_t     numOfRanFunToBeAdded;
   RanFuncInfo ranFunToBeAdded[MAX_RAN_FUNCTION];
   uint8_t     numOfRanFunToBeModified;
   RanFuncInfo ranFunToBeModified[MAX_RAN_FUNCTION];
   uint8_t     numOfRanFunToBeDeleted;
   RanFuncInfo ranFunToBeDeleted[MAX_RAN_FUNCTION];
}E2TmpRanFunList;

typedef struct ricServiceUpdate
{
   E2ProcedureDirection dir;
   uint8_t              transId;
   E2TmpRanFunList      recvRanFuncList;
}RicServiceUpdate;

typedef struct 
{
   RicServiceUpdate ricService;
   CmTimer          timer;
}RicServiceUpdateTimer;

typedef struct e2NodeCfgItem
{
   InterfaceType interface;
   uint64_t      componentId; 
}E2NodeConfigItem;

typedef struct e2NodeCfgList
{
   uint16_t addE2NodeCount;
   E2NodeConfigItem addE2Node[MAX_E2_NODE_COMPONENT];
   uint16_t updateE2NodeCount;
   E2NodeConfigItem updateE2Node[MAX_E2_NODE_COMPONENT];
   uint16_t removeE2NodeCount;
   E2NodeConfigItem removeE2Node[MAX_E2_NODE_COMPONENT];
}E2NodeConfigList;

typedef struct
{
   E2NodeConfigList configList;
   CmTimer    timer;
}E2NodeConfigUpdateTimer;

typedef struct e2Timer
{
   CmTimer e2SetupTimer;
   RicServiceUpdateTimer   ricServiceUpdateTimer;
   E2NodeConfigUpdateTimer e2NodeConfigUpdate;
   /* More timers can be added to this structure in future */
}E2Timer;


/* O-RAN.WG3.E2AP-R003-v03.00 : Section 9.2.4 */
typedef struct
{
   Plmn     plmnId; 
   uint32_t nearRtRicId;
}GlobalRicId;

/* O-RAN.WG3.E2SM-R003-v03.00 : Section 6.2.2.1 */
typedef struct
{
   char  shortName[STRING_SIZE_150_BYTES];
   char  serviceModelOID[STRING_SIZE_1000_BYTES];
   char  description[STRING_SIZE_150_BYTES];
}RanFunctionName;

/* O-RAN.WG3.E2SM-R003-v03.00 : Section 6.2.2.2-6.2.2.4 */
typedef struct
{
   uint8_t styleType;
   char    name[STRING_SIZE_150_BYTES];
   uint8_t formatType;
}RicStyle;

/* O-RAN.WG3.E2SM-KPM-R003-v03.00 : Section 8.2.2.1 */
typedef struct
{
   char     measurementTypeName[STRING_SIZE_150_BYTES];
   uint16_t measurementTypeId;
}MeasurementInfoForAction;

typedef struct
{
   RicStyle       reportStyle;
   CmLListCp      measurementInfoList;
}RicReportStyle;

/* O-RAN.WG3.E2AP-R003-v03.00 : Section 9.2.7 */
typedef struct
{
   uint16_t requestorId;
   uint16_t instanceId;
}RicRequestId;

typedef struct reportStartTime
{
   uint32_t timeInSec;
   uint32_t timeInMilliSec;
}ReportStartTime;

/* O-RAN.WG3.E2SM-KPM-R003-v03.00 : Section 8.2.1.1.1 */
typedef struct
{
   uint32_t reportingPeriod; /* In milliseconds */
   ReportStartTime startTime;
}EventTriggerFormat1;

/* O-RAN.WG3.E2SM-KPM-R003-v03.00 : Section 8.2.1.1 */
typedef struct
{
   uint8_t  formatType;
   union
   {
      EventTriggerFormat1 format1;
      /* More formats can be added in future */
   }choice;
}EventTriggerDefinition;

/* O-RAN.WG3.E2SM-KPM-R003-v03.00 : Section 8.3.11 */
typedef struct
{
   /* TODO : To be added in future when subcounters for any
    * measurment type is required */
}LabelInfo;

/* O-RAN.WG3.E2SM-KPM-R003-v03.00 : Section 8.2.1.2.1 */
typedef struct
{
   char     measurementTypeName[STRING_SIZE_150_BYTES];
   uint16_t measurementTypeId;

   /* As of now Labels are not used, hence it is not implemented completely */
   //uint32_t    numOfLabels;
   //LabelInfo   LabelInfoList[MAX_LABEL_INFO];

   CmLListCp   measuredValue;  /* To be filled when numOfLabels is 0, else values are calculated per Label */
}MeasurementInfo;

/* O-RAN.WG3.E2SM-KPM-R003-v03.00 : Section 8.2.1.2.1 */
typedef struct
{
   CmLListCp               measurementInfoList;    /* Each node corresponds to MeasurementInfo */
   uint32_t                granularityPeriod;      /* In millisecond */
}ActionDefFormat1;

/* O-RAN.WG3.E2SM-KPM-R003-v03.00 : Section 8.2.1.2.2 */
typedef struct
{
   /* TODO : This format will be defined in future if required */
}ActionDefFormat2;

/* O-RAN.WG3.E2SM-KPM-R003-v03.00 : Section 8.2.1.2.3 */
typedef struct
{
   /* TODO : This format will be defined in future if required */
}ActionDefFormat3;

/* O-RAN.WG3.E2SM-KPM-R003-v03.00 : Section 8.2.1.2.4 */
typedef struct
{
   /* TODO : This format will be defined in future if required */
}ActionDefFormat4;

/* O-RAN.WG3.E2SM-KPM-R003-v03.00 : Section 8.2.1.2.5 */
typedef struct
{
   /* TODO : This format will be defined in future if required */
}ActionDefFormat5;

/* O-RAN.WG3.E2SM-KPM-R003-v03.00 : Section 8.2.1.2 */
typedef struct
{
   uint8_t     styleType;
   uint8_t     formatType;
   union
   {
      ActionDefFormat1  format1;
      ActionDefFormat2  format2;
      ActionDefFormat3  format3;
      ActionDefFormat4  format4;
      ActionDefFormat5  format5;
   }choice;
}ActionDefinition;

/* O-RAN.WG3.E2AP-R003-v03.00 : Section 9.1.1.1 : maxofRICActionID */
typedef struct
{
   int16_t           actionId;     /* O-RAN.WG3.E2AP-R003-v03.00 Section 9.2.10. Valid action id range = 0..255 */
   ActionType        type;
   ActionDefinition  definition;
   ConfigType        action;
   E2FailureCause    failureCause; /* Used only when an action is required to be deleted */
}ActionInfo;

/* O-RAN.WG3.E2AP-R003-v03.00 : Section 9.1.1.1 : maxofRICActionID */
typedef struct
{
   RicRequestId           requestId;
   uint16_t               ranFuncId;
   EventTriggerDefinition eventTriggerDefinition;
   CmLListCp              actionSequence; 
   CmTimer                ricSubsReportTimer;
   ConfigType             action;
   E2FailureCause         failureCause; /* Used only when a subscription is required to be deleted */
}RicSubscription;

typedef struct rejectedAction
{
   uint8_t id;
   E2FailureCause failureCause;
}RejectedAction;

typedef struct pendingSubsRspInfo
{
   RicRequestId    requestId;
   uint16_t        ranFuncId;
   uint8_t         numOfAcceptedActions;
   uint8_t         acceptedActionList[MAX_RIC_ACTION];
   uint8_t         numOfRejectedActions;
   RejectedAction  rejectedActionList[MAX_RIC_ACTION];
}PendingSubsRspInfo;

typedef struct actionStatus
{
   uint8_t         numOfAcceptedActions;
   uint8_t         acceptedActionList[MAX_RIC_ACTION];
   uint8_t         numOfRejectedActions;
   RejectedAction  rejectedActionList[MAX_RIC_ACTION];
}ActionStatus;

typedef struct pendingSubsModRspInfo 
{
   RicRequestId requestId;
   uint16_t     ranFuncId;
   bool         addActionCompleted;
   ActionStatus addActionStatus;
   bool         modActionCompleted;
   ActionStatus modActionStatus;
   bool         removeActionCompleted;
   ActionStatus removeActionStatus;
}PendingSubsModRspInfo;

typedef struct
{
   /* O-RAN.WG3.E2SM-KPM-R003-v03.00 : Section 8.2.2.1 */
   uint16_t         id;
   RanFunctionName  name;
   uint16_t         revisionCounter;
   uint8_t          numOfEventTriggerStyleSupported;
   RicStyle         eventTriggerStyleList[MAX_RIC_STYLES];
   uint8_t          numOfReportStyleSupported;
   RicReportStyle   reportStyleList[MAX_RIC_STYLES];
   uint8_t          ricIndicationHeaderFormat;
   uint8_t          ricIndicationMessageFormat;
   /* O-RAN.WG3.E2AP-R003-v03.00 : Section 9.1.1.1 */
   CmLListCp        subscriptionList;
   uint8_t          numPendingSubsRsp;
   PendingSubsRspInfo pendingSubsRspInfo[MAX_PENDING_SUBSCRIPTION_RSP];
   uint8_t            numPendingSubsModRsp;
   PendingSubsModRspInfo pendingSubsModRspInfo[MAX_PENDING_SUBSCRIPTION_RSP];
}RanFunction;

/* O-RAN.WG3.E2AP-R003-v03.00 : Section 9.2.26-9.2.27 */
typedef struct
{
   uint8_t    reqBufSize;
   uint8_t    *componentRequestPart;
   uint8_t    rspBufSize;
   uint8_t    *componentResponsePart;
}E2NodeConfig;

typedef struct
{
   InterfaceType interfaceType;
   uint64_t      componentId; 
   E2NodeConfig  *addConfiguration;
   E2NodeConfig  *updateConfiguration;
   bool          deleteConfiguration;
}E2NodeComponent;

/* O-RAN.WG3.E2AP-R003-v03.00 : Section 9.2.29 */

typedef struct e2Ipaddr
{
   bool     ipV4Pres;
   uint32_t ipV4Addr;
}E2IpAddr;

typedef struct
{
   E2IpAddr    localIpAddress;
   uint16_t    localPort;
   E2IpAddr    destIpAddress;
   uint16_t    destPort;
   AssocUsage  usage;
}TNLAssociation;

typedef struct e2TimersInfo
{
   E2Timer  e2Timers;
   uint8_t  e2SetupTimerInterval;
}E2TimersInfo;

typedef struct
{
   uint64_t         e2NodeId; 
   GlobalRicId      ricId;
   E2Transaction    e2TransInfo;
   uint16_t         numOfRanFunction;
   RanFunction      ranFunction[MAX_RAN_FUNCTION];  
   CmLListCp        e2NodeComponentList; 
   uint8_t          numOfTNLAssoc;
   TNLAssociation   tnlAssoc[MAX_TNL_ASSOCIATION];
   E2TimersInfo     e2TimersInfo;
}E2apDb;

typedef struct e2ConnectionItem
{
   uint32_t    ipV4Addr;
   AssocUsage  usage;
}E2ConnectionItem;

typedef struct e2ConnectionList
{
   uint8_t numOfE2ConnectionSetup;
   E2ConnectionItem setupE2Connection[MAX_TNL_ASSOCIATION];
   uint8_t numOfE2ConnectionFailedToSetup;
   E2ConnectionItem failedToSetupE2Connection[MAX_TNL_ASSOCIATION];
}E2ConnectionList;

uint8_t assignTransactionId();
ActionInfo *fetchActionInfoFromActionId(uint8_t actionId, RicSubscription *ricSubscriptionInfo, CmLList ** actionNode, ConfigType configType);
RicSubscription *fetchSubsInfoFromRicReqId(RicRequestId ricReqId, RanFunction *ranFuncDb, CmLList **ricSubscriptionNode);
RanFunction *fetchRanFuncFromRanFuncId(uint16_t ranFuncId);
uint8_t fetchSubsInfoFromSubsId(uint64_t subscriptionId, RanFunction **ranFuncDb, CmLList **ricSubscriptionNode, \
   RicSubscription **ricSubscriptionInfo);
void fetchRicSubsToBeDeleted(CmLListCp *ricSubsToBeDelList);

uint8_t fillRicSubsInMacStatsReq(MacStatsReq *macStatsReq, RicSubscription* ricSubscriptionInfo);
uint8_t e2ProcStatsRsp(MacStatsRsp *statsRsp);
uint8_t e2ProcStatsInd(MacStatsInd *statsInd);
void E2apHdlRicSubsReportTmrExp(RicSubscription *ricSubscription);

uint8_t ResetE2Request(E2ProcedureDirection dir, E2FailureCause resetCause);
uint8_t SendE2APMsg(Region region, Pool pool, char *encBuf, int encBufSize);
E2NodeComponent *fetchE2NodeComponentInfo(InterfaceType interfaceType, uint64_t componentId, CmLList **e2ComponentNode);
uint8_t fillE2NodeComponentReqInfo(InterfaceType interfaceType, uint64_t componentId, uint8_t action, uint8_t bufSize, char *bufString);
uint8_t fillE2NodeComponentRspInfo(InterfaceType interfaceType, uint64_t componentId, uint8_t action, uint8_t bufSize, char *bufString);
void deleteRicSubscriptionList(CmLListCp *subscriptionList);
void deleteRicSubscriptionNode(CmLList *ricSubscriptionInfo);
void deleteMeasurementInfoList(CmLListCp *measInfoList);
void deleteActionSequence(CmLList *action);
void deleteMeasuredValueList(CmLListCp *measuredValueList);
void removeE2NodeInformation();
void encodeSubscriptionId(uint64_t *subscriptionId, uint16_t ranFuncId, RicRequestId ricReqId);
uint8_t e2ProcStatsDeleteRsp(MacStatsDeleteRsp *statsDeleteRsp);
uint8_t fillRicSubsInMacStatsModificationReq(MacStatsModificationReq *macStatsModReq, RicSubscription* ricSubscriptionInfo);
uint8_t e2ProcActionDeleteRsp(MacStatsDeleteRsp *statsDeleteRsp);
uint8_t e2ProcStatsModificationRsp(MacStatsModificationRsp *statsModificationRsp);
uint8_t duProcPendingSubsModRsp(RicSubscription *ricSubscriptionInfo, PendingSubsModRspInfo *pendingSubsModRsp);

/**********************************************************************
  End of file
 **********************************************************************/
