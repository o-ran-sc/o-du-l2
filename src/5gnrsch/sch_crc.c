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
   SchUlHqProcCb *hqP = NULLP;

   while(count  <crcInd->numCrcInd)
   {
      GET_UE_ID(crcInd->crnti, ueId);
      if (cell->raCb[ueId-1].raState == SCH_RA_STATE_MSG3_PENDING)
      {
         if (crcInd->crcInd[count])
         {
            /* failure case*/
            if (cell->raCb[ueId-1].msg3HqProc.tbInfo.txCntr < cell->cellCfg.schRachCfg.maxMsg3Tx)
            {
               addUeToBeScheduled(cell, ueId);
               cell->raCb[ueId - 1].retxMsg3HqProc = &cell->raCb[ueId - 1].msg3HqProc;
               HQ_LOG(1,cell->raCb[ueId-1].msg3HqProc.procId,"\nANKIT schProcessCrcInd FAIL - state SCH_RA_STATE_MSG3_PENDING ueId[%2d] txCntr[%2d] maxMsg3Tx[%2d]\n",\
                  ueId, cell->raCb[ueId-1].msg3HqProc.tbInfo.txCntr, cell->cellCfg.schRachCfg.maxMsg3Tx);
            }
            else
            {
               /*Release all contexts of the UE RA*/
               HQ_LOG(1,cell->raCb[ueId-1].msg3HqProc.procId,"\nANKIT schProcessCrcInd FAIL - state SCH_RA_STATE_MSG3_PENDING MAX TX Reached\n");
            }
         }
         else
         {
            /* pass case*/
            /*Dedicated preamble case need to be added*/
            cell->raCb[ueId-1].raState = SCH_RA_STATE_MSG4_PENDING;
            /*HARQ init part is in ADD UE CONFIG now, could be moved here*/
            HQ_LOG(1,cell->raCb[ueId-1].msg3HqProc.procId,"\nANKIT schProcessCrcInd PASS - state SCH_RA_STATE_MSG3_PENDING\n");
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
               HQ_LOG(1,hqP->procId,"\nANKIT schProcessCrcInd FAIL - state[%d] proc[%2d]\n",cell->raCb[ueId-1].raState, hqP->procId);
            }
            else
            {
               /* pass case*/
               schUlHqProcessAck(hqP);
               HQ_LOG(1,hqP->procId,"\nANKIT schProcessCrcInd PASS - state[%d] proc[%2d]\n",cell->raCb[ueId-1].raState, hqP->procId);
            }
         }
         cmLListDelFrm(&(cell->ueCb[ueId-1].hqUlmap[crcInd->timingInfo.slot]->hqList), &hqP->ulSlotLnk);
      }
      count++;
   }
   return ROK;
}
