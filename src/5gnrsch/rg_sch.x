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

/**********************************************************************

    Name:   LTE MAC layer

    Type:   C include file

    Desc:   Defines required by LTE MAC

    File:  rg_sch.x

**********************************************************************/
/** @file rg_sch.x
@brief This file contains basic data structures for the scheuler.
*/

#ifdef TENB_STATS
#include "pj_tenb_stats.x"   
#endif

#ifndef __SCH__
#define __SCH__

#ifdef TENB_STATS
#include "l2_tenb_stats.x"   
#endif

#ifdef EMTC_ENABLE
#include "rg_sch_emtc.x"
#endif


typedef struct rgSchHistNode
{
   U32 line;
   S8* file;
   const S8* func;
   Void * dbgVal; /* This is specific to the data struct being debug
                     for example if the debugging is done fo list
                     then this should contain the node address */
   U32 action;
}RgSchHistNode;

#define MAX_HIST_NODES    50

#define RGSCH_ACTION_ADD  11
#define RGSCH_ACTION_DEL  12

typedef struct rgSchHistInfo
{
   U32 histCount;
   RgSchHistNode hist[MAX_HIST_NODES];
}RgSchHistInfo;

#define RG_SCH_RECORD(_histInfo,_action,_dbgVal)\
{\
   (_histInfo)->hist[(_histInfo)->histCount%MAX_HIST_NODES].file = __FILE__;\
   (_histInfo)->hist[(_histInfo)->histCount%MAX_HIST_NODES].func = __FUNCTION__;\
   (_histInfo)->hist[(_histInfo)->histCount%MAX_HIST_NODES].line = __LINE__;\
   (_histInfo)->hist[(_histInfo)->histCount%MAX_HIST_NODES].action = _action;\
   (_histInfo)->hist[(_histInfo)->histCount%MAX_HIST_NODES].dbgVal = _dbgVal;\
   (_histInfo)->histCount++;\
}



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef TfuDciFormat1aInfo RgDciFmt1AInfo;
typedef TfuRaReqInfo       RgTfuRaReqInfo;
typedef TfuSubbandCqiInfo  RgSchSubbandCqiInfo;
typedef TfuHqIndInfo       RgTfuHqIndInfo;
typedef TfuHqInfo          RgTfuHqInfo;
typedef TfuCntrlReqInfo    RgTfuCntrlReqInfo;

/* Forward declarations for some structures */
#ifdef LTE_L2_MEAS
typedef struct rgSchL2MeasCb     RgSchL2MeasCb;
#endif /* LTE_L2_MEAS */
typedef struct rgSchQciCb        RgSchQciCb;
typedef struct rgSchUeCb         RgSchUeCb;
typedef struct rgSchCellCb       RgSchCellCb;
typedef struct rgSchErrInfo      RgSchErrInfo;
typedef struct rgSchUlAlloc      RgSchUlAlloc;
typedef struct rgSchUlRetxAlloc  RgSchUlRetxAlloc;
typedef struct rgSchUlHqProcCb   RgSchUlHqProcCb;
typedef struct rgSchDlHqProcCb   RgSchDlHqProcCb;
/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */
typedef struct rgSchDlHqTbCb     RgSchDlHqTbCb;
typedef struct rgSchLcgCb        RgSchLcgCb;
typedef struct rgSchDlHqEnt      RgSchDlHqEnt;
typedef struct rgSchRaCb         RgSchRaCb;
typedef struct _rgSchCb          RgSchCb;
typedef struct rgSchUlLcCb       RgSchUlLcCb;
typedef struct rgSchDlLcCb       RgSchDlLcCb;
typedef struct _rgSchdApis       RgSchdApis;
#ifdef LTE_TDD
typedef struct rgSchTddPhichOffInfo    RgSchTddPhichOffInfo;
typedef U8 RgSchTddNpValTbl[RGSCH_TDD_MAX_P_PLUS_ONE_VAL];
#endif
/* Added support for SPS*/
#ifdef LTEMAC_SPS
typedef struct rgSchDlSfAllocInfo RgSchDlSfAllocInfo;
#endif

typedef struct rgSchUeCellInfo RgSchUeCellInfo;
/**
  * @brief
  * Scheduler APIs
  */
struct _rgSchdApis
{
   S16 (*rgSCHRgrUeCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgrUeCfg *cfg,
            RgSchErrInfo *err));
   S16 (*rgSCHRgrUeRecfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgrUeRecfg *recfg,
            RgSchErrInfo *err));
   Void (*rgSCHFreeUe) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   S16 (*rgSCHRgrCellCfg) ARGS((RgSchCellCb *cell, RgrCellCfg *cfg,
            RgSchErrInfo *err));
   S16 (*rgSCHRgrCellRecfg) ARGS((RgSchCellCb *cell, RgrCellRecfg *recfg,
            RgSchErrInfo *err));
   Void (*rgSCHFreeCell) ARGS((RgSchCellCb *cell));
   S16 (*rgSCHRgrLchCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
            RgSchDlLcCb *dl, RgrLchCfg *cfg, RgSchErrInfo *errInfo));
   S16 (*rgSCHRgrLcgCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
            RgSchLcgCb *lcg, RgrLcgCfg *cfg, RgSchErrInfo *errInfo));
   S16 (*rgSCHRgrLchRecfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
            RgSchDlLcCb *dl, RgrLchRecfg *recfg,
            RgSchErrInfo *errInfo));
   S16 (*rgSCHRgrLcgRecfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
            RgSchLcgCb *lcg, RgrLcgRecfg *recfg, RgSchErrInfo *errInfo));
   Void (*rgSCHFreeDlLc) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchDlLcCb *dlLc));
   Void (*rgSCHFreeLcg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchLcgCb *lcg));
   S16 (*rgSCHRgrLchDel) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,CmLteLcId lcId, \
                             U8 lcgId));
   Void (*rgSCHActvtUlUe) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHActvtDlUe) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHHdlUlTransInd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
            CmLteTimingInfo timingInfo));
   Void (*rgSCHUeReset) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   S16 (*rgSCHUpdBsrShort) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchLcgCb *ulLcg, U8 bsr, RgSchErrInfo *err));
   S16 (*rgSCHUpdBsrTrunc) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchLcgCb *ulLcg, U8 bsr, RgSchErrInfo *err));
   S16 (*rgSCHUpdBsrLong) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, U8 bsArr[], RgSchErrInfo *err));
   S16 (*rgSCHUpdPhr) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, U8 phr, RgSchErrInfo *err));
   S16 (*rgSCHUpdExtPhr) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgInfExtPhrCEInfo * extPhr, RgSchErrInfo *err));
#ifdef RG_UNUSED
   S16 (*rgSCHUpdUlHqProc) ARGS((RgSchCellCb *cell, RgSchUlHqProcCb *curProc,
            RgSchUlHqProcCb *oldProc));
#endif
   S16 (*rgSCHContResUlGrant) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchErrInfo *err));
   S16 (*rgSCHSrRcvd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, CmLteTimingInfo, RgSchErrInfo *err));
   S16 (*rgSCHTti) ARGS((RgSchCellCb *cell, RgSchErrInfo *err));
   Void (*rgSCHUlCqiInd) ARGS(( RgSchCellCb *cell, RgSchUeCb *ue, TfuUlCqiRpt *ulCqiInfo));
   Void (*rgSCHPucchDeltaPwrInd) ARGS(( RgSchCellCb *cell,
            RgSchUeCb *ue, S8 delta));
   S16 (*rgSCHlUeReset) ARGS(( RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHDlDedBoUpd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchDlLcCb *svc));
   /* ccpu00105914: PHR handling for MSG3 */
   Void (*rgSCHUlRecMsg3Alloc) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
            RgSchRaCb *raCb));
   Void (*rgSCHUlHqProcForUe) ARGS((RgSchCellCb *cell, CmLteTimingInfo frm,
            RgSchUeCb *ue, RgSchUlHqProcCb  **procRef));
   RgSchUlAlloc *(*rgSCHFirstRcptnReq) ARGS((RgSchCellCb *cell));
   RgSchUlAlloc *(*rgSCHNextRcptnReq) ARGS((RgSchCellCb *cell,
            RgSchUlAlloc *alloc));
   RgSchUlAlloc *(*rgSCHFirstHqFdbkAlloc) ARGS((RgSchCellCb *cell, U8 idx));
   RgSchUlAlloc *(*rgSCHNextHqFdbkAlloc) ARGS((RgSchCellCb *cell,
            RgSchUlAlloc *alloc,U8 idx));
   Void (*rgSCHDlProcAddToRetx) ARGS((RgSchCellCb *cell,RgSchDlHqProcCb *hqP));
   Void (*rgSCHDlCqiInd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
            Bool isPucchInfo, Void *dlCqi, CmLteTimingInfo timingInfo));
/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
Void (*rgSCHSrsInd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         TfuSrsRpt* srsInd, CmLteTimingInfo timingInfo));
#endif

   Void (*rgSCHDlTARpt) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   /* Changes for MIMO feature addition */
   /* Removed dependency on MIMO compile-time flag */
   Void (*rgSCHDlRlsSubFrm) ARGS((RgSchCellCb *cell, CmLteTimingInfo subFrm));
   /* Added support for SPS*/
#ifdef LTEMAC_SPS
   Void (*rgSCHHdlCrntiCE) ARGS((RgSchCellCb *cell, RgSchUeCb * ue));
   Void (*rgSCHDlProcAck) ARGS((RgSchCellCb *cell, RgSchDlHqProcCb *hqP));
   Void (*rgSCHDlProcDtx) ARGS((RgSchCellCb *cell, RgSchDlHqProcCb *hqP));
   Void (*rgSCHDlRelPdcchFbk) ARGS((RgSchCellCb *cell, RgSchUeCb * ue,
            U8 isAck));
   Void (*rgSCHUlSpsRelInd) ARGS((RgSchCellCb *cell, RgSchUeCb * ue,
            Bool isExplRel));

   Void (*rgSCHUlSpsActInd) ARGS((RgSchCellCb *cell, RgSchUeCb * ue,
            U16 sduSuze));

   Void (*rgSCHUlCrcFailInd) ARGS((RgSchCellCb *cell, RgSchUeCb * ue,
           CmLteTimingInfo    crcTime));
   Void (*rgSCHUlCrcInd) ARGS((RgSchCellCb *cell, RgSchUeCb * ue,
           CmLteTimingInfo    crcTime));
#endif /* LTEMAC_SPS */
   Void (*rgSCHDrxStrtInActvTmrInUl) ARGS((RgSchCellCb *cell));
   Void (*rgSCHUpdUeDataIndLcg) ARGS((RgSchCellCb *cell, RgSchUeCb * ue, RgInfUeDatInd  *datInd));
#ifdef LTE_ADV
   S16 (*rgSCHRgrSCellUeCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue ,RgrUeSecCellCfg  *sCellInfoCfg, 
            RgSchErrInfo *err));
   S16 (*rgSCHRgrSCellUeDel) ARGS((RgSchUeCellInfo *sCellInfo, RgSchUeCb *ue));
#endif
#ifdef EMTC_ENABLE
   Void (*rgSCHUlProcAddToRetx) ARGS((RgSchCellCb *cell,RgSchUlHqProcCb *hqP));
#endif
};

/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE

/**
* @brief Periodic CQI/PMI/RI configuration parameters information
*/
typedef RgrUePrdDlCqiCfg RgSchUeDlPCqiCfg;

/**
* @brief Periodic CQI Setup configuration parameters information
*/
/* Reference: 36.313: CQI-ReportPeriodic */
typedef RgrUeDlPCqiSetup RgSchUeDlPCqiSetup;

/**
* @brief SRS configuration parameters information
*/
/* Reference 36.313 SoundingRS-UL-Config */

typedef RgrUeUlSrsCfg RgSchUeUlSrsCfg;


/**
* @brief SRS configuration setup parameters information
*/
/* Reference 36.313 SoundingRS-UL-Config */

typedef RgrUeUlSrsSetupCfg RgSchUeSrsUlSetupCfg;

/**
* @brief SR configuration parameters information
*/

typedef RgrUeSrCfg RgSchUeSrCfg;

/**
* @brief SR Setup configuration parameters information
*/

typedef RgrUeSrSetupCfg RgSchUeSrSetupCfg;

#define IOT_INVALID_FREQSTART 0xffffffff
#define IOT_INFINITE_SIZE 0xffffffff
#define RGSCH_IOT_PDCCH_POOLSZ 100
#define RGSCH_IOT_PDSCH_POOLSZ 100
#define RGSCH_IOT_PUSCH_POOLSZ 100
#define RGSCH_IOT_PUCCH_POOLSZ 100
#define RGSCH_IOT_SCHED_POOLSZ 100
/* TODO: Minimum Delta between CRNT Time and TX time */
#define RGSCH_IOT_PDCCH_DELTA  RG_SCH_CMN_DL_DELTA  
#define RGSCH_IOT_PDSCH_DELTA  RG_SCH_CMN_DL_DELTA - 1  /* UL_CNTRL_DELTA value is 2*/
#define RGSCH_IOT_PUCCH_DELTA  6
#define RGSCH_IOT_PUSCH_DELTA  6
#define RGSCH_IOT_PDCCH_MAXFREQSZ 24  /* MAX num of eCCEs per SF */ 
#define RGSCH_IOT_PDSCH_MAXFREQSZ 100 /* MAX num of PDSCH RB per SF */
#define RGSCH_IOT_PUCCH_MAXFREQSZ 2048  /* MAx num of PUCCH resource per SF */
#define RGSCH_IOT_PUSCH_MAXFREQSZ 100 /* MAx num of PUSCh RB per SF */
#define RGSCH_IOT_SCHED_MAXFREQSZ 1 /*Resource only in Time domain */
#define RGSCH_IOT_PUCCH_INVALID_FREQ 2049
/**
  * @brief
  * IoT PDCCH/PDSCH/PUSCH/PUCCH resource definition.
  * For Iot Devices resource can span multiple subframes
  * Hence resource is defined as a set of freq resources 
  * over a set of consecutive valid subframes
 */
typedef struct rgSchIotRes
{
   U32                   resType;
   PTR                   allctdBy;
   CmLteTimingInfo       timeStart;
   CmLteTimingInfo       timeEnd;
   U32                   tSize;
   U8                    freqStart;
   U8                    freqEnd;
   U32                   fSize;
   CmLList               lnk; /*!< Link to other Fragments or Allocs in resMngmt */
   CmLList               cbLnk; /*!< Link to other allocs in a given control block (ueCb) */
   CmLList               resLnk;/*!<Link to resList in UE*/
} RgSchIotRes;

/**
  * @brief
  * IoT PDCCH/PDSCH/PUSCH resource control block.
  * Has pre-initialized pool of resources of #poolSize.
  * allocList will hold the allocations for PDCCH/PUSCH/PDSCH
  * fragList will hold the context of fragments in resource
 */
typedef struct rgSchIotResCb
{
   U32            poolSz;
   RgSchIotRes    *resources;
   CmLListCp      resPool;
   U8             maxFreqSz;   /*!< Max Number of RBs/SCs/CCEs per Subframe */
   U32            deltaTime;   /*!< PDCCH/PUSCH/PDSCH Delta w.r.t current time */
   RgSchIotRes    *contRes;    /*!< Represents continuous availability of resource
                                    in frequency and time domain */
   CmLListCp      fragList;    /*!< Represents list of fragmented resourcesi b4
                                    contRes, ordered in time domain */
   CmLListCp      allocList;   /*!< Represents list of resources allocated which 
                                    are ordered in time domain */
} RgSchIotResCb;


/**
  * @brief
  * IoT Resource Management Control Block
  * for PDCCH, PDSCH and PUSCH resources
 */
typedef struct rgSchIotResMngmtCb
{
   RgSchIotResCb pdcchResCb;
   RgSchIotResCb pdschResCb;
   RgSchIotResCb puschResCb;
   RgSchIotResCb pucchHqFdbkCb;
} RgSchIotResMngmtCb;

S16 rgSCHUtlIotResPrcTti ARGS((
RgSchCellCb *cell
));

Void rgSCHUtlIotInsAlloc2Lst ARGS((
CmLListCp       *allocList,
RgSchIotRes     *alloc,
CmLList         *lnk
));

S16 rgSCHUtlIotResMngmtInit ARGS((
   RgSchCellCb *cell,
   U32         pdcchPoolSz,
   U32         pdcchDelta,
   U32         pdcchMaxFreqSz,
   U32         pdschPoolSz,
   U32         pdschDelta,
   U32         pdschMaxFreqSz,
   U32         puschPoolSz,
   U32         puschDelta,
   U32         puschMaxFreqSz
   ));

Void rgSCHUtlIotResMngmtDeinit ARGS((
   RgSchCellCb *cell
   ));

RgSchIotRes *rgSCHUtlIotAllocRes ARGS((
   RgSchIotResCb   *resCb,
   U32             fStart,
   U32             fSize,
   CmLteTimingInfo tStart,
   U32             tSize,
   Bool            isForEnable
   ));

RgSchIotRes *rgSCHUtlIotFirstAllocForTx ARGS((
   RgSchIotResCb   *resCb,
   CmLteTimingInfo txTime
   ));

RgSchIotRes *rgSCHUtlIotNextAllocForTx ARGS((
   RgSchIotResCb   *resCb,
   RgSchIotRes     *prevAlloc,
   CmLteTimingInfo txTime
   ));

Void rgSCHUtlIotAllocRls ARGS((
   RgSchIotResCb   *resCb,
   RgSchIotRes     *alloc
   ));

RgSchIotRes *rgSCHUtlEmtcAllocRes ARGS((
   RgSchIotResCb   *resCb,
   U32             *fStart,
   U32             *fEnd,
   CmLteTimingInfo tStart,
   U32             tSize,
   U32             numPrb,
   Bool            isRetx
   ));

S16 rgSCHUtlEmtcResPrcTti ARGS((
   RgSchCellCb *cell
   ));

S16 rgSCHUtlEmtcResMngmtInit ARGS((
   RgSchCellCb *cell,
   U32         pdschPoolSz,
   U32         pdschDelta,
   U32         pdschMaxFreqSz,
   U32         puschPoolSz,
   U32         puschDelta,
   U32         puschMaxFreqSz,
   U32         pucchPoolSz,
   U32         pucchDelta,
   U32         pucchMaxFreqSz
   ));

Void rgSCHUtlEmtcResMngmtDeinit ARGS((
   RgSchCellCb *cell
   ));

#ifdef RG_5GTF
EXTERN RgSchUlHqProcCb* rgSCHUhmGetUlProcByTime ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue, 
CmLteTimingInfo  frm
));
EXTERN S16 rgSCHUhmGetAvlHqProc ARGS((
RgSchCellCb           *cell,
RgSchUeCb               *ue,
RgSchUlHqProcCb         **hqP
));
#endif
/**
* @brief Store Periodic CQI, SRS and SR tranmission instances list
* */

typedef struct rgSchPerPucchCb
{
   CmLListCp         srLst;
   CmLListCp         srsLst;
   CmLListCp         cqiLst;
   CmLListCp         riLst;
}RgSchPerPucchCb;

/**
  @brief   UE Specific SRS Periodicity   and Subframe Offset Configuration FDD */

typedef struct rgSchUePCqiSrsSrCfgIdxTbl
{
   U16     min;          /*!< start ISRB Index */
   U16     max;          /*!< end ISRB Index */
   U16     peri;         /*!< SRS Periodicity in ms */
   U16     offset;        /*!< Offset with ISRB value */
}RgSchUePCqiSrsSrCfgIdxTbl;

/**
  @brief   UE Specific SRS Periodicity   and Subframe Offset Configuration TDD */

/* Ref: Table 8.2-2: UE Specific SRS Periodicity */

typedef struct rgSchUeSrsCfgIdxTbl
{
   U16     min;      /*!< start ISRB Index */
   U16     max;      /*!< end ISRB Index */
   U16     peri;    /*!< SRS Periodicity in ms */
   U16     offset1;  /*!< Offset with ISRB value */
   U16     offset2;  /*!< Offset with ISRB value */

}RgSchUeSrsCfgIdxTbl;

#ifdef RG_5GTF
/*
typedef enum rgSchSfType
{
   RG_SCH_SF_DLCNTRL_DLDATA = 0,
   RG_SCH_SF_DLCNTRL_DLDATA_ULCNTRL,
   RG_SCH_SF_DLCNTRL_ULDATA,
   RG_SCH_SF_DLCNTRL_ULDATA_ULCNTRL
}RgSchSfType;
*/
/**
@brief Per UE Group Info 
*/
typedef struct rgSchUeGrp
{
   U8      beamBitMask;    /*!<  Beam bit mask */
   //U8      numUe;       /*!<  Ue present in Group */
}RgSchUeGrp;

/**
@brief 5gtf Cell Info 
*/
typedef struct rgSch5gtfCellCb
{
   RgSchUeGrp      ueGrp5gConf[MAX_5GTF_GROUP];   /*!<  Contains all the group configured */
   RgSchSfType     dynConfig[MAX_5GTF_SUBFRAME_INFO];  /*!<  Get config from config file as of now */
   U8              numUes;
   U8              uePerGrpPerTti; /*!< Num of UEs to be scheduled in a group per TTI */
   U8              ueGrpPerTti;   /*!< Num of Groups to ne scheduled per Tti */
   U8              numCCs;
   U8              bwPerCC;
   U8              cfi;
   U8              totVrbg;
}RgSch5gtfCellCb;

#endif // 5GTF

/**
  @brief   UE Specific SRS Control Block */

typedef struct rgSchUeSrsCb
{
    RgSchUeUlSrsCfg     srsCfg;     /*!< SRS Specific configuration */
    U16                 peri;       /*!< Periodicity */
    U16                  offset;     /*!< subframe Offset */
    U16                  nSrsTrIdx;
                                   /*!< Next Tranmission instance
                                       Referenence: 36.213 Section:8.2
                                      ((10*sfn +sf-sfOffset))/mod(peri))  */
    CmLList             srsLstEnt;      /*!< SRS List for next tranmission instance subframe */
    U8                selectedAnt;    /*!< Selected Antenna for SRS Reception*/
    U16                 srsDist;    /*!<  crnt Distance to get RI Transmission */
    Bool                srsRecpPrcsd; /*!< SRS Recpeption processed as part of PUCCH
                                          // or PUSCH processing in current TTI or not */

}RgSchUeSrsCb;

/**
@brief  Periodic CQI/PMI Reporting configuration using PUCCH */



/**
@brief Subband Size (k) and Bandwidth Parts (J) vs. DL sys BW */

typedef struct rgSchUeBwSubSzBwParts
{
   U8 rbStart;   /* RB Start */
   U8 rbEnd;     /* RB End */
   U8 subSize;   /* k */
   U8 bwParts;   /* J */

}RgSchUeBwSubSzBwParts;

#ifdef LTE_ADV
/**
  * @brief
  * Enum for specifying priority level of various PUCCH report types
  * RG_SCH_CQI_PRIO_LVL_0 :PUCCH Report type 1,1a 
  * RG_SCH_CQI_PRIO_LVL_1 :PUCCH Report type 2,2b,2c,4
  * RG_SCH_CQI_PRIO_LVL_2 :PUCCH Report type 2a,3,5,6
  */
typedef enum
{
   RG_SCH_CQI_PRIO_LVL_0,
   RG_SCH_CQI_PRIO_LVL_1,
   RG_SCH_CQI_PRIO_LVL_2, 
   RG_SCH_CQI_PRIO_LVL_INVALID
}RgSchCqiPrioLvl;
#endif


/**
@brief Periodic CQI/PMI/RI Control Block */

typedef struct rgSchUePCqiCb
{
   RgrUePrdDlCqiCfg     cqiCfg;        /*!< Cqi Pmi Configuration */
   U16                  cqiPeri;       /*!< Periodicity */
   U16                  cqiOffset;     /*!< CQI offset */
   U16                  riOffset;      /*!< RI Offset */
   U16                  riPeri;        /*!< RI periodicity */
   U16                  nCqiTrIdx;
                                      /*!< Next Active Tranmission instace index ,
                                          precompute CQI/PMI
                                          Transmission Instance
                                          Referenence: 36.213 Section:8.2 */

   /* 1.  where wideband CQI/PMI reporting is configured
         (Mode: 1-0 or 1-1)
          10*sfn+floor(subframe/2)-Noffsetcqi )Mod(Nperiod)=0
      2.  where both wideband CQI/PMI and subband CQI reporting are configured
         (Mode: 2-0 or 2-1)
             10*sfn+floor(subframe/2)-Noffsetcqi )Mod(Nperiod)=0
             only the few occasations will be used for WB and
             few will be used for subbands.
   */

   U16                nRiTrIdx;      /*!< Next Active Transmission instance for RI
                                          Transmission Instance Referenence:
                                          36.213 Section:8.2 */
   U16                riDist;        /*!<  crnt Distance to get RI Transmission */
   U16                h;             /*!< Precompute and store H */
   U8                 riNumBits;     /*Precomputed value as it doesn't change
                                        on-the-fly */
   /* 1. In the case where wideband RI reporting is configured
         (Mode 1-0 or  1-1)
         (10*sfn+floor(subframe)-Noffsetcqi-NoffsetRI )Mod(NCqiperiod * 
          MriPeriod)=0

      2. Where Widesband and Subband RI reporting is configured
         (Mode 2-0 or 2-1 )
          (10*sfn+floor(subframe)-Noffsetcqi-NoffsetRI )
          Mod(H. NCqiperiod *MriPeriod )=0
          where H= J * K +1;  J=Number of bandwidth parts(BW/subsize). K is RGR
          interf input */
   Bool               isWb;    /*! < Is True when a WideBand CQI is to be
                                    Transmitted in a SubBand CQI Config*/
   U8                 bpIdx;   /*Index of the Bandwidth Part*/
   U8                 label;   /*Label L associated */
   U8                 J;       /*Number of Bandwidth Parts*/

   CmLList            cqiLstEnt;/*!< List CQI UEs for next transmission
                                     instance */
   CmLList            riLstEnt; /*!< List RI UEs for next transmission
                                     instance */
   Bool               invalidateCqi; /*!< If TRUE, ignore the WB/SB CQI report*/
   U8                 perRiVal;      /*!< Latest Periodic RI value reported */
   Bool               riRecpPrcsd;   /*!< RI Recpeption processed as part of
                                          PUCCH or PUSCH processing in current
                                          TTI or not */
#ifdef LTE_ADV
   RgSchCqiPrioLvl   prioLvl;   /* Priority lvl set based on the PUCCH Report
                                   type */
   Bool              isCqiIgnoByCollsn; /*!< Is this CQI type being ignored for
                                             this reporting instance due to
                                             collision with higher priority
                                             CQI type */
   Bool              isRiIgnoByCollsn; /*!< Is this CQI type being ignored for
                                            this reporting instance due to
                                            collision with higher priority CQI
                                            type */
#endif
   RgSchUeCellInfo  *servCellInfo;     /*!< Access to the serving cell of this
                                            CQI CB */
   RgSchHistInfo     histElem;
#ifdef EMTC_ENABLE
   U8 		     rgSchPucchRepNumCount;  /*!< CQI repetition count for CATM1 */
#endif
}RgSchUePCqiCb;



/**
@brief SR Configuration Index */

typedef struct rgSchUeSrCfgIdx
{
   U16     sIsr;      /*!< Start ISR Index */
   U16     eIsr;      /*!< End   ISR Index */
   U16     peri;      /*!< SR Periodicity */
   U16     offset;    /*!< offset */
}RgSchUeSrCfgIdx;
#ifdef RG_UNUSED
/* Reference : 36.213 Table 10.1-5 */
/* Note: SR is same table for TDD and FDD */
RgSchUeSrCfgIdx  schUeSrCfgIdx[RG_SCH_ISR_INDX_MAX]=
{
 { 0,4,   5, 0 },
 { 5,14,   10, 5 },
 { 15,34,  20,15 },
 { 35,74, 40,35},
 { 75,154, 80, 75},
 /* RESERVED: Configuration should not allow Res values */
};
#endif
/**
@brief SR Control block */

typedef struct rgSchUeSrCb
{
    RgSchUeSrCfg         srCfg;    /*!< SR configuration */
    U16                  offset;   /*!< SR offset */
    U16                  peri;     /*!< SR periodicity */

    U16                  nSrTrIdx;    /*!< Next Active Tranmission Instance Index ,
                                     Referenence: 36.213 Section:10.1 */

   /*
      ((10*sfn+floor(sf)-srOffset)Mod(srPeriodcity))
   */

#ifdef EMTC_ENABLE
   U8 		     rgSchPucchRepNumCount;  /*!< SR repetition count for CATM1 */
#endif
   CmLList           srLstEnt;      /*!< Next Transmission instance SR UE list */

}RgSchUeSrCb;

/**
  * @enum rgSchCqiReqField
  * Enumeration of CSI Request Field (Table 7.2.1-1A 36.213).
  * "00"    No aperiodic CSI report is triggered
  * "01"  Aperiodic CSI report is triggered for serving cell  c
  * "10"  Aperiodic CSI report is triggered for a 1st set of 
  *          serving cells configured by higher layers
  * "11"  Aperiodic CSI report is triggered for a 2nd  set of 
  *       serving cells configured by higher layers
  *
  */
typedef enum rgSchCqiReqField 
{
   RG_SCH_APCQI_NO = 0,
   RG_SCH_APCQI_SERVING_CC,
   RG_SCH_APCQI_1ST_SERVING_CCS_SET,
   RG_SCH_APCQI_2ND_SERVING_CCS_SET
}RgSchCqiReqField;

/**
@brief  APeriodic CQI Control Block */
typedef struct rgSchUeACqiCb
{
 RgrUeAprdDlCqiCfg aCqiCfg;/*!< Cqi Pmi Configuration */
 U8                L; /*Unique Label through which positioning of Subbands
                        will be conveyed by the UE */
 U8                N;  /*Total Bandwidth / Num of Subbands*/
 U8                M; /*Number of Selected Subbands*/
 U8                riNumBits;
 U8                k; /*k - SB size. 36.213 Tables:7.2.1-3, 7.2.1-5*/
 U8                cqiPmiSzR1;  /*CQIPMI Size for Rank =1*/
 U8                cqiPmiSzRn1; /*CQIPMI Size for Rank > 1*/
 U32               aCqiTrigWt;  /* Metric to track Aperiodic CQI Trigger occassion */ 
 RgSchCqiReqField  cqiReqField; /* Cqi Request field. This Value can be 00 01 10 11, based upon 
                                   the cell present in which trigger list form App */ 
}RgSchUeACqiCb;

typedef enum
{
   RG_SCH_FDD_PCQI_TBL = 0,
   RG_SCH_TDD_PCQI_TBL,
   RG_SCH_RI_TBL,
   RG_SCH_FDD_SRS_TBL,
   RG_SCH_TDD_SRS_TBL,
   RG_SCH_SR_TBL
} RgSchPerTbl;

/*ccpu00116923 - ADD - SRS present support*/
#ifdef LTE_TDD
typedef U8 RgSchTddCellSpSrsSubfrmTbl[RGSCH_CELLSP_SRS_SF_CONFIGS][RGSCH_NUM_SUB_FRAMES];
#else
typedef U8 RgSchFddCellSpSrsSubfrmTbl[RGSCH_CELLSP_SRS_SF_CONFIGS][RGSCH_NUM_SUB_FRAMES];
#endif


#endif



#ifdef LTE_TDD
/**
  * @brief
  * Enum to define the type of Downlink subframe.
  */
typedef enum 
{
  RG_SCH_SPL_SF_NO_DATA = 0,
  RG_SCH_SPL_SF_DATA,
  RG_SCH_DL_SF_0,
  RG_SCH_DL_SF
}RgSchTddSfType;
/**
  * @brief
  * TDD UE specific PUCCH recpetion information.
  */
typedef struct rgSchUePucchRecpInfo
{
   CmHashListEnt     hashLstEnt;     /*!< List of PUCCH for receiving
                                          ACK/NACK feedback information */
   TfuUeRecpReqInfo  *pucchRecpInfo; /*!< UE PUCCH Reception information */
} RgSchUePucchRecpInfo;

/**
  * @brief
  * TDD switch point information.
  */
typedef struct rgSchTddSubfrmInfo
{
   U8 switchPoints;     /*!< Number of DL-UL switch points */
   U8 numFrmHf1;        /*!< Number of subframes for half frame 1
                             Present for both 5ms and 10ms periodicity */
   U8 numFrmHf2;        /*!< Number of subframes for half frame 2
                             Present only for 5ms periodicity */
} RgSchTddSubfrmInfo;

/**
  * @brief
  * TDD DL Association Set information.
  */
typedef struct rgSchTddDlAscSetIdxK
{
   U8 numFdbkSubfrms;                        /*!< Number of Feedbacks for DL Subframes */
   U8 subfrmNum[RGSCH_NUM_SUB_FRAMES-1];     /*!< List of Subframe Number */
} RgSchTddDlAscSetIdxK;

/** @brief PRACH Information for a frequency resource. */
typedef struct rgrSchTddPrachInfo
{
   U8     freqIdx;          /*!< Frequency Index */
   U8     sfn;              /*!< Even/Odd/All Radio Frames */
   U8     halfFrm;          /*!< First/Second Half Frame */
   U8     ulStartSfIdx;     /*!< Uplink Start Subframe Index*/
} RgSchTddPrachInfo;

/** @brief PRACH resource Information for each of the
 *  frequency resources. */
typedef struct rgrSchTddPrachRscInfo
{
   U8                 numRsc;                             /*!< Number of frequency resources*/
   RgSchTddPrachInfo  prachInfo[RGSCH_TDD_MAX_FREQ_RSRC]; /*!< PRACH Information */
} RgSchTddPrachRscInfo;

/**
  * @brief
  * TDD Special subframe configuration information.
  */
struct rgSchTddSplSubfrmInfo
{
   U8   norDlDwPts;        /*!< DL Normal CP: DwPTS in Ts */
   U8   norDlNorUpPts;     /*!< DL Normal CP: UL Normal CP:UpPTS in Ts */
   U8   norDlExtUpPts;     /*!< DL Normal CP: UL Extended CP: UpPTS in Ts */
   U8   extDlDwPts;        /*!< DL Extended CP: DwPTS in Ts */
   U8   extDlNorUpPts;     /*!< DL Extended CP: UL Normal CP:UpPTS in Ts */
   U8   extDlExtUpPts;     /*!< DL Extended CP: UL Extended CP: UpPTS in Ts */
};

/**
  * @brief
  * RACH response awaiting scheduling from the current time is
  * identified with sfn offset and subframe.
  */
typedef struct rgSchTddRachRspInfo
{
   U8    sfnOffset;                    /*!< SFN offset with respect to
                                             expected RACH available for
                                             scheduling */
   U8    numSubfrms;                   /* Number of subframes present */
   U8    subframe[RGSCH_NUM_SUB_FRAMES];  /*!< List of Subframe numbers */
} RgSchTddRachRspInfo;

typedef RgSchTddRachRspInfo RgSchTddRachDelInfo;

/**
  * @brief
  * List of awaiting RACH responses for scheduling across radio frames.
  *
  */
typedef struct rgSchTddRachRspLst
{
   U8                   numRadiofrms;   /*!< Number of radio frames */
   RgSchTddRachRspInfo  rachRsp[2];     /*!< RACH Occasions for which response
                                          can be sent */
   RgSchTddRachDelInfo  delInfo;        /*!< Previous RACH responses for
                                          which the scheduling deadline
                                          has expired. So those responses
                                          can be deleted */
} RgSchTddRachRspLst;

/**
  * @brief
  * Uplink association index information indicates the SFN offset and
  * subframe in which DL HARQ ACK/NACK is expected.
  */
typedef struct rgSchTddUlAscInfo
{
   U8    subframe;         /*!< Subframe number */
   U8    sfnOffset;        /*!< SFN offset with respect to expected
                                UL data reception time */
} RgSchTddUlAscInfo;

/**
  * @brief
  * PUSCH information indicates the SFN offset and
  * subframe in which UL data is scheduled.
  */
typedef struct rgSchTddPuschOffInfo
{
   U8    subframe;         /*!< Subframe number */
   U8    sfnOffset;        /*!< SFN offset with respect to expected
                                UL data reception time */
} RgSchTddPuschOffInfo;

/**
  * @brief
  * PHICH information indicates the SFN offset and
  * subframe for which PHICH should be sent.
  */
struct rgSchTddPhichOffInfo
{
   U8    numSubfrms;       /*!< Number of subframes */
/* ACC-TDD */
   U8    subframe;         /*!< The Uplink Subframe number corresponding
                                to the phich */
   U8    sfnOffset;        /*!< SFN offset with respect to expected
                                UL data reception time */
};

/**
  * @brief
  * DL feedback reception information indicates the SFN offset
  * and subframe at which feedback is expected.
  */
typedef struct rgSchTddDlFdbkInfo
{
   U8    subframe;      /*!< Subframe number */
   U8    sfnOffset;     /*!< SFN offset with respect to current
                             scheduled time */
   U8    m;             /*!< m factor used in Downlink Association
                             Set Index */
#ifdef LTE_ADV /*Naw:: This is not correct */
   CmLListCp n1PucchResLst;        /*!< List for storing the used N1 resource */
#endif
} RgSchTddDlFdbkInfo;


/**
  * @brief
  * Special subframe configuration index.
  */
typedef struct rgSchTddSplSubfrmCfg
{
   U16   dwPts;      /*!< DwPTS in OFDM Symbol Duration */
   U16   upPts;      /*!< UpPTS in OFDM Symbol Duration */
   Bool  isDlDataAllowed; /*!< To allow scheduling of DL data on 
                               special subframe */
} RgSchTddSplSubfrmCfg;

/**
  * @brief
  * ACK/NACK information to be used for ACK/NACK bundling mode.
  */
typedef struct rgSchTddANInfo
{
   U16    sfn;           /*!< ACK/NACK is sent for PDU in this SFN */
   U8     subframe;      /*!< ACK/NACK is sent for PDU in this subframe */
   U8     dlDai;         /*!< Downlink Assignment Index for
                                                    UL-DL Configuration 1-6 */
   U8     ulDai;         /*!< DAI for uplink */
   U8     latestMIdx;    /*!< Last transmitted DL subframe 'm' index */
   U8     n1ResTpcIdx;    /*!< N1 Res idx for scell assigned in TPC command */
   Bool   isSpsOccasion; /*!< To indicate the presence of SPS occasion */
#ifdef LTE_ADV
   U8     wUlDai;        /*!< Max Ul dai in all the cells */
#endif
} RgSchTddANInfo;
#endif

/**
  * @brief
  * Information about one MCS entry.
  */
typedef struct rgSchUlIMcsInfo
{
   U8 qm;
   U8 iTbs;
} RgSchUlIMcsTbl[29];
EXTERN RgSchUlIMcsTbl rgUlIMcsTbl;

typedef struct rgSchUeCatTbl
{
   U32    maxUlBits;/*Maximum number of
                      bits of an UL-SCH
                      transport block
                      transmitted within a
                      TTI*/
   U32    maxDlBits[4];/*Maximum number of 
                      bits of a DLSCH
                      transport block 
                      received within a TTI*/
/* correcting DL harq softbuffer limitation logic */
   U32    maxSftChBits;/*Total number of soft channel bits*/
   Bool   ul64qamSup;/*Support for 64QAM in UL*/
/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */
   U32    maxDlTbBits;/*Maximum number of DL-SCH 
                        transport block bits 
                        received within a TTI*/
   U8     maxTxLyrs;/*Maximum number of supported 
                      layers for spatial multiplexing 
                      in DL*/
} RgSchUeCatTbl[CM_MAX_UE_CAT_SUPP + 1];
EXTERN RgSchUeCatTbl rgUeCatTbl;

/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */
typedef U32 RgSchTbSzTbl[RGSCH_MAX_NUM_LYR_PERCW][RGSCH_NUM_ITBS][RGSCH_MAX_NUM_RB];

#ifdef LTE_TDD
typedef U8 RgSchRaPrmblToRaFrmTbl[RGSCH_MAX_TDD_RA_PREAMBLE_FMT+1];
#else
/* Added matrix 'rgRaPrmblToRaFrmTbl' for computation of RA
   sub-frames from preamble format */
typedef U8 RgSchRaPrmblToRaFrmTbl[RGSCH_MAX_RA_PREAMBLE_FMT+1];
#endif
EXTERN RgSchRaPrmblToRaFrmTbl rgRaPrmblToRaFrmTbl;

EXTERN U8 rgRvTable[4];

typedef struct rgDciFmt
{
   U8            dciType;
   union
   {
      RgDciFmt1AInfo   dci1a;
   } dci;
} RgDciFmt;

typedef enum rgSchPdcchSearchSpace
{
   RG_SCH_UE_SPECIFIC_SEARCH_SPACE,
   RG_SCH_CMN_SEARCH_SPACE,
}RgSchPdcchSearchSpace;

/**
  * @brief
  * Information about one PDCCH.
  */
typedef struct rgSchPdcch {
   U8             nCce;    /*!< CCE index */
   CmLteAggrLvl   aggrLvl; /*!< Aggregation level */
   TfuDciInfo     dci;     /*!< PDCCH format */
   U16            rnti;    /*!< RNTI to who the PDCCH is allocated */
#if (defined (LTE_TDD))
   U8                dlDai;          /*!< DAI associated with this PDCCH.
                                          THis is used for F1BCS resource calulcation */
#endif
  /* Added support for SPS*/
#ifdef LTEMAC_SPS
   CmLteTimingInfo relFbkTiming; /*!< Feebback timing information for release
                                   PDCCH */
   Bool           isSpsRnti; /*!< TRUE if rnti is SPS RNTI */
   U16            crnti;    /*!< CRNTI to who the PDCCH is allocated */
#endif
   CmLList        lnk;     /*!< To link PDCCHs in a subframe */
#ifdef EMTC_ENABLE   
   Void           *emtcPdcch;
#endif   
   RgSchUeCb      *ue;     /*!< Pointer to the UE Control Block */
   RgSchPdcchSearchSpace pdcchSearchSpace; /*!< Search Space from this PDCCH allocated */
   U8             dciNumOfBits; /*!< Size of DCI in bits */
} RgSchPdcch;

/**
  * @brief
  * PDCCH information for cell.
  */
typedef struct rgSchPdcchInfo {
   U8             *map;    /*!< Bit map of PDCCHs */
   U8             currCfi;       /*!< Number of CCEs */
   U16            nCce;    /*!< Total CCEs */
   CmLListCp      pdcchs;  /*!< List of RgSchPdcch */
} RgSchPdcchInfo;

typedef struct rgSchPhich
{
   CmLList        lnk;        /*!< To link PHICHs in a subframe */
   U8             hqFeedBack; /*!< Harq Feed Back */
   U8             rbStart;    /*!< Starting RB */
   U8             nDmrs;      /*!< 3 bits for DMRS cyclic shift */
   /* changes for passing iphich at TFU;*/
   Bool           isForMsg3;  /*! < Phich Ack/Nack conveyed for MSG 3 */
#ifdef LTE_TDD
   U8             iPhich;     /*!< For determining phich group */
#endif
} RgSchPhich;

typedef struct rgSchPhichInfo
{
   CmLListCp      phichs;  /*!< List of RgSchPhich */
} RgSchPhichInfo;

typedef struct rgSchBcchTb
{
   RgSchPdcch     *pdcch;
   Buffer         *tb;
   U16            tbSize;
} RgSchBcchTb;

typedef struct rgSchPcchTb
{
   RgSchPdcch     *pdcch;
   Buffer         *tb;
   U16            tbSize;
} RgSchPcchTb;

typedef struct rgSchRaRspAlloc
{
   U16            raRnti;
   U32            tbSz;
   TknU8          backOffInd; /*!< Backoff index value */
   CmLListCp      raRspLst;   /*!< List of RaCbs */
   CmLListCp      contFreeUeLst; /*! List of HandOver or PdcchOrder UEs */
   RgSchPdcch     *pdcch;     /*!< NULLP if no Rsp allocation done for raRnti*/
}RgSchRaRspAlloc;

typedef struct rgSchBchTb
{
   Buffer         *tb;    /*!< BCH data for this frame */
   U16            tbSize; /*!< Non-Zero if bch data is scheduled for this SF */
}RgSchBchTb;

/* Added support for SPS*/
#ifdef LTEMAC_SPS
/**
  * TODO: check compilation
  @brief Downlink Resource allocation type information. */
struct rgSchDlSfAllocInfo
{
  U32 raType0Mask;       /*!< RBG allocation mask for type 0*/
  U32 raType1Mask[RG_SCH_NUM_RATYPE1_32BIT_MASK]; /*!< RA Type 1
                                                    allocation mask */
  U32 raType1UsedRbs[RG_SCH_NUM_RATYPE1_32BIT_MASK];/*!< RA Type 1 Used RBs
                                                     per subset */
  U32 nxtRbgSubset;     /*!< Next RBG subset to be used for allocation */
  U32 raType2Mask[RG_SCH_NUM_RATYPE2_32BIT_MASK];
                        /*!< Mask for resource allocation type 2 */
};
#endif /* LTEMAC_SPS */

/* LTE_ADV_FLAG_REMOVED_START */
/**
  @brief RGR RB range for SFR */
typedef struct rgrPwrHiCCRange
{
   U8   startRb;  /*<! Start RB for power high cell centre user */
   U8   endRb;    /*<! End RB for power high cell centre user */
} RgrPwrHiCCRange;

typedef struct rgSchSFRTotalPoolInfo
{
   CmLListCp       ccPool;            /*Dynamic Variable, It will change when BW is assigned. 
                                        Initially assigned to 0 */
   Bool            ccBwFull;          /*If BW is full*/
   CmLListCp       cePool;            /*Dynamic Variable, It will change when BW is assigned. 
                                        Initially assigned to 0 */
   Bool            ceBwFull;          /*If BW is full*/
   Bool            isUeCellEdge;
   Bool            CERetx;            /* to check if there is any CE retransmission */ 
   Bool            CCRetx;            /* to check if there is any CC retransmission */
   Bool            CC1;               /* Cell centre pool 1 */  
   Bool            CC2;               /* cell centre pool 2 */
   U8              CCPool1BwAvlbl;    /* Cell Centre Bw available for Pool1 */  
   U8              CCPool2BwAvlbl;    /* Cell Centre Bw available for Pool2 */
   U8              CEPoolBwAvlbl;     /* Cell Edge Bw available for CE Pool */
}RgSchSFRTotalPoolInfo;

typedef struct rgSchSFRPoolInfo
{
   /*Fixed RB Range of the Pool. Fixed by user configuration*/
   U16               poolstartRB;
   U16               poolendRB;								  
   U16               bw;         /*!< Number of RBs in the pool */
   /*Dynamic Values */
   U8                type2Start; /*!< Start RB for the next type 2 allocation */
   U8                type2End;   /*!< End position of Type2 allocation with  in the bit mask */
   U8                type0End;   /*!< End position of Type0 allocation with   in the bit mask */

   U16               bwAlloced;  /*!< Number of RBs already allocated by DLFS */
   Bool              CCPool2Exists;  /*!< To check if the last pool in the RB allocation is a CC pool */  
   struct rgSchSFRPoolInfo * adjCCPool; /*!< The CC pool adjacent to this CE Pool SFR_FIX */
   RgrPwrHiCCRange   pwrHiCCRange; /*!< Power High Range which can be used by CC user based on RNTP info */
}RgSchSFRPoolInfo;
/* LTE_ADV_FLAG_REMOVED_END */
                
typedef struct rgSchPuschBwInfo
{
   U8 numSb;         /*!< PUSCH BW in subbands */
   U8 startRb;       /*!< Starting RB for PUSCH BW */
}RgSchPuschBwInfo; 
 
typedef struct rgSchDynCfiCb
{
   U8               isDynCfiEnb;     /*!< Dynamic CFI feature Flag */
   U8               maxCfi;          /*!< max possible CFI in the cell */ 
   U8               switchOvrWinLen; /*!< Length of Switchover window */
   U16              cceFailCnt;      /*!< Number of CCE allocation Failures in a 
                                           CCE failure sample period */            
   U16              *cceFailSamples; /*!< Array holding samples of Avg number 
                                           of CCE allocation failures */
   U16              cceFailSum;      /*!< Moving sum of the CCE faliures of 
                                          N samples */
   U16              cfiStepUpTtiCnt; /*!< No.of TTIs to take decision for CFI 
                                          step Up */
   U16              cceUsed;         /*!< Total CCEs used in current monitoring 
                                          interval */
   U16              lowCceCnt;       /*!< Number of TTI in which CCEs used is 
                                          less than available CCEs in 
                                          lower CFI */
   U16              cfiStepDownTtiCnt; /*!< No.of TTIs to take decision for CFI 
                                            step Down */
   U32              cfiSwitches;     /*!< Total number of CFI switches */       
   U32              cfiIncr;         /*!< Total number of CFI increments */
   U32              cfiDecr;         /*!< Total number of CFI decrements */
                                     /*!< Total CCE per CFI */ 
   U8               cfi2NCceTbl[RG_SCH_MAX_MPHICH][RG_SCH_CMN_MAX_CFI]; 
                                                               
   U8               numFailSamples;    /*!< Number of CCE Allocation Failure 
                                            samples */
   U16              failSamplePrd;     /*!< Failure Sample Period */
   U16              ttiCnt;            /*!< TTI count to track monitoring period
                                            and sample period expiry */  
   RgSchPuschBwInfo bwInfo[RG_SCH_CMN_MAX_CFI]; /*!< PUSCH BW info */ 
   U8               pdcchSfIdx;        /*!< Subframe Idx for CFI applying */
   U8               prevCceFailIdx;   /*!< To maintain Previous Index of 
                                           CCE failure array */  
   Bool             switchOvrInProgress; /*!< Switchover is in progress */
   Bool             dynCfiRecfgPend; /*!< Flag for pending dynamic cfi reconfig */    
}RgSchDynCfiCb;    

#ifdef RG_5GTF

/**
  * @brief
  * Beam information for a subframe
  */
typedef struct rgSchSfBeamInfo
{
   U16      totVrbgAvail;        /*!< Total VRBG available */
	//U16      totRb;               /*!<  Total RB per Beam */
	U16      totVrbgRequired;     /*!<  total Rbs requested for beam */
   U16      totVrbgAllocated;    /*!<  total Rbs allocated for beam */
   U16      vrbgStart;           /*!<  VRBG start for beam */
} RgSchSfBeamInfo;
#endif

typedef struct rgSchDlSf
{
   U8                cceCnt;       /*!< Number of CCEs used in the subframe */
   Bool              isCceFailure; /*!< TRUE if PDCCH allocation is failed for 
                                        this subframe */
   U8                dlUlBothCmplt; /*!< To track that DL and UL both scheduling
                                         is done */
   U8                sfNum;      /*!< Number of the subframe */
   U16               bw;         /*!< Number of RBs in the cell */
   U16               bwAlloced;  /*!< Number of RBs already allocated by DLFS */
   U16               bwAssigned; /*!< Number of RBs already allocated by scheduler */
/* LTE_ADV_FLAG_REMOVED_START */
   RgSchSFRTotalPoolInfo  sfrTotalPoolInfo; /* SFR Pool Info*/
   TknStrOSXL        rntpInfo;   /* RNTP Info for the complete subframe*/
/* LTE_ADV_FLAG_REMOVED_END */

   U8                type2Start; /*!< Start RB for the next type 2 allocation */
   U8                type2End;   /*!< End position of Type2 allocation with
                                      in the bit mask */
   U8                type0End;   /*!< End position of Type0 allocation with
                                      in the bit mask */
   U8                lstRbgDfct; /*!< The last RBG deficit RBs, Ex. if DLBW = 97,
                                  * RBGsz = 4, lstRbgDfct = 3 */
   Bool              txDone;     /*!< Flag to indicate if transmission is done*/
   U32               numDlActvUes; /* 4UE_TTI_DELTA: num of active Ues */
   RgSchBchTb        bch;        /*!< BCH data for this frame */
   RgSchBcchTb       bcch;       /*!< BCCH allocation for this frame */
   RgSchPcchTb       pcch;       /*!< PCCH allocation for this frame */
   RgSchPdcchInfo    pdcchInfo;  /*!< PDCCH info */
   RgSchPhichInfo    phichInfo;  /*!< PHICH info */
   /* CA dev Start */
   
   CmLListCp         ueLst;        /*!< UE List which are scheduled in Perticular SF */
   Void              *laaCb;
   CmLListCp         msg4HqPLst;   /*!< Msg4 Hq Procs that are scheduled in Perticular SF */
   /* CA dev End */
   U8                remUeCnt;   /*!< Remaining number of UEs that can be scheduled */
   Bool              schdAmbrNxt;/*!< Flag indicates to pick an AMBR LC after GBR LC */
   /*[ccpu00138609]-ADD- Counter to track the number of Msg4/DL CCCH UEs */ 
   U8                schdCcchUe; /*!< Num of Msg4/DL CCCH UEs scheduled in 
                                      the DL Sf */ 
#ifdef LTE_TDD
   U8                   nCce;       /*!< Number of CCEs */
   RgSchTddPhichOffInfo phichOffInfo;  /*!< PHICH Information */
   RgSchTddDlFdbkInfo   dlFdbkInfo; /*!< HARQ Ack/Nack feedback expected time */
   RgSchTddUlAscInfo    ulAscInfo;  /*!< UL Association set Information */
   RgSchRaRspAlloc      raRsp[RGSCH_MAX_TDD_RA_RSP_ALLOC]; /* Array of list
                                                 of Ra Rsp
                                                 Allocations for each rarnti */
   RgSchTddSfType       sfType;    /*!< Stores the enum value for SF type */                                               
#else
   RgSchRaRspAlloc   raRsp[RGSCH_MAX_RA_RSP_ALLOC]; /* Array of list of Ra Rsp
                                                 Allocations for each rarnti */
#endif
   Void              *dlfsSf;     /*!< DLFS specific information per sub-frame */
   CmLListCp         ackNakRepQ;  /*!< ACK NACK repetition queue */
   /* Added support for SPS*/
#ifdef LTEMAC_SPS
   Bool              isSPSOcc;    /*!< TRUE if this SF has SPS Occasion allocation */
   RgSchDlSfAllocInfo dlSfAllocInfo;/*!< Allocation information for DL SF */
   U32               spsAllocdBw;   /*!< Number of RBs allocated for SPS */
   RgSchPdcch        *relPdcch;     /*!< Only one release PDCCH scheduled per
                                      sub-frame */
#ifdef LTE_TDD
   struct
   {
      CmLteRnti         crnti;       /*!< UE's C-RNTI (not SPS RNTI) */
      RgrTddAckNackMode ackNackMode; /*!< Ack/nack mode */
   } relPdcchInfo;                  /*!< Info required for receiving Hq fdbk
                                     *   for release PDCCH (DL) */
#endif
#endif
   U32      schedLcCount;          /*!< Num of LCs scheduled in this TTI */
   U32      totalLcCntOfSlctdUes;  /*!< total Lc count of all UE's selected in 
                                    *   this TTI*/
   U32      totPrbReq;             /*!< Sum of PRBs required by selected UEs
                                        in this subframe */
#ifdef BEST_EFFORT_2_UE_PER_TTI
  U32       beTotPrbReq;
#endif
                                    
  /* CA dev Start */
   U8      dlIdx;
  /* CA dev Start */
#ifdef LTE_ADV
   CmLListCp n1PucchResLst;        /*!< List for storing the used N3 resource */
   CmLListCp n3PucchResLst;        /*!< List for storing the used N1 resource */
#endif
#ifdef RG_5GTF
   RgSchSfBeamInfo  sfBeamInfo[MAX_5GTF_BEAMS];      /*!< Per info Beam per sf*/
   U8               numGrpPerTti;     /*!< number of Group per TTI*/
	U8               numUePerGrp;      /*!< number of UE per group in TTI*/
#endif
} RgSchDlSf;

/* Data Structures used for DL Allocations by Schedulers */
/**
  @brief Downlink Resource allocation type 0 information. */
typedef struct rgSchDlRbAllocRaType0
{
   U8 numDlAlloc;          /*!< Number of downlink allocations */
   U32 dlAllocBitMask;     /*!< Downlink allocations done for the UE */
} RgSchDlRbAllocRaType0;

/* Added support for SPS*/
#ifdef LTEMAC_SPS
/**
  @brief Downlink Resource allocation type 1 information. */
typedef struct rgSchDlRbAllocRaType1
{
   U8 numDlAlloc;          /*!< Number of downlink allocations */
   U8 shift;               /*!< Indicates if the shift is triggered in the
                                allocation */
   U8 rbgSubset;           /*!< RBG subset number selected for the allocation*/
   U32 dlAllocBitMask;     /*!< Downlink allocations done for the UE */
} RgSchDlRbAllocRaType1;
#endif /* LTEMAC_SPS */
/**
  @brief Downlink Resource allocation type 2 information. */
typedef struct rgSchDlRbAllocRaType2
{
   Bool        isLocal;       /*!< True if localised VRB */
   U8          rbStart;       /*!< Starting RB */
   U8          numRb;         /*!< Number of RBs */
} RgSchDlRbAllocRaType2;


typedef struct rgSchcmnDlGrnt
{
   U16     schdTime;       /*!< Time at which Harq proc has been scheduled */
   U8      rbStrt;         /*!< Starting RB of the allocation */
   U8      numRb;          /*!< Number of RBs allocated */
   U8      iMcs;           /*!< Index to the MCS */
   U8      rv;             /*!< RV for HARQ (re)transmission */
   U8      rvIdx;          /*!< RVIdx for HARQ(re)transmission */
#ifdef RG_5GTF   /* ToDo:: Anoop need to check for other fields required*/
   U8      vrbgStart;
   U8      numVrbg;
   U16     rbAssign;
   U8      xPDSCHRange;
   U8      SCID;
   TfuDciFormat dciFormat;  /*!< DCI format for the allocation */
	/* ToDo */
#endif
} RgSchDlCmnGrnt;


/**
  @brief Downlink allocation details per TB. */
typedef struct rgSchDlTbAllocInfo
{
   Bool          schdlngForTb;/*!< Indicates if this TB has been scheduled */
   /*ccpu00120365:-ADD-is this TB disabled. Refer to 36.213-7.1.7.2 */
   Bool          isDisabled; /*!< Indicates if this TB is disabled */
   U32           bytesReq;    /*!< Number of bytes required to be allocated
                                 for each TB: filled in by RR/MAX C/I/PFS */
   U32           bytesAlloc;  /*!< Num of bytes allocated for each TB */
   U8            iTbs;        /*!< Itbs for allocation for this allocation
                               * of TB */
   U8            imcs;        /*!< Imcs for allocation for this allocation
                               * of TB */
   U8            noLyr;       /*!< No. of SM layers for this TB transmission */
   RgSchDlHqTbCb *tbCb;       /*!< address of TB Control Block */
#ifdef RG_5GTF
   RgSchDlCmnGrnt   cmnGrnt;     /*! < Alloc info to store 5GTF RAR sched */
#endif
}RgSchDlTbAllocInfo;

/**
  @brief Downlink MIMO related allocation details. */
typedef struct rgSchMimoAllocInfo
{
   Bool    hasNewTxData;/*!< Indicator from Specific Sched to Common.
                         * Set, if UE has new data available for clubbing
                         * with RETX in case of TM3 and TM4 */
   Bool    swpFlg;      /*!< Swap Flag to indicate TB to CW association
                         * incase of 2 TB transmission */
   U8      precIdxInfo; /*!< Precoding index information stored */
   U8      numTxLyrs;   /*!< Number of SM layers scheduled for Transmission */
}RgSchMimoAllocInfo;


/**
  @brief Downlink RB allocation details. */
typedef struct rgSchDlRbAlloc
{
   CmLteRnti rnti;          /*!< RNTI for allocation: used only for RARs and
                                 dedicated transmissions */
   U8 rbsReq;               /*!< Number of RBs required to be allocated:
                                 filled in by RR/MAX C/I/PFS */
   RgSchDlSf *dlSf;         /*!< DL sub-frame for which allocation is to be
                                 done: filled in by RR/MAX C/I/PFS */
   TfuDciFormat dciFormat;  /*!< DCI format for the allocation */
   U8 raType;               /*!< Resource allocation Type */
   RgSchPdcch *pdcch;       /*!< Pointer to allocated PDCCH */
   union allocInfoU
   {
      RgSchDlRbAllocRaType0 raType0; /*!< Resource allocation type 0 information */
      /* Added support for SPS*/
#ifdef LTEMAC_SPS
      RgSchDlRbAllocRaType1 raType1; /*!< Resource allocation type 1
                                       information */
#endif
      RgSchDlRbAllocRaType2 raType2;  /*!< Resource allocation type 2 information */
   } allocInfo;             /*!< Resource allocation information */
   U8 rbsAlloc;             /*!< Number of RBs allocated */
   U8      numRapids;       /*!< Number of RAPIDs serviced within RAR*/
   /* Nprb indication at PHY for common Ch */
   U8      nPrb;            /*!< NPRB column num. either 2 or 3 */
   U32     raIndex;
   Bool    schdFirst;
   /* Add BI sub-header size to the tbSize requirement */
   U8      biEstmt;
   RgSchMimoAllocInfo mimoAllocInfo; /*!< Mimo specific allocation params */
   RgSchDlTbAllocInfo tbInfo[2]; /*!< Allocation information for each TB. */

   /* Added support for SPS*/
#ifdef LTEMAC_SPS
   U8      cqiForTx;     /*!< Assumed value of CQI at which transmission is
                           scheduled */
   RgSchDlSfAllocInfo resAllocInfo; /*!< Resource allocation information for
                                      the current allocation for
                                      RA type 0, 1 and 2 */
   Bool    isSchdBySps;  /*!< Indicates if this UE is already scheduled by SPS
                           module */
   CmLteTimingInfo spsSchdTime; /*!< Timing info at which UE was scheduled by
                                  SPS module */
#endif
   Void *              laaCb;
#ifdef EMTC_ENABLE
   RgSchPdcch          *mpdcch;       /*!< Pointer to allocated PDCCH */
   RgSchEmtcDlRbAlloc  emtcAllocInfo; /*!< EMTC DL RB alloc Info */
#endif
#ifdef RG_5GTF
   U8                  vrbgReq;
#endif
} RgSchDlRbAlloc;

/**
  * @brief
  * Uplink Grant Information per UE.
  */
typedef struct rgSchUlGrnt
{
   U8           iMcs;       /*!< Original MCS index */
   U8           iMcsCrnt;   /*!< Current MCS index of the grant */
   U8           hop;        /*!< Hopping flag, set to 0 in this version */
   U8           rbStart;    /*!< Start Resource block of allocation */
   U8           numRb;      /*!< Number of resource blocks allocated */
   U8           tpc;        /*!< TPC command for the uplink grant */
   U8           nDmrs;      /*!< n1DMRS for the uplink grant */
   U8           delayBit;   /*!< Delay bit, for msg3 grant, set to 1 */
   /* Added for Uplink Adaptive retransmission */
   Bool         isRtx;      /*!< To indicate if this grant is for a RETX */
   /* To include the length and ModOrder in DataRecp Req. */
   U16          datSz;      /*!< Length of the Data */
   TfuModScheme modOdr;     /*!< Modulation order */
#ifdef RG_5GTF
   U8           vrbgStart;
   U8           numVrbg;
   U16          rbAssign;
   U8           xPUSCHRange;
   U8           SCID;
   U8           PMI;
   U8           uciOnxPUSCH;
   U8           hqProcId;
   Bool         ndi;
   TfuDciFormat dciFrmt;
   U8           numLyr;
#endif
} RgSchUlGrnt;


/* Added for Uplink Adaptive retransmission */
/**
  * @brief
  * Uplink retransmission allocation.
  */
struct rgSchUlRetxAlloc
{
   CmLteRnti       rnti;      /*!< RNTI */
   U8              numSb;     /*!< Number of subbands */
   U32             tbSz;      /*!< Transmission Block Size */
   U8              iMcs;      /*!< Original MCS index */
   RgSchUeCb       *ue;       /*!< UE assocated with allocation */
   Bool            forMsg3;   /*!< If allocation is for Msg3 */
   /* PHR handling for MSG3 */
   RgSchRaCb       *raCb;     /*!< Pointer to RaCb, if forMsg3 = TRUE */
#ifdef EMTC_ENABLE
   RgSchEmtcUlGrnt  emtcGrnt;
#endif
#ifdef RG_5GTF
   TfuDciFormat dciFrmt;
   U8           numLyr;
   U8           vrbgStart;
   U8           numVrbg;
   TfuModScheme modOdr;     /*!< Modulation order */
#endif
};

/**
  * @brief
  * Hole information, pertaining to uplink bandwidth
 */
typedef struct rgSchUlHole
{
   U8 start;      /*!< Beginning of hole */
   U8 num;        /*!< Number of elements making up the hole */
   struct rgSchUlHole *prv; /*!< Previous hole */
   struct rgSchUlHole *nxt; /*!< Next hole */
   struct rgSchUlAlloc *nxtAlloc; /*!< Next allocation */
   struct rgSchUlAlloc *prvAlloc; /*!< Previous allocation */
} RgSchUlHole;

/**
  * @brief
  * Uplink allocation
  */
struct rgSchUlAlloc
{
   RgSchUlHqProcCb *hqProc;         /*!< HARQ process */
   CmLteRnti       rnti;            /*!< RNTI */
   U8              sbStart;         /*!< Subband start idx */
   U8              numSb;           /*!< Number of subbands */
   RgSchUlGrnt     grnt;            /*!< Grant info */
   /* Not keeping grant attributes now */
   RgSchPdcch      *pdcch;          /*!< Associated PDCCH */
   RgSchUeCb       *ue;             /*!< UE assocated with allocation */
   Bool            forMsg3;         /*!< If allocation is for Msg3 */
   Bool            isMsg3CCCHSdu;   /*!<Msg3 contains CCCH Sdu */
   Bool            isMsg3CrntiCe;   /*!< Msg3 contains CRNTI CE */
   /* PHR handling for MSG3 */
   RgSchRaCb       *raCb;           /*!< RaCb, if forMsg3 = TRUE */
   Bool            mrgdNewTxAlloc;  /*!< Alloc corresponding to newTx
                                     *   that was merged in the process of
                                     * comdining txSf and reTxSf. */
   RgSchUlAlloc    *prv;            /*!< Previous alloc */
   RgSchUlAlloc    *nxt;            /*!< Next alloc */
   RgSchUlHole     *prvHole;        /*!< Next hole */
   RgSchUlHole     *nxtHole;        /*!< Prev hole */
   /* UL_ALLOC_CHANGES*/
   void           *allocDbRef;     /*!< The db from which this allocation was made*/
   void           *holeDbRef;      /*!< The db from which this allocation was made*/
   Bool           isAdaptive;
#ifdef EMTC_ENABLE 
   CmLList               allocLink;
   RgSchEmtcUlGrnt       emtcGrnt;
   RgSchDlRbAllocRaType0 raType0;  /*!< Resource Alloction type zero for eMTC */
#endif /* EMTC_ENABLE */
};

/**
  * @brief
  * Allocation free pool for an allocation db
 */
typedef struct rgSchUlAllocMem
{
   U8            maxAllocs;     /*!< Maximum possible allocations per subframe */
   RgSchUlAlloc  *firstFree;    /*!< First free index */
   RgSchUlAlloc  *allocs;       /*!< Array of 'maxAllocs' elems */
} RgSchUlAllocMem;

/**
  * @brief
  * Allocation information for a subframe
 */
typedef struct rgSchUlAllocDb
{
   U8              count;      /*!< Number of allocations */
   RgSchUlAlloc    *first;     /*!< First allocation */
   RgSchUlAllocMem mem;        /*!< Alloc pool management */
} RgSchUlAllocDb;

/**
  * @brief
  * Hole free pool for a hole db
 */
typedef struct rgSchUlHoleMem
{
   U8           maxHoles;     /*!< Maximum possible holes per subframe */
   RgSchUlHole  *firstFree;    /*!< First free index */
   RgSchUlHole  *holes;        /*!< Array of 'maxHoles' elems */
} RgSchUlHoleMem;

/**
  * @brief
  * Holes information for a subframe
  */
typedef struct rgSchUlHoleDb
{
   U8             count;           /*!< Number of holes */
   RgSchUlHole    *first;       /*!< First hole */
   RgSchUlHoleMem mem;        /*!< Hole pool management */
} RgSchUlHoleDb;

/**
  * @brief
  * Uplink subframe information.
  */
typedef struct rgSchUlSf
{
   U8               idx;      /*!< Identifier for uplink subframe (range: 0-7)
                                - maps to HARQ proc ID */
   CmLListCp        reTxLst;  /*!< Retransmission List*/
#ifdef LTE_TDD
   RgSchTddPuschOffInfo puschOffset; /*!< PUSCH offset information */
   U8               ulSfIdx;  /*!< The Uplink subframe number 
                                   in the TDD frame. Range [2-9]
                                   Used only in TDD Cfg 0  */ 
#endif
   U32              totPrb;    /*!< Total PRB used in this sub frame */
   /* ccpu00129725 -DEL- removed Platform flag */
   /* Fix:ccpu00120610 add a counter to keep track of remaining allocations */
   U8               *allocCountRef; /*!< Allocation count in this
                                *  subframe, it is a reference to the actual
                                *  counter held in allocDb (no additional
                                *  logic needed to maintain this) */
   /* Added for Uplink Adaptive retransmission */
   RgSchUlAllocDb   *allocDb;  /*!< Allocation info */
   RgSchUlHoleDb    *holeDb;   /*!< Holes info */
   U8               availSubbands; /*!< Number of total available subbands
                                    * for the current sub-frame */
   U8   numACqiCount;        /*!< Used to Restrict 2 Aperiodic cqi per TTI*/ 
#ifdef RG_5GTF
   RgSchSfBeamInfo  sfBeamInfo[MAX_5GTF_BEAMS];      /*!< Per info Beam per sf*/
   U8               numGrpPerTti;     /*!< number of Group per TTI*/
	U8               numUePerGrp;      /*!< number of UE per group in TTI*/
#endif
} RgSchUlSf;

/* Nprb indication at PHY for common Ch */
/**
  * @brief
  * Bo report given for common logical channel.
  */
typedef struct rgSchClcBoRpt
{
   CmLList         boLstEnt;  /*!< Linked list entity for BO list */
   U32             bo;        /*!< Bo in bytes */
   CmLteTimingInfo timeToTx;  /*!< Time at which the BO needs to be satisfied
                                   (data to be sent to PHY) */
#ifdef EMTC_ENABLE
   U8              emtcDIReason;   /*!< Reason for DI message to send. */
   U8              pnb;            /*!< Paging narrowBand on which Ue performs reception of paging*/
#endif
   CmLteTimingInfo maxTimeToTx;
   U8              retxCnt;
   U16             i;
   U8              nPrb;      /*! NPRB column. Allowed values {2,3} */
   /* Corrected allocation for common channels */
   U8              mcs;
} RgSchClcBoRpt;

/**
  * @brief
  * Logical channel control block for BCCH and PCCH channels.
  */
typedef struct rgSchClcDlLcCb
{
   U8              lcId;                     /*!< Logical channel ID */
   Bool            si;
   CmLListCp       boLst;                    /*!< List of BOs reported (RgSchClcBoRpt) */
} RgSchClcDlLcCb;

/**
  * @brief
  * Transmission offset configuration for SRS.
  */
typedef struct rgSchSrsTxOffst
{
   U8 count;                       /*!< Number of offsets in the array */
   U8 offst[RGSCH_MAX_SRS_TX_OFFSET]; /*!< SRS TX offset value */
} RgSchSrsTxOffst;

/**
  * @brief
  * Cell Control block per cell.
  */
typedef struct rgSchSrsCfg
{
   /*ccpu00130768 - ADD - SRS CFG Present flag to enable/disable cell specific SRS*/
   Bool            isSrsCfgPres;  /*!< cell specific SRS CFG enable/disable flag */
   RgrSrsCfgPrd    srsCfgPrdEnum; /*!< SRS configuration period (in subframes) */
   RgrSrsBwCfg     srsBwEnum;     /*!< SRS Bandwidth configuration per cell.*/
   RgSchSrsTxOffst srsTxOffst;    /*!< Transmission offset for SRS */
/*ccpu00116923 - ADD - SRS present support*/
#ifdef TFU_UPGRADE
    U8           srsSubFrameCfg;/*!< SRS subframe configuration index per cell.
                                     Range - [0-15] */
#endif
} RgSchSrsCfg;


/**
  * @brief
  * MAC Configuration element for CRG interface.
  */
typedef struct rgSchCfgElem
{
   CmLList         cfgReqLstEnt;  /*!< Linked list entry for cfgReqLst */
   CmLteTimingInfo actvTime;      /*!< Activation time to apply configuration */
   struct rgrCfgS
   {
      Region reg;
      Pool   pool;
      RgrCfgReqInfo   *rgrCfgReq;    /*!< Configuration request information from
                                       RRM */
      RgrCfgTransId   transId;       /*!< Transaction ID for the config request */
   } rgrCfg;                         /*!< RGR configuration information */
} RgSchCfgElem;


/**
  * @brief
  * Configuration Control block per cell.
  */
typedef struct rgSchCfgCb
{
   CmLListCp  crntRgrCfgLst;    /*!< List of config requests recieved in the
                                     current TTI from RRM */
   CmLListCp  pndngRgrCfgLst;  /*!< List of pending config requests recieved in the
                                     previous TTIs from RRM */
} RgSchCfgCfb;

/**
  * @brief
  * Link of RNTIs managed by MAC.
  */
typedef struct rgSchRntiLnk
{
   U16 rnti;                /*!< RNTI */
   /*Fix: Overcome race condition between MAC and Scheduler delete*/
   CmLList rntiGrdPoolLnk;     /*!< Link for RNTI guard pool*/
   struct rgSchRntiLnk *prv;   /*!< Link to previous RNTI */
   struct rgSchRntiLnk *nxt;   /*!< Link to next RNTI */
} RgSchRntiLnk;

/**
  * @brief
  * Database of RNTIs managed by MAC.
  */
typedef struct rgSchRntiDb
{
   U16  rntiStart;         /*!< Start RNTI */
   U16  maxRntis;          /*!< Maximum number of RNTIs managed by MAC */
   U16  count;
   RgSchRntiLnk *freeRnti;   /*!< first free RNTI in the list */
   RgSchRntiLnk *lastRnti;   /*!< last RNTI in the list */
   RgSchRntiLnk *rntiPool;   /*!< Linked list of RNTIs */
   /*Fix:Overcome Race condition between MAC and Scheduler delete*/
   CmLListCp  rntiGuardPool; /*!< Holds guard pool for RNTIs when deleted in 
                                  Scheduler but not deleted at MAC */
} RgSchRntiDb;

/**
  * @brief
  * Random access information per cell.
  */
typedef struct rgSchRaInfoCb
{
#ifdef LTE_TDD
   U8              maxRaSize;                /*!< Number of RA-RNTIs stored
                                                  across radio frames */
   U8              lstSize;                  /*!< list size */
   CmLListCp       *raReqLst;                 /*!< Each array elem is linked
                                                   list of RgRaReqInfo
                                                   (per ra-rnti) */
#else
   /* ccpu00132523 Doubled the array size from MAX_RARNTI because window expiry 
    * is overlapping with newly received TACH reqs in max RAR window*/
   /* Array Size should be more than gap towards window start */
   CmLListCp       raReqLst[RGSCH_RAREQ_ARRAY_SIZE];  /*!< Each array elem is linked
                                                   list of RgRaReqInfo
                                                   (per max ra-rnti*2) */
#endif
   CmLListCp       raCbLst;                   /*!< List of RaCbs */
   CmLListCp       toBeSchdLst;               /*!< List of RaCbs to be
                                                   scheduled */
} RgSchRaInfoCb;
#ifdef EMTC_ENABLE
/** @brief This structure is part of the uplink HARQ process, this structure
 * encapsulates all the DRX related information.
 */
typedef struct rgSchDrxUlHqProcCb
{
   CmLList harqRTTEnt;   /*!< Entry into the HARQ RTT timer list */
   CmLList harqRetxEnt;   /*!< Entry into the harqRetxQ */

   U16     rttIndx;    /*!< Current Index into HARQ RTT Q */
   U16     reTxIndx;   /*!< Current Index into Re-Tx Q */
   U8      retxTmrReduction; /*!< Due to platform specific timing diff between
                                 UL HARQ processing and UL Scheduling, the 
                                 drx retx timer may start at a later time, than
                                 actual. That delay in starting the timer is 
                                 stored in this */   
                                     
} RgSchDrxUlHqProcCb;

#endif

/**
  * @brief
  * Uplink HARQ process information per UE
  */
struct rgSchUlHqProcCb
{
   U8                ndi;          /*!< NDI */
   U8                remTx;        /*!< Number of remaining transmissions */
   RgSchUlAlloc      *alloc;       /*!< Uplink allocation */
   /* Renamed rcvdDatInd to rcvdCrcInd */
   Bool              rcvdCrcInd;   /*!< Set to true when data rcvd, false when
                               decode failure. */
   U8                rvIdx;        /*!< Redundancy version index */
   TknU8             rvIdxPhy;     /*!< Whatever is given by PHY. To be inspected in case
                               of NACK.Always initialised to 'not present' */
   CmLList           reTxLnk;      /*!< Retransmission List */
   RgSchUlRetxAlloc  reTxAlloc;    /*!< Retransmission allocation
                                        information */
   Bool              isRetx;       /*!< Awaiting retransmission */
   U8                procId;       /*!< HARQ Process ID */
   U8                ulSfIdx;      /*!< UL Subframe Index */
#ifdef LTE_TDD
   /* Changes for passing iPhich at TFU*/
   U8                iPhich;       /*!< Needed to Calculate PHICH
                                        location. For TDD only */
#endif
#ifdef LTEMAC_SPS
   Bool              isSpsActvnHqP;   /*!< Set to true when this HqP is used for
                                           UL SPS Activation */
   Bool              isSpsOccnHqP;   /*!< Set to true when this HqP is used for
                                           UL SPS Occasion TX */
#endif
   void              *hqEnt;
#ifdef EMTC_ENABLE
  RgSchDrxUlHqProcCb  drxCb;  
  Bool                isDtx;
#endif
  /* 5gtf: TODO two links are not required */
  CmLList             lnk;      /*!< To link to inUse/Free Pool */
  CmLteTimingInfo     schdTime; /*!< SFN,SF of schd time */
};

/**
  * @brief
  * Uplink HARQ entity information per UE.
  */
typedef struct rgSchUlHqCb
{
   U8           maxHqRetx;        /*!< Maximum number of harq
                                   * re-transmissions */
   U8              numHqPrcs; /*!< Number of HARQ Processes */
#ifdef LTE_TDD
   RgSchUlHqProcCb *hqProcCb; /*!< Uplink harq process info */
#else
   RgSchUlHqProcCb hqProcCb[RGSCH_NUM_UL_HQ_PROC]; /*!< Uplink harq process info */
#endif
#ifdef LTE_L2_MEAS
   U8          numBusyHqProcs;    /*!< Total Num of Hq procs in use */
#endif
#ifdef EMTC_ENABLE
   void       *sch;
   RgSchUeCb  *ue;
#endif
   CmLListCp    free;      /*!< List of free HARQ processes */
   CmLListCp    inUse;     /*!< List of in-use HARQ processes */
} RgUeUlHqCb;

/**
  * @brief
  * Logical channel group.
  */
#ifdef LTE_L2_MEAS
/**
 * @brief
 * Structure to store values for Ra Preambles received values
 * */
typedef struct rgSchRaPreambles
{
   U8               dedPream;    /*!< Dedicated RA Preamble received */
   U8               preamGrpA;   /*!< Randomly selected preambles in low range */
   U8               preamGrpB;   /*!< Randomly selected preambles in high range */
}RgSchRaPreambles;

/**
 * @brief strutcure to store Avergae PRB usage for a given Time period in
 * Downlink.
 * */
typedef struct rgSchAvgPrbDl
{
   Bool             cntActive;   /* Set to TRUE if counting activated */
   CmLteTimingInfo  startTime;   /*!< Start Time */
   U8               timePrd;     /*!< Period For which meas to be done */
   U32              prbCount;    /*!< PRB Count Updated every Per TTI */
} RgSchAvgPrbDl;

/**
 * @brief strutcure to store Avergae PRB usage for a given Time period in
 * Uplink .
 * */
typedef struct rgSchAvgPrbUl
{
   Bool             cntActive;   /*!< Set to TRUE if counting activated */
   CmLteTimingInfo  startTime;   /*!< Start Time */
   U8               timePrd;     /*!< Period For which meas to be done */
   U32              prbCount;    /*!< PRB Count Updated every Per TTI */
} RgSchAvgPrbUl;

/** @brief
 *  L2 measurement control block.
 *  */
struct rgSchL2MeasCb
{
   CmLList            measLnk;       /*!< MeasCb node */
   LrgSchMeasReqInfo  measReq;       /*!< Request received for measurement */
   CmLteTimingInfo    startTime;     /*!< Time at which measurement started */
   RgInfPrbCfm        avgPrbQciUl;   /*!< Used to store result from MAC */
   Bool               cfmRcvd;       /*!< Used to check if confirm received from MAC */
                                     /*!< For average PRB usage in UL */
   U16                sfnCycle;      /*<! Count of Num of SFN wraps */  
   U32                dlTotalBw;     /*!< Total DL Bw between meas Req and Meas Send Req */
   U32                ulTotalBw;     /*!< Total UL Bw between meas Req and Meas Send Req */
};

typedef struct rgSchTbCnt
{
   U32    tbTransDlTotalCnt;  /*!< Total DL TB count */
   U32    tbTransDlFaulty;    /*!< Total DL Faulty TB count */ 
   U32    tbTransUlTotalCnt;  /*!< Total UL TB count */
   U32    tbTransUlFaulty;    /*!< Total UL Faulty TB count */
}RgSchTbCnt;   
#endif /* LTE_L2_MEAS */
/** @brief
 *  Control block to store measurement details per QCI.
 *  */
struct rgSchQciCb
{
   U8     qci;              /*!< QCI of the Logical Channel */
   U32    dlPrbCount;       /*!< Cumulative Prb Count for this QCI */
   U32    dlUeCount;        /*!<  Cumulative number of active UE's */
   U32    dlTotal_UeCount;  /*!< Cummulative count added for every sampling
                              Occasion*/
   U32    ulUeCount;        /*!<  Cumulative number of active UE's */
   U32    ulTotal_UeCount;  /*!< Cummulative count added for every sampling
                              Occasion*/
};
struct rgSchLcgCb
{
   /* Right now not keeping associated logical channels, searching for
    * associated channels needed only during config */
   U8    lcgId;  /*!< Group ID */
   Void  *sch;
#ifdef LTE_L2_MEAS
   U8               numLch;                    /*!< Number fo LC's for this LCG*/
   RgSchUlLcCb      *lcArray[RGSCH_MAX_LC_PER_UE]; /*!< Dedicated Uplink logical
                                                 channel per LCG */
#endif /* LTE_L2_MEAS */
   /* Added support for SPS*/
#ifdef LTEMAC_SPS
   U8    lcCnt;
   U8    lcId[RGR_MAX_SPS_LC];
#endif
};

/**
  * @brief
  * Uplink Dedicated Logical channel control block.
  */
struct rgSchUlLcCb
{
   Bool          isValid; /*!< If this struct holds valid values*/
   U8            lcId;    /*!< Logical channel ID */
   U8            lcgArrIdx;    /*!< Index of this LC in the LCG Array*/
   RgSchLcgCb    *lcg;    /*!< Logical channel group */
   RgrLchQosCfg  ulQos;   /*!< UL Qos parameters */
   /* After Merging from 2.1 to 2.2 */
   RgSchQciCb   *qciCb; /* QCI Details for this LC */
};

/**
  * @brief
  * Ul Grnt Info for Ho/Po UE in RAR.
  */
typedef struct rgSchHoPoUeGrnt
{
   U8      rapId;         /*!< dedicated rapId */
   U8      hop;           /*!< Hopping flag, set to 0 in this version */
   U8      rbStart;       /*!< Start Resource block of allocation */
   U8      numRb;         /*!< Number of resource blocks allocated */
   U8      tpc;           /*!< TPC command for the uplink grant */
   U8      iMcsCrnt;      /*!< Current MCS index of the grant */
   TknU16  ta;            /*!< Timing Adjustment */
   CmLList raRspLnk;      /*!< To link UE to RaRsp cont free list */
   /* Added support for SPS*/
   U16     datSz;         /*!< Length of the Data */
   Bool    cqiReqBit;     /*!< Aperiodic CQI is requested or not */
} RgSchHoPoUeGrnt;

/**
  * @brief
  * Uplink SPS scheduling information per UE
  */
typedef struct  rgSchCmnSpsUlUeSchdInfo
{
   U32             allocBwMask[RGSCH_SPS_ULBW_MASK_LEN]; /*!< Bitmask indicating the allocation info 
                                      for the UE. Bit set at position 'x'
                                      indicates subband 'x' is allocated */
   U8              scaledCqi;    /*!< Assumed value of CQI for transmission */
   U16             actvSfLstIdx; /*!< Index into cell-wide UL SPS sub-frame 
                                      table during activation */
   CmLteTimingInfo ulSpsActvOcc; /*!< Time at which SPS is activated/reactivated
                                      for the UE */
   CmLteTimingInfo crntUlSpsOcc;/*!< Key into the list of UL SPS active 
                                      UEs: next UL SPS ocassion */
   RgSchUlAlloc    allocInfo;    /*!< Allocation information of the UE */
   RgSchUlAlloc    *crntAlloc;   /*!< Allocation pointer in the subframe */
   RgSchUlSf       *crntAllocSf;          /*!< sf in which crntAlloc was made */
} RgSchCmnSpsUlUeSchdInfo;

/**
  * @brief
  * Uplink information for SPS per UE
  */
typedef struct rgSchCmnUlUeSpsInfo
{
   CmLList                 actvUeLstEnt; /*!< Linked List entry for UL SPS
                                              active UE list*/
   CmLList                 pndngUeLstEnt;/*!< Linked List entry for UE list with
                                              pending SPS action:
                                              activation/reactivation/release */
   Bool                    isUlSpsActv;   /*!< Indicates if UE's UL SPS is 
                                              active currently */
   U8                      measGapMask[RGSCH_SPS_MG_MASK_LEN];  /*!< Indicates the UL sub-frames with 
                                               ongoing measurement gap. Mask is
                                               of size (80/8) */
   U8                      state;         /*!< State of the UE: can be one of
                                               ACTV_REACTV_SENT (pdcch for 
                                               (re)activation sent),REL_SENT, 
                                               ULSPS_ACTV */
   Bool                    pdcchToRetx;   /*!< set to TRUE if DTX occurs at the
                                               (re)activated SPS occasion */
   RgSchCmnSpsUlUeSchdInfo ulSpsSchdInfo; /*!< Scheduled info for UL SPS 
                                               active UE */

   U8                      spsOccIdx;         /*!< N value for nxt sps occasion */

   U32                     packetSize;       /*!< Packet size for SPS - SPS allocation*/
   U32                     spsSduSize;       /*!< SDU Size recvd on SPS Occasion*/
   U32                     spsGrantSize;     /*!< Grant size for SPS allocation */

   CmLteTimingInfo         lastSpsDatRecvdTime; /*!< timing info on which the  
                                                     SPS data is recieved*/
   CmLListCp               *spsList; /*!< Pointer to the SPS list of which
                                      UE is a part */
   U32                     actPdcchCrcFailCount; /*!< Num of consecutive CRC fails for 
                                                   SPS Activation PDCCH */
   U32                     crcFailCntSpsOcc;     /*!< Consecutive CRC fail for SPS Occasion
                                         TX */
   U32                     relPdcchSntCnt;     /*!< Num ber Rel PDCCH sent to UE*/
#ifdef RGSCH_SPS_STATS
   U32                     crcFailStats;
   U32                     crcPassStats;
   U32                     totalBsr;
   U32                     numBsrRpt;
#endif
} RgSchCmnUlUeSpsInfo;



/**
  * @brief
  * Uplink control block for UE specific information.
  */
typedef struct rgSchUeUlCb
{
   RgSchUlLcCb      lcCb[RGSCH_MAX_LC_PER_UE]; /*!< Dedicated Uplink logical channel
                                              information */
   RgSchLcgCb       lcgArr[RGSCH_MAX_LCG_PER_UE]; /*!< Logical channel groups */
   U8               ulInactvMask; /*!< Bitmask indicating if UE is inactive for UL scheduling */
   CmLList          ulInactvLnk;  /*!< Link to the inactive UE List for UL */
   RgSchHoPoUeGrnt  rarGrnt;      /*!< UE's RAR grant Information */
   RgrUeTxAntSelCfg ulTxAntSel;/*!< UL Transmit antenna selection Cfg Info */
#ifdef RGR_V1
   /*  Added periodic BSR timer */
   RgrUeBsrTmrCfg   bsrTmrCfg;    /*!< BSR timers configuraiton for the UE */
#endif
   /* Added support for SPS*/
#ifdef LTEMAC_SPS
   RgrUeSpsUlCfg    ulSpsCfg;       /*!< UL SPS configuration information */
   U8               explicitRelCnt; /*!< Number of subframes after sending
                                         release to consider it successful */
   U16              spsPrdcty;      /*!< Shall store the SPS periodicity of the
                                         UE. Needed to round-off in case of
                                         TDD */
   CmLteTimingInfo   relPdcchSchdTime;     /*!< This field holds the scheduled time 
                                          of SPS rel PDCCH*/
#endif
#ifdef LTE_L2_MEAS
   U32               nPrb;          /*!< Number of resource block allocated */
#endif
#ifdef TFU_UPGRADE
   U8                betaHqOffst; /*!< Delta HARQ offset used in the case where HARQ
                             feedback is multiplexed with PUSCH data.  */
   U8                betaCqiOffst;/*!< Beta offset for CQI[0-15]. Given by RRC*/
   U8                betaRiOffst;/*!Beta offset for RI[0-15]. Given by RRC */
#endif
   U32               cqiRiSz;   /*!<Size of Periodic/Aperiodic CQI or RI depending
                                     on the occassion */
   U32               betaOffstVal; /*!< beta offset of CQI or RI depending 
                                     on the occassion*/
   U32               maxBytesPerUePerTti;         /*!< Maximum bytes that can be allocated 
                                       *   in 1 scheduling instance per UE */
   U32               minReqBytes;     /*!< The buffer amount of lcg0 */
   U32               totalBsr;        /*!< Total BSR as reported by UE (in bytes) */
   U32               nonLcg0Bs;       /*!< Total BSR for LCG1/LCG2/LCG3 */
   U32               nonGbrLcgBs;     /*!< Total BSR as reported by UE (in bytes) for non-GBR LCG's*/
   U32               effBsr;          /*!< BSR yet to be satisfied in allocations */

   U32               cfgdAmbr;        /*!< Byte rate of UE Ambr per Refresh Cycle */
   U32               effAmbr;         /*!< Remaining Bytes of Byte rate available in a refresh Cycle */
   CmLteTimingInfo   ulTransTime;     /*!< Timing info of the latest reception of 
                                     any UL data/signal from UE */
#ifdef LTEMAC_SPS
   RgSchCmnUlUeSpsInfo ulSpsInfo;     /*!< UL SPS information for UE */
#endif
   Bool              isUlCaEnabled;   /*!< If TRUE UL CA is enabled for the UE */
   CmLteCellId       ctrlOnServCellIdx[RGSCH_ULCTRL_RECP_DIST]; /*!< 
                                 * Serving Cell Idx on which UL CTRL INFO is to
                                 * be received of all configured 
                                 * serving cell */

  Bool               useExtBSRSizes; /*!< If TRUE used R10 Extended BSR Size Table*/
} RgSchUeUlCb;

/**
  * @brief
  * Downlink CCCH Tx/Retx schedule information.
  */
typedef struct rgSchCcchSchdInfo
{
   U32     totBytes;       /*!< Total bytes allocated */
   CmLList retxLnk;        /*!< To link hqP to retx Queues */
   U8      rvIdx;          /*!< Rv Index */
} RgSchCcchSchdInfo;

/**
  * @brief
  * Downlink Grant Information per UE.
  */
typedef struct rgSchDlGrnt
{
   U16     schdTime;       /*!< Time at which Harq proc has been scheduled */
   U8      rbStrt;         /*!< Starting RB of the allocation */
   U8      numRb;          /*!< Number of RBs allocated */
   U8      iMcs;           /*!< Index to the MCS */
   U8      rv;             /*!< RV for HARQ (re)transmission */
   U8      rvIdx;          /*!< RVIdx for HARQ(re)transmission */
#ifdef RG_5GTF   /* ToDo:: Anoop need to check for other fields required*/
   U8      vrbgStart;
   U8      numVrbg;
   U16     rbAssign;
   U8      xPDSCHRange;
   U8      SCID;
   TfuDciFormat dciFormat;  /*!< DCI format for the allocation */
	/* ToDo */
#endif
} RgSchDlGrnt;

/**
  * @brief
  * Logical channel data as being scheduled by the scheduler.
  */
typedef struct rgSchLchAllocInfo
{
   U8       lcId;       /*!< Logical channel ID */
   U32      schdData;   /*!< Scheduled logical channel data */
} RgSchLchAllocInfo;

/** @brief This structure is part of the downlink HARQ process, this structure
 * encapsulates all the DRX related information.
 */
typedef struct rgSchDrxDlHqProcCb
{
   CmLList harqRTTEnt;   /*!< Entry into the HARQ RTT timer list */
   CmLList harqRetxEnt;   /*!< Entry into the harqRetxQ */

   U16     rttIndx;    /*!< Current Index into HARQ RTT Q */
   U16     reTxIndx;   /*!< Current Index into Re-Tx Q */
   U8      retxTmrReduction; /*!< Due to platform specific timing diff between
                                 DL HARQ processing and DL Scheduling, the 
                                 drx retx timer may start at a later time, than
                                 actual. That delay in starting the timer is 
                                 stored in this */   
                                     

} RgSchDrxDlHqProcCb;

/** @enum TfuDciFormat
 * This Enum has values for Harq Proc TB state.
 */
typedef enum
{
   HQ_TB_ACKED=0,
   HQ_TB_NACKED,
   HQ_TB_WAITING
}RgSchHqTbState;
/**
  * @brief
  * Downlink TB information per DL HqProc.
  */
struct rgSchDlHqTbCb
{
   U8                tbIdx;          /*!< Index of this TB (0/1). Initialized
                                          at HqEnt allocation */
   U32               tbSz;           /*!< TB size scheduled for this TB */
   U8                txCntr;         /*!< Transmission counter */
   U8                ndi;            /*!< New Data Indicator 0 or 1*/
   TknU8             schdTa;      /*!< TA scheduled to be sent */
   /* CR timer implementation changes*/
   U8                contResCe;
   U8                numLch;         /*!< No. of DL Logical Channels scheduled
                                          for this TB */
   CmLteTimingInfo   timingInfo;     /*!< This field holds the scheduled time */
   RgSchLchAllocInfo lchSchdDataArr[RGSCH_MAX_NUM_DED_LC];
   RgSchLchAllocInfo *lchSchdData;
   RgSchCcchSchdInfo ccchSchdInfo;/*!< CCCH TX/RETX scheduler Info */
   U8                fbkRepCntr;     /*!< Counter to ANRep Feedback */
   U8                fbkRecpRepCntr; /*!< Counter to ANRep Feedback */
   CmLList           anRepLnk[6];    /*!< Links for ANRep we need
                                          these many links to
                                          simultaneously hold the
                                          hqProcs in multiple
                                          subframes. */
   RgSchDlSf         *crntSubfrm[6]; /*!< Pointer to subframes */
   U8          ackCount;   /*!< Counter for ACKs */
   U8          isAckNackDtx; /* ACK or NACK or DTX*/
   U8          nackCount;  /* NACK Counter */
   U8          dtxCount;   /* DTX Counter */
   RgSchDlGrnt       dlGrnt;      /*!< Scheduler grant */
   Bool              taSnt;       /*!< TA sent or not */
#ifdef LTE_TDD
   CmLteTimingInfo   fdbkTime;    /*!< Expected feedback time */
   U8                m;           /*!< Subframe Order Index within the subframe
                                       downlink association index K */
   RgSchTddSfType    sfType;      /*!< Identifies the first Tx subframe type */ 
   U8                pucchFdbkIdx;/*!< DL subframe idx for which fdbk is expected */
   U8                dai;          /*!< DAI associated with this TB.
                                          THis is used for F1BCS resource calulcation */
#ifdef XEON_TDD_SPCL
   U8                initTxNumRbs;/*!< Number of Rbs Allocated in First TX */
#endif

#endif
   /* 3.1 MIMO */
   U8                numLyrs;     /*!< Number of layers using which this TB is
                                       being transmitted */
   RgSchHqTbState    state;       /*!< State of this Trasport Block */
   RgSchDlHqProcCb   *hqP;        /*!< Reference to the TB container */
   /* Freeing up the HARQ proc blocked for
    * indefinite time in case of Retx */
   U8                cntrRetxAllocFail; /*!< Number of times allocation failed
                                            for the retransmission of this TB */
#ifdef LTE_ADV
   TknU8             schdSCellActCe;   /* !< SCell Act values and whether
                                          scheduled or not */
#endif
  U32                firstSchedTime; /*!< First time when the TB was scheduled */
  U8                 minRlcReordrTmr; /*!< Min RLC reorder timer of all LCs
                                        scheduled in this hqP*/
};


/**
  * @brief
  * Downlink HARQ process information per UE.
  */
struct rgSchDlHqProcCb
{
   CmLList           lnk;         /*!< To link with other HARQ processes */
   RgSchDlHqEnt      *hqE;        /*!< HARQ entity */
#ifdef LTE_TDD
   U8                txCntr;      /*!< Transmission counter */
#endif
   U8                procId;      /*!< Harq process ID */
   Void              *sch;        /*!< Cmn sched Hq control block */
   /* 3.1 MIMO */
   RgSchDlHqTbCb     tbInfo[2];   /*!< TB related information for 1+1(if any) TBs */
   RgSchDrxDlHqProcCb   drxCb;   /*!< DRX control block */
#ifdef TFU_UPGRADE
   U8                tbCnt;     /*!< Count of the number TBs being sent with a
                                  Reception Requesti */
#endif
#ifdef LTEMAC_SPS
   TknU16       spsN1PucchRes;   /*!< N1Pucch resource for HARQ process at SPS
                                      ocassions: present only when HqP contains
                                      Data without PDCCH */
#endif
   /* Fix: syed having a hqP added to Lists for RB assignment rather than
    * a UE, as adding UE was limiting handling some scenarios */ 
   CmLList reqLnk;          /*!< Link for list of UEs/RACBs to be scheduled */
   CmLList schdLstLnk;      /*!< Link for list of scheduled/non-scheduled
                                   allocations: used by RB allocation module*/
   Bool hasDcch;            /*!< Set to TRUE if DCCH channel is present in 
                                 scheduling grant over this hqProc */
   Bool cwSwpEnabled;         /*!< Set if CW swap enabled */
/*f1b_Sprint3*/
   U8        tpc;           /*!< TPC command to be used for Secondary cell
                              N1PUCCH resource Index*/
/*f1b_Sprint3*/
   /*CA Dev Start*/
   CmLList     hqPSfLnk;
   /* Shifted from TbCb to HqCb*/ 
   RgSchPdcch        *pdcch;      /*!< Pdcch Allocated for this Hq TB */
   RgSchDlSf         *subFrm;     /*!< Subfrm for which this TB is assgnd */
#ifdef EMTC_ENABLE
   Void               *emtcHqInfo;/*!< emtc specfic hq info */
#endif
   U8                dlDai;        /*!< DL DAI assosciated with this harq proc */
   U8                ulDai;        /*!< UL DAI assosciated with this harq proc */
#ifdef BRDCM
   /*CA Dev End*/
   U32               isPuschFdbk;
#endif
   Void *         laaCb;
   CmLListCp   *hqPLst;    /*!< pointer to either inUse of free list of hqE */
   TfuDciFormat prevDciFormat; /*!< Previous alloction DCI format used for ERR Ind prcessing */

   /* LAA DBG Only */
   U32 tbSizeAtEstimate[2];
   U32 tbSizeAtFnlz[2];
   U32 tbSizeOfMvdTb[2];
   U32 itbsAtEstimate[2];
   U32 prbAtEstimate;
};

/**
  * @brief
  * Downlink HARQ entity information per UE.
  */
struct rgSchDlHqEnt
{
   RgSchRaCb    *raCb;     /*!< Parent RACB when attached to RACB */
   RgSchUeCb    *ue;       /*!< Parent UE */
   CmLListCp    free;      /*!< List of free HARQ processes */
   CmLListCp    inUse;     /*!< List of in-use HARQ processes */
   U8           maxHqTx;   /*!< Maximum number of harq transmissions */
   RgSchDlHqProcCb *msg4Proc; /*!< Points to MSG4 HARQ process */
#ifdef RGR_V1
   /* CR timer changes*/
   RgSchDlHqProcCb *ccchSduProc; /*!< Points to CCCH SDU HARQ process
              to identify feedback for CCCH SDU
              transmissions done without cont res CE*/
#endif
   U8              numHqPrcs; /*!< Number of HARQ Processes */
#ifdef LTE_TDD
   RgSchDlHqProcCb *procs; /*!< Downlink harq processes */
#else
   RgSchDlHqProcCb procs[RGSCH_NUM_DL_HQ_PROC]; /*!< Downlink harq processes */
#endif
   Void         *sch;      /*!< Scheduler specific Info */
   /* CA Dev Start */
   RgSchCellCb  *cell;     /*Contains the pointer to coresponding CellCb*/
   /* CA Dev End*/
};


/**
  * @enum rgSchTaState
  * Enumeration of time alignment states.
  */
typedef enum rgSchTaState
{
   RGSCH_TA_IDLE = 0,      /*!< TA is to be scheduled */
   RGSCH_TA_TOBE_SCHEDULED,/*!< TA is to be scheduled */
   RGSCH_TA_SCHEDULED      /*!< TA is scheduled */
} RgSchTaState;

/**
  * @brief
  * Downlink Time alignment information per UE.
  */
typedef struct rgSchUeDlTaCb
{
  U16       cfgTaTmr; /*!< Configured TA timer value */
  U8        ta;       /*!< TA value for UE */
  U8        numRemSf; /*!< Number of subframes left to apply TA */
  RgSchTaState state;    /*!< Indicates HARQ state for TA */
  /*rg003.301[ccpu00121813] ADD added new var*/
  Bool     outStndngTa;  /*!< if new TA is rcvd before it gets
                               applied at UE*/
  U8       outStndngTaval; /*!< outstanding TA val */
} RgSchUeDlTaCb;

/**
  * @brief
  * Downlink Dedicated Logical channel control block.
  */
struct rgSchDlLcCb
{
   U8            lcId;    /*!< Logical channel ID */
   U32           bo;
   RgSchUeCb     *ue;     /* UE to which this LC belongs to */
   /* Not validating DlQos for DCCH */
   CmLteLcType   lcType;   /* Logical channel Type*/
   RgSchQciCb    *qciCb;  /*!< Pointer to ACI Control Block */
   /* Added support for SPS*/
#ifdef LTEMAC_SPS
   RgrLchSpsCfg   dlLcSpsCfg;  /*!< SPS configuration for DL logical channel */
#endif
#ifdef CCPU_OPT
   U16           estRlcHdrSz;  /*!< Estimated RLC header size */
   Bool          staPduPrsnt;  /*!< Indicate the status pdu present or not*/
#endif
   U32           staPduBo;     /*!< Indicate the Status PDU BO.
                                    This amount is already included in original BO */
   Void          *sch;        /*!< Holds Scheduler Info pointer */
   U32           prbsReqd;    /*!< PRBs reqd in DL for each UE in MUE scenario*/
   CmLList       schdLnk;     /*!< Link to ueCb inserted in toBeSchdList in DL*/
   U32           oldestSduArrTime; /*!< oldest SDU arrival time for this LC */
   CmLList       lcFlowCntrlEnt; /*!<link to Lc in flow Cntrl lst */
   CmLList       lcPdbFbkLnk;  /*!<link to LC in LC flow contrl monitoring lst */
   Bool          setMaxUlPrio; /*!<set if Poll bit is sent in the PDU */
   Bool          setMaxDlPrio; /*!<set if there is a RLC status PDU in DL */
   Void*         laaCb;

};
/*f1b_Sprint3*/

typedef struct rgSchN1PucchResCb
{
   U16 n1PucchIdx;  /* !< N1 resource */ 
   CmLList n1Lnk;  /* !< Link for adding into dlsf N1 used list */
}RgSchN1PucchResCb;
typedef struct rgSchN3PucchResCb
{
   U16 n3PucchIdx;  /* !< N3 resource */ 
   CmLList n3Lnk;   /* !< Link for adding into dlsf N3 used list */
   U8 sCellIdx;      /* !< Allocated by which scell */ 
}RgSchN3PucchResCb;

typedef struct rgSchSCellN3ResCb
{
  U8                 antP0N3ResCount; /* !< Number of N3 res for antenna port 0*/
  U8                 antP1N3ResCount;  /* !< Number of N3 res for antenna port 0*/
  RgSchN3PucchResCb  antP0N3Res[RG_SCH_MAX_NUM_N3PUCCH_PER_UE]; /* !< Antenna Port 0 N3 resources*/
  RgSchN3PucchResCb  antP1N3Res[RG_SCH_MAX_NUM_N3PUCCH_PER_UE]; /* !< Antenna Port 1 N3 resources*/
}RgSchSCellN3ResCb;

typedef struct rgSchSCellN1ResCb
{
  U8                 cw1N1ResCount; /* !< Number of N1 res for CW 1*/
  U8                 cw2N1ResCount;  /* !< Number of N1 res for CW 2*/
  RgSchN1PucchResCb  cw1N1Res[RG_SCH_MAX_NUM_N1PUCCH_PER_UE]; /* !< CW1 N1 resources*/
  RgSchN1PucchResCb  cw2N1Res[RG_SCH_MAX_NUM_N1PUCCH_PER_UE];  /* !< CW2 N1 resources*/
}RgSchSCellN1ResCb;
/*CA Dev Start*/
#ifdef LTE_ADV
typedef struct rgSchN3PucchRes
{
   U16 n3PucchIdx;  /* !< N3 resource */
   U8  tpcIdx;
}RgSchN3PucchRes;
#endif
typedef struct rgSchDlHqInfo
{
/*f1b_Sprint3*/
#ifdef LTE_TDD
   U8                     numOfCells; /*!<This will be used to store number of unique cells in
                                        this subframe*/
   Bool                   isSCellPdschPres; /*!< Flag to indicate whether any PDSCH transmission
                                             present in secondary cell. Used for CSI/HARQ collision
                                              */
#endif
   Bool                   isPuschHarqRecpPres;/*!< Flag to indicate pusch recp req is pres or not */
   RgrSchFrmt1b3TypEnum   uciFrmtTyp; /*!< Store value of format1b or format3.
                                       */
   U8                     totalTbCnt; /*!<  This will be used to store total number of TBs 
                                        across all harqProcs.It will be used to identify 
                                        PUCCH format type to be used in this SF*/
   CmLList                dlSfUeLnk; /*!< list entry into dlsf->ueLst */
   Void *                laaCb;
   CmLListCp              hqPLst;    /*!< This is a list of hq proc per DL  
                                        SF which are scheduled in that SF.
                                        The number of harq procs awaiting 
                                        feedback for the same subframe depends on 
                                        mode TDD or FDD and max number of Carriers 
                                        that can be aggregated */
#ifdef LTE_ADV
  RgSchN3PucchRes       n3ScellPucch;
#endif
}RgSchDlHqInfo;

/*CA Dev End*/
/** @brief This structure contains the Measurement gap configuration for an UE.
 */
typedef struct rgUeMeasGapCfg
{
   Bool isMesGapEnabled;    /*!< Is Measuremnet gap enabled or disabled */
   U8   gapPrd;             /*!< Gap period 40ms/80ms */
   U8   gapOffst;           /*!< Gap offset - Vaue is 0 to 1*/
} RgUeMeasGapCfg;

/**
  @brief Measurement Gap related information per UE. */
typedef struct rgSchUeMeasGapCb
{
   Bool              isMesGapEnabled;/*!< TRUE if Measurement gap is enabled for this UE */
   U8                isMeasuring;  /*!< Set to TRUE during measurement gap */
   U8                gapPrd;     /*!< Measurement gap period configuration for the UE */
   U8                gapOffst;   /*!< Measurement gap offset for the UE */
   CmLList           measQLnk;   /*!< To Link to the measurement gap list */
   CmLList           ackNakQLnk; /*!< To Link to the ACK NACK Rep list */
   CmTimer           measGapTmr;  /*!< Timer for Measurement Gap */
   CmTimer           measGapUlInactvTmr; /*!< UL Inactive timer for measurement gap */
   CmTimer           measGapDlInactvTmr; /*!< DL Inactive timer for measurement gap */
} RgSchUeMeasGapCb;

/**
  @brief ACK-NACK repetition related information per UE. */
typedef struct rgSchUeAckNakRepCb
{
   Bool        isAckNackEnabled;  /*!< Is ACK/NACK Enabled*/
   U8          isAckNakRep;  /*!< Set to TRUE during ACK-NACK repetition prd */
   U8          cfgRepCnt;         /*!< Configured value for the repetition counter */
   U8          repCntr;           /*!< Actual repetition counter */
   U16         pucchRes;          /*!< PUCCH resource for repetition */
   CmTimer     ackNakRepUlInactvTmr; /*!< UL Inactive timer for ack-nack repetition */
   CmTimer     ackNakRepDlInactvTmr; /*!< DL Inactive timer for ack-nack repetition */
   CmTimer     ackNakRepTmr;      /*!< Timer for ack-nack repetition */
   CmLList     ackNakRepLnk;      /*!< ACK NACK repetition queue link */
   CmLListCp   *prsntQ;   /*!< Pointer to the Queue that this UE is current
                            present in. */
} RgSchUeAckNakRepCb;

/**
  * @brief
  * UE's MIMO specific information.
  */
typedef struct rgSchUeMimoInfo
{
   RgrTxMode          oldTMode;     /*!< UE's Previous Transmission Mode */
   RgrTxMode          txMode;       /*!< UE's Transmission Mode */
   TknU32             doa;          /*!< DOA indicator for this UE */
   Bool               puschFdbkVld; /*!< True if Precoding Info in PDCCH has to be
                                         in-accordance with the latest PUSCH report */
   TfuDlCqiPuschInfo  puschPmiInfo; /*!< PUSCH report details for explicit PMI
                                       * information to PHY during a PDSCH */
   RgrCodeBookRstCfg  cdbkSbstRstrctn; /*!< Codebook subset restriction defined as per
                                       * 36.331 section 6.3.2. As of now, this info
                                       * is not processed by MAC. SCH shall use the
                                       * PMI reported by UE unconditionally.*/
#ifdef DL_LA
   S32                txModUpChgFactor; /*!< tx mode chnage factor for step up*/
   S32                txModDownChgFactor; /*!< tx mode chnage factor for step
                                            Down*/
#endif 
}RgSchUeMimoInfo;

/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE

/** @brief This structure that stores the length of Bits that
*     will be received over PUSCH for Aperiodic Mode 3-1.
*/
typedef struct rgSchCqiRawPuschMode31
{
   U8                            wideBCqiCw0;    /*!< Length of Wideband CQI Codeword 0 */
   U8                            totLenSbDiffCqiCw0;    /*!< Length of SubBand Differential CQI Codeword 0 */
   U8                            r1WideBCqiCw1;    /*!< Length of Wideband CQI Codeword 1 for Rank =1*/
   U8                            r1TotLenSbDiffCqiCw1;    /*!< Length of SubBand Differential CQI Codeword 1 for Rank = 1*/
   U8                            rg1WideBCqiCw1;    /*!< Length of Wideband CQI Codeword 1 for Rank > 1*/
   U8                            rg1TotLenSbDiffCqiCw1;    /*!< Length of SubBand Differential CQI Codeword 1 for Rank > 1*/
   U8                            r1PmiBitLen;   /*!< Length of PMI Bits for Rank = 1*/
   U8                            rg1PmiBitLen;   /*!< Length of PMI Bits for Rank > 1*/
} RgSchCqiRawPuschMode31;

/** @brief This structure that stores the length of Bits that
*     will be received over PUSCH for Aperiodic Mode 3-0.
*/
typedef struct rgSchCqiRawPuschMode30
{
   U8                     wideBCqiCw;    /*!< Length of Wideband CQI */
   U8                     totLenSbDiffCqi;    /*!< Length of SubBand Differential CQI */
} RgSchCqiRawPuschMode30;

/** @brief This structure that stores the length of Bits that
*     will be received over PUSCH for Aperiodic Mode 2-2.
*/
typedef struct rgSchCqiRawPuschMode22
{
   U8                            wideBCqiCw0;    /*!< Length of Wideband CQI Codeword 0 */
   U8                            sBDiffCqiCw0;    /*!< Length of SubBand Differential CQI Codeword 0 */
   U8                            r1WideBCqiCw1;    /*!< Length of Wideband CQI Codeword 1  for Rank =1 */
   U8                            r1SbDiffCqiCw1;    /*!< Length of SubBand Differential CQI Codeword 1 for Rank =1*/
   U8                            rg1WideBCqiCw1;    /*!< Length of Wideband CQI Codeword 1  for Rank > 1*/
   U8                            rg1SbDiffCqiCw1;    /*!< Length of SubBand Differential CQI Codeword 1 for Rank >1*/
   U8                           posOfM;   /*!< Position of M selected SubBands */
   U8                           r1PmiBitLen;   /*!< Length of PMI Bits for Rank =1*/
   U8                           rg1PmiBitLen;   /*!< Length of PMI Bits for Rank >1*/
} RgSchCqiRawPuschMode22;

/** @brief This structure that stores the length of Bits that
*     will be received over PUSCH for Aperiodic Mode 2-0.
*/
typedef struct rgSchCqiRawPuschMode20
{
   U8                     wideBCqiCw;    /*!< Length of Wideband CQI */
   U8                     subBandDiffCqi;    /*!< Length of SubBand Differential CQI */
   U8                     posOfM;   /*!< Position of M selected SubBands */
} RgSchCqiRawPuschMode20;

/** @brief This structure that stores the length of Bits that
*     will be received over PUSCH for Aperiodic Mode 1-2.
*/
typedef struct rgSchCqiRawPuschMode12
{
   U8                     wideBCqiCw0;    /*!< Length of Wideband CQI Codeword 0 */
   U8                     r1WideBCqiCw1;    /*!< Length of Wideband CQI Codeword 1 for Rank =1*/
   U8                     rg1WideBCqiCw1;    /*!< Length of Wideband CQI Codeword for Rank > 1 */
   U8                     r1TotalPmiBitLen;   /*!< Aggregate length of PMI Bits for Rank =1 */
   U8                     rg1TotalPmiBitLen;  /*!< Aggregate length of PMI Bits for Rank > 1 */
} RgSchCqiRawPuschMode12;


/** @brief This structure that stores the length of Bits that
*     will be received over PUSCH.
*/
typedef struct rgSchDlCqiRawPusch
{
   TfuDlCqiPuschMode      mode;          /*!< PUSCH CQI mode */
   TknU8                  ri;            /*!< Rank Indicator for TM 3,4 */
   union
   {
      RgSchCqiRawPuschMode12   mode12Info;    /*!< Mode 1-2 information */
      RgSchCqiRawPuschMode20   mode20Info;    /*!< Mode 2-0 information */
      RgSchCqiRawPuschMode22   mode22Info;    /*!< Mode 2-2 information */
      RgSchCqiRawPuschMode30   mode30Info;    /*!< Mode 3-0 information */
      RgSchCqiRawPuschMode31   mode31Info;    /*!< Mode 3-1 information */
   }u;
} RgSchDlCqiRawPusch;

typedef struct rgSchPuschRawCqiInfoPerCell
{
   U8                   sCellIdx;        /*!< Serving cell idx of the cell for
                                              this cqi info*/
   RgSchDlCqiRawPusch   puschRawCqiInfo; /*!< Raw CQI Bit Width for  PUSCH */
} RgSchPuschRawCqiInfoPerCell;

typedef struct rgSchPuschRawCqiInfoForSCells 
{
   U8                          numOfCells;   /* Num of cells for which Apcqi is comming*/
   RgSchPuschRawCqiInfoPerCell cqiBitWidth[CM_LTE_MAX_CELLS];
} RgSchPuschRawCqiInfoForSCells;

typedef struct rgSchPucchRawCqiInfoPerCell
{
  U8 sCellIdx;                          /*!< Serving cell idx of the cell for
                                             this cqi info*/
  TfuDlCqiPucch        pucchRawCqiInfo;  /*!< Raw CQI Bit Width for PUCCH */
} RgSchPucchRawCqiInfoPerCell;

typedef struct rgSchUeRawCqiBitWidthInfo
{
  TfuRecpReqType         type;          /*!< Type indicating PUCCH or PUSCH */
  CmLteTimingInfo        recvTime;
   union
   {
      RgSchPucchRawCqiInfoPerCell   pucch;
      RgSchPuschRawCqiInfoForSCells pusch;
   }u;
} RgSchUeRawCqiBitWidthInfo;
#endif


/* CaDev start */
#ifdef LTE_ADV

/**
  * @brief
  * Enum for storing the different states of a Scell
  * RG_SCH_SCELL_INACTIVE : SCell is added but not activate
  * RG_SCH_SCELL_TOBE_ACTIVATED : SCell Activation trigger condition is met
                                   Need to be scheduled.
  * RG_SCH_SCELL_ACTVTN_IN_PROG : Waiting for Harq feedback for the scell activation
  * RG_SCH_SCELL_ACTIVE         : SCell is activated succesfully
  */
typedef enum
{
   RG_SCH_SCELL_INACTIVE = 0,      /*!<SCELL Addition/Reset */
   RG_SCH_SCELL_READY,             /*!<SCELL Ready is reaceived or SCell is deactivated */
   RG_SCH_SCELL_TOBE_ACTIVATED,    /*!<Activation Trigger */
   RG_SCH_SCELL_ACTVTN_IN_PROG,    /*<!Activation CE is sent */
   RG_SCH_SCELL_TOBE_DEACTIVATED,  /*<!Deactivation Trigger */
   RG_SCH_SCELL_DEACTVTN_IN_PROG,  /*<!Deactivation CE is sent */
   RG_SCH_SCELL_ACTIVE             /*<!SCell is activated */
}RgSCellStateEnum;

/**
  * @brief
  * Enum to define the value of A for Format 1B with CS 
  * in FDD Case and for M=1 in TDD Case.
  */
typedef enum 
{
  RG_SCH_A_VAL_2 = 2,
  RG_SCH_A_VAL_3 = 3,
  RG_SCH_A_VAL_4 = 4,
  RG_SCH_A_VAL_INVALID = 5
}RgSchAValue;

#ifdef LTE_TDD
/**
  * @brief
  * Enum to define the value of A for Format 1B with CS 
  * in FDD Case and for M=1 in TDD Case.
  */
typedef enum 
{
  RG_SCH_M_VAL_1 = 1,
  RG_SCH_M_VAL_2 = 2,
  RG_SCH_M_VAL_3 = 3,
  RG_SCH_M_VAL_4 = 4,
  RG_SCH_M_VAL_INVALID = 5
}RgSchMValue;
#endif/*LTE_TDD*/
#endif/*LTE_ADV*/

/**
  * @brief
  * Downlink UE specific SCell information.
  */
struct rgSchUeCellInfo
{
   CmHashListEnt        ueLstEnt;  /*!< Hash List entity for UE List */
   RgSchUeCb            *ue;        /*!< Pointer to UECB */
#ifdef LTE_ADV
   U8                    sCellIdx;   /*!< Serving Cell Index  */
   U16                   sCellId;    /*!< Secondary Cell Id */
   RgSCellStateEnum      sCellState; /* !< For holding the current state of the sec cell */
   CmLList               sCellLnk;   /*!< Node for adding this UE in secondary cell */
#endif
   RgSchDlHqEnt          *hqEnt;        /*!< Downlink HARQ information for the UE */
   RgSchDlRbAlloc        dlAllocCb;     /*!< RB Allocation Info for Ded Trans/Retrans */
   RgSchCellCb           *cell;    /* !< Reference to sec Cell Cb */
   Void                  *sch;         /*!< UE Specific Scheduler information */
   RgrUeTxModeCfg       txMode;       /*!< UE transmission mode in Secondary */
#ifdef LTE_TDD
   RgSchTddANInfo       *anInfo;       /*!< ACK/NACK related Information */
   U8                   nextFreeANIdx; /*!< Next Available ANInfo Index */

#endif
   U8                    reqForCqi;     /*!< set to True if Aperiodic CQI from UE is required */
#ifdef TFU_UPGRADE
   RgSchUeACqiCb         acqiCb;        /* ! < Aperiodic CQI Report Control Block*/
   RgSchUePCqiCb         cqiCb;      /*!< Periodic CQI PMI RI Control Block */
   TknU8      pA;                     /* PA value configured by RRM
                                         ref: RRC 36.331, 6.3.2, PDSCH-Config*/
   /* RgSchUeRawCqiBitWidthInfo  rawCqiBitW[MAX_CQI_RI_RPT_BUFF]; */
#endif
   U8                   cqiRiWritIdx; /*!< write index to be used whenever CQI/RI reception
                             request is being filled*/
   U8                   cqiRiReadIdx; /*!< Read index to be used whenevr CQI/RI indication 
                             is recieved from PHY*/
   CmTimer              deactTmr;        /*!< SCell deactivation timer */
   CmTimer              actDelayTmr;        /*!< SCell  timer */

};



/* CaDev end */



/**
  * @brief
  * Downlink control block for UE specific information.
  */
typedef struct rgSchUeDlCb
{
   RgSchUeDlTaCb taCb;         /*!< Downlink timing adjustment information for the
UE */
   RgSchDlLcCb   *lcCb[RGSCH_MAX_LC_PER_UE];/*!< Dedicated Downlink logical channels in
UE */
   RgrUeDlCqiCfg  ueDlCqiCfg;    /*!< UE DL CQI config */
   U8             dlInactvMask;  /*!< Bitmask indicating if UE is inactive for DL scheduling */
   RgSchCqiReqField reqForCqi;   /*!< set to True if Aperiodic CQI from UE is required */
   Bool             pCqiPrsnt;     /*!< set to True if periodic CQI from UE is expected */
   Bool             acqiResGrntd;  /*!< Aperiodic CQI resources granted in RB Estimation */
   CmLList        dlInactvLnk;   /*!< Link to the inactive UE List for DL */
#ifdef LTE_TDD
   RgrTddAckNackMode ackNackMode;   /*!< ACK/NACK Bundling/ ACK/NACK Multiplexing */
#endif
   /* Added support for SPS*/
#ifdef LTEMAC_SPS
   RgrUeSpsDlCfg  dlSpsCfg;         /*!< DL SPS configuration information */
   Bool           isSpsHqPInUse;    /*!< Indicates if SPS HARQ procs are in use
                                         or are to be used for SPS
                                         transmissions */
   RgSchPdcch      spsOccPdcch;      /*!< PDCCH information for SPS ocassion */
#endif
   /* CA dev Start */
   U8 numHqDlSfInfo;
#ifdef LTE_TDD
   RgSchDlHqInfo *dlSfHqInfo;
#else
#ifdef RG_5GTF 
   /* 5GTF TODO : Hard setting number of CCs to 3 */
   RgSchDlHqInfo dlSfHqInfo[MAX_5GTF_CELL][RGSCH_NUM_DL_slotS];
#else
   RgSchDlHqInfo dlSfHqInfo[RGSCH_NUM_DL_slotS];
#endif
#endif  
   /* Moved from rgSchCmnDlUe to Here, as this shouldn't be present per cell*/
   U32            ambrCfgd; /*!< UE's configured AMBR scaled to Refresh Cycle */ 
   /* CA dev End */
   /* Added to restrict max TB Bits in case of more than one CCs for a UE */
   U32            aggTbBits;        /*!< Aggregated Transport Block Bits this UE can receive per TTI*/
   CmLteTimingInfo lstSchTime;      /*!< Last Time UE got Scheduled */

} RgSchUeDlCb;



#ifdef LTEMAC_HDFDD

/******************************************************************************
 *                         Start of Data declarations                         *
 ******************************************************************************/
/**
  @brief Half Duplex subframtion information stored per ue. */

typedef struct rgSchUeHdFddSfInfo
{
  U16             sfn;                   /*!< Store the sfn for updated state
                                              Default Value= 0xffff
                                          */
  U8              subFrmDir;
                                         /*!< 0x00= DFLT
                                              0x01= DL DATA +(OPT:CNTRL INFO)
                                              0x02= DL CNTRL
                                              0x04= UL DATA+(OPT: CTNRL INFO)
                                         */

}RgSchUeHdFddSfInfo;

/**
  @brief Half Duplex control related information per ue. */

typedef struct rgSchUeHdFddCb
{
   RgSchUeHdFddSfInfo    subfrm[RG_SCH_HDFDD_NUMSFINFO];

}RgSchUeHdFddCb;
#endif /* LTEMAC_HDFDD */


/** @brief This structure is part of rgSchUeCb structure and stored DRX related
 * information for a UE.
 */
typedef struct rgSchDrxUeCb
{
#ifdef LTEMAC_R9
   TknS32 cqiMask;  /*!<If pres, only value supported is SETUP i.e 0 */
#endif
   Bool     srRcvd;   /*!< TRUE if SR is received from the UE. */
   Bool     raRcvd;   /*!< TRUE if Random access request is received using a
                        dedicated preamble for this UE. */
   U16      onDurTmrLen;   /*!< Length of onDuration Timer [1 - 200]. */
   U16      drxStartOffset;   /*!< Value of the DRX Starting Offset [0 - 2559]. */
   S16      onDurExpDistance;   /*!< Keeps track of actual distance */

   U16       drxRetransTmrLen; /*!< Length of DRX Retransmission timer [1 - 33].*/

   U16      inactvtyTmrLen;   /*!< Length of drx-Inactivity Timer [1 - 2560]. */
   S16      drxInactDistance;   /*!< Keeps track of actual distance */

   Bool     isLongCycle;
   U16      longDrxCycle;   /*!< Value of the DRX long cycle [10 - 2560]. */

   Bool     isShortCycleCfgd;   /*!< TRUE if short cycle is enabled. */
   U8       shortCycleTmrLen;   /*!< Value of DRX short cycle Timer [1-16]. */
   U16      shortDrxCycle;   /*!< Value of the DRX short cycle [2 - 640]. */
   S16      drxShortCycleDistance;   /*!< Keeps track of actual distance */


   CmLList  onDurationEnt;   /*!< Entry into the OnDuration List. */
   CmLList  onDurationExpEnt;   /*!< Entry into the onDuration Expiry List. */
   CmLList  inActvTmrEnt;   /*!< Entry into the inactivity Timer list.  */
   CmLList  shortCycleEnt;   /*!< Entry into HARQ Retransmission list. */

   S16      distance;   /*!< Keeps track of actual distance */

/* The following elements track current indices into the drxQ present at the
 * cell level. These indicies help in fast deletion in case of UE Delete,
 * otherwise it might have required a linear search. */
   U16      onDurIndx;   /*!< The current index for onDuration Queue. */
   U16      onDurExpIndx;   /*!< The current index for onDuration Queue. */
   U16      drxInactvIndx;   /*!< The current index for drx-InactityTmr Queue. */
   U16      shortCycleIndx;   /*!< The current index for Short Cycle Queue. */

   U8       shortCycleTmr;   /*!< Counter to keep track of Short DRX Cycle. */
   U32      drxDlInactvMask;  /*!< Downlink Mask to track InActivity */
   U32      drxUlInactvMask;  /*!< Uplink Mask to track InActivity */
   U32      drxDlInactvMaskPerCell[CM_LTE_MAX_CELLS]; /*!< Downlink Mask to track InActivity per cell */
   U32      drxUlInactvMaskPerCell[CM_LTE_MAX_CELLS]; /*!< Uplink Mask to track InActivity per cell */
} RgSchDrxUeCb;



/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
typedef struct RgSchCqiInfo
{
   U8        cqiCount;                  /* To keep track of CQI reports
                                           received so far */
   RgrUeCqiRept cqiRept[RGR_CQIRPTS_MAXN]; /* Array to maintain CQI reports */
}RgSchCqiInfo;


#endif

/* LTE_ADV_FLAG_REMOVED_START */

/* @brief Control block for LTE Advance Feature for UE */
typedef struct rgSchLteAdvUeCb
{
   RgrLteAdvancedUeConfig   rgrLteAdvUeCfg;   /*< RGR Configuration of LTE Adv */
   Bool                     isCCUePHigh;   /* CC user gets high power after RNTP info */
} RgSchLteAdvFeatureUeCb;
/* LTE_ADV_FLAG_REMOVED_END */

/*<! Sizes of DCI 0/1/1A/2/2A */
typedef struct rgSchUeDciSize {
#ifdef EMTC_ENABLE   
   U8 cmnSize[TFU_DCI_FORMAT_6_2+1]; /*!< DCI 0/1A/6-0A/6-1A final size in common Search Space and scrambled by C-RNTI */
   U8 dedSize[TFU_DCI_FORMAT_6_2+1]; /*!< DCI 0/1/1A/2/2A/6-0A/6-1A final size in UE Search Space and scrambled by C-RNTI */
#else   
   U8 cmnSize[TFU_DCI_FORMAT_1A+1]; /*!< DCI 0/1A final size in common Search Space and scrambled by C-RNTI */
   U8 dedSize[TFU_DCI_FORMAT_2A+1]; /*!< DCI 0/1/1A/2/2A final size in UE Search Space and scrambled by C-RNTI */
#endif   
   U8 noUlCcSize[TFU_DCI_FORMAT_2A+1]; /*!< DCI 1/1A final size in UE/Common Search Space when the cell 
                                        is SCell and no corresponding UL CC configured */
} RgSchUeDciSize;

#ifdef RG_PFS_STATS
typedef struct rgSchPerRefreshStats {
   U32  remGbr;
   U32  remDeltaMbr;
   U32  totByteSchd;
   U32  lcSchdOcc;
}RgSchPerRefreshStats;

typedef struct rgSchLcStats
{
   //Bool           isLcCntSet;
   //U32            schdOccCnt; 
   U32            ueSchdOcc[CM_LTE_MAX_CELLS];
   U32            gbrSatisfiedCnt;
   Bool           ignoredFirstRefresh;
   U32            gbrRefreshCycleCnt;
   U32            totGbrBytesSchd;
   U32            totMbrBytesSchd;
   U32            achvdFracDataRate;
   Bool           isRecvdBo;
#define RGSCH_NUM_STATS_PER_REFRESH 50
   U32            startIdx;
   U32            lastIdx;
   U32            favCellCnt[CM_LTE_MAX_CELLS];
   RgSchPerRefreshStats perRefresh[RGSCH_NUM_STATS_PER_REFRESH];
}RgSchLcStats;

typedef struct rgSchCqiStats
{
   U32            totalCqiOcc;
   U32            avgCqi;
}RgSchCqiStats;

typedef struct rgSchPfsStats
{
   RgSchLcStats   lcStats[RGSCH_MAX_LC_PER_UE];
   //U32            ueSchdOcc[CM_LTE_MAX_CELLS];
   U32            refreshCycleCnt;
   RgSchCqiStats  cqiStats[CM_LTE_MAX_CELLS];
   Bool           isCaUe;
}RgSchPfsStats;
#endif

#ifdef RG_5GTF
/**
  * @brief
  * UE control block for UE specific information for 5gtf.
  */
typedef struct rgSch5gtfUeCb
{
   U8              grpId;      // Group Id 
   U8              BeamId;     // Beam Id of UE
   U8              numCC;      // num Carrier configured for UE
   U8              mcs;        // MCS configured
   U8              maxPrb;     // Max Prb configured for UE 
   CmLteTimingInfo nxtCqiRiOccn;   /*!< Next CQI RI Occn */
   U16             cqiRiPer;   /*!< CQI RI periodicity in SFs */
   U8              rank;       /*!< Latest Rank Report from UE */
}RgSch5gtfUeCb;
#endif

/**
  * @brief
  * UE control block for UE specific information.
  */
struct rgSchUeCb
{
   U32                  totalBo; /*!<Sum of DL BO across all logical channels*/
   U32                  totalPrbRequired;/*!<Num of PRB reqd to satisfy DL totlBo*/
   /* Added support for SPS*/
#ifdef LTEMAC_SPS
   CmHashListEnt        spsUeLstEnt;  /*!< Hash List entity for UE List */
   U32                  spsOccasionCnt; /*!< Total number of SPS occasion cnt*/
#endif
#ifdef CQI_CONFBITMASK_DROP
   U8                   cqiConfBitMask;
   U8                   prevCqi; 
#endif
   RgSchRntiLnk         *rntiLnk;  /*!< Link to RNTI for the UE */
   CmLteRnti            ueId;      /*!< UE identifier */
   RgSchUeMimoInfo      mimoInfo;  /*!< MIMO related information for a UE */
   /* Added support for SPS*/
#ifdef LTEMAC_SPS
   CmLteRnti            spsRnti;   /*!<  Configured value for spsRnti if SPg
                                         is enabled for the UE */
   /* TODO antz - put all relPdcch related info in a struct */
   CmLteTimingInfo      relPdcchTxTime; /*!< Time at which release pdcch is
                                             transmitted */
#ifdef LTE_TDD
   U8                   relPdcchValm;   /*!< 'm' for relPdcch */
#endif
   CmLteTimingInfo relPdcchFbkTiming;/*!< Release PDCCH feedback timing for
                                          the UE: value used by DHM */
#endif
   RgSchCellCb          *cell;     /*!< Cell to which this UE belongs */
#ifdef XEON_SPECIFIC_CHANGES
   Void                 *sch;      /*!< UE Specific Scheduler information */ 
#endif
   RgSchUeUlCb          ul;        /*!< UE Specific Uplink information */
   RgSchUeDlCb          dl;        /*!< UE Specific Downlink information */
   CmLteTimingInfo      datIndTime;/*!< Timing info of the last received Dat
                                                              indication */
   CmLteTimingInfo      macCeRptTime;/*!< Timing info of the last received
                                       *  MAC CE (BSR/PHR) */
   U32                  y[RGSCH_NUM_SUB_FRAMES]; /*!< 'Y' values calculated 
                                                     using C-RNTI and subframe
                                                     no based on formula
                                                     present in sec 9.1.1 of 
                                                     3GPP 36.313*/

   CmLList              taLnk;     /*!< To link to the taUeLst list */
   CmLList              dlTaLnk;   /*!< To link to the Dl SCHED's taLst */
   CmTimer              taTmr;       /*!< Timer for Timing Alignment */
   RgSchUeMeasGapCb     measGapCb;     /*!< Measurement gap control block */
   RgSchUeAckNakRepCb   ackNakRepCb;   /*!< Ack-Nack Repetition control block */
#ifdef LTE_ADV
   CmLList              sCellActLnk;   /*!< To link to the Dl SCHED's
                                            ScellActivation List */
#endif

#ifdef RGR_V1
   /* Added periodic BSR timer */
   CmTimer              bsrTmr;        /*!< BSR timer expiry handling case */
   /* CR timer implementation changes*/
   CmLList         ccchSduLnk;       /*!< To link raCb to the "to be
                                       scheduled"
                                       list
                                      */
   struct
   {
      U32          bo;            /*!< Buffer occupancy for
                                    CCCH */
   } dlCcchInfo;                  /*!< Params for DL
                                    CCCH */
#else

   CmTimer              bsrTmr;        /*!< BSR timer expiry handling case */
#endif
/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
   CmLteUeCategory     ueCatEnum;
   /* Moved to SCellInfo*/
//   RgSchUeACqiCb     acqiCb;        /* ! < Aperiodic CQI Report Control Block*/
   /* Periodic CQI/SRS/SR Report Feature */
   RgSchUeSrsCb      srsCb;        /*!< SRS  Control Block*/
   RgSchUeSrCb       srCb;         /*!< SR Control Block */
#endif

#ifdef LTEMAC_HDFDD
   /* Half Duplex Feature */
   Bool              hdFddEnbld;        /*!< Half Duplex FDD is configured TRUE=1/FALSE=0 */
   RgSchUeHdFddCb    *hdFddCb;     /*!< Half Duplex Control Block */
#endif  /* LTEMAC_HDFDD */
/* Added changes of TFU_UPGRADE */
   Bool               isDrxEnabled; /*!< isDrx Enabled for this UE?*/
   RgSchDrxUeCb       *drxCb;       /*!< DRX control block. Allocated at time of
                                        UE configuration. */
   CmLList            ulDrxInactvLnk; /*!<List of UE's which become inactive for UL */
   CmLList            dlDrxInactvLnk; /*!<List of UE's which become inactive for DL */

   CmLList            ulDrxInactvTmrLnk; /*!<List of UEs on which inactivity
                                             tmr has to be started for UL */
   CmLList            dlDrxInactvTmrLnk; /*!<List of UEs on which inactivity
                                             tmr has to be started for DL */
#ifdef TFU_UPGRADE
  
   U8     validTxAnt;  /*! < Tx Antenna selected after computing the CQI among two Antennas*/

   U8     cqiRiWritIdx; /*!< write index to be used whenever CQI/RI reception
                             request is being filled*/
   U8     cqiRiReadIdx; /*!< Read index to be used whenevr CQI/RI indication 
                             is recieved from PHY*/
   RgSchUeRawCqiBitWidthInfo  rawCqiBitW[MAX_CQI_RI_RPT_BUFF];
   U8     initNumRbs; /* No. of RBs allocated for UL Data New Transmission */
#endif

/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   RgrUeCqiReptCfg cqiReptCfgInfo; /* Holds static information such
                                                 as configured values for DL
                                                 Power Control*/
#endif
#ifdef TFU_UPGRADE
#endif
/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   RgSchCqiInfo schCqiInfo; /* This structure is place holder for
                               keeping all CQI reporting related information */
#endif
/* LTE_ADV_FLAG_REMOVED_START */
   RgSchLteAdvFeatureUeCb   lteAdvUeCb; /* LTE-Advanced control block for UE */                                    
/* LTE_ADV_FLAG_REMOVED_END */


#ifdef LTE_L2_MEAS
   U8 qciActiveLCs[LRG_MAX_QCI]; /* This structure has number of active LCs per 
                                    Qci for this UE */ /* L2_COUNTERS */
   U16 ulActiveLCs;     /* This is a bitmask - Each bit representing a QCI
                           LSB - QCI 1 ... When bit is set, UE has an active
                           LC for that QCI */
   U16 lastDatIndLCs;     /* This is a bitmask - Each bit representing a QCI
                            for which Data for received in UL 
                           LSB - QCI 1 ... When bit is set, UE has an active
                           LC for that QCI */
#endif
   Bool isMsg4PdcchWithCrnti; /* This is to aid rgNumMsg4PdcchWithCrnti counter which counts
                                          num of PDCCH scrambled with CRNTI for CRNTI Based contention resolution */
   Bool         isSrGrant;    /*!< This is to aid Counter to check number of successful SR Grants  SR_RACH_STATS*/
   Bool           txModeTransCmplt; /*!< Flag to confirm TM Mode 
                                         transition completion*/
   CmTimer        txModeTransTmr;  /*!< Timer for TxMode transition Completion */
#ifdef RGSCH_SPS_STATS
   CmLteTimingInfo lastSpsLcBoRptTime; 
   CmLteTimingInfo lastSpsLcSchedTime;
   U64             absTimeBo;
   U64             absTimeSched;
#endif
   U8           refreshOffset; /*!< UE referesh offset */
   U8              csgMmbrSta;        /*!< CSG Membership status configured */
#ifdef TENB_STATS
   TSL2UeStatsCb   *tenbStats; /*!< UE Stats Holder */
#endif
   CmLListCp       flowCntrlLcLst;/*!< Dedicated Downlink logical channels in UE for Flow Control*/
   CmLList         ueFlowCntrlEnt;/*!<UE lnk to the flow control Lst*/ 
   
   RgSchLcgCb      *repLcg;/*!<representative LCG of the UE in  UL */
#ifdef XEON_SPECIFIC_CHANGES
   CmLteTimingInfo riRecpTime; /*! To idnentify Ri wrap arround during PUSCH and
                                 PUCCH reception filling      */
#endif
#ifdef LTE_ADV
   TknU32          sCellDeactTmrVal; /* !< SCell Deactivation Timer Value */
   U8              f1bCsAVal; /* !< A value mentioned in table 10.12.2.1-1 of 36.213. This will tell
                            the max number of tbs possible based on TM mode of each configured 
                            scell. It is used only for F1B with channel selection*/
#endif
/*f1b_Sprint3*/
   RgSchSCellN1ResCb     n1PucchF1bResCb; /*!< N1 resouurce Cb for F1b Channel selection */
   RgSchSCellN3ResCb     n3PucchResCb;
   RgrSchFrmt1b3TypEnum  uciFrmtTyp; /*!< Store value of format1bcs or format3.It
                                       is updated from upper layer*/
/*f1b_Sprint3*/
   U8             numSCells; /* !< number of configured SCells */
   U8             cellIdToCellIdxMap[CM_LTE_MAX_CELLS]; /*!< Mapping info of Cell Id to Cell Idx */

   RgSchUeCellInfo *cellInfo[CM_LTE_MAX_CELLS]; /*!< DL Sec Cell Information */
#ifdef TFU_UPGRADE
   RgSchUePCqiCb   *nPCqiCb;  /*!< Pointer to Periodic Cqi CB for which next CQI is expected*/
   RgSchUePCqiCb   *nPRiCb;   /*!< Pointer to Periodic Cqi CB for which next RI is expected*/
#endif
   U8              remBoCnt;     /*!< count of occurence when BO is not fullfilled
                                in a TTI */
   U8              *zeroBoCnt;   /*!< pointer of count of occurence when BO is
                                   Zero */
#ifdef LTE_ADV
   Bool            isScellExplicitDeAct; /*!< TRUE when SCELL deactivation timer is Infinity/Not configured */
   Bool            allocCmnUlPdcch;  /*!< If this flag is TRUE, allocate PDCCH from Common
                                        search space */
   U8              simulAckNackCQIFormat3; /* !< Flag for simultanious A/N and CQI on PUCCH Format 3 */
#endif
   RgSchUeDciSize  dciSize;          /*!< DCI Sizes */
   RgrAccessStratumRls accessStratumRls; /*!< UE Release */
#ifdef RG_PFS_STATS
   RgSchPfsStats   pfsStats;
#endif
#ifdef EMTC_ENABLE
	Bool            isEmtcUe;            /*!< flag to check EMTC UE */
   Void            *emtcUeInfo;        /*!< EMTC UE specific information */
#endif
#ifdef RG_5GTF
   RgSch5gtfUeCb   ue5gtfCb;           /*!< UECb of 5gtf */
#endif
};


/**
  * @brief
  * Configuration Information for Upper SAPs at RGU, CRG and RGR interfaces.
  */
typedef struct rgSchUpSapCfgInfo
{
   Pst  sapPst;              /*!< Post info associated with SAP */
   SpId spId;                /*!< SpId associated with SAP */
   SuId suId;                /*!< SuId associated with SAP */
}RgSchUpSapCfgInfo;

/**
  * @brief
  * Configuration Information for Lower SAP at TFU interface.
  */
typedef struct rgSchLowSapCfgInfo
{
   Pst    sapPst;            /*!< Post info associated with SAP */
   SpId   spId;              /*!< SpId associated with SAP */
   SuId   suId;              /*!< SuId associated with SAP */
   TmrCfg bndTmr;            /*!< Bind Timer Value */
}RgSchLowSapCfgInfo;

/**
  * @brief
  * Control Block structure for Upper SAPs at RGU, CRG and RGR interfaces.
  */
typedef struct rgSchUpSapCb
{
   RgSchUpSapCfgInfo sapCfg;    /*!< Configuration info */
   RgSapSta     sapSta;      /*!< SAP Status */
   RgSchCellCb  *cell;       /*!< Cell associated with this sap */
}RgSchUpSapCb;

/**
  * @brief
  * Control Block structure for Lower SAP at TFU interface.
  */
typedef struct rgSchLowSapCb
{
   RgSchLowSapCfgInfo sapCfg;          /*!< SAP configuration info */
   RgSapSta           sapSta;          /*!< SAP Status */
   U8                 numBndRetries;   /*!< Number of Bind Retries */
   RgSchCellCb        *cell;           /*!< Cell associated with this SAP */
   CmTimer            tmrBlk;       /*!< Timer Block associated with this SAP */
}RgSchLowSapCb;

/**
  * @brief
  * structure holding Layer Manager Response information cached from control
  * request.
  */
typedef struct rgSchLmResponse
{
   TranId transId;           /*!< Transaction ID */
   Resp   response;          /*!< Response */
}RgSchLmResponse;

/* XXX: Below structures added for PDCCH Order req for RACH Module */
typedef enum sfnEnum
{
   RG_SCH_SFN_ANY,
   RG_SCH_SFN_ODD,
   RG_SCH_SFN_EVEN
} SfnEnum;

typedef struct rgSchConfigIdx
{
   U8         configurationIndex; /* New RGR Cell cfg */
   SfnEnum    sfn;
   U8         subframes[10];
} RgSchConfigIdx;

typedef struct rgSchRapId
{
   U8                rapId;
   CmLteTimingInfo   lastAllocPRACHMaskIdx;
} RgSchRapId;

typedef struct pdcchOrderCfg
{
   RgSchRapId       rapIds[RGSCH_MAX_RAPID];
   RgSchConfigIdx   configIdx;
} PdcchOrderCfg;
/* XXX: End */

/**
  @brief Measurement Gap related information per cell. */
typedef struct rgSchMeasGapCb
{
   CmLListCp  gapPrd40Q[RG_SCH_CMN_MEAS_GAPPRD40]; /*!< Measurement Gap queue
                                               for UEs with 40 ms gap period */
   CmLListCp  gapPrd80Q[RG_SCH_CMN_MEAS_GAPPRD80]; /*!< Measurement Gap queue
                                               for UEs with 80 ms gap period */
} RgSchMeasGapCb;

/**
  @brief ACK-NACK repetition related information per cell. */
typedef struct rgSchAckNakRepCb
{
#ifdef LTE_TDD
   CmLListCp  ackNakRepQ[2*RGSCH_NUM_SUB_FRAMES];  /*!< ACK NACK repetition queue */
#else
   CmLListCp  ackNakRepQ[RGSCH_NUM_SUB_FRAMES];  /*!< ACK NACK repetition queue */
#endif
} RgSchAckNakRepCb;
/**
  * @brief
  * Structure holding RBG information for the BW
  */
typedef struct rgSchBwRbgInfo
{
   U8     numRbs;        /*!< Total number of RBs for which information is
                              stored */
   U8     numRbgs;       /*!< Number of RBGs for the BW (rounded off to the
                              closest RBG number */
   U8     rbgSize;        /*!< RBG size */
   U8     lastRbgSize;    /*!< Last RBG size : in number of RBs */
   U8     rbgSubsetSize[RG_SCH_NUM_RATYPE1_SUBSETS]; /*!< RBG Subset 0,1,2,3
                                                          sizes: number of RBs
                                                      */
}RgSchBwRbgInfo;
/** @brief This structure is one element of the DRX Queue mainted per cell.
 * @details It is composed of a list of linked lists, each tracking a specific
 * timer as defined in the DRX functionality.
 */
typedef struct rgSchDrxQ
{
   CmLListCp   onDurationQ;   /*!< Tracks the start of onDuration Timer. */
   CmLListCp   onDurationExpQ;   /*!< Tracks the Expiry of onDuration Timer. */
   CmLListCp   inActvTmrQ;   /*!< Tracks the Expiry of drx-InactivityTimer. */
   CmLListCp   harqRTTQ;   /*!< Tracks the Expiry of HARQ RTT timer. */
   CmLListCp   harqRetxQ;   /*!< Tracks the Expiry of Re-Transmission timer. */
   CmLListCp   shortCycleQ;   /*!< Tracks the Expiry of DRX Short Cycle. */
#ifdef EMTC_ENABLE
   CmLListCp   ulHarqRTTQ;   /*!< Tracks the Expiry of HARQ RTT timer for Uplink transmission */
   CmLListCp   ulHarqRetxQ;   /*!< Tracks the Expiry of Re-Transmission timer for UPLINK. */
#endif
} RgSchDrxQ;

/** @brief This structure is part of the RgSchCellCb structure and stores the
 * DRX related information for a cell.dfort
 * The cell contains a single Array of queues, wherein UEs shall be enqueued
 * based on configured offset and periodicity and timer values. Please note the
 * same queue is utilized for Long and Short DRX cycle.
 */
typedef struct rgSchDRXCellCb
{
   RgSchDrxQ   drxQ[RG_SCH_MAX_DRXQ_SIZE ]; /*!< Maintains all
                                              the timers for DRX. */
   Bool        delInUlScan;                 /*!< Maintains whether elements
                                                 from timer queue should be
                                                 removed while scanning in
                                                 UL or DL.*/
} RgSchDRXCellCb;


/* Added for SI Enhancement*/
#ifdef RGR_SI_SCH

/* Sib1 info */
typedef struct sib1Info
{
   Buffer *sib1;
   U8      mcs;
   U8      nPrb; 
   MsgLen  msgLen;
}RgSchSib1Info;

/* SI info */
typedef struct siInfo
{
   Buffer *si;
   U8      mcs;
   U8      nPrb; 
   MsgLen  msgLen;
}RgSchSiInfo;

/**
  @brief SI Configuration information per cell. */
typedef struct rgSchSiCfgInfo
{
   Buffer   *mib;   /*!< MIB PDU */
   RgSchSib1Info sib1Info;
   RgSchSiInfo   siInfo[RGR_MAX_NUM_SI];
}RgSchSiCfgInfo;

/**
  @brief SI Context information per SI. */
typedef struct rgSchSiCtx
{
   U8   siId;             /*!< SI Id */
   CmLteTimingInfo timeToTx;   /*!< Time at which the SI for this SI
                                    context need to be scheduled.*/
   CmLteTimingInfo maxTimeToTx;/*!< Max Time to TX for this SI */
   U8              retxCntRem; /*!< SI retransmit count remaining */
   U16             i;          /*!< Value used to calculate the Riv of SI */
   Bool           warningSiFlag; /*!< Flag for Warning SI */
} RgSchSiCtx;


/** 
 @brief Contains each PDU of Warning SI */
typedef struct warningSiPdu
{
   CmLList lnk;
   Buffer *pdu;
   U8      mcs;
   U8      nPrb; 
   MsgLen  msgLen;
}RgSchWarningSiPdu;


/**
  @brief warningSi node having multiple segments */
typedef struct warningSiSeg
{
   /* ccpu00136659: CMAS ETWS design changes */
   CmLListCp     segLstCp; /*!< LList of Warning SI Segments */
   RgSchWarningSiPdu pduNode[RGR_MAX_WARNING_SI_SEG]; 
   RgrCfgTransId transId;   /*!< Transaction Id */
}RgSchWarningSiSeg;

/**
  @brief Warning SI structure per Cell. */
typedef struct warningSi
{
   /* ccpu00136659: CMAS ETWS design changes */
   RgSchWarningSiSeg      warningSiMsg;
                          /*!< Each node contains LList of si segments. */
   U8                     siId; /*!< Warning SI ID */
   U8                     idx; /*!< Warning SI Idx in RgSchWarningSiInfo */
}RgSchWarningSiInfo;

/**
  @brief SI Array Structure for each SI */
typedef struct siArray
{
   Bool isWarningSi; /*!< Flag for Warning SI */
   void *si;         /*!< Pointer for SI */
}
RgSchSiArray;
/**
  @brief SI Control BLock per Cell. */
typedef struct rgSchSiCb
{
   Bool        siBitMask;  /*!< Bitmask to indicate which of the SI
                                    components have been updated */
   RgSchSiCtx  siCtx;      /*!< SI  Context */
   RgrSiCfg    newSiCfg;   /*!< New SI Configuration, valid if the
                                    respective bit is set in bit mask
                                    siBitMask  */
   S8          inWindow;  /*!< counter to indicate the start of a new 
                                si window. Reset to siWinLen at the start
                                of window. */
   RgSchSiCfgInfo   crntSiInfo;/*!< PDUs for current modification period */
   RgSchSiCfgInfo   newSiInfo; /*!< PDUs for next modification period */
   RgSchWarningSiInfo   warningSi[RGR_MAX_NUM_WARNING_SI]; 
                              /*!< PDUs for warning SI */
   RgSchSiArray        siArray[RGR_MAX_NUM_SI]; /*!< Pointers for SIs */
} RgSchSiCb;
#endif /*RGR_SI_SCH */
/* R8 Upgrade */
typedef struct rgSchBiInfo
{
   U16             prevBiTime;   /*!< Previous BI Value in ms Calculated and
                                   Sent in Previous Response */
   CmLteTimingInfo biTime;       /*!< Time at which previous BI sent */
} RgSchBiInfo;

/* RRM_SP1_START */
typedef struct rgSchQciPrbUsage
{
   U8  qci;          /*!< QCI of the Logical Channel */
   U32 dlTotPrbUsed; /*!<  total PRB used for DL within one interval*/
   U32 ulTotPrbUsed; /*!< total PRB used for UL within one interval*/
}RgSchQciPrbUsage;

/* RRM_SP1_END */

typedef struct rgSchPrbUsage
{
   Bool prbRprtEnabld; /*!< reporting is enabled or not*/
   U16 rprtPeriod; /*!< reporting interval to send PRB usage to the 
                        RRM (in subframes)*/
   CmLteTimingInfo startTime; /*!< timing information when the summation is 
                                   started in terms of sfn and subframe*/
/* RRM_SP1_START */
   RgSchQciPrbUsage qciPrbRpts[RGINF_MAX_GBR_QCI_REPORTS]; /*!< toal 
                                                             PRB usgae for GBR
                                                             QCIs */
/* RRM_SP1_END */
}RgSchPrbUsage;

/* LTE_ADV_FLAG_REMOVED_START */
/* @brief  Enum to differntiate the subframe */
/* Donot change the value of enum member - code dependency */
typedef enum rgSchAbsSfEnum
{
   RG_SCH_ABS_ENABLED_NONABS_SF = 0,  /* ABS is enabled and ABS SF */
   RG_SCH_ABS_ENABLED_ABS_SF    = 1,  /* ABS is enabled and non ABS SF */
   RG_SCH_ABS_DISABLED          = 2   /* ABS is disabled */
} RgSchAbsSfEnum;

/* @brief Control block for LTE Advance Feature */
typedef struct rgSchLteAdvFeatureCb
{
   RgrSfrConfig               sfrCfg; /*!< Configuration of SFR feature */
   RgrDsfrConfig              dsfrCfg;  /*!< Configuration of DSFR feature */
   RgrAbsConfig               absCfg; /*!< Configuration of ABS feature */
   RgSchAbsSfEnum             absDlSfInfo; /*< Flag to indicate current scheduling 
                                             DL subframe is ABS subframe or not */
   U8                         absPatternDlIdx;
   U32                        absLoadTtiCnt;                   
   U32                        absLoadInfo[RGR_ABS_PATTERN_LEN];
} RgSchLteAdvFeatureCb;
/* LTE_ADV_FLAG_REMOVED_END */

/*<! baseSize is the fixed sizes of the respective DCIs. If no other size
 * mentioned, baseSize is the final size of the DCI */
typedef struct rgSchCellDciSize {
#ifdef EMTC_ENABLE   
   U8 baseSize[TFU_DCI_FORMAT_6_2+1]; /*!< Fixed Base Size of DCI 0/1/1A/6-0A/6-1A/6-0B/6-1B/6-2 without any adjustment */
   U8 size[TFU_DCI_FORMAT_6_2+1]; /*!< DCI 0/1A final size in common Search Space and not scrambled by C-RNTI
                                      DCi 3/3A final sizes
                                      DCI 2/2A final sizes 
                                      DCI 6-0A/6-0B final sizes 
                                      DCI 6-1A/6-1B final sizes 
                                      DCI 6-2 final sizes */
#else                                      
   U8 baseSize[TFU_DCI_FORMAT_1A+1]; /*!< Fixed Base Size of DCI 0/1/1A without any adjustment */
   U8 size[TFU_DCI_FORMAT_3A+1]; /*!< DCI 0/1A final size in common Search Space and not scrambled by C-RNTI
                                      DCi 3/3A final sizes
                                      DCI 2/2A final sizes */
#endif                                      
   U8 dci0HopSize;               /*!< DCI 0 Hop Size */
} RgSchCellDciSize;

/**
  * @brief
  * Cell level measurements
  */
typedef struct rgSchMeasCb
{
   U32 dlTpt; /*!< DL Bytes served in a fixed time PERIOD */
   U32 dlBytesCnt; /*!< DL Bytes served from start of this time period */
   U32 ulTpt; /*!< DL Bytes served in a fixed time PERIOD */
   U32 ulBytesCnt; /*!< UL Bytes served from start of this time period */
}RgSchMeasCb;

/**
  * @brief
  * Cell level thresholds 
  */
typedef struct rgSchThrsldsCb
{
   U8 maxDlItbs;
   U8 maxUlItbs;
}RgSchThrsldsCb;
/**
  * @brief
  * CPU Overload control state info
  */
typedef struct rgSchCpuOvrLdCntrlCb 
{
   U8  cpuOvrLdIns;
   U32 tgtDlTpt;
   U32 tgtUlTpt;
   U8  dlNxtIndxDecNumUeTti; /*!< Total DL num UE per TTI reduction instruction applied */
   U8  ulNxtIndxDecNumUeTti; /*!< Total UL num UE per TTI reduction instruction applied */
   U8  maxUeNewTxPerTti[10]; /*!< list of subframe  where DL num UE per TTI reduction applied */
   U8  maxUeNewRxPerTti[10]; /*!< list of subframe  where UL num UE per TTI reduction applied */
}RgSchCpuOvrLdCntrlCb; 

/**
  * @brief
  * Cell Control block per cell.
  */
struct rgSchCellCb
{
   CmLteCellId   cellId;           /*!< Cell ID */
   Inst          instIdx;          /*!< Index of the scheduler instance */
   Inst          macInst;          /*!< Index of the MAC instance */
   U8            schTickDelta;     /* 4UE_TTI_DELTA */
   Bool          stopSiSch;        /*!< If TRUE Bcch,Pcch Scheduling is not done */
   U8            stopDlSch;        /*!< If TRUE DL scheduling is not done */ 
   Bool          stopUlSch;        /*!< If TRUE UL scheduling is not done */
   Bool          isDlDataAllwd;    /*!< FALSE for Uplink subframes */ 
   RgrDlHqCfg    dlHqCfg;          /*!< HARQ related configuration */
   RgSchRntiDb   rntiDb;           /*!< RNTIs DB: range of rntis to be managed by MAC */
   struct schdInfoS
   {
      Void         *sch;    /*!< Common Scheduler specific information */
      RgSchdApis   *apis;   /*!< Common Scheduler APIs */
   } sc;

   Bool            isCpUlExtend;       /*!< Cyclic prefix : TRUE-extended/FALSE-normal */
   Bool            isCpDlExtend;       /*!< Cyclic prefix : TRUE-extended/FALSE-normal */
   U8              numTxAntPorts;    /*!< Number of Tx antenna ports */
   RgrBwCfg        bwCfg;            /*!< Bandwidth Configuration */
   U8              pbchRbStart;      /*!< Indicates the Start RB of the center 6 RBs of DL BW */
   U8              pbchRbEnd;        /*!< Indicates the Start RB of the center 6 RBs of DL BW */
   U8              numCellRSPerSf;   /*!< Indicates the number of cell specific
                                          Reference symbols in a Subframe */
   RgrPhichCfg     phichCfg;         /*!< PHICH Config Information */
   RgrPucchCfg     pucchCfg;         /*!< PUCCH Config Information */
   RgSchSrsCfg     srsCfg;           /*!< SRS Config Information */
   RgrRachCfg      rachCfg;          /*!< RACH Configuration */
   /* R8 Upgrade */
   RgSchBiInfo     biInfo;           /* CELL wide BI Info */
   RgrSiCfg        siCfg;            /*!< SI Configuration */
   /* Added support for SPS*/
#ifdef LTEMAC_SPS
   RgrPuschCfg         puschCfg;     /*!< Cell-specific hopping configuration */
#endif
   RgrPreambleSetCfg macPreambleSet; /*!< Range of PDCCH Order Preamble Id's
                                         to be managed by MAC */
   CmLteTimingInfo crntTime;         /*!< Current frame and subframe number for
                                          the cell */
   CmLteTimingInfo hiDci0Time;       /*!< Scheduling SFN, SF for HIDCI-0 */
   CmLteTimingInfo hqRlsTime;        /*!< SFN, SF for HARQ clean up */
   CmLteTimingInfo dlSfRlsTime;      /*!< SFN, SF for Downlink subframe clean up*/
   CmLteTimingInfo dlDciTime;        /*!< SFN, SF for DL control Request */ 
   CmLteTimingInfo rcpReqTime;       /*!< SFN, SF for UL reception Request */ 
   RgSchCfgCfb     rgCfgInfo;        /*!< Control block for configuration related
                                          information  */
   U8              ulCcchId;         /*!< LcId for uplink CCCH */
   U8              dlCcchId;         /*!< LcId for downlink CCCH */
   RgSchClcDlLcCb  cmnLcCb[RGSCH_MAX_CMN_LC_CB]; /*!< BCCH/PCCH logical channel control block */
   CmHashListCp    ueLst;                     /*!< Hash list of UE control
                                                   blocks: RgSchUeCb */
   /* Added support for SPS*/
#ifdef LTEMAC_SPS
   CmHashListCp    spsUeLst;            /*!< Hash list of UE control blocks
                                          for spsRnti: RgSchUeCb */
#endif /* LTEMAC_SPS */
   CmLListCp       taUeLst;          /*!< List of ueCbs having ta */
#ifdef RGR_V1
   /* CR timer changes*/
   CmLListCp       ccchSduUeLst;     /*!< List of ueCbs scheduled for CCCH
                  SDU */
   CmLListCp       contResGrdTmrLst; /*!< List of raCbs whose Guard timer is
                   running  */
   CmLListCp       contResTmrLst;    /*!< List of raCbs whose contention
                   resolution timer is running  */
#endif
   RgSchRaInfoCb   raInfo;           /*!< Random access related information for
                                          cell */
   CmLListCp       pdcchLst;         /*!< List of free RgSchPdcch structures */
   CmTqCp          tqCp;
   CmTqType        tq[RGSCH_UE_TQ_SIZE];
   U8              crntSfIdx;       /*!< Current index for allocation */
#ifdef LTE_TDD
   RgInfSfAlloc    sfAllocArr[RGSCH_SF_ALLOC_SIZE]; /*!< Subframe Allocation
                                            info to be sent to MAC */
#else
   RgInfSfAlloc    sfAllocArr[RGSCH_NUM_SUB_FRAMES]; /*!< Subframe Allocation
                                            info to be sent to MAC */
#endif
   RgInfRlsHqInfo  rlsHqArr[RGSCH_NUM_SUB_FRAMES]; /*!< Harq Release
                                            info to be sent to MAC */
   U8              crntHqIdx;       /*!< Current index for Harq release info */
   RgSchLowSapCb   *tfuSap;
/* Added for sending TTI tick to RRM */
#if (defined(RGR_RRM_TICK) || defined(RGR_CQI_REPT))
   RgSchUpSapCb    *rgrSap;         /*!< Pointer to the cell's RGR SAP
                                      Control Block */
#endif
   RgSchUpSapCb    *rgmSap;         /*!< Pointer to the cell's RGM SAP
                                      Control Block */
#ifdef RGR_RRM_TICK
   U8               rrmTtiIndPrd;   /*!< Periodicity of TTI indication from
                                         MAC towards RGR user. Range [1-255]. A
                                         value of 1 means one tick per System
                                         Frame and 2 means one tick per 2 System
                                         Frame, and so on. */
#endif
   PdcchOrderCfg   pdcchOrdCfg;
   RgSchMeasGapCb    measGapCb;    /*!< Measurement gap control block */
   RgSchAckNakRepCb  ackNakRepCb;  /*!< Ack-Nack Repetition control block */
#ifdef LTE_TDD
   RgSchTddRachRspLst   *rachRspLst;     /*!< List of awaiting RACH responses */
   U8                   numDlSubfrms;    /*!< Number of DL subframes */
   U8                   ulDlCfgIdx;      /*!< UL-DL Configuration Index */
   U8                   ackNackFdbkArrSize; /*!< Maximum Number of Ack/Nack 
                                               feedback information to be 
                                               stored */
   S8                   tddHqSfnCycle;   /*!< Counter to keep track of the
                                            number of sfn,sf wrap arounds.
                                            This is used for UL harq
                                            determination. It ranges from
                                            0 to num Ul Harq-1 */
   RgSchTddSplSubfrmCfg splSubfrmCfg;    /*!< Special subframe configuration */
   Bool                 isDwPtsCnted;    /*!< is DwPts counted as PDCCH sf */
   RgSchTddNpValTbl     rgSchTddNpValTbl;/*!< Mapping between 'Np' and 'p' */
   RgSchDlSf            **subFrms;       /*!< DL subframes list */
   RgSchTddPrachRscInfo prachCfg;        /*!< PRACH configuration */
   CmHashListCp         ueTfuPendLst;    /*!< Used for HARQ Ack/Nack
                                              Multiplexing */
   /* TODO:: change to array of pointers */
   CmLListCp         n1ResUsedLst[RGSCH_NUM_SUB_FRAMES]; /*!< For storing the used
                                            N1 resources for scell in case of F1B CS */
#else
   RgSchDlSf          *subFrms[RGSCH_NUM_DL_slotS];
   U16             nCce;
#endif
   RgSchDynCfiCb         dynCfiCb;        /*!< Dynamic CFI control block */  
/* Changes for MIMO feature addition */
   U8                   noOfRbgs;       /*!< Number of RBGs for this bw */
   U8                   rbgSize;        /*!< RBG Size */
   /* Added support for SPS*/
#ifdef LTEMAC_SPS
   RgSchBwRbgInfo     dlBwRbgInfo; /*!< RBG information for the configured
                                         DL BW */
   RgrSpsCellCfg    spsCellCfg;    /*!< SPS specific configuration for
                                                         the cell */
   RgSchBwRbgInfo  spsBwRbgInfo;    /*!< RBG information for configured maximum
                                        SPS bandwidth */
#endif
#ifdef LTE_L2_MEAS
   RgSchRaPreambles  raPrmbs;              /*!< Different received RA preambles */
   RgSchAvgPrbDl     avgPrbDl;             /*!< PRB usage in percentage per QCI for DL */
   RgSchAvgPrbUl     avgPrbUl;             /*!< PRB usage in percentage per QCI for UL */
   CmLListCp         l2mList;              /*!< List of all L2M requests
                                           rgSchL2MeasCb */
   RgSchQciCb        qciArray[LRG_MAX_QCI]; /*!< Array of all qci's configured
                                            rgSchQciCb */
   Bool              sndL2Meas;             /*!< send L2 Meas in case no le mesurement timer is running*/
   RgSchTbCnt        dlUlTbCnt;             /*!< Count of DL and UL TB transmitteed and Faulty 
                                              TB (for wich NACK is received) */
#endif /* LTE_L2_MEAS */
   U8                ulAvailBw;            /*!< Total Uplink band width available
                                              for this sub frame */
#ifdef TFU_UPGRADE
 RgSchPerPucchCb   pCqiSrsSrLst[RG_SCH_PCQI_SRS_SR_TRINS_SIZE];
                                /*!< To store next active Periodic CQI/PMI, RI,
                                     SRS and SR Transmission instance */
#endif /* TFU_UPGRADE */

#ifdef LTEMAC_HDFDD
 CmLListCp         hdFddLst;   /*!< Half Duplex FDD UE list */
#endif /* LTEMAC_HDFDD */
   RgSchDRXCellCb    *drxCb;   /*!< Pointer to the DRX control block shall be
                                 allocated at the time of cell configuration. */
   /* Added for SI Enhancement*/
#ifdef RGR_SI_SCH
   RgSchSiCb       siCb;            /*!< SI Control Block */
#endif /*RGR_SI_SCH */
   RgSchPrbUsage prbUsage; /*!< measures average PRB usage for configured interval*/
   U16  t300TmrVal; /*!< t300Timer value configured in Frames */
/* LTE_ADV_FLAG_REMOVED_START */
   TknStrOSXL            rntpAggrInfo;    /*!< RNTP Info */
   RgrLoadInfReqInfo     loadInfReqInfo;  /*!< Consists startRb & endRb 
                                               location for which CC to 
                                               be raised to P-High */
   RgSchLteAdvFeatureCb  lteAdvCb;        /*!< Control block for LTE Adv 
                                               features */
/* LTE_ADV_FLAG_REMOVED_END */
   U32   dlNumUeSchedPerTti[RG_MAX_NUM_UE_PER_TTI]; /*!<DL mUe/Tti histograms*/
   U32   ulNumUeSchedPerTti[RG_MAX_NUM_UE_PER_TTI]; /*!<UL mUe/Tti histograms*/
   Bool  overLoadBackOffEnab; /*!< Overload Rach Backoff enable/disable */
   U8    overLoadBackOffval;  /*!< Overload Rach BackOff value */
   U8    refreshUeCnt[RGSCH_MAX_REFRESH_OFFSET];  /*! To maintain number of UE refreshed per subframe */
   U8    minDlResNonCsg;     /*!< Minimum DL resources reserved for Non CSG UEs */
   U8    minUlResNonCsg;     /*!< Minimum UL resources reserved for CSG UEs */
   Bool isAutoCfgModeEnb;     /*!< Indicates Auto config of TM mode is enabled or
                               disabled. True - Enabled, False - Disabled */
   CmLListCp lcMonitorLst;         /*LC Lst used for flow cntrl */
   U32       prbCnt;         /*!<PRB usage in flow control interval*/
   U32       maxPrbCnt;      /*!<Max PRB cnt after which Flow Cntrl 
                               can be triggered */
   RgSchCpuOvrLdCntrlCb cpuOvrLdCntrl; /*!< CPU Overload control state info */
   RgSchMeasCb          measurements;  /*!< Cell level measurements */
   RgSchThrsldsCb       thresholds;    /*!< Cell level thresholds */
#ifdef TENB_STATS
   TSL2CellStatsCb   *tenbStats; /*!< CELL Stats Holder */
#endif
   RgSchCellDciSize  dciSize;
#ifdef LTE_ADV
   CmLListCp        sCellUeLst; /*!< List of UEs for which this cell is secondary cell*/
   Bool             isPucchFormat3Sptd;
#endif
#ifdef RG_PFS_STATS
   U32              totalPrb;
   U32              totalTime;
#endif
   Void *            laaCb;
#ifdef EMTC_ENABLE
   /* TODO:: Below members need to be moved
    * to emtc specific files and have void * 
    * here */
   U8                    emtcEnable;
   Void                  *emtcCb;
   RgSchRntiDb           emtcRntiDb;           /*!< RNTIs DB: range of rntis to be managed by MAC */
#endif
   RgSchIotResMngmtCb resMngmt;
#ifdef RG_5GTF
   RgSch5gtfCellCb    cell5gtfCb;
#endif
};

/**
  * @brief
  * Structure holding LTE MAC's General Configuration information.
  */
typedef struct rgSchGenCb
{
   U8              tmrRes;           /*!< Timer resolution */
   RgSchLmResponse *bndCfmResp;       /*!< Respones value for Bind Confirm */
   RgSchLmResponse ustaResp;         /*!< Respones value for Alarms */
   U8              startCellId;      /*!< Starting Cell Id */
#ifdef LTE_ADV
   Bool            forceCntrlSrbBoOnPCel; /*!< value 1 means force scheduling
                                               of RLC control BO and SRB BO on
                                               PCell. val 0 means don't force*/
   Bool            isSCellActDeactAlgoEnable; /*!< TRUE will enable activation/deactivation algo at Schd */
#endif
}RgSchGenCb;

#ifdef RG_5GTF
/* 5GTF Dynamic TDD Specific defines */
 
/* Number of subframes information managed */
#define RG_SCH_DYNTDD_MAX_SFINFO   20

/* Subframe Types */
#define RG_SCH_DYNTDD_NOTDEF       0
#define RG_SCH_DYNTDD_DLC_DLD      1 
#define RG_SCH_DYNTDD_DLC_DLD_ULC  2       
#define RG_SCH_DYNTDD_DLC_ULD      3
#define RG_SCH_DYNTDD_DLC_ULD_ULC  4

/* Mark the subframe */
#define RG_SCH_DYN_TDD_MARKTYPE(_dynTdd, _sfi, _state)\
{\
   (_dynTdd)->sfInfo[(_sfi)].sfType = _state;\
}

/* Mark sfi as UL Subframe */
#define RG_SCH_DYN_TDD_MARKTYPE_UL(_dynTdd, _sfi)\
{\
   RG_SCH_DYN_TDD_MARKTYPE(_dynTdd, _sfi, RG_SCH_DYNTDD_DLC_ULD);\
}

/* Mark sfi as DL Subframe */
#define RG_SCH_DYN_TDD_MARKTYPE_DL(_dynTdd, _sfi)\
{\
   RG_SCH_DYN_TDD_MARKTYPE(_dynTdd, _sfi, RG_SCH_DYNTDD_DLC_DLD);\
}

/* Get SFI and SFN from given time and subframe offset */
#define RG_SCH_DYN_TDD_GET_SFIDX(_sfi, _crntSfIdx, _offset)\
          (_sfi) = (_crntSfIdx + _offset)% RG_SCH_DYNTDD_MAX_SFINFO

/**
  @brief Dynamic TDD subframe type. */

typedef struct rgSchDynTddSfType
{
  U8              sfType;  /*!< 0= NOT Defined 
                                1= DL Cntrl + DL Data
                                2= DL Cntrl + DL Data + UL Cntrl
                                3= DL Cntrl + UL Data
                                4= DL Cntrl + UL Data + UL Cntrl
                           */ 

}RgSchDynTddSfType;
/**
  @brief Dynamic TDD control Block */

typedef struct rgSchDynTddCb
{
   Bool                 isDynTddEnbld; /*!< Is dynamic TDD enabled */
   U8                   crntDTddSfIdx; /*!< Pivot Index corresponding 
                                            cell's current subframe */ 
   RgSchDynTddSfType    sfInfo[RG_SCH_DYNTDD_MAX_SFINFO]; 
}RgSchDynTddCb;

#endif

/**
  * @brief
  * Global Control block for LTE-MAC.
  */
struct _rgSchCb
{
   TskInit       rgSchInit;                    /*!< Task Init info */
   RgSchGenCb    genCfg;                    /*!< General Config info */
   U8            numSaps;                   /*!< Num RGR Saps = Num TFU Saps */
   RgSchUpSapCb  *rgrSap;                    /*!< RGR SAP Control Block */
   RgSchLowSapCb *tfuSap;                    /*!< TFU SAP Control Block */
   RgSchUpSapCb  *rgmSap;                    /*!< TFU SAP Control Block */
   CmTqCp        tmrTqCp;                   /*!< Timer Task Queue Cntrl Point */
   CmTqType      tmrTq[RGSCH_TQ_SIZE];         /*!< Timer Task Queue */
   U8            rgSchDlDelta;                /* 4UE_TTI_DELTA */
   U8            rgSchCmnDlDelta;
   U8            rgSchUlDelta;
   RgSchCellCb   *cells[CM_LTE_MAX_CELLS];  /* Array to store cellCb ptr */  
   RgrSchedEnbCfg rgrSchedEnbCfg;                  /*!< eNB level RR/PFS Config */
   Void          *rgSchEnbPfsDl;             /*!< eNB level PFS DL Block */

   Void *         laaCb;
#ifdef RG_5GTF
   RgSchDynTddCb  rgSchDynTdd;               /*!< Dynamic TDD Control Block */
#endif
};

/* Declaration for scheduler control blocks */
EXTERN RgSchCb rgSchCb[RGSCH_MAX_INST];

/*
 * Data structures for RAM
 */

/**
  * @brief
  * Random Access Req Info to be stored in cellCb.
  */
typedef struct rgSchRaReqInfo
{
   CmLList         raReqLstEnt;  /*!< Linked list entity for RaReq List */
   CmLteTimingInfo timingInfo;   /*!< RACHO: Time of RaReq Reception */
   RgTfuRaReqInfo  raReq;        /*!< Random Access Request Information */
   RgSchUeCb       *ue;          /*!< UECB if RAP ID is a dedicated one */ 
} RgSchRaReqInfo;

/**
  * @enum rgSchRaState
  * Enumeration of random access states.
  */
typedef enum rgSchRaState
{
   RGSCH_RA_MSG3_PENDING,   /*!< Msg3 reception pending */
   RGSCH_RA_MSG4_PENDING,   /*!< Msg4 transmission pending */
   RGSCH_RA_MSG4_DONE      /*!< Msg4 transmission successful */
} RgSchRaState;

/**
  * @brief
  * Control block for Random Access.
  */
struct rgSchRaCb
{
   CmLList         raCbLnk;       /*!< To link to the raCb list */
   CmLList         schdLnk;       /*!< To link raCb to the "to be scheduled"
                                       list */
   CmLteRnti       tmpCrnti;      /*!< Temporary C-RNTI */
   CmLteTimingInfo timingInfo;    /*!< Timing information */
   RgSchRntiLnk    *rntiLnk;      /*!< Link to RNTI for raCb */
   RgSchRaState    raState;       /*!< Random access state */
   struct
   {
      U32          bo;            /*!< Buffer occupancy for CCCH */
   } dlCcchInfo;                  /*!< Params for DL CCCH */
   U8              msg3HqProcId;  /*!< Msg3 Harq Process ID */
   /*ccpu00128820 - DEL - msg3HqProcRef is delete for Msg3 alloc double delete issue*/
   RgSchUlHqProcCb msg3HqProc;    /*!< msg3HqProcRef points to this initially */
   RgSchUeCb       *ue;           /*!< NULL initially */
   Bool            toDel;         /*!< To delete this RaCb after msg4 reject */
   TknU8           phr;           /*!< To store the PHR, if received along with
                                       Msg3 */
   CmLList         rspLnk;        /*!< Used to link RACB to a frame for resp */
   U8              rapId;         /*!< RAP ID */
   TknU16          ta;            /*!< Timing Adjustment */
   RgSchUlGrnt     msg3Grnt; /*!< Msg3 grant as given by the UL Sched */
   U32             y[RGSCH_NUM_SUB_FRAMES]; /*!< y values using tmpCrnti by DLSCHED */
   RgSchDlHqEnt    *dlHqE;     /*!< DL HARQ module */
   U8              ccchCqi;     /*!< DL Cqi obtained from RaReq and Used for CCCH */
   RgSchDlRbAlloc  rbAllocInfo; /*!< RB Allocation Info for MSG4 Trans/Retrans */
   /* PHR handling for MSG3 */
   CmLteTimingInfo msg3AllocTime; /*!< Allocation time for msg3 grant */
#ifdef RGR_V1
   /* CR timer  changes*/
   CmLList         contResTmrLnk;       /*!< To link raCb to the
                   Guard Timer/Contention Resolution timer list*/
   CmLteTimingInfo expiryTime; /*!< Expiry time for Guard/Contention
             Resolution timers */

   U32             ccchSduBo;   /*!<To store CCCH SDU BO if it arrives while
              ContRes CE is transmitting or retransmitting*/
#endif
#ifdef EMTC_ENABLE
   Bool            isEmtcRaCb;       /*!< 0 - Normal, 1 - EMTC */
   Void            *emtcRaInfo;
#endif
};

/**
  * @brief
  * Carries the Error information.
  */
struct rgSchErrInfo
{
   U8    errType;    /*!< Error Type */
   U16   errCause;   /*!< Cause of Error */
};

/* Global Variables */
#ifdef LTE_TDD

#ifdef LTEMAC_SPS
EXTERN U8 rgSchTddSpsDlMaxRetxTbl[RGSCH_MAX_TDD_UL_DL_CFG]; 
#endif
typedef U8 RgSchTddUlDlSubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddUlDlSubfrmTbl rgSchTddUlDlSubfrmTbl;

typedef struct rgSchTddSplSubfrmInfo RgSchTddSplSubfrmInfoTbl[RGSCH_MAX_TDD_SPL_SUBFRM_CFG];
EXTERN RgSchTddSplSubfrmInfoTbl rgSchTddSplSubfrmInfoTbl;

typedef struct rgSchTddDlAscSetIdxK RgSchTddDlAscSetIdxKTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddDlAscSetIdxKTbl rgSchTddDlAscSetIdxKTbl;
/* ccpu00132282 */
EXTERN RgSchTddDlAscSetIdxKTbl rgSchTddDlHqPucchResCalTbl;

typedef U8 RgSchTddPhichMValTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddPhichMValTbl rgSchTddPhichMValTbl;

typedef U8 RgSchTddKPhichTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddKPhichTbl rgSchTddKPhichTbl;

typedef RgSchTddPhichOffInfo RgSchTddPhichOffInfoTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];

typedef U8 RgSchTddUlAscIdxKDashTbl[RGSCH_MAX_TDD_UL_DL_CFG-1][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddUlAscIdxKDashTbl rgSchTddUlAscIdxKDashTbl;

#ifdef LTEMAC_SPS
typedef U8 RgSchTddInvDlAscSetIdxTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddInvDlAscSetIdxTbl rgSchTddInvDlAscSetIdxTbl;
#endif

typedef U8 RgSchTddPuschTxKTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddPuschTxKTbl rgSchTddPuschTxKTbl;

typedef U8 RgSchTddUlNumHarqProcTbl[RGSCH_MAX_TDD_UL_DL_CFG];
EXTERN RgSchTddUlNumHarqProcTbl rgSchTddUlNumHarqProcTbl;

typedef U8 RgSchTddDlNumHarqProcTbl[RGSCH_MAX_TDD_UL_DL_CFG];
EXTERN RgSchTddDlNumHarqProcTbl rgSchTddDlNumHarqProcTbl;

/* Number of ACK/NACK Feedback to be stored based on UL-DL Configuration Index */
typedef U8 RgSchTddANFdbkMapTbl[RGSCH_MAX_TDD_UL_DL_CFG];
EXTERN RgSchTddANFdbkMapTbl rgSchTddANFdbkMapTbl;

/* Number of UL subframes */
typedef RgSchTddSubfrmInfo RgSchTddMaxUlSubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG];
EXTERN RgSchTddMaxUlSubfrmTbl rgSchTddMaxUlSubfrmTbl;

/* Number of UL subframes */
typedef U8 RgSchTddNumUlSubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddNumUlSubfrmTbl rgSchTddNumUlSubfrmTbl;

/* Number of low UL subframes Indices*/
typedef U8 RgSchTddLowUlSubfrmIdxTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddLowUlSubfrmIdxTbl rgSchTddLowUlSubfrmIdxTbl;

/* Number of high UL subframes Indices*/
typedef U8 RgSchTddHighUlSubfrmIdxTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddHighUlSubfrmIdxTbl rgSchTddHighUlSubfrmIdxTbl;

/* Number of low DL subframes Indices*/
typedef U8 RgSchTddLowDlSubfrmIdxTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddLowDlSubfrmIdxTbl rgSchTddLowDlSubfrmIdxTbl;

/* Number of high DL subframes Indices*/
typedef U8 RgSchTddHighDlSubfrmIdxTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddHighDlSubfrmIdxTbl rgSchTddHighDlSubfrmIdxTbl;

/* Number of DL subframes and Special subframes with DwPTS */
typedef U8 RgSchTddNumDlSubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddNumDlSubfrmTbl rgSchTddNumDlSubfrmTbl;

/* Number of DL subframes and Special subframes with DwPTS */
typedef RgSchTddSubfrmInfo RgSchTddMaxDlSubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG];
EXTERN RgSchTddMaxDlSubfrmTbl rgSchTddMaxDlSubfrmTbl;

typedef U8 RgSchTddMsg3SubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddMsg3SubfrmTbl rgSchTddMsg3SubfrmTbl;
#ifdef LTEMAC_SPS
typedef RgSchTddMsg3SubfrmTbl RgSchTddSpsUlRsrvTbl;
EXTERN RgSchTddMsg3SubfrmTbl rgSchTddSpsUlRsrvTbl;
#endif

typedef U8 RgSchTddRlsDlSubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddRlsDlSubfrmTbl rgSchTddRlsDlSubfrmTbl;


EXTERN U8 rgSchTddPucchTxTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];

#endif
#ifdef LTE_ADV
EXTERN RgSchCellCb* rgSchUtlGetCellCb ARGS((
Inst         inst,
U16          cellId
));

EXTERN Void rgSCHSCellDlUeReset ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue
));

EXTERN Void rgSCHSCellDlLcCfg ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));

EXTERN Void rgSCHSCellDlLcDel ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));

EXTERN Void rgSCHSCellDlDedBoUpd ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));

EXTERN Void rgSCHSCellSchdActDeactCe ARGS((
RgSchUeCb         *ueCb,
RgSchDlHqTbCb     *tbInfo
));

EXTERN Void rgSCHSCellAddToActDeactLst ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue
));

EXTERN Void rgSCHSCellRmvFrmActLst ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue
));
EXTERN S16 rgSCHSCellIsActive ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue
));



EXTERN Void rgSCHSCellHndlFdbkInd ARGS((
RgSchDlHqProcCb   *hqP,
U8                tbIdx,
U8                fdbk,
Bool              maxHqRetxReached
));

EXTERN Void rgSCHSCellDeactTmrExpry ARGS((
RgSchUeCellInfo *sCell
));

EXTERN S16 rgSCHSCellTrigActDeact ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ueCb,
U8            sCellIdx,
U8            action
));

EXTERN S16 rgSCHSCellDelUe ARGS((
RgSchCellCb  *cellCb,
RgSchUeCb    *ueCb
));

EXTERN Bool rgSCHIsActvReqd ARGS ((
RgSchCellCb    *cell,
RgSchUeCb      *ue
));

EXTERN Void rgSCHSCellSelectAndActDeAct ARGS ((
RgSchCellCb  *PCell,
RgSchUeCb    *ueCb,
U8           action
));

EXTERN S16 rgSCHSCellPCqiCfg ARGS((
RgSchCellCb  *priCellCb,
RgSchCellCb  *secCellCb,
RgSchUeCb    *ueCb,
RgrUePrdDlCqiCfg  *cqiCfg,
CmLteUeCategory   ueCat,
U8            sCellIdx
));

EXTERN Void rgSCHUtlSndUeSCellDel2Mac ARGS ((
RgSchCellCb    *cell,
CmLteRnti      rnti
));

EXTERN U8 rgSCHUtlGetMaxTbSupp ARGS ((
RgrTxMode txMode
));
#endif/*LTE_ADV*/

/* APIs exposed by TMR module */
EXTERN Void rgSCHTmrStartTmr ARGS((
         RgSchCellCb *cellCb,
         Ptr         cb,
         S16         tmrEvnt,
         U32         tmrVal));

EXTERN Void rgSCHTmrStopTmr ARGS((
         RgSchCellCb *cellCb,
         S16         tmrEvnt,
         Ptr         cb));

EXTERN Void rgSCHTmrProcTmr ARGS((
         Ptr         cb,
         S16         tmrEvnt));

/* APIs exposed by TOM */

EXTERN S16 rgSCHTomRaReqInd ARGS((
         RgSchCellCb     *cell,
         TfuRaReqIndInfo *raReqInd));

EXTERN S16 rgSCHTomPucchDeltaPwrInd ARGS((
         RgSchCellCb           *cell,
         TfuPucchDeltaPwrIndInfo *pucchDeltaPwr));

EXTERN S16 rgSCHTomUlCqiInd ARGS((
         RgSchCellCb     *cell,
         TfuUlCqiIndInfo *ulCqiInd));

EXTERN S16 rgSCHTomSrInd ARGS((
         RgSchCellCb     *cell,
         TfuSrIndInfo    *srInd));

EXTERN S16 rgSCHTomDlCqiInd ARGS((
         RgSchCellCb     *cell,
         TfuDlCqiIndInfo *dlCqiInd));
/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
EXTERN S16 rgSCHTomRawCqiInd ARGS
((
RgSchCellCb     *cell,
TfuRawCqiIndInfo* rawCqiInd
));

EXTERN S16 rgSCHTomSrsInd ARGS
((
RgSchCellCb     *cell,
TfuSrsIndInfo* srsInd
));

#endif

EXTERN S16 rgSCHTomDoaInd ARGS((
         RgSchCellCb     *cell,
         TfuDoaIndInfo   *doaInd));
EXTERN S16 rgSCHTomCrcInd ARGS((
         RgSchCellCb       *cell,
         TfuCrcIndInfo *crcInd));

EXTERN S16 rgSCHTomHarqAckInd ARGS((
         RgSchCellCb       *cell,
         TfuHqIndInfo      *harqAckInd));

EXTERN S16 rgSCHTomTimingAdvInd ARGS((
         RgSchCellCb         *cell,
         TfuTimingAdvIndInfo *timingAdvInd));


/*
 * APIs exposed by LMM
 */
EXTERN S16 rgSCHLmmStartTmr ARGS ((Inst instId, S16 tmrEvnt,
         U32 tmrVal, PTR cb));
EXTERN S16 rgSCHLmmStopTmr ARGS((Inst instId, S16 tmrEvnt, PTR cb));
EXTERN S16 rgSCHLmmTmrExpiry ARGS((PTR cb, S16 tmrEvnt));
/* This function invokes a Control Confirmation to the LM from scheduler. */
EXTERN S16 rgSCHLmmBndCfm ARGS((Pst *pst, SuId suId, U8 status));
EXTERN S16 schActvTmr ARGS((Ent entity, Inst inst));
/* To send a Unsolicited Status Indication to Layer Manager */
EXTERN S16 rgSCHLmmStaInd ARGS((Inst instId, U16 category, U16 event,
                          U16 cause, RgUstaDgn *dgn));
EXTERN S16 schActvTsk ARGS((Pst *pst, Buffer *mBuf));
EXTERN Void SchFillCfmPst ARGS((Pst    *reqPst,Pst *cfmPst,RgMngmt  *cfm));
EXTERN U16 SchInstCfg ARGS((RgCfg    *cfg, Inst inst));
EXTERN Void printSchCellInfo ARGS((Void));
EXTERN Void rgSCHLmmGenCntrl ARGS((RgMngmt *cntrl,RgMngmt  *cfm,Pst *cfmPst));
EXTERN Void rgSCHLmmSapCntrl ARGS((RgMngmt *cntrl,RgMngmt *cfm,Pst *cfmPst));

#ifdef EMTC_ENABLE
EXTERN S16 rgSCHCfgEmtcCellCfg ARGS ((RgSchCellCb   *cell,
         RgrEmtcCellCfg *emtcCellCfg));
EXTERN S16 rgSCHCfgVldtRgrEmtcCellCfg ARGS ((RgrCellCfg *cellCfg));

EXTERN Void rgSchTomTtiEmtcSched ARGS((RgSchCellCb        *cell));
EXTERN S16 rgSCHCfgVldtEmtcUeCfg ARGS((RgSchCellCb *cell, RgrUeEmtcCfg *emtcUeCfg));
EXTERN S16 rgSCHUtlUpdUeEmtcInfo ARGS((RgSchCellCb *cell, RgrUeCfg *ueCfg, RgSchUeCb *ueCb));
EXTERN S16 rgSCHEmtcCellDel ARGS((RgSchCellCb        *cell));
EXTERN S16 rgSCHEmtcUeDel ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
EXTERN S16 rgSCHEmtcHdFddUeCfg ARGS((RgSchCellCb  *cellCb,RgSchUeCb *ueCb,Bool hdFddEnbl));

#endif
/*
 * APIs exposed by CFG module
 */
EXTERN S16 rgSCHCfgVldtRgrCellCfg ARGS((Inst inst, RgrCellCfg  *cellCfg,
      RgSchCellCb *cell, RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgRgrCellCfg ARGS((RgSchCb *instCb, SpId spId,
      RgrCellCfg *cellCfg, RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgRgrSchedEnbCfg ARGS((Inst inst, SpId spId,
      RgrSchedEnbCfg *schedEnbCfg, RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgVldtRgrCellRecfg ARGS((Inst inst, RgrCellRecfg *cellRecfg,
      RgSchCellCb  **cell, RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgRgrCellRecfg ARGS((RgSchCellCb *cell, RgrCellRecfg *cellRecfg,
      RgSchErrInfo *errInfo));

EXTERN S16 rgSCHCfgVldtRgrUeCfg ARGS((Inst inst, RgrUeCfg  *ueCfg,
      RgSchCellCb  **cell, RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgRgrUeCfg ARGS((RgSchCellCb  *cell, RgrUeCfg  *ueCfg,
      RgSchErrInfo  *errInfo));
EXTERN S16 rgSCHCfgVldtRgrUeRecfg ARGS((Inst inst, RgrUeRecfg *ueRecfg,
      RgSchCellCb **cell, RgSchUeCb **ue, RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgRgrUeRecfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgrUeRecfg
      *ueRecfg, RgSchErrInfo *errInfo));

EXTERN S16 rgSCHCfgVldtRgrLcCfg ARGS((Inst inst, RgrLchCfg *lcCfg,
      RgSchCellCb **cell, RgSchUeCb **ue, RgSchErrInfo  *errInfo));
EXTERN S16 rgSCHCfgRgrLchCfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
      RgrLchCfg *lcCfg, RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgVldtRgrLchRecfg ARGS((Inst inst, RgrLchRecfg *lcRecfg,
      RgSchCellCb **cell, RgSchUeCb **ue, RgSchDlLcCb **dlLc,
      RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgRgrLchRecfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *dlLc, RgrLchRecfg *lcRecfg, RgSchErrInfo *errInfo));

EXTERN S16 rgSCHCfgVldtRgrLcgRecfg ARGS ((Inst inst,RgrLcgRecfg *lcgRecfg,
         RgSchCellCb *cell,RgSchUeCb **ue,RgSchErrInfo *errInfo ));
EXTERN S16 rgSCHCfgVldtRgrLcgCfg ARGS ((Inst inst,RgrLcgCfg *lcgCfg,
         RgSchCellCb **cell,RgSchUeCb **ue,RgSchErrInfo *errInfo ));
EXTERN S16 rgSCHCfgVldtRgrSchedEnbCfg ARGS ((Inst inst, 
         RgrSchedEnbCfg *schedEnbCfg, RgSchErrInfo *errInfo ));
EXTERN S16 rgSCHCfgRgrLcgCfg ARGS ((RgSchCellCb *cell,RgSchUeCb *ue,
         RgrLcgCfg *lcgCfg,RgSchErrInfo *errInfo ));
EXTERN S16 rgSCHCfgRgrLcgRecfg ARGS ((RgSchCellCb *cell,RgSchUeCb *ue,
         RgrLcgRecfg *lcgRecfg,RgSchErrInfo *errInfo));

EXTERN S16 rgSCHCfgVldtRgrUeReset ARGS((Inst inst, RgrRst *reset, RgSchCellCb *cell,
         RgSchUeCb **ue,RgSchErrInfo     *errInfo));
EXTERN S16 rgSCHCfgRgrUeReset ARGS((RgSchCellCb *cell,RgSchUeCb *ue,RgrRst *reset,
         RgSchErrInfo   *errInfo));

EXTERN S16 rgSCHCfgRgrCellDel ARGS((RgSchCellCb *cell, RgrDel *cellDelInfo,
      RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgRgrUeDel ARGS((RgSchCellCb *cell, RgrDel *ueDelInfo,
      RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgRgrLcDel ARGS((RgSchCellCb *cell, RgrDel *lcDelInfo,
      RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgRgrLcgDel ARGS ((RgSchCellCb *cell,RgrDel *lcDelInfo,
         RgSchErrInfo *errInfo));
EXTERN Void rgSCHCfgFreeCellCb ARGS((RgSchCellCb *cell));
/* Added for SI Enhancement*/
#ifdef RGR_SI_SCH
EXTERN S16 rgSCHCfgVldtRgrSiCfg ARGS(( Inst inst, RgrSiCfgReqInfo *siCfg,
                    RgSchCellCb      *cell, RgSchErrInfo *errInfo));
EXTERN S16 rgSCHGomHndlSiCfg ARGS(( Region   reg, Pool   pool,
                  RgSchCb       *instCb, SpId    spId,
                  RgrCfgTransId transId, RgrSiCfgReqInfo *cfgReqInfo));
EXTERN S16 rgSCHUtlRgrSiCfgCfm ARGS ((Inst inst, SpId spId,
                                  RgrCfgTransId transId,U8 status));

EXTERN S16 rgSCHGomHndlWarningSiCfg ARGS(( Region   reg, Pool   pool,
                  RgSchCb       *instCb, SpId    spId, RgrCfgTransId transId,
                  RgrWarningSiCfgReqInfo *warningSiCfgReqInfo));

EXTERN Void rgSCHGomHndlWarningSiStopReq ARGS(( Region   reg, Pool   pool,
                  RgSchCb       *instCb, U8            siId, 
                  RgrCfgTransId transId, SpId    spId));

EXTERN S16 rgSCHUtlRgrWarningSiCfgCfm ARGS ((Inst inst, SpId spId, U8 siId,
                                  RgrCfgTransId transId,U8 status));
#endif /* RGR_SI_SCH */
/* LTE_ADV_FLAG_REMOVED_START */
EXTERN S16 rgSchDSFRRntpInfoInit ARGS ((TknStrOSXL *rntpPtr, RgSchCellCb  *cell,
                                           U16 bw));
EXTERN S16 rgSchDSFRRntpInfoFree ARGS ((TknStrOSXL *rntpPtr, RgSchCellCb  *cell,
                                           U16 bw));
EXTERN S16 rgSchUpdtRNTPInfo ARGS ((RgSchCellCb *cell, RgSchDlSf  *sf,
                                RgrLoadInfReqInfo       *loadInfReq));
EXTERN S16 rgSCHCfgVldtRgrLoadInf ARGS(( Inst inst, RgrLoadInfReqInfo *loadInfReq,
                    RgSchCellCb      *cell, RgSchErrInfo *errInfo));
EXTERN S16 rgSCHGomHndlLoadInf ARGS(( Region   reg, Pool   pool,
                    RgSchCb       *instCb, SpId    spId,
                    RgrCfgTransId transId, RgrLoadInfReqInfo *cfgReqInfo));
/* LTE_ADV_FLAG_REMOVED_END */

/*
 * APIs exposed by GOM module
 */
EXTERN S16 rgSCHGomHndlCfg ARGS((Pst *pst, RgSchCb *instCb,
                            RgrCfgTransId transId, RgrCfgReqInfo *cfgReqInfo));
EXTERN S16 rgSCHGomTtiHndlr ARGS((RgSchCellCb *cell, SpId spId));


/*
 * APIs exposed by RAM module
 */
EXTERN S16 rgSCHRamVldtUeCfg ARGS((
RgSchCellCb  *cell,
RgrUeCfg     *ueCfg
));
EXTERN S16 rgSCHRamProcRaReq     ARGS((U8 raReqCnt, RgSchCellCb *cell, CmLteRnti raRnti,
                                    TfuRachInfo  *raReqInd,
                                    CmLteTimingInfo timingInfo, 
                                    RgSchUeCb *ue,
                                    RgSchErrInfo *err));
EXTERN S16 rgSCHRamCreateRaCb ARGS((RgSchCellCb *cell, RgSchRaCb **raCb,
                                    RgSchErrInfo *err));
EXTERN S16 rgSCHRamRgrUeCfg      ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                                    RgSchRaCb *raCb, RgSchErrInfo *err));
EXTERN S16 rgSCHRamProcMsg3      ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                                    RgSchRaCb *raCb, RgInfUeDatInd  *pdu,
                                    RgSchErrInfo *err));
EXTERN S16 rgSCHRamUpdtBo        ARGS((RgSchCellCb *cell, RgSchRaCb *raCb,
                                    RgInfCmnBoRpt *staRsp));
EXTERN S16 rgSCHRamMsg3DatInd   ARGS((RgSchRaCb *raCb));
EXTERN S16 rgSCHRamMsg3FailureInd   ARGS((RgSchRaCb *raCb));
EXTERN S16 rgSCHRamMsg4FdbkInd   ARGS((RgSchRaCb *raCb));
EXTERN S16 rgSCHRamMsg4Done      ARGS((RgSchCellCb *cell, RgSchRaCb *raCb));
EXTERN S16 rgSCHRamDelRaCb       ARGS((RgSchCellCb *cell, RgSchRaCb *raCb,
                                    Bool rlsRnti));
EXTERN S16 rgSCHRamFreeCell      ARGS((RgSchCellCb *cell));
EXTERN S16 rgSCHRamTtiHndlr      ARGS((RgSchCellCb *cell));
EXTERN Void rgSCHCmnUlSch ARGS((RgSchCellCb *cell));
EXTERN Void rgSCHCmnDlCommonChSch ARGS ((RgSchCellCb  *cell));
#ifdef RGR_V1
/* Added periodic BSR timer */
EXTERN S16 rgSCHCmnBsrTmrExpry ARGS(( RgSchUeCb  *ueCb));
#endif
#ifdef LTE_TDD
EXTERN S16 rgSCHRamDelRaReq      ARGS((RgSchCellCb *cell,
                                       CmLteTimingInfo timingInfo,
                                       U8 raIdx));
#endif
EXTERN S16 rgSCHRamAddToRaInfoSchdLst(RgSchCellCb   *cell, RgSchRaCb *raCb);

EXTERN S16 rgSCHRamRmvFrmRaInfoSchdLst(RgSchCellCb   *cell, RgSchRaCb   *raCb);

/* APIs exposed by UHM */
/* Added for Uplink Adaptive retransmission */
EXTERN Void rgSCHUhmNonadapRetx ARGS((RgSchUlHqProcCb *hqProc));
EXTERN S16 rgSCHUhmHqEntInit ARGS ((RgSchCellCb       *cellCb, RgSchUeCb *ueCb));
#ifndef MAC_SCH_STATS
EXTERN Void rgSCHUhmProcDatInd ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         CmLteTimingInfo frm));
#else /* MAC_SCH_STATS */
EXTERN Void rgSCHUhmProcDatInd ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         CmLteTimingInfo frm, U8 cqi));
#endif
EXTERN Void rgSCHUhmProcMsg3DatInd ARGS((RgSchUlHqProcCb *hqProc));
EXTERN Void rgSCHUhmProcMsg3Failure ARGS((RgSchUlHqProcCb *hqProc));
#ifndef MAC_SCH_STATS
EXTERN Void rgSCHUhmProcHqFailure ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         CmLteTimingInfo frm, TknU8 rv));
#else /* MAC_SCH_STATS */
EXTERN Void rgSCHUhmProcHqFailure ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         CmLteTimingInfo frm, TknU8 rv, U8 cqi));
#endif
EXTERN RgSchUlHqProcCb* rgSCHUhmGetUlHqProc ARGS((RgSchCellCb      *cell, RgSchUeCb *ue,
         U8 idx));
EXTERN Void rgSCHUhmNewTx ARGS((RgSchUlHqProcCb *hqProc, U8 maxHqRetx,
         RgSchUlAlloc *alloc));
EXTERN Void rgSCHUhmFreeProc ARGS((RgSchUlHqProcCb *hqProc,
         RgSchCellCb      *cell));
EXTERN Void rgSCHUhmRetx ARGS((RgSchUlHqProcCb *hqProc, RgSchUlAlloc *alloc));
EXTERN Void rgSCHUhmRgrUeCfg ARGS(( RgSchCellCb *cellCb, RgSchUeCb *ueCb,
          RgrUeCfg *ueCfg));
EXTERN Void rgSCHUhmRgrUeRecfg ARGS(( RgSchCellCb *cellCb, RgSchUeCb *ueCb,
          RgrUeRecfg *ueRecfg));
EXTERN Void rgSCHUhmFreeUe ARGS(( RgSchCellCb *cellCb, RgUeUlHqCb *hqEnt));
EXTERN S16 rgSCHUhmAppendPhich ARGS(( RgSchCellCb *cellCb,
          CmLteTimingInfo frm, U8 idx));

/* APIs exposed by DBM */
EXTERN S16 rgSCHDbmInitCell ARGS((RgSchCellCb *cellCb));
EXTERN S16 rgSCHDbmDeInitUeCbLst ARGS(( RgSchCellCb *cellCb));
#ifdef LTE_TDD
EXTERN S16 rgSCHDbmDeInitUeTfuPendLst ARGS(( RgSchCellCb *cellCb));
#endif
EXTERN S16 rgSCHDbmInsUeCb ARGS(( RgSchCellCb *cellCb, RgSchUeCb *ueCb));
EXTERN RgSchUeCb* rgSCHDbmGetUeCb ARGS(( RgSchCellCb *cellCb, CmLteRnti ueId));
EXTERN RgSchUeCb* rgSCHDbmGetNextUeCb ( RgSchCellCb *cellCb, RgSchUeCb *ueCb);
EXTERN S16 rgSCHDbmDelUeCb ARGS(( RgSchCellCb *cellCb, RgSchUeCb *ueCb));
#ifdef LTE_L2_MEAS
EXTERN S16 rgSCHDbmDelL2MUe ARGS(( RgSchCellCb *cellCb, RgSchUeCb *ueCb));
#endif
EXTERN S16 rgSCHDbmInitUe ARGS((RgSchUeCb *ueCb));
EXTERN Void rgSCHDbmInsDlDedLcCb ARGS((RgSchUeCb *ueCb, RgSchDlLcCb *dlLcCb));
EXTERN Void rgSCHDbmDelDlDedLcCb ARGS((RgSchUeCb *ueCb, RgSchDlLcCb *dlLcCb));
EXTERN RgSchDlLcCb* rgSCHDbmGetDlDedLcCb ARGS(( RgSchUeCb *ueCb, CmLteLcId idx));
EXTERN RgSchDlLcCb* rgSCHDbmGetFirstDlDedLcCb ARGS((RgSchUeCb *ueCbb));
EXTERN RgSchDlLcCb* rgSCHDbmGetNextDlDedLcCb ARGS((RgSchUeCb *ueCb, RgSchDlLcCb *lcCb));
EXTERN RgSchClcDlLcCb* rgSCHDbmGetCmnLcCb ARGS(( RgSchCellCb *cellCb, CmLteLcId lcId));
EXTERN RgSchClcDlLcCb* rgSCHDbmGetBcchOnBch ARGS(( RgSchCellCb *cellCb ));
EXTERN RgSchClcDlLcCb* rgSCHDbmGetFirstBcchOnDlsch ARGS(( RgSchCellCb *cellCb));
EXTERN RgSchClcDlLcCb* rgSCHDbmGetSecondBcchOnDlsch ARGS(( RgSchCellCb *cellCb));
EXTERN RgSchClcDlLcCb* rgSCHDbmGetPcch ARGS(( RgSchCellCb *cellCb));
EXTERN Void rgSCHDbmInsBcchOnBch ARGS(( RgSchCellCb *cellCb, RgSchClcDlLcCb *cmnDlLcCb));
EXTERN Void rgSCHDbmInsBcchOnDlsch ARGS(( RgSchCellCb *cellCb, RgSchClcDlLcCb *cmnDlLcCb));
EXTERN Void rgSCHDbmInsPcch ARGS(( RgSchCellCb *cellCb, RgSchClcDlLcCb *cmnDlLcCb));

EXTERN Void rgSCHDbmInitCmnLcBoLst ARGS(( RgSchClcDlLcCb *cmnDlLcCb));
EXTERN Void rgSCHDbmInsCmnLcBoRpt ARGS(( RgSchClcDlLcCb *cmnDlLcCb,
                                    RgSchClcBoRpt *cmnBoRpt));
EXTERN RgSchRaCb* rgSCHDbmGetRaCb ARGS(( RgSchCellCb *cellCb, CmLteRnti key));
EXTERN Void rgSCHDbmInsCrntRgrCfgElem ARGS(( RgSchCellCb *cellCb,
                                          RgSchCfgElem *cfgElem));
EXTERN Void rgSCHDbmInsPndngRgrCfgElem ARGS(( RgSchCellCb *cellCb,
                                          RgSchCfgElem *cfgElem));
EXTERN RgSchCfgElem* rgSCHDbmGetNextCrntRgrCfgElem ARGS(( RgSchCellCb *cellCb,
                                                   RgSchCfgElem *cfgElem));
EXTERN RgSchCfgElem* rgSCHDbmGetNextPndngRgrCfgElem ARGS(( RgSchCellCb *cellCb,
                                                   RgSchCfgElem *cfgElem));
EXTERN RgSchCfgElem* rgSCHDbmGetPndngRgrCfgElemByKey ARGS(( RgSchCellCb *cellCb,
                                                   CmLteTimingInfo key));
EXTERN RgSchCfgElem* rgSCHDbmDelCrntRgrCfgElem ARGS(( RgSchCellCb *cellCb,
                                                RgSchCfgElem *cfgElem));
EXTERN RgSchCfgElem* rgSCHDbmDelPndngRgrCfgElem ARGS(( RgSchCellCb *cellCb,
                                                RgSchCfgElem *cfgElem));

EXTERN S16 rgSCHDbmRntiDbInit ARGS(( RgSchCellCb *cellCb, U16 rntiStart, U16 maxRntis));
EXTERN Void rgSCHDbmRntiDbDeInit ARGS(( RgSchCellCb *cellCb));
EXTERN RgSchRntiLnk* rgSCHDbmGetRnti ARGS(( RgSchCellCb *cellCb));
EXTERN Void rgSCHDbmRlsRnti ARGS(( RgSchCellCb *cellCb, RgSchRntiLnk *rntiLnk));
/* Fix : syed HO UE does not have a valid ue->rntiLnk */
EXTERN Void rgSCHUtlIndRntiRls2Mac ARGS(( RgSchCellCb *cell, CmLteRnti rnti,
                                          Bool ueIdChng, CmLteRnti newRnti));

/*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
EXTERN S16 rgSCHDbmDeInitSpsUeCbLst ARGS((RgSchCellCb *cellCb));
EXTERN S16 rgSCHDbmInsSpsUeCb ARGS((RgSchCellCb *cellCb, RgSchUeCb *ueCb));
EXTERN RgSchUeCb* rgSCHDbmGetSpsUeCb ARGS((RgSchCellCb *cellCb, CmLteRnti ueId));
EXTERN RgSchUeCb* rgSCHDbmGetNextSpsUeCb ARGS((RgSchCellCb *cellCb, RgSchUeCb *ueCb));
EXTERN S16 rgSCHDbmDelSpsUeCb ARGS((RgSchCellCb *cellCb,RgSchUeCb *ueCb));
#endif /* LTEMAC_SPS */

#ifdef LTE_L2_MEAS
/*
 * L2M APIs
 */
EXTERN S16 rgSchL2mMeasReq ARGS ((
         RgSchCellCb       *cell,
         LrgSchMeasReqInfo *measInfo,
         RgSchErrInfo      err));
EXTERN S16 RgSchMacL2MeasSend ARGS
((
Pst*                pst,
RgInfL2MeasSndReq *measInfo
));

EXTERN S16 RgSchMacL2MeasStop ARGS
((
Pst*                pst,
RgInfL2MeasStopReq *measInfo
));
#endif /* LTE_L2_MEAS */
/*
 * DHM APIs
 */
/* LTE_ADV_FLAG_REMOVED_START */
EXTERN S16 rgSchSFRTotalPoolInit ARGS((RgSchCellCb *cell, RgSchDlSf *sf));
/* LTE_ADV_FLAG_REMOVED_END */
EXTERN Void rgSCHDhmHqPAdd2FreeLst ARGS (( RgSchDlHqProcCb *hqP));
EXTERN Void rgSCHDhmHqPAdd2InUseLst ARGS (( RgSchDlHqProcCb *hqP));
EXTERN Void rgSCHDhmHqPDelFrmFreeLst ARGS (( RgSchDlHqProcCb *hqP));
EXTERN Void rgSCHDhmHqPDelFrmInUseLst ARGS (( RgSchDlHqProcCb *hqP));

EXTERN RgSchDlHqEnt *rgSCHDhmHqEntInit ARGS((RgSchCellCb *cell));
EXTERN S16 rgSCHDhmGetAvlHqProc ARGS((RgSchCellCb *cell, RgSchUeCb *ue, CmLteTimingInfo timingInfo,
                                   RgSchDlHqProcCb **hqP));
EXTERN Void rgSCHDhmHqRetx ARGS((RgSchDlHqEnt *hqE, CmLteTimingInfo timeInfo,
                                   RgSchDlHqProcCb *hqP));
EXTERN RgSchDlHqProcCb * rgSCHDhmLastSchedHqProc ARGS((RgSchDlHqEnt *hqE));
/* CR timer changes*/
EXTERN S16 rgSCHDhmGetCcchSduHqProc ARGS((RgSchUeCb *ueCb, CmLteTimingInfo timeInfo, 
                                          RgSchDlHqProcCb **hqP));
EXTERN S16 rgSCHDhmGetMsg4HqProc ARGS((RgSchRaCb *raCb, CmLteTimingInfo timeInfo));
EXTERN Void rgSCHDhmRlsHqProc ARGS((RgSchDlHqProcCb *hqP));
/* ccpu00118350 : Correcting NDI manipulation of Harq */
EXTERN Void rgSCHDhmRlsHqpTb ARGS((RgSchDlHqProcCb *hqP, U8 tbIdx, Bool togNdi));
EXTERN Void rgSCHUtlDlHqPTbAddToTx ARGS((RgSchDlSf *subFrm,
RgSchDlHqProcCb *hqP, U8 tbIdx ));
EXTERN Void rgSCHDhmHqTbRetx ARGS(( RgSchDlHqEnt *hqE,
CmLteTimingInfo timingInfo, RgSchDlHqProcCb *hqP, U8 tbIdx));
EXTERN Void rgSCHUtlDlHqPTbAddToTx ARGS((RgSchDlSf *subFrm,
RgSchDlHqProcCb *hqP, U8 tbIdx ));
EXTERN Void rgSCHDhmHqTbRetx ARGS(( RgSchDlHqEnt *hqE,
CmLteTimingInfo timingInfo, RgSchDlHqProcCb *hqP, U8 tbIdx));
#ifdef RG_UNUSED
EXTERN S16 rgSCHDhmGetHqProcFrmId ARGS((RgSchCellCb *cell, RgSchUeCb *ue, U8 idx,
                                    RgSchDlHqProcCb **hqP));
#endif
/* Changes for MIMO feature addition */
EXTERN Void rgSCHDhmSchdTa ARGS((RgSchUeCb *ueCb, RgSchDlHqTbCb *tbInfo));
EXTERN S16 rgSCHDhmHqFdbkInd ARGS((Void *cb, U8 cbType, RgSchCellCb *cellCb,
                     CmLteTimingInfo timingInfo, RgTfuHqInfo *fdbk, RgInfRlsHqInfo
                                    *rlsHqBufs,RgSchErrInfo *err));
#ifdef EMTC_ENABLE
EXTERN S16 rgSCHDhmEmtcHqFdbkInd ARGS((Void *cb, U8 cbType, RgSchCellCb *cellCb,
                     CmLteTimingInfo timingInfo, RgTfuHqInfo *fdbk, RgInfRlsHqInfo
                                    *rlsHqBufs,RgSchErrInfo *err));
EXTERN S16 rgSCHUtlAddToResLst 
(
 CmLListCp   *cp, 
 RgSchIotRes *iotRes
 );
#endif
/*CA Dev Start */
EXTERN S16 rgSCHDhmPrcFdbkForTb(RgSchCellCb *cell,RgSchUeCb  *ue,
                               RgSchDlHqProcCb *hqP,RgSchDlSf *sf,Bool isMsg4,
                               U16 rnti,U8 tbCnt,CmLteTimingInfo timingInfo, U8 isAck,
                               RgInfRlsHqInfo *rlsHqBufs,RgSchErrInfo *err
                               );
/*CA Dev End */
EXTERN Void rgSCHDhmRgrUeCfg ARGS((RgSchCellCb *cellCb, RgSchUeCb *ueCb,
                                    RgrUeCfg *ueCfg, RgSchErrInfo *err));
EXTERN Void rgSCHDhmRgrUeRecfg ARGS((RgSchCellCb *cellCb, RgSchUeCb *ueCb,
                                    RgrUeRecfg *ueCfg, RgSchErrInfo *err));
EXTERN Void rgSCHDhmRgrCellCfg ARGS((RgSchCellCb *cellCb, RgrCellCfg *cellCfg,
                                    RgSchErrInfo *err));
EXTERN Void rgSCHDhmRgrCellRecfg ARGS((RgSchCellCb *cellCb, RgrCellRecfg
                                    *cellRecfg, RgSchErrInfo *err));
EXTERN Void rgSCHDhmFreeUe ARGS((RgSchUeCb *ueCb));
EXTERN Void rgSCHDhmUpdTa ARGS((RgSchCellCb *cellCb, RgSchUeCb *ueCb, U8 ta));
EXTERN Void rgSCHDhmProcTAExp ARGS((RgSchUeCb *ue));
/* Changes for MIMO feature addition */
EXTERN S16 rgSCHDhmAddLcData ARGS((Inst inst, RgSchLchAllocInfo *lchData,
                                    RgSchDlHqTbCb *tbInfo));
EXTERN S16 rgSCHDhmRlsDlsfHqProc ARGS((RgSchCellCb *cellCb, CmLteTimingInfo
timingInfo));

#ifdef LTE_TDD
EXTERN S16 rgSCHDhmTddRlsSubFrm ARGS((RgSchCellCb *cell, CmLteTimingInfo uciTimingInfo));
EXTERN S16 rgSCHCfgVldtTddDrxCycCfg ARGS((RgSchCellCb  *cell, U16 drxCycle, 
                                    U8 onDurTmr, U16 offSet));
#endif
/* Added support for SPS*/
#ifdef LTEMAC_SPS
EXTERN S16 rgSCHDhmGetHqProcFrmId ARGS((
RgSchCellCb           *cell,
RgSchUeCb               *ue,
U8                      idx,
RgSchDlHqProcCb         **hqP
));
#endif /* LTEMAC_SPS */
/* Freeing up the HARQ proc blocked for
 * indefinite time in case of Retx */
EXTERN S16 rgSCHDhmDlRetxAllocFail ARGS((
RgSchUeCb        *ue,
RgSchDlHqProcCb  *proc
));
/* MS_WORKAROUND for ccpu00122893 temp fix Incorrect HqProc release was done instead of
 * a Harq Entity reset. Fixing the same */
EXTERN Void rgSCHDhmHqEntReset ARGS((
   RgSchDlHqEnt      *hqE
));
/* Measurement GAP and ACK NACK */

EXTERN S16 rgSCHMeasGapANRepUeCfg ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgrUeCfg            *ueCfg
));
EXTERN S16 rgSCHMeasGapANRepUeRecfg ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgrUeRecfg          *ueRecfg
));
/* ccpu00133470- Added extra argument to identify UE DEL*/
EXTERN Void rgSCHMeasGapANRepUeDel ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
Bool                isUeDel
));
EXTERN S16 rgSCHMeasGapANRepTtiHndl ARGS((
RgSchCellCb         *cell
));
EXTERN S16 rgSCHMeasGapANRepGetDlInactvUe ARGS((
RgSchCellCb         *cell,
CmLListCp           *dlInactvUeLst
));
EXTERN S16 rgSCHMeasGapANRepGetUlInactvUe ARGS((
RgSchCellCb         *cell,
CmLListCp           *ulInactvUeLst
));
EXTERN Void rgSCHMeasGapANRepDlInactvTmrExpry ARGS((
RgSchUeCb           *ue,
U8                   tmrEvnt
));
EXTERN Void rgSCHMeasGapANRepUlInactvTmrExpry ARGS((
RgSchUeCb           *ue,
U8                   tmrEvnt
));
EXTERN Void rgSCHMeasGapANRepTmrExpry ARGS((
RgSchUeCb           *ue
));
EXTERN Void rgSCHAckNakRepTmrExpry ARGS((
RgSchUeCb           *ue
));
EXTERN Void rgSCHAckNakRepSndHqFbkRcpReq ARGS((
RgSchCellCb         *cell,
RgSchDlSf           *dlSf,
CmLteTimingInfo     timingInfo));

EXTERN Void rgSCHAckNakRepAddToQ ARGS((
RgSchCellCb       *cell,
RgSchDlSf         *crntDlSf));

/*
 * SCH Util APIs
 */
#ifdef LTEMAC_SPS
EXTERN Void rgSCHUtlHdlCrcInd ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue,
CmLteTimingInfo timingInfo
));
#endif

#ifdef LTE_L2_MEAS
EXTERN  S16 rgSCHUtlValidateMeasReq ARGS ((RgSchCellCb *cellCb,
         LrgSchMeasReqInfo *schL2MeasInfo,
         RgSchErrInfo      *err
         ));
EXTERN S16 rgSchL2mSndCfm ARGS((Pst   *pst,
         RgSchL2MeasCb   *measCb,
         LrgSchMeasReqInfo *measInfo,
         Bool              isErr
));
EXTERN  S16 rgSchFillL2MeasCfm ARGS((
         RgSchCellCb       *cell,
         RgSchL2MeasCb     *measCb,
         LrgSchMeasCfmInfo *cfm,
         U32               measTime
));
EXTERN Void rgSchL2mFillCfmPst ARGS((
         Pst    *pst,
         Pst    *cfmPst,
         LrgSchMeasReqInfo *measInfo
));
EXTERN S16 rgSCHL2Meas ARGS((
         RgSchCellCb  *cell,
         U8           isCalrCrcInd
));
#endif /* LTE_L2_MEAS */
/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
EXTERN F64 rgSCHUtlPower ARGS
((
F64 x,
F64 n
));

 EXTERN U32 rgSCHUtlParse ARGS
  ((
 U8 *buff,
 U8 startPos,
 U8 endPos,
 U8 buffSize
 ));

 EXTERN  U8 rgSCHUtlFindDist ARGS
((
U16    crntTime,
U16    tempIdx
));
#endif
EXTERN Bool rgSCHUtlPdcchAvail ARGS((RgSchCellCb *cell, RgSchPdcchInfo
        *pdcchInfo, CmLteAggrLvl aggrLvl, RgSchPdcch **pdcch));
EXTERN Void rgSCHUtlPdcchPut ARGS((RgSchCellCb *cell, RgSchPdcchInfo *pdcchInfo,
        RgSchPdcch *pdcch));
#ifdef LTE_TDD
/* Changes for passing iPhich at TFU interface*/
EXTERN S16 rgSCHUtlAddPhich  ARGS((RgSchCellCb *cellCb, CmLteTimingInfo frm,
         U8 hqFeedBack, U8 nDmrs, U8 rbStart, U8 iPhich));
#else
EXTERN S16 rgSCHUtlAddPhich  ARGS((RgSchCellCb *cellCb, CmLteTimingInfo frm,
         U8 hqFeedBack, U8 nDmrs, U8 rbStart,Bool isForMsg3));
#endif
EXTERN RgSchDlSf* rgSCHUtlSubFrmGet ARGS((RgSchCellCb *cell,
         CmLteTimingInfo frm));
EXTERN Void rgSCHUtlSubFrmPut ARGS((RgSchCellCb *cell, RgSchDlSf *sf));
EXTERN U8 rgSCHUtlLog32bitNbase2 ARGS((U32 n));
/* Added support for SPS*/


#ifdef LTEMAC_SPS
EXTERN RgSchDlHqProcCb * rgSCHDhmSpsDlGetHqProc ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
CmLteTimingInfo  timingInfo));
#endif
#ifdef LTE_TDD
EXTERN U8 rgSCHUtlCalcNCce ARGS((U8 bw, RgrPhichNg ng, U8 cfi, U8 mPhich,
                                 U8 numAntna, Bool isEcp));
#else
EXTERN U8 rgSCHUtlCalcNCce ARGS((U8 bw, RgrPhichNg ng, U8 cfi, U8 numAntna, Bool
isEcp));
#endif
#ifdef LTE_TDD
/* Changes for passing iPhich at TFU interface*/
EXTERN S16 rgSCHUtlGetPhichInfo ARGS((RgSchUlHqProcCb *hqProc, U8 *rbStartRef,
         U8 *nDmrsRef, U8 *iPhich));
#else
EXTERN S16 rgSCHUtlGetPhichInfo ARGS((RgSchUlHqProcCb *hqProc, U8 *rbStartRef,
         U8 *nDmrsRef));
#endif
/*  Added changes of TFU_UPGRADE */
#ifndef TFU_UPGRADE
/* To include the length and ModOrder in DataRecp Req. */
/* Updating NDI and HARQ proc Id */
EXTERN S16 rgSCHUtlAllocRcptInfo ARGS((RgSchUlAlloc *alloc, CmLteRnti *rnti,
           U8 *iMcsRef, U8 *rbStartRef, U8 *numRbRef, U8 *rvRef, U16 *size,
           TfuModScheme *modType,Bool *isRtx,
U8 *nDmrs,
Bool *ndi,
U8 *hqPId));
#else
EXTERN S16 rgSCHUtlAllocRcptInfo ARGS((
         RgSchCellCb             *cell,
         RgSchUlAlloc            *alloc,
         CmLteTimingInfo         *timeInfo,
         TfuUeUlSchRecpInfo      *recpReq
         ));
#endif /* TFU_UPGRADE */

EXTERN S16 rgSCHUtlRgrCellCfg ARGS((RgSchCellCb *cell, RgrCellCfg *cellCfg,
         RgSchErrInfo *errInfo));
EXTERN S16 rgSCHUtlRgrCellRecfg ARGS((RgSchCellCb *cell, RgrCellRecfg *recfg,
         RgSchErrInfo *errInfo));
EXTERN S16 rgSCHUtlFreeCell ARGS((RgSchCellCb *cell));
EXTERN S16 rgSCHUtlRgrUeCfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgrUeCfg *cfg, RgSchErrInfo *err));
EXTERN S16 rgSCHUtlRgrLcCfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *dl, RgrLchCfg *cfg,RgSchErrInfo *errInfo));
EXTERN S16 rgSCHUtlRgrLcDel ARGS((RgSchCellCb *cell, RgSchUeCb *ue, 
         CmLteLcId lcId, U8 lcgId));
EXTERN S16 rgSCHUtlRgrLcRecfg ARGS ((RgSchCellCb *cell,RgSchUeCb *ue,
      RgSchDlLcCb *dlLc,RgrLchRecfg *recfg,RgSchErrInfo *err));
EXTERN S16 rgSCHUtlRgrLcgCfg ARGS ((RgSchCellCb *cell,RgSchUeCb *ue,
         RgrLcgCfg *cfg,RgSchErrInfo *errInfo));
EXTERN S16 rgSCHUtlRgrLcgRecfg ARGS ((RgSchCellCb *cell,RgSchUeCb *ue,
         RgrLcgRecfg *recfg,RgSchErrInfo *err));
EXTERN Void rgSCHUtlRgrLcgDel ARGS ((RgSchCellCb *cell,RgSchUeCb *ue,
         U8 lcgId));
EXTERN Void rgSCHUtlDlCqiInd ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         TfuDlCqiRpt *dlCqiInd, CmLteTimingInfo timingInfo));

/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
EXTERN Void rgSCHUtlRawCqiInd ARGS
((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
TfuRawCqiRpt*     rawCqiRpt,
CmLteTimingInfo    timingInfo
));

EXTERN Void rgSCHUtlSrsInd ARGS
((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
TfuSrsRpt*     srsRpt,
CmLteTimingInfo    timingInfo
));
EXTERN S16 rgSCHUtlGetCfgPerOff ARGS
((
RgSchPerTbl     tbl,
U16             cfgIdx, 
U16             *peri, 
U16             *offset
));
#endif

EXTERN Void rgSCHUtlDoaInd ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         TfuDoaRpt *doaInd));
EXTERN Void rgSCHUtlDlTARpt ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
/* Changes for MIMO feature addition */
EXTERN Void rgSCHUtlDlRlsSubFrm ARGS((RgSchCellCb *cell, CmLteTimingInfo subFrm));
EXTERN Void rgSCHUtlDlProcAddToRetx ARGS((RgSchCellCb *cell,
         RgSchDlHqProcCb *hqP));
EXTERN S16 rgSCHUtlRegSch ARGS((U8 schIdx, RgSchdApis *apis));
EXTERN Void rgSCHUtlDlHqProcAddToTx ARGS((RgSchDlSf *subFrm, RgSchDlHqProcCb *hqP));
/* Changes for MIMO feature addition */
EXTERN Void rgSCHUtlDlHqPTbRmvFrmTx ARGS((RgSchDlSf *subFrm,
         RgSchDlHqProcCb *hqP, U8 tbIdx, Bool isRepeating));
EXTERN S16 rgSCHUtlRgrUeRecfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgrUeRecfg *recfg, RgSchErrInfo *err));
EXTERN Void rgSCHUtlFreeDlLc ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *dlLc));
EXTERN Void rgSCHUtlFreeUlLc ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchUlLcCb *ulLc));
EXTERN Void rgSCHUtlFreeUe ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
EXTERN Void rgSCHUtlDlDedBoUpd ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *svc));
#ifdef RG_UNUSED
EXTERN S16 rgSCHUtlUpdUlHqProc ARGS((RgSchCellCb *cell, RgSchUlHqProcCb *curProc,
         RgSchUlHqProcCb *oldProc));
#endif
/* PHR handling for MSG3 */
EXTERN Void rgSCHUtlRecMsg3Alloc ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchRaCb *raCb));
EXTERN S16 rgSCHUtlContResUlGrant ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchErrInfo *err));
EXTERN S16 rgSCHUtlSrRcvd ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         CmLteTimingInfo, RgSchErrInfo *err));
EXTERN Void rgSCHUtlUpdBsrShort ARGS((RgSchCellCb *cell, RgSchUeCb *ue, U8 lcgId,
         U8 bsr, RgSchErrInfo *err));
EXTERN Void rgSCHUtlUpdBsrTrunc ARGS((RgSchCellCb *cell, RgSchUeCb *ue, U8 lcgId,
         U8 bsr, RgSchErrInfo *err));
EXTERN Void rgSCHUtlUpdBsrLong ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         U8 bsr1,U8 bsr2,U8 bsr3,U8 bsr4, RgSchErrInfo *err));
EXTERN S16 rgSCHUtlUpdPhr ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         U8 phr, RgSchErrInfo *err));
EXTERN S16 rgSCHUtlUpdExtPhr ARGS(( RgSchCellCb  *cell, RgSchUeCb *ue,
RgInfExtPhrCEInfo * extPhr, RgSchErrInfo *err));
EXTERN S16 rgSCHUtlDataRcvd ARGS((RgSchCellCb *cell, RgSchUeCb *ue, U8 numLc,
         RgSchUlLcCb *lcArr[], U16 bytesArr[], RgSchErrInfo *err));
EXTERN Void rgSCHUtlUlCqiInd ARGS(( RgSchCellCb *cell, RgSchUeCb *ue,
                                    TfuUlCqiRpt *ulCqiInfo));
EXTERN Void rgSCHUtlPucchDeltaPwrInd ARGS(( RgSchCellCb *cell, RgSchUeCb *ue,
         S8 delta));
EXTERN Void rgSCHUtlUeReset ARGS(( RgSchCellCb *cell, RgSchUeCb *ue));
EXTERN Void rgSCHUtlUlHqProcForUe ARGS((RgSchCellCb *cell, CmLteTimingInfo frm,
         RgSchUeCb *ue, RgSchUlHqProcCb **procRef));
EXTERN RgSchUlAlloc *rgSCHUtlFirstRcptnReq ARGS((RgSchCellCb *cell));
EXTERN RgSchUlAlloc *rgSCHUtlNextRcptnReq ARGS((RgSchCellCb *cell,
         RgSchUlAlloc *alloc));
EXTERN RgSchUlAlloc *rgSCHUtlFirstHqFdbkAlloc ARGS((RgSchCellCb *cell, U8 idx));
EXTERN RgSchUlAlloc *rgSCHUtlNextHqFdbkAlloc ARGS((RgSchCellCb *cell,
         RgSchUlAlloc *alloc, U8 idx));
EXTERN S16 rgSCHUtlTfuBndReq ARGS((Inst inst, SuId suId, SpId spId));
EXTERN S16 rgSCHUtlTfuUBndReq ARGS((Inst inst, RgSchLowSapCfgInfo sapCfg, Reason reason));
#ifdef EMTC_ENABLE
EXTERN S16 rgSCHEmtcUtlResetSfAlloc ARGS((RgInfSfAlloc *sfAlloc,
            Bool resetCmnLcInfo, Bool restAlloc));
#endif
EXTERN S16 rgSCHUtlResetSfAlloc ARGS((RgInfSfAlloc *sfAlloc,
            Bool resetCmnLcInfo, Bool restAlloc));
EXTERN S16 rgSCHUtlGetSfAlloc ARGS((RgSchCellCb *cell));
EXTERN S16 rgSCHUtlPutSfAlloc ARGS((RgSchCellCb *cell));
EXTERN S16  rgSCHUtlAllocSBuf  ARGS((Inst inst, Data **pData, Size size));
/* ccpu00117052 - MOD - Passing double pointer
for proper NULLP assignment*/
EXTERN Void rgSCHUtlFreeSBuf   ARGS((Inst inst, Data **data, Size size));
EXTERN Void rgSCHUtlFillDgnParams ARGS((Inst inst, RgUstaDgn *dgn,U8 dgnType));
EXTERN Void rgSCHUtlGetPstToLyr ARGS((Pst *pst,RgSchCb *schCb,Inst macInst));
EXTERN S16 rgSCHUtlFillRgInfCmnLcInfo ARGS((RgSchDlSf *sf,RgInfSfAlloc *sfAlloc,
                                 CmLteLcId lcId, Bool sendInd));
EXTERN S16 rgSCHUtlFillRgInfRarInfo ARGS((RgSchDlSf *sf,RgInfSfAlloc *sfAlloc,RgSchCellCb *cell));
EXTERN S16 rgSCHUtlFillPdschDciInfo ARGS((TfuPdschDciInfo *pdschDci,TfuDciInfo
        *pdcchDci));
  /* CA dev Start */
EXTERN Void rgSCHUtlFillRgInfUeInfo ARGS((RgSchDlSf*, RgSchCellCb *cell, CmLListCp *dlDrxInactvTmrLst,
                                          CmLListCp *dlInActvLst, CmLListCp *ulInActvLst));
  /* CA dev End */
EXTERN S16 rgSCHUtlUpdSch ARGS((RgInfSfDatInd *subfrmInfo, RgSchCellCb *cellCb,
               RgSchUeCb *ueCb, RgInfUeDatInd *pdu,RgSchErrInfo *err));
EXTERN S16 rgSCHUtlHndlCcchBoUpdt ARGS((RgSchCellCb *cell,RgInfCmnBoRpt *boRpt));
EXTERN S16 rgSCHUtlHndlBcchPcchBoUpdt ARGS((RgSchCellCb *cell,RgInfCmnBoRpt
        *boUpdt));
EXTERN S16 rgSCHUtlRgrBndCfm ARGS ((Inst inst, SuId suId,U8 status));
/* Added for sending TTI tick to RRM */
#ifdef RGR_RRM_TICK
EXTERN S16 rgSCHUtlRgrTtiInd ARGS ((RgSchCellCb *cell, RgrTtiIndInfo *ttiInd));
#endif
EXTERN S16 schSendCfgCfm ARGS ((Region reg, Pool pool, \
         RgrCfgTransId transId, U8 status));
EXTERN S16 rgSCHUtlProcMsg3 ARGS((RgInfSfDatInd *subfrmInfo, RgSchCellCb  *cellCb,
            RgSchUeCb   *ueCb, CmLteRnti  rnti,RgInfUeDatInd  *pdu,
            RgSchErrInfo *err ));
#ifdef RG_PHASE_2
EXTERN S16 rgSCHUtlTfuGrpPwrCntrlReq ARGS((Inst inst,S16  sapId,
         TfuGrpPwrCntrlReqInfo *grpPwrCntrlReq));
#endif
EXTERN S16 rgSCHUtlTfuCntrlReq ARGS((Inst  inst, S16 sapId,
         TfuCntrlReqInfo *cntrlReq));
EXTERN S16 rgSCHUtlTfuRecpReq ARGS((Inst inst, S16  sapId,
         TfuRecpReqInfo *recpReq));
EXTERN S16 rgSCHUtlValidateTfuSap ARGS((Inst  inst,SuId  suId));
EXTERN S16 rgSCHUtlAllocEventMem ARGS((Inst  inst,Ptr  *memPtr,Size  memSize));
EXTERN S16 rgSCHUtlGetEventMem ARGS((Ptr *ptr,Size  len,Ptr   memCpa));
EXTERN S16 rgSCHUtlGetRlsHqAlloc ARGS((RgSchCellCb *cell));
EXTERN S16 rgSCHUtlPutRlsHqAlloc ARGS((RgSchCellCb *cell));

EXTERN S16 rgSCHUtlDlActvtUe ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
EXTERN S16 rgSCHUtlUlActvtUe ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
EXTERN Void rgSCHUtlHdlUlTransInd ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                                   CmLteTimingInfo timingInfo));
#ifdef TFU_UPGRADE
EXTERN Void rgSCHUtlUpdACqiTrigWt ARGS((RgSchUeCb *ue,RgSchUeCellInfo *sCellInfo, U8 isAck));
#endif
/* Nprb indication at PHY for common Ch */
/* Corrected allocation for common channels */
EXTERN S32 rgSCHUtlGetAllwdCchTbSz ARGS((U32 bo, U8 *nPrb, U8 *mcs
));
/* CR timer changes*/
EXTERN S16 rgSCHUtlUpdtBo ARGS((RgSchCellCb  *cell,
                               RgInfCmnBoRpt     *staRsp));
EXTERN S16 rgSCHUtlAddUeToCcchSduLst ARGS(
                          (RgSchCellCb       *cell,
                            RgSchUeCb         *ueCb));
#ifdef EMTC_ENABLE
EXTERN S16 rgSCHUtlAddUeToEmtcCcchSduLst ARGS(
      (RgSchCellCb       *cell,
       RgSchUeCb         *ueCb));

EXTERN S16 rgSCHRamRmvFrmEmtcRaInfoSchdLst ARGS((RgSchCellCb   *cell, RgSchRaCb *raCb));
EXTERN Void rgSCHRamEmtcDelRaCb ARGS((RgSchCellCb *cell, RgSchRaCb *raCb));
EXTERN S16 rgSCHRamEmtcUpdtBo        ARGS((RgSchCellCb *cell, RgSchRaCb *raCb,
                                    RgInfCmnBoRpt *staRsp));
#endif
/* Added for SI Enhancement*/
#ifdef RGR_SI_SCH
EXTERN Void rgSCHUtlPutSiInfo ARGS((RgSchCellCb *cell));
EXTERN Void rgSCHUtlFreeWarningSiSeg  ARGS((Region reg,Pool pool,
                                            CmLListCp *siPduLst));
EXTERN Void rgSCHUtlFreeWarningSiPdu  ARGS((RgSchCellCb *cell));
EXTERN Buffer *rgSCHUtlGetWarningSiPdu ARGS((RgSchCellCb *cell));
EXTERN S16 rgSCHUtlGetMcsAndNPrb ARGS((RgSchCellCb *cell, U8 *nPrb, U8 *mcs, MsgLen *msgLen));
EXTERN S16 rgSCHUtlCalMcsAndNPrb ARGS((RgSchCellCb *cell, U8 cfgType, MsgLen msgLen, U8 siId));
#endif/*RGR_SI_SCH*/

#ifdef LTE_TDD
EXTERN S16 rgSCHUtlAllocUeANFdbkInfo ARGS((RgSchUeCb *ue,U8 servCellIdx));
EXTERN Void rgSCHUtlDelUeANFdbkInfo ARGS((RgSchUeCb *ue,U8 servCellIdx));
EXTERN S16 rgSCHUtlInitUeANFdbkInfo ARGS((RgSchTddANInfo *anInfo));
EXTERN RgSchTddANInfo* rgSCHUtlGetUeANFdbkInfo ARGS((RgSchUeCb *ueCb, CmLteTimingInfo *timeInfo,U8 servCellIdx));
EXTERN U8  rgSCHUtlGetDlSfIdx ARGS((RgSchCellCb *cell, CmLteTimingInfo *timeInfo));
EXTERN Void rgSCHUtlPrachCfgInit ARGS((RgSchCellCb *cell, RgrCellCfg *cellCfg ));
EXTERN Void rgSCHUtlGetNxtDlSfInfo ARGS((CmLteTimingInfo curDlTime, RgSchCellCb  *cell, RgSchDlSf *dlSf, RgSchDlSf **nxtDlsf, CmLteTimingInfo *nxtDlTime));
EXTERN Void rgSCHUtlGetPrevDlSfInfo ARGS((RgSchCellCb * cell, CmLteTimingInfo curDlTime, CmLteTimingInfo *prevDlTime, U8 *numSubfrm));
#endif
EXTERN Void rgSCHCmnDlSch ARGS
((
RgSchCellCb        *cell
));
EXTERN Void rgSCHCmnSndCnsldtInfo ARGS
((
RgSchCellCb  *cell
));
EXTERN Void rgSCHCmnCnsldtSfAlloc ARGS
((
RgSchCellCb  *cell
));

/* Added support for SPS*/
EXTERN Void rgSCHCmnDlAllocFnlz ARGS
((
RgSchCellCb  *cell
));

#ifdef LTEMAC_SPS
EXTERN Void rgSCHUtlDlRelPdcchFbk ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
Bool               isAck
));

EXTERN Void rgSCHUtlDlProcAck ARGS((
RgSchCellCb        *cell,
RgSchDlHqProcCb    *hqP
));
EXTERN S16 rgSCHUtlSpsRelInd ARGS((
RgSchCellCb        *cellCb,
RgSchUeCb          *ueCb,
Bool               isExplRel
));

EXTERN Void rgSCHCmnDlSch ARGS
((
RgSchCellCb        *cell
));

EXTERN S16 rgSCHUtlSpsActInd ARGS((
RgSchCellCb        *cellCb,
RgSchUeCb          *ueCb,
U16                spsSduSize
));

EXTERN Void rgSCHUtlHdlCrcFailInd ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue,
CmLteTimingInfo timingInfo
));

EXTERN Void rgSCHUtlHdlCrntiCE ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue
));

#endif /* LTEMAC_SPS*/

/******* </AllocHolesMemMgmnt>: START *****/
EXTERN S16 rgSCHUtlUlSfInit ARGS((
         RgSchCellCb  *cell,
         RgSchUlSf    *sf,
         U8           idx,
         U8           maxUePerSf
         ));
EXTERN Void rgSCHUtlUlSfDeinit ARGS((
         RgSchCellCb *cell,
         RgSchUlSf   *sf
         ));
EXTERN RgSchUlAlloc *rgSCHUtlUlAllocGetHole ARGS((
         RgSchUlSf *sf,
         U8 numRb,
         RgSchUlHole *hole
         ));
EXTERN RgSchUlAlloc *rgSCHUtlUlAllocGetCompHole ARGS((
         RgSchUlSf *sf,
         RgSchUlHole *hole
         ));
EXTERN RgSchUlAlloc *rgSCHUtlUlAllocGetPartHole ARGS((
         RgSchUlSf *sf,
         U8 numRb,
         RgSchUlHole *hole
         ));
EXTERN Void rgSCHUtlUlAllocRls ARGS((
         RgSchUlSf *sf,
         RgSchUlAlloc *alloc
         ));

/* UL_ALLOC_ENHANCEMENT */
EXTERN Void rgSCHUtlUlAllocRelease ARGS((
         RgSchUlAlloc *alloc
         ));
EXTERN RgSchUlAlloc *rgSCHUtlUlAllocFirst ARGS((
         RgSchUlSf *sf
         ));
EXTERN RgSchUlAlloc *rgSCHUtlUlAllocNxt ARGS((
         RgSchUlSf *sf,
         RgSchUlAlloc *alloc
         ));
EXTERN RgSchUlHole *rgSCHUtlUlHoleFirst ARGS((
         RgSchUlSf *sf
         ));
EXTERN RgSchUlHole *rgSCHUtlUlHoleNxt ARGS((
         RgSchUlSf *sf,
         RgSchUlHole *hole
         ));
EXTERN RgSchUlAlloc *rgSCHUtlUlAllocGetAdjNxt ARGS((
         RgSchUlAllocDb *db,
         RgSchUlAlloc *prv
         ));
EXTERN RgSchUlAlloc *rgSCHUtlUlAllocGetFirst ARGS((
         RgSchUlAllocDb *db
         ));
EXTERN Void rgSCHUtlUlHoleAddAlloc ARGS((
         RgSchUlSf *sf,
         RgSchUlAlloc *alloc
         ));
/* UL_ALLOC_ENHANCEMENT */
EXTERN Void rgSCHUtlUlHoleAddAllocation ARGS((
         RgSchUlAlloc *alloc
         ));

EXTERN Void rgSCHUtlUlHoleJoin ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *prv,
         RgSchUlHole *nxt,
         RgSchUlAlloc *alloc
         ));
EXTERN Void rgSCHUtlUlHoleExtndRight ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *prv,
         RgSchUlAlloc *alloc
         ));
EXTERN Void rgSCHUtlUlHoleExtndLeft ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *nxt,
         RgSchUlAlloc *alloc
         ));
EXTERN Void rgSCHUtlUlHoleNew ARGS((
         RgSchUlHoleDb *db,
         RgSchUlAlloc *alloc
         ));
EXTERN Void rgSCHUtlUlHoleUpdAllocLnks ARGS((
         RgSchUlHole *hole,
         RgSchUlAlloc *prvAlloc,
         RgSchUlAlloc *nxtAlloc
         ));
EXTERN Void rgSCHUtlUlHoleIns ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *hole
         ));
EXTERN Void rgSCHUtlUlHoleIncr ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *hole
         ));
EXTERN Void rgSCHUtlUlHoleDecr ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *hole
         ));
EXTERN Void rgSCHUtlUlHoleRls ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *hole
         ));
EXTERN S16 rgSCHUtlUlAllocMemInit ARGS((
         RgSchCellCb     *cell,
         RgSchUlAllocMem *mem,
         U8 maxAllocs
         ));
EXTERN Void rgSCHUtlUlAllocMemDeinit ARGS((
         RgSchCellCb     *cell,
         RgSchUlAllocMem *mem
         ));
EXTERN S16 rgSCHUtlUlHoleMemInit ARGS((
         RgSchCellCb    *cell,
         RgSchUlHoleMem *mem,
         U8 maxHoles,
         RgSchUlHole **holeRef
         ));
EXTERN Void rgSCHUtlUlHoleMemDeinit ARGS((
         RgSchCellCb    *cell,
         RgSchUlHoleMem *mem
         ));
EXTERN RgSchUlAlloc *rgSCHUtlUlAllocMemGet ARGS((
         RgSchUlAllocMem *mem
         ));
EXTERN Void rgSCHUtlUlAllocMemRls ARGS((
         RgSchUlAllocMem *mem,
         RgSchUlAlloc *alloc
         ));
EXTERN RgSchUlHole *rgSCHUtlUlHoleMemGet ARGS((
         RgSchUlHoleMem *mem
         ));
EXTERN Void rgSCHUtlUlHoleMemRls ARGS((
         RgSchUlHoleMem *mem,
         RgSchUlHole *hole
         ));
EXTERN RgSchUlAlloc *rgSCHUtlUlGetSpfcAlloc ARGS((
         RgSchUlSf       *sf,
         U8              startSb,
         U8              numSb
));
/******* </AllocHolesMemMgmnt>: END *****/

/* DRX function declarations */
EXTERN S16 rgSCHDrxCellCfg ARGS((RgSchCellCb *cell, RgrCellCfg *cellCfg));
EXTERN Void rgSCHDrxCellDel ARGS((RgSchCellCb *cell));
EXTERN S16 rgSCHDrxUeCfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                              RgrUeCfg *ueCfg));
#ifdef RGR_V2
EXTERN S16 rgSCHDrxUeReCfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                              RgrUeRecfg *ueCfg));
#endif
EXTERN S16 rgSCHDrxUeDel ARGS((RgSchCellCb *cell,RgSchUeCb *ue));
EXTERN Void rgSCHDrxTtiInd ARGS ((RgSchCellCb *cell));

EXTERN S16 rgSCHDrxSfAlloc ARGS ((RgSchCellCb *cellCb, RgSchDlSf
                           *dlSf));
EXTERN S16 rgSCHDrxDlTrnsFail ARGS((RgSchCellCb *cell, RgSchDlHqProcCb
                             *dlHq));
EXTERN Void rgSCHDrxDedRa ARGS((RgSchCellCb *cellCb, RgSchUeCb* ueCb));
EXTERN S16 rgSCHDrxSrInd ARGS((RgSchCellCb *cell,RgSchUeCb *ue));

EXTERN Void rgSCHDrxStrtInActvTmr ARGS((RgSchCellCb *cell,
                                       CmLListCp *ueLst,
                                       U8 direction));
EXTERN S16 rgSCHUtlGetDrxSchdUesInDl ARGS((RgSchCellCb *cellCb,
                                        RgSchUeCb       *ueCb,
                                        RgSchDlHqProcCb *dlHq,
                                        RgInfUeAlloc    *allocInfo,
                                        CmLListCp       *dlDrxInactvTmrLst,
                                        CmLListCp       *dlInActvLst,
                                        CmLListCp       *ulInActvLst));
EXTERN Void rgSCHDrxStartHarqRTTTmr ARGS((RgSchCellCb     *cell,
                                          RgSchDlHqProcCb *hqP,
                                          U8              tbCnt));
EXTERN Void rgSCHDrxUeHqReset ARGS((RgSchCellCb   *cell,
                                   RgSchUeCb     *ue,
                                   RgSchDlHqEnt  *hqE,
                                   U8            cellIdx));

#ifdef TFU_UPGRADE
#ifdef LTE_TDD
EXTERN CONSTANT RgSchTddCellSpSrsSubfrmTbl rgSchTddCellSpSrsSubfrmTbl;
#else
EXTERN CONSTANT RgSchFddCellSpSrsSubfrmTbl rgSchFddCellSpSrsSubfrmTbl;
#endif
#endif

#ifdef LTEMAC_HDFDD
EXTERN S16 rgSCHHdFddUeCfg ARGS((
         RgSchCellCb  *cellCb,
         RgSchUeCb *ueCb,
         Bool hdFdd));
EXTERN S16 rgSCHHdFddUeDel  ARGS((
         RgSchCellCb *cellCb,
         RgSchUeCb *ueCb));
EXTERN Void rgSCHCmnHdFddPtUlMrk ARGS((
         RgSchCellCb *cellCb));
EXTERN Void rgSCHCmnHdFddChkUlAllow ARGS((
         RgSchCellCb *cellCb,
         RgSchUeCb *ueCb,
         U8 *flag));
EXTERN Void rgSCHCmnHdFddChkDlAllow ARGS((
         RgSchCellCb *cellCb,
         RgSchUeCb *ueCb,
         Bool *flag));
EXTERN Void rgSCHCmnHdFddChkNackAllow ARGS((
         RgSchCellCb *cellCb,
         RgSchUeCb *ueCb,
         CmLteTimingInfo timInfo,
         Bool *flag));
EXTERN Void rgSCHCmnHdFddUpdULMark ARGS((
         RgSchCellCb *cellCb,
         RgSchUeCb *ueCb));
EXTERN Void rgSCHCmnHdFddUpdDLMark ARGS((
         RgSchCellCb *cellCb,
         RgSchUeCb *ueCb));
EXTERN Void rgSCHHdFddGetSfn ARGS((
         U16 *sfn,
         CmLteTimingInfo timeInfo,
         S16 offset));
#endif /* ifdef LTEMAC_HDFDD */

/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
S16 rgSCHUtlRgrStaInd ARGS((
RgSchCellCb          *cell,
RgrStaIndInfo        *rgrSta
));

S16 rgSCHUtlFillSndStaInd ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrStaIndInfo      *staInfo,
U8                 numCqiRept
));
#endif /* End of RGR_CQI_REPT */
S16 rgSCHUtlRgrUeStaInd ARGS((
RgSchCellCb          *cell,
RgrUeStaIndInfo        *rgrUeSta
));

S16 rgSCHUtlFillSndUeStaInd ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeStaIndInfo    *ueStaInfo
));


/* LTE_ADV_FLAG_REMOVED_START */
S16 rgSCHUtlRgrLoadInfInd ARGS((
RgSchCellCb          *cell,
RgrLoadInfIndInfo        *rgrLoadInf
));
/* LTE_ADV_FLAG_REMOVED_END */
#ifdef LTE_ADV
#ifdef TFU_UPGRADE
TfuAckNackMode rgSchUtlGetFdbkMode ARGS((
RgrSchFrmt1b3TypEnum fdbkType
));

EXTERN TfuAckNackMode rgSchUtlGetFdbkMode ARGS((
RgrSchFrmt1b3TypEnum fdbkType
));

#endif /*TFU_UPGRADE */
#endif /* LTE_ADV */
/* FIX */
Void rgSCHUtlRlsRnti  ARGS((
RgSchCellCb    *cellCb,
RgSchRntiLnk   *rntiLnk,
Bool           ueIdChngd,
CmLteRnti      newRnti
));
S16 rgSCHUtlRgmBndCfm ARGS((
Inst          instId,
SuId          suId,
U8            status
));
Void rgSCHDhmDelHqEnt ARGS((
RgSchCellCb       *cell,
RgSchDlHqEnt      **hqE
));
Void rgSCHDhmAssgnUeHqEntFrmRaCb ARGS((
RgSchUeCb       *ue,
RgSchRaCb       *raCb
));
 Void rgSCHUtlReTxTa ARGS((
RgSchCellCb *cellCb, 
RgSchUeCb *ueCb));
/* LTE_ADV_FLAG_REMOVED_START */
Void rgSchSFRTotalPoolFree ARGS((
RgSchSFRTotalPoolInfo  *sfrTotalPoolInfo,
RgSchCellCb             *cell));
Void rgSchDSFRPwrCheck ARGS((
RgSchDlSf               *sf,
Bool                    *isAllUePwrHigh));
/* LTE_ADV_FLAG_REMOVED_END */

S16 rgSCHUtlUpdAvgPrbUsage ARGS((
RgSchCellCb          *cell
));

U8 rgSchUtlCfg0ReTxIdx ARGS((
RgSchCellCb    *cell,
CmLteTimingInfo phichTime,
U8              hqFdbkIdx
));

EXTERN S16 rgSCHUtlBuildNSendLcgReg ARGS((
RgSchCellCb    *cell,
CmLteRnti      crnti,
U8             lcgId,
Bool           isGbr
));

EXTERN Void rgSCHUtlPdcchInit ARGS((
         RgSchCellCb *cell,
         RgSchDlSf   *subFrm,
         U16          nCce));
EXTERN Void rgSCHDynCfiReCfg ARGS((
         RgSchCellCb   *cell,
         Bool          isDynCfiEnb
));
Void rgSchUtlCalcTotalPrbReq ARGS((RgSchCellCb  *cell,
                                          RgSchUeCb    *ue,
                                          U32           bo,
                                          U32          *prbReqrd));
EXTERN U8 rgSchUtlGetNumSbs ARGS((
RgSchCellCb    *cell,
RgSchUeCb      *ue,
U32            *numSbs
));

EXTERN U8 rgSchUtlSortInsUeLst ARGS((
RgSchCellCb    *cell,
CmLListCp    *ueLst,
CmLList      *node,
U8           subbandRequired
));
EXTERN S16 rgSCHUtlResetCpuOvrLdState ARGS((
 RgSchCellCb *cell, 
 U8          cnrtCpuOvrLdIns
));
EXTERN Void rgSCHUtlCpuOvrLdAdjItbsCap ARGS((
 RgSchCellCb *cell
));
#ifdef TFU_UPGRADE
EXTERN S16 rgSCHTomUtlPcqiSbCalcBpIdx ARGS((
CmLteTimingInfo  crntTimInfo,
RgSchUeCb        *ueCb,
RgSchUePCqiCb   *cqiCb
));

#ifdef LTE_ADV
EXTERN S16 rgSCHUtlSCellHndlCqiCollsn ARGS((
RgSchUePCqiCb     *cqiCb
));

EXTERN S16 rgSCHUtlSCellHndlRiCollsn ARGS((
RgSchUePCqiCb     *cqiCb
));
   
#endif/*LTE_ADV*/
#endif/*TFU_UPGRADE*/

EXTERN Void rgSCHTomUtlGetTrigSet ARGS((
   RgSchCellCb       *cell,
   RgSchUeCb	     *ueCb,
   U8                cqiReq,
   U8                *triggerSet
));

EXTERN Void rgSCHUtlUpdUeDciSize ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ueCb,
Bool             isCsi2Bit
));
EXTERN Void rgSCHUtlCalcDciSizes ARGS((
RgSchCellCb       *cell
));

EXTERN Void rgSchCmnPreDlSch ARGS ((
 RgSchCellCb        **cell,
 U8                 nCell,
 RgSchCellCb        **cellLst
 ));

EXTERN Void rgSchCmnPstDlSch ARGS ((
 RgSchCellCb        *cell
 ));

EXTERN U8 rgSCHCmnGetBiIndex ARGS ((
RgSchCellCb   *cell,
U32           ueCount
));

EXTERN uint8_t SchSendCfgCfm(Pst *pst, RgMngmt *cfm);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __SCH__ */


/**********************************************************************
         End of file
**********************************************************************/
