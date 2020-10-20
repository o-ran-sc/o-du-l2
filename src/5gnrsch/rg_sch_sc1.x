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
 
    File:   rg_sch_sc1.x
 
**********************************************************************/
/** @file rg_sch_sc1.x
@brief This file contains data structures for the SC1 scheduler.
*/
 
#ifndef __RGSCHSC1X__
#define __RGSCHSC1X__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef Void (*RgSchSc1SvcMngFunc) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchDlLcCb *svc));

/**
  * @brief
  * Cell specific downlink scheduling information for Scheduler type 1.
  */
typedef struct rgSc1DlCell
{
   CmLListCp          prioLst[RG_SC1_DL_NUM_Q];   /*!< Priority queues */
   CmLListCp          retxLst[RG_SC1_DL_NUM_Q];   /*!< Retransmission queues */
   RgSchSc1SvcMngFunc svcMngFunc[RG_SC1_DL_NUM_Q]; /*!< Function for managing
                                                        services */
} RgSchSc1DlCell;

/**
  * @brief
  * Cell specific uplink scheduling information for Scheduler type 1.
  */
typedef struct rgSc1UlCell
{
   CmLListCp     contResLst;/*!< UEs with pending UL alloc for msg4 */
   CmLListCp     ueTxLst[2];/*!< Queue for UEs Ded Data Trans
                             *   ueTxLst[0] for signalling and
                             *   ueTxLst[1] for other data */
} RgSchSc1UlCell;

/**
  * @brief
  * Uplink information for scheduler per UE.
  */
typedef struct rgSc1UlUe
{
   CmLList      txLnk;      /* Lnk to one of Transmission Queues */
   CmLList      contResLnk; /* Lnk to one of Cont Res Queue */
   Bool         srRcvd;     /* TRUE if SR reported is yet to be satisfied */
   uint8_t           qId;        /* Id of the Tx Q to which this UE belongs */
} RgSchSc1UlUe;

/**
  * @brief
  * Downlink information for scheduler per UE.
  */
typedef struct rgSc1DlUe
{
   CmLList           prioLnk; /*!< To link UE into priority queues */
   CmLListCp         gbrSvcs; /*!< List of GBR services */
   uint8_t                prio;    /*!< Current priority of UE for DL */
   CmLListCp         ambrLst; /*!< To maintain services per priority for a UE */
   RgSchDlLcCb       *ambrSvc;/*!< Points to the current AMBR service */
   uint32_t               ambr;    /*!< UE's Remaining AMBR */
   uint32_t               effAmbr; /*!< min(svc->bo, ambr)*/
   CmLListCp         schdSvcs; /*!< List of services for which 
                                 allocation requested */
   uint8_t                taReqBytes;/*!< Set to the Number of bytes Requested for TA
                                  allocation */
   CmLListCp         retxHqProcs; /*!< List of RETX Hq Procs in suspension */
   CmLListCp         lcsWithData; /*!< List of services with non-zero effBo */
} RgSchSc1DlUe;

/**
  * @brief
  * Downlink HARQ information for SC1 scheduler per UE.
  */
typedef struct rgSc1DlHqProc {
   CmLList        retxLnkUe; /*!< To link retransmitting HARQ processes in ue */
   uint8_t             cqi;     /*!< CQI at which the allocation was made */
   uint8_t             prio;    /*!< Priority of the allocation */
}RgSchSc1DlHqProc;

/**
  * @brief
  * Downlink service information for SC1 scheduler per UE.
  */
typedef  struct rgSc1DlSvc {
   uint32_t          gbr;     /*!< Pending GBR to be satisfied     */
   uint32_t          mbr;     /*!< Pending MBR to be satisfied     */
   uint32_t          bo;      /*!< BO yet to be satisfied */
   uint32_t          effGbr;  /*!< GBR/BO, lower of the two        */
   uint32_t          effMbr;  /*!< MBR/BO, lower of the two        */
   CmLList      gbrLnk;  /*!< used to maintain svcs to be refreshed */
   CmLList      prioLnk; /*!< Used to queue up services on UE */
   CmLList      schdSvcLnk; /*!< Used to queue up services on UE 
                             * scheduled svcs list */
   uint32_t          reqBytes; /*!< Set to number of bytes Req for Allocation */
   uint16_t          hdrEstimate; /*!< RLC Hdr est computed during SVC allocn */
   CmLList      lcWithDataLnk;/*!< Used to maintain svc in ue's 
                               *   lcsWithData List */
} RgSchSc1DlSvc;


Void rgSCHSc1DlLcRmv ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));
Void rgSCHSc1DlLcBoUpd ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));
Void rgSCHSc1DlProcAddToCellRetx ARGS((
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
));
Void rgSCHSc1DlAllocFnlz ARGS((
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));
Void rgSCHSc1UlAllocFnlz ARGS((
RgSchCellCb           *cell,
RgSchCmnUlRbAllocInfo *allocInfo
));
Void rgSCHSc1UlCqiInd ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue,
TfuUlCqiRpt          *ulCqiInfo
));
S16 rgSCHSc1UlLcgUpd ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgInfUeDatInd       *datInd
));

Void rgSCHSc1DlCqiInd ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
Bool               isPucchInfo,
Void               *dlCqi
));
Void rgSCHSc1UlUeRefresh ARGS((
RgSchCellCb    *cell,
RgSchUeCb      *ue
));
Void rgSCHSc1UlUeReset ARGS((
RgSchCellCb    *cell,
RgSchUeCb      *ue
));
Void rgSCHSc1DlUeRefresh ARGS((
RgSchCellCb    *cell,
RgSchUeCb      *ue
));
Void rgSCHSc1DlHndlInActUes ARGS((
RgSchCellCb           *cell,
CmLListCp             *inactvLst
));
Void rgSCHSc1UlHndlInActUes ARGS((
RgSchCellCb           *cell,
CmLListCp             *inactvLst
));
Void rgSCHSc1UlActvtUe ARGS((
RgSchCellCb    *cell,
RgSchUeCb      *ue
));
Void rgSCHSc1DlActvtUe ARGS((
RgSchCellCb    *cell,
RgSchUeCb      *ue
));
Void rgSCHSc1DlInit ARGS((
RgDlSchdApis *apis
));
Void rgSCHSc1UlInit ARGS((
RgUlSchdApis *apis
));
S16 rgSCHSc1RgrUlCellCfg ARGS((
RgSchCellCb   *cell,
RgrCellCfg    *cellCfg,
RgSchErrInfo  *err
));
S16 rgSCHSc1RgrDlCellCfg ARGS((
RgSchCellCb   *cell,
RgrCellCfg    *cellCfg,
RgSchErrInfo  *err
));
Void rgSCHSc1UlCellDel ARGS((
RgSchCellCb     *cell
));
Void rgSCHSc1DlCellDel ARGS((
RgSchCellCb     *cell
));
S16 rgSCHSc1RgrUlUeCfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeCfg     *ueCfg,
RgSchErrInfo *err
));
S16 rgSCHSc1RgrDlUeCfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeCfg     *ueCfg,
RgSchErrInfo *err
));
S16 rgSCHSc1RgrUlUeRecfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg,
RgSchErrInfo *err
));
S16 rgSCHSc1RgrDlUeRecfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg,
RgSchErrInfo *err
));
Void rgSCHSc1UlUeDel ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
Void rgSCHSc1DlUeDel ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
S16 rgSCHSc1RgrLcCfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchDlLcCb  *dlLc,
RgrLchCfg *lcCfg,
RgSchErrInfo *err
));
S16 rgSCHSc1RgrLcgCfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *lcg,
RgrLcgCfg    *lcgCfg,
RgSchErrInfo *err
));
S16 rgSCHSc1RgrLcRecfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchDlLcCb  *dlLc,
RgrLchRecfg  *lcRecfg,
RgSchErrInfo *err
));
S16 rgSCHSc1RgrLcgRecfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *lcg,
RgrLcgRecfg  *reCfg,
RgSchErrInfo *err
));
Void rgSCHSc1LcgDel ARGS((
RgSchCellCb   *cell,
RgSchUeCb     *ue,
RgSchLcgCb    *lcg
));
Void rgSCHSc1UpdBsrShort ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *lcg,
uint8_t           bsr
));
Void rgSCHSc1UpdBsrTrunc ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *lcg,
uint8_t           bsr
));
Void rgSCHSc1UpdBsrLong ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
uint8_t bsArr[]
));
Void rgSCHSc1ContResUlGrant ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
Void rgSCHSc1SrRcvd ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
Void rgSCHSc1UlSched ARGS((
RgSchCellCb           *cell,
RgSchCmnUlRbAllocInfo *allocInfo
));
Void rgSCHSc1DlSched ARGS((
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));
S16 rgSCHSc1RgrUlCellRecfg ARGS((
RgSchCellCb             *cell,
RgrCellRecfg            *recfg,
RgSchErrInfo            *err
));
Void rgSCHSc1DlUeReset ARGS((
RgSchCellCb    *cell,
RgSchUeCb      *ue
));

S16 rgSCHSc1DlUeHqEntInit ARGS((
RgSchCellCb      *cell,
RgSchDlHqEnt     *hqEnt
));

S16 rgSCHSc1DlUeHqEntDeInit ARGS((
RgSchCellCb      *cell,
RgSchDlHqEnt     *hqE
));

S16 rgSCHSc1UlLchCfg ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrLchCfg        *cfg,
RgSchErrInfo     *err
));

S16 rgSCHSc1UlLchRecfg ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrLchRecfg      *recfg,
RgSchErrInfo     *err
));

S16 rgSCHSc1UlLchDel ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue,
CmLteLcId        lcId,
uint8_t               lcgId
));
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RGSCHSC1X__ */


/**********************************************************************
         End of file
**********************************************************************/
