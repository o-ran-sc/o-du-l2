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

#include "stdio.h"
#include "ctype.h"
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list  defines  */
#include "cm_hash.h"       /* Common hashlist  defines */
#include "cm_lte.h"
#include "cm_inet.h"
#include "lkw.h"
#include "lrg.h"
#include "legtp.h"
#include "du_app_mac_inf.h"

#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"
#include "cm_inet.x"
#include "cm_lib.x"
#include "lkw.x"
#include "lrg.x"

#include "du_cfg.h"

#define DU_PROC  0
/* Memory related configs */
#define DU_APP_MEM_REGION    0
#define RLC_UL_MEM_REGION     1
#define RLC_DL_MEM_REGION     4
#define RG_MEM_REGION     4

#define DU_POOL  1
#define RLC_POOL  1
#define RG_POOL 1

/* Events */
#define EVTCFG 0
#define EVTSCTPSTRT  1
#define EVENT_CU_DATA  2
#define EVENT_SCTP_NTFY  3
#define EVTSRVOPENREQ  4
#define EVTSRVOPENCFM  5
#define EVTTNLMGMTREQ  6
#define EVTTNLMGMTCFM  7
#define EVTSLOTIND    8
#define EVTSTARTPOLL  9
#define EVENT_RIC_DATA  10

/* Selector */
#define DU_SELECTOR_LC   0
#define DU_SELECTOR_TC   1
#define DU_SELECTOR_LWLC 2

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
   cmMemset((U8 *)(_buf), 0, _size);

/* allocate and zero out a static buffer */
#define DU_ALLOC(_datPtr, _size)                                \
{                                                               \
   S16 _ret;                                                    \
   _ret = SGetSBuf(DU_APP_MEM_REGION, DU_POOL,                  \
                    (Data **)&_datPtr, _size);                  \
   if(_ret == ROK)                                              \
      cmMemset((U8*)_datPtr, 0, _size);                         \
   else                                                         \
      _datPtr = NULLP;                                          \
}

/* free a static buffer */
#define DU_FREE(_datPtr, _size)                                 \
   if(_datPtr)                                                  \
      SPutSBuf(DU_APP_MEM_REGION, DU_POOL,                      \
         (Data *)_datPtr, _size);

/* Free shared memory, received through LWLC */
#define DU_FREE_SHRD_MEM(_region, _pool, _datPtr, _size)        \
   if(_datPtr)                                                  \
	   SPutSBuf(_region, _pool,(Data *)_datPtr, _size);          \

typedef enum
{
   CELL_OUT_OF_SERVICE,
   ACTIVATION_IN_PROGRESS,
   ACTIVATED,
   DELETION_IN_PROGRESS
}CellStatus;

typedef struct cellCfgParams
{
   NrEcgi      nrEcgi;         /* ECGI */
   U16         nrPci;          /* PCI */
   U16         fiveGsTac;         /* 5gSTac */
   Plmn      plmn[MAX_PLMN]; /* List of serving PLMN IDs */
   U32         maxUe;          /* max UE per slot */
}CellCfgParams;


typedef struct duCellCb
{
   U32            cellId;      /* Internal cell Id */
   CellCfgParams  cellInfo;    /* Cell info */
   CellStatus     cellStatus;  /*Cell status */
}DuCellCb;

typedef struct duLSapCb
{
   Pst pst;
   SuId        suId;
   SpId        spId;
   State       sapState;
   Mem         mem;
   CmTimer     timer;
   U8          bndRetryCnt;
   U8          maxBndRetry;
   TmrCfg      bndTmr;
}DuLSapCb;

/* DU APP DB */
typedef struct duCb
{
   Mem           mem;    /* Memory configs */
   TskInit       init;   /* DU Init */
   //DuLSapCb      **macSap;  /* MAC SAP */
   Bool          f1Status; /* Status of F1 connection */
   Bool          e2Status; /* Status of E2 connection */
   DuCellCb*     cfgCellLst[DU_MAX_CELLS];     /* List of cells at DU APP of type DuCellCb */
   DuCellCb*     actvCellLst[DU_MAX_CELLS];    /* List of cells activated/to be activated of type DuCellCb */
   /* pointer to store the address of macCellCfg params used to send du-app to MAC */
   MacCellCfg     *duMacCellCfg;     /* pointer to store params while sending DU-APP to MAC */
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
   U8          maxBndRetry;
   TmrCfg      bndTmr;
   TmrCfg      connTmr;
}DuLSapCfg;

/* global variables */
DuCb duCb;
//DuCfgParams duCfgParam;

/* DU Cell Functions */
S16 duActvInit(Ent entity, Inst inst, Region region, Reason reason);
S16 duActvTsk(Pst *pst, Buffer *mBuf);
S16 duSendRlcUlCfg();
S16 duSendRlcDlCfg();
S16 duBuildRlcCfg(Inst inst);
S16 duBuildRlcLsapCfg(Ent ent, Inst inst, U8 lsapInst);
S16 duBuildRlcUsapCfg(U8 elemId, Ent ent, Inst inst);
S16 duHdlRlcCfgComplete(Pst *pst, KwMngmt *cfm);
S16 duHdlRlcCntrlCfgComplete(Pst *pst, KwMngmt *cfm);
S16 duProcRlcUlCfgComplete(Pst *pst, KwMngmt *cfm);
S16 duProcRlcDlCfgComplete(Pst *pst, KwMngmt *cfm);
S16 duSendMacCfg();
S16 duBuildMacGenCfg();
S16 duBuildMacUsapCfg(SpId sapId);
S16 duHdlMacCfgComplete(Pst *pst, RgMngmt *cfm);
S16 duBindUnbindRlcToMacSap(U8 inst, U8 action);
S16 duProcCfgComplete();
S16 duSendSchCfg();
S16 duSctpStartReq();
S16 duSctpNtfyHdl(Buffer *mBuf, CmInetSctpNotification *ntfy);

S16 duBuildEgtpCfgReq();
S16 duHdlEgtpCfgComplete(CmStatus cfm);
S16 duSendEgtpSrvOpenReq();
S16 duHdlEgtpSrvOpenComplete(CmStatus cfm);
S16 duSendEgtpTnlMgmtReq(U8 action, U32 lclTeid, U32 remTeid);
S16 duHdlEgtpTnlMgmtCfm(EgtpTnlEvt tnlEvtCfm);
S16 duSendEgtpTestData();
S16 duSendEgtpDatInd(Buffer *mBuf);
S16 duHdlSchCfgComplete(Pst *pst, RgMngmt *cfm);
uint16_t duBuildAndSendMacCellStartReq();

#endif

/**********************************************************************
         End of file
**********************************************************************/
