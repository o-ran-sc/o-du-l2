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
   uint32_t line;
   S8* file;
   const S8* func;
   Void * dbgVal; /* This is specific to the data struct being debug
                     for example if the debugging is done fo list
                     then this should contain the node address */
   uint32_t action;
}RgSchHistNode;

#define MAX_HIST_NODES    50

#define RGSCH_ACTION_ADD  11
#define RGSCH_ACTION_DEL  12

typedef struct rgSchHistInfo
{
   uint32_t histCount;
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
typedef uint8_t RgSchTddNpValTbl[RGSCH_TDD_MAX_P_PLUS_ONE_VAL];
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
                             uint8_t lcgId));
   Void (*rgSCHActvtUlUe) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHActvtDlUe) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHHdlUlTransInd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
            CmLteTimingInfo timingInfo));
   Void (*rgSCHUeReset) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   S16 (*rgSCHUpdBsrShort) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchLcgCb *ulLcg, uint8_t bsr, RgSchErrInfo *err));
   S16 (*rgSCHUpdBsrTrunc) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchLcgCb *ulLcg, uint8_t bsr, RgSchErrInfo *err));
   S16 (*rgSCHUpdBsrLong) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, uint8_t bsArr[], RgSchErrInfo *err));
   S16 (*rgSCHUpdPhr) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, uint8_t phr, RgSchErrInfo *err));
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
   RgSchUlAlloc *(*rgSCHFirstHqFdbkAlloc) ARGS((RgSchCellCb *cell, uint8_t idx));
   RgSchUlAlloc *(*rgSCHNextHqFdbkAlloc) ARGS((RgSchCellCb *cell,
            RgSchUlAlloc *alloc,uint8_t idx));
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
            uint8_t isAck));
   Void (*rgSCHUlSpsRelInd) ARGS((RgSchCellCb *cell, RgSchUeCb * ue,
            Bool isExplRel));

   Void (*rgSCHUlSpsActInd) ARGS((RgSchCellCb *cell, RgSchUeCb * ue,
            uint16_t sduSuze));

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
   uint32_t                   resType;
   PTR                   allctdBy;
   CmLteTimingInfo       timeStart;
   CmLteTimingInfo       timeEnd;
   uint32_t                   tSize;
   uint8_t                    freqStart;
   uint8_t                    freqEnd;
   uint32_t                   fSize;
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
   uint32_t            poolSz;
   RgSchIotRes    *resources;
   CmLListCp      resPool;
   uint8_t             maxFreqSz;   /*!< Max Number of RBs/SCs/CCEs per Subframe */
   uint32_t            deltaTime;   /*!< PDCCH/PUSCH/PDSCH Delta w.r.t current time */
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
   uint32_t         pdcchPoolSz,
   uint32_t         pdcchDelta,
   uint32_t         pdcchMaxFreqSz,
   uint32_t         pdschPoolSz,
   uint32_t         pdschDelta,
   uint32_t         pdschMaxFreqSz,
   uint32_t         puschPoolSz,
   uint32_t         puschDelta,
   uint32_t         puschMaxFreqSz
   ));

Void rgSCHUtlIotResMngmtDeinit ARGS((
   RgSchCellCb *cell
   ));

RgSchIotRes *rgSCHUtlIotAllocRes ARGS((
   RgSchIotResCb   *resCb,
   uint32_t             fStart,
   uint32_t             fSize,
   CmLteTimingInfo tStart,
   uint32_t             tSize,
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
   uint32_t             *fStart,
   uint32_t             *fEnd,
   CmLteTimingInfo tStart,
   uint32_t             tSize,
   uint32_t             numPrb,
   Bool            isRetx
   ));

S16 rgSCHUtlEmtcResPrcTti ARGS((
   RgSchCellCb *cell
   ));

S16 rgSCHUtlEmtcResMngmtInit ARGS((
   RgSchCellCb *cell,
   uint32_t         pdschPoolSz,
   uint32_t         pdschDelta,
   uint32_t         pdschMaxFreqSz,
   uint32_t         puschPoolSz,
   uint32_t         puschDelta,
   uint32_t         puschMaxFreqSz,
   uint32_t         pucchPoolSz,
   uint32_t         pucchDelta,
   uint32_t         pucchMaxFreqSz
   ));

Void rgSCHUtlEmtcResMngmtDeinit ARGS((
   RgSchCellCb *cell
   ));

#ifdef RG_5GTF
RgSchUlHqProcCb* rgSCHUhmGetUlProcByTime ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue, 
CmLteTimingInfo  frm
));
S16 rgSCHUhmGetAvlHqProc ARGS((
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
   uint16_t     min;          /*!< start ISRB Index */
   uint16_t     max;          /*!< end ISRB Index */
   uint16_t     peri;         /*!< SRS Periodicity in ms */
   uint16_t     offset;        /*!< Offset with ISRB value */
}RgSchUePCqiSrsSrCfgIdxTbl;

/**
  @brief   UE Specific SRS Periodicity   and Subframe Offset Configuration TDD */

/* Ref: Table 8.2-2: UE Specific SRS Periodicity */

typedef struct rgSchUeSrsCfgIdxTbl
{
   uint16_t     min;      /*!< start ISRB Index */
   uint16_t     max;      /*!< end ISRB Index */
   uint16_t     peri;    /*!< SRS Periodicity in ms */
   uint16_t     offset1;  /*!< Offset with ISRB value */
   uint16_t     offset2;  /*!< Offset with ISRB value */

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
   uint8_t      beamBitMask;    /*!<  Beam bit mask */
   //uint8_t      numUe;       /*!<  Ue present in Group */
}RgSchUeGrp;

/**
@brief 5gtf Cell Info 
*/
typedef struct rgSch5gtfCellCb
{
   RgSchUeGrp      ueGrp5gConf[MAX_5GTF_GROUP];   /*!<  Contains all the group configured */
   RgSchSfType     dynConfig[MAX_5GTF_SUBFRAME_INFO];  /*!<  Get config from config file as of now */
   uint8_t              numUes;
   uint8_t              uePerGrpPerTti; /*!< Num of UEs to be scheduled in a group per TTI */
   uint8_t              ueGrpPerTti;   /*!< Num of Groups to ne scheduled per Tti */
   uint8_t              numCCs;
   uint8_t              bwPerCC;
   uint8_t              cfi;
   uint8_t              totVrbg;
}RgSch5gtfCellCb;

#endif // 5GTF

/**
  @brief   UE Specific SRS Control Block */

typedef struct rgSchUeSrsCb
{
    RgSchUeUlSrsCfg     srsCfg;     /*!< SRS Specific configuration */
    uint16_t                 peri;       /*!< Periodicity */
    uint16_t                  offset;     /*!< subframe Offset */
    uint16_t                  nSrsTrIdx;
                                   /*!< Next Tranmission instance
                                       Referenence: 36.213 Section:8.2
                                      ((10*sfn +sf-sfOffset))/mod(peri))  */
    CmLList             srsLstEnt;      /*!< SRS List for next tranmission instance subframe */
    uint8_t                selectedAnt;    /*!< Selected Antenna for SRS Reception*/
    uint16_t                 srsDist;    /*!<  crnt Distance to get RI Transmission */
    Bool                srsRecpPrcsd; /*!< SRS Recpeption processed as part of PUCCH
                                          // or PUSCH processing in current TTI or not */

}RgSchUeSrsCb;

/**
@brief  Periodic CQI/PMI Reporting configuration using PUCCH */



/**
@brief Subband Size (k) and Bandwidth Parts (J) vs. DL sys BW */

typedef struct rgSchUeBwSubSzBwParts
{
   uint8_t rbStart;   /* RB Start */
   uint8_t rbEnd;     /* RB End */
   uint8_t subSize;   /* k */
   uint8_t bwParts;   /* J */

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
   uint16_t                  cqiPeri;       /*!< Periodicity */
   uint16_t                  cqiOffset;     /*!< CQI offset */
   uint16_t                  riOffset;      /*!< RI Offset */
   uint16_t                  riPeri;        /*!< RI periodicity */
   uint16_t                  nCqiTrIdx;
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

   uint16_t                nRiTrIdx;      /*!< Next Active Transmission instance for RI
                                          Transmission Instance Referenence:
                                          36.213 Section:8.2 */
   uint16_t                riDist;        /*!<  crnt Distance to get RI Transmission */
   uint16_t                h;             /*!< Precompute and store H */
   uint8_t                 riNumBits;     /*Precomputed value as it doesn't change
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
   uint8_t                 bpIdx;   /*Index of the Bandwidth Part*/
   uint8_t                 label;   /*Label L associated */
   uint8_t                 J;       /*Number of Bandwidth Parts*/

   CmLList            cqiLstEnt;/*!< List CQI UEs for next transmission
                                     instance */
   CmLList            riLstEnt; /*!< List RI UEs for next transmission
                                     instance */
   Bool               invalidateCqi; /*!< If TRUE, ignore the WB/SB CQI report*/
   uint8_t                 perRiVal;      /*!< Latest Periodic RI value reported */
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
   uint8_t 		     rgSchPucchRepNumCount;  /*!< CQI repetition count for CATM1 */
#endif
}RgSchUePCqiCb;



/**
@brief SR Configuration Index */

typedef struct rgSchUeSrCfgIdx
{
   uint16_t     sIsr;      /*!< Start ISR Index */
   uint16_t     eIsr;      /*!< End   ISR Index */
   uint16_t     peri;      /*!< SR Periodicity */
   uint16_t     offset;    /*!< offset */
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
    uint16_t                  offset;   /*!< SR offset */
    uint16_t                  peri;     /*!< SR periodicity */

    uint16_t                  nSrTrIdx;    /*!< Next Active Tranmission Instance Index ,
                                     Referenence: 36.213 Section:10.1 */

   /*
      ((10*sfn+floor(sf)-srOffset)Mod(srPeriodcity))
   */

#ifdef EMTC_ENABLE
   uint8_t 		     rgSchPucchRepNumCount;  /*!< SR repetition count for CATM1 */
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
 uint8_t                L; /*Unique Label through which positioning of Subbands
                        will be conveyed by the UE */
 uint8_t                N;  /*Total Bandwidth / Num of Subbands*/
 uint8_t                M; /*Number of Selected Subbands*/
 uint8_t                riNumBits;
 uint8_t                k; /*k - SB size. 36.213 Tables:7.2.1-3, 7.2.1-5*/
 uint8_t                cqiPmiSzR1;  /*CQIPMI Size for Rank =1*/
 uint8_t                cqiPmiSzRn1; /*CQIPMI Size for Rank > 1*/
 uint32_t               aCqiTrigWt;  /* Metric to track Aperiodic CQI Trigger occassion */ 
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
typedef uint8_t RgSchTddCellSpSrsSubfrmTbl[RGSCH_CELLSP_SRS_SF_CONFIGS][RGSCH_NUM_SUB_FRAMES];
#else
typedef uint8_t RgSchFddCellSpSrsSubfrmTbl[RGSCH_CELLSP_SRS_SF_CONFIGS][RGSCH_NUM_SUB_FRAMES];
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
   uint8_t switchPoints;     /*!< Number of DL-UL switch points */
   uint8_t numFrmHf1;        /*!< Number of subframes for half frame 1
                             Present for both 5ms and 10ms periodicity */
   uint8_t numFrmHf2;        /*!< Number of subframes for half frame 2
                             Present only for 5ms periodicity */
} RgSchTddSubfrmInfo;

/**
  * @brief
  * TDD DL Association Set information.
  */
typedef struct rgSchTddDlAscSetIdxK
{
   uint8_t numFdbkSubfrms;                        /*!< Number of Feedbacks for DL Subframes */
   uint8_t subfrmNum[RGSCH_NUM_SUB_FRAMES-1];     /*!< List of Subframe Number */
} RgSchTddDlAscSetIdxK;

/** @brief PRACH Information for a frequency resource. */
typedef struct rgrSchTddPrachInfo
{
   uint8_t     freqIdx;          /*!< Frequency Index */
   uint8_t     sfn;              /*!< Even/Odd/All Radio Frames */
   uint8_t     halfFrm;          /*!< First/Second Half Frame */
   uint8_t     ulStartSfIdx;     /*!< Uplink Start Subframe Index*/
} RgSchTddPrachInfo;

/** @brief PRACH resource Information for each of the
 *  frequency resources. */
typedef struct rgrSchTddPrachRscInfo
{
   uint8_t                 numRsc;                             /*!< Number of frequency resources*/
   RgSchTddPrachInfo  prachInfo[RGSCH_TDD_MAX_FREQ_RSRC]; /*!< PRACH Information */
} RgSchTddPrachRscInfo;

/**
  * @brief
  * TDD Special subframe configuration information.
  */
struct rgSchTddSplSubfrmInfo
{
   uint8_t   norDlDwPts;        /*!< DL Normal CP: DwPTS in Ts */
   uint8_t   norDlNorUpPts;     /*!< DL Normal CP: UL Normal CP:UpPTS in Ts */
   uint8_t   norDlExtUpPts;     /*!< DL Normal CP: UL Extended CP: UpPTS in Ts */
   uint8_t   extDlDwPts;        /*!< DL Extended CP: DwPTS in Ts */
   uint8_t   extDlNorUpPts;     /*!< DL Extended CP: UL Normal CP:UpPTS in Ts */
   uint8_t   extDlExtUpPts;     /*!< DL Extended CP: UL Extended CP: UpPTS in Ts */
};

/**
  * @brief
  * RACH response awaiting scheduling from the current time is
  * identified with sfn offset and subframe.
  */
typedef struct rgSchTddRachRspInfo
{
   uint8_t    sfnOffset;                    /*!< SFN offset with respect to
                                             expected RACH available for
                                             scheduling */
   uint8_t    numSubfrms;                   /* Number of subframes present */
   uint8_t    subframe[RGSCH_NUM_SUB_FRAMES];  /*!< List of Subframe numbers */
} RgSchTddRachRspInfo;

typedef RgSchTddRachRspInfo RgSchTddRachDelInfo;

/**
  * @brief
  * List of awaiting RACH responses for scheduling across radio frames.
  *
  */
typedef struct rgSchTddRachRspLst
{
   uint8_t                   numRadiofrms;   /*!< Number of radio frames */
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
   uint8_t    subframe;         /*!< Subframe number */
   uint8_t    sfnOffset;        /*!< SFN offset with respect to expected
                                UL data reception time */
} RgSchTddUlAscInfo;

/**
  * @brief
  * PUSCH information indicates the SFN offset and
  * subframe in which UL data is scheduled.
  */
typedef struct rgSchTddPuschOffInfo
{
   uint8_t    subframe;         /*!< Subframe number */
   uint8_t    sfnOffset;        /*!< SFN offset with respect to expected
                                UL data reception time */
} RgSchTddPuschOffInfo;

/**
  * @brief
  * PHICH information indicates the SFN offset and
  * subframe for which PHICH should be sent.
  */
struct rgSchTddPhichOffInfo
{
   uint8_t    numSubfrms;       /*!< Number of subframes */
/* ACC-TDD */
   uint8_t    subframe;         /*!< The Uplink Subframe number corresponding
                                to the phich */
   uint8_t    sfnOffset;        /*!< SFN offset with respect to expected
                                UL data reception time */
};

/**
  * @brief
  * DL feedback reception information indicates the SFN offset
  * and subframe at which feedback is expected.
  */
typedef struct rgSchTddDlFdbkInfo
{
   uint8_t    subframe;      /*!< Subframe number */
   uint8_t    sfnOffset;     /*!< SFN offset with respect to current
                             scheduled time */
   uint8_t    m;             /*!< m factor used in Downlink Association
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
   uint16_t   dwPts;      /*!< DwPTS in OFDM Symbol Duration */
   uint16_t   upPts;      /*!< UpPTS in OFDM Symbol Duration */
   Bool  isDlDataAllowed; /*!< To allow scheduling of DL data on 
                               special subframe */
} RgSchTddSplSubfrmCfg;

/**
  * @brief
  * ACK/NACK information to be used for ACK/NACK bundling mode.
  */
typedef struct rgSchTddANInfo
{
   uint16_t    sfn;           /*!< ACK/NACK is sent for PDU in this SFN */
   uint8_t     subframe;      /*!< ACK/NACK is sent for PDU in this subframe */
   uint8_t     dlDai;         /*!< Downlink Assignment Index for
                                                    UL-DL Configuration 1-6 */
   uint8_t     ulDai;         /*!< DAI for uplink */
   uint8_t     latestMIdx;    /*!< Last transmitted DL subframe 'm' index */
   uint8_t     n1ResTpcIdx;    /*!< N1 Res idx for scell assigned in TPC command */
   Bool   isSpsOccasion; /*!< To indicate the presence of SPS occasion */
#ifdef LTE_ADV
   uint8_t     wUlDai;        /*!< Max Ul dai in all the cells */
#endif
} RgSchTddANInfo;
#endif

/**
  * @brief
  * Information about one MCS entry.
  */
typedef struct rgSchUlIMcsInfo
{
   uint8_t qm;
   uint8_t iTbs;
} RgSchUlIMcsTbl[29];
RgSchUlIMcsTbl rgUlIMcsTbl;

typedef struct rgSchUeCatTbl
{
   uint32_t    maxUlBits;/*Maximum number of
                      bits of an UL-SCH
                      transport block
                      transmitted within a
                      TTI*/
   uint32_t    maxDlBits[4];/*Maximum number of 
                      bits of a DLSCH
                      transport block 
                      received within a TTI*/
/* correcting DL harq softbuffer limitation logic */
   uint32_t    maxSftChBits;/*Total number of soft channel bits*/
   Bool   ul64qamSup;/*Support for 64QAM in UL*/
/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */
   uint32_t    maxDlTbBits;/*Maximum number of DL-SCH 
                        transport block bits 
                        received within a TTI*/
   uint8_t     maxTxLyrs;/*Maximum number of supported 
                      layers for spatial multiplexing 
                      in DL*/
} RgSchUeCatTbl[CM_MAX_UE_CAT_SUPP + 1];
RgSchUeCatTbl rgUeCatTbl;

/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */
typedef uint32_t RgSchTbSzTbl[RGSCH_MAX_NUM_LYR_PERCW][RGSCH_NUM_ITBS][RGSCH_MAX_NUM_RB];

#ifdef LTE_TDD
typedef uint8_t RgSchRaPrmblToRaFrmTbl[RGSCH_MAX_TDD_RA_PREAMBLE_FMT+1];
#else
/* Added matrix 'rgRaPrmblToRaFrmTbl' for computation of RA
   sub-frames from preamble format */
typedef uint8_t RgSchRaPrmblToRaFrmTbl[RGSCH_MAX_RA_PREAMBLE_FMT+1];
#endif
RgSchRaPrmblToRaFrmTbl rgRaPrmblToRaFrmTbl;

uint8_t rgRvTable[4];

typedef struct rgDciFmt
{
   uint8_t            dciType;
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
   uint8_t             nCce;    /*!< CCE index */
   CmLteAggrLvl   aggrLvl; /*!< Aggregation level */
   TfuDciInfo     dci;     /*!< PDCCH format */
   uint16_t            rnti;    /*!< RNTI to who the PDCCH is allocated */
#if (defined (LTE_TDD))
   uint8_t                dlDai;          /*!< DAI associated with this PDCCH.
                                          THis is used for F1BCS resource calulcation */
#endif
  /* Added support for SPS*/
#ifdef LTEMAC_SPS
   CmLteTimingInfo relFbkTiming; /*!< Feebback timing information for release
                                   PDCCH */
   Bool           isSpsRnti; /*!< TRUE if rnti is SPS RNTI */
   uint16_t            crnti;    /*!< CRNTI to who the PDCCH is allocated */
#endif
   CmLList        lnk;     /*!< To link PDCCHs in a subframe */
#ifdef EMTC_ENABLE   
   Void           *emtcPdcch;
#endif   
   RgSchUeCb      *ue;     /*!< Pointer to the UE Control Block */
   RgSchPdcchSearchSpace pdcchSearchSpace; /*!< Search Space from this PDCCH allocated */
   uint8_t             dciNumOfBits; /*!< Size of DCI in bits */
} RgSchPdcch;

/**
  * @brief
  * PDCCH information for cell.
  */
typedef struct rgSchPdcchInfo {
   uint8_t             *map;    /*!< Bit map of PDCCHs */
   uint8_t             currCfi;       /*!< Number of CCEs */
   uint16_t            nCce;    /*!< Total CCEs */
   CmLListCp      pdcchs;  /*!< List of RgSchPdcch */
} RgSchPdcchInfo;

typedef struct rgSchPhich
{
   CmLList        lnk;        /*!< To link PHICHs in a subframe */
   uint8_t             hqFeedBack; /*!< Harq Feed Back */
   uint8_t             rbStart;    /*!< Starting RB */
   uint8_t             nDmrs;      /*!< 3 bits for DMRS cyclic shift */
   /* changes for passing iphich at TFU;*/
   Bool           isForMsg3;  /*! < Phich Ack/Nack conveyed for MSG 3 */
#ifdef LTE_TDD
   uint8_t             iPhich;     /*!< For determining phich group */
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
   uint16_t            tbSize;
} RgSchBcchTb;

typedef struct rgSchPcchTb
{
   RgSchPdcch     *pdcch;
   Buffer         *tb;
   uint16_t            tbSize;
} RgSchPcchTb;

typedef struct rgSchRaRspAlloc
{
   uint16_t            raRnti;
   uint32_t            tbSz;
   TknUInt8          backOffInd; /*!< Backoff index value */
   CmLListCp      raRspLst;   /*!< List of RaCbs */
   CmLListCp      contFreeUeLst; /*! List of HandOver or PdcchOrder UEs */
   RgSchPdcch     *pdcch;     /*!< NULLP if no Rsp allocation done for raRnti*/
}RgSchRaRspAlloc;

typedef struct rgSchBchTb
{
   Buffer         *tb;    /*!< BCH data for this frame */
   uint16_t            tbSize; /*!< Non-Zero if bch data is scheduled for this SF */
}RgSchBchTb;

/* Added support for SPS*/
#ifdef LTEMAC_SPS
/**
  * TODO: check compilation
  @brief Downlink Resource allocation type information. */
struct rgSchDlSfAllocInfo
{
  uint32_t raType0Mask;       /*!< RBG allocation mask for type 0*/
  uint32_t raType1Mask[RG_SCH_NUM_RATYPE1_32BIT_MASK]; /*!< RA Type 1
                                                    allocation mask */
  uint32_t raType1UsedRbs[RG_SCH_NUM_RATYPE1_32BIT_MASK];/*!< RA Type 1 Used RBs
                                                     per subset */
  uint32_t nxtRbgSubset;     /*!< Next RBG subset to be used for allocation */
  uint32_t raType2Mask[RG_SCH_NUM_RATYPE2_32BIT_MASK];
                        /*!< Mask for resource allocation type 2 */
};
#endif /* LTEMAC_SPS */

/* LTE_ADV_FLAG_REMOVED_START */
/**
  @brief RGR RB range for SFR */
typedef struct rgrPwrHiCCRange
{
   uint8_t   startRb;  /*<! Start RB for power high cell centre user */
   uint8_t   endRb;    /*<! End RB for power high cell centre user */
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
   uint8_t              CCPool1BwAvlbl;    /* Cell Centre Bw available for Pool1 */  
   uint8_t              CCPool2BwAvlbl;    /* Cell Centre Bw available for Pool2 */
   uint8_t              CEPoolBwAvlbl;     /* Cell Edge Bw available for CE Pool */
}RgSchSFRTotalPoolInfo;

typedef struct rgSchSFRPoolInfo
{
   /*Fixed RB Range of the Pool. Fixed by user configuration*/
   uint16_t               poolstartRB;
   uint16_t               poolendRB;								  
   uint16_t               bw;         /*!< Number of RBs in the pool */
   /*Dynamic Values */
   uint8_t                type2Start; /*!< Start RB for the next type 2 allocation */
   uint8_t                type2End;   /*!< End position of Type2 allocation with  in the bit mask */
   uint8_t                type0End;   /*!< End position of Type0 allocation with   in the bit mask */

   uint16_t               bwAlloced;  /*!< Number of RBs already allocated by DLFS */
   Bool              CCPool2Exists;  /*!< To check if the last pool in the RB allocation is a CC pool */  
   struct rgSchSFRPoolInfo * adjCCPool; /*!< The CC pool adjacent to this CE Pool SFR_FIX */
   RgrPwrHiCCRange   pwrHiCCRange; /*!< Power High Range which can be used by CC user based on RNTP info */
}RgSchSFRPoolInfo;
/* LTE_ADV_FLAG_REMOVED_END */
                
typedef struct rgSchPuschBwInfo
{
   uint8_t numSb;         /*!< PUSCH BW in subbands */
   uint8_t startRb;       /*!< Starting RB for PUSCH BW */
}RgSchPuschBwInfo; 
 
typedef struct rgSchDynCfiCb
{
   uint8_t               isDynCfiEnb;     /*!< Dynamic CFI feature Flag */
   uint8_t               maxCfi;          /*!< max possible CFI in the cell */ 
   uint8_t               switchOvrWinLen; /*!< Length of Switchover window */
   uint16_t              cceFailCnt;      /*!< Number of CCE allocation Failures in a 
                                           CCE failure sample period */            
   uint16_t              *cceFailSamples; /*!< Array holding samples of Avg number 
                                           of CCE allocation failures */
   uint16_t              cceFailSum;      /*!< Moving sum of the CCE faliures of 
                                          N samples */
   uint16_t              cfiStepUpTtiCnt; /*!< No.of TTIs to take decision for CFI 
                                          step Up */
   uint16_t              cceUsed;         /*!< Total CCEs used in current monitoring 
                                          interval */
   uint16_t              lowCceCnt;       /*!< Number of TTI in which CCEs used is 
                                          less than available CCEs in 
                                          lower CFI */
   uint16_t              cfiStepDownTtiCnt; /*!< No.of TTIs to take decision for CFI 
                                            step Down */
   uint32_t              cfiSwitches;     /*!< Total number of CFI switches */       
   uint32_t              cfiIncr;         /*!< Total number of CFI increments */
   uint32_t              cfiDecr;         /*!< Total number of CFI decrements */
                                     /*!< Total CCE per CFI */ 
   uint8_t               cfi2NCceTbl[RG_SCH_MAX_MPHICH][RG_SCH_CMN_MAX_CFI]; 
                                                               
   uint8_t               numFailSamples;    /*!< Number of CCE Allocation Failure 
                                            samples */
   uint16_t              failSamplePrd;     /*!< Failure Sample Period */
   uint16_t              ttiCnt;            /*!< TTI count to track monitoring period
                                            and sample period expiry */  
   RgSchPuschBwInfo bwInfo[RG_SCH_CMN_MAX_CFI]; /*!< PUSCH BW info */ 
   uint8_t               pdcchSfIdx;        /*!< Subframe Idx for CFI applying */
   uint8_t               prevCceFailIdx;   /*!< To maintain Previous Index of 
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
   uint16_t      totVrbgAvail;        /*!< Total VRBG available */
	//uint16_t      totRb;               /*!<  Total RB per Beam */
	uint16_t      totVrbgRequired;     /*!<  total Rbs requested for beam */
   uint16_t      totVrbgAllocated;    /*!<  total Rbs allocated for beam */
   uint16_t      vrbgStart;           /*!<  VRBG start for beam */
} RgSchSfBeamInfo;
#endif

typedef struct rgSchDlSf
{
   uint8_t                cceCnt;       /*!< Number of CCEs used in the subframe */
   Bool              isCceFailure; /*!< TRUE if PDCCH allocation is failed for 
                                        this subframe */
   uint8_t                dlUlBothCmplt; /*!< To track that DL and UL both scheduling
                                         is done */
   uint8_t                sfNum;      /*!< Number of the subframe */
   uint16_t               bw;         /*!< Number of RBs in the cell */
   uint16_t               bwAlloced;  /*!< Number of RBs already allocated by DLFS */
   uint16_t               bwAssigned; /*!< Number of RBs already allocated by scheduler */
/* LTE_ADV_FLAG_REMOVED_START */
   RgSchSFRTotalPoolInfo  sfrTotalPoolInfo; /* SFR Pool Info*/
   TknStrOSXL        rntpInfo;   /* RNTP Info for the complete subframe*/
/* LTE_ADV_FLAG_REMOVED_END */

   uint8_t                type2Start; /*!< Start RB for the next type 2 allocation */
   uint8_t                type2End;   /*!< End position of Type2 allocation with
                                      in the bit mask */
   uint8_t                type0End;   /*!< End position of Type0 allocation with
                                      in the bit mask */
   uint8_t                lstRbgDfct; /*!< The last RBG deficit RBs, Ex. if DLBW = 97,
                                  * RBGsz = 4, lstRbgDfct = 3 */
   Bool              txDone;     /*!< Flag to indicate if transmission is done*/
   uint32_t               numDlActvUes; /* 4UE_TTI_DELTA: num of active Ues */
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
   uint8_t                remUeCnt;   /*!< Remaining number of UEs that can be scheduled */
   Bool              schdAmbrNxt;/*!< Flag indicates to pick an AMBR LC after GBR LC */
   /*[ccpu00138609]-ADD- Counter to track the number of Msg4/DL CCCH UEs */ 
   uint8_t                schdCcchUe; /*!< Num of Msg4/DL CCCH UEs scheduled in 
                                      the DL Sf */ 
#ifdef LTE_TDD
   uint8_t                   nCce;       /*!< Number of CCEs */
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
   uint32_t               spsAllocdBw;   /*!< Number of RBs allocated for SPS */
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
   uint32_t      schedLcCount;          /*!< Num of LCs scheduled in this TTI */
   uint32_t      totalLcCntOfSlctdUes;  /*!< total Lc count of all UE's selected in 
                                    *   this TTI*/
   uint32_t      totPrbReq;             /*!< Sum of PRBs required by selected UEs
                                        in this subframe */
#ifdef BEST_EFFORT_2_UE_PER_TTI
  uint32_t       beTotPrbReq;
#endif
                                    
  /* CA dev Start */
   uint8_t      dlIdx;
  /* CA dev Start */
#ifdef LTE_ADV
   CmLListCp n1PucchResLst;        /*!< List for storing the used N3 resource */
   CmLListCp n3PucchResLst;        /*!< List for storing the used N1 resource */
#endif
#ifdef RG_5GTF
   RgSchSfBeamInfo  sfBeamInfo[MAX_5GTF_BEAMS];      /*!< Per info Beam per sf*/
   uint8_t               numGrpPerTti;     /*!< number of Group per TTI*/
	uint8_t               numUePerGrp;      /*!< number of UE per group in TTI*/
#endif
} RgSchDlSf;

/* Data Structures used for DL Allocations by Schedulers */
/**
  @brief Downlink Resource allocation type 0 information. */
typedef struct rgSchDlRbAllocRaType0
{
   uint8_t numDlAlloc;          /*!< Number of downlink allocations */
   uint32_t dlAllocBitMask;     /*!< Downlink allocations done for the UE */
} RgSchDlRbAllocRaType0;

/* Added support for SPS*/
#ifdef LTEMAC_SPS
/**
  @brief Downlink Resource allocation type 1 information. */
typedef struct rgSchDlRbAllocRaType1
{
   uint8_t numDlAlloc;          /*!< Number of downlink allocations */
   uint8_t shift;               /*!< Indicates if the shift is triggered in the
                                allocation */
   uint8_t rbgSubset;           /*!< RBG subset number selected for the allocation*/
   uint32_t dlAllocBitMask;     /*!< Downlink allocations done for the UE */
} RgSchDlRbAllocRaType1;
#endif /* LTEMAC_SPS */
/**
  @brief Downlink Resource allocation type 2 information. */
typedef struct rgSchDlRbAllocRaType2
{
   Bool        isLocal;       /*!< True if localised VRB */
   uint8_t          rbStart;       /*!< Starting RB */
   uint8_t          numRb;         /*!< Number of RBs */
} RgSchDlRbAllocRaType2;


typedef struct rgSchcmnDlGrnt
{
   uint16_t     schdTime;       /*!< Time at which Harq proc has been scheduled */
   uint8_t      rbStrt;         /*!< Starting RB of the allocation */
   uint8_t      numRb;          /*!< Number of RBs allocated */
   uint8_t      iMcs;           /*!< Index to the MCS */
   uint8_t      rv;             /*!< RV for HARQ (re)transmission */
   uint8_t      rvIdx;          /*!< RVIdx for HARQ(re)transmission */
#ifdef RG_5GTF   /* ToDo:: Anoop need to check for other fields required*/
   uint8_t      vrbgStart;
   uint8_t      numVrbg;
   uint16_t     rbAssign;
   uint8_t      xPDSCHRange;
   uint8_t      SCID;
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
   uint32_t           bytesReq;    /*!< Number of bytes required to be allocated
                                 for each TB: filled in by RR/MAX C/I/PFS */
   uint32_t           bytesAlloc;  /*!< Num of bytes allocated for each TB */
   uint8_t            iTbs;        /*!< Itbs for allocation for this allocation
                               * of TB */
   uint8_t            imcs;        /*!< Imcs for allocation for this allocation
                               * of TB */
   uint8_t            noLyr;       /*!< No. of SM layers for this TB transmission */
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
   uint8_t      precIdxInfo; /*!< Precoding index information stored */
   uint8_t      numTxLyrs;   /*!< Number of SM layers scheduled for Transmission */
}RgSchMimoAllocInfo;


/**
  @brief Downlink RB allocation details. */
typedef struct rgSchDlRbAlloc
{
   CmLteRnti rnti;          /*!< RNTI for allocation: used only for RARs and
                                 dedicated transmissions */
   uint8_t rbsReq;               /*!< Number of RBs required to be allocated:
                                 filled in by RR/MAX C/I/PFS */
   RgSchDlSf *dlSf;         /*!< DL sub-frame for which allocation is to be
                                 done: filled in by RR/MAX C/I/PFS */
   TfuDciFormat dciFormat;  /*!< DCI format for the allocation */
   uint8_t raType;               /*!< Resource allocation Type */
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
   uint8_t rbsAlloc;             /*!< Number of RBs allocated */
   uint8_t      numRapids;       /*!< Number of RAPIDs serviced within RAR*/
   /* Nprb indication at PHY for common Ch */
   uint8_t      nPrb;            /*!< NPRB column num. either 2 or 3 */
   uint32_t     raIndex;
   Bool    schdFirst;
   /* Add BI sub-header size to the tbSize requirement */
   uint8_t      biEstmt;
   RgSchMimoAllocInfo mimoAllocInfo; /*!< Mimo specific allocation params */
   RgSchDlTbAllocInfo tbInfo[2]; /*!< Allocation information for each TB. */

   /* Added support for SPS*/
#ifdef LTEMAC_SPS
   uint8_t      cqiForTx;     /*!< Assumed value of CQI at which transmission is
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
   uint8_t                  vrbgReq;
#endif
} RgSchDlRbAlloc;

/**
  * @brief
  * Uplink Grant Information per UE.
  */
typedef struct rgSchUlGrnt
{
   uint8_t           iMcs;       /*!< Original MCS index */
   uint8_t           iMcsCrnt;   /*!< Current MCS index of the grant */
   uint8_t           hop;        /*!< Hopping flag, set to 0 in this version */
   uint8_t           rbStart;    /*!< Start Resource block of allocation */
   uint8_t           numRb;      /*!< Number of resource blocks allocated */
   uint8_t           tpc;        /*!< TPC command for the uplink grant */
   uint8_t           nDmrs;      /*!< n1DMRS for the uplink grant */
   uint8_t           delayBit;   /*!< Delay bit, for msg3 grant, set to 1 */
   /* Added for Uplink Adaptive retransmission */
   Bool         isRtx;      /*!< To indicate if this grant is for a RETX */
   /* To include the length and ModOrder in DataRecp Req. */
   uint16_t          datSz;      /*!< Length of the Data */
   TfuModScheme modOdr;     /*!< Modulation order */
#ifdef RG_5GTF
   uint8_t           vrbgStart;
   uint8_t           numVrbg;
   uint16_t          rbAssign;
   uint8_t           xPUSCHRange;
   uint8_t           SCID;
   uint8_t           PMI;
   uint8_t           uciOnxPUSCH;
   uint8_t           hqProcId;
   Bool         ndi;
   TfuDciFormat dciFrmt;
   uint8_t           numLyr;
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
   uint8_t              numSb;     /*!< Number of subbands */
   uint32_t             tbSz;      /*!< Transmission Block Size */
   uint8_t              iMcs;      /*!< Original MCS index */
   RgSchUeCb       *ue;       /*!< UE assocated with allocation */
   Bool            forMsg3;   /*!< If allocation is for Msg3 */
   /* PHR handling for MSG3 */
   RgSchRaCb       *raCb;     /*!< Pointer to RaCb, if forMsg3 = TRUE */
#ifdef EMTC_ENABLE
   RgSchEmtcUlGrnt  emtcGrnt;
#endif
#ifdef RG_5GTF
   TfuDciFormat dciFrmt;
   uint8_t           numLyr;
   uint8_t           vrbgStart;
   uint8_t           numVrbg;
   TfuModScheme modOdr;     /*!< Modulation order */
#endif
};

/**
  * @brief
  * Hole information, pertaining to uplink bandwidth
 */
typedef struct rgSchUlHole
{
   uint8_t start;      /*!< Beginning of hole */
   uint8_t num;        /*!< Number of elements making up the hole */
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
   uint8_t              sbStart;         /*!< Subband start idx */
   uint8_t              numSb;           /*!< Number of subbands */
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
   uint8_t            maxAllocs;     /*!< Maximum possible allocations per subframe */
   RgSchUlAlloc  *firstFree;    /*!< First free index */
   RgSchUlAlloc  *allocs;       /*!< Array of 'maxAllocs' elems */
} RgSchUlAllocMem;

/**
  * @brief
  * Allocation information for a subframe
 */
typedef struct rgSchUlAllocDb
{
   uint8_t              count;      /*!< Number of allocations */
   RgSchUlAlloc    *first;     /*!< First allocation */
   RgSchUlAllocMem mem;        /*!< Alloc pool management */
} RgSchUlAllocDb;

/**
  * @brief
  * Hole free pool for a hole db
 */
typedef struct rgSchUlHoleMem
{
   uint8_t           maxHoles;     /*!< Maximum possible holes per subframe */
   RgSchUlHole  *firstFree;    /*!< First free index */
   RgSchUlHole  *holes;        /*!< Array of 'maxHoles' elems */
} RgSchUlHoleMem;

/**
  * @brief
  * Holes information for a subframe
  */
typedef struct rgSchUlHoleDb
{
   uint8_t             count;           /*!< Number of holes */
   RgSchUlHole    *first;       /*!< First hole */
   RgSchUlHoleMem mem;        /*!< Hole pool management */
} RgSchUlHoleDb;

/**
  * @brief
  * Uplink subframe information.
  */
typedef struct rgSchUlSf
{
   uint8_t               idx;      /*!< Identifier for uplink subframe (range: 0-7)
                                - maps to HARQ proc ID */
   CmLListCp        reTxLst;  /*!< Retransmission List*/
#ifdef LTE_TDD
   RgSchTddPuschOffInfo puschOffset; /*!< PUSCH offset information */
   uint8_t               ulSfIdx;  /*!< The Uplink subframe number 
                                   in the TDD frame. Range [2-9]
                                   Used only in TDD Cfg 0  */ 
#endif
   uint32_t              totPrb;    /*!< Total PRB used in this sub frame */
   /* ccpu00129725 -DEL- removed Platform flag */
   /* Fix:ccpu00120610 add a counter to keep track of remaining allocations */
   uint8_t               *allocCountRef; /*!< Allocation count in this
                                *  subframe, it is a reference to the actual
                                *  counter held in allocDb (no additional
                                *  logic needed to maintain this) */
   /* Added for Uplink Adaptive retransmission */
   RgSchUlAllocDb   *allocDb;  /*!< Allocation info */
   RgSchUlHoleDb    *holeDb;   /*!< Holes info */
   uint8_t               availSubbands; /*!< Number of total available subbands
                                    * for the current sub-frame */
   uint8_t   numACqiCount;        /*!< Used to Restrict 2 Aperiodic cqi per TTI*/ 
#ifdef RG_5GTF
   RgSchSfBeamInfo  sfBeamInfo[MAX_5GTF_BEAMS];      /*!< Per info Beam per sf*/
   uint8_t               numGrpPerTti;     /*!< number of Group per TTI*/
	uint8_t               numUePerGrp;      /*!< number of UE per group in TTI*/
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
   uint32_t             bo;        /*!< Bo in bytes */
   CmLteTimingInfo timeToTx;  /*!< Time at which the BO needs to be satisfied
                                   (data to be sent to PHY) */
#ifdef EMTC_ENABLE
   uint8_t              emtcDIReason;   /*!< Reason for DI message to send. */
   uint8_t              pnb;            /*!< Paging narrowBand on which Ue performs reception of paging*/
#endif
   CmLteTimingInfo maxTimeToTx;
   uint8_t              retxCnt;
   uint16_t             i;
   uint8_t              nPrb;      /*! NPRB column. Allowed values {2,3} */
   /* Corrected allocation for common channels */
   uint8_t              mcs;
} RgSchClcBoRpt;

/**
  * @brief
  * Logical channel control block for BCCH and PCCH channels.
  */
typedef struct rgSchClcDlLcCb
{
   uint8_t              lcId;                     /*!< Logical channel ID */
   Bool            si;
   CmLListCp       boLst;                    /*!< List of BOs reported (RgSchClcBoRpt) */
} RgSchClcDlLcCb;

/**
  * @brief
  * Transmission offset configuration for SRS.
  */
typedef struct rgSchSrsTxOffst
{
   uint8_t count;                       /*!< Number of offsets in the array */
   uint8_t offst[RGSCH_MAX_SRS_TX_OFFSET]; /*!< SRS TX offset value */
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
    uint8_t           srsSubFrameCfg;/*!< SRS subframe configuration index per cell.
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
   uint16_t rnti;                /*!< RNTI */
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
   uint16_t  rntiStart;         /*!< Start RNTI */
   uint16_t  maxRntis;          /*!< Maximum number of RNTIs managed by MAC */
   uint16_t  count;
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
   uint8_t              maxRaSize;                /*!< Number of RA-RNTIs stored
                                                  across radio frames */
   uint8_t              lstSize;                  /*!< list size */
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

   uint16_t     rttIndx;    /*!< Current Index into HARQ RTT Q */
   uint16_t     reTxIndx;   /*!< Current Index into Re-Tx Q */
   uint8_t      retxTmrReduction; /*!< Due to platform specific timing diff between
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
   uint8_t                ndi;          /*!< NDI */
   uint8_t                remTx;        /*!< Number of remaining transmissions */
   RgSchUlAlloc      *alloc;       /*!< Uplink allocation */
   /* Renamed rcvdDatInd to rcvdCrcInd */
   Bool              rcvdCrcInd;   /*!< Set to true when data rcvd, false when
                               decode failure. */
   uint8_t                rvIdx;        /*!< Redundancy version index */
   TknUInt8             rvIdxPhy;     /*!< Whatever is given by PHY. To be inspected in case
                               of NACK.Always initialised to 'not present' */
   CmLList           reTxLnk;      /*!< Retransmission List */
   RgSchUlRetxAlloc  reTxAlloc;    /*!< Retransmission allocation
                                        information */
   Bool              isRetx;       /*!< Awaiting retransmission */
   uint8_t                procId;       /*!< HARQ Process ID */
   uint8_t                ulSfIdx;      /*!< UL Subframe Index */
#ifdef LTE_TDD
   /* Changes for passing iPhich at TFU*/
   uint8_t                iPhich;       /*!< Needed to Calculate PHICH
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
   uint8_t           maxHqRetx;        /*!< Maximum number of harq
                                   * re-transmissions */
   uint8_t              numHqPrcs; /*!< Number of HARQ Processes */
#ifdef LTE_TDD
   RgSchUlHqProcCb *hqProcCb; /*!< Uplink harq process info */
#else
   RgSchUlHqProcCb hqProcCb[RGSCH_NUM_UL_HQ_PROC]; /*!< Uplink harq process info */
#endif
#ifdef LTE_L2_MEAS
   uint8_t          numBusyHqProcs;    /*!< Total Num of Hq procs in use */
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
   uint8_t               dedPream;    /*!< Dedicated RA Preamble received */
   uint8_t               preamGrpA;   /*!< Randomly selected preambles in low range */
   uint8_t               preamGrpB;   /*!< Randomly selected preambles in high range */
}RgSchRaPreambles;

/**
 * @brief strutcure to store Avergae PRB usage for a given Time period in
 * Downlink.
 * */
typedef struct rgSchAvgPrbDl
{
   Bool             cntActive;   /* Set to TRUE if counting activated */
   CmLteTimingInfo  startTime;   /*!< Start Time */
   uint8_t               timePrd;     /*!< Period For which meas to be done */
   uint32_t              prbCount;    /*!< PRB Count Updated every Per TTI */
} RgSchAvgPrbDl;

/**
 * @brief strutcure to store Avergae PRB usage for a given Time period in
 * Uplink .
 * */
typedef struct rgSchAvgPrbUl
{
   Bool             cntActive;   /*!< Set to TRUE if counting activated */
   CmLteTimingInfo  startTime;   /*!< Start Time */
   uint8_t               timePrd;     /*!< Period For which meas to be done */
   uint32_t              prbCount;    /*!< PRB Count Updated every Per TTI */
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
   uint16_t                sfnCycle;      /*<! Count of Num of SFN wraps */  
   uint32_t                dlTotalBw;     /*!< Total DL Bw between meas Req and Meas Send Req */
   uint32_t                ulTotalBw;     /*!< Total UL Bw between meas Req and Meas Send Req */
};

typedef struct rgSchTbCnt
{
   uint32_t    tbTransDlTotalCnt;  /*!< Total DL TB count */
   uint32_t    tbTransDlFaulty;    /*!< Total DL Faulty TB count */ 
   uint32_t    tbTransUlTotalCnt;  /*!< Total UL TB count */
   uint32_t    tbTransUlFaulty;    /*!< Total UL Faulty TB count */
}RgSchTbCnt;   
#endif /* LTE_L2_MEAS */
/** @brief
 *  Control block to store measurement details per QCI.
 *  */
struct rgSchQciCb
{
   uint8_t     qci;              /*!< QCI of the Logical Channel */
   uint32_t    dlPrbCount;       /*!< Cumulative Prb Count for this QCI */
   uint32_t    dlUeCount;        /*!<  Cumulative number of active UE's */
   uint32_t    dlTotal_UeCount;  /*!< Cummulative count added for every sampling
                              Occasion*/
   uint32_t    ulUeCount;        /*!<  Cumulative number of active UE's */
   uint32_t    ulTotal_UeCount;  /*!< Cummulative count added for every sampling
                              Occasion*/
};
struct rgSchLcgCb
{
   /* Right now not keeping associated logical channels, searching for
    * associated channels needed only during config */
   uint8_t    lcgId;  /*!< Group ID */
   Void  *sch;
#ifdef LTE_L2_MEAS
   uint8_t               numLch;                    /*!< Number fo LC's for this LCG*/
   RgSchUlLcCb      *lcArray[RGSCH_MAX_LC_PER_UE]; /*!< Dedicated Uplink logical
                                                 channel per LCG */
#endif /* LTE_L2_MEAS */
   /* Added support for SPS*/
#ifdef LTEMAC_SPS
   uint8_t    lcCnt;
   uint8_t    lcId[RGR_MAX_SPS_LC];
#endif
};

/**
  * @brief
  * Uplink Dedicated Logical channel control block.
  */
struct rgSchUlLcCb
{
   Bool          isValid; /*!< If this struct holds valid values*/
   uint8_t            lcId;    /*!< Logical channel ID */
   uint8_t            lcgArrIdx;    /*!< Index of this LC in the LCG Array*/
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
   uint8_t      rapId;         /*!< dedicated rapId */
   uint8_t      hop;           /*!< Hopping flag, set to 0 in this version */
   uint8_t      rbStart;       /*!< Start Resource block of allocation */
   uint8_t      numRb;         /*!< Number of resource blocks allocated */
   uint8_t      tpc;           /*!< TPC command for the uplink grant */
   uint8_t      iMcsCrnt;      /*!< Current MCS index of the grant */
   TknUInt16  ta;            /*!< Timing Adjustment */
   CmLList raRspLnk;      /*!< To link UE to RaRsp cont free list */
   /* Added support for SPS*/
   uint16_t     datSz;         /*!< Length of the Data */
   Bool    cqiReqBit;     /*!< Aperiodic CQI is requested or not */
} RgSchHoPoUeGrnt;

/**
  * @brief
  * Uplink SPS scheduling information per UE
  */
typedef struct  rgSchCmnSpsUlUeSchdInfo
{
   uint32_t             allocBwMask[RGSCH_SPS_ULBW_MASK_LEN]; /*!< Bitmask indicating the allocation info 
                                      for the UE. Bit set at position 'x'
                                      indicates subband 'x' is allocated */
   uint8_t              scaledCqi;    /*!< Assumed value of CQI for transmission */
   uint16_t             actvSfLstIdx; /*!< Index into cell-wide UL SPS sub-frame 
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
   uint8_t                      measGapMask[RGSCH_SPS_MG_MASK_LEN];  /*!< Indicates the UL sub-frames with 
                                               ongoing measurement gap. Mask is
                                               of size (80/8) */
   uint8_t                      state;         /*!< State of the UE: can be one of
                                               ACTV_REACTV_SENT (pdcch for 
                                               (re)activation sent),REL_SENT, 
                                               ULSPS_ACTV */
   Bool                    pdcchToRetx;   /*!< set to TRUE if DTX occurs at the
                                               (re)activated SPS occasion */
   RgSchCmnSpsUlUeSchdInfo ulSpsSchdInfo; /*!< Scheduled info for UL SPS 
                                               active UE */

   uint8_t                      spsOccIdx;         /*!< N value for nxt sps occasion */

   uint32_t                     packetSize;       /*!< Packet size for SPS - SPS allocation*/
   uint32_t                     spsSduSize;       /*!< SDU Size recvd on SPS Occasion*/
   uint32_t                     spsGrantSize;     /*!< Grant size for SPS allocation */

   CmLteTimingInfo         lastSpsDatRecvdTime; /*!< timing info on which the  
                                                     SPS data is recieved*/
   CmLListCp               *spsList; /*!< Pointer to the SPS list of which
                                      UE is a part */
   uint32_t                     actPdcchCrcFailCount; /*!< Num of consecutive CRC fails for 
                                                   SPS Activation PDCCH */
   uint32_t                     crcFailCntSpsOcc;     /*!< Consecutive CRC fail for SPS Occasion
                                         TX */
   uint32_t                     relPdcchSntCnt;     /*!< Num ber Rel PDCCH sent to UE*/
#ifdef RGSCH_SPS_STATS
   uint32_t                     crcFailStats;
   uint32_t                     crcPassStats;
   uint32_t                     totalBsr;
   uint32_t                     numBsrRpt;
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
   uint8_t               ulInactvMask; /*!< Bitmask indicating if UE is inactive for UL scheduling */
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
   uint8_t               explicitRelCnt; /*!< Number of subframes after sending
                                         release to consider it successful */
   uint16_t              spsPrdcty;      /*!< Shall store the SPS periodicity of the
                                         UE. Needed to round-off in case of
                                         TDD */
   CmLteTimingInfo   relPdcchSchdTime;     /*!< This field holds the scheduled time 
                                          of SPS rel PDCCH*/
#endif
#ifdef LTE_L2_MEAS
   uint32_t               nPrb;          /*!< Number of resource block allocated */
#endif
#ifdef TFU_UPGRADE
   uint8_t                betaHqOffst; /*!< Delta HARQ offset used in the case where HARQ
                             feedback is multiplexed with PUSCH data.  */
   uint8_t                betaCqiOffst;/*!< Beta offset for CQI[0-15]. Given by RRC*/
   uint8_t                betaRiOffst;/*!Beta offset for RI[0-15]. Given by RRC */
#endif
   uint32_t               cqiRiSz;   /*!<Size of Periodic/Aperiodic CQI or RI depending
                                     on the occassion */
   uint32_t               betaOffstVal; /*!< beta offset of CQI or RI depending 
                                     on the occassion*/
   uint32_t               maxBytesPerUePerTti;         /*!< Maximum bytes that can be allocated 
                                       *   in 1 scheduling instance per UE */
   uint32_t               minReqBytes;     /*!< The buffer amount of lcg0 */
   uint32_t               totalBsr;        /*!< Total BSR as reported by UE (in bytes) */
   uint32_t               nonLcg0Bs;       /*!< Total BSR for LCG1/LCG2/LCG3 */
   uint32_t               nonGbrLcgBs;     /*!< Total BSR as reported by UE (in bytes) for non-GBR LCG's*/
   uint32_t               effBsr;          /*!< BSR yet to be satisfied in allocations */

   uint32_t               cfgdAmbr;        /*!< Byte rate of UE Ambr per Refresh Cycle */
   uint32_t               effAmbr;         /*!< Remaining Bytes of Byte rate available in a refresh Cycle */
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
   uint32_t     totBytes;       /*!< Total bytes allocated */
   CmLList retxLnk;        /*!< To link hqP to retx Queues */
   uint8_t      rvIdx;          /*!< Rv Index */
} RgSchCcchSchdInfo;

/**
  * @brief
  * Downlink Grant Information per UE.
  */
typedef struct rgSchDlGrnt
{
   uint16_t     schdTime;       /*!< Time at which Harq proc has been scheduled */
   uint8_t      rbStrt;         /*!< Starting RB of the allocation */
   uint8_t      numRb;          /*!< Number of RBs allocated */
   uint8_t      iMcs;           /*!< Index to the MCS */
   uint8_t      rv;             /*!< RV for HARQ (re)transmission */
   uint8_t      rvIdx;          /*!< RVIdx for HARQ(re)transmission */
#ifdef RG_5GTF   /* ToDo:: Anoop need to check for other fields required*/
   uint8_t      vrbgStart;
   uint8_t      numVrbg;
   uint16_t     rbAssign;
   uint8_t      xPDSCHRange;
   uint8_t      SCID;
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
   uint8_t       lcId;       /*!< Logical channel ID */
   uint32_t      schdData;   /*!< Scheduled logical channel data */
} RgSchLchAllocInfo;

/** @brief This structure is part of the downlink HARQ process, this structure
 * encapsulates all the DRX related information.
 */
typedef struct rgSchDrxDlHqProcCb
{
   CmLList harqRTTEnt;   /*!< Entry into the HARQ RTT timer list */
   CmLList harqRetxEnt;   /*!< Entry into the harqRetxQ */

   uint16_t     rttIndx;    /*!< Current Index into HARQ RTT Q */
   uint16_t     reTxIndx;   /*!< Current Index into Re-Tx Q */
   uint8_t      retxTmrReduction; /*!< Due to platform specific timing diff between
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
   uint8_t                tbIdx;          /*!< Index of this TB (0/1). Initialized
                                          at HqEnt allocation */
   uint32_t               tbSz;           /*!< TB size scheduled for this TB */
   uint8_t                txCntr;         /*!< Transmission counter */
   uint8_t                ndi;            /*!< New Data Indicator 0 or 1*/
   TknUInt8             schdTa;      /*!< TA scheduled to be sent */
   /* CR timer implementation changes*/
   uint8_t                contResCe;
   uint8_t                numLch;         /*!< No. of DL Logical Channels scheduled
                                          for this TB */
   CmLteTimingInfo   timingInfo;     /*!< This field holds the scheduled time */
   RgSchLchAllocInfo lchSchdDataArr[RGSCH_MAX_NUM_DED_LC];
   RgSchLchAllocInfo *lchSchdData;
   RgSchCcchSchdInfo ccchSchdInfo;/*!< CCCH TX/RETX scheduler Info */
   uint8_t                fbkRepCntr;     /*!< Counter to ANRep Feedback */
   uint8_t                fbkRecpRepCntr; /*!< Counter to ANRep Feedback */
   CmLList           anRepLnk[6];    /*!< Links for ANRep we need
                                          these many links to
                                          simultaneously hold the
                                          hqProcs in multiple
                                          subframes. */
   RgSchDlSf         *crntSubfrm[6]; /*!< Pointer to subframes */
   uint8_t          ackCount;   /*!< Counter for ACKs */
   uint8_t          isAckNackDtx; /* ACK or NACK or DTX*/
   uint8_t          nackCount;  /* NACK Counter */
   uint8_t          dtxCount;   /* DTX Counter */
   RgSchDlGrnt       dlGrnt;      /*!< Scheduler grant */
   Bool              taSnt;       /*!< TA sent or not */
#ifdef LTE_TDD
   CmLteTimingInfo   fdbkTime;    /*!< Expected feedback time */
   uint8_t                m;           /*!< Subframe Order Index within the subframe
                                       downlink association index K */
   RgSchTddSfType    sfType;      /*!< Identifies the first Tx subframe type */ 
   uint8_t                pucchFdbkIdx;/*!< DL subframe idx for which fdbk is expected */
   uint8_t                dai;          /*!< DAI associated with this TB.
                                          THis is used for F1BCS resource calulcation */
#ifdef XEON_TDD_SPCL
   uint8_t                initTxNumRbs;/*!< Number of Rbs Allocated in First TX */
#endif

#endif
   /* 3.1 MIMO */
   uint8_t                numLyrs;     /*!< Number of layers using which this TB is
                                       being transmitted */
   RgSchHqTbState    state;       /*!< State of this Trasport Block */
   RgSchDlHqProcCb   *hqP;        /*!< Reference to the TB container */
   /* Freeing up the HARQ proc blocked for
    * indefinite time in case of Retx */
   uint8_t                cntrRetxAllocFail; /*!< Number of times allocation failed
                                            for the retransmission of this TB */
#ifdef LTE_ADV
   TknUInt8             schdSCellActCe;   /* !< SCell Act values and whether
                                          scheduled or not */
#endif
  uint32_t                firstSchedTime; /*!< First time when the TB was scheduled */
  uint8_t                 minRlcReordrTmr; /*!< Min RLC reorder timer of all LCs
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
   uint8_t                txCntr;      /*!< Transmission counter */
#endif
   uint8_t                procId;      /*!< Harq process ID */
   Void              *sch;        /*!< Cmn sched Hq control block */
   /* 3.1 MIMO */
   RgSchDlHqTbCb     tbInfo[2];   /*!< TB related information for 1+1(if any) TBs */
   RgSchDrxDlHqProcCb   drxCb;   /*!< DRX control block */
#ifdef TFU_UPGRADE
   uint8_t                tbCnt;     /*!< Count of the number TBs being sent with a
                                  Reception Requesti */
#endif
#ifdef LTEMAC_SPS
   TknUInt16       spsN1PucchRes;   /*!< N1Pucch resource for HARQ process at SPS
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
   uint8_t        tpc;           /*!< TPC command to be used for Secondary cell
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
   uint8_t                dlDai;        /*!< DL DAI assosciated with this harq proc */
   uint8_t                ulDai;        /*!< UL DAI assosciated with this harq proc */
#ifdef BRDCM
   /*CA Dev End*/
   uint32_t               isPuschFdbk;
#endif
   Void *         laaCb;
   CmLListCp   *hqPLst;    /*!< pointer to either inUse of free list of hqE */
   TfuDciFormat prevDciFormat; /*!< Previous alloction DCI format used for ERR Ind prcessing */

   /* LAA DBG Only */
   uint32_t tbSizeAtEstimate[2];
   uint32_t tbSizeAtFnlz[2];
   uint32_t tbSizeOfMvdTb[2];
   uint32_t itbsAtEstimate[2];
   uint32_t prbAtEstimate;
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
   uint8_t           maxHqTx;   /*!< Maximum number of harq transmissions */
   RgSchDlHqProcCb *msg4Proc; /*!< Points to MSG4 HARQ process */
#ifdef RGR_V1
   /* CR timer changes*/
   RgSchDlHqProcCb *ccchSduProc; /*!< Points to CCCH SDU HARQ process
              to identify feedback for CCCH SDU
              transmissions done without cont res CE*/
#endif
   uint8_t              numHqPrcs; /*!< Number of HARQ Processes */
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
  uint16_t       cfgTaTmr; /*!< Configured TA timer value */
  uint8_t        ta;       /*!< TA value for UE */
  uint8_t        numRemSf; /*!< Number of subframes left to apply TA */
  RgSchTaState state;    /*!< Indicates HARQ state for TA */
  /*rg003.301[ccpu00121813] ADD added new var*/
  Bool     outStndngTa;  /*!< if new TA is rcvd before it gets
                               applied at UE*/
  uint8_t       outStndngTaval; /*!< outstanding TA val */
} RgSchUeDlTaCb;

/**
  * @brief
  * Downlink Dedicated Logical channel control block.
  */
struct rgSchDlLcCb
{
   uint8_t            lcId;    /*!< Logical channel ID */
   uint32_t           bo;
   RgSchUeCb     *ue;     /* UE to which this LC belongs to */
   /* Not validating DlQos for DCCH */
   CmLteLcType   lcType;   /* Logical channel Type*/
   RgSchQciCb    *qciCb;  /*!< Pointer to ACI Control Block */
   /* Added support for SPS*/
#ifdef LTEMAC_SPS
   RgrLchSpsCfg   dlLcSpsCfg;  /*!< SPS configuration for DL logical channel */
#endif
#ifdef CCPU_OPT
   uint16_t           estRlcHdrSz;  /*!< Estimated RLC header size */
   Bool          staPduPrsnt;  /*!< Indicate the status pdu present or not*/
#endif
   uint32_t           staPduBo;     /*!< Indicate the Status PDU BO.
                                    This amount is already included in original BO */
   Void          *sch;        /*!< Holds Scheduler Info pointer */
   uint32_t           prbsReqd;    /*!< PRBs reqd in DL for each UE in MUE scenario*/
   CmLList       schdLnk;     /*!< Link to ueCb inserted in toBeSchdList in DL*/
   uint32_t           oldestSduArrTime; /*!< oldest SDU arrival time for this LC */
   CmLList       lcFlowCntrlEnt; /*!<link to Lc in flow Cntrl lst */
   CmLList       lcPdbFbkLnk;  /*!<link to LC in LC flow contrl monitoring lst */
   Bool          setMaxUlPrio; /*!<set if Poll bit is sent in the PDU */
   Bool          setMaxDlPrio; /*!<set if there is a RLC status PDU in DL */
   Void*         laaCb;

};
/*f1b_Sprint3*/

typedef struct rgSchN1PucchResCb
{
   uint16_t n1PucchIdx;  /* !< N1 resource */ 
   CmLList n1Lnk;  /* !< Link for adding into dlsf N1 used list */
}RgSchN1PucchResCb;
typedef struct rgSchN3PucchResCb
{
   uint16_t n3PucchIdx;  /* !< N3 resource */ 
   CmLList n3Lnk;   /* !< Link for adding into dlsf N3 used list */
   uint8_t sCellIdx;      /* !< Allocated by which scell */ 
}RgSchN3PucchResCb;

typedef struct rgSchSCellN3ResCb
{
  uint8_t                 antP0N3ResCount; /* !< Number of N3 res for antenna port 0*/
  uint8_t                 antP1N3ResCount;  /* !< Number of N3 res for antenna port 0*/
  RgSchN3PucchResCb  antP0N3Res[RG_SCH_MAX_NUM_N3PUCCH_PER_UE]; /* !< Antenna Port 0 N3 resources*/
  RgSchN3PucchResCb  antP1N3Res[RG_SCH_MAX_NUM_N3PUCCH_PER_UE]; /* !< Antenna Port 1 N3 resources*/
}RgSchSCellN3ResCb;

typedef struct rgSchSCellN1ResCb
{
  uint8_t                 cw1N1ResCount; /* !< Number of N1 res for CW 1*/
  uint8_t                 cw2N1ResCount;  /* !< Number of N1 res for CW 2*/
  RgSchN1PucchResCb  cw1N1Res[RG_SCH_MAX_NUM_N1PUCCH_PER_UE]; /* !< CW1 N1 resources*/
  RgSchN1PucchResCb  cw2N1Res[RG_SCH_MAX_NUM_N1PUCCH_PER_UE];  /* !< CW2 N1 resources*/
}RgSchSCellN1ResCb;
/*CA Dev Start*/
#ifdef LTE_ADV
typedef struct rgSchN3PucchRes
{
   uint16_t n3PucchIdx;  /* !< N3 resource */
   uint8_t  tpcIdx;
}RgSchN3PucchRes;
#endif
typedef struct rgSchDlHqInfo
{
/*f1b_Sprint3*/
#ifdef LTE_TDD
   uint8_t                     numOfCells; /*!<This will be used to store number of unique cells in
                                        this subframe*/
   Bool                   isSCellPdschPres; /*!< Flag to indicate whether any PDSCH transmission
                                             present in secondary cell. Used for CSI/HARQ collision
                                              */
#endif
   Bool                   isPuschHarqRecpPres;/*!< Flag to indicate pusch recp req is pres or not */
   RgrSchFrmt1b3TypEnum   uciFrmtTyp; /*!< Store value of format1b or format3.
                                       */
   uint8_t                     totalTbCnt; /*!<  This will be used to store total number of TBs 
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
   uint8_t   gapPrd;             /*!< Gap period 40ms/80ms */
   uint8_t   gapOffst;           /*!< Gap offset - Vaue is 0 to 1*/
} RgUeMeasGapCfg;

/**
  @brief Measurement Gap related information per UE. */
typedef struct rgSchUeMeasGapCb
{
   Bool              isMesGapEnabled;/*!< TRUE if Measurement gap is enabled for this UE */
   uint8_t                isMeasuring;  /*!< Set to TRUE during measurement gap */
   uint8_t                gapPrd;     /*!< Measurement gap period configuration for the UE */
   uint8_t                gapOffst;   /*!< Measurement gap offset for the UE */
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
   uint8_t          isAckNakRep;  /*!< Set to TRUE during ACK-NACK repetition prd */
   uint8_t          cfgRepCnt;         /*!< Configured value for the repetition counter */
   uint8_t          repCntr;           /*!< Actual repetition counter */
   uint16_t         pucchRes;          /*!< PUCCH resource for repetition */
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
   TknUInt32             doa;          /*!< DOA indicator for this UE */
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
   uint8_t                            wideBCqiCw0;    /*!< Length of Wideband CQI Codeword 0 */
   uint8_t                            totLenSbDiffCqiCw0;    /*!< Length of SubBand Differential CQI Codeword 0 */
   uint8_t                            r1WideBCqiCw1;    /*!< Length of Wideband CQI Codeword 1 for Rank =1*/
   uint8_t                            r1TotLenSbDiffCqiCw1;    /*!< Length of SubBand Differential CQI Codeword 1 for Rank = 1*/
   uint8_t                            rg1WideBCqiCw1;    /*!< Length of Wideband CQI Codeword 1 for Rank > 1*/
   uint8_t                            rg1TotLenSbDiffCqiCw1;    /*!< Length of SubBand Differential CQI Codeword 1 for Rank > 1*/
   uint8_t                            r1PmiBitLen;   /*!< Length of PMI Bits for Rank = 1*/
   uint8_t                            rg1PmiBitLen;   /*!< Length of PMI Bits for Rank > 1*/
} RgSchCqiRawPuschMode31;

/** @brief This structure that stores the length of Bits that
*     will be received over PUSCH for Aperiodic Mode 3-0.
*/
typedef struct rgSchCqiRawPuschMode30
{
   uint8_t                     wideBCqiCw;    /*!< Length of Wideband CQI */
   uint8_t                     totLenSbDiffCqi;    /*!< Length of SubBand Differential CQI */
} RgSchCqiRawPuschMode30;

/** @brief This structure that stores the length of Bits that
*     will be received over PUSCH for Aperiodic Mode 2-2.
*/
typedef struct rgSchCqiRawPuschMode22
{
   uint8_t                            wideBCqiCw0;    /*!< Length of Wideband CQI Codeword 0 */
   uint8_t                            sBDiffCqiCw0;    /*!< Length of SubBand Differential CQI Codeword 0 */
   uint8_t                            r1WideBCqiCw1;    /*!< Length of Wideband CQI Codeword 1  for Rank =1 */
   uint8_t                            r1SbDiffCqiCw1;    /*!< Length of SubBand Differential CQI Codeword 1 for Rank =1*/
   uint8_t                            rg1WideBCqiCw1;    /*!< Length of Wideband CQI Codeword 1  for Rank > 1*/
   uint8_t                            rg1SbDiffCqiCw1;    /*!< Length of SubBand Differential CQI Codeword 1 for Rank >1*/
   uint8_t                           posOfM;   /*!< Position of M selected SubBands */
   uint8_t                           r1PmiBitLen;   /*!< Length of PMI Bits for Rank =1*/
   uint8_t                           rg1PmiBitLen;   /*!< Length of PMI Bits for Rank >1*/
} RgSchCqiRawPuschMode22;

/** @brief This structure that stores the length of Bits that
*     will be received over PUSCH for Aperiodic Mode 2-0.
*/
typedef struct rgSchCqiRawPuschMode20
{
   uint8_t                     wideBCqiCw;    /*!< Length of Wideband CQI */
   uint8_t                     subBandDiffCqi;    /*!< Length of SubBand Differential CQI */
   uint8_t                     posOfM;   /*!< Position of M selected SubBands */
} RgSchCqiRawPuschMode20;

/** @brief This structure that stores the length of Bits that
*     will be received over PUSCH for Aperiodic Mode 1-2.
*/
typedef struct rgSchCqiRawPuschMode12
{
   uint8_t                     wideBCqiCw0;    /*!< Length of Wideband CQI Codeword 0 */
   uint8_t                     r1WideBCqiCw1;    /*!< Length of Wideband CQI Codeword 1 for Rank =1*/
   uint8_t                     rg1WideBCqiCw1;    /*!< Length of Wideband CQI Codeword for Rank > 1 */
   uint8_t                     r1TotalPmiBitLen;   /*!< Aggregate length of PMI Bits for Rank =1 */
   uint8_t                     rg1TotalPmiBitLen;  /*!< Aggregate length of PMI Bits for Rank > 1 */
} RgSchCqiRawPuschMode12;


/** @brief This structure that stores the length of Bits that
*     will be received over PUSCH.
*/
typedef struct rgSchDlCqiRawPusch
{
   TfuDlCqiPuschMode      mode;          /*!< PUSCH CQI mode */
   TknUInt8                  ri;            /*!< Rank Indicator for TM 3,4 */
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
   uint8_t                   sCellIdx;        /*!< Serving cell idx of the cell for
                                              this cqi info*/
   RgSchDlCqiRawPusch   puschRawCqiInfo; /*!< Raw CQI Bit Width for  PUSCH */
} RgSchPuschRawCqiInfoPerCell;

typedef struct rgSchPuschRawCqiInfoForSCells 
{
   uint8_t                          numOfCells;   /* Num of cells for which Apcqi is comming*/
   RgSchPuschRawCqiInfoPerCell cqiBitWidth[CM_LTE_MAX_CELLS];
} RgSchPuschRawCqiInfoForSCells;

typedef struct rgSchPucchRawCqiInfoPerCell
{
  uint8_t sCellIdx;                          /*!< Serving cell idx of the cell for
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
   uint8_t                    sCellIdx;   /*!< Serving Cell Index  */
   uint16_t                   sCellId;    /*!< Secondary Cell Id */
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
   uint8_t                   nextFreeANIdx; /*!< Next Available ANInfo Index */

#endif
   uint8_t                    reqForCqi;     /*!< set to True if Aperiodic CQI from UE is required */
#ifdef TFU_UPGRADE
   RgSchUeACqiCb         acqiCb;        /* ! < Aperiodic CQI Report Control Block*/
   RgSchUePCqiCb         cqiCb;      /*!< Periodic CQI PMI RI Control Block */
   TknUInt8      pA;                     /* PA value configured by RRM
                                         ref: RRC 36.331, 6.3.2, PDSCH-Config*/
   /* RgSchUeRawCqiBitWidthInfo  rawCqiBitW[MAX_CQI_RI_RPT_BUFF]; */
#endif
   uint8_t                   cqiRiWritIdx; /*!< write index to be used whenever CQI/RI reception
                             request is being filled*/
   uint8_t                   cqiRiReadIdx; /*!< Read index to be used whenevr CQI/RI indication 
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
   uint8_t             dlInactvMask;  /*!< Bitmask indicating if UE is inactive for DL scheduling */
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
   uint8_t numHqDlSfInfo;
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
   uint32_t            ambrCfgd; /*!< UE's configured AMBR scaled to Refresh Cycle */ 
   /* CA dev End */
   /* Added to restrict max TB Bits in case of more than one CCs for a UE */
   uint32_t            aggTbBits;        /*!< Aggregated Transport Block Bits this UE can receive per TTI*/
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
  uint16_t             sfn;                   /*!< Store the sfn for updated state
                                              Default Value= 0xffff
                                          */
  uint8_t              subFrmDir;
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
   uint16_t      onDurTmrLen;   /*!< Length of onDuration Timer [1 - 200]. */
   uint16_t      drxStartOffset;   /*!< Value of the DRX Starting Offset [0 - 2559]. */
   S16      onDurExpDistance;   /*!< Keeps track of actual distance */

   uint16_t       drxRetransTmrLen; /*!< Length of DRX Retransmission timer [1 - 33].*/

   uint16_t      inactvtyTmrLen;   /*!< Length of drx-Inactivity Timer [1 - 2560]. */
   S16      drxInactDistance;   /*!< Keeps track of actual distance */

   Bool     isLongCycle;
   uint16_t      longDrxCycle;   /*!< Value of the DRX long cycle [10 - 2560]. */

   Bool     isShortCycleCfgd;   /*!< TRUE if short cycle is enabled. */
   uint8_t       shortCycleTmrLen;   /*!< Value of DRX short cycle Timer [1-16]. */
   uint16_t      shortDrxCycle;   /*!< Value of the DRX short cycle [2 - 640]. */
   S16      drxShortCycleDistance;   /*!< Keeps track of actual distance */


   CmLList  onDurationEnt;   /*!< Entry into the OnDuration List. */
   CmLList  onDurationExpEnt;   /*!< Entry into the onDuration Expiry List. */
   CmLList  inActvTmrEnt;   /*!< Entry into the inactivity Timer list.  */
   CmLList  shortCycleEnt;   /*!< Entry into HARQ Retransmission list. */

   S16      distance;   /*!< Keeps track of actual distance */

/* The following elements track current indices into the drxQ present at the
 * cell level. These indicies help in fast deletion in case of UE Delete,
 * otherwise it might have required a linear search. */
   uint16_t      onDurIndx;   /*!< The current index for onDuration Queue. */
   uint16_t      onDurExpIndx;   /*!< The current index for onDuration Queue. */
   uint16_t      drxInactvIndx;   /*!< The current index for drx-InactityTmr Queue. */
   uint16_t      shortCycleIndx;   /*!< The current index for Short Cycle Queue. */

   uint8_t       shortCycleTmr;   /*!< Counter to keep track of Short DRX Cycle. */
   uint32_t      drxDlInactvMask;  /*!< Downlink Mask to track InActivity */
   uint32_t      drxUlInactvMask;  /*!< Uplink Mask to track InActivity */
   uint32_t      drxDlInactvMaskPerCell[CM_LTE_MAX_CELLS]; /*!< Downlink Mask to track InActivity per cell */
   uint32_t      drxUlInactvMaskPerCell[CM_LTE_MAX_CELLS]; /*!< Uplink Mask to track InActivity per cell */
} RgSchDrxUeCb;



/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
typedef struct RgSchCqiInfo
{
   uint8_t        cqiCount;                  /* To keep track of CQI reports
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
   uint8_t cmnSize[TFU_DCI_FORMAT_6_2+1]; /*!< DCI 0/1A/6-0A/6-1A final size in common Search Space and scrambled by C-RNTI */
   uint8_t dedSize[TFU_DCI_FORMAT_6_2+1]; /*!< DCI 0/1/1A/2/2A/6-0A/6-1A final size in UE Search Space and scrambled by C-RNTI */
#else   
   uint8_t cmnSize[TFU_DCI_FORMAT_1A+1]; /*!< DCI 0/1A final size in common Search Space and scrambled by C-RNTI */
   uint8_t dedSize[TFU_DCI_FORMAT_2A+1]; /*!< DCI 0/1/1A/2/2A final size in UE Search Space and scrambled by C-RNTI */
#endif   
   uint8_t noUlCcSize[TFU_DCI_FORMAT_2A+1]; /*!< DCI 1/1A final size in UE/Common Search Space when the cell 
                                        is SCell and no corresponding UL CC configured */
} RgSchUeDciSize;

#ifdef RG_PFS_STATS
typedef struct rgSchPerRefreshStats {
   uint32_t  remGbr;
   uint32_t  remDeltaMbr;
   uint32_t  totByteSchd;
   uint32_t  lcSchdOcc;
}RgSchPerRefreshStats;

typedef struct rgSchLcStats
{
   //Bool           isLcCntSet;
   //uint32_t            schdOccCnt; 
   uint32_t            ueSchdOcc[CM_LTE_MAX_CELLS];
   uint32_t            gbrSatisfiedCnt;
   Bool           ignoredFirstRefresh;
   uint32_t            gbrRefreshCycleCnt;
   uint32_t            totGbrBytesSchd;
   uint32_t            totMbrBytesSchd;
   uint32_t            achvdFracDataRate;
   Bool           isRecvdBo;
#define RGSCH_NUM_STATS_PER_REFRESH 50
   uint32_t            startIdx;
   uint32_t            lastIdx;
   uint32_t            favCellCnt[CM_LTE_MAX_CELLS];
   RgSchPerRefreshStats perRefresh[RGSCH_NUM_STATS_PER_REFRESH];
}RgSchLcStats;

typedef struct rgSchCqiStats
{
   uint32_t            totalCqiOcc;
   uint32_t            avgCqi;
}RgSchCqiStats;

typedef struct rgSchPfsStats
{
   RgSchLcStats   lcStats[RGSCH_MAX_LC_PER_UE];
   //uint32_t            ueSchdOcc[CM_LTE_MAX_CELLS];
   uint32_t            refreshCycleCnt;
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
   uint8_t              grpId;      // Group Id 
   uint8_t              BeamId;     // Beam Id of UE
   uint8_t              numCC;      // num Carrier configured for UE
   uint8_t              mcs;        // MCS configured
   uint8_t              maxPrb;     // Max Prb configured for UE 
   CmLteTimingInfo nxtCqiRiOccn;   /*!< Next CQI RI Occn */
   uint16_t             cqiRiPer;   /*!< CQI RI periodicity in SFs */
   uint8_t              rank;       /*!< Latest Rank Report from UE */
}RgSch5gtfUeCb;
#endif

/**
  * @brief
  * UE control block for UE specific information.
  */
struct rgSchUeCb
{
   uint32_t                  totalBo; /*!<Sum of DL BO across all logical channels*/
   uint32_t                  totalPrbRequired;/*!<Num of PRB reqd to satisfy DL totlBo*/
   /* Added support for SPS*/
#ifdef LTEMAC_SPS
   CmHashListEnt        spsUeLstEnt;  /*!< Hash List entity for UE List */
   uint32_t                  spsOccasionCnt; /*!< Total number of SPS occasion cnt*/
#endif
#ifdef CQI_CONFBITMASK_DROP
   uint8_t                   cqiConfBitMask;
   uint8_t                   prevCqi; 
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
   uint8_t                   relPdcchValm;   /*!< 'm' for relPdcch */
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
   uint32_t                  y[RGSCH_NUM_SUB_FRAMES]; /*!< 'Y' values calculated 
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
      uint32_t          bo;            /*!< Buffer occupancy for
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
  
   uint8_t     validTxAnt;  /*! < Tx Antenna selected after computing the CQI among two Antennas*/

   uint8_t     cqiRiWritIdx; /*!< write index to be used whenever CQI/RI reception
                             request is being filled*/
   uint8_t     cqiRiReadIdx; /*!< Read index to be used whenevr CQI/RI indication 
                             is recieved from PHY*/
   RgSchUeRawCqiBitWidthInfo  rawCqiBitW[MAX_CQI_RI_RPT_BUFF];
   uint8_t     initNumRbs; /* No. of RBs allocated for UL Data New Transmission */
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
   uint8_t qciActiveLCs[LRG_MAX_QCI]; /* This structure has number of active LCs per 
                                    Qci for this UE */ /* L2_COUNTERS */
   uint16_t ulActiveLCs;     /* This is a bitmask - Each bit representing a QCI
                           LSB - QCI 1 ... When bit is set, UE has an active
                           LC for that QCI */
   uint16_t lastDatIndLCs;     /* This is a bitmask - Each bit representing a QCI
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
   uint64_t             absTimeBo;
   uint64_t             absTimeSched;
#endif
   uint8_t           refreshOffset; /*!< UE referesh offset */
   uint8_t              csgMmbrSta;        /*!< CSG Membership status configured */
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
   TknUInt32          sCellDeactTmrVal; /* !< SCell Deactivation Timer Value */
   uint8_t              f1bCsAVal; /* !< A value mentioned in table 10.12.2.1-1 of 36.213. This will tell
                            the max number of tbs possible based on TM mode of each configured 
                            scell. It is used only for F1B with channel selection*/
#endif
/*f1b_Sprint3*/
   RgSchSCellN1ResCb     n1PucchF1bResCb; /*!< N1 resouurce Cb for F1b Channel selection */
   RgSchSCellN3ResCb     n3PucchResCb;
   RgrSchFrmt1b3TypEnum  uciFrmtTyp; /*!< Store value of format1bcs or format3.It
                                       is updated from upper layer*/
/*f1b_Sprint3*/
   uint8_t             numSCells; /* !< number of configured SCells */
   uint8_t             cellIdToCellIdxMap[CM_LTE_MAX_CELLS]; /*!< Mapping info of Cell Id to Cell Idx */

   RgSchUeCellInfo *cellInfo[CM_LTE_MAX_CELLS]; /*!< DL Sec Cell Information */
#ifdef TFU_UPGRADE
   RgSchUePCqiCb   *nPCqiCb;  /*!< Pointer to Periodic Cqi CB for which next CQI is expected*/
   RgSchUePCqiCb   *nPRiCb;   /*!< Pointer to Periodic Cqi CB for which next RI is expected*/
#endif
   uint8_t              remBoCnt;     /*!< count of occurence when BO is not fullfilled
                                in a TTI */
   uint8_t              *zeroBoCnt;   /*!< pointer of count of occurence when BO is
                                   Zero */
#ifdef LTE_ADV
   Bool            isScellExplicitDeAct; /*!< TRUE when SCELL deactivation timer is Infinity/Not configured */
   Bool            allocCmnUlPdcch;  /*!< If this flag is TRUE, allocate PDCCH from Common
                                        search space */
   uint8_t              simulAckNackCQIFormat3; /* !< Flag for simultanious A/N and CQI on PUCCH Format 3 */
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
   uint8_t                 numBndRetries;   /*!< Number of Bind Retries */
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
   uint8_t         configurationIndex; /* New RGR Cell cfg */
   SfnEnum    sfn;
   uint8_t         subframes[10];
} RgSchConfigIdx;

typedef struct rgSchRapId
{
   uint8_t                rapId;
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
   uint8_t     numRbs;        /*!< Total number of RBs for which information is
                              stored */
   uint8_t     numRbgs;       /*!< Number of RBGs for the BW (rounded off to the
                              closest RBG number */
   uint8_t     rbgSize;        /*!< RBG size */
   uint8_t     lastRbgSize;    /*!< Last RBG size : in number of RBs */
   uint8_t     rbgSubsetSize[RG_SCH_NUM_RATYPE1_SUBSETS]; /*!< RBG Subset 0,1,2,3
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
   uint8_t      mcs;
   uint8_t      nPrb; 
   MsgLen  msgLen;
}RgSchSib1Info;

/* SI info */
typedef struct siInfo
{
   Buffer *si;
   uint8_t      mcs;
   uint8_t      nPrb; 
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
   uint8_t   siId;             /*!< SI Id */
   CmLteTimingInfo timeToTx;   /*!< Time at which the SI for this SI
                                    context need to be scheduled.*/
   CmLteTimingInfo maxTimeToTx;/*!< Max Time to TX for this SI */
   uint8_t              retxCntRem; /*!< SI retransmit count remaining */
   uint16_t             i;          /*!< Value used to calculate the Riv of SI */
   Bool           warningSiFlag; /*!< Flag for Warning SI */
} RgSchSiCtx;


/** 
 @brief Contains each PDU of Warning SI */
typedef struct warningSiPdu
{
   CmLList lnk;
   Buffer *pdu;
   uint8_t      mcs;
   uint8_t      nPrb; 
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
   uint8_t                     siId; /*!< Warning SI ID */
   uint8_t                     idx; /*!< Warning SI Idx in RgSchWarningSiInfo */
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
   uint16_t             prevBiTime;   /*!< Previous BI Value in ms Calculated and
                                   Sent in Previous Response */
   CmLteTimingInfo biTime;       /*!< Time at which previous BI sent */
} RgSchBiInfo;

/* RRM_SP1_START */
typedef struct rgSchQciPrbUsage
{
   uint8_t  qci;          /*!< QCI of the Logical Channel */
   uint32_t dlTotPrbUsed; /*!<  total PRB used for DL within one interval*/
   uint32_t ulTotPrbUsed; /*!< total PRB used for UL within one interval*/
}RgSchQciPrbUsage;

/* RRM_SP1_END */

typedef struct rgSchPrbUsage
{
   Bool prbRprtEnabld; /*!< reporting is enabled or not*/
   uint16_t rprtPeriod; /*!< reporting interval to send PRB usage to the 
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
   uint8_t                         absPatternDlIdx;
   uint32_t                        absLoadTtiCnt;                   
   uint32_t                        absLoadInfo[RGR_ABS_PATTERN_LEN];
} RgSchLteAdvFeatureCb;
/* LTE_ADV_FLAG_REMOVED_END */

/*<! baseSize is the fixed sizes of the respective DCIs. If no other size
 * mentioned, baseSize is the final size of the DCI */
typedef struct rgSchCellDciSize {
#ifdef EMTC_ENABLE   
   uint8_t baseSize[TFU_DCI_FORMAT_6_2+1]; /*!< Fixed Base Size of DCI 0/1/1A/6-0A/6-1A/6-0B/6-1B/6-2 without any adjustment */
   uint8_t size[TFU_DCI_FORMAT_6_2+1]; /*!< DCI 0/1A final size in common Search Space and not scrambled by C-RNTI
                                      DCi 3/3A final sizes
                                      DCI 2/2A final sizes 
                                      DCI 6-0A/6-0B final sizes 
                                      DCI 6-1A/6-1B final sizes 
                                      DCI 6-2 final sizes */
#else                                      
   uint8_t baseSize[TFU_DCI_FORMAT_1A+1]; /*!< Fixed Base Size of DCI 0/1/1A without any adjustment */
   uint8_t size[TFU_DCI_FORMAT_3A+1]; /*!< DCI 0/1A final size in common Search Space and not scrambled by C-RNTI
                                      DCi 3/3A final sizes
                                      DCI 2/2A final sizes */
#endif                                      
   uint8_t dci0HopSize;               /*!< DCI 0 Hop Size */
} RgSchCellDciSize;

/**
  * @brief
  * Cell level measurements
  */
typedef struct rgSchMeasCb
{
   uint32_t dlTpt; /*!< DL Bytes served in a fixed time PERIOD */
   uint32_t dlBytesCnt; /*!< DL Bytes served from start of this time period */
   uint32_t ulTpt; /*!< DL Bytes served in a fixed time PERIOD */
   uint32_t ulBytesCnt; /*!< UL Bytes served from start of this time period */
}RgSchMeasCb;

/**
  * @brief
  * Cell level thresholds 
  */
typedef struct rgSchThrsldsCb
{
   uint8_t maxDlItbs;
   uint8_t maxUlItbs;
}RgSchThrsldsCb;
/**
  * @brief
  * CPU Overload control state info
  */
typedef struct rgSchCpuOvrLdCntrlCb 
{
   uint8_t  cpuOvrLdIns;
   uint32_t tgtDlTpt;
   uint32_t tgtUlTpt;
   uint8_t  dlNxtIndxDecNumUeTti; /*!< Total DL num UE per TTI reduction instruction applied */
   uint8_t  ulNxtIndxDecNumUeTti; /*!< Total UL num UE per TTI reduction instruction applied */
   uint8_t  maxUeNewTxPerTti[10]; /*!< list of subframe  where DL num UE per TTI reduction applied */
   uint8_t  maxUeNewRxPerTti[10]; /*!< list of subframe  where UL num UE per TTI reduction applied */
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
   uint8_t            schTickDelta;     /* 4UE_TTI_DELTA */
   Bool          stopSiSch;        /*!< If TRUE Bcch,Pcch Scheduling is not done */
   uint8_t            stopDlSch;        /*!< If TRUE DL scheduling is not done */ 
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
   uint8_t              numTxAntPorts;    /*!< Number of Tx antenna ports */
   RgrBwCfg        bwCfg;            /*!< Bandwidth Configuration */
   uint8_t              pbchRbStart;      /*!< Indicates the Start RB of the center 6 RBs of DL BW */
   uint8_t              pbchRbEnd;        /*!< Indicates the Start RB of the center 6 RBs of DL BW */
   uint8_t              numCellRSPerSf;   /*!< Indicates the number of cell specific
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
   uint8_t              ulCcchId;         /*!< LcId for uplink CCCH */
   uint8_t              dlCcchId;         /*!< LcId for downlink CCCH */
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
   uint8_t              crntSfIdx;       /*!< Current index for allocation */
#ifdef LTE_TDD
   RgInfSfAlloc    sfAllocArr[RGSCH_SF_ALLOC_SIZE]; /*!< Subframe Allocation
                                            info to be sent to MAC */
#else
   RgInfSfAlloc    sfAllocArr[RGSCH_NUM_SUB_FRAMES]; /*!< Subframe Allocation
                                            info to be sent to MAC */
#endif
   RgInfRlsHqInfo  rlsHqArr[RGSCH_NUM_SUB_FRAMES]; /*!< Harq Release
                                            info to be sent to MAC */
   uint8_t              crntHqIdx;       /*!< Current index for Harq release info */
   RgSchLowSapCb   *tfuSap;
/* Added for sending TTI tick to RRM */
#if (defined(RGR_RRM_TICK) || defined(RGR_CQI_REPT))
   RgSchUpSapCb    *rgrSap;         /*!< Pointer to the cell's RGR SAP
                                      Control Block */
#endif
   RgSchUpSapCb    *rgmSap;         /*!< Pointer to the cell's RGM SAP
                                      Control Block */
#ifdef RGR_RRM_TICK
   uint8_t               rrmTtiIndPrd;   /*!< Periodicity of TTI indication from
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
   uint8_t                   numDlSubfrms;    /*!< Number of DL subframes */
   uint8_t                   ulDlCfgIdx;      /*!< UL-DL Configuration Index */
   uint8_t                   ackNackFdbkArrSize; /*!< Maximum Number of Ack/Nack 
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
   uint16_t             nCce;
#endif
   RgSchDynCfiCb         dynCfiCb;        /*!< Dynamic CFI control block */  
/* Changes for MIMO feature addition */
   uint8_t                   noOfRbgs;       /*!< Number of RBGs for this bw */
   uint8_t                   rbgSize;        /*!< RBG Size */
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
   uint8_t                ulAvailBw;            /*!< Total Uplink band width available
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
   uint16_t  t300TmrVal; /*!< t300Timer value configured in Frames */
/* LTE_ADV_FLAG_REMOVED_START */
   TknStrOSXL            rntpAggrInfo;    /*!< RNTP Info */
   RgrLoadInfReqInfo     loadInfReqInfo;  /*!< Consists startRb & endRb 
                                               location for which CC to 
                                               be raised to P-High */
   RgSchLteAdvFeatureCb  lteAdvCb;        /*!< Control block for LTE Adv 
                                               features */
/* LTE_ADV_FLAG_REMOVED_END */
   uint32_t   dlNumUeSchedPerTti[RG_MAX_NUM_UE_PER_TTI]; /*!<DL mUe/Tti histograms*/
   uint32_t   ulNumUeSchedPerTti[RG_MAX_NUM_UE_PER_TTI]; /*!<UL mUe/Tti histograms*/
   Bool  overLoadBackOffEnab; /*!< Overload Rach Backoff enable/disable */
   uint8_t    overLoadBackOffval;  /*!< Overload Rach BackOff value */
   uint8_t    refreshUeCnt[RGSCH_MAX_REFRESH_OFFSET];  /*! To maintain number of UE refreshed per subframe */
   uint8_t    minDlResNonCsg;     /*!< Minimum DL resources reserved for Non CSG UEs */
   uint8_t    minUlResNonCsg;     /*!< Minimum UL resources reserved for CSG UEs */
   Bool isAutoCfgModeEnb;     /*!< Indicates Auto config of TM mode is enabled or
                               disabled. True - Enabled, False - Disabled */
   CmLListCp lcMonitorLst;         /*LC Lst used for flow cntrl */
   uint32_t       prbCnt;         /*!<PRB usage in flow control interval*/
   uint32_t       maxPrbCnt;      /*!<Max PRB cnt after which Flow Cntrl 
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
   uint32_t              totalPrb;
   uint32_t              totalTime;
#endif
   Void *            laaCb;
#ifdef EMTC_ENABLE
   /* TODO:: Below members need to be moved
    * to emtc specific files and have void * 
    * here */
   uint8_t                    emtcEnable;
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
   uint8_t              tmrRes;           /*!< Timer resolution */
   RgSchLmResponse *bndCfmResp;       /*!< Respones value for Bind Confirm */
   RgSchLmResponse ustaResp;         /*!< Respones value for Alarms */
   uint8_t              startCellId;      /*!< Starting Cell Id */
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
  uint8_t              sfType;  /*!< 0= NOT Defined 
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
   uint8_t                   crntDTddSfIdx; /*!< Pivot Index corresponding 
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
   uint8_t            numSaps;                   /*!< Num RGR Saps = Num TFU Saps */
   RgSchUpSapCb  *rgrSap;                    /*!< RGR SAP Control Block */
   RgSchLowSapCb *tfuSap;                    /*!< TFU SAP Control Block */
   RgSchUpSapCb  *rgmSap;                    /*!< TFU SAP Control Block */
   CmTqCp        tmrTqCp;                   /*!< Timer Task Queue Cntrl Point */
   CmTqType      tmrTq[RGSCH_TQ_SIZE];         /*!< Timer Task Queue */
   uint8_t            rgSchDlDelta;                /* 4UE_TTI_DELTA */
   uint8_t            rgSchCmnDlDelta;
   uint8_t            rgSchUlDelta;
   RgSchCellCb   *cells[CM_LTE_MAX_CELLS];  /* Array to store cellCb ptr */  
   RgrSchedEnbCfg rgrSchedEnbCfg;                  /*!< eNB level RR/PFS Config */
   Void          *rgSchEnbPfsDl;             /*!< eNB level PFS DL Block */

   Void *         laaCb;
#ifdef RG_5GTF
   RgSchDynTddCb  rgSchDynTdd;               /*!< Dynamic TDD Control Block */
#endif
};

/* Declaration for scheduler control blocks */
RgSchCb rgSchCb[RGSCH_MAX_INST];

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
      uint32_t          bo;            /*!< Buffer occupancy for CCCH */
   } dlCcchInfo;                  /*!< Params for DL CCCH */
   uint8_t              msg3HqProcId;  /*!< Msg3 Harq Process ID */
   /*ccpu00128820 - DEL - msg3HqProcRef is delete for Msg3 alloc double delete issue*/
   RgSchUlHqProcCb msg3HqProc;    /*!< msg3HqProcRef points to this initially */
   RgSchUeCb       *ue;           /*!< NULL initially */
   Bool            toDel;         /*!< To delete this RaCb after msg4 reject */
   TknUInt8           phr;           /*!< To store the PHR, if received along with
                                       Msg3 */
   CmLList         rspLnk;        /*!< Used to link RACB to a frame for resp */
   uint8_t              rapId;         /*!< RAP ID */
   TknUInt16          ta;            /*!< Timing Adjustment */
   RgSchUlGrnt     msg3Grnt; /*!< Msg3 grant as given by the UL Sched */
   uint32_t             y[RGSCH_NUM_SUB_FRAMES]; /*!< y values using tmpCrnti by DLSCHED */
   RgSchDlHqEnt    *dlHqE;     /*!< DL HARQ module */
   uint8_t              ccchCqi;     /*!< DL Cqi obtained from RaReq and Used for CCCH */
   RgSchDlRbAlloc  rbAllocInfo; /*!< RB Allocation Info for MSG4 Trans/Retrans */
   /* PHR handling for MSG3 */
   CmLteTimingInfo msg3AllocTime; /*!< Allocation time for msg3 grant */
#ifdef RGR_V1
   /* CR timer  changes*/
   CmLList         contResTmrLnk;       /*!< To link raCb to the
                   Guard Timer/Contention Resolution timer list*/
   CmLteTimingInfo expiryTime; /*!< Expiry time for Guard/Contention
             Resolution timers */

   uint32_t             ccchSduBo;   /*!<To store CCCH SDU BO if it arrives while
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
   uint8_t    errType;    /*!< Error Type */
   uint16_t   errCause;   /*!< Cause of Error */
};

/* Global Variables */
#ifdef LTE_TDD

#ifdef LTEMAC_SPS
uint8_t rgSchTddSpsDlMaxRetxTbl[RGSCH_MAX_TDD_UL_DL_CFG]; 
#endif
typedef uint8_t RgSchTddUlDlSubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
RgSchTddUlDlSubfrmTbl rgSchTddUlDlSubfrmTbl;

typedef struct rgSchTddSplSubfrmInfo RgSchTddSplSubfrmInfoTbl[RGSCH_MAX_TDD_SPL_SUBFRM_CFG];
RgSchTddSplSubfrmInfoTbl rgSchTddSplSubfrmInfoTbl;

typedef struct rgSchTddDlAscSetIdxK RgSchTddDlAscSetIdxKTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
RgSchTddDlAscSetIdxKTbl rgSchTddDlAscSetIdxKTbl;
/* ccpu00132282 */
RgSchTddDlAscSetIdxKTbl rgSchTddDlHqPucchResCalTbl;

typedef uint8_t RgSchTddPhichMValTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
RgSchTddPhichMValTbl rgSchTddPhichMValTbl;

typedef uint8_t RgSchTddKPhichTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
RgSchTddKPhichTbl rgSchTddKPhichTbl;

typedef RgSchTddPhichOffInfo RgSchTddPhichOffInfoTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];

typedef uint8_t RgSchTddUlAscIdxKDashTbl[RGSCH_MAX_TDD_UL_DL_CFG-1][RGSCH_NUM_SUB_FRAMES];
RgSchTddUlAscIdxKDashTbl rgSchTddUlAscIdxKDashTbl;

#ifdef LTEMAC_SPS
typedef uint8_t RgSchTddInvDlAscSetIdxTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
RgSchTddInvDlAscSetIdxTbl rgSchTddInvDlAscSetIdxTbl;
#endif

typedef uint8_t RgSchTddPuschTxKTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
RgSchTddPuschTxKTbl rgSchTddPuschTxKTbl;

typedef uint8_t RgSchTddUlNumHarqProcTbl[RGSCH_MAX_TDD_UL_DL_CFG];
RgSchTddUlNumHarqProcTbl rgSchTddUlNumHarqProcTbl;

typedef uint8_t RgSchTddDlNumHarqProcTbl[RGSCH_MAX_TDD_UL_DL_CFG];
RgSchTddDlNumHarqProcTbl rgSchTddDlNumHarqProcTbl;

/* Number of ACK/NACK Feedback to be stored based on UL-DL Configuration Index */
typedef uint8_t RgSchTddANFdbkMapTbl[RGSCH_MAX_TDD_UL_DL_CFG];
RgSchTddANFdbkMapTbl rgSchTddANFdbkMapTbl;

/* Number of UL subframes */
typedef RgSchTddSubfrmInfo RgSchTddMaxUlSubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG];
RgSchTddMaxUlSubfrmTbl rgSchTddMaxUlSubfrmTbl;

/* Number of UL subframes */
typedef uint8_t RgSchTddNumUlSubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
RgSchTddNumUlSubfrmTbl rgSchTddNumUlSubfrmTbl;

/* Number of low UL subframes Indices*/
typedef uint8_t RgSchTddLowUlSubfrmIdxTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
RgSchTddLowUlSubfrmIdxTbl rgSchTddLowUlSubfrmIdxTbl;

/* Number of high UL subframes Indices*/
typedef uint8_t RgSchTddHighUlSubfrmIdxTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
RgSchTddHighUlSubfrmIdxTbl rgSchTddHighUlSubfrmIdxTbl;

/* Number of low DL subframes Indices*/
typedef uint8_t RgSchTddLowDlSubfrmIdxTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
RgSchTddLowDlSubfrmIdxTbl rgSchTddLowDlSubfrmIdxTbl;

/* Number of high DL subframes Indices*/
typedef uint8_t RgSchTddHighDlSubfrmIdxTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
RgSchTddHighDlSubfrmIdxTbl rgSchTddHighDlSubfrmIdxTbl;

/* Number of DL subframes and Special subframes with DwPTS */
typedef uint8_t RgSchTddNumDlSubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
RgSchTddNumDlSubfrmTbl rgSchTddNumDlSubfrmTbl;

/* Number of DL subframes and Special subframes with DwPTS */
typedef RgSchTddSubfrmInfo RgSchTddMaxDlSubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG];
RgSchTddMaxDlSubfrmTbl rgSchTddMaxDlSubfrmTbl;

typedef uint8_t RgSchTddMsg3SubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
RgSchTddMsg3SubfrmTbl rgSchTddMsg3SubfrmTbl;
#ifdef LTEMAC_SPS
typedef RgSchTddMsg3SubfrmTbl RgSchTddSpsUlRsrvTbl;
RgSchTddMsg3SubfrmTbl rgSchTddSpsUlRsrvTbl;
#endif

typedef uint8_t RgSchTddRlsDlSubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
RgSchTddRlsDlSubfrmTbl rgSchTddRlsDlSubfrmTbl;


uint8_t rgSchTddPucchTxTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];

#endif
#ifdef LTE_ADV
RgSchCellCb* rgSchUtlGetCellCb ARGS((
Inst         inst,
uint16_t          cellId
));

Void rgSCHSCellDlUeReset ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue
));

Void rgSCHSCellDlLcCfg ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));

Void rgSCHSCellDlLcDel ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));

Void rgSCHSCellDlDedBoUpd ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));

Void rgSCHSCellSchdActDeactCe ARGS((
RgSchUeCb         *ueCb,
RgSchDlHqTbCb     *tbInfo
));

Void rgSCHSCellAddToActDeactLst ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue
));

Void rgSCHSCellRmvFrmActLst ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue
));
S16 rgSCHSCellIsActive ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue
));



Void rgSCHSCellHndlFdbkInd ARGS((
RgSchDlHqProcCb   *hqP,
uint8_t                tbIdx,
uint8_t                fdbk,
Bool              maxHqRetxReached
));

Void rgSCHSCellDeactTmrExpry ARGS((
RgSchUeCellInfo *sCell
));

S16 rgSCHSCellTrigActDeact ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ueCb,
uint8_t            sCellIdx,
uint8_t            action
));

S16 rgSCHSCellDelUe ARGS((
RgSchCellCb  *cellCb,
RgSchUeCb    *ueCb
));

Bool rgSCHIsActvReqd ARGS ((
RgSchCellCb    *cell,
RgSchUeCb      *ue
));

Void rgSCHSCellSelectAndActDeAct ARGS ((
RgSchCellCb  *PCell,
RgSchUeCb    *ueCb,
uint8_t           action
));

S16 rgSCHSCellPCqiCfg ARGS((
RgSchCellCb  *priCellCb,
RgSchCellCb  *secCellCb,
RgSchUeCb    *ueCb,
RgrUePrdDlCqiCfg  *cqiCfg,
CmLteUeCategory   ueCat,
uint8_t            sCellIdx
));

Void rgSCHUtlSndUeSCellDel2Mac ARGS ((
RgSchCellCb    *cell,
CmLteRnti      rnti
));

uint8_t rgSCHUtlGetMaxTbSupp ARGS ((
RgrTxMode txMode
));
#endif/*LTE_ADV*/

/* APIs exposed by TMR module */
Void rgSCHTmrStartTmr ARGS((
         RgSchCellCb *cellCb,
         Ptr         cb,
         S16         tmrEvnt,
         uint32_t    tmrVal));

Void rgSCHTmrStopTmr ARGS((
         RgSchCellCb *cellCb,
         S16         tmrEvnt,
         Ptr         cb));

Void rgSCHTmrProcTmr ARGS((
         Ptr         cb,
         S16         tmrEvnt));

/* APIs exposed by TOM */

S16 rgSCHTomRaReqInd ARGS((
         RgSchCellCb     *cell,
         TfuRaReqIndInfo *raReqInd));

S16 rgSCHTomPucchDeltaPwrInd ARGS((
         RgSchCellCb           *cell,
         TfuPucchDeltaPwrIndInfo *pucchDeltaPwr));

S16 rgSCHTomUlCqiInd ARGS((
         RgSchCellCb     *cell,
         TfuUlCqiIndInfo *ulCqiInd));

S16 rgSCHTomSrInd ARGS((
         RgSchCellCb     *cell,
         TfuSrIndInfo    *srInd));

S16 rgSCHTomDlCqiInd ARGS((
         RgSchCellCb     *cell,
         TfuDlCqiIndInfo *dlCqiInd));
/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
S16 rgSCHTomRawCqiInd ARGS
((
RgSchCellCb     *cell,
TfuRawCqiIndInfo* rawCqiInd
));

S16 rgSCHTomSrsInd ARGS
((
RgSchCellCb     *cell,
TfuSrsIndInfo* srsInd
));

#endif

S16 rgSCHTomDoaInd ARGS((
         RgSchCellCb     *cell,
         TfuDoaIndInfo   *doaInd));
S16 rgSCHTomCrcInd ARGS((
         RgSchCellCb       *cell,
         TfuCrcIndInfo *crcInd));

S16 rgSCHTomHarqAckInd ARGS((
         RgSchCellCb       *cell,
         TfuHqIndInfo      *harqAckInd));

S16 rgSCHTomTimingAdvInd ARGS((
         RgSchCellCb         *cell,
         TfuTimingAdvIndInfo *timingAdvInd));


/*
 * APIs exposed by LMM
 */
S16 rgSCHLmmStartTmr ARGS ((Inst instId, S16 tmrEvnt,
         uint32_t tmrVal, PTR cb));
S16 rgSCHLmmStopTmr ARGS((Inst instId, S16 tmrEvnt, PTR cb));
S16 rgSCHLmmTmrExpiry ARGS((PTR cb, S16 tmrEvnt));
/* This function invokes a Control Confirmation to the LM from scheduler. */
S16 rgSCHLmmBndCfm ARGS((Pst *pst, SuId suId, uint8_t status));
S16 schActvTmr ARGS((Ent entity, Inst inst));
/* To send a Unsolicited Status Indication to Layer Manager */
S16 rgSCHLmmStaInd ARGS((Inst instId, uint16_t category, uint16_t event,
                          uint16_t cause, RgUstaDgn *dgn));
S16 schActvTsk ARGS((Pst *pst, Buffer *mBuf));
Void SchFillCfmPst ARGS((Pst    *reqPst,Pst *cfmPst,RgMngmt  *cfm));
uint16_t SchInstCfg ARGS((RgCfg    *cfg, Inst inst));
Void printSchCellInfo ARGS((Void));
Void rgSCHLmmGenCntrl ARGS((RgMngmt *cntrl,RgMngmt  *cfm,Pst *cfmPst));
Void rgSCHLmmSapCntrl ARGS((RgMngmt *cntrl,RgMngmt *cfm,Pst *cfmPst));

#ifdef EMTC_ENABLE
S16 rgSCHCfgEmtcCellCfg ARGS ((RgSchCellCb   *cell,
         RgrEmtcCellCfg *emtcCellCfg));
S16 rgSCHCfgVldtRgrEmtcCellCfg ARGS ((RgrCellCfg *cellCfg));

Void rgSchTomTtiEmtcSched ARGS((RgSchCellCb        *cell));
S16 rgSCHCfgVldtEmtcUeCfg ARGS((RgSchCellCb *cell, RgrUeEmtcCfg *emtcUeCfg));
S16 rgSCHUtlUpdUeEmtcInfo ARGS((RgSchCellCb *cell, RgrUeCfg *ueCfg, RgSchUeCb *ueCb));
S16 rgSCHEmtcCellDel ARGS((RgSchCellCb        *cell));
S16 rgSCHEmtcUeDel ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
S16 rgSCHEmtcHdFddUeCfg ARGS((RgSchCellCb  *cellCb,RgSchUeCb *ueCb,Bool hdFddEnbl));

#endif
/*
 * APIs exposed by CFG module
 */
S16 rgSCHCfgVldtRgrCellCfg ARGS((Inst inst, RgrCellCfg  *cellCfg,
      RgSchCellCb *cell, RgSchErrInfo *errInfo));
S16 rgSCHCfgRgrCellCfg ARGS((RgSchCb *instCb, SpId spId,
      RgrCellCfg *cellCfg, RgSchErrInfo *errInfo));
S16 rgSCHCfgRgrSchedEnbCfg ARGS((Inst inst, SpId spId,
      RgrSchedEnbCfg *schedEnbCfg, RgSchErrInfo *errInfo));
S16 rgSCHCfgVldtRgrCellRecfg ARGS((Inst inst, RgrCellRecfg *cellRecfg,
      RgSchCellCb  **cell, RgSchErrInfo *errInfo));
S16 rgSCHCfgRgrCellRecfg ARGS((RgSchCellCb *cell, RgrCellRecfg *cellRecfg,
      RgSchErrInfo *errInfo));

S16 rgSCHCfgVldtRgrUeCfg ARGS((Inst inst, RgrUeCfg  *ueCfg,
      RgSchCellCb  **cell, RgSchErrInfo *errInfo));
S16 rgSCHCfgRgrUeCfg ARGS((RgSchCellCb  *cell, RgrUeCfg  *ueCfg,
      RgSchErrInfo  *errInfo));
S16 rgSCHCfgVldtRgrUeRecfg ARGS((Inst inst, RgrUeRecfg *ueRecfg,
      RgSchCellCb **cell, RgSchUeCb **ue, RgSchErrInfo *errInfo));
S16 rgSCHCfgRgrUeRecfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgrUeRecfg
      *ueRecfg, RgSchErrInfo *errInfo));

S16 rgSCHCfgVldtRgrLcCfg ARGS((Inst inst, RgrLchCfg *lcCfg,
      RgSchCellCb **cell, RgSchUeCb **ue, RgSchErrInfo  *errInfo));
S16 rgSCHCfgRgrLchCfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
      RgrLchCfg *lcCfg, RgSchErrInfo *errInfo));
S16 rgSCHCfgVldtRgrLchRecfg ARGS((Inst inst, RgrLchRecfg *lcRecfg,
      RgSchCellCb **cell, RgSchUeCb **ue, RgSchDlLcCb **dlLc,
      RgSchErrInfo *errInfo));
S16 rgSCHCfgRgrLchRecfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *dlLc, RgrLchRecfg *lcRecfg, RgSchErrInfo *errInfo));

S16 rgSCHCfgVldtRgrLcgRecfg ARGS ((Inst inst,RgrLcgRecfg *lcgRecfg,
         RgSchCellCb *cell,RgSchUeCb **ue,RgSchErrInfo *errInfo ));
S16 rgSCHCfgVldtRgrLcgCfg ARGS ((Inst inst,RgrLcgCfg *lcgCfg,
         RgSchCellCb **cell,RgSchUeCb **ue,RgSchErrInfo *errInfo ));
S16 rgSCHCfgVldtRgrSchedEnbCfg ARGS ((Inst inst, 
         RgrSchedEnbCfg *schedEnbCfg, RgSchErrInfo *errInfo ));
S16 rgSCHCfgRgrLcgCfg ARGS ((RgSchCellCb *cell,RgSchUeCb *ue,
         RgrLcgCfg *lcgCfg,RgSchErrInfo *errInfo ));
S16 rgSCHCfgRgrLcgRecfg ARGS ((RgSchCellCb *cell,RgSchUeCb *ue,
         RgrLcgRecfg *lcgRecfg,RgSchErrInfo *errInfo));

S16 rgSCHCfgVldtRgrUeReset ARGS((Inst inst, RgrRst *reset, RgSchCellCb *cell,
         RgSchUeCb **ue,RgSchErrInfo     *errInfo));
S16 rgSCHCfgRgrUeReset ARGS((RgSchCellCb *cell,RgSchUeCb *ue,RgrRst *reset,
         RgSchErrInfo   *errInfo));

S16 rgSCHCfgRgrCellDel ARGS((RgSchCellCb *cell, RgrDel *cellDelInfo,
      RgSchErrInfo *errInfo));
S16 rgSCHCfgRgrUeDel ARGS((RgSchCellCb *cell, RgrDel *ueDelInfo,
      RgSchErrInfo *errInfo));
S16 rgSCHCfgRgrLcDel ARGS((RgSchCellCb *cell, RgrDel *lcDelInfo,
      RgSchErrInfo *errInfo));
S16 rgSCHCfgRgrLcgDel ARGS ((RgSchCellCb *cell,RgrDel *lcDelInfo,
         RgSchErrInfo *errInfo));
Void rgSCHCfgFreeCellCb ARGS((RgSchCellCb *cell));
/* Added for SI Enhancement*/
#ifdef RGR_SI_SCH
S16 rgSCHCfgVldtRgrSiCfg ARGS(( Inst inst, RgrSiCfgReqInfo *siCfg,
                    RgSchCellCb      *cell, RgSchErrInfo *errInfo));
S16 rgSCHGomHndlSiCfg ARGS(( Region   reg, Pool   pool,
                  RgSchCb       *instCb, SpId    spId,
                  RgrCfgTransId transId, RgrSiCfgReqInfo *cfgReqInfo));
S16 rgSCHUtlRgrSiCfgCfm ARGS ((Inst inst, SpId spId,
                                  RgrCfgTransId transId,uint8_t status));

S16 rgSCHGomHndlWarningSiCfg ARGS(( Region   reg, Pool   pool,
                  RgSchCb       *instCb, SpId    spId, RgrCfgTransId transId,
                  RgrWarningSiCfgReqInfo *warningSiCfgReqInfo));

Void rgSCHGomHndlWarningSiStopReq ARGS(( Region   reg, Pool   pool,
                  RgSchCb       *instCb, uint8_t            siId, 
                  RgrCfgTransId transId, SpId    spId));

S16 rgSCHUtlRgrWarningSiCfgCfm ARGS ((Inst inst, SpId spId, uint8_t siId,
                                  RgrCfgTransId transId,uint8_t status));
#endif /* RGR_SI_SCH */
/* LTE_ADV_FLAG_REMOVED_START */
S16 rgSchDSFRRntpInfoInit ARGS ((TknStrOSXL *rntpPtr, RgSchCellCb  *cell,
                                           uint16_t bw));
S16 rgSchDSFRRntpInfoFree ARGS ((TknStrOSXL *rntpPtr, RgSchCellCb  *cell,
                                           uint16_t bw));
S16 rgSchUpdtRNTPInfo ARGS ((RgSchCellCb *cell, RgSchDlSf  *sf,
                                RgrLoadInfReqInfo       *loadInfReq));
S16 rgSCHCfgVldtRgrLoadInf ARGS(( Inst inst, RgrLoadInfReqInfo *loadInfReq,
                    RgSchCellCb      *cell, RgSchErrInfo *errInfo));
S16 rgSCHGomHndlLoadInf ARGS(( Region   reg, Pool   pool,
                    RgSchCb       *instCb, SpId    spId,
                    RgrCfgTransId transId, RgrLoadInfReqInfo *cfgReqInfo));
/* LTE_ADV_FLAG_REMOVED_END */

/*
 * APIs exposed by GOM module
 */
S16 rgSCHGomHndlCfg ARGS((Pst *pst, RgSchCb *instCb,
                            RgrCfgTransId transId, RgrCfgReqInfo *cfgReqInfo));
S16 rgSCHGomTtiHndlr ARGS((RgSchCellCb *cell, SpId spId));


/*
 * APIs exposed by RAM module
 */
S16 rgSCHRamVldtUeCfg ARGS((
RgSchCellCb  *cell,
RgrUeCfg     *ueCfg
));
S16 rgSCHRamProcRaReq     ARGS((uint8_t raReqCnt, RgSchCellCb *cell, CmLteRnti raRnti,
                                    TfuRachInfo  *raReqInd,
                                    CmLteTimingInfo timingInfo, 
                                    RgSchUeCb *ue,
                                    RgSchErrInfo *err));
S16 rgSCHRamCreateRaCb ARGS((RgSchCellCb *cell, RgSchRaCb **raCb,
                                    RgSchErrInfo *err));
S16 rgSCHRamRgrUeCfg      ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                                    RgSchRaCb *raCb, RgSchErrInfo *err));
S16 rgSCHRamProcMsg3      ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                                    RgSchRaCb *raCb, RgInfUeDatInd  *pdu,
                                    RgSchErrInfo *err));
S16 rgSCHRamUpdtBo        ARGS((RgSchCellCb *cell, RgSchRaCb *raCb,
                                    RgInfCmnBoRpt *staRsp));
S16 rgSCHRamMsg3DatInd   ARGS((RgSchRaCb *raCb));
S16 rgSCHRamMsg3FailureInd   ARGS((RgSchRaCb *raCb));
S16 rgSCHRamMsg4FdbkInd   ARGS((RgSchRaCb *raCb));
S16 rgSCHRamMsg4Done      ARGS((RgSchCellCb *cell, RgSchRaCb *raCb));
S16 rgSCHRamDelRaCb       ARGS((RgSchCellCb *cell, RgSchRaCb *raCb,
                                    Bool rlsRnti));
S16 rgSCHRamFreeCell      ARGS((RgSchCellCb *cell));
S16 rgSCHRamTtiHndlr      ARGS((RgSchCellCb *cell));
Void rgSCHCmnUlSch ARGS((RgSchCellCb *cell));
Void rgSCHCmnDlCommonChSch ARGS ((RgSchCellCb  *cell));
#ifdef RGR_V1
/* Added periodic BSR timer */
S16 rgSCHCmnBsrTmrExpry ARGS(( RgSchUeCb  *ueCb));
#endif
#ifdef LTE_TDD
S16 rgSCHRamDelRaReq      ARGS((RgSchCellCb *cell,
                                       CmLteTimingInfo timingInfo,
                                       uint8_t raIdx));
#endif
S16 rgSCHRamAddToRaInfoSchdLst(RgSchCellCb   *cell, RgSchRaCb *raCb);

S16 rgSCHRamRmvFrmRaInfoSchdLst(RgSchCellCb   *cell, RgSchRaCb   *raCb);

/* APIs exposed by UHM */
/* Added for Uplink Adaptive retransmission */
Void rgSCHUhmNonadapRetx ARGS((RgSchUlHqProcCb *hqProc));
S16 rgSCHUhmHqEntInit ARGS ((RgSchCellCb       *cellCb, RgSchUeCb *ueCb));
#ifndef MAC_SCH_STATS
Void rgSCHUhmProcDatInd ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         CmLteTimingInfo frm));
#else /* MAC_SCH_STATS */
Void rgSCHUhmProcDatInd ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         CmLteTimingInfo frm, uint8_t cqi));
#endif
Void rgSCHUhmProcMsg3DatInd ARGS((RgSchUlHqProcCb *hqProc));
Void rgSCHUhmProcMsg3Failure ARGS((RgSchUlHqProcCb *hqProc));
#ifndef MAC_SCH_STATS
Void rgSCHUhmProcHqFailure ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         CmLteTimingInfo frm, TknUInt8 rv));
#else /* MAC_SCH_STATS */
Void rgSCHUhmProcHqFailure ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         CmLteTimingInfo frm, TknUInt8 rv, uint8_t cqi));
#endif
RgSchUlHqProcCb* rgSCHUhmGetUlHqProc ARGS((RgSchCellCb      *cell, RgSchUeCb *ue,
         uint8_t idx));
Void rgSCHUhmNewTx ARGS((RgSchUlHqProcCb *hqProc, uint8_t maxHqRetx,
         RgSchUlAlloc *alloc));
Void rgSCHUhmFreeProc ARGS((RgSchUlHqProcCb *hqProc,
         RgSchCellCb      *cell));
Void rgSCHUhmRetx ARGS((RgSchUlHqProcCb *hqProc, RgSchUlAlloc *alloc));
Void rgSCHUhmRgrUeCfg ARGS(( RgSchCellCb *cellCb, RgSchUeCb *ueCb,
          RgrUeCfg *ueCfg));
Void rgSCHUhmRgrUeRecfg ARGS(( RgSchCellCb *cellCb, RgSchUeCb *ueCb,
          RgrUeRecfg *ueRecfg));
Void rgSCHUhmFreeUe ARGS(( RgSchCellCb *cellCb, RgUeUlHqCb *hqEnt));
S16 rgSCHUhmAppendPhich ARGS(( RgSchCellCb *cellCb,
          CmLteTimingInfo frm, uint8_t idx));

/* APIs exposed by DBM */
S16 rgSCHDbmInitCell ARGS((RgSchCellCb *cellCb));
S16 rgSCHDbmDeInitUeCbLst ARGS(( RgSchCellCb *cellCb));
#ifdef LTE_TDD
S16 rgSCHDbmDeInitUeTfuPendLst ARGS(( RgSchCellCb *cellCb));
#endif
S16 rgSCHDbmInsUeCb ARGS(( RgSchCellCb *cellCb, RgSchUeCb *ueCb));
RgSchUeCb* rgSCHDbmGetUeCb ARGS(( RgSchCellCb *cellCb, CmLteRnti ueId));
RgSchUeCb* rgSCHDbmGetNextUeCb ( RgSchCellCb *cellCb, RgSchUeCb *ueCb);
S16 rgSCHDbmDelUeCb ARGS(( RgSchCellCb *cellCb, RgSchUeCb *ueCb));
#ifdef LTE_L2_MEAS
S16 rgSCHDbmDelL2MUe ARGS(( RgSchCellCb *cellCb, RgSchUeCb *ueCb));
#endif
S16 rgSCHDbmInitUe ARGS((RgSchUeCb *ueCb));
Void rgSCHDbmInsDlDedLcCb ARGS((RgSchUeCb *ueCb, RgSchDlLcCb *dlLcCb));
Void rgSCHDbmDelDlDedLcCb ARGS((RgSchUeCb *ueCb, RgSchDlLcCb *dlLcCb));
RgSchDlLcCb* rgSCHDbmGetDlDedLcCb ARGS(( RgSchUeCb *ueCb, CmLteLcId idx));
RgSchDlLcCb* rgSCHDbmGetFirstDlDedLcCb ARGS((RgSchUeCb *ueCbb));
RgSchDlLcCb* rgSCHDbmGetNextDlDedLcCb ARGS((RgSchUeCb *ueCb, RgSchDlLcCb *lcCb));
RgSchClcDlLcCb* rgSCHDbmGetCmnLcCb ARGS(( RgSchCellCb *cellCb, CmLteLcId lcId));
RgSchClcDlLcCb* rgSCHDbmGetBcchOnBch ARGS(( RgSchCellCb *cellCb ));
RgSchClcDlLcCb* rgSCHDbmGetFirstBcchOnDlsch ARGS(( RgSchCellCb *cellCb));
RgSchClcDlLcCb* rgSCHDbmGetSecondBcchOnDlsch ARGS(( RgSchCellCb *cellCb));
RgSchClcDlLcCb* rgSCHDbmGetPcch ARGS(( RgSchCellCb *cellCb));
Void rgSCHDbmInsBcchOnBch ARGS(( RgSchCellCb *cellCb, RgSchClcDlLcCb *cmnDlLcCb));
Void rgSCHDbmInsBcchOnDlsch ARGS(( RgSchCellCb *cellCb, RgSchClcDlLcCb *cmnDlLcCb));
Void rgSCHDbmInsPcch ARGS(( RgSchCellCb *cellCb, RgSchClcDlLcCb *cmnDlLcCb));

Void rgSCHDbmInitCmnLcBoLst ARGS(( RgSchClcDlLcCb *cmnDlLcCb));
Void rgSCHDbmInsCmnLcBoRpt ARGS(( RgSchClcDlLcCb *cmnDlLcCb,
                                    RgSchClcBoRpt *cmnBoRpt));
RgSchRaCb* rgSCHDbmGetRaCb ARGS(( RgSchCellCb *cellCb, CmLteRnti key));
Void rgSCHDbmInsCrntRgrCfgElem ARGS(( RgSchCellCb *cellCb,
                                          RgSchCfgElem *cfgElem));
Void rgSCHDbmInsPndngRgrCfgElem ARGS(( RgSchCellCb *cellCb,
                                          RgSchCfgElem *cfgElem));
RgSchCfgElem* rgSCHDbmGetNextCrntRgrCfgElem ARGS(( RgSchCellCb *cellCb,
                                                   RgSchCfgElem *cfgElem));
RgSchCfgElem* rgSCHDbmGetNextPndngRgrCfgElem ARGS(( RgSchCellCb *cellCb,
                                                   RgSchCfgElem *cfgElem));
RgSchCfgElem* rgSCHDbmGetPndngRgrCfgElemByKey ARGS(( RgSchCellCb *cellCb,
                                                   CmLteTimingInfo key));
RgSchCfgElem* rgSCHDbmDelCrntRgrCfgElem ARGS(( RgSchCellCb *cellCb,
                                                RgSchCfgElem *cfgElem));
RgSchCfgElem* rgSCHDbmDelPndngRgrCfgElem ARGS(( RgSchCellCb *cellCb,
                                                RgSchCfgElem *cfgElem));

S16 rgSCHDbmRntiDbInit ARGS(( RgSchCellCb *cellCb, uint16_t rntiStart, uint16_t maxRntis));
Void rgSCHDbmRntiDbDeInit ARGS(( RgSchCellCb *cellCb));
RgSchRntiLnk* rgSCHDbmGetRnti ARGS(( RgSchCellCb *cellCb));
Void rgSCHDbmRlsRnti ARGS(( RgSchCellCb *cellCb, RgSchRntiLnk *rntiLnk));
/* Fix : syed HO UE does not have a valid ue->rntiLnk */
Void rgSCHUtlIndRntiRls2Mac ARGS(( RgSchCellCb *cell, CmLteRnti rnti,
                                          Bool ueIdChng, CmLteRnti newRnti));

/*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
S16 rgSCHDbmDeInitSpsUeCbLst ARGS((RgSchCellCb *cellCb));
S16 rgSCHDbmInsSpsUeCb ARGS((RgSchCellCb *cellCb, RgSchUeCb *ueCb));
RgSchUeCb* rgSCHDbmGetSpsUeCb ARGS((RgSchCellCb *cellCb, CmLteRnti ueId));
RgSchUeCb* rgSCHDbmGetNextSpsUeCb ARGS((RgSchCellCb *cellCb, RgSchUeCb *ueCb));
S16 rgSCHDbmDelSpsUeCb ARGS((RgSchCellCb *cellCb,RgSchUeCb *ueCb));
#endif /* LTEMAC_SPS */

#ifdef LTE_L2_MEAS
/*
 * L2M APIs
 */
S16 rgSchL2mMeasReq ARGS ((
         RgSchCellCb       *cell,
         LrgSchMeasReqInfo *measInfo,
         RgSchErrInfo      err));
S16 RgSchMacL2MeasSend ARGS
((
Pst*                pst,
RgInfL2MeasSndReq *measInfo
));

S16 RgSchMacL2MeasStop ARGS
((
Pst*                pst,
RgInfL2MeasStopReq *measInfo
));
#endif /* LTE_L2_MEAS */
/*
 * DHM APIs
 */
/* LTE_ADV_FLAG_REMOVED_START */
S16 rgSchSFRTotalPoolInit ARGS((RgSchCellCb *cell, RgSchDlSf *sf));
/* LTE_ADV_FLAG_REMOVED_END */
Void rgSCHDhmHqPAdd2FreeLst ARGS (( RgSchDlHqProcCb *hqP));
Void rgSCHDhmHqPAdd2InUseLst ARGS (( RgSchDlHqProcCb *hqP));
Void rgSCHDhmHqPDelFrmFreeLst ARGS (( RgSchDlHqProcCb *hqP));
Void rgSCHDhmHqPDelFrmInUseLst ARGS (( RgSchDlHqProcCb *hqP));

RgSchDlHqEnt *rgSCHDhmHqEntInit ARGS((RgSchCellCb *cell));
S16 rgSCHDhmGetAvlHqProc ARGS((RgSchCellCb *cell, RgSchUeCb *ue, CmLteTimingInfo timingInfo,
                                   RgSchDlHqProcCb **hqP));
Void rgSCHDhmHqRetx ARGS((RgSchDlHqEnt *hqE, CmLteTimingInfo timeInfo,
                                   RgSchDlHqProcCb *hqP));
RgSchDlHqProcCb * rgSCHDhmLastSchedHqProc ARGS((RgSchDlHqEnt *hqE));
/* CR timer changes*/
S16 rgSCHDhmGetCcchSduHqProc ARGS((RgSchUeCb *ueCb, CmLteTimingInfo timeInfo, 
                                          RgSchDlHqProcCb **hqP));
S16 rgSCHDhmGetMsg4HqProc ARGS((RgSchRaCb *raCb, CmLteTimingInfo timeInfo));
Void rgSCHDhmRlsHqProc ARGS((RgSchDlHqProcCb *hqP));
/* ccpu00118350 : Correcting NDI manipulation of Harq */
Void rgSCHDhmRlsHqpTb ARGS((RgSchDlHqProcCb *hqP, uint8_t tbIdx, Bool togNdi));
Void rgSCHUtlDlHqPTbAddToTx ARGS((RgSchDlSf *subFrm,
RgSchDlHqProcCb *hqP, uint8_t tbIdx ));
Void rgSCHDhmHqTbRetx ARGS(( RgSchDlHqEnt *hqE,
CmLteTimingInfo timingInfo, RgSchDlHqProcCb *hqP, uint8_t tbIdx));
Void rgSCHUtlDlHqPTbAddToTx ARGS((RgSchDlSf *subFrm,
RgSchDlHqProcCb *hqP, uint8_t tbIdx ));
Void rgSCHDhmHqTbRetx ARGS(( RgSchDlHqEnt *hqE,
CmLteTimingInfo timingInfo, RgSchDlHqProcCb *hqP, uint8_t tbIdx));
#ifdef RG_UNUSED
S16 rgSCHDhmGetHqProcFrmId ARGS((RgSchCellCb *cell, RgSchUeCb *ue, uint8_t idx,
                                    RgSchDlHqProcCb **hqP));
#endif
/* Changes for MIMO feature addition */
Void rgSCHDhmSchdTa ARGS((RgSchUeCb *ueCb, RgSchDlHqTbCb *tbInfo));
S16 rgSCHDhmHqFdbkInd ARGS((Void *cb, uint8_t cbType, RgSchCellCb *cellCb,
                     CmLteTimingInfo timingInfo, RgTfuHqInfo *fdbk, RgInfRlsHqInfo
                                    *rlsHqBufs,RgSchErrInfo *err));
#ifdef EMTC_ENABLE
S16 rgSCHDhmEmtcHqFdbkInd ARGS((Void *cb, uint8_t cbType, RgSchCellCb *cellCb,
                     CmLteTimingInfo timingInfo, RgTfuHqInfo *fdbk, RgInfRlsHqInfo
                                    *rlsHqBufs,RgSchErrInfo *err));
S16 rgSCHUtlAddToResLst 
(
 CmLListCp   *cp, 
 RgSchIotRes *iotRes
 );
#endif
/*CA Dev Start */
S16 rgSCHDhmPrcFdbkForTb(RgSchCellCb *cell,RgSchUeCb  *ue,
                               RgSchDlHqProcCb *hqP,RgSchDlSf *sf,Bool isMsg4,
                               uint16_t rnti,uint8_t tbCnt,CmLteTimingInfo timingInfo, uint8_t isAck,
                               RgInfRlsHqInfo *rlsHqBufs,RgSchErrInfo *err
                               );
/*CA Dev End */
Void rgSCHDhmRgrUeCfg ARGS((RgSchCellCb *cellCb, RgSchUeCb *ueCb,
                                    RgrUeCfg *ueCfg, RgSchErrInfo *err));
Void rgSCHDhmRgrUeRecfg ARGS((RgSchCellCb *cellCb, RgSchUeCb *ueCb,
                                    RgrUeRecfg *ueCfg, RgSchErrInfo *err));
Void rgSCHDhmRgrCellCfg ARGS((RgSchCellCb *cellCb, RgrCellCfg *cellCfg,
                                    RgSchErrInfo *err));
Void rgSCHDhmRgrCellRecfg ARGS((RgSchCellCb *cellCb, RgrCellRecfg
                                    *cellRecfg, RgSchErrInfo *err));
Void rgSCHDhmFreeUe ARGS((RgSchUeCb *ueCb));
Void rgSCHDhmUpdTa ARGS((RgSchCellCb *cellCb, RgSchUeCb *ueCb, uint8_t ta));
Void rgSCHDhmProcTAExp ARGS((RgSchUeCb *ue));
/* Changes for MIMO feature addition */
S16 rgSCHDhmAddLcData ARGS((Inst inst, RgSchLchAllocInfo *lchData,
                                    RgSchDlHqTbCb *tbInfo));
S16 rgSCHDhmRlsDlsfHqProc ARGS((RgSchCellCb *cellCb, CmLteTimingInfo
timingInfo));

#ifdef LTE_TDD
S16 rgSCHDhmTddRlsSubFrm ARGS((RgSchCellCb *cell, CmLteTimingInfo uciTimingInfo));
S16 rgSCHCfgVldtTddDrxCycCfg ARGS((RgSchCellCb  *cell, uint16_t drxCycle, 
                                    uint8_t onDurTmr, uint16_t offSet));
#endif
/* Added support for SPS*/
#ifdef LTEMAC_SPS
S16 rgSCHDhmGetHqProcFrmId ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue,
uint8_t               idx,
RgSchDlHqProcCb       **hqP
));
#endif /* LTEMAC_SPS */
/* Freeing up the HARQ proc blocked for
 * indefinite time in case of Retx */
S16 rgSCHDhmDlRetxAllocFail ARGS((
RgSchUeCb        *ue,
RgSchDlHqProcCb  *proc
));
/* MS_WORKAROUND for ccpu00122893 temp fix Incorrect HqProc release was done instead of
 * a Harq Entity reset. Fixing the same */
Void rgSCHDhmHqEntReset ARGS((
   RgSchDlHqEnt      *hqE
));
/* Measurement GAP and ACK NACK */

S16 rgSCHMeasGapANRepUeCfg ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgrUeCfg            *ueCfg
));
S16 rgSCHMeasGapANRepUeRecfg ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgrUeRecfg          *ueRecfg
));
/* ccpu00133470- Added extra argument to identify UE DEL*/
Void rgSCHMeasGapANRepUeDel ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
Bool                isUeDel
));
S16 rgSCHMeasGapANRepTtiHndl ARGS((
RgSchCellCb         *cell
));
S16 rgSCHMeasGapANRepGetDlInactvUe ARGS((
RgSchCellCb         *cell,
CmLListCp           *dlInactvUeLst
));
S16 rgSCHMeasGapANRepGetUlInactvUe ARGS((
RgSchCellCb         *cell,
CmLListCp           *ulInactvUeLst
));
Void rgSCHMeasGapANRepDlInactvTmrExpry ARGS((
RgSchUeCb           *ue,
uint8_t              tmrEvnt
));
Void rgSCHMeasGapANRepUlInactvTmrExpry ARGS((
RgSchUeCb           *ue,
uint8_t             tmrEvnt
));
Void rgSCHMeasGapANRepTmrExpry ARGS((
RgSchUeCb           *ue
));
Void rgSCHAckNakRepTmrExpry ARGS((
RgSchUeCb           *ue
));
Void rgSCHAckNakRepSndHqFbkRcpReq ARGS((
RgSchCellCb         *cell,
RgSchDlSf           *dlSf,
CmLteTimingInfo     timingInfo));

Void rgSCHAckNakRepAddToQ ARGS((
RgSchCellCb       *cell,
RgSchDlSf         *crntDlSf));

/*
 * SCH Util APIs
 */
#ifdef LTEMAC_SPS
Void rgSCHUtlHdlCrcInd ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue,
CmLteTimingInfo timingInfo
));
#endif

#ifdef LTE_L2_MEAS
 S16 rgSCHUtlValidateMeasReq ARGS ((RgSchCellCb *cellCb,
         LrgSchMeasReqInfo *schL2MeasInfo,
         RgSchErrInfo      *err
         ));
S16 rgSchL2mSndCfm ARGS((Pst   *pst,
         RgSchL2MeasCb   *measCb,
         LrgSchMeasReqInfo *measInfo,
         Bool              isErr
));
 S16 rgSchFillL2MeasCfm ARGS((
         RgSchCellCb       *cell,
         RgSchL2MeasCb     *measCb,
         LrgSchMeasCfmInfo *cfm,
         uint32_t          measTime
));
Void rgSchL2mFillCfmPst ARGS((
         Pst    *pst,
         Pst    *cfmPst,
         LrgSchMeasReqInfo *measInfo
));
S16 rgSCHL2Meas ARGS((
         RgSchCellCb  *cell,
         uint8_t      isCalrCrcInd
));
#endif /* LTE_L2_MEAS */
/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
F64 rgSCHUtlPower ARGS
((
F64 x,
F64 n
));

 uint32_t rgSCHUtlParse ARGS
  ((
 uint8_t *buff,
 uint8_t startPos,
 uint8_t endPos,
 uint8_t buffSize
 ));

  uint8_t rgSCHUtlFindDist ARGS
((
uint16_t    crntTime,
uint16_t    tempIdx
));
#endif
Bool rgSCHUtlPdcchAvail ARGS((RgSchCellCb *cell, RgSchPdcchInfo
        *pdcchInfo, CmLteAggrLvl aggrLvl, RgSchPdcch **pdcch));
Void rgSCHUtlPdcchPut ARGS((RgSchCellCb *cell, RgSchPdcchInfo *pdcchInfo,
        RgSchPdcch *pdcch));
#ifdef LTE_TDD
/* Changes for passing iPhich at TFU interface*/
S16 rgSCHUtlAddPhich  ARGS((RgSchCellCb *cellCb, CmLteTimingInfo frm,
         uint8_t hqFeedBack, uint8_t nDmrs, uint8_t rbStart, uint8_t iPhich));
#else
S16 rgSCHUtlAddPhich  ARGS((RgSchCellCb *cellCb, CmLteTimingInfo frm,
         uint8_t hqFeedBack, uint8_t nDmrs, uint8_t rbStart,Bool isForMsg3));
#endif
RgSchDlSf* rgSCHUtlSubFrmGet ARGS((RgSchCellCb *cell,
         CmLteTimingInfo frm));
Void rgSCHUtlSubFrmPut ARGS((RgSchCellCb *cell, RgSchDlSf *sf));
uint8_t rgSCHUtlLog32bitNbase2 ARGS((uint32_t n));
/* Added support for SPS*/


#ifdef LTEMAC_SPS
RgSchDlHqProcCb * rgSCHDhmSpsDlGetHqProc ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
CmLteTimingInfo  timingInfo));
#endif
#ifdef LTE_TDD
uint8_t rgSCHUtlCalcNCce ARGS((uint8_t bw, RgrPhichNg ng, uint8_t cfi, uint8_t mPhich,
                                 uint8_t numAntna, Bool isEcp));
#else
uint8_t rgSCHUtlCalcNCce ARGS((uint8_t bw, RgrPhichNg ng, uint8_t cfi, uint8_t numAntna, Bool
isEcp));
#endif
#ifdef LTE_TDD
/* Changes for passing iPhich at TFU interface*/
S16 rgSCHUtlGetPhichInfo ARGS((RgSchUlHqProcCb *hqProc, uint8_t *rbStartRef,
         uint8_t *nDmrsRef, uint8_t *iPhich));
#else
S16 rgSCHUtlGetPhichInfo ARGS((RgSchUlHqProcCb *hqProc, uint8_t *rbStartRef,
         uint8_t *nDmrsRef));
#endif
/*  Added changes of TFU_UPGRADE */
#ifndef TFU_UPGRADE
/* To include the length and ModOrder in DataRecp Req. */
/* Updating NDI and HARQ proc Id */
S16 rgSCHUtlAllocRcptInfo ARGS((RgSchUlAlloc *alloc, CmLteRnti *rnti,
           uint8_t *iMcsRef, uint8_t *rbStartRef, uint8_t *numRbRef, uint8_t *rvRef, uint16_t *size,
           TfuModScheme *modType,Bool *isRtx,
uint8_t *nDmrs,
Bool *ndi,
uint8_t *hqPId));
#else
S16 rgSCHUtlAllocRcptInfo ARGS((
         RgSchCellCb             *cell,
         RgSchUlAlloc            *alloc,
         CmLteTimingInfo         *timeInfo,
         TfuUeUlSchRecpInfo      *recpReq
         ));
#endif /* TFU_UPGRADE */

S16 rgSCHUtlRgrCellCfg ARGS((RgSchCellCb *cell, RgrCellCfg *cellCfg,
         RgSchErrInfo *errInfo));
S16 rgSCHUtlRgrCellRecfg ARGS((RgSchCellCb *cell, RgrCellRecfg *recfg,
         RgSchErrInfo *errInfo));
S16 rgSCHUtlFreeCell ARGS((RgSchCellCb *cell));
S16 rgSCHUtlRgrUeCfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgrUeCfg *cfg, RgSchErrInfo *err));
S16 rgSCHUtlRgrLcCfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *dl, RgrLchCfg *cfg,RgSchErrInfo *errInfo));
S16 rgSCHUtlRgrLcDel ARGS((RgSchCellCb *cell, RgSchUeCb *ue, 
         CmLteLcId lcId, uint8_t lcgId));
S16 rgSCHUtlRgrLcRecfg ARGS ((RgSchCellCb *cell,RgSchUeCb *ue,
      RgSchDlLcCb *dlLc,RgrLchRecfg *recfg,RgSchErrInfo *err));
S16 rgSCHUtlRgrLcgCfg ARGS ((RgSchCellCb *cell,RgSchUeCb *ue,
         RgrLcgCfg *cfg,RgSchErrInfo *errInfo));
S16 rgSCHUtlRgrLcgRecfg ARGS ((RgSchCellCb *cell,RgSchUeCb *ue,
         RgrLcgRecfg *recfg,RgSchErrInfo *err));
Void rgSCHUtlRgrLcgDel ARGS ((RgSchCellCb *cell,RgSchUeCb *ue,
         uint8_t lcgId));
Void rgSCHUtlDlCqiInd ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         TfuDlCqiRpt *dlCqiInd, CmLteTimingInfo timingInfo));

/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
Void rgSCHUtlRawCqiInd ARGS
((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
TfuRawCqiRpt*     rawCqiRpt,
CmLteTimingInfo    timingInfo
));

Void rgSCHUtlSrsInd ARGS
((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
TfuSrsRpt*     srsRpt,
CmLteTimingInfo    timingInfo
));
S16 rgSCHUtlGetCfgPerOff ARGS
((
RgSchPerTbl     tbl,
uint16_t        cfgIdx, 
uint16_t        *peri, 
uint16_t        *offset
));
#endif

Void rgSCHUtlDoaInd ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         TfuDoaRpt *doaInd));
Void rgSCHUtlDlTARpt ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
/* Changes for MIMO feature addition */
Void rgSCHUtlDlRlsSubFrm ARGS((RgSchCellCb *cell, CmLteTimingInfo subFrm));
Void rgSCHUtlDlProcAddToRetx ARGS((RgSchCellCb *cell,
         RgSchDlHqProcCb *hqP));
S16 rgSCHUtlRegSch ARGS((uint8_t schIdx, RgSchdApis *apis));
Void rgSCHUtlDlHqProcAddToTx ARGS((RgSchDlSf *subFrm, RgSchDlHqProcCb *hqP));
/* Changes for MIMO feature addition */
Void rgSCHUtlDlHqPTbRmvFrmTx ARGS((RgSchDlSf *subFrm,
         RgSchDlHqProcCb *hqP, uint8_t tbIdx, Bool isRepeating));
S16 rgSCHUtlRgrUeRecfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgrUeRecfg *recfg, RgSchErrInfo *err));
Void rgSCHUtlFreeDlLc ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *dlLc));
Void rgSCHUtlFreeUlLc ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchUlLcCb *ulLc));
Void rgSCHUtlFreeUe ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
Void rgSCHUtlDlDedBoUpd ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *svc));
#ifdef RG_UNUSED
S16 rgSCHUtlUpdUlHqProc ARGS((RgSchCellCb *cell, RgSchUlHqProcCb *curProc,
         RgSchUlHqProcCb *oldProc));
#endif
/* PHR handling for MSG3 */
Void rgSCHUtlRecMsg3Alloc ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchRaCb *raCb));
S16 rgSCHUtlContResUlGrant ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchErrInfo *err));
S16 rgSCHUtlSrRcvd ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         CmLteTimingInfo, RgSchErrInfo *err));
Void rgSCHUtlUpdBsrShort ARGS((RgSchCellCb *cell, RgSchUeCb *ue, uint8_t lcgId,
         uint8_t bsr, RgSchErrInfo *err));
Void rgSCHUtlUpdBsrTrunc ARGS((RgSchCellCb *cell, RgSchUeCb *ue, uint8_t lcgId,
         uint8_t bsr, RgSchErrInfo *err));
Void rgSCHUtlUpdBsrLong ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         uint8_t bsr1,uint8_t bsr2,uint8_t bsr3,uint8_t bsr4, RgSchErrInfo *err));
S16 rgSCHUtlUpdPhr ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         uint8_t phr, RgSchErrInfo *err));
S16 rgSCHUtlUpdExtPhr ARGS(( RgSchCellCb  *cell, RgSchUeCb *ue,
RgInfExtPhrCEInfo * extPhr, RgSchErrInfo *err));
S16 rgSCHUtlDataRcvd ARGS((RgSchCellCb *cell, RgSchUeCb *ue, uint8_t numLc,
         RgSchUlLcCb *lcArr[], uint16_t bytesArr[], RgSchErrInfo *err));
Void rgSCHUtlUlCqiInd ARGS(( RgSchCellCb *cell, RgSchUeCb *ue,
                                    TfuUlCqiRpt *ulCqiInfo));
Void rgSCHUtlPucchDeltaPwrInd ARGS(( RgSchCellCb *cell, RgSchUeCb *ue,
         S8 delta));
Void rgSCHUtlUeReset ARGS(( RgSchCellCb *cell, RgSchUeCb *ue));
Void rgSCHUtlUlHqProcForUe ARGS((RgSchCellCb *cell, CmLteTimingInfo frm,
         RgSchUeCb *ue, RgSchUlHqProcCb **procRef));
RgSchUlAlloc *rgSCHUtlFirstRcptnReq ARGS((RgSchCellCb *cell));
RgSchUlAlloc *rgSCHUtlNextRcptnReq ARGS((RgSchCellCb *cell,
         RgSchUlAlloc *alloc));
RgSchUlAlloc *rgSCHUtlFirstHqFdbkAlloc ARGS((RgSchCellCb *cell, uint8_t idx));
RgSchUlAlloc *rgSCHUtlNextHqFdbkAlloc ARGS((RgSchCellCb *cell,
         RgSchUlAlloc *alloc, uint8_t idx));
S16 rgSCHUtlTfuBndReq ARGS((Inst inst, SuId suId, SpId spId));
S16 rgSCHUtlTfuUBndReq ARGS((Inst inst, RgSchLowSapCfgInfo sapCfg, Reason reason));
#ifdef EMTC_ENABLE
S16 rgSCHEmtcUtlResetSfAlloc ARGS((RgInfSfAlloc *sfAlloc,
            Bool resetCmnLcInfo, Bool restAlloc));
#endif
S16 rgSCHUtlResetSfAlloc ARGS((RgInfSfAlloc *sfAlloc,
            Bool resetCmnLcInfo, Bool restAlloc));
S16 rgSCHUtlGetSfAlloc ARGS((RgSchCellCb *cell));
S16 rgSCHUtlPutSfAlloc ARGS((RgSchCellCb *cell));
S16  rgSCHUtlAllocSBuf  ARGS((Inst inst, Data **pData, Size size));
/* ccpu00117052 - MOD - Passing double pointer
for proper NULLP assignment*/
Void rgSCHUtlFreeSBuf   ARGS((Inst inst, Data **data, Size size));
Void rgSCHUtlFillDgnParams ARGS((Inst inst, RgUstaDgn *dgn,uint8_t dgnType));
Void rgSCHUtlGetPstToLyr ARGS((Pst *pst,RgSchCb *schCb,Inst macInst));
S16 rgSCHUtlFillRgInfCmnLcInfo ARGS((RgSchDlSf *sf,RgInfSfAlloc *sfAlloc,
                                 CmLteLcId lcId, Bool sendInd));
S16 rgSCHUtlFillRgInfRarInfo ARGS((RgSchDlSf *sf,RgInfSfAlloc *sfAlloc,RgSchCellCb *cell));
S16 rgSCHUtlFillPdschDciInfo ARGS((TfuPdschDciInfo *pdschDci,TfuDciInfo
        *pdcchDci));
  /* CA dev Start */
Void rgSCHUtlFillRgInfUeInfo ARGS((RgSchDlSf*, RgSchCellCb *cell, CmLListCp *dlDrxInactvTmrLst,
                                          CmLListCp *dlInActvLst, CmLListCp *ulInActvLst));
  /* CA dev End */
S16 rgSCHUtlUpdSch ARGS((RgInfSfDatInd *subfrmInfo, RgSchCellCb *cellCb,
               RgSchUeCb *ueCb, RgInfUeDatInd *pdu,RgSchErrInfo *err));
S16 rgSCHUtlHndlCcchBoUpdt ARGS((RgSchCellCb *cell,RgInfCmnBoRpt *boRpt));
S16 rgSCHUtlHndlBcchPcchBoUpdt ARGS((RgSchCellCb *cell,RgInfCmnBoRpt
        *boUpdt));
S16 rgSCHUtlRgrBndCfm ARGS ((Inst inst, SuId suId,uint8_t status));
/* Added for sending TTI tick to RRM */
#ifdef RGR_RRM_TICK
S16 rgSCHUtlRgrTtiInd ARGS ((RgSchCellCb *cell, RgrTtiIndInfo *ttiInd));
#endif
S16 schSendCfgCfm ARGS ((Region reg, Pool pool, \
         RgrCfgTransId transId, uint8_t status));
S16 rgSCHUtlProcMsg3 ARGS((RgInfSfDatInd *subfrmInfo, RgSchCellCb  *cellCb,
            RgSchUeCb   *ueCb, CmLteRnti  rnti,RgInfUeDatInd  *pdu,
            RgSchErrInfo *err ));
#ifdef RG_PHASE_2
S16 rgSCHUtlTfuGrpPwrCntrlReq ARGS((Inst inst,S16  sapId,
         TfuGrpPwrCntrlReqInfo *grpPwrCntrlReq));
#endif
S16 rgSCHUtlTfuCntrlReq ARGS((Inst  inst, S16 sapId,
         TfuCntrlReqInfo *cntrlReq));
S16 rgSCHUtlTfuRecpReq ARGS((Inst inst, S16  sapId,
         TfuRecpReqInfo *recpReq));
S16 rgSCHUtlValidateTfuSap ARGS((Inst  inst,SuId  suId));
S16 rgSCHUtlAllocEventMem ARGS((Inst  inst,Ptr  *memPtr,Size  memSize));
S16 rgSCHUtlGetEventMem ARGS((Ptr *ptr,Size  len,Ptr   memCpa));
S16 rgSCHUtlGetRlsHqAlloc ARGS((RgSchCellCb *cell));
S16 rgSCHUtlPutRlsHqAlloc ARGS((RgSchCellCb *cell));

S16 rgSCHUtlDlActvtUe ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
S16 rgSCHUtlUlActvtUe ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
Void rgSCHUtlHdlUlTransInd ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                                   CmLteTimingInfo timingInfo));
#ifdef TFU_UPGRADE
Void rgSCHUtlUpdACqiTrigWt ARGS((RgSchUeCb *ue,RgSchUeCellInfo *sCellInfo, uint8_t isAck));
#endif
/* Nprb indication at PHY for common Ch */
/* Corrected allocation for common channels */
S32 rgSCHUtlGetAllwdCchTbSz ARGS((uint32_t bo, uint8_t *nPrb, uint8_t *mcs
));
/* CR timer changes*/
S16 rgSCHUtlUpdtBo ARGS((RgSchCellCb  *cell,
                               RgInfCmnBoRpt     *staRsp));
S16 rgSCHUtlAddUeToCcchSduLst ARGS(
                          (RgSchCellCb       *cell,
                            RgSchUeCb         *ueCb));
#ifdef EMTC_ENABLE
S16 rgSCHUtlAddUeToEmtcCcchSduLst ARGS(
      (RgSchCellCb       *cell,
       RgSchUeCb         *ueCb));

S16 rgSCHRamRmvFrmEmtcRaInfoSchdLst ARGS((RgSchCellCb   *cell, RgSchRaCb *raCb));
Void rgSCHRamEmtcDelRaCb ARGS((RgSchCellCb *cell, RgSchRaCb *raCb));
S16 rgSCHRamEmtcUpdtBo        ARGS((RgSchCellCb *cell, RgSchRaCb *raCb,
                                    RgInfCmnBoRpt *staRsp));
#endif
/* Added for SI Enhancement*/
#ifdef RGR_SI_SCH
Void rgSCHUtlPutSiInfo ARGS((RgSchCellCb *cell));
Void rgSCHUtlFreeWarningSiSeg  ARGS((Region reg,Pool pool,
                                            CmLListCp *siPduLst));
Void rgSCHUtlFreeWarningSiPdu  ARGS((RgSchCellCb *cell));
Buffer *rgSCHUtlGetWarningSiPdu ARGS((RgSchCellCb *cell));
S16 rgSCHUtlGetMcsAndNPrb ARGS((RgSchCellCb *cell, uint8_t *nPrb, uint8_t *mcs, MsgLen *msgLen));
S16 rgSCHUtlCalMcsAndNPrb ARGS((RgSchCellCb *cell, uint8_t cfgType, MsgLen msgLen, uint8_t siId));
#endif/*RGR_SI_SCH*/

#ifdef LTE_TDD
S16 rgSCHUtlAllocUeANFdbkInfo ARGS((RgSchUeCb *ue,uint8_t servCellIdx));
Void rgSCHUtlDelUeANFdbkInfo ARGS((RgSchUeCb *ue,uint8_t servCellIdx));
S16 rgSCHUtlInitUeANFdbkInfo ARGS((RgSchTddANInfo *anInfo));
RgSchTddANInfo* rgSCHUtlGetUeANFdbkInfo ARGS((RgSchUeCb *ueCb, CmLteTimingInfo *timeInfo,uint8_t servCellIdx));
uint8_t  rgSCHUtlGetDlSfIdx ARGS((RgSchCellCb *cell, CmLteTimingInfo *timeInfo));
Void rgSCHUtlPrachCfgInit ARGS((RgSchCellCb *cell, RgrCellCfg *cellCfg ));
Void rgSCHUtlGetNxtDlSfInfo ARGS((CmLteTimingInfo curDlTime, RgSchCellCb  *cell, RgSchDlSf *dlSf, RgSchDlSf **nxtDlsf, CmLteTimingInfo *nxtDlTime));
Void rgSCHUtlGetPrevDlSfInfo ARGS((RgSchCellCb * cell, CmLteTimingInfo curDlTime, CmLteTimingInfo *prevDlTime, uint8_t *numSubfrm));
#endif
Void rgSCHCmnDlSch ARGS
((
RgSchCellCb        *cell
));
Void rgSCHCmnSndCnsldtInfo ARGS
((
RgSchCellCb  *cell
));
Void rgSCHCmnCnsldtSfAlloc ARGS
((
RgSchCellCb  *cell
));

/* Added support for SPS*/
Void rgSCHCmnDlAllocFnlz ARGS
((
RgSchCellCb  *cell
));

#ifdef LTEMAC_SPS
Void rgSCHUtlDlRelPdcchFbk ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
Bool               isAck
));

Void rgSCHUtlDlProcAck ARGS((
RgSchCellCb        *cell,
RgSchDlHqProcCb    *hqP
));
S16 rgSCHUtlSpsRelInd ARGS((
RgSchCellCb        *cellCb,
RgSchUeCb          *ueCb,
Bool               isExplRel
));

Void rgSCHCmnDlSch ARGS
((
RgSchCellCb        *cell
));

S16 rgSCHUtlSpsActInd ARGS((
RgSchCellCb        *cellCb,
RgSchUeCb          *ueCb,
uint16_t           spsSduSize
));

Void rgSCHUtlHdlCrcFailInd ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue,
CmLteTimingInfo timingInfo
));

Void rgSCHUtlHdlCrntiCE ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue
));

#endif /* LTEMAC_SPS*/

/******* </AllocHolesMemMgmnt>: START *****/
S16 rgSCHUtlUlSfInit ARGS((
         RgSchCellCb  *cell,
         RgSchUlSf    *sf,
         uint8_t      idx,
         uint8_t      maxUePerSf
         ));
Void rgSCHUtlUlSfDeinit ARGS((
         RgSchCellCb *cell,
         RgSchUlSf   *sf
         ));
RgSchUlAlloc *rgSCHUtlUlAllocGetHole ARGS((
         RgSchUlSf *sf,
         uint8_t numRb,
         RgSchUlHole *hole
         ));
RgSchUlAlloc *rgSCHUtlUlAllocGetCompHole ARGS((
         RgSchUlSf *sf,
         RgSchUlHole *hole
         ));
RgSchUlAlloc *rgSCHUtlUlAllocGetPartHole ARGS((
         RgSchUlSf *sf,
         uint8_t numRb,
         RgSchUlHole *hole
         ));
Void rgSCHUtlUlAllocRls ARGS((
         RgSchUlSf *sf,
         RgSchUlAlloc *alloc
         ));

/* UL_ALLOC_ENHANCEMENT */
Void rgSCHUtlUlAllocRelease ARGS((
         RgSchUlAlloc *alloc
         ));
RgSchUlAlloc *rgSCHUtlUlAllocFirst ARGS((
         RgSchUlSf *sf
         ));
RgSchUlAlloc *rgSCHUtlUlAllocNxt ARGS((
         RgSchUlSf *sf,
         RgSchUlAlloc *alloc
         ));
RgSchUlHole *rgSCHUtlUlHoleFirst ARGS((
         RgSchUlSf *sf
         ));
RgSchUlHole *rgSCHUtlUlHoleNxt ARGS((
         RgSchUlSf *sf,
         RgSchUlHole *hole
         ));
RgSchUlAlloc *rgSCHUtlUlAllocGetAdjNxt ARGS((
         RgSchUlAllocDb *db,
         RgSchUlAlloc *prv
         ));
RgSchUlAlloc *rgSCHUtlUlAllocGetFirst ARGS((
         RgSchUlAllocDb *db
         ));
Void rgSCHUtlUlHoleAddAlloc ARGS((
         RgSchUlSf *sf,
         RgSchUlAlloc *alloc
         ));
/* UL_ALLOC_ENHANCEMENT */
Void rgSCHUtlUlHoleAddAllocation ARGS((
         RgSchUlAlloc *alloc
         ));

Void rgSCHUtlUlHoleJoin ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *prv,
         RgSchUlHole *nxt,
         RgSchUlAlloc *alloc
         ));
Void rgSCHUtlUlHoleExtndRight ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *prv,
         RgSchUlAlloc *alloc
         ));
Void rgSCHUtlUlHoleExtndLeft ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *nxt,
         RgSchUlAlloc *alloc
         ));
Void rgSCHUtlUlHoleNew ARGS((
         RgSchUlHoleDb *db,
         RgSchUlAlloc *alloc
         ));
Void rgSCHUtlUlHoleUpdAllocLnks ARGS((
         RgSchUlHole *hole,
         RgSchUlAlloc *prvAlloc,
         RgSchUlAlloc *nxtAlloc
         ));
Void rgSCHUtlUlHoleIns ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *hole
         ));
Void rgSCHUtlUlHoleIncr ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *hole
         ));
Void rgSCHUtlUlHoleDecr ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *hole
         ));
Void rgSCHUtlUlHoleRls ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *hole
         ));
S16 rgSCHUtlUlAllocMemInit ARGS((
         RgSchCellCb     *cell,
         RgSchUlAllocMem *mem,
         uint8_t maxAllocs
         ));
Void rgSCHUtlUlAllocMemDeinit ARGS((
         RgSchCellCb     *cell,
         RgSchUlAllocMem *mem
         ));
S16 rgSCHUtlUlHoleMemInit ARGS((
         RgSchCellCb    *cell,
         RgSchUlHoleMem *mem,
         uint8_t maxHoles,
         RgSchUlHole **holeRef
         ));
Void rgSCHUtlUlHoleMemDeinit ARGS((
         RgSchCellCb    *cell,
         RgSchUlHoleMem *mem
         ));
RgSchUlAlloc *rgSCHUtlUlAllocMemGet ARGS((
         RgSchUlAllocMem *mem
         ));
Void rgSCHUtlUlAllocMemRls ARGS((
         RgSchUlAllocMem *mem,
         RgSchUlAlloc *alloc
         ));
RgSchUlHole *rgSCHUtlUlHoleMemGet ARGS((
         RgSchUlHoleMem *mem
         ));
Void rgSCHUtlUlHoleMemRls ARGS((
         RgSchUlHoleMem *mem,
         RgSchUlHole *hole
         ));
RgSchUlAlloc *rgSCHUtlUlGetSpfcAlloc ARGS((
         RgSchUlSf       *sf,
         uint8_t         startSb,
         uint8_t         numSb
));
/******* </AllocHolesMemMgmnt>: END *****/

/* DRX function declarations */
S16 rgSCHDrxCellCfg ARGS((RgSchCellCb *cell, RgrCellCfg *cellCfg));
Void rgSCHDrxCellDel ARGS((RgSchCellCb *cell));
S16 rgSCHDrxUeCfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                              RgrUeCfg *ueCfg));
#ifdef RGR_V2
S16 rgSCHDrxUeReCfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                              RgrUeRecfg *ueCfg));
#endif
S16 rgSCHDrxUeDel ARGS((RgSchCellCb *cell,RgSchUeCb *ue));
Void rgSCHDrxTtiInd ARGS ((RgSchCellCb *cell));

S16 rgSCHDrxSfAlloc ARGS ((RgSchCellCb *cellCb, RgSchDlSf
                           *dlSf));
S16 rgSCHDrxDlTrnsFail ARGS((RgSchCellCb *cell, RgSchDlHqProcCb
                             *dlHq));
Void rgSCHDrxDedRa ARGS((RgSchCellCb *cellCb, RgSchUeCb* ueCb));
S16 rgSCHDrxSrInd ARGS((RgSchCellCb *cell,RgSchUeCb *ue));

Void rgSCHDrxStrtInActvTmr ARGS((RgSchCellCb *cell,
                                       CmLListCp *ueLst,
                                       uint8_t direction));
S16 rgSCHUtlGetDrxSchdUesInDl ARGS((RgSchCellCb *cellCb,
                                        RgSchUeCb       *ueCb,
                                        RgSchDlHqProcCb *dlHq,
                                        RgInfUeAlloc    *allocInfo,
                                        CmLListCp       *dlDrxInactvTmrLst,
                                        CmLListCp       *dlInActvLst,
                                        CmLListCp       *ulInActvLst));
Void rgSCHDrxStartHarqRTTTmr ARGS((RgSchCellCb     *cell,
                                          RgSchDlHqProcCb *hqP,
                                          uint8_t              tbCnt));
Void rgSCHDrxUeHqReset ARGS((RgSchCellCb   *cell,
                                   RgSchUeCb     *ue,
                                   RgSchDlHqEnt  *hqE,
                                   uint8_t       cellIdx));

#ifdef TFU_UPGRADE
#ifdef LTE_TDD
const RgSchTddCellSpSrsSubfrmTbl rgSchTddCellSpSrsSubfrmTbl;
#else
const RgSchFddCellSpSrsSubfrmTbl rgSchFddCellSpSrsSubfrmTbl;
#endif
#endif

#ifdef LTEMAC_HDFDD
S16 rgSCHHdFddUeCfg ARGS((
         RgSchCellCb  *cellCb,
         RgSchUeCb *ueCb,
         Bool hdFdd));
S16 rgSCHHdFddUeDel  ARGS((
         RgSchCellCb *cellCb,
         RgSchUeCb *ueCb));
Void rgSCHCmnHdFddPtUlMrk ARGS((
         RgSchCellCb *cellCb));
Void rgSCHCmnHdFddChkUlAllow ARGS((
         RgSchCellCb *cellCb,
         RgSchUeCb *ueCb,
         uint8_t *flag));
Void rgSCHCmnHdFddChkDlAllow ARGS((
         RgSchCellCb *cellCb,
         RgSchUeCb *ueCb,
         Bool *flag));
Void rgSCHCmnHdFddChkNackAllow ARGS((
         RgSchCellCb *cellCb,
         RgSchUeCb *ueCb,
         CmLteTimingInfo timInfo,
         Bool *flag));
Void rgSCHCmnHdFddUpdULMark ARGS((
         RgSchCellCb *cellCb,
         RgSchUeCb *ueCb));
Void rgSCHCmnHdFddUpdDLMark ARGS((
         RgSchCellCb *cellCb,
         RgSchUeCb *ueCb));
Void rgSCHHdFddGetSfn ARGS((
         uint16_t *sfn,
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
uint8_t            numCqiRept
));
#endif /* End of RGR_CQI_REPT */
S16 rgSCHUtlRgrUeStaInd ARGS((
RgSchCellCb            *cell,
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

TfuAckNackMode rgSchUtlGetFdbkMode ARGS((
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
uint8_t       status
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

uint8_t rgSchUtlCfg0ReTxIdx ARGS((
RgSchCellCb    *cell,
CmLteTimingInfo phichTime,
uint8_t         hqFdbkIdx
));

S16 rgSCHUtlBuildNSendLcgReg ARGS((
RgSchCellCb    *cell,
CmLteRnti      crnti,
uint8_t             lcgId,
Bool           isGbr
));

Void rgSCHUtlPdcchInit ARGS((
         RgSchCellCb *cell,
         RgSchDlSf   *subFrm,
         uint16_t          nCce));
Void rgSCHDynCfiReCfg ARGS((
         RgSchCellCb   *cell,
         Bool          isDynCfiEnb
));
Void rgSchUtlCalcTotalPrbReq ARGS((RgSchCellCb  *cell,
                                          RgSchUeCb    *ue,
                                          uint32_t           bo,
                                          uint32_t          *prbReqrd));
uint8_t rgSchUtlGetNumSbs ARGS((
RgSchCellCb    *cell,
RgSchUeCb      *ue,
uint32_t       *numSbs
));

uint8_t rgSchUtlSortInsUeLst ARGS((
RgSchCellCb    *cell,
CmLListCp    *ueLst,
CmLList      *node,
uint8_t      subbandRequired
));
S16 rgSCHUtlResetCpuOvrLdState ARGS((
 RgSchCellCb *cell, 
 uint8_t      cnrtCpuOvrLdIns
));
Void rgSCHUtlCpuOvrLdAdjItbsCap ARGS((
 RgSchCellCb *cell
));
#ifdef TFU_UPGRADE
S16 rgSCHTomUtlPcqiSbCalcBpIdx ARGS((
CmLteTimingInfo  crntTimInfo,
RgSchUeCb        *ueCb,
RgSchUePCqiCb   *cqiCb
));

#ifdef LTE_ADV
S16 rgSCHUtlSCellHndlCqiCollsn ARGS((
RgSchUePCqiCb     *cqiCb
));

S16 rgSCHUtlSCellHndlRiCollsn ARGS((
RgSchUePCqiCb     *cqiCb
));
   
#endif/*LTE_ADV*/
#endif/*TFU_UPGRADE*/

Void rgSCHTomUtlGetTrigSet ARGS((
   RgSchCellCb       *cell,
   RgSchUeCb	     *ueCb,
   uint8_t           cqiReq,
   uint8_t           *triggerSet
));

Void rgSCHUtlUpdUeDciSize ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ueCb,
Bool             isCsi2Bit
));
Void rgSCHUtlCalcDciSizes ARGS((
RgSchCellCb       *cell
));

Void rgSchCmnPreDlSch ARGS ((
 RgSchCellCb        **cell,
 uint8_t            nCell,
 RgSchCellCb        **cellLst
 ));

Void rgSchCmnPstDlSch ARGS ((
 RgSchCellCb        *cell
 ));

uint8_t rgSCHCmnGetBiIndex ARGS ((
RgSchCellCb   *cell,
uint32_t      ueCount
));

uint8_t SchSendCfgCfm(Pst *pst, RgMngmt *cfm);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __SCH__ */


/**********************************************************************
         End of file
**********************************************************************/
