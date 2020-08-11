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

#define MAX_PREAM_PER_SLOT      1    /* Max number of preamble per slot */
#define MAX_RACH_PDU_PER_SLOT   1    /* Max number of rach pdu per slot */
#define MAX_CRCS_PER_SLOT       1
#define MAX_CB_PER_TTI_IN_BYTES 1
#define MAX_ULSCH_PDUS_PER_TTI  1

/* Selector */
#define MAC_SELECTOR_LC   0
#define MAC_SELECTOR_TC   1
#define MAC_SELECTOR_LWLC 2

/* events */
#define EVENT_RACH_IND_TO_MAC    0
#define EVENT_CRC_IND_TO_MAC     1
#define EVENT_RX_DATA_IND_TO_MAC 2
#define EVENT_STOP_IND_TO_MAC    3
#define EVENT_SLOT_IND_TO_MAC    4

typedef struct rachPreamInfo
{
   uint8_t   preamIdx;
   uint16_t  timingAdv;
}RachPreamInfo;

typedef struct rachPduInfo
{
   uint16_t   pci;
   uint8_t    symbolIdx;
   uint8_t    slotIdx;
   uint8_t    freqIdx;
   uint8_t    numPream;
   RachPreamInfo   preamInfo[MAX_PREAM_PER_SLOT];
}RachPduInfo;

typedef struct rachInd
{
   uint16_t      cellId;
   SlotIndInfo   timingInfo;
   uint8_t       numPdu;
   RachPduInfo   rachPdu[MAX_RACH_PDU_PER_SLOT];
}RachInd;

typedef struct crcInfo
{
   uint32_t   handle;
   uint16_t   rnti;
   uint8_t    harqId;
   uint8_t    tbCrcStatus;
   uint16_t   numCb;
   uint8_t    cbCrcStatus[MAX_CB_PER_TTI_IN_BYTES];
   uint8_t    ul_cqi;
   uint16_t   timingAdvance;
   uint16_t   rssi;
}CrcInfo;

typedef struct crcInd
{
   uint16_t      cellId;
   SlotIndInfo   timingInfo;
   uint16_t      numCrc;
   CrcInfo       crcInfo[MAX_CRCS_PER_SLOT];
}CrcInd;

typedef struct
{
   uint32_t      handle;
   uint16_t      rnti;
   uint8_t       harqId;
   uint16_t      pduLength;
   uint8_t       ul_cqi;
   uint16_t      timingAdvance;
   uint16_t      rssi;
   uint8_t       *pduData;
}RxDataIndPdu;

typedef struct
{
   uint16_t      cellId;
   SlotIndInfo   timingInfo;
   uint16_t      numPdus;
   RxDataIndPdu  pdus[MAX_ULSCH_PDUS_PER_TTI];
}RxDataInd;

typedef uint8_t (*packSlotIndMsg)(Pst *pst, SlotIndInfo *slotInd);
uint8_t packLcSlotInd (Pst *pst, SlotIndInfo *slotInd);
uint8_t packLwlcSlotInd (Pst *pst, SlotIndInfo *slotInd);

uint8_t fapiMacSlotInd(Pst  *pst, SlotIndInfo  *slotInd);

typedef uint8_t (*packRachIndMsg)(Pst *pst, RachInd *rachInd);
uint8_t packRachInd(Pst *pst, RachInd *rachInd);
uint8_t fapiMacRachInd(Pst *pst, RachInd *rachInd);

typedef uint8_t (*packCrcIndMsg)(Pst *pst, CrcInd *crcInd);
uint8_t packCrcInd(Pst *pst, CrcInd *crcInd);
uint8_t fapiMacCrcInd(Pst *pst, CrcInd *crcInd);


typedef uint8_t (*packRxDataIndMsg)(Pst *pst, RxDataInd *rxDataInd);
uint8_t packRxDataInd(Pst *pst, RxDataInd *rxDataInd);
uint8_t fapiMacRxDataInd(Pst *pst, RxDataInd *rxDataInd);

typedef uint8_t (*packStopIndMsg)(Pst *pst, uint16_t cellId);
uint8_t packStopInd(Pst *pst, uint16_t cellId);
uint8_t fapiMacStopInd(Pst *pst, uint16_t cellId);
#endif
/**********************************************************************
  End of file
 **********************************************************************/
