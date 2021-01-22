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
  
     Desc:     C source code for  L2 Measurements in MAC
  
     File:     rg_l2m.c
  
**********************************************************************/

/** @file rg_l2m.c
@brief This file implements the schedulers main access to MAC layer code.
*/

/* header include files -- defines (.h) */
#include "common_def.h"
#include "lrg.h"
#include "crg.h"        
#include "rgr.h"
#include "rgu.h"           
#include "tfu.h"
#include "rg_env.h"
#include "rg_err.h"
#include "rg_sch_inf.h"
#include "rg.h"

/* header/extern include files (.x) */
#include "rgu.x"           /* RGU types */
#include "tfu.x"           /* TFU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "crg.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* typedefs for Scheduler */
#include "rg_prg.x"        /* typedefs for PRG interface */
#include "du_app_mac_inf.h"
#include "rg.x"            /* MAC types */


/* local defines */
#ifdef LTE_L2_MEAS
static S16 rgL2mInsertMeasCb ARGS((
         RgCellCb       *cell,
         RgL2MeasCb     *measCb,
         RgInfL2MeasReq *measInfo ));

static RgL2MeasCb * rgL2mAllocMeasCb ARGS((
         RgCellCb       *cell,
         RgInfL2MeasReq *measInfo,
         RgErrInfo      *err));

/* Function definitions */

/** @brief This function creates the measCb
 *
 * @details
 *
 *     Function: rgL2mCreateMeasCb
 *         Processing steps:
 *         - Check the measType
 *         - Create measCb for every qci
 *
 * @param  [in] RgCellCb       *cell
 * @param  [in] RgInfL2MeasReq *measInfo
 * @param  [in] uint8_t             measType
 * @param  [out] RgErrInfo      *err
 * @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 */
S16 rgL2mCreateMeasCb 
(
RgCellCb       *cell,
RgInfL2MeasReq *measInfo, 
uint8_t        measType,
RgErrInfo      *err
)
{
  // Inst    inst = cell->macInst - RG_INST_START;
   uint32_t     idx;
   RgL2MeasCb   *measCb = NULLP;
   uint8_t      qciVal = 0;

   UNUSED(measType);
   UNUSED(err);

   if ((measCb = rgL2mAllocMeasCb(cell, measInfo, err)) == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : Allocation of RgL2MeasCb failed");
      return RFAILED;
   }
   //Memcpy is already done in rgL2mAllocMeasCb
   /*memcpy(&measCb->measReq, (const uint8_t *)measInfo,\
     sizeof(RgInfL2MeasReq));*/
   rgL2mInsertMeasCb(cell, measCb, measInfo);
   measCb->measReq.timePrd = measInfo->timePrd;

   if(measInfo->timePrd == 0)
   {
      cell->sndL2Meas = FALSE;
   }

   for(idx = 0; idx < measInfo->t.prbReq.numQci; idx++)
   {
      if(measInfo->timePrd == 0)
      {
         qciVal = measInfo->t.prbReq.qci[idx];
         cell->qciArray[qciVal].qci = qciVal;
      }
      cell->qciArray[measInfo->t.prbReq.qci[idx]].mask = TRUE;
   }
   return ROK;
} /* rgL2mCreateMeasCb */


/**
 * @brief Layer Manager Measurement request handler. 
 *
 * @details
 *
 *     Function : rgL2mMeasReq
 *     
 *     This function handles  measurement request received at MAC
 *     from the Scheduler.
 *     -# Measurement request will be stored in the list in descending order of
 *     there time period.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgInfL2MeasReq *measInfo, the measurement request structure
 *  @param[out] RgErrInfo   *err, error information
 *  @return  S16
 *      -# ROK
 **/
S16 rgL2mMeasReq 
(
RgCellCb       *cell,
RgInfL2MeasReq *measInfo,
RgErrInfo      *err
)
{
   S16  ret=RFAILED;

   /* Creaet MeasCb Insert in cell->l2mList and return*/
   if ( (ret = rgL2mCreateMeasCb(cell, measInfo,
               LRG_L2MEAS_AVG_PRB_PER_QCI_UL, err)) != ROK)
   {
      /* Clear Downlink MeasCb created Above If exists*/
      return (ret);
   }
   return ROK;
} /* rgL2mMeasReq */
/** @brief This function sends the measurement confirm
 *  from mac to scheduler
 *
 * @details
 *
 *     Function: rgSndL2MeasCfm
 *
 * @param  [in] RgCellCb          *cell
 * @param  [in] RgInfL2MeasCfm    *measCfm
 */
static Void rgSndL2MeasCfm(RgCellCb *cell, RgInfL2MeasCfm *measCfm)
{
   Pst             pst;
   Inst            macInst = cell->macInst - RG_INST_START;

   memset(&pst, 0, sizeof(Pst));
   rgGetPstToInst(&pst,macInst, cell->schInstMap.schInst);
   RgMacSchL2Meas(&pst, measCfm);

   return;
}/* rgSndL2MeasCfm */

/** @brief This function sends the measurement stop confirm
 *  from mac to scheduler
 *
 * @details
 *
 *     Function: rgSndL2MeasStopCfm
 *
 * @param  [in] RgCellCb          *cell
 * @param  [in] RgInfL2MeasCfm    *measCfm
 */
static Void rgSndL2MeasStopCfm(RgCellCb *cell,RgInfL2MeasCfm *measCfm)
{
   Pst             pst;
   Inst            macInst = cell->macInst - RG_INST_START;

   memset(&pst, 0, sizeof(Pst));
   rgGetPstToInst(&pst,macInst, cell->schInstMap.schInst);
   RgMacSchL2MeasStop(&pst, measCfm);

   return;
}/* rgSndL2MeasStopCfm */

/**
 * @brief  L2 Measurement request handler.This function shall be called by
 *  scheduler to calculate average PRB usage Per Qci in Uplink
 *
 * @details
 *
 *     Function : RgSchMacL2MeasReq
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgInfL2MeasReq *measInfo, L2 Measurement req structure
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 RgSchMacL2MeasReq
(
Pst               *pst,          /* post structure  */
RgInfL2MeasReq    *measInfo      /* Meas Req Info */
)
{
   Inst            inst;
   RgCellCb        *cellCb = NULLP;
   RgErrInfo       err;
   S16             ret = ROK;
   RgInfL2MeasCfm   measCfm;

   RG_IS_INST_VALID(pst->dstInst);
   inst   = pst->dstInst - RG_INST_START;
   cellCb = rgCb[inst].cell;
   /* Get the  cell using cellId */
   if ((cellCb == NULLP) ||
       (cellCb->cellId != measInfo->cellId))
   {
      DU_LOG("\nERROR  -->  MAC : unable to get the cellCb");
      return RFAILED;
   }
   /* Call L2M Function to store Meas req */
   ret = rgL2mMeasReq(cellCb, measInfo, &err);
   if (ret != ROK)
   {
      memset(&measCfm, 0, sizeof(RgInfL2MeasCfm));
      measCfm.transId     = measInfo->transId;
      measCfm.cellId      = measInfo->cellId;
      measCfm.measType    = measInfo->measType;
      measCfm.cfm.reason   = LCM_REASON_INVALID_PAR_VAL;
      measCfm.cfm.status  = LCM_PRIM_NOK;
      rgSndL2MeasCfm(cellCb, &measCfm);
      DU_LOG("\nERROR  -->  MAC : Meas req Failed  errType(%d) errCause(%d)",
               err.errType, err.errCause);
      return RFAILED;
   }
   return (ret);
} /* -- RgSchMacL2MeasReq-- */

/**
 * @brief  L2 Measurement request handler.This function shall be called by
 *         sch to to stop l2 measurement in MAC,
 *
 * @details
 *
 *     Function : RgSchMacL2MeasStopReq
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgInfL2MeasReq *measInfo, L2 Measurement req structure
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 RgSchMacL2MeasStopReq
(
Pst               *pst,          /* post structure  */
RgInfL2MeasStopReq *measInfo      /* Meas Req Info */
)
{
   S16            ret = ROK;   
   CmLList        *node   = NULLP;
   RgL2MeasCb     *measCb = NULLP;
   uint8_t        idx;
   uint8_t        qciVal;
   Inst           inst;
   RgCellCb       *cellCb = NULLP;
   RgInfL2MeasCfm  measCfm;

   RG_IS_INST_VALID(pst->dstInst);
   inst   = pst->dstInst - RG_INST_START;
   cellCb = rgCb[inst].cell;
      /* Get the  cell using cellId */
   if ((cellCb == NULLP) ||
       (cellCb->cellId != measInfo->cellId))
   {
      
      DU_LOG("\nERROR  -->  MAC : Unable to get the cellCb");
      return RFAILED;
   }
   node = cellCb->l2mList.first; 
   while(node != NULLP)
   {
      measCb = (RgL2MeasCb *)(node)->node;
      node = (node)->next;
      /*L2 Meas off for qci in cell */
      for(idx = 0; idx < measCb->measReq.t.prbReq.numQci; idx++)
      {
         qciVal = measCb->measReq.t.prbReq.qci[idx];
         cellCb->qciArray[qciVal].mask = FALSE;
      }
      cmLListDelFrm(&cellCb->l2mList, &measCb->measLnk);
      rgFreeSBuf(inst,(Data**)&measCb, sizeof(RgL2MeasCb));
   }
   memset(&measCfm, 0, sizeof(RgInfL2MeasCfm));
   measCfm.transId     = measInfo->transId;
   measCfm.cellId      = measInfo->cellId;
   measCfm.measType    = measInfo->measType;
   measCfm.cfm.status  = LCM_PRIM_OK;
   rgSndL2MeasStopCfm(cellCb, &measCfm);
   return (ret);
} /* -- RgSchMacL2MeasStopReq-- */

/**
 * @brief  L2 Measurement request handler.This function shall be called by
 *  scheduler for  sending L2 meas 
 *
 * @details
 *
 *     Function : RgSchMacL2MeasSendReq
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgInfL2MeasReq *measInfo, L2 Measurement req structure
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 RgSchMacL2MeasSendReq
(
Pst               *pst,          /* post structure  */
RgInfL2MeasSndReq *measInfo      /* Meas Req Info */
)
{
   Inst            inst;
   RgCellCb       *cellCb = NULLP;
   S16             ret    = ROK;

   RG_IS_INST_VALID(pst->dstInst);
   inst   = pst->dstInst - RG_INST_START;
   cellCb = rgCb[inst].cell;
      /* Get the  cell using cellId */
   if ((cellCb == NULLP) ||
       (cellCb->cellId != measInfo->cellId))
   {
      
      DU_LOG("\nERROR  -->  MAC : Unable to get the cellCb");
      return RFAILED;
   }
   /*set sndL2Meas as applicatoin sent l2 meas info request*/
   cellCb->sndL2Meas = TRUE;

   return (ret);
}/*RgSchMacL2MeasSendReq*/ 

/** @brief This function inserts the MeasCb in to data base
 *
 * @details
 *
 *     Function: rgL2mInsertMeasCb
 *
 * @param  [in] RgCellCb       *cell
 * @param  [in] RgL2MeasCb     *measCb
 * @param  [in] RgInfMeasReq   *measInfo
 * @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
static S16 rgL2mInsertMeasCb
(
RgCellCb       *cell,
RgL2MeasCb     *measCb,
RgInfL2MeasReq *measInfo
)
{
   CmLList    *lnk, *node;
   RgL2MeasCb *oldMeasCb;
   uint16_t   diffTime;

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
      oldMeasCb = (RgL2MeasCb *)lnk->node;
      diffTime = (oldMeasCb->measReq.timePrd - 
            (RG_CALC_SF_DIFF(cell->crntTime, oldMeasCb->startTime)));
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
} /* rgL2mInsertMeasCb */

/** @brief This function allocates memory from the heap
 *
 * @details
 *
 *     Function: rgL2mAllocMeasCb
 *
 * @param  [in] RgCellCb       *cell
 * @param  [in] RgInfL2MeasReq *measInfo
 * @param  [out] RgErrInfo      *err
 * @return  RgSchL2MeasCb *
 */
static RgL2MeasCb * rgL2mAllocMeasCb
(
RgCellCb       *cell,
RgInfL2MeasReq *measInfo,
RgErrInfo      *err
)
{
   RgL2MeasCb       *measCb = NULLP;
   Inst             inst = cell->macInst - RG_INST_START;


      if((rgAllocSBuf(inst,(Data **)&(measCb),
                  sizeof(RgL2MeasCb))) == RFAILED)
      {
         DU_LOG("\nERROR  -->  MAC : Allocation of RgL2MeasCb failed");
         err->errType  = RGERR_L2M_MEASREQ;
         err->errCause = RGERR_RAM_MEM_EXHAUST;
         return (NULLP);
      }
   memcpy(&measCb->measReq, measInfo, sizeof(RgInfL2MeasReq));
   RGCPYTIMEINFO(cell->crntTime, measCb->startTime);

   return (measCb);
} /* rgL2mAllocMeasCb */


/**
 * @brief This function calculates the measurement for measType 
 * LRG_L2MEAS_AVG_PRB_PER_QCI_UL and send the end result to the 
 * MAC Scheduler.
 *
 * @details
 *
 *  Function : rgL2Meas
 *     
 *  @param[in] RgCellCb  *cell
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgL2Meas(RgCellCb  *cell)
{
   CmLList         *node   = NULLP;
   RgL2MeasCb      *measCb = NULLP;
   RgInfL2MeasCfm   measCfm;
   uint8_t          idx = 0;
   uint8_t          qciVal = 0;
   uint32_t         measPrd; /*LTE_L2_MEAS_PHASE2*/
   CmLteTimingInfo  crntTime;
   Inst             inst = cell->macInst - RG_INST_START;
   
   node = cell->l2mList.first;

   while(node != NULLP)
   {
      measCb = (RgL2MeasCb *)node->node;
      node = node->next;
      crntTime = cell->crntTime;

      if(cell->crntTime.sfn == 0 && (cell->crntTime.slot % RG_NUM_SUB_FRAMES) == 0)
      {
         measCb->sfnCycle++;
      }

      measPrd = RG_CALC_SFN_SF_DIFF(cell->crntTime, 
                        measCb->sfnCycle, measCb->startTime);
      
      /*LTE_L2_MEAS_PHASE2*/
      if (cell->sndL2Meas || measPrd == measCb->measReq.timePrd)
      {
         memset(&measCfm, 0, sizeof(RgInfL2MeasCfm));
         for(idx = 0; idx < measCb->measReq.t.prbReq.numQci; idx++)
         {
            qciVal = measCb->measReq.t.prbReq.qci[idx];
            measCfm.u.prbCfm.prbUsage[idx].qciValue = qciVal;

            measCfm.transId  = measCb->measReq.transId;
            measCfm.measType = measCb->measReq.measType;
            measCfm.cellId    = measCb->measReq.cellId;

            measCfm.u.prbCfm.prbUsage[idx].prbUsage = 
               cell->qciArray[qciVal].prbCount;

            cell->qciArray[qciVal].prbCount = 0;
            measCfm.u.prbCfm.numQci++;
            if(measCb->measReq.timePrd > 0)
            {
               cell->qciArray[qciVal].mask = FALSE;
            }
         }
         rgSndL2MeasCfm(cell, &measCfm);

         if(measCb->measReq.timePrd > 0) 
         {
            cmLListDelFrm(&cell->l2mList, &measCb->measLnk);
            rgFreeSBuf(inst,(Data**)&measCb, sizeof(RgL2MeasCb));
         }
         else /*if meas period is 0 then do not delette meascb , just reset l2 cntrs value to 0*/
         { 
            measCb->startTime = crntTime;
            measCb->measReq.timePrd = 0;
            cell->sndL2Meas = FALSE;
         }
         continue;
      } 
   }
   return ROK;
} /* rgL2MEas */

#endif /* LTE_L2_MEAS */
/**********************************************************************
 
         End of file
**********************************************************************/
