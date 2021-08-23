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
  
     File:     rg_sch_tom.c 
  
**********************************************************************/

/** @file rg_sch_tom.c 
@brief This module does processing related to handling of lower interface APIs 
invoked by PHY towards scheduler.
*/

/* header include files -- defines (.h) */
#include "common_def.h"
#include "tfu.h"           /* RGU defines */
#include "lrg.h"           /* layer management defines for LTE-MAC */
#include "rgr.h"           /* layer management defines for LTE-MAC */
#include "rgm.h"           /* layer management defines for LTE-MAC */
#include "rg_env.h"        /* defines and macros for MAC */
#include "rg_sch_err.h"    /* defines and macros for MAC */
#include "rg_sch_inf.h"    /* defines and macros for MAC */
#include "rg_sch.h"        /* defines and macros for MAC */
#include "rg_sch_cmn.h"    /* typedefs for MAC */


/* header/extern include files (.x) */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for MAC */
#include "rg_sch_cmn.x"    /* typedefs for MAC */
#ifdef EMTC_ENABLE
#include "rg_sch_emtc_ext.x"
#endif 
uint8_t rgSCHCmnCalcPcqiBitSz(RgSchUeCb    *ueCb,uint8_t numTxAnt);
S16 rgSCHDhm5gtfHqFdbkInd(RgSchUeCb *ue,RgSchCellCb *cell,CmLteTimingInfo timingInfo,TfuHqFdbk fdbk,RgSchErrInfo *err);
/* local defines */
#ifdef EMTC_ENABLE
Bool rgSCHEmtcChkEmtcUe ARGS(
(
RgSchCellCb  *cell,
uint16_t          rapId
));
Void rgSchTomTtiEmtcSched ARGS(
(
RgSchCellCb        *cell
));

S16 rgSCHEmtcRamVldtProcRaReq
(
uint8_t              raRntiCnt,
uint8_t              raReqCnt,
RgSchCellCb     *cell, 
TfuRaReqIndInfo *raReqInd,
RgSchUeCb       *ue,
Bool            *isEmtcUe,  
RgSchErrInfo    *err
);
Void rgSCHEmtcUpdCqiInfo
(
RgSchUeCb       *ue,
RgSchUePCqiCb   *cqiCb,
uint16_t             *cqiIdx
);
Void rgSCHEmtcUpdSRInfo
(
RgSchUeCb       *ue,
uint16_t             *srIdx
);
Void rgSCHCmnEmtcHdlCrcFailInd
(
RgSchCellCb       *cell,
RgSchRaCb         *raCb
);
S16 rgSCHEmtcTomUtlProcAtCrc
(
RgSchCellCb       *cell,
CmLteTimingInfo   crntHiDci0Frm,
TfuCntrlReqInfo   *cntrlInfo,
RgSchErrInfo      *err
);
Void rgSCHEmtcInitUeRecpReqLst
(
TfuRecpReqInfo  *recpReqInfo
);
Void rgSCHEmtcFillPucchRecpInfo
(
RgSchCellCb       *cell,
RgSchDlHqProcCb   *hqCb,
uint16_t               *hqRes
);
Bool rgSCHEmtcAddRecpInfoToLst
(
RgSchDlHqProcCb   *hqCb,
TfuRecpReqInfo    *recpReqInfo,
TfuUeRecpReqInfo  *pucchRecpInfo,
Bool              isEmtcUe
);
Void rgSCHEmtcWillUeRptCqi
(
RgSchUeCb         *ue, 
Bool              *willUeRprtCqi  
);
Void rgSchEmtcTomTtiCnsldtSfAlloc
(
RgSchCellCb        *cell
);

S16 rgSchEmtcTomTtiL1DlAndUlCfg
(
RgSchCellCb        *cell,
RgTfuCntrlReqInfo  *cntrlInfo
);

S16 rgSCHTomEmtcUtlFillDatRecpReq
(
TfuRecpReqInfo       *recpReqInfo,
RgSchCellCb          *cell,
uint16_t                  validIdx,
RgSchErrInfo         *err
);

S16 rgSCHEmtcTomUtlFillHqFdbkRecpReq
(
TfuRecpReqInfo          *recpReqInfo,
RgSchCellCb             *cell,
uint16_t                  validIdx,
RgSchErrInfo            *err
);

S16 rgSCHEmtcDhmRlsDlsfHqProc
(
RgSchCellCb          *cell,
CmLteTimingInfo      timingInfo
);

Void rgSCHEmtcCmnUlSch
(
RgSchCellCb  *cell
);

#ifdef RG_ULSCHED_AT_CRC
S16 rgSCHEmtcTomUtlProcDlSfAtCrc
(
RgSchEmtcDlSf        *ulSf,
CmLteTimingInfo      crntUlFrm,
RgSchCellCb          *cell,
TfuCntrlReqInfo      *cntrlInfo,
RgSchErrInfo         *err
);

RgSchEmtcDlSf* rgSCHEmtcUtlSubFrmGet
(
RgSchCellCb            *cell,
CmLteTimingInfo        frm
);
#endif

uint32_t gDlMpdcchBlank;
uint32_t gUlMpdcchBlank;
S16 rgSCHUtlIotResPrcTti
(
RgSchCellCb *cell
);

#endif

RgSchUeCb* rgSCHCmnGetHoUe
(
RgSchCellCb           *cell,
uint16_t                   rapId
);
RgSchUeCb* rgSCHCmnGetPoUe
(
RgSchCellCb           *cell,
uint16_t                   rapId,
CmLteTimingInfo       timingInfo
);
S16 rgSCHTomUtlFillDatAperRecpReq ARGS(
(
RgSchCellCb       *cell,
uint8_t                cqiReq,
RgSchUlAlloc      *alloc,
TfuUeRecpReqInfo  *datRecpInfo,
CmLteTimingInfo   *timeInfo,
Bool              hqPres,
uint16_t                validIdx
));

S16 rgSCHTomUtlFillDatPriRecpReq ARGS(
(
RgSchCellCb       *cell,
RgSchUlAlloc      *alloc,
TfuUeRecpReqInfo  *datRecpInfo,
CmLteTimingInfo   *timeInfo,
Bool              hqPres,
uint16_t               validIdx
));

S16 rgSCHTomUtlFillDatPCqiRecpReq ARGS(
(
RgSchCellCb       *cell,
RgSchUlAlloc      *alloc,
TfuUeRecpReqInfo  *datRecpInfo,
CmLteTimingInfo   *timeInfo,
Bool              hqPres, 
uint16_t                validIdx
));

S16 rgSCHTomUtlFillDatSrsRecpReq ARGS(
(
RgSchCellCb       *cell,
RgSchUlAlloc      *alloc,
TfuUeRecpReqInfo  *datRecpInfo,
CmLteTimingInfo   *timeInfo,
Bool              hqPres
));

void schFillCrntTime( SlotTimingInfo slotInd,Inst schInst);

#ifdef CA_DBG
uint32_t delayedApiCnt;
uint32_t gPCellTb1AckCount=0,gPCellTb2AckCount=0,gPCellTb1NackCount=0,gPCellTb2NackCount=0;
uint32_t gSCellSchedCount=0,gPrimarySchedCount=0;
uint32_t gSCellTb1AckCount=0,gSCellTb2AckCount=0,gSCellTb1NackCount=0,gSCellTb2NackCount=0;
uint32_t gPCellTb1DtxCount = 0, gPCellTb2DtxCount = 0, gSCellTb1DtxCount = 0, gSCellTb2DtxCount = 0;
uint32_t gHqFdbkCount = 0;

 

uint32_t gCqiRecpCount = 0;
uint32_t gCqiRecpPuschCount = 0;
uint32_t gCqiRcvdCount = 0;
Bool gF1bCsPres = FALSE;
uint32_t  gRiReqCount = 0;
uint32_t gCqiReqCount = 0;
uint32_t gF1bCsCount = 0;
uint32_t gACqiRcvdCount = 0;
uint32_t gCqiReptToAppCount = 0;
uint32_t gRawACqiCount= 0;
uint32_t gCqiDropCount,gPucchDropCount;
uint32_t gCqiPucchLowSnrDropCount,gCqiPucchConfMaskDropCount,gCqiPuschConfMaskDropCount;
uint32_t gDci0Count = 0;
uint32_t gUlCrcFailCount = 0;
uint32_t gUlCrcPassCount = 0;
uint32_t gPuschCqiDropCount = 0;
uint32_t gCaDbgCaFrmt = 0;
uint32_t gCaDbgNonCaFrmt = 0;
uint32_t gPcellZeroBoOcc=0,gScellZeroBoOcc=0, dbgDelayedDatReqInMac=0,gDropDatReqCnt=0, gIccPktRcvrMemDropCnt=0;
#endif

#ifdef EMTC_ENABLE
uint32_t gUlCrcFailCounter = 0;
uint32_t gUlCrcPassCounter = 0;
#endif

#ifdef RG_5GTF
uint32_t gUl5gtfPdcchSend;
#endif

#ifdef UNUSED_FUNC
#ifdef TFU_UPGRADE
static S16 rgSCHTomUtlFillCqiSrsWithSr ARGS
((
RgSchCellCb       *cell,
RgSchUeCb         *ue,
TfuRecpReqInfo    *recpReqInfo,
TfuUeRecpReqInfo  *pucchRecpInfo,
uint16_t               validIdx
));
static Bool rgSCHTomUtlFillDatHarqRecpReq ARGS
((
RgSchCellCb       *cell,
RgSchUlAlloc      *alloc,
TfuUeRecpReqInfo  *datRecpInfo,
TfuRecpReqInfo    *recpReqInfo
));
static S16 rgSCHTomUtlFillSrRecpReq ARGS((
TfuRecpReqInfo   *recpReq,
RgSchCellCb      *cell,
uint16_t              validIdx,
RgSchErrInfo     *err));
static S16 rgSCHTomUtlFillRiRecpReq ARGS((
TfuRecpReqInfo   *recpReq,
RgSchCellCb      *cell,
uint16_t              validIdx,
RgSchErrInfo     *err));
static S16 rgSCHTomUtlFillPcqiRecpReq ARGS((
TfuRecpReqInfo   *recpReq,
RgSchCellCb      *cell,
uint16_t              validIdx,
RgSchErrInfo     *err));
static S16 rgSCHTomUtlFillSrsRecpReq ARGS((
TfuRecpReqInfo   *recpReq,
RgSchCellCb      *cell,
uint16_t              validIdx,
RgSchErrInfo     *err));
static S16 rgSCHTomUtlGenIndices ARGS((
uint32_t      label,
uint8_t        posM,
uint8_t        valN,
uint8_t        valK,
TfuSubbandInfo*    sbInfo));
#endif
static S16 rgSCHTomUtlFillCqiRiRecpReq ARGS(
(
TfuRecpReqInfo       *recpReqInfo,
RgSchCellCb          *cell,
uint16_t                  validIdx, 
RgSchErrInfo         *err
));
static Void rgSchTomFillCellTtiInfo ARGS
((
TfuTtiIndInfo      *ttiInd,
Inst               schInst,
uint8_t                 *nCell,
RgSchCellCb        *cells[]
));
#endif

/* local typedefs */
uint32_t rgBwAlloInfo[RGSCH_NUM_SUB_FRAMES]; /* Num of Rbs Allocated in each SF */ 
uint32_t rgBwAlcnt[RGSCH_NUM_SUB_FRAMES];   /*Num of times Allocation done in each Subframe */

/* local externs */
/* rg006.201: [ccpu000111706, ccpu00112394]: Separated UL and DL TTI
 * processing */
#ifdef LTE_L2_MEAS
   uint64_t     glblTtiCnt = 0;
#endif
uint32_t ri1Cnt ;
uint32_t ri2Cnt ;  
uint32_t gDlNumUePerTti[20] = {0};
uint32_t gUlNumUePerTti[20] = {0};
static S16 rgSCHTomUtlProcDlSf ARGS((
         RgSchDlSf        *dlSf,
         RgSchDlSf        *ulSf,
         RgSchCellCb      *cell,
         RgTfuCntrlReqInfo *cntrlInfo,
         RgSchErrInfo     *err));
#ifdef RG_ULSCHED_AT_CRC
static S16 rgSCHTomUtlProcDlSfAtCrc ARGS((
         RgSchDlSf        *ulSf,
	 CmLteTimingInfo  crntUlFrm,
         RgSchCellCb      *cell,
         TfuCntrlReqInfo  *cntrlInfo,
         RgSchErrInfo     *err));
#endif /* RG_ULSCHED_AT_CRC */
#ifdef LTE_TDD
#ifdef TFU_UPGRADE
static S16 rgSCHTomUtlPrcUlTddSpclSf ARGS((
         RgSchCellCb        *cell,
         RgSchErrInfo       *err));
#endif /* TFU_UPGRADE */
#endif
static S16 rgSCHTomUtlFillPhich ARGS((
         RgSchCellCb     *cell,
         TfuCntrlReqInfo  *cntrlInfo,
         RgSchDlSf        *dlSf,
         RgSchErrInfo     *err));

static S16 rgSCHTomUtlFillDlPdcch ARGS((
         RgSchCellCb      *cell,
         TfuCntrlReqInfo  *cntrlInfo,
         RgSchDlSf        *dlSf,
         RgSchErrInfo     *err));
static S16 rgSCHTomUtlFillUlPdcch ARGS((
         RgSchCellCb       *cell,
         TfuCntrlReqInfo  *cntrlInfo,
         RgSchDlSf        *ulSf,
         RgSchErrInfo     *err));

static S16 rgSCHTomUtlProcTA ARGS((
         RgSchCellCb      *cell));
#ifdef TFU_UPGRADE
static S16 rgSCHTomUtlFillHqFdbkRecpReq ARGS((
         TfuRecpReqInfo   *recpReq,
         RgSchCellCb      *cell,
         uint16_t              validIdx,
        RgSchErrInfo     *err));
#else
static S16 rgSCHTomUtlFillHqFdbkRecpReq ARGS((
         TfuRecpReqInfo   *recpReq,
         RgSchCellCb      *cell,
         RgSchErrInfo     *err));
#endif
#ifdef TFU_UPGRADE

S16 rgSCHTomFillOnlySrsRecpReq ARGS
((
RgSchCellCb       *cell,
RgSchUlAlloc      *alloc,
TfuUeRecpReqInfo  *datRecpInfo
));
static S16 rgSCHTomUtlFillCqiSrSrsWithHq ARGS
((
RgSchCellCb         *cell,
TfuRecpReqInfo      *recpReqInfo,
RgSchUeCb           *ue,
TfuUeRecpReqInfo    *pucchRecpInfo,
uint16_t                  validIdx,
Bool                 isDatPresOnSecCell
));

S16 rgSCHTomUtlFillRiBitWidthInfo ARGS
((
RgSchUeCb          *ueCb
));

uint8_t rgSCHTomUtlFetchPcqiBitSz ARGS
((
RgSchUeCb          *ueCb, 
uint8_t                 numTxAnt,
uint8_t                 *ri
));

uint8_t rgSCHTomUtlFetchPcqiBitSzPucchMode21 ARGS
((
RgSchUeCb          *ueCb, 
TfuCqiPucchMode21   *mode21Info,
uint8_t                 numTxAnt,
uint8_t                 *ri
));

S16 rgSCHTomUtlMoveNxtOccasion ARGS
((
RgSchCellCb        *cell,
RgSchUeCb          *ueCb,
uint16_t                validIdx
));

static S16 rgSCHTomUtlMovePcqiNxtOccasion ARGS
((
RgSchCellCb     *cell,
RgSchUeCb       *ue,
RgSchUePCqiCb   *cqiCb
));

static S16 rgSCHTomUtlMovePriNxtOccasion ARGS
((
RgSchCellCb     *cell,
RgSchUeCb       *ue,
RgSchUePCqiCb   *riCb
));

static S16 rgSCHTomUtlMoveSrNxtOccasion ARGS
((
RgSchCellCb     *cell,
RgSchUeCb       *ue
));

static S16 rgSCHTomUtlMoveSrsNxtOccasion ARGS
((
RgSchCellCb     *cell,
RgSchUeCb       *ue
));
static S16 rgSCHTomUtlWillUeRprtCqiRi ARGS((
         RgSchUeCb        *ue,
         Bool             *willueRprtCqiRii));
#endif 
#ifdef TFU_UPGRADE
static S16 rgSCHTomUtlFillDatRecpReq ARGS((
         TfuRecpReqInfo   *recpReq,
         RgSchCellCb      *cell,
         uint16_t              validIdx,
         RgSchErrInfo     *err));
#else
static S16 rgSCHTomUtlFillDatRecpReq ARGS((
         TfuRecpReqInfo   *recpReq,
         RgSchCellCb      *cell,
         RgSchErrInfo     *err));
#endif

#ifdef LTE_TDD
#ifdef TFU_UPGRADE
static S16 rgSCHTomUtlFillSfRepHqFdbk ARGS((
      TfuRecpReqInfo *recpReqInfo,
      RgSchCellCb    *cell,
      RgSchErrInfo   *err,
      RgSchDlSf      *dlSf,
      uint8_t             noFdbks,
      CmMemListCp    *memCp,
      uint8_t             elemIdx,
      RgSchDlSf      *nxtDlsf,
      uint16_t              validIdx
     ));
#else
static S16 rgSCHTomUtlFillSfRepHqFdbk ARGS((
      TfuRecpReqInfo *recpReqInfo,
      RgSchCellCb    *cell,
      RgSchErrInfo   *err,
      RgSchDlSf      *dlSf,
      uint8_t             noFdbks,
      CmMemListCp    *memCp,
      uint8_t             elemIdx,
      RgSchDlSf      *nxtDlsf
     ));
#endif
#ifdef TFU_UPGRADE
static S16 rgSCHTomUtlFillSfHqFdbk ARGS((
      TfuRecpReqInfo *recpReqInfo,
      RgSchCellCb    *cell,
      RgSchErrInfo   *err,
      RgSchDlSf      *dlSf,
      uint8_t             noFdbks,
      CmMemListCp    *memCp,
      uint8_t             elemIdx,
      RgSchDlSf      *nxtDlsf,
      uint16_t              validIdx
     ));
#else
static S16 rgSCHTomUtlFillSfHqFdbk ARGS((
      TfuRecpReqInfo *recpReqInfo,
      RgSchCellCb    *cell,
      RgSchErrInfo   *err,
      RgSchDlSf      *dlSf,
      uint8_t             noFdbks,
      CmMemListCp    *memCp,
      uint8_t             elemIdx,
      RgSchDlSf      *nxtDlsf
     ));
#endif

static S16 rgSCHTomUtlFillSfHqFdbkForOneUe ARGS((
      RgSchDlHqProcCb         *hqCb,
      TfuRecpReqInfo          *recpReqInfo,
      RgSchCellCb             *cellCb,
      RgSchErrInfo            *err,
      RgSchDlSf               *dlSf,
      uint8_t                      noFdbks,
      CmMemListCp             *memCp,
      uint8_t                      elemIdx,
      RgSchDlSf               *nxtDlsf,
      CmLteRnti               rnti,
      RgrTddAckNackMode       ackNackMode,
      RgSchUePucchRecpInfo    **pucchInfoRef,
      RgSchPdcch              *pdcch,
      TknUInt16                  n1PucchTkn,
      Bool                    *allocRef,
      uint8_t                      hqSz
      ));
#endif
#ifdef LTEMAC_SPS
Void rgSCHCmnDlSpsSch (RgSchCellCb *cell);
#ifndef LTE_TDD
#ifdef UNUSED_FUNC
#ifdef TFU_UPGRADE
static S16 rgSCHTomCnsdrRelPdcch ARGS
((
RgSchCellCb    *cell,
RgSchDlSf      *dlSf,
TfuRecpReqInfo *recpReqInfo,
uint16_t       validIdx,
RgSchErrInfo   *err
));
#else
static S16 rgSCHTomCnsdrRelPdcch ARGS
((
 RgSchCellCb    *cell,
 RgSchDlSf      *dlSf,
 TfuRecpReqInfo *recpReqInfo,
 RgSchErrInfo    *err
 ));
#endif
#endif
#endif
#endif

static Void rgSchTomTtiMiscFunctions ARGS
((
RgSchCellCb  *cell
));

static Void rgSchTomTtiUlAndDlCmnChSch ARGS
((
RgSchCellCb  *cell
));

static Void rgSchTomTtiDlSch ARGS
((
RgSchCellCb  *cell
));

static Void rgSchTomTtiCnsldtSfAlloc ARGS
((
RgSchCellCb  *cell
));

static Void rgSchTomTtiL1DlAndUlCfg ARGS
((
RgSchCellCb  *cell,
RgTfuCntrlReqInfo  *cntrlInfo
));

#ifdef RGR_RRM_TICK   
static Void rgSCHTomUtlSendSfnTick ARGS
((
RgSchCellCb  *cell
));
#endif
#ifdef LTE_TDD
static Void rgSchTomUtlTddRlsSfAndHarq ARGS
((
RgSchCellCb        *cell
));
static Void rgSCHTomUtlProcTddUlSf ARGS
((
RgSchCellCb        *cell
));
#ifdef LTE_ADV
static Void rgSCHTomUtlGethqRes ARGS
((
uint8_t                      noFdbks,
RgSchDlSf               *dlSf,
RgSchPdcch              *pdcch,
RgSchCellCb             *cellCb,
uint16_t                     *hqRes
));
static Void rgSCHTomUtlFillSfHqFdbkForFrmt1BCSForM1 ARGS
((
RgSchDlHqProcCb     *hqCb,
TfuUePucchRecpReq   *hqRecpReq,
uint8_t                  noFdbks,
RgSchDlSf           *dlSf,
RgSchPdcch          *pdcch,
RgSchCellCb         *cellCb
));
static Void rgSCHTomUtlFillSfHqFdbkForFrmt1BCSForM234 ARGS
((
RgSchDlHqProcCb         *hqCb,
TfuUePucchRecpReq       *hqRecpReq,
uint8_t                      noFdbks,
RgSchDlSf               *dlSf,
RgSchPdcch              *pdcch,
RgSchCellCb             *cellCb,
uint8_t                      elemIdx
));
#endif/*LTE_ADV*/
#endif/*LTE_TDD*/

uint32_t rgDlCqiRptCnt[16], rgTotDlCqiRpt;

#ifdef RG_5GTF
uint32_t rgSch5gtfCqi2Mcs[15] = 
    {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
#endif
/* forward references */
#ifdef TFU_UPGRADE
/*HARQ Feedback interpretation in accordance with Femto Forum.
Note: There is no value as '0' in Femto Forum Spec but in order to retain
the existing usage in MAC (and its Acceptance), its being considered*/
//const static uint8_t rgSchTomHqFbkMap[8] = {0,1,0,0,4,4,4,4};
/*added #defines instead of magic numbers*/
/*const static uint32_t rgSCHTomBinCoe[RG_SCH_MAX_NUM_UE_SEL_SUBBANDS][RG_SCH_MAX_TOT_NUM_SUBBANDS]={
{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28},
{0,1,3,6,10,15,21,28,36,45,55,66,78,91,105,120,136,153,171,190,210,231,253,276,300,325,351,378},
{0,0,1,4,10,20,35,56,84,120,165,220,286,364,455,560,680,816,969,1140,1330,1540,1771,2024,2300,2600,2925,3276},
{0,0,0,1,5,15,35,70,126,210,330,495,715,1001,1365,1820,2380,3060,3876,4845,5985,7315,8855,10626,12650,14950,17550,20475},
{0,0,0,0,1,6,21,56,126,252,462,792,1287,2002,3003,4368,6188,8568,11628,15504,20349,26334,33649,42504,53130,65780,80730,98280},
{0,0,0,0,0,1,7,28,84,210,462,924,1716,3003,5005,8008,12376,18564,27132,38760,54264,74613,100947,134596,177100,230230,296010,376740}
};
*/

/*ccpu00116923 - ADD - SRS present support*/
/*Tables Derived from 3GPP TS 36.211 Section 5.5.3.3 */
/* Table 5.5.3.3-1 */
#ifndef LTE_TDD
const RgSchFddCellSpSrsSubfrmTbl rgSchFddCellSpSrsSubfrmTbl = {
   {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
   {TRUE, FALSE, TRUE, FALSE, TRUE, FALSE, TRUE, FALSE, TRUE, FALSE},
   {FALSE, TRUE, FALSE, TRUE, FALSE, TRUE, FALSE, TRUE, FALSE, TRUE},
   {TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE},
   {FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE},
   {FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE},
   {FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE},
   {TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE},
   {FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE},
   {TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
   {FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
   {FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
   {FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
   {TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, FALSE, TRUE, FALSE},
   {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, FALSE},
   {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}
};
#else
/* Table 5.5.3.3-2 */
const RgSchTddCellSpSrsSubfrmTbl rgSchTddCellSpSrsSubfrmTbl = {
   {FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE},
   {FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE},
   {FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE},
   {FALSE, TRUE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, TRUE},
   {FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE},
   {FALSE, TRUE, TRUE, FALSE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE},
   {FALSE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, FALSE, TRUE, TRUE},
   {FALSE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE},
   {FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE},
   {FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE},
   {FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE},
   {FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE},
   {FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE},
   {FALSE, TRUE, FALSE, FALSE, TRUE, FALSE, TRUE, TRUE, FALSE, FALSE},
   {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
   {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}
};
#endif
S8 rgSchCmnAper20n22DiffCqi[4] = {1, 2, 3, 4};
S8 rgSchCmnAper30n31DiffCqi[4] = {0, 1, 2, -1};
#endif 

/**
 * @brief get Ue for dedicated preamble rach
 *
 * @details
 *
 *     Function: rgSCHGetDedPrmUe 
 *
 *     Invoked by: rgSCHTomRaReqInd 
 *
 *  @param[in] RgSchCellCb     *cell
 *  @param[in] TfuRaReqIndInfo *raReqInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHGetDedPrmUe 
(
RgSchCellCb     *cell,
uint16_t        rapId,
CmLteTimingInfo timingInfo,
RgSchUeCb       **ue
)
{
   RgSchCmnCell          *cellSch = (RgSchCmnCell *)(cell->sc.sch);

   DU_LOG("\nINFO  -->  SCH : rapId[%d] cellSch->rachCfg.dedPrmStart[%d] cellSch->rachCfg.numDedPrm[%d]\n",\
   rapId,cellSch->rachCfg.dedPrmStart,cellSch->rachCfg.numDedPrm);
   /* Finding UE in handOver List */
   if ((rapId < cellSch->rachCfg.dedPrmStart) ||
         (rapId > cellSch->rachCfg.dedPrmStart +
          cellSch->rachCfg.numDedPrm - 1))
   {
      /* This ded Preamble corresponds to handover */
      *ue = rgSCHCmnGetHoUe(cell, rapId);
      DU_LOG("\nDEBUG  -->  SCH : ded Preamble corresponds to handover\n");
   }
   else/* Finding UE from PDCCH Order Mappings */
   {
      /* Get the UE which has transmitted this RaReq */
      *ue = rgSCHCmnGetPoUe(cell, rapId, timingInfo);
      DU_LOG("\nDEBUG  -->  SCH :  UE from PDCCH Order Mapping\n");
   }
   return ROK;
}
/**
 * @brief Handler for processing Random Access request indication 
 * recieved from PHY.
 *
 * @details
 *
 *     Function: rgSCHTomRaReqInd
 *
 *     Handler for processing Random Access request indication recieved from
 *     PHY.
 *
 *     Invoked by: RgLiTfuRaReqInd of LIM
 *
 *     Processing Steps: 
 *      - Validate the information received: cellId value and raRnti values
 *      - Process the request: Call rgSCHRamProcRaReq (cell, raRnti, raReqInd)
 *           
 *  @param[in] RgSchCellCb     *cell
 *  @param[in] TfuRaReqIndInfo *raReqInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHTomRaReqInd
(
RgSchCellCb     *cell,
TfuRaReqIndInfo *raReqInd
)
{
   S16           ret;
   uint8_t       raRntiCnt;
   uint8_t       raReqCnt;
   RgSchErrInfo  err;
   Bool          isEmtcUe = FALSE;
   uint16_t      rapId;
   RgSchUeCb     *ue = NULLP;

   if(cell->cellId != raReqInd->cellId)
   {
      err.errType    = RGSCHERR_TOM_RAREQIND;
      err.errCause   = RGSCHERR_TOM_INV_CELL_ID;
      DU_LOG("\nERROR  -->  SCH : rgSCHTomRaReqInd(): No cell found with raReq cellId = (%d) errorType (%d)"
         " errorCause(%d)",raReqInd->cellId, err.errType, err.errCause);
      return RFAILED;
   } 
   
   for (raRntiCnt = 0; raRntiCnt < raReqInd->nmbOfRaRnti; raRntiCnt++)
   {
      for (raReqCnt = 0; raReqCnt < raReqInd->rachInfoArr->numRaReqInfo; raReqCnt++)
      {
         rapId = raReqInd->rachInfoArr[raRntiCnt].raReqInfoArr[raReqCnt].rapId;

         if(RGSCH_IS_DEDPRM(cell, rapId))
         {
            rgSCHGetDedPrmUe(cell, rapId, raReqInd->timingInfo, &ue);
            if(NULLP == ue)
            {
               /* Since rapId is within dedicated range and No ue context
                * is found means it is a spurious rach. So ignore it.*/
               continue;
            }
         }

         if(FALSE == isEmtcUe)
         {
#if (ERRCLASS & ERRCLS_DEBUG)
            if(raReqInd->rachInfoArr[raRntiCnt].raRnti > RGSCH_MAX_RA_RNTI)
            {
               DU_LOG("\nERROR  -->  SCH : rgSCHTomRaReqInd(): raRnti  is out of range\n");
               continue;
            }
#endif
            ret = rgSCHRamProcRaReq(raReqCnt, cell, raReqInd->rachInfoArr[raRntiCnt].raRnti, 
                  (TfuRachInfo *)&raReqInd->rachInfoArr[raRntiCnt],
                  raReqInd->timingInfo, ue, &err);
            if(ret == RFAILED)
            {
               err.errType = RGSCHERR_TOM_RAREQIND;
               DU_LOG("\nERROR  -->  SCH : RARNTI:%d rgSCHTomRaReqInd(): RAM processing failed errType(%d) "
                     "errCause(%d)", raReqInd->rachInfoArr[raRntiCnt].raRnti, 
                     err.errType, err.errCause);
               continue;
            }
         }
      }
   }
   return ROK;
}  /* rgSCHTomRaReqInd */


/**
 * @brief Handler for processing uplink CQI indication recieved from PHY.
 *
 * @details
 *
 *     Function: rgSCHTomUlCqiInd
 *
 *     Handler for processing uplink CQI indication recieved from PHY.
 *
 *     Invoked by: RgLiTfuUlCqiInd 
 *
 *     Processing Steps:
 *     - Gets UE
 *     - Invoke scheduler to push reported CQI info rgSCHUtlUlCqiInd 
 *           
 *  @param[in] RgSchCellCb     *cell
 *  @param[in] TfuUlCqiIndInfo *ulCqiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHTomUlCqiInd
( 
RgSchCellCb     *cell,
TfuUlCqiIndInfo *ulCqiInd
)
{
   RgSchUeCb    *ue;
   CmLList      *node;
   TfuUlCqiRpt  *ulCqiInfo;

   node =  ulCqiInd->ulCqiRpt.first;
   if(cell->cellId != ulCqiInd->cellId)
   {
      DU_LOG("\nERROR  -->  SCH : rgSCHTomUlCqiInd() Unable to get the ulCqiInd cell with id(%d)", 
         ulCqiInd->cellId);
      return RFAILED;
   }

   for (;node; node=node->next)
   {
      ulCqiInfo = (TfuUlCqiRpt *)node->node;
#if (ERRCLASS & ERRCLS_DEBUG)
      if(ulCqiInfo->numSubband == 0)
      {
         DU_LOG("\nERROR  -->  SCH : Num Subband is"
            "out of range RNTI:%d",ulCqiInfo->rnti);
         continue;
      }
#endif
      if((ue = rgSCHDbmGetUeCb(cell, ulCqiInfo->rnti)) == NULLP)
      {
#ifdef LTEMAC_SPS
         if((ue = rgSCHDbmGetSpsUeCb(cell, ulCqiInfo->rnti)) == NULLP)
#endif
         {
            DU_LOG("\nERROR  -->  SCH : Unable to get the ue for RNTI:%d", ulCqiInfo->rnti);
            continue;
         }
      }
      /* wideband cqi is directly reported now. and also isTxPort0 */
      rgSCHUtlUlCqiInd(cell, ue, ulCqiInfo);
   }
       return ROK;
}  /* rgSCHTomUlCqiInd */

/**
 * @brief Handler for processing PUCCH power adjustment indication
 *
 * @details
 *
 *     Function: rgSCHTomPucchDeltaPwrInd
 *
 *     Handler for processing PUCCH power adjustment indication
 *     received from PHY.
 *
 *     Invoked by: RgLiTfuPucchDeltaPwrInd 
 *
 *     Processing Steps:
 *     - Gets UE
 *     - Invoke scheduler to push reported CQI info rgSCHUtlPucchDeltaPwrInd 
 *           
 *  @param[in] RgSchCellCb             *cell
 *  @param[in] TfuPucchDeltaPwrIndInfo *pucchDeltaPwr
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHTomPucchDeltaPwrInd
( 
RgSchCellCb             *cell,
TfuPucchDeltaPwrIndInfo *pucchDeltaPwr
)
{
   RgSchUeCb       *ue;
   CmLList         *node;
   TfuPucchDeltaPwr  *ueElem;

   if(cell->cellId != pucchDeltaPwr->cellId)
   {
      DU_LOG("\nERROR  -->  SCH : rgSCHTomPucchDeltaPwrInd() Unable to get the pucchDeltaPwr cell with id(%d)", 
         pucchDeltaPwr->cellId);
      return RFAILED;
   }

   node =  pucchDeltaPwr->pucchDeltaPwrLst.first;
   for (;node; node=node->next)
   {
      ueElem = (TfuPucchDeltaPwr *)node->node;
      if((ue = rgSCHDbmGetUeCb(cell, ueElem->rnti)) == NULLP)
      {
#ifdef LTEMAC_SPS 
         if((ue = rgSCHDbmGetSpsUeCb(cell, ueElem->rnti)) == NULLP)
#endif
         {
            DU_LOG("\nERROR  -->  SCH : RNTI:%d "
                     "rgSCHTomPucchDeltaPwrInd() Unable to get the ue ", 
                     ueElem->rnti);
            continue;
         }
      }
      rgSCHUtlPucchDeltaPwrInd(cell, ue, ueElem->pucchDeltaPwr);
   }
   return ROK;
}  /* rgSCHTomPucchDeltaPwrInd */

/**
 * @brief Handler for processing harq ACK/NACK indication recieved from PHY.
 *
 * @details
 *
 *     Function: rgSCHTomHarqAckInd
 *
 *     Handler for processing harq ACK/NACK indication recieved from PHY.
 *
 *     Invoked by: RgLiTfuHqInd
 *
 *     Processing Steps:
 *     For each HqAckInfo received
 *     - Get UE
 *     - If UE doesnt exist look for a RaCb and invoke rgSCHRamMsg4FdbkInd
 *     - Invoke HARQ module to pass HARQ-ACK info rgSCHDhmHqFdbkInd
 *           
 *  @param[in]  TfuHqIndInfo *harqAckInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHTomHarqAckInd
(
RgSchCellCb     *cell,
TfuHqIndInfo    *harqAckInd
)
{
   RgSchErrInfo    err;
   RgSchUeCb       *ue;
   RgSchRaCb       *raCb;
   CmLList         *node;
   TfuHqInfo       *hqInfo;
   Pst             pst;
  // uint8_t              tbCnt;

   RgInfRlsHqInfo  *rlsHqBufs = &(cell->rlsHqArr[cell->crntHqIdx]);
   uint32_t            cellIdx;
   RgSchCellCb     *iterCellP; 
   
   if(cell->cellId != harqAckInd->cellId)
   {
      DU_LOG("\nERROR  -->  SCH : rgSCHTomHarqAckInd() Unable to get"
         " the cell for cellId (%d)", harqAckInd->cellId);
      err.errType = RGSCHERR_TOM_HARQACKIND;
      err.errCause = RGSCHERR_TOM_INV_CELL_ID;
      return RFAILED;
	}
#ifdef RG_5GTF   
   node =  harqAckInd->hqIndLst.first;
   for (;node; node=node->next)
   {
      hqInfo = (TfuHqInfo *)node->node;
      {
         rlsHqBufs = &(cell->rlsHqArr[cell->crntHqIdx]);
         TfuHqFdbk fdbk = hqInfo->isAck[0];
         raCb = rgSCHDbmGetRaCb (cell, hqInfo->rnti);
         ue = rgSCHDbmGetUeCb (cell, hqInfo->rnti);
         if (ue != NULLP && raCb == NULLP)
         {  
            if ((rgSCHDhm5gtfHqFdbkInd (ue, cell, harqAckInd->timingInfo, fdbk, &err)) != ROK)
            {
               err.errType = RGSCHERR_TOM_HARQACKIND;
               DU_LOG("\nERROR  -->  SCH : rgSCHTomHarqAckInd() "
                     "HARQ feedback processing failed errType(%d)errCause(%d)n",
                     err.errType, err.errCause); 
               continue;
            }
         }
      }
      
   }

   if ((rgSCHDhmRlsDlsfHqProc (cell, harqAckInd->timingInfo)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Unable to Release Downlink "
            "subframe for cellId (%d) ", cell->cellId);
      err.errType = RGSCHERR_TOM_HARQACKIND;
   }

   for (cellIdx = 0; cellIdx < CM_LTE_MAX_CELLS; cellIdx++)
   {
      if (NULLP != rgSchCb[cell->instIdx].cells[cellIdx])
      {
         iterCellP = rgSchCb[cell->instIdx].cells[cellIdx];

         rlsHqBufs = &(iterCellP->rlsHqArr[iterCellP->crntHqIdx]);
         if(rlsHqBufs->numUes)
         {
            rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], iterCellP->macInst);
            RgSchMacRlsHq (&pst, rlsHqBufs);
         }
         rlsHqBufs->numUes = 0;
      }
   }
#else 
   rlsHqBufs->numUes = 0;
   node =  harqAckInd->hqIndLst.first;
   for (;node; node=node->next)
   {
      hqInfo = (TfuHqInfo *)node->node;
         for(tbCnt=0; tbCnt<TFU_MAX_HARQ_FDBKS; tbCnt++)
         {
            hqInfo->isAck[tbCnt]=(TfuHqFdbk)rgSchTomHqFbkMap[hqInfo->isAck[tbCnt]];
         }
      raCb = rgSCHDbmGetRaCb (cell, hqInfo->rnti);
      ue = rgSCHDbmGetUeCb (cell, hqInfo->rnti);
      if (ue == NULLP && raCb != NULLP)
      {
#ifdef RG_UNUSED
         rgSCHRamMsg4FdbkInd (raCb);
#endif
         if ((rgSCHDhmHqFdbkInd (raCb, RGSCH_HQ_FDB_IND_CB_TYPE_RA_CB, 
                     cell, harqAckInd->timingInfo, hqInfo, rlsHqBufs, &err)) != ROK)
         {
            err.errType = RGSCHERR_TOM_HARQACKIND;
            DU_LOG("\nERROR  -->  SCH : rgSCHTomHarqAckInd() HARQ"
               " feedback processing failed errType(%d) errCause(%d)", 
               err.errType, err.errCause); 
            continue;
         }
         continue;
      }
      else if (ue != NULLP && raCb == NULLP)
      {
         /* Get the Downlink HARQ entity from ue */
         if ((rgSCHDhmHqFdbkInd (ue, RGSCH_HQ_FDB_IND_CB_TYPE_HQ_ENT, 
                     cell, harqAckInd->timingInfo, hqInfo, rlsHqBufs, &err)) != ROK)
         {
            err.errType = RGSCHERR_TOM_HARQACKIND;
            DU_LOG("\nERROR  -->  SCH : rgSCHTomHarqAckInd() "
               "HARQ feedback processing failed errType(%d)errCause(%d)n",
               err.errType, err.errCause);
            continue;
         }
      }
      else if (ue != NULLP && raCb != NULLP)
      {
         if ((rgSCHDhmHqFdbkInd (raCb, RGSCH_HQ_FDB_IND_CB_TYPE_RA_CB, 
                     cell, harqAckInd->timingInfo, hqInfo, rlsHqBufs, &err)) != ROK)
         {
            err.errType = RGSCHERR_TOM_HARQACKIND;
            DU_LOG("\nERROR  -->  SCH : rgSCHTomHarqAckInd() HARQ"
               " feedback processing failed errType(%d) errCause(%d).", 
               err.errType, err.errCause); 
            continue;
         }
      }
      else
      {
            DU_LOG("\nERROR  -->  SCH : RNTI:%d Unable to get the "
                     "UE CB or RA CB ", hqInfo->rnti);
            err.errType = RGSCHERR_TOM_HARQACKIND;
            continue;
      }
   }

   /* Check with TDD call DHM*/
   if ((rgSCHDhmRlsDlsfHqProc (cell, harqAckInd->timingInfo)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Unable to Release Downlink "
         "subframe for cellId (%d) ", harqAckInd->cellId);
      err.errType = RGSCHERR_TOM_HARQACKIND;
   }

   for (cellIdx = 0; cellIdx < CM_LTE_MAX_CELLS; cellIdx++)
   {
      if (NULLP != rgSchCb[cell->instIdx].cells[cellIdx])
      {
         iterCellP = rgSchCb[cell->instIdx].cells[cellIdx];

         rlsHqBufs = &(iterCellP->rlsHqArr[iterCellP->crntHqIdx]);
         if(rlsHqBufs->numUes)
         {
            rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], iterCellP->macInst);
            RgSchMacRlsHq (&pst, rlsHqBufs);
         }
         rlsHqBufs->numUes = 0;
      }
   }
#endif 
   return ROK;
}  /* rgSCHTomHarqAckInd */


/**
 * @brief Handler for processing Scheduling Request indication 
 * recieved from PHY for a list of UEs.
 *
 * @details
 *
 *     Function: rgSCHTomSrInd
 *
 *     Handler for processing Scheduling Request indication recieved from PHY
 *     for UEs.
 *
 *     Invoked by: RgLiTfuSrInd
 *
 *     Processing Steps:
 *     - Get UE
 *     - Invoke scheduler to indicate SR rgSCHUtlSrRcvd 
 *           
 *  @param[in]  TfuSrIndInfo *srInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHTomSrInd
(
RgSchCellCb     *cell,
TfuSrIndInfo    *srInd
)
{
   S16          ret = RFAILED;
   RgSchErrInfo err;
   RgSchUeCb    *ue;
   CmLList      *node;
   TfuSrInfo    *srInfo;

   if(cell->cellId != srInd->cellId)
   {
      DU_LOG("\nERROR  -->  SCH : Unable to get the cell for srcInd cellId"
         ":%d ", srInd->cellId);
      err.errType = RGSCHERR_TOM_SRIND;
      err.errCause = RGSCHERR_TOM_INV_CELL_ID;
      return RFAILED;
	}


   node =  srInd->srLst.first;
   for (;node; node=node->next)
   {
      rgNumSrRecvd++;
	  
      srInfo = (TfuSrInfo *)node->node;
      ue = rgSCHDbmGetUeCb (cell, srInfo->rnti);
      if (ue == NULLP)
      {
         DU_LOG("\nERROR  -->  SCH : RNTI:%d Unable to get the UE CB",
            srInfo->rnti);
         continue;
      }
      rgSCHUtlHdlUlTransInd(cell, ue, srInd->timingInfo);
      /*Need to activate UE as SR received*/
      if (ue->isDrxEnabled)
      {
         rgSCHDrxSrInd(cell, ue);
      }
      ret = rgSCHUtlSrRcvd (cell, ue, srInd->timingInfo, &err);
      if (ret != ROK)
      {
         err.errType = RGSCHERR_TOM_SRIND;
         DU_LOG("\nERROR  -->  SCH : Scheduler processing failed "
             "errType(%d) errCause(%d) RNTI:%d", err.errType, err.errCause,srInfo->rnti);
         continue;
      }
   }
   return ROK;
}  /* end of rgSCHTomSrInd */

/**
 * @brief Handler for processing downlink CQI indication recieved from 
 * PHY for a UE.
 *
 * @details
 *
 *     Function:  rgSCHTomDoaInd  
 *
 *     Handler for processing DOA recieved from PHY
 *     for a set of UEs.
 *
 *     Invoked by: RgLiTfuDoaInd
 *
 *     Processing Steps:
 *     - Get UE
 *     - Invoke scheduler to indicate DOA rgSCHUtlDoaInd
 *           
 *  @param[in]  TfuDoaIndInfo *doaInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHTomDoaInd
(
RgSchCellCb     *cell,
TfuDoaIndInfo   *doaInd
)
{
   RgSchUeCb    *ue;
   CmLList      *node;
   TfuDoaRpt    *doaInfo;

   if(cell->cellId != doaInd->cellId)
   {
      DU_LOG("\nERROR  -->  SCH : Unable to get the cell for doaInd cellId"
         ":%d", doaInd->cellId);
      return RFAILED;
	}


   node =  doaInd->doaRpt.first;
   for (;node; node=node->next)
   {
      doaInfo = (TfuDoaRpt *)node->node;
      ue = rgSCHDbmGetUeCb (cell, doaInfo->rnti);
      if (ue == NULLP)
      {
         DU_LOG("\nERROR  -->  SCH : RNTI:%d Unable to get the UE CB",
            doaInfo->rnti);
         continue;
      }
      rgSCHUtlDoaInd(cell, ue, doaInfo);
   }
   return ROK;
}  /* rgSCHTomDoaInd */
/**
 * @brief Handler for processing downlink CQI indication recieved from 
 * PHY for a UE.
 *
 * @details
 *
 *     Function: rgSCHTomDlCqiInd
 *
 *     Handler for processing downlink CQI indication recieved from PHY
 *     for a set of UEs.
 *
 *     Invoked by: RgLiTfuDlCqiInd
 *
 *     Processing Steps:
 *     - Get UE
 *     - Invoke scheduler to indicate DL CQI rgSCHUtlDlCqiInd
 *           
 *  @param[in]  TfuDlCqiIndInfo *dlCqiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHTomDlCqiInd
(
RgSchCellCb     *cell,
TfuDlCqiIndInfo *dlCqiInd
)
{
   RgSchUeCb    *ue;
   CmLList      *node;
   TfuDlCqiRpt  *dlCqiInfo;

   if(cell->cellId != dlCqiInd->cellId)
   {
      DU_LOG("\nERROR  -->  SCH : Unable to get the cell for cellId"
         ":%d", dlCqiInd->cellId);
      return RFAILED;
	}


   node =  dlCqiInd->dlCqiRptsLst.first;
   for (;node; node=node->next)
   {
      dlCqiInfo = (TfuDlCqiRpt *)node->node;
      ue = rgSCHDbmGetUeCb (cell, dlCqiInfo->rnti);
      if (ue == NULLP)
      {
         DU_LOG("\nERROR  -->  SCH : RNTI:%dUnable to get the UE CB",
            dlCqiInfo->rnti);
         continue;
      }
      rgSCHUtlDlCqiInd(cell, ue, dlCqiInfo, dlCqiInd->timingInfo);
      rgSCHUtlHdlUlTransInd(cell, ue, dlCqiInd->timingInfo);
   }
   return ROK;
}  /* rgSCHTomDlCqiInd */

/**
 * @brief Handler for moving PCQI instance for the next periodic occasion
 *
 * @details
 *
 *     Function: rgSCHTomUtlMovePcqiNxtOccasion
 *
 *     Handler for moving PCQI instance for the next periodic occasion
 *
 *     Invoked by: rgSCHTomUtlFill*
 *
 *     Processing Steps:
 *     - For a UE move its occurence instance to next occasion 
 *        depending on its periodicity 
 *     - Remove it from the current list and insert it to the list 
 *        having the index matching with the derived number. 
 *           
 *  @param[in]  RgSchCellCb     *cell,
 *               [in]        RgSchUeCb      *ue
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgSCHTomUtlMovePcqiNxtOccasion
(
RgSchCellCb     *cell,
RgSchUeCb      *ue,
RgSchUePCqiCb   *cqiCb
)
{
   uint16_t   cqiIdx = 0;

   CmLteTimingInfo timingInfo;

   if(cqiCb->cqiCfg.cqiSetup.cqiRepType == RGR_UE_PCQI_SB_REP)
   {
#ifdef xLTE_TDD
      RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, timingInfo, TFU_DELTA);
#else
      RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, timingInfo,
            TFU_RECPREQ_DLDELTA);
#endif
      RG_SCH_ADD_TO_CRNT_TIME(timingInfo,timingInfo,cqiCb->cqiPeri);
      rgSCHTomUtlPcqiSbCalcBpIdx(timingInfo,ue,cqiCb); 
   }
   /* Compute Next Transmission Instance */ 
   cqiIdx = cqiCb->cqiPeri + cqiCb->nCqiTrIdx;
   cqiIdx = cqiIdx%RG_SCH_PCQI_SRS_SR_TRINS_SIZE;  
   /* Delete from current List and move to new list */ 
   if (NULLP == cmLListDelFrm(&cell->pCqiSrsSrLst[cqiCb->nCqiTrIdx].cqiLst,
            &cqiCb->cqiLstEnt))
   {
      DU_LOG("\nERROR  -->  SCH : RNTI:%d Unable to remove node",
         ue->ueId);
   }
   cqiCb->nCqiTrIdx = cqiIdx;
   cmLListAdd2Tail(&(cell->pCqiSrsSrLst[cqiCb->nCqiTrIdx].cqiLst), 
         &(cqiCb->cqiLstEnt));
#ifdef LTE_ADV
   rgSCHUtlSCellHndlCqiCollsn(cqiCb);
#endif

   return ROK;
}  /* rgSCHTomUtlMovePcqiNxtOccasion */

/**
 * @brief Handler for moving RI instance for the next periodic occasion
 *
 * @details
 *
 *     Function: rgSCHTomUtlMovePriNxtOccasion
 *
 *     Handler for moving PCQI instance for the next periodic occasion
 *
 *     Invoked by: rgSCHTomUtlFill*
 *
 *     Processing Steps:
 *     - For a UE move its occurence instance to next occasion 
 *        depending on its periodicity 
 *     - Remove it from the current list and insert it to the list 
 *        having the index matching with the derived number. 
 *           
 *  @param[in]  RgSchCellCb     *cell,
 *               [in]        RgSchUeCb      *ue
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgSCHTomUtlMovePriNxtOccasion
(
RgSchCellCb    *cell,
RgSchUeCb      *ue, 
RgSchUePCqiCb  *riCb
)
{
   uint16_t   riIdx;
   uint16_t   riDist=0;
   uint16_t   effPeriodicity;
   uint16_t   riTrInsTime;  
   uint16_t   crntTime;
   uint16_t   tempIdx; 

   crntTime = (cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G)
      +(cell->crntTime.slot);
#ifdef XEON_SPECIFIC_CHANGES
   RGSCHCPYTIMEINFO(cell->crntTime, ue->riRecpTime);
#endif
   /* Compute Next Transmission Instance */ 
   if (riCb->cqiCfg.cqiSetup.cqiRepType == RGR_UE_PCQI_WB_REP)
   {
      effPeriodicity = riCb->cqiPeri * riCb->riPeri;
      tempIdx = effPeriodicity + riCb->nRiTrIdx;
   }
   else 
   {
      effPeriodicity = (riCb->h * riCb->cqiPeri * riCb->riPeri);
      /* In case of SFN wraparound, the SB CQI reporting cycle breaks
       * and RI->WB CQI->SBCQI.. should resume. RI is repositioned 
       * accordingly. WBCQI handling is naturally accomplished */
      if ((crntTime + TFU_RECPREQ_DLDELTA + effPeriodicity) > 
          (RGSCH_MAX_SUBFRM_5G - 1))
      {
         riTrInsTime = (effPeriodicity - riCb->cqiOffset + riCb->riOffset) % effPeriodicity; 
         tempIdx = RGSCH_MAX_SUBFRM_5G + (effPeriodicity - riTrInsTime);
         /*  In case of SFN wraparound, riDist should be distance from crntTime
          *  + TFU_RECPREQ_DLDELTA to tempIdx. Updating effPeriodicity 
          *  to make riDist calculation consistent for both SFN wraparound 
          *  case and normal case */
         effPeriodicity = tempIdx - TFU_RECPREQ_DLDELTA - crntTime;
      }
      else
      {
         tempIdx = effPeriodicity + riCb->nRiTrIdx;
      }
   }
   riIdx = tempIdx % RG_SCH_PCQI_SRS_SR_TRINS_SIZE;
   if (effPeriodicity >= RG_SCH_PCQI_SRS_SR_TRINS_SIZE)
   {
      riDist = rgSCHUtlFindDist((uint16_t)(crntTime + TFU_RECPREQ_DLDELTA), 
      (uint16_t)(crntTime + TFU_RECPREQ_DLDELTA + effPeriodicity));  
   }
   else
   {
      riDist = 0; 
   }

   /* ccpu00138306- If Periodicity is equal to Queue Size or multiple of it 
    * then the next occasion idx will be same as current Idx, Hence need not 
    * to delete and add 
    */
   if((effPeriodicity%RG_SCH_PCQI_SRS_SR_TRINS_SIZE) != 0)
   {
      /* Delete from current List and move to new list */ 
      if (NULLP == cmLListDelFrm(&cell->pCqiSrsSrLst[riCb->nRiTrIdx].riLst,
            &riCb->riLstEnt))
      {
         DU_LOG("\nERROR  -->  SCH : [%d]UEID:Unable to remove node",
            ue->ueId);
      }
      RG_SCH_RECORD(&riCb->histElem,RGSCH_ACTION_DEL, &cell->pCqiSrsSrLst[riCb->nRiTrIdx].riLst);
      cmLListAdd2Tail(&cell->pCqiSrsSrLst[riIdx].riLst, 
            &riCb->riLstEnt);
      RG_SCH_RECORD(&riCb->histElem,RGSCH_ACTION_ADD, &cell->pCqiSrsSrLst[riIdx].riLst);
   }
   else
   {
      if(riDist > 0) 
      {   
         riDist--;
      }
   }
   riCb->nRiTrIdx = riIdx;
   riCb->riDist = riDist;

#ifdef LTE_ADV
   rgSCHUtlSCellHndlRiCollsn(riCb);
#endif
   return ROK;
}  /* rgSCHTomUtlMovePriNxtOccasion */

/**
 * @brief Handler for moving SR instance for the next periodic occasion
 *
 * @details
 *
 *     Function: rgSCHTomUtlMoveSrNxtOccasion
 *
 *     Handler for moving SR instance for the next periodic occasion
 *
 *     Invoked by: rgSCHTomUtlFill*
 *
 *     Processing Steps:
 *     - For a UE move its occurence instance to next occasion 
 *        depending on its periodicity 
 *     - Remove it from the current list and insert it to the list 
 *        having the index matching with the derived number. 
 *           
 *  @param[in]  RgSchCellCb     *cell,
 *               [in]        RgSchUeCb      *ue
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgSCHTomUtlMoveSrNxtOccasion
(
RgSchCellCb     *cell,
RgSchUeCb      *ue
)
{
   uint16_t   srIdx = 0;

   /* Compute Next Transmission Instance */ 
   srIdx = ue->srCb.peri + ue->srCb.nSrTrIdx;
   srIdx = srIdx%RG_SCH_PCQI_SRS_SR_TRINS_SIZE;  
   /* Delete from current List and move to new list */ 
   if (NULLP == cmLListDelFrm(&cell->pCqiSrsSrLst[ue->srCb.nSrTrIdx].srLst,
            &ue->srCb.srLstEnt))
   {
      DU_LOG("\nERROR  -->  SCH : RNTI:%d Unable to remove node",
         ue->ueId);
   }
   ue->srCb.nSrTrIdx = srIdx;
   cmLListAdd2Tail(&cell->pCqiSrsSrLst[ue->srCb.nSrTrIdx].srLst, 
         &ue->srCb.srLstEnt);

   return ROK;
}  /* rgSCHTomUtlMoveSrNxtOccasion */

/**
 * @brief Handler for moving SRS instance for the next periodic occasion
 *
 * @details
 *
 *     Function: rgSCHTomUtlMoveSrsNxtOccasion
 *
 *     Handler for moving SRS instance for the next periodic occasion
 *
 *     Invoked by: rgSCHTomUtlFill*
 *
 *     Processing Steps:
 *     - For a UE move its occurence instance to next occasion 
 *        depending on its periodicity 
 *     - Remove it from the current list and insert it to the list 
 *        having the index matching with the derived number. 
 *           
 *  @param[in]  RgSchCellCb     *cell,
 *               [in]        RgSchUeCb      *ue
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgSCHTomUtlMoveSrsNxtOccasion
(
RgSchCellCb     *cell,
RgSchUeCb      *ue
)
{
   uint16_t   srsIdx;
   uint16_t   srsDist;
   uint16_t   tempIdx;
   uint16_t   crntTime; 


   crntTime = (cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G)
      +(cell->crntTime.slot);

   /* Compute Next Transmission Instance */ 
   tempIdx = ue->srsCb.peri + ue->srsCb.nSrsTrIdx;
   srsIdx = tempIdx %RG_SCH_PCQI_SRS_SR_TRINS_SIZE;  
   if (ue->srsCb.peri > RG_SCH_PCQI_SRS_SR_TRINS_SIZE)
   {
      srsDist = rgSCHUtlFindDist((uint16_t)(crntTime + TFU_RECPREQ_DLDELTA), 
      (uint16_t)(crntTime + TFU_RECPREQ_DLDELTA + ue->srsCb.peri));  
   }
   else
   {
      srsDist =0; 
   }

   /* ccpu00138306- If Periodicity is equal to Queue Size or multiple of it 
    * then the next occasion idx will be same as current Idx, Hence need not 
    * to delete and add 
    */
   if((ue->srsCb.peri%RG_SCH_PCQI_SRS_SR_TRINS_SIZE) != 0)
   {
      /* Delete from current List and move to new list */ 
      if (NULLP == cmLListDelFrm(&cell->pCqiSrsSrLst[ue->srsCb.nSrsTrIdx].srsLst,
            &ue->srsCb.srsLstEnt))
      {
         DU_LOG("\nERROR  -->  SCH : RNTI:%d Unable to remove node",
            ue->ueId);
      }
      cmLListAdd2Tail(&cell->pCqiSrsSrLst[srsIdx].srsLst,
            &ue->srsCb.srsLstEnt);
   }
   else
   {
      if(srsDist > 0)
      {
         srsDist--; 
      }   
   }   
   ue->srsCb.nSrsTrIdx = srsIdx;
   ue->srsCb.srsDist = srsDist; 
   return ROK;
}  /* rgSCHTomUtlMoveSrsNxtOccasion */


/**
 * @brief Handler for processing RAW CQI indication recieved from 
 * PHY for a UE.
 *
 * @details
 *
 *     Function: rgSCHTomRawCqiInd
 *
 *     Handler for processing RAW CQI indication recieved from PHY
 *     for a set of UEs.
 *
 *     Invoked by: RgLiTfuRawCqiInd
 *
 *     Processing Steps:
 *     - Get UE
 *     - Invoke scheduler to indicate Raw CQI rgSCHUtlRawCqiInd
 *           
 *  @param[in]  TfuRawCqiIndInfo *rawCqiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHTomRawCqiInd
(
RgSchCellCb     *cell,
TfuRawCqiIndInfo *rawCqiInd
)
{
   RgSchUeCb    *ue;
   CmLList      *node;
   TfuRawCqiRpt* rawCqiInfo;

   RgSchErrInfo err;
   uint32_t     cellIdx;
   RgInfRlsHqInfo  *rlsHqBufs = &(cell->rlsHqArr[cell->crntHqIdx]);
   RgSchCellCb     *iterCellP;
#if DL_LA
   RgSchCmnDlUe    *ueDl;
#endif
   uint8_t      cqi;
   uint8_t      ri;
   uint8_t      hqAck;
   Pst          pst;
   RgSchRaCb    *raCb;
   TfuHqInfo    hqInfo;

   if(cell->cellId != rawCqiInd->cellId)
   {
      DU_LOG("\nERROR  -->  SCH : Unable to get the cell for cellId"
            ":%d", rawCqiInd->cellId);
      return RFAILED;
	}


   node =  rawCqiInd->rawCqiRpt.first;
   for (;node; node=node->next)
   {
      rawCqiInfo = (TfuRawCqiRpt *)node->node;
      ue = rgSCHDbmGetUeCb (cell, rawCqiInfo->crnti);
      raCb = rgSCHDbmGetRaCb (cell, rawCqiInfo->crnti);
      /*
      if (ue == NULLP)
      {
         DU_LOG("\nERROR  -->  SCH : CRNTI:%d Unable to get the UECB",
               rawCqiInfo->crnti);
         continue;
      }
      */
#ifdef RG_5GTF
      /*
      if (rawCqiInfo->numBits >= 5)
         DU_LOG("\nINFO  -->  SCH : cellId [%d] crnti [%d] numBits [%d]  uciPayload [0x%08x] sfn/sf [%d:%d]\n", 
                cell->cellId, rawCqiInfo->crnti, rawCqiInfo->numBits, rawCqiInfo->uciPayload, 
                rawCqiInd->timingInfo.sfn, rawCqiInd->timingInfo.slot);
      */
      if (rawCqiInfo->numBits == 1)
      {
         rlsHqBufs = &(cell->rlsHqArr[cell->crntHqIdx]);
         uint8_t fdbk = TFU_HQFDB_NACK;
         /* Process HARQ FdbkInd */
         hqAck = (rawCqiInfo->uciPayload >> 31) & 0x1;
         if (hqAck)
         {
            fdbk = TFU_HQFDB_ACK;
            hqInfo.isAck[0] = fdbk;
         }
         if (ue != NULLP && raCb == NULLP)
         {  
            if ((rgSCHDhm5gtfHqFdbkInd (ue, cell, rawCqiInd->timingInfo, fdbk, &err)) != ROK)
            {
               err.errType = RGSCHERR_TOM_HARQACKIND;
               DU_LOG("\nERROR  -->  SCH : rgSCHTomHarqAckInd() "
                     "HARQ feedback processing failed errType(%d)errCause(%d)n",
                     err.errType, err.errCause); 
               continue;
            }
         }
         else if (ue == NULLP && raCb != NULLP)
         {
            if ((rgSCHDhmHqFdbkInd (raCb, RGSCH_HQ_FDB_IND_CB_TYPE_RA_CB, 
                        cell, rawCqiInd->timingInfo, &hqInfo, rlsHqBufs, &err)) != ROK)
            {
               err.errType = RGSCHERR_TOM_HARQACKIND;
               DU_LOG("\nERROR  -->  SCH : rgSCHTomHarqAckInd() HARQ"
                     " feedback processing failed errType(%d) errCause(%d)", 
                     err.errType, err.errCause); 
               continue;
            }
            continue;
         }
         else if (ue != NULLP && raCb != NULLP)
         {
            if ((rgSCHDhmHqFdbkInd (raCb, RGSCH_HQ_FDB_IND_CB_TYPE_RA_CB, 
                        cell, rawCqiInd->timingInfo, &hqInfo, rlsHqBufs, &err)) != ROK)
            {
               err.errType = RGSCHERR_TOM_HARQACKIND;
               DU_LOG("\nERROR  -->  SCH : rgSCHTomHarqAckInd() HARQ"
                     " feedback processing failed errType(%d) errCause(%d).", 
                     err.errType, err.errCause); 
               continue;
            }
         }
         else
         {
            DU_LOG("\nERROR  -->  SCH : RNTI:%d Unable to get the "
                  "UE CB or RA CB ", rawCqiInfo->crnti);
            err.errType = RGSCHERR_TOM_HARQACKIND;
            continue;
         }
         /*
         DU_LOG("rawCqiInfo->numBits [%d]  uciPayload [0x%08x] sfn/sf [%d:%d]\n", rawCqiInfo->numBits,
                rawCqiInfo->uciPayload, rawCqiInd->timingInfo.sfn, rawCqiInd->timingInfo.slot);
         */
      }
      else if (rawCqiInfo->numBits == 5)
      {
         /* Process CQI-RI Ind*/
         ri = (rawCqiInfo->uciPayload >> 27) & 0x1;
         cqi = (rawCqiInfo->uciPayload >> 28) & 0xF;
         if(ue) {
         if (cqi == 0)
         {
           DU_LOG("\nERROR  -->  SCH : UE[%d] CQI[%d] Invalid\n", ue->ueId, cqi);
            cqi = 15;
         }
         ue->ue5gtfCb.mcs = rgSch5gtfCqi2Mcs[cqi - 1];
         ue->ue5gtfCb.rank = ri + 1;
#ifdef DL_LA
         if (rawCqiInfo->numBits > 1)
         {
               ueDl =  RG_SCH_CMN_GET_DL_UE(ue,cell);
               ueDl->mimoInfo.cwInfo[0].cqi = cqi;
               ueDl->cqiFlag = TRUE;
               rgSCHCmnDlSetUeAllocLmtLa(cell, ue);
            // rgSCHCheckAndSetTxScheme(cell, ue);
         }
#endif
         }
         /*
         DU_LOG("\nERROR  -->  SCH : UE[%d] CQI[%d] MCS[%d] RI[%d]\n", ue->ueId, cqi, ue->ue5gtfCb.mcs, ri);
         */
      }
      else if (rawCqiInfo->numBits == 6)
      {
         rlsHqBufs = &(cell->rlsHqArr[cell->crntHqIdx]);
         TfuHqFdbk fdbk = TFU_HQFDB_NACK;
         /* Process both HARQ and CQI-RI Ind*/
         ri = (rawCqiInfo->uciPayload >> 26) & 0x1;
         cqi = (rawCqiInfo->uciPayload >> 27) & 0xF;
         hqAck = (rawCqiInfo->uciPayload >> 31) & 0x1;
         if (cqi == 0)
         {
            DU_LOG("\nERROR  -->  SCH : UE[%d] CQI[%d] Invalid\n", ue->ueId, cqi);
            cqi = 13;
         }
         ue->ue5gtfCb.mcs = rgSch5gtfCqi2Mcs[cqi - 1];
         ue->ue5gtfCb.rank = ri + 1;
#ifdef DL_LA
         if (rawCqiInfo->numBits > 1)
         {
               ueDl =  RG_SCH_CMN_GET_DL_UE(ue,cell);
               ueDl->mimoInfo.cwInfo[0].cqi = cqi;
               ueDl->cqiFlag = TRUE;
               rgSCHCmnDlSetUeAllocLmtLa(cell, ue);
            // rgSCHCheckAndSetTxScheme(cell, ue);
         }
#endif
         if (hqAck)
         {
            fdbk = TFU_HQFDB_ACK;
            hqInfo.isAck[0] = fdbk;
         }
         if (ue != NULLP && raCb == NULLP)
         {  
             if ((rgSCHDhm5gtfHqFdbkInd (ue, cell, rawCqiInd->timingInfo, fdbk, &err)) != ROK)
             {
                 err.errType = RGSCHERR_TOM_HARQACKIND;
                 DU_LOG("\nERROR  -->  SCH : rgSCHTomHarqAckInd() "
                         "HARQ feedback processing failed errType(%d)errCause(%d)n",
                         err.errType, err.errCause); 
                 continue;
             }
         }
         else if (ue == NULLP && raCb != NULLP)
         {
            if ((rgSCHDhmHqFdbkInd (raCb, RGSCH_HQ_FDB_IND_CB_TYPE_RA_CB, 
                        cell, rawCqiInd->timingInfo, &hqInfo, rlsHqBufs, &err)) != ROK)
            {
               err.errType = RGSCHERR_TOM_HARQACKIND;
               DU_LOG("\nERROR  -->  SCH : rgSCHTomHarqAckInd() HARQ"
                     " feedback processing failed errType(%d) errCause(%d)", 
                     err.errType, err.errCause); 
               continue;
            }
            continue;
         }
         else if (ue != NULLP && raCb != NULLP)
         {
            if ((rgSCHDhmHqFdbkInd (raCb, RGSCH_HQ_FDB_IND_CB_TYPE_RA_CB, 
                        cell, rawCqiInd->timingInfo, &hqInfo, rlsHqBufs, &err)) != ROK)
            {
               err.errType = RGSCHERR_TOM_HARQACKIND;
               DU_LOG("\nERROR  -->  SCH : rgSCHTomHarqAckInd() HARQ"
                     " feedback processing failed errType(%d) errCause(%d).", 
                     err.errType, err.errCause); 
               continue;
            }
         }
         else
         {
            DU_LOG("\nERROR  -->  SCH : RNTI:%d Unable to get the "
                  "UE CB or RA CB ", rawCqiInfo->crnti);
            err.errType = RGSCHERR_TOM_HARQACKIND;
            continue;
         }

         /*
         DU_LOG("\nERROR  -->  SCH : UE[%u] CQI[%u] MCS[%u] RI[%u] HQ[%u]\n", ue->ueId, cqi, ue->ue5gtfCb.mcs, ri, hqAck);
         */
      }
   }

   if ((rgSCHDhmRlsDlsfHqProc (cell, rawCqiInd->timingInfo)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Unable to Release Downlink "
            "subframe for cellId (%d) ", cell->cellId);
      err.errType = RGSCHERR_TOM_HARQACKIND;
   }

   for (cellIdx = 0; cellIdx < CM_LTE_MAX_CELLS; cellIdx++)
   {
      if (NULLP != rgSchCb[cell->instIdx].cells[cellIdx])
      {
         iterCellP = rgSchCb[cell->instIdx].cells[cellIdx];

         rlsHqBufs = &(iterCellP->rlsHqArr[iterCellP->crntHqIdx]);
         if(rlsHqBufs->numUes)
         {
            rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], iterCellP->macInst);
            RgSchMacRlsHq (&pst, rlsHqBufs);
         }
         rlsHqBufs->numUes = 0;
      }
   }
   return ROK;
}  /* rgSCHTomRawCqiInd */

/**
 * @brief Handler for processing SRS indication recieved from 
 * PHY for a UE.
 *
 * @details
 *
 *     Function: rgSCHTomSrsInd
 *
 *     Handler for SRS indication recieved from PHY
 *     for a set of UEs.
 *
 *     Invoked by: RgLiTfuSrsInd
 *
 *     Processing Steps:
 *     - Get UE
 *     - Invoke scheduler to indicate UL SRS  rgSCHUtlSrsInd
 *           
 *  @param[in]  TfuSrsIndInfo *srsInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHTomSrsInd
(
RgSchCellCb     *cell,
TfuSrsIndInfo *srsInd
)
{
   RgSchUeCb    *ue;
   CmLList      *node;
   TfuSrsRpt* srsInfo;

   if(cell->cellId != srsInd->cellId)
   {
      DU_LOG("\nERROR  -->  SCH : Unable to get the cell for cellId"
         ":%d", srsInd->cellId);
      return RFAILED;
	}

   node =  srsInd->srsRpt.first;
   for (;node; node=node->next)
   {
      srsInfo = (TfuSrsRpt *)node->node;
      ue = rgSCHDbmGetUeCb (cell, srsInfo->ueId);
      if (ue == NULLP)
      {
         DU_LOG("\nERROR  -->  SCH : RNTI:%d Unable to get the "
            "UE CB", srsInfo->ueId);
         continue;
      }
      rgSCHUtlSrsInd(cell, ue, srsInfo, srsInd->timingInfo);
      rgSCHUtlHdlUlTransInd(cell, ue, srsInd->timingInfo);
   }
   return ROK;
}  /* rgSCHTomSrsInd */

/*
*
*       Fun:   rgSCHTomUtlGenIndices
*
*       Desc:  This function reconstructs the Subband Indices for
*       of M selected Subbands conveyed by the UE for APeriodic Modes
*       2-0 and 2-2. It decodes the Label which uniquely encodes M out
*       of N subbands.
*                   
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  rg_sch_utl.c
*
*/
#ifdef UNUSED_FUNC
static S16 rgSCHTomUtlGenIndices
(
uint32_t      label,
uint8_t        posM,
uint8_t        valN,
uint8_t        valK,
TfuSubbandInfo* sbInfo
)
{
   uint8_t idx, kval, xval, xmin;
   uint32_t binCoe;
   xmin =1;
   for(kval=0; kval<posM;kval++)
   {
      xval = xmin;
      RGSCH_ARRAY_BOUND_CHECK_WITH_POS_IDX(0, rgSCHTomBinCoe, (posM-kval-1));
      RGSCH_ARRAY_BOUND_CHECK_WITH_POS_IDX(0, rgSCHTomBinCoe[posM-kval-1], (valN-xval-1));
      binCoe = rgSCHTomBinCoe[posM-kval-1][valN-xval-1];
      while(binCoe>label)
      {
         xval = xval+1;
         RGSCH_ARRAY_BOUND_CHECK_WITH_POS_IDX(0, rgSCHTomBinCoe[posM-kval-1], (valN-xval-1));
         binCoe = rgSCHTomBinCoe[posM-kval-1][valN-xval-1];
      }
      idx = xval;
      sbInfo[kval].numRb = valK;
      sbInfo[kval].rbStart = idx*valK;
      xmin = idx+1;
      label = label-binCoe;
   }
   return ROK;
} /* end of rgSCHTomUtlGenIndices*/
#endif
#endif 
/**
 * @brief Handler for processing decode failure indication recieved from 
 * PHY for a UE.
 *
 * @details
 *
 *     Function: rgSCHTomCrcInd
 *
 *     Handler for processing decode failure indication recieved from 
 *     PHY for a set of UEs.
 *
 *     Invoked by: RgLiTfuCrcInd of rg_sch.x 
 *
 *     Processing Steps:
 *     - Validate the information received and retrieve cell and ue.
 *     - Process Decode failure Indication: Call rgSCHUhmProcHqFailure().
 *           
 *  @param[in]  TfuCrcIndInfo *crcInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHTomCrcInd
(
RgSchCellCb       *cell,
TfuCrcIndInfo *crcInd
)
{
   RgSchUeCb      *ue = NULLP;
   RgSchRaCb      *raCb = NULLP;
   CmLList        *node;
   TfuCrcInfo     *crcInfo;
#ifdef RG_ULSCHED_AT_CRC
   RgSchErrInfo    err;
   RgSchDlSf       *ulSf;
   CmLteTimingInfo crntHiDci0Frm;
   //RgSchCmnUlCell  *cellUl; 
   Inst             inst = cell->instIdx;
   TfuCntrlReqInfo  *cntrlInfo;
   uint32_t               ret;
#ifdef LTE_TDD
   uint8_t              Mval;
   uint8_t              idx;
#endif
#endif
#ifdef LTE_TDD
   RgSchUlHqProcCb   *hqProc;
#endif

#ifdef LTE_L2_MEAS
   RgSchUlHqProcCb   *ulHqProc;
#endif   

   if(cell->cellId != crcInd->cellId)
   {
      DU_LOG("\nERROR  -->  SCH : Unable to get the cell for cellId"
         ":%d", crcInd->cellId);
      return RFAILED;
	}
#ifdef RG_ULSCHED_AT_CRC
#ifndef LTE_ADV
   {
      static CmLteTimingInfo lastCrc = {2000,0};
      CmLteTimingInfo crntCrc = cell->crntTime;
      if (RGSCH_TIMEINFO_SAME(lastCrc, crntCrc))
      {
         /*Removed the WA to drop 2nd CRC*/
         DU_LOG("\nINFO  -->  SCH : Recieved CRC "
            "twice per TTI @(%u,%u)", cell->crntTime.sfn,
            cell->crntTime.slot);
      }
      lastCrc = crntCrc;
   }
#endif
#endif
   node =  crcInd->crcLst.first;
   for (;node; node=node->next)
   {
      crcInfo = (TfuCrcInfo*)node->node;
      ue = rgSCHDbmGetUeCb (cell, crcInfo->rnti);
      if (ue == NULLP)
      {
#ifdef LTEMAC_SPS
         /* Fetch from SPS List */
         ue = rgSCHDbmGetSpsUeCb(cell, crcInfo->rnti);
         if (ue == NULLP)         
#endif 
         {
            raCb = rgSCHDbmGetRaCb (cell, crcInfo->rnti);
            if (raCb == NULLP)
            {
               continue;
            }
         }
      }
      /* Added Ul TB count for Uplink data scheduled*/
#ifdef LTE_L2_MEAS
      if(raCb)
      {
         ulHqProc = &(raCb->msg3HqProc);
         if(ulHqProc->remTx == (cell->rachCfg.maxMsg3Tx -1))
         {
            cell->dlUlTbCnt.tbTransUlTotalCnt++;
         }   
      }  
      else
      {
         rgSCHUtlUlHqProcForUe(cell, crcInd->timingInfo, ue, &ulHqProc); 
         if(ulHqProc->remTx == ((RgUeUlHqCb*)ulHqProc->hqEnt)->maxHqRetx) 
         {
            cell->dlUlTbCnt.tbTransUlTotalCnt++;
         }   
      }   
#endif

      if (crcInfo->isFailure == FALSE)
      {
         if(raCb)
         {
            rgSCHRamMsg3DatInd(raCb);
#ifdef LTE_TDD
            /*ccpu00128820 - MOD - Msg3 alloc double delete issue*/
            hqProc = &(raCb->msg3HqProc);
            RGSCH_UPD_PHICH(cell->ulDlCfgIdx, crcInd->timingInfo.slot,
                    hqProc);
#endif
            raCb = NULLP;
         }
         else
         {
#ifdef EMTC_ENABLE
            gUlCrcPassCounter++;
#endif
#ifdef CA_DBG
            gUlCrcPassCount++;
#endif
            RGSCHCPYTIMEINFO(crcInd->timingInfo, ue->datIndTime);
#ifndef MAC_SCH_STATS
            rgSCHUhmProcDatInd(cell, ue, crcInd->timingInfo);

#else 
            /** Stats update over here 
            */
            {
               RgSchCmnUe     *cmnUe = RG_SCH_CMN_GET_UE(ue,cell);

               rgSCHUhmProcDatInd(cell, ue, crcInd->timingInfo, cmnUe->ul.crntUlCqi[0]);
            }
#endif /* MAC_SCH_STATS */

            rgSCHUtlHdlUlTransInd(cell, ue, crcInd->timingInfo);
#ifdef LTEMAC_SPS
            rgSCHUtlHdlCrcInd(cell, ue, crcInd->timingInfo);
#endif
         }
      }
      else
      {
         if(raCb)
         {
            /* SR_RACH_STATS : MSG3 Nack / DTX*/
            if (crcInfo->isDtx == TRUE)
            {
               rgNumMsg3DtxRcvd++;
            }
            else
            {
               rgNumMsg3CrcFailed++;
            }
            rgSCHRamMsg3FailureInd(raCb);
#ifdef EMTC_ENABLE
            rgSCHCmnEmtcHdlCrcFailInd(cell, raCb);
#endif
            /* Added Ul TB count for CRC Failure of MSG3 */
#ifdef LTE_L2_MEAS            
            ulHqProc = &(raCb->msg3HqProc);
            if(ulHqProc->remTx == (cell->rachCfg.maxMsg3Tx -1))
            {
               cell->dlUlTbCnt.tbTransUlFaulty++;
            }
#endif
            raCb = NULLP;
         }
         else
         {
#ifdef EMTC_ENABLE
            gUlCrcFailCounter++; 
#endif 
#ifdef CA_DBG
            gUlCrcFailCount++;
#endif
#ifndef MAC_SCH_STATS
            rgSCHUhmProcHqFailure (cell, ue, crcInd->timingInfo, crcInfo->rv);
#else
            {
               RgSchCmnUe     *cmnUe = RG_SCH_CMN_GET_UE(ue,cell);

               rgSCHUhmProcHqFailure (cell, ue, crcInd->timingInfo, crcInfo->rv, cmnUe->ul.crntUlCqi[0]);
            }
#endif /* MAC_SCH_STATS */
            rgSCHUtlHdlUlTransInd(cell, ue, crcInd->timingInfo);
#ifdef LTEMAC_SPS
            rgSCHUtlHdlCrcFailInd(cell, ue, crcInd->timingInfo);
#endif
            /* Added Ul TB count for CRC Failure of Uplink data */
#ifdef LTE_L2_MEAS            
            rgSCHUtlUlHqProcForUe(cell, crcInd->timingInfo, ue, &ulHqProc); 
            if(ulHqProc->remTx == ((RgUeUlHqCb*)ulHqProc->hqEnt)->maxHqRetx) 
            {
               cell->dlUlTbCnt.tbTransUlFaulty++;
            }   
#endif   
         }
      }
   }

/* ccpu00132653-ADD Added Sched_At_Crc Changes for TDD and optimized here 
   the codebase across TDD and FDD*/
#ifdef RG_ULSCHED_AT_CRC
   /* Changes to do uplink scheduling at CRC Indication */
   //cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, crntHiDci0Frm, TFU_ULCNTRL_DLDELTA);


   rgSCHCmnRlsUlSf(cell,0);


      /* Allocating memory for CntrlReq as it required for both EMTC and
       * Normal UEs */   
      if ((ret = rgSCHUtlAllocEventMem(inst, (Ptr *)&cntrlInfo, 
                  sizeof(TfuCntrlReqInfo))) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : Unable to Allocate TfuCntrlReqInfo "
               "for cell");
         return ret;
      }
      rgSCHCmnUlSch(cell);
#ifdef LTE_L2_MEAS
      rgSCHL2Meas(cell,TRUE);
#endif
      /* Also, sending UL DCI and PHICH for just scheduled subframe */
      ulSf = rgSCHUtlSubFrmGet (cell, crntHiDci0Frm);

      if ((rgSCHTomUtlProcDlSfAtCrc (ulSf, crntHiDci0Frm, cell, cntrlInfo, &err)) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : rgSCHTomCrcInd() Unable to process"
                  " downlink subframe for cellId %d", crcInd->cellId);
         err.errType = RGSCHERR_TOM_TTIIND;
         return RFAILED;
		}
#endif /* RG_ULSCHED_AT_CRC */
   return ROK;
}  /* rgSCHTomCrcInd */

/**
 * @brief Handler for processing timing Advance indication recieved from 
 * PHY for a UE.
 *
 * @details
 *
 *     Function: rgSCHTomTimingAdvInd
 *
 *     Handler for processing timing advance indication recieved from PHY
 *     for a set of UEs.
 *
 *     Invoked by: RgLiTfuTimingAdvInd
 *
 *     Processing Steps:
 *     - Get UE.
 *     - Call DHM to update value of Timing Advance rgSCHDhmUpdTa.
 *           
 *  @param[in] TfuTimingAdvIndInfo *timingAdvInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHTomTimingAdvInd
(
RgSchCellCb         *cell,
TfuTimingAdvIndInfo *timingAdvInd
)
{
   RgSchUeCb        *ue;
   CmLList          *node;
   TfuTimingAdvInfo *timingAdvInfo;

   if(cell->cellId != timingAdvInd->cellId)
   {
      DU_LOG("\nERROR  -->  SCH : Unable to get the cell for cellId"
         "=(%d)", timingAdvInd->cellId);
      return RFAILED;
	}


   node =  timingAdvInd->timingAdvLst.first;
   for (;node; node=node->next)
   {
      timingAdvInfo = (TfuTimingAdvInfo *)node->node;
      ue = rgSCHDbmGetUeCb (cell, timingAdvInfo->rnti);
      if (ue == NULLP)
      {
         DU_LOG("\nERROR  -->  SCH : RNTI:%d Unable to get the UE CB",
            timingAdvInfo->rnti);
         continue;
      }
      rgSCHDhmUpdTa (cell, ue, timingAdvInfo->timingAdv);
   }
   return ROK;
}  /* rgSCHTomTimingAdvInd */

/**
 * @brief Handler for processing TTI indication recieved from 
 * PHY for 'n' cells.
 *
 * @details
 *
 *     Function: rgSCHTomTtiInd
 *
 *     Handler for processing slot indication recieved from MAC
 *     for a cell. This is split into the below Steps. 
 *     
 *     1: Complete the Uplink and Common Channel Scheduling for each Cell
 *     2: Complete the UE specific Scheduling for each Cell / across Cells.
 *     3: Consolidate the subframe allocations and send to each MAC instance
 *     4: Fill the Tfu structures for DL and UL Config requests  
 *     5: Handle the RGR Config messages per Cell
 *
 *  @param[in] SlotTimingInfo    *slotInd
 *  @param[in] Inst           schInst
 *  @return  Void
 **/
Void rgSCHTomTtiInd
(
SlotTimingInfo        *slotInd,
Inst               schInst
)
{
   RgInfSfAlloc      *subfrmAlloc;
   RgTfuCntrlReqInfo *cntrlInfo = NULLP;
   S16               ret = ROK;
   uint8_t           i;
   uint8_t           nCell = 0;
   RgSchCellCb       *cell[CM_LTE_MAX_CELLS];
   RgSchCellCb       *cellLst[CM_LTE_MAX_CELLS];
   
#ifdef LTE_L2_MEAS
   glblTtiCnt++;
#endif

   //rgSchTomFillCellTtiInfo(slotInd, schInst, &nCell, &cell[0]);

   schFillCrntTime(*slotInd,schInst);
   for (i = 0; i < nCell; i++)
   {
      /* Perform UL and DL Common Channel scheduling */
      rgSchTomTtiUlAndDlCmnChSch (cell[i]);
   }

   /* Perform scheduling in Order of
    * 1. SPS
    * 2. CEs
    * 3. Retx */
   for (i = 0; i < nCell; i++)
   {
      
      if (cell[i]->isDlDataAllwd && (cell[i]->stopDlSch == FALSE))
      {
         RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell[i]);
         /* Perform DL Retx scheduling */
         cellSch->apisDl->rgSCHDlRetxSched(cell[i], &cellSch->allocInfo);      
      }
   }

   rgSchCmnPreDlSch(cell, nCell, cellLst);
   for (i = 0; i < nCell; i++)
   {
      /* Perform DL scheduling */
      rgSchTomTtiDlSch (cellLst[i]);
   }
   rgSchCmnPstDlSch(cell[0]);

   for (i = 0; i < nCell; i++)
   {
#ifdef LTE_TDD
#ifndef RG_ULSCHED_AT_CRC      
      /* Perform UL scheduling for TDD */
      rgSCHCmnUlSch (cell[i]);
#endif      
#endif
   }
   /* Init SF Alloc info per Cell */
   for (i = 0; i < nCell; i++)
   {
      subfrmAlloc = &(cell[i]->sfAllocArr[cell[i]->crntSfIdx]);
      rgSCHUtlResetSfAlloc(subfrmAlloc,FALSE,TRUE);
   }
   for (i = 0; i < nCell; i++)
   {
      if (cell[i]->isDlDataAllwd && (cell[i]->stopSiSch == FALSE))
      {
         subfrmAlloc = &(cell[i]->sfAllocArr[cell[i]->crntSfIdx]);
         /*
          * TFU_DLDATA_DLDELTA is used in this calculation because the subfrmAlloc
          * timingInfo which is being calculated here will be used by MAC
          */
         RG_SCH_ADD_TO_CRNT_TIME(cell[i]->crntTime, subfrmAlloc->timingInfo,
                                 RG_DL_DELTA - TFU_DLDATA_DLDELTA);
         /* Consolidate the Allocations and send response to MAC instances */
         rgSchTomTtiCnsldtSfAlloc (cell[i]);
      }
   }

   for (i = 0; i < nCell; i++)
   {
      if (cell[i]->isDlDataAllwd && (cell[i]->stopSiSch == FALSE))
      {
         /* Send the consolidated Alloc Info to MAC instances */
         rgSCHCmnSndCnsldtInfo (cell[i]);
      }
   }

   for (i = 0; i < nCell; i++)
   {
      /* Fill control data from scheduler to PHY */   
      if ((ret = rgSCHUtlAllocEventMem((cell[i]->instIdx), (Ptr *)&cntrlInfo, 
                  sizeof(RgTfuCntrlReqInfo))) != ROK)
      {     
         DU_LOG("\nERROR   -->  SCH : Unable to Allocate TfuCntrlReqInfo"
               " for cell");
         return;
      }

#ifdef EMTC_ENABLE
      /* Fill the TFU structures and send to CL */
         if(TRUE == cell[i]->emtcEnable)
         {
            ret = rgSchEmtcTomTtiL1DlAndUlCfg  (cell[i], cntrlInfo);
         }
#endif
      if((ROK == ret)
         && (NULLP != cntrlInfo))
      {
      /* Fill the TFU structures and send to CL */
         rgSchTomTtiL1DlAndUlCfg  (cell[i], cntrlInfo);
      }
   }
#ifdef RGR_RRM_TICK   
   rgSCHTomUtlSendSfnTick(cell[0]);
#endif

   for (i = 0; i < nCell; i++)
   {
      /* Invoke non critical functions like measurements, etc */
      rgSchTomTtiMiscFunctions (cell[i]);
   }

#ifdef CA_DBG
   {
      uint32_t dbgUeIdChngAndDatReqInClCnt = 0;
      static uint32_t gTtiCount = 0;
      gTtiCount++;

      if(gTtiCount == 3000)
      {
#ifdef XEON_SPECIFIC_CHANGES
         DU_LOG("\nINFO   -->  SCH : SChed:: (P/S)::(%u/%u) \n",
               gPrimarySchedCount,gSCellSchedCount);

         DU_LOG("\nINFO   -->  SCH :  HQFDBK :: %u\n",gHqFdbkCount);
         
         long int total;
         long int total2 ;

         total = gPCellTb1AckCount + gPCellTb1NackCount + gPCellTb1DtxCount;
         total2 = gPCellTb2AckCount + gPCellTb2NackCount + gPCellTb2DtxCount;
         
         DU_LOG("\nINFO   -->  SCH :  PCell:: TB1:: (A/N/D)::(%u/%u/%u)  TB2:: (A/N/D)::(%u/%u/%u)\n",
               gPCellTb1AckCount,gPCellTb1NackCount,gPCellTb1DtxCount,
               gPCellTb2AckCount,gPCellTb2NackCount,gPCellTb2DtxCount);
         if ((total != 0 ) && total2 != 0)
         {
            DU_LOG("\nINFO   -->  SCH :  PCell:: TB1:: (AP/NP/DP)::(%.2f/%.2f/%.2f)   TB2:: (AP/NP/DP)::(%.2f/%.2f/%.2f)\n",
                  (float)gPCellTb1AckCount/total * 100,(float)gPCellTb1NackCount/total * 100,(float)gPCellTb1DtxCount/total * 100,
                  (float)gPCellTb2AckCount/total2 *100 ,(float)gPCellTb2NackCount/total2 *100 ,(float)gPCellTb2DtxCount/total2 *2);
         }

         total = gSCellTb1AckCount + gSCellTb1NackCount + gSCellTb1DtxCount;
         total2 = gSCellTb2AckCount + gSCellTb2NackCount + gSCellTb2DtxCount;


         DU_LOG("\nINFO   -->  SCH : SCell:: TB1:: (A/N/D)::(%u/%u/%u)  TB2:: (A/N/D)::(%u/%u/%u)\n",
               gSCellTb1AckCount,gSCellTb1NackCount,gSCellTb1DtxCount,
               gSCellTb2AckCount,gSCellTb2NackCount,gSCellTb2DtxCount);
         if ((total != 0 ) && total2 != 0)
         {
            DU_LOG("\nINFO   -->  SCH : SCell:: TB1:: (AP/NP/DP)::(%.2f/%.2f/%.2f)   TB2:: (AP/NP/DP)::(%.2f/%.2f/%.2f)\n",
                  (float)gSCellTb1AckCount/total * 100,(float)gSCellTb1NackCount/total * 100,(float)gSCellTb1DtxCount/total * 100,
                  (float)gSCellTb2AckCount/total2 *100 ,(float)gSCellTb2NackCount/total2 *100 ,(float)gSCellTb2DtxCount/total2 *2);
         }


         DU_LOG("\nINFO   -->  SCH : CQI:: Recp(Pucch/Pusch):Rcvd(pcqi/rawacqireport/apcqi/AppReprt)::(%u/%u):(%u/%u/%u/%u)\n",
               gCqiRecpCount,gCqiRecpPuschCount,gCqiRcvdCount,gRawACqiCount,
               gACqiRcvdCount,gCqiReptToAppCount);
              
         DU_LOG("\nINFO   -->  SCH : (F1BCS Count/Cqi/Ri/CqiDrop/PucchDrop/PuschCqiDrop)::(%u/%u/%u/%u/%u/%u)\n",
               gF1bCsCount,gCqiReqCount,gRiReqCount,gCqiDropCount,gPucchDropCount,gPuschCqiDropCount); 

         DU_LOG("\nINFO   -->  SCH : UL::(DCI0/CrcPass/CrcFail)::(%u/%u/%u)\n"
               "gPcellZeroBoOcc:%u\t gScellZeroBoOcc:%u dbgUeIdChngAndDatReqInClCnt: %u\n"
               "DelayedDatReqInMac: %u DelayedDatReqInCl : %u gIccPktRcvrMemDropCnt :%u\n",
               gDci0Count,
               gUlCrcPassCount,
               gUlCrcFailCount,
               gPcellZeroBoOcc,
               gScellZeroBoOcc,
               dbgUeIdChngAndDatReqInClCnt,
               dbgDelayedDatReqInMac,
              gDropDatReqCnt, gIccPktRcvrMemDropCnt);
#else
         DU_LOG("\nINFO   -->  SCH : SChed:: (P/S)::(%ld/%ld) \n",
               gPrimarySchedCount,gSCellSchedCount);

         DU_LOG("\nINFO   -->  SCH :  HQFDBK :: %ld\n",gHqFdbkCount);

         
         DU_LOG("\nINFO   -->  SCH :  PCell:: TB1:: (A/N/D)::(%ld/%ld/%ld)  TB2:: (A/N/D)::(%ld/%ld/%ld)\n",
               gPCellTb1AckCount,gPCellTb1NackCount,gPCellTb1DtxCount,
               gPCellTb2AckCount,gPCellTb2NackCount,gPCellTb2DtxCount);

         DU_LOG("\nINFO   -->  SCH : SCell:: TB1:: (A/N/D)::(%ld/%ld/%ld)  TB2:: (A/N/D)::(%ld/%ld/%ld)\n",
               gSCellTb1AckCount,gSCellTb1NackCount,gSCellTb1DtxCount,
               gSCellTb2AckCount,gSCellTb2NackCount,gSCellTb2DtxCount);

         DU_LOG("\nINFO   -->  SCH : CQI:: Recp(Pucch/Pusch):Rcvd(pcqi/rawacqireport/apcqi/AppReprt)::\
	 (%ld/%ld):(%ld/%ld/%ld/%ld)\n",
               gCqiRecpCount,gCqiRecpPuschCount,gCqiRcvdCount,gRawACqiCount,
               gACqiRcvdCount,gCqiReptToAppCount);
         DU_LOG("\nINFO   -->  SCH : CQI:: PucchCqiSnrDropCnt/PucchCqiConfBitMaskDropCnt/PuschCqiConfMaskDropCount\
	 :: (%ld/%ld/%ld) \n",gCqiPucchLowSnrDropCount,gCqiPucchConfMaskDropCount,gCqiPuschConfMaskDropCount);
              
         DU_LOG("\nINFO   -->  SCH : (F1BCS Count/Cqi/Ri/CqiDrop/PucchDrop/PuschCqiDrop)::(%ld/%ld/%ld/%ld/%ld/%ld)\n",
               gF1bCsCount,gCqiReqCount,gRiReqCount,gCqiDropCount,gPucchDropCount,gPuschCqiDropCount); 

         DU_LOG("\nINFO   -->  SCH : UL::(DCI0/CrcPass/CrcFail)::(%ld/%ld/%ld)\n"
               "gPcellZeroBoOcc:%ld\t gScellZeroBoOcc:%ld dbgUeIdChngAndDatReqInClCnt: %ld\n"
               "DelayedDatReqInMac: %ld DelayedDatReqInCl : %ld gIccPktRcvrMemDropCnt :%ld\n",
               gDci0Count,
               gUlCrcPassCount,
               gUlCrcFailCount,
               gPcellZeroBoOcc,
               gScellZeroBoOcc,
               dbgUeIdChngAndDatReqInClCnt,
               dbgDelayedDatReqInMac,
              gDropDatReqCnt, gIccPktRcvrMemDropCnt);
         //DU_LOG("\nINFO   -->  SCH : delayedApiCnt:%ld",delayedApiCnt);
#endif

        /*LAA STATS*/               
        rgSCHLaaPrintStats();

         gCaDbgNonCaFrmt = gIccPktRcvrMemDropCnt = 0;

         gCaDbgCaFrmt = 0;
         
         gF1bCsCount = 0;
         gCqiReqCount = 0;
         gACqiRcvdCount = 0;
         gRawACqiCount= 0;
         gRiReqCount = 0;
         gCqiDropCount = 0;
         gPucchDropCount= 0;

         gCqiPucchLowSnrDropCount     = 0;
         gCqiPucchConfMaskDropCount   = 0;
         gCqiPuschConfMaskDropCount   = 0;
         gPuschCqiDropCount = 0;

         gDci0Count = 0;
         gUlCrcPassCount = 0;
         gUlCrcFailCount = 0;
         
         gCqiRecpCount = 0;
         gCqiRecpPuschCount = 0;
         gCqiRcvdCount = 0;
         
         gCqiReptToAppCount = 0;
      
         gTtiCount = 0;

         gHqFdbkCount       = 0;
         gPrimarySchedCount = 0;
         gSCellSchedCount   = 0;
         gSCellTb1AckCount  = 0;
         gSCellTb2AckCount  = 0;
         gSCellTb2AckCount  = 0;
         gSCellTb2NackCount = 0;
         gPCellTb1AckCount  = 0;
         gPCellTb1NackCount  = 0;
         gPCellTb2AckCount  = 0;
         gPCellTb2NackCount  = 0;
         gSCellTb1NackCount=0;

         gPCellTb1DtxCount  = 0;
         gPCellTb2DtxCount  = 0;
         gSCellTb1DtxCount  = 0;
         gSCellTb2DtxCount  = 0;
         gPcellZeroBoOcc    = 0;
         gScellZeroBoOcc    = 0;

      }
   
   }

#endif
   return;
}  /* rgSCHTomTtiInd */

/** @brief This function does the TTI processin for the uplink subframe,
 * already populated by the scheduler.
 *
 * @details
 *
 *     Function: rgSCHTomUtlProcUlSf
 *
 *         Processing steps:
 *         - Loop through the Uplink allocations present in the uplink subframe.
 *         - For each allocation Fill a data reception request to be sent to PHY
 *         - Also fills the harq reception requests for the expected HQ feedbacks.
 *
 *
 * @param  [in]  RgSchCellCb   *cell
 * @param  [out] RgSchErrInfo  *err
 * @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */

static S16 rgSCHTomUtlProcUlSf 
(
RgSchCellCb        *cell,
RgSchErrInfo       *err
)
{
   S16             ret;
   TfuRecpReqInfo  *recpReqInfo;
#ifdef TFU_UPGRADE
   uint16_t             validIdx = 0; /* Index computed from recreq's timing info*/
#endif 
   Inst            inst = cell->instIdx;

   if ((ret = rgSCHUtlAllocEventMem(inst, (Ptr *)&recpReqInfo, 
                            sizeof(TfuRecpReqInfo))) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Unable to Allocate TfuRecpReqInfo "
         "for cell");
      err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
      return ret;
   }    
   recpReqInfo->cellId = cell->cellId;
   cmLListInit(&recpReqInfo->ueRecpReqLst);

   RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, recpReqInfo->timingInfo,
                      TFU_RECPREQ_DLDELTA);

   /* Filling data Reception requests */
   ret = rgSCHTomUtlFillDatRecpReq(recpReqInfo, cell, 
         validIdx,
         err);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Unable to fill Data recption "
         "requests for cell");
      RGSCH_FREE_MEM(recpReqInfo);
      return ret;
   } 
   /* Filling HARQ Reception requests */
   ret = rgSCHTomUtlFillHqFdbkRecpReq (recpReqInfo, cell, validIdx,err);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Unable to fill Harq Feedback "
         "reception requests for cell");
      RGSCH_FREE_MEM(recpReqInfo);
      return ret;
   }
   /* sending the RecpReq to Phy */
   //if (rgSCHUtlTfuRecpReq(inst, cell->tfuSap->sapCfg.suId, recpReqInfo) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Unable to send Cntrl info for cell");
   }
   return ROK;
} /* end of rgSCHTomUtlProcUlSf */ 

#ifdef LTE_TDD
#ifdef TFU_UPGRADE
/** @brief This function does the TTI processin for the uplink subframe,
 * already populated by the scheduler.
 *
 * @details
 *
 *     Function: rgSCHTomUtlPrcUlTddSpclSf
 *
 *         Processing steps:
 *         - Fill the SRS Info for the Special Subframe in Reception Req.
 *         - Send the Reception Req to TFU 
 *
 *
 * @param  [in]  RgSchCellCb   *cell
 * @param  [out] RgSchErrInfo  *err
 * @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
static S16 rgSCHTomUtlPrcUlTddSpclSf 
(
RgSchCellCb  *cell,
RgSchErrInfo *err
)
{
   S16             ret;
   TfuRecpReqInfo  *recpReqInfo;
   uint16_t        validIdx; /* Index computed from recreq's timing info*/
   Inst            inst = cell->instIdx;

   if ((ret = rgSCHUtlAllocEventMem(inst, (Ptr *)&recpReqInfo, 
                            sizeof(TfuRecpReqInfo))) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : rgSCHTomUtlPrcUlTddSpclSf() Unable to "
               "Allocate TfuRecpReqInfo for cell");
      err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
      return ret;
   }    
   recpReqInfo->cellId = cell->cellId;
   cmLListInit(&recpReqInfo->ueRecpReqLst);

   RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, recpReqInfo->timingInfo, TFU_RECPREQ_DLDELTA);

   RG_SCH_GET_IDX_PCQISRSSR(recpReqInfo->timingInfo, validIdx);

   /*ccpu00130768  */ 
   if(cell->srsCfg.isSrsCfgPres && 
      rgSchTddCellSpSrsSubfrmTbl[cell->srsCfg.srsSubFrameCfg][recpReqInfo->timingInfo.slot])
   {
      recpReqInfo->srsPres = TRUE;
   }
   else
   {
      recpReqInfo->srsPres = FALSE;
   }  

   /* Filling SRS Reception requests */
   ret = rgSCHTomUtlFillSrsRecpReq (recpReqInfo, cell, validIdx, err);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : rgSCHTomUtlPrcUlTddSpclSf() Unable to fill"
            " SRS recption requests for cell");
      RGSCH_FREE_MEM(recpReqInfo);
      return ret;
   }
   /* sending the RecpReq to Phy */
   //if (rgSCHUtlTfuRecpReq(inst, cell->tfuSap->sapCfg.suId, recpReqInfo) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : rgSCHTomUtlPrcUlTddSpclSf() Unable to send "
                               "Cntrl info for cell");
   }
   return ROK;
} /* end of rgSCHTomUtlPrcUlTddSpclSf */ 
#endif
#endif
/** @brief This function does all the processing related to a single downlink
 * subframe.
 *
 * @details 
 *
 *     Function: rgSCHTomUtlProcDlSf
 *
 *         Processing steps:
 *         - collate control data for all UEs and send to PHY 
 *         - collate data buffers for all UEs and send to PHY 
 *
 * @param  [in] RgSchDlSf     *dlSf
 * @param  [in] RgSchDlSf     *ulSf
 * @param  [in] RgSchCellCb   *cell
 * @param  [out] RgSchErrInfo *err
 * @return S16
 */
static S16 rgSCHTomUtlProcDlSf
(
RgSchDlSf            *dlSf,
RgSchDlSf            *ulSf,
RgSchCellCb          *cell,
RgTfuCntrlReqInfo    *cntrlInfo,
RgSchErrInfo         *err
)
{
   S16     ret;
   uint8_t sfTyp = 1; /* Dl Subframe */

      cmLListInit(&cntrlInfo->phichLst);
      cmLListInit(&cntrlInfo->dlPdcchLst);
      cmLListInit(&cntrlInfo->ulPdcchLst);
      
#ifdef TFU_ALLOC_EVENT_NO_INIT
      cntrlInfo->ulTiming.sfn = cntrlInfo->ulTiming.subframe = 0;
#endif
      cntrlInfo->dlTiming = cell->dlDciTime;
      cntrlInfo->cellId   = cell->cellId;
      cntrlInfo->ulTiming = cell->hiDci0Time;
      if((0 == (cntrlInfo->dlTiming.sfn % 30)) && (0 == cntrlInfo->dlTiming.slot))
      {
	 //DU_LOG("\nERROR  -->  SCH : 5GTF_CHECK rgSCHTomUtlProcDlSf Cntrl dl (%d : %d) ul (%d : %d)\n", \
	 cntrlInfo->dlTiming.sfn, cntrlInfo->dlTiming.subframe, cntrlInfo->ulTiming.sfn, cntrlInfo->ulTiming.subframe);
      }
      /* Fill PCFICH info */
      /* Fix for DCFI FLE issue: when DL delta is 1 and UL delta is 0 and CFI
       *change happens in that SF then UL PDCCH allocation happens with old CFI
       *but CFI in control Req goes updated one since it was stored in the CELL
       */
      cntrlInfo->cfi = dlSf->pdcchInfo.currCfi;
#ifndef RG_ULSCHED_AT_CRC
      uint8_t                Mval  = 1;
#ifdef LTE_TDD
      Mval = rgSchTddPhichMValTbl[cell->ulDlCfgIdx]
                                 [cell->hiDci0Time.subframe];
      if(dlSf->sfType == RG_SCH_SPL_SF_DATA)
      {
         RGSCH_GET_SPS_SF_CFI(cell->bwCfg.dlTotalBw, cntrlInfo->cfi);
      }
#endif
      if(Mval)
      {
         /* Fill PHICH info */
         if ((ret = rgSCHTomUtlFillPhich (cell, cntrlInfo, ulSf, err)) != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Unable to send PHICH info "
                     "for cellId (%d)\n", cell->cellId);
            RGSCH_FREE_MEM(cntrlInfo);
            return ret;
         }
         if ((ret = rgSCHTomUtlFillUlPdcch (cell, cntrlInfo, ulSf, err)) != 
                        ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Unable to send PDCCH info "
                     "for cellId (%d)\n", cell->cellId);
            RGSCH_FREE_MEM(cntrlInfo);
            return ret;
         }
      }
#ifdef EMTC_ENABLE
      if(0 == cntrlInfo->ulMpdcchLst.count)
      {
         gUlMpdcchBlank++;
      }
#endif
#endif
#ifdef LTE_TDD
      sfTyp = rgSchTddUlDlSubfrmTbl[cell->ulDlCfgIdx]
                                   [cell->dlDciTime.subframe]; 
#endif      
      if (sfTyp != 2) /* Uplink subframe */   
      {
         /* Fill PDCCH info */
         if ((ret = rgSCHTomUtlFillDlPdcch(cell,cntrlInfo, dlSf, err)) != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Unable to send PDCCH info "
                     "for cellId (%d)\n", cell->cellId);
            RGSCH_FREE_MEM(cntrlInfo);
            return ret;
         }
         rgBwAlloInfo[dlSf->sfNum] += dlSf->bwAssigned;
         rgBwAlcnt[dlSf->sfNum] ++;

      }
#ifdef LTEMAC_SPS /* SPS_NEW_CHGS */
      cntrlInfo->isSPSOcc = dlSf->isSPSOcc;
#endif
      cntrlInfo->numDlActvUes += dlSf->numDlActvUes;  /* 4UE_TTI_DELTA */
      dlSf->numDlActvUes      = 0;
#ifdef EMTC_ENABLE
if(0 == cntrlInfo->dlMpdcchLst.count)
{
   gDlMpdcchBlank++;
}
#endif
      /* Now always sending down a cntrl req */
      /* sending the cntrl data to Phy */
      //if (rgSCHUtlTfuCntrlReq(inst, cell->tfuSap->sapCfg.suId, cntrlInfo) 
            //!= ROK)
      {
         DU_LOG("\nERROR  -->  SCH : Unable to send Cntrl info for cell");
      }
   return ROK;

} 


/** @brief This function handles sending of the PHICH information for the
 * downlink subframe to be sent in the next TTI.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Loop through the PHICH information present in the downlink
 *         subframe and fill the information in cntrlInfo.
 *
 * @param  [out] TfuCntrlReqInfo *cntrlInfo 
 * @param  [in]  RgSchDlSf   *dlSf
 * @param  [out] RgSchErrInfo *err
 * @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
static S16 rgSCHTomUtlFillPhich
(
RgSchCellCb     *cell,
TfuCntrlReqInfo    *cntrlInfo,
RgSchDlSf          *dlSf,
RgSchErrInfo       *err
)
{
   S16             ret;
   CmLList         *node;
   RgSchPhich      *phich;
   TfuPhichInfo    *harqAck;
#ifdef TFU_UPGRADE
   RgSchCmnDlCell   *cellDl = RG_SCH_CMN_GET_DL_CELL(cell);
#endif

   ret = ROK;
   /* Traversing the list of Phichs */
   node =  dlSf->phichInfo.phichs.first;
   while (node)
   {
      phich = (RgSchPhich*)node->node;
      if ((ret = rgSCHUtlGetEventMem((Ptr *)&harqAck, sizeof(TfuPhichInfo), 
                                &(cntrlInfo->memCp))) != ROK)
      {
         err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
         return ret;
      }
#ifdef TFU_ALLOC_EVENT_NO_INIT
      harqAck->txPower = 0;
#endif
      /* fill in the tfu structure from the information present in the
       * phich node */
      harqAck->rbStart     = phich->rbStart;
      harqAck->nDmrs       = phich->nDmrs;
      harqAck->isAck       = phich->hqFeedBack;
      harqAck->isForMsg3   = phich->isForMsg3;  /*SR_RACH_STATS : PHICH ACK/NACK for MSG3 */
#ifdef LTE_TDD
      /* Changes for passing iPhich at TFU interface*/
      harqAck->iPhich      = phich->iPhich;
#endif
  /* ccpu00138898 - Added Tx pwr offset for PHICH Tx*/
#ifdef TFU_UPGRADE
      harqAck->txPower       = cellDl->phichTxPwrOffset;
#endif
      cmLListAdd2Tail(&cntrlInfo->phichLst, &(harqAck->lnk));
      harqAck->lnk.node = (PTR)harqAck;
      node = node->next;
   } /* end of while */
   return ret;
} /* end of */ 


#ifdef LTE_ADV
/** @brief This function is a utility function to restart 
 * deactivation timer.
 *
 * @details
 *
 *     Function: rgSCHTmrRestartScellDeactTmr 
 *
 *         Processing steps:
 *         - Starts timer at scheduler
 *         
 *   @param[in]  RgSchCellCb    *cell
 *   @param[in]  CmLteRnti       rnti
 *   @return  Void
 */
static Void rgSCHTmrRestartScellDeactTmr
(
RgSchCellCb     *cell,
RgSchUeCb       *ueCb
)
{
   RgSchUeCellInfo   *sCellInfo = NULLP;
   
   if(NULLP != ueCb)
   {
      if(RG_SCH_IS_CELL_SEC(ueCb, cell))
      {
         sCellInfo = ueCb->cellInfo[(ueCb->cellIdToCellIdxMap[RG_SCH_CELLINDEX(cell)])];

         if(sCellInfo->deactTmr.tmrEvnt != TMR_NONE)
         {
            rgSCHTmrStopTmr(cell, RG_SCH_TMR_SCELL_DEACT, sCellInfo);
         }
         if(PRSNT_NODEF == ueCb->sCellDeactTmrVal.pres)
         {
            /*
            rgSCHTmrStartTmr(cell,sCellInfo,RG_SCH_TMR_SCELL_DEACT,
                  ueCb->sCellDeactTmrVal.val);
            */
         }
      }
   }
}/*end of rgSCHTmrRestartScellDeactTmr*/
#endif

/** @brief This function will send all the PDCCH's for the given downlink
 * subframe.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Loop through all the scheduled HARQ processes and fill
 *           the PDCCH information in cntrlInfo.
 *
 * @param  [out] TfuCntrlReqInfo *cntrlInfo 
 * @param  [in]  RgSchDlSf   *dlSf
 * @param  [out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
uint32_t numdlSpsRelSentToTf;
static S16 rgSCHTomUtlFillDlPdcch
(
RgSchCellCb     *cell,
TfuCntrlReqInfo *cntrlInfo,
RgSchDlSf       *dlSf,
RgSchErrInfo    *err
)
{
   S16          ret;
   CmLList      *node;
   RgSchPdcch   *pdcch;
   TfuPdcchInfo *tfuPdcch;
   uint8_t isDcivld = FALSE;
   uint8_t            numUePerTti = 0;

   ret = ROK;
   /* Traversing the scheduled Harq processes */
   node =  dlSf->pdcchInfo.pdcchs.first;
   while (node)
   {
      pdcch = (RgSchPdcch*)node->node;
       switch(pdcch->dci.dciFormat)
       {
          case TFU_DCI_FORMAT_3:
             isDcivld = (pdcch->dci.u.format3Info.isPucch) ? TRUE : FALSE;
             break;

          case TFU_DCI_FORMAT_3A:
             isDcivld = (pdcch->dci.u.format3AInfo.isPucch) ? TRUE : FALSE;
             break;

          default:
             isDcivld = TRUE;
             break;
       }
      if(!isDcivld) 
      {
         node = node->next;
         continue;
      }    

      /*ccpu00117179 - ADD - Build only non DCI format-0 messages */
      if((pdcch->dci.dciFormat == TFU_DCI_FORMAT_0) ||
			(pdcch->dci.dciFormat == TFU_DCI_FORMAT_A1) ||
			(pdcch->dci.dciFormat == TFU_DCI_FORMAT_A2))
      {
         node = node->next;
         continue;
      }


#ifdef RGSCH_SPS_STATS
      if((pdcch->dci.dciFormat == TFU_DCI_FORMAT_1A) &&
         (pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.mcs == 0x1F) &&
         (pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.alloc.type == TFU_ALLOC_TYPE_RIV) &&
         (pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.alloc.u.riv == 0xFFFFFFFF))
      {
         numdlSpsRelSentToTf++;
      }
#endif

      if ((ret = rgSCHUtlGetEventMem((Ptr *)&tfuPdcch, sizeof(TfuPdcchInfo),
                                &(cntrlInfo->memCp))) != ROK)
      {
         err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
         return ret;
      }
#ifdef LTEMAC_SPS
      tfuPdcch->crnti       = pdcch->crnti;
      tfuPdcch->isSpsRnti   = pdcch->isSpsRnti;
#endif
      tfuPdcch->rnti       = pdcch->rnti;
      
#ifdef LTE_ADV
      rgSCHTmrRestartScellDeactTmr(cell,pdcch->ue);
#endif
      tfuPdcch->dciNumOfBits = pdcch->dciNumOfBits;

      tfuPdcch->nCce       = pdcch->nCce;
      tfuPdcch->aggrLvl    = pdcch->aggrLvl;
      tfuPdcch->dci        = pdcch->dci;
#ifdef RG_5GTF
      //TODO_SID: Need to check these values during INT
      tfuPdcch->sectorId = 0;
      tfuPdcch->sccIdx = 0;
		tfuPdcch->grpId = 
#endif
      /* SR_RACH_STATS : Reset isTBMsg4 */
      pdcch->dci.u.format1aInfo.t.pdschInfo.isTBMsg4 = FALSE;        
      /* To be enhanced later for 2.1 */
      cmLListAdd2Tail(&cntrlInfo->dlPdcchLst, &(tfuPdcch->lnk));
      tfuPdcch->lnk.node = (PTR)tfuPdcch;
      node = node->next;
      if((pdcch->rnti > 60) && (pdcch->rnti < 5000))
      {
#if defined (TENB_STATS) && defined (RG_5GTF)
         cell->tenbStats->sch.dl5gtfPdcchSend++;
#endif
         numUePerTti++;
      }
   } /* end of while */

   if((numUePerTti) && (numUePerTti < RG_MAX_NUM_UE_PER_TTI ))
   {
      cell->dlNumUeSchedPerTti[numUePerTti-1]++;
      {
         gDlNumUePerTti[numUePerTti-1]++;
      }
   }
   return ret;
} /* end of rgSCHTomUtlFillDlPdcch*/ 

#ifdef RGSCH_SPS_STATS
uint32_t rgSchSpsRelSentToTf;
uint32_t rgSchSpsRelPdcchAllocd; 
#endif
/** @brief This function will send all the UL PDCCH's for the given
 * subframe.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Loop through all the scheduled HARQ processes and fill
 *           the PDCCH information in cntrlInfo.
 *
 * @param  [out] TfuCntrlReqInfo *cntrlInfo 
 * @param  [in]  RgSchDlSf   *dlSf
 * @param  [out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
static S16 rgSCHTomUtlFillUlPdcch
(
RgSchCellCb       *cell,
TfuCntrlReqInfo *cntrlInfo,
RgSchDlSf       *dlSf,
RgSchErrInfo    *err
)
{
   S16          ret;
   CmLList      *node;
   RgSchPdcch   *pdcch;
   TfuPdcchInfo *tfuPdcch;
   uint8_t isDcivld = FALSE;

   ret = ROK;
   /* Traversing the scheduled Harq processes */
   node =  dlSf->pdcchInfo.pdcchs.first;
   while (node)
   {
      pdcch = (RgSchPdcch*)node->node;
      node = node->next;
      /*ccpu00116712- Function should pick only UL allocation related control
       * info- start */
       switch(pdcch->dci.dciFormat)
       {
          case TFU_DCI_FORMAT_A1:
             isDcivld = TRUE;
             break;

			 case TFU_DCI_FORMAT_A2:
             isDcivld = TRUE;
             break;

          case TFU_DCI_FORMAT_3:
             isDcivld = (pdcch->dci.u.format3Info.isPucch) ? FALSE : TRUE;
             break;

          case TFU_DCI_FORMAT_3A:
             isDcivld = (pdcch->dci.u.format3AInfo.isPucch) ? FALSE : TRUE;
             break;

          default:
             isDcivld = FALSE;
             break;
       }
      if(!isDcivld) 
      {    
         continue;
      } 
#ifdef CA_DBG
      gDci0Count++;
#endif

      /*ccpu00116712- Function should pick only UL allocation related control
       * info- end */
      if ((ret = rgSCHUtlGetEventMem((Ptr *)&tfuPdcch, sizeof(TfuPdcchInfo),
                  &(cntrlInfo->memCp))) != ROK)
      {
         err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
         return ret;
      }
      tfuPdcch->rnti       = pdcch->rnti;
#ifdef LTE_ADV
      rgSCHTmrRestartScellDeactTmr(cell,pdcch->ue);
#endif
      tfuPdcch->dciNumOfBits = pdcch->dciNumOfBits;

      tfuPdcch->nCce       = pdcch->nCce;
      tfuPdcch->aggrLvl    = pdcch->aggrLvl;
      tfuPdcch->dci        = pdcch->dci;
#ifdef RG_5GTF
      //TODO_SID: Need to check these values during INT
      tfuPdcch->sectorId = 0;
      tfuPdcch->sccIdx = 0;
		tfuPdcch->grpId = 
#endif
      /* To be enhanced later for 2.1 */
      gUl5gtfPdcchSend++;
#if defined (TENB_STATS) && defined (RG_5GTF)
      cell->tenbStats->sch.ul5gtfPdcchSend++;
#endif
      cmLListAdd2Tail(&cntrlInfo->ulPdcchLst, &(tfuPdcch->lnk));
      tfuPdcch->lnk.node = (PTR)tfuPdcch;
   } /* end of while */

#ifdef RGSCH_SPS_STATS
   if (rgSchSpsRelSentToTf != rgSchSpsRelPdcchAllocd)
   {
     //   abort();
   }
#endif
   return ret;
} /* end of rgSCHTomUtlFillUlPdcch*/ 

/** @brief This function does the processing for Timing adjustment.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Loop through the ue present ueTimeLst, decrement the remaining
 *         frames left. 
 *
 *
 * @param  [in] RgSchCellCb   *cell
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
static S16 rgSCHTomUtlProcTA(RgSchCellCb    *cell)
{
   CmLList     *node;
   RgSchUeCb   *ue;

   node =  cell->taUeLst.first;
   while (node)
   {
      ue = (RgSchUeCb *)node->node;
      node = node->next;
      if (ue->dl.taCb.numRemSf == 0)
      {
         ue->dl.taCb.state = RGSCH_TA_IDLE;
         /* If Outstanding Ta is present, schedule it */
         if(ue->dl.taCb.outStndngTa == TRUE)
         {
            rgSCHUtlReTxTa(cell, ue);
         }
         else
         {
            /* We need to reset state and also value of TA, 
             * then we start the timer */
            ue->dl.taCb.ta = RGSCH_NO_TA_RQD;
            /* Start the timer only if TA is cfgd as FINITE value */
            if (ue->dl.taCb.cfgTaTmr)
            {
               rgSCHTmrStartTmr (cell, ue, RG_SCH_TMR_TA, ue->dl.taCb.cfgTaTmr);
            }
         }
         /* need to delete from the link list */
         cmLListDelFrm(&(cell->taUeLst), &(ue->taLnk));
         ue->taLnk.node = NULLP;
      }
      else
      {
         ue->dl.taCb.numRemSf--;
      }
   } /* end of taUeLst */
   return ROK;
} /* end of rgSCHTomUtlProcTA */ 

/** @brief This function handles filling of Hq reception request to
 *   Per Hq Proc.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef TFU_UPGRADE
S16 rgSCHTomUtlFillHqFdbkInfo
(
TfuRecpReqInfo          *recpReqInfo,
RgSchCellCb             *cell,
uint16_t                validIdx,
RgSchDlHqProcCb         *hqCb,
RgSchDlSf               *nxtDlsf,
TfuUeRecpReqInfo        *pucchRecpInfo,
RgSchDlHqProcCb         *prvHqCb,
RgSchErrInfo            *err
)
#else
S16 rgSCHTomUtlFillHqFdbkInfo
(
TfuRecpReqInfo          *recpReqInfo,
RgSchCellCb             *cell,
RgSchDlHqProcCb         *hqCb,
RgSchDlSf               *nxtDlsf,
TfuUeRecpReqInfo        *pucchRecpInfo,
RgSchDlHqProcCb         *prvHqCb,
RgSchErrInfo            *err
)
#endif 
{ 
   S16                  ret;
   RgSchDlHqTbCb        *tbCb;
   uint32_t             idx;
   Bool                 isAddToLst = FALSE;

   for (idx = 0 ;idx < 2; idx++)
   {
      if (HQ_TB_WAITING == hqCb->tbInfo[idx].state)
      {
         tbCb = &hqCb->tbInfo[idx];

         /* FOR ACK NAK REP */
         if ((hqCb->hqE->ue != NULLP) &&
               (hqCb->hqE->ue->measGapCb.isMeasuring == TRUE))
         {
            if ((tbCb->fbkRecpRepCntr) && 
                  (--tbCb->fbkRecpRepCntr))
            {
               /* Add to next subfarme */
               /* Add this hqCb to the next dlSf's ackNakRepQ */
               cmLListAdd2Tail (&(nxtDlsf->ackNakRepQ), 
                     &(tbCb->anRepLnk[tbCb->fbkRecpRepCntr]));
               tbCb->anRepLnk[tbCb->fbkRecpRepCntr].node = (PTR)tbCb;
               tbCb->crntSubfrm[tbCb->fbkRecpRepCntr] = nxtDlsf;
            }
#ifdef TFU_UPGRADE
            rgSCHTomUtlMoveNxtOccasion(cell, hqCb->hqE->ue, validIdx);
#endif 
            continue;
         }
#ifdef TFU_UPGRADE
         if (hqCb->tbCnt) 
         {
            hqCb->tbCnt--;
            /* Go to the next node */
            continue;
         }
#endif


         //if (hqCb != prvHqCb) 
         {
            ret = rgSCHUtlGetEventMem((Ptr *)&pucchRecpInfo,
                  sizeof(TfuUeRecpReqInfo), &(recpReqInfo->memCp));
            if (ret != ROK)
            {
               DU_LOG("\nERROR  -->  SCH : Unable to Allocate TfuUeRecpReqInfo for cell");
               err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
               return ret;
            }
            pucchRecpInfo->type = TFU_RECP_REQ_PUCCH;
            if ((hqCb->hqE->ue != NULLP) /*&& 
                                           ((tbCb->lchSchdData[0].lcId != 0) || (tbCb->taSnt ==
                                           TRUE))*/
               )
            {
               pucchRecpInfo->rnti = hqCb->hqE->ue->ueId;
            }
            else
            {
               if (hqCb->hqE->raCb)
               {
                  pucchRecpInfo->rnti    =  hqCb->hqE->raCb->tmpCrnti;
               }
            }
#ifndef TFU_UPGRADE
#ifndef TFU_TDD         
#ifdef LTEMAC_SPS
            if (!hqCb->spsN1PucchRes.pres)
#endif
            {
               pucchRecpInfo->t.pucchRecpReq.hqType = 
                  TFU_HQ_RECP_REQ_NORMAL;
               pucchRecpInfo->t.pucchRecpReq.t.nCce = 
                  hqCb->pdcch->nCce;
            }
#ifdef LTEMAC_SPS
            else
            {
               pucchRecpInfo->t.pucchRecpReq.hqType = 
                  TFU_HQ_RECP_REQ_N1PUCCH;
               pucchRecpInfo->t.pucchRecpReq.t.n1Pucch =
                  hqCb->spsN1PucchRes.val;
            }
#endif
#endif
            /* Handling of other types */
            pucchRecpInfo->t.pucchRecpReq.type = TFU_UCI_HARQ;
#else   /* TFU_UPGRADE */
            pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ;
            if ((hqCb->tbInfo[0].state == HQ_TB_WAITING) && 
                  (hqCb->tbInfo[1].state == HQ_TB_WAITING))
            {
               pucchRecpInfo->t.pucchRecpReq.hqInfo.hqSz = 2; /* MIMO */
            }
            else
            {
               pucchRecpInfo->t.pucchRecpReq.hqInfo.hqSz = 1; /* NON-MIMO */ 
            }
            {
#ifdef LTEMAC_SPS
               /* PucchRecpReq needs to be filled up for n1Pucch resource for SPS
                * ocassions */
               if (hqCb->spsN1PucchRes.pres)
               {
                  pucchRecpInfo->t.pucchRecpReq.hqInfo.hqRes[0] = hqCb->spsN1PucchRes.val;
               }
               else
#endif /* LTEMAC_SPS */
               {
                  pucchRecpInfo->t.pucchRecpReq.hqInfo.hqRes[0] = (hqCb->pdcch->nCce +
                        cell->pucchCfg.n1PucchAn);
               }
#ifdef EMTC_ENABLE
               rgSCHEmtcFillPucchRecpInfo(cell, hqCb, &(pucchRecpInfo->t.pucchRecpReq.hqInfo.hqRes[0]));
#endif
            }
#endif/*TFU_UPGRADE*/

#ifdef TFU_UPGRADE
            rgSCHTomUtlFillCqiSrSrsWithHq(cell,recpReqInfo, hqCb->hqE->ue, 
                  pucchRecpInfo, validIdx,FALSE);    
#endif 
#ifdef EMTC_ENABLE
            /* Passing last parameter as FALSE in this case as it will be verified from hqCb*/
            isAddToLst = rgSCHEmtcAddRecpInfoToLst(hqCb,recpReqInfo, pucchRecpInfo,FALSE);
#endif
            if(!isAddToLst)
            {
               cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(pucchRecpInfo->lnk));
            }
            pucchRecpInfo->lnk.node = (PTR)pucchRecpInfo;
         }

         if ((tbCb->fbkRecpRepCntr) && 
               (--tbCb->fbkRecpRepCntr))
         {
            /* Add to next subfarme */
            /* Add this hqCb to the next dlSf's ackNakRepQ */
            cmLListAdd2Tail (&(nxtDlsf->ackNakRepQ), 
                  &(tbCb->anRepLnk[tbCb->fbkRecpRepCntr]));
            tbCb->anRepLnk[tbCb->fbkRecpRepCntr].node = (PTR)tbCb;
            tbCb->crntSubfrm[tbCb->fbkRecpRepCntr] = nxtDlsf;
         }
         break;
      }
   }
   return ROK;
}/* end of rgSCHTomUtlFillHqFdbkInfo */

#ifdef RG_5GTF 
/** @brief This function handles filling of Hq reception request to
 *   Per Hq Proc.
 *
 * @details
 *
 *     Function:rgSCHTomUtlFillHqFdbkFor5gtf 
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo   *recpReqInfo
 * @param  [in]  RgSchCellCb      *cell
 * @param  [in]  uint16_t              validIdx,
 * @param  [in]  RgSchDlHqInfo    *dlSfHqInfo,
 * @param  [in]  RgSchDlSf        *dlSf,
 * @param  [in]  TfuUeRecpReqInfo *pucchRecpInfo,
 * @param  [out] RgSchErrInfo     *err
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
static S16 rgSCHTomUtlFillHqFdbkFor5gtf
(
TfuRecpReqInfo          *recpReqInfo,
RgSchCellCb             *cell,
uint16_t                validIdx,
RgSchDlHqInfo           *dlSfHqInfo,
RgSchDlSf               *dlSf,
TfuUeRecpReqInfo        *pucchRecpInfo,
RgSchErrInfo            *err
)
{ 
   S16                  ret;
   RgSchUeCb            *ue;
   TfuUePucchRecpReq    *pucchReqInfo = NULLP;

   ue = (RgSchUeCb*)dlSfHqInfo->dlSfUeLnk.node;
   
   if (ue == NULLP)
   {
      return RFAILED;
	}
   ret = rgSCHUtlGetEventMem((Ptr *)&pucchRecpInfo,
         sizeof(TfuUeRecpReqInfo), &(recpReqInfo->memCp));
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Unable to Allocate "
               "TfuUeRecpReqInfo for cellId=%d \n", cell->cellId);
      err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
      return ret;
   }
   pucchRecpInfo->type = TFU_RECP_REQ_PUCCH;
   pucchRecpInfo->rnti = ue->ueId; /* Even for Rel pdcch also setting CRNTI
                                      * instead of SPS-CRNTI */

   pucchReqInfo = &(pucchRecpInfo->t.pucchRecpReq);

   pucchReqInfo->uciInfo = TFU_XPUCCH_UCI_INFO;

   /* 5gtf TODO : Hardcoded nPUCCHIdx */
   pucchReqInfo->uciPduInfo.pucchIndex = 0;

   pucchReqInfo->uciPduInfo.numBits = 1;
         
   /* 5gtf TODO : CQI Periodicity Hardcoded to (n,0)*/
   if (RGSCH_TIMEINFO_SAME (recpReqInfo->timingInfo, ue->ue5gtfCb.nxtCqiRiOccn)) 
   {
      pucchReqInfo->uciPduInfo.numBits += 5;
      RG_SCH_ADD_TO_CRNT_TIME(recpReqInfo->timingInfo, ue->ue5gtfCb.nxtCqiRiOccn,
                              ue->ue5gtfCb.cqiRiPer);
   }
   
   cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(pucchRecpInfo->lnk));
   pucchRecpInfo->lnk.node = (PTR)pucchRecpInfo;
   return ROK;
}/* end of rgSCHTomUtlFillHqFdbkForFrmt1B */
#endif

#ifdef LTE_ADV
/** @brief This function handles filling of Hq reception request to
 *   Per Hq Proc.
 *
 * @details
 *
 *     Function:rgSCHTomUtlFillHqFdbkForFrmt1B 
 *
 *         Processing steps:
 *         Allocates the N1Pucch Resources based on teh A Value
 *
 * @param  [out] TfuRecpReqInfo   *recpReqInfo
 * @param  [in]  RgSchCellCb      *cell
 * @param  [in]  uint16_t              validIdx,
 * @param  [in]  RgSchDlHqInfo    *dlSfHqInfo,
 * @param  [in]  RgSchDlSf        *dlSf,
 * @param  [in]  TfuUeRecpReqInfo *pucchRecpInfo,
 * @param  [out] RgSchErrInfo     *err
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef TFU_UPGRADE
static S16 rgSCHTomUtlFillHqFdbkForFrmt1B
(
TfuRecpReqInfo          *recpReqInfo,
RgSchCellCb             *cell,
uint16_t                     validIdx,
RgSchDlHqInfo           *dlSfHqInfo,
RgSchDlSf               *dlSf,
TfuUeRecpReqInfo        *pucchRecpInfo,
RgSchErrInfo            *err
)
#else
static S16 rgSCHTomUtlFillHqFdbkForFrmt1B
(
TfuRecpReqInfo          *recpReqInfo,
RgSchCellCb             *cell,
RgSchDlHqInfo           *dlSfHqInfo,
RgSchDlSf               *dlSf,
TfuUeRecpReqInfo        *pucchRecpInfo,
RgSchErrInfo            *err
)
#endif 
{ 
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif
   S16                  ret;
   CmLList              *hqPNode;
   RgSchDlHqProcCb      *hqCb = NULLP;
   RgSchUeCb            *ue;
   TfuUePucchRecpReq    *pucchReqInfo = NULLP;
   Bool                 isDatPresOnSecCell = FALSE;
   uint8_t              primCellTbCount = 0;

   hqPNode = dlSfHqInfo->hqPLst.first;
   ue = (RgSchUeCb*)dlSfHqInfo->dlSfUeLnk.node;
   
   if (ue == NULLP)
   {
      return RFAILED;
	}
   ret = rgSCHUtlGetEventMem((Ptr *)&pucchRecpInfo,
         sizeof(TfuUeRecpReqInfo), &(recpReqInfo->memCp));
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Unable to Allocate TfuUeRecpReqInfo for cellId=%d \n", cell->cellId);
      err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
      return ret;
   }
   pucchRecpInfo->type = TFU_RECP_REQ_PUCCH;
   pucchRecpInfo->rnti = ue->ueId; /* Even for Rel pdcch also setting CRNTI
                                      * instead of SPS-CRNTI */

   pucchReqInfo = &(pucchRecpInfo->t.pucchRecpReq);

#ifndef TFU_UPGRADE
   pucchReqInfo->hqType = TFU_HQ_RECP_REQ_NORMAL;
   /* Handling of other types */
   pucchReqInfo->type = TFU_UCI_HARQ;
#else   /* TFU_UPGRADE */
   pucchReqInfo->uciInfo = TFU_PUCCH_HARQ;
   /* Fill hqFdbkMode by using uciFrmtTyp from dlSfHqInfo */
   pucchReqInfo->hqInfo.hqFdbkMode = rgSchUtlGetFdbkMode(dlSfHqInfo->uciFrmtTyp);
   /* Fill HqSz by using totalTbCnt based on the TM mode and
    * the number of serv cells configured*/

   pucchReqInfo->hqInfo.hqSz = ue->f1bCsAVal;
   pucchReqInfo->hqInfo.pucchResCnt = ue->f1bCsAVal;

  memset(pucchReqInfo->hqInfo.hqRes,0xff,sizeof(uint16_t)*TFU_MAX_HQ_RES);
#ifdef LTEMAC_SPS
   /* Two Resources needs to be configured if the 
    * serving cell is in mimo mode else single
    * resource */
   if ((dlSf->relPdcch != NULLP) && 
         (RGSCH_TIMEINFO_SAME(recpReqInfo->timingInfo, ue->relPdcchFbkTiming)))
   {/* Pcell is having sps rel pdcch present */
      if(rgSCHUtlGetMaxTbSupp(ue->mimoInfo.txMode) > 1)
      {/* prim cell is in mimo mode, use 0 and 1 */
         pucchReqInfo->hqInfo.hqRes[0] = (dlSf->relPdcch->nCce +
                                          cell->pucchCfg.n1PucchAn);
         pucchReqInfo->hqInfo.hqRes[1] = pucchReqInfo->hqInfo.hqRes[0] + 1; 
                                        
             
      }else
      {
         pucchReqInfo->hqInfo.hqRes[2] = (dlSf->relPdcch->nCce +
                                          cell->pucchCfg.n1PucchAn);
      }
      /* Release the pdcch  so that it will not further processed */
      rgSCHUtlPdcchPut(ue->cell,&dlSf->pdcchInfo, dlSf->relPdcch);
      dlSf->relPdcch = NULLP;/* only one UE will be scheduled for release pdcch order in one tti */
   }
#endif/*LTEMAC_SPS*/
#endif/*TFU_UPGRADE*/
   while(hqPNode)
   {
      hqCb = (RgSchDlHqProcCb *)hqPNode->node;
      hqPNode = hqPNode->next;
      /* In case of CSI + 1BCS , CSI will be 
       * dropped if scheduling is present on
       * seconday cell.36.213 10.1.1
       * */
      if(RG_SCH_IS_CELL_SEC(ue,hqCb->hqE->cell))
      {
         isDatPresOnSecCell = TRUE;
      }else
      {
         if ((hqCb->tbInfo[0].state == HQ_TB_WAITING) && 
             (hqCb->tbInfo[1].state == HQ_TB_WAITING))
         {
            primCellTbCount = 2;
         }else
         {
            primCellTbCount = 1;
         }
      }
#ifndef TFU_UPGRADE
      pucchReqInfo->t.nCce = hqCb->pdcch->nCce;
#else
      {
         switch(ue->f1bCsAVal)
         {/* A Value */
            case RG_SCH_A_VAL_2:
               /* harq(0) is primary harq(1) is secondary) */
               if(RG_SCH_IS_CELL_SEC(ue,hqCb->hqE->cell))
               {
                   pucchReqInfo->hqInfo.hqRes[1] = ue->n1PucchF1bResCb.\
                                                   cw1N1Res[hqCb->tpc].n1PucchIdx;
               }
               else/* primary cell */
               {
#ifdef LTEMAC_SPS
                  /* Need to consider only sps occasions */
                  if (hqCb->spsN1PucchRes.pres)
                  {
                     pucchReqInfo->hqInfo.hqRes[0] = hqCb->spsN1PucchRes.val;
                  }
                  else
#endif /* LTEMAC_SPS */
                  {

                     pucchReqInfo->hqInfo.hqRes[0] = (hqCb->pdcch->nCce +
                           cell->pucchCfg.n1PucchAn);
                  }
               }
               break;
            case RG_SCH_A_VAL_3:
               /* Serving cell in mimo mode should be
                * in 0 and 1 and the serving cell in siso
                * mode should be in 2 indices */
               if(RG_SCH_IS_CELL_SEC(ue,hqCb->hqE->cell))
               {
                   uint8_t cellIdx = ue->cellIdToCellIdxMap[RG_SCH_CELLINDEX(hqCb->hqE->cell)];
                   if(rgSCHUtlGetMaxTbSupp(ue->cellInfo[cellIdx]->txMode.txModeEnum) > 1)
                   {/* Sec cell is in mimo mode, use 0 and 1 */
                      pucchReqInfo->hqInfo.hqRes[0] = 
                         ue->n1PucchF1bResCb.cw1N1Res[hqCb->tpc].n1PucchIdx;

                      pucchReqInfo->hqInfo.hqRes[1] = 
                            ue->n1PucchF1bResCb.cw2N1Res[hqCb->tpc].n1PucchIdx;
                   }
                   else
                   {/* Sec cell is in siso mode, use 2 */
                      pucchReqInfo->hqInfo.hqRes[2] = 
                         ue->n1PucchF1bResCb.cw1N1Res[hqCb->tpc].n1PucchIdx;
                   }   
               }
               else
               {/* primary cell hq */
                  if(rgSCHUtlGetMaxTbSupp(ue->mimoInfo.txMode) > 1)
                  {/* prim cell is in mimo mode, use 0 and 1 */
#ifdef LTEMAC_SPS
                     if (hqCb->spsN1PucchRes.pres)
                     {/* SPS occasions */
                        pucchReqInfo->hqInfo.hqRes[0] = hqCb->spsN1PucchRes.val;
                        pucchReqInfo->hqInfo.hqRes[1] = hqCb->spsN1PucchRes.val + 1;
                     }
                     else
#endif /* LTEMAC_SPS */
                     {
                        pucchReqInfo->hqInfo.hqRes[0] = (hqCb->pdcch->nCce +
                              cell->pucchCfg.n1PucchAn);
                        pucchReqInfo->hqInfo.hqRes[1] = (hqCb->pdcch->nCce +
                                 cell->pucchCfg.n1PucchAn + 1);
                     }
                  }
                  else
                  {/* prim cell is in siso mode use 2 */
#ifdef LTEMAC_SPS
                     /* Need to consider only sps occasions */
                     if (hqCb->spsN1PucchRes.pres)
                     {
                        pucchReqInfo->hqInfo.hqRes[2] = hqCb->spsN1PucchRes.val;
                     }
                     else
#endif /* LTEMAC_SPS */
                     {

                        pucchReqInfo->hqInfo.hqRes[2] = (hqCb->pdcch->nCce +
                              cell->pucchCfg.n1PucchAn);

                     }
                  }
               }
               break;
            case RG_SCH_A_VAL_4:
               {
                  if(RG_SCH_IS_CELL_SEC(ue,hqCb->hqE->cell))
                  {/*  2 and 3 for sec cell */
                     pucchReqInfo->hqInfo.hqRes[2] = 
                        ue->n1PucchF1bResCb.cw1N1Res[hqCb->tpc].n1PucchIdx;
                     pucchReqInfo->hqInfo.hqRes[3] = 
                        ue->n1PucchF1bResCb.cw2N1Res[hqCb->tpc].n1PucchIdx;
                  }
                  else/* primary cell */
                  {/* 0 and 1 are for primary cell */
#ifdef LTEMAC_SPS
                     /* Need to consider only sps occasions */
                     if (hqCb->spsN1PucchRes.pres)
                     {
                        pucchReqInfo->hqInfo.hqRes[0] = hqCb->spsN1PucchRes.val;
                        pucchReqInfo->hqInfo.hqRes[1] = hqCb->spsN1PucchRes.val + 1;
                     }
                     else
#endif /* LTEMAC_SPS */
                     {

                        pucchReqInfo->hqInfo.hqRes[0] = (hqCb->pdcch->nCce +
                              cell->pucchCfg.n1PucchAn);
                        pucchReqInfo->hqInfo.hqRes[1] = (hqCb->pdcch->nCce +
                              cell->pucchCfg.n1PucchAn + 1);
                     }
                  }
               }

               break;
            default:
               /* TOD:: Add error print */
               break;
         }
      }
#endif/*TFU_UPGRADE*/
   }
#ifdef TFU_UPGRADE

#ifdef CA_DBG
   {
      gF1bCsCount++;
      gF1bCsPres = TRUE;
   }

#endif
   rgSCHTomUtlFillCqiSrSrsWithHq(cell,recpReqInfo, ue, 
         pucchRecpInfo, validIdx,isDatPresOnSecCell);    

   /* Channel selection wil not be used in case of
    * CQI + HARQ. if the data was present only on 
    * primary cell */
   if((isDatPresOnSecCell == FALSE) &&
      (dlSfHqInfo->uciFrmtTyp == RG_SCH_UCI_FORMAT1B_CS))
   {/* Data is present only on primary cell */
    
      switch(pucchReqInfo->uciInfo)
      {
         case TFU_PUCCH_HARQ_SRS:
         case TFU_PUCCH_HARQ_CQI:
         case TFU_PUCCH_HARQ_SR_SRS:
         case TFU_PUCCH_HARQ_SR_CQI:
            {
               dlSfHqInfo->uciFrmtTyp          = RG_SCH_UCI_FORMAT1A_1B;
               pucchReqInfo->hqInfo.hqSz       = primCellTbCount;
               pucchReqInfo->hqInfo.hqFdbkMode = rgSchUtlGetFdbkMode(dlSfHqInfo->uciFrmtTyp);
            }
            break;
         default:
            {
               break;
            }
      }
   }
#endif/*TFU_UPGRADE*/
   cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(pucchRecpInfo->lnk));
   pucchRecpInfo->lnk.node = (PTR)pucchRecpInfo;
   return ROK;
}/* end of rgSCHTomUtlFillHqFdbkForFrmt1B */
/** @brief This function handles filling of Hq reception request to
 *   Per Hq Proc.
 *
 * @details
 *
 *     Function:rgSCHTomUtlFillHqFdbkForFrmt3 
 *
 *         Processing steps:
 *         Allocates the N1Pucch Resources based on teh A Value
 *
 * @param  [out] TfuRecpReqInfo   *recpReqInfo
 * @param  [in]  RgSchCellCb      *cell
 * @param  [in]  uint16_t              validIdx,
 * @param  [in]  RgSchDlHqInfo    *dlSfHqInfo,
 * @param  [in]  RgSchDlSf        *dlSf,
 * @param  [in]  TfuUeRecpReqInfo *pucchRecpInfo,
 * @param  [out] RgSchErrInfo     *err
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef TFU_UPGRADE
static S16 rgSCHTomUtlFillHqFdbkForFrmt3
(
TfuRecpReqInfo          *recpReqInfo,
RgSchCellCb             *cell,
uint16_t                     validIdx,
RgSchDlHqInfo           *dlSfHqInfo,
RgSchDlSf               *dlSf,
TfuUeRecpReqInfo        *pucchRecpInfo,
RgSchErrInfo            *err
)
#else
static S16 rgSCHTomUtlFillHqFdbkForFrmt3
(
TfuRecpReqInfo          *recpReqInfo,
RgSchCellCb             *cell,
RgSchDlHqInfo           *dlSfHqInfo,
RgSchDlSf               *dlSf,
TfuUeRecpReqInfo        *pucchRecpInfo,
RgSchErrInfo            *err
)
#endif 
{ 
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif
   S16                  ret;
   //CmLList              *hqPNode;
   RgSchUeCb            *ue;
   TfuUePucchRecpReq    *pucchReqInfo = NULLP;

   //hqPNode = dlSfHqInfo->hqPLst.first;
   ue = (RgSchUeCb*)dlSfHqInfo->dlSfUeLnk.node;
   
   if (ue == NULLP)
   {
      return RFAILED;
	}
   ret = rgSCHUtlGetEventMem((Ptr *)&pucchRecpInfo,
         sizeof(TfuUeRecpReqInfo), &(recpReqInfo->memCp));
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Unable to Allocate "
               "TfuUeRecpReqInfo for cellId=%d \n", cell->cellId);
      err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
      return ret;
   }
   pucchRecpInfo->type = TFU_RECP_REQ_PUCCH;
   pucchRecpInfo->rnti = ue->ueId; /* Even for Rel pdcch also setting CRNTI
                                      * instead of SPS-CRNTI */

   pucchReqInfo = &(pucchRecpInfo->t.pucchRecpReq);

#ifndef TFU_UPGRADE
   pucchReqInfo->hqType = TFU_HQ_RECP_REQ_NORMAL;
   /* Handling of other types */
   pucchReqInfo->type = TFU_UCI_HARQ;
#else   /* TFU_UPGRADE */
   pucchReqInfo->uciInfo = TFU_PUCCH_HARQ;
   /* Fill hqFdbkMode by using uciFrmtTyp from dlSfHqInfo */
   pucchReqInfo->hqInfo.hqFdbkMode = rgSchUtlGetFdbkMode(dlSfHqInfo->uciFrmtTyp);
   /* Fill HqSz by using totalTbCnt based on the TM mode and
    * the number of serv cells configured*/

   pucchReqInfo->hqInfo.hqSz = ue->f1bCsAVal;
   pucchReqInfo->hqInfo.pucchResCnt = 1;

  memset(pucchReqInfo->hqInfo.hqRes,0xff,sizeof(uint16_t)*TFU_MAX_HQ_RES);
#endif/*TFU_UPGRADE*/
   pucchReqInfo->hqInfo.hqRes[0] = dlSfHqInfo->n3ScellPucch.n3PucchIdx; 
#ifdef TFU_UPGRADE
   rgSCHTomUtlFillCqiSrSrsWithHq(cell,recpReqInfo, ue, 
         pucchRecpInfo, validIdx,TRUE);    
#endif/*TFU_UPGRADE*/
   cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(pucchRecpInfo->lnk));
   pucchRecpInfo->lnk.node = (PTR)pucchRecpInfo;
   return ROK;
}/* end of rgSCHTomUtlFillHqFdbkForFrmt3 */

#endif/*LTE_ADV*/

/** @brief This function handles filling of HARQ feedback recption request to
 * PHY.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo *recpReqInfo
 * @param  [in]  RgSchCellCb   *cell
 * @param  [out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef TFU_UPGRADE
static S16 rgSCHTomUtlFillHqFdbkRecpReq
(
TfuRecpReqInfo    *recpReqInfo,
RgSchCellCb       *cell,
uint16_t          validIdx,
RgSchErrInfo      *err
)
#else
static S16 rgSCHTomUtlFillHqFdbkRecpReq
(
TfuRecpReqInfo    *recpReqInfo,
RgSchCellCb       *cell,
RgSchErrInfo      *err
)
#endif 
{
   CmLList              *node;
   RgSchDlHqProcCb      *hqCb;
   CmLteTimingInfo      futTime;
   RgSchDlSf            *dlSf;
   RgSchDlSf            *nxtDlsf;
   TfuUeRecpReqInfo     *pucchRecpInfo = NULLP;
   RgSchUeCb            *ue;
   RgSchDlHqProcCb      *prvHqCb=NULLP;

#ifdef CA_DBG
   {
      gF1bCsPres = FALSE;
   }
#endif
   /* ccpu00133109: Removed RGSCHSUBFRMCRNTTIME as it is not giving proper output 
    * if diff is more than 10. Instead using RGSCHDECRFRMCRNTTIME() as it is 
    * serving the purpose */
   RGSCHDECRFRMCRNTTIME(cell->crntTime, futTime, (RG_SCH_CMN_HARQ_INTERVAL - 
       TFU_RECPREQ_DLDELTA));
   dlSf = rgSCHUtlSubFrmGet (cell, futTime);
   /* Get the next dlsf as well */
   RG_SCH_ADD_TO_CRNT_TIME(futTime, futTime, 1)
   nxtDlsf = rgSCHUtlSubFrmGet (cell, futTime);

   prvHqCb = NULLP;

   if (dlSf->ueLst.count != 0)
   {
      node =  dlSf->ueLst.first;
      while (node)
      {
         ue = (RgSchUeCb *)(node->node);
         node = node->next;

         if(ue->dl.dlSfHqInfo[cell->cellId][dlSf->dlIdx].isPuschHarqRecpPres == TRUE)
         {/* This UE is already considered for PUSCH
             Ignore for PUCCH */
            continue;
         }
         rgSCHTomUtlFillHqFdbkFor5gtf(recpReqInfo, cell, validIdx, 
               &ue->dl.dlSfHqInfo[cell->cellId][dlSf->dlIdx], dlSf, pucchRecpInfo, err);
      } /* end of while */
   } /* If hq is expected */

   if (dlSf->msg4HqPLst.count != 0)
   {
      prvHqCb = NULLP;
      node =  dlSf->msg4HqPLst.first;
      while (node)
      {
         hqCb = (RgSchDlHqProcCb*)(node->node);
         node = node->next;
         //TODO_SID: need to check validIdx 
         rgSCHTomUtlFillHqFdbkInfo (recpReqInfo, cell, validIdx, hqCb, nxtDlsf, pucchRecpInfo, prvHqCb, err);
         prvHqCb = hqCb; 
      } /* end of while */
   }

   /* Check with TDD Code */
   /* FOR ACK NACK REP */
   return ROK;
} /* end of rgSCHTomUtlFillHqFdbkRecpReq */ 
#ifdef TFU_UPGRADE 
/** @brief This function handles filling of SR reception request to
 * PHY.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo *recpReqInfo
 * @param  [in]  RgSchCellCb   *cell
 * @param  [out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef UNUSED_FUNC
static S16 rgSCHTomUtlFillSrRecpReq
(
TfuRecpReqInfo          *recpReqInfo,
RgSchCellCb             *cell,
uint16_t                  validIdx,
RgSchErrInfo            *err
)
{
   CmLList              *node;
   TfuUeRecpReqInfo     *pucchRecpInfo;
   S16                  ret;

   RgSchUeCb            *ue;
   Bool                 isAddToLst;
#ifdef LTEMAC_SPS
   RgSchCmnUlUeSpsInfo   *ulSpsUe = NULL;
#endif


   isAddToLst = FALSE;

   node = cell->pCqiSrsSrLst[validIdx].srLst.first;
   while(node)
   {
      ue = (RgSchUeCb *)(node->node);
      /* Fix: ccpu00124011: Fix for missing reception request for UE with same offset */
      node = node->next;
      if(ue == NULLP)
      {
         continue;
      }
      if ((ret = rgSCHUtlGetEventMem((Ptr *)&pucchRecpInfo,
                  sizeof(TfuUeRecpReqInfo),&(recpReqInfo->memCp))) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : Unable to Allocate TfuUeRecpReqInfo for cell RNTI:%d",ue->ueId);
         err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
         return ret;
      }
#ifdef TFU_ALLOC_EVENT_NO_INIT
     memset(&pucchRecpInfo->t.pucchRecpReq, 0, sizeof(TfuUePucchRecpReq));
      pucchRecpInfo->type = TFU_RECP_REQ_PUCCH;
#endif      
      /*Fill SR params*/


#ifdef LTEMAC_SPS
      /* Should we check for Rel8 and above???
       * Dont send SR recp req if logicalChannelSR-Mask enabled and UL SPS is
       * active*/
      ulSpsUe =  RG_SCH_CMN_GET_UL_SPS_UE(ue, cell);
      /* Avoiding check for ulSpsEnabled as isUlSpsActv FALSE if sps is not enabled*/
      if((ue->ul.ulSpsCfg.isLcSRMaskEnab) &&
         (ulSpsUe->isUlSpsActv))
      {
         rgSCHTomUtlMoveSrNxtOccasion(cell, ue);
         continue;
      }
#endif

      pucchRecpInfo->rnti =  ue->ueId; 
      pucchRecpInfo->t.pucchRecpReq.srInfo.n1PucchIdx = 
      ue->srCb.srCfg.srSetup.srResIdx;
      pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_SR;
      rgSCHTomUtlMoveSrNxtOccasion(cell, ue);
      rgSCHTomUtlFillCqiSrsWithSr(cell, ue, recpReqInfo,  
                  pucchRecpInfo, validIdx);
#ifdef EMTC_ENABLE
      isAddToLst = rgSCHEmtcAddRecpInfoToLst(NULLP,recpReqInfo, pucchRecpInfo,ue->isEmtcUe);
#endif
      if(!isAddToLst)
      {
         cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(pucchRecpInfo->lnk));
      }
      pucchRecpInfo->lnk.node = (PTR)pucchRecpInfo;
   }
   return ROK;
}/* end of rgSCHTomUtlFillSrRecpReq */
#endif
#endif
/** @brief This function tells will the UE has a periodic CQI/PMI/RI 
 *  reporting
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *
 *  @param  [in]   RgSchUeCb        *ue 
 *  @param  [out]  Bool             *willueRprtCqiRi  
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */

static S16 rgSCHTomUtlWillUeRprtCqiRi
(
RgSchUeCb        *ue,
Bool             *willueRprtCqiRi
)
{
   /* Intialising Reporting probability as TRUE */ 
   *willueRprtCqiRi = TRUE; 

   /* Checking the cases in which UE will not report CQIPMI/RI */
   if(ue->isDrxEnabled && ue->drxCb)
   {
#ifdef LTEMAC_R9
      if(ue->drxCb->cqiMask.pres && ue->drxCb->cqiMask.val == RGR_DRX_SETUP)
      {/*cqiMask is setup by upper layers */                                
         if((ue->drxCb->drxUlInactvMask & RG_SCH_DRX_ONDUR_BITMASK) ==
               RG_SCH_DRX_ONDUR_BITMASK)
         {/*onDuration NOT running, do not expect cqi/pmi/ri*/
            *willueRprtCqiRi = FALSE;
         }
         return ROK;
		}
#endif /*end of LTEMAC_R9*/
      /* ccpu00134258: Fix for CQI DRX issue*/
      if(ue->drxCb->onDurTmrLen > 2)
      {
         if ( !RG_SCH_DRX_UL_IS_UE_ACTIVE(ue->drxCb) ) 
         {/*UE is not active, do not expect cqi/pmi/ri*/                                  
            *willueRprtCqiRi = FALSE; 
         }
      }
   }/*ue->isDrxEnabled*/
   return ROK; 
} /*End of rgSCHTomUtlWillUeRprtCqiRi*/

/** @brief This function handles filling of RI reception request to
 * PHY.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo *recpReqInfo
 * @param  [in]  RgSchCellCb   *cell
 * @param  [in]  uint16_t           validIdx
 * @param  [out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef UNUSED_FUNC
static S16 rgSCHTomUtlFillRiRecpReq
(
TfuRecpReqInfo       *recpReqInfo,
RgSchCellCb          *cell,
uint16_t             validIdx,
RgSchErrInfo         *err
)
{
   CmLList              *node;
   TfuUeRecpReqInfo     *pucchRecpInfo;
   S16                  ret;
   RgSchUeCb            *ue;
   Bool                  willUeRprtCqi; /* Flag set due to CQI Mask
                                       and UE inactive state (DRX) */
   RgSchUePCqiCb *riCb = NULLP;

   node = cell->pCqiSrsSrLst[validIdx].riLst.first;
   while(node)
   {
      riCb = (RgSchUePCqiCb *)(node->node);
      ue = riCb->servCellInfo->ue;
      /* Fix: ccpu00124011: Fix for missing reception request for UE with same offset */
      node = node->next;
      if(riCb->riRecpPrcsd)
      {
         /*ccpu00140578:: RI Proecssing is already done for this TTI
          * as part of PUSCH reception process or HARQ 
          * Reception processing. Hence skipping this UE
          * */
         riCb->riRecpPrcsd = FALSE;
         continue;
      }
      if(riCb->riDist ==0)
      {
         rgSCHTomUtlWillUeRprtCqiRi(ue, &willUeRprtCqi);
#ifdef XEON_SPECIFIC_CHANGES
         if(RGSCH_TIMEINFO_SAME(cell->crntTime, ue->riRecpTime))
         {
            continue;
         }
#endif
#ifdef LTE_ADV
         if((TRUE == riCb->isRiIgnoByCollsn) 
               || (willUeRprtCqi == FALSE))
#else
         if(willUeRprtCqi == FALSE)
#endif
         {    
            rgSCHTomUtlMovePriNxtOccasion(cell, ue, riCb);
            continue;
         }  
         if ((ret = rgSCHUtlGetEventMem((Ptr *)&pucchRecpInfo,
                     sizeof(TfuUeRecpReqInfo),&(recpReqInfo->memCp))) != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Unable to Allocate "
               "TfuUeRecpReqInfo for cell RNTI:%d",ue->ueId);
            err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
            return ret;
         }
#ifdef TFU_ALLOC_EVENT_NO_INIT
         memset(&pucchRecpInfo->t.pucchRecpReq, 0, sizeof(TfuUePucchRecpReq));
         pucchRecpInfo->type = TFU_RECP_REQ_PUCCH;
#endif
         /*Fill RI params*/
         pucchRecpInfo->rnti =  ue->ueId; 
         pucchRecpInfo->t.pucchRecpReq.cqiInfo.n2PucchIdx = 
            riCb->cqiCfg.cqiSetup.cqiPResIdx; 
         pucchRecpInfo->t.pucchRecpReq.cqiInfo.cqiPmiSz = riCb->riNumBits; 
         pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_CQI;
         ue->rawCqiBitW[ue->cqiRiWritIdx].recvTime = recpReqInfo->timingInfo;
#ifdef LTE_ADV
         ue->rawCqiBitW[ue->cqiRiWritIdx].u.pucch.sCellIdx =
            riCb->servCellInfo->sCellIdx;
#endif
         rgSCHTomUtlFillRiBitWidthInfo(ue);
         rgSCHTomUtlMovePriNxtOccasion(cell, ue, riCb);
         if (ue->nPCqiCb->nCqiTrIdx == validIdx)
         {
            rgSCHTomUtlMovePcqiNxtOccasion(cell, ue, ue->nPCqiCb); 
         }
         if((ue->srsCb.nSrsTrIdx == validIdx) && (ue->srsCb.srsDist ==0))
         {
            rgSCHTomUtlMoveSrsNxtOccasion(cell, ue);
         }
         cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(pucchRecpInfo->lnk));
         pucchRecpInfo->lnk.node = (PTR)pucchRecpInfo;
      }
      else
      {
         riCb->riDist--; 
      }     
   }
   return ROK;
}/* end of rgSCHTomUtlFillRiRecpReq */
#endif
#ifdef RG_5GTF
/** @brief This function handles filling of 5GTF CQI-RI reception request to
 * PHY.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo *recpReqInfo
 * @param  [in]  RgSchCellCb   *cell
 * @param  [in]  uint16_t           validIdx
 * @param  [out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */

#ifdef UNUSED_FUNC
static S16 rgSCHTomUtlFillCqiRiRecpReq
(
TfuRecpReqInfo       *recpReqInfo,
RgSchCellCb          *cell,
uint16_t             validIdx, 
RgSchErrInfo         *err
)
{
   TfuUeRecpReqInfo  *pucchRecpInfo;
   RgSchUeCb         *ue = NULLP;
   uint16_t          ret;

   while ((ue = rgSCHDbmGetNextUeCb(cell, ue)) != NULLP)
   {
      if (RGSCH_TIMEINFO_SAME (recpReqInfo->timingInfo, ue->ue5gtfCb.nxtCqiRiOccn)) 
      {
         if ((ret = rgSCHUtlGetEventMem((Ptr *)&pucchRecpInfo,
                     sizeof(TfuUeRecpReqInfo),&(recpReqInfo->memCp))) != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Unable to Allocate TfuUeRecpReqInfo for cell RNTI:%d ", ue->ueId);
            err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
            return ret;
         }
#ifdef TFU_ALLOC_EVENT_NO_INIT
         memset(&pucchRecpInfo->t.pucchRecpReq, 0, sizeof(TfuUePucchRecpReq));
         pucchRecpInfo->type = TFU_RECP_REQ_PUCCH;     
#endif
         pucchRecpInfo->rnti =  ue->ueId; 
         pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_XPUCCH_UCI_INFO;
         pucchRecpInfo->t.pucchRecpReq.uciPduInfo.pucchIndex = 0;
         pucchRecpInfo->t.pucchRecpReq.uciPduInfo.numBits = 5;

         RG_SCH_ADD_TO_CRNT_TIME(recpReqInfo->timingInfo, ue->ue5gtfCb.nxtCqiRiOccn,
                                 ue->ue5gtfCb.cqiRiPer);
         cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(pucchRecpInfo->lnk));
         pucchRecpInfo->lnk.node = (PTR)pucchRecpInfo;
      }
   }
   return ROK;
}/* end of rgSCHTomUtlFillCqiRiRecpReq */
#endif
#endif
/** @brief This function handles filling of PCQI reception request to
 * PHY.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo *recpReqInfo
 * @param  [in]  RgSchCellCb   *cell
 * @param  [in]  uint16_t           validIdx
 * @param  [out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef UNUSED_FUNC
static S16 rgSCHTomUtlFillPcqiRecpReq
(
TfuRecpReqInfo       *recpReqInfo,
RgSchCellCb          *cell,
uint16_t             validIdx, 
RgSchErrInfo         *err
)
{
   CmLList              *node;
   TfuUeRecpReqInfo     *pucchRecpInfo;
   S16                  ret;
   RgSchUeCb            *ue;
   uint8_t              ri; /*RI value*/
   Bool                 willUeRprtCqi;   /* Flag set due to CQI Mask   and UE Inactive state (DRX)*/
   uint8_t              cqiPmiSz; 
   RgSchUePCqiCb        *cqiCb = NULLP;
   Bool                 isAddToLst = FALSE;

   node = cell->pCqiSrsSrLst[validIdx].cqiLst.first;
   while(node)
   {
      cqiCb = (RgSchUePCqiCb*)(node->node);
      ue = cqiCb->servCellInfo->ue;
      /* Fix: ccpu00124011: Fix for missing reception request for UE with same offset */
      node = node->next;
      rgSCHTomUtlWillUeRprtCqiRi(ue, &willUeRprtCqi);
#ifdef LTE_ADV
      if ((cqiCb->isCqiIgnoByCollsn == TRUE) ||
            (willUeRprtCqi == FALSE))
#else
      if(willUeRprtCqi == FALSE)
#endif
      {
         rgSCHTomUtlMovePcqiNxtOccasion(cell, ue, cqiCb);  
         continue;
      }

      ue->rawCqiBitW[ue->cqiRiWritIdx].recvTime = recpReqInfo->timingInfo;
#ifdef LTE_ADV
      ue->rawCqiBitW[ue->cqiRiWritIdx].u.pucch.sCellIdx =
         cqiCb->servCellInfo->sCellIdx;
#endif
      cqiPmiSz = rgSCHTomUtlFetchPcqiBitSz(ue, cell->numTxAntPorts, &ri); 
      if(!cqiPmiSz)
      {
         DU_LOG("\nERROR  -->  SCH : RNTI:%d Unable to Fill CqiPmi size", ue->ueId);
         continue;
      }

      if ((ret = rgSCHUtlGetEventMem((Ptr *)&pucchRecpInfo,
                  sizeof(TfuUeRecpReqInfo),&(recpReqInfo->memCp))) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : Unable to Allocate TfuUeRecpReqInfo for cell RNTI:%d ", ue->ueId);
         err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
         return ret;
      }
#ifdef TFU_ALLOC_EVENT_NO_INIT
      memset(&pucchRecpInfo->t.pucchRecpReq, 0, sizeof(TfuUePucchRecpReq));
      pucchRecpInfo->type = TFU_RECP_REQ_PUCCH;     
#endif
      
      /*Fill PCQI params*/
      pucchRecpInfo->rnti =  ue->ueId; 
      pucchRecpInfo->t.pucchRecpReq.cqiInfo.n2PucchIdx = 
         cqiCb->cqiCfg.cqiSetup.cqiPResIdx; 
      pucchRecpInfo->t.pucchRecpReq.cqiInfo.cqiPmiSz = cqiPmiSz; 
      pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_CQI;
      rgSCHTomUtlMovePcqiNxtOccasion(cell, ue, cqiCb);  
      if((ue->srsCb.nSrsTrIdx == validIdx) && (ue->srsCb.srsDist ==0))
      {
         rgSCHTomUtlMoveSrsNxtOccasion(cell, ue);
      }
#ifdef EMTC_ENABLE
      isAddToLst = rgSCHEmtcAddRecpInfoToLst(NULLP,recpReqInfo, pucchRecpInfo,ue->isEmtcUe);
#endif
      if(!isAddToLst)
      {
         cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(pucchRecpInfo->lnk));
      }
      pucchRecpInfo->lnk.node = (PTR)pucchRecpInfo;
   }
   return ROK;
}/* end of rgSCHTomUtlFillPcqiRecpReq */
/** @brief This function handles filling of SRS reception request to
 * PHY.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo *recpReqInfo
 * @param  [in]  RgSchCellCb   *cell
 * @param  [in]  uint16_t           validIdx
 * @param  [out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
static S16 rgSCHTomUtlFillSrsRecpReq
(
TfuRecpReqInfo       *recpReqInfo,
RgSchCellCb          *cell,
uint16_t             validIdx,
RgSchErrInfo         *err
)
{
   CmLList              *node;
   TfuUeRecpReqInfo     *pucchRecpInfo;
   S16                  ret;
   RgSchUeCb            *ue;

   node = cell->pCqiSrsSrLst[validIdx].srsLst.first;
   while(node)
   {
      ue = (RgSchUeCb *)(node->node);
      /* Fix: ccpu00124011: Fix for missing reception request for UE with same offset */
      node = node->next;
      if(ue->srsCb.srsRecpPrcsd)
      {
         /* ccpu00140578::SRS Proecssing is already done for this TTI
          * as part of PUSCH or HARQ reception process and
          * hence skipping this UE */
         ue->srsCb.srsRecpPrcsd = FALSE;
         continue;
      }

      if(ue->srsCb.srsDist ==0)
      {
         /* We need to add the recp request to be sent on the pucchANRep value. */
         if ((ret = rgSCHUtlGetEventMem((Ptr *)&pucchRecpInfo,
                     sizeof(TfuUeRecpReqInfo),&(recpReqInfo->memCp))) != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Unable to Allocate TfuUeRecpReqInfo for RNTI:%d ",ue->ueId);
            err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
            return ret;
         }

#ifdef TFU_ALLOC_EVENT_NO_INIT
         pucchRecpInfo->type = TFU_RECP_REQ_PUCCH;
#endif
         
         /*Fill SRS params*/
         pucchRecpInfo->rnti = ue->ueId;
         pucchRecpInfo->t.pucchRecpReq.srsInfo.srsBw = 
            (TfuUlSrsBwInfo)ue->srsCb.srsCfg.srsSetup.srsBw;
         pucchRecpInfo->t.pucchRecpReq.srsInfo.nRrc = 
            ue->srsCb.srsCfg.srsSetup.fDomPosi;
         pucchRecpInfo->t.pucchRecpReq.srsInfo.srsHopBw = 
            (TfuUlSrsHoBwInfo)ue->srsCb.srsCfg.srsSetup.srsHopBw;
         pucchRecpInfo->t.pucchRecpReq.srsInfo.transComb = 
            ue->srsCb.srsCfg.srsSetup.txComb;
         pucchRecpInfo->t.pucchRecpReq.srsInfo.srsCfgIdx = 
            ue->srsCb.srsCfg.srsSetup.srsCfgIdx;
         pucchRecpInfo->t.pucchRecpReq.srsInfo.srsCyclicShft = 
            (TfuUlSrsCycShiftInfo)ue->srsCb.srsCfg.srsSetup.cycShift;
         
         pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_SRS;
         rgSCHTomUtlMoveSrsNxtOccasion(cell, ue);
         cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(pucchRecpInfo->lnk));
         pucchRecpInfo->lnk.node = (PTR)pucchRecpInfo;
      }
      else
      {
         ue->srsCb.srsDist--; 
      }
   }
   return ROK;
}/* end of rgSCHTomUtlFillSrsRecpReq */
#endif 
#ifndef TFU_UPGRADE
/** @brief This function handles filling of data reception requests for
 *  PUSCH and MSG3.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo *recpReqInfo
 * @param  [in]  RgSchCellCb   *cell
 * @param  [out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
static S16 rgSCHTomUtlFillDatRecpReq
(
TfuRecpReqInfo       *recpReqInfo,
RgSchCellCb          *cell,
RgSchErrInfo         *err
)
{
   S16               ret;
   RgSchUlAlloc      *alloc;
   TfuUeRecpReqInfo  *datRecpInfo;


   /* processing steps are 
    * - Run through the UL allocations going out in this subframe.
    * - Run through the UL receptions expected the next subframe.
    */
   alloc = rgSCHUtlFirstRcptnReq (cell);
   while(alloc)
   {
      /* FOR ACK NACK REP */
      if (NULLP != alloc->ue)
      {
         /* If measuring or ackNakRep we shall not send dat RecpReq */
         if ((alloc->ue->measGapCb.isMeasuring == TRUE) ||
               (alloc->ue->ackNakRepCb.isAckNakRep == TRUE))
         {
            alloc = rgSCHUtlNextRcptnReq (cell, alloc);
            continue;
         }

      }
      if ((ret = rgSCHUtlGetEventMem((Ptr *)&datRecpInfo,
                                sizeof(TfuUeRecpReqInfo), 
                                &(recpReqInfo->memCp))) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : Unable to Allocate TfuUeRecpReqInfo for RNTI:%d ", alloc->ue->ueId);
         err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
         return ret;
      }
      if (!alloc->forMsg3)
      {
         datRecpInfo->type = TFU_RECP_REQ_PUSCH;
         rgSCHUtlAllocRcptInfo (alloc, 
            &datRecpInfo->rnti,
            &datRecpInfo->t.puschRecpReq.mcs,
            &datRecpInfo->t.puschRecpReq.rbStart,
            &datRecpInfo->t.puschRecpReq.numRb,
            &datRecpInfo->t.puschRecpReq.rv,
            &datRecpInfo->t.puschRecpReq.size,
            &datRecpInfo->t.puschRecpReq.modType,
            &datRecpInfo->t.puschRecpReq.isRtx,
            &datRecpInfo->t.puschRecpReq.nDmrs,
            &datRecpInfo->t.puschRecpReq.ndi,
            &datRecpInfo->t.puschRecpReq.harqProcId
            );
      }
      else
      {
         datRecpInfo->type = TFU_RECP_REQ_MSG3;
         rgSCHUtlAllocRcptInfo (alloc, 
            &datRecpInfo->rnti,
            &datRecpInfo->t.msg3RecpReq.mcs,
            &datRecpInfo->t.msg3RecpReq.rbStart,
            &datRecpInfo->t.msg3RecpReq.numRb,
			/*ccpu00128993 - MOD - fix for msg3 softcombining bug*/
            &datRecpInfo->t.msg3RecpReq.rv,
            &datRecpInfo->t.msg3RecpReq.size,
            &datRecpInfo->t.msg3RecpReq.modType,
            &datRecpInfo->t.msg3RecpReq.isRtx,
            &datRecpInfo->t.msg3RecpReq.nDmrs,
            &datRecpInfo->t.msg3RecpReq.ndi,
            &datRecpInfo->t.msg3RecpReq.harqProcId
            );
         
      }
      /* Other fields of datRecpInfo shall be filled 
       * here for new features */
      cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(datRecpInfo->lnk));
      datRecpInfo->lnk.node = (PTR)datRecpInfo;
      
      alloc = rgSCHUtlNextRcptnReq (cell, alloc);
   } /* end of while */
   return ROK;
} /* end of rgSCHTomUtlFillDatRecpReq */

#else
/** @brief This function handles filling of data reception requests for
 *  PUSCH and MSG3.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo *recpReqInfo
 * @param  [in]  RgSchCellCb   *cell
 * @param  [in]  uint16_t           validIdx
 * @param  [out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
static S16 rgSCHTomUtlFillDatRecpReq
(
TfuRecpReqInfo       *recpReqInfo,
RgSchCellCb          *cell,
uint16_t             validIdx,
RgSchErrInfo         *err
)
{
   CmLteTimingInfo   dci0Time;
   uint8_t           idx;
   S16               ret;
   RgSchUlAlloc      *alloc;
   TfuUeRecpReqInfo  *datRecpInfo;

   Bool              isAperiodic = FALSE; /*Set when Aperiodic CQI is expected */
   uint8_t           numUePerTti = 0;

   if((0 == (recpReqInfo->timingInfo.sfn % 30)) && (0 == recpReqInfo->timingInfo.slot))
   {
      //DU_LOG("\nERROR  -->  SCH : 5GTF_CHECK rgSCHTomUtlFillDatRecpReq (%d : %d)\n",\
      recpReqInfo->timingInfo.sfn, recpReqInfo->timingInfo.slot);
   }
   /* processing steps are 
    * - Run through the UL allocations going out in this subframe.
    * - Run through the UL receptions expected the next subframe.
    */

   alloc = rgSCHUtlFirstRcptnReq (cell);
   while(alloc)
   {
      isAperiodic = FALSE;
      ret = rgSCHUtlGetEventMem((Ptr *)&datRecpInfo,
            sizeof(TfuUeRecpReqInfo), &(recpReqInfo->memCp));
      if(ret != ROK)            
      {
         DU_LOG("\nERROR  -->  SCH : Unable to Allocate "
            "TfuUeRecpReqInfo for RNTI:%d ", alloc->rnti);
         err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
         return ret;
      }
#ifdef TFU_ALLOC_EVENT_NO_INIT
      datRecpInfo->t.puschRecpReq.initialNSrs.pres = FALSE;
      datRecpInfo->t.puschRecpReq.initialNumRbs.pres = FALSE;
#endif
      datRecpInfo->type = TFU_RECP_REQ_PUSCH;
      /* Check if this if for MSG3 - no scope for feedback along with it. */
      if ((FALSE == alloc->forMsg3))
      {
         /*  Check if any DL HARQ processes has a feedback coming at the time of
          *  this reception request. 
          */
/* ACC-TDD */    	  
         if(alloc->ue)
         {
            RGSCHDECRFRMCRNTTIME(cell->crntTime,dci0Time,(RGSCH_ULCTRL_RECP_DIST));
            
            idx = (dci0Time.sfn * RGSCH_NUM_SUB_FRAMES_5G + dci0Time.slot)%
                     RGSCH_ULCTRL_RECP_DIST; 
            UNUSED(idx);
            datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA;
            datRecpInfo->rnti = alloc->rnti;
            rgSCHUtlAllocRcptInfo (cell,alloc, &recpReqInfo->timingInfo,
                                   &datRecpInfo->t.puschRecpReq.ulSchInfo);
         }
      }
      else /*Enters for Msg3 == TRUE condition*/
      {
         /* ccpu00130884 - ADD - HO case when Msg3 alloc and Cqi/Ri/SRS opportunity 
          * occur at same time */
         if(NULLP != alloc->ue)
	 {
         		 
            /* Only DATA is expected */
            datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA;
            datRecpInfo->rnti = alloc->rnti;
            rgSCHUtlAllocRcptInfo (cell,alloc, &recpReqInfo->timingInfo,
               &datRecpInfo->t.puschRecpReq.ulSchInfo);
         }
      }
      if(alloc->ue)
      {
         if(datRecpInfo->t.puschRecpReq.rcpInfo != TFU_PUSCH_DATA &&
               datRecpInfo->t.puschRecpReq.rcpInfo != TFU_PUSCH_DATA_SRS &&
               isAperiodic == FALSE)
         {
            datRecpInfo->t.puschRecpReq.initialNumRbs.pres = TRUE;
            datRecpInfo->t.puschRecpReq.initialNumRbs.val = alloc->ue->initNumRbs;
         }
         else
         {
            datRecpInfo->t.puschRecpReq.initialNumRbs.pres = FALSE;
         }
      }
      cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(datRecpInfo->lnk));
      datRecpInfo->lnk.node = (PTR)datRecpInfo;
      alloc = rgSCHUtlNextRcptnReq (cell, alloc);
      numUePerTti++;
   } /* end of while */

   if(numUePerTti && (numUePerTti < RG_MAX_NUM_UE_PER_TTI))
   {
      cell->ulNumUeSchedPerTti[numUePerTti-1]++;
      gUlNumUePerTti[numUePerTti - 1]++;
   }
   return ROK;
} /* end of rgSCHTomUtlFillDatRecpReq */
#endif
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
/***********************************************************
 *
 *     Func : rgSCHTomUtlFillRiBitWidthInfo
 *
 *
 *     Desc : Fills the RI BitWidth and stores it for decoding.
 *
 *     Ret  : S16
 *            ROK - Success
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
S16 rgSCHTomUtlFillRiBitWidthInfo
(
RgSchUeCb     *ueCb
)
{
   RgSchUePCqiCb *riCb = ueCb->nPRiCb;
   
   if (ueCb->mimoInfo.txMode != RGR_UE_TM_3 &&
             ueCb->mimoInfo.txMode != RGR_UE_TM_4)
   {
      return RFAILED;
	}

   ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].type = TFU_RECP_REQ_PUCCH;
   ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pucch.pucchRawCqiInfo.mode= 
      (TfuDlCqiPucchMode)riCb->cqiCfg.cqiSetup.prdModeEnum;
   switch(ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pucch.pucchRawCqiInfo.mode)
   {
      case TFU_PUCCH_CQI_MODE10:
         ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pucch.pucchRawCqiInfo.u.mode10Info.type = TFU_RPT_RI;
         ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pucch.pucchRawCqiInfo.u.mode10Info.u.ri = 
            riCb->riNumBits;
         break;
      case TFU_PUCCH_CQI_MODE11:
         ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pucch.pucchRawCqiInfo.u.mode11Info.type = TFU_RPT_RI;
         ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pucch.pucchRawCqiInfo.u.mode11Info.u.ri = 
            riCb->riNumBits;
         break;
      case TFU_PUCCH_CQI_MODE20:
         ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pucch.pucchRawCqiInfo.u.mode20Info.type = TFU_RPT_RI;
         ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pucch.pucchRawCqiInfo.u.mode20Info.u.ri = 
            riCb->riNumBits;
         break;
      case TFU_PUCCH_CQI_MODE21:
         ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pucch.pucchRawCqiInfo.u.mode21Info.type = TFU_RPT_RI;
         ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pucch.pucchRawCqiInfo.u.mode21Info.u.ri = 
            riCb->riNumBits;
         break;
      default:
         break;
   }
   
   RG_SCH_INCR_CQIRI_INDEX(ueCb->cqiRiWritIdx);
   return ROK;
}

/***********************************************************
 *
 *     Func : rgSCHTomUtlFetchPcqiBitSz
 *
 *
 *     Desc : Fetch the CQI/PMI bits for a UE based on the mode, periodicity.
 *
 *     Ret  : uint8_t
 *            ROK - Success
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
uint8_t rgSCHTomUtlFetchPcqiBitSz
(
RgSchUeCb  *ueCb, 
uint8_t    numTxAnt,
uint8_t    *ri
)
{
   uint8_t confRepMode;
   uint8_t pcqiSz;
   TfuCqiPucchMode10   *mode10Info;    
   TfuCqiPucchMode11   *mode11Info;    
   TfuCqiPucchMode20   *mode20Info;   
   TfuCqiPucchMode21   *mode21Info;    
   RgSchUePCqiCb *cqiCb = ueCb->nPCqiCb;


   confRepMode = cqiCb->cqiCfg.cqiSetup.prdModeEnum;
   if((ueCb->mimoInfo.txMode != RGR_UE_TM_3) && 
         (ueCb->mimoInfo.txMode != RGR_UE_TM_4))
   {
      *ri =1;
   }
   else
   {
      *ri = cqiCb->perRiVal;
   }
   ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].type = TFU_RECP_REQ_PUCCH;
   ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pucch.pucchRawCqiInfo.mode= 
       (TfuDlCqiPucchMode)confRepMode;
   switch(confRepMode)
   {
      case RGR_PRD_CQI_MOD10:
         {
            mode10Info = &ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pucch.pucchRawCqiInfo.u.mode10Info;
            pcqiSz = 4;
            mode10Info->type = TFU_RPT_CQI;
            mode10Info->u.cqi = 4;
         }
         break;

      case RGR_PRD_CQI_MOD11:
         {
            mode11Info = &ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pucch.pucchRawCqiInfo.u.mode11Info;
            mode11Info->type = TFU_RPT_CQI;
            if(numTxAnt == 2)
            {
               if (*ri ==1)
               {
                  pcqiSz = 6;
                  mode11Info->u.cqi.cqi = 4;
                  mode11Info->u.cqi.wideDiffCqi.pres = FALSE;
                  mode11Info->u.cqi.pmi = 2;
               }
               else
               {
                  pcqiSz = 8;
                  mode11Info->u.cqi.cqi = 4;
                  mode11Info->u.cqi.wideDiffCqi.pres = TRUE;
                  mode11Info->u.cqi.wideDiffCqi.val = 3;
                  mode11Info->u.cqi.pmi = 1;
               }
            }
            else if(numTxAnt == 4)
            {
               if (*ri ==1)
               {
                  pcqiSz = 8;
                  mode11Info->u.cqi.cqi = 4;
                  mode11Info->u.cqi.wideDiffCqi.pres = FALSE;
                  mode11Info->u.cqi.pmi = 4;
               }
               else
               {
                  pcqiSz = 11;
                  mode11Info->u.cqi.cqi = 4;
                  mode11Info->u.cqi.wideDiffCqi.pres = TRUE;
                  mode11Info->u.cqi.wideDiffCqi.val = 3;
                  mode11Info->u.cqi.pmi = 4;
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
            mode20Info = &ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pucch.pucchRawCqiInfo.u.mode20Info;
            mode20Info->type = TFU_RPT_CQI;
            if(cqiCb->isWb)
            {
               pcqiSz = 4;
               mode20Info->u.cqi.isWideband = TRUE;
               mode20Info->u.cqi.u.wideCqi = 4;
            }
            else
            {
               pcqiSz = 4 + cqiCb->label;
               mode20Info->u.cqi.isWideband = FALSE;
               mode20Info->u.cqi.u.subCqi.cqi = 4;
               mode20Info->u.cqi.u.subCqi.l = cqiCb->label;
            }
         }
         break;

      case RGR_PRD_CQI_MOD21:
         {
            mode21Info = &ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pucch.pucchRawCqiInfo.u.mode21Info;
            mode21Info->type = TFU_RPT_CQI;
            //pcqiSz = rgSCHTomUtlFetchPcqiBitSzPucchMode21(ueCb, 
              //                   mode21Info, numTxAnt, ri);
         }
         break;
      default:
          pcqiSz = 0;
          break;
   }
   
   RG_SCH_INCR_CQIRI_INDEX(ueCb->cqiRiWritIdx);
   return pcqiSz;
}


/***********************************************************
 *
 *     Func : rgSCHTomUtlPcqiSbCalcBpIdx
 *
 *
 *     Desc : Determines the BP index from the timing info
 *
 *     Ret  : S16
 *            ROK - Success
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
S16 rgSCHTomUtlPcqiSbCalcBpIdx
(
CmLteTimingInfo   crntTimInfo,
RgSchUeCb         *ueCb,
RgSchUePCqiCb     *cqiCb
)
{
   uint16_t tti = (crntTimInfo.sfn * RGSCH_NUM_SUB_FRAMES_5G + crntTimInfo.slot);
   uint16_t  prdNum = tti/cqiCb->cqiPeri;

   if((prdNum % cqiCb->h) == 0)
   {
      cqiCb->isWb = TRUE;
#ifdef LTE_ADV
      cqiCb->prioLvl = RG_SCH_CQI_PRIO_LVL_1;
#endif
   }
   else
   {
      cqiCb->isWb = FALSE;
      cqiCb->bpIdx = ((prdNum % cqiCb->h) - 1) % cqiCb->J;  
#ifdef LTE_ADV
      cqiCb->prioLvl = RG_SCH_CQI_PRIO_LVL_0;
#endif
   }
   return ROK;
}


/**
 * @brief Function which moves PCQI, RI, SR and SRS to next periodicity
 *           Occasions as that needs to be done in case of Ack/Nack repetition
 *           reception request occasions or during Measurement Gap occasions.
 *
 * @details
 *
 *     Function: rgSCHTomUtlMoveNxtOccasion
 *
 *       Function which moves PCQI, RI, SR and SRS to next perodicity
 *      Occasions as that needs to be done in case of Ack/Nack repetition
 *      reception request occasions or during Measurement Gap occasions.
 *
 *     Invoked by: rgSCHTomUtlFillDatRecpReq of rg_sch_tom.c 
 *
 *     Processing Steps:
 *     - Check whether the current Tx Instance matches with the rec req time
 *     - If true, then move them to their next Tx Instance
 *           
 *  @param[in]   RgSchCellCb    *cell,
 *               RgSchUeCb      *ue,
 *               uint16_t            validIdx
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHTomUtlMoveNxtOccasion
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
uint16_t     validIdx
)
{
   RgSchUePCqiCb *cqiCb = ue->nPCqiCb;
   RgSchUePCqiCb *riCb = ue->nPRiCb;
 
   /* ccpu00140578::Skip the UE if already RI recpetion 
    * is processed in the same subframe */
   if ((riCb->nRiTrIdx == validIdx) &&
      (riCb->riRecpPrcsd == FALSE))
   {
      if(riCb->riDist ==0)
      {
         rgSCHTomUtlMovePriNxtOccasion(cell, ue, riCb); 
      }
      else
      {
         riCb->riDist--; 
      }
      /* ccpu00140578:: As this UE is considered for this TTI
       * Same UE should not get processed for RI reception 
       * or for updating th RI distance.*/
      if(riCb->nRiTrIdx == validIdx)
      {
         riCb->riRecpPrcsd = TRUE;
      }
   }
   if (cqiCb->nCqiTrIdx == validIdx)
   {
      rgSCHTomUtlMovePcqiNxtOccasion(cell, ue, cqiCb); 
   }

      /* ccpu00140578::Skip the UE if SRS recpetion 
       * is already processed in the same subframe */
   if ((ue->srsCb.nSrsTrIdx == validIdx) &&
       (ue->srsCb.srsRecpPrcsd == FALSE))  
   {
      if(ue->srsCb.srsDist ==0)
      {
         rgSCHTomUtlMoveSrsNxtOccasion(cell, ue);  
      }
      else
      {
         ue->srsCb.srsDist--; 
      }
      /* ccpu00140578:: As this UE is considered for this TTI
       * Same UE should not get processed for SRS reception 
       * or for updating th SRS distance.*/
      if(ue->srsCb.nSrsTrIdx == validIdx)
      {
         ue->srsCb.srsRecpPrcsd = TRUE;
      }
   }
   if (ue->srCb.nSrTrIdx == validIdx)
   {
      rgSCHTomUtlMoveSrNxtOccasion(cell, ue); 
   }
   return ROK;
}  /* rgSCHTomUtlMoveNxtOccasion */


/***********************************************************
 *
 *     Func : rgSCHTomPrepareAcqiRecp
 *
 *
 *     Desc : Fetch the CQI/PMI bits for a UE based on the mode and store them 
 *            for decoding. Fill RECP request and prepare the scartchpad
 *            to aid decoding of Aperiodic CQI. 
 *
 *     Ret  : Void
 *            ROK - void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
Void rgSCHTomPrepareAcqiRecp
(
RgSchUeCb	*ueCb, 
RgSchCellCb	*cell,
TfuUePuschCqiRecpInfo *cqiRecpReqInfo,
uint8_t         ccIdx
)
{
   uint8_t confRepMode;
   RgSchCqiRawPuschMode12   *mode12Info;   
   RgSchCqiRawPuschMode20   *mode20Info;    
   RgSchCqiRawPuschMode22   *mode22Info;    
   RgSchCqiRawPuschMode30   *mode30Info;   
   RgSchCqiRawPuschMode31   *mode31Info;    
   uint8_t numTxAnt   = cell->numTxAntPorts;
   uint8_t sCellIdx   = ueCb->cellIdToCellIdxMap[RG_SCH_CELLINDEX(cell)];
   uint8_t numOfCells = 0;
   RgSchUeACqiCb *acqiCb  = &ueCb->cellInfo[sCellIdx]->acqiCb;

   /* Fill TFU Recp */
   cqiRecpReqInfo->reportType = TFU_APERIODIC_CQI_TYPE; /* Aperiodic */
   if (ueCb->mimoInfo.txMode == RGR_UE_TM_3 ||
         ueCb->mimoInfo.txMode == RGR_UE_TM_4)
   {
      cqiRecpReqInfo->riSz[ccIdx].pres = TRUE;
      cqiRecpReqInfo->riSz[ccIdx].val  = acqiCb->riNumBits; 
   }
   /* This flag will be rmeoved after making changes in BRDCM CL
    * Sachin is doing the change 
    * */
#if (defined (TENB_T2K3K_SPECIFIC_CHANGES) && defined(LTE_TDD))
   //LTE_ADV_ACQI_SUPP
   cqiRecpReqInfo->cqiPmiSzR1[ccIdx]  = acqiCb->cqiPmiSzR1;
   cqiRecpReqInfo->cqiPmiSzRn1[ccIdx] = acqiCb->cqiPmiSzRn1;
#else
   if(ueCb->nPCqiCb->perRiVal == 1)
   {
      cqiRecpReqInfo->cqiPmiSzR1[ccIdx]  = acqiCb->cqiPmiSzR1;
   }
   else
   {
      cqiRecpReqInfo->cqiPmiSzRn1[ccIdx] = acqiCb->cqiPmiSzRn1;
   }
#endif
   /* Fill scratchpad to aid decoding of aper CQI upon
    * reception */
   confRepMode = acqiCb->aCqiCfg.aprdModeEnum;
   ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].type = TFU_RECP_REQ_PUSCH;

   numOfCells = ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pusch.numOfCells;

   ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pusch.cqiBitWidth[numOfCells].\
      puschRawCqiInfo.mode = (TfuDlCqiPuschMode)confRepMode;

   ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pusch.cqiBitWidth[numOfCells].\
      puschRawCqiInfo.ri.pres = cqiRecpReqInfo->riSz[ccIdx].pres;

   ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pusch.cqiBitWidth[numOfCells].\
      puschRawCqiInfo.ri.val = cqiRecpReqInfo->riSz[ccIdx].val;

   /* Setting the sCellIdx */
   ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pusch.cqiBitWidth[numOfCells].\
      sCellIdx = sCellIdx;

   switch(confRepMode)
   {
      case RGR_APRD_CQI_MOD12:
         {
            mode12Info = &ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pusch.\
                         cqiBitWidth[numOfCells].puschRawCqiInfo.u.mode12Info;
            mode12Info->wideBCqiCw0 = 4;
            mode12Info->r1WideBCqiCw1 = 0;
            mode12Info->rg1WideBCqiCw1 = 4;
            if(numTxAnt == 2)
            {
               mode12Info->r1TotalPmiBitLen = 2*acqiCb->N;
               mode12Info->rg1TotalPmiBitLen = acqiCb->N;
            }
            else if(numTxAnt == 4)
            {
               mode12Info->r1TotalPmiBitLen = 4*acqiCb->N;
               mode12Info->rg1TotalPmiBitLen = 4*acqiCb->N;
            }
         }
         break;

      case RGR_APRD_CQI_MOD20:
         {
            mode20Info = &ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pusch.\
                         cqiBitWidth[numOfCells].puschRawCqiInfo.u.mode20Info;
            mode20Info->wideBCqiCw = 4;
            mode20Info->subBandDiffCqi = 2;
            mode20Info->posOfM = acqiCb->L;
         }
         break;

      case RGR_APRD_CQI_MOD22:
         {
            mode22Info = &ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pusch.\
                         cqiBitWidth[numOfCells].puschRawCqiInfo.u.mode22Info;
            mode22Info->wideBCqiCw0 = 4;
            mode22Info->sBDiffCqiCw0 = 2;
            mode22Info->r1WideBCqiCw1 = 0;
            mode22Info->r1SbDiffCqiCw1 = 0;
            mode22Info->rg1WideBCqiCw1 = 4;
            mode22Info->rg1SbDiffCqiCw1 = 2;
            mode22Info->posOfM = acqiCb->L;
            if(numTxAnt == 2)
            {
               mode22Info->r1PmiBitLen = 4;
               mode22Info->rg1PmiBitLen = 2;
            }
            else if(numTxAnt == 4)
            {
               mode22Info->r1PmiBitLen = 8;
               mode22Info->rg1PmiBitLen = 8;
            }
         }
         break;

      case RGR_APRD_CQI_MOD30:
         {
            mode30Info = &ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pusch.\
                         cqiBitWidth[numOfCells].puschRawCqiInfo.u.mode30Info;
            mode30Info->wideBCqiCw = 4;
            mode30Info->totLenSbDiffCqi = 2*acqiCb->N;
         }
         break;

      case RGR_APRD_CQI_MOD31:
         {
            mode31Info = &ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pusch.\
                         cqiBitWidth[numOfCells].puschRawCqiInfo.u.mode31Info;
            mode31Info->wideBCqiCw0 = 4;
            mode31Info->totLenSbDiffCqiCw0 = 2*acqiCb->N;
            mode31Info->r1WideBCqiCw1 = 0;
            mode31Info->r1TotLenSbDiffCqiCw1 =0;
            mode31Info->rg1WideBCqiCw1 = 4;
            mode31Info->rg1TotLenSbDiffCqiCw1 = 2*acqiCb->N;
            if(numTxAnt == 2)
            {
               mode31Info->r1PmiBitLen = 2;
               mode31Info->rg1PmiBitLen = 1;
            }
            else if(numTxAnt == 4)
            {
               mode31Info->r1PmiBitLen = 4;
               mode31Info->rg1PmiBitLen = 4;
            }
         }
         break;
      default:
         break;
   }
   return;
}

/**
 * @brief Function which handles the filling of Aperiodic CQI/RI reception
 * request values 
 *
 * @details
 *
 *     Function: rgSCHTomUtlFillDatAperRecpReq
 *
 *    Function which handles the filling of Aperiodic CQI/RI reception
 *    request values
 *
 *     Invoked by: rgSCHTomUtlFillDatRecpReq of rg_sch_tom.c 
 *
 *     Processing Steps:
 *     - Fill the reception request for the data arriving on the ULSCH
 *     - Fill the reception request information for the Aperiodic CQI/PMI/RI
 *           
 *  @param[in]  RgSchCellCb       *cell,
 *              RgSchUlAlloc      *alloc,
 *              TfuUeRecpReqInfo  *datRecpInfo,
 *              CmLteTimingInfo   *timeInfo,
 *              Bool              hqPres
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHTomUtlFillDatAperRecpReq
(
RgSchCellCb       *cell,
uint8_t           cqiReq,
RgSchUlAlloc      *alloc,
TfuUeRecpReqInfo  *datRecpInfo,
CmLteTimingInfo   *timeInfo,
Bool              hqPres,
uint16_t          validIdx
)
{
   TfuUePuschCqiRecpInfo  *cqiRecpReqInfo;
   RgSchUeCb	*ueCb = alloc->ue;
#ifdef LTE_ADV
   uint8_t      triggerSet = 0;
   uint8_t      sIdx = 0;
#endif

   /*Fill RI Reception Params*/
   cqiRecpReqInfo = &datRecpInfo->t.puschRecpReq.cqiRiInfo;   
   cqiRecpReqInfo->riBetaOff = alloc->ue->ul.betaRiOffst;
   cqiRecpReqInfo->cqiBetaOff = alloc->ue->ul.betaCqiOffst;

   
   cqiRecpReqInfo->cCNum = 0;
   ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pusch.numOfCells = 0;

#ifdef LTE_ADV
   rgSCHTomUtlGetTrigSet(cell, ueCb, cqiReq, &triggerSet);
   for (sIdx = 0; sIdx < CM_LTE_MAX_CELLS; sIdx++)
   {
      /* The Aperiodic request for SCell index sIdx */
      if ((triggerSet >> (7 - sIdx)) & 0x01)
      {
         /* The Aperiodic request for SCell index sIdx */
         rgSCHTomPrepareAcqiRecp(ueCb, ueCb->cellInfo[sIdx]->cell, cqiRecpReqInfo, cqiRecpReqInfo->cCNum);
         cqiRecpReqInfo->cCNum++;
         ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pusch.numOfCells++;
      }
   }
#else
   rgSCHTomPrepareAcqiRecp(ueCb, ueCb->cellInfo[0]->cell, cqiRecpReqInfo, cqiRecpReqInfo->cCNum);
   ueCb->rawCqiBitW[ueCb->cqiRiWritIdx].u.pusch.numOfCells++;
#endif

   RG_SCH_INCR_CQIRI_INDEX(ueCb->cqiRiWritIdx);

   if((alloc->ue->srsCb.nSrsTrIdx == validIdx) && (alloc->ue->srsCb.srsDist ==0))
   {
         rgSCHTomFillOnlySrsRecpReq(cell,alloc, datRecpInfo); 
         datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI_SRS; 
         
   }
   if(hqPres && 
   (datRecpInfo->t.puschRecpReq.rcpInfo == TFU_PUSCH_DATA_CQI_SRS))
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI_HARQ_SRS;
   }
   else if (hqPres)
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI_HARQ;
   }
   else if(datRecpInfo->t.puschRecpReq.rcpInfo != TFU_PUSCH_DATA_CQI_SRS)
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI;
   }
   datRecpInfo->rnti = alloc->rnti;
   rgSCHUtlAllocRcptInfo (cell, alloc, timeInfo,
            &datRecpInfo->t.puschRecpReq.ulSchInfo);
   return ROK;
}  /* rgSCHTomUtlFillDatAperRecpReq */



/**
 * @brief Function which handles the filling of Periodic RI reception
 * request values which arrives along with UL Data on ULSCH
 *
 * @details
 *
 *     Function: rgSCHTomUtlFillDatPriRecpReq
 *
      *    Function which handles the filling of Periodic RI reception
 *    request values which arrives along with UL Data on ULSCH
 *
 *     Invoked by: rgSCHTomUtlFillDatRecpReq of rg_sch_tom.c 
 *
 *     Processing Steps:
 *     - Fill the reception request for the data arriving on the ULSCH
 *     - Fill the reception request information for the Periodic RI
 *           
 *  @param[in]  RgSchCellCb       *cell,
 *              RgSchUlAlloc      *alloc,
 *              TfuUeRecpReqInfo  *datRecpInfo,
 *              CmLteTimingInfo   *timeInfo,
 *              Bool              hqPres
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHTomUtlFillDatPriRecpReq
(
RgSchCellCb       *cell,
RgSchUlAlloc      *alloc,
TfuUeRecpReqInfo  *datRecpInfo,
CmLteTimingInfo   *timeInfo,
Bool              hqPres,
uint16_t          validIdx
)
{
   TfuUePuschCqiRecpInfo  *cqiRecpReqInfo;

   /*Fill RI Reception Params*/
   cqiRecpReqInfo = &datRecpInfo->t.puschRecpReq.cqiRiInfo;   
#ifdef TFU_ALLOC_EVENT_NO_INIT
   cqiRecpReqInfo->cqiBetaOff = 0;
   /* Fill only the first RI index since Periodic can come 
    * only for 1 CC */
   cqiRecpReqInfo->cqiPmiSzR1[0] = 0;
   cqiRecpReqInfo->cqiPmiSzRn1[0] = 0;
#endif
   cqiRecpReqInfo->reportType = TFU_PERIODIC_CQI_TYPE; /* periodic */
   cqiRecpReqInfo->riBetaOff = alloc->ue->ul.betaRiOffst;

   /* Fill only the first RI index since Periodic can come 
    * only for 1 CC */
   cqiRecpReqInfo->cCNum = 1;
   cqiRecpReqInfo->riSz[0].pres = TRUE;
   cqiRecpReqInfo->riSz[0].val = alloc->ue->nPRiCb->riNumBits;
    /*Other params*/
   rgSCHTomUtlFillRiBitWidthInfo(alloc->ue);
    if((alloc->ue->srsCb.nSrsTrIdx == validIdx) && (alloc->ue->srsCb.srsDist ==0))
   {
         rgSCHTomFillOnlySrsRecpReq(cell,alloc, datRecpInfo);
         datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI_SRS; 

   }
   if(hqPres && 
   (datRecpInfo->t.puschRecpReq.rcpInfo == TFU_PUSCH_DATA_CQI_SRS))
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI_HARQ_SRS;
   }
   else if (hqPres)
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI_HARQ;
   }
   else if(datRecpInfo->t.puschRecpReq.rcpInfo != TFU_PUSCH_DATA_CQI_SRS)
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI;
   }
   datRecpInfo->rnti = alloc->rnti;
   rgSCHUtlAllocRcptInfo (cell, alloc, timeInfo,
         &datRecpInfo->t.puschRecpReq.ulSchInfo);
   return ROK;
}  /* rgSCHTomUtlFillDatPriRecpReq */


/**
 * @brief Function which handles the filling of Periodic CQI/PMI reception
 * request values which arrives along with UL Data on ULSCH
 *
 * @details
 *
 *     Function: rgSCHTomUtlFillDatPCqiRecpReq
 *
 *    Function which handles the filling of Periodic CQI/PMI reception
 *    request values which arrives along with UL Data on ULSCH
 *
 *     Invoked by: rgSCHTomUtlFillDatRecpReq of rg_sch_tom.c 
 *
 *     Processing Steps:
 *     - Fill the reception request for the data arriving on the ULSCH
 *     - Fill the reception request information for the Periodic CQI/PMI
 *           
 *  @param[in]  RgSchCellCb       *cell,
 *              RgSchUlAlloc      *alloc,
 *              TfuUeRecpReqInfo  *datRecpInfo,
 *              CmLteTimingInfo         *timeInfo,
 *              Bool hqPres
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/

S16 rgSCHTomUtlFillDatPCqiRecpReq
(
RgSchCellCb       *cell,
RgSchUlAlloc      *alloc,
TfuUeRecpReqInfo  *datRecpInfo,
CmLteTimingInfo   *timeInfo,
Bool              hqPres, 
uint16_t          validIdx
)
{
   TfuUePuschCqiRecpInfo  *cqiRecpReqInfo;
   uint8_t     cqiPmiSz; /*Raw CQI/PMI Size*/
   uint8_t     ri;

   /*Fill CQI Reception Params*/
   cqiRecpReqInfo = &datRecpInfo->t.puschRecpReq.cqiRiInfo;
#ifdef TFU_ALLOC_EVENT_NO_INIT
   cqiRecpReqInfo->riBetaOff = 0;
#endif
   cqiRecpReqInfo->cqiBetaOff = alloc->ue->ul.betaCqiOffst;
   cqiPmiSz = rgSCHTomUtlFetchPcqiBitSz(alloc->ue, cell->numTxAntPorts, &ri);
   if(0 == cqiPmiSz)
   {
      DU_LOG("\nERROR  -->  SCH : Unable to Fill "
         "CqiPmi size RNTI:%d",alloc->rnti);
      return RFAILED;
   }

   /* Fill only the first RI index since Periodic can come 
    * only for 1 CC */
   cqiRecpReqInfo->cCNum = 1;
   cqiRecpReqInfo->reportType = TFU_PERIODIC_CQI_TYPE; /* Periodic */
   /* This flags will be removed once Sachin does changes 
    * in BRDCM CL */
#if (defined (TENB_T2K3K_SPECIFIC_CHANGES) && defined(LTE_TDD))
      cqiRecpReqInfo->cqiPmiSzR1[0] = cqiPmiSz;
      cqiRecpReqInfo->cqiPmiSzRn1[0] = cqiPmiSz;
#else
   if (ri ==1)
   {
      cqiRecpReqInfo->cqiPmiSzR1[0] = cqiPmiSz;
      cqiRecpReqInfo->cqiPmiSzRn1[0] = 0;
   }
   else
   {
      cqiRecpReqInfo->cqiPmiSzRn1[0] = cqiPmiSz;
      cqiRecpReqInfo->cqiPmiSzR1[0] = 0;
   }
#endif
   cqiRecpReqInfo->riSz[0].pres = FALSE; 

   if((alloc->ue->srsCb.nSrsTrIdx == validIdx) && (alloc->ue->srsCb.srsDist ==0))
   {
         rgSCHTomFillOnlySrsRecpReq(cell,alloc, datRecpInfo); 
         datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI_SRS; 
   }
   if(hqPres && 
   (datRecpInfo->t.puschRecpReq.rcpInfo == TFU_PUSCH_DATA_CQI_SRS))
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI_HARQ_SRS;
   }
   else if (hqPres)
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI_HARQ;
   }
   else if(datRecpInfo->t.puschRecpReq.rcpInfo != TFU_PUSCH_DATA_CQI_SRS)
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_CQI;
   }
   datRecpInfo->rnti = alloc->rnti;
   rgSCHUtlAllocRcptInfo (cell, alloc, timeInfo,
            &datRecpInfo->t.puschRecpReq.ulSchInfo);
   return ROK;
}  /* rgSCHTomUtlFillDatPCqiRecpReq */

/**
 * @brief Function which handles the filling of SRS reception
 * request values which arrives along with UL Data on ULSCH
 *
 * @details
 *
 *     Function: rgSCHTomUtlFillDatSrsRecpReq
 *
 *    Function which handles the filling of SRS reception
 *    request values which arrives along with UL Data on ULSCH
 *
 *     Invoked by: rgSCHTomUtlFillDatRecpReq of rg_sch_tom.c 
 *
 *     Processing Steps:
 *     - Fill the reception request for the data arriving on the ULSCH
 *     - Fill the reception request information for the SRS
 *           
 *  @param[in]  RgSchCellCb       *cell,
 *              RgSchUlAlloc      *alloc,
 *              TfuUeRecpReqInfo  *datRecpInfo,
 *              CmLteTimingInfo   *timeInfo,
 *              Bool              hqPres
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHTomUtlFillDatSrsRecpReq
(
RgSchCellCb       *cell,
RgSchUlAlloc      *alloc,
TfuUeRecpReqInfo  *datRecpInfo,
CmLteTimingInfo   *timeInfo,
Bool              hqPres
)
{
   datRecpInfo->rnti = alloc->rnti;
   rgSCHTomFillOnlySrsRecpReq(cell,alloc, datRecpInfo); 
   if(hqPres)
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_HARQ_SRS;
   }
   else
   {
      datRecpInfo->t.puschRecpReq.rcpInfo = TFU_PUSCH_DATA_SRS;
   }
   datRecpInfo->rnti = alloc->rnti;
   rgSCHUtlAllocRcptInfo (cell, alloc, timeInfo, 
            &datRecpInfo->t.puschRecpReq.ulSchInfo);
   return ROK;
}  /* rgSCHTomUtlFillDatSrsRecpReq */

/**
 * @brief Function which handles the filling of only SRS reception
 * request values on ULSCH
 *
 * @details
 *
 *     Function: rgSCHTomFillOnlySrsRecpReq
 *
 *    Function which handles the filling of SRS reception
 *    request values which arrives along with UL Data on ULSCH
 *
 *     Invoked by: rgSCHTomUtlFillDatSrsRecpReq of rg_sch_tom.c 
 *
 *     Processing Steps:
 *     - Fill the reception request for the data arriving on the ULSCH
 *     - Fill the reception request information for the SRS
 *           
 *  @param[in]  RgSchCellCb       *cell,
 *              RgSchUlAlloc      *alloc,
 *              TfuUeRecpReqInfo  *datRecpInfo,
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHTomFillOnlySrsRecpReq
(
RgSchCellCb       *cell,
RgSchUlAlloc      *alloc,
TfuUeRecpReqInfo  *datRecpInfo
)
{
   TfuUePuschSrsRecpInfo  *srsRecpReqInfo;

   srsRecpReqInfo = &datRecpInfo->t.puschRecpReq.srsInfo;
   srsRecpReqInfo->srsBw = (TfuUlSrsBwInfo)alloc->ue->srsCb.srsCfg.srsSetup.srsBw;
   srsRecpReqInfo->nRrc = alloc->ue->srsCb.srsCfg.srsSetup.fDomPosi;
   srsRecpReqInfo->srsHopBw = (TfuUlSrsHoBwInfo)alloc->ue->srsCb.srsCfg.srsSetup.srsHopBw;
   srsRecpReqInfo->transComb = alloc->ue->srsCb.srsCfg.srsSetup.txComb;
   srsRecpReqInfo->srsCfgIdx = alloc->ue->srsCb.srsCfg.srsSetup.srsCfgIdx;
   srsRecpReqInfo->srsCyclicShft = (TfuUlSrsCycShiftInfo)alloc->ue->srsCb.srsCfg.srsSetup.cycShift;

   /* ccpu00117050 - ADD - nSrs setting 
    * Refer Section 5.2.2.6 of TS 36.212 V8.5.0*/
   datRecpInfo->t.puschRecpReq.ulSchInfo.nSrs = 1;

   return ROK;
}  /* rgSCHTomFillOnlySrsRecpReq */

/**
 * @brief Function which handles the filling of PCQI/RI, SRS and SR
 *             Reception Request Information along
 *             with the HARQ reception Request
 *
 * @details
 *
 *     Function: rgSCHTomUtlFillCqiSrSrsWithHq
 *
 *              Function which handles the filling of PCQI/RI, SRS ans SR
 *             Reception Request Information along
 *             with the HARQ reception Request
 *
 *
 *     Invoked by: rgSCHTomUtlFillHqFdbkRecpReq & 
 *          rgSCHTomUtlFillSfHqFdbk of rg_sch_tom.c 
 *
 *     Processing Steps:
 *     - Fill the reception request for the Control Info arriving on the PUCCH
 *     - Fill the reception request information for the SR, RI, CQI, SRS
 *           
 *  @param[in]   RgSchCellCb       *cell,
 *               TfuRecpReqInfo    *recpReqInfo,
 *               RgSchDlHqProcCb   *hqCb,
 *  @param[out]  TfuUeRecpReqInfo  *pucchRecpInfo
 *  @param[in]   uint16_t               validIdx
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgSCHTomUtlFillCqiSrSrsWithHq
(
RgSchCellCb       *cell,
TfuRecpReqInfo    *recpReqInfo,
RgSchUeCb         *ue,
TfuUeRecpReqInfo  *pucchRecpInfo,
uint16_t          validIdx,
Bool               isDatPresOnSecCell
)
{
   RgSchUePCqiCb   *cqiCb;
   RgSchUePCqiCb   *riCb;
   uint8_t         ri; /*To fetch RI value*/
   Bool            willUeRprtCqi;   /* Flag set due to CQI Mask and UE Inactive state (DRX)*/
   Bool            willUeRprtSr = TRUE;   
   TfuAckNackMode  hqFdbkMode;
   uint8_t         numCqiBit;
   uint8_t         totalPucchBits;
   Bool            dropCqi = FALSE;
#ifdef LTEMAC_SPS
   RgSchCmnUlUeSpsInfo   *ulSpsUe = NULL;
#endif
#ifdef EMTC_ENABLE
   RgSchEmtcUeInfo *emtcUe = NULLP;
#endif

   if(ue)
   {
      /*Changes for PUCCH Format3 */
      hqFdbkMode = pucchRecpInfo->t.pucchRecpReq.hqInfo.hqFdbkMode; 
      numCqiBit = rgSCHCmnCalcPcqiBitSz (ue,cell->numTxAntPorts);
      totalPucchBits = pucchRecpInfo->t.pucchRecpReq.hqInfo.hqSz + numCqiBit; 

#ifdef EMTC_ENABLE
      emtcUe = RG_GET_EMTC_UE_CB(ue);
#endif
      rgSCHTomUtlWillUeRprtCqiRi(ue, &willUeRprtCqi);
#ifdef EMTC_ENABLE         /*VINU*/
      if (ue->isEmtcUe)
      {
         if((emtcUe->pucchRepNumFr1 > 1) || (emtcUe->pucchRepNumFr2 > 1))
         {
            willUeRprtCqi = FALSE;
            willUeRprtSr = FALSE;
         }
      } 
#endif
      if(ue->srCb.nSrTrIdx == validIdx)
      {

#ifdef LTEMAC_SPS
        /* Should we check for Rel8 and above???
         * Dont send SR recp req if logicalChannelSR-Mask enabled and UL SPS is
         * active*/
        ulSpsUe =  RG_SCH_CMN_GET_UL_SPS_UE(ue, cell);
        /* Avoiding check for ulSpsEnabled as isUlSpsActv FALSE if sps is not enabled*/
        if(!((ue->ul.ulSpsCfg.isLcSRMaskEnab) &&
           (ulSpsUe->isUlSpsActv)))
        {
#endif

           if(willUeRprtSr)     
           {
         /*Fill SR params*/
         pucchRecpInfo->t.pucchRecpReq.srInfo.n1PucchIdx = 
         ue->srCb.srCfg.srSetup.srResIdx; 
         pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ_SR;
         /* FORMAT3: If SR is present it will be appended after HARQ */
         totalPucchBits = totalPucchBits + 1;
           }

#ifdef LTEMAC_SPS
        }
#endif

         rgSCHTomUtlMoveSrNxtOccasion(cell, ue);
      }
      /* LTE_ADV:: UE will drop CSI during CSI+1BCS if data is present
       * on sec cell(isDatPresOnSecCell)*/
#ifdef LTE_TDD
      if (hqFdbkMode == TFU_ACK_NACK_CHANNEL_SELECTION) 
#else
      if (hqFdbkMode == TFU_UCI_FORMAT_1B_CS) 
#endif
      {
         if (isDatPresOnSecCell == TRUE)
         {
            dropCqi = TRUE;
         }
      }
#ifdef LTE_ADV
#ifndef LTE_TDD
      /* Format 3 Changes : If Hq + SR + CQI bits < 22 and simultaneousAckNackAndCQI-Format3
         is enabled then CQI will be multiplexed with HQ otherwise CQI will be dropped 
         Spec 36.213 Sec 10.1.1 */
      else if (hqFdbkMode == TFU_UCI_FORMAT_3)
      {
         if ((isDatPresOnSecCell == TRUE) && 
               ((!ue->simulAckNackCQIFormat3) || (totalPucchBits > 22)))
         {
            dropCqi = TRUE;
         }
      }
#endif
#endif
      riCb = ue->nPRiCb;
      cqiCb = ue->nPCqiCb;
      if(riCb->nRiTrIdx == validIdx)
      {
         /*ccpu00140578:: Skip the UE if the RI is already processed
          * for PUSCH */
         if(riCb->riRecpPrcsd == FALSE)
         {
            if(riCb->riDist == 0)
            {
               if((riCb->cqiCfg.cqiSetup.sANCQI == TRUE) && (willUeRprtCqi == TRUE)&&
                     (isDatPresOnSecCell == FALSE))
               {
                  /*Fill RI params*/
                  pucchRecpInfo->t.pucchRecpReq.cqiInfo.n2PucchIdx =
                     riCb->cqiCfg.cqiSetup.cqiPResIdx; 
                  pucchRecpInfo->t.pucchRecpReq.cqiInfo.cqiPmiSz = 
                     riCb->riNumBits; 
                  if(pucchRecpInfo->t.pucchRecpReq.uciInfo == TFU_PUCCH_HARQ_SR)
                  {
                     pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ_SR_CQI;
                  }
                  else
                  {
                     pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ_CQI;
                  }
                  ue->rawCqiBitW[ue->cqiRiWritIdx].recvTime = recpReqInfo->timingInfo;
#ifdef LTE_ADV
                  ue->rawCqiBitW[ue->cqiRiWritIdx].u.pucch.sCellIdx =
                     ue->nPRiCb->servCellInfo->sCellIdx;
#endif
                  rgSCHTomUtlFillRiBitWidthInfo(ue);
                  if (ue->nPCqiCb->nCqiTrIdx == validIdx)
                  {
                     rgSCHTomUtlMovePcqiNxtOccasion(cell, ue, ue->nPCqiCb); 
                  } 
#ifdef CA_DBG
                  {
                     if(gF1bCsPres)
                     {
                        gRiReqCount++;
                     } 
                  }

#endif



               }
               rgSCHTomUtlMovePriNxtOccasion(cell, ue, riCb);
            }
            else
            {
               riCb->riDist--;
            }
            /* Skip the UE for RI processing on PUCCH
             * in the same subframe as it already processed */
            if(riCb->nRiTrIdx == validIdx)
            {
               /* As the new idx is same is current idx
                * then PUCCH reception processing will consider
                * RI also in the same subframe. To block this
                * below flag is used*/
               riCb->riRecpPrcsd = TRUE;
            }
         }
      }
      else if(cqiCb->nCqiTrIdx == validIdx)
      {
         if((cqiCb->cqiCfg.cqiSetup.sANCQI == TRUE) && (willUeRprtCqi == TRUE)&&
            (isDatPresOnSecCell == FALSE))
         {
            /*Fill CQI Params*/
            pucchRecpInfo->t.pucchRecpReq.cqiInfo.n2PucchIdx =
               cqiCb->cqiCfg.cqiSetup.cqiPResIdx; 

            ue->rawCqiBitW[ue->cqiRiWritIdx].recvTime = recpReqInfo->timingInfo;
#ifdef LTE_ADV
            ue->rawCqiBitW[ue->cqiRiWritIdx].u.pucch.sCellIdx =
               cqiCb->servCellInfo->sCellIdx;
#endif
            pucchRecpInfo->t.pucchRecpReq.cqiInfo.cqiPmiSz =
               rgSCHTomUtlFetchPcqiBitSz(ue, cell->numTxAntPorts, &ri); 
            if(0 == pucchRecpInfo->t.pucchRecpReq.cqiInfo.cqiPmiSz)
            {
               DU_LOG("\nERROR  -->  SCH : RNTI:%d Unable to Fill CqiPmi size", ue->ueId);
               return RFAILED;
            }
            if(pucchRecpInfo->t.pucchRecpReq.uciInfo == TFU_PUCCH_HARQ_SR)
            {
               pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ_SR_CQI;
            }
            else
            {
               pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ_CQI;
            }
         }
#ifdef CA_DBG
               {
                  if(gF1bCsPres)
                  {
                     gCqiReqCount++;
                  } 
               }

#endif

         rgSCHTomUtlMovePcqiNxtOccasion(cell, ue, cqiCb);           
      }
      if(ue->srsCb.nSrsTrIdx == validIdx)
      {
         /* ccpu00140578::Skip the UE for SRS reception processing
          * if already done as part of PUSCH recpetion 
          * process*/
         if(ue->srsCb.srsRecpPrcsd == FALSE)
         {
            if(ue->srsCb.srsDist ==0 )
            {
               if((pucchRecpInfo->t.pucchRecpReq.uciInfo != TFU_PUCCH_HARQ_CQI) 
                     && (ue->srsCb.srsCfg.srsSetup.sANSrs)
                     && (isDatPresOnSecCell == FALSE))
               {
                  /*Fill SRS params*/
                  pucchRecpInfo->t.pucchRecpReq.srsInfo.srsBw = 
                     (TfuUlSrsBwInfo)ue->srsCb.srsCfg.srsSetup.srsBw;
                  pucchRecpInfo->t.pucchRecpReq.srsInfo.nRrc = 
                     ue->srsCb.srsCfg.srsSetup.fDomPosi;
                  pucchRecpInfo->t.pucchRecpReq.srsInfo.srsHopBw = 
                     (TfuUlSrsHoBwInfo)ue->srsCb.srsCfg.srsSetup.srsHopBw;
                  pucchRecpInfo->t.pucchRecpReq.srsInfo.transComb = 
                     ue->srsCb.srsCfg.srsSetup.txComb;
                  pucchRecpInfo->t.pucchRecpReq.srsInfo.srsCfgIdx = 
                     ue->srsCb.srsCfg.srsSetup.srsCfgIdx;
                  pucchRecpInfo->t.pucchRecpReq.srsInfo.srsCyclicShft = 
                     (TfuUlSrsCycShiftInfo)ue->srsCb.srsCfg.srsSetup.cycShift;
                  /* ccpu00116923 - ADD - New Reception Request types for CQI and SRS with SR */
                  switch(pucchRecpInfo->t.pucchRecpReq.uciInfo)
                  {
                     case TFU_PUCCH_HARQ_SR:
                        pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ_SR_SRS;
                        break;
                     case TFU_PUCCH_HARQ_SR_CQI:
                        pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ_SR_CQI_SRS;
                        break;
                     default:
                        pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ_SRS;
                        break;
                  }
               }
               rgSCHTomUtlMoveSrsNxtOccasion(cell, ue);
            }
            else
            {
               ue->srsCb.srsDist--; 
            }
            /* Skip the UE for SRS processing on PUCCH
             * in the same subframe as it already processed */
            if(ue->srsCb.nSrsTrIdx == validIdx)
            {
               /* As the new idx is same is current idx
                * then PUCCH reception processing will consider
                * SRS also in the same subframe. To block this
                * below flag is used*/
               ue->srsCb.srsRecpPrcsd = TRUE;
            }

         }
      }
   }            
   UNUSED(dropCqi);
   return ROK;
}  /* rgSCHTomUtlFillCqiSrSrsWithHq */

/**
 * @brief  Function which handles the filling of PCQI/RI, SRS 
 *             Reception Request Information along with SR reception 
 *              Request 
 *
 * @details
 *
 *     Function: rgSCHTomUtlFillCqiSrsWithSr
 *
 *              Function which handles the filling of PCQI/RI, SRS 
 *             Reception Request Information along
 *             with the SR reception Request
 *
 *
 *     Invoked by: rgSCHTomUtlFillSrRecpReq of rg_sch_tom.c 
 *
 *     Processing Steps:
 *     - Fill the reception request for CQI/RI, SRS if they occur
 *       in the same instance as of SR. 
 *           
 *  @param[in]   RgSchCellCb       *cell,
 *               RgSchUeCb         *ue
 *               TfuRecpReqInfo    *recpReqInfo,
 *  @param[out]  TfuUeRecpReqInfo  *pucchRecpInfo
 *  @param[in]   uint16_t               validIdx
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef UNUSED_FUNC
static S16 rgSCHTomUtlFillCqiSrsWithSr
(
RgSchCellCb       *cell,
RgSchUeCb         *ue, 
TfuRecpReqInfo    *recpReqInfo,
TfuUeRecpReqInfo  *pucchRecpInfo,
uint16_t          validIdx
)
{
   RgSchUePCqiCb  *cqiCb;
   RgSchUePCqiCb  *riCb;
   uint8_t        ri; /*To fetch RI value*/
   Bool           willUeRprtCqi;   /* Flag set due to CQI Mask and 
                                      UE Inactive state (DRX)*/
   riCb = ue->nPRiCb;
   cqiCb = ue->nPCqiCb;
   rgSCHTomUtlWillUeRprtCqiRi(ue, &willUeRprtCqi);
#ifdef EMTC_ENABLE
   rgSCHEmtcWillUeRptCqi(ue, &willUeRprtCqi);
#endif
   if(riCb->nRiTrIdx == validIdx)
   {
      /*ccpu00140578:: Skip the UE if the RI is already processed
       * for PUSCH */
      if(riCb->riRecpPrcsd == FALSE)
      {
         if(riCb->riDist == 0)
         {
            if(willUeRprtCqi == TRUE)
            {
               /*Fill RI params*/
               pucchRecpInfo->t.pucchRecpReq.cqiInfo.n2PucchIdx =
                  riCb->cqiCfg.cqiSetup.cqiPResIdx; 
               pucchRecpInfo->t.pucchRecpReq.cqiInfo.cqiPmiSz = 
                  riCb->riNumBits; 

               pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_SR_CQI;
               ue->rawCqiBitW[ue->cqiRiWritIdx].recvTime = recpReqInfo->timingInfo;
#ifdef LTE_ADV
               ue->rawCqiBitW[ue->cqiRiWritIdx].u.pucch.sCellIdx =
                  ue->nPRiCb->servCellInfo->sCellIdx;
#endif
               rgSCHTomUtlFillRiBitWidthInfo(ue);
               /* TODO:: syed Shouldn't this be done outside this if condition */
               if (cqiCb->nCqiTrIdx == validIdx)
               {
                  rgSCHTomUtlMovePcqiNxtOccasion(cell, ue, cqiCb); 
               }       
            }
            rgSCHTomUtlMovePriNxtOccasion(cell, ue, riCb);
         }
         else
         {
            riCb->riDist--;
         }
         if(riCb->nRiTrIdx == validIdx)
         {/* Need to skip this UE during PUCCH RI recpetion process
             in the current subframe */
            riCb->riRecpPrcsd = TRUE;
         }
      }
   }
   else if(cqiCb->nCqiTrIdx == validIdx)
   {
      if(willUeRprtCqi == TRUE)
      {
         /*Fill CQI Params*/
         pucchRecpInfo->t.pucchRecpReq.cqiInfo.n2PucchIdx =
            cqiCb->cqiCfg.cqiSetup.cqiPResIdx; 

            ue->rawCqiBitW[ue->cqiRiWritIdx].recvTime = recpReqInfo->timingInfo;

#ifdef LTE_ADV
            ue->rawCqiBitW[ue->cqiRiWritIdx].u.pucch.sCellIdx =
               cqiCb->servCellInfo->sCellIdx;
#endif
            pucchRecpInfo->t.pucchRecpReq.cqiInfo.cqiPmiSz =
               rgSCHTomUtlFetchPcqiBitSz(ue, cell->numTxAntPorts, &ri); 
            if(0 == pucchRecpInfo->t.pucchRecpReq.cqiInfo.cqiPmiSz)
            {
               DU_LOG("\nERROR  -->  SCH : RNTI:%d Unable to Fill CqiPmi size", ue->ueId);
               return RFAILED;
	    }

            pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_SR_CQI;
         }
         rgSCHTomUtlMovePcqiNxtOccasion(cell, ue, cqiCb);  
      }
      if(ue->srsCb.nSrsTrIdx == validIdx)
      {
         /* ccpu00140578:: Cnsider the SRS processing 
          * only if not done in the same TTI 
          * as part of PUSCH or HARQ reception process*/
         if(ue->srsCb.srsRecpPrcsd == FALSE)
         {
            if(ue->srsCb.srsDist ==0 )
            {
               if(ue->srsCb.srsCfg.srsSetup.sANSrs) 
               {
                  /*Fill SRS params*/
                  pucchRecpInfo->t.pucchRecpReq.srsInfo.srsBw = 
                     (TfuUlSrsBwInfo)ue->srsCb.srsCfg.srsSetup.srsBw;
                  pucchRecpInfo->t.pucchRecpReq.srsInfo.nRrc = 
                     ue->srsCb.srsCfg.srsSetup.fDomPosi;
                  pucchRecpInfo->t.pucchRecpReq.srsInfo.srsHopBw = 
                     (TfuUlSrsHoBwInfo)ue->srsCb.srsCfg.srsSetup.srsHopBw;
                  pucchRecpInfo->t.pucchRecpReq.srsInfo.transComb = 
                     ue->srsCb.srsCfg.srsSetup.txComb;
                  pucchRecpInfo->t.pucchRecpReq.srsInfo.srsCfgIdx = 
                     ue->srsCb.srsCfg.srsSetup.srsCfgIdx;
                  pucchRecpInfo->t.pucchRecpReq.srsInfo.srsCyclicShft = 
                     (TfuUlSrsCycShiftInfo)ue->srsCb.srsCfg.srsSetup.cycShift;
                  /* ccpu00116923 - ADD - New Reception Request types for CQI and
                   * SRS with SR */
                  if(pucchRecpInfo->t.pucchRecpReq.uciInfo == TFU_PUCCH_SR_CQI)
                  {
                     pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_SR_CQI_SRS;
                  }
                  else
                  {
                     pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_SR_SRS;
                  }

               }
               rgSCHTomUtlMoveSrsNxtOccasion(cell, ue);
            }
            else
            {
               ue->srsCb.srsDist--; 
            }
            /* Skip the UE for SRS processing on PUCCH
             * in the same subframe as it already processed */
            if(ue->srsCb.nSrsTrIdx == validIdx)
            {
               /* As the new idx is same is current idx
                * then PUCCH reception processing will consider
                * SRS also in the same subframe. To block this
                * below flag is used*/
               ue->srsCb.srsRecpPrcsd = TRUE;
            }

         }
      }
   return ROK;
}  /* rgSCHTomUtlFillCqiSrsWithSr */

#endif 
#endif

#ifdef LTE_TDD
/** @brief This function handles filling of HARQ feedback repetition
 * recption request for each subframe
 *
 * @details
 *
 *     Function: rgSCHTomUtlFillSfRepHqFdbk 
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo  *recpReqInfo
 * @param  [in]  RgSchCellCb     *cell
 * @param  [out] RgSchErrInfo    *err
 * @param  [in]  RgSchDlSf       *dlSf
 * @param  [in]  uint8_t              noFdbks
 * @param  [in]  CmMemListCp     *memCp
 * @param  [in]  uint8_t              elemIdx
 * @param  [in]  RgSchDlSf       *nxtDlsf
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef TFU_UPGRADE
static S16 rgSCHTomUtlFillSfRepHqFdbk
(
TfuRecpReqInfo *recpReqInfo,
RgSchCellCb    *cellCb,
RgSchErrInfo   *err,
RgSchDlSf      *dlSf,
uint8_t        noFdbks,
CmMemListCp    *memCp,
uint8_t        elemIdx,
RgSchDlSf      *nxtDlsf,
uint16_t       validIdx
)
#else
static S16 rgSCHTomUtlFillSfRepHqFdbk
(
TfuRecpReqInfo  *recpReqInfo,
RgSchCellCb     *cellCb,
RgSchErrInfo    *err,
RgSchDlSf       *dlSf,
uint8_t         noFdbks,
CmMemListCp     *memCp,
uint8_t         elemIdx,
RgSchDlSf       *nxtDlsf
)
#endif 
{
   RgSchDlHqProcCb         *hqCb;
   CmLList                 *node;
   S16                     ret;
   RgSchUeCb               *ueCb;
   TfuUeRecpReqInfo        *pucchRecpInfo;
#ifdef TFU_UPGRADE
   TfuUePucchHqRecpInfo    *hqRecpReq;  
#endif
   RgSchDlHqTbCb           *tbCb;
   RgSchDlHqProcCb         *prvHqCb = NULLP;

   node =  dlSf->ackNakRepQ.first;
   while (node)
   {
      tbCb = (RgSchDlHqTbCb *)(node->node);
      hqCb = tbCb->hqP;
      ueCb = hqCb->hqE->ue;

      if (--tbCb->fbkRecpRepCntr)
      {            
         /* Add to next subfarme */
         /* Add this hqCb to the next dlSf's ackNakRepQ */
         cmLListAdd2Tail (&(nxtDlsf->ackNakRepQ), 
               &(tbCb->anRepLnk[tbCb->fbkRecpRepCntr]));
         tbCb->anRepLnk[tbCb->fbkRecpRepCntr].node = (PTR)tbCb;
         tbCb->crntSubfrm[tbCb->fbkRecpRepCntr] = nxtDlsf;
      }

#ifdef TFU_UPGRADE
      if (hqCb->tbCnt) 
      {
         hqCb->tbCnt--;
         /* Go to the next node */
         node = node->next;
         continue;
      }
#endif
      if ((hqCb->hqE->ue != NULLP) &&
            (hqCb->hqE->ue->measGapCb.isMeasuring != TRUE)
            && (hqCb != prvHqCb)
         )
      {
         /* We need to add the recp request to be sent on the pucchANRep
          * value.
          */
         ret = rgSCHUtlGetEventMem((Ptr *)&pucchRecpInfo,
                     sizeof(TfuUeRecpReqInfo), &(recpReqInfo->memCp));
         if (ret != ROK)            
         {
            DU_LOG("\nERROR  -->  SCH : Unable to"
               "Allocate TfuUeRecpReqInfo for RNTI:%d ", ueCb->ueId);
            err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
            return ret;
         }
         pucchRecpInfo->rnti    =  ueCb->ueId;
#ifndef TFU_UPGRADE
         pucchRecpInfo->t.pucchRecpReq.type = TFU_UCI_HARQ;
#else
         pucchRecpInfo->type = TFU_RECP_REQ_PUCCH;
#endif

         /* FOR repetition Feedback shall come on n1PucchAnRep Configured per
          * UE.
          */
#ifndef TFU_UPGRADE
         pucchRecpInfo->t.pucchRecpReq.hqType = TFU_HQ_RECP_REQ_N1PUCCH;
         pucchRecpInfo->t.pucchRecpReq.t.n1Pucch = ueCb->ackNakRepCb.pucchRes;
#else
         pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ;
         hqRecpReq = &(pucchRecpInfo->t.pucchRecpReq.hqInfo);
         /* ACK NACK rep works only in bundling mode . */
         hqRecpReq->hqFdbkMode = (TfuAckNackMode)RGR_TDD_ACKNACK_MODE_BUNDL;
         if ((hqCb->hqPSfLnk.node != NULLP) && 
               (hqCb->hqPSfLnk.node != NULLP))
         {

            hqRecpReq->hqSz = 2;
         }
         else
         {
            hqRecpReq->hqSz = 1;
         }
         hqRecpReq->pucchResCnt = 1; 
         hqRecpReq->hqRes[0] = ueCb->ackNakRepCb.pucchRes; 
#endif
         cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, &(pucchRecpInfo->lnk));
         pucchRecpInfo->lnk.node = (PTR)pucchRecpInfo;
      }
      /* In a given dlSf, if there is 2 TBs context 
       * stored for a given harq, then they are added
       * adjacent to each other in the subframe. To avoid
       * adding duplicate recpnInfo for each TB, store this
       * hqCb in prvHqCb. If nextHqCb is same as prvHqCb then
       * do not add reception req info.*/
      prvHqCb = hqCb;
#ifdef TFU_UPGRADE
      RGSCH_NULL_CHECK(cellCb->instIdx, hqCb->hqE->ue);
      rgSCHTomUtlMoveNxtOccasion(cellCb, hqCb->hqE->ue, validIdx);
#endif 
      /* Go to the next node */
      node = node->next;
   }
   return ROK;
}

/** @brief This function handles filling of HARQ feedback recption request
 * for each subframe 
 *
 * @details 
 *
 *     Function: rgSCHTomUtlFillSfHqFdbkInfo
 *
 *         Processing steps:
 *
 * @param  [out] TfuRecpReqInfo  *recpReqInfo
 * @param  [in]  RgSchCellCb     *cell
 * @param  [out] RgSchErrInfo    *err
 * @param  [in]  RgSchDlSf       *dlSf
 * @param  [in]  uint8_t              noFdbks
 * @param  [in]  CmMemListCp     *memCp
 * @param  [in]  uint8_t              elemIdx
 * @param  [in]  RgSchDlSf       *nxtDlsf
*  @param  [in]  uint16_t             validIdx; 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef TFU_UPGRADE
static S16 rgSCHTomUtlFillSfHqFdbkInfo
(
TfuRecpReqInfo       *recpReqInfo,
RgSchCellCb          *cellCb,
RgSchErrInfo         *err,
RgSchDlSf            *dlSf,
uint8_t              noFdbks,
CmMemListCp          *memCp,
uint8_t              elemIdx,
RgSchDlSf            *nxtDlsf,
uint16_t             validIdx,
RgSchDlHqProcCb      *hqCb,
RgSchUePucchRecpInfo *pucchInfo,
Bool                 alloc,
RgSchDlHqProcCb      *prvHqCb
)
#else
static S16 rgSCHTomUtlFillSfHqFdbkInfo
(
TfuRecpReqInfo       *recpReqInfo,
RgSchCellCb          *cellCb,
RgSchErrInfo         *err,
RgSchDlSf            *dlSf,
uint8_t              noFdbks,
CmMemListCp          *memCp,
uint8_t              elemIdx,
RgSchDlSf            *nxtDlsf,
RgSchDlHqProcCb      *hqCb,
RgSchUePucchRecpInfo *pucchInfo,
Bool                 alloc,
RgSchDlHqProcCb      *prvHqCb
)
#endif
{
   S16                     ret;
   RgSchUeCb               *ueCb = hqCb->hqE->ue;
#ifndef TFU_UPGRADE
   CmLteTimingInfo         futTime;
   RgSchTddANInfo          *anInfo;
#else
#endif
   RgrTddAckNackMode       ackNackMode;
   RgSchDlHqTbCb           *tbCb;
   CmLteRnti               rnti;
   uint8_t                      hqSz = 0;
   uint32_t                     idx = 0;

#ifndef TFU_UPGRADE
   RG_SCH_ADD_TO_CRNT_TIME(cellCb->crntTime, futTime, TFU_RECPREQ_DLDELTA);
#endif


   for (idx = 0 ;idx < 2; idx++)
   {
      if (HQ_TB_WAITING == hqCb->tbInfo[idx].state)
      {

         tbCb = &hqCb->tbInfo[idx];
         if (ueCb)
         {
            rnti = ueCb->ueId;
            ackNackMode = ueCb->dl.ackNackMode;
#ifndef TFU_UPGRADE
            if(ackNackMode == RGR_TDD_ACKNACK_MODE_BUNDL)
            {
               anInfo = rgSCHUtlGetUeANFdbkInfo(ueCb, &futTime);
               /* Only the last scheduled TB for the UE is for HARQ
                * ACK/NACK reception in Bundling case */
               if((anInfo == NULLP) || 
                     (anInfo->latestMIdx != dlSf->dlFdbkInfo.m))
               {
					   return ROK; 
					}
            }
            else
            {
               /* Get the TFU reception request pointer, if present */
               cmHashListFind(&cellCb->ueTfuPendLst, (uint8_t*) &ueCb->ueId,
                     sizeof(ueCb->ueId), 0, (PTR *) &pucchInfo);
            }
#else
            /* For upgrade we shall use the existing logic of pending list. */
            cmHashListFind(&cellCb->ueTfuPendLst, (uint8_t*) &ueCb->ueId,
                  sizeof(ueCb->ueId), 0, (PTR *) &pucchInfo);
#endif
         }
         else if(hqCb->hqE->raCb != NULLP)
         {
            /* For RACH it is set to Bundling */
            ackNackMode = RGR_TDD_ACKNACK_MODE_BUNDL;
            rnti = hqCb->hqE->raCb->tmpCrnti;
         }
         else
         {
			   return ROK;
         }

         /* Do not proceed if PUSCH
            reception req is already filled*/
#ifdef TFU_UPGRADE
         if (hqCb->tbCnt) 
         {
            hqCb->tbCnt--;
            /* Go to the next node */
            continue;
         }
#endif
         if(((ueCb == NULLP) || (ueCb->measGapCb.isMeasuring != TRUE))
               &&(hqCb != prvHqCb)
           )
         {
            TknUInt16       n1PucchTkn = {FALSE, 0};
            RgSchPdcch   *pdcch;
            uint8_t            tbIndx;
            pdcch = tbCb->hqP->pdcch;
#ifdef LTEMAC_SPS
            n1PucchTkn = hqCb->spsN1PucchRes;
#endif
            for (tbIndx = 0; tbIndx < TFU_MAX_TB; tbIndx++)
            {
               if (hqCb->tbInfo[tbIndx].state == HQ_TB_WAITING && 
                     (RGSCH_TIMEINFO_SAME(hqCb->tbInfo[tbIndx].fdbkTime,
                                          recpReqInfo->timingInfo)))
               {
                  hqSz++;
                  hqCb->tbInfo[tbIndx].pucchFdbkIdx = hqCb->ulDai;
               }
            }
            ret = rgSCHTomUtlFillSfHqFdbkForOneUe(hqCb,recpReqInfo, cellCb, err, dlSf, noFdbks,
                  memCp, elemIdx, nxtDlsf, rnti, ackNackMode, &pucchInfo, pdcch,
                  n1PucchTkn, &alloc, hqSz);
            if (ret != ROK)
            {
               return ret;
            }
            /* TODO:: In case of F1BCS and CSI in same subframe
             * UE shall drop the CSI if there was at least one 
             * PDSCH transmission in any of the DL subframe
             * mapping to this UL subframe
             * */
#ifdef TFU_UPGRADE
            rgSCHTomUtlFillCqiSrSrsWithHq(cellCb,recpReqInfo, hqCb->hqE->ue, 
                  pucchInfo->pucchRecpInfo, validIdx,FALSE);  
#ifdef LTE_ADV
            if((hqCb->hqE->ue) &&
               (hqCb->hqE->ue->uciFrmtTyp == RG_SCH_UCI_FORMAT1B_CS))
            {

               if(RG_SCH_IS_CELL_SEC(hqCb->hqE->ue,hqCb->hqE->cell))
               {
                  switch(pucchInfo->pucchRecpInfo->t.pucchRecpReq.uciInfo)
                  {
                     case TFU_PUCCH_HARQ_SR_CQI:
                        pucchInfo->pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ_SR;
                        RG_SCH_DECR_CQIRI_INDEX(ueCb->cqiRiWritIdx);
                        break;
                     case TFU_PUCCH_HARQ_CQI:
                        pucchInfo->pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ;
                        RG_SCH_DECR_CQIRI_INDEX(ueCb->cqiRiWritIdx);
                        break;
                     case TFU_PUCCH_HARQ_SR_CQI_SRS:
                        pucchInfo->pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ_SR_SRS;
                        RG_SCH_DECR_CQIRI_INDEX(ueCb->cqiRiWritIdx);
                        break;
                     case TFU_PUCCH_HARQ_SR_SRS:
                        pucchInfo->pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ_SR;
                        break;
                     case TFU_PUCCH_HARQ_SRS:
                        pucchInfo->pucchRecpInfo->t.pucchRecpReq.uciInfo = TFU_PUCCH_HARQ;
                        break;
                     default:
                        break;
                  }
               }
            }
#endif

#endif 

            /* TODO antz - pushing the following code (under TFU_UPGRADE)
             * into the above function (...ForOneUe) did not work (caused
             * two additional TCs to fail). Don't know why. If this
             * is done later, make sure that the code branch
             * for relPdcch (later in this func) is also modified appropriately.
             */
            /* Now add to the recp request or pending list */
            //if((elemIdx != (noFdbks - 1)))
            {
               cmHashListInsert(&cellCb->ueTfuPendLst, (PTR) pucchInfo,
                     (uint8_t *)&rnti ,(uint16_t) sizeof(CmLteRnti));
               alloc = FALSE;
            }

         } /* If measuring */
         /* Go to the next node */
         if ((tbCb->fbkRecpRepCntr) && (--tbCb->fbkRecpRepCntr))
         {
            /* Add to next subfarme */
            /* Add this hqCb to the next dlSf's ackNakRepQ */
            cmLListAdd2Tail (&(nxtDlsf->ackNakRepQ), 
                  &(tbCb->anRepLnk[tbCb->fbkRecpRepCntr]));
            tbCb->anRepLnk[tbCb->fbkRecpRepCntr].node = (PTR)tbCb;
            tbCb->crntSubfrm[tbCb->fbkRecpRepCntr] = nxtDlsf;
         }
         /* In a given dlSf, if there is 2 TBs context 
          * stored for a given harq, then they are added
          * adjacent to each other in the subframe. To avoid
          * adding duplicate recpnInfo for each TB, store this
          * hqCb in prvHqCb. If nextHqCb is same as prvHqCb then
          * do not add reception req info.*/
         prvHqCb = hqCb;
      }
   }
   return ROK;
}

#ifdef LTE_ADV
/** @brief This function calculates the pucch resource idx 
 *         that is to be filled in harq reception request
 *
 * @details 
 *
 *     Function: rgSCHTomUtlGethqRes
 *
 *         Processing steps:
 *          -Calculate the pucch resource idx 
 *           Harq Reception Request for Format 1B with
 *           Channel Selection
 *
 * @param  [in] uint8_t                      noFdbks
 * @param  [in] RgSchDlSf               *dlSf
 * @param  [in] RgSchPdcch              *pdcch
 * @param  [in] RgSchCellCb             *cellCb
 * @param  [out]uint16_t                     *hqRes
 * @return void
 */
static Void rgSCHTomUtlGethqRes
(
uint8_t       noFdbks,
RgSchDlSf     *dlSf,
RgSchPdcch    *pdcch,
RgSchCellCb   *cellCb,
uint16_t      *hqRes
)
{
   uint8_t    M;
   uint8_t    P;
   uint8_t    m;
   uint8_t    nP;
   uint8_t    nPlusOne;
   uint8_t    nCce;
   
   M = noFdbks;
   m = dlSf->dlFdbkInfo.m;
   nCce = pdcch->nCce;
   P = rgSCHCmnGetPValFrmCCE(cellCb, nCce);
   nP = cellCb->rgSchTddNpValTbl[P];
   nPlusOne = cellCb->rgSchTddNpValTbl[P + 1];
   *hqRes = (M - m - 1)* nP + (m * nPlusOne) + pdcch->nCce + 
      cellCb->pucchCfg.n1PucchAn;

   return;
}

/** @brief This function fills the harq reception request for
 *         TDD in case of Fomat 1B with CS for M=1
 *
 * @details 
 *
 *     Function: rgSCHTomUtlFillSfHqFdbkForFrmt1BCSForM1
 *
 *         Processing steps:
 *          -Fill Harq Reception Request for Format 1B with
 *           Channel Selection
 *
 * @param  [in] RgSchDlHqProcCb         *hqCb
 * @param  [in] TfuUePucchRecpReq       *hqRecpReq
 * @param  [in] uint8_t                      noFdbks
 * @param  [in] RgSchDlSf               *dlSf
 * @param  [in] RgSchPdcch              *pdcch
 * @param  [in] RgSchCellCb             *cellCb
 * @return void
 */
static Void rgSCHTomUtlFillSfHqFdbkForFrmt1BCSForM1
(
RgSchDlHqProcCb     *hqCb,
TfuUePucchRecpReq   *hqRecpReq,
uint8_t             noFdbks,
RgSchDlSf           *dlSf,
RgSchPdcch          *pdcch,
RgSchCellCb         *cellCb
)
{
   RgSchUeCb *ue = NULLP;
   Bool      isCellSec = FALSE;
   uint16_t  hqRes;

   /*ccpu00147920: UeCb is NULL for SPS activation*/
   if(pdcch && pdcch->ue)
   {/* SPS Release pdcch or dynamic data */
      ue = pdcch->ue;
   }else
   {/* SPS occasion */
#ifdef ERRCLS_KW
      if(hqCb == NULLP)
      {
         /* This is not supposed to happen
          * Error case. hqCB has to be ter
          * when pdcch is present . Adding 
          * if check bcs of kwork*/
         return;
      }
#endif
      ue = hqCb->hqE->ue;
   }

   if((hqCb != NULLP) && 
      (RG_SCH_IS_CELL_SEC(ue,hqCb->hqE->cell)))
   {
      isCellSec = TRUE;
   }

   switch(ue->f1bCsAVal)
   {
      case RG_SCH_A_VAL_2:
         /* harq(0) is primary harq(1) is secondary) */
         if(isCellSec)
         {
            hqRecpReq->hqInfo.hqRes[1] = ue->n1PucchF1bResCb.
               cw1N1Res[hqCb->tpc].n1PucchIdx;
         }
         else/* primary cell */
         {
#ifdef LTEMAC_SPS
            /* hqCb will be null in case of sps rel pdcch */
            if ((hqCb) && hqCb->spsN1PucchRes.pres)
            {/* SPS occasion or dyn sched*/
               hqRecpReq->hqInfo.hqRes[0] = hqCb->spsN1PucchRes.val;
            }
            else
#endif /* LTEMAC_SPS */
            {/* dyn data or sps release */
#ifdef ERRCLS_KW
               if(pdcch == NULLP)
               {
                  /* This is not supposed to happen
                   * Error case. hqCB has to be ter
                   * when pdcch is present . Adding 
                   * if check bcs of kwork*/
                  return;
               }
#endif
 
               rgSCHTomUtlGethqRes(noFdbks,dlSf,pdcch,cellCb,&hqRes);
               hqRecpReq->hqInfo.hqRes[0] = hqRes;
            }
         }
         break;
      case RG_SCH_A_VAL_3:
         {
            /* Serving cell in mimo mode should be
             * in 0 and 1 and the serving cell in siso
             * mode should be in 2 indices */
            if(isCellSec)
            {
               uint8_t servCellIdx = rgSchUtlGetServCellIdx(hqCb->hqE->cell->instIdx,
                     hqCb->hqE->cell->cellId,
                     hqCb->hqE->ue);

               if(rgSCHUtlGetMaxTbSupp(ue->cellInfo[servCellIdx]->txMode.txModeEnum) > 1)
               {/* Sec cell is in mimo mode, use 0 and 1 */
                  hqRecpReq->hqInfo.hqRes[0] = 
                     ue->n1PucchF1bResCb.cw1N1Res[hqCb->tpc].n1PucchIdx;
                  hqRecpReq->hqInfo.hqRes[1] = 
                        ue->n1PucchF1bResCb.cw2N1Res[hqCb->tpc].n1PucchIdx;
               }
               else
               {/* Sec cell is in siso mode, use 2 */
                  hqRecpReq->hqInfo.hqRes[2] = 
                     ue->n1PucchF1bResCb.cw1N1Res[hqCb->tpc].n1PucchIdx;
               }   
            }
            else
            {/* primary cell hq */

               if(rgSCHUtlGetMaxTbSupp(ue->mimoInfo.txMode) > 1)
               {/* prim cell is in mimo mode, use 0 and 1 */
#ifdef LTEMAC_SPS
                  if (hqCb && hqCb->spsN1PucchRes.pres)
                  {/* Not sps release */
                     hqRecpReq->hqInfo.hqRes[0] = hqCb->spsN1PucchRes.val;
                  }
                  else
#endif /* LTEMAC_SPS */
                  {/* sps rel or dyn */
#ifdef ERRCLS_KW
                     if(pdcch == NULLP)
                     {
                        /* This is not supposed to happen
                         * Error case. hqCB has to be ter
                         * when pdcch is present . Adding 
                         * if check bcs of kwork*/
                        return;
                     }
#endif

                     rgSCHTomUtlGethqRes(noFdbks,dlSf,pdcch,cellCb,&hqRes);
                     hqRecpReq->hqInfo.hqRes[0] = hqRes;
                     hqRecpReq->hqInfo.hqRes[1] = hqRes + 1;
                  }
               }
               else
               {/* prim cell is in siso mode use 2 */
#ifdef LTEMAC_SPS
                  /* Consider sps occasions */
                  if (hqCb && hqCb->spsN1PucchRes.pres)
                  {/* Not sps release */
                     hqRecpReq->hqInfo.hqRes[2] = hqCb->spsN1PucchRes.val;
                  }
                  else
#endif /* LTEMAC_SPS */
                  {
#ifdef ERRCLS_KW
                     if(pdcch == NULLP)
                     {
                        /* This is not supposed to happen
                         * Error case. hqCB has to be ter
                         * when pdcch is present . Adding 
                         * if check bcs of kwork*/
                        return;
                     }
#endif

                     rgSCHTomUtlGethqRes(noFdbks,dlSf,pdcch,cellCb,&hqRes);
                     hqRecpReq->hqInfo.hqRes[2] = hqRes;
                  }
               }
            }
         }
         break;
      case RG_SCH_A_VAL_4:
         {/* Both the serv cells are in mimo mode */
            if(isCellSec)
            {/* 2 and 3 for sec cell */
               hqRecpReq->hqInfo.hqRes[2] = 
                  ue->n1PucchF1bResCb.cw1N1Res[hqCb->tpc].n1PucchIdx;
               hqRecpReq->hqInfo.hqRes[3] = 
                     ue->n1PucchF1bResCb.cw2N1Res[hqCb->tpc].n1PucchIdx;
            }
            else/* primary cell */
            {/* 0 and 1 are for primary cell */
#ifdef LTEMAC_SPS
               if (hqCb && hqCb->spsN1PucchRes.pres)
               {/* Not sps release */
                  hqRecpReq->hqInfo.hqRes[0] = hqCb->spsN1PucchRes.val;
               }
               else
#endif /* LTEMAC_SPS */
               {
#ifdef ERRCLS_KW
                  if(pdcch == NULLP)
                  {
                     /* This is not supposed to happen
                      * Error case. hqCB has to be ter
                      * when pdcch is present . Adding 
                      * if check bcs of kwork*/
                     return;
                  }
#endif

                  rgSCHTomUtlGethqRes(noFdbks,dlSf,pdcch,cellCb,&hqRes);
                  hqRecpReq->hqInfo.hqRes[0] = hqRes;
                  hqRecpReq->hqInfo.hqRes[1] = hqRes + 1;
               }
            }
         }
         break;
      default:
         break;
   }
   return;
}

/** @brief This function fills the harq reception request for
 *         TDD in case of Fomat 1B with CS for M>=2
 *
 * @details 
 *
 *     Function: rgSCHTomUtlFillSfHqFdbkForFrmt1BCSForM234
 *
 *         Processing steps:
 *          -Fill Harq Reception Request for Format 1B with
 *           Channel Selection
 *
 * @param  [in] RgSchDlHqProcCb         *hqCb
 * @param  [in] TfuUePucchRecpReq       *hqRecpReq
 * @param  [in] uint8_t                      noFdbks
 * @param  [in] RgSchDlSf               *dlSf
 * @param  [in] RgSchPdcch              *pdcch
 * @param  [in] RgSchCellCb             *cellCb
 * @param  [in] uint8_t                      elemIdx
 * @return void
 */
static Void rgSCHTomUtlFillSfHqFdbkForFrmt1BCSForM234
(
RgSchDlHqProcCb    *hqCb,
TfuUePucchRecpReq  *hqRecpReq,
uint8_t            noFdbks,
RgSchDlSf          *dlSf,
RgSchPdcch         *pdcch,
RgSchCellCb        *cellCb,
uint8_t            elemIdx
)
{
   RgSchUeCb  *ue;
   Bool       isCellSec = FALSE;
   uint16_t   hqRes = 0;
   uint8_t    servCellIdx;

   if(pdcch)
   {/* SPS Release pdcch or dynamic data */
      ue = pdcch->ue;
   }else
   {/* SPS occasion */
#ifdef ERRCLS_KW
      if(hqCb == NULLP)
      {
         /* This is not supposed to happen
          * Error case. hqCB has to be ter
          * when pdcch is present . Adding 
          * if check bcs of kwork*/
         return;
      }
#endif
      ue = hqCb->hqE->ue;
   }

   if((hqCb != NULLP) && (ue != NULLP) &&  
      (RG_SCH_IS_CELL_SEC(ue,hqCb->hqE->cell)))
   {
      isCellSec = TRUE;
   }   

   if(isCellSec)
   {/* Sec Cell indices are 2 and 3*/
      servCellIdx = rgSchUtlGetServCellIdx(hqCb->hqE->cell->instIdx,
                                           hqCb->hqE->cell->cellId,
                                           hqCb->hqE->ue);

      hqRecpReq->hqInfo.hqRes[2] = 
         ue->n1PucchF1bResCb.cw1N1Res[hqCb->tpc].n1PucchIdx;

      if(rgSCHUtlGetMaxTbSupp(ue->cellInfo[servCellIdx]->txMode.txModeEnum) > 1)
      {
         hqRecpReq->hqInfo.hqRes[3] = 
            ue->n1PucchF1bResCb.cw2N1Res[hqCb->tpc].n1PucchIdx;
      }
   }
   else
   {/* Primary cell indices are 0 and 1 */
      /* SPS occasions
       * M > 2 if SPS occasion is present in any of the 
       * DL subframe in the bundle, the n1Pucch(0) is
       * the SPS resource and n1Pucch(1) is the resource
       * derived from pdcch with DAI = 1
       * If No SPS Occasion
       * Then n1Pucch(0) is from pdcch with DAI =1
       * and n1Pucch(1) is from pdcch with DAI = 2
       * */

      if(hqCb != NULLP)
      {/* this is not sps release pdcch */
         if(hqCb->spsN1PucchRes.pres == TRUE)
         {/* SPS occasion*/
            hqRes = hqCb->spsN1PucchRes.val;
         }
      }

      if(pdcch)
      {/*Dynamic scheduling or SPS Release
         Derive from pdcch */
         if(pdcch->dlDai < 3)
         {/* No need to calcualte from DAI > 2 */
            rgSCHTomUtlGethqRes(noFdbks,dlSf,pdcch,cellCb,&hqRes);
         }
      }

      if(2 == noFdbks)
      {/* M == 2 case */
         hqRecpReq->hqInfo.hqRes[elemIdx] = hqRes; 
      }
      else
      {/* Pdcch with DAI = 1 and 2 needs to be used
          for resource calculation*/
         if(hqCb && hqCb->spsN1PucchRes.pres == TRUE)
         {/* dyn or sps occasion */
            /* Shift the hqRes[0] if it was filled
             * if there was a pdcch with DAI 1 before to this 
             * subframe*/
            if(hqCb->ulDai > 1)
            {/* SPS occasion happened in the middle 
                of the bundle */
               /* shifting the non SPS resource to n1Pucch(1) */
               hqRecpReq->hqInfo.hqRes[1] = hqRecpReq->hqInfo.hqRes[0]; 
            }

            hqRecpReq->hqInfo.hqRes[0] = hqRes; 
         }
#ifdef ERRCLS_KW
         else if(pdcch && pdcch->dlDai < 3)
#else
         else if(pdcch->dlDai < 3)
#endif
         {/* sps rel or dyn sched */
            /* hqCb wil not be present for sps release pdcch */
            if(hqCb && (pdcch->dlDai != hqCb->ulDai))
            {/* there was a SPS occasion before to this */
               if(pdcch->dlDai == 1)
               {
                  hqRecpReq->hqInfo.hqRes[1] = hqRes; 
               }/* ignore the DAI 2 in this case */
            }else
            {/* There was no SPS occasion before to this */
#ifdef ERRCLS_KW
               if(pdcch->dlDai)
#endif
               {/* Added check to ignore kwork warning */
                  hqRecpReq->hqInfo.hqRes[(pdcch->dlDai)-1] = hqRes; 
               }
            }
         }
      }
   }
   return;
}

/** @brief This function fills the harq reception request for
 *         TDD in case of Fomat 1B with CS
 *
 * @details 
 *
 *     Function: rgSCHTomUtlFillSfHqFdbkForFrmt1BCS
 *
 *         Processing steps:
 *          -Fill Harq Reception Request for Format 1B with
 *           Channel Selection
 *
 * @param  [in] RgSchDlSf     *ulSf
 * @param  [in] RgSchCellCb   *cell
 * @param  [out]TfuUePucchRecpReq *hqRecpReq
 * @return S16
 */
static S16 rgSCHTomUtlFillSfHqFdbkForFrmt1BCS
(
RgSchDlHqProcCb    *hqCb,
TfuUePucchRecpReq  *hqRecpReq,
uint8_t            noFdbks,
RgSchDlSf          *dlSf,
RgSchPdcch         *pdcch,
uint8_t            elemIdx,
RgSchCellCb        *cellCb
)
{
   /* Update teh fdbk mode if something different is present
    * in L1 API file for F1BS *//* 1 --> F1BCS */
   hqRecpReq->hqInfo.hqFdbkMode = TFU_ACK_NACK_CHANNEL_SELECTION;

   switch(noFdbks)
   {/* M Value */
      case RG_SCH_M_VAL_1:
         {

            rgSCHTomUtlFillSfHqFdbkForFrmt1BCSForM1(hqCb,hqRecpReq,
                  noFdbks,dlSf,pdcch,cellCb);
            break;
         }
      case RG_SCH_M_VAL_2:
      case RG_SCH_M_VAL_3:
      case RG_SCH_M_VAL_4:
         {
            /* Spatial bundling will be applied */
            rgSCHTomUtlFillSfHqFdbkForFrmt1BCSForM234(hqCb,hqRecpReq,
                  noFdbks,dlSf,pdcch,cellCb,elemIdx);
            break;
         }
      default:
         break;
   }
   return ROK;
}
#endif

/***********************************************************
 *
 *     Func : rgSCHTomUtlFillSfHqFdbkForOneUe
 *
 *     Desc : Fill HARQ feedback info for one UE/entry
 *
 *     Ret  : ROK/RFAILED
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHTomUtlFillSfHqFdbkForOneUe
( 
RgSchDlHqProcCb         *hqCb,
TfuRecpReqInfo          *recpReqInfo,
RgSchCellCb             *cellCb,
RgSchErrInfo            *err,
RgSchDlSf               *dlSf,
uint8_t                 noFdbks,
CmMemListCp             *memCp,
uint8_t                 elemIdx,
RgSchDlSf               *nxtDlsf,
CmLteRnti               rnti,
RgrTddAckNackMode       ackNackMode,
RgSchUePucchRecpInfo    **pucchInfoRef,
RgSchPdcch              *pdcch,
TknUInt16               n1PucchTkn,
Bool                    *allocRef,
uint8_t                 hqSz  
)
{
   RgSchUePucchRecpInfo    *pucchInfo = *pucchInfoRef;
   Bool                    alloc = FALSE;
   S16                     ret;
   TfuUePucchRecpReq       *hqRecpReq;
#ifdef TFU_UPGRADE
   uint8_t                 M;
   uint8_t                 P;
   uint8_t                 m;
   uint8_t                 nP;
   uint8_t                 nPlusOne;
   uint16_t                pucchRes;
   uint8_t                 resIdx;
   uint8_t                 nCce;
   uint8_t                 prevHqSize;
#else
   uint8_t                 multCnt;
#endif
#ifdef LTEMAC_SPS
   Bool                    isFirstFdbk = FALSE;
#endif
   if(pucchInfo == NULLP)
   {
      if ((ret = rgSCHUtlGetEventMem((Ptr *)&pucchInfo,
                  sizeof(RgSchUePucchRecpInfo), memCp)) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : Unable to "
            "Allocate TfuUeRecpReqInfo for cell RNTI:%d",rnti);
         err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
         return ret;
      }
      alloc = TRUE;
#ifdef TFU_ALLOC_EVENT_NO_INIT
      pucchInfo->hashLstEnt.hashVal = 0;
      pucchInfo->hashLstEnt.keyLen = 0;
      pucchInfo->hashLstEnt.key = 0;
      pucchInfo->hashLstEnt.list.prev = pucchInfo->hashLstEnt.list.next = 0;
#endif
      if ((ret = rgSCHUtlGetEventMem((Ptr *)&(pucchInfo->pucchRecpInfo),
                  sizeof(TfuUeRecpReqInfo), &(recpReqInfo->memCp))) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : Unable to "
            "Allocate TfuUeRecpReqInfo for cell RNTI:%d",rnti);
         err->errCause = RGSCHERR_TOM_MEM_EXHAUST;
         return ret;
      }
      memset(pucchInfo->pucchRecpInfo->t.pucchRecpReq.hqInfo.hqRes,0xff,sizeof(uint16_t)*TFU_MAX_HQ_RES);
#ifdef TFU_ALLOC_EVENT_NO_INIT
      memset(&pucchInfo->pucchRecpInfo->t.pucchRecpReq, 0, sizeof(TfuUePucchRecpReq));
#endif
      pucchInfo->pucchRecpInfo->type = TFU_RECP_REQ_PUCCH;
      pucchInfo->pucchRecpInfo->rnti = rnti;
#ifdef TFU_UPGRADE
      pucchInfo->pucchRecpInfo->t.pucchRecpReq.uciInfo=TFU_PUCCH_HARQ;
#endif
#ifdef LTEMAC_SPS
      isFirstFdbk = TRUE;
#endif
   }
   /* Fill m, M, P */
   /* Calculation of resources same for both bundling and muxing for M = 1
    * */
#ifdef LTE_ADV
    RgSchUeCb     *ue = rgSCHDbmGetUeCb (cellCb, rnti);
    if((ue) && (1 == ue->numSCells))
    {
       if(ue->uciFrmtTyp == RG_SCH_UCI_FORMAT1B_CS)
       {
          hqRecpReq = &(pucchInfo->pucchRecpInfo->t.pucchRecpReq);
          rgSCHTomUtlFillSfHqFdbkForFrmt1BCS(hqCb,hqRecpReq,
                noFdbks,dlSf,pdcch,elemIdx,cellCb);

          if(noFdbks == 1)
          {/* M = 1 case . size is same as A Value*/
               hqRecpReq->hqInfo.hqSz = ue->f1bCsAVal;
               hqRecpReq->hqInfo.pucchResCnt = hqRecpReq->hqInfo.hqSz;
          }else
          {/* M > 1 case */
            hqRecpReq->hqInfo.hqSz = (noFdbks * 2); /* M for 2 cells */
            hqRecpReq->hqInfo.pucchResCnt = 4;
          }
          hqRecpReq->hqInfo.a = ue->f1bCsAVal;
          /* handling for SPS occasions*/
          if(elemIdx == 0)
          {
             /* set the datPresinFirstSUbframe to TRUE if this 
              * is for pcell txion*/
#ifdef ERRCLS_KW
             RgSchTddANInfo      *anInfo = NULLP;
#endif
             /* if this txion is on pcell
              * sps occaion, dyn sched or sps release pdcch
              * set the sched present in first 
              * dl subframe of the bundle to TRUE. This 
              * is required for mapping the feedbak when SPS occasion
              * is present in any of the DL subframe in the bundle in 
              * case of M > 2*/

              /* SPS will happen only on pcell */
             if((hqCb == NULLP) || (!RG_SCH_IS_CELL_SEC(ue,hqCb->hqE->cell)))
             {
#ifdef ERRCLS_KW
                 anInfo = rgSCHUtlGetUeANFdbkInfo(ue,
                      &recpReqInfo->timingInfo,RGSCH_PCELL_INDEX);
                if(anInfo == NULL)
                {/* ANInfo must be there. adding block
                    because of kworks*/
                   DU_LOG("\nERROR  -->  SCH : ANInfo should not be NULL for cellId=%d \n", cellCb->cellId);
                   return RFAILED;
                }
#endif
             }
         }
       }else
       {/* This needs to be revisited while
           adding support for PUCCH format 3 */
          DU_LOG("\nERROR  -->  SCH : Invalid Pucch format configured..");
          return RFAILED;
       }
    }
    else
#endif
    {
       if((ackNackMode == RGR_TDD_ACKNACK_MODE_BUNDL) || 
             ((noFdbks == 1) && (ackNackMode == RGR_TDD_ACKNACK_MODE_MULT)))
       {
          hqRecpReq = &(pucchInfo->pucchRecpInfo->t.pucchRecpReq);
#ifdef TFU_UPGRADE
          prevHqSize =  hqRecpReq->hqInfo.hqSz;
#endif
#ifndef TFU_UPGRADE
          /* Only one index for bundling case */
          hqRecpReq->M = noFdbks;
          hqRecpReq->hqType =
             TFU_HQ_RECP_REQ_NORMAL;
          hqRecpReq->multCnt = 1;
          hqRecpReq->t.nCce[0] = 
             pdcch->nCce;
          hqRecpReq->m[0] = 
             dlSf->dlFdbkInfo.m;
          hqRecpReq->p[0] =
             rgSCHCmnGetPValFrmCCE(cellCb, pdcch->nCce);

          hqRecpReq->type = TFU_UCI_HARQ;

#else /* TFU_UPGRADE */

#ifdef LTEMAC_SPS
          if ((TRUE == isFirstFdbk) && (TRUE == n1PucchTkn.pres))
          {
             hqRecpReq->hqInfo.hqFdbkMode = (TfuAckNackMode)ackNackMode;
             hqRecpReq->hqInfo.pucchResCnt=1;
             hqRecpReq->hqInfo.hqRes[0] = n1PucchTkn.val;
             hqRecpReq->hqInfo.hqSz = hqSz;
          }
          /* ccpu00139413 */
          else if (FALSE == n1PucchTkn.pres)
#endif
          {
             hqRecpReq->hqInfo.hqFdbkMode = (TfuAckNackMode)ackNackMode;
             M = noFdbks;
             P = rgSCHCmnGetPValFrmCCE(cellCb, pdcch->nCce);
             nP = cellCb->rgSchTddNpValTbl[P];
             nPlusOne = cellCb->rgSchTddNpValTbl[P + 1];
             m = dlSf->dlFdbkInfo.m;
             /* In case of no UE */
             pucchRes = (M - m - 1)* nP + (m * nPlusOne) + pdcch->nCce + 
                cellCb->pucchCfg.n1PucchAn;
             /*ccpu00130164:MOD-Changed to maitain value of
               hqRecpReq->hqInfo.pucchResCnt=1 in case of bundling*/
             /*ccpu00132284 -MOD- hqRes need to be updated after pucchReCnt set to 1
              * and resource should be update at index-0*/
             hqRecpReq->hqInfo.pucchResCnt=1;
             hqRecpReq->hqInfo.hqRes[hqRecpReq->hqInfo.pucchResCnt-1] = pucchRes;

             if((ackNackMode == RGR_TDD_ACKNACK_MODE_BUNDL) && (hqSz > prevHqSize))
                hqRecpReq->hqInfo.hqSz = hqSz;
             else if (ackNackMode == RGR_TDD_ACKNACK_MODE_MULT)
                hqRecpReq->hqInfo.hqSz = hqSz;
             else
                hqRecpReq->hqInfo.hqSz = prevHqSize;
          }
#endif /* TFU_UPGRADE */
#ifndef TFU_UPGRADE
          cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, 
                &(pucchInfo->pucchRecpInfo->lnk));
          pucchInfo->pucchRecpInfo->lnk.node = 
             (PTR)pucchInfo->pucchRecpInfo;
#endif
       }
       else /* Multiplexing */
       {
#ifndef TFU_UPGRADE
          pucchInfo->pucchRecpInfo->t.pucchRecpReq.M = noFdbks;
#ifdef LTEMAC_SPS
          if (n1PucchTkn.pres == TRUE)
          {
             pucchInfo->pucchRecpInfo->t.pucchRecpReq.hqType =
                TFU_HQ_RECP_REQ_N1PUCCH;
             pucchInfo->pucchRecpInfo->t.pucchRecpReq.t.n1Pucch = n1PucchTkn.val;
          }
          else
#endif
          {
             pucchInfo->pucchRecpInfo->t.pucchRecpReq.hqType =
                TFU_HQ_RECP_REQ_NORMAL;
             multCnt = pucchInfo->pucchRecpInfo->t.pucchRecpReq.multCnt;
             pucchInfo->pucchRecpInfo->t.pucchRecpReq.t.nCce[multCnt] = 
                pdcch->nCce;
             pucchInfo->pucchRecpInfo->t.pucchRecpReq.m[multCnt] =
                dlSf->dlFdbkInfo.m;
             pucchInfo->pucchRecpInfo->t.pucchRecpReq.p[multCnt] =
                rgSCHCmnGetPValFrmCCE(cellCb, pdcch->nCce);

             pucchInfo->pucchRecpInfo->t.pucchRecpReq.multCnt++;
          }
#else /* TFU_UPGRADE */

          hqRecpReq = &(pucchInfo->pucchRecpInfo->t.pucchRecpReq);
          hqRecpReq->hqInfo.hqFdbkMode = (TfuAckNackMode)RGR_TDD_ACKNACK_MODE_MULT;
          hqRecpReq->hqInfo.hqSz = noFdbks;

          resIdx = hqRecpReq->hqInfo.pucchResCnt;
          hqRecpReq->hqInfo.pucchResCnt++;

#ifdef LTEMAC_SPS
          if (n1PucchTkn.pres == TRUE)
          {
             hqRecpReq->hqInfo.hqRes[resIdx] = n1PucchTkn.val;
          }
          else
#endif
          {
             M = noFdbks;
             m = dlSf->dlFdbkInfo.m;
             nCce = pdcch->nCce;
             P = rgSCHCmnGetPValFrmCCE(cellCb, nCce);
             nP = cellCb->rgSchTddNpValTbl[P];
             nPlusOne = cellCb->rgSchTddNpValTbl[P + 1];
             hqRecpReq->hqInfo.hqRes[resIdx] = (M - m - 1)* nP + 
                (m * nPlusOne) + pdcch->nCce + 
                cellCb->pucchCfg.n1PucchAn;
          }
#endif /* TFU_UPGRADE */
          /* If all the DL subframes are scanned, then
           * send TFU request*/
#ifndef TFU_UPGRADE
          if((elemIdx != noFdbks) && alloc)
          {
             cmHashListInsert(&cellCb->ueTfuPendLst, (PTR) pucchInfo,
                   (uint8_t *)&rnti, (uint16_t) sizeof(rnti));
             alloc = FALSE;
          }
          else
          {
             pucchInfo->pucchRecpInfo->t.pucchRecpReq.type = TFU_UCI_HARQ;
             cmLListAdd2Tail(&recpReqInfo->ueRecpReqLst, 
                   &(pucchInfo->pucchRecpInfo->lnk));
             pucchInfo->pucchRecpInfo->lnk.node = 
                (PTR)pucchInfo->pucchRecpInfo;
             /* Delete the entry after addition to the list */
             cmHashListDelete(&cellCb->ueTfuPendLst, (PTR) pucchInfo);
          }
#endif
       }
    }

   *pucchInfoRef = pucchInfo;
   *allocRef     = alloc;
   return ROK;
}
#endif

#ifdef RG_ULSCHED_AT_CRC
/** @brief This function does all the processing related to a single downlink
 * subframe.
 *
 * @details 
 *
 *     Function: rgSCHTomUtlProcDlSfAtCrc
 *
 *         Processing steps:
 *         - collate control data for all UEs and send to PHY 
 *         - collate data buffers for all UEs and send to PHY 
 *
 * @param  [in] RgSchDlSf     *ulSf
 * @param  [in] RgSchCellCb   *cell
 * @param  [in] TfuCntrlReqInfo *cntrlInfo
 * @param  [out] RgSchErrInfo *err
 * @return S16
 */
static S16 rgSCHTomUtlProcDlSfAtCrc
(
RgSchDlSf            *ulSf,
CmLteTimingInfo      crntUlFrm,
RgSchCellCb          *cell,
TfuCntrlReqInfo      *cntrlInfo,
RgSchErrInfo         *err
)
{
   Inst              inst = cell->instIdx;
   S16               ret;

   cntrlInfo->numDlActvUes = 0;
   cmLListInit(&cntrlInfo->phichLst);
   cmLListInit(&cntrlInfo->dlPdcchLst);
   cmLListInit(&cntrlInfo->ulPdcchLst);
#ifdef TFU_ALLOC_EVENT_NO_INIT
   cntrlInfo->dlTiming.sfn = cntrlInfo->dlTiming.subframe = 0;
   cntrlInfo->cfi = 0;
#endif

   cntrlInfo->ulTiming = crntUlFrm;
   RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, cntrlInfo->ulTiming, TFU_ULCNTRL_DLDELTA);

   cntrlInfo->cellId = cell->cellId;
   /* Fill PHICH info */
   if ((ret = rgSCHTomUtlFillPhich (cell, cntrlInfo, ulSf, err)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Unable to send PHICH info for cell");
      RGSCH_FREE_MEM(cntrlInfo);
      return ret;
   }

   /* Fill UL Pdcch */
   if ((ret = rgSCHTomUtlFillUlPdcch (cell, cntrlInfo, ulSf, err)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Unable to send PDCCH info for cell");
      RGSCH_FREE_MEM(cntrlInfo);
      return ret;
   }
   
#ifdef EMTC_ENABLE
   if(0 == cntrlInfo->ulMpdcchLst.count)
   {
      gUlMpdcchBlank++;
   }
#endif

#ifdef EMTC_ENABLE
   if ((cntrlInfo->ulPdcchLst.count || cntrlInfo->phichLst.count) || RG_SCH_EMTC_GET_PDCCHLST_CNT(cntrlInfo))
#else
   if (cntrlInfo->ulPdcchLst.count || cntrlInfo->phichLst.count)
#endif
   {
      //if (rgSCHUtlTfuCntrlReq(inst, cell->tfuSap->sapCfg.suId, cntrlInfo) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : Unable to send Cntrl info for cell");
      }
   }
   else
   {
      RGSCH_FREE_MEM(cntrlInfo);
   }
   return ROK;
} /* end of */ 
#endif /* #ifdef RG_ULSCHED_AT_CRC*/

#ifdef RGR_RRM_TICK
/** @brief This function sends the SFN Tick to L3  
 * subframe.
 *
 * @details 
 *
 *     Function: rgSCHTomUtlSendSfnTick
 *
 * @param  [in] RgSchCellCb   *cell
 */
static Void rgSCHTomUtlSendSfnTick
(
RgSchCellCb          *cell
)
{
   RgrTtiIndInfo     *rgrTtiInd;
   
   /* TTI to be sent to RRM only once per system frame */
   /* Added support for period = 0 to disable tick to RRM */
   if ((cell->rrmTtiIndPrd != 0) && 
         ((cell->crntTime.sfn % cell->rrmTtiIndPrd) == 0) && 
         (cell->crntTime.slot == 0))
   {
      /* Allocate a TTI indication structure and send to RRM over RGR interface */
      if (rgSCHUtlAllocSBuf (cell->instIdx,
               (Data**)&rgrTtiInd, sizeof(RgrTtiIndInfo)) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : Mem alloc failed for RGR TTI ind, cellId (%d))\n", 
                  cell->cellId);
         return;
      }
      rgrTtiInd->cellId = cell->cellId;
      //rgrTtiInd->hSfn = cell->crntTime.hSfn;
      rgrTtiInd->sfn    = cell->crntTime.sfn;

      if (rgSCHUtlRgrTtiInd (cell, rgrTtiInd) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : Failed to send RGR TTI ind, cellId (%d))\n", 
                  cell->cellId);
         rgSCHUtlFreeSBuf(cell->instIdx, (Data**)&rgrTtiInd,
               sizeof(RgrTtiIndInfo));
         return;
      }
   }
   return;
}
#endif

#ifdef RG_5GTF

/* @brief Mark Dyn TDD CrntSfIdx.
 *
 * @details
 *
 *     Function: rgSCHDynTDDMrkCrntSfIdx 
 *     Purpose:  update the dyn tdd sunframe index
 *  @param[in]  Inst   schInst
 *  @return    None
 */
#ifdef UNUSED_FUNC
static Void rgSCHDynTDDMrkCrntSfIdx(Inst   schInst)
{
   RgSchDynTddCb  *rgSchDynTddInfo = &(rgSchCb[schInst].rgSchDynTdd);


	RG_SCH_DYN_TDD_MARKTYPE(rgSchDynTddInfo, rgSchDynTddInfo->crntDTddSfIdx, 
	                      RG_SCH_DYNTDD_NOTDEF);
	rgSchDynTddInfo->crntDTddSfIdx = (rgSchDynTddInfo->crntDTddSfIdx + 1) %
		                   RG_SCH_DYNTDD_MAX_SFINFO;

   //DU_LOG("\nERROR  -->  SCH : Initializing Index %d \n", rgSchDynTddInfo->crntDTddSfIdx);

   return;
}
#endif
#endif
/** @brief This function fills the TTI timinig info for each cell  
 *
 * @details 
 *
 *     Function: rgSchTomFillCellTtiInfo
 *
 * @param  [in]  TfuTtiIndInfo  *ttiInd
 * @param  [in]  Inst           schInst
 * @param  [out] uint8_t             *nCell
 * @param  [out] RgSchCellCb    *cell[]
 *
 * Returns: Void  
 *  
 */
#ifdef UNUSED_FUNC
static Void rgSchTomFillCellTtiInfo
(
TfuTtiIndInfo      *ttiInd,
Inst               schInst,
uint8_t            *nCell,
RgSchCellCb        *cells[]
)
{
   uint8_t         i = 0;
   uint8_t         strtCellId;
   TfuTtiCellInfo  *cellInfo;
   RgSchCellCb     *cell; 
   uint32_t        Idx1;
 
   CmLteTimingInfo frm;
     
   if (CM_LTE_MAX_CELLS < ttiInd->numCells)
   {
      return;
   }
	
#ifdef RG_5GTF
   rgSCHDynTDDMrkCrntSfIdx(schInst);
#endif

   for (i = 0; i < ttiInd->numCells; i++)
   {
      cellInfo   = &ttiInd->cells[i];
      strtCellId = rgSchCb[schInst].genCfg.startCellId;
      Idx1 = (uint8_t)((cellInfo->cellId - strtCellId)&(CM_LTE_MAX_CELLS-1));
      cell = rgSchCb[schInst].cells[Idx1];
      /* Validate the cell */
      if (cell == NULLP) 
      {
         /* Use SCH inst 0 print buff */
        DU_LOG("\nERROR  -->  SCH : RgLiTfuTtiInd()No cell exists for cellId %d\n", 
                  cellInfo->cellId));
         continue;
      }
      *nCell = *nCell + 1;
      cells[i] = (RgSchCellCb *)cell;

      /* 4UE_TTI_DELTA */
      if(cell->schTickDelta != cellInfo->schTickDelta)
      {
         DU_LOG("\nERROR  -->  SCH : Delta changed for cellId=%d: curr delta=%d new delta=%d\n"
         "dlblankSf=%d ulblankSf=%d dummyTti=%d \n",
         cell->cellId, cell->schTickDelta, cellInfo->schTickDelta, cellInfo->dlBlankSf,cellInfo->ulBlankSf,
         cellInfo->isDummyTti);
      }
      RGSCH_UPDATE_DELTA(schInst, cellInfo->schTickDelta);
      cell->schTickDelta = cellInfo->schTickDelta;
      /* 4UE_TTI_DELTA */

      cell->stopSiSch    = cellInfo->dlBlankSf;
      cell->stopDlSch    = cellInfo->dlBlankSf;
      cell->stopUlSch    = cellInfo->ulBlankSf;
      if (cellInfo->isDummyTti)
      {
         cell->stopDlSch = TRUE;
      }
      if((0 == (cellInfo->timingInfo.sfn % 30)) && (0 == cellInfo->timingInfo.slot))
      {
	 //DU_LOG("5GTF_CHECK rgSCHTOMTtiInd (%d : %d)\n", cellInfo->timingInfo.sfn, cellInfo->timingInfo.slot);
      }
#ifndef EMTC_ENABLE 
      RGSCHCPYTIMEINFO(cellInfo->timingInfo, cell->crntTime);
      RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, cell->hiDci0Time, 
                              TFU_ULCNTRL_DLDELTA);
      RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, cell->dlDciTime, 
                              TFU_DLCNTRL_DLDELTA);
      RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, cell->rcpReqTime, 
                              TFU_RECPREQ_DLDELTA);
      RGSCHDECRFRMCRNTTIME(cell->crntTime, cell->hqRlsTime, 
                              TFU_HQFBKIND_ULDELTA); 
      RGSCHDECRFRMCRNTTIME(cell->crntTime, cell->dlSfRlsTime, 
                              RGSCH_RLS_SF_IDX);
#else
      RGSCHCPYTIMEINFO_EMTC(cellInfo->timingInfo, cell->crntTime);
      RG_SCH_ADD_TO_CRNT_TIME_EMTC(cell->crntTime, cell->hiDci0Time, 
                              TFU_ULCNTRL_DLDELTA);
      RG_SCH_ADD_TO_CRNT_TIME_EMTC(cell->crntTime, cell->dlDciTime, 
                              TFU_DLCNTRL_DLDELTA);
      RG_SCH_ADD_TO_CRNT_TIME_EMTC(cell->crntTime, cell->rcpReqTime, 
                              TFU_RECPREQ_DLDELTA);
      RGSCHDECRFRMCRNTTIME_EMTC(cell->crntTime, cell->hqRlsTime, 
                              TFU_HQFBKIND_ULDELTA); 
      RGSCHDECRFRMCRNTTIME_EMTC(cell->crntTime, cell->dlSfRlsTime, 
                              RGSCH_RLS_SF_IDX);
#endif
      rgSCHCmnUpdVars(cell);
      cell->isDlDataAllwd = TRUE;
/* Get DownLink SubFrame */
      RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
      frm   = cell->crntTime;
#ifndef EMTC_ENABLE 
      RGSCH_INCR_SUB_FRAME(frm, RG_SCH_CMN_DL_DELTA);
#else
      RGSCH_INCR_SUB_FRAME_EMTC(frm, RG_SCH_CMN_DL_DELTA);
#endif
      cellSch->dl.time = frm; 

#ifdef RG_PFS_STATS
      cell->totalTime++;
#endif
#ifdef LTE_TDD
      uint8_t idx = (cell->crntTime.slot + RG_SCH_CMN_DL_DELTA) %
         RGSCH_NUM_SUB_FRAMES_5G;       
      
      cell->isDlDataAllwd = RG_SCH_CMN_CHK_DL_DATA_ALLOWED(cell, idx);
      
      /*ccpu00130639 -ADD - used in UL HARQ proc id calculation*/
      if((cell->crntTime.sfn == 0) && (cell->crntTime.slot == 0))
      {
         /* sfn Cycle used for Tdd UL Harq Proc Determination. 
            This sfn Cycle will have values from 0 to numUl Harq-1. */
         cell->tddHqSfnCycle = (cell->tddHqSfnCycle + 1 ) %
            (rgSchTddUlNumHarqProcTbl[cell->ulDlCfgIdx]);
      }
#endif      
#ifdef EMTC_ENABLE
      if(cell->emtcEnable)
      {
         rgSCHUtlEmtcResPrcTti(cell);
      }
#endif
   } 
}
#endif
void schFillCrntTime(
   SlotTimingInfo slotInd,
   Inst        schInst)
{
   uint8_t cellCount = 0;
   for(cellCount = 0; cellCount < CM_LTE_MAX_CELLS; cellCount++)
   {
      RgSchCellCb    *cell;
      cell = rgSchCb[schInst].cells[cellCount];

      RGSCHCPYTIMEINFO(slotInd, cell->crntTime);

      RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, cell->hiDci0Time, 
                           TFU_ULCNTRL_DLDELTA);
      RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, cell->dlDciTime, 
                           TFU_DLCNTRL_DLDELTA);
      RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, cell->rcpReqTime, 
                           TFU_RECPREQ_DLDELTA);
      RGSCHDECRFRMCRNTTIME(cell->crntTime, cell->hqRlsTime, 
                           TFU_HQFBKIND_ULDELTA); 
      RGSCHDECRFRMCRNTTIME(cell->crntTime, cell->dlSfRlsTime, 
                           RGSCH_RLS_SF_IDX);

      RGSCH_INCR_SUB_FRAME(cell->crntTime, RG_SCH_CMN_DL_DELTA);

      RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
      cellSch->dl.time = cell->crntTime; 
   }
}

/** @brief This function prepares the TTI for scheduling and 
 *         invokes the Common channel scheduler. Uplink scheduler
 *         is invoked first if UL Scheduling at CRC is not enabled   
 *
 * @details 
 *
 *     Function: rgSchTomTtiUlAndDlCmnChSch
 *
 * @param  [out] RgSchCellCb    *cell
 *
 * Returns: Void  
 *  
 */
static Void rgSchTomTtiUlAndDlCmnChSch
(
RgSchCellCb        *cell
)
{

   cell->rlsHqArr[cell->crntHqIdx].numUes = 0;
   cell->crntHqIdx++;
   cell->crntHqIdx  = cell->crntHqIdx % RGSCH_NUM_SUB_FRAMES;

   cmPrcTmr(&cell->tqCp, cell->tq, (PFV)rgSCHTmrProcTmr);
   rgSCHMeasGapANRepTtiHndl (cell);
   /* We need to fill the PHICH for the UL Data, first we need to get the UL
    * SF from Scheduler, next we fill the dlSf that goes out this TTI. 
    * This must be performed prior to any other processing of the TTI
    * so that we do not wrap around and generate feedback prior to 
    * reception of UL data.
    */
#ifndef RG_ULSCHED_AT_CRC
   {
      uint8_t   Mval = 1;
      uint8_t   idx; /* Index into Uplink Sf array */ 
#ifdef LTE_TDD      
      Mval = rgSchTddPhichMValTbl[cell->ulDlCfgIdx]
                                 [cell->hiDci0Time.subframe];
#endif      
      if(Mval)
      {
         for(idx=0; idx < Mval; idx++)
         {
            rgSCHCmnRlsUlSf(cell, idx);
         }
      }           
   }
#endif

   /* DTX processing for those Harq's which did not get feedback from L1 */
   rgSCHDhmRlsDlsfHqProc (cell, cell->hqRlsTime);
   /* Re-Init the Downlink subframe */
   rgSCHUtlDlRlsSubFrm(cell, cell->dlSfRlsTime);
   /* Added handling to retransmit
    * release PDCCH in case of DTX
    */

   /*Check for DRX every TTI*/
   rgSCHDrxTtiInd(cell);

   /* For TDD, UL scheduling should happen after DL scheduling */
#ifndef LTE_TDD   
#ifndef RG_ULSCHED_AT_CRC
   /* Perform UL scheduling */ 
   rgSCHCmnUlSch(cell);
#endif
#endif   
   /* Perform DL scheduling  for Common channels */
   rgSCHCmnDlCommonChSch(cell);
      
   return;
}

/** @brief This function invokes the Non critical procedures like measurements,
 *         and RGR configurations.    
 *
 * @details 
 *
 *     Function: rgSchTomTtiMiscFunctions
 *
 * @param  [in] RgSchCellCb    *cell
 *
 * Returns: Void  
 *  
 */
static Void rgSchTomTtiMiscFunctions
(
RgSchCellCb *cell
)
{
   uint8_t   suId = cell->tfuSap->sapCfg.suId;
    
   /* Invoke RAM Tti Handler  */
   rgSCHRamTtiHndlr(cell);

   /* Handle RGR configurations */
   rgSCHGomTtiHndlr(cell, suId);
#ifdef LTE_L2_MEAS
   if((RGM_PRB_REPORT_START == cell->prbUsage.prbRprtEnabld) 
      && (!(cell->prbUsage.rprtPeriod) || ((glblTtiCnt % cell->prbUsage.rprtPeriod) == 0)))
   {
      rgSCHUtlUpdAvgPrbUsage(cell);
   }
   rgSCHL2Meas(cell,FALSE);
#endif 
   
   /* LTE_ADV_FLAG_REMOVED_START */
   /* Report ABS Load information to application periodically */
   if((RGR_ENABLE == cell->lteAdvCb.absCfg.status) &&
                    (cell->lteAdvCb.absCfg.absLoadPeriodicity))
   {
      RgrLoadInfIndInfo *rgrLoadInf;
      uint8_t                 idx;

      cell->lteAdvCb.absLoadTtiCnt++;
      if(cell->lteAdvCb.absLoadTtiCnt >= cell->lteAdvCb.absCfg.absLoadPeriodicity)
      {
         /* ccpu00134492 */
         if(rgSCHUtlAllocSBuf (cell->instIdx,(Data**)&rgrLoadInf,
               sizeof(RgrLoadInfIndInfo)) != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Could not "\
                     "allocate memory for sending LoadInfo\n");
            return;
         }
         cell->lteAdvCb.absLoadTtiCnt = 0;
         rgrLoadInf->cellId = cell->cellId;
         rgrLoadInf->bw     = cell->bwCfg.dlTotalBw;
         rgrLoadInf->type   = RGR_ABS;
         for(idx= 0; idx<RGR_ABS_PATTERN_LEN; idx++)
         {
            rgrLoadInf->u.absLoadInfo[idx] = cell->lteAdvCb.absLoadInfo[idx];
            cell->lteAdvCb.absLoadInfo[idx] = 0;
         }
         rgSCHUtlRgrLoadInfInd(cell, rgrLoadInf);
      }
   }

#ifdef LTE_TDD   
   if(cell->isDlDataAllwd)
   {   
      /* Calling function to update CFI parameters*/
      rgSchCmnUpdCfiDb(cell, RG_SCH_CMN_DL_DELTA);   
   }
   else
   {
      /* Incrementing the ttiCnt in case of UL subframe */
      if(!cell->dynCfiCb.switchOvrInProgress)
      {   
         cell->dynCfiCb.ttiCnt++;
      }
   }   
#else
   rgSchCmnUpdCfiDb(cell, RG_SCH_CMN_DL_DELTA);   
#endif   

   /* LTE_ADV_FLAG_REMOVED_END */
   return;
}


/** @brief This function invokes the Downlink scheduler
 *
 * @details
 *
 *     Function: rgSchTomTtiDlSch
 *
 * @param  [in] RgSchCellCb    *cell
 *
 * Returns: Void
 *
 */
static Void rgSchTomTtiDlSch
(
RgSchCellCb *cell
)
{

   if (cell->isDlDataAllwd && (cell->stopDlSch == FALSE))
   {
      rgSCHCmnDlSch(cell);
   }

   return;
}

/** @brief This function invokes Consolidates the allocations
 *         send the Subframe allocation info to MAC
 *
 * @details
 *
 *     Function: rgSchTomTtiCnsldtSfAlloc
 *
 * @param  [in] RgSchCellCb    *cell
 *
 * Returns: Void
 *
 */
static Void rgSchTomTtiCnsldtSfAlloc
(
RgSchCellCb  *cell
)
{
   RgSchDlSf *dlSf;
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   
   dlSf = rgSCHUtlSubFrmGet(cell, cellSch->dl.time);
   
   /* Prepare Subframe allocation info and send to MAC */
   rgSCHCmnCnsldtSfAlloc(cell); 
   
   /* Call ACK NACK module to add to dlsf Queue */
   rgSCHAckNakRepAddToQ(cell, dlSf);
      
   rgSCHTomUtlProcTA(cell);
  
   return;
}

/** @brief This function prepares the DL and UL Config requests
 *         and sends to CL 
 *
 * @details
 *
 *     Function: rgSchTomTtiL1DlAndUlCfg
 *
 * @param  [in] RgSchCellCb    *cell
 *
 * Returns: Void
 *
 */
static Void rgSchTomTtiL1DlAndUlCfg
(
RgSchCellCb        *cell,
RgTfuCntrlReqInfo  *cntrlInfo
)
{
   RgSchDlSf *dlSf = rgSCHUtlSubFrmGet (cell, cell->dlDciTime);
   RgSchDlSf *ulSf = rgSCHUtlSubFrmGet (cell, cell->hiDci0Time);
   RgSchErrInfo    err;

   rgSCHTomUtlProcDlSf (dlSf, ulSf, cell, cntrlInfo, &err);
   /* Mark this frame as sent */
   dlSf->txDone = TRUE;

   rgBwAlloInfo[dlSf->sfNum] += dlSf->bwAssigned;
   rgBwAlcnt[dlSf->sfNum] ++;


#ifdef LTE_TDD 
   rgSCHTomUtlProcTddUlSf(cell);
#else
   rgSCHTomUtlProcUlSf (cell, &err);
#endif   
   
   return;
}
#ifdef LTE_TDD
/** @brief This function prepares does the Downlink subframe re-init and 
 *         Harq DTX processing 
 *
 * @details
 *
 *     Function: rgSchTomUtlTddRlsSfAndHarq
 *
 * @param  [in] RgSchCellCb    *cell
 *
 * Returns: Void
 *
 */
static Void rgSchTomUtlTddRlsSfAndHarq
(
RgSchCellCb  *cell
)
{

   /* ccpu00132341-MOD- rgSchTddRlsDlSubfrmTbl is dependent on DELTA(=2).
    * Instead rgSchTddDlAscSetIdxKTbl can be used as the K set gives proper
    * UL subframes*/
   /* ccpu00133109: Removed RGSCHSUBFRMCRNTTIME as it is not giving proper 
    * output if diff is more than 10. Instead using RGSCHDECRFRMCRNTTIME() 
    * as it is serving the purpose */
   if(rgSchTddDlAscSetIdxKTbl[cell->ulDlCfgIdx][cell->hqRlsTime.subframe].
      numFdbkSubfrms)   
   {
      /* ccpu00132341-MOD- Providing the UL SF timing for avoiding 
       * calculation inside the function */
      rgSCHDhmTddRlsSubFrm(cell, cell->hqRlsTime);
      rgSCHDhmRlsDlsfHqProc(cell, cell->hqRlsTime);
   }
   return;
}

/** @brief This function processes the UL subframe and fills TFU reception
 *         request
 *
 * @details
 *
 *     Function: rgSCHTomUtlProcTddUlSf
 *
 * @param  [in] RgSchCellCb    *cell
 *
 * Returns: Void
 *
 */
static Void rgSCHTomUtlProcTddUlSf(RgSchCellCb  *cell)
{
   RgSchErrInfo    err;
   
   if(rgSchTddUlDlSubfrmTbl[cell->ulDlCfgIdx]
         [cell->rcpReqTime.subframe] == RG_SCH_TDD_UL_SUBFRAME)
   {
      if (rgSCHTomUtlProcUlSf (cell, &err) != ROK)
      {
         /* fill in err type and call sta ind */
         DU_LOG("\nERROR  -->  SCH : Unable to process Uplink subframe for cellId (%d))\n", cell->cellId);
      }
   }
   /* TDD Fix , to allow Special SF  SRS CFg  */
   else if(rgSchTddUlDlSubfrmTbl[cell->ulDlCfgIdx]
         [cell->rcpReqTime.subframe] == RG_SCH_TDD_SPL_SUBFRAME)
   {
      if (rgSCHTomUtlPrcUlTddSpclSf(cell, &err) != ROK)
      {
         /* fill in err type and call sta ind */
         DU_LOG("\nERROR  -->  SCH : Unable to process Sipceial subframe for cellId (%d))\n", cell->cellId);
      }
   }

   return;
}
#endif


/**********************************************************************

     End of file
**********************************************************************/
