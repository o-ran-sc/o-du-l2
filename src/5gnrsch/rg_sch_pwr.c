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
 
     Desc:     C source code for power control functionality
 
     File:     rg_sch_pwr.c
 
**********************************************************************/

/** @file rg_sch_pwr.c
@brief This module handles schedulers' power control functionality
*/

/* header include files -- defines (.h) */
#include "common_def.h"
#include "lrg.h"
#include "rgr.h"
#include "rgm.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch_err.h"
#include "rg_sch.h"
#include "rg_sch_cmn.h"

/* header/extern include files (.x) */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */
#include "rg_sch_cmn.x"


/* Current specs have 23 dBm as max tx power capability for UEs */
#define RG_SCH_PWR_UE_MAX_PWR    23

#define RG_SCH_REF_PCMAX         0xFF

#define RG_SCH_CMN_GET_UL_UE(_ue,_cell) (&(((RgSchCmnUe *)((_ue->cellInfo[_ue->cellIdToCellIdxMap\
               [RG_SCH_CELLINDEX(_cell)]])->sch))->ul))
#define RG_SCH_PWR_GETUEPWR(_ue, _cell) &(((RgSchCmnUe *)((_ue->cellInfo[_ue->cellIdToCellIdxMap\
               [RG_SCH_CELLINDEX(_cell)]])->sch))->ul.ulPwrCb)
#define RG_SCH_PWR_GETCELLPWR(cell) &((RgSchCmnCell *)((cell)->sc.sch))->ul.ulPwrCb


typedef S8 RgSchCmnUlPwrCqiToPwrTbl[RG_SCH_CMN_UL_NUM_CQI];

static RgSchCmnUlPwrCqiToPwrTbl rgSchPwrCqiToPwrTbl;

/* This table maps a given number of RBs (given by array index)
 * to the power in dB that these many RBs map to. */
const uint8_t rgSchPwrRbToPwrTbl[111] = { 0,    /* First entry is dummy */
   0,  3,  4,  6,  7,  7,  8,  9,  9,  10,
   10, 10, 11, 11, 11, 12, 12, 12, 12, 13,
   13, 13, 13, 13, 14, 14, 14, 14, 14, 14,
   15, 15, 15, 15, 15, 15, 15, 15, 16, 16,
   16, 16, 16, 16, 16, 16, 16, 16, 17, 17,
   17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
   17, 18, 18, 18, 18, 18, 18, 18, 18, 18,
   18, 18, 18, 18, 18, 18, 18, 19, 19, 19,
   19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
   19, 19, 19, 19, 19, 19, 19, 20, 20, 20,
   20, 20, 20, 20, 20, 20, 20, 20, 20, 20
};


/* This table maps power (in dB) to number of RBs */
/* The array size comes from max power in rgSchPwrRbToPwrTbl */
const uint8_t rgSchPwrToRbTbl[20+1] = {
  1, 1, 2, 2, 3, 4, 5, 6, 7, 9, 11,
  13, 17, 21, 26, 33, 41, 52, 65, 82, 103 
};



static S8 rgSCHPwrGetCqiPwr ARGS((
         uint8_t                  cqi
         ));
static S8 rgSCHPwrGetCqiPwrForUe ARGS((
         RgSchCellCb          *cell,
         RgSchUeCb          *ue,
         uint8_t                  cqi
         ));
static S8 rgSCHPwrCalcEfficncyPwr ARGS((
         uint32_t                 eff
         ));
static S8 rgSCHPwrGetDelta2FrmCqi ARGS((
         uint8_t                  crntCqi,
         uint8_t                  trgCqi,
         RgSchUeCb           *ue,
         RgSchCellCb         *cell
         ));
static Void rgSCHPwrGetPuschTpc ARGS((
         uint8_t                  isAcc,
         S8                  delta,
         S8                  availPwr,
         uint8_t                 *tpc,
         S8                 *tpcDelta
         ));
static uint8_t rgSCHPwrGetMaxRb ARGS((
         RgSchCellCb          *cell,
         S8                  pwr
         ));
static uint8_t rgSCHPwrRbToPwr ARGS((
         RgSchCellCb          *cell,
         uint8_t                  numRb
         ));
static Void rgSCHPwrSchedPucchRnti ARGS((
         RgSchCellCb          *cell,
         RgSchCmnTpcRntiCb    *cb,
         RgSchPdcch           *pdcch,
         RgSchDlSf            *dlSf,
         Bool                 *sched
         ));
static Void rgSCHPwrPuschCntrl ARGS((
         RgSchCellCb          *cell,
         RgSchUeCb            *ue
         ));
static Void rgSCHPwrPucchCntrl ARGS((
         RgSchCellCb *cell,
         RgSchUeCb   *ue
         ));
static Void rgSCHPwrSchedPuschRnti ARGS((
         RgSchCellCb          *cell,
         RgSchCmnTpcRntiCb    *cb,
         RgSchPdcch           *pdcch,
         RgSchUlSf            *ulSf,
         Bool                 *sched
         ));
static Void rgSCHPwrGetPucchFmt3TpcForUe ARGS((
         RgSchUeCb            *ue,
         uint8_t                   *tpc,
         S8                   *delta
         ));
static Void rgSCHPwrGetPucchFmt3aTpcForUe ARGS((
         RgSchUeCb            *ue,
         uint8_t                   *tpc,
         S8                   *delta
         ));
static Void rgSCHPwrGetPuschFmt3TpcForUe ARGS((
         RgSchUeCb            *ue,
         uint8_t                   *tpc,
         S8                   *delta
         ));
static Void rgSCHPwrGetPuschFmt3aTpcForUe ARGS((
         RgSchUeCb            *ue,
         uint8_t                   *tpc,
         S8                   *delta
         ));
static Void rgSCHPwrGetAcc1bitTpc ARGS((
         S8                    remPwr,
         uint8_t                   *tpc,
         S8                   *delta
         ));
static Void rgSCHPwrGetAcc2bitTpc ARGS((
         S8                   remPwr,
         uint8_t                  *tpc,
         S8                  *delta
         ));
static Void rgSCHPwrGetAbsTpc ARGS((
         S8                   remPwr,
         uint8_t                  *tpc,
         S8                  *delta
         ));
static Void rgSCHPwrOnPucchGrpPwrForUe  ARGS((
         RgSchCellCb          *cell,
         RgSchUeCb            *ue,
         S8                    delta
         ));
static Void rgSCHPwrOnPuschGrpPwrForUe  ARGS((
         RgSchCellCb          *cell,
         RgSchUeCb            *ue,
         S8                    delta
         ));
static Bool rgSCHPwrIsDlUeSched ARGS((
         RgSchCellCb            *cell,
         RgSchUeCb            *ue,
         RgSchDlSf            *sf
         ));
static Bool rgSCHPwrIsUlUeSched ARGS((
         RgSchCellCb          *cell,
         RgSchUeCb            *ue,
         RgSchUlSf            *sf
         ));
static Void rgSCHPwrOnSchedPucchTpc ARGS((
         RgSchCellCb           *cell,
         RgSchUeCb             *ue,
         S8                     delta
         ));
static Void rgSCHPwrOnSchedPuschTpc ARGS((
         RgSchCellCb           *cell,
         RgSchUeCb             *ue
         ));
static S16 rgSCHPwrApplyUePwrCfg  ARGS((
         RgSchCellCb          *cell,
         RgSchUeCb            *ue,
         RgrUeUlPwrCfg        *pwrCfg
         ));
static Void rgSCHPwrUeResetPucch ARGS((
         RgSchCellCb          *cell,
         RgSchUeCb            *ue
         ));
static Void rgSCHPwrUeResetPusch ARGS((
         RgSchCellCb          *cell,
         RgSchUeCb            *ue
         ));
static Void rgSCHPwrOnPuschPwrUpd ARGS((
         RgSchCellCb          *cell,
         RgSchUeCb            *ue
         ));
static Void rgSCHPwrAddRntiToPucchRntiLst  ARGS((
         RgSchCellCb          *cell,
         CmLteRnti             rnti,
         Bool                  isFmt3a
         ));
static Void rgSCHPwrAddRntiToPuschRntiLst  ARGS((
         RgSchCellCb          *cell,
         CmLteRnti             rnti,
         Bool                  isFmt3a
         ));
static Void rgSCHPwrInitTpcRntiCb  ARGS((
         RgSchCmnTpcRntiCb    *cb,
         CmLteRnti             rnti,
         Bool                  isFmt3a
         ));
static RgSchCmnTpcRntiCb* rgSCHPwrGetPucchRntiCb ARGS((
         RgSchCellCb *cell,
         CmLteRnti   tpcRnti
         ));
static RgSchCmnTpcRntiCb* rgSCHPwrGetPuschRntiCb ARGS((
         RgSchCellCb *cell,
         CmLteRnti   tpcRnti
         ));
static Void rgSCHPwrAddUeToPucchTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb,
         RgSchUeCb             *ue
         ));
static Void rgSCHPwrDelUeFrmPucchTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb,
         RgSchUeCb             *ue
         ));
static Void rgSCHPwrRmvSchdUeFrmPucchTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb,
         RgSchUeCb             *ue
         ));
static Void rgSCHPwrRmvSchdUeOnlyFrmPucchTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb,
         RgSchUeCb             *ue
         ));
static Void rgSCHPwrRmvSchdPucchTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb
         ));
static Void rgSCHPwrAddSchdUeToPucchTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb,
         RgSchUeCb             *ue
         ));
static Void rgSCHPwrAddSchdPucchTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb
         ));
static Void rgSCHPwrAddUeToPuschTpcRntiCb ARGS((
         RgSchCmnTpcRntiCb     *cb,
         RgSchUeCb             *ue
         ));
static Void rgSCHPwrAddSchdUeToPuschTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb,
         RgSchUeCb             *ue
         ));
static Void rgSCHPwrDelUeFrmPuschTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb,
         RgSchUeCb             *ue
         ));
static Void rgSCHPwrRmvSchdUeFrmPuschTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb,
         RgSchUeCb             *ue
         ));
static Void rgSCHPwrRmvSchdUeOnlyFrmPuschTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb,
         RgSchUeCb             *ue
         ));
static Void rgSCHPwrAddSchdPuschTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb
         ));
static Void rgSCHPwrRmvSchdPuschTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb
         ));
static S16 rgSCHPwrChkPucchTpcRntiIdx ARGS((
         RgSchCmnTpcRntiCb     *cb,
         uint8_t                     idx
         ));
static S16 rgSCHPwrChkPuschTpcRntiIdx ARGS((
         RgSchCmnTpcRntiCb     *cb,
         uint8_t                     idx
         ));
static S16 rgSCHPwrChkUniqPucchTpcRntiIdx ARGS((
         RgSchCmnTpcRntiCb     *cb,
         uint8_t                     idx
         ));
static S16 rgSCHPwrChkUniqPuschTpcRntiIdx ARGS((
         RgSchCmnTpcRntiCb     *cb,
         uint8_t                     idx
         ));
static S16 rgSCHPwrChkTpcRntiIdx ARGS((
         RgSchCmnTpcRntiCb     *cb,
         uint8_t                     idx
         ));
static S8 rgSCHPwrGetPhValFromPhr ARGS((
         uint8_t                    phr
         ));
static S8 rgSCHPwrGetPCMaxValFromPCMax ARGS((
         uint8_t                    pCMax
         ));

/* local defines */


/**
 * @brief Does power related initialisation (not cell specific).
 *        
 *
 * @details
 *
 *     Function : rgSCHPwrInit
 *
 *     Processing Steps:
 *      - This shall precompute coding efficiency to power
 *        mappings (assuming beta of 1).
 *
 *  @return  Void
 **/
Void rgSCHPwrInit()
{
   uint8_t             idx;

   rgSchPwrCqiToPwrTbl[0] = 0;  /* This should never be used anyway */
   for (idx = 1; idx < RG_SCH_CMN_UL_NUM_CQI; ++idx)
   {
      rgSchPwrCqiToPwrTbl[idx] = rgSCHPwrCalcEfficncyPwr(rgSchCmnUlCqiTbl[idx].eff);
   }
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrGetCqiPwr
 *
 *     Desc : Returns power corresponding to coding efficiency
 *            when beta pusch is assumed 1.
 *
 *     Ret  : uint8_t
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S8 rgSCHPwrGetCqiPwr(uint8_t  cqi)
{

   return (rgSchPwrCqiToPwrTbl[cqi]);
}  /* rgSCHPwrGetCqiPwr */

/***********************************************************
 *
 *     Func : rgSCHPwrGetCqiPwrForUe
 *
 *     Desc : If MCS control is enabled for UE, returns
 *            power corresponding to CQI, else 0.
 *
 *     Ret  : uint8_t
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S8 rgSCHPwrGetCqiPwrForUe(RgSchCellCb *cell,RgSchUeCb *ue,uint8_t  cqi)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, cell);

   if (!uePwr->deltaMcsEnbld)
   {
      return (0);
   }
   return (rgSCHPwrGetCqiPwr(cqi));
}  /* rgSCHPwrGetCqiPwrForUe */

/***********************************************************
 *
 *     Func : rgSCHPwrCalcEfficncyPwr
 *
 *     Desc : Computes power corresponding to a coding
 *            efficiency.
 *
 *     Ret  : uint8_t
 *
 *     Notes: Assumes beta pusch to be 1
 *
 *     File :
 *
 **********************************************************/
static S8 rgSCHPwrCalcEfficncyPwr(uint32_t  eff)
{
   F64          ks = 1.25; /* or F64 */
   F64          tmp = cmPow(2, ks*eff/1024) - 1;

   if (tmp <= 0)
      return (0);
   return ((S8)(10 * cmLog10(tmp)));
}  /* rgSCHPwrCalcEfficncyPwr */


/**
 * @brief Returns TPC to be sent in UL allocation
 *
 * @details
 *
 *     Function : rgSCHPwrPuschTpcForUe
 *
 *     Invoking Module Processing:
 *      - After allocation for UE, this function shall
 *        be invoked to retrieve TPC.
 *      - This assumes that rgSCHPwrGetMaxUlRb() was
 *        invoked prior to final allocation for UE.
 *        
 *     Processing Steps:
 *     - Just return TPC that was determined
 *       earlier.
 *     - After this, do necessary updates.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  uint8_t
 **/
uint8_t rgSCHPwrPuschTpcForUe(RgSchCellCb *cell,RgSchUeCb   *ue)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue,cell);

   UNUSED(cell);

   rgSCHPwrOnSchedPuschTpc(cell, ue);
   return (uePwr->puschTpc);
}

/**
 * @brief Handles Pusch power control for DCI format 0
 *
 * @details
 *
 *     Function : rgSCHPwrGetMaxUlRb
 *
 *     Invoking Module Processing:
 *      - This shall be invoked to determine maximum
 *        number of UL RBs for scheduling.
 *      - This is expected to be invoked every time
 *        priority to attempt at UE allocation. Later
 *        TPC retrieval depends on it.
 *
 *     Processing Steps:
 *     - Returns maximum allowed UL RBs to be granted
 *       after invoking Pusch power control.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
uint8_t rgSCHPwrGetMaxUlRb(RgSchCellCb *cell,RgSchUeCb   *ue)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, cell);

   rgSCHPwrPuschCntrl(cell, ue); /* This stores tpc, delta and maxRb
                                  * in uePwr */
   return (uePwr->maxUlRbs);
}

/**
 * @brief Handles Pusch power control for DCI format 0
 *
 * @details
 *
 *     Function : rgSCHPwrPuschCntrl
 *
 *     Invoking Module Processing:
 *      - This shall be invoked to determine TPC
 *      and maximum number of UL RBs for scheduling
 *      (through DCI format 0).
 *
 *     Processing Steps:
 *     - 'remPuschPwr' is the final delta power that the UE
 *       should apply to get to target CQI.
 *     - The available headroom (availPwr) is determined.
 *     - Power command is given by considering remPuschPwr and
 *       availPwr.
 *     - After factoring in the power command into availPwr, the
 *       maximum number of RBs that can be supported is determined
 *       assuming that UE is going to use transmission efficiency
 *       corresponding to current CQI.
 *     - The results determined in this function are stored
 *       in the UE power control block.
 *     - [Not doing anything of power control of msg3
 *        retransmissions now]
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
static Void rgSCHPwrPuschCntrl(RgSchCellCb *cell,RgSchUeCb   *ue)
{
   RgSchCmnUlUe       *ueUl    = RG_SCH_CMN_GET_UL_UE(ue, cell);
   RgSchCmnUeUlPwrCb  *uePwr   = RG_SCH_PWR_GETUEPWR(ue, cell);
   RgSchCmnUlCell     *cellUl  = RG_SCH_CMN_GET_UL_CELL(cell);
   S8                  delta;
#ifdef TFU_UPGRADE
   uint8_t            cqi     = ueUl->validUlCqi;
   S32                tmp;
#else
   uint8_t            cqi     = ueUl->crntUlCqi[0];
#endif
   Bool               isAcc   = uePwr->isAccumulated;
   uint8_t            tpc;
   S8                 availPwr;
   uint8_t            maxRb;

   UNUSED(cell);

   if (!uePwr->isPhrAvail)
   {
      availPwr = 60; /* setting a large value so that availPwr does
                      * not constrain delta */
   }
   else
   {
      availPwr = uePwr->maxUePwr - uePwr->pwrPerRb;
      availPwr -= rgSCHPwrGetCqiPwrForUe(cell, ue, cqi);
   }
   delta = uePwr->remPuschPwr;
   rgSCHPwrGetPuschTpc(isAcc, delta, availPwr, &tpc, &delta);
   availPwr -= delta;

   maxRb = rgSCHPwrGetMaxRb(cell,availPwr);

   /* Store the results in ue power control block to be used later */
   if(maxRb < cellUl->sbSize)
   {
       maxRb = cellUl->sbSize;
#ifdef TFU_UPGRADE
       if(uePwr->maxPwrDeltaByPhr < 0)
       {
          tmp = ueUl->validUlCqi;
          tmp = tmp + uePwr->maxPwrDeltaByPhr;
          if (tmp < 1 )
          {
              ueUl->validUlCqi = 1;
          }
          else
          {
              ueUl->validUlCqi = tmp;
          }
       }
#endif
   }
   DU_LOG("\nDEBUG  -->  SCH : UEID:%d Output Max Rb (%d), phVal (%d) AvailPwr (%d) ",
         ue->ueId, maxRb, uePwr->phVal, availPwr);
   DU_LOG("\nDEBUG  -->  SCH : UEID:%d pwrPerRb %d remPuschPwr %d", 
         ue->ueId,
         uePwr->pwrPerRb,
         uePwr->remPuschPwr);
   uePwr->delta   = delta;
   uePwr->maxUlRbs = maxRb;
   uePwr->puschTpc = tpc;
   return;
}

/**
 * @brief Returns TPC to be sent in DL allocation
 *
 * @details
 *
 *     Function : rgSCHPwrPucchTpcForUe
 *
 *     Invoking Module Processing:
 *      - After DL allocation for UE, this function shall
 *        be invoked to obtain TPC.
 *
 *     Processing Steps:
 *     - Do Pucch power control processing
 *       and return TPC
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  uint8_t
 **/
uint8_t rgSCHPwrPucchTpcForUe(RgSchCellCb *cell,RgSchUeCb   *ue)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, cell);

   rgSCHPwrPucchCntrl(cell, ue);
   return (uePwr->pucchTpc);
}

/***********************************************************
 *
 *     Func : rgSCHPwrGetDelta2FrmCqi
 *
 *     Desc : Get power to be applied to achieve
 *            target CQI (the power returned is
 *            twice is actual power)
 *
 *     Ret  : S8
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S8 rgSCHPwrGetDelta2FrmCqi(uint8_t crntCqi,uint8_t trgCqi,RgSchUeCb *ue,RgSchCellCb *cell)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, cell);
 
   if (uePwr->isPhrAvail)
   {
      //uePwr->maxPwrDeltaByPhr = uePwr->maxPwrPerRb - uePwr->pwrPerRb - uePwr->remPuschPwr;
      uePwr->maxPwrDeltaByPhr = uePwr->maxPwrPerRb - uePwr->pwrPerRb;
   }
   else
   {
      uePwr->maxPwrDeltaByPhr = 0;	   
   }
   
   if (uePwr->maxPwrDeltaByPhr < 0 && (trgCqi - crntCqi) * 
         RG_SCH_UL_CQI_DB_STEP_2 > 0)
   {
      return (0);
   }
   return (RGSCH_MIN(uePwr->maxPwrDeltaByPhr, 
           (trgCqi - crntCqi) * RG_SCH_UL_CQI_DB_STEP_2));
}  /* rgSCHPwrGetDelta2FrmCqi */

/***********************************************************
 *
 *     Func : rgSCHPwrGetPuschTpc
 *
 *     Desc : Based on whether accumulation is enabled or
 *            not, this returns an applicable power delta
 *            to be applied based on the input delta.
 *
 *     Ret  : S8
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrGetPuschTpc(uint8_t isAcc,S8 delta,S8 availPwr,uint8_t *tpc,S8  *tpcDelta)
{

   delta = RGSCH_MIN(delta, availPwr);

   /* As of now, the functions below possibly cause delta
    * to be breached by 1 only. So calling these as is. */
   if (isAcc)
   {
      rgSCHPwrGetAcc2bitTpc(delta, tpc, tpcDelta);
   }
   else
   {
      rgSCHPwrGetAbsTpc(delta, tpc, tpcDelta);
   }
   return;
}  /* rgSCHPwrGetPuschTpc */

/***********************************************************
 *
 *     Func : rgSCHPwrGetMaxRb
 *
 *     Desc : Get the maximum number of RBs that can be
 *            expected to be supported by the passed
 *            power headroom.
 *
 *     Ret  : uint8_t
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static uint8_t rgSCHPwrGetMaxRb(RgSchCellCb *cell,S8 pwr)
{
   RgSchCmnUlCell  *cellUl;

   cellUl    = RG_SCH_CMN_GET_UL_CELL(cell);
   if (pwr <= 0)
   {
      /* Give 4 RBS so that UE can report changed power status*/
      /* [ccpu00119916] Mod -return 0th index of rgSchPwrToRbTbl when pwr <=0
       *  Change the Macros from RGSCH_MAX_DL_BW to RGSCH_MAX_UL_BW*/
      return (rgSchPwrToRbTbl[0]);
   }
   if (pwr > rgSchPwrRbToPwrTbl[cellUl->maxUlBwPerUe])
   {
      return (cellUl->maxUlBwPerUe);
   }
   return (RGSCH_MIN(cellUl->maxUlBwPerUe,rgSchPwrToRbTbl[(uint8_t)pwr]));
}  /* rgSCHPwrGetMaxRb */

/***********************************************************
 *
 *     Func : rgSCHPwrRbToPwr
 *
 *     Desc : Get the power corresponding to number of RBs
 *
 *     Ret  : uint8_t
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static uint8_t rgSCHPwrRbToPwr(RgSchCellCb *cell,uint8_t numRb)
{
#ifndef NO_ERRCLS
   RgSchCmnUlCell  *cellUl;
#endif
#if (ERRCLASS & ERRCLS_DEBUG)
   cellUl    = RG_SCH_CMN_GET_UL_CELL(cell);
   if (numRb > cellUl->maxUlBwPerUe)
   {
      numRb = cellUl->maxUlBwPerUe;
   }
#endif
   return (rgSchPwrRbToPwrTbl[numRb]);
}  /* rgSCHPwrRbToPwr */


/**
 * @brief Handles Pucch power control for DCI formats 1A/1B/1D/1/2A/2 
 *
 * @details
 *
 *     Function : rgSCHPwrPucchCntrl
 *
 *     Processing Steps:
 *     - Determine 2 bit TPC to be sent using remPucchPwr.
 *     - Update remPucchPwr appropriately
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
static Void rgSCHPwrPucchCntrl(RgSchCellCb *cell,RgSchUeCb   *ue)
{
   S8                     delta;
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, cell);

   rgSCHPwrGetAcc2bitTpc(uePwr->remPucchPwr, &uePwr->pucchTpc, &delta);
   rgSCHPwrOnSchedPucchTpc(cell, ue, delta);
   return;
}

/**
 * @brief Handles group power control for DCI formats 3/3A for Pucch and Pusch
 *
 * @details
 *
 *     Function : rgSCHPwrGrpCntrlPucch
 *
 *     Invoking Module Processing:
 *      - This shall be invoked to do group power control for
 *        all TPC RNTIs for which it is deemed necessary to
 *        do the same (group power control).
 *      - This function should only be invoked after all UEs
 *        have been scheduled for uplink (re)transmissions
 *        requiring DL DCI format in the passed subframe.
 *
 *     Processing Steps:
 *     - For Pucch group power control
 *       - For each TPC-Pucch-RNTI in the pucchGrpPwr List and 
 *         TPC-Pusch-RNTI in the puschGrpPwr List,
 *          - Request for PDCCH, skip if not available
 *          - Form DCI format 3/3A information depending
 *            on the format type of the TPC-RNTI and add it to the sub-frame.
 *          - For each Ue in ueLst of TPC RNTI Cb
 *             - if (fmtType == 3A)
 *              - if((Ue not scheduled DL dci formats)
 *                     && (remPwr >= 2 || remPwr <= -2))
 *                  - Determine TPC. Set puschTpc/pucchTpc.
 *                  - remPwr -= TPC
 *                  - if (remPwr >= -1 && remPwr <= 1)
 *                    - If already added, remove from toBeSchdLst
 *              - else
 *                  - Toggle the remainig power value
 *             - else if (fmtType == 3)
 *              - if((Ue not scheduled DL dci formats)
 *                    && (remPwr))
 *                  - Determine TPC. Set puschTpc/pucchTpc.
 *                  - remPwr -= TPC
 *                  - if (!remPwr)
 *                    - If already added, remove from toBeSchdLst
 *          - if (!toBeSchdUeCnt)
 *             - Remove the tpcRntiCb frm pucchGrpPwr/puschGrpPwr List
 *          - else, Move the tpcRntiCb to end of the list (not doing
 *             this)
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchDlSf    *dlSf
 *  @return  Void
 **/
Void rgSCHPwrGrpCntrlPucch(RgSchCellCb *cell,RgSchDlSf   *dlSf)
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);
   CmLListCp             *lst;
   CmLList               *lnk;
   RgSchPdcch            *pdcch;

   lst = &cellPwr->pucchGrpPwr;
   lnk = lst->first;
   while (lnk && ((pdcch = rgSCHCmnCmnPdcchAlloc(cell, dlSf)) != NULLP))
   {
      RgSchCmnTpcRntiCb      *cb = (RgSchCmnTpcRntiCb *)lnk->node;
      Bool                    sched;
      lnk = lnk->next;
      rgSCHPwrSchedPucchRnti(cell, cb, pdcch, dlSf, &sched);
      if (!sched)
      {
         rgSCHUtlPdcchPut(cell, &dlSf->pdcchInfo, pdcch);
      }
      /* TPC RNTI would not have been removed if needs to
       * be scheduled again */
   }

   return;
}

/**
 * @brief Handles group power control for DCI formats 3/3A for Pusch and Pusch
 *
 * @details
 *
 *     Function : rgSCHPwrGrpCntrlPusch
 *
 *     Invoking Module Processing:
 *      - This shall be invoked to do group power control for
 *        all TPC RNTIs for which it is deemed necessary to
 *        do the same (group power control).
 *      - This function should only be invoked after all UEs
 *        have been scheduled for uplink (re)transmissions
 *        requiring DCI format 0 in the passed subframe.
 *
 *     Processing Steps:
 *     - For Pusch group power control
 *       - For each TPC-Pusch-RNTI in the puschGrpPwr List and 
 *          - Request for PDCCH, skip if not available
 *          - Form DCI format 3/3A information depending
 *            on the format type of the TPC-RNTI and add it to the sub-frame.
 *          - For each Ue in ueLst of TPC RNTI Cb
 *             - if (fmtType == 3A)
 *              - if (Ue not scheduled for dci format 0) and
 *                (remPwr >= 2 || remPwr <= -2))
 *                  - Determine TPC. Set puschTpc/puschTpc.
 *                  - remPwr -= TPC
 *                  - if (remPwr >= -1 && remPwr <= 1)
 *                    - If already added, remove from toBeSchdLst
 *              - else
 *                  - Toggle the remainig power value
 *             - else if (fmtType == 3)
 *              - if((Ue not scheduled for dci format 0) && (remPwr))
 *                  - Determine TPC. Set puschTpc.
 *                  - remPwr -= TPC
 *                  - if (!remPwr)
 *                    - If already added, remove from toBeSchdLst
 *          - if (!toBeSchdUeCnt)
 *             - Remove the tpcRntiCb frm puschGrpPwr/puschGrpPwr List
 *          - else, Move the tpcRntiCb to end of the list (not doing
 *            this now)
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchDlSf    *sf
 *  @return  Void
 **/
Void rgSCHPwrGrpCntrlPusch(RgSchCellCb *cell,RgSchDlSf   *dlSf,RgSchUlSf   *ulSf)
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);
   CmLListCp             *lst;
   CmLList               *lnk;
   RgSchPdcch            *pdcch;

   lst = &cellPwr->puschGrpPwr;
   lnk = lst->first;
   while (lnk && ((pdcch = rgSCHCmnCmnPdcchAlloc(cell, dlSf)) != NULLP))
   {
      RgSchCmnTpcRntiCb      *cb = (RgSchCmnTpcRntiCb *)lnk->node;
      Bool                    sched;
      lnk = lnk->next;
      rgSCHPwrSchedPuschRnti(cell, cb, pdcch, ulSf, &sched);
      if (!sched)
      {
         rgSCHUtlPdcchPut(cell, &dlSf->pdcchInfo, pdcch);
      }
      /* TPC RNTI would not have been removed if needs to
       * be scheduled again */
   }

   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrSchedPucchRnti
 *
 *     Desc : Schedule TPC RNTI to be sent out
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrSchedPucchRnti(RgSchCellCb *cell,RgSchCmnTpcRntiCb *cb,RgSchPdcch *pdcch,RgSchDlSf *dlSf,Bool *sched)
{
   CmLListCp         *lst;
   CmLList           *lnk;
   uint8_t           *tpcCmds;
   uint8_t           tpc;
   S8                delta;
   Bool              atleastOne;

   pdcch->rnti = cb->tpcRnti;

   if (cb->isFmt3a)
   {
      /* Go through all UEs for format 3A case */
      lst = &cb->cfgdUes;
      pdcch->dci.dciFormat = TFU_DCI_FORMAT_3A;
      pdcch->dciNumOfBits = cell->dciSize.size[TFU_DCI_FORMAT_3A];
      pdcch->dci.u.format3AInfo.isPucch = TRUE;

      tpcCmds = pdcch->dci.u.format3AInfo.tpcCmd;
      /* No need to memset zero initially as every TPC is going
       * to be filled up for every configured UE */
      for (atleastOne = FALSE, lnk = lst->first; lnk; lnk = lnk->next)
      {
         RgSchUeCb             *ue    = (RgSchUeCb *)lnk->node;
         RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, cell);

         if ( ue->isDrxEnabled == TRUE && 
               !RG_SCH_DRX_DL_IS_UE_ACTIVE(ue->drxCb))
         {
            /* UE is in its DRX time. So we cannot give command
             * to this UE.
             */
            continue;
         }

         if (rgSCHPwrIsDlUeSched(cell, ue, dlSf))
         {
            /* UE already scheduled in downlink with PDCCH
             * carrying PUCCH pwr cmd. So don't care about
             * giving command to this UE. */
            continue;
         }
         rgSCHPwrGetPucchFmt3aTpcForUe(ue, &tpc, &delta);
         tpcCmds[uePwr->pucchIdx] = tpc;
         atleastOne = TRUE;
         rgSCHPwrOnPucchGrpPwrForUe(cell, ue, delta);
      }
   }
   else
   {
      /* Go through to-be-scheduled UEs for format 3 case */
      lst = &cb->toBeSchdUes;
      pdcch->dci.dciFormat = TFU_DCI_FORMAT_3;
      pdcch->dciNumOfBits = cell->dciSize.size[TFU_DCI_FORMAT_3];
      tpcCmds = pdcch->dci.u.format3Info.tpcCmd;
      pdcch->dci.u.format3Info.isPucch = TRUE;

      /* Fill TPC 1 (corresponding to no power change) initially */
      memset(tpcCmds, 1, sizeof(pdcch->dci.u.format3Info.tpcCmd));

      for (atleastOne = FALSE, lnk = lst->first; lnk; lnk = lnk->next)
      {
         RgSchUeCb             *ue    = (RgSchUeCb *)lnk->node;
         RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, cell);

         if ( ue->isDrxEnabled == TRUE && 
               !RG_SCH_DRX_DL_IS_UE_ACTIVE(ue->drxCb))
         {
            /* UE is in its DRX time. So we cannot give command
             * to this UE.
             */
            continue;
         }

         if (rgSCHPwrIsDlUeSched(cell, ue, dlSf))
         {
            /* UE already scheduled in downlink with PDCCH
             * carrying PUCCH pwr cmd. So don't care about
             * giving command to this UE. */
            continue;
         }
         rgSCHPwrGetPucchFmt3TpcForUe(ue, &tpc, &delta);
         tpcCmds[uePwr->pucchIdx] = tpc;
         atleastOne = TRUE;
         rgSCHPwrOnPucchGrpPwrForUe(cell, ue, delta);
      }
   }

   *sched = atleastOne;

   /* Check if no more UEs in TPC RNTI, and then remove
    * this TPC RNTI from scheduled list */
    if (cb->toBeSchdUes.count == 0)
    {
       rgSCHPwrRmvSchdPucchTpcRntiCb(cell, cb);
    }

   return;
}  /* rgSCHPwrSchedPucchRnti */

/***********************************************************
 *
 *     Func : rgSCHPwrSchedPuschRnti
 *
 *     Desc : Schedule TPC RNTI to be sent out
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrSchedPuschRnti(RgSchCellCb *cell,RgSchCmnTpcRntiCb *cb,RgSchPdcch *pdcch,RgSchUlSf *ulSf,Bool  *sched)
{
   CmLListCp         *lst;
   CmLList           *lnk;
   uint8_t           *tpcCmds;
   uint8_t           tpc;
   S8                delta;
   Bool              atleastOne;

   pdcch->rnti = cb->tpcRnti;

   if (cb->isFmt3a)
   {
      /* Go through all UEs for format 3A case */
      lst = &cb->cfgdUes;
      pdcch->dci.dciFormat = TFU_DCI_FORMAT_3A;
      pdcch->dciNumOfBits = cell->dciSize.size[TFU_DCI_FORMAT_3A];
      pdcch->dci.u.format3AInfo.isPucch = FALSE;
      tpcCmds = pdcch->dci.u.format3AInfo.tpcCmd;
      /* No need to memset zero initially as every TPC is going
       * to be filled up for every configured UE */
      for (atleastOne = FALSE, lnk = lst->first; lnk; lnk = lnk->next)
      {
         RgSchUeCb             *ue    = (RgSchUeCb *)lnk->node;
         RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, cell);
         if (rgSCHPwrIsUlUeSched(cell, ue, ulSf))
         {
            /* UE already scheduled in uplink with DCI
             * format 0. So don't care about giving
             * command to this UE. */
            continue;
         }

         if ( ue->isDrxEnabled == TRUE && 
               !RG_SCH_DRX_DL_IS_UE_ACTIVE(ue->drxCb))
         {
            /* UE is in its DRX time. So we cannot give command
             * to this UE.
             */
            continue;
         }

         rgSCHPwrGetPuschFmt3aTpcForUe(ue, &tpc, &delta);
         tpcCmds[uePwr->puschIdx] = tpc;
         atleastOne = TRUE;
         rgSCHPwrOnPuschGrpPwrForUe(cell, ue, delta);
      }
   }
   else
   {
      /* Go through to-be-scheduled UEs for format 3 case */
      lst = &cb->toBeSchdUes;
      pdcch->dci.dciFormat = TFU_DCI_FORMAT_3;
      pdcch->dciNumOfBits = cell->dciSize.size[TFU_DCI_FORMAT_3];
      pdcch->dci.u.format3Info.isPucch = FALSE;
      tpcCmds = pdcch->dci.u.format3Info.tpcCmd;

      /* Fill TPC 1 (corresponding to no power change) initially */
      memset(tpcCmds, 1, sizeof(pdcch->dci.u.format3Info.tpcCmd));

      for (atleastOne = FALSE, lnk = lst->first; lnk; lnk = lnk->next)
      {
         RgSchUeCb             *ue    = (RgSchUeCb *)lnk->node;
         RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, cell);
         if (rgSCHPwrIsUlUeSched(cell, ue, ulSf))
         {
            /* UE already scheduled in uplink with DCI
             * format 0. So don't care about giving
             * command to this UE. */
            continue;
         }

         if ( ue->isDrxEnabled == TRUE && 
               !RG_SCH_DRX_DL_IS_UE_ACTIVE(ue->drxCb))
         {
            /* UE is in its DRX time. So we cannot give command
             * to this UE.
             */
            continue;
         }

         rgSCHPwrGetPuschFmt3TpcForUe(ue, &tpc, &delta);
         tpcCmds[uePwr->puschIdx] = tpc;
         atleastOne = TRUE;
         rgSCHPwrOnPuschGrpPwrForUe(cell, ue, delta);
      }
   }

   *sched = atleastOne;

   /* Check if no more UEs in TPC RNTI, and then remove
    * this TPC RNTI from scheduled list */
    if (cb->toBeSchdUes.count == 0)
    {
       rgSCHPwrRmvSchdPuschTpcRntiCb(cell, cb);
    }

   return;
}  /* rgSCHPwrSchedPuschRnti */

/***********************************************************
 *
 *     Func : rgSCHPwrGetPucchFmt3TpcForUe
 *
 *     Desc : Gets 2 bit TPC cmd for PUCCH
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrGetPucchFmt3TpcForUe(RgSchUeCb *ue,uint8_t  *tpc,S8 *delta)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, ue->cell);

   rgSCHPwrGetAcc2bitTpc(uePwr->remPucchPwr, tpc, delta);
   return;
}  /* rgSCHPwrGetPucchFmt3TpcForUe */

/***********************************************************
 *
 *     Func : rgSCHPwrGetPucchFmt3aTpcForUe
 *
 *     Desc : Gets 1 bit TPC cmd for PUCCH
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrGetPucchFmt3aTpcForUe(RgSchUeCb *ue,uint8_t *tpc,S8 *delta)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, ue->cell);

   rgSCHPwrGetAcc1bitTpc(uePwr->remPucchPwr, tpc, delta);
   return;
}  /* rgSCHPwrGetPucchFmt3aTpcForUe */

/***********************************************************
 *
 *     Func : rgSCHPwrGetPuschFmt3TpcForUe
 *
 *     Desc : Gets 2 bit TPC cmd for PUCCH
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrGetPuschFmt3TpcForUe(RgSchUeCb *ue,uint8_t *tpc,S8 *delta)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, ue->cell);
   S8                     adj = RGSCH_MIN(uePwr->remPuschPwr, uePwr->phVal);

   rgSCHPwrGetAcc2bitTpc(adj, tpc, delta);
   return;
}  /* rgSCHPwrGetPuschFmt3TpcForUe */

/***********************************************************
 *
 *     Func : rgSCHPwrGetPuschFmt3aTpcForUe
 *
 *     Desc : Gets 1 bit TPC cmd for PUCCH
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrGetPuschFmt3aTpcForUe(RgSchUeCb *ue,uint8_t *tpc,S8 *delta)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, ue->cell);

   /* Don't attempt to look at headroom now, power
    * adjustment is small anyway */
   rgSCHPwrGetAcc1bitTpc(uePwr->remPuschPwr, tpc, delta);
   return;
}  /* rgSCHPwrGetPuschFmt3aTpcForUe */

/***********************************************************
 *
 *     Func : rgSCHPwrGetAcc1bitTpc
 *
 *     Desc : Gets 1 bit TPC cmd
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrGetAcc1bitTpc(S8 remPwr,uint8_t *tpc,S8 *delta)
{
   /*
    * TPC   delta
    *  0     -1
    *  1      1
    */
   if (remPwr <= 0)
   {
      *delta = -1;
      *tpc = 0;
   }
   else
   {
      *delta = 1;
      *tpc = 1;
   }
   return;
}  /* rgSCHPwrGetAcc1bitTpc */

/***********************************************************
 *
 *     Func : rgSCHPwrGetAcc2bitTpc
 *
 *     Desc : Allocate PDCCH for group power control
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrGetAcc2bitTpc(S8 remPwr,uint8_t *tpc,S8 *delta)
{
   /*
    * TPC   delta
    *  0     -1
    *  1      0
    *  2      1
    *  3      3
    */
   uint8_t            tpcs[3]   = {1, 2, 2};
   uint8_t            deltas[3] = {0, 1, 1};
   if (remPwr <= -1)
   {
      *tpc   = 0;
      *delta = -1;
   }
   else if (remPwr >= 3)
   {
      *tpc   = 3;
      *delta = 3;
   }
   else
   {
      *tpc   = tpcs[(uint8_t)remPwr];
      *delta = deltas[(uint8_t)remPwr];
   }
   return;
}  /* rgSCHPwrGetAcc2bitTpc */

/***********************************************************
 *
 *     Func : rgSCHPwrGetAbsTpc
 *
 *     Desc : Allocate PDCCH for group power control
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrGetAbsTpc(S8 remPwr,uint8_t *tpc,S8 *delta)
{
   /*
    * TPC   delta
    *  0     -4
    *  1     -1
    *  2      1
    *  3      4
    */
   if (remPwr <= -3)
   {
      *tpc = 0;
      *delta = -4;
   }
   else if (remPwr < 1)
   {
      *tpc = 1;
      *delta = -1;
   }
   else if (remPwr < 4)
   {
      *tpc = 2;
      *delta = 1;
   }
   else
   {
      *tpc = 3;
      *delta = 4;
   }
   return;
}  /* rgSCHPwrGetAbsTpc */

/***********************************************************
 *
 *     Func : rgSCHPwrOnPucchGrpPwrForUe
 *
 *     Desc : Processing on sending TPC for UE through group power
 *            control. Apart from updating remPwr, this only takes
 *            care of possibly removing UE from scheduled
 *            list in TPC RNTI.
 *            It does not take care of possibly removing TPC RNTI
 *            from scheduled list in cell. This is done
 *            in the caller after TPC for all UEs has been
 *            determined. (This is where it differs
 *            from the usual OnSendingPu[cs]ch TPC]
 *
 *     Ret  : ROK/RFAILED
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrOnPucchGrpPwrForUe(RgSchCellCb *cell,RgSchUeCb *ue,S8  delta)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, ue->cell);
   Bool                   rmvUe = FALSE;

   UNUSED(cell);

   uePwr->remPucchPwr -= delta;

   /* UE was already scheduled for PUCCH group power
    * control which is why we came here. Don't
    * again check for this. */

   /* UE was scheduled for pucch grp pwr, sent TPC may
    * possibly cause it to be removed. */
      if (!uePwr->remPucchPwr)
      {
         rmvUe = TRUE;
      }
   if (rmvUe)
   {
      rgSCHPwrRmvSchdUeOnlyFrmPucchTpcRntiCb(cell, uePwr->tpcPucchRntiCb, ue);
      /* Not removing TPC RNTI from scheduled list,
       * this will happen in the caller once this
       * function is called for every UE scheduled. */
   }
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrOnPuschGrpPwrForUe
 *
 *     Desc : Processing on sending TPC for UE through group power
 *            control. Apart from updating remPwr, this only takes
 *            care of possibly removing UE from scheduled
 *            list in TPC RNTI.
 *            It does not take care of possibly removing TPC RNTI
 *            from scheduled list in cell. This is done
 *            in the caller after TPC for all UEs has been
 *            determined. (This is where it differs
 *            from the usual OnSendingPu[cs]ch TPC]
 *
 *     Ret  : ROK/RFAILED
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrOnPuschGrpPwrForUe(RgSchCellCb *cell,RgSchUeCb *ue,S8 delta)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, ue->cell);
   Bool                   rmvUe = FALSE;

   UNUSED(cell);

   uePwr->delta = delta;
   uePwr->remPuschPwr -= delta;
   if (uePwr->isPhrAvail)
   {
      uePwr->phVal -= uePwr->delta;
      uePwr->phVal = RGSCH_MAX(-23, uePwr->phVal);
   }

   /* UE was already scheduled for PUSCH group power
    * control which is why we came here. Don't
    * again check for this. */

   /* UE was scheduled for pusch grp pwr, sent TPC may
    * possibly cause it to be removed. */

      if (!uePwr->remPuschPwr)
      {
         rmvUe = TRUE;
      }

   if (rmvUe)
   {
      rgSCHPwrRmvSchdUeOnlyFrmPuschTpcRntiCb(cell, uePwr->tpcPuschRntiCb, ue);
      /* Not removing TPC RNTI from scheduled list,
       * this will happen in the caller once this
       * function is called for every UE scheduled. */
   }
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrIsDlUeSched
 *
 *     Desc : Check if UE is scheduled in the passed DL SF
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Bool rgSCHPwrIsDlUeSched(RgSchCellCb *cell,RgSchUeCb *ue,RgSchDlSf  *sf)
{
   RgSchDlHqEnt          *hqEnt = RG_SCH_CMN_GET_UE_HQE(ue, cell);
   RgSchDlHqProcCb      *proc    = rgSCHDhmLastSchedHqProc(hqEnt);

   if (proc == NULLP)
   {
      return (FALSE);
   }

   /*
    * The following subframe check is assumed enough, since
    * scheduled procs stay for a short time (until feedback
    * arrives), which typically is expected to have a
    * turnaround time of less than 8 subframes. So
    * we are probably never going to come across cases
    * where a process stays in the list for more than
    * 10 subframes, which would have otherwise caused
    * the check to succeed for a possibly older process.
    */
   if ((proc->tbInfo[0].timingInfo.slot == sf->sfNum) ||
       (proc->tbInfo[1].timingInfo.slot == sf->sfNum))
   {
      /*
       * Later, if a proc can be scheduled without having an
       * associated PDCCH, need to also check if PDCCH exists.
       * This is because for power, what matters is whether
       * TPC is going out for UE at this time or not, at least
       * that is what this function was introduced for.
       * Checking for PDCCH would have to be in common proc
       * the way things are now.
       */
      return (TRUE);
   }
   else
   {
      return (FALSE);
   }
}  /* rgSCHPwrIsDlUeSched */

/***********************************************************
 *
 *     Func : rgSCHPwrIsUlUeSched
 *
 *     Desc : Check if UE is scheduled in the passed UL SF
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Bool rgSCHPwrIsUlUeSched(RgSchCellCb  *cell,RgSchUeCb *ue,RgSchUlSf *sf)
{
   RgSchCmnUlCell      *cmnCell = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchUlHqProcCb     *proc = rgSCHUhmGetUlHqProc(cell, ue, cmnCell->schdHqProcIdx);

   UNUSED(sf);

#if (ERRCLASS & ERRCLS_DEBUG)
   if( proc == NULLP )
   {
      return  (FALSE);
   }
#endif  

   if (proc->alloc)
   {
      return (TRUE);
   }
   else
   {
      return (FALSE);
   }
}  /* rgSCHPwrIsUlUeSched */

/**
 * @brief Handles Pucch power delta indication recieved from PHY
 *
 * @details
 *
 *     Function : rgSCHPwrPucchDeltaInd
 *
 *     Invoking Module Processing:
 *      - This shall be invoked on reception of Pucch power 
 *        delta indication from PHY.
 *        
 *     Processing Steps:
 *     - Update the remPucchPwr 
 *       ue->remPucchPwr = pwrDelta
 *     - If (ue->tpcPucchRntiCb)
 *       - If (fmtType = 3A)
 *          - if (remPucchPwr >= 2 || remPucchPwr <= -2 )
 *             - if (tpcPucchRntiCb is not in the pucchGrpPwr List)
 *               - Add tpcPucchRntiCb to the pucchGrpPwr list.
 *             - If not added, add to toBeSchdLst
 *          - else
 *             - If already added, remove from toBeSchdLst
 *       - else If (fmtType == 3) 
 *          - if (remPucchPwr)
 *             - if (tpcPucchRntiCb is not in the pucchGrpPwr List)
 *               - Add tpcPucchRntiCb to the pucchGrpPwr list.
 *             - If not added, add to toBeSchdLst
 *          - else
 *             - If already added, remove from toBeSchdLst
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  uint8_t           pwrDelta
 *  @return  Void
 **/
Void rgSCHPwrPucchDeltaInd(RgSchCellCb *cell,RgSchUeCb *ue,S8 pwrDelta)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, cell);
   RgSchCmnTpcRntiCb     *cb;
   Bool                   toAdd;

   uePwr->remPucchPwr = pwrDelta;
   
   if ((cb = uePwr->tpcPucchRntiCb) == NULLP)
   {
      return;
   }

   toAdd = FALSE;

   if (0 != uePwr->remPucchPwr)
      {
         toAdd = TRUE;
      }


   if (toAdd)
   {
      rgSCHPwrAddSchdUeToPucchTpcRntiCb(cell, cb, ue);
   }
   else
   {
      rgSCHPwrRmvSchdUeFrmPucchTpcRntiCb(cell, cb, ue);
   }

   return;
}

/**
 * @brief Does processing after TPC for Pucch has been sent
 *
 * @details
 *
 *     Function : rgSCHPwrOnSchedPucchTpc
 *
 *     Invoking Module Processing:
 *      - It shall be invoked after it is determined that PDCCH for UE
 *        is finalised to go out, and thus TPC for PUCCH is being
 *        sent out.
 *
 *     Processing Steps:
 *     - Update remPucchPwr with the delta
 *     - Do group power control related processing
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @param[in]  S8                 delta
 *  @return  Void
 **/
static Void rgSCHPwrOnSchedPucchTpc(RgSchCellCb *cell,RgSchUeCb *ue,S8 delta)
{
   /* Similar to rgSCHPwrPucchDeltaInd.. not reusing
    * that since we use the fact that UE could only have
    * improved its remPwr as part of power control. */
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, cell);
   Bool                   rmvUe = FALSE;

   uePwr->remPucchPwr -= delta;

   if (uePwr->schdPucchGrpLnk.node == NULLP)
   {
      return;
   }

   /* UE was scheduled for TPC, sent TPC may
    * possibly cause it to be removed. */

      if (!uePwr->remPucchPwr)
      {
         rmvUe = TRUE;
      }

   if (rmvUe)
   {
      rgSCHPwrRmvSchdUeFrmPucchTpcRntiCb(cell, uePwr->tpcPucchRntiCb, ue);
      if (uePwr->tpcPucchRntiCb->toBeSchdUes.count == 0)
      {
         rgSCHPwrRmvSchdPucchTpcRntiCb(cell, uePwr->tpcPucchRntiCb);
      }
   }
   return;
}


/**
 * @brief Does processing after TPC for Pusch has been sent
 *
 * @details
 *
 *     Function : rgSCHPwrOnSchedPuschTpc
 *
 *     Processing Steps:
 *      - If accumulative
 *        - Update remPuschPwr with the delta
 *        - Do group power related processing if applicable
 *       
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @return  Void
 **/
static Void rgSCHPwrOnSchedPuschTpc(RgSchCellCb  *cell,RgSchUeCb *ue)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, cell);
   Bool                   rmvUe = FALSE;

   /* Don't do anything for the case of absolute TPC commands */
   if (!uePwr->isAccumulated)
   {
      return;
   }

   uePwr->remPuschPwr -= uePwr->delta;
   if (uePwr->isPhrAvail)
   {
      uePwr->phVal -= uePwr->delta;
      uePwr->phVal = RGSCH_MAX(-23, uePwr->phVal);
   }

   if (uePwr->schdPuschGrpLnk.node == NULLP)
   {
      return;
   }

   /* UE was scheduled for pusch TPC, sent TPC may
    * possibly cause it to be removed. */

      if (!uePwr->remPuschPwr)
      {
         rmvUe = TRUE;
      }

   if (rmvUe)
   {
      rgSCHPwrRmvSchdUeFrmPuschTpcRntiCb(cell, uePwr->tpcPuschRntiCb, ue);
   }

   return;
}

/**
 * @brief Handles PHR updation for the UE
 *
 * @details
 *
 *     Function : rgSCHPwrUpdExtPhr
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]  RgInfExtPhrCEInfo   *extPhr
 *  @param[in]  RgSchCmnAllocRecord  allocInfo
 *  @return  Void
 **/
Void rgSCHPwrUpdExtPhr(RgSchCellCb  *cell,RgSchUeCb *ue,RgInfExtPhrCEInfo *extPhr,RgSchCmnAllocRecord  *allocInfo)
{
   uint8_t idx;
   RgInfExtPhrSCellInfo  *servCellPhr;
   S8      pCMax;

   for (idx = 0; idx < extPhr->numServCells; idx++)
   {
      servCellPhr = &extPhr->servCellPhr[idx];

      if (RG_SCH_REF_PCMAX == servCellPhr->pCmax)
      {
         pCMax = RG_SCH_CMN_PWR_USE_CFG_MAX_PWR;
      }
      else
      {
         pCMax = rgSCHPwrGetPCMaxValFromPCMax(servCellPhr->pCmax);
      }
      rgSCHPwrUpdPhr(ue->cellInfo[servCellPhr->sCellIdx]->cell,
            ue, servCellPhr->phr, allocInfo, pCMax);
   }
   return;
}

/**
 * @brief Handles PHR updation for the UE
 *
 * @details
 *
 *     Function : rgSCHPwrUpdPhr
 *
 *     Invoking Module Processing:
 *      - This shall be invoked on reception of PHR from MAC to SCH. It shall
 *      pass the information of number of RBs, coding efficiency and TPC for 
 *      the Pusch transmission for which PHR has been reported.
 *
 *     Processing Steps:
 *     - Compute power per RB using the PHR report
 *        - ue_transmit_pwr = ue_max_pwr - PHR
 *        - if isDeltaMcs = TRUE
 *          - ue_transmit_pwr - 
 *            [10log(phr_num_rb) + 10log(2^ (1.25 * phr_coding_effeciency) -1)
 *            + phr_tpc(if absolute TPC)] = pwrPerRB
 *        - else
 *          - ue_transmit_pwr - [10log(phr_num_rb) + phr_tpc(if absolute TPC)]
 *            = pwrPerRB
 *        (Use the number of RBs and efficiency used by UE which caused the PHR
 *         report to happen)
 *     - Adjust PHR according to last allocation (take into account
 *       number of RBs granted in the last allocation)
 *     - Update the PHR report in the control block
 *     - Set isPhrAvail = TRUE
 *     - Do group power control related processing if applicable
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]  uint8_t                   phr
 *  @param[in]  RgSchCmnAllocRecord  allocInfo
 *  @param[in]  uint8_t                   maxUePwr
 *  @return  Void
 **/
Void rgSCHPwrUpdPhr(RgSchCellCb *cell,RgSchUeCb *ue,uint8_t phr,RgSchCmnAllocRecord   *allocInfo,S8 maxUePwr )
{
   RgSchCmnUeUlPwrCb     *uePwr   = RG_SCH_PWR_GETUEPWR(ue, cell);
   uint8_t               rbPwr;
   uint8_t               effPwr;
   RgSchCmnUlCell *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);

   uePwr->phVal = rgSCHPwrGetPhValFromPhr(phr);
  
   if (maxUePwr == RG_SCH_CMN_PWR_USE_CFG_MAX_PWR)
   {
      maxUePwr = uePwr->maxUePwr;
   }
   rbPwr = rgSCHPwrRbToPwr(cell,allocInfo->numRb);
   effPwr = rgSCHPwrGetCqiPwrForUe(cell, ue, allocInfo->cqi);
   uePwr->pwrPerRb = maxUePwr - uePwr->phVal - rbPwr - effPwr;
   /*if (!uePwr->isAccumulated)
   {
      uePwr->pwrPerRb -= rgSCHPwrGetDeltaFrmAbsTpc(allocInfo->tpc);
   }*/

   /* Let headroom reflect remaining power according to last
    * allocated number of RBs. Intermediate TPCs not yet
    * taken care of (for accumulated case, it is anyway
    * not applicable for absolute commands). */
   uePwr->phVal -= (rgSCHPwrRbToPwr(cell, cellUl->sbSize)) - rbPwr;
   uePwr->phVal = RGSCH_MAX(-23, uePwr->phVal);
   uePwr->isPhrAvail = TRUE;

   rgSCHPwrOnPuschPwrUpd(cell, ue);

   DU_LOG("\nDEBUG  -->  SCH : Output: Reported PHR[%d] cqi[%u] allocRb[%u] uePwr->pwrPerRb[%d]",
            uePwr->phVal,
            allocInfo->cqi,
            allocInfo->numRb,  
            uePwr->pwrPerRb); 
   return;
}

/**
 * @brief Handles UL CQI indication
 *
 * @details
 *
 *     Function : rgSCHPwrUlCqiInd
 *
 *     Invoking Module Processing:
 *      - This shall be invoked when uplink CQI indication
 *        is receiving from PHY for a UE.
 *
 *     Processing Steps:
 *     - Update remPuschPwr.
 *     - Possibly schedule for group power control.
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]  uint8_t                   numRb
 *  @return  Void
 **/
Void rgSCHPwrUlCqiInd(RgSchCellCb *cell,RgSchUeCb  *ue)
{
   RgSchCmnUlUe          *ueUl    = RG_SCH_CMN_GET_UL_UE(ue, cell);
   RgSchCmnUeUlPwrCb     *uePwr   = RG_SCH_PWR_GETUEPWR(ue, cell);
#ifdef TFU_UPGRADE 
   S32  tmp;
#endif

   /*
    * For absolute power cmd case, we could look at the time
    * at which CQI was received, determine if there was a
    * PUSCH TPC cmd for that time (this could come from
    * group power control too), and (if this
    * CQI report is indeed based on the the PUSCH tx)
    * then factor in that cmd here. Not doing
    * this as of now.
    */

   /* See how much power needs to be adjusted based on cqi
    * differential */
   uePwr->remPuschPwr = 
#ifdef TFU_UPGRADE
      rgSCHPwrGetDelta2FrmCqi(ueUl->validUlCqi, uePwr->trgCqi, ue, cell);
#else
      rgSCHPwrGetDelta2FrmCqi(ueUl->crntUlCqi[0], uePwr->trgCqi, ue, cell);
#endif

   rgSCHPwrOnPuschPwrUpd(cell, ue);
#ifdef TFU_UPGRADE
   if(uePwr->maxPwrDeltaByPhr < 0)
   {
      tmp = ueUl->validUlCqi;
      tmp = tmp + uePwr->maxPwrDeltaByPhr;
      if (tmp < 1 )
      {
         ueUl->validUlCqi = 1;
      }
      else
      {
         ueUl->validUlCqi = tmp;
      }
   }
#endif 

   return;
}

/**
 * @brief Updates the stored last number of RBs allocated
 *
 * @details
 *
 *     Function : rgSCHPwrRecordRbAlloc
 *
 *     Invoking Module Processing:
 *      - This shall be invoked when uplink allocation is made for
 *        a UE.
 *      - Note: If outstanding TPCs are considered at the time
 *        of PHR report, the last numRb would also be known
 *        and then this API would not be needed. 
 *
 *     Processing Steps:
 *     - Adjust PHR according to now allocated number of RBs
 *     - Store the number of RBs
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]  uint8_t                   numRb
 *  @return  Void
 **/
Void rgSCHPwrRecordRbAlloc(RgSchCellCb *cell,RgSchUeCb  *ue,uint8_t  numRb)
{
   RgSchCmnUeUlPwrCb     *uePwr   = RG_SCH_PWR_GETUEPWR(ue, cell);
   UNUSED(cell);
   return; 

   if (uePwr->isPhrAvail)
   {
      uePwr->phVal += rgSCHPwrRbToPwr(cell,numRb) - rgSCHPwrRbToPwr(cell,uePwr->numRb);
      uePwr->phVal = RGSCH_MIN(40, uePwr->phVal);
   }
   uePwr->numRb = numRb;
   return;
}

/**
 * @brief Handles power related configuration for a cell
 *
 * @details
 *
 *     Function : rgSCHPwrCellCfg
 *
 *     Invoking Module Processing:
 *      - This shall be invoked during cell config
 *        
 *     Processing Steps:
 *      - Set pMax
 *      - Set target CQI
 *      - Update TPC-RNTI information for the cell for Pucch and Pusch.
 *      - For each TPC-Pucch-RNTI,
 *         - Call  rgSCHAddRntiToPucchRntiLst()
 *      - For each TPC-Pusch-RNTI,
 *         - Call  rgSCHAddRntiToPuschRntiLst()
 *      - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgrCellCfg   *cfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHPwrCellCfg(RgSchCellCb *cell,RgrCellCfg  *cfg)
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);
   CmLteRnti              rnti;
   CmLteRnti              startRnti;
   uint16_t                    size;
   Bool                   isFmt3a;

   /* Right now, all UEs have fixed maximum power capability. So
    * we store cell wide pMax as minimum of configured pMax and
    * UE's max power */
   cellPwr->pMax = RGSCH_MIN(cfg->pMax, RG_SCH_PWR_UE_MAX_PWR);

   /* trgUlCqi already validated by common */
   cellPwr->trgUlCqi = cfg->trgUlCqi.trgCqi;

   /* Validate number of TPC RNTIs */
   if ((cfg->pwrCfg.pucchPwrFmt3.size + cfg->pwrCfg.pucchPwrFmt3a.size
            > RG_SCH_CMN_MAX_NUM_TPC_PUCCH_RNTI)
         || (cfg->pwrCfg.puschPwrFmt3.size + cfg->pwrCfg.puschPwrFmt3a.size
            > RG_SCH_CMN_MAX_NUM_TPC_PUSCH_RNTI))
   {
      return RFAILED;
   }

   /* Now initialise TPC RNTIs */

   /* Format 3 Pucch TPC RNTIs */
   isFmt3a = FALSE;
   startRnti = cfg->pwrCfg.pucchPwrFmt3.startTpcRnti;
   size = cfg->pwrCfg.pucchPwrFmt3.size;
   for (rnti = startRnti; (rnti < startRnti + size); ++rnti)
   {
      rgSCHPwrAddRntiToPucchRntiLst(cell, rnti, isFmt3a);
   }

   /* Format 3A Pucch TPC RNTIs */
   isFmt3a = TRUE;
   startRnti = cfg->pwrCfg.pucchPwrFmt3a.startTpcRnti;
   size = cfg->pwrCfg.pucchPwrFmt3a.size;
   for (rnti = startRnti; (rnti < startRnti + size); ++rnti)
   {
      rgSCHPwrAddRntiToPucchRntiLst(cell, rnti, isFmt3a);
   }

   /* Format 3 Pusch TPC RNTIs */
   isFmt3a = FALSE;
   startRnti = cfg->pwrCfg.puschPwrFmt3.startTpcRnti;
   size = cfg->pwrCfg.puschPwrFmt3.size;
   for (rnti = startRnti; (rnti < startRnti + size); ++rnti)
   {
      rgSCHPwrAddRntiToPuschRntiLst(cell, rnti, isFmt3a);
   }

   /* Format 3A Pusch TPC RNTIs */
   isFmt3a = TRUE;
   startRnti = cfg->pwrCfg.puschPwrFmt3a.startTpcRnti;
   size = cfg->pwrCfg.puschPwrFmt3a.size;
   for (rnti = startRnti; (rnti < startRnti + size); ++rnti)
   {
      rgSCHPwrAddRntiToPuschRntiLst(cell, rnti, isFmt3a);
   }

   return ROK;
}

/**
 * @brief Handles power related re-configuration for a cell
 *
 * @details
 *
 *     Function : rgSCHPwrCellRecfg
 *
 *     Processing Steps:
 *     - NONE
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgrCellRecfg *recfg
 *  @return  S16
 *      -# ROK
 **/
S16 rgSCHPwrCellRecfg(RgSchCellCb *cell,RgrCellRecfg *recfg)
{
   UNUSED(cell);
   UNUSED(recfg);

   /* Not doing anything for power reconfig, so such structure
    * in RGR */
   return ROK;
}

/**
 * @brief Frees power related data structures in cell
 *
 * @details
 *
 *     Function : rgSCHPwrCellDel
 *
 *     Processing Steps:
 *      - NONE
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @return  Void
 **/
Void rgSCHPwrCellDel(RgSchCellCb *cell)
{
   UNUSED(cell);

   /* There is no allocated memory, so do nothing */
   return;
}


#ifdef LTE_ADV
/**
 * @brief Configures ULPC CB for a SCELL being added
 *
 * @details
 *
 *     Function : rgSCHPwrUeSCellCfg
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeCfg     *cfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHPwrUeSCellCfg(RgSchCellCb *cell,RgSchUeCb   *ue,RgrUeSecCellCfg  *sCellInfoCfg)
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, cell);
   RgSchCmnUeUlPwrCb     *uePwrPCell = RG_SCH_PWR_GETUEPWR(ue, ue->cell);
   RgSchCmnUlUe          *ueUl        = RG_SCH_CMN_GET_UL_UE(ue, cell);

   RgSchCmnUlCell *cellUl      = RG_SCH_CMN_GET_UL_CELL(cell);


   uePwr->maxUePwr = cellPwr->pMax;
   uePwr->trgCqi   = cellPwr->trgUlCqi; /* Overriding with UE's happens later */
   uePwr->numRb    = 1;

   uePwr->maxPwrPerRb = uePwr->maxUePwr - rgSchPwrRbToPwrTbl[cellUl->sbSize];

   uePwr->isPhrAvail  = FALSE;
   uePwr->phVal       = 40;
   uePwr->maxUlRbs    = RGSCH_MAX_DL_BW;
   uePwr->delta       = 0;
   uePwr->puschTpc    = 1;
   uePwr->remPuschPwr = 0;

   /* Rest of the vars update and group power control related
    * config happens in the function below */
   uePwr->isAccumulated = sCellInfoCfg->ueSCellUlDedPwrCfg.isAccumulated;
   uePwr->deltaMcsEnbld = sCellInfoCfg->ueSCellUlDedPwrCfg.isDeltaMCSEnabled;

   uePwr->trgCqi = uePwrPCell->trgCqi;

   if (ueUl->maxUlCqi < uePwr->trgCqi)
   {
      uePwr->trgCqi = ueUl->maxUlCqi;
   }
   uePwr->p0UePusch = sCellInfoCfg->ueSCellUlDedPwrCfg.p0UePusch;

   return ROK;
}
#endif

/**
 * @brief Handles power related configuration for a UE
 *
 * @details
 *
 *     Function : rgSCHPwrUeCfg
 *
 *     Processing Steps:
 *      - If Pusch group power configuration exists && accumulation enabled,
 *        - Fetch the TPC-Pusch-RNTI control block for the configured
 *        TPC-Pusch-RNTI. Call rgSCHGetRntiFrmPuschRntiLst(). If it does not
 *        exist, return RFAILED.
 *        - Add Ue to the ueLst of TPC-Pusch-RNTI control block.
 *        - Update tpcPuschRntiCb pointer in UE.
 *        - Update the puschIdx value.
 *      - If Pucch group power configuration exists && accumulation enabled,
 *        - Fetch the TPC-Pucch-RNTI control block for the configured
 *        TPC-Pucch-RNTI. Call rgSCHGetRntiFrmPucchRntiLst(). If it does not
 *        exist, return RFAILED.
 *        - Add Ue to the ueLst of TPC-Pucch-RNTI control block.
 *        - Update tpcPucchRntiCb pointer in UE.
 *        - Update the pucchIdx value.
 *      - Update isAccumulated and isDeltaMcs variables.
 *      - maxUlRbs = configured maximum UL bandwidth value per UE.
 *      - trgUlCqi = configured value, if any, else cell-wide default trg CQI value.
 *      - If format type is format 3A, update remaining power to +1
 *      - Update TPC-RNTI information for the cell for Pucch and Pusch.
 *      - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeCfg     *cfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHPwrUeCfg(RgSchCellCb *cell,RgSchUeCb   *ue,RgrUeCfg    *cfg)
{
   S16                    ret;
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, cell);

   RgSchCmnUlCell *cellUl      = RG_SCH_CMN_GET_UL_CELL(cell);

   uePwr->maxUePwr = cellPwr->pMax;
   uePwr->trgCqi   = cellPwr->trgUlCqi; /* Overriding with UE's happens later */
   uePwr->numRb    = 1;

   uePwr->maxPwrPerRb = uePwr->maxUePwr - rgSchPwrRbToPwrTbl[cellUl->sbSize];

   rgSCHPwrUeResetPucch(cell, ue);
   rgSCHPwrUeResetPusch(cell, ue);

   /* Rest of the vars update and group power control related
    * config happens in the function below */
   ret = rgSCHPwrApplyUePwrCfg(cell, ue, &cfg->ueUlPwrCfg);

   return (ret);
}

/**
 * @brief Handles power related re-configuration for a UE
 *
 * @details
 *
 *     Function : rgSCHPwrUeRecfg
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE re-configuration.
 *        
 *     Processing Steps:
 *      - If change in TPC-RNTI, update the pointer and the TPC RNTI Cb appropriately.
 *      - If accumulation disabled, remove the UE from TPC-RNTI lists of UE, if
 *      it exists.
 *      - If group power configuration disabled, remove the UE from TPC-RNTI lists of UE, if
 *      it exists.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeRecfg   *recfg
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHPwrUeRecfg(RgSchCellCb   *cell,RgSchUeCb     *ue,RgrUeRecfg    *recfg)
{
   S16                    ret;
   RgSchCmnUeUlPwrCb     *uePwr       = RG_SCH_PWR_GETUEPWR(ue, cell);
   RgrUeUlPwrCfg         *pwrCfg = &recfg->ueUlPwrRecfg;

   if (pwrCfg->p0UePucch != uePwr->p0UePucch)
   {
      rgSCHPwrUeResetPucch(cell, ue);
   }
   if ((pwrCfg->isAccumulated != uePwr->isAccumulated)
         || (pwrCfg->p0UePusch != uePwr->p0UePusch))
   {
      rgSCHPwrUeResetPusch(cell, ue);
   }
   ret = rgSCHPwrApplyUePwrCfg(cell, ue, &recfg->ueUlPwrRecfg);

   return (ret);
}

/***********************************************************
 *
 *     Func : rgSCHPwrApplyUePwrCfg
 *
 *     Desc : Applies power config for UE. Meants to be
 *            used during both power config and reconfig.
 *
 *     Ret  : ROK/RFAILED
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHPwrApplyUePwrCfg(RgSchCellCb *cell,RgSchUeCb *ue,RgrUeUlPwrCfg *pwrCfg)
{
   S16                    ret;
   RgSchCmnUlUe          *ueUl        = RG_SCH_CMN_GET_UL_UE(ue, cell);
   RgSchCmnUeUlPwrCb     *uePwr       = RG_SCH_PWR_GETUEPWR(ue, cell);
   RgSchCmnTpcRntiCb     *pucchRntiCb = NULLP;
   RgSchCmnTpcRntiCb     *puschRntiCb = NULLP;
   uint8_t               pucchIdx    = 0;
   uint8_t               puschIdx    = 0;

   /* Validate Pucch group power control config */
   if (pwrCfg->uePucchPwr.pres)
   {
      pucchRntiCb =
         rgSCHPwrGetPucchRntiCb(cell, pwrCfg->uePucchPwr.tpcRnti);
      if (pucchRntiCb == NULLP)
      {
         return RFAILED;
      }
      pucchIdx = pwrCfg->uePucchPwr.idx;
      ret = rgSCHPwrChkPucchTpcRntiIdx(pucchRntiCb, pucchIdx);
      if (ret != ROK)
      {
         return (ret);
      }
   }

   /* Validate Pusch group power control config */
   if (pwrCfg->uePuschPwr.pres)
   {
      puschRntiCb =
         rgSCHPwrGetPuschRntiCb(cell, pwrCfg->uePuschPwr.tpcRnti);
      if (puschRntiCb == NULLP)
      {
         return RFAILED;
      }
      puschIdx = pwrCfg->uePuschPwr.idx;
      ret = rgSCHPwrChkPuschTpcRntiIdx(puschRntiCb, puschIdx);
      if (ret != ROK)
      {
         return (ret);
      }
   }

   /* Apply Pucch group power control config */
   if (pucchRntiCb)
   {
      if (uePwr->tpcPucchRntiCb != pucchRntiCb) /* This part for recfg */
      {
         if (uePwr->tpcPucchRntiCb)
         {
            rgSCHPwrDelUeFrmPucchTpcRntiCb(cell, uePwr->tpcPucchRntiCb, ue);
         }
         uePwr->tpcPucchRntiCb = pucchRntiCb;
         rgSCHPwrAddUeToPucchTpcRntiCb(cell, pucchRntiCb, ue);
      }
      uePwr->pucchIdx = pucchIdx;
#ifndef ALIGN_64BIT
       DU_LOG("\nDEBUG  -->  SCH : <GRP_PWR>PucchRntiCb cfgdUes(%ld %lu %lu) UEID:%d",
               pucchRntiCb->cfgdUes.count,((uint32_t)pucchRntiCb->cfgdUes.first),
               ((uint32_t)pucchRntiCb->cfgdUes.last),ue->ueId);
       DU_LOG("\nDEBUG  -->  SCH : UEID:%d isFmt3a(%u) ueNode(%ld)",
               ue->ueId,pucchRntiCb->isFmt3a,
               pucchRntiCb->schdLnk.node);
       DU_LOG("\nDEBUG  -->  SCH : toBeSchdUes(%ld %lu %lu) tpcRnti(%u)", 
               pucchRntiCb->toBeSchdUes.count, 
               ((uint32_t)pucchRntiCb->toBeSchdUes.first),
               ((uint32_t)pucchRntiCb->toBeSchdUes.last), 
               pucchRntiCb->tpcRnti);
#else
      DU_LOG("\nDEBUG  -->  SCH : <GRP_PWR>PucchRntiCb cfgdUes(%d %lu %lu) UEID:%d",
               pucchRntiCb->cfgdUes.count,((uint64_t)pucchRntiCb->cfgdUes.first),
               ((uint64_t)pucchRntiCb->cfgdUes.last),ue->ueId);
      DU_LOG("\nDEBUG  -->  SCH : UEID:%d isFmt3a(%u) ueNode(%ld)",
               ue->ueId,pucchRntiCb->isFmt3a,
               pucchRntiCb->schdLnk.node);
      DU_LOG("\nDEBUG  -->  SCH : toBeSchdUes(%d %lu %lu) tpcRnti(%u)", 
               pucchRntiCb->toBeSchdUes.count, 
               ((uint64_t)pucchRntiCb->toBeSchdUes.first),
               ((uint64_t)pucchRntiCb->toBeSchdUes.last), 
               pucchRntiCb->tpcRnti);

#endif
   }

   /* Apply Pusch group power control config */
   if (puschRntiCb)
   {
      if (uePwr->tpcPuschRntiCb != puschRntiCb) /* This part for recfg */
      {
         if (uePwr->tpcPuschRntiCb)
         {
            rgSCHPwrDelUeFrmPuschTpcRntiCb(cell, uePwr->tpcPuschRntiCb, ue);
         }
         uePwr->tpcPuschRntiCb = puschRntiCb;
         rgSCHPwrAddUeToPuschTpcRntiCb(puschRntiCb, ue);
      }
      uePwr->puschIdx = puschIdx;
   }

   /* Update vars */
   uePwr->isAccumulated = pwrCfg->isAccumulated;
   uePwr->deltaMcsEnbld = pwrCfg->isDeltaMCSEnabled;
   if (pwrCfg->trgCqi)
   {
      uePwr->trgCqi = pwrCfg->trgCqi;
   }
   if (ueUl->maxUlCqi < uePwr->trgCqi)
   {
      uePwr->trgCqi = ueUl->maxUlCqi;
   }
   uePwr->p0UePusch = pwrCfg->p0UePusch;
   uePwr->p0UePucch = pwrCfg->p0UePucch;

   return ROK;
}


/**
 * @brief Deletes power related information for UE
 *
 * @details
 *
 *     Function : rgSCHPwrUeDel
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by at the time of UE deletion.
 *        
 *     Processing Steps:
 *     - if (ue->tpcPucchRntiCb)
 *       - delete UE from tpcPucchRntiCb->ueLst
 *       - ue->tpcPucchRntiCb = NULLP
 *       - If in (ue->tpcPucchRntiCb->toBeSchdLst) 
 *         - remove from the list.
 *     - if (ue->tpcPuschRntiCb)
 *       - delete UE from tpcPuschRntiCb->ueLst
 *       - ue->tpcPuschRntiCb = NULLP
 *       - If in (ue->tpcPuschRntiCb->toBeSchdLst) 
 *         - remove from the list.
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @return  Void
 **/
Void rgSCHPwrUeDel(RgSchCellCb *cell,RgSchUeCb *ue)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, cell);

   if (uePwr->tpcPucchRntiCb)
   {
      rgSCHPwrDelUeFrmPucchTpcRntiCb(cell, uePwr->tpcPucchRntiCb, ue);
      uePwr->tpcPucchRntiCb = NULLP;
   }
   if (uePwr->tpcPuschRntiCb)
   {
      rgSCHPwrDelUeFrmPuschTpcRntiCb(cell, uePwr->tpcPuschRntiCb, ue);
      uePwr->tpcPuschRntiCb = NULLP;
   }
   return;
}

/**
 * @brief Resets UE's power state
 *
 * @details
 *
 *     Function : rgSCHPwrUeReset
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by at the time PDCCH order.
 *
 *     Processing Steps:
 *     - Reset PUSCH power state
 *     - Reset PUCCH power state
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @return  Void
 **/
Void rgSCHPwrUeReset(RgSchCellCb  *cell,RgSchUeCb  *ue)
{

   rgSCHPwrUeResetPucch(cell, ue);
   rgSCHPwrUeResetPusch(cell, ue);
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrUeResetPucch
 *
 *     Desc : This function is called to reset UE
 *            to initial PUCCH power state.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrUeResetPucch(RgSchCellCb *cell,RgSchUeCb *ue)
{
   RgSchCmnUeUlPwrCb     *uePwr       = RG_SCH_PWR_GETUEPWR(ue, cell);

   uePwr->pucchTpc    = 1;
   uePwr->remPucchPwr = 0;
   if (uePwr->tpcPucchRntiCb)
   {
      rgSCHPwrRmvSchdUeFrmPucchTpcRntiCb(cell, uePwr->tpcPucchRntiCb, ue);
   }

   /* Stack Crash problem for TRACE5 changes. Added the line below */
  return;
 
}

/***********************************************************
 *
 *     Func : rgSCHPwrUeResetPusch
 *
 *     Desc : This function is called to reset UE
 *            to initial PUSCH power state.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrUeResetPusch(RgSchCellCb *cell,RgSchUeCb *ue)
{
   RgSchCmnUeUlPwrCb     *uePwr       = RG_SCH_PWR_GETUEPWR(ue, cell);

   uePwr->isPhrAvail  = FALSE;
   uePwr->phVal       = 40;
   uePwr->maxUlRbs    = RGSCH_MAX_DL_BW;
   uePwr->delta       = 0;
   uePwr->puschTpc    = 1;
   uePwr->remPuschPwr = 0;
   if (uePwr->tpcPuschRntiCb)
   {
      rgSCHPwrRmvSchdUeFrmPuschTpcRntiCb(cell, uePwr->tpcPuschRntiCb, ue);
   }
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrOnPuschPwrUpd
 *
 *     Desc : This function is called whenever 'remPuschPwr'
 *            is updated
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrOnPuschPwrUpd(RgSchCellCb  *cell,RgSchUeCb  *ue)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, cell);
   RgSchCmnTpcRntiCb     *cb;
   Bool                   toAdd;

   if ((cb = uePwr->tpcPuschRntiCb) == NULLP)
   {
      return;
   }

   /* Not checking for uwPwr->isPhrAvail as uePwr->phVal
    * is set to a large value initially */
   toAdd = FALSE;


   if ((uePwr->phVal != 0) && (uePwr->remPuschPwr != 0))
      {
         toAdd = TRUE;
      }


   if (toAdd)
   {
      rgSCHPwrAddSchdUeToPuschTpcRntiCb(cell, cb, ue);
   }
   else
   {
      rgSCHPwrRmvSchdUeFrmPuschTpcRntiCb(cell, cb, ue);
   }

   return;
}


/***********************************************************
 *
 *     Func : rgSCHPwrAddRntiToPucchRntiLst
 *
 *
 *     Desc : Adds RNTI to Pucch Rnti list and updates requisite
 *            information. 
 *
 *     Ret  : Void
 *
 *     Notes: Assumed that array bounds are checked
 *            in caller before adding.
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrAddRntiToPucchRntiLst(RgSchCellCb *cell,CmLteRnti rnti,Bool  isFmt3a)
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);

   rgSCHPwrInitTpcRntiCb(&cellPwr->tpcPucchRntiLst[cellPwr->tpcPucchRntiCnt++],
       rnti, isFmt3a);
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrAddRntiToPuschRntiLst
 *
 *
 *     Desc : Adds RNTI to Pusch Rnti list and updates requisite
 *            information. 
 *
 *     Ret  : Void
 *
 *     Notes: Assumed that array bounds are checked
 *            in caller before adding.
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrAddRntiToPuschRntiLst(RgSchCellCb *cell,CmLteRnti rnti,Bool  isFmt3a)
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);

   rgSCHPwrInitTpcRntiCb(&cellPwr->tpcPuschRntiLst[cellPwr->tpcPuschRntiCnt++], 
       rnti, isFmt3a);
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrInitTpcRntiCb
 *
 *
 *     Desc : Initialises a TPC RNTI CB
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrInitTpcRntiCb(RgSchCmnTpcRntiCb *cb,CmLteRnti rnti,Bool isFmt3a)
{

   memset(cb, 0, sizeof(*cb));
   cb->tpcRnti = rnti;
   cb->isFmt3a = isFmt3a;
   /* Not initialising lists as memset 0 takes care of it */
   /* cb->schdLnk.node is set when this rnti is to be scheduled */
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrGetPucchRntiCb
 *
 *
 *     Desc : Gets TPC RNTI control block from Pucch rnti list 
 *
 *     Ret  : RgSchCmnTpcRntiCb * - Success
 *            NULLP - Fail
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static RgSchCmnTpcRntiCb* rgSCHPwrGetPucchRntiCb(RgSchCellCb *cell,CmLteRnti   tpcRnti)
{
   RgSchCmnUlPwrCb *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);
   uint16_t  idx;

   if (!cellPwr->tpcPucchRntiCnt)
   {
      return (NULLP);
   }
   for (idx = 0; idx < cellPwr->tpcPucchRntiCnt; ++idx)
   {
      if (cellPwr->tpcPucchRntiLst[idx].tpcRnti == tpcRnti)
      {
         return (&cellPwr->tpcPucchRntiLst[idx]);
      }
   }
   return (NULLP);
}

/***********************************************************
 *
 *     Func : rgSCHPwrGetPuschRntiCb
 *
 *
 *     Desc : Gets TPC RNTI control block from Pusch rnti list 
 *
 *     Ret  : RgSchCmnTpcRntiCb * - Success
 *            NULLP - Fail
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static RgSchCmnTpcRntiCb* rgSCHPwrGetPuschRntiCb(RgSchCellCb *cell,CmLteRnti   tpcRnti)
{
   RgSchCmnUlPwrCb *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);
   uint16_t idx;

   if (!cellPwr->tpcPuschRntiCnt)
   {
      return (NULLP);
   }
   for (idx = 0; idx < cellPwr->tpcPuschRntiCnt; ++idx)
   {
      if (cellPwr->tpcPuschRntiLst[idx].tpcRnti == tpcRnti)
      {
         return (&cellPwr->tpcPuschRntiLst[idx]);
      }
   }
   return (NULLP);
}


/***********************************************************
 *
 *     Func : rgSCHPwrAddUeToPucchTpcRntiCb
 *
 *
 *     Desc : Add UE to cfgd list of UEs in rnti cb
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrAddUeToPucchTpcRntiCb(RgSchCellCb *cell,RgSchCmnTpcRntiCb *cb,RgSchUeCb *ue)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, ue->cell);
   UNUSED(cell);

   cmLListAdd2Tail(&cb->cfgdUes, &uePwr->pucchGrpLnk);
   uePwr->pucchGrpLnk.node = (PTR)ue;
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrDelUeFrmPucchTpcRntiCb
 *
 *
 *     Desc : Remove UE from Pucch TPC RNTI CB
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrDelUeFrmPucchTpcRntiCb(RgSchCellCb *cell,RgSchCmnTpcRntiCb *cb,RgSchUeCb  *ue)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, ue->cell);

   rgSCHPwrRmvSchdUeFrmPucchTpcRntiCb(cell, cb, ue);
   cmLListDelFrm(&cb->cfgdUes, &uePwr->pucchGrpLnk);
   uePwr->pucchGrpLnk.node = NULLP;
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrRmvSchdUeFrmPucchTpcRntiCb
 *
 *
 *     Desc : Remove UE from to-be-scheduled list of UEs
 *            in Pusch RNTI CB
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrRmvSchdUeFrmPucchTpcRntiCb(RgSchCellCb *cell,RgSchCmnTpcRntiCb  *cb,RgSchUeCb *ue)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, ue->cell);

   if (uePwr->schdPucchGrpLnk.node == NULLP)
   {
      return;
   }
   rgSCHPwrRmvSchdUeOnlyFrmPucchTpcRntiCb(cell, cb, ue);
   if (!cb->toBeSchdUes.count)
   {
      rgSCHPwrRmvSchdPucchTpcRntiCb(cell, cb);
   }
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrRmvSchdUeOnlyFrmPucchTpcRntiCb
 *
 *     Desc : Remove UE from to-be-scheduled list of UEs
 *            in Pucch RNTI CB. Do not both about
 *            possibly removing Pucch RNTI CB from
 *            the cell wide to-be-scheduled list.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrRmvSchdUeOnlyFrmPucchTpcRntiCb(RgSchCellCb *cell,RgSchCmnTpcRntiCb  *cb,RgSchUeCb *ue)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, ue->cell);

   if (uePwr->schdPucchGrpLnk.node != NULLP)
   {
      cmLListDelFrm(&cb->toBeSchdUes, &uePwr->schdPucchGrpLnk);
      uePwr->schdPucchGrpLnk.node = NULLP;
   }
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrRmvSchdPucchTpcRntiCb
 *
 *     Desc : Remove Pucch TPC RNTI CB from to-be-scheduled
 *            list in cell
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrRmvSchdPucchTpcRntiCb(RgSchCellCb *cell,RgSchCmnTpcRntiCb *cb)
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);

   if (cb->schdLnk.node == NULLP)
   {
      return;
   }
   cmLListDelFrm(&cellPwr->pucchGrpPwr, &cb->schdLnk);
   cb->schdLnk.node = NULLP;
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrAddSchdUeToPucchTpcRntiCb
 *
 *     Desc : Add UE to to-be-scheduled list of UEs
 *            in Pucch RNTI CB
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrAddSchdUeToPucchTpcRntiCb(RgSchCellCb *cell,RgSchCmnTpcRntiCb *cb,RgSchUeCb *ue)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, ue->cell);

   if (uePwr->schdPucchGrpLnk.node != NULLP)
   {
      /* UE is already in the list */
      return;
   }
   cmLListAdd2Tail(&cb->toBeSchdUes, &uePwr->schdPucchGrpLnk);
   uePwr->schdPucchGrpLnk.node = (PTR)ue;
   if (cb->toBeSchdUes.count == 1)
   {
      /* This is first UE, so queue up this TPC RNTI
       * for scheduling */
      rgSCHPwrAddSchdPucchTpcRntiCb(cell, cb);
   }
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrAddSchdPucchTpcRntiCb
 *
 *     Desc : Add Pucch TPC RNTI CB from to-be-scheduled
 *            list in cell
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrAddSchdPucchTpcRntiCb(RgSchCellCb *cell,RgSchCmnTpcRntiCb *cb)
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);

   cmLListAdd2Tail(&cellPwr->pucchGrpPwr, &cb->schdLnk);
   cb->schdLnk.node = (PTR)cb;
   return;
}


/***********************************************************
 *
 *     Func : rgSCHPwrAddUeToPuschTpcRntiCb
 *
 *
 *     Desc : Add UE to cfgd list of UEs in rnti cb
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrAddUeToPuschTpcRntiCb(RgSchCmnTpcRntiCb *cb,RgSchUeCb  *ue)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, ue->cell);

   cmLListAdd2Tail(&cb->cfgdUes, &uePwr->puschGrpLnk);
   uePwr->puschGrpLnk.node = (PTR)ue;
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrAddSchdUeToPuschTpcRntiCb
 *
 *     Desc : Add UE to to-be-scheduled list of UEs
 *            in Pusch RNTI CB
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrAddSchdUeToPuschTpcRntiCb(RgSchCellCb *cell,RgSchCmnTpcRntiCb *cb,RgSchUeCb *ue)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, ue->cell);

   if (uePwr->schdPuschGrpLnk.node != NULLP)
   {
      /* UE is already in the list */
      return;
   }
   cmLListAdd2Tail(&cb->toBeSchdUes, &uePwr->schdPuschGrpLnk);
   uePwr->schdPuschGrpLnk.node = (PTR)ue;
   if (cb->toBeSchdUes.count == 1)
   {
      /* This is first UE, so queue up this TPC RNTI
       * for scheduling */
      rgSCHPwrAddSchdPuschTpcRntiCb(cell, cb);
   }
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrDelUeFrmPuschTpcRntiCb
 *
 *
 *     Desc : Add UE to cfgd list of UEs in rnti cb
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrDelUeFrmPuschTpcRntiCb(RgSchCellCb *cell,RgSchCmnTpcRntiCb  *cb,RgSchUeCb  *ue)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, ue->cell);

   rgSCHPwrRmvSchdUeFrmPuschTpcRntiCb(cell, cb, ue);
   cmLListDelFrm(&cb->cfgdUes, &uePwr->puschGrpLnk);
   uePwr->puschGrpLnk.node = NULLP;
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrRmvSchdUeFrmPuschTpcRntiCb
 *
 *     Desc : Remove UE from to-be-scheduled list of UEs
 *            in Pusch RNTI CB
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrRmvSchdUeFrmPuschTpcRntiCb(RgSchCellCb *cell,RgSchCmnTpcRntiCb *cb,RgSchUeCb *ue)
{
   RgSchCmnUeUlPwrCb     *uePwr       = RG_SCH_PWR_GETUEPWR(ue, ue->cell);

   if (uePwr->schdPuschGrpLnk.node == NULLP)
   {
      return;
   }
   rgSCHPwrRmvSchdUeOnlyFrmPuschTpcRntiCb(cell, cb, ue);
   if (!cb->toBeSchdUes.count)
   {
      rgSCHPwrRmvSchdPuschTpcRntiCb(cell, cb);
   }
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrRmvSchdUeOnlyFrmPuschTpcRntiCb
 *
 *     Desc : Remove UE from to-be-scheduled list of UEs
 *            in Pusch RNTI CB. Do not both about
 *            possibly removing Pusch RNTI CB from
 *            the cell wide to-be-scheduled list.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrRmvSchdUeOnlyFrmPuschTpcRntiCb(RgSchCellCb *cell,RgSchCmnTpcRntiCb *cb,RgSchUeCb *ue)
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, ue->cell);

   if (uePwr->schdPuschGrpLnk.node != NULLP)
   {
      cmLListDelFrm(&cb->toBeSchdUes, &uePwr->schdPuschGrpLnk);
      uePwr->schdPuschGrpLnk.node = NULLP;
   }
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrAddSchdPuschTpcRntiCb
 *
 *     Desc : Add Pusch TPC RNTI CB from to-be-scheduled
 *            list in cell
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrAddSchdPuschTpcRntiCb(RgSchCellCb *cell,RgSchCmnTpcRntiCb *cb)
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);

   cmLListAdd2Tail(&cellPwr->puschGrpPwr, &cb->schdLnk);
   cb->schdLnk.node = (PTR)cb;
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrRmvSchdPuschTpcRntiCb
 *
 *     Desc : Remove Pusch TPC RNTI CB from to-be-scheduled
 *            list in cell
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgSCHPwrRmvSchdPuschTpcRntiCb(RgSchCellCb *cell,RgSchCmnTpcRntiCb *cb)
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);

   if (cb->schdLnk.node == NULLP)
   {
      return;
   }
   cmLListDelFrm(&cellPwr->puschGrpPwr, &cb->schdLnk);
   cb->schdLnk.node = NULLP;
   return;
}

/***********************************************************
 *
 *     Func : rgSCHPwrChkPucchTpcRntiIdx
 *
 *     Desc : Validate that the given index is OK to
 *            be assigned to a new UE for the Pucch TPC
 *            RNTI
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHPwrChkPucchTpcRntiIdx(RgSchCmnTpcRntiCb *cb,uint8_t idx)
{

   if (rgSCHPwrChkTpcRntiIdx(cb, idx) != ROK)
   {
      return RFAILED;
   }
   if (rgSCHPwrChkUniqPucchTpcRntiIdx(cb, idx) != ROK)
   {
      return RFAILED;
   }
   return ROK;
}

/***********************************************************
 *
 *     Func : rgSCHPwrChkPuschTpcRntiIdx
 *
 *     Desc : Validate that the given index is OK to
 *            be assigned to a new UE for the Pusch TPC
 *            RNTI
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHPwrChkPuschTpcRntiIdx(RgSchCmnTpcRntiCb *cb,uint8_t  idx)
{

   if (rgSCHPwrChkTpcRntiIdx(cb, idx) != ROK)
   {
      return RFAILED;
   }
   if (rgSCHPwrChkUniqPuschTpcRntiIdx(cb, idx) != ROK)
   {
      return RFAILED;
   }
   return ROK;
}

/***********************************************************
 *
 *     Func : rgSCHPwrChkUniqPucchTpcRntiIdx
 *
 *     Desc : Validate index against format type of TPC RNTI
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHPwrChkUniqPucchTpcRntiIdx(RgSchCmnTpcRntiCb *cb,uint8_t idx)
{
   CmLList           *lnk;

   for (lnk = cb->cfgdUes.first; lnk; lnk = lnk->next)
   {
      RgSchUeCb             *ue = (RgSchUeCb *)lnk->node;
      RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, ue->cell);
      if (uePwr->pucchIdx == idx)
      {
         return RFAILED;
      }
   }
   return ROK;
}

/***********************************************************
 *
 *     Func : rgSCHPwrChkUniqPuschTpcRntiIdx
 *
 *     Desc : Validate index against format type of TPC RNTI
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHPwrChkUniqPuschTpcRntiIdx(RgSchCmnTpcRntiCb  *cb,uint8_t  idx)
{
   CmLList           *lnk;

   for (lnk = cb->cfgdUes.first; lnk; lnk = lnk->next)
   {
      RgSchUeCb             *ue = (RgSchUeCb *)lnk->node;
      RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue, ue->cell);
      if (uePwr->puschIdx == idx)
      {
         return RFAILED;
      }
   }
   return ROK;
}

/***********************************************************
 *
 *     Func : rgSCHPwrChkTpcRntiIdx
 *
 *     Desc : Validate index against format type of TPC RNTI.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgSCHPwrChkTpcRntiIdx(RgSchCmnTpcRntiCb *cb,uint8_t idx)
{
   if (cb->isFmt3a)
   {
      if (idx >= TFU_MAX_1BIT_TPC)
      {
         return RFAILED;
      }
   }
   else
   {
      if (idx >= TFU_MAX_2BIT_TPC)
      {
         return RFAILED;
      }
   }
   return ROK;
}
/* Warning Fix: Commenting out as not used */

/***********************************************************
 *
 *     Func : rgSCHPwrGetPCMaxValFromPCMax
 *
 *     Desc : Returns the power headroom in dB
 *            corresponding to a power headroom
 *            report
 *
 *     Ret  : S8
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S8 rgSCHPwrGetPCMaxValFromPCMax(uint8_t pCMax)
{
   return ((pCMax & 63) - 30);
}



/***********************************************************
 *
 *     Func : rgSCHPwrGetPhValFromPhr
 *
 *     Desc : Returns the power headroom in dB
 *            corresponding to a power headroom
 *            report
 *
 *     Ret  : S8
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S8 rgSCHPwrGetPhValFromPhr(uint8_t  phr)
{
   return ((phr & 63) - 23);
}



/**********************************************************************
 
         End of file
**********************************************************************/
