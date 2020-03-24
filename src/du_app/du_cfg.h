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

#ifndef __DU_CONFIG_H_
#define __DU_CONFIG_H__

#include "du_mgr.h"
#include "du_app_mac_inf.h"
#include "du_log.h"
#include "BIT_STRING.h"

/* MACROS */
#define DU_INST 0
#define DU_ID 1
#define DU_IP_V4_ADDR "10.0.2.20"
#define CU_IP_V4_ADDR "10.0.2.25"
#define RIC_IP_V4_ADDR "10.0.2.30"
#define DU_PORT 38472
#define CU_PORT 38472
#define RIC_PORT 38482
#define DU_EGTP_PORT  39001
#define CU_EGTP_PORT  39002
#define NR_PCI 1
#define NR_CELL_ID 1
#define DU_NAME "ORAN_OAM_DU"
#define CELL_TYPE SMALL
#define DUPLEX_MODE DUP_MODE_TDD
#define DU_TAC 1
#define PLMN_MCC0 3
#define PLMN_MCC1 1
#define PLMN_MCC2 1
#define PLMN_MNC0 4
#define PLMN_MNC1 8
#define PLMN_MNC2 0
#define NR_ARFCN  2079427
#define SUL_ARFCN 100
#define NR_FREQ_BAND 257
#define SUL_BAND 2
#define TIME_CFG 0
#define CARRIER_IDX 1
#define NUM_TX_ANT 2
#define NUM_RX_ANT 2
#define FREQ_SHIFT_7P5KHZ FALSE
#define SSB_PBCH_PWR 0
#define BCH_PAYLOAD MAC_GEN_FULL_PBCH_PAYLD
#define SUBCARRIER_SPACING 3
#define PRB_OFFSET 0
#define BETA_PSS BETA_PSS_0DB
#define SSB_PERIODICITTY SSB_PRDCTY_MS_20
#define SSB_SUBCARRIER_OFFSET 0
#define SSB_MULT_CARRIER_BAND FALSE
#define MULT_CELL_CARRIER FALSE

#define PRACH_SEQ_LEN SHORT_SEQUENCE
#define PRACH_SUBCARRIER_SPACING 3
#define PRACH_RESTRICTED_SET_CFG UNRESTRICTED
#define NUM_PRACH_FDM 1
#define ROOT_SEQ_IDX 0
#define NUM_ROOT_SEQ 1
#define ZERO_CORRELATION_ZONE_CFG 2
#define NUM_UNUSED_ROOT_SEQ 1
#define UNUSED_ROOT_SEQ 1
#define SSB_PER_RACH 3
#define PRACH_MULT_CARRIER_BAND FALSE
#define TDD_PERIODICITY TX_PRDCTY_MS_2P5
#define RSS_MEASUREMENT_UNIT DONT_REPORT_RSSI

#define DU_PROC  0
#define DU_INST 0
#define DU_POOL  1
#define MAX_DU_PORT 2
#define F1_INTERFACE 0
#define E2_INTERFACE 1

#define SCTP_INST 0
#define EGTP_INST 0

/* Events */
#define EVTCFG 0

/* Selector */
#define DU_SELECTOR_LC   0
#define DU_SELECTOR_TC   1
#define DU_SELECTOR_LWLC 2

#ifdef EGTP_TEST
/* Macro definitions for EGTP procedures */
#define EGTP_LCL_TEID 1     /* EGTP local tunnel id */
#define EGTP_REM_TEID 10    /* EGTP remote tinnel id */
#endif


/* Macro definitions for F1 procedures */
#define CU_DU_NAME_LEN_MAX 30      /* Max length of CU/DU name string */
#define MAX_F1_CONNECTIONS 65536    /* Max num of F1 connections */
#define DU_MAX_CELLS       512      /* Max num of cells served by gNB-DU */
#define MAX_PLMN           6        /* Max num of broadcast PLMN ids */
#define MAXNRARFCN         3279165  /* Maximum values of NRAFCN */
#define MAXNRCELLBANDS     2       /* Maximum number of frequency bands */
#define MAXNUMOFSLICEITEMS 1024     /* Maximum number of signalled slice support items */
#define MAXBPLMNNRMINUS1   1       /* Maximum number of PLMN Ids broadcast in an NR cell minus 1 */
#define MAXNUMOFSIBTYPES   32       /* Maximum number of SIB types */
#define MAX_UEID           512      /* Maximum number of UE Ids */
#define MAX_TNL_ASSOC      32       /* Max num of TNL Assoc between CU and DU */
#define MAXCELLINENB       256      /* Max num of cells served by eNB */
#define MAXNUMOFUEID       65536    /* Max num of UEs served by DU */
#define MAXNUMOFUACPLMN    12       /* Maximum number of PLMN ids*/
#define MAXNUMOFUACPERPLMN 64       /* Maximum number of signalled categories per PLMN */
#define DEFAULT_CELLS      1        /* Max num of broadcast PLMN ids */


/* Macro definitions for MIB/SIB1 */
#define SYS_FRAME_NUM 0
#define SPARE 0
#define SSB_SC_OFFSET 8
#define CORESET_ZERO 1
#define SEARCH_SPACE_ZERO 8
#define DU_RANAC 1
#define CELL_IDENTITY 1


typedef enum
{
   GNBDU,
   GNBCU 
}F1Entity;

typedef enum
{
   UNSPECIFIED,
   RL_FAILURE_RLC,
   INVALID_CU_UE_F1AP_ID,
   INVALID_DU_UE_F1AP_ID,
   INTERACTION_WITH_OTHER_PROC,
   INVALID_QCI_VALUE,
   RADIO_RELATED_REASON,
   INSUFFICIENT_RADIO_RESOURCE,
   PROCEDURE_CANCELLED,
   NORMAL_RELEASE,
   UNAVAILABLE_CELL,
   RL_FAILURE_OTHERS,
   UE_ACCESS_REJECTED,
   SLICE_RESOURCE_UNAVAILABLE,
   AMF_ABNORMAL_RELEASE,
   INVALID_PLMN
}RadioNwCause;

typedef enum
{
   TRANS_CAUSE_UNSPECIFIED,
   TRANSPORT_RESOURCE_UNAVAIL
}TransLayerCause;

typedef enum
{
   TRANSFER_SYNTAX_ERR,
   ABSTRACT_SYNTAX_ERR_REJECT,
   ABSTRACT_SYNTAX_ERR_NOTIFY,
   INCOMPATIBLE_MESSAGE,
   SEMANTIC_ERROR,
   ABSTRACT_SYNTAX_ERR_MSG_CONSTRUCT,
   PROTOCOL_CAUSE_UNSPECIFIED
}ProtocolCause;

typedef enum
{
   CONTROL_PROC_OVERLOAD,
   INSUFF_USER_PLANE_RESOURCE,
   HARDWARE_FAILURE,
   O_AND_M_INTERVENTION,
   MISC_CAUSE_UNSPECIFIED
}MiscCause;

typedef enum
{
  ALL
}F1ResetAll;

typedef enum 
{
  IN_SERVICE,
  OUT_OF_SERVICE
}SrvState;

typedef enum
{
   INITIATING_MSG,
   SUCCESSFUL_OUTCOME,
   UNSUCCESSFUL_OUTCOME
}F1TriggMsgVal;

typedef enum
{
   CRIT_REJECT,
   CRIT_IGNORE,
   CRIT_NOTIFY
}F1ProcCritVal;

typedef enum
{
   NOT_UNDERSTOOD,
   MISSING_IE
}F1ErrorType;

typedef enum
{
   SCS_15,
   SCS_30,
   SCS_60,
   SCS_120
}F1NrScs;

typedef enum
{
   NRB_11,
   NRB_18,
   NRB_24,
   NRB_25,
   NRB_31,
   NRB_32,
   NRB_38,
   NRB_51,
   NRB_52,
   NRB_65,
   NRB_66,
   NRB_78,
   NRB_79,
   NRB_93,
   NRB_106,
   NRB_107,
   NRB_121,
   NRB_132,
   NRB_133,
   NRB_135,
   NRB_160,
   NRB_162,
   NRB_189,
   NRB_216,
   NRB_217,
   NRB_245,
   NRB_264,
   NRB_270,
   NRB_273
}F1Nrb;

typedef enum
{
   VERY_SMALL,
   SMALL,
   MEDIUM,
   LARGE
}F1CellType;

typedef enum
{
   DL,
   UL,
   DL_UL
}F1CellDir;

typedef enum
{
   UE_USAGE,
   NON_UE_USAGE,
   BOTH
}F1AssocUsage;

typedef enum
{
   BARRED,
   NOT_BARRED
}CellBarred;

typedef enum
{
   OFFER,
   EXECUTION
}F1ReqType;

typedef enum
{
   IGNORE_YES,
   IGNORE_NO
}F1IgnoreReq;

typedef enum
{
   OVERLOADED,
   NOT_OVERLOADED
}F1GnbDuOvrloadInfo;

typedef enum
{
   REJECT_RRC_CONN_NON_EMERG_MODT,
   REJECT_RRC_CONN_SIGNALLING,
   PERMIT_EMERG_SESSION_AND_MOBILE_TERM_SERVICE,
   PERMIT_HIGH_PRIOR_SESSION_AND_MOBILE_TERM_SERVICE
}F1UacStandardAction;

typedef struct f1RrcVersion
{
   char    rrcVer[30];     /* Latest RRC Version */
   U32   extRrcVer;  /* Latest RRC version extended */
}F1RrcVersion;

typedef struct f1FailCause
{
   union
   {
      RadioNwCause      radioNwCause;     /* Radio Network Layer Cause */
      TransLayerCause   transLayerCause;  /* Transport Layer Cause */
      ProtocolCause     protCause;        /* Protocol Cause */
      MiscCause         miscCause;        /* Miscellaneous Cause */
   }cause;
}F1FailCause;

typedef struct f1ProcCode
{
   Bool   pres;
   U8     value;
}F1ProcCode;

typedef struct f1TriggMsg
{
   Bool   pres;
   F1TriggMsgVal   value;
}F1TriggMsg;

typedef struct f1ProcCrit
{
   Bool   pres;
   F1ProcCritVal   value;
}F1ProcCrit;

typedef struct f1CritDignosIE
{
   F1ProcCritVal   ieCritVal;
   U16             ieId;
   F1ErrorType       type;
}F1CritDignosIE;

typedef struct f1CritDiagnostic
{
   Bool         pres;
   F1ProcCode   procCode;      /* Procedure Code */
   F1TriggMsg   triggeringMsg; /* Triggering Message */
   F1ProcCrit   procCrit;      /* Criticality of Triggering msg/proc */
   U32          transId;
   F1CritDignosIE diagnosIE;
}F1CritDiagnostic;

typedef struct f1FailureIE
{
   U32                transId;        /* Uniquely identify transaction */
   F1FailCause        cause;          /* Failure cause */
   U8                 timeToWait;     /* Seconds to be waited before retry */
   F1CritDiagnostic   critDiagnostic; /* Critical diagnostics */
}F1FailureIE;

typedef struct f1LogicalConnUeAssoc
{
   U32   gnbCuUeF1apId;
   U32   gnbDuUeF1apId;
}F1LogicalConnUeAssoc;

typedef struct f1ResetType
{
   union
   {
      F1ResetAll             resetAll; /* Reset All */
      F1LogicalConnUeAssoc   ueAssocLogicalConn[MAX_F1_CONNECTIONS]; /* In case of Partial Reset : UE associated logical F1 conn list*/
   }type;
}F1ResetType;

typedef struct f1PLMN
{
  U8 mcc[3];
  U8 mnc[3];
}PlmnId;

typedef struct nrEcgi
{
  PlmnId  plmn;
  U16     cellId;
}NrEcgi;

typedef struct f1SibType
{
   U8   sibType;
   U8   sibMsg;
   U8   value;
}F1SibType;

typedef struct f1TxBw
{
   F1NrScs   nrScs; /* NR subcarrier spacing */
   F1Nrb     nrb;   /* Transmission bandwidth expressed in units of RBs */
}F1TxBw;

typedef struct f1SulInfo
{
   U32      sulArfcn; /* SUL ARFCN */
   F1TxBw   sulTxBw;  /* SUL transmission bandwidth */ 
}F1SulInfo;

typedef struct f1FreqBand
{
  U16   nrFreqBand;
  U16   sulBand[MAXNRCELLBANDS];
}F1FreqBand;

typedef struct f1NrFreqInfo
{
   U32        nrArfcn;
   F1SulInfo  sulInfo;
   F1FreqBand freqBand[MAXNRCELLBANDS];
}F1NrFreqInfo;

typedef struct f1NrFddInfo
{
   F1NrFreqInfo   ulNrFreqInfo;  /* UL NR Frequency Info */
   F1NrFreqInfo   dlNrFreqInfo;  /* DL NR Frequency Info */
   F1TxBw         ulTxBw;        /* UL Transmission bandwidth */
   F1TxBw         dlTxBw;        /* DL Transmission bandwidth */
}F1NrFddInfo;

typedef struct f1NrTddInfo
{
   F1NrFreqInfo   nrFreqInfo;   /* NR Frequency Info */
   F1TxBw       nrTxBw;       /* NR Transmission Bandwidth */
}F1NrTddInfo;

typedef struct f1NrModeInfo
{
   union
   {
      F1NrFddInfo  fdd;
      F1NrTddInfo  tdd;
   }mode;
}NrModeInfo;

typedef struct f1EutraFddInfo
{
   U16   ulOffset;   /* UL offset to Point A */
   U16   dlOffset;   /* DL offset to Point A */
}F1EutraFDDInfo;

typedef struct f1EutraTddInfo
{
   U16   tddOffset;  /* Offset to Point A */
}F1EutraTDDInfo;

typedef struct f1EutraModeInfo
{
   union
   {
      F1EutraFDDInfo  fdd;
      F1EutraTDDInfo  tdd;
   }mode;
}F1EutraModeInfo;

typedef struct f1Snsaai
{
   U8   sst;
   U32  sd;
}F1Snsaai;

typedef struct epIpAddr
{
   char transportAddr[20]; /* Transport Layer Address */
}EpIpAddr;

typedef struct epIpAddrPort
{
    EpIpAddr epIpAddr;
    char   port[2];
}EpIpAddrPort;

typedef struct f1TaiSliceSuppLst
{
   Bool       pres;
   F1Snsaai   snssai[MAXNUMOFSLICEITEMS];   
}F1TaiSliceSuppLst;

typedef struct f1SrvdPlmn
{
   PlmnId              plmn;
   F1TaiSliceSuppLst   taiSliceSuppLst;
}F1SrvdPlmn;

typedef struct f1BrdcstPlmnInfo
{
   PlmnId   plmn[MAX_PLMN];     /* PLMN id list */
   PlmnId   extPlmn[MAX_PLMN];   /* Extended PLMN id list */
   U16      tac;                     /* 5GS-TAC */
   U32      nrCellId;                /* NR Cell id */
   U8       ranac;                   /* RAN Area code */
}F1BrdcstPlmnInfo;

typedef struct f1CellInfo
{
   NrEcgi   nrCgi;                   /* Cell global Identity */
   U32      nrPci;                   /* Physical Cell Identity */
   PlmnId   plmn[MAX_PLMN];     /* Available PLMN list */
   PlmnId   extPlmn[MAX_PLMN];  /* Extended available PLMN list */
}F1CellInfo;

typedef struct f1DuCellInfo
{
   F1CellInfo         cellInfo;     /* cell info */
   U16                tac;          /* tracking area code */
   U16                epsTac;       /* Configured EPS TAC */
   NrModeInfo         f1Mode;       /* NR mode info : FDD/TDD */
   U8                 measTimeCfg;  /* Measurement timing configuration */
   F1CellDir          cellDir;      /* Cell Direction */
   F1CellType         cellType;     /* Cell Type */
   F1BrdcstPlmnInfo   brdcstPlmnInfo[MAXBPLMNNRMINUS1]; /* Broadcast PLMN Identity Info List */
}F1DuCellInfo;

typedef struct f1DuSysInfo
{
   U8   *mibMsg;   /* MIB message */
   U8   *sib1Msg;  /* SIB1 message */
}F1DuSysInfo;

typedef struct f1CuSysInfo
{
   F1SibType   sibTypeToUpd[MAXNUMOFSIBTYPES];   /* SIB types to be updated */
}F1CuSysInfo;

typedef struct f1DuSrvdCellInfo
{
   F1DuCellInfo   duCellInfo;   /* gNB-DU served cell info */
   F1DuSysInfo    duSysInfo;    /* gNB-DU System Information */
}F1DuSrvdCellInfo;

typedef struct f1DuSrvdCellToDel
{
   NrEcgi             oldNrCgi;     /* Old NR CGI */
   F1DuSrvdCellInfo   srvdCellInfo; /* Info of served cell to be deleted */ 
}F1DuSrvdCellToDel;

typedef struct f1CuActCellInfo
{
   F1CellInfo    cuCellInfo;   /* gNB-CU available cell info */
   F1CuSysInfo   cuSysInfo;    /* gNB-CU System Information */
}F1CuActCellInfo;

typedef struct f1ActCellFail
{
   NrEcgi        nrCgi;
   F1FailCause   cause;
}F1ActCellFail;

typedef struct srvStatus
{
  SrvState    state;
  Bool        switchOffOngoing;
}SrvStatus;

typedef struct f1CellStatus
{
   NrEcgi      nrEcgi;
   SrvStatus   status;   /* Service status */
}F1CellStatus;

typedef struct f1DedSIDelUE
{
   U32      gnbCuUeF1apId;
   NrEcgi   nrEcgi;
}F1DedSIDelUE;

typedef struct tnlAssocInfo
{
   Bool           pres;
   EpIpAddr       epIpAddr;      /* Endpoint-IP Address */
   EpIpAddrPort   epIpAddrport;  /* Endpoint-IP Address and Port */
}F1TnlAssocAddr;

typedef struct f1TnlAssocUsage
{
    Bool           pres;
    F1AssocUsage   usage;
}F1TnlAssocUsage;

typedef struct f1TnlAssoc
{
   F1TnlAssocAddr   assocInfo;    /* TNL Assoc Transport Layer Info */
   F1TnlAssocUsage  assocUsage;   /* TNL Assoc usage */
}F1TnlAssoc;

typedef struct f1TnlAssocToRmv
{
   F1TnlAssocAddr   assocInfoCu;   /* TNL Assoc Transport Layer Info of CU */
   F1TnlAssocAddr   assocInfoDu;   /* TNL Assoc Transport Layer Info of DU */
}F1TnlAssocToRmv;

typedef struct f1TnlAssocSetupFail
{
   F1TnlAssocAddr   assocInfo;
   F1FailCause      cause;
}F1TnlAssocSetupFail;


typedef struct f1CellBarred
{
   NrEcgi       nrcgi;
   CellBarred   cellBarred;
}F1CellBarred;

typedef struct f1EutraCell
{
   U32                eutraCellId; /* EUTRA cell id */
   F1EutraModeInfo    mode;        /* EUTRA mode info : FDD/TDD */
   U8                 rsrcInd;     /* Protected EUTRA resource Indication */
}F1EutraCell;

typedef struct f1ProtectEUTRARsrc
{
   U32           specShareGrpId;               /* Spectrum sharing group id */
   F1EutraCell   eutraCellList[MAXCELLINENB];  /* EUTRA cells list */
}F1ProtectEUTRARsrc;

typedef struct f1UacOperatorDefined
{
   U8   accessCategory;
   U8   accessId; 
}F1UacOperatorDefined;

typedef struct f1UacCategoryType
{
   union
   {
      F1UacStandardAction    action;      /* UAC standardized action */
      F1UacOperatorDefined   operatorDef; /* UAC operator defined */
   }type;
}F1UacCategoryType;

typedef struct f1UacType
{
   U8                  uacReducInd;     /* Value 0 means no access rate reduction. Value 100 means full access rate reduction */
   F1UacCategoryType   uacCategoryType; /* UAC Category type */
}F1UacType;

/* Unified Access Class Assistance Information */
typedef struct f1UacAssistInfo
{
   PlmnId      plmn[MAXNUMOFUACPLMN];        /* UAC PLMN list */
   F1UacType   uacType[MAXNUMOFUACPERPLMN];  /* UAC Type list */
}F1UacAssistInfo;

/* F1 setup related structures */

typedef struct f1SetupReq
{
  U32                transId;                       /* Uniquely identify transaction */
  U32                duId;                          /* DU ID */ 
  char               duName[CU_DU_NAME_LEN_MAX];    /* DU name */
  F1DuSrvdCellInfo   srvdCellLst[DU_MAX_CELLS];   /* Serving cell list */
  F1RrcVersion       rrcVersion;                    /* RRC version */
}F1SetupReq;

typedef struct f1setupRsp
{
  U32               transId;                      /* Uniquely identify transaction */
  char              cuName[CU_DU_NAME_LEN_MAX];   /* CU Name */
  F1CuActCellInfo   actCellInfo;                  /* cells to be activated */
  F1RrcVersion      rrcVersion;                   /* RRC version */
}F1SetupRsp;

typedef struct f1SetupFail
{
   F1FailureIE   setupFail;
}F1SetupFail;

/* At reception of the RESET message the gNB-DU shall release all allocated resources on F1 
 * and radio resources related to the UE association(s) indicated explicitly or implicitly in 
 * the RESET message and remove the indicated UE contexts including F1AP ID. */
typedef struct f1Reset
{
  U32              transId;   /* Uniquely identify transaction */
  F1FailCause      cause;     /* Failure cause */
  F1ResetType      resetType; /* type of reset */
}F1Reset;

/* After the gNB-CU has released all assigned F1 resources and the UE F1AP IDs for all indicated
 * UE associations which can be used for new UE-associated logical F1-connections over the F1 interface,
 *  the gNB-CU shall respond with the RESET ACKNOWLEDGE message. */
typedef struct f1ResetAck
{
  U32                    transId;         /* Uniquely identify transaction */
  F1LogicalConnUeAssoc   ueAssocLogicalConn[MAX_F1_CONNECTIONS]; /* UE associated logical F1-connection list */
  F1CritDiagnostic       critDiagnostic;  /* Critical diagnostics */
}F1ResetAck;

typedef struct f1ErrorInd
{
  U32                transId;         /* Uniquely identify transaction */
  F1Entity           errorOrigin;     /* Specifies if error is originated at DU or CU */
  F1FailCause        cause;           /* Failure cause */
/* If failure is due to Ue related message. */
  U32                gnbCuUeF1apId;   /* gNB-CU UE F1AP Id */
  U32                gnbDuUeF1apId;   /* gNB-DU UE F1AP Id */
  F1CritDiagnostic   critDiagnostic;  /* Critical diagnostics */
}F1ErrorInd;

typedef struct f1GnbDuCfgUpd
{
  U32                 transId;                             /* Uniquely identify transaction */
  F1DuSrvdCellInfo    srvdCellLstAdd[DU_MAX_CELLS];      /* Served cell list to be added */
  F1DuSrvdCellToDel   srvdCellLstMod[DU_MAX_CELLS];      /* Served cell list to be modified */
  NrEcgi              srvdCellLstDel[DU_MAX_CELLS];      /* Served cell list to be deleted */
  F1CellStatus        cellStatus[DU_MAX_CELLS];          /* Cell status */
  F1DedSIDelUE        ueLst[MAX_UEID];                     /* Ue list that requires dedicated SI delivery */
  U32                 gnbDuId;
  F1TnlAssocToRmv     gnbDuTnlAssocRmv[MAX_TNL_ASSOC];  /* TNL Assoc list to remove */ 
}F1GnbDuCfgUpd;

typedef struct f1GnbDuCfgUpdAck
{
   U32                transId;                      /* Uniquely identify transaction */
   F1CuActCellInfo    cellLstAct[DU_MAX_CELLS];    /* List of cells to be activated */   
   F1CritDiagnostic   critDiagnostic;               /* Critical diagnostics */
   NrEcgi             cellLstDeact[DU_MAX_CELLS]; /* List of cells to be deactivated */
}F1GnbDuCfgUpdAck;

typedef struct f1GnbDuCfgUpdFail
{
   F1FailureIE   gnbDuCfgUpdFail;
}F1GnbDuCfgUpdFail;

/* Sent by the gNB-CU to transfer updated information associated to an F1-C interface instance */
typedef struct f1GnbCuCfgUpd
{
    U32                 transId;                            /* Uniquely identifies transaction */
    F1CuActCellInfo     cellLstAct[DU_MAX_CELLS];         /* List of cells to be activated */
    NrEcgi              cellLstDeact[DU_MAX_CELLS];       /* List of cells to be deactivated */
    F1TnlAssoc          assocLstAdd[MAX_TNL_ASSOC];     /* List of TNL assocs to be added */
    F1TnlAssoc          assocLstUpd[MAX_TNL_ASSOC];     /* List of TNL assocs to be updated */
    F1TnlAssocToRmv     assocLstRmv[MAX_TNL_ASSOC];     /* List of TNL assocs to be removed */
    F1CellBarred        cellToBarList[DU_MAX_CELLS];      /* List of Cells to be barred */
    F1ProtectEUTRARsrc  protectEutraRsrcList[MAXCELLINENB]; /* List of Protected EUTRA resources */
}F1GnbCuCfgUpd;

/* Sent by a gNB-DU to a gNB-CU to acknowledge update of information
 * associated to an F1-C interface instance */
typedef struct f1GnbCuCfgUpdAck
{
    U32                   transId;                              /* Uniquely identify transaction */
    F1ActCellFail         actCellFailList[DU_MAX_CELLS];      /* Cells failed to be activated list */
    F1CritDiagnostic      critDiagnostic;                       /* Critical diagnostics */
    F1TnlAssocAddr        assocSetupList[MAX_TNL_ASSOC];     /* TNL Assoc Setup list */
    F1TnlAssocSetupFail   assocSetupFailList[MAX_TNL_ASSOC]; /* TNL Assoc Setup fail list */
    F1DedSIDelUE          dedSiDelUelist[MAXNUMOFUEID];          /* Dedicated SI delivery needed UE list */
}F1GnbCuCfgUpdAck;

typedef struct f1GnbCuCfgUpdFail
{
    F1FailureIE   gnbCuCfgUpdFail;
}F1GnbCuCfgUpdFail;

/* This procedure enables coordination of radio resource allocation between a gNB-CU and
 * a gNB-DU. Coordination request is sent by CU to DU */
typedef struct f1GnbDuRsrcCoordReq
{
   U32           transId;              /* Uniquely identifies transaction */
   F1ReqType     reqType;              /* Request type */
   U8            cellResCoordReqCont;  /* Container for X2AP E-UTRA - NR cell resource coordination request */
   F1IgnoreReq   ignoreReq;            /* Ignore coordination request */
}F1GnbDuRsrcCoordReq;

/* This message is sent by a gNB-DU to a gNB-CU, to express the desired resource allocation
 * for data traffic, as a response to the GNB-DU RESOURCE COORDINATION REQUEST. */
typedef struct f1GnbDuRsrcCoordRsp
{
    U32   transId;               /* Uniquely identifies transaction */
    U8    cellResCoordRspCont;   /* Container for X2AP E-UTRA - NR cell resource coordination response */
}F1GnbDuRsrcCoordRsp;

/* This message is sent by the gNB-DU to indicate to the gNB-CU its status of overload */
typedef struct f1GnbDuStatusInd
{
   U32                  transId;      /* Uniquely identifies transaction */
   F1GnbDuOvrloadInfo   ovrloadInfo;  /* gNB-DU overloaded information */
}F1GnbDuStatusInd;

/* This message is sent by either the gNB-DU or the gNB-CU to intiate the removal
 * of the interface instance and the related resources */
typedef struct f1RmvReq
{
   U32   transId; /* Uniquely identifies transaction */
}F1RmvReq;

/* This message acknowledges the initiation of removal of the interface
 * instance and the related resources */
typedef struct f1RmvRsp
{
   U32                transId;     /* Uniquely identifies transaction */
   F1CritDiagnostic   critDiagnos; /* Crititcality diagnostics */
}F1RmvRsp;

/* This message indicates that removing the interface instance and the related
 * resources cannot be accepted */
typedef struct f1RmvFail
{
   U32                transId;         /* Uniquely identifies transaction */
   F1FailCause        cause;          /* Failure cause */
   F1CritDiagnostic   critDiagnostic; /* Criticality diagnostics */
}F1RmvFail;

/* This message is sent by the gNB-CU to indicate to the gNB-DU a need to reduce
 * the rate at which UEs access the network. */
typedef struct f1NwkAccessRateRed
{
   U32               transId;        /* Uniquely identifies transaction */
   F1UacAssistInfo   uacAssistInfo;  /* UAC Assistance Information */
}F1NwkAccessRateRed;

typedef struct f1Ipaddr
{
 Bool ipV4Pres;
 U32  ipV4Addr; 
}F1IpAddr;

typedef struct sctpParams
{
   F1IpAddr  duIpAddr;
   U16       duPort[MAX_DU_PORT];
   F1IpAddr  cuIpAddr;
   U16       cuPort;
   F1IpAddr  ricIpAddr;
   U16       ricPort;
}SctpParams;

typedef struct f1EgtpParams
{
   F1IpAddr  localIp;      /* Ip address of local node */
   U16       localPort;    /* Sender Port at local node */
   F1IpAddr  destIp;       /* Ip address of peer node */
   U16       destPort;     /* Sender port at peer node */
   U32       minTunnelId;
   U32       maxTunnelId;
}F1EgtpParams;

typedef struct cellCfgParams
{
   NrEcgi      nrEcgi;         /* ECGI */
   U16         nrPci;          /* PCI */
   U16         fiveGsTac;         /* 5gSTac */
   PlmnId      plmn[MAX_PLMN]; /* List of serving PLMN IDs */
   U8          ranac;          /* RAN Area Code */
   U32         maxUe;          /* max UE per slot */
}CellCfgParams;

typedef struct schedulerCfg
{
   U8         numTxAntPorts;    /*!< Number of Tx antenna ports */
   U8         ulSchdType;     /*!< Indicates which UL scheduler to use, range
                               * is 0..(number of schedulers - 1) */
   U8         dlSchdType;     /*!< Indicates which DL scheduler to use, range
                               * is 0..(number of schedulers - 1) */
   U8         numCells;       /*!< Max number of cells */
   U8         maxUlUePerTti;  /*!< Max number of UE in UL per TTI */
   U8         maxDlUePerTti;  /*!< Max number of UE in DL per TTI */
}SchedulerCfg;

typedef struct mibParams
{
	uint8_t sysFrmNum;
	long    subCarrierSpacingCommon;
	long    ssb_SubcarrierOffset;
	long    dmrs_TypeA_Position;
	long    controlResourceSetZero;
	long    searchSpaceZero;
	long    cellBarred;
	long    intraFreqReselection;
}MibParams;

typedef struct sib1Params
{
	PlmnId    plmn;
	uint8_t   tac;
	long      ranac;
	U8        cellIdentity;
	long      cellResvdForOpUse;
}Sib1Params;

typedef struct duCfgParams
{
   SctpParams         sctpParams;                    /* SCTP Params */
   F1EgtpParams       egtpParams;                    /* EGTP Params */
   U32                maxUe;
   U32                duId;
   U8                 duName[CU_DU_NAME_LEN_MAX];
   SchedulerCfg       schedCfg;
   CellCfgParams      cellCfg[DU_MAX_CELLS];  /* Cell info config*/
   F1DuSrvdCellInfo   srvdCellLst[DU_MAX_CELLS];   /* Serving cell list *///TODO: this must be removed eventually
   F1RrcVersion       rrcVersion;                    /* RRC version */

   MacCellCfg	      macCellCfg;	/* MAC cell configuration */
   MibParams          mibParams;                     /* MIB Params */
	Sib1Params         sib1Params;                    /* SIB1 Params */
}DuCfgParams;

typedef struct duCellCb
{
   U32            cellId;      /* Internal cell Id */
   CellCfgParams  cellInfo;    /* Cell info */
   CellStatus     cellStatus;  /*Cell status */
}DuCellCb;

/*function declarations */
void FillSlotConfig();
S16 readClCfg();
S16 readCfg();
S16 duReadCfg(); 
S16 bitStringToInt(BIT_STRING_t *bitString, U16 *val);

#endif /* __DU_CONFIG_H__ */

/**********************************************************************
         End of file
**********************************************************************/
