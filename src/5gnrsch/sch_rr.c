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
   uint8_t   k2TblIdx = 0;
   uint8_t   k2Index = 0;
   uint8_t   k2 = 0;
   uint8_t   puschMu = 0;
   uint8_t   msg3Delta = 0, msg3MinSchTime = 0;
#ifdef NR_TDD
   uint8_t   totalCfgSlot = 0;
#endif
   //SchUlHqProcCb        *hqP = NULLP;
   uint16_t             dciSlot = 0, rarSlot = 0;
   SlotTimingInfo       dciTime, msg3Time;
   SchPuschInfo         *msg3PuschInfo = NULLP;   /* Stores MSG3 PUSCH scheduling information */
   SchK2TimingInfoTbl   *msg3K2InfoTbl=NULLP;
   RaRspWindowStatus    windowStatus=0;
   SchCellCb            *cell = NULLP;
   SlotTimingInfo       currTime;
   DciInfo  *dciInfo = NULLP;
   cell = raCb->cell;
   currTime = cell->slotInfo;
   msg3K2InfoTbl = &cell->cellCfg.schInitialUlBwp.msg3K2InfoTbl;
   puschMu = cell->cellCfg.numerology;
   msg3Delta = puschDeltaTable[puschMu];
   msg3MinSchTime = minMsg3SchTime[cell->cellCfg.numerology];

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
         {
            for(k2TblIdx = 0; k2TblIdx < msg3K2InfoTbl->k2TimingInfo[rarSlot].numK2; k2TblIdx++)
            {
               k2Index = msg3K2InfoTbl->k2TimingInfo[rarSlot].k2Indexes[k2TblIdx];
               k2 = cell->cellCfg.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[k2Index].k2;

               /* Delta is added to the slot allocation for msg3 based on 38.214 section 6.1.2.1 */
               k2 = k2 + msg3Delta;
               if(k2 >= msg3MinSchTime)
               {
                  ADD_DELTA_TO_TIME(currTime, msg3Time, k2);
#ifdef NR_TDD
                  if(schGetSlotSymbFrmt(msg3Time.slot % totalCfgSlot, cell->slotFrmtBitMap) == DL_SLOT)
                     continue;
#endif
                  /* If PUSCH is already scheduled on this slot, another PUSCH
                   * pdu cannot be scheduled here */
                  if(cell->schUlSlotInfo[msg3Time.slot]->puschUe != 0)
                     continue;
                  k2Found = true;
                  break;
               }
            }
         }         
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
   return ROK;
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

