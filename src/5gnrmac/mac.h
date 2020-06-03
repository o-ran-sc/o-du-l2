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
#ifndef _MAC_H_
#define _MAC_H_

#include <stdio.h>

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_lte.h"        /* Common LTE Defines */
#include "cm_mblk.h"        /* Common LTE Defines */
#include "tfu.h"           /* RGU Interface defines */
#include "lrg.h"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_lte.x"        /* Common LTE Defines */
#include "cm_mblk.x"        /* Common LTE Defines */
#include "tfu.x"           /* RGU Interface includes */
#include "lrg.x"

#define MAX_SLOT_SUPPORTED    10 /* numerology 0 15Khz */
#define MAX_ZERO_CORR_CFG_IDX 16 /* max zero correlation config index */
#define SI_RNTI 0xFFFF
#define P_RNTI 0xFFFE

#define MAC_LCID_CCCH              0
#define MAC_DEDLC_MIN_LCID         1
#define MAC_DEDLC_MAX_LCID         32
#define MAC_LCID_RESERVED_MIN      33
#define MAC_LCID_RESERVED_MAX      51
#define MAC_LCID_CCCH_48BIT        52
#define MAC_LCID_BIT_RATE_QUERY    53
#define MAC_LCID_MULT_PHR_FOUR_OCT 54
#define MAC_LCID_CFG_GRANT_CFM     55
#define MAC_LCID_MULT_PHR_ONE_OCT  56
#define MAC_LCID_SINGLE_PHR        57
#define MAC_LCID_CRNTI             58
#define MAC_LCID_SHORT_TRUNC_BSR   59
#define MAC_LCID_LONG_TRUNC_BSR    60
#define MAC_LCID_SHORT_BSR         61
#define MAC_LCID_LONG_BSR          62
#define MAC_LCID_PADDING           63

typedef struct macDlSlot
{
   DlAlloc dlInfo;
}MacDlSlot;

typedef struct macUlSlot
{
   UlSchInfo    ulCellInfo;
}MacUlSlot;

typedef struct macCellCb
{
   uint16_t   cellId;
   RachIndInfo raCb;
   MacDlSlot  dlSlot[MAX_SLOT_SUPPORTED];
   MacUlSlot  ulSlot[MAX_SLOT_SUPPORTED];
}MacCellCb;

typedef struct macCb
{
   Inst       macInst;
   MacCellCb  *macCell;
}MacCb;

/* global variable */
MacCb macCb;
void fillMacToSchPst(Pst *pst);
void fillRarPdu(RarInfo *rarInfo);
int unpackRxData(SlotIndInfo timingInfo, RxDataIndPdu *rxDataIndPdu);

#endif
/**********************************************************************
  End of file
 **********************************************************************/
