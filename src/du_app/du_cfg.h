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

/* MACROS */
#define DU_INST 0
#define DU_ID 1

#ifndef O1_ENABLE

#define DU_IP_V4_ADDR "192.168.130.81"
#define CU_IP_V4_ADDR "192.168.130.82"
#define RIC_IP_V4_ADDR "192.168.130.80"
#define DU_PORT 38472
#define CU_PORT 38472
#define RIC_PORT 36421
#endif

#define DU_EGTP_PORT  39001
#define CU_EGTP_PORT  39002
#define NR_PCI 1
#define NR_CELL_ID 1

#define DU_NAME "ORAN_OAM_DU"
#define CELL_TYPE SMALL

//TODO: while testing for TDD, Mu1 and 100 MHz, this flag must be enabled
#ifdef NR_TDD
#define DUPLEX_MODE DUP_MODE_TDD
#define NR_NUMEROLOGY 1
#define NR_DL_ARFCN 623400
#define NR_UL_ARFCN 623400
#define NR_FREQ_BAND 78
#else
#define DUPLEX_MODE DUP_MODE_FDD
#define NR_NUMEROLOGY 0
#define NR_DL_ARFCN 428000
#define NR_UL_ARFCN 390000
#define NR_FREQ_BAND 1
#endif

#define DU_TAC 1
#define PLMN_MCC0 3
#define PLMN_MCC1 1
#define PLMN_MCC2 1
#define PLMN_MNC0 4
#define PLMN_MNC1 8
#define PLMN_MNC2 0
#define PLMN_SIZE 3

/* Spec 38.104 Table 5.4.2.3-1:Applicable NR-ARFCN per operating band in FR1 */
#define SUL_ARFCN 100
#define SUL_BAND 2



#define TIME_CFG 0
#define CARRIER_IDX 1
#define NUM_TX_ANT 2
#define NUM_RX_ANT 2
#define FREQ_SHIFT_7P5KHZ FALSE
#define SSB_PBCH_PWR 0
#define BCH_PAYLOAD PHY_GEN_TIMING_PBCH_BIT
#define NORMAL_CYCLIC_PREFIX 0
#define OFFSET_TO_POINT_A 24                     /* PRB Offset to Point A */
#define BETA_PSS BETA_PSS_0DB  
#define SSB_PERIODICITY_5MS   5
#define SSB_PERIODICITY_10MS  10
#define SSB_PERIODICITY_20MS  20
#define SSB_PERIODICITY_40MS  40
#define SSB_PERIODICITY_80MS  80
#define SSB_PERIODICITY_160MS 160
#define SSB_SUBCARRIER_OFFSET 0               
#define SSB_MULT_CARRIER_BAND FALSE
#define MULT_CELL_CARRIER FALSE
#define FREQ_LOC_BW  28875             /* DL frequency location and bandwidth. Spec 38.508 Table 4.3.1.0B-1*/
#define UL_P_MAX  23
#define DMRS_TYPE_A_POS 2
#define NUM_SYMBOLS_PER_SLOT 14       /* Number of symbols within a slot */
#define CORESET0_END_PRB   48
#define CORESET1_NUM_PRB   24

/* MACRO defines for PRACH Configuration */
#define PRACH_CONFIG_IDX   88
#define PRACH_FREQ_START   0
#define PRACH_SEQ_LEN SHORT_SEQUENCE
#define PRACH_SUBCARRIER_SPACING 0
#define PRACH_RESTRICTED_SET_CFG 0
#define NUM_PRACH_FDM 1
#define ROOT_SEQ_IDX 0
#define NUM_ROOT_SEQ 1
#define ZERO_CORRELATION_ZONE_CFG 4
#define NUM_UNUSED_ROOT_SEQ 0
#define UNUSED_ROOT_SEQ 1
#define SSB_PER_RACH 1
#define PRACH_MULT_CARRIER_BAND FALSE
#define PRACH_PREAMBLE_RCVD_TGT_PWR  -74   
#define NUM_RA_PREAMBLE  63
#define RSRP_THRESHOLD_SSB   31

#ifdef NR_TDD
#define TDD_PERIODICITY TX_PRDCTY_MS_5 
#endif

#define RSS_MEASUREMENT_UNIT DONT_REPORT_RSSI
#define RA_CONT_RES_TIMER 64
#define RA_RSP_WINDOW 180
#define PRACH_RESTRICTED_SET 0 /* Unrestricted */
#define ROOT_SEQ_LEN 139

/* MACRCO Ddefine for PDCCH Configuration */
#define PDCCH_SEARCH_SPACE_ID      1    /* Common search space id */
#define PDCCH_CTRL_RSRC_SET_ID     0    /* Control resource set id */
#define PDCCH_SEARCH_SPACE_ID_SIB1 1    /* Search space id for sib1 */
#define PDCCH_SEARCH_SPACE_ID_PAGING 1  /* Search space id for paging */
#define PDCCH_SEARCH_SPACE_ID_RA   1    /* Search spaced id for random access */
#define PDCCH_SERACH_SPACE_DCI_FORMAT 0
#define PDCCH_CTRL_RSRC_SET_ONE_ID  1
#define PDCCH_CTRL_RSRC_SET_ONE_DURATION 2  /* Duration for control resource set id i */
#define PDCCH_CTRL_RSRC_SET_ONE_PRECOD_GRANULARITY 1   /* Precoded granularity */
#define PDCCH_SRCH_SPC_TWO_ID 2
#define PDCCH_SRCH_SPC_TWO_AGG_LVL1_CANDIDATE 7   /* Num of candidate at aggregation level 1 */
#define PDCCH_SRCH_SPC_TWO_AGG_LVL2_CANDIDATE 7   /* Num of candidate at aggregation level 2 */
#define PDCCH_SRCH_SPC_TWO_AGG_LVL4_CANDIDATE 4   /* Num of candidate at aggregation level 4 */
#define PDCCH_SRCH_SPC_TWO_AGG_LVL8_CANDIDATE 2   /* Num of candidate at aggregation level 8 */
#define PDCCH_SRCH_SPC_TWO_AGG_LVL16_CANDIDATE 1  /* Num of candidate at aggregation level 16 */
#define PDCCH_SRCH_SPC_TWO_UE_SPEC_DCI_FORMAT  0  /* format 0-0 and 1-0 */
#define PDCCH_SYMBOL_WITHIN_SLOT 128     /* Symbol within Slot Value */


#define SIB1_VALUE_TAG 10

/* MACRO Ddefine for PDSCH Configuration */
#define PDSCH_K0_CFG1  0
#define PDSCH_K0_CFG2  1
#define PDSCH_START_SYMBOL  3
#define PDSCH_LENGTH_SYMBOL 11
#define PDSCH_RES_ALLOC_TYPE       1          /* Resource allocation type */
#define PDSCH_MAX_CODEWORD_SCH_BY_DCI 0       /* Max num of codewords scheduled by DCI */
#define PDSCH_RBG_SIZE   0                    /* 0: config1 */
#define PDSCH_NUM_HARQ_PROC 5

/* MACRO Define for PUSCH Configuration */
#define MAX_UL_ALLOC 16
#define PUSCH_K2_CFG1  3
#define PUSCH_K2_CFG2  4
#define PUSCH_START_SYMBOL  3
#define PUSCH_LENGTH_SYMBOL 11

#define PUSCH_MSG3_DELTA_PREAMBLE 0
#define PUSCH_P0_NOMINAL_WITH_GRANT -70
#define PUSCH_TRANSFORM_PRECODER    1      /* 1: Disabled */
#define PUSCH_MAX_MIMO_LAYERS       1
#define PUSCH_PROCESS_TYPE2_ENABLED false

/* Macro define for PUCCH Configuration */
#define PUCCH_RSRC_COMMON  0
#define PUCCH_GROUP_HOPPING 2 /* disable */
#define PUCCH_P0_NOMINAL   -74

/* MACRO defines for TDD DL-UL Configuration */
#define NUM_DL_SLOTS 7
#define NUM_DL_SYMBOLS 12 
#define NUM_UL_SLOTS 2
#define NUM_UL_SYMBOLS 1
#define GUARD_SLOT_IDX 7

/* MACRO defines for SRC config */
#define SRS_RSRC_ID  1
#define SRS_RSET_ID  1
#define SRS_COMB_OFFSET_N2   0
#define SRS_CYCLIC_SHIFT_N2  0
#define SRS_FREQ_DOM_POS     0
#define SRS_FREQ_DOM_SHIFT   0
#define C_SRS 0
#define B_SRS 0
#define B_HOP 0
#define SRS_SEQ_ID 0
#define APERIODIC_SRS_RESRC_TRIGGER 1 

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

#ifdef EGTP_TEST
/* Macro definitions for EGTP procedures */
#define EGTP_LCL_TEID 1     /* EGTP local tunnel id */
#define EGTP_REM_TEID 10    /* EGTP remote tinnel id */
#endif


/* Macro definitions for F1 procedures */
#define CU_DU_NAME_LEN_MAX 30      /* Max length of CU/DU name string */
#define MAX_F1_CONNECTIONS 65536    /* Max num of F1 connections */
#define MAX_PLMN           1        /* Max num of broadcast PLMN ids */
#define MAXNRARFCN         3279165  /* Maximum values of NRAFCN */
#define MAXNRCELLBANDS     2       /* Maximum number of frequency bands */
#define MAX_NUM_OF_SLICE_ITEMS 1024     /* Maximum number of signalled slice support items */
#define MAXBPLMNNRMINUS1   1       /* Maximum number of PLMN Ids broadcast in an NR cell minus 1 */
#define MAXNUMOFSIBTYPES   32       /* Maximum number of SIB types */
#define MAX_TNL_ASSOC      32       /* Max num of TNL Assoc between CU and DU */
#define MAXCELLINENB       256      /* Max num of cells served by eNB */
#define MAX_NUM_OF_UE_ID       65536    /* Max num of UEs served by DU */
#define MAXNUMOFUACPLMN    12       /* Maximum number of PLMN ids*/
#define MAXNUMOFUACPERPLMN 64       /* Maximum number of signalled categories per PLMN */
#define NR_RANAC           150      /* RANAC */
#define DEFAULT_CELLS      1        /* Max num of broadcast PLMN ids */


/* Macro definitions for MIB/SIB1 */
#define SYS_FRAME_NUM 0
#define SPARE 0
#define SSB_SC_OFFSET 0
#define DU_RANAC 1
#define CELL_IDENTITY 32

/* Macro definitions for DUtoCuRrcContainer */
#define CELL_GRP_ID 1
#define SCH_REQ_ID  0
#define SR_PROHIBIT_TMR 5
#define SR_TRANS_MAX 2
#define PERIODIC_BSR_TMR 2
#define RETX_BSR_TMR     5
#define SR_DELAY_TMR     6
#define TAG_ID 0
#define TIME_ALIGNMENT_TMR 7
#define PHR_PERIODIC_TMR 7
#define PHR_PROHIBHIT_TMR 0
#define PHR_PWR_FACTOR_CHANGE 3
#define PHR_MODE_OTHER_CG 0
#define SN_FIELD_LEN 0
#define T_POLL_RETRANSMIT 8       /* Enum for 45ms */ 
#define T_POLL_RETRANSMIT_VAL 45  /* Value in ms */
#define POLL_PDU 0                /* Enum for 4 pdus */
#define POLL_PDU_VAL 4            /* Value of poll pdu */
#define POLL_BYTE 43              /* Enum for infinite poll bytes */
#define POLL_BYTE_VAL -1          /* Value for infinite poll byte */
#define MAX_RETX_THRESHOLD 5      /* Enum for 8 retransmissions */
#define MAX_RETX_THRESHOLD_VAL 8  /* Value for 8 retransmissions */
#define T_REASSEMBLY 8 /* enum for RE_ASM_40MS  */
#define T_REASSEMBLY_VAL 40 /* default re assembly timer */
#define T_STATUS_PROHIBHIT 7 /* enum for PROH_35MS */
#define T_STATUS_PROHIBHIT_VAL 35 /* default status prohibit timer */
#define MAC_LC_PRIORITY 1
#define PRIORTISIED_BIT_RATE 15
#define BUCKET_SIZE_DURATION 5
#define LC_GRP 0
#define P_NR_FR1 0
#define PDSCH_HARQ_ACK_CODEBOOK 1
#define SERV_CELL_IDX 0
#define RLM_SYNC_OUT_SYNC_THRESHOLD 0
#define ACTIVE_DL_BWP_ID 0
#define ACTIVE_UL_BWP_ID 0
#define SCRAMBLING_ID  NR_PCI
#define DMRS_ADDITIONAL_POS  0          /* DMRS Additional poistion */
#define RES_ALLOC_TYPE       1          /* Resource allocation type */

#ifdef EGTP_TEST
#define UE_ID 1
#define RB_ID 1
#define LC_ID 1
#endif

/* MACRO definitions for modulcation order */
#define MOD_ORDER_QPSK  2
#define MOD_ORDER_QAM16 4
#define MOD_ORDER_QAM64 6
#define MOD_ORDER_QAM256 8
#define PDSCH_MCS_INDEX 20  /* For 64QAM, valid mcs index: 17-28 in 38.214  - Table 5.1.3.1-1*/
#define PUSCH_MCS_INDEX 10  /* For 16QAM, valid mcs index: 10-16 in 38.214  - Table 5.1.3.1-1*/

typedef enum
{
   GNBDU,
   GNBCU 
}F1Entity;

typedef enum
{
   SERV_CELL_TO_MODIFY,
   SERV_CELL_TO_DELETE
}ServCellAction;

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

typedef enum
{
   PDSCH_MAPPING_TYPE_A,
   PDSCH_MAPPING_TYPE_B,
}pdschMappingType;

typedef enum
{
   PUSCH_MAPPING_TYPE_A,
   PUSCH_MAPPING_TYPE_B,
}puschMappingType;


typedef struct f1RrcVersion
{
   char    rrcVer[30];     /* Latest RRC Version */
   uint32_t   extRrcVer;  /* Latest RRC version extended */
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
   bool   pres;
   uint8_t     value;
}F1ProcCode;

typedef struct f1TriggMsg
{
   bool   pres;
   F1TriggMsgVal   value;
}F1TriggMsg;

typedef struct f1ProcCrit
{
   bool   pres;
   F1ProcCritVal   value;
}F1ProcCrit;

typedef struct f1CritDignosIE
{
   F1ProcCritVal   ieCritVal;
   uint16_t             ieId;
   F1ErrorType       type;
}F1CritDignosIE;

typedef struct f1CritDiagnostic
{
   bool         pres;
   F1ProcCode   procCode;      /* Procedure Code */
   F1TriggMsg   triggeringMsg; /* Triggering Message */
   F1ProcCrit   procCrit;      /* Criticality of Triggering msg/proc */
   uint32_t          transId;
   F1CritDignosIE diagnosIE;
}F1CritDiagnostic;

typedef struct f1FailureIE
{
   uint32_t                transId;        /* Uniquely identify transaction */
   F1FailCause        cause;          /* Failure cause */
   uint8_t                 timeToWait;     /* Seconds to be waited before retry */
   F1CritDiagnostic   critDiagnostic; /* Critical diagnostics */
}F1FailureIE;

typedef struct f1LogicalConnUeAssoc
{
   uint32_t   gnbCuUeF1apId;
   uint32_t   gnbDuUeF1apId;
}F1LogicalConnUeAssoc;

typedef struct f1ResetType
{
   union
   {
      F1ResetAll             resetAll; /* Reset All */
      F1LogicalConnUeAssoc   ueAssocLogicalConn[MAX_F1_CONNECTIONS]; /* In case of Partial Reset : UE associated logical F1 conn list*/
   }type;
}F1ResetType;

typedef struct nrEcgi
{
   Plmn  plmn;
   uint16_t     cellId;
}NrEcgi;

typedef struct f1SibType
{
   uint8_t   sibType;
   uint8_t   sibMsg;
   uint8_t   value;
}F1SibType;

typedef struct f1TxBw
{
   F1NrScs   nrScs; /* NR subcarrier spacing */
   F1Nrb     nrb;   /* Transmission bandwidth expressed in units of RBs */
}F1TxBw;

typedef struct f1SulInfo
{
   uint32_t      sulArfcn; /* SUL ARFCN */
   F1TxBw   sulTxBw;  /* SUL transmission bandwidth */ 
}F1SulInfo;

typedef struct f1FreqBand
{
   uint16_t   nrFreqBand;
   uint16_t   sulBand[MAXNRCELLBANDS];
}F1FreqBand;

typedef struct f1NrFreqInfo
{
   uint32_t        nrArfcn;
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
   uint16_t   ulOffset;   /* UL offset to Point A */
   uint16_t   dlOffset;   /* DL offset to Point A */
}F1EutraFDDInfo;

typedef struct f1EutraTddInfo
{
   uint16_t   tddOffset;  /* Offset to Point A */
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
   uint8_t   sst;
   uint32_t  sd;
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
   bool       pres;
   F1Snsaai   snssai[MAX_NUM_OF_SLICE_ITEMS];   
}F1TaiSliceSuppLst;

typedef struct f1SrvdPlmn
{
   Plmn              plmn;
   F1TaiSliceSuppLst   taiSliceSuppLst;
}F1SrvdPlmn;

typedef struct f1BrdcstPlmnInfo
{
   Plmn   plmn[MAX_PLMN];     /* PLMN id list */
   Plmn   extPlmn[MAX_PLMN];   /* Extended PLMN id list */
   uint16_t      tac;                     /* 5GS-TAC */
   uint32_t      nrCellId;                /* NR Cell id */
   uint8_t       ranac;                   /* RAN Area code */
}F1BrdcstPlmnInfo;

typedef struct f1CellInfo
{
   NrEcgi   nrCgi;                   /* Cell global Identity */
   uint32_t nrPci;                   /* Physical Cell Identity */
   Plmn   plmn[MAX_PLMN];     /* Available PLMN list */
   Plmn   extPlmn[MAX_PLMN];  /* Extended available PLMN list */
}F1CellInfo;

typedef struct f1DuCellInfo
{
   F1CellInfo         cellInfo;     /* cell info */
   uint16_t           tac;          /* tracking area code */
   uint16_t           epsTac;       /* Configured EPS TAC */
   NrModeInfo         f1Mode;       /* NR mode info : FDD/TDD */
   uint8_t            measTimeCfg;  /* Measurement timing configuration */
   F1CellDir          cellDir;      /* Cell Direction */
   F1CellType         cellType;     /* Cell Type */
   F1BrdcstPlmnInfo   brdcstPlmnInfo[MAXBPLMNNRMINUS1]; /* Broadcast PLMN Identity Info List */
}F1DuCellInfo;

typedef struct f1DuSysInfo
{
   uint8_t   *mibMsg;   /* MIB message */
   uint16_t  mibLen;
   uint8_t   *sib1Msg;  /* SIB1 message */
   uint16_t  sib1Len;
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
   bool        switchOffOngoing;
}SrvStatus;

typedef struct f1CellStatus
{
   NrEcgi      nrEcgi;
   SrvStatus   status;   /* Service status */
}F1CellStatus;

typedef struct f1DedSIDelUE
{
   uint32_t      gnbCuUeF1apId;
   NrEcgi   nrEcgi;
}F1DedSIDelUE;

typedef struct tnlAssocInfo
{
   bool           pres;
   EpIpAddr       epIpAddr;      /* Endpoint-IP Address */
   EpIpAddrPort   epIpAddrport;  /* Endpoint-IP Address and Port */
}F1TnlAssocAddr;

typedef struct f1TnlAssocUsage
{
   bool           pres;
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
   uint32_t                eutraCellId; /* EUTRA cell id */
   F1EutraModeInfo    mode;        /* EUTRA mode info : FDD/TDD */
   uint8_t                 rsrcInd;     /* Protected EUTRA resource Indication */
}F1EutraCell;

typedef struct f1ProtectEUTRARsrc
{
   uint32_t           specShareGrpId;               /* Spectrum sharing group id */
   F1EutraCell   eutraCellList[MAXCELLINENB];  /* EUTRA cells list */
}F1ProtectEUTRARsrc;

typedef struct f1UacOperatorDefined
{
   uint8_t   accessCategory;
   uint8_t   accessId; 
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
   uint8_t                  uacReducInd;     /* Value 0 means no access rate reduction. Value 100 means full access rate reduction */
   F1UacCategoryType   uacCategoryType; /* UAC Category type */
}F1UacType;

/* Unified Access Class Assistance Information */
typedef struct f1UacAssistInfo
{
   Plmn      plmn[MAXNUMOFUACPLMN];        /* UAC PLMN list */
   F1UacType   uacType[MAXNUMOFUACPERPLMN];  /* UAC Type list */
}F1UacAssistInfo;

/* F1 setup related structures */
#if 0
typedef struct f1SetupReq
{
   uint32_t                transId;                       /* Uniquely identify transaction */
   uint32_t                duId;                          /* DU ID */ 
   char               duName[CU_DU_NAME_LEN_MAX];    /* DU name */
   F1DuSrvdCellInfo   srvdCellLst[MAX_NUM_CELL];   /* Serving cell list */
   F1RrcVersion       rrcVersion;                    /* RRC version */
}F1SetupReq;

typedef struct f1setupRsp
{
   uint32_t               transId;                      /* Uniquely identify transaction */
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
   uint32_t              transId;   /* Uniquely identify transaction */
   F1FailCause      cause;     /* Failure cause */
   F1ResetType      resetType; /* type of reset */
}F1Reset;

/* After the gNB-CU has released all assigned F1 resources and the UE F1AP IDs for all indicated
 * UE associations which can be used for new UE-associated logical F1-connections over the F1 interface,
 *  the gNB-CU shall respond with the RESET ACKNOWLEDGE message. */
typedef struct f1ResetAck
{
   uint32_t                    transId;         /* Uniquely identify transaction */
   F1LogicalConnUeAssoc   ueAssocLogicalConn[MAX_F1_CONNECTIONS]; /* UE associated logical F1-connection list */
   F1CritDiagnostic       critDiagnostic;  /* Critical diagnostics */
}F1ResetAck;

typedef struct f1ErrorInd
{
   uint32_t                transId;         /* Uniquely identify transaction */
   F1Entity           errorOrigin;     /* Specifies if error is originated at DU or CU */
   F1FailCause        cause;           /* Failure cause */
   /* If failure is due to Ue related message. */
   uint32_t                gnbCuUeF1apId;   /* gNB-CU UE F1AP Id */
   uint32_t                gnbDuUeF1apId;   /* gNB-DU UE F1AP Id */
   F1CritDiagnostic   critDiagnostic;  /* Critical diagnostics */
}F1ErrorInd;

typedef struct f1GnbDuCfgUpd
{
   uint32_t                 transId;                             /* Uniquely identify transaction */
   F1DuSrvdCellInfo    srvdCellLstAdd[MAX_NUM_CELL];      /* Served cell list to be added */
   F1DuSrvdCellToDel   srvdCellLstMod[MAX_NUM_CELL];      /* Served cell list to be modified */
   NrEcgi              srvdCellLstDel[MAX_NUM_CELL];      /* Served cell list to be deleted */
   F1CellStatus        cellStatus[MAX_NUM_CELL];          /* Cell status */
   F1DedSIDelUE        ueLst[MAX_NUM_UE];                     /* Ue list that requires dedicated SI delivery */
   uint32_t            gnbDuId;
   F1TnlAssocToRmv     gnbDuTnlAssocRmv[MAX_TNL_ASSOC];  /* TNL Assoc list to remove */ 
}F1GnbDuCfgUpd;

typedef struct f1GnbDuCfgUpdAck
{
   uint32_t           transId;                      /* Uniquely identify transaction */
   F1CuActCellInfo    cellLstAct[MAX_NUM_CELL];    /* List of cells to be activated */   
   F1CritDiagnostic   critDiagnostic;               /* Critical diagnostics */
   NrEcgi             cellLstDeact[MAX_NUM_CELL]; /* List of cells to be deactivated */
}F1GnbDuCfgUpdAck;

typedef struct f1GnbDuCfgUpdFail
{
   F1FailureIE   gnbDuCfgUpdFail;
}F1GnbDuCfgUpdFail;

/* Sent by the gNB-CU to transfer updated information associated to an F1-C interface instance */
typedef struct f1GnbCuCfgUpd
{
   uint32_t            transId;                            /* Uniquely identifies transaction */
   F1CuActCellInfo     cellLstAct[MAX_NUM_CELL];         /* List of cells to be activated */
   NrEcgi              cellLstDeact[MAX_NUM_CELL];       /* List of cells to be deactivated */
   F1TnlAssoc          assocLstAdd[MAX_TNL_ASSOC];     /* List of TNL assocs to be added */
   F1TnlAssoc          assocLstUpd[MAX_TNL_ASSOC];     /* List of TNL assocs to be updated */
   F1TnlAssocToRmv     assocLstRmv[MAX_TNL_ASSOC];     /* List of TNL assocs to be removed */
   F1CellBarred        cellToBarList[MAX_NUM_CELL];      /* List of Cells to be barred */
   F1ProtectEUTRARsrc  protectEutraRsrcList[MAXCELLINENB]; /* List of Protected EUTRA resources */
}F1GnbCuCfgUpd;

/* Sent by a gNB-DU to a gNB-CU to acknowledge update of information
 * associated to an F1-C interface instance */
typedef struct f1GnbCuCfgUpdAck
{
   uint32_t              transId;                              /* Uniquely identify transaction */
   F1ActCellFail         actCellFailList[MAX_NUM_CELL];      /* Cells failed to be activated list */
   F1CritDiagnostic      critDiagnostic;                       /* Critical diagnostics */
   F1TnlAssocAddr        assocSetupList[MAX_TNL_ASSOC];     /* TNL Assoc Setup list */
   F1TnlAssocSetupFail   assocSetupFailList[MAX_TNL_ASSOC]; /* TNL Assoc Setup fail list */
   F1DedSIDelUE          dedSiDelUelist[MAX_NUM_OF_UE_ID];          /* Dedicated SI delivery needed UE list */
}F1GnbCuCfgUpdAck;

typedef struct f1GnbCuCfgUpdFail
{
   F1FailureIE   gnbCuCfgUpdFail;
}F1GnbCuCfgUpdFail;

/* This procedure enables coordination of radio resource allocation between a gNB-CU and
 * a gNB-DU. Coordination request is sent by CU to DU */
typedef struct f1GnbDuRsrcCoordReq
{
   uint32_t      transId;              /* Uniquely identifies transaction */
   F1ReqType     reqType;              /* Request type */
   uint8_t       cellResCoordReqCont;  /* Container for X2AP E-UTRA - NR cell resource coordination request */
   F1IgnoreReq   ignoreReq;            /* Ignore coordination request */
}F1GnbDuRsrcCoordReq;

/* This message is sent by a gNB-DU to a gNB-CU, to express the desired resource allocation
 * for data traffic, as a response to the GNB-DU RESOURCE COORDINATION REQUEST. */
typedef struct f1GnbDuRsrcCoordRsp
{
   uint32_t   transId;               /* Uniquely identifies transaction */
   uint8_t    cellResCoordRspCont;   /* Container for X2AP E-UTRA - NR cell resource coordination response */
}F1GnbDuRsrcCoordRsp;

/* This message is sent by the gNB-DU to indicate to the gNB-CU its status of overload */
typedef struct f1GnbDuStatusInd
{
   uint32_t                  transId;      /* Uniquely identifies transaction */
   F1GnbDuOvrloadInfo   ovrloadInfo;  /* gNB-DU overloaded information */
}F1GnbDuStatusInd;

/* This message is sent by either the gNB-DU or the gNB-CU to intiate the removal
 * of the interface instance and the related resources */
typedef struct f1RmvReq
{
   uint32_t   transId; /* Uniquely identifies transaction */
}F1RmvReq;

/* This message acknowledges the initiation of removal of the interface
 * instance and the related resources */
typedef struct f1RmvRsp
{
   uint32_t           transId;     /* Uniquely identifies transaction */
   F1CritDiagnostic   critDiagnos; /* Crititcality diagnostics */
}F1RmvRsp;

/* This message indicates that removing the interface instance and the related
 * resources cannot be accepted */
typedef struct f1RmvFail
{
   uint32_t           transId;         /* Uniquely identifies transaction */
   F1FailCause        cause;          /* Failure cause */
   F1CritDiagnostic   critDiagnostic; /* Criticality diagnostics */
}F1RmvFail;

/* This message is sent by the gNB-CU to indicate to the gNB-DU a need to reduce
 * the rate at which UEs access the network. */
typedef struct f1NwkAccessRateRed
{
   uint32_t          transId;        /* Uniquely identifies transaction */
   F1UacAssistInfo   uacAssistInfo;  /* UAC Assistance Information */
}F1NwkAccessRateRed;
#endif

typedef struct f1Ipaddr
{
   bool ipV4Pres;
   uint32_t  ipV4Addr; 
}F1IpAddr;

typedef struct sctpParams
{
   F1IpAddr  duIpAddr;
   uint16_t       duPort[MAX_DU_PORT];
   F1IpAddr  cuIpAddr;
   uint16_t       cuPort;
   F1IpAddr  ricIpAddr;
   uint16_t       ricPort;
}SctpParams;

typedef struct f1EgtpParams
{
   F1IpAddr  localIp;      /* Ip address of local node */
   uint16_t  localPort;    /* Sender Port at local node */
   F1IpAddr  destIp;       /* Ip address of peer node */
   uint16_t  destPort;     /* Sender port at peer node */
   uint32_t  minTunnelId;
   uint32_t  maxTunnelId;
}F1EgtpParams;

typedef struct schedulerCfg
{
   uint8_t   numTxAntPorts;    /*!< Number of Tx antenna ports */
   uint8_t   ulSchdType;     /*!< Indicates which UL scheduler to use, range
			      * is 0..(number of schedulers - 1) */
   uint8_t   dlSchdType;     /*!< Indicates which DL scheduler to use, range
			      * is 0..(number of schedulers - 1) */
   uint8_t   numCells;       /*!< Max number of cells */
   uint8_t   maxUlUePerTti;  /*!< Max number of UE in UL per TTI */
   uint8_t   maxDlUePerTti;  /*!< Max number of UE in DL per TTI */
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

typedef struct siSchedInfo
{
   long     winLen;         /* SI Window Length */
   long     broadcastSta;   /* Broadcast status */
   long     preiodicity;    /* si periodicity */
   long     sibType;
   long     sibValTag;      /* Value tag */
}SiSchedInfo;

typedef struct pdcchCfgCommon
{
   uint8_t  present;
   long     ctrlRsrcSetZero;  /* Control resource set zero */
   long     searchSpcZero;    /* Search space zero */
   long     searchSpcId;      /* Search space id */
   long     ctrlRsrcSetId;    /* Control resource set id */
   uint8_t  monitorSlotPrdAndOffPresent;
   long     monitorSlotPrdAndOff;     /* Monitoring slot periodicity and offset */
   uint8_t  monitorSymbolsInSlot[2];  /* Monitoring symbols within slot */
   long     numCandAggLvl1;        /* Number of candiates at aggregation level 1 */
   long     numCandAggLvl2;        /* Number of candiates at aggregation level 2 */
   long     numCandAggLvl4;        /* Number of candiates at aggregation level 4 */
   long     numCandAggLvl8;        /* Number of candiates at aggregation level 8 */
   long     numCandAggLvl16;       /* Number of candiates at aggregation level 16 */
   uint8_t  searchSpcType;        /*  Search Space type */
   uint8_t  commSrchSpcDciFrmt;    /* DCI format of common search space type */
   long     searchSpcSib1;         /* Id of serch space for SIB1 */
   long     pagingSearchSpc;       /* Id of search space for paging */
   long     raSearchSpc;           /* Id of search space for Random Access proc */
}PdcchCfgCommon;

typedef struct pdschTimeDomainRsrcAlloc
{
   long     k0;
   long     mapType;              /* Mapping Type */
   uint16_t sliv;  
}PdschTimeDomainRsrcAlloc;

typedef struct pdschCfgCommon
{
   uint8_t  present;
   uint8_t  numTimeDomRsrcAlloc;
   PdschTimeDomainRsrcAlloc timeDomAlloc[MAX_NUM_DL_ALLOC];
}PdschCfgCommon;

typedef struct bcchCfg
{
   long   modPrdCoeff;          /* Modification period co-efficient */
}BcchCfg;

typedef struct pcchCfg
{
   long   dfltPagingCycle;      /* Default paging cycle */
   long   nAndPagingFrmOffPresent;
   long   nAndPagingFrmOff;     /* n and Paging Frame offset */
   long   numPagingOcc;         /* Number of paging occassions in paging frame */
}PcchCfg;

typedef struct scsSpecCarrier
{
   long   scsOffset;    /* Offset to point A considering subcarrier spacing */
   long   scs;          /* subcarrier spacing */
   long   scsBw;        /* Carrier bandwidth considering subcarrier spacing */

}ScsSpecCarrier;

typedef struct dlCfgCommon
{
   long   freqBandInd;  /* Downlink frequency band indicator */
   long   offsetToPointA;       /* DL ofset to point A */
   long   locAndBw;     /* Frequency location and bandwidth */
   ScsSpecCarrier   dlScsCarrier;  /* SCS Specific carrier */
   PdcchCfgCommon   pdcchCfg;
   PdschCfgCommon   pdschCfg;
   BcchCfg          bcchCfg;
   PcchCfg          pcchCfg;
}DlCfgCommon;

typedef struct rachCfgCommon
{
   uint8_t present;
   long   prachCfgIdx;       /* PRACH configuration index */
   long   msg1Fdm;           /* Num of PRACH transmission occassions */
   long   msg1FreqStart;     /* Offset of lowest msg1 transmission occassion */
   long   zeroCorrZoneCfg;   /* Zero correlation zone configuration */
   long   preambleRcvdTgtPwr; /* Traget power level */
   long   preambleTransMax;  /* Max num of preamble transmission */
   long   pwrRampingStep;    /* Power ramping steps for PRACH */
   long   raRspWindow;       /* RA response window */
   long   numRaPreamble;     /* Total num of preamble used in random access */
   uint8_t ssbPerRachOccPresent;
   long   numSsbPerRachOcc;  /* Numer of SSBs per RACH Occassion */
   long   contResTimer;      /* Contention resolution timer */
   long   rsrpThreshSsb;
   uint8_t rootSeqIdxPresent;
   long   rootSeqIdx;        /* PRACH root sequence index */
   long   msg1Scs;           /* Msg1 subcarrier spacing */
   long   restrictedSetCfg;  /* Restricted set configuration */
}RachCfgCommon;

typedef struct puschCfgCmnTimeDomAlloc
{
   long     k2;
   long     mapType;
   uint16_t sliv;
}PuschCfgCmnTimeDomAlloc;

typedef struct  puschCfgCommon
{
   uint8_t  puschCfgPresent;
   uint8_t  numTimeDomRsrcAlloc;
   PuschCfgCmnTimeDomAlloc timeDomAllocList[MAX_UL_ALLOC];
   long     msg3DeltaPreamble;
   long     p0NominalWithGrant;
}PuschCfgCommon;

typedef struct pucchCfgCommon
{
   uint8_t present;
   long   rsrcComm;
   long   grpHop;
   long   p0Nominal;
}PucchCfgCommon;

typedef struct ulCfgCommon
{
   long   pMax;         /* Max UL transmission power that UE applies */
   long   locAndBw;     /* Frequency location and bandwidth */
   ScsSpecCarrier   ulScsCarrier; /* SCS Specific carrier */
   RachCfgCommon    rachCfg;
   PuschCfgCommon   puschCfg;
   PucchCfgCommon   pucchCfg;
   long   timeAlignTimerComm;
}UlCfgCommon;

typedef struct tddUlDlCfgCommon
{
   long   refScs;     /* Reference subcarrier spacing */
   long   txPrd;      /* Transmission periodicity */
   long   numDlSlots;
   long   numDlSymbols;
   long   numUlSlots;
   long   numUlSymbols;
}TddUlDlCfgCommon;

typedef struct srvCellCfgCommSib
{ 
   DlCfgCommon        dlCfg;
   UlCfgCommon        ulCfg;
   uint8_t            ssbPosInBurst;
   long               ssbPrdServingCell;
   TddUlDlCfgCommon   tddCfg;
   long               ssPbchBlockPwr;
}SrvCellCfgCommSib;

typedef struct sib1Params
{
   Plmn    plmn;
   uint8_t   tac;
   long      ranac;
   uint8_t   cellIdentity;
   long      cellResvdForOpUse;
   long      connEstFailCnt;
   long      connEstFailOffValidity;
   SiSchedInfo           siSchedInfo;
   SrvCellCfgCommSib     srvCellCfgCommSib;
}Sib1Params;

typedef struct duCfgParams
{
   SctpParams         sctpParams;                  /* SCTP Params */
   F1EgtpParams       egtpParams;                  /* EGTP Params */
   uint32_t           maxUe;
   uint32_t           duId;
   uint8_t            duName[CU_DU_NAME_LEN_MAX];
   SchedulerCfg       schedCfg;
   F1DuSrvdCellInfo   srvdCellLst[MAX_NUM_CELL];  /* Serving cell list *///TODO: this must be removed eventually
   F1RrcVersion       rrcVersion;                 /* RRC version */
   MacCellCfg	       macCellCfg;	              /* MAC cell configuration */
   MibParams          mibParams;                  /* MIB Params */
   Sib1Params         sib1Params;                 /* SIB1 Params */
}DuCfgParams;

/*function declarations */
void FillSlotConfig();
uint8_t readClCfg();
uint8_t readCfg();
uint8_t duReadCfg(); 
uint16_t calcSliv(uint8_t startSymbol, uint8_t lengthSymbol);

#endif /* __DU_CONFIG_H__ */

/**********************************************************************
  End of file
 **********************************************************************/
