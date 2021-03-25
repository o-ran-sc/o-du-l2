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
  
     Desc:     C source code for scheduler 1
  
     File:     rg_sch_sc1.c
  
**********************************************************************/

/** @file rg_sch_sc1.c
@brief The scheduling functionality is implemented in this file.
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
#include "rg_sch_sc1.h"

/* header/extern include files (.x) */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */
#include "rg_sch_cmn.x"
#include "rg_sch_sc1.x"    /* typedefs for SC1 Scheduler */




#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Functions called from outside */
static S16 rgSCHSc1RgrDlCellRecfg ARGS((
RgSchCellCb             *cell,
RgrCellRecfg            *recfg,
RgSchErrInfo            *err
));

/*--------------------------*
 * DL SCHED STATIC declarations START
 *---------------------------*/
static Void rgSCHSc1DlSvcAddToSchd ARGS((
RgSchCellCb                *cell,
RgSchDlLcCb                *svc
));
static Void rgSCHSc1DlAdd2UeSchdSvcs ARGS((
RgSchCellCb                *cell,
RgSchUeCb          *ue,
RgSchDlLcCb        *svc
));
static Void rgSCHSc1DlRmvUeFrmPrioQs ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
static Void rgSCHSc1DlSuspendUe ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
static Void rgSCHSc1DlInactvtUe ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
static Void rgSCHSc1DlProcRmvFrmCellRetx ARGS((
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
));
static Void rgSCHSc1DlProcRmvFrmUeRetx ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlHqProcCb            *hqP
));
static Void rgSCHSc1DlMngPrio0SvcPosn ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));
static Void rgSCHSc1DlMngGbrSvcPosn ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));
static Void rgSCHSc1DlMngAmbrSvcPosn ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));
static Void rgSCHSc1DlMngSvcPosn ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));
static Void rgSCHSc1DlUeAddToSchd ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue
));
static Void rgSCHSc1DlTaCmd ARGS((
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));
static Void rgSCHSc1DlInitQueues ARGS((
RgSchSc1DlCell *cellDl
));
static Void rgSCHSc1DlDeinitQueues ARGS((
RgSchSc1DlCell *cellDl
));
static Void rgSCHSc1DlAdd2UeLcsWithData ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));
static Void rgSCHSc1DlRmFrmUeLcsWithData ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));
/*--------------------------*
 * UL SCHED STATIC declarations START
 *---------------------------*/
static Void rgSCHSc1UlPosnUeInQ ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue
));
static Void rgSCHSc1UlSchdUeTxLst ARGS((
RgSchCellCb         *cell,
CmLListCp             *ueTxLst,
RgSchCmnUlRbAllocInfo *allocInfo,
uint8_t                    *remUe
));
static Void rgSCHSc1DlProcRmvFrmRetx ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlHqProcCb            *hqP
));
Void rgSCHSc1DlScanUpdPdbPrio ARGS((
RgSchCellCb *cell
));
S16 rgSCHSc1DlFillFlowCntrlInfo ARGS(( 
RgSchCellCb    *cell,
RgInfSfAlloc   *sfAlloc
));

static Void rgSCHSc1DlPreSchd ARGS ((
RgSchCellCb   *cell
));
static Void rgSCHSc1DlPstSchd ARGS ((
 Inst       schInst
));
#ifdef __cplusplus
}
#endif /* __cplusplus */




/***************** SC1 DL SCHEDULER FUNCTION DEFNs START HERE ********/

/***********************************************************
 *
 *     Func : rgSCHSc1DlUeReset 
 *        
 *     Desc : Out of Meas Gap. Reposition the UEs Retx Hq Procs,
 *            and Svc in respective Prio Qs.
 *            
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
Void rgSCHSc1DlUeReset(RgSchCellCb *cell,RgSchUeCb *ue)
{

   rgSCHSc1DlSuspendUe(cell, ue);

   return;
}


/***********************************************************
 *
 *     Func : rgSCHSc1DlActvtUe
 *        
 *     Desc : Out of Meas Gap. Reposition the UEs Retx Hq Procs,
 *            and Svc in respective Prio Qs.
 *            
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
Void rgSCHSc1DlActvtUe(RgSchCellCb *cell,RgSchUeCb  *ue)
{
   RgSchSc1DlUe    *ueDl = RG_GET_SC1_UE_DL(ue, cell);
   CmLListCp       *lst;
   CmLList         *node;
   RgSchDlHqProcCb *hqP;
   RgSchDlLcCb     *svc;
   uint8_t         idx;

   /* Add UE's HqProcs From UERetxLst to CellRetxLst */
   lst = &ueDl->retxHqProcs;
   node = lst->first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)node->node;
      node = node->next;
      rgSCHSc1DlProcRmvFrmUeRetx(cell, ue, hqP);
      rgSCHSc1DlProcAddToCellRetx(cell, hqP);
   }

   /* Iterate over all the Services if bo != 0 then add */
   for (idx = 0; idx < RGSCH_MAX_LC_PER_UE; ++idx)
   {
      svc = ue->dl.lcCb[idx];
      if (svc == NULLP)
      {
         continue;
      }
      rgSCHSc1DlMngSvcPosn(cell, ue, svc);
   } 

   /* Add UE to AMBR Prio Q */
   if (ueDl->ambrSvc)
   {
      rgSCHSc1DlUeAddToSchd(cell, ue);
   }

   return;
}


/***********************************************************
 *
 *     Func : rgSCHSc1DlUeRefresh
 *        
 *     Desc : Handle 'refresh' for Downlink
 *            (ie UE's downlink AMBR and downlink GBR LCGs are
 *            refreshed at this point)
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
Void rgSCHSc1DlUeRefresh(RgSchCellCb *cell,RgSchUeCb *ue)
{
   RgSchSc1DlUe   *ueDl = RG_GET_SC1_UE_DL(ue, cell);
                           /*cell added as part of CA dev*/
   RgSchCmnDlSvc *svcCmn;
   RgSchSc1DlSvc  *svcSc1;
   CmLListCp   *lst;
   CmLList     *node;
   RgSchDlLcCb *svc;

   if (ue->dl.ambrCfgd)
   {
      ueDl->ambr = ue->dl.ambrCfgd;
   }
   else
   {
      ueDl->ambr = RG_SC1_MAX_DL_AMBR;
   }

   if (ueDl->ambrSvc != NULLP)
   {
      ueDl->effAmbr = RGSCH_MIN(ueDl->ambr, ueDl->ambrSvc->bo);
      /* Update UEs position in the Queue */
      rgSCHSc1DlUeAddToSchd(cell, ue);
   }

   lst = &ueDl->gbrSvcs;
   node = lst->first;
   while (node != NULLP)
   {
      svc   = (RgSchDlLcCb *)node->node;
      svcSc1 = RG_GET_SC1_SVC_DL(ue,svc,cell);
      svcCmn = RG_SCH_CMN_GET_DL_SVC(svc);
      node = node->next;
      svcSc1->gbr   = svcCmn->gbr;
      svcSc1->mbr   = svcCmn->mbr;
      /* Update the SVC's positioning in the Queue */
      rgSCHSc1DlMngGbrSvcPosn(cell, ue, svc);
   }
   return;
}


/**
 * @brief This function removes a HARQ process from the retx
 *
 * @details
 *
 *     Function: rgSCHSc1DlProcRmvFrmCellRetx
 *     Purpose:  This function removes a HARQ process from retransmission
 *               queue. This may be performed when a HARQ ack is successful
 *               for a retransmission or when the scheduling determines
 *               to throw out the process due to poor conditions
 *     
 *     Invoked by: LIM and Scheduler
 *     
 *  @param[in]  RgSchSc1Cb*      cell
 *  @param[in]  RgDlHqProc*   hqP  
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlProcRmvFrmCellRetx(RgSchCellCb *cell,RgSchDlHqProcCb  *hqP)
{
   RgSchSc1DlCell       *cellDl = RG_GET_SC1_CELL_DL(cell);
   RgSchCmnDlHqProc     *hqProcDl = RG_SCH_CMN_GET_DL_HQP(hqP);

   if (hqProcDl->retxLnk.node != NULLP)
   {
      cmLListDelFrm(&cellDl->retxLst[((RgSchSc1DlHqProc *)\
                    (hqProcDl->schSpfc))->prio], &(hqProcDl->retxLnk));
      hqProcDl->retxLnk.node = NULLP;
   }
   return;
}


/**
 * @brief This function removes a HARQ process from the UE retx
 *
 * @details
 *
 *     Function: rgSCHSc1DlProcRmvFrmUeRetx
 *     Purpose:  This function removes a HARQ process from UE retransmission
 *               queue. 
 *     
 *     Invoked by: LIM and Scheduler
 *     
 *  @param[in]  RgSchUeCb*    ue
 *  @param[in]  RgDlHqProc*   hqP  
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlProcRmvFrmUeRetx(RgSchCellCb *cell,RgSchUeCb  *ue,RgSchDlHqProcCb *hqP)
{
   RgSchSc1DlUe            *sc1Ue = RG_GET_SC1_UE_DL(ue, cell);
   RgSchSc1DlHqProc     *hqProcDl = RG_GET_SC1_HQP_DL(hqP);

   if (hqProcDl->retxLnkUe.node != NULLP)
   {
      cmLListDelFrm(&sc1Ue->retxHqProcs,
                  &(hqProcDl->retxLnkUe));
      hqProcDl->retxLnkUe.node = NULLP;
   }
   return;
}


/**
 * @brief This function adds a HARQ process for UEs retxLst
 *
 * @details
 *
 *     Function: rgSCHSc1DlProcAddToUeRetx
 *     Purpose:  This function adds a HARQ process to UE retransmission
 *               queue. This is performed when UE is suspended due
 *               to measurement gap.
 *     
 *     Invoked by: HARQ feedback processing
 *     
 *  @param[in]  RgSchUeCb*       ue
 *  @param[in]  RgSchDlHqProc*   hqP  
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlProcAddToUeRetx(RgSchCellCb *cell,RgSchUeCb *ue,RgSchDlHqProcCb *hqP)
{
   RgSchSc1DlUe            *sc1Ue = RG_GET_SC1_UE_DL(ue, cell);
   RgSchSc1DlHqProc     *cmnHqDl = RG_GET_SC1_HQP_DL(hqP);

   cmLListAdd2Tail(&sc1Ue->retxHqProcs, 
                    &(cmnHqDl->retxLnkUe));
   cmnHqDl->retxLnkUe.node = (PTR)hqP;
   return;
}


/**
 * @brief This function adds a HARQ process for retx
 *
 * @details
 *
 *     Function: rgSCHSc1DlProcAddToCellRetx
 *     Purpose:  This function adds a HARQ process to retransmission
 *               queue. This may be performed when a HARQ ack is
 *               unsuccessful.
 *     
 *     Invoked by: HARQ feedback processing
 *     
 *  @param[in]  RgSchCellCb*     cell 
 *  @param[in]  RgSchDlHqProc*   hqP  
 *  @return  Void
 *
 **/
Void rgSCHSc1DlProcAddToCellRetx(RgSchCellCb *cell,RgSchDlHqProcCb  *hqP)
{
   RgSchSc1DlCell          *sc1CellDl = RG_GET_SC1_CELL_DL(cell);
   RgSchCmnDlHqProc     *cmnHqDl = RG_SCH_CMN_GET_DL_HQP(hqP);


   if (!RG_SCH_CMN_DL_IS_UE_ACTIVE(hqP->hqE->ue))
   {
      rgSCHSc1DlProcAddToUeRetx(cell, hqP->hqE->ue, hqP);
      return;
   }
   cmLListAdd2Tail(&sc1CellDl->retxLst[((RgSchSc1DlHqProc *)\
                  (cmnHqDl->schSpfc))->prio], &(cmnHqDl->retxLnk));
   cmnHqDl->retxLnk.node = (PTR)hqP;
   return;
}


/**
 * @brief This function implements DL RETRANSMISSION allocation
 *
 * @details
 *
 *     Function: rgSCHSc1DlRetxAlloc
 *     Purpose:  This function implements downlink scheduler's
 *               retransmission allocation.
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo 
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlRetxAlloc(RgSchCellCb *cell,RgSchDlSf *subFrm,RgSchCmnDlRbAllocInfo *allocInfo)
{
   uint8_t              i;
   CmLListCp            *retxLst;
   CmLList              *node;
   RgSchDlHqProcCb      *hqP;
   RgSchSc1DlCell       *sc1CellDl;
   RgSchSc1DlUe         *sc1DlUe;
   RgSchCmnDlUe         *cmnUeDl;
#if (defined(LTEMAC_SPS) || (!defined(LTE_TDD)))
   CmLteTimingInfo      schdTime;
#endif
   uint32_t             effBo;
   RgSchUeCb            *ue = NULLP;
#ifdef LTEMAC_HDFDD
   Bool                 dlAllowed = FALSE;
#endif
   RgSchDlRbAlloc *dlAllocCb;

   sc1CellDl = RG_GET_SC1_CELL_DL(cell);
#if (defined(LTEMAC_SPS) || (!defined(LTE_TDD)))
   schdTime = cell->crntTime;

   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);
#endif
   for (i = 0; i < RG_SCH_SC1_DL_PRIOS; i++)
   {
      retxLst = &sc1CellDl->retxLst[i];
      /* allocate bw for the retransmission..should be same are previous */
      /* If CQI gets worse, as we cannot find same TB size for another   */
      /* MCS, we just remove this from the retransmission queue          */
      node = retxLst->first;
      while (node != NULLP)
      {
         hqP = (RgSchDlHqProcCb *)node->node;
         node = node->next;
         ue = hqP->hqE->ue;

#ifndef LTE_TDD
         if((0 == schdTime.slot) || (5 == schdTime.slot))
         {
            Bool reTxAllw;
            rgSCHCmnChkRetxAllowDtx(cell, ue, hqP, &reTxAllw);
            if(FALSE == reTxAllw)
            {
               continue;
            }
         }
#endif
#ifdef LTEMAC_HDFDD
         if (ue->hdFddEnbld)
         {
            rgSCHCmnHdFddChkDlAllow ( cell, ue, &dlAllowed);
            if (dlAllowed == FALSE)
            {
               continue;
            }
         }
#endif
         /* This UE is already scheduled for transmission */
         cmnUeDl = RG_SCH_CMN_GET_DL_UE(ue,cell);
                   /*cell added as part of CA dev*/
#ifdef LTEMAC_SPS
         if (RG_SCH_CMN_IS_UE_SPS_SCHDLD(ue, cell, schdTime))
         {
            continue;
         }
#endif
         if (RG_SCH_CMN_IS_UE_SCHDLD(ue, cell))
         {
            continue;
         }
         effBo = 0;
         /* Extra check: indicate if there is furtherScope for NewTx
          * addition for a HqProc. This information will
          * be utilized by common scheduler, in case of SM
          * UEs with only one of the TBs retransmitting and the 
          * other TB can be used for clubbing new TX. */
         sc1DlUe = RG_GET_SC1_UE_DL(ue, cell);
         dlAllocCb = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue, cell);
         if (sc1DlUe->lcsWithData.first != NULLP)
         {
            dlAllocCb->mimoAllocInfo.hasNewTxData = TRUE; 
         }
         /* 3.1 MIMO : last parameter changed */
         if (rgSCHCmnDlAllocRetxRb(cell, subFrm, ue, 0, &effBo, hqP, allocInfo) !=\
               ROK)
         {
            /* SF/RETX Bandwidth expired */
            return;
         }
         if (effBo == 0)
         {
            continue;
         }

         if ((hqP->tbInfo[0].state == HQ_TB_ACKED)
            && (hqP->tbInfo[1].state == HQ_TB_ACKED))
         {
            rgSCHSc1DlProcRmvFrmCellRetx(cell, hqP);
            return;
         }

         cmnUeDl->proc = hqP;
         /* 3.1 MIMO moving this call in cmn scheduler */
         /*rgSCHCmnDlRbInfoAddUeRetx(allocInfo, ue);*/
      }
   }
   return;
}

/***********************************************************
 *
 *     Func : rgSCHSc1RlsHqProc
 *        
 *     Desc : Toggles the NDI and releases the harq proc.
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : 
 *
 **********************************************************/
static Void rgSCHSc1RlsHqProc(RgSchDlHqProcCb *hqProc)
{
   rgSCHDhmRlsHqProc(hqProc);
   return;
}

/**
 * @brief This function implements dedicated logical channel data scheduling
 *
 * @details
 *
 *     Function: rgSCHSc1DlDedSvcAlloc 
 *     Purpose:  This function implements dedicated logical 
 *               channel data scheduling 
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchDlSf              *subFrm
 *  @param[in]  RgSchDlLcCb            *svc
 *  @param[in]  uint32_t                    bo
 *  @param[in]  RgSchCmnDlRbAllocInfo  *allocInfo
 *  @return  S16 
 *
 **/
static S16 rgSCHSc1DlDedSvcAlloc(RgSchCellCb *cell,RgSchDlSf *subFrm,RgSchDlLcCb *svc,uint32_t bo,RgSchCmnDlRbAllocInfo *allocInfo)
{
   RgSchUeCb               *ue;
   RgSchDlHqProcCb         *proc;
   uint16_t                rlcHdrEstmt;
   uint32_t                effBo;
   RgSchCmnDlCell          *cmnCellDl = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchCmnDlSvc           *svcCmn = RG_SCH_CMN_GET_DL_SVC(svc);
   RgSchSc1DlSvc           *svcSc1;
   RgSchCmnDlUe            *ueDl;
   RgSchSc1DlHqProc        *sc1HqDl;
   RgSchCmnDlHqProc        *cmnHqDl;
#ifdef LTEMAC_SPS
   CmLteTimingInfo      schdTime;
#endif
#ifdef LTEMAC_HDFDD
   Bool                 dlAllowed = FALSE;
#endif
   S16                  ret;


   /* Get the UE to which this service belongs to */
   ue = svc->ue;
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddChkDlAllow ( cell, ue, &dlAllowed);
         if (dlAllowed == FALSE)
         {
            return ROK;
         }
      }
#endif
   ueDl = RG_SCH_CMN_GET_DL_UE(ue,cell);
          /*cell added as part of CA dev*/ 
#ifdef LTEMAC_SPS
   schdTime = cell->crntTime;

   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);
   if (RG_SCH_CMN_IS_UE_SPS_SCHDLD(ue, cell, schdTime))
   {
      return ROK;
   }
#endif
   if (RG_SCH_CMN_IS_UE_SCHDLD(ue, cell))
   {
      proc = (RgSchDlHqProcCb *)(ueDl->proc);
      /* This UE is selected for retransmission. Hence no further */
      /* scheduling may be done for this UE                       */
      if (RG_SCH_CMN_PROC_SLCTD_FOR_RETX(proc))
      {
         DU_LOG("\nERROR  -->  SCH : CRNTI:%d rgSCHSc1DlDedSvcAlloc():"
            "Ue retransmitting",ue->ueId);
         return ROK;
      }
      /* UE is scheduled for either other services or TA */
      sc1HqDl = RG_GET_SC1_HQP_DL(proc);
      cmnHqDl = RG_SCH_CMN_GET_DL_HQP(proc);
      if (sc1HqDl->prio > svcCmn->prio)
      {
         sc1HqDl->prio = svcCmn->prio;
      }
   }
   else /* First consideration of this UE for scheduling */
   {
      if (rgSCHDhmGetAvlHqProc(cell, ue, cmnCellDl->time, &proc) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : CRNTI:%d rgSCHSc1DlDedSvcAlloc():"
            " No HARQ Proc available", ue->ueId);
         return ROK;
      }
      sc1HqDl = RG_GET_SC1_HQP_DL(proc);
      cmnHqDl = RG_SCH_CMN_GET_DL_HQP(proc);
      cmnHqDl->totBytes = 0;
      /* Initialize some of the parameters of the HQ proc */
      sc1HqDl->prio     = svcCmn->prio;
   }

   /* Including each SDU's header size */
   RG_SCH_CMN_DL_GET_HDR_EST(svc, rlcHdrEstmt);
   bo += rlcHdrEstmt;
   effBo = 0;
   ret = rgSCHCmnDlAllocTxRb(cell, subFrm, ue, bo, &effBo, proc, allocInfo);
   if ((ret != ROK) || (effBo == 0))
   {
      /* If no allocations so far, meaning proc obtained now */
      if (cmnHqDl->totBytes == 0)
      {
         rgSCHSc1RlsHqProc(proc);
         /* Added the handling for removing
          * UE from txHqPLst and resetting outStndAlloc.*/
         if(proc->reqLnk.node != (PTR)NULLP)
         {
            cmLListDelFrm(&allocInfo->dedAlloc.txHqPLst, &proc->reqLnk);
            proc->reqLnk.node = (PTR)NULLP;
         }
         /*Re-set the outstanding alloc information.*/
         ueDl->outStndAlloc = 0;

         /* ccpu00126519: proc should be set to NULLP in UE's DL scratch pad info as well. */
         ueDl->proc = NULLP;
      }
      return (ret);
   }
   svcSc1 = RG_GET_SC1_SVC_DL(ue,svc,cell);
   svcSc1->hdrEstimate = rlcHdrEstmt;
   svcSc1->reqBytes = bo;
   ueDl->proc = proc;
   cmnHqDl->totBytes += effBo;

   rgSCHSc1DlAdd2UeSchdSvcs(cell, ue, svc);
   /* 3.1 MIMO moving this call to cmn scheduler */
   /*rgSCHCmnDlRbInfoAddUeTx(allocInfo, ue); */
   return ROK;
}

/**
 * @brief This function adds a SVC to UE's schdSvcsLst. 
 *
 * @details
 *
 *     Function: rgSCHSc1DlAdd2UeSchdSvcs
 *     Purpose:  This function adds a SVC to UE's schdSvcsLst. 
 *     
 *     Invoked by: Specific Scheduler 
 *     
 *  @param[out] RgSchUeCb          *ue
 *  @param[in]  RgSchDlLcCb        *svc
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlAdd2UeSchdSvcs(RgSchCellCb *cell,RgSchUeCb *ue,RgSchDlLcCb *svc)
{
   RgSchSc1DlSvc  *svcSc1 = RG_GET_SC1_SVC_DL(ue,svc,cell);
   RgSchSc1DlUe   *ueSc1 = RG_GET_SC1_UE_DL(ue, cell);

   /* checking SVC's presence in this lst is unnecessary */
   cmLListAdd2Tail(&ueSc1->schdSvcs, &svcSc1->schdSvcLnk);
   svcSc1->schdSvcLnk.node = (PTR)svc;
   return;
}


/**
 * @brief This function performs new allocations for UEs
 *
 * @details
 *
 *     Function: rgSCHSc1DlDedTx
 *     Purpose:  This function implements scheduler for DL allocation for
 *               new transmissions of UEs.
 *               1. It performs across 9 priorities that it supports - 
 *                   This is from 3GPP specifications
 *               2. There are known number of GBR/MBR queues
 *               3. The first queue is highest priority queue and is 
 *                  satisfied completely prior to any other queues. This
 *                  queue is for RRC signalling.
 *               4. Futher GBR/MBR queues are satisfied for GBR and then MBR
 *               5. Subsequently all other queues are looked at for AMBR
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo 
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlDedTx(RgSchCellCb *cell,RgSchDlSf *subFrm,RgSchCmnDlRbAllocInfo *allocInfo)
{
   CmLListCp            *lst;
   CmLList              *node;
   RgSchUeCb            *ue = NULLP;
   RgSchDlLcCb          *svc;
   uint8_t              i;
   RgSchSc1DlSvc        *svcSc1;
   RgSchSc1DlUe         *ueDl;
   RgSchSc1DlCell       *sc1CellDl = RG_GET_SC1_CELL_DL(cell);
   

   /* Process the first queue that is for RRC signalling and is of */
   /* highest priority.                                            */
   lst  = &sc1CellDl->prioLst[0];
   node = lst->first;
   while(node != NULLP)
   {
      /* Getting service instead of UE */
      svc   = (RgSchDlLcCb *)node->node;
      ue = svc->ue;
      svcSc1 = RG_GET_SC1_SVC_DL(ue,svc,cell);
      node = node->next;
      if (rgSCHSc1DlDedSvcAlloc(cell, subFrm, svc, svcSc1->bo, allocInfo) != ROK) 
      {
         /* sf bw expired */
         return;
      }
   }

   /* Perform allocation for the GBR transmissions */
   for(i = RG_SCH_SC1_DL_GBR_PRIO_START; i <= RG_SCH_SC1_DL_GBR_PRIO_END; i++)
   {
      lst  = &sc1CellDl->prioLst[i];
      node = lst->first;
      while(node != NULLP)
      {
         /* Getting service instead of UE */
         svc   = (RgSchDlLcCb *)node->node;
         ue = svc->ue;
         svcSc1 = RG_GET_SC1_SVC_DL(ue,svc,cell);
         node = node->next;
         if (rgSCHSc1DlDedSvcAlloc(cell, subFrm, svc, svcSc1->effMbr, allocInfo) != ROK) 
         {
            /* sf bw expired */
            return;
         }
      }
   }

   /* To implement AMBR svc scheduling */
   for(i = RG_SCH_SC1_DL_GBR_PRIO_END + 1; i < RG_SCH_SC1_DL_PRIOS; i++)
   {
      lst  = &sc1CellDl->prioLst[i];
      node = lst->first;
      while(node != NULLP)
      {
         ue = (RgSchUeCb *)node->node;
         ueDl = RG_GET_SC1_UE_DL(ue, cell);
         node = node->next;
         /* Get the Curr ambr svc for which allocation is to be made */
         svc = ueDl->ambrSvc;
         if (rgSCHSc1DlDedSvcAlloc(cell, subFrm, svc, ueDl->effAmbr, allocInfo) != ROK) 
         {
            /* sf bw expired */
            return;
         }
      }
   }
   return;
}

/**
 * @brief scheduling for a cell
 *
 * @details
 *
 *     Function : rgSCHSc1DlPreSchd
 *
 *     Processing Steps:
 *     - Nothing to be done in case of RR
 *
 *  @param[in]  Inst      schInst
 *  @return  Void
 **/
static Void rgSCHSc1DlPreSchd( RgSchCellCb   *cell)
{
   
   return;
}
/**
 * @brief scheduling for a cell
 *
 * @details
 *
 *     Function : rgSCHSc1DlPstSchd
 *
 *     Processing Steps:
 *     - Nothing to be done in case of RR
 *
 *  @param[in]  Inst      schInst
 *  @return  Void
 **/
static Void rgSCHSc1DlPstSchd(Inst  schInst)
{
   
   return;
}


/**
 * @brief This function implements scheduler DL allocation
 *
 * @details
 *
 *     Function: rgSCHSc1DlDedNewTx
 *     Purpose:  This function implements scheduler for DL allocation for
 *               UEs.
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlDedNewTx(RgSchCellCb *cell,RgSchCmnDlRbAllocInfo *allocInfo)
{
   RgSchDlSf            *subFrm = allocInfo->dedAlloc.dedDlSf;
   DU_LOG("\nDEBUG  -->  SCH : rgSCHSc1DlDedNewTx\n");

   /* Now perform the new UE selections */
   rgSCHSc1DlDedTx(cell, subFrm, allocInfo);

  /* Stack Crash problem for TRACE5 changes. Added the return below */
  return;

}
/**
 * @brief This function implements scheduler DL allocation
 *
 * @details
 *
 *     Function: rgSCHSc1DlDedRetx
 *     Purpose:  This function implements scheduler for DL allocation for
 *               UEs.
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlDedRetx(RgSchCellCb *cell,RgSchCmnDlRbAllocInfo *allocInfo)
{
   RgSchDlSf  *subFrm = allocInfo->dedAlloc.dedDlSf;
   DU_LOG("\nDEBUG  -->  SCH : rgSCHSc1DlDedRetx\n");

   rgSCHSc1DlRetxAlloc(cell, subFrm, allocInfo);

   return;

}



/**
 * @brief This function adds a service to scheduler
 *
 * @details
 *
 *     Function: rgSCHSc1DlSvcAddToSchd
 *     Purpose:  This function adds a service to the list of services
 *               based on the priority of the services.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue   
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlSvcAddToSchd(RgSchCellCb *cell,RgSchDlLcCb  *svc)
{
   CmLListCp            *lst;
   CmLList              *node;
   RgSchDlLcCb          *lSvc;
   RgSchSc1DlSvc        *svcSc1;
   RgSchSc1DlSvc        *lSvcSc1;
   RgSchSc1DlCell       *sc1CellDl = RG_GET_SC1_CELL_DL(cell);
   RgSchCmnDlSvc        *svcCmn = RG_SCH_CMN_GET_DL_SVC(svc);


   svcSc1 = RG_GET_SC1_SVC_DL(svc->ue,svc,cell);
   /* The service is already in the scheduler */
   if (svcSc1->prioLnk.node != NULLP)
   {
      return;
   }

   /* If the priority = 0, it is the highest priority with infinite */
   /* allowance and the priority is time bound and hence just place */
   /* it at the end of the queue                                    */
   if (svcCmn->prio == 0)
   {
      lst  = &(sc1CellDl->prioLst[0]);
      cmLListAdd2Tail(lst, &svcSc1->prioLnk);
      svcSc1->prioLnk.node = (PTR)svc;
      /* If a svc is put in to cell wide priority Qs
       * then add the same to UE's lcsWithData List */
      rgSCHSc1DlAdd2UeLcsWithData(cell, svc->ue, svc);
      return;
   }

   /* Handle GBR services. We have them of next importance */
   /* check changed from .._START to  .._END */
   if (svcCmn->prio <= RG_SCH_SC1_DL_GBR_PRIO_END)
   {
      if (!RG_SC1_SVC_HAS_DATA(svc,cell))
         return;
      lst  = &(sc1CellDl->prioLst[svcCmn->prio]);
      node = lst->first;
      while(node)
      {
         lSvc = (RgSchDlLcCb *)(node->node);
         lSvcSc1 = RG_GET_SC1_SVC_DL(lSvc->ue,lSvc,cell);
         if (((svcSc1->effGbr > 0) && 
                  (lSvcSc1->effGbr <= svcSc1->effGbr)) ||
               ((lSvcSc1->effGbr == 0) && (svcSc1->effMbr > 0) && 
                (lSvcSc1->effMbr <= svcSc1->effMbr)))
         {
            break;
         }
         node = node->next;
      }
      if (node == NULLP)
      {
         /* We have come to the end of the queue. Let's place it */
         /* here irresepctive of effGbr or effMBr                */
         cmLListAdd2Tail(lst, &svcSc1->prioLnk);
         svcSc1->prioLnk.node = (PTR)svc;
      }
      else
      {
         lst->crnt = node;
         cmLListInsCrnt(lst, &svcSc1->prioLnk);
         svcSc1->prioLnk.node = (PTR)svc;
      }
      /* If a svc is put in to cell wide priority Qs
       * then add the same to UE's lcsWithData List */
      rgSCHSc1DlAdd2UeLcsWithData(cell, svc->ue, svc);
   }
   return;
}



/**
 * @brief This function removes a UE from scheduler Queue
 *
 * @details
 *
 *     Function: rgSCHSc1DlUeRmvFrmSchd
 *     Purpose:  This function removes a UE from the list of UEs
 *               based on the priority of the UEs Current AMBR SVC.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue   
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlUeRmvFrmSchd(RgSchCellCb *cell,RgSchUeCb *ue)
{
   RgSchSc1DlCell *cellDl = RG_GET_SC1_CELL_DL(cell);
   RgSchSc1DlUe   *ueDl = RG_GET_SC1_UE_DL(ue, cell);
   CmLListCp   *lst;


   lst  = &cellDl->prioLst[ueDl->prio];
   if (ueDl->prioLnk.node != NULLP)
   {
      cmLListDelFrm(lst, &ueDl->prioLnk);
      ueDl->prioLnk.node = (PTR)NULLP;
      /* If a svc is removed from cell wide priority Qs
       * then remove the same from UE's lcsWithData List */
      rgSCHSc1DlRmFrmUeLcsWithData(cell, ue, ueDl->ambrSvc);
   }
   return;
}


/**
 * @brief This function removes a SVC from UEs AMBR LIST 
 *
 * @details
 *
 *     Function: rgSCHSc1DlSvcRmvFrmUeAmbrLst
 *     Purpose:  This function removes a SVC from UEs AMBR List.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchUeCb*    ue   
 *  @param[in]  RgSchDlLcCb*  svc
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlSvcRmvFrmUeAmbrLst(RgSchCellCb *cell,RgSchUeCb  *ue,RgSchDlLcCb  *svc)
{
   RgSchSc1DlUe *ueDl = RG_GET_SC1_UE_DL(ue, cell);
   RgSchSc1DlSvc *svcSc1 = RG_GET_SC1_SVC_DL(ue,svc,cell);
   CmLListCp            *lst;

   lst  = &ueDl->ambrLst;
   if (svcSc1->prioLnk.node != NULLP)
   {
      cmLListDelFrm(lst, &svcSc1->prioLnk);
      svcSc1->prioLnk.node = (PTR)NULLP;
   }
   return;
}


/**
 * @brief This function adds a SVC to UEs AMBR LIST 
 *
 * @details
 *
 *     Function: rgSCHSc1DlSvcAddToUeAmbrLst
 *     Purpose:  This function adds a SVC to UEs AMBR List.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchUeCb*    ue   
 *  @param[in]  RgSchDlLcCb*  svc
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlSvcAddToUeAmbrLst(RgSchCellCb *cell,RgSchUeCb *ue,RgSchDlLcCb *svc)
{
   RgSchSc1DlUe    *ueDl = RG_GET_SC1_UE_DL(ue, cell);
   CmLList         *node;
   RgSchDlLcCb     *lsvc;
   RgSchSc1DlSvc   *svcSc1 = RG_GET_SC1_SVC_DL(ue,svc,cell);
   RgSchCmnDlSvc   *svcCmn = RG_SCH_CMN_GET_DL_SVC(svc);


   /* If svc already present in AMBR List return */
   if (svcSc1->prioLnk.node != NULLP)
      return;

   node = ueDl->ambrLst.first;
   while(node)
   {
      lsvc = (RgSchDlLcCb *)(node->node);
      if (((RgSchCmnDlSvc*)(lsvc->sch))->prio > svcCmn->prio)
      {
         break;
      }
      node = node->next;
   }
   if (node == NULLP)
   {
      cmLListAdd2Tail(&ueDl->ambrLst, &svcSc1->prioLnk);
      svcSc1->prioLnk.node = (PTR)svc;
   }
   else
   {
      ueDl->ambrLst.crnt = node;
      cmLListInsCrnt(&ueDl->ambrLst, &svcSc1->prioLnk);
      svcSc1->prioLnk.node = (PTR)svc;
   }
   
   return;
}


/**
 * @brief This function removes a service from scheduler
 *
 * @details
 *
 *     Function: rgSCHSc1DlSvcRmvFrmSchd
 *     Purpose:  This function removes the SVC from the scheduler Qs.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue   
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlSvcRmvFrmSchd(RgSchCellCb *cell,RgSchDlLcCb  *svc)
{
   RgSchSc1DlCell *cellDl = RG_GET_SC1_CELL_DL(cell);
   RgSchSc1DlSvc *svcDl = RG_GET_SC1_SVC_DL(svc->ue,svc,cell);
   RgSchCmnDlSvc *svcCmn = RG_SCH_CMN_GET_DL_SVC(svc);
   CmLListCp    *lst;


   lst = &(cellDl->prioLst[svcCmn->prio]);
   if (svcDl->prioLnk.node != NULLP)
   {
      cmLListDelFrm(lst, &svcDl->prioLnk);
      svcDl->prioLnk.node = NULLP;
      /* If a svc is removed from cell wide priority Qs
       * then remove the same from UE's lcsWithData List */
      rgSCHSc1DlRmFrmUeLcsWithData(cell, svc->ue, svc);
   }
   return;
}


/**
 * @brief This function adds a service to scheduler for a UE
 *
 * @details
 *
 *     Function: rgSCHSc1DlSvcAdd
 *     Purpose:  This function is made available through a FP for 
 *               making scheduler aware of a service added to UE
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchUeCb*        ue   
 *  @param[in]  RgSchDlLcCb*      svc   
 *  @param[in]  CrgDlLchCfg*   qos
 *  @return                    Void 
 *
 **/
static Void rgSCHSc1DlSvcAdd(RgSchCellCb *cell,RgSchUeCb *ue,RgSchDlLcCb *svc,RgrDlLchCfg *cfg)
{
   RgSchSc1DlUe  *ueDl = RG_GET_SC1_UE_DL(ue, cell);
   RgSchSc1DlSvc *svcSc1 = RG_GET_SC1_SVC_DL(ue,svc,cell);
   RgSchCmnDlSvc *svcCmn = RG_SCH_CMN_GET_DL_SVC(svc);

   UNUSED(cfg);

   if (RG_SCH_CMN_SVC_IS_GBR(svc))
   {
      svcSc1->gbr   = svcCmn->gbr;
      svcSc1->mbr   = svcCmn->mbr;
      cmLListAdd2Tail(&ueDl->gbrSvcs, &svcSc1->gbrLnk);
      svcSc1->gbrLnk.node = (PTR)svc;
   }
   return;
}


/**
 * @brief This function deletes a service from scheduler
 *
 * @details
 *
 *     Function: rgSCHSc1DlLcRmv
 *     Purpose:  This function is made available through a FP for 
 *               making scheduler aware of a service being deleted from UE
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue   
 *  @param[in]  RgSchDlLcCb*  svc   
 *  @return  Void
 *
 **/
Void rgSCHSc1DlLcRmv(RgSchCellCb *cell,RgSchUeCb *ue,RgSchDlLcCb *svc)
{
   RgSchSc1DlUe  *ueDl;
   RgSchSc1DlSvc *svcSc1 = RG_GET_SC1_SVC_DL(ue,svc,cell);
   RgSchCmnDlSvc *svcCmn = RG_SCH_CMN_GET_DL_SVC(svc);


   if (svcSc1 == NULLP)
   {
      return;
   }
   ueDl = RG_GET_SC1_UE_DL(ue, cell);

   if (svcCmn->prio == 0)
   {
      rgSCHSc1DlSvcRmvFrmSchd(cell, svc);
   }
   else if (RG_SCH_CMN_SVC_IS_GBR(svc))
   {
      if (svcSc1->gbrLnk.node != NULLP)
      {
         cmLListDelFrm(&ueDl->gbrSvcs, &svcSc1->gbrLnk);
         svcSc1->gbrLnk.node = NULLP;
      }
      rgSCHSc1DlSvcRmvFrmSchd(cell, svc);
   }
   else /* if AMBR service */
   {
      if (ueDl->ambrSvc == svc)
      {
         rgSCHSc1DlUeRmvFrmSchd(cell, ue);
         rgSCHSc1DlSvcRmvFrmUeAmbrLst(cell, ue, svc);
         ueDl->ambrSvc = NULLP;
         if (ueDl->ambrLst.first != NULLP)
         {
            ueDl->ambrSvc = (RgSchDlLcCb *)(ueDl->ambrLst.first->node);
            ueDl->effAmbr = RGSCH_MIN(ueDl->ambr, svc->bo);
            if(ueDl->effAmbr)
            {
               rgSCHSc1DlUeAddToSchd(cell, ue);
            }
         }
      }
      else
      {
         rgSCHSc1DlSvcRmvFrmUeAmbrLst(cell, ue, svc);
      }
   }
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx, 
         (Data**)(&(RG_SCH_CMN_GET_LC_SCH_SPFC(ue,svc,cell))), (sizeof(RgSchSc1DlSvc)));
   return;
}

/**
 * @brief This function is invoked as part of SVC reconfig 
 *
 * @details
 *
 *     Function: rgSCHSc1DlSvcMod
 *     Purpose:  This function is made available through a FP for 
 *               making scheduler aware of a service reconfiguration. 
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchDlLcCb*      svc   
 *  @param[in]  CrgLchRecfg*   recfg
 *  @return     Void 
 *
 **/
static Void rgSCHSc1DlSvcMod(RgSchCellCb *cell,RgSchUeCb *ue,RgSchDlLcCb *svc,RgrLchRecfg  *recfg)
{ 
   RgSchSc1DlSvc *svcSc1 = RG_GET_SC1_SVC_DL(ue,svc,cell);
   RgSchCmnDlSvc *svcCmn = RG_SCH_CMN_GET_DL_SVC(svc);

   if (RG_SCH_CMN_SVC_IS_GBR(svc))
   {
      /* Convert the QOS to handle the refresh duration */
      svcSc1->gbr   = svcCmn->gbr;
      svcSc1->mbr   = svcCmn->mbr;
   }
   return;
}

/**
 * @brief This function adds UE to scheduler for an AMBR service
 *
 * @details
 *
 *     Function: rgSCHSc1DlUeAddToSchd
 *     Purpose:  This function adds a UE to scheduler for the AMBR
 *               service of highest priority.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchCellCb*      cell
 *  @param[in]  RgSchUeCb*        ue   
 *  @return                    Void
 *
 **/
static Void rgSCHSc1DlUeAddToSchd(RgSchCellCb *cell,RgSchUeCb *ue)
{
   RgSchSc1DlCell *cellDl = RG_GET_SC1_CELL_DL(cell);
   RgSchSc1DlUe   *ueDl = RG_GET_SC1_UE_DL(ue, cell);
   RgSchSc1DlUe   *lueDl;
   CmLList        *node;
   CmLListCp      *lst;
   RgSchUeCb      *nodeUe = NULLP;

   ueDl->prio = ((RgSchCmnDlSvc *)(ueDl->ambrSvc->sch))->prio;
   lst  = &cellDl->prioLst[ueDl->prio]; 
   /* if UE already in list, remove and
    * readjust */
   if (ueDl->prioLnk.node != NULLP)
   {
      cmLListDelFrm(lst, &ueDl->prioLnk);
      ueDl->prioLnk.node = NULLP;
      /* If a svc is removed from cell wide priority Qs
       * then remove the same from UE's lcsWithData List */
      rgSCHSc1DlRmFrmUeLcsWithData(cell, ue, ueDl->ambrSvc);
   }
   node = lst->first;
   while(node)
   {
      nodeUe = (RgSchUeCb *)(node->node);
      lueDl = RG_GET_SC1_UE_DL(nodeUe, cell);
      if (lueDl->effAmbr < ueDl->effAmbr)
         break;
      node = node->next;
   }
   if (node == NULLP)
   {
      cmLListAdd2Tail(lst, &ueDl->prioLnk);
      ueDl->prioLnk.node = (PTR)ue;
   }
   else
   {
      lst->crnt = node;
      cmLListInsCrnt(lst, &ueDl->prioLnk);
      ueDl->prioLnk.node = (PTR)ue;
   }
   /* If a svc is put in to cell wide priority Qs
    * then add the same to UE's lcsWithData List */
   rgSCHSc1DlAdd2UeLcsWithData(cell, ue, ueDl->ambrSvc);
   return;
}


/**
 * @brief This function implements managing BO for an ABMR service
 *
 * @details
 *
 *     Function: rgSCHSc1DlMngAmbrSvcPosn
 *     Purpose:  This function should be called whenever there is a 
 *               change BO for a AMBR service.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue   
 *  @param[in]  RgSchDlLcCb*  svc
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlMngAmbrSvcPosn(RgSchCellCb *cell,RgSchUeCb *ue,RgSchDlLcCb *svc)
{
   RgSchSc1DlUe    *ueDl = RG_GET_SC1_UE_DL(ue, cell);
   RgSchSc1DlSvc   *svcSc1 = RG_GET_SC1_SVC_DL(ue,svc,cell);

   if (svcSc1->bo == 0)
   {
      if (ueDl->ambrSvc == svc)
      {
         rgSCHSc1DlUeRmvFrmSchd(cell, ue);
         rgSCHSc1DlSvcRmvFrmUeAmbrLst(cell, ue, svc);
         ueDl->ambrSvc = NULLP;
         if (ueDl->ambrLst.first != NULLP)
         {
            ueDl->ambrSvc = (RgSchDlLcCb *)(ueDl->ambrLst.first->node);
            ueDl->effAmbr = RGSCH_MIN(ueDl->ambr, svcSc1->bo);
            if(ueDl->effAmbr)
            {
               rgSCHSc1DlUeAddToSchd(cell, ue);
            }
         }
      }
      else
      {
         rgSCHSc1DlSvcRmvFrmUeAmbrLst(cell, ue, svc);
      }
   }
   else /* svcSc1->bo != 0 */
   {
      if (svcSc1->prioLnk.node != NULLP)
      {
         if (svc == ueDl->ambrSvc)
         {
            ueDl->effAmbr = RGSCH_MIN(svcSc1->bo, ueDl->ambr);
            /* Update UE's position in the scheduler */
            if(ueDl->effAmbr)
            {
               rgSCHSc1DlUeAddToSchd(cell, ue);
            }
            else
            {
               rgSCHSc1DlUeRmvFrmSchd(cell, ue);
            }
         }
         return;
      }
      rgSCHSc1DlSvcAddToUeAmbrLst(cell, ue, svc);
      /* Current ambr svc is always the first node of ambrLst.*/ 
      if (ueDl->ambrLst.first->node == (PTR)svc)
      {
         if(ueDl->ambrSvc != svc)
         {
            if(ueDl->ambrSvc)
            {
               rgSCHSc1DlUeRmvFrmSchd(cell, ue);
            }
            ueDl->ambrSvc = svc;
            ueDl->effAmbr = RGSCH_MIN(ueDl->ambr, svcSc1->bo);
            if(ueDl->effAmbr)
            {
               rgSCHSc1DlUeAddToSchd(cell, ue);
            }
         }
      }
   }
   return;
}


/**
 * @brief This function updates the scheduler with service for a UE
 *
 * @details
 *
 *     Function: rgSCHSc1DlLcBoUpd
 *     Purpose:  This function should be called whenever there is a 
 *               change BO for a service.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue   
 *  @param[in]  RgSchDlLcCb*  svc
 *  @return  Void
 *
 **/
Void rgSCHSc1DlLcBoUpd(RgSchCellCb *cell,RgSchUeCb  *ue,RgSchDlLcCb *svc)
{
   RgSchSc1DlSvc   *svcSc1 = RG_GET_SC1_SVC_DL(ue,svc,cell);

   if (svcSc1->bo == svc->bo)
   {
      return;
   }
   svcSc1->bo = svc->bo;
   if (!RG_SCH_CMN_DL_IS_UE_ACTIVE(ue))
   {
      return;
   }
   rgSCHSc1DlMngSvcPosn(cell, ue, svc);

  /* Stack Crash problem for TRACE5 changes. Added the return below */
  return;

}


/**
 * @brief This function updates the scheduler with Prio0 service for a UE
 *
 * @details
 *
 *     Function: rgSCHSc1DlMngPrio0SvcPosn 
 *     Purpose:  This func shall be triggered whenever there is a
 *     change in the "Bo yet to be satisfied" field of the service.
 *     Appropriately positions the svc in its prio Q.
 *     Removes the SVC from the Q if BO is completely satisfied.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue   
 *  @param[in]  RgSchDlLcCb*  svc
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlMngPrio0SvcPosn(RgSchCellCb *cell,RgSchUeCb  *ue,RgSchDlLcCb  *svc)
{ 
   RgSchSc1DlSvc *svcSc1 = RG_GET_SC1_SVC_DL(ue,svc,cell);

   /* In this priority, we just add or remove to the queue */
   if (svcSc1->bo > 0)
   {
      rgSCHSc1DlSvcAddToSchd(cell, svc);
   }
   else
   {
      rgSCHSc1DlSvcRmvFrmSchd(cell, svc);
   }
   return;
}


/**
 * @brief This function updates the scheduler with GBR service for a UE
 *
 * @details
 *
 *     Function: rgSCHSc1DlMngGbrSvcPosn 
 *     Purpose:  This func shall be triggered whenever there is a
 *     change in the "Bo yet to be satisfied" field of the service.
 *     Appropriately positions the svc in its prio Q.
 *     Removes the SVC from the Q if BO is completely satisfied.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue   
 *  @param[in]  RgSchDlLcCb*  svc
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlMngGbrSvcPosn(RgSchCellCb *cell,RgSchUeCb *ue,RgSchDlLcCb *svc)
{ 
   RgSchSc1DlSvc *svcSc1 = RG_GET_SC1_SVC_DL(ue,svc,cell);

   /* Handle a GBR service. */
   svcSc1->effGbr = RGSCH_MIN(svcSc1->bo, svcSc1->gbr);
   svcSc1->effMbr = RGSCH_MIN(svcSc1->bo, svcSc1->mbr);
   /* Adjust the SVC priority within the queue */
   rgSCHSc1DlSvcRmvFrmSchd(cell, svc);
   rgSCHSc1DlSvcAddToSchd(cell, svc);
   return;
}


/**
 * @brief This function updates the scheduler with service for a UE
 *
 * @details
 *
 *     Function: rgSCHSc1DlMngSvcPosn 
 *     Purpose:  This func shall be triggered whenever there is a
 *     change in the "Bo yet to be satisfied" field of the service.
 *     Appropriately positions the svc in its prio Q.
 *     Removes the SVC from the Q if BO is completely satisfied.
 *     
 *     Invoked by: BO and Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue   
 *  @param[in]  RgSchDlLcCb*  svc
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlMngSvcPosn(RgSchCellCb *cell,RgSchUeCb  *ue,RgSchDlLcCb *svc)
{ 
   RgSchCmnDlSvc *svcCmn = RG_SCH_CMN_GET_DL_SVC(svc);
   RgSchSc1DlCell *cellDl = RG_GET_SC1_CELL_DL(cell);

   (cellDl->svcMngFunc[svcCmn->prio])(cell, ue, svc);
   return; 
}

/*--------------------------*
 * DL specific functions END
 *---------------------------*/



/**
 * @brief Scheduler processing on cell configuration
 *
 * @details
 *
 *     Function : rgSCHSc1RgrDlCellCfg
 *     
 *     This function does requisite initialisation 
 *     and setup for scheduler1 when a cell is
 *     configured
 *
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgrCellCfg    *cellCfg
 *  @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHSc1RgrDlCellCfg(RgSchCellCb *cell,RgrCellCfg *cellCfg,RgSchErrInfo *err)
{
   S16         ret;
   RgSchSc1DlCell *cellDl;

   if((ret = rgSCHUtlAllocSBuf(cell->instIdx, 
      (Data**)&(((RgSchCmnCell*)((cell)->sc.sch))->dl.schSpfc), \
      (sizeof(RgSchSc1DlCell)))) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation FAILED");
      err->errCause = RGSCHERR_SCH_SC1_DL_CFG;
      return (ret);
   }

   cellDl = RG_GET_SC1_CELL_DL(cell);
   /* Now perform downlink Queues related initializations  */
   rgSCHSc1DlInitQueues(cellDl);
   return ROK;
}  /* rgSCHSc1RgrDlCellCfg */

/***********************************************************
 *
 *     Func : rgSCHSc1DlDeinitQueues
 *        
 *     Desc : De-initialise downlink scheduler queues
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : 
 *
 **********************************************************/
static Void rgSCHSc1DlDeinitQueues(RgSchSc1DlCell *cellDl)
{
   uint8_t i;

   for (i = 0; i < RG_SC1_DL_NUM_Q; ++i)
   {
      cmLListInit(&cellDl->prioLst[i]);
      cmLListInit(&cellDl->retxLst[i]);
   } 
   return;
}


/**
 * @brief Scheduler processing for cell delete
 *
 * @details
 *
 *     Function : rgSCHSc1DlCellDel
 *     
 *     This functions de-initialises and frees memory
 *     taken up by scheduler1 for the entire cell.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @return  Void
 **/
Void rgSCHSc1DlCellDel(RgSchCellCb  *cell)
{

   if (((RgSchSc1DlCell *)((RgSchCmnCell*)((cell)->sc.sch))->dl.schSpfc) \
       == NULLP)
   {
      return;
   }
      
   /* Perform the deinit for the DL scheduler */
   rgSCHSc1DlDeinitQueues(RG_GET_SC1_CELL_DL(cell));
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx, 
      (Data**)(&(((RgSchCmnCell*)((cell)->sc.sch))->dl.schSpfc)),
      (sizeof(RgSchSc1DlCell)));
   return;
}  /* rgSCHSc1DlCellDel */

/**
 * @brief UE initialisation for scheduler
 *
 * @details
 *
 *     Function : rgSCHSc1RgrDlUeCfg
 *     
 *     This functions intialises UE specific scheduler
 *     information
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[int] RgrUeCfg     *ueCfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHSc1RgrDlUeCfg(RgSchCellCb  *cell,RgSchUeCb *ue,RgrUeCfg *ueCfg,RgSchErrInfo *err)
{
   RgSchCmnUe         *ueSchCmn = RG_SCH_CMN_GET_UE(ue, cell);
   Inst               inst = cell->instIdx;
   RgSchSc1DlUe       *ueDl;


   if((rgSCHUtlAllocSBuf(inst, 
               (Data**)&(ueSchCmn->dl.schSpfc), (sizeof(RgSchSc1DlUe))) != ROK))
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation FAILED"
               "CRNTI:%d",ue->ueId);
      err->errCause = RGSCHERR_SCH_SC1_DL_CFG;
      return RFAILED;
   }
   ueDl = (RgSchSc1DlUe *)ueSchCmn->dl.schSpfc;
   if (ue->dl.ambrCfgd)
   {
      ueDl->ambr = ue->dl.ambrCfgd;
   }
   else
   {
      ueDl->ambr = RG_SC1_MAX_DL_AMBR;
   }
   cmLListInit(&ueDl->lcsWithData);
   cmLListInit(&ueDl->gbrSvcs);
   cmLListInit(&ueDl->ambrLst);
   cmLListInit(&ueDl->schdSvcs);
   cmLListInit(&ueDl->retxHqProcs);
   return ROK;
}  /* rgSCHSc1RgrDlUeCfg */


/**
 * @brief Dl Harq Entity initialization for SC1
 *
 * @details
 *
 *     Function : rgSCHSc1DlUeHqEntInit
 *
 *     Processing Steps:
 *      - Create SC1 related information per Harq Entity
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHSc1DlUeHqEntInit(RgSchCellCb *cell,RgSchDlHqEnt *hqEnt)
{
   RgSchSc1DlHqProc   *hqSpcSch;
   RgSchDlHqProcCb    *hqP;
   uint8_t            cnt;
   /* making use of hqE->sch for one shot allocation 
    * of RgSchSc1DlHqProc structures */
   if (rgSCHUtlAllocSBuf(cell->instIdx, 
            (Data**)&(hqEnt->sch),
            (hqEnt->numHqPrcs * sizeof(RgSchSc1DlHqProc))) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation FAILED CRNTI:%d",hqEnt->ue->ueId);
      return RFAILED;
   }
   hqSpcSch = (RgSchSc1DlHqProc *)(hqEnt->sch);
   for(cnt = 0; cnt < hqEnt->numHqPrcs; cnt++)
   {
      hqP = &hqEnt->procs[cnt];
      ((RgSchCmnDlHqProc *)((hqP)->sch))->schSpfc = \
                                                    hqSpcSch++;
   }
   return ROK;
}

/**
 * @brief Dl Harq Entity deletion for Sc1
 *
 * @details
 *
 *     Function : rgSCHSc1DlUeHqEntDeInit 
 *
 *     Processing Steps:
 *      - Free SC1 related information per Harq Entity
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchDlHqEnt     *hqE 
 *  @return  Void
 **/
S16 rgSCHSc1DlUeHqEntDeInit(RgSchCellCb *cell,RgSchDlHqEnt *hqE)
{

   if(hqE->sch)
   {
      rgSCHUtlFreeSBuf(cell->instIdx,
      (Data**)(&(hqE->sch)),
      (hqE->numHqPrcs * sizeof(RgSchSc1DlHqProc)));
   }
   else
   {
      return RFAILED;
   }
   return ROK;
}
/**
 * @brief UE reconfiguration for scheduler
 *
 * @details
 *
 *     Function : rgSCHSc1RgrDlUeRecfg
 *     
 *     This functions updates UE specific scheduler
 *     information upon UE reconfiguration
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[int] RgrUeRecfg   *ueRecfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHSc1RgrDlUeRecfg(RgSchCellCb  *cell,RgSchUeCb  *ue,RgrUeRecfg  *ueRecfg,RgSchErrInfo *err)
{
   RgSchSc1DlUe *ueDl     = RG_GET_SC1_UE_DL(ue, cell);
   RgSchCmnDlUe *ueCmnDl = RG_SCH_CMN_GET_DL_UE(ue,cell);
                           /*cell added as part of CA dev*/
   RgSchDlHqEnt          *hqEnt = RG_SCH_CMN_GET_UE_HQE(ue, cell);

   if (ue->dl.ambrCfgd)
   {
      ueDl->ambr = ue->dl.ambrCfgd;
   }
   else
   {
      ueDl->ambr = RG_SC1_MAX_DL_AMBR;
   }

   /* Discarding TB2's context from scheduling Queues.
    * Since TB2 transmission needs signalling using 
    * TM specific formats. And since during this transient
    * period of UE TM Recfg, SCH always uses Format 1A,
    * the TB2s are discarded. */
   if (ueCmnDl->mimoInfo.forceTD & RG_SCH_CMN_TD_TXMODE_RECFG)
   {
      /* If HqP is in retx queue only for TB2 retx scheduling
       * then remove the harp proc from retx Queue */

      /* If Hqp is in retx queue for retx allocation of 
       * both TB1 and TB2, then reset TB2's state as ACKED */
      RgSchDlHqProcCb    *hqP;
      Pst                pst;
      RgInfRlsHqInfo     *rlsHqBufs = &(cell->rlsHqArr[cell->crntHqIdx]);
      uint8_t                 i;

      /* Prepare TB2 release information to be sent to MAC */
      rlsHqBufs->numUes = 0;
      for(i = 0; i < hqEnt->numHqPrcs; i++)
      {
         hqP = &hqEnt->procs[i];
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].rnti = ue->ueId;
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].hqProcId = hqP->procId;
         if (hqP->tbInfo[1].state == HQ_TB_NACKED)
         {
            if (hqP->tbInfo[0].state != HQ_TB_NACKED)
            {
               /* Remove the HqP from retx Queue.
                  Release HqP.*/
               rgSCHSc1DlProcRmvFrmCellRetx(cell, hqP);
               rgSCHSc1DlProcRmvFrmUeRetx(cell, ue, hqP);
            }
            rgSCHDhmRlsHqpTb(hqP, 1, TRUE);
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].tbId[0] = 2;
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs = 1;
         }
         rlsHqBufs->numUes++;
      }
      /* Send the hqProc list for MAC to clear TB1 contents */
      if (rlsHqBufs->numUes)
      {
         rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], cell->macInst);
         RgSchMacRlsHq (&pst, rlsHqBufs);
      }
   }
   return ROK;
}  /* rgSCHSc1RgrDlUeRecfg */

/**
 * @brief Removes UEs context from Priority Qs.
 *
 * @details
 *
 *     Function : rgSCHSc1DlRmvUeFrmPrioQs
 *     
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
static Void rgSCHSc1DlRmvUeFrmPrioQs(RgSchCellCb  *cell,RgSchUeCb    *ue)
{
   RgSchSc1DlUe         *sc1Ue;
   RgSchDlLcCb          *svc;
   uint32_t             idx;


   sc1Ue = RG_GET_SC1_UE_DL(ue, cell);

   /* Remove UE From DL priority queues */
   if (sc1Ue->ambrSvc != NULLP)
   {
      rgSCHSc1DlUeRmvFrmSchd(cell, ue);
   }

   for (idx = 0; idx < RGSCH_MAX_LC_PER_UE; ++idx)
   {
      svc = ue->dl.lcCb[idx];
      if (svc == NULLP)
      {
         continue;
      }
      rgSCHSc1DlSvcRmvFrmSchd(cell, svc);
   }
   
   return;
}  /* rgSCHSc1DlRmvUeFrmPrioQs */

/**
 * @brief Inactivate UE reason : measgap, acknaprept, poInactv.
 *
 * @details
 *
 *     Function : rgSCHSc1DlInactvtUe
 *     
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
static Void rgSCHSc1DlInactvtUe(RgSchCellCb  *cell,RgSchUeCb    *ue)
{
   RgSchSc1DlCell       *cellDl = RG_GET_SC1_CELL_DL(cell);
   RgSchDlHqProcCb      *hqP;
   RgSchCmnDlHqProc     *hqProcDl;
   uint8_t              i;
   RgSchDlHqEnt         *hqEnt = RG_SCH_CMN_GET_UE_HQE(ue, cell);


   /* ccpu00130170: UE related HARQ Procs are cleared only 
      if UE's Re-establishment procedure is not in progress*/
   if(!(ue->dl.dlInactvMask & RG_HQENT_INACTIVE))
   {
      /* remove all in use HARQ processes from the subframes.
       * Store them in UEs hqProc Lst. Add back to cell's
       * retx lst when UE is activated again. */
      for(i = 0; i < hqEnt->numHqPrcs; i++)
      {
         hqP = &hqEnt->procs[i];
         hqProcDl = RG_SCH_CMN_GET_DL_HQP(hqP);
         /* Remove retx procs from cell's list and 
          * add them to UE's List */
         if(
#ifdef LTEMAC_SPS
            !(RG_SCH_CMN_SPS_DL_IS_SPS_HQP(hqP)) &&
#endif
            hqProcDl->retxLnk.node != NULLP)
         {
            cmLListDelFrm(&cellDl->retxLst[((RgSchSc1DlHqProc *)\
                       (hqProcDl->schSpfc))->prio], &(hqProcDl->retxLnk));
            hqProcDl->retxLnk.node = NULLP;
            rgSCHSc1DlProcAddToUeRetx(cell, ue, hqP);
         }
      }
   }

   rgSCHSc1DlRmvUeFrmPrioQs(cell, ue);

   return;
}  /* rgSCHSc1DlInactvtUe */


/**
 * @brief UE suspension.
 *
 * @details
 *
 *     Function : rgSCHSc1DlSuspendUe 
 *     
 *     Removes UE, its SVCs and its HqPs from CELL WIDE
 *     PrioQs and Retx Qs Respectively.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
static Void rgSCHSc1DlSuspendUe(RgSchCellCb  *cell,RgSchUeCb    *ue)
{
   RgSchDlHqProcCb      *hqP;
   uint8_t              i;
   uint8_t              j;
   RgSchDlHqEnt         *hqEnt = RG_SCH_CMN_GET_UE_HQE(ue, cell);


   /* remove all in use HARQ processes from the subframes.
    * Store them in UEs hqProc Lst. Add back to cell's
    * retx lst when UE is activated again. */
   for(i = 0; i < hqEnt->numHqPrcs; i++)
   {
      hqP = &hqEnt->procs[i];
      rgSCHSc1DlProcRmvFrmCellRetx(cell, hqP);
      rgSCHSc1DlProcRmvFrmUeRetx(cell, ue, hqP);
      /* Removing the Harq Proc from subframes list */
      if (hqP->hqPSfLnk.node != NULLP)
      {
         if (hqP->pdcch )
         {
            cmLListDelFrm(&hqP->subFrm->pdcchInfo.pdcchs, 
                  &hqP->pdcch->lnk);
            cmLListAdd2Tail(&cell->pdcchLst, &hqP->pdcch->lnk);
            hqP->pdcch = NULLP;
         }
         /*CA DEV Start */
         rgSCHUtlDlHqPTbRmvFrmTx(hqP->subFrm,hqP,0,FALSE);
         /*CA DEV End*/ 
         for (j = 0; j < 2; j++)
         {
            if (hqP->tbInfo[j].state == HQ_TB_WAITING)
            {
              rgSCHDhmRlsHqpTb(hqP, j, TRUE);
            }
         }
      }
   }
   rgSCHSc1DlRmvUeFrmPrioQs(cell, ue);

   return;
}  /* rgSCHSc1DlSuspendUe */

/***********************************************************
 *
 *     Func : rgSCHSc1DlScanUpdPdbPrio
 *
 *     Desc : Increment the pivot and reposition the LCs under the pivot to 
 *            new location according to thieir PDB and elapsed time.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
Void rgSCHSc1DlScanUpdPdbPrio(RgSchCellCb *cell)
{
   
   return;
}

/**
 * @brief Function to update Flow control information 
 *        to be sent to MAC.
 *
 * @details
 *
 *     Function: rgSCHSc1DlFillFlowCntrlInfo
 *
 *              update Flow control information 
 *
 *     Invoked by: 
 *         SCHD
 *
 *     Processing Steps:
 *           
 *  @param[in] RgSchCellCb       *cell
               RgInfSfAlloc    *sfAlloc;
 *  @return  S16
 *      -# ROK 
 **/
S16 rgSCHSc1DlFillFlowCntrlInfo(RgSchCellCb    *cell,RgInfSfAlloc   *sfAlloc)
{
  return ROK;
}
/**
 * @brief UE deletion for scheduler
 *
 * @details
 *
 *     Function : rgSCHSc1DlUeDel
 *     
 *     This functions deletes all scheduler information
 *     pertaining to a UE
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
Void rgSCHSc1DlUeDel(RgSchCellCb  *cell,RgSchUeCb    *ue)
{
   RgSchDlHqEnt          *hqEnt = RG_SCH_CMN_GET_UE_HQE(ue, cell);
   RgSchSc1DlUe *sc1DlUe = RG_GET_SC1_UE_DL(ue, cell);


   if (sc1DlUe == NULLP)
   {
      return;
   }
   if( hqEnt)
   {
      /* Remove UEs scheduler context */
      rgSCHSc1DlSuspendUe(cell, ue);

      /* Free all SC1 specific control blocks */
      if (hqEnt->sch != NULLP)
      {
         /* ccpu00117052 - MOD - Passing double pointer
            for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(cell->instIdx, 
               (Data**)(&(hqEnt->sch)), 
               (hqEnt->numHqPrcs * sizeof(RgSchSc1DlHqProc)));
      }
   }

   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx, (Data**)(&sc1DlUe), (sizeof(RgSchSc1DlUe))); 

   return;
}  /* rgSCHSc1DlUeDel */

/**
 * @brief Scheduler invocation on Downlink logical channel addition
 *
 * @details
 *
 *     Function : rgSCHSc1RgrLcCfg
 *     
 *     This functions does required processing when a new
 *     (dedicated) logical channel is added. 
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgSchDlLcCb  *dlLc
 *  @param[int] RgrLchCfg    *lcCfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHSc1RgrLcCfg(RgSchCellCb  *cell,RgSchUeCb    *ue,RgSchDlLcCb  *dlLc,RgrLchCfg *lcCfg,RgSchErrInfo *err)
{
   S16      ret;

   ret = rgSCHUtlAllocSBuf(cell->instIdx, 
      (Data**)&(RG_SCH_CMN_GET_LC_SCH_SPFC(ue,dlLc,cell)), \
      (sizeof(RgSchSc1DlSvc)));
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : rgSCHSc1CrgLcCfg():"
      "SCH struct alloc failed CRNTI:%d LCID:%d",ue->ueId,lcCfg->lcId);
      err->errCause = RGSCHERR_SCH_SC1_DL_CFG;
      return (ret);
   }

   rgSCHSc1DlSvcAdd(cell, ue, dlLc, &lcCfg->dlInfo);
   return ROK;
}  /* rgSCHSc1RgrLcCfg */


/**
 * @brief Scheduler invocation on logical channel addition
 *
 * @details
 *
 *     Function : rgSCHSc1RgrLcRecfg
 *     
 *     This functions does required processing when an existing
 *     (dedicated) logical channel is reconfigured. Assumes lcg
 *     pointer in ulLc is set to the old value.
 *     Independent of whether new LCG is meant to be configured,
 *     the new LCG scheduler info is accessed and possibly modified.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgSchDlLcCb  *dlLc
 *  @param[int] RgrLchRecfg  *lcRecfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHSc1RgrLcRecfg(RgSchCellCb  *cell,RgSchUeCb  *ue,RgSchDlLcCb  *dlLc,RgrLchRecfg  *lcRecfg,RgSchErrInfo *err)
{

   UNUSED(err);

   rgSCHSc1DlSvcMod(cell,ue,dlLc, lcRecfg);

   return ROK;
}  /* rgSCHSc1RgrLcRecfg */


/**
 * @brief This function handles the reconfiguration of cell 
 *
 * @details
 *
 *     Function: rgSCHSc1RgrDlCellRecfg
 *     Purpose:  Update the reconfiguration parameters.
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @return  Void
 *
 **/
static S16 rgSCHSc1RgrDlCellRecfg(RgSchCellCb *cell,RgrCellRecfg *recfg,RgSchErrInfo *err)
{
   return ROK;
}



/**
 * @brief This function implements scheduler DL allocation
 *
 * @details
 *
 *     Function: rgSCHSc1DlTaCmd
 *     Purpose:  This function implements scheduler for TA cmd alloc for
 *               UEs. The hq proc availed as part of this alloc can be used 
 *               by the UEs Dedicated CH transmission allocation.
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb*     cell
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo 
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlTaCmd(RgSchCellCb *cell,RgSchCmnDlRbAllocInfo *allocInfo)
{
   CmLListCp            *lst;
   CmLList              *node;
   RgSchDlHqProcCb      *proc;
   RgSchUeCb            *ue;
   uint32_t             effBo;
   RgSchCmnDlCell       *cellCmnDl = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchCmnDlUe         *cmnUeDl; 
   RgSchSc1DlUe         *ueDl;
   RgSchCmnDlHqProc     *cmnHqDl;
   RgSchDlSf            *subFrm = allocInfo->dedAlloc.dedDlSf;
#ifdef LTEMAC_HDFDD
   Bool                 dlAllowed = FALSE;
#endif

   lst  = &cellCmnDl->taLst;
   node = lst->first;
   while(node)
   {
      ue = (RgSchUeCb *)node->node;
      node = node->next;
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddChkDlAllow ( cell, ue, &dlAllowed);
         if (dlAllowed == FALSE)
         {
            continue;
         }
      }
#endif
      /* If Ue is inactive in DL then ignore */
      if (ue->dl.dlInactvMask)
      {
         continue;
      }
      cmnUeDl = RG_SCH_CMN_GET_DL_UE(ue,cell);
                /*cell added as part of CA dev*/
      ueDl = RG_GET_SC1_UE_DL(ue, cell);

      if (rgSCHDhmGetAvlHqProc(cell, ue, cellCmnDl->time, &proc) != ROK)
      {
         continue;    
      }
      /* Initialize some of the parameters of the HQ proc */
      cmnHqDl = RG_SCH_CMN_GET_DL_HQP(proc);

      effBo = 0;
      /* 3.1 MIMO */
      cmnHqDl->totBytes = 0;
      rgSCHCmnDlAllocTxRb(cell, subFrm, ue, RGSCH_TA_SIZE, &effBo, proc, allocInfo);
      if (effBo == 0)
      {
         /* If no allocations so far, meaning proc obtained now */
         if (cmnHqDl->totBytes == 0)
         {
            rgSCHSc1RlsHqProc(proc);
            /* Added the handling for removing
             * UE from txHqPLst and resetting outStndalloc.*/
            if(proc->reqLnk.node != (PTR)NULLP)
            {
               cmLListDelFrm(&allocInfo->dedAlloc.txHqPLst, &proc->reqLnk);
               proc->reqLnk.node = (PTR)NULLP;
            }
            /*Re-set the outstanding alloc information.*/
            cmnUeDl->outStndAlloc = 0;
         }
         /* Avl BW could not satisfy even TA so break */
         break;
      }
      ueDl->taReqBytes = RGSCH_TA_SIZE;
      cmnUeDl->proc = proc;
      cmnHqDl->totBytes += effBo;
      /* 3.1 MIMO moving this call to cmn scheduler */
      /*rgSCHCmnDlRbInfoAddUeTx(allocInfo, ue);*/
   }
   return;
}

/**
 * @brief Scheduler invocation
 *
 * @details
 *
 *     Function: rgSCHSc1DlHndlInActUes
 *     Purpose:  The list of inactive UEs present in inactvLst should
 *               be removed from the scheduling Qs.
 *     
 *     Invoked by: Common Scheduler (TTI processing)
 *     
 *  @param[in]  RgSchCellCb *cell
 *  @param[out] CmLListCp   *inactvLst
 *  @return  Void
 **/
Void rgSCHSc1DlHndlInActUes(RgSchCellCb *cell,CmLListCp *inactvLst)
{
   CmLList   *node;
   RgSchUeCb *ue;

   node = inactvLst->first;
   while(node)
   {
      ue = (RgSchUeCb *)node->node;
      node = node->next;
      /* Suspend this UE from further scheduling
       * till it is activated again. */
      rgSCHSc1DlInactvtUe(cell, ue);
   }
   return;
}

/**
 * @brief This function initializes all the data for the scheduler
 *
 * @details
 *
 *     Function: rgSCHSc1DlInit
 *     Purpose:  This function initializes the following information
 *               1. Efficiency table
 *               2. CQI to table index - It is one row for upto 3 RBs
 *                  and another row for greater than 3 RBs
 *     
 *               currently extended prefix is compiled out.
 *     Invoked by: MAC intialization code..may be ActvInit
 *     
 *  @return  Void
 *
 **/
Void rgSCHSc1DlInit(RgDlSchdApis *rgSchDlApis)
{
   /* Init the function pointers */
   rgSchDlApis->rgSCHRgrDlUeCfg        = rgSCHSc1RgrDlUeCfg;
   rgSchDlApis->rgSCHRgrDlUeRecfg      = rgSCHSc1RgrDlUeRecfg;
   rgSchDlApis->rgSCHFreeDlUe          = rgSCHSc1DlUeDel;
   rgSchDlApis->rgSCHRgrDlCellCfg      = rgSCHSc1RgrDlCellCfg;
   rgSchDlApis->rgSCHRgrDlCellRecfg    = rgSCHSc1RgrDlCellRecfg; 
   rgSchDlApis->rgSCHFreeDlCell        = rgSCHSc1DlCellDel;
   rgSchDlApis->rgSCHRgrDlLcCfg        = rgSCHSc1RgrLcCfg;
   rgSchDlApis->rgSCHRgrDlLcRecfg      = rgSCHSc1RgrLcRecfg;
   rgSchDlApis->rgSCHFreeDlLc          = rgSCHSc1DlLcRmv;
   rgSchDlApis->rgSCHDlNewSched        = rgSCHSc1DlDedNewTx;
   rgSchDlApis->rgSCHDlPreSched        = rgSCHSc1DlPreSchd;
   rgSchDlApis->rgSCHDlPstSched        = rgSCHSc1DlPstSchd;
   rgSchDlApis->rgSCHDlRetxSched       = rgSCHSc1DlDedRetx;
   rgSchDlApis->rgSCHDlCeSched         = rgSCHSc1DlTaCmd;
   rgSchDlApis->rgSCHDlDedBoUpd        = rgSCHSc1DlLcBoUpd;
   rgSchDlApis->rgSCHDlProcAddToRetx   = rgSCHSc1DlProcAddToCellRetx;
   rgSchDlApis->rgSCHDlAllocFnlz       = rgSCHSc1DlAllocFnlz;
   rgSchDlApis->rgSCHDlCqiInd          = rgSCHSc1DlCqiInd;
   rgSchDlApis->rgSCHDlUeRefresh       = rgSCHSc1DlUeRefresh;
   rgSchDlApis->rgSCHDlUeReset         = rgSCHSc1DlUeReset;
   rgSchDlApis->rgSCHDlActvtUe         = rgSCHSc1DlActvtUe;
   rgSchDlApis->rgSCHDlInactvtUes      = rgSCHSc1DlHndlInActUes;
   rgSchDlApis->rgSCHDlUeHqEntInit     = rgSCHSc1DlUeHqEntInit;
   rgSchDlApis->rgSCHDlUeHqEntDeInit   = rgSCHSc1DlUeHqEntDeInit;
   rgSchDlApis->rgSCHDlProcRmvFrmRetx  = rgSCHSc1DlProcRmvFrmRetx; 
   rgSchDlApis->rgSCHDlTickForPdbTrkng = rgSCHSc1DlScanUpdPdbPrio;
   rgSchDlApis->rgSCHDlFillFlwCtrlInfo = rgSCHSc1DlFillFlowCntrlInfo; 

   return;
}




/***********************************************************
 *
 *     Func : rgSCHSc1DlInitQueues
 *        
 *     Desc : Initial downlink scheduler queues
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : 
 *
 **********************************************************/
static Void rgSCHSc1DlInitQueues(RgSchSc1DlCell *cellDl)
{
   uint8_t i;

   for (i = 0; i < RG_SC1_DL_NUM_Q; ++i)
   {
      cmLListInit(&cellDl->prioLst[i]);
      cmLListInit(&cellDl->retxLst[i]);
   }
   /* Set appropriate "manage svc positioning" function based on
    * svc priority as array index */
   /* for DCCH svcs */
   for (i = 0; i < RG_SCH_SC1_DL_GBR_PRIO_START; i++)
   {
      cellDl->svcMngFunc[i] = rgSCHSc1DlMngPrio0SvcPosn;
   }
   /* for GBR svcs */
   for (i = RG_SCH_SC1_DL_GBR_PRIO_START; i <= RG_SCH_SC1_DL_GBR_PRIO_END; i++)
   {
      cellDl->svcMngFunc[i] = rgSCHSc1DlMngGbrSvcPosn;
   }
   /* for Non-GBR svcs */
   for (i = RG_SCH_SC1_DL_GBR_PRIO_END+1; i <= RG_SCH_CMN_MAX_PRIO; i++)
   {
      cellDl->svcMngFunc[i] = rgSCHSc1DlMngAmbrSvcPosn;
   }
   return;
}




/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        RETX allocations. 
 *
 * @details
 *
 *     Function: rgSCHSc1DlRetxAllocFnlz 
 *     Purpose : Remove the Retx Hq Proc from the Cell's
 *     Retx list, if allocation is successful.
 *     Fill the HqProc and PDCCH and append it to the SubFrm.
 *     
 *               
 *     
 *     Invoked by: Common Scheduler 
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlRetxAllocFnlz(RgSchCellCb *cell,RgSchCmnDlRbAllocInfo *allocInfo)
{
   CmLList          *node;
   RgSchUeCb        *ue;
   RgSchDlHqProcCb  *hqP;
   RgSchDlRbAlloc *dlAllocCb = NULLP;

   node = allocInfo->dedAlloc.schdRetxHqPLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)node->node;
      ue = hqP->hqE->ue;
      node = node->next;
      /* Fill PDCCH and assign it to HqP */
      dlAllocCb = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue, cell);

      rgSCHCmnFillHqPPdcch(cell, dlAllocCb, hqP);
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddUpdDLMark(cell, ue);
      }
#endif
      /* Extra Check: Retain the hqProc in the RETX Queue if one/more
       * TBs of the HqProc are yet to be scheduled for RETX.
       * Note: Here we are not tracking at TB Level, the priority Q
       * to which it belongs to. The retx prio of transmission is still
       * being maintained at hqProc level, rather than at TB level */
      if ((hqP->tbInfo[0].state != HQ_TB_NACKED) && 
            (hqP->tbInfo[1].state != HQ_TB_NACKED)) {
         rgSCHSc1DlProcRmvFrmCellRetx(cell, hqP);
      }
      /* reset the UE allocation Information */
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }

   /* Traverse the nonSchdTxUeLst and reset the UE allocation Info */
   node = allocInfo->dedAlloc.nonSchdRetxHqPLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)node->node;
      ue = hqP->hqE->ue;
      node = node->next;
      /* reset the UE allocation Information */
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }
   return;
}

/* 3.1 MIMO Alloc distribution functions being performed
 * TB wise */


/***********************************************************
 *
 *     Func : rgSCHSc1DlSprTxTbDstn 
 *        
 *     Desc : Perform Actual allocation distribution among
 *     UEs schd svcs and TA for a given spare TB "tbInfo" allocation.
 *     spare TB allocation is as a result of 1 RETX TB allocation, when
 *     conditions are favourable for 2 TB spatial multiplexing.
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
static Void rgSCHSc1DlSprTxTbDstn(RgSchCellCb *cell,RgSchUeCb *ue,RgSchDlHqTbCb  *tbInfo,uint32_t  *effAlloc,CmLList **node)
{
   RgSchDlLcCb        *svc;
   RgSchSc1DlSvc      *svcSc1;
   RgSchSc1DlUe       *ueDl = RG_GET_SC1_UE_DL(ue, cell);
   uint32_t           bytes;
   RgSchLchAllocInfo  lchSchdData;
   uint32_t           effBo;
   uint32_t           rlcHdrEstmt;


   while((*node) && (*effAlloc > 0))
   {
      svc = (RgSchDlLcCb *)(*node)->node;
      *node = (*node)->next;
      svcSc1 = RG_GET_SC1_SVC_DL(ue,svc,cell);
      
      RG_SCH_CMN_DL_GET_HDR_EST(svc, rlcHdrEstmt);
      /* Update the SVC QOS Param */
      if (RG_SCH_CMN_SVC_IS_GBR(svc))
      {
         effBo = svcSc1->effMbr + rlcHdrEstmt;
         bytes = RGSCH_MIN(*effAlloc, effBo);
         /* Determine How much BO is satisfied */
         if (bytes <= rlcHdrEstmt)
         {
            break; 
         }
         (svcSc1->bo <= bytes-rlcHdrEstmt)?\
           (svcSc1->bo = 0):\
           (svcSc1->bo -= bytes-rlcHdrEstmt);
         svc->bo = svcSc1->bo;

         /* L2_COUNTERS */
#ifdef LTE_L2_MEAS
         /* Increment qciActiveLCs once since this LCs buffer will be present
            in Harq process */
         if (svc->lcType == CM_LTE_LCH_DTCH)
         {
            ue->qciActiveLCs[svc->qciCb->qci]++;
         }

         if ((svc->bo == 0) && (svc->lcType == CM_LTE_LCH_DTCH))
         {
            if (ue->qciActiveLCs[svc->qciCb->qci])
            {
               ue->qciActiveLCs[svc->qciCb->qci]--;
            }
            if (!(ue->qciActiveLCs[svc->qciCb->qci]))
            {
               svc->qciCb->dlUeCount--;
            }
         }
#endif
         (svcSc1->gbr <= bytes)? (svcSc1->gbr = 0):
                                    (svcSc1->gbr -= bytes);
         (svcSc1->mbr <= bytes)? (svcSc1->mbr = 0):
                                    (svcSc1->mbr -= bytes);
      }
      else if(RG_SCH_CMN_SVC_IS_AMBR(svc))
      {
         effBo = ueDl->effAmbr + rlcHdrEstmt;
         bytes = RGSCH_MIN(*effAlloc, effBo);
         /* Determine How much BO is satisfied */
         if (bytes <= rlcHdrEstmt)
         {
            break; 
         }
         (svcSc1->bo <= bytes-rlcHdrEstmt)?\
           (svcSc1->bo = 0):\
           (svcSc1->bo -= bytes-rlcHdrEstmt);

         (ueDl->ambr <= bytes)? (ueDl->ambr = 0):
                                       (ueDl->ambr -= bytes);
      }
      else /* Prio 0 SVC */
      {
         effBo = svcSc1->bo + rlcHdrEstmt;
         bytes = RGSCH_MIN(*effAlloc, effBo);
         /* Determine How much BO is satisfied */
         if (bytes <= rlcHdrEstmt)
         {
            break; 
         }
         (svcSc1->bo <= bytes-rlcHdrEstmt)?\
           (svcSc1->bo = 0):\
           (svcSc1->bo -= bytes-rlcHdrEstmt);
      }
      /* Position the service accordingly */
      rgSCHSc1DlMngSvcPosn(cell, svc->ue, svc);
      /* Update effAlloc */
      *effAlloc -= bytes;

      /* Update DHM for this SVC */
      lchSchdData.lcId     = svc->lcId;
      lchSchdData.schdData = bytes;
      rgSCHDhmAddLcData(cell->instIdx, &lchSchdData, tbInfo);
   }
   return;
}


/***********************************************************
 *
 *     Func : rgSCHSc1DlNewTxTbDstn 
 *        
 *     Desc : Perform Actual allocation distribution among
 *     UEs schd svcs and TA for a given TB "tbInfo" allocation.
 *     Assumption: TA is given higher priority in Alloc Distribution.
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
static Void rgSCHSc1DlNewTxTbDstn(RgSchCellCb *cell,RgSchUeCb *ue,RgSchDlHqTbCb  *tbInfo,uint32_t *effAlloc,CmLList **node)
{
   RgSchDlLcCb        *svc;
   RgSchSc1DlSvc      *svcSc1 = NULLP;
   RgSchSc1DlUe       *ueDl = RG_GET_SC1_UE_DL(ue, cell);
   uint32_t           bytes;
   RgSchLchAllocInfo  lchSchdData;
   CmLList            *prev = NULLP;


   if (ueDl->taReqBytes)
   {
      if (ueDl->taReqBytes < *effAlloc)
      {
         /*TA satisfied, hence remove from TA Lst */
         rgSCHCmnRmvFrmTaLst(cell, ue);
         /* Indicate to DHM that TA has been scheduled */
         rgSCHDhmSchdTa(ue, tbInfo);
         *effAlloc -= ueDl->taReqBytes;
      }
      /* Reset the TA Req Bytes Field */
      ueDl->taReqBytes = 0;
   }
   while((*node) && (*effAlloc > 0))
   {
      svc = (RgSchDlLcCb *)(*node)->node;
      prev = *node;
      *node = (*node)->next;
      svcSc1 = RG_GET_SC1_SVC_DL(ue,svc,cell);
      if (*effAlloc > svcSc1->reqBytes)
      {
         bytes = svcSc1->reqBytes;
         if (bytes <= svcSc1->hdrEstimate)
         {
            break; 
         }
         /* 3.1 MIMO updating the reqBytes field */
         svcSc1->reqBytes = 0;
         svcSc1->bo = 0;
      }
      else
      {
         bytes = *effAlloc;
         if (bytes <= svcSc1->hdrEstimate)
         {
            break; 
         }
         /* 3.1 MIMO updating the reqBytes field */
         svcSc1->reqBytes -= bytes;
         (svcSc1->bo <= bytes-svcSc1->hdrEstimate)?\
           (svcSc1->bo = 0):\
           (svcSc1->bo -= bytes-svcSc1->hdrEstimate);
      }
      svc->bo = svcSc1->bo;

      /* L2_COUNTERS */
#ifdef LTE_L2_MEAS

      /* Increment qciActiveLCs once since this LCs buffer will be present
         in Harq process */
      if (svc->lcType == CM_LTE_LCH_DTCH)
      {
         ue->qciActiveLCs[svc->qciCb->qci]++;
      }

      if ((svc->bo == 0) && (svc->lcType == CM_LTE_LCH_DTCH))
      {
         if (ue->qciActiveLCs[svc->qciCb->qci])
         {
            ue->qciActiveLCs[svc->qciCb->qci]--;
         }
         if (!(ue->qciActiveLCs[svc->qciCb->qci]))
         {
            svc->qciCb->dlUeCount--;
         }
      }
#endif

      /* Update the SVC QOS Param */
      if (RG_SCH_CMN_SVC_IS_GBR(svc))
      {
         (svcSc1->gbr <= bytes)? (svcSc1->gbr = 0):
                                    (svcSc1->gbr -= bytes);
         (svcSc1->mbr <= bytes)? (svcSc1->mbr = 0):
                                    (svcSc1->mbr -= bytes);
      }
      else if(RG_SCH_CMN_SVC_IS_AMBR(svc))
      {
         (ueDl->ambr <= bytes)? (ueDl->ambr = 0):
                                       (ueDl->ambr -= bytes);
      }
      /* Position the service accordingly */
      rgSCHSc1DlMngSvcPosn(cell, svc->ue, svc);
      /* Update effAlloc */
      *effAlloc -= bytes;

      /* Update DHM for this SVC */
      lchSchdData.lcId     = svc->lcId;
      lchSchdData.schdData = bytes;
      rgSCHDhmAddLcData(cell->instIdx, &lchSchdData, tbInfo);
   }
   /* If no more scheduled LCs for TB data distribution
    * then distribute the spare TB data among the LCs
    * of the UE with non-zero BO. This is effective for 
    * schedulers work on LC level priorities rather than
    * UE level. */
   if ((*node == NULLP) && (svcSc1) && (svcSc1->reqBytes == 0))
   {
      rgSCHSc1DlSprTxTbDstn(cell, ue, tbInfo, effAlloc,
                            &ueDl->lcsWithData.first);
      *node = NULLP;
      return;
   }
   /* make sure node points to the svc not completely
    * satisfied.
    * make sure if not served completely then 
    * the other TB allocation accomodates the same */
   *node = prev;
   return;
}



/***********************************************************
 *
 *     Func : rgSCHSc1DlNewTxUeFnlz 
 *        
 *     Desc : Perform allocation Distribution from scheduled TB
 *            among the list of services considered for scheduling.
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
static Void rgSCHSc1DlNewTxUeFnlz (RgSchCellCb  *cell,RgSchCmnDlRbAllocInfo *allocInfo,RgSchUeCb *ue)
{
   CmLList            *node;
   RgSchSc1DlUe       *ueDl = RG_GET_SC1_UE_DL(ue, cell);
   RgSchCmnDlUe       *cmnUeDl = RG_SCH_CMN_GET_DL_UE(ue,cell);
                                 /*cell added as part of CA dev*/
   /* 3.1 MIMO Distribute data of each TB across services */
   RgSchDlRbAlloc *dlAllocCb = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue, cell);
   uint32_t           remTb1Bytes = dlAllocCb->tbInfo[0].bytesAlloc;
   uint32_t           remTb2Bytes = dlAllocCb->tbInfo[1].bytesAlloc;
   uint32_t           effAlloc = 0;
   /*ccpu00120365-ADD-added to code to check if second TB is utilized */
   uint32_t           tb2Bytes = 0;



   /* 3.1 MIMO Consider the allocation of New TX TB for distribution */
   /* Handle schd services */
   node = ueDl->schdSvcs.first;
   if (remTb1Bytes){
      effAlloc += remTb1Bytes;
      rgSCHSc1DlNewTxTbDstn(cell, ue, &cmnUeDl->proc->tbInfo[0],\
            &remTb1Bytes, &node);
      /* In the event that TB1 is not completely filled by the DL LCs
       * BO, consider the reducing the iMcs for increasing redundancy
       * and hence reception quality at UE */
      rgSCHCmnRdcImcsTxTb(dlAllocCb, 0, 
            dlAllocCb->tbInfo[0].bytesAlloc - remTb1Bytes);
   }
   
   /*ccpu00120365-ADD-assigning value of remTb2Bytes before utilization */
   tb2Bytes = remTb2Bytes;
   
   /* Extra check for a non SM UE allocation */
   if (remTb2Bytes){
      effAlloc += remTb2Bytes;
      rgSCHSc1DlNewTxTbDstn(cell, ue, &cmnUeDl->proc->tbInfo[1],\
            &remTb2Bytes, &node);
      /* In the event that TB2 is not completely filled by the DL LCs
       * BO, consider the reducing the iMcs for increasing redundancy
       * and hence reception quality at UE */
      rgSCHCmnRdcImcsTxTb(dlAllocCb, 1, 
            dlAllocCb->tbInfo[1].bytesAlloc - remTb2Bytes);
   }

   /* ccpu00120365-ADD-Disable the second TB as the second TB is not
    * utilized */
   if ( remTb2Bytes && ( tb2Bytes == remTb2Bytes) )
   {
      dlAllocCb->mimoAllocInfo.precIdxInfo = 0;
      dlAllocCb->mimoAllocInfo.numTxLyrs   = 1;
      dlAllocCb->tbInfo[1].schdlngForTb    = FALSE;
      dlAllocCb->tbInfo[1].isDisabled      = TRUE;
   }

   if (effAlloc == (remTb1Bytes + remTb2Bytes))
   {
      /* Allocation such that Nothing could be satisfied */
      /* Return the grabbed PDCCH */
      rgSCHUtlPdcchPut(cell, &dlAllocCb->dlSf->pdcchInfo, 
            dlAllocCb->pdcch);
      rgSCHSc1RlsHqProc(cmnUeDl->proc);
      return;
   }

   /* Fill PDCCH and assign it to HqP */
   rgSCHCmnFillHqPPdcch(cell, dlAllocCb, cmnUeDl->proc);

   return;
}


/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        New TX allocations. 
 *
 * @details
 *
 *     Function: rgSCHSc1DlNewTxAllocFnlz 
 *     Purpose : Distribute the allocation among the Scheduled SVCs.
 *     Fill pdcch and HqP for UEs will allocations.
 *     Release HqP for UE with no allocation.
 *
 *     Invoked by: Common Scheduler 
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlNewTxAllocFnlz (RgSchCellCb  *cell,RgSchCmnDlRbAllocInfo *allocInfo)
{
   CmLList          *node;
   RgSchUeCb        *ue;
   RgSchCmnDlUe     *cmnUeDl;
   RgSchDlHqProcCb  *hqP;

   node = allocInfo->dedAlloc.schdTxHqPLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)node->node;
      ue = hqP->hqE->ue;
      node = node->next;
      cmnUeDl = RG_SCH_CMN_GET_DL_UE(ue,cell);
                /*cell added as part of CA dev*/

      rgSCHSc1DlNewTxUeFnlz(cell, allocInfo, ue);
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddUpdDLMark(cell, ue);
      }
#endif
      /* reset the UE allocation Information */
      cmLListInit(&((RgSchSc1DlUe *)(cmnUeDl->schSpfc))->schdSvcs);
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }

   /* Traverse the nonSchdTxUeLst and reset the UE allocation Info */
   node = allocInfo->dedAlloc.nonSchdTxHqPLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)node->node;
      ue = hqP->hqE->ue;
      node = node->next;
      cmnUeDl = RG_SCH_CMN_GET_DL_UE(ue, cell);

      /* Release HqProc */
      rgSCHSc1RlsHqProc(hqP);
      /* reset the UE allocation Information */
      cmLListInit(&((RgSchSc1DlUe *)(cmnUeDl->schSpfc))->schdSvcs);
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }
   return;
}

/* 3.1 Added new function to handle TX+RETX alloc fnlz'n */

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        RETX+New TX allocations. The NewTx TB allocation
 *        is considered for distribution among LCs.
 *
 * @details
 *
 *     Function: rgSCHSc1DlRetxNewTxAllocFnlz 
 *     Purpose : 1. Reached here due to 1 RETX TB allocation for a 
 *                  SM UE, which is capable to accomodate a newTX
 *                  in the other TB.
 *               2. Distribute NewTX TB allocation among the
 *                  SVCs present in lcsWithData list of UE.
 *
 *
 *     Invoked by: Common Scheduler 
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *cellAllocInfo
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlRetxNewTxAllocFnlz(RgSchCellCb *cell,RgSchCmnDlRbAllocInfo *cellAllocInfo)
{
   CmLList          *node;
   RgSchUeCb        *ue;
   RgSchSc1DlUe     *sc1DlUe;
   RgSchDlHqProcCb  *hqP;
   RgSchDlHqTbCb    *newTxTbInfo;
   uint32_t         effAlloc;
   uint32_t         remTbBytes;
   RgSchDlRbAlloc   *ueAllocInfo; 
   RgSchDlRbAlloc   *dlAllocCb;

   
   node = cellAllocInfo->dedAlloc.schdTxRetxHqPLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)node->node;
      ue = hqP->hqE->ue;
      node = node->next;
      sc1DlUe = RG_GET_SC1_UE_DL(ue, cell);
      ueAllocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue, cell);
      dlAllocCb = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue, cell);
      /* Index 0 of ueAllocInfo->tbInfo will always hold the 
       * RETX TB and index 1 will hold the NewTX TB in case of
       * RETX+TX allocation. */
      newTxTbInfo = ueAllocInfo->tbInfo[1].tbCb;
      effAlloc = remTbBytes  =  ueAllocInfo->tbInfo[1].bytesAlloc;
      rgSCHSc1DlSprTxTbDstn(cell, ue, newTxTbInfo,\
                     &remTbBytes, &(sc1DlUe->lcsWithData.first));
      /* Trying to reduce mcs of TX TB to increase reception quality at UE.
       * In case of RETX+TX allocation, TX TB allocation was irrespective
       * of actual requirement by UE, hence in case if consumption is 
       * less than allocation, consider reducing the iMcs of this TX TB. */
      rgSCHCmnRdcImcsTxTb(dlAllocCb, 1, effAlloc - remTbBytes);
      /* 3.1 MIMO Remove/Retain from/in cell RETX List */
      rgSCHSc1DlProcRmvFrmCellRetx(cell, hqP);
      /* Fill PDCCH and assign it to HqP */
      rgSCHCmnFillHqPPdcch(cell, dlAllocCb, hqP);
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddUpdDLMark(cell, ue);
      }
#endif
      /* reset the UE allocation Information */
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }

   /* Traverse the nonSchdTxRetxHqPLst and reset the UE allocation Info */
   node = cellAllocInfo->dedAlloc.nonSchdTxRetxHqPLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)node->node;
      ue = hqP->hqE->ue;
      node = node->next;
      /* reset the UE allocation Information */
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }
}


/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested. 
 *
 * @details
 *
 *     Function: rgSCHSc1DlAllocFnlz
 *     Purpose:  This function Processes the Final Allocations
 *               made by the RB Allocator against the requested. 
 *               1. Loop through scheduled TX and RETX lists.
 *                  Fill in the corresponding PDCCH and HqProcs.
 *                  In case of TX If actual Alloc < requested, then perform
 *                     an appropriate distribution among the schdSvcs.
 *                     If TA is satisfied, then remove UE from TA Lst.
 *               2. Loop through UnScheduled TX and RETX Lists.
 *                  Release grabbed HqProcs.
 *                  Put back SVCs from schdSvcsLst to their corresponding Qs.
 *               
 *     
 *     Invoked by: Common Scheduler 
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
Void rgSCHSc1DlAllocFnlz(RgSchCellCb *cell,RgSchCmnDlRbAllocInfo *allocInfo)
{

   rgSCHSc1DlRetxAllocFnlz(cell, allocInfo);

   rgSCHSc1DlNewTxAllocFnlz(cell, allocInfo);

   /*3.1 MIMO new Function added to handle TX+RETX 
    * harq process scheduling finalization */
   rgSCHSc1DlRetxNewTxAllocFnlz(cell, allocInfo);
   return;
}



/**
 * @brief This function Updates the DL CQI for the UE. 
 *
 * @details
 *
 *     Function: rgSCHSc1DlCqiInd 
 *     Purpose:  Updates the DL CQI for the UE
 *     
 *     Invoked by: Common Scheduler. SC1 does nothing.
 *     
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  TfuDlCqiRpt        *dlCqiRpt
 *  @return  Void
 *
 **/
Void rgSCHSc1DlCqiInd(RgSchCellCb *cell,RgSchUeCb *ue,Bool isPucchInfo,Void *dlCqi)
{
   return;
}


/**
 * @brief This function adds a service to UE's list of lcsWithData. 
 *
 * @details
 *
 *     Function: rgSCHSc1DlAdd2UeLcsWithData
 *     Purpose:  1. This is to maintain a snapshot view of the 
 *               DL SVCs distributions among the cellwide priority
 *               queues. 
 *               2. This snapshot view is maintained in the order 
 *               of priority of the SVCs with in UE.
 *               3. Addition of SVC to a cellwide priority Queue
 *                  triggers this function.
 *     
 *     Invoked by: Functions of DL SC1 which add SVC or UE(for ambr svc)
 *                 to cellwide priority Queues.
 *     
 *  @param[in]  RgSchUeCb*    ue   
 *  @param[in]  RgSchDlLcCb*  svc 
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlAdd2UeLcsWithData(RgSchCellCb *cell,RgSchUeCb *ue,RgSchDlLcCb *svc)
{
   CmLListCp            *lst;
   CmLList              *node;
   RgSchCmnDlSvc        *cmnDlSvc = RG_SCH_CMN_GET_DL_SVC(svc);
   RgSchSc1DlSvc        *sc1DlSvc = RG_GET_SC1_SVC_DL(ue,svc,cell);
   RgSchSc1DlUe         *sc1DlUe  = RG_GET_SC1_UE_DL(ue, cell);
   RgSchCmnDlSvc        *cmnDlLstSvc;

   
   lst  = &(sc1DlUe->lcsWithData);
   node = lst->first;
   while(node)
   {
      cmnDlLstSvc = RG_SCH_CMN_GET_DL_SVC(((RgSchDlLcCb *)(node->node)));
      if (cmnDlSvc->prio <= cmnDlLstSvc->prio)
      {
         break;
      }
      node = node->next;
   }
   if (node == NULLP)
   {
      cmLListAdd2Tail(lst, &sc1DlSvc->lcWithDataLnk);
      sc1DlSvc->lcWithDataLnk.node = (PTR)svc;
   }
   else
   {
      lst->crnt = node;
      cmLListInsCrnt(lst, &sc1DlSvc->lcWithDataLnk);
      sc1DlSvc->lcWithDataLnk.node = (PTR)svc;
   }
   return;
}


/**
 * @brief This function adds a service to UE's list of lcsWithData. 
 *
 * @details
 *
 *     Function: rgSCHSc1DlRmFrmUeLcsWithData
 *     Purpose:  1. This is to maintain a snapshot view of the 
 *               DL SVCs distributions among the cellwide priority
 *               queues. 
 *               2. This snapshot view is maintained in the order 
 *               of priority of the SVCs with in UE.
 *               3. Addition of SVC to a cellwide priority Queue
 *                  triggers this function.
 *     
 *     Invoked by: Functions of DL SC1 which add SVC or UE(for ambr svc)
 *                 to cellwide priority Queues.
 *     
 *  @param[in]  RgSchUeCb*    ue   
 *  @param[in]  RgSchDlLcCb*  svc 
 *  @return  Void
 *
 **/
static Void rgSCHSc1DlRmFrmUeLcsWithData(RgSchCellCb *cell,RgSchUeCb *ue,RgSchDlLcCb  *svc)
{
   RgSchSc1DlSvc        *sc1DlSvc = RG_GET_SC1_SVC_DL(ue,svc,cell);
   RgSchSc1DlUe         *sc1DlUe  = RG_GET_SC1_UE_DL(ue, cell);

   
   cmLListDelFrm(&(sc1DlUe->lcsWithData), &sc1DlSvc->lcWithDataLnk);
   sc1DlSvc->lcWithDataLnk.node = NULLP;
   return;
}
/***************** SC1 DL SCHEDULER FUNCTION DEFNs END HERE ****************/

/***************************************************************************/

/***************** SC1 UL SCHEDULER FUNCTION DEFNs START HERE **************/

/*--------------------------*
 * UL specific functions START
 *---------------------------*/

/**
 * @brief UE Lc Config for RR 
 *
 * @details
 *
 *     Function : rgSCHSc1UlLchCfg
 *
 *     Processing Steps:  Dummy function
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgrLchCfg        *cfg 
 *  @param[in]  RgSchErrInfo     *err 
 *  @return  S16
 *      -# ROK
 **/
S16 rgSCHSc1UlLchCfg 
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrLchCfg        *cfg,
RgSchErrInfo     *err
)
{
   return ROK;
}
/**
 * @brief UE Lc Reconfig for RR 
 *
 * @details
 *
 *     Function : rgSCHSc1UlLchRecfg
 *
 *     Processing Steps:  Dummy function
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgrLchRecfg      *recfg 
 *  @param[in]  RgSchErrInfo     *err 
 *  @return  S16
 *      -# ROK
 **/
S16 rgSCHSc1UlLchRecfg 
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrLchRecfg      *recfg,
RgSchErrInfo     *err
)
{
   return ROK;
}
/**
 * @brief LC deletion for PFS
 *
 * @details
 *
 *     Function : rgSCHSc1UlLchDel
 *
 *     Processing Steps: Dummy function
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  CmLteLcId        lcId
 *  @return  S16
 *      -# ROK
 **/
S16 rgSCHSc1UlLchDel(RgSchCellCb *cell,RgSchUeCb  *ue,CmLteLcId lcId,uint8_t lcgId)
{
  return  (ROK);
}

/**
 * @brief This function initializes all the data for the scheduler
 *
 * @details
 *
 *     Function: rgSCHSc1UlInit
 *     Purpose:  This function initializes the following information
 *               1. Efficiency table
 *               2. CQI to table index - It is one row for upto 3 RBs
 *                  and another row for greater than 3 RBs
 *     
 *               currently extended prefix is compiled out.
 *     Invoked by: MAC intialization code..may be ActvInit
 *     
 *  @return  Void
 *
 **/
Void rgSCHSc1UlInit(RgUlSchdApis *rgSchUlApis)
{
   /* Init the function pointers */
   rgSchUlApis->rgSCHRgrUlUeCfg     = rgSCHSc1RgrUlUeCfg;
   rgSchUlApis->rgSCHRgrUlUeRecfg   = rgSCHSc1RgrUlUeRecfg;
   rgSchUlApis->rgSCHFreeUlUe       = rgSCHSc1UlUeDel;
   rgSchUlApis->rgSCHRgrUlCellCfg   = rgSCHSc1RgrUlCellCfg;
   rgSchUlApis->rgSCHRgrUlCellRecfg = rgSCHSc1RgrUlCellRecfg; 
   rgSchUlApis->rgSCHFreeUlCell     = rgSCHSc1UlCellDel;
   rgSchUlApis->rgSCHRgrUlLcCfg     = rgSCHSc1UlLchCfg;
   rgSchUlApis->rgSCHRgrUlLcRecfg   = rgSCHSc1UlLchRecfg;
   rgSchUlApis->rgSCHRgrUlLcgCfg    = rgSCHSc1RgrLcgCfg;
   rgSchUlApis->rgSCHRgrUlLcgRecfg  = rgSCHSc1RgrLcgRecfg;
   rgSchUlApis->rgSCHFreeUlLcg      = rgSCHSc1LcgDel;
   rgSchUlApis->rgSCHRgrUlLchDel    = rgSCHSc1UlLchDel;
   rgSchUlApis->rgSCHUlSched        = rgSCHSc1UlSched;
   rgSchUlApis->rgSCHUpdBsrShort    = rgSCHSc1UpdBsrShort;
   rgSchUlApis->rgSCHUpdBsrTrunc    = rgSCHSc1UpdBsrTrunc;
   rgSchUlApis->rgSCHUpdBsrLong     = rgSCHSc1UpdBsrLong;
   rgSchUlApis->rgSCHContResUlGrant = rgSCHSc1ContResUlGrant;
   rgSchUlApis->rgSCHSrRcvd         = rgSCHSc1SrRcvd;
   rgSchUlApis->rgSCHUlCqiInd       = rgSCHSc1UlCqiInd;
   rgSchUlApis->rgSCHUlUeRefresh    = rgSCHSc1UlUeRefresh;
   rgSchUlApis->rgSCHUlAllocFnlz    = rgSCHSc1UlAllocFnlz;
   rgSchUlApis->rgSCHUlInactvtUes   = rgSCHSc1UlHndlInActUes;
   rgSchUlApis->rgSCHUlActvtUe      = rgSCHSc1UlActvtUe;
   rgSchUlApis->rgSCHUlUeReset      = rgSCHSc1UlUeReset;
   rgSchUlApis->rgSCHRgrUlLcgUpd    = rgSCHSc1UlLcgUpd;
   return;
}

/**
 * @brief UE initialisation for scheduler
 *
 * @details
 *
 *     Function : rgSCHSc1RgrUlUeCfg
 *     
 *     This functions intialises UE specific scheduler
 *     information
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[int] RgrUeCfg     *ueCfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHSc1RgrUlUeCfg(RgSchCellCb  *cell,RgSchUeCb *ue,RgrUeCfg  *ueCfg,RgSchErrInfo *err)
{
  
   RgSchCmnUe *ueSchCmn = RG_SCH_CMN_GET_UE(ue, cell);

   if(rgSCHUtlAllocSBuf(cell->instIdx, 
      (Data**)&(ueSchCmn->ul.schSpfc), (sizeof(RgSchSc1UlUe))) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation FAILED CRNTI:%d",ue->ueId);
      err->errCause = RGSCHERR_SCH_SC1_UL_CFG;
      return RFAILED;
   }

   return ROK;
}  /* rgSCHSc1RgrUlUeCfg */

/**
 * @brief UE reconfiguration for scheduler
 *
 * @details
 *
 *     Function : rgSCHSc1RgrUlUeRecfg
 *     
 *     This functions updates UE specific scheduler
 *     information upon UE reconfiguration
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[int] RgrUeRecfg   *ueRecfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHSc1RgrUlUeRecfg(RgSchCellCb *cell,RgSchUeCb  *ue,RgrUeRecfg *ueRecfg,RgSchErrInfo *err)
{
   return ROK;
}  /* rgSCHSc1RgrUeRecfg */

/**
 * @brief UE deletion for scheduler
 *
 * @details
 *
 *     Function : rgSCHSc1UlUeDel
 *     
 *     This functions deletes all scheduler information
 *     pertaining to a UE
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
Void rgSCHSc1UlUeDel(RgSchCellCb *cell,RgSchUeCb *ue)
{
   RgSchSc1UlCell *cellUl = RG_GET_SC1_CELL_UL(cell);
   RgSchSc1UlUe   *ueUl   = RG_GET_SC1_UE_UL(ue, cell);
   

   if (ueUl == NULLP)
   {
      return;
   }
   if(ueUl->txLnk.node)
   {
      cmLListDelFrm(&(cellUl->ueTxLst[ueUl->qId]), &(ueUl->txLnk));
      ueUl->txLnk.node = NULLP;
   }
   if(ueUl->contResLnk.node)
   {
      cmLListDelFrm(&(cellUl->contResLst), &(ueUl->contResLnk));
      ueUl->contResLnk.node = NULLP;
   }
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx,
      (Data**)(&(ueUl)), (sizeof(RgSchSc1UlUe)));

   return;
}  /* rgSCHSc1UlUeDel */

/**
 * @brief UE Reset for scheduler
 *
 * @details
 *
 *     Function : rgSCHSc1UlUeReset
 *     
 *     Remove this UE from all Scheduling Priority Qs
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
Void rgSCHSc1UlUeReset(RgSchCellCb  *cell,RgSchUeCb    *ue)
{
   RgSchSc1UlCell *cellUl = RG_GET_SC1_CELL_UL(cell);
   RgSchSc1UlUe   *ueUl   = RG_GET_SC1_UE_UL(ue, cell);
   

   ueUl->srRcvd = FALSE;

   if(ueUl->txLnk.node)
   {
      cmLListDelFrm(&(cellUl->ueTxLst[ueUl->qId]), &(ueUl->txLnk));
      ueUl->txLnk.node = NULLP;
   }
   if(ueUl->contResLnk.node)
   {
      cmLListDelFrm(&(cellUl->contResLst), &(ueUl->contResLnk));
      ueUl->contResLnk.node = NULLP;
   }
   return;
}  /* rgSCHSc1UlUeReset */


/**
 * @brief Scheduler processing on cell configuration
 *
 * @details
 *
 *     Function : rgSCHSc1RgrUlCellCfg
 *     
 *     This function does requisite initialisation 
 *     and setup for scheduler1 when a cell is
 *     configured
 *
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgrCellCfg    *cellCfg
 *  @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHSc1RgrUlCellCfg(RgSchCellCb  *cell,RgrCellCfg  *cellCfg,RgSchErrInfo *err)
{
   RgSchSc1UlCell     *cellUl; 

   if((rgSCHUtlAllocSBuf(cell->instIdx, 
      (Data**)&(((RgSchCmnCell*)((cell)->sc.sch))->ul.schSpfc), \
      (sizeof(RgSchSc1UlCell))) != ROK))
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation FAILED");
      err->errCause = RGSCHERR_SCH_SC1_UL_CFG;
      return RFAILED;
   }
   cellUl = RG_GET_SC1_CELL_UL(cell);
   cmLListInit(&cellUl->contResLst);
   cmLListInit(&cellUl->ueTxLst[0]);
   cmLListInit(&cellUl->ueTxLst[1]);

   return ROK;
}  /* rgSCHSc1RgrUlCellCfg */


/**
 * @brief This function handles the reconfiguration of cell 
 *
 * @details
 *
 *     Function: rgSCHSc1RgrUlCellRecfg
 *     Purpose:  Update the reconfiguration parameters.
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @return  Void
 *
 **/
S16 rgSCHSc1RgrUlCellRecfg(RgSchCellCb *cell,RgrCellRecfg  *recfg,RgSchErrInfo  *err)
{
   return ROK;
}

/**
 * @brief Scheduler processing for cell delete
 *
 * @details
 *
 *     Function : rgSCHSc1UlCellDel
 *     
 *     This functions de-initialises and frees memory
 *     taken up by scheduler1 for the entire cell.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @return  Void
 **/
Void rgSCHSc1UlCellDel(RgSchCellCb  *cell)
{
   RgSchSc1UlCell *cellUl = RG_GET_SC1_CELL_UL(cell);


   if (cellUl == NULLP)
   {
      return;
   }
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx,
      (Data**)(&(cellUl)), (sizeof(RgSchSc1UlCell)));

   return;
}  /* rgSCHSc1UlCellDel */

/**
 * @brief Scheduler invocation on logical channel Group addition
 *
 * @details
 *
 *     Function : rgSCHSc1RgrLcgCfg
 *     
 *     This functions does required processing when a new
 *     (dedicated) logical channel is added. Assumes lcg
 *     pointer in ulLc is set.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgSchLcgCb   *lcg
 *  @param[int] RgrLcgCfg    *lcgCfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHSc1RgrLcgCfg(RgSchCellCb *cell,RgSchUeCb *ue,RgSchLcgCb *lcg,RgrLcgCfg *lcgCfg,RgSchErrInfo *err)
{
   return ROK;
}  /* rgSCHSc1RgrLcgCfg */

/**
 * @brief Scheduler invocation on logical channel addition
 *
 * @details
 *
 *     Function : rgSCHSc1RgrLcgRecfg
 *     
 *     This functions does required processing when an existing
 *     (dedicated) logical channel is reconfigured. Assumes lcg
 *     pointer in ulLc is set to the old value.
 *     Independent of whether new LCG is meant to be configured,
 *     the new LCG scheduler info is accessed and possibly modified.
 *
 *  @param[in]  RgSchCellCb  *cell,
 *  @param[in]  RgSchUeCb    *ue,
 *  @param[in]  RgSchLcgCb   *lcg,
 *  @param[in]  RgrLcgRecfg  *reCfg,
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHSc1RgrLcgRecfg(RgSchCellCb *cell,RgSchUeCb *ue,RgSchLcgCb *lcg,RgrLcgRecfg *reCfg,RgSchErrInfo *err)
{
   return ROK;
}  /* rgSCHSc1RgrLcgRecfg */

/***********************************************************
 *
 *     Func : rgSCHSc1LcgDel
 *        
 *     Desc : Scheduler handling for a (dedicated)
 *             uplink lcg being deleted
 *
 *     Ret  : 
 *
 *     Notes: 
 *
 *     File : 
 *
 **********************************************************/
Void rgSCHSc1LcgDel(RgSchCellCb *cell,RgSchUeCb *ue,RgSchLcgCb *lcg)
{
   rgSCHSc1UlPosnUeInQ(cell, ue);
   return;
}

/**
 * @brief Perform alloction for this UE 
 *
 * @details
 *
 *     Function : rgSCHSc1UlSchdUe
 *
 *     Processing Steps: cater to as much as UE needs, with 
 *     a limitation on maxBits per scheduling instance(per TTI)
 *     per UE. Return failure, if UE is not considered 
 *     for scheduling (case, where it is already selected for a
 *     retransmission).
 *                   
 *
 *  @param[in]  RgSchUeCb        *ue
 *  @return  Void
 **/
static Void rgSCHSc1UlSchdUe(RgSchUeCb *ue,RgSchCellCb *cell)
{
   RgSchCmnUlUe      *cmnUlUe = RG_SCH_CMN_GET_UL_UE(ue, cell);
                                /*cell added as part of CA dev*/
   RgSchSc1UlUe      *ulUe    = RG_GET_SC1_UE_UL(ue, cell);


   if(ulUe->srRcvd == TRUE)
   {
      cmnUlUe->alloc.reqBytes = RGSCH_MAX(RG_SCH_CMN_UL_SR_BYTES, \
                                ue->ul.effBsr);
      return;
   }

   cmnUlUe->alloc.reqBytes = ue->ul.effBsr;

   return;
}

/**
 * @brief Scheduler invocation
 *
 * @details
 *
 *     Function: rgSCHSc1UlSchdForDataTrans
 *     Purpose:  Uplink Scheduling for UE data Transmission.
 *
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[out] RgSchCmnUlRbAllocInfo *allocInfo 
 *  @param[in] uint8_t                     remUe 
 *  @return Void 
 **/
static Void rgSCHSc1UlSchdForDataTrans(RgSchCellCb  *cell,RgSchCmnUlRbAllocInfo *allocInfo,uint8_t remUe)
{
   RgSchSc1UlCell    *sc1UlCell  = RG_GET_SC1_CELL_UL(cell);

   if (remUe == 0)
   {
      return;
   }
   /* Allocate UEs with LCG0 data pending */
   rgSCHSc1UlSchdUeTxLst(cell, &sc1UlCell->ueTxLst[0], allocInfo, &remUe);

   if (remUe == 0)
   {
      return;
   }
   /* Allocate UEs with other LCGs data pending */
   rgSCHSc1UlSchdUeTxLst(cell, &sc1UlCell->ueTxLst[1], allocInfo, &remUe);

   return;
}

/**
 * @brief Scheduler invocation
 *
 * @details
 *
 *     Function: rgSCHSc1UlSchdUeTxLst
 *     Purpose:  Uplink Scheduling for UE data Transmission.
 *
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  CmLListCp             *ueTxLst
 *  @param[out] RgSchCmnUlRbAllocInfo *allocInfo 
 *  @param[in] uint8_t                     *remUe 
 *  @return Void 
 **/
static Void rgSCHSc1UlSchdUeTxLst(RgSchCellCb *cell,CmLListCp *ueTxLst,RgSchCmnUlRbAllocInfo *allocInfo,uint8_t *remUe)
{
   RgSchUeCb *ue;
   CmLList   *node;
#ifdef LTEMAC_HDFDD
   Bool      ulAllowed = FALSE;
#endif


   node = ueTxLst->first;
   while ((node) && (*remUe))
   {
      ue = (RgSchUeCb *)node->node;
      node = node->next;
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddChkUlAllow (cell, ue, &ulAllowed);
         if (ulAllowed == FALSE)
         {
            continue;
         }
      }
#endif

      if (RG_SCH_CMN_IS_UL_UE_RETX(ue, cell))
      {
         /* UE already scheduled in this subframe (for retx) 
          * OR is inactive for UL Transmission.*/
         continue;
      }
      /* Added support for SPS*/
#ifdef LTEMAC_SPS
         else if (RG_SCH_CMN_IS_SPS_SCHD(ue, cell))
         {
            /*-- Already Scheduled by SPS --*/
           continue;
         }
#endif

      rgSCHSc1UlSchdUe(ue,cell);/*cell added as part of CA dev*/

      rgSCHCmnUlAdd2UeLst(cell, allocInfo, ue);

      --(*remUe);
   }

   return;
}

/**
 * @brief Scheduler invocation
 *
 * @details
 *
 *     Function: rgSCHSc1UlSchdForContRes
 *     Purpose:  Uplink Scheduling for Contention Resolution.
 *
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[out] RgSchCmnUlRbAllocInfo *allocInfo 
 *  @param[out] uint8_t                    *remUe 
 *  @return Void 
 **/
static Void rgSCHSc1UlSchdForContRes(RgSchCellCb  *cell,RgSchCmnUlRbAllocInfo *allocInfo,uint8_t *remUe)
{
   RgSchSc1UlCell    *sc1UlCell  = RG_GET_SC1_CELL_UL(cell);
   RgSchUeCb         *ue;
   CmLList           *node;
   RgSchCmnUlUe      *cmnUlUe;
#ifdef LTEMAC_HDFDD
   Bool                 ulAllowed = FALSE;
#endif


   node = sc1UlCell->contResLst.first;
   while ((node) && (*remUe))
   {
      ue = (RgSchUeCb *)node->node;
      cmnUlUe = RG_SCH_CMN_GET_UL_UE(ue, cell);
                /*cell added as part of CA dev*/
      node = node->next;
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddChkUlAllow (cell, ue,  &ulAllowed);
         if (ulAllowed == FALSE)
         {
            continue;
         }
      }
#endif
      if (RG_SCH_CMN_IS_UL_UE_RETX(ue, cell))
      {
         /* UE already scheduled in this subframe (for retx) 
          * OR is inactive for UL Transmission.*/
         continue;
      }
      cmnUlUe->alloc.reqBytes = RG_SCH_CMN_MAX_UL_CONTRES_GRNT;
      rgSCHCmnUlAdd2CntResLst(allocInfo, ue);
      --(*remUe);
      /* Node removal deferred to ULAllocFinalization */
   }

   return;
}

/**
 * @brief Scheduler invocation
 *
 * @details
 *
 *     Function: rgSCHSc1UlNewTx
 *     Purpose:  Uplink Scheduling for New Transmissions.
 *
 *     
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[out] RgSchCmnUlRbAllocInfo *allocInfo 
 *  @return Void 
 **/
static Void rgSCHSc1UlNewTx(RgSchCellCb *cell,RgSchCmnUlRbAllocInfo *allocInfo)
{
   RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   uint8_t               remUe = cellUl->maxUeNewTxPerTti;


   rgSCHSc1UlSchdForContRes(cell, allocInfo, &remUe);
   rgSCHSc1UlSchdForDataTrans(cell, allocInfo, remUe);
   return;
}

/**
 * @brief Scheduler invocation
 *
 * @details
 *
 *     Function: rgSCHSc1UlSched
 *     Purpose:  This function implements an UL scheduler for LTE. This is
 *               made available as a function pointer to be called
 *               at the time of TTI processing by the MAC.
 *     
 *     Invoked by: Common Scheduler (TTI processing)
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[out] RgSchCmnUlRbAllocInfo *allocInfo 
 *  @return Void 
 **/
Void rgSCHSc1UlSched(RgSchCellCb *cell,RgSchCmnUlRbAllocInfo *allocInfo)
{
   rgSCHSc1UlNewTx(cell, allocInfo);
   return;
}

/**
 * @brief UEs Buffer Status Has changed so reposition it. 
 *
 * @details
 *
 *     Function : rgSCHSc1UlInsUeInQ 
 *     
 *     In UE in the list in Descending order of effBsr.
 *     
 *
 *  @param[in]  CmLListCp    *lst
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void 
 **/
static Void rgSCHSc1UlInsUeInQ(CmLListCp *lst,RgSchUeCb *ue,RgSchCellCb *cell)
{
                              /*cell added as part of CA dev*/
   RgSchSc1UlUe    *ueUl    = RG_GET_SC1_UE_UL(ue, cell);
   RgSchUeCb       *lUe;
   CmLList         *node;


   node = lst->first;
   while(node)
   {
      lUe = (RgSchUeCb *)(node->node);
      if (lUe->ul.effBsr <= ue->ul.effBsr)
      {
         break;
      }
      node = node->next;
   }
   if (node == NULLP)
   {
      /* We have come to the end of the queue, so Append */
      cmLListAdd2Tail(lst, &ueUl->txLnk);
      ueUl->txLnk.node = (PTR)ue;
   }
   else
   {
      lst->crnt = node;
      cmLListInsCrnt(lst, &ueUl->txLnk);
      ueUl->txLnk.node = (PTR)ue;
   }

   return;
}
/**
 * @brief UEs Buffer Status Has changed so reposition it. 
 *
 * @details
 *
 *     Function : rgSCHSc1UlPosnUeInQ
 *     
 *     -Ues bs value for its LCG has changed, due to either 
 *     allocation or BSR report OR the effUeBR, i.e the byteRate
 *     has changed due to some allocation, so add/reposition/remove
 *     it from Qs based on this new bs and/or effUeBR value.
 *     -If UE has non-zero lcg0 bs value, but the byteRate is 
 *     consumed totally, UE is still schedulable for this control data. 
 *     -If UE's LCG0 has pending bs then position this UE in 
 *     ueTxLst[0].
 *     -If Ue has pending BSR to be satisfied, but lcg0's BS
 *     is 0, then position it in ueTxLst[1].
 *     -In any of these 2 Qs, insertion is such that UEs are
 *     positioned in Descending order of their Pending BS.
 *     
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void 
 **/
static Void rgSCHSc1UlPosnUeInQ(RgSchCellCb *cell,RgSchUeCb *ue)
{
   RgSchSc1UlUe    *ueUl   = RG_GET_SC1_UE_UL(ue, cell);
                              /*cell added as part of CA dev*/
   RgSchSc1UlCell  *cellUl = RG_GET_SC1_CELL_UL(cell);
   RgSchCmnLcg     *cmnLcg0 = RG_SCH_CMN_GET_UL_LCG(&ue->ul.lcgArr[0]);
   CmLListCp       *lst;


   if (!RG_SCH_CMN_UL_IS_UE_ACTIVE(ue))
   {
      return;
   }
                     
   /* Remove the UE from its existing position */
   if (ueUl->txLnk.node)
   {
      cmLListDelFrm(&(cellUl->ueTxLst[ueUl->qId]), &(ueUl->txLnk));
      ueUl->txLnk.node = (PTR)NULLP;
   }
   /* If UE has still bs left for scheduling 
    * then reposition it */
   if ((ue->ul.effBsr > 0) || (ueUl->srRcvd == TRUE))
   {
      /* Select the Queue where UE would be Placed */
      if (cmnLcg0->bs > 0)
      {
         lst = &cellUl->ueTxLst[0];
         ueUl->qId = 0;
      }
      else
      {
         lst = &cellUl->ueTxLst[1];
         ueUl->qId = 1;
      }
      /* Insert the UE in the Q */
      rgSCHSc1UlInsUeInQ(lst, ue, cell);/*cell added as part of CA dev*/ 
   }
#ifdef RGR_V1
   else if(ue->ul.totalBsr != 0)
   {
      if (ue->bsrTmr.tmrEvnt != TMR_NONE)
      {
         rgSCHTmrStopTmr(cell, ue->bsrTmr.tmrEvnt, ue); 
      }
      if (ue->ul.bsrTmrCfg.isPrdBsrTmrPres)
      {
         rgSCHTmrStartTmr(cell, ue, RG_SCH_TMR_BSR, 
               ue->ul.bsrTmrCfg.prdBsrTmr);
      }
   }
#endif

   return;
}

/**
 * @brief Short BSR update
 *
 * @details
 *
 *     Function : rgSCHSc1UpdBsrShort
 *     
 *     This functions does requisite updates to handle short BSR reporting
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgSchLcgCb   *lcg
 *  @param[in]  uint8_t           bsr
 *  @return  Void
 **/
Void rgSCHSc1UpdBsrShort(RgSchCellCb  *cell,RgSchUeCb    *ue,RgSchLcgCb   *lcg,uint8_t  bsr)
{
   rgSCHSc1UlPosnUeInQ(cell, ue);
   return;
}  /* rgSCHSc1UpdBsrShort */

/**
 * @brief Truncated BSR update
 *
 * @details
 *
 *     Function : rgSCHSc1UpdBsrTrunc
 *     
 *     This functions does required updates to handle truncated BSR report
 *     
 *           
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgSchLcgCb   *lcg
 *  @param[in]  uint8_t           bsr
 *  @return  Void 
 **/
Void rgSCHSc1UpdBsrTrunc(RgSchCellCb  *cell,RgSchUeCb    *ue,RgSchLcgCb   *lcg,uint8_t bsr)
{
   rgSCHSc1UlPosnUeInQ(cell, ue);
   return;
}  /* rgSCHSc1UpdBsrTrunc */

/**
 * @brief Long BSR update
 *
 * @details
 *
 *     Function : rgSCHSc1UpdBsrLong
 *
 *     - Update UE's position within/across uplink scheduling queues
 *
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  uint8_t bsArr[]
 *  @return  Void 
 **/
Void rgSCHSc1UpdBsrLong(RgSchCellCb  *cell,RgSchUeCb *ue,uint8_t *bsArr)
{
   rgSCHSc1UlPosnUeInQ(cell, ue);
   return;
}  /* rgSCHSc1UpdBsrLong */

/**
 * @brief UL grant for contention resolution
 *
 * @details
 *
 *     Function : rgSCHSc1ContResUlGrant
 *     
 *     Add UE to another queue specifically for CRNTI based contention
 *     resolution
 *     
 *           
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return Void 
 **/
Void rgSCHSc1ContResUlGrant(RgSchCellCb  *cell,RgSchUeCb *ue)
{
   RgSchSc1UlUe    *ueUl   = RG_GET_SC1_UE_UL(ue, cell);
   RgSchSc1UlCell  *cellUl = RG_GET_SC1_CELL_UL(cell);


   if (ueUl->contResLnk.node)
   {
      return;
   }

   /* Remove the UE from other Qs */
   if(ueUl->txLnk.node)
   {
      cmLListDelFrm(&(cellUl->ueTxLst[ueUl->qId]), &(ueUl->txLnk));
      ueUl->txLnk.node = NULLP;
   }

   cmLListAdd2Tail(&cellUl->contResLst, &ueUl->contResLnk);
   ueUl->contResLnk.node = (PTR)ue;
   return;
}  /* rgSCHSc1ContResUlGrant */

/**
 * @brief SR reception handling
 *
 * @details
 *
 *     Function : rgSCHSc1SrRcvd
 *     Shift the UE with SrInd in to the lcgO queue.
 *
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
Void rgSCHSc1SrRcvd(RgSchCellCb *cell,RgSchUeCb *ue)
{
   RgSchSc1UlUe    *ulUe    = RG_GET_SC1_UE_UL(ue, cell);
   RgSchSc1UlCell  *ulCell  = RG_GET_SC1_CELL_UL(cell);


   ulUe->srRcvd = TRUE;

   if (ulUe->txLnk.node != NULLP)
   {
      if (ulUe->qId == 0)
      {
         /* Already present in lcg0 Q */
         return;
      }
      cmLListDelFrm(&(ulCell->ueTxLst[ulUe->qId]), &(ulUe->txLnk));
   }
   /* Adding the UE to the LCG0 list for SR IND */
   cmLListAdd2Tail(&ulCell->ueTxLst[0], &ulUe->txLnk);
   ulUe->txLnk.node = (PTR)ue;
   ulUe->qId = 0;

   return;
}  /* rgSCHSc1SrRcvd */

/**
 * @brief Indication of UL CQI
 *
 * @details
 *
 *     Function : rgSCHSc1UlCqiInd 
 *
 *     - Common Scheduler. SC1 does nothing. 
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]  TfuUlCqiRpt         *ulCqiInfo
 *  @return  Void
 **/
Void rgSCHSc1UlCqiInd(RgSchCellCb *cell,RgSchUeCb *ue,TfuUlCqiRpt *ulCqiInfo)
{

  /* Stack Crash problem for TRACE5 changes. Added the return below */
  return;

}

/**
 * @brief UL Lcg received data updation
 *
 * @details
 *
 *     Function : rgSCHSc1UlLcgUpd
 *
 *     Processing Steps:Sc1 Does nothing
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  RgInfUeDatInd      *datInd
 *  @return  S16
 **/
S16 rgSCHSc1UlLcgUpd(RgSchCellCb *cell,RgSchUeCb *ue,RgInfUeDatInd  *datInd)
{

   return ROK;  
}


/***********************************************************
 *
 *     Func : rgSCHSc1UlUeRefresh
 *        
 *     Desc : Handle 'refresh' for uplink part of a UE
 *            (ie UE's uplink AMBR and uplink GBR LCGs are
 *            refreshed at this point)
 *
 *     Ret  : 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
Void rgSCHSc1UlUeRefresh(RgSchCellCb *cell,RgSchUeCb  *ue)
{
   rgSCHSc1UlPosnUeInQ(cell, ue);
   return;
}

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        UE data Trans Allocations. 
 *
 * @details
 *
 *     Function: rgSCHSc1UlDatTransAllocFnlz
 *     Purpose:  This function Processes the Final Allocations
 *               made by the RB Allocator against the requested
 *               UE data Trans Allocations . 
 *     
 *     Invoked by: Scheduler 
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
static Void rgSCHSc1UlDatTransAllocFnlz(RgSchCellCb  *cell,RgSchCmnUlRbAllocInfo *allocInfo)
{
   RgSchSc1UlUe      *ueUl;
   RgSchUeCb         *ue;
   CmLList           *node;
   RgSchDrxUeCb      *drxUe    = NULLP;
   CmLListCp         ulInactvLst; /* list of UE's becoming UL-inactive */

   cmLListInit(&ulInactvLst);
   node = allocInfo->schdUeLst.first;
   while(node)
   {
      ue   = (RgSchUeCb *)node->node;
      node = node->next;
      ueUl = RG_GET_SC1_UE_UL(ue, cell);

      if (ue->isDrxEnabled)
      {
         if(ueUl->srRcvd == TRUE)
         {
            drxUe = RG_SCH_DRX_GET_UE(ue);
            drxUe->drxUlInactvMask  |= RG_SCH_DRX_SR_BITMASK;

            if(!RG_SCH_DRX_UL_IS_UE_ACTIVE(drxUe))
            {
               ue->ul.ulInactvMask |= RG_DRX_INACTIVE;
               /* Add to Ul inactive List */
               ue->ulDrxInactvLnk.node  = (PTR)ue;
               cmLListAdd2Tail(&ulInactvLst,&(ue->ulDrxInactvLnk));
            }
            drxUe->srRcvd = FALSE;
         }
      }
      /* Reset no matter */
      ueUl->srRcvd = FALSE;
      /* Reposition UE in Qs */
      rgSCHSc1UlPosnUeInQ(cell, ue);
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddUpdULMark (cell,ue);
      }
#endif
      /* reset the UE UL allocation Information */
      rgSCHCmnUlUeResetTemp(cell, ue);
   }
   rgSCHSc1UlHndlInActUes(cell, &ulInactvLst);
   node = allocInfo->nonSchdUeLst.first;
   while(node)
   {
      ue   = (RgSchUeCb *)node->node;
      node = node->next;
      /* reset the UE UL allocation Information */
      rgSCHCmnUlUeResetTemp(cell, ue);
   }

   return;
}

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        cont res Allocations. 
 *
 * @details
 *
 *     Function: rgSCHSc1UlContResAllocFnlz
 *     Purpose:  This function Processes the Final Allocations
 *               made by the RB Allocator against the requested
 *               cont res Allocations . 
 *     
 *     Invoked by: Scheduler 
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
static Void rgSCHSc1UlContResAllocFnlz(RgSchCellCb *cell,RgSchCmnUlRbAllocInfo *allocInfo)
{
   RgSchSc1UlCell    *sc1UlCell  = RG_GET_SC1_CELL_UL(cell);
   RgSchSc1UlUe      *ueUl;
   RgSchUeCb         *ue;
   CmLList           *node;

   node = allocInfo->schdContResLst.first;
   while(node)
   {
      ue   = (RgSchUeCb *)node->node;
      node = node->next;
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddUpdULMark (cell,ue);
      }
#endif
      ueUl = RG_GET_SC1_UE_UL(ue, cell);

      /* Remove UE from Cont Res Q */ 
      cmLListDelFrm(&sc1UlCell->contResLst,
      &ueUl->contResLnk);
      ueUl->contResLnk.node = NULLP;
      /* reset the UE UL allocation Information */
      rgSCHCmnUlUeResetTemp(cell, ue);
   }

   node = allocInfo->nonSchdContResLst.first;
   while(node)
   {
      ue   = (RgSchUeCb *)node->node;
      node = node->next;
      /* reset the UE UL allocation Information */
      rgSCHCmnUlUeResetTemp(cell, ue);
   }

   return;
}

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested. 
 *
 * @details
 *
 *     Function: rgSCHSc1UlAllocFnlz
 *     Purpose:  This function Processes the Final Allocations
 *               made by the RB Allocator against the requested. 
 *     
 *     Invoked by: Common Scheduler 
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
Void rgSCHSc1UlAllocFnlz(RgSchCellCb *cell,RgSchCmnUlRbAllocInfo *allocInfo)
{

   rgSCHSc1UlContResAllocFnlz(cell, allocInfo);
   rgSCHSc1UlDatTransAllocFnlz(cell, allocInfo);

   return;
}


/**
 * @brief Scheduler invocation
 *
 * @details
 *
 *     Function: rgSCHSc1UlActvtUe 
 *     Purpose:  Put back the UE in to appropriate Qs.
 *     
 *     Invoked by: Common Scheduler 
 *     
 *  @param[in]  RgSchCellCb *cell
 *  @param[in]  RgSchUeCb   *ue
 *  @return  Void
 **/
Void rgSCHSc1UlActvtUe(RgSchCellCb *cell,RgSchUeCb *ue)
{

   rgSCHSc1UlPosnUeInQ(cell, ue);
   return;
}

/**
 * @brief Scheduler invocation
 *
 * @details
 *
 *     Function: rgSCHSc1UlHndlInActUes
 *     Purpose:  The list of inactive UEs present in inactvLst should
 *               be removed from the scheduling Qs.
 *               But store the information pertaining to which Qs,
 *               were they belonging to. This information shall be used 
 *               to put them back in appropriate Qs when their Activation
 *               is initiated.
 *     
 *     Invoked by: Common Scheduler (TTI processing)
 *     
 *  @param[in]  RgSchCellCb *cell
 *  @param[out] CmLListCp   *inactvLst
 *  @return  Void
 **/
Void rgSCHSc1UlHndlInActUes(RgSchCellCb *cell,CmLListCp  *inactvLst)
{
   RgSchUeCb      *ue;
   RgSchSc1UlUe   *ulUe;
   RgSchSc1UlCell *cellUl  = RG_GET_SC1_CELL_UL(cell);
   CmLList        *node = inactvLst->first;

   while (node)
   {
      ue = (RgSchUeCb *)node->node;
      node = node->next;
      ulUe = RG_GET_SC1_UE_UL(ue, cell);
      if(ulUe->txLnk.node)
      {
         cmLListDelFrm(&(cellUl->ueTxLst[ulUe->qId]), &(ulUe->txLnk));
         /* This is required as lcg bs might change during
          * inactivity to activity. So we need to recompute 
          * its position. */
         ulUe->txLnk.node = NULLP;
      }
      /* Do not remove UE from contResLst */
   }
   return;
}
/**
 * @brief Scheduler invocation
 *
 * @details
 *
 *     Function: rgSCHSc1DlProcRmvFrmRetx
 *     Purpose:  To remove the Harq process from the cell and from the UE 
 *                retransmission list
 *     
 *     Invoked by: Common Scheduler (TTI processing)
 *     
 *  @param[in]  RgSchCellCb *cell
 *  @param[in]  RgSchUeCb   *ue;
 *  @param[in] RgSchDlHqProcCb  *hqP
 *  @return  Void
 **/

Void rgSCHSc1DlProcRmvFrmRetx(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlHqProcCb            *hqP
)
{
  
   /* Remove the HqP from retx Queue.
   Release HqP.*/
   rgSCHSc1DlProcRmvFrmCellRetx(cell, hqP);
   rgSCHSc1DlProcRmvFrmUeRetx(cell, ue, hqP);
   return;
}




/**********************************************************************
 
         End of file
**********************************************************************/
