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

#include "du_common.h"

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

#define DU_PROC  0
/* Memory related configs */
#define DU_APP_MEM_REGION    1
#define RLC_UL_MEM_REGION     1
#define RLC_DL_MEM_REGION     4
#define RG_MEM_REGION     4

#define DU_POOL  1
#define RLC_POOL  1
#define RG_POOL 1

/* Events */
#define EVTCFG 0
#define EVTSCTPSTRT 1
#define EVTSCTPDATA 2
#define EVTSCTPNTFY 3

/* Selector */
#define DU_SELECTOR_LC   0
#define DU_SELECTOR_TC   1
#define DU_SELECTOR_LWLC 2

/* SAP IDs */
#define DU_RG_SUID 0
#define DU_RG_SPID 0

/* Instance */
#define RLC_UL_INST 0
#define RLC_DL_INST 1

#define DU_ZERO_VAL 0

/* Macros */
#define DEFAULT_CELLS    1

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


/* DU APP DB */
typedef struct duCb
{
   Mem      mem;
   TskInit  init;
   Bool     sctpStatus;
   Bool     f1Status;
   Bool     duStatus;

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
DuCfgParams duCfgParam;

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
S16 duSctpStartReq();
S16 duSctpNtfyHdl(Buffer *mBuf, CmInetSctpNotification *ntfy);
#endif

/**********************************************************************
         End of file
**********************************************************************/
