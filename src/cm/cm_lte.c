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
 
     Name:     Common LTE
  
     Type:     C source file
  
     Desc:     C code for packing/unpacking of cm_lte data structures. 
  
     File:     cm_lte.c 
  
**********************************************************************/

/* header include files (.h) */

#include "envopt.h"             /* environment options */  
#include "envdep.h"             /* environment dependent */
#include "envind.h"             /* environment independent */

/* header/extern include files (.x) */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm_lte.h"             /* common lte header file */
#include "cm5.h"           /* common timers */
#include "mt_ss.h"         /* MTSS specific */
#include "mt_err.h"        /* MTSS error defines */
#include "ss_queue.h"      /* queues */
#include "ss_task.h"       /* tasking */
#include "ss_msg.h"        /* messaging */
#include "ss_mem.h"        /* memory management interface */
#include "ss_gen.h"        /* general */
/* mt003.301 Additions - Task deregistration */
#include "ss_err.h"        /* error */


/* header/extern include files (.x) */

#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm_lte.x"             /* common lte header file */
#include "cm5.x"           /* common timers */
#include "mt_ss.x"         /* MTSS specific */
#include "ss_queue.x"      /* queues */
#include "ss_task.x"       /* tasking */
#include "ss_timer.x"      /* timers */
#include "ss_strm.h"
#include "ss_strm.x"       /* STREAMS */
#include "ss_msg.x"        /* messaging */
#include "ss_mem.x"        /* memory management interface */
#include "ss_drvr.x"       /* driver tasks */
#include "ss_gen.x"        /* general */


/* public variable declarations */
U32 cmLteTime;


/***********************************************************
*
*     Func : cmPkLteRlcId
*
*
*     Desc : RLC Identifier
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
S16 cmPkLteRlcId
(
CmLteRlcId *param,
Buffer *mBuf
)
#else
S16 cmPkLteRlcId(param, mBuf)
CmLteRlcId *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkLteRlcId);

   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   CMCHKPK(oduUnpackUInt8, param->rbType, mBuf);
   CMCHKPK(cmPkLteRbId, param->rbId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkLteRlcId
*
*
*     Desc : RLC Identifier
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
S16 cmUnpkLteRlcId
(
CmLteRlcId *param,
Buffer *mBuf
)
#else
S16 cmUnpkLteRlcId(param, mBuf)
CmLteRlcId *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkLteRlcId);

   CMCHKUNPK(cmUnpkLteRbId, &param->rbId, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->rbType, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkLteTimingInfo
*
*
*     Desc : LTE Timing Info
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
S16 cmPkLteTimingInfo
(
CmLteTimingInfo *param,
Buffer *mBuf
)
#else
S16 cmPkLteTimingInfo(param, mBuf)
CmLteTimingInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkLteTimingInfo);

   CMCHKPK(oduUnpackUInt16, param->slot, mBuf);
   CMCHKPK(oduUnpackUInt16, param->sfn, mBuf);
   //CMCHKPK(oduUnpackUInt16, param->hSfn, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkLteTimingInfo
*
*
*     Desc : LTE Timing Info
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
S16 cmUnpkLteTimingInfo
(
CmLteTimingInfo *param,
Buffer *mBuf
)
#else
S16 cmUnpkLteTimingInfo(param, mBuf)
CmLteTimingInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkLteTimingInfo);

   //CMCHKUNPK(oduPackUInt16, &param->hSfn, mBuf);
   CMCHKUNPK(oduPackUInt16, &param->sfn, mBuf);
   CMCHKUNPK(oduPackUInt16,&param->slot, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmPkLtePdcpId
*
*
*     Desc : PDCP Id
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
S16 cmPkLtePdcpId
(
CmLtePdcpId *param,
Buffer *mBuf
)
#else
S16 cmPkLtePdcpId(param, mBuf)
CmLtePdcpId *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkLtePdcpId);

   CMCHKPK(oduUnpackUInt8, param->rbType, mBuf);
   CMCHKPK(cmPkLteRbId, param->rbId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   return ROK;
}



/***********************************************************
*
*     Func : cmUnpkLtePdcpId
*
*
*     Desc : PDCP Id
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
S16 cmUnpkLtePdcpId
(
CmLtePdcpId *param,
Buffer *mBuf
)
#else
S16 cmUnpkLtePdcpId(param, mBuf)
CmLtePdcpId *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkLtePdcpId);

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteRbId, &param->rbId, mBuf);
   CMCHKUNPK(oduPackUInt8, &param->rbType, mBuf);
   return ROK;
}
#ifdef LTE_L2_MEAS 
#ifdef ANSI
S16 cmUpdateSsiMemInfo
(
CmLteMemInfo *mInfo
)
#else
S16 cmUpdateSsiMemInfo(mInfo)
CmLteMemInfo *mInfo;
#endif
{
   U8   numReg = 0;
   U8   numPool = 0;
   U8   idxReg;
   U8   idxPool;
   SsMemDbgInfo dbgInfo;

   SGetRegPoolInfo(&numReg,&numPool); 
   mInfo->numRegions = numReg;

   for(idxReg=0; idxReg< numReg; idxReg++)
   {
      SGetRegInfo(idxReg, &dbgInfo);

      if(mInfo->regInfo[idxReg].isGenMemInfoUpdated == TRUE)
      {
         for(idxPool=0; idxPool<numPool; idxPool++)
         {
            mInfo->regInfo[idxReg].poolInfo[idxPool].crntUsed = dbgInfo.bktDbgTbl[idxPool].numAlloc;
            if(mInfo->regInfo[idxReg].poolInfo[idxPool].crntUsed > mInfo->regInfo[idxReg].poolInfo[idxPool].maxUsed)
               mInfo->regInfo[idxReg].poolInfo[idxPool].maxUsed = mInfo->regInfo[idxReg].poolInfo[idxPool].crntUsed;
         }
      }
      else
      {
      mInfo->regInfo[idxReg].numPools = numPool;
      mInfo->regInfo[idxReg].regionId = idxReg;
      mInfo->regInfo[idxReg].regionType = 0;

      for(idxPool=0; idxPool<numPool; idxPool++)
      {
         mInfo->regInfo[idxReg].poolInfo[idxPool].totAvailable = dbgInfo.bktDbgTbl[idxPool].numBlks;
         mInfo->regInfo[idxReg].poolInfo[idxPool].poolSize = dbgInfo.bktDbgTbl[idxPool].size;
         mInfo->regInfo[idxReg].poolInfo[idxPool].crntUsed = dbgInfo.bktDbgTbl[idxPool].numAlloc;
         if(mInfo->regInfo[idxReg].poolInfo[idxPool].crntUsed > mInfo->regInfo[idxReg].poolInfo[idxPool].maxUsed)
            mInfo->regInfo[idxReg].poolInfo[idxPool].maxUsed = mInfo->regInfo[idxReg].poolInfo[idxPool].crntUsed;
      }
         mInfo->regInfo[idxReg].isGenMemInfoUpdated = TRUE; 
      }
   }
   return ROK;
}

#ifdef ANSI
S16 cmFillMemUtilizationMeas
(
 CmLteMemInfo *memoryInfo,
CmLteMemInfo *memInfo
)
#else
S16 cmFillMemUtilizationMeas(memoryInfo,memInfo)
CmLteMemInfo *memoryInfo;
CmLteMemInfo *memInfo;
#endif
{
   U8 idxReg = 0;
   U8 idxPool = 0;
   U8 numPool = 0;

   memoryInfo->numRegions = memInfo->numRegions;
   for(idxReg=0; idxReg < memInfo->numRegions; idxReg++)
   {
      memoryInfo->regInfo[idxReg].regionId = memInfo->regInfo[idxReg].regionId;
      memoryInfo->regInfo[idxReg].numPools = memInfo->regInfo[idxReg].numPools;
      memoryInfo->regInfo[idxReg].regionType = memInfo->regInfo[idxReg].regionType;
      numPool = memoryInfo->regInfo[idxReg].numPools;

      for(idxPool=0; idxPool < numPool; idxPool++)
      {
         memoryInfo->regInfo[idxReg].poolInfo[idxPool].totAvailable = memInfo->regInfo[idxReg].poolInfo[idxPool].totAvailable;
         memoryInfo->regInfo[idxReg].poolInfo[idxPool].poolSize = memInfo->regInfo[idxReg].poolInfo[idxPool].poolSize;
         memoryInfo->regInfo[idxReg].poolInfo[idxPool].crntUsed = memInfo->regInfo[idxReg].poolInfo[idxPool].crntUsed;
         memoryInfo->regInfo[idxReg].poolInfo[idxPool].maxUsed  = memInfo->regInfo[idxReg].poolInfo[idxPool].maxUsed;
      }
   }
   return ROK;
}

#ifdef ANSI
S16 cmClearMemUtilizationCounter
(
CmLteMemInfo *memInfo
)
#else
S16 cmClearMemUtilizationCounter(memInfo)
CmLteMemInfo *memInfo;
#endif
{
   U8 idxReg = 0;
   U8 idxPool = 0;
   U8 numPool = 0;
   for(idxReg=0; idxReg < memInfo->numRegions; idxReg++)
   {
      memInfo->regInfo[idxReg].regionId = 0;
      memInfo->regInfo[idxReg].numPools = 0;
      memInfo->regInfo[idxReg].regionType = 0;
      memInfo->regInfo[idxReg].isGenMemInfoUpdated = FALSE;
      numPool = memInfo->regInfo[idxReg].numPools;

      for(idxPool=0; idxPool<numPool; idxPool++)
      {
         memInfo->regInfo[idxReg].poolInfo[idxPool].totAvailable = 0;
         memInfo->regInfo[idxReg].poolInfo[idxPool].poolSize = 0;
         memInfo->regInfo[idxReg].poolInfo[idxPool].crntUsed = 0;
         memInfo->regInfo[idxReg].poolInfo[idxPool].maxUsed  = 0;
      }
   }
   return ROK;
}

#ifdef ANSI
S16 cmClearCpuUtilizationCounter
(
CmCpuStatsInfo *cpuInfo
)
#else
S16 cmClearCpuUtilizationCounter(cpuInfo)
CmCpuStatsInfo *cpuInfo
#endif
{
   U8 idx= 0;
   for(idx=0; idx < cpuInfo->numCores; idx++)
   {
      cpuInfo->cpuUtil[idx].maxCpuUtil = 0;
      cpuInfo->cpuUtil[idx].totCpuUtil = 0;
      cpuInfo->cpuUtil[idx].numSamples = 0;
   }
   cpuInfo->numCores = 0;
   return ROK;
}

#ifdef ANSI
S16 cmFillCpuUtilizationMeas
(
CmLteCpuInfo *cpuMeasInfo,
CmCpuStatsInfo *cpuInfo
)
#else
S16 cmFillCpuUtilizationMeas(cpuMeasInfo,cpuInfo)
CmLteCpuInfo *cpuMeasInfo;
CmCpuStatsInfo *cpuInfo;
#endif
{
   U8 idx= 0;
   cpuMeasInfo->numCores = cpuInfo->numCores;
   for(idx=0; idx < cpuInfo->numCores; idx++)
   {
      cpuMeasInfo->cpuUtil[idx].avgCpuUtil = cpuInfo->cpuUtil[idx].totCpuUtil/cpuInfo->cpuUtil[idx].numSamples;
      cpuMeasInfo->cpuUtil[idx].maxCpuUtil = cpuInfo->cpuUtil[idx].maxCpuUtil;
   }
   return ROK;

}
#endif

#ifdef TENB_TTI_PERF
CmTtiProc ttiProc;
#define TTI_1MS 950

Void cmUpdateTtiCounters(U32 ttiProcessingTime)
{
   ttiProc.numOfTti++;
   ttiProc.totTtiProcessingTime += ttiProcessingTime;
   if(ttiProcessingTime > ttiProc.maxTtiProcessingTime)  
   {
      ttiProc.maxTtiProcessingTime = ttiProcessingTime;
   }
   if(ttiProcessingTime > TTI_THRESHOLD_VALUE)
   {
      ttiProc.ttiThresholdExceedCount++;
   }
   if(ttiProcessingTime > TTI_1MS)
   {
      ttiProc.ttiStretchCount++;
   }
   RETVOID;    
}

Void cmResetTtiCounters(Void)
{
	ttiProc.totTtiProcessingTime = 0;
	ttiProc.numOfTti = 0;
	ttiProc.maxTtiProcessingTime = 0;
	ttiProc.ttiStretchCount = 0;
	ttiProc.ttiThresholdExceedCount = 0;
	ttiProc.phyReptTtiStretchCount = 0;
	RETVOID;
}
#endif

/**********************************************************************
         End of file
**********************************************************************/
