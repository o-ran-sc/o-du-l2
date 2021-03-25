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
 
     Desc:     C source code for Entry point functions.
 
     File:     rg_sch_cfg.c
 
**********************************************************************/

/** @file rg_sch_cfg.c
@brief This module handles the configuration of SCH by RRC and RRM.
*/


/* header include files -- defines (.h) */
#include "common_def.h"
#include "lrg.h"
#include "rgr.h"
#include "rgm.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch.h"
#include "rg_sch_err.h"
#include "rg_sch_cmn.h"

/* header/extern include files (.x) */
#include "tfu.x"           /* TFU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"            /* typedefs for MAC */
#include "rg_sch.x"            /* typedefs for MAC */
/* [ccpu00124018]-MOD- Retrieving CQI value from cell config*/
#include "rg_sch_cmn.x"   
#include "rg_sch_clist.x"


/* LTE-MAC Scheduler instance control block structures */
RgSchCb rgSchCb[RGSCH_MAX_INST];

#ifdef PHY_ERROR_LOGING
RgSchUlAllocCntr rgSchUlAllocCntr;
#endif

#ifdef EMTC_ENABLE
Void rgSCHEmtcPOTrigger ARGS ((
RgSchCellCb   *cell,
RgSchUeCb     *ue
));

S16 rgSchEmtcUpdSiCfg ARGS ((
RgSchCellCb   *cell,
RgrCellRecfg  *cellRecfg
));

S16 rgSCHEmtcCfgVldtDrxReTxCfg ARGS((
uint16_t       reTxTmr
));

S16 rgSCHEmtcCfgVldtDrxUlReTxCfg ARGS((
uint16_t       reTxTmr
));
#endif

S16 rgSCHEnbPfsDlCfg ARGS((
         Inst                instIdx,
         RgSchErrInfo       *err
         ));
/* local defines */
static S16 rgSCHCfgRgrUeRecfgRntiChg ARGS (( RgSchCellCb *cell,
      RgSchUeCb *ue, RgrUeRecfg *ueRecfg, RgSchErrInfo *errInfo));
static S16 rgSCHCfgVldtUePwrCfg ARGS((RgSchCellCb *cell,
         RgrUeUlPwrCfg *pwrCfg));
static S16 rgSCHCfgVldtUeGrpPwrCfg ARGS((RgSchCellCb *cell,
         RgrUeGrpPwrCfg *grpPwrCfg));
#ifdef LTEMAC_SPS
static S16 rgSCHCfgVldtUeDlSpsCfg ARGS((RgSchCellCb *cell,
         RgrUeSpsDlCfg *dlSpsCfg));
static S16 rgSCHCfgVldtSpsReCfg ARGS ((RgSchCellCb *cell,
         RgSchUeCb *ue, RgrUeRecfg *ueRecfg));
#endif /*LTEMAC_SPS*/
static S16 rgSCHCfgVldtUeCqiModeCfg ARGS((RgSchCellCb *cell,RgrUeDlCqiCfg *ueDlCqiCfg));
static S16 rgSCHCfgVldtUeMeasGapAckNakRepCfg ARGS ((RgSchCellCb *cell,
         RgrUeCfg *ueCfg));
static S16 rgSCHCfgVldtUeMeasGapAckNakRepRecfg ARGS ((RgSchCellCb *cell,
         RgrUeRecfg *ueRecfg));
static Void rgSCHCfgFreeDlDedLcCb ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *dlLc));
static Void rgSCHCfgFreeDlCmnLcCb ARGS((RgSchClcDlLcCb *cmnDlLc));
static Void rgSCHCfgFreeUeCb ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
static Void rgSCHCfgFreeRgrCfgLst ARGS((RgSchCellCb *cell));
static Void rgSCHCfgFreeCmnLcLst ARGS((RgSchCellCb *cell));
static Void rgSCHCfgFreeUeLst ARGS((RgSchCellCb *cell));
#ifdef LTEMAC_SPS
static Void rgSCHCfgFreeSpsUeLst ARGS((RgSchCellCb *cell));
#endif
static S16 rgSCHCfgVldtRgrCmnLcCfg ARGS((Inst inst, RgrCellCfg *cellCfg,
                         RgSchErrInfo *errInfo));
static S16 rgSCHCfgVldtRgrCellPwrCfg ARGS((Inst inst, RgrCellCfg *cellCfg,
         RgSchErrInfo *errInfo));
static S16 rgSCHCfgVldtRgrCellSchCfg ARGS((Inst inst, RgrCellCfg *cellCfg));
static S16 rgSCHCfgVldtRgrSchCfg ARGS((Inst inst, RgrSchedEnbCfg *schedEnbCfg));
static S16 rgSCHCfgVldtRgrCellRACfg ARGS((Inst inst, RgrCellCfg *cellCfg));
static Void rgSCHCfgRgrUePhrMsg3 ARGS(( RgSchCellCb *cell,
             RgSchRaCb *raCb,RgSchUeCb *ue, RgSchErrInfo *errInfo));
static S16 rgSCHCfgRgrCmnLcCfg ARGS((RgSchCellCb *cell, RgrCmnLchCfg *lcCfg,
                         RgSchErrInfo *errInfo));
Void rgSCHSCellFreeBuf ARGS((Inst inst,RgSchUeCb *ue,RgrUeRecfg *ueRecfg,uint8_t idx));    
#ifdef RGR_SI_SCH
static S16 rgSCHCfgVldtRgrCellSiCfg ARGS(( Inst inst, RgrSiCfg  *siCfg));
#endif/*RGR_SI_SCH */

/* LTE_ADV_FLAG_REMOVED_START */
static S16 rgSCHCfgVldtRgrCellLteAdvCfg ARGS(( Inst inst, 
                                 RgrLteAdvancedCellConfig *lteAdvCfg, uint8_t dlTotalBw));
/* LTE_ADV_FLAG_REMOVED_END */

static S16 rgSCHCfgVldtDrxUeCfg ARGS ((RgSchCellCb *cell,
                                       RgrUeDrxCfg *ueDrxCfg));
static S16 rgSCHCfgVldtDrxOnDurCfg ARGS((uint8_t onDurTmr));
static S16 rgSCHCfgVldtDrxInActvCfg ARGS((uint16_t inActvTmr));
static S16 rgSCHCfgVldtDrxReTxCfg ARGS((uint8_t reTxTmr));
static S16 rgSCHCfgVldtDrxLngCycCfg ARGS((RgrDrxLongCycleOffst lngCycleOffst));
static S16 rgSCHCfgVldtDrxLngCyclTmrs ARGS((uint16_t val));
static S16 rgSCHCfgVldtDrxShrtCycCfg ARGS((RgrDrxShortDrx shrtCycCfg));
static S16 rgSCHCfgVldtRgrCellCsgParamCfg ARGS((Inst inst,
                          RgrCellCsgParamCfg   *csgParam));
#ifdef TFU_UPGRADE

S16 rgSCHCfgACqiUeCfg ARGS(( RgSchCellCb *cellCb,RgSchUeCb *ue, RgSchUeACqiCb *aCqiCb, 
                     RgrTxMode ueTxMode,RgrUeAprdDlCqiCfg *aCqiCfg, CmLteUeCategory ueCat ));

S16 rgSCHCfgAcqiUeReCfg ARGS(( RgSchCellCb *cellCb, RgSchUeCb *ueCb, 
 RgrUeAprdDlCqiCfg   *acqiCfg, CmLteUeCategory  ueCat ));

S16 rgSCHUtlGetCfgPerOff ARGS(( RgSchPerTbl tbl, uint16_t  cfgIdx, 
         uint16_t *peri, uint16_t *offset ));

S16 rgSCHCfgRiUeCfg ARGS((RgSchCellCb  *cellCb, RgSchUeCb    *ueCb, 
                   RgrUePrdDlCqiCfg *cqiCfg, CmLteUeCategory ueCat ));

S16 rgSCHCfgPCqiUeCfg ARGS((RgSchCellCb  *cellCb, RgSchUeCb    *ueCb, 
                     RgrUePrdDlCqiCfg *cqiCfg,CmLteUeCategory ueCat ));

S16 rgSCHCfgSrsUeCfg ARGS((RgSchCellCb   *cellCb,RgSchUeCb     *ueCb,
                     RgrUeUlSrsCfg *srsCfg ));

S16 rgSCHCfgSrUeCfg ARGS((RgSchCellCb   *cellCb,RgSchUeCb     *ueCb,
                     RgrUeSrCfg    *srCfg));

S16 rgSCHCfgPCqiUeReCfg ARGS((RgSchCellCb  *cellCb, RgSchUeCb *ueCb, 
                     RgrUePrdDlCqiCfg *cqiCfg,CmLteUeCategory ueCat));

S16 rgSCHCfgSrsUeReCfg ARGS((RgSchCellCb  *cellCb, RgSchUeCb *ueCb, 
                     RgrUeUlSrsCfg *srsCfg));

S16 rgSCHCfgSrUeReCfg ARGS((RgSchCellCb  *cellCb, RgSchUeCb *ueCb, 
                      RgrUeSrCfg   *srCfg));

S16 rgSCHCfgVldtRgrTxmodePuschMode ARGS((RgSchCellCb  *cellCb,
         RgrTxMode  txMde, RgrAprdCqiMode puschMode,RgSchErrInfo *errInfo));

S16 rgSCHCfgVldtRgrUeACqiCfg ARGS(( RgSchCellCb *cellCb,
         CmLteRnti  crnti, RgrUeAprdDlCqiCfg *acqiCfg, RgrUeTxModeCfg txMode,
         RgSchErrInfo  *errInfo ));

S16 rgSCHCfgVldtRgrTxmodePucchMode ARGS((RgSchCellCb  *cellCb,
         RgrTxMode  txMde, RgrPrdCqiMode pucchMode,RgSchErrInfo *errInfo));

#ifdef LTEMAC_HDFDD
S16 rgSCHCfgVldtRgrUePCqiCfg ARGS(( RgSchCellCb *cellCb,
         CmLteRnti  crnti, RgrUePrdDlCqiCfg *cqiCfg, Bool hdFdd,
         RgrUeTxModeCfg txMode, RgSchErrInfo *errInfo));
#else
S16 rgSCHCfgVldtRgrUePCqiCfg ARGS(( RgSchCellCb *cellCb,
         CmLteRnti  crnti, RgrUePrdDlCqiCfg *cqiCfg, RgrUeTxModeCfg txMode,
         RgSchErrInfo *errInfo));         
#endif
#ifdef LTEMAC_HDFDD
S16 rgSCHCfgVldtRgrUeUlSrsCfg  ARGS ((RgSchCellCb *cellCb,
             CmLteRnti crnti, RgrUeUlSrsCfg *srsCfg, Bool hdFdd, 
             RgSchErrInfo *errInfo));
#else
S16 rgSCHCfgVldtRgrUeUlSrsCfg  ARGS ((RgSchCellCb *cellCb,
         CmLteRnti crnti, RgrUeUlSrsCfg *srsCfg, 
         RgSchErrInfo *errInfo));
#endif

#ifdef LTEMAC_HDFDD
S16 rgSCHCfgVldtRgrUeSrCfg  ARGS((RgSchCellCb *cellCb, CmLteRnti crnti,  
         RgrUeSrCfg *srCfg, Bool hdFdd, RgSchErrInfo *errInfo));
#else
S16 rgSCHCfgVldtRgrUeSrCfg  ARGS((RgSchCellCb *cellCb, CmLteRnti crnti,  
         RgrUeSrCfg *srCfg, RgSchErrInfo *errInfo));
#endif
static S16 rgSCHCfgVldtCqiSrSrsUeCfg ARGS (( RgSchCellCb       *cellCb,
                                             RgrUeCfg          *ueCfg,  
                                             RgSchErrInfo      *errInfo));

static S16 rgSCHCfgVldtCqiSrSrsUeReCfg ARGS (( RgSchCellCb  *cellCb,
                                               RgSchUeCb    *ueCb,
                                               RgrUeRecfg   *ueReCfg,  
                                               RgSchErrInfo *errInfo ));

S16 rgSCHCfgPCqiSrsSrUeDel ARGS ((RgSchCellCb *cellCb, 
                                         RgSchUeCb *ueCb));

static Void rgSCHCfgUtlFetchAcqiBitSz ARGS (( RgSchUeACqiCb *acqiCb,uint8_t  numTxAnt,
                     uint8_t* cqiPmiSzR1,uint8_t* cqiPmiSzRn1 ));

/*  Added the function to be used instead of the 
 *  MACRO RG_SCH_GET_PERIODICITY_TBL */
static const RgSchUePCqiSrsSrCfgIdxTbl*  rgSCHCfgUtlGetPcqiSrsSrRiTbl ARGS ((
                                           RgSchPerTbl tblType, 
                                           uint8_t * min, 
                                           uint8_t * max));

#endif /* TFU_UPGRADE */ 
static Void rgSCHCfgUeTaRecfg ARGS (( RgSchCellCb  *cellCb,
                                               RgSchUeCb    *ueCb,
                                               RgrUeRecfg   *ueReCfg,  
                                               RgSchErrInfo *errInfo ));
#ifdef LTE_ADV
S16 rgSCHSCellCfgUeCfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg,
RgSchErrInfo *err
));

S16 rgSCHSCellCfgUePucchReCfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg,
RgSchErrInfo *err
));
S16 rgSCHCfgVldtRgrUeSCellRecfg ARGS((
RgrUeRecfg     *ueRecfg,
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgSchErrInfo   *errInfo
));
static S16 rgSCHSCellCfgUeCfgRollBack ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg
));
#endif

/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#if ((defined (RGR_CQI_REPT)) && (defined (RGR_V2)))
static S16 rgSCHCfgUeCqiReptReCfg ARGS ((
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgrUeRecfg     *ueRecfg
));

static S16 rgSCHCfgVldtCqiReptReCfg ARGS ((
RgSchCellCb       *cell,
RgrUeRecfg     *ueRecfg
));
#endif
/*LTE_L2_MEAS_PHASE2*/
#ifdef LTE_L2_MEAS
static S16 rgSchAddToL2Meas ARGS ((RgSchCellCb *cellCb,RgSchDlLcCb *dlLc));
#endif
#ifdef EMTC_ENABLE
S16 rgSCHEmtcCfgVldtDrxOnDurCfg
(
uint16_t      onDurTmr 
);
Void rgSCHUtlUpdEmtcY
(
RgSchUeCb                  *ue
);
Void rgSCHEmtcHqPAlloc 
(
RgSchCellCb       *cell,
RgSchDlHqEnt      *hqEnt
);
#endif

/* local typedefs */
const RgSchSrsTxOffst rgSrsTxOffstTbl[RGSCH_MAX_SRS_SFCFG_IDX+1]=
                                  {{1,{0,0,0,0,0,0,0,0}},
                                   {1,{0,0,0,0,0,0,0,0}},
                                   {1,{1,0,0,0,0,0,0,0}},
                                   {1,{0,0,0,0,0,0,0,0}},
                                   {1,{1,0,0,0,0,0,0,0}},
                                   {1,{2,0,0,0,0,0,0,0}},
                                   {1,{3,0,0,0,0,0,0,0}},
                                   {2,{0,1,0,0,0,0,0,0}},
                                   {2,{2,3,0,0,0,0,0,0}},
                                   {1,{0,0,0,0,0,0,0,0}},
                                   {1,{1,0,0,0,0,0,0,0}},
                                   {1,{2,0,0,0,0,0,0,0}},
                                   {1,{3,0,0,0,0,0,0,0}},
                                   {7,{0,1,2,3,4,6,8,0}},
                                   {8,{0,1,2,3,4,5,6,8}},
                                   {0,{0,0,0,0,0,0,0,0}}};
 
/* local externs */

#ifdef TFU_UPGRADE
/* SRSCfg Table: Ref 36.213, Table: 8.2.1  */
const RgSchUePCqiSrsSrCfgIdxTbl  
rgSchUeSrsCfgIdxFddTbl[RG_SCH_SRS_ISRS_INDX_MAX_FDD]=
{
 { 0,1,    2,  0 },
 { 2,6,    5,  2 },
 { 7,16,   10, 7 },
 { 17,36,  20, 17},
 { 37,76,  40, 37},
 { 77,156, 80, 77},
 { 157,316,160,157},
 { 317,636,320,317 }
};

/* Reference : 36.213 Table 8.2-2 */
const RgSchUePCqiSrsSrCfgIdxTbl 
rgSchUeSrsCfgIdxTddTbl[RG_SCH_SRS_ISRS_INDX_MAX_TDD]=
{
 { 10,14,  5,  10 },
 { 15,24,  10, 15 },
 { 25,44,  20, 25 },
 { 45,84,  40, 45 },
 { 85,164, 80, 85 },
 { 165,324,160, 165 },
 { 325,644,320, 325 }
 /* RESERVED: Configuration Module should not allow Res values */
};

/*Reference: 36.213 Table:7.2.2-1A */
const RgSchUePCqiSrsSrCfgIdxTbl 
rgSchUePCqiCfgIdxFddTbl[RG_SCH_CQIPMI_CFGIDX_MAX_FDD]=
{
 { 0,1,   2, 0 },
 { 2,6,   5, 2 },
 { 7,16,  10,7 },
 {17,36,  20,17},
 {37,76,  40, 37},
 {77,156, 80, 77},
 {157,316,160,157},
 {318,349,32,318}, 
 {350,413,64, 350},
 {414,541,128,414 }
 /* RESERVED: Configuration should not allow Res values */
};

/* Reference: 36.213 Table:7.2.2-1C */
const RgSchUePCqiSrsSrCfgIdxTbl 
rgSchUeCqiPmiCfgIdxTddTbl[RG_SCH_CQIPMI_CFGIDX_MAX_TDD]=
{
 { 0,0,   1, 0 },
 { 1,5,   5, 1 },
 { 6,15,  10,6 },
 {16,35,  20,16},
 {36,75,  40,36},
 {76,155, 80,76},
 {156,315,160,156}
 /* RESERVED: Configuration should not allow Res values */
};

/* Note: RI table is same for FDD and TDD */
/*Reference: 36.213 Table:7.2.2-1B */
const RgSchUePCqiSrsSrCfgIdxTbl 
rgSchUeRiCfgIdxTbl[RG_SCH_RI_CFGIDX_MAX]=
{
 { 0,160,    1, 0 },
 { 161,321,  2, 161 },
 { 322,482,  4, 322 },
 { 483,643,  8, 483},
 { 644,804,  16,644},
 { 805,965,  32,805 }
 /* RESERVED: Configuration should not allow Res values */
};

/*Reference: 36.213 Table:7.2.2-2 */
const RgSchUeBwSubSzBwParts 
rgSchUeBwSubSzBwPartsTbl[RG_SCH_BW_SUBSZ_BWPARTS_MAX]=
{
 {6,7, 0, 0},  /*TODO: 6,7, NA, NA */
 {8, 10, 4,1},
 {11, 26, 4,2},
 {27, 63, 6,3},
 {64, 110, 8,4}
};


/* Reference : 36.213 Table 10.1-5 */
/* Note: SR is same table for TDD and FDD */
const RgSchUePCqiSrsSrCfgIdxTbl rgSchUeSrCfgIdxTbl[RG_SCH_ISR_INDX_MAX]=
{
 { 0,4,   5, 0 },
 { 5,14,   10, 5 },
 { 15,34,  20,15 },
 { 35,74, 40,35},
 { 75,154, 80, 75}
 /* RESERVED: Configuration should not allow Res values */
};

/*Reference:36.213: Derived from Table: 7.2.1-5 for Label L.
 The Label L is CEIL(log2(BinCoe(N and M)))*/
const uint8_t RgSCHUeAcqi2022LBitWidth[6][28] ={
{0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5},
{0,0,2,3,4,4,5,5,6,6,6,7,7,7,7,7,8,8,8,8,8,8,8,9,9,9,9,9},
{0,0,0,2,4,5,6,6,7,7,8,8,9,9,9,10,10,10,10,11,11,11,11,11,12,12,12,12},
{0,0,0,0,3,4,6,7,7,8,9,9,10,10,11,11,12,12,12,13,13,13,14,14,14,14,15,15},
{0,0,0,0,0,3,5,6,7,8,9,10,11,11,12,13,13,14,14,14,15,15,16,16,16,17,17,17},
{0,0,0,0,0,0,3,5,7,8,9,10,11,12,13,13,14,15,15,16,16,17,17,18,18,18,19,19}
};

#endif 
 
extern uint8_t rgSchCmnHarqRtt[];
#ifdef EMTC_ENABLE
S16 rgSCHEmtcCellAlloc ARGS((RgSchCellCb *cel)); 
Void rgSCHEmtcCellFree ARGS((RgSchCellCb *cel)); 
Void rgSCHEmtcUeInfoFree ARGS((RgSchCellCb *cell, RgSchUeCb *ue)); 
uint8_t rgSchfillPucchSrRepNumCountemtc ARGS((RgSchUeCb *ueCb)); 
uint8_t rgSchfillPucchCqiRepNumCountemtc ARGS((RgSchUePCqiCb *cqiCb, RgSchUeCb *ueCb)); 
S16 rgEmtcvalidateSiCfg ARGS((RgrSiCfgReqInfo *siCfg,RgSchCellCb  *cell));
#endif
/* forward references */


/**
 * @brief Validates the SCH EndoeB configuration request from RRM to SCH.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrSchedEnbCfg
 *
 **     Processing Steps:
 *      - Retrieve the cell control block.
 *      - If successful,
 *        - Validate the range of configured values recieved in
 *          configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *  @param[in]  Inst               inst
 *  @param[in]  RgrSchedEnbCfg    *schedEnbCfg
 *  @param[out] RgSchCellCb      **cell
 *  @param[out] RgSchErrInfo      *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgVldtRgrSchedEnbCfg
(
Inst              inst,
RgrSchedEnbCfg   *schedEnbCfg,
RgSchErrInfo     *errInfo
)
{
   DU_LOG("\nDEBUG  -->  SCH : VALIDATE RGR SCH ENB CONFIG: \n");

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_ENB_CFG;

   // TODO
   if ((rgSCHCfgVldtRgrSchCfg(inst, schedEnbCfg)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Validation for scheduler related "
                          "config failed\n");
      return RFAILED;
   }

   DU_LOG("\nINFO  -->  SCH : MIMO_DBG:: SCH:: numAntPorts=%d\n",schedEnbCfg->numTxAntPorts);
   
   /* Validation for the ENB parameters */
   if ((schedEnbCfg->numTxAntPorts == 0) || (schedEnbCfg->numTxAntPorts > 4))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid number of transmit antenna" 
            " ports %d\n", schedEnbCfg->numTxAntPorts);
      return RFAILED;
   }

   /* Validate csg access mode */
   if((schedEnbCfg->accsMode < RGR_CELL_ACCS_OPEN) ||
      (schedEnbCfg->accsMode > RGR_CELL_ACCS_HYBRID))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid CSG Access mode\n");
      return RFAILED;
   }

   errInfo->errCause = RGSCHERR_NONE;
   DU_LOG("\nINFO  -->  SCH : RGR SCHED ENB config validation done:");

   return ROK;
}  /* rgSCHCfgVldtRgrSchedEnbCfg */

/**
 * @brief Validates the cell configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrCellCfg
 *
 *     Processing Steps:
 *      - Retrieve the cell control block.
 *      - If successful,
 *        - Validate the range of configured values recieved in
 *          configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *  @param[in]  Inst         inst
 *  @param[in]  RgrCellCfg   *cellCfg
 *  @param[out] RgSchCellCb  **cell
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgVldtRgrCellCfg
(
Inst         inst,
RgrCellCfg   *cellCfg,
RgSchCellCb  *cell,
RgSchErrInfo *errInfo
)
{
   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_CELL_CFG;

   /* check if cell exists already */
   if ((uint8_t *)cell != NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : Cell Id already exists");
      return RFAILED;
   }
   
   if(cellCfg->macInst >= SCH_INST_START)
   {
      DU_LOG("\nERROR  -->  SCH : Invalid MAC Instance %d ",
               cellCfg->macInst);
      return RFAILED;
   }
   
   if (cellCfg->macRnti.startRnti < RGSCH_MIN_MAC_RNTI )
   {
      DU_LOG("\nERROR  -->  SCH : Invalid start RNTI %d for cell ",
               cellCfg->macRnti.startRnti);
      return RFAILED;
   }

   if ((rgSCHCfgVldtRgrCellSchCfg(inst, cellCfg)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Validation for scheduler related "
                          "config failed");
      return RFAILED;
   }

   if ((cellCfg->dlHqCfg.maxDlHqTx < RGSCH_MIN_HQ_TX) ||
         (cellCfg->dlHqCfg.maxMsg4HqTx < RGSCH_MIN_HQ_TX))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid Downlink HARQ configuration:"
               " maxDlHqTx %d maxMsg4HqTx %d", cellCfg->dlHqCfg.maxDlHqTx,
               cellCfg->dlHqCfg.maxMsg4HqTx);
      return RFAILED;
   }
   if ((cellCfg->cfiCfg.cfi < RGSCH_MIN_CFI_VAL) ||
         (cellCfg->cfiCfg.cfi > RGSCH_MAX_CFI_VAL))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid CFI configuration %d",
           cellCfg->cfiCfg.cfi);
      return RFAILED;
   }
   if (((cellCfg->puschSubBand.subbandStart) + 
        ((cellCfg->puschSubBand.numSubbands -1) * cellCfg->puschSubBand.size))
         > (cellCfg->bwCfg.ulTotalBw - 1))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid PUSCH subband configuration:"
               " subBandStart %d numSubBands %d subBandSize %d ulTotBw %d",
               cellCfg->puschSubBand.subbandStart, 
               cellCfg->puschSubBand.numSubbands, cellCfg->puschSubBand.size,
               cellCfg->bwCfg.ulTotalBw);
      return RFAILED;
   }

   if (((cellCfg->bwCfg.dlTotalBw < RGSCH_MIN_DL_BW) ||
            (cellCfg->bwCfg.dlTotalBw > RGSCH_MAX_DL_BW)) ||
         ((cellCfg->bwCfg.ulTotalBw < RGSCH_MIN_UL_BW) || 
          (cellCfg->bwCfg.ulTotalBw > RGSCH_MAX_UL_BW)))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid Bandwidth configuration:"
            " ul %d dl %d",cellCfg->bwCfg.ulTotalBw,
            cellCfg->bwCfg.dlTotalBw);
      return RFAILED;
   }
   if (cellCfg->phichCfg.ngEnum > RGR_NG_TWO)
   {
      DU_LOG("\nERROR  -->  SCH : Invalid PHICH Ng configuration %d",
               (uint8_t)cellCfg->phichCfg.ngEnum);
      return RFAILED;
   }
   /* Validation for extended PHICH Duration */
   if ((cellCfg->phichCfg.isDurExtend == TRUE) && 
         (cellCfg->bwCfg.dlTotalBw <= 10) && (cellCfg->cfiCfg.cfi < 2))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid cfi value for"
               "Extended PHICH duration cfi:%d dlBw:%d",
               (uint8_t)cellCfg->cfiCfg.cfi, cellCfg->bwCfg.dlTotalBw);
      return RFAILED;
   }
   if ((cellCfg->phichCfg.isDurExtend == TRUE) &&
         (cellCfg->bwCfg.dlTotalBw > 10) && (cellCfg->cfiCfg.cfi < 3))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid cfi value for"
               "Extended PHICH duration cfi:%d dlBw:%d",
               (uint8_t)cellCfg->cfiCfg.cfi, cellCfg->bwCfg.dlTotalBw);
      return RFAILED;
   }
   DU_LOG("\nINFO  -->  SCH : CA_DBG:: PUCCH configuration:"
         " N2_RB %d N1_PUCCH %d deltaShift %d cyclicShift %d",
         cellCfg->pucchCfg.resourceSize,
         cellCfg->pucchCfg.n1PucchAn,
         cellCfg->pucchCfg.deltaShift,
         cellCfg->pucchCfg.cyclicShift);

   /* ccpu00138567- Removing validation check for resourceSize as 0.
    * From the spec, n2RB value 0 is a valid config. */
   if ((cellCfg->pucchCfg.resourceSize >= cellCfg->bwCfg.ulTotalBw/2) ||
         (cellCfg->pucchCfg.n1PucchAn == 0) ||
         (cellCfg->pucchCfg.deltaShift < RGSCH_PUCCH_MINVAL_DS) ||
         (cellCfg->pucchCfg.deltaShift > RGSCH_PUCCH_MAXVAL_DS) ||
         (cellCfg->pucchCfg.cyclicShift > RGSCH_PUCCH_MAXVAL_CS))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid PUCCH configuration:"
              " N2_RB %d N1_PUCCH %d deltaShift %d cyclicShift %d",
               cellCfg->pucchCfg.resourceSize,
               cellCfg->pucchCfg.n1PucchAn,
               cellCfg->pucchCfg.deltaShift,
               cellCfg->pucchCfg.cyclicShift);
      return RFAILED;
   }
   if (cellCfg->srsCfg.isSrsCfgSetup && cellCfg->srsCfg.srsBwEnum > RGR_SRS_BWCFG_7)
   {
      DU_LOG("\nERROR  -->  SCH : Invalid SRS configuration: "
               " srsBw %d", (uint8_t)cellCfg->srsCfg.srsBwEnum);
      return RFAILED;
   }

   if ((rgSCHCfgVldtRgrCellRACfg(inst, cellCfg)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Validation for Random access related"
                          "config failed");
      return RFAILED;
   }

   if ((rgSCHCfgVldtRgrCellPwrCfg(inst, cellCfg, errInfo)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Validation for cell power "
                          "config failed");
      return RFAILED;
   }

   /* Validate the common logical channel configuration */
   if( (cellCfg->numCmnLcs < RGR_MIN_CMN_LC_PER_CELL)||
       (cellCfg->numCmnLcs > RGR_MAX_CMN_LC_PER_CELL))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid number(%d) of common logical"
              "channels in cell config", cellCfg->numCmnLcs);
      return RFAILED;
   }
   if ((rgSCHCfgVldtRgrCmnLcCfg(inst, cellCfg, errInfo)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Validation for common logical"
                          "channels failed");
      return RFAILED;
   }

   /* Added 0 as a valid value for number of TICKs RRM
    * 0 implies no ticks i.e. shutting off the feature.*/

#ifdef RGR_SI_SCH
   if ((rgSCHCfgVldtRgrCellSiCfg(inst, &(cellCfg->siCfg))) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Validation for SI"
                          "configuration failed");
      return RFAILED;
   }
#endif /*RGR_SI_SCH */

   /*ccpu00116923 - ADD - Srs Present support - Start*/
#ifdef TFU_UPGRADE
#ifdef LTE_TDD
   if(cellCfg->srsCfg.isSrsCfgSetup && cellCfg->srsCfg.srsSubFrameCfg > 13)
#else
      if(cellCfg->srsCfg.isSrsCfgSetup && cellCfg->srsCfg.srsSubFrameCfg > 14)
#endif
      {
         DU_LOG("\nERROR  -->  SCH : Invalid"
                  "Subframe configuration");
         return RFAILED;
      }
#endif
   /*ccpu00116923 - ADD - Srs Present support - End*/
  
  if ((cellCfg->bcchTxPwrOffset > 10000) ||
       (cellCfg->pcchTxPwrOffset > 10000) ||
       (cellCfg->rarTxPwrOffset > 10000) ||
       (cellCfg->phichTxPwrOffset > 10000) 
      )
   {
      DU_LOG("\nERROR  -->  SCH : Invalid txPower offset ");

      return RFAILED;
   }

/* LTE_ADV_FLAG_REMOVED_START */
   /* Checking Whether DSFR is enabled without enabling SFR */
   if(((cellCfg->rgrLteAdvCfg.pres & RGR_DSFR) && 
       (RGR_ENABLE == cellCfg->rgrLteAdvCfg.dsfrCfg.status)) &&
      (!((cellCfg->rgrLteAdvCfg.pres & RGR_SFR) &&
         (RGR_ENABLE == cellCfg->rgrLteAdvCfg.sfrCfg.status))))
   {
      DU_LOG("\nERROR  -->  SCH : DSFR is enbaled"
               "Without enabling SFR");
      return RFAILED;
   }

   if ((rgSCHCfgVldtRgrCellLteAdvCfg(inst, &(cellCfg->rgrLteAdvCfg), 
                                      cellCfg->bwCfg.dlTotalBw)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Validation for LTE Adv"
               "configuration failed");
      return RFAILED;
   }
#ifdef LTE_ADV
   if ((rgSCHCfgVldtRgrCellLteLAACfg(inst, cellCfg)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Validation for LTE LAA"
               "configuration failed");
      return RFAILED;
   }
#endif
/* LTE_ADV_FLAG_REMOVED_END */
   if (cellCfg->msg4pAVal > RGRUE_DLPWRCNTRL_PA_DB3)
   {
      DU_LOG("\nERROR  -->  SCH : Invalid"
               "msg4pAVal %u", cellCfg->msg4pAVal);
      return RFAILED;
   }
  
   /* Validate RgrCellCsgParamCfg */
   if(rgSchCb[inst].rgrSchedEnbCfg.accsMode == RGR_CELL_ACCS_HYBRID) 
   {
      if((rgSCHCfgVldtRgrCellCsgParamCfg(inst, 
                  &(cellCfg->csgParamCfg)) != ROK))
      {
         DU_LOG("\nERROR  -->  SCH : Validation failed for \n"
                  "Access CSG parameter failed\n");
         return RFAILED;
      }
   }
#ifdef EMTC_ENABLE
          if (cellCfg->emtcEnable)
          {
   	     if(ROK != rgSCHCfgVldtRgrEmtcCellCfg(cellCfg))
   	     {
                DU_LOG("\nERROR  -->  SCH : Invalid EMTC cell Configuration %d for cell" ,cellCfg->cellId);
     			 return RFAILED;
   	     }
          }
#endif
   errInfo->errCause = RGSCHERR_NONE;
   return ROK;
}  /* rgSCHCfgVldtRgrCellCfg */

/**
 * @brief Validates the scheduler related configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrSchCfg
 *
 *     Processing Steps:
 *      - Validate the scheduler related configuration request from RRC to MAC at CFG:
 *        validate the value range for the configured values.
 *      - If validated successfully,
 *        - Return ROK .
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  Inst         inst
 *  @param[out] RgSchedCfg  *rgSchedCfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgVldtRgrSchCfg
(
Inst         inst,
RgrSchedEnbCfg   *rgSchedCfg
)
{
   DU_LOG("\nDEBUG  -->  SCH : rgSCHCfgVldtRgrSchCfg:Validating \
               scheduler related Configuration");
   if (rgSchedCfg->ulSchdType > (RGSCH_NUM_SCHEDULERS - 1))
   {
      DU_LOG("\nERROR  -->  SCH : rgSCHCfgVldtRgrSchCfg:Invalid \
                  UL scheduler type %d \n", rgSchedCfg->ulSchdType);
      return RFAILED;
   }
   if (rgSchedCfg->dlSchdType > (RGSCH_NUM_SCHEDULERS - 1))
   {
      DU_LOG("\nERROR  -->  SCH : rgSCHCfgVldtRgrSchCfg:Invalid \
                  DL scheduler type %d \n", rgSchedCfg->dlSchdType);
      return RFAILED;
   }
   return ROK;
}
/**
 * @brief Validates the scheduler related configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrCellSchCfg
 *
 *     Processing Steps:
 *      - Validate the scheduler related configuration request from RRC to MAC at CFG:
 *        validate the value range for the configured values.
 *      - If validated successfully,
 *        - Return ROK and pointer to the cell of UE.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  Inst         inst
 *  @param[out] RgSchCellCfg  *cellCfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgVldtRgrCellSchCfg
(
Inst         inst,
RgrCellCfg   *cellCfg
)
{

#if RGSCH_NUM_DLFS_SCHEDULERS
   if (cellCfg->dlfsSchdType > RGSCH_NUM_DLFS_SCHEDULERS - 1)
   {
      DU_LOG("\nERROR  -->  SCH : Invalid dlfs scheduler type %d for cell",
                cellCfg->dlfsSchdType);
      return RFAILED;
   }
#endif
   return ROK;
}
/**
 * @brief Validates the RACH related configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrCellRACfg
 *
 *     Processing Steps:
 *      - Validate the RA configuration request from RRC to MAC at CFG:
 *        validate the value range for the configured values.
 *      - If validated successfully,
 *        - Return ROK and pointer to the cell of UE.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  Inst         inst
 *  @param[out] RgSchCellCfg  *cellCfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgVldtRgrCellRACfg
(
Inst         inst,
RgrCellCfg   *cellCfg
)
{

#ifdef LTE_TDD
   if ((cellCfg->rachCfg.preambleFormat > RGSCH_MAX_TDD_RA_PREAMBLE_FMT) ||
#else
   if ((cellCfg->rachCfg.preambleFormat > RGSCH_MAX_RA_PREAMBLE_FMT) ||
#endif
         (cellCfg->rachCfg.raWinSize < RGSCH_MIN_RA_WINSIZE)  ||
         (cellCfg->rachCfg.raWinSize > RGSCH_MAX_RA_WINSIZE) ||
         (cellCfg->rachCfg.maxMsg3Tx < RGSCH_MIN_HQ_TX) ||
         (cellCfg->rachCfg.numRaPreamble < RGSCH_MIN_NUM_RA_PREAMBLE) ||
         (cellCfg->rachCfg.numRaPreamble > RGSCH_MAX_NUM_RA_PREAMBLE) ||
         (cellCfg->rachCfg.sizeRaPreambleGrpA >
          cellCfg->rachCfg.numRaPreamble) || 
         (cellCfg->rachCfg.prachResource > 
          (cellCfg->bwCfg.ulTotalBw - RGSCH_NUM_RA_RB)))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid RACH configuration:"
                "preamble Fmt %d raWinSize %d maxMsg3Tx %d",
                cellCfg->rachCfg.preambleFormat, cellCfg->rachCfg.raWinSize,
                cellCfg->rachCfg.maxMsg3Tx);
      DU_LOG("\nERROR  -->  SCH : Invalid numRaPreamble %d sizeRaPreambleGrpA %d",
                cellCfg->rachCfg.numRaPreamble,
                cellCfg->rachCfg.sizeRaPreambleGrpA);
      return RFAILED;
   }
   /* RACHO */
   /* verify that the ded Preambles cfgd for Pdcch Order
    * do not collide with that of non-dedicated  and validates against
    * the configuration index and number of RACH
    * ded-preambles. For non-zero ded preamble cfg, 
    * the config index is expected to be != NA.*/
   if ((cellCfg->macPreambleSet.pres) && 
       ((cellCfg->macPreambleSet.start < cellCfg->rachCfg.numRaPreamble) ||
       (cellCfg->macPreambleSet.start >= RGSCH_MAX_NUM_RA_PREAMBLE) ||
       (cellCfg->macPreambleSet.size < 1) ||
       (cellCfg->macPreambleSet.size > RGSCH_MAX_NUM_RA_PREAMBLE-
                                       cellCfg->rachCfg.numRaPreamble) ||
       (cellCfg->rachCfg.raOccasion.sfnEnum == RGR_SFN_NA)))
   {
       DU_LOG("\nERROR  -->  SCH : Invalid RACH Preambleset conf:"
                "preambleSet Start %d preambleSet Size %d",
                cellCfg->macPreambleSet.start, cellCfg->macPreambleSet.size);
      return RFAILED;
   }
#ifdef RGR_V1
   if(cellCfg->rachCfg.contResTmr)
   {
      uint8_t idx;
#ifdef LTE_TDD
      idx = cellCfg->ulDlCfgIdx;
#else
      idx = 7; /* FDD */
#endif
      /* maxMsg4TxDelay = (HARQ_RTT * MAX_MSG4_HARQ_RETX)  + 
         3 TTI (MAX L1+L2 processing delay at the UE) */
      uint8_t maxMsg4TxDelay = (cellCfg->dlHqCfg.maxMsg4HqTx-1) * 
         rgSchCmnHarqRtt[idx] + 3; 


      if(maxMsg4TxDelay >= cellCfg->rachCfg.contResTmr)
      {
         DU_LOG("\nERROR  -->  SCH : Warining !: Contention Resolution timer not greater than the "
               "guard timer. Conte Res timer %d Guard timer %d",
               cellCfg->rachCfg.contResTmr, 
               maxMsg4TxDelay );
         /* [ccpu00138532]-DEL- removed return fail here as it is ok if the 
            max Msg4 Tx delay is more than the contension Resolution timer. 
            In such case, the CRI CE will be scheduled immediately once 
            msg3 is received */ 
      }
   }
   else
   {
      /* ccpu00128575 ADD - If contention resolution timer is configured as 0,
         Then return fail*/
      DU_LOG("\nERROR  -->  SCH : Contention Resolution timer is configured as '0'");
      return RFAILED;
   }
#endif

   return ROK;
}

/**
 * @brief Validates the UE configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUeCfg
 *
 *     Processing Steps:
 *      - Validate the UE configuration request from RRC to MAC at CFG:
 *        validate the value range for the configured values.
 *      - If validated successfully,
 *        - Return ROK and pointer to the cell of UE.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  Inst         inst
 *  @param[in]  RgrUeCfg     *ueCfg
 *  @param[out] RgSchCellCb  **cell
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgVldtRgrUeCfg
(
Inst         inst,
RgrUeCfg     *ueCfg,
RgSchCellCb  **cell,
RgSchErrInfo *errInfo
)
{
   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_UE_CFG;

   if (((*cell) == NULLP) || 
       ((*cell)->cellId != ueCfg->cellId))
   {
      DU_LOG("\nERROR  --> SCH : Cell does not exist %d", 
           ueCfg->cellId);
      return RFAILED;
   }
   /* RACHO: 
    * Check configured preamble id not colliding with non dedicated or PDCCH
    * order preamble sets. When valid preamble id given check that C-RNTI given
    * in configuration is not amongst the C-RNTI'smanaged by scheduler */
   if ((rgSCHRamVldtUeCfg(*cell, ueCfg)) != ROK)
   {
      DU_LOG("\nERROR  --> SCH : CELLID:%d Preamble Id configuration"
               "failed ",ueCfg->cellId);
      return RFAILED;
   }
   /* Check if Ue already configured */
   if (rgSCHDbmGetUeCb(*cell, ueCfg->crnti) != NULLP)
   {
      DU_LOG("\nERROR  --> SCH : CELLID:%d UEID already exists",
             ueCfg->cellId);
      return RFAILED;
   }
   /* Validate Transmission UE modes  */
   if ((ueCfg->txMode.pres == TRUE) && ((ueCfg->txMode.txModeEnum < RGR_UE_TM_1)
            || (ueCfg->txMode.txModeEnum > RGR_UE_TM_7)))
   {
      DU_LOG("\nERROR  --> SCH : CELLID:%d Invalid transmission mode for"
               " UE is %d", ueCfg->cellId,(uint8_t)ueCfg->txMode.txModeEnum);
      return RFAILED;
   }
  
   /* Validate UE Category */
   if (ueCfg->ueCatEnum > CM_LTE_UE_CAT_8)
   {
      DU_LOG("\nERROR  --> SCH : CELLID:%d Invalid category for UE %d",
               ueCfg->cellId,(uint8_t)ueCfg->ueCatEnum);
      return RFAILED;
   }
  
   /* Validate UE Access Stratum Release */
   if (ueCfg->accessStratumRls > RGR_REL_10)
   {
      RGSCHDBGERRNEW(inst, (rgSchPBuf(inst), "Invalid Access Stratum Release %u for UE\n",
               ueCfg->accessStratumRls));
      return RFAILED;
   }
   RGSCHDBGINFONEW(inst, (rgSchPBuf(inst), "Configured Access Stratum Release %u\n", \
            ueCfg->accessStratumRls));
  
   if ((*cell)->numTxAntPorts == 1)
   {
      if ((ueCfg->txMode.pres == TRUE) && 
            (ueCfg->txMode.txModeEnum > RGR_UE_TM_1))
      {
         DU_LOG("\nERROR  --> SCH : CELLID:%d Invalid transmission mode for"
                  " UE (%d) for the configured Cell Antenna Ports",ueCfg->cellId,
                  (uint8_t)ueCfg->txMode.txModeEnum);
         return RFAILED;
      }
   }
   
   if ((rgSCHCfgVldtUeCqiModeCfg(*cell, &ueCfg->ueDlCqiCfg)) != ROK)
   {
      DU_LOG("\nERROR  --> SCH : CELLID:%d Invalid CQI Mode configuration",
                ueCfg->cellId);
      return RFAILED;
   }

   /* Validate Max Uplink HARQ transmission value */
   if (ueCfg->ueUlHqCfg.maxUlHqTx < RGSCH_MIN_HQ_TX)
   {
      DU_LOG("\nERROR  --> SCH : CELLID:%d Invalid Uplink HARQ config for "
            "UE %d",ueCfg->cellId,ueCfg->ueUlHqCfg.maxUlHqTx);
      return RFAILED;
   }

   if (rgSCHCfgVldtUePwrCfg(*cell, &ueCfg->ueUlPwrCfg) != ROK)
   {
      DU_LOG("\nERROR  --> SCH : CELLID:%d Invalid PUSCH Group power"
           " configuration",ueCfg->cellId);
      return RFAILED;
   }
 
   if (rgSCHCfgVldtUeMeasGapAckNakRepCfg(*cell, ueCfg) != ROK)
   {
      DU_LOG("\nERROR  --> SCH : CELLID:%d Invalid MeasGap/AckNackRep"
            " configuration",ueCfg->cellId);
      return RFAILED;
   }
  
#ifdef LTEMAC_SPS
   /* Validating SPS RNTI */
   if (((ueCfg->ueSpsCfg.spsRnti >=  (*cell)->rntiDb.rntiStart) && 
       (ueCfg->ueSpsCfg.spsRnti<=((*cell)->rntiDb.rntiStart+(*cell)->rntiDb.maxRntis)))
       ||(ueCfg->ueSpsCfg.spsRnti == RGSCH_SI_RNTI) 
       ||(ueCfg->ueSpsCfg.spsRnti == RGSCH_P_RNTI))
   {
      DU_LOG("\nERROR  --> SCH : CELLID:%d Invalid SPS RNTI "
               " in DL SPS Config",ueCfg->cellId);
      return RFAILED;
   }

   if (ueCfg->ueSpsCfg.dlSpsCfg.isDlSpsEnabled) 
   {
      if (rgSCHCfgVldtUeDlSpsCfg(*cell, &ueCfg->ueSpsCfg.dlSpsCfg) != ROK)
      {
         DU_LOG("\nERROR  --> SCH : CELLID:%d Invalid DL SPS configuration"
                  " for the UE",ueCfg->cellId);
         return RFAILED;
      }
   }
#endif 
#ifdef TFU_UPGRADE
   /* Validated Periodic CQI/PMI, RI , SRS and SR related UeCfg */
   if ( ROK != rgSCHCfgVldtCqiSrSrsUeCfg(*cell, ueCfg, errInfo))
   {
      DU_LOG("\nERROR  --> SCH : CELLID:%d Invalid Periodic CQI/SR/SRS"
               "configuration",ueCfg->cellId);
      return RFAILED;
   }
#endif

   /* Validate DRX specific parameters */
   if ( ROK != rgSCHCfgVldtDrxUeCfg(*cell, &(ueCfg->ueDrxCfg)))
   {
      DU_LOG("\nERROR  --> SCH : CELLID:%d Invalid DRX configuration",
                ueCfg->cellId);
      return RFAILED;
   }

/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   if (ueCfg->ueCqiReptCfg.numColltdCqiRept > RGR_CQIRPTS_MAXN)
   {
      DU_LOG("\nERROR  --> SCH : CELLID:%d Invalid numColltdCqiRept,"
               "MAX supported %d",RGR_CQIRPTS_MAXN,ueCfg->cellId);
      return RFAILED;
   }
#endif /* End of RGR_CQI_REPT */

#ifdef EMTC_ENABLE
/*This is to validate the EMTC related configuration if a UE is an EMTC UE*/
          if(TRUE == ueCfg->emtcUeCfg.pres)
          {
   	     if ( ROK != rgSCHCfgVldtEmtcUeCfg(*cell, &(ueCfg->emtcUeCfg)))
   	     {
      	        	 DU_LOG("\nERROR  --> SCH : CELLID:%d Invalid EMTC UE configurationfor crnti:%d",
                  		  ueCfg->cellId, ueCfg->crnti);
      		         return RFAILED;
   	     }
          }
#endif
   errInfo->errCause = RGSCHERR_NONE;
   return ROK;
}  /* rgSCHCfgVldtRgrUeCfg */


/**
 * @brief Validates the cell reconfiguration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrCellRecfg
 *
 *     Processing Steps:
 *      - Retrieve the cell control block.
 *      - If successful,
 *        - Validate the range of reconfigured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  Inst          inst
 *  @param[in]  RgrCellRecfg  *cellRecfg
 *  @param[out] RgSchCellCb   **cell
 *  @param[out] RgSchErrInfo  *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgVldtRgrCellRecfg
(
Inst          inst,
RgrCellRecfg  *cellRecfg,
RgSchCellCb   **cell,
RgSchErrInfo  *errInfo
)
{

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_CELL_RECFG;
   
   /* Fetch cell and validate cell Id with the cell control block*/
   if (((*cell) == NULLP) ||
       ((*cell)->cellId != cellRecfg->cellId)) 
   {
      DU_LOG("\nERROR  --> SCH : Cell control block does not exist");
      return RFAILED;
   }

   /* Validate recieved values */
   if ((cellRecfg->recfgTypes & RGR_CELL_DL_HARQ_RECFG) &&
         ((cellRecfg->dlHqRecfg.maxDlHqTx < RGSCH_MIN_HQ_TX) ||
          (cellRecfg->dlHqRecfg.maxMsg4HqTx < RGSCH_MIN_HQ_TX)))
   {
      DU_LOG("\nERROR  --> SCH : Invalid Downlink HARQ configuration:"
           " maxDlHqTx %d maxMsg4HqTx %d", cellRecfg->dlHqRecfg.maxDlHqTx,
            cellRecfg->dlHqRecfg.maxMsg4HqTx);
      return RFAILED;
   }
   if ((cellRecfg->recfgTypes & RGR_CELL_CFI_RECFG) && 
         ((cellRecfg->cfiRecfg.cfi < RGSCH_MIN_CFI_VAL) ||
          (cellRecfg->cfiRecfg.cfi > RGSCH_MAX_CFI_VAL)))
   {
      DU_LOG("\nERROR  --> SCH : Invalid CFI configuration %d",
               cellRecfg->cfiRecfg.cfi);
      return RFAILED;
   }
   if (cellRecfg->recfgTypes & RGR_CELL_PUCCH_RECFG)
   {
      /* ccpu00138567- Removing validation check for resourceSize as 0.
       * From the spec, n2RB value 0 is a valid config. */
      if ((cellRecfg->pucchRecfg.n1PucchAn == 0) ||
          (cellRecfg->pucchRecfg.resourceSize >= (*cell)->bwCfg.ulTotalBw/2)||
          ((cellRecfg->pucchRecfg.deltaShift < RGSCH_PUCCH_MINVAL_DS) || 
           (cellRecfg->pucchRecfg.deltaShift > RGSCH_PUCCH_MAXVAL_DS)) ||
          (cellRecfg->pucchRecfg.cyclicShift > RGSCH_PUCCH_MAXVAL_CS))
      {
         DU_LOG("\nERROR  --> SCH : Invalid PUCCH configuration: "
               "N2_RB %d N1_PUCCH %d deltaShift %d cyclicShift %d",
               cellRecfg->pucchRecfg.resourceSize,
               cellRecfg->pucchRecfg.n1PucchAn,
               cellRecfg->pucchRecfg.deltaShift,
               cellRecfg->pucchRecfg.cyclicShift);
         return RFAILED;
      }
   }
   if (cellRecfg->recfgTypes & RGR_CELL_SRS_RECFG)
   {
      if (cellRecfg->srsRecfg.isSrsCfgSetup && cellRecfg->srsRecfg.srsBwEnum > RGR_SRS_BWCFG_7)
      {
         DU_LOG("\nERROR  --> SCH : Invalid SRS configuration: "
               "srsBw %d", (uint8_t)cellRecfg->srsRecfg.srsBwEnum);
         return RFAILED;
      }

      /*ccpu00116923 - ADD - Srs Present support - Start*/
#ifdef TFU_UPGRADE
#ifdef LTE_TDD
      if(cellRecfg->srsRecfg.isSrsCfgSetup && cellRecfg->srsRecfg.srsSubFrameCfg > 13)
#else
         if(cellRecfg->srsRecfg.isSrsCfgSetup && cellRecfg->srsRecfg.srsSubFrameCfg > 14)
#endif
         {
            DU_LOG("\nERROR  --> SCH : Invalid Subframe configuration ");
            return RFAILED;
         }
#endif
      /*ccpu00116923 - ADD - Srs Present support - End*/
   }
   if (cellRecfg->recfgTypes & RGR_CELL_RACH_RECFG)
   {
      if ((cellRecfg->rachRecfg.preambleFormat > RGSCH_MAX_RA_PREAMBLE_FMT) ||
            ((cellRecfg->rachRecfg.raWinSize < RGSCH_MIN_RA_WINSIZE) ||
             (cellRecfg->rachRecfg.raWinSize > RGSCH_MAX_RA_WINSIZE)) ||
            (cellRecfg->rachRecfg.maxMsg3Tx < RGSCH_MIN_HQ_TX) ||
            ((cellRecfg->rachRecfg.numRaPreamble < RGSCH_MIN_NUM_RA_PREAMBLE)
            || (cellRecfg->rachRecfg.numRaPreamble > RGSCH_MAX_NUM_RA_PREAMBLE))
            || (cellRecfg->rachRecfg.sizeRaPreambleGrpA >
               cellRecfg->rachRecfg.numRaPreamble) ||
            (cellRecfg->rachRecfg.prachResource >
               (*cell)->bwCfg.ulTotalBw - RGSCH_NUM_RA_RB))
      {
         DU_LOG("\nERROR  --> SCH : Invalid RACH configuration:"
               " preamble Fmt %d raWinSize %d maxMsg3Tx %d",
               cellRecfg->rachRecfg.preambleFormat,
               cellRecfg->rachRecfg.raWinSize,
               cellRecfg->rachRecfg.maxMsg3Tx);
         DU_LOG("\nERROR  --> SCH : Invalid RACH configuration:"
               "numRaPreamble %d sizeRaPreambleGrpA %d",
               cellRecfg->rachRecfg.numRaPreamble,
               cellRecfg->rachRecfg.sizeRaPreambleGrpA);
         return RFAILED;
      }
   }

#ifdef RGR_SI_SCH
   if (cellRecfg->recfgTypes & RGR_CELL_SI_RECFG)
   {
      if ((rgSCHCfgVldtRgrCellSiCfg(inst, &(cellRecfg->siReCfg))) != ROK)
      {
         DU_LOG("\nERROR  --> SCH : Validation for SI"
                          "Re-configuration failed");
         return RFAILED;
      }
   }
#endif /*RGR_SI_SCH */

/* LTE_ADV_FLAG_REMOVED_START */
   if (cellRecfg->recfgTypes & RGR_CELL_LTEA_FEATURE_RECFG)
   {
      /* Checkin whether DSFR is enbaled without enabling SFR.
       * So we need to check if SFR is enabled along with DSFR
       * in the same reconfiguration or it is already enabled earlier*/
      if((cellRecfg->rgrLteAdvCfg.pres & RGR_DSFR) && 
         (RGR_ENABLE == cellRecfg->rgrLteAdvCfg.dsfrCfg.status)) 
      {   
         if(!(((cellRecfg->rgrLteAdvCfg.pres & RGR_SFR) &&
               (RGR_ENABLE == cellRecfg->rgrLteAdvCfg.sfrCfg.status)) || 
               ((*cell)->lteAdvCb.sfrCfg.status == RGR_ENABLE)))
         {
            DU_LOG("\nERROR  --> SCH : DSFR is enbaled"
                     "Without enabling SFR");
            return RFAILED;
         }
      }
      if ((rgSCHCfgVldtRgrCellLteAdvCfg(inst, &(cellRecfg->rgrLteAdvCfg),
                                       (*cell)->bwCfg.dlTotalBw)) != ROK)
      {
         DU_LOG("\nERROR  --> SCH : Validation for Lte Adv"
                  "Re-configuration failed");
         return RFAILED;
      }
   }
/* LTE_ADV_FLAG_REMOVED_END */

   /* Validating minimum resource for non-CSG users */
   if (cellRecfg->recfgTypes & RGR_CELL_CSG_PARAM_RECFG)
   {
     if (cellRecfg->csgParamCfg.minDlResNonCsg > 100)
     {
         DU_LOG("\nERROR  --> SCH : Invalid Configuration "
               "of minimum DL resources for NON-CSG");
         return RFAILED;
     }
     if (cellRecfg->csgParamCfg.minUlResNonCsg > 100)
     {
         DU_LOG("\nERROR  --> SCH : Invalid Configuration "
               "of minimum UL resources for NON-CSG");
         return RFAILED;
     }
   }
   errInfo->errCause = RGSCHERR_NONE;
   return ROK;
}  /* rgSCHCfgVldtRgrCellRecfg */

#ifdef LTE_ADV
/**
 * @brief Ue SCell configuration for scheduler. It is invoked during first time
 *        Scell configuration. It is not for reconfiguration
 *
 * @details
 *
 *     Function : rgSCHSCellCfgUeCfg
 *
 *     This functions updates UE specific scheduler
 *     information upon UE SCell first time Scell configuration
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[int] RgrUeRecfg   *ueRecfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHSCellCfgUeCfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg,
RgSchErrInfo *err
)
{
   uint8_t          idx;
   Inst             inst = cell->instIdx;
   RgSchCellCb      *secCellCb = NULLP;
   uint8_t          sCellidx = 0;
   RgSchUeCellInfo  *sCellInfo = NULLP;
   RgrUeSecCellCfg  *sCellInfoCfg = NULLP;
#ifdef TFU_UPGRADE
   RgrUeAprdDlCqiCfg   *aCqiCfg;
   RgrUePrdDlCqiCfg    *pCqiCfg;  
#endif

   DU_LOG("\nINFO   --> SCH : SCELL recfg received from APP \n");  

   RGSCHDBGINFONEW(inst, (rgSchPBuf(inst), 
     "--------------------------------------------------------------------\n"
     "UE SCell ReConfiguration at SCH: rnti (%u) cell(%u)\n"
     "--------------------------------------------------------------------\n",
     ue->ueId, cell->cellId));

 
   for(idx = 0; idx < ueRecfg->ueSCellCfgInfo.numSCells; idx++)
   {
     /* Allocate the Ue control block */
      if (((rgSCHUtlAllocSBuf(inst, (Data **)&sCellInfo,
                    sizeof(RgSchUeCellInfo))) != ROK))
      {
         RGSCHDBGERRNEW(inst, (rgSchPBuf(inst), "[%d]SCellIdx :Memomy allocation "
                  "Failed while Adding SCell Information\n", idx));
         return RFAILED;
      }
      

      sCellInfoCfg = &ueRecfg->ueSCellCfgInfo.ueSCellDedCfg[idx];


      sCellInfo->sCellIdx  = sCellInfoCfg->sCellIdx;
      sCellInfo->sCellId   = sCellInfoCfg->sCellId;
      
      if(PRSNT_NODEF == sCellInfoCfg->sCellDeActTmr.pres)
      {
         /* Configure implicit release */
         ue->sCellDeactTmrVal.val = sCellInfoCfg->sCellDeActTmr.val;
         ue->isScellExplicitDeAct = FALSE;
         ue->sCellDeactTmrVal.pres = PRSNT_NODEF;
      }
      else if (rgSchCb[cell->instIdx].genCfg.isSCellActDeactAlgoEnable == TRUE)
      {
         /* Configure explicit release */
         ue->sCellDeactTmrVal.val = RGSCH_SCELL_DEACT_TMR_INFINITY_VAL;
         ue->isScellExplicitDeAct = TRUE;
         ue->sCellDeactTmrVal.pres = PRSNT_NODEF;
      }
      else
      {
         ue->sCellDeactTmrVal.val  = 0;
         ue->isScellExplicitDeAct  = FALSE;
         ue->sCellDeactTmrVal.pres = NOTPRSNT;
      }

      sCellInfo->sCellState = RG_SCH_SCELL_INACTIVE;

      sCellInfo->ue = ue;
      ue->cellInfo[(sCellInfoCfg->sCellIdx)] = sCellInfo;
      sCellidx = ((sCellInfo->sCellId -
               rgSchCb[cell->instIdx].genCfg.startCellId)&(CM_LTE_MAX_CELLS-1));
      ue->cellIdToCellIdxMap[sCellidx] = sCellInfo->sCellIdx;

      /* For for time one Scell got added, setting allocCmnUlPdcch flag to TRUE, So that
         we will allocate PDCCH from common search space and the csiRequest field in DCI0 will 
         be one bit (spec 36.213 sec 7.2.1)*/
#ifdef LTE_ADV
      if ( ue->numSCells == 0)
      {
         ue->allocCmnUlPdcch = TRUE;
      }
#endif      
      ue->numSCells++;
#ifdef CA_DBG      
      DU_LOG("\nDEBUG  -->  SCH : SCell added for ue %d numScells %d\n",ue->ueId,ue->numSCells);
#endif
      /* retrieve teh sec cell Cb */
      if((secCellCb = (RgSchCellCb *)rgSchUtlGetCellCb(inst, sCellInfo->sCellId)) == NULLP)
      {
         DU_LOG("\nERROR  -->  SCH : SCell doesnt exists");
         rgSCHSCellFreeBuf (inst,ue,ueRecfg,idx);
         return RFAILED;
      }

      if(TRUE == sCellInfoCfg->txMode.pres)
      {
         sCellInfo->txMode = sCellInfoCfg->txMode;
      }
      else
      {
         DU_LOG("\nERROR  -->  SCH : [%d]SCellIdx Tx mode not present ",idx);
         sCellInfoCfg->txMode.pres = TRUE;
         sCellInfoCfg->txMode.txModeEnum = RGR_UE_TM_1;
      
         sCellInfo->txMode = sCellInfoCfg->txMode;
      }
      cmInitTimers (&sCellInfo->actDelayTmr, 1);
      cmInitTimers (&sCellInfo->deactTmr, 1);

      ue->f1bCsAVal += rgSCHUtlGetMaxTbSupp(sCellInfo->txMode.txModeEnum);

#ifdef TFU_UPGRADE
      if(TRUE == sCellInfoCfg->ueSCellDlCqiCfg.aprdCqiCfg.pres)
      {
         sCellInfo->acqiCb.aCqiCfg.aprdModeEnum = 
            sCellInfoCfg->ueSCellDlCqiCfg.aprdCqiCfg.aprdModeEnum;
      }

      if(TRUE == sCellInfoCfg->uePdschDedCfg.uepACfg.pAPrsnt)
      {
         sCellInfo->pA.pres = TRUE;
         sCellInfo->pA.val = sCellInfoCfg->uePdschDedCfg.uepACfg.pA;
      }
      else
      {
         sCellInfo->pA.pres = FALSE;
      }

      aCqiCfg = &sCellInfoCfg->ueSCellDlCqiCfg.aprdCqiCfg; 
      DU_LOG("\nDEBUG  -->  SCH : rgSCHCfgACqiUeCfg cellId =%d,Config Presence =%d for \
            Sec Cell Id = %d\n",
            cellCb->cellId,  aCqiCfg->pres,sCellInfo->sCellId);

      /* if aperiodic cqi is present then only call the below function as it is
       * not mandatory*/
      if(aCqiCfg->pres)
      {
         if( ROK != rgSCHCfgACqiUeCfg(secCellCb,ue, &sCellInfo->acqiCb,
                  sCellInfo->txMode.txModeEnum, aCqiCfg, ue->ueCatEnum))
         {
            DU_LOG("\nERROR  -->  SCH : [%d]SCellIdx ACQI Cfg"
                     "failed..n\n", idx);
            rgSCHSCellFreeBuf (inst,ue,ueRecfg,idx);
            return RFAILED;
         }
      }
      /* Configuring PCQI */ 
      /* Scell needs to be added to the
       * pcqi list only after activation */
      pCqiCfg = &sCellInfoCfg->ueSCellDlCqiCfg.prdCqiCfg;

      if(ROK != rgSCHSCellPCqiCfg(cell,secCellCb,ue,pCqiCfg,
            ue->ueCatEnum,sCellInfoCfg->sCellIdx))
      {
         DU_LOG("\nERROR  -->  SCH : [%d]SCellIdx PCQI Cfg failed..n\n", idx);
         rgSCHSCellFreeBuf (inst,ue,ueRecfg,idx);
         return RFAILED;
      }
#endif
   
      /* Configuring ACQI */ 
 
      /* Stroing the secCell for easy access */
      sCellInfo->cell   = secCellCb;


#ifdef LTE_ADV
      if (ROK != rgSCHLaaInitDlRbAllocCb(secCellCb, 
               &sCellInfo->dlAllocCb))
      {
         return RFAILED;
      }
#endif
      /* Initialize Harq entity */

      sCellInfo->hqEnt = rgSCHDhmHqEntInit(secCellCb);
      if (sCellInfo->hqEnt == NULLP)
      {
         DU_LOG("\nERROR  -->  SCH : [%d]UEID:Hq Entity Initialization "
                  "failed in config\n", ue->ueId);
         rgSCHSCellFreeBuf (inst,ue,ueRecfg,idx);
         return RFAILED;
      }
#ifdef EMTC_ENABLE
      rgSCHEmtcHqPAlloc(secCellCb, sCellInfo->hqEnt);
#endif
      rgSCHCmnDlInitHqEnt(secCellCb, sCellInfo->hqEnt);

      sCellInfo->hqEnt->ue = ue;
      /* Init SCell Specific Sched Spfc UE DL CB */
      if ((secCellCb->sc.apis->rgSCHRgrSCellUeCfg(secCellCb, ue, sCellInfoCfg, err)) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : Spec Sched DL UE CFG FAILED\n");
         rgSCHSCellFreeBuf (inst,ue,ueRecfg,idx);
         return RFAILED;
      }

#ifdef LTE_TDD
     if((rgSCHUtlAllocUeANFdbkInfo(ue,sCellInfoCfg->sCellIdx)) != ROK)
     {
        DU_LOG("\nERROR  -->  SCH : [%d]UEID:Memomy allocation "
                 "Failed while UE related Ack Nack Information\n",
                 ue->ueId);
        rgSCHSCellFreeBuf (inst,ue,ueRecfg,idx);
        return RFAILED;
     }
#endif /* LTE_TDD */

#ifdef LTE_ADV
     sCellInfo->sCellLnk.node = (PTR)sCellInfo;
     cmLListAdd2Tail(&secCellCb->sCellUeLst, &sCellInfo->sCellLnk);
#endif

      /* Inserting UECB into SCELL DBM */
     rgSCHDbmInsUeCb(secCellCb, ue);
   }

#ifndef MAC_5GTF_UPDATE
   ue->ul.useExtBSRSizes = ueRecfg->ueSCellCfgInfo.useExtBSRSizes;
#else
   ue->ul.useExtBSRSizes = TRUE;
#endif

   for (idx = 0; idx < RGSCH_ULCTRL_RECP_DIST; idx++)
   {
      ue->ul.ctrlOnServCellIdx[idx] = 0xFF;
   }
   /* Trigger SCell addition to primary MAC */
   return ROK;

}  /* rgSCHSCellCfgUeCfg */
/*f1b_Sprint */
/**
 * @brief UE SCell PUCCH reconfiguration for scheduler
 *
 * @details
 *
 *     Function : rgSCHSCellCfgUePucchReCfg
 *
 *     This functions updates UE specific scheduler
 *     information upon UE SCell PUCCH reconfiguration
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[int] RgrUeRecfg   *ueRecfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHSCellCfgUePucchReCfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg,
RgSchErrInfo *err
)
{
   Inst         inst = cell->instIdx;
   RgrUeSCellAckPucchCfg *sCellPucchRecfg = NULLP;
   uint8_t idx;

   RGSCHDBGINFONEW(inst, (rgSchPBuf(inst), 
     "--------------------------------------------------------------------\n"
     "UE SCell PUCCH ReConfiguration at SCH: rnti (%u) cell(%u)\n"
     "--------------------------------------------------------------------\n",
     ue->ueId, cell->cellId));

 
   sCellPucchRecfg = &ueRecfg->sCellAckN1ResCfg;
   /* Copy the UCI format type suported/configured for UE */
   ue->uciFrmtTyp = sCellPucchRecfg->pucchFormatType;
   
   if (ue->uciFrmtTyp == RG_SCH_UCI_FORMAT1B_CS)
   {
      ue->n1PucchF1bResCb.cw1N1ResCount = sCellPucchRecfg->u.format1Bcs.sCellAckN1ResTb1Count;
      ue->n1PucchF1bResCb.cw2N1ResCount = sCellPucchRecfg->u.format1Bcs.sCellAckN1ResTb2Count;

      for(idx = 0; idx < sCellPucchRecfg->u.format1Bcs.sCellAckN1ResTb1Count; idx++)
      {
         ue->n1PucchF1bResCb.cw1N1Res[idx].n1PucchIdx = sCellPucchRecfg->u.format1Bcs.sCellAckN1ResTb1[idx];
      }

      for(idx = 0; idx < sCellPucchRecfg->u.format1Bcs.sCellAckN1ResTb2Count; idx++)
      {
         ue->n1PucchF1bResCb.cw2N1Res[idx].n1PucchIdx = sCellPucchRecfg->u.format1Bcs.sCellAckN1ResTb2[idx];
      }
   }
#ifdef LTE_ADV
   else if (ue->uciFrmtTyp == RG_SCH_UCI_FORMAT3)
   {
      ue->n3PucchResCb.antP0N3ResCount = sCellPucchRecfg->u.format3.sCellAckN3ResAntP0Count;
      ue->n3PucchResCb.antP1N3ResCount = sCellPucchRecfg->u.format3.sCellAckN3ResAntP1Count;
      for (idx = 0;idx < ue->n3PucchResCb.antP0N3ResCount; idx++ )
      {
         ue->n3PucchResCb.antP0N3Res[idx].n3PucchIdx 
                        = sCellPucchRecfg->u.format3.sCellAckN3ResAntP0[idx];
         ue->n3PucchResCb.antP0N3Res[idx].n3Lnk.node = NULLP;
         ue->n3PucchResCb.antP0N3Res[idx].sCellIdx = RGSCH_INVALID_CELL_IDX;
      }
      for (idx = 0;idx < ue->n3PucchResCb.antP1N3ResCount; idx++ )
      {
         ue->n3PucchResCb.antP1N3Res[idx].n3PucchIdx 
                        = sCellPucchRecfg->u.format3.sCellAckN3ResAntP1[idx];
         ue->n3PucchResCb.antP1N3Res[idx].n3Lnk.node = NULLP;
         ue->n3PucchResCb.antP1N3Res[idx].sCellIdx = RGSCH_INVALID_CELL_IDX;
      }
      ue->simulAckNackCQIFormat3 = ueRecfg->simulAckNackCQIFormat3;
   }
#endif
   else
   {
      DU_LOG("\nERROR  -->  SCH : Wrong PUCCH Format:%d configured for CA",ue->uciFrmtTyp);
   }

   return ROK;

}  /* rgSCHSCellCfgUePucchReCfg */
/**
 * @brief Validates the UE SCell Reconfiguration request from APP to SCH.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUeSCellRecfg
 *
 *     Processing Steps:
 *      - Validate Number of SCells
 *        - If validated successfully,
 *          - Process Number of SCells
 *        - Else 
 *          - Return RFAILED.
 *      - Validate SCellIdx value,
 *        - If validated successfully,
 *          - Process Number of RgrUeSecCellCfg
 *        - Else 
 *          - Return RFAILED.
 *
 *  @param[in]  RgrUeRecfg    *ueRecfg
 *  @param[out] RgSchCellCb   *cell
 *  @param[out] RgSchUeCb     *ue
 *  @param[out] RgSchErrInfo  *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgVldtRgrUeSCellRecfg
(
RgrUeRecfg     *ueRecfg,
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgSchErrInfo   *errInfo
)
{
   RgrUeSecCellCfg   *ueSCellDedCfg = NULLP;
   RgSchCellCb          *sCell = NULLP;
   Inst    inst = cell->instIdx;


   DU_LOG("\nDEBUG  -->  SCH : VALIDATE RGR UE SCELL RECONFIG: cellId %d "
            "oldUeId %d cell %p \n", ueRecfg->cellId, ueRecfg->oldCrnti);
   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_UE_SCELL_RECFG;

   if((ueRecfg->ueSCellCfgInfo.numSCells > RGR_MAX_SCELL_PER_UE) ||
         (ueRecfg->ueSCellCfgInfo.numSCells < 1))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid number of SCELL "
               " in SCELL Recfg\n");
      return RFAILED;
   }
   
   for(uint8_t idx = 0; idx < ueRecfg->ueSCellCfgInfo.numSCells; idx++)
   {
      ueSCellDedCfg = &ueRecfg->ueSCellCfgInfo.ueSCellDedCfg[idx];
      if(ROK != rgSchUtlVldtCellId(inst, ueSCellDedCfg->sCellId))
      {
         DU_LOG("\nERROR  -->  SCH : SCellId is out of range");
         return RFAILED;
      }
      /* Validate existence of sec cell */
      sCell = rgSchUtlGetCellCb(inst, ueSCellDedCfg->sCellId);
      if(NULLP == sCell )
      {
         DU_LOG("\nERROR  -->  SCH : SCell doesnt exists");
         return RFAILED;
      }

      /* validate the range of serv cell index */
      if((ueSCellDedCfg->sCellIdx < 1) ||
            (ueSCellDedCfg->sCellIdx > RGR_MAX_SCELL_PER_UE))
      {
         DU_LOG("\nERROR  -->  SCH : Invalid Serv Cell Idx %d\n",
                  ueSCellDedCfg->sCellIdx);
         return RFAILED;
      }

      /* Is this sec cell alredy confiured */
      if(NULLP != ue->cellInfo[ueSCellDedCfg->sCellIdx])
      {
         DU_LOG("\nERROR  -->  SCH : Secll with id %d already added\n",
                  ueSCellDedCfg->sCellIdx);
         return RFAILED;
      }

      /* Validate CQI config  params */
      if((rgSCHCfgVldtUeCqiModeCfg(sCell, &ueSCellDedCfg->ueSCellDlCqiCfg)) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : Invalid CQI Mode "
                  " configuration for Ue %d\n",ue->ueId);
         return RFAILED;
      }
#ifdef TFU_UPGRADE
      /*  1. Validate UE Aperiodic CQI related parameters */
      if( ROK != rgSCHCfgVldtRgrUeACqiCfg (sCell, ue->ueId,
               &ueSCellDedCfg->ueSCellDlCqiCfg.aprdCqiCfg, ueSCellDedCfg->txMode,
               errInfo ))
      {
         DU_LOG("\nERROR  -->  SCH : rgSCHCfgVldtCqiSrSrsUeCfg: Invalid Aperiodic CQI configuration\n");
         return RFAILED;
      }

#ifdef LTEMAC_HDFDD
      if( ROK != rgSCHCfgVldtRgrUePCqiCfg (sCell, ue->ueId,
               &ueSCellDedCfg->ueSCellDlCqiCfg.prdCqiCfg, ueRecfg->isHdFddEnbld, 
               ueSCellDedCfg->txMode, errInfo ))
#else
      if( ROK != rgSCHCfgVldtRgrUePCqiCfg (sCell, ue->ueId,
               &ueSCellDedCfg->ueSCellDlCqiCfg.prdCqiCfg,
               ueSCellDedCfg->txMode, 
               errInfo )) 
#endif
      {
         DU_LOG("\nERROR  -->  SCH : rgSCHCfgVldtCqiSrSrsUeCfg: Invalid Periodic CQI configuration\n");
         return RFAILED;
      }

      if((ueSCellDedCfg->txMode.txModeEnum < RGR_UE_TM_1) ||
            (ueSCellDedCfg->txMode.txModeEnum > RGR_UE_TM_9))
      {
         DU_LOG("\nERROR  -->  SCH : SCELL Invalid transmission mode for"
                  " UE %d\n", (uint8_t)ueSCellDedCfg->txMode.txModeEnum);
         return RFAILED;
      }
#endif
   }

   errInfo->errCause = RGSCHERR_NONE;
   DU_LOG("\nDEBUG  -->  SCH : RGR Ue SCell Reconfig validation done: "
            "cellId %d oldUeId %d\n", ueRecfg->cellId, ue->ueId);
   return ROK;
}  /* rgSCHCfgVldtRgrUeSCellRecfg */

/**
 * @brief Ue SCell configuration roll back due to failure during configuration
 *        of any scell
 *
 * @details
 *
 *     Function : rgSCHSCellCfgUeCfgRollBack
 *
 *     This functions roll backs the configuration of successfully added Scell
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[int] RgrUeRecfg   *ueRecfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
static S16 rgSCHSCellCfgUeCfgRollBack
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg
)
{
   Inst                inst = cell->instIdx;
   RgrUeSecCellCfg  *sCellInfoCfg = NULLP;
   RgSchUeCellInfo     *sCellInfo = NULLP;
   RgSchCmnCell        *cellSch   = NULLP;

   RGSCHDBGINFONEW(inst, (rgSchPBuf(inst), 
     "--------------------------------------------------------------------\n"
     "UE SCell config roll back at SCH: rnti (%u) cell(%u)\n"
     "--------------------------------------------------------------------\n",
     ue->ueId, cell->cellId));
  
  /* Free all Added scell in this transaction */
   for(uint8_t idx = 0; idx < ueRecfg->ueSCellCfgInfo.numSCells; idx++)
   {
      sCellInfoCfg = &ueRecfg->ueSCellCfgInfo.ueSCellDedCfg[idx];
      sCellInfo =  ue->cellInfo[(sCellInfoCfg->sCellIdx)];

      /* if sCellInfo is not NULLP that means this Scell is added hence
       * delte it*/
      if (NULLP != sCellInfo)
      {
         /* Clear Scheduler specific list for this UE from the 
          * corresponding CELL */
         cellSch = RG_SCH_CMN_GET_CELL(sCellInfo->cell);
         cellSch->apisDl->rgSCHDlUeReset(sCellInfo->cell, sCellInfo->ue);
         
         /* Delete harq Entity of Scell*/
         rgSCHDhmDelHqEnt(cell, &(sCellInfo->hqEnt));
         
         rgSCHUtlFreeSBuf(inst, (Data**)&(sCellInfo),
               sizeof(RgSchUeCellInfo));
         
         ue->cellInfo[(sCellInfoCfg->sCellIdx)] = NULLP;
         ue->numSCells--;
#ifdef LTE_ADV
         if (ue->numSCells == 0)
         {
            ue->allocCmnUlPdcch = TRUE;
            /* As there is no SCell left so DCI 0 size at UE specific search space 
             * will be recalculated as the CSI is reduced to 1 bit */
            rgSCHUtlUpdUeDciSize(cell, ue, FALSE);
         }
#endif
      }
   }
   return ROK;
}
#endif /* LTE_ADV */
/**
 * @brief Validates the UE reconfiguration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUeRecfg
 *
 *     Processing Steps:
 *      - Retrieve the UE control block.
 *      - If successful,
 *        - Validate the range of reconfigured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell and ue.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  Inst          inst
 *  @param[in]  RgrUeRecfg    *ueRecfg
 *  @param[out] RgSchCellCb   **cell
 *  @param[out] RgSchUeCb     **ue
 *  @param[out] RgSchErrInfo  *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgVldtRgrUeRecfg
(
Inst           inst,
RgrUeRecfg     *ueRecfg,
RgSchCellCb    **cell,
RgSchUeCb      **ue,
RgSchErrInfo   *errInfo
)
{

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_UE_RECFG;
   
   if (((*cell) == NULLP) ||
       ((*cell)->cellId != ueRecfg->cellId))
   {
      DU_LOG("\nERROR  -->  SCH : Cell does not exist for OLD CRNTI:%d",ueRecfg->oldCrnti);
      return RFAILED;
   }

   /* Fetch the Old Ue */
   if ((*ue = rgSCHDbmGetUeCb(*cell, ueRecfg->oldCrnti)) == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : OLD CRNTI:%d does not exist", 
            ueRecfg->oldCrnti);
      return RFAILED;
   }

#ifdef LTE_ADV
   if(RGR_UE_SCELL_ADD_RECFG & ueRecfg->ueRecfgTypes) 
   {
      S16 ret = rgSCHCfgVldtRgrUeSCellRecfg(ueRecfg,*cell, *ue, errInfo);
      if ( ret != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : Ue SCell Recfg Validation FAILED\n");
         return RFAILED;
      }
      errInfo->errCause = RGSCHERR_NONE;
      return ROK;
   }
#endif

   if (ueRecfg->oldCrnti != ueRecfg->newCrnti)
   {
      if (rgSCHDbmGetUeCb(*cell, ueRecfg->newCrnti) != NULLP)
      {
         DU_LOG("\nERROR  -->  SCH : NEW CRNTI:%d already exists", 
              ueRecfg->newCrnti);
         return RFAILED;
      }
   }

   if ((ueRecfg->ueRecfgTypes & RGR_UE_CSG_PARAM_RECFG) &&\
       ((*ue)->csgMmbrSta == ueRecfg->csgMmbrSta))
   {
      DU_LOG("\nERROR  -->  SCH : UE ID [%d] invalid CSG Membership reconfig :%d ", 
              ueRecfg->newCrnti, (uint8_t)ueRecfg->csgMmbrSta);
      return RFAILED;
   }
   /* Validate values */
   if ((ueRecfg->ueRecfgTypes & RGR_UE_TXMODE_RECFG) &&
         (ueRecfg->txMode.pres == TRUE) &&
         ((ueRecfg->txMode.txModeEnum < RGR_UE_TM_1) ||
          (ueRecfg->txMode.txModeEnum > RGR_UE_TM_7)))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid transmission mode %d"
            "for NEW CRNTI:%d", (uint8_t)ueRecfg->txMode.txModeEnum,ueRecfg->newCrnti);
      return RFAILED;
   }
#ifndef TFU_UPGRADE
   if ((ueRecfg->ueRecfgTypes & RGR_UE_PRD_DLCQI_RECFG) &&
         (((ueRecfg->prdDlCqiRecfg.k < 1) || (ueRecfg->prdDlCqiRecfg.k > 4)) ||
         ((ueRecfg->prdDlCqiRecfg.cqiPmiCfgIdx < 1) || 
         (ueRecfg->prdDlCqiRecfg.cqiPmiCfgIdx > 1024))))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid Periodic CQI INFO"
               "OLD CRNTI:%d NEW CRNTI:%d",(uint8_t)ueRecfg->oldCrnti,ueRecfg->newCrnti);
      return RFAILED;
   }
#endif
   if ((ueRecfg->ueRecfgTypes & RGR_UE_ULHARQ_RECFG) &&
         (ueRecfg->ueUlHqRecfg.maxUlHqTx < RGSCH_MIN_HQ_TX))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid Uplink HARQ config %d"
          "for NEW CRNTI:%d", ueRecfg->ueUlHqRecfg.maxUlHqTx,ueRecfg->newCrnti);
      return RFAILED;
   }
#ifndef TFU_UPGRADE
   if ((ueRecfg->ueRecfgTypes & RGR_UE_PRD_DLCQI_RECFG) &&
         (ueRecfg->prdDlCqiRecfg.prdModeEnum > RGR_PRD_CQI_MOD21))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid periodic mode config for"
          " DL CQI %d NEW CRNTI:%d", (uint8_t)ueRecfg->prdDlCqiRecfg.prdModeEnum,ueRecfg->newCrnti);
      return RFAILED;
   }
#else
   if ((ueRecfg->ueRecfgTypes & RGR_UE_PCQI_RECFG) &&
       (ueRecfg->cqiCfg.cqiSetup.prdModeEnum > RGR_PRD_CQI_MOD21))
   {
     DU_LOG("\nERROR  -->  SCH : Invalid periodic mode config for "
            "DL CQI %d for NEW CRNTI:%d",(uint8_t)ueRecfg->cqiCfg.cqiSetup.prdModeEnum,ueRecfg->newCrnti);
     return RFAILED;
   }
#endif /* TFU_UPGRADE */
   /* Validate UE Category */
   if (ueRecfg->ueCatEnum > CM_LTE_UE_CAT_8)
   {
      DU_LOG("\nERROR  -->  SCH : Invalid category %d for NEW CRNTI:%d",
               (uint8_t)ueRecfg->ueCatEnum,ueRecfg->newCrnti);
      return RFAILED;
   }

   /* Validate UE Access Stratum Release */
   if ((ueRecfg->ueRecfgTypes & RGR_UE_UE_ACCESS_STRATUM_REL_RECFG) && \
         (ueRecfg->accessStratumRls > RGR_REL_11))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid Access Stratum Release %u for UE\n",
               ueRecfg->accessStratumRls);
      return RFAILED;
   }
   DU_LOG("\nINFO  -->  SCH : Configured Access Stratum Release %u\n", \
            ueRecfg->accessStratumRls);

   if ((ueRecfg->ueRecfgTypes & RGR_UE_APRD_DLCQI_RECFG) && 
         ((ueRecfg->aprdDlCqiRecfg.pres == TRUE) &&
          ((ueRecfg->aprdDlCqiRecfg.aprdModeEnum > RGR_APRD_CQI_MOD31) ||
          (*cell)->bwCfg.dlTotalBw <= 7)))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid aperiodic mode config for"
          " DL CQI %d for NEW CRNTI:%d", (uint8_t)ueRecfg->aprdDlCqiRecfg.aprdModeEnum,ueRecfg->newCrnti);
      return RFAILED;
   }
   if ((ueRecfg->ueRecfgTypes & RGR_UE_ULPWR_RECFG) &&
         (rgSCHCfgVldtUePwrCfg(*cell, &ueRecfg->ueUlPwrRecfg) != ROK))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid PUSCH Group power"
           " Reconfiguration for NEW CRNTI:%d",ueRecfg->newCrnti);
      return RFAILED;
   }
 
   
   if ((ueRecfg->ueRecfgTypes & RGR_UE_ACKNACK_MEASGAP_RECFG) &&
      (rgSCHCfgVldtUeMeasGapAckNakRepRecfg(*cell, ueRecfg) != ROK))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid MeasGap/AckNackRep"
            " Reconfiguration for NEW CRNTI:%d",ueRecfg->newCrnti);
      return RFAILED;
   }
#ifdef LTEMAC_SPS
   if(rgSCHCfgVldtSpsReCfg(*cell, *ue, ueRecfg)!= ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Invalid SPS"
           " Reconfiguration for NEW CRNTI:%d",ueRecfg->newCrnti);
      return RFAILED;
   }
#endif
#ifdef TFU_UPGRADE
   /* Validated Periodic CQI/PMI, RI , SRS and SR related UeCfg */
   if ( ROK != rgSCHCfgVldtCqiSrSrsUeReCfg(*cell, *ue, ueRecfg,  errInfo))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid ACQI, PCQI/SR/SRS "
               "Re-configuration for NEW CRNTI:%d",ueRecfg->newCrnti);
      return RFAILED;
   }
#endif
   if ((ueRecfg->ueRecfgTypes & RGR_UE_DRX_RECFG) &&
      (rgSCHCfgVldtDrxUeCfg(*cell, &(ueRecfg->ueDrxRecfg)) != ROK))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid drxParams"
            " Reconfiguration for NEW CRNTI:%d",ueRecfg->newCrnti);
      return RFAILED;
   }

/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   /* Validate DL Power Control Config parameters */
   if(rgSCHCfgVldtCqiReptReCfg(*cell, ueRecfg)!= ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Invalid DL Power Control"
           " Reconfiguration for NEW CRNTI:%d",ueRecfg->newCrnti);
      return RFAILED;
   }
#endif /* End of RGR_CQI_REPT */
   errInfo->errCause = RGSCHERR_NONE;
   return ROK;
}  /* rgSCHCfgVldtRgrUeRecfg */


/**
 * @brief Validates the logical channel reconfiguration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrLchRecfg
 *
 *     Processing Steps:
 *      - Retrieve the uplink and downlink logical channel control block.
 *      - If successful,
 *        - Validate the range of reconfigured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell, UE and logical channel.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  RgrLchRecfg     *lcRecfg
 *  @param[out] RgSchCellCb     **cell
 *  @param[out] RgSchUeCb       **ue
 *  @param[out] RgSchUlLcCb     **ulLc
 *  @param[out] RgSchDlLcCb     **dlLc
 *  @param[out] RgSchErrInfo       *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgVldtRgrLchRecfg
(
Inst           inst,
RgrLchRecfg    *lcRecfg,
RgSchCellCb    **cell,
RgSchUeCb      **ue,
RgSchDlLcCb    **dlLc,
RgSchErrInfo   *errInfo
)
{
   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_LC_RECFG;

   if (((*cell) == NULLP) ||
       ((*cell)->cellId != lcRecfg->cellId))
   {
      DU_LOG("\nERROR  -->  SCH : Cell does not exist " 
                "for CRNTI:%d LCID:%d",lcRecfg->crnti,lcRecfg->lcId);
      return RFAILED;
   }

   /* Fetch the Ue for dedicated channels */
   if ((*ue = rgSCHDbmGetUeCb(*cell, lcRecfg->crnti)) == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : UEID does not exist"
         "dedicated logical channel for CRNTI:%d LCID:%d",lcRecfg->crnti,lcRecfg->lcId);
      return RFAILED;
   }
   
   if ((*dlLc = rgSCHDbmGetDlDedLcCb((*ue), lcRecfg->lcId)) == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : Dedicated DL LC does not "
               "exist for CRNTI:%d LCID:%d",lcRecfg->crnti,lcRecfg->lcId);
      return RFAILED;
   }

   errInfo->errCause = RGSCHERR_NONE;
   return ROK;
}  /* rgSCHCfgVldtRgrLchRecfg */

/**
 * @brief Validates the UE Reset request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUeReset
 *
 *     Processing Steps:
 *      - Retrieve the CELL control block
 *      - If cell does not exist return RFAILED
 *      - Retrieve UE Control block
 *      - If UE does not exist return RFAILED
 *      - Return ROK
 *
 *  @param[in]  Inst        inst
 *  @param[in]  RgrRst      *reset
 *  @param[out] RgSchCellCb **cell
 *  @param[out] RgSchUeCb      **ue
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgVldtRgrUeReset
(
Inst         inst,
RgrRst       *reset,
RgSchCellCb  *cell,
RgSchUeCb    **ue,
RgSchErrInfo *errInfo
)
{

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_UE_RESET;
   
   if ((cell == NULLP) || (cell->cellId != reset->cellId))
   {
      DU_LOG("\nERROR  -->  SCH : CELL does not exist for CRNTI:%d",
               reset->crnti);
      return RFAILED;
   }
   /* Fetch the Ue  */
   if ((*ue = rgSCHDbmGetUeCb(&(*cell), reset->crnti)) == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : UE does not exist for CRNTI:%d",
               reset->crnti);
      return RFAILED;
   }

   errInfo->errCause = RGSCHERR_NONE;

   return ROK;
}  /* rgSCHCfgVldtRgrUeReset */
   

/**
 * @brief Validates the logical channel reconfiguration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrLcgRecfg
 *
 *     Processing Steps:
 *      - Retrieve the uplink and downlink logical channel control block.
 *      - If successful,
 *        - Validate the range of reconfigured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell, UE and logical channel.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  RgrLchRecfg     *lcRecfg
 *  @param[out] RgSchCellCb     **cell
 *  @param[out] RgSchUeCb       **ue
 *  @param[out] RgSchErrInfo    *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgVldtRgrLcgRecfg
(
Inst           inst,
RgrLcgRecfg    *lcgRecfg,
RgSchCellCb    *cell,
RgSchUeCb      **ue,
RgSchErrInfo   *errInfo
)
{
   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_LCG_RECFG;

   if (((cell) == NULLP) ||
       ((cell)->cellId != lcgRecfg->cellId))
   {
      DU_LOG("\nERROR  -->  SCH : Cell does not exist for" 
                "CRNTI:%d LCGID:%d",lcgRecfg->crnti,lcgRecfg->ulRecfg.lcgId);
      return RFAILED;
   }

   /* Fetch the Ue for dedicated channels */
   if ((*ue = rgSCHDbmGetUeCb(&(*cell), lcgRecfg->crnti)) == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : UE does not exist for "
                "dedicated logical channel group CRNTI:%d LCGID:%d",
                lcgRecfg->crnti,lcgRecfg->ulRecfg.lcgId);
      return RFAILED;
   }
   if (lcgRecfg->ulRecfg.lcgId > (RGSCH_MAX_LCG_PER_UE - 1))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid lcgId for uplink logical"
                "channel CRNTI:%d LCGID:%d",
                lcgRecfg->crnti,lcgRecfg->ulRecfg.lcgId);
      return RFAILED;
   }
   
   if ((lcgRecfg->ulRecfg.gbr != 0) && (lcgRecfg->ulRecfg.mbr < lcgRecfg->ulRecfg.gbr))
   {
      DU_LOG("\nERROR  -->  SCH : Dedicated Logical Group %d validation failed"\
	  " for ue %d for cell %d\n", lcgRecfg->ulRecfg.lcgId,(*ue)->ueId, cell->cellId);
      return RFAILED;
   }
   errInfo->errCause = RGSCHERR_NONE;
   return ROK;
}  /* rgSCHCfgVldtRgrLcgRecfg */

/**
 *
 * @details
 *
 *     Function : rgSCHDynCfiCfg
 *
 *  @param[in]  RgSchCellCb   *cell
 *              RgrCellCfg    *cellCfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
static S16 rgSCHDynCfiCfg
(
RgSchCellCb   *cell,
RgrCellCfg    *cellCfg
)
{
   uint8_t        cfi; 
#ifdef LTE_TDD
   uint8_t        ulDlCfgIdx = cellCfg->ulDlCfgIdx;
   uint8_t        mphIdx;
   uint8_t        maxMPhich;
   uint16_t       numDlSf;
#endif    

   cell->dynCfiCb.isDynCfiEnb = cellCfg->isDynCfiEnb;

   /* Initializing Failure Sample Period */
   cell->dynCfiCb.failSamplePrd = (RGSCH_CFI_TTI_MON_INTRVL *
                     RGSCH_CFI_STEP_UP_TTI_PRCNTG)/100;
   /* Initializing Number of Failure Samples */
   cell->dynCfiCb.numFailSamples = (RGSCH_CFI_TTI_MON_INTRVL/
                                    cell->dynCfiCb.failSamplePrd);
   cell->dynCfiCb.maxCfi = RGSCH_MAX_CFI_VAL;
   /* Allocating memory for CCE failure average array based on 
    * monitoring interval and CCE failure sample period */
   if((rgSCHUtlAllocSBuf(cell->instIdx, (Data**)&(cell->dynCfiCb.cceFailSamples),
               (cell->dynCfiCb.numFailSamples * sizeof(uint16_t)))) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation FAILED for cell");
      return RFAILED;
   }

   /* Setting the Invalid value 0xFF to pdcchSfIdx, it will be assigned 
    * a valid value during CFI swithing is done */
   cell->dynCfiCb.pdcchSfIdx = 0xFF;

#ifdef LTE_TDD
   /* In case of config index 0, the mphich index can be upto 2 
    * in other config index cases, it will always be set as 1*/
   if(ulDlCfgIdx == 0)
   {
      maxMPhich = RG_SCH_MAX_MPHICH;
   }
   else
   {
      maxMPhich = RG_SCH_MAX_MPHICH -1;
   }    
   /* Calculate the number of CCEs in the cell */
   for(cfi = 1; cfi < RG_SCH_CMN_MAX_CFI; cfi++)
   {   
      for(mphIdx = 0; mphIdx < maxMPhich; mphIdx++)
      {   
         cell->dynCfiCb.cfi2NCceTbl[mphIdx][cfi] = 
            rgSCHUtlCalcNCce(cell->bwCfg.dlTotalBw,
               cell->phichCfg.ngEnum, cfi, mphIdx,
               cell->numTxAntPorts,
               cell->isCpDlExtend);
      }
   }
#else   
   /* Calculate the number of CCEs in the cell */
   for(cfi = 1; cfi < RG_SCH_CMN_MAX_CFI; cfi++)
   {   
      /* CFI Index starts from 1 so that there can be a direct mapping from 
         actual CFI value to cfi Index. mPhich index will always be set 
         as 0 for FDD */
      cell->dynCfiCb.cfi2NCceTbl[0][cfi] = 
            rgSCHUtlCalcNCce(cell->bwCfg.dlTotalBw, cell->phichCfg.ngEnum,
                  cfi, cell->numTxAntPorts, cell->isCpDlExtend);
   }

   /* Calculate the number of CCEs in the cell */
   if(cell->dynCfiCb.isDynCfiEnb == TRUE)
   {   
      /* In case if Dynamic CFI feature is enabled, default CFI 
       * value 1 is used  */
      cell->nCce = cell->dynCfiCb.cfi2NCceTbl[0][1];
   }   
   else
   {
      cell->nCce = cell->dynCfiCb.cfi2NCceTbl[0][cellCfg->cfiCfg.cfi];
   }    
#endif  

#ifdef LTE_TDD   
   numDlSf = rgSchTddNumDlSubfrmTbl[ulDlCfgIdx][9] * 
               (RGSCH_CFI_TTI_MON_INTRVL/10);
   cell->dynCfiCb.cfiStepUpTtiCnt = 
               (RGSCH_CFI_STEP_UP_TTI_PRCNTG * numDlSf)/100;
   cell->dynCfiCb.cfiStepDownTtiCnt = 
               (RGSCH_CFI_STEP_DOWN_TTI_PERCNTG * numDlSf)/100;
#else
   cell->dynCfiCb.cfiStepUpTtiCnt = (RGSCH_CFI_STEP_UP_TTI_PRCNTG * 
                                     RGSCH_CFI_TTI_MON_INTRVL)/100;
   cell->dynCfiCb.cfiStepDownTtiCnt = (RGSCH_CFI_STEP_DOWN_TTI_PERCNTG * 
                                     RGSCH_CFI_TTI_MON_INTRVL)/100;
#endif   

   return ROK;
}

/**
 * @brief Handler for the SCHED Enb configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrSchedEnbCfg
 *
 *     Processing Steps:
 *      - Invoke SCH with SCHEDULER control block to update
 *        scheduler specific information.
 *      - Update rgSch control block with the values recieved in the
 *        configuration.
 *      - If successful, return ROK else RFAILED.
 *      
 *  @param[in]  RgSchCb      *instCb
 *  @param[in]  SpId         spId
 *  @param[in]  RgSchSchedEnbCfg    *schedEnbCfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgRgrSchedEnbCfg
(
Inst            inst,
SpId            spId,
RgrSchedEnbCfg *schedEnbCfg,
RgSchErrInfo   *errInfo
)
{

   DU_LOG("\nDEBUG  -->  SCH : APPLYING RGR SCH ENB CONFIG: \n");
   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_ENB_CFG;

   rgSchCb[inst].rgrSchedEnbCfg = *schedEnbCfg;
   DU_LOG("\nDEBUG  -->  SCH : dlSchdType %d ulSchdType %d dlTptCoeffi %d"
           "dlFairCoeffi %d  ulTptCoeffi %d ulFairCoeffi %d\n",
         schedEnbCfg->dlSchdType, schedEnbCfg->ulSchdType, schedEnbCfg->dlSchInfo.dlPfs.tptCoeffi,
         schedEnbCfg->dlSchInfo.dlPfs.fairCoeffi, schedEnbCfg->ulSchInfo.ulPfs.tptCoeffi,
         schedEnbCfg->ulSchInfo.ulPfs.fairCoeffi);

#ifdef RG_5GTF
   rgSchCb[inst].rgSchDynTdd.isDynTddEnbld = schedEnbCfg->isDynTddEnbld;
#endif
# if 0
   if(RGR_SCH_TYPE_PFS == schedEnbCfg->dlSchdType)
   {
     rgSCHEnbPfsDlCfg(inst, errInfo);
   }
#endif
   errInfo->errCause = RGSCHERR_NONE;
   DU_LOG("\nINFO  -->  SCH : RGR SCH gNB done: \n");
   return ROK;
}  /* rgSCHCfgRgrSchedEnbCfg */

#ifdef RG_5GTF
/**
 * @brief Handler for the cell configuration of 5gtf.
 *
 * @details
 *
 *     Function : rgSCH5gtfCellCfg
 *      
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgSchCellCfg    *cellCfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCH5gtfCellCfg
(
RgSchCellCb   *cell,
RgrCellCfg    *cellCfg
)
{
   uint8_t           idx;

   for(idx = 0; idx < MAX_5GTF_GROUP; idx++)
   {
      cell->cell5gtfCb.ueGrp5gConf[idx].beamBitMask = 0;
   }

   for(idx = 0 ; idx < MAX_5GTF_SUBFRAME_INFO ; ++idx)
   {
      cell->cell5gtfCb.dynConfig[idx] = cellCfg->Cell5gtfCfg.dynConfig[idx]; 
   }
   cell->cell5gtfCb.numUes = cellCfg->Cell5gtfCfg.numUes;
   cell->cell5gtfCb.uePerGrpPerTti = cellCfg->Cell5gtfCfg.uePerGrp;
   cell->cell5gtfCb.ueGrpPerTti = cellCfg->Cell5gtfCfg.ueGrpPerTti;
   cell->cell5gtfCb.numCCs = cellCfg->Cell5gtfCfg.numOfCC;
   cell->cell5gtfCb.bwPerCC = cellCfg->Cell5gtfCfg.bwPerCC;
   DU_LOG("\nINFO  -->  SCH : cell cfg at schd,numUes:%u,uepergrp:%u,uegrppertti:%u,numCC:%u,bwPerc:%u cfi %u\n",
         cell->cell5gtfCb.numUes,cell->cell5gtfCb.uePerGrpPerTti,cell->cell5gtfCb.ueGrpPerTti,
         cell->cell5gtfCb.numCCs,cell->cell5gtfCb.bwPerCC, cell->cell5gtfCb.cfi);
   return ROK;
}
#endif

#ifdef XEON_LMT_ITBS
uint16_t gWrMaxDlItbs;
uint16_t gWrMaxUlItbs;
#endif
/**
 * @brief Handler for the cell configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrCellCfg
 *
 *     Processing Steps:
 *      - Invoke SCH with cell control block to update
 *        scheduler specific information.
 *      - Update cell control block with the values recieved in the
 *        configuration.
 *      - Add to the active list of cells if cell becomes ACTIVE.
 *      - If successful, return ROK else RFAILED.
 *      
 *  @param[in]  RgSchCb      *instCb
 *  @param[in]  SpId         spId
 *  @param[in]  RgSchCellCfg    *cellCfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgRgrCellCfg
(
RgSchCb       *instCb,
SpId          spId,
RgrCellCfg    *cellCfg,
RgSchErrInfo  *errInfo
)
{
   S16          ret;
   uint8_t           idx;
   Pst          pst;
   RgInfCellReg cellRegReq;
   RgSchCellCb  *cell = NULLP;
   Inst         inst = instCb->rgSchInit.inst;
   uint32_t          Idx1 = (uint8_t)((cellCfg->cellId - instCb->genCfg.startCellId)&(CM_LTE_MAX_CELLS-1));

   errInfo->errCause = RGSCHERR_CFG_RGR_CELL_CFG;
   
   memset(&pst, 0, sizeof(Pst));
    
   /* Allocate the scheduler's cell control block */
   if((ret = rgSCHUtlAllocSBuf(inst, (Data**)&cell, sizeof(RgSchCellCb))) 
         != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation FAILED for cell");
      return RFAILED;
   }
#ifdef EMTC_ENABLE
   if(cellCfg->emtcEnable)
   {
      if((ret = rgSCHEmtcCellAlloc(cell)) 
            != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : Memory allocation FAILED for emtc cell");
         return RFAILED;
      }
   }
#endif
   if ((uint8_t *)cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation FAILED for cell");
      return RFAILED;
   }
   /* Initialize the lists of the cell */
   ret = rgSCHDbmInitCell(cell);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : DBM initialization FAILED for cell");
      rgSCHCfgFreeCellCb(cell);
      return RFAILED;
   }
/* LTE_ADV_FLAG_REMOVED_START */
   if(cellCfg->rgrLteAdvCfg.pres & RGR_ABS)
   {
      cell->lteAdvCb.absCfg =
         cellCfg->rgrLteAdvCfg.absCfg;
      memset(cell->lteAdvCb.absLoadInfo, 0, sizeof(uint32_t)*RGR_ABS_PATTERN_LEN);
      cell->lteAdvCb.absLoadTtiCnt = 0;
   }

   if(cellCfg->rgrLteAdvCfg.pres & RGR_SFR)
   {
      cell->lteAdvCb.sfrCfg =
         cellCfg->rgrLteAdvCfg.sfrCfg;
   }
   if(cellCfg->rgrLteAdvCfg.pres & RGR_DSFR)
   {
      cell->lteAdvCb.dsfrCfg =
         cellCfg->rgrLteAdvCfg.dsfrCfg;
   }
/* LTE_ADV_FLAG_REMOVED_END */

#ifdef EMTC_ENABLE
   cell->emtcEnable = cellCfg->emtcEnable;	
#endif
   /* Initialize the cell */
   cell->cellId = cellCfg->cellId;
   cell->instIdx = inst;
   cell->macInst = cellCfg->macInst;
   cell->isCpUlExtend = cellCfg->isCpUlExtend;
   cell->isCpDlExtend = cellCfg->isCpDlExtend;

   cell->numTxAntPorts = rgSchCb[inst].rgrSchedEnbCfg.numTxAntPorts; 
   if(cell->numTxAntPorts == 1)
   {
      cell->numCellRSPerSf = RGSCH_NUM_CELL_RS_ONE_ANT_PORT;
   }
   else if(cell->numTxAntPorts == 2)
   {
      cell->numCellRSPerSf = RGSCH_NUM_CELL_RS_TWO_ANT_PORT;
   }
   else
   {
      cell->numCellRSPerSf = RGSCH_NUM_CELL_RS_FOUR_ANT_PORT;
   }
   cell->bwCfg = cellCfg->bwCfg;
   cell->pbchRbStart = ((((cell->bwCfg.dlTotalBw * 12)/2) - 36)/12); /* Ref section 6.6 in 36.211 */ 
   cell->pbchRbEnd = cell->pbchRbStart + 5;
   cell->pucchCfg = cellCfg->pucchCfg;
   cell->rachCfg = cellCfg->rachCfg;
   cell->siCfg = cellCfg->siCfg;
   cell->t300TmrVal = cellCfg->t300TmrVal;
#ifdef RGR_SI_SCH
   /*Initialize the SI CB in Cell CB */
   memset(&cell->siCb, 0, sizeof(RgSchSiCb));
#endif
   /*Fix: Added Guard Pool for RNTI which will contain RNTIs 
       *for UEs deleted from Scheduler but not yet from MAC*/
   cmLListInit(&cell->rntiDb.rntiGuardPool);

   /* Initialize the inWindow to sync with scheduler time when ticks starts */
#ifdef LTEMAC_HDFDD
      cell->siCb.inWindow = (cellCfg->siCfg.siWinSize - 
            (RG_SCH_CMN_DL_DELTA + RG_SCH_CMN_HARQ_INTERVAL));
#else
      cell->siCb.inWindow = (cellCfg->siCfg.siWinSize - 
            (RG_SCH_CMN_DL_DELTA));
#endif
     
      if(cell->siCb.inWindow < 0)
      { 
         cell->siCb.inWindow = 0;
      }
   cell->macPreambleSet = cellCfg->macPreambleSet;
   cell->phichCfg = cellCfg->phichCfg;

   /* Initialize UL and DL CCCH logical channels */
   cell->ulCcchId = RGSCH_INVALID_LC_ID;
   cell->dlCcchId = RGSCH_INVALID_LC_ID;

   /* Update SRS configuration */
   cell->srsCfg.isSrsCfgPres = cellCfg->srsCfg.isSrsCfgSetup;
   if(cellCfg->srsCfg.isSrsCfgSetup)
   {
      cell->srsCfg.srsCfgPrdEnum = cellCfg->srsCfg.srsCfgPrdEnum;
      cell->srsCfg.srsBwEnum = cellCfg->srsCfg.srsBwEnum;
      cell->srsCfg.srsTxOffst =
         rgSrsTxOffstTbl[cellCfg->srsCfg.srsSubFrameCfg];
      /*ccpu00116923 - ADD - Srs Present support */
#ifdef TFU_UPGRADE
      cell->srsCfg.srsSubFrameCfg = cellCfg->srsCfg.srsSubFrameCfg;
#endif
   }

   /* Configure all the common logical channels for the cell */
   for(idx = 0; idx < cellCfg->numCmnLcs; idx++)
   {
      /* This never returns failure and hence not checked for */
      rgSCHCfgRgrCmnLcCfg(cell, &(cellCfg->cmnLcCfg[idx]), errInfo);
   }

   /* Invoke the MeasGap and ACK NACK Rep handler for cell cfg */

   /* Dynamic CFI cell configuration */
   ret = rgSCHDynCfiCfg(cell, cellCfg);
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Rgr cell Config failed at "
       "Scheduler for cell");
      rgSCHCfgFreeCellCb(cell);
      return RFAILED;
   }   
   /* Updating Auto TM Mode enable/diable flag */
   cell->isAutoCfgModeEnb = cellCfg->isAutoCfgModeEnb;
   {
     if(cell->isAutoCfgModeEnb)
     {
        DU_LOG("\nINFO  -->  SCH : Auto Mode Cfg enabled durint cell cfg\n");
     }
   }
   /* CPU OvrLoad State Initialization */
#ifdef XEON_LMT_ITBS
   cell->thresholds.maxDlItbs = gWrMaxDlItbs;
   cell->thresholds.maxUlItbs = gWrMaxUlItbs;
   DU_LOG("\nINFO  -->  SCH : LIMIT DL and UL ITBS %d:%d \n",gWrMaxDlItbs,gWrMaxUlItbs);
#else
   cell->thresholds.maxDlItbs = RG_SCH_DL_MAX_ITBS;
   cell->thresholds.maxUlItbs = RG_SCH_UL_MAX_ITBS;
#endif
   cell->measurements.dlTpt = 0;
   cell->measurements.ulTpt = 0;
   cell->measurements.dlBytesCnt = 0;
   cell->measurements.ulBytesCnt = 0;
   cell->cpuOvrLdCntrl.cpuOvrLdIns = 0; /* 0 - No command */
   cell->cpuOvrLdCntrl.dlNxtIndxDecNumUeTti = 0;
   cell->cpuOvrLdCntrl.ulNxtIndxDecNumUeTti = 0;
   for ( idx = 0; idx < 10; idx++ )
   {
      cell->cpuOvrLdCntrl.maxUeNewTxPerTti[idx] = cellCfg->maxDlUeNewTxPerTti;
      cell->cpuOvrLdCntrl.maxUeNewRxPerTti[idx] = cellCfg->maxUlUeNewTxPerTti;
   }

   /* Invoke scheduler to update scheduler specific information */
   ret =  rgSCHUtlRgrCellCfg(cell, cellCfg, errInfo);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Rgr cell Config failed at "
       "Scheduler for cell ");
      rgSCHCfgFreeCellCb(cell);
      return RFAILED;
   }

   /* Invoke DHM to update DHM specific information */
   rgSCHDhmRgrCellCfg(cell, cellCfg, errInfo);
  

   /* Initialize RNTI DB */
   ret = rgSCHDbmRntiDbInit(cell, cellCfg->macRnti.startRnti,
         cellCfg->macRnti.size);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Rgr Cell Config failed at"
          " RNTI DB init for cell");
      rgSCHCfgFreeCellCb(cell);
      return RFAILED;
   }

   /* Update the cell with recieved configuration */
   cell->dlHqCfg  =  cellCfg->dlHqCfg;

   DU_LOG("\nINFO  -->  SCH : Config DL HQTX = %d\n",cell->dlHqCfg.maxDlHqTx);

   cell->crntSfIdx = 0; 
   /* Allocate the subframe allocation information */
   if((ret = rgSCHUtlGetSfAlloc(cell)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation FAILED for "
               "cell");
      rgSCHCfgFreeCellCb(cell);
      return RFAILED;
   }
   /* Update RACH Related information 
    * XXX: Below function yet to be written in RAM
    * To store the preambles given in  the configuration for PDCCH order in the
    * scheduler cell control block. Initialize the PRACH Mask Index allocated
    * for these preambles to invalid values */

   cell->crntHqIdx = 0; 
   /* Allocate the subframe allocation information */
   if((ret = rgSCHUtlGetRlsHqAlloc(cell)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation FAILED for"
               "cell");
      rgSCHCfgFreeCellCb(cell);
      return RFAILED;
   }

   /* Associate a pair of upper and lower sapCbs with this cell */
   instCb->rgrSap[spId].cell = cell;
   instCb->tfuSap[spId].cell = cell;
   instCb->rgmSap[spId].cell = cell;
   cell->tfuSap = &(instCb->tfuSap[spId]);

   /* CaDev Start */
      instCb->cells[Idx1] = cell;
   /* CaDev End */

   /* rg001.201: Added for sending TTI tick to RRM */
#if (defined(RGR_RRM_TICK) || defined(RGR_CQI_REPT))
   /* Associate the RGR SAP as well utilized while sending TTI
    * Ticks to RGR User. */
   cell->rgrSap = &(instCb->rgrSap[spId]);
#endif
   cell->rgmSap = &(instCb->rgmSap[spId]);
#ifdef RGR_RRM_TICK
   /* Store the periodicity configured */
   cell->rrmTtiIndPrd = cellCfg->rrmTtiIndPrd;
#endif

#ifdef LTE_L2_MEAS
   cmLListInit(&cell->l2mList);
#endif

   if (rgSCHDrxCellCfg(cell,cellCfg) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Drx Memory allocation FAILED for"
         " cell");
      rgSCHCfgFreeCellCb(cell);
      return RFAILED;
   }
   cell->overLoadBackOffEnab = FALSE;/* Disabling RachOverload by default */
   /* Updating CSG Parameters */
   cell->minDlResNonCsg = cellCfg->csgParamCfg.minDlResNonCsg;
   cell->minUlResNonCsg = cellCfg->csgParamCfg.minUlResNonCsg;

   /* Register the cell with MAC */ 
   rgSCHUtlGetPstToLyr(&pst, instCb, cell->macInst);
   cellRegReq.cellId = cell->cellId;
   cellRegReq.cellSapId = spId;
#ifdef LTE_TDD
   cellRegReq.maxDlHqProcPerUe = rgSchTddDlNumHarqProcTbl[cellCfg->ulDlCfgIdx];
#else
   cellRegReq.maxDlHqProcPerUe = RGSCH_MAX_DL_HQ_PROC;
#endif
   RgSchMacCellReg(&pst, &cellRegReq);

#ifdef TENB_STATS
   cell->tenbStats = TSL2AllocCellStatsBlk(cell->cellId);
   cell->tenbStats->cellId = cell->cellId;
#endif

   rgSCHUtlCalcDciSizes(cell);

#ifdef LTE_ADV
   /* Initilalization of the list of UE for which this cell is secondary cell*/
   cmLListInit(&cell->sCellUeLst);
#endif 


#ifdef LTE_ADV
   ret = rgSCHLaaSCellCbInit(cell, cellCfg);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Rgr Cell Config failed at"
          " Initializing the LAA Cell Control Cb");
      rgSCHCfgFreeCellCb(cell);
      return RFAILED;
   }
   cell->isPucchFormat3Sptd = cellCfg->isPucchFormat3Sptd;
   DU_LOG("\nINFO  -->  SCH : Format 3 is Enabled");
   DU_LOG ("\n Format 3 is Enabled for CELL:%d",cell->cellId);
#endif


#ifdef EMTC_ENABLE

	if(cell->emtcEnable)
	{	
   	  if (rgSCHCfgEmtcCellCfg(cell,&(cellCfg->emtcCellCfg)) != ROK)
   	  {
      		 DU_LOG("\nERROR  -->  SCH : EMTC Config Failed cell");
      		 return RFAILED;
   	  }
	}
#endif

#ifdef RG_5GTF
   ret = rgSCH5gtfCellCfg(cell, cellCfg);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : 5GTF Rgr Cell Config failed");
      rgSCHCfgFreeCellCb(cell);
      return RFAILED;
   }
#endif
   errInfo->errCause = RGSCHERR_NONE;
   return ROK;
}  /* rgSCHCfgRgrCellCfg */

/**
 * @brief Handler for the UE configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrUeCfg
 *
 *     Processing Steps:
 *      - Allocate and create UE control block.
 *      - Update UE control block with the values recieved in the
 *        configuration.
 *      - Invoke RAM, SCH, UHM and DHM with created UE control block, to
 *        update random access, scheduler, uplink harq and downlink harq
 *        specific information respectively.
 *      - If successful, add the control block to hash list of UEs for the cell
 *        else Rollback and FAIL.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgrUeCfg     *ueCfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgRgrUeCfg
(
RgSchCellCb  *cell,
RgrUeCfg     *ueCfg,
RgSchErrInfo *errInfo
)
{
   S16          ret;
   RgSchRaCb    *raCb=NULLP;
   RgSchUeCb    *ue = NULLP;
   Inst         inst = cell->instIdx;
   uint32_t          lcgCnt;
   RgSchDlHqEnt *hqEnt = NULLP;
#ifdef LTE_TDD
   uint8_t           ulDlCfgIdx = cell->ulDlCfgIdx;
   uint8_t           maxSubframes ;
   uint8_t           maxDlSubframes;
#endif
   uint32_t          idx = 0;  
#ifdef TFU_UPGRADE
   RgSchUePCqiCb *cqiCb = NULLP;
#endif

  do {
     errInfo->errCause = RGSCHERR_CFG_RGR_UE_CFG;
     /* RACHO : Check for raCb only if preamble Id not provded */
#ifndef PRE_DEF_UE_CTX
     if (ueCfg->dedPreambleId.pres == NOTPRSNT)
     {
        if ((raCb = rgSCHDbmGetRaCb(cell, ueCfg->crnti)) == NULLP)
        {
           DU_LOG("\nERROR  -->  SCH : No RaCb exists for"
                    "CRNTI:%d ",ueCfg->crnti);
           break;
        }
     }
#endif

     /* Allocate the Ue control block */
     if (((rgSCHUtlAllocSBuf(inst, (Data **)&ue, sizeof(RgSchUeCb))) != ROK) ||
            ((uint8_t *)ue == NULLP))
     {
        DU_LOG("\nERROR  -->  SCH : Memory allocation"
           " FAILED for CRNTI:%d", ueCfg->crnti);
        break;
     }

     /* Inititialize Ue control block */
     ue->ueId = ueCfg->crnti;
     ue->cell = cell;
     /*ccpu00117778- Initialize Transmission Indices upon UE CB creation */
#ifdef LA
   ue->lastRprdAckNackTime.sfn = cell->crntTime.sfn;
   ue->lastRprdAckNackTime.subframe = cell->crntTime.slot;
   ue->ueIdle = FALSE; 
#endif

   /* Allocate the Ue control block */
   if (((rgSCHUtlAllocSBuf(inst, (Data **)&(ue->cellInfo[RGSCH_PCELL_INDEX]),
                  sizeof(RgSchUeCellInfo))) != ROK))
   {
#ifndef ALIGN_64BIT
      DU_LOG("\nERROR  -->  SCH : [%lu]SCellIdx :Memomy allocation "
               "Failed while Adding SCell Information\n", idx);
#else
      DU_LOG("\nERROR  -->  SCH : [%u]SCellIdx :Memomy allocation "
               "Failed while Adding SCell Information\n", idx);
#endif
      return RFAILED;
   }

   ue->cellIdToCellIdxMap[RG_SCH_CELLINDEX(cell)] = RGSCH_PCELL_INDEX;
   ue->cellInfo[RGSCH_PCELL_INDEX]->cell = cell;
   ue->cellInfo[RGSCH_PCELL_INDEX]->ue = ue;
#ifdef LTE_ADV
   ue->cellInfo[RGSCH_PCELL_INDEX]->sCellState = RG_SCH_SCELL_ACTIVE;
   ue->cellInfo[RGSCH_PCELL_INDEX]->sCellIdx = RGSCH_PCELL_INDEX;
   ue->cellInfo[RGSCH_PCELL_INDEX]->sCellId = cell->cellId;

   if (ROK != rgSCHLaaInitDlRbAllocCb(cell, 
            &ue->cellInfo[RGSCH_PCELL_INDEX]->dlAllocCb))
   {
      return RFAILED;
   }
#endif
#ifdef TFU_UPGRADE
     cqiCb = RG_SCH_GET_UE_CELL_CQI_CB(ue,cell);
     cqiCb->nCqiTrIdx = RG_SCH_INVALID_IDX;
     cqiCb->nRiTrIdx = RG_SCH_INVALID_IDX;
     ue->srsCb.nSrsTrIdx = RG_SCH_INVALID_IDX;
     ue->srCb.nSrTrIdx = RG_SCH_INVALID_IDX;
#endif
   /* LTE_ADV_FLAG_REMOVED_START */
   /* While doing UE configuration for SFR at SCH, by default 
    * CC UE power is configured as LOW */
   ue->lteAdvUeCb.isCCUePHigh = FALSE;
   /* LTE_ADV_FLAG_REMOVED_END */

     /* Initialize the lists of the UE */
     if((rgSCHDbmInitUe(ue)) != ROK)
     {
        DU_LOG("\nERROR  -->  SCH : DBM initialization "
           "failed for CRNTI:%d", ueCfg->crnti);
        break;
     }
#ifdef EMTC_ENABLE
     if(raCb != NULLP)
     {
        if(TRUE == raCb->isEmtcRaCb)
        { 
           ue->isEmtcUe = TRUE;
           if (rgSCHUtlUpdUeEmtcInfo(cell, ueCfg, ue) != ROK)
           {
              DU_LOG("\nERROR  -->  SCH : EMTC UE Cfg"
                    "failed for CRNTI:%d", ueCfg->crnti);
              break;
           }
        }
     } 
#endif

     /* Initialize scheduler related information for UE */
     if(rgSCHUtlRgrUeCfg(cell, ue, ueCfg, errInfo) != ROK)
     {
        DU_LOG("\nERROR  -->  SCH : Scheduler handling "
                 "failed in config for CRNTI:%d", ueCfg->crnti);
        break;
     }

     ret = rgSCHUhmHqEntInit(cell, ue);
     if (ret != ROK)
     {
        DU_LOG("\nERROR  -->  SCH : UHM HARQ Ent Init "
           "Failed for CRNTI:%d", ueCfg->crnti);
        break;
     }

     /* Initialize RAM related information for UE 
      * RACHO: if preamble Id is present in ueCfg then raCb will be NULL
      * so rgSCHRamRgrUeCfg should take care of creating raCb */
   if ((ueCfg->dedPreambleId.pres == NOTPRSNT) && (NULLP != raCb) )
     {
        if((rgSCHRamRgrUeCfg(cell, ue, raCb, errInfo)) != ROK)
        {
           DU_LOG("\nERROR  -->  SCH : Random access "
              "handling config failed for CRNTI:%d", ueCfg->crnti);
           break;
        }
     }
     else /* if HO Ue */
     {
        RG_SCH_CMN_GET_UE_HQE(ue, cell) = rgSCHDhmHqEntInit(cell);
        hqEnt = RG_SCH_CMN_GET_UE_HQE(ue, cell);
        if (hqEnt == NULLP)
        {
           DU_LOG("\nERROR  -->  SCH : Hq Entity Initialization "
                    "failed in config for CRNTI:%d", ueCfg->crnti);
           break;
        }
#ifdef EMTC_ENABLE
        rgSCHEmtcHqPAlloc(cell, hqEnt);
#endif
        hqEnt->ue = ue;
        /* Fix : syed Assign hqEnt to UE only if msg4 is done */

        rgSCHCmnDlInitHqEnt(cell, hqEnt);

        /* For Hand-In UE Request Aper CQI report
         * immediately */
        if (ueCfg->ueDlCqiCfg.aprdCqiCfg.pres)
        {
           /* Set APCQI for Pcell only*/
           ue->dl.reqForCqi = RG_SCH_APCQI_SERVING_CC;
        }
     }
     /* CA dev Start */
#ifdef LTE_TDD
     maxDlSubframes = rgSchTddNumDlSubfrmTbl[ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];
     maxSubframes = 2 * maxDlSubframes;
     ue->dl.numHqDlSfInfo = maxSubframes;
     rgSCHUtlAllocSBuf(cell->instIdx,
           (Data **)&ue->dl.dlSfHqInfo, sizeof(RgSchDlHqInfo) * (ue->dl.numHqDlSfInfo));

#else
     ue->dl.numHqDlSfInfo = RGSCH_NUM_DL_slotS;
#endif
#ifndef RG_5GTF 
     for (idx =0;idx < ue->dl.numHqDlSfInfo; idx++)
     {
        cmLListInit(&ue->dl.dlSfHqInfo[idx].hqPLst);
        ue->dl.dlSfHqInfo[idx].dlSfUeLnk.node = NULLP;

     } 
#else
     {
        uint8_t cellIdx=0;
        for (cellIdx = 0;cellIdx < MAX_5GTF_CELL ; cellIdx++)
        {
           for (idx =0;idx < ue->dl.numHqDlSfInfo; idx++)
           {
              cmLListInit(&ue->dl.dlSfHqInfo[cellIdx][idx].hqPLst);
              ue->dl.dlSfHqInfo[cellIdx][idx].dlSfUeLnk.node = NULLP;
           }
        }
     }
#endif
#ifdef LTE_ADV
     rgSCHLaaInitDlHqInfo(cell, ue);
#endif
     /* CA dev End */

     /* Initialize lcgIds to Invalid */
     for (lcgCnt = 0; lcgCnt < RGSCH_MAX_LCG_PER_UE; lcgCnt++)
     {
        ue->ul.lcgArr[lcgCnt].lcgId = RGSCH_INVALID_LCG_ID;
     }
     if(raCb != NULLP)
     {
        rgSCHCfgRgrUePhrMsg3(cell,raCb,ue,errInfo);
        /* Moved this code out of rgSCHCfgRgrUePhrMsg3()
         * as it was not the appropriate place to 
         * do this. */
        if (raCb->raState == RGSCH_RA_MSG4_DONE)
        {
           DU_LOG("\nERROR  -->  SCH : RNTI:%d RaCb deleted as Msg4 transmission is done",
                 raCb->tmpCrnti);
           rgSCHRamDelRaCb(cell, raCb, FALSE);
        }
     }
     /* Initialize uplink HARQ related information for UE */
     rgSCHUhmRgrUeCfg(cell, ue, ueCfg);
     cmInitTimers(&ue->bsrTmr, 1);
#ifdef RGR_V1
     /* Added periodic BSR timer */
     cmInitTimers(&ue->bsrTmr, 1);

     /* Fix - Added proper configuration from U-ARM */
     if(ueCfg->ueBsrTmrCfg.isPrdBsrTmrPres == TRUE)
     {
        ue->ul.bsrTmrCfg.isPrdBsrTmrPres = TRUE;
        ue->ul.bsrTmrCfg.prdBsrTmr       = ueCfg->ueBsrTmrCfg.prdBsrTmr;
        ue->ul.bsrTmrCfg.retxBsrTmr      = ueCfg->ueBsrTmrCfg.retxBsrTmr;
     }
    
#endif
     /* Initialize downlink HARQ related information for UE */
     rgSCHDhmRgrUeCfg(cell, ue, ueCfg, errInfo);

     /* Initialize MeasureGap and Acknack Rep Information for UE */
     if((rgSCHMeasGapANRepUeCfg(cell, ue, ueCfg)) != ROK)
     {
        DU_LOG("\nERROR  -->  SCH : Measurement Gap and"
                 " AckNack Rep failed in Config for CRNTI:%d", ueCfg->crnti);
        break;
     }


#ifdef LTE_TDD
     if((rgSCHUtlAllocUeANFdbkInfo(ue,RGSCH_PCELL_INDEX)) != ROK)
     {
        DU_LOG("\nERROR  -->  SCH : Memomy allocation "
                 "Failed while UE related Ack Nack Information for CRNTI:%d",
                 ueCfg->crnti);
        break;
     }
     ue->dl.ackNackMode = ueCfg->ackNackModeEnum;
#endif /* LTE_TDD */

     /* Insert Ue */
     rgSCHDbmInsUeCb(cell, ue);

#ifdef TFU_UPGRADE
     /*  Int ialize APeriodic CQI/PMI/RI Information for UE */

      DU_LOG("\nDEBUG  -->  SCH : rgSCHCfgRgrUeCfg CellID=%d UeId =%d AcqiCfg Pres =%d", cell->cellId, ue->ueId,\
      ueCfg->ueDlCqiCfg.aprdCqiCfg.pres);

      /*Store Trigger Set Bit String to UE */
      
      ret = rgSCHCfgACqiUeCfg(cell,ue, (RG_SCH_CMN_GET_ACQICB(ue,cell)),ue->mimoInfo.txMode,
           &ueCfg->ueDlCqiCfg.aprdCqiCfg, ue->ueCatEnum);

     ue->cqiRiWritIdx = 0;
     ue->cqiRiReadIdx = 0;
     /*   Initialize Periodic CQI/PMI, RI Information for UE  */
     ret = rgSCHCfgPCqiUeCfg(cell, ue, &ueCfg->ueDlCqiCfg.prdCqiCfg, 
           ue->ueCatEnum);

     /*   Initialize UL SRS Information for UE  */
     ret = rgSCHCfgSrsUeCfg(cell, ue, &ueCfg->srsCfg);

     /*   Initialize SR Information for UE  */
     ret = rgSCHCfgSrUeCfg(cell, ue, &ueCfg->srCfg);
#endif 

#ifdef LTEMAC_HDFDD
     if (rgSCHHdFddUeCfg(cell, ue, ueCfg->isHdFddEnbld) != ROK)
     {
        DU_LOG("\nERROR  -->  SCH : Could not do HD-FDD config for CRNTI:%d",ueCfg->crnti);
        break;
     }

#endif /* LTEMAC_HDFDD */
     /* ccpu00117452 - MOD - Changed macro name from
        RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
     ue->cqiReptCfgInfo.numColltdCqiRept =
        ueCfg->ueCqiReptCfg.numColltdCqiRept;
#endif /* End of RGR_CQI_REPT */
#ifdef TFU_UPGRADE
     RG_SCH_CMN_GET_PA(ue,cell).pres = FALSE;
     if (RG_SCH_UE_CFG_ISPAPRSNT(ueCfg->uePdschDedCfg.uepACfg))
     {
        RG_SCH_CMN_GET_PA(ue,cell).pres = TRUE;
        RG_SCH_CMN_GET_PA(ue,cell).val = ueCfg->uePdschDedCfg.uepACfg.pA;
     }
#endif
     ue->isDrxEnabled = ueCfg->ueDrxCfg.isDrxEnabled;

     if ( ue->isDrxEnabled )
     {
        if((rgSCHDrxUeCfg(cell,ue,ueCfg)) != ROK )
        {
           DU_LOG("\nERROR  -->  SCH : DRX configuration failed");
           break;
        }
     }

     /* LTE_ADV_FLAG_REMOVED_START */
     if ((cell->lteAdvCb.sfrCfg.status == RGR_ENABLE) ||    \
        (cell->lteAdvCb.absCfg.status == RGR_ENABLE))
     {
        ue->lteAdvUeCb.rgrLteAdvUeCfg = ueCfg->ueLteAdvCfg;
     }   
     /* LTE_ADV_FLAG_REMOVED_END */

#ifdef TENB_STATS
     ue->tenbStats = TSL2AllocUeStatsBlk(ue->ueId);
     ue->tenbStats->stats.rnti = ue->ueId;
#endif
#ifdef LTE_ADV
     /*Update A Value for PCell TBs*/
     ue->f1bCsAVal = rgSCHUtlGetMaxTbSupp(ue->mimoInfo.txMode); 
     DU_LOG("\nERROR  -->  SCH :  UeCfg A value is %d\n",ue->f1bCsAVal);
#endif
     errInfo->errCause = RGSCHERR_NONE;

     ue->accessStratumRls = ueCfg->accessStratumRls;
     if (ue->numSCells > 0)
     {
        /* 2 bit CSI */
        rgSCHUtlUpdUeDciSize(cell, ue, TRUE);
     }
     else
     {
        /* 1 bit CSI Access Stratum Release Change */
        rgSCHUtlUpdUeDciSize(cell, ue, FALSE);
     }

     return ROK;
  }while(0);

  if (ue)
  {
     rgSCHCfgFreeUeCb(cell, ue);
  }
  return RFAILED;
}  /* rgSCHCfgRgrUeCfg */

/**
 * @brief Handler for PHR for MSG3.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrUePhrMsg3
 *
 *     Processing Steps:
 *       Handle PHR related config for MSG3
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgrUeCb     *ueCb
 *  @param[in]  RgSchRaCb    *raCb
 *  @param[out] RgSchErrInfo *errInfo
 **/
static Void rgSCHCfgRgrUePhrMsg3
(
RgSchCellCb  *cell,
RgSchRaCb    *raCb,
RgSchUeCb    *ue,
RgSchErrInfo *errInfo
)
{

   /* Record msg3 allocation in the UE */
   rgSCHUtlRecMsg3Alloc(cell, ue, raCb);

   /* If raCb received PHR, update scheduler */
   if(raCb->phr.pres == TRUE)
   {
      ue->macCeRptTime = raCb->msg3AllocTime; 
      rgSCHUtlUpdPhr(cell, ue, raCb->phr.val, errInfo);
   }

   return;
}

/**
 *
 * @details
 *
 *     Function : rgSCHDynCfiReCfg
 *
 *  @param[in]  RgSchCellCb   *cell
 *              Bool          isDynCfiEnb 
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
Void rgSCHDynCfiReCfg
(
RgSchCellCb   *cell,
Bool          isDynCfiEnb
)
{            
   uint8_t                idx;
   RgSchCmnDlCell    *cellSchDl = RG_SCH_CMN_GET_DL_CELL(cell);

   if(isDynCfiEnb)
   {
      cell->dynCfiCb.ttiCnt = 0;
      cellSchDl->newCfi = cellSchDl->currCfi;
   }   
   else
   {
      /* Resetting the parameters*/
      cell->dynCfiCb.cceFailCnt          = 0;
      cell->dynCfiCb.cceFailSum          = 0;
      cell->dynCfiCb.prevCceFailIdx      = 0;

      for(idx = 0; idx < cell->dynCfiCb.numFailSamples; idx++)
      {
         cell->dynCfiCb.cceFailSamples[idx] = 0;
      }   

      cell->dynCfiCb.cceUsed             = 0;
      cell->dynCfiCb.lowCceCnt           = 0;
      cell->dynCfiCb.ttiCnt = 0;
   }   
}
/**
 * @brief Handler for the cell reconfiguration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrCellRecfg
 *
 *     Processing Steps:
 *      - Invoke SCH with cell control block to update
 *        scheduler specific information.
 *      - Update cell control block with the values recieved in the
 *        configuration.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgrCellRecfg  *cellRecfg
 *  @param[out] RgSchErrInfo  *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgRgrCellRecfg
(
RgSchCellCb   *cell,
RgrCellRecfg  *cellRecfg,
RgSchErrInfo  *errInfo
)
{
   S16      ret;
   Inst    inst = cell->instIdx;
/* LTE_ADV_FLAG_REMOVED_START */
   uint8_t      i = 0;
   uint16_t     len; /* dsfr_pal_fixes ** 21-March-2013 ** SKS */
/* LTE_ADV_FLAG_REMOVED_END */
   
   errInfo->errCause = RGSCHERR_CFG_RGR_CELL_RECFG;
   
   /* Invoke scheduler to update scheduler specific information */
   ret =  rgSCHUtlRgrCellRecfg(cell, cellRecfg, errInfo);
   if (ret != ROK)
   {
     DU_LOG("\nERROR  -->  SCH : RGR Cell re-configuration failed "
          "at Scheduler ");
      return RFAILED;
   }
   
   /* Invoke DHM to update DHM specific information */
   rgSCHDhmRgrCellRecfg(cell, cellRecfg, errInfo);

      /* PUCCH Reconfiguration */
   if (cellRecfg->recfgTypes & RGR_CELL_PUCCH_RECFG)
   {
      cell->pucchCfg = cellRecfg->pucchRecfg;
   }
 
   /* SRS Reconfiguration */
   if (cellRecfg->recfgTypes & RGR_CELL_SRS_RECFG)
   {
      cell->srsCfg.isSrsCfgPres = cellRecfg->srsRecfg.isSrsCfgSetup;
      if(cellRecfg->srsRecfg.isSrsCfgSetup)
      {
         cell->srsCfg.srsCfgPrdEnum = cellRecfg->srsRecfg.srsCfgPrdEnum;
         cell->srsCfg.srsBwEnum = cellRecfg->srsRecfg.srsBwEnum;
         cell->srsCfg.srsTxOffst =
            rgSrsTxOffstTbl[cellRecfg->srsRecfg.srsSubFrameCfg];
         /*ccpu00116923 - ADD - Srs Present support */
#ifdef TFU_UPGRADE
         cell->srsCfg.srsSubFrameCfg = cellRecfg->srsRecfg.srsSubFrameCfg;
#endif
      }
   }

   /* RACH Reconfiguration */
   if (cellRecfg->recfgTypes & RGR_CELL_RACH_RECFG)
   {
      cell->rachCfg = cellRecfg->rachRecfg;
   }

   /* ccpu00132256:MOD: Moved this assignment from Validation to here.*/
   if (cellRecfg->recfgTypes & RGR_CELL_TMRS_RECFG)
   {
      cell->t300TmrVal = cellRecfg->t300TmrVal;
   }
#ifdef RGR_SI_SCH
   /* SI Reconfiguration */
   if (cellRecfg->recfgTypes & RGR_CELL_SI_RECFG)
   {
      /*Set the specified SI configuration. */
      cell->siCb.newSiCfg = cellRecfg->siReCfg;
      /* Set the Bit mask for SI re-configuration */
      cell->siCb.siBitMask |= RGSCH_SI_SICFG_UPD; 
#ifdef EMTC_ENABLE
      if(cell->emtcEnable)
      {
      rgSchEmtcUpdSiCfg(cell, cellRecfg);
      }
#endif
   }
#endif /*RGR_SI_SCH */

   /* Overload RACH Control changes */
   if (cellRecfg->recfgTypes & RGR_CELL_CNTRL_CMD_RECFG)
   {
      if (cellRecfg->cntrlCmdCfg.cmdType == RGR_CNTRL_CMD_RACH_OVRLD)
      {
         cell->overLoadBackOffEnab = cellRecfg->cntrlCmdCfg.cmdDesc.rachOvrLd.backOffEnb;
         cell->overLoadBackOffval = cellRecfg->cntrlCmdCfg.cmdDesc.rachOvrLd.backOffVal;
      }
      else if (cellRecfg->cntrlCmdCfg.cmdType == RGR_CNTRL_CMD_CPU_OVRLD)
      {
         if( ROK != rgSCHUtlResetCpuOvrLdState(cell, cellRecfg->cntrlCmdCfg.cmdDesc.\
                                          cpuOvrLd.instruction))
         {
            DU_LOG("\nERROR  -->  SCH : Invalid CPU OvrLd Ins %d for cell", 
	       cellRecfg->cntrlCmdCfg.cmdDesc.cpuOvrLd.instruction);
            return RFAILED;
         }
      }
   }

/* LTE_ADV_FLAG_REMOVED_START */
   if (cellRecfg->recfgTypes & RGR_CELL_LTEA_FEATURE_RECFG)
   {
      if(cellRecfg->rgrLteAdvCfg.pres & RGR_ABS)
      {
         cell->lteAdvCb.absCfg =
            cellRecfg->rgrLteAdvCfg.absCfg;         
      }
      if(cellRecfg->rgrLteAdvCfg.pres & RGR_SFR)
      {
         memcpy(&cell->lteAdvCb.sfrCfg, &cellRecfg->rgrLteAdvCfg.sfrCfg,
               sizeof(RgrSfrConfig));
         /* dsfr_pal_fixes ** 21-March-2013 ** SKS */
         if (cellRecfg->rgrLteAdvCfg.sfrCfg.status == RGR_ENABLE)
         {
            for(i = 0; i < RGSCH_NUM_DL_slotS; i++)
            {
               /*initialise the pools of CC and CE*/
               if(rgSchSFRTotalPoolInit(cell, cell->subFrms[i]))
               {
                  return RFAILED;
               }
            }
         }
         else
         {
            for(i = 0; i < RGSCH_NUM_DL_slotS; i++)
            {
               /*initialise the pools of CC and CE*/
               rgSchSFRTotalPoolFree(&cell->subFrms[i]->sfrTotalPoolInfo, cell);
            }

            if (cell->lteAdvCb.dsfrCfg.status == RGR_ENABLE)
            {
               /* releasing rntp info val from each subframe */ 
               for(i = 0; i < RGSCH_NUM_DL_slotS; i++)
               {
                  rgSchDSFRRntpInfoFree(&cell->subFrms[i]->rntpInfo, cell, cell->bwCfg.dlTotalBw);
               }

               /* releasing RNTP Aggregation Info from CellCb*/
               rgSchDSFRRntpInfoFree(&cell->rntpAggrInfo, cell, cell->bwCfg.dlTotalBw); 

               cell->lteAdvCb.dsfrCfg.status = RGR_DISABLE;
            }
         }
      }
      /* dsfr_pal_fixes ** 21-March-2013 ** SKS ** Start */
      if(cellRecfg->rgrLteAdvCfg.pres & RGR_DSFR)
      {
         cell->lteAdvCb.dsfrCfg =
            cellRecfg->rgrLteAdvCfg.dsfrCfg;
         if (cell->lteAdvCb.dsfrCfg.status == RGR_ENABLE)
         {
            for(i = 0; i < RGSCH_NUM_DL_slotS; i++)
            {
               /*initialise the pools of CC and CE*/
               if(rgSchDSFRRntpInfoInit(&cell->subFrms[i]->rntpInfo,cell,cell->bwCfg.dlTotalBw))
               {
                  return RFAILED;
               }
            }
            /*Calculating the length of RNTP array based on Dl Bandwidth */
            len = (uint16_t)((cell->bwCfg.dlTotalBw % 8 == 0) ? (cell->bwCfg.dlTotalBw/8) : (cell->bwCfg.dlTotalBw/8 + 1)); /* KW fix for LTE_ADV */
            if(cell->rntpAggrInfo.pres == NOTPRSNT)
            {   
               if((rgSCHUtlAllocSBuf(inst, (Data**)&(cell->rntpAggrInfo.val),
                           (len * sizeof(uint8_t)))) != ROK)
               {
                  DU_LOG("\nERROR  -->  SCH : Memory allocation FAILED for RNTP Alloc");
                  return RFAILED;
               }
               cell->rntpAggrInfo.pres = PRSNT_NODEF;
               cell->rntpAggrInfo.len  = len; 
            }
         }
         /* in case if DSFR is disabled, need to free RNTP pattern val*/
         else
         {
            /* releasing rntp info val from each subframe */ 
            for(i = 0; i < RGSCH_NUM_DL_slotS; i++)
            {
               rgSchDSFRRntpInfoFree(&cell->subFrms[i]->rntpInfo, cell, cell->bwCfg.dlTotalBw);
            }

            /* releasing RNTP Aggregation Info from CellCb*/
            rgSchDSFRRntpInfoFree(&cell->rntpAggrInfo, cell, cell->bwCfg.dlTotalBw);
         }   
      }
      /* dsfr_pal_fixes ** 21-March-2013 ** SKS ** End */
   }
/* LTE_ADV_FLAG_REMOVED_END */

   /* Dynamic CFI cell Reconfiguration */
   if(cellRecfg->recfgTypes & RGR_CELL_DYN_CFI_RECFG)
   {
      if(cell->dynCfiCb.isDynCfiEnb != cellRecfg->isDynCfiEnb)
      {
         if(cell->dynCfiCb.switchOvrInProgress)  
         {
            cell->dynCfiCb.dynCfiRecfgPend = TRUE;
         }  
         else
         { 
            cell->dynCfiCb.isDynCfiEnb = cellRecfg->isDynCfiEnb;
            rgSCHDynCfiReCfg(cell, cellRecfg->isDynCfiEnb); 
         }
      }   
      else
      {
         /* To hanlde the case where reconfiguration comes for disabling 
          * and then enabling before switchover period expires */
         cell->dynCfiCb.dynCfiRecfgPend = FALSE;
      }   
   }   
   /* Dynamic config of AUTO chnage flag */
   if(cellRecfg->recfgTypes & RGR_CELL_AUTO_CFG_MODE_RECFG)
   {
      if(cell->isAutoCfgModeEnb != cellRecfg->isAutoCfgModeEnb)
      {
         cell->isAutoCfgModeEnb = cellRecfg->isAutoCfgModeEnb;
      }
   }
   {
     if(cell->isAutoCfgModeEnb)
     {
        DU_LOG("\nINFO  -->  SCH : Auto Mode Cfg enabled durint cell recfg\n");
     }
   }

   if (cellRecfg->recfgTypes & RGR_CELL_CSG_PARAM_RECFG)
   {
      cell->minDlResNonCsg = cellRecfg->csgParamCfg.minDlResNonCsg;
      cell->minUlResNonCsg = cellRecfg->csgParamCfg.minUlResNonCsg;
   }
  
   errInfo->errCause = RGSCHERR_NONE;
   return ROK;
}  /* rgSCHCfgRgrCellRecfg */

/**
 * @brief Handler for the UE reconfiguration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrUeRecfgRntiChg
 *
 *     Processing Steps:
 *      - If rnti changes,
 *        - Invoke RAM for UE reconfiguration.
 *        - Delete old UE from the list.
 *        - Update the new rnti and re-insert the UE in the list.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeRecfg   *ueRecfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
static S16 rgSCHCfgRgrUeRecfgRntiChg
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgrUeRecfg     *ueRecfg,
RgSchErrInfo   *errInfo
)
{
#ifdef LTE_ADV
   uint8_t          sCellIdx;
#endif
   S16          ret;
   RgSchRaCb    *raCb;
   RgSchRaCb    *oldRaCb;
   RgSchDlHqEnt **hqEnt = &(RG_SCH_CMN_GET_UE_HQE(ue, cell));
   uint8_t           idx;
    
   /* Handle CRNTI change in reconfiguration */
   if (ueRecfg->oldCrnti != ueRecfg->newCrnti)
   {
      RgSchRntiLnk   *oldRntiLnk=NULLP;
      CmLteRnti      oldRnti = 0;
      if ((raCb = rgSCHDbmGetRaCb(cell, ueRecfg->newCrnti)) == NULLP)
      {
         DU_LOG("\nERROR  -->  SCH : UEID:No RaCb exists while"
              "Reconfig for OLD CRNTI:%d NEW CRNTI:%d",ueRecfg->oldCrnti,ueRecfg->newCrnti);
         return RFAILED;
      }

      /* rntiLnk does not exist for a HandIn UE. Hence this check. */
      if(ue->rntiLnk)
      {
         oldRntiLnk = ue->rntiLnk;
      }
      else
      {
    	 /* Fix : syed HO UE does not have a valid ue->rntiLnk */
         oldRnti = ue->ueId;
      }
     
      DU_LOG("\nINFO  -->  SCH : UE ID CHNG OLD %d new %d",ueRecfg->oldCrnti, ueRecfg->newCrnti); 

      /* Fix : syed Deleting Old DL HqEnt. It would be assigned after
       * reest RACH(msg4) is completed. */
      rgSCHDhmDelHqEnt(cell, hqEnt);

      /* Initialize RAM related information for UE */
      ret = rgSCHRamRgrUeCfg(cell, ue, raCb, errInfo);
      if (ret != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : RAM Handling for UE Reconfig failed"
              "for OLD CRNTI:%d NEW CRNTI:%d",ueRecfg->oldCrnti,ueRecfg->newCrnti);
         return RFAILED;
      }
      /* Delete Ue from the ue list */
      rgSCHDbmDelUeCb(cell, ue);

#ifdef LTE_ADV
      if (ue->numSCells)
      {
         for ( sCellIdx = 1; sCellIdx < CM_LTE_MAX_CELLS; sCellIdx++)
         {
            if(ue->cellInfo[sCellIdx] != NULLP)
            {
               rgSCHDbmDelUeCb(ue->cellInfo[sCellIdx]->cell, ue);
            }
         }
      }
#endif

      /* Inititialize Ue control block */
      ue->ueId = ueRecfg->newCrnti;
      DU_LOG("\nDEBUG  -->  SCH : Changing RNTI from %d to %d",
            ueRecfg->oldCrnti, 
            ueRecfg->newCrnti);
#ifdef EMTC_ENABLE 
      if(ue->isEmtcUe)
      {
         rgSCHUtlUpdEmtcY(ue);
      }
#endif
      
      /* Fix ccpu00122631: PCell_Reest: Updating new Rnti in all the cells
       * dlAllocCb 
       */
      for(idx = 0; idx < CM_LTE_MAX_CELLS; idx++)
      {
         if(ue->cellInfo[idx])
         {
            ue->cellInfo[idx]->dlAllocCb.rnti = ueRecfg->newCrnti;
         }
      }

      rgSCHUtlRecMsg3Alloc(cell, ue, raCb);

      /* If raCb received PHR, update scheduler */
      if(raCb->phr.pres == TRUE)
      {
         ue->macCeRptTime = raCb->msg3AllocTime; 
         rgSCHUtlUpdPhr(cell, ue, raCb->phr.val, errInfo);
      }

#ifdef RGR_V2  /* Acc Fix */
     if(TRUE == ue->isDrxEnabled)
     {
         ueRecfg->ueDrxRecfg.isDrxEnabled = TRUE;
         ret = rgSCHDrxUeReCfg(cell,ue,ueRecfg);  

         if ( ret != ROK )
         {
             DU_LOG("\nERROR  -->  SCH : UE DRX re-est failed"
              "for OLD CRNTI:%d NEW CRNTI:%d",ueRecfg->oldCrnti,ueRecfg->newCrnti);
             return RFAILED;
         }
     }
#endif /* Acc Fix */

      /* Re-insert updated Ue */
      rgSCHDbmInsUeCb(cell, ue);

#ifdef LTE_ADV
      if (ue->numSCells)
      {
         for ( sCellIdx = 1; sCellIdx < CM_LTE_MAX_CELLS; sCellIdx++)
         {
            if(ue->cellInfo[sCellIdx] != NULLP)
            {
               rgSCHDbmInsUeCb(ue->cellInfo[sCellIdx]->cell, ue);
            }
         }
      }
#endif


#ifdef TENB_STATS
      ue->tenbStats->stats.rnti = ue->ueId;
#endif

      /* Fix : syed If MSG4 is done, since corresponding ueCb
       * is ready, the raCb should be cleared immediately.
       * Otherwise it would remain in the cell until timed out
       * and till then the hq Feedbacks will be assumed to be
       * for msg4 */
      if (raCb->raState == RGSCH_RA_MSG4_DONE)
      {
         DU_LOG("\nDEBUG  -->  SCH : RNTI:%d with RaCb deleted as Msg4 transmission is done", 
               raCb->tmpCrnti);
         rgSCHRamDelRaCb(cell, raCb, FALSE);
      }
      /* Fix : syed moving the UL CQI initialization to UERESET */

      /* Release Older rnti */
      if(oldRntiLnk)
      {
         /* This is the rare case in which back to back reestablishment is 
          * happening and previous re-est was not done completely (MSG4 was 
          * not done) for an UE, and again re-est is triggered for the same 
          * UE. We are deleting the old RA CB for the previous re-est which 
          * still exist due to MSG4 not transmitted successfully */
         if ((oldRaCb = rgSCHDbmGetRaCb(cell, oldRntiLnk->rnti)) != NULLP)
         {
            rgSCHRamDelRaCb(cell, oldRaCb, FALSE);
         }

         rgSCHUtlRlsRnti(cell, oldRntiLnk, TRUE, ueRecfg->newCrnti);
      }
      else
      {
    	 /* Fix : syed HO UE does not have a valid ue->rntiLnk */
	     /* Just indicate to MAC, no need to release at SCH */
         DU_LOG("\nDEBUG  -->  SCH : HO OldRnti:%d RLS and NewRnti:%d CHNG IND TO MAC",
               oldRnti, ueRecfg->newCrnti);
         rgSCHUtlIndRntiRls2Mac(cell, oldRnti, TRUE, ueRecfg->newCrnti);
      }
   }
   return ROK;
}
/**
 * @brief Handler for the UE reconfiguration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrUeRecfg
 *
 *     Processing Steps:
 *      - If rnti changes,
 *        - Invoke RAM for UE reconfiguration.
 *        - Delete old UE from the list.
 *        - Update the new rnti and re-insert the UE in the list.
 *      - Update the UE control block with the reconfigured values.
 *      - Invoke SCH, UHM and DHM with updated UE control block to 
 *        update scheduler, uplink HARQ and downlink HARQ specific
 *        parameters.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeRecfg   *ueRecfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgRgrUeRecfg
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgrUeRecfg     *ueRecfg,
RgSchErrInfo   *errInfo
)
{
   S16          ret;
#ifdef LTE_ADV
   Bool dciChange = TRUE;
#endif
   
   errInfo->errCause = RGSCHERR_CFG_RGR_UE_RECFG;

#ifdef LTE_ADV
   if (ue->numSCells > 0)
   {
      dciChange = FALSE;
   }
   if ((ueRecfg->ueRecfgTypes & RGR_UE_UE_ACCESS_STRATUM_REL_RECFG) && \
         (ue->accessStratumRls != ueRecfg->accessStratumRls))
   {
      ue->accessStratumRls = ueRecfg->accessStratumRls;
      dciChange = TRUE;
   }

   /* if SCELL_RECFG is present , no other 
    * type will be present. Process Scell addition
    * and return 
    * */
   if (ueRecfg->ueRecfgTypes & RGR_UE_SCELL_ADD_RECFG)
   {
      ret = rgSCHSCellCfgUeCfg(cell, ue, ueRecfg, errInfo);
      if( ret != ROK)
      {
         errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_UE_SCELL_RECFG;
         /*FH: SCell config failed for a scell index hence revert all successful
          * Scell config and send negative confirmation to APP*/
         rgSCHSCellCfgUeCfgRollBack(cell, ue, ueRecfg);
         return RFAILED;
      }
   }
   if (dciChange == TRUE)
   {
      if (ue->numSCells > 0)
      {
         /* 2 bit CSI */
         rgSCHUtlUpdUeDciSize(cell, ue, TRUE);
      }
      else
      {
         /* 1 bit CSI Access Stratum Release Change */
         rgSCHUtlUpdUeDciSize(cell, ue, FALSE);
      }
   }
   if (ueRecfg->ueRecfgTypes & RGR_UE_SCELL_PUCCH_RECFG)
   {
      ret = rgSCHSCellCfgUePucchReCfg(cell, ue, ueRecfg, errInfo);
      if( ret != ROK)
      {
         errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_UE_SCELL_PUCCH_RECFG;
         return RFAILED;
      }
   }
#else
   if ((ueRecfg->ueRecfgTypes & RGR_UE_UE_ACCESS_STRATUM_REL_RECFG) && \
         (ue->accessStratumRls != ueRecfg->accessStratumRls))
   {
      ue->accessStratumRls = ueRecfg->accessStratumRls;
      rgSCHUtlUpdUeDciSize(cell, ue, FALSE);
   }
#endif /* LTE_ADV */

   if (ueRecfg->ueRecfgTypes)
   {
      /* Update scheduler related information for UE */
      ret = rgSCHUtlRgrUeRecfg(cell, ue, ueRecfg, errInfo);
      if (ret != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : Scheduler handling while reconfig failed"
              "for OLD CRNTI:%d NEW CRNTI:%d",ueRecfg->oldCrnti,ueRecfg->newCrnti);
         return RFAILED;
      }

      /* Update uplink HARQ related information for UE */
      rgSCHUhmRgrUeRecfg(cell, ue, ueRecfg);

      /* Update TA related information for UE */
      if (ueRecfg->ueRecfgTypes & RGR_UE_TATMR_RECFG)
      {
         rgSCHCfgUeTaRecfg(cell, ue, ueRecfg, errInfo);
      }

      /*Update Measurement Gap and AckNack Details */
      /* After merging from 2.2 */
      if (ueRecfg->ueRecfgTypes & RGR_UE_ACKNACK_MEASGAP_RECFG)
      {
         ret = rgSCHMeasGapANRepUeRecfg(cell, ue, ueRecfg);
         if (ret != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Measurement Gap and"
                     "AckNack Rep Recfg failed for OLD CRNTI:%d NEW CRNTI:%d",
                      ueRecfg->oldCrnti,ueRecfg->newCrnti);
            return RFAILED;
         }
      }
      if (ueRecfg->ueRecfgTypes & RGR_UE_BSRTMR_RECFG)
      {
         cmInitTimers(&ue->bsrTmr, 1);
         ue->ul.bsrTmrCfg = ueRecfg->ueBsrTmrRecfg;
         if ((ue->ul.bsrTmrCfg.isPrdBsrTmrPres) && 
               (ue->ul.bsrTmrCfg.prdBsrTmr == 0xFFFF))
         {
            ue->ul.bsrTmrCfg.isPrdBsrTmrPres = FALSE;
         }
      }
   }

   if (RFAILED == rgSCHCfgRgrUeRecfgRntiChg (cell, ue, ueRecfg, errInfo))
   {
       DU_LOG("\nERROR  -->  SCH : RNTI change "
               "failed for OLD CRNTI:%d NEW CRNTI:%d",
                ueRecfg->oldCrnti,ueRecfg->newCrnti);
       return RFAILED;
   }

#ifdef TFU_UPGRADE
   /* Re-Initialize Aperiodic CQI Information for UE*/
   if ( ueRecfg->ueRecfgTypes & RGR_UE_APRD_DLCQI_RECFG ) 
   {
      ret = rgSCHCfgAcqiUeReCfg(cell, ue, &ueRecfg->aprdDlCqiRecfg, 
            ue->ueCatEnum);
   }
   /*   Re-Initialize Periodic CQI/PMI, RI Information for UE  */
   if ( ueRecfg->ueRecfgTypes & RGR_UE_PCQI_RECFG) 
   {
      ret = rgSCHCfgPCqiUeReCfg(cell, ue, &ueRecfg->cqiCfg, 
            ue->ueCatEnum);
   }
   /*   Re-Initialize UL SRS Information for UE  */
   if ( ueRecfg->ueRecfgTypes & RGR_UE_SRS_RECFG) 
   {
      ret = rgSCHCfgSrsUeReCfg(cell, ue, &ueRecfg->srsCfg);
   }
   /*   Re-Initialize SR Information for UE  */
   if ( ueRecfg->ueRecfgTypes & RGR_UE_SR_RECFG) 
   {
      ret = rgSCHCfgSrUeReCfg(cell, ue, &ueRecfg->srCfg);
   }
#endif 

#ifdef LTEMAC_HDFDD  
   if(ueRecfg->isHdFddEnbld)
   {
        ret = rgSCHHdFddUeCfg(cell, ue, ueRecfg->isHdFddEnbld);
        if (ret != ROK)
        {
           errInfo->errCause = RGSCHERR_HDFDD_SPSCFGRD;
           return (ret);
        }
   }
#endif /* LTEMAC_HDFDD */
#ifdef RGR_V2
  if ( ueRecfg->ueRecfgTypes & RGR_UE_DRX_RECFG)
  {
     ret = rgSCHDrxUeReCfg(cell,ue,ueRecfg);  

     if ( ret != ROK )
     {
        DU_LOG("\nERROR  -->  SCH : UE DRX reconfig failed"
               "failed for OLD CRNTI:%d NEW CRNTI:%d",
                ueRecfg->oldCrnti,ueRecfg->newCrnti);
        return RFAILED;
     }
  }
#endif  
/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   /* CQI Reporting (N)  Re-configuration */
   if(ueRecfg->ueRecfgTypes & RGR_UE_CQIREPT_RECFG)
   {
      ret = rgSCHCfgUeCqiReptReCfg(cell, ue, ueRecfg);
      if(ret != OK)
      {
         errInfo->errCause = RGSCHERR_CQIREPT;
         return (ret);
      }
   }
#endif /* End of RGR_CQI_REPT */
#ifdef TFU_UPGRADE
   /* pA Re-configuration */
   if((ueRecfg->ueRecfgTypes & RGR_UE_PA_RECFG) &&
         RG_SCH_UE_CFG_ISPAPRSNT(ueRecfg->uePdschDedCfg.uepACfg))
   {
      RG_SCH_CMN_GET_PA(ue,cell).pres = TRUE;
      RG_SCH_CMN_GET_PA(ue,cell).val = ueRecfg->uePdschDedCfg.uepACfg.pA;
   }
#endif

/* LTE_ADV_FLAG_REMOVED_START */
   if(ueRecfg->ueRecfgTypes & RGR_UE_LTEA_RECFG)
   {
      if(ueRecfg->ueLteAdvCfg.pres & RGR_ABS)
      {
         ue->lteAdvUeCb.rgrLteAdvUeCfg.isAbsUe = ueRecfg->ueLteAdvCfg.isAbsUe;
      }

      if(ueRecfg->ueLteAdvCfg.pres & RGR_SFR)
      {
         ue->lteAdvUeCb.rgrLteAdvUeCfg.isUeCellEdge = ueRecfg->ueLteAdvCfg.isUeCellEdge;
      }
   }
/* LTE_ADV_FLAG_REMOVED_END */
#ifdef EMTC_ENABLE
   if(ueRecfg->ueRecfgTypes & RGR_UE_EMTC_DPLXMODE_RECFG)
   {
      rgSCHEmtcHdFddUeCfg (cell, ue,
               ueRecfg->emtcUeRecfg.isHdFddEnbld);
   }
   if(ueRecfg->ueRecfgTypes & RGR_UE_EMTC_PO_TRIGGER)
   {
      rgSCHEmtcPOTrigger(cell, ue);
   }
#endif
   errInfo->errCause = RGSCHERR_NONE;

   return ROK;
}  /* rgSCHCfgRgrUeRecfg */


/**
 * @brief Handler for the logical channel reconfiguration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrLchRecfg
 *
 *     Processing Steps:
 *      - Invoke scheduler to update scheduler specific information.
 *      - Update the dedicated logical channel Cb with the reconfigured
 *        values.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgUlCellCb  *cell
 *  @param[in]  RgUlUeCb    *ue
 *  @param[in]  RgSchUlLcCb    *ulLc
 *  @param[in]  RgSchDlLcCb    *dlLc
 *  @param[in]  RgrLchRecfg *lcRecfg
 *  @param[out] RgSchErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgRgrLchRecfg
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgSchDlLcCb    *dlLc,
RgrLchRecfg    *lcRecfg,
RgSchErrInfo   *errInfo
)
{
   S16     ret = ROK;

   errInfo->errCause = RGSCHERR_CFG_RGR_LC_RECFG;
   /* Invoke Scheduler to update the new configuration */
   ret = rgSCHUtlRgrLcRecfg(cell, ue, dlLc, lcRecfg, errInfo);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Scheduler handling for LC Recfg"
              " failed for CRNTI:%d LCID:%d",lcRecfg->crnti,lcRecfg->lcId);
      return RFAILED;
   }

   errInfo->errCause = RGSCHERR_NONE;
   return ROK;
}  /* rgSCHCfgRgrLchRecfg */
/**
 * @brief Handler for the logical channel reconfiguration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrLcgRecfg
 *
 *     Processing Steps:
 *      - Invoke scheduler to update scheduler specific information.
 *      - Update the dedicated logical channel Cb with the re-configured
 *        values.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgUlCellCb   *cell
 *  @param[in]  RgUlUeCb     *ue
 *  @param[in]  RgrLcgRecfg  *lcgRecfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgRgrLcgRecfg
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgrLcgRecfg    *lcgRecfg,
RgSchErrInfo   *errInfo
)
{
   S16     ret = ROK;

   errInfo->errCause = RGSCHERR_CFG_RGR_LCG_RECFG;
  
   /*Added for handling LCG ReConfig if the LCG was deleted */
   ue->ul.lcgArr[lcgRecfg->ulRecfg.lcgId].lcgId = lcgRecfg->ulRecfg.lcgId;

   /* Invoke Scheduler to update the new configuration */
   ret = rgSCHUtlRgrLcgRecfg(cell, ue, lcgRecfg, errInfo);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Scheduler handling for LCG Recfg"
              " failed for CRNTI:%d LCGID:%d",lcgRecfg->crnti,lcgRecfg->ulRecfg.lcgId);
      return RFAILED;
   }

   errInfo->errCause = RGSCHERR_NONE;
   return ROK;
}  /* rgSCHCfgRgrLcgRecfg */

/**
 * @brief Handler for the UE Reset request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrUeReset
 *
 *     Processing Steps:
 *     - Call Measument Gap Module and Ack/Nack Module for resetting UE.
 *     - Call Common Schduler UE rest API which inturn will call scheduler
 *     specific UE Reset APis to reset UE.
 *
 *  @param[in]  RgSchCellCb    *cell
 *  @param[in]  RgSchUeCb      *ue
 *  @param[in]  RgrRst         *reset
 *  @param[out] RgSchErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgRgrUeReset
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgrRst         *reset,
RgSchErrInfo   *errInfo
)
{
   uint32_t     idx;
   RgSchRaCb *raCb;

   errInfo->errCause = RGSCHERR_CFG_RGR_UE_RESET;
  
   /* Setting BO of Each Logical Channel of the UE to 0 */
   for (idx = 0; idx < RGSCH_MAX_LC_PER_UE; idx++)
   {
      if(ue->dl.lcCb[idx] != NULLP)
         ue->dl.lcCb[idx]->bo = 0;
   }

   /* Reset the totalBo */
   ue->totalBo = 0;
   /* Call DRX module to stop all DRX timers */
   /* ccpu00129899 */
   if(ue->drxCb != NULLP)
   {
     (Void)rgSCHDrxUeDel(cell,ue);
   }

   /* ccpu00140894- Stop TXMode transiition timer if it is running*/
   if (ue->txModeTransTmr.tmrEvnt != TMR_NONE)
   {
      rgSCHTmrStopTmr(cell, RG_SCH_TMR_TXMODE_TRNSTN, ue);
      ue->txModeTransCmplt =TRUE;
   }

   /* ccpu00133470- Meas Gap should be released during RRC re-establishment */
   rgSCHMeasGapANRepUeDel(cell, ue, FALSE);

   /* Call Common scheduler which in turn will call specific scheduler for UE
    * Reset*/
   rgSCHUtlUeReset(cell, ue);
#ifdef LTE_ADV
   /*PCell which is at idx 0 is always active. Adding a line after the loop
    *setting RGSCH_PCELL_INDEX to SCELL ACTIVE*/
   ue->cellInfo[RGSCH_PCELL_INDEX]->sCellState = RG_SCH_SCELL_ACTIVE;
#endif

   /* In case of back to back reestablishments, when this UE's
    * previous ReEst is still in progress and has got RESET
    * as part of new ReEst */
   if((raCb = rgSCHDbmGetRaCb(cell, ue->ueId)) != NULLP)
   {
      rgSCHRamDelRaCb(cell, raCb, FALSE);
   }
   /* Fix : syed set UE inactive in DL until UE is reinitialization completed */
   ue->dl.dlInactvMask |= RG_HQENT_INACTIVE;
   ue->ul.ulInactvMask |= RG_HQENT_INACTIVE;
   /* [ccpu00127141] Resetting TA related parameters */
   ue->dl.taCb.ta = RGSCH_NO_TA_RQD;
   ue->dl.taCb.state = RGSCH_TA_IDLE;
   
   /*[ccpu00121813]-ADD-Initializing outstanding TA value */
   ue->dl.taCb.outStndngTa = FALSE;
   ue->dl.taCb.outStndngTaval = RGSCH_NO_TA_RQD;
   
   if (ue->dl.taCb.cfgTaTmr)
   {
      rgSCHTmrStartTmr (cell, ue, RG_SCH_TMR_TA, ue->dl.taCb.cfgTaTmr);
   }

#ifdef DL_LA
   /* Resetting the Tx mode change factor on UE reset */
   ue->mimoInfo.txModUpChgFactor = 0;
   ue->mimoInfo.txModDownChgFactor = 0;
#endif

   errInfo->errCause = RGSCHERR_NONE;
   return ROK;
}  /* rgSCHCfgRgrUeReset */

/**
 * @brief Handler for the cell delete request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrCellDel
 *
 *     Processing Steps:
 *      - Fetch the cell control block.
 *      - Remove the cell control block from the hash list of cells.
 *      - Free the cell control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  RgSchCellCb    *cell
 *  @param[in]  RgrDel         *cellDelInfo
 *  @param[out] RgSchErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgRgrCellDel
(
RgSchCellCb    *cell,
RgrDel         *cellDelInfo,
RgSchErrInfo   *errInfo
)
{ 

   errInfo->errCause = RGSCHERR_CFG_RGR_CELL_DEL;

   if (cell->cellId != cellDelInfo->u.cellDel.cellId)
   {
      DU_LOG("\nERROR  -->  SCH : Cell does not exist");
      return RFAILED;
   }

   /* Free the active cell */
   rgSCHCfgFreeCellCb(cell);

   errInfo->errCause = RGSCHERR_NONE;
   return ROK;
}  /* rgSCHCfgRgrCellDel */


/**
 * @brief Handler for the UE delete request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrUeDel
 *
 *     Processing Steps:
 *      - Fetch the UE control block.
 *      - Remove the UE control block from the hash list of UEs for the cell.
 *      - Free the UE control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  RgSchCellCb    *cell
 *  @param[in]  RgrDel         *ueDelInfo
 *  @param[out] RgSchErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgRgrUeDel
(
RgSchCellCb         *cell,
RgrDel              *ueDelInfo,
RgSchErrInfo        *errInfo
)
{
   RgSchUeCb        *ue;
   RgSchRaCb        *raCb;
#ifdef LTE_ADV
   Inst    inst = cell->instIdx;
   RgSchCellCb      *secCellCb = NULLP;
#endif

   errInfo->errCause = RGSCHERR_CFG_RGR_UE_DEL;

   if (cell->cellId != ueDelInfo->u.ueDel.cellId)
   {
      DU_LOG("\nERROR  -->  SCH : Cell does not exist CRNTI:%d",
                ueDelInfo->u.ueDel.crnti);
      return RFAILED;
   }
   if ((ue = rgSCHDbmGetUeCb(cell, ueDelInfo->u.ueDel.crnti)) == NULLP)
   {
      if((raCb = rgSCHDbmGetRaCb(cell, ueDelInfo->u.ueDel.crnti)) == NULLP)
      {
         DU_LOG("\nERROR  -->  SCH : RaCb does not exist for CRNTI:%d",ueDelInfo->u.ueDel.crnti);
         return RFAILED;
      }
      else
      {
         /* This happens in case of Msg4 rejection */
         raCb->toDel = TRUE;
         return ROK;
      }
   }
   else
   {
#ifdef LTE_ADV
      if(ueDelInfo->u.ueScellRel.ueDelTypes & RGR_UE_SCELL_DEL_RECFG)
      {
         for(uint8_t idx = 0; idx < ueDelInfo->u.ueScellRel.ueSCellRelCfgInfo.numSCells; idx++)
         {
            if(NULLP != (secCellCb = (RgSchCellCb *)rgSchUtlGetCellCb(inst, \
                        ueDelInfo->u.ueScellRel.ueSCellRelCfgInfo.ueSCellRelDedCfg[idx].sCellId)))
            {
               rgSCHUtlSndUeSCellDel2Mac(secCellCb, ue->ueId);
               rgSCHSCellDelUeSCell(cell,ue,ueDelInfo->u.ueScellRel.ueSCellRelCfgInfo.ueSCellRelDedCfg[idx].sCellIdx);
               ue->numSCells--;
               if ( ue->numSCells == 0)
               {
                  ue->allocCmnUlPdcch = TRUE;
               }
            }
         }
         if (ue->numSCells == 0)
         {
            /* As there is no SCell left so DCI 0 size at UE specific search space 
             * will be recalculated as the CSI is reduced to 1 bit */
            rgSCHUtlUpdUeDciSize(cell, ue, FALSE);
         }
      }
      else
#endif
      {
         /* Delete Ue from the UE list of CELL*/
         rgSCHDbmDelUeCb(cell, ue);

#ifdef LTE_L2_MEAS
         rgSCHDbmDelL2MUe(cell, ue);
#endif

         /* Call MeasGap and AckNakRep processing module */
         rgSCHMeasGapANRepUeDel(cell, ue, TRUE);

         /* ccpu00140894- Stop TXMode transiition timer if it is running*/
         if (ue->txModeTransTmr.tmrEvnt != TMR_NONE)
         {
            rgSCHTmrStopTmr(cell, RG_SCH_TMR_TXMODE_TRNSTN, ue);
         }

         /* Call DRX module to remove UEs from various
          * lists it maintain 
          */
         /* ccpu00129899 */
         if(ue->drxCb != NULLP)
         {
            (Void)rgSCHDrxUeDel(cell,ue);
            /* Free Ue */
         }
         /*Fix: If RA CB exists, delete it*/
         if((raCb = rgSCHDbmGetRaCb(cell, ueDelInfo->u.ueDel.crnti)) != NULLP)
         {
            /* Fix : syed RNTI was getting released twice, once by racb del 
             * and subsequently by ueDel. Let it get released by ueDel alone */       
            rgSCHRamDelRaCb(cell, raCb, FALSE);
         }
#ifdef EMTC_ENABLE
         if(ue->isEmtcUe)
         {
            rgSCHEmtcUeDel(cell, ue);
         }
#endif

         rgSCHCfgFreeUeCb(cell, ue);

         errInfo->errCause = RGSCHERR_NONE;

      }
      return ROK;
   }
}  /* rgSCHCfgRgrUeDel */


/**
 * @brief Handler for the logical channel delete request from
 * RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrLcDel
 *
 *     Processing Steps:
 *      - Fetch the logical channel control block.
 *      - Free the logical channel control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  RgrDel      *lcDelInfo
 *  @param[out] RgSchErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgRgrLcDel
(
RgSchCellCb    *cell,
RgrDel         *lcDelInfo,
RgSchErrInfo   *errInfo
)
{
   RgSchUeCb    *ue;
   RgSchDlLcCb  *dlLc;
#ifdef LTE_L2_MEAS
   uint8_t          lcId;
   uint8_t          idx;
   RgSchUlLcCb  *ulLc; 
#endif

   errInfo->errCause = RGSCHERR_CFG_RGR_LC_DEL;

   /* Fetch the Active cell */
   if (cell->cellId != lcDelInfo->u.lchDel.cellId)
   {
      DU_LOG("\nERROR  -->  SCH : Cell does not exist %d",
                  lcDelInfo->u.lchDel.cellId);
      return RFAILED;
   }

   /* Fetch the Ue */
   if ((ue = rgSCHDbmGetUeCb(cell, lcDelInfo->u.lchDel.crnti)) == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : UE does not exist for CRNTI:%d LCID:%d",
               lcDelInfo->u.lchDel.crnti,lcDelInfo->u.lchDel.lcId);
      return RFAILED;
   }
   if (lcDelInfo->u.lchDel.lcgId > 3)
   {
      DU_LOG("\nERROR  -->  SCH : [%d]UEID:For LC %d, LCGid %d is invalid",
               lcDelInfo->u.lchDel.crnti,lcDelInfo->u.lchDel.lcId,
               lcDelInfo->u.lchDel.lcgId);
      return RFAILED;
   }
   if ((dlLc = rgSCHDbmGetDlDedLcCb(ue, lcDelInfo->u.lchDel.lcId))
         == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : LC does not exist for CRNTI:%d LCID:%d",
               lcDelInfo->u.lchDel.crnti, lcDelInfo->u.lchDel.lcId);
      return RFAILED;
   }
   rgSCHUtlRgrLcDel(cell, ue, lcDelInfo->u.lchDel.lcId,lcDelInfo->u.lchDel.lcgId); 

   /* Reduce any pending bo from this LC(if any) 
    * from the UE's total BO */
   if(dlLc->bo)
   {
      if(ue->totalBo >= dlLc->bo)
      {
         ue->totalBo -= dlLc->bo;
      }
      else
      {
        ue->totalBo = 0; /* this scenario should not occur */
      }
   }
   rgSCHDbmDelDlDedLcCb(ue, dlLc);
   rgSCHCfgFreeDlDedLcCb(cell, ue, dlLc);

#ifdef LTE_L2_MEAS
   lcId = lcDelInfo->u.lchDel.lcId;
   if (TRUE == ue->ul.lcCb[lcId -1].isValid)
   {
      ulLc = &(ue->ul.lcCb[lcId -1]);
      ue->ul.lcCb[lcId -1].isValid = FALSE;

      if((ulLc->qciCb->ulUeCount) &&
            (ue->ulActiveLCs & (1 << (ulLc->qciCb->qci -1))))
      {
         ulLc->qciCb->ulUeCount--;
         ue->ulActiveLCs &= ~(1 << (ulLc->qciCb->qci -1));
      }
      /* Shifting LCs in LCG Array because of LC deletion */
      for (idx = ulLc->lcgArrIdx +1; idx < ulLc->lcg->numLch;
               idx++)
      {
         ulLc->lcg->lcArray[idx -1] =
            ulLc->lcg->lcArray[idx];
         ulLc->lcg->lcArray[idx -1]->lcgArrIdx = idx -1;
      }
      ulLc->lcg->numLch--;
      ulLc->lcg->lcArray[idx -1] = NULLP;
   }
#endif /* LTE_L2_MEAS */


   errInfo->errCause = RGSCHERR_NONE;
   return ROK;
}  /* rgSCHCfgRgrLcDel */



/**
 * @brief Handler for the logical channel delete request from
 * RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrLcgDel
 *
 *     Processing Steps:
 *      - Fetch the logical channel control block.
 *      - Free the logical channel control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  RgrDel      *lcDelInfo
 *  @param[out] RgSchErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgRgrLcgDel
(
RgSchCellCb    *cell,
RgrDel         *lcDelInfo,
RgSchErrInfo   *errInfo
)
{
   RgSchUeCb    *ue = NULLP;
#ifdef LTE_L2_MEAS
   uint8_t           lcCount = 0;
#endif
   uint8_t           lcgId = 0;

   lcgId = lcDelInfo->u.lcgDel.lcgId;

   errInfo->errCause = RGSCHERR_CFG_RGR_LCG_DEL;

   /* Fetch the Active cell */
   if (cell->cellId != lcDelInfo->u.lcgDel.cellId)
   {
      DU_LOG("\nERROR  -->  SCH : CELL does not exist for CRNTI:%d LCGID:%d",
               lcDelInfo->u.lchDel.crnti,lcDelInfo->u.lchDel.lcId);
      return RFAILED;
   }

   /* Fetch the Ue */
   if ((ue = rgSCHDbmGetUeCb(cell, lcDelInfo->u.lcgDel.crnti)) == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : UE does not exist for CRNTI:%d LCGID:%d",
               lcDelInfo->u.lchDel.crnti,lcDelInfo->u.lchDel.lcId);
      return RFAILED;
   }

   /* set lcgId in UEs lcg cntrl blk to invalid */
   rgSCHUtlRgrLcgDel(cell, ue, lcgId);
   ue->ul.lcgArr[lcgId].lcgId = RGSCH_INVALID_LCG_ID;

#ifdef LTE_L2_MEAS
   /* Since LCs are being deleted, if any of them are contributing
      to Active UE count for a QCI, decrease the count */
   for (lcCount =0; (lcCount < RGSCH_MAX_LC_PER_UE) && 
         (lcCount < ue->ul.lcgArr[lcgId].numLch) ; lcCount++)
   {
      if (ue->ul.lcgArr[lcgId].lcArray[lcCount])
      {
         if((ue->ul.lcgArr[lcgId].
                  lcArray[lcCount]->qciCb->ulUeCount) &&
            (ue->ulActiveLCs &
             (1 << ((ue->ul.lcgArr[lcgId].
                    lcArray[lcCount])->qciCb->qci -1))))
            {
               /* L2_COUNTERS */
               ue->ul.lcgArr[lcgId].
                  lcArray[lcCount]->qciCb->ulUeCount--;
               ue->ulActiveLCs &= ~(1 << 
                     (ue->ul.lcgArr[lcgId].
                      lcArray[lcCount]->qciCb->qci -1));
            }
      }
   }
#endif
   
   errInfo->errCause = RGSCHERR_NONE;
   return ROK;
}  /* rgSCHCfgRgrLcgDel */



/***********************************************************
 *
 *     Func : rgSCHCfgVldtRgrLcCfg
 *
 *
 *     Desc : Validates dedicated logical channel configuration recieved from RRM.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
S16 rgSCHCfgVldtRgrLcCfg
(
Inst             inst,
RgrLchCfg     *lcCfg,
RgSchCellCb      **cell,
RgSchUeCb        **ue,
RgSchErrInfo     *errInfo
)
{

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_DED_LC_CFG;

   if (((*cell) == NULLP) ||
       ((*cell)->cellId != lcCfg->cellId))
   {
      DU_LOG("\nERROR  -->  SCH : Cell does not existi for "
               "CRNTI:%d LCID:%d",lcCfg->crnti, lcCfg->lcId);
      return RFAILED;
   }

   /* Fetch the Ue */
   if ((*ue = rgSCHDbmGetUeCb(*cell, lcCfg->crnti)) == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : UE does not exist for dedicated"
         " logical channel CRNTI:%d LCID:%d", lcCfg->crnti, lcCfg->lcId);
      return RFAILED;
   }

   /* Validate logical channel Id */
   if ((lcCfg->lcId < RGSCH_DEDLC_MIN_LCID)
            ||(lcCfg->lcId > RGSCH_DEDLC_MAX_LCID))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid logical channel Id:%d"
               "for CRNTI:%d",lcCfg->lcId,lcCfg->crnti);
      return RFAILED;
   }

   if (lcCfg->lcType != CM_LTE_LCH_DTCH && lcCfg->lcType != CM_LTE_LCH_DCCH)
   {
      DU_LOG("\nERROR  -->  SCH : Invalid logical channel Type %d"
               "CRNTI:%d LCID:%d",lcCfg->lcType,lcCfg->crnti, lcCfg->lcId);
      return RFAILED;
   }

   return ROK;
}  /* rgSCHCfgVldtRgrLcCfg */

/***********************************************************
 *
 *     Func : rgSCHCfgVldtRgrLcgCfg
 *
 *
 *     Desc : Validates dedicated logical channel group configuration recieved from RRM.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
S16 rgSCHCfgVldtRgrLcgCfg
(
Inst             inst,
RgrLcgCfg        *lcgCfg,
RgSchCellCb      **cell,
RgSchUeCb        **ue,
RgSchErrInfo     *errInfo
)
{

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_DED_LCG_CFG;

   if (((*cell) == NULLP) ||
       ((*cell)->cellId != lcgCfg->cellId))
   {
      DU_LOG("\nERROR  -->  SCH : Cell does not exist for"
               "CRNTI:%d LCGID:%d",lcgCfg->crnti,lcgCfg->ulInfo.lcgId);
      return RFAILED;
   }

   /* Fetch the Ue */
   if ((*ue = rgSCHDbmGetUeCb(*cell, lcgCfg->crnti)) == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : UE does not exist for "
        "dedicated logical channel CRNTI:%d LCGID:%d", lcgCfg->crnti, lcgCfg->ulInfo.lcgId);
      return RFAILED;
   }
   
   if ((lcgCfg->ulInfo.gbr != 0) && (lcgCfg->ulInfo.mbr < lcgCfg->ulInfo.gbr))
   {
     return RFAILED;
   }
   
   return ROK;
}  /* rgSCHCfgVldtRgrLcgCfg */


/***********************************************************
 *
 *     Func : rgSCHCfgVldtRgrCellPwrCfg
 *
 *     Desc : Validates cell power configuration.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgVldtRgrCellPwrCfg
(
Inst             inst,
RgrCellCfg       *cellCfg,
RgSchErrInfo     *errInfo
)
{
   UNUSED(inst);
   UNUSED(cellCfg);
   UNUSED(errInfo);

   /* This function does nothing now, placeholder for
    * subsequent power config validations that may be needed */


   return ROK;
}  /* rgSCHCfgVldtRgrCellPwrCfg */


/***********************************************************
 *
 *     Func : rgSCHCfgVldtRgrCmnLcCfg
 *
 *
 *     Desc : Validates common logical channel configuration recieved from RRM.
 *
 *  @param[in]  Inst         inst
 *  @param[in]  RgrCellCfg   *cellCfg
 *  @param[out] RgSchErrInfo *errInfo
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgVldtRgrCmnLcCfg
(
Inst             inst,
RgrCellCfg       *cellCfg,
RgSchErrInfo     *errInfo
)
{
   uint8_t            idx;
   RgrCmnLchCfg  *lcCfg; 
   uint8_t            dirVld  = FALSE;
   uint8_t            bitMask = 0x00;
   uint8_t            cnt=0;

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_CMN_LC_CFG;
   
   for (idx = 0; idx < cellCfg->numCmnLcs; idx++)
   {
      lcCfg = &(cellCfg->cmnLcCfg[idx]);
      /* Validate downlink info */
      if (lcCfg->dir & RGR_DIR_TX)
      {
         if (lcCfg->lcType == CM_LTE_LCH_BCCH)
         {
            if (lcCfg->dlTrchType == CM_LTE_TRCH_DL_SCH)
            {
               if(cnt == 0)
               {
                  bitMask |= RGSCH_BCCH_DLSCH_CFG1;
                  cnt++;
               }
               else
               {

                  if((
                     (cellCfg->siCfg.siWinSize == 1)  ||
                     (cellCfg->siCfg.siWinSize == 2)  ||
                     (cellCfg->siCfg.siWinSize == 5)  ||
                     (cellCfg->siCfg.siWinSize == 10) ||
                     (cellCfg->siCfg.siWinSize == 15) ||
                     (cellCfg->siCfg.siWinSize == 20) ||
                     (cellCfg->siCfg.siWinSize == 40)) &&
                     (cellCfg->siCfg.retxCnt>0)
                    )
                  {
                     bitMask |= RGSCH_BCCH_DLSCH_CFG2;
                  }
                  else
                  {
                     DU_LOG("\nERROR  -->  SCH : Invalid si config for cell");
                     return RFAILED;
                  }
               }
            }
            else if (lcCfg->dlTrchType == CM_LTE_TRCH_BCH)
            {
               bitMask |= RGSCH_BCCH_BCH_CFG;
            }
            else
            {
               DU_LOG("\nERROR  -->  SCH : Invalid transport channel %d for cell", lcCfg->dlTrchType);
               return RFAILED;
            }
         }
         else if (lcCfg->lcType == CM_LTE_LCH_PCCH)
         {
            bitMask |= RGSCH_PCCH_CFG;
         }
         else if (lcCfg->lcType == CM_LTE_LCH_CCCH)
         {
            bitMask |= RGSCH_DL_CCCH_CFG;
         }
         dirVld = TRUE;
      }

      /* Validate uplink info */
      if (lcCfg->dir & RGR_DIR_RX)
      {
         /* Uplink CCCH */
         if (lcCfg->lcType != CM_LTE_LCH_CCCH)
         {
            DU_LOG("\nERROR  -->  SCH : Invalid UL common lcType %d for cell", lcCfg->lcType);
            return RFAILED;
         }
         else
         {
            bitMask |= RGSCH_UL_CCCH_CFG;
         }
         dirVld = TRUE;
      }

      /* Invalid direction */
      if (!dirVld)
      {
         DU_LOG("\nERROR  -->  SCH : Invalid Direction %d", lcCfg->dir);
         return RFAILED;
      }
   }
   if (bitMask != RGSCH_CELL_ACTIVE_CFG)
   {
      DU_LOG("\nERROR  -->  SCH : Invalid Common channel config for cell");
      return RFAILED;
   }

   return ROK;
}  /* rgSCHCfgVldtRgrCmnLcCfg */


/***********************************************************
 *
 *     Func : rgSCHCfgVldtUeCqiModeCfg
 *
 *
 *     Desc : Validates UE CQI modes Configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgVldtUeCqiModeCfg
(
RgSchCellCb       *cell,
RgrUeDlCqiCfg     *ueDlCqiCfg
)
{
   
#ifndef TFU_UPGRADE
   if((ueDlCqiCfg->prdCqiCfg.cqiPmiCfgIdx < 1) || 
         (ueDlCqiCfg->prdCqiCfg.cqiPmiCfgIdx > 1024))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid Periodic CQI Info");
      return RFAILED;
   }
#endif
   /* Validate UE Aperiodic CQI mode */
   if ((ueDlCqiCfg->aprdCqiCfg.pres == TRUE) &&
       ((ueDlCqiCfg->aprdCqiCfg.aprdModeEnum > RGR_APRD_CQI_MOD31) ||
        (cell->bwCfg.dlTotalBw <= 7)))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid Aperiodic mode config for DL CQI");
      return RFAILED;
   }
#ifndef TFU_UPGRADE
   /* Validate UE Periodic CQI mode */ 
   if (ueDlCqiCfg->prdCqiCfg.prdModeEnum > RGR_PRD_CQI_MOD21)
   {
      DU_LOG("\nERROR  -->  SCH : Invalid periodic mode config for DL CQI");
      return RFAILED;
   }
   /* Validate K value in periodic CQI Config */
   if(((ueDlCqiCfg->prdCqiCfg.prdModeEnum == RGR_PRD_CQI_MOD20) ||
            (ueDlCqiCfg->prdCqiCfg.prdModeEnum == RGR_PRD_CQI_MOD21)) && 
         ((ueDlCqiCfg->prdCqiCfg.k < 1)||
            (ueDlCqiCfg->prdCqiCfg.k > 4)))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid K for Subband CQI reporting");
      return RFAILED;
   }
#else
   if ((ueDlCqiCfg->prdCqiCfg.type == 1) &&
       (ueDlCqiCfg->prdCqiCfg.cqiSetup.prdModeEnum > RGR_PRD_CQI_MOD21))
   {
     DU_LOG("\nERROR  -->  SCH : Invalid periodic mode config for DL CQI");
     return RFAILED;
   }

#endif
   
   return ROK;
      
}
/***********************************************************
 *
 *     Func : rgSCHCfgVldtUeMeasGapAckNakRepCfg
 *
 *
 *     Desc : Validates UE Measurement Gap and Ack Nack Repetition Configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgVldtUeMeasGapAckNakRepCfg
(
RgSchCellCb       *cell,
RgrUeCfg          *ueCfg
)
{

#ifdef LTE_TDD
   if ((ueCfg->ackNackModeEnum == RGR_TDD_ACKNACK_MODE_MULT) &&
         (ueCfg->ueAckNackCfg.isAckNackEnabled == TRUE))
   {
      DU_LOG("\nERROR  -->  SCH : TDD ACK NACK Multiplexing Mode"
      "is not allowed when Ack/Nack is Enabled: %d CRNTI:%d",
       ueCfg->ueAckNackCfg.ackNackRepFactor,ueCfg->crnti);
      return RFAILED;
   }
#endif /* LTE_TDD */
   /* Validate AckNackRep Factor */
   if((ueCfg->ueAckNackCfg.isAckNackEnabled == FALSE) &&
         (!ueCfg->ueMesGapCfg.isMesGapEnabled))
   {
      return ROK;
   }

   if(ueCfg->ueAckNackCfg.isAckNackEnabled)
   {
   if ( (ueCfg->ueAckNackCfg.ackNackRepFactor < RGR_ACKNACK_REPFACT_N2)
         || (ueCfg->ueAckNackCfg.ackNackRepFactor > RGR_ACKNACK_REPFACT_N6))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid ACK NACK REP Factor:%d CRNTI:%d",
               ueCfg->ueAckNackCfg.ackNackRepFactor,ueCfg->crnti);
      return RFAILED;
   }
   }
   if(ueCfg->ueMesGapCfg.isMesGapEnabled)
   {
   switch(ueCfg->ueMesGapCfg.gapPrd)
   {
      case RG_MEAS_GAPPRD_40:
         if(ueCfg->ueMesGapCfg.gapOffst >= RG_MEAS_GAPPRD_40)
         {
            DU_LOG("\nERROR  -->  SCH : Invalid GAP Offset:%d CRNTI:%d",
                     ueCfg->ueMesGapCfg.gapOffst,ueCfg->crnti);
            return RFAILED;
         }
         break;
      case RG_MEAS_GAPPRD_80:
         if(ueCfg->ueMesGapCfg.gapOffst >= RG_MEAS_GAPPRD_80)
         {
            DU_LOG("\nERROR  -->  SCH : Invalid GAP Offset:%d CRNTI:%d",
                     ueCfg->ueMesGapCfg.gapOffst,ueCfg->crnti);
            return RFAILED;
         }
         break;
         default:
         { 
            DU_LOG("\nERROR  -->  SCH : Invalid GAP Periodicity Settings:%d"
                      "CRNTI:%d", ueCfg->ueMesGapCfg.gapPrd,ueCfg->crnti);
            return RFAILED;
         }
      }
   }

   return ROK;
}  /* rgSCHCfgVldtUeMeasGapAckNakRepCfg*/


/***********************************************************
 *
 *     Func : rgSCHCfgVldtUeMeasGapAckNakRepRecfg
 *
 *
 *     Desc : Validates UE Measurement Gap and Ack Nack Repetition Configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgVldtUeMeasGapAckNakRepRecfg
(
RgSchCellCb       *cell,
RgrUeRecfg        *ueRecfg
)
{

   if((ueRecfg->ueAckNackRecfg.isAckNackEnabled == FALSE) &&
          (!ueRecfg->ueMeasGapRecfg.isMesGapEnabled))
   {
      return ROK;
   }

   if(ueRecfg->ueAckNackRecfg.isAckNackEnabled )
   {
   /* Validate AckNackRep Factor */
   if ( (ueRecfg->ueAckNackRecfg.ackNackRepFactor < RGR_ACKNACK_REPFACT_N2)
         || (ueRecfg->ueAckNackRecfg.ackNackRepFactor > RGR_ACKNACK_REPFACT_N6))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid ACK NACK REP Factor:%d"
               "NEW CRNTI:%d",ueRecfg->ueAckNackRecfg.ackNackRepFactor,ueRecfg->newCrnti);
      return RFAILED;
   }
   }
   if(ueRecfg->ueMeasGapRecfg.isMesGapEnabled)
   {
   switch(ueRecfg->ueMeasGapRecfg.gapPrd)
   {
      case RG_MEAS_GAPPRD_40:
         if(ueRecfg->ueMeasGapRecfg.gapOffst >= RG_MEAS_GAPPRD_40)
         {
            DU_LOG("\nERROR  -->  SCH : Invalid GAP Offset:%d"
                     "NEW CRNTI:%d",ueRecfg->ueMeasGapRecfg.gapOffst,ueRecfg->newCrnti);
            return RFAILED;
         }
         break;
      case RG_MEAS_GAPPRD_80:
         if(ueRecfg->ueMeasGapRecfg.gapOffst >= RG_MEAS_GAPPRD_80)
         {
            DU_LOG("\nERROR  -->  SCH : Invalid GAP Offset:%d"
                     "NEW CRNTI:%d",ueRecfg->ueMeasGapRecfg.gapOffst,ueRecfg->newCrnti);
            return RFAILED;
         }
         break;
         default:
         { 
            DU_LOG("\nERROR  -->  SCH : Invalid GAP Periodicity Settings:%d"
                     "NEW CRNTI:%d",ueRecfg->ueMeasGapRecfg.gapPrd,ueRecfg->newCrnti);
            return RFAILED;
         }
      }
   }

   return ROK;
}  /* rgSCHCfgVldtUeMeasGapAckNakRepRecfg*/

#ifdef LTEMAC_SPS
/***********************************************************
 *
 *     Func : rgSCHCfgVldtUeDlSpsCfg
 *
 *
 *     Desc : Validates UE's DL SPS configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgVldtUeDlSpsCfg
(
RgSchCellCb       *cell,
RgrUeSpsDlCfg     *dlSpsCfg
)
{

   uint8_t                 idx = 0;

   /* peridicity validation done in SPS module */
   if ((dlSpsCfg->numPucchVal > RG_SCH_MAX_NUM_N1PUCCH_PER_UE) ||
         (dlSpsCfg->numPucchVal == 0))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid number of n1Pucch values"
               " in DL SPS Config");
      return RFAILED;
   }

   for (idx = 0; idx < dlSpsCfg->numPucchVal; ++idx)
   {
      if (dlSpsCfg->n1PucchVal[idx] > RG_SCH_MAX_N1PUCCH_VAL)
      {
#ifdef ALIGN_64BIT
         DU_LOG("\nERROR  -->  SCH : Invalid N1Pucch value"
                  " in DL SPS Config %u", dlSpsCfg->n1PucchVal[idx]);
#else
         DU_LOG("\nERROR  -->  SCH : Invalid N1Pucch value"
                  " in DL SPS Config %lu", dlSpsCfg->n1PucchVal[idx]);
#endif
         return RFAILED;
      }
   }
   /* SPS_TODO: check will change for TDD */
   if ((dlSpsCfg->numSpsHqProc == 0) ||
         (dlSpsCfg->numSpsHqProc > RGSCH_MAX_DL_HQ_PROC))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid number of SPS HARQ procs"
               " in DL SPS Config");
      return RFAILED;
   }

   return ROK;
}  /* rgSCHCfgVldtDlSpsCfg */
#endif /* LTEMAC_SPS */

/***********************************************************
 *
 *     Func : rgSCHCfgVldtUePwrCfg
 *
 *
 *     Desc : Validates UE Group power configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgVldtUePwrCfg
(
RgSchCellCb       *cell,
RgrUeUlPwrCfg     *pwrCfg
)
{

   /* Group power control works only in accumulated mode */
   if (!pwrCfg->isAccumulated)
   {
      /* Fix */
      if (pwrCfg->uePuschPwr.pres)
      {
         DU_LOG("\nERROR  -->  SCH : Accumulation configutation"
              " not in sync with group power configuration");
         return RFAILED;
      }
   }

   if (rgSCHCfgVldtUeGrpPwrCfg(cell, &pwrCfg->uePuschPwr) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Invalid PUSCH Group power"
           " configuration");
      return RFAILED;
   }
   if (rgSCHCfgVldtUeGrpPwrCfg(cell, &pwrCfg->uePucchPwr) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Invalid PUSCH Group power"
           " configuration");
      return RFAILED;
   }

   return ROK;
}  /* rgSCHCfgVldtUePwrCfg */

/***********************************************************
 *
 *     Func : rgSCHCfgVldtUeGrpPwrCfg
 *
 *
 *     Desc : Validates UE Group power configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgVldtUeGrpPwrCfg
(
RgSchCellCb       *cell,
RgrUeGrpPwrCfg *grpPwrCfg
)
{

   if ((grpPwrCfg->pres) &&
       (((grpPwrCfg->tpcRnti > cell->rntiDb.rntiStart) &&
        ((grpPwrCfg->tpcRnti <
                     (cell->rntiDb.rntiStart + cell->rntiDb.maxRntis))))))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid Uplink Group power " 
             "configuration");
      return RFAILED;
   }

   return ROK;
}  /* rgSCHCfgVldtUeGrpPwrCfg */

#ifdef LTEMAC_SPS
/***********************************************************
 *
 *     Func : rgSCHCfgVldtSpsReCfg
 *
 *
 *     Desc : Validates UE SPS and other SPS dependent 
 *     configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgVldtSpsReCfg
(
RgSchCellCb       *cell,
RgSchUeCb         *ue,
RgrUeRecfg        *ueRecfg
)
{

   if ((ueRecfg->ueRecfgTypes & RGR_UE_DLSPS_RECFG) &&
         (ueRecfg->ueSpsRecfg.dlSpsCfg.isDlSpsEnabled)) 
   {
      /* Validating SPS RNTI */ 
      if (((ueRecfg->ueSpsRecfg.spsRnti >=  cell->rntiDb.rntiStart) && 
          (ueRecfg->ueSpsRecfg.spsRnti<=
          (cell->rntiDb.rntiStart+cell->rntiDb.maxRntis)))||
          (ueRecfg->ueSpsRecfg.spsRnti == RGSCH_SI_RNTI) ||
          (ueRecfg->ueSpsRecfg.spsRnti == RGSCH_P_RNTI))
      {
         DU_LOG("\nERROR  -->  SCH : Invalid SPS RNTI "
                  " in DL SPS Recfg OLD CRNTI:%d NEW CCRNTI:%d",
                  ueRecfg->oldCrnti,ueRecfg->newCrnti);
         return RFAILED;
      }
      if (rgSCHCfgVldtUeDlSpsCfg(cell, &ueRecfg->ueSpsRecfg.dlSpsCfg) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : Invalid DL SPS configuration"
                  " for the OLD CRNTI:%d NEW CRNTI:%d",
                  ueRecfg->oldCrnti,ueRecfg->newCrnti);
         return RFAILED;
      }
   }

#if RG_SPS_UNUSED 
   if(ueRecfg->ueSpsRecfg.dlSpsCfg.isDlSpsEnabled)
   {
     if (ueRecfg->ueRecfgTypes & RGR_UE_DRX_RECFG)
     {
     /* ccpu00117035 - MOD - changed instIdx to inst */
     /* ccpu00117035 - MOD - changed ueID to oldCrnti*/
       DU_LOG("\nERROR  -->  SCH :  DRX reconfig not supported DL SPS enabled for OLD CRNTI:%d NEW CRNTI:%d", 
                  ueRecfg->oldCrnti,ueRecfg->newCrnti);
       return RFAILED;
     }
   }
#endif
/* ccpu00117627 - ADD - SPS recfg validation against HDFDD */
#ifdef LTEMAC_HDFDD
   if(ueRecfg->ueSpsRecfg.dlSpsCfg.isDlSpsEnabled)
   {
      if(ue->hdFddEnbld == TRUE)
      {
         DU_LOG("\nERROR  -->  SCH : DL SPS is not supported for HDFDD enabled for OLD CRNTI:%d NEW CRNTI:%d",
                  ueRecfg->oldCrnti,ueRecfg->newCrnti);
         return RFAILED;
      }
   }
   if(ueRecfg->ueSpsRecfg.ulSpsCfg.isUlSpsEnabled)
   {
      if(ue->hdFddEnbld == TRUE)
      {
         DU_LOG("\nERROR  -->  SCH : UL SPS is not supported for HDFDD enabled for OLD CRNTI:%d NEW CRNTI:%d",
                  ueRecfg->oldCrnti,ueRecfg->newCrnti);
         return RFAILED;
      }
   }
#endif

   return ROK;
}  /*rgSCHCfgVldtSpsReCfg*/
#endif

#if ((defined (RGR_CQI_REPT)) && (defined (RGR_V2)))
/***********************************************************
 *
 *     Func : rgSCHCfgVldtCqiReptReCfg
 *
 *
 *     Desc : Validates UE CQI report for DL Power control
 *     configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgVldtCqiReptReCfg
(
RgSchCellCb       *cell,
RgrUeRecfg     *ueRecfg
)
{

   /* Validate DL Power Control Config parameters */
   if (ueRecfg->ueCqiReptCfg.numColltdCqiRept > RGR_CQIRPTS_MAXN)
   {
      DU_LOG("\nERROR  -->  SCH : Invalide numColltdCqiRept,"
               "MAX supported %d for OLD CRNTI:%d NEW CRNTI:%d",RGR_CQIRPTS_MAXN,
                  ueRecfg->oldCrnti,ueRecfg->newCrnti);
      return RFAILED;
   }

   return ROK;
}  /*rgSCHCfgVldtCqiReptReCfg*/
#endif

/***********************************************************
 *
 *     Func : rgSCHCfgRgrLcChfg
 *
 *
 *     Desc : Handles dedicated logical channel configuration 
 *     recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
S16 rgSCHCfgRgrLchCfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrLchCfg     *lcCfg,
RgSchErrInfo     *errInfo
)
{
   S16       ret;
   RgSchDlLcCb  *dlLc = NULLP;
   Inst    inst = cell->instIdx;
#ifdef LTE_L2_MEAS
   RgSchUlLcCb  *ulLc; 
#endif

   errInfo->errCause = RGSCHERR_CFG_RGR_DED_LC_CFG;

   /* Allocate the downlink logical channel control block */
   if((ret = rgSCHUtlAllocSBuf(inst, (Data**)&dlLc, 
               sizeof(RgSchDlLcCb))) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation FAILED for "
               "Downlink LCId:%d CRNTI:%d", lcCfg->lcId,lcCfg->crnti);
      return RFAILED;
   }
   if ((uint8_t *)dlLc == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation FAILED for "
               "Downlink LCID:%d CRNTI:%d", lcCfg->lcId,lcCfg->crnti);
      return RFAILED;
   }
   dlLc->lcId   = lcCfg->lcId;
#ifdef LTE_ADV
   rgSCHLaaLcCfg(cell, dlLc, lcCfg);
#endif

   rgSCHDbmInsDlDedLcCb(ue, dlLc);
   
   ret = rgSCHUtlRgrLcCfg(cell, ue, dlLc, lcCfg, errInfo);

   if (ret != ROK)
   {
      /* ROLLBACK */
      if (dlLc)
      {
         rgSCHDbmDelDlDedLcCb(ue, dlLc);
         rgSCHCfgFreeDlDedLcCb(cell, ue, dlLc);
      }

      DU_LOG("\nERROR  -->  SCH : Dedicated logical channel "
        "configuration failed at SCH:UEID:%d LCID:%d CRNTI:%d",
        ue->ueId, lcCfg->lcId,lcCfg->crnti);
      return RFAILED;
   }
#ifdef LTE_L2_MEAS
   RGSCH_ARRAY_BOUND_CHECK(inst, ue->ul.lcCb, (lcCfg->lcId -1));
   if ( !lcCfg->lcId || 
         (TRUE == ue->ul.lcCb[lcCfg->lcId -1].isValid))
   {
      /* ROLLBACK */
      if (dlLc)
      {
         rgSCHDbmDelDlDedLcCb(ue, dlLc);
         rgSCHCfgFreeDlDedLcCb(cell, ue, dlLc);
      }

      DU_LOG("\nERROR  -->  SCH : Dedicated logical channel "
        "configuration failed at SCH: UL LC CB already existing"
        " UEID:%d LCID:%d CRNTI:%d",
        ue->ueId, lcCfg->lcId,lcCfg->crnti);
      return RFAILED;
   }

   /* Create UL LC context to maintain LCG to LC mapping and 
      LC and QCI mapping, this is for L2 Counters :UL ACTIVE UE 
      PER QCI */
   ue->ul.lcCb[lcCfg->lcId -1].isValid = TRUE;
   ulLc = &(ue->ul.lcCb[lcCfg->lcId -1]);

   ulLc->lcId = lcCfg->lcId;
   ulLc->qciCb = &(cell->qciArray[lcCfg->dlInfo.dlQos.qci]);
   ulLc->qciCb->qci = lcCfg->dlInfo.dlQos.qci;
   ue->ul.lcgArr[lcCfg->lcgId].lcArray[ue->ul.lcgArr[lcCfg->lcgId].numLch] = ulLc;
   ulLc->lcg = &ue->ul.lcgArr[lcCfg->lcgId];
   ulLc->lcgArrIdx = ue->ul.lcgArr[lcCfg->lcgId].numLch;
   ue->ul.lcgArr[lcCfg->lcgId].numLch++;

   dlLc->qciCb = &(cell->qciArray[lcCfg->dlInfo.dlQos.qci]);
   dlLc->qciCb->qci = lcCfg->dlInfo.dlQos.qci;
   if(lcCfg->lcType == CM_LTE_LCH_DTCH)
   {
     rgSchAddToL2Meas(cell,dlLc); /*LTE_L2_MEAS_PHASE2*/
   }
#endif /* LTE_L2_MEAS */

   return ROK;
}  /* rgSCHCfgRgrLchCfg */

/***********************************************************
 *
 *     Func : rgSCHCfgRgrLcgCfg
 *
 *
 *     Desc : Handles dedicated logical channel group configuration
 *     recieved from RRM.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
S16 rgSCHCfgRgrLcgCfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrLcgCfg        *lcgCfg,
RgSchErrInfo     *errInfo
)
{
   S16          ret = ROK;

#ifdef RG_UNUSED
//#ifdef LTE_L2_MEAS
   uint32_t          idx;
   RgSchUlLcCb  *ulLc; 
#endif

   errInfo->errCause = RGSCHERR_CFG_RGR_DED_LCG_CFG;

   ue->ul.lcgArr[lcgCfg->ulInfo.lcgId].lcgId = lcgCfg->ulInfo.lcgId;

   ret = rgSCHUtlRgrLcgCfg(cell, ue, lcgCfg, errInfo);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Dedicated logical channel "
        "configuration failed at SCH: UEID:%d LCGID:%d CRNTI:%d",
        ue->ueId, lcgCfg->ulInfo.lcgId,lcgCfg->crnti);
      /* Roll back lcgCfg */
      ue->ul.lcgArr[lcgCfg->ulInfo.lcgId].lcgId = RGSCH_INVALID_LCG_ID;
      rgSCHUtlRgrLcgDel(cell, ue, lcgCfg->ulInfo.lcgId);
      return RFAILED;
   }
#ifdef RG_UNUSED
//#ifdef LTE_L2_MEAS
   /* Copy all info of UL LCH in cfg to ulLcgCb */
   for (idx = 0; idx < lcgCfg->ulInfo.numLch; idx++)
   {
      /* Allocate the uplink logical channel control block */
      if((ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data**)&ulLc, 
                  sizeof(RgSchUlLcCb))) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : Memory allocation FAILED ");
         return RFAILED;
      }
      if ((uint8_t *)ulLc == NULLP)
      {
         DU_LOG("\nERROR  -->  SCH : Memory allocation FAILED ");
         return RFAILED;
      }
      /* Create UL LC context to maintain LCG to LC mapping and 
         LC and QCI mapping, this is for L2 Counters :UL ACTIVE UE 
         PER QCI */
      ulLc->lcId = lcgCfg->ulInfo.lchUlCfg[idx].lcId;
      ulLc->qciCb = &(cell->qciArray[lcgCfg->ulInfo.lchUlCfg[idx].qci]);
      ulLc->qciCb->qci = lcgCfg->ulInfo.lchUlCfg[idx].qci;
      ue->ul.lcgArr[lcgCfg->ulInfo.lcgId].lcArray[idx] = ulLc;
      /* L2_COUNTERS */
      ue->ul.lcCb[ulLc->lcId -1] = ulLc;
      ulLc->lcg = &ue->ul.lcgArr[lcgCfg->ulInfo.lcgId];
      ulLc->lcgArrIdx = idx;
   }
   ue->ul.lcgArr[lcgCfg->ulInfo.lcgId].numLch = lcgCfg->ulInfo.numLch;
#endif /* LTE_L2_MEAS */

   return ROK;
}  /* rgSCHCfgRgrLcgCfg */



/***********************************************************
 *
 *     Func : rgSCHCfgRgrCmnLcCfg
 *
 *
 *     Desc : Handles dedicated logical channel configuration 
 *     recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgRgrCmnLcCfg
(
RgSchCellCb           *cell,
RgrCmnLchCfg          *lcCfg,
RgSchErrInfo          *errInfo
)
{
   RgSchClcDlLcCb cmnLcCb;

   errInfo->errCause = RGSCHERR_CFG_RGR_CMN_LC_CFG;

   memset(&cmnLcCb, 0, sizeof(cmnLcCb));

   /* Handle configuration for CCCH/BCCH/PCCH */
   if (lcCfg->lcType == CM_LTE_LCH_CCCH)
   {
      /* UL and DL CCCH configuration */
      if (lcCfg->dir & RGR_DIR_TX)
      {
         cell->dlCcchId = lcCfg->lcId;
      }

      if (lcCfg->dir & RGR_DIR_RX)
      {
         cell->ulCcchId = lcCfg->lcId;
      }
   }
   else
   {
      cmnLcCb.lcId = lcCfg->lcId;
      rgSCHDbmInitCmnLcBoLst(&cmnLcCb);
      if (lcCfg->lcType == CM_LTE_LCH_BCCH)
      {
         /* BCCH on BCH and DLSCH configuration */
         if (lcCfg->dlTrchType == CM_LTE_TRCH_DL_SCH)
         {
            rgSCHDbmInsBcchOnDlsch(cell, &cmnLcCb);
         }
         else
         {
            rgSCHDbmInsBcchOnBch(cell, &cmnLcCb);
         }
      }
      else  /* PCCH configuration */
      {
         rgSCHDbmInsPcch(cell, &cmnLcCb);
      }
   }

   return ROK;
}  /* rgSCHCfgRgrCmnLcCfg */



/***********************************************************
 *
 *     Func : rgSCHCfgFreeDlDedLcCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees downlink dedicated logical channel control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHCfgFreeDlDedLcCb
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchDlLcCb      *dlLc
)
{
   Inst    inst = cell->instIdx;

   rgSCHUtlFreeDlLc(cell, ue, dlLc);

   /* De-allocate the Cb */
   /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(inst, (Data **)&dlLc, sizeof(*dlLc));


  /* Stack Crash problem for TRACE5 changes. Added the return below */
  return;

}  /* rgSCHCfgFreeDlDedLcCb */


/***********************************************************
 *
 *     Func : rgSCHCfgFreeDlCmnLcCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees downlink common logical channel control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHCfgFreeDlCmnLcCb(RgSchClcDlLcCb   *cmnDlLc)
{
 
   memset(cmnDlLc, 0, sizeof(*cmnDlLc));
   cmnDlLc->lcId = RGSCH_INVALID_LC_ID;
   return;
}  /* rgSCHCfgFreeDlCmnLcCb */


/***********************************************************
 *
 *     Func : rgSCHCfgFreeCellCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees scheduler cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
Void rgSCHCfgFreeCellCb(RgSchCellCb      *cell)
{
   Inst    inst = cell->instIdx;
   CmLList            *node;
   Buffer             *pdu;
   RgSchWarningSiInfo *warningSi;
   RgSchWarningSiPdu  *warningSiPdu;
   uint8_t                  idx;
 
   /* ccpu00132385- SI Warning PDUs which are not processed need to be deleted */
   /* Search for used index in WarningSi */
   for(idx = 0; idx < RGR_MAX_NUM_WARNING_SI; idx++)
   {
      if(cell->siCb.warningSi[idx].siId == 0)
        continue;
      cell->siCb.siCtx.siId = cell->siCb.warningSi[idx].siId;
      warningSi = (RgSchWarningSiInfo *) cell->siCb.
                  siArray[cell->siCb.siCtx.siId-1].si; 
      if(warningSi != NULLP)
      {
         /* ccpu00136659: CMAS ETWS design change */
         while (CM_LLIST_FIRST_NODE(&warningSi->warningSiMsg.segLstCp, node))
         {
            warningSiPdu = (RgSchWarningSiPdu *)node->node;
            pdu = warningSiPdu->pdu;
            /* ccpu00136659: CMAS ETWS design change */
            cmLListDelFrm(&warningSi->warningSiMsg.segLstCp, node);
            RGSCH_FREE_MSG(pdu);
         }
         cell->siCb.siArray[cell->siCb.siCtx.siId-1].si = NULLP;
      }
   }
   /* Free lists of the cell */
   rgSCHCfgFreeUeLst(cell);
#ifdef LTEMAC_SPS
   rgSCHCfgFreeSpsUeLst(cell);
#endif /* LTEMAC_SPS */
#ifdef EMTC_ENABLE
      if ( TRUE == cell->emtcEnable )
      {
         rgSCHEmtcCellDel(cell);
      }
#endif
   rgSCHRamFreeCell(cell);

   rgSCHDbmRntiDbDeInit(cell);
   /* Deallocate the subframe allocation information */
   rgSCHUtlPutSfAlloc(cell);
   rgSCHUtlFreeCell(cell);

   rgSCHCfgFreeRgrCfgLst(cell);
   rgSCHCfgFreeCmnLcLst(cell);

   rgSCHUtlPutRlsHqAlloc(cell);

#ifdef LTE_TDD
   rgSCHDbmDeInitUeTfuPendLst(cell);
#endif /* LTE_TDD */

#ifdef RGR_SI_SCH
   rgSCHUtlPutSiInfo(cell);
#endif/*RGR_SI_SCH*/

   (Void)rgSCHDrxCellDel(cell);

   rgSCHUtlFreeSBuf(inst, (Data**)&(cell->dynCfiCb.cceFailSamples),
               (cell->dynCfiCb.numFailSamples * sizeof(uint16_t)));

#ifdef TENB_STATS
   TSL2DeallocCellStatsBlk(cell->cellId); 
#endif

#ifdef LTE_ADV
   /* LAA_SCELL: Trigger the De-Init function for the LAA Module */
   rgSCHLaaSCellCbDeInit(cell);
#endif

#ifdef EMTC_ENABLE
   if(cell->emtcEnable)
   {
      rgSCHEmtcCellFree(cell);
   }
#endif
   /* De-allocate the Cell */
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(inst, (Data **)&cell, sizeof(*cell));

 

  /* Stack Crash problem for TRACE5 changes. Added the return below */
  return;

}  /* rgSCHCfgFreeCellCb */


/***********************************************************
 *
 *     Func : rgSCHCfgFreeUeCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees UE control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHCfgFreeUeCb
(
RgSchCellCb    *cell,
RgSchUeCb      *ue
)
{
   RgSchCmnUlUe *ueUl = RG_SCH_CMN_GET_UL_UE(ue, cell);
   RgUeUlHqCb       *ulHqEnt;
   RgSchDlLcCb       *dlLc;
   Inst    inst = cell->instIdx;
   uint8_t lcCnt; 
   uint8_t lcgId; 

   /* Free all logical channel info per UE */
   while((dlLc = rgSCHDbmGetNextDlDedLcCb(ue, NULLP)) != NULLP)
   {
      rgSCHDbmDelDlDedLcCb(ue, dlLc);
      rgSCHCfgFreeDlDedLcCb(cell, ue, dlLc);
   }
   for (lcCnt =0; lcCnt<RGSCH_MAX_LC_PER_UE; lcCnt++)
   {
      if(ue->ul.lcCb[lcCnt].isValid == TRUE) 
      {
         lcgId = ue->ul.lcCb[lcCnt].lcg->lcgId;
         if (lcgId <=3)
         {
            rgSCHUtlRgrLcDel(cell, ue, ue->ul.lcCb[lcCnt].lcId,lcgId); 
            ue->ul.lcCb[lcCnt].isValid = FALSE;
         }
      }
   }

   ulHqEnt = &(ueUl->hqEnt);
   /* Free Scheduler specific information per UE */
   rgSCHUtlFreeUe(cell, ue);

   /* Free Uplink HARQ specific information per UE */
   rgSCHUhmFreeUe(cell, ulHqEnt);

   if ( ue->drxCb != NULLP)
   {
      /* free drxCb */
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx,
            (Data**)(&(ue->drxCb)),
            sizeof(RgSchDrxUeCb));
   }

   ue->drxCb = (RgSchDrxUeCb *)NULLP;
   /* Free Downlink HARQ specific information per UE */
   rgSCHDhmFreeUe(ue);
   /* Release the RNTI */
   if (ue->rntiLnk)
   {
      rgSCHUtlRlsRnti(cell, ue->rntiLnk, FALSE, 0);
   }
   else
   {
      /* Fix : syed HO UE does not have a valid ue->rntiLnk */
      /* Just indicate to MAC, no need to release at SCH */
      rgSCHUtlIndRntiRls2Mac(cell, ue->ueId, FALSE, 0);
   }
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
    rgSCHCfgPCqiSrsSrUeDel(cell,ue); 
#endif 
#ifdef LTEMAC_HDFDD
    rgSCHHdFddUeDel(cell, ue);
#endif
#ifdef TENB_STATS
    if (ue->tenbStats)
    {
       TSL2DeallocUeStatsBlk(ue->ueId, ue->tenbStats); 
    }
#endif

   /* CA TODO Some handling needed while SCell Delete*/
#ifdef LTE_ADV
   /* Delete the UE from the PCell secCellActCeLst*/
   rgSCHSCellRmvFrmActLst(cell, ue);
   rgSCHSCellDelUe(cell,ue);
#endif
   
#ifdef LTE_ADV
   rgSCHLaaDeInitDlRbAllocCb(cell, &ue->cellInfo[RGSCH_PCELL_INDEX]->dlAllocCb);
#endif

   rgSCHUtlFreeSBuf(inst, (Data **)&ue->cellInfo[0], sizeof(RgSchUeCellInfo));
   /* De-allocate the Ue */
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
#ifdef EMTC_ENABLE
   if(ue->isEmtcUe)
   {
      rgSCHEmtcUeInfoFree(cell, ue);
   }
#endif
   rgSCHUtlFreeSBuf(inst, (Data **)&ue, sizeof(*ue));

  /* Stack Crash problem for TRACE5 changes. Added the return below */
  return;

}  /* rgSCHCfgFreeUeCb */

/***********************************************************
 *
 *     Func : rgSCHCfgFreeRgrCfgLst
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees configuration lists in cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHCfgFreeRgrCfgLst
(
RgSchCellCb      *cell
)
{
   RgSchCfgElem  *rgCfgElem;
   Inst    inst = cell->instIdx;

   /* Free CURRENT RGR cfg list */
   while ((rgCfgElem = rgSCHDbmGetNextCrntRgrCfgElem(cell, NULLP)) != NULLP)
   {
      rgSCHDbmDelCrntRgrCfgElem(cell, rgCfgElem);
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)&rgCfgElem, sizeof(*rgCfgElem));
   }

   /* Free PENDING RGR cfg list */
   while ((rgCfgElem = rgSCHDbmGetNextPndngRgrCfgElem(cell, NULLP)) != NULLP)
   {
      rgSCHDbmDelPndngRgrCfgElem(cell, rgCfgElem);
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)&rgCfgElem, sizeof(*rgCfgElem));
   }


  /* Stack Crash problem for TRACE5 changes. Added the return below */
  return;

}  /* rgSCHCfgFreeRgrCfgLst */


/***********************************************************
 *
 *     Func : rgSCHCfgFreeCmnLcLst
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees common logical channels in cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHCfgFreeCmnLcLst
(
RgSchCellCb      *cell
)
{
   RgSchClcDlLcCb *dlCmnLc;


   if ((dlCmnLc = rgSCHDbmGetBcchOnBch(cell)) != NULLP)
   {
      rgSCHCfgFreeDlCmnLcCb(dlCmnLc);
   }
   if ((dlCmnLc = rgSCHDbmGetFirstBcchOnDlsch(cell)) != NULLP)
   {
      rgSCHCfgFreeDlCmnLcCb(dlCmnLc);
   }
   if ((dlCmnLc = rgSCHDbmGetSecondBcchOnDlsch(cell)) != NULLP)
   {
      rgSCHCfgFreeDlCmnLcCb(dlCmnLc);
   }
   if ((dlCmnLc = rgSCHDbmGetPcch(cell)) != NULLP)
   {
      rgSCHCfgFreeDlCmnLcCb(dlCmnLc);
   }


  /* Stack Crash problem for TRACE5 changes. Added the return below */
  return;

}  /* rgSCHCfgFreeCmnLcLst */


/***********************************************************
 *
 *     Func : rgSCHCfgFreeUeLst
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees UE list in cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHCfgFreeUeLst
(
RgSchCellCb      *cell
)
{
   RgSchUeCb     *ue;
#ifdef LTE_ADV
   RgSchUeCellInfo *sCellInfo;       
   CmLList         *node;
#endif

   /* Free Ues in the list */
   while ((ue = rgSCHDbmGetNextUeCb(cell, NULLP)) != NULLP)
   {
      rgSCHDbmDelUeCb(cell, ue);

#ifdef LTE_ADV
      if(ue->cell != cell)
      {
         continue;
      }
#endif
      
      /* Call MeasGap and AckNakRep processing module */
      rgSCHMeasGapANRepUeDel(cell, ue, TRUE);

      rgSCHCfgFreeUeCb(cell, ue);
   }

   /* De-initialize the Ue list */
   rgSCHDbmDeInitUeCbLst(cell);


#ifdef LTE_ADV
   node = cell->sCellUeLst.first; 
   while(node)
   {
      sCellInfo = (RgSchUeCellInfo *)node->node;
      node = node->next;
      rgSCHSCellDelUeSCell(sCellInfo->ue->cell, sCellInfo->ue, sCellInfo->sCellIdx);
   }   
#endif

  /* Stack Crash problem for TRACE5 changes. Added the return below */
  return;

}  /* rgSCHCfgFreeUeLst */

#ifdef LTEMAC_SPS
/***********************************************************
 *
 *     Func : rgSCHCfgFreeSpsUeLst
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees Sps UE list in cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHCfgFreeSpsUeLst(RgSchCellCb *cell)
{
   RgSchUeCb     *ue;

   /* Free Ues in the list */
   while ((ue = rgSCHDbmGetNextSpsUeCb(cell, NULLP)))
   {
      rgSCHDbmDelSpsUeCb(cell, ue);
   }

   /* De-initialize the Ue list */
   rgSCHDbmDeInitSpsUeCbLst(cell);

}  /* rgSCHCfgFreeSpsUeLst */

#endif /* LTEMAC_SPS */

#ifdef RGR_SI_SCH
/***********************************************************
 *
 *     Func : rgSCHCfgVldtRgrCellSiCfg
 *
 *     Desc : Validates SI Configuration for SI
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgVldtRgrCellSiCfg
(
Inst             inst,
RgrSiCfg         *siCfg
)
{
   uint8_t   idx; /* idx for iteration */

   UNUSED(inst);

#ifndef LTE_TDD
   /* Check that retxCnt value should be <= value of siWinSize.
   This validation is only applicable for FDD mode. */
   if(siCfg->retxCnt > siCfg->siWinSize) 
   {
      DU_LOG("\nERROR  -->  SCH : retxCnt is greater than siWinSize, validation failed");
      return RFAILED;
   }
#endif

   /* Validate that a valid value for numSi has been specified */
   if(siCfg->numSi > RGR_MAX_NUM_SI) 
   {
      DU_LOG("\nERROR  -->  SCH : Validation for numSi in SI CFG failed");
      return RFAILED;
   }

   /* MinPeriodicity will have the least configured periodicity
    * Hence initializing with Max periodicity */
   siCfg->minPeriodicity = RGR_SI_PERD_512;

   /*Validate the value of periodicity specified for SIs */
   for(idx = 0;idx < siCfg->numSi;idx++)
   {
      siCfg->minPeriodicity =     RGSCH_MIN(siCfg->minPeriodicity, 
            siCfg->siPeriodicity[idx]);
         /* Set the siPeriodicity as a multiple of 80 subframes */
         switch(siCfg->siPeriodicity[idx])
         {
            case RGR_SI_PERD_8:
            case RGR_SI_PERD_16:
            case RGR_SI_PERD_32:
            case RGR_SI_PERD_64:
            case RGR_SI_PERD_128:
            case RGR_SI_PERD_256:
            case RGR_SI_PERD_512:
               continue;

         default:
            DU_LOG("\nERROR  -->  SCH : Validation for SI Periodicity in SI-CFG failed");
            return RFAILED;
      }
   }

   return ROK;
}  /* rgSCHCfgVldtRgrCellSiCfg */

/* LTE_ADV_FLAG_REMOVED_START */
/***********************************************************
 *
 *     Func : rgSCHCfgVldtRgrCellLtrAdvCfg
 *
 *     Desc : Validates Lte Adv Configuration
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgVldtRgrCellLteAdvCfg
(
Inst                     inst,
RgrLteAdvancedCellConfig *lteAdvCfg,
uint8_t                       dlTotalBw  
)
{
   uint8_t temp[RGR_ABS_PATTERN_LEN];
   uint32_t idx;
   UNUSED(inst);

   if((lteAdvCfg->pres & RGR_SFR) && (RGR_ENABLE == lteAdvCfg->sfrCfg.status))
   {
      if(lteAdvCfg->sfrCfg.cellEdgeRbRange.startRb > lteAdvCfg->sfrCfg.cellEdgeRbRange.endRb)
      {
         DU_LOG("\nERROR  -->  SCH : Invalid configuration of cell edge bandwidth for SFR feature");
         return RFAILED;
      }

      if(lteAdvCfg->sfrCfg.cellEdgeRbRange.endRb >= dlTotalBw)
      {
        DU_LOG("\nERROR  -->  SCH : Invalid configuration of cell edge end RB for SFR feature");
         return RFAILED;
      }
         
#ifdef TFU_UPGRADE      
      if(lteAdvCfg->sfrCfg.pwrThreshold.pLow >= lteAdvCfg->sfrCfg.pwrThreshold.pHigh)
      {
         DU_LOG("\nERROR  -->  SCH : Invalid configuration of power threshold for SFR feature");
         return RFAILED;
      }
#endif      
   }

   if((lteAdvCfg->pres & RGR_ABS) && (RGR_ENABLE == lteAdvCfg->absCfg.status))
   {
      if((RGR_ABS_MUTE != lteAdvCfg->absCfg.absPatternType) && 
            (RGR_ABS_TRANSMIT != lteAdvCfg->absCfg.absPatternType)) 
      {
         DU_LOG("\nERROR  -->  SCH : Invalid configuration of ABS pattern type");
         return RFAILED;
      }

       memcpy(temp, lteAdvCfg->absCfg.absPattern,RGR_ABS_PATTERN_LEN);
      
       /* Added validation for ABS pattern len */
      for(idx = 0; idx < RGR_ABS_PATTERN_LEN; idx++)
      {   
         if((temp[idx] != 1) && (temp[idx] != 0))
        {
           DU_LOG("\nERROR  -->  SCH : Invalid configuration of ABS pattern type");
           return RFAILED;
        }
      }
   }

   return ROK;
}
/* LTE_ADV_FLAG_REMOVED_END */


/***********************************************************
 *
 *     Func : rgSCHCfgVldtRgrCellCsgParamCfg
 *
 *     Desc : Validates CSG Parameter Configuration
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgVldtRgrCellCsgParamCfg
(
Inst                     inst,
RgrCellCsgParamCfg       *csgParam
)
{

   DU_LOG("\nDEBUG  -->  SCH : Validating CSG Parameters \n");

   if(csgParam->minDlResNonCsg > 100)
   {
      DU_LOG("\nERROR  -->  SCH : Invalid Configuration of minimum DL resources "
            "for NON-CSG");
      return RFAILED;
   }
   if(csgParam->minUlResNonCsg > 100)
   {
      DU_LOG("\nERROR  -->  SCH : Invalid Configuration of minimum UL resources "
            "for NON-CSG");
      return RFAILED;
   }
   return ROK;
}

/**
 * @brief Validates the SI configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrSiCfg
 *
 *     Processing Steps:
 *        - Validate the range of configured values recieved in
 *          configuration request.
 *        - If validated successfully,
 *          - Return ROK
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *  @param[in]  Inst         inst
 *  @param[in]  RgrCellCfg   *siCfg
 *  @param[out] RgSchCellCb  *cell
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgVldtRgrSiCfg
(
Inst         inst,
RgrSiCfgReqInfo *siCfg,
RgSchCellCb      *cell,
RgSchErrInfo *errInfo
)
{
   MsgLen  msgLen = 0;
   uint8_t      numSi;

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_SI_CFG;

   /*Validate the cfgType parameter */
   switch(siCfg->cfgType)
   {
      /*ccpu00140789*/
      case RGR_SI_STOP:
         numSi = (cell->siCb.siBitMask & RGSCH_SI_SICFG_UPD) ?
              cell->siCb.newSiCfg.numSi : cell->siCfg.numSi;
         if((siCfg->siId < RGSCH_SI_SIID_LOWER_LMT) ||
                 (siCfg->siId > numSi))
         {
            DU_LOG("\nERROR  -->  SCH : Invalid SI Id value"
                     " specified");
            return RFAILED;
         }
         errInfo->errCause = RGSCHERR_NONE;
         return ROK;
      break;
      case RGR_SI_CFG_TYPE_MIB:      /* SI CFG Type MIB */
      case RGR_SI_CFG_TYPE_SIB1:     /* SI CFG TYPE SIB1 */
      case RGR_SI_CFG_TYPE_SIB1_PWS: /* SI CFG TYPE SIB1_PWS */
      case RGR_SI_CFG_TYPE_SIB8_CDMA:    /* SI CFG TYPE SIB 8 CDMA */
#ifdef EMTC_ENABLE
      case RGR_SI_CFG_EMTC_TYPE_SIB1_BR:
      case RGR_SI_CFG_EMTC_TYPE_SIB1_BR_PER:
#endif
         break;

      case RGR_SI_CFG_TYPE_SI:    /* SI CFG TYPE SI */
         /*Check that value of SI should be less than equal 
           to configured numSi parameter value */
         /* Added siId validation for lower limit */
         numSi = (cell->siCb.siBitMask & RGSCH_SI_SICFG_UPD) ? 
            cell->siCb.newSiCfg.numSi : cell->siCfg.numSi;
         if((siCfg->siId < RGSCH_SI_SIID_LOWER_LMT) || 
               (siCfg->siId > numSi))
         {
            DU_LOG("\nERROR  -->  SCH : Invalid SI Id value"
                     " specified");
            return RFAILED;
         }

         if(siCfg->siId > ((cell->siCfg.minPeriodicity * 10)/cell->siCfg.siWinSize))
         {
            DU_LOG("\nERROR  -->  SCH : SiId can not be scheduled ");
            return RFAILED;
         }
         break;

#ifdef EMTC_ENABLE
      case RGR_SI_CFG_EMTC_TYPE_SI:
      case RGR_SI_CFG_EMTC_TYPE_SI_PER:
         if(ROK != rgEmtcvalidateSiCfg(siCfg,cell))
         {
            return RFAILED;
         } 
         break;
#endif
      default:
         DU_LOG("\nERROR  -->  SCH : Invalid cfgType "
                      "parameter value");
         return RFAILED;
   }

   /*Validate the specified pdu */
   if(NULLP == siCfg->pdu)
   {
      DU_LOG("\nERROR  -->  SCH : Invalid NULLP pdu "
                   "specified");
      return RFAILED;
   }

   /*Check if PDU is of 0 length*/
   SFndLenMsg(siCfg->pdu, &msgLen);
   if(0 == msgLen)
   {
      DU_LOG("\nERROR  -->  SCH : Invalid pdu "
                   "specified");
      return RFAILED;
   }

   errInfo->errCause = RGSCHERR_NONE;


   return ROK;
}  /* rgSCHCfgVldtRgrSiCfg */
#endif /*RGR_SI_SCH*/

/* LTE_ADV_FLAG_REMOVED_START */
/**
 * @brief Validates the RNTP INF request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrLoadInf
 *
 *     Processing Steps:
 *        - Validate the range of configured values recieved in
 *          LOAD INF request.
 *        - If validated successfully,
 *          - Return ROK
 *        - Else
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *  @param[in]  Inst                inst
 *  @param[in]  RgrLoadInfReqInfo   *loadInfReq
 *  @param[out] RgSchCellCb         *cell
 *  @param[out] RgSchErrInfo        *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHCfgVldtRgrLoadInf
(
Inst              inst,
RgrLoadInfReqInfo *loadInfReq,
RgSchCellCb       *cell,
RgSchErrInfo      *errInfo
)
{

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_LOAD_INF;

   /* Validate if the CC startRb which we have received from DSFR lies in CE sub-band of the receiving eNB */
   if((loadInfReq->rgrCcPHighStartRb >= cell->lteAdvCb.sfrCfg.cellEdgeRbRange.startRb) && 
         (loadInfReq->rgrCcPHighStartRb <= cell->lteAdvCb.sfrCfg.cellEdgeRbRange.endRb))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid rgrCcPHighStartRb received specified");
      return RFAILED;
   }

   /* Validate if the CC endRb which we have received from DSFR lies in CE sub-band of the receiving eNB */
   if((loadInfReq->rgrCcPHighEndRb >= cell->lteAdvCb.sfrCfg.cellEdgeRbRange.startRb) && 
         (loadInfReq->rgrCcPHighEndRb <= cell->lteAdvCb.sfrCfg.cellEdgeRbRange.endRb))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid rgrCcPHighEndRb received specified");
      return RFAILED;
   }

   errInfo->errCause = RGSCHERR_NONE;


   return ROK;
}  /* rgSCHCfgVldtRgrLoadInf */
/* LTE_ADV_FLAG_REMOVED_END */

#ifdef TFU_UPGRADE

/********************************************************************
 *   UE ACQI, PCQI, RI, SRS and SR Re/Configuration  Validation Functions  *
 *                                                                   *
 *********************************************************************/

 /*
 * @brief Validates the Tx Mode and PUSCH Mode configuration.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrTxmodePuschMode  
 *
 *     Processing Steps:
 *      - Validate whether the configured PUSCH Mode and the
 *         Configured Tx Mode are in the right combination
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  RgrTxMode     txMde
 *  @param[in]  RgrAprdCqiMode puschMode
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
S16 rgSCHCfgVldtRgrTxmodePuschMode 
(
RgSchCellCb     *cellCb,
RgrTxMode       txMde,
RgrAprdCqiMode  puschMode,
RgSchErrInfo    *errInfo
)
{

   if (txMde == RGR_UE_TM_1 ||  txMde == RGR_UE_TM_2 ||
         txMde == RGR_UE_TM_3 || txMde == RGR_UE_TM_7)
   {
      if (puschMode == RGR_APRD_CQI_MOD12 ||
            puschMode == RGR_APRD_CQI_MOD22  ||
            puschMode == RGR_APRD_CQI_MOD31)
      {
         return RFAILED;
      }
   }

   if (txMde == RGR_UE_TM_4 ||  txMde == RGR_UE_TM_6)
   {
      if (puschMode == RGR_APRD_CQI_MOD20 ||
            puschMode == RGR_APRD_CQI_MOD30)
      {
         return RFAILED;
      }
   }

   if (txMde == RGR_UE_TM_5 )
   {
      if (puschMode != RGR_APRD_CQI_MOD31)
      {
         return RFAILED;
      }
   }
#ifdef LTE_ADV
   /* TOODO:: Tm8 and TM9 validation has to
    * be changed as mentioned inthe commented 
    * code below*/
   /* TM8 and TM9 supports all modes
    * Mode 1-2, 2-2, 3-1 if pmi/ri reporting enabled
    * 2-0,3-0 of pmi/ri reporitng isdisabled *
    * if pmi/ri is enabled 
    * Mode 1-2, 2-2, 3-1 if with pmi/ri and csi-rs ports > 1
    * 2-0,3-0 of pmi/ri reporitng isdisabled and csi-rs ports == 1*/

#endif
   return ROK;
}/*rgSCHCfgVldtRgrTxmodePuschMode ends*/

 /*
 * @brief Validates the UE ACQI configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUeACqiCfg  
 *
 *     Processing Steps:
 *      - Validate the UE configuration request from RRC to MAC at CFG:
 *        validate the ACQI Configuration 
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in] RgSchCellCb        *cellCb,
 *  @param[in] CmLteRnti          crnti,  
 *  @param[in] RgrUeAprdDlCqiCfg   *acqiCfg,  
 *  @param[in] RgrUeTxModeCfg     txMode,
 *  @param[out] RgSchErrInfo       *errInfo

 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
S16 rgSCHCfgVldtRgrUeACqiCfg 
(
RgSchCellCb        *cellCb,
CmLteRnti          crnti,  
RgrUeAprdDlCqiCfg  *acqiCfg,  
RgrUeTxModeCfg     txMode,
RgSchErrInfo       *errInfo
)
{
   RgrTxMode      txMde;
   RgrAprdCqiMode puschMode; 

   if(acqiCfg->pres)
   {
      if(txMode.pres == TRUE)
      {
         txMde = txMode.txModeEnum;
         puschMode = acqiCfg->aprdModeEnum;
         if ( ROK != rgSCHCfgVldtRgrTxmodePuschMode(cellCb, txMde,
                  puschMode, errInfo))
         {
            DU_LOG("\nERROR  -->  SCH : Invalid Aperiodic CQI configuration CRNTI:%d",crnti);
            return RFAILED;
         } 
      }
   }
   return ROK;
}

 /*
 * @brief Validates the Tx Mode and PUCCH Mode configuration.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrTxmodePucchMode  
 *
 *     Processing Steps:
 *      - Validate whether the configured PUCCH Mode and the
 *         Configured Tx Mode are in the right combination
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  RgrTxMode     txMde
 *  @param[in]  RgrPrdCqiMode     pucchMode
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
S16 rgSCHCfgVldtRgrTxmodePucchMode 
(
RgSchCellCb     *cellCb,
RgrTxMode       txMde,
RgrPrdCqiMode   pucchMode,
RgSchErrInfo    *errInfo
)
{

  if (pucchMode == RGR_PRD_CQI_MOD10 || pucchMode == RGR_PRD_CQI_MOD20 )
  {
   if (txMde ==RGR_UE_TM_4 || txMde ==RGR_UE_TM_5 || txMde ==RGR_UE_TM_6)
   {
      return RFAILED;
   }
  }
  else if (pucchMode == RGR_PRD_CQI_MOD11 || pucchMode == RGR_PRD_CQI_MOD21)
  {
     if (txMde ==RGR_UE_TM_1 || txMde ==RGR_UE_TM_2 || txMde ==RGR_UE_TM_3 \
             || txMde ==RGR_UE_TM_7)
   {
      return RFAILED;
   }
  }
  /* TODO:: Tm8 and TM9 validation needs to be added */
   return ROK;
}

/*
 * @brief Validates the UE Periodic CQI, PMI, RI, re/configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUePCqiCfg  
 *
 *     Processing Steps:
 *      - Validate the UE configuration request from RRC to MAC at CFG:
 *        validate the value range for Periodic CQI, PMI, RI values.
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in] RgSchCellCb        *cellCb,
 *  @param[in] CmLteRnti          crnti,  
 *  @param[in] RgrUePrdDlCqiCfg   *cqiCfg,  
 *  @param[in] RgrUeTxModeCfg     txMode,
 *  @param[out] RgSchErrInfo       *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef LTEMAC_HDFDD
S16 rgSCHCfgVldtRgrUePCqiCfg 
(
RgSchCellCb        *cellCb,
CmLteRnti          crnti,  
RgrUePrdDlCqiCfg   *cqiCfg,
Bool               hdFdd,
RgrUeTxModeCfg     txMode,
RgSchErrInfo       *errInfo
)
#else
S16 rgSCHCfgVldtRgrUePCqiCfg 
(
RgSchCellCb        *cellCb,
CmLteRnti          crnti,  
RgrUePrdDlCqiCfg   *cqiCfg,  
RgrUeTxModeCfg     txMode,
RgSchErrInfo       *errInfo
)
#endif
{
   RgrTxMode     txMde; 
   RgrPrdCqiMode pucchMode; 

   txMde = RGR_UE_TM_1;
   pucchMode = RGR_PRD_CQI_MOD20;
   if ( RGR_SCH_PCQI_SETUP == cqiCfg->type )
   {
      /*1. Validate for Tx Mode and PUCCH Mode combination*/
      if(txMode.pres == TRUE)
      {
         txMde = txMode.txModeEnum;
         pucchMode = cqiCfg->cqiSetup.prdModeEnum;
         if ( ROK != rgSCHCfgVldtRgrTxmodePucchMode(cellCb, txMde,
                  pucchMode, errInfo))
         {
            DU_LOG("\nERROR  -->  SCH : Invalid Tx Mode-PUCCH Mode combination CRNTI:%d",crnti);
            return RFAILED;
         } 
      }

      /*2. Validate for PCQI Reporting Type and PUCCH Mode combination*/
      if((cqiCfg->cqiSetup.cqiRepType==1) && 
              ((pucchMode == RGR_PRD_CQI_MOD20) ||
               (pucchMode == RGR_PRD_CQI_MOD21)))
      {
          DU_LOG("\nERROR  -->  SCH : Invalid Reporting Type-PUCCH Mode combination CRNTI:%d",crnti);
            return RFAILED;
      }

      if((cqiCfg->cqiSetup.cqiRepType==2) && 
              ((pucchMode == RGR_PRD_CQI_MOD10) ||
               (pucchMode == RGR_PRD_CQI_MOD11)))
      {
            DU_LOG("\nERROR  -->  SCH : Invalid Reporting Type-PUCCH Mode combination CRNTI:%d",crnti);
            return RFAILED;
      }

      /*3. Validate CQI/PMI and RI Configuration related parameter values */
      /*TODO- To be compared with configured n2Pucch Index*/
      if (cqiCfg->cqiSetup.cqiPResIdx > RG_SCH_PUCCH_RES_MAX_SUPP ) 
      {
         DU_LOG("\nERROR  -->  SCH : Not Supported or Invalid CQI-PUCCH resourceIndex=%d Cfg Val=%d CRNTI:%d", 
                  RG_SCH_PUCCH_RES_MAX_SUPP, cqiCfg->type,crnti);
         return RFAILED;
      } 

#ifdef LTEMAC_HDFDD
      if(hdFdd)
      {
         if((cqiCfg->cqiSetup.cqiPCfgIdx > RG_SCH_ICQI_MAX_SUPP)||
               (cqiCfg->cqiSetup.cqiPCfgIdx < 7) ||
               (cqiCfg->cqiSetup.cqiPCfgIdx == 317))
         {
            DU_LOG("\nERROR  -->  SCH : Not Supported or Invalid Cfg CQI Min Index Sup =%d"
                     "Max Index Sup=%d  Cfg Val=%d CRNTI:%d", RG_SCH_ICQI_MIN_SUPP, 
                     RG_SCH_ICQI_MAX_SUPP, cqiCfg->type,crnti);
            return RFAILED;
         }
      }
      else 
      {
         if((cqiCfg->cqiSetup.cqiPCfgIdx > RG_SCH_ICQI_MAX_SUPP)||
               (cqiCfg->cqiSetup.cqiPCfgIdx == 317))
         {
            DU_LOG("\nERROR  -->  SCH : Not Supported or Invalid Cfg CQI Min Index Sup =%d "
                     "Max Index Sup=%d  Cfg Val=%d CRNTI:%d", RG_SCH_ICQI_MIN_SUPP, 
                     RG_SCH_ICQI_MAX_SUPP, cqiCfg->type,crnti);
            return RFAILED;
         }
      }
#else
      if((cqiCfg->cqiSetup.cqiPCfgIdx > RG_SCH_ICQI_MAX_SUPP)||
            (cqiCfg->cqiSetup.cqiPCfgIdx == 317))
      {
         DU_LOG("\nERROR  -->  SCH : Not Supported or Invalid Cfg CQI Min Index Sup =%d"
                  "Max Index Sup=%d  Cfg Val=%d CRNTI;%d", RG_SCH_ICQI_MIN_SUPP, 
                  RG_SCH_ICQI_MAX_SUPP, cqiCfg->type,crnti);
         return RFAILED;
      }
#endif

      if((cqiCfg->cqiSetup.cqiRepType < RGR_UE_PCQI_WB_REP) ||
            (cqiCfg->cqiSetup.cqiRepType > RGR_UE_PCQI_SB_REP))
      {
         DU_LOG("\nERROR  -->  SCH : Invalid Cfg CQI Report"
                  "ModeCfg Val=%d CRNTI:%d", cqiCfg->type,crnti);
         return RFAILED;
      }

      if((cqiCfg->cqiSetup.cqiRepType == RGR_UE_PCQI_SB_REP) && 
            ((cqiCfg->cqiSetup.k < RG_SCH_CQI_K_MIN) ||
            (cqiCfg->cqiSetup.k > RG_SCH_CQI_K_MAX)))
      {
         DU_LOG("\nERROR  -->  SCH : Invalid CQI Cfg K Cfg Val=%d CRNTI:%d", cqiCfg->type,crnti);
         return RFAILED;
      }

      if((cqiCfg->cqiSetup.cqiRepType == RGR_UE_PCQI_SB_REP) && 
            (cellCb->bwCfg.dlTotalBw <= 7))
      {
         DU_LOG("\nERROR  -->  SCH : Invalid Periodic CQI mode Cfg for dlTotalBw (%d) for CRNTI:%d", 
                  cellCb->bwCfg.dlTotalBw, crnti);
         return RFAILED;
      }

#ifndef LTE_TDD
      if (cqiCfg->cqiSetup.cqiPCfgIdx == RG_SCH_ICQI_RESV_FDD ) 
      {  
         DU_LOG("\nERROR  -->  SCH : Reserved value Cfg =%d CRNTI:%d", 
                  cqiCfg->cqiSetup.cqiPResIdx,crnti);
         return RFAILED;
      }
#endif

      /* 4. Check RI Configuration values */
       if(cqiCfg->cqiSetup.riEna == TRUE)
      {
         if(txMode.pres == TRUE)
         {
            if((txMde != RGR_UE_TM_3)  
            && (txMde != RGR_UE_TM_4) 
            && (txMde != RGR_UE_TM_8) 
#ifdef LTE_ADV
            && (txMde != RGR_UE_TM_9)
#endif
             )
            {
               DU_LOG("\nERROR  -->  SCH : Invalid Transmission Mode =%d CRNTI:%d",
                  txMde,crnti);
               return RFAILED;
            }
         }
         
         if(cqiCfg->cqiSetup.riCfgIdx > RG_SCH_IRI_MAX_SUPP)
         {
            DU_LOG("\nERROR  -->  SCH : Invalid Index RI value Cfg =%d CRNTI:%d",
                  cqiCfg->cqiSetup.riCfgIdx,crnti);
            return RFAILED;
         }
      }
   }

  
   return ROK;
}

/*
 * @brief Validates the UE SRS Re/Configuation request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUeUlSrsCfg 
 *
 *     Processing Steps:
 *      - Validate the UE configuration request from RRC to MAC at CFG:
 *        validate the value range for SRS values.
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in] RgSchCellCb       *cellCb,
 *  @param[in] CmLteRnti         crnti,  
 *  @param[in] RgrUeUlSrsCfg     *srsCfg,  
 *  @param[out] RgSchErrInfo     *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef LTEMAC_HDFDD
S16 rgSCHCfgVldtRgrUeUlSrsCfg 
(
 RgSchCellCb       *cellCb,
 CmLteRnti         crnti,  
 RgrUeUlSrsCfg     *srsCfg,
 Bool              hdFdd,
 RgSchErrInfo      *errInfo
 )
#else
S16 rgSCHCfgVldtRgrUeUlSrsCfg 
(
RgSchCellCb       *cellCb,
CmLteRnti         crnti,  
RgrUeUlSrsCfg     *srsCfg,  
RgSchErrInfo      *errInfo
)
#endif
{
   uint16_t    srsPeri=0;
   uint16_t    srsOffset=0;
   uint8_t     srsSubframe=0;
   
   if ( RGR_SCH_SRS_SETUP == srsCfg->type )
   {

      /*ccpu00130768 - ADD - if cell specific SRS is not configured*/
      if(cellCb->srsCfg.isSrsCfgPres == FALSE)
      {
         DU_LOG("\nERROR  -->  SCH : Cell specific SRS is not configured CRNTI:%d",crnti);
         return RFAILED;
      }

      /*  1. Validate SRS Configuration related parameter values  */
      /* 1.1 iSRS should be 0-636; Ref: 36.213. Table 8.2-1 */
#ifdef LTEMAC_HDFDD
      if(hdFdd)
      {
         if ( (srsCfg->srsSetup.srsCfgIdx < 7) || 
               (srsCfg->srsSetup.srsCfgIdx > RG_SCH_ISRS_MAX_SUPP) )
         {
            DU_LOG("\nERROR  -->  SCH : Not Supported or Invalid Cfg "
                     "SRS Min Index Sup =%d   Max Index Sup=%d  Cfg Val=%d CRNTI:%d",
                     RG_SCH_ISRS_MIN_SUPP, RG_SCH_ISRS_MAX_SUPP,
                     srsCfg->srsSetup.srsCfgIdx,crnti);
            return RFAILED;
         }
      }
      else
      {
         if ( srsCfg->srsSetup.srsCfgIdx > RG_SCH_ISRS_MAX_SUPP )
         {
            DU_LOG("\nERROR  -->  SCH : Not Supported or Invalid Cfg"
                     "SRS Min Index Sup =%d   Max Index Sup=%d  Cfg Val=%d CRNTI:%d",
                     RG_SCH_ISRS_MIN_SUPP, RG_SCH_ISRS_MAX_SUPP,
                     srsCfg->srsSetup.srsCfgIdx,crnti);
            return RFAILED;
         }

      }
#else
      if ( srsCfg->srsSetup.srsCfgIdx > RG_SCH_ISRS_MAX_SUPP )
      {
         DU_LOG("\nERROR  -->  SCH : Not Supported or Invalid Cfg" 
                  "SRS Min Index Sup =%d   Max Index Sup=%d  Cfg Val=%d CRNTI:%d",
                  RG_SCH_ISRS_MIN_SUPP, RG_SCH_ISRS_MAX_SUPP,
                  srsCfg->srsSetup.srsCfgIdx,crnti);
         return RFAILED;
      }
#endif
#ifdef LTE_TDD
      /* Compute SRS Offset and Periodicity */
      rgSCHUtlGetCfgPerOff( RG_SCH_TDD_SRS_TBL, 
            srsCfg->srsSetup.srsCfgIdx,
            &srsPeri, &srsOffset);  
#else
      rgSCHUtlGetCfgPerOff( RG_SCH_FDD_SRS_TBL, 
            srsCfg->srsSetup.srsCfgIdx,
            &srsPeri, &srsOffset);     
#endif
       srsSubframe = srsOffset%RGSCH_NUM_SUB_FRAMES;
#ifdef LTE_TDD   
      if(rgSchTddCellSpSrsSubfrmTbl[cellCb->srsCfg.srsSubFrameCfg][srsSubframe] == FALSE)   {      
#else   
      if(rgSchFddCellSpSrsSubfrmTbl[cellCb->srsCfg.srsSubFrameCfg][srsSubframe] == FALSE)   {
#endif
         DU_LOG("\nERROR  -->  SCH : UE specific SRS is not occuring in Cell specific SRS subframe"
               "srs Cfg Idx =%d CRNTI:%d",
               srsCfg->srsSetup.srsCfgIdx,crnti);
         return RFAILED;
      }


      if ( srsCfg->srsSetup.fDomPosi > RG_SCH_SRS_FREQDOM_POS_MAX )
      {
         DU_LOG("\nERROR  -->  SCH : Not Supported or Invalid Cfg" 
                  "SRS Min Freq Domain Position =%d"   
                  "Max Freq Domain Position =%d  Cfg Val=%d CRNTI:%d",
                  RG_SCH_SRS_FREQDOM_POS_MIN, RG_SCH_SRS_FREQDOM_POS_MAX,
                  srsCfg->srsSetup.srsCfgIdx,crnti);
         return RFAILED;
      }

      if ( srsCfg->srsSetup.txComb > RG_SCH_SRS_TXCOMB_MAX )
      {
         DU_LOG("\nERROR  -->  SCH : Not Supported or Invalid Cfg" 
                  "SRS Min TX Comb =%d   Max TX Comb =%d  Cfg Val=%d CRNTI:%d",
                  RG_SCH_SRS_TXCOMB_MIN, RG_SCH_SRS_TXCOMB_MAX,
                  srsCfg->srsSetup.srsCfgIdx,crnti);
         return RFAILED;
      }
   }
   return ROK;
}

/*
 * @brief Validates the UE SR Re/configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUeSrCfg 
 *
 *     Processing Steps:
 *      - Validate the UE re/configuration request from RRC to MAC at CFG:
 *        validate the value range for SR values.
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in] RgSchCellCb      *cellCb,
 *  @param[in] CmLteRnti        crnti,  
 *  @param[in] RgrUeSrCfg       *srCfg,  
 *  @param[out] RgSchErrInfo    *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef LTEMAC_HDFDD 
S16 rgSCHCfgVldtRgrUeSrCfg 
(
 RgSchCellCb      *cellCb,
 CmLteRnti        crnti,  
 RgrUeSrCfg       *srCfg,
 Bool             hdFdd,
 RgSchErrInfo     *errInfo
 )
#else
S16 rgSCHCfgVldtRgrUeSrCfg 
(
 RgSchCellCb      *cellCb,
 CmLteRnti        crnti,  
 RgrUeSrCfg       *srCfg,  
 RgSchErrInfo     *errInfo
 )
#endif
{

   if ( RGR_SCH_SR_SETUP == srCfg->type )
   {
      /* 1. Validate SR Configuration related parameter values  */
#ifdef LTEMAC_HDFDD
      if(hdFdd)
      {
         if (( srCfg->srSetup.srResIdx > RG_SCH_SR_RES_IDX   ) ||
               ( srCfg->srSetup.srCfgIdx < 5 ) ||
               ( srCfg->srSetup.srCfgIdx > RG_SCH_ISR_MAX_SUPP )) 
         {
             DU_LOG("\nERROR  -->  SCH : Invalid SR Index Cfg =%d"
                      "Max Supp=%d,Min Supp=%d CRNTI:%d", srCfg->srSetup.srCfgIdx, 
                     RG_SCH_ISR_MAX_SUPP, RG_SCH_ISR_MIN_SUPP,crnti);

            return RFAILED;
         }
      }
      else
      {
         if (( srCfg->srSetup.srResIdx > RG_SCH_SR_RES_IDX   ) ||
               ( srCfg->srSetup.srCfgIdx > RG_SCH_ISR_MAX_SUPP )) 
         {
            DU_LOG("\nERROR  -->  SCH : Invalid SR Index Cfg =%d" 
                      "Max Supp=%d,Min Supp=%d CRNTI:%d", srCfg->srSetup.srCfgIdx, 
                     RG_SCH_ISR_MAX_SUPP, RG_SCH_ISR_MIN_SUPP,crnti);

            return RFAILED;
         }
      }
#else
      if (( srCfg->srSetup.srResIdx > RG_SCH_SR_RES_IDX   ) ||
            ( srCfg->srSetup.srCfgIdx > RG_SCH_ISR_MAX_SUPP )) 
      {
         DU_LOG("\nERROR  -->  SCH : Invalid SR Index Cfg =%d"
                  "Max Supp=%d,Min Supp=%d CRNTI:%d", srCfg->srSetup.srCfgIdx, 
                  RG_SCH_ISR_MAX_SUPP, RG_SCH_ISR_MIN_SUPP,crnti);

         return RFAILED;
      }
#endif
   } 
   return ROK;
}


/*
 * @brief Validates the UE Aperiodic & Periodic  CQI, PMI, RI, SRS and SR Configuration 
 *             request from RRM to MAC.
 *
 * @details
 *
 *     Function :rgSCHCfgVldtCqiSrSrsUeCfg 
 *
 *     Processing Steps:
 *      - Validate the UE configuration request from RRC to MAC at CFG:
 *        validate the value range for Aperiodic & Periodic CQI, PMI, RI , SRS and SR values.
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  RgSchCellCb  *cellCb
 *  @param[in]  RgrUeCfg     *ueCfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
static S16 rgSCHCfgVldtCqiSrSrsUeCfg 
(
RgSchCellCb       *cellCb,  
RgrUeCfg          *ueCfg,  
RgSchErrInfo      *errInfo
)
{

   /*  1. Validate UE Aperiodic CQI related parameters */
   if ( ROK != rgSCHCfgVldtRgrUeACqiCfg (cellCb, ueCfg->crnti,
            &ueCfg->ueDlCqiCfg.aprdCqiCfg, ueCfg->txMode, errInfo ))
   {
      DU_LOG("\nERROR  -->  SCH : Invalid Aperiodic CQI configuration CRNTI:%d",ueCfg->crnti);
      return RFAILED;
   }

   /*  1. Validate UE Periodic CQI/PMI, SRS and SR related parameters */
#ifdef LTEMAC_HDFDD
   if ( ROK != rgSCHCfgVldtRgrUePCqiCfg (cellCb, ueCfg->crnti,
            &ueCfg->ueDlCqiCfg.prdCqiCfg, ueCfg->isHdFddEnbld, 
            ueCfg->txMode, errInfo ))
#else
      if ( ROK != rgSCHCfgVldtRgrUePCqiCfg (cellCb, ueCfg->crnti,
             &ueCfg->ueDlCqiCfg.prdCqiCfg, ueCfg->txMode, errInfo ))
#endif
      {
         DU_LOG("\nERROR  -->  SCH : Invalid Periodic CQI configuration CRNTI:%d",ueCfg->crnti);
         return RFAILED;
      }

   /*  2. Validate SRS Configuration related parameter values  */
#ifdef LTEMAC_HDFDD
   if ( ROK != rgSCHCfgVldtRgrUeUlSrsCfg(cellCb, ueCfg->crnti, &ueCfg->srsCfg,
            ueCfg->isHdFddEnbld, errInfo ))
#else
      if ( ROK != rgSCHCfgVldtRgrUeUlSrsCfg(cellCb, ueCfg->crnti, 
                  &ueCfg->srsCfg, errInfo ))
#endif
      {
         DU_LOG("\nERROR  -->  SCH : Invalid SRS  configuration CRNTI:%d",ueCfg->crnti);
         return RFAILED;
      } 

   /*  3. Validate SR Configuration related parameter values  */
#ifdef LTEMAC_HDFDD
   if ( ROK != rgSCHCfgVldtRgrUeSrCfg (cellCb, ueCfg->crnti, &ueCfg->srCfg,
            ueCfg->isHdFddEnbld, errInfo))
#else
      if ( ROK != rgSCHCfgVldtRgrUeSrCfg (cellCb, ueCfg->crnti, 
                  &ueCfg->srCfg, errInfo))
#endif
      {
         DU_LOG("\nERROR  -->  SCH : Invalid SR  configuration CRNTI:%d",ueCfg->crnti);
         return RFAILED;
      }

   return ROK;

}



/*****************************************************************
 *   UE PCQI, RI, SRS and SR Re Configuration  Validation Functions  *
 *                                                                *
 ******************************************************************/
/*
 * @brief Validates the UE Periodic CQI, PMI, RI, SRS and SR 
 * Re-configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtCqiSrSrsUeReCfg 
 *
 *     Processing Steps:
 *      - Validate the UE Re configuration request from RRC to MAC at CFG:
 *        validate the value range for Periodic CQI, PMI, RI, SRS and SR values.
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]   RgSchCellCb *cellCb
 *  @param[in]   RgSchUeCb   *ueCb
 *  @param[in]  RgrUeCfg     *ueCfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
static S16 rgSCHCfgVldtCqiSrSrsUeReCfg 
(
RgSchCellCb          *cellCb,
RgSchUeCb            *ueCb,
RgrUeRecfg           *ueReCfg,  
RgSchErrInfo         *errInfo
)
{
   RgrUeTxModeCfg     txMode;

   txMode.pres = TRUE;
   txMode.tmTrnstnState = RGR_TXMODE_RECFG_CMPLT;
   if ((ueReCfg->ueRecfgTypes & RGR_UE_TXMODE_RECFG) &&
       (ueReCfg->txMode.pres == TRUE)) 
   {
      txMode.txModeEnum = ueReCfg->txMode.txModeEnum;
   }
   else
   {
      txMode.txModeEnum = ueCb->mimoInfo.txMode;
   }

   /*    1. Validate UE CQI/PMI, SRS and SR related parameters */

   if ( ueReCfg->ueRecfgTypes & RGR_UE_APRD_DLCQI_RECFG ) 
   { 
      if ( ROK != rgSCHCfgVldtRgrUeACqiCfg (cellCb, ueReCfg->oldCrnti,     
               &ueReCfg->aprdDlCqiRecfg, txMode, errInfo ))
      {
         DU_LOG("\nERROR  -->  SCH : Invalid Aperiodic CQI configuration OLD CRNTI:%d",ueReCfg->oldCrnti);
         return RFAILED;
      }
   }


   /*    2. Validate UE CQI/PMI, SRS and SR related parameters */

   if ( ueReCfg->ueRecfgTypes & RGR_UE_PCQI_RECFG ) 
   { 
#ifdef LTEMAC_HDFDD   
      if ( ROK != rgSCHCfgVldtRgrUePCqiCfg (cellCb, ueReCfg->oldCrnti,     
               &ueReCfg->cqiCfg, ueReCfg->isHdFddEnbld, txMode, errInfo ))
#else
         if ( ROK != rgSCHCfgVldtRgrUePCqiCfg (cellCb, ueReCfg->oldCrnti,     
                  &ueReCfg->cqiCfg, txMode, errInfo ))
#endif

         {
            DU_LOG("\nERROR  -->  SCH : Invalid Periodic CQI configuration OLD CRNTI:%d",ueReCfg->oldCrnti);
            return RFAILED;
         }
   }

   if(ueReCfg->ueRecfgTypes & RGR_UE_SRS_RECFG )
   {
#ifdef LTEMAC_HDFDD
      if ( ROK != rgSCHCfgVldtRgrUeUlSrsCfg(cellCb, ueReCfg->oldCrnti,
               &ueReCfg->srsCfg, ueReCfg->isHdFddEnbld, errInfo ))
#else
         if ( ROK != rgSCHCfgVldtRgrUeUlSrsCfg(cellCb, ueReCfg->oldCrnti,
                  &ueReCfg->srsCfg, errInfo ))
#endif
         {
            DU_LOG("\nERROR  -->  SCH : Invalid SRS  configuration OLD CRNTI:%d",ueReCfg->oldCrnti);
            return RFAILED;
         } 

   }

   if ( ueReCfg->ueRecfgTypes & RGR_UE_SR_RECFG )
   {
#ifdef LTEMAC_HDFDD
      if ( ROK != rgSCHCfgVldtRgrUeSrCfg (cellCb, ueReCfg->oldCrnti, 
            &ueReCfg->srCfg, ueReCfg->isHdFddEnbld, errInfo))
#else
         if ( ROK != rgSCHCfgVldtRgrUeSrCfg (cellCb, ueReCfg->oldCrnti, 
            &ueReCfg->srCfg, errInfo))
#endif
         {
            DU_LOG("\nERROR  -->  SCH : Invalid SR configuration OLD CRNTI:%d",ueReCfg->oldCrnti);
            return RFAILED;
         }
   }

   return ROK;
}


/*****************************************************************
 *   UE ACQI, PCQI, RI, SRS SR Configuration Functions      *
 *                                                                *
 ******************************************************************/
   /**
    * @brief Handles Aperiodic CQI , PMI, RI  configuration for a UE.
    *
    * @details
    *
    *     Function : rgSCHCfgACqiUeCfg 
    *
    *     Invoking Module Processing:
    *      - This shall be invoked by SCH_GOM at UE configuration. It 
    *      shall do the validations for the spec-defined values.
    *        
    *     Processing Steps:
    *     
    *     - Return ROK
    *
    *  @param[in]  RgSchCellCb  *cell
    *  @param[in]  RgrTxMode    txMode
    *  @param[in]  RgrUeAprdDlCqiCfg *aCqiCfg
    *  @param[in]  CmLteUeCategory        ueCat
    *  @return  S16
    *      -# ROK
    *
    **/
S16 rgSCHCfgACqiUeCfg
(
RgSchCellCb        *cellCb, 
RgSchUeCb          *ue, 
RgSchUeACqiCb      *acqiCb,
RgrTxMode           ueTxMode,
RgrUeAprdDlCqiCfg  *aCqiCfg,
CmLteUeCategory     ueCat
)
{
   uint8_t     M;  /*Num of Subbands -- Applicable only for Mode 2-0 and 2-2*/
   uint8_t     k;  /*SubBand Size (RB)  -- 
              Holds different values depending on Mode*/
   uint8_t     cqiPmiSzR1;   /*CQIPMI Size for Rank =1*/
   uint8_t     cqiPmiSzRn1; /*CQIPMI Size for Rank > 1*/

   cqiPmiSzR1 = 0;
   cqiPmiSzRn1 = 0;
   
   acqiCb->aCqiCfg.pres = aCqiCfg->pres;
   acqiCb->aCqiCfg.aprdModeEnum = aCqiCfg->aprdModeEnum;

   if(aCqiCfg->pres)
   {
#ifdef LTE_ADV
      /*Store Trigger Set Bit String to UE */
      RgSchUeCellInfo *pCellInfo = RG_SCH_CMN_GET_PCELL_INFO(ue);
      pCellInfo->acqiCb.aCqiCfg.triggerSet1 = aCqiCfg->triggerSet1;
      pCellInfo->acqiCb.aCqiCfg.triggerSet2 = aCqiCfg->triggerSet2;
#endif


      switch(aCqiCfg->aprdModeEnum)
      {
         case RGR_APRD_CQI_MOD12:
         case RGR_APRD_CQI_MOD30:
         case RGR_APRD_CQI_MOD31:
            RG_SCH_GET_CQI_K_VAL(cellCb->bwCfg.dlTotalBw, k);
            acqiCb->N = RGSCH_CEIL(cellCb->bwCfg.dlTotalBw, k);
            acqiCb->k = k;
            break;

         case RGR_APRD_CQI_MOD20:
         case RGR_APRD_CQI_MOD22:
            RG_SCH_GET_SBCQI_M_K_VAL(cellCb->bwCfg.dlTotalBw, M, k);
            acqiCb->N = RGSCH_CEIL(cellCb->bwCfg.dlTotalBw, k);
            acqiCb->k = k;
            acqiCb->M = M;
            acqiCb->L = RgSCHUeAcqi2022LBitWidth[M-1][acqiCb->N-1];
            break;

         default:
            break; 
      }
      if((ueTxMode == RGR_UE_TM_3) || 
         (ueTxMode == RGR_UE_TM_4))
      {
         if(cellCb->numTxAntPorts ==2)
         {
            acqiCb->riNumBits = 1;
         }
         else if(cellCb->numTxAntPorts ==4)
         {
            if(ueCat == CM_LTE_UE_CAT_8)
            {
               acqiCb->riNumBits = 3;
            }
            else if(ueCat >= CM_LTE_UE_CAT_5)  
            {
               acqiCb->riNumBits = 2;
            }
            else
            {
               acqiCb->riNumBits = 1;
            }
         }
      }
      rgSCHCfgUtlFetchAcqiBitSz(acqiCb, cellCb->numTxAntPorts, 
            &cqiPmiSzR1, &cqiPmiSzRn1);
      acqiCb->cqiPmiSzR1  = cqiPmiSzR1;
      acqiCb->cqiPmiSzRn1 = cqiPmiSzRn1;
   }
   acqiCb->cqiReqField = TRUE;
#ifdef LTE_ADV
   rgSchCmnSetCqiReqField(RG_SCH_CMN_GET_SCELL_INFO(ue, cellCb),ue,&acqiCb->cqiReqField);
#endif
   
   return ROK;
}

/**
 * @brief Handles Periodic CQI , PMI, RI  configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgPCqiUeCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE configuration. It shall do the
 *      validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific Periodic CQI  related configuration, 
 *      - If Periodic CQI/PMI is configured,
 *        - Update UE with the configured values.
 - Compute and Update next occurance of CQI/PMI or RI Tranmission instance.
 - Update the CQI offset and CQI perodicity information
 - Add Ue to cell's list  
 *        
 *
 *     - For UE-specific Periodic RI  related configuration, 
 *      - If Periodic RI is configured,
 *        - Update UE with the configured values.
 - Compute and Update next occurance of RI Tranmission instance.
 - Update the  RI offset and RI perodicity information
 *      
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUePrdDlCqiCfg *cqiCfg
 *  @param[in]  CmLteUeCategory   ueCat
 *  @return  S16
 *      -# ROK
 *
 **/
S16 rgSCHCfgPCqiUeCfg
(
RgSchCellCb         *cellCb, 
RgSchUeCb           *ueCb, 
RgrUePrdDlCqiCfg    *cqiCfg,
CmLteUeCategory     ueCat
)
{  
   CmLteTimingInfo timingInfo;
   uint16_t    crntTime;           
   uint16_t    cqiTrInstTime; 
   uint8_t     j;  /*Bandwidth Parts*/
   uint8_t     temp; 
   uint8_t     loop;
   RgSchUePCqiCb *cqiCb = NULLP;

   crntTime = (cellCb->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G)+
                  (cellCb->crntTime.slot);
   cqiCb = RG_SCH_GET_UE_CELL_CQI_CB(ueCb,cellCb);
   cqiCb->servCellInfo = ueCb->cellInfo[0];
   /* Periodic CQI is setup  */
   if (cqiCfg->type == RGR_SCH_PCQI_SETUP)
   {   
      for(loop = 0; loop < MAX_CQI_RI_RPT_BUFF;loop++)
      {
         ueCb->rawCqiBitW[loop].type = TFU_RECP_REQ_INVLD; /* setting invalid  type*/
      }
      /*  1. Copy the Received CQI Cfg parameters to ueCb  */
      memcpy(&cqiCb->cqiCfg, cqiCfg, 
            sizeof(RgrUePrdDlCqiCfg));

      /*  2. Compute Periodic CQI Periodicity and subframe offset   */
#ifndef LTE_TDD           
      rgSCHUtlGetCfgPerOff(RG_SCH_FDD_PCQI_TBL, cqiCfg->cqiSetup.cqiPCfgIdx,
            &cqiCb->cqiPeri, &cqiCb->cqiOffset);      
#else
      rgSCHUtlGetCfgPerOff( RG_SCH_TDD_PCQI_TBL, 
            cqiCfg->cqiSetup.cqiPCfgIdx,
            &cqiCb->cqiPeri, &cqiCb->cqiOffset);    
#endif
      DU_LOG("\nDEBUG  -->  SCH : rgSCHCfgPCqiUeCfg(): UEID:%d CQI Peri=%d, CQI Offset=%d",
            ueCb->ueId,
            cqiCb->cqiPeri,
            cqiCb->cqiOffset);


      cqiTrInstTime = ((cqiCb->cqiPeri+crntTime) - cqiCb->cqiOffset)
         %cqiCb->cqiPeri;
      cqiCb->nCqiTrIdx = (crntTime + 
            (cqiCb->cqiPeri - cqiTrInstTime));
      /* Introduced timing delta for reception req
       * in FDD*/
      if(cqiCb->nCqiTrIdx  <= (crntTime + TFU_RECPREQ_DLDELTA))
      {
         cqiCb->nCqiTrIdx = cqiCb->nCqiTrIdx + cqiCb->cqiPeri;
      }

      /* To handle the SFN wrap around case */
      cqiCb->nCqiTrIdx = cqiCb->nCqiTrIdx % (RGSCH_MAX_SFN * RGSCH_NUM_SUB_FRAMES_5G);

      timingInfo.sfn =  cqiCb->nCqiTrIdx/RGSCH_NUM_SUB_FRAMES_5G;
      timingInfo.slot =  cqiCb->nCqiTrIdx % RGSCH_NUM_SUB_FRAMES_5G;

      cqiCb->nCqiTrIdx = cqiCb->nCqiTrIdx
           %RG_SCH_PCQI_SRS_SR_TRINS_SIZE;

#ifdef EMTC_ENABLE
	  /*CQI Repetition configuration*/
      if(ueCb->isEmtcUe)
      {
          rgSchfillPucchCqiRepNumCountemtc(cqiCb, ueCb);
      }
#endif

      if(RGR_UE_PCQI_SB_REP == cqiCfg->cqiSetup.cqiRepType)
      {
         uint8_t     k;  /*SubBand Size (RB) */
         RG_SCH_GET_CQI_J_VAL(cellCb->bwCfg.dlTotalBw, j);
         RG_SCH_GET_CQI_K_VAL(cellCb->bwCfg.dlTotalBw, k);
         cqiCb->J = j; /*Number of Bandwidth Parts*/
         /*h: reporting instances required for a complete CQI/PMI report */
         /*j:Number of bandwidth parts; k: Subband Size*/
         cqiCb->h = (cqiCb->cqiCfg.cqiSetup.k *j )+1; 
         /* ccpu00140905- L-size is coming as 3 for 100Rbs where it should be 2*/
         temp = RGSCH_CEIL(cellCb->bwCfg.dlTotalBw, (j*k));
         cqiCb->label = (temp & (temp-1)) ?
            (1+ rgSCHUtlLog32bitNbase2(temp)) : rgSCHUtlLog32bitNbase2(temp);

         rgSCHTomUtlPcqiSbCalcBpIdx(timingInfo,ueCb, cqiCb); 
      }
#ifdef LTE_ADV
      else
      {
         cqiCb->prioLvl = RG_SCH_CQI_PRIO_LVL_1;
      }
#endif

      /*    Place the UE in cellCb->tIUeLstCp  */
      cqiCb->cqiLstEnt.node=(PTR) cqiCb;
#ifdef LTE_ADV
      cqiCb->isCqiIgnoByCollsn = FALSE;
      cqiCb->isRiIgnoByCollsn = FALSE;
#endif


      cmLListAdd2Tail(&cellCb->pCqiSrsSrLst[cqiCb->nCqiTrIdx].cqiLst,
            &cqiCb->cqiLstEnt);  


      /* 4. Rank Indicator Cfg handler */
      rgSCHCfgRiUeCfg(cellCb, ueCb, cqiCfg, ueCat);
   }
   else
   { 
      cqiCb->cqiCfg.type =  RGR_SCH_PCQI_REL;
      if(cqiCb->nCqiTrIdx != RG_SCH_INVALID_IDX)
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[cqiCb->nCqiTrIdx].cqiLst,\
               &cqiCb->cqiLstEnt);
      } 
      if(cqiCb->nRiTrIdx != RG_SCH_INVALID_IDX)
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[cqiCb->nRiTrIdx].riLst, \
               &cqiCb->riLstEnt);
          RG_SCH_RECORD(&cqiCb->histElem,RGSCH_ACTION_DEL,
            &cellCb->pCqiSrsSrLst[cqiCb->nRiTrIdx].riLst);
      }
      cqiCb->nCqiTrIdx = RG_SCH_INVALID_IDX;
      cqiCb->nRiTrIdx   = RG_SCH_INVALID_IDX;
      cqiCb->riDist   = RG_SCH_INVALID_IDX; 
   } 
   ueCb->nPCqiCb = cqiCb;
   ueCb->nPRiCb = cqiCb;
   return ROK;
}

/**
 * @brief Handles Periodic  RI  configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgRiUeCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE configuration. It shall do the
 *      validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific Periodic RI  related configuration, 
 *      - If Periodic RI is configured,
 *        - Update UE with the configured values.
 - Compute and Update next occurance of RI Tranmission instance.
 - Update the  RI offset and RI perodicity information
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUePrdDlCqiCfg *cqiCfg
 *  @param[in]  CmLteUeCategory   ueCat
 *  @return  S16
 *      -# ROK
 *
 **/
S16 rgSCHCfgRiUeCfg
(
RgSchCellCb        *cellCb, 
RgSchUeCb          *ueCb, 
RgrUePrdDlCqiCfg   *cqiCfg,
CmLteUeCategory    ueCat
)
{
   uint16_t     crntTime;  
   uint16_t     riTrInsTime;  
   uint8_t      j; /*Bandwidth parts. Valid for Modes 2-0, 2-1*/         
   uint16_t     periodicity; 
   uint16_t    tempIdx; 
   RgSchUePCqiCb *cqiCb = NULLP;

   crntTime = (cellCb->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G)
      +(cellCb->crntTime.slot);
   cqiCb = RG_SCH_GET_UE_CELL_CQI_CB(ueCb,cellCb);
   /* 1. Rank Indicator is enabled  */
   if(cqiCfg->cqiSetup.riEna)
   {

      rgSCHUtlGetCfgPerOff(RG_SCH_RI_TBL, 
            cqiCfg->cqiSetup.riCfgIdx,
            &cqiCb->riPeri, &cqiCb->riOffset);

      DU_LOG("\nDEBUG  -->  SCH : rgSCHCfgRiUeCfg(): RI Peri=%d, RI Offset=%d UEID:%d", 
            cqiCb->riPeri, cqiCb->riOffset,ueCb->ueId);

      cqiCb->perRiVal = 1;
      cqiCb->invalidateCqi = FALSE;

      if(RGR_UE_PCQI_WB_REP == cqiCfg->cqiSetup.cqiRepType)
      {
         /* 
            1. wideband RI reporting is configured 
            (Mode 1-0 or 1-1)
            (10*sfn+floor(subframe)-Noffsetcqi-NoffsetRI )Mod(NCqiperiod
          *MriPeriod)=0  
          */ 
         periodicity = cqiCb->cqiPeri * cqiCb->riPeri; 
      }
      else
      {
         /*
         *  Where Widesband and Subband RI reporting is configured
         *   (Mode 2-0 or 2-1 )
         *   (10*sfn+floor(subframe)-Noffsetcqi-NoffsetRI )
         *   Mod(H. NCqiperiod *MriPeriod )=0 
         *   where H= J * K +1;  J=Number of bandwidth parts(BW/subsize). 
         *   K is RGR interf input 
         */

         RG_SCH_GET_CQI_J_VAL(cellCb->bwCfg.dlTotalBw, j);
         cqiCb->h = (cqiCb->cqiCfg.cqiSetup.k *j )+1;  
         periodicity = cqiCb->h * cqiCb->cqiPeri * 
                                  cqiCb->riPeri; 
      }

      /* In case of SFN wraparound, the SB CQI reporting cycle breaks
       * and RI->WB CQI->SBCQI.. should resume. RI is repositioned 
       * accordingly. WBCQI handling is naturally accomplished */
      if (periodicity >= RGSCH_MAX_SUBFRM_5G)
      {
         periodicity = cqiCb->cqiOffset - cqiCb->riOffset + 
                       RGSCH_MAX_SUBFRM_5G - (crntTime);
         tempIdx = crntTime + periodicity;
	 DU_LOG("\nINFO  -->  SCH : CHECK_SID - periodicity %d tempIdx %d\n", periodicity, tempIdx);
      }
      else
      {
         if ((crntTime + TFU_RECPREQ_DLDELTA + periodicity) > 
                            (RGSCH_MAX_SUBFRM_5G - 1))
         {
            riTrInsTime = (periodicity - cqiCb->cqiOffset + cqiCb->riOffset) % periodicity; 
            tempIdx = RGSCH_MAX_SUBFRM_5G + (periodicity - riTrInsTime);
         }
         else
         {
            riTrInsTime = ((periodicity +crntTime )- \
               cqiCb->cqiOffset + cqiCb->riOffset)\
                       % periodicity;
            tempIdx = (crntTime + (periodicity -riTrInsTime));
         }
      }
      if (tempIdx <= (crntTime + TFU_RECPREQ_DLDELTA))
      {
         tempIdx = tempIdx + periodicity; 
      }
      cqiCb->nRiTrIdx = tempIdx 
         % RG_SCH_PCQI_SRS_SR_TRINS_SIZE;
      if(periodicity >= RG_SCH_PCQI_SRS_SR_TRINS_SIZE)
      {  
         cqiCb->riDist = rgSCHUtlFindDist((uint16_t)(crntTime + TFU_RECPREQ_DLDELTA),
                (uint16_t) tempIdx);
      }
      else
      {
         cqiCb->riDist =0; 
      }
      if(ueCb->mimoInfo.txMode == RGR_UE_TM_3 
            || ueCb->mimoInfo.txMode == RGR_UE_TM_4)
      {
         if (cellCb->numTxAntPorts ==2)
         {
            cqiCb->riNumBits = 1;
         }
         else if(cellCb->numTxAntPorts ==4)
         {
            if(ueCat == CM_LTE_UE_CAT_8)
            {
               cqiCb->riNumBits = 3;
            }
            else if(ueCat >= CM_LTE_UE_CAT_5)  
            {
               cqiCb->riNumBits = 2;
            }
            else
            {
               cqiCb->riNumBits = 1;
            }
         }
      }
      /*    Place the UE in cellCb->tIUeLstCp  */
      cqiCb->riLstEnt.node=(PTR) cqiCb;

      cmLListAdd2Tail(&cellCb->pCqiSrsSrLst[cqiCb->nRiTrIdx].riLst,
            &cqiCb->riLstEnt);  
      RG_SCH_RECORD(&cqiCb->histElem,RGSCH_ACTION_ADD,
         &cellCb->pCqiSrsSrLst[cqiCb->nRiTrIdx].riLst);
     

   }
   else
   {
      cqiCb->nRiTrIdx = RG_SCH_INVALID_IDX;
      cqiCb->riDist   = RG_SCH_INVALID_IDX; 
   }

   return ROK;

}

/* @brief Handles SRS configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgSrsUeCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE configuration. It shall do the
 *      validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific SRS  related configuration, 
 *      - If SRS is configured,
 *        - Update UE with the configured values.
 - Compute and Update next occurance of SRS Tranmission instance.
 - Update the  SRS offset and SRS perodicity information
 - Add Ue to cell's srs list
 *      - else
 *        - next occurance transmission instance of SRS  = RG_SCH_INVALID_IDX
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeUlSrsCfg   *srsCfg
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
S16 rgSCHCfgSrsUeCfg
(
RgSchCellCb     *cellCb,
RgSchUeCb       *ueCb,
RgrUeUlSrsCfg   *srsCfg 
)
{
   uint16_t      srsTrInsTime;
   uint16_t      crntTime;  
   uint16_t      tempIdx;

   crntTime = (cellCb->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G)
      +(cellCb->crntTime.slot);

   if(RGR_SCH_SRS_SETUP == srsCfg->type) 
   {
      /*  1. Copy the Received Cfg parameters to local cb  */
      memcpy(&ueCb->srsCb.srsCfg, srsCfg, sizeof(RgrUeUlSrsCfg));

#ifndef LTE_TDD
      /* 2.  Compute SRS Offset and Periodicity */
      rgSCHUtlGetCfgPerOff( RG_SCH_FDD_SRS_TBL, 
            srsCfg->srsSetup.srsCfgIdx,
            &ueCb->srsCb.peri, &ueCb->srsCb.offset);      
#else
      rgSCHUtlGetCfgPerOff( RG_SCH_TDD_SRS_TBL, 
            srsCfg->srsSetup.srsCfgIdx,
            &ueCb->srsCb.peri, &ueCb->srsCb.offset);      
#endif       

      DU_LOG("\nDEBUG  -->  SCH : rgSCHCfgSrsUeCfg(): SRS Peri=%d, SRS Offset=%d UEID:%d", 
            ueCb->srsCb.peri,ueCb->srsCb.offset,ueCb->ueId);

      /*  3. Compute next Tranmission index for SRS */ 
      /*     Referenence: 36.213 Section:8.2
             i. SRS transmission instances for TDD with  period > 2 and for FDD are 
             ((10*sfn +Ksrs-suframeoffset))/mod(periodicity))  
             FDD: Ksrs is 0...9
             TDD: Ksrs Table 8.2-3: 
             ii.The SRS transmission instances for TDD (periodicity == 2)
             (Ksrs-Toffset)mod(5)==0.  Note: This is not supported now  
      */ 

      srsTrInsTime = ((ueCb->srsCb.peri+crntTime) - ueCb->srsCb.offset)
         %ueCb->srsCb.peri;
     tempIdx = (crntTime + (ueCb->srsCb.peri - srsTrInsTime));
#ifdef LTE_TDD
      if (tempIdx  <=  (crntTime + TFU_DELTA))
#else
      if (tempIdx  <=  (crntTime + TFU_RECPREQ_DLDELTA))
#endif
      {
         tempIdx = tempIdx + ueCb->srsCb.peri;
      }
      ueCb->srsCb.nSrsTrIdx =(uint16_t) (tempIdx 
         % RG_SCH_PCQI_SRS_SR_TRINS_SIZE);
      if(ueCb->srsCb.peri >= RG_SCH_PCQI_SRS_SR_TRINS_SIZE)
      {
#ifdef LTE_TDD
         ueCb->srsCb.srsDist = rgSCHUtlFindDist((uint8_t)(crntTime+TFU_DELTA), 
                                                    (uint16_t)tempIdx); 
#else
         ueCb->srsCb.srsDist = rgSCHUtlFindDist((uint8_t)(crntTime + TFU_RECPREQ_DLDELTA), 
                                                    (uint16_t)tempIdx); 
#endif
      }
      else
      {
         ueCb->srsCb.srsDist = 0; 
      }

      /*UE Tx Antenna Selection - START*/
      if(ueCb->ul.ulTxAntSel.pres == TRUE )
      {
          /*for both partial and full sounding bandwidth, 
           and when frequency hopping is disabled */
         ueCb->srsCb.selectedAnt = (crntTime/ueCb->srsCb.peri)%2; 
      }
      else
      {
         /* TS 36.213 specifies that if Tx Antenna Selection is 
            disabled/not supported then its Port 0*/
         ueCb->srsCb.selectedAnt=0; 
      }
      ueCb->validTxAnt = ueCb->srsCb.selectedAnt; 
      /*UE Tx Antenna Selection - ENDS*/

      ueCb->srsCb.srsLstEnt.node=(PTR)ueCb;
      cmLListAdd2Tail(&cellCb->pCqiSrsSrLst[ueCb->srsCb.nSrsTrIdx].srsLst,
            &ueCb->srsCb.srsLstEnt);  


   }
   else
   {
      /*  SRS Release / Not configured */
      ueCb->srsCb.srsCfg.type = RGR_SCH_SRS_REL;
      if(ueCb->srsCb.nSrsTrIdx != RG_SCH_INVALID_IDX)
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->srsCb.nSrsTrIdx].srsLst,
               &ueCb->srsCb.srsLstEnt);
      }
      ueCb->srsCb.nSrsTrIdx = RG_SCH_INVALID_IDX; 
      ueCb->srsCb.srsLstEnt.node =(PTR) NULLP;
   }

   return ROK;
}


/* * @brief Handles SR configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgSrUeCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE configuration. It shall do the
 *      validations for the spec-defined values.
 *        
 *      - If SR is configured,
 *        - Update UE with the configured values.
 - Compute and Update next occurance of SR Tranmission instance.
 - Update the  SR offset and SR perodicity information
 - Add Ue to cell->tIUeLstCp[n]  
 *      - else
 *        - next occurance transmission instance of SR = RG_INVALID_SR_ID
 *
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeSrCfg       *srCfg
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
S16 rgSCHCfgSrUeCfg
(
RgSchCellCb      *cellCb,
RgSchUeCb        *ueCb,
RgrUeSrCfg       *srCfg
)
{
   uint16_t     srTrInsTime;
   uint16_t     crntTime;


   crntTime = (cellCb->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G)
      +(cellCb->crntTime.slot);
   if(srCfg->type == RGR_SCH_SR_SETUP) 
   {
      /*  1. Copy the Received Cfg parameters to local cb  */
      memcpy(&ueCb->srCb.srCfg, srCfg, sizeof(RgrUeSrCfg));


      /*  2. Compute SR periodicity and offset */
      rgSCHUtlGetCfgPerOff( RG_SCH_SR_TBL, 
            srCfg->srSetup.srCfgIdx,
            &ueCb->srCb.peri, &ueCb->srCb.offset);
    
      DU_LOG("\nDEBUG  -->  SCH : SR Config: idx(%u), period (%u) offset (%u) UEID:%d",
            srCfg->srSetup.srCfgIdx,
            ueCb->srCb.peri, 
            ueCb->srCb.offset,
            ueCb->ueId);
#ifdef EMTC_ENABLE
      if(ueCb->isEmtcUe)
      {
         rgSchfillPucchSrRepNumCountemtc(ueCb);
      }
#endif
      /*   3. Compute Next Transmission Instance */

      srTrInsTime = ((ueCb->srCb.peri+crntTime) - ueCb->srCb.offset)
         %ueCb->srCb.peri;
      ueCb->srCb.nSrTrIdx = (crntTime + (ueCb->srCb.peri- srTrInsTime)); 
#ifdef LTE_TDD
      if (ueCb->srCb.nSrTrIdx  <= (crntTime + TFU_DELTA))
#else
      if (ueCb->srCb.nSrTrIdx  <= (crntTime + TFU_RECPREQ_DLDELTA))
#endif
      {
         ueCb->srCb.nSrTrIdx = ueCb->srCb.nSrTrIdx +  ueCb->srCb.peri;
      }
      ueCb->srCb.nSrTrIdx = ueCb->srCb.nSrTrIdx 
         % RG_SCH_PCQI_SRS_SR_TRINS_SIZE;
      ueCb->srCb.srLstEnt.node= (PTR) ueCb;

      /* 4. Place UE in Cell SR Tranmisison Instance List  */
      cmLListAdd2Tail(&cellCb->pCqiSrsSrLst[ueCb->srCb.nSrTrIdx].srLst,
            &ueCb->srCb.srLstEnt);  
   }
   else
   {
      ueCb->srCb.srCfg.type =  RGR_SCH_SR_REL;

      if(ueCb->srCb.nSrTrIdx != RG_SCH_INVALID_IDX)
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->srCb.nSrTrIdx].srLst,
               &ueCb->srCb.srLstEnt);
      }
      ueCb->srCb.nSrTrIdx = RG_SCH_INVALID_IDX; 
      ueCb->srCb.srLstEnt.node = (PTR)NULLP;
   }

   return ROK;
}


/*****************************************************************
 *    UE PCQI, RI, SRS and SR       Re Configuration Functions    *
 *                                                                *
 ******************************************************************/


/* * @brief Handles Periodic CQI, PMI, RI  Re-configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgPCqiUeReCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE Re configuration. It shall do the
 *      validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific Periodic CQI  related configuration, 
 *      - If Periodic CQI/PMI is re configured(first time enabled),
 *          - Update UE with the configured values.
 *         - Compute and Update next occurance of CQI/PMI or RI Tranmission 
 *         instance.
 *         - Update the CQI offset and CQI perodicity information
 *         - Add Ue to cell's list 
 *      - If Periodic CQI/PMI is re configured(modify),
 *         - Update UE with the configured values.
 *         - Del Ue from cell->tIUeLstCp list 
 *          - Compute and Update next occurance of CQI/PMI or RI Tranmission
 *          instance.
 *         - Update the CQI offset and CQI perodicity information
 *         - Add Ue to cell's list
 *      - If Periodic CQI/PMI is re configured(disabled),
 *        - Update UE with the configured values.
 *         - Del Ue from cell's list  
 *         - Update next occurance of CQI/PMI or RI Tranmission instance.
 *        - next occurance of CQI/PMI = RG_INVALID_CQIPMI_ID
 *
 *     - For UE-specific Periodic RI  related configuration, 
 *      - If Periodic RI is configured(first time enabled),
 *        - Update UE with the configured values.
 *          - Compute and Update next occurance of RI Tranmission instance.
 *         - Update the  RI offset and RI perodicity information
 *      - If Periodic RI is configured(modify),
 *        - Update UE with the configured values.
 *         - Compute and Update next occurance of RI Tranmission instance.
 *         - Update the  RI offset and RI perodicity information
 *      - else
 *        - next occurance of RI  = RG_INVALID_CQIPMI_ID
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeCfg     *ueCfg
 *
 *  @return  S16
 *      -# ROK
 *
 */
S16 rgSCHCfgPCqiUeReCfg
(
RgSchCellCb        *cellCb, 
RgSchUeCb          *ueCb, 
RgrUePrdDlCqiCfg   *cqiCfg,
CmLteUeCategory      ueCat
)
{

   RgSchUePCqiCb *cqiCb = NULLP;

   cqiCb = RG_SCH_GET_UE_CELL_CQI_CB(ueCb,cellCb);
   /* Fix: ccpu00124008 Fix for incorrect check causing missed CQI reception instance */
   if((cqiCfg->type == RGR_SCH_PCQI_SETUP) &&
         (cqiCb->cqiCfg.type != RGR_SCH_PCQI_SETUP))
   {
      /* 1. cqi is in Release (Disable) state,  Recfg is allowing Setup (Enable)
      */
      rgSCHCfgPCqiUeCfg(cellCb, ueCb, cqiCfg, ueCat);

   }
   else if((cqiCfg->type == RGR_SCH_PCQI_SETUP) &&
         (cqiCb->cqiCfg.type == RGR_SCH_PCQI_SETUP ))
   {

      /*
         2. Present is SETUP(Enable) state,  Recfg is modifying SETUP(Enable)

         2.1 Delete UE from the cqiList 
         2.2 Set next occurance Transmission instace to "INVALID"
         2.3 Compute Next occurance Transmission instace 
         2.4 Placed ue in Transmission instance list.
         */    
      if(cqiCb->nCqiTrIdx != RG_SCH_INVALID_IDX )
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[cqiCb->nCqiTrIdx].cqiLst,
               &cqiCb->cqiLstEnt); 

      if(cqiCb->nRiTrIdx != RG_SCH_INVALID_IDX )
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[cqiCb->nRiTrIdx].riLst,
               &cqiCb->riLstEnt); 
         RG_SCH_RECORD(&cqiCb->histElem,RGSCH_ACTION_DEL,
            &cellCb->pCqiSrsSrLst[cqiCb->nRiTrIdx].riLst);
      }


      cqiCb->cqiLstEnt.next = NULLP;
      cqiCb->cqiLstEnt.prev  = NULLP;
      cqiCb->nCqiTrIdx = RG_SCH_INVALID_IDX;
      cqiCb->nRiTrIdx  = RG_SCH_INVALID_IDX;
      cqiCb->riDist    = RG_SCH_INVALID_IDX;

      rgSCHCfgPCqiUeCfg(cellCb, ueCb, cqiCfg, ueCat);
   }
   else
   {
      /* Present is SETUP(Enable) state,  Recfg is Release(Disable)  */
      rgSCHCfgPCqiUeCfg(cellCb, ueCb, cqiCfg, ueCat);
   }

   /* ccpu00140578:: */
   cqiCb->riRecpPrcsd = FALSE;
   return ROK;
}


/* * @brief Handles SRS  Re-configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgSrsUeReCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE Re configuration. It shall do the
 *      validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific SRS  related re configuration, 
 *      - If SRS is configured modified(First time Enabled),
 *        - Update UE with the configured values.
 *         - Compute and Update next occurance of SRS Tranmission instance.
 *        - Update the  SRS offset and SRS perodicity information
 *        - Add Ue to cell's list  
 *      - If SRS is configured modified(Changed offset or index ),
 *      - Delete UE from cell->tIUeLstCp[n] if present  
 *      - Update UE with the configured values.
 *      - Compute and Update next occurance of SRS Tranmission instance.
 *      - Update the  SRS offset and SRS perodicity information
 *      - Add Ue to cell's list  
 *      - If SRS is configured modified(disabled),
 *      - Delete UE from cell->tIUeLstCp[n] if present  
 *        - Update next occurance of SRS Tranmission instance to "INVALID".
 *        - Update the  SRS offset and SRS perodicity information "INVALID"
 *      - else
 *        - ROK 
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeUlSrsCfg    *srsCfg
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
S16 rgSCHCfgSrsUeReCfg
(
RgSchCellCb        *cellCb, 
RgSchUeCb          *ueCb, 
RgrUeUlSrsCfg      *srsCfg
)
{


   if(( RGR_SCH_SRS_SETUP == srsCfg->type) ||
         ( RGR_SCH_SRS_SETUP != ueCb->srsCb.srsCfg.type ))
   {
      /*     1. Present is Release(Disable) state,  Recfg is allowing
       *     SETUP(Enable)  */
      rgSCHCfgSrsUeCfg(cellCb, ueCb, srsCfg);
   } 
   else if (( RGR_SCH_SRS_SETUP == srsCfg->type ) &&
         ( RGR_SCH_SRS_SETUP  == ueCb->srsCb.srsCfg.type))
   {

      /*     2. Present is SETUP(Eanble) state,  Recfg is modifying SETUP(Enable) 

             2.1 Delete UE from the cqiList 
             2.2 Set next occurance Transmission instance to "INVALID"
             2.3 Compute Next occurance Transmission instance 
             2.4 Placed ue in Transmission instance list.
             */ 
      if (ueCb->srsCb.nSrsTrIdx != RG_SCH_INVALID_IDX)
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->srsCb.nSrsTrIdx].srsLst,
               &ueCb->srsCb.srsLstEnt);
         ueCb->srsCb.nSrsTrIdx = RG_SCH_INVALID_IDX;
      }

      rgSCHCfgSrsUeCfg(cellCb, ueCb, srsCfg);
   }
   else
   {
      /* 3. Present is SETUP(Enable) state,  Recfg is Release(Disable)*/ 
      rgSCHCfgSrsUeCfg(cellCb, ueCb, srsCfg);
   }
   /* ccpu00140578:: */
   ueCb->srsCb.srsRecpPrcsd = FALSE;

   return ROK;
}

/* @brief Handles  SR Re-configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgSrUeReCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE Re configuration. 
 *      It shall do the validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific SR  related re configuration, 
 *      - If SR is configured modified(First time Enabled),
 *        - Update UE with the configured values.
 *          - Compute and Update next occurance of SR Tranmission instance.
 *         - Update the  SR offset and SR perodicity information
 *         - Add Ue to cell->tIUeLstCp[n]  
 *      - If SR is configured modified(Changed offset or index ),
 *      - Delete UE from cell->tIUeLstCp[n] if present  
 *       - Update UE with the configured values.
 *       - Compute and Update next occurance of SR Tranmission instance.
 *       - Update the  SR offset and SR perodicity information
 *       - Add Ue to cell->tIUeLstCp[n]  
 *      - If SR is configured modified(disabled),
 *      - Delete UE from cell->tIUeLstCp[n] if present  
 *         - Update next occurance of SR Tranmission instance to "INVALID".
 *         - Update the  SR offset and SR perodicity information "INVALID"
 *      - else
 *        - ROK 
 *
 *
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeCfg     *ueCfg
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
S16 rgSCHCfgSrUeReCfg
(
RgSchCellCb    *cellCb, 
RgSchUeCb      *ueCb, 
RgrUeSrCfg     *srCfg
)
{

   /* Fix : syed Incorrect check for SR RECFG */
   if((srCfg->type == RGR_SCH_SR_SETUP) &&
         (ueCb->srCb.srCfg.type != RGR_SCH_SR_SETUP))
   {
      /*
         1. Present is Release(Disable) state,  Recfg is allowing SETUP(Enable) 
         */
      rgSCHCfgSrUeCfg(cellCb, ueCb, srCfg);
   } 
   else if((srCfg->type == RGR_SCH_SR_SETUP) &&
         (ueCb->srCb.srCfg.type == RGR_SCH_SR_SETUP))
   {

      /*   2. Present is SETUP(Eanble) state,  Recfg is modifying SETUP(Enable) 

           2.1 Delete UE from the cqiList 
           2.2 Compute Next occurance Transmission instace 
           */      
      if(ueCb->srCb.nSrTrIdx != RG_SCH_INVALID_IDX )
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->srCb.nSrTrIdx].srLst, 
               &ueCb->srCb.srLstEnt);
         ueCb->srCb.nSrTrIdx = RG_SCH_INVALID_IDX;
      }
      rgSCHCfgSrUeCfg(cellCb, ueCb, srCfg);

   }
   else
   {
      /* 3. Present is SETUP(Enable) state,  Recfg is Release(Disable) */
      rgSCHCfgSrUeCfg(cellCb, ueCb, srCfg);
   }

   return ROK;
}

/* @brief Handles  ACQI Re-configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgAcqiUeReCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE Re configuration. 
 *      It shall do the validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific ACQI  related re configuration, 
 *          - Check if the ACQI Mode has been changed from the
 *             existing Configuration.
 *     - If the configuration has been changed,
 *          - Call Aperiodic Config function to change the config
 *      - else
 *        - ROK 
 *
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeAprdDlCqiCfg   *acqiCfg
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
S16 rgSCHCfgAcqiUeReCfg
(
RgSchCellCb    *cellCb, 
RgSchUeCb      *ueCb, 
RgrUeAprdDlCqiCfg   *acqiCfg,
CmLteUeCategory   ueCat
)
{

   return (rgSCHCfgACqiUeCfg(cellCb,ueCb,(RG_SCH_CMN_GET_ACQICB(ueCb,cellCb))
                                    ,ueCb->mimoInfo.txMode, acqiCfg, ueCat));

}

/*****************************************************************
 *   UE PCQI, RI, SRS and SR Configuration  Delete                *
 *                                                                *
 *****************************************************************/

/* @brief Free Periodic CQI/PMI/RI, SRS and SR transmission instance 
 * related data structures of this UE from CellCb 
 *
 * @details
 *
 *     Function : rgSCHCfgPCqiSrsSrUeDel
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at Ue deletion.
 *
 *     Processing Steps:
 *      - For SRS Transmission Instance
 *       - if (srsTxInst!= RG_INVALID)
 *        - Remove from the cellCb->tIUeLstCp[srsTxInst*3+2]
 *       - else
 *        - Nothing to do
 *      - For SR Transmission Instance
 *       - if (srTxInst!= RG_INVALID)
 *        - Remove from the cellCb->tIUeLstCp[srTxInst*3+1]
 *       - else
 *        - Nothing to do
 *      - For Periodic CQI/PMI RI  Transmission Instance
 *       - if (pCqiTxInst!= RG_INVALID)
 *        - Remove from the cellCb->tIUeLstCp[srTxInst*3+0]
 *       - else
 *        - Nothing to do
 *      - Return ROK
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *
 *  @return  S16
 *      -# ROK
 */
S16 rgSCHCfgPCqiSrsSrUeDel 
(
RgSchCellCb      *cellCb,
RgSchUeCb        *ueCb
)
{
#ifdef LTE_ADV
   uint32_t cellIdx;
   uint32_t sCellCnt = 0;
#endif
   RgSchUePCqiCb *cqiRiCb = NULLP;

   cqiRiCb = RG_SCH_GET_UE_CELL_CQI_CB(ueCb,cellCb);



   /* Delete SRS Transmission Instance  */
   if (ueCb->srsCb.nSrsTrIdx != RG_SCH_INVALID_IDX)
   {
      cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->srsCb.nSrsTrIdx].srsLst,
            &ueCb->srsCb.srsLstEnt); 
      ueCb->srsCb.nSrsTrIdx = RG_SCH_INVALID_IDX;
   }

   /* Delete SR Transmission Instance  */
   if (ueCb->srCb.nSrTrIdx != RG_SCH_INVALID_IDX)
   {
      cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->srCb.nSrTrIdx].srLst,
            &ueCb->srCb.srLstEnt); 
      ueCb->srCb.nSrTrIdx = RG_SCH_INVALID_IDX;
   }

   /* Delete Periodic CQI/PMI  Transmission Instance  */
   if (cqiRiCb->nCqiTrIdx != RG_SCH_INVALID_IDX)
   {
      cmLListDelFrm(&cellCb->pCqiSrsSrLst[cqiRiCb->nCqiTrIdx].cqiLst, 
            &cqiRiCb->cqiLstEnt); 
      cqiRiCb->nCqiTrIdx = RG_SCH_INVALID_IDX;

      /* Delete Periodic  RI  Transmission Instance  */

      if (cqiRiCb->nRiTrIdx != RG_SCH_INVALID_IDX)
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[cqiRiCb->nRiTrIdx].riLst, 
               &cqiRiCb->riLstEnt); 
         RG_SCH_RECORD(&cqiRiCb->histElem,RGSCH_ACTION_DEL,
            &cellCb->pCqiSrsSrLst[cqiRiCb->nRiTrIdx].riLst);
         cqiRiCb->nRiTrIdx = RG_SCH_INVALID_IDX;
      }
   }

#ifdef LTE_ADV
   for (cellIdx =1; cellIdx <= RG_SCH_MAX_SCELL; cellIdx++)
   {
      /* If a serving cell is configured */
      if(ueCb->cellInfo[cellIdx] != NULLP)
      {
         /* If the serving cell is in ACTIVE state and 
            If it is not the same serving cell as cqiRiCb for which 
            collision is being checked */
         cqiRiCb = &ueCb->cellInfo[cellIdx]->cqiCb;
         /* Delete Periodic CQI/PMI  Transmission Instance  */
         if (cqiRiCb->nCqiTrIdx != RG_SCH_INVALID_IDX)
         {
            cmLListDelFrm(&cellCb->pCqiSrsSrLst[cqiRiCb->nCqiTrIdx].cqiLst, 
                  &cqiRiCb->cqiLstEnt); 
            cqiRiCb->nCqiTrIdx = RG_SCH_INVALID_IDX;

            /* Delete Periodic  RI  Transmission Instance  */

            if (cqiRiCb->nRiTrIdx != RG_SCH_INVALID_IDX)
            {
               cmLListDelFrm(&cellCb->pCqiSrsSrLst[cqiRiCb->nRiTrIdx].riLst, 
                     &cqiRiCb->riLstEnt); 
               RG_SCH_RECORD(&cqiRiCb->histElem,RGSCH_ACTION_DEL,
                  &cellCb->pCqiSrsSrLst[cqiRiCb->nRiTrIdx].riLst);
               cqiRiCb->nRiTrIdx = RG_SCH_INVALID_IDX;
            }
         }
         sCellCnt++;
         /* If all of the num of configured scells are checked then break */
         if (sCellCnt == ueCb->numSCells)
         {
            break;
         }   
      }
   }
#endif

   return ROK;
}


/* @brief Search the cfgIdx in given table and retrive periodicity & offset
 * @details
 *
 *     Function : rgSCHUtlGetCfgPerOff 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by Cfg Module 
 *
 *     Processing Steps:
 *          binary search for given entry in table
 *          find the periodicty, offset for a given config index from the table
 *      - Return ROK
 *  @param[in]  RgSchPerTbl tbl
 *  @param[in]  uint16_t      cfgIdx
 *  @param[out] uint16_t     *peri 
 *  @param[out] uint16_t     *offset 
 *
 *  @return  S16
 *      -# ROK
 *
 */
S16 rgSCHUtlGetCfgPerOff
(
RgSchPerTbl   tbl,
uint16_t      cfgIdx, 
uint16_t      *peri, 
uint16_t      *offset
)
{
   uint8_t     mid;
   uint8_t     min = 0;
   uint8_t     max = 0;
   const RgSchUePCqiSrsSrCfgIdxTbl* table;
  
   /* Added the function instead of the MACRO to get the 
    * periodicity table */
   table = rgSCHCfgUtlGetPcqiSrsSrRiTbl ( tbl,&min,&max );
   do{
      mid = (min+max)/2;
      if (( cfgIdx >= table[mid].min) &&
            ( cfgIdx <= table[mid].max))
      {
         *peri = table[mid].peri;
         *offset = cfgIdx - table[mid].offset;
         break;
      } 

      if ( cfgIdx > table[mid].min)
      {
         min = mid+1;
      }
      else
      {
         max = mid-1;
      }

   }while( min <= max );

   return ROK;
} 


/***********************************************************
 *
 *     Func : rgSCHCfgUtlFetchAcqiBitSz
 *
 *
 *     Desc : Fetch the CQI/PMI bits for a UE based on the mode and store them 
 *                for decoding. 
 *
 *     Ret  : Void
 *            ROK - void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHCfgUtlFetchAcqiBitSz
(
RgSchUeACqiCb  *acqiCb,
uint8_t        numTxAnt,
uint8_t*       cqiPmiSzR1,
uint8_t*       cqiPmiSzRn1
)
{

   uint32_t    confRepMode;

   confRepMode = acqiCb->aCqiCfg.aprdModeEnum;
   switch(confRepMode)
   {
      case RGR_APRD_CQI_MOD12:
         {
            if(numTxAnt == 2)
            {
               *cqiPmiSzR1 = 4 + 2*acqiCb->N;
               *cqiPmiSzRn1 = 8+ acqiCb->N;
            }
            else if(numTxAnt == 4)
            {
               *cqiPmiSzR1 = 4 + 4*acqiCb->N;
               *cqiPmiSzRn1 = 8 + 4*acqiCb->N;
            }
         }
         break;

      case RGR_APRD_CQI_MOD20:
         {
            *cqiPmiSzR1 = 6 + acqiCb->L;
            *cqiPmiSzRn1 = 6  + acqiCb->L;
         }
         break;

      case RGR_APRD_CQI_MOD22:
         {
            if(numTxAnt == 2)
            {
               *cqiPmiSzR1 = 10 + acqiCb->L;
               *cqiPmiSzRn1 = 14 + acqiCb->L;
            }
            else if(numTxAnt == 4)
            {
               *cqiPmiSzR1 = 14 + acqiCb->L;
               *cqiPmiSzRn1 = 20 + acqiCb->L;
            }
         }
         break;

      case RGR_APRD_CQI_MOD30:
         {
            *cqiPmiSzR1 = 4 + 2*acqiCb->N;
            *cqiPmiSzRn1 = 4 + 2*acqiCb->N;
         }
         break;

      case RGR_APRD_CQI_MOD31:
         {
            if(numTxAnt == 2)
            {
               *cqiPmiSzR1 = 6 + 2*acqiCb->N;
               *cqiPmiSzRn1 = 9 + 4*acqiCb->N;
            }
            else if(numTxAnt == 4)
            {
               *cqiPmiSzR1 = 8 + 2*acqiCb->N;
               *cqiPmiSzRn1 = 12 + 4*acqiCb->N;
            }
         }
         break;
      default:
         break;
   }
   return;
}
/* Added the function rgSCHCfgUtlGetPcqiCrsSrRiTbl to be used
 * instead of the MACRO RG_SCH_GET_PERIODICITY_TBL */
/***********************************************************
 *
 *     Func : rgSCHCfgUtlGetPcqiCrsSrRiTbl
 *
 *
 *     Desc :  Get the Srs Cqi Crs Ri Table 
 *
 *     Ret  : Void
 *            ROK - void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/

static const RgSchUePCqiSrsSrCfgIdxTbl *  rgSCHCfgUtlGetPcqiSrsSrRiTbl
(
RgSchPerTbl tblType, 
uint8_t* min, 
uint8_t* max
)
{
    const RgSchUePCqiSrsSrCfgIdxTbl * table;
    
    switch (tblType)
    {

        case RG_SCH_FDD_PCQI_TBL:
        {
            table = rgSchUePCqiCfgIdxFddTbl;
            * min = 0;
            * max=RG_SCH_CQIPMI_CFGIDX_MAX_FDD;
            break;
        }
        case RG_SCH_TDD_PCQI_TBL:
        {
            table = rgSchUeCqiPmiCfgIdxTddTbl;
            * min = 0;
            * max=RG_SCH_CQIPMI_CFGIDX_MAX_TDD;
            break;
        }
        case RG_SCH_RI_TBL:
        {
            table = rgSchUeRiCfgIdxTbl;
            * min = 0;
            * max=RG_SCH_RI_CFGIDX_MAX;
            break;
        }
        case RG_SCH_FDD_SRS_TBL:
        {
            table = rgSchUeSrsCfgIdxFddTbl;
            * min = 0;
            * max=RG_SCH_SRS_ISRS_INDX_MAX_FDD;
            break;
        }
        case RG_SCH_TDD_SRS_TBL:
        {
            table = rgSchUeSrsCfgIdxTddTbl;
            * min = 0;
            * max=RG_SCH_SRS_ISRS_INDX_MAX_TDD;
            break;
        }
        case RG_SCH_SR_TBL:
        {
            table = rgSchUeSrCfgIdxTbl;
            * min = 0;
            * max=RG_SCH_ISR_INDX_MAX;
            break;
        }
        default:
        {
            table = (const RgSchUePCqiSrsSrCfgIdxTbl *) 0;
            * min = 0;
            * max = 0;
            break;
        }

    }
    return  ( table );
}
/* #endif */
#endif /* TFU_UPGRADE */

/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxUeCfg
 *
 *
 *     Desc : Validates UE DRX Timers Configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgVldtDrxUeCfg
(
RgSchCellCb       *cell,
RgrUeDrxCfg       *ueDrxCfg
)
{

   if (ueDrxCfg->isDrxEnabled == FALSE)
   {
      return ROK;
   }

#ifdef LTEMAC_R9
   if ( ueDrxCfg->cqiMask.pres )
   {
      if ( ueDrxCfg->cqiMask.val != RGR_DRX_SETUP )
      {
#ifdef ALIGN_64BIT
         DU_LOG("\nERROR  -->  SCH : Invalid cqiMask configuration (%d)",
                  ueDrxCfg->cqiMask.val);
#else
         DU_LOG("\nERROR  -->  SCH : Invalid cqiMask configuration(%ld)",
                  ueDrxCfg->cqiMask.val);
#endif
         return RFAILED;
      }
   }
#endif /*LTEMAC_R9*/
#ifdef EMTC_ENABLE
   if(ueDrxCfg->isEmtcUe)
   {
      if(ueDrxCfg->drxOnDurTmrR13Pres)
      {
         if ( rgSCHEmtcCfgVldtDrxOnDurCfg(ueDrxCfg->drxOnDurTmr) != ROK )
         {
            DU_LOG("\nERROR  -->  SCH : Invalid onDurTimer configuration(%d)",
               ueDrxCfg->drxOnDurTmr);
            return RFAILED;
         }
       }
         else
         {
            if (rgSCHCfgVldtDrxOnDurCfg(ueDrxCfg->drxOnDurTmr) != ROK )
            {
               DU_LOG("\nERROR  -->  SCH : Invalid onDurTimer configuration(%d) for EMTC",
               ueDrxCfg->drxOnDurTmr);
               return RFAILED;
            }

         }
    }
   else
#endif
   {
      if ( rgSCHCfgVldtDrxOnDurCfg(ueDrxCfg->drxOnDurTmr) != ROK )
      {
         DU_LOG("\nERROR  -->  SCH : Invalid onDurTimer configuration(%d) for EMTC",
               ueDrxCfg->drxOnDurTmr);
         return RFAILED;
      }
   }

   if ( rgSCHCfgVldtDrxInActvCfg(ueDrxCfg->drxInactvTmr) != ROK )
   {
      DU_LOG("\nERROR  -->  SCH : Invalid Inactivity configuration(%d)",
               ueDrxCfg->drxInactvTmr);
      return RFAILED;
   }
#ifdef EMTC_ENABLE
   if(ueDrxCfg->isEmtcUe)
   {
      if(ueDrxCfg->drxRetxTmrR13Pres)
      {
         if ( rgSCHEmtcCfgVldtDrxReTxCfg(ueDrxCfg->drxRetxTmr) != ROK )
         {
            DU_LOG("\nERROR  -->  SCH : Invalid DrxReTX configuration(%d) for EMTC",
               ueDrxCfg->drxRetxTmr);
            return RFAILED;
         }
      }
         else
         {
            if (rgSCHCfgVldtDrxReTxCfg(ueDrxCfg->drxRetxTmr) != ROK )
            {
               DU_LOG("\nERROR  -->  SCH : Invalid DrxReTX configuration(%d)",
               ueDrxCfg->drxRetxTmr);
               return RFAILED;
            }

         }
    }
   else
#endif
   {
      if ( rgSCHCfgVldtDrxReTxCfg(ueDrxCfg->drxRetxTmr) != ROK )
      {
         DU_LOG("\nERROR  -->  SCH : Invalid DrxReTX configuration(%d)",
               ueDrxCfg->drxRetxTmr);
         return RFAILED;
      }
   }
#ifdef EMTC_ENABLE
   if(ueDrxCfg->isEmtcUe)
   {
      if ( rgSCHEmtcCfgVldtDrxUlReTxCfg(ueDrxCfg->emtcDrxUlRetxTmr) != ROK )
      {
         DU_LOG("\nERROR  -->  SCH : Invalid DrxReTX configuration(%d) for EMTC",
               ueDrxCfg->drxRetxTmr);
         return RFAILED;
      }

   }
#endif

   if ( rgSCHCfgVldtDrxLngCycCfg(ueDrxCfg->drxLongCycleOffst) != ROK )
   {
      DU_LOG("\nERROR  -->  SCH : Invalid LongCycle configuration");
      return RFAILED;
   }

   if ( ueDrxCfg->drxLongCycleOffst.longDrxCycle < ueDrxCfg->drxOnDurTmr ) 
   {
      DU_LOG("\nERROR  -->  SCH : Invalid combination of Long DRX Cycle " 
              " and onDuration timer values");
      return RFAILED;
   }

#ifdef LTE_TDD
   if( rgSCHCfgVldtTddDrxCycCfg(cell, ueDrxCfg->drxLongCycleOffst.longDrxCycle, 
       ueDrxCfg->drxOnDurTmr, ueDrxCfg->drxLongCycleOffst.drxStartOffst) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Invalid combination of Long DRX Cycle " 
              " and onDuration timer values");
      return RFAILED;
   }
#endif
  
   if( TRUE == ueDrxCfg->drxShortDrx.pres )
   { 
      if ( ueDrxCfg->drxShortDrx.shortDrxCycle < ueDrxCfg->drxOnDurTmr )
      {
         DU_LOG("\nERROR  -->  SCH : Invalid combination of short DRX "
                 "Cycle and onDuration timer values");
         return RFAILED;
      }

      if ( (ueDrxCfg->drxLongCycleOffst.longDrxCycle %
               ueDrxCfg->drxShortDrx.shortDrxCycle) != 0)
      {
         DU_LOG("\nERROR  -->  SCH : Long DRX cycle is not multiple of "
                 "short DRX cycle");
         return RFAILED;
      }

      if ( rgSCHCfgVldtDrxShrtCycCfg(ueDrxCfg->drxShortDrx) != ROK )
      {
         DU_LOG("\nERROR  -->  SCH : Invalid Short Cycle configuration");
         return RFAILED;
      }

#ifdef LTE_TDD
      if( rgSCHCfgVldtTddDrxCycCfg(cell, ueDrxCfg->drxShortDrx.shortDrxCycle, 
             ueDrxCfg->drxOnDurTmr, 
             ueDrxCfg->drxLongCycleOffst.drxStartOffst % 
             ueDrxCfg->drxShortDrx.shortDrxCycle) != ROK)
      { 
         DU_LOG("\nERROR  -->  SCH : Invalid combination of Long DRX Cycle " 
                 " and onDuration timer values");
         return RFAILED;
      }
#endif
   }
   
   return ROK;
}/*rgSCHCfgVldtDrxUeCfg*/
      
/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxOnDurCfg
 *
 *
 *     Desc : Validates UE DRX On Duration configuration 
 *            recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgVldtDrxOnDurCfg
(
uint8_t      onDurTmr 
)
{

   switch ( onDurTmr )
   {
      case RGR_DRX_PRD_1PSF:
      case RGR_DRX_PRD_2PSF:
      case RGR_DRX_PRD_3PSF:
      case RGR_DRX_PRD_4PSF:
      case RGR_DRX_PRD_5PSF:
      case RGR_DRX_PRD_6PSF:
      case RGR_DRX_PRD_8PSF:
      case RGR_DRX_PRD_10PSF:
      case RGR_DRX_PRD_20PSF:
      case RGR_DRX_PRD_30PSF:
      case RGR_DRX_PRD_40PSF:
      case RGR_DRX_PRD_50PSF:
      case RGR_DRX_PRD_60PSF:
      case RGR_DRX_PRD_80PSF:
      case RGR_DRX_PRD_100PSF:
      case RGR_DRX_PRD_200PSF:
         break;

      default:
         {
            return RFAILED;
         }
   }/*switch(onDurTmr)*/

   return ROK;
}/*rgSCHCfgVldtOnDurCfg*/

/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxInActvCfg
 *
 *
 *     Desc : Validates UE DRX InActivity configuration 
 *            recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgVldtDrxInActvCfg(uint16_t inActvTmr)
{
   switch ( inActvTmr )
   {
      case RGR_DRX_PRD_1PSF:
      case RGR_DRX_PRD_2PSF:
      case RGR_DRX_PRD_3PSF:
      case RGR_DRX_PRD_4PSF:
      case RGR_DRX_PRD_5PSF:
      case RGR_DRX_PRD_6PSF:
      case RGR_DRX_PRD_8PSF:
      case RGR_DRX_PRD_10PSF:
      case RGR_DRX_PRD_20PSF:
      case RGR_DRX_PRD_30PSF:
      case RGR_DRX_PRD_40PSF:
      case RGR_DRX_PRD_50PSF:
      case RGR_DRX_PRD_60PSF:
      case RGR_DRX_PRD_80PSF:
      case RGR_DRX_PRD_100PSF:
      case RGR_DRX_PRD_200PSF:
      case RGR_DRX_PRD_300PSF:
      case RGR_DRX_PRD_500PSF:
      case RGR_DRX_PRD_750PSF:
      case RGR_DRX_PRD_1280PSF:
      case RGR_DRX_PRD_1920PSF:
      case RGR_DRX_PRD_2560PSF:
         break;

      default:
         {
            return RFAILED;
         }
   }/*switch(InactvTmr)*/

   return ROK;
}/*rgSCHCfgVldtDrxInActvCfg*/

/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxReTxCfg
 *
 *
 *     Desc : Validates DRX ReTx timer  configuration 
 *            recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgVldtDrxReTxCfg(uint8_t reTxTmr)
{
   switch ( reTxTmr )
   {
      case RGR_DRX_PRD_1PSF:
      case RGR_DRX_PRD_2PSF:
      case RGR_DRX_PRD_4PSF:
      case RGR_DRX_PRD_6PSF:
      case RGR_DRX_PRD_8PSF:
      case RGR_DRX_PRD_16PSF:
      case RGR_DRX_PRD_24PSF:
      case RGR_DRX_PRD_33PSF:
         break;

      default:
         {
            return RFAILED;
         }
   }/*switch(drxRetxTmr)*/

   return ROK;
}/*rgSCHCfgVldtDrxReTxCfg*/

/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxShrtCycCfg
 *
 *
 *     Desc : Validates DRX Short Cycle timer  configuration 
 *            recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgVldtDrxLngCycCfg(RgrDrxLongCycleOffst lngCycleOffst)
{
   if ( rgSCHCfgVldtDrxLngCyclTmrs(lngCycleOffst.longDrxCycle) != ROK )
   {
      return RFAILED;
   }

   if ( lngCycleOffst.drxStartOffst >= lngCycleOffst.longDrxCycle ) 
   {
      return RFAILED;  
   }

   return ROK;
}/*rgSCHCfgVldtDrxLngCycCfg*/

/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxLngCyclTmrs
 *
 *
 *     Desc : Validates DRX Long Cycle timer values 
 *            recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgVldtDrxLngCyclTmrs(uint16_t val)
{

   switch ( val )
   {
      case RGR_DRX_PRD_10SF:
      case RGR_DRX_PRD_20SF:
      case RGR_DRX_PRD_32SF:
      case RGR_DRX_PRD_40SF:
      case RGR_DRX_PRD_64SF:
      case RGR_DRX_PRD_80SF:
      case RGR_DRX_PRD_128SF:
      case RGR_DRX_PRD_160SF:
      case RGR_DRX_PRD_256SF:
      case RGR_DRX_PRD_320SF:
      case RGR_DRX_PRD_512SF:
      case RGR_DRX_PRD_640SF:
      case RGR_DRX_PRD_1024SF:
      case RGR_DRX_PRD_1280SF:
      case RGR_DRX_PRD_2048SF:
      case RGR_DRX_PRD_2560SF:
         break;

      default:
         {
            return RFAILED;
         }
   }/*switch(longDrxCycle)*/

   return ROK;
}/*rgSCHCfgVldtDrxLngCyclTmrs*/

/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxShrtCycCfg
 *
 *
 *     Desc : Validates DRX Short Cycle timer  configuration 
 *            recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgVldtDrxShrtCycCfg(RgrDrxShortDrx shrtCycCfg )
{
   switch(shrtCycCfg.shortDrxCycle)
   {
      case RGR_DRX_PRD_2SF:
      case RGR_DRX_PRD_5SF:
      case RGR_DRX_PRD_8SF:
      case RGR_DRX_PRD_10SF:
      case RGR_DRX_PRD_16SF:
      case RGR_DRX_PRD_20SF:
      case RGR_DRX_PRD_32SF:
      case RGR_DRX_PRD_40SF:
      case RGR_DRX_PRD_64SF:
      case RGR_DRX_PRD_80SF:
      case RGR_DRX_PRD_128SF:
      case RGR_DRX_PRD_160SF:
      case RGR_DRX_PRD_256SF:
      case RGR_DRX_PRD_320SF:
      case RGR_DRX_PRD_640SF:
         break;

      default:
         {
            return RFAILED;
         }

   }/*switch(shortDrxCycle)*/

   if ( (shrtCycCfg.drxShortCycleTmr < RGR_DRX_SHRTCYCLE_MIN) ||
         (shrtCycCfg.drxShortCycleTmr > RGR_DRX_SHRTCYCLE_MAX)
      )
   {
      return RFAILED;
   }

   return ROK;
}

/**
 * @brief Handler for TA related UE Reconfiguration.
 *
 * @details
 *
 *     Function : rgSCHCfgUeTaRecfg
 *     
 *     This function shall fetch the TA timer related information into the
 *     respective ueCb from the UE configuration as provided by the 
 *     upper layers. 
 *           
 *  @param[in]  RgSchCellCb   *cell 
 *  @param[in]  RgSchUeCb     *ueCb 
 *  @param[in]  RgrUeCfg      *ueCfg 
 *  @param[out] RgSchErrInfo  *errInfo 
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static Void rgSCHCfgUeTaRecfg
(
RgSchCellCb       *cell,
RgSchUeCb         *ueCb,
RgrUeRecfg        *ueCfg,
RgSchErrInfo      *err
)
{
   UNUSED(err);

   /* Update the TA related information */
   
   if (ueCfg->ueTaTmrRecfg.pres)
   {
      /* Configuring taTmr with 30 deficit, to enable eNodeB sending
       * TA command before the expiry of TA at UE. Also considering for 
       * possible retx for this TA command */
      /* [ccpu00121813]-ADD-Added chk if tatmr val > 30 */
      if(ueCfg->ueTaTmrRecfg.taTmr > 30)
      {
         ueCb->dl.taCb.cfgTaTmr = ueCfg->ueTaTmrRecfg.taTmr - 30;
      }
       /* If TA Timer is running. Stop it and then start it*/
      if (ueCb->taTmr.tmrEvnt != TMR_NONE)
      {
         rgSCHTmrStopTmr(cell, ueCb->taTmr.tmrEvnt, ueCb);
         rgSCHTmrStartTmr(cell, ueCb, RG_SCH_TMR_TA, ueCb->dl.taCb.cfgTaTmr);
      }
      else
      {
         rgSCHTmrStartTmr(cell, ueCb, RG_SCH_TMR_TA, ueCb->dl.taCb.cfgTaTmr);
      }
   }
   return;
}  /* rgSCHCfgUeTaRecfg */

/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#if ((defined (RGR_CQI_REPT)) && (defined (RGR_V2)))
/***********************************************************
 *
 *     Func : rgSCHCfgUeCqiReptReCfg
 *
 *
 *     Desc : Reconfiguration of PUSH N CQI Reporting 
 *
 *     Ret  : RFAILED in case of failure
 *            ROK if success
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHCfgUeCqiReptReCfg
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgrUeRecfg     *ueRecfg
)
{
   S16 retVal;

   /* Check has it been disabled */
   if(ueRecfg->ueCqiReptCfg.numColltdCqiRept)
   {
    /* Check if we need to send CQI reports collagted so far and send if so */
      if(ue->schCqiInfo.cqiCount >=
              ueRecfg->ueCqiReptCfg.numColltdCqiRept)
      {
         RgrStaIndInfo *staInfo;
      /* if yes, Send StaInd to RRM */
         retVal = rgSCHUtlAllocSBuf (cell->instIdx,(Data**)&staInfo,
                  sizeof(RgrStaIndInfo));
         if(retVal != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Could not allocate memory for sending StaInd OLD CRNTI:%d",ueRecfg->oldCrnti);
            return (retVal);
         }

      /* Fill StaInd for sending collated N CQI rpeort                        */
         rgSCHUtlFillSndStaInd(cell, ue, staInfo,
               ueRecfg->ueCqiReptCfg.numColltdCqiRept);
      }
   }
   else
   {
      ue->schCqiInfo.cqiCount = 0;
   }

   ue->cqiReptCfgInfo.numColltdCqiRept =
      ueRecfg->ueCqiReptCfg.numColltdCqiRept;
   return ROK;
} /* End of rgSCHCfgUeCqiReptReCfg */
#endif /* End of RGR_CQI_REPT */
/*This function Added Ue in ongoing L2 Meas*/
/*LTE_L2_MEAS_PHASE2*/
#ifdef LTE_L2_MEAS
static S16 rgSchAddToL2Meas(RgSchCellCb *cellCb,RgSchDlLcCb *dlLc)
{
   CmLList             *lnk;
   uint16_t       idx;
   RgSchL2MeasCb       *measCb = NULLP;
   lnk = cellCb->l2mList.first;

   while(lnk != NULLP)
   {
      /* Get the MeasCb : RgSchL2MeasCb */
      measCb = (RgSchL2MeasCb *)lnk->node;
      if (measCb->measReq.measType & LRG_L2MEAS_AVG_PRB_PER_QCI_DL)
      {
         for(idx = 0;idx < measCb->measReq.avgPrbQciDl.numQci;idx++)
         {
            if(measCb->measReq.avgPrbQciDl.qci[idx] == dlLc->qciCb->qci)
            {
               break; /*exit from for loop*/
            } 
         }	
         if(idx  == measCb->measReq.avgPrbQciDl.numQci)
         {
            measCb->measReq.avgPrbQciDl.qci[measCb->measReq.avgPrbQciDl.numQci++] = dlLc->qciCb->qci;
         }		
      }
      lnk = lnk->next;
   }/*End of while loop*/

   return ROK;
}
#endif
#ifdef LTE_ADV
/**
 * @brief UE SCell Buffer Free 
 *
 * @details
 *
 *     Function : rgSCHSCellFreeBuf
 *
 *     This functions will free allocated memory 
 *     for UE secondart cellCB
 *     
 *
 *  @param[in]  Inst         inst 
 *  @param[in]  RgSchUeCb    *ue
 *  @param[int] RgrUeRecfg   *ueRecfg
 *  @param[out] uint8_t           idx 
 *  @return  VOID
 **/
Void rgSCHSCellFreeBuf
(
Inst         inst,         
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg,
uint8_t      idx
)
{
   RgSchUeCellInfo *sCellInfo = NULLP;
   RgrUeSecCellCfg *sCellInfoRecfg = NULLP;
   

   for(uint8_t i = 0; i <= idx; i++)
   {
      sCellInfoRecfg = &ueRecfg->ueSCellCfgInfo.ueSCellDedCfg[i]; 
      sCellInfo = ue->cellInfo[(sCellInfoRecfg->sCellIdx)];
      
      if (NULLP != sCellInfo)
      {
         rgSCHUtlFreeSBuf(inst, (Data**)&(sCellInfo),
               sizeof(RgSchUeCellInfo));
         ue->cellInfo[(sCellInfoRecfg->sCellIdx)] = NULLP;

      } 
   }
   return;
}
#endif
/**********************************************************************
 
         End of file
**********************************************************************/
