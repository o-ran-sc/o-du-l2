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
 
    Type:  
 
    Desc:  
 
    File:  cm_tenb_stats.x 
 
**********************************************************************/
 
/** @file cm_tenb_stats.x
*/

#ifdef TENB_STATS
 
#include "cm_tenb_stats.h"
#include "l2_tenb_stats.h"

#ifndef __TENBSTATSINFX__
#define __TENBSTATSINFX__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct tsInfL2CellStats
{
   uint32_t   cellId;
   struct schCellStats{ 
      uint32_t   msg4Fail;
      uint32_t   msg3Fail;
      uint32_t   dlSumCw0Cqi;
      uint32_t   dlNumCw0Cqi;
      uint32_t   dlSumCw1Cqi;
      uint32_t   dlNumCw1Cqi;
      uint32_t   dlSumCw0iTbs;
      uint32_t   dlNumCw0iTbs;
      uint32_t   dlSumCw1iTbs;
      uint32_t   dlNumCw1iTbs;
      uint32_t   riCnt[4]; /* Rank = 1..4 */
      uint32_t   dlNack[2][4];
      uint32_t   dlAckNack[2][4]; /* [CW_CNT][RV_CNT] */
      uint32_t   dlDtx[2][4];
      uint32_t   ulNack[4];
      uint32_t   ulAckNack[4];    /* [RV_CNT] */
      uint32_t   ulDtx[4];
      uint32_t   dlPrbUsage[10]; /* dlPrbUsage[0] to have overall PRB usage */
      uint32_t   ulPrbUsage[10]; /* dlPrbUsage[0] to have overall PRB usage */
      uint32_t   dlPdbRatio[10];
      uint32_t   avgTtiProcTime; /* In us */
      uint32_t   cntTtiProcTime1000; /* Count of instances where TTI proc time was
      greater than 1000us */
      uint32_t   cntTtiProcTime900; /* Count of instances where TTI proc time was
      greater than 900us and less than 1000us */
      uint32_t   cntTtiProcTime800;
      uint32_t   cntTtiProcTime700;
      uint32_t   cntTtiProcTime600;
      uint32_t   cntTtiProcTime500;
#ifdef RG_5GTF
      uint32_t   dl5gtfUePick;
      uint32_t   dl5gtfRbAllocPass;
      uint32_t   dl5gtfRbAllocFail;
      uint32_t   dl5gtfFnlzPass;
      uint32_t   dl5gtfFnlzFail;
      uint32_t   dl5gtfBoUpd;
      uint32_t   dl5gtfPdcchSend;
      uint32_t   dl5gtfPdschCons;

      uint32_t   ul5gtfSrRecv;
      uint32_t   ul5gtfBsrRecv;
      uint32_t   ul5gtfUeSchPick;
      uint32_t   ul5gtfPdcchSchd;
      uint32_t   ul5gtfAllocAllocated;
      uint32_t   ul5gtfUeRbAllocDone;
      uint32_t   ul5gtfUeRmvFnlzZeroBo;
      uint32_t   ul5gtfUeFnlzReAdd;
      uint32_t   ul5gtfPdcchSend;
      uint32_t   ul5gtfRbAllocFail;
#endif
      uint32_t   ulSumCqi;
      uint32_t   ulNumCqi;
      uint32_t   ulSumiTbs;
      uint32_t   ulNumiTbs;
      uint32_t   dlTtlTpt;
      uint32_t   ulTtlTpt;
   }sch;
   struct rlcCellStats{ 
      uint32_t   maxRlcSrbRetxFail;
      uint32_t   maxRlcDrbRetxFail;
      uint32_t   reOdrTmrExp;
   }rlc;
}TSInfL2CellStats;

typedef struct tsInfL2UeStats
{
   uint32_t rnti;
   struct nonPersistentUeStats{
      struct schUeStats{
         uint32_t dlTxOccns;
         uint32_t dlRetxOccns;
         uint32_t dlPrbUsg;
         uint32_t dlNackCnt[2]; /* [CW_CNT] */
         uint32_t dlAckNackCnt[2]; /* [CW_CNT] */
         uint32_t dlDtxCnt[2]; /* [CW_CNT] */
         uint32_t dlSumCw0Cqi;
         uint32_t dlNumCw0Cqi;
         uint32_t dlSumCw1Cqi;
         uint32_t dlNumCw1Cqi;
         uint32_t dlSumCw0iTbs;
         uint32_t dlNumCw0iTbs;
         uint32_t dlSumCw1iTbs;
         uint32_t dlNumCw1iTbs;
         uint32_t cqiDropCnt;
         uint32_t dlPdbLvl[5]; /* PDB Levels */ 
         uint32_t riCnt[4]; /* Rank = 1..4 */
         uint32_t dlBo;
         uint32_t dlTpt;
         uint32_t ulTxOccns;
         uint32_t ulRetxOccns;
         uint32_t ulPrbUsg;
         uint32_t ulAckNackCnt;
         uint32_t ulNackCnt;
         uint32_t ulDtxCnt;
         uint32_t ulSumCqi;
         uint32_t ulNumCqi;
         uint32_t ulSumiTbs;
         uint32_t ulNumiTbs;
         uint32_t ulTpt;
      }sch[L2_STATS_MAX_CELLS];
      struct pdcpUeStats{
         uint32_t dlPdcpDropCnt;
         uint32_t dlPdcpAckWaitDropCnt;
         uint32_t dlFlowCtrlDropCnt;
      }pdcp;
      struct rlcUeStats{
         uint32_t ulReOdrTmrExpCnt;
         uint32_t dlMaxPktsInSduQ;
         uint32_t dlMaxWindowSz;
      }rlc;
   }nonPersistent;
   struct persistentUeStats{
      uint32_t activatedSCells;
      uint32_t numActivation;
      uint32_t numDeactivation;
   }persistent;
}TSInfL2UeStats;

typedef struct tsInfStatsCb
{
   Bool initDone;
   Bool sockSend;
   Bool fileSend;
   Bool consPrint;
   uint32_t  statsPer;
   uint32_t  sockPort;
   Bool startSockSend;
   uint8_t   cmdForFileStats;
   char fileName[100];
   FILE *fptr;
   uint32_t  enbIp;
}TSInfStatsCb;

typedef Void (*TSInfL2UeStatsInd) ARGS((
 Pst *, 
 SuId , 
 TSInfL2UeStats *
));
Void TSInfHdlL2UeStatsInd ARGS((
 Pst               *pst,
 SuId              suId,
 TSInfL2UeStats     *stats 
));
Void TSInfUnpkL2UeStats ARGS((
 TSInfL2UeStatsInd   func,
 Pst                *pst,
 Buffer             *mBuf
));
typedef Void (*TSInfL2CellStatsInd) ARGS((
 Pst *, 
 SuId , 
 TSInfL2CellStats *
));
Void TSInfHdlL2CellStatsInd ARGS((
 Pst                 *pst,
 SuId                suId,
 TSInfL2CellStats    *stats 
));
Void TSInfUnpkL2CellStats ARGS((
 TSInfL2CellStatsInd   func,
 Pst                  *pst,
 Buffer               *mBuf
));
Void TSInfPkSndL2UeStats ARGS((
 Pst               *pst,
 SuId              suId,
 TSInfL2UeStats     *stats 
));
Void TSInfPkSndL2CellStats ARGS((
 Pst                 *pst,
 SuId                suId,
 TSInfL2CellStats     *stats 
));
Void TSInfTrigStats ARGS((
 Region    region,
 Pool      pool
));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __TENBSTATSINFX__ */
#endif /* TENB_STATS */

/**********************************************************************
         End of file
**********************************************************************/
