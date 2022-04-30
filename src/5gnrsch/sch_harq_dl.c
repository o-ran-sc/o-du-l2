#include "common_def.h"
#include "tfu.h"
#include "lrg.h"
#include "tfu.x"
#include "lrg.x"
#include "du_log.h"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "sch.h"
#include "sch_utils.h"
#include "cm_llist.h"

typedef struct schCellCb SchCellCb;
typedef struct schUeCb SchUeCb;
void schDlHqEntReset(SchCellCb *cellCb, SchUeCb *ueCb, SchDlHqEnt *hqE);
void schDlHqAddToFreeList(SchDlHqProcCb *hqP);
void schDlHqEntInit(SchCellCb *cellCb, SchUeCb *ueCb)
{
   ueCb->dlHqEnt.numHqPrcs = SCH_MAX_NUM_DL_HQ_PROC;
   ueCb->dlHqEnt.maxHqTx  = SCH_MAX_NUM_DL_HQ_TX;
   ueCb->dlHqEnt.cell = cellCb;
   ueCb->dlHqEnt.ue =ueCb;
   schDlHqEntReset(cellCb, ueCb, &ueCb->dlHqEnt);
}
void schDlHqEntReset(SchCellCb *cellCb, SchUeCb *ueCb, SchDlHqEnt *hqE)
{
   uint8_t count = 0;
   SchDlHqProcCb *hqP = NULL;
   cmLListInit(&hqE->free);
   cmLListInit(&hqE->inUse);

   for(count=0; count < hqE->numHqPrcs; count++)
   {
      hqP = &(hqE->procs[count]);
      hqP->procId = count;
      hqP->hqEnt = hqE;
      hqP->maxHqTxPerHqP = hqE->maxHqTx;
      hqP->dlHqEntLnk.node = (PTR)hqP;
      schDlHqAddToFreeList(hqP);
   }
}

void schDlHqAddToFreeList(SchDlHqProcCb *hqP)
{
   cmLListAdd2Tail(&(hqP->hqEnt->free), &hqP->dlHqEntLnk);
}

void schDlHqDeleteFromFreeList(SchDlHqProcCb *hqP)
{
   cmLListDelFrm(&(hqP->hqEnt->free), &hqP->dlHqEntLnk);
}

void schDlHqAddToInUseList(SchDlHqProcCb *hqP)
{
   cmLListAdd2Tail(&(hqP->hqEnt->inUse), &hqP->dlHqEntLnk);
}

void schDlHqDeleteFromInUseList(SchDlHqProcCb *hqP)
{
   cmLListDelFrm(&(hqP->hqEnt->inUse), &hqP->dlHqEntLnk);
}

uint8_t schDlGetAvlHqProcess(SchCellCb *cellCb, SchUeCb *ueCb, SchDlHqProcCb **hqP)
{
   SchDlHqProcCb                  *tmp;
   tmp = (SchDlHqProcCb*)(cmLListFirst(&(ueCb->dlHqEnt.free))->node);
   if (NULLP == tmp)
   {
      return RFAILED;
   }
   schDlHqDeleteFromFreeList(tmp);
   schDlHqDeleteFromInUseList(tmp);
   *hqP = tmp;
   (*hqP)->maxHqTxPerHqP = ueCb->dlHqEnt.maxHqTx;
   return ROK;
}

uint8_t schDlEnableHarq(SchCellCb *cellCb, SchUeCb *ueCb)
{
   uint8_t count = 0;
   SchDlHqProcCb *hqP = NULLP;
   if (ueCb->dlHqEnt.free.count || ueCb->dlHqEnt.inUse.count)
   {
      return RFAILED;
   }
   for(count = 0;count < ueCb->dlHqEnt.numHqPrcs; count++)
   {
      hqP = &ueCb->dlHqEnt.procs[count];
      schDlHqAddToFreeList(hqP);
   }
   return ROK;
}

void schDlReleaseHqProcess(SchDlHqProcCb *hqP)
{
   cmLListDeleteLList(&hqP->dlLcPrbEst.dedLcList);
   cmLListDeleteLList(&hqP->dlLcPrbEst.defLcList);
   schDlHqDeleteFromInUseList(hqP);
   schDlHqAddToFreeList(hqP);

}

void schDlFreeUe(SchCellCb *cellCb, SchUeCb *ueCb)
{
   
}

void schDlReleaseHqPTb(SchDlHqProcCb *hqP, uint8_t tbIdx, bool togNdi)
{
   if (TRUE == togNdi)
   {
      hqP->tbInfo[tbIdx].ndi ^= 1;
   }
   else
   {
      hqP->tbInfo[tbIdx].isAckNackDtx = HQ_ACK;
      hqP->tbInfo[tbIdx].isEnabled = FALSE;
      hqP->tbInfo[tbIdx].state = HQ_TB_ACKED;
      hqP->tbInfo[tbIdx].txCntr = 0;
      if (HQ_TB_ACKED == hqP->tbInfo[tbIdx^1].state)
      {
         schDlReleaseHqProcess(hqP);
      }
   }
}

void schDlHqTbFail(SchDlHqProcCb *hqP, uint8_t tbIdx, bool isMaxRetx)
{
   if (isMaxRetx)
   {
      schDlReleaseHqPTb(hqP, tbIdx, TRUE);
   }
   else
   {
      hqP->tbInfo[tbIdx].state = HQ_TB_NACKED;
      if (HQ_TB_WAITING == hqP->tbInfo[tbIdx^1].state)
      {
         CmLList         *node = NULLP;
         SCH_ALLOC(node, sizeof(CmLList));
         node->node = (PTR)(hqP); 
         cmLListAdd2Tail( &(schCb[hqP->hqEnt->cell->instIdx].dlPrioLst[SCH_DL_HQ_RETX_QUEUE]), node);
      }
   }
}

void schMsg4FeedbackUpdate(SchDlHqProcCb *hqP, uint8_t fdbk)
{
   hqP->tbInfo[0].isAckNackDtx = fdbk;
   if (HQ_TB_ACKED == hqP->tbInfo[0].isAckNackDtx)
   {
      schDlReleaseHqPTb(hqP, 0, TRUE);
      
   }
}
void schDlHqFeedbackUpdate(SchDlHqProcCb *hqP, uint8_t fdbk1, uint8_t fdbk2)
{
   uint8_t tbIdx;   
   for (tbIdx = 0; tbIdx <2; tbIdx++)
   {
      if (HQ_TB_WAITING == hqP->tbInfo[tbIdx].state)
      {
         hqP->tbInfo[tbIdx].isAckNackDtx = (0 == tbIdx)?fdbk1:fdbk2;
      }
      if (TRUE == hqP->tbInfo[tbIdx].isEnabled)
      {
         if (HQ_TB_ACKED == hqP->tbInfo[tbIdx].isAckNackDtx)
         {
            schDlReleaseHqPTb(hqP, tbIdx, TRUE);               
         }
         else
         {
            if(hqP->tbInfo[tbIdx].txCntr >= hqP->maxHqTxPerHqP)
            {
               schDlHqTbFail(hqP, tbIdx, TRUE);
            }

         }
      }
   }
}
