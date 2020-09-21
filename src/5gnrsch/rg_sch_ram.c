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
  
     File:     rg_sch_ram.c
  
**********************************************************************/

/** @file rg_sch_ram.c
@brief This file has APIs to handle the random access procedure functionality for the scheduler.
*/

static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_MODULE_ID=4096;
static int RLOG_FILE_ID=171;

/* header include files (.h) */
#include "common_def.h"

#include "rg_env.h"        /* MAC Environment Defines */
#include "rgr.h"           /* RGR Interface defines */
#include "rgm.h"           /* RGR Interface defines */
#include "tfu.h"           /* TFU Interface defines */
#include "lrg.h"           /* LRG Interface defines */
#include "rg_env.h"    /* Scheduler error defines */
#include "rg_sch_inf.h"        /* Scheduler defines */
#include "rg_sch_err.h"    /* Scheduler error defines */
#include "rg_sch.h"        /* Scheduler defines */
#include "rg_sch_cmn.h"
#include "rl_interface.h"
#include "rl_common.h"

/* header/extern include files (.x) */

#include "rgr.x"           /* RGR Interface includes */
#include "rgm.x"           /* RGR Interface includes */
#include "tfu.x"           /* TFU Interface includes */
#include "lrg.x"           /* LRG Interface includes */

#include "rg_sch_inf.x"         /* typedefs for Scheduler */
#include "rg_sch.x"        /* Scheduler includes */
#include "rg_sch_cmn.x"
#ifdef EMTC_ENABLE
EXTERN Bool rgSCHRamVldtRgrEmtcUeCfg  ARGS((
RgSchCellCb  *cell,
RgrUeCfg     *ueCfg
));

EXTERN S16 rgSCHRamRmvAllFrmEmtcRaInfoSchdLst
(
RgSchCellCb       *cell
);
EXTERN Void rgSCHEmtcUtlUpdCmnNb 
(
RgSchRaCb      *raCb
);
EXTERN Void rgSCHEmtcHqPAlloc 
(
RgSchCellCb       *cell,
RgSchDlHqEnt      *hqEnt
);
#endif
/* local defines */
/* local typedefs */
PRIVATE Void rgSCHRamUlFreeAllocation ARGS((RgSchUlSf *sf,RgSchUlAlloc *alloc, 
         RgSchCellCb     *cell,Bool isEmtc));

PRIVATE S16 rgSCHRamContResCrnti   ARGS((RgSchCellCb *cell, RgSchUeCb *ue, 
                                      RgSchRaCb *raCb, RgSchErrInfo *err));
PRIVATE S16 rgSCHRamContResCcchsdu ARGS((RgSchCellCb *cell, RgSchRaCb *raCb));
#ifdef EMTC_ENABLE

EXTERN S16 rgSCHEmtcRamContResCcchsdu ARGS((RgSchCellCb *cell, RgSchRaCb *raCb));
EXTERN S16 rgSCHRamEmtcContResCcchsdu ARGS((RgSchCellCb *cell, RgSchRaCb *raCb));
EXTERN Void rgSCHChkEmtcContResGrdTmrExp ARGS((RgSchCellCb        *cell));
EXTERN Void rgSCHChkEmtcContResTmrExp ARGS((RgSchCellCb        *cell));
EXTERN Void rgSCHEmtcRaInfoFree ARGS((RgSchCellCb *cell, RgSchRaCb *raCb));
#endif
#ifdef RGR_V1
PRIVATE Void rgSCHChkContResGrdTmrExp ARGS((RgSchCellCb        *cell));
PRIVATE Void rgSCHChkContResTmrExp ARGS((RgSchCellCb        *cell));
PRIVATE Void rgSCHRamProcContResExp ARGS((RgSchCellCb *cell, 
                                 RgSchRaCb  *raCb));
PRIVATE Void rgSCHRamProcContResGrdExp ARGS((RgSchCellCb *cell,
                                      RgSchRaCb  *raCb));
#ifdef EMTC_ENABLE
EXTERN Void rgSCHChkEmtcContResGrdTmrExp ARGS((RgSchCellCb        *cell));
EXTERN Void rgSCHChkEmtcContResTmrExp ARGS((RgSchCellCb        *cell));
#endif
#endif
/* forward references */

/**
 * @brief Check configured preamble id not colliding with non dedicated or PDCCH
 * order preamble sets. When valid preamble id given check that C-RNTI given
 * in configuration is not amongst the C-RNTI'smanaged by scheduler
 *
 * @details
 *
 *     Function : rgSCHRamVldtUeCfg 
 *
 *     Processing Steps: Check configured preamble id not colliding with non dedicated or PDCCH
 *       order preamble sets. When valid preamble id given check that C-RNTI given
 *       in configuration is not amongst the C-RNTI'smanaged by scheduler
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgrUeCfg     *ueCfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
S16 rgSCHRamVldtUeCfg 
(
RgSchCellCb  *cell,
RgrUeCfg     *ueCfg
)
#else
S16 rgSCHRamVldtUeCfg(cell, ueCfg)
RgSchCellCb  *cell;
RgrUeCfg     *ueCfg;
#endif
{
   TRC2(rgSCHRamVldtUeCfg);
   if (ueCfg->dedPreambleId.pres == PRSNT_NODEF)
   {
      if ((ueCfg->dedPreambleId.val < cell->rachCfg.numRaPreamble) ||
          (ueCfg->dedPreambleId.val >= RGSCH_MAX_NUM_RA_PREAMBLE) ||
          ((ueCfg->dedPreambleId.val >= cell->macPreambleSet.start) && 
           (ueCfg->dedPreambleId.val <= cell->macPreambleSet.start +
                                       cell->macPreambleSet.size - 1)) ||
          ((ueCfg->crnti >= cell->rntiDb.rntiStart) && 
           (ueCfg->crnti < cell->rntiDb.rntiStart + cell->rntiDb.maxRntis-1))
#ifdef EMTC_ENABLE
          || (rgSCHRamVldtRgrEmtcUeCfg(cell,ueCfg))
#endif          
          )
      {
         return RFAILED;
      }
   }
   return ROK;
}

/**
 * @brief Handler for Random Access Request
 *
 * @details
 *
 *     Function : rgSCHRamProcRaReq
 *     
 *     -# Create a node for each TfuRaReqInfo element received
 *     -# Initialize the list with the above elements at the raRnti index 
 *        in the cell. 
 *     
 *           
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  CmLteRnti      raRnti
 *  @param[in]  RgTfuRaReqInd *raReqInd
 *  @param[out] RgSchErrInfo      *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
S16 rgSCHRamProcRaReq
(
U8                raReqCnt,
RgSchCellCb       *cell,
CmLteRnti         raRnti,
TfuRachInfo       *raReqInd,
CmLteTimingInfo   timingInfo,
RgSchUeCb         *ue,
RgSchErrInfo      *err
)
#else
S16 rgSCHRamProcRaReq(raReqCnt, cell, raRnti, raReqInd, timingInfo, ue, err)
U8                raReqCnt;
RgSchCellCb       *cell;
CmLteRnti         raRnti;
TfuRachInfo       *raReqInd;
CmLteTimingInfo   timingInfo;
RgSchUeCb         *ue;
RgSchErrInfo      *err;
#endif
{
   RgSchRaReqInfo *raReqInfo;
   U16            raIndex;
#ifdef LTE_TDD
   U8             fid;
   U8             tid;
#endif

   TRC2(rgSCHRamProcRaReq)


      /* SR_RACH_STATS : RACH REQ */
      rgNumPrachRecvd += raReqInd->numRaReqInfo;

   /* ccpu00132523- Moved out this from for loop as updating of raIndex is 
    * relates to only raRnti and all preambles having same raRnti*/
#ifdef LTE_TDD
   /* UL subframes do not occupy all the subframes in a radio frame.
    * So RA Rnti index to be calculated based on actual UL subframe index. */
   /* Get the actual subframe number */
   tid = (raRnti-1)%RGSCH_NUM_SUB_FRAMES;
   /* Get the frequency index in the subframe */
   fid = ((raRnti-1) / RGSCH_NUM_SUB_FRAMES)* RGSCH_NUM_SUB_FRAMES;
   /* Get the index of RA RNTI in the array */
   raIndex = ((timingInfo.sfn % cell->raInfo.maxRaSize) \
               * RGSCH_MAX_RA_RNTI_PER_SUBFRM * RGSCH_NUM_SUB_FRAMES) + \
               tid + fid;
   /* Fixes for RACH handling in TDD: Removed deletion of queued RaReq */
#else
   /* ccpu00132523- Placing the raReq into array based on RA SFN */
   raIndex = (timingInfo.sfn & 1) * RGSCH_MAX_RA_RNTI + raRnti-1;
#endif

   /* allocate new raReqInfos and enqueue them */
   if (raReqInd->raReqInfoArr[raReqCnt].rapId >= RGSCH_MAX_NUM_RA_PREAMBLE)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
               "RARNTI:%d rgSCHTomRaReqInd(): RAM processing failed errType(%d) ",
               raReqInd->raRnti);
      return RFAILED;
   }

   /* SR_RACH_STATS : DED PREAMB*/
   if (RGSCH_IS_DEDPRM(cell, raReqInd->raReqInfoArr[raReqCnt].rapId))
   {
      rgNumDedPream++;
   }


#ifdef LTE_L2_MEAS
   if (raReqInd->raReqInfoArr[raReqCnt].rapId < cell->rachCfg.sizeRaPreambleGrpA)
   {
      cell->raPrmbs.preamGrpA++;
   }
   else if (RGSCH_IS_DEDPRM(cell, raReqInd->raReqInfoArr[raReqCnt].rapId))
   {
      cell->raPrmbs.dedPream++;
   }
   else
   {
      cell->raPrmbs.preamGrpB++;
   }
#endif

   if((rgSCHUtlAllocSBuf(cell->instIdx, (Data **)(&raReqInfo), 
               sizeof(RgSchRaReqInfo))) == RFAILED)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"rgSCHRamProcRaReq(): Allocation"
            " of RaReq failed RARNTI:%d",raRnti);
      err->errCause = RGSCHERR_RAM_MEM_EXHAUST;
      return RFAILED;
   }

   /* Insert the given raReqInfo */
   /* RACHO */
   raReqInfo->timingInfo = timingInfo;
   raReqInfo->raReq = raReqInd->raReqInfoArr[raReqCnt]; 
   raReqInfo->raReqLstEnt.next = NULLP;
   raReqInfo->raReqLstEnt.prev = NULLP;
   raReqInfo->raReqLstEnt.node = (PTR)raReqInfo;
   /* ccpu00133504 */
   raReqInfo->ue = ue;

#ifndef LTE_TDD
   RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, cell->raInfo.raReqLst, raIndex);
#endif
   /* RACHO: If dedicated preamble, then give preference by appending at front */
   if (RGSCH_IS_DEDPRM(cell, raReqInd->raReqInfoArr[raReqCnt].rapId))
   {
      cmLListFirst(&cell->raInfo.raReqLst[raIndex]);
      cmLListInsCrnt(&cell->raInfo.raReqLst[raIndex], &raReqInfo->raReqLstEnt);
   }
   else
   {
      cmLListAdd2Tail(&cell->raInfo.raReqLst[raIndex], &raReqInfo->raReqLstEnt);
   }

   return ROK;
}  /* rgSCHRamProcRaReq */

/**
 * @brief Handler for Random Access control block creation
 *
 * @details
 *
 *     Function : rgSCHRamCreateRaCb
 *                Creates a raCb and gives the same to scheduler for its updation
 *     
 *
 *  @param[in]       RgSchCellCb       *cell 
 *  @param[in, out]  RgSchRaCb         **raCb 
 *  @param[out]      RgSchErrInfo         *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
S16 rgSCHRamCreateRaCb
(
RgSchCellCb       *cell,
RgSchRaCb         **raCb,
RgSchErrInfo      *err
)
#else
S16 rgSCHRamCreateRaCb(cell, raCb, err)
RgSchCellCb       *cell;
RgSchRaCb         **raCb;
RgSchErrInfo      *err;
#endif
{
   RgSchRntiLnk *rntiLnk;
   Inst         inst = cell->instIdx;

   TRC2(rgSCHRamCreateRaCb)

   if((rgSCHUtlAllocSBuf(inst, (Data **)(raCb),
                      sizeof(RgSchRaCb))) == RFAILED)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"rgSCHRamCreateRaCb(): Allocation of "
         "RaCb failed");
      err->errCause = RGSCHERR_RAM_MEM_EXHAUST;
      return RFAILED;
   }

   rntiLnk = rgSCHDbmGetRnti(cell);
   if(rntiLnk != NULLP)
   {
      (*raCb)->rntiLnk = rntiLnk;
      (*raCb)->tmpCrnti = rntiLnk->rnti;
   }
   else
   {

      /* SR_RACH_STATS: RNTI POOL Exhaution */
      rgNumRarFailDuetoRntiExhaustion++;

      /* No rnti available! */
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"rgSCHRamCreateRaCb(): Allocation of "
         "temporary RNTI failed at MAC(CRNTI exhausted)");
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)(raCb), sizeof(RgSchRaCb));
      err->errCause = RGSCHERR_RAM_RNTI_EXHAUST;
      return RFAILED;
   }

   /* Allocate and initialize the DL HARQ portion of the RACB */
   (*raCb)->dlHqE = rgSCHDhmHqEntInit(cell);
   if ((*raCb)->dlHqE == NULLP)
   {
      /* No memory available! */
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"rgSCHRamCreateRaCb(): Creation of"
         " DL HARQ failed");
	  /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)(raCb), sizeof(RgSchRaCb));
      err->errCause = RGSCHERR_RAM_MEM_EXHAUST;
      return RFAILED;
   }
#ifdef EMTC_ENABLE
   (*raCb)->isEmtcRaCb = FALSE;
   rgSCHEmtcHqPAlloc(cell, (*raCb)->dlHqE);
#endif
   (*raCb)->dlHqE->raCb = (*raCb);
   /* Initialize RaCb's contents */
   (*raCb)->timingInfo = cell->crntTime;
   (*raCb)->raState = RGSCH_RA_MSG3_PENDING;
   (*raCb)->toDel = FALSE;
   (*raCb)->phr.pres = FALSE;

   /* Insert the created raCb into raCb list of cell */
   (*raCb)->raCbLnk.node = (PTR)(*raCb);
   cmLListAdd2Tail(&cell->raInfo.raCbLst, &(*raCb)->raCbLnk);
   
   return ROK;
}  /* rgSCHRamCreateRaCb */

/**
 * @brief Handler for Ue Configuration Request
 *
 * @details
 *
 *     Function : rgSCHRamRgrUeCfg
 *     
 *     This function handles the UE config received based on the state of the
 *     raCb.
 *     -# If raCb is in RGSCH_RA_MSG4_PENDING state, it shall update the harq 
 *        information to UeCb and update the references.
 *     -# If raCb is in RGSCH_RA_MSG4_DONE, then it shall free the raCb
 *     
 *           
 *  @param[in]     RgSchCellCb    *cell
 *  @param[in,out] RgSchUeCb      *ue 
 *  @param[in,out] RgSchRaCb      *raCb 
 *  @param[out]    RgSchErrInfo   *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
S16 rgSCHRamRgrUeCfg
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgSchRaCb      *raCb,
RgSchErrInfo   *err
)
#else
S16 rgSCHRamRgrUeCfg(cell, ue, raCb, err)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgSchRaCb      *raCb;
RgSchErrInfo   *err;
#endif
{
   /* Releasing HARQ processes of old UE when ue
    *           reconfig with new crnti */
  /* U32 cnt; */
   RgSchDlHqEnt          **hqEnt = &(RG_SCH_CMN_GET_UE_HQE(ue, cell));
   RgSchCmnUlUe *ueUl = RG_SCH_CMN_GET_UL_UE(ue, cell);
   TRC2(rgSCHRamRgrUeCfg)


   /* Fix : set UE inactive in DL until UE is reinitialization completed */
   ue->dl.dlInactvMask |= RG_HQENT_INACTIVE;
   ue->ul.ulInactvMask |= RG_HQENT_INACTIVE;

   if(raCb->raState ==  RGSCH_RA_MSG4_PENDING)
   {
      raCb->ue = ue;
      ue->rntiLnk = raCb->rntiLnk;
      /* Update UL Harq process information */
      /*ccpu00128820 - MOD - Msg3 alloc double delete issue*/
      ueUl->hqEnt.hqProcCb[raCb->msg3HqProcId].ndi = raCb->msg3HqProc.ndi;    
   }
   else if(raCb->raState == RGSCH_RA_MSG4_DONE)
   {
      ue->rntiLnk = raCb->rntiLnk;
      /* Update UL Harq process information */
      /*ccpu00128820 - MOD - Msg3 alloc double delete issue*/
      ueUl->hqEnt.hqProcCb[raCb->msg3HqProcId].ndi = raCb->msg3HqProc.ndi;      
      /* Fix : syed Assign hqEnt to UE only if msg4 is done */
      rgSCHDhmAssgnUeHqEntFrmRaCb(ue, raCb);
   }
   else
   {
      err->errCause = RGSCHERR_RAM_NO_MSG3_RCVD;
      *hqEnt = NULLP;
      raCb->dlHqE->ue = NULLP;
      return RFAILED;
   }

   return ROK;
}  /* rgSCHRamRgrUeCfg */


/**
 * @brief Handler for C-RNTI based contention resolution
 *
 * @details
 *
 *     Function : rgSCHRamContResCrnti
 *     
 *     This function shall be invoked once Msg3 indicates C-RNTI based
 *     contention resolution.This shall indicate the scheduler regarding
 *     C-RNTI based uplink grant.
 *     
 *           
 *  @param[in,out] RgSchCellCb *cell 
 *  @param[in,out] RgSchUeCb   *ue 
 *  @param[in,out] RgSchRaCb   *raCb 
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHRamContResCrnti
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchRaCb    *raCb,
RgSchErrInfo *err
)
#else
PRIVATE S16 rgSCHRamContResCrnti(cell, ue, raCb, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchRaCb    *raCb;
RgSchErrInfo *err;
#endif
{
   TfuUlCqiRpt   ulCqiRpt;
   RgSchCmnCell  *cellSch= (RgSchCmnCell *)(cell->sc.sch);
   TRC2(rgSCHRamContResCrnti)


   /* Fix: syed It is incorrect to copy over msg3HqProc to ueCb's 
    * UL harq proc. In case of Crnti based RACH, ueCb has valid context which
    * cannot be over written. It was leading to a crash. */ 

   rgSCHUtlRecMsg3Alloc(cell, ue, raCb);

   /* Fix for ccpu00123908: Reset the UL CQI to the cell default value here */
   ulCqiRpt.isTxPort0 = TRUE;
   ulCqiRpt.numSubband = 0;
   /* Fix : syed HO UE does not have a valid ue->rntiLnk */	   
   ulCqiRpt.rnti = ue->ueId;
   /* rg002.301:[ccpu00124018]-MOD- Avoiding hard coding of CQI and retriving from cell config*/
   ulCqiRpt.wideCqi = cellSch->ul.dfltUlCqi;
   rgSCHUtlUlCqiInd(cell, ue, &ulCqiRpt);

   
   /* Invoke scheduler to indicate UL grant req for contention resolution */
   rgSCHUtlContResUlGrant(cell, ue, err);

   if (raCb->phr.pres == TRUE)
   {
      rgSCHUtlUpdPhr(cell, ue, raCb->phr.val, err);
   }
   /* No need of raCb any more */
   rgSCHRamDelRaCb(cell, raCb, TRUE);

   return ROK;
}  /* rgSCHRamContResCrnti */


/**
 * @brief Handler for CCCH SDU based contention resolution
 *
 * @details
 *
 *     Function : rgSCHRamContResCcchsdu
 *     
 *     This function shall be invoked once Msg3 indicates contention resolution
 *     based on CCCH sdu. This shall update the raCb state to 
 *     RGSCH_RA_MSG4_PENDING.
 *     
 *           
 *  @param[in,out] RgSchRaCb *raCb 
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHRamContResCcchsdu
(
RgSchCellCb *cell,
RgSchRaCb *raCb
)
#else
PRIVATE S16 rgSCHRamContResCcchsdu(cell, raCb)
RgSchCellCb *cell;
RgSchRaCb *raCb;
#endif
{
#ifdef RGR_V1 
   CmLteTimingInfo expTime = {0}; 
   RgSchCmnCell  *cellSch  = RG_SCH_CMN_GET_CELL(cell);
#endif
   TRC2(rgSCHRamContResCcchsdu)
   if(raCb->raState != RGSCH_RA_MSG3_PENDING)
   {
      RLOG_ARG2(L_DEBUG,DBG_CELLID,cell->cellId,
               "RNTI:%d RaCb in wrong State %d Drop Msg 3",
               raCb->rntiLnk->rnti, 
               raCb->raState);
      return ROK;
   }

   raCb->raState = RGSCH_RA_MSG4_PENDING;

#ifdef RGR_V1 
   if(cell->rachCfg.contResTmr - cellSch->dl.msg4TxDelay > 0)
      {
      /* Set the contension resolution guard timer = 
         Cont Res Timer - Max msg4 Tx Delay */
         RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, expTime,
            (cell->rachCfg.contResTmr - cellSch->dl.msg4TxDelay));
      }
      else
      {
      /* Schedule the CRI CE in the next Sf itself */
         RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, expTime, 1);
      }
      raCb->expiryTime = expTime;
      raCb->contResTmrLnk.node = (PTR)(raCb);
      cmLListAdd2Tail(&(cell->contResGrdTmrLst), &(raCb->contResTmrLnk));
#endif
   return ROK;
}  /* rgSCHRamContResCcchsdu */


/**
 * @brief Handler for Msg3
 *
 * @details
 *
 *     Function : rgSCHRamProcMsg3
 *     
 *     This function processes the received Msg3 and identifies the type of 
 *     contention resolution and act accordingly. 
 *     
 *           
 *  @param[in,out]  RgSchCellCb     *cell
 *  @param[in,out]  RgSchUeCb       *ue
 *  @param[in,out] RgSchRaCb        *raCb 
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
S16 rgSCHRamProcMsg3
(
RgSchCellCb     *cell,
RgSchUeCb       *ue,
RgSchRaCb       *raCb,
RgInfUeDatInd   *pdu,
RgSchErrInfo    *err
)
#else
S16 rgSCHRamProcMsg3(cell, ue, raCb, pdu, err)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
RgSchRaCb       *raCb;
RgInfUeDatInd   *pdu;
RgSchErrInfo    *err;
#endif
{
   TRC2(rgSCHRamProcMsg3)


   /* Update raCb with PHR if received along with Msg3 */ 
   if (pdu->ceInfo.bitMask & RGSCH_PHR_CE_PRSNT)
   {
      /* PHR present */
      raCb->phr.pres = TRUE;
      raCb->phr.val = pdu->ceInfo.ces.phr;
   } 
   if (ue)
   {
      rgSCHRamContResCrnti(cell, ue, raCb, err);
   }
   else
   {
#ifdef EMTC_ENABLE
      if(TRUE == raCb->isEmtcRaCb)
      {
         /* starting the emtc Contention resolution timer */
         rgSCHRamEmtcContResCcchsdu(cell,raCb);
      }
      else
#endif
      {
         rgSCHRamContResCcchsdu(cell, raCb);
      }
   } 

   return ROK;
}  /* rgSCHRamProcMsg3 */


/**
 * @brief Handler for Updating Bo received in StaRsp
 *
 * @details
 *
 *     Function : rgSCHRamUpdtBo
 *     
 *     This function shall be invoked by RAM once it receives staRsp on CCCH
 *  
 *  @param[in]     RgSchCellCb       *cell         
 *  @param[in,out] RgSchRaCb         *raCb 
 *  @param[in]     RgRguCmnStaRsp *staRsp
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
S16 rgSCHRamUpdtBo
(
RgSchCellCb       *cell,
RgSchRaCb         *raCb,
RgInfCmnBoRpt     *staRsp
)
#else
S16 rgSCHRamUpdtBo(cell, raCb, staRsp)
RgSchCellCb       *cell;
RgSchRaCb         *raCb;
RgInfCmnBoRpt     *staRsp;
#endif
{
   TRC2(rgSCHRamUpdtBo)

   /* Update Bo in RaCb */
   raCb->dlCcchInfo.bo = (U32)(staRsp->bo);
   /* SR_RACH_STATS : MSG4 WITH CCCH SDU */
   rgNumMsg4WithCCCHSdu++;

   /* add this to the "tobeSchdLst" */
   /* MSG4 Fix  Start */   
   rgSCHRamAddToRaInfoSchdLst(cell, raCb);
   /* MSG4 Fix  End */      
   
   return ROK;
} /* rgSCHRamUpdtBo */

/**
 * @brief Handler for Msg3 Feedback indication
 *
 * @details
 *
 *     Function : rgSCHRamMsg3DatInd
 *     
 *     This function shall be invoked by TOM once the transmission of Msg4 is
 *     ACKed/NACKed.
 *     This shall invoke UHM to set ACK for Msg3 reception.
 *           
 *  @param[in,out] RgSchRaCb *raCb 
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
S16 rgSCHRamMsg3DatInd
(
RgSchRaCb      *raCb
)
#else
S16 rgSCHRamMsg3DatInd(raCb)
RgSchRaCb      *raCb;
#endif
{
   TRC2(rgSCHRamMsg3DatInd)

   /* SR_RACH_STATS : MSG3 ACK*/
   rgNumMsg3CrcPassed++;
   /*ccpu00128820 - MOD - Msg3 alloc double delete issue*/
   rgSCHUhmProcMsg3DatInd(&(raCb->msg3HqProc));

   return ROK;
}  /* rgSCHRamMsg3DatInd */

/**
 * @brief Handler for Msg3 Feedback indication
 *
 * @details
 *
 *     Function : rgSCHRamMsg3FailureInd
 *     
 *     This function shall be invoked by TOM once the transmission of Msg4 is
 *     ACKed/NACKed.
 *     This shall invoke UHM to set ACK for Msg3 reception.
 *           
 *  @param[in,out] RgSchRaCb *raCb 
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
S16 rgSCHRamMsg3FailureInd
(
RgSchRaCb      *raCb
)
#else
S16 rgSCHRamMsg3FailureInd(raCb)
RgSchRaCb      *raCb;
#endif
{
   TRC2(rgSCHRamMsg3FailureInd)

   /*ccpu00128820 - MOD - Msg3 alloc double delete issue*/
   rgSCHUhmProcMsg3Failure(&(raCb->msg3HqProc));

   return ROK;
}  /* rgSCHRamMsg3FailureInd */

/**
 * @brief Handler for Msg4 Feedback indication
 *
 * @details
 *
 *     Function : rgSCHRamMsg4FdbkInd
 *     
 *     This function shall be invoked by TOM once the transmission of Msg4 is
 *     ACKed/NACKed.
 *     This shall invoke UHM to set ACK for Msg3 reception.
 *           
 *  @param[in,out] RgSchRaCb *raCb 
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
S16 rgSCHRamMsg4FdbkInd
(
RgSchRaCb      *raCb
)
#else
S16 rgSCHRamMsg4FdbkInd(raCb)
RgSchRaCb      *raCb;
#endif
{
   TRC2(rgSCHRamMsg4FdbkInd)

   return ROK;
}  /* rgSCHRamMsg4FdbkInd */


/**
 * @brief Handler for Msg4 state updation
 *
 * @details
 *
 *     Function : rgSCHRamMsg4Done
 *     
 *     This function shall be invoked by DHM once the transmission of Msg4 is
 *     done. This shall delete the raCb if there is a valid Ue or if this is to
 *     be deleted. If not this shall update the state of the raCb.
 *     
 *           
 *  @param[in]     RgSchCellCb    *cell
 *  @param[in,out] RgSchRaCb      *raCb 
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
S16 rgSCHRamMsg4Done
(
RgSchCellCb    *cell,
RgSchRaCb      *raCb
)
#else
S16 rgSCHRamMsg4Done(cell, raCb)
RgSchCellCb    *cell;
RgSchRaCb      *raCb;
#endif
{
   TRC2(rgSCHRamMsg4Done)

   RLOG_ARG1(L_DEBUG,DBG_CELLID,cell->cellId,
            "rgSCHRamMsg4Done(): tmpCRNTI = %u",
            raCb->tmpCrnti);

   if(raCb->ue != NULLP) 
   {
      /* Fix : syed Let this funtion decide on releasing
       * hqP than the caller of this function otherwise sometimes it 
       * might lead to incorrec NDI setting. */	   
      rgSCHDhmRlsHqpTb(raCb->dlHqE->msg4Proc, 0, TRUE);
      /* Fix : syed Assign hqEnt to UE only if msg4 is done */
      rgSCHDhmAssgnUeHqEntFrmRaCb(raCb->ue, raCb);
#ifdef EMTC_ENABLE
      if(TRUE == raCb->isEmtcRaCb)
      {
         rgSCHEmtcUtlUpdCmnNb(raCb);
      }
#endif
      /* MS_FIX :Proceed to CCCH scheduling irrespective of
       * MSG4 result */
      if (raCb->ue->dlCcchInfo.bo)
      {
#ifdef EMTC_ENABLE
         /*if CR-ID Ack has been received ,Add emtc Ue to cchSduUeLst*/
         if(TRUE == raCb->isEmtcRaCb)
         {
            rgSCHUtlAddUeToEmtcCcchSduLst(cell, raCb->ue);
         }
         else
#endif
         {
            rgSCHUtlAddUeToCcchSduLst(cell, raCb->ue);
         }
      }
      /* Rnti shall not be released as Ue exists with this rnti */
      rgSCHRamDelRaCb(cell, raCb, FALSE);
   }
   else if(raCb->toDel == TRUE)
   {
#ifdef XEON_SPECIFIC_CHANGES
      CM_LOG_DEBUG(CM_LOG_ID_SCH, "Deleting RacB:%d\n", raCb->tmpCrnti);
#endif
      /* Delete RACB and release RNTI */
      rgSCHRamDelRaCb(cell, raCb, TRUE);
   }
   else
   {
#ifdef XEON_SPECIFIC_CHANGES
      CM_LOG_DEBUG(CM_LOG_ID_SCH, "Releasing Harq of RacB:%d\n", raCb->tmpCrnti);
#endif
      raCb->raState = RGSCH_RA_MSG4_DONE;
      /* Release harq process as final feedback is received for Msg4. In other
       * cases, delRaCb will take care of releasing the harq process */
      printf("=======Harq process released \n"); 
      RLOG_ARG0(L_DEBUG,DBG_CELLID,cell->cellId,
                                         "Harq process released "); 
      rgSCHDhmRlsHqpTb(raCb->dlHqE->msg4Proc, 0, TRUE);
   }

   return ROK;
}  /* rgSCHRamMsg4Done */


/**
 * @brief Handler for deletion
 *
 * @details
 *
 *     Function : rgSCHRamDelRaCb
 *     
 *     This function shall be invoked whenever a raCb needs to be deleted. 
 *    Invoked by both RAM and downlink scheduler 
 *           
 *  @param[in]     RgSchCellCb *cell 
 *  @param[in,out] RgSchRaCb   *raCb
 *  @param[in]     Bool        rlsRnti 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
S16 rgSCHRamDelRaCb
(
RgSchCellCb *cell,
RgSchRaCb   *raCb,
Bool        rlsRnti
)
#else
S16 rgSCHRamDelRaCb(cell, raCb, rlsRnti)
RgSchCellCb *cell;
RgSchRaCb   *raCb;
Bool        rlsRnti;
#endif
{
   Inst         inst = cell->instIdx;
   Bool         isEmtc = FALSE;
   TRC2(rgSCHRamDelRaCb)
 
    /* Delete from all the lists it is enqueued */
    cmLListDelFrm(&(cell->raInfo.raCbLst),&(raCb->raCbLnk));
#ifdef EMTC_ENABLE
    /*ue Type is EMTC, then Delete the toBeSchedLst and stop the Guard Timer */
   if(TRUE == raCb->isEmtcRaCb)
   {
      rgSCHRamEmtcDelRaCb(cell,raCb);
      isEmtc = TRUE;
   }
    else
#endif
    {
      if (raCb->schdLnk.node == (PTR)raCb)
      {
         rgSCHRamRmvFrmRaInfoSchdLst(cell, raCb);
      }
#ifdef RGR_V1
      else if(raCb->contResTmrLnk.node != NULLP)
      {
         cmLListDelFrm(&cell->contResGrdTmrLst, &(raCb->contResTmrLnk));
         raCb->contResTmrLnk.node = NULLP;
      }
#endif
    }

   if(rlsRnti == TRUE)
   {
      rgSCHUtlRlsRnti(cell, raCb->rntiLnk, FALSE, 0);
   }

   /* Check if msg4 Hq Proc has been released. If not, release it */
   if (raCb->dlHqE )
   {
      if (raCb->dlHqE->msg4Proc != NULLP)
      {
         /* Fix: syed Remove the msg4Proc if it waiting in sf->tbs list for
	  * harq feedback */	      
         if ((raCb->dlHqE->msg4Proc->subFrm != NULLP) &&
             (raCb->dlHqE->msg4Proc->hqPSfLnk.node != NULLP))
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"TMP CRNTI:%d RACH FAILURE!! "
               "msg4proc  removed from SF", raCb->tmpCrnti);
            rgSCHUtlDlHqPTbRmvFrmTx(raCb->dlHqE->msg4Proc->subFrm, 
			            raCb->dlHqE->msg4Proc, 0, FALSE);
         }	      
         /* Fix: syed Remove the msg4Proc from cell
	  * msg4Retx Queue. I have used CMN scheduler function
	  * directly. Please define a new API and call this
	  * function through that. */	      
         rgSCHCmnDlMsg4ProcRmvFrmRetx(cell, raCb->dlHqE->msg4Proc);	      
         rgSCHDhmRlsHqpTb(raCb->dlHqE->msg4Proc, 0, TRUE);
      }

      /* Mark the raCb pointer in dlHqE to NULLP */
      raCb->dlHqE->raCb = NULLP;

      rgSCHDhmDelHqEnt(cell, &raCb->dlHqE);
   }
   /* Fix: syed Adaptive Msg3 Retx crash. Remove the harqProc
    * from adaptive retx List. Free the alloc if it exists. */
   if (raCb->msg3HqProc.reTxLnk.node)
   {
      //TODO_SID: Need to take care of retxLst
      //cmLListDelFrm(raCb->msg3HqProc.reTxAlloc.reTxLst, &raCb->msg3HqProc.reTxLnk); 
      raCb->msg3HqProc.reTxLnk.node = (PTR)NULLP;
   }

   if (raCb->msg3HqProc.alloc)
   {
      /* Fix: syed During GPR, please write an API instead of direct
       * call to cmn scheduler function */	   
      RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
      /*ccpu00130356 - MOD- To avoid segmentation problem because of double
       free due to recursive calling of rgSCHRamDelRaCb*/
      rgSCHRamUlFreeAllocation(&cellUl->ulSfArr[raCb->msg3HqProc.ulSfIdx],
                                                   raCb->msg3HqProc.alloc,
                                                   cell,isEmtc);
   }

#ifdef EMTC_ENABLE
   if(TRUE == raCb->isEmtcRaCb)
   {
      rgSCHEmtcRaInfoFree(cell, raCb);
   }
#endif
   rgSCHUtlFreeSBuf(inst, (Data **)&raCb, sizeof(RgSchRaCb));

   return ROK;
}  /* rgSCHRamDelRaCb */


/**
 * @brief  TTI Handler for RAM module
 *
 * @details
 *
 *     Function : rgSCHRamTtiHndlr
 *     
 *     This function shall be invoked upon TtiInd by TOM
 *     This shall
 *         - remove RaReqs added to the queue for a raRnti for which PHY may
 *           give the requests in the next subframe
 *         - remove raCbs which are not yet processed once the 
 *           counter for raCb processing expires.
 *     
 *           
 *  @param[in,out] RgSchCellCb  *cell
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
S16 rgSCHRamTtiHndlr
(
RgSchCellCb  *cell
)
#else
S16 rgSCHRamTtiHndlr(cell)
RgSchCellCb  *cell;
#endif
{
   RgSchRaCb   *raCb;
   U16         raSfn;
   U16         crntSfn;
   U16         dist;       /* Number of frames between raCb's creation and crnt
                              frame */
   U8          idx;
   U32         maxCnt;
#ifndef LTE_TDD
   U8          winGap;        
   U8          raIdx;
   RgSchRaReqInfo *raReqInfo;
#else
   CmLteTimingInfo      frm;
   U8                   raIdx;
#endif

   TRC2(rgSCHRamTtiHndlr)

   crntSfn = cell->crntTime.sfn;
  
#ifdef RGR_V1
   /*Check if Contention resolution guard timer expiring in the TTI*/
   rgSCHChkContResGrdTmrExp(cell);
   /*Check if Contention resolution timer expiring in the TTI*/
   rgSCHChkContResTmrExp(cell);
#ifdef EMTC_ENABLE
      /*Check if EMTC Contention resolution guard timer expiring in the TTI*/
      rgSCHChkEmtcContResGrdTmrExp(cell);
      /*Check if EMTC Contention resolution timer expiring in the TTI*/
      rgSCHChkEmtcContResTmrExp(cell);
#endif
#endif
#ifndef LTE_TDD

   /* Delete the RA requests for which RAR window expired in this subframe 
    * And were not considered for RAR scheduling*/
   winGap = (rgRaPrmblToRaFrmTbl[cell->rachCfg.preambleFormat]-1)+ 
      (cell->rachCfg.raWinSize -1 ) + RGSCH_RARSP_WAIT_PERIOD;   
 
   raIdx = (((crntSfn & 1) * RGSCH_MAX_RA_RNTI+ cell->crntTime.slot 
            + RG_SCH_CMN_DL_DELTA - winGap)+ RGSCH_RAREQ_ARRAY_SIZE ) 
           % RGSCH_RAREQ_ARRAY_SIZE;

   /* Flush the already existing raReqs against the given raRnti */

   maxCnt = cell->raInfo.raReqLst[raIdx].count;
   for (idx = 0; idx < maxCnt; idx++)
   {
      raReqInfo = (RgSchRaReqInfo *)(cell->raInfo.raReqLst[raIdx].first->node);
      cmLListDelFrm(&(cell->raInfo.raReqLst[raIdx]),&(raReqInfo->raReqLstEnt));
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)&raReqInfo,
            sizeof(RgSchRaReqInfo));
   }
#else
   /* Fixes for RACH handling: Added deletion of queued RaReq */
   frm   = cell->crntTime;
   RGSCH_INCR_SUB_FRAME(frm, RG_SCH_CMN_DL_DELTA);
   if(rgSchTddUlDlSubfrmTbl[cell->ulDlCfgIdx][frm.slot] !=
                     RG_SCH_TDD_UL_SUBFRAME)
   {
      raIdx = rgSchTddNumDlSubfrmTbl[cell->ulDlCfgIdx][frm.slot]-1;
      rgSCHRamDelRaReq(cell, cell->crntTime, raIdx);
   }
#endif
   
   /* Remove the RACBs which are timed out */
   /* ccpu00132536:MOD- racb timeout will be verified in each SFN such that 
    * the RACB whose processing is not completed in RG_MAX_RA_PRC_FRM
    * will be deleted*/
   if (cell->crntTime.slot == 0)
   {
      maxCnt = cell->raInfo.raCbLst.count;
      for (idx = 0; idx < maxCnt; idx++)
      {
         raCb = (RgSchRaCb *)(cell->raInfo.raCbLst.first->node);
         /* Calculate number of frames between raCb's creation and crnt frame */
         raSfn = raCb->timingInfo.sfn;
         dist = (crntSfn + (RGSCH_MAX_SFN - raSfn)) % RGSCH_MAX_SFN;
         /* Delete RaCbs whose processing is not complete within 
          * "cell->t300TmrVal" frames */
          /* raCb not to be deleted if msg4 is not completed */
          /* raCb should not be deleted(RNTI should not be released) if UE is present
	   * as it means the application still holds the RNTI. raCb will get deleted
	   * as part of UE deletion. raCb will anyway get deleted without releasing RNTI on success/failure of MSG4*/
	 
         if (dist >= cell->t300TmrVal) 
	 {
            if ((raCb->dlHqE->msg4Proc == NULLP) && (raCb->dlHqE->ue == NULLP))
            {
               rgSCHRamDelRaCb(cell, raCb, TRUE);
            }
	 }
         else
         {
            break;
         }
      }
   }
   
   return ROK; 
}  /* rgSCHRamTtiHndlr */


/**
 * @brief Function for handling cell delete
 *
 * @details
 *
 *     Function : rgSCHRamFreeCell
 *     
 *     This function shall be invoked whenever a cell needs to be deleted.
 *     This shall remove raCbs and raReqs stored in cell.
 *     
 *           
 *  @param[in,out] RgSchCellCb  *cell
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
S16 rgSCHRamFreeCell
(
RgSchCellCb    *cell
)
#else
S16 rgSCHRamFreeCell(cell)
RgSchCellCb    *cell;
#endif
{
   RgSchRaReqInfo  *raReqInfo;
   RgSchRaCb       *raCb;
   U8              idx;
   U8              raCbCnt;
   Inst            inst = cell->instIdx;
   U8              lstSz;
#ifdef LTE_TDD
   U8              maxUlSubframes;
   U8              maxDlSubframes;
#endif


   TRC2(rgSCHRamFreeCell)


#ifdef LTE_TDD
   maxUlSubframes =
      rgSchTddNumUlSubfrmTbl[cell->ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];
   maxDlSubframes =
      rgSchTddNumDlSubfrmTbl[cell->ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];
   lstSz = cell->raInfo.maxRaSize * RGSCH_MAX_RA_RNTI_PER_SUBFRM * \
                                                      maxUlSubframes;
#else
   /* ccpu00133557- MEM LEAK FIX- Need to free all the nodes in RA Array list */
   lstSz = RGSCH_RAREQ_ARRAY_SIZE;
#endif

   for (idx = 0; idx < lstSz; idx++)
   {
      /* Delete and free raReqs stored */
      /* ccpu00133557- MEM LEAK FIX- Need to be freed till the count is non-zero */
      while(cell->raInfo.raReqLst[idx].count)
      {
         raReqInfo = (RgSchRaReqInfo *)(cell->raInfo.raReqLst[idx].first->node);
         cmLListDelFrm(&(cell->raInfo.raReqLst[idx]),&(raReqInfo->raReqLstEnt));
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(inst, (Data **)&raReqInfo, sizeof(RgSchRaReqInfo));
      }
   }

#ifdef LTE_TDD
   /* Delete the RACH response list*/
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(inst, 
         (Data **)(&(cell->rachRspLst)), sizeof(RgSchTddRachRspLst) * \
                                    maxDlSubframes);
#endif

   /* Delete raCbs in the "to be scheduled" list */
   /* ccpu00133557- MEM LEAK FIX- Need to be freed till the count is non-zero */
   while(cell->raInfo.toBeSchdLst.count)
   {
      raCb = (RgSchRaCb *)(cell->raInfo.toBeSchdLst.first->node);
      /* MSG4 Fix Start */
	  	  
      rgSCHRamRmvFrmRaInfoSchdLst(cell, raCb);
      /* MSG4 Fix End */	  
   }
#ifdef EMTC_ENABLE
   /* Delete raCbs in the "Emtc to be scheduled" list */
      if(cell->emtcEnable)
      {
          rgSCHRamRmvAllFrmEmtcRaInfoSchdLst(cell);
      }
#endif

   raCbCnt =  cell->raInfo.raCbLst.count;

   /* Delete and free raCbs stored */ 
   for (idx = 0; idx < raCbCnt; idx++)
   {
      raCb = (RgSchRaCb *)(cell->raInfo.raCbLst.first->node);
      rgSCHRamDelRaCb(cell, raCb, TRUE);
   }

   return ROK; 
} /* rgSCHRamFreeCell */
#ifdef RGR_V1
#ifdef ANSI
PRIVATE Void rgSCHRamProcContResExp
(
RgSchCellCb *cell,
RgSchRaCb  *raCb
)
#else
PRIVATE Void rgSCHRamProcContResExp (cell, raCb)
RgSchCellCb *cell;
RgSchRaCb  *raCb;
#endif
{
   TRC2(rgSCHRamProcContResExp);
   raCb->expiryTime.sfn = RGSCH_CONTRES_EXP;
   /*MSG4 Fix*/
   if (raCb->ue)
   {
      /* UE exists and RNTI will be released as part of UE DEL */	   
      rgSCHRamDelRaCb(cell, raCb, FALSE);
   }
   else
   {
      /* Calling Release RNTI, which would perform Racb deletion
       * RNTI removal and RNTI release indication to MAC. */	   
      /* Delete RACB and release RNTI */
      rgSCHRamDelRaCb(cell, raCb, TRUE);
   }
   RETVOID;
}

#ifdef ANSI
PRIVATE Void rgSCHRamProcContResGrdExp
(
RgSchCellCb *cell,
RgSchRaCb  *raCb
)
#else
PRIVATE Void rgSCHRamProcContResGrdExp (cell, raCb)
RgSchCellCb  *cell;
RgSchRaCb  *raCb;
#endif
{
   TRC2(rgSCHRamProcContResGrdExp)


/*Guard timer has expired, schedule only the contention REsolution CE with 
 * zero bo*/
   raCb->dlCcchInfo.bo = 0; 
   /* SR_RACH_STATS : MSG4 WO CCCH SDU */
   rgNumMsg4WoCCCHSdu++;

   /* add this to the "tobeSchdLst" */
   raCb->schdLnk.node = (PTR)(raCb);
   
   cmLListDelFrm(&cell->contResGrdTmrLst, &(raCb->contResTmrLnk));
   raCb->contResTmrLnk.node = NULLP;

   /* MSG4 Fix Start */
   RLOG_ARG1(L_DEBUG,DBG_CELLID,cell->cellId,
            "Con Res Grd Tmr exp RNTI:%d", 
            raCb->rntiLnk->rnti);   
   rgSCHRamAddToRaInfoSchdLst(cell, raCb);
   /* MSG4 Fix End */    
   RETVOID;
   
}
/**
 * @brief Check the Contention Resolution Guard Timer Expiry. 
 *
 * @details
 *
 *     Function: rgSCHChkContResTmrExp 
 *
 *
 *     Invoked by: Scheduler 
 *  @param[in] RgSchCellCb        *cell 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE Void rgSCHChkContResTmrExp
(
RgSchCellCb        *cell
)
#else
PRIVATE Void rgSCHChkContResTmrExp(cell)
RgSchCellCb        *cell;
#endif
{
   CmLList         *chkLnk    = NULLP;
   RgSchRaCb       *raCb = NULLP;
   
   TRC2(rgSCHChkContResTmrExp)
      
   chkLnk = cmLListFirst(&(cell->contResTmrLst));
   
   for (; chkLnk; chkLnk = chkLnk->next)
   {
      raCb = (RgSchRaCb *)(chkLnk->node);
      
      if(RGSCH_TIMEINFO_SAME(raCb->expiryTime, cell->crntTime))
      {
         /*If timer expired, call the handler function*/
         rgSCHRamProcContResExp(cell, raCb);
      }
	  /*Fix: Need to traverse till end of list as the entries may not be in ascending order*/
   /*   else
      {
    break;
      }*/
   }
}
/**
 * @brief Check the Contention Resolution Guard Timer Expiry. 
 *
 * @details
 *
 *     Function: rgSCHChkContResGrdTmrExp 
 *
 *
 *     Invoked by: Scheduler 
 *  @param[in] RgSchCellCb        *cell 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE Void rgSCHChkContResGrdTmrExp
(
RgSchCellCb        *cell
)
#else
PRIVATE Void rgSCHChkContResGrdTmrExp(cell)
RgSchCellCb        *cell;
#endif
{
   CmLList         *chkLnk    = NULLP;
   RgSchRaCb       *raCb = NULLP;
   
   TRC2(rgSCHChkContResGrdTmrExp)
      
   chkLnk = cmLListFirst(&(cell->contResGrdTmrLst));
   
   /*[ccpu00131941]-MOD-List traversal should be done using the listCp */
   for (; chkLnk; chkLnk = cmLListNext(&cell->contResGrdTmrLst))
   {
      raCb = (RgSchRaCb *)(chkLnk->node);
      
      if(RGSCH_TIMEINFO_SAME(raCb->expiryTime, cell->crntTime))
      {
    /*If timer expired, call the handler function*/
    rgSCHRamProcContResGrdExp(cell, raCb);
      }
      else
      {
    break;
      }
   }
}
#endif
#ifdef LTE_TDD
/**
 * @brief Function for handling RACH Request deletion
 *
 * @details
 *
 *     Function : rgSCHRamDelRaReq
 *
 *     This function shall be invoked to delete the RACH Requests
 *     that is not scheduled within the RA window size.
 *
 *
 *  @param[in,out] RgSchCellCb      *cell
 *  @param[in]     CmLteTimingInfo  timingInfo
 *  @param[in]     U8               raIdx
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
S16 rgSCHRamDelRaReq
(
RgSchCellCb          *cell,
CmLteTimingInfo      timingInfo,
U8                   raIdx
)
#else
S16 rgSCHRamDelRaReq(cell, timingInfo, raIdx)
RgSchCellCb          *cell;
CmLteTimingInfo      timingInfo;
U8                   raIdx;
#endif
{
   U8                   subfrmIdx;
   RgSchTddRachRspLst   *rachRsp;
   U16                   sfnIdx;
   S16                  calcSfn;
   U8                   subfrm;
   RgSchRaReqInfo       *raReqInfo;
   U8                   idx;
   U8                   i;
   U8                   raRntiIdx;
   CmLteRnti            raRnti;

   TRC2(rgSCHRamDelRaReq)


   rachRsp = &cell->rachRspLst[raIdx];
   /* Get the SFN Index to be deleted */
   calcSfn = timingInfo.sfn - rachRsp->delInfo.sfnOffset;
   if(calcSfn < 0)
   {
      sfnIdx = (calcSfn + RGSCH_MAX_SFN) % cell->raInfo.maxRaSize;
   }
   else
   {
      sfnIdx = calcSfn;
   }

   /* Iterate through all the subframes to be delted in the SFN */
   for(subfrmIdx=0; subfrmIdx < rachRsp->delInfo.numSubfrms; subfrmIdx++)
   {
      subfrm = rachRsp->delInfo.subframe[subfrmIdx];
      /* Get the subframe Index to be deleted */
      /* Fixes for RACH handling in TDD: 
       * Corrected the computation of raRntiIdx
       * */
      raRntiIdx = ((sfnIdx % cell->raInfo.maxRaSize) * \
                     RGSCH_MAX_RA_RNTI_PER_SUBFRM * \
                     RGSCH_NUM_SUB_FRAMES) + subfrm;

      /* Iterate through all the RNTIs in the subframe */
      for(i=0; i < RGSCH_MAX_RA_RNTI_PER_SUBFRM; i++)
      {
         raRnti = raRntiIdx + (i*RGSCH_NUM_SUB_FRAMES);
         for (idx = 0; idx < cell->raInfo.raReqLst[raRnti].count; idx++)
         {
            raReqInfo = 
               (RgSchRaReqInfo *)(cell->raInfo.raReqLst[raRnti].first->node);
            cmLListDelFrm(&(cell->raInfo.raReqLst[raRnti]),
                                    &(raReqInfo->raReqLstEnt));
            /* ccpu00117052 - MOD - Passing double pointer
            for proper NULLP assignment*/
            rgSCHUtlFreeSBuf(cell->instIdx,
                              (Data **)&raReqInfo, sizeof(RgSchRaReqInfo));
         }
      }
   }

   return ROK;
}
#endif

/*MSG4 Fix Start */
#ifdef ANSI
S16 rgSCHRamAddToRaInfoSchdLst
(
RgSchCellCb       *cell,
RgSchRaCb         *raCb
)
#else
S16 rgSCHRamAddToRaInfoSchdLst(cell, raCb)
RgSchCellCb       *cell;
RgSchRaCb         *raCb;
#endif
{
   CmLteTimingInfo expTime ={0};
   RgSchCmnCell    *cellSch =  RG_SCH_CMN_GET_CELL(cell);

   TRC2(rgSCHRamAddToRaInfoSchdLst)

   /*Fix: This can be called even when guard timer is not expired. 
       * In this case CR timer expiry should be guard timer expiry time + Guard timer time*/
   RG_SCH_ADD_TO_CRNT_TIME(raCb->expiryTime, expTime, cellSch->dl.msg4TxDelay);
   raCb->expiryTime = expTime;
   raCb->schdLnk.node = (PTR)(raCb);
   cmLListAdd2Tail(&(cell->raInfo.toBeSchdLst), &(raCb->schdLnk));
   raCb->contResTmrLnk.node = (PTR)(raCb);
   cmLListAdd2Tail(&(cell->contResTmrLst), &(raCb->contResTmrLnk));
   return ROK;
} /* rgSCHRamAddToRaInfoSchdLst */



#ifdef ANSI
S16 rgSCHRamRmvFrmRaInfoSchdLst
(
RgSchCellCb       *cell,
RgSchRaCb         *raCb
)
#else
S16 rgSCHRamRmvFrmRaInfoSchdLst(cell, raCb)
RgSchCellCb       *cell;
RgSchRaCb         *raCb;
#endif
{
   TRC2(rgSCHRamRmvFrmRaInfoSchdLst)

   cmLListDelFrm(&(cell->raInfo.toBeSchdLst), &(raCb->schdLnk));
   raCb->schdLnk.node = NULLP;   
   cmLListDelFrm(&(cell->contResTmrLst), &(raCb->contResTmrLnk));
   raCb->contResTmrLnk.node = NULLP;
   return ROK;
} /* rgSCHRamRmvFrmRaInfoSchdLst */

/*MSG4 Fix End*/

/***********************************************************
 *
 *     Func : rgSCHRamUlFreeAllocation
 *
 *     Desc : Free an allocation - invokes UHM and releases
 *            alloc
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHRamUlFreeAllocation
(
RgSchUlSf       *sf,
RgSchUlAlloc    *alloc,
RgSchCellCb     *cell,
Bool            isEmtc

)
#else
PRIVATE Void rgSCHRamUlFreeAllocation(sf, alloc, cell,isEmtc)
RgSchUlSf       *sf;
RgSchUlAlloc    *alloc;
RgSchCellCb     *cell;
Bool            isEmtc;
#endif
{
   TRC2(rgSCHRamUlFreeAllocation);

   rgSCHUhmFreeProc(alloc->hqProc, cell);
   if(!isEmtc)
   {
      rgSCHUtlUlAllocRls(sf, alloc);
   }
   RETVOID;
}

/**********************************************************************
 
         End of file
**********************************************************************/
