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

#ifndef __RIC_MGR_MAIN_H__
#define __RIC_MGR_MAIN_H__


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
#define RIC_DU_NAME_LEN_MAX 30      /* Max length of RIC/DU name string */

#define RIC_APP_MEM_REG 1
#define RIC_POOL 1

/* allocate and zero out a static buffer */
#define RIC_ALLOC(_datPtr, _size)                                \
{                                                               \
   S16 _ret;                                                    \
   _ret = SGetSBuf(RIC_APP_MEM_REG, RIC_POOL,                  \
                     (Data **)&_datPtr, _size);                  \
   if(_ret == ROK)                                              \
      cmMemset((U8*)_datPtr, 0, _size);                         \
   else                                                         \
      _datPtr = NULLP;                                          \
}
 
/* free a static buffer */
#define RIC_FREE(_datPtr, _size)                                 \
   SPutSBuf(RIC_APP_MEM_REG, RIC_POOL,                         \
         (Data *)_datPtr, _size);

typedef struct ipAddr
{
 Bool ipV4Pres;
 U32  ipV4Addr;
 Bool ipV6Pres;
 U8   ipV6Addr[MAX_IPV6_LEN];
}SctpIpAddr;

typedef struct RicSctpParams
{
   SctpIpAddr  duIpAddr;
   U16         duPort;
   SctpIpAddr  ricIpAddr;
   U16         ricPort;
}RicSctpParams;

typedef struct ricCfgParams
{
   U32             ricId;
   char            ricName[RIC_DU_NAME_LEN_MAX];
   RicSctpParams   sctpParams;
   Plmn            plmn;
}RicCfgParams;
RicCfgParams ricCfgParams; //global variable to hold all configs

void readRicCfg();
void cuAppInmsgHdlr(Buffer *mBuf);
void sctpNtfyInd(CmInetSctpNotification *ntfy);

#endif

/**********************************************************************
         End of file
**********************************************************************/
