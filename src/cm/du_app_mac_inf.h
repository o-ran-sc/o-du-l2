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

/* Defines APIs exchanged between du_app and cl module of MAC */
#ifndef __MACINT_H__
#define __MACINT_H__

#define NUM_NUMEROLOGY 5  /* Number of numerology */
#define MAXIMUM_TDD_PERIODICITY 5
#define MAX_SYMB_PER_SLOT 14 

#define NUM_SSB		1	/* max value is 64 */
#define SSB_MASK_SIZE	1	/* SSB mask size is 32bit for sub6 */
#define SIB1_NEW_TX_PERIOD      160
#define SIB1_REPETITION_PERIOD   20
#define CORESET_0_INDEX      0
#define SEARCHSPACE_0_INDEX   0
#define SIB1_MCS  4


/* Event IDs */
#define EVENT_MAC_CELL_CONFIG_REQ    200
#define EVENT_MAC_CELL_CONFIG_CFM    201

typedef enum
{
   DUP_MODE_FDD,
   DUP_MODE_TDD
}DuplexMode;

typedef enum
{
   MAC_GEN_FULL_PBCH_PAYLD,   /* MAC generated the full PBCH Payload */
   PHY_GEN_TIMING_PBCH_BIT,   /* PHY generates the timing PBCH bits */
   PHY_GEN_FULL_PBCH_PAYLD    /* PHY generates full PBCH payload */
}BchPduOpt;

typedef enum
{
   LONG_SEQUENCE,
   SHORT_SEQUENCE
}PrachSeqLen;

typedef enum
{
   UNRESTRICTED,
   RESTRICTED_SET_TYPE_A,
   RESTRICTED_SET_TYPE_B
}RstSetCfg;

typedef enum
{
   DONT_REPORT_RSSI,
   RSSI_UNIT_DBM,
   RSSI_UNIT_DBFS
}RSSIMeasUnit;

typedef enum
{
   DL_SLOT,
   UL_SLOT,
   GUARD_SLOT
}SlotConfig;

typedef enum
{
   TX_PRDCTY_MS_0P5,
   TX_PRDCTY_MS_0P625,
   TX_PRDCTY_MS_1,
   TX_PRDCTY_MS_1P25,
   TX_PRDCTY_MS_2,
   TX_PRDCTY_MS_2P5,
   TX_PRDCTY_MS_5,
   TX_PRDCTY_MS_10
}DlUlTxPeriodicity;

typedef enum
{
   BETA_PSS_0DB,
   BETA_PSS_1DB
}BetaPss;

typedef enum 
{
   SSB_PRDCTY_MS_5,
   SSB_PRDCTY_MS_10,
   SSB_PRDCTY_MS_20,
   SSB_PRDCTY_MS_40,
   SSB_PRDCTY_MS_80,
   SSB_PRDCTY_MS_160
}SSBPeriod;

typedef struct carrierCfg
{
   Bool  pres;
   U16   bw;             /* DL/UL bandwidth */
   U32   freq;           /* Absolute frequency of DL/UL point A in KHz */
   U16   k0[NUM_NUMEROLOGY];          /* K0 for DL/UL */
   U16   gridSize[NUM_NUMEROLOGY];    /* DL/UL Grid size for each numerologies */
   U16   numAnt;         /* Number of Tx/Rx antennas */
}CarrierCfg;

typedef struct ssbCfg
{
   uint32_t    ssbPbchPwr;       /* SSB block power */
   BchPduOpt   bchPayloadFlag;   /* Options for generation of payload */
   uint8_t     scsCmn;           /* subcarrier spacing for common */
   uint16_t    ssbOffsetPointA;  /* SSB subcarrier offset from point A */
   BetaPss     betaPss;
   SSBPeriod   ssbPeriod;        /* SSB Periodicity in msec */
   uint8_t     ssbScOffset;       /* Subcarrier Offset */
   uint8_t     mibPdu[3];           /* MIB payload */
   uint32_t    ssbMask[SSB_MASK_SIZE];      /* Bitmap for actually transmitted SSB. */
   uint8_t     beamId[NUM_SSB];
   Bool        multCarrBand;     /* Multiple carriers in a band */
   Bool        multCellCarr;     /* Multiple cells in single carrier */
}SsbCfg;

typedef struct fdmInfo
{
   U16   rootSeqIdx;        /* Root sequence index */
   U8    numRootSeq;        /* Number of root sequences required for FD */
   U16   k1;                /* Frequency Offset for each FD */
   U8    zeroCorrZoneCfg;   /* Zero correlation zone cofig */
   U8    numUnusedRootSeq;  /* Number of unused root sequence */
   U8    *unsuedRootSeq;     /* Unused root sequence per FD */
}FdmInfo;

typedef struct prachCfg
{
   Bool          pres;
   PrachSeqLen   prachSeqLen;         /* RACH Sequence length: Long/short */
   U8            prachSubcSpacing;    /* Subcarrier spacing of RACH */
   RstSetCfg     prachRstSetCfg;      /* PRACH restricted set config */
   U8            prachFdm;            /* PRACH FDM (1,2,4,8) */
   FdmInfo       fdm[8];
   U8            ssbPerRach;          /* SSB per RACH occassion */
   Bool          prachMultCarrBand;    /* Multiple carriers in Band */
}PrachCfg;

typedef struct tddCfg
{
   Bool               pres;
   DlUlTxPeriodicity  tddPeriod;      /* DL UL Transmission periodicity */
   SlotConfig         slotCfg[MAXIMUM_TDD_PERIODICITY][MAX_SYMB_PER_SLOT]; 
}TDDCfg;

typedef struct sib1CellCfg
{
   uint8_t  *sib1Pdu;
   uint16_t sib1PduLen;
   uint16_t sib1NewTxPeriod;
   uint16_t sib1RepetitionPeriod;
   uint8_t coresetZeroIndex;     /* derived from 4 LSB of pdcchSib1 present in MIB */
   uint8_t searchSpaceZeroIndex; /* derived from 4 MSB of pdcchSib1 present in MIB */
   uint16_t sib1Mcs;
} Sib1CellCfg; 

typedef struct macCellCfg
{
   U16            transId;
   U16            cellId;     /* Cell Id */
   U8             numTlv;     /* Number of configuration TLVs */
   U8             carrierId;  /* Carrired Index */
   U16            phyCellId;  /* Physical cell id */
   DuplexMode     dupType;    /* Duplex type: TDD/FDD */
   CarrierCfg     dlCarrCfg;  /* DL Carrier configuration */
   CarrierCfg     ulCarrCfg;  /* UL Carrier configuration */
   Bool           freqShft;   /* Indicates presence of 7.5kHz frequency shift */
   SsbCfg         ssbCfg;     /* SSB configuration */          
   PrachCfg       prachCfg;   /* PRACH Configuration */
   TDDCfg         tddCfg;     /* TDD periodicity and slot configuration */
   RSSIMeasUnit   rssiUnit;   /* RSSI measurement unit */
   Sib1CellCfg    sib1Cfg;
}MacCellCfg;

typedef struct macCellCfgCfm
{
	uint8_t        rsp; 
   U16            transId;
}MacCellCfgCfm;

/* function pointers for packing macCellCfg Request */
typedef U16 (*packMacCellCfgReq) ARGS((
   Pst           *pst,
   MacCellCfg    *macCellCfg
));

typedef S16 (*packMacCellCfgConfirm) ARGS((
   Pst              *pst,
   MacCellCfgCfm    *macCellCfgCfm
));

typedef S16 (*DuMacCellCfgReq)     ARGS((
        Pst        *pst,               /* Post Structure */
        MacCellCfg *macCellCfg         /* Config Structure */
     ));

typedef S16 (*DuMacCellCfgCfm)     ARGS((
        MacCellCfgCfm *macCellCfgCfm         /* Config Structure */
     ));

U16 packMacCellCfg(Pst *pst, MacCellCfg *macCellCfg);

EXTERN int MacHdlCellCfgReq
(
 Pst           *pst,
 MacCellCfg    *macCellCfg
);

void cmUnpackLwLcMacCellCfg(
   DuMacCellCfgReq func,
   Pst *pst,
   Buffer *mBuf);

U16 unpackMacCellCfgCfm(
   DuMacCellCfgCfm func,
   Pst *pst,
   Buffer *mBuf);

EXTERN S16 duHandleMacCellCfgCfm
(
 MacCellCfgCfm    *macCellCfgCfm
);

#endif

/**********************************************************************
         End of file
**********************************************************************/
