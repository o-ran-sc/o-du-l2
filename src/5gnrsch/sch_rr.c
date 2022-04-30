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
//#include "mac_sch_interface.h"

uint8_t schMsg3RetxSchedulingForUe(SchRaCb *raCb)
{
   bool      k2Found = false;
#ifdef NR_TDD
   uint8_t   totalCfgSlot = 0;
#endif
   uint16_t             dciSlot = 0;
   SlotTimingInfo       dciTime, msg3Time;
   SchPuschInfo         *msg3PuschInfo = NULLP;   /* Stores MSG3 PUSCH scheduling information */
   RaRspWindowStatus    windowStatus=0;
   SchCellCb            *cell = NULLP;
   SlotTimingInfo       currTime;
   DciInfo  *dciInfo = NULLP;
   cell = raCb->cell;
   currTime = cell->slotInfo;

   /* Calculating time frame to send DCI for MSG3 Retx*/
   ADD_DELTA_TO_TIME(currTime, dciTime, PHY_DELTA_DL + SCHED_DELTA);
#ifdef NR_TDD
   /* Consider this slot for sending DCI, only if it is a DL slot */
   if(schGetSlotSymbFrmt(dciSlot, raCb->cell->slotFrmtBitMap) == DL_SLOT)
#endif
   {
      /* If PDCCH is already scheduled on this slot, cannot schedule PDSCH for another UE here. */
      if(cell->schDlSlotInfo[dciSlot]->pdcchUe != 0)
         return false;
      /* Check if this slot is within RA response window */
      windowStatus = isInRaRspWindow(raCb->raReq, dciTime, cell->numSlots);
      if(windowStatus == WITHIN_WINDOW)
      {
         k2Found = schGetMsg3K2(cell, &raCb->msg3HqProc, dciTime.slot, &msg3Time, TRUE);
      }
      else if(windowStatus == WINDOW_EXPIRED)
      {
         SCH_FREE(raCb->raReq->rachInd, sizeof(RachIndInfo));
         SCH_FREE(raCb->cell->raReq[raCb->ueId], sizeof(SchRaReq));
         raCb->raReq = NULLP;
         return RFAILED;
      }
      if (!k2Found)
      {
         return RFAILED;
      }
      SCH_ALLOC(dciInfo, sizeof(DciInfo));
      if(!dciInfo)
      {
         DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for dciInfo alloc");
         return RFAILED;
      }
      cell->schDlSlotInfo[msg3Time.slot]->ulGrant = dciInfo;
      memset(dciInfo,0,sizeof(DciInfo));
      msg3PuschInfo = cell->schUlSlotInfo[msg3Time.slot]->schPuschInfo;
      schFillUlDciForMsg3Retx(raCb, msg3PuschInfo, dciInfo);
   }
   raCb->retxMsg3HqProc = NULLP;
   return ROK;
}

bool schGetMsg3K2(SchCellCb *cell, SchUlHqProcCb* msg3HqProc, uint16_t dlTime, SlotTimingInfo *msg3Time, bool isRetx)
{
   bool      k2Found = false;
   uint8_t   k2TblIdx = 0;
   uint8_t   k2Index = 0;
   uint8_t   k2 = 0;
   uint8_t   numK2 = 0;
   uint8_t   puschMu = 0;
   uint8_t   msg3Delta = 0, msg3MinSchTime = 0;
#ifdef NR_TDD
   uint8_t   totalCfgSlot = 0;
#endif
   SchK2TimingInfoTbl   *msg3K2InfoTbl=NULLP;
   SlotTimingInfo       currTime, msg3TempTime;
   currTime = cell->slotInfo;
   puschMu = cell->cellCfg.numerology;

   if (isRetx)
   {
      if(!msg3HqProc)
         return false;

      numK2 = cell->cellCfg.schInitialUlBwp.k2InfoTbl.k2TimingInfo[dlTime].numK2;
      msg3MinSchTime = minMsg3SchTime[cell->cellCfg.numerology];
      msg3K2InfoTbl = &cell->cellCfg.schInitialUlBwp.msg3K2InfoTbl;
   }
   else
   {
      msg3Delta = puschDeltaTable[puschMu];
      numK2 = cell->cellCfg.schInitialUlBwp.msg3K2InfoTbl.k2TimingInfo[dlTime].numK2;
      msg3K2InfoTbl = &cell->cellCfg.schInitialUlBwp.k2InfoTbl;
   }

   for(k2TblIdx = 0; k2TblIdx < numK2; k2TblIdx++)
   {
      k2Index = msg3K2InfoTbl->k2TimingInfo[dlTime].k2Indexes[k2TblIdx];

      k2 = cell->cellCfg.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[k2Index].k2;
      if (isRetx)
      {
         if ((msg3HqProc->strtSymbl != cell->cellCfg.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[k2Index].startSymbol) ||
            (msg3HqProc->numSymbl != cell->cellCfg.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[k2Index].symbolLength))
         {
            continue;
         }
      }
      /* Delta is added to the slot allocation for msg3 based on 38.214 section 6.1.2.1 */
      k2 = k2 + msg3Delta;
      if(k2 >= msg3MinSchTime)
      {
         ADD_DELTA_TO_TIME(currTime, msg3TempTime, k2);
#ifdef NR_TDD
         if(schGetSlotSymbFrmt(msg3TempTime.slot % totalCfgSlot, cell->slotFrmtBitMap) == DL_SLOT)
            continue;
#endif
         /* If PUSCH is already scheduled on this slot, another PUSCH
          * pdu cannot be scheduled here */
         if(cell->schUlSlotInfo[msg3TempTime.slot]->puschUe != 0)
            continue;
         k2Found = true;
         break;
      }
   }
   if (k2Found == true)
   {
      msg3Time->slot = msg3TempTime.slot;
      msg3Time->sfn = msg3TempTime.sfn;
      msg3Time->slot = msg3TempTime.slot;
   }
   return k2Found;
}

uint8_t schMsg3RetxScheduling(CmLListCp *list)
{
   CmLList *node = NULLP;
   node = list->first;
   while(node != NULLP)
   {
      CmLList *current = node;
      SchRaCb* raCb = (SchRaCb*)current->node;
      node = node->next;
      if (ROK == schMsg3RetxSchedulingForUe(raCb))
      {
         deleteNodeFromLList(list, current);
      }
   }
   return ROK;
}

uint8_t schMsg4SchedulingForUe(SchUeCb *ueCb)
{
     if (schProcessMsg4Req(ueCb->cellCb, ueCb->cellCb->slotInfo, ueCb->ueId))
     {
        return ROK;
     }
     return RFAILED;
}
uint8_t schMsg4Scheduling(CmLListCp *list)
{
   CmLList *node = NULLP;
   node = list->first;
   while(node != NULLP)
   {
      CmLList *current = node;
      SchUeCb* ueCb = (SchUeCb*)current->node;
      node = node->next;
      if (ROK == schMsg4SchedulingForUe(ueCb))
      {
         deleteNodeFromLList(list, current);
      }
   }
   return ROK;
}
