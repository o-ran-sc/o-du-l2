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
PRIVATE Void TSInfUtlPackUeInfo ARGS((
 Buffer            *mBuf,
 TSInfL2UeStats     *stats 
));
PRIVATE Void TSInfUtlPackCellInfo ARGS((
 Buffer              *mBuf,
 TSInfL2CellStats     *stats 
));
PRIVATE Void TSInfUtlUnpkUeInfo ARGS((
 Buffer            *mBuf,
 TSInfL2UeStats     *stats 
));
PRIVATE Void TSInfUtlUnpkCellInfo ARGS((
 Buffer              *mBuf,
 TSInfL2CellStats     *stats
));
PRIVATE Buffer* TSInfUtlAllocMsg ARGS((
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
#ifdef ANSI
PRIVATE Void TSInfTrigL2Stats
(
 Region    region,
 Pool      pool
)
#else
PRIVATE Void TSInfTrigL2Stats(region, pool)
 Region    region;
 Pool      pool;
#endif
{
   Buffer* pBuf;
   Pst pst = {0};

   TRC2(TSInfTrigL2Stats)

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

   RETVOID;
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
#ifdef ANSI
PUBLIC Void TSInfTrigStats
(
 Region    region,
 Pool      pool
)
#else
PUBLIC Void TSInfTrigStats(region, pool)
 Region    region;
 Pool      pool;
#endif
{
   TRC2(TSInfTrigStats)
//TODO
   TSInfTrigL2Stats(region, pool);

   RETVOID;
}

         
#ifdef ANSI
PRIVATE Buffer* TSInfUtlAllocMsg
(
 Pst   *pst
)
#else
PRIVATE Buffer* TSInfUtlAllocMsg(pst)
 Pst   *pst;
#endif
{
   Buffer *mBuf; 

   TRC2(TSInfUtlAllocMsg)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      printf("\n MBuf Allocation failed\n");
   }
   RETVALUE(mBuf);
}

#ifdef ANSI
PRIVATE Void TSInfUtlPackUeInfo
(
 Buffer            *mBuf,
 TSInfL2UeStats     *stats
)
#else
PRIVATE Void TSInfUtlPackUeInfo(mBuf, stats)
 Buffer            *mBuf;
 TSInfL2UeStats     *stats; 
#endif
{
   S32 i;
   U32 k;

   TRC2(TSInfUtlPackUeInfo)

   CMCHKPK(packUint32, stats->persistent.numDeactivation, mBuf);
   CMCHKPK(packUint32, stats->persistent.numActivation, mBuf);
   CMCHKPK(packUint32, stats->persistent.activatedSCells, mBuf);

   CMCHKPK(packUint32, stats->nonPersistent.rlc.dlMaxWindowSz, mBuf);
   CMCHKPK(packUint32, stats->nonPersistent.rlc.dlMaxPktsInSduQ, mBuf);
   CMCHKPK(packUint32, stats->nonPersistent.rlc.ulReOdrTmrExpCnt, mBuf);

   CMCHKPK(packUint32, stats->nonPersistent.pdcp.dlFlowCtrlDropCnt, mBuf);
   CMCHKPK(packUint32, stats->nonPersistent.pdcp.dlPdcpAckWaitDropCnt, mBuf);
   CMCHKPK(packUint32, stats->nonPersistent.pdcp.dlPdcpDropCnt, mBuf);


   for (k = 0; k < L2_STATS_MAX_CELLS; k++)
   {
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].ulTpt, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].ulNumiTbs, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].ulSumiTbs, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].ulNumCqi, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].ulSumCqi, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].ulDtxCnt, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].ulNackCnt, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].ulAckNackCnt, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].ulPrbUsg, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].ulRetxOccns, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].ulTxOccns, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].dlTpt, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].dlBo, mBuf);
      for (i=0; i<4; i++)
      {
         CMCHKPK(packUint32, stats->nonPersistent.sch[k].riCnt[i], mBuf);
      }
      for (i=0; i<5; i++)
      {
         CMCHKPK(packUint32, stats->nonPersistent.sch[k].dlPdbLvl[i], mBuf);
      }
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].cqiDropCnt, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].dlNumCw1iTbs, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].dlSumCw1iTbs, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].dlNumCw0iTbs, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].dlSumCw0iTbs, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].dlNumCw1Cqi, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].dlSumCw1Cqi, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].dlNumCw0Cqi, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].dlSumCw0Cqi, mBuf);
      for (i=0; i<2; i++)
      {
         CMCHKPK(packUint32, stats->nonPersistent.sch[k].dlDtxCnt[i], mBuf);
      }
      for (i=0; i<2; i++)
      {
         CMCHKPK(packUint32, stats->nonPersistent.sch[k].dlAckNackCnt[i], mBuf);
      }
      for (i=0; i<2; i++)
      {
         CMCHKPK(packUint32, stats->nonPersistent.sch[k].dlNackCnt[i], mBuf);
      }
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].dlPrbUsg, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].dlRetxOccns, mBuf);
      CMCHKPK(packUint32, stats->nonPersistent.sch[k].dlTxOccns, mBuf);
   }
   CMCHKPK(packUint32, stats->rnti, mBuf);
   RETVOID;
}

#ifdef ANSI
PRIVATE Void TSInfUtlPackCellInfo
(
 Buffer              *mBuf,
 TSInfL2CellStats     *stats  
)
#else
PRIVATE Void TSInfUtlPackCellInfo(mBuf, stats)
 Buffer              *mBuf;
 TSInfL2CellStats     *stats; 
#endif
{
   S32 i,j;

   TRC2(TSInfUtlPackCellInfo)

   CMCHKPK(packUint32, stats->rlc.reOdrTmrExp, mBuf);
   CMCHKPK(packUint32, stats->rlc.maxRlcDrbRetxFail, mBuf);
   CMCHKPK(packUint32, stats->rlc.maxRlcSrbRetxFail, mBuf);

   CMCHKPK(packUint32, stats->sch.ulNumiTbs, mBuf);
   CMCHKPK(packUint32, stats->sch.ulSumiTbs, mBuf);
   CMCHKPK(packUint32, stats->sch.ulNumCqi, mBuf);
   CMCHKPK(packUint32, stats->sch.ulSumCqi, mBuf);
#ifdef RG_5GTF
   CMCHKPK(packUint32, stats->sch.ul5gtfRbAllocFail, mBuf);
   CMCHKPK(packUint32, stats->sch.ul5gtfPdcchSend, mBuf);
   CMCHKPK(packUint32, stats->sch.ul5gtfUeFnlzReAdd, mBuf);
   CMCHKPK(packUint32, stats->sch.ul5gtfUeRmvFnlzZeroBo, mBuf);
   CMCHKPK(packUint32, stats->sch.ul5gtfUeRbAllocDone, mBuf);
   CMCHKPK(packUint32, stats->sch.ul5gtfAllocAllocated, mBuf);
   CMCHKPK(packUint32, stats->sch.ul5gtfPdcchSchd, mBuf);
   CMCHKPK(packUint32, stats->sch.ul5gtfUeSchPick, mBuf);
   CMCHKPK(packUint32, stats->sch.ul5gtfBsrRecv, mBuf);
   CMCHKPK(packUint32, stats->sch.ul5gtfSrRecv, mBuf);

   CMCHKPK(packUint32, stats->sch.dl5gtfPdschCons, mBuf);
   CMCHKPK(packUint32, stats->sch.dl5gtfPdcchSend, mBuf);
   CMCHKPK(packUint32, stats->sch.dl5gtfBoUpd, mBuf);
   CMCHKPK(packUint32, stats->sch.dl5gtfFnlzFail, mBuf);
   CMCHKPK(packUint32, stats->sch.dl5gtfFnlzPass, mBuf);
   CMCHKPK(packUint32, stats->sch.dl5gtfRbAllocFail, mBuf);
   CMCHKPK(packUint32, stats->sch.dl5gtfRbAllocPass, mBuf);
   CMCHKPK(packUint32, stats->sch.dl5gtfUePick, mBuf);
#endif
   CMCHKPK(packUint32, stats->sch.cntTtiProcTime500, mBuf);
   CMCHKPK(packUint32, stats->sch.cntTtiProcTime600, mBuf);
   CMCHKPK(packUint32, stats->sch.cntTtiProcTime700, mBuf);
   CMCHKPK(packUint32, stats->sch.cntTtiProcTime800, mBuf);
   CMCHKPK(packUint32, stats->sch.cntTtiProcTime900, mBuf);
   CMCHKPK(packUint32, stats->sch.cntTtiProcTime1000, mBuf);
   CMCHKPK(packUint32, stats->sch.avgTtiProcTime, mBuf);
   for (i=9; i>=0; i--)
   {
      CMCHKPK(packUint32, stats->sch.dlPdbRatio[i], mBuf);
   }
   for (i=9; i>=0; i--)
   {
      CMCHKPK(packUint32, stats->sch.ulPrbUsage[i], mBuf);
   }
   for (i=9; i>=0; i--)
   {
      CMCHKPK(packUint32, stats->sch.dlPrbUsage[i], mBuf);
   }
   for (i=3; i>=0; i--)
   {
      CMCHKPK(packUint32, stats->sch.ulDtx[i], mBuf);
   }
   for (i=3; i>=0; i--)
   {
      CMCHKPK(packUint32, stats->sch.ulAckNack[i], mBuf);
   }
   for (i=3; i>=0; i--)
   {
      CMCHKPK(packUint32, stats->sch.ulNack[i], mBuf);
   }
   for (i=1; i>=0; i--)
   {
      for (j=3; j>=0; j--)
      {
         CMCHKPK(packUint32, stats->sch.dlDtx[i][j], mBuf);
      }
   }
   for (i=1; i>=0; i--)
   {
      for (j=3; j>=0; j--)
      {
         CMCHKPK(packUint32, stats->sch.dlAckNack[i][j], mBuf);
      }
   }
   for (i=1; i>=0; i--)
   {
      for (j=3; j>=0; j--)
      {
         CMCHKPK(packUint32, stats->sch.dlNack[i][j], mBuf);
      }
   }
   for (i=3; i>=0; i--)
   {
      CMCHKPK(packUint32, stats->sch.riCnt[i], mBuf);
   }
   CMCHKPK(packUint32, stats->sch.dlNumCw1iTbs, mBuf);
   CMCHKPK(packUint32, stats->sch.dlSumCw1iTbs, mBuf);
   CMCHKPK(packUint32, stats->sch.dlNumCw0iTbs, mBuf);
   CMCHKPK(packUint32, stats->sch.dlSumCw0iTbs, mBuf);
   CMCHKPK(packUint32, stats->sch.dlNumCw1Cqi, mBuf);
   CMCHKPK(packUint32, stats->sch.dlSumCw1Cqi, mBuf);
   CMCHKPK(packUint32, stats->sch.dlNumCw0Cqi, mBuf);
   CMCHKPK(packUint32, stats->sch.dlSumCw0Cqi, mBuf);
   CMCHKPK(packUint32, stats->sch.msg3Fail, mBuf);
   CMCHKPK(packUint32, stats->sch.msg4Fail, mBuf);

   CMCHKPK(packUint32, stats->cellId, mBuf);

   RETVOID;
}

#ifdef ANSI
PRIVATE Void TSInfUtlUnpkUeInfo
(
 Buffer            *mBuf,
 TSInfL2UeStats     *stats 
)
#else
PRIVATE Void TSInfUtlUnpkUeInfo(mBuf, stats)
 Buffer            *mBuf;
 TSInfL2UeStats     *stats; 
#endif
{
   S32 i;
   U32 k;

   TRC2(TSInfUtlUnpkUeInfo)

   CMCHKUNPK(unPackUint32, &stats->rnti, mBuf);

   for (k = L2_STATS_MAX_CELLS; k > 0; k--)
   {
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].dlTxOccns, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].dlRetxOccns, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].dlPrbUsg, mBuf);
      for (i=1; i>=0; i--)
      {
         CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].dlNackCnt[i], mBuf);
      }
      for (i=1; i>=0; i--)
      {
         CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].dlAckNackCnt[i], mBuf);
      }
      for (i=1; i>=0; i--)
      {
         CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].dlDtxCnt[i], mBuf);
      }
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].dlSumCw0Cqi, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].dlNumCw0Cqi, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].dlSumCw1Cqi, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].dlNumCw1Cqi, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].dlSumCw0iTbs, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].dlNumCw0iTbs, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].dlSumCw1iTbs, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].dlNumCw1iTbs, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].cqiDropCnt, mBuf);
      for (i=4; i>=0; i--)
      {
         CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].dlPdbLvl[i], mBuf);
      }
      for (i=3; i>=0; i--)
      {
         CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].riCnt[i], mBuf);
      }
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].dlBo, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].dlTpt, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].ulTxOccns, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].ulRetxOccns, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].ulPrbUsg, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].ulAckNackCnt, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].ulNackCnt, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].ulDtxCnt, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].ulSumCqi, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].ulNumCqi, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].ulSumiTbs, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].ulNumiTbs, mBuf);
      CMCHKUNPK(unPackUint32, &stats->nonPersistent.sch[k-1].ulTpt, mBuf);
   }

   CMCHKUNPK(unPackUint32, &stats->nonPersistent.pdcp.dlPdcpDropCnt, mBuf);
   CMCHKUNPK(unPackUint32, &stats->nonPersistent.pdcp.dlPdcpAckWaitDropCnt, mBuf);
   CMCHKUNPK(unPackUint32, &stats->nonPersistent.pdcp.dlFlowCtrlDropCnt, mBuf);

   CMCHKUNPK(unPackUint32, &stats->nonPersistent.rlc.ulReOdrTmrExpCnt, mBuf);
   CMCHKUNPK(unPackUint32, &stats->nonPersistent.rlc.dlMaxPktsInSduQ, mBuf);
   CMCHKUNPK(unPackUint32, &stats->nonPersistent.rlc.dlMaxWindowSz, mBuf);

   CMCHKUNPK(unPackUint32, &stats->persistent.activatedSCells, mBuf);
   CMCHKUNPK(unPackUint32, &stats->persistent.numActivation, mBuf);
   CMCHKUNPK(unPackUint32, &stats->persistent.numDeactivation, mBuf);

   RETVOID;
}

#ifdef ANSI
PRIVATE Void TSInfUtlUnpkCellInfo
(
 Buffer              *mBuf,
 TSInfL2CellStats     *stats 
)
#else
PRIVATE Void TSInfUtlUnpkCellInfo(mBuf, stats)
 Buffer              *mBuf;
 TSInfL2CellStats     *stats; 
#endif
{
   S32 i,j;

   TRC2(TSInfUtlUnpkCellInfo)

      CMCHKUNPK(unPackUint32, &stats->cellId, mBuf);

   CMCHKUNPK(unPackUint32, &stats->sch.msg4Fail, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.msg3Fail, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.dlSumCw0Cqi, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.dlNumCw0Cqi, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.dlSumCw1Cqi, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.dlNumCw1Cqi, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.dlSumCw0iTbs, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.dlNumCw0iTbs, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.dlSumCw1iTbs, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.dlNumCw1iTbs, mBuf);
   for (i=0; i<4; i++)
   {
      CMCHKUNPK(unPackUint32, &stats->sch.riCnt[i], mBuf);
   }
   for (i=0; i<2; i++)
   {
      for (j=0; j<4; j++)
      {
         CMCHKUNPK(unPackUint32, &stats->sch.dlNack[i][j], mBuf);
      }
   }
   for (i=0; i<2; i++)
   {
      for (j=0; j<4; j++)
      {
         CMCHKUNPK(unPackUint32, &stats->sch.dlAckNack[i][j], mBuf);
      }
   }
   for (i=0; i<2; i++)
   {
      for (j=0; j<4; j++)
      {
         CMCHKUNPK(unPackUint32, &stats->sch.dlDtx[i][j], mBuf);
      }
   }
   for (i=0; i<4; i++)
   {
      CMCHKUNPK(unPackUint32, &stats->sch.ulNack[i], mBuf);
   }
   for (i=0; i<4; i++)
   {
      CMCHKUNPK(unPackUint32, &stats->sch.ulAckNack[i], mBuf);
   }
   for (i=0; i<4; i++)
   {
      CMCHKUNPK(unPackUint32, &stats->sch.ulDtx[i], mBuf);
   }
   for (i=0; i<10; i++)
   {
      CMCHKUNPK(unPackUint32, &stats->sch.dlPrbUsage[i], mBuf);
   }
   for (i=0; i<10; i++)
   {
      CMCHKUNPK(unPackUint32, &stats->sch.ulPrbUsage[i], mBuf);
   }
   for (i=0; i<10; i++)
   {
      CMCHKUNPK(unPackUint32, &stats->sch.dlPdbRatio[i], mBuf);
   }
   CMCHKUNPK(unPackUint32, &stats->sch.avgTtiProcTime, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.cntTtiProcTime1000, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.cntTtiProcTime900, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.cntTtiProcTime800, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.cntTtiProcTime700, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.cntTtiProcTime600, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.cntTtiProcTime500, mBuf);
#ifdef RG_5GTF
   CMCHKUNPK(unPackUint32, &stats->sch.dl5gtfUePick, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.dl5gtfRbAllocPass, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.dl5gtfRbAllocFail, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.dl5gtfFnlzPass, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.dl5gtfFnlzFail, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.dl5gtfBoUpd, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.dl5gtfPdcchSend, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.dl5gtfPdschCons, mBuf);

   CMCHKUNPK(unPackUint32, &stats->sch.ul5gtfSrRecv, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.ul5gtfBsrRecv, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.ul5gtfUeSchPick, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.ul5gtfPdcchSchd, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.ul5gtfAllocAllocated, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.ul5gtfUeRbAllocDone, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.ul5gtfUeRmvFnlzZeroBo, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.ul5gtfUeFnlzReAdd, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.ul5gtfPdcchSend, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.ul5gtfRbAllocFail, mBuf);
#endif
   CMCHKUNPK(unPackUint32, &stats->sch.ulSumCqi, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.ulNumCqi, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.ulSumiTbs, mBuf);
   CMCHKUNPK(unPackUint32, &stats->sch.ulNumiTbs, mBuf);

   CMCHKUNPK(unPackUint32, &stats->rlc.maxRlcSrbRetxFail, mBuf);
   CMCHKUNPK(unPackUint32, &stats->rlc.maxRlcDrbRetxFail, mBuf);
   CMCHKUNPK(unPackUint32, &stats->rlc.reOdrTmrExp, mBuf);

   RETVOID;
}

#ifdef ANSI
PUBLIC Void TSInfPkSndL2UeStats
(
 Pst               *pst,
 SuId              suId,
 TSInfL2UeStats     *stats  
)
#else
PUBLIC Void TSInfPkSndL2UeStats(pst, suId, stats)
 Pst               *pst;
 SuId              suId;
 TSInfL2UeStats     *stats;
#endif
{
   Buffer *mBuf;

   TRC2(TSInfPkSndL2UeStats)

   mBuf = TSInfUtlAllocMsg(pst);
   TSInfUtlPackUeInfo(mBuf, stats);
   SPkS16(suId, mBuf);
   pst->event = (Event) EVTTENBL2UESTATS;
   SPstTsk(pst, mBuf);
   RETVOID;
}

#ifdef ANSI
PUBLIC Void TSInfUnpkL2UeStats
(
 TSInfL2UeStatsInd   func,
 Pst                *pst,
 Buffer             *mBuf
)
#else
PUBLIC Void TSInfUnpkL2UeStats(func, pst, mBuf)
 TSInfL2UeStatsInd   func;
 Pst                *pst;
 Buffer             *mBuf;
#endif
{
   SuId              suId;
   TSInfL2UeStats     stats; 

   TRC2(TSInfUnpkL2UeStats)

   SUnpkS16(&suId, mBuf);
   TSInfUtlUnpkUeInfo(mBuf, &stats);
   SPutMsg(mBuf);
   (*func)(pst, suId, &stats);
   RETVOID;
}

#ifdef ANSI
PUBLIC Void TSInfPkSndL2CellStats
(
 Pst                 *pst,
 SuId                suId,
 TSInfL2CellStats   *stats 
)
#else
PUBLIC Void TSInfPkSndL2CellStats(pst, suId, stats)
 Pst                 *pst;
 SuId                suId;
 TSInfL2Cellstats   *stats;
#endif
{
   Buffer *mBuf;

   TRC2(TSInfPkSndL2CellStats)

   mBuf = TSInfUtlAllocMsg(pst);
   TSInfUtlPackCellInfo(mBuf, stats);
   SPkS16(suId, mBuf);
   pst->event = (Event) EVTTENBL2CELLSTATS;
   SPstTsk(pst, mBuf);
   RETVOID;
}

#ifdef ANSI
PUBLIC Void TSInfUnpkL2CellStats
(
 TSInfL2CellStatsInd   func,
 Pst                *pst,
 Buffer             *mBuf
)
#else
PUBLIC Void TSInfUnpkL2CellStats(func, pst, mBuf)
 TSInfL2CellStatsInd   func;
 Pst                *pst;
 Buffer             *mBuf;
#endif
{
   SuId                suId;
   TSInfL2CellStats    stats; 

   TRC2(TSInfUnpkL2CellStats)

   SUnpkS16(&suId, mBuf);
   TSInfUtlUnpkCellInfo(mBuf, &stats);
   SPutMsg(mBuf);
   (*func)(pst, suId, &stats);
   RETVOID;
}
#endif /* TENB_STATS */

/**********************************************************************
         End of file
 **********************************************************************/
