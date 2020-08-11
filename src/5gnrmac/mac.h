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

#define DEFAULT_CELLS 1
#define MAX_SLOT_SUPPORTED    10 /* numerology 0 15Khz */
#define MAX_ZERO_CORR_CFG_IDX 16 /* max zero correlation config index */
#define SI_RNTI 0xFFFF
#define P_RNTI 0xFFFE
#define MAX_MAC_CE 6
#define MAX_UE 1
#define MAX_CRI_SIZE 6
#define MAX_MAC_DL_PDU 10

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
#define MAC_LCID_CRI               62
#define MAC_LCID_PADDING           63

#define MAC_MEM_REGION       4
#define MAC_POOL 1
/* allocate and zero out a MAC static buffer */
#define MAC_ALLOC(_datPtr, _size)                            \
{                                                            \
   S16 _ret;                                                 \
   _ret = SGetSBuf(MAC_MEM_REGION, MAC_POOL,                 \
	 (Data **)&_datPtr, _size);               \
   if(_ret == ROK)                                           \
      cmMemset((U8*)_datPtr, 0, _size);                      \
   else                                                      \
      _datPtr = NULLP;                                       \
}

/* free a static buffer */
#define MAC_FREE(_datPtr, _size)                             \
   if(_datPtr)                                               \
      SPutSBuf(MAC_MEM_REGION, MAC_POOL,                     \
      (Data *)_datPtr, _size);

/* Allocate shared memory to be used for LWLC
 * during inter-layer communication */
#define MAC_ALLOC_SHRABL_BUF(_buf, _size)                    \
{                                                            \
   if(SGetStaticBuffer(MAC_MEM_REGION, MAC_POOL,             \
	    (Data **)&_buf, (Size) _size, 0) == ROK)               \
   {                                                         \
      cmMemset((U8 *)(_buf), 0, _size);                      \
   }                                                         \
   else                                                      \
   {                                                         \
      (_buf) = NULLP;                                        \
   }                                                         \
}

/* Free shared memory, received through LWLC */
#define MAC_FREE_SHRABL_BUF(_region, _pool,_buf, _size)       \
{                                                            \
   if (_buf != NULLP)                                        \
   {                                                         \
      (Void) SPutStaticBuffer(_region, _pool,                \
	    (Data *) _buf, (Size) _size, 0);                 \
      _buf = NULLP;                                         \
   }                                                         \
}

/* Fill Pst structure for sending msg from MAC to DU APP */
#define FILL_PST_TO_DUAPP(_pst, _event)                     \
{                                                           \
   _pst.selector  = ODU_SELECTOR_LWLC;                      \
   _pst.srcEnt    = ENTRG;                                  \
   _pst.dstEnt    = ENTDUAPP;                               \
   _pst.dstInst   = 0;                                      \
   _pst.srcInst   = macCb.macInst;                          \
   _pst.dstProcId = macCb.procId;                           \
   _pst.srcProcId = macCb.procId;                           \
   _pst.region = MAC_MEM_REGION;                            \
   _pst.pool = MAC_POOL;                                    \
   _pst.event = _event;                                     \
   _pst.route = 0;                                          \
   _pst.prior = 0;                                          \
   _pst.intfVer = 0;                                        \
}

/* Fill Pst structure for sending msg from MAC to SCH */
#define FILL_PST_TO_SCH(_pst, _event)                       \
{                                                           \
   _pst.selector  = ODU_SELECTOR_TC;                        \
   _pst.srcEnt    = ENTRG;                                  \
   _pst.dstEnt    = ENTRG;                                  \
   _pst.dstInst   = 1;                                      \
   _pst.srcInst   = macCb.macInst;                          \
   _pst.dstProcId = macCb.procId;                           \
   _pst.srcProcId = macCb.procId;                           \
   _pst.region = MAC_MEM_REGION;                            \
   _pst.pool = MAC_POOL;                                    \
   _pst.event = _event;                                     \
   _pst.route = 0;                                          \
   _pst.prior = 0;                                          \
   _pst.intfVer = 0;                                        \
}

typedef struct macDlSlot
{
   DlSchedInfo dlInfo;
}MacDlSlot;

typedef struct macUlSlot
{
   UlSchedInfo  ulInfo;
}MacUlSlot;

typedef struct macCbInfo
{
   uint16_t    cellId;
   uint16_t    crnti;
   uint8_t     msg3Pdu[6];  /* used as CRI value during muxing */
   uint8_t     *msg4Pdu;    /* storing DL-CCCH Ind Pdu */
   uint16_t    msg4PduLen;  /* storing DL-CCCH Ind Pdu Len */
   uint8_t     *msg4TxPdu;  /* muxed Pdu used for re-transmission */
   uint16_t    msg4TbSize;  /* size required for msg4TxPdu */
}MacRaCbInfo;

typedef struct macCe
{
   uint16_t macCeLcid;
   uint8_t  macCeValue[6];
}MacCe;

typedef struct macCeInfo
{
   uint16_t numCes;
   MacCe macCe[MAX_MAC_CE];
}MacCeInfo;

typedef struct macDlInfo
{
   uint16_t  lcId;
   uint16_t  pduLen;
   uint8_t  *dlPdu;
}MacDlInfo;

typedef struct macDlData
{
   uint16_t numPdu;
   MacDlInfo  pduInfo[MAX_MAC_DL_PDU];
}MacDlData;

typedef struct macCellCb
{
   uint16_t    cellId;
   MacRaCbInfo macRaCb[MAX_UE];
   MacDlSlot   dlSlot[MAX_SLOT_SUPPORTED];
   MacUlSlot   ulSlot[MAX_SLOT_SUPPORTED];
   MacCellCfg  macCellCfg;
   SlotIndInfo currTime;
}MacCellCb;

typedef struct macCb
{
   Inst       macInst;
   ProcId     procId;
   MacCellCb  *macCell;
}MacCb;

/* global variable */
MacCb macCb;
void fillMacToSchPst(Pst *pst);
void fillRarPdu(RarInfo *rarInfo);
void createMacRaCb(uint16_t cellId, uint16_t crnti);
void fillMsg4DlData(MacDlData *dlData, uint8_t *msg4Pdu);
void fillMacCe(MacCeInfo  *macCeData, uint8_t *msg3Pdu);
void macMuxPdu(MacDlData *dlData, MacCeInfo *macCeData, uint8_t *msg4TxPdu, uint16_t tbSize);
int unpackRxData(RxDataIndPdu *rxDataIndPdu);
uint16_t macSendUlCcchInd(uint8_t *rrcContainer, uint16_t cellId, uint16_t crnti);
void fillMg4Pdu(Msg4Alloc *msg4Alloc);
void buildAndSendMuxPdu(SlotIndInfo currTimingInfo);

#endif
/**********************************************************************
  End of file
 **********************************************************************/
