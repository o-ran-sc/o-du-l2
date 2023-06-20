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

/* This file contains message handling functionality for DU cell management */
#ifndef __DU_MGR_H__
#define __DU_MGR_H__
#define DU_PROC  0

/* SAP IDs */
#define DU_MAC_SUID 0
#define DU_MAC_SPID 0

/* Instance */
#define RLC_UL_INST 0
#define RLC_DL_INST 1

/* SAP state */
#define DU_SAP_UNBOUND 1
#define DU_SAP_BINDING 2
#define DU_SAP_BOUND   3

#define DU_ZERO_VAL 0

/* Macros */
#define ADD 0
#define DEL 1

#define RLC_GEN_CFG      1
#define RLC_MAC_SAP_CFG  2
#define RLC_UDX_SAP_CFG  4

#define MAC_GEN_CFG   1
#define MAC_SAP_CFG   2
#define MAX_MAC_SAP   2

#define DU_RLC_UL_CONFIGURED  (RLC_GEN_CFG |  \
      RLC_MAC_SAP_CFG | RLC_UDX_SAP_CFG)
#define DU_RLC_DL_CONFIGURED DU_RLC_UL_CONFIGURED

#define MAC_CONFIGURED (MAC_GEN_CFG | MAC_SAP_CFG)

#define DU_SET_ZERO(_buf, _size)   \
   memset((_buf), 0, _size);

/*Refer Spec 38.331 Annexure A: maxNrofPageRec : 32 [Maximum number of page records]*/
#define MAX_PAGING_UE_RECORDS 32

typedef enum
{
   SLICE_INFO_NOT_AVAILABLE,
   SLICE_CONFIGURED,
   SLICE_RECONFIGURED
}SliceCfgState;

typedef enum
{
   CELL_OUT_OF_SERVICE,
   ACTIVATION_IN_PROGRESS,
   ACTIVATED,
   DELETION_IN_PROGRESS
}CellStatus;

typedef enum
{
   UE_INACTIVE,
   UE_ACTIVE,
   UE_HANDIN_IN_PROGRESS,
   UE_DELETION_IN_PROGRESS,
   UE_RESET_IN_PROGRESS
}UeState;

typedef enum
{
   UE_CTXT_UNKNOWN,
   UE_CTXT_SETUP,
   UE_CTXT_MOD,
   UE_CTXT_RRC_RECFG_COMPLETE,
   UE_CTXT_CFG_QUERY,
   UE_CTXT_RELEASE
}UeCtxtActionType;

/** F1AP Msg IE **/
typedef struct f1setupRsp
{
   uint32_t transId; /* Uniquely identify transaction */
   char     cuName[CU_DU_NAME_LEN_MAX];   /* CU Name */
   F1RrcVersion    rrcVersion;  /* RRC version */
}F1SetupRsp;

typedef struct f1DlRrcMsg 
{
   uint32_t gnbDuUeF1apId;
   uint32_t gnbCuUeF1apId;
   uint8_t  srbId;
   bool     execDup;
   bool     deliveryStatRpt;
   uint16_t rrcMsgSize;
   uint8_t  *rrcMsgPdu;
}F1DlRrcMsg;

typedef struct gtpTnlCfg
{
   uint32_t ulTnlAddress;  /* remote Address */
   uint32_t dlTnlAddress;  /* local Address */
   uint32_t teId;
}GtpTnlCfg;

typedef struct upTnlCfg
{
   ConfigType configType;
   uint8_t cellId;
   uint8_t ueId;
   uint8_t drbId;
   GtpTnlCfg *tnlCfg1; /* Tunnel 1 */
   GtpTnlCfg *tnlCfg2; /* Tunnel 2 */
}UpTnlCfg;

#ifdef NR_DRX
/* This structures is taken from the DRX Cycle section of
 * specification 33.473. */
typedef struct drxCycle
{
   uint16_t drxLongCycleLength;
   bool     shortDrxCyclePres;
   ShortDrx shortDrxCycle;
}DrxCycle;
#endif

typedef struct duLcCfg
{
   ConfigType configType;
   LcCfg      lcConfig;
}DuLcCfg;

typedef struct duMacUeCfg
{
   uint16_t               cellId;
   uint8_t                ueId;
   uint16_t               crnti;
   bool                   macCellGrpCfgPres;
   MacCellGrpCfg          macCellGrpCfg;
   bool                   phyCellGrpCfgPres;
   PhyCellGrpCfg          phyCellGrpCfg;
   bool                   spCellCfgPres;
   SpCellRecfg            spCellCfg;
   AmbrCfg                *ambrCfg;
   ModulationInfo         dlModInfo;    /* DL modulation info */ //TBD
   ModulationInfo         ulModInfo;    /* UL modulation info */  //TBD
   uint8_t                numLcs;
   DuLcCfg                lcCfgList[MAX_NUM_LC];
   UeCfgState             macUeCfgState;    /* InActive / Completed */ //TBD
   DataTransmissionAction transmissionAction; //TBD
#ifdef NR_DRX
   bool     drxConfigIndicatorRelease;
#endif
}DuMacUeCfg;

typedef struct duRlcBearerCfg
{
   ConfigType   configType;
   RlcBearerCfg rlcBearerCfg;
}DuRlcBearerCfg;

typedef struct duRlcUeCfg
{
   uint16_t       cellId;
   uint8_t        ueId;
   uint8_t        numLcs;
   DuRlcBearerCfg rlcLcCfg[MAX_NUM_LC];
   UeCfgState     rlcUeCfgState; /* InActive / Completed */
}DuRlcUeCfg;


typedef struct duUeCfg
{
   void *cellGrpCfg;
   void *ueNrCapability;
   uint8_t numRlcLcs;        /* Rlc Ue Cfg */
   DuRlcBearerCfg rlcLcCfg[MAX_NUM_LC];
   uint8_t numMacLcs;        /* Mac Ue Cfg */
   DuLcCfg   macLcCfg[MAX_NUM_LC];
   AmbrCfg *ambrCfg;
   uint8_t numDrb;
   UpTnlCfg upTnlInfo[MAX_NUM_DRB];  /* User plane TNL Info*/
   uint8_t numDrbSetupMod;        /*No. of DRB Added during Modification*/
   uint8_t numDrbModified;        /*No. of DRB Modified during Modification*/
   DuMacUeCfg copyOfmacUeCfg;
   DataTransmissionAction dataTransmissionAction;
#ifdef NR_DRX
   bool     drxCyclePres;
   DrxCycle drxCycle;
   bool     drxConfigIndicatorRelease;
#endif
}DuUeCfg;

typedef struct f1UeContextSetup
{
   UeCtxtActionType actionType;
   uint8_t cellIdx;
   DuUeCfg  duUeCfg;        
   bool     dlRrcMsgPres;
   F1DlRrcMsg  *dlRrcMsg;
}F1UeContextSetupDb;

typedef struct cellCfgParams
{
   NrEcgi      nrEcgi;         /* ECGI */
   uint16_t    nrPci;          /* PCI */
   uint16_t    fiveGsTac;         /* 5gSTac */
   Plmn        plmn[MAX_PLMN]; /* List of serving PLMN IDs */
   uint32_t    maxUe;          /* max UE per slot */
}CellCfgParams;

typedef struct duUeCb
{
   F1UeContextSetupDb *f1UeDb;
   uint16_t crnti;
   uint32_t gnbDuUeF1apId;   /* GNB DU UE F1AP ID */
   uint32_t gnbCuUeF1apId;   /* GNB CU UE F1AP ID */
   uint32_t drbBitMap;       /* Drb Bit Map */
   UeState  ueState;         /* UE Active/ Ue Inactive state */
   MacCfraResource cfraResource; /* CF-RA resource assigned by MAC/SCH */
   DuMacUeCfg duMacUeCfg;        /* Mac Ue Cfg */
   DuRlcUeCfg duRlcUeCfg;        /* Rlc Ue Cfg */
}DuUeCb;

typedef struct  slotInfo
{
   uint16_t sfn;
   uint16_t slot;
}SlotInfo;

typedef struct pagingMsg
{
   /*Note: Paging UEID is extracted from 5gSTMSI as per Spec 38.304, Sec 7.1.
    *This is not same as DU-UE_F1AP_ID or CU_UE_F1AP_ID*/
   uint16_t  pagUeId;              /* UE Identifier from CU*/

   /*TODO: When RAN Inititated Paging will be supported then I-RNTI will be
    * added as a choice for UE Identity along with S-TMSI*/
   uint64_t  sTmsi;             /* UE Paging Identity: S-TMSI */
   bool      pagingDrxPres;     /* flag to indicate paging drx present or not */
   uint16_t  pagingDrx;         /* UE specific paging drx */
   uint8_t   pagPriority;       /* Paging priority */
   uint16_t  pagingFrame;       /* paging frame */
   uint16_t  pagingFrameOffset; /* paging offset */
   uint8_t   i_s;               /* Index points to PO from the sub-frame pattern */
   uint16_t  T;                 /* T is DRX cycle of the UE */
}DuPagingMsg;

typedef struct duPagUeRecord
{
   uint16_t  pagUeId;           /*UEID calculated from 5gsTMSI as per Spec 38.304*/
   uint64_t  sTmsi;             /* UE Paging Identity: S-TMSI */
}DuPagUeRecord;

typedef struct duPagInfo
{
   uint8_t      i_s;          /*Index of PO*/
   CmLListCp    pagUeList;    /*List of UEs to be paged in this Paging Frame/Paging Occ*/
}DuPagUeList;

typedef struct duPagInfoMap
{
   uint16_t    pf;             /* Paging Frame*/
   CmLListCp   pagInfoList;    /* Master List of Paging Identified by PF and i_s*/
}DuPagInfoList;

typedef struct duCellCb
{
   uint16_t       cellId;                 /* Internal cell Id */
   CellCfgParams  cellInfo;               /* Cell info */
   /* pointer to store the address of macCellCfg params used to send du-app to MAC */
   MacCellCfg     *duMacCellCfg;
   CellStatus     cellStatus;             /* Cell status */
   uint32_t       numActvUes;             /* Total Active UEs */
   DuUeCb         ueCb[MAX_NUM_UE];       /* Stores UE context */
   SlotInfo       currSlotInfo;
   DuPagingMsg    tmpPagingInfoOfUe;      /* UE paging information */
   CmHashListCp   pagingInfoMap;          /*Paging Map between PF and PagingInfoList*/
}DuCellCb;

typedef struct duLSapCb
{
   Pst pst;
   SuId        suId;
   SpId        spId;
   State       sapState;
   Mem         mem;
   CmTimer     timer;
   uint8_t     bndRetryCnt;
   uint8_t     maxBndRetry;
   TmrCfg      bndTmr;
}DuLSapCb;

typedef struct ueCcchCtxt
{
   uint32_t gnbDuUeF1apId; /* GNB DU UE F1AP ID */
   uint16_t crnti;         /* CRNTI */
   uint16_t cellId;        /* Cell Id */
}UeCcchCtxt;

/* This structure is added to store the information about the F1AP pdu and
 * transId so that It can be used in future */
typedef struct reservedF1apPduInfo
{
   uint8_t transId;               /* trans Id */
   void *f1apMsg;              /* msg structure */
}ReservedF1apPduInfo;

typedef struct duTimer
{
   CmTqCp       tmrTqCp;               /*!< Timer Task Queue Cntrl Point */
   CmTqType     tmrTq[DU_TQ_SIZE];    /*!< Timer Task Queue */
   uint8_t      tmrRes;              /*!< Timer resolution */
}DuTimers;

/* DU APP DB */
typedef struct duCb
{
   Mem           mem;                       /* Memory configs */
   TskInit       init;                      /* DU Init */
   bool          f1Status;                  /* Status of F1 connection */
   bool          e2Status;                  /* Status of E2 connection */
   E2apDb        e2apDb;                    /* E2AP database */
   uint8_t       numCfgCells;               /* number of configured cells */ 
   DuCellCb*     cfgCellLst[MAX_NUM_CELL];  /* List of cells at DU APP of type DuCellCb */
   uint8_t       numActvCells;              /* Number of active cells */
   DuCellCb*     actvCellLst[MAX_NUM_CELL]; /* List of cells activated/to be activated of type DuCellCb */
   uint32_t      gnbDuUeF1apIdGenerator;    /* Generating Du Ue F1ap Id */
   uint32_t      numUe;                     /* current number of UEs */
   UeCcchCtxt    ueCcchCtxt[MAX_NUM_UE];    /* mapping of gnbDuUeF1apId to CRNTI required for CCCH processing*/
   uint8_t       numTeId;                   /* current number of TEIDs configured in the system*/
   UpTnlCfg*     upTnlCfg[MAX_TEID];        /* tunnel info for every Drb */
   CmLListCp     reservedF1apPduList;       /*storing F1AP pdu infomation and transId */
   SliceCfgState sliceState;                /* Slice status */ 
   F1SetupMsg    f1SetupReqAndRspMsg;       /* f1 Setup Req And Rsp Msg*/
   DuTimers      duTimersInfo;              /* Du timers queue */
}DuCb;


typedef struct duLSapCfg
{
   SuId        suId;
   SpId        spId;
   Ent         srcEnt;
   Inst        srcInst;
   ProcId      dstProcId;
   Ent         dstEnt;
   Inst        dstInst;
   Priority    dstPrior;
   Route       dstRoute;
   Selector    dstSel;
   Mem         mem;
   uint8_t          maxBndRetry;
   TmrCfg      bndTmr;
   TmrCfg      connTmr;
}DuLSapCfg;

/* global variables */
DuCb duCb;
//DuCfgParams duCfgParam;

/* DU Cell Functions */
uint8_t duActvInit(Ent entity, Inst inst, Region region, Reason reason);
uint8_t duActvTsk(Pst *pst, Buffer *mBuf);
uint8_t duSendRlcUlCfg();
uint8_t duSendRlcDlCfg();
uint8_t duBuildRlcCfg(Inst inst);
uint8_t duBuildRlcLsapCfg(Ent ent, Inst inst, uint8_t lsapInst);
uint8_t duBuildRlcUsapCfg(uint8_t elemId, Ent ent, Inst inst);
uint8_t DuHdlRlcCfgComplete(Pst *pst, RlcMngmt *cfm);
uint8_t duHdlRlcCntrlCfgComplete(Pst *pst, RlcMngmt *cfm);
uint8_t duProcRlcUlCfgComplete(Pst *pst, RlcMngmt *cfm);
uint8_t duProcRlcDlCfgComplete(Pst *pst, RlcMngmt *cfm);
uint8_t duSendMacCfg();
uint8_t duBuildMacGenCfg();
uint8_t duBuildMacUsapCfg(SpId sapId);
uint8_t duHdlMacCfgComplete(Pst *pst, RgMngmt *cfm);
uint8_t duBindUnbindRlcToMacSap(uint8_t inst, uint8_t action);
uint8_t duProcCfgComplete();
uint8_t duSendSchCfg();
uint8_t duSctpStartReq();
uint8_t duSctpNtfyHdl(Buffer *mBuf, CmInetSctpNotification *ntfy);

uint8_t duBuildEgtpCfgReq();
uint8_t duHdlEgtpCfgComplete(CmStatus cfm);
uint8_t duSendEgtpSrvOpenReq();
uint8_t duHdlEgtpSrvOpenComplete(CmStatus cfm);
uint8_t duSendEgtpTnlMgmtReq(uint8_t action, uint32_t teIdMod, GtpTnlCfg *tnlInfo);
uint8_t duSendEgtpDatInd(Buffer *mBuf);
uint8_t duHdlSchCfgComplete(Pst *pst, RgMngmt *cfm);
uint8_t duBuildAndSendMacCellStart();
uint8_t duBuildAndSendMacCellStop(uint16_t cellId);

#endif

/**********************************************************************
  End of file
 **********************************************************************/
