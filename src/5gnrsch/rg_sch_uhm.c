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
   U32 gUlRetxPassCntr = 0;
   /*EXTERN U32 gUlRetxFailCntr;
   EXTERN U32 gUlCrcPassCounter;
   EXTERN U32 gUlCrcFailCounter;*/
#endif
U8 rgRvIdxTable[] = {0, 3, 1, 2}; /* This gives rvIdx for a given rv */
U8 rgRvTable[] = {0, 2, 3 ,1};    /* This gives rv for a given rvIdx */

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
#ifdef ANSI
Void rgSCHUhmProcDatInd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      frm
)
#else
Void rgSCHUhmProcDatInd(cell, ue, frm)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
CmLteTimingInfo      frm;
#endif
#else  /* MAC_SCH_STATS */
#ifdef ANSI
Void rgSCHUhmProcDatInd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      frm,
U8                   cqi
)
#else
Void rgSCHUhmProcDatInd(cell, ue, frm, cqi)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
CmLteTimingInfo      frm;
U8                   cqi;
#endif
#endif /* MAC_SCH_STATS */
{
   RgSchUlHqProcCb   *hqProc;
#ifdef UL_LA
   RgSchCmnUlUe   *ueUl = RG_SCH_CMN_GET_UL_UE(ue, cell);
   S32            iTbs;
   U8             maxiTbs = rgSchCmnUlCqiToTbsTbl[cell->isCpUlExtend]
                                                 [ueUl->maxUlCqi];
#endif

   TRC2(rgSCHUhmProcDatInd);

   rgSCHUtlUlHqProcForUe(cell, frm, ue, &hqProc);
   if (hqProc == NULLP)
   {
      printf("UE[%d] failed to find UL HqProc for [%d:%d]\n",
		      ue->ueId, frm.sfn, frm.slot);
      RETVOID;
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
   
   RETVOID;
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
#ifdef ANSI
Void rgSCHUhmProcMsg3DatInd
(
RgSchUlHqProcCb *hqProc
)
#else
Void rgSCHUhmProcMsg3DatInd(hqProc)
RgSchUlHqProcCb *hqProc;
#endif
{
   TRC2(rgSCHUhmProcMsg3DatInd);
   hqProc->rcvdCrcInd = TRUE;
   hqProc->remTx = 0;        /*Reseting the value of rem Tx*/
   printf("\nrgSCHUhmProcMsg3DatInd,id:%u\n",hqProc->procId);
   RETVOID;
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
#ifdef ANSI
Void rgSCHUhmProcMsg3Failure
(
RgSchUlHqProcCb *hqProc
)
#else
Void rgSCHUhmProcMsg3Failure(hqProc)
RgSchUlHqProcCb *hqProc;
#endif
{
   TRC2(rgSCHUhmProcMsg3Failure);
#ifdef EMTC_ENABLE
   RG_SCH_EMTC_IS_CRCIND_RCVD_CHK_RACB(hqProc);
#endif  
   if(hqProc->rcvdCrcInd != TRUE) 
   {
      hqProc->rcvdCrcInd = FALSE;
   }

   RETVOID;
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
#ifdef ANSI
Void rgSCHUhmProcHqFailure
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      frm,
TknU8                rv
)
#else
Void rgSCHUhmProcHqFailure(cell, ue, frm, rv)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
CmLteTimingInfo      frm;
TknU8                rv;
#endif
#else /* MAC_SCH_STATS */
#ifdef ANSI
Void rgSCHUhmProcHqFailure
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      frm,
TknU8                rv,
U8                   cqi
)
#else
Void rgSCHUhmProcHqFailure(cell, ue, frm, rv, cqi)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
CmLteTimingInfo      frm;
TknU8                rv;
U8                   cqi;
#endif
#endif /* MAC_SCH_STATS */
{
   RgSchUlHqProcCb   *hqProc;
#ifdef UL_LA
   RgSchCmnUlUe   *ueUl = RG_SCH_CMN_GET_UL_UE(ue,cell);
   S32               iTbs;
#endif
   TRC2(rgSCHUhmProcHqFailure);

   rgSCHUtlUlHqProcForUe(cell, frm, ue, &hqProc);
   if (hqProc == NULLP)
   {
      printf("UE[%d] failed to find UL HqProc for [%d:%d]\n",
		      ue->ueId, frm.sfn, frm.slot);
      RETVOID;
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
      static U32 retxCnt = 0;
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
   RETVOID;
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
#ifdef ANSI
RgSchUlHqProcCb* rgSCHUhmGetUlHqProc
(
RgSchCellCb      *cell,
RgSchUeCb        *ue, 
U8               idx
)
#else
RgSchUlHqProcCb* rgSCHUhmGetUlHqProc(cell, ue, idx)
RgSchCellCb      *cell;
RgSchUeCb        *ue; 
U8               idx;
#endif
{
   RgSchCmnUlUe *ueUl = RG_SCH_CMN_GET_UL_UE(ue, cell);
   TRC2(rgSCHUhmGetUlHqProc);

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
#ifdef ANSI
Void rgSCHUhmNewTx
(
RgSchUlHqProcCb *hqProc,
U8              maxHqRetx,
RgSchUlAlloc    *alloc
)
#else
Void rgSCHUhmNewTx(hqProc, maxHqRetx, alloc)
RgSchUlHqProcCb *hqProc;
U8              maxHqRetx;
RgSchUlAlloc    *alloc;
#endif
{
   TRC2(rgSCHUhmNewTx);

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
   RETVOID;
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
#ifdef ANSI
Void rgSCHUhmFreeProc
(
RgSchUlHqProcCb *hqProc,
RgSchCellCb      *cell
)
#else
Void rgSCHUhmFreeProc(hqProc, cell)
RgSchUlHqProcCb *hqProc;
RgSchCellCb      *cell;
#endif
{
#ifdef LTE_L2_MEAS
   RgSchUeCb         *ueCb;
   U8 qci = 1;
#endif
   TRC2(rgSCHUhmFreeProc);

#ifdef LTE_L2_MEAS
   if (hqProc->alloc && hqProc->alloc->ue)
   {
      ueCb = hqProc->alloc->ue;
      if (ueCb && cell)
      {
         U32 nonLcg0ReportedBs = ((RgSchCmnLcg *)(ueCb->ul.lcgArr[1].sch))->reportedBs + \
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
    printf("\n\n########HARQ FREED HARQPROC ID (%d )after rgSCHUhmFreeProc inuse %ld free %ld \n",hqProc->alloc->grnt.hqProcId, (CmLListCp *)(&((RgUeUlHqCb*)hqProc->hqEnt)->inUse)->count,(CmLListCp *) (&((RgUeUlHqCb*)hqProc->hqEnt)->free)->count);
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
   printf("after rgSCHUhmFreeProc inuse %ld free %ld \n", 
        (CmLListCp *)(&((RgUeUlHqCb*)hqProc->hqEnt)->inUse)->count,
         (CmLListCp *) (&((RgUeUlHqCb*)hqProc->hqEnt)->free)->count);
   */
 }
 else
 {
     printf("\nhqEnt is NULL\n");
 }
   RETVOID;
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
#ifdef ANSI
Void rgSCHUhmRetx
(
RgSchUlHqProcCb *hqProc,
RgSchUlAlloc    *alloc
)
#else
Void rgSCHUhmRetx(hqProc, alloc) 
RgSchUlHqProcCb *hqProc;
RgSchUlAlloc    *alloc;
#endif
{
   TRC2(rgSCHUhmRetx);

   hqProc->alloc = alloc;
   --hqProc->remTx;
   hqProc->rvIdx = (hqProc->rvIdx + 1) % 4;
   hqProc->rvIdxPhy.pres = FALSE;
   RETVOID;
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
#ifdef ANSI
Void rgSCHUhmRgrUeCfg
(
RgSchCellCb       *cellCb,
RgSchUeCb         *ueCb,
RgrUeCfg          *ueCfg
)
#else
Void rgSCHUhmRgrUeCfg(cellCb, ueCb, ueCfg) 
RgSchCellCb       *cellCb;
RgSchUeCb         *ueCb;
RgrUeCfg          *ueCfg;
#endif
{
   U8 i;
   RgSchCmnUlUe *ueUl = RG_SCH_CMN_GET_UL_UE(ueCb, cellCb);
   
   TRC2(rgSCHUhmRgrUeCfg);

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
   RETVOID;
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
#ifdef ANSI
Void rgSCHUhmRgrUeRecfg
(
RgSchCellCb       *cellCb,
RgSchUeCb         *ueCb,
RgrUeRecfg        *ueRecfg
)
#else
Void rgSCHUhmRgrUeRecfg(cellCb, ueCb, ueRecfg) 
RgSchCellCb       *cellCb;
RgSchUeCb         *ueCb;
RgrUeRecfg        *ueRecfg;
#endif
{
   RgSchCmnUlUe *ueUl = RG_SCH_CMN_GET_UL_UE(ueCb, cellCb);
   TRC2(rgSCHUhmRgrUeRecfg);

   /* [ccpu00123958]-ADD- Check for HARQ Reconfig from the bit mask  */
   if(RGR_UE_ULHARQ_RECFG & ueRecfg->ueRecfgTypes)
   {
      ueUl->hqEnt.maxHqRetx = (ueRecfg->ueUlHqRecfg.maxUlHqTx - 1); 
   }

   RETVOID;
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
#ifdef ANSI
Void rgSCHUhmFreeUe
(
RgSchCellCb       *cellCb,
RgUeUlHqCb        *hqEnt
)
#else
Void rgSCHUhmFreeUe(cellCb, hqEnt) 
RgSchCellCb       *cellCb;
RgUeUlHqCb       *hqEnt;
#endif
{
   TRC2(rgSCHUhmFreeUe);
#ifdef LTE_TDD
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cellCb->instIdx, 
                  (Data **)(&(hqEnt->hqProcCb)),
                  hqEnt->numHqPrcs * sizeof(RgSchUlHqProcCb));
#endif

   RETVOID;
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
#ifdef ANSI
S16 rgSCHUhmAppendPhich
(
RgSchCellCb            *cellCb,
CmLteTimingInfo        frm,
U8                     idx
)
#else
S16 rgSCHUhmAppendPhich (cellCb, frm, idx)
RgSchCellCb            *cellCb;
CmLteTimingInfo        frm;
U8                     idx;
#endif
{
   U8              nDmrs;
   U8              rbStart;
#ifdef LTE_TDD
   U8              iPhich;
#endif
   RgSchUlAlloc    *ulAlloc;
#ifdef LTEMAC_HDFDD
   Bool            allwNack = TRUE;
#endif /* LTEMAC_HDFDD */
   RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cellCb);

   TRC2(rgSCHUhmAppendPhich)

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
#ifdef ANSI
S16 rgSCHUhmHqEntInit
(
RgSchCellCb       *cellCb,
RgSchUeCb           *ueCb
)
#else
S16 rgSCHUhmHqEntInit(cellCb, ueCb)
RgSchCellCb       *cellCb;
RgSchUeCb           *ueCb;
#endif
{
   RgSchCmnUlUe *ueUl = RG_SCH_CMN_GET_UL_UE(ueCb, cellCb);
#ifdef LTE_TDD   
   Inst              inst = ueCb->cell->instIdx;
#endif
   TRC2(rgSCHUhmHqEntInit)

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
#ifdef ANSI
S16 rgSCHUhmGetAvlHqProc
(
RgSchCellCb           *cell,
RgSchUeCb               *ue,
RgSchUlHqProcCb         **hqP
)
#else
S16 rgSCHUhmGetAvlHqProc (cell, ue, hqP)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
RgSchUlHqProcCb       **hqP;
#endif
{
   RgSchCmnUlCell    *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchCmnUlUe      *ueUl = RG_SCH_CMN_GET_UL_UE(ue, cell);
   RgSchUlHqProcCb   *tmpHqProc;
   CmLList           *tmp;
   RgUeUlHqCb        *hqE;

   TRC2(rgSCHUhmGetAvlHqProc);

   hqE = &ueUl->hqEnt;
 
   CM_LLIST_FIRST_NODE(&(hqE->free), tmp);

   if (NULLP == tmp)
   {
      //RLOG_ARG3(L_ERROR,DBG_CELLID,cell->cellId,
       //                "rgSCHUhmGetAvlHqProc free %ld inUse %ld ue %d"
        //                                   , hqE->free.count, hqE->inUse.count, ue->ueId);
      //printf("5GTF_ERROR rgSCHUhmGetAvlHqProc cellId %d  %ld inUse %ld ue %d"
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
         printf("rgSCHUhmGetAvlHqProc cellId %d  free %ld inUse %ld ue %d time (%d %d)\n",cell->cellId, hqE->free.count, hqE->inUse.count, ue->ueId,cellUl->schdTime.sfn,cellUl->schdTime.slot);
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
#ifdef ANSI
RgSchUlHqProcCb* rgSCHUhmGetUlProcByTime 
(
RgSchCellCb      *cell,
RgSchUeCb        *ue, 
CmLteTimingInfo  frm
)
#else
RgSchUlHqProcCb* rgSCHUhmGetUlProcByTime(cell, ue, frm)
RgSchCellCb      *cell;
RgSchUeCb        *ue; 
CmLteTimingInfo  frm;
#endif
{
   RgSchCmnUlUe *ueUl = RG_SCH_CMN_GET_UL_UE(ue, cell);
   CmLListCp    *lst = &ueUl->hqEnt.inUse;
   CmLList      *lnk = lst->first;
   RgSchUlHqProcCb   *proc;

   TRC2(rgSCHUhmGetUlProcByTime);

   while (lnk)
   {
      proc = (RgSchUlHqProcCb *)(lnk->node);
      lnk = lnk->next;
      //   printf("compare rgSCHUhmGetUlProcByTime time (%d %d) CRC time (%d %d) proc->procId %d \n",proc->schdTime.sfn,proc->schdTime.slot,frm.sfn,frm.slot ,proc->procId);
      if (RGSCH_TIMEINFO_SAME(proc->schdTime, frm))
      {
        // printf("Harq timing Matched rgSCHUhmGetUlProcByTime time (%d %d) proc->procId %d \n",proc->schdTime.sfn,proc->schdTime.slot, proc->procId);
         return (proc);
      }
   }

   return (NULLP);
}  /* rgSCHUhmGetUlProcByTime */
#endif


/**********************************************************************
 
         End of file
**********************************************************************/
