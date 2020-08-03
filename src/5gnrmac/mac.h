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
#include "rgu.h"           /* RGU interface includes*/
#include "tfu.h"           /* TFU Interface defines */
#include "lrg.h"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "rg.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_lte.x"        /* Common LTE Defines */
#include "cm_mblk.x"        /* Common LTE Defines */
#include "rgu.x"           
#include "tfu.x"           /* RGU Interface includes */
#include "lrg.x"

#define MAX_SLOT_SUPPORTED    10 /* numerology 0 15Khz */
#define MAX_ZERO_CORR_CFG_IDX 16 /* max zero correlation config index */
#define SI_RNTI 0xFFFF
#define P_RNTI 0xFFFE
#define MAX_MAC_CE 6
#define MAX_UE 1
#define MAX_CRI_SIZE 6
#define MAX_MAC_DL_PDU 10
#define MAX_NUM_HARQ_PROC  16

#define PERIODIC_BSR_TMR_1MS    1
#define PERIODIC_BSR_TMR_5MS    5
#define PERIODIC_BSR_TMR_10MS   10
#define PERIODIC_BSR_TMR_16MS   16
#define PERIODIC_BSR_TMR_20MS   20
#define PERIODIC_BSR_TMR_32MS   32
#define PERIODIC_BSR_TMR_40MS   40
#define PERIODIC_BSR_TMR_60MS   60
#define PERIODIC_BSR_TMR_80MS   80
#define PERIODIC_BSR_TMR_128MS   128
#define PERIODIC_BSR_TMR_160MS   160
#define PERIODIC_BSR_TMR_320MS   320
#define PERIODIC_BSR_TMR_640MS   640
#define PERIODIC_BSR_TMR_1280MS  1280
#define PERIODIC_BSR_TMR_2560MS  2560

#define RETX_BSR_TMR_10MS    10
#define RETX_BSR_TMR_20MS    20
#define RETX_BSR_TMR_40MS    40
#define RETX_BSR_TMR_80MS    80
#define RETX_BSR_TMR_160MS   160
#define RETX_BSR_TMR_320MS   320
#define RETX_BSR_TMR_640MS   640
#define RETX_BSR_TMR_1280MS   1280
#define RETX_BSR_TMR_2560MS   2560
#define RETX_BSR_TMR_5120MS   5120
#define RETX_BSR_TMR_10240MS  10240

#define SR_DELAY_TMR_20MS     20
#define SR_DELAY_TMR_40MS     40
#define SR_DELAY_TMR_64MS     64
#define SR_DELAY_TMR_128MS    128
#define SR_DELAY_TMR_512MS    512
#define SR_DELAY_TMR_1024MS   1024
#define SR_DELAY_TMR_2560MS   2560

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

typedef struct macCellCb MacCellCb;

typedef enum
{
   UE_STATE_INACTIVE,
   UE_STATE_ACTIVE
}UeState;

typedef enum
{
   LC_STATE_INACTIVE,
   LC_STATE_ACTIVE
}LcState;

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
   uint8_t   lcId;      /* Logical Channel Id */
   uint8_t   lcGrpId;   /* Logical Channel group */
   LcState   lcActive;  /* Is LC active ? */
}UlLcCb;

/* Downlink dedicated logical channel info */
typedef struct dlLcCb
{
   uint8_t   lcId;      /* Logical channel Id */ 
   LcState   lcState;  /* Is LC active ? */
}DlLcCb;

/* BSR Information */
typedef struct bsrTmrCfg
{
   uint16_t   periodicTimer;
   uint16_t   retxTimer;
   uint16_t   srDelayTimer;
}BsrTmrCfg;

/* UE specific UL info */
typedef struct ueUlCb
{
   uint8_t    maxReTx;     /* MAX HARQ retransmission */
   uint8_t    numUlLc;     /* Number of uplink logical channels */       
   UlLcCb     lcCb[MAX_NUM_LOGICAL_CHANNELS];    /* Uplink dedicated logocal channels */
}UeUlCb;

/* UE specific DL Info */
typedef struct ueDlCb
{
   DlHarqEnt  dlHarqEnt;      /* DL HARQ entity */
   uint8_t    numDlLc;        /* Number of downlink logical channels */
   DlLcCb     lcCb[MAX_NUM_LOGICAL_CHANNELS];  /* Downlink dedicated logical channels */
}UeDlCb;

/* UE Cb */
typedef struct macUeCb
{
   uint16_t     ueIdx;    /* UE Idx assigned by DU APP */
   uint16_t     crnti;    /* UE CRNTI */
   MacCellCb    *cellCb;  /* Pointer to cellCb to whihc this UE belongs */
   UeState      state;    /* Is UE active ? */
   MacRaCbInfo  *raCb;    /* RA info */
   BsrTmrCfg    bsrTmrCfg;  /* BSR Timer Info */
   UeUlCb       ulInfo;   /* UE specific UL info */
   UeDlCb       dlInfo;   /* UE specific DL info */
}MacUeCb;

struct macCellCb
{
   uint16_t    cellId;
   MacRaCbInfo macRaCb[MAX_UE];
   MacDlSlot   dlSlot[MAX_SLOT_SUPPORTED];
   MacUlSlot   ulSlot[MAX_SLOT_SUPPORTED];
   uint16_t    numActvUe;
   MacUeCb     ueCb[MAX_UE];
};

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
void BuildAndSendMsg4MuxPdu(Msg4Alloc *msg4Alloc);

#endif
/**********************************************************************
  End of file
 **********************************************************************/
