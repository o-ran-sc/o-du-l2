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
 
    Name:   LTE MAC SC1 scheduler
 
    Type:   C include file
 
    Desc:   Defines required by SC1 scheduler
 
    File:   rg_sch_cmn.x
 
**********************************************************************/
/** @file rg_sch_cmn.x
@brief This file contains data structures for the common module of the scheuler.
*/

#ifndef __RGSCHCMNX__
#define __RGSCHCMNX__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*-------------------------------------*
 * Common Scheduler DataStructure START
 *-------------------------------------*/
typedef struct _rgDlSchdApis   RgDlSchdApis;
typedef struct _rgUlSchdApis   RgUlSchdApis;
typedef struct _rgDlfsSchdApis RgDlfsSchdApis;
typedef Void (*RgDlSchdInits[RGSCH_NUM_SCHEDULERS]) ARGS((RgDlSchdApis *apis));
typedef Void (*RgUlSchdInits[RGSCH_NUM_SCHEDULERS]) ARGS((RgUlSchdApis *apis));
typedef Void (*RgDlfsSchdInits[RGSCH_NUM_SCHEDULERS]) ARGS((RgDlfsSchdApis *apis));
#ifdef EMTC_ENABLE
typedef struct _rgDlEmtcSchdApis   RgDlEmtcSchdApis;
typedef Void (*RgEmtcDlSchdInits[RGSCH_NUM_EMTC_SCHEDULERS]) ARGS((RgDlEmtcSchdApis *apis));
typedef Void (*RgEmtcUlSchdInits[RGSCH_NUM_EMTC_SCHEDULERS]) ARGS((RgUlSchdApis *apis));
#endif
typedef struct rgSchCmnDlRbAllocInfo RgSchCmnDlRbAllocInfo;
typedef struct rgSchCmnUeUlAlloc RgSchCmnUeUlAlloc;
typedef struct rgSchCmnUlRbAllocInfo RgSchCmnUlRbAllocInfo;

/**
  * @brief
  * Uplink Scheduler APIs.
  */
struct _rgUlSchdApis
{
   S16 (*rgSCHRgrUlUeCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgrUeCfg *cfg, 
                                                     RgSchErrInfo *err));
   S16 (*rgSCHRgrUlUeRecfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgrUeRecfg *recfg, 
                                                     RgSchErrInfo *err));
   Void (*rgSCHFreeUlUe) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   S16 (*rgSCHRgrUlCellCfg) ARGS((RgSchCellCb *cell, RgrCellCfg *cfg, 
                                                     RgSchErrInfo *err));
   S16 (*rgSCHRgrUlCellRecfg) ARGS((RgSchCellCb *cell, RgrCellRecfg *recfg, 
                                                     RgSchErrInfo *err));
   Void (*rgSCHFreeUlCell) ARGS((RgSchCellCb *cell));
   S16 (*rgSCHRgrUlLcgCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchLcgCb *lcg, RgrLcgCfg *cfg, RgSchErrInfo *errInfo));
   S16 (*rgSCHRgrUlLcCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgrLchCfg *cfg, RgSchErrInfo *errInfo));
   S16 (*rgSCHRgrUlLcgRecfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchLcgCb *lcg, RgrLcgRecfg *recfg, RgSchErrInfo *errInfo));
   S16 (*rgSCHRgrUlLcRecfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgrLchRecfg *recfg, RgSchErrInfo *errInfo));
   Void (*rgSCHFreeUlLcg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchLcgCb *lcg));
   S16 (*rgSCHRgrUlLchDel) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, CmLteLcId lcId, uint8_t lcgId));
   Void (*rgSCHUlActvtUe) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHUpdBsrShort) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchLcgCb *ulLcg, uint8_t bsr));
   Void (*rgSCHUpdBsrTrunc) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchLcgCb *ulLcg, uint8_t bsr));
   Void (*rgSCHUpdBsrLong) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, uint8_t bsArr[]));
   Void (*rgSCHContResUlGrant) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHSrRcvd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHUlSched) ARGS((RgSchCellCb *cell, RgSchCmnUlRbAllocInfo
            *allocInfo));
   Void (*rgSCHUlRetxSched) ARGS((RgSchCellCb *cell, RgSchCmnUlRbAllocInfo  *allocInfo));
   Void (*rgSCHUlCqiInd) ARGS(( RgSchCellCb *cell, RgSchUeCb *ue, TfuUlCqiRpt *ulCqiInfo));
   S16 (*rgSCHRgrUlLcgUpd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgInfUeDatInd *datInd));
   Void (*rgSCHUlUeRefresh) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHUlUeReset) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHUlAllocFnlz) ARGS((RgSchCellCb *cell, RgSchCmnUlRbAllocInfo
                                 *allocInfo));
   Void (*rgSCHUlInactvtUes) ARGS((RgSchCellCb *cell, CmLListCp *lst));
#ifdef EMTC_ENABLE
   Void (*rgSCHUlProcAddToRetx) ARGS((RgSchCellCb *cell,RgSchUlHqProcCb *hqP));
   S16 (*rgSCHUlUeHqEntInit) ARGS((RgSchCellCb *cell, RgUeUlHqCb  *hqE));
   S16 (*rgSCHUlUeHqEntDeInit) ARGS((RgSchCellCb *cell, RgUeUlHqCb  *hqE));
#endif
};
#ifdef EMTC_ENABLE
/**
  * @brief
  * DL Scheduler APIs For EMTC.
  */
struct _rgDlEmtcSchdApis
{
   S16 (*rgSCHRgrDlUeCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgrUeCfg *cfg, 
                                                     RgSchErrInfo *err));
   S16 (*rgSCHRgrDlUeRecfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgrUeRecfg *recfg, 
                                                     RgSchErrInfo *err));
   Void (*rgSCHFreeDlUe) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   S16 (*rgSCHRgrDlCellCfg) ARGS((RgSchCellCb *cell, RgrCellCfg *cfg, 
                                                     RgSchErrInfo *err));
   S16 (*rgSCHRgrDlCellRecfg) ARGS((RgSchCellCb *cell, RgrCellRecfg *recfg, 
                                                     RgSchErrInfo *err));
   Void (*rgSCHFreeDlCell) ARGS((RgSchCellCb *cell));
   S16 (*rgSCHRgrDlLcCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *dl, RgrLchCfg *cfg, 
         RgSchErrInfo *errInfo));
   S16 (*rgSCHRgrDlLcRecfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *dl, RgrLchRecfg *recfg,
         RgSchErrInfo *errInfo));
   Void (*rgSCHFreeDlLc) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchDlLcCb *dlLc));
   Void (*rgSCHDlActvtUe) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHDlNewSched) ARGS((RgSchCellCb *cell, RgSchEmtcDlSf *cntrlDlsf,RgSchEmtcDlSf *datDlsf));
   Void (*rgSCHDlPreSched) ARGS((RgSchCellCb *cell));
   Void (*rgSCHDlPstSched) ARGS((Inst schInst));
   Void (*rgSCHDlRetxSched) ARGS((RgSchCellCb *cell, RgSchEmtcDlSf *cntrlDlsf, RgSchEmtcDlSf *datDlsf));
   Void (*rgSCHDlCeSched) ARGS((RgSchCellCb *cell, RgSchCmnDlRbAllocInfo *allocInfo));
   Void (*rgSCHDlDedBoUpd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchDlLcCb *svc));
   Void (*rgSCHDlProcAddToRetx) ARGS((RgSchCellCb *cell,RgSchDlHqProcCb *hqP));
   Void (*rgSCHDlCqiInd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, Bool isPucchInfo, Void *dlCqi));
   #ifdef TFU_UPGRADE
   Void (*rgSCHSrsInd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, TfuSrsRpt*srsInd));
   #endif 
   Void (*rgSCHDlAllocFnlz) ARGS((RgSchCellCb *cell, RgSchCmnDlRbAllocInfo
                                 *allocInfo));
   Void (*rgSCHDlUeRefresh) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHDlUeReset) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHDlInactvtUes) ARGS((RgSchCellCb *cell, CmLListCp *lst));

   S16 (*rgSCHDlUeHqEntInit) ARGS((RgSchCellCb *cell, RgSchDlHqEnt *hqE));

   S16 (*rgSCHDlUeHqEntDeInit) ARGS((RgSchCellCb *cell, RgSchDlHqEnt *hqE));
   Void (*rgSCHDlProcRmvFrmRetx) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchDlHqProcCb *hqP));
#ifdef LTE_ADV
   S16 (*rgSCHRgrSCellDlUeCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, 
                                                     RgSchErrInfo *err));
   S16 (*rgSCHRgrSCellDlUeDel) ARGS((RgSchUeCellInfo *sCellInfo, RgSchUeCb *ue));
   S16 (*rgSCHDlSCellDeactv) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   S16 (*rgSCHDlSCellActv) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
#endif
   Void (*rgSCHDlTickForPdbTrkng ) ARGS((RgSchCellCb *cell));
   S16  (*rgSCHDlFillFlwCtrlInfo) ARGS((RgSchCellCb *cell, RgInfSfAlloc *sfAlloc));
};

#endif
/**
  * @brief
  * DL Scheduler APIs.
  */
struct _rgDlSchdApis
{
   S16 (*rgSCHRgrDlUeCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgrUeCfg *cfg, 
                                                     RgSchErrInfo *err));
   S16 (*rgSCHRgrDlUeRecfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgrUeRecfg *recfg, 
                                                     RgSchErrInfo *err));
   Void (*rgSCHFreeDlUe) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   S16 (*rgSCHRgrDlCellCfg) ARGS((RgSchCellCb *cell, RgrCellCfg *cfg, 
                                                     RgSchErrInfo *err));
   S16 (*rgSCHRgrDlCellRecfg) ARGS((RgSchCellCb *cell, RgrCellRecfg *recfg, 
                                                     RgSchErrInfo *err));
   Void (*rgSCHFreeDlCell) ARGS((RgSchCellCb *cell));
   S16 (*rgSCHRgrDlLcCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *dl, RgrLchCfg *cfg, 
         RgSchErrInfo *errInfo));
   S16 (*rgSCHRgrDlLcRecfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *dl, RgrLchRecfg *recfg,
         RgSchErrInfo *errInfo));
   Void (*rgSCHFreeDlLc) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchDlLcCb *dlLc));
   Void (*rgSCHDlActvtUe) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHDlNewSched) ARGS((RgSchCellCb *cell, RgSchCmnDlRbAllocInfo *allocInfo));
   Void (*rgSCHDlPreSched) ARGS((RgSchCellCb *cell));
   Void (*rgSCHDlPstSched) ARGS((Inst schInst));
   Void (*rgSCHDlRetxSched) ARGS((RgSchCellCb *cell, RgSchCmnDlRbAllocInfo *allocInfo));
   Void (*rgSCHDlCeSched) ARGS((RgSchCellCb *cell, RgSchCmnDlRbAllocInfo *allocInfo));
   Void (*rgSCHDlDedBoUpd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchDlLcCb *svc));
   Void (*rgSCHDlProcAddToRetx) ARGS((RgSchCellCb *cell,RgSchDlHqProcCb *hqP));
   Void (*rgSCHDlCqiInd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, Bool isPucchInfo, Void *dlCqi));
   #ifdef TFU_UPGRADE
   Void (*rgSCHSrsInd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, TfuSrsRpt*srsInd));
   #endif 
   Void (*rgSCHDlAllocFnlz) ARGS((RgSchCellCb *cell, RgSchCmnDlRbAllocInfo
                                 *allocInfo));
   Void (*rgSCHDlUeRefresh) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHDlUeReset) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHDlInactvtUes) ARGS((RgSchCellCb *cell, CmLListCp *lst));

   S16 (*rgSCHDlUeHqEntInit) ARGS((RgSchCellCb *cell, RgSchDlHqEnt *hqE));

   S16 (*rgSCHDlUeHqEntDeInit) ARGS((RgSchCellCb *cell, RgSchDlHqEnt *hqE));
   Void (*rgSCHDlProcRmvFrmRetx) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchDlHqProcCb *hqP));
#ifdef LTE_ADV
   S16 (*rgSCHRgrSCellDlUeCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, 
                                                     RgSchErrInfo *err));
   S16 (*rgSCHRgrSCellDlUeDel) ARGS((RgSchUeCellInfo *sCellInfo, RgSchUeCb *ue));
   S16 (*rgSCHDlSCellDeactv) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   S16 (*rgSCHDlSCellActv) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
#endif
   Void (*rgSCHDlTickForPdbTrkng ) ARGS((RgSchCellCb *cell));
   S16  (*rgSCHDlFillFlwCtrlInfo) ARGS((RgSchCellCb *cell, RgInfSfAlloc *sfAlloc));
};

/**
  * @brief
  * DLFS Scheduler APIs.
  */
struct _rgDlfsSchdApis
{
   S16 (*rgSCHDlfsCellCfg) ARGS((RgSchCellCb *cell, RgrCellCfg *cfg, 
                                                     RgSchErrInfo *err));
   S16 (*rgSCHDlfsCellRecfg) ARGS((RgSchCellCb *cell, RgrCellRecfg *recfg, 
                                                     RgSchErrInfo *err));
   Void (*rgSCHDlfsCellDel) ARGS((RgSchCellCb *cell));
   S16 (*rgSCHDlfsUeCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                               RgrUeCfg *cfg, RgSchErrInfo *err));
   S16 (*rgSCHDlfsUeRecfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                                 RgrUeRecfg *recfg, RgSchErrInfo *err));
   Void (*rgSCHDlfsUeDel) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHDlfsDlCqiInd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                                  Bool isPucchInfo,
                                  Void *dlCqiRpt, 
                                  CmLteTimingInfo timingInfo));
   Void (*rgSCHDlfsReinitSf) ARGS((RgSchCellCb *cell, RgSchDlSf *dlSf));
   Void (*rgSCHDlfsAllocRb) ARGS((RgSchCellCb *cell, RgSchCmnDlRbAllocInfo
                                 *dlRbAllocInfo));
   /* Added for BCCH/PCCH handling */
   Void (*rgSCHDlfsBcchPcchAllocRb) ARGS((RgSchCellCb *cell, RgSchCmnDlRbAllocInfo *dlRbAllocInfo));
   Void (*rgSCHDlfsAddUeToLst) ARGS((RgSchCellCb *cell, CmLListCp *lCp, RgSchDlHqProcCb *hqP));
#ifdef LTE_ADV
   S16 (*rgSCHDlfsSCellUeCfg) ARGS((RgSchCellCb *sCell, RgSchUeCb *ueCb, RgrUeSecCellCfg *sCellCfg,RgSchErrInfo *err));
   S16 (*rgSCHDlfsSCellUeDel) ARGS((RgSchCellCb *sCell, RgSchUeCb *ueCb));
#endif
};

typedef enum rgSchCmnTpcAccVal
{
   RG_SCH_CMN_TPC_ACC_NEG_1DB = 0,
   RG_SCH_CMN_TPC_ACC_0DB     = 1,
   RG_SCH_CMN_TPC_ACC_1DB     = 2,
   RG_SCH_CMN_TPC_ACC_3DB     = 3
} RgSchCmnTpcAccVal;

typedef enum rgSchCmnTpcAbsVal
{
   RG_SCH_CMN_TPC_ABS_NEG_4DB = 0,
   RG_SCH_CMN_TPC_ABS_NEG_1DB = 1,
   RG_SCH_CMN_TPC_ABS_1DB     = 2,
   RG_SCH_CMN_TPC_ABS_4DB     = 3
} RgSchCmnTpcAbsVal;
/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
typedef enum rgSchCmnRank
{
   RG_SCH_CMN_RANK_1 = 1,
   RG_SCH_CMN_RANK_2 = 2,
   RG_SCH_CMN_RANK_3 = 3,
   RG_SCH_CMN_RANK_4 = 4
} RgSchCmnRank;
#endif

typedef struct rgSchCmnUlCqiInfo
{
   uint8_t  qm;
   uint16_t eff;      /* Efficiency in terms of bits/RE */
} RgSchCmnUlCqiInfo;

RgSchCmnUlCqiInfo rgSchCmnUlCqiTbl[RG_SCH_CMN_UL_NUM_CQI];
S8 rgSchCmnDlCqiDiffOfst[8];
/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
S8 rgSchCmnApUeSelDiffCqi[4];
S8 rgSchCmnApEnbConfDiffCqi[4];
#endif 


uint8_t rgSchCmnUlCqiToTbsTbl[RG_SCH_CMN_MAX_CP][RG_SCH_CMN_UL_NUM_CQI];

#if (LTEMAC_SPS & LTE_TDD)
/* subframe offset values to be used when twoIntervalsConfig is enabled in UL
 * SPS for a UE */
typedef S8 RgSchTddSfOffTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
RgSchTddSfOffTbl rgSchTddSfOffTbl;

#endif /* LTEMAC_SPS & LTE_TDD */

/*--------------------------*
 * SPS specific declarations 
 *---------------------------*/
#ifdef LTEMAC_SPS

/**
  * @brief
  * Downlink SPS scheduling information per UE
  */
typedef struct  rgSchCmnSpsDlUeSchdInfo
{
   uint8_t              scaledCqi;    /*!< Assumed value of CQI for transmission */
   uint16_t             actvSfTblIdx; /*!< Index into cell-wide DL SPS sub-frame 
                                      table during activation */
   CmLteTimingInfo schdKey;      /*!< Key into the list of DL SPS active 
                                      UEs: next DL SPS ocassion */
   RgSchDlRbAlloc  spsAllocInfo; /*!< Allocation information for an SPS active
                                      UE */
   uint8_t              allocN1PucchIdx;  /*!< Index value in UE's n1Pucch array 
                                      of the allocated n1Pucch */
   //Bool            pdcchPndng;      /*!< Indicates if the activaton/
    //                                     reactivation PDCCH needs to be sent 
    //                                     for this allocation */
} RgSchCmnSpsDlUeSchdInfo;

/**
  * @brief
  * Downlink stats information for SPS per UE
  */
typedef struct rgSchCmnDlUeSpsStatInfo
{
   uint32_t      numSchedSPSRnti;  /*!< Number of SPS occasions sched using SPS RNTI*/
   uint32_t      totalSPSSchedOcc; /*!< Number of SPS occasions sched 
                                     using SPS RNTI + CRNTI*/
   uint32_t      numSpsReactv;     /*!< Number of Reactivations */
   uint32_t      numSpsActv;       /*!< Number of activations */
   uint32_t      numSpsRel;        /*!< Number of Deactivations */
}RgSchCmnDlUeSpsStatInfo;

/**
  * @brief
  * Downlink information for SPS per UE
  */
typedef struct rgSchCmnDlUeSpsInfo
{
   CmLteTimingInfo prevDlBoUpdTm; /*!< BO updation interval*/
   CmLList        zeroBOSvcUesEnt;    /*!< Linked list entity for zeroBOSvcUes lst */ 
   CmLList        actvUeLstEnt; /*!< Linked List entry for DL SPS
                                       active UE list*/
   CmLList        pndngUeLstEnt;/*!< Linked List entry for UE list with
                                      pending SPS action:
                                      activation/reactivation/release */
  /* Added handling to retrnasmit RelPDCCH in case no
                     feedback is received */
   CmLList        wtngForRelFdbkUeEnt;/*!< Linked list entry for UE who
                                          have a feedback pending for
                                          Release PDCCH */
   RgSchDlLcCb    *spsSvc;       /*!< Pointer to the SPS service of the 
                                      UE */
   CmLListCp      *spsList; /*!< Pointer to the SPS list of which
                                      UE is a part */
   uint32_t             measGapMask[RG_SCH_CMN_SPS_DL_MEASGAP_32BITMASK_SIZE]; 
                                 /*!< Indicates the DL sub-frames with 
                                      ongoing measurement gap */
   uint16_t            n1PucchIdx[RG_SCH_CMN_SPS_DL_MAX_N1PUCCH_IDX_PER_UE]; 
                                 /*!< N1Pucch indices configured for the UE */
   uint8_t             actionPndng;   /*!< Indicates the action pending on the UE
                                      activation/re-activation/release */
   uint8_t             dlSpsStatus;     /*!< Indicates the current status of DL SPS */
   uint8_t             prdIdx;        /*!< DL SPS periodicity index for the
                                      configured peridicity */
   RgSchCmnSpsDlUeSchdInfo dlSpsUeSchdInfo; /*!< Scheduled info for DL SPS 
                                                 active UE */  
   Bool           isRelPdcchSent; /*!< Indicates if release PDCCH is sent for
                                       this UE. For TDD, Used while sending DAI
                                       in DCI formats 0/1/1A/1B/1D/2/2A. 
                                       For FDD, used to not repeat relPdcch 
                                       till the feddback is recieved */
   uint8_t             numRelPdcchSent; /*!< Number of times RelPdcch has been sent. */

   RgSchCmnDlUeSpsStatInfo statInfo; /*!< SPS Metric Info */
   uint8_t             dynSchedCount; /*!< To track num of consecutive times SPS BO
                                                            is sched dynamically */
   uint8_t             reducedBoCount; /*!< To track num of consecutive times BO
                                                            is lesser than SPS BO */                                                         
   uint32_t             maxChgdBo;    /* !< The Maximum of BO which is different from the 
                                                            BO for which SPS has been activated */
   uint32_t            spsSchedBo;    /* !< BO for which SPS is activated */
   Bool           isDynSched;    /* !< BO is dynamically scheduled */
} RgSchCmnDlUeSpsInfo;

/**
  * @brief
  * Downlink information for SPS per Cell
  */
typedef struct rgSchCmnSpsDlSf
{
  uint32_t rbsAlloc;         /*!< Allocated BW for this subframe (in actual number of
                             RBs) */ 
  RgSchDlSfAllocInfo spsAllocInfo; /*!< Allocation information for SPS BW */
  uint32_t n1PucchMask[RG_SCH_CMN_SPS_DL_N1PUCCH_32BITMASK_SIZE]; 
                        /*!< N1Pucch allocation mask per Sub-frame */
  uint8_t  numDlSpsActiveUes;   /*!<  number of DL SPS UEs that 
                                                                  have been activated */
} RgSchCmnSpsDlSf;

/**
  * @brief
  * SPS N1Pucch Database for the cell
  */
typedef struct rgSchCmnSpsDlN1Pucch RgSchCmnSpsDlN1Pucch;
struct rgSchCmnSpsDlN1Pucch
{
 uint16_t   idx;        /*!< Index in the n1PucchLst */
 uint16_t   n1PucchVal; /*!< Pucch Value corresponding to the index */
 uint32_t   numUes;     /*!< Count of UEs with this N1Pucch value configured */
 uint16_t   next;       /*!< Next available index */
};

/**
  * @brief
  * SPS N1Pucch Database for the cell
  */
typedef struct rgSchCmnSpsDlN1PucchDb
{
  uint16_t                   numFreeN1Pucch; /*!< Number of free n1Pucch values */
  uint16_t                   numInUseN1Pucch; /*!< Number of inUse n1Pucch values
                                          */
  RgSchCmnSpsDlN1Pucch  *freeN1PucchStart; /*!< Start for free n1Pucch list */
  RgSchCmnSpsDlN1Pucch  *inUseN1PucchStart;/*!< Start for in-use n1Pucch list
                                           */
  RgSchCmnSpsDlN1Pucch   n1PucchLst[RG_SCH_SPS_DL_MAX_N1PUCCH_PER_SF];
                                            /*!< List of cell wide n1Pucch
                                                 values*/
} RgSchCmnSpsDlN1PucchDb;

/**
  * @brief
  * Downlink information for SPS per Cell
  */
typedef struct rgSchCmnDlCellSpsInfo
{
  CmLListCp       zeroBOSvcUes;    /*!< List of SPS services which 
                                       are not sched at SPS Occasion due 
                                       to zero BO*//* REVANTH_SPS_FIX */
  CmLListCp       toBeSchdSvcs;    /*!< List of SPS services to be scheduled */
  CmLListCp       retxHqProcs;     /*!< List of SPS HARQ procs for 
                                        re-transmission: all the HARQ procs 
                                        with isSpsSvcSchd = TRUE shall be
                                        part of this list */
  CmLListCp       actvDlSpsUeLsts[RG_SCH_CMN_SPS_MAX_PRD];    
                                 /*!< Array of list of UE control blocks with
                                      DL SPS activated: index - next time of
                                      transmission */
  CmLListCp       toBeActvtdUes; /*!< List of DL SPS UEs with pending 
                                      activation/re-activation */
  CmLListCp       toBeRelUes;    /*!< List of DL SPS enabled UEs with release
                                      pending */
 /* Added handling when no feedback is received
                    for the Release PDCCH sent
  */
  CmLListCp       wtngForRelFdbkUeLst[RGSCH_NUM_SUB_FRAMES]; /*!< List of DL SPS
                                                             enabled UEs waiting
                                                             for feedback for 
                                                             Release PDCCH sent
                                                            */
  uint16_t             spsPrdLcmVal;  /*!< LCM value for all configured
                                      SPS periodicities: maxVal = 640 for FDD
                                      and (640 * 3) for TDD */
  uint8_t              lcmIdx;        /*!< Index value for computed LCM */
  RgSchCmnSpsDlSf *spsSfTbl;     /*!< DL sub-frame information for the cell*/
  RgSchCmnSpsDlN1PucchDb n1PucchDb; /*!< Database of configured n1Pucch values
                                     */  
} RgSchCmnDlCellSpsInfo;

/**
  * @brief
  *  Information per uplink SPS allocation
  */
typedef struct rgSchCmnSpsUlAlloc
{
   uint8_t             sbStart;        /*!< Starting subband of the alloc */
   uint8_t             numSb;          /*!< Num of subbands in the alloc */
} RgSchCmnSpsUlAlloc;

/**
  * @brief
  * Uplink information for SPS per subframe
  */
typedef struct rgSchCmnSpsUlSf
{
   uint32_t            ulBwBitMask[RGSCH_SPS_ULBW_MASK_LEN];    /*!< Bitmask indicating the alloc/hole info 
                                         for SPS BW. Bit set at position 'x'
                                         indicates subband 'x' is occupied */
   uint8_t             maskLen;         /*!< Length of ulBwBitMask based on numSb */
   uint8_t             numUlSpsActiveUes;    /*!< Number of UL SPS Active UEs in this Subframe */
   RgSchCmnSpsUlAlloc allocInfo;   /*!< Info per SPS Allocation - Used to mark
                                        previous allocations in a subframe */
} RgSchCmnSpsUlSf;

/**
  * @brief
  * Uplink information for SPS per Cell
  */
typedef struct rgSchCmnUlCellSpsInfo
{
   uint8_t              spsSbStart;      /*!< Starting subband of SPS BW */
   uint8_t              numSpsSb;        /*!< number of subbands for SPS */
   uint16_t             spsPrdLcmVal;    /*!< LCM value for all configured UL
                                         SPS periodicities:maxVal = 640 for FDD
                                         and (640 * 3) for TDD */ 
   RgSchCmnSpsUlSf *spsSfLst;       /*!< UL subframe information for the cell*/
   CmLListCp       actvUlSpsUeLsts[RG_SCH_CMN_SPS_MAX_PRD];    
                                    /*!< Array of list of UeCbs with
                                         UL SPS activated: index - next time of
                                         transmission */
   CmLListCp       toBeActvtdUeLst; /*!< List of ULSPS enabled UEs with pending
                                         activation */
   CmLListCp       toBeRelUeLst;    /*!< List of ULSPS enabled UEs with release
                                         pending */
} RgSchCmnUlCellSpsInfo;


#endif
/*--------------------------*
 * SPS specific declarations End 
 *---------------------------*/
/**
  * @brief
  * Scheduler uplink scheduling parameters related to random access.
  */
typedef struct rgSchCmnUlCellRa
{
   uint8_t  prmblANumSb; /*!< Number of msg3 RBs to allocate for preamble A */
   uint8_t  prmblAIMcs;  /*!< Imcs for msg3 when preamble A was used */
   uint8_t  prmblBNumSb; /*!< Number of msg3 RBs to allocate for preamble B */
   uint8_t  prmblBIMcs;  /*!< Imcs for msg3 when preamble B was used */
} RgSchCmnUlCellRa;

typedef struct rgSchCmnCellClcITbs
{
   uint8_t       iTbs2Rbs; /*!< iTbs value for 2 Rbs precomputed at cell cfg */ 
   uint8_t       iTbs3Rbs; /*!< iTbs value for 3 Rbs precomputed at cell cfg */ 
}RgSchCmnCellClcITbs;

typedef struct rgSchCmnDlCell
{
   Bool                isDlFreqSel;  /*!< Bool indicating if cell is frequency 
                                          selective or not */
   uint8_t                  maxUeNewTxPerTti; /*!< Max UEs to be considered for New Tx Alloc in DL */
   uint8_t                  numRaSubFrms; /*!< Number of frames of RA transmission */
   uint8_t                  iTbsCap;      /*!< Max value DL iTbs capped to */
   uint16_t                 nCce;         /*!< Number of CCEs computed based on CFI */
   uint8_t                  maxDlBwPerUe; /*!< Max DL B/W per UE */
   uint8_t                  maxDlRetxBw;  /*!< Max DL retx B/W, as part of 256 */
   uint8_t                  maxUePerDlSf;  /*!< Max UE to be considered for DL scheduling
                                       *   in a TTI */
   /*[ccpu00138609]-ADD- max Msg4/ DL CCCH UE configuration */                                    
   uint8_t                  maxCcchPerDlSf; /*!< Max Msg4/DL CCCH UE sched in Dlsf */                                     
   uint8_t                  msg4TxDelay;  /*!<  Max estimated time for HARQ tx
                                           of msg4 based on the Harq  RTT and 
                                           max Harq retries for msg4 */
   RgSchCmnCellClcITbs cmnChITbs;  /*!< iTbs value for 2 Rbs precomputed at cell cfg */ 
   CmLteAggrLvl        cmnChAggrLvl; /*!< Precomputed aggregation level for common channel */ 
   uint8_t                  ccchCqi;      /*!< Default Cqi to be used for Msg4 and UE */
   CmLListCp           msg4RetxLst;  /*!< Queue to hold Msg4 procs for retransmission */
   /* Changes for CR timer */
#ifdef RGR_V1
   CmLListCp           ccchSduRetxLst;  /*!< Queue to hold CCCH SDU procs for retransmission */
#endif
#ifdef EMTC_ENABLE
   Void                *emtcCqiToTbsTbl[RGSCH_MAX_NUM_LYR_PERCW][RG_SCH_CMN_MAX_CFI]; 
#endif   
   Void                *cqiToTbsTbl[RGSCH_MAX_NUM_LYR_PERCW][RG_SCH_CMN_MAX_CFI]; 
   /* cqi to Tbs tables for each 1 and 2 layer TbSz table */
                                        /*!< CQI to efficiency translation */
   Void                *cqiToEffTbl[RGSCH_MAX_NUM_LYR_PERCW][RG_SCH_CMN_MAX_CFI]; 
   uint8_t                  newCfi;          /*!< New CFI value */
   uint8_t                  currCfi;         /*!< Current CFI value */
                                        
   uint16_t                 noResPerRb[RG_SCH_CMN_MAX_CFI]; /*!< Num REs per RB */   
   CmLteTimingInfo     time;         /*!< Timing info for current allocation */
   Void                *schSpfc;     /*!< Scheduler Specific Cell DL dereferencing */
   Void                *dlfsCell;    /*!< DLFS specific information per cell */
   CmLListCp           taLst;        /*!< TA queues, holds the UEs for which TA 
                                        has to be scheduled */
#ifdef LTEMAC_SPS
   RgSchCmnDlCellSpsInfo dlSpsInfo; /*!< DL SPS info for the cell */
#endif
   /* Member to store no. of Bits per RB */
   uint32_t                 bitsPerRb;   /*!< Bits per RB calculated from
                                      BcchPcchRaRsp Code rate configured through
                                      RGR */
#ifdef LTE_TDD                                      
   uint16_t                 numReDwPts[RG_SCH_CMN_MAX_CFI-1];  /*!< Num of RE in DwPTS RB */
   uint8_t                  splSfCfg;    /*!<Stores the special subframe cfg */  
#endif   

   /* ccpu00132314-ADD-Tx power offsets for Common PDSCH transmissions */                                   
   uint16_t                 bcchTxPwrOffset; /*!< Tx Pwr Offset for BCCH tx on PDSCH.
                                             Offset to the reference signal 
                                             power. Value: 0 -> 10000, 
                                             representing -6 dB to 4 dB in 0.001
                                             dB steps */                                    
   uint16_t                 pcchTxPwrOffset; /*!< Tx Pwr Offset for PCCH tx.
                                             Offset to the reference signal 
                                             power. Value: 0 -> 10000, 
                                             representing -6 dB to 4 dB in 0.001
                                             dB steps */                                    
   uint16_t                 rarTxPwrOffset; /*!< Tx Pwr Offset for RAR tx.
                                             Offset to the reference signal 
                                             power. Value: 0 -> 10000, 
                                             representing -6 dB to 4 dB in 0.001
                                             dB steps */                                    
  /* ccpu00138898 - Added Tx pwr offset for PHICH Tx*/
   uint16_t                 phichTxPwrOffset; /*!< Tx Pwr Offset for PHICH tx.
                                             Offset to the reference signal 
                                             power. Value: 0 -> 10000, 
                                             representing -6 dB to 4 dB in 0.001
                                             dB steps */                                    
   uint32_t          ncsgPrbCnt; /*!< Cumulative sum of PDSCH PRBs assigned to non-Csg UEs */
   uint32_t          totPrbCnt; /*!< Cumulative sum of PDSCH PRBs assigned to all UEs */
   RgrUeDlPwrCntrlPaCfg msg4pAVal;      /*!< Default value (Enum) of PA that is 
                                          used by Scheduler for msg4 */
#ifdef LTE_ADV
   CmLListCp           secCellActCeLst; /*!< List for holding the UE's
                                           for which sec cell act CE's needs to scheduled */ 
                                            
#endif
#ifdef EMTC_ENABLE
   CmLListCp           emtcTaLst;        /*!< TA queues, holds the EMTC UEs for which TA 
                                        has to be scheduled */
   Void                *schSpfcEmtc;     /*!< Scheduler Specific Cell DL dereferencing */
#endif
} RgSchCmnDlCell;

/**
  @brief Information related to TPC-PUCCH-RNTI/TPC-PUSCH-RNTI. */
typedef struct rgSchCmnTpcRntiCb
{
   CmLteRnti  tpcRnti;       /*!< TPC-PUCCH-RNTI/TPC-PUSCH-RNTI*/
   Bool       isFmt3a;       /*!< DCI format type: 3/3A */
   CmLListCp  toBeSchdUes;   /*!< List of UEs requiring power adjustment 
                               for this TPC-RNTI */
   CmLListCp  cfgdUes;         /*!< List of UEs */
   CmLList    schdLnk;     /*!< Link to the list of TPC RNTIs to be
                                  scheduled */
} RgSchCmnTpcRntiCb;

/**
  @brief Uplink Power control related information per cell. */
typedef struct rgSchCmnUlPwrCb 
{
   uint8_t                tpcPucchRntiCnt;/*!< Count of TPC-PUCCH-RNTIs for the cell */
   RgSchCmnTpcRntiCb tpcPucchRntiLst[RG_SCH_CMN_MAX_NUM_TPC_PUCCH_RNTI];
                                     /*!< List of TPC-PUCCH-RNTIs */
   uint8_t                tpcPuschRntiCnt;/*!< Count of TPC-PUSCH-RNTIs for the cell */
   RgSchCmnTpcRntiCb tpcPuschRntiLst[RG_SCH_CMN_MAX_NUM_TPC_PUSCH_RNTI];
                                     /*!< List of TPC-PUSCH-RNTIs */
   CmLListCp         pucchGrpPwr;    /*!< List of TPC-PUCCH-RNTIs for PUCCH group
                                       power control: 'RgSchCmnTpcRntiCb' */
   CmLListCp         puschGrpPwr;    /*!< List of  TPC-PUSCH-RNTIs for PUSCH group 
                                       power control: 'RgSchCmnTpcRntiCb' */
   S8                pMax;           /*!< Max allowed uplink power in cell */
   uint8_t                trgUlCqi;       /*!< Default target CQI */
} RgSchCmnUlPwrCb;

/**
  * @brief
  * Cell specific uplink scheduling information for Scheduler type 1.
  */
typedef struct rgSchCmnUlCell
{
   uint8_t               maxUeNewTxPerTti; /*!< Max UEs to be considered for New Tx Alloc in UL */
   /* Added new variable maxUlBwPerUe */
   uint8_t               maxUlBwPerUe;      /*!< Max UL BW per UE */
   uint8_t               maxSbPerUe;      /*!< Max subbands per UE */
   uint8_t               dfltUlCqi;       /*!< Default uplink CQI assumed intitially */
   uint8_t               max16qamCqi;     /*!< Highest CQI supporting 16 QAM */
   uint8_t               maxUlSpsCqi;     /*!< Highest CQI supporting 16 QAM */
   uint8_t               iTbsCap;         /*!< Max value UL iTbs capped to */
   uint8_t               sbSize;          /*!< Subband size */
   uint8_t               dmrsArrSize;     /*!< DMRS array size */
   uint8_t               *dmrsArr;        /*!< DMRS array */
   RgSchCmnUlCellRa ra;          /*!< RA related info */
   uint8_t               idx;             /*!< Current subframe - maps to HARQ process ID */
   uint8_t               schdIdx;         /*!< Subframe to schedule for */
   uint8_t               schdHqProcIdx;   /*!< Proc to schedule for */
   uint8_t               msg3SchdIdx;     /*!< Subframe to schedule for msg3 */
#ifdef EMTC_ENABLE
   RgSchCmnUlCellRa emtcRa;          /*!< RA related info */
   uint8_t               emtcMsg3SchdIdx;
   Void             *schSpfcEmtc;        /*!< Scheduler Specific Cell UL dereferencing */
#endif
   uint8_t               msg3SchdHqProcIdx;/*!< Proc to schedule for */
   uint8_t               rcpReqIdx;       /*!< Subframe to send reception req for */
   /* ccpu00130688 -MOD- for config-0 changes */
   uint8_t               hqFdbkIdx[2];    /*!< In FDD only Idx 0 is used. 
                                          In TDD n+k value is updated at idx 0.
                                          For TDD Cfg 0 both indices are used */ 
   uint8_t               reTxIdx[2];      /*!< Retransmission Index corresponding to 
                                          the hqFdbkIdx */
#ifdef LTEMAC_SPS
   uint8_t               spsUlRsrvIdx;     /*!< Subframe to reserve UL SPS cfgd grant */
   uint8_t               spsUlRsrvHqProcIdx;/*!< Proc for the cfgd UL SPS grant */
#endif
   CmLteTimingInfo  schdTime;
#ifdef LTE_TDD
   uint8_t               numUlSubfrms;    /*!< Number of UL subframes */
   RgSchUlSf        *ulSfArr;        /*!< no msg3 alloc info here */
#else
   RgSchUlSf        ulSfArr[RG_SCH_CMN_UL_NUM_SF];      /*!< no msg3 alloc info here */
#endif
   Void             *schSpfc;        /*!< Scheduler Specific Cell UL dereferencing */
   RgSchCmnUlPwrCb  ulPwrCb;     /*!< Uplink power control block */
   uint8_t               ulNumRePerRb;  /*!< Number of REs per RB in UL */
   /* Added support for non-adaptive retransmission in uplink */
   uint8_t               maxAllocPerUlSf; /*!< Max Allocations in a given SF */
#ifdef RGR_V1
/* Added a param to limit msg3 allocations */
   uint8_t               maxMsg3PerUlSf; /*!< Max msg3 alocs in a given SF */
#endif

#ifdef LTEMAC_SPS
   RgSchCmnUlCellSpsInfo ulSpsInfo; /*!< UL SPS info for the cell */
   uint16_t                   schdTti;   /*< 0..1023, corresponding to scheduling time,
                                     * can theoretically used for non-SPS
                                     * purposes as well */
#endif
   uint32_t          ncsgPrbCnt; /*!< Cumulative sum of PDSCH PRBs assigned to non-Csg UEs */
   uint32_t          totPrbCnt; /*!< Cumulative sum of PDSCH PRBs assigned to all UEs */
   CmLListCp    reTxLst;  /*!< Retransmission List*/
} RgSchCmnUlCell;

/**
  @brief ACK-NACK repetition related information per cell. */
typedef struct rgSchCmnAckNakRepCb 
{
#ifdef LTE_TDD
   CmLListCp  ackNakRepQ[2*RGSCH_NUM_SUB_FRAMES];  /*!< ACK NACK repetition queue */
#else
   CmLListCp  ackNakRepQ[RGSCH_NUM_SUB_FRAMES];  /*!< ACK NACK repetition queue */
#endif
} RgSchCmnAckNakRepCb;

/**
  @brief Measurement Gap related information per cell. */
typedef struct rgSchCmnMeasGapCb 
{
   CmLListCp  gapPrd40Q[RG_SCH_CMN_MEAS_GAPPRD40]; /*!< Measurement Gap queue
                                               for UEs with 40 ms gap period */
   CmLListCp  gapPrd80Q[RG_SCH_CMN_MEAS_GAPPRD80]; /*!< Measurement Gap queue 
                                               for UEs with 80 ms gap period */
} RgSchCmnMeasGapCb;

/**
  * @brief
  * common scheduler specific information for rapId to UE mapping.  */
typedef struct rgSchCmnRapIdMap
{
   uint8_t            rapId;
   CmLListCp     assgndUes;   /*!< List of UEs for which this rapId is 
                               assigned. */
} RgSchCmnRapIdMap;

/**
  * @brief
  * common scheduler specific information for RACH Dedicated Preambles.  */
typedef struct rgSchCmnRachCfg
{
   uint8_t                numDedPrm;   /*!< number of configured dedicated prmbls */
   uint8_t                dedPrmStart; /*!< starting rapId Number */
   uint8_t                remDedPrm;   /*!< remaining number of ded Prm available
                                    for the "applFrm" */
   CmLteTimingInfo   applFrm;     /*!< Frame under consideration for dedPrm
                                     distribution */
   uint8_t                prachMskIndx;/*!< Prach Mask Idx corresponding to
                                      applFrm*/
   RgSchCmnRapIdMap  rapIdMap[RG_SCH_MAX_DED_PRMBLS]; /*!< mapping of RapId 
                                                       *   to assigned UEs */
   CmLListCp         hoUeLst;  /*!< List of UEs undergoing Handover */
   CmLListCp         pdcchOdrLst;  /*!< Pdcch Order Q, holds the UEs for which
                                       PO has to be generated. */
} RgSchCmnRachCfg;

/**
  @brief Uplink Power control related information per UE. */
typedef struct rgSchCmnUeUlPwrCb 
{
   Bool              isAccumulated;   /*!< Indicates if power is accumulative or not */
   Bool              deltaMcsEnbld;   /*!< Indicates if coding effeciency is 
                                       * considered or not for PUSCH power computation */
   uint8_t                pucchIdx;        /*!< Index for TPC-PUCCH-RNTI */
   uint8_t                puschIdx;        /*!< Index for TPC-PUSCH-RNTI */
   uint8_t                isPhrAvail;      /*!< Indicates if PHR is recieved */
   S8                phVal;           /*!< Power headroom value in dB */
   S8                pwrPerRb;        /*!< UL power computed per RB */
   S8                maxUePwr;        /*!< Maximum power with which UE can transmit */
   uint8_t                maxUlRbs;        /*!< Maximum number of UL Rbs for UL scheduling */
   S8                delta;           /*!< Delta corresponding to TPC, for PUSCH */
   uint8_t                numRb;           /*!< Number of RBs used in last allocation */
   S8                remPuschPwr;     /*!< PUSCH power remaining to be adjusted
                                        (in db) */ /* chk if needed */
   S8                remPucchPwr;     /*!< PUCCH Power remaining to be adjusted (in db) */
   uint8_t                pucchTpc;        /*!< TPC to be used for PUCCH power control */
   uint8_t                puschTpc;        /*!< TPC to be used for PUSCH power control */
   uint8_t                trgCqi;          /*!< Target CQI */
   RgSchCmnTpcRntiCb *tpcPucchRntiCb; /*!< Pointer to tpcPucchRntiCb for the UE */
   CmLList           pucchGrpLnk;     /*!< To link together UEs in
                                       * RgSchCmnTpcRntiCb */
   CmLList           schdPucchGrpLnk; /*!< To link together scheduled
                                       * UEs in RgSchCmnTpcRntiCb */
   RgSchCmnTpcRntiCb *tpcPuschRntiCb; /*!< Pointer to tpcPuschRntiCb for the UE */
   CmLList           puschGrpLnk;     /*!< To link together UEs in
                                       * RgSchCmnTpcRntiCb */
   CmLList           schdPuschGrpLnk; /*!< To link together scheduled
                                       * UEs in RgSchCmnTpcRntiCb */
   S8                p0UePusch;         /*!< P_0UE_PUSCH*/
   S8                p0UePucch;         /*!< P_0_PUCCH*/
   S8                maxPwrPerRb;
   S8                maxPwrDeltaByPhr;
} RgSchCmnUeUlPwrCb;

/**
  @brief Uplink RB allocation information. */
struct rgSchCmnUeUlAlloc
{
   /* Request */
   uint32_t          reqBytes;     /*!< Requested bytes */

   /* Allocation to be filled by UL RB allocator module */
   uint32_t          allocdBytes;  /*!< Allocated bytes */
   RgSchUlAlloc *alloc;       /*!< Alloc assgnd by Allocator */
   CmLList      reqLnk;       /*!< To link UL Tx UEs */
   CmLList      schdLstLnk;   /*!< To link scheduled/non-scheduled UL UEs */
};

typedef struct rgSchCmnAllocRecord
{
   uint32_t             alloc;      /* allocation amount */
   CmLteTimingInfo allocTime;  /* Time at which allocation made */
   CmLList         lnk;        /* To link in ulAllocLst */
   uint8_t              numRb;      /* Number of RBs */
   uint8_t              cqi;        /* CQI assumed for allocation */
   uint8_t              tpc;        /* TPC */
}RgSchCmnAllocRecord;


/**
  * @brief
  * Uplink Bler LA information for UE
  */
#ifdef UL_LA
typedef struct ueUlLaCb 
{
   S32             deltaiTbs;  
   uint32_t             iTbsUpperCap;
   S32             cqiBasediTbs;
   Bool            lastiTbsIgnored;
} UeUlLaCb;
#endif 

/**
  * @brief
  * Uplink information for scheduler per UE
  */
typedef struct rgSchCmnUlUe
{
   uint8_t                maxUlCqi;        /*!< CQI for which no better Imcs can be granted */
   uint8_t                crntUlCqi[RG_SCH_MAX_UL_TX_ANT]; /*!< Current CQI */
/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
   uint8_t                validUlCqi;
#endif 
   uint8_t                lastCfi;          /* last CFI, updated in case of SPS */
   CmLListCp         ulAllocLst;      /*!< To track the outstanding Allocations 
                                       *   node type RgSchCmnAllocRecord */
   

   Void              *schSpfc;        /*!< scheduler specific UE DL Info */
   RgSchCmnUeUlPwrCb ulPwrCb;         /*!< Uplink power control block */
   RgSchCmnUeUlAlloc alloc;           /*!< Allocation info */
#ifdef SCH_STATS
   uint32_t            schedOccns;      /*!< Number of scheduling occassions in a refresh period */
   uint32_t            schedRetxOccns;
   uint32_t            avgCqi;          /*!< AvgCqi in a refresh period */
   uint32_t            numCqiOccns;
   uint32_t            prbAlloc;
#endif
#ifdef UL_LA
   UeUlLaCb       ulLaCb;  /*!< Uplink LA structure */
#endif
   RgUeUlHqCb     hqEnt;      /*!< Uplink HARQ information for the UE */
   uint8_t             subbandShare; /*!< New variable added to store the number
                                    * of subbands alowed for this UE */
   uint32_t            subbandRequired; /*!< Number of subbands required to
                                      * serve the total BO */
   CmLList        ulSchedLnk; /*!< To link UE UL Cb to toBeSchedList */
#ifdef EMTC_ENABLE
   RgSchUlHqProcCb    *tempProc; /*!< To identify UE is serverd for Retx */
#endif
#ifdef RG_5GTF
   uint8_t            vrbgRequired;
   uint8_t            vrbgAllocated;
#endif
} RgSchCmnUlUe;

/**
  @brief Downlink RB allocation information for Msg4. */
typedef struct rgSchCmnMsg4RbAlloc
{
   RgSchDlSf     *msg4DlSf;         /*!< DL sub-frame for which allocation is to
                                         be done: filled in by RR/MAX C/I/PFS */ 
   CmLListCp     msg4TxLst;          /*!< List of RgSchDlRbAllocs for Msg4 Tx */ 
   CmLListCp     msg4RetxLst;        /*!< List of RgSchDlRbAllocs for Msg4 ReTx */ 
   CmLListCp     schdMsg4TxLst;      /*!< List of Msg4 Txs scheduled per TTI */
   CmLListCp     schdMsg4RetxLst;    /*!< List of Msg4 ReTxs scheduled in the TTI */
   CmLListCp     nonSchdMsg4TxLst;   /*!< List of transmitting MSG4 not scheduled in the TTI */
   CmLListCp     nonSchdMsg4RetxLst; /*!< List of re-transmitting MSG4 not 
                                          scheduled in the TTI */
} RgSchCmnMsg4RbAlloc;
#ifdef RGR_V1
/* Changes for CR timer implementation*/
typedef struct rgSchCmnCcchSduRbAlloc
{
   RgSchDlSf     *ccchSduDlSf;         /*!< DL sub-frame for which allocation is to
                                         be done: filled in by RR/MAX C/I/PFS */ 
   CmLListCp     ccchSduTxLst;          /*!< List of RgSchDlRbAllocs for CcchSdu Tx */ 
   CmLListCp     ccchSduRetxLst;        /*!< List of RgSchDlRbAllocs for CcchSdu ReTx */ 
   CmLListCp     schdCcchSduTxLst;      /*!< List of CcchSdu Txs scheduled per TTI */
   CmLListCp     schdCcchSduRetxLst;    /*!< List of CcchSdu ReTxs scheduled in the TTI */
   CmLListCp     nonSchdCcchSduTxLst;   /*!< List of transmitting MSG4 not scheduled in the TTI */
   CmLListCp     nonSchdCcchSduRetxLst; /*!< List of re-transmitting MSG4 not 
                                          scheduled in the TTI */
} RgSchCmnCcchSduRbAlloc;
#endif

/**
  @brief Downlink RB allocation information for UEs. */
typedef struct rgSchCmnUeRbAlloc
{
   RgSchDlSf     *dedDlSf;      /*!< DL sub-frame for which dedicated
                                     allocation is to be done: filled in
                                     by RR/MAX C/I/PFS */ 
   CmLListCp     txHqPLst;       /*!< List of HqPs to be scheduled for Tx per 
                                     TTI: RgSchUeCb list  */
   CmLListCp     retxHqPLst;     /*!< List of HqPs scheduled for ReTx per 
                                     TTI: RgSchUeCb list */
   CmLListCp     errIndTxHqPLst; /*!< LAA SCELL: List of transmitting LAA Err Ind Tx HqPs scheduled per TTI */
#ifdef LTEMAC_SPS
   CmLListCp     retxSpsHqPLst;   /*!< List of SPS HqPs scheduled for ReTx per 
                                     TTI: RgSchUeCb list */
   CmLListCp     txSpsHqPLst;     /*!< List of SPS HqPs scheduled for Tx per 
                                     TTI: RgSchUeCb list */
#endif
   CmLListCp     txLaaHqPLst;     /*!< List of LAA HqPs scheduled on PCell for Tx per
TTI*/
   CmLListCp     schdTxHqPLst;     /*!< List of transmitting HqPs scheduled per TTI */
   CmLListCp     schdRetxHqPLst;   /*!< List of re-transmitting HqPs scheduled per TTI */
   CmLListCp     nonSchdTxHqPLst;  /*!< List of transmitting HqPs not scheduled in the TTI */
   CmLListCp     nonSchdRetxHqPLst;/*!< List of re-transmitting HqPs not scheduled in the TTI */
/* Changes for MIMO feature addition */
   /* MIMO Tx+Retx hqProc scheduling handling */
   CmLListCp     txRetxHqPLst;   /*!< List of HqPs scheduled for tx and retx per 
                                     TTI(MIMO case): RgSchUeCb list */
   CmLListCp     schdTxRetxHqPLst; /*!< List of TX&RETXing(MIMO case) HqPs scheduled per TTI */
   CmLListCp     nonSchdTxRetxHqPLst; /*!< List of TX&RETXing(MIMO case) HqPs not scheduled in the TTI */
#ifdef LTEMAC_SPS
   CmLListCp     schdRetxSpsHqPLst;   /*!< List of re-transmitting SPS HqPs scheduled per TTI */
   CmLListCp     nonSchdRetxSpsHqPLst;/*!< List of re-transmitting SPS HqPs 
                                          not scheduled in the TTI */
   CmLListCp     schdTxSpsHqPLst;     /*!< List of transmitting SPS HqPs scheduled per TTI */
   CmLListCp     nonSchdTxSpsHqPLst; /*!< List of transmitting SPS HqPs not scheduled per TTI */
#endif
   CmLListCp     schdTxLaaHqPLst;     /*!< List of transmitting LAA TBs scheduled on PCell per TTI */
   CmLListCp     nonSchdTxLaaHqPLst; /*!< List of transmitting LAA TBs not scheduled on PCell per TTI */
   CmLListCp     schdErrIndTxHqPLst;     /*!< List of transmitting LAA ErrInd TBs scheduled per TTI */
   CmLListCp     nonSchdErrIndTxHqPLst;  /*!< List of transmitting LAA ErrInd not scheduled per TTI */
} RgSchCmnUeRbAlloc;

/**
  @brief Downlink RB allocation information. */
struct rgSchCmnDlRbAllocInfo
{
   RgSchDlRbAlloc      pcchAlloc;  /*!< Allocation for PCCH */
   RgSchDlRbAlloc      bcchAlloc;  /*!< Allocation for BCCH on DLSCH */
   RgSchDlRbAlloc      raRspAlloc[RG_SCH_CMN_MAX_CMN_PDCCH]; /*!< Allocation for RAR */
   RgSchCmnMsg4RbAlloc msg4Alloc;  /*!< Alloction for Msg4  */
#ifdef RGR_V1
   /* Changes for CR timer implementation*/
   RgSchCmnCcchSduRbAlloc ccchSduAlloc;  /*!< Alloction for ccchSdu  */
#endif
   RgSchCmnUeRbAlloc   dedAlloc;   /*!< Alloction information for UEs */
};

/**
  * @brief
  * Cell specific common scheduler information for all Scheduler types.
  */
typedef struct rgSchCmnCell
{
   RgrCfiCfg           cfiCfg;         /*!< CFI for PDCCH */
   RgrUlTrgCqiCfg      trgUlCqi;       /*!< Target UL CQI */
   CmTqCp              tmrTqCp; /*!< Refresh Timer Task Queue
                                 * Control Point */
   CmTqType            tmrTq[RG_SCH_CMN_NUM_REFRESH_Q]; /*!< Timer Task Queue */
   RgrDlCmnCodeRateCfg dlCmnCodeRate;  /*!< Coding rate for common DL channels: 
                                            Expressed in multiples of 1024 */
   RgrPuschSubBandCfg  puschSubBand;   /*!< UL subband information */
   RgrUlCmnCodeRateCfg ulCmnCodeRate;  /*!< Coding rate for common UL channels: 
                                            Expressed in multiples of 1024 */
   RgSchCmnRachCfg     rachCfg;    /*!< Rach configuration for schCmn */
   RgSchCmnUlCell      ul;         /*!< Scheduler UL info */
   RgSchCmnDlCell      dl;         /*!< Scheduler DL info */
   RgUlSchdApis        *apisUl;    /*!< Specific UL Scheduler APIs */
   RgDlSchdApis        *apisDl;    /*!< Specific DL Scheduler APIs */
   RgDlfsSchdApis      *apisDlfs; /*!< APIs specific to DLFS scheduler */
#ifdef EMTC_ENABLE   
   RgUlSchdApis        *apisEmtcUl;    /*!< Specific UL Scheduler APIs for EMTC*/
   RgDlEmtcSchdApis    *apisEmtcDl;    /*!< Specific DL Scheduler APIs for EMTC*/
#endif   
   CmLteAggrLvl        dciAggrLvl[RG_SCH_CMN_MAX_CQI][10]; 
                                           /*!< Aggr Level for each CQI for 
                                            * each DCI Format */
   RgSchCmnDlRbAllocInfo  allocInfo;
}RgSchCmnCell;


/**
  * @brief
  * RACHO information for scheduler per UE.
  */
typedef struct rgSchCmnDlUeRachInfo
{
   CmLList         inActUeLnk; /*!< Link UE to PO inactUeList */
   CmLList         poLnk;    /*!< To link UE to PDCCH Order Q */
   CmLList         hoLnk;    /*!< To link UE to HandOver UE lst */
   CmLList         rapIdLnk; /*!< Link to the list assgndUes */
   CmLteTimingInfo asgnOppr; /*!< PRACH oppurtunity time assgined to UE */
   uint8_t              hoRapId;  /*!< RAPID assigned to UE for HandOver */
   uint8_t              poRapId;  /*!< RAPID assigned to UE for PdcchOrder */
}RgSchCmnDlUeRachInfo;


/**
  * @brief
  * Downlink CodeWord information for scheduler per UE.
  */
typedef struct rgSchCmnDlUeCwInfo
{
   uint8_t     cqi;     /*!< CQI reported for this CW */
   uint8_t     iTbs[2]; /*!< [0]ITBS for CW for 1 Layer,
                        corresponding to this CW's cqi. */
                   /*!< [1]ITBS for CW for 2 Layer,
                        corresponding to this CW's cqi. */
   uint32_t    eff[2];  /*!< [0]eff for CW for 1 Layer,
                        corresponding to this CW's cqi. */
                   /*!< [1]eff for CW for 2 Layer,
                        corresponding to this CW's cqi. */
   uint8_t     noLyr;   /*!< No. of layers this CW shall be using 
                    *   for transmission */
   uint16_t    dtxCnt;
   uint16_t    ackCnt;
   uint16_t    nackCnt;
}RgSchCmnDlUeCwInfo;
/**
  * @brief UE cmn scheduler specific MIMO Info.
  */
typedef struct rgSchCmnUeMimoInfo
{
   RgSchCmnDlUeCwInfo   cwInfo[RG_SCH_CMN_MAX_CW_PER_UE];/*!< Codeword related feddback Information */ 
   uint8_t                   ri;       /*!< Maximum allowable number of TX layers for SM */ 
   uint8_t                   pmi;      /*!< Precoding matrix indicator(if any) */
   uint8_t                   btrCwIdx; /*!< Index of a better(efficient) CW (0 or 1) */ 
   uint8_t                   forceTD;  /*!< Flag to indicate transmission scheme as TD
                                   *   beyond any other consideration */
}RgSchCmnUeMimoInfo;

typedef struct ueLaCb {
   S32                  deltaiTbs;  
   uint32_t                  iTbsUpperCap;
   S32                  cqiBasediTbs;
   Bool                 lastiTbsIgnored;
   uint8_t                   notFirstCqi;
   uint8_t                   numLastiTbsIgnored;
} UeLaCb;

/**
  * @brief
  * Downlink information for scheduler per UE.
  */
typedef struct rgSchCmnDlUe
{
   uint32_t                  maxSbSz;  /*!< Max soft channel bits per Hq proc per TTI */
   uint32_t                  maxTbSz;  /*!< Max DLSCH TB bits per TB per TTI */ 
   uint8_t                   maxRb;    /*!< updated based on SoftBuffer Limitation and MaxDlBwPerUE */
   uint32_t                  maxTbBits;/*!< Max Transport Block Bits this UE can receive per TTI*/
   RgSchCmnUeMimoInfo   mimoInfo; /*!< UE cmn scheduler specific MIMO Info */
   RgSchDlHqProcCb      *proc;    /*!< Proc which is picked for Trans for this Subfrm,"dlSf" */
   Void                 *schSpfc; /*!< scheduler specific UE DL Info */
   Void                 *dlfsUe;  /*!< DLFS Specific information */
   uint32_t                  outStndAlloc; /*!< UEs outstanding allocation, for a given TTI.
                                       * valid for a single scheduling index */
   RgSchCmnDlUeRachInfo rachInfo; /*!< Ue specific RACH HO Info */
#ifdef LTEMAC_SPS
   RgSchCmnDlUeSpsInfo  dlSpsInfo;/*!< DL SPS information for the UE */
#endif
#if defined(SCH_STATS) || defined(TENB_STATS)
   uint32_t            schedOccns;
   uint32_t            currPdbLvl;
   uint32_t            prevOccnLvlUpd;
 /*  uint32_t            schedRetxOccns;
   uint32_t            prbAlloc;*/
#endif
#ifdef SCH_STATS
   uint32_t            schedRetxOccns;
   uint32_t            avgCqi;
   uint32_t            numCqiOccns;
   uint32_t            numRi1;
   uint32_t            numRi2;
   uint32_t            boReported;
   uint32_t            prbAlloc;
   uint32_t            remAmbrForStats;
#endif
   UeLaCb         laCb[RG_SCH_CMN_MAX_CW_PER_UE];
   uint8_t             cqiFlag;
   uint8_t             lastCfi;
#ifdef RG_5GTF
   uint8_t            vrbgRequired;
   uint8_t            vrbgAllocated;
#endif
} RgSchCmnDlUe;

/**
  @brief Uplink RB allocation information. */
struct rgSchCmnUlRbAllocInfo
{
#ifdef EMTC_ENABLE
   RgSchEmtcUlSf *ulsf;
#endif
   RgSchUlSf     *sf;             /*!< Subframe to schedule for */
   CmLListCp     contResLst;      /*!< UEs to schedule for cnt resn */
   CmLListCp     schdContResLst; /*!< Final UEs scheduled for cnt resn */
   CmLListCp     nonSchdContResLst; /*!< UEs not scheduled for  cnt resn*/
   CmLListCp     ueLst;           /*!< UEs to schedule for data */
   CmLListCp     schdUeLst;      /*!< Final UEs scheduled for data */
   CmLListCp     nonSchdUeLst;   /*!< Final UEs not scheduled for data */
};

/**
  * @brief
  * Information common to DL and UL scheduler per UE.
  */
typedef struct rgSchCmnUeInfo
{
   uint8_t                      ueCat;         /*!< UE category */
   CmTimer                 tmr;
} RgSchCmnUeInfo;
/**
  * @brief
  * Information for scheduler per UE. 
  */
typedef struct rgSchCmnUe
{
  RgSchCmnUeInfo  cmn; /*!< UE specific scheduler information common to
                                  uplink and downlink */ 
  RgSchCmnUlUe   ul;  /*!< UE specific UL scheduler information */
  RgSchCmnDlUe   dl;  /*!< UE specific DL scheduler informaion */ 
} RgSchCmnUe;

typedef struct rgSchCmnLcg
{
   uint32_t      bs;            /*!< Effective Buffer Status */
   uint32_t      cfgdGbr;       /*!< Configured GBR */
   uint32_t      effGbr;        /*!< Effective GBR */
   uint32_t      deltaMbr;      /*!< Configured MBR in excess of configured GBR */
   uint32_t      effDeltaMbr;   /*!< Effective MBR */
   uint32_t      reportedBs;    /*!< Latest Buffer Status */
   Void     *schSpfc;
}RgSchCmnLcg;

#ifdef LTEMAC_SPS
/**
  * @brief
  * SPS information for DL service 
  */
typedef struct rgSchCmnDlSvcSpsInfo
{
   CmLList   toBeSchdSvcEnt;    /*!< Linked list entity for toBeSchdSvcs lst */
   uint16_t       zeroBoOcassionCnt; /*!< Number of contiguous SPS ocassions for 
                                     which BO=0 */
   uint32_t       effSpsBo;          /*!< Effective BO of the SPS service */
   uint32_t       bytesReq;          /*!< Bytes Requested for this SPS service */
   uint8_t        hdrEst;            /*!< Header estimate for SPS service */
   
} RgSchCmnDlSvcSpsInfo;
#endif

typedef struct rgSchCmnDlSvc {
   uint8_t           qci;    /*!< Prio computed against Qci */
   uint8_t           prio;    /*!< Prio computed against Qci */
   uint32_t          gbr;     /*!< scaled GBR as per Refresh time resolution */
   uint32_t          mbr;     /*!< scaled MBR as per Refresh time resolution */
   Void         *schSpfc[CM_LTE_MAX_CELLS];/*!< Scheduler specific Info */
#ifdef LTEMAC_SPS
   RgSchCmnDlSvcSpsInfo dlSvcSpsInfo; /*!< SPS related information for DL
                                           service */
#endif
}RgSchCmnDlSvc;

typedef struct rgSchCmnDlHqProc {
   CmLList        retxLnk; /*!< To link retransmitting HARQ processes in cell */
   uint32_t            totBytes;/*!< This maintains total allocation */ 
#ifdef LTEMAC_SPS
   Bool          isSpsSvcSchd;/*!< Indicates if this HARQ process is having SPS
                                   service scheduled: TRUE for SPS and non-SPS
                                   ocassions */
   Bool          isSpsActv;   /*!< Indicates if this HARQ proc
                                   is in-use for SPS transmission: TRUE only for
                                   SPS ocassions */
   uint8_t            spsAction;   /*!< SPS action associated with this HARQ proc:
                                   activation/reactivation */
   CmLteTimingInfo maxRetxTime; /*!< Maximum retransmission time for SPS HARQ
                                  proc */
#endif 
   Void           *schSpfc;/*!< Scheduler specific Info */
}RgSchCmnDlHqProc;

/*--------------------------*
 * UL specific declarations END
 *---------------------------*/

/* Inappropriate name of CQI to ITbs table for DL. */
typedef uint8_t RgSchCmnCqiToTbs[16];
/* The following data type is used to store computed efficiency */
/* for each MCS and consequently, will be used to derive MCS    */
/* for a CQI. The last row is used for storing the average   */
typedef uint32_t RgSchCmnTbSzEff[RG_SCH_CMN_NUM_TBS];

/* Inappropriate name of CQI to ITbs table for DL. */
/* Changes for MIMO feature addition */
RgSchCmnTbSzEff rgSchCmnNorCfi1Eff[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnNorCfi2Eff[RGSCH_MAX_NUM_LYR_PERCW];
RgSchCmnTbSzEff rgSchCmnNorCfi3Eff[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnNorCfi4Eff[RGSCH_MAX_NUM_LYR_PERCW];
/* Added new variable for Ul eff */
RgSchCmnTbSzEff rgSchCmnNorUlEff[1],rgSchCmnExtUlEff[1];
RgSchCmnCqiToTbs rgSchCmnNorCfi1CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnNorCfi2CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW];
RgSchCmnCqiToTbs rgSchCmnNorCfi3CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnNorCfi4CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW];
RgSchCmnCqiToTbs *rgSchCmnCqiToTbs[RGSCH_MAX_NUM_LYR_PERCW][RG_SCH_CMN_MAX_CP][RG_SCH_CMN_MAX_CFI]; 
RgSchCmnTbSzEff rgSchCmnExtCfi1Eff[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnExtCfi2Eff[RGSCH_MAX_NUM_LYR_PERCW];
RgSchCmnTbSzEff rgSchCmnExtCfi3Eff[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnExtCfi4Eff[RGSCH_MAX_NUM_LYR_PERCW];
RgSchCmnCqiToTbs rgSchCmnExtCfi1CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnExtCfi2CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW];
RgSchCmnCqiToTbs rgSchCmnExtCfi3CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnExtCfi4CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW];
/* Include CRS REs while calculating Efficiency */
RgSchCmnTbSzEff
*rgSchCmnEffTbl[RGSCH_MAX_NUM_LYR_PERCW][RG_SCH_CMN_MAX_CP][RG_SCH_CMN_MAX_ANT_CONF][RG_SCH_CMN_MAX_CFI];
/* Added new variable for Ul eff */
RgSchCmnTbSzEff *rgSchCmnUlEffTbl[RG_SCH_CMN_MAX_CP];

RgSchTbSzTbl rgTbSzTbl;

Void rgSCHCmnInit ARGS((Void
));
S16 rgSCHCmnRgrCellCfg ARGS((
RgSchCellCb   *cell,
RgrCellCfg    *cellCfg,
RgSchErrInfo  *err
));
S16 rgSCHCmnRgrCellRecfg ARGS((
RgSchCellCb             *cell,
RgrCellRecfg            *recfg,
RgSchErrInfo            *err
));
Void rgSCHCmnFreeDlLc ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));
Void rgSCHCmnCellDel ARGS((
RgSchCellCb     *cell
));
Void rgSCHCmnDlRlsSubFrm ARGS((
RgSchCellCb        *cell,
CmLteTimingInfo   frm
));
#ifdef LTE_ADV
S16  rgSCHCmnRgrSCellUeCfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeSecCellCfg  *sCellInfoCfg,
RgSchErrInfo *err
));

Void rgSchFreeTpcIdxForSCell ARGS((
RgSchUeCb *ue, 
RgSchDlHqProcCb   *proc, 
RgSchDlSf  *dlsf
));

Bool rgSchIsN1PucchResAvail ARGS((
  CmLListCp *lst, 
  RgSchUeCb *ue, 
  uint8_t        n1Idx,
  uint8_t        resCount
));
Bool rgSchIsN3PucchResAvail ARGS((
  CmLListCp *lst, 
  RgSchUeCb *ue, 
  uint8_t        n1Idx
));

S16 rgSchGetAvlTpcIdx ARGS((
 RgSchUeCb *ue, 
 uint8_t        *tpcIdx, 
 RgSchDlSf *dlsf,
 RgSchCellCb   *cell
));

Void rgSCHSCellDelUeSCell ARGS((
 RgSchCellCb  *cellCb,
 RgSchUeCb    *ueCb,
 uint8_t            sCellIdx
));

S16 rgSCHCmnRgrSCellUeDel ARGS((
 RgSchUeCellInfo *sCellInfo,
 RgSchUeCb    *ue
));

#endif /* LTE_ADV */
S16 rgSCHCmnRgrUeCfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeCfg     *ueCfg,
RgSchErrInfo *err
));
S16 rgSCHCmnRgrUeRecfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg,
RgSchErrInfo *err
));
Void rgSCHCmnUeDel ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
Void rgSCHCmnUeReset ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
S16 rgSCHCmnRgrLcgRecfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *lcg,
RgrLcgRecfg  *reCfg,
RgSchErrInfo *err
));
S16 rgSCHCmnRgrLcgCfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *lcg,
RgrLcgCfg    *lcgCfg,
RgSchErrInfo *err
));
S16 rgSCHCmnRgrLchCfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchDlLcCb  *dlLc,
RgrLchCfg *lcCfg,
RgSchErrInfo *err
));
S16 rgSCHCmnRgrLchDel ARGS((
RgSchCellCb   *cell,
RgSchUeCb     *ue,
CmLteLcId     lcId,    
uint8_t            lcgId
));
S16 rgSCHCmnRgrLchRecfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchDlLcCb  *dlLc,
RgrLchRecfg  *lcRecfg,
RgSchErrInfo *err
));
Void rgSCHCmnLcgDel ARGS((
RgSchCellCb   *cell,
RgSchUeCb     *ue,
RgSchLcgCb    *lcg
));
S16 rgSCHCmnUpdBsrShort ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *ulLcg,
uint8_t           bsr,
RgSchErrInfo *err
));
S16 rgSCHCmnUpdBsrTrunc ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *ulLcg,
uint8_t           bsr,
RgSchErrInfo *err
));
S16 rgSCHCmnUpdBsrLong ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
uint8_t           bsArr[],
RgSchErrInfo *err
));
S16 rgSCHCmnDataRcvd ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
uint8_t           numLc,
RgSchUlLcCb     *lcArr[],
uint16_t          bytesArr[],
RgSchErrInfo *err
));
Void rgSCHCmnUlCqiInd ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue,
TfuUlCqiRpt          *ulCqiInfo
));
S16 rgSCHCmnUpdExtPhr ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgInfExtPhrCEInfo *extPhr,
RgSchErrInfo *err
));
S16 rgSCHCmnUpdPhr ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
uint8_t           phr,
RgSchErrInfo *err
));
S16 rgSCHCmnUpdUlHqProc ARGS((
RgSchCellCb      *cell,
RgSchUlHqProcCb  *curProc,
RgSchUlHqProcCb  *oldProc
));
S16 rgSCHCmnContResUlGrant ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchErrInfo *err
));
Void rgSCHCmnActvtUlUe ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
Void rgSCHCmnActvtDlUe ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
Void rgSCHCmnHdlUlTransInd ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue,
CmLteTimingInfo timingInfo
));
S16 rgSCHCmnSrRcvd ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
CmLteTimingInfo frm,
RgSchErrInfo *err
));
Void rgSCHCmnUlRbAllocAddUeToLst ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue,
CmLListCp             *lst
));
S16 rgSCHCmnTti ARGS((
RgSchCellCb  *cell,
RgSchErrInfo *err
));
Void rgSCHCmnUlHqProcForUe ARGS((
RgSchCellCb         *cell,
CmLteTimingInfo     frm,
RgSchUeCb           *ue,
RgSchUlHqProcCb     **procRef
));
RgSchUlAlloc *rgSCHCmnFirstRcptnReq ARGS((
RgSchCellCb      *cell
));
RgSchUlAlloc *rgSCHCmnNextRcptnReq ARGS((
RgSchCellCb      *cell,
RgSchUlAlloc     *alloc
));
RgSchUlAlloc *rgSCHCmnFirstHqFdbkAlloc ARGS((
RgSchCellCb      *cell,
uint8_t                idx
));
RgSchUlAlloc *rgSCHCmnNextHqFdbkAlloc ARGS((
RgSchCellCb      *cell,
RgSchUlAlloc     *alloc,
uint8_t                idx
));
Void rgSCHCmnDlDedBoUpd ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));
/* Fix: syed Remove the msg4Proc from cell
 * msg4Retx Queue. I have used CMN scheduler function
 * directly. Please define a new API and call this
 * function through that. */     
Void rgSCHCmnDlMsg4ProcRmvFrmRetx ARGS((
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
));
Void rgSCHCmnDlProcAddToRetx ARGS((
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
));

#ifdef EMTC_ENABLE
Void rgSCHCmnEmtcUlProcAddToRetx ARGS((
RgSchCellCb                *cell,
RgSchUlHqProcCb            *hqP
));
#endif

Void rgSCHCmnDlCqiInd ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
Bool               isPucchInfo,
Void               *dlCqi,
CmLteTimingInfo    timingInfo
));
/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
 Void rgSCHCmnRawCqiInd ARGS
((
RgSchCellCb        *cellCb,
RgSchUeCb          *ueCb,
TfuRawCqiRpt      *rawCqiRpt,
CmLteTimingInfo    timingInfo
));

 Void rgSCHCmnSrsInd ARGS
((
RgSchCellCb        *cellCb,
RgSchUeCb          *ueCb,
TfuSrsRpt      *srsRpt,
CmLteTimingInfo    timingInfo
));
#endif /* TFU_UPGRADE */ 

Void rgSCHCmnDlTARpt ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue
));
RgSchPdcch *rgSCHCmnCmnPdcchAlloc ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm
));
RgSchUlAlloc *rgSCHCmnUlSbAlloc ARGS((
RgSchUlSf       *sf,
uint8_t              numSb,
RgSchUlHole     *hole
));
Void rgSCHCmnRlsUlSf ARGS((
RgSchCellCb    *cell,
uint8_t idx
));

/* PHR handling for MSG3 */
Void rgSCHCmnUlRecMsg3Alloc ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchRaCb                  *raCb
));

/* Added periodic BSR timer */

/*ccpu00117180 - ADD - Added Prototype in .x since the function access is now */
Void rgSCHCmnUpdVars ARGS((
RgSchCellCb *cell
));

#ifdef LTEMAC_SPS
Void rgSCHCmnFillHqPTb ARGS((
RgSchCellCb                *cell,
RgSchDlRbAlloc             *rbAllocInfo,
uint8_t                         tbAllocIdx,
RgSchPdcch                 *pdcch
));

Void rgSCHCmnDlProcAck ARGS((
RgSchCellCb        *cell,
RgSchDlHqProcCb    *hqP
));
Void rgSCHCmnHdlCrntiCE ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue
));
Void rgSCHCmnDlRelPdcchFbk ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
Bool               isAck
));
Void rgSCHCmnDlGetRbgInfo ARGS((
uint8_t             dlTotalBw,
uint8_t             dlSubsetBw,
uint8_t             maxRaType1SubsetBw,
uint8_t             rbgSize,
RgSchBwRbgInfo *rbgInfo
));
uint8_t rgSCHCmnDlRaType0Alloc ARGS((
RgSchDlSfAllocInfo *allocedInfo, 
uint8_t                 rbsReq,
RgSchBwRbgInfo     *rbgInfo,
uint8_t                 *numAllocRbs,
RgSchDlSfAllocInfo *resAllocInfo, 
Bool               isPartialAlloc
));
#ifdef RGSCH_SPS_UNUSED
uint8_t rgSCHCmnDlRaType1Alloc ARGS((
RgSchDlSfAllocInfo *allocedInfo, 
uint8_t                 rbsReq,
RgSchBwRbgInfo     *rbgInfo,
uint8_t                 startRbgSubset,
uint8_t                 *allocRbgSubset,
RgSchDlSfAllocInfo *resAllocInfo, 
Bool               isPartialAlloc
));
#endif
uint8_t rgSCHCmnDlRaType2Alloc ARGS((
RgSchDlSfAllocInfo *allocedInfo, 
uint8_t                 rbsReq,
RgSchBwRbgInfo     *rbgInfo,
uint8_t                 *rbStart,
RgSchDlSfAllocInfo *resAllocInfo, 
Bool               isPartialAlloc
));
Bool rgSCHCmnAllocUeInSpsBw ARGS((
RgSchDlSf           *dlSf,
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgSchDlRbAlloc      *rbAllocInfo,
Bool                isPartialAlloc
));
#endif
Void rgSCHCmnDrxStrtInActvTmrInUl ARGS((RgSchCellCb *cell));
Void rgSCHCmnUpdUeDataIndLcg ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgInfUeDatInd  *datInd));
#ifdef LTE_TDD
uint8_t  rgSCHCmnGetPhichUlSfIdx ARGS((CmLteTimingInfo *timeInfo, RgSchCellCb *cell));
uint8_t  rgSCHCmnGetUlSfIdx ARGS((CmLteTimingInfo *timeInfo, RgSchCellCb *cell));
uint8_t rgSCHCmnGetPValFrmCCE ARGS((RgSchCellCb *cell, uint8_t cce));
#endif
uint8_t  rgSCHCmnGetUlHqProcIdx ARGS((CmLteTimingInfo *timeInfo, RgSchCellCb *cell));

Void rgSchCmnSetCqiReqField ARGS((
 RgSchUeCellInfo   *cellInfo,
 RgSchUeCb         *ue,
 RgSchCqiReqField  *cqiReq 
));

/* APIs exposed by COMMON SCHEDULER to
 * SPECIFIC SCHEDULER */
/* UL_ALLOC_CHANGES */
Void rgSCHCmnUlFreeAlloc ARGS((
RgSchCellCb     *cell,
RgSchUlAlloc    *alloc  
));
#ifndef EMTC_ENABLE
Void rgSCHCmnUlFreeAllocation ARGS((
RgSchCellCb     *cell,
RgSchUlSf       *sf,    
RgSchUlAlloc    *alloc  
));
#else
Void rgSCHCmnUlFreeAllocation ARGS((
RgSchCellCb     *cell,
RgSchUlSf       *sf,    
RgSchUlAlloc    *alloc,
Bool             isEmtcUe
));
#endif
/* APIs exposed by DL RB allocation module */
S16 rgSCHCmnAllocDlRb ARGS((
RgSchCellCb         *cell, 
RgSchCmnDlRbAllocInfo  *dlRbAllocInfo
));

/* APIs exposed by UL RB allocation module */
Void rgSCHCmnAllocUlRb ARGS((
RgSchCellCb            *cell, 
RgSchCmnUlRbAllocInfo  *ulRbAllocInfo
));

/* APIs Exposed to Specific Scheduler */
RgSchPdcch *rgSCHCmnPdcchAllocCrntSf ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue
));
Void rgSCHCmnPdcchRlsCrntSf ARGS((
RgSchCellCb                *cell,
RgSchPdcch                 *pdcch
));
Void rgSCHCmnUlFillPdcchWithAlloc ARGS((
RgSchPdcch      *pdcch,
RgSchUlAlloc    *alloc,
RgSchUeCb       *ue
));
Void rgSCHCmnUlAllocFillTpc ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchUlAlloc *alloc
));
Void rgSCHCmnUlAllocFillNdmrs ARGS((
RgSchCmnUlCell *cellUl,
RgSchUlAlloc   *alloc
));
Void rgSCHCmnUlAllocLnkHqProc ARGS((
RgSchUeCb       *ue,
RgSchUlAlloc    *alloc,
RgSchUlHqProcCb *proc,
Bool            isReTx
));
RgSchPdcch *rgSCHCmnPdcchAlloc ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlSf                  *subFrm,
uint8_t                         cqi,
TfuDciFormat               dciFrmt,
Bool                       isDtx 
));
Void rgSCHCmnRdcImcsTxTb ARGS((
RgSchDlRbAlloc   *allocInfo,
uint8_t               tbInfoIdx,
uint32_t              cnsmdBytes
));
Void rgSCHCmnFillPdcch ARGS((
RgSchCellCb                *cell,
RgSchPdcch                 *pdcch,
RgSchDlRbAlloc             *rbAllocInfo
));
uint8_t rgSCHCmnUpdDai ARGS((
RgSchUeCb         *ue,
CmLteTimingInfo   *fdbkTime,
uint8_t                 m,
Bool               havePdcch,
RgSchDlHqProcCb   *hqP,
uint8_t                *ulDai

));
Void rgSCHCmnFillHqPPdcch ARGS((
RgSchCellCb                *cell,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchDlHqProcCb            *hqP
));
S16 rgSCHCmnDlChkResAvl ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
RgSchDlHqProcCb            *proc,
uint32_t                        *bo,
uint8_t                         *iTbs,
uint32_t                        *maxRb
));
S16 rgSCHCmnDlDedAlloc ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
RgSchDlHqProcCb            *proc,
uint32_t                        bo,
uint8_t                         iTbs,
uint32_t                        maxRb,
uint32_t                        *bytes
));
Void rgSCHCmnUlUeFillAllocInfo  ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue
));
/* Fixing incorrect Imcs derivation */
uint8_t rgSCHCmnUlGetITbsFrmIMcs ARGS((
uint8_t          iMcs
));
/* Adding ueCtg to argument list */
uint8_t rgSCHCmnUlGetIMcsFrmITbs ARGS((
uint8_t              iTbs,
CmLteUeCategory ueCtg
));
uint32_t rgSCHCmnUlMinTbBitsForITbs ARGS((
RgSchCmnUlCell     *cellUl,
uint8_t                 iTbs
));
uint8_t rgSCHCmnUlGetITbs ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue,
Bool              isEcp
));
Void rgSCHCmnUlAllocFillRbInfo ARGS((
RgSchCellCb    *cell,
RgSchUlSf      *sf,
RgSchUlAlloc   *alloc
));
Void rgSCHCmnDlUeResetTemp ARGS((
RgSchUeCb               *ue,
RgSchDlHqProcCb         *hqP
));
Void rgSCHCmnUlUeResetTemp ARGS((
RgSchCellCb             *cell,
RgSchUeCb               *ue
));
/* proc is added for DTX support */
/* DL per UE RB allocation API */
S16 rgSCHCmnDlAllocTxRb ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
uint32_t                        bo,
uint32_t                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
Bool rgSCHCmnIsDlCsgPrio ARGS((
RgSchCellCb    *cell
));
Bool rgSCHCmnIsUlCsgPrio ARGS((
RgSchCellCb    *cell
));
S16 rgSCHCmnDlAllocRetxRb ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
uint32_t                        bo,
uint32_t                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
#ifdef LTEMAC_SPS
Void rgSCHCmnClcRbAlloc ARGS((
RgSchCellCb             *cell,
uint32_t                     bo,
uint8_t                      cqi,
uint8_t                      *rb,
uint32_t                     *tbs,
uint8_t                      *mcs,
uint8_t                      *iTbs,
Bool                    isSpsBo,
RgSchDlSf               *sf 
));
uint32_t rgSCHCmnCalcRiv ARGS((
uint8_t           bw,
uint8_t           rbStart,
uint8_t           numRb
));
#endif /* LTEMAC_SPS */

/* end: Apis to add Ues in to DlRbAllocInfo Lists */
/* start: Apis to add Ues in to UlRbAllocInfo Lists */
Void rgSCHCmnUlAdd2UeLst ARGS((
RgSchCellCb           *cell,
RgSchCmnUlRbAllocInfo *allocInfo,
RgSchUeCb             *ue
));
Void rgSCHCmnUlAdd2CntResLst ARGS((
RgSchCmnUlRbAllocInfo *allocInfo,
RgSchUeCb             *ue
));
Void rgSCHCmnRmvFrmTaLst ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue
));
/* end: Apis to add Ues in to UlRbAllocInfo Lists */
Void rgSCHCmnUlUpdOutStndAlloc ARGS((
RgSchCellCb *cell,
RgSchUeCb   *ue,
uint32_t alloc
));

Void rgSCHCmnUlRecordUeAlloc ARGS((
RgSchCellCb *cell,
RgSchUeCb   *ue
));



/* APIs exposed by common power module */
Void rgSCHPwrInit ARGS((
         Void));
uint8_t rgSCHPwrPuschTpcForUe ARGS((
         RgSchCellCb *cell,
         RgSchUeCb   *ue));
uint8_t rgSCHPwrGetMaxUlRb ARGS((
         RgSchCellCb *cell,
         RgSchUeCb   *ue));
uint8_t rgSCHPwrPucchTpcForUe ARGS((
         RgSchCellCb *cell,
         RgSchUeCb   *ue));
Void rgSCHPwrGrpCntrlPucch ARGS((
         RgSchCellCb *cell,
         RgSchDlSf   *dlSf));
Void rgSCHPwrGrpCntrlPusch ARGS((
         RgSchCellCb *cell,
         RgSchDlSf   *dlSf,
         RgSchUlSf   *ulSf));
Void rgSCHPwrPucchDeltaInd ARGS((
         RgSchCellCb *cell,
         RgSchUeCb   *ue,
         S8          pwrDelta));
Void rgSCHPwrUpdExtPhr ARGS((
         RgSchCellCb           *cell,
         RgSchUeCb             *ue,
         RgInfExtPhrCEInfo     *extPhr,
         RgSchCmnAllocRecord   *allocInfo));
Void rgSCHPwrUpdPhr ARGS((
         RgSchCellCb           *cell,
         RgSchUeCb             *ue,
         uint8_t                     phr,
         RgSchCmnAllocRecord   *allocInfo,
         S8                     maxUePwr));
Void rgSCHPwrUlCqiInd ARGS((
         RgSchCellCb           *cell,
         RgSchUeCb             *ue
         ));
Void rgSCHPwrRecordRbAlloc ARGS((
         RgSchCellCb           *cell,
         RgSchUeCb             *ue,
         uint8_t                     numRb
         ));
S16 rgSCHPwrCellCfg ARGS((
         RgSchCellCb          *cell,
         RgrCellCfg           *cfg));
S16 rgSCHPwrCellRecfg ARGS((
         RgSchCellCb          *cell,
         RgrCellRecfg         *recfg));
Void rgSCHPwrCellDel ARGS((
         RgSchCellCb *cell));

#ifdef LTE_ADV
S16 rgSCHPwrUeSCellCfg ARGS((
         RgSchCellCb *cell,
         RgSchUeCb   *ue,
         RgrUeSecCellCfg  *sCellInfoCfg));
#endif

S16 rgSCHPwrUeCfg ARGS((
         RgSchCellCb *cell,
         RgSchUeCb   *ue,
         RgrUeCfg    *cfg));
S16 rgSCHPwrUeRecfg ARGS((
         RgSchCellCb   *cell,
         RgSchUeCb     *ue,
         RgrUeRecfg    *recfg));
Void rgSCHPwrUeDel ARGS((
         RgSchCellCb   *cell,
         RgSchUeCb     *ue));
Void rgSCHPwrUeReset ARGS((
         RgSchCellCb   *cell,
         RgSchUeCb     *ue));

#ifdef LTEMAC_SPS
S16 rgSCHCmnSpsUlProcCrcInd ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      crcTime
));
Void rgSCHCmnSpsInit ARGS((Void));

Void rgSCHCmnSpsRelDlSpsActHqP ARGS((
RgSchCellCb        *cell,
RgSchDlHqProcCb    *hqP));

S16 rgSCHCmnSpsCellCfg ARGS((
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg,
RgSchErrInfo       *err));
Void rgSCHCmnSpsCellDel ARGS((
RgSchCellCb        *cell
));
S16 rgSCHCmnSpsUeCfg ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeCfg           *ueCfg,
RgSchErrInfo       *err
));
S16 rgSCHCmnSpsUeRecfg ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeRecfg         *ueRecfg,
RgSchErrInfo       *err
));
Void rgSCHCmnSpsUeDel ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue
));
S16 rgSCHCmnSpsDlLcRecfg ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlLcCb        *dlLc,
RgrLchRecfg        *lcRecfg,
RgSchErrInfo       *err
));
S16 rgSCHCmnSpsDlLcCfg ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlLcCb        *dlLc,
RgrLchCfg          *lcCfg,
RgSchErrInfo       *err
));
Void rgSCHCmnSpsDlLcDel ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlLcCb        *dlLc
));
Void rgSCHCmnSpsDlCqiIndHndlr ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      timingInfo
));
Void rgSCHCmnSpsDlDedBoUpd ARGS((
RgSchCellCb            *cell,
RgSchUeCb              *ue,
RgSchDlLcCb            *dlLc
));
Void rgSCHCmnSpsDlUeReset ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue
));
Void rgSCHCmnSpsDlProcAddToRetx ARGS((
RgSchCellCb           *cell,
RgSchDlHqProcCb       *hqP
));
Void rgSCHCmnSpsDlProcAck ARGS((
RgSchCellCb           *cell,
RgSchDlHqProcCb       *hqP
));

Void rgSCHCmnSpsDlRelPdcchFbk ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue,
Bool                  isAck
));
Void rgSCHCmnSpsDlSched ARGS((
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));
#ifdef RG_UNUSED
S16 rgSCHCmnSpsGetDlActvUe ARGS((
RgSchCellCb     *cell,
CmLteTimingInfo *timingInfo,
CmLListCp       *dlSpsActvUeLst
));
#endif
Void rgSCHCmnSpsDlAllocFnlz ARGS((
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));

Void rgSCHCmnSpsDlUpdDlSfAllocWithSps ARGS((
RgSchCellCb           *cell,
CmLteTimingInfo       schdTime,
RgSchDlSf             *dlSf
));


/* APIs exposed by UL SPS */
Void rgSCHCmnSpsUlLcgDel ARGS(( 
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchLcgCb       *lcg
));
Void rgSCHCmnSpsUlUeReset ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue
));
Void rgSCHCmnSpsUlProcRelInd ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
Bool                isExplRel
));

Void rgSCHCmnSpsUlProcActInd ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
uint16_t                 spsSduSize
));
Void rgSCHCmnSpsPhrInd ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue
));


S16 rgSCHCmnSpsBsrRpt ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *ulLcg
));


S16 rgSCHCmnSpsUlCqiInd ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue
));
S16 rgSCHCmnSpsUlProcDtxInd ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      dtxTime
));
S16 rgSCHCmnSpsUlTti ARGS((
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
));
#ifdef RG_UNUSED
S16 rgSCHCmnSpsUlGetActvUeLst  ARGS((
RgSchCellCb     *cell,
CmLteTimingInfo timingInfo,
CmLListCp       *ulSpsActvUeLst
));
#endif
Void rgSCHCmnUlSpsRelInd ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
Bool               isExplRel
));


Void rgSCHCmnUlSpsActInd ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
uint16_t                spsSduSize
));

Void rgSCHCmnUlCrcFailInd ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
CmLteTimingInfo    crcTime
));
Void rgSCHCmnUlCrcInd ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
CmLteTimingInfo    crcTime
));

/* Added handling to retrnasmit RelPDCCH in case no
                             feedback is received */
Void rgSCHCmnSpsDlReTxRelPdcch ARGS((
RgSchCellCb        *cell
));
#endif

Void rgSCHCmnChkRetxAllowDtx
ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ueCb,
RgSchDlHqProcCb    *proc,
Bool               *reTxAllwd
));

S16 PtUiRgmBndCfm ARGS((Pst* pst, SuId suId, uint8_t status));

 S16 rgSCHCmnDlInitHqEnt 
ARGS((
RgSchCellCb  *cell,
RgSchDlHqEnt  *hqEnt
));

Void rgSchCmnDlSfHqDel 
ARGS((
RgSchUeCb    *ue,
RgSchCellCb  *cell
));

Void rgSCHCmnDlDeInitHqEnt 
ARGS((
RgSchCellCb  *cell,
RgSchDlHqEnt *hqE
));
uint8_t rgSCHCmnUlGetCqi
ARGS ((
RgSchCellCb  *cell,
RgSchUeCb *ue,
CmLteUeCategory  ueCtgy
));
#ifdef DL_LA
S16 rgSCHDhmUpdBlerBasediTbsEff ARGS((
         RgSchCellCb  *cell,
         RgSchUeCb               *ueCb,
         uint8_t                      tbCnt
         ));
#endif

Void rgSchCmnUpdCfiDb ARGS((
         RgSchCellCb     *cell,
         uint8_t              delta                
         ));
S16 RgUiRgmChangeTransModeInd ARGS((
         Pst               *pst, 
         SuId              suId, 
         RgmTransModeInd   *transModeInd));

Void rgSchCheckAndTriggerModeChange ARGS((
RgSchCellCb    *cell,
RgSchUeCb      *ue,
uint8_t             reportediTbs,
uint8_t             previTbs,
uint8_t             maxiTbs
));
Void rgSCHRrDlProcRmvFrmRetx ARGS((
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
));
uint8_t rgSchUtlGetServCellIdx ARGS((
         Inst         inst,
         uint16_t          cellId,
         RgSchUeCb    *ue
         ));
S16 rgSchUtlVldtCellId ARGS ((
 Inst         inst,
 uint16_t          cellId
));
Void rgSCHCmnInitUlRbAllocInfo ARGS((
RgSchCmnUlRbAllocInfo  *allocInfo
));
TfuDciFormat rgSCHCmnSlctPdcchFrmt ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
uint8_t                         *raType
));

Void rgSCHCmnNonDlfsDedRbAlloc ARGS((
RgSchCellCb        *cell,
RgSchCmnUeRbAlloc  *allocInfo,
CmLListCp          *ueLst,
CmLListCp          *schdUeLst,
CmLListCp          *nonSchdUeLst
));

Bool rgSCHCmnRetxAvoidTdd ARGS 
((
RgSchDlSf                  *curSf,
RgSchCellCb                *cell,
RgSchDlHqProcCb            *proc
));



#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RGSCHCMNX__ */


/**********************************************************************
         End of file
**********************************************************************/
