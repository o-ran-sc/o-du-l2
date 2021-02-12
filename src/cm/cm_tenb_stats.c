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

/**********************************************************************
 
    Name:  
 
    Type:   C include file
 
    Desc:   
 
    File:  cm_tenb_stats.c
 
**********************************************************************/
 
/** @file cm_tenb_stats.c
@brief This file contains 
*/

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*
*/
/* header include files (.h) */
  
#include "envopt.h"             /* Environment options */  
#include "envdep.h"             /* Environment dependent */
#include "envind.h"             /* Environment independent */
#include "gen.h"                /* General */
#include "ssi.h"                /* System services interface */
#include "cm_lte.h"             /* Common LTE header file */
#include "cm_llist.h"           /* Linked list header file */
#include "cm_tkns.h"            /* Common tokens header file */
#include "du_log.h"

/* header/extern include files (.x) */

#include "gen.x"                /* General */
#include "ssi.x"                /* System services interface */
#include "cm_lib.x"             /* Common lib header file */
#include "cm_llist.x"           /* Linked list header file */
#include "cm_tkns.x"            /* Common tokens header file */
#include "cm_lte.x"             /* Common LTE header file */

#include "ss_queue.h"
#include "ss_queue.x"
#include "ss_task.x"
#include "ss_msg.x"
#include "cm_tenb_stats.x"    /* Total EnodeB Stats declarations */

#ifdef TENB_STATS
static Void TSInfUtlPackUeInfo ARGS((
 Buffer            *mBuf,
 TSInfL2UeStats     *stats 
));
static Void TSInfUtlPackCellInfo ARGS((
 Buffer              *mBuf,
 TSInfL2CellStats     *stats 
));
static Void TSInfUtlUnpkUeInfo ARGS((
 Buffer            *mBuf,
 TSInfL2UeStats     *stats 
));
static Void TSInfUtlUnpkCellInfo ARGS((
 Buffer              *mBuf,
 TSInfL2CellStats     *stats
));
static Buffer* TSInfUtlAllocMsg ARGS((
 Pst   *pst
));
 
/*
*
*       Fun:   TSInfTrigL2Stats 
*
*       Desc:  Indicate PDCP UL to initiate Stats procedure 
*
*       Ret:   
*
*       Notes: None
*
*
*/
static Void TSInfTrigL2Stats(Region region,Pool pool)
{
   Buffer* pBuf;
   Pst pst = {0};


   SGetMsg(region, pool, &pBuf);
//#if defined(SCH_STATS) || defined(TENB_STATS)
   pst.dstEnt = STATS_DSTENT;
   pst.dstInst = STATS_DSTINST;
   pst.selector = CL_DSTENT_SELECTOR;

   pst.srcProcId = STATS_SRCPROCID;
   pst.dstProcId = STATS_DSTPROCID;
   pst.srcEnt = STATS_SRCENT;
   pst.event = TENBSTATSINIT; 
   SPstTsk(&pst, pBuf);

   return;
}

/*
*
*       Fun:   TSInfTrigStats
*
*       Desc:  Trigger Sending L2 Stats
*
*       Ret:   
*
*       Notes: None
*
*
*/
Void TSInfTrigStats(Region region,Pool pool)
{
//TODO
   TSInfTrigL2Stats(region, pool);

   return;
}

         
static Buffer* TSInfUtlAllocMsg(Pst   *pst)
{
   Buffer *mBuf; 


   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      DU_LOG("\nERROR  -->  RLC : MBuf Allocation failed\n");
   }
   return (mBuf);
}

static Void TSInfUtlPackUeInfo(Buffer *mBuf,TSInfL2UeStats *stats)
{
   S32 i;
   uint32_t k;


   CMCHKPK(oduUnpackUInt32, stats->persistent.numDeactivation, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->persistent.numActivation, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->persistent.activatedSCells, mBuf);

   CMCHKPK(oduUnpackUInt32, stats->nonPersistent.rlc.dlMaxWindowSz, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->nonPersistent.rlc.dlMaxPktsInSduQ, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->nonPersistent.rlc.ulReOdrTmrExpCnt, mBuf);

   CMCHKPK(oduUnpackUInt32, stats->nonPersistent.pdcp.dlFlowCtrlDropCnt, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->nonPersistent.pdcp.dlPdcpAckWaitDropCnt, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->nonPersistent.pdcp.dlPdcpDropCnt, mBuf);


   for (k = 0; k < L2_STATS_MAX_CELLS; k++)
   {
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].ulTpt, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].ulNumiTbs, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].ulSumiTbs, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].ulNumCqi, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].ulSumCqi, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].ulDtxCnt, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].ulNackCnt, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].ulAckNackCnt, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].ulPrbUsg, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].ulRetxOccns, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].ulTxOccns, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].dlTpt, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].dlBo, mBuf);
      for (i=0; i<4; i++)
      {
         CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].riCnt[i], mBuf);
      }
      for (i=0; i<5; i++)
      {
         CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].dlPdbLvl[i], mBuf);
      }
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].cqiDropCnt, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].dlNumCw1iTbs, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].dlSumCw1iTbs, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].dlNumCw0iTbs, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].dlSumCw0iTbs, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].dlNumCw1Cqi, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].dlSumCw1Cqi, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].dlNumCw0Cqi, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].dlSumCw0Cqi, mBuf);
      for (i=0; i<2; i++)
      {
         CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].dlDtxCnt[i], mBuf);
      }
      for (i=0; i<2; i++)
      {
         CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].dlAckNackCnt[i], mBuf);
      }
      for (i=0; i<2; i++)
      {
         CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].dlNackCnt[i], mBuf);
      }
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].dlPrbUsg, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].dlRetxOccns, mBuf);
      CMCHKPK(oduUnpackUInt32, stats->nonPersistent.sch[k].dlTxOccns, mBuf);
   }
   CMCHKPK(oduUnpackUInt32, stats->rnti, mBuf);
   return;
}

static Void TSInfUtlPackCellInfo(Buffer *mBuf,TSInfL2CellStats *stats  )
{
   S32 i,j;


   CMCHKPK(oduUnpackUInt32, stats->rlc.reOdrTmrExp, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->rlc.maxRlcDrbRetxFail, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->rlc.maxRlcSrbRetxFail, mBuf);

   CMCHKPK(oduUnpackUInt32, stats->sch.ulNumiTbs, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.ulSumiTbs, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.ulNumCqi, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.ulSumCqi, mBuf);
#ifdef RG_5GTF
   CMCHKPK(oduUnpackUInt32, stats->sch.ul5gtfRbAllocFail, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.ul5gtfPdcchSend, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.ul5gtfUeFnlzReAdd, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.ul5gtfUeRmvFnlzZeroBo, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.ul5gtfUeRbAllocDone, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.ul5gtfAllocAllocated, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.ul5gtfPdcchSchd, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.ul5gtfUeSchPick, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.ul5gtfBsrRecv, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.ul5gtfSrRecv, mBuf);

   CMCHKPK(oduUnpackUInt32, stats->sch.dl5gtfPdschCons, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.dl5gtfPdcchSend, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.dl5gtfBoUpd, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.dl5gtfFnlzFail, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.dl5gtfFnlzPass, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.dl5gtfRbAllocFail, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.dl5gtfRbAllocPass, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.dl5gtfUePick, mBuf);
#endif
   CMCHKPK(oduUnpackUInt32, stats->sch.cntTtiProcTime500, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.cntTtiProcTime600, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.cntTtiProcTime700, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.cntTtiProcTime800, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.cntTtiProcTime900, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.cntTtiProcTime1000, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.avgTtiProcTime, mBuf);
   for (i=9; i>=0; i--)
   {
      CMCHKPK(oduUnpackUInt32, stats->sch.dlPdbRatio[i], mBuf);
   }
   for (i=9; i>=0; i--)
   {
      CMCHKPK(oduUnpackUInt32, stats->sch.ulPrbUsage[i], mBuf);
   }
   for (i=9; i>=0; i--)
   {
      CMCHKPK(oduUnpackUInt32, stats->sch.dlPrbUsage[i], mBuf);
   }
   for (i=3; i>=0; i--)
   {
      CMCHKPK(oduUnpackUInt32, stats->sch.ulDtx[i], mBuf);
   }
   for (i=3; i>=0; i--)
   {
      CMCHKPK(oduUnpackUInt32, stats->sch.ulAckNack[i], mBuf);
   }
   for (i=3; i>=0; i--)
   {
      CMCHKPK(oduUnpackUInt32, stats->sch.ulNack[i], mBuf);
   }
   for (i=1; i>=0; i--)
   {
      for (j=3; j>=0; j--)
      {
         CMCHKPK(oduUnpackUInt32, stats->sch.dlDtx[i][j], mBuf);
      }
   }
   for (i=1; i>=0; i--)
   {
      for (j=3; j>=0; j--)
      {
         CMCHKPK(oduUnpackUInt32, stats->sch.dlAckNack[i][j], mBuf);
      }
   }
   for (i=1; i>=0; i--)
   {
      for (j=3; j>=0; j--)
      {
         CMCHKPK(oduUnpackUInt32, stats->sch.dlNack[i][j], mBuf);
      }
   }
   for (i=3; i>=0; i--)
   {
      CMCHKPK(oduUnpackUInt32, stats->sch.riCnt[i], mBuf);
   }
   CMCHKPK(oduUnpackUInt32, stats->sch.dlNumCw1iTbs, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.dlSumCw1iTbs, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.dlNumCw0iTbs, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.dlSumCw0iTbs, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.dlNumCw1Cqi, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.dlSumCw1Cqi, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.dlNumCw0Cqi, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.dlSumCw0Cqi, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.msg3Fail, mBuf);
   CMCHKPK(oduUnpackUInt32, stats->sch.msg4Fail, mBuf);

   CMCHKPK(oduUnpackUInt32, stats->cellId, mBuf);

   return;
}

static Void TSInfUtlUnpkUeInfo(Buffer *mBuf, TSInfL2UeStats *stats )
{
   S32 i;
   uint32_t k;


   CMCHKUNPK(oduPackUInt32, &stats->rnti, mBuf);

   for (k = L2_STATS_MAX_CELLS; k > 0; k--)
   {
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].dlTxOccns, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].dlRetxOccns, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].dlPrbUsg, mBuf);
      for (i=1; i>=0; i--)
      {
         CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].dlNackCnt[i], mBuf);
      }
      for (i=1; i>=0; i--)
      {
         CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].dlAckNackCnt[i], mBuf);
      }
      for (i=1; i>=0; i--)
      {
         CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].dlDtxCnt[i], mBuf);
      }
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].dlSumCw0Cqi, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].dlNumCw0Cqi, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].dlSumCw1Cqi, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].dlNumCw1Cqi, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].dlSumCw0iTbs, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].dlNumCw0iTbs, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].dlSumCw1iTbs, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].dlNumCw1iTbs, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].cqiDropCnt, mBuf);
      for (i=4; i>=0; i--)
      {
         CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].dlPdbLvl[i], mBuf);
      }
      for (i=3; i>=0; i--)
      {
         CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].riCnt[i], mBuf);
      }
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].dlBo, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].dlTpt, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].ulTxOccns, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].ulRetxOccns, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].ulPrbUsg, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].ulAckNackCnt, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].ulNackCnt, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].ulDtxCnt, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].ulSumCqi, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].ulNumCqi, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].ulSumiTbs, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].ulNumiTbs, mBuf);
      CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.sch[k-1].ulTpt, mBuf);
   }

   CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.pdcp.dlPdcpDropCnt, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.pdcp.dlPdcpAckWaitDropCnt, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.pdcp.dlFlowCtrlDropCnt, mBuf);

   CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.rlc.ulReOdrTmrExpCnt, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.rlc.dlMaxPktsInSduQ, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->nonPersistent.rlc.dlMaxWindowSz, mBuf);

   CMCHKUNPK(oduPackUInt32, &stats->persistent.activatedSCells, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->persistent.numActivation, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->persistent.numDeactivation, mBuf);

   return;
}

static Void TSInfUtlUnpkCellInfo(Buffer *mBuf,TSInfL2CellStats *stats )
{
   S32 i,j;


      CMCHKUNPK(oduPackUInt32, &stats->cellId, mBuf);

   CMCHKUNPK(oduPackUInt32, &stats->sch.msg4Fail, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.msg3Fail, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.dlSumCw0Cqi, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.dlNumCw0Cqi, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.dlSumCw1Cqi, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.dlNumCw1Cqi, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.dlSumCw0iTbs, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.dlNumCw0iTbs, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.dlSumCw1iTbs, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.dlNumCw1iTbs, mBuf);
   for (i=0; i<4; i++)
   {
      CMCHKUNPK(oduPackUInt32, &stats->sch.riCnt[i], mBuf);
   }
   for (i=0; i<2; i++)
   {
      for (j=0; j<4; j++)
      {
         CMCHKUNPK(oduPackUInt32, &stats->sch.dlNack[i][j], mBuf);
      }
   }
   for (i=0; i<2; i++)
   {
      for (j=0; j<4; j++)
      {
         CMCHKUNPK(oduPackUInt32, &stats->sch.dlAckNack[i][j], mBuf);
      }
   }
   for (i=0; i<2; i++)
   {
      for (j=0; j<4; j++)
      {
         CMCHKUNPK(oduPackUInt32, &stats->sch.dlDtx[i][j], mBuf);
      }
   }
   for (i=0; i<4; i++)
   {
      CMCHKUNPK(oduPackUInt32, &stats->sch.ulNack[i], mBuf);
   }
   for (i=0; i<4; i++)
   {
      CMCHKUNPK(oduPackUInt32, &stats->sch.ulAckNack[i], mBuf);
   }
   for (i=0; i<4; i++)
   {
      CMCHKUNPK(oduPackUInt32, &stats->sch.ulDtx[i], mBuf);
   }
   for (i=0; i<10; i++)
   {
      CMCHKUNPK(oduPackUInt32, &stats->sch.dlPrbUsage[i], mBuf);
   }
   for (i=0; i<10; i++)
   {
      CMCHKUNPK(oduPackUInt32, &stats->sch.ulPrbUsage[i], mBuf);
   }
   for (i=0; i<10; i++)
   {
      CMCHKUNPK(oduPackUInt32, &stats->sch.dlPdbRatio[i], mBuf);
   }
   CMCHKUNPK(oduPackUInt32, &stats->sch.avgTtiProcTime, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.cntTtiProcTime1000, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.cntTtiProcTime900, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.cntTtiProcTime800, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.cntTtiProcTime700, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.cntTtiProcTime600, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.cntTtiProcTime500, mBuf);
#ifdef RG_5GTF
   CMCHKUNPK(oduPackUInt32, &stats->sch.dl5gtfUePick, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.dl5gtfRbAllocPass, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.dl5gtfRbAllocFail, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.dl5gtfFnlzPass, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.dl5gtfFnlzFail, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.dl5gtfBoUpd, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.dl5gtfPdcchSend, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.dl5gtfPdschCons, mBuf);

   CMCHKUNPK(oduPackUInt32, &stats->sch.ul5gtfSrRecv, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.ul5gtfBsrRecv, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.ul5gtfUeSchPick, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.ul5gtfPdcchSchd, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.ul5gtfAllocAllocated, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.ul5gtfUeRbAllocDone, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.ul5gtfUeRmvFnlzZeroBo, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.ul5gtfUeFnlzReAdd, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.ul5gtfPdcchSend, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.ul5gtfRbAllocFail, mBuf);
#endif
   CMCHKUNPK(oduPackUInt32, &stats->sch.ulSumCqi, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.ulNumCqi, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.ulSumiTbs, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->sch.ulNumiTbs, mBuf);

   CMCHKUNPK(oduPackUInt32, &stats->rlc.maxRlcSrbRetxFail, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->rlc.maxRlcDrbRetxFail, mBuf);
   CMCHKUNPK(oduPackUInt32, &stats->rlc.reOdrTmrExp, mBuf);

   return;
}

Void TSInfPkSndL2UeStats(Pst  *pst,SuId suId,TSInfL2UeStats *stats)
{
   Buffer *mBuf;


   mBuf = TSInfUtlAllocMsg(pst);
   TSInfUtlPackUeInfo(mBuf, stats);
   SPkS16(suId, mBuf);
   pst->event = (Event) EVTTENBL2UESTATS;
   SPstTsk(pst, mBuf);
   return;
}

Void TSInfUnpkL2UeStats(TSInfL2UeStatsInd func, Pst  *pst,Buffer *mBuf)
{
   SuId              suId;
   TSInfL2UeStats     stats; 

   SUnpkS16(&suId, mBuf);
   TSInfUtlUnpkUeInfo(mBuf, &stats);
   SPutMsg(mBuf);
   (*func)(pst, suId, &stats);
   return;
}

Void TSInfPkSndL2CellStats(Pst *pst,SuId suId,TSInfL2CellStats   *stats )
{
   Buffer *mBuf;

   mBuf = TSInfUtlAllocMsg(pst);
   TSInfUtlPackCellInfo(mBuf, stats);
   SPkS16(suId, mBuf);
   pst->event = (Event) EVTTENBL2CELLSTATS;
   SPstTsk(pst, mBuf);
   return;
}

Void TSInfUnpkL2CellStats(TSInfL2CellStatsInd func,Pst  *pst,Buffer *mBuf)
{
   SuId                suId;
   TSInfL2CellStats    stats; 


   SUnpkS16(&suId, mBuf);
   TSInfUtlUnpkCellInfo(mBuf, &stats);
   SPutMsg(mBuf);
   (*func)(pst, suId, &stats);
   return;
}
#endif /* TENB_STATS */

/**********************************************************************
         End of file
 **********************************************************************/
