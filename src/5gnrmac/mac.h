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
#define MAX_PENDING_STATS_RSP  5      /* Maximum number of statistics request for which response is pending */

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
}MacCellStatus;	   

typedef enum
{
   UE_STATE_INACTIVE,
   UE_STATE_ACTIVE,
   UE_HANDIN_IN_PROGRESS
}UeState;

typedef enum
{
   MAC_LC_STATE_INACTIVE,
   MAC_LC_STATE_ACTIVE
}MacLcState;

typedef struct dlTbInfo
{
   SlotTimingInfo  txTime;  
   uint16_t        tbSize;
   uint8_t         *tb;
}DlTbInfo;

/* DL HARQ Process Info */
typedef struct dlHarqProcCb
{
   uint8_t     procId;                 /* HARQ Process Id */
   uint8_t     numTb;                  /* Number of TB */
   DlTbInfo    tbInfo[MAX_NUM_TB_PER_UE];  /* TB information */
}DlHarqProcCb;

typedef struct macDlSlot
{
   DlSchedInfo dlInfo;
   DlPageAlloc *pageAllocInfo;
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
   DlHarqProcCb msg4HqInfo; /* HARQ process info for msg 4 */
   bool         *macMsg4Status;
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
   uint16_t         ueId;           /* UE Id from DU APP */
   uint16_t         crnti;          /* UE CRNTI */
   MacCellCb        *cellCb;        /* Pointer to cellCb to whihc this UE belongs */
   UeState          state;          /* Is UE active ? */
   MacCfraResource  cfraResource;   /* CF-RA resource */
   MacRaCbInfo      *raCb;          /* RA info */
   MacBsrTmrCfg     bsrTmrCfg;      /* BSR Timer Info */
   UeUlCb           ulInfo;         /* UE specific UL info */
   UeDlCb           dlInfo;         /* UE specific DL info */
   DataTransmissionAction transmissionAction;
}MacUeCb;

struct macCellCb
{
   uint16_t        cellId;
   uint16_t        numOfSlots;
   MacCellStatus   state;
   uint16_t        crntiMap;
   MacRaCbInfo     macRaCb[MAX_NUM_UE];
   MacDlSlot       dlSlot[MAX_SLOTS];
   MacUlSlot       ulSlot[MAX_SLOTS];
   uint16_t        numActvUe;
   MacUeCreateReq  *ueCfgTmpData[MAX_NUM_UE];
   MacUeRecfg      *ueRecfgTmpData[MAX_NUM_UE];
   MacUeCb         ueCb[MAX_NUM_UE];
   MacCellCfg      macCellCfg;
   uint8_t         numerology;
   SlotTimingInfo  currTime;
};

typedef struct macStatistics
{
   uint8_t      numPendingStatsRsp;
   MacStatsRsp  pendingStatsRsp[MAX_PENDING_STATS_RSP];

   /* This structure can be developed in future to add details of
    * the statistics to be calculated at MAC */
}MacStatistics;

typedef struct macCb
{
   Inst        macInst;
   ProcId      procId;
   uint8_t     tmrRes;                    /*!< Timer resolution */
   CmTqCp      tmrTqCp;                   /*!< Timer Task Queue Cntrl Point */
   CmTqType    tmrTq[MAC_TQ_SIZE];        /*!< Timer Task Queue */
   CmTimer     tmrBlk[MAX_NUM_TIMER];     /*!< Timer Block */
   MacCellCb   *macCell[MAX_NUM_CELL];
   MacStatistics  statistics;
}MacCb;

/* global variable */
MacCb macCb;

/* Function declarations */
uint8_t macActvInit ARGS((Ent entity, Inst inst, Region region, Reason reason));
uint8_t macActvTsk(Pst *pst, Buffer *mBuf);
short int macActvTmr(Ent ent,Inst inst);

void fillRarPdu(RarInfo *rarInfo);
void fillMsg4DlData(MacDlData *dlData, uint16_t msg4PduLen, uint8_t *msg4Pdu);
void fillMacCe(MacCeInfo  *macCeData, uint8_t *msg3Pdu);
void macMuxPdu(MacDlData *dlData, MacCeInfo *macCeData, uint8_t *msg4TxPdu, uint16_t tbSize);
uint8_t unpackRxData(uint16_t cellId, SlotTimingInfo slotInfo, RxDataIndPdu *rxDataIndPdu);
void fillMg4Pdu(DlMsgSchInfo *msg4Alloc);
void buildAndSendMuxPdu(SlotTimingInfo currTimingInfo);
uint8_t macProcUlCcchInd(uint16_t cellId, uint16_t crnti, uint16_t rrcContSize, uint8_t *rrcContainer);
uint8_t macProcShortBsr(uint16_t cellId, uint16_t crnti, uint8_t lcgId, uint32_t bufferSize);
uint8_t macProcUlData(uint16_t cellId, uint16_t rnti, SlotTimingInfo slotInfo, uint8_t lcId, uint16_t pduLen, uint8_t *pdu);
uint8_t sendSchedRptToRlc(DlSchedInfo dlInfo, SlotTimingInfo slotInfo, uint8_t ueIdx, uint8_t schInfoIdx);
uint8_t macProcLongBsr(uint16_t cellId, uint16_t crnti,uint8_t numLcg, DataVolInfo dataVolInfo[MAX_NUM_LOGICAL_CHANNEL_GROUPS]);
void freeMacSliceCfgReq(MacSliceCfgReq *cfgReq,Pst *pst);
void deleteMacRaCb(uint16_t cellIdx, MacUeCb *ueCb);

uint8_t MacProcSchSliceCfgRsp(Pst *pst, SchSliceCfgRsp *schSliceCfgRsp);
uint8_t MacProcSchSliceRecfgRsp(Pst *pst, SchSliceRecfgRsp *sliceRecfgrsp);
uint8_t MacProcSchCellCfgCfm(Pst *pst, SchCellCfgCfm  *schCellCfgCfm);
uint8_t MacProcDlAlloc(Pst *pst, DlSchedInfo *dlSchedInfo);
uint8_t MacProcUlSchInfo(Pst *pst, UlSchedInfo *ulSchedInfo);
uint8_t MacProcSchRachRsrcRsp(Pst *pst, SchRachRsrcRsp *schRachRsrcRsp);
uint8_t MacProcDlPageAlloc(Pst *pst, DlPageAlloc *dlPageAlloc);
uint8_t MacProcSchCellDeleteRsp(Pst *pst, SchCellDeleteRsp *schCellDeleteRsp);

uint8_t MacProcSchStatsRsp(Pst *pst, SchStatsRsp *schStatsRsp);
uint8_t MacProcSchStatsInd(Pst *pst, SchStatsInd *schStatsInd);
uint8_t MacProcSchStatsDeleteRsp(Pst *pst, SchStatsDeleteRsp *schStatsDeleteRsp);
uint8_t MacProcSchStatsModificationRsp(Pst *pst, SchStatsModificationRsp *schStatsModificationRsp);

#endif
/**********************************************************************
  End of file
 **********************************************************************/
