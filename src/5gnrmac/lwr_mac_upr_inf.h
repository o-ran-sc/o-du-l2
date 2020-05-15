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

/* header include files -- defines (.h)  */

#ifndef __LWR_MAC_UPR_INF_H__
#define __LWR_MAC_UPR_INF_H__

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_lte.h"        /* common tokens */
#include "tfu.h"
 
/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"        /* common tokens */
#include "tfu.x"

typedef S16 (*packSlotIndMsg)(Pst *pst, SlotIndInfo *slotInd);
S16 packLcSlotInd (Pst *pst, SlotIndInfo *slotInd);
S16 packLwlcSlotInd (Pst *pst, SlotIndInfo *slotInd);

typedef uint16_t (*packRachIndMsg)(Pst *pst, RachInd *rachInd);
uint16_t packRachInd(Pst *pst, RachInd *rachInd);
uint16_t fapiMacRachInd(Pst *pst, RachInd *rachInd);


typedef uint16_t (*packCrcIndMsg)(Pst *pst, CrcInd *crcInd);
uint16_t packCrcInd(Pst *pst, CrcInd *crcInd);
uint16_t fapiMacCrcInd(Pst *pst, CrcInd *crcInd);


typedef uint16_t (*packRxDataIndMsg)(Pst *pst, RxDataInd *rxDataInd);
uint16_t packRxDataInd(Pst *pst, RxDataInd *rxDataInd);
uint16_t fapiMacRxDataInd(Pst *pst, RxDataInd *rxDataInd);

#endif
/**********************************************************************
         End of file
**********************************************************************/
