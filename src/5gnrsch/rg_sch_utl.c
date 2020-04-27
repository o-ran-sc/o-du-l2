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

/************************************************************************

     Name:     LTE-MAC layer

     Type:     C source file

     Desc:     C source code for Entry point fucntions

     File:     rg_sch_utl.c

**********************************************************************/

/** @file rg_sch_utl.c
@brief This file implements the schedulers main access to MAC layer code.
*/

static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_MODULE_ID=4096;
static int RLOG_FILE_ID=177;

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_hash.h"       /* common hash list */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_err.h"
#include "rg_sch_inf.h"
#include "rg_sch.h"
#include "rg_sch_cmn.h"
#include "rgm.h"
#include "rl_interface.h"
#include "rl_common.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"       /* common tokens */
#include "tfu.x"           /* TFU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rgm.x"
#include "rg_sch_inf.x"         /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */
#include "rg_sch_cmn.x"        /* typedefs for Scheduler */
#ifdef EMTC_ENABLE
#include "rg_sch_emtc_ext.x"
#endif


/* SR_RACH_STATS */
U32 rgNumPrachRecvd =0;       /* Num of Rach Req received including dedicated preambles */
U32 rgNumRarSched =0;         /* Num of RARs sent */
U32 rgNumBI =0;               /* Num of BackOff Ind sent */
U32 rgNumMsg3CrcPassed =0;    /* Num of CRC success for Msg3 */
U32 rgNumMsg3CrcFailed =0;    /* Num of CRC fail for Msg 3 */
U32 rgNumMsg3FailMaxRetx =0;  /* Num of Msg3 fail after Max Retx attempts */
U32 rgNumMsg4Ack =0;          /* Num of Acks for Msg4 Tx */
U32 rgNumMsg4Nack =0; 
       /* Num of Nacks for Msg4 Tx */
U32 rgNumMsg4FailMaxRetx =0;  /* Num of Msg4 Tx failed after Max Retx attempts */
U32 rgNumSrRecvd =0;          /* Num of Sched Req received */
U32 rgNumSrGrant =0;          /* Num of Sched Req Grants sent */
U32 rgNumMsg3CrntiCE =0;      /* Num of Msg 3 CRNTI CE received */
U32 rgNumDedPream =0;         /* Num of Dedicated Preambles recvd */
U32 rgNumMsg3CCCHSdu =0;      /* Num of Msg 3 CCCH Sdus recvd */
U32 rgNumCCCHSduCrntiNotFound =0;  /*UE Ctx not found for CCCH SDU Msg 3 */
U32 rgNumCrntiCeCrntiNotFound =0;  /*UE Ctx not found for CRNTI CE Msg 3 */
U32 rgNumMsg4WithCCCHSdu =0;       /* Num of Msg4 with CCCH Sdu */
U32 rgNumMsg4WoCCCHSdu =0;         /* Num of Msg4 without CCCH Sdu */
U32 rgNumMsg4Dtx =0;               /* Num of DTX received for Msg 4 */
U32 rgNumMsg3AckSent =0;           /* Num of PHICH Ack sent for Msg 3 */
U32 rgNumMsg3NackSent =0;          /* Num of PHICH Nack sent for Msg 3 */
U32 rgNumMsg4PdcchWithCrnti =0;    /* Num of PDCCH for CRNTI based contention resolution */
U32 rgNumRarFailDuetoRntiExhaustion =0; /* Num of RACH Failures due to RNTI pool exhaution */
U32 rgNumTAModified =0;            /* Num of times TA received is different from prev value */
U32 rgNumTASent =0;               /* Num of TA Command sent */
U32 rgNumMsg4ToBeTx =0;           /* Num of times MSG4 that should be sent */
U32 rgNumMsg4Txed =0;             /* Num of MSG4 actually sent *//* ysNumMsg4ToBeTx -ysNumMsg4Txed == Failed MSG4 TX */
U32 rgNumMsg3DtxRcvd  =0;         /* CRC Fail with SINR < 0 */

U32 rgNumDedPreamUECtxtFound =0;         /* Num of Dedicated Preambles recvd */

PRIVATE U8 rgSchDciAmbigSizeTbl[61] = {0,0,0,0,0,0,0,0,0,0,0,
                         0,1,0,1,0,1,0,0,0,1,
                         0,0,0,1,0,1,0,0,0,0,
                         0,1,0,0,0,0,0,0,0,1,
                         0,0,0,1,0,0,0,0,0,0,
                         0,0,0,0,0,1,0,0,0,0};

/* local defines */

EXTERN U32 rgSchCmnBetaCqiOffstTbl[16];
EXTERN U32 rgSchCmnBetaRiOffstTbl[16]; 
EXTERN RgSchdApis rgSchCmnApis;
EXTERN PUBLIC S16 RgUiRgmSendPrbRprtInd ARGS((
Pst* pst, 
SuId suId, 
RgmPrbRprtInd *prbRprtInd
));

EXTERN PUBLIC S16 RgUiRgmSendTmModeChangeInd ARGS((
Pst* pst, 
SuId suId, 
RgmTransModeInd *txModeChngInd
));
#ifdef EMTC_ENABLE
EXTERN PUBLIC S16 rgSCHEmtcUtlGetSfAlloc ARGS((
RgSchCellCb *cell
));
EXTERN PUBLIC S16 rgSCHEmtcUtlPutSfAlloc ARGS((
RgSchCellCb *cell
));
EXTERN PUBLIC Void rgSCHEmtcUtlUpdUeDciSize ARGS((
RgSchCellCb *cell,
RgSchUeCb *ueCb
));
EXTERN PUBLIC Void rgSCHEmtcGetDciFrmt61ASize ARGS((
RgSchCellCb *cell
));
EXTERN PUBLIC Void rgSCHEmtcGetDciFrmt60ASize ARGS((
RgSchCellCb *cell
));
EXTERN PUBLIC S16 rgSCHEmtcUtlFillPdschDciInfo ARGS((
TfuPdschDciInfo *pdsch,
TfuDciInfo      *pdcchDci
));
EXTERN PUBLIC Void rgSCHEmtcUtlRlsRnti ARGS((
RgSchCellCb *cell,
RgSchRntiLnk   *rntiLnk,
U8             *isLegacy
));
EXTERN PUBLIC S16 rgSCHEmtcPdcchAlloc ARGS((
RgSchCellCb *cell,
RgSchPdcch  *pdcch
));
EXTERN PUBLIC Void rgSCHEmtcPdcchFree ARGS((
RgSchCellCb *cell,
RgSchPdcch  *pdcch
));
#endif
/* Functions specific to TM1/TM2/TM6/TM7 for PRB calculation*/
PUBLIC Void rgSchUtlDlCalc1CwPrb ARGS(( RgSchCellCb    *cell,
                                        RgSchUeCb      *ue,
                                        U32             bo,
                                        U32            *prbReqrd));

/* Functions specific to TM3/TM4 for PRB calculation*/
PUBLIC Void rgSchUtlDlCalc2CwPrb ARGS(( RgSchCellCb    *cell,
                                        RgSchUeCb      *ue,
                                        U32             bo,
                                        U32            *prbReqrd));

#ifdef LTE_ADV
PUBLIC RgSchCellCb* rgSchUtlGetCellCb ARGS(( Inst       inst,
                                             U16        cellId
));
#endif

typedef Void (*RgSchUtlDlCalcPrbFunc) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                                            U32 bo, U32 *prbRequrd));
#ifndef LTE_ADV
/* Functions specific to each transmission mode for PRB calculation*/
RgSchUtlDlCalcPrbFunc  dlCalcPrbFunc[7] = {rgSchUtlDlCalc1CwPrb,
rgSchUtlDlCalc1CwPrb, rgSchUtlDlCalc2CwPrb, rgSchUtlDlCalc2CwPrb,
NULLP, rgSchUtlDlCalc1CwPrb, rgSchUtlDlCalc1CwPrb};

#else
/* Functions specific to each transmission mode for PRB calculation*/
RgSchUtlDlCalcPrbFunc  dlCalcPrbFunc[9] = {rgSchUtlDlCalc1CwPrb,
rgSchUtlDlCalc1CwPrb, rgSchUtlDlCalc2CwPrb, rgSchUtlDlCalc2CwPrb,
NULLP, rgSchUtlDlCalc1CwPrb, rgSchUtlDlCalc1CwPrb, NULLP, NULLP};

#endif

#ifdef LTE_TDD
/* The below table will be used to map the UL SF number in a TDD Cfg 0 
   frame to the ul Sf array maintained in cellCb */
PRIVATE U8  rgSchTddCfg0UlSfTbl[] = {2, 3, 4, 7, 8, 9};
#endif

PRIVATE S16 rgSCHUtlUlAllocDbInit ARGS((
         RgSchCellCb    *cell,
         RgSchUlAllocDb *db,
         U8 maxAllocs
         ));
PRIVATE Void rgSCHUtlUlAllocDbDeinit ARGS((
         RgSchCellCb    *cell,
         RgSchUlAllocDb *db
         ));
PRIVATE S16 rgSCHUtlUlHoleDbInit ARGS((
         RgSchCellCb    *cell,
         RgSchUlHoleDb *db,
         U8 maxHoles,
         U8 start,
         U8 num
         ));
PRIVATE Void rgSCHUtlUlHoleDbDeinit ARGS((
         RgSchCellCb   *cell,
         RgSchUlHoleDb *db
         ));

PRIVATE S16 rgSCHChkBoUpdate ARGS((
         RgSchCellCb    *cell,
         RgInfCmnBoRpt  *boUpdt
         ));
#ifdef UNUSE_FUN
#ifdef TFU_UPGRADE
PRIVATE U8 rgSCHUtlFetchPcqiBitSz ARGS((
 RgSchCellCb    *cell,
 RgSchUeCb    *ueCb, 
 U8           numTxAnt
 ));
#endif
#endif
/* sorted in ascending order of tbSz */
CONSTANT struct rgSchUtlBcchPcchTbSz
{
   U8   rbIndex;    /* RB index {2,3} */
   U16  tbSz;       /* one of the Transport block size in bits of
                     * rbIndex 2 or 3 */
   /* Corrected allocation for common channels */
   U8   mcs;        /* imcs */
} rgSchUtlBcchPcchTbSzTbl[44] = {
   { 2,   32,  0 }, { 2,   56,  1 }, { 2,   72,  2 }, { 3,   88,  1 },
   { 2,  104,  3 }, { 2,  120,  4 }, { 2,  144,  5 }, { 2,  176,  6 },
   { 3,  208,  4 }, { 2,  224,  7 }, { 2,  256,  8 }, { 2,  296,  9 },
   { 2,  328, 10 }, { 2,  376, 11 }, { 3,  392,  8 }, { 2,  440, 12 },
   { 3,  456,  9 }, { 2,  488, 13 }, { 3,  504, 10 }, { 2,  552, 14 },
   { 3,  584, 11 }, { 2,  600, 15 }, { 2,  632, 16 }, { 3,  680, 12 },
   { 2,  696, 17 }, { 3,  744, 13 }, { 2,  776, 18 }, { 2,  840, 19 },
   { 2,  904, 20 }, { 3,  968, 16 }, { 2, 1000, 21 }, { 2, 1064, 22 },
   { 2, 1128, 23 }, { 3, 1160, 18 }, { 2, 1192, 24 }, { 2, 1256, 25 },
   { 3, 1288, 19 }, { 3, 1384, 20 }, { 2, 1480, 26 }, { 3, 1608, 22 },
   { 3, 1736, 23 }, { 3, 1800, 24 }, { 3, 1864, 25 }, { 3, 2216, 26 }
};

/* local typedefs */

/* local externs */

/* forward references */
#ifdef LTE_TDD
PRIVATE Void rgSCHUtlUpdPrachOcc ARGS((
RgSchCellCb *cell,
RgrTddPrachInfo *cellCfg));
#endif

#define RGSCH_NUM_PCFICH_REG 4
#define RGSCH_NUM_REG_PER_CCE 9
#define RGSCH_NUM_REG_PER_PHICH_GRP 3

#ifdef LTE_TDD
#define RGSCH_INITPHICH(_phich, _hqFeedBack, _nDmrs, _rbStart, _iPhich) {\
   (_phich)->hqFeedBack = _hqFeedBack; \
   (_phich)->rbStart = _rbStart; \
   (_phich)->nDmrs = _nDmrs; \
   (_phich)->iPhich = _iPhich; \
   (_phich)->lnk.next = NULLP; \
   (_phich)->lnk.prev = NULLP; \
   (_phich)->lnk.node = (PTR)(_phich); \
}
#else
#define RGSCH_INITPHICH(_phich, _hqFeedBack, _nDmrs, _rbStart, _isForMsg3) {\
   (_phich)->hqFeedBack = _hqFeedBack; \
   (_phich)->rbStart = _rbStart; \
   (_phich)->nDmrs = _nDmrs; \
   (_phich)->isForMsg3 = _isForMsg3; \
   (_phich)->lnk.next = NULLP; \
   (_phich)->lnk.prev = NULLP; \
   (_phich)->lnk.node = (PTR)(_phich); \
}
#endif

#define RGSCH_PHICH_ALLOC(_inst,_dataPtr, _size, _ret) {\
   _ret = rgSCHUtlAllocSBuf(_inst, (Data **)&_dataPtr, _size); \
}

/* ccpu00117052 - MOD - Passing double pointer
for proper NULLP assignment*/
#define RGSCH_PHICH_FREE(_inst, _dataPtr, _size) {\
      rgSCHUtlFreeSBuf(_inst, (Data **)(&(_dataPtr)), _size); \
}

#ifdef TFU_UPGRADE
#define RGSCH_GETBIT(a, b)     ((((U8*)a)[(b)>>3] >> ((7-((b)&7)))) & 1)

/*
*
*       Fun:   rgSCHUtlPower
*
*       Desc:  This function finds of the Power of x raised to n
*
*       Ret:   value of x raised to n
*
*       Notes: None
*
*       File:  rg_sch_utl.c
*
*/
#ifdef ANSI
PUBLIC F64 rgSCHUtlPower
(
F64    x,
F64    n
)
#else
PUBLIC F64 rgSCHUtlPower(x, n)
F64    x;
F64    n;
#endif
{
 if( n==0 )
 {
   RETVALUE( 1 );
 }
 else if ( n>0 )
 {
   RETVALUE( x * rgSCHUtlPower( x, n-1 ) );
 }
 else
 {
   RETVALUE( (1/x) * rgSCHUtlPower( x, n+1 ) );
 }
} /* end of rgSCHUtlPower*/

/*
*
*       Fun:   rgSCHUtlParse
*
*       Desc:  This function parses bits x to y of an array and
*                  returns the integer value  out of it.
*
*       Ret:   integer value of z bits
*
*       Notes: None
*
*       File:  rg_sch_utl.c
*
*/
#ifdef ANSI
PUBLIC U32 rgSCHUtlParse
(
U8     *buff,
U8     startPos,
U8     endPos,
U8     buffSize
)
#else
PUBLIC U32 rgSCHUtlParse(buff, startPos, endPos, buffSize)
U8     *buff;
U8     startPos;
U8     endPos;
U8     buffSize;
#endif
{
 U8 pointToChar,pointToEnd, loop;
 U8 size =  endPos - startPos;
 F64 result = 0;
 TRC2(rgSCHUtlParse);
 pointToEnd = (startPos)%8;
   for ( loop=0; loop<size; loop++)
   {
      pointToChar = (((startPos)+loop)/8);
      if (RGSCH_GETBIT(buff+pointToChar,pointToEnd%8)==1)
      {
         result=result+(rgSCHUtlPower(2,(size-loop-1)));
      }
      pointToEnd++;
   }
   RETVALUE((U32)result);
} /* end of rgSCHUtlParse*/

/*
*
*       Fun:   rgSCHUtlFindDist
*
*       Desc:  This function calculates the iterations need to cover
*                  before the valid Index can be used for next possible Reception
*
*       Ret:   integer value of z bits
*
*       Notes: None
*
*       File:  rg_sch_utl.c
*
*/
#ifdef ANSI
PUBLIC U8 rgSCHUtlFindDist
(
U16    crntTime,
U16    tempIdx
)
#else
PUBLIC U8 rgSCHUtlFindDist(crntTime, tempIdx)
U16    crntTime;
U16    tempIdx;
#endif
{
 U8  dist =0;
 /* ccpu00137113- Distance is not estimated properly if the periodicity is 
  * equal to RG_SCH_PCQI_SRS_SR_TRINS_SIZE.
  */
 while(crntTime<=tempIdx)
 {
    crntTime += RG_SCH_PCQI_SRS_SR_TRINS_SIZE;
    dist+=1;
 }
 RETVALUE(dist-1);
} /* end of rgSCHUtlFindDist*/
#endif


/**
 * @brief This function checks availability of a PDCCH
 *
 * @details
 *
 *     Function: rgSCHUtlPdcchAvail
 *     Purpose:  This function checks if a particular PDCCH is in use.
 *               map field of PDCCH is used to track the CCEs arleady
 *               allocated. Each bit of map represents one CCE and the
 *               LSBit of first byte represents CCE 0.
 *               steps:
 *               1. Locate the set of bits that represent the PDCCH for
 *               the provided location.
 *               2.  If the value of the bits is non-zero one or many CCEs
 *               for the PDCCH are in use and hence the PDCCH is not available.
 *               3. If pdcch is available, assign it to [out]pdcch.
 *               4.  Set all of the bits to one. There is no check performed
 *               to see if the PDCCH is available.
 *
 *     Invoked by: scheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSchPdcchInfo*  pdcchInfo
 *  @param[in]  U8            loc
 *  @param[in]  U8            aggrLvl
 *  @param[out] RgSchPdcch**     pdcch
 *  @return  Boolean
 *         -#   TRUE  if available
 *         -#   FALSE otherwise
 *
 **/
#ifdef ANSI
PUBLIC Bool rgSCHUtlPdcchAvail
(
RgSchCellCb             *cell,
RgSchPdcchInfo          *pdcchInfo,
CmLteAggrLvl            aggrLvl,
RgSchPdcch              **pdcch
)
#else
PUBLIC Bool rgSCHUtlPdcchAvail(cell, pdcchInfo, aggrLvl, pdcch)
RgSchCellCb             *cell;
RgSchPdcchInfo          *pdcchInfo;
CmLteAggrLvl            aggrLvl;
RgSchPdcch              **pdcch;
#endif
{
   U8							*byte;
   U16						offset;
	U16                  initMask;
	U16                  currMask;
   Inst                 inst = cell->instIdx;
   S16                  ret;
	U16                  offsetStepMask;

   TRC2(rgSCHUtlPdcchAvail);

   /* V5G_213 : 10.1 */
	offset = 0;
	byte = &pdcchInfo->map[0];
	initMask = (0xffff >> (16 - aggrLvl));
	currMask = initMask;
	/* if N(symbol, xPDCCH) =2, then xPDCCH will be candidates in 
	  * search space of index {0,1,2,3} and {8,9,..14}
	  */
	if ((cell->cell5gtfCb.cfi == 2) && (aggrLvl == CM_LTE_AGGR_LVL2))
	{
		offsetStepMask = 0xc;
	}
	else
	{
		offsetStepMask = 0xc0;
	}

	/* Loop till the number of bytes available in the CCE map */
	while (offset < ((pdcchInfo->nCce+ 7) >> 3))
	{
	   byte = &pdcchInfo->map[offset];
		/* Checking for available CCE */
		if ((*byte & currMask) == 0)
	   {
	      break;
	   }
		/* if the number of CCEs required are not available, move to next offset */
		if (currMask & offsetStepMask)
		{
			offset++;
			currMask = initMask;
		}
		else
		{
		   /* Move to the next available CCE index in the current byte(cce map) */
			currMask = currMask << aggrLvl;
		}
	}

	if ((offset >= ((pdcchInfo->nCce + 7) >> 3)) || 
		  ((aggrLvl == CM_LTE_AGGR_LVL16) && (offset > 0)))
	{
		RETVALUE(FALSE);
	}

	byte = &pdcchInfo->map[offset];
   
   if (cell->pdcchLst.first != NULLP)
   {
      *pdcch = (RgSchPdcch *)(cell->pdcchLst.first->node);
      cmLListDelFrm(&cell->pdcchLst, cell->pdcchLst.first);
   }
   else
   {
      ret = rgSCHUtlAllocSBuf(inst, (Data **)pdcch, sizeof(RgSchPdcch));
      if(ROK != ret)
      {
         RETVALUE(FALSE);
      }
   }

   if (*pdcch)
   {
      (*byte) |= currMask;
		/* ALL CCEs will be used in case of level 16 */
		if (aggrLvl == CM_LTE_AGGR_LVL16)
		{
			*(byte+1) |= currMask;
		}
      (*pdcch)->aggrLvl = aggrLvl;
      cmLListAdd2Tail(&pdcchInfo->pdcchs, &((*pdcch)->lnk));
      (*pdcch)->lnk.node = (PTR)*pdcch;
      (*pdcch)->nCce = aggrLvl;
      (*pdcch)->ue = NULLP;
   }
   RETVALUE(TRUE);
}



/**
 * @brief This function releases a PDCCH
 *
 * @details
 *
 *     Function: rgSCHUtlPdcchPut
 *     Purpose:  This function releases a PDCCH.
 *               steps:
 *               1. Locate the set of bits that represent the PDCCH for
 *               the provided location.
 *               2. Set all of the bits to zero.
 *               3. Release the memory of PDCCH to the cell free Q
 *
 *     Invoked by: scheduler
 *
 *  @param[in]  RgSchPdcchInfo*  pdcchInfo
 *  @param[in]  U8            loc
 *  @param[in]  U8            aggrLvl
 *  @return     Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlPdcchPut
(
RgSchCellCb                *cell,
RgSchPdcchInfo             *pdcchInfo,
RgSchPdcch                 *pdcch
)
#else
PUBLIC Void rgSCHUtlPdcchPut(cell, pdcchInfo, pdcch)
RgSchCellCb                *cell;
RgSchPdcchInfo             *pdcchInfo;
RgSchPdcch                 *pdcch;
#endif
{
   U8                   *byte;
   U8                   offset;
   uint16_t             mask;

   TRC2(rgSCHUtlPdcchPut);

   switch(pdcch->aggrLvl)
   {
      case CM_LTE_AGGR_LVL2:
         offset = (pdcch->nCce >> 1) & 3;
         mask = 0x3 << (offset * 2); /*ccpu00128826 - Offset Correction */
         break;
      case CM_LTE_AGGR_LVL4:
         offset = (pdcch->nCce >> 2) & 1;
         mask = 0xf << (offset * 4);/*ccpu00128826 - Offset Correction */
         break;
      case CM_LTE_AGGR_LVL8:
         mask = 0xff;
         break;
		case CM_LTE_AGGR_LVL16:
         mask = 0xffff;
         break;
      default:
         RETVOID;
   }
   /* Placing common computation of byte from all the cases above here
      for optimization */
   byte = &pdcchInfo->map[pdcch->nCce >> 3];

   cmLListDelFrm(&pdcchInfo->pdcchs, &pdcch->lnk);
   cmLListAdd2Tail(&cell->pdcchLst, &pdcch->lnk);
   pdcch->lnk.node = (PTR)pdcch;
   pdcch->ue = NULLP;
   (*byte) &= ~mask;

  RETVOID;
}


/**
 * @brief This function initializes PDCCH information for frame
 *
 * @details
 *
 *     Function: rgSCHUtlPdcchInit
 *     Purpose:  This function initializes PDCCH information for
 *               a slot. It removes the list of PDCCHs allocated
 *               in the prior use of this slot structure.
 *
 *     Invoked by: rgSCHUtlSubFrmPut
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSubFrm*     subFrm
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlPdcchInit
(
RgSchCellCb             *cell,
RgSchDlSf               *subFrm,
U16                     nCce
)
#else
PUBLIC Void rgSCHUtlPdcchInit(cell, subFrm, nCce)
RgSchCellCb             *cell;
RgSchDlSf               *subFrm;
U16                     nCce;
#endif
{
   RgSchPdcchInfo       *pdcchInfo;
   RgSchPdcch           *pdcch;
   Inst                 inst = cell->instIdx;
   U8                   extraBits;
   U32                  cceMapSz;

   TRC2(rgSCHUtlPdcchInit);

   pdcchInfo = &subFrm->pdcchInfo;
   while(pdcchInfo->pdcchs.first != NULLP)
   {
      pdcch = (RgSchPdcch *)pdcchInfo->pdcchs.first->node;
      cmLListDelFrm(&pdcchInfo->pdcchs, pdcchInfo->pdcchs.first);
      cmLListAdd2Tail(&cell->pdcchLst, &pdcch->lnk);
      pdcch->ue = NULLP;
   }
   cmLListInit(&pdcchInfo->pdcchs);
   
#ifdef LTEMAC_SPS
   subFrm->relPdcch = NULLP;
#endif
   
   cceMapSz = ((pdcchInfo->nCce + 7) >> 3);

	/* The bitMap array size is the number of ceiling(CCEs/8) */
	/* If nCce received is not the same as the one stored in    
 	* pdcchInfo, free the pdcchInfo map                      */

   if(pdcchInfo->nCce != nCce)
   {
      if(pdcchInfo->nCce)
      {
      	rgSCHUtlFreeSBuf(inst, (Data **)(&(pdcchInfo->map)), cceMapSz);
      }
      pdcchInfo->nCce = nCce;
      cceMapSz = ((pdcchInfo->nCce + 7) >> 3);
      rgSCHUtlAllocSBuf(inst, (Data **)&pdcchInfo->map,  
      cceMapSz);
      if (pdcchInfo->map == NULLP)
      {
         /* Generate log error here */
         RETVOID;
      }    
   }

   cmMemset(subFrm->pdcchInfo.map, 0, cceMapSz);
   /* 	If nCce is not exactly same as the bitMap size(no of bits allocated
    	* to represent the Cce's, then mark the extra bits as unavailable
    	extra bits = (((pdcchInfo->nCce + 7) >> 3)*8) - pdcchInfo->nCce
    	The last byte of bit map = subFrm->pdcchInfo.map[((pdcchInfo->nCce + 7) >> 3) - 1]
   	 NOTE : extra bits are most significant of the last byte eg.  */
   extraBits = (cceMapSz)*8 - pdcchInfo->nCce;
   subFrm->pdcchInfo.map[cceMapSz - 1] |=
     ((1 << extraBits) - 1) << (8 - extraBits);
   RETVOID;
}

/* LTE_ADV_FLAG_REMOVED_START */
/**
 * @brief This function frees Pool
 * @details
 *
 *     Function: rgSchSFRTotalPoolFree
 *
 *     Invoked by: rgSchSFRTotalPoolInit
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSubFrm*     subFrm
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSchSFRTotalPoolFree
(
 RgSchSFRTotalPoolInfo  *sfrTotalPoolInfo,
 RgSchCellCb             *cell
 )
#else
PUBLIC Void rgSchSFRTotalPoolFree(sfrTotalPoolInfo, cell)
   RgSchSFRTotalPoolInfo  *sfrTotalPoolInfo;
   RgSchCellCb             *cell;
#endif
{
   CmLListCp   *l;
   CmLList     *n;
   TRC2(rgSchSFRTotalPoolFree);  

   /*Deinitialise if  these cc pools and ce pools are already existent*/
   l = &sfrTotalPoolInfo->ccPool;
   n = cmLListFirst(l);
   while (n != NULL)
   {
      /*REMOVING Cell Centred POOLS IF ANY*/
      n = cmLListDelFrm(l, n);

      /* Deallocate buffer */
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(n->node)), sizeof(RgSchSFRPoolInfo));

      /* Deallocate buffer */
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(n)), sizeof(CmLList));   
      n = cmLListNext(l);
   }

   /*REMOVING Cell Edged POOLS IF ANY*/
   l = &sfrTotalPoolInfo->cePool;
   n = cmLListFirst(l);
   while (n != NULL)
   {
      n = cmLListDelFrm(l, n);

      /* Deallocate buffer */
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(n->node)), sizeof(RgSchSFRPoolInfo));

      /* Deallocate buffer */
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(n)), sizeof(CmLList));   
      n = cmLListNext(l);
   } 

}

/**
 * @brief This function resets temporary variables in Pool
 * @details
 *
 *     Function: rgSchSFRResetPoolVariables
 *
 *     Invoked by: rgSCHSFRUtlTotalPoolInit
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSubFrm*     subFrm
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSchSFRTotalPoolInit
(
 RgSchCellCb             *cell,
 RgSchDlSf               *sf
 )
#else
PRIVATE Void rgSchSFRTotalPoolInit(cell, sf)
   RgSchCellCb             *cell;
   RgSchDlSf               *sf;
#endif
{      
   /*  Initialise the variables */
   RgSchSFRPoolInfo *sfrCCPool;
   RgSchSFRPoolInfo *sfrCEPool;
   CmLListCp   *l;
   CmLList     *n;
   CmLList *temp = NULLP;
   S16 ret = 0;

   TRC2(rgSchSFRTotalPoolInit);  

   rgSchSFRTotalPoolFree(&sf->sfrTotalPoolInfo, cell);  
   sf->sfrTotalPoolInfo.CCPool1BwAvlbl          = 0;
   sf->sfrTotalPoolInfo.CCPool2BwAvlbl          = 0;
   sf->sfrTotalPoolInfo.CEPoolBwAvlbl           = 0;
   sf->sfrTotalPoolInfo.CC1                     = FALSE;
   sf->sfrTotalPoolInfo.CC2                     = FALSE;
   /*Initialise the CE Pools*/
   cmLListInit (&(sf->sfrTotalPoolInfo.cePool));

   ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data **)&temp, sizeof(CmLList));
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
		      "CE Pool memory allocation FAILED for cell");       
      rgSchSFRTotalPoolFree(&sf->sfrTotalPoolInfo, cell);
      RETVALUE(RFAILED);
   }

   ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data **)&temp->node, sizeof(RgSchSFRPoolInfo));
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
		      "CE Pool memory allocation FAILED for cell ");       
      rgSchSFRTotalPoolFree(&sf->sfrTotalPoolInfo,cell);
      RETVALUE(RFAILED);
   }

   l = &sf->sfrTotalPoolInfo.cePool;
   cmLListAdd2Tail(l, temp);

   /*Initialise Bandwidth and startRB and endRB for each pool*/
   n = cmLListFirst(l);

   /* Initialise the CE Pools */
   sfrCEPool = (RgSchSFRPoolInfo*)n->node;

   sfrCEPool->poolstartRB             = cell->lteAdvCb.sfrCfg.cellEdgeRbRange.startRb;
   sfrCEPool->poolendRB               = cell->lteAdvCb.sfrCfg.cellEdgeRbRange.endRb;
   sfrCEPool->bw                      = sfrCEPool->poolendRB - sfrCEPool->poolstartRB + 1;
   sf->sfrTotalPoolInfo.CEPoolBwAvlbl = sfrCEPool->bw;

   sfrCEPool->bwAlloced               = 0;
   sfrCEPool->type2Start              = sfrCEPool->poolstartRB;
   sfrCEPool->type2End                = RGSCH_CEIL(sfrCEPool->poolstartRB, cell->rbgSize);   
   sfrCEPool->type0End                = ((sfrCEPool->poolendRB + 1) / cell->rbgSize) - 1;
   sfrCEPool->pwrHiCCRange.startRb    = 0;
   sfrCEPool->pwrHiCCRange.endRb      = 0;

   /*Initialise CC Pool*/
   cmLListInit (&(sf->sfrTotalPoolInfo.ccPool));

   /*Add memory and Update CCPool*/
   ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data **)&temp, sizeof(CmLList));
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
		      "CC Pool memory allocation FAILED for cell ");       
      rgSchSFRTotalPoolFree(&sf->sfrTotalPoolInfo,cell);
      RETVALUE(RFAILED);
   }

   ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data **)&temp->node, sizeof(RgSchSFRPoolInfo));
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
		      "CC Pool memory allocation FAILED for cell ");       
      rgSchSFRTotalPoolFree(&sf->sfrTotalPoolInfo,cell);
      RETVALUE(RFAILED);
   }

   l = &sf->sfrTotalPoolInfo.ccPool;
   cmLListAdd2Tail(l, temp);

   /*Initialise Bandwidth and startRB and endRB for each pool*/
   if(sfrCEPool->poolstartRB)
   {
      n = cmLListFirst(l);
      sfrCCPool = (RgSchSFRPoolInfo*)n->node;

      sfrCCPool->poolstartRB              = 0;
      sfrCCPool->poolendRB                = sfrCEPool->poolstartRB - 1;
      sfrCCPool->bw                       = sfrCCPool->poolendRB - sfrCCPool->poolstartRB + 1;
      sf->sfrTotalPoolInfo.CCPool1BwAvlbl = sfrCCPool->bw;
      sfrCCPool->bwAlloced                = 0;
      sfrCCPool->type2Start               = 0;
      sfrCCPool->type2End                 = 0;
      sfrCCPool->type0End                 = ((sfrCCPool->poolendRB + 1) / cell->rbgSize) - 1;
      sf->sfrTotalPoolInfo.CC1            = TRUE;
      sfrCCPool->pwrHiCCRange.startRb     = 0;
      sfrCCPool->pwrHiCCRange.endRb       = 0;
   }
   else
   {
      n = cmLListFirst(l);
      sfrCCPool = (RgSchSFRPoolInfo*)n->node;

      sfrCCPool->poolstartRB              = sfrCEPool->poolendRB + 1;
      sfrCCPool->poolendRB                = sf->bw - 1;
      sfrCCPool->bw                       = sfrCCPool->poolendRB - sfrCCPool->poolstartRB + 1;
      sf->sfrTotalPoolInfo.CCPool2BwAvlbl = sfrCCPool->bw;
      sfrCCPool->CCPool2Exists            = TRUE;
      sfrCCPool->bwAlloced                = 0;
      sfrCCPool->type2Start               = sfrCCPool->poolstartRB;
      sfrCCPool->type2End                 = RGSCH_CEIL(sfrCCPool->poolstartRB, cell->rbgSize);
      sfrCCPool->type0End                 = ((sfrCCPool->poolendRB + 1) / cell->rbgSize) - 1;
      sf->sfrTotalPoolInfo.CC2            = TRUE;
      sfrCEPool->adjCCPool                = sfrCCPool; /* SFR_FIX */
      sfrCCPool->pwrHiCCRange.startRb     = 0;
      sfrCCPool->pwrHiCCRange.endRb       = 0;
   }

   if((sfrCEPool->poolendRB != sf->bw - 1) && (!sfrCCPool->poolstartRB))
   {	
      /*Add memory and Update CCPool*/
      ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data **)&temp, sizeof(CmLList));
      if (ret != ROK)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
			 "CC Pool memory allocation FAILED for cell ");       
         rgSchSFRTotalPoolFree(&sf->sfrTotalPoolInfo,cell);
         RETVALUE(RFAILED);
      }

      ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data **)&temp->node, sizeof(RgSchSFRPoolInfo));
      if (ret != ROK)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
			 "CC Pool memory allocation FAILED for cell ");       
         rgSchSFRTotalPoolFree(&sf->sfrTotalPoolInfo,cell);
         RETVALUE(RFAILED);
      }

      cmLListAdd2Tail(l, temp);

      n = cmLListCrnt(l);
      sfrCCPool = (RgSchSFRPoolInfo*)n->node;

      sfrCCPool->poolstartRB              = sfrCEPool->poolendRB + 1;
      sfrCCPool->poolendRB                = sf->bw - 1;
      sfrCCPool->bw                       = sfrCCPool->poolendRB - sfrCCPool->poolstartRB + 1;
      sf->sfrTotalPoolInfo.CCPool2BwAvlbl = sfrCCPool->bw;
      sfrCCPool->CCPool2Exists            = TRUE;
      sfrCCPool->bwAlloced                = 0;
      sfrCCPool->type2Start               = sfrCCPool->poolstartRB;	 
      sfrCCPool->type2End                 = RGSCH_CEIL(sfrCCPool->poolstartRB, cell->rbgSize);
      sfrCCPool->type0End                 = ((sfrCCPool->poolendRB + 1) / cell->rbgSize) - 1;
      sf->sfrTotalPoolInfo.CC2            = TRUE;
      sfrCEPool->adjCCPool                = sfrCCPool; /* SFR_FIX */
      sfrCCPool->pwrHiCCRange.startRb     = 0;
      sfrCCPool->pwrHiCCRange.endRb       = 0;
   }   

   sf->sfrTotalPoolInfo.CCRetx            = FALSE;
   sf->sfrTotalPoolInfo.CERetx            = FALSE;

   sf->sfrTotalPoolInfo.ccBwFull          = FALSE;
   sf->sfrTotalPoolInfo.ceBwFull          = FALSE;
   sf->sfrTotalPoolInfo.isUeCellEdge      = FALSE;
   RETVALUE(ROK);
}
/**
 * @brief This function resets temporary variables in RNTP Prepration
 * @details
 *
 *     Function:   rgSchDSFRRntpInfoInit 
 *
 *     Invoked by: rgSCHSFRUtlTotalPoolInit
 *
 *  @param[in]  TknStrOSXL*     rntpPtr
 *  @param[in]  RgSubFrm*        subFrm
 *  @return  S16
 *
 **/
#ifdef ANSI
PUBLIC S16   rgSchDSFRRntpInfoInit 
(
 TknStrOSXL              *rntpPtr,
 RgSchCellCb             *cell,
 U16                      bw
 )
#else
PRIVATE Void rgSchDSFRRntpInfoInit(rntpPtr, cell, bw)
   TknStrOSXL              *rntpPtr;
   RgSchCellCb             *cell;
   U16                      bw;
#endif
{   
   Inst inst = cell->instIdx;
   U16 len;

   TRC2(rgSchDSFRRntpInfoInit);

   rntpPtr->pres = PRSNT_NODEF;

   len = (bw % 8 == 0) ? (bw/8) : (bw/8 + 1);

   rntpPtr->len  = len;   

   /* Allocate memory for "scheduled UE" Info */
   if((rgSCHUtlAllocSBuf(inst, (Data**)&(rntpPtr->val),
               (len * sizeof(U8)))) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Memory allocation FAILED for RNTP Alloc");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/**
 * @brief This function release RNTP pattern from slot and Cell 
 * @details
 *
 *     Function:   rgSchDSFRRntpInfoFree 
 *
 *     Invoked by: rgSCHSFRUtlTotalPoolInit
 *
 *  @param[in]  TknStrOSXL*     rntpPtr
 *  @param[in]  RgSubFrm*        subFrm
 *  @return  S16
 *
 **/
#ifdef ANSI
PUBLIC S16   rgSchDSFRRntpInfoFree 
(
 TknStrOSXL              *rntpPtr,
 RgSchCellCb             *cell,
 U16                      bw
 )
#else
PRIVATE Void rgSchDSFRRntpInfoFree(rntpPtr, cell, bw)
   TknStrOSXL              *rntpPtr;
   RgSchCellCb             *cell;
   U16                      bw;
#endif
{   
   Inst inst = cell->instIdx;
   U16 len;

   TRC2(rgSchDSFRRntpInfoFree);

   len = (bw % 8 == 0) ? (bw/8) : (bw/8 + 1);

   if(rntpPtr->pres == PRSNT_NODEF)
   {   
      rgSCHUtlFreeSBuf(inst, (Data **)(&(rntpPtr->val)),(len * sizeof(U8)));
      rntpPtr->pres = NOTPRSNT;
      rntpPtr->len  = 0;   
   }

   RETVALUE(ROK);
}

/**
 * @brief This function resets temporary variables in Pool
 * @details
 *
 *     Function: rgSchSFRResetPoolVariables
 *     Purpose:  Initialise the dynamic variables in each pool.
 *                    Reset bwAlloced, bwAssigned, type2End, type0End, type2Start
 *     Invoked by: rgSCHSFRUtlTotalPoolReset
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSchSFRPoolInfo *pool
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSchSFRResetPoolVariables
(
 RgSchCellCb             *cell,
 RgSchSFRPoolInfo        *pool
 )
#else
PRIVATE Void rgSchSFRResetPoolVariables(cell, pool)
   RgSchCellCb             *cell;
   RgSchSFRPoolInfo *pool;
#endif
{

   TRC2(rgSchSFRResetPoolVariables);
   pool->bwAlloced  = 0;

   /*type0end will be the last RBG in pool with all available RBs*/
   pool->type0End = (((pool->poolendRB + 1)/cell->rbgSize) - 1);

   /*type2end will be the first RBG in pool with all available RBs*/
   pool->type2End = RGSCH_CEIL(pool->poolstartRB, cell->rbgSize);
   pool->type2Start = pool->poolstartRB;
   pool->bw = pool->poolendRB - pool->poolstartRB + 1;

      RETVOID;
}
/**
 * @brief This function resets SFR Pool information for frame
 *
 * @details
 *
 *     Function: rgSCHSFRUtlTotalPooReset
 *     Purpose:  Update the dynamic variables in each pool as they will be modified in each slot.
 *                    Dont modify the static variables like startRB, endRB, BW
 *     Invoked by: rgSCHUtlSubFrmPut
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSchDlSf*     subFrm
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHSFRUtlTotalPoolReset
(
 RgSchCellCb             *cell,
 RgSchDlSf               *subFrm
 )
#else
PRIVATE Void rgSCHSFRUtlTotalPoolReset(cell, subFrm)
   RgSchCellCb             *cell;
   RgSchDlSf               *subFrm;
#endif
{
   RgSchSFRTotalPoolInfo *totalPoolInfo = &subFrm->sfrTotalPoolInfo;
   CmLListCp    *ccPool = &totalPoolInfo->ccPool;
   CmLListCp    *cePool = &totalPoolInfo->cePool;
   CmLList *node = NULLP;
   RgSchSFRPoolInfo *tempPool = NULLP;

   TRC2(rgSCHSFRUtlTotalPoolReset);

   totalPoolInfo->ccBwFull          = FALSE;
   totalPoolInfo->ceBwFull          = FALSE;
   totalPoolInfo->isUeCellEdge      = FALSE;
   totalPoolInfo->CCPool1BwAvlbl    = 0;
   totalPoolInfo->CCPool2BwAvlbl    = 0;
   totalPoolInfo->CEPoolBwAvlbl     = 0;
   totalPoolInfo->CCRetx            = FALSE;
   totalPoolInfo->CERetx            = FALSE;

   node = ccPool->first;
   while(node)
   {
      tempPool = (RgSchSFRPoolInfo *)(node->node);
      node = node->next;
      rgSchSFRResetPoolVariables(cell, tempPool);
      if(tempPool->poolstartRB == 0)
         totalPoolInfo->CCPool1BwAvlbl = tempPool->bw;
      else
         totalPoolInfo->CCPool2BwAvlbl = tempPool->bw;
   }

   node = cePool->first;
   while(node)
   {
      tempPool = (RgSchSFRPoolInfo *)(node->node);
      node = node->next;
      rgSchSFRResetPoolVariables(cell, tempPool);
      totalPoolInfo->CEPoolBwAvlbl = tempPool->bw;    
   }

   RETVOID;
}
/* LTE_ADV_FLAG_REMOVED_END */
/**
 * @brief This function appends PHICH information for frame
 *
 * @details
 *
 *     Function: rgSCHUtlAddPhich
 *     Purpose:  This function appends PHICH information for
 *               a slot.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSubFrm*     subFrm
 *  @param[in]  U8            hqFeedBack
 *  @param[in]  U8            nDmrs
 *  @param[in]  U8            rbStart
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef LTE_TDD
#ifdef ANSI
PUBLIC S16 rgSCHUtlAddPhich
(
RgSchCellCb             *cell,
CmLteTimingInfo         frm,
U8                      hqFeedBack,
U8                      nDmrs,
U8                      rbStart,
U8                      iPhich
)
#else
PUBLIC S16 rgSCHUtlAddPhich(cell, frm, hqFeedBack, nDmrs, rbStart, iPhich)
RgSchCellCb             *cell;
CmLteTimingInfo         frm;
U8                      hqFeedBack;
U8                      nDmrs;
U8                      rbStart;
U8                      iPhich;
#endif
#else
#ifdef ANSI
PUBLIC S16 rgSCHUtlAddPhich
(
RgSchCellCb             *cell,
CmLteTimingInfo         frm,
U8                      hqFeedBack,
U8                      nDmrs,
U8                      rbStart,
Bool                    isForMsg3
)
#else
PUBLIC S16 rgSCHUtlAddPhich(cell, frm, hqFeedBack, nDmrs, rbStart, isForMsg3)
RgSchCellCb             *cell;
CmLteTimingInfo         frm;
U8                      hqFeedBack;
U8                      nDmrs;
U8                      rbStart;
Bool                    isForMsg3;
#endif
#endif
{
   S16                ret;
   RgSchPhich         *phich;
   RgSchDlSf          *dlSf;
   Inst               inst = cell->instIdx;
   TRC2(rgSCHUtlAddPhich);

   dlSf = rgSCHUtlSubFrmGet(cell, frm);
   RGSCH_PHICH_ALLOC(inst, phich,sizeof(RgSchPhich), ret);

   if(ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId, " rgSCHUtlAddPhich(): "
               "Allocation of RgSchPhich failed");
      RETVALUE(RFAILED);
   }
#ifdef LTE_TDD
   RGSCH_INITPHICH(phich, hqFeedBack, nDmrs, rbStart, iPhich);
#else
   RGSCH_INITPHICH(phich, hqFeedBack, nDmrs, rbStart, isForMsg3); /*SR_RACH_STATS */
#endif
   cmLListAdd2Tail(&dlSf->phichInfo.phichs, &phich->lnk);
   RETVALUE(ROK);
} /* rgSCHUtlAddPhich */

/**
 * @brief This function resets PHICH information for frame
 *
 * @details
 *
 *     Function: rgSCHUtlPhichReset
 *     Purpose:  This function initializes PHICH information for
 *               a slot. It removes the list of PHICHs allocated
 *               in the prior use of this slot structure.
 *
 *     Invoked by: rgSCHUtlSubFrmPut
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSubFrm*     subFrm
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHUtlPhichReset
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm
)
#else
PRIVATE Void rgSCHUtlPhichReset(cell, subFrm)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
#endif
{
   RgSchPhichInfo          *phichInfo;
   RgSchPhich              *phich;

   UNUSED(cell);

   TRC2(rgSCHUtlPhichReset);

   phichInfo = &subFrm->phichInfo;
   while(phichInfo->phichs.first != NULLP)
   {
      phich = (RgSchPhich *)phichInfo->phichs.first->node;
      cmLListDelFrm(&phichInfo->phichs, phichInfo->phichs.first);
      RGSCH_PHICH_FREE(cell->instIdx, phich, sizeof(RgSchPhich));
   }
   cmLListInit(&phichInfo->phichs);
   RETVOID;
} /* rgSCHUtlPhichReset */


/**
 * @brief This function returns slot data structure for a cell
 *
 * @details
 *
 *     Function: rgSCHUtlSubFrmGet
 *     Purpose:  This function resets the slot data structure
 *               when the slot is released
 *
 *     Invoked by: scheduler
 *
 *  @param[in]  RgSubFrm  subFrm
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC RgSchDlSf* rgSCHUtlSubFrmGet
(
RgSchCellCb            *cell,
CmLteTimingInfo        frm
)
#else
PUBLIC RgSchDlSf* rgSCHUtlSubFrmGet(cell, frm)
RgSchCellCb            *cell;
CmLteTimingInfo        frm;
#endif
{
   RgSchDlSf            *sf;
   U8                   dlIdx;

   TRC2(rgSCHUtlSubFrmGet);

#ifdef LTE_TDD
   dlIdx = rgSCHUtlGetDlSfIdx(cell, &frm);
   //RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, cell->subFrms, dlIdx);
   sf = cell->subFrms[dlIdx];
#else
   /* Changing the idexing
      so that proper slot is selected */
   dlIdx = (((frm.sfn & 1) * RGSCH_NUM_SUB_FRAMES) + (frm.slot % RGSCH_NUM_SUB_FRAMES));
   RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, cell->subFrms, dlIdx);
   sf = cell->subFrms[dlIdx];
#endif
  /* CA dev Start */
   sf->dlIdx    = dlIdx;
  /* CA dev End */
   RETVALUE(sf);
}


/**
 * @brief This function returns slot data structure for a cell
 *
 * @details
 *
 *     Function: rgSCHUtlSubFrmPut
 *     Purpose:  This function resets the slot data structure
 *               when the slot is released
 *
 *     Invoked by: scheduler
 *
 *  @param[in]  RgSubFrm  subFrm
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlSubFrmPut
(
RgSchCellCb            *cell,
RgSchDlSf              *sf
)
#else
PUBLIC Void rgSCHUtlSubFrmPut(cell, sf)
RgSchCellCb            *cell;
RgSchDlSf              *sf;
#endif
{
   U8                  i;
   U8                  noRaRsps;

   TRC2(rgSCHUtlSubFrmPut);

#ifdef LTE_TDD
   /* Release all the held PDCCH information */
   rgSCHUtlPdcchInit(cell, sf, sf->nCce);
#else
   /* Release all the held PDCCH information */
   rgSCHUtlPdcchInit(cell, sf, cell->nCce);
#endif
   rgSCHUtlPhichReset(cell, sf);

   /* Reset the bw allocated. */
   sf->bwAssigned = 0;
#ifdef LTEMAC_SPS
   /* Setting allocated bandwidth to SPS bandwidth for non-SPS RB allocator */
   sf->bwAlloced = ((cell->spsCellCfg.maxSpsDlBw +
      cell->rbgSize - 1)/cell->rbgSize) * cell->rbgSize;
   if (sf->bwAlloced > sf->bw)
   {
      sf->bwAlloced = sf->bw;
   }
   sf->spsAllocdBw = 0;
   sf->type2Start = sf->bwAlloced;
   cmMemset((U8*) &sf->dlSfAllocInfo, 0, sizeof(RgSchDlSfAllocInfo));
#else
   sf->bwAlloced = 0;
   /* Fix for ccpu00123918*/
   sf->type2Start = 0;
   /* LTE_ADV_FLAG_REMOVED_START */
   /* dsfr_pal_fixes ** 21-March-2013 ** SKS */
   if (cell->lteAdvCb.dsfrCfg.status == RGR_ENABLE)
   {
      cmMemset((U8*) sf->rntpInfo.val, 0, sf->rntpInfo.len);
   }
   /* LTE_ADV_FLAG_REMOVED_END */
#endif
   sf->txDone = FALSE;
   /*[ccpu00138609]-ADD-Reset the CCCH UE counter */
   sf->schdCcchUe = 0;

   /* Non DLFS scheduling using Type0 RA requires the following
    * parameter's tracking */
   /* Type 2 localized allocations start from 0th RBG and onwards */
   /* Type 0 allocations start from last RBG and backwards*/
#ifndef LTEMAC_SPS
   sf->type2End   = 0;
#else
   sf->type2End   = RGSCH_CEIL(sf->bwAlloced,cell->rbgSize);
#endif
   sf->type0End   = cell->noOfRbgs - 1;
   /* If last RBG is of incomplete size then special handling */
   (sf->bw % cell->rbgSize == 0)? (sf->lstRbgDfct = 0) :
      (sf->lstRbgDfct = cell->rbgSize - (sf->bw % cell->rbgSize));
   /* This resets the allocation for BCCH and PDCCH */
#ifdef EMTC_ENABLE
   /* TODO we need to move this reset for emtc functions */
   if(!(cell->emtcEnable))
   {
      sf->bch.tb     = NULLP;
      sf->bch.tbSize = 0;
   }
#else
   sf->bch.tb     = NULLP;
   sf->bch.tbSize = 0;
#endif
   sf->bcch.pdcch = NULLP;
   sf->pcch.pdcch = NULLP;
#ifdef LTE_TDD
   noRaRsps = RGSCH_MAX_TDD_RA_RSP_ALLOC;
#else
   noRaRsps = RGSCH_MAX_RA_RSP_ALLOC;
#endif
   for (i = 0; i < noRaRsps; i++)
   {
      sf->raRsp[i].pdcch = NULLP;
      cmLListInit(&(sf->raRsp[i].raRspLst));
   }
   /* LTE_ADV_FLAG_REMOVED_START */
   if (cell->lteAdvCb.sfrCfg.status == RGR_ENABLE)
   {
      rgSCHSFRUtlTotalPoolReset(cell, sf);
   }
   /* LTE_ADV_FLAG_REMOVED_END */
#ifdef LTE_ADV
   cmLListInit(&sf->n1PucchResLst);
#endif

   sf->cceCnt = 0;
   sf->isCceFailure = FALSE;
   sf->dlUlBothCmplt = 0;
   RETVOID;
}


/**
 * @brief This function computes log N (32 bit Unsigned) to the base 2
 *
 * @details
 *
 *     Function: rgSCHUtlLog32bitNbase2
 *     Purpose:  This function computes log N (32 bit Unsigned) to the base 2.
 *               For n= 0,1 ret = 0.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  U32       n
 *  @return  U8
 *
 **/
#ifdef ANSI
PUBLIC U8 rgSCHUtlLog32bitNbase2
(
U32               n
)
#else
PUBLIC U8 rgSCHUtlLog32bitNbase2(n)
U32               n;
#endif
{
   U32            b[] = {0x2, 0xc, 0xf0, 0xff00, 0xffff0000};
   U32            s[] = {1, 2, 4, 8, 16};
   S16            i;
   U8             ret = 0;

   TRC2(rgSCHUtlLog32bitNbase2)

   for (i=4; i >= 0; i--)
   {
      if (n & b[i])
      {
         n >>= s[i];
         ret |= s[i];
      }
   }
   RETVALUE(ret);
}

#ifdef LTEMAC_SPS

/**
 * @brief This function is a wrapper to call scheduler specific API.
 *
 * @details
 *
 *     Function: rgSCHUtlDlRelPdcchFbk
 *     Purpose:  Calls scheduler's handler for SPS release PDCCH feedback
 *     information.
 *
 *     Invoked by: DHM
 *
 *  @param[in]   RgSchCellCb     *cell
 *  @param[in]   RgSchUeCb       *ue
 *  @param[in]   U8            isAck
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlDlRelPdcchFbk
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
U8               isAck
)
#else
PUBLIC Void rgSCHUtlDlRelPdcchFbk(cell, ue, isAck)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
U8               isAck;
#endif
{
   TRC2(rgSCHUtlDlRelPdcchFbk);
   cell->sc.apis->rgSCHDlRelPdcchFbk(cell, ue, isAck);
   RETVOID;
}



/**
 * @brief This function is a wrapper to call scheduler specific API.
 *
 * @details
 *
 *     Function: rgSCHUtlDlProcAck
 *     Purpose:  Calls scheduler's handler to process Ack
 *     information.
 *
 *     Invoked by: DHM
 *
 *  @param[in]   RgSchCellCb     *cell
 *  @param[in]   RgSchDlHqProcCb *hqP
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlDlProcAck
(
RgSchCellCb        *cell,
RgSchDlHqProcCb    *hqP
)
#else
PUBLIC Void rgSCHUtlDlProcAck(cell, hqP)
RgSchCellCb        *cell;
RgSchDlHqProcCb    *hqP;
#endif
{
   TRC2(rgSCHUtlDlProcAck);
   cell->sc.apis->rgSCHDlProcAck(cell, hqP);
   RETVOID;
}

/**
 * @brief CRNTI CE Handler
 *
 * @details
 *
 *     Function : rgSCHUtlHdlCrntiCE
 *
 *     - Call scheduler common API
 *
 *
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgSchUeCb     *ue
 *  @param[out] RgSchErrInfo  *err
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlHdlCrntiCE
(
RgSchCellCb  *cell,
RgSchUeCb    *ue
)
#else
PUBLIC Void rgSCHUtlHdlCrntiCE(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
#endif
{
   TRC2(rgSCHUtlHdlCrntiCE);

   cell->sc.apis->rgSCHHdlCrntiCE(cell, ue);
   RETVOID;
}  /* rgSCHUtlHdlCrntiCE */
#endif /* LTEMAC_SPS */

/***********************************************************
 *
 *     Func : rgSCHUtlCalcTotalRegs
 *
 *     Desc : Calculate total REGs, given a bandwidth, CFI
 *            and number of antennas.
 *
 *     Ret  : Total REGs (U16)
 *
 *     Notes: Could optimise if bw values are limited
 *            (taken from RRC spec) by indexing values from
 *            a table.
 *            Input values are not validated. CFI is assumed
 *            to be 1/2/3/4.
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U16 rgSCHUtlCalcTotalRegs
(
U8    bw,
U8    cfi,
U8    numAntna,
Bool  isEcp
)
#else
PRIVATE U16 rgSCHUtlCalcTotalRegs(bw, cfi, numAntna, isEcp)
U8    bw;
U8    cfi;
U8    numAntna;
Bool  isEcp;
#endif
{
   U16 regs = 0;
   TRC2(rgSCHUtlCalcTotalRegs);

   /*ccpu00116757-  removed check for (ERRCLASS & ERRCLS_DEBUG)*/

   if (bw <= 10)
      ++cfi;
   switch (cfi)
   {
      /* Refer 36.211 section 6.10.1.2
       * For symbols 2 and 4, the REGs per RB will be based on cyclic prefix
       *  and number of antenna ports.
       * For symbol 1, there are 2 REGs per RB always. Similarly symbol 3
       * will have 3 REGS.
       */
      case 4:
         /*CR changes [ccpu00124416] - MOD*/
         if(isEcp)
         {
            regs =  bw * RGSCH_NUM_REGS_4TH_SYM_EXT_CP;
         }
         else
         {
            regs =  bw * RGSCH_NUM_REGS_4TH_SYM_NOR_CP;
         }
      case 3:
         regs += bw * RGSCH_NUM_REGS_3RD_SYM;
      case 2:
            /*CR changes [ccpu00124416] - MOD using number of antenna ports*/
         regs += (numAntna == RGSCH_NUM_ANT_PORT_FOUR) ? \
            (bw * RGSCH_NUM_REGS_2ND_SYM_FOUR_ANT_PORT) : \
            (bw * RGSCH_NUM_REGS_2ND_SYM_1OR2_ANT_PORT);
      default: /* case 1 */
         regs += bw * RGSCH_NUM_REGS_1ST_SYM;
   }
   RETVALUE(regs);
}

/***********************************************************
 *
 *     Func : rgSCHUtlCalcPhichRegs
 *
 *     Desc : Calculates number of PHICH REGs
 *
 *     Ret  : Number of PHICH REGs (U8)
 *
 *     Notes: ng6 is Ng multiplied by 6
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U16 rgSCHUtlCalcPhichRegs
(
U8 bw,
U8 ng6
)
#else
PRIVATE U16 rgSCHUtlCalcPhichRegs(bw, ng6)
U8 bw;
U8 ng6;
#endif
{
   TRC2(rgSCHUtlCalcPhichRegs);
   /* ccpu00115330: Corrected the calculation for number of PHICH groups*/
   RETVALUE(RGSCH_CEIL((bw * ng6) ,(8 * 6)) * RGSCH_NUM_REG_PER_PHICH_GRP);
}

#ifdef LTE_TDD
/**
 * @brief Calculates total CCEs (N_cce)
 *
 * @details
 *
 *     Function: rgSCHUtlCalcNCce
 *     Purpose:  This function calculates and returns total CCEs for a
 *               cell, given the following: bandwidth, Ng configuration
 *               (multiplied by six), cfi (actual number of control
 *               symbols), m factor for PHICH  and number of antennas.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  U8      bw
 *  @param[in]  U8      ng6
 *  @param[in]  U8      cfi
 *  @param[in]  U8      mPhich
 *  @param[in]  U8      numAntna
 *  @param[in]  Bool    isEcp
 *  @return     N_cce (U8)
 *
 **/
#ifdef ANSI
PUBLIC U8 rgSCHUtlCalcNCce
(
U8            bw,
RgrPhichNg    ng,
U8            cfi,
U8            mPhich,
U8            numAntna,
Bool          isEcp
)
#else
PUBLIC U8 rgSCHUtlCalcNCce(bw, ng, cfi, mPhich, numAntna, isEcp)
U8             bw;
RgrPhichNg     ng;
U8             cfi;
U8             mPhich;
U8             numAntna;
Bool           isEcp;
#endif
{
   U16         totalRegs;
   U16         phichRegs;
   U16         cceRegs;
   U8          ng6;
   TRC2(rgSCHUtlCalcNCce);

   /*ccpu00116757-  removed check for (ERRCLASS & ERRCLS_DEBUG)*/

   switch (ng)
   {
      case RGR_NG_ONESIXTH:
         ng6 = 1;
         break;
      case RGR_NG_HALF:
         ng6 = 3;
         break;
      case RGR_NG_ONE:
         ng6 = 6;
         break;
      case RGR_NG_TWO:
      default:
         ng6 = 12;
         break;
   }

   totalRegs = rgSCHUtlCalcTotalRegs(bw, cfi, numAntna, isEcp);
   phichRegs = rgSCHUtlCalcPhichRegs(bw, ng6);
   cceRegs   = totalRegs - mPhich*phichRegs - RGSCH_NUM_PCFICH_REG;

   RETVALUE((U8)(cceRegs/RGSCH_NUM_REG_PER_CCE));
}

#else
/**
 * @brief Calculates total CCEs (N_cce)
 *
 * @details
 *
 *     Function: rgSCHUtlCalcNCce
 *     Purpose:  This function calculates and returns total CCEs for a
 *               cell, given the following: bandwidth, Ng configuration
 *               (multiplied by six), cfi (actual number of control
 *               symbols) and number of antennas.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  U8      bw
 *  @param[in]  U8      ng6
 *  @param[in]  U8      cfi
 *  @param[in]  U8      numAntna
 *  @return     N_cce (U8)
 *
 **/
#ifdef ANSI
PUBLIC U8 rgSCHUtlCalcNCce
(
U8            bw,
RgrPhichNg    ng,
U8            cfi,
U8            numAntna,
Bool          isEcp
)
#else
PUBLIC U8 rgSCHUtlCalcNCce(bw, ng, cfi, numAntna, isEcp)
U8             bw;
RgrPhichNg     ng;
U8             cfi;
U8             numAntna;
Bool           isEcp;
#endif
{
   U16         totalRegs;
   U16         phichRegs;
   U16         cceRegs;
   U8          ng6;
   TRC2(rgSCHUtlCalcNCce);

   /*ccpu00116757-  removed check for (ERRCLASS & ERRCLS_DEBUG)*/

   switch (ng)
   {
      case RGR_NG_ONESIXTH:
         ng6 = 1;
         break;
      case RGR_NG_HALF:
         ng6 = 3;
         break;
      case RGR_NG_ONE:
         ng6 = 6;
         break;
      case RGR_NG_TWO:
      default:
         ng6 = 12;
         break;
   }

   totalRegs = rgSCHUtlCalcTotalRegs(bw, cfi, numAntna, isEcp);
   phichRegs = rgSCHUtlCalcPhichRegs(bw, ng6);
   cceRegs   = totalRegs - phichRegs - RGSCH_NUM_PCFICH_REG;

   RETVALUE((U8)(cceRegs/RGSCH_NUM_REG_PER_CCE));
}
#endif

/**
 * @brief Returns PHICH info associated with an uplink
 *        HARQ process allocation
 *
 * @details
 *
 *     Function: rgSCHUtlGetPhichInfo
 *     Purpose:  This function returns PHICH info associated with
 *               an uplink HARQ process allocation. PHICH info
 *               comprises RB start and N_dmrs.
 *
 *  @param[in]  RgSchUlHqProcCb   *hqProc
 *  @param[out] U8             *rbStartRef
 *  @param[out] U8             *nDmrsRef
 *  @return  S16
 **/
#ifdef LTE_TDD
#ifdef ANSI
PUBLIC S16 rgSCHUtlGetPhichInfo
(
RgSchUlHqProcCb   *hqProc,
U8                *rbStartRef,
U8                *nDmrsRef,
U8                *iPhich
)
#else
PUBLIC S16 rgSCHUtlGetPhichInfo(hqProc, rbStartRef, nDmrsRef, iPhich)
RgSchUlHqProcCb   *hqProc;
U8                *rbStartRef;
U8                *nDmrsRef;
U8                *iPhich;
#endif
#else
#ifdef ANSI
PUBLIC S16 rgSCHUtlGetPhichInfo
(
RgSchUlHqProcCb   *hqProc,
U8                *rbStartRef,
U8                *nDmrsRef
)
#else
PUBLIC S16 rgSCHUtlGetPhichInfo(hqProc, rbStartRef, nDmrsRef)
RgSchUlHqProcCb   *hqProc;
U8                *rbStartRef;
U8                *nDmrsRef;
#endif
#endif
{
/* ACC-TDD */
	S16 ret = RFAILED;

	TRC2(rgSCHUtlGetPhichInfo);

	if ((hqProc != NULLP) && (hqProc->alloc != NULLP))
    {
       *rbStartRef = hqProc->alloc->grnt.rbStart;
       *nDmrsRef   = hqProc->alloc->grnt.nDmrs;
#ifdef LTE_TDD
   *iPhich     = hqProc->iPhich;
#endif
       ret = ROK;
    }
    RETVALUE(ret);
}
#ifndef TFU_UPGRADE
/**
 * @brief Returns uplink grant information required to permit
 *        PHY to receive data
 *
 * @details
 *
 *     Function: rgSCHUtlAllocRcptInfo
 *     Purpose:  Given an uplink allocation, this function returns
 *               uplink grant information which is needed by PHY to
 *               decode data sent from UE. This information includes:
 *               - RB start
 *               - Number of RBs
 *               - RV
 *
 *  @param[in]  RgSchUlAlloc   *alloc
 *  @param[out] U8             *rbStartRef
 *  @param[out] U8             *numRbRef
 *  @param[out] U8             *rvRef
 *  @param[out] U16            *size
 *  @param[out] TfuModScheme   *modType
 *  @param[out] Bool           *isRtx
 *  @param[out] U8             *nDmrs
 *  @param[out] Bool           *ndi
 *  @param[out] U8             *hqPId
 *  @return  S16
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlAllocRcptInfo
(
RgSchUlAlloc   *alloc,
CmLteRnti      *rnti,
U8             *iMcsRef,
U8             *rbStartRef,
U8             *numRbRef,
U8             *rvRef,
U16            *size,
TfuModScheme   *modType,
Bool           *isRtx,
U8             *nDmrs,
Bool           *ndi,
U8             *hqPId
)
#else
PUBLIC S16 rgSCHUtlAllocRcptInfo(alloc, rnti, iMcsRef, rbStartRef, numRbRef,
                                 rvRef, size, modType, isRtx, nDmrs, ndi,
                                 hqPId)
RgSchUlAlloc   *alloc;
CmLteRnti      *rnti;
U8             *iMcsRef;
U8             *rbStartRef;
U8             *numRbRef;
U8             *rvRef;
U16            *size;
TfuModScheme   *modType;
Bool           *isRtx;
U8             *nDmrs;
Bool           *ndi;
U8             *hqPId;
#endif
{
   /* Modulation order for 16qam UEs would be
    * min(4,modulation order in grant). Please refer to 36.213-8.6.1*/
   CmLteUeCategory ueCtgy;

   TRC2(rgSCHUtlAllocRcptInfo);
#if (ERRCLASS & ERRCLS_DEBUG)
   if ((alloc == NULLP) || (alloc->hqProc == NULLP))
   {
      RETVALUE(RFAILED);
   }
#endif

   if ( !alloc->forMsg3 )
   {
      if ( ((alloc->ue) == NULLP) ||  (RG_SCH_CMN_GET_UE(alloc->ue, alloc->ue->cell) == NULLP))
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,alloc->ue->cell->cellId,
                     "Failed: ue->sch is null RNTI:%d,isRetx=%d",
                     alloc->rnti, alloc->grnt.isRtx);
         RETVALUE(RFAILED); 
      }
     ueCtgy =  (RG_SCH_CMN_GET_UE_CTGY(alloc->ue));
   }

   *iMcsRef    = alloc->grnt.iMcs;
   *rbStartRef = alloc->grnt.rbStart;
   *numRbRef   = alloc->grnt.numRb;
   *rvRef      = rgRvTable[alloc->hqProc->rvIdx];
   *rnti       = alloc->rnti;
   *size       = alloc->grnt.datSz;
   *modType    = (alloc->forMsg3)? alloc->grnt.modOdr:
                         ((ueCtgy == CM_LTE_UE_CAT_5)?
                         alloc->grnt.modOdr:
                         (RGSCH_MIN(RGSCH_QM_QPSK,alloc->grnt.modOdr)));
   *isRtx      = alloc->grnt.isRtx;
   *nDmrs      = alloc->grnt.nDmrs;
   *ndi        = alloc->hqProc->ndi;
   *hqPId      = alloc->hqProc->procId;

   RETVALUE(ROK);
}
#else
/**
 * @brief Returns uplink grant information required to permit
 *        PHY to receive data
 *
 * @details
 *
 *     Function: rgSCHUtlAllocRcptInfo
 *     Purpose:  Given an uplink allocation, this function returns
 *               uplink grant information which is needed by PHY to
 *               decode data sent from UE. This information includes:
 *               - RB start
 *               - Number of RBs
 *               - RV
 *
 *  @param[in]  RgSchUlAlloc   *alloc
 *  @param[out] U8             *rbStartRef
 *  @param[out] U8             *numRbRef
 *  @param[out] U8             *rvRef
 *  @param[out] U16            *size
 *  @param[out] TfuModScheme   *modType
 *  @return  S16
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlAllocRcptInfo
(
RgSchCellCb             *cell,
RgSchUlAlloc            *alloc,
CmLteTimingInfo         *timeInfo,
TfuUeUlSchRecpInfo      *recpReq
)
#else
PUBLIC S16 rgSCHUtlAllocRcptInfo(cell, alloc, timeInfo, recpReq)
RgSchCellCb             *cell;
RgSchUlAlloc            *alloc;
CmLteTimingInfo         *timeInfo;
TfuUeUlSchRecpInfo      *recpReq;
#endif
{
   TRC2(rgSCHUtlAllocRcptInfo);
#if (ERRCLASS & ERRCLS_DEBUG)
   if ((alloc == NULLP) || (alloc->hqProc == NULLP))
   {
      RETVALUE(RFAILED);
   }
#endif
   recpReq->size           = alloc->grnt.datSz;
   recpReq->rbStart        = alloc->grnt.rbStart;
   recpReq->numRb          = alloc->grnt.numRb;
   /* Modulation order min(4,mod in grant) for 16 qam UEs.
    * Please refer to 36.213-8.6.1*/
#ifdef FOUR_TX_ANTENNA
   recpReq->modType        = (TfuModScheme)((alloc->forMsg3)?alloc->grnt.modOdr:
         (/*(alloc->ue->ueCatEnum == CM_LTE_UE_CAT_5)?
            alloc->grnt.modOdr:    *//* Chandra:TmpFx-TM500 Cat5 with Only16QAM */
          (RGSCH_MIN(RGSCH_QM_QPSK,alloc->grnt.modOdr))));
#else
   recpReq->modType        = (TfuModScheme)((alloc->forMsg3)?alloc->grnt.modOdr:
                             ((alloc->ue->ueCatEnum == CM_LTE_UE_CAT_5)?
                              alloc->grnt.modOdr:
                             (RGSCH_MIN(RGSCH_QM_QPSK,alloc->grnt.modOdr))));
#endif
   recpReq->nDmrs          = alloc->grnt.nDmrs;
   recpReq->hoppingEnbld   = FALSE;
   recpReq->hoppingBits    = 0;
   recpReq->isRtx          = alloc->grnt.isRtx;
   recpReq->ndi            = alloc->hqProc->ndi;   
   recpReq->rv             = rgRvTable[alloc->hqProc->rvIdx];
#ifndef LTE_TDD
   recpReq->harqProcId = alloc->hqProc->procId;
#else
   recpReq->harqProcId = rgSCHCmnGetUlHqProcIdx(timeInfo, cell);
#endif
   /* Transmission mode is SISO till Uplink MIMO is implemented. */
   recpReq->txMode   = 0;
   /* This value needs to filled in in the case of frequency hopping. */
   recpReq->crntTxNb = 0;

   recpReq->mcs = alloc->grnt.iMcs;
#ifdef RG_5GTF
   recpReq->rbgStart = alloc->grnt.vrbgStart;
   recpReq->numRbg = alloc->grnt.numVrbg;
   recpReq->xPUSCHRange = alloc->grnt.xPUSCHRange;
   //TODO_SID Need to check
   recpReq->nAntPortLayer = 0;
   recpReq->SCID = alloc->grnt.SCID;
   recpReq->PMI = alloc->grnt.PMI;
   recpReq->uciWoTBFlag = alloc->grnt.uciOnxPUSCH;
   if(alloc->ue)
   {
      recpReq->beamIndex = alloc->ue->ue5gtfCb.BeamId;
   }
#endif

#ifdef TENB_STATS
   if (!alloc->forMsg3)
   {
      if (alloc->grnt.isRtx)
      {
         alloc->ue->tenbStats->stats.nonPersistent.sch[RG_SCH_CELLINDEX(alloc->ue->cell)].ulRetxOccns++;
      }
      else
      {
         alloc->ue->tenbStats->stats.nonPersistent.sch[RG_SCH_CELLINDEX(alloc->ue->cell)].ulTxOccns++;
         alloc->ue->tenbStats->stats.nonPersistent.sch[RG_SCH_CELLINDEX(alloc->ue->cell)].ulSumiTbs += \
            rgSCHCmnUlGetITbsFrmIMcs(alloc->grnt.iMcs);
         alloc->ue->tenbStats->stats.nonPersistent.sch[RG_SCH_CELLINDEX(alloc->ue->cell)].ulNumiTbs ++;
         cell->tenbStats->sch.ulSumiTbs += \
            rgSCHCmnUlGetITbsFrmIMcs(alloc->grnt.iMcs);
         cell->tenbStats->sch.ulNumiTbs ++;
      }
      alloc->ue->tenbStats->stats.nonPersistent.sch[RG_SCH_CELLINDEX(alloc->ue->cell)].ulPrbUsg += alloc->grnt.numRb;
      cell->tenbStats->sch.ulPrbUsage[0] += alloc->grnt.numRb;
   }
#endif
  /* ccpu00117050 - DEL - nSrs setting at rgSCHUtlAllocRcptInfo */
   RETVALUE(ROK);
}
#endif

#ifdef LTE_TDD
/**
 * @brief This function initialises the PRACH slot occasions
 *
 * @details
 *
 *     Function: rgSCHUtlUpdPrachOcc
 *     Purpose:  This function updates the PRACH slots based on
 *               RGR configuration.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgrTddPrachInfo  *cellCfg
 *  @return     Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHUtlUpdPrachOcc
(
RgSchCellCb       *cell,
RgrTddPrachInfo   *cellCfg
)
#else
PRIVATE Void rgSCHUtlUpdPrachOcc(cell, cellCfg)
RgSchCellCb       *cell;
RgrTddPrachInfo   *cellCfg;
#endif
{
   U8   idx;
   U8    count = 0;
   U8    size;
   U8    startIdx;
   U8    endIdx;

   TRC2(rgSCHUtlUpdPrachOcc)

   /* In the 1st half frame */
   if(cellCfg->halfFrm == 0)
   {
      startIdx = 2;
      endIdx = 6;
   }
   /* In the 2nd half frame */
   else
   {
      startIdx = 6;
      endIdx = 10;
   }
   for(idx = startIdx; idx < endIdx; idx++)
   {
      if(rgSchTddUlDlSubfrmTbl[cell->ulDlCfgIdx][idx]
            == RG_SCH_TDD_UL_slot)
      {
         if(cellCfg->ulStartSfIdx == count)
         {
            size = cell->rachCfg.raOccasion.size;
            cell->rachCfg.raOccasion.slotNum[size] = idx;
            cell->rachCfg.raOccasion.size++;
            break;
         }
         count ++;
      }
   }
   RETVOID;
}

/**
 * @brief This function initialises the PRACH occasions
 *
 * @details
 *
 *     Function: rgSCHUtlPrachCfgInit
 *     Purpose:  This function initialises the PRACH occasions based on
 *               RGR configuration.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgrCellCfg        *cellCfg
 *  @return     Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlPrachCfgInit
(
RgSchCellCb       *cell,
RgrCellCfg        *cellCfg
)
#else
PUBLIC Void rgSCHUtlPrachCfgInit(cell, cellCfg)
RgSchCellCb       *cell;
RgrCellCfg        *cellCfg;
#endif
{
   U8 idx;
   U8 subfrmIdx;
   U8 splFrm;

   TRC2(rgSCHUtlPrachCfgInit)
   if(cellCfg->prachRscInfo.numRsc <= 0)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId, "Invalid"
                  "PRACH resources Configuration ");
      RETVOID;
   }

   /* Update SFN occasions */
   cell->rachCfg.raOccasion.sfnEnum =
                     cellCfg->prachRscInfo.prachInfo[0].sfn;

   cell->rachCfg.raOccasion.size = 0;

   /* Update slot occasions */
   for(idx = 0; idx < cellCfg->prachRscInfo.numRsc; idx++)
   {
      if(cellCfg->prachRscInfo.prachInfo[idx].freqIdx == 0)
      {
         if(cellCfg->prachRscInfo.prachInfo[idx].halfFrm == 0)
         {
            splFrm = 1;
         }
         else
         {
            splFrm = 6;
         }
         if(cellCfg->prachRscInfo.prachInfo[idx].ulStartSfIdx ==
                                                RGR_TDD_SPL_UL_IDX)
         {
            subfrmIdx = cell->rachCfg.raOccasion.size;
            cell->rachCfg.raOccasion.slotNum[subfrmIdx] = splFrm;
            cell->rachCfg.raOccasion.size++;
         }
         else
         {
            rgSCHUtlUpdPrachOcc(cell,
                  &cellCfg->prachRscInfo.prachInfo[idx]);
         }
      }
   }
   RETVOID;
}

/**
 * @brief This function performs RGR cell initialization
 *
 * @details
 *
 *     Function: rgSCHUtlRgrCellCfg
 *     Purpose:  This function initialises the cell with RGR configuration
 *               and slot related initialization.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgrCellCfg        *cellCfg
 *  @param[in]  RgSchErrInfo      *errInfo
 *  @return     Void
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrCellCfg
(
RgSchCellCb       *cell,
RgrCellCfg        *cellCfg,
RgSchErrInfo      *errInfo
)
#else
PUBLIC S16 rgSCHUtlRgrCellCfg(cell, cellCfg, errInfo)
RgSchCellCb       *cell;
RgrCellCfg        *cellCfg;
RgSchErrInfo      *errInfo;
#endif
{
   U8              i;
   U8              sfn=0;
   U8              sfNum = 0;
   RgSchDlSf       *sf;
   CmLteTimingInfo frm;
   U8              ulDlCfgIdx = cellCfg->ulDlCfgIdx;
   U8              maxslots ;
   U8              maxDlslots;
   S16             ret = ROK;
   U16             bw;         /*!< Number of RBs in the cell */
   
   TRC2(rgSCHUtlRgrCellCfg);

   cmMemset((U8 *)&frm,0,sizeof(CmLteTimingInfo));

   /* ccpu00132657-MOD- Determining DLSF array size independent of DELTAS */
   maxDlslots = rgSchTddNumDlSubfrmTbl[ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];
   maxslots = 2 * maxDlslots;
   cell->numDlSubfrms = maxslots;
/* ACC-TDD <ccpu00130639> */
   cell->tddHqSfnCycle = -1;
   cell->ulDlCfgIdx = ulDlCfgIdx;

   /* PRACH Occasions Initialization */
   rgSCHUtlPrachCfgInit(cell, cellCfg);

   /* ccpu00132658- Moved out of below for loop since the updating rbgSize and 
    * bw are independent of sfNum*/
   /* determine the RBG size and no of RBGs for the configured
    * DL BW */
   if (cell->bwCfg.dlTotalBw > 63)
   {
      cell->rbgSize  = 4;
   }
   else if (cell->bwCfg.dlTotalBw > 26)
   {
      cell->rbgSize  = 3;
   }
   else if (cell->bwCfg.dlTotalBw > 10)
   {
      cell->rbgSize  = 2;
   }
   else
   {
      cell->rbgSize  = 1;
   }
   cell->noOfRbgs = RGSCH_CEIL(cell->bwCfg.dlTotalBw, cell->rbgSize);

   bw    = cell->bwCfg.dlTotalBw;

   rgSCHUtlAllocSBuf(cell->instIdx,
               (Data **)&cell->subFrms, sizeof(RgSchDlSf *) * maxslots);
   if (cell->subFrms == NULLP)
   {
      RETVALUE(RFAILED);
   }

   /* Create memory for each frame. */
   for(i = 0; i < maxslots; i++)
   {
      while(rgSchTddUlDlSubfrmTbl[ulDlCfgIdx][sfNum] ==
            RG_SCH_TDD_UL_slot)
      {
         sfNum = (sfNum+1) % RGSCH_NUM_SUB_FRAMES;
      }

      rgSCHUtlAllocSBuf(cell->instIdx, (Data **)&sf, sizeof(RgSchDlSf));
      if (sf == NULLP)
      {
         break;
      }
      cmMemset((U8 *)sf, 0, sizeof(*sf));

#ifdef LTE_ADV
      if (ROK != rgSCHLaaInitDlSfCb(cell, sf))
      {
         break;
      }
#endif
      sf->sfNum = sfNum;
      sf->bw    = bw;
#ifdef LTEMAC_SPS
   /* Mark SPS bandwidth to be occupied */
   sf->bwAlloced = ((cellCfg->spsCfg.maxSpsDlBw +
      cell->rbgSize - 1)/cell->rbgSize) * cell->rbgSize;
   sf->spsAllocdBw = 0;
   sf->type2End = sf->bwAlloced/cell->rbgSize;
#else
   sf->bwAlloced = 0;
   /* Fix for ccpu00123918*/
   sf->type2Start = 0;
#endif /* LTEMAC_SPS */
      /* Initialize the ackNakRepQ here */
#ifdef RG_MAC_MEASGAP
      cmLListInit (&(sf->ackNakRepQ));
#endif
      cell->subFrms[i] = sf;
      sfNum = (sfNum+1) % RGSCH_NUM_SUB_FRAMES;
   }
   if (i != maxslots)
   {
      for (; i > 0; i--)
      {
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(cell->instIdx,
               (Data **)(&(cell->subFrms[i-1])), sizeof(RgSchDlSf));
#ifdef LTE_ADV
         rgSCHLaaDeInitDlSfCb(cell, sf);
#endif
      }
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx,
            (Data **)(&(cell->subFrms)), sizeof(RgSchDlSf *) * maxslots);

      RETVALUE(RFAILED);
   }

   if (cell->sc.apis == NULLP)
   {
      cell->sc.apis = &rgSchCmnApis;
   }
   ret = cell->sc.apis->rgSCHRgrCellCfg(cell, cellCfg, errInfo);

   if (ret != ROK)
   {
      /* ccpu00132286- Removed deletion of sf nodes as the deletion will be 
       * happening during CellDelete. Added return handling to provide negative
       * confirm*/
      RETVALUE(ret);
   }

   /* Release the slots and thereby perform the initialization */
   for (i = 0; i < maxslots; i++)
   {
     if((i > 0) && (i%maxDlslots == 0))
     {
      sfn++;
     }
     frm.sfn = sfn;
     frm.slot = cell->subFrms[i]->sfNum;
     rgSCHUtlDlRlsSubFrm(cell, frm);
   }

   RETVALUE(ret);
}

#else

/**
 * @brief This function performs scheduler related cell creation
 *
 * @details
 *
 *     Function: rgSCHUtlRgrCellCfg
 *     Purpose:  This function creates the slots needed for the
 *               cell. It then peforms init of the scheduler by calling
 *               scheduler specific cell init function.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb          *cell
 *  @param[in]  RgrCellCfg        *cellCfg
 *  @param[in]  RgSchErrInfo         *errInfo
 *  @return     Void
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrCellCfg
(
RgSchCellCb       *cell,
RgrCellCfg        *cellCfg,
RgSchErrInfo      *errInfo
)
#else
PUBLIC S16 rgSCHUtlRgrCellCfg(cell, cellCfg, errInfo)
RgSchCellCb       *cell;
RgrCellCfg        *cellCfg;
RgSchErrInfo      *errInfo;
#endif
{
   U8              i;
   RgSchDlSf       *sf;
   CmLteTimingInfo frm;
   S16             ret;
   Inst            inst = cell->instIdx;
   /* LTE_ADV_FLAG_REMOVED_START */
   U16             len;
   len = (U16)((cell->bwCfg.dlTotalBw % 8 == 0) ? (cell->bwCfg.dlTotalBw/8) : (cell->bwCfg.dlTotalBw/8 + 1)); /*KW fix for LTE_ADV */
   /* LTE_ADV_FLAG_REMOVED_END */
   TRC2(rgSCHUtlRgrCellCfg);

   cmMemset((U8 *)&frm,0,sizeof(CmLteTimingInfo));

   /* determine the RBG size and no of RBGs for the configured
    * DL BW */
   if (cell->bwCfg.dlTotalBw > 63)
   {
      cell->rbgSize  = 4;
   }
   else if (cell->bwCfg.dlTotalBw > 26)
   {
      cell->rbgSize  = 3;
   }
   else if (cell->bwCfg.dlTotalBw > 10)
   {
      cell->rbgSize  = 2;
   }
   else
   {
      cell->rbgSize  = 1;
   }
   cell->noOfRbgs = RGSCH_CEIL(cell->bwCfg.dlTotalBw, cell->rbgSize);
   /* Create memory for each frame. */
   /* Changing loop limit from
      RGSCH_NUM_SUB_FRAMES to RGSCH_NUM_DL_slotS */
   for(i = 0; i < RGSCH_NUM_DL_slotS; i++)
   {
      rgSCHUtlAllocSBuf(inst, (Data **)&sf, sizeof(RgSchDlSf));
      if (sf == NULLP)
      {
         break;
      }
      cmMemset((U8 *)sf, 0, sizeof(*sf));

#ifdef LTE_ADV
      if (ROK != rgSCHLaaInitDlSfCb(cell, sf))
      {
         break;
      }
#endif
      /* Doing MOD operation before assigning value of i */
      sf->sfNum = i % RGSCH_NUM_SUB_FRAMES;
      sf->bw    = cell->bwCfg.dlTotalBw;
      /* Initialize the ackNakRepQ here */
#ifdef RG_MAC_MEASGAP
      cmLListInit (&(sf->ackNakRepQ));
#endif
      cell->subFrms[i] = sf;
      /* LTE_ADV_FLAG_REMOVED_START */
      if (cell->lteAdvCb.dsfrCfg.status == RGR_ENABLE)
      {
         /*initialize the RNTP Buffer*/
         if(rgSchDSFRRntpInfoInit(&sf->rntpInfo, cell, sf->bw))
         {
            RETVALUE(RFAILED); 
         }
      }

      if (cell->lteAdvCb.sfrCfg.status == RGR_ENABLE)
      {
         /*initialise the pools of CC and CE*/
         if(rgSchSFRTotalPoolInit(cell, sf))
         {
            RETVALUE(RFAILED); 
         }
      }
      /* LTE_ADV_FLAG_REMOVED_END */
   }

   /* LTE_ADV_FLAG_REMOVED_START */
   /* Allocate memory for "scheduled UE" Info */
   if (cell->lteAdvCb.dsfrCfg.status == RGR_ENABLE)
   {    
      if((rgSCHUtlAllocSBuf(inst, (Data**)&(cell->rntpAggrInfo.val),
                  (len * sizeof(U8)))) != ROK)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Memory allocation FAILED for RNTP Alloc");
         RETVALUE(RFAILED);
      }
      cell->rntpAggrInfo.pres = PRSNT_NODEF;
      cell->rntpAggrInfo.len  = len;
   }     
   /* LTE_ADV_FLAG_REMOVED_END */

   /* Changing loop limit from
      RGSCH_NUM_SUB_FRAMES to RGSCH_NUM_DL_slotS */
   if (i != RGSCH_NUM_DL_slotS)
   {
      for (; i > 0; i--)
      {
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(inst, (Data **)(&(cell->subFrms[i-1])),
               sizeof(RgSchDlSf));
#ifdef LTE_ADV
         rgSCHLaaDeInitDlSfCb(cell, sf);
#endif
      }
      RETVALUE(RFAILED);
   }

   if (cell->sc.apis == NULLP)
   {
      cell->sc.apis = &rgSchCmnApis;
   }

   /* Release the slots and thereby perform the initialization */
   for (i = 0; i < RGSCH_NUM_DL_slotS; i++)
   {
      if (i >= RGSCH_NUM_SUB_FRAMES)
      {
         /* [ccpu00123828]-MOD-The below statement sfn += 1incorrectly modified
          * the value of sfn for i>=10 thru 19. Correct way is to assign
          it to one */
         frm.sfn = 1;
      }
      frm.slot = i % RGSCH_NUM_SUB_FRAMES;
      rgSCHUtlDlRlsSubFrm(cell, frm);
   }

   ret = cell->sc.apis->rgSCHRgrCellCfg(cell, cellCfg, errInfo);
   if (ret != ROK)
   {
      errInfo->errCause = RGSCHERR_SCH_CFG;
      RETVALUE(RFAILED);
   }
#ifdef EMTC_ENABLE
      if(cell->emtcEnable)
      {
         /* TODO: Repetition framework in RGR and APP */
         if (rgSCHUtlEmtcResMngmtInit(
                  cell, 
                  RGSCH_IOT_PDSCH_POOLSZ, RGSCH_IOT_PDSCH_DELTA, cellCfg->bwCfg.dlTotalBw,
                  RGSCH_IOT_PUSCH_POOLSZ, RGSCH_IOT_PUSCH_DELTA, RGSCH_IOT_PUSCH_MAXFREQSZ,
                  RGSCH_IOT_PUCCH_POOLSZ, RGSCH_IOT_PUCCH_DELTA, RGSCH_IOT_PUCCH_MAXFREQSZ) != ROK)
         {
            errInfo->errCause = RGSCHERR_SCH_CFG;
            RETVALUE(RFAILED);
         }
      }
#endif

   RETVALUE(ret);
}
#endif


/**
 * @brief This function performs the cell reconfiguration at RGR interface
 *
 * @details
 *
 *     Function: rgSCHUtlRgrCellRecfg
 *     Purpose:  This function updates the reconfigurable parameters
 *               on the cell control block for the scheduler.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb          *cell
 *  @param[in]  RgrCellCfg        *cellCfg
 *  @param[in]  RgSchErrInfo         *errInfo
 *  @return     Void
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrCellRecfg
(
RgSchCellCb       *cell,
RgrCellRecfg      *recfg,
RgSchErrInfo      *err
)
#else
PUBLIC S16 rgSCHUtlRgrCellRecfg(cell, recfg, err)
RgSchCellCb       *cell;
RgrCellRecfg      *recfg;
RgSchErrInfo      *err;
#endif
{
   TRC2(rgSCHUtlRgrCellRecfg);
   RETVALUE(cell->sc.apis->rgSCHRgrCellRecfg(cell, recfg, err));
}



/**
 * @brief This function returns the Y value of UE for a sub frame
 *
 * @details
 *
 *     Function: rgSCHUtlFreeCell
 *     Purpose:  This function updates the value of Y stored in the
 *               UE control block. It uses the previously computed
 *               value for computing for this slot.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb          *cell
 *  @return     Void
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlFreeCell
(
RgSchCellCb          *cell
)
#else
PUBLIC S16 rgSCHUtlFreeCell(cell)
RgSchCellCb          *cell;
#endif
{
   U8               i;
   CmLListCp        *lst;
   RgSchPdcch       *pdcch;
   RgSchPdcchInfo   *pdcchInfo;
   RgSchPhichInfo   *phichInfo;
   RgSchPhich       *phich;
   Inst             inst = cell->instIdx;
   U8               maxslots;
#ifdef LTE_TDD
   RgSchRaReqInfo *raReqInfo;
   U8                idx;
#endif
   TRC2(rgSCHUtlFreeCell);

#ifdef LTE_TDD
   maxslots = cell->numDlSubfrms;
#else
   maxslots = RGSCH_NUM_DL_slotS;
#endif


   /* Invoke the index for scheduler, cell deletion */
   cell->sc.apis->rgSCHFreeCell(cell);

   /* Release the slots allocated               */
   for (i = 0; i < maxslots; i++)
   {
#ifdef LTE_ADV
      rgSCHLaaDeInitDlSfCb(cell, cell->subFrms[i]);
#endif
      pdcchInfo = &cell->subFrms[i]->pdcchInfo;
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)(&(pdcchInfo->map)),
            (pdcchInfo->nCce + 7) >> 3);
      while (pdcchInfo->pdcchs.first != NULLP)
      {
         pdcch = (RgSchPdcch *)pdcchInfo->pdcchs.first->node;
         cmLListDelFrm(&pdcchInfo->pdcchs, pdcchInfo->pdcchs.first);
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(inst, (Data **)&pdcch, sizeof(RgSchPdcch));
      }

      phichInfo = &cell->subFrms[i]->phichInfo;
      while(phichInfo->phichs.first != NULLP)
      {
         phich = (RgSchPhich *)phichInfo->phichs.first->node;
         cmLListDelFrm(&phichInfo->phichs, phichInfo->phichs.first);
         RGSCH_PHICH_FREE(inst, phich, sizeof(RgSchPhich));
      }

      /* LTE_ADV_FLAG_REMOVED_START */
      /*releasing SFR pool entries*/
      rgSchSFRTotalPoolFree(&cell->subFrms[i]->sfrTotalPoolInfo, cell);

      /*releasing dsfr rntp pattern info*/
      rgSchDSFRRntpInfoFree(&cell->subFrms[i]->rntpInfo, cell, 
                                       cell->bwCfg.dlTotalBw);
      /* LTE_ADV_FLAG_REMOVED_END */

      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)(&(cell->subFrms[i])), sizeof(RgSchDlSf));
   }
#ifdef LTE_TDD
   /* Release the slot pointers */
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(inst,
         (Data **) (&(cell->subFrms)), sizeof(RgSchDlSf *) * maxslots);

   for(idx=0; idx < cell->raInfo.lstSize; idx++)
   {
      lst = &cell->raInfo.raReqLst[idx];
      while (lst->first != NULLP)
      {
         raReqInfo = (RgSchRaReqInfo *)lst->first->node;
         cmLListDelFrm(lst, &raReqInfo->raReqLstEnt);
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(inst,(Data **)&raReqInfo, sizeof(RgSchRaReqInfo));
      }
   }
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(inst,
         (Data **)(&(cell->raInfo.raReqLst)),
            sizeof(CmLListCp) * (cell->raInfo.lstSize));
#endif

   /* Release allocated pdcchs */
   lst = &cell->pdcchLst;
   while (lst->first != NULLP)
   {
      pdcch = (RgSchPdcch *)lst->first->node;
      cmLListDelFrm(lst, &pdcch->lnk);
#ifdef EMTC_ENABLE
      if(cell->emtcEnable)
      {
         rgSCHEmtcPdcchFree(cell, pdcch);
         rgSCHUtlEmtcResMngmtDeinit(cell);
      }
#endif
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst,(Data **)&pdcch, sizeof(RgSchPdcch));
   }
#ifdef LTE_ADV
   rgSCHLaaFreeLists(cell);
#endif

   /* LTE_ADV_FLAG_REMOVED_START */
   /* releasing RNTP Aggregation Info from CellCb*/
   rgSchDSFRRntpInfoFree(&cell->rntpAggrInfo, cell, cell->bwCfg.dlTotalBw);
   /* LTE_ADV_FLAG_REMOVED_END */

   RETVALUE(ROK);
}


/**
 * @brief This function adds the UE to scheduler
 *
 * @details
 *
 *     Function: rgSCHUtlRgrUeCfg
 *     Purpose:  This function performs addition of UE to scheduler
 *               1. First, it updates the Y table in the UE
 *               2. Then, it calls the scheduler's handler for UE addition
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb          *cell
 *  @param[in]  RgSchUeCb            *ue
 *  @param[in]  RgrUeCfg             *cfg
 *  @param[in]  RgSchErrInfo            *err
 *  @return     S16
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrUeCfg
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgrUeCfg                   *cfg,
RgSchErrInfo               *err
)
#else
PUBLIC S16 rgSCHUtlRgrUeCfg(cell, ue, cfg, err)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgrUeCfg                   *cfg;
RgSchErrInfo               *err;
#endif
{
   TRC2(rgSCHUtlRgrUeCfg);

   /* Assign TM 1 as UE's default TM */
   ue->mimoInfo.txMode = RGR_UE_TM_1;
   ue->txModeTransCmplt = TRUE;
   cmInitTimers(&ue->txModeTransTmr, 1);
   if (cfg->txMode.pres == PRSNT_NODEF)
   {
      /* DL MU-MIMO not supported */
      if (cfg->txMode.txModeEnum == RGR_UE_TM_5)
      {
         err->errCause = RGSCHERR_SCH_CFG;
         RETVALUE(RFAILED);
      }
      ue->mimoInfo.txMode = cfg->txMode.txModeEnum;
   }
   ue->ul.ulTxAntSel = cfg->ulTxAntSel;
   ue->mimoInfo.cdbkSbstRstrctn = cfg->ueCodeBookRstCfg;
#ifdef TFU_UPGRADE
   ue->ueCatEnum = cfg->ueCatEnum;
   if ((cfg->puschDedCfg.bACKIdx > 15) ||
       (cfg->puschDedCfg.bCQIIdx > 15) ||
       (cfg->puschDedCfg.bRIIdx > 15))
   {
      err->errCause = RGSCHERR_SCH_CFG;
      RETVALUE(RFAILED);
   }
   ue->ul.betaHqOffst = cfg->puschDedCfg.bACKIdx;
   ue->ul.betaCqiOffst = cfg->puschDedCfg.bCQIIdx;
   ue->ul.betaRiOffst = cfg->puschDedCfg.bRIIdx;
#endif
   ue->csgMmbrSta = cfg->csgMmbrSta;
#ifdef RG_PFS_STATS
   cmMemset((U8 *)&ue->pfsStats, 0, sizeof(RgSchPfsStats));
#endif
   /* Call the handler of the scheduler based on cell configuration */
   RETVALUE(cell->sc.apis->rgSCHRgrUeCfg(cell, ue, cfg, err));
}
/* Start : LTEMAC_2.1_DEV_CFG */

/**
 * @brief This function adds a service to scheduler
 *
 * @details
 *
 *     Function: rgSCHUtlRgrLcCfg
 *     Purpose:  This function performs addition of service to scheduler
 *               The addition is performed for each direction based
 *               the direction field of the configuration
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb          *cell
 *  @param[in]  RgSchUeCb            *ue
 *  @param[in]  RgSchDlLcCb          *dlLc
 *  @param[in]  RgrLchCfg            *cfg
 *  @param[in]  RgSchErrInfo         *err
 *  @return     S16
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrLcCfg
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *dlLc,
RgrLchCfg               *cfg,
RgSchErrInfo               *errInfo
)
#else
PUBLIC S16 rgSCHUtlRgrLcCfg(cell, ue, dlLc, cfg, errInfo)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlLcCb                *dlLc;
RgrLchCfg               *cfg;
RgSchErrInfo               *errInfo;
#endif
{
   TRC2(rgSCHUtlRgrLcCfg);
   RETVALUE(cell->sc.apis->rgSCHRgrLchCfg(cell, ue, dlLc, cfg, errInfo));
}


/**
 * @brief This function modifies a service to scheduler
 *
 * @details
 *
 *     Function: rgSCHUtlRgrLcRecfg
 *     Purpose:  This function performs modification of a service in
 *               scheduler. The modification is performed for each direction
 *               based the direction field of the configuration
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb          *cell
 *  @param[in]  RgSchUeCb            *ue
 *  @param[in]  RgSchDlLcCb          *dlLc
 *  @param[in]  RgrLchRecfg          *recfg
 *  @param[in]  RgSchErrInfo         *err
 *  @return     S16
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrLcRecfg
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *dlLc,
RgrLchRecfg                *recfg,
RgSchErrInfo               *err
)
#else
PUBLIC S16 rgSCHUtlRgrLcRecfg(cell, ue, dlLc, recfg, err)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlLcCb                *dlLc;
RgrLchRecfg                *recfg;
RgSchErrInfo               *err;
#endif
{
   TRC2(rgSCHUtlRgrLcRecfg);
   RETVALUE(cell->sc.apis->rgSCHRgrLchRecfg(cell, ue, dlLc, recfg, err));
}

/**
 * @brief This function deletes a Lc in scheduler
 *
 * @details
 *
 *     Function: rgSCHUtlRgrLcDel
 *     Purpose:  This function performs deletion of Lc in scheduler
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgSchUeCb     *ue
 *  @param[in]  CmLteLcId     lcId
 *  @param[in]  U8            lcgId
 *  @return     S16
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrLcDel
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
CmLteLcId                  lcId,
U8                         lcgId
)
#else
PUBLIC S16 rgSCHUtlRgrLcDel(cell, ue, lcId, lcgId)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
CmLteLcId                  lcId;
U8                         lcgId;
#endif
{
   TRC2(rgSCHUtlRgrLcDel);
   cell->sc.apis->rgSCHRgrLchDel(cell, ue, lcId, lcgId);

   RETVALUE (ROK);
} /* rgSCHUtlRgrLcDel */

/**
 * @brief This function adds a service to scheduler
 *
 * @details
 *
 *     Function: rgSCHUtlRgrLcgCfg
 *     Purpose:  This function performs addition of service to scheduler
 *               The addition is performed for each direction based
 *               the direction field of the configuration
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb          *cell
 *  @param[in]  RgSchUeCb            *ue
 *  @param[in]  RgrLchCfg            *cfg
 *  @param[in]  RgSchErrInfo         *err
 *  @return     S16
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrLcgCfg
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgrLcgCfg               *cfg,
RgSchErrInfo               *errInfo
)
#else
PUBLIC S16 rgSCHUtlRgrLcgCfg(cell, ue, cfg, errInfo)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgrLcgCfg               *cfg;
RgSchErrInfo               *errInfo;
#endif
{
   TRC2(rgSCHUtlRgrLcgCfg);
   RETVALUE(cell->sc.apis->rgSCHRgrLcgCfg(cell, ue, &(ue->ul.lcgArr[cfg->ulInfo.lcgId]), cfg, errInfo));
}


/**
 * @brief This function modifies a service to scheduler
 *
 * @details
 *
 *     Function: rgSCHUtlRgrLcgRecfg
 *     Purpose:  This function performs modification of a service in
 *               scheduler. The modification is performed for each direction
 *               based the direction field of the configuration
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb          *cell
 *  @param[in]  RgSchUeCb            *ue
 *  @param[in]  RgrLcgRecfg          *recfg
 *  @param[in]  RgSchErrInfo         *err
 *  @return     S16
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrLcgRecfg
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgrLcgRecfg                *recfg,
RgSchErrInfo               *err
)
#else
PUBLIC S16 rgSCHUtlRgrLcgRecfg(cell, ue, recfg, err)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgrLcgRecfg                *recfg;
RgSchErrInfo               *err;
#endif
{
   TRC2(rgSCHUtlRgrLcgRecfg);
   RETVALUE(cell->sc.apis->rgSCHRgrLcgRecfg(cell, ue, &(ue->ul.lcgArr[recfg->ulRecfg.lcgId]), recfg, err));
} /* rgSCHUtlRgrLcRecfg */

/**
 * @brief This function modifies a service to scheduler
 *
 * @details
 *
 *     Function: rgSCHUtlRgrLcgDel
 *     Purpose:  This function performs modification of a service in
 *               scheduler. The modification is performed for each direction
 *               based the direction field of the configuration
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgSchUeCb     *ue
 *  @param[in]  RgrDel        *lcDelInfo
 *  @return     S16
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlRgrLcgDel
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
U8                         lcgId
)
#else
PUBLIC Void rgSCHUtlRgrLcgDel(cell, ue, lcgId)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
U8                         lcgId;
#endif
{
   TRC2(rgSCHUtlRgrLcgDel);
   cell->sc.apis->rgSCHFreeLcg(cell, ue, &ue->ul.lcgArr[lcgId]);

  /* Stack Crash problem for TRACE5 changes. added the return below . */
 RETVOID;

} /* rgSCHUtlRgrLcgDel */


/* End: LTEMAC_2.1_DEV_CFG */

/**
 * @brief This function is a wrapper to call scheduler specific API.
 *
 * @details
 *
 *     Function: rgSCHUtlDoaInd
 *     Purpose:  Updates the DOA for the UE
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  TfuDoaRpt          *doaRpt
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void  rgSCHUtlDoaInd
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
TfuDoaRpt          *doaRpt
)
#else
PUBLIC Void rgSCHUtlDoaInd(cell, ue, doaRpt)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
TfuDoaRpt          *doaRpt;
#endif
{
   TRC2(rgSCHUtlDoaInd);
   ue->mimoInfo.doa.pres = PRSNT_NODEF;
   ue->mimoInfo.doa.val = doaRpt->doa;
   RETVOID;
}

/**
 * @brief This function is a wrapper to call scheduler specific API.
 *
 * @details
 *
 *     Function: rgSCHUtlDlCqiInd
 *     Purpose:  Updates the DL CQI for the UE
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  TfuDlCqiRpt        *dlCqiRpt
 *  @param[in]  CmLteTimingInfo    timingInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlDlCqiInd
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
TfuDlCqiRpt        *dlCqiRpt,
CmLteTimingInfo    timingInfo
)
#else
PUBLIC Void rgSCHUtlDlCqiInd(cell, ue, dlCqiRpt, timingInfo)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
TfuDlCqiRpt        *dlCqiRpt;
CmLteTimingInfo    timingInfo;
#endif
{
   RgSchCellCb        *sCellCb = NULLP;
   TRC2(rgSCHUtlDlCqiInd);
   if (dlCqiRpt->isPucchInfo)
   {
      sCellCb = ue->cellInfo[dlCqiRpt->dlCqiInfo.pucchCqi.cellIdx]->cell;
      sCellCb->sc.apis->rgSCHDlCqiInd(sCellCb, ue, dlCqiRpt->isPucchInfo, \
            (Void *)&dlCqiRpt->dlCqiInfo.pucchCqi, timingInfo);
   }
   else
   {
      U32 idx;
      for (idx = 0; idx < dlCqiRpt->dlCqiInfo.pusch.numOfCells; idx++)
      {
         sCellCb = ue->cellInfo[dlCqiRpt->dlCqiInfo.pusch.puschCqi[idx].cellIdx]->cell;
         sCellCb->sc.apis->rgSCHDlCqiInd(sCellCb, ue, dlCqiRpt->isPucchInfo, \
               (Void *)&dlCqiRpt->dlCqiInfo.pusch.puschCqi[idx], timingInfo);
      }
   }
   RETVOID;
}


#ifdef TFU_UPGRADE
/**
 * @brief This function is a wrapper to call scheduler specific API.
 *
 * @details
 *
 *     Function: rgSCHUtlSrsInd
 *     Purpose:  Updates the UL SRS for the UE
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  TfuSrsRpt*     srsRpt
 *  @param[in]  CmLteTimingInfo    timingInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlSrsInd
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
TfuSrsRpt          *srsRpt,
CmLteTimingInfo    timingInfo
)
#else
PUBLIC Void rgSCHUtlSrsInd(cell, ue, srsRpt, timingInfo)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
TfuSrsRpt        *srsRpt;
CmLteTimingInfo    timingInfo;
#endif
{
   TRC2(rgSCHUtlSrsInd);
   cell->sc.apis->rgSCHSrsInd(cell, ue, srsRpt, timingInfo);
   RETVOID;
}
#endif

/**
 * @brief This function is a wrapper to call scheduler specific API.
 *
 * @details
 *
 *     Function: rgSCHUtlDlTARpt
 *     Purpose:  Reports PHY TA for a UE.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlDlTARpt
(
RgSchCellCb        *cell,
RgSchUeCb          *ue
)
#else
PUBLIC Void rgSCHUtlDlTARpt(cell, ue)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
#endif
{
   TRC2(rgSCHUtlDlTARpt);
   cell->sc.apis->rgSCHDlTARpt(cell, ue);
   RETVOID;
}


/**
 * @brief This function is a wrapper to call scheduler specific API.
 *
 * @details
 *
 *     Function: rgSCHUtlDlRlsSubFrm
 *     Purpose:  Releases scheduler Information from DL SubFrm.
 *
 *     Invoked by: DHM
 *
 *  @param[in]   RgSchCellCb     *cell
 *  @param[out]  CmLteTimingInfo subFrm
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlDlRlsSubFrm
(
RgSchCellCb        *cell,
CmLteTimingInfo    subFrm
)
#else
PUBLIC Void rgSCHUtlDlRlsSubFrm(cell, subFrm)
RgSchCellCb        *cell;
CmLteTimingInfo    subFrm;
#endif
{
   TRC2(rgSCHUtlDlRlsSubFrm);
   cell->sc.apis->rgSCHDlRlsSubFrm(cell, subFrm);
   RETVOID;
}

#ifdef TFU_UPGRADE
/**
 * @brief This API is invoked to update the AperCQI trigger
 *        weight.
 *
 * @details
 *
 *     Function : rgSCHUtlUpdACqiTrigWt
 *              - If HqFdbk is ACK then add up weight corresponding
 *                to ACK to the AcqiTrigWt.
 *              - If HqFdbk is NACK then add up weight corresponding
 *                to NACK to the AcqiTrigWt.
 *              - If AcqiTrigWt crosses threshold then trigger
 *                grant req for APERCQI to SCH.
 *
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  U8              isAck 
 *
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlUpdACqiTrigWt
(
RgSchUeCb       *ue,
RgSchUeCellInfo *cellInfo,
U8              isAck 
)
#else
PUBLIC Void rgSCHUtlUpdACqiTrigWt(ue,cellInfo, isAck)
RgSchUeCb       *ue;
RgSchUeCellInfo *cellInfo;
U8              isAck; 
#endif
{
#ifdef LTE_ADV
   U8 triggerSet  = 0;
   U8 sIdx        = 0;
#endif

   TRC2(rgSCHUtlUpdACqiTrigWt);

   if (isAck == TFU_HQFDB_ACK)
   {
      cellInfo->acqiCb.aCqiTrigWt += RG_APER_CQI_ACK_WGT;
   }
   else
   {
      cellInfo->acqiCb.aCqiTrigWt += RG_APER_CQI_NACK_WGT;
   }

   if (cellInfo->acqiCb.aCqiTrigWt > RG_APER_CQI_THRESHOLD_WGT)
   {
      RgSchCellCb  *cell = ue->cell;
      RgSchErrInfo unUsed;

      if(ue->dl.reqForCqi)
      {
         /* Already one ACQI trigger procedure is going on
          * which is not yet satisfied. Delaying this request till
          * the previous is getting satisfied*/
         RETVOID;
      }

     ue->dl.reqForCqi = TRUE;
#ifdef LTE_ADV
      rgSchCmnSetCqiReqField(cellInfo,ue,&ue->dl.reqForCqi);
      //Reset aCqiTrigWt for all the serving cells for which we have triggered ACQI 
      rgSCHTomUtlGetTrigSet(cell, ue, ue->dl.reqForCqi, &triggerSet);
      for (sIdx = 0; sIdx < CM_LTE_MAX_CELLS; sIdx++)
      {
         /* The Aperiodic requested for SCell index sIdx */
         if ((triggerSet >> (7 - sIdx)) & 0x01)
         {
            /* The Aperiodic request for SCell index sIdx */
            ue->cellInfo[sIdx]->acqiCb.aCqiTrigWt = 0;
         }
      }
 
#endif
      /* Force SCH to send UL grant by indicating fake SR.
       * If this UE already in UL SCH Qs this SR Ind will
       * be ignored */
      rgSCHUtlSrRcvd(cell, ue, cell->crntTime, &unUsed);
   }

   RETVOID;
}
#endif

/**
 * @brief This API is invoked to indicate scheduler of a CRC indication.
 *
 * @details
 *
 *     Function : rgSCHUtlHdlUlTransInd
 *      This API is invoked to indicate scheduler of a CRC indication.
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  CmLteTimingInfo timingInfo
 *
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlHdlUlTransInd
(
RgSchCellCb     *cell,
RgSchUeCb       *ue,
CmLteTimingInfo timingInfo
)
#else
PUBLIC Void rgSCHUtlHdlUlTransInd(cell, ue, timingInfo)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
CmLteTimingInfo timingInfo;
#endif
{
   TRC2(rgSCHUtlHdlUlTransInd);
   cell->sc.apis->rgSCHHdlUlTransInd(cell, ue, timingInfo);
   RETVOID;
}
#ifdef LTEMAC_SPS
/**
 * @brief This API is invoked to indicate scheduler of a CRC failure.
 *
 * @details
 *
 *     Function : rgSCHUtlHdlCrcInd
 *      This API is invoked to indicate CRC  to scheduler.
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  CmLteTimingInfo timingInfo
 *
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlHdlCrcInd
(
RgSchCellCb     *cell,
RgSchUeCb       *ue,
CmLteTimingInfo timingInfo
)
#else
PUBLIC Void rgSCHUtlHdlCrcInd(cell, ue, timingInfo)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
CmLteTimingInfo timingInfo;
#endif
{
   TRC2(rgSCHUtlHdlCrcFail);
   cell->sc.apis->rgSCHUlCrcInd(cell, ue, timingInfo);
   RETVOID;
} /* end of rgSCHUtlHdlCrcFailInd */

/**
 * @brief This API is invoked to indicate scheduler of a CRC failure.
 *
 * @details
 *
 *     Function : rgSCHUtlHdlCrcFailInd
 *      This API is invoked to indicate CRC failure to scheduler.
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  CmLteTimingInfo timingInfo
 *
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlHdlCrcFailInd
(
RgSchCellCb     *cell,
RgSchUeCb       *ue,
CmLteTimingInfo timingInfo
)
#else
PUBLIC Void rgSCHUtlHdlCrcFailInd(cell, ue, timingInfo)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
CmLteTimingInfo timingInfo;
#endif
{
   TRC2(rgSCHUtlHdlCrcFail);
   cell->sc.apis->rgSCHUlCrcFailInd(cell, ue, timingInfo);
   RETVOID;
} /* end of rgSCHUtlHdlCrcFailInd */
#endif /* LTEMAC_SPS */


/**
 * @brief This function is a wrapper to call scheduler specific API.
 *
 * @details
 *
 *     Function: rgSCHUtlDlProcAddToRetx
 *     Purpose:  This function adds a HARQ process to retransmission
 *               queue. This may be performed when a HARQ ack is
 *               unsuccessful.
 *
 *     Invoked by: HARQ feedback processing
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSchDlHqProc*   hqP
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlDlProcAddToRetx
(
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
)
#else
PUBLIC Void rgSCHUtlDlProcAddToRetx(cell, hqP)
RgSchCellCb                *cell;
RgSchDlHqProcCb            *hqP;
#endif
{
   TRC2(rgSCHUtlDlProcAddToRetx);
   cell->sc.apis->rgSCHDlProcAddToRetx(cell, hqP);
   RETVOID;
}


/**
 * @brief This function adds a HARQ process TB to transmission
 *
 * @details
 *
 *     Function: rgSCHUtlDlHqPTbAddToTx
 *     Purpose:  This function a HarqProcess TB to the slot
 *               list.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSubFrm*     subFrm
 *  @param[in]  RgDlHqProc*   hqP
 *  @param[in]  U8            tbIdx
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlDlHqPTbAddToTx
(
RgSchDlSf                  *subFrm,
RgSchDlHqProcCb            *hqP,
U8                         tbIdx
)
#else
PUBLIC Void rgSCHUtlDlHqPTbAddToTx(subFrm, hqP, tbIdx)
RgSchDlSf                  *subFrm;
RgSchDlHqProcCb            *hqP;
U8                         tbIdx;
#endif
{
   RgSchUeCb *ue = NULLP;
   RgSchCellCb *cell = hqP->hqE->cell;
   /* CA Dev Start */
   /* Addition of UE to dlSf->ueLst shall be done only to UE's PCell */
   /* ue->cell will always hold PCell information */
   if (NULLP == hqP->hqPSfLnk.node)
   {
      if (hqP->hqE->ue)
      {
         ue = hqP->hqE->ue;
         if(NULLP == ue->dl.dlSfHqInfo[cell->cellId][subFrm->dlIdx].dlSfUeLnk.node)
         {
            ue->dl.dlSfHqInfo[cell->cellId][subFrm->dlIdx].dlSfUeLnk.node = (PTR)ue;
            cmLListAdd2Tail(&cell->subFrms[subFrm->dlIdx]->ueLst, 
                  &ue->dl.dlSfHqInfo[cell->cellId][subFrm->dlIdx].dlSfUeLnk);

            ue->dl.dlSfHqInfo[cell->cellId][subFrm->dlIdx].isPuschHarqRecpPres = FALSE;

         }

         /* Add Hq proc in particular dlIdx List for this UE 
            This list will be used while processing feedback*/
         hqP->hqPSfLnk.node = (PTR)hqP;
         cmLListAdd2Tail(&ue->dl.dlSfHqInfo[cell->cellId][subFrm->dlIdx].hqPLst,&hqP->hqPSfLnk); 
#ifdef CA_DBG
         {
            extern U32 gSCellSchedCount,gPrimarySchedCount;
            if(RG_SCH_IS_CELL_SEC(hqP->hqE->ue,hqP->hqE->cell))
            {
               gSCellSchedCount++;
            }else
               gPrimarySchedCount++;
         }
#endif
      }
      else if (hqP->hqE->msg4Proc == hqP)
      {
         /* Msg4 will be scheduled on PCELL only hence add directly to subFrm msg4HqpList */
         hqP->hqPSfLnk.node = (PTR)hqP;
         cmLListAdd2Tail(&subFrm->msg4HqPLst, &hqP->hqPSfLnk);
      }
   }
   else
   {
      ue = hqP->hqE->ue; 
   }
   if((ue) && (HQ_TB_WAITING == hqP->tbInfo[tbIdx].state))
         
   {   
      ue->dl.dlSfHqInfo[cell->cellId][subFrm->dlIdx].totalTbCnt++;
   }
   /*totalTbCnt will hold the total number of TBs across all harq Proc from all 
    * cells*/

   hqP->subFrm = subFrm; 

   /* CA Dev End */
   RETVOID;
}



/**
 * @brief This function removes a HARQ process TB from transmission
 *
 * @details
 *
 *     Function: rgSCHUtlDlHqPTbRmvFrmTx
 *     Purpose:  This function removes a HarqProcess TB to the slot
 *               list.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSubFrm*     subFrm
 *  @param[in]  RgDlHqProc*   hqP
 *  @param[in]  U8            tbIdx
 *  @param[in]  Bool          isRepeting
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlDlHqPTbRmvFrmTx
(
RgSchDlSf                  *subFrm,
RgSchDlHqProcCb            *hqP,
U8                         tbIdx,
Bool                       isRepeting
)
#else
PUBLIC Void rgSCHUtlDlHqPTbRmvFrmTx(subFrm, hqP, tbIdx, isRepeting)
RgSchDlSf                  *subFrm;
RgSchDlHqProcCb            *hqP;
U8                         tbIdx;
Bool                       isRepeting;
#endif
{
   RgSchCellCb *cell = NULLP;
   /* Check with TDD */
   if ((isRepeting) &&
         (hqP->hqE->ue->ackNakRepCb.cfgRepCnt !=
          hqP->tbInfo[tbIdx].fbkRepCntr))
   {
      cmLListDelFrm(&subFrm->ackNakRepQ,
         &hqP->tbInfo[tbIdx].anRepLnk[hqP->tbInfo[tbIdx].fbkRepCntr]);
   }
   else
   {
      if (NULLP != hqP->hqPSfLnk.node)
      {
         /* CA dev Start */
         if (hqP->hqE->msg4Proc == hqP)
         {
            /* Msg4 will be scheduled on PCELL only hence delete directly from subFrm msg4HqpList */
            cmLListDelFrm(&subFrm->msg4HqPLst, &hqP->hqPSfLnk); 
         }
         else
         {
            cell = hqP->hqE->cell;
            /* Addition of UE to dlSf->ueLst shall be done only to UE's PCell */
            /* ue->cell will always hold PCell information */
            cmLListDelFrm(&hqP->hqE->ue->dl.dlSfHqInfo[cell->cellId][subFrm->dlIdx].hqPLst,&hqP->hqPSfLnk); 
            if (0 == hqP->hqE->ue->dl.dlSfHqInfo[cell->cellId][subFrm->dlIdx].hqPLst.count)
            {

               cmLListDelFrm(&cell->subFrms[subFrm->dlIdx]->ueLst, 
                     &hqP->hqE->ue->dl.dlSfHqInfo[cell->cellId][subFrm->dlIdx].dlSfUeLnk);
               hqP->hqE->ue->dl.dlSfHqInfo[cell->cellId][subFrm->dlIdx].dlSfUeLnk.node = (PTR)NULLP;
               hqP->hqE->ue->dl.dlSfHqInfo[cell->cellId][subFrm->dlIdx].totalTbCnt = 0;
            }
         }
         hqP->hqPSfLnk.node = NULLP;
      }
      hqP->subFrm = NULLP;
   }
   RETVOID;
}

#ifdef LTE_ADV
/**
 * @brief Handler for accessing the existing SCellCb identified by the key
 * SCellId under the CellCb.
 *
 * @details
 *
 *     Function : rgSchUtlGetCellCb
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  ueId
 *  @return  RgSchUeCb*
 **/
#ifdef ANSI
PUBLIC RgSchCellCb* rgSchUtlGetCellCb
(
 Inst         inst,
 U16          cellId
)
#else
PUBLIC RgSchCellCb* rgSchUtlGetCellCb(inst, cellId)
 Inst          inst;
 U16           cellId;
#endif
{
   RgSchCellCb    *cellCb = NULLP; 
   U8              strtCellId;

   TRC2(rgSchUtlGetCellCb);

   strtCellId = rgSchCb[inst].genCfg.startCellId;
   cellCb     = rgSchCb[inst].cells[cellId - strtCellId];

   RETVALUE(cellCb);

}  /* rgSchUtlGetCellCb */

/**
 * @brief Handler for deriving the servCellidx
 *
 * @details
 *
 *     Function : rgSchUtlGetServCellIdx
 *     
 *           
 *  @param[in]  *cellId
 *  @param[in]  RgSchUeCb *ue
 *  @return   U8 servCellIdx
 **/
#ifdef ANSI
PUBLIC U8 rgSchUtlGetServCellIdx
(
 Inst         inst,
 U16          cellId,
 RgSchUeCb    *ue
)
#else
PUBLIC U8 rgSchUtlGetServCellIdx(inst,cellId,ue)
 Inst         inst;
 U16          cellId;
 RgSchUeCb    *ue;
#endif
{
   U8 servCellIdx;
   U16 strtCellId;

   TRC2(rgSchUtlGetCellCb);

   strtCellId = rgSchCb[inst].genCfg.startCellId;

   servCellIdx = ue->cellIdToCellIdxMap[cellId - strtCellId];

   RETVALUE(servCellIdx);

}  /* rgSchUtlGetCellCb */

/**
 * @brief Handler for validating the Cell Id received secondary Cell Addition
 *
 * @details
 *
 *     Function : rgSchUtlGetCellId
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  ueId
 *  @return  RgSchUeCb*
 **/
#ifdef ANSI
PUBLIC S16 rgSchUtlVldtCellId
(
 Inst         inst,
 U16          cellId
)
#else
PUBLIC S16 rgSchUtlVldtCellId(inst, cellId)
 Inst          inst;
 U16           cellId;
#endif
{
   U8              strtCellId;

   TRC2(rgSchUtlVldtCellId);

   strtCellId = rgSchCb[inst].genCfg.startCellId;
   if((cellId >= strtCellId) && ((cellId - strtCellId) < CM_LTE_MAX_CELLS))
   {
      RETVALUE(ROK);
   }
   RETVALUE(RFAILED);
}  /* rgSchUtlVldtCellId */

#endif /* LTE_ADV*/
/**
 * @brief UE reconfiguration for scheduler
 *
 * @details
 *
 *     Function : rgSCHUtlRgrUeRecfg
 *
 *     This functions updates UE specific scheduler
 *     information upon UE reconfiguration
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[int] RgrUeRecfg   *ueRecfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrUeRecfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHUtlRgrUeRecfg(cell, ue, ueRecfg, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgrUeRecfg   *ueRecfg;
RgSchErrInfo *err;
#endif
{
/* Changes for UE Category Reconfiguration feature addition */
   RgSchCmnUe    *ueSch = RG_SCH_CMN_GET_UE(ue, cell);

   TRC2(rgSCHUtlRgrUeRecfg);

   /* Changes for UE Category Reconfiguration feature addition */
   if (ueRecfg->ueRecfgTypes & RGR_UE_UECAT_RECFG)
   {
      ueSch->cmn.ueCat = ueRecfg->ueCatEnum-1;
#ifdef TFU_UPGRADE
      ue->ueCatEnum = ueRecfg->ueCatEnum;
#endif
   } 

   /* DL MU-MIMO not supported */
   if (ueRecfg->ueRecfgTypes & RGR_UE_TXMODE_RECFG)
   {

      if (ueRecfg->txMode.pres == PRSNT_NODEF)
      {
         if (ueRecfg->txMode.txModeEnum == RGR_UE_TM_5)
         {
            err->errCause = RGSCHERR_SCH_CFG;
            RETVALUE(RFAILED);
         }
#ifdef LTE_ADV
        if(ue->mimoInfo.txMode != ueRecfg->txMode.txModeEnum)
         {
            /* Decremnt the previos A value for this cell */
            ue->f1bCsAVal -= rgSCHUtlGetMaxTbSupp(ue->mimoInfo.txMode);
            /* Update A value with the new TM Mode */
            ue->f1bCsAVal += rgSCHUtlGetMaxTbSupp(ueRecfg->txMode.txModeEnum);


            RLOG1(L_INFO,"UeReCfg A valie is %d\n",ue->f1bCsAVal);
         }
#endif
         ue->mimoInfo.txMode = ueRecfg->txMode.txModeEnum;
      }
   }
#ifdef TFU_UPGRADE
   /* [ccpu00123958]-ADD- Check for PUSCH related Reconfig from the bit mask  */
    if(ueRecfg->ueRecfgTypes & RGR_UE_PUSCH_RECFG)
    {
       /* Fix: ccpu00124012 */
       /* TODO:: Need to check if this is 
          mandatory to be re-configured on UE category re-configuration */
       /* ue->ul.betaHqOffst = ueRecfg->puschDedCfg.bACKIdx;
          ue->ul.betaCqiOffst = ueRecfg->puschDedCfg.bCQIIdx;
          ue->ul.betaRiOffst = ueRecfg->puschDedCfg.bRIIdx;*/
    }
#endif
   if (ueRecfg->ueRecfgTypes & RGR_UE_ULTXANTSEL_RECFG)
   {
      ue->ul.ulTxAntSel = ueRecfg->ulTxAntSel;
   }
   if (ueRecfg->ueRecfgTypes & RGR_UE_CDBKSBST_RECFG)
   {
      ue->mimoInfo.cdbkSbstRstrctn = ueRecfg->ueCodeBookRstRecfg;
   }

   /* Commenting here to assign garbage value when it is not set in APP. */
   //ue->accessStratumRls = ueRecfg->accessStratumRls;
   RETVALUE(cell->sc.apis->rgSCHRgrUeRecfg(cell, ue, ueRecfg, err));
}  /* rgSCHUtlRgrUeRecfg */

/**
 * @brief This function deletes a service from scheduler
 *
 * @details
 *
 *     Function: rgSCHUtlFreeDlLc
 *     Purpose:  This function is made available through a FP for
 *               making scheduler aware of a service being deleted from UE
 *
 *     Invoked by: BO and Scheduler
 *
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue
 *  @param[in]  RgSchDlLcCb*  svc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlFreeDlLc
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
)
#else
PUBLIC Void rgSCHUtlFreeDlLc(cell, ue, svc)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlLcCb                *svc;
#endif
{
   TRC2(rgSCHUtlFreeDlLc);
   cell->sc.apis->rgSCHFreeDlLc(cell, ue, svc);

  /* Stack Crash problem for TRACE5 changes. added the return below . */
 RETVOID;

}

/**
 * @brief UE deletion for scheduler
 *
 * @details
 *
 *     Function : rgSCHUtlFreeUe
 *
 *     This functions deletes all scheduler information
 *     pertaining to a UE
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlFreeUe
(
RgSchCellCb  *cell,
RgSchUeCb    *ue
)
#else
PUBLIC Void rgSCHUtlFreeUe(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
#endif
{
   TRC2(rgSCHUtlFreeUe);
#ifdef LTE_TDD
   rgSCHUtlDelUeANFdbkInfo(ue,RGSCH_PCELL_INDEX);
#endif
   cell->sc.apis->rgSCHFreeUe(cell, ue);

  /* Stack Crash problem for TRACE5 changes. added the return below . */
 RETVOID;

}  /* rgSCHUtlFreeUe */

/**
 * @brief This function updates the scheduler with service for a UE
 *
 * @details
 *
 *     Function: rgSCHUtlDlDedBoUpd
 *     Purpose:  This function should be called whenever there is a
 *               change BO for a service.
 *
 *     Invoked by: BO and Scheduler
 *
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue
 *  @param[in]  RgSchDlLcCb*  lc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlDlDedBoUpd
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *lc
)
#else
PUBLIC Void rgSCHUtlDlDedBoUpd(cell, ue, lc)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlLcCb                *lc;
#endif
{
   TRC2(rgSCHUtlDlDedBoUpd);
   cell->sc.apis->rgSCHDlDedBoUpd(cell, ue, lc);
   RETVOID;
}
/**
 * @brief Record MSG3 allocation into the UE
 *
 * @details
 *
 *     Function : rgSCHUtlRecMsg3Alloc
 *
 *     This function is invoked to update record msg3 allocation information
 *    in the UE when UE is detected for RaCb
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in] RgSchRaCb       *raCb
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlRecMsg3Alloc
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchRaCb        *raCb
)
#else
PUBLIC Void rgSCHUtlRecMsg3Alloc(cell, ue, raCb)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchRaCb        *raCb;
#endif
{
   TRC2(rgSCHUtlRecMsg3Alloc)
   cell->sc.apis->rgSCHUlRecMsg3Alloc(cell, ue, raCb);
   RETVOID;

}  /* rgSCHRecMsg3Alloc */

#ifdef RG_UNUSED
/**
 * @brief Update harq process for allocation
 *
 * @details
 *
 *     Function : rgSCHUtlUpdUlHqProc
 *
 *     This function is invoked when harq process
 *     control block is now in a new memory location
 *     thus requiring a pointer/reference update.
 *
 *  @param[in] RgSchCellCb      *cell
 *  @param[in] RgSchUlHqProcCb  *curProc
 *  @param[in] RgSchUlHqProcCb  *oldProc
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlUpdUlHqProc
(
RgSchCellCb      *cell,
RgSchUlHqProcCb  *curProc,
RgSchUlHqProcCb  *oldProc
)
#else
PUBLIC S16 rgSCHUtlUpdUlHqProc(cell, curProc, oldProc)
RgSchCellCb      *cell;
RgSchUlHqProcCb  *curProc;
RgSchUlHqProcCb  *oldProc;
#endif
{
   TRC2(rgSCHUtlUpdUlHqProc);
   RETVALUE(cell->sc.apis->rgSCHUpdUlHqProc(cell, curProc, oldProc));
}  /* rgSCHUtlUpdUlHqProc */
#endif
/**
 * @brief UL grant for contention resolution
 *
 * @details
 *
 *     Function : rgSCHUtlContResUlGrant
 *
 *     Add UE to another queue specifically for CRNTI based contention
 *     resolution
 *
 *  @param[in]  RgSchCellCb *cell
 *  @param[in]  RgSchUeCb  *ue
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlContResUlGrant
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHUtlContResUlGrant(cell, ue, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchErrInfo *err;
#endif
{
   TRC2(rgSCHUtlContResUlGrant);


   ue->isMsg4PdcchWithCrnti = TRUE; 

   RETVALUE(cell->sc.apis->rgSCHContResUlGrant(cell, ue, err));
}  /* rgSCHUtlContResUlGrant */

/**
 * @brief SR reception handling
 *
 * @details
 *
 *     Function : rgSCHUtlSrRcvd
 *
 *     - Handles SR reception for UE
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlSrRcvd
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
CmLteTimingInfo frm,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHUtlSrRcvd(cell, ue, frm, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
CmLteTimingInfo frm;
RgSchErrInfo *err;
#endif
{
   TRC2(rgSCHUtlSrRcvd);
   RETVALUE(cell->sc.apis->rgSCHSrRcvd(cell, ue, frm, err));
}  /* rgSCHUtlSrRcvd */

/**
 * @brief Short BSR update
 *
 * @details
 *
 *     Function : rgSCHUtlUpdBsrShort
 *
 *     This functions does requisite updates to handle short BSR reporting
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  U8           lcgId
 *  @param[in]  U8           bsr
 *  @param[out] RgSchErrInfo *err
 *  @return  Void
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlUpdBsrShort
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
U8           lcgId,
U8           bsr,
RgSchErrInfo *err
)
#else
PUBLIC Void rgSCHUtlUpdBsrShort(cell, ue, lcgId, bsr, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
U8           lcgId;
U8           bsr;
RgSchErrInfo *err;
#endif
{
   TRC2(rgSCHUtlUpdBsrShort);
   cell->sc.apis->rgSCHUpdBsrShort(cell, ue, &ue->ul.lcgArr[lcgId], bsr, err);
   RETVOID;
}  /* rgSCHUtlUpdBsrShort */


/**
 * @brief Truncated BSR update
 *
 * @details
 *
 *     Function : rgSCHUtlUpdBsrTrunc
 *
 *     This functions does required updates to handle truncated BSR report
 *
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  U8           lcgId
 *  @param[in]  U8           bsr
 *  @param[out] RgSchErrInfo *err
 *  @return  Void
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlUpdBsrTrunc
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
U8           lcgId,
U8           bsr,
RgSchErrInfo *err
)
#else
PUBLIC Void rgSCHUtlUpdBsrTrunc(cell, ue, lcgId, bsr, err)
RgSchCellCb   *cell;
RgSchUeCb     *ue;
U8            lcgId;
U8            bsr;
RgSchErrInfo *err;
#endif
{
   TRC2(rgSCHUtlUpdBsrTrunc);
   cell->sc.apis->rgSCHUpdBsrTrunc(cell, ue, &ue->ul.lcgArr[lcgId], bsr, err);
   RETVOID; 
}  /* rgSCHUtlUpdBsrTrunc */


/**
 * @brief Long BSR update
 *
 * @details
 *
 *     Function : rgSCHUtlUpdBsrLong
 *
 *     - Update BSRs for all configured LCGs
 *     - Update priority of LCGs if needed
 *     - Update UE's position within/across uplink scheduling queues
 *
 *
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgSchUeCb     *ue
 *  @param[in]  U8            bsr0
 *  @param[in]  U8            bsr1
 *  @param[in]  U8            bsr2
 *  @param[in]  U8            bsr3
 *  @param[out] RgSchErrInfo  *err
 *  @return  Void
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlUpdBsrLong
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
U8           bsr0,
U8           bsr1,
U8           bsr2,
U8           bsr3,
RgSchErrInfo *err
)
#else
PUBLIC Void rgSCHUtlUpdBsrLong(cell, ue, bsr0, bsr1, bsr2, bsr3, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
U8           bsr0;
U8           bsr1;
U8           bsr2;
U8           bsr3;
RgSchErrInfo *err;
#endif
{
   U8 bsArr[4];
   TRC2(rgSCHUtlUpdBsrLong);

   bsArr[0] = bsr0;
   bsArr[1] = bsr1;
   bsArr[2] = bsr2;
   bsArr[3] = bsr3;
   cell->sc.apis->rgSCHUpdBsrLong(cell, ue, bsArr, err);
   RETVOID;
}  /* rgSCHUtlUpdBsrLong */

/**
 * @brief EXT PHR update
 *
 * @details
 *
 *     Function : rgSCHUtlUpdExtPhr
 *
 *     Updates extended power headroom info for a UE
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  U8           phr
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlUpdExtPhr
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgInfExtPhrCEInfo * extPhr,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHUtlUpdExtPhr(cell, ue, extPhr, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgInfExtPhrCEInfo * extPhr;
RgSchErrInfo *err;
#endif
{
   TRC2(rgSCHUtlUpdExtPhr);
   RETVALUE(cell->sc.apis->rgSCHUpdExtPhr(cell, ue, extPhr, err));
}  /* rgSCHUtlUpdExtPhr */



/**
 * @brief PHR update
 *
 * @details
 *
 *     Function : rgSCHUtlUpdPhr
 *
 *     Updates power headroom info for a UE
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  U8           phr
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlUpdPhr
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
U8           phr,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHUtlUpdPhr(cell, ue, phr, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
U8           phr;
RgSchErrInfo *err;
#endif
{
   TRC2(rgSCHUtlUpdPhr);
   RETVALUE(cell->sc.apis->rgSCHUpdPhr(cell, ue, phr, err));
}  /* rgSCHUtlUpdPhr */


/**
 * @brief Indication of UL CQI
 *
 * @details
 *
 *     Function : rgSCHUtlUlCqiInd
 *
 *     - Updates uplink CQI information for the UE. Computes and
 *       stores the lowest CQI of CQIs reported in all subbands
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]  TfuUlCqiRpt         *ulCqiInfo
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlCqiInd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
TfuUlCqiRpt          *ulCqiInfo
)
#else
PUBLIC Void rgSCHUtlUlCqiInd(cell, ue, ulCqiInfo)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
TfuUlCqiRpt          *ulCqiInfo;
#endif
{
   TRC2(rgSCHUtlUlCqiInd);
   cell->sc.apis->rgSCHUlCqiInd(cell, ue, ulCqiInfo);
   RETVOID;
}  /* rgSCHUtlUlCqiInd */

/**
 * @brief Indication of PUCCH power adjustment
 *
 * @details
 *
 *     Function : rgSCHUtlPucchDeltaPwrInd
 *
 *     - Updates uplink CQI information for the UE. Computes and
 *       stores the lowest CQI of CQIs reported in all subbands
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]  U8                  delta
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlPucchDeltaPwrInd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
S8                   delta
)
#else
PUBLIC Void rgSCHUtlPucchDeltaPwrInd(cell, ue, delta)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
S8                   delta;
#endif
{
   TRC2(rgSCHUtlPucchDeltaPwrInd);
   cell->sc.apis->rgSCHPucchDeltaPwrInd(cell, ue, delta);
   RETVOID;
}  /* rgSCHUtlPucchDeltaPwrInd */

/* Start: LTEMAC_2.1_DEV_CFG */
/**
 * @brief Ue Reset Request
 *
 * @details
 *
 *     Function : rgSCHUtlUeReset
 *
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @return  S16
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlUeReset
(
RgSchCellCb          *cell,
RgSchUeCb            *ue
)
#else
PUBLIC Void rgSCHUtlUeReset(cell, ue)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
#endif
{
   TRC2(rgSCHUtlUeReset);
   ue->remBoCnt = 0;
   cell->sc.apis->rgSCHUeReset(cell, ue);
   RETVOID;
}  /* rgSCHUtlUeReset */
/* End: LTEMAC_2.1_DEV_CFG */

/**
 * @brief Returns HARQ proc for which data expected now
 *
 * @details
 *
 *     Function: rgSCHUtlUlHqProcForUe
 *     Purpose:  This function returns the harq process for
 *               which data is expected in the current slot.
 *               It does not validate if the HARQ process
 *               has an allocation.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  CmLteTimingInfo     frm
 *  @param[in]  RgSchUeCb           *ue
 *  @param[out] RgSchUlHqProcCb     **procRef
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHqProcForUe
(
RgSchCellCb         *cell,
CmLteTimingInfo     frm,
RgSchUeCb           *ue,
RgSchUlHqProcCb     **procRef
)
#else
PUBLIC Void rgSCHUtlUlHqProcForUe(cell, frm, ue, procRef)
RgSchCellCb         *cell;
CmLteTimingInfo     frm;
RgSchUeCb           *ue;
RgSchUlHqProcCb     **procRef;
#endif
{
   TRC2(rgSCHUtlUlHqProcForUe);
   cell->sc.apis->rgSCHUlHqProcForUe(cell, frm, ue, procRef);

 /* Stack Crash problems for TRACE5 changes. added the return below */
 RETVOID;

}

/**
 * @brief Returns first uplink allocation to send reception
 *        request to PHY
 *
 * @details
 *
 *     Function: rgSCHUtlFirstRcptnReq(cell)
 *     Purpose:  This function returns the first uplink allocation
 *               (or NULLP if there is none) in the slot
 *               in which is expected to prepare and send reception
 *               request to PHY.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @return  RgSchUlAlloc*
 **/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlFirstRcptnReq
(
RgSchCellCb      *cell
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlFirstRcptnReq(cell)
RgSchCellCb      *cell;
#endif
{
   TRC2(rgSCHUtlFirstRcptnReq);
   RETVALUE(cell->sc.apis->rgSCHFirstRcptnReq(cell));
}

/**
 * @brief Returns first uplink allocation to send reception
 *        request to PHY
 *
 * @details
 *
 *     Function: rgSCHUtlNextRcptnReq(cell)
 *     Purpose:  This function returns the next uplink allocation
 *               (or NULLP if there is none) in the slot
 *               in which is expected to prepare and send reception
 *               request to PHY.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @return  RgSchUlAlloc*
 **/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlNextRcptnReq
(
RgSchCellCb      *cell,
RgSchUlAlloc     *alloc
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlNextRcptnReq(cell, alloc)
RgSchCellCb      *cell;
RgSchUlAlloc     *alloc;
#endif
{
   TRC2(rgSCHUtlNextRcptnReq);
   RETVALUE(cell->sc.apis->rgSCHNextRcptnReq(cell, alloc));
}

/**
 * @brief Returns first uplink allocation to send HARQ feedback
 *        request to PHY
 *
 * @details
 *
 *     Function: rgSCHUtlFirstHqFdbkAlloc
 *     Purpose:  This function returns the first uplink allocation
 *               (or NULLP if there is none) in the slot
 *               in which it is expected to prepare and send HARQ
 *               feedback to PHY.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  U8                idx  
 *  @return  RgSchUlAlloc*
 **/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlFirstHqFdbkAlloc
(
RgSchCellCb      *cell,
U8               idx
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlFirstHqFdbkAlloc(cell, idx)
RgSchCellCb      *cell;
U8               idx;
#endif
{
   TRC2(rgSCHUtlFirstHqFdbkAlloc);
   RETVALUE(cell->sc.apis->rgSCHFirstHqFdbkAlloc(cell, idx));
}


/**
 * @brief Returns next allocation to send HARQ feedback for
 *
 * @details
 *
 *     Function: rgSCHUtlNextHqFdbkAlloc(cell)
 *     Purpose:  This function returns the next uplink allocation
 *               (or NULLP if there is none) in the slot
 *               for which HARQ feedback needs to be sent.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @return  RgSchUlAlloc*
 **/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlNextHqFdbkAlloc
(
RgSchCellCb      *cell,
RgSchUlAlloc     *alloc,
U8               idx 
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlNextHqFdbkAlloc(cell, alloc, idx)
RgSchCellCb      *cell;
RgSchUlAlloc     *alloc;
U8               idx; 
#endif
{
   TRC2(rgSCHUtlNextHqFdbkAlloc);
   RETVALUE(cell->sc.apis->rgSCHNextHqFdbkAlloc(cell, alloc, idx));
}

/***********************************
***********************************/
/**
 * @brief This API is invoked to send TFU SAP bind request to PHY.
 *
 * @details
 *
 *     Function : rgSCHUtlTfuBndReq
 *
 *      This API is invoked to send TFU SAP bind request to PHY from scheduler
 *      isntance. It fills in the Pst structure, spId and suId values and
 *      invokes bind request primitive at TFU.
 *
 *  @param[in]  Inst            instId
 *  @param[in]  SuId            suId
 *  @param[in]  SpId            spId
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlTfuBndReq
(
Inst    instId,
SuId    suId,
SpId    spId
)
#else
PUBLIC S16 rgSCHUtlTfuBndReq(instId, suId, spId)
Inst    instId;
SuId    suId;
SpId    spId;
#endif
{
   S16            ret;
   RgSchLowSapCb  *tfuSap;
   Pst            pst;
   TRC2(rgSCHUtlTfuBndReq);

   /* Get the lower SAP control block from the layer control block. */
   tfuSap = &(rgSchCb[instId].tfuSap[suId]);
   (Void)cmMemcpy ((U8*)&pst, (U8*)&(tfuSap->sapCfg.sapPst), sizeof(Pst));
   if((ret = RgLiTfuSchBndReq (&pst, suId, spId)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,instId,"rgSCHUtlTfuBndReq() Call to RgLiTfuBndReq()"
                    " failed");
   }
   RETVALUE(ret);
}  /* rgSCHUtlTfuBndReq */

/**
 * @brief This API is invoked to send TFU SAP unbind request to PHY.
 *
 * @details
 *
 *     Function : rgSCHUtlTfuUBndReq
 *      This API is invoked to send TFU SAP unbind request to PHY from Scheduler
 *      isntance. It fills in the Pst structure and spId value and invokes
 *      unbind request primitive at TFU.
 *
 *  @param[in]  SpId            spId
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlTfuUBndReq
(
Inst    inst,
RgSchLowSapCfgInfo sapCfg,
Reason  reason
)
#else
PUBLIC S16 rgSCHUtlTfuUBndReq(inst, sapCfg, reason)
Inst    inst;
RgSchLowSapCfgInfo sapCfg;
Reason  reason;
#endif
{
   S16            ret;
   Pst            pst;

   TRC2(rgSCHUtlTfuUBndReq);

   /* Get the lower SAP control block from the layer control block. */
   cmMemcpy ((U8*)&pst, (U8*)&(sapCfg.sapPst), sizeof(Pst));
   if((ret = RgLiTfuSchUbndReq (&pst, sapCfg.spId, reason)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"rgSCHUtlTfuUBndReq() Call to"
                                    " RgLiTfuUbndReq() failed");
   }
   RETVALUE(ret);

}  /* rgSCHUtlTfuUBndReq */

/***********************************************************
 *
 *     Func : rgSCHUtlResetSfAlloc
 *
 *     Desc : Utility Function to Reset slot allocation information.
 *
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHUtlResetSfAlloc
(
RgInfSfAlloc *sfAlloc,
Bool         resetCmnLcInfo,
Bool         restAlloc
)
#else
PUBLIC S16 rgSCHUtlResetSfAlloc(sfAlloc,resetCmnLcInfo,restAlloc)
RgInfSfAlloc *sfAlloc;
Bool         resetCmnLcInfo;
Bool         restAlloc;
#endif
{
   TRC2(rgSCHUtlResetSfAlloc);
   if(TRUE == restAlloc)
   {
      if(sfAlloc->ueInfo.numUes)
      {
         cmMemset((U8 *)sfAlloc->ueInfo.allocInfo,0x00,
               (sizeof(RgInfUeAlloc)*sfAlloc->ueInfo.numUes));
      }
      sfAlloc->ueInfo.numUes = 0;
      sfAlloc->rarInfo.numRaRntis = 0;
      sfAlloc->flowCntrlInfo.numUes = 0;
   }
   if(TRUE == resetCmnLcInfo)
   {
      sfAlloc->cmnLcInfo.bitMask = 0;
   }
   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHUtlGetRlsHqAlloc
 *
 *     Desc : Utility Function to Allocate slot allocation information.
 *
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHUtlGetRlsHqAlloc
(
RgSchCellCb *cell
)
#else
PUBLIC S16 rgSCHUtlGetRlsHqAlloc(cell)
RgSchCellCb *cell;
#endif
{
   U8    idx = 0;
   Inst  inst = cell->instIdx;
   TRC2(rgSCHUtlGetRlsHqAlloc);
   for(idx=0; idx < RGSCH_NUM_SUB_FRAMES; idx++)
   {
      cell->rlsHqArr[idx].cellId = cell->cellId;

      /* Allocating with additional location, to accommodate
         TA scheduling along with maximum no of UEs per SF */

      /* Allocate memory for "scheduled UE" Info */
      if((rgSCHUtlAllocSBuf(inst,
                     (Data**)&(cell->rlsHqArr[idx].ueHqInfo),
                      (sizeof(RgInfUeHqInfo)*RGSCH_MAX_UE_PER_DL_SF))) != ROK)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Memory allocation FAILED for "
            "UE Alloc");
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);

}

/***********************************************************
 *
 *     Func : rgSCHUtlPutRlsHqAlloc
 *
 *     Desc : Utility Function to deallocate slot allocation information.
 *
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHUtlPutRlsHqAlloc
(
RgSchCellCb *cell
)
#else
PUBLIC S16 rgSCHUtlPutRlsHqAlloc(cell)
RgSchCellCb *cell;
#endif
{
   U8    idx = 0;
   Inst  inst = cell->instIdx;
   TRC2(rgSCHUtlPutRlsHqAlloc);

   for(idx=0; idx < RGSCH_NUM_SUB_FRAMES; idx++)
   {
      /* Deallocate memory for "scheduled UE" Info */
      if (cell->rlsHqArr[idx].ueHqInfo != NULLP)
      {
         /* Freeing with additional location, to accommodate TA
            scheduling along with maximum no of UEs per SF */
         /* ccpu00117052 - MOD - Passing double pointer
            for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(inst,
               (Data **)(&(cell->rlsHqArr[idx].ueHqInfo)),
             (sizeof(RgInfUeHqInfo)*RGSCH_MAX_UE_PER_DL_SF));
      }
   }

   RETVALUE(ROK);

}


/***********************************************************
 *
 *     Func : rgSCHUtlGetSfAlloc
 *
 *     Desc : Utility Function to Allocate slot allocation information.
 *
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHUtlGetSfAlloc
(
RgSchCellCb *cell
)
#else
PUBLIC S16 rgSCHUtlGetSfAlloc(cell)
RgSchCellCb *cell;
#endif
{
   U8    idx;
   U8    indx;
   Inst  inst = cell->instIdx;
   RgSchCmnUlCell *cellUl      = RG_SCH_CMN_GET_UL_CELL(cell);
   TRC2(rgSCHUtlGetSfAlloc);

#ifdef LTE_TDD
   for(idx=0; idx < RGSCH_SF_ALLOC_SIZE; idx++)
#else
   for(idx=0; idx < RGSCH_NUM_SUB_FRAMES; idx++)
#endif
   {
      cell->sfAllocArr[idx].cellId = cell->cellId;

      /* Allocating with additional location, to accommodate
         TA scheduling along with maximum no of UEs per SF */

      /* Allocate memory for "scheduled UE" Info */
      if((rgSCHUtlAllocSBuf(inst,
                     (Data**)&(cell->sfAllocArr[idx].ueInfo.allocInfo),
                      (sizeof(RgInfUeAlloc)*RGSCH_MAX_UE_PER_DL_SF))) != ROK)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Memory allocation FAILED for "
            "UE Alloc");
         RETVALUE(RFAILED);
      }

      /* Allocate memory for "scheduled RAR" Info */
      if((rgSCHUtlAllocSBuf(inst,
                     (Data**)&(cell->sfAllocArr[idx].rarInfo.raRntiInfo),
                      (sizeof(RgInfRaRntiInfo)*RGSCH_MAX_RARNTI_PER_DL_SF))) != ROK)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Memory allocation FAILED for "
            "RARNTI");
         RETVALUE(RFAILED);
      }
      for(indx = 0; indx < RGSCH_MAX_RARNTI_PER_DL_SF; indx++)
      {
         if((rgSCHUtlAllocSBuf(inst,
            (Data**)&(cell->sfAllocArr[idx].rarInfo.raRntiInfo[indx].crntiInfo),
                   (sizeof(RgInfCrntiInfo)* (cellUl->maxMsg3PerUlSf)))) != ROK)
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Memory allocation FAILED for "
               "RNTI");
            RETVALUE(RFAILED);
         }
      }

   }

#ifdef EMTC_ENABLE
   rgSCHEmtcUtlGetSfAlloc(cell);
#endif

   RETVALUE(ROK);

}

/***********************************************************
 *
 *     Func : rgSCHUtlPutSfAlloc
 *
 *     Desc : Utility Function to deallocate slot allocation information.
 *
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHUtlPutSfAlloc
(
RgSchCellCb *cell
)
#else
PUBLIC S16 rgSCHUtlPutSfAlloc(cell)
RgSchCellCb *cell;
#endif
{
   U8    idx;
   U8    indx;
   Inst  inst = cell->instIdx;
   RgSchCmnUlCell *cellUl      = RG_SCH_CMN_GET_UL_CELL(cell);
   TRC2(rgSCHUtlPutSfAlloc);

#ifdef LTE_TDD
   for(idx=0; idx < RGSCH_SF_ALLOC_SIZE; idx++)
#else
      for(idx=0; idx < RGSCH_NUM_SUB_FRAMES; idx++)
#endif
      {
         if (cell->sfAllocArr[idx].rarInfo.raRntiInfo != NULLP)
         {
            for(indx = 0; indx < RGSCH_MAX_RARNTI_PER_DL_SF; indx++)
            {
               if (cell->sfAllocArr[idx].rarInfo.raRntiInfo[indx].crntiInfo != NULLP)
                  /* ccpu00117052 - MOD - Passing double pointer
                     for proper NULLP assignment*/
                  rgSCHUtlFreeSBuf(inst,
                        (Data**)(&(cell->sfAllocArr[idx].rarInfo.raRntiInfo[indx].\
                              crntiInfo)),
                        (sizeof(RgInfCrntiInfo)* (cellUl->maxMsg3PerUlSf)));
            }
            /* Deallocate memory for "scheduled RAR" Info */
            /* ccpu00117052 - MOD - Passing double pointer
               for proper NULLP assignment*/
            rgSCHUtlFreeSBuf(inst,
                  (Data**)(&(cell->sfAllocArr[idx].rarInfo.raRntiInfo)),
                  (sizeof(RgInfRaRntiInfo)*RGSCH_MAX_RARNTI_PER_DL_SF));
         }
         /* Deallocate memory for "scheduled UE" Info */
         if (cell->sfAllocArr[idx].ueInfo.allocInfo != NULLP)
         {
            /* Freeing with additional location, to accommodate TA
               scheduling along with maximum no of UEs per SF */
            /* ccpu00117052 - MOD - Passing double pointer
               for proper NULLP assignment*/
            rgSCHUtlFreeSBuf(inst,
                  (Data**)(&(cell->sfAllocArr[idx].ueInfo.allocInfo)),
                  (sizeof(RgInfUeAlloc)*RGSCH_MAX_UE_PER_DL_SF));
         }
      }

#ifdef EMTC_ENABLE
   rgSCHEmtcUtlPutSfAlloc(cell);
#endif
   RETVALUE(ROK);

}

/***********************************************************
 *
 *     Func : rgSCHUtlAllocSBuf
 *
 *     Desc : Utility Function to Allocate static buffer.
 *            Memory allocated is assumed contiguous.
 *
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: Caller doesnt need to raise the alarm in case of memory
 *            allocation gets failed.
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHUtlAllocSBuf
(
Inst    inst,               /* Instance of the invoking scheduler */
Data    **pData,            /* Pointer of the data to be returned */
Size    size                /* size */
)
#else
PUBLIC S16 rgSCHUtlAllocSBuf(inst, pData, size)
Inst    inst;              /* Instance of the invoking scheduler */
Data    **pData;            /* Pointer of the data to be returned */
Size    size;               /* size */
#endif
{
   /* Moving alarm diagnostics to available scope */

   TRC2(rgSCHUtlAllocSBuf)

   /* Initialize the param to NULLP */
   *pData = NULLP;

   /* May not be necessary for data performance path */
#ifndef NO_ERRCLS
   if (size == 0)
   {
      RETVALUE(RFAILED);
   }
#endif

   /* allocate buffer */
#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
   MS_BUF_ADD_ALLOC_CALLER();
#endif /* */
   if (SGetSBuf(rgSchCb[inst].rgSchInit.region, rgSchCb[inst].rgSchInit.pool,
                      pData, size) != ROK)
   {
     RgUstaDgn dgn;      /* Alarm diagnostics structure */
     dgn.type = LRG_USTA_DGNVAL_MEM;
     dgn.u.mem.region = rgSchCb[inst].rgSchInit.region;
     dgn.u.mem.pool = rgSchCb[inst].rgSchInit.pool;
     /*  Send an alarm to Layer Manager */
     rgSCHLmmStaInd(inst, LCM_CATEGORY_RESOURCE, LCM_EVENT_SMEM_ALLOC_FAIL,
                                       LCM_CAUSE_MEM_ALLOC_FAIL, &dgn);
     RGSCHLOGERROR(inst, ERRCLS_DEBUG, ERG015, 0, "Unable to Allocate Buffer");
     RLOG_ARG0(L_ERROR,DBG_INSTID,inst, "Unable to Allocate the Buffer");
     RETVALUE(RFAILED);
   }


   /* zero out the allocated memory */
   cmMemset((U8 *)*pData, 0x00, size);

   RETVALUE(ROK);

} /* end of rgSCHUtlAllocSBuf */


/*
*
*       Fun:   rgSCHUtlFreeSBuf
*
*       Desc:  The argument to rgSCHUtlFreeSBuf() is a pointer to a block
*              previously allocated by rgSCHUtlAllocSBuf() and size. It
*              deallocates the memory.
*
*       Ret:   RETVOID
*
*       Notes: None
*       File:  rg_utl.c
*/
#ifdef ANSI
PUBLIC Void rgSCHUtlFreeSBuf
(
Inst inst,          /* Instance of the invoking scheduler */
Data **data,         /* pointer to data */
Size size           /* size */
)
#else
PUBLIC Void rgSCHUtlFreeSBuf(inst, data, size)
Inst inst;          /* Instance of the invoking scheduler */
Data **data;         /* pointer to data */
Size size;          /* size */
#endif
{

   S16 ret;

   TRC2(rgSCHUtlFreeSBuf)

   if ((data == NULLP) || (*data == NULLP) || (size == 0))
   {
      RETVOID;
   }


#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
   MS_BUF_ADD_CALLER();
#endif /* */
   /* Deallocate buffer */
   ret = SPutSBuf(rgSchCb[inst].rgSchInit.region,
                  rgSchCb[inst].rgSchInit.pool, (*data), size);

   if (ret != ROK)
   {
      RGSCHLOGERROR(inst, ERRCLS_DEBUG, ERG016, (ErrVal) 0,
                 "rgSCHUtlFreeSBuf failed.\n");
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst, "rgSCHUtlFreeSBuf failed");
      RETVOID;
   }

   /* ccpu00117052 - ADD - Assigning the pointer to NULLP */
   *data = NULLP;

   RETVOID;
} /* end of rgSCHUtlFreeSBuf */


#ifdef RGR_SI_SCH
/*
*
*       Fun:   rgSCHUtlFreeWarningSiSeg
*
*       Desc:  This is used to deallocate Warning SI Seg.
*
*       Ret:   RETVOID
*
*       Notes: None
*
*       File:  rg_utl.c
*/
#ifdef ANSI
PUBLIC Void rgSCHUtlFreeWarningSiSeg
(
Region        reg,
Pool          pool,
CmLListCp     *siPduLst          
)
#else
PUBLIC Void rgSCHUtlFreeWarningSiSeg(reg, pool, siPduLst)
Region        reg;
Pool          pool;
CmLListCp     *siPduLst;          
#endif
{
   CmLList      *node;
   Buffer       *pdu;

   TRC2(rgSCHUtlFreeWarningSiSeg)

      while (siPduLst->first != NULLP)
      {
         node = siPduLst->first;
         pdu = (Buffer *)node->node;
         cmLListDelFrm(siPduLst, node);
         RGSCH_FREE_MSG(pdu);
         SPutSBuf(reg, pool, (Data *)node,sizeof(CmLList));
         node = NULLP;
      }

   RETVOID;
} /* end of rgSCHUtlFreeWarningSiSeg */         


/*
*
*       Fun:   rgSCHUtlFreeWarningSiPdu
*
*       Desc:  This is used to deallocate Warning SI PDU.
*
*       Ret:   RETVOID
*
*       Notes: None
*
*       File:  rg_utl.c
*/
#ifdef ANSI
PUBLIC Void rgSCHUtlFreeWarningSiPdu
(
RgSchCellCb       *cell
)
#else
PUBLIC Void rgSCHUtlFreeWarningSiPdu(cell)
RgSchCellCb       *cell;
#endif
{
   CmLList            *node;
   Buffer             *pdu;
   RgSchWarningSiInfo *warningSi;
   RgSchWarningSiPdu  *warningSiPdu; 

   TRC2(rgSCHUtlFreeWarningSiPdu)

   warningSi = (RgSchWarningSiInfo *) cell->siCb.\
                  siArray[cell->siCb.siCtx.siId-1].si; 
   /* ccpu00136659: CMAS ETWS design changes */
   CM_LLIST_FIRST_NODE(&warningSi->warningSiMsg.segLstCp, node);
   if (node == NULLP)
   {
      RETVOID;
   }

   warningSiPdu = (RgSchWarningSiPdu *)node->node;
   pdu = warningSiPdu->pdu;
   /* ccpu00136659: CMAS ETWS design changes */
   cmLListDelFrm(&warningSi->warningSiMsg.segLstCp, node); 
   RGSCH_FREE_MSG(pdu);
   if(warningSi->warningSiMsg.segLstCp.count == 0)
   {
      /* ccpu00136659: CMAS ETWS design changes */
      cell->siCb.siArray[cell->siCb.siCtx.siId-1].si = NULLP;
      rgSCHUtlRgrWarningSiCfgCfm(cell->instIdx,
            rgSchCb[cell->instIdx].rgrSap->sapCfg.spId,
            cell->siCb.warningSi[warningSi->idx].siId,
            warningSi->warningSiMsg.transId, RGR_CFG_CFM_TX_COMPLETE);
   }                                                                                 

   RETVOID;

} /* end of rgSCHUtlFreeWarningSiPdu */         
 

/*
*
*       Fun:   rgSCHUtlGetWarningSiPdu
*
*       Desc:  This is used to get Warning SI PDU for Scheduling. 
*
*       Ret:   
*
*       Notes: None
*
*       File:  rg_utl.c
*/
#ifdef ANSI
PUBLIC Buffer *rgSCHUtlGetWarningSiPdu
(
RgSchCellCb             *cell
)
#else
PUBLIC Buffer *rgSCHUtlGetWarningSiPdu(cell)
RgSchCellCb             *cell;
#endif
{
   RgSchWarningSiInfo  *warningSi;
   RgSchWarningSiPdu  *warningSiPdu; 
   Buffer       *pdu;
   CmLList      *node;

   TRC2(rgSCHUtlGetWarningSiPdu)

      warningSi = (RgSchWarningSiInfo *) cell->siCb.
      siArray[cell->siCb.siCtx.siId-1].si; 
   /* ccpu00136659: CMAS ETWS design changes */
   CM_LLIST_FIRST_NODE(&warningSi->warningSiMsg.segLstCp, node);
   if (node != NULLP)
   {
      warningSiPdu = (RgSchWarningSiPdu *)node->node;
      pdu = warningSiPdu->pdu;
      RETVALUE(pdu);
   }
   else
   {
      RETVALUE(NULLP);
   }                                                  
} /* rgSCHUtlGetWarningSiPdu  */    


/*
*
*       Fun:   rgSCHUtlGetMcsAndNPrb
*
*       Desc:  This is used to get mcs and nPrb value. 
*
*       Ret:   
*
*       Notes: None
*
*       File:  rg_utl.c
*/
#ifdef ANSI
PUBLIC S16 rgSCHUtlGetMcsAndNPrb
(
RgSchCellCb         *cell,
U8                  *nPrb,
U8                  *mcs,
MsgLen              *msgLen
)
#else
PUBLIC S16 rgSCHUtlGetMcsAndNPrb(cell, nPrb, mcs, msgLen)
RgSchCellCb         *cell;
U8                  *nPrb;
U8                  *mcs;
MsgLen              *msgLen;
#endif
{
   RgSchWarningSiInfo  *warningSi;
   RgSchWarningSiPdu  *warningSiPdu; 
   CmLList      *node;

   TRC2(rgSCHUtlGetMcsAndNPrb)

   if(cell->siCb.siCtx.warningSiFlag == FALSE)
   {
      *mcs =  cell->siCb.crntSiInfo.siInfo[cell->siCb.siCtx.siId-1].mcs;
      *nPrb =  cell->siCb.crntSiInfo.siInfo[cell->siCb.siCtx.siId-1].nPrb;
      *msgLen = cell->siCb.crntSiInfo.siInfo[cell->siCb.siCtx.siId-1].msgLen;
   }
   else
   {
      warningSi = (RgSchWarningSiInfo *) cell->siCb.
            siArray[cell->siCb.siCtx.siId-1].si; 
      /* ccpu00136659: CMAS ETWS design changes */
      CM_LLIST_FIRST_NODE(&warningSi->warningSiMsg.segLstCp, node);
      if (node == NULLP)
      {
        RETVALUE(RFAILED);
      }
      
      warningSiPdu = (RgSchWarningSiPdu *)node->node;
      *mcs = warningSiPdu->mcs;
      *nPrb = warningSiPdu->nPrb;
      *msgLen = warningSiPdu->msgLen;
      RETVALUE(ROK);
            
   }
   RETVALUE(ROK);
} /* rgSCHUtlGetMcsAndNPrb  */     

/*
*
*       Fun:   rgSCHUtlCalMacAndPrb
*
*       Desc:  This is used to Calculate mcs and nPrb value for SIB1 and SIs. 
*
*       Ret:   
*
*       Notes: None
*
*       File:  rg_utl.c
*/
#ifdef ANSI
PUBLIC S16 rgSCHUtlCalMcsAndNPrb
(
RgSchCellCb         *cell,
U8                  cfgType,
MsgLen              msgLen,
U8                  siId
)
#else
PUBLIC S16 rgSCHUtlCalMcsAndNPrb(cell, nPrb, mcs, msgLen)
RgSchCellCb         *cell;
U8                  cfgType;
MsgLen              msgLen;
U8                  siId;
#endif
{
   U8 mcs = 0;
   U8 nPrb = 0;

   TRC2(rgSCHUtlCalMcsAndNPrb)    

      /*Get the nPrb and mcs parametr values */
   if (rgSCHUtlGetAllwdCchTbSz(msgLen*8, &nPrb, &mcs) != (msgLen*8))
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId, "msgLen does "
            "not match any valid TB Size");
         RETVALUE(RFAILED);
      }


   if(cfgType == RGR_SI_CFG_TYPE_SIB1 || cfgType == RGR_SI_CFG_TYPE_SIB1_PWS)
   {

      if(cell->siCb.crntSiInfo.sib1Info.sib1 == NULLP)
      {
         cell->siCb.crntSiInfo.sib1Info.mcs = mcs;
         cell->siCb.crntSiInfo.sib1Info.nPrb = nPrb;
         cell->siCb.crntSiInfo.sib1Info.msgLen = msgLen;
      }
      else
      {
         cell->siCb.newSiInfo.sib1Info.mcs = mcs;
         cell->siCb.newSiInfo.sib1Info.nPrb= nPrb;
         cell->siCb.newSiInfo.sib1Info.msgLen = msgLen;
      }
   }

    
   if(cfgType == RGR_SI_CFG_TYPE_SI)
   {
      if(cell->siCb.crntSiInfo.siInfo[siId-1].si == NULLP &&
         !(cell->siCb.siBitMask & RGSCH_SI_SICFG_UPD))
      {
         cell->siCb.crntSiInfo.siInfo[siId-1].mcs = mcs;
         cell->siCb.crntSiInfo.siInfo[siId-1].nPrb = nPrb;
         cell->siCb.crntSiInfo.siInfo[siId-1].msgLen = msgLen;
      }
      else
      {
         cell->siCb.newSiInfo.siInfo[siId-1].mcs = mcs;
         cell->siCb.newSiInfo.siInfo[siId-1].nPrb= nPrb;
         cell->siCb.newSiInfo.siInfo[siId-1].msgLen = msgLen;
      }
   }

   if(cfgType == RGR_SI_CFG_TYPE_SIB8_CDMA)
   {
      cell->siCb.crntSiInfo.siInfo[siId-1].mcs = mcs;
      cell->siCb.crntSiInfo.siInfo[siId-1].nPrb = nPrb;
      cell->siCb.crntSiInfo.siInfo[siId-1].msgLen = msgLen;
   }

    RETVALUE(ROK);
}
#endif

/***********************************************************
 *
 *     Func : rgSCHUtlFillDgnParams
 *
 *     Desc : Utility Function to Fill Diagonostic params.
 *
 *     Ret  : None.
 *
 *     Notes: None.
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlFillDgnParams
(
Inst        inst,
RgUstaDgn   *dgn,
U8          dgnType
)
#else
PUBLIC Void rgSCHUtlFillDgnParams(inst, dgn, dgnType)
Inst        inst;
RgUstaDgn   *dgn;
U8          dgnType;
#endif
{

   TRC2(rgSCHUtlFillDgnParams)

   switch(dgnType)
   {
      case LRG_USTA_DGNVAL_MEM:
         dgn->type = (U8) LRG_USTA_DGNVAL_MEM;
         dgn->u.mem.region  = rgSchCb[inst].rgSchInit.region;
         dgn->u.mem.pool    = rgSchCb[inst].rgSchInit.pool;
      break;

      default:
      break;
   }

   RETVOID;
} /* end of rgSCHUtlFillDgnParams */

/***********************************************************
 *
 *     Func : rgSCHUtlGetPstToLyr
 *
 *     Desc : Utility Function to get the pst structure to post a message to MAC
 *
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called while sending a msg from
 *     scheduler instance to MAC
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlGetPstToLyr
(
Pst     *pst,
RgSchCb *schCb,
Inst    macInst
)
#else
PUBLIC Void rgSCHUtlGetPstToLyr (pst, schCb, macInst)
Pst     *pst;
RgSchCb *schCb;
Inst    macInst;
#endif
{
   TRC2(rgSCHUtlGetPstToLyr);

   /* Only the needed params are filled */
   pst->region = schCb->rgSchInit.region;
   pst->pool = schCb->rgSchInit.pool;
   pst->srcInst = schCb->rgSchInit.inst+SCH_INST_START;
   pst->srcProcId = schCb->rgSchInit.procId;
   pst->dstProcId = schCb->rgSchInit.procId;

   pst->dstInst = macInst;
   pst->dstEnt = ENTRG;
   pst->srcEnt = ENTRG;
   pst->selector = 0;
   pst->prior     = PRIOR0;
   pst->intfVer   = 0;
   pst->route   = RTESPEC;

   RETVOID;
} /* end of rgSCHUtlGetPstToLyr */

/** @brief This function fills in the common lc information to be sent to MAC
 *
 * @details
 *
 *     Function: rgSCHUtlFillRgInfCmnLcInfo
 *       @param  RgSchDlSf       *sf,
 *       @param  RgInfSfAlloc    *sfAlloc,
 *       @param  CmLteLcId       lcId,
 *       @param  Bool            sendInd
 *
 * @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHUtlFillRgInfCmnLcInfo
(
RgSchDlSf       *sf,
RgInfSfAlloc    *sfAlloc,
CmLteLcId       lcId,
Bool            sendInd
)
#else
PUBLIC S16 rgSCHUtlFillRgInfCmnLcInfo(sf, sfAlloc, lcId, sendInd)
RgSchDlSf       *sf;
RgInfSfAlloc    *sfAlloc;
CmLteLcId       lcId;
Bool            sendInd;
#endif
{
   TRC2(rgSCHUtlFillRgInfCmnLcInfo);

   if((sf->bch.tbSize)&&
      !(sfAlloc->cmnLcInfo.bitMask & RGINF_BCH_INFO))
   {
#ifndef RGR_SI_SCH
      sfAlloc->cmnLcInfo.bchInfo.lcId = lcId;
#endif
      sfAlloc->cmnLcInfo.bitMask |= RGINF_BCH_INFO;
   }
   else if((sf->bcch.pdcch != NULLP)&&
      !(sfAlloc->cmnLcInfo.bitMask & RGINF_BCCH_INFO))
   {
      sfAlloc->cmnLcInfo.bcchInfo.rnti = RGSCH_SI_RNTI;
      rgSCHUtlFillPdschDciInfo(&(sfAlloc->cmnLcInfo.bcchInfo.dciInfo),
                                       &(sf->bcch.pdcch->dci));
#ifndef RGR_SI_SCH
      sfAlloc->cmnLcInfo.bcchInfo.lcId = lcId;
      sfAlloc->cmnLcInfo.bcchInfo.sndStatInd = sendInd;
#endif
      sfAlloc->cmnLcInfo.bitMask |= RGINF_BCCH_INFO;
   }
   else if((sf->pcch.pdcch != NULLP) &&
      !(sfAlloc->cmnLcInfo.bitMask & RGINF_PCCH_INFO))
   {
      sfAlloc->cmnLcInfo.pcchInfo.rnti = RGSCH_P_RNTI;
      rgSCHUtlFillPdschDciInfo(&(sfAlloc->cmnLcInfo.pcchInfo.dciInfo),
                                         &(sf->pcch.pdcch->dci));
      sfAlloc->cmnLcInfo.pcchInfo.lcId = lcId;
      sfAlloc->cmnLcInfo.bitMask |= RGINF_PCCH_INFO;
   }
   RETVALUE(ROK);
}

/** @brief This function fills in the RAR information to be sent to MAC
 *
 * @details
 *
 *     Function: rgSCHUtlFillRgInfRarInfo
 *
 * @param  RgSchCellCb  *cell 
 * @param  RgSchDlSf    *sf 
 * @param  RgInfSfAlloc *sfAlloc
 * @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHUtlFillRgInfRarInfo
(
RgSchDlSf       *sf,
RgInfSfAlloc    *sfAlloc,
RgSchCellCb     *cell
)
#else
PUBLIC S16 rgSCHUtlFillRgInfRarInfo(sf, sfAlloc, cell)
RgSchDlSf       *sf;
RgInfSfAlloc    *sfAlloc;
RgSchCellCb     *cell;
#endif 
{
   U8               idx;
   CmLListCp        *lnkLst;
   CmLList          *tmp;
   RgSchRaCb        *raCb;
   RgSchUeCb        *ue;
   RgInfRaRntiInfo  *raRntiAlloc;
   U8               noRaRsps;
   RgSchCmnDlCell   *cellDl = RG_SCH_CMN_GET_DL_CELL(cell);

   TRC2(rgSCHUtlFillRgInfRarInfo);

#ifdef LTE_TDD
   noRaRsps = RGSCH_MAX_TDD_RA_RSP_ALLOC;
#else
   noRaRsps = RGSCH_MAX_RA_RSP_ALLOC;
#endif

   for(idx =0; idx < noRaRsps; idx++)
   {
      if (sf->raRsp[idx].pdcch == NULLP)
      {
         /* No further raResp Allocations. */
         break;
      }
      /* Added Dl TB count for RACH Response transmission*/
#ifdef LTE_L2_MEAS
      cell->dlUlTbCnt.tbTransDlTotalCnt++;
#endif      
      raRntiAlloc = &(sfAlloc->rarInfo.raRntiInfo[idx]);
      raRntiAlloc->raRnti = sf->raRsp[idx].raRnti;
      raRntiAlloc->schdTbSz = sf->raRsp[idx].tbSz;
      raRntiAlloc->numCrnti = 0;
      rgSCHUtlFillPdschDciInfo(&(raRntiAlloc->dciInfo),
                      &(sf->raRsp[idx].pdcch->dci));
      /* RACHO : fill backoff indicator information */
      raRntiAlloc->backOffInd = sf->raRsp[idx].backOffInd;

      /* Fill for contention free UEs*/
      lnkLst = &(sf->raRsp[idx].contFreeUeLst);
      CM_LLIST_FIRST_NODE(lnkLst, tmp);
      while (tmp)
      {
         ue = (RgSchUeCb *)tmp->node;
         tmp = tmp->next;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].tmpCrnti = ue->ueId;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].isContFree = TRUE;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].rapId = ue->ul.rarGrnt.rapId;
#ifndef MAC_5GTF_UPDATE
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.hop =
                                                        ue->ul.rarGrnt.hop;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.cqiBit =
                                                        ue->ul.rarGrnt.cqiReqBit;
#endif
         /* SHASHAHNK ADD RIV CALC */
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.rbStart =
                                                        ue->ul.rarGrnt.rbStart;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.numRb =
                                                        ue->ul.rarGrnt.numRb;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.tpc =
                                                        ue->ul.rarGrnt.tpc;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.iMcsCrnt =
                                                        ue->ul.rarGrnt.iMcsCrnt;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].ta = ue->ul.rarGrnt.ta;
         raRntiAlloc->numCrnti++;
         cmLListDelFrm(lnkLst, &ue->ul.rarGrnt.raRspLnk);
         ue->ul.rarGrnt.raRspLnk.node = (PTR)NULLP;
      }
      /* Fill for contention based UEs*/
      lnkLst = &(sf->raRsp[idx].raRspLst);

      CM_LLIST_FIRST_NODE(lnkLst, tmp);

      while((NULLP != tmp) && ((RgSchRaCb *)tmp->node != NULLP))
      {
         raCb     = (RgSchRaCb *)tmp->node;

         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].tmpCrnti = raCb->tmpCrnti;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].isContFree = FALSE;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].rapId = raCb->rapId;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].ta.pres = TRUE;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].ta.val = raCb->ta.val;
#ifndef MAC_5GTF_UPDATE
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.hop =
                                                        raCb->msg3Grnt.hop;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.cqiBit = FALSE;
#endif
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.rbStart =
                                                        raCb->msg3Grnt.rbStart;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.numRb =
                                                        raCb->msg3Grnt.numRb;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.tpc =
                                                        raCb->msg3Grnt.tpc;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.iMcsCrnt =
                                                        raCb->msg3Grnt.iMcsCrnt;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.delayBit =
                                                        raCb->msg3Grnt.delayBit;
         /* For initial attaching UEs Aperiodic CQI need not be triggered */
         raRntiAlloc->numCrnti++;
         /* Search the next node */
         CM_LLIST_NEXT_NODE(lnkLst, tmp);
      }
   }
   sfAlloc->rarInfo.numRaRntis = idx;
   /* ccpu00132314-ADD-Update the tx power allocation info  
      TODO-Need to add a check for max tx power per symbol */
   sfAlloc->rarInfo.txPwrOffset = cellDl->rarTxPwrOffset;

   RETVALUE(ROK);
} /* end of rgSCHUtlFillRgInfRarInfo */

/** @brief This function fills in the pdsch data related allocation Info
 *         from the pdcch DCI info.
 * slot
 *
 * @details
 *
 *     Function: rgSCHUtlFillPdschDciInfo
 *
 *         Processing steps:
 *         - Depending upon the DCI Format, fill the appropriate fields.
 *
 * @param  [out] TfuPdschDciInfo *pdschDci
 * @param  [in]  TfuDciInfo      *pdcchDci
 * @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHUtlFillPdschDciInfo
(
TfuPdschDciInfo *pdsch,
TfuDciInfo      *pdcchDci
)
#else
PUBLIC S16 rgSCHUtlFillPdschDciInfo(pdsch, pdcchDci)
TfuPdschDciInfo *pdsch;
TfuDciInfo      *pdcchDci;
#endif
{
   TRC2(rgSCHUtlFillPdschDciInfo)

#ifdef EMTC_ENABLE
  S16 ret = ROK; 
#endif
   pdsch->format = pdcchDci->dciFormat;
   switch(pdcchDci->dciFormat)
   {
      case TFU_DCI_FORMAT_1:
         pdsch->u.format1AllocInfo = pdcchDci->u.format1Info.allocInfo;
         break;
      case TFU_DCI_FORMAT_1A:
         if (pdcchDci->u.format1aInfo.isPdcchOrder == FALSE)
         {
            pdsch->u.format1aAllocInfo = pdcchDci->u.format1aInfo.t.pdschInfo.allocInfo;
         }
         break;
      case TFU_DCI_FORMAT_1B:
         pdsch->u.format1bAllocInfo = pdcchDci->u.format1bInfo.allocInfo;
         break;
      case TFU_DCI_FORMAT_1C:
         pdsch->u.format1cAllocInfo = pdcchDci->u.format1cInfo;
         break;
      case TFU_DCI_FORMAT_1D:
         pdsch->u.format1dAllocInfo = pdcchDci->u.format1dInfo.allocInfo;
         break;
      case TFU_DCI_FORMAT_2:
         pdsch->u.format2AllocInfo = pdcchDci->u.format2Info.allocInfo;
         break;
      case TFU_DCI_FORMAT_2A:
         pdsch->u.format2AAllocInfo = pdcchDci->u.format2AInfo.allocInfo;
         break;
#ifdef RG_5GTF
		case TFU_DCI_FORMAT_B1:
         pdsch->u.formatB1Info = pdcchDci->u.formatB1Info;
         break;
      case TFU_DCI_FORMAT_B2:
         pdsch->u.formatB2Info = pdcchDci->u.formatB2Info;
         break;
#endif
      default:
#ifdef EMTC_ENABLE
 ret = rgSCHEmtcUtlFillPdschDciInfo(pdsch, pdcchDci);
 if(RFAILED == ret)
 {
      RETVALUE(RFAILED);
	 }
#else  
	 RETVALUE(RFAILED);
#endif
   }
   RETVALUE(ROK);
}

/* LTE_ADV_FLAG_REMOVED_START */
/**
 * @brief This function resets temporary variables in Pool
 * @details
 *
 *     Function: rgSchSFRResetPoolVariables
 *
 *     Invoked by: rgSCHSFRUtlTotalPoolInit
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSubFrm*     subFrm
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSchDSFRPwrCheck
(
 RgSchDlSf               *sf,
 Bool                    *isAllUePwrHigh
 )
#else
PRIVATE Void rgSchDSFRPwrCheck(sf, isAllUePwrHigh)
   RgSchDlSf               *sf;
   Bool                    *isAllUePwrHigh;

#endif
{     
   RgSchSFRPoolInfo *sfrCCPool;

   CmLListCp   *l;
   CmLList     *n;   

   TRC2(rgSchDSFRPwrCheck); 

   l = &sf->sfrTotalPoolInfo.ccPool;    
   n = cmLListFirst(l);
   while(n)
   {    
      sfrCCPool = (RgSchSFRPoolInfo*)n->node;
      if((sfrCCPool->poolstartRB == sfrCCPool->pwrHiCCRange.startRb) &&
            (sfrCCPool->poolendRB == sfrCCPool->pwrHiCCRange.endRb))
      {
         n = cmLListNext(l);
         if(n)
         {
            continue;
         }
         *isAllUePwrHigh = TRUE;
         break;
      }
      else
         break;   
   } 
}
/* LTE_ADV_FLAG_REMOVED_END */
/***********************************************************
 *
 *     Func : rgSCHUtlFillRgInfTbInfo
 *
 *     Desc : Utility Function to fill the allocation info of each Tb
 *
 *
 *     Ret  :  RETVOID
 *
 *
 *     Notes: This function should be called while sending a msg from
 *     scheduler instance to MAC
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHUtlFillRgInfTbInfo
(
RgSchDlHqProcCb *hqP,
RgInfUeAlloc    *allocInfo,
RgSchCellCb     *cell
)
#else
PRIVATE Void rgSCHUtlFillRgInfTbInfo (hqP, allocInfo, cell)
RgSchDlHqProcCb *hqP;
RgInfUeAlloc    *allocInfo;
RgSchCellCb     *cell;
#endif
{
   RgSchDlSf       *sf;
   U8              idx;
   RgInfUeTbInfo   *tbInfo;
   U8              tbCnt;
   /* LTE_ADV_FLAG_REMOVED_START */
#ifdef TFU_UPGRADE
   PRIVATE         U32  tmpCnt = 0;
   Bool            isAllUePwrHigh = FALSE;
#endif 
   /* LTE_ADV_FLAG_REMOVED_END */
   RgSchDlLcCb    *dlLcCb = NULLP;
   U16             rlcHdrEstmt;
   U8              lcId;
   /* RRM_RBC_X */
#ifdef LTE_L2_MEAS
   U8              prbUsed = 0;
#endif
   /* RRM_RBC_Y */

   CmLteTimingInfo        frm;

   /* Get Downlink slot */
   frm   = cell->crntTime;
   RGSCH_INCR_SUB_FRAME(frm, RG_SCH_CMN_DL_DELTA);
   sf = rgSCHUtlSubFrmGet(cell, frm);
   /* Setting of fillCtrlPdu flag
      If both P-cell and S-cell are present,
      make TRUE for P-cell and FALSE for all s-cells
      For all other cases set TRUE */
#ifdef LTE_ADV
   if ((rgSchCb[cell->instIdx].genCfg.forceCntrlSrbBoOnPCel) &&
         !RG_SCH_CMN_IS_PCELL_HQP(hqP))
   {
      allocInfo->fillCtrlPdu = FALSE;
   }
   else
   {
      allocInfo->fillCtrlPdu = TRUE;
   }
#endif

   allocInfo->tbStrtIdx = -1;


#ifdef LTE_ADV
   allocInfo->tbReqInfo.sCellHqPId = 0xff;
   rgSCHLaaHndlFillRgInfTbInfo(cell, hqP, allocInfo);
#endif

   /*TODO:REEMA: Check and fill the isRetx */
   for(tbCnt = 0; tbCnt < 2; tbCnt++)
   {
      RgSchUeCb  *ue = NULLP;
      /*Changed as a result of CR timer*/
      if ((hqP->hqE->ue != NULLP)/* &&
      ((hqP->tbInfo[tbCnt].lchSchdData[0].lcId != 0) || \
       (hqP->tbInfo[tbCnt].schdTa.pres == PRSNT_NODEF))*/)
      {
         ue = hqP->hqE->ue;
         allocInfo->rnti = ue->ueId;
         allocInfo->doa = hqP->hqE->ue->mimoInfo.doa;
         allocInfo->txMode = (TfuTxMode)(hqP->hqE->ue->mimoInfo.txMode);
         allocInfo->puschRptUsd = hqP->hqE->ue->mimoInfo.puschFdbkVld;
         allocInfo->puschPmiInfo = hqP->hqE->ue->mimoInfo.puschPmiInfo;
         if(hqP->tbInfo[tbCnt].schdTa.pres == PRSNT_NODEF)
         {
            hqP->tbInfo[tbCnt].taSnt = TRUE;
         }
#ifdef TFU_UPGRADE
         if (RG_SCH_IS_PAPRSNT(ue,hqP->hqE->cell))
         {
            /*update pA value */
            allocInfo->pa = (RG_SCH_CMN_GET_PA(ue,hqP->hqE->cell)).val;
         }
#endif

         /* LTE_ADV_FLAG_REMOVED_START */
         /* If ABS is enabled, calculate resource used */
         if((0 == tbCnt) && (RGR_ENABLE == ue->cell->lteAdvCb.absCfg.status))
         {
            /* For Macro count number resource used in Non-ABS SF */
            if(RGR_ABS_MUTE == ue->cell->lteAdvCb.absCfg.absPatternType)
            {
               if(RG_SCH_ABS_ENABLED_NONABS_SF == ue->cell->lteAdvCb.absDlSfInfo)
               {
                  ue->cell->lteAdvCb.absLoadInfo[ue->cell->lteAdvCb.absPatternDlIdx]+=
                     hqP->tbInfo[tbCnt].dlGrnt.numRb;
               }
            }
            /* For pico count number resource used in ABS SF for ABS UE */
            else if(RGR_ABS_TRANSMIT == ue->cell->lteAdvCb.absCfg.absPatternType)
            {
               if(RG_SCH_ABS_ENABLED_ABS_SF == ue->cell->lteAdvCb.absDlSfInfo)
               {
                  if(TRUE == ue->lteAdvUeCb.rgrLteAdvUeCfg.isAbsUe)
                  {
                     ue->cell->lteAdvCb.absLoadInfo[ue->cell->lteAdvCb.absPatternDlIdx]+=
                        hqP->tbInfo[tbCnt].dlGrnt.numRb;
                  }
               }
            }
         } 

#ifdef TFU_UPGRADE         
         /*if SFR is enabled*/
         allocInfo->isEnbSFR = (U8)RG_SCH_CMN_IS_SFR_ENB(ue->cell); /* KW fix for LTE_ADV */
         if((ue->cell->lteAdvCb.dsfrCfg.status == RGR_ENABLE) && 
               (ue->lteAdvUeCb.rgrLteAdvUeCfg.isUeCellEdge == FALSE))
         {         
            rgSchDSFRPwrCheck(sf, &isAllUePwrHigh);          
         } 
         if(isAllUePwrHigh)
         {  
            allocInfo->pa = (U8)ue->cell->lteAdvCb.sfrCfg.pwrThreshold.pHigh;  /* KW fix for LTE_ADV */  
            if(tmpCnt++ == 100000)
            {
               RLOG_ARG2(L_DEBUG,DBG_CELLID,ue->cell->cellId, 
                        "DSFR::ll UEs can go HIGH, PHigh(%d) for UE(%d)",allocInfo->pa, ue->ueId);
               tmpCnt = 0;
            }
         }    
         else
         {  
            if (allocInfo->isEnbSFR)
            {
               /*Update pA to Plow if it is cell-centred ,else pA will be pHigh*/
               if (ue->lteAdvUeCb.rgrLteAdvUeCfg.isUeCellEdge == TRUE)
               {  
                  allocInfo->pa = ue->cell->lteAdvCb.sfrCfg.pwrThreshold.pHigh;
                  if(tmpCnt++ == 100000)
                  {
                     RLOG_ARG2(L_DEBUG,DBG_CELLID,ue->cell->cellId, 
                              "SFR::UE is CELL EDGE, PHigh(%d) for UE(%d)",allocInfo->pa, ue->ueId);
                     tmpCnt = 0;
                  }

               }  
               else
               {
                  if(TRUE == ue->lteAdvUeCb.isCCUePHigh)
                  {
                     allocInfo->pa = ue->cell->lteAdvCb.sfrCfg.pwrThreshold.pHigh;
                     ue->lteAdvUeCb.isCCUePHigh = FALSE;
                  }
                  else
                  {
                     allocInfo->pa = ue->cell->lteAdvCb.sfrCfg.pwrThreshold.pLow;
                     if(tmpCnt++ == 100000)
                     {
                        RLOG_ARG2(L_DEBUG,DBG_CELLID,ue->cell->cellId, 
                                 "SFR::UE is CELL CENTRE, PLow(%d) for UE(%d)\n",allocInfo->pa, ue->ueId);
                        tmpCnt = 0;
                     }
                  }
               }
            }
         }
         /* LTE_ADV_FLAG_REMOVED_END */
#endif         
      }
      else
      {
         if (hqP->hqE->raCb)
         {
#ifdef TFU_UPGRADE
            RgSchCmnDlCell   *cellDl = RG_SCH_CMN_GET_DL_CELL(cell);
#endif 
#ifdef LTEMAC_SPS
            allocInfo->pdcchRnti = hqP->hqE->raCb->tmpCrnti;
#endif
            allocInfo->rnti  =  hqP->hqE->raCb->tmpCrnti;
#ifdef TFU_UPGRADE
            /*ccpu00132314-ADD-Use a default pA value
              for msg4 */
            allocInfo->pa = cellDl->msg4pAVal; 
#endif 
         }
      }
      /* If TB Is scheduled for this SF */
      if(hqP->tbInfo[tbCnt].state == HQ_TB_WAITING)
      {
         if (allocInfo->tbStrtIdx == -1){
            allocInfo->tbStrtIdx = tbCnt;
#ifndef LTEMAC_SPS
            rgSCHUtlFillPdschDciInfo(&(allocInfo->dciInfo),
                               &(hqP->pdcch->dci));
#else
            if (hqP->pdcch)
            {
               rgSCHUtlFillPdschDciInfo(&(allocInfo->dciInfo),
                     &(hqP->pdcch->dci));
            }
            else if ((ue) && (ue->dl.spsOccPdcch.rnti == ue->spsRnti))
            {
               rgSCHUtlFillPdschDciInfo(&(allocInfo->dciInfo),
                     &(ue->dl.spsOccPdcch.dci));
            }
#endif /* ifndef LTEMAC_SPS */
         }
#ifdef LTEMAC_SPS
            if (hqP->pdcch)
            {
               allocInfo->pdcchRnti = hqP->pdcch->rnti;
            }
            else if (ue)
            {
               allocInfo->pdcchRnti = ue->spsRnti;
            }
#endif
         tbInfo = &(allocInfo->tbInfo[tbCnt]);
         allocInfo->nmbOfTBs++;
         allocInfo->hqProcId = hqP->procId;
         allocInfo->tbInfo[tbCnt].schdTbSz = hqP->tbInfo[tbCnt].tbSz;

         tbInfo->disTb  = FALSE;
         if(!(hqP->tbInfo[tbCnt].txCntr))
         {
#ifdef LTE_ADV
            if(!((rgSCHLaaCheckIfLAAProc(hqP)) && (TRUE ==
                        rgSCHLaaSCellEnabled(cell))))
#endif
            {
               hqP->tbInfo[tbCnt].txCntr++;
            }
            for(idx = 0; idx < hqP->tbInfo[tbCnt].numLch; idx++)
            {
               tbInfo->schdDat[idx].lcId =\
                  hqP->tbInfo[tbCnt].lchSchdData[idx].lcId;
               tbInfo->schdDat[idx].numBytes =\
                  hqP->tbInfo[tbCnt].lchSchdData[idx].schdData;
               if(hqP->hqE->ue)
               {
                  lcId = hqP->tbInfo[tbCnt].lchSchdData[idx].lcId; 
                  if(lcId != 0)
                  {
                     dlLcCb = hqP->hqE->ue->dl.lcCb[lcId-1];
                     if(dlLcCb != NULLP)
                     {
                        RG_SCH_CMN_DL_GET_HDR_EST(dlLcCb, rlcHdrEstmt);
                        /* Update the totalBo with the scheduled bo */
                        (hqP->hqE->ue->totalBo <= tbInfo->schdDat[idx].numBytes - rlcHdrEstmt)?\
                                                  (hqP->hqE->ue->totalBo = 0):\
                                                  (hqP->hqE->ue->totalBo -= tbInfo->schdDat[idx].numBytes-rlcHdrEstmt);

                        /* RRM_RBC_X */
#ifdef LTE_L2_MEAS
                        prbUsed = ((hqP->tbInfo[tbCnt].\
                                 lchSchdData[idx].schdData  *
                                 hqP->tbInfo[tbCnt].dlGrnt.numRb) /
                              (hqP->tbInfo[0].tbSz + hqP->tbInfo[1].tbSz));
                        dlLcCb->qciCb->dlPrbCount += prbUsed;
                        if(dlLcCb->qciCb->qci > 0)
                        {
                           RG_SCH_CALC_GBR_UTILIZATION(cell, dlLcCb, prbUsed);
                        }
#endif                        /* RRM_RBC_Y */

#ifdef RG_PFS_STATS
                        //if(!(hqP->hqE->ue->pfsStats.lcStats[lcId-1].isLcCntSet))
                        if(tbCnt == 0)
                        {
                           U8 idx = 0;
                           if (hqP->hqE->ue->cell == hqP->hqE->cell)
                           {
                              idx = RGSCH_PCELL_INDEX;
                           }
                           else
                           {
                              idx = RG_SCH_GET_SCELL_INDEX((hqP->hqE->ue), (hqP->hqE->cell));
                           }
                           hqP->hqE->ue->pfsStats.lcStats[lcId-1].ueSchdOcc[idx]++;
                           hqP->hqE->ue->pfsStats.lcStats[lcId-1].perRefresh[ue->pfsStats.lcStats[lcId-1].lastIdx].lcSchdOcc++;
                        }   
#endif
                     }
                  }
               }
            }
            /* Added Dl TB count for SRB/DRB data transmission*/
#ifdef LTE_L2_MEAS 
            cell->dlUlTbCnt.tbTransDlTotalCnt++;  
#endif            
            tbInfo->ta.pres = hqP->tbInfo[tbCnt].schdTa.pres;
            tbInfo->ta.val =  hqP->tbInfo[tbCnt].schdTa.val;
#ifdef LTE_ADV
            tbInfo->sCellActCe = hqP->tbInfo[tbCnt].schdSCellActCe;
#endif
            tbInfo->numSchLch = hqP->tbInfo[tbCnt].numLch;
            if(!(hqP->tbInfo[tbCnt].numLch))
            {
               tbInfo->schdDat[tbInfo->numSchLch].numBytes= hqP->tbInfo[tbCnt].tbSz;
               /* Fix: If only TA is scheduled, use some dummy LCID */
               if (tbInfo->ta.pres)
                  tbInfo->schdDat[tbInfo->numSchLch].lcId = RG_TA_LCID;
            }

            tbInfo->contResCe = hqP->tbInfo[tbCnt].contResCe;
            tbInfo->isReTx = FALSE;
         }
         else
         {
#ifdef LTE_ADV
            if(!((rgSCHLaaCheckIfLAAProc(hqP)) && (TRUE ==
                        rgSCHLaaSCellEnabled(cell))))
#endif
            {
               hqP->tbInfo[tbCnt].txCntr++;
            }
            tbInfo->isReTx = TRUE;
            /* RRM_RBC_X */
            /* As per 36.314, harq retransmission also considered for 
             * prb utilization calculation*/
            for(idx = 0; idx < hqP->tbInfo[tbCnt].numLch; idx++)
            {
#ifdef LTE_L2_MEAS
               if(hqP->hqE->ue)
               {
                  lcId = hqP->tbInfo[tbCnt].lchSchdData[idx].lcId; 
                  if(lcId != 0)
                  {
                     dlLcCb = hqP->hqE->ue->dl.lcCb[lcId-1];
                     if(dlLcCb != NULLP)
                     {
                        prbUsed = ((hqP->tbInfo[tbCnt].\
                               lchSchdData[idx].schdData  *
                               hqP->tbInfo[tbCnt].dlGrnt.numRb) /
                                  (hqP->tbInfo[0].tbSz + hqP->tbInfo[1].tbSz));
                        if(dlLcCb->qciCb->qci > 0)
                        {
                           RG_SCH_CALC_GBR_UTILIZATION(cell, dlLcCb, prbUsed);
                        }
                     }
                  }
               }
#endif
            }
            /* RRM_RBC_Y */
         }
      }
    }
#ifdef LTE_ADV 
   rgSCHLaaResetDlHqProcCb(hqP);
#endif

   RETVOID;
}
/***********************************************************
 *
 *     Func : rgSCHUtlFillRgInfUeInfo
 *
 *     Desc : Utility Function to fill the allocation info of Ue
 *             : MIMO : Filling 2TB's of each UE
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called while sending a msg from
 *     scheduler instance to MAC
 *
 *     File : rg_utl.c
 *
 **********************************************************/
  /* CA dev Start */
#ifdef ANSI
PUBLIC Void rgSCHUtlFillRgInfUeInfo
(
RgSchDlSf       *sf,
RgSchCellCb     *cell,
CmLListCp       *dlDrxInactvTmrLst,
CmLListCp       *dlInActvLst,
CmLListCp       *ulInActvLst
)
#else
PUBLIC Void rgSCHUtlFillRgInfUeInfo (sf,cell, dlDrxInactvTmrLst, dlInActvLst, ulInActvLst)
{
RgSchDlSf       *sf;
RgSchCellCb     *cell;
CmLListCp       *dlDrxInactvTmrLst;
CmLListCp       *dlInActvLst;
CmLListCp       *ulInActvLst;
#endif
{
   RgInfSfAlloc    *sfAlloc;
   CmLListCp       *lnkLst;   /* lnkLst assignment */
   CmLList         *tmp;
   CmLList         *hqPNode;
   RgSchUeCb       *ue = NULLP;
   RgInfUeInfo     *ueInfo = NULLP;
   RgInfUeAlloc    *ueAlloc = NULLP;
   RgSchDlHqProcCb *hqCb = NULLP;

   /* Since Msg4 is sched only on PCELL, use cell arg's sfAllocArr */
   sfAlloc = &(cell->sfAllocArr[cell->crntSfIdx]);
   ueInfo = &(sfAlloc->ueInfo);
   ueAlloc = sfAlloc->ueInfo.allocInfo;

   lnkLst = &(sf->msg4HqPLst);
   CM_LLIST_FIRST_NODE(lnkLst, tmp);
   while(NULLP != tmp)
   {
      printf("5GTF_ERROR MSG4 Consolidation\n");
      hqCb = (RgSchDlHqProcCb *)(tmp->node);
      CM_LLIST_NEXT_NODE(lnkLst, tmp);

      rgSCHUtlFillRgInfTbInfo(hqCb, &ueAlloc[ueInfo->numUes], cell);

      ue = hqCb->hqE->ue;

      if(ue != NULLP)
      {   
         if((!(ue->dl.dlInactvMask & RG_HQENT_INACTIVE)) && (ue->isDrxEnabled))
         {   
            rgSCHUtlGetDrxSchdUesInDl(cell, ue, hqCb, &ueAlloc[ueInfo->numUes], 
                  dlDrxInactvTmrLst, dlInActvLst, ulInActvLst);
         }
      }
      ueInfo->numUes++;
   }
 
   lnkLst = &(sf->ueLst);
   CM_LLIST_FIRST_NODE(lnkLst, tmp);
   while(NULLP != tmp)
   {
#if defined (TENB_STATS) && defined (RG_5GTF)
      cell->tenbStats->sch.dl5gtfPdschCons++;
#endif
      ue = (RgSchUeCb *)(tmp->node);
      CM_LLIST_NEXT_NODE(lnkLst, tmp);

      hqPNode = ue->dl.dlSfHqInfo[cell->cellId][sf->dlIdx].hqPLst.first;
      while (hqPNode)
      {
         hqCb = (RgSchDlHqProcCb *)hqPNode->node;
         hqPNode = hqPNode->next;

         sfAlloc = &(hqCb->hqE->cell->sfAllocArr[hqCb->hqE->cell->crntSfIdx]);
         ueInfo = &(sfAlloc->ueInfo);
         ueAlloc = sfAlloc->ueInfo.allocInfo;

         rgSCHUtlFillRgInfTbInfo(hqCb, &ueAlloc[ueInfo->numUes], 
               hqCb->hqE->cell);

         if(ue->isDrxEnabled)
         {   
            rgSCHUtlGetDrxSchdUesInDl(cell, ue, hqCb, &ueAlloc[ueInfo->numUes], 
                  dlDrxInactvTmrLst, dlInActvLst, ulInActvLst);
         }
         ueInfo->numUes++;
      }
#ifdef LTE_ADV
      if (rgSchCb[cell->instIdx].genCfg.isSCellActDeactAlgoEnable == TRUE)
      {
         /*If remaining BO is left then increment the count*/
         if(ue->totalBo > 0)
         {
            ue->remBoCnt++;
            /* Check if trigger for Activation is met or not */
            if(rgSCHIsActvReqd(cell, ue))
            {
               ue->remBoCnt = 0;
               /*Passing primary cell*/
               rgSCHSCellSelectAndActDeAct(ue->cell, ue, RGR_SCELL_ACT);
            }
         }
         else
         {
            /*If remaining BO is 0 then reset the count*/
            ue->remBoCnt = 0;
         }
      }
#endif
   }
  
   RETVOID;
} /* end of rgSCHUtlFillRgInfUeInfo */
  /* CA dev End */


/** @brief This function  shall update the scheduler with the CEs and data rcvd
 *
 * @details
 *
 *     Function: rgSCHUtlUpdSch
 *
 *         Processing steps:
 *         - Collate the information of all the SDUs received and inform the
 *         scheduler rgSCHDataRcvd
 *         - Send Data indication to the higher layer with the dedicated data
 *         (rgUIMSndDedDatInd)
 *         - Inform scheduler with any MAC CEs if present.
 *
 * @param  [in] RgCellCb   *cellCb
 * @param  [in] RgUeCb     *ueCb
 * @param  [in] RgMacPdu   *pdu
 * @param  [in] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHUtlUpdSch
(
RgInfSfDatInd    *subfrmInfo,
RgSchCellCb      *cellCb,
RgSchUeCb        *ueCb,
RgInfUeDatInd    *pdu,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHUtlUpdSch (subfrmInfo, cellCb, ueCb, pdu, err)
RgInfSfDatInd    *subfrmInfo;
RgSchCellCb      *cellCb;
RgSchUeCb        *ueCb;
RgInfUeDatInd    *pdu;
RgSchErrInfo     *err;
#endif
{

   S16               ret;
   TRC2(rgSCHUtlUpdSch);

#ifdef LTEMAC_SPS
   if (RGSCH_UL_SPS_ACT_PRSENT & pdu->ceInfo.bitMask)
   {
      /* SPS to be activated due to data on SPS LCG ID*/
      rgSCHUtlSpsActInd(cellCb, ueCb, pdu->ceInfo.spsSduSize);
   }
#endif
   /* TODO : Temp Fix for crash due to UL SDU corruption*/
   if (RGSCH_PHR_CE_PRSNT & pdu->ceInfo.bitMask)
   {
      /* PHR present */
      RGSCHCPYTIMEINFO(subfrmInfo->timingInfo, ueCb->macCeRptTime);
      if ((ret = rgSCHUtlUpdPhr(cellCb, ueCb, pdu->ceInfo.ces.phr, err)) != ROK)
         RETVALUE(ret);
   }
   /* Note: Order of indication to Sch now is
    *       1st Indicate the DataInd info for each LCG's
    *       2nd Update the BSR reports received along with data
    *       this is to make sure the effBsr is updated to the latest BSR 
    *       received.
    */
   cellCb->sc.apis->rgSCHUpdUeDataIndLcg(cellCb, ueCb, pdu);

#ifndef MAC_5GTF_UPDATE
   if (RGSCH_TRUNC_BSR_CE_PRSNT & pdu->ceInfo.bitMask)
   {
      RGSCHCPYTIMEINFO(subfrmInfo->timingInfo, ueCb->macCeRptTime);
      /*ccpu00129922 - MOD - Deleted return value
       * checking since it returns void*/
      rgSCHUtlUpdBsrTrunc (cellCb, ueCb,
                  (U8)(pdu->ceInfo.ces.bsr.truncBsr >> 6),
                  (U8)(pdu->ceInfo.ces.bsr.truncBsr & 0x3F), err); 
   }
   else
   {
      if (RGSCH_SHORT_BSR_CE_PRSNT & pdu->ceInfo.bitMask)
      {
         RGSCHCPYTIMEINFO(subfrmInfo->timingInfo, ueCb->macCeRptTime);
         /*ccpu00129922 - MOD - Deleted return value
          checking since it returns void*/
         rgSCHUtlUpdBsrShort (cellCb, ueCb,
                     (U8)(pdu->ceInfo.ces.bsr.shortBsr >> 6),
                     (U8)(pdu->ceInfo.ces.bsr.shortBsr & 0x3F), err);
      }
      else
      {
         if (RGSCH_LONG_BSR_CE_PRSNT & pdu->ceInfo.bitMask)
#else
         if (RGSCH_BSR_CE_PRSNT & pdu->ceInfo.bitMask)
#endif
         {
            RGSCHCPYTIMEINFO(subfrmInfo->timingInfo, ueCb->macCeRptTime);
            /*ccpu00129922 - MOD - Deleted return value
              checking since it returns void*/
            rgSCHUtlUpdBsrLong (cellCb, ueCb,
                        pdu->ceInfo.ces.bsr.longBsr.bs1,
                        pdu->ceInfo.ces.bsr.longBsr.bs2,
                        pdu->ceInfo.ces.bsr.longBsr.bs3,
                        pdu->ceInfo.ces.bsr.longBsr.bs4,
                        err);
         }
#ifndef MAC_5GTF_UPDATE
      }
 
   }
#endif
            
   RETVALUE(ROK);
} /* end of rgSCHUtlUpdSch */
#ifdef RGR_V1
/**
 * @brief Handler for Updating Bo received in StaRsp
 *
 * @details
 *
 *     Function : rgSCHUtlAddUeToCcchSduLst
 *
 *     This function shall be invoked once it receives staRsp on CCCH
 *
 *  @param[in]     RgSchCellCb       *cell
 *  @param[in]     RgSchUeCb       *ueCb
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlAddUeToCcchSduLst
(
RgSchCellCb       *cell,
RgSchUeCb         *ueCb
)
#else
PUBLIC S16 rgSCHUtlAddUeToCcchSduLst(cell, ueCb)
RgSchCellCb       *cell;
RgSchUeCb         *ueCb;
#endif
{
   RgSchCmnDlUe  *ueDl = RG_SCH_CMN_GET_DL_UE(ueCb, cell);
   RgSchDlHqProcCb *hqP  = (RgSchDlHqProcCb *)ueDl->proc;
   TRC2(rgSCHUtlAddUeToCcchSduLst);

   /* Temp Guard: For back to back CCCH SDU BO 
    * twice. Hence an extra guard. If already added to scheduling
    * queue or if scheduled and waiting for HQ FDBK, ignore */
   if ((ueCb->ccchSduLnk.node) ||
       ((!(ueCb->dl.dlInactvMask & RG_HQENT_INACTIVE)) &&
       ((hqP != NULLP) && (hqP->hqE->ccchSduProc))))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"RNTI:%d Unexpected CCCH SDU BO",
         ueCb->ueId);
      RETVALUE(ROK);
   }

   ueCb->ccchSduLnk.node = (PTR)(ueCb);
   cmLListAdd2Tail(&(cell->ccchSduUeLst), &(ueCb->ccchSduLnk));

   RETVALUE(ROK);
}
/**
 *
 * @details
 *
 *     Function : rgSCHUtlUpdtBo
 *
 *     This function shall be invoked once it receives staRsp on CCCH
 *
 *  @param[in]     RgSchCellCb       *cell
 *  @param[in]     RgRguCmnStaRsp *staRsp
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlUpdtBo
(
RgSchCellCb       *cell,
RgInfCmnBoRpt     *staRsp
)
#else
PUBLIC S16 rgSCHUtlUpdtBo(cell, staRsp)
RgSchCellCb       *cell;
RgInfCmnBoRpt     *staRsp;
#endif
{
   RgSchUeCb *ueCb;
   TRC2(rgSCHUtlUpdtBo)


   if ((ueCb = rgSCHDbmGetUeCb(cell, staRsp->u.rnti)) == NULLP)
   {
      /* Handle Ue fetch failure */
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"Invalid UEID:%d",staRsp->u.rnti);
      RETVALUE(RFAILED);
   }
   /* Update Bo in ueCb */
   ueCb->dlCcchInfo.bo = (U32)(staRsp->bo);
#ifdef EMTC_ENABLE
     if(ueCb->isEmtcUe)
     {
        rgSCHUtlAddUeToEmtcCcchSduLst(cell,ueCb);
     }
     else
#endif
    {
      rgSCHUtlAddUeToCcchSduLst(cell, ueCb);
    }

   RETVALUE(ROK);
} /* rgSCHUtlUpdtBo */

#endif
/**
 *
 * @details
 *     Function : rgSCHUtlHndlCcchBoUpdt
 *
 *     This function shall fetch the raCb with the given rnti and ask RAM to
 *     update BO
 *
 *
 *  @param[in]  RgSchCellCb    *cell
 *  @param[in]  RgInfCmnBoRpt *boRpt
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlHndlCcchBoUpdt
(
RgSchCellCb    *cell,
RgInfCmnBoRpt  *boRpt
)
#else
PUBLIC S16 rgSCHUtlHndlCcchBoUpdt(cell, boRpt)
RgSchCellCb    *cell;
RgInfCmnBoRpt  *boRpt;
#endif
{
   RgSchRaCb       *raCb;
   RgSchUeCb *ueCb;

   TRC2(rgSCHUtlHndlCcchBoUpdt);

   if ((raCb = rgSCHDbmGetRaCb(cell, boRpt->u.rnti)) == NULLP)
   {
#ifdef RGR_V1
   /* CR timer implementation changes*/
      /*If no raCb, schedule ueCb, ueCb is extracted in rgSCHUtlUpdtBo*/
      RETVALUE(rgSCHUtlUpdtBo(cell, boRpt));
#else
      /* Handle RaCb fetch failure */
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
		      "Invalid RNTI:%d to fetch raCb",boRpt->u.rnti);
      RETVALUE(RFAILED);
#endif
   }

#ifdef RGR_V1

      /*Fix: If RaCb exists, then MSG4 is not completed yet*/
      /*Check if guard timer has expired, if not CR CE + CCCH SDU will be scheduled*/
      if((raCb->contResTmrLnk.node != NULLP) && \
            (raCb->schdLnk.node == NULLP) && (raCb->dlHqE->msg4Proc == NULLP))
      {
#ifdef EMTC_ENABLE
         /*if contention resolution timer left ,Stop the Contention Resolution Guard Timer ,
         add in toBeSchduled list and update the Bo */
         if(TRUE == raCb->isEmtcRaCb)
         {
            rgSCHRamEmtcUpdtBo(cell, raCb, boRpt);
         }
         else
#endif
         {
            cmLListDelFrm(&cell->contResGrdTmrLst, &(raCb->contResTmrLnk));
            raCb->contResTmrLnk.node=NULLP;               
            rgSCHRamUpdtBo(cell, raCb, boRpt);
         }
       }
      else
      {
         /*Fix:Guard timer has expired */
         /*Update the BO in UE CB but dont add it to the scheduling list. 
          *Should be added to the list after MSG4 completion*/
         if ((ueCb = rgSCHDbmGetUeCb(cell, boRpt->u.rnti)) == NULLP)
         {
            /* Handle Ue fetch failure */
            RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"Invalid RNTI:%d",boRpt->u.rnti);
            RETVALUE(RFAILED);
         }
         /* Update Bo in ueCb */
         ueCb->dlCcchInfo.bo = (U32)(boRpt->bo);     
      }

#else
      rgSCHRamUpdtBo(cell, raCb, boRpt);
#endif

   RETVALUE(ROK);
} /* rgSCHUtlHndlCcchBoUpdt */

/**
 * @brief Validates BO received for BCCH or PCCH.
 *
 * @details
 *
 *     Function : rgSCHUtlGetAllwdCchTbSz
 *
 *     This function shall return the tbSz equal to or
 *     the nearest greater value for a given bo.
 *     If no such value found return -1. The nPrb value is
 *     accordingly set.
 *
 *
 *  @param[in]  U32            bo
 *  @param[out] U8             *nPrb
 *  @return  S32
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S32 rgSCHUtlGetAllwdCchTbSz
(
U32 bo,
U8  *nPrb,
U8  *mcs
)
#else
PUBLIC S32 rgSCHUtlGetAllwdCchTbSz(bo, nPrb, mcs)
U32 bo;
U8  *nPrb;
U8  *mcs;
#endif
{
   S32 lt;
   S32 cn;
   S32 rt;

   TRC2(rgSCHUtlGetAllwdCchTbSz);

   for (lt = 0, rt = 43; lt <= rt;)
   {
      cn = (lt + rt)/2;
      if (rgSchUtlBcchPcchTbSzTbl[cn].tbSz == bo)
      {
         *nPrb = rgSchUtlBcchPcchTbSzTbl[cn].rbIndex;
         *mcs  = rgSchUtlBcchPcchTbSzTbl[cn].mcs;
         RETVALUE(rgSchUtlBcchPcchTbSzTbl[cn].tbSz);
      }
      else if (rgSchUtlBcchPcchTbSzTbl[cn].tbSz < bo)
         lt = cn + 1;
      else
         rt = cn - 1;
   }
   if (lt == 44)
   {
      RETVALUE(RFAILED);
   }
   *nPrb = rgSchUtlBcchPcchTbSzTbl[lt].rbIndex;
   *mcs  = rgSchUtlBcchPcchTbSzTbl[lt].mcs;
   RETVALUE(rgSchUtlBcchPcchTbSzTbl[lt].tbSz);
}

/**
 * @brief Handler for BO Updt received for BCCH or PCCH.
 *
 * @details
 *
 *     Function : rgSCHUtlHndlBcchPcchBoUpdt
 *
 *     This function shall store the buffer and time to transmit in lcCb
 *
 *
 *  @param[in]  RgSchCellCb    *cell
 *  @param[in]  RgInfCmnBoRpt  *boRpt
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlHndlBcchPcchBoUpdt
(
RgSchCellCb    *cell,
RgInfCmnBoRpt  *boUpdt
)
#else
PUBLIC S16 rgSCHUtlHndlBcchPcchBoUpdt(cell, boUpdt)
RgSchCellCb    *cell;
RgInfCmnBoRpt  *boUpdt;
#endif
{
   RgSchClcDlLcCb *dlLc;
   RgSchClcBoRpt  *boRpt;
   Inst           inst = cell->instIdx;
   U8             nPrb=0;
   U8             mcs=0;

   TRC2(rgSCHUtlHndlBcchPcchBoUpdt);

   dlLc = rgSCHDbmGetBcchOnBch(cell);
   if (dlLc == NULLP)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
      "No Logical Channel dlLc is NULLP for RNTI:%d LCID:%d",boUpdt->u.rnti,boUpdt->lcId);
      RETVALUE(RFAILED);
   }
   if (boUpdt->lcId != dlLc->lcId)
   {
      /* Added for dropping paging Message*/	  
	  /*suman*/
      if ((rgSCHChkBoUpdate(cell,boUpdt))== ROK)  /* Checking if received BO falls within the window of 5120 slots*/
      {
         if (rgSCHUtlGetAllwdCchTbSz(boUpdt->bo*8, &nPrb, &mcs) 
               != (boUpdt->bo*8))
         {
            RLOG_ARG3(L_ERROR,DBG_CELLID,cell->cellId,"[%ld]BO: does not match any "
                  "valid TB Size RNTI:%d LCID:%d", boUpdt->bo,boUpdt->u.rnti,boUpdt->lcId);
            RETVALUE(RFAILED);
         }
      }/*end of rgSCHChkBoUpdate*/
      else
      {
          RETVALUE(ROK);
      } 
   }
   if ((dlLc = rgSCHDbmGetCmnLcCb(cell, boUpdt->lcId)) == NULLP)
   {
      /* Handle lcCb fetch failure */
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
		      "LCID:%d Invalid for RNTI:%d",boUpdt->lcId,boUpdt->u.rnti);
   }

   if (((rgSCHUtlAllocSBuf(inst, (Data **)(&boRpt), sizeof(RgSchClcBoRpt))) ==RFAILED) ||
       (!boRpt))
   {
      RLOG_ARG3(L_ERROR,DBG_CELLID,cell->cellId, "Allocation of common bo %dreport "
         "failed  RNTI:%d LCID:%d", boUpdt->bo,boUpdt->u.rnti,boUpdt->lcId);
      RETVALUE(RFAILED);
   }

   boRpt->bo = boUpdt->bo;
   boRpt->mcs = mcs;
   boRpt->timeToTx = boUpdt->u.timeToTx;
   boRpt->nPrb = nPrb;
#ifdef EMTC_ENABLE
   if(cell->emtcEnable)
   {
      boRpt->emtcDIReason = boUpdt->emtcDIReason;
      boRpt->pnb = boUpdt->pnb;
   }
#endif
   RG_SCH_ADD_TO_CRNT_TIME(boRpt->timeToTx, 
         boRpt->maxTimeToTx, cell->siCfg.siWinSize)
   if((NULLP != dlLc) && (dlLc->si))
   {
      boRpt->retxCnt = cell->siCfg.retxCnt;
   }
   else
   {
      boRpt->retxCnt = 0;
   }
   rgSCHDbmInsCmnLcBoRpt(dlLc, boRpt);

   RETVALUE(ROK);
} /* rgSCHUtlHndlBcchPcchBoUpdt */

/**
 * @brief API for sending bind confirm from Scheduler instance to RRM
 *
 * @details
 *
 *     Function: rgSCHUtlRgrBndCfm
 *
 *     This API is invoked to send bind confirm from Scheduler instance to RRM.
 *     This API fills in Pst structure and SAP Ids and invokes
 *     bind confirm API towards RRM.
 *
 *  @param[in]  SuId          suId
 *  @param[in]  U8            status
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrBndCfm
(
Inst          instId,
SuId          suId,
U8            status
)
#else
PUBLIC S16 rgSCHUtlRgrBndCfm(instId, suId, status)
Inst          instId;
SuId          suId;
U8            status;
#endif
{
   S16  ret = ROK;

   TRC2(rgSCHUtlRgrBndCfm)


   ret = RgUiRgrBndCfm(&rgSchCb[instId].rgrSap[suId].sapCfg.sapPst, rgSchCb[instId].rgrSap[suId].sapCfg.suId, status);
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,instId,"rgSCHUtlRgrBndCfm: RgUiRgrBndCfm Failed ");
      RETVALUE(ret);
   }
   RETVALUE(ret);
}  /* rgSCHUtlRgrBndCfm*/

/**
 * @brief API for sending bind confirm from Scheduler instance to RRM via RGM
 *        interface
 *
 * @details
 *
 *     Function: rgSCHUtlRgmBndCfm
 *
 *     This API is invoked to send bind confirm from Scheduler instance to RRM.
 *     This API fills in Pst structure and SAP Ids and invokes
 *
 *  @param[in]  SuId          suId
 *  @param[in]  U8            status
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgmBndCfm
(
Inst          instId,
SuId          suId,
U8            status
)
#else
PUBLIC S16 rgSCHUtlRgmBndCfm(instId, suId, status)
Inst          instId;
SuId          suId;
U8            status;
#endif
{
   S16  ret = ROK;

   TRC2(rgSCHUtlRgmBndCfm)


   ret = RgUiRgmBndCfm(&rgSchCb[instId].rgmSap[suId].sapCfg.sapPst, rgSchCb[instId].rgmSap[suId].sapCfg.suId, status);
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,instId,"rgSCHUtlRgmBndCfm: RgUiRgrBndCfm Failed ");
      RETVALUE(ret);
   }
   RETVALUE(ret);
}  /* rgSCHUtlRgmBndCfm*/



/**
 * @brief API for sending configuration confirm from Scheduler to DU APP
 *
 * @details
 *
 *     Function: schSendCfgCfm 
 *
 *     This API is invoked to send configuration confirm from Scheduler to DU
 *     APP.
 *
 *  @param[in]  Pst           pst 
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  U8            status
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 schSendCfgCfm
(
Region        reg,
Pool          pool,
RgrCfgTransId transId,
U8            status
)
#else
PUBLIC S16 schSendCfgCfm(reg, pool, transId, status)
Region        reg;
Pool          pool;
RgrCfgTransId transId;
U8            status;
#endif
{
   Pst cfmPst;
   Inst inst = 0;

   TRC2(schSendCfgCfm)
   cmMemset((U8 *)(&cfmPst), 0, sizeof(Pst));

   cfmPst.srcEnt    = (Ent)ENTDUAPP;
   cfmPst.srcInst   = (Inst) 0;
   cfmPst.srcProcId = SFndProcId();
   cfmPst.dstEnt    = (Ent)ENTRG;
   cfmPst.dstInst   = (Inst) 0;
   cfmPst.dstProcId = SFndProcId();
   cfmPst.selector  = RGR_SEL_LC;
   cfmPst.region    = reg;
   cfmPst.pool      = pool;

   if(RgUiRgrCfgCfm(&cfmPst,transId, status) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"schSendCfgCfm: RgUiRgrCfgCfm Failed");
      printf("\nschSendCfgCfm: RgUiRgrCfgCfm Failed ");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}  /* schSendCfgCfm*/
#ifdef RGR_RRM_TICK
/**
 * @brief API for sending TTI indication from Scheduler to RRM.
 *
 * @details
 *
 *     Function: rgSCHUtlRgrTtiInd
 *
 *     This API is invoked to send TTI indication from Scheduler instance to RRM.
 *     This API fills in Pst structure and RgrTtiIndInfo
 *
 *  @param[in]  cell                   RgSchCellCb
 *  @param[in]  CmLteTimingInfo        status
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrTtiInd
(
RgSchCellCb          *cell,
RgrTtiIndInfo        *rgrTti
)
#else
PUBLIC S16 rgSCHUtlRgrTtiInd(cell, rgrTti)
RgSchCellCb          *cell;
RgrTtiIndInfo        *rgrTti;
#endif
{
   S16           ret = ROK;
   RgSchUpSapCb  *rgrSap;                    /*!< RGR SAP Control Block */
#ifdef L2_L3_SPLIT
   extern Bool g_usettitmr;
   extern Void mtTmrHdlrPublic(void);
#endif

   TRC2(rgSCHUtlRgrTtiInd)


   rgrSap = cell->rgrSap;
   if (rgrSap->sapSta.sapState != LRG_BND)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
               "rgSCHUtlRgrTtiInd() Upper SAP not bound (%d) ",
               rgrSap->sapSta.sapState);
      RETVALUE(RFAILED);
   }
   RgUiRgrTtiInd(&(cell->rgrSap->sapCfg.sapPst),
         cell->rgrSap->sapCfg.suId, rgrTti);
#ifdef L2_L3_SPLIT
   {
      g_usettitmr = TRUE;
      mtTmrHdlrPublic();
   }
#endif
   RETVALUE(ret);
}  /* rgSCHUtlRgrTtiInd*/
#endif
/** @brief This function is called by rgMacSchSfRecpInd. This function invokes the
 * scheduler with the information of the received Data and any Control Elements
 * if present.
 *
 * @details
 *
 *     Function:
 *
 *         Processing steps:
 *         - Retrieves the RaCb with the rnti provided, if it doesnt exist
 *         return failure.
 *         - If UE exists then update the Schduler with any MAC CEs if present.
 *         - Invoke RAM module to do Msg3 related processing rgSCHRamProcMsg3
 *
 * @param  [in] RgSchCellCb   *cellCb
 * @param  [in] RgSchUeCb     *ueCb
 * @param  [in] CmLteRnti     rnti
 * @param  [in] RgMacPdu   *pdu
 * @param  [in] RgSchErrInfo  *err
 * @param
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHUtlProcMsg3
(
RgInfSfDatInd    *subfrmInfo,
RgSchCellCb      *cellCb,
RgSchUeCb        *ueCb,
CmLteRnti        rnti,
RgInfUeDatInd    *pdu,
RgSchErrInfo     *err
 )
#else
PUBLIC S16 rgSCHUtlProcMsg3 (subfrmInfo, cellCb, ueCb, rnti, pdu, err)
RgInfSfDatInd    *subfrmInfo;
RgSchCellCb      *cellCb;
RgSchUeCb        *ueCb;
CmLteRnti        rnti;
RgInfUeDatInd    *pdu;
RgSchErrInfo     *err;
#endif
{
   S16               ret;
   RgSchRaCb         *raCb;

   TRC2(rgSCHUtlProcMsg3)


   /* must have an raCb for this case */
   raCb = rgSCHDbmGetRaCb (cellCb, rnti);
   if (raCb == NULLP)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId, "RNTI:%d Received MSG3, unable to "
         "find raCb",rnti);
      RETVALUE(RFAILED);
   }

   /* ccpu00130982: Processing CRNTI MAC CE before Short BSR, if any, such that 
    * effBsr of current case only will be considered in scheduling of ContResLst*/
   ret = rgSCHRamProcMsg3 (cellCb, ueCb, raCb, pdu, err);
   if (ret != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,"Processing failed in the RAM "
         "RNTI:%d ",rnti);
      RETVALUE(ret);
   }
   /* if ueCb is present */
   if (ueCb != NULLP)
   {
      rgSCHUtlUpdSch (subfrmInfo, cellCb, ueCb, pdu, err);
   }

   RETVALUE(ROK);
}
#ifdef LTEMAC_SPS
/** @brief This function is called by RgMacSchSpsRelInd. This function invokes the
 * scheduler with the information of the received Data.
 *
 * @details
 *
 *     Function: rgSCHUtlSpsRelInd
 *
 *         Processing steps:
 *         TODO
 *
 * @param  [in] RgSchCellCb        *cellCb
 * @param  [in] RgSchUeCb          *ueCb
 * @param  [in] Bool               *isExplRel
 * @param
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHUtlSpsRelInd
(
RgSchCellCb        *cellCb,
RgSchUeCb          *ueCb,
Bool               isExplRel
)
#else
PUBLIC S16 rgSCHUtlSpsRelInd (cellCb, ueCb, isExplRel)
RgSchCellCb        *cellCb;
RgSchUeCb          *ueCb;
Bool               isExplRel;
#endif
{
   TRC2(rgSCHUtlSpsRelInd);
   cellCb->sc.apis->rgSCHUlSpsRelInd(cellCb, ueCb, isExplRel);
   RETVALUE(ROK);
} /* end of rgSCHUtlSpsRelInd */


/** @brief This function is called by RgMacSchSpsRelInd. This function invokes the
 * scheduler with the information of the received Data.
 *
 * @details
 *
 *     Function: rgSCHUtlSpsActInd
 *
 *         Processing steps:
 *         TODO
 *
 * @param  [in] RgSchCellCb        *cellCb
 * @param  [in] RgSchUeCb          *ueCb
 * @param  [in] U16                spsSduSize
 * @param
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHUtlSpsActInd
(
RgSchCellCb        *cellCb,
RgSchUeCb          *ueCb,
U16                spsSduSize
)
#else
PUBLIC S16 rgSCHUtlSpsActInd (cellCb, ueCb, spsSduSize)
RgSchCellCb        *cellCb;
RgSchUeCb          *ueCb;
U16                spsSduSize;
#endif
{
   TRC2(rgSCHUtlSpsActInd);
   cellCb->sc.apis->rgSCHUlSpsActInd(cellCb, ueCb, spsSduSize);
   RETVALUE(ROK);
} /* end of rgSCHUtlSpsActInd */


#endif /* LTEMAC_SPS */

#ifdef RG_PHASE_2
/**
 * @brief This API is invoked to send uplink group power control request to PHY.
 *
 * @details
 *
 *     Function : rgSCHUtlTfuGrpPwrCntrlReq
 *
 *      This API is invoked to send uplink group power control request to PHY.
 *      It fills in the Pst structure, spId value and invokes group power
 *      control request primitive at TFU.
 *
 *  @param[in]  TfuGrpPwrCntrlReqInfo *grpPwrCntrlReq
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlTfuGrpPwrCntrlReq
(
Inst                  inst,
S16                   sapId,
TfuGrpPwrCntrlReqInfo *grpPwrCntrlReq
)
#else
PUBLIC S16 rgSCHUtlTfuGrpPwrCntrlReq(inst, sapId, grpPwrCntrlReq)
Inst                  inst;
S16                   sapId;
TfuGrpPwrCntrlReqInfo *grpPwrCntrlReq;
#endif
{
   S16             ret;
   RgSchLowSapCb  *tfuSap;
   Pst             pst;

   TRC2(rgSCHUtlTfuGrpPwrCntrlReq);

   /* Get the lower SAP control block from the layer control block. */
   tfuSap = &(rgSchCb[inst].tfuSap[sapId]);
   if (tfuSap->sapSta.sapState != LRG_BND)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,grpPwrCntrlReq->cellId,
		      "rgSCHUtlTfuGrpPwrCntrlReq() Lower SAP not bound (%d) ",tfuSap->sapSta.sapState);
      RETVALUE(RFAILED);
   }
   cmMemcpy ((U8*)&pst, (U8*)&(tfuSap->sapCfg.sapPst), sizeof(Pst));
   if((ret = RgLiTfuGrpPwrCntrlReq (&pst, tfuSap->sapCfg.spId, grpPwrCntrlReq)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,grpPwrCntrlReq->cellId,
		      "rgSCHUtlTfuGrpPwrCntrlReq() Call to RgLiTfuGrpPwrCntrlReq() failed");
   }
   RETVALUE(ret);
}  /* rgSCHUtlTfuGrpPwrCntrlReq */
#endif

/**
 * @brief This API is invoked to send Control Info to PHY.
 *
 * @details
 *
 *     Function : rgSCHUtlTfuCntrlReq
 *
 *      This API is invoked to send Control Info to PHY. It
 *      fills in the Pst structure, spId value and invokes Cntrl
 *      request primitive at TFU.
 *
 *  @param[in]  TfuCntrlReqInfo *cntrlReq
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlTfuCntrlReq
(
Inst            inst,
S16             sapId,
TfuCntrlReqInfo *cntrlReq
)
#else
PUBLIC S16 rgSCHUtlTfuCntrlReq(inst, sapId, cntrlReq)
Inst            inst;
S16             sapId;
TfuCntrlReqInfo *cntrlReq;
#endif
{
   S16            ret;
   RgSchLowSapCb  *tfuSap;

   TRC2(rgSCHUtlTfuCntrlReq)

   /* Get the lower SAP control block from the layer control block. */
   tfuSap = &(rgSchCb[inst].tfuSap[sapId]);

#ifndef NO_ERRCLS
   if (tfuSap->sapSta.sapState != LRG_BND)
   {
      RLOG_ARG1(L_ERROR,DBG_INSTID,inst,"rgSCHUtlTfuCntrlReq() Lower SAP not bound (%d) ",
            tfuSap->sapSta.sapState);
      RGSCH_FREE_MEM(cntrlReq);
      RETVALUE(RFAILED);
   }
#endif

  /* Using local variable for pst is unnecessary - for optimization */
   if((ret = RgLiTfuCntrlReq(&tfuSap->sapCfg.sapPst, tfuSap->sapCfg.spId,
                             cntrlReq)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"rgSCHUtlTfuCntrlReq() Call to RgLiTfuCntrlReq() failed");
   }
   RETVALUE(ret);
}  /* rgSCHUtlTfuCntrlReq*/


/* FOR ACK NACK REP */

/**
 * @brief This API is invoked to tell the DL Scheduler to add the UE back into
 * its scheduling queues.
 *
 * @details
 *
 *     Function : rgSCHUtlDlActvtUe
 *
 *      This API is invoked from Measurement gap moduled.
 *
 *  @param[in]  RgSchCellCb    *cell
 *  @param[in]  RgSchUeCb        *ueCb
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlDlActvtUe
(
RgSchCellCb *cell,
RgSchUeCb   *ue
)
#else
PUBLIC S16 rgSCHUtlDlActvtUe(cell, ue)
RgSchCellCb *cell;
RgSchUeCb   *ue;
#endif
{
   TRC2(rgSCHUtlDlActvtUe);
   cell->sc.apis->rgSCHActvtDlUe(cell, ue);
   RETVALUE(ROK);
}

/**
 * @brief This API is invoked to tell the UL Scheduler to add the UE back into
 * its scheduling queues.
 *
 * @details
 *
 *     Function : rgSCHUtlUlActvtUe
 *
 *      This API is invoked from Measurement gap moduled.
 *
 *  @param[in]  RgSchCellCb    *cell
 *  @param[in]  RgSchUeCb        *ueCb
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlUlActvtUe
(
RgSchCellCb *cell,
RgSchUeCb   *ue
)
#else
PUBLIC S16 rgSCHUtlUlActvtUe(cell, ue)
RgSchCellCb *cell;
RgSchUeCb   *ue;
#endif
{
   TRC2(rgSCHUtlUlActvtUe);
   cell->sc.apis->rgSCHActvtUlUe(cell, ue);
   RETVALUE(ROK);
}

/**
 * @brief This API is invoked to send Reception Request Info to PHY.
 *
 * @details
 *
 *     Function : rgSCHUtlTfuRecpReq
 *
 *      This API is invoked to send Reception Request Info to PHY. It
 *      fills in the Pst structure, spId value and invokes Reception
 *      request primitive at TFU.
 *
 *  @param[in]  TfuRecpReqInfo *recpReq
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlTfuRecpReq
(
Inst            inst,
S16             sapId,
TfuRecpReqInfo *recpReq
)
#else
PUBLIC S16 rgSCHUtlTfuRecpReq(inst, sapId, recpReq)
Inst            inst;
S16             sapId;
TfuRecpReqInfo *recpReq;
#endif
{
   S16            ret;
   RgSchLowSapCb  *tfuSap;

   TRC2(rgSCHUtlTfuRecpReq)

   /* Get the lower SAP control block from the layer control block. */
   tfuSap = &(rgSchCb[inst].tfuSap[sapId]);

#ifndef NO_ERRCLS
   if (tfuSap->sapSta.sapState != LRG_BND)
   {
      RLOG_ARG1(L_ERROR,DBG_INSTID,inst,"rgSCHUtlTfuRecpReq() Lower SAP not bound (%d) ",
            tfuSap->sapSta.sapState);
      RGSCH_FREE_MEM(recpReq);
      RETVALUE(RFAILED);
   }
#endif

  /* Using local variable for pst is unnecessary - for optimization */
   if((ret = RgLiTfuRecpReq(&tfuSap->sapCfg.sapPst, tfuSap->sapCfg.spId,
                            recpReq)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"rgSCHUtlTfuRecpReq() Call to RgLiTfuRecpReq() failed");
   }
   RETVALUE(ret);
}  /* rgSCHUtlTfuRecpReq */

 /** @brief This function Validates the SAP information received along with the
  * primitive from the lower layer.
  *
  * Function: rgSCHUtlValidateTfuSap
  *
  *                      Validates SAP information.
  * @param  suId The SAP Id
  * @return
  *   -# ROK
  *   -# RFAILED
  */
#ifdef ANSI
PUBLIC S16 rgSCHUtlValidateTfuSap
(
Inst  inst,
SuId  suId
)
#else
PUBLIC S16 rgSCHUtlValidateTfuSap(inst, suId)
Inst  inst;
SuId  suId;
#endif
{
   RgSchLowSapCb  *tfuSap;

   TRC2(rgSCHUtlValidateTfuSap)

   if(suId >= rgSchCb[inst].numSaps)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst, "Incorrect SuId");
      RETVALUE(RFAILED);
   }
   tfuSap = &(rgSchCb[inst].tfuSap[suId]);

   /* First lets check the suId */
   if( suId != tfuSap->sapCfg.suId)
   {
      RLOG_ARG2(L_ERROR,DBG_INSTID,inst,"Incorrect SuId. Configured (%d) Recieved (%d)",
            tfuSap->sapCfg.suId, suId);
      RETVALUE(RFAILED);
   }
   if (tfuSap->sapSta.sapState != LRG_BND)
   {
      RLOG_ARG1(L_ERROR,DBG_INSTID,inst,"Lower SAP not enabled SuId (%d)",
            tfuSap->sapCfg.suId);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of rgSCHUtlValidateTfuSap */

/*
*
*       Fun:   rgSCHUtlAllocEventMem
*
*       Desc:  This function allocates event memory
*
*       Ret:   ROK      - on success
*              RFAILED  - on failure
*
*       Notes: None
*
*       File:  rg_utl.c
*
*/
#ifdef ANSI
PUBLIC S16 rgSCHUtlAllocEventMem
(
Inst      inst,
Ptr       *memPtr,
Size      memSize
)
#else
PUBLIC S16 rgSCHUtlAllocEventMem(inst, memPtr, memSize)
Inst      inst;
Ptr       *memPtr;
Size      memSize;
#endif
{
   Mem              sMem;
   VOLATILE U32     startTime=0;

   TRC2(rgSCHUtlAllocEventMem)

   sMem.region = rgSchCb[inst].rgSchInit.region;
   sMem.pool = rgSchCb[inst].rgSchInit.pool;

#if (ERRCLASS & ERRCLS_DEBUG)
   if (memSize<= 0)
   {
      RGSCHLOGERROR(inst, ERRCLS_INT_PAR, ERG022, memSize,
                   "rgAllocEventMem(): memSize invalid\n");
      RETVALUE (RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_DEBUG */
   /*starting Task*/
   SStartTask(&startTime, PID_SCHUTL_CMALLCEVT);

#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
   MS_BUF_ADD_ALLOC_CALLER();
#endif /* */
#ifdef TFU_ALLOC_EVENT_NO_INIT
   if(ROK != cmAllocEvntNoInit(memSize, TFU_MAX_MEMBLK_SIZE, &sMem, memPtr))
#else
   if(ROK != cmAllocEvnt(memSize, TFU_MAX_MEMBLK_SIZE, &sMem, memPtr))
#endif /* */
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"cmAllocEvnt Failed.");
      RETVALUE(RFAILED);
   }
   /*stoping Task*/
   SStopTask(startTime, PID_SCHUTL_CMALLCEVT);
   RETVALUE(ROK);
} /* end of rgSCHUtlAllocEventMem*/

/*
*
*       Fun:   rgGetEventMem
*
*       Desc:  This function allocates event memory
*
*       Ret:   ROK      - on success
*              RFAILED  - on failure
*
*       Notes: None
*
*       File:  rg_utl.c
*
*/
#ifdef ANSI
PUBLIC S16 rgSCHUtlGetEventMem
(
Ptr       *ptr,
Size      len,
Ptr       memCp
)
#else
PUBLIC S16 rgSCHUtlGetEventMem(ptr, len, memCp)
Ptr       *ptr;
Size      len;
Ptr       memCp;
#endif
{
   S16   ret;

   TRC2(rgSCHUtlGetEventMem)
#ifdef TFU_ALLOC_EVENT_NO_INIT
   ret = cmGetMemNoInit(memCp, len, (Ptr *)ptr);
#else
   ret = cmGetMem(memCp, len, (Ptr *)ptr);
#endif
   RETVALUE(ret);
} /* end of rgSCHUtlGetEventMem*/

#ifdef LTE_TDD


/**
 * @brief Handler to allocate memory for ACK/NACk feedback information
 *
 * @details
 *
 *     Function : rgSCHUtlAllocUeANFdbkInfo
 *
 *   It allocates memory for the UE related ACK NACK information.
 *
 *  @param[in]  RgSchUeCb           *ue
 *  @return     S16
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlAllocUeANFdbkInfo
(
RgSchUeCb            *ue,
U8                   servCellIdx
)
#else
PUBLIC S16 rgSCHUtlAllocUeANFdbkInfo(ue,servCellIdx)
RgSchUeCb            *ue;
U8                   servCellIdx;
#endif
{
   U8 idx;

   TRC2(rgSCHUtlAllocUeANFdbkInfo);

   if (rgSCHUtlAllocSBuf(ue->cell->instIdx,
               (Data **) &(ue->cellInfo[servCellIdx]->anInfo), sizeof(RgSchTddANInfo) * \
               ue->cell->ackNackFdbkArrSize) != ROK)
   {
      RETVALUE(RFAILED);
   }

   for(idx=0; idx < ue->cell->ackNackFdbkArrSize; idx++)
   {
      rgSCHUtlInitUeANFdbkInfo(&ue->cellInfo[servCellIdx]->anInfo[idx]);
   }

   /* Set it to the first index */
   ue->cellInfo[servCellIdx]->nextFreeANIdx = 0;
   RETVALUE(ROK);
} /* rgSCHUtlAllocUeANFdbkInfo */

/**
 * @brief Handler to release memory for ACK/NACk feedback information
 *
 * @details
 *
 *     Function : rgSCHUtlDelUeANFdbkInfo
 *
 *   It releases memory for the UE related ACK NACK information.
 *
 *  @param[in]  RgSchUeCb           *ue
 *  @return     Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlDelUeANFdbkInfo
(
RgSchUeCb            *ue,
U8                   servCellIdx
)
#else
PUBLIC Void rgSCHUtlDelUeANFdbkInfo(ue,servCellIdx)
RgSchUeCb            *ue;
U8                   servCellIdx;
#endif
{
   TRC2(rgSCHUtlDelUeANFdbkInfo);

   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(ue->cell->instIdx,
         (Data **)(&( ue->cellInfo[servCellIdx]->anInfo)), sizeof(RgSchTddANInfo) * \
                        ue->cell->ackNackFdbkArrSize);

   RETVOID;
} /* rgSCHUtlDelUeANFdbkInfo */

/**
 * @brief Handler to initialise UE ACK/NACk feedback information
 *
 * @details
 *
 *     Function : rgSCHUtlInitUeANFdbkInfo
 *
 *   It initialises UE related ACK NACK information.
 *
 *  @param[in]  RgSchTddANInfo   *anFdInfo
 *  @return     S16
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlInitUeANFdbkInfo
(
RgSchTddANInfo       *anFdInfo
)
#else
PUBLIC S16 rgSCHUtlInitUeANFdbkInfo(anFdInfo)
RgSchTddANInfo       *anFdInfo;
#endif
{
   TRC2(rgSCHUtlInitUeANFdbkInfo);

   anFdInfo->sfn = RGSCH_MAX_SFN+1; /* defensively setting invalid sfn */
   anFdInfo->slot = 0;
   anFdInfo->ulDai = RG_SCH_INVALID_DAI_VAL;
   anFdInfo->dlDai = RG_SCH_INVALID_DAI_VAL;
   anFdInfo->latestMIdx = RG_SCH_INVALID_M_VAL;

   RETVALUE(ROK);
} /* rgSCHUtlInitUeANFdbkInfo */

/**
 * @brief Handler to get UE related ACK NACK feedback information
 *
 * @details
 *
 *     Function : rgSCHUtlGetUeANFdbkInfo
 *
 *   It gets the UE related ACK NACK information based on
 *   SFN and slot number.
 *
 *  @param[in]  RgSchUeCb        *ueCb
 *  @param[in]  CmLteTimingInfo  *time
 *  @return     RgSchTddANInfo*
 **/
#ifdef ANSI
PUBLIC RgSchTddANInfo* rgSCHUtlGetUeANFdbkInfo
(
RgSchUeCb         *ueCb,
CmLteTimingInfo   *timeInfo,
U8                servCellIdx
)
#else
PUBLIC RgSchTddANInfo* rgSCHUtlGetUeANFdbkInfo(ueCb, timeInfo,servCellIdx)
RgSchUeCb         *ueCb;
CmLteTimingInfo   *timeInfo;
U8                servCellIdx;
#endif
{
   U8 idx;

   TRC2(rgSCHUtlGetUeANFdbkInfo);

   for (idx = 0; idx < ueCb->cell->ackNackFdbkArrSize; ++idx)
   {
       if( (timeInfo->sfn == ueCb->cellInfo[servCellIdx]->anInfo[idx].sfn) &&
               (timeInfo->slot == ueCb->cellInfo[servCellIdx]->anInfo[idx].slot))
       {
            RETVALUE(&ueCb->cellInfo[servCellIdx]->anInfo[idx]);
       }
   }

   RETVALUE(NULLP);
} /* rgSCHUtlGetUeANFdbkInfo */

/**
 * @brief To get downlink slot index
 *
 * @details
 *
 *     Function: rgSCHUtlGetDlSfIdx
 *     Purpose:  Gets downlink slot index based on SFN and slot no
 *
 *  @param[in]  CmLteTimingInfo  *timeInfo
 *  @param[in]  RgSchCellCb         *cell
 *  @return U8
 *
 **/
#ifdef ANSI
PUBLIC U8  rgSCHUtlGetDlSfIdx
(
RgSchCellCb        *cell,
CmLteTimingInfo *timeInfo
)
#else
PUBLIC U8  rgSCHUtlGetDlSfIdx(cell, timeInfo)
RgSchCellCb        *cell;
CmLteTimingInfo *timeInfo;
#endif
{
   U16       idx = 0;
   TRC2(rgSCHUtlGetDlSfIdx);

   idx = RGSCH_NUM_SUB_FRAMES - \
         rgSchTddNumUlSubfrmTbl[cell->ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];
   idx = ((idx * timeInfo->sfn) + \
         rgSchTddNumDlSubfrmTbl[cell->ulDlCfgIdx][timeInfo->slot]) - 1;
   idx = idx % cell->numDlSubfrms;

   RETVALUE((U8)idx);
}

/**
 * @brief To get the next downlink slot
 *
 * @details
 *
 *     Function: rgSCHUtlGetNxtDlSfInfo
 *     Purpose:  Gets next downlink slot based on current DL slot
 *
 *  @param[in]  CmLteTimingInfo  curDlTime
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchDlSf        *dlSf
 *  @param[in]  RgSchDlSf        **nxtDlsf
 *  @param[in]  CmLteTimingInfo  *nxtDlTime
 *  @return U8
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlGetNxtDlSfInfo
(
CmLteTimingInfo   curDlTime,
RgSchCellCb       *cell,
RgSchDlSf         *dlSf,
RgSchDlSf         **nxtDlsf,
CmLteTimingInfo   *nxtDlTime
)
#else
PUBLIC Void rgSCHUtlGetNxtDlSfInfo(curDlTime, cell, dlSf, nxtDlsf, nxtDlTime)
CmLteTimingInfo   curDlTime;
RgSchCellCb       *cell;
RgSchDlSf         *dlSf;
RgSchDlSf         **nxtDlsf;
CmLteTimingInfo   *nxtDlTime;
#endif
{
   U16  idx = curDlTime.slot;
   U8   count = 0;
   TRC2(rgSCHUtlGetNxtDlSfInfo);

   while(TRUE)
   {
      do
      {
         idx = (idx + 1) % RGSCH_NUM_SUB_FRAMES;
         count++;
      }while(rgSchTddUlDlSubfrmTbl[cell->ulDlCfgIdx][idx]
                                       != RG_SCH_TDD_DL_slot);
      RG_SCH_ADD_TO_CRNT_TIME(curDlTime, (*nxtDlTime), count);
      *nxtDlsf = rgSCHUtlSubFrmGet(cell, *nxtDlTime);
      if(dlSf->dlFdbkInfo.slot != (*nxtDlsf)->dlFdbkInfo.slot)
      {
         break;
      }
   }
   RETVOID;
}

/**
 * @brief To get the previous downlink slot
 *
 * @details
 *
 *     Function: rgSCHUtlGetPrevDlSfInfo
 *     Purpose:  Gets previous downlink slot based on current DL slot
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  CmLteTimingInfo  curDlTime
 *  @param[in]  CmLteTimingInfo  *prevDlTime
 *  @param[in]  U8               *numSubfrm
 *  @return U8
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlGetPrevDlSfInfo
(
RgSchCellCb       *cell,
CmLteTimingInfo   curDlTime,
CmLteTimingInfo   *prevDlTime,
U8                *numSubfrm
)
#else
PUBLIC Void rgSCHUtlGetPrevDlSfInfo(cell, curDlTime, prevDlTime, numSubfrm)
RgSchCellCb       *cell;
CmLteTimingInfo   curDlTime;
CmLteTimingInfo   *prevDlTime;
U8                *numSubfrm;
#endif
{
   S16 idx = curDlTime.slot;
   U8  count = 0;
   TRC2(rgSCHUtlGetPrevDlSfInfo);

   do
   {
      idx--;
      if(idx < 0)
      {
         idx = RGSCH_NUM_SUB_FRAMES-1;
      }
      count++;
   }while(rgSchTddUlDlSubfrmTbl[cell->ulDlCfgIdx][idx]
         !=  RG_SCH_TDD_DL_slot);
   *numSubfrm = count;
   RGSCHDECRFRMCRNTTIME(curDlTime, (*prevDlTime), count);
   RETVOID;
}

#endif
/* Added Holes Management functions for Adaptive Re transmission */
/******* </AllocHolesMemMgmnt>: START *****/
/***********************************************************
 *
 *     Func : rgSCHUtlUlSfInit
 *
 *     Desc : UL slot init.
 *
 *     Ret  : S16
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHUtlUlSfInit
(
RgSchCellCb  *cell,
RgSchUlSf    *sf,
U8           idx,
U8           maxUePerSf
)
#else
PUBLIC S16 rgSCHUtlUlSfInit(cell, sf, idx, maxUePerSf)
RgSchCellCb  *cell;
RgSchUlSf    *sf;
U8           idx;
U8           maxUePerSf;
#endif
{
   S16             ret=ROK;
   TRC2(rgSCHUtlUlSfInit);

   sf->idx = idx;
#ifdef RG_5GTF
   U8 index;
#endif

#ifdef LTE_TDD   
   if(cell->ulDlCfgIdx == 0)
   {
      /* Store the Uplink slot number corresponding to the idx */
      sf->ulSfIdx = rgSchTddCfg0UlSfTbl[idx%6]; 
   }
#endif   
   ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data **)&sf->allocDb,
                           sizeof(RgSchUlAllocDb));
   if (ret != ROK)
   {
      RETVALUE(ret);
   }
   ret = rgSCHUtlUlAllocDbInit(cell, sf->allocDb, maxUePerSf);
   if (ret != ROK)
   {
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(sf->allocDb)),
                       sizeof(RgSchUlAllocDb));
      RETVALUE(ret);
   }
   ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data **)&sf->holeDb,
         sizeof(RgSchUlHoleDb));
   if (ret != ROK)
   {
      rgSCHUtlUlAllocDbDeinit(cell, sf->allocDb);
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(sf->allocDb)),
                       sizeof(RgSchUlAllocDb));
      RETVALUE(ret);
   }
   /* Initialize the hole with CFI 1 Pusch Bw Info */ 
   ret = rgSCHUtlUlHoleDbInit(cell, sf->holeDb, (U8)(maxUePerSf + 2), \
                              0, cell->dynCfiCb.bwInfo[1].numSb);

   if (ret != ROK)
   {
      rgSCHUtlUlAllocDbDeinit(cell, sf->allocDb);
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(sf->allocDb)),
                       sizeof(RgSchUlAllocDb));
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(sf->holeDb)),
                       sizeof(RgSchUlHoleDb));
      RETVALUE(ret);
   }
   cmLListInit(&sf->reTxLst);

   /* Fix ccpu00120610*/
   sf->allocCountRef = &sf->allocDb->count;

   /* initialize UL available subbands for current sub-frame */
   sf->availSubbands = cell->dynCfiCb.bwInfo[1].numSb;
#ifdef RG_5GTF
   sf->numGrpPerTti = cell->cell5gtfCb.ueGrpPerTti;
   sf->numUePerGrp = cell->cell5gtfCb.uePerGrpPerTti;
   for(index = 0; index < MAX_5GTF_BEAMS; index++)
   {
      sf->sfBeamInfo[index].totVrbgAllocated = 0;
      sf->sfBeamInfo[index].totVrbgRequired = 0;
      sf->sfBeamInfo[index].vrbgStart = 0;
   }
#endif

   RETVALUE(ret);
}


/***********************************************************
 *
 *     Func : rgSCHUtlUlSfDeinit
 *
 *     Desc : Deinitialises a slot
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlSfDeinit
(
RgSchCellCb  *cell,
RgSchUlSf    *sf
)
#else
PUBLIC Void rgSCHUtlUlSfDeinit(cell, sf)
RgSchCellCb  *cell;
RgSchUlSf    *sf;
#endif
{
   TRC2(rgSCHUtlUlSfDeinit);
   if (sf->allocDb)
   {
      rgSCHUtlUlAllocDbDeinit(cell, sf->allocDb);
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(sf->allocDb)),
                       sizeof(RgSchUlAllocDb));
   }
   if (sf->holeDb)
   {
      rgSCHUtlUlHoleDbDeinit(cell, sf->holeDb);
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(sf->holeDb)),
                       sizeof(RgSchUlHoleDb));
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocDbInit
 *
 *     Desc : Initialise allocation DB
 *
 *     Ret  : S16 (ROK/RFAILED)
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHUtlUlAllocDbInit
(
RgSchCellCb    *cell,
RgSchUlAllocDb *allocDb,
U8             maxAllocs
)
#else
PRIVATE S16 rgSCHUtlUlAllocDbInit(cell, allocDb, maxAllocs)
RgSchCellCb    *cell;
RgSchUlAllocDb *allocDb;
U8             maxAllocs;
#endif
{
   S16 ret = rgSCHUtlUlAllocMemInit(cell, &allocDb->mem, maxAllocs);
   TRC2(rgSCHUtlUlAllocDbInit);
   if (ret != ROK)
   {
      RETVALUE(ret);
   }
   allocDb->count = 0;
   allocDb->first = NULLP;
   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocDbDeinit
 *
 *     Desc : Deinitialises allocation DB
 *            sent to UE, for a UE with accumulation disabled
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHUtlUlAllocDbDeinit
(
RgSchCellCb     *cell,
RgSchUlAllocDb *allocDb
)
#else
PRIVATE Void rgSCHUtlUlAllocDbDeinit(cell, allocDb)
RgSchCellCb     *cell;
RgSchUlAllocDb *allocDb;
#endif
{
   TRC2(rgSCHUtlUlAllocDbDeinit);
   rgSCHUtlUlAllocMemDeinit(cell, &allocDb->mem);
   allocDb->count = 0;
   allocDb->first = NULLP;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleDbInit
 *
 *     Desc : Initialise hole DB
 *
 *     Ret  : S16 (ROK/RFAILED)
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHUtlUlHoleDbInit
(
RgSchCellCb   *cell,
RgSchUlHoleDb *holeDb,
U8            maxHoles,
U8            start,
U8            num
)
#else
PRIVATE S16 rgSCHUtlUlHoleDbInit(cell, holeDb, maxHoles, start, num)
RgSchCellCb   *cell;
RgSchUlHoleDb *holeDb;
U8            maxHoles;
U8            start;
U8            num;
#endif
{
   S16 ret;
   RgSchUlHole *hole = NULLP;
   TRC2(rgSCHUtlUlHoleDbInit);

   ret = rgSCHUtlUlHoleMemInit(cell, &holeDb->mem, maxHoles, &hole);
   if (ret != ROK)
   {
      RETVALUE(ret);
   }
   holeDb->count = 1;
   holeDb->first = hole;
   hole->start = start;
   hole->num = num;
   hole->prv = hole->nxt = NULLP;
   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleDbDeinit
 *
 *     Desc : Deinitialises hole DB
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHUtlUlHoleDbDeinit
(
RgSchCellCb   *cell,
RgSchUlHoleDb *holeDb
)
#else
PRIVATE Void rgSCHUtlUlHoleDbDeinit(cell, holeDb)
RgSchCellCb   *cell;
RgSchUlHoleDb *holeDb;
#endif
{
   TRC2(rgSCHUtlUlHoleDbDeinit);
   rgSCHUtlUlHoleMemDeinit(cell, &holeDb->mem);
   holeDb->count = 0;
   holeDb->first = NULLP;
   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocGetHole
 *
 *     Desc : Get allocation from hole
 *
 *     Ret  : RgSchUlAlloc *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocGetHole
(
RgSchUlSf   *sf,
U8       numSb,
RgSchUlHole *hole
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocGetHole(sf, numSb, hole)
RgSchUlSf   *sf;
U8       numSb;
RgSchUlHole *hole;
#endif
{
   TRC2(rgSCHUtlUlAllocGetHole);
   if (numSb < hole->num)
   {
      RETVALUE(rgSCHUtlUlAllocGetPartHole(sf, numSb, hole));
   }
   else
   {
      RETVALUE(rgSCHUtlUlAllocGetCompHole(sf, hole));
   }
}


/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocGetCompHole
 *
 *     Desc : Get an allocation corresponding to an entire hole
 *
 *     Ret  : RgSchUlAlloc *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocGetCompHole
(
RgSchUlSf   *sf,
RgSchUlHole *hole
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocGetCompHole(sf, hole)
RgSchUlSf   *sf;
RgSchUlHole *hole;
#endif
{
   RgSchUlAlloc *alloc;
      /* alloc = rgSCHUtlUlAllocGetAndIns(sf->allocDb, hole->prvAlloc, hole->nxtAlloc); */
   /* Calling rgSchCmnUlAllocGetAndIns is ok, but prv alloc needs to have nxtHole
    * updated, causing another check for prv */
   RgSchUlAlloc *prv = hole->prvAlloc;
   RgSchUlAlloc *nxt = hole->nxtAlloc;
   TRC2(rgSCHUtlUlAllocGetCompHole);

   if (prv)
   {
      if (hole->start == prv->nxtHole->start)
      {
         prv->nxtHole = NULLP;
      }
      alloc = rgSCHUtlUlAllocGetAdjNxt(sf->allocDb, prv);
   }
   else
   {
      alloc = rgSCHUtlUlAllocGetFirst(sf->allocDb);
   }

   RGSCH_NULL_CHECK( 0, alloc);
   alloc->prvHole = NULLP;
   alloc->nxtHole = NULLP;

   alloc->sbStart = hole->start;
   alloc->numSb = hole->num;

   if (nxt)
   {
      nxt->prvHole = NULLP;
   }

   rgSCHUtlUlHoleRls(sf->holeDb, hole);

   /* UL_ALLOC_CHANGES*/
   alloc->allocDbRef = (void*)sf->allocDb;
   alloc->holeDbRef  = (void*)sf->holeDb;
   RETVALUE(alloc);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocGetPartHole
 *
 *     Desc : Get an allocation corresponding to a part of a hole.
 *            The initial 'numSb' part of the hole shall be taken
 *            away for this alloc.
 *
 *     Ret  : RgSchUlAlloc *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocGetPartHole
(
RgSchUlSf   *sf,
U8       numSb,
RgSchUlHole *hole
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocGetPartHole(sf, numSb, hole)
RgSchUlSf   *sf;
U8       numSb;
RgSchUlHole *hole;
#endif
{
   RgSchUlAlloc *alloc;
      /* alloc = rgSCHUtlUlAllocGetAndIns(sf->allocDb, hole->prvAlloc, hole->nxtAlloc); */
   /* Calling rgSchCmnUlAllocGetAndIns is ok, but prv alloc needs to have nxtHole
    * updated, causing another check for prv */
   RgSchUlAlloc *prv = hole->prvAlloc;
   TRC2(rgSCHUtlUlAllocGetPartHole);

   if (prv)
   {
      if (hole->start == prv->nxtHole->start)
      {
         prv->nxtHole = NULLP;
      }
      alloc = rgSCHUtlUlAllocGetAdjNxt(sf->allocDb, prv);
   }
   else
   {
      alloc = rgSCHUtlUlAllocGetFirst(sf->allocDb);
   }

   RGSCH_NULL_CHECK( 0, alloc);
   alloc->prvHole = NULLP;
   alloc->nxtHole = hole;
   hole->prvAlloc = alloc;

   alloc->sbStart = hole->start;
   alloc->numSb = numSb;
   hole->start += numSb;
   hole->num -= numSb;

   rgSCHUtlUlHoleDecr(sf->holeDb, hole);

   /* UL_ALLOC_CHANGES*/
   alloc->allocDbRef = (void*)sf->allocDb;
   alloc->holeDbRef  = (void*)sf->holeDb;

   RETVALUE(alloc);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocFirst
 *
 *     Desc : Get first alloc in slot
 *
 *     Ret  : RgSchUlAlloc *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocFirst
(
RgSchUlSf *sf
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocFirst(sf)
RgSchUlSf *sf;
#endif
{
   TRC2(rgSCHUtlUlAllocFirst);
   RETVALUE(sf->allocDb->first);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocNxt
 *
 *     Desc : Get next alloc
 *
 *     Ret  : RgSchUlAlloc *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocNxt
(
RgSchUlSf    *sf,
RgSchUlAlloc *alloc
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocNxt(sf, alloc)
RgSchUlSf    *sf;
RgSchUlAlloc *alloc;
#endif
{
   TRC2(rgSCHUtlUlAllocNxt);
   UNUSED(sf);
   RETVALUE(alloc->nxt);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocGetAdjNxt
 *
 *     Desc : Get alloc which is immediately after the passed one.
 *            1. Gets alloc from mem.
 *            2. Inserts alloc into list (between prv and
 *                prv->nxt, prv is not NULLP).
 *            3. Increments alloc count.
 *            Note 1: Holes are not dealt with here.
 *            Note 2: Assumes prv to be NULL.
 *
 *     Ret  : RgSchUlAlloc *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocGetAdjNxt
(
RgSchUlAllocDb *db,
RgSchUlAlloc   *prv
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocGetAdjNxt(db, prv)
RgSchUlAllocDb *db;
RgSchUlAlloc   *prv;
#endif
{
   RgSchUlAlloc *alloc = rgSCHUtlUlAllocMemGet(&db->mem);
   RgSchUlAlloc *nxt = prv->nxt;
   TRC2(rgSCHUtlUlAllocGetAdjNxt);

#if (ERRCLASS & ERRCLS_DEBUG)
   if ( alloc == NULLP )
   {
       RETVALUE ( NULLP );
   }
#endif
   alloc->prv = prv;
   alloc->nxt = nxt;
   prv->nxt = alloc;
   if (nxt)
   {
      nxt->prv = alloc;
   }

   ++db->count;

   RETVALUE(alloc);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocGetFirst
 *
 *     Desc : Get alloc which is to be the first one in the alloc list
 *            1. Gets alloc from mem.
 *            2. Inserts alloc as first element into list.
 *            3. Increments alloc count.
 *            Note 1: Holes are not dealt with here.
 *            Note 2: prv to necessarily NULLP.
 *
 *     Ret  : RgSchUlAlloc *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocGetFirst
(
RgSchUlAllocDb *db
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocGetFirst(db)
RgSchUlAllocDb *db;
#endif
{
   RgSchUlAlloc *alloc = rgSCHUtlUlAllocMemGet(&db->mem);
   RgSchUlAlloc *nxt = db->first;
   TRC2(rgSCHUtlUlAllocGetFirst);

#if (ERRCLASS & ERRCLS_DEBUG)
    if ( alloc == NULLP )
    {
       RETVALUE ( NULLP );
    }
#endif

   alloc->prv = NULLP;
   alloc->nxt = nxt;
   if (nxt)
   {
      nxt->prv = alloc;
   }
   db->first = alloc;

   ++db->count;

   RETVALUE(alloc);
}

/* UL_ALLOC_ENHANCEMENT */
/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleAddAllocation
 *
 *     Desc : On freeing an alloc, add to hole
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleAddAllocation
(
RgSchUlAlloc *alloc
)
#else
PUBLIC Void rgSCHUtlUlHoleAddAllocation(alloc)
RgSchUlAlloc *alloc;
#endif
{
   /* Note: rgSchCmnUlHoleUpdAllocLnks function that is used should not exist as
    * one, if such excessive branching is done (AllocNone, AllocNoPrv etc).
    * The excessive branching is meant to utilise the knowledge of whether prv
    * and nxt allocs exist or not. Hence for each kind (none, noprv, nonxt,
    * both), there should be a rgSchCmnUlHoleUpdAllocLnks... function (such as
    * rgSchCmnUlHoleUpdAllocLnksNone/NoPrv etc. */
   RgSchUlHoleDb *db = alloc->holeDbRef;
   RgSchUlHole *prv = alloc->prvHole;
   RgSchUlHole *nxt = alloc->nxtHole;
   TRC2(rgSCHUtlUlHoleAddAllocation);

   if (prv)
   {
      if (nxt)
      {
         rgSCHUtlUlHoleJoin(db, prv, nxt, alloc);
      }
      else
         rgSCHUtlUlHoleExtndRight(db, prv, alloc);
   }
   else
   {
      if (nxt)
      {
         rgSCHUtlUlHoleExtndLeft(db, nxt, alloc);
      }
      else
         rgSCHUtlUlHoleNew(db, alloc);
   }
   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocRelease
 *
 *     Desc : Releases an uplink allocation, only take alloc ptr
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlAllocRelease
(
RgSchUlAlloc *alloc
)
#else
PUBLIC Void rgSCHUtlUlAllocRelease(alloc)
RgSchUlAlloc *alloc;
#endif
{
   RgSchUlAllocDb *allocDb = alloc->allocDbRef;
   RgSchUlAlloc   *prv = alloc->prv;
   RgSchUlAlloc   *nxt = alloc->nxt;
   TRC2(rgSCHUtlUlAllocRelease);

   alloc->ue = NULLP;
   alloc->raCb = NULLP;
   alloc->isAdaptive = FALSE;

   if (prv)
   {
      prv->nxt = nxt;
      if (nxt)           /* general case: this allocation lies btw two */
      {
         nxt->prv = prv;
      }
   }
   else
   {
      allocDb->first = nxt;
      if (nxt)
      {
         nxt->prv = NULLP;
      }
   }
   --allocDb->count;
   rgSCHUtlUlHoleAddAllocation(alloc);
   rgSCHUtlUlAllocMemRls(&allocDb->mem, alloc);

   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocRls
 *
 *     Desc : Releases an uplink allocation
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlAllocRls
(
RgSchUlSf    *sf,
RgSchUlAlloc *alloc
)
#else
PUBLIC Void rgSCHUtlUlAllocRls(sf, alloc)
RgSchUlSf    *sf;
RgSchUlAlloc *alloc;
#endif
{
   RgSchUlAllocDb *allocDb = sf->allocDb;
   RgSchUlAlloc   *prv = alloc->prv;
   RgSchUlAlloc   *nxt = alloc->nxt;
   TRC2(rgSCHUtlUlAllocRls);

   alloc->ue = NULLP;
   alloc->raCb = NULLP;
   alloc->isAdaptive = FALSE;

   if(allocDb->count)
   {
      if (prv)
      {
         prv->nxt = nxt;
         if (nxt)           /* general case: this allocation lies btw two */
         {
            nxt->prv = prv;
         }
      }
      else
      {
         allocDb->first = nxt;
         if (nxt)
         {
            nxt->prv = NULLP;
         }
      }
      --allocDb->count;
      rgSCHUtlUlHoleAddAlloc(sf, alloc);
      rgSCHUtlUlAllocMemRls(&allocDb->mem, alloc);
   }
   else
   {

      printf("\nError: allocDb->count is ZERO ====\n");
   }

   //printf("\nallocDb->count:%u\n",allocDb->count);

   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleFirst
 *
 *     Desc : Get first (largest) hole
 *
 *     Ret  : RgSchUlHole *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlHole *rgSCHUtlUlHoleFirst
(
RgSchUlSf *sf
)
#else
PUBLIC RgSchUlHole *rgSCHUtlUlHoleFirst(sf)
RgSchUlSf *sf;
#endif
{
   TRC2(rgSCHUtlUlHoleFirst);
   RETVALUE(sf->holeDb->first);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleNxt
 *
 *     Desc : Get next largest hole
 *
 *     Ret  : RgSchUlHole *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlHole *rgSCHUtlUlHoleNxt
(
RgSchUlSf   *sf,
RgSchUlHole *hole
)
#else
PUBLIC RgSchUlHole *rgSCHUtlUlHoleNxt(sf, hole)
RgSchUlSf   *sf;
RgSchUlHole *hole;
#endif
{
   TRC2(rgSCHUtlUlHoleNxt);
   UNUSED(sf);
   RETVALUE(hole->nxt);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleAddAlloc
 *
 *     Desc : On freeing an alloc, add to hole
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleAddAlloc
(
RgSchUlSf    *sf,
RgSchUlAlloc *alloc
)
#else
PUBLIC Void rgSCHUtlUlHoleAddAlloc(sf, alloc)
RgSchUlSf    *sf;
RgSchUlAlloc *alloc;
#endif
{
   /* Note: rgSchCmnUlHoleUpdAllocLnks function that is used should not exist as
    * one, if such excessive branching is done (AllocNone, AllocNoPrv etc).
    * The excessive branching is meant to utilise the knowledge of whether prv
    * and nxt allocs exist or not. Hence for each kind (none, noprv, nonxt,
    * both), there should be a rgSchCmnUlHoleUpdAllocLnks... function (such as
    * rgSchCmnUlHoleUpdAllocLnksNone/NoPrv etc. */
   RgSchUlHoleDb *db = sf->holeDb;
   RgSchUlHole *prv = alloc->prvHole;
   RgSchUlHole *nxt = alloc->nxtHole;
   TRC2(rgSCHUtlUlHoleAddAlloc);

   if (prv)
   {
      if (nxt)
      {
         rgSCHUtlUlHoleJoin(db, prv, nxt, alloc);
      }
      else
         rgSCHUtlUlHoleExtndRight(db, prv, alloc);
   }
   else
   {
      if (nxt)
      {
         rgSCHUtlUlHoleExtndLeft(db, nxt, alloc);
      }
      else
         rgSCHUtlUlHoleNew(db, alloc);
   }

   /* increment the number of subbands getting freed to total available list */
   sf->availSubbands += alloc->numSb;

   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleJoin
 *
 *     Desc : Join two holes (due to alloc being deleted)
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleJoin
(
RgSchUlHoleDb *db,
RgSchUlHole   *prv,
RgSchUlHole   *nxt,
RgSchUlAlloc  *alloc
)
#else
PUBLIC Void rgSCHUtlUlHoleJoin(db, prv, nxt, alloc)
RgSchUlHoleDb *db;
RgSchUlHole   *prv;
RgSchUlHole   *nxt;
RgSchUlAlloc  *alloc;
#endif
{
   TRC2(rgSCHUtlUlHoleJoin);
   prv->num += alloc->numSb + nxt->num;
   rgSCHUtlUlHoleRls(db, nxt);
   rgSCHUtlUlHoleIncr(db, prv);
   rgSCHUtlUlHoleUpdAllocLnks(prv, alloc->prv, alloc->nxt);

   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleExtndRight
 *
 *     Desc : Extend hole due to alloc coming 'after' the hole
 *            being deleted
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleExtndRight
(
RgSchUlHoleDb *db,
RgSchUlHole   *prv,
RgSchUlAlloc  *alloc
)
#else
PUBLIC Void rgSCHUtlUlHoleExtndRight(db, prv, alloc)
RgSchUlHoleDb *db;
RgSchUlHole   *prv;
RgSchUlAlloc  *alloc;
#endif
{
   TRC2(rgSCHUtlUlHoleExtndRight);
   prv->num += alloc->numSb;
   rgSCHUtlUlHoleIncr(db, prv);
   rgSCHUtlUlHoleUpdAllocLnks(prv, alloc->prv, alloc->nxt);
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleExtndLeft
 *
 *     Desc : Extend hole due to alloc coming 'before' the hole
 *            being deleted
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleExtndLeft
(
RgSchUlHoleDb *db,
RgSchUlHole   *nxt,
RgSchUlAlloc  *alloc
)
#else
PUBLIC Void rgSCHUtlUlHoleExtndLeft(db, nxt, alloc)
RgSchUlHoleDb *db;
RgSchUlHole   *nxt;
RgSchUlAlloc  *alloc;
#endif
{
   TRC2(rgSCHUtlUlHoleExtndLeft);
   nxt->num += alloc->numSb;
   nxt->start = alloc->sbStart;
   rgSCHUtlUlHoleIncr(db, nxt);
   rgSCHUtlUlHoleUpdAllocLnks(nxt, alloc->prv, alloc->nxt);
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleNew
 *
 *     Desc : Create new hole due to alloc being deleted
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleNew
(
RgSchUlHoleDb *db,
RgSchUlAlloc  *alloc
)
#else
PUBLIC Void rgSCHUtlUlHoleNew(db, alloc)
RgSchUlHoleDb *db;
RgSchUlAlloc  *alloc;
#endif
{
   RgSchUlHole *hole = rgSCHUtlUlHoleMemGet(&db->mem);
#if (ERRCLASS & ERRCLS_DEBUG)
   if ( hole == NULLP )
   {
      RETVOID;
   }
#endif
   TRC2(rgSCHUtlUlHoleNew);
   hole->start = alloc->sbStart;
   hole->num = alloc->numSb;
   ++db->count;
   rgSCHUtlUlHoleIns(db, hole);
   rgSCHUtlUlHoleUpdAllocLnks(hole, alloc->prv, alloc->nxt);
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleUpdAllocLnks
 *
 *     Desc : Update alloc links in hole
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleUpdAllocLnks
(
RgSchUlHole  *hole,
RgSchUlAlloc *prvAlloc,
RgSchUlAlloc *nxtAlloc
)
#else
PUBLIC Void rgSCHUtlUlHoleUpdAllocLnks(hole, prvAlloc, nxtAlloc)
RgSchUlHole  *hole;
RgSchUlAlloc *prvAlloc;
RgSchUlAlloc *nxtAlloc;
#endif
{
   TRC2(rgSCHUtlUlHoleUpdAllocLnks);
   if (prvAlloc)
   {
      prvAlloc->nxtHole = hole;
   }
   if (nxtAlloc)
   {
      nxtAlloc->prvHole = hole;
   }
   hole->prvAlloc = prvAlloc;
   hole->nxtAlloc = nxtAlloc;
   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleIns
 *
 *     Desc : Insert (newly created) hole in sorted list of holes.
 *            Searches linearly, beginning with the largest hole.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleIns
(
RgSchUlHoleDb *db,
RgSchUlHole   *hole
)
#else
PUBLIC Void rgSCHUtlUlHoleIns(db, hole)
RgSchUlHoleDb *db;
RgSchUlHole   *hole;
#endif
{
   RgSchUlHole *cur;
   TRC2(rgSCHUtlUlHoleIns);

   if ((cur = db->first) != NULLP)
   {
      RgSchUlHole *nxt;
      if (cur->num < hole->num)
      {
         /* Add at front */
         hole->nxt = cur;
         cur->prv = hole;
         db->first = hole;
         hole->prv = NULLP;
         RETVOID;
      }

      for (nxt = cur->nxt; nxt; cur = nxt, nxt = nxt->nxt)
      {
         if (nxt->num < hole->num)
         {
            /* Insert hole:  cur <-> hole <-> nxt */
            cur->nxt = hole;
            hole->prv = cur;
            hole->nxt = nxt;
            nxt->prv = hole;
            RETVOID;
         }
      }

      /* Add at end */
      cur->nxt = hole;
      hole->prv = cur;
      hole->nxt = NULLP;
      RETVOID;
   }

   /* This is the first hole */
   db->first = hole;
   hole->prv = NULLP; /* may not be needed */
   hole->nxt = NULLP;
   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleIncr
 *
 *     Desc : hole->num has increeased, reposition in sorted
 *            list if needed
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleIncr
(
RgSchUlHoleDb *db,
RgSchUlHole   *hole
)
#else
PUBLIC Void rgSCHUtlUlHoleIncr(db, hole)
RgSchUlHoleDb *db;
RgSchUlHole   *hole;
#endif
{
   RgSchUlHole *cur;
   TRC2(rgSCHUtlUlHoleIncr);

   if ((cur = hole->prv) != NULLP)
   {
      RgSchUlHole *prv;

      if (cur->num > hole->num)
      {
         RETVOID;
      }

      /* Remove hole from current position */
      cur->nxt = hole->nxt;
      if (hole->nxt)
      {
         hole->nxt->prv = cur;
      }

      for (prv = cur->prv; prv; cur = prv, prv = prv->prv)
      {
         if (prv->num > hole->num)
         {
            /* Insert hole:  prv <-> hole <-> cur */
            prv->nxt = hole;
            hole->prv = prv;
            hole->nxt = cur;
            cur->prv = hole;
            RETVOID;
         }
      }

      /* Add at front */
      hole->nxt = cur;
      cur->prv = hole;
      db->first = hole;
      hole->prv = NULLP;
      RETVOID;
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleDecr
 *
 *     Desc : hole->num has decreeased, reposition in sorted
 *            list if needed
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleDecr
(
RgSchUlHoleDb *db,
RgSchUlHole   *hole
)
#else
PUBLIC Void rgSCHUtlUlHoleDecr(db, hole)
RgSchUlHoleDb *db;
RgSchUlHole   *hole;
#endif
{
   RgSchUlHole *cur;
   TRC2(rgSCHUtlUlHoleDecr);

   if ((cur = hole->nxt) != NULLP)
   {
      RgSchUlHole *nxt;

      if (cur->num < hole->num)
      {
         RETVOID;
      }

      /* Remove hole from current position */
      cur->prv = hole->prv;
      if (hole->prv)
      {
         hole->prv->nxt = cur;
      }
      else /* no prv, so cur to replace hole as first in list */
      {
         db->first = cur;
      }

      for (nxt = cur->nxt; nxt; cur = nxt, nxt = nxt->nxt)
      {
         if (nxt->num < hole->num)
         {
            /* Insert hole:  cur <-> hole <-> nxt */
            cur->nxt = hole;
            hole->prv = cur;
            hole->nxt = nxt;
            nxt->prv = hole;
            RETVOID;
         }
      }

      /* Add at end */
      cur->nxt = hole;
      hole->prv = cur;
      hole->nxt = NULLP;
      RETVOID;
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleRls
 *
 *     Desc : Releases hole.
 *            1. Decrements hole count.
 *            2. Deletes hole from list.
 *            3. Frees hole (hole memory release).
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleRls
(
RgSchUlHoleDb *db,
RgSchUlHole   *hole
)
#else
PUBLIC Void rgSCHUtlUlHoleRls(db, hole)
RgSchUlHoleDb *db;
RgSchUlHole   *hole;
#endif
{
   RgSchUlHole *prv = hole->prv;
   RgSchUlHole *nxt = hole->nxt;
   TRC2(rgSCHUtlUlHoleRls);

   --db->count;
   if (prv)
   {
      prv->nxt = nxt;
      if (nxt)
      {
         nxt->prv = prv;
      }
   }
   else
   {
      db->first = nxt;
      if (nxt)
      {
         nxt->prv = NULLP;
      }
   }

   rgSCHUtlUlHoleMemRls(&db->mem, hole);
   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocMemInit
 *
 *     Desc : Initialises alloc free pool
 *
 *     Ret  : S16 (ROK/RFAILED)
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHUtlUlAllocMemInit
(
RgSchCellCb     *cell,
RgSchUlAllocMem *mem,
U8           maxAllocs
)
#else
PUBLIC S16 rgSCHUtlUlAllocMemInit(cell, mem, maxAllocs)
RgSchCellCb     *cell;
RgSchUlAllocMem *mem;
U8           maxAllocs;
#endif
{
   S16 ret;
   RgSchUlAlloc *allocs;
   TRC2(rgSCHUtlUlAllocMemInit);

   ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data **)&allocs,
                           maxAllocs * sizeof(*allocs));
   if (ret != ROK)
   {
      RETVALUE(ret);
   }
   mem->allocs = allocs;
   mem->maxAllocs = maxAllocs;
   if (mem->maxAllocs == 1)
   {
      allocs[0].prv = NULLP;
      allocs[0].nxt = NULLP;
   }
   else
   {
      U8 i;
      allocs[0].prv = NULLP;
      allocs[0].nxt = &allocs[1];
      for (i = 1; i < mem->maxAllocs - 1; ++i)
      {
         allocs[i].prv = &allocs[i-1];
         allocs[i].nxt = &allocs[i+1];
      }
      allocs[i].prv = &allocs[i-1];
      allocs[i].nxt = NULLP;
   }
   mem->firstFree = &allocs[0];
   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocMemDeinit
 *
 *     Desc : Deinitialises alloc free pool
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlAllocMemDeinit
(
RgSchCellCb     *cell,
RgSchUlAllocMem *mem
)
#else
PUBLIC Void rgSCHUtlUlAllocMemDeinit(cell, mem)
RgSchCellCb     *cell;
RgSchUlAllocMem *mem;
#endif
{
   TRC2(rgSCHUtlUlAllocMemDeinit);
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(mem->allocs)),
                    mem->maxAllocs * sizeof(*mem->allocs));
   mem->maxAllocs = 0;
   mem->firstFree = NULLP;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleMemInit
 *
 *     Desc : Initialises hole free pool. Assumes maxHoles
 *            to be at least 2.
 *
 *     Ret  : S16 (ROK/RFAILED)
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHUtlUlHoleMemInit
(
RgSchCellCb    *cell,
RgSchUlHoleMem *mem,
U8          maxHoles,
RgSchUlHole    **holeRef
)
#else
PUBLIC S16 rgSCHUtlUlHoleMemInit(cell, mem, maxHoles, holeRef)
RgSchCellCb     *cell;
RgSchUlHoleMem *mem;
U8          maxHoles;
RgSchUlHole    **holeRef;
#endif
{
   S16 ret;
   RgSchUlHole *holes;
   TRC2(rgSCHUtlUlHoleMemInit);

   ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data **)&holes,
                           maxHoles * sizeof(*holes));
   if (ret != ROK)
   {
      RETVALUE(ret);
   }

   mem->holes = holes;
   mem->maxHoles = maxHoles;

   /* first hole is taken up */
   holes[0].prv = NULLP; /* not needed */
   holes[0].nxt = NULLP; /* not needed */
   *holeRef = &holes[0];

   if (mem->maxHoles == 2)
   {
      holes[1].prv = NULLP; /* may not be needed */
      holes[1].nxt = NULLP; /* may not be needed */
   }
   else
   {
      U8 i;
      holes[1].prv = NULLP;
      holes[0].nxt = &holes[1];
      for (i = 1; i < mem->maxHoles - 1; ++i)
      {
         holes[i].prv = &holes[i-1];
         holes[i].nxt = &holes[i+1];
      }
      holes[i].prv = &holes[i-1];
      holes[i].nxt = NULLP;
   }
   mem->firstFree = &holes[1];

   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleMemDeinit
 *
 *     Desc : Deinitialises hole free pool
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleMemDeinit
(
RgSchCellCb    *cell,
RgSchUlHoleMem *mem
)
#else
PUBLIC Void rgSCHUtlUlHoleMemDeinit(cell, mem)
RgSchCellCb    *cell;
RgSchUlHoleMem *mem;
#endif
{
   TRC2(rgSCHUtlUlHoleMemDeinit);
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(mem->holes)),
                    mem->maxHoles * sizeof(*mem->holes));
   mem->maxHoles = 0;
   mem->firstFree = NULLP;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocMemGet
 *
 *     Desc : Gets an 'alloc' from the free pool
 *
 *     Ret  : RgSchUlAlloc *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocMemGet
(
RgSchUlAllocMem *mem
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocMemGet(mem)
RgSchUlAllocMem *mem;
#endif
{
   RgSchUlAlloc *alloc;
   TRC2(rgSCHUtlUlAllocMemGet);

#if (ERRCLASS & ERRCLS_DEBUG)
   if (mem->firstFree == NULLP)
   {
      RETVALUE(NULLP);
   }
#endif

   alloc = mem->firstFree;
   mem->firstFree = alloc->nxt;
   alloc->nxt = NULLP; /* probably not needed */
   /* alloc->prv might already be NULLP, in case was needed to set it to NULLP */

   RETVALUE(alloc);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocMemRls
 *
 *     Desc : Returns an 'alloc' to the free pool
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlAllocMemRls
(
RgSchUlAllocMem *mem,
RgSchUlAlloc    *alloc
)
#else
PUBLIC Void rgSCHUtlUlAllocMemRls(mem, alloc)
RgSchUlAllocMem *mem;
RgSchUlAlloc    *alloc;
#endif
{
   TRC2(rgSCHUtlUlAllocMemRls);
   alloc->prv = NULLP;

   alloc->nxt = mem->firstFree;
   if (mem->firstFree != NULLP)
   {
      mem->firstFree->prv = alloc;
   }
   mem->firstFree = alloc;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleMemGet
 *
 *     Desc : Gets a 'hole' from the free pool
 *
 *     Ret  : RgSchUlHole *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlHole *rgSCHUtlUlHoleMemGet
(
RgSchUlHoleMem *mem
)
#else
PUBLIC RgSchUlHole *rgSCHUtlUlHoleMemGet(mem)
RgSchUlHoleMem *mem;
#endif
{
   RgSchUlHole *hole;
   TRC2(rgSCHUtlUlHoleMemGet);

#if (ERRCLASS & ERRCLS_DEBUG)
   if (mem->firstFree == NULLP)
   {
      RETVALUE(NULLP);
   }
#endif

   hole = mem->firstFree;
   mem->firstFree = hole->nxt;
   mem->firstFree->prv = NULLP; /* may not be needed, under error class */
   hole->nxt = NULLP; /* probably not needed */
   /* hole->prv is might already be NULLP, in case was needed to set it to NULLP */

   RETVALUE(hole);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleMemRls
 *
 *     Desc : Returns a 'hole' to the free pool
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleMemRls
(
RgSchUlHoleMem *mem,
RgSchUlHole    *hole
)
#else
PUBLIC Void rgSCHUtlUlHoleMemRls(mem, hole)
RgSchUlHoleMem *mem;
RgSchUlHole    *hole;
#endif
{
   TRC2(rgSCHUtlUlHoleMemRls);
   hole->prv = NULLP;

   hole->nxt = mem->firstFree;
   if (mem->firstFree != NULLP)
   {
      mem->firstFree->prv = hole;
   }
   mem->firstFree = hole;
   RETVOID;
}

/**
 * @brief Get an alloc from the specified position in the BW.
 *
 * @details
 *
 *     Function : rgSCHUtlUlGetSpfcAlloc
 *
 *      - Return an alloc from the specified position in the BW.
 *        Note: This function assumes there is always a hole
 *              Existing which completely has the specified
 *              allocation. The reason for such an assumption is
 *              the function's usage as of now guarantees that there
 *              will always be such hole. And also for efficiency.
 *
 *  @param[in]  RgSchUlSf     *sf
 *  @param[in]  U8            startSb
 *  @param[in]  U8            numSb
 *  @return  RgSchUlAlloc*
 **/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlUlGetSpfcAlloc
(
RgSchUlSf       *sf,
U8              startSb,
U8              numSb
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlUlGetSpfcAlloc(sf, startSb, numSb)
RgSchUlSf       *sf;
U8              startSb;
U8              numSb;
#endif
{
   RgSchUlHole     *hole, *nxtHole;
   RgSchUlAlloc    *alloc = NULLP;
   TRC2(rgSCHUtlUlGetSpfcAlloc);

   if ((hole = rgSCHUtlUlHoleFirst(sf)) == NULLP)
   {
      RETVALUE(NULLP);
   }
   do
   {
      nxtHole = rgSCHUtlUlHoleNxt(sf, hole);
      if ((startSb >= hole->start) &&
          (startSb+numSb <= hole->start+hole->num))
      {
         if (startSb != hole->start)
         {
            /* Create a new hole to accomodate Subbands between
             * hole start and req alloc start */
            RgSchUlHole *newHole = rgSCHUtlUlHoleMemGet(&(sf->holeDb->mem));

#if (ERRCLASS & ERRCLS_DEBUG)
            if ( newHole == NULLP )
            {
                RETVALUE( NULLP );
            }
#endif
            newHole->start = hole->start;
            newHole->num = startSb - hole->start;
            hole->start = startSb;
            /* [ccpu00122847]-MOD- Correctly updating the hole->num */
            hole->num -= newHole->num;
            ++(sf->holeDb->count);
            rgSCHUtlUlHoleIns(sf->holeDb, newHole);
            newHole->prvAlloc = hole->prvAlloc;
            if (newHole->prvAlloc)
            {
               newHole->prvAlloc->nxtHole = newHole;
            }
            if (numSb == hole->num)
            {
               alloc = rgSCHUtlUlAllocGetCompHole(sf, hole);
            }
            else
            {
               alloc = rgSCHUtlUlAllocGetPartHole(sf, numSb, hole);
            }
            alloc->prvHole = newHole;
            newHole->nxtAlloc = alloc;
         }
         else /* Hole start and req alloc start are same */
         {
            if (numSb == hole->num)
            {
               alloc = rgSCHUtlUlAllocGetCompHole(sf, hole);
            }
            else
            {
               alloc = rgSCHUtlUlAllocGetPartHole(sf, numSb, hole);
            }
         }
         break;
      }
   } while ((hole = nxtHole) != NULLP);
   RETVALUE(alloc);
}
#ifdef LTE_L2_MEAS
/**
 * @brief  Validates the qci values
 *
 * @details
 *
 *     Function :rgSCHUtlValidateQci
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  U8              numQci
 *  @param[out] U8              *qci
 *  @return  S16
 *           ROK
 *           RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHUtlValidateQci
(
RgSchCellCb  *cellCb,
U8           numQci,
U8           *qci
)
#else
PRIVATE S16 rgSCHUtlValidateQci(cellCb, numQci, qci)
RgSchCellCb  *cellCb;
U8           numQci;
U8           *qci;
#endif
{
   U8        qciIdx;
   U8        qciVal;

   TRC3(rgSCHUtlValidateQci)

   for(qciIdx = 0; qciIdx < numQci; qciIdx++)
   {
      qciVal = qci[qciIdx];
      if(qciVal == 0 || qciVal > 9)
      {
         RETVALUE(RFAILED);
      }
      if(qciVal != cellCb->qciArray[qciVal].qci)
      {
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}/* rgSCHUtlValidateQci */
/**
 * @brief  Validates the measurement request parameters.
 *
 * @details
 *
 *     Function :rgSCHUtlValidateMeasReq
 *
 *  @param[in]  RgSchCellCb        *cellCb
 *  @param[in]  LrgSchMeasReqInfo  *schL2MeasInfo
 *  @param[out] RgSchErrInfo       *err
 *  @return  RgSchUlAlloc*
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlValidateMeasReq
(
RgSchCellCb       *cellCb, 
LrgSchMeasReqInfo *schL2MeasInfo,
RgSchErrInfo      *err
)
#else
PUBLIC S16 rgSCHUtlValidateMeasReq(cellCb, schL2MeasInfo, err)
RgSchCellCb       *cellCb;
LrgSchMeasReqInfo *schL2MeasInfo;
RgSchErrInfo      *err;
#endif
{
   U16    measType;
   S16   ret;

   TRC3(rgSCHUtlValidateMeasReq)

   measType = schL2MeasInfo->measType;

   if((measType == 0) ||
       measType > 2047)
   {
      err->errType = RGSCHERR_SCH_INVALID_MEAS_TYPE;
      err->errCause = RGSCHERR_SCH_L2MEAS;
      RETVALUE(RFAILED);
   }
   if((schL2MeasInfo->timePrd !=0) &&
      (measType & LRG_L2MEAS_AVG_PRB_PER_QCI_DL) &&
      ((schL2MeasInfo->avgPrbQciDl.numQci > LRG_MAX_QCI_PER_REQ)||
       (schL2MeasInfo->avgPrbQciDl.numQci == 0)))
   {
      err->errType = RGSCHERR_SCH_INVALID_PARAM_RANGE;
      err->errCause = RGSCHERR_SCH_L2MEAS;
      RETVALUE(RFAILED);
   }
   if((schL2MeasInfo->timePrd !=0) &&
     (measType & LRG_L2MEAS_AVG_PRB_PER_QCI_UL) &&
      (schL2MeasInfo->avgPrbQciUl.numQci > LRG_MAX_QCI_PER_REQ)) 
   {
      err->errType = RGSCHERR_SCH_INVALID_PARAM_RANGE;
      err->errCause = RGSCHERR_SCH_L2MEAS;
      RETVALUE(RFAILED);
   }
   if((measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL) &&
         ((schL2MeasInfo->nmbActvUeQciDl.numQci > LRG_MAX_QCI_PER_REQ) ||
          (schL2MeasInfo->nmbActvUeQciDl.sampPrd == 0)||
          ((schL2MeasInfo->timePrd !=0)&&
           (schL2MeasInfo->timePrd < schL2MeasInfo->nmbActvUeQciDl.sampPrd)) ||
         (schL2MeasInfo->nmbActvUeQciDl.sampPrd > LRG_MAX_SAMP_PRD)))
   {
      err->errType = RGSCHERR_SCH_INVALID_PARAM_RANGE;
      err->errCause = RGSCHERR_SCH_L2MEAS;
      RETVALUE(RFAILED);
   }
   if((measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL) &&
      ((schL2MeasInfo->nmbActvUeQciUl.numQci > LRG_MAX_QCI_PER_REQ) ||
       (schL2MeasInfo->nmbActvUeQciUl.sampPrd == 0)||
       ((schL2MeasInfo->timePrd !=0) &&
        (schL2MeasInfo->timePrd < schL2MeasInfo->nmbActvUeQciUl.sampPrd)) ||
        (schL2MeasInfo->nmbActvUeQciUl.sampPrd > LRG_MAX_SAMP_PRD)))
   {
      err->errType = RGSCHERR_SCH_INVALID_PARAM_RANGE;
      err->errCause = RGSCHERR_SCH_L2MEAS;
      RETVALUE(RFAILED);
   }
   if((schL2MeasInfo->timePrd !=0) &&
       (measType & LRG_L2MEAS_AVG_PRB_PER_QCI_DL))
   {
      RGSCH_ARRAY_BOUND_CHECK(cellCb->instIdx, schL2MeasInfo->avgPrbQciDl.qci,  \
      (schL2MeasInfo->avgPrbQciDl.numQci));
      ret  = rgSCHUtlValidateQci(cellCb, schL2MeasInfo->avgPrbQciDl.numQci,
                                 schL2MeasInfo->avgPrbQciDl.qci);
      if(ret != ROK)
      {
          err->errType = RGSCHERR_SCH_INVALID_QCI_VAL;
          err->errCause = RGSCHERR_SCH_L2MEAS;
          RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}/* rgSCHUtlValidateMeasReq */
#endif /* LTE_L2_MEAS */
/******* </AllocHolesMemMgmnt>: END *****/
#ifdef RGR_SI_SCH
/**
 * @brief API for sending SI configuration confirm from Scheduler to RRM
 *
 * @details
 *
 *     Function: rgSCHUtlRgrSiCfgCfm
 *
 *     This API is invoked to send SI configuration confirm from Scheduler
 *     to RRM.
 *     This API fills in Pst structure and SAP Ids and invokes
 *     config confirm API towards RRM.
 *
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  U8            status
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrSiCfgCfm
(
Inst          instId,
SpId          spId,
RgrCfgTransId transId,
U8            status
)
#else
PUBLIC S16 rgSCHUtlRgrSiCfgCfm(instId, spId, transId, status)
Inst          instId;
SpId          spId;
RgrCfgTransId transId;
U8            status;
#endif
{
   U8        prntTrans[RGR_CFG_TRANSID_SIZE+1];

   TRC2(rgSCHUtlRgrSiCfgCfm)

   cmMemcpy((U8 *)prntTrans, (U8 *)transId.trans, RGR_CFG_TRANSID_SIZE);
   prntTrans[RGR_CFG_TRANSID_SIZE] = '\0';


   if(RgUiRgrSiCfgCfm(&rgSchCb[instId].rgrSap[spId].sapCfg.sapPst,
                    rgSchCb[instId].rgrSap[spId].sapCfg.suId,
                    transId, status) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,instId,"rgSCHUtlRgrSiCfgCfm: "
                "RgUiRgrSiCfgCfm Failed ");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* rgSCHUtlRgrSiCfgCfm */


/**
 * @brief API for sending Warning SI configuration confirm from 
 * Scheduler to RRM
 *
 * @details
 *
 *
 *     This API is invoked to send Warning SI configuration confirm 
 *     from Scheduler to RRM.
 *     This API fills in Pst structure and SAP Ids and invokes
 *     config confirm API towards RRM.
 *
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  U8            status
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrWarningSiCfgCfm
(
Inst          instId,
SpId          spId,
U8            siId,
RgrCfgTransId transId,
U8            status
)
#else
PUBLIC S16 rgSCHUtlRgrWarningSiCfgCfm(instId, spId, siId, transId, status)
Inst          instId;
SpId          spId;
U8            siId;
RgrCfgTransId transId;
U8            status;
#endif
{
   U8        prntTrans[RGR_CFG_TRANSID_SIZE+1];

   TRC2(rgSCHUtlRgrWarningSiCfgCfm)

   cmMemcpy((U8 *)prntTrans, (U8 *)transId.trans, RGR_CFG_TRANSID_SIZE);
   prntTrans[RGR_CFG_TRANSID_SIZE] = '\0';


   if(RgUiRgrWarningSiCfgCfm(&rgSchCb[instId].rgrSap[spId].sapCfg.sapPst,
                    rgSchCb[instId].rgrSap[spId].sapCfg.suId, 
                    transId, siId, status) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,instId,"rgSCHUtlRgrSiCfgCfm: "
                "RgUiRgrSiCfgCfm Failed ");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* rgSCHUtlRgrWarningSiCfgCfm */

/***********************************************************
 *
 *     Func : rgSCHUtlPutSiInfo
 *
 *     Desc : Utility Function to deallocate SI information
 *
 *
 *            RFAILED
 *
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlPutSiInfo
(
RgSchCellCb *cell
)
#else
PUBLIC Void rgSCHUtlPutSiInfo(cell)
RgSchCellCb *cell;
#endif
{
   U8    idx = 0;
   U32   sizeOfSiInfo = 0;
   TRC2(rgSCHUtlPutSiInfo)
   /*Free the buffers in crntSiInfo*/
   RGSCH_FREE_MSG(cell->siCb.crntSiInfo.mib)
   RGSCH_FREE_MSG(cell->siCb.crntSiInfo.sib1Info.sib1)
   
   sizeOfSiInfo = sizeof(cell->siCb.crntSiInfo.siInfo)/sizeof(cell->siCb.crntSiInfo.siInfo[0]);
   
   for(idx=0; idx < sizeOfSiInfo; idx++)
   {
      RGSCH_FREE_MSG(cell->siCb.crntSiInfo.siInfo[idx].si)
   }

   /*Free the buffers in newSiInfo */
   RGSCH_FREE_MSG(cell->siCb.newSiInfo.mib)
   RGSCH_FREE_MSG(cell->siCb.newSiInfo.sib1Info.sib1)

   sizeOfSiInfo = sizeof(cell->siCb.newSiInfo.siInfo)/sizeof(cell->siCb.newSiInfo.siInfo[0]);

   for(idx=0; idx < sizeOfSiInfo; idx++)
   {
      RGSCH_FREE_MSG(cell->siCb.newSiInfo.siInfo[idx].si)
   }

   RETVOID;
}
#endif /*RGR_SI_SCH */



/***********************************************************
 *
 *     Func : rgSCHUtlGetDrxSchdUesInDl
 *
 *     Desc : Utility Function to fill the get the list of
 *            scheduled UEs. On these UE's, drx-inactivity
 *            timer will be started/restarted.
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes:
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHUtlGetDrxSchdUesInDl
(
RgSchCellCb     *cellCb,
RgSchUeCb       *ueCb,
RgSchDlHqProcCb *dlHq,
RgInfUeAlloc    *allocInfo,
CmLListCp       *dlDrxInactvTmrLst,
CmLListCp       *dlInActvLst,
CmLListCp       *ulInActvLst
)
#else
PUBLIC S16 rgSCHUtlGetDrxSchdUesInDl(cellCb, ueCb, dlHq, allocInfo, dlDrxInactvTmrLst, dlInActvLst, ulInActvLst)
RgSchCellCb     *cellCb;
RgSchUeCb       *ueCb;
RgSchDlHqProcCb *dlHq;
RgInfUeAlloc    *allocInfo;
CmLListCp       *dlDrxInactvTmrLst;
CmLListCp       *dlInActvLst;
CmLListCp       *ulInActvLst;
#endif
{
   Bool                  isNewTx = FALSE;
   U8                    idx;
   RgSchDrxDlHqProcCb    *drxHq;
   RgSchDRXCellCb        *drxCell = cellCb->drxCb;
   RgSchDrxUeCb          *drxUe;
#ifdef DEBUGP
   Inst                  inst = cellCb->instIdx;
#endif
   U8                    cellIdx = ueCb->cellIdToCellIdxMap[RG_SCH_CELLINDEX(dlHq->hqE->cell)];
   U32                   dlInactvMask;
   U32                   ulInactvMask;

   for(idx = 0; idx < allocInfo->nmbOfTBs; idx++)
   {
      if(allocInfo->tbInfo[idx].isReTx == FALSE)
      {
         isNewTx = TRUE;
         /* Removing break here, since in 2 TB case if 2nd TB is proceeding with 
            retx then drxretx timer should be stopped.*/
      }
      else
      {
         /*Stop the DRX retransmission timer as UE scheduled for retx. Here
          * we stop the timer and inactivate the UE for both UL and DL.
          * This may result in loss of one slot for UL but this trade
          * off is taken to avoid the overhead of maintaining a list of UEs
          * to be inactivated in the next slot.*/
         drxHq = RG_SCH_DRX_GET_DL_HQ(dlHq);
         drxUe = RG_SCH_DRX_GET_UE(ueCb);
         if(drxHq->reTxIndx != DRX_INVALID)
         {  
            /* This condition should never occur */      
            if(drxHq->reTxIndx >= RG_SCH_MAX_DRXQ_SIZE)
            {
               RGSCHDBGERRNEW(inst,(rgSchPBuf(inst),"[%d]UE:DRXUE RETX IDX[%d]"
                        "is out of bound,dlInactvMask %d,procId %d\n", ueCb->ueId,
                        drxHq->reTxIndx,ueCb->dl.dlInactvMask, dlHq->procId));
            }

            drxUe->drxDlInactvMaskPerCell[cellIdx]  |= (RG_SCH_DRX_DLHQ_BITMASK << dlHq->procId);
            drxUe->drxUlInactvMaskPerCell[cellIdx]  |= (RG_SCH_DRX_DLHQ_BITMASK << dlHq->procId);

            dlInactvMask = RG_SCH_DRX_DLHQ_BITMASK << dlHq->procId;
            ulInactvMask = RG_SCH_DRX_DLHQ_BITMASK << dlHq->procId;

            for(cellIdx = 0; cellIdx < CM_LTE_MAX_CELLS; cellIdx++)
            {
               dlInactvMask &= drxUe->drxDlInactvMaskPerCell[cellIdx];
               ulInactvMask &= drxUe->drxUlInactvMaskPerCell[cellIdx];
            }

            drxUe->drxDlInactvMask |= dlInactvMask;
            drxUe->drxUlInactvMask |= ulInactvMask;

            /* if no other condition is keeping ue active,
             * inactivate the Ue
             */
            if(!RG_SCH_DRX_DL_IS_UE_ACTIVE(drxUe))
            {
               /* BUG 2 : HARQ_RTT, changed for consistency */
               ueCb->dl.dlInactvMask |= (RG_DRX_INACTIVE);

               /* Add to DL inactive list */
               cmLListAdd2Tail(dlInActvLst,&(ueCb->dlDrxInactvLnk));
               ueCb->dlDrxInactvLnk.node = (PTR)ueCb;
            }

            if(!RG_SCH_DRX_UL_IS_UE_ACTIVE(drxUe))
            {
               /*BUG 2: HARQ_RTT changed for consistency */
               ueCb->ul.ulInactvMask |= (RG_DRX_INACTIVE);

               cmLListAdd2Tail(ulInActvLst,&(ueCb->ulDrxInactvLnk));
               ueCb->ulDrxInactvLnk.node  = (PTR)ueCb;
            }

            /* Deleting entry from HARQ RTT queue for the same HARQ proc, 
             * if exist. This is the special case which can happen iF UL 
             * scheduling is done later. */
            if(drxHq->rttIndx != DRX_INVALID)
            {
               cmLListDelFrm (&(cellCb->drxCb->drxQ[drxHq->rttIndx].harqRTTQ),
                     &(drxHq->harqRTTEnt));

               drxHq->rttIndx = DRX_INVALID;
            }   

            cmLListDelFrm (&(drxCell->drxQ[drxHq->reTxIndx].harqRetxQ),
                  &(drxHq->harqRetxEnt));
            drxHq->reTxIndx = DRX_INVALID;
         }
      }
   }

   if(isNewTx == TRUE)
   {
      if(ueCb->drxCb->raRcvd == TRUE)
      {
         ueCb->drxCb->raRcvd = FALSE;

         /* mark the ra bit */
         ueCb->drxCb->drxUlInactvMask |= RG_SCH_DRX_RA_BITMASK;
         ueCb->drxCb->drxDlInactvMask |= RG_SCH_DRX_RA_BITMASK;

      }/*if(ra->rcvd) == TRUE */

      if(ueCb->dlDrxInactvTmrLnk.node == NULLP)
      {   
         cmLListAdd2Tail(dlDrxInactvTmrLst,&(ueCb->dlDrxInactvTmrLnk));
         ueCb->dlDrxInactvTmrLnk.node = (PTR)ueCb;
      }
   }/*if(isNewTx == TRUE) */

   RETVALUE(ROK);
}/* rgSCHUtlGetSchdUes*/

/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
/**
 * @brief This function fills StaInd struct
 *
 * @details
 *
 *     Function: rgSCHUtlFillSndStaInd
 *     Purpose:  Fills StaInd struct and sends the
 *               StaInd to RRM
 *
 *  @param[in]  RgSchCellCb        *cell  pointer to Cell Control block
 *  @param[in]  RgSchUeCb          *ue  pointer to Ue Control block
 *  @param[in]  RgrStaIndInfo      *staInfo Sta Ind struct to be filled
 *  @param[in]  U8                 numCqiRept NUmber of reports to be filled
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlFillSndStaInd
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrStaIndInfo      *staInfo,
U8                 numCqiRept
)
#else
PUBLIC S16 rgSCHUtlFillSndStaInd(cell, ue, staInfo, numCqiRept)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrStaIndInfo      *staInfo;
U8                 numCqiRept;
#endif
{
   U8 idxStart;

   /* Fill StaInd for sending collated Latest N CQI rpeorts */
   /* Find index in the array from where Latest N
      reports needs to be fetched. Use this value to index in the array
      and copy the reports into staInfo */

   /* Fill the Cell Id of PCC of the UE */
   staInfo->cellId = ue->cell->cellId;
   staInfo->crnti = ue->ueId;

   idxStart = ue->schCqiInfo.cqiCount - numCqiRept;

   cmMemcpy ((U8*)&(staInfo->ueCqiInfo.cqiRept),
            (U8*)&(ue->schCqiInfo.cqiRept[idxStart]),
            numCqiRept * sizeof(RgrUeCqiRept));

   staInfo->ueCqiInfo.numCqiRept = numCqiRept;

   ue->schCqiInfo.cqiCount = 0;

   /* Call utility function (rgSCHUtlRgrStaInd) to send rpts to RRM */
   if(rgSCHUtlRgrStaInd(cell, staInfo) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"Could not send "
         "CQI reports for RNTI:%d",ue->ueId);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

}/* End of rgSCHUtlFillSndStaInd */



/**
 * @brief API for sending STA indication from Scheduler to RRM.
 *
 * @details
 *
 *     Function: rgSCHUtlRgrStaInd
 *
 *     This API is invoked to send STA indication from Scheduler instance to RRM.
 *     This API fills in Pst structure and RgrStaIndInfo
 *     and calls the Sta primitive API towards RRM.
 *
 *  @param[in]  cell                   RgSchCellCb
 *  @param[in]  RgrStsIndInfo          *rgrSta
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrStaInd
(
RgSchCellCb          *cell,
RgrStaIndInfo        *rgrSta
)
#else
PUBLIC S16 rgSCHUtlRgrStaInd(cell, rgrSta)
RgSchCellCb          *cell;
RgrStaIndInfo        *rgrSta;
#endif
{
   S16           ret = ROK;
   RgSchUpSapCb  *rgrSap;                    /*!< RGR SAP Control Block */

   TRC2(rgSCHUtlRgrStaInd)


   rgrSap = cell->rgrSap;
   if (rgrSap->sapSta.sapState != LRG_BND)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
               "rgSCHUtlRgrStaInd() Upper SAP not bound (%d) ",
               rgrSap->sapSta.sapState);
      RETVALUE(RFAILED);
   }
   RgUiRgrStaInd(&(cell->rgrSap->sapCfg.sapPst),
         cell->rgrSap->sapCfg.suId, rgrSta);
   RETVALUE(ret);
}  /* rgSCHUtlRgrStaInd*/
#endif /* End of RGR_CQI_REPT */

/* Fix : syed HO UE does not have a valid ue->rntiLnk */
/**
 * @brief Indicates MAC to release any rnti context it has.
 *
 * @details
 *     Function : rgSCHUtlIndRntiRls2Mac 
 *       This function indicates MAC for this rnti release.
 *       In case of ueId change it will indicate MAC
 *       about the new rnti to be updated.
 *       It will post a release RNTI indication to MAC.
 *     
 *    
 *           
 *  @param[in]     RgSchCellCb    *cell
 *  @param[in]     CmLteRnti      rnti 
 *  @param[in]     Bool           ueIdChng
 *  @param[in]     CmLteRnti      newRnti
 *  @return  Void
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlIndRntiRls2Mac 
(
RgSchCellCb    *cell,
CmLteRnti      rnti,
Bool           ueIdChng,
CmLteRnti      newRnti
)
#else
PUBLIC Void rgSCHUtlIndRntiRls2Mac(cell, rnti, ueIdChng, newRnti)
RgSchCellCb    *cell;
CmLteRnti      rnti;
Bool           ueIdChng;
CmLteRnti      newRnti;
#endif
{
   Pst          pst;
   Inst         inst = cell->instIdx;
   RgInfRlsRnti rntiInfo;

   TRC2(rgSCHUtlIndRntiRls2Mac)

   /* Copy the info to rntiInfo */
   rntiInfo.cellId = cell->cellId;
   rntiInfo.rnti   = rnti;
   /* Fix : syed ueId change as part of reestablishment.
    * Now SCH to trigger this. CRG ueRecfg for ueId change 
    * is dummy */	   
   rntiInfo.ueIdChng = ueIdChng;
   rntiInfo.newRnti  = newRnti;
#ifdef LTE_ADV
   rntiInfo.isUeSCellDel = FALSE;
#endif
   /* Invoke MAC to release the rnti */
   rgSCHUtlGetPstToLyr(&pst, &rgSchCb[inst], cell->macInst);
   RgSchMacRlsRnti(&pst, &rntiInfo);
   RETVOID;
}

/* LTE_ADV_FLAG_REMOVED_START */
/**
 * @brief API for sending LOAD INF indication from Scheduler to RRM.
 * @details
 *
 *     Function: rgSCHUtlRgrLoadInfInd
 *
 *     This API is invoked to send LOAD INF indication from Scheduler instance to RRM.
 *     This API fills in Pst structure and RgrLoadInfIndInfo
 *     and calls the Sta primitive API towards RRM.
 *
 *  @param[in]  cell                    RgSchCellCb
 *  @param[in]  RgrLoadInfIndInfo       *rgrLoadInf
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrLoadInfInd
(
 RgSchCellCb           *cell,
 RgrLoadInfIndInfo     *rgrLoadInf
 )
#else
PUBLIC S16 rgSCHUtlRgrLoadInfInd(cell, rgrLoadInf)
   RgSchCellCb           *cell;
   RgrLoadInfIndInfo     *rgrLoadInf;
#endif
{
   S16           ret = ROK;
   RgSchUpSapCb  *rgrSap;                    /*!< RGR SAP Control Block */

   TRC2(rgSCHUtlRgrLoadInfInd)


   rgrSap = cell->rgrSap;
   if (rgrSap->sapSta.sapState != LRG_BND)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
               "rgSCHUtlRgrLoadInfInd() Upper SAP not bound (%d) ",
               rgrSap->sapSta.sapState);
      RETVALUE(RFAILED);
   }
   RgUiRgrLoadInfInd(&(cell->rgrSap->sapCfg.sapPst),
         cell->rgrSap->sapCfg.suId, rgrLoadInf);
   RETVALUE(ret);
}  /* rgSCHUtlRgrLoadInfInd*/
/* LTE_ADV_FLAG_REMOVED_END */

/* MS_FIX : syed SCH to act as MASTER in maintaining
 * rnti related context. Trigger to rnti del/Chng at SCH
 * will result in a Indication to MAC to release its
 * RNTI context. MAC inturn indicates the context cleared
 * indication to SCH, upon which SCH would set this
/**
 * @brief API for sending STA indication from Scheduler to RRM.
 *
 * @details
 *
 *     Function: rgSCHUtlRlsRnti
 *
 *     This API is invoked to indicate MAC to release rnti
 *
 *  @param[in]  RgSchCellCb    *cellCb                   
 *  @param[in]  RgSchRntiLnk   *rntiLnk,
 *  @param[in]  Bool           ueIdChngd,
 *  @param[in]  CmLteRnti      newRnti
 *  @return  Void
 **/

#ifdef ANSI
PUBLIC Void rgSCHUtlRlsRnti
(
RgSchCellCb    *cell,
RgSchRntiLnk   *rntiLnk,
Bool           ueIdChngd,
CmLteRnti      newRnti
)
#else
PUBLIC Void rgSCHUtlRlsRnti(cell, rntiLnk, ueIdChngd, newRnti)
RgSchCellCb    *cell;
RgSchRntiLnk   *rntiLnk;
Bool           ueIdChngd;
CmLteRnti      newRnti;
#endif
{

   TRC2(rgSCHUtlRlsRnti)
   U8 isLegacy = 0;
#ifdef EMTC_ENABLE
   if(cell->emtcEnable)
   {
      rgSCHEmtcUtlRlsRnti(cell, rntiLnk, &isLegacy);
   }
#endif
   if(!isLegacy)
   {
      /*Add to Guard Pool*/
      cmLListAdd2Tail(&cell->rntiDb.rntiGuardPool, &rntiLnk->rntiGrdPoolLnk);
      rntiLnk->rntiGrdPoolLnk.node = (PTR)rntiLnk;
   }
   /* Fix: syed Explicitly Inidcate MAC to release RNTI */
   rgSCHUtlIndRntiRls2Mac(cell, rntiLnk->rnti, ueIdChngd, newRnti);

   RETVOID;
}


/**
 * @brief This function fills StaInd struct
 *
 * @details
 *
 *     Function: rgSCHUtlFillSndUeStaInd
 *     Purpose:  Fills StaInd struct and sends the
 *               StaInd to RRM
 *
 *  @param[in]  RgSchCellCb        *cell  pointer to Cell Control block
 *  @param[in]  RgSchUeCb          *ue  pointer to Ue Control block
 *  @param[in]  U8                 numCqiRept NUmber of reports to be filled
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlFillSndUeStaInd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
RgrUeStaIndInfo      *ueStaInfo
)
#else
PUBLIC S16 rgSCHUtlFillSndUeStaInd(cell, ue, ueStaInfo)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
RgrUeStaIndInfo      *ueStaInfo;
#endif
{

   ueStaInfo->cellId = cell->cellId;
   ueStaInfo->crnti = ue->ueId;

   /* Call utility function (rgSCHUtlRgrUeStaInd) to send rpts to RRM */
   if(rgSCHUtlRgrUeStaInd(cell, ueStaInfo) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"Could not send "
         "UE Sta reports CRNTI:%d",ue->ueId);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

}/* End of rgSCHUtlFillSndStaInd */



/**
 * @brief API for sending STA indication from Scheduler to RRM.
 *
 * @details
 *
 *     Function: rgSCHUtlRgrStaInd
 *
 *     This API is invoked to send STA indication from Scheduler instance to RRM.
 *     This API fills in Pst structure and RgrStaIndInfo
 *     and calls the Sta primitive API towards RRM.
 *
 *  @param[in]  cell                   RgSchCellCb
 *  @param[in]  RgrStsIndInfo          *rgrSta
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrUeStaInd
(
RgSchCellCb          *cell,
RgrUeStaIndInfo      *rgrUeSta
)
#else
PUBLIC S16 rgSCHUtlRgrUeStaInd(cell, rgrUeSta)
RgSchCellCb          *cell;
RgrUeStaIndInfo      *rgrUeSta;
#endif
{
   S16           ret = ROK;
   RgSchUpSapCb  *rgrSap;                    /*!< RGR SAP Control Block */

   TRC2(rgSCHUtlRgrStaInd)


   rgrSap = cell->rgrSap;
   if (rgrSap->sapSta.sapState != LRG_BND)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
               "rgSCHUtlRgrUeStaInd() Upper SAP not bound (%d) ",
               rgrSap->sapSta.sapState);
      RETVALUE(RFAILED);
   }
   RgUiRgrUeStaInd(&(cell->rgrSap->sapCfg.sapPst),
         cell->rgrSap->sapCfg.suId, rgrUeSta);
   RETVALUE(ret);
}  /* rgSCHUtlRgrStaInd*/

/* RRM_RBC_X */
/**
 * @brief function to report DL and UL PRB usage to RRM.
 *
 *
 *     Function: rgSCHUtlUpdAvgPrbUsage
 *               This function sends the PRB usage report to 
 *               RRM with the interval configured by RRM.
 *
 *  @param[in]  cell       *RgSchCellCb
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlUpdAvgPrbUsage
(
RgSchCellCb          *cell
)
#else
PUBLIC S16 rgSCHUtlUpdAvgPrbUsage(cell)
RgSchCellCb       *cell;
#endif
{
   CmLteTimingInfo  frm;
   RgmPrbRprtInd    *prbRprtInd;
   S16              ret = ROK;
   U32              idx;
#ifdef DBG_MAC_RRM_PRB_PRINT
   static U32       count = 0;
   const U32 reprotForEvery20Sec = 20000/cell->prbUsage.rprtPeriod;

   count++;
#endif

   TRC2(rgSCHUtlUpdAvgPrbUsage);

   frm   = cell->crntTime;
   RGSCH_INCR_SUB_FRAME(frm, RG_SCH_CMN_DL_DELTA);

   U16 numDlSf;
   U16 numUlSf;
#ifdef LTE_TDD
  
   if(cell->prbUsage.rprtPeriod >= RGSCH_NUM_SUB_FRAMES)
   {
      /* Get the total number of DL and UL slots within the reporting period*/
      numDlSf = (cell->prbUsage.rprtPeriod * 
            rgSchTddNumDlSubfrmTbl[cell->ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1])
         / RGSCH_NUM_SUB_FRAMES;
      numUlSf = (cell->prbUsage.rprtPeriod * 
            rgSchTddNumUlSubfrmTbl[cell->ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1])
         / RGSCH_NUM_SUB_FRAMES;
   }
   else
   {
      /* Get the total number of DL and UL slots < 10 ms interval */
      numDlSf = rgSchTddNumDlSubfrmTbl[cell->ulDlCfgIdx][frm.slot];
      numUlSf = rgSchTddNumUlSubfrmTbl[cell->ulDlCfgIdx][frm.slot];
   }
#else
     numDlSf = cell->prbUsage.rprtPeriod;
     numUlSf = cell->prbUsage.rprtPeriod;
#endif

   if(SGetSBuf(cell->rgmSap->sapCfg.sapPst.region, 
               cell->rgmSap->sapCfg.sapPst.pool, (Data**)&prbRprtInd, 
               sizeof(RgmPrbRprtInd)) != ROK)
   {
      RETVALUE(RFAILED);
   }

   cmMemset((U8 *) &prbRprtInd->stQciPrbRpts[0],
            0,
            (RGM_MAX_QCI_REPORTS * sizeof(RgmPrbRptPerQci)));

   prbRprtInd->bCellId            = cell->cellId;

   if(numDlSf > 0)
   {
      prbRprtInd->bPrbUsageMask |= RGM_PRB_USAGE_DL; 
      for (idx = 0; idx < RGM_MAX_QCI_REPORTS; idx++ )
      {
         prbRprtInd->stQciPrbRpts[idx].bAvgPrbDlUsage  = 
           RGSCH_DIV_ROUND((cell->prbUsage.qciPrbRpts[idx].dlTotPrbUsed*100),
                           (numDlSf * cell->bwCfg.dlTotalBw));
         prbRprtInd->stQciPrbRpts[idx].bQci = cell->prbUsage.qciPrbRpts[idx].qci;
         cell->prbUsage.qciPrbRpts[idx].dlTotPrbUsed = 0;
      }
   }

   if(numUlSf > 0)
   {
      prbRprtInd->bPrbUsageMask |= RGM_PRB_USAGE_UL; 
      for (idx = 0; idx < RGM_MAX_QCI_REPORTS; idx++ )
      {
         prbRprtInd->stQciPrbRpts[idx].bAvgPrbUlUsage  = 
           RGSCH_DIV_ROUND((cell->prbUsage.qciPrbRpts[idx].ulTotPrbUsed*100),
                           (numUlSf * cell->ulAvailBw));
         prbRprtInd->stQciPrbRpts[idx].bQci = cell->prbUsage.qciPrbRpts[idx].qci;
         cell->prbUsage.qciPrbRpts[idx].ulTotPrbUsed = 0;
      }
   }

#ifdef DBG_MAC_RRM_PRB_PRINT
   if((count % reprotForEvery20Sec) == 0 )
   {
      printf("\n====================================================================");
      printf("\nMAC:  QCI-1[DL:UL]  | QCI-2[DL:UL]  | QCI-3[DL:UL]  | QCI-4[DL:UL] \n");
      printf("======================================================================\n");
      printf(" [%d: %d]\t | [%d: %d]\t | [%d: %d]\t| [%d: %d]\t\n", 
                 prbRprtInd->stQciPrbRpts[0].bAvgPrbDlUsage,
                 prbRprtInd->stQciPrbRpts[0].bAvgPrbUlUsage,
                 prbRprtInd->stQciPrbRpts[1].bAvgPrbDlUsage,
                 prbRprtInd->stQciPrbRpts[1].bAvgPrbUlUsage,
                 prbRprtInd->stQciPrbRpts[2].bAvgPrbDlUsage,
                 prbRprtInd->stQciPrbRpts[2].bAvgPrbUlUsage,
                 prbRprtInd->stQciPrbRpts[3].bAvgPrbDlUsage,
                 prbRprtInd->stQciPrbRpts[3].bAvgPrbUlUsage);
   }
#endif
   RgUiRgmSendPrbRprtInd(&(cell->rgmSap->sapCfg.sapPst), 
                    cell->rgmSap->sapCfg.suId, prbRprtInd);


   RETVALUE(ret);
}
/* RRM_RBC_Y */

/**
 * @brief This function resends the Ta in case of 
 *        max retx failure or DTX for the Ta transmitted 
 *
 * @details
 *
 *     Function: rgSCHUtlReTxTa
 *     Purpose:  
 *                
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchUeCb             *ue
 *  @return  Void 
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlReTxTa 
(
RgSchCellCb       *cellCb,
RgSchUeCb         *ueCb
)
#else
PUBLIC Void rgSCHUtlReTxTa(cellCb, ueCb)
RgSchCellCb       *cellCb;
RgSchUeCb         *ueCb;
#endif
{
   TRC2(rgSCHUtlReTxTa)

   /* If TA Timer is running. Stop it */
   if (ueCb->taTmr.tmrEvnt != TMR_NONE)
   {
      rgSCHTmrStopTmr(cellCb, ueCb->taTmr.tmrEvnt, ueCb);
   }
   /*[ccpu00121813]-ADD-If maxretx is reached then 
    * use outstanding TA val for scheduling again */
   if(ueCb->dl.taCb.outStndngTa == TRUE)
   {
      ueCb->dl.taCb.ta = ueCb->dl.taCb.outStndngTaval;
      ueCb->dl.taCb.outStndngTaval = RGSCH_NO_TA_RQD;
      ueCb->dl.taCb.outStndngTa = FALSE;

   }
   /* Fix : syed TA state updation missing */
   ueCb->dl.taCb.state = RGSCH_TA_TOBE_SCHEDULED;
   rgSCHUtlDlTARpt(cellCb, ueCb); 

   RETVOID;
}

/* Added function for dropping Paging Message*/
/**
 * @brief Handler for BO Updt received for BCCH or PCCH.
 *
 * @details
 *
 *     Function : rgSCHChkBoUpdate
 *
 *     This function shall check for BO received falls within the scheduling window or not
 *
 *
 *  @param[in]  RgSchCellCb    *cell
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHChkBoUpdate
(
RgSchCellCb    *cell,
RgInfCmnBoRpt  *boUpdt
)
#else
PRIVATE S16 rgSCHChkBoUpdate (cell, boUpdt)
RgSchCellCb    *cell;
RgInfCmnBoRpt  *boUpdt;
#endif
{

   U32 crntTimeInSubFrms = 0;
   U32 boUpdTimeInSubFrms = 0;
   U32 distance = 0;
   TRC2(rgSCHChkBoUpdate);

   crntTimeInSubFrms = (cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G) +  cell->crntTime.slot +
	   RG_SCH_CMN_DL_DELTA + 2;  /* As bo received will scheduled in next TTI
					so incrementing with +1 more */
   boUpdTimeInSubFrms = (boUpdt->u.timeToTx.sfn * RGSCH_NUM_SUB_FRAMES_5G)+ boUpdt->u.timeToTx.slot;


   distance = boUpdTimeInSubFrms > crntTimeInSubFrms ? \
	      boUpdTimeInSubFrms - crntTimeInSubFrms : \
	      (RGSCH_MAX_SUBFRM_5G - crntTimeInSubFrms + boUpdTimeInSubFrms);

   if (distance > RGSCH_PCCHBCCH_WIN)
   {
	   RETVALUE(RFAILED);
   }
   RETVALUE(ROK);

}/*rgSCHChkBoUpdate*/


#ifdef LTE_TDD
/**
 * @brief  Utility function to calculate the UL reTxIdx in TDD cfg0
 *
 * @details
 *
 *     Function : rgSchUtlCfg0ReTxIdx
 *
 *     Update the reTxIdx according to the rules mentioned
 *     in 3GPP TS 36.213 section 8 for TDD Cfg0
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  CmLteTimingInfo  phichTime 
 *  @param[in]  U8               hqFdbkIdx
 *  @return     U8
 **/
#ifdef ANSI
PUBLIC U8 rgSchUtlCfg0ReTxIdx
(
RgSchCellCb    *cell,
CmLteTimingInfo phichTime,
U8              hqFdbkIdx
)
#else
PUBLIC U8 rgSchUtlCfg0ReTxIdx (cell, phichTime, hqFdbkIdx)
RgSchCellCb    *cell;
CmLteTimingInfo phichTime;
U8              hqFdbkIdx;
#endif
{
   U8 reTxIdx = RGSCH_INVALID_INFO;
   U8 iPhich = 0; 
   RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchUlSf       *ulSf;   
   U8               ulSF; /* UL SF in the TDD frame */
         
   TRC2(rgSchUtlCfg0ReTxIdx);

   ulSf = &cellUl->ulSfArr[hqFdbkIdx];
   ulSF = ulSf->ulSfIdx;
    
   /* Check for the UL SF 4 or 9 */
   if(ulSF == 9 || ulSF == 4)
   {
     iPhich = 1;  
   }
   if(phichTime.slot == 0 || phichTime.slot == 5)
   {    
      if(iPhich == 0)
      {
         /* Retx will happen according to the Pusch k table */
         reTxIdx = cellUl->schdIdx;
      }
      if(iPhich == 1)
      {
         /* Retx will happen at n+7 */
         RGSCHCMNADDTOCRNTTIME(phichTime, phichTime, 7);
         /* Fetch the corresponding  UL slot Idx in UL sf array */ 
         reTxIdx = rgSCHCmnGetUlSfIdx(&phichTime, cell);
      }   
   }
   else if(phichTime.slot == 1 || phichTime.slot == 6) 
   { 
      /* Retx will happen at n+7 */
      RGSCHCMNADDTOCRNTTIME(phichTime, phichTime, 7);
      /* Fetch the corresponding  UL slot Idx in UL sf array */ 
      reTxIdx = rgSCHCmnGetUlSfIdx(&phichTime, cell);
   }
   RETVALUE(reTxIdx);
}
#endif

/**
 * @brief  Utility function to calculate total num of PRBs required to
 *         satisfy DL BO for TM1/TM2/TM6/TM7
 *
 * @details
 *
 *     Function : rgSchUtlDlCalc1CwPrb
 *
 *    Calculate PRBs required for UE to satisfy BO in DL
 *   
 *    Note : Total calculated PRBs will be assigned to *prbReqrd
 *    
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue 
 *  @param[in]  U32              bo 
 *  @param[out] U32             *prbReqrd 
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSchUtlDlCalc1CwPrb
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
U32             bo,
U32            *prbReqrd
)
#else
PUBLIC Void rgSchUtlDlCalc1CwPrb(cell, ue, bo, prbReqrd)
RgSchCellCb   *cell;
RgSchUeCb     *ue;
U32            bo;
U32           *prbReqrd;
#endif
{
   RgSchCmnDlCell *dlCell  = RG_SCH_CMN_GET_DL_CELL(cell); 
   RgSchCmnDlUe   *dlUe  = RG_SCH_CMN_GET_DL_UE(ue, cell); 
   U32            eff;
   U32            noRes;
   U8             iTbs;
   U8             cfi = dlCell->currCfi;               

   TRC2(rgSchUtlDlCalc1CwPrb);

   iTbs  = dlUe->mimoInfo.cwInfo[0].iTbs[0];
   eff   = (*(RgSchCmnTbSzEff *)(dlCell->cqiToEffTbl[0][cfi]))[iTbs];

   /* Optimization to convert totalBo (which is in-terms of bytes) to bits 
    * i.e, << 3 and multiply with 1024 i.e, << 10 */
   noRes = ((U64)((bo << 3) << 10)) / (eff);
   /* Get the number of RBs needed for this transmission */
   /* Number of RBs = No of REs / No of REs per RB       */
   *prbReqrd = RGSCH_CEIL(noRes, dlCell->noResPerRb[cfi]);

   RETVOID;
} /* rgSchUtlDlCalc1CwPrb*/

/**
 * @brief  Utility function to calculate total num of PRBs required to
 *         satisfy DL BO(BO sum of all logical channels for that UE or an LC BO)
 *         for TM3/TM4
 *
 * @details
 *
 *     Function : rgSchUtlDlCalc2CwPrb
 *
 *    Calculate PRBs required for UE to satisfy BO in DL
 *   
 *    Note : Total calculated PRBs will be assigned to *prbReqrd
 *    
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue 
 *  @param[in]  U32              bo 
 *  @param[out] U32             *prbReqrd 
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSchUtlDlCalc2CwPrb
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
U32             bo,
U32            *prbReqrd
)
#else
PUBLIC Void rgSchUtlDlCalc2CwPrb(cell, ue, bo, prbReqrd)
RgSchCellCb   *cell;
RgSchUeCb     *ue;
U32            bo;
U32           *prbReqrd;
#endif
{
   RgSchCmnDlCell *dlCell  = RG_SCH_CMN_GET_DL_CELL(cell); 
   RgSchCmnDlUe   *dlUe  = RG_SCH_CMN_GET_DL_UE(ue, cell); 
   U32            eff1, eff2;
   U32            noRes;
   U8             noLyr1, noLyr2;
   U8             iTbs1, iTbs2;
   U8             cfi = dlCell->currCfi;               

   TRC2(rgSchUtlDlCalc2CwPrb);

   if ((dlUe->mimoInfo.forceTD) ||/* Transmit Diversity (TD) */
       (dlUe->mimoInfo.ri < 2))/* 1 layer precoding */
   {
      iTbs1  = dlUe->mimoInfo.cwInfo[0].iTbs[0];
      eff1   = (*(RgSchCmnTbSzEff *)(dlCell->cqiToEffTbl[0][cfi]))[iTbs1];

      /* Optimization to convert totalBo (which is in-terms of bytes) to bits 
       * i.e, << 3 and multiply with 1024 i.e, << 10 */
      noRes = ((U64)((bo << 3) << 10)) / (eff1);
      /* Get the number of RBs needed for this transmission */
      /* Number of RBs = No of REs / No of REs per RB       */
      *prbReqrd = RGSCH_CEIL(noRes, dlCell->noResPerRb[cfi]);
   }
   else
   {
      noLyr1 = dlUe->mimoInfo.cwInfo[0].noLyr;
      noLyr2 = dlUe->mimoInfo.cwInfo[1].noLyr;
      iTbs1  = dlUe->mimoInfo.cwInfo[0].iTbs[noLyr1 - 1];
      iTbs2  = dlUe->mimoInfo.cwInfo[1].iTbs[noLyr2 - 1];
      eff1 = (*(RgSchCmnTbSzEff *)(dlCell->cqiToEffTbl[noLyr1 - 1][cfi]))[iTbs1];
      eff2 = (*(RgSchCmnTbSzEff *)(dlCell->cqiToEffTbl[noLyr2 - 1][cfi]))[iTbs2];

      /* Optimization to convert totalBo (which is in-terms of bytes) to bits 
       * i.e, << 3 and multiply with 1024 i.e, << 10 */
      noRes = ((U64)((bo << 3) << 10)) / (eff1 + eff2);
      /* Get the number of RBs needed for this transmission */
      /* Number of RBs = No of REs / No of REs per RB       */
      *prbReqrd = RGSCH_CEIL(noRes, dlCell->noResPerRb[cfi]);
   }
   RETVOID;
} /* rgSchUtlDlCalc2CwPrb */

/**
 * @brief  Utility function to calculate total num of PRBs required to
 *         satisfy DL BO(BO sum of all logical channels for that UE or an LC BO)
 *
 * @details
 *
 *     Function : rgSchUtlCalcTotalPrbReq
 *
 *    This function calls TM specific routine to calculate PRB
 *   
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue 
 *  @param[in]  U32              bo 
 *  @param[out] U32             *prbReqrd 
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSchUtlCalcTotalPrbReq
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
U32             bo,
U32            *prbReqrd
)
#else
PUBLIC Void rgSchUtlCalcTotalPrbReq(cell, ue, bo, prbReqrd)
RgSchCellCb   *cell;
RgSchUeCb     *ue;
U32            bo;
U32           *prbReqrd;
#endif
{
   TRC2(rgSchUtlCalcTotalPrbReq);

   /* Call TM specific Prb calculation routine */
   (dlCalcPrbFunc[ue->mimoInfo.txMode - 1])(cell, ue, bo, prbReqrd);

   RETVOID;
} /* rgSchUtlCalcTotalPrbReq */
#ifdef UNUSE_FUN
#ifdef TFU_UPGRADE
/***********************************************************
 *
 *     Func : rgSCHUtlFetchPcqiBitSz
 *
 *
 *     Desc : Fetch the CQI/PMI bits for a UE based on the mode, periodicity.
 *
 *     Ret  : U8
 *            ROK - Success
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U8 rgSCHUtlFetchPcqiBitSz
(
 RgSchCellCb    *cell,
 RgSchUeCb    *ueCb, 
 U8           numTxAnt
 )
#else
PRIVATE U8 rgSCHUtlFetchPcqiBitSz (cell, ueCb, numTxAnt)
RgSchCellCb   *cell;
RgSchUeCb     *ueCb;
U8            numTxAnt;
#endif
{
   U8   confRepMode;
   U8   pcqiSz;
   U8   ri;
   RgSchUePCqiCb *cqiCb = RG_SCH_GET_UE_CELL_CQI_CB(ueCb,cell);

   TRC3(rgSCHUtlFetchPcqiBitSz);
   confRepMode = cqiCb->cqiCfg.cqiSetup.prdModeEnum;
   if((ueCb->mimoInfo.txMode != RGR_UE_TM_3) && 
         (ueCb->mimoInfo.txMode != RGR_UE_TM_4))
   {
      ri =1;
   }
   else
   {
      ri = cqiCb->perRiVal;
   }
   switch(confRepMode)
   {
      case RGR_PRD_CQI_MOD10:
      {
         pcqiSz = 4;
      }
      break;

      case RGR_PRD_CQI_MOD11:
      {
         if(numTxAnt == 2)
         {
            if (ri ==1)
            {
               pcqiSz = 6;
            }
            else
            {
               pcqiSz = 8;
            }
         }
         else if(numTxAnt == 4)
         {
            if (ri ==1)
            {
               pcqiSz = 8;
            }
            else
            {
               pcqiSz = 11;
            }
         }
         else
         {
            /* This is number of antenna case 1.
             * This is not applicable for Mode 1-1. 
             * So setting it to invalid value */
            pcqiSz = 0;
         }
      }
      break;

      case RGR_PRD_CQI_MOD20:
      {
         if(cqiCb->isWb)
         {
            pcqiSz = 4;
         }
         else
         {
            pcqiSz = 4 + cqiCb->label;
         }
      }
      break;

      case RGR_PRD_CQI_MOD21:
      {
         if(cqiCb->isWb)
         {
             if(numTxAnt == 2)
             {
                if (ri ==1)
                {
                    pcqiSz = 6;
                }
                else
                {
                    pcqiSz = 8;
                }
             }
             else if(numTxAnt == 4)
             {
                if (ri ==1)
                {
                    pcqiSz = 8;
                }
                else
                {
                    pcqiSz = 11;
                }
             }
             else
             {
                /* This might be number of antenna case 1.
                 * For mode 2-1 wideband case only antenna port 2 or 4 is supported.
                 * So setting invalid value.*/
                pcqiSz = 0;
             }
          }
          else
          {
             if (ri ==1)
             {
                 pcqiSz = 4 + cqiCb->label;
             }
             else
             {
                 pcqiSz = 7 + cqiCb->label;
             }
          }
      }
      break;

      default:
         pcqiSz = 0;
      break;
   }
   
   RETVALUE(pcqiSz);
}
#endif
#endif
/**
 * @brief  Utility function to returns the number of subbands based on the 
 *         requested bytes.
 *
 * @details
 *
 *     Function : rgSchUtlGetNumSbs
 *
 *     Calculate the number of PRBs
 *     Update the subbandRequired based on the nPrbs and subband size
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  U32          *numSbs
 *  @return     U8
 **/
#ifdef ANSI
PUBLIC U8 rgSchUtlGetNumSbs
(
RgSchCellCb                  *cell,
RgSchUeCb                    *ue,
U32                          *numSbs
)
#else
PUBLIC U8 rgSchUtlGetNumSbs (cell, ue, numSbs)
RgSchCellCb                  *cell;
RgSchUeCb                    *ue;
U32                          *numSbs;
#endif
{
   U32                       nPrb;
   //Currently hardcoding MAX prb for each UE
   nPrb = ue->ue5gtfCb.maxPrb;
   (*numSbs) = RGSCH_CEIL(nPrb, MAX_5GTF_VRBG_SIZE);
   RETVALUE(ROK);
}

/**
 * @brief  Utility function to insert the UE node into UE Lst based on the
 *         number of subbands allocated for the UE for the current TTI.
 *
 * @details
 *
 *     Function : rgSchUtlSortInsUeLst
 *
 *     If subbandRequired < Min, then insert at head
 *     Else If subbandRequired > Max, then insert at tail
 *     Else, traverse the list and place the node at the appropriate place
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return     U8
 **/
#ifdef ANSI
PUBLIC U8 rgSchUtlSortInsUeLst
(
RgSchCellCb  *cell,
CmLListCp    *ueLst,
CmLList      *node,
U8           vrbgRequired
)
#else
PUBLIC U8 rgSchUtlSortInsUeLst (cell, ueLst, node, vrbgRequired)
RgSchCellCb  *cell;
CmLListCp    *ueLst;
CmLList      *node;
U8           vrbgRequired;
#endif
{
   CmLList      *ueInLst;
   CmLList      *firstUeInLst;
   CmLList      *lastUeInLst;
   RgSchUeCb    *tempUe;
   RgSchCmnUlUe *ueUl;

   //firstUeInLst = cmLListFirst(ueLst);
   CM_LLIST_FIRST_NODE(ueLst,firstUeInLst);
   if(NULLP == firstUeInLst)
   {
      /* first node to be added to the list */
      cmLListAdd2Tail(ueLst, node);
   }
   else
   {
      /* Sb Required for the UE is less than the first node in the list */
      tempUe      = (RgSchUeCb *)(firstUeInLst->node);
      ueUl = RG_SCH_CMN_GET_UL_UE(tempUe, cell);

      if(vrbgRequired <= ueUl->vrbgRequired)
      {
         cmLListInsCrnt(ueLst, (node));
      }
      else
      {
         /* Sb Required for this UE is higher than the UEs in the list */
         lastUeInLst = cmLListLast(ueLst);
         tempUe      = (RgSchUeCb *)(lastUeInLst->node);
         if(vrbgRequired >= ueUl->vrbgRequired)
         {
            cmLListAdd2Tail(ueLst, (node));
         }
         else
         {
            /* This UE needs to be in the middle. Search and insert the UE */
            ueInLst = cmLListFirst(ueLst);
            do
            {
               tempUe = (RgSchUeCb *)(ueInLst->node);

               if(vrbgRequired <= ueUl->vrbgRequired)
               {
                  cmLListInsCrnt(ueLst, (node));
                  break;
               }

               ueInLst = cmLListNext(ueLst);

            } while(NULLP != ueInLst && ueInLst != firstUeInLst);
         }
      }
   }

   RETVALUE(ROK);
}

/**
 * @brief Function to Send LCG GBR register to MAC
 *
 * @details
 *
 *     Function: rgSCHUtlBuildNSendLcgReg
 *
 *     Handler for sending LCG GBR registration 
 *
 *     Invoked by: 
 *         SCHD
 *
 *     Processing Steps:
 *           
 *  @param[in] RgSchCellCb       *cell
 *  @param[in] CmLteRnti         crnti
 *  @param[in] U8                lcgId
 *  @param[in] Bool              isGbr
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlBuildNSendLcgReg
(
RgSchCellCb    *cell,
CmLteRnti      crnti,
U8             lcgId,
Bool           isGbr
)
#else
PUBLIC S16 rgSCHUtlBuildNSendLcgReg(cell, crnti, lcgId, isGbr)
RgSchCellCb    *cell;
CmLteRnti      crnti;
U8             lcgId;
Bool           isGbr;
#endif
{
   Pst            pst;
   RgInfLcgRegReq lcgRegReq;

   TRC3(rgSCHUtlBuildNSendLcgReg);

   cmMemset((U8*)&pst, (U8)0, sizeof(Pst));
   lcgRegReq.isGbr  = isGbr;
   lcgRegReq.cellId = cell->cellId;
   lcgRegReq.crnti  = crnti;
   lcgRegReq.lcgId  = lcgId;
   rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], cell->macInst);
   /* code Coverage portion of the test case */ 
   RgSchMacLcgReg(&pst, &lcgRegReq);

   RETVALUE(ROK);
}

#ifdef TFU_UPGRADE
#ifdef LTE_ADV
#ifndef TFU_TDD
/**
 * @brief Function to map RGR pucch type to TFU type
 *
 * @details
 *
 *     Function: rgSchUtlGetFdbkMode
 *
 *
 *     Invoked by: 
 *         SCHD
 *
 *     Processing Steps:
 *           
 *  @param[in] RgrSchFrmt1b3TypEnum
 *  @return  TfuAckNackMode
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC TfuAckNackMode rgSchUtlGetFdbkMode
(
RgrSchFrmt1b3TypEnum fdbkType
)
#else
PUBLIC TfuAckNackMode rgSchUtlGetFdbkMode(fdbkType)
RgrSchFrmt1b3TypEnum fdbkType;
#endif
{

   TfuAckNackMode mode = TFU_UCI_FORMAT_1A_1B;

   TRC2(rgSchUtlGetFdbkMode);

   switch(fdbkType)
   {
     case RG_SCH_UCI_FORMAT_NON_CA:
     case RG_SCH_UCI_FORMAT1A_1B:
     {
        mode = TFU_UCI_FORMAT_1A_1B;
     }
     break;
     case RG_SCH_UCI_FORMAT1B_CS:
     {
        mode = TFU_UCI_FORMAT_1B_CS;
     }
     break;
     case RG_SCH_UCI_FORMAT3:
     {
        mode = TFU_UCI_FORMAT_3;
     }
     break;
   }
   RETVALUE(mode);
}
#endif /* TFU_TDD */
#endif /* LTE_ADV */
#endif /*TFU_UPGRADE */

#ifdef LTE_ADV
/**
 * @brief Send Ue SCell delete to SMAC.
 *
 * @details
 *
 *     Function : rgSCHUtlSndUeSCellDel2Mac 
 *       This function populates the struct RgInfRlsRnti and
 *       get the pst for SMac and mark field isUeSCellDel to TRUE which 
 *       indicates that it is a Ue SCell delete.
 *     
 *    
 *           
 *  @param[in]     RgSchCellCb    *cell
 *  @param[in]     CmLteRnti      rnti 
 *  @return  Void
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlSndUeSCellDel2Mac 
(
RgSchCellCb    *cell,
CmLteRnti      rnti
)
#else
PUBLIC Void rgSCHUtlSndUeSCellDel2Mac(cell, rnti)
RgSchCellCb    *cell;
CmLteRnti      rnti;
#endif
{
   Pst          pst;
   Inst         inst = cell->instIdx;
   RgInfRlsRnti rntiInfo;

   TRC2(rgSCHUtlSndUeSCellDel2Mac)

   RGSCHDBGINFONEW(inst,(rgSchPBuf(inst),"RNTI Release IND for UE(%d)\n", rnti));
   /* Copy the info to rntiInfo */
   rntiInfo.cellId = cell->cellId;
   rntiInfo.rnti   = rnti;
   /* Fix : syed ueId change as part of reestablishment.
    * Now SCH to trigger this. CRG ueRecfg for ueId change 
    * is dummy */	   
   rntiInfo.ueIdChng = FALSE;
   rntiInfo.newRnti  = rnti;
   rntiInfo.isUeSCellDel = TRUE;
   /* Invoke MAC to release the rnti */
   rgSCHUtlGetPstToLyr(&pst, &rgSchCb[inst], cell->macInst);
   RgSchMacRlsRnti(&pst, &rntiInfo);
   RETVOID;
}

/**
 * @brief Returns max TB supported by a given txMode
 *
 * @details
 *
 *     Function : rgSCHUtlGetMaxTbSupp
 *     Max TB supported for TM Modes (1,2,5,6 and 7) is 1
 *     and 2 for others
 *    
 *           
 *  @param[in]     RgrTxMode  txMode
 *  @return        U8 maxTbCount; 
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC U8 rgSCHUtlGetMaxTbSupp
(
RgrTxMode txMode
)
#else
PUBLIC U8 rgSCHUtlGetMaxTbSupp(txMode)
RgrTxMode txMode
#endif
{
   U8 maxTbCount;

   TRC2(rgSCHUtlGetMaxTbSupp);

   /* Primary Cell */

   switch(txMode)
   {
      case RGR_UE_TM_1:
      case RGR_UE_TM_2:
      case RGR_UE_TM_5:
      case RGR_UE_TM_6:
      case RGR_UE_TM_7:
         maxTbCount = 1;
         break;
      case RGR_UE_TM_3:
      case RGR_UE_TM_4:
      case RGR_UE_TM_8:
      case RGR_UE_TM_9:
         maxTbCount = 2;
         break;
      default:
         maxTbCount = 0;
         break;
   }

   RETVALUE(maxTbCount);
}

/**
 * @brief Send Ue SCell delete to SMAC.
 *
 * @details
 *
 *     Function : rgSCHTomUtlGetTrigSet 
 *      This function gets the triggerset based on cqiReq 
 *     
 *  @param[in]     RgSchCellCb    *cell
 *  @param[in]     RgSchUeCb      ueCb 
 *  @param[in]     U8             cqiReq,
 *  @param[out]    U8             *triggerSet
 *
 *  @return  Void
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC Void rgSCHTomUtlGetTrigSet
(
   RgSchCellCb       *cell,
   RgSchUeCb	     *ueCb,
   U8                cqiReq,
   U8                *triggerSet
 )
#else
PRIVATE S16 rgSCHTomUtlGetTrigSet(cell, ueCb, cqiReq, triggerSet)
   RgSchCellCb       *cell;
   RgSchUeCb	     *ueCb;
   U8                cqiReq;
   U8                *triggerSet;
#endif
{
   RgSchUeCellInfo *pCellInfo = RG_SCH_CMN_GET_PCELL_INFO(ueCb);
   switch(cqiReq)
   {
      case RG_SCH_APCQI_SERVING_CC:
         {
            /* APeriodic CQI request for Current Carrier.*/
            U8 sCellIdx = ueCb->cellIdToCellIdxMap[RG_SCH_CELLINDEX(cell)];
            *triggerSet = 1 << (7 - sCellIdx);
            break;
         }
      case RG_SCH_APCQI_1ST_SERVING_CCS_SET:
         {
            *triggerSet = pCellInfo->acqiCb.aCqiCfg.triggerSet1;
            break;
         }
      case RG_SCH_APCQI_2ND_SERVING_CCS_SET:
         {
            *triggerSet = pCellInfo->acqiCb.aCqiCfg.triggerSet2;
            break;
         }
      default:
         {
            /* BUG */
            break;
         }
   }
   RETVOID;
}
#endif
/**
 * @brief This function updates the value of UE specific DCI sizes
 *
 * @details
 *
 *     Function: rgSCHUtlUpdUeDciSize
 *     Purpose:  This function calculates and updates DCI Sizes in bits.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ueCb
 *  @param[in]  isCsi2Bit         *isCsi2Bit: is 1 bit or 2 bit CSI
 *  @return     Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlUpdUeDciSize
(
RgSchCellCb      *cell,
RgSchUeCb        *ueCb,
Bool             isCsi2Bit
)
#else
PUBLIC Void rgSCHUtlUpdUeDciSize(cell, ueCb, isCsi2Bit)
RgSchCellCb      *cell;
RgSchUeCb        *ueCb;
Bool             isCsi2Bit;
#endif
{
   U8 dci01aCmnSize = cell->dciSize.baseSize[TFU_DCI_FORMAT_0];
   U8 dci01aDedSize = cell->dciSize.baseSize[TFU_DCI_FORMAT_0];
   if ((ueCb->accessStratumRls >= RGR_REL_10) && (cell->bwCfg.dlTotalBw >= cell->bwCfg.ulTotalBw))
   {
      dci01aCmnSize += 1; /* Resource Allocation Type DCI 0 */
      dci01aDedSize += 1; /* Resource Allocation Type DCI 0 */
   }
   if (isCsi2Bit == TRUE)
   {
      dci01aDedSize += 2; /* 2 bit CSI DCI 0 */
   }
   else
   {
      dci01aDedSize += 1; /* 1 bit CSI DCI 0 */
   }
   
   /* Common CSI is always 1 bit DCI 0 */
   dci01aCmnSize += 1; /* 1 bit CSI DCI 0 */

   /* Compare the sizes of DCI 0 with DCI 1A and consider the greater */
   if (dci01aCmnSize < cell->dciSize.baseSize[TFU_DCI_FORMAT_1A])
   {
      dci01aCmnSize = cell->dciSize.baseSize[TFU_DCI_FORMAT_1A];
   }
   if (dci01aDedSize < cell->dciSize.baseSize[TFU_DCI_FORMAT_1A])
   {
      dci01aDedSize = cell->dciSize.baseSize[TFU_DCI_FORMAT_1A];
   }

   /* Remove the Ambiguous Sizes as mentioned in table Table 5.3.3.1.2-1 Spec 36.212-a80 Sec 5.3.3.1.3 */
   dci01aCmnSize += rgSchDciAmbigSizeTbl[dci01aCmnSize];
   dci01aDedSize += rgSchDciAmbigSizeTbl[dci01aDedSize];

   ueCb->dciSize.cmnSize[TFU_DCI_FORMAT_0]  = dci01aCmnSize;
   ueCb->dciSize.cmnSize[TFU_DCI_FORMAT_1A] = dci01aCmnSize;
   
   ueCb->dciSize.dedSize[TFU_DCI_FORMAT_0]  = dci01aDedSize;
   ueCb->dciSize.dedSize[TFU_DCI_FORMAT_1A] = dci01aDedSize;

   ueCb->dciSize.dedSize[TFU_DCI_FORMAT_1] = cell->dciSize.baseSize[TFU_DCI_FORMAT_1];
   do {
      /* Spec 36.212-a80 Sec 5.3.3.1.2: If the UE is configured to decode PDCCH with CRC scrambled 
       * by the C-RNTI and the number of information bits in format 1 is equal to that for format 0/1A 
       * for scheduling the same serving cell and mapped onto the UE specific search space given by the 
       * C-RNTI as defined in [3], one bit of value zero shall be appended to format 1. */
      if (ueCb->dciSize.dedSize[TFU_DCI_FORMAT_1] == ueCb->dciSize.dedSize[TFU_DCI_FORMAT_1A])
      {
         ueCb->dciSize.dedSize[TFU_DCI_FORMAT_1] += 1;
      }

      /* Spec 36.212-a80 Sec 5.3.3.1.2: If the number of information bits in format 1 belongs 
       * to one of the sizes in Table 5.3.3.1.2-1, one or more zero bit(s) shall be appended 
       * to format 1 until the payload size of format 1 does not belong to one of the sizes in 
       * Table 5.3.3.1.2-1 and is not equal to that of format 0/1A mapped onto the same search space. */
      ueCb->dciSize.dedSize[TFU_DCI_FORMAT_1] += rgSchDciAmbigSizeTbl[ueCb->dciSize.dedSize[TFU_DCI_FORMAT_1]];
   } while (ueCb->dciSize.dedSize[TFU_DCI_FORMAT_1] == ueCb->dciSize.dedSize[TFU_DCI_FORMAT_1A]);

   /* Just copying the value of 2/2A to avoid multiple checks at PDCCH allocations. This values never change.*/
   ueCb->dciSize.dedSize[TFU_DCI_FORMAT_2]  = cell->dciSize.size[TFU_DCI_FORMAT_2];
   ueCb->dciSize.dedSize[TFU_DCI_FORMAT_2A] = cell->dciSize.size[TFU_DCI_FORMAT_2A];
   ueCb->dciSize.noUlCcSize[TFU_DCI_FORMAT_2]  = cell->dciSize.size[TFU_DCI_FORMAT_2];
   ueCb->dciSize.noUlCcSize[TFU_DCI_FORMAT_2A] = cell->dciSize.size[TFU_DCI_FORMAT_2A];

   /* Spec 36.212-a80 Sec 5.3.3.1.3: except when format 1A assigns downlink resource 
    * on a secondary cell without an uplink configuration associated with the secondary cell */
   ueCb->dciSize.noUlCcSize[TFU_DCI_FORMAT_1A] = cell->dciSize.baseSize[TFU_DCI_FORMAT_1A];
   ueCb->dciSize.noUlCcSize[TFU_DCI_FORMAT_1A] += rgSchDciAmbigSizeTbl[ueCb->dciSize.noUlCcSize[TFU_DCI_FORMAT_1A]];
   ueCb->dciSize.noUlCcSize[TFU_DCI_FORMAT_1] = cell->dciSize.baseSize[TFU_DCI_FORMAT_1];
   do {
      /* Spec 36.212-a80 Sec 5.3.3.1.2: If the UE is configured to decode PDCCH with CRC scrambled 
       * by the C-RNTI and the number of information bits in format 1 is equal to that for format 0/1A 
       * for scheduling the same serving cell and mapped onto the UE specific search space given by the 
       * C-RNTI as defined in [3], one bit of value zero shall be appended to format 1. */
      if (ueCb->dciSize.noUlCcSize[TFU_DCI_FORMAT_1] == ueCb->dciSize.noUlCcSize[TFU_DCI_FORMAT_1A])
      {
         ueCb->dciSize.noUlCcSize[TFU_DCI_FORMAT_1] += 1;
      }

      /* Spec 36.212-a80 Sec 5.3.3.1.2: If the number of information bits in format 1 belongs 
       * to one of the sizes in Table 5.3.3.1.2-1, one or more zero bit(s) shall be appended 
       * to format 1 until the payload size of format 1 does not belong to one of the sizes in 
       * Table 5.3.3.1.2-1 and is not equal to that of format 0/1A mapped onto the same search space. */
      ueCb->dciSize.noUlCcSize[TFU_DCI_FORMAT_1] += rgSchDciAmbigSizeTbl[ueCb->dciSize.noUlCcSize[TFU_DCI_FORMAT_1]];
   } while (ueCb->dciSize.noUlCcSize[TFU_DCI_FORMAT_1] == ueCb->dciSize.noUlCcSize[TFU_DCI_FORMAT_1A]);
#ifdef EMTC_ENABLE
   rgSCHEmtcUtlUpdUeDciSize(cell, ueCb); 
#endif      
}

/**
 * @brief This function initialises the DCI Size table
 *
 * @details
 *
 *     Function: rgSCHUtlCalcDciSizes
 *     Purpose:  This function calculates and initialises DCI Sizes in bits.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @return     Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlCalcDciSizes
(
RgSchCellCb       *cell
)
#else
PUBLIC Void rgSCHUtlCalcDciSizes(cell)
RgSchCellCb       *cell;
#endif
{
   U8 dciSize = 0;
   U8 dci01aSize = 0;
   U32 bits = 0, idx = 0;

   switch(TFU_DCI_FORMAT_0) /* Switch case for the purpose of readability */
   {
      case TFU_DCI_FORMAT_0:
         {
            /* DCI 0: Spec 36.212 Section 5.3.3.1.1 */
            dciSize = 0;
            /*-- Calculate resource block assignment bits need to be set
              Which is ln(N(N+1)/2) 36.212 5.3.3.1 --*/
            bits = (cell->bwCfg.ulTotalBw * (cell->bwCfg.ulTotalBw + 1) / 2);
            while ((bits & 0x8000) == 0)
            {
               bits <<= 1;
               idx++;
            }
            bits = 16 - idx;

            dciSize = 1 /* DCI 0 bit indicator */ + \
                      1 /* Frequency hoping enable bit field */ + \
                      (U8)bits /* For frequency Hopping */ + \
                      5 /* MCS */ + \
                      1 /* NDI */ + \
                      2 /* TPC */ + \
                      3 /* DMRS */
#ifdef TFU_TDD
                      + \
                      2 /* UL Index Config 0 or DAI Config 1-6 */
#endif
                      ;

            cell->dciSize.baseSize[TFU_DCI_FORMAT_0] = dciSize;

            /* If hoping flag is enabled */
            if (cell->bwCfg.ulTotalBw <= 49) /* Spec 36.213 Table 8.4-1, N UL_hop, if hopping is enabled */
            {
               cell->dciSize.dci0HopSize = 1;
            }
            else
            {
               cell->dciSize.dci0HopSize = 2;
            }

            /* Update common non-CRNTI scrambled DCI 0/1A flag */
            dci01aSize = cell->dciSize.baseSize[TFU_DCI_FORMAT_0] + 1; /* 1 bit CSI */
         }
      case TFU_DCI_FORMAT_1A:
         {
            /* DCI 1A: Spec 36.212 Section 5.3.3.1.3 */
            dciSize = 0;
            idx = 0;
            /* Calculate resource block assignment bits need to be set
              Which is ln(N(N+1)/2) */
            bits = (cell->bwCfg.dlTotalBw * (cell->bwCfg.dlTotalBw + 1) / 2);
            while ((bits & 0x8000) == 0)
            {
               bits <<= 1;
               idx++;
            }
            bits = 16 - idx;

            dciSize += 1 /* Format 1A */ + \
                       1 /* Local or Distributed */ + \
                       (U8)bits /* Resource block Assignment */ + \
                       5 /* MCS */ + 
#ifdef TFU_TDD
                       4 /* HARQ Proc Id */ + 
#else
                       3 /* HARQ Proc Id */ + 
#endif
                       1 /* NDI */ + \
                       2 /* RV */ + \
                       2 /* TPC CMD */
#ifdef TFU_TDD
                       + \
                       2 /* DAI */
#endif
                       ;
            cell->dciSize.baseSize[TFU_DCI_FORMAT_1A] = dciSize;

            /* If the UE is not configured to decode PDCCH with CRC scrambled by the C-RNTI, 
             * and the number of information bits in format 1A is less than that of format 0, 
             * zeros shall be appended to format 1A until the payload size equals that of format 0. */
            /* Compare the size with DCI 1A  and DCI 0 and consider the greater one */
            if (dci01aSize < cell->dciSize.baseSize[TFU_DCI_FORMAT_1A])
            {
               dci01aSize = cell->dciSize.baseSize[TFU_DCI_FORMAT_1A];
            }
            /* If the number of information bits in format 1A belongs to one of the sizes in 
             * Table 5.3.3.1.2-1, one zero bit shall be appended to format 1A. */
            dci01aSize += rgSchDciAmbigSizeTbl[dci01aSize];
            cell->dciSize.size[TFU_DCI_FORMAT_1A] = cell->dciSize.size[TFU_DCI_FORMAT_0] = dci01aSize;
         }
      case TFU_DCI_FORMAT_1:
         {
            /* DCI 1: Spec 36.212 Section 5.3.3.1.2 */
            dciSize = 0;
            if (cell->bwCfg.dlTotalBw > 10)
            {
               dciSize = 1; /* Resource Allocation header bit */
            }
            
            /* Resouce allocation bits Type 0 and Type 1 */
            bits = (cell->bwCfg.dlTotalBw/cell->rbgSize);
            if ((cell->bwCfg.dlTotalBw % cell->rbgSize) != 0)
            {
               bits++;
            }

            dciSize += (U8)bits /* Resource Allocation bits */ + \
                       5 /* MCS */ + 
#ifdef TFU_TDD
                       4 /* HARQ TDD */ + 
#else
                       3 /* HARQ FDD */ + 
#endif
                       1 /* NDI */ + \
                       2 /* Redunancy Version */ + \
                       2 /* TPC Cmd */
#ifdef TFU_TDD
                       + \
                       2 /* DAI */
#endif
                       ;

            
            cell->dciSize.baseSize[TFU_DCI_FORMAT_1] = dciSize;

            cell->dciSize.size[TFU_DCI_FORMAT_1] = dciSize;
            
            do {
               /* If the UE is not configured to decode PDCCH with CRC 
                * scrambled by the C-RNTI and the number of information bits in format 1 
                * is equal to that for format 0/1A, one bit of value zero shall be appended 
                * to format 1. */
               if (dci01aSize == cell->dciSize.size[TFU_DCI_FORMAT_1])
               {
                  cell->dciSize.size[TFU_DCI_FORMAT_1] += 1;
               }

               /* If the number of information bits in format 1 belongs to one of the sizes in 
                * Table 5.3.3.1.2-1, one or more zero bit(s) shall be appended to format 1 until 
                * the payload size of format 1 does not belong to one of the sizes in Table 5.3.3.1.2-1 
                * and is not equal to that of format 0/1A mapped onto the same search space. */
               cell->dciSize.size[TFU_DCI_FORMAT_1] += rgSchDciAmbigSizeTbl[cell->dciSize.size[TFU_DCI_FORMAT_1]];
            } while (cell->dciSize.size[TFU_DCI_FORMAT_1] == dci01aSize);
         }
      case TFU_DCI_FORMAT_2:
         {
            /* DCI 2: Spec 36.212 Section 5.3.3.1.5 */
            dciSize = 0;
            if (cell->bwCfg.dlTotalBw > 10)
            {
               dciSize = 1; /* Resource Allocation bit */
            }

            dciSize += (U8)bits /* Resource Allocation bits */ + \
                       2 /* TPC */ + 
#ifdef TFU_TDD
                       2 /* DAI */ + \
                       4 /* HARQ */ + 
#else
                       3 /* HARQ */ +
#endif
                       1 /* CW Swap Flag */ + \
                       5 /* MCS for TB1 */+ \
                       1 /* NDI for TB1 */+ \
                       2 /* RV for TB1 */ + \
                       5 /* MCS for TB2 */+ \
                       1 /* NDI for TB2 */+ \
                       2 /* RV for TB2 */;
            if (cell->numTxAntPorts == 2)
            {
               dciSize += 3;
            }
            else if (cell->numTxAntPorts == 4)
            {
               dciSize += 6;
            }
            cell->dciSize.size[TFU_DCI_FORMAT_2] = dciSize;
            cell->dciSize.size[TFU_DCI_FORMAT_2] += rgSchDciAmbigSizeTbl[cell->dciSize.size[TFU_DCI_FORMAT_2]];
         }
      case TFU_DCI_FORMAT_2A:
         {
            /* DCI 2A: Spec 36.212 Section 5.3.3.1.5A */
            dciSize = 0;
            if (cell->bwCfg.dlTotalBw > 10)
            {
               dciSize = 1; /* Resource Allocation bit */
            }

            dciSize += (U8)bits /* Resource Allocation bits */ + \
                         2 /* TPC */ + 
#ifdef TFU_TDD
                         2 /* DAI */ + \
                         4 /* HARQ */ + 
#else
                         3 /* HARQ */ +
#endif
                         1 /* CW Swap Flag */ + \
                         5 /* MCS for TB1 */+ \
                         1 /* NDI for TB1 */+ \
                         2 /* RV for TB1 */ + \
                         5 /* MCS for TB2 */+ \
                         1 /* NDI for TB2 */+ \
                         2 /* RV for TB2 */;
            if (cell->numTxAntPorts == 4)
            {
               dciSize += 2;
            }
            cell->dciSize.size[TFU_DCI_FORMAT_2A] = dciSize;
            cell->dciSize.size[TFU_DCI_FORMAT_2A] += \
                          rgSchDciAmbigSizeTbl[cell->dciSize.size[TFU_DCI_FORMAT_2A]]; /* Spec 39.212 Table 5.3.3.1.2-1 */
         }
      case TFU_DCI_FORMAT_3:
         {
            /* DCI 3: Spec 36.212 Section 5.3.3.1.6 */
            cell->dciSize.size[TFU_DCI_FORMAT_3] = cell->dciSize.size[TFU_DCI_FORMAT_1A] / 2;
            if (cell->dciSize.size[TFU_DCI_FORMAT_3] % 2)
            {
               cell->dciSize.size[TFU_DCI_FORMAT_3]++;
            }
         }
      case TFU_DCI_FORMAT_3A:
         {
            /* DCI 3A: Spec 36.212 Section 5.3.3.1.7 */
            cell->dciSize.size[TFU_DCI_FORMAT_3A] = cell->dciSize.size[TFU_DCI_FORMAT_1A];
         }
#ifdef EMTC_ENABLE
      case TFU_DCI_FORMAT_6_0A:
         {
            rgSCHEmtcGetDciFrmt60ASize(cell);
         }
         case TFU_DCI_FORMAT_6_1A:
         {
            rgSCHEmtcGetDciFrmt61ASize(cell);
         }
#endif                  
      default:
         {
            /* DCI format not supported */
            break;
         }
   }
}

/**
 * @brief Handler for the CPU OvrLd related state adjustment.
 *
 * @details
 *
 *     Function : rgSCHUtlCpuOvrLdAdjItbsCap
 *
 *     Processing Steps:
 *      - Record dl/ulTpts 
 *      - Adjust maxItbs to acheive target throughputs
 *
 *  @param[in]  RgSchCellCb *cell
 *  @return  Void 
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlCpuOvrLdAdjItbsCap
(
 RgSchCellCb *cell
)
#else
PUBLIC Void rgSCHUtlCpuOvrLdAdjItbsCap(cell)
 RgSchCellCb *cell
#endif
{
   U32 tptDelta;

   TRC3(rgSCHUtlCpuOvrLdAdjItbsCap)

   if ((cell->cpuOvrLdCntrl.cpuOvrLdIns) & (RGR_CPU_OVRLD_DL_TPT_UP | 
            RGR_CPU_OVRLD_DL_TPT_DOWN))
   {
      /* Regulate DL Tpt for CPU overload */
      if (cell->measurements.dlTpt > cell->cpuOvrLdCntrl.tgtDlTpt)
      {
         tptDelta = cell->measurements.dlTpt - cell->cpuOvrLdCntrl.tgtDlTpt;
         /* Upto 0.5% drift in measured vs target tpt is ignored */
         if (((tptDelta*1000)/cell->cpuOvrLdCntrl.tgtDlTpt) > 5)
         {
            cell->thresholds.maxDlItbs = RGSCH_MAX((cell->thresholds.maxDlItbs-1), 1);
         }
      }
      else
      {
         tptDelta = cell->cpuOvrLdCntrl.tgtDlTpt - cell->measurements.dlTpt;
         /* Upto 0.5% drift in measured vs target tpt is ignored */
         if (((tptDelta*1000)/cell->cpuOvrLdCntrl.tgtDlTpt) > 5)
         {
            cell->thresholds.maxDlItbs = RGSCH_MIN((cell->thresholds.maxDlItbs+1), RG_SCH_DL_MAX_ITBS);
         }
      }
#ifdef CPU_OL_DBG_PRINTS
      printf("\n DL CPU OL ADJ = %lu, %lu, %d\n", cell->measurements.dlTpt, cell->cpuOvrLdCntrl.tgtDlTpt, 
            cell->thresholds.maxDlItbs);
#endif
   }

   if ((cell->cpuOvrLdCntrl.cpuOvrLdIns) & (RGR_CPU_OVRLD_UL_TPT_UP | 
            RGR_CPU_OVRLD_UL_TPT_DOWN))
   {
      /* Regualte DL Tpt for CPU overload */
      if (cell->measurements.ulTpt > cell->cpuOvrLdCntrl.tgtUlTpt)
      {
         tptDelta = cell->measurements.ulTpt - cell->cpuOvrLdCntrl.tgtUlTpt;
         /* Upto 1% drift in measured vs target tpt is ignored */
         if (((tptDelta*1000)/cell->cpuOvrLdCntrl.tgtUlTpt) > 10)
         {
            cell->thresholds.maxUlItbs = RGSCH_MAX((cell->thresholds.maxUlItbs-1), 1);
         }
      }
      else
      {
         tptDelta = cell->cpuOvrLdCntrl.tgtUlTpt - cell->measurements.ulTpt;
         /* Upto 1% drift in measured vs target tpt is ignored */
         if (((tptDelta*1000)/cell->cpuOvrLdCntrl.tgtUlTpt) > 10)
         {
            cell->thresholds.maxUlItbs = RGSCH_MIN((cell->thresholds.maxUlItbs+1), RG_SCH_UL_MAX_ITBS);
         }
      }
#ifdef CPU_OL_DBG_PRINTS
      printf("\n UL CPU OL ADJ = %lu, %lu, %d\n", cell->measurements.ulTpt, cell->cpuOvrLdCntrl.tgtUlTpt, 
            cell->thresholds.maxUlItbs);
#endif
   }

   RETVOID;
}
/**
 * @brief Handler for the num UE per TTI based CPU OvrLd instr updating
 *
 * @details
 *
 *     Function : rgSCHUtlChkAndUpdNumUePerTtiCpuOvInstr
 *
 *     Processing Steps:
 *      - Validate the config params.
 *      - Update numUEperTTi CPU OL related information.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgSchCellCb *cell
 *  @param[in]  U8          cnrtCpuOvrLdIns 
 *  @return     Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHUtlChkAndUpdNumUePerTtiCpuOvInstr
(
 RgSchCellCb *cell, 
 U8          crntCpuOvrLdIns
)
#else
PRIVATE S16 rgSCHUtlChkAndUpdNumUePerTtiCpuOvInstr(cell, crntCpuOvrLdIns)
 RgSchCellCb *cell; 
 U8          crntCpuOvrLdIns;
#endif
{
   RgSchCpuOvrLdCntrlCb    *cpuInstr = &(cell->cpuOvrLdCntrl);
   RgSchCmnCell            *cellSch;
   U8                       maxUeNewDlTxPerTti;
   U8                       maxUeNewUlTxPerTti;
   U8                       tmpslot        = 0;
#ifdef CPU_OL_DBG_PRINTS 
   U8                       idx = 0;
#endif
   U8                       maxDlDecCnt;
   U8                       maxUlDecCnt;

   cellSch = RG_SCH_CMN_GET_CELL(cell);

   maxUeNewDlTxPerTti = cellSch->dl.maxUeNewTxPerTti; 
   maxUeNewUlTxPerTti = cellSch->ul.maxUeNewTxPerTti;
  
   /* Calculate Maximum Decremen */
   maxDlDecCnt =  (10*(maxUeNewDlTxPerTti - 1))-(10-RGR_MAX_PERC_NUM_UE_PER_TTI_RED);
   maxUlDecCnt =  (10*(maxUeNewUlTxPerTti - 1))-(10-RGR_MAX_PERC_NUM_UE_PER_TTI_RED);

   /* Check for DL CPU Commands */
   if ( crntCpuOvrLdIns &  RGR_CPU_OVRLD_DL_DEC_NUM_UE_PER_TTI ) 
   {
      /* Decrement till 90% of maxUeNewDlTxPerTti */
      if ( cpuInstr->dlNxtIndxDecNumUeTti < maxDlDecCnt )
      {
         tmpslot = (cpuInstr->dlNxtIndxDecNumUeTti) % 10;
         cpuInstr->dlNxtIndxDecNumUeTti++;
         if ( cpuInstr->maxUeNewTxPerTti[tmpslot] > 1 )
         {
            cpuInstr->maxUeNewTxPerTti[tmpslot]--;
         }
         else
         {
#ifdef CPU_OL_DBG_PRINTS
            printf("CPU_OL_TTI__ERROR\n");
#endif
            RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Invalid CPU OL");
         }
      }
#ifdef CPU_OL_DBG_PRINTS
      printf("dlNxtIndxDecNumUeTti = %d\n", cpuInstr->dlNxtIndxDecNumUeTti);
#endif
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cell->cellId,"dlNxtIndxDecNumUeTti = %d",
         cpuInstr->dlNxtIndxDecNumUeTti);
   }
   else if ( crntCpuOvrLdIns &  RGR_CPU_OVRLD_DL_INC_NUM_UE_PER_TTI )
   {
      if ( cpuInstr->dlNxtIndxDecNumUeTti >  0) 
      {
         cpuInstr->dlNxtIndxDecNumUeTti--;
         tmpslot = (cpuInstr->dlNxtIndxDecNumUeTti) % 10;
         if ( cpuInstr->maxUeNewTxPerTti[tmpslot] < maxUeNewDlTxPerTti )
         {
            cpuInstr->maxUeNewTxPerTti[tmpslot]++;
         }
         else
         {
#ifdef CPU_OL_DBG_PRINTS
            printf("CPU_OL_TTI__ERROR\n");
#endif
            RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Invalid CPU OL");
         }
      }
#ifdef CPU_OL_DBG_PRINTS
      printf("dlNxtIndxDecNumUeTti = %d\n", cpuInstr->dlNxtIndxDecNumUeTti);
#endif
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cell->cellId,"dlNxtIndxDecNumUeTti = %d",
         cpuInstr->dlNxtIndxDecNumUeTti);
   }
   /* Check for UL CPU commands */
   if ( crntCpuOvrLdIns &  RGR_CPU_OVRLD_UL_DEC_NUM_UE_PER_TTI )
   {
      /* Decrement till 90% of maxUeNewDlTxPerTti */
      if ( cpuInstr->ulNxtIndxDecNumUeTti < maxUlDecCnt )
      {
         tmpslot = (cpuInstr->ulNxtIndxDecNumUeTti) % 10;
         cpuInstr->ulNxtIndxDecNumUeTti++;
         if ( cpuInstr->maxUeNewRxPerTti[tmpslot] > 1 )
         {
            cpuInstr->maxUeNewRxPerTti[tmpslot]--;
         }
         else
         {
#ifdef CPU_OL_DBG_PRINTS
            printf("CPU_OL_TTI__ERROR\n");
#endif
            RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Invalid CPU OL");
         }
      }
#ifdef CPU_OL_DBG_PRINTS
      printf("ulNxtIndxDecNumUeTti = %d\n", cpuInstr->ulNxtIndxDecNumUeTti);
#endif
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cell->cellId,"dlNxtIndxDecNumUeTti = %d",
         cpuInstr->dlNxtIndxDecNumUeTti);
   }
   else if ( crntCpuOvrLdIns &  RGR_CPU_OVRLD_UL_INC_NUM_UE_PER_TTI )
   {
      if ( cpuInstr->ulNxtIndxDecNumUeTti >  0) 
      {
         cpuInstr->ulNxtIndxDecNumUeTti--;
         tmpslot = (cpuInstr->ulNxtIndxDecNumUeTti) % 10;
         if ( cpuInstr->maxUeNewRxPerTti[tmpslot] < maxUeNewUlTxPerTti )
         {
            cpuInstr->maxUeNewRxPerTti[tmpslot]++;
         }
         else
         {
#ifdef CPU_OL_DBG_PRINTS
            printf("CPU_OL_TTI__ERROR\n");
#endif
            RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Invalid CPU OL");
         }
      }
#ifdef CPU_OL_DBG_PRINTS
      printf("ulNxtIndxDecNumUeTti = %d\n", cpuInstr->ulNxtIndxDecNumUeTti);
#endif
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cell->cellId,"dlNxtIndxDecNumUeTti = %d",
         cpuInstr->dlNxtIndxDecNumUeTti);
   }
#ifdef CPU_OL_DBG_PRINTS 
 /* TODO: Debug Information - Shall be moved under CPU_OL_DBG_PRINTS */
   printf("maxUeNewDlTxPerTti = %d, maxUeNewUlTxPerTti = %d\n", maxUeNewDlTxPerTti, maxUeNewUlTxPerTti);
   printf("DL Sf numUePerTti:");
   for ( idx = 0; idx < 10 ; idx ++ )
   {
      printf("  %d", cpuInstr->maxUeNewTxPerTti[idx]);  
   }
   printf("\nUL Sf numUePerTti:");
   for ( idx = 0; idx < 10 ; idx ++ )
   {
      printf("  %d", cpuInstr->maxUeNewRxPerTti[idx]);  
   }
   printf("\n");
#endif

   RETVOID;
} /* rgSCHUtlChkAndUpdNumUePerTtiCpuOvInstr */

/**
 * @brief Handler for the CPU OvrLd related cell Recfg.
 *
 * @details
 *
 *     Function : rgSCHUtlResetCpuOvrLdState
 *
 *     Processing Steps:
 *      - Validate the config params.
 *      - Update CPU OL related state information.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgSchCellCb *cell
 *  @param[in]  U8          cnrtCpuOvrLdIns 
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlResetCpuOvrLdState
(
 RgSchCellCb *cell, 
 U8          crntCpuOvrLdIns
)
#else
PUBLIC S16 rgSCHUtlResetCpuOvrLdState(cell, crntCpuOvrLdIns)
 RgSchCellCb *cell; 
 U8          crntCpuOvrLdIns;
#endif
{
   U8              crntDlCpuOL=0;
   U8              crntUlCpuOL=0;
   RgSchCmnCell    *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   U8 idx;

   TRC3(rgSCHUtlResetCpuOvrLdState)

#ifdef CPU_OL_DBG_PRINTS
   printf("\n CPU OVR LD Ins Rcvd = %d\n", (int)crntCpuOvrLdIns);
#endif
   RLOG_ARG0(L_INFO,DBG_CELLID,cell->cellId,"CPU OVR LD Ins Rcvd");

   if ( RGR_CPU_OVRLD_RESET == crntCpuOvrLdIns )
   {
      /* The CPU OL instruction received with RESET (0), hence reset it */
#ifdef CPU_OL_DBG_PRINTS
      printf("rgSCHUtlResetCpuOvrLdState: RESET CPU OL instr\n");
#endif
      RLOG_ARG0(L_INFO,DBG_CELLID,cell->cellId,"RESET CPU OVR LD");
      cell->cpuOvrLdCntrl.cpuOvrLdIns = 0;
      /* Reset the max UL and DL itbs to 26 */
      cell->thresholds.maxUlItbs = RG_SCH_UL_MAX_ITBS;
      cell->thresholds.maxDlItbs = RG_SCH_DL_MAX_ITBS;
      /* Reset the num UE per TTI intructions */
      cell->cpuOvrLdCntrl.dlNxtIndxDecNumUeTti = 0;
      cell->cpuOvrLdCntrl.ulNxtIndxDecNumUeTti = 0;
      for ( idx = 0; idx < 10; idx++ )
      {
         cell->cpuOvrLdCntrl.maxUeNewTxPerTti[idx] = 
            schCmnCell->dl.maxUeNewTxPerTti;
         cell->cpuOvrLdCntrl.maxUeNewRxPerTti[idx] = 
            schCmnCell->ul.maxUeNewTxPerTti;
      }

      RETVALUE(ROK);
   }
   /* Check and Update numUEPer TTI based CPU overload instruction before
    * going for TP based CPU OL  
    * TTI based intrcuctions shall be > 0xF */
   if ( crntCpuOvrLdIns >  0xF )  
   {
      rgSCHUtlChkAndUpdNumUePerTtiCpuOvInstr(cell, crntCpuOvrLdIns);
      /* If need to have both TP and numUePerTti instrcution together in
       * one command then dont return from here */
      RETVALUE(ROK);
   }

   crntDlCpuOL = (crntCpuOvrLdIns & RGR_CPU_OVRLD_DL_TPT_UP) +\
                 (crntCpuOvrLdIns & RGR_CPU_OVRLD_DL_TPT_DOWN);
   if ((crntDlCpuOL) && (crntDlCpuOL != RGR_CPU_OVRLD_DL_TPT_UP) && 
       (crntDlCpuOL != RGR_CPU_OVRLD_DL_TPT_DOWN))
   {
      /* Cfg validation failed. Invalid Command. Either UP/DOWN is allowed */
      RETVALUE(RFAILED);
   }
   crntUlCpuOL = (crntCpuOvrLdIns & RGR_CPU_OVRLD_UL_TPT_UP) +\
                 (crntCpuOvrLdIns & RGR_CPU_OVRLD_UL_TPT_DOWN);
   if ((crntUlCpuOL) && (crntUlCpuOL != RGR_CPU_OVRLD_UL_TPT_UP) && 
       (crntUlCpuOL != RGR_CPU_OVRLD_UL_TPT_DOWN))
   {
      /* Cfg validation failed. Invalid Command. Either UP/DOWN is allowed */
      RETVALUE(RFAILED);
   }
   if ((crntDlCpuOL == 0) && (crntUlCpuOL == 0))
   {
      /* Cfg validation failed. Invalid Command. Either UP/DOWN is allowed */
      RETVALUE(RFAILED);
   }

   cell->cpuOvrLdCntrl.cpuOvrLdIns = crntCpuOvrLdIns;

   if (crntUlCpuOL)
   {
      if (crntUlCpuOL == RGR_CPU_OVRLD_UL_TPT_DOWN)
      {
         cell->cpuOvrLdCntrl.tgtUlTpt = cell->measurements.ulTpt - \
            (cell->measurements.ulTpt * 3 )/100;
      }
      else
      {
         cell->cpuOvrLdCntrl.tgtUlTpt = cell->measurements.ulTpt + \
            (cell->measurements.ulTpt * 2 )/100;
      }
      RLOG_ARG3(L_DEBUG,DBG_CELLID,cell->cellId,"CPU OVR LD UL Reset to "
            "%d, %lu, %lu", (int)crntUlCpuOL, cell->cpuOvrLdCntrl.tgtUlTpt,cell->measurements.ulTpt);
#ifdef CPU_OL_DBG_PRINTS
      printf("\n CPU OVR LD UL Reset to= %d, %lu, %lu\n", (int)crntUlCpuOL, cell->cpuOvrLdCntrl.tgtUlTpt,
		              cell->measurements.ulTpt);
#endif
   }

   if (crntDlCpuOL)
   {
      if (crntDlCpuOL == RGR_CPU_OVRLD_DL_TPT_DOWN)
      {
         cell->cpuOvrLdCntrl.tgtDlTpt = cell->measurements.dlTpt - \
                                        (cell->measurements.dlTpt * 1 )/100;
      }
      else
      {
         cell->cpuOvrLdCntrl.tgtDlTpt = cell->measurements.dlTpt + \
            (cell->measurements.dlTpt * 1 )/100;
      }
      RLOG_ARG3(L_DEBUG,DBG_CELLID,cell->cellId,"CPU OVR LD DL Reset to "
            "%d, %lu, %lu", (int)crntDlCpuOL, cell->cpuOvrLdCntrl.tgtDlTpt,cell->measurements.dlTpt);

#ifdef CPU_OL_DBG_PRINTS
      printf("\n CPU OVR LD DL Reset to= %d, %lu, %lu\n", (int)crntDlCpuOL, cell->cpuOvrLdCntrl.tgtDlTpt,
		              cell->measurements.dlTpt);
#endif
   }
   rgSCHUtlCpuOvrLdAdjItbsCap(cell);
   RETVALUE(ROK);
}
#ifdef EMTC_ENABLE
PUBLIC S16 rgSCHUtlAddToResLst
(
 CmLListCp   *cp,  
 RgSchIotRes *iotRes
 )
{
   cmLListAdd2Tail(cp, &iotRes->resLnk);
   iotRes->resLnk.node = (PTR)iotRes;
   RETVALUE(ROK);
}
PUBLIC S16 rgSCHUtlDelFrmResLst
(
RgSchUeCb *ue,
RgSchIotRes *iotRes
)
{
   CmLListCp  *cp = NULLP;
   RgSchEmtcUeInfo *emtcUe = NULLP;
   emtcUe = RG_GET_EMTC_UE_CB(ue);
   if(iotRes->resType == RG_SCH_EMTC_PUCCH_RES)
   {
      cp = &emtcUe->ulResLst;
   }else if(iotRes->resType == RG_SCH_EMTC_PDSCH_RES)
   {
      cp = &emtcUe->dlResLst;
   }else
   {
      RLOG0(L_INFO, "*****restype mismatch");
   }
   if(cp != NULLP )
   { 
      if(cp->count == 0)
      {
         RLOG0(L_INFO,"****error count*****\n");
         RETVALUE(ROK);
      }
   }
   cmLListDelFrm(cp, &iotRes->resLnk);
   iotRes->resLnk.node = NULLP;
   RETVALUE(ROK);
}
#endif
/**********************************************************************

         End of file
**********************************************************************/
