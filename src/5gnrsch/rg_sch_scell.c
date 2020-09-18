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

     Desc:     C source code for Round Robin functions

     File:     rg_sch_scell.c

**********************************************************************/

/** @file rg_sch_rr.c
@brief This module handles the round robin scheduler functionality
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

#ifdef LTE_ADV

Void rgSCHSCellActivation ARGS((
RgSchUeCellInfo  *sCell
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
U8                tbIdx,
U8                fdbk,
Bool              maxHqRetxReached
));

#ifdef LTE_ADV
Void rgSCHSCellDeactTmrExpry ARGS((
RgSchUeCellInfo *sCell
));
#endif

Void rgSCHSCellDelUeSCell ARGS((
RgSchCellCb  *cellCb,
RgSchUeCb    *ueCb,
U8            sCellIdx
));

S16 rgSCHSCellDelUe ARGS((
RgSchCellCb  *cellCb,
RgSchUeCb    *ueCb
));
#ifdef TFU_UPGRADE
S16 rgSCHSCellPCqiCfg ARGS((
RgSchCellCb  *priCellCb,
RgSchCellCb  *secCellCb,
RgSchUeCb    *ueCb,
RgrUePrdDlCqiCfg  *cqiCfg,
CmLteUeCategory   ueCat,
U8            sCellIdx
));
#endif
PRIVATE S16 rgSCHSCellTrgMacHqEReset ARGS((
Inst          inst,
U16           secCellId,
U16           rnti
));



/** * @brief Handler for scheduling Scell Activation CE.
 *
 * @details
 *
 *     Function : rgSCHDhmShcdSCellActCe
 *     
 *     This function is called by scheduler when resource allocation
 *     for SCell Activation CE transmission is done.
 *           
 *  @param[in]  RgSchUeCb         *ue
 *  @param[out] RgSchDlHqTbCb     *tbInfo
 *  @return     Void
 *      -# None.
 **/
#ifdef ANSI
Void rgSCHSCellSchdActDeactCe
(
RgSchUeCb         *ueCb,
RgSchDlHqTbCb     *tbInfo
)
#else
Void rgSCHSCellSchdActDeactCe(ueCb, tbInfo)
RgSchUeCb         *ueCb;
RgSchDlHqTbCb     *tbInfo;
#endif
{

   U8   bitVal = 0;
   U8   sCellActDeactBitMask = 0;
   TRC3(rgSCHSCellSchdActDeactCe);

   /* Change the state of all Scells waiting for
    * activation */

   /* -------------------------
    * | C7|C6|C5|C4|C3|C2|C1|R|
    * -------------------------*/
    /* 1 for activation
     * 0 for deactivation
     * */

   for(U8 idx = 1; idx <= RG_SCH_MAX_SCELL ; idx++)
   {
      if(ueCb->cellInfo[idx] != NULLP)
      {
         switch(ueCb->cellInfo[idx]->sCellState)
         {
            case RG_SCH_SCELL_TOBE_ACTIVATED:
            case RG_SCH_SCELL_ACTVTN_IN_PROG:
               {
                  ueCb->cellInfo[idx]->sCellState = RG_SCH_SCELL_ACTVTN_IN_PROG; 
                  bitVal = 1;
               }
               break;
            case RG_SCH_SCELL_ACTIVE:
               {
                  bitVal = 1;
               }
               break;
            case RG_SCH_SCELL_TOBE_DEACTIVATED:
            case RG_SCH_SCELL_DEACTVTN_IN_PROG:
               {
                  ueCb->cellInfo[idx]->sCellState = RG_SCH_SCELL_DEACTVTN_IN_PROG; 
                  bitVal = 0;
               }
               break;
            case RG_SCH_SCELL_INACTIVE:
            case RG_SCH_SCELL_READY:
               {
                  bitVal = 0;
               }
               break;
         }
      }
      if(1 == bitVal)
      {
         sCellActDeactBitMask |= 1 << (idx);/* servCellIdx = idx + 1 */
         bitVal = 0;
      }
   }
   tbInfo->schdSCellActCe.pres    = PRSNT_NODEF;
   tbInfo->schdSCellActCe.val     = sCellActDeactBitMask;

   RETVOID;
} /* rgSCHSCellSchdActDeactCe */


/**
 * @brief Adds an UE to the Cell's SCell Activation list
 *
 * @details
 *
 *     Function: rgSCHSCellAddToActDeactLst
 *     Purpose:  Adds an UE to Cell's SCEll Act list
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSchUeCb*       ue
 *  @return  Void
 *
 **/
#ifdef ANSI
Void rgSCHSCellAddToActDeactLst
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue
)
#else
Void rgSCHSCellAddToActDeactLst(cell, ue)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
#endif
{
   RgSchCmnDlCell *cellCmnDl = RG_SCH_CMN_GET_DL_CELL(cell);
   TRC3(rgSCHSCellAddToActDeactLst);

   if(NULLP == ue->sCellActLnk.node)
   {/* Ue is not present in the list */
      cmLListAdd2Tail(&cellCmnDl->secCellActCeLst, &ue->sCellActLnk);
      ue->sCellActLnk.node = (PTR)ue;
   }
   else
   {
      RGSCHDBGINFONEW(cell->instIdx,(rgSchPBuf(cell->instIdx),
            "SCell is already added in the Act List: ueId(%u)\n", ue->ueId));
   }
   
   RETVOID;
}


/**
 * @brief Removes an UE from Cell's SCell Activation list
 *
 * @details
 *
 *     Function: rgSCHSCellRmvFrmActLst
 *     Purpose:  Removes an UE from Cell's SCEll Act list
 *
 *     Invoked by: Specific Scheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSchUeCb*       ue
 *  @return  Void
 *
 **/
#ifdef ANSI
Void rgSCHSCellRmvFrmActLst
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue
)
#else
Void rgSCHSCellRmvFrmActLst(cell, ue)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
#endif
{
   RgSchCmnDlCell *cellCmnDl = RG_SCH_CMN_GET_DL_CELL(cell);
   TRC3(rgSCHSCellRmvFrmActLst);
   if (NULLP != ue->sCellActLnk.node)
   {
      cmLListDelFrm(&cellCmnDl->secCellActCeLst, &ue->sCellActLnk);
   }
   ue->sCellActLnk.node = (PTR)NULLP;

   RETVOID;
}

/**
 * @brief Handling of SCell Activation
 *
 * @details
 *
 *     Function: rgSCHSCellActivation
 *     Purpose : Perform Activation of secondary cell
 *             : Move the state to ACTIVE
 *             : Start the procedure for PCQI/SRS for this scell
 *
 *     Invoked by:Cfg/Commn Scheduler 
 *
 *  @param[in]  RgSchUeCellInfo *sCellInfo
 *
 *  @return  ROK/RFAILED
 *
 **/
#ifdef ANSI
Void rgSCHSCellActivation
(
RgSchUeCellInfo  *sCellInfo
)
#else
Void rgSCHSCellActivation(sCellInfo)
RgSchUeCellInfo  *sCellInfo
#endif
{
   RgSchCellCb  *sCell = sCellInfo->cell;
   RgSchUeCb    *ueCb = sCellInfo->ue;
   RgSchCmnCell *cellSch;
#ifdef TFU_UPGRADE
#ifdef DEBUGP
   Inst   inst = ueCb->cell->instIdx;
#endif
   U16    tempIdx; 
   RgrUePrdDlCqiCfg  *cqiCfg;
   U8     j;  /*Bandwidth Parts*/
   U16    riTrInsTime; 
   U16    periodicity; 
   U16    cqiTrInstTime; 
   RgSchUePCqiCb *cqiCb = NULLP;
   CmLteTimingInfo timingInfo;
   U16    crntTime;           
#endif

   TRC3(rgSCHSCellActivation);

   sCellInfo->sCellState = RG_SCH_SCELL_ACTIVE;
#ifdef TENB_STATS
   ueCb->tenbStats->stats.persistent.numActivation++;
#endif

#ifdef CA_DBG 
   printf("ueId is SCELL_ACTIVE\n ueCb->ueId = %d sCell->sCellIdx =%d, sCell->sCellId=%d, sCell->sCellState=%d \n", ueCb->ueId, sCellInfo->sCellIdx, sCellInfo->sCellId, sCellInfo->sCellState);
#endif
   /* Start the sCellDeactivation timer if cfgd */
   if(PRSNT_NODEF == ueCb->sCellDeactTmrVal.pres)
   {
      //rgSCHTmrStartTmr (sCell,sCellInfo ,RG_SCH_TMR_SCELL_DEACT,
        //    ueCb->sCellDeactTmrVal.val);
   }

#ifdef TFU_UPGRADE
   /* Start receiving CQI for this SCell for this UE */
   crntTime = (ueCb->cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G)+
                  (ueCb->cell->crntTime.slot);

   cqiCb = &sCellInfo->cqiCb;
   cqiCfg = &cqiCb->cqiCfg;
   if (cqiCfg->type == RGR_SCH_PCQI_SETUP)
   {
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

      timingInfo.sfn =  cqiCb->nCqiTrIdx/RGSCH_NUM_SUB_FRAMES_5G;
      timingInfo.slot =  cqiCb->nCqiTrIdx%RGSCH_NUM_SUB_FRAMES_5G;
      if(cqiCb->cqiCfg.cqiSetup.cqiRepType == RGR_UE_PCQI_SB_REP)
      {
         rgSCHTomUtlPcqiSbCalcBpIdx(timingInfo,ueCb,cqiCb); 
      }

      cqiCb->nCqiTrIdx = cqiCb->nCqiTrIdx
         %RG_SCH_PCQI_SRS_SR_TRINS_SIZE;
      RGSCHDBGINFONEW(inst, (rgSchPBuf(inst), "CQI Config: idx(%u) Periodicity %u"
               "Offset %u uePosInQ (%u)\n", cqiCfg->cqiSetup.cqiPCfgIdx,
               cqiCb->cqiPeri, cqiCb->cqiOffset,cqiCb->nCqiTrIdx));

      cmLListAdd2Tail(&ueCb->cell->pCqiSrsSrLst[cqiCb->nCqiTrIdx].cqiLst,
            &cqiCb->cqiLstEnt);  

      rgSCHUtlSCellHndlCqiCollsn(cqiCb);

      RGSCHDBGINFO(inst,(rgSchPBuf(inst), 
               "\n rgSCHCfgPCqiUeCfg():"
               "  CrntTime=%d  Next CqiTrInstTime=%d  Index Stored at=%d  ",
               crntTime, cqiTrInstTime, cqiCb->nCqiTrIdx));

      if(cqiCfg->cqiSetup.riEna)
      {
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

            RG_SCH_GET_CQI_J_VAL(sCell->bwCfg.dlTotalBw, j);
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
         }
         else
         {
            riTrInsTime = ((periodicity +crntTime )- \
                  cqiCb->cqiOffset + cqiCb->riOffset)\
                          % periodicity;
            tempIdx = (crntTime + (periodicity -riTrInsTime));
         }
         if (tempIdx <= (crntTime + TFU_RECPREQ_DLDELTA))
         {
            tempIdx = tempIdx + periodicity; 
         }
         cqiCb->nRiTrIdx = tempIdx 
            % RG_SCH_PCQI_SRS_SR_TRINS_SIZE;
         if(periodicity >= RG_SCH_PCQI_SRS_SR_TRINS_SIZE)
         {  
            cqiCb->riDist = rgSCHUtlFindDist((U16)(crntTime + TFU_RECPREQ_DLDELTA),
                  (U16) tempIdx);
         }
         else
         {
            cqiCb->riDist =0; 
         }


         /* Start receiving RI for this SCell for this UE */
         cmLListAdd2Tail(&ueCb->cell->pCqiSrsSrLst[cqiCb->nRiTrIdx].riLst,
               &cqiCb->riLstEnt);  
         RG_SCH_RECORD(&cqiCb->histElem,RGSCH_ACTION_ADD,
            &ueCb->cell->pCqiSrsSrLst[cqiCb->nRiTrIdx].riLst);

         rgSCHUtlSCellHndlRiCollsn(cqiCb);
         /*werror*/
#ifndef BIT_64
         RGSCHDBGINFONEW(inst,(rgSchPBuf(inst), "SCel RI cfg:"
                  "idx %u period %u Offset %u posInQ(%u) riDist(%u)lst count"
                  "%lu\n", cqiCfg->cqiSetup.riCfgIdx, cqiCb->riPeri,
                  cqiCb->riOffset, cqiCb->nRiTrIdx, cqiCb->riDist, 
                  ueCb->cell->pCqiSrsSrLst[cqiCb->nRiTrIdx].riLst.count));
#else
         RGSCHDBGINFONEW(inst,(rgSchPBuf(inst), "SCel RI cfg:"
                  "idx %u period %u Offset %u posInQ(%u) riDist(%u)lst count"
                  "%u\n", cqiCfg->cqiSetup.riCfgIdx, cqiCb->riPeri,
                  cqiCb->riOffset, cqiCb->nRiTrIdx, cqiCb->riDist, 
                  ueCb->cell->pCqiSrsSrLst[cqiCb->nRiTrIdx].riLst.count));


#endif

         RGSCHDBGINFO(inst,(rgSchPBuf(inst),
                  "\n rgSCHSCellActivation(): CrntTime=%d Next RiTrInstTime=%d"
                  "Index Stored at=%d riDis=%d ",
                  crntTime, riTrInsTime, cqiCb->nRiTrIdx, cqiCb->riDist));
      }
   }
#endif

   cellSch = RG_SCH_CMN_GET_CELL(sCellInfo->cell);
   cellSch->apisDl->rgSCHDlSCellActv(sCellInfo->cell, sCellInfo->ue);

   RETVOID;
}

#ifdef TFU_UPGRADE

/**
 * @brief Remove CQI from Scell Lst
 *
 * @details
 *
 *     Function: rgSCHCellClearScellLstOfCQI
 *     Purpose : Remove CQI from Scell Lst
 *              
 *
 *     Invoked by: Timer
 *
 *  @param[in]  RgSchUeCellInfo *sCellInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCellClearScellLstOfCQI
(
RgSchUeCellInfo *sCellInfo
)
#else
PRIVATE Void rgSCHCellClearScellLstOfCQI(sCellInfo)
RgSchUeCellInfo *sCellInfo;
#endif
{

   TRC3(rgSCHCellClearScellLstOfCQI);
   RgSchUePCqiCb *cqiRiCb = NULLP;
   RgSchUeCb    *ueCb;
   ueCb = sCellInfo->ue;
   /* Clear CQI/RI entry for this SCELL */
   cqiRiCb = &sCellInfo->cqiCb;
   /* Delete Periodic CQI/PMI  Transmission Instance  */
   if (cqiRiCb->nCqiTrIdx != RG_SCH_INVALID_IDX)
   {
      cmLListDelFrm(&ueCb->cell->pCqiSrsSrLst[cqiRiCb->nCqiTrIdx].cqiLst, 
            &cqiRiCb->cqiLstEnt); 
      cqiRiCb->nCqiTrIdx = RG_SCH_INVALID_IDX;

      if (ueCb->nPCqiCb == cqiRiCb)
      {
         rgSCHUtlSCellHndlCqiCollsn(&ueCb->cellInfo[RGSCH_PCELL_INDEX]->cqiCb);
      }
      /* Delete Periodic  RI  Transmission Instance  */

      if (cqiRiCb->nRiTrIdx != RG_SCH_INVALID_IDX)
      {
         cmLListDelFrm(&ueCb->cell->pCqiSrsSrLst[cqiRiCb->nRiTrIdx].riLst, 
               &cqiRiCb->riLstEnt); 
         RG_SCH_RECORD(&cqiRiCb->histElem,RGSCH_ACTION_DEL,
            &ueCb->cell->pCqiSrsSrLst[cqiRiCb->nRiTrIdx].riLst);
         cqiRiCb->nRiTrIdx = RG_SCH_INVALID_IDX;
         if (ueCb->nPRiCb == cqiRiCb)
         {
            rgSCHUtlSCellHndlRiCollsn(&ueCb->cellInfo[RGSCH_PCELL_INDEX]->cqiCb);
         }
      }
   }

   RETVOID;
}
#endif/*TFU_UPGRADE*/

/**
 * @brief Handling of SCell DeActivation
 *
 * @details
 *
 *     Function: rgSCHSCellDeActivation
 *     Purpose : Perform Deactivation of secondary cell
 *             : Move the state to IN_ACTIVE
 *             : Flush the harqEntity
 *             : Trigger harqEntity flushing to MAC
 *             : Remove  PCQI/SRS for this scell
 *             : Stop Deactivation timer if running
 *
 *     Invoked by:Cfg/Commn Scheduler 
 *
 *  @param[in]  RgSchUeCellInfo *sCellInfo
 *
 *  @return  ROK/RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHSCellDeActivation
(
RgSchUeCellInfo *sCellInfo
)
#else
PRIVATE S16 rgSCHSCellDeActivation(sCellInfo)
RgSchUeCellInfo *sCellInfo
#endif
{
   return ROK;
   RgSchCmnCell *cellSch;
   Inst inst = sCellInfo->cell->instIdx;

   TRC3(rgSCHSCellDeActivation);
   /* Stop the timer if running */

   if(sCellInfo->deactTmr.tmrEvnt != TMR_NONE)
   {
      rgSCHTmrStopTmr(sCellInfo->cell, RG_SCH_TMR_SCELL_DEACT, sCellInfo);
   }

   if (sCellInfo->actDelayTmr.tmrEvnt != TMR_NONE)
   {
      rgSCHTmrStopTmr(sCellInfo->cell, RG_SCH_TMR_SCELL_ACT_DELAY, sCellInfo);
   }

   cellSch = RG_SCH_CMN_GET_CELL(sCellInfo->cell);
   cellSch->apisDl->rgSCHDlUeReset(sCellInfo->cell, sCellInfo->ue);

   if(sCellInfo->ue->isDrxEnabled)
   {   
      rgSCHDrxUeHqReset(sCellInfo->ue->cell, sCellInfo->ue, 
                        sCellInfo->hqEnt, sCellInfo->sCellIdx);
   }

   /* Flush the harqEntity at scheduler */
   if(sCellInfo->hqEnt != NULLP) 
   {
      rgSCHDhmHqEntReset(sCellInfo->hqEnt);
   }
   /* Trigger harq flush req to MAC */


   rgSCHSCellTrgMacHqEReset(inst,sCellInfo->sCellId,sCellInfo->ue->ueId);
   
   sCellInfo->sCellState = RG_SCH_SCELL_READY;
#ifdef TFU_UPGRADE
   rgSCHCellClearScellLstOfCQI(sCellInfo);
#endif

#ifdef TENB_STATS
   sCellInfo->ue->tenbStats->stats.persistent.numDeactivation++;
#endif

   cellSch->apisDl->rgSCHDlSCellDeactv(sCellInfo->cell, sCellInfo->ue);

#ifdef CA_DBG 
   printf("SCELL DEATIVATED  sCellInfo->ue->ueId =%d, sCellInfo->sCellId =%d\n", sCellInfo->ue->ueId, sCellInfo->sCellId);
   //MSPD_DBG("SCELL DEATIVATED  sCellInfo->ue->ueId =%d, sCellInfo->sCellId =%d\n", sCellInfo->ue->ueId, sCellInfo->sCellId);
#endif
   return ROK;
}


/**
 * @brief Triggering hqEntity reset to mac
 *
 * @details
 *
 *     Function: rgSCHSCellTrgMacHqEReset
 *     Purpose: Frame the interface for mac to reset
 *              the mac
 *              Derive the macInstance corresponding
 *              to the secondary cell going to be deactivated.
 *              Triiger the msg to that macInstance
 *
 *     Invoked by: CommonScheduler
 *
 *  @param[in]  U16        sCellId
 *  @param[in]  U16        rnti
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHSCellTrgMacHqEReset
(
Inst          inst,
U16           secCellId,
U16           rnti
)
#else
PRIVATE S16 rgSCHSCellTrgMacHqEReset(inst,secCellId,rnti)
Inst          inst;
U16           secCellId;
U16           rnti;
#endif
{
   Pst               pst;
   RgSchCellCb      *secCellCb = NULLP;
   RgInfResetHqEnt   hqEntRstInfo;

   if((secCellCb = (RgSchCellCb *)rgSchUtlGetCellCb(inst, secCellId)) == NULLP)
   {
      RGSCHDBGERRNEW(inst, (rgSchPBuf(inst), "SCell doesnt exists"));
      return RFAILED;
   }

   hqEntRstInfo.cellId = secCellId;
   hqEntRstInfo.crnti  = rnti;

   rgSCHUtlGetPstToLyr(&pst, &rgSchCb[inst], secCellCb->macInst);

   RgSchMacRstHqEnt(&pst, &hqEntRstInfo);

   return ROK;
}
/*removed endif*/



/**
 * @brief Handling of harq feeback for SCell act CE txion
 *
 * @details
 *
 *     Function: rgSCHSCellHndlFdbkInd
 *     Purpose:  Handling the harq feedback for SCell ACT ce txion
 *               ACK:: Set the state as active for the Scells for which
 *                     CE was sent
 *               HQ FAILURE/DTX/NACK:: Perform retxion. Add to Act CE list
 *                                   Set the state to TOBE_SCHEDULED
 *               
 *
 *     Invoked by: CommonScheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSchUeCb*       ue
 *  @return  Void
 *
 **/
#ifdef ANSI
Void rgSCHSCellHndlFdbkInd
(
RgSchDlHqProcCb   *hqP,
U8                tbIdx,
U8                fdbk,
Bool              maxHqRetxReached
)
#else
Void rgSCHSCellHndlFdbkInd(hqP, tbIdx, fdbk,maxHqRetxReached)
RgSchDlHqProcCb   *hqP;
U8                tbIdx;
U8                fdbk;
Bool              maxHqRetxReached;
#endif
{

   RgSchUeCb      *ueCb;
   RgSchCellCb    *cell;
   RgSchUeCellInfo *sCellInfo;

   TRC3(rgSCHSCellHndlFdbkInd);

   ueCb  = hqP->hqE->ue;
   cell  = ueCb->cell;
   switch(fdbk)
   {
      case TFU_HQFDB_ACK:
         {
            hqP->tbInfo[tbIdx].schdSCellActCe.pres =  FALSE;

            for(U8 idx = 1; idx <= RG_SCH_MAX_SCELL ; idx++)
            {
               if(ueCb->cellInfo[idx] != NULLP) 
               {
                  if(ueCb->cellInfo[idx]->sCellState == RG_SCH_SCELL_ACTVTN_IN_PROG)
                  {
#ifdef CA_DBG
                     printf("\n starting delay timer...\n");
#endif                     
                     rgSCHTmrStartTmr (cell,ueCb->cellInfo[idx] ,RG_SCH_TMR_SCELL_ACT_DELAY,
                           RG_SCH_CMN_SCELL_ACT_DELAY_TMR);
                  }
                  else
                  {
                     if(ueCb->cellInfo[idx]->sCellState == RG_SCH_SCELL_DEACTVTN_IN_PROG)
                     {
                        sCellInfo = ueCb->cellInfo[idx];
                        rgSCHSCellDeActivation(sCellInfo);
                     }
                  }
               }
            }
         }
         break;
      case TFU_HQFDB_NACK:
      case TFU_HQFDB_DTX:
         {
            if(TRUE == maxHqRetxReached)
            {
               hqP->tbInfo[tbIdx].schdSCellActCe.pres =  FALSE;
               for(U8 idx = 1; idx <= RG_SCH_MAX_SCELL ; idx++)
               {
                  if(ueCb->cellInfo[idx] != NULLP) 
                  {
                     if(ueCb->cellInfo[idx]->sCellState == RG_SCH_SCELL_ACTVTN_IN_PROG)
                     {
                        ueCb->cellInfo[idx]->sCellState = RG_SCH_SCELL_TOBE_ACTIVATED;
                     }
                     else
                     {
                        if(ueCb->cellInfo[idx]->sCellState == RG_SCH_SCELL_DEACTVTN_IN_PROG)
                        {
                           ueCb->cellInfo[idx]->sCellState = RG_SCH_SCELL_TOBE_DEACTIVATED;
                        }
                     }
                  }
                  /* Add to actDeactCe lst */
                  rgSCHSCellAddToActDeactLst(cell,ueCb);
               }
            }
         }
         break;
      default:
         break;
   }
   RETVOID;
}

#ifdef LTE_ADV
/**
 * @brief Handling of SCell Deactivation Tmr Expiry
 *
 * @details
 *
 *     Function: rgSCHSCellDeactTmrExpry
 *     Purpose : Deactivating the SCell. a
 *               Clear all the Harq Procs associated with this
 *               scell.
 *               Trigger Harq Reset to the respective MAC
 *               Set the state of the cell to Inactive  
 *              
 *
 *     Invoked by: Timer
 *
 *  @param[in]  RgSchUeCellInfo *sCellInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
Void rgSCHSCellDeactTmrExpry
(
RgSchUeCellInfo *sCellInfo
)
#else
Void rgSCHSCellDeactTmrExpry(sCellInfo)
RgSchUeCellInfo *sCellInfo;
#endif
{

   TRC3(rgSCHSCellDeactTmrExpry);
   if (sCellInfo->ue->isScellExplicitDeAct == TRUE)
   {
      /* Deactivation Timer is not configured (infinity), thus send deactivation CE explicitly */ 
      /* No doing Deactivaiton of LAA Cell */
      if (FALSE == rgSCHLaaSCellEnabled(sCellInfo->cell))
      {
         rgSCHSCellTrigActDeact(sCellInfo->ue->cell, sCellInfo->ue, sCellInfo->sCellIdx, RGR_SCELL_DEACT);
      }
      else
      {
         printf (" !!!!!! Avoiding DEACT for UE %d because of LAA Cell !!!!!!!!!!!!! \n",
         sCellInfo->ue->ueId);
      }

   }
   else
   {
      /* Deactivation Timer is configured, thus assume that UE has deactivated */ 
      rgSCHSCellDeActivation(sCellInfo);
   }
   RETVOID;
}
#endif

/**
 * @brief This function handles the action of the SCell
 *
 * @details
 *
 *     Function: rgSCHSCellTrigActDeact
 *     Purpose :  
 *        1) Prepares SCELL ready for activation OR
 *        2) Initiates activation of SCELL OR
 *        3) Initiate deactivation of SCELL OR
 *
 *     Invoked by:Cfg/Commn Scheduler 
 *
 *  @param[in]  RgSchCellCb *cellCb
 *  @param[in]  RgSchUeCb   *ueCb
 *  @param[in]  U8           sCellIdx
 *  @param[in]  U8           action
 *
 *  @return  ROK/RFAILED
 *
 **/
#ifdef ANSI
S16 rgSCHSCellTrigActDeact
(
RgSchCellCb  *cell,
RgSchUeCb    *ueCb,
U8            sCellIdx,
U8            action
)
#else
S16 rgSCHSCellTrigActDeact(cell,ueCb,sCellIdx,action)
RgSchCellCb  *cell,
RgSchUeCb    *ueCb;
U8            sCellIdx;
U8            action;
#endif
{
   Inst inst = cell->instIdx;
   S16 ret   = ROK;

   TRC3(rgSCHSCellTrigActDeact);

   if((sCellIdx < 1) ||
      (sCellIdx > RGR_MAX_SCELL_PER_UE))
   {
      RGSCHDBGERRNEW(inst, (rgSchPBuf(inst),"Invalid Serv Cell Idx %d\n", \
               sCellIdx));
      return RFAILED;
   }

   if(ueCb->cellInfo[sCellIdx] == NULLP)
   {
      RGSCHDBGERRNEW(inst, (rgSchPBuf(inst),"Serv Cell not added to this Ue Scell Idx %d ueId %d\n", \
              sCellIdx,ueCb->ueId));
      return RFAILED;
   }

   switch (action)
   {
      case RGR_SCELL_READY:
      {
         if(ueCb->cellInfo[sCellIdx]->sCellState != RG_SCH_SCELL_INACTIVE)
         {
            RGSCHDBGERRNEW(inst, (rgSchPBuf(inst),"Invalid state %u for preparing SCell Idx %u for UE %u\n", \
                     ueCb->cellInfo[sCellIdx]->sCellState, sCellIdx, ueCb->ueId));
            ret = RFAILED;
         }
         else
         {
            ueCb->cellInfo[sCellIdx]->sCellState = RG_SCH_SCELL_READY;
            //TODO_SID Activating the cell directly. Ignoring the ActCe procedure.
            rgSCHSCellActivation(ueCb->cellInfo[sCellIdx]);
            /* Setting allocCmnUlPdcch flag to FALSE, So that PDCCH allocation will be done 
               from UE Searchspace */
            ueCb->allocCmnUlPdcch = FALSE;
            printf("\n***** SCellIdx=%d state Changed to %d State \n",sCellIdx, ueCb->cellInfo[sCellIdx]->sCellState);
            printf("\n***** SCellInfo Addr=%p state Changed to RG_SCH_SCELL_READY\n",(void*)ueCb->cellInfo[sCellIdx]);
         }
         break;
      }
      case RGR_SCELL_ACT:
      {
         if(ueCb->cellInfo[sCellIdx]->sCellState != RG_SCH_SCELL_READY)
         {
            RGSCHDBGERRNEW(inst, (rgSchPBuf(inst),"Invalid state %u for activating SCell Idx %u for UE %u\n", \
                     ueCb->cellInfo[sCellIdx]->sCellState, sCellIdx, ueCb->ueId));
            ret = RFAILED;
         }
         else
         {
            ueCb->cellInfo[sCellIdx]->sCellState = RG_SCH_SCELL_TOBE_ACTIVATED;
            if (NULLP == ueCb->sCellActLnk.node)
            {
               /* Add only if UE is not already present in the activation/deactivation list */
               rgSCHSCellAddToActDeactLst(cell,ueCb);
            }
         }
         break;
      }
      case RGR_SCELL_DEACT:
      {
         if(ueCb->cellInfo[sCellIdx]->sCellState != RG_SCH_SCELL_ACTIVE)
         {
            RGSCHDBGERRNEW(inst, (rgSchPBuf(inst),"Invalid state %u for deactivating SCell Idx %u for UE %u\n", \
                     ueCb->cellInfo[sCellIdx]->sCellState, sCellIdx, ueCb->ueId));
            ret = RFAILED;
         }
         else
         {
            ueCb->cellInfo[sCellIdx]->sCellState = RG_SCH_SCELL_TOBE_DEACTIVATED;
            if (NULLP == ueCb->sCellActLnk.node)
            {
               /* Add only if UE is not already present in the activation/deactivation list */
               rgSCHSCellAddToActDeactLst(cell,ueCb);
            }
         }
         break;
      }
      default:
      {
         RGSCHDBGERRNEW(inst, (rgSchPBuf(inst),"Invalid action received for SCell Idx %u for UE %u\n", \
                  sCellIdx, ueCb->ueId));
         ret = RFAILED;
         break;
      }
   }
   return (ret);
}

 
/**
 * @brief SCell Activation of selected cell
 *
 * @details
 *
 *     Function: rgSCHSCellSelectForAct
 *     Purpose :  Perform Selection of secondary cell for activation
 *
 *     Invoked by:Cfg/Commn Scheduler 
 *
 *  @param[in]  RgSchCellCb *cellCb
 *  @param[in]  RgSchUeCb   *ueCb
 *
 *  @return  ROK/RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHSCellSelectForAct
(
RgSchCellCb  *cell,
RgSchUeCb    *ueCb,
U8           *sCellIdx
)
#else
PRIVATE S16 rgSCHSCellSelectForAct(cell, ueCb)
RgSchCellCb  *cell;
RgSchUeCb    *ueCb;
U8           *sCellIdx;
#endif
{
   TRC3(rgSCHSCellSelectAndAct);

   for((*sCellIdx) = 1; (*sCellIdx) <= RG_SCH_MAX_SCELL; (*sCellIdx)++)
   {
      if((ueCb->cellInfo[(*sCellIdx)] != NULLP) &&
            (ueCb->cellInfo[(*sCellIdx)]->sCellState == RG_SCH_SCELL_READY))
      {
         return ROK;
      }
   }
   return RFAILED;
}

/**
 * @brief SCell Activation of selected cell
 *
 * @details
 *
 *     Function: rgSCHSCellSelectAndActDeAct
 *     Purpose :  Perform Selection and Activation/Deactivation of secondary cell
 *
 *     Invoked by:Cfg/Commn Scheduler 
 *
 *  @param[in]  RgSchCellCb *cellCb
 *  @param[in]  RgSchUeCb   *ueCb
 *  @param[in]  U8          action
 *
 *  @return  Void
 *
 **/
#ifdef ANSI
Void rgSCHSCellSelectAndActDeAct
(
RgSchCellCb  *pCell,
RgSchUeCb    *ueCb,
U8           action
)
#else
Void rgSCHSCellSelectAndActDeAct(pCell, ueCb, action)
RgSchCellCb  *pCell;
RgSchUeCb    *ueCb;
U8           action;
#endif
{
   U8  sCellIdx = 0;
   S16 ret = ROK;

   switch (action)
   {
      case RGR_SCELL_ACT:
         {

            if(((ret = rgSCHSCellSelectForAct(pCell, ueCb, &sCellIdx)) == ROK)
                  && (sCellIdx == 0))
               RETVOID;
            break;
         }
         default:
         RETVOID;
   }
   if ((ret != ROK) || 
         (ROK != (rgSCHSCellTrigActDeact(pCell, ueCb, sCellIdx, action))))
   {
      RGSCHDBGERR(pCell->instIdx,(rgSchPBuf(pCell->instIdx), "SCell Actication failed"
               "for UE [%d] with SCellIdx [%d]\n", ueCb->ueId, sCellIdx));
   }
   RETVOID;
}

 
/**
 * @brief Handling of Scell Deletion 
 *
 * @details
 *
 *     Function: rgSCHSCellDelUeSCell
 *     Purpose : Perform Scell Deletion for an UE
 *             : flush harqEnttiy of the given scell associated
 *               with this UE
 *              
 *
 *     Invoked by:Cfg module 
 *
 *  @param[in]  RgSchCellCb  *cellCb
 *  @param[in]  RgSchUeCb    *ueCb
 *  @param[in]  U8            idx
 *  @return  ROK/RFAILED
 *
 **/
#ifdef ANSI
Void rgSCHSCellDelUeSCell
(
RgSchCellCb  *cellCb,
RgSchUeCb    *ueCb,
U8            sCellIdx
)
#else
Void rgSCHSCellDelUeSCell(cellCb,ueCb,sCellIdx)
RgSchCellCb  *cellCb;
RgSchUeCb    *ueCb;
U8            sCellIdx;
#endif
{
   RgUeUlHqCb       *ulHqEnt;
   Inst inst = cellCb->instIdx;
   RgSchUeCellInfo *sCellInfo;
   RgSchCmnUlUe *ueUl;

   TRC3(rgSCHSCellDelUeSCell);
   sCellInfo = ueCb->cellInfo[sCellIdx];


   if(sCellInfo == NULLP)
   {
      RGSCHDBGERRNEW(inst, (rgSchPBuf(inst),"Serv Cell not added to this Ue Scell Idx %d\
               ueId %d\n",
               sCellIdx,ueCb->ueId));
      RETVOID;
   }

   rgSCHDbmDelUeCb(sCellInfo->cell, ueCb);
   ueUl = RG_SCH_CMN_GET_UL_UE(ueCb, sCellInfo->cell);

   if (NULLP != sCellInfo->sCellLnk.node)
   {
      cmLListDelFrm(&sCellInfo->cell->sCellUeLst, &sCellInfo->sCellLnk);
   }

   /* Clear Scheduler specific list for this UE from the 
    * corresponding CELL */

   /*Updating 1BCS Value*/
   ueCb->f1bCsAVal = (ueCb->f1bCsAVal - 
         rgSCHUtlGetMaxTbSupp(sCellInfo->txMode.txModeEnum));

#ifdef LTE_TDD
   rgSCHUtlDelUeANFdbkInfo(ueCb,sCellIdx);
#endif

   rgSCHSCellDeActivation(sCellInfo);
   /* Release hqEnt mem */
   rgSCHDhmDelHqEnt(cellCb, &sCellInfo->hqEnt);

   ulHqEnt = &(ueUl->hqEnt);

   cellCb->sc.apis->rgSCHRgrSCellUeDel(sCellInfo, sCellInfo->ue);

   rgSCHUhmFreeUe(sCellInfo->cell, ulHqEnt);

   rgSCHUtlFreeSBuf(cellCb->instIdx,
         (Data**)(&(sCellInfo)), (sizeof(RgSchUeCellInfo)));

   ueCb->cellInfo[sCellIdx] = NULLP;
   
   RETVOID;
}

/**
 * @brief Handling of UE Deletion
 *
 * @details
 *
 *     Function: rgSCHSCellDelUe
 *     Purpose : Perform UE Deletion
 *             : Delete all the SCells added for this UE
 *             : flush harqEnttiy of all scells associated
 *               with this UE
 *              
 *
 *     Invoked by:Cfg module 
 *
 *  @param[in]  RgSchCellCb  *cellCb
 *  @param[in]  RgSchUeCb    *ueCb
 *  @return  ROK/RFAILED
 *
 **/
#ifdef ANSI
S16 rgSCHSCellDelUe
(
RgSchCellCb  *cellCb,
RgSchUeCb    *ueCb
)
#else
S16 rgSCHSCellDelUe(cellCb,ueCb)
RgSchCellCb  *cellCb;
RgSchUeCb    *ueCb;
#endif
{

   TRC3(rgSCHSCellDelUe);

   for(U8 idx = 1; idx <= RG_SCH_MAX_SCELL ; idx++)
   {
      rgSCHSCellDelUeSCell(cellCb,ueCb,idx);
   }

   return ROK;
}

#ifdef TFU_UPGRADE

/**
 * @brief Handling of PCqi cfg fro a scell
 *
 * @details
 *
 *     Function: rgSCHSCellPCqiCfg
 *     Purpose : 
 *             : Delete all the SCells added for this UE
 *             : flush harqEnttiy of all scells associated
 *               with this UE
 *    Processing Steps:
 *             - For SCell-specific Periodic CQI  related configuration, 
 *             - If Periodic CQI/PMI is configured,
 *                   - Update SCell with the configured values.
 *                   - Update the CQI offset and CQI perodicity information
 *        
 *
 *     - For SCell-specific Periodic RI  related configuration, 
 *      - If Periodic RI is configured,
 *        - Update SCell with the configured values.
 *         - Update the  RI offset and RI perodicity information
 *      
 *
 *     Invoked by:Cfg module 
 *
 *  @param[in]  RgSchCellCb  *cellCb
 *  @param[in]  RgSchUeCb    *ueCb
 *  @return  ROK/RFAILED
 *
 **/
#ifdef ANSI
S16 rgSCHSCellPCqiCfg
(
RgSchCellCb  *priCellCb,
RgSchCellCb  *secCellCb,
RgSchUeCb    *ueCb,
RgrUePrdDlCqiCfg  *cqiCfg,
CmLteUeCategory   ueCat,
U8            sCellIdx
)
#else
S16 rgSCHSCellPCqiCfg(priCellCb,secCellCb,ueCb,cqiCfg,ueCat,sCellIdx)
RgSchCellCb       *priCellCb;
RgSchCellCb       *secCellCb;
RgSchUeCb         *ueCb;
RgrUePrdDlCqiCfg  *cqiCfg;
CmLteUeCategory    ueCat;
U8                sCellIdx;
#endif
{
   U8     j;  /*Bandwidth Parts*/
   U8     temp; 
#ifdef DEBUGP
   Inst   inst = priCellCb->instIdx;
#endif
   RgSchUeCellInfo *sCellInfo;
   RgSchUePCqiCb *cqiCb = NULLP;

   TRC3(rgSCHSCellPCqiCfg);

   RGSCHDBGINFO(priCellCb->instIdx, (rgSchPBuf(priCellCb->instIdx), 
            "rgSCHSCellPCqiCfg cellId =%d, ueId = %d, CfgType =%d\n",
            secCellCb->cellId,  ueCb->ueId, cqiCfg->type));
   
   if((sCellIdx < 1) ||
      (sCellIdx > RGR_MAX_SCELL_PER_UE))
   {
      RGSCHDBGERRNEW(inst, (rgSchPBuf(inst),"Invalid Serv Cell Idx %d\n",
               sCellIdx));
      return RFAILED;
   }

   sCellInfo = ueCb->cellInfo[sCellIdx];

   cqiCb = &ueCb->cellInfo[sCellIdx]->cqiCb;
   cqiCb->servCellInfo = sCellInfo;

   /* Periodic CQI is setup  */
   if (cqiCfg->type == RGR_SCH_PCQI_SETUP)
   {   
      /*  1. Copy the Received CQI Cfg parameters to ueCb  */
      cmMemcpy((U8 *)&cqiCb->cqiCfg, (U8 *)cqiCfg, 
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


      RGSCHDBGINFO(priCellCb->instIdx,(rgSchPBuf(priCellCb->instIdx), 
               "\n rgSCHSCellPCqiCfg(): CQI Peri=%d, CQI Offset=%d", 
               cqiCb->cqiPeri,cqiCb->cqiOffset));

      if(RGR_UE_PCQI_SB_REP == cqiCfg->cqiSetup.cqiRepType)
      {
         U8     k;  /*SubBand Size (RB) */
         RG_SCH_GET_CQI_J_VAL(secCellCb->bwCfg.dlTotalBw, j);
         RG_SCH_GET_CQI_K_VAL(secCellCb->bwCfg.dlTotalBw, k);
         cqiCb->J = j; /*Number of Bandwidth Parts*/
         /*h: reporting instances required for a complete CQI/PMI report */
         /*j:Number of bandwidth parts; k: Subband Size*/
         cqiCb->h = (cqiCb->cqiCfg.cqiSetup.k *j )+1; 
         /* ccpu00140905- L-size is coming as 3 for 100Rbs where it should be 2*/
         temp = RGSCH_CEIL(secCellCb->bwCfg.dlTotalBw, (j*k));
         cqiCb->label = (temp & (temp-1)) ?
            (1+ rgSCHUtlLog32bitNbase2(temp)) : rgSCHUtlLog32bitNbase2(temp);
      }
      else
      {
         /* Wideband Cqi Rep Type */
         cqiCb->prioLvl = RG_SCH_CQI_PRIO_LVL_1;
      }
      cqiCb->cqiLstEnt.node=(PTR)cqiCb;
      cqiCb->isCqiIgnoByCollsn = FALSE;


      /* 4. Rank Indicator Cfg handler */
      /* 1. Rank Indicator is enabled  */
      if(cqiCfg->cqiSetup.riEna)
      {
         rgSCHUtlGetCfgPerOff(RG_SCH_RI_TBL, 
               cqiCfg->cqiSetup.riCfgIdx,
               &cqiCb->riPeri, &cqiCb->riOffset);

         RGSCHDBGINFO(priCellCb->instIdx,(rgSchPBuf(priCellCb->instIdx),
                  "\n rgSCHSCellPCqiCfg(): RI Peri=%d, RI Offset=%d", 
                  cqiCb->riPeri,cqiCb->riOffset));

         if(ueCb->cellInfo[sCellIdx]->txMode.txModeEnum == RGR_UE_TM_3 
               || ueCb->cellInfo[sCellIdx]->txMode.txModeEnum == RGR_UE_TM_4)
         {
            if (secCellCb->numTxAntPorts ==2)
            {
               cqiCb->riNumBits = 1;
            }
            else if(secCellCb->numTxAntPorts ==4)
            {
               if(ueCat == CM_LTE_UE_CAT_8)
               {
                  cqiCb->riNumBits = 3;
               }
               else if((ueCat == CM_LTE_UE_CAT_5) || 
                     (ueCat == CM_LTE_UE_CAT_6) || CM_LTE_UE_CAT_7)
               {
                  cqiCb->riNumBits = 2;
               }
               else
               {
                  cqiCb->riNumBits = 1;
               }
            }
         }
         cqiCb->riLstEnt.node=(PTR) cqiCb;
         cqiCb->isRiIgnoByCollsn = FALSE;

      }
   }
   else
   {
      sCellInfo->cqiCb.cqiCfg.type =  RGR_SCH_PCQI_REL;
   }
   /* Setting the indices to invalid during
      scell addition. These indices will be set during 
      activation */
   cqiCb->nRiTrIdx  = RG_SCH_INVALID_IDX;
   cqiCb->riDist    = RG_SCH_INVALID_IDX; 
   cqiCb->nCqiTrIdx = RG_SCH_INVALID_IDX;
 
   return ROK;
}
#endif

/**
 * @brief Handling of Ue Reset from common scheduler
 *
 * @details
 *
 *     Function: rgSCHSCellDlUeReset
 *     Purpose:  Call scheudler type spcefic UE RESET
 *               for all the secondary cells
 *
 *     Invoked by: CommonScheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSchUeCb*       ue
 *  @return  Void
 *
 **/
#ifdef ANSI
Void rgSCHSCellDlUeReset
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue
)
#else
Void rgSCHSCellDlUeReset(cell, ue)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
#endif
{
   RgSchCmnCell *cellSch;
   TRC3(rgSCHSCellDlUeReset);

   for(U8 idx = 1; idx <= RG_SCH_MAX_SCELL ; idx++)
   {
      if(ue->cellInfo[idx] != NULLP) 
      {
         cellSch = RG_SCH_CMN_GET_CELL(ue->cellInfo[idx]->cell);
         cellSch->apisDl->rgSCHDlUeReset(ue->cellInfo[idx]->cell, ue);
         rgSCHSCellDeActivation(ue->cellInfo[idx]);
         ue->cellInfo[idx]->sCellState = RG_SCH_SCELL_INACTIVE;
      }
   }
   RETVOID;
}


/**
 * @brief Handling of LC Cfg from common scheduler
 *
 * @details
 *
 *     Function: rgSCHSCellDlLcCfg
 *     Purpose:  Call scheudler type spcefic LC config
 *               for all the secondary cells
 *
 *     Invoked by: CommonScheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSchUeCb*       ue
 *  @return  Void
 *
 **/
#ifdef ANSI
Void rgSCHSCellDlLcCfg
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
)
#else
Void rgSCHSCellDlLcCfg(cell, ue, svc)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlLcCb                *svc;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   TRC3(rgSCHSCellDlLcCfg);
   for(U8 idx = 1; idx <= RG_SCH_MAX_SCELL ; idx++)
   {
      if(ue->cellInfo[idx] != NULLP) 
      {
         cellSch->apisDl->rgSCHRgrDlLcCfg(ue->cellInfo[idx]->cell, ue, svc,NULLP,NULLP);
      }
   }
   RETVOID;
}

/**
 * @brief Handling of LC Delete from common scheduler
 *
 * @details
 *
 *     Function: rgSCHSCellDlLcDel
 *     Purpose:  Call scheudler type spcefic bo update handler
 *               for all the secondary cells
 *
 *     Invoked by: CommonScheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSchUeCb*       ue
 *  @return  Void
 *
 **/
#ifdef ANSI
Void rgSCHSCellDlLcDel
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
)
#else
Void rgSCHSCellDlLcDel(cell, ue, svc)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlLcCb                *svc;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   TRC3(rgSCHSCellDlLcDel);
   for(U8 idx = 1; idx <= RG_SCH_MAX_SCELL ; idx++)
   {
      if(ue->cellInfo[idx] != NULLP) 
      {
         cellSch->apisDl->rgSCHFreeDlLc(ue->cellInfo[idx]->cell, ue, svc);
      }
   }
   RETVOID;
}

/**
 * @brief Handling of Bo update from common scheduler
 *
 * @details
 *
 *     Function: rgSCHSCellDlDedBoUpd
 *     Purpose:  Call scheudler type spcefic bo update handler
 *               for all the secondary cells
 *
 *     Invoked by: CommonScheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSchUeCb*       ue
 *  @return  Void
 *
 **/
#ifdef ANSI
Void rgSCHSCellDlDedBoUpd
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
)
#else
Void rgSCHSCellDlDedBoUpd(cell, ue, svc)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlLcCb                *svc;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   TRC3(rgSCHSCellDlDedBoUpd);

   /* If this is not invoked by PCell, then
      invoke the call to PCell handler 
      This happens during finalization if LC Bo becomes zero*/
   if (ue->cell != cell)
   {
      cellSch->apisDl->rgSCHDlDedBoUpd(ue->cell, ue, svc);
   }
   for(U8 idx = 1; idx <= RG_SCH_MAX_SCELL ; idx++)
   {
      if((ue->cellInfo[idx] != NULLP) &&
            (ue->cellInfo[idx]->sCellState == RG_SCH_SCELL_ACTIVE) &&
           (ue->cellInfo[idx]->cell != cell))
      {
         cellSch->apisDl->rgSCHDlDedBoUpd(ue->cellInfo[idx]->cell, ue, svc);
      }
   }
   RETVOID;
}
#ifdef TFU_UPGRADE
/**
 * @brief Compare two CQI CB configs are return the result
 *
 * @details
 *
 *     Function: rgSCHUtlSCellCmpCqiCfg
 *     Purpose : Compare priority levels of cqiCb1 and cqiCb2
 *               and set the isCqiIgnoByCollsn to TRUE for the 
 *               cqiCb which has lower priority 
 *     Invoked by:scell module 
 *
 *  @param[in] RgSchUePCqiCb *cqiCb1
 *  @param[in] RgSchUePCqiCb *cqiCb2
 *  @return U8 cqiCb cell idx which has the higher priority
 *
 **/
#ifdef ANSI
PRIVATE U8  rgSCHUtlSCellCmpCqiCfg
(
RgSchUePCqiCb *cqiCb1,
RgSchUePCqiCb *cqiCb2
)
#else
PRIVATE U8  rgSCHUtlSCellCmpCqiCfg(cqiCb1, cqiCb2)
RgSchUePCqiCb     *cqiCb1;
RgSchUePCqiCb     *cqiCb2;
#endif
{
   RgSchUePCqiCb     *retCqiCb;
   TRC3(rgSCHUtlSCellCmpCqiCfg);
   /* Collision rules are defined in TS 36.213,7.2.2 */
   /* RI, WB first PMI > WB CQI >  SB CQI */
   /* As of now only taking care of RI > WB CQI > SB CQI */

   if (cqiCb1->prioLvl > cqiCb2->prioLvl)
   {
      cqiCb2->isCqiIgnoByCollsn = TRUE;
      cqiCb1->isCqiIgnoByCollsn = FALSE;
      retCqiCb = cqiCb1;
   }
   else if (cqiCb2->prioLvl > cqiCb1->prioLvl)
   {
      cqiCb1->isCqiIgnoByCollsn = TRUE;
      cqiCb2->isCqiIgnoByCollsn = FALSE;
      retCqiCb = cqiCb2;
   }
   else
   {
      if (cqiCb1->servCellInfo->sCellIdx > cqiCb2->servCellInfo->sCellIdx)
      {
         cqiCb1->isCqiIgnoByCollsn = TRUE;
         cqiCb2->isCqiIgnoByCollsn = FALSE;
         retCqiCb = cqiCb2;
      }
      else
      {
         cqiCb2->isCqiIgnoByCollsn = TRUE;
         cqiCb1->isCqiIgnoByCollsn = FALSE;
         retCqiCb = cqiCb1;
      }
   }

   return (retCqiCb->servCellInfo->sCellIdx);
}

/**
 * @brief Handling of collision of CQI types between serving cells
 *
 * @details
 *
 *     Function: rgSCHUtlSCellHndlCqiCollsn
 *     Purpose : Takes care of collision clauses specified in 36.213 7.2.2 Rel 10 
 *               and selects next nearest cqiCb 
 *     Invoked by:Cfg module 
 *
 *  @param[in]  RgSchCellCb  *cellCb
 *  @param[in]  RgSchUeCb    *ueCb
 *  @return  ROK/RFAILED
 *
 **/
#ifdef ANSI
S16 rgSCHUtlSCellHndlCqiCollsn
(
RgSchUePCqiCb *cqiCb
)
#else
S16 rgSCHUtlSCellHndlCqiCollsn(cqiCb)
RgSchUePCqiCb     *cqiCb;
#endif
{
   U32 nPCqiServCellIdx;
   U32 minPCqiTrIdx;
   U32 scellPCqiTrIdx;
   U32 pCqiTrIdx;
   RgSchCellCb       *priCellCb = cqiCb->servCellInfo->ue->cell;
   RgSchUeCb         *ueCb = cqiCb->servCellInfo->ue;
   U16 crntSfIdx;
   U32 cellIdx;
   U32 sCellCnt = 0;
   CmLteTimingInfo timingInfo;
   U8 idx = 0;
   TRC3(rgSCHUtlSCellHndlCqiCollsn);

#ifdef xLTE_TDD
   RG_SCH_ADD_TO_CRNT_TIME(priCellCb->crntTime, timingInfo, TFU_DELTA);
#else
   RG_SCH_ADD_TO_CRNT_TIME(priCellCb->crntTime, timingInfo,
         TFU_RECPREQ_DLDELTA);
#endif

   RG_SCH_GET_IDX_PCQISRSSR(timingInfo, crntSfIdx);

   cqiCb->isCqiIgnoByCollsn = FALSE;

   pCqiTrIdx = cqiCb->nCqiTrIdx;
   nPCqiServCellIdx = cqiCb->servCellInfo->sCellIdx;
   /* Handle wrap around case */
   if (pCqiTrIdx < crntSfIdx)
   {
      pCqiTrIdx += RG_SCH_PCQI_SRS_SR_TRINS_SIZE;
   }
   minPCqiTrIdx = pCqiTrIdx;

   for (cellIdx =0; cellIdx <= RG_SCH_MAX_SCELL; cellIdx++)
   {
      /* If a serving cell is configured */
      if(ueCb->cellInfo[cellIdx] != NULLP)
      {
         /* If the serving cell is in ACTIVE state and 
            If it is not the same serving cell as cqiCb for which 
            collision is being checked */
         if ((ueCb->cellInfo[cellIdx]->sCellState == RG_SCH_SCELL_ACTIVE)&&
               (cellIdx != cqiCb->servCellInfo->sCellIdx))
         {
            scellPCqiTrIdx = ueCb->cellInfo[cellIdx]->cqiCb.nCqiTrIdx;

            /* Handle wrap around case */
            if (scellPCqiTrIdx < crntSfIdx)
            {
               scellPCqiTrIdx += RG_SCH_PCQI_SRS_SR_TRINS_SIZE;
            }
            
            /* If cqiCb->isCqiIgnoByCollsn is TRUE then a higher prio cqiCb
               is already found so need to compare */
            if ((FALSE == ueCb->cellInfo[cellIdx]->cqiCb.isCqiIgnoByCollsn) &&
                  (FALSE == cqiCb->isCqiIgnoByCollsn) && 
                  (scellPCqiTrIdx == pCqiTrIdx))
            {
               /* Handle Collision */
               /* set isCqiIgnoByCollsn to TRUE for low prio CQI Rep type */
               nPCqiServCellIdx = rgSCHUtlSCellCmpCqiCfg(&ueCb->cellInfo[cellIdx]->cqiCb,cqiCb);
            }
            else if (scellPCqiTrIdx < minPCqiTrIdx)
            {
               minPCqiTrIdx = scellPCqiTrIdx;
               nPCqiServCellIdx = cellIdx;
            }
         }

         /* If all of the num of configured scells are checked then break */
         if (sCellCnt == ueCb->numSCells)
         {
            break;
         }   
         sCellCnt++;
      }
   }

   /* Set the next expected Cqi into nPCqiCb */
   idx = ((nPCqiServCellIdx)& (CM_LTE_MAX_CELLS -1));
   ueCb->nPCqiCb = &ueCb->cellInfo[idx]->cqiCb;

   return ROK;
}


/**
 * @brief Handling of collision of RI types between serving cells
 *
 * @details
 *
 *     Function: rgSCHUtlSCellHndlRiCollsn
 *     Purpose : Takes care of collision clauses specified in 36.213 7.2.2 Rel 10 
 *               and selects next nearest cqiCb 
 *     Invoked by:Cfg module 
 *
 *  @param[in]  RgSchCellCb  *cellCb
 *  @param[in]  RgSchUeCb    *ueCb
 *  @return  ROK/RFAILED
 *
 **/
#ifdef ANSI
S16 rgSCHUtlSCellHndlRiCollsn
(
RgSchUePCqiCb *cqiCb
)
#else
S16 rgSCHUtlSCellHndlRiCollsn(cqiCb)
RgSchUePCqiCb     *cqiCb;
#endif
{
   U32 nPRiServCellIdx;
   U32 minPRiTrIdx;
   U32 scellPRiTrIdx;
   U32 pRiTrIdx;
   RgSchCellCb       *priCellCb = cqiCb->servCellInfo->ue->cell;
   RgSchUeCb         *ueCb = cqiCb->servCellInfo->ue;
   U16 crntSfIdx;
   U32 cellIdx;
   U32 sCellCnt = 0;
   CmLteTimingInfo timingInfo;
   TRC3(rgSCHUtlSCellHndlRiCollsn);

#ifdef xLTE_TDD
   RG_SCH_ADD_TO_CRNT_TIME(priCellCb->crntTime, timingInfo, TFU_DELTA);
#else
   RG_SCH_ADD_TO_CRNT_TIME(priCellCb->crntTime, timingInfo,
         TFU_RECPREQ_DLDELTA);
#endif

   RG_SCH_GET_IDX_PCQISRSSR(timingInfo, crntSfIdx);

   pRiTrIdx = cqiCb->nRiTrIdx + cqiCb->riDist * RG_SCH_PCQI_SRS_SR_TRINS_SIZE;

   /* Handle wrap around case */
   if (pRiTrIdx < crntSfIdx)
   {
      pRiTrIdx += RG_SCH_PCQI_SRS_SR_TRINS_SIZE;
   }

   cqiCb->isRiIgnoByCollsn = FALSE;
   nPRiServCellIdx = cqiCb->servCellInfo->sCellIdx;
   minPRiTrIdx = pRiTrIdx;

   for (cellIdx =0; cellIdx <= RG_SCH_MAX_SCELL; cellIdx++)
   {
      /* If a serving cell is configured */
      if(ueCb->cellInfo[cellIdx] != NULLP)
      {
         /* If the serving cell is in ACTIVE state and 
            If it is not the same serving cell as cqiCb for which 
            collision is being checked */
         if ((ueCb->cellInfo[cellIdx]->sCellState == RG_SCH_SCELL_ACTIVE)&&
               (cellIdx != cqiCb->servCellInfo->sCellIdx))
         {
            scellPRiTrIdx = ueCb->cellInfo[cellIdx]->cqiCb.nRiTrIdx + 
               ueCb->cellInfo[cellIdx]->cqiCb.riDist * RG_SCH_PCQI_SRS_SR_TRINS_SIZE;

            /* Handle wrap around case */
            if (scellPRiTrIdx < crntSfIdx)
            {
               scellPRiTrIdx += RG_SCH_PCQI_SRS_SR_TRINS_SIZE;
            }
            
            /* If cqiCb->isRiIgnoByCollsn is TRUE then a higher prio cqiCb
               is already found so need to compare */
            if ((FALSE == ueCb->cellInfo[cellIdx]->cqiCb.isRiIgnoByCollsn) &&
                  (FALSE == cqiCb->isRiIgnoByCollsn) && 
                  (scellPRiTrIdx == pRiTrIdx))
            {
               /* Handle Collision */
               /* set isRiIgnoByCollsn to TRUE for low prio CQI Rep type */
               if (cqiCb->servCellInfo->sCellIdx < (ueCb->cellInfo[cellIdx]->sCellIdx))
               {
                  ueCb->cellInfo[cellIdx]->cqiCb.isRiIgnoByCollsn = TRUE;
               }
               else
               {
                  cqiCb->isRiIgnoByCollsn = TRUE;
               }
            }
            else if (scellPRiTrIdx < minPRiTrIdx)
            {
               minPRiTrIdx = scellPRiTrIdx;
               nPRiServCellIdx = cellIdx;
            }
         }

         /* If all of the num of configured scells are checked then break */
         if (sCellCnt == ueCb->numSCells)
         {
            break;
         }   
         sCellCnt++;
      }
   }

   /* Set the next expected Cqi into nPCqiCb */
   ueCb->nPRiCb = &ueCb->cellInfo[nPRiServCellIdx]->cqiCb;

   return ROK;
}
#endif/*TFU_UPGRADE*/

/**
 * @brief Checking whethter the scell is active or not
 *
 * @details
 *
 *     Function: rgSCHSCellIsActive
 *     Purpose: Check the Scell is in active state or not 
 *              
 *
 *     Invoked by: SpecificScheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSchUeCb*       ue
 *  @return  Void
 *
 **/
#ifdef ANSI
S16 rgSCHSCellIsActive
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue
)
#else
S16 rgSCHSCellIsActive(cell, ue)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
#endif
{
   S16 retVal = RFAILED;
   TRC3(rgSCHSCellIsActive);

   for(U8 idx = 1; idx <= RG_SCH_MAX_SCELL ; idx++)
   {
      if((ue->cellInfo[idx] != NULLP) &&
         (ue->cellInfo[idx]->cell->cellId == cell->cellId)&&
         (ue->cellInfo[idx]->sCellState == RG_SCH_SCELL_ACTIVE)) 
      {
         retVal = ROK;
         break;
      }
   }
   return (retVal);	
}

/**
 * @brief Function to check for Acell Activation Trigered.
 *
 * @details
 *
 *     Function : rgSCHIsActvReqd
 *       This function will check for Secondary cell activation criteria
 *       If met this will return TRUE else FALSE
 *     
 *  @param[in]     RgSchCellCb    *cell
 *  @param[in]     RgSchUeCb      *ue
 *  @return  BOOL
 *      -#  TRUE
 **/
#ifdef ANSI
Bool rgSCHIsActvReqd 
(
RgSchCellCb    *cell,
RgSchUeCb      *ue
)
#else
Bool rgSCHIsActvReqd(cell, ue)
RgSchCellCb    *cell;
RgSchUeCb      *ue
#endif
{
   TRC2(rgSCHIsActvReqd)
   /* Check if remBoCnt in this UE is greater than ZERO for sufficient number of
    * Scheduling TTIs. If yes then We should activate a secondary cell to handle
    * outstanding BO */
   if(ue->remBoCnt == RG_SCH_ACTIVATION_COUNT)
   {
      return (TRUE);
   }
   return (FALSE);
}
#endif/*LTE_ADV*/



/**********************************************************************

         End of file
**********************************************************************/
