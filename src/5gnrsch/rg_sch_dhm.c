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
  
     File:     rg_sch_dhm.c
  
**********************************************************************/

/** @file rg_sch_dhm.c
@brief APIs related to Downlink HARQ for the scheduler.
*/


/* header include files -- defines (.h) */
#include "common_def.h"
#include "lrg.h"
#include "rgr.h"
#include "rgm.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_err.h"
#include "rg_sch_inf.h"         /* typedefs for Scheduler */
#include "rg_sch.h"
#include "rg_sch_cmn.h"

/* header/extern include files (.x) */
#include "lrg.x"
#include "rgr.x"
#include "rgm.x"
#include "tfu.x"
#include "rg_sch_inf.x"         /* typedefs for Scheduler */
#include "rg_sch.x"
#include "rg_sch_cmn.x"

#ifdef RGSCH_SPS_STATS
uint32_t rgNumSPSSchedDropMaxRetx;
uint32_t rgNumActDtx;
#endif
uint32_t  nackSf[10];



#ifdef MAC_SCH_STATS
RgSchNackAckStats hqFailStats;
RgSchHqRetxStats  hqRetxStats;
#endif /* MAC_SCH_STATS */
//Chandan Stats Collection
#ifdef DLHQ_STATS
uint32_t statsCnt;
RgSchDlHqStats dlHqStats[10000] = {{0,0,0}};
#endif

#ifdef TFU_TDD
/* For special bundling case: convert numOfAcks to ACK/NACK
 * The below table derives the HARQ aknowledgement as ACK or NACK using the 
 * number of transmission done and the feedback value received
 * The below table is based on Table 7.3-X from 36.213 and 
 * table 79 from FAPIv1.1 doc
 */
uint8_t rgSchNumOfAcksToAckNack[RG_SCH_MAX_NUM_EXPECTED_ACKS][RG_SCH_NUM_FDBK_VALUE] = {
{TFU_HQFDB_ACK,     TFU_HQFDB_NACK,    TFU_HQFDB_NACK},
{TFU_HQFDB_NACK,    TFU_HQFDB_ACK,     TFU_HQFDB_NACK},
{TFU_HQFDB_NACK,    TFU_HQFDB_NACK,    TFU_HQFDB_ACK},
{TFU_HQFDB_ACK,     TFU_HQFDB_NACK,    TFU_HQFDB_NACK},
{TFU_HQFDB_NACK,    TFU_HQFDB_ACK,     TFU_HQFDB_NACK},
{TFU_HQFDB_NACK,    TFU_HQFDB_NACK,    TFU_HQFDB_ACK},
{TFU_HQFDB_ACK,     TFU_HQFDB_NACK,    TFU_HQFDB_NACK},
{TFU_HQFDB_NACK,    TFU_HQFDB_ACK,     TFU_HQFDB_NACK},
{TFU_HQFDB_NACK,    TFU_HQFDB_NACK,    TFU_HQFDB_ACK}
};
#endif

/* local typedefs */
 
/* local externs */

static Void rgSCHDhmFdbkIndHndlTa ARGS((RgSchDlHqProcCb *hqP, uint8_t tbIdx, uint8_t fdbk,
            Bool maxHqRetxReached));
void rgEmtcsetNullSubFrm ARGS((RgSchDlHqProcCb *hqP));
#ifndef LTE_TDD
static S16 rgSCHDhmProcHqFdbkAckNackRep ARGS((
RgSchDlHqProcCb      *hqP,
RgSchDlSf            *sf,
uint8_t                   tbCnt,
uint8_t                   *isAck
));
#endif
#ifdef DL_LA
static S16 rgSCHDhmUpdateAckNackHistory ARGS((
        RgSchCellCb             *cell,
        RgSchUeCb               *ueCb,
        uint8_t                      hqfdbk,
        uint8_t                      tbCnt
        ));
#endif
#ifdef LTE_TDD
static Void rgSCHDhmPrcSplBundlFdbk ARGS((
        RgSchCellCb  *cell,
        TfuHqInfo    *fdbk,
        uint8_t            hqCnt
        ));
#ifdef LTE_ADV
static Void rgSchGetHqFdbkPosForM1 ARGS((
        RgSchUeCb            *ue,
        RgSchDlHqProcCb      *hqP,
        uint8_t                  *isAck, 
        RgTfuHqInfo          *fdbk,
        uint8_t                   tbIdx,
        RgSchTddANInfo       *anInfo
        ));
static Void rgSchGetHqFdbkPosForM234 ARGS((
        RgSchUeCb            *ue,
        RgSchDlHqProcCb      *hqP,
        uint8_t                   *isAck, 
        RgTfuHqInfo          *fdbk,
        uint8_t                   tbIdx,
        RgSchTddANInfo       *anInfo,
        uint8_t                    M,
        CmLteTimingInfo      timeInfo
        ));
#endif/*LTE_ADV*/
#endif/*LTE_TDD*/

/* Freeing up the HARQ proc blocked for
 * indefinite time in case of Retx */
S16 rgSCHDhmDlRetxAllocFail ARGS((
RgSchUeCb        *ue, 
RgSchDlHqProcCb  *proc
));

#ifdef EMTC_ENABLE
S16 rgSCHDhmEmtcRgrCellCfg ARGS((
RgSchCellCb   *cell
));
#endif

#ifdef CA_DBG
uint32_t gPCellTb1AckCount,gPCellTb2AckCount,gPCellTb1NackCount,gPCellTb2NackCount;
uint32_t gSCellSchedCount,gPrimarySchedCount;
uint32_t gSCellTb1AckCount,gSCellTb2AckCount,gSCellTb1NackCount,gSCellTb2NackCount;
uint32_t gPCellTb1DtxCount, gPCellTb2DtxCount, gSCellTb1DtxCount, gSCellTb2DtxCount;
uint32_t gHqFdbkCount;

#endif
#ifdef EMTC_ENABLE
Void rgSCHEmtcUtlDlHqPTbRmvFrmTx
(
RgSchEmtcDlSf               *subFrm,
RgSchDlHqProcCb            *hqP,
uint8_t                         tbIdx,
Bool                       isRepeting
);
RgSchEmtcDlSf* rgSCHEmtcUtlSubFrmGet
(
RgSchCellCb            *cell,
CmLteTimingInfo        frm
);
Void rgSCHEmtcHqInfoAlloc ARGS((RgSchCellCb *cell, RgSchDlHqProcCb *hqP));
#endif
/* forward references */

/**
 * @brief This function initializes the DL HARQ Entity of UE.
 *
 * @details
 *
 *     Function: rgSCHDhmHqEntInit
 *     Purpose:  This function initializes the DL HARQ entity of 
 *               UE control block. This is performed at the time
 *               of creating UE control block.
 *     
 *     Invoked by: configuration module
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @return  RgSchDlHqEnt *
 *
 **/
/*MS_WORKAROUND for ccpu00122893*/
Void rgSCHDhmHqEntReset(RgSchDlHqEnt *hqE)
{
   RgSchDlHqProcCb   *hqP;
   uint8_t                i;
   cmLListInit(&hqE->inUse);     
   cmLListInit(&hqE->free);      
   for (i=0; i < hqE->numHqPrcs; i++)
   {
      hqP              = &hqE->procs[i];
      hqP->hqE         = hqE;
      hqP->procId      = i;
      /* Fix  - reset numLch */
      hqP->tbInfo[0].numLch      = 0;
      hqP->tbInfo[1].numLch      = 0;
      hqP->tbInfo[0].txCntr      = 0;
      hqP->tbInfo[0].ndi         = 0;  /* Initialize the NDI to Zero */
      hqP->tbInfo[1].txCntr      = 0;
      hqP->tbInfo[1].ndi         = 0;  /* Initialize the NDI to Zero */
      hqP->tbInfo[0].tbIdx       = 0;
      hqP->tbInfo[1].tbIdx       = 1;
      hqP->tbInfo[0].hqP         = hqP;
      hqP->tbInfo[1].hqP         = hqP;
      hqP->tbInfo[0].state       = HQ_TB_ACKED;
      hqP->tbInfo[1].state       = HQ_TB_ACKED;
      hqP->tbInfo[0].contResCe   = NOTPRSNT;
      hqP->tbInfo[1].contResCe   = NOTPRSNT;
      hqP->lnk.node = (PTR)hqP;
      //cmLListAdd2Tail(&hqE->free, &hqP->lnk);
      hqP->hqPLst = NULLP;
      rgSCHDhmHqPAdd2FreeLst(hqP);
      hqP->tbInfo[0].lchSchdData = hqP->tbInfo[0].lchSchdDataArr;
      hqP->tbInfo[1].lchSchdData = hqP->tbInfo[1].lchSchdDataArr;
      hqP->drxCb.rttIndx  = DRX_INVALID;
      hqP->drxCb.reTxIndx = DRX_INVALID;
      hqP->tbInfo[0].cntrRetxAllocFail = 0;
      hqP->tbInfo[1].cntrRetxAllocFail = 0;
      hqP->hasDcch = FALSE;
      hqP->cwSwpEnabled = FALSE;
      hqP->pdcch   = NULLP;
      hqP->subFrm  = NULLP;

#ifdef LTE_ADV
      rgSCHLaaResetDlHqProcCb(hqP);
#endif
   }
   return;
} /* rgSCHDhmHqEntReset */

/**
 * @brief This function assigns dlHqEnt of raCb to ueCb.
 *
 * @details
 *
 *     Function: rgSCHDhmAssgnUeHqEntFrmRaCb 
 *     Purpose:  This function assigns dlHqEnt of raCb to ueCb.
 *     
 *     Invoked by: configuration module
 *     
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  RgSchRaCb       *raCb
 *  @return  Void 
 *
 **/
Void rgSCHDhmAssgnUeHqEntFrmRaCb(RgSchUeCb *ue,RgSchRaCb *raCb)
{

   ue->cellInfo[0]->hqEnt = raCb->dlHqE;
   ue->cellInfo[0]->hqEnt->ue = ue;
   /* Update the DL Harq related information */
   ue->cellInfo[0]->hqEnt->maxHqTx = ue->cell->dlHqCfg.maxDlHqTx;
   raCb->dlHqE = NULLP;
   /* Fix : set UE active in DL as UE initialization completed */
   ue->dl.dlInactvMask &= ~(RG_HQENT_INACTIVE);
   ue->ul.ulInactvMask &= ~(RG_HQENT_INACTIVE);
   rgSCHCmnDlInitHqEnt(ue->cell, ue->cellInfo[0]->hqEnt);
   
   return;
}

/**
 * @brief This function deletes the dlHqEnt.
 *
 * @details
 *
 *     Function: rgSCHDhmDelHqEnt 
 *     Purpose:  This function deletes the dlHqEnt.
 *     
 *     Invoked by: configuration module
 *     
 *  @param[in]  RgSchCellCb       *cell 
 *  @param[in]  RgSchDlHqEnt      **hqE 
 *  @return  Void 
 *
 **/
Void rgSCHDhmDelHqEnt(RgSchCellCb *cell,RgSchDlHqEnt **hqE)
{

   if (!(*hqE))
   {
      return;    
   }

   rgSCHCmnDlDeInitHqEnt(cell, *hqE);

   rgSCHUtlFreeSBuf(cell->instIdx, (Data **)hqE, 
       sizeof(RgSchDlHqEnt));
   
   return;
}

RgSchDlHqEnt *rgSCHDhmHqEntInit(RgSchCellCb *cell)
{
   RgSchDlHqEnt      *hqE;
   Inst              inst = cell->instIdx;

   /* Init the HARQ data structure */
   if (rgSCHUtlAllocSBuf(inst, (Data **)&hqE, sizeof(RgSchDlHqEnt)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH: rgSCHDhmHqEntInit hqE alloc fail"); 
      return (NULLP);
   }
#ifdef LTE_TDD
   /* Init the HARQ processes */
   hqE->numHqPrcs = rgSchTddDlNumHarqProcTbl[cell->ulDlCfgIdx];
   if (rgSCHUtlAllocSBuf(inst, (Data **)&hqE->procs, 
                           hqE->numHqPrcs * sizeof(RgSchDlHqProcCb)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH: rgSCHDhmHqEntInit hqP alloc fail in hqE"); 
      return (NULLP);
   }
#else
   hqE->numHqPrcs = RGSCH_NUM_DL_HQ_PROC;
#endif

#ifdef LTE_ADV
   rgSCHLaaInitDlHqProcCb (cell, hqE);
#endif

   /* Initialize maximum tranmission counter */
   hqE->maxHqTx = cell->dlHqCfg.maxDlHqTx;
   

   /* MW_WORKAROUND for ccpu00122893 */
   rgSCHDhmHqEntReset(hqE); 
   /* CA Dev Start*/
   hqE->cell = cell;
   /* CA Dev End*/

   return (hqE);
} /* rgSCHDhmHqEntInit */

/**
 * @brief This function gets an available HARQ process.
 *
 * @details
 *
 *     Function: rgSCHDhmGetAvlHqProc
 *     Purpose:  This function returns an available HARQ process in 
 *               the DL direction. All HARQ processes are maintained
 *               in queues of free and inuse.
 *
 *               1. Check if the free queue is empty. If yes, return
 *                  RFAILED
 *               2. If not empty, update the proc variable with the
 *                  first process in the queue. Return ROK.
 *     
 *     Invoked by: scheduler
 *     
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]  CmLteTimingInfo  timingInfo
 *  @param[out] RgSchDlHqProc       **hqP
 *  @return  S16       
 *         -#ROK     if successful
 *         -#RFAILED otherwise
 *
 **/
S16 rgSCHDhmGetAvlHqProc(RgSchCellCb *cell,RgSchUeCb *ue,CmLteTimingInfo  timingInfo,RgSchDlHqProcCb **hqP)
{
   RgSchDlHqEnt      *hqE = NULLP;
   RgSchDlHqProcCb   *tmpHqProc;
   CmLList           *tmp;

   hqE      = RG_SCH_CMN_GET_UE_HQE(ue, cell);
 
   if (hqE == NULLP)
   {   
      DU_LOG("\nERROR  -->  SCH : rgSCHDhmGetAvlHqProc hqE NULL ue %d"
                           , ue->ueId);     
      return RFAILED;
   }


   CM_LLIST_FIRST_NODE(&(hqE->free), tmp);

   if (NULLP == tmp)
   {
       DU_LOG("\nERROR  -->  SCH : rgSCHDhmGetAvlHqProc free %d inUse %d ue %d"
                                           , hqE->free.count, hqE->inUse.count, ue->ueId);
      /* No Harq Process available in the free queue. */
      return RFAILED;
   }

   tmpHqProc = (RgSchDlHqProcCb *)(tmp->node);

#ifdef LTEMAC_SPS
   /* If SPS HARQ procs are in use, do not use SPS harq procs for non-SPS
    * transmissions */
   if (ue->dl.isSpsHqPInUse)
   {
      while (tmpHqProc->procId < ue->dl.dlSpsCfg.numSpsHqProc)
      {
         CM_LLIST_NEXT_NODE(&(hqE->free), tmp);
         if (!tmp)
         {
            break;
         }
         tmpHqProc = (RgSchDlHqProcCb *)(tmp->node);
      }
      if (!tmp)
      {
         /* No Harq Process available in the free queue. */
         return RFAILED;
      }
   }
#endif


   tmpHqProc->tbInfo[0].timingInfo = timingInfo;
   tmpHqProc->tbInfo[1].timingInfo = timingInfo;
   tmpHqProc->hasDcch = FALSE;
   tmpHqProc->cwSwpEnabled = FALSE;

   /* Remove the element from the free Queue */
   //cmLListDelFrm(&hqE->free, tmp);
   rgSCHDhmHqPDelFrmFreeLst(tmpHqProc);

   /* Add the element into the inUse Queue as well */
   //cmLListAdd2Tail(&hqE->inUse, &tmpHqProc->lnk);
   rgSCHDhmHqPAdd2InUseLst(tmpHqProc);

   *hqP = tmpHqProc;
   
#ifdef LTE_ADV
   rgSCHLaaResetDlHqProcCb(tmpHqProc);
#endif

   /* LAA DBG Only */
   tmpHqProc->tbSizeAtEstimate[0] = 0;
   tmpHqProc->tbSizeAtEstimate[1] = 0;
   tmpHqProc->tbSizeAtFnlz[0] = 0;
   tmpHqProc->tbSizeAtFnlz[1] = 0;
   tmpHqProc->tbSizeOfMvdTb[0] = 0;
   tmpHqProc->tbSizeOfMvdTb[1] = 0;
   tmpHqProc->itbsAtEstimate[0] = 0;
   tmpHqProc->itbsAtEstimate[1] = 0;
   tmpHqProc->prbAtEstimate = 0;

   return ROK;
} /* rgSCHDhmGetAvlHqProc */


/**
 * @brief This function adds HARQ process for a given TB in to 
 *        the inuse queue upon retx.
 *
 * @details
 *
 *     Function: rgSCHDhmHqTbRetx
 *     Purpose:  This function handles when a HARQ process is scheduled
 *               for retransmission. It adds the HARQ procss to inuse
 *               queue.
 *               1. Check if this HqP is already added to the inUse
 *                  queue as part of this function call for other
 *                  TB's retransmission.
 *               2. If already present in inUse Q then do not add.
 *     
 *     Invoked by: scheduler
 *     
 *  @param[in]  RgSchDlHqEnt     *hqE
 *  @param[in]  CmLteTimingInfo  timingInfo
 *  @param[out] RgDlHqProc       *hqP
 *  @param[in]  uint8_t               tbIdx
 *  @return  Void      
 *
 **/
Void rgSCHDhmHqTbRetx(RgSchDlHqEnt *hqE,CmLteTimingInfo timingInfo,RgSchDlHqProcCb *hqP,uint8_t tbIdx)
{
   uint8_t othrTbIdx = tbIdx ^ 1;

   hqP->tbInfo[tbIdx].timingInfo = timingInfo;

   if (hqE->msg4Proc == hqP)
   {
       return;
   }
   /* fix for ccpu00118633 No Hq proc Avl end*/
      
   /* Extra:check if Harq process is already linked to in-use
      Queue by means of other TB handling. */
   if (hqP->tbInfo[othrTbIdx].state != HQ_TB_WAITING)
   {
      /*Fix FIXME */
      if (hqE->msg4Proc != hqP)
      {
         //cmLListAdd2Tail(&hqE->inUse, &hqP->lnk);
         rgSCHDhmHqPAdd2InUseLst(hqP);
      }
   }

    hqP->tbInfo[tbIdx].cntrRetxAllocFail = 0;

   return;
} /* rgSCHDhmHqTbRetx */

/**
 * @brief This function returns last scheduled HARQ process for
 *        a UE's HARQ entity.
 *
 * @details
 *
 *     Function: rgSCHDhmLastSchedHqProc
 *     Purpose:  This function returns the last (most recent)
 *               process in the inUse list, which corresponds
 *               to the last scheduled process. Returns NULLP
 *               if list is empty.
 *               The reason for introducing this is to have
 *               an ability to check if UE was scheduled
 *               in the current subframe (scheduling would
 *               have caused a proc to be added to the end
 *               of the list, and checking time (only subframe
 *               number probably works) would confirm this.
 *
 *     Invoked by: scheduler
 *     
 *  @param[in]  RgSchDlHqEnt        *hqE
 *  @return  RgSchDlHqProcCb *
 *
 **/
RgSchDlHqProcCb * rgSCHDhmLastSchedHqProc(RgSchDlHqEnt *hqE)
{
   /* GRPPWR_CNTRL Fix: UE context will not hold a valid hqE,
    * until RACH procedure is completed */
   if ((hqE == NULLP) || (hqE->inUse.last == NULLP))
   {
      return (NULLP);
   }
   return ((RgSchDlHqProcCb *)hqE->inUse.last->node);
} /* rgSCHDhmLastSchedHqProc */

#ifdef RGR_V1
/**
 * @brief This function gets an available HARQ process for MSG 4.
 *
 * @details
 *
 *     Function: rgSCHDhmGetCcchSduHqProc
 *     Purpose:  This function returns an available HARQ process in 
 *               the DL direction. All HARQ processes are maintained
 *               in queues of free and inuse.
 *
 *               1. Check if the free queue is empty. If yes, return
 *                  RFAILED.
 *               2. If not empty, update the proc variable with the
 *                  first process in the queue. Return ROK.
 *     
 *     Invoked by: scheduler
 *     
 *  @param[in]  RgSchRaCb           *raCb
 *  @param[in]  CmLteTimingInfo  timingInfo
 *  @param[out] RgSchDlHqProcCb  **hqP
 *  @return  S16       
 *         -#ROK     if successful
 *         -#RFAILED otherwise
 *
 **/
S16 rgSCHDhmGetCcchSduHqProc
(
RgSchUeCb               *ueCb,
CmLteTimingInfo         timingInfo,
RgSchDlHqProcCb         **hqP
)
{
   RgSchDlHqProcCb      *tmpHqProc;
   CmLList              *tmp;
   RgSchDlHqEnt         *hqE;

   hqE = ueCb->cellInfo[0]->hqEnt;
   CM_LLIST_FIRST_NODE(&(hqE->free), tmp);
   if (NULLP == tmp)
   {
      /* No Harq Process available in the free queue. */
      return RFAILED;
   }

   /* Remove the element from the free Queue and */
   /* set the MSG 4 HARQ proc pointer            */
   //cmLListDelFrm(&hqE->free, tmp);

   tmpHqProc             = (RgSchDlHqProcCb *)(tmp->node);

   rgSCHDhmHqPDelFrmFreeLst(tmpHqProc);

   tmpHqProc->tbInfo[0].timingInfo = timingInfo;
   /* Fix : syed minor code reorg */
   *hqP = tmpHqProc;
   /*Updating ccchSduProc to identify feedback for CCCH SDU sent without 
    * Cont Res CE*/
   hqE->ccchSduProc         = tmpHqProc;
   //cmLListAdd2Tail(&hqE->inUse, &tmpHqProc->lnk);
   rgSCHDhmHqPAdd2InUseLst(tmpHqProc);
   
   return ROK;
} /* rgSCHDhmGetCcchSduHqProc */
#endif

/**
 * @brief This function gets an available HARQ process for MSG 4.
 *
 * @details
 *
 *     Function: rgSCHDhmGetMsg4HqProc
 *     Purpose:  This function returns an available HARQ process in 
 *               the DL direction. All HARQ processes are maintained
 *               in queues of free and inuse.
 *
 *               1. Check if the free queue is empty. If yes, return
 *                  RFAILED.
 *               2. If not empty, update the proc variable with the
 *                  first process in the queue. Return ROK.
 *     
 *     Invoked by: scheduler
 *     
 *  @param[in]  RgSchRaCb           *raCb
 *  @param[in]  CmLteTimingInfo  timingInfo
 *  @param[out]  RgDlHqProc       **hqP
 *  @return  S16       
 *         -#ROK     if successful
 *         -#RFAILED otherwise
 *
 **/
S16 rgSCHDhmGetMsg4HqProc(RgSchRaCb *raCb,CmLteTimingInfo timingInfo)
{
   RgSchDlHqProcCb      *tmpHqProc;
   CmLList              *tmp;
   RgSchDlHqEnt         *hqE;

   hqE = raCb->dlHqE;
   CM_LLIST_FIRST_NODE(&(hqE->free), tmp);
   if (NULLP == tmp)
   {
      /* No Harq Process available in the free queue. */
      return RFAILED;
   }

   /* Remove the element from the free Queue and */
   /* set the MSG 4 HARQ proc pointer            */
   //cmLListDelFrm(&hqE->free, tmp);
   tmpHqProc             = (RgSchDlHqProcCb *)(tmp->node);
   rgSCHDhmHqPDelFrmFreeLst(tmpHqProc);
   tmpHqProc->tbInfo[0].timingInfo = timingInfo;
   hqE->msg4Proc         = tmpHqProc;

   return ROK;
} /* rgSCHDhmGetMsg4HqProc */

/**
 * @brief This function releases a HARQ process.
 *
 * @details
 *
 *     Function: rgSCHDhmRlsHqpTb 
 *     Purpose:  This function resets the TB specific fields
 *               Based on the other TBs state, this HqProcess
 *               is returned to the HqEnt.
 *     
 *               1. Add the HARQ process to the free queue.
 *     Invoked by: scheduler and HARQ processing
 *     
 *  @param[in]  RgDlHqProc    *hqP
 *  @param[in]  uint8_t            tbIdx
 *  @param[in]  Bool          togNdi 
 *  @return  Void      
 *
 **/
Void rgSCHDhmRlsHqpTb(RgSchDlHqProcCb *hqP,uint8_t tbIdx,Bool  togNdi) 
{
   RgSchDlHqEnt         *hqE;
   uint8_t              othrTbIdx = tbIdx ^ 1;
#ifdef LTEMAC_SPS
   RgSchCmnDlHqProc     *cmnHqDl;
#endif
   /* L2_COUNTERS */
#ifdef LTE_L2_MEAS
   RgSchDlLcCb*         lcCb = NULLP;
   uint8_t              numLch = 0;
#endif

   /* Reset all tbInfo values */

   hqE = hqP->hqE;

#ifdef MAC_SCH_STATS
   if (hqP->hqE->ue != NULLP)
   {
      RgSchUeCb      *ueCb  = hqP->hqE->ue;
      RgSchCmnUe     *cmnUe = (RgSchCmnUe*)ueCb->sch;
      RgSchCmnDlUe   *dlUe  = RG_SCH_CMN_GET_DL_UE(ueCb,hqE->cell);/*CA dev*/
      uint8_t             cqi    = dlUe->mimoInfo.cwInfo[0].cqi;
      /* to get retransmission, decreasing transmission counter */
      uint16_t            numDlRetx = hqP->tbInfo[0].txCntr-1; 
      uint8_t             tbs       = dlUe->mimoInfo.cwInfo[0].iTbs[0];
      static uint32_t retxCnt1      = 0;

      RG_SCH_CMN_DL_TBS_TO_MCS(tbs, \
            hqRetxStats.dlCqiStat[(cqi - 1)].mcs);

      switch (numDlRetx)
      {
         case 1:
            hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_1++;
            break;
         case 2:
            hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_2++;
            break;
         case 3:
            hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_3++;
            break;
         case 4:
            hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_4++;
            break;
      }
      hqRetxStats.dlCqiStat[(cqi - 1)].totalTx = \
             hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_1 + \
            (hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_2 * 2) + \
            (hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_3 * 3) + \
            (hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_4 * 4); 

      retxCnt1 += numDlRetx;
   }
#endif /* MAC_SCH_STATS */
   RGSCH_ARRAY_BOUND_CHECK(0, hqP->tbInfo, tbIdx);
   /* Toggle ndi */
   if(togNdi == TRUE)
   {
      hqP->tbInfo[tbIdx].ndi          ^= 1;
   }

   /* L2_COUNTERS */
#ifdef LTE_L2_MEAS
   for (numLch =0; numLch < hqP->tbInfo[tbIdx].numLch; numLch++)
   {
      if (NULLP != (lcCb = rgSCHDbmGetDlDedLcCb( hqP->hqE->ue,
                  hqP->tbInfo[tbIdx].lchSchdDataArr[numLch].lcId)))
      {
         if (lcCb->lcType == CM_LTE_LCH_DTCH)
         {
            if (hqP->hqE->ue->qciActiveLCs[lcCb->qciCb->qci])
            {
               hqP->hqE->ue->qciActiveLCs[lcCb->qciCb->qci]--;
            }

            if (!(hqP->hqE->ue->qciActiveLCs[lcCb->qciCb->qci]))
            {
               lcCb->qciCb->dlUeCount--;
            }
         }
      }
   }
#endif

   /* Initialization */
   hqP->tbInfo[tbIdx].tbSz           = 0;
   hqP->tbInfo[tbIdx].numLch         = 0;
   hqP->tbInfo[tbIdx].txCntr         = 0;
   /* FOR ACK NACK REP */
   hqP->tbInfo[tbIdx].fbkRepCntr     = 0;
   hqP->tbInfo[tbIdx].fbkRecpRepCntr = 0;
   hqP->tbInfo[tbIdx].ackCount       = 0;
   /* pdcch is moved from TbCb to HqCb.
      This pdcch will be set to NULL when 
      HqCb will be pushed to free list*/
   hqP->tbInfo[tbIdx].state          = HQ_TB_ACKED;
   hqP->tbInfo[tbIdx].isAckNackDtx   = 0;
   hqP->tbInfo[tbIdx].nackCount      = 0;
   hqP->tbInfo[tbIdx].dtxCount       = 0;
   hqP->tbInfo[tbIdx].schdTa.pres    = NOTPRSNT;
   hqP->tbInfo[tbIdx].contResCe      = NOTPRSNT;
#ifdef LTE_ADV
   hqP->tbInfo[tbIdx].schdSCellActCe.pres = NOTPRSNT;
#endif
   hqP->tbInfo[tbIdx].minRlcReordrTmr = 0;
   /* Handling msg4 hqProc */
   if (hqE->msg4Proc == hqP)
   {
      hqE->msg4Proc = NULLP;
      hqP->pdcch    = NULLP;
      hqP->subFrm   = NULLP;
      /* Add the proc to the free list */
      //cmLListAdd2Tail(&hqE->free, &hqP->lnk);
      rgSCHDhmHqPAdd2FreeLst(hqP);
      return;
   }
#ifdef RGR_V1
   /* MS_WORKAROUND : syed The check (hqE->ccchSduProc != NULLP)
    * is dangerous and it expects ccchSduProc is the first
    * DL allocation for a UE, and considering a scenario
    * of multiple UEs contending and 1 UE per TTI, this 
    * assumption can be wronged, leading to inUse list
    * corruption. Hence altering this check.
    * A better approach would be do avoid having this 
    * special handling for ccchSduProc, streamline 
    * it with the usual approach. */
   if (hqE->ccchSduProc == hqP)
   {
      hqE->ccchSduProc = NULLP;
      /* ccpu00137582- If hqP is in reTxLst then it will be no more available
       * in inUse list, Hence need not to delete from inUse list*/
      if(NULLP == hqP->tbInfo[tbIdx].ccchSchdInfo.retxLnk.node)
      {
         //cmLListDelFrm(&hqE->inUse, &hqP->lnk);
         rgSCHDhmHqPDelFrmInUseLst(hqP);
      }
      else
      {
         hqP->tbInfo[tbIdx].ccchSchdInfo.retxLnk.node = NULLP;
      }
      hqP->pdcch    = NULLP;
      hqP->subFrm   = NULLP;
      /* Add the proc to the free list */
      //cmLListAdd2Tail(&hqE->free, &hqP->lnk);
      rgSCHDhmHqPAdd2FreeLst(hqP);
      return;
   }
#endif
      
   /* extra:check if other TB is also free for allocation then 
    * add it to FREE List */
   switch(hqP->tbInfo[othrTbIdx].state)
   {
      case HQ_TB_ACKED:
         /* Remove the element from the inUse Queue */
         /* Freeing up the HARQ proc blocked for
          * indefinite time in case of Retx */
         if (hqP->tbInfo[tbIdx].cntrRetxAllocFail != RG_SCH_MAX_RETX_ALLOC_FAIL)
         {
            //cmLListDelFrm(&hqE->inUse, &hqP->lnk);
            rgSCHDhmHqPDelFrmInUseLst(hqP);
         }
         hqP->pdcch   = NULLP;
         hqP->subFrm  = NULLP;
#ifdef EMTC_ENABLE
rgEmtcsetNullSubFrm(hqP);
#endif
         /* Add the proc to the free list */
         //cmLListAdd2Tail(&hqE->free, &hqP->lnk);
         rgSCHDhmHqPAdd2FreeLst(hqP);
#ifdef LAA_DBG
         if (hqE->free.count > 8)
         {
            int *p = NULL;
            printf("\nDEBUG  -->  SCH: Crashing invalid hq count after free \n");
            printf("\nDEBUG  -->  SCH: Crashing %d \n", *p);
            *p = 10;
         }
#endif
#ifdef LTEMAC_SPS
         cmnHqDl = RG_SCH_CMN_GET_DL_HQP(hqP);
         if (cmnHqDl) 
         {
            cmnHqDl->spsAction = 0;
            cmnHqDl->isSpsActv = FALSE;
            cmnHqDl->isSpsSvcSchd = FALSE;
         }
#endif
         break;
         case HQ_TB_NACKED:
         /* Remove the element from the inUse Queue */
         /* Freeing up the HARQ proc blocked for
          * indefinite time in case of Retx */
         if (hqP->tbInfo[othrTbIdx].cntrRetxAllocFail == 0)
         {
            //cmLListDelFrm(&hqE->inUse, &hqP->lnk);
            rgSCHDhmHqPDelFrmInUseLst(hqP);
         }
         break;
         case HQ_TB_WAITING:
            /* Do nothing */
         break;
   }
   
   hqP->tbInfo[tbIdx].cntrRetxAllocFail = 0;

   return;
} /* rgSCHDhmRlsHqpTb */

/**
 * @brief This function releases a HARQ process.
 *
 * @details
 *
 *     Function: rgSCHDhmRlsHqProc
 *     Purpose:  This function returns a HARQ process to HARQ Entity 
 *               in the DL direction.
 *     
 *               1. Add the HARQ process to the free queue.
 *     Invoked by: scheduler and HARQ processing
 *     
 *  @param[in]  RgDlHqProc    *hqP
 *  @return  Void      
 *
 **/
Void rgSCHDhmRlsHqProc(RgSchDlHqProcCb *hqP)
{


#ifdef MAC_SCH_STATS
    /* THIS FUNCTION IS NOT CALLED */
   if (hqP->hqE->ue != NULLP)
   {
      RgSchUeCb      *ueCb  = hqP->hqE->ue;
      RgSchCmnUe     *cmnUe = (RgSchCmnUe*)ueCb->sch;
      RgSchCmnDlUe   *dlUe  = RG_SCH_CMN_GET_DL_UE(ueCb,hqE->cell);/*CA dev*/
      uint8_t             cqi    = dlUe->mimoInfo.cwInfo[0].cqi;
      /* to get retransmission, decreasing transmission counter */
      uint16_t            numDlRetx = hqP->tbInfo[0].txCntr-1; 
      uint8_t             tbs       = dlUe->mimoInfo.cwInfo[0].iTbs[0];

      RG_SCH_CMN_DL_TBS_TO_MCS(tbs, hqRetxStats.dlCqiStat[(cqi - 1)].mcs);

      switch (numDlRetx)
      {
         case 1:
            hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_1++;
            break;
         case 2:
            hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_2++;
            break;
         case 3:
            hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_3++;
            break;
         case 4:
            hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_4++;
            break;
      }
      hqRetxStats.dlCqiStat[(cqi - 1)].totalTx = \
             hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_1 + \
            (hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_2 * 2) + \
            (hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_3 * 3) + \
            (hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_4 * 4);
   }
#endif /* MAC_SCH_STATS */
      hqP->pdcch   = NULLP;
      hqP->subFrm  = NULLP;
#ifdef EMTC_ENABLE
rgEmtcsetNullSubFrm(hqP);
#endif
      rgSCHDhmHqPDelFrmInUseLst(hqP);
      rgSCHDhmHqPAdd2FreeLst(hqP);
#ifdef TFU_UPGRADE
   hqP->tbCnt = 0;
#endif

   return;
} /* rgSCHDhmRlsHqProc */

#ifdef LTEMAC_SPS
/**
 * @brief This function gets HARQ process with the given ID.
 *
 * @details
 *
 *     Function: rgSCHDhmGetHqProcFrmId
 *     Purpose:  This function returns the HARQ process with the given ID.
 *     Invoked by: ROM
 *     
 *  @param[in]  RgSchUeCb     *ue
 *  @param[in]  uint8_t            idx
 *  @param[in]  RgDlHqProc    **hqP
 *  @return  S16       
 *         -#   ROK     if successful
 *         -#   RFAILED otherwise
 *
 **/
S16 rgSCHDhmGetHqProcFrmId(RgSchCellCb *cell,RgSchUeCb  *ue,uint8_t idx,RgSchDlHqProcCb **hqP)
{
   RgSchDlHqEnt         *hqE;

   hqE      = RG_SCH_CMN_GET_UE_HQE(ue, cell);
   /* Pick the proc based on the index provided */
   *hqP = &(hqE->procs[idx]);

   return ROK;
} /* rgSCHDhmGetHqProcFrmId */

/**
 * @brief This function gets SPS HARQ process from the given time
 *
 * @details
 *
 *     Function: rgSCHDhmSpsDlGetHqProc
 *     Purpose:  This function returns the SPS HARQ process for the given time
 *     
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  CmLteTimingInfo timingInfo
 *  @return  RgSchDlHqProcCb control block 
 *
 **/
RgSchDlHqProcCb* rgSCHDhmSpsDlGetHqProc(RgSchCellCb *cell,RgSchUeCb *ue,CmLteTimingInfo timingInfo)
{
   RgSchDlHqEnt       *hqE;
   uint8_t            idx;
   RgSchDlHqProcCb    *hqProc = NULLP;
   CmLList            *tmp = NULLP;

   hqE      = RG_SCH_CMN_GET_UE_HQE(ue, cell);

   CM_LLIST_FIRST_NODE(&(hqE->free), tmp);

   if (NULLP == tmp)
   {
      /* No Harq Process available in the free queue. */
      return (NULLP);
   }

   idx = ((timingInfo.sfn * RGSCH_NUM_SUB_FRAMES_5G + timingInfo.slot)/
         ue->dl.dlSpsCfg.dlSpsPrdctyEnum) % ue->dl.dlSpsCfg.numSpsHqProc;


   hqProc = (RgSchDlHqProcCb *)(tmp->node);

   /* If the  HARQ process is in the free list, retrieve the process */
   while (hqProc->procId != idx)
   {
      CM_LLIST_NEXT_NODE(&(hqE->free), tmp);
      if (!tmp)
      {
         break;
      }
      hqProc = (RgSchDlHqProcCb *)(tmp->node);
   }

   if (!tmp)
   {
      /* No Harq Process available in the free queue. */
      return (NULLP);
   }

   hqProc->tbInfo[0].timingInfo = timingInfo;
   hqProc->tbInfo[1].timingInfo = timingInfo;

   /* Remove the element from the free Queue */
   //cmLListDelFrm(&hqE->free, tmp);
   rgSCHDhmHqPDelFrmFreeLst(hqProc);

   /* Add the element into the inUse Queue as well */
   //cmLListAdd2Tail(&hqE->inUse, &hqProc->lnk);
   rgSCHDhmHqPAdd2InUseLst(hqProc);

#ifdef LTE_ADV
   rgSCHLaaResetDlHqProcCb(hqProc);
#endif

   return (hqProc);
} /* rgSCHDhmSpsDlGetHqProc */
#endif /* LTEMAC_SPS */


/** * @brief Handler for handling TA. 
 *
 * @details
 *
 *     Function : rgSCHDhmFdbkIndHndlTa
 *     
 *     This function handles the TA state and values based on the 
 *     feedback indication received.
 *           
 *  @param[in]  RgSchDlHqProcCb   *hqP
 *  @param[in]  uint8_t                tbIdx
 *  @param[in]  uint8_t                fdbk
 *  @return     Void
 *      -# None.
 **/
static Void rgSCHDhmFdbkIndHndlTa(RgSchDlHqProcCb *hqP,uint8_t tbIdx,uint8_t fdbk,Bool maxHqRetxReached)
{
   RgSchUeCb      *ueCb;
   RgSchCellCb    *cell;

   ueCb  = hqP->hqE->ue;
   cell  = ueCb->cell;
   switch(fdbk)
   {
      case TRUE:
         /*ccpu00130018 -ADD - To prevent duplicate insert into the TA list*/  
         hqP->tbInfo[tbIdx].taSnt =  FALSE;
         /* To prevent duplicate inserts of ueCb into TA list */
         if (ueCb->taLnk.node == NULLP)
         {
            ueCb->taLnk.node         =  (PTR)ueCb;
            cmLListAdd2Tail(&cell->taUeLst, &ueCb->taLnk);
         }
         else
         { 
#ifdef DEBUGP            
            DU_LOG("\nERROR  -->  SCH : Trying to add CRNTI:%d into TA"
                     "ACK List twice", ueCb->ueId);
#endif            
         } 
         break;

      case FALSE:
         /* If Ta was sent and its the final NACK, then reset only the 
          * taState to IDLE and not the value */
         /* Changed handling in case maxhqretx is reached for TA */
         if(TRUE == maxHqRetxReached)
         {
            hqP->tbInfo[tbIdx].taSnt    =  FALSE;
            hqP->hqE->ue->dl.taCb.state =  RGSCH_TA_IDLE;

            rgSCHUtlReTxTa(cell, ueCb);
            DU_LOG("\nERROR  -->  SCH : Nack Rcvd for TA. Max Tries Attempted");
         }
         break;
      case TFU_HQFDB_DTX:
         /* If Ta was sent and its the final NACK, then reset only the 
          * taState to IDLE and not the value */
         if(TRUE == maxHqRetxReached)
         {
            hqP->tbInfo[tbIdx].taSnt    =  FALSE;
            hqP->hqE->ue->dl.taCb.state =  RGSCH_TA_IDLE;

            /*ccpu00131191 and ccpu00131317 - Fix for RRC Reconfig failure
             * issue for VoLTE call */
            rgSCHUtlDlTARpt(cell, ueCb);
         }
         break;

      default:
         break;
   }

   return;
} /* rgSCHDhmFdbkIndHndlTa */

/* 3.1 MIMO: TA cmd details at TB level rather than Hq Level */
/** * @brief Handler for scheduling TA.
 *
 * @details
 *
 *     Function : rgSCHDhmShcdTa
 *     
 *     This function is called by scheduler when resource allocation
 *     for TA transmission is done.
 *           
 *  @param[in]  RgSchUeCb         *ue
 *  @param[out] RgSchDlHqTbCb     *tbInfo
 *  @return     Void
 *      -# None.
 **/
Void rgSCHDhmSchdTa(RgSchUeCb *ueCb,RgSchDlHqTbCb *tbInfo)
{

   ueCb->dl.taCb.state = RGSCH_TA_SCHEDULED;
    ueCb->dl.taCb.numRemSf  =  2; 
   tbInfo->schdTa.pres    = PRSNT_NODEF;
   tbInfo->schdTa.val     = ueCb->dl.taCb.ta;

   return;
} /* rgSCHDhmSchdTa */

#ifdef LTE_TDD
/** * @brief Handler for fetching Harq Proc given the feeback information.
 *
 * @details
 *
 *     Function : rgSCHDhmHqProcByFdbkTime
 *
 *     This function shall fetch all the harq proc having the feedback
 *     timing information.
 *
 *  @param[in]  RgSchDlHqEnt       *hqE
 *  @param[in]  CmLteTimingInfo    timeInfo
 *  @param[in]  Bool               *isMsg4
 *  @param[out] RgSchDlHqProcCb    **hqPrcs
 *  @param[out] uint8_t                 *numTbs
 *  @param[out] S8                 *tbStrtIdx
 *  @param[out] uint8_t                 *cntHqPrcs
 *  @return     S16
 **/
static S16 rgSCHDhmHqProcByFdbkTime
(
RgSchDlHqEnt            *hqE,
CmLteTimingInfo         timeInfo,
Bool                    *isMsg4,
RgSchDlHqProcCb         **hqPrcs,
uint8_t                 *numTbs,
S8                      *tbStrtIdx,
uint8_t                 *cntHqPrcs,
RgSchCellCb             *cell
)
{
   RgSchDlHqTbCb        *tbCb;
   RgSchDlHqProcCb      *hqP;
   CmLteTimingInfo      schdSfTime;
   RgSchTddDlAscSetIdxK ascIdx;
   uint8_t              noFdbks;
   uint8_t              i;
   uint8_t              idx;
   uint8_t              dlIdx;
   CmLListCp            *lnk;
   CmLList              *node;

   *cntHqPrcs = 0;
   if (hqE->msg4Proc)
   {
      if (RGSCH_TIMEINFO_SAME(hqE->msg4Proc->tbInfo[0].fdbkTime, timeInfo))
      {
          *isMsg4 = TRUE;
          hqPrcs[*cntHqPrcs] = hqE->msg4Proc;
          tbStrtIdx[*cntHqPrcs] = 0;
          numTbs[*cntHqPrcs] = 1;
          (*cntHqPrcs)++;
          return ROK;
      }
   }
   ascIdx = rgSchTddDlAscSetIdxKTbl[cell->ulDlCfgIdx][timeInfo.slot];
   noFdbks = ascIdx.numFdbkSubfrms;

   for(idx=0; idx<noFdbks; idx++)
   {
      /* ccpu00133109: Removed RGSCHSUBFRMCRNTTIME as it is not giving proper output 
       * if diff is more than 10. Instead using RGSCHDECRFRMCRNTTIME() as it is 
       * serving the purpose */
      RGSCHDECRFRMCRNTTIME(timeInfo,schdSfTime,ascIdx.subfrmNum[idx]);
      dlIdx = rgSCHUtlGetDlSfIdx(cell, &schdSfTime);
      lnk = &hqE->ue->dl.dlSfHqInfo[dlIdx].hqPLst;
      node = lnk->first;
      while (node)
      {
         hqP = (RgSchDlHqProcCb*)node->node;
         node = node->next; 

         numTbs[*cntHqPrcs] = 0;
         tbStrtIdx[*cntHqPrcs] = -1;
         for (i = 0; i < 2; i++)
         {
            /* Extra:check which TB is waiting for feedback */
            if (hqP->tbInfo[i].state == HQ_TB_WAITING)
            {
               if (tbStrtIdx[*cntHqPrcs] == -1)
               {
                  tbStrtIdx[*cntHqPrcs] = i;
               }
               numTbs[*cntHqPrcs]++;
            }
         }
         if (numTbs[*cntHqPrcs] > 0)
         {
            hqPrcs[*cntHqPrcs] = hqP;
            (*cntHqPrcs)++;
         }
      }

      /* AN REP Hq Procs */
      node = cell->subFrms[dlIdx]->ackNakRepQ.first;
      while(node)
      {
         tbCb = (RgSchDlHqTbCb *)(node->node);
         hqP = tbCb->hqP;

         numTbs[*cntHqPrcs] = 0;
         tbStrtIdx[*cntHqPrcs] = -1;
         for (i = 0; i < 2; i++)
         {
            /* Extra:check which TB is waiting for feedback */
            if (hqP->tbInfo[i].state == HQ_TB_WAITING)
            {
               if (tbStrtIdx[*cntHqPrcs] == -1)
               {
                  tbStrtIdx[*cntHqPrcs] = i;
               }
               numTbs[*cntHqPrcs]++;
            }
         }
         if (numTbs[*cntHqPrcs] == 2)
         {
            node = node->next;
         }
         if (numTbs[*cntHqPrcs] > 0)
         {
            hqPrcs[*cntHqPrcs] = hqP;
            (*cntHqPrcs)++;
         }

         node = node->next;
      }
   }


   return ROK;
}
#else  /* LTE_TDD */
/** * @brief Handler for fetching Harq Proc given the timming information.
 *
 * @details
 *
 *     Function : rgSCHDhmHqProcByTime
 *     
 *     This function shall fetch the harq proc using the timing information.
 *           
 *  @param[in]  RgSchDlHqEnt    *hqE
 *  @param[in]  CmLteTimingInfo timeInfo
 *  @param[in]  Bool            *isMsg4
 *  @param[out] uint8_t              *numTbs
 *  @param[out] S8              *tbStrtIdx 
 *  @return     RgSchDlHqProcCb*
 *      -# RgSchDlHqProcCb* 
 *      -# NULLP 
 **/
RgSchDlHqProcCb *rgSCHDhmHqProcByTime(RgSchDlHqEnt *hqE,CmLteTimingInfo timeInfo,Bool *isMsg4,RgSchDlSf *sf)
{
   if (hqE->msg4Proc)
   {
      if (RGSCH_TIMEINFO_SAME(hqE->msg4Proc->tbInfo[0].timingInfo, timeInfo))
      {
          *isMsg4    = TRUE;
          return (hqE->msg4Proc);
      }
   }
   
   return (NULLP);
}
#endif

/** * @brief Handler for handling the harq transaction failure.
 *
 * @details
 *
 *     Function : rgSCHDhmHqTbTrnsFail
 *     
 *     This function handled the harq TB transaction failure :
 *     - If retries have not reached maximum, add to the reTx Q.
 *     - else do error recovery.
 *           
 *  @param[in]  RgSchCellCb             *cell
 *  @param[in]  RgSchDlHqProcCb         *hqP
 *  @param[in]  uint8_t                      tbCnt
 *  @param[out] Bool                    *isMaxRetx
 *  @return    Void
 *             -#None.
 *      
 **/
Void rgSCHDhmHqTbTrnsFail(RgSchCellCb *cell,RgSchDlHqProcCb *hqP,uint8_t tbCnt,Bool  *isMaxRetx)
{
   RgSchDlHqEnt   *hqE;
   uint8_t        maxHqTx;

   hqE = hqP->hqE;

   /* Fetch the maximum number of harq transmissions */
   if (hqE->msg4Proc == hqP)
   {
#ifdef RGR_V1
      if(hqP->hqE->raCb->expiryTime.sfn == RGSCH_CONTRES_EXP)
      {
         DU_LOG("\nDEBUG  -->  SCH : rgSCHDhmHqTbTrnsFail contRes exp(): tmpCRNTI = %u",
               hqP->hqE->raCb->tmpCrnti);
         rgSCHRamMsg4Done(cell, (RgSchRaCb *)hqP->hqE->raCb);
         return;
      }
#endif
      maxHqTx  = cell->dlHqCfg.maxMsg4HqTx;
   }
   else
   {
      maxHqTx  = hqE->maxHqTx;
   }

#ifdef MAC_SCH_STATS
   if (hqE->ue != NULLP)
   {
      RgSchUeCb      *ueCb  = hqE->ue;
      RgSchCmnUe     *cmnUe = (RgSchCmnUe*)ueCb->sch;
      RgSchCmnDlUe   *dlUe  = RG_SCH_CMN_GET_DL_UE(ueCb,hqE->cell);/*CA dev*/
      uint8_t             tbs    = dlUe->mimoInfo.cwInfo[0].iTbs[0];
      static uint32_t retxCnt    = 0;

      {
         ++retxCnt;
         hqFailStats.dlCqiStat[(dlUe->mimoInfo.cwInfo[0].cqi - 1)].numOfNacks++;
      }
      RG_SCH_CMN_DL_TBS_TO_MCS(tbs, 
               (hqFailStats.dlCqiStat[(dlUe->mimoInfo.cwInfo[0].cqi - 1)].mcs));
   }
#endif /* MAC_SCH_STATS */ 

   RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, hqP->tbInfo, tbCnt);
   /* Reset the PDCCH reference */
   hqP->pdcch =  NULL;
   if (hqP->tbInfo[tbCnt].txCntr < maxHqTx)
   {
      hqP->tbInfo[tbCnt].state = HQ_TB_NACKED;

      if((hqE->ue != NULLP) && (hqE->ue->isDrxEnabled == TRUE))
      {

         /*If DRX is enabled for the UE, we need to start the HARQ RTT timer
          * for the UE. Addtion to the retransmission queue will be done on
          * HARQ RTT timer expiry.--*/
         switch(hqP->tbInfo[tbCnt ^ 1].state)
         {
            case HQ_TB_ACKED:
               /*As the first TB is ACKED we have not started HARQ RTT for the
                * HqP, so start it here.*/
               //cmLListDelFrm(&hqE->inUse, &hqP->lnk);
               rgSCHDhmHqPDelFrmInUseLst(hqP);
               /* CA Dev Start */
               rgSCHDrxStartHarqRTTTmr(hqP->hqE->ue->cell, hqP, tbCnt);
               /* CA Dev End*/
#ifdef LTEMAC_SPS
               /* Integration fix */   
               /* Setting cntrRetxAllocFail to MAX value here */
               /* Since the hqP entry is already deleted from inUse list of HqEntity
                  setting the value here will ensure the entry is not deleted
                  again during release harq proc */
               if ( (hqP->sch != (RgSchCmnDlHqProc *)NULLP) && 
                     (RG_SCH_CMN_SPS_DL_IS_SPS_HQP(hqP)))
               {
                  hqP->tbInfo[0].cntrRetxAllocFail = RG_SCH_MAX_RETX_ALLOC_FAIL;
                  if (hqP->tbInfo[1].txCntr)
                  {
                     hqP->tbInfo[1].cntrRetxAllocFail = RG_SCH_MAX_RETX_ALLOC_FAIL;
                  }
               }
#endif
               break;
            case HQ_TB_NACKED:
               /*As the first TB is NACKED we have already started HARQ RTT for the
                * HqP, so dont start it here, just delete from in use queue.*/
               //cmLListDelFrm(&hqE->inUse, &hqP->lnk);
               rgSCHDhmHqPDelFrmInUseLst(hqP);
               break;
            case HQ_TB_WAITING:
               /*As this is the first TB to be fed back and is NACKED start
                * the HARQ RTT here.*/
               /* CA Dev Start */
               rgSCHDrxStartHarqRTTTmr(hqP->hqE->ue->cell, hqP,tbCnt);
               /* CA Dev End*/
               break;
         }
         return;
      }
      /* extra:check if already removed as part of other TB processing
       * then donot remove from InUse Q */
      /* Check if other TB is not waiting for feedback.
       * Makinf sure hqP is present in inUse Queue until
       * it is fedback for all its TBs */
      switch(hqP->tbInfo[tbCnt ^ 1].state)
      {
         case HQ_TB_ACKED:
            /*Fix for ccpu00113296 - Do not delete for Msg4 Harq Entities*/
            if(hqE->msg4Proc != hqP)
            {
              //cmLListDelFrm(&hqE->inUse, &hqP->lnk);
              rgSCHDhmHqPDelFrmInUseLst(hqP);
            }
            /* Retransmission needs to be done. Add to the scheduler Q */
            rgSCHUtlDlProcAddToRetx(hqP->hqE->cell, hqP);
            break;
         case HQ_TB_NACKED:
            /*Fix for ccpu00113296 - Do not delete for Msg4 Harq Entities*/
            if(hqE->msg4Proc != hqP)
            {
              //cmLListDelFrm(&hqE->inUse, &hqP->lnk);
              rgSCHDhmHqPDelFrmInUseLst(hqP);
            }
            break;
         case HQ_TB_WAITING:
            /* Retransmission needs to be done. Add to the scheduler Q */
            /* CA Dev Start*/
            rgSCHUtlDlProcAddToRetx(hqP->hqE->cell, hqP);
            /* CA Dev End*/
            break;
      }
      *isMaxRetx = FALSE; 
   }
   else
   {
      /* Failure Notification */
      if (hqE->msg4Proc == hqP)
      {
       /* SR_RACH_STATS : MSG4 Max Retx Fail*/
         rgNumMsg4FailMaxRetx++;
#ifdef TENB_STATS
         hqE->cell->tenbStats->sch.msg4Fail ++;
#endif

         /* Perform RAM MSG4 done processing */
         DU_LOG("\nDEBUG  -->  SCH : rgSCHDhmHqTbTrnsFail(): hq max retx fail: tmpCRNTI = %u",
               hqP->hqE->raCb->tmpCrnti);
         rgSCHRamMsg4Done(cell, (RgSchRaCb *)hqP->hqE->raCb);
      }
      else
      {
         /* Release the Harq Proc */
         rgSCHDhmRlsHqpTb(hqP, tbCnt, TRUE);
      }
      *isMaxRetx = TRUE; 
   }

   return;
} /* rgSCHDhmHqTbTrnsFail */

uint32_t rgHqRvStats[2][4][2] = {{{0, 0}, {0, 0}, {0, 0}, {0, 0}}, 
                                   {{0, 0}, {0, 0}, {0, 0}, {0, 0}}};
#ifdef LTE_TDD
#ifdef LTE_ADV
/** * @brief Function to decode the position of HarqFb for M=1.
 *
 * @details
 *
 *     Function : rgSchGetHqFdbkPosForM1
 *           
 *  @param[in]  RgSchUeCb            *ue,
 *  @param[in]  RgSchDlHqProcCb      *hqP,
 *  @param[in]  uint8_t                 *isAck, 
 *  @param[in]  RgTfuHqInfo          *fdbk,
 *  @param[in]  uint8_t                   tbIdx,
 *  @param[in]  RgSchTddANInfo       *anInfo;
 *  @return     void
 **/
static Void rgSchGetHqFdbkPosForM1
(
RgSchUeCb       *ue,
RgSchDlHqProcCb *hqP,
uint8_t         *isAck, 
RgTfuHqInfo     *fdbk,
uint8_t         tbIdx,
RgSchTddANInfo  *anInfo
)
{
   if((ue != NULLP))
   {
      /* handle pusch and pucch cases */
      /* PUSCH:: Fdbks are in the increasing order
       * of servCellIdx as per 36.212 section 5.2.26*/
      switch(ue->f1bCsAVal)
      {
         case RG_SCH_A_VAL_2:
            {
               if(RG_SCH_IS_CELL_SEC(ue,hqP->hqE->cell)) 
               {
                  *isAck = fdbk->isAck[1];/*SCell*/
               }
               else
               {
                  *isAck = fdbk->isAck[0];/*PCell*/
               }
               break;
            }
         case RG_SCH_A_VAL_3:
            {
                  if(RG_SCH_IS_CELL_SEC(ue,hqP->hqE->cell)) 
                  {
                     uint8_t cellIdx = rgSchUtlGetServCellIdx(hqP->hqE->cell->instIdx,
                           hqP->hqE->cell->cellId,
                           hqP->hqE->ue);

                     if(rgSCHUtlGetMaxTbSupp(ue->cellInfo[cellIdx]->txMode.txModeEnum) > 1)
                     {/*SCell - mimo mode*/
                        if(TRUE == fdbk->isPusch)
                        {
                           *isAck = fdbk->isAck[tbIdx + 1];
                        }
                        else
                        {
                           *isAck = fdbk->isAck[tbIdx];
                        }
                     }
                     else
                     {/*SCell - siso mode*/
                        *isAck = fdbk->isAck[2];
                     }
                  }else
                  {
                     if(rgSCHUtlGetMaxTbSupp(ue->mimoInfo.txMode) > 1)
                     {/*Primary Cell - mimo mode*/
                        *isAck = fdbk->isAck[tbIdx];
                     }
                     else
                     {/*Primary Cell - siso mode*/
                        if((TRUE == fdbk->isPusch) && (FALSE == anInfo->isSpsOccasion))
                        {
                           /* If fdbk is on PUSCH but its not an SPS occasion*/
                           *isAck = fdbk->isAck[0];
                        }
                        else
                        {
                           /* If fdbk is on PUCCH or its an SPS occasion*/
                           *isAck = fdbk->isAck[2];
                        }
                     }
                  }
               break;
            }
         case RG_SCH_A_VAL_4:
            {
               if(RG_SCH_IS_CELL_SEC(ue,hqP->hqE->cell)) 
               {
                  *isAck = fdbk->isAck[tbIdx + 2];
               }
               else
               {
                  *isAck = fdbk->isAck[tbIdx];
               }
               break;
            }
         default:
            break;
      }
   }
   return;
}/* End of rgSchGetHqFdbkPosForM1 */

/** * @brief Function to decode the position of HarqFb for M>=2 cases.
 *
 * @details
 *
 *     Function : rgSchGetHqFdbkPosForM234
 *           
 *  @param[in]  RgSchUeCb            *ue,
 *  @param[in]  RgSchDlHqProcCb      *hqP,
 *  @param[in]  uint8_t                   *isAck, 
 *  @param[in]  RgTfuHqInfo          *fdbk,
 *  @param[in]  uint8_t                   tbIdx,
 *  @param[in]  RgSchTddANInfo       *anInfo;
 *  @param[in]  CmLteTimingInfo      timeInfo;
 *  @return     void
 **/
static Void rgSchGetHqFdbkPosForM234
(
RgSchUeCb       *ue,
RgSchDlHqProcCb *hqP,
uint8_t         *isAck, 
RgTfuHqInfo     *fdbk,
uint8_t         tbIdx,
RgSchTddANInfo  *anInfo,
uint8_t         M,
CmLteTimingInfo timeInfo
)
{
   uint8_t        fdbkIdx;
   Bool           isSCell;
   RgSchTddANInfo *pCellAnInfo;
   uint8_t        incr = 0;

   if(NULLP != ue)
   {
      isSCell = RG_SCH_IS_CELL_SEC(ue,hqP->hqE->cell);
      pCellAnInfo = rgSCHUtlGetUeANFdbkInfo(ue, &timeInfo, RGSCH_PCELL_INDEX);

      if(TRUE == fdbk->isPusch)
      {
         if(TRUE == isSCell) 
         {/*SCell*/
            if (anInfo->wUlDai == 3)
            {
               incr = anInfo->wUlDai;
            }
            else
            {
               incr = M;
            }
            if(1 == anInfo->ulDai)
            {
               fdbkIdx = (hqP->tbInfo[tbIdx].dai - 1) + 
                  hqP->tbInfo[tbIdx].tbIdx + incr;
            }
            else
            {
               fdbkIdx = (hqP->tbInfo[tbIdx].dai - 1) + incr;
            }
         }
         else
         {/*PCell*/
            if(1 == anInfo->ulDai)
            {
               if(rgSCHUtlGetMaxTbSupp(ue->mimoInfo.txMode) > 1)
               {
                  fdbkIdx = (hqP->tbInfo[tbIdx].dai - 1) + (hqP->tbInfo[tbIdx].tbIdx);
               }
               else
               {
                  fdbkIdx = (hqP->tbInfo[tbIdx].dai) - 1; 
               }
            }
            else
            {
               fdbkIdx = (hqP->tbInfo[tbIdx].dai) - 1; 
            }
         }
      }
      else
      {/*PUCCH*/
         if(TRUE == isSCell) 
         {
            /* pucchFdbkIdx is set to DAI hence -1 to get index */
            fdbkIdx = ((hqP->tbInfo[tbIdx].pucchFdbkIdx) + M -1); 
         }
         else
         {
            if(M > 2)
            {
               /* SPS occasion feedback in case of M > 2 will
                * be always present in the index 0*/
#ifdef LTEMAC_SPS
               if(hqP->spsN1PucchRes.pres == TRUE)
               {/* SPS occasion hq proc */
                  fdbkIdx = 0;
               }else 
#endif
               if((NULLP != pCellAnInfo) && 
                     (pCellAnInfo->dlDai != pCellAnInfo->ulDai))
               {
                  fdbkIdx = hqP->tbInfo[tbIdx].pucchFdbkIdx; 
               }else
               {/* NO SPS occasion was present in the bundle*/
                  fdbkIdx = hqP->tbInfo[tbIdx].pucchFdbkIdx - 1; 
               }
            }
            else
            {
               fdbkIdx = hqP->tbInfo[tbIdx].pucchFdbkIdx - 1; 
            }
         }
      }
      *isAck = fdbk->isAck[fdbkIdx];
#ifdef DLHQ_STATS
      static RgSchDlHqProcCb      *temp = NULLP;
      if (temp != hqP->tbInfo[tbIdx].hqP)
      {
         statsCnt = statsCnt % 10000;
         dlHqStats[statsCnt].cellId = hqP->hqE->cell->cellId;
         dlHqStats[statsCnt].sfn = hqP->tbInfo[tbIdx].timingInfo.sfn; 
         dlHqStats[statsCnt].sf = hqP->tbInfo[tbIdx].timingInfo.slot; 
         dlHqStats[statsCnt].ack = *isAck;
         dlHqStats[statsCnt].fdbkIdx = fdbkIdx;
         dlHqStats[statsCnt].ue = hqP->hqE->ue->ueId;
         if (anInfo)
            dlHqStats[statsCnt].ulDai = incr;
         if(TRUE == fdbk->isPusch)
         {
            dlHqStats[statsCnt].dlDai = hqP->tbInfo[tbIdx].dai;
         }
         else
         {
            dlHqStats[statsCnt].dlDai = hqP->tbInfo[tbIdx].pucchFdbkIdx;
         }
         if (*isAck != 1)
         {
            dlHqStats[statsCnt].ack0 = fdbk->isAck[0];
            dlHqStats[statsCnt].ack1 = fdbk->isAck[1];
            dlHqStats[statsCnt].ack2 = fdbk->isAck[2];
            dlHqStats[statsCnt].ack3 = fdbk->isAck[3];
            dlHqStats[statsCnt].ack4 = fdbk->isAck[4];
            dlHqStats[statsCnt].ack5 = fdbk->isAck[5];
            dlHqStats[statsCnt].ack6 = fdbk->isAck[6];
            dlHqStats[statsCnt].ack7 = fdbk->isAck[7];
         }
         statsCnt++; 
         temp = hqP->tbInfo[tbIdx].hqP;
      }
#endif
   }/*ue*/
   return;
}/*rgSchGetHqFdbkPosForM234*/
#endif/*LTE_ADV*/

/*
 * @brief Handler for HARQ feedback received for DL transmission.
 *
 * @details
 *
 *     Function : rgSCHDhmHqFdbkInd
 *
 *     This function shall act on the feedback received from TOM for DL
 *     transmission. If the feedback for msg4 is final (after max transmissions
 *     or ACK) inform RAM that Msg4 transmission is done.
 *
 *
 *  @param[in]  Void                *cb
 *  @param[in]  uint8_t                  cbType
 *  @param[in]  RgSchCellCb         cellCb
 *  @param[in]  CmLteTimingInfo     timeInfo 
 *  @param[in]  TfuHqInfo           *fdbk
 *  @param[in]  RgInfRlsHqInfo      *rlsHqBufs
 *  @param[in]  RgSchErrInfo        *err
 *  @return     S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHDhmHqFdbkInd
(
Void                 *cb,
uint8_t              cbType,
RgSchCellCb          *cellCb,
CmLteTimingInfo      timeInfo,
TfuHqInfo            *fdbk,
RgInfRlsHqInfo       *rlsHqBufs,
RgSchErrInfo         *err
)
{
   RgSchCellCb *sCell = NULLP;
   RgSchDlHqEnt      *hqE;
   /*ccpu00127339 - MOD - change to avoid the crash*/
   RgSchUeCb         *ue = NULLP;
   RgSchDlSf         *sf;
   Bool              isMsg4 = FALSE;
   RgSchRaCb         *raCb = NULLP;
   uint16_t          rnti=0;
   /* Maximum possible HARQ processes in UL-DL configuration 5 that is
    * given feedback at a time */
   RgSchDlHqProcCb   *hqPrcs[(RGSCH_NUM_SUB_FRAMES-1)*5];  /*MAX 5 Cells*/
   uint8_t                numTb[(RGSCH_NUM_SUB_FRAMES-1)*5];
   S8                tbStrtIdx[(RGSCH_NUM_SUB_FRAMES-1)*5];
   uint8_t                hqCnt;
   uint8_t                idx;
   RgSchTddANInfo    *anInfo = NULLP;
   uint8_t                isAck = 0;
   uint8_t                tbCnt;
   RgrTddAckNackMode ackNackMode;
   Bool              hqRls = FALSE;
   RgSchDlSf         *nxtDlsf = NULLP;
   /* uint8_t                rcvCnt = 0; */
   CmLteTimingInfo   nxtfrm = {0,0};
   Bool              anUpd = FALSE;
   Bool              maxHqRetxReached;
#ifdef LTEMAC_SPS
   Bool              hasRelPdcch = FALSE;
#endif

#if ((defined LTEMAC_SPS_AN_MUX) || (defined LTE_ADV))
   RgSchTddDlAscSetIdxK    ascIdx;
   uint8_t                      noFdbks;
#endif

#ifdef LTEMAC_SPS_AN_MUX
   Bool                    isPusch   = FALSE;   
   uint8_t                      tmpIdx;
   uint8_t                      hIdx;   
   /* Subframes in which transmissions are scheduled and whose feedback can come
    * in this subframe. Used only for Multiplexing mode */
   CmLteTimingInfo         schdSfTime[RGSCH_TDD_MAX_FDBK];
#ifdef RGSCH_SPS_STATS
   RgSchCmnDlHqProc        *cmnHqDl;
#endif
#endif
#ifdef LTE_ADV
   uint8_t                  sCellActCePres = 0;
#endif
/* LTEMAC_SPS_AN_MUX*/
   RgrSchFrmt1b3TypEnum uciFrmtTyp  = RG_SCH_UCI_FORMAT1A_1B; 

   if (cbType == RGSCH_HQ_FDB_IND_CB_TYPE_RA_CB)
   {
      raCb     = (RgSchRaCb *)(cb);
      ackNackMode = RGR_TDD_ACKNACK_MODE_BUNDL;
      hqE      = raCb->dlHqE;
      /* ccpu00139061 Fix */
      rnti     = raCb->tmpCrnti;
   }
   else
   {
      ue       = (RgSchUeCb *)(cb);
      ackNackMode = ue->dl.ackNackMode;
      hqE      = RG_SCH_CMN_GET_UE_HQE(ue, cellCb);
      rnti     = ue->ueId;
#ifdef LTEMAC_SPS_AN_MUX
      isPusch = fdbk->isPusch;
#endif

#ifdef LTEMAC_SPS      
      if (RGSCH_TIMEINFO_SAME(timeInfo, ue->relPdcchFbkTiming))
      {
         hasRelPdcch = TRUE;
      }
#endif
   }
   
#if ((defined LTEMAC_SPS_AN_MUX) || (defined LTE_ADV))
   ascIdx = rgSchTddDlAscSetIdxKTbl[cellCb->ulDlCfgIdx][timeInfo.slot];
   noFdbks = ascIdx.numFdbkSubfrms;
#endif
#ifdef LTEMAC_SPS_AN_MUX
   /* Calculate the subframe time at which transmissions should have happened to
    * receive feedback in this subframe */
   if (ackNackMode == RGR_TDD_ACKNACK_MODE_MULT)
   {
      for(idx=0; idx<noFdbks; idx++)
      {
         /* ccpu00133109: Removed RGSCHSUBFRMCRNTTIME as it is not giving proper output 
          * if diff is more than 10. Instead using RGSCHDECRFRMCRNTTIME() as it is 
          * serving the purpose */
          RGSCHDECRFRMCRNTTIME(timeInfo,schdSfTime[idx],ascIdx.subfrmNum[idx]);
      }
   }
#endif

   rgSCHDhmHqProcByFdbkTime(hqE, timeInfo, &isMsg4, hqPrcs, numTb, \
                            tbStrtIdx, &hqCnt,hqE->cell);
   /* Fetch the harqProc from the inUse list */
#ifdef LTEMAC_SPS
   if ((FALSE == hasRelPdcch) && (hqCnt == 0))
#endif
   if(hqCnt == 0)
   {
      err->errType   = RGSCHERR_DHM_FDBK_IND;
      err->errCause  = RGSCHERR_DHM_FDBK_IND_INVALID_CB;
      return RFAILED;
   }

   /* ccpu00147469 : This code is moved below as here this code always try to
    * get the primary cell aninfo. it is due to hqE->cell->cellId as it is
    * cellId of PCEll
    */

   if(fdbk->hqFdbkMode == TFU_ACK_NACK_SPECIAL_BUNDLING)
   {
      rgSCHDhmPrcSplBundlFdbk(cellCb, fdbk, hqCnt);    
   }

#ifdef TFU_TDD
#endif

#ifdef LTEMAC_SPS_AN_MUX
   /* Check if feedback came on configured UL SPS grant in Muxing mode */
  if((ackNackMode == RGR_TDD_ACKNACK_MODE_MULT) && 
        (isPusch == TRUE) )
   {
      hIdx = 0;
      /* Pick the valid feedbacks out of M feedbacks */
      for(idx=0; idx<noFdbks; idx++)
      {
         /* Skip the feedback if hqProc[idx] did not transmit in schdSf[idx] 
          * and no release pdcch is sent in prevTime */
         /* TODO: SPS_TDD: Check this while testing */
         if ((hqPrcs[hIdx] && !RGSCH_TIMEINFO_SAME(hqPrcs[hIdx]->\
                     tbInfo[(S16)(tbStrtIdx[hIdx])].timingInfo,\
                        schdSfTime[idx])) &&
               !RGSCH_TIMEINFO_SAME(ue->relPdcchTxTime, schdSfTime[idx]))
         { 
            /* Discard the feedback which is corresponding to a subframe in
             * which no DL transmission took place */
            tmpIdx = idx+1;
            while(tmpIdx < noFdbks)
            {
               fdbk->isAck[tmpIdx-1] = fdbk->isAck[tmpIdx];
               fdbk->isAck[tmpIdx-1] = fdbk->isAck[tmpIdx];
               tmpIdx++;
            }
            rcvCnt--;
            ++hIdx;
         }
      } /* end of for loop */
   } /* end of configured UL SPS grant check */
#endif   
#ifdef CA_DBG
  {
     if(ue)
     {
        gHqFdbkCount++;
     }
  }

#endif   

  for(idx=0;idx < hqCnt; idx++)
  {
     /* Fix for CR ccpu00147469: Get the anInfo for each harq proc */
     if(ue)
     {
#ifdef LTE_ADV
        uint8_t servCellIdx = rgSchUtlGetServCellIdx(hqPrcs[idx]->hqE->cell->instIdx,
        hqPrcs[idx]->hqE->cell->cellId,ue);

        if(ue->cellInfo[servCellIdx]->sCellState != RG_SCH_SCELL_ACTIVE)  
        {
           continue; 
        }
        anInfo = rgSCHUtlGetUeANFdbkInfo(ue, &timeInfo,servCellIdx);
#else
        anInfo = rgSCHUtlGetUeANFdbkInfo(ue, &timeInfo,RGSCH_PCELL_INDEX);
#endif
        if(anInfo == NULLP)
        {
           RGSCHDBGINFO(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx), 
                    "Ack Rcvd. No Ack/Nack feedback available \n"));
           return RFAILED;
        }
     }

     sCell = hqPrcs[idx]->hqE->cell;
     rlsHqBufs = &(sCell->rlsHqArr[sCell->crntHqIdx]);
     rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs = 0;
     for (tbCnt = tbStrtIdx[idx]; (tbCnt-tbStrtIdx[idx]) < numTb[idx]; tbCnt++)
     {
        /* Fix : syed MultiUe per TTI crash in TA List. */
        hqRls = FALSE;
        maxHqRetxReached = FALSE;
        /* Remove the harq process from the subframe */
        sf = rgSCHUtlSubFrmGet(cellCb, hqPrcs[idx]->tbInfo[tbCnt].timingInfo);

        if(NULLP != ue)
        {
           uciFrmtTyp = ue->dl.dlSfHqInfo[cellCb->cellId][sf->dlIdx].uciFrmtTyp;
        }

        if(uciFrmtTyp != RG_SCH_UCI_FORMAT1B_CS)
        {
           if((fdbk->hqFdbkMode != TFU_ACK_NACK_SPECIAL_BUNDLING)&&
                 (RGR_TDD_ACKNACK_MODE_MULT == ackNackMode))
           {
              isAck = fdbk->isAck[hqPrcs[idx]->tbInfo[tbCnt].m];
           }
           else
           {
              /* TODO: review for TM4 and CA interaction */
              if((TRUE == hqPrcs[idx]->cwSwpEnabled) && (1 < numTb[idx]))
              {
                 isAck = fdbk->isAck[!tbCnt];
              }
              else
              {
                 isAck = fdbk->isAck[tbCnt];
              } 
           }
        }
#ifdef LTE_ADV
        else
        {
              if(1 == noFdbks)
              {/* M == 1 case */
                 rgSchGetHqFdbkPosForM1(ue, hqPrcs[idx], &isAck, fdbk, tbCnt, anInfo); 
              }
              else
              {
                 rgSchGetHqFdbkPosForM234(ue, hqPrcs[idx], &isAck, fdbk, tbCnt, anInfo, noFdbks, timeInfo); 
              }
        }
#endif

#ifdef BRDCM
         /* revanth tweakin AN PUSCH to ACK always */
         if (hqPrcs[idx]->isPuschFdbk)
         {
            isAck = 1;
         }
#endif

         hqPrcs[idx]->tbInfo[tbCnt].isAckNackDtx = isAck;
         if(cellCb->ulDlCfgIdx != 5)
         {
            rgSCHUtlGetNxtDlSfInfo(hqPrcs[idx]->tbInfo[tbCnt].timingInfo,\
                                   cellCb, sf, &nxtDlsf, &nxtfrm);
         }
         /* Keep a tab on how many ACKs or NACKs we have received */
         if (isAck == TFU_HQFDB_ACK)
         {
            hqPrcs[idx]->tbInfo[tbCnt].ackCount += 1; /* Ack counter */ 
            rgHqRvStats[tbCnt][hqPrcs[idx]->tbInfo[tbCnt].dlGrnt.rv][0]++;
#ifdef TENB_STATS
            sCell->tenbStats->sch.dlAckNack[tbCnt]\
               [hqPrcs[idx]->tbInfo[tbCnt].dlGrnt.rv]++;
#endif
            /* Do not update the Ul Trans Time in case of raCb */
            if (ue)
            { 
               rgSCHUtlHdlUlTransInd(cellCb, ue, timeInfo);
#ifdef TENB_STATS
               ue->tenbStats->stats.nonPersistent.sch[RG_SCH_CELLINDEX(sCell)].dlAckNackCnt[tbCnt] ++;
#endif
            }
         }
         else if (isAck == TFU_HQFDB_NACK)
         {
            hqPrcs[idx]->tbInfo[tbCnt].nackCount += 1; /* Nack Counter */ 
            rgHqRvStats[tbCnt][hqPrcs[idx]->tbInfo[tbCnt].dlGrnt.rv][1]++;
#ifdef TENB_STATS
            sCell->tenbStats->sch.dlNack[tbCnt]\
               [hqPrcs[idx]->tbInfo[tbCnt].dlGrnt.rv]++;
            sCell->tenbStats->sch.dlAckNack[tbCnt]\
               [hqPrcs[idx]->tbInfo[tbCnt].dlGrnt.rv]++;
#endif
            /* Do not update the Ul Trans Time in case of raCb */
            if (ue)
            { 
               rgSCHUtlHdlUlTransInd(cellCb, ue, timeInfo);
#ifdef TENB_STATS
               ue->tenbStats->stats.nonPersistent.sch[RG_SCH_CELLINDEX(sCell)].dlNackCnt[tbCnt] ++;
               ue->tenbStats->stats.nonPersistent.sch[RG_SCH_CELLINDEX(sCell)].dlAckNackCnt[tbCnt] ++;
#endif
            }
#ifdef LTE_L2_MEAS           
            if(hqPrcs[idx]->tbInfo[tbCnt].txCntr == 1)
            {
               cellCb->dlUlTbCnt.tbTransDlFaulty++; 
            }   
#endif            
         }
         else
         {
            hqPrcs[idx]->tbInfo[tbCnt].dtxCount += 1;  /* DTX Counter*/
#ifdef TENB_STATS
            sCell->tenbStats->sch.dlDtx[tbCnt]\
               [hqPrcs[idx]->tbInfo[tbCnt].dlGrnt.rv]++;
            if (ue)
            {
               ue->tenbStats->stats.nonPersistent.sch[RG_SCH_CELLINDEX(sCell)].dlDtxCnt[tbCnt] ++;
            }
#endif
#ifdef LTE_L2_MEAS           
            if(hqPrcs[idx]->tbInfo[tbCnt].txCntr == 1)
            {
               cellCb->dlUlTbCnt.tbTransDlFaulty++; 
            }   
#endif            
         }
#ifdef CA_DBG
         if(ue && RG_SCH_IS_CELL_SEC(ue,hqPrcs[idx]->hqE->cell))
         {
            if(isAck == TFU_HQFDB_ACK)
            {
               gSCellTb1AckCount++;
               gSCellTb2AckCount++;
            }else if(isAck == TFU_HQFDB_NACK)
            {
               gSCellTb1NackCount++;
               gSCellTb2NackCount++;
            }else
            {
               gSCellTb1DtxCount++;
               gSCellTb2DtxCount++;
            }
         }
         else
         {
            if(isAck == TFU_HQFDB_ACK)
            {
               gPCellTb1AckCount++;
               gPCellTb2AckCount++;
            }else if(isAck == TFU_HQFDB_NACK)
            {
               gPCellTb1NackCount++;
               gPCellTb2NackCount++;
            }else
            {
               gPCellTb1DtxCount++;
               gPCellTb2DtxCount++;
            }
         }
#endif
         /* Check if this is repeating UE */
         if (hqPrcs[idx]->tbInfo[tbCnt].fbkRepCntr != 0)
         {
            rgSCHUtlDlHqPTbRmvFrmTx(sf, hqPrcs[idx], tbCnt, TRUE);
            /* Check if last repetition */
            if (--hqPrcs[idx]->tbInfo[tbCnt].fbkRepCntr)
            {
               RGSCH_NULL_CHECK(cellCb->instIdx, nxtDlsf);
               /* Update feedback time for this hqP TB so that 
                * next subframe its picked up */
               RGSCH_UPD_HQAN_FDBKTIME(&hqPrcs[idx]->tbInfo[tbCnt],\
                                       nxtDlsf, nxtfrm);
               RGSCH_NULL_CHECK(cellCb->instIdx, anInfo);
               RGSCH_UPD_ANINFO_WITH_HQ(anInfo, &hqPrcs[idx]->tbInfo[tbCnt]);
               anUpd = TRUE;
               continue;
            }
            /* For a repeating UE take the decision here */
            /* For a repeating UE take the decision here */
            if (((hqPrcs[idx]->tbInfo[tbCnt].ackCount) > (hqPrcs[idx]->tbInfo[tbCnt].nackCount)) && 
               ((hqPrcs[idx]->tbInfo[tbCnt].ackCount) > (hqPrcs[idx]->tbInfo[tbCnt].dtxCount)))
            {
               isAck = TFU_HQFDB_ACK; 
            }
            else if (((hqPrcs[idx]->tbInfo[tbCnt].dtxCount) > (hqPrcs[idx]->tbInfo[tbCnt].nackCount)) && 
                  ((hqPrcs[idx]->tbInfo[tbCnt].dtxCount) > (hqPrcs[idx]->tbInfo[tbCnt].ackCount)))
            {
               isAck = TFU_HQFDB_DTX; 
            }
            else
            {
               isAck = TFU_HQFDB_NACK; 
            }
            hqPrcs[idx]->tbInfo[tbCnt].isAckNackDtx = isAck;
         }
         else
         {
            rgSCHUtlDlHqPTbRmvFrmTx(sf, hqPrcs[idx], tbCnt, FALSE);
         }
#ifdef LTEMAC_SPS
         if (((isAck == TFU_HQ_NACK) || (isAck == TFU_HQ_ACK)) &&
               ((hqPrcs[idx]->sch != (RgSchCmnDlHqProc *)NULLP) &&
                (RG_SCH_CMN_SPS_DL_IS_SPS_TX_HQP(hqPrcs[idx])))
            )
         {
            /* ACK or NACK received for SPS ACTV PDCCH
             * Hence consider SPS ACTVN PDCCH received successfully */
            rgSCHUtlDlProcAck(cellCb, hqPrcs[idx]);
         }
#endif
         if(TFU_HQFDB_ACK == isAck)
         {
            /* SPS_REVIEW */
            if (isMsg4 == TRUE)
            {
               if (raCb == NULLP)
               {
                  raCb = rgSCHDbmGetRaCb(cellCb, rnti);
               }
               /* Inform Random Access Module regarding the ack received */
               if (raCb != NULLP)
               {
                  /*RRC Connection Setup failure issue where RRC connection
                   * setup was not reaching UE due to message 4 HARQ failure */
#ifdef XEON_SPECIFIC_CHANGES
                  CM_LOG_DEBUG(CM_LOG_ID_SCH, "Msg4 Harq SUCCESS for UE(%d)\n", rnti);
#endif 
                  rgSCHRamMsg4Done(cellCb, raCb);
               }
            }
            else /*ccpu00114124- HARQ Release for Msg4 */
            {
#ifdef DL_LA
               /*Update feedback history for every Tx/Retx */
                  rgSCHDhmUpdateAckNackHistory(sCell, ue, isAck, tbCnt);
#endif
               RGSCH_NULL_CHECK(cellCb->instIdx, ue);
               RG_UPD_ACQI_TRIG_WT(ue, sCell, isAck);
#ifdef LTE_ADV
               /* Store activation CE presence as it is required later to start
                *activation delay timer */
               sCellActCePres = hqPrcs[idx]->tbInfo[tbCnt].schdSCellActCe.pres; 
#endif
               rgSCHDhmRlsHqpTb(hqPrcs[idx], tbCnt, TRUE);
            }
            hqRls = TRUE;
         }
         else
         {
            /* If this Msg4 DTX, there will be 
             * no DlHqProc as it has its own HarqProc */
             /* SPS_REVIEW */
            {
               rgSCHDhmHqTbTrnsFail(cellCb, hqPrcs[idx], tbCnt, &hqRls);
               maxHqRetxReached = hqRls;
#ifdef DL_LA
               if ((isMsg4 == FALSE))
               {
                  /*Update feedback history for every Tx/Retx */
                  rgSCHDhmUpdateAckNackHistory(sCell, ue, isAck, tbCnt);
               }
#endif
               if (isMsg4 == FALSE)
               {
                  RGSCH_NULL_CHECK(cellCb->instIdx, ue);
                  RG_UPD_ACQI_TRIG_WT(ue, sCell, isAck);
               }
            }
         }

         if(TRUE == hqRls)
         {
     /* MS_WORKAROUND: to increase Harq Fail Counter .
       The status field is required for tracking the number of harq faliures at MAC*/
         if (isAck)
         {
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].status[\
               rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs] = TRUE;
         }
         else
         {
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].status[\
               rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs] = FALSE;
         }
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].tbId[\
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs] = tbCnt + 1;
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs++;
         }

         /* Handle the TA */
         if (hqPrcs[idx]->tbInfo[tbCnt].taSnt == TRUE)
         {
            rgSCHDhmFdbkIndHndlTa(hqPrcs[idx], tbCnt, isAck, maxHqRetxReached);
         }
#ifdef LTE_ADV
         /* Handle Scell activation */
         if (TRUE == sCellActCePres)
         {
            /* Primary Cellcb needs to be retrived
             * if the feedback is coming on pusch of 
             * sec cell. THis needs to be considered
             * while UL_CA*/
            rgSCHSCellHndlFdbkInd(hqPrcs[idx], tbCnt, isAck, maxHqRetxReached);
         }
#endif
      }
     if (hqRls == FALSE)
     {
        hqPrcs[idx]->cwSwpEnabled = FALSE;
     }
      if(rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs)
      {
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].rnti = rnti;
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].hqProcId = 
            hqPrcs[idx]->procId;
         rlsHqBufs->numUes++;
      }
#ifdef BRDCM
      hqPrcs[idx]->isPuschFdbk = 0;
#endif
   }

#ifdef LTEMAC_SPS
  /*it is possible for some TDD configurations (like TDD cfg 5) 
   * to have multiple feedback for 13 subframes before. It is 
   * possible in such a case to have a release sent after data
   * thus running into a situation where we are receiving feedback
   * for both data and relese pdcch 
   */
/*
   if ( (hqCnt == 0) ||
        (hasRelPdcch && rcvCnt > hqCnt)
      ) 
      */
   if (ue && hasRelPdcch)   
   {
      /* Bool found      = FALSE; */
      
      sf = rgSCHUtlSubFrmGet(cellCb, ue->relPdcchTxTime);

      /*
      if ( !sf->relPdcch)
      {
         found = FALSE;
      } 
      */

      
#if ((defined LTEMAC_SPS_AN_MUX) || (defined LTE_ADV))
      if(ackNackMode == RGR_TDD_ACKNACK_MODE_MULT)
      {
         CmLteTimingInfo txTime;
         uint8_t   ulDlCfgIdx = 0;
         uint8_t   maxFdbks   = 0;
         uint8_t   itr        = 0;

         ulDlCfgIdx  = cellCb->ulDlCfgIdx;

         maxFdbks = rgSchTddDlAscSetIdxKTbl[ulDlCfgIdx]
            [timeInfo.slot].
            numFdbkSubfrms;

         for(itr=0; itr< maxFdbks; itr++)
         {
#ifdef LTE_ADV
            /* Handling the case of only SPS release pdcch
             * and no other scheduling in both the serving cells
             * */
            if(ue->uciFrmtTyp == RG_SCH_UCI_FORMAT1B_CS)
            {/* Using the sorted K table */
               RGSCHDECRFRMCRNTTIME (timeInfo, txTime, 
                     rgSchTddDlHqPucchResCalTbl[ulDlCfgIdx][timeInfo.slot].subfrmNum[itr]);
            }else
#endif
            {
               RGSCHDECRFRMCRNTTIME (timeInfo, txTime, 
                     rgSchTddDlAscSetIdxKTbl[ulDlCfgIdx][timeInfo.slot].subfrmNum[itr]);
            }

            if (RGSCH_TIMEINFO_SAME (txTime, ue->relPdcchTxTime))
            {
#ifdef LTE_ADV
               if((ue->uciFrmtTyp == RG_SCH_UCI_FORMAT1B_CS)&&
                     (maxFdbks == 1))
               {/* M == 1 case */
                  if(rgSCHUtlGetMaxTbSupp(ue->mimoInfo.txMode) > 1)
                  {
                     isAck = fdbk->isAck[0];
                  }else
                  {
                     isAck = fdbk->isAck[2];
                  }
               }
               else
                  /* M > 1 same below logic apply. 
                     If SPS occasion and rel pdcch is present
                     SPS occasion after SPS release cannot 
                     happen in a bundle
                     */
#endif
               {
                  isAck = fdbk->isAck[itr];
               }
               
               rgSCHUtlDlRelPdcchFbk(cellCb, ue, isAck);

               RGSCH_NULL_CHECK(cellCb->instIdx, sf->relPdcch);
               /* Remove release PDCCH from the subframe */
               rgSCHUtlPdcchPut(cellCb, &sf->pdcchInfo, sf->relPdcch);
               sf->relPdcch = NULLP;
               /* found = TRUE; */
               break;
            }

         }
      }
      else
#endif
      {
         RGSCH_NULL_CHECK(cellCb->instIdx, sf->relPdcch);
         /* Remove release PDCCH from the subframe */
         rgSCHUtlPdcchPut(cellCb, &sf->pdcchInfo, sf->relPdcch);
         sf->relPdcch = NULLP;
         /* found = TRUE; */
         rgSCHUtlDlRelPdcchFbk(cellCb, ue, fdbk->isAck[0]);
      }
      /*
         if ( found == FALSE ) 
         {
         RGSCH_NULL_CHECK(cellCb->instIdx, ue);
         DU_LOG("\nERROR  -->  SCH : NO HARQ proc available for feedback:timeInfo:snf %d,slot %d",
         ue->ueId,timeInfo.sfn, timeInfo.slot);
         err->errType   = RGSCHERR_DHM_FDBK_IND;
         err->errCause  = RGSCHERR_DHM_FDBK_IND_INVALID_CB;
         return RFAILED;
         }
       */
   }/*if(hqCnt==0)*/
#endif /* LTEMAC_SPS */
   /* Initialise the Ack/Nack feedback */
   /* [ccpu00127651] - MOD For Msg4 Harq Proc, anInfo will not be filled while 
      scheduling. So added a condition !isMsg4 to avoid calling the function 
      rgSCHUtlInitUeANFdbkInfo*/
   if((ue) && (!anUpd) && (!isMsg4))
   {
#ifdef LTE_ADV
      /* TODO:: Initi the anInfo all the serving cells */
      for(idx = 0; idx <= RG_SCH_MAX_SCELL; idx++)
      {
         if(ue->cellInfo[idx])
         {
            anInfo = rgSCHUtlGetUeANFdbkInfo(ue, &timeInfo,idx);
            /* Fix for CR ccpu00147693: If anInfo is there then initialize it
             * else don't do anything. basically continue for next serving
             * cell*/
            if(anInfo)
            {
               rgSCHUtlInitUeANFdbkInfo(anInfo);
            }
         }
      }
#else
      rgSCHUtlInitUeANFdbkInfo(anInfo);
#endif
   }

   return ROK;
}
//#endif /* LTEMAC_SPS */

#else /* LTE_TDD */
/** * @brief Handler for HARQ feedback received for DL transmission.
 *
 * @details
 *
 *     Function : rgSCHDhmPrcFdbkForTb
 *     
 *                Process Hq Prc Fdbk for a TB
 *           
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgTfuHarqAckIndInfo *fdbk
 *  @param[in]  RgSchErrInfo        *err
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHDhmPrcFdbkForTb
(
RgSchCellCb     *cell,
RgSchUeCb       *ue,
RgSchDlHqProcCb *hqP,
RgSchDlSf       *sf,
Bool            isMsg4,
uint16_t        rnti,
uint8_t         tbCnt,
CmLteTimingInfo timingInfo,
uint8_t         isAck,
RgInfRlsHqInfo  *rlsHqBufs,
RgSchErrInfo    *err
)
{
#ifdef DEBUGP
   Inst    inst = cell->instIdx;
#endif
#ifdef RGSCH_SPS_STATS
   RgSchCmnDlHqProc        *cmnHqDl;
#endif
   S16                  ret = ROK;
   RgSchRaCb            *raCb = NULLP;
   Bool                 hqRls=FALSE;
   Bool                 hqFreed =FALSE;
   Bool                 maxHqRetxReached = FALSE;
   RgSchCmnDlUe         *ueDl = NULLP;
   RgSchCellCb          *sCell = hqP->hqE->cell;
#ifdef EMTC_ENABLE
   RgSchEmtcDlSf        *emtcSf;
   CmLteTimingInfo      frm = timingInfo;
#endif

   if (ue)
   {
      ueDl     = RG_SCH_CMN_GET_DL_UE(ue,cell);
   }
   hqRls   = FALSE;
   /* Fix : syed MultiUe per TTI crash in TA List. */
   maxHqRetxReached = FALSE;

   /* Fix : syed Consider CW to TB mapping for Hq Feedback.
    * TODO: Need to enhance this in case of TM4 testing,
    * when cwSwap flag is considered. */

   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "rgSCHDhmHqFdbkInd():\
            tbCnt=%d , isAck=%d",tbCnt,isAck));
   if (isAck == TFU_HQFDB_ACK)
   {
      hqP->tbInfo[tbCnt].ackCount += 1; /* Ack counter */
      /*sanjay*/
      rgHqRvStats[tbCnt][hqP->tbInfo[tbCnt].dlGrnt.rv][0]++;
      /* Do not update the Ul Trans Time in case of raCb */
      if (ue)
      { 
         rgSCHUtlHdlUlTransInd(cell, ue, timingInfo);
#ifdef TENB_STATS
         ue->tenbStats->stats.nonPersistent.sch[RG_SCH_CELLINDEX(sCell)].dlAckNackCnt[tbCnt]++;
#endif

      }

#ifdef TENB_STATS
      sCell->tenbStats->sch.dlAckNack[tbCnt]\
         [hqP->tbInfo[tbCnt].dlGrnt.rv]++;

#endif

   }
   else if (isAck == TFU_HQFDB_NACK)
   {
      hqP->tbInfo[tbCnt].nackCount += 1; /* Nack Counter */
      RGSCHDBGINFONEW(inst, (rgSchPBuf(inst), "<HARQ> HqP[%d:%d] NACKED "
               "ue(%d)\n", hqP->procId, tbCnt, hqP->hqE->ue->ueId));
      rgHqRvStats[tbCnt][hqP->tbInfo[tbCnt].dlGrnt.rv][1]++;
      /* Do not update the Ul Trans Time in case of raCb */

#ifdef TENB_STATS
      sCell->tenbStats->sch.dlAckNack[tbCnt]\
         [hqP->tbInfo[tbCnt].dlGrnt.rv]++;
      
      sCell->tenbStats->sch.dlNack[tbCnt]\
         [hqP->tbInfo[tbCnt].dlGrnt.rv]++;


#endif

      if (ue)
      { 
         rgSCHUtlHdlUlTransInd(cell, ue, timingInfo);
#ifdef TENB_STATS
         ue->tenbStats->stats.nonPersistent.sch[RG_SCH_CELLINDEX(sCell)].dlAckNackCnt[tbCnt]++;
         ue->tenbStats->stats.nonPersistent.sch[RG_SCH_CELLINDEX(sCell)].dlNackCnt[tbCnt] ++;
#endif
      }
      /* Added Dl TB count for NACKED data*/
#ifdef LTE_L2_MEAS
      if(hqP->tbInfo[tbCnt].txCntr == 1)
      {   
         cell->dlUlTbCnt.tbTransDlFaulty++;
      }   
#endif         
   }
   else
   {
      RGSCHDBGINFONEW(inst,(rgSchPBuf(inst),"<HARQ> HqP[%d:%d] DTXED UE(%d)\n",
               hqP->procId, tbCnt,hqP->hqE->ue->ueId));
      hqP->tbInfo[tbCnt].dtxCount += 1;  /* DTX Counter*/

#ifdef TENB_STATS
      sCell->tenbStats->sch.dlDtx[tbCnt]\
         [hqP->tbInfo[tbCnt].dlGrnt.rv]++;
      if (ue)
      {
         ue->tenbStats->stats.nonPersistent.sch[RG_SCH_CELLINDEX(sCell)].dlDtxCnt[tbCnt]++;
      }
#endif


      /* Added Dl TB count for DTXED data*/
#ifdef LTE_L2_MEAS
      if(hqP->tbInfo[tbCnt].txCntr == 1)
      {   
         cell->dlUlTbCnt.tbTransDlFaulty++;
      }
#endif         
   }

   /* Check if this is repeating UE */
   if (hqP->tbInfo[tbCnt].fbkRepCntr != 0)
   {
      if((rgSCHDhmProcHqFdbkAckNackRep(hqP,sf,tbCnt,&isAck)) != ROK)
      {
         return (ret);
      }
   }
   else
   {
     /* For a Normal UE take the decision here */
     hqP->tbInfo[tbCnt].isAckNackDtx = isAck;
     {
         rgSCHUtlDlHqPTbRmvFrmTx(sf, hqP, tbCnt, FALSE);
     }
   }
   /* Process either the ACK received or max retries have occurred */
   /* Assuming for Repetition that 2 ACKs and 2 NACKs make an NACK */
   if (TFU_HQFDB_ACK == isAck)
   {
      if (isMsg4 == TRUE) 
      {
         /* SR_RACH_STATS : MSG4 ACK*/
         rgNumMsg4Ack++;

         if (raCb == NULLP)
         {
            raCb = rgSCHDbmGetRaCb(cell, rnti);
         }
         DU_LOG("\nDEBUG  -->  SCH: Ack Rcvd. FdbkInd for Msg4Done\n");
         /* Inform Random Access Module regarding the ack received */
         if (raCb != NULLP)
         {
            /*RRC Connection Setup failure issue where RRC connection 
             * setup was not reaching UE due to message 4 HARQ failure */
            printf("\nDEBUG  -->  SCH: MSG4 Ack ,calling rgSCHRamMsg4Done\n");
            ret = rgSCHRamMsg4Done(cell, raCb);
            hqFreed = TRUE;
         }
         else
         {
            printf("\nDEBUG  -->  SCH: raCb is NULLP\n");
         }
      }
      else /*ccpu00114124- HARQ Release for Msg4 */
      {
         RGSCH_NULL_CHECK(cell->instIdx, ueDl);
         /* Push this harq process back to the free queue */
         ueDl->mimoInfo.cwInfo[tbCnt].ackCnt++;
#ifdef DL_LA
         if(hqP->tbInfo[tbCnt].txCntr == 1)
         {
            rgSCHDhmUpdateAckNackHistory(sCell, ue, isAck, tbCnt);
         } 
#endif
         RGSCH_NULL_CHECK(cell->instIdx, ue);
         RG_UPD_ACQI_TRIG_WT(ue, sCell,isAck);
         rgSCHDhmRlsHqpTb(hqP, tbCnt, TRUE);
      }
      hqRls = TRUE;
   }
   else
   {
      {
         if(!isMsg4)
         {
            RGSCH_NULL_CHECK(cell->instIdx, ueDl);
            ueDl->mimoInfo.cwInfo[tbCnt].nackCnt++;
#ifdef DL_LA
            if(hqP->tbInfo[tbCnt].txCntr == 1)
            {
               rgSCHDhmUpdateAckNackHistory(sCell, ue, isAck, tbCnt);
            }
#endif
            RGSCH_NULL_CHECK(cell->instIdx, ue);
            RG_UPD_ACQI_TRIG_WT(ue, sCell, isAck);
         }
         else
         { 
#ifdef XEON_SPECIFIC_CHANGES
            CM_LOG_DEBUG(CM_LOG_ID_SCH,"Msg4 Harq FAILURE for UE(%d)\n", rnti);
#endif
            rgNumMsg4Nack++;
         }
         rgSCHDhmHqTbTrnsFail(cell, hqP, tbCnt, &hqRls);
         maxHqRetxReached = hqRls;
      }
   }

   if(hqRls == TRUE)
   {
      /* MS_WORKAROUND: to increase Harq Fail Counter .
         The status field is required for tracking the number of harq faliures at MAC*/
      if (isAck)
      {
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].status[\
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs] = TRUE;
      }
#ifdef LTE_L2_MEAS
      else if(maxHqRetxReached)
      {
         /* this is to differentiat the NACK with data loss used for UU loss L2 meas */
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].status[\
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs] = 0xFF; /* RGU_NACK_LOSS; */
      }
#endif
      else
      {
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].status[\
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs] = FALSE;
      }
      rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].tbId[\
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs] = tbCnt + 1;
      rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs++;
   }

   /* Handle the TA */
   if (hqFreed == FALSE && hqP->tbInfo[tbCnt].taSnt == TRUE)
   {
      rgSCHDhmFdbkIndHndlTa(hqP, tbCnt, isAck, maxHqRetxReached);
   }
   return (ret);
}  /* rgSCHDhmPrcFdbkForTb */
/** * @brief Function to decode the position of HarqFb for eachCell.
 *
 * @details
 *
 *     Function : rgSchGetHqFdbkPos
 *           
 *  @param[in]  RgSchCellCb          *cell,
 *  @param[in]  RgSchUeCb            *ue,
 *  @param[in]  RgSchDlHqProcCb      *hqP,
 *  @param[in]  RgrSchFrmt1b3TypEnum uciFrmtTyp, 
 *  @param[in]  Bool                 *isAck, 
 *  @param[in]  RgTfuHqInfo          *fdbk,
 *  @return     void
 **/
Void rgSchGetHqFdbkPos
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
RgSchDlHqProcCb      *hqP,
RgrSchFrmt1b3TypEnum uciFrmtTyp, 
uint8_t              *isAck, 
RgTfuHqInfo          *fdbk
)
{
   if(uciFrmtTyp != RG_SCH_UCI_FORMAT1B_CS)
   {
      isAck[0] = fdbk->isAck[0];
      isAck[1] = fdbk->isAck[1];
      return;
   }
#ifdef LTE_ADV
   /* LAA Making all ack for LAA CELL */
   //if (hqP->hqE && rgSCHLaaSCellEnabled(hqP->hqE->cell))
   if (0)
   {
      isAck[0] = TRUE;
      isAck[1] = TRUE;
      return;
   }

   if((ue != NULLP))
   {
      /* PUSCH:: Fdbks are in the increasing order
       * of servCellIdx as per 36.212 section 5.2.26*/
      switch(ue->f1bCsAVal)
      {/* A Value */
         case RG_SCH_A_VAL_2:
            {
               if(RG_SCH_IS_CELL_SEC(ue,hqP->hqE->cell)) 
               {
                  isAck[0] = fdbk->isAck[1];/*SCell*/
                  isAck[1] = fdbk->isAck[1];/*SCell*/
               }
               else
               {
                  isAck[0] = fdbk->isAck[0];/*PCell*/
                  isAck[1] = fdbk->isAck[0];/*PCell*/
               }
               break;
            }
         case RG_SCH_A_VAL_3:
            {
               if(RG_SCH_IS_CELL_SEC(ue,hqP->hqE->cell)) 
               {
#ifdef LTE_ADV
                  uint8_t servCellIdx = rgSchUtlGetServCellIdx(hqP->hqE->cell->instIdx,
                        hqP->hqE->cell->cellId,
                        ue);

                  if(rgSCHUtlGetMaxTbSupp(ue->cellInfo[servCellIdx]->txMode.txModeEnum) > 1)
#else
                     if(rgSCHUtlGetMaxTbSupp(ue->cellInfo[RGSCH_PCELL_INDEX]->txMode.txModeEnum) > 1)
#endif
                     {  /* Sec cell is in mimo mode */
                        /* use 0 and 1 for sec in case of pucch
                         * and 1 and 2 in case of PUSCH as the primary cell is in
                         * siso case as A =3 */
                        if(!fdbk->isPusch)
                        {
                           isAck[0] = fdbk->isAck[0];
                           isAck[1] = fdbk->isAck[1];
                        }else
                        {/* PUSCH as per 36.212 serction 5.2.26*/
                           isAck[0] = fdbk->isAck[1];
                           isAck[1] = fdbk->isAck[2];
                        }
                     }else
                     {/* sec cell is in siso */
                        isAck[0] = fdbk->isAck[2];
                     }
               }else
               {
                  if(rgSCHUtlGetMaxTbSupp(ue->mimoInfo.txMode) > 1)
                  {/* primay cell is in mimo
                      use 0 and 1 */
                     isAck[0] = fdbk->isAck[0];
                     isAck[1] = fdbk->isAck[1];
                  }else
                  {
                     if(!fdbk->isPusch)
                     {
                        isAck[0] = fdbk->isAck[2];
                     }else
                     {/* PUSCH as per 36.212 serction 5.2.26*/
                        isAck[0] = fdbk->isAck[0];
                     }
                  }
               }
               break;
            }
         case RG_SCH_A_VAL_4:
            {
               if(RG_SCH_IS_CELL_SEC(ue,hqP->hqE->cell)) 
               {
                  isAck[0] = fdbk->isAck[2];
                  isAck[1] = fdbk->isAck[3];
#ifdef CA_DBG
                  {
                     if(isAck[0] == TFU_HQFDB_ACK)
                     {
                        gSCellTb1AckCount++;
                     }else if(isAck[0] == TFU_HQFDB_NACK)
                     {
                        gSCellTb1NackCount++;
                     }else
                     {
                        gSCellTb1DtxCount++;
                     }

                     if(isAck[1] == TFU_HQFDB_ACK)
                     {
                        gSCellTb2AckCount++;
                     }else if(isAck[1] == TFU_HQFDB_NACK)
                     {
                        gSCellTb2NackCount++;
                     }else
                     {
                        gSCellTb2DtxCount++;
                     }

                  }
#endif
               }
               else
               {
                  isAck[0] = fdbk->isAck[0];
                  isAck[1] = fdbk->isAck[1];
#ifdef CA_DBG
                  {
                     if(isAck[0] == TFU_HQFDB_ACK)
                     {
                        gPCellTb1AckCount++;
                     }else if(isAck[0] == TFU_HQFDB_NACK)
                     {
                        gPCellTb1NackCount++;
                     }else
                     {
                        gPCellTb1DtxCount++;
                     }

                     if(isAck[1] == TFU_HQFDB_ACK)
                     {
                        gPCellTb2AckCount++;
                     }else if(isAck[1] == TFU_HQFDB_NACK)
                     {
                        gPCellTb2NackCount++;
                     }else
                     {
                        gPCellTb2DtxCount++;
                     }

                  }
#endif

               }
               break;
            }
         default:
            break;
      }
   }
#endif
   return;
}/* End of rgSchGetHqFdbkPos */
#ifdef LTE_ADV
Void rgSchGetHqFdbkPosFormat3
(
RgSchDlHqProcCb *hqP,
uint8_t         *isAck,
TfuHqFdbk       *fdbk
)
{
   uint8_t cellIdx = RG_SCH_CMN_GET_CELL_IDX_FROM_HQP(hqP);
   isAck[0] = (uint8_t)fdbk[cellIdx]; 
   isAck[1] = (uint8_t)fdbk[cellIdx + 1]; 
}
#endif
/** * @brief Handler for HARQ feedback received for DL transmission.
 *
 * @details
 *
 *     Function : rgSCHDhm5gtfHqFdbkInd 
 *     
 *     This function shall act on the feedback received from TOM for DL
 *     transmission. If the feedback for msg4 is final (after max transmissions
 *     or ACK) inform RAM that Msg4 transmission is done. 
 *     
 *           
 *  @param[in]  Void                *cb
 *  @param[in]  uint8_t                  cbType
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgTfuHarqAckIndInfo *fdbk
 *  @param[in]  RgInfRlsHqInfo      *rlsHqBufs
 *  @param[in]  RgSchErrInfo        *err
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHDhm5gtfHqFdbkInd 
(
RgSchUeCb            *ue,
RgSchCellCb          *cell,
CmLteTimingInfo      timingInfo,
TfuHqFdbk            fdbk,
RgSchErrInfo         *err
)
{
   RgSchDlHqProcCb  *hqP = NULLP;   
   CmLList          *node = NULLP;
   CmLListCp        *lnk; 
   S16              ret = ROK;
   RgSchDlSf        *sf;
   Bool             isMsg4 = FALSE;
   uint16_t         rnti=0;
   uint16_t         procId=0;
   uint8_t          hqPCount = 0;
   RgInfRlsHqInfo   *rlsHqBufs = NULLP;

   RGSCHDECRFRMCRNTTIME(timingInfo, timingInfo, 4); 

   sf = rgSCHUtlSubFrmGet(cell, timingInfo);

   lnk      = &ue->dl.dlSfHqInfo[cell->cellId][sf->dlIdx].hqPLst;
   node     = lnk->first; 
   hqPCount = lnk->count;
   rnti     = ue->ueId;

   while (hqPCount)
   {
      hqP = (RgSchDlHqProcCb *)node->node;
      node = node->next;
      rlsHqBufs = &(hqP->hqE->cell->rlsHqArr[hqP->hqE->cell->crntHqIdx]);
      procId = hqP->procId;

      rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs = 0;

      if (HQ_TB_WAITING == hqP->tbInfo[0].state)
      {
         rgSCHDhmPrcFdbkForTb(cell, ue, hqP, sf, isMsg4, rnti, 0, 
            timingInfo, fdbk, rlsHqBufs, err);
      }
      if(rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs)
      {
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].rnti = rnti;
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].hqProcId = 
            (uint8_t)procId;
         rlsHqBufs->numUes++;
      }
      hqPCount--;
   }

   return (ret);
}  /* rgSCHDhm5gtfHqFdbkInd */

/** * @brief Handler for HARQ feedback received for DL transmission.
 *
 * @details
 *
 *     Function : rgSCHDhmHqFdbkInd
 *     
 *     This function shall act on the feedback received from TOM for DL
 *     transmission. If the feedback for msg4 is final (after max transmissions
 *     or ACK) inform RAM that Msg4 transmission is done. 
 *     
 *           
 *  @param[in]  Void                *cb
 *  @param[in]  uint8_t                  cbType
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgTfuHarqAckIndInfo *fdbk
 *  @param[in]  RgInfRlsHqInfo      *rlsHqBufs
 *  @param[in]  RgSchErrInfo        *err
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHDhmHqFdbkInd
(
Void                 *cb, 
uint8_t              cbType,
RgSchCellCb          *cell,
CmLteTimingInfo      timingInfo,
RgTfuHqInfo          *fdbk,
RgInfRlsHqInfo       *rlsHqBufs,
RgSchErrInfo         *err
)
{
   RgSchDlHqTbCb        *tbCb;
   RgSchDlHqEnt         *hqE = NULLP;
   RgSchDlHqProcCb      *hqP = NULLP;   
   CmLList              *node = NULLP;
   CmLListCp            *lnk; 
  /* Create and Initialize Ue it so that Its not Deferenced Unnecessarily */
   RgSchUeCb            *ue = NULLP;

   S16                  ret = ROK;
   RgSchDlSf            *sf;
   Bool                 isMsg4 = FALSE;
   RgSchRaCb            *raCb = NULLP;
   uint16_t             rnti=0;
  /* Added Insure Fixes Of UR.Initialized procId  */
   uint16_t             procId=0;
   /* DTX Change: Bool is converted into uint8_t*/
   uint8_t              isAck[2]={0}; /*Changed to Array of 2*/
   uint8_t              tbCnt;
   uint8_t              hqPCount = 0;

#ifdef LTEMAC_SPS
   CmLteTimingInfo      fdbkRcptTime = timingInfo;
#ifdef RGSCH_SPS_STATS
   RgSchCmnDlHqProc        *cmnHqDl;
#endif
#endif
#ifdef LTE_ADV
   TfuHqFdbk            format3Ack[CM_LTE_MAX_CELLS *2] = {0};
#endif
   RgrSchFrmt1b3TypEnum uciFrmtTyp = RG_SCH_UCI_FORMAT1A_1B;

   /* Get the subframe associated with the feedback */
   /* ccpu00133109: Removed RGSCHSUBFRMCRNTTIME as it is not giving proper output 
    * if diff is more than 10. Instead using RGSCHDECRFRMCRNTTIME() as it is 
    * serving the purpose */
   RGSCHDECRFRMCRNTTIME(timingInfo, timingInfo, 4); 

   sf = rgSCHUtlSubFrmGet(cell, timingInfo);
   if (cbType == RGSCH_HQ_FDB_IND_CB_TYPE_RA_CB)
   {
      raCb     = (RgSchRaCb *)(cb);
      hqE      = raCb->dlHqE;
      hqP      = rgSCHDhmHqProcByTime(hqE, timingInfo, &isMsg4,\
                                      sf);
      if(hqP)
      {
         hqPCount = 1;
      }
      rnti     = raCb->tmpCrnti;
   }
   else
   {
      ue       = (RgSchUeCb *)(cb);
      hqE      = RG_SCH_CMN_GET_UE_HQE(ue, cell);
      {
         lnk      = &ue->dl.dlSfHqInfo[cell->cellId][sf->dlIdx].hqPLst;
      }
      node     = lnk->first; 
      hqPCount = lnk->count;
      rnti     = ue->ueId;
#ifdef LTE_ADV
      uciFrmtTyp = ue->dl.dlSfHqInfo[cell->cellId][sf->dlIdx].uciFrmtTyp;
#endif
   }
   /*
      TO ADD STATS 
      from Harq Proc get ueCb = hqP->hqEnt->ueCb
      from ueCb get cmnUecb = (RgSchCmnUe *)ueCb->sch;
      from ueCb get dlUe = (RgSchCmnDlUe)cmnUeCb->dl
      from get cmInfo "RgSchCmnDlUeCwInfo" dlUe->mimoInfo->cwInfo[0] 
      from get CQI from cmInfo->cqi 
      from cmInfo get iTbs cmInfo->iTbs[0]
      call RG_SCH_CMN_DL_TBS_TO_MCS to map iTbs=>MCS 
      Update stats in cellCb
      cellCb->hqFailStats[cmInfo->cqi].mcs = RG_SCH_CMN_DL_TBS_TO_MCS(cmInfo->iTbs[0]);
      if (fdbk->isAck == TRUE)
      cellCb->hqFailStats[cmInfo->cqi].numOfNacks += 1;
      else
      cellCb->hqFailStats[cmInfo->cqi].numOfAcks += 1;
      DL Ack/Nack statistics
    */   
#ifdef MAC_SCH_STATS
   if (hqE->ue != NULLP)
   {
      RgSchUeCb      *ueCb  = hqE->ue;
      RgSchCmnUe     *cmnUe = (RgSchCmnUe*)ueCb->sch;
      RgSchCmnDlUe   *dlUe  = RG_SCH_CMN_GET_DL_UE(ueCb,hqE->cell);/*CA dev*/
      uint8_t             tbs    = dlUe->mimoInfo.cwInfo[0].iTbs[0];
      static uint32_t retxCnt    = 0;

      if (fdbk->isAck[0] == TFU_HQFDB_ACK)
      {
         hqFailStats.dlCqiStat[(dlUe->mimoInfo.cwInfo[0].cqi - 1)].numOfAcks++;
      }
      else
      {
         ++retxCnt;
         hqFailStats.dlCqiStat[(dlUe->mimoInfo.cwInfo[0].cqi - 1)].numOfNacks++;
      }
      RG_SCH_CMN_DL_TBS_TO_MCS(tbs, 
               (hqFailStats.dlCqiStat[(dlUe->mimoInfo.cwInfo[0].cqi - 1)].mcs));
   }
#endif /* MAC_SCH_STATS */

   /* Fetch the harqProc from the inUse list */
#ifdef LTEMAC_SPS
   /* Check if the feedback timing matches with ue->relPdcchFbkTiming*/
   /* Call Common module with the feedback information */
   if (ue && (ue->relPdcchFbkTiming.sfn != (RGSCH_MAX_SFN + 1)))
   {
      if (RGSCH_TIMEINFO_SAME(fdbkRcptTime, ue->relPdcchFbkTiming))
      {
         sf = rgSCHUtlSubFrmGet(cell, timingInfo);

#ifdef LTE_ADV
         if(uciFrmtTyp == RG_SCH_UCI_FORMAT1B_CS)
         {/* Feedback for SPS Release on PCell
             If Pcell is in mimo, feedback index will be 0
             else 2 */
            if(rgSCHUtlGetMaxTbSupp(ue->mimoInfo.txMode) > 1)
            {
               isAck[0] = fdbk->isAck[0];
            }else
            {
               isAck[0] = fdbk->isAck[2];
            }

            /* Not releasing pdcch here
             * as it is already done at the time of
             * reception req */
            rgSCHUtlDlRelPdcchFbk(cell, ue, isAck[0]);
         }
         else
#endif
         {
            if (!sf->relPdcch)
            {
               DU_LOG("\nERROR  -->  SCH : CRNTI:%d NO HARQ proc available for feedback: TimingInfo: "
                  "sfn %d slot %d", ue->ueId, timingInfo.sfn,
                  timingInfo.slot);
               return RFAILED;
            }

            isAck[0] = fdbk->isAck[0];
            /* Note: Since relPdcchFbkTimimg matches with the recieved
             * feedback, assumed that feedback is for release PDCCH */
            rgSCHUtlDlRelPdcchFbk(cell, ue, isAck[0]);

            /* Remove release PDCCH from the subframe */
            rgSCHUtlPdcchPut(cell, &sf->pdcchInfo, sf->relPdcch);
            sf->relPdcch = NULLP;
            return ROK;
         }
      }
   }
#endif /* LTEMAC_SPS */
   
   /* Remove the harq process from the subframe */
   sf = rgSCHUtlSubFrmGet(cell, timingInfo);
   RG_SCH_ADD_TO_CRNT_TIME(timingInfo, timingInfo, 1);
   
#ifdef CA_DBG
   {
      if(ue)
      {
         gHqFdbkCount++;
      }
   }

#endif   
   while (hqPCount)
   {
      if(cbType != RGSCH_HQ_FDB_IND_CB_TYPE_RA_CB)
      {
         hqP = (RgSchDlHqProcCb *)node->node;
         node = node->next;
         rlsHqBufs = &(hqP->hqE->cell->rlsHqArr[hqP->hqE->cell->crntHqIdx]);
      }
      procId = hqP->procId;

      rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs = 0;

      /*Get the position of Ack/Nack from 2 bytes fdbkInfo.
       * On the basis of f1bCsAVal find the position of iAck or Nack*/
#ifdef LTE_ADV
      if (uciFrmtTyp == RG_SCH_UCI_FORMAT3)
      {
         rgSchGetHqFdbkPosFormat3(hqP,isAck,format3Ack); 
      }
      else
#endif
      {
         rgSchGetHqFdbkPos(cell,ue,hqP, uciFrmtTyp, isAck,fdbk); 
      }
      for (tbCnt = 0; tbCnt < 2; tbCnt++)
      {
         if (HQ_TB_WAITING == hqP->tbInfo[tbCnt].state)
         {
            rgSCHDhmPrcFdbkForTb(cell, ue, hqP, sf, isMsg4, rnti, tbCnt, 
                  timingInfo, isAck[tbCnt], rlsHqBufs, err);
         }
      }
      if(rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs)
      {
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].rnti = rnti;
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].hqProcId = 
            (uint8_t)procId;
         rlsHqBufs->numUes++;
      }
      hqPCount--;
   }

   node = sf->ackNakRepQ.first;
   while (node)
   {
      tbCb = (RgSchDlHqTbCb *)(node->node);
      hqP = tbCb->hqP;

      procId = hqP->procId;
      rlsHqBufs = &(hqP->hqE->cell->rlsHqArr[hqP->hqE->cell->crntHqIdx]);
      rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs = 0;
      if (HQ_TB_WAITING == tbCb->state)
      {
         isAck[0] = fdbk->isAck[tbCb->tbIdx];
         rgSCHDhmPrcFdbkForTb(cell, ue, hqP, sf, isMsg4, rnti, tbCb->tbIdx,
               timingInfo, isAck[0], rlsHqBufs, err);
      }
      hqP->cwSwpEnabled = FALSE;
      if(rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs)
      {
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].rnti = rnti;
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].hqProcId = 
            (uint8_t)procId;
         rlsHqBufs->numUes++;
      }
      hqPCount--;
      node = node->next;
   }

   return (ret);
}  /* rgSCHDhmHqFdbkInd */
#endif /* LTE_FDD */


/**
 * @brief Handler for Harq related UE configuration.
 *
 * @details
 *
 *     Function : rgSCHDhmRgrUeCfg
 *     
 *     This function shall fetch the harq related information into the
 *     respective ueCb from the UE configuration as provided by the 
 *     upper layers. 
 *           
 *  @param[in]  RgSchCellCb   *cell 
 *  @param[in]  RgSchUeCb     *ueCb 
 *  @param[in]  RgrUeCfg      *ueCfg 
 *  @param[out] RgSchErrInfo     *err
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
Void rgSCHDhmRgrUeCfg(RgSchCellCb *cell,RgSchUeCb *ueCb,RgrUeCfg *ueCfg,RgSchErrInfo *err)
{

   UNUSED(err);

   /* Initialize the TA Timer */
   cmInitTimers(&ueCb->taTmr, 1);

   /* Setting these values irrespective of taTmr value */
   ueCb->dl.taCb.state = RGSCH_TA_IDLE;
   /* Corrected default value of TA as per 36.213, 4.2.3 */
   ueCb->dl.taCb.ta = RGSCH_NO_TA_RQD;
  
   /*[ccpu00121813]-ADD-Initializing outstanding TA value */
   ueCb->dl.taCb.outStndngTa = FALSE;
   ueCb->dl.taCb.outStndngTaval = RGSCH_NO_TA_RQD;
   
   /* Start TA timer only if cfgd as FINITE value */
   if (ueCfg->ueTaTmrCfg.pres)
   {
      /* Configuring taTmr with 30 deficit, to enable eNodeB sending
       * TA command before the expiry of TA at UE. Also considering for 
       * possible retx for this TA command */
      /*[ccpu00121813]-ADD-Added chk if tatmr val > 30 */
      if(ueCfg->ueTaTmrCfg.taTmr > 30)
      {
         ueCb->dl.taCb.cfgTaTmr = ueCfg->ueTaTmrCfg.taTmr - 30; 
      }
      rgSCHTmrStartTmr (cell, ueCb, RG_SCH_TMR_TA, ueCb->dl.taCb.cfgTaTmr);
   }
   return;
}  /* rgSCHDhmRgrUeCfg */


/**
 * @brief Handler for HARQ related UE Reconfiguration
 *
 * @details
 *
 *     Function : rgSCHDhmRgrCellCfg
 *     
 *     This function shall fetch the HARQ related information into the
 *     respective ueCb from the UE configuration as provided by the 
 *     upper layers. 
 *           
 *  @param[in]  RgSchCellCb     *cell 
 *  @param[in]  RgrCellCfg   *cellCfg
 *  @param[out] RgSchErrInfo    *err 
 *
 *  @return     S16
 *                -# ROK 
 *                -# RFAILED 
 **/
Void rgSCHDhmRgrCellCfg(RgSchCellCb *cell,RgrCellCfg  *cellCfg,RgSchErrInfo *err)
{
   RgSchDlHqEnt *hqE;
   PTR         pUeCb;/* previous UE Control block */
   PTR         nUeCb;/* next UE control block */
   S16         ret;
   uint8_t     idx;

   UNUSED(err);

   pUeCb = NULLP;
   
   cell->dlHqCfg = cellCfg->dlHqCfg;
   for (;;)
   {
      ret = cmHashListGetNext(&(cell->ueLst), pUeCb, &nUeCb);
      if (ret != ROK)
      {
         break;
      }
      else
      {
         pUeCb = nUeCb;
         /* Update the DL Harq related information */
         hqE   = RG_SCH_CMN_GET_UE_HQE(((RgSchUeCb*)nUeCb), cell);
         hqE->maxHqTx = cell->dlHqCfg.maxDlHqTx;
      }
   }
   /* Initializing the list for ueCbs that would have ta */
   cmLListInit(&cell->taUeLst);
#ifdef RGR_V1
   cmLListInit(&cell->ccchSduUeLst);
   cmLListInit(&cell->contResGrdTmrLst);
   cmLListInit(&cell->contResTmrLst);
#ifdef EMTC_ENABLE
   if(cell->emtcEnable)
   {
      rgSCHDhmEmtcRgrCellCfg(cell);
   }
#endif
#endif

   /* Initializing the timer queue */
   cell->tqCp.nxtEnt = 0;
   cell->tqCp.tmrLen = RGSCH_UE_TQ_SIZE;

   for (idx = 0; idx < RGSCH_UE_TQ_SIZE; idx++)
   {
     cell->tq[idx].first = NULLP;
     cell->tq[idx].tail  = NULLP;
   }
   return;
}  /* rgSCHDhmRgrCellCfg */

/**
 * @brief Handler for Updating HARQ Information from Cell Reconfiguration 
 *
 * @details
 *
 *     Function : rgSCHDhmRgrCellRecfg
 *     
 *     This function shall fetch the HARQ related information into the
 *     respective ueCb from the UE configuration as provided by the 
 *     upper layers. 
 *           
 *  @param[in]  RgSchCellCb     *cell 
 *  @param[in]  RgrCellRecfg *cellRecfg
 *  @param[out] RgSchErrInfo    *err 
 *
 *  @return     S16
 *                -# ROK 
 *                -# RFAILED 
 **/
Void rgSCHDhmRgrCellRecfg(RgSchCellCb *cell,RgrCellRecfg *cellRecfg,RgSchErrInfo *err)
{
   RgSchDlHqEnt         *hqE;
   PTR         pUeCb;/* previous UE Control block */
   PTR         nUeCb;/* next UE control block */
   S16         ret;

   UNUSED(err);

   pUeCb = NULLP;
   
   /* Update the cell with recieved configuration */
   if (cellRecfg->recfgTypes & RGR_CELL_DL_HARQ_RECFG)
   {
      cell->dlHqCfg  =  cellRecfg->dlHqRecfg;

      for (;;)
      {
         ret = cmHashListGetNext(&(cell->ueLst), pUeCb, &nUeCb);
         if (ret != ROK)
         {
            break;
         }
         else
         {
            pUeCb = nUeCb;
            /* Update the DL Harq related information */
            hqE      = RG_SCH_CMN_GET_UE_HQE(((RgSchUeCb*)nUeCb), cell);
            hqE->maxHqTx = cell->dlHqCfg.maxDlHqTx;
         }
      }
   }
   return;
}  /* rgSCHDhmRgrCellRecfg */

/**
 * @brief Handler for freeing up the HARQ related information from ueCb
 *
 * @details
 *
 *     Function : rgSCHDhmFreeUe
 *     
 *     This function shall free up the HARQ specific information from ueCb.
 *           
 *  @param[in]  RgSchUeCb     *ueCb 
 *
 *  @return     None.
 *
 **/
Void rgSCHDhmFreeUe(RgSchUeCb *ueCb)
{

   /* If TA Timer is running. Stop it */
   if (ueCb->taTmr.tmrEvnt != TMR_NONE)
   {
      rgSCHTmrStopTmr(ueCb->cell, ueCb->taTmr.tmrEvnt, ueCb);
   }

   /* ccpu00118357 - ADD - stop the periodic BSR timer so it 
    * doesn't expire after UE is deleted */
#ifdef RGR_V1
   if (ueCb->bsrTmr.tmrEvnt != TMR_NONE)
   {
      rgSCHTmrStopTmr(ueCb->cell, ueCb->bsrTmr.tmrEvnt, ueCb);
   }
#endif /* ifdef RGR_V1*/

   
   if (RG_SCH_CMN_GET_UE_HQE(ueCb, ueCb->cell))
   {
      rgSCHDhmDelHqEnt(ueCb->cell, &(RG_SCH_CMN_GET_UE_HQE(ueCb, ueCb->cell)));
   }

   /* This UE needs to be removed from its entry into cell's taUeLst */
   /*Fix for ccpu00113622 - Delete Only when taLnk Node exists*/
   if(ueCb->taLnk.node)
   {
      cmLListDelFrm(&(ueCb->cell->taUeLst), &ueCb->taLnk);
      ueCb->taLnk.node = NULLP; 
   }

   if (ueCb->dlTaLnk.node != NULLP)
   {
      /* Fix: syed Need to raise a CR for not calling CMN or specific scheduler
       * function directly from other modules. APIs should be defined and/or used
       * instead. Please check for other possible incorrect usage. */      
      rgSCHCmnRmvFrmTaLst(ueCb->cell, ueCb);
   }

   return;

}  /* rgSCHDhmFreeUe */

/**
 * @brief Handler for updating the TA.
 *
 * @details
 *
 *     Function : rgSCHDhmUpdTa
 *     
 *     This function shall update the TA received. 
 *           
 *  @param[in]  RgSchCellCb   *cell 
 *  @param[in]  RgSchUeCb     *ueCb 
 *  @param[in]  uint8_t         ta
 *
 *  @return     None.
 *
 **/
Void rgSCHDhmUpdTa(RgSchCellCb *cell,RgSchUeCb *ueCb,uint8_t ta)
{

   if (ueCb->dl.taCb.state == RGSCH_TA_IDLE)
   {
      ueCb->dl.taCb.state     =  RGSCH_TA_TOBE_SCHEDULED;
      ueCb->dl.taCb.numRemSf  =  2;
      rgSCHUtlDlTARpt(cell, ueCb);  
      /* If TA Timer is running. Stop it */
      if (ueCb->taTmr.tmrEvnt != TMR_NONE)
      {
         rgSCHTmrStopTmr(cell, ueCb->taTmr.tmrEvnt, ueCb);
      }
	  
	  /* SR_RACH_STATS : TA MODIFIED */
      if (ueCb->dl.taCb.ta != ta)
      {
         rgNumTAModified++;
      }
      ueCb->dl.taCb.ta        =  ta;
   }
   else
   {
      /* [ccpu00121813]-ADD-Updating outstanding values 
      * TA which gets transmitted at N gets applied at UE at N+6,once TA
      * has been scheduled,further TA values get stored in outstndngTaval.
      * Once TA gets applied at UE or when NACK/DTX is rcvd for maxhqretx times
      * then schedule the outstanding TA val if present */
      ueCb->dl.taCb.outStndngTa    =  TRUE;
      ueCb->dl.taCb.outStndngTaval =  ta;
   }

   return;
}  /* rgSCHDhmUpdTa */

 /** @brief This function handles the TA timer expiry.
  *
  * @details
  *
  *     Function: This function handled the TA Expiry.
  *
  *         Processing steps:
  *         - 
  *
  *
  * @param[in] RgSchUeCb  *ueCb  
  *
  * @return Void
  *      -#None.
  */
Void rgSCHDhmProcTAExp( RgSchUeCb  *ueCb)
{
   /* Ask scheduler to schedule this UE */
   ueCb->dl.taCb.state     =  RGSCH_TA_TOBE_SCHEDULED;
   rgSCHUtlDlTARpt(ueCb->cell, ueCb);  
   return;
} /* end of rgSCHDhmProcTAExp */ 

/* 3.1 MIMO: LC details at TB level rather than Hq Level */
/**
 * @brief Handler for Adding scheduled logical channel data information  
 *        to harqProc.
 *                    
 * @details
 *
 *     Function : rgSCHDhmAddLcData
 *     
 *     This function shall add the scheduled logical channel data 
 *     information to the HARQ process. 
 *           
 *  @param[in]  RgSchLchAllocInfo  *lchData
 *  @param[in]  RgSchDlHqTbCb     *tbInfo
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHDhmAddLcData(Inst inst,RgSchLchAllocInfo  *lchData,RgSchDlHqTbCb *tbInfo)
{

   if(tbInfo->numLch >= RGSCH_MAX_NUM_DED_LC)
   {
     return RFAILED;
   } 

   tbInfo->lchSchdDataArr[tbInfo->numLch]   = *lchData;

   tbInfo->numLch++;

   return ROK;

}  /* rgSCHDhmAddLcData */

#ifdef LTE_TDD
/*
 * @brief Handler for releaseing the subframe allocation.
 *
 * @details
 *
 *     Function : rgSCHDhmTddRlsSubFrm
 *
 *     This function shall be invoked to release the DL Sf 
 *     allocations for which HARQ feedback time has expired.
 *
 *  @param[in]  RgSchCellCb      *cellCb
 *  @param[in]  CmLteTimingInfo  uciTimingInfo;
 *  @return     S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHDhmTddRlsSubFrm(RgSchCellCb *cellCb,CmLteTimingInfo uciTimingInfo)
{
   CmLteTimingInfo         dlSfTime;
   RgSchTddDlAscSetIdxK    ascIdx;
   uint8_t                 noFdbks;
   uint8_t                 i;

   ascIdx = 
      rgSchTddDlAscSetIdxKTbl[cellCb->ulDlCfgIdx][uciTimingInfo.slot];
   noFdbks = ascIdx.numFdbkSubfrms;
   for(i=0; i < noFdbks; i++)
   {
      /* Get the subframe and sfn for which HARQ Ack/Nack
       * has to be sent */
      /* ccpu00132341-MOD- optimized getting DLSF time using macro*/
      /* ccpu00133109: Removed RGSCHSUBFRMCRNTTIME as it is not giving proper 
       * output if diff is more than 10. Instead using RGSCHDECRFRMCRNTTIME() 
       * as it is serving the purpose */
      RGSCHDECRFRMCRNTTIME(uciTimingInfo, dlSfTime, ascIdx.subfrmNum[i]);
      rgSCHUtlDlRlsSubFrm(cellCb, dlSfTime);
   }
   return ROK;
}/* rgSCHDhmTddRlsSubFrm */

#ifdef TFU_TDD
uint32_t macDtx = 0;
#endif
/**
 * @brief Handler for Removing the HARQ process from a dlsf.
 *
 * @details
 *
 *     Function : rgSCHDhmRlsDlsfHqProc
 *
 *     This function shall be invoked for every tti. It goes back to
 *     to the sixth last subframe to check whether it still exists. If
 *     that exists this function traverses through the entire harq
 *     proc list associated and frees up all of them.
 *
 *  @param[in]  RgSchCellCb      *cellCb
 *  @param[in]  CmLteTimingInfo  timingInfo
 *  @return     S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHDhmRlsDlsfHqProc(RgSchCellCb *cellCb,CmLteTimingInfo uciTimingInfo)
{
   RgSchDlSf               *dlSf;
   CmLteTimingInfo         dlSfTime;
   CmLteTimingInfo         nxtfrm = {0,0};
   RgSchDlHqProcCb         *tmpHqProc;
   RgSchTddDlAscSetIdxK    ascIdx;
   uint8_t                 noFdbks;
   S16                     i;
   RgSchDlSf               *nxtDlsf = NULLP;
   CmLList                 *node;
   CmLList                 *hqPNode;
   uint8_t                 idx;
   /*ccpu00130018 -MOD -Initiatizing with FALSE*/ 
   uint8_t                 maxRetx=FALSE;
   RgSchTddANInfo          *anInfo = NULLP;
   RgSchDlHqTbCb           *tbCb;
   RgSchUeCb               *ue = NULLP;

   ascIdx = 
      rgSchTddDlAscSetIdxKTbl[cellCb->ulDlCfgIdx][uciTimingInfo.slot];
   noFdbks = ascIdx.numFdbkSubfrms;
   for(i=0; i < noFdbks; i++)
   {
      /* Get the subframe and sfn for which HARQ Ack/Nack
       * has to be sent */
      /* ccpu00132341-MOD- optimized getting DLSF time using macro*/
      /* ccpu00133109: Removed RGSCHSUBFRMCRNTTIME as it is not giving proper 
       * output if diff is more than 10. Instead using RGSCHDECRFRMCRNTTIME() 
       * as it is serving the purpose */
      RGSCHDECRFRMCRNTTIME(uciTimingInfo, dlSfTime, ascIdx.subfrmNum[i]);

      dlSf = rgSCHUtlSubFrmGet (cellCb, dlSfTime);
      if(cellCb->ulDlCfgIdx != 5)
      {
         rgSCHUtlGetNxtDlSfInfo(dlSfTime, cellCb, dlSf, &nxtDlsf, &nxtfrm);
      }
      /* Subframe is present. Delete all the harq associations from 
       * this subframe.
       */

      /*Handling for Msg4*/
      node = dlSf->msg4HqPLst.first;
      while (node)
      {
         tmpHqProc = (RgSchDlHqProcCb *)(node->node);
         node = node->next;
         tmpHqProc->cwSwpEnabled = FALSE;
         if (HQ_TB_WAITING == tmpHqProc->tbInfo[0].state)
         {
            tbCb = &tmpHqProc->tbInfo[0];

            /* Fix : syed MultiUe per TTI crash in TA List. */
            maxRetx = FALSE;

            tbCb->dtxCount++;
            tbCb->isAckNackDtx = TFU_HQFDB_DTX;


            rgSCHUtlDlHqPTbRmvFrmTx(dlSf, tmpHqProc, tbCb->tbIdx, FALSE);

            /* Delete the Harq Association. Release the Harq Process */
            rgSCHDhmHqTbTrnsFail(cellCb, tmpHqProc, tbCb->tbIdx, &maxRetx);

            if (tbCb->taSnt == TRUE)
            {
               /* [ccpu00127148] Correcting the check */
               if (TRUE == maxRetx)
               {
                  tbCb->taSnt                  =  FALSE;
                  RGSCH_NULL_CHECK(cellCb->instIdx, ue)
                     ue->dl.taCb.state = RGSCH_TA_IDLE;

                  rgSCHUtlReTxTa(cellCb, ue);
               }
            }
         }
      } 

      node = dlSf->ueLst.first;
      while (node)
      {
#ifdef TFU_TDD
         macDtx++;
#endif
         ue = (RgSchUeCb *)(node->node);
         node = node->next;
         if (NULLP != ue)
         {
            hqPNode = ue->dl.dlSfHqInfo[cellCb->cellId][dlSf->dlIdx].hqPLst.first;
            while (hqPNode)
            {
               tmpHqProc = (RgSchDlHqProcCb *)hqPNode->node;
               hqPNode = hqPNode->next;
               for (idx = 0 ;idx < 2; idx++)
               {
                  if (HQ_TB_WAITING == tmpHqProc->tbInfo[idx].state)
                  {
                     tbCb = &tmpHqProc->tbInfo[idx];

                     /* Fix : syed MultiUe per TTI crash in TA List. */
                     maxRetx = FALSE;

                     tbCb->dtxCount++;
                     tbCb->isAckNackDtx = TFU_HQFDB_DTX;


                     /* Update feedback time for this process so that 
                      * next subframe its picked up */
#ifdef LTE_ADV
                     uint8_t servCellIdx = rgSchUtlGetServCellIdx(
                           tmpHqProc->hqE->cell->instIdx,
                           tmpHqProc->hqE->cell->cellId,
                           ue);
                     anInfo = rgSCHUtlGetUeANFdbkInfo(ue, &tbCb->fdbkTime,servCellIdx);
#else
                     anInfo = rgSCHUtlGetUeANFdbkInfo(ue, &tbCb->fdbkTime,RGSCH_PCELL_INDEX);
#endif
                     if(anInfo == NULLP)
                     {
                        RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx), 
                                 "Ack/Nack Info is NULL, Processing %dth feedback slot for DTX" 
                                 "received on SFN [%d] and SF [%d]\n",i, uciTimingInfo.sfn, 
                                 uciTimingInfo.slot));
                     }
                     else  if (tbCb->fbkRepCntr == 0)
                     {
                        /* Initialise the Ack/Nack feedback */
                        anInfo->dlDai--;
                        if(!(anInfo->dlDai))
                        {
                           rgSCHUtlInitUeANFdbkInfo(anInfo);
                        }
                     }
                     else
                     {
                        /* Update feedback time for this process so that
                         *              * next subframe its picked up */
                        RGSCH_NULL_CHECK(cellCb->instIdx, nxtDlsf); 
                        RGSCH_UPD_HQAN_FDBKTIME(tbCb, nxtDlsf, nxtfrm);
                        RGSCH_UPD_ANINFO_WITH_HQ(anInfo, tbCb);
                        rgSCHUtlDlHqPTbRmvFrmTx(dlSf, tmpHqProc, tbCb->tbIdx, TRUE);
                        tbCb->fbkRepCntr--;
                        continue;

                     }
                     rgSCHUtlDlHqPTbRmvFrmTx(dlSf, tmpHqProc, tbCb->tbIdx, FALSE);
                     /*ccpu000119494-ADD- for SPS, call SPS specific DTX handler */

                     {
                        /* Delete the Harq Association. Release the Harq Process */
                        rgSCHDhmHqTbTrnsFail(cellCb, tmpHqProc, tbCb->tbIdx, &maxRetx);
                     }
                     if (tbCb->taSnt == TRUE)
                     {
                        /* [ccpu00127148] Correcting the check */
                        if (TRUE == maxRetx)
                        {
                           tbCb->taSnt                  =  FALSE;
                           RGSCH_NULL_CHECK(cellCb->instIdx, ue)
                              ue->dl.taCb.state = RGSCH_TA_IDLE;

                           rgSCHUtlReTxTa(cellCb, ue);

                           DU_LOG("\nDEBUG  -->  SCH : Nack/DTX Rcvd for TA. Max Tries Attempted");
                        }
                     }
                  }
               }
            }
         }
      }

      node = dlSf->ackNakRepQ.first;
      while (node)
      {
         tbCb = (RgSchDlHqTbCb *)(node->node);
         tmpHqProc = tbCb->hqP;
         /* [ccpu00121813]-ADD-Fetch ueCb */
         ue = tmpHqProc->hqE->ue;
         /* Fix : syed MultiUe per TTI crash in TA List. */
         maxRetx = FALSE;
         
         tbCb->dtxCount++;
#ifdef TENB_STATS
         tmpHqProc->hqE->cell->tenbStats->sch.dlDtx[tbCb->tbIdx][tbCb->dlGrnt.rv]++;
         ue->tenbStats->stats.nonPersistent.sch[RG_SCH_CELLINDEX(tmpHqProc->hqE->cell)].dlDtxCnt[tbCb->tbIdx] ++;
#endif

         node = node->next;
         /* If This is not the last repetition */
         if (tbCb->fbkRepCntr > 1)
         {
            /* Update feedback time for this process so that 
             * next subframe its picked up */
#ifdef LTE_ADV
            uint8_t servCellIdx = rgSchUtlGetServCellIdx(
                  tmpHqProc->hqE->cell->instIdx,
                  tmpHqProc->hqE->cell->cellId,
                  ue);
            anInfo = rgSCHUtlGetUeANFdbkInfo(ue, &tbCb->fdbkTime,servCellIdx);
#else
            anInfo = rgSCHUtlGetUeANFdbkInfo(ue, &tbCb->fdbkTime,0);
#endif
            if(anInfo == NULLP)
            {
               return RFAILED;
            }
            RGSCH_NULL_CHECK(cellCb->instIdx, nxtDlsf);
            RGSCH_UPD_HQAN_FDBKTIME(tbCb, nxtDlsf, nxtfrm);
            RGSCH_UPD_ANINFO_WITH_HQ(anInfo, tbCb);
            rgSCHUtlDlHqPTbRmvFrmTx(dlSf,tmpHqProc,tbCb->tbIdx, TRUE);
            tbCb->fbkRepCntr--;
            continue;
         }
         else
         {
            rgSCHUtlDlHqPTbRmvFrmTx(dlSf,tmpHqProc,tbCb->tbIdx, TRUE);

            if (((tbCb->nackCount + tbCb->dtxCount) >= tbCb->ackCount))
            {
               /*even if one NACK, we consider the feedback
                * on a whole as NACk */
               if ( tbCb->nackCount != 0 )
               {
                  tbCb->isAckNackDtx = TFU_HQFDB_NACK;
               }
               else
               {
                  tbCb->isAckNackDtx = TFU_HQFDB_DTX;
               }
              
            {
               /* Delete the Harq Association. Release the Harq Process */
              rgSCHDhmHqTbTrnsFail(cellCb, tmpHqProc, tbCb->tbIdx, &maxRetx);
            }
           }/*if(((tbCb->nackCount+....*/
         }/*else....*/

         if (tbCb->taSnt == TRUE)
         {
            /* [ccpu00127148] Correcting the check */
            if (TRUE == maxRetx)
            {
               tbCb->taSnt                  =  FALSE;
               ue->dl.taCb.state = RGSCH_TA_IDLE;
               
               rgSCHUtlReTxTa(cellCb, ue); 
               DU_LOG("\nDEBUG  -->  SCH : Nack/DTX Rcvd for TA. Max Tries Attempted");
               
             }
         }
      }
   }
   return ROK;
}/* rgSCHDhmRlsDlsfHqProc */
#else /* ifdef LTE_TDD */
/**
 * @brief Handler for Removing the HARQ process from a dlsf.
 *                    
 * @details
 *
 *     Function : rgSCHDhmRlsDlsfHqProc
 *     
 *     This function shall be invoked for every tti. It goes back to
 *     to the sixth last subframe to check whether it still exists. If
 *     that exists this function traverses through the entire harq 
 *     proc list associated and frees up all of them.  
 *           
 *  @param[in]  RgSchCellCb     *cell 
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHDhmRlsDlsfHqProc(RgSchCellCb *cell,CmLteTimingInfo timingInfo)
{
   RgSchDlSf         *sf;
   CmLteTimingInfo   frm;
   RgSchDlHqProcCb   *tmpHqProc;
   Bool              maxRetx;
   CmLList           *node;
   CmLList           *hqPNode;
   uint8_t                idx;
   RgSchDlHqTbCb     *tbCb;
   RgSchUeCb         *ue;

   /* Fetch the current timing info. Modify it to Last sf to be rlsd.*/
   /* ccpu00133109: Removed RGSCHSUBFRMCRNTTIME as it is not giving proper 
    * output if diff is more than 10. Instead using RGSCHDECRFRMCRNTTIME() 
    * as it is serving the purpose */
   RGSCHDECRFRMCRNTTIME(timingInfo, frm, RG_SCH_CMN_HARQ_INTERVAL); 
   

   /* Get the required Last subframe */
   sf = rgSCHUtlSubFrmGet(cell, frm);

   /*CA Dev Start*/
   /*Handling for Msg4*/
   node = sf->msg4HqPLst.first;
   while (node)
   {
      tmpHqProc = (RgSchDlHqProcCb *)(node->node);
      if (HQ_TB_WAITING == tmpHqProc->tbInfo[0].state)
      {
         tbCb = &tmpHqProc->tbInfo[0];
         /* Fix : syed MultiUe per TTI crash in TA List. */
         maxRetx = FALSE;

         RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),"\n rgSCHDhmRlsDlsfHqProc():\
                  txCntr=%d tmpHqProc=%d",tbCb->txCntr,tmpHqProc->procId));

         tbCb->dtxCount++;
         if ((tmpHqProc->hqE->msg4Proc == tmpHqProc) || 
               (tmpHqProc->hqE->ccchSduProc == tmpHqProc))
         {
            tbCb->isAckNackDtx = TFU_HQFDB_NACK;
            rgNumMsg4Dtx++;
         }

         node = node->next;
         if (tbCb->fbkRepCntr != 0)
         {
            /* Update timingInfo for this hqP so that next subframe its picked up */
            RG_SCH_ADD_TO_CRNT_TIME(tbCb->timingInfo, tbCb->timingInfo, 1);
            rgSCHUtlDlHqPTbRmvFrmTx(sf,tmpHqProc,tbCb->tbIdx, TRUE);
            tbCb->fbkRepCntr--;
            continue;
         }
         rgSCHUtlDlHqPTbRmvFrmTx(sf,tmpHqProc,tbCb->tbIdx, FALSE);

         /* Delete the Harq Association. Release the Harq Process */
         rgSCHDhmHqTbTrnsFail(cell, tmpHqProc, tbCb->tbIdx, &maxRetx);

      }
   } 
   /* Subframe is present. Delete all the harq associations from 
    * this subframe.
    */
   node = sf->ueLst.first;
   while (node)
   {
      ue = (RgSchUeCb *)(node->node);
      node = node->next;
      if(ue != NULLP)
      {
         hqPNode = ue->dl.dlSfHqInfo[cell->cellId][sf->dlIdx].hqPLst.first;

         while (hqPNode)
         {
            tmpHqProc = (RgSchDlHqProcCb *)hqPNode->node;
            tmpHqProc->cwSwpEnabled = FALSE;
            hqPNode = hqPNode->next;
            for (idx = 0 ;idx < 2; idx++)
            {
               if (HQ_TB_WAITING == tmpHqProc->tbInfo[idx].state)
               {
                  tbCb = &tmpHqProc->tbInfo[idx];
                  /* Fix : syed MultiUe per TTI crash in TA List. */
                  maxRetx = FALSE;

                  RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),"\n rgSCHDhmRlsDlsfHqProc():\
                           txCntr=%d tmpHqProc=%d",tbCb->txCntr,tmpHqProc->procId));

                  tbCb->dtxCount++;
                  if ((tmpHqProc->hqE->msg4Proc == tmpHqProc) || 
                        (tmpHqProc->hqE->ccchSduProc == tmpHqProc))
                  {
                     tbCb->isAckNackDtx = TFU_HQFDB_NACK;
                     rgNumMsg4Dtx++;
                  }
                  else
                  {
                     tbCb->isAckNackDtx = TFU_HQFDB_DTX;
                  }

                  rgSCHUtlDlHqPTbRmvFrmTx(sf,tmpHqProc,idx, FALSE);

                  {
                     /* Delete the Harq Association. Release the Harq Process */
                     rgSCHDhmHqTbTrnsFail(cell, tmpHqProc, tbCb->tbIdx, &maxRetx);
                  }
                  if (tbCb->taSnt == TRUE)
                  {
                     /* [ccpu00127148] Correcting the check */
                     if (TRUE == maxRetx)
                     {
                        tbCb->taSnt                  =  FALSE;
                        ue->dl.taCb.state = RGSCH_TA_IDLE;

                        rgSCHUtlReTxTa(cell, ue); 
                        RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx), 
                                 "Nack/DTX Rcvd for TA. Max Tries Attempted\n"));
                     }
                  }
                  RgSchCmnDlUe *ueDl = RG_SCH_CMN_GET_DL_UE(ue,cell);
                  ueDl->mimoInfo.cwInfo[tbCb->tbIdx].dtxCnt++;
               }
            }
         }
      }
   }
   /*CA Dev End*/

   return ROK;
}  /* rgSCHDhmRlsDlsfHqProc */
#endif
#ifdef LTEMAC_SPS
#ifdef RG_UNUSED
/**
 * @brief This function marks the HARQ process with a given ID as SPS HARQ
 * proc
 *
 * @details
 *
 *     Function: rgSCHDhmMarkSpsHqProc
 *     Purpose:  This function returns the HARQ process with the given ID.
 *     Invoked by: SPS Module
 *     Processing steps: 
 *     - Get the HARQ process by index from the UE
 *     - Set isSpsHqProc = TRUE
 *     
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  uint8_t            idx
 *  @return  S16       
 *         -#   ROK     if successful
 *         -#   RFAILED otherwise
 *
 **/
S16 rgSCHDhmMarkSpsHqProc(RgSchUeCb *ue,uint8_t idx)
{
   RgSchDlHqProcCb      *hqP;

   /* Pick the proc based on the index provided */
   rgSCHDhmGetHqProcFrmId(ue->cell, ue, idx, &hqP);

   return ROK;
} /* rgSCHDhmMarkSpsHqProc */
#endif /* RG_UNUSED */
#endif /* LTEMAC_SPS */

#ifndef LTE_TDD
/** * @brief Handler for HARQ feedback received for DL AckNack rep enabled UE
 *
 * @details
 *
 *     Function : rgSCHDhmProcHqFdbkAckNackRep
 *     
 *     This function shall act on the feedback received from TOM for DL
 *     transmission. 
 *     
 *           
 *  @param[in]  RgSchDlHqProcCb     *hqP
 *  @param[in]  RgSchDlSf           *sf                  
 *  @param[in]  uint8_t                  tbCnt
 *  @param[in]  uint8_t                  *isAck
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgSCHDhmProcHqFdbkAckNackRep(RgSchDlHqProcCb *hqP,RgSchDlSf *sf,uint8_t tbCnt,uint8_t *isAck)
{
      /* Check if this is repeating UE */
    rgSCHUtlDlHqPTbRmvFrmTx(sf, hqP, tbCnt, TRUE);
    /* Check if last repetition */
    if (--hqP->tbInfo[tbCnt].fbkRepCntr)
    {
        /* Update timingInfo for this hqP so that next subframe its picked up */
        RG_SCH_ADD_TO_CRNT_TIME(hqP->tbInfo[tbCnt].timingInfo, \
                               hqP->tbInfo[tbCnt].timingInfo, 1);
        return RFAILED;
    }
    
    /* Take decision here based on the number
     * of DTX's,NACK's and ACK's received
     */
    if (((hqP->tbInfo[tbCnt].ackCount) > (hqP->tbInfo[tbCnt].nackCount) + 
                (hqP->tbInfo[tbCnt].dtxCount)))
    {
        *isAck = TFU_HQFDB_ACK; 
    }
    /*even a single NACK indicates that UE received
     * the transmission. 
     */
    else if ( hqP->tbInfo[tbCnt].nackCount != 0 )
    {
        *isAck = TFU_HQFDB_NACK;
    }
    else
    {
        *isAck = TFU_HQFDB_DTX;
    }
      
       
    hqP->tbInfo[tbCnt].isAckNackDtx = *isAck; 
    return ROK;
}
#endif /* ifndef LTE_TDD */


/* Freeing up the HARQ proc blocked for
 * indefinite time in case of Retx */
/**
 * @brief This function handles the scenario in case Retx allocation is failed.
 *
 * @details
 *
 *     Function: rgSCHDhmDlRetxAllocFail
 *     Purpose:  
 *                
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  RgSchDlHqProcCb       *hqP
 *  @return  Void 
 *
 **/
S16 rgSCHDhmDlRetxAllocFail(RgSchUeCb *ue,RgSchDlHqProcCb *hqP)
{
   RgSchCellCb      *cell;
   RgInfRlsHqInfo   *rlsHqInfo;
   Pst              pst;   
   Bool             maxRetx = FALSE;
   RgSchCmnCell           *cellSch;

   cell = hqP->hqE->cell;
   cellSch = RG_SCH_CMN_GET_CELL(cell);
   rlsHqInfo = &(cell->rlsHqArr[cell->crntHqIdx]);
 
   /* If retx was attempted for 1st TB, increment its retx alloc fail counter */
   if (hqP->tbInfo[0].state == HQ_TB_NACKED)
   {
      hqP->tbInfo[0].cntrRetxAllocFail++;
   }

   /* If retx was attempted for 2nd TB, increment its retx alloc fail counter */
   if (hqP->tbInfo[1].state == HQ_TB_NACKED)
   {
      hqP->tbInfo[1].cntrRetxAllocFail++;
   }

   /* initialize MAC-SCH interface HARQ release info */
   rlsHqInfo->numUes = 0;
   rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].numOfTBs = 0;

   /* Release HARQ proc for TB1 if Retx alloc failure count has reached max */
   if (hqP->tbInfo[0].cntrRetxAllocFail == RG_SCH_MAX_RETX_ALLOC_FAIL)
   {
      if (hqP->hqE->msg4Proc == hqP)
      {
         hqP->tbInfo[0].txCntr = cell->dlHqCfg.maxMsg4HqTx;
      }
      else
      {
         hqP->tbInfo[0].txCntr = hqP->hqE->maxHqTx;
      }

      rgSCHDhmHqTbTrnsFail(cell, hqP, hqP->tbInfo[0].tbIdx, &maxRetx);

#ifdef LTE_L2_MEAS
      if (maxRetx)
      {
      rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].status[\
               rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].numOfTBs] = 0xFF; /* RGU_NACK_LOSS */;
      }
      else
      {
      rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].status[\
               rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].numOfTBs] = FALSE;
      }
#else
      rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].status[\
               rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].numOfTBs] = FALSE;
#endif

      rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].tbId[\
               rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].numOfTBs] = 1;
      rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].numOfTBs++;
   }

   /* Release HARQ proc for TB2 if Retx alloc failure count has reached max */
   if (hqP->tbInfo[1].cntrRetxAllocFail == RG_SCH_MAX_RETX_ALLOC_FAIL)
   {
      if (hqP->hqE->msg4Proc == hqP)
      {
         hqP->tbInfo[1].txCntr = cell->dlHqCfg.maxMsg4HqTx;
      }
      else
      {
         hqP->tbInfo[1].txCntr = hqP->hqE->maxHqTx;
      }

      rgSCHDhmHqTbTrnsFail(cell, hqP, hqP->tbInfo[1].tbIdx, &maxRetx);

      rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].status[\
               rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].numOfTBs] = FALSE;
      rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].tbId[\
               rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].numOfTBs] = 2;
      rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].numOfTBs++;
   }

   /* MS_WORKAROUND for ccpu00122892 Temp fix for erroeneous RETX Harq release by rgSCHCmnDlAllocRetxRb */
   
   if ((hqP->tbInfo[0].state != HQ_TB_NACKED) &&
       (hqP->tbInfo[1].state != HQ_TB_NACKED))    
   {
      cellSch->apisDl->rgSCHDlProcRmvFrmRetx(cell, ue, hqP);
   }

   /* send HARQ release to MAC */
   if (rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].numOfTBs > 0)
   {
      /* Fix : syed HO UE does not have a valid ue->rntiLnk */	   
      rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].rnti = ue->ueId;
      rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].hqProcId = hqP->procId;
      rlsHqInfo->numUes = 1;

      rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], cell->macInst);
      RgSchMacRlsHq(&pst, rlsHqInfo);
   }

   return ROK;
}

#ifdef DL_LA
static S16 rgSCHDhmUpdateAckNackHistory(RgSchCellCb *cell,RgSchUeCb *ueCb,uint8_t hqfdbk,uint8_t tbCnt)
{
   RgSchCmnDlUe  *ueDl;
   
   ueDl = RG_SCH_CMN_GET_DL_UE(ueCb,cell);
   
   /*
    * If fdbk is ack update totalNoOfAck and ackNackHistory for 
    * current idx
    */ 
   if (hqfdbk == TFU_HQFDB_ACK)
   {
      ueDl->laCb[tbCnt].deltaiTbs += DL_LA_STEPUP; 
   }
   else   
   {         
      ueDl->laCb[tbCnt].deltaiTbs = ueDl->laCb[tbCnt].deltaiTbs - DL_LA_STEPDOWN; 
   }
   /*
   printf("\nDEBUG  -->  SCH: deltaiTbs[%d] cqibasediTbs[%d] iTbs[%d] tbCnt[%d]\n", 
           ueDl->laCb[tbCnt].deltaiTbs, ueDl->laCb[tbCnt].cqiBasediTbs, 
           (ueDl->laCb[tbCnt].deltaiTbs + ueDl->laCb[tbCnt].cqiBasediTbs)/100,
           tbCnt);
   */
   rgSCHDhmUpdBlerBasediTbsEff(cell, ueCb, tbCnt); 

   return ROK;
}

S16 rgSCHDhmUpdBlerBasediTbsEff(RgSchCellCb *cell,RgSchUeCb *ueCb,uint8_t tbCnt)
{
   S32            iTbs;
   RgSchCmnDlUe  *ueDl;
   RgSchCmnCell  *cellSch = RG_SCH_CMN_GET_CELL(cell);
   uint8_t       cfi = cellSch->dl.currCfi;
   uint8_t       maxiTbs = (*(RgSchCmnCqiToTbs *)(cellSch->dl.cqiToTbsTbl[0][cfi]))[RG_SCH_CMN_MAX_CQI - 1];
   maxiTbs       = RG_SCH_DL_MAX_ITBS;

   ueDl = RG_SCH_CMN_GET_DL_UE(ueCb,cell);
   iTbs =  (ueDl->laCb[tbCnt].deltaiTbs + ueDl->laCb[tbCnt].cqiBasediTbs)/100; 
 
   if (iTbs > maxiTbs)
   {
      ueDl->laCb[tbCnt].deltaiTbs = (maxiTbs * 100) - ueDl->laCb[tbCnt].cqiBasediTbs;
      ueDl->mimoInfo.cwInfo[tbCnt].iTbs[0] = RGSCH_MIN(maxiTbs, ueCb->cell->thresholds.maxDlItbs); 
   }
   else if (iTbs < 0)
   {
      ueDl->laCb[tbCnt].deltaiTbs = -(ueDl->laCb[tbCnt].cqiBasediTbs);
      ueDl->mimoInfo.cwInfo[tbCnt].iTbs[0] = 0; 
   }
   else
   {
      ueDl->mimoInfo.cwInfo[tbCnt].iTbs[0] = RGSCH_MIN(((ueDl->laCb[tbCnt].cqiBasediTbs +\
                                                         ueDl->laCb[tbCnt].deltaiTbs)/100),  
		                                       ueCb->cell->thresholds.maxDlItbs);
   }
#ifdef RG_5GTF
   ueCb->ue5gtfCb.mcs = ueDl->mimoInfo.cwInfo[tbCnt].iTbs[0];
#endif
   ueDl->mimoInfo.cwInfo[tbCnt].iTbs[1] = ueDl->mimoInfo.cwInfo[tbCnt].iTbs[0];

   /* Eff for CW for 1 Layer Tx */
   ueDl->mimoInfo.cwInfo[tbCnt].eff[0] = 
      (*(RgSchCmnTbSzEff *)(cellSch->dl.cqiToEffTbl[0][cfi]))\
      [ueDl->mimoInfo.cwInfo[tbCnt].iTbs[0]];

   /* Eff for CW for 2 Layer Tx */
   ueDl->mimoInfo.cwInfo[tbCnt].eff[1] = 
      (*(RgSchCmnTbSzEff *)(cellSch->dl.cqiToEffTbl[1][cfi]))\
      [ueDl->mimoInfo.cwInfo[tbCnt].iTbs[1]];

   return ROK;
}
#endif

#ifdef LTE_TDD
/**
 * @brief This function Processes the Hq Fdbk in case of 
 *        special Bundling in TDD (FAPIv1.1: Table 79)   
 *
 * @details
 *
 *     Function: rgSCHDhmPrcSplBundlFdbk
 *     Purpose: To Interpret the Harq Feedback according to
 *              table 7.3-1: 36.213
 *    
 *          0 = 0 or None (UE detect at least one DL is missed)
 *          1 = 1 or 4 or 7 ACKs reported
 *          2 = 2 or 5 or 8 ACKs reported
 *          3 = 3 or 6 or 9 ACKs reported
 *          4 = DTX (UE did not transmit anything)
 *                
 *  @param[in]  TfuHqInfo    *fdbk
 *  @param[in]  uint8_t            hqCnt 
 *  @return  Void 
 *
 **/
static Void rgSCHDhmPrcSplBundlFdbk(RgSchCellCb *cell,TfuHqInfo *fdbk,uint8_t hqCnt)
{
   uint8_t       numOfAcks;
   
   /* Num of ACKs reported by UE */
   numOfAcks = fdbk->isAck[0];

   if(fdbk->isAck[0] == TFU_HQFDB_NACK || 
      fdbk->isAck[0] == TFU_HQFDB_DTX)
   {
      /* NACK/DTX CASE */
   } 
   else 
   {
      RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, 
            rgSchNumOfAcksToAckNack[(hqCnt-1)], (numOfAcks - 1));

      fdbk->isAck[0] = rgSchNumOfAcksToAckNack[(hqCnt-1)]
                                                 [(numOfAcks-1)];  
   }
   /* The Hq Fdbk is a combined Ack/Nack for multiple Codewords within
      the PDSCH trasnmission (spatial bundling). So we have 
      to assume same feedback for both codewords */ 
#ifdef LTE_ADV
   for(uint8_t idx = 1 ; idx < TFU_MAX_HARQ_FDBKS; idx++)
   {
      fdbk->isAck[idx] = fdbk->isAck[0]; 
   }
#else
   fdbk->isAck[1] = fdbk->isAck[0]; 
#endif
   
   return;
}
#endif

/**
 * @brief This function adds HARQ process to FREE list
 *
 * @details
 *
 *     Function: rgSCHDhmHqPAdd2FreeLst
 *     Purpose:  
 *     
 *     Invoked by: scheduler
 *     
 *  @param[out] RgDlHqProc       *hqP
 *  @return  Void      
 *
 **/
Void rgSCHDhmHqPAdd2FreeLst(RgSchDlHqProcCb  *hqP)
{

#ifdef LAA_DBG
   if (hqP->hqPLst)
   {
      int *p = NULL;
      printf("\nDEBUG  -->  SCH: Crashing already part of free lst\n");
      printf("\nDEBUG  -->  SCH: Crashing %d \n", *p);
      *p = 10;
   }
#endif
   cmLListAdd2Tail(&hqP->hqE->free, &hqP->lnk);
   hqP->hqPLst = &hqP->hqE->free;


#ifdef LAA_DBG
   if (hqP->hqE->free.count > 8)
   {
      int *p = NULL;
      printf("\nDEBUG  -->  SCH: Crashing invalid hq count\n");
      printf("\nDEBUG  -->  SCH: Crashing %d \n", *p);
      *p = 10;
   }
#endif

#ifdef LTE_ADV
   rgSCHLaaHndlHqProcFree(hqP);
#endif

   return;
} /* rgSCHDhmHqPAdd2FreeLst */


/**
 * @brief This function adds HARQ process to inUse list
 *
 * @details
 *
 *     Function: rgSCHDhmHqPAdd2InUseLst
 *     Purpose:  
 *     
 *     Invoked by: scheduler
 *     
 *  @param[out] RgDlHqProc       *hqP
 *  @return  Void      
 *
 **/
Void rgSCHDhmHqPAdd2InUseLst(RgSchDlHqProcCb  *hqP)
{

#ifdef LAA_DBG
   if (hqP->hqPLst)
   {
      int *p = NULL;
      printf("\nDEBUG  -->  SCH: Crashing already part of inuse lst\n");
      printf("\nDEBUG  -->  SCH: Crashing %d \n", *p);
      *p = 10;
   }
#endif
   cmLListAdd2Tail(&hqP->hqE->inUse, &hqP->lnk);
   hqP->hqPLst = &hqP->hqE->inUse;


#ifdef LAA_DBG
   if (hqP->hqE->inUse.count > 8)
   {
      int *p = NULL;
      printf("\nDEBUG  -->  SCH: Crashing invalid hq count \n");
      printf("\nDEBUG  -->  SCH: Crashing %d \n", *p);
      *p = 10;
   }
#endif

   return;
} /* rgSCHDhmHqPAdd2InUseLst */

/**
 * @brief This function adds HARQ process to FREE list
 *
 * @details
 *
 *     Function: rgSCHDhmHqPDelFrmFreeLst
 *     Purpose:  
 *     
 *     Invoked by: scheduler
 *     
 *  @param[out] RgDlHqProc       *hqP
 *  @return  Void      
 *
 **/
Void rgSCHDhmHqPDelFrmFreeLst(RgSchDlHqProcCb *hqP)
{

#ifdef LAA_DBG
   if (!hqP->hqPLst)
   {
      int *p = NULL;
      printf("\nDEBUG  -->  SCH: Crashing not part of any lst\n");
      printf("\nDEBUG  -->  SCH: Crashing %d \n", *p);
      *p = 10;
   }
#endif
#ifdef LAA_DBG
   if (hqP->hqPLst != &hqP->hqE->free)
   {
      int *p = NULL;
      printf("\nDEBUG  -->  SCH: Crashing del from wrong lst\n");
      printf("\nDEBUG  -->  SCH: Crashing %d \n", *p);
      *p = 10;
   }
#endif

   cmLListDelFrm(&hqP->hqE->free, &hqP->lnk);
   hqP->hqPLst = NULLP;

#ifdef LAA_DBG
   if (hqP->hqE->free.count > 8)
   {
      int *p = NULL;
      printf("\nDEBUG  -->  SCH: Crashing invalid hq count\n");
      printf("\nDEBUG  -->  SCH: Crashing %d \n", *p);
      *p = 10;
   }
#endif

   return;
} /* rgSCHDhmHqPDelFrmFreeLst */



/**
 * @brief This function adds HARQ process to FREE list
 *
 * @details
 *
 *     Function: rgSCHDhmHqPDelFrmInUseLst
 *     Purpose:  
 *     
 *     Invoked by: scheduler
 *     
 *  @param[out] RgDlHqProc       *hqP
 *  @return  Void      
 *
 **/
Void rgSCHDhmHqPDelFrmInUseLst(RgSchDlHqProcCb *hqP)
{

#ifdef LAA_DBG
   if (!hqP->hqPLst)
   {
      int *p = NULL;
      printf("\nDEBUG  -->  SCH: Crashing not part of any lst\n");
      printf("\nDEBUG  -->  SCH: Crashing %d \n", *p);
      *p = 10;

   }
#endif
#ifdef LAA_DBG
   if (hqP->hqPLst != &hqP->hqE->inUse)
   {
      int *p = NULL;
      printf("\nDEBUG  -->  SCH: Crashing del from wrong lst\n");
      printf("\nDEBUG  -->  SCH: Crashing %d \n", *p);
      *p = 10;
   }
#endif

   cmLListDelFrm(&hqP->hqE->inUse, &hqP->lnk);
   hqP->hqPLst = NULLP;

#ifdef LAA_DBG
   if (hqP->hqE->inUse.count > 8)
   {
      int *p = NULL;
      printf("\nDEBUG  -->  SCH: Crashing invalid hq count\n");
      printf("\nDEBUG  -->  SCH: Crashing %d \n", *p);
      *p = 10;
   }
#endif

   return;
} /* rgSCHDhmHqPDelFrmInUseLst */


/**********************************************************************
 
         End of file
**********************************************************************/
