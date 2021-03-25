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
  
     File:     rg_sch_uhm.c
  
**********************************************************************/

/** @file rg_sch_uhm.c
@brief This module handles uplink harq related functionality in MAC.
*/

/* header include files -- defines (.h) */
#include "common_def.h"
#include "rg_env.h"        /* MAC Environment Defines */
#include "tfu.h"           /* TFU Interface defines */
#include "rgr.h"           /* RGR Interface defines */
#include "lrg.h"           /* LRG Interface defines */

#include "rg_sch.h"            /* Scheduler defines */
#include "rg_sch_inf.h"            /* Scheduler defines */
#include "rg_sch_err.h"        /* MAC error defines */

/* header/extern include files (.x) */

#include "tfu.x"           /* TFU Interface defines */
#include "rgr.x"           /* RGR Interface includes */
#include "lrg.x"           /* LRG Interface includes */
#include "rgm.x"
#include "rg_sch_inf.x"            /* Scheduler defines */
#include "rg_sch.x"            /* Scheduler includes */
#include "rg_sch_cmn.h"
#include "rg_sch_cmn.x"

/* local defines */

/* local typedefs */
 
/* local externs */
 
/* forward references */

#ifdef EMTC_ENABLE
   uint32_t gUlRetxPassCntr = 0;
   /*uint32_t gUlRetxFailCntr;
   uint32_t gUlCrcPassCounter;
   uint32_t gUlCrcFailCounter;*/
#endif
uint8_t rgRvIdxTable[] = {0, 3, 1, 2}; /* This gives rvIdx for a given rv */
uint8_t rgRvTable[] = {0, 2, 3 ,1};    /* This gives rv for a given rvIdx */

#ifdef EMTC_ENABLE
Void rgSCHCmnEmtcHdlHarqProcFail
(
RgSchCellCb       *cell,
RgSchUeCb         *ue,
RgSchUlHqProcCb   *hqProc,
CmLteTimingInfo   frm
);
Void rgSCHEmtcInitUlUeHqEnt
(
RgSchCellCb      *cell,
RgrUeCfg         *ueCfg,
RgSchUeCb        *ueCb
);

#endif

/**
 * @brief Handler for HARQ processing on recieving Data indication from PHY.
 *
 * @details
 *
 *     Function: rgSCHUhmProcDatInd
 *     
 *     Invoked by: rgSCHTomTfuDatInd of  TOM
 *
 *     Processing Steps:
 *      - Set rcvdCrcInd variable to TRUE
 *           
 *  @param[in] *cell
 *  @param[in] *ue
 *  @param[in] frm
 *  @return  Void
 **/
#ifndef MAC_SCH_STATS
Void rgSCHUhmProcDatInd(RgSchCellCb *cell,RgSchUeCb *ue,CmLteTimingInfo frm)
#else  /* MAC_SCH_STATS */
Void rgSCHUhmProcDatInd(RgSchCellCb *cell,RgSchUeCb  *ue,CmLteTimingInfo frm,uint8_t  cqi)
#endif /* MAC_SCH_STATS */
{
   RgSchUlHqProcCb   *hqProc;
#ifdef UL_LA
   RgSchCmnUlUe   *ueUl = RG_SCH_CMN_GET_UL_UE(ue, cell);
   S32            iTbs;
   uint8_t        maxiTbs = rgSchCmnUlCqiToTbsTbl[cell->isCpUlExtend]
                                                 [ueUl->maxUlCqi];
#endif

   rgSCHUtlUlHqProcForUe(cell, frm, ue, &hqProc);
   if (hqProc == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : UE[%d] failed to find UL HqProc for [%d:%d]\n",
		      ue->ueId, frm.sfn, frm.slot);
      return;
   }
   hqProc->rcvdCrcInd = TRUE;

#ifdef UL_LA
   {
      ueUl->ulLaCb.deltaiTbs += UL_LA_STEPUP;
      iTbs = (ueUl->ulLaCb.cqiBasediTbs + ueUl->ulLaCb.deltaiTbs)/100;

      if (iTbs > maxiTbs)
      {
         ueUl->ulLaCb.deltaiTbs = (maxiTbs * 100) - ueUl->ulLaCb.cqiBasediTbs;
      }

   }
#endif
#ifdef MAC_SCH_STATS
   /** Stats update over here 
    */
   {
      hqFailStats.ulCqiStat[cqi - 1].numOfAcks++;
   }
#endif

#ifdef TENB_STATS
   /* UL stats are filled in primary index as of now */
   cell->tenbStats->sch.ulAckNack[rgRvTable[hqProc->rvIdx]]++;
   ue->tenbStats->stats.nonPersistent.sch[RG_SCH_CELLINDEX(cell)].ulAckNackCnt++;
   if(hqProc->alloc)
   {
      ue->tenbStats->stats.nonPersistent.sch[RG_SCH_CELLINDEX(cell)].ulTpt += (hqProc->alloc->grnt.datSz << 3);
      cell->tenbStats->sch.ulTtlTpt +=(hqProc->alloc->grnt.datSz << 3);//pverma
   }
#endif
   
   return;
}  /* rgSCHUhmProcDatInd */

/**
 * @brief Handler for HARQ processing on recieving Data indication from PHY.
 *
 * @details
 *
 *     Function: rgSCHUhmProcMsg3DatInd
 *     
 *     Invoked by: rgSCHTomTfuDatInd of  TOM
 *
 *     Processing Steps:
 *      - Set rcvdCrcInd variable to TRUE  
 *           
 *  @param[in,out] *hqProc
 *  @return  Void
 **/
Void rgSCHUhmProcMsg3DatInd(RgSchUlHqProcCb *hqProc)
{
   hqProc->rcvdCrcInd = TRUE;
   hqProc->remTx = 0;        /*Reseting the value of rem Tx*/
   DU_LOG("\nINFO  -->  SCH : rgSCHUhmProcMsg3DatInd,id:%u\n",hqProc->procId);
   return;
}  /* rgSCHUhmProcMsg3DatInd */

/**
 * @brief Handler for HARQ processing on recieving Data indication from PHY.
 *
 * @details
 *
 *     Function: rgSCHUhmProcMsg3Failure
 *     
 *     Invoked by: rgSCHTomTfuDatInd of  TOM
 *
 *     Processing Steps:
 *      - Set rcvdCrcInd variable to TRUE  
 *           
 *  @param[in,out] *hqProc
 *  @return  Void
 **/
Void rgSCHUhmProcMsg3Failure(RgSchUlHqProcCb *hqProc)
{
#ifdef EMTC_ENABLE
   RG_SCH_EMTC_IS_CRCIND_RCVD_CHK_RACB(hqProc);
#endif  
   if(hqProc->rcvdCrcInd != TRUE) 
   {
      hqProc->rcvdCrcInd = FALSE;
   }

   return;
}  /* rgSCHUhmProcMsg3Failure */

/**
 * @brief Handler for HARQ processing on recieving Decode failure from PHY.
 *
 * @details
 *
 *     Function: rgSCHUhmProcHqFailure
 *     
 *     Invoked by: rgSCHTomTfuDecFailInd of TOM
 *
 *     Processing Steps: 
 *      - Update NACK information in harq info.
 *      - Update RV index of received RV from PHY in harq info.
 *      - Set PhichInfo in DlSf
 *           
 *  @param[in] *cell
 *  @param[in] *ue
 *  @param[in] frm
 *  @param[in] rv
 *  @return  Void
 **/
#ifndef MAC_SCH_STATS
Void rgSCHUhmProcHqFailure(RgSchCellCb *cell,RgSchUeCb *ue,CmLteTimingInfo  frm,TknUInt8  rv)
#else /* MAC_SCH_STATS */
Void rgSCHUhmProcHqFailure(RgSchCellCb  *cell,RgSchUeCb  *ue,CmLteTimingInfo  frm,TknUInt8  rv,uint8_t cqi)
#endif /* MAC_SCH_STATS */
{
   RgSchUlHqProcCb   *hqProc;
#ifdef UL_LA
   RgSchCmnUlUe   *ueUl = RG_SCH_CMN_GET_UL_UE(ue,cell);
   S32               iTbs;
#endif

   rgSCHUtlUlHqProcForUe(cell, frm, ue, &hqProc);
   if (hqProc == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : UE[%d] failed to find UL HqProc for [%d:%d]\n",
		      ue->ueId, frm.sfn, frm.slot);
      return;
   }
#ifdef UL_LA
   {
      ueUl->ulLaCb.deltaiTbs -= UL_LA_STEPDOWN;
      iTbs = (ueUl->ulLaCb.cqiBasediTbs + ueUl->ulLaCb.deltaiTbs)/100;

      if (iTbs < 0)
      {
         ueUl->ulLaCb.deltaiTbs = -(ueUl->ulLaCb.cqiBasediTbs);
      }

   } 
#endif
#ifdef MAC_SCH_STATS
   /** Stats update over here */
   {
      static uint32_t retxCnt = 0;
      ++retxCnt;
      hqFailStats.ulCqiStat[cqi - 1].numOfNacks++;
   }
#endif /* MAC_SCH_STATS */
   if(hqProc->rcvdCrcInd != TRUE) 
   {
      hqProc->rcvdCrcInd = FALSE;
   }
#ifdef TENB_STATS
   /* UL stats are filled in primary index as of now */
   cell->tenbStats->sch.ulAckNack[rgRvTable[hqProc->rvIdx]]++;
   ue->tenbStats->stats.nonPersistent.sch[RG_SCH_CELLINDEX(cell)].ulAckNackCnt++;
   cell->tenbStats->sch.ulNack[rgRvTable[hqProc->rvIdx]]++;
   ue->tenbStats->stats.nonPersistent.sch[RG_SCH_CELLINDEX(cell)].ulNackCnt++;
#endif
   hqProc->rvIdxPhy.pres = rv.pres;
   if(rv.pres)
   {
      hqProc->rvIdxPhy.val = rgRvIdxTable[rv.val];
   }
   return;
} /* rgSCHUhmProcHqFailure */

/**
 * @brief Handler for identifying the HARQ process cb associated with the
 * index.
 *
 * @details
 *
 *     Function: rgSCHUhmGetUlHqProc
 *     
 *     Processing Steps: 
 *      - Return pointer to uplink harq process corresponding to the timing
 *        information passed.
 *           
 *  @param[in]  *ue
 *  @param[in]  idx
 *  @return  RgSchUlHqProcCb*
 *      -# Pointer to harq process corresponding to index
 *      -# NULL
 **/
RgSchUlHqProcCb* rgSCHUhmGetUlHqProc(RgSchCellCb *cell,RgSchUeCb  *ue, uint8_t idx)
{
   RgSchCmnUlUe *ueUl = RG_SCH_CMN_GET_UL_UE(ue, cell);

#if (ERRCLASS & ERRCLS_DEBUG)
   if(idx >= ueUl->hqEnt.numHqPrcs)
   {
      return (NULLP);
   }
#endif
   return (&(ueUl->hqEnt.hqProcCb[idx]));
}  /* rgSCHUhmGetUlHqProc */

/**
 * @brief Handler for HARQ processing on recieving new trasmission indication 
 * from USM.
 *
 * @details
 *
 *     Function: rgSCHUhmNewTx
 *     
 *     Invoked by: USM
 *
 *     Processing Steps: 
 *      - Update harq info with values indicating new HARQ transmission.
 *           
 *  @param[in,out]  *hqProc
 *  @param[in]      *alloc
 *  @return  Void
 **/
Void rgSCHUhmNewTx(RgSchUlHqProcCb *hqProc,uint8_t maxHqRetx,RgSchUlAlloc *alloc)
{

   hqProc->ndi ^= 1;
   hqProc->alloc = alloc;
   hqProc->remTx = maxHqRetx;
   hqProc->rcvdCrcInd = FALSE;
   hqProc->rvIdx = 0;
   hqProc->rvIdxPhy.pres = FALSE;
#ifdef LTE_L2_MEAS
   if (hqProc->alloc->ue)
   {
      ((RgUeUlHqCb*)hqProc->hqEnt)->numBusyHqProcs++;
   }
#endif
   return;
}  /* rgSCHUhmNewTx */

/**
 * @brief Free an uplink HARQ process.
 *
 * @details
 *
 *     Function: rgSCHUhmFreeProc
 *     
 *     Invoked by: USM
 *
 *     Processing Steps: 
 *      - Set alloc pointer to NULLP
 *           
 *  @param[in]  RgSchUlHqProcCb   *hqProc
 *  @param[in]  RgSchCellCb      *cell
 *  @return  Void
 **/
Void rgSCHUhmFreeProc(RgSchUlHqProcCb *hqProc,RgSchCellCb *cell)
{
#ifdef LTE_L2_MEAS
   RgSchUeCb         *ueCb;
   uint8_t qci = 1;
#endif

#ifdef LTE_L2_MEAS
   if (hqProc->alloc && hqProc->alloc->ue)
   {
      ueCb = hqProc->alloc->ue;
      if (ueCb && cell)
      {
         uint32_t nonLcg0ReportedBs = ((RgSchCmnLcg *)(ueCb->ul.lcgArr[1].sch))->reportedBs + \
                               ((RgSchCmnLcg *)(ueCb->ul.lcgArr[2].sch))->reportedBs + \
                               ((RgSchCmnLcg *)(ueCb->ul.lcgArr[3].sch))->reportedBs;
         ((RgUeUlHqCb*)hqProc->hqEnt)->numBusyHqProcs--;
         if (! ((RgUeUlHqCb*)hqProc->hqEnt)->numBusyHqProcs && !(nonLcg0ReportedBs))
         {
            while (ueCb->ulActiveLCs)
            {
               if (ueCb->ulActiveLCs & 0x1)
               {
                  cell->qciArray[qci].ulUeCount--;
               }
               qci++;
               ueCb->ulActiveLCs >>= 1;
            }
         }
      }
   }
#endif


 if(hqProc && (RgUeUlHqCb*)hqProc->hqEnt)
 {

#ifdef UL_ADPT_DBG 
    DU_LOG("\nDEBUG  -->  SCH : ########HARQ FREED HARQPROC ID (%d )after rgSCHUhmFreeProc inuse %ld free %ld \n",hqProc->alloc->grnt.hqProcId, (CmLListCp *)(&((RgUeUlHqCb*)hqProc->hqEnt)->inUse)->count,(CmLListCp *) (&((RgUeUlHqCb*)hqProc->hqEnt)->free)->count);
#endif
   hqProc->alloc = NULLP;
   hqProc->ulSfIdx = RGSCH_INVALID_INFO;
   /*ccpu00116293 - Correcting relation between UL slot and DL slot based on RG_UL_DELTA*/
   hqProc->isRetx  = FALSE;
   hqProc->remTx = 0; /*Reseting the remTx value to 0*/
#ifdef EMTC_ENABLE
   RG_SCH_EMTC_SET_ISDTX_TO_FALSE(hqProc);
#endif
   cmLListDelFrm(&((RgUeUlHqCb*)hqProc->hqEnt)->inUse,&hqProc->lnk);
   cmLListAdd2Tail(&((RgUeUlHqCb*)hqProc->hqEnt)->free, &hqProc->lnk);

   /*
   DU_LOG("\nINFO  -->  SCH : after rgSCHUhmFreeProc inuse %ld free %ld \n", 
        (CmLListCp *)(&((RgUeUlHqCb*)hqProc->hqEnt)->inUse)->count,
         (CmLListCp *) (&((RgUeUlHqCb*)hqProc->hqEnt)->free)->count);
   */
 }
 else
 {
     DU_LOG("\nERROR  -->  SCH : hqEnt is NULL\n");
 }
   return;
}  /* rgSCHUhmFreeProc */

/**
 * @brief Handler for HARQ processing on recieving re-trasmission 
 * indication from USM.
 *
 * @details
 *
 *     Function: rgSCHUhmRetx
 *     
 *     Invoked by: USM
 *
 *     Processing Steps: 
 *      - Update harq info with values corresponding to
 *        re-transmission. 
 *           
 *  @param[in,out]  *hqProc
 *  @return  Void
 **/
Void rgSCHUhmRetx(RgSchUlHqProcCb *hqProc,RgSchUlAlloc    *alloc)
{

   hqProc->alloc = alloc;
   --hqProc->remTx;
   hqProc->rvIdx = (hqProc->rvIdx + 1) % 4;
   hqProc->rvIdxPhy.pres = FALSE;
   return;
}  /* rgSCHUhmRetx */


/**
 * @brief Handler for initializing the HARQ entity.
 *
 * @details
 *
 *     Function: rgSCHUhmRgrUeCfg
 *     
 *     Invoked by: RGR
 *
 *     Processing Steps: 
 *      -  Initialize maxHqRetx
 *           
 *  @param[in]      *cellCb
 *  @param[in,out]  *ueCb
 *  @param[in]      *ueCfg
 *  @param[out]     *err
 *  @return  Void
 **/
Void rgSCHUhmRgrUeCfg(RgSchCellCb *cellCb,RgSchUeCb *ueCb,RgrUeCfg *ueCfg)
{
   uint8_t i;
   RgSchCmnUlUe *ueUl = RG_SCH_CMN_GET_UL_UE(ueCb, cellCb);
   
   ueUl->hqEnt.maxHqRetx = ((ueCfg->ueUlHqCfg.maxUlHqTx) - 1);
#ifdef TFU_UPGRADE
   /* Storing the delta HARQ offset for HARQ + PUSCH */
   ueCb->ul.betaHqOffst = ueCfg->puschDedCfg.bACKIdx;
#endif
   cmLListInit(&ueUl->hqEnt.free);
   cmLListInit(&ueUl->hqEnt.inUse);
   for(i=0; i < ueUl->hqEnt.numHqPrcs; i++)
   {
      ueUl->hqEnt.hqProcCb[i].hqEnt = (void*)(&ueUl->hqEnt);
      ueUl->hqEnt.hqProcCb[i].procId = i;
      ueUl->hqEnt.hqProcCb[i].remTx = 0;
      ueUl->hqEnt.hqProcCb[i].ulSfIdx = RGSCH_INVALID_INFO;
      ueUl->hqEnt.hqProcCb[i].alloc = NULLP;
#ifdef LTEMAC_SPS
      /* ccpu00139513- Initializing SPS flags*/
      ueUl->hqEnt.hqProcCb[i].isSpsActvnHqP = FALSE;
      ueUl->hqEnt.hqProcCb[i].isSpsOccnHqP = FALSE;
#endif
      cmLListAdd2Tail(&ueUl->hqEnt.free, &ueUl->hqEnt.hqProcCb[i].lnk);
      ueUl->hqEnt.hqProcCb[i].lnk.node = (PTR)&ueUl->hqEnt.hqProcCb[i];
   }

#ifdef EMTC_ENABLE
   rgSCHEmtcInitUlUeHqEnt(cellCb, ueCfg, ueCb);
#endif
   return;
}  /* rgSCHUhmRgrUeCfg */

/**
 * @brief Handler for re-initializing the HARQ entity.
 *
 * @details
 *
 *     Function: rgSCHUhmRgrUeRecfg
 *     
 *     Invoked by: RGR
 *
 *     Processing Steps: 
 *      -  Re-initialize maxHqRetx
 *           
 *  @param[in]      *cellCb
 *  @param[in,out]  *ueCb
 *  @param[in]      *ueCfg
 *  @param[out]     *err
 *  @return  Void
 **/
Void rgSCHUhmRgrUeRecfg(RgSchCellCb *cellCb,RgSchUeCb *ueCb,RgrUeRecfg  *ueRecfg)
{
   RgSchCmnUlUe *ueUl = RG_SCH_CMN_GET_UL_UE(ueCb, cellCb);

   /* [ccpu00123958]-ADD- Check for HARQ Reconfig from the bit mask  */
   if(RGR_UE_ULHARQ_RECFG & ueRecfg->ueRecfgTypes)
   {
      ueUl->hqEnt.maxHqRetx = (ueRecfg->ueUlHqRecfg.maxUlHqTx - 1); 
   }

   return;
}  /* rgSCHUhmRgrUeRecfg */

/**
 * @brief Handler for de-initializing the HARQ entity.
 *
 * @details
 *
 *     Function: rgSCHUhmFreeUe
 *     
 *     Invoked by: RGR
 *
 *     Processing Steps: 
 *      - 
 *           
 *  @param[in,out]  *ueCb
 *  @return  Void
 **/
Void rgSCHUhmFreeUe(RgSchCellCb *cellCb,RgUeUlHqCb *hqEnt)
{
#ifdef LTE_TDD
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cellCb->instIdx, 
                  (Data **)(&(hqEnt->hqProcCb)),
                  hqEnt->numHqPrcs * sizeof(RgSchUlHqProcCb));
#endif

   return;
}  /* rgSCHUhmFreeUe */


/**
* @brief Handler for appending the PHICH information in to the dlSf.
*
* @details
*
*     Function: rgSCHUhmAppendPhich
*     
*     Invoked by: TOM
*
*     Processing Steps:
*      - Set PhichInfo in DlSf for each Hq
*
*  @param[in] *RgSchCellCb
*  @param[in] CmLteTimingInfo
*  @param[in] idx
*  @return  Void
*/
S16 rgSCHUhmAppendPhich(RgSchCellCb *cellCb,CmLteTimingInfo frm,uint8_t idx)
{
   uint8_t         nDmrs;
   uint8_t         rbStart;
#ifdef LTE_TDD
   uint8_t         iPhich;
#endif
   RgSchUlAlloc    *ulAlloc;
#ifdef LTEMAC_HDFDD
   Bool            allwNack = TRUE;
#endif /* LTEMAC_HDFDD */
   RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cellCb);

#ifdef RG_5GTF
   return ROK;
#endif
   if(cellUl->hqFdbkIdx[idx] != RGSCH_INVALID_INFO)
   {
      ulAlloc = rgSCHUtlFirstHqFdbkAlloc (cellCb, idx);
      while (ulAlloc)
      {
         /*ccpu00106104 MOD added check for ACKNACK rep*/
         /*added check for acknack so that adaptive retx considers ue 
           inactivity due to ack nack repetition*/
         if((ulAlloc->ue != NULLP) && ((TRUE != ulAlloc->forMsg3) &&
                  ((ulAlloc->ue->measGapCb.isMeasuring == TRUE) ||
                   (ulAlloc->ue->ackNakRepCb.isAckNakRep == TRUE))))
         {
            /* Mark the UE for retransmission */
            /* If UE is measuring then we should not be sending PHICH unless msg3 */
            /*UE assumes ack, if nack then do adaptive re-transmission*/
            /*ulAlloc->hqProc->rcvdCrcInd = FALSE;--*/
            ulAlloc = rgSCHUtlNextHqFdbkAlloc (cellCb, ulAlloc, idx);
            continue;
         }
#ifdef LTE_TDD
         if (rgSCHUtlGetPhichInfo (ulAlloc->hqProc, &rbStart, &nDmrs, &iPhich) != ROK)
#else
            if (rgSCHUtlGetPhichInfo (ulAlloc->hqProc, &rbStart, &nDmrs) != ROK)
#endif
            {
               return  (RFAILED);
            }
         if(nDmrs != RGSCH_INVALID_NDMRS)
         {
            if(cellCb->dynCfiCb.switchOvrInProgress)
            {
               ulAlloc->hqProc->rcvdCrcInd = TRUE;
            }    

            if(ulAlloc->hqProc->rcvdCrcInd) 
            {
#ifdef LTE_TDD
               rgSCHUtlAddPhich (cellCb, frm, TRUE, nDmrs, rbStart, iPhich);
#else
               rgSCHUtlAddPhich (cellCb, frm, TRUE, nDmrs, rbStart, ulAlloc->forMsg3);
#endif
            }
            /* Sending NACK in PHICH for failed UL TX */
            else
            {
#ifdef LTE_TDD
               rgSCHUtlAddPhich (cellCb, frm, FALSE, nDmrs, rbStart, iPhich);
#else
#ifdef LTEMAC_HDFDD
               if (ulAlloc->ue != NULLP && ulAlloc->ue->hdFddEnbld)
               {
                  rgSCHCmnHdFddChkNackAllow( cellCb, ulAlloc->ue, frm, &allwNack);
                  /* Present implementaion of non-HDFDD does not send phich req
                     incase of NACK. So commented this part to maintain same right
                     now.*/

                  if (allwNack)
                  {
                     rgSCHUtlAddPhich (cellCb, frm, FALSE, nDmrs, rbStart, ulAlloc->forMsg3);
                  }
                  else
                  {
                     rgSCHUtlAddPhich (cellCb, frm, TRUE, nDmrs, rbStart, ulAlloc->forMsg3);
                  }
               }
               else
               {
                  rgSCHUtlAddPhich (cellCb, frm, FALSE, nDmrs, rbStart, ulAlloc->forMsg3);
               }
#else
               rgSCHUtlAddPhich (cellCb, frm, FALSE, nDmrs, rbStart, ulAlloc->forMsg3);
#endif/* LTEMAC_HDFDD */
#endif
            }
         }
         ulAlloc = rgSCHUtlNextHqFdbkAlloc (cellCb, ulAlloc, idx);
      }
   }
   return ROK;
} /* rgSCHUhmAppendPhich */

/**
 * @brief This function initializes the DL HARQ Entity of UE.
 *
 * @details
 *
 *     Function: rgSCHUhmHqEntInit
 *     Purpose:  This function initializes the UL HARQ Processes of 
 *               UE control block. This is performed at the time
 *               of creating UE control block.
 *     
 *     Invoked by: configuration module
 *     
 *  @param[in]  RgSchUeCb*    ueCb
 *  @return  S16
 *           -# ROK
 *           -# RFAILED
 *
 **/
S16 rgSCHUhmHqEntInit(RgSchCellCb *cellCb,RgSchUeCb *ueCb)
{
   RgSchCmnUlUe *ueUl = RG_SCH_CMN_GET_UL_UE(ueCb, cellCb);
#ifdef LTE_TDD   
   Inst              inst = ueCb->cell->instIdx;
#endif

#ifndef LTE_TDD
   /* Init the HARQ processes */
   ueUl->hqEnt.numHqPrcs = RGSCH_NUM_UL_HQ_PROC;
#else
   /* Init the HARQ processes */
   ueUl->hqEnt.numHqPrcs = 
                        rgSchTddUlNumHarqProcTbl[ueCb->cell->ulDlCfgIdx];
   if (rgSCHUtlAllocSBuf(inst, (Data **)&ueUl->hqEnt.hqProcCb, 
                           ueUl->hqEnt.numHqPrcs * \
                           sizeof(RgSchUlHqProcCb)) != ROK)
   {
      return RFAILED;
   }
#endif

   return ROK;
} /* rgSCHUhmHqEntInit */

#ifdef RG_5GTF
/**
 * @brief This function gets an available HARQ process.
 *
 * @details
 *
 *     Function: rgSCHUhmGetAvlHqProc
 *     Purpose:  This function returns an available HARQ process in 
 *               the UL direction. All HARQ processes are maintained
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
S16 rgSCHUhmGetAvlHqProc(RgSchCellCb *cell,RgSchUeCb  *ue,RgSchUlHqProcCb  **hqP)
{
   RgSchCmnUlCell    *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchCmnUlUe      *ueUl = RG_SCH_CMN_GET_UL_UE(ue, cell);
   RgSchUlHqProcCb   *tmpHqProc;
   CmLList           *tmp;
   RgUeUlHqCb        *hqE;

   hqE = &ueUl->hqEnt;
 
   CM_LLIST_FIRST_NODE(&(hqE->free), tmp);

   if (NULLP == tmp)
   {
       //DU_LOG("rgSCHUhmGetAvlHqProc free %ld inUse %ld ue %d"
        //                                   , hqE->free.count, hqE->inUse.count, ue->ueId);
      //DU_LOG("5GTF_ERROR rgSCHUhmGetAvlHqProc cellId %d  %ld inUse %ld ue %d"
                              //, cell->cellId, hqE->free.count, hqE->inUse.count, ue->ueId);
      /* No Harq Process available in the free queue. */
      return RFAILED;
   }

   tmpHqProc = (RgSchUlHqProcCb *)(tmp->node);

   /* Remove the element from the free Queue */
   cmLListDelFrm(&hqE->free, tmp);

   /* Add the element into the inUse Queue as well */
   cmLListAdd2Tail(&hqE->inUse, &tmpHqProc->lnk);

#ifdef UL_ADPT_DBG 
         DU_LOG("\nDEBUG  -->  SCH : rgSCHUhmGetAvlHqProc cellId %d  free %ld inUse %ld UE %d time (%d %d)\n",cell->cellId, hqE->free.count, hqE->inUse.count, ue->ueId,cellUl->schdTime.sfn,cellUl->schdTime.slot);
#endif         
   tmpHqProc->schdTime = cellUl->schdTime;

   *hqP = tmpHqProc;
   
   return ROK;
} /* rgSCHUhmGetAvlHqProc */

/**
 * @brief Handler for identifying the HARQ process cb associated with the
 * TX Time.
 *
 * @details
 *
 *     Function: rgSCHUhmGetUlProcByTime 
 *     
 *     Processing Steps: 
 *      - Return pointer to uplink harq process corresponding to the timing
 *        information passed.
 *           
 *  @param[in]  *ue
 *  @param[in]  idx
 *  @return  RgSchUlHqProcCb*
 *      -# Pointer to harq process corresponding to index
 *      -# NULL
 **/
RgSchUlHqProcCb* rgSCHUhmGetUlProcByTime(RgSchCellCb *cell,RgSchUeCb *ue, CmLteTimingInfo  frm)
{
   RgSchCmnUlUe *ueUl = RG_SCH_CMN_GET_UL_UE(ue, cell);
   CmLListCp    *lst = &ueUl->hqEnt.inUse;
   CmLList      *lnk = lst->first;
   RgSchUlHqProcCb   *proc;

   while (lnk)
   {
      proc = (RgSchUlHqProcCb *)(lnk->node);
      lnk = lnk->next;
      //   DU_LOG("\nINFO  -->  SCH : compare rgSCHUhmGetUlProcByTime time (%d %d) CRC time (%d %d) proc->procId %d
      //   \n",\proc->schdTime.sfn,proc->schdTime.slot,frm.sfn,frm.slot ,proc->procId);
      if (RGSCH_TIMEINFO_SAME(proc->schdTime, frm))
      {
        // DU_LOG("\nINFO  -->  SCH : Harq timing Matched rgSCHUhmGetUlProcByTime time (%d %d) proc->procId %d \n",\
	proc->schdTime.sfn,proc->schdTime.slot, proc->procId);
         return (proc);
      }
   }

   return (NULLP);
}  /* rgSCHUhmGetUlProcByTime */
#endif


/**********************************************************************
 
         End of file
**********************************************************************/
