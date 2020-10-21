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
 
    File:  l2_tenb_stats.c
 
**********************************************************************/

/* header (.h) include files */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_err.h"        /* RLC error options */
#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */
#include "kw_ul.h"

/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "kw.x"
#include "kw_ul.x"
#ifdef TENB_STATS
#include "l2_tenb_stats.x"    /* Total EnodeB Stats declarations */
#endif

#ifdef TENB_STATS
TSL2CellStatsCb* l2CellStats[L2_STATS_MAX_CELLS];
TSL2UeStatsCb*   l2UeStats[L2_STATS_MAX_UES];
CmLListCp        freeL2UeStatsLst; /*!< Free Pool of UE stats Blocks */
CmLListCp        inUseL2UeStatsLst;/*!< In Use Pool of UE stats Blocks */

/*
*
*       Fun:   TSL2AllocStatsMem
*
*       Desc:  Pre-Allocate Memory for L2 stats BLOCKs 
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
Void TSL2AllocStatsMem 
(
 Region region,
 Pool   pool 
)
#else
Void TSL2AllocStatsMem(region, pool)
 Region region;
 Pool   pool;
#endif
{
   U32 cnt=0;

   cmLListInit(&inUseL2UeStatsLst);     
   cmLListInit(&freeL2UeStatsLst);      
   for (cnt=0; cnt < L2_STATS_MAX_CELLS; cnt++)
   {
		  if(NULL == l2CellStats[cnt])
		  {
      		 if (SGetSBuf(region, pool, (Data **)&l2CellStats[cnt],
               				 (Size)sizeof (TSL2CellStatsCb)) != ROK)
      		 {
         			printf("\n STATS Unexpected MEM Alloc Failure\n");
      		 }
   	  }
        memset(l2CellStats[cnt], 0x00, (Size)sizeof(TSL2CellStatsCb));
   }

   for (cnt=0; cnt < L2_STATS_MAX_UES; cnt++)
   {
        TSL2UeStatsCb *statsCb = l2UeStats[cnt];
		  if(NULL == statsCb)
		  {
      		 if (SGetSBuf(region, pool, (Data **)&statsCb,
               				 (Size)sizeof (TSL2UeStatsCb)) != ROK)
      		 {
         		 	printf("\n STATS Unexpected MEM Alloc Failure at %d\n", (int)cnt);
      		 }
		  }
        memset(statsCb, 0x00, (Size)sizeof(TSL2UeStatsCb));
        statsCb->lnk.node = (PTR)statsCb;
        cmLListAdd2Tail(&freeL2UeStatsLst, &statsCb->lnk);
        l2UeStats[cnt] = statsCb;
   }

   RETVOID;
}

/*
*
*       Fun:   TSL2AllocUeStatsBlk
*
*       Desc:  Assign Stats Block for this UE[RNTI] 
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
TSL2UeStatsCb* TSL2AllocUeStatsBlk 
(
 U16   rnti
)
#else
TSL2UeStatsCb* TSL2AllocUeStatsBlk(rnti)
 U16   rnti;
#endif
{
   CmLList          *tmp = NULLP;
   TSL2UeStatsCb  *statsCb = NULLP;

   tmp = freeL2UeStatsLst.first;
   if (tmp == NULLP)
   {
      printf("\n STATS Unexpected Mem BLK unavailable for UE %d\n", rnti);
   }
   cmLListDelFrm(&freeL2UeStatsLst, tmp);
   statsCb = (TSL2UeStatsCb *)(tmp->node);
   cmLListAdd2Tail(&inUseL2UeStatsLst, tmp);

   statsCb->stats.rnti = (U32)rnti;
   statsCb->inUse = TRUE;

   return (statsCb);
}

/*
*
*       Fun:   TSL2DeallocUeStatsBlk
*
*       Desc:  Deassign Stats Block for this UE[RNTI] 
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
Void TSL2DeallocUeStatsBlk 
(
 U16              rnti,
 TSL2UeStatsCb  *statsCb
)
#else
Void TSL2DeallocUeStatsBlk(rnti, statsCb)
 U16              rnti;
 TSL2UeStatsCb  *statsCb;
#endif
{
   statsCb->inUse = FALSE;
   cmLListDelFrm(&inUseL2UeStatsLst, &statsCb->lnk);
   freeL2UeStatsLst.crnt = freeL2UeStatsLst.first;
   cmLListInsAfterCrnt(&freeL2UeStatsLst, &statsCb->lnk);

   RETVOID;
}

/*
*
*       Fun:   TSL2AllocCellStatsBlk
*
*       Desc:  Assign Stats Block for this CELL[CELLID] 
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
TSL2CellStatsCb* TSL2AllocCellStatsBlk 
(
 U32 cellId
)
#else
TSL2CellStatsCb* TSL2AllocCellStatsBlk(cellId)
 U32 cellId;
#endif
{
   if (cellId != 1)
   {
      printf("\n STATS Unexpected CellID = %d\n", (int)cellId);
   }

   return (l2CellStats[cellId-1]);
}

/*
*
*       Fun:   TSL2DeallocCellStatsBlk
*
*       Desc:  Deassign Stats Block for this CELL[CELLID] 
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
Void TSL2DeallocCellStatsBlk 
(
 U32 cellId
)
#else
Void TSL2DeallocCellStatsBlk(cellId)
 U32 cellId;
#endif
{

   RETVOID;
}

/*
*
*       Fun:   TSL2SendStatsToApp
*
*       Desc:  Collates and Sends STATS to Application 
*              Send UE STATS first. 10 UEs are grouped in one message.
*              Followed by CELL Stats. All CELLS are grouped in one msg.
*              At Reception of CELL stats APP assumes STATS reception cycle is complete.
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
Void TSL2SendStatsToApp
(
 Pst    *pst,
 SuId   suId
 )
#else
Void TSL2SendStatsToApp(pst, suId)
 Pst    *pst;
 SuId   suId;
#endif
{
   U32 idx;

   for (idx = 0; idx < L2_STATS_MAX_UES; idx++)
   {
      TSL2UeStatsCb *statsCb = l2UeStats[idx];
      U32 rnti;
      if (statsCb->inUse != TRUE)
      {
         continue;
      }
      if (pst->selector == 0)
      {
         /* Loose Coupling */
         TSInfPkSndL2UeStats(pst, suId, &statsCb->stats);
      }
      else
      {
#ifdef PX
         /* Tight Coupling */
         TSInfHdlL2UeStatsInd(pst, suId, &statsCb->stats);
#endif
      }
      rnti = statsCb->stats.rnti;
      memset(&statsCb->stats.nonPersistent, 0x00, (Size)sizeof(statsCb->stats.nonPersistent));
      /* memset(&statsCb->stats, 0x00, (Size)sizeof(TSInfL2UeStats)); */
      statsCb->stats.rnti = rnti;
   }

   /* Allocate mBuf for CELLSTATS */
   for (idx = 0; idx < L2_STATS_MAX_CELLS; idx++)
   {
      TSL2CellStatsCb *statsCellCb = l2CellStats[idx];
      U32 cellId;
      if (pst->selector == 0)
      {
         /* Loose Coupling */
         TSInfPkSndL2CellStats(pst, suId, l2CellStats[idx]);
      }
      else
      {
#ifdef PX
         /* Tight Coupling */
         TSInfHdlL2CellStatsInd(pst, suId, l2CellStats[idx]);
#endif
      }
      cellId = statsCellCb->cellId;
      memset(l2CellStats[idx], 0x00, (Size)sizeof(TSInfL2CellStats));
      statsCellCb->cellId = cellId;
   }
   RETVOID;
}
#endif /* TENB_STATS */
/**********************************************************************
         End of file
**********************************************************************/
