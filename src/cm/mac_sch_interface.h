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

/* events */
#define EVENT_SCH_CELL_CFG 1
#define EVENT_SCH_CELL_CFG_CFM 2

/* selector */
#define MAC_SCH_LC_SELECTOR 0
#define MAC_SCH_TC_SELECTOR 1
#define MAC_SCH_LWLC_SELECTOR 2

/*macros*/
#define NO_SSB 0
#define SSB_TRANSMISSION 1
#define SSB_REPEAT 2
#define MAX_SSB_IDX 1 /* forcing it as 1 for now. Right value is 64 */

/*structures*/


typedef enum
{
	RSP_OK,
	RSP_NOK
}schMacRsp;

typedef struct schCellCfg
{
   U16         cellId;     /* Cell Id */
   U16         phyCellId;  /* Physical cell id */
	U8          bandwidth;  /* Supported B/W */
   DuplexMode  dupMode;    /* Duplex type: TDD/FDD */
   U32         ssbPbchPwr;       /* SSB block power */
   U8          scsCommon;           /* subcarrier spacing for common [0-3]*/
   U16         ssbOffsetPointA;  /* SSB sub carrier offset from point A */
   SSBPeriod   ssbPeriod;        /* SSB Periodicity in msec */
   U8          ssbSubcOffset;    /* Subcarrier Offset(Kssb) */
   U32         nSSBMask[SSB_MASK_SIZE];      /* Bitmap for actually transmitted SSB. */
}SchCellCfg;

typedef struct schCellCfgCfm
{
   U16         cellId;     /* Cell Id */
   schMacRsp   rsp;   
}SchCellCfgCfm;

typedef struct timeDomainAlloc
{
   uint16_t ssbStartSymbIdx;
	uint16_t ssbSymbolDuration;
}TimeDomainAlloc;

typedef struct freqDomainAlloc
{
   uint16_t ssbStartPrbIdx;
   uint16_t ssbPrbDuration;
}FreqDomainAlloc;

typedef struct ssbInfo
{
   uint8_t ssbIdx;          /* SSB Index */
	TimeDomainAlloc tdAlloc; /* Time domain allocation */
	FreqDomainAlloc fdAlloc; /* Freq domain allocation */
}SsbInfo;

/* Interface structure signifying DL broadcast allocation for SSB, SIB1 */
typedef struct dlBrdcstAlloc
{
   uint16_t cellId;  /* Cell Id */
	SlotIndInfo slotIndInfo; /* Slot Info: sfn, slot number */
	/* Ssb transmission is determined as follows:
	 * 0 : No tranamission
	 * 1 : SSB Transmission
	 * 2 : SSB Repetition */
	uint8_t ssbTrans;
	uint8_t ssbIdxSupported;
	SsbInfo ssbInfo[MAX_SSB_IDX];
	/* Sib1 transmission is determined as follows:
	 * 0 : No tranamission
	 * 1 : SIB1 Transmission
	 * 2 : SIB1 Repetition */
	U8 sib1Trans;
}DlBrdcstAlloc;

/* function pointers */

typedef int (*SchMacDlBrdcstAllocFunc)     ARGS((                     
   Pst            *pst,           /* Post Structure */                         
   DlBrdcstAlloc  *dlBrdcstAlloc    /* slot ind Info */                      
));

typedef int (*SchCellCfgCfmFunc)    ARGS((
   Pst            *pst,           /* Post Structure */                         
   SchCellCfgCfm  *schCellCfgCfm  /* Cell Cfg Cfm */
));

typedef int (*SchCellCfgFunc)    ARGS((
   Pst            *pst,           /* Post Structure */                         
   SchCellCfg  *schCellCfg     /* Cell Cfg  */
));
/* function declarations */
int packMacSchSlotInd(Pst *pst, SlotIndInfo *slotInd);
int packSchMacDlBrdcstAlloc(Pst *pst, DlBrdcstAlloc  *dlBrdcstAlloc);
EXTERN int packSchCellCfg(Pst *pst, SchCellCfg  *schCellCfg);
EXTERN int packSchCellCfgCfm(Pst *pst, SchCellCfgCfm  *schCellCfgCfm);

EXTERN int MacProcDlBrdcstAlloc(Pst *pst, DlBrdcstAlloc *dlBrdcstAlloc);
EXTERN int MacProcSchCellCfg(Pst *pst, SchCellCfg  *schCellCfg);
EXTERN int MacProcSchCellCfgCfm(Pst *pst, SchCellCfgCfm  *schCellCfgCfm);
EXTERN int SchHdlCellCfgReq(Pst *pst, SchCellCfg *schCellCfg);
EXTERN int schActvInit(Ent entity, Inst instId, Region region, Reason reason);

/**********************************************************************
  End of file
 **********************************************************************/

