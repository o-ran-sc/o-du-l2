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

#ifndef __CU_MGR_MAIN_H__
#define __CU_MGR_MAIN_H__


#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "ctype.h"
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "ss_queue.h"
#include "ss_task.h"
#include "ss_msg.h"
#include "cm_inet.h"
#include "cm_llist.h"      /* Common link list  defines  */
#include "cm_hash.h"       /* Common hashlist  defines */

#include "gen.x"           /* General */
#include "ssi.x"   /* System services */
#include "ss_queue.x"
#include "ss_task.x"
#include "ss_msg.x"
#include "cm_lib.x"
#include "cm_inet.x"
#include "cm_llist.x"      /* Common link list  defines  */
#include "cm_hash.x"       /* Common hashlist  defines */
#include "odu_common_codec.h"

#include "du_log.h"
#define MAX_IPV6_LEN 16
#define CU_DU_NAME_LEN_MAX 30      /* Max length of CU/DU name string */

#define CU_APP_MEM_REG 1
#define CU_POOL 1
#define MAX_DU_PORT 2
#define DU_PORT 38472

/* allocate and zero out a static buffer */
#define CU_ALLOC(_datPtr, _size)                                \
{                                                               \
   S16 _ret;                                                    \
   _ret = SGetSBuf(CU_APP_MEM_REG, CU_POOL,                  \
                     (Data **)&_datPtr, _size);                  \
   if(_ret == ROK)                                              \
      cmMemset((U8*)_datPtr, 0, _size);                         \
   else                                                         \
      _datPtr = NULLP;                                          \
}
 
/* free a static buffer */
#define CU_FREE(_datPtr, _size)                                 \
   SPutSBuf(CU_APP_MEM_REG, CU_POOL,                         \
         (Data *)_datPtr, _size);


typedef struct ipAddr
{
 Bool ipV4Pres;
 U32  ipV4Addr;
 Bool ipV6Pres;
 U8   ipV6Addr[MAX_IPV6_LEN];
}SctpIpAddr;

typedef struct RrcVersion
{
  U8    rrcVer;     /* Latest RRC Version */
  U32   extRrcVer;  /* Latest RRC version extended */
}RrcVersion;

typedef struct egtpParams
{
   SctpIpAddr  localIp;
   U16         localPort;
   SctpIpAddr  destIp;
   U16         destPort;
   U32       minTunnelId;
   U32       maxTunnelId;
}EgtpParams;

typedef struct CuSctpParams
{
   SctpIpAddr  duIpAddr;
   U16         duPort;
   SctpIpAddr  cuIpAddr;
   U16         cuPort;
}CuSctpParams;

typedef struct cuCfgParams
{
   U32              cuId;
   char             cuName[CU_DU_NAME_LEN_MAX];
   CuSctpParams     sctpParams;
   Plmn             plmn;
   EgtpParams       egtpParams;
   RrcVersion       rrcVersion;
}CuCfgParams;
CuCfgParams cuCfgParams; //global variable to hold all configs

void readCuCfg();
void cuAppInmsgHdlr(Buffer *mBuf);
void sctpNtfyInd(CmInetSctpNotification *ntfy);

#endif

/**********************************************************************
         End of file
**********************************************************************/
