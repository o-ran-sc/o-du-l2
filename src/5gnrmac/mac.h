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

/* MAX values */
#define MAX_MAC_CE 6
#define MAX_CRI_SIZE 6
#define MAX_MAC_DL_PDU 16
#define MAX_NUM_HARQ_PROC  16
#define MAX_ZERO_CORR_CFG_IDX 16 /* max zero correlation config index */
#define MAC_TQ_SIZE    10        /* Timing Queue Size */
#define MAX_NUM_TIMER  1         /* MAX number of MAC timers */

#define DEFAULT_CELLS 1
#define SI_RNTI 0xFFFF
#define P_RNTI 0xFFFE

#define MAC_LCID_CCCH              0
#define MAC_LCID_MIN               1
#define MAC_LCID_MAX               32
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

/* Definitions for size of RAR PDU contents (in bits).
 * Refer spec 38.321, sections 6.1.4, 6.2.2 and 6.2.3*/
#define E_BIT_SIZE 1
#define T_BIT_SIZE 1
#define R_BIT_SIZE 1
#define RAPID_SIZE 6
#define LC_ID_SIZE 6
#define TIMING_ADVANCE_SIZE 12
#define T_CRNTI_SIZE 16
/*  UL Grant is of size = 27 bits. Refer to Spec 38.213, Table 8.2-1 for
 *  contents of UL grant in RAR */
#define FREQ_HOP_FLAG_SIZE 1
#define FREQ_RSRC_ALLOC_SIZE 14
#define TIME_RSRC_ALLOC_SIZE 4
#define MCS_SIZE 4
#define TPC_COMMAND_SIZE 3
#define CSI_REQUEST_SIZE 1

typedef struct macCellCb MacCellCb;

typedef enum
{
   CELL_STATE_UP,
   CELL_TO_BE_STOPPED,
   CELL_STOP_IN_PROGRESS,
}CellState;	   

typedef enum
{
   UE_STATE_INACTIVE,
   UE_STATE_ACTIVE
}UeState;

typedef enum
{
   MAC_LC_STATE_INACTIVE,
   MAC_LC_STATE_ACTIVE
}MacLcState;

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
   uint8_t  ueId;
   uint16_t numPdu;
   MacDlInfo  pduInfo[MAX_MAC_DL_PDU];
}MacDlData;

/* HARQ Process Info */
typedef struct dlHarqProcCb
{
   uint8_t   procId;    /* HARQ Process Id */
}DlHarqProcCb;

/* DL HARQ entity */
typedef struct dlHarqEnt
{
   uint8_t        maxReTx;            /* MAX HARQ retransmission */
   uint8_t        numHarqProcs;       /* Number of HARQ procs */
   DlHarqProcCb   harqProcCb[MAX_NUM_HARQ_PROC];
}DlHarqEnt;

/* Uplink deidcated logical channel info */
typedef struct ulLcCb
{
   uint8_t    lcId;         /* Logical Channel Id */
   uint8_t    lcGrpId;      /* Logical Channel group */
   MacLcState lcActive;     /* Is LC active ? */
   /*Commenting as S-NSSAI and PDU session will be used in future scope*/
   /*For eg: When we have to send these for AMBR cases*/
   #if 0
   uint16_t   pduSessionId; /*Pdu Session Id*/
   Snssai     *snssai;      /*S-NSSAI assoc with LCID*/
   #endif
}UlLcCb;

/* Downlink dedicated logical channel info */
typedef struct dlLcCb
{
   uint8_t    lcId;        /* Logical channel Id */ 
   MacLcState lcState;     /* Is LC active ? */
   /*Commenting as S-NSSAI and PDU session will be used in future scope*/
   /*For eg: When we have to send these info via FAPI to phy layer*/
   #if 0
   uint16_t   pduSessionId;/*Pdu Session Id*/
   Snssai     *snssai;    /*S-NSSAI assoc with LCID*/
   #endif
}DlLcCb;

/* BSR Information */
typedef struct macBsrTmrCfg
{
   uint16_t   periodicTimer;
   uint16_t   retxTimer;
   uint16_t   srDelayTimer;
}MacBsrTmrCfg;

/* UE specific UL info */
typedef struct ueUlCb
{
   uint8_t    maxReTx;     /* MAX HARQ retransmission */
   uint8_t    numUlLc;     /* Number of uplink logical channels */       
   UlLcCb     lcCb[MAX_NUM_LC];    /* Uplink dedicated logocal channels */
}UeUlCb;

/* UE specific DL Info */
typedef struct ueDlCb
{
   DlHarqEnt  dlHarqEnt;      /* DL HARQ entity */
   uint8_t    numDlLc;        /* Number of downlink logical channels */
   DlLcCb     lcCb[MAX_NUM_LC];  /* Downlink dedicated logical channels */
}UeDlCb;

/* UE Cb */
typedef struct macUeCb
{
   uint16_t     ueId;     /* UE Id assigned by DU APP */
   uint16_t     crnti;    /* UE CRNTI */
   MacCellCb    *cellCb;  /* Pointer to cellCb to whihc this UE belongs */
   UeState      state;    /* Is UE active ? */
   MacRaCbInfo  *raCb;    /* RA info */
   MacBsrTmrCfg bsrTmrCfg;  /* BSR Timer Info */
   UeUlCb       ulInfo;   /* UE specific UL info */
   UeDlCb       dlInfo;   /* UE specific DL info */
}MacUeCb;

struct macCellCb
{
   uint16_t    cellId;
   CellState   state;
   uint8_t     crntiMap;
   MacRaCbInfo macRaCb[MAX_NUM_UE];
   MacDlSlot   dlSlot[MAX_SLOTS];
   MacUlSlot   ulSlot[MAX_SLOTS];
   uint16_t    numActvUe;
   MacUeCfg    *ueCfgTmpData[MAX_NUM_UE];
   MacUeCb     ueCb[MAX_NUM_UE];
   MacCellCfg  macCellCfg;
   SlotTimingInfo currTime;
};

typedef struct macCb
{
   Inst        macInst;
   ProcId      procId;
   uint8_t     tmrRes;                    /*!< Timer resolution */
   CmTqCp      tmrTqCp;                   /*!< Timer Task Queue Cntrl Point */
   CmTqType    tmrTq[MAC_TQ_SIZE];        /*!< Timer Task Queue */
   CmTimer     tmrBlk[MAX_NUM_TIMER];     /*!< Timer Block */
   MacCellCb   *macCell[MAX_NUM_CELL];
}MacCb;

/* global variable */
MacCb macCb;

/* Function declarations */
short int macActvTmr(Ent ent,Inst inst);
void fillRarPdu(RarInfo *rarInfo);
void createMacRaCb(RachIndInfo *rachIndInfo);
void fillMsg4DlData(MacDlData *dlData, uint16_t msg4PduLen, uint8_t *msg4Pdu);
void fillMacCe(MacCeInfo  *macCeData, uint8_t *msg3Pdu);
void macMuxPdu(MacDlData *dlData, MacCeInfo *macCeData, uint8_t *msg4TxPdu, uint16_t tbSize);
uint8_t unpackRxData(uint16_t cellId, SlotTimingInfo slotInfo, RxDataIndPdu *rxDataIndPdu);
void fillMg4Pdu(DlMsgAlloc *msg4Alloc);
void buildAndSendMuxPdu(SlotTimingInfo currTimingInfo);
uint8_t macProcUlCcchInd(uint16_t cellId, uint16_t crnti, uint16_t rrcContSize, uint8_t *rrcContainer);
uint8_t macProcShortBsr(uint16_t cellId, uint16_t crnti, uint8_t lcgId, uint32_t bufferSize);
uint8_t macProcUlData(uint16_t cellId, uint16_t rnti, SlotTimingInfo slotInfo, \
   uint8_t lcId, uint16_t pduLen, uint8_t *pdu);
uint8_t sendSchedRptToRlc(DlSchedInfo dlInfo, SlotTimingInfo slotInfo, uint8_t ueIdx, uint8_t schInfoIdx);
uint8_t macProcLongBsr(uint16_t cellId, uint16_t crnti,uint8_t numLcg,\
                         DataVolInfo dataVolInfo[MAX_NUM_LOGICAL_CHANNEL_GROUPS]);
void freeMacSliceCfgReq(MacSliceCfgReq *cfgReq,Pst *pst);
#endif
/**********************************************************************
  End of file
 **********************************************************************/
