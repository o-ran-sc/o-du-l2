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
  
     Desc:     C source code for L2 Measurement fucntions
  
     File:     rg_sch_l2m.c
  
**********************************************************************/

/** @file rg_sch_l2m.c
@brief This file implements the L2 Measurement feature code.
*/

/* header include files -- defines (.h) */
#include "common_def.h"
#include "lrg.h"
#include "rgr.h"
#include "rgm.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch.h"
#include "rg_sch_cmn.h"
#include "rg_sch_inf.h"         /* typedefs for Scheduler */
#include "rg_sch_err.h"

/* header/extern include files (.x) */
#include "tfu.x"           /* TFU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */
#include "rg_sch_cmn.x"    /* typedefs for Scheduler */
/* local defines */
uint32_t dlPrbCnt;
#ifdef LTE_L2_MEAS


static S16 rgSchL2mInsertMeasCb ARGS((
         RgSchCellCb       *cell,
         RgSchL2MeasCb     *measCb,
         LrgSchMeasReqInfo *measInfo ));

static RgSchL2MeasCb * rgSchL2mAllocMeasCb ARGS((
         RgSchCellCb       *cell,
         LrgSchMeasReqInfo *measInfo,
         RgSchErrInfo      err));

/* Function definitions */

/** @brief This function fills the L2 measurement confirm structure
 *
 * @details
 *
 *     Function: rgSchFillL2MeasCfm
 *
 * @param  [in] RgSchCellCb *cell 
 * @param  [in] RgSchL2MeasCb *measCb
 * @param  [out] LrgSchMeasCfmInfo *measCfm
 * @param  [in] measTime
 * @return  Void
 */
S16 rgSchFillL2MeasCfm
(
RgSchCellCb       *cell,
RgSchL2MeasCb     *measCb,
LrgSchMeasCfmInfo *cfm,
uint32_t          measTime   
)
{
   uint8_t            idx;
   LrgSchMeasReqInfo  *measInfo;
   uint8_t            qciVal = 0;
   uint32_t           sampOc = 0;

   measInfo = &measCb->measReq;   

   cfm->hdr.transId  = measInfo->hdr.transId;
   cfm->measType     = measInfo->measType;
   cfm->cellId       = measInfo->cellId;
   cfm->cfm.status   = LCM_PRIM_OK;
   if((measCb->measReq.measType & LRG_L2MEAS_AVG_PRB_DL) &&
                                    (measCb->dlTotalBw))
   {
       cfm->avgPrbDl.prbPerc = ((cell->avgPrbDl.prbCount * 100) /
                                 measCb->dlTotalBw);
       /* Resetting the prbCount to 0,  fix for ccpu00125002 */
       cell->avgPrbDl.prbCount = 0;
   }
   if((measCb->measReq.measType & LRG_L2MEAS_AVG_PRB_UL) &&
                                    (measCb->ulTotalBw))
   {
       cfm->avgPrbUl.prbPerc = ((cell->avgPrbUl.prbCount * 100) /
                                 measCb->ulTotalBw);
       /* Resetting the prbCount to 0,  fix for ccpu00125002 */
       cell->avgPrbUl.prbCount = 0;
   }
   if((measCb->measReq.measType & LRG_L2MEAS_AVG_PRB_PER_QCI_DL) &&
                                    (measCb->dlTotalBw))
   {
       cfm->avgPrbQciDlCfm.numQci = measCb->measReq.avgPrbQciDl.numQci;
       for(idx = 0; idx < measCb->measReq.avgPrbQciDl.numQci; idx++)
       {
          qciVal = measCb->measReq.avgPrbQciDl.qci[idx];
          cfm->avgPrbQciDlCfm.prbPercQci[idx].prbPercQci = 
          ((cell->qciArray[qciVal].dlPrbCount * 100) /
            measCb->dlTotalBw);
            cfm->avgPrbQciDlCfm.prbPercQci[idx].qciValue = qciVal;
          cell->qciArray[qciVal].dlPrbCount = 0;
       }
   }
   if((measCb->measReq.measType & LRG_L2MEAS_AVG_PRB_PER_QCI_UL) && 
                                    (measCb->ulTotalBw))
   {
       cfm->avgPrbQciUlCfm.numQci = measCb->measReq.avgPrbQciUl.numQci;
       for(idx = 0; idx < measCb->measReq.avgPrbQciUl.numQci; idx++)
       {
          cfm->avgPrbQciUlCfm.prbPercQci[idx].qciValue = 
                                 measCb->avgPrbQciUl.prbUsage[idx].qciValue;

          if(measCb->avgPrbQciUl.prbUsage[idx].prbUsage > measCb->ulTotalBw)
          {
             measCb->avgPrbQciUl.prbUsage[idx].prbUsage = measCb->ulTotalBw;
          }   

          cfm->avgPrbQciUlCfm.prbPercQci[idx].prbPercQci = 
             ((measCb->avgPrbQciUl.prbUsage[idx].prbUsage * 100) /
               measCb->ulTotalBw);
       }
   }
   if(measCb->measReq.measType & LRG_L2MEAS_RA_PREAMBLE)
   {
       cfm->raPrmbsCfm.dedPreambles        = cell->raPrmbs.dedPream;
       cfm->raPrmbsCfm.randSelPreLowRange  = cell->raPrmbs.preamGrpA;
       cfm->raPrmbsCfm.randSelPreHighRange = cell->raPrmbs.preamGrpB;
       cell->raPrmbs.dedPream              = 0;
       cell->raPrmbs.preamGrpA             = 0;
       cell->raPrmbs.preamGrpB             = 0;
   }
   if(measCb->measReq.measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL)
   {
       cfm->numUeQciDlCfm.numQci = measInfo->nmbActvUeQciDl.numQci;
       sampOc = (measTime / measInfo->nmbActvUeQciDl.sampPrd);
       
       if(sampOc)
       {   
          if (measCb->measReq.nmbActvUeQciDl.numQci)
          {
             for(idx = 0; idx < measCb->measReq.nmbActvUeQciDl.numQci; idx++)
             {
                qciVal = measCb->measReq.nmbActvUeQciDl.qci[idx];
                /* L2_COUNTERS */
                cell->qciArray[qciVal].dlTotal_UeCount +=
                   cell->qciArray[qciVal].dlUeCount;
                cfm->numUeQciDlCfm.numActvUeQci[idx].numActvUeQci =
                   cell->qciArray[qciVal].dlTotal_UeCount / sampOc;
                cfm->numUeQciDlCfm.numActvUeQci[idx].qciValue = qciVal;
                
                DU_LOG("\nDEBUG  -->  SCH : L2_MEAS:CFM DL QCI %u TOTAL Count %lu Active UE %d ",
                      qciVal,cell->qciArray[qciVal].dlTotal_UeCount,
                      cfm->numUeQciDlCfm.numActvUeQci[idx].numActvUeQci);
                
                cell->qciArray[qciVal].dlTotal_UeCount = 0;
             }
          }
          else
          {
             idx = 0;
             for(qciVal = 1; qciVal < LRG_MAX_QCI_PER_REQ; qciVal++)
             {
                /* L2_COUNTERS */
                cell->qciArray[qciVal].dlTotal_UeCount +=
                   cell->qciArray[qciVal].dlUeCount;
                if (cell->qciArray[qciVal].dlTotal_UeCount)
                {
                   cfm->numUeQciDlCfm.numActvUeQci[idx].numActvUeQci =
                      cell->qciArray[qciVal].dlTotal_UeCount / sampOc;
                   cfm->numUeQciDlCfm.numActvUeQci[idx].qciValue = qciVal;

                   DU_LOG("\nDEBUG  -->  SCH : L2_MEAS:CFM DL QCI %u TOTAL Count %lu Active UE %d ",
                         qciVal,cell->qciArray[qciVal].dlTotal_UeCount,
                         cfm->numUeQciDlCfm.numActvUeQci[idx].numActvUeQci);

                   cell->qciArray[qciVal].dlTotal_UeCount = 0;
                   idx++;
                }
             }
             cfm->numUeQciDlCfm.numQci = idx;
          }
       }
   }
   if(measCb->measReq.measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL)
   {
       cfm->numUeQciUlCfm.numQci = measInfo->nmbActvUeQciUl.numQci;
       sampOc = (measTime / measInfo->nmbActvUeQciUl.sampPrd);
       
       
       if(sampOc)
       {   
          if (measCb->measReq.nmbActvUeQciUl.numQci)
          {
             for(idx = 0; idx < measCb->measReq.nmbActvUeQciUl.numQci; idx++)
             {
                cell->qciArray[qciVal].ulTotal_UeCount +=
                   cell->qciArray[qciVal].ulUeCount;
                qciVal = measCb->measReq.nmbActvUeQciUl.qci[idx]; 
                cfm->numUeQciUlCfm.numActvUeQci[idx].numActvUeQci =
                   cell->qciArray[qciVal].ulTotal_UeCount/ sampOc;
                cfm->numUeQciUlCfm.numActvUeQci[idx].qciValue = qciVal;
                
                DU_LOG("\nDEBUG  -->  SCH : L2_MEAS:CFM UL QCI %d TOTAL Count %lu Active UE %d ",
                      qciVal,cell->qciArray[qciVal].ulTotal_UeCount,
                      cfm->numUeQciUlCfm.numActvUeQci[idx].numActvUeQci);
                
                cell->qciArray[qciVal].ulTotal_UeCount = 0;
             }
          }
          else
          {
             idx = 0;
             for(qciVal = 1; qciVal < LRG_MAX_QCI_PER_REQ; qciVal++)
             {
                cell->qciArray[qciVal].ulTotal_UeCount +=
                   cell->qciArray[qciVal].ulUeCount;
                if (cell->qciArray[qciVal].ulTotal_UeCount)
                {
                   cfm->numUeQciUlCfm.numActvUeQci[idx].numActvUeQci =
                      cell->qciArray[qciVal].ulTotal_UeCount/ sampOc;
                   cfm->numUeQciUlCfm.numActvUeQci[idx].qciValue = qciVal;

                   DU_LOG("\nDEBUG  -->  SCH : L2_MEAS:CFM UL QCI %d TOTAL Count %lu Active UE %d ",
                         qciVal,cell->qciArray[qciVal].ulTotal_UeCount,
                         cfm->numUeQciUlCfm.numActvUeQci[idx].numActvUeQci);

                   cell->qciArray[qciVal].ulTotal_UeCount = 0;
                   idx++;
                }
             }
             cfm->numUeQciUlCfm.numQci = idx;
          }
       }
   }
   if(measCb->measReq.measType & LRG_L2MEAS_TB_TRANS_DL_COUNT)
   {
      cfm->tbTransDlTotalCnt = cell->dlUlTbCnt.tbTransDlTotalCnt;
      cell->dlUlTbCnt.tbTransDlTotalCnt = 0;
   }   
   if(measCb->measReq.measType & LRG_L2MEAS_TB_TRANS_DL_FAULTY_COUNT)
   {
      cfm->tbTransDlFaulty = cell->dlUlTbCnt.tbTransDlFaulty;
      cell->dlUlTbCnt.tbTransDlFaulty = 0;
   }   
   if(measCb->measReq.measType & LRG_L2MEAS_TB_TRANS_UL_COUNT)
   {
      cfm->tbTransUlTotalCnt = cell->dlUlTbCnt.tbTransUlTotalCnt;
      cell->dlUlTbCnt.tbTransUlTotalCnt = 0;
   }   
   if(measCb->measReq.measType & LRG_L2MEAS_TB_TRANS_UL_FAULTY_COUNT)
   {
      cfm->tbTransUlFaulty = cell->dlUlTbCnt.tbTransUlFaulty;
      cell->dlUlTbCnt.tbTransUlFaulty = 0;
   }

   measCb->dlTotalBw = 0;
   measCb->ulTotalBw = 0;

   return ROK;
} /* rgSchFillL2MeasCfm */

/** @brief This function sends the L2 measurement confirm to LM 
 * from Shceduler
 *
 * @details
 *
 *     Function: rgSchL2mSndCfm
 *
 * @param  [in] Pst   *pst
 * @param  [in] RgSchL2MeasCb *measCb
 * @param  [in] Bool     isErr
 * @return  Void
 */
S16 rgSchL2mSndCfm
(
Pst               *pst,
RgSchL2MeasCb     *measCb,
LrgSchMeasReqInfo *measInfo,
Bool              isErr
)
{
   LrgSchMeasCfmInfo   cfm;

   memset(&cfm, 0, sizeof(LrgSchMeasCfmInfo));
   cfm.hdr.transId  = measInfo->hdr.transId;
   cfm.measType     = measInfo->measType;
   cfm.cellId       = measInfo->cellId;
   cfm.cfm.status   = LCM_PRIM_OK;
   if(isErr == TRUE)
   {
      cfm.cfm.status   = LCM_PRIM_NOK;
      cfm.cfm.reason   = LCM_REASON_INVALID_PAR_VAL;
      RgMiLrgSchL2MeasCfm(pst, &cfm);
      return ROK;
   }
   return ROK;
} /* rgSchL2mSndCfm */

/** @brief This function fills the LM confirmation pst structure 
 *
 * @details
 *
 *     Function: rgSchL2mFillCfmPst
 *
 * @param  [in] Pst    *pst
 * @param  [out] Pst    *cfmPst
 * @param  [in] LrgSchMeasReqInfo *measInfo
 * @return  Void
 */
Void rgSchL2mFillCfmPst
(
Pst    *pst,
Pst    *cfmPst,
LrgSchMeasReqInfo *measInfo 
)
{
 
   cfmPst->srcEnt    = pst->dstEnt;
   cfmPst->srcInst   = pst->dstInst;
   cfmPst->srcProcId = pst->dstProcId;
   cfmPst->dstEnt    = pst->srcEnt;
   cfmPst->dstInst   = pst->srcInst;
   cfmPst->dstProcId = pst->srcProcId;

   cfmPst->selector  = measInfo->hdr.response.selector;
   cfmPst->prior     = measInfo->hdr.response.prior;
   cfmPst->route     = measInfo->hdr.response.route;
   cfmPst->region    = measInfo->hdr.response.mem.region;
   cfmPst->pool      = measInfo->hdr.response.mem.pool;

   return;
} /* rgSchL2mFillCfmPst */

/** @brief This function inserts the MeasCb in to data base
 *
 * @details
 *
 *     Function: rgSchL2mInsertMeasCb
 *
 * @param  [in] RgSchCellCb       *cell
 * @param  [in] RgSchL2MeasCb     *measCb
 * @param  [in] LrgSchMeasReqInfo *measInfo
 * @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
static S16 rgSchL2mInsertMeasCb
(
RgSchCellCb       *cell,
RgSchL2MeasCb     *measCb,
LrgSchMeasReqInfo *measInfo
)
{
   CmLList   *lnk, *node;
   RgSchL2MeasCb   *oldMeasCb;
   uint32_t        diffTime;
   
   /* 
    * 1. Check if l2mList has any entries.
    * 2. If yes 
    *       1. Take the first entrie's time period and find the diff with
    *       cell->crntTime.
    *       2. If the diff is > measInfo->timePeriod then insert before this
    *       entry.
    *       3. Else take the next entry in list
    *       4. If reached without adding to list . Append at the end of list.
    * 3. If no entries in l2mList add at the first.
    */
   lnk = cell->l2mList.first;
   node = &measCb->measLnk;
   node->node = (PTR)measCb;
   while(lnk != NULLP )
   {
      oldMeasCb = (RgSchL2MeasCb *)lnk->node;
      diffTime = (oldMeasCb->measReq.timePrd - 
                 (RGSCH_CALC_SF_DIFF(cell->crntTime, oldMeasCb->startTime)));
      if (diffTime > measInfo->timePrd)
      {
         cell->l2mList.crnt = lnk;
         cmLListInsCrnt(&(cell->l2mList), node);
         return ROK;
      }
      else
      {
         lnk = lnk->next;
      }
   }  /* End of While */
   cmLListAdd2Tail(&(cell->l2mList), node);
   return ROK;
} /* rgSchL2mInsertMeasCb */

/** @brief This function calculates the Down link prb count 
 * for a DlSf
 *
 * @details
 *
 *     Function: rgSchL2CalDlPrbCount
 *
 * @param  [in] RgSchCellCb       *cell
 */
static Void rgSchL2CalDlPrbCount(RgSchCellCb *cell)
{
   CmLteTimingInfo    frm;
   RgSchDlSf          *sf = NULLP;
#ifdef LTE_TDD
   uint8_t            idx;
#endif

   frm   = cell->crntTime;
   RGSCH_INCR_SUB_FRAME(frm, RG_SCH_CMN_DL_DELTA);
   sf = rgSCHUtlSubFrmGet(cell, frm);
#ifdef LTE_TDD
   idx = (cell->crntTime.slot + RG_SCH_CMN_DL_DELTA) % 
                  RGSCH_NUM_SUB_FRAMES;
   if(RG_SCH_CMN_CHK_DL_DATA_ALLOWED(cell, idx)) 
   {
      cell->avgPrbDl.prbCount += sf->bwAssigned;
      dlPrbCnt += sf->bwAssigned;
   }
#else
   cell->avgPrbDl.prbCount += sf->bwAssigned;
#endif
   return;
}

/** @brief This function calculates the up link prb count 
 * for a UlSf
 *
 * @details
 *
 *     Function: rgSchL2CalUlPrbCount
 *
 * @param  [in] RgSchCellCb       *cell
 */
static Void rgSchL2CalUlPrbCount(RgSchCellCb  *cell)
{
   RgSchUlSf        *sf = NULLP;
   RgSchCmnUlCell   *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
#ifdef LTE_TDD
   uint8_t  idx;
#endif

#ifdef LTE_TDD
   idx = cellUl->schdIdx;
   if(idx < cellUl->numUlSubfrms)
   {
      sf = &cellUl->ulSfArr[idx];
      cell->avgPrbUl.prbCount += sf->totPrb;
   }
#else
   sf = &cellUl->ulSfArr[cellUl->schdIdx];
   cell->avgPrbUl.prbCount += sf->totPrb;
#endif
   return;
}
/** @brief This function allocates memory from the heap
 *
 * @details
 *
 *     Function: rgSchL2mAllocMeasCb
 *
 * @param  [in] RgSchCellCb       *cell
 * @param  [in] RgSchL2MeasCb     *measInfo
 * @param  [out] RgSchErrInfo      *err
 * @return  RgSchL2MeasCb *
 */
static RgSchL2MeasCb * rgSchL2mAllocMeasCb
(
RgSchCellCb       *cell,
LrgSchMeasReqInfo *measInfo,
RgSchErrInfo      err
)
{
   RgSchL2MeasCb       *measCb = NULLP;
   Inst                inst = cell->instIdx;
   UNUSED(err);

   if((rgSCHUtlAllocSBuf(inst, (Data **)&measCb,
                   sizeof(RgSchL2MeasCb))) == RFAILED)
   {
      DU_LOG("\nERROR  -->  SCH : Allocation of RgSchL2MeasCb failed");
      return (NULLP);
   }
   memcpy(&measCb->measReq, measInfo, sizeof(LrgSchMeasReqInfo));
   RGSCHCPYTIMEINFO(cell->crntTime, measCb->startTime);

   measCb->dlTotalBw = 0;
   measCb->ulTotalBw = 0;

   return (measCb);
} /* rgSchL2mAllocMeasCb */

/**
 * @brief Layer Manager Measurement request handler. 
 *
 * @details
 *
 *     Function : rgSchL2mMeasReq
 *     
 *     This function handles  measurement request received at scheduler instance
 *     from the Layer Manager.
 *     -# Measurement request will be stored in the list in ascending order of
 *     their time period.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  LrgSchMeasReqInfo *measInfo, the measurement request structure
 *  @param[out] RgSchErrInfo   *err, error information
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSchL2mMeasReq 
(
RgSchCellCb       *cell,
LrgSchMeasReqInfo *measInfo,
RgSchErrInfo      err
)
{
   RgSchL2MeasCb *measCb;
   uint8_t       idx;
   uint8_t       qciVal;


   qciVal = 0;
   if ((measCb = rgSchL2mAllocMeasCb(cell, measInfo, err)) == NULLP)
   {
       RGSCHFILLERR(err, RGSCHERR_L2M_MEASREQ,
                    RGSCHERR_SCH_ALLOC_FAILED);
       DU_LOG("\nERROR  -->  SCH : Allocation of RgSchL2MeasCb failed");
       return RFAILED;
   }
   /*memcpy(&measCb->measReq, measInfo,\
             sizeof(LrgSchMeasReqInfo));*/
   rgSchL2mInsertMeasCb(cell, measCb, measInfo);
  
   if (measInfo->timePrd == 0)
   {
      cell->sndL2Meas = FALSE;
      if (measInfo->measType & LRG_L2MEAS_AVG_PRB_PER_QCI_DL)
      {
         for (idx = 0; idx < measInfo->avgPrbQciDl.numQci; idx++)
         {
            qciVal = measInfo->avgPrbQciDl.qci[idx];
            cell->qciArray[qciVal].qci = qciVal;
         }
      }
      if (measInfo->measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL)
      {
         for (idx = 0; idx < measInfo->nmbActvUeQciDl.numQci; idx++)
         {
            qciVal = measInfo->nmbActvUeQciDl.qci[idx];
            cell->qciArray[qciVal].qci = qciVal;
         }
      }
      if (measInfo->measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL)
      {
         for (idx = 0; idx < measInfo->nmbActvUeQciUl.numQci; idx++)
         {
            qciVal = measInfo->nmbActvUeQciUl.qci[idx];
            cell->qciArray[qciVal].qci = qciVal;
         }
      }
   }
   /* Here post the message to MAC */
   if(measInfo->measType & LRG_L2MEAS_AVG_PRB_PER_QCI_UL)
   {
      RgInfL2MeasReq    measReq;
      Pst               pst;
      memset(&measReq, 0, sizeof(RgInfL2MeasReq));
      measReq.transId  = measInfo->hdr.transId;
      measReq.measType = measInfo->measType;
      measReq.timePrd  = measInfo->timePrd;
      measReq.cellId   = measInfo->cellId;
      measReq.t.prbReq.numQci = measInfo->avgPrbQciUl.numQci;
      for (idx = 0; idx < measInfo->avgPrbQciUl.numQci; idx++)
      {
        measReq.t.prbReq.qci[idx] = measInfo->avgPrbQciUl.qci[idx]; 
      }
      /* Send measReq to MAC */
      rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], cell->macInst);
      RgSchMacL2Meas(&pst, &measReq);
   }
   return ROK;
} /* rgSchL2mMeasReq */

/**
 * @brief This function calculates the measurement for differnt measurement type
 * and send the end result to the layer manager
 *
 * @details
 *
 *     Function : rgSCHL2Meas
 *     
 *  @param[in] RgSchCellCb  *cell
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgSCHL2Meas
(
RgSchCellCb  *cell,
uint8_t isCalrCrcInd
)
{
   CmLList           *node = NULLP;
   RgSchL2MeasCb     *measCb = NULLP;
   uint8_t           idx;
   LrgSchMeasCfmInfo measCfm;
   uint8_t           qciVal = 0;
   uint32_t          sfDiff;
   uint32_t          meas;
#ifdef LTE_TDD
   uint8_t           sfIdx;
   Bool              isDlDataAllowed;  
   uint8_t           rem;
   uint32_t          numDlSf;
   uint32_t          numUlSf;
#endif

   node = cell->l2mList.first;
   memset(&measCfm, 0, sizeof(LrgSchMeasCfmInfo));
   while(node != NULLP)
   {
      measCb = (RgSchL2MeasCb *)node->node;
      node = node->next;
      if(cell->crntTime.sfn == 1023 && cell->crntTime.slot == 9)  
      {
         /*calculates diff between crnt time and start time*/
         meas = RGSCH_CALC_SFN_SF_DIFF(cell->crntTime, 
               measCb->sfnCycle, measCb->startTime);
         measCb->sfnCycle++;
      }
      else
      {
         /*calculates diff between crnt time and start time*/
         meas = RGSCH_CALC_SFN_SF_DIFF(cell->crntTime, 
               measCb->sfnCycle, measCb->startTime);
      }

      if (cell->sndL2Meas || meas == measCb->measReq.timePrd)
      {
#ifdef LTE_TDD
         rem = meas % RGSCH_NUM_SUB_FRAMES;
         /* Get the total number of DL and UL subframes within the reporting period*/
         numDlSf = (meas / RGSCH_NUM_SUB_FRAMES) * rgSchTddNumDlSubfrmTbl[cell->ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];
         numUlSf = (meas / RGSCH_NUM_SUB_FRAMES) * rgSchTddNumUlSubfrmTbl[cell->ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];

         sfIdx = (measCb->startTime.slot + 1) % RGSCH_NUM_SUB_FRAMES;

         while(rem)
         {
            isDlDataAllowed = RG_SCH_CMN_CHK_DL_DATA_ALLOWED(cell, sfIdx); 
            if(isDlDataAllowed)
            {
               numDlSf++;
            }   
            else if(rgSchTddUlDlSubfrmTbl[cell->ulDlCfgIdx][sfIdx] == 
                  RG_SCH_TDD_UL_SUBFRAME)
            {
               numUlSf++;
            }     
            sfIdx = (sfIdx + 1) % RGSCH_NUM_SUB_FRAMES;
            rem--;
         }   

         measCb->dlTotalBw = numDlSf * cell->bwCfg.dlTotalBw;
         measCb->ulTotalBw = numUlSf * cell->bwCfg.ulTotalBw;

#else            
         measCb->dlTotalBw = meas * cell->bwCfg.dlTotalBw;
         measCb->ulTotalBw = meas * cell->bwCfg.ulTotalBw;
#endif            
         if((measCb->measReq.measType & LRG_L2MEAS_AVG_PRB_PER_QCI_UL))
         {
            if(measCb->cfmRcvd)
            {
               rgSchFillL2MeasCfm(cell, measCb, &measCfm, meas);
            }
            else
            {
               continue;
            }
         }
         else
         {
            rgSchFillL2MeasCfm(cell, measCb, &measCfm, meas);
         }
         RgMiLrgSchL2MeasCfm(&(rgSchCb[cell->instIdx].rgSchInit.lmPst),
               &measCfm);
         memset(&measCfm, 0, sizeof(LrgSchMeasCfmInfo));
         
         /* Delete this measCb from the list */
         if(measCb->measReq.timePrd > 0)
         {
            cmLListDelFrm(&cell->l2mList, &measCb->measLnk);
            rgSCHUtlFreeSBuf(cell->instIdx, (Data **)&measCb,
                  sizeof(RgSchL2MeasCb));
         }
         else/*do not delete measCb, will use for next measurement*/
         {
            measCb->startTime = cell->crntTime;
            measCb->sfnCycle = 0;
            measCb->cfmRcvd = FALSE;
            memset(&measCb->avgPrbQciUl, 0, sizeof(LrgAvgPrbQCICfm));
            cell->sndL2Meas = FALSE;
         } 
         /* ccpu00117052 - MOD - Passing double pointer
            for proper NULLP assignment*/
      }
      else
      {
         /* Just update the AVERAGE UL PRB counter here and return
          * if the caller is CRCIndication() and the UL scheduling happens
          * as a part of it*/
#ifdef RG_ULSCHED_AT_CRC 
         if(isCalrCrcInd)
         {
            if(measCb->measReq.measType & LRG_L2MEAS_AVG_PRB_UL)
            {
               rgSchL2CalUlPrbCount(cell);
            }
            continue;
         }
#else
         /* UL PRB counter gets updated as a part of CRC indication 
          * if the UL scheduling happens there */
         if((measCb->measReq.measType & LRG_L2MEAS_AVG_PRB_UL))
         {
            rgSchL2CalUlPrbCount(cell);
         }
#endif
         if((measCb->measReq.measType & LRG_L2MEAS_AVG_PRB_DL))
         {
            rgSchL2CalDlPrbCount(cell);
         }
         if(measCb->measReq.measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL)
         {
            sfDiff = RGSCH_CALC_SF_DIFF(cell->crntTime, measCb->startTime);

            if((sfDiff % measCb->measReq.nmbActvUeQciDl.sampPrd) == 0)
            {
               if (measCb->measReq.nmbActvUeQciDl.numQci)
               {
                  for (idx = 0; idx < measCb->measReq.nmbActvUeQciDl.numQci; 
                        idx++)
                  {
                     qciVal = measCb->measReq.nmbActvUeQciDl.qci[idx];
                     cell->qciArray[qciVal].dlTotal_UeCount +=
                        cell->qciArray[qciVal].dlUeCount;
                  }
               }
               else
               {
                  for (qciVal = 1; qciVal < LRG_MAX_QCI_PER_REQ; qciVal++)
                  {
                     cell->qciArray[qciVal].dlTotal_UeCount +=
                        cell->qciArray[qciVal].dlUeCount;
                  }
               }
            }
         }
         if(measCb->measReq.measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL)
         {
            sfDiff = RGSCH_CALC_SF_DIFF(cell->crntTime , measCb->startTime);
            if((sfDiff % measCb->measReq.nmbActvUeQciUl.sampPrd) == 0)
            {
               if (measCb->measReq.nmbActvUeQciUl.numQci)
               {
                  for (idx = 0; idx < measCb->measReq.nmbActvUeQciUl.numQci; 
                        idx++)
                  {
                     qciVal = measCb->measReq.nmbActvUeQciUl.qci[idx];
                     cell->qciArray[qciVal].ulTotal_UeCount += 
                        cell->qciArray[qciVal].ulUeCount;
                  }
               }
               else
               {
                  for (qciVal = 1; qciVal < LRG_MAX_QCI_PER_REQ; qciVal++)
                  {
                     cell->qciArray[qciVal].ulTotal_UeCount += 
                        cell->qciArray[qciVal].ulUeCount;
                  }
               }
            }
         }
      }
   }/* end of while */
   return ROK;
} /* rgSCHL2MEas */
#endif /* LTE_L2_MEAS */
/**********************************************************************
 
         End of file
**********************************************************************/
