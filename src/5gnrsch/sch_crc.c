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

uint8_t schProcessCrcInd(CrcIndInfo *crcInd, Inst schInst)
{
   SchCellCb *cell = schCb[schInst].cells[schInst];
   uint16_t count=0;
   uint8_t  ueId=0;
   CmLList  *node = NULLP;
   SchUlHqProcCb *hqP = NULLP;

   while(count  <crcInd->numCrcInd)
   {
      GET_UE_ID(crcInd->crnti, ueId);
      if (cell->raCb[ueId-1].raState == SCH_RA_STATE_MSG3_PENDING)
      {
         SCH_ALLOC(node, sizeof(CmLList));
         node->node = (PTR)&(cell->raCb[ueId-1]);
         if (crcInd->crcInd[count])
         {
            /* failure case*/
            if (cell->raCb[ueId-1].msg3HqProc.tbInfo.txCntr < SCH_MAX_NUM_MSG3_TX)
            {
               cell->raCb[ueId - 1].retxMsg3HqProc = &cell->raCb[ueId - 1].msg3HqProc;
            }
         }
         else
         {
            /* pass case*/
            /*Dedicated preamble case need to be added*/
            cell->raCb[ueId-1].raState = SCH_RA_STATE_MSG4_PENDING;
            /*HARQ init part is in ADD UE CONFIG now, could be moved here*/
         }
      }
      else
      {
         if (cell->ueCb[ueId-1].hqUlmap[crcInd->timingInfo.slot]->hqList.count == 0)
         {
            DU_LOG("\n ERROR no harq stored in ul hq map at slot %d ue id %d\n",crcInd->timingInfo.slot, ueId);
            continue;
         }
         if (cell->ueCb[ueId-1].hqUlmap[crcInd->timingInfo.slot]->hqList.first == 0)
         {
            DU_LOG("\n ERROR NULL harq stored in ul hq map at slot %d ue id %d\n",crcInd->timingInfo.slot, ueId);
            continue;
         }
         hqP = (SchUlHqProcCb*) cell->ueCb[ueId-1].hqUlmap[crcInd->timingInfo.slot]->hqList.first->node;
         if(hqP == NULLP)
         {
            continue;
         }
         else
         {
            if (crcInd->crcInd[count])
            {             
               /* failure case*/
               schUlHqProcessNack(hqP);
               cmLListAdd2Tail(&cell->ueCb[ueId-1].ulRetxHqList, &hqP->ulHqProcLink);
            }
            else
            {
               /* pass case*/
               schUlHqProcessAck(hqP);
            }
         }
         cmLListDelFrm(&(cell->ueCb[ueId-1].hqUlmap[crcInd->timingInfo.slot]->hqList), &hqP->ulSlotLnk);
      }
      count++;
   }
   return ROK;
}
