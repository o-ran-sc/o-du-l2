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
  
     Desc:     C source code for Entry point fucntions.
  
     File:     rg_rom.c
  
**********************************************************************/

/** @file rg_rom.c
@brief APIs to handle all the primitives invoked on RGU interface.
*/


/* header include files (.h) */
#include "common_def.h"
#include "rg_env.h"        /* MAC Environment Defines */
#include "crg.h"           /* CRG Interface defines */
#include "rgu.h"           /* RGU Interface defines */
#include "tfu.h"           /* TFU Interface defines */
#include "rg_sch_inf.h"           /* RGR Interface defines */
#include "lrg.h"           /* LRG Interface defines */

#include "rg.h"            /* MAC defines */
#include "rg_err.h"        /* MAC error defines */

/* header/extern include files (.x) */

#include "crg.x"           /* CRG Interface includes */
#include "rgu.x"           /* RGU Interface includes */
#include "tfu.x"           /* TFU Interface includes */
#include "rg_sch_inf.x"    /* SCH Interface includes */
#include "rg_prg.x"        /* PRG Interface includes */
#include "lrg.x"           /* LRG Interface includes */

#include "du_app_mac_inf.h"
#include "rg.x"            /* MAC includes */

/* local defines */
#define RG_NON_MIMO_IDX 0

/* local typedefs */
 
S16 RgMacSchBrdcmDedBoUpdtReq ARGS((Inst inst, CmLteCellId cellId, CmLteRnti rnti, CmLteLcId lcId, S32 bo ));
static S16 rgROMHndlCcchDatReq     ARGS((RgCellCb *cell,
                                    RgRguCmnDatReq *datReq, RgErrInfo *err));
static S16 rgROMHndlBcchPcchDatReq ARGS((RgCellCb *cell,
                                    RgRguCmnDatReq *datReq, RgErrInfo *err));
static S16 rgROMHndlCcchStaRsp     ARGS((RgCellCb *cell, 
                                    RgRguCmnStaRsp *staRsp, RgErrInfo *err));
static S16 rgROMHndlBcchPcchStaRsp ARGS((RgCellCb *cell, 
                                    RgRguCmnStaRsp *staRsp, RgErrInfo *err));

/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
static S16 rgROMUpdDlSfRemDataCnt ARGS((RgCellCb  *cellCb,
                                    RgDlSf    *dlSf));
S16 rgTOMUtlProcDlSf ARGS(( RgDlSf     *dlSf,
                                   RgCellCb   *cellCb,
                                   RgErrInfo  *err));
#endif

/* forward references */

/**
 * @brief Handler for dedicated DatReq received on RGU for an UE.
 *
 * @details
 *
 *     Function : rgROMDedDatReq
 *     
 *     This function shall
 *      -# store the BO reported for the given logical channels.
 *      -# invoke DL HARQ for further processing.
 *     
 *           
 *  @param[in]  Inst        inst
 *  @param[in]  RgRguDedDatReq *datReq 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgROMDedDatReq(Inst inst,RgRguDedDatReq *datReq)
{
#if 0
   RgCellCb     *cell;
   RgUeCb       *ue;
   uint8_t           idx1,idx2;
   RgDlHqProcCb *hqProc;
   uint8_t           hqPId;
   RgErrInfo    err;
   Pst          schPst;
   RgInfDedBoRpt boRpt;
   CmLteTimingInfo timingInfo;
   RgDlSf       *sf;
#if (ERRCLASS & ERRCLS_DEBUG)
   RgUstaDgn   dgn;      /* Alarm diagnostics structure */
#endif
/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
   S16 ret;
#endif
   uint32_t idx;
   //uint8_t  datReqFailCnt = 0;


   if (((cell = rgCb[inst].cell) == NULLP) 
       || (cell->cellId != datReq->cellId))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      /* Handle Cell fetch failure */
      DU_LOG("\nERROR  -->  MAC : rgROMDedDatReq(): Invalid cell Id");
#endif
      err.errType = RGERR_ROM_DEDDATREQ;
      err.errCause = RGERR_ROM_INV_CELL_ID;
      if(cell != NULLP)
      {
         /* Update stats */
         rgUpdtRguDedSts(inst,cell->rguDlSap,RG_RGU_SDU_DROP, datReq);
      }
      return RFAILED;
   }

/* Add loop here to scan for all UEs in the consolidated DDatReq*/
   for(idx = 0; idx < datReq->nmbOfUeGrantPerTti; idx++)
   {

      timingInfo.slot = (uint8_t)((datReq->datReq[idx].transId >> 8) & 0XFF);
      timingInfo.sfn = (uint16_t)((datReq->datReq[idx].transId >> 16) & 0xFFFF);
      sf = &cell->subFrms[(timingInfo.slot % RG_NUM_SUB_FRAMES)];

      if( (sf->txDone == TRUE) ||
            (!RG_TIMEINFO_SAME(sf->schdTime,timingInfo)))
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         /* Transmission is already done for this slot. This is a delayed
          * datReq. So discard */
         rgFillDgnParams(inst,&dgn, LRG_USTA_DGNVAL_MEM);
         rgLMMStaInd(inst,LCM_CATEGORY_PROTOCOL, LCM_EVENT_UI_INV_EVT,
               LRG_CAUSE_DELAYED_DATREQ, &dgn);
#endif
         err.errType = RGERR_ROM_DEDDATREQ;
         err.errCause = RGERR_ROM_DELAYED_DATREQ;
         /* Update stats */
         rgUpdtRguDedSts(inst,cell->rguDlSap,RG_RGU_SDU_DROP, datReq);
#ifdef CA_DBG
         {
            uint32_t dbgDelayedDatReqInMac;
            dbgDelayedDatReqInMac++;
         }
#endif /* CA_DBG */         
#ifndef L2_OPTMZ
         RG_DROP_RGUDDATREQ_MBUF(datReq->datReq[idx]);
#endif
         continue;
        // return RFAILED;
      }

      if ((ue = rgDBMGetUeCb(cell, datReq->datReq[idx].rnti)) == NULLP)
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
            /* Handle Ue fetch failure */
           DU_LOG("\nERROR  -->  MAC : rgROMDedDatReq(): Invalid ue Id");
#endif
            err.errType = RGERR_ROM_DEDDATREQ;
            err.errCause = RGERR_ROM_INV_UE_ID;
            /* Update stats */
            rgUpdtRguDedSts(inst,cell->rguDlSap,RG_RGU_SDU_DROP, datReq);
            /* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
            /* Trying to send the prev successful PDU's 
             * if present */
            ret = rgROMUpdDlSfRemDataCnt(cell, sf);
            if(ret == RFAILED)
            {
               DU_LOG("\nERROR  -->  MAC : Dropping due to no ue \n");
#ifndef L2_OPTMZ
               RG_DROP_RGUDDATREQ_MBUF(datReq->datReq[idx]);
#endif
               /* Return from here as above functions found more datReq than expected*/
              /* return (ret); */
            }
#endif
            /* Conitnue for next UE */
            continue;
      }

      hqPId = (uint8_t)(datReq->datReq[idx].transId);
      hqPId = hqPId >> 2;
      /* get harq process and invoke DHM */
      rgDHMGetHqProcFrmId(ue, hqPId, &hqProc);

      if (rgDHMHndlDedDatReq(inst,hqProc, &datReq->datReq[idx], sf, &err) == RFAILED)
      {
         DU_LOG("\nERROR  -->  MAC : Handling of Data request in DHM failedi RNTI:%d",
                    datReq->datReq[idx].rnti);
         err.errType = RGERR_ROM_DEDDATREQ;
         /* errcause shall be filled in appropriately by DHM */
         /* Update stats */
         rgUpdtRguDedSts(inst,ue->rguDlSap,RG_RGU_SDU_DROP, datReq);
         /* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
         /* Trying to send the prev successful PDU's 
          * if present */
         ret = rgROMUpdDlSfRemDataCnt(cell, sf);
         if(ret == RFAILED)
         {
            DU_LOG("\nERROR  -->  MAC : Dropping due to no failure of remCnt update");
#ifndef L2_OPTMZ
            RG_DROP_RGUDDATREQ_MBUF(datReq->datReq[idx]);
#endif
            /* Return from here as above functions found more datReq than expected*/
            //return (ret);
         }
#endif
         continue;
      }

      /* Merging the updation of statistics of SDUs with for loop below */ 

      rgGetPstToInst(&schPst,inst, cell->schInstMap.schInst);
      schPst.event = 0;
      boRpt.cellSapId  = cell->schInstMap.cellSapId;
      boRpt.cellId  = datReq->cellId;

      boRpt.rnti    = datReq->datReq[idx].rnti; 


      /* Fill the DStaRsp struct and send it to scheduler */
      for (idx1 = 0; idx1 < datReq->datReq[idx].nmbOfTbs; idx1++)
      {
         for(idx2 = 0; idx2 < datReq->datReq[idx].datReqTb[idx1].nmbLch; idx2++)
         {
            /* Updating dedicated SDUs received statistics without 
               additional function above for optimization */
            ue->rguDlSap->sapSts.numPduRcvd +=
               datReq->datReq[idx].datReqTb[idx1].lchData[idx2].pdu.numPdu;

            boRpt.lcId    = datReq->datReq[idx].datReqTb[idx1].lchData[idx2].lcId; 
            boRpt.bo      = datReq->datReq[idx].datReqTb[idx1].lchData[idx2].boReport.bo;
            boRpt.oldestSduArrTime 
               = datReq->datReq[idx].datReqTb[idx1].lchData[idx2].boReport.oldestSduArrTime;
            boRpt.staPduBo = datReq->datReq[idx].datReqTb[idx1].lchData[idx2].boReport.staPduBo;
            
            boRpt.setMaxUlPrio= datReq->datReq[idx].datReqTb[idx1].lchData[idx2].setMaxUlPrio;
#ifdef CCPU_OPT
            boRpt.setMaxDlPrio= datReq->datReq[idx].datReqTb[idx1].lchData[idx2].boReport.staPduPrsnt;
#endif
            RgMacSchDedBoUpdt(&schPst, &boRpt);
         }
      }

      /* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
//       sf->remDatReqCnt -= datReqFailCnt;
      /*Presently this function is not returning RFAILED, thus not checking
        for failure condition.*/
      ret = rgROMUpdDlSfRemDataCnt(cell, sf);
      if(ret == RFAILED)
      {
         DU_LOG("\nERROR  -->  MAC : Dropping due to no failure of remCnt update(1) \n");
#ifndef L2_OPTMZ
         RG_DROP_RGUDDATREQ_MBUF(datReq->datReq[idx]);
#endif
         /* Return from here as above functions found more datReq than expected*/
        // return (ret);
      }
#endif
   } /* for loop for num of Ue per TTI*/

#endif
   /* Data send successfully to PHY. lets retuns ROK*/
   return ROK;
}  /* rgROMDedDatReq */


/**
 * @brief Handler for DatReq received on RGU for a common logical channel.
 *
 * @details
 *
 *     Function : rgROMCmnDatReq
 *     
 *     This function shall invoke rgROMHndlCcchDatReq() if datReq is on CCCH
 *     If not, it shall invoke rgROMHndlBcchPcchDatReq().
 *     
 *           
 *  @param[in]  Inst        inst
 *  @param[in]  RgRguCmnDatReq *datReq 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgROMCmnDatReq(Inst inst, RgRguCmnDatReq *datReq)
{
   RgCellCb    *cell;
   RgErrInfo   err;
   S16         ret;
/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
   CmLteTimingInfo timingInfo;
   RgDlSf   *sf;
#endif

   ret = ROK;
   err.errType = RGERR_ROM_CMNDATREQ;
   if(((cell = rgCb[inst].cell) == NULLP)
      ||(cell->cellId != datReq->cellId))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      /* Handle Cell fetch failure */
     DU_LOG("\nERROR  -->  MAC : rgROMCmnDatReq(): Invalid cell Id");
#endif
      err.errCause = RGERR_ROM_INV_CELL_ID;
      /* Update stats */
      if(cell != NULLP)
      {
         rgUpdtRguCmnSts(inst,cell->rguDlSap,RG_RGU_SDU_DROP);
      }
      return RFAILED;
   }

   if (datReq->lcId == cell->dlCcchId)
   {
      ret = rgROMHndlCcchDatReq(cell, datReq, &err);

      /*Get the timing Info*/
      /* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
      timingInfo.slot = (uint8_t)((datReq->transId >> 8) & 0XFF);
      timingInfo.sfn = (uint16_t)((datReq->transId >> 16) & 0xFFFF);
#endif
   } 
   else
   {
      ret = rgROMHndlBcchPcchDatReq(cell, datReq, &err);

      /*Get the timing Info*/
      /* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
      timingInfo.slot = (uint8_t)(datReq->transId & 0XFF);
      timingInfo.sfn = (uint16_t)((datReq->transId >> 8) & 0xFFFF);
#endif
   }

   /* Update stats */
   if (ret == RFAILED)
   {
      rgUpdtRguCmnSts(inst,cell->rguDlSap,RG_RGU_SDU_DROP);
   }
   else
   {
      /* Update stats with number of SDUs received */
      rgUpdtRguCmnSts(inst,cell->rguDlSap,RG_RGU_SDU_RCVD);
   }

   /* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
   RG_ARRAY_BOUND_CHECK(0, cell->subFrms, (timingInfo.slot % RG_NUM_SUB_FRAMES));
   sf = &cell->subFrms[(timingInfo.slot % RG_NUM_SUB_FRAMES)];

   ret = rgROMUpdDlSfRemDataCnt(cell, sf);
   /*Added check for RFAILED as above function can return RFAILED*/
#endif

   return (ret);
}  /* rgROMCmnDatReq */

/**
 * @brief Handler for DatReq received on RGU for CCCH.
 *
 * @details
 *
 *     Function : rgROMHndlCcchDatReq
 *     
 *     This function shall fetch the raCb with the given rnti and indicate msg4
 *     arrival to RAM.
 *     
 *           
 *  @param[in]  RgCellCb       *cell
 *  @param[in]  RgRguCmnDatReq *datReq 
 *  @param[out] RgErrInfo      *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgROMHndlCcchDatReq(RgCellCb *cell, RgRguCmnDatReq *datReq, RgErrInfo *err)
{
   Inst     inst = cell->macInst - RG_INST_START;
   RgUeCb   *ue;
   uint8_t   hqPId;
   RgDlHqProcCb *hqProc;
   CmLteTimingInfo timingInfo;
   RgDlSf   *sf;
#if (ERRCLASS & ERRCLS_DEBUG)
   RgUstaDgn   dgn;      /* Alarm diagnostics structure */
#endif

   err->errType = RGERR_ROM_CMNDATREQ;

   if ((ue = rgDBMGetUeCb(cell, datReq->u.rnti)) == NULLP)
   {
      if ((ue = rgDBMGetUeCbFromRachLst(cell, datReq->u.rnti)) == NULLP)
      {
   #if (ERRCLASS & ERRCLS_INT_PAR)
         /* Handle Ue fetch failure */
         DU_LOG("\nERROR  -->  MAC : rgROMHndlCcchDatReq(): Invalid ue Id");
   #endif
         err->errCause = RGERR_ROM_INV_UE_ID;
         return RFAILED;
      }
   }

   timingInfo.slot = (uint8_t)((datReq->transId >> 8) & 0XFF);
   timingInfo.sfn = (uint16_t)((datReq->transId >> 16) & 0xFFFF);
   sf = &cell->subFrms[(timingInfo.slot % RG_NUM_SUB_FRAMES)];

   if( (sf->txDone == TRUE) ||
       (!RG_TIMEINFO_SAME(sf->schdTime,timingInfo)))
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      /* Transmission is already done for this slot. This is a delayed
       * datReq. So discard */
      rgFillDgnParams(inst,&dgn, LRG_USTA_DGNVAL_MEM);
      rgLMMStaInd(inst,LCM_CATEGORY_PROTOCOL, LCM_EVENT_UI_INV_EVT,
                              LRG_CAUSE_DELAYED_DATREQ, &dgn);
#endif
      err->errCause = RGERR_ROM_DELAYED_DATREQ;
      return RFAILED;
   }

   hqPId = (uint8_t)(datReq->transId);
   hqPId = hqPId >> 2;

   /* get harq process and invoke DHM */
   rgDHMGetHqProcFrmId(ue, hqPId, &hqProc);
 
   /* Changed for CR timer implementation*/
   /* invoke DHM to process CCCH data */
   if (rgDHMHndlCmnDatReq(inst,hqProc, datReq, err) == RFAILED)
   {
      DU_LOG("\nERROR  -->  MAC : Handling of Data request in DHM failed RNTI:%d LCID:%d",
            datReq->u.rnti,datReq->lcId);
      /* Release First TB */
      rgDHMRlsHqProcTB(cell, hqProc, 1);
      /* err shall be filled in appropriately by DHM */
      return RFAILED;
   }
 
   return ROK; 
} /* rgROMHndlCcchDatReq */


/**
 * @brief Handler for DatReq received on RGU for BCCH or PCCH.
 *
 * @details
 *
 *     Function : rgROMHndlBcchPcchDatReq
 *     
 *     This function shall store the buffer and time to transmit in lcCb.
 *     
 *           
 *  @param[in]  RgCellCb       *cell
 *  @param[in]  RgRguCmnDatReq *datReq 
 *  @param[out] RgErrInfo      *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgROMHndlBcchPcchDatReq(RgCellCb *cell, RgRguCmnDatReq *datReq, RgErrInfo *err)
{
   Inst            inst = cell->macInst - RG_INST_START;
   RgPcchLcCb      *pcch;
   /* Modified for SI Enhancement*/
#ifndef RGR_SI_SCH
   RgBcchBchLcCb   *bch;
   RgBcchDlschLcCb *bcch;
#endif/*RGR_SI_SCH*/
   RgDlSf          *sf;
   CmLteTimingInfo timingInfo;
#if (ERRCLASS & ERRCLS_DEBUG)
   RgUstaDgn   dgn;      /* Alarm diagnostics structure */
#endif


   timingInfo.slot = (uint8_t)(datReq->transId & 0XFF);
   timingInfo.sfn = (uint16_t)((datReq->transId >> 8) & 0xFFFF);
   sf = &cell->subFrms[(timingInfo.slot % RG_NUM_SUB_FRAMES)];

   if( (sf->txDone == TRUE) ||
       (!RG_TIMEINFO_SAME(sf->schdTime,timingInfo)))
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      /* Transmission is already done for this slot. This is a delayed
       * datReq. So discard */
      rgFillDgnParams(inst,&dgn, LRG_USTA_DGNVAL_MEM);
      rgLMMStaInd(inst,LCM_CATEGORY_PROTOCOL, LCM_EVENT_UI_INV_EVT,
                              LRG_CAUSE_DELAYED_DATREQ, &dgn);
#endif
      err->errCause = RGERR_ROM_DELAYED_DATREQ;
      return RFAILED;
   }

#ifndef RGR_SI_SCH
   bcch = rgDBMGetBcchOnDlsch(cell,datReq->lcId);
   if (bcch )
   {
      /* Store BCCH-DLSCH data received in Scheduled slot */
      sf->bcch.tb = datReq->pdu;

      SCpyMsgMsg(datReq->pdu, RG_GET_MEM_REGION(rgCb[inst]),
               RG_GET_MEM_POOL(rgCb[inst]), &bcch->tb);

      return ROK;
   }

   bch = rgDBMGetBcchOnBch(cell);
   if ((bch) && (bch->lcId == datReq->lcId))
   {
      /* Store BCH data received in Scheduled slot */
      sf->bch.tb = datReq->pdu;
      return ROK;
   }
#endif/*RGR_SI_SCH*/
   UNUSED(inst);
   pcch = rgDBMGetPcch(cell);
   if ((pcch) && (pcch->lcId == datReq->lcId))
   {
      /* Store PCCH-DLSCH data received in Scheduled slot */
      sf->pcch.tb = datReq->pdu;
      return ROK;
   }

   /* Handle lcCb fetch failure */
   DU_LOG("\nERROR  -->  MAC : rgROMHndlBcchPcchDatReq(): Invalid Lc Id");
   err->errCause = RGERR_ROM_INV_LC_ID;

   return RFAILED;
} /* rgROMHndlBcchPcchDatReq */

/**
 * @brief Handler for StaRsp received on RGU for a dedicated logical channel.
 *
 * @details
 *
 *     Function : rgROMDedStaRsp
 *     
 *     This fucntion shall store the BO reported for the given logical
 *     channel.
 *     
 *           
 *  @param[in]  Inst        inst
 *  @param[in]  RgRguDedStaRsp *staRsp 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgROMDedStaRsp(Inst inst, RgRguDedStaRsp *staRsp)
{
   RgCellCb   *cell;

   /* Moving the error variables and assignments to available scope */

   /* Avoiding memset, as the variables of this are getting 
      initialized */


   if(((cell = rgCb[inst].cell) != NULLP)
      && (cell->cellId == staRsp->cellId))
   {
            Pst        schPst;
#ifdef UNUSED_VAR
    RgInfDedBoRpt boRpt
	 boRpt.cellSapId  = cell->schInstMap.cellSapId;
    boRpt.cellId  = staRsp->cellId;
	 boRpt.rnti    = staRsp->rnti; 
    boRpt.lcId    = staRsp->lcId; 
	 boRpt.bo      = staRsp->boReport.bo;
    boRpt.oldestSduArrTime = staRsp->boReport.oldestSduArrTime;
    boRpt.staPduBo = staRsp->boReport.staPduBo;
    boRpt.oldestSduArrTime = staRsp->boReport.oldestSduArrTime;
#endif
				rgGetPstToInst(&schPst,inst, cell->schInstMap.schInst);
            schPst.event = 0;
            //TODO: commented for compilation without SCH RgMacSchDedBoUpdt(&schPst, &boRpt);
            return ROK;
   }
   DU_LOG("\nERROR  -->  MAC : Invalid cell for CRNTI:%d LCID:%d ",
             staRsp->rnti,staRsp->lcId);

   return RFAILED;
}  /* rgROMDedStaRsp */

S16 RgMacSchBrdcmDedBoUpdtReq(
Inst inst,
CmLteCellId cellId,
CmLteRnti rnti, 
CmLteLcId lcId, 
S32 bo 
)
{

  RgCellCb   *cell;
  //if ((cell = rgDBMGetCellCb(cellId)) != NULLP)
  if (((cell = rgCb[inst].cell) != NULLP) &&
        (cell->cellId == cellId))
 {
     Pst        schPst;
#ifdef UNUSED_VAR
    RgInfDedBoRpt  boRpt;
    boRpt.cellSapId  = cell->schInstMap.cellSapId;
    boRpt.cellId     = cellId;
    boRpt.rnti       = rnti; 
    boRpt.lcId       = lcId; 
    boRpt.bo         = bo;
#endif
     rgGetPstToInst(&schPst,inst, cell->schInstMap.schInst);
     schPst.event = 0;
     //TODO: commented for compilation without SCH RgMacSchDedBoUpdtReq (&schPst,&boRpt);
  }
  return ROK;
}
/**
 * @brief Handler for StaRsp received on RGU for a common logical channel.
 *
 * @details
 *
 *     Function : rgROMCmnStaRsp
 *     
 *     This fucntion shall invoke rgROMHndlCcchStaRsp() for status response on
 *     CCCH and shall invoke rgROMHndlBcchPcchStaRsp() for status response on
 *     BCCH or PCCH.
 *     
 *           
 *  @param[in]  Inst        inst
 *  @param[in]  RgRguCmnStaRsp *staRsp 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgROMCmnStaRsp(Inst inst, RgRguCmnStaRsp *staRsp)
{
   RgCellCb   *cell;
   RgErrInfo  err;

   if(((cell = rgCb[inst].cell) == NULLP)
      || (cell->cellId != staRsp->cellId))
   {
      /* Handle Cell fetch failure */
      DU_LOG("\nERROR  -->  MAC : Invalid cell for CRNTI:%d LCID:%d",staRsp->u.rnti,staRsp->lcId);
      err.errType = RGERR_ROM_CMNSTARSP;
      err.errCause = RGERR_ROM_INV_CELL_ID;
      return RFAILED;
   }

   /* handle status response on CCCH */
   if(staRsp->lcId == cell->dlCcchId)
   {
      rgROMHndlCcchStaRsp(cell, staRsp, &err); 
   }
   else
   {
      rgROMHndlBcchPcchStaRsp(cell, staRsp, &err); 
   }
   
   return ROK;
}  /* rgROMCmnStaRsp */

#ifdef LTE_L2_MEAS

/**
 * @brief Handler for Request received on RGU for a UL Throughput measurement
 * enabled logical channel.
 *
 * @details
 *
 *     Function :rgROML2MUlThrpMeasReq 
 *     
 *     This function shall store the L2M UL Throughput Measurement status  information 
 *     for the given logical channel.
 *     
 *           
 *  @param[in]  Inst        inst
 *  @param[in]  RgRguL2MUlThrpMeasReq *measReq 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgROML2MUlThrpMeasReq(Inst inst, RgRguL2MUlThrpMeasReq *measReq)
{
   RgCellCb   *cell;
   RgUeCb     *ue;
   uint8_t    lcgId;
   uint8_t    loop;

   if(((cell = rgCb[inst].cell) != NULLP)
      &&(cell->cellId == measReq->cellId))
   {
      if ((ue = rgDBMGetUeCb(cell, measReq->rnti)) != NULLP)
      {
         for(loop=0; loop<measReq->numLcId;loop++)
         {
            if ((rgDBMGetUlDedLcCb(ue, measReq->lcId[loop])) != NULLP)
            {
               ue->ul.lcCb[measReq->lcId[loop]].measOn = measReq->enbMeas;
               if(ue->ul.lcCb[measReq->lcId[loop]].measOn == FALSE)
               {
                  lcgId=ue->ul.lcCb[measReq->lcId[loop]].lcgId;
                  ue->ul.lcgArr[lcgId].lcgBsInfo.outStndngBs = 0;
                  ue->ul.lcgArr[lcgId].lcgBsInfo.firstDatSegRcvd = FALSE;
               }
            }
         }
         return ROK;
      }
   }
   DU_LOG("\nERROR  -->  MAC : Invalid cell CRNTI:%d",
             measReq->rnti);
   return RFAILED;
}  /* rgROML2MUlThrpMeasReq */

#endif

/**
 * @brief Handler for StaRsp received on RGU for CCCH.
 *
 * @details
 *
 *     Function : rgROMHndlCcchStaRsp
 *     
 *     This function shall fetch the raCb with the given RNTI and ask RAM to
 *     update BO. 
 *     
 *           
 *  @param[in]  RgCellCb       *cell
 *  @param[in]  RgRguCmnStaRsp *staRsp
 *  @param[out] RgErrInfo      *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgROMHndlCcchStaRsp(RgCellCb *cell,RgRguCmnStaRsp *staRsp, RgErrInfo *err)
{
   Pst      schPst;
   Inst     macInst = cell->macInst - RG_INST_START;
   //RgInfCmnBoRpt boRpt;

#ifdef UNUSED_VAR
   RgInfCmnBoRpt boRpt;
   boRpt.cellSapId  = cell->schInstMap.cellSapId;
   boRpt.cellId  = staRsp->cellId;
   boRpt.u.rnti    = staRsp->u.rnti; 
   boRpt.lcId    = staRsp->lcId; 
   boRpt.lcType  = staRsp->lcType; 
   boRpt.bo      = staRsp->bo;
#endif
   rgGetPstToInst(&schPst,macInst, cell->schInstMap.schInst);
   //TODO: commented for compilation without SCH RgMacSchCmnBoUpdt(&schPst, &boRpt);

   return ROK;
} /* rgROMHndlCcchStaRsp */


/**
 * @brief Handler for StaRsp received on RGU for BCCH or PCCH.
 *
 * @details
 *
 *     Function : rgROMHndlBcchPcchStaRsp
 *     
 *     This function shall store the buffer and time to transmit in lcCb.
 *     
 *           
 *  @param[in]  RgCellCb       *cell
 *  @param[in]  RgRguCmnStaRsp *staRsp
 *  @param[out] RgErrInfo      *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgROMHndlBcchPcchStaRsp(RgCellCb *cell,RgRguCmnStaRsp *staRsp,RgErrInfo *err)
{
   Pst      schPst;
   //RgInfCmnBoRpt boRpt;
   Inst     macInst = cell->macInst - RG_INST_START;

   memset(&schPst, 0, sizeof(Pst));

   if (rgDBMChkCmnLcCb(cell, staRsp->lcId) != ROK)
   {
      /* Handle lcCb fetch failure */
      DU_LOG("\nERROR  -->  MAC : Invalid LCID:%d",staRsp->lcId);
      err->errCause = RGERR_ROM_INV_LC_ID;
      return RFAILED;
   }
   /* MS_WORKAROUND : This is to ensure that the queue for BCH is not filled with old BO requests :
         This assumes that BO is not received more than 4 frames in advance from the enodeb application */
   if (cell->bcchBchInfo.lcId == staRsp->lcId)
   {
      uint16_t nextBchSfn;

      nextBchSfn = (cell->crntTime.sfn + 4 - (cell->crntTime.sfn%4)) % RG_MAX_SFN;
      if ((staRsp->u.timeToTx.sfn != nextBchSfn) ||
         ((staRsp->u.timeToTx.sfn == cell->crntTime.sfn) && (cell->crntTime.slot >= 7)))
      {
        return ROK;
      }
   }
/*
   boRpt.cellSapId     = cell->schInstMap.cellSapId;
   boRpt.cellId     = staRsp->cellId;
   boRpt.u.timeToTx = staRsp->u.timeToTx; 
   boRpt.lcId       = staRsp->lcId; 
   boRpt.lcType     = staRsp->lcType; 
   boRpt.bo         = staRsp->bo;*/	
#ifdef EMTC_ENABLE
  if(cell->emtcEnable)
  {
     if(boRpt.lcType == CM_LTE_LCH_PCCH)
     {
        boRpt.emtcDIReason = staRsp->emtcDiReason;
        boRpt.pnb = staRsp->pnb;
     }
  }
#endif
   rgGetPstToInst(&schPst,macInst, cell->schInstMap.schInst);
   //TODO: commented for compilation without SCH RgMacSchCmnBoUpdt(&schPst, &boRpt);

   return ROK;
} /* rgROMHndlBcchPcchStaRsp */

/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
/**
 * @brief Handler for updating DL SF information on receiving
 *  DL dedicated data, CCCH, BCCH/PCCH data request.
 *
 * @details
 *
 *     Function : rgROMUpdDLSfRemDataCnt
 *     
 *           
 *  @param[in]  RgCellCb       *cell
 *  @param[in]  RgDlSf         *dlSf;
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgROMUpdDlSfRemDataCnt(RgCellCb *cellCb, RgDlSf *dlSf)
{
   RgErrInfo            err;
   //Inst                 inst = cellCb->macInst - RG_INST_START;

   if(!dlSf->remDatReqCnt)
   {
       /*This is an error scenario of RLC generating more data          
        * request than the allocation. Do nothing for this. */
      DU_LOG("\nERROR  -->  MAC : RX new data while remDatReqCnt is 0 for cell");
      return RFAILED;
   }

   /*Decrement the remaining data request to be received countter
     for this SF.
     Check if this was the last pending data request for this DL SF.*/
    /* Fix[ccpu00126310]: Tracks Data Requests from RLC for both loosely and tight coupled 
     RLC-MAC */
   if((0 == --dlSf->remDatReqCnt) && !(dlSf->txDone) &&
      (RG_TIMEINFO_SAME(cellCb->crntTime, dlSf->schdTime)) && (dlSf->statIndDone))
   {
      /*Check if we have already received a TTI for this Data,
        if that is the case then we need to send TFU Data request 
        to PHY */

      if (ROK != rgTOMUtlProcDlSf (dlSf, cellCb, &err))
      {
         DU_LOG("\nERROR  -->  MAC : Unable to process downlink slot for cell");
         err.errType = RGERR_ROM_DEDDATREQ;
      }

      /* Mark this frame as sent */
      dlSf->txDone = TRUE;
   }

   return ROK;
} /* rgROMUpdDlSfRemDataCnt*/
#endif

/**********************************************************************
 
         End of file
**********************************************************************/
