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

#ifdef LTE_L2_MEAS
static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_FILE_ID=183;
static int RLOG_MODULE_ID=4096;
#endif 

/* local defines */
#ifdef LTE_L2_MEAS
PRIVATE S16 rgL2mInsertMeasCb ARGS((
         RgCellCb       *cell,
         RgL2MeasCb     *measCb,
         RgInfL2MeasReq *measInfo ));

PRIVATE RgL2MeasCb * rgL2mAllocMeasCb ARGS((
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
 * @param  [in] U8             measType
 * @param  [out] RgErrInfo      *err
 * @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 */
#ifdef ANSI
PUBLIC S16 rgL2mCreateMeasCb 
(
RgCellCb       *cell,
RgInfL2MeasReq *measInfo, 
U8              measType,
RgErrInfo      *err
)
#else
PUBLIC S16 rgL2mCreateMeasCb(cell, measInfo, measType, err)
RgCellCb       *cell;
RgInfL2MeasReq *measInfo; 
U8              measType;
RgErrInfo      *err;
#endif    
{
  // Inst    inst = cell->macInst - RG_INST_START;
   U32     idx;
   RgL2MeasCb       *measCb = NULLP;
   U8            qciVal = 0;

   UNUSED(measType);
   UNUSED(err);

   TRC3(rgL2mCreateMeasCb)

      if ((measCb = rgL2mAllocMeasCb(cell, measInfo, err)) == NULLP)
      {
	 RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Allocation of RgL2MeasCb failed");
	 RETVALUE(RFAILED);
      }
   //Memcpy is already done in rgL2mAllocMeasCb
   /*cmMemcpy((U8 *)&measCb->measReq, (CONSTANT U8 *)measInfo,\
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
   RETVALUE(ROK);
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
#ifdef ANSI
   PUBLIC S16 rgL2mMeasReq 
(
 RgCellCb       *cell,
 RgInfL2MeasReq *measInfo,
 RgErrInfo      *err
 )
#else
PUBLIC S16 rgL2mMeasReq(cell, measInfo, err)
   RgCellCb       *cell;
   RgInfL2MeasReq *measInfo; 
   RgErrInfo      *err;
#endif    
{
   S16  ret=RFAILED;

   TRC3(rgL2mMeasReq)
      /* Creaet MeasCb Insert in cell->l2mList and return*/
      if ( (ret = rgL2mCreateMeasCb(cell, measInfo,
		  LRG_L2MEAS_AVG_PRB_PER_QCI_UL, err)) != ROK)
      {
	 /* Clear Downlink MeasCb created Above If exists*/
	 RETVALUE(ret);
      }
   RETVALUE(ROK);
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
#ifdef ANSI
   PRIVATE Void rgSndL2MeasCfm
(
 RgCellCb          *cell, 
 RgInfL2MeasCfm    *measCfm
 )
#else
PRIVATE Void rgSndL2MeasCfm (cell, measCfm)
   RgCellCb          *cell; 
   RgInfL2MeasCfm    *measCfm;   
#endif
{
   Pst             pst;
   Inst            macInst = cell->macInst - RG_INST_START;
   TRC3(rgSndL2MeasCfm)

      cmMemset((U8 *)&pst, 0, sizeof(Pst));
   rgGetPstToInst(&pst,macInst, cell->schInstMap.schInst);
   RgMacSchL2Meas(&pst, measCfm);

   RETVOID;
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
#ifdef ANSI
   PRIVATE Void rgSndL2MeasStopCfm
(
 RgCellCb          *cell,
 RgInfL2MeasCfm    *measCfm
 )
#else
PRIVATE Void rgSndL2MeasStopCfm (cell, measCfm)
   RgCellCb          *cell;
   RgInfL2MeasCfm    *measCfm;
#endif
{
   Pst             pst;
   Inst            macInst = cell->macInst - RG_INST_START;

   TRC3(rgSndL2MeasStopCfm)
      cmMemset((U8 *)&pst, 0, sizeof(Pst));
   rgGetPstToInst(&pst,macInst, cell->schInstMap.schInst);
   RgMacSchL2MeasStop(&pst, measCfm);

   RETVOID;
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
#ifdef ANSI
   PUBLIC S16 RgSchMacL2MeasReq
(
 Pst               *pst,          /* post structure  */
 RgInfL2MeasReq    *measInfo      /* Meas Req Info */
 )
#else
PUBLIC S16 RgSchMacL2MeasReq(pst, measInfo)
   Pst               *pst;          /* post structure  */
   RgInfL2MeasReq    *measInfo;      /* Meas Req Info */
#endif    
{
   Inst            inst;
   RgCellCb        *cellCb = NULLP;
   RgErrInfo       err;
   S16             ret = ROK;
   RgInfL2MeasCfm   measCfm;

   TRC3(RgSchMacL2MeasReq)


      RG_IS_INST_VALID(pst->dstInst);
   inst   = pst->dstInst - RG_INST_START;
   cellCb = rgCb[inst].cell;
   /* Get the  cell using cellId */
   if ((cellCb == NULLP) ||
	 (cellCb->cellId != measInfo->cellId))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,measInfo->cellId,"unable to get the cellCb");
      RETVALUE(RFAILED);
   }
   /* Call L2M Function to store Meas req */
   ret = rgL2mMeasReq(cellCb, measInfo, &err);
   if (ret != ROK)
   {
      cmMemset((U8 *)&measCfm, 0, sizeof(RgInfL2MeasCfm));
      measCfm.transId     = measInfo->transId;
      measCfm.cellId      = measInfo->cellId;
      measCfm.measType    = measInfo->measType;
      measCfm.cfm.reason   = LCM_REASON_INVALID_PAR_VAL;
      measCfm.cfm.status  = LCM_PRIM_NOK;
      rgSndL2MeasCfm(cellCb, &measCfm);
      RLOG_ARG2(L_ERROR,DBG_CELLID,measInfo->cellId,
	    "Meas req Failed  errType(%d) errCause(%d)",
	    err.errType, err.errCause);
      RETVALUE(RFAILED);
   }
   RETVALUE(ret);
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
#ifdef ANSI
   PUBLIC S16 RgSchMacL2MeasStopReq
(
 Pst               *pst,          /* post structure  */
 RgInfL2MeasStopReq *measInfo      /* Meas Req Info */
 )
#else
PUBLIC S16 RgSchMacL2MeasStopReq(pst, measInfo)
   Pst               *pst;          /* post structure  */
   RgInfL2MeasStopReq *measInfo;      /* Meas Req Info */
#endif
{
   S16             ret = ROK;   
   CmLList         *node   = NULLP;
   RgL2MeasCb      *measCb = NULLP;
   U8               idx;
   U8               qciVal;
   Inst             inst;
   RgCellCb        *cellCb = NULLP;

   RgInfL2MeasCfm  measCfm;

   TRC3(RgSchMacL2MeasStopReq)


      RG_IS_INST_VALID(pst->dstInst);
   inst   = pst->dstInst - RG_INST_START;
   cellCb = rgCb[inst].cell;
   /* Get the  cell using cellId */
   if ((cellCb == NULLP) ||
	 (cellCb->cellId != measInfo->cellId))
   {

      RLOG_ARG0(L_ERROR,DBG_CELLID,measInfo->cellId,
	    "Unable to get the cellCb");
      RETVALUE(RFAILED);
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
   cmMemset((U8 *)&measCfm, 0, sizeof(RgInfL2MeasCfm));
   measCfm.transId     = measInfo->transId;
   measCfm.cellId      = measInfo->cellId;
   measCfm.measType    = measInfo->measType;
   measCfm.cfm.status  = LCM_PRIM_OK;
   rgSndL2MeasStopCfm(cellCb, &measCfm);
   RETVALUE(ret);
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
#ifdef ANSI
   PUBLIC S16 RgSchMacL2MeasSendReq
(
 Pst               *pst,          /* post structure  */
 RgInfL2MeasSndReq *measInfo      /* Meas Req Info */
 )
#else
PUBLIC S16 RgSchMacL2MeasSendReq(pst, measInfo)
   Pst               *pst;          /* post structure  */
   RgInfL2MeasSndReq *measInfo;      /* Meas Req Info */
#endif
{
   Inst            inst;
   RgCellCb       *cellCb = NULLP;
   S16             ret    = ROK;

   TRC3(RgSchMacL2MeasSendReq)


      RG_IS_INST_VALID(pst->dstInst);
   inst   = pst->dstInst - RG_INST_START;
   cellCb = rgCb[inst].cell;
   /* Get the  cell using cellId */
   if ((cellCb == NULLP) ||
	 (cellCb->cellId != measInfo->cellId))
   {

      RLOG_ARG0(L_ERROR,DBG_CELLID,measInfo->cellId,
	    "Unable to get the cellCb");
      RETVALUE(RFAILED);
   }
   /*set sndL2Meas as applicatoin sent l2 meas info request*/
   cellCb->sndL2Meas = TRUE;

   RETVALUE(ret);
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
#ifdef ANSI
   PRIVATE S16 rgL2mInsertMeasCb
(
 RgCellCb       *cell,
 RgL2MeasCb     *measCb,
 RgInfL2MeasReq *measInfo
 )
#else
PRIVATE S16 rgL2mInsertMeasCb(cell, measCb, measInfo)
   RgCellCb       *cell;
   RgL2MeasCb     *measCb;
   RgInfL2MeasReq *measInfo;
#endif
{
   CmLList   *lnk, *node;
   RgL2MeasCb  *oldMeasCb;
   U16         diffTime;

   TRC3(rgL2mInsertMeasCb)

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
	 RETVALUE(ROK);
      }
      else
      {
	 lnk = lnk->next;
      }
   }  /* End of While */

   cmLListAdd2Tail(&(cell->l2mList), node);
   RETVALUE(ROK);
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
#ifdef ANSI
   PRIVATE RgL2MeasCb * rgL2mAllocMeasCb
(
 RgCellCb       *cell,
 RgInfL2MeasReq *measInfo,
 RgErrInfo      *err
 )
#else
PRIVATE RgL2MeasCb * rgL2mAllocMeasCb(cell, measInfo, err)
   RgCellCb       *cell;
   RgInfL2MeasReq *measInfo;
   RgErrInfo      *err;
#endif
{
   RgL2MeasCb       *measCb = NULLP;
   Inst             inst = cell->macInst - RG_INST_START;

   TRC3(rgL2mAllocMeasCb)

      if((rgAllocSBuf(inst,(Data **)&(measCb),
		  sizeof(RgL2MeasCb))) == RFAILED)
      {
	 RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
	       "Allocation of RgL2MeasCb failed");
	 err->errType  = RGERR_L2M_MEASREQ;
	 err->errCause = RGERR_RAM_MEM_EXHAUST;
	 RETVALUE(NULLP);
      }
   cmMemcpy((U8 *)&measCb->measReq, (U8 *)measInfo, sizeof(RgInfL2MeasReq));
   RGCPYTIMEINFO(cell->crntTime, measCb->startTime);

   RETVALUE(measCb);
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
#ifdef ANSI
   PUBLIC S16 rgL2Meas
(
 RgCellCb  *cell
 )
#else
PUBLIC S16 rgL2Meas(cell)
   RgCellCb  *cell;
#endif
{
   CmLList         *node   = NULLP;
   RgL2MeasCb      *measCb = NULLP;
   RgInfL2MeasCfm  measCfm;
   U8              idx = 0;
   U8              qciVal = 0;
   U32             measPrd; /*LTE_L2_MEAS_PHASE2*/
   CmLteTimingInfo  crntTime;
   Inst           inst = cell->macInst - RG_INST_START;

   TRC3(rgL2Meas)

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
	 cmMemset((U8 *)&measCfm, 0, sizeof(RgInfL2MeasCfm));
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
   RETVALUE(ROK);
} /* rgL2MEas */

#endif /* LTE_L2_MEAS */
/**********************************************************************

  End of file
 **********************************************************************/
