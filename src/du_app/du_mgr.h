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
   memset((uint8_t *)(_buf), 0, _size);

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
   UE_ACTIVE
}UeState;

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
   uint32_t    gnbDuUeF1apId; /* GNB DU UE F1AP ID */
   uint32_t    gnbCuUeF1apId; /* GNB CU UE F1AP ID */
   UeState     ueState;
   MacUeCfg    macUeCfg;
   RlcUeCfg    rlcUeCfg;
}DuUeCb;

typedef struct duCellCb
{
   uint16_t       cellId;           /* Internal cell Id */
   CellCfgParams  cellInfo;         /* Cell info */
   /* pointer to store the address of macCellCfg params used to send du-app to MAC */
   MacCellCfg    *duMacCellCfg;
   bool           firstSlotIndRcvd;
   CellStatus     cellStatus;       /* Cell status */
   uint32_t       numActvUes;       /* Total Active UEs */
   DuUeCb         ueCb[MAX_NUM_UE];  /* UE CONTEXT */
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


/* DU APP DB */
typedef struct duCb
{
   Mem           mem;    /* Memory configs */
   TskInit       init;   /* DU Init */
   //DuLSapCb      **macSap;  /* MAC SAP */
   bool          f1Status; /* Status of F1 connection */
   bool          e2Status; /* Status of E2 connection */
   uint8_t       numCfgCells; 
   DuCellCb*     cfgCellLst[MAX_NUM_CELL];     /* List of cells at DU APP of type DuCellCb */
   uint8_t       numActvCells;
   DuCellCb*     actvCellLst[MAX_NUM_CELL];    /* List of cells activated/to be activated of type DuCellCb */
   uint32_t       numUe;            /* current number of UEs */
   UeCcchCtxt     ueCcchCtxt[MAX_NUM_UE]; /* mapping of gnbDuUeF1apId to CRNTI required for CCCH processing*/
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
uint8_t duSendEgtpTnlMgmtReq(uint8_t action, uint32_t lclTeid, uint32_t remTeid);
uint8_t duHdlEgtpTnlMgmtCfm(EgtpTnlEvt tnlEvtCfm);
uint8_t duSendEgtpTestData();
uint8_t duSendEgtpDatInd(Buffer *mBuf);
uint8_t duHdlSchCfgComplete(Pst *pst, RgMngmt *cfm);
uint8_t duBuildAndSendMacCellStartReq();
uint8_t duBuildAndSendMacCellStopReq();
#endif

/**********************************************************************
  End of file
 **********************************************************************/
