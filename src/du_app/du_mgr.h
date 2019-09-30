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
#define DU_POOL  1
/* Events */
#define EVTCFG 0
#define EVTSCTPUP 1

/* Selector */
#define DU_SELECTOR_LC   0
#define DU_SELECTOR_TC   1
#define DU_SELECTOR_LWLC 2

/* DU APP DB */
typedef struct duDb
{
   F1Status f1Status;

}DuDb;


/* DU Cell Functions */
S16 duActvInit(Ent entity, Inst inst, Region region, Reason reason);
S16 duActvTsk(Pst *pst, Buffer *mBuf);
S16 duProcCfgComplete();

#endif

/**********************************************************************
         End of file
**********************************************************************/
