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


SchMacDlReleaseHarqFunc schMacDlReleaseHarqOpts[] =
{
   packSchMacDlReleaseHarq,
   MacSchReleaseDlHarqProc,
   packSchMacDlReleaseHarq
};

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
      hqP->dlHqProcLink.node = (PTR)hqP;
      hqP->ulSlotLnk.node = (PTR)hqP;
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
   if (ueCb->dlHqEnt.free.count == 0)
   {
      return RFAILED;
   }
   tmp = (SchDlHqProcCb*)(cmLListFirst(&(ueCb->dlHqEnt.free))->node);
   if (NULLP == tmp)
   {
      return RFAILED;
   }
   schDlHqDeleteFromFreeList(tmp);
   schDlHqAddToInUseList(tmp);
   *hqP = tmp;
   (*hqP)->maxHqTxPerHqP = ueCb->dlHqEnt.maxHqTx;
   return ROK;
}
/*
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
*/
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


/*******************************************************************
 *
 * @brief Handles sending DL HARQ process release to MAC 
 *
 * @details
 *
 *    Function : sendDlHarqProcReleaseToMac
 *
 *    Functionality:
 *     Sends DL DL HARQ process release to MAC from SCH
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sendDlHarqProcReleaseToMac(SchDlHqProcCb *hqP, Inst inst)
{
   Pst pst;
   SchRlsHqInfo *rlsHqInfo;
   memset(&pst, 0, sizeof(Pst));
   FILL_PST_SCH_TO_MAC(pst, inst);
   pst.event = EVENT_DL_REL_HQ_PROC;

   SCH_ALLOC(rlsHqInfo, sizeof(SchRlsHqInfo));
   rlsHqInfo->cellId = hqP->hqEnt->cell->cellId;
   rlsHqInfo->numUes = 1;

   SCH_ALLOC(rlsHqInfo->ueHqInfo, sizeof(SchUeHqInfo)*rlsHqInfo->numUes);
   rlsHqInfo->ueHqInfo[0].crnti = hqP->hqEnt->ue->crnti;
   rlsHqInfo->ueHqInfo[0].hqProcId = hqP->procId;   

   return(*schMacDlReleaseHarqOpts[pst.selector])(&pst, rlsHqInfo);
}
void schDlReleaseHqPTb(SchDlHqProcCb *hqP, uint8_t tbIdx, bool togNdi)
{
   if (TRUE == togNdi)
   {
      hqP->tbInfo[tbIdx].ndi ^= 1;
   }

   {
      hqP->tbInfo[tbIdx].isAckNackDtx = HQ_ACK;
      hqP->tbInfo[tbIdx].isEnabled = FALSE;
      hqP->tbInfo[tbIdx].state = HQ_TB_ACKED;
      hqP->tbInfo[tbIdx].txCntr = 0;
      if (HQ_TB_ACKED == hqP->tbInfo[tbIdx^1].state)
      {
         schDlReleaseHqProcess(hqP);
         sendDlHarqProcReleaseToMac(hqP, hqP->hqEnt->cell->instIdx);
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
         cmLListAdd2Tail( &(hqP->hqEnt->ue->dlRetxHqList), &hqP->dlHqProcLink);
      }
   }
}

void schMsg4FeedbackUpdate(SchDlHqProcCb *hqP, uint8_t fdbk)
{
   hqP->tbInfo[0].isAckNackDtx = fdbk;
   hqP->tbInfo[1].isAckNackDtx = HQ_TB_ACKED;
   if (HQ_TB_ACKED == hqP->tbInfo[0].isAckNackDtx)
   {
      schDlReleaseHqPTb(hqP, 0, TRUE);
      schDlReleaseHqPTb(hqP, 1, TRUE);
      schMsg4Complete(hqP->hqEnt->ue);
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
            else
            {
               schDlHqTbFail(hqP, tbIdx, FALSE);
            }
         }
      }
   }
}