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
#include "common_def.h"
#include "tfu.h"
 
/* header/extern include files (.x) */
#include "tfu.x"

/* events */
#define EVENT_RACH_IND_TO_MAC 0
#define EVENT_CRC_IND_TO_MAC  1
#define EVENT_RX_DATA_IND_TO_MAC 2
#define EVENT_STOP_IND_TO_MAC 3

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

typedef uint16_t (*packStopIndMsg)(Pst *pst);
uint16_t packStopInd(Pst *pst);
uint16_t fapiMacStopInd(Pst *pst);
#endif
/**********************************************************************
         End of file
**********************************************************************/
