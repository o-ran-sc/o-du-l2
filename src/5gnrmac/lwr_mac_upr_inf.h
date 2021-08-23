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

#define MAX_PREAMBLE_PER_SLOT      1    /* Max number of preamble per slot */
#define MAX_RACH_PDU_PER_SLOT   1    /* Max number of rach pdu per slot */
#define MAX_CRCS_PER_SLOT       1
#define MAX_CB_PER_TTI_IN_BYTES 1
#define MAX_ULSCH_PDUS_PER_TTI  1
#define MAX_ULCCH_PDUS_PER_TTI  1
#define MAX_NUM_HARQS_PER_TTI   1
#define MAX_HARQ_INFO_IN_BYTES 1
#define MAX_CSI_PART1_DATA_IN_BYTES 1
#define MAX_CSI_PART2_DATA_IN_BYTES 1
#define MAX_UCI_PDUS_PER_TTI    1
#define MAX_UCI_BIT_PER_TTI_IN_BYTES 2
#define UCI_IND_PUSCH     0      /* UCI Indication carried on PUSCH */
#define UCI_IND_PUCCH_F0F1   1   /* UCI Indication carried on PUCCH Format 0, 1 */
#define UCI_IND_PUCCH_F2F3F4 2   /* UCI Indication carried on PUCCH Format 2, 3, 4 */
#define SR_PDU_BITMASK    1      /* Bit Mask for SR_PDU */
#define HARQ_PDU_BITMASK  2      /* Bit Mask for HARQ PDU */
#define SR_NOT_DETECTED   0      /* SR not detected */
#define SR_DETECTED       1      /* SR detected */
#define CONFDC_LEVEL_GOOD 0      /* Confidence Level HARQ/SR */
#define CONFDC_LEVEL_BAD  1
#define HARQ_PASS         0
#define HARQ_FAIL         1
#define HARQ_NOT_PRESENT  3

/* Events */
#define EVENT_RACH_IND_TO_MAC    1
#define EVENT_CRC_IND_TO_MAC     2
#define EVENT_RX_DATA_IND_TO_MAC 3
#define EVENT_STOP_IND_TO_MAC    4
#define EVENT_SLOT_IND_TO_MAC    5
#define EVENT_UCI_IND_TO_MAC     6

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
   RachPreamInfo   preamInfo[MAX_PREAMBLE_PER_SLOT];
}RachPduInfo;

typedef struct rachInd
{
   uint16_t      cellId;
   SlotTimingInfo   timingInfo;
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
   SlotTimingInfo   timingInfo;
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
   SlotTimingInfo   timingInfo;
   uint16_t      numPdus;
   RxDataIndPdu  pdus[MAX_ULSCH_PDUS_PER_TTI];
}RxDataInd;

/* UCI Indication Structure */
typedef struct 
{
   uint8_t  harqCrc;
   uint16_t harqBitLen;
   uint8_t  harqPayload[MAX_HARQ_INFO_IN_BYTES];
}PuschHarqInfo;

typedef struct
{
   uint8_t  csiPart1Crc;
   uint16_t csiPart1BitLen;
   uint8_t  csiPart1Payload[MAX_CSI_PART1_DATA_IN_BYTES];
}PuschCsiPart1Info;

typedef struct
{
   uint8_t  csiPart2Crc;
   uint16_t csiPart2BitLen;
   uint8_t  csiPart2Payload[MAX_CSI_PART2_DATA_IN_BYTES];
}PuschCsiPart2Info;

typedef struct
{
   uint32_t handle;
   uint8_t  pduBitmap;
   uint8_t  ul_cqi;
   uint16_t crnti;
   uint16_t timingAdvance;
   uint16_t rssi;
   PuschHarqInfo harqInfo; 
   PuschCsiPart1Info csiPart1Info;
   PuschCsiPart2Info csiPart2Info;
}UciPusch;

typedef struct
{
   uint8_t srIndPres;
   uint8_t srConfdcLevel;
}SrInfoF0F1;

typedef struct
{
   uint8_t numHarq;
   uint8_t harqConfdcLevel;
   uint8_t harqValue[MAX_NUM_HARQS_PER_TTI];
}HarqInfoF0F1;

typedef struct
{
   uint32_t handle;
   uint8_t pduBitmap;
   uint8_t pucchFormat;
   uint8_t ul_cqi;
   uint16_t crnti;
   uint16_t timingAdvance;
   uint16_t rssi;         
   SrInfoF0F1   srInfo;
   HarqInfoF0F1 harqInfo;
}UciPucchF0F1;

typedef struct
{
   uint32_t handle;
   uint8_t pduBitmap;
   uint8_t pucchFormat;
   uint8_t ul_cqi;
   uint16_t crnti;
   uint16_t timingAdvance;
   uint16_t rssi;
   uint16_t num_uci_bits;
   uint8_t uciBits[MAX_UCI_BIT_PER_TTI_IN_BYTES];
}UciPucchF2F3F4;

typedef struct 
{
   uint16_t pduType;
   uint16_t pduSize;
   union
   {
      UciPusch      uciPusch; /*TODO: UCI Ind for PUSCH to be handled separately */
      UciPucchF0F1  uciPucchF0F1;
      UciPucchF2F3F4  uciPucchF2F3F4;
   }uci;
}UciIndPduInfo;

typedef struct
{
   uint16_t      cellId;
   SlotTimingInfo   slotInd;
   uint16_t      numUcis;
   UciIndPduInfo pdus[MAX_UCI_PDUS_PER_TTI];
}UciInd;

typedef uint8_t (*SlotIndFunc)(Pst *pst, SlotTimingInfo *slotInd);
typedef uint8_t (*RachIndFunc)(Pst *pst, RachInd *rachInd);
typedef uint8_t (*CrcIndFunc)(Pst *pst, CrcInd *crcInd);
typedef uint8_t (*RxDataIndFunc)(Pst *pst, RxDataInd *rxDataInd);
typedef uint8_t (*StopIndFunc)(Pst *pst, uint16_t *cellId);
typedef uint8_t (*UciIndFunc)(Pst *pst, UciInd *uciInd);

uint8_t packSlotInd (Pst *pst, SlotTimingInfo *slotInd);
uint8_t unpackSlotInd(SlotIndFunc func, Pst *pst, Buffer *mBuf);
uint8_t fapiMacSlotInd(Pst  *pst, SlotTimingInfo  *slotInd);
uint8_t packRachInd(Pst *pst, RachInd *rachInd);
uint8_t unpackRachInd(RachIndFunc func, Pst *pst, Buffer *mBuf);
uint8_t fapiMacRachInd(Pst *pst, RachInd *rachInd);
uint8_t packCrcInd(Pst *pst, CrcInd *crcInd);
uint8_t unpackCrcInd(CrcIndFunc func, Pst *pst, Buffer *mBuf);
uint8_t fapiMacCrcInd(Pst *pst, CrcInd *crcInd);
uint8_t packRxDataInd(Pst *pst, RxDataInd *rxDataInd);
uint8_t unpackRxDataInd(RxDataIndFunc func, Pst *pst, Buffer *mBuf);
uint8_t fapiMacRxDataInd(Pst *pst, RxDataInd *rxDataInd);
uint8_t packStopInd(Pst *pst, uint16_t *cellId);
uint8_t unpackStopInd(StopIndFunc func, Pst *pst, Buffer *mBuf);
uint8_t fapiMacStopInd(Pst *pst, uint16_t *cellId);
uint8_t packUciInd(Pst *pst, UciInd *uciInd);
uint8_t unpackUciInd(UciIndFunc func, Pst *pst, Buffer *mBuf);
uint8_t FapiMacUciInd(Pst *pst, UciInd *uciInd);

#endif
/**********************************************************************
  End of file
 **********************************************************************/
