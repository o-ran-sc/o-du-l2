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
typedef struct schCellCb SchCellCb;
typedef struct schUeCb SchUeCb;
void schUlHqEntReset(SchCellCb *cellCb, SchUeCb *ueCb, SchUlHqEnt *hqE);
void schUlHqAddToFreeList(SchUlHqProcCb *hqP);

void schUlHqEntInit(SchCellCb *cellCb, SchUeCb *ueCb)
{
   ueCb->ulHqEnt.numHqPrcs = SCH_MAX_NUM_UL_HQ_PROC;
   ueCb->ulHqEnt.maxHqTx  = SCH_MAX_NUM_UL_HQ_TX;
   ueCb->ulHqEnt.cell = cellCb;
   ueCb->ulHqEnt.ue =ueCb;
   schUlHqEntReset(cellCb, ueCb, &ueCb->ulHqEnt);
}
void schUlHqEntReset(SchCellCb *cellCb, SchUeCb *ueCb, SchUlHqEnt *hqE)
{
   uint8_t count = 0;
   SchUlHqProcCb *hqP = NULL;
   cmLListInit(&hqE->free);
   cmLListInit(&hqE->inUse);

   for(count=0; count < hqE->numHqPrcs; count++)
   {
      hqP = &(hqE->procs[count]);
      hqP->procId = count;
      hqP->hqEnt = hqE;
      hqP->maxHqTxPerHqP = hqE->maxHqTx;
      hqP->ulHqEntLnk.node = (PTR)hqP;
      hqP->ulHqProcLink.node = (PTR)hqP;
      hqP->ulSlotLnk.node = (PTR)hqP;
      schUlHqAddToFreeList(hqP);
   }
}

void schUlHqAddToFreeList(SchUlHqProcCb *hqP)
{
   cmLListAdd2Tail(&(hqP->hqEnt->free), &hqP->ulHqEntLnk);
}

void schUlHqDeleteFromFreeList(SchUlHqProcCb *hqP)
{
   cmLListDelFrm(&(hqP->hqEnt->free), &hqP->ulHqEntLnk);
}

void schUlHqAddToInUseList(SchUlHqProcCb *hqP)
{
   cmLListAdd2Tail(&(hqP->hqEnt->free), &hqP->ulHqEntLnk);
}

void schUlHqDeleteFromInUseList(SchUlHqProcCb *hqP)
{
   cmLListDelFrm(&(hqP->hqEnt->free), &hqP->ulHqEntLnk);
}

uint8_t schUlGetAvlHqProcess(SchCellCb *cellCb, SchUeCb *ueCb, SchUlHqProcCb **hqP)
{
   SchUlHqProcCb                  *tmp;
   if (ueCb->ulHqEnt.free.count == 0)
   {
      return RFAILED;
   }
   tmp = (SchUlHqProcCb*)(cmLListFirst(&(ueCb->ulHqEnt.free))->node);
   if (NULLP == tmp)
   {
      return RFAILED;
   }
   schUlHqDeleteFromFreeList(tmp);
   schUlHqAddToInUseList(tmp);
   *hqP = tmp;
   (*hqP)->maxHqTxPerHqP = ueCb->ulHqEnt.maxHqTx;
   return ROK;
}
/*
void schUlFreeUe(SchCellCb *cellCb, SchUeCb *ueCb)
{
}
*/
void schUlReleaseHqProcess(SchUlHqProcCb *hqP, Bool togNdi)
{
   cmLListDeleteLList(&hqP->ulLcPrbEst.dedLcList);
   cmLListDeleteLList(&hqP->ulLcPrbEst.defLcList);
   schUlHqDeleteFromInUseList(hqP);
   schUlHqAddToFreeList(hqP);
}

void schUlHqProcessNack(SchUlHqProcCb *hqP)
{
   if (hqP->tbInfo.txCntr < hqP->maxHqTxPerHqP)
   {
      cmLListAdd2Tail(&(hqP->hqEnt->ue->ulRetxHqList), &hqP->ulHqProcLink);
   }
   else
   {
      schUlReleaseHqProcess(hqP, TRUE);
   }
}

void schUlHqProcessAck(SchUlHqProcCb *hqP)
{
   schUlReleaseHqProcess(hqP, TRUE);
}