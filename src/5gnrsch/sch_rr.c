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
/**
 * @brief Handles retransmission for MSG3
 *
 * @details
 *
 *     Function : schMsg3RetxSchedulingForUe
 *      
 *      This function handles retransmission for MSG3
 *           
 *  @param[in]  SchRaCb *raCb, RA cb pointer
 *  @return  
 *      -# ROK
 *      -# RFAILED
 **/
uint8_t schMsg3RetxSchedulingForUe(SchRaCb *raCb)
{
   bool      k2Found = false;
#ifdef NR_TDD
   uint8_t   totalCfgSlot = 0;
#endif
   uint16_t             dciSlot = 0;
   SlotTimingInfo       dciTime, msg3Time;
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

      k2Found = schGetMsg3K2(cell, &raCb->msg3HqProc, dciTime.slot, &msg3Time, TRUE);

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
      SCH_ALLOC(cell->schUlSlotInfo[msg3Time.slot]->schPuschInfo, sizeof(SchPuschInfo));
      memset(dciInfo,0,sizeof(DciInfo));
      schFillUlDciForMsg3Retx(raCb, cell->schUlSlotInfo[msg3Time.slot]->schPuschInfo, dciInfo);
   }   
   raCb->retxMsg3HqProc = NULLP;
   return ROK;
}
/**
 * @brief Get K2 value for MSG3
 *
 * @details
 *
 *     Function : schGetMsg3K2
 *      
 *      This function gets K2 for MSG3
 *           
 *  @param[in]  SchCellCb *cell, Cell cb struc pointer
 *  @param[in]  SchUlHqProcCb* msg3HqProc, msg3 harq proc pointer
 *  @param[in]  uint16_t dlTime, DL time of scheduling
 *  @param[in]  SlotTimingInfo *msg3Time, MSG3 timing info
 *  @param[in]  bool isRetx, indicates MSG3 retransmission
 *  @return  
 *      -# true
 *      -# false
 **/
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
      msg3K2InfoTbl = &cell->cellCfg.schInitialUlBwp.msg3K2InfoTbl;      
      msg3MinSchTime = 0;
      msg3Delta = 0;
   }
   else
   {
      numK2 = cell->cellCfg.schInitialUlBwp.msg3K2InfoTbl.k2TimingInfo[dlTime].numK2;
      msg3K2InfoTbl = &cell->cellCfg.schInitialUlBwp.k2InfoTbl;
      msg3MinSchTime = minMsg3SchTime[cell->cellCfg.numerology];
      msg3Delta = puschDeltaTable[puschMu];      
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
/**********************************************************************
  End of file
 **********************************************************************/