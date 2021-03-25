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
 
     Desc:     C source code for Scheduler common functions
 
     File:     rg_sch_mga.c
 
**********************************************************************/

/** @file rg_sch_mga.c
@brief This module handles schedulers' measurement gap and ack-nack repetiton functionality */


/* header include files -- defines (.h) */
#include "common_def.h"
#include "lrg.h"
#include "rgr.h"
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
#include "rg_sch_inf.x"    /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */

/* local defines */
static S16 rgSCHMeasGapANRepUtlAddUe ARGS((
RgSchCellCb       *cell,
RgSchUeCb          *ue,
RgrUeMeasGapCfg    *cfg));

static S16 rgSCHMeasGapANRepUtlRmvUe ARGS((
RgSchCellCb       *cell,
RgSchUeCb          *ue));

static S16 rgSchAckNackRepUtlRmvUe ARGS((
 RgSchCellCb      *cell,
 RgSchUeCb        *ue));

static Void rgSchAckNackRepUtlHdlTti ARGS((
RgSchCellCb *cell,
CmLListCp   *ackNackRepQ));

static Void rgSCHMeasGapANRepUtlHdlTti ARGS((
RgSchCellCb *cell,
CmLListCp   *measGapQ));
#ifdef LTE_TDD
static uint8_t rgSCHAckNakRepFindUlDuration ARGS((
RgSchCellCb       *cell,
RgSchDlSf         *dlSf,
CmLteTimingInfo   repTime,
uint8_t                repCnt));
static Void rgSCHAckNakRepGetUlOffsetFrmDl ARGS((
RgSchDlSf         *dlSf,
CmLteTimingInfo   crntDlTime,
uint8_t                *noSubfrms));
#endif

/**
 * @brief Handles Measurement gap and ack-nack repetition related 
 * configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHMeasGapANRepUeCfg
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE configuration. It shall do the
 *      validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific measurement gap related configuration, 
 *      - If measurementGap is configured,
 *        - Update UE with the configured values.
 *        - Add Ue to cell->measGapCb->gapPrdQ depending on the gap period
 *          configuration at index = the configured gap offset.
 *        - measGapOffst = the configured gap offset
 *        - Initialize timers.
 *      - else
 *        - measGapOffst = RG_INVALID_MEASGAPQ_ID
 *     - For UE-specific ACK-NACK repetition related configuration, 
 *      - Update the configured value. Set 'cfgRepCnt' variable value.
 *      - repCntr = cfgRepCnt.
 *      - qOffst = RG_INVALID_ACKNACKREPQ_ID
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeCfg     *ueCfg
 *
 *  @return  S16
 *      -# ROK
 **/

S16 rgSCHMeasGapANRepUeCfg(RgSchCellCb *cell,RgSchUeCb *ue,RgrUeCfg *ueCfg)
{

   ue->measGapCb.isMesGapEnabled = ueCfg->ueMesGapCfg.isMesGapEnabled;

   if (ueCfg->ueMesGapCfg.isMesGapEnabled)
   {
      ue->measGapCb.gapPrd          = ueCfg->ueMesGapCfg.gapPrd;
      ue->measGapCb.gapOffst        = ueCfg->ueMesGapCfg.gapOffst;
      rgSCHMeasGapANRepUtlAddUe (cell, ue, &(ueCfg->ueMesGapCfg));
      cmInitTimers (&ue->measGapCb.measGapTmr, 1);
      cmInitTimers (&ue->measGapCb.measGapUlInactvTmr, 1);
      cmInitTimers (&ue->measGapCb.measGapDlInactvTmr, 1);
   }

   /* ACK NACK repetition part */
   if (ueCfg->ueAckNackCfg.isAckNackEnabled)
   {
      ue->ackNakRepCb.cfgRepCnt = ueCfg->ueAckNackCfg.ackNackRepFactor;
      ue->ackNakRepCb.repCntr = ue->ackNakRepCb.cfgRepCnt;
      ue->ackNakRepCb.isAckNackEnabled = TRUE;
      ue->ackNakRepCb.pucchRes = ueCfg->ueAckNackCfg.pucchAckNackRep;
      cmInitTimers (&ue->ackNakRepCb.ackNakRepUlInactvTmr, 1);
      cmInitTimers (&ue->ackNakRepCb.ackNakRepDlInactvTmr, 1);
      cmInitTimers (&ue->ackNakRepCb.ackNakRepTmr, 1);
   }
   return ROK;
}

/**
 * @brief Handles Measurement gap and ack-nack repetition related 
 * re-configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHMeasGapANRepUeRecfg
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE re-configuration. It shall do the
 *        validations for the spec-defined values.
 *        
 *     Processing Steps:
 *      - For measurement gap,
 *       - If measurement gap period or offset is re-configured, remove UE from
 *         the previous list, if any and add it to the new list.
 *       - Update configured values appropriately.
 *      - For ACK-NACK repetition,
 *       - Update the configured value. Set 'cfgRepCnt' variable value.
 *       - If (repCntr == 0)
 *         - repCntr = cfgRepCnt.
 *         - qOffst = RG_INVALID_ACKNACKREPQ_ID
 *       
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeRecfg   *ueRecfg
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/

S16 rgSCHMeasGapANRepUeRecfg(RgSchCellCb  *cell,RgSchUeCb *ue,RgrUeRecfg *ueRecfg)
{
   RgrUeMeasGapCfg         *reCfg;
   RgSchUeMeasGapCb        *ueMeasCb;
   RgrUeAckNackRepCfg      *ackNackReCfg = &(ueRecfg->ueAckNackRecfg);
   RgSchUeAckNakRepCb      *ackNakRepCb = &(ue->ackNakRepCb);

   reCfg    = &(ueRecfg->ueMeasGapRecfg);
   ueMeasCb = &(ue->measGapCb);

   /* Removed extra comments
    * Check this once again Check to see if anything changed or not */
   if ((reCfg->isMesGapEnabled == TRUE) &&
         (ueMeasCb->isMesGapEnabled == TRUE) &&
         (reCfg->gapPrd == ueMeasCb->gapPrd) &&
         (reCfg->gapOffst == ueMeasCb->gapOffst))
   {
      /* Nothing changed hence nothing to do */
   }
   else
   {
      if (reCfg->isMesGapEnabled)
      {
         if (ueMeasCb->isMesGapEnabled)
         {
            rgSCHMeasGapANRepUtlRmvUe (cell, ue);
         } 
         else
         {
            cmInitTimers (&ueMeasCb->measGapTmr, 1);
            cmInitTimers (&ueMeasCb->measGapUlInactvTmr, 1);
            cmInitTimers (&ueMeasCb->measGapDlInactvTmr, 1);
         } 

         /* Add to the correct Measurement gap queue */
         rgSCHMeasGapANRepUtlAddUe (cell, ue, reCfg);

         ueMeasCb->gapPrd = reCfg->gapPrd;
         ueMeasCb->gapOffst = reCfg->gapOffst;
         ueMeasCb->isMesGapEnabled = TRUE;
      } /* if new config has Measurement gap enabled */
      else
      {
         if (ueMeasCb->isMesGapEnabled)
         {
            /* check if return value needed or not */
            rgSCHMeasGapANRepUtlRmvUe (cell, ue);
            ueMeasCb->isMesGapEnabled = FALSE;
         } 
      } /* if new config has Measurement gap disabled */
   } /* For MeasGap configuration */

   if (ackNackReCfg->isAckNackEnabled)
   {
      if (!ackNakRepCb->isAckNackEnabled)
      {
         ackNakRepCb->isAckNackEnabled = TRUE;
         /* Timers need to be init immediately after config*/
         cmInitTimers (&ue->ackNakRepCb.ackNakRepUlInactvTmr, 1);
         cmInitTimers (&ue->ackNakRepCb.ackNakRepDlInactvTmr, 1);
         cmInitTimers (&ue->ackNakRepCb.ackNakRepTmr, 1);
      } /* repetition was disabled */
      ackNakRepCb->pucchRes   = ackNackReCfg->pucchAckNackRep;
      ackNakRepCb->cfgRepCnt  = ackNackReCfg->ackNackRepFactor;
      if (ackNakRepCb->repCntr == 0)
      {
         ackNakRepCb->repCntr = ackNackReCfg->ackNackRepFactor;
      }
   } /* repetition enabled in re configuration */
   else
   {
      ackNakRepCb->isAckNackEnabled = FALSE;
   } /* repetition disabled in re configuration */

   return ROK;
}

 /** @brief This function is a utility to add the UE to the correct Measurement
  * queue present in the cellCb.
  *
  * @details
  *
  *     Function: 
  *
  *         Processing steps:
  *         - 
  *
  *
  * @param  
  * @param 
  * @return 
  */
static S16 rgSCHMeasGapANRepUtlAddUe(RgSchCellCb *cell,RgSchUeCb *ue,RgrUeMeasGapCfg *cfg)
{

   switch (cfg->gapPrd)
   {
      case RG_MEAS_GAPPRD_40:
         /* Insert the UE into the linked list based on the gap Offset */
         ue->measGapCb.measQLnk.node = (PTR)ue;
         cmLListAdd2Tail (&(cell->measGapCb.gapPrd40Q[cfg->gapOffst]), 
               &(ue->measGapCb.measQLnk));
         break;
      case RG_MEAS_GAPPRD_80:
         ue->measGapCb.measQLnk.node = (PTR)ue;
         cmLListAdd2Tail (&(cell->measGapCb.gapPrd80Q[cfg->gapOffst]), 
               &(ue->measGapCb.measQLnk));
         break;
      default:
         DU_LOG("\nERROR  -->  SCH : rgSCHMeasGapANRepUeRecfg() Incorrect GAP Period"
                  "CRNTI:%d",ue->ueId);
         return RFAILED;
   }
   return ROK;
} /* end of rgSCHMeasGapANRepUtlAddUe */ 


 /** @brief This function is a utility function to remove the ue from the measQ
  * preset in tthe cell Cb.
  *
  * @details
  *
  *     Function: 
  *
  *         Processing steps:
  *         - 
  *
  * @param  
  * @param 
  * @return 
  */
static S16 rgSCHMeasGapANRepUtlRmvUe(RgSchCellCb *cell,RgSchUeCb *ue)
{

   switch (ue->measGapCb.gapPrd)
   {
      case RG_MEAS_GAPPRD_40:
         /* Remove from the existing list */
         cmLListDelFrm (&(cell->measGapCb.gapPrd40Q[ue->measGapCb.gapOffst]), 
               &(ue->measGapCb.measQLnk));
         ue->measGapCb.measQLnk.node = NULLP;
         break;
      case RG_MEAS_GAPPRD_80:
         /* Remove from the existing list */
         cmLListDelFrm (&(cell->measGapCb.gapPrd80Q[ue->measGapCb.gapOffst]), 
               &(ue->measGapCb.measQLnk));
         ue->measGapCb.measQLnk.node = NULLP;
         break;
   }
   return ROK;
} /* end of rgSCHMeasGapANRepUtlRmvUe */ 

/**
 * @brief Frees Measurement gap and ack-nack repetition related data structures in UE
 *
 * @details
 *
 *     Function : rgSCHMeasGapANRepUeDel
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at Ue deletion.
 *        
 *     Processing Steps:
 *      - For measurement gap, 
 *       - if (measGapOffst != RG_INVALID_MEASGAPQ_ID)
 *        - Remove from the measurement queue depending on the measGapPrd
 *          value.
 *        - Delete all timers
 *      - For ACK-NACK repetition,
 *       - if (qOffst != RG_INVALID_ACKNACKREPQ_ID)
 *        - Remove from the ackNakRepQ queue  
 *        - Delete all timers
 *      - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *
 *  @return  S16
 *      -# ROK
 **/

Void rgSCHMeasGapANRepUeDel(RgSchCellCb *cell,RgSchUeCb *ue,Bool isUeDel)
{

   if (ue->measGapCb.isMesGapEnabled)
   {
      rgSCHMeasGapANRepUtlRmvUe (cell, ue);
      /* Must stop the timer if its running */
      if (ue->measGapCb.isMeasuring)
      {
         rgSCHTmrStopTmr (cell, RG_SCH_TMR_MEASGAP, ue);
      }

      ue->measGapCb.isMesGapEnabled = FALSE;
   }

   /* Stop timers if running */
   if (ue->dl.dlInactvMask)
   {
      if (ue->dl.dlInactvMask & RG_MEASGAP_INACTIVE)
      {
         rgSCHTmrStopTmr (cell, RG_SCH_TMR_DL_MEASGAP, ue);
      }
      if (ue->dl.dlInactvMask & RG_ACKNAKREP_INACTIVE)
      {
         rgSCHTmrStopTmr (cell, RG_SCH_TMR_DL_ACKNACK, ue);
      }
      ue->dl.dlInactvLnk.node = NULLP;
   }
   if (ue->ul.ulInactvMask)
   {
      if (ue->ul.ulInactvMask & RG_MEASGAP_INACTIVE)
      {
         rgSCHTmrStopTmr (cell, RG_SCH_TMR_UL_MEASGAP, ue);
      }
      if (ue->ul.ulInactvMask & RG_ACKNAKREP_INACTIVE)
      {
         rgSCHTmrStopTmr (cell, RG_SCH_TMR_UL_ACKNACK, ue);
      }
      ue->ul.ulInactvLnk.node = NULLP;
   }

   /* ccpu00133470- Releasing ACKNACK Rep UE Deleted */
   if (isUeDel & ue->ackNakRepCb.isAckNakRep)
   {
      rgSCHTmrStopTmr (cell, RG_SCH_TMR_ACKNACK_REP, ue);
      rgSchAckNackRepUtlRmvUe (cell, ue);
   }
   return;
}

 /** @brief This function deletes the UEs information related to ACK NACK
  * repetition.
  *
  * @details
  *
  *     Function: 
  *
  *         Processing steps:
  *         - Mainly we need to remove the UEs hqProcs from the ackNackQ(s)
  *         present in the subframes.
  *
  *
  * @param  
  * @param 
  * @return 
  */
static S16 rgSchAckNackRepUtlRmvUe(RgSchCellCb *cell,RgSchUeCb *ue)
{


   uint8_t           hqIdx;
   uint8_t           repIdx;
   RgSchDlHqProcCb   *hqP;
   uint8_t           tbCnt;

   RgSchDlHqEnt      *hqEnt = RG_SCH_CMN_GET_UE_HQE(ue, cell);

   for (hqIdx = 0; hqIdx < hqEnt->numHqPrcs; hqIdx++)
   {
      hqP = &hqEnt->procs[hqIdx];
      /* Starting from index 1 as index 0 isn't used */
      for (repIdx = 1; repIdx < ue->ackNakRepCb.cfgRepCnt; repIdx++)
      {
         for (tbCnt = 0; tbCnt < 2; tbCnt++)
         {
            if ((hqP->tbInfo[tbCnt].crntSubfrm[repIdx] != NULLP) &&
                  (hqP->tbInfo[tbCnt].anRepLnk[repIdx].node != NULLP))
            {
               cmLListDelFrm(&((hqP->tbInfo[tbCnt].crntSubfrm[repIdx])->\
               ackNakRepQ), &hqP->tbInfo[tbCnt].anRepLnk[repIdx]);
               hqP->tbInfo[tbCnt].anRepLnk[repIdx].node = NULLP;
               hqP->tbInfo[tbCnt].crntSubfrm[repIdx] = NULLP;
            }
         }
      }
   }
 
   return ROK;
} /* end of */ 


/**
 * @brief Per TTI processing for measurement gap and ack nack repetition
 * handling.
 *
 * @details
 *
 *     Function : rgSCHMeasGapANRepTtiHndl
 *
 *     Invoking Module Processing:
 *     - This shall be invoked by SCH_TOM on recieving TTI indication from PHY
 *     . SCH_TOM shall update the cell->crntTime before invoking this API.     
 *        
 *     Processing Steps:
 *     - Compute qOffset for 40ms queue as = 
 *     ((cell->crntTime->sfn * 10)+cell->crntTime->subframe)%RG_MEAS_GAPPRD_40.
 *       - Mark all the UEs at computed offset for performing measurement. Set
 *         isMeasuring = TRUE.
 *       - Start measGapTmr for each UE:
 *          - length = RG_MEAS_GAP_LEN.
 *          - event = RG_MEASGAP_ON
 *          - handler = rgSCHMeasGapANRepTmrExpry
 *       - Reinitalize the list at the offset.
 *     - Compute qOffset for 80ms queue as = 
 *     ((cell->crntTime->sfn * 10)+cell->crntTime->subframe)%RG_MEAS_GAPPRD_80.
 *       - Mark all the UEs at computed offset for performing measurement. Set
 *         isMeasuring = TRUE.
 *       - Start measGapTmr for each UE:
 *          - length = RG_MEAS_GAP_LEN.
 *          - event = RG_MEASGAP_ON
 *          - handler = rgSCHMeasGapANRepTmrExpry
 *       - Reinitalize the list at the offset.
 *     - Compute qOffset for ACK NACK repetition queue as = 
 *     ((cell->crntTime->sfn * 10)+cell->crntTime->subframe)%RG_MAX_NUM_DLSF.
 *       - Mark all the UEs at computed offset for performing ack-nack repetition. Set
 *         isAckNakRep = TRUE.
 *       - Start ackNakRepTmr for each UE: 
 *          - length = repCntr.
 *          - event = RG_ACKNAKREP_ON
 *          - handler = rgSchAckNakRepTmrExpry
 *       - Reinitalize the list at the offset.
 *     - 'isMeasuring' bool value shall be cheked
 *       - While sending dataRecpReq to PHY for non-adaptive uplink re-tx 
 *         and if 'TRUE', no dataRecpReq shall be sent.
 *       - While sending NACK as feedback to UE and if 'TRUE' no PHICH shall 
 *         be sent and shall mark the UE for adaptive re-tx.
 *       - While sending HqFbkRecpReq for a UE (applicable only if ACK NACK 
 *         repetition coincides) and if 'TRUE',
 *          - The request shall not be sent.
 *          - Decrement repCntr
 *          - if (repCntr == 0)
 *            - Delete UE from the list.
 *          - Move the Ue to next subframe's list of ACK-NACK repeating UEs.
 *     - 'isAckNakRep' bool value shall be cheked
 *       - While sending dataRecpReq to PHY for non-adaptive uplink re-tx 
 *         and if 'TRUE', no dataRecpReq shall be sent.
 *     - Check if any refresh for these cell-specific queues is needed
 *     anywhere else as well.
 *     - Check if TTI miss needs some trigger to the module.
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *
 *  @return  S16
 *      -# ROK
 **/

S16 rgSCHMeasGapANRepTtiHndl(RgSchCellCb *cell)
{
   uint8_t          offset;
   CmLListCp        *queue;
   RgSchDlSf        *dlSf;
   CmLteTimingInfo   repTime;

   /* Measurement GAP Starts at offSet - however at MAC we are concerned at
    * subframe + TFU_DELTA.
    */
   /* 40ms offset */
   /* Introduced timing delta for DL control in FDD */
#ifdef LTE_TDD
   offset = (cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G + cell->crntTime.slot + TFU_DELTA) % 
      RG_MEAS_GAPPRD_40;
#else
   offset = (cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G + cell->crntTime.slot + TFU_DLCNTRL_DLDELTA) % 
      RG_MEAS_GAPPRD_40;
#endif
   queue = &(cell->measGapCb.gapPrd40Q[offset]);
   rgSCHMeasGapANRepUtlHdlTti (cell, queue);
   /* 80ms offset */
   /* Introduced timing delta for DL control in FDD */
#ifdef LTE_TDD
   offset = (cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G + cell->crntTime.slot + TFU_DELTA) % 
      RG_MEAS_GAPPRD_80;
#else
   offset = (cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G + cell->crntTime.slot + TFU_DLCNTRL_DLDELTA) % 
      RG_MEAS_GAPPRD_80;
#endif
   queue = &(cell->measGapCb.gapPrd80Q[offset]);
   rgSCHMeasGapANRepUtlHdlTti (cell, queue);

   /* for ACK NACK repetition starts at offset - however at MAC we are 
    * concerned with subframe - TFU_DELTA */
   /* offset = ((cell->crntTime.sfn * 10) + cell->crntTime.slot) % 
    * RG_MAX_NUM_DLSF; */
   /* We wish to get the subframe whose HARQ Reception request would go out in
    * this subframe. HARQ_RTT - TFU_DELTA
    */
   /* Introduced timing delta for reception req */
#ifdef LTE_TDD
   /* ACC_TDD */
   //RGSCHSUBFRMCRNTTIME(cell->crntTime, repTime, (4 - TFU_DELTA));
   RGSCHDECRFRMCRNTTIME(cell->crntTime, repTime, (RG_SCH_CMN_HARQ_INTERVAL - TFU_RECPREQ_DLDELTA));
#else
   RGSCHDECRFRMCRNTTIME(cell->crntTime, repTime, (RG_SCH_CMN_HARQ_INTERVAL - TFU_RECPREQ_DLDELTA)); 
#endif
   dlSf = rgSCHUtlSubFrmGet (cell, repTime);
   queue = &(dlSf->ueLst);
   rgSchAckNackRepUtlHdlTti (cell, queue);

   return ROK;
}

 /** @brief This function Marks the UE as ackNakRep so that Reception request
  * isnt sent for any other thing than HARQ.
  *
  * @details
  *
  *     Function: 
  *
  *         Processing steps:
  *         - Loop through HARQ procs of the DlSf.
  *         - If the UE is a repeating one 
  *           - Mark as ackNakRep = TRUE
  *           - Start the timer 
  *
  * @param  RgSchCellCb *cell
  * @param  CmLListCp   *ackNakRepQ
  * @return Void
  */
static Void rgSchAckNackRepUtlHdlTti(RgSchCellCb *cell,CmLListCp   *ackNackRepQ)
{
   CmLList           *node;
   RgSchUeCb         *ue;

   node = ackNackRepQ->first;
   while (node)
   {
      ue = (RgSchUeCb *)(node->node);
      if ((NULLP != ue) && (ue->ackNakRepCb.isAckNackEnabled))
      {
         ue->ackNakRepCb.isAckNakRep = TRUE;
         rgSCHTmrStartTmr (cell, ue, RG_SCH_TMR_ACKNACK_REP, 
               ue->ackNakRepCb.repCntr);
      }
      node = node->next;
   } /* end of while */
   return;
} /* end of */ 


 /** @brief This function 
  *
  * @details
  *
  *     Function: 
  *
  *         Processing steps:
  *         - 
  *
  *
  * @param  
  * @param 
  * @return 
  */
static Void rgSCHMeasGapANRepUtlHdlTti(RgSchCellCb *cell,CmLListCp   *measGapQ)
{
   CmLList           *node;
   RgSchUeCb         *ue;

   node = measGapQ->first;
   while (node)
   {
      ue    = (RgSchUeCb*)node->node;
      ue->measGapCb.isMeasuring = TRUE;
      rgSCHTmrStartTmr (cell, ue, RG_SCH_TMR_MEASGAP, RG_SCH_MEAS_GAP_LEN);
      node = node->next;
   } /* end of while */
   return;
} /* end of */ 


/**
 * @brief Determines the list of UEs inactive for DL scheduling due to
 * measurement gap and ack nack repetitions
 *
 * @details
 *
 *     Function : rgSCHMeasGapANRepGetDlInactvUe
 *
 *     Invoking Module Processing:
 *     - This API shall be invoked to get list of inactive UEs for downlink
 *     scheduling due to measurement gaps and ACK NACK repetitions.
 *        
 *     Processing Steps:
 *     - Compute qOffset for 40ms or 80ms queue as = 
 *     ((cell->crntTime->sfn * 10)+cell->crntTime->subframe + DL_DELTA +
 *     RG_SCH_CMN_HARQ_INTERVAL)
 *     % RG_MEAS_GAPPRD_40 or RG_MEAS_GAPPRD_80.
 *       - Add all the UEs at computed offset to dlInactvUeLst since the
 *         DL transmission or feedback transmission from UE for DL 
 *         transmissions shall collide with measurement gap.
 *       - Mark each UE. Set dlInactvMask |= RG_MEASGAP_INACTIVE
 *       - Start measGapDlInactvTmr timer for each UE,
 *         - length = RG_MEAS_GAP_LEN + RG_SCH_CMN_HARQ_INTERVAL
 *         - event = RG_MEASGAP_DLINACTV
 *         - handler = rgSCHMeasGapANRepDlInactvTmrExpry
 *     - Compute qOffset for ACK NACK repetition queue as = 
 *     ((cell->crntTime->sfn * 10)+cell->crntTime->subframe + DL_DELTA +
 *     RG_SCH_CMN_HARQ_INTERVAL -1)
 *     % RG_MAX_NUM_DLSF.
 *       - Add all the UEs at computed offset to dlInactvUeLst since the
 *         feedback transmission from UE for DL transmissions shall 
 *         collide with ACK NACK repetition of the UE.
 *       - Mark each UE. Set dlInactvMask |= RG_ACKNAKREP_INACTIVE
 *       - Start ackNakRepDlInactvTmr timer for each UE,
 *         - length = repCntr - 1
 *         - event = RG_ACKNAKREP_DLINACTV
 *         - handler = rgSCHMeasGapANRepDlInactvTmrExpry
 *     - Verify the above computations before coding
 *       
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[out] CmLListCp    *dlInactvUeLst
 *  @return  S16
 *      -# ROK
 **/
S16 rgSCHMeasGapANRepGetDlInactvUe(RgSchCellCb *cell,CmLListCp   *dlInactvUeLst)
{
   uint8_t           offset;
   CmLList           *node;
   CmLList           *hqNode;
   CmLListCp         *queue;
   RgSchUeCb         *ue;
   RgSchDlSf         *dlSf;
   CmLteTimingInfo   ackNakTime;
   uint16_t          schedTime;
   uint8_t           harqFdbkOffset;
#ifdef LTE_TDD
   uint8_t           repCntr;
#endif
   RgSchDlHqProcCb   *hqP;
   RgSchDlHqTbCb     *tbCb;
   uint32_t          i;

   schedTime = cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G + cell->crntTime.slot + RG_DL_DELTA; 

#ifdef LTE_TDD
   RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, ackNakTime, RG_DL_DELTA);
   if(rgSchTddUlDlSubfrmTbl[cell->ulDlCfgIdx][ackNakTime.subframe] !=
         RG_SCH_TDD_DL_SUBFRAME)
   {
      return ROK;
   }

   dlSf = rgSCHUtlSubFrmGet (cell, ackNakTime);
   if(dlSf->dlFdbkInfo.sfnOffset > 0)
   {
      harqFdbkOffset =  
         (dlSf->dlFdbkInfo.sfnOffset - 1) * RGSCH_NUM_SUB_FRAMES+ \
         RGSCH_NUM_SUB_FRAMES - ackNakTime.subframe + \
         dlSf->dlFdbkInfo.subframe;
   }
   else
   {
      harqFdbkOffset =  dlSf->dlFdbkInfo.subframe - ackNakTime.subframe;
   }
#else
   harqFdbkOffset = RG_SCH_CMN_HARQ_INTERVAL;
#endif
   /* Calc offset for Measurement gap 40 */
   offset = (schedTime + harqFdbkOffset) % RG_MEAS_GAPPRD_40;
   queue = &(cell->measGapCb.gapPrd40Q[offset]);

   node = queue->first;
   while (node)
   {
      ue = (RgSchUeCb*)node->node;
      ue->dl.dlInactvMask |= RG_MEASGAP_INACTIVE;
      /* Add to the inactv list */
      ue->dl.dlInactvLnk.node = (PTR)ue;
      cmLListAdd2Tail (dlInactvUeLst, &(ue->dl.dlInactvLnk));
      /* Start timer */
      rgSCHTmrStartTmr (cell, ue, RG_SCH_TMR_DL_MEASGAP, 
            (RG_SCH_MEAS_GAP_LEN + harqFdbkOffset));
      node = node->next;
   }

   /* Calc offset for Measurement gap 80 */
   offset = (schedTime + harqFdbkOffset) % RG_MEAS_GAPPRD_80;
   queue = &(cell->measGapCb.gapPrd80Q[offset]);

   node = queue->first;
   while (node)
   {
      ue = (RgSchUeCb*)node->node;
      ue->dl.dlInactvMask |= RG_MEASGAP_INACTIVE;
      /* Add to the inactv list */
      ue->dl.dlInactvLnk.node = (PTR)ue;
      cmLListAdd2Tail (dlInactvUeLst, &(ue->dl.dlInactvLnk));
      /* Start timer */
      rgSCHTmrStartTmr (cell, ue, RG_SCH_TMR_DL_MEASGAP, 
            (RG_SCH_MEAS_GAP_LEN + harqFdbkOffset));
      node = node->next;
   }

   /* Calc offset for ACK NACK repetition */
   /*offset = (cell->crntTime.sfn * 10 + 
     cell->crntTime.slot + RG_DL_DELTA + RG_SCH_CMN_HARQ_INTERVAL - 1) 
     % RG_MAX_NUM_DLSF;*/
   /* The ackNakRepQ resides in each dlSf corresponding to the repStart */
   /* Must pick up the subframe that was scheduled in the last TTI */
#ifdef LTE_TDD
   if(cell->ulDlCfgIdx == 5)
   {
      return ROK;
   }
   rgSCHUtlGetPrevDlSfInfo(cell, ackNakTime, &ackNakTime, &repCntr);
   dlSf = rgSCHUtlSubFrmGet (cell, ackNakTime);
   /* crnt DL subframe */
   RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, ackNakTime, RG_DL_DELTA);
#else
   if(0 == RG_DL_DELTA)
   {
     /* Go to the subframe being scheduled */
     RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, ackNakTime, RG_DL_DELTA);
     /* Go to the previous subframe */
     RGSCHDECRFRMCRNTTIME(ackNakTime, ackNakTime, 1);
   }
   else
   {
     RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, ackNakTime,
           (RG_DL_DELTA - 1));
   }
   dlSf = rgSCHUtlSubFrmGet (cell, ackNakTime);
#endif
   queue = &(dlSf->ueLst);

   node = queue->first;
   while (node)
   {
      ue = (RgSchUeCb *)(node->node);
      node = node->next;
      hqNode = ue->dl.dlSfHqInfo[cell->cellId][dlSf->dlIdx].hqPLst.first;
      while (hqNode)
      {
         hqP = (RgSchDlHqProcCb *)hqNode->node;
         hqNode = hqNode->next;
         for (i = 0;(i<2) && (hqP->tbInfo[i].state == HQ_TB_WAITING);i++)
         {
            tbCb = &hqP->tbInfo[i];
            if (tbCb->fbkRepCntr > 0)
            {
               ue->dl.dlInactvMask |= RG_ACKNAKREP_INACTIVE;
               /* Check if already added to the list */
               if (!(ue->dl.dlInactvMask & RG_MEASGAP_INACTIVE))
               {
                  /* Add to the inactv list */
                  ue->dl.dlInactvLnk.node = (PTR)ue;
                  cmLListAdd2Tail (dlInactvUeLst, &(ue->dl.dlInactvLnk));
               }
               /* Start timer */
#ifdef LTE_TDD
               repCntr = rgSCHAckNakRepFindUlDuration(cell, dlSf, ackNakTime, 
                     (uint8_t)(ue->ackNakRepCb.repCntr - 1));
               rgSCHTmrStartTmr (cell, ue, RG_SCH_TMR_DL_ACKNACK, repCntr);
#else
               rgSCHTmrStartTmr (cell, ue, RG_SCH_TMR_DL_ACKNACK, 
                     (ue->ackNakRepCb.repCntr - 1));
#endif
            }
         }
      }
   }
   return ROK;
}

/**
 * @brief Determines the list of UEs inactive for UL scheduling due to
 * measurement gap and ack nack repetitions
 *
 * @details
 *
 *     Function : rgSCHMeasGapANRepGetUlInactvUe
 *
 *     Invoking Module Processing:
 *     - This API shall be invoked to get list of inactive UEs for uplink
 *     scheduling due to measurement gaps and ACK NACK repetitions.
 *        
 *     Processing Steps:
 *     - Compute qOffset for 40ms or 80ms queue as = 
 *     ((cell->crntTime->sfn * 10)+cell->crntTime->subframe + TFU_DELTA +
 *     RG_SCH_CMN_HARQ_INTERVAL)
 *     % RG_MEAS_GAPPRD_40 or RG_MEAS_GAPPRD_80.
 *       - Add all the UEs at computed offset to ulInactvUeLst since the UL
 *         transmissions shall collide with measurement gap.
 *       - Mark each UE. Set ulInactvMask |= RG_MEASGAP_INACTIVE
 *       - Start measGapUlInactvTmr timer for each UE 
 *         - length = RG_MEAS_GAP_LEN
 *         - event = RG_MEASGAP_ULINACTV
 *         - handler = rgSCHMeasGapANRepUlInactvTmrExpry
 *     - Compute qOffset for ACK NACK repetition queue as = 
 *     ((cell->crntTime->sfn * 10)+cell->crntTime->subframe + TFU_DELTA +
 *     RG_SCH_CMN_HARQ_INTERVAL)
 *     % RG_MAX_NUM_DLSF.
 *       - Add all the UEs at computed offset to ulInactvUeLst since the
 *         feedback transmission from UE for DL transmissions shall 
 *         collide with repeating ACK ACK-NACKs.
 *       - Mark each UE. Set ulInactvMask |= RG_ACKNAKREP_INACTIVE
 *       - Start ackNakRepUlInactv timer for each UE
 *         - length = repCntr
 *         - event = RG_ACKNAKREP_ULINACTV
 *         - handler = rgSCHMeasGapANRepUlInactvTmrExpry
 *     - Verify the above computations before coding
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[out] CmLListCp    *ulInactvUeLst
 *  @return  S16
 *      -# ROK
 **/

S16 rgSCHMeasGapANRepGetUlInactvUe(RgSchCellCb *cell,CmLListCp   *ulInactvUeLst)
{

   uint8_t         offset;
   CmLList         *node;
   CmLList         *hqNode;
   CmLListCp       *queue;
   RgSchUeCb       *ue;
   CmLteTimingInfo ackNakTime;
   RgSchDlSf       *dlSf;
   uint16_t        schedTime;
   uint8_t         pdcchToPuschGap;
   uint8_t         idx=0;
#ifdef LTE_TDD
   uint8_t         repCntr;
#endif
   RgSchDlHqProcCb   *hqP;
   RgSchDlHqTbCb     *tbCb;
   uint32_t          i;

   /*ccpu00139481- Meas Gap should be monitored in UL with TFU_ULCNTRL_DLDELTA*/
   schedTime = cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES_5G + cell->crntTime.slot + \
               TFU_ULCNTRL_DLDELTA;
#ifndef LTE_TDD
   pdcchToPuschGap = RGSCH_PDCCH_PUSCH_DELTA;
#else
   RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, ackNakTime, TFU_ULCNTRL_DLDELTA);
   pdcchToPuschGap = rgSchTddPuschTxKTbl[cell->ulDlCfgIdx][ackNakTime.subframe];
   for(idx=0; pdcchToPuschGap && (idx< (pdcchToPuschGap+RG_SCH_MEAS_GAP_LEN)) ; idx++)
#endif
   {
      /* Calc offset for Measurement gap 40 */
      offset = (schedTime + pdcchToPuschGap -idx + RG_MEAS_GAPPRD_40) % RG_MEAS_GAPPRD_40;
      queue = &(cell->measGapCb.gapPrd40Q[offset]);
      node = queue->first;
      while (node)
      {
         ue = (RgSchUeCb*)node->node;
         if(!(ue->ul.ulInactvMask & RG_MEASGAP_INACTIVE))
         {
         ue->ul.ulInactvMask |= RG_MEASGAP_INACTIVE;
         /* Add to the inactv list */
         ue->ul.ulInactvLnk.node = (PTR)ue;
         cmLListAdd2Tail (ulInactvUeLst, &(ue->ul.ulInactvLnk));
         /* Start timer Note the timer is started for a value GAP_LEN +
          * RG_SCH_CMN_HARQ_INTERVAL. The "4"
          * is added because for UE to transmit, the PDCCH must be sent 4 subframes
          * ahead - UE cant read PDCCH format0 if it is in measurement gap. */
         rgSCHTmrStartTmr (cell, ue, RG_SCH_TMR_UL_MEASGAP, 
               (RG_SCH_MEAS_GAP_LEN + pdcchToPuschGap - idx));
   //DU_LOG("\nINFO  -->  SCH : Starting Meas Gap 40 @ DL TTI- (%d:%d) K-%d offset-%d Len %d \n", ackNakTime.sfn,\
   ackNakTime.subframe, harqFdbkOffset, offset, RG_SCH_MEAS_GAP_LEN + harqFdbkOffset-idx);
         }
         node = node->next;
      }

      /* Calc offset for Measurement gap 80 */
      offset = (schedTime + pdcchToPuschGap - idx + RG_MEAS_GAPPRD_80) % RG_MEAS_GAPPRD_80;
      queue = &(cell->measGapCb.gapPrd80Q[offset]);

      node = queue->first;
      while (node)
      {
         ue = (RgSchUeCb*)node->node;
         if(!(ue->ul.ulInactvMask & RG_MEASGAP_INACTIVE))
         {
         ue->ul.ulInactvMask |= RG_MEASGAP_INACTIVE;
         /* Add to the inactv list */
         ue->ul.ulInactvLnk.node = (PTR)ue;
         cmLListAdd2Tail (ulInactvUeLst, &(ue->ul.ulInactvLnk));
         /* Start timer */
         rgSCHTmrStartTmr (cell, ue, RG_SCH_TMR_UL_MEASGAP, 
               (RG_SCH_MEAS_GAP_LEN + pdcchToPuschGap - idx));
   //DU_LOG("\nINFO  -->  SCH : Starting Meas Gap 80 @ DL TTI- (%d:%d) K-%d offset-%d Len %d \n", ackNakTime.sfn,\
   ackNakTime.subframe, harqFdbkOffset, offset, RG_SCH_MEAS_GAP_LEN + harqFdbkOffset-idx);
         }
         node = node->next;
      }
   }
   /* Calc offset for ACK NACK repetition */
   /*offset = (cell->crntTime.sfn * 10 + 
     cell->crntTime.slot + RG_UL_SCHED_DELTA + 
     RG_SCH_CMN_HARQ_INTERVAL ) % RG_MAX_NUM_DLSF;*/

   /* Must get the DLSF that is scheduled at TFU_DELTA Away */
#ifdef LTE_TDD
   if(cell->ulDlCfgIdx == 5)
   {
      return ROK;
   }
   RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, ackNakTime, TFU_DELTA);
   if(rgSchTddUlDlSubfrmTbl[cell->ulDlCfgIdx][ackNakTime.subframe] !=
         RG_SCH_TDD_DL_SUBFRAME)
   {
      return ROK;
   }
#else
   /* Introduced timing delta for DL control in FDD */
   RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, ackNakTime, TFU_DLCNTRL_DLDELTA);
#endif
   dlSf = rgSCHUtlSubFrmGet (cell, ackNakTime);
   queue = &(dlSf->ueLst);

   node = queue->first;
   while (node)
   {
      ue = (RgSchUeCb *)(node->node);
      node = node->next;
      hqNode = ue->dl.dlSfHqInfo[cell->cellId][dlSf->dlIdx].hqPLst.first;
      while (hqNode)
      {
         hqP = (RgSchDlHqProcCb *)hqNode->node;
         hqNode = hqNode->next;
         for (i = 0;(i<2) && (hqP->tbInfo[i].state == HQ_TB_WAITING);i++)
         {
            tbCb = &hqP->tbInfo[i];
            if (tbCb->fbkRepCntr > 0)
            {
               ue->ul.ulInactvMask |= RG_ACKNAKREP_INACTIVE;
               /* Check if already added to the list */
               if (!(ue->ul.ulInactvMask & RG_MEASGAP_INACTIVE))
               {
                  /* Add to the inactv list */
                  ue->ul.ulInactvLnk.node = (PTR)ue;
                  cmLListAdd2Tail (ulInactvUeLst, &(ue->ul.ulInactvLnk));
               }
               /* Start timer */
#ifdef LTE_TDD
               repCntr = rgSCHAckNakRepFindUlDuration(cell, dlSf, ackNakTime, 
                     ue->ackNakRepCb.repCntr);
               rgSCHTmrStartTmr (cell, ue, RG_SCH_TMR_UL_ACKNACK, repCntr);
#else
               rgSCHTmrStartTmr (cell, ue, RG_SCH_TMR_UL_ACKNACK, 
                     (ue->ackNakRepCb.repCntr));
#endif
            }
         }
      }
   }
   return ROK;
}

/**
 * @brief Handles processing of DL Inactive timer expiry at the end of
 * measurement gap or ack nack repetition for a UE
 *
 * @details
 *
 *     Function : rgSCHMeasGapANRepDlInactvTmrExpry
 *
 *     Invoking Module Processing:
 *     - This API shall be invoked to process DL inactive timer expiry
 *        
 *     Processing Steps:
 *     - If timer event is RG_MEASGAP_DLINACTV,
 *       - dlInactvMask &= ~RG_MEASGAP_INACTIVE
 *     - If timer event is RG_ACKNAKREP_DLINACTV,
 *       - dlInactvMask &= ~RG_ACKNAKREP_INACTIVE
 *     - if (!dlInactvMask)
 *       - Invoke DL scheduler to put the UE back into the scheduling queues.
 *       - Re-initialize timer.
 *     - Return ROK
 *
 *  @param[in]  RgSchUeCb  *ue
 *  @param[in]  Check if some timer related parameter needs to be passed
 *  @return  S16
 *      -# ROK
 **/

Void rgSCHMeasGapANRepDlInactvTmrExpry(RgSchUeCb *ue,uint8_t tmrEvnt)
{

   RgSchCellCb    *cell = ue->cell;

   switch (tmrEvnt)
   {
      case RG_SCH_TMR_DL_MEASGAP:
         RG_SCH_CMN_DL_UPDT_INACTV_MASK ( cell, ue, RG_MEASGAP_INACTIVE);
         break;
      case RG_SCH_TMR_DL_ACKNACK:
         RG_SCH_CMN_DL_UPDT_INACTV_MASK ( cell, ue, RG_ACKNAKREP_INACTIVE);
         break;
   }
   if (!ue->dl.dlInactvMask)
   {
      cmInitTimers (&ue->measGapCb.measGapDlInactvTmr, 1);
      cmInitTimers (&ue->ackNakRepCb.ackNakRepDlInactvTmr, 1);
   }
   return;
}

/**
 * @brief Handles processing of UL Inactive timer expiry at the end of
 * measurement gap or ack nack repetition for a UE
 *
 * @details
 *
 *     Function : rgSCHMeasGapANRepUlInactvTmrExpry
 *
 *     Invoking Module Processing:
 *     - This API shall be invoked to process UL inactive timer expiry
 *        
 *     Processing Steps:
 *     - If timer event is RG_MEASGAP_ULINACTV,
 *       - ulInactvMask &= ~RG_MEASGAP_INACTIVE
 *     - If timer event is RG_ACKNAKREP_ULINACTV,
 *       - ulInactvMask &= ~RG_ACKNAKREP_INACTIVE
 *     - if (!ulInactvMask)
 *       - Invoke UL scheduler to put the UE back into the scheduling queues.
 *       - Re-initialize timer.
 *     - Return ROK
 *
 *  @param[in]  RgSchUeCb  *ue
 *  @param[in]  Check if some timer related parameter needs to be passed
 *  @return  S16
 *      -# ROK
 **/

Void rgSCHMeasGapANRepUlInactvTmrExpry(RgSchUeCb *ue,uint8_t tmrEvnt)
{
   RgSchCellCb       *cell = ue->cell;

   switch (tmrEvnt)
   {
      case RG_SCH_TMR_UL_MEASGAP:
         RG_SCH_CMN_UL_UPDT_INACTV_MASK ( cell, ue, RG_MEASGAP_INACTIVE);
         break;
      case RG_SCH_TMR_UL_ACKNACK:
         RG_SCH_CMN_UL_UPDT_INACTV_MASK ( cell, ue, RG_ACKNAKREP_INACTIVE);
         break;
   }
   if (!ue->ul.ulInactvMask)
   {
      cmInitTimers (&ue->measGapCb.measGapUlInactvTmr, 1);
      cmInitTimers (&ue->ackNakRepCb.ackNakRepUlInactvTmr, 1);
   }
   return;
}

/**
 * @brief Handles processing of measurement gap timer expiry at the end of
 * measurement gap
 *
 * @details
 *
 *     Function : rgSCHMeasGapANRepTmrExpry
 *
 *     Invoking Module Processing:
 *     - This API shall be invoked to process measurement gap timer expiry
 *        
 *     Processing Steps:
 *     - Set ue->isMeasuring = FALSE
 *     - Re-initialize timer.
 *     - Return ROK
 *
 *  @param[in]  RgSchUeCb  *ue
 *  @param[in]  Check if some timer related parameter needs to be passed
 *  @return  S16
 *      -# ROK
 **/

Void rgSCHMeasGapANRepTmrExpry(RgSchUeCb *ue)
{

   ue->measGapCb.isMeasuring = FALSE;
   cmInitTimers (&ue->measGapCb.measGapTmr, 1);

   return;
}

/**
 * @brief Handles processing of ACK-NACK repetition timer expiry at the end of
 * ACK-NACK repetition.
 *
 * @details
 *
 *     Function : rgSchAckNakRepTmrExpry
 *
 *     Invoking Module Processing:
 *     - This API shall be invoked to process ACK-NACK repetition timer expiry
 *        
 *     Processing Steps:
 *     - Set ue->isAckNakRep = FALSE
 *     - Re-initialize timer.
 *     - Return ROK
 *
 *  @param[in]  RgSchUeCb  *ue
 *  @param[in]  Check if some timer related parameter needs to be passed
 *  @return  S16
 *      -# ROK
 **/

Void rgSCHAckNakRepTmrExpry(RgSchUeCb *ue)
{

   ue->ackNakRepCb.isAckNakRep = FALSE;
   cmInitTimers (&ue->ackNakRepCb.ackNakRepTmr, 1);

   return; 
}


/**
 * @brief Adds ACK-NACK repeating UEs to the ackNakRepQ
 *
 * @details
 *
 *     Function : rgSchAckNakRepAddToQ
 *
 *     Invoking Module Processing:
 *     - This API shall be invoked for adding list of UEs to the ACK-NACK
 *     repeating queue at appropriate poistion. Invoking module shall invoke
 *     with the list of ACK-NACK repeating UEs for a sub-frame and timing info
 *     at which ACK NACK repetition shall start for the UEs.
 *        
 *     Processing Steps:
 *     - Determine the qOffset depending on the timing info as
 *       - qOffset = (repStartTime->sfn *10 + repStartTime->subframe) % RG_MAX_NUM_DLSF
 *     - Initialize the list at the qOffset. 
 *     - For each UE in the list,
 *       - Add the UE to ackNakRepQ to the list at the determined qOffset.
 *       - Set ue->qOffset = qOffset
 *       - Initialize timers.
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchDlSf    *crntDlSf
 *  @param[in]  CmLteTimingInfo repStartTime
 *
 *  @return  S16
 *      -# ROK
 **/

Void rgSCHAckNakRepAddToQ(RgSchCellCb       *cell,RgSchDlSf         *crntDlSf)
{
   RgSchUeCb         *ue;
   CmLList           *node;
   CmLList           *hqNode;
   RgSchDlHqProcCb   *hqP;
   RgSchDlHqTbCb     *tbCb;
   uint32_t               i;

   node = crntDlSf->ueLst.first;
   while (node)
   {
      ue = (RgSchUeCb *)(node->node);
      node = node->next;
      hqNode = ue->dl.dlSfHqInfo[cell->cellId][crntDlSf->dlIdx].hqPLst.first;
      while (hqNode)
      {
         hqP = (RgSchDlHqProcCb *)hqNode->node;
         hqNode = hqNode->next;
         for (i = 0;(i<2) && (hqP->tbInfo[i].state == HQ_TB_WAITING);i++)
         {
            tbCb = &hqP->tbInfo[i];
            /* Add UEs that have enabled ACK NACK repetition */
            if (ue->ackNakRepCb.isAckNackEnabled)
            {
               tbCb->fbkRepCntr = ue->ackNakRepCb.cfgRepCnt;
               tbCb->fbkRecpRepCntr = ue->ackNakRepCb.cfgRepCnt;
               /* Removed init of timers as the init will be happening during
                * config or timer expiry*/
            }
         }
      }
   }
   return;
}


#ifdef LTE_TDD
/**
 * @brief Finds the number of subframes used for ACK-NACK cycle
 *
 * @details
 *
 *     Function : rgSCHAckNakRepFindUlDuration
 *
 *                This function finds the number of subframes required
 *                for ACK-NACK repetition cycle based on UL subframes.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchDlSf        *dlSf
 *  @param[in]  CmLteTimingInfo  repTime
 *  @param[in]  uint8_t           repCnt
 *
 *  @return  uint8_t
 *      
 **/

static uint8_t rgSCHAckNakRepFindUlDuration(RgSchCellCb *cell,RgSchDlSf *dlSf,CmLteTimingInfo repTime,uint8_t repCnt)
{
   CmLteTimingInfo   ulfrm;
   uint8_t    noSubfrms = 0;
   uint16_t   ulDlCfgIdx = cell->ulDlCfgIdx;
   S16   rem = 0;
   S16   idx;
   S8    diff;

   rgSCHAckNakRepGetUlOffsetFrmDl(dlSf, repTime, &noSubfrms);
   RG_SCH_ADD_TO_CRNT_TIME(repTime, ulfrm, noSubfrms);
   diff = repCnt-1;
   idx = ulfrm.subframe;
   while(diff)
      {
      idx = (idx + 1) % RGSCH_NUM_SUB_FRAMES;
         if(rgSchTddUlDlSubfrmTbl[ulDlCfgIdx][idx] ==
                                    RG_SCH_TDD_UL_SUBFRAME)
         {
            diff--;
         }
      rem++;
      }
   noSubfrms += rem;

   return (noSubfrms);
   }

/**
 * @brief Finds the number of subframes used for ACK-NACK cycle
 *
 * @details
 *
 *     Function : rgSCHAckNakRepGetUlOffsetFrmDl
 *
 *                This function finds the number of subframes after
 *                which UL subframes are present for the gicen DL
 *                subframe.
 *
 *  @param[in]  RgSchDlSf        *dlSf
 *  @param[in]  CmLteTimingInfo  crntDlTime
 *  @param[in]  uint8_t               *noSubfrms
 *
 *  @return  uint8_t
 *      
 **/

static Void rgSCHAckNakRepGetUlOffsetFrmDl(RgSchDlSf *dlSf,CmLteTimingInfo crntDlTime,uint8_t *noSubfrms )
{

   if(dlSf->dlFdbkInfo.sfnOffset != 0)
            {
      *noSubfrms = (dlSf->dlFdbkInfo.sfnOffset - 1) * RGSCH_NUM_SUB_FRAMES;
      *noSubfrms = *noSubfrms + RGSCH_NUM_SUB_FRAMES - crntDlTime.subframe;
      *noSubfrms = *noSubfrms + dlSf->dlFdbkInfo.subframe;
      }
   else
   {
      *noSubfrms = dlSf->dlFdbkInfo.subframe - crntDlTime.subframe;
   }
   return;
}
#endif

/**********************************************************************
 
         End of file
**********************************************************************/
