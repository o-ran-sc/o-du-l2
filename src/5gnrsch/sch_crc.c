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
#include "sch_tmr.h"
#include "sch_utils.h"

/**
 * @brief Process CRC indication
 *
 * @details
 *
 *     Function : SchProcCrcInd
 *      
 *      This function process CRC indication
 *           
 *  @param[in]  CrcIndInfo *crcInd, recvd crc indication
 *  @param[in]  Inst schInst, scheduler inst
 *  @return  
 *      -# ROK
 *      -# RFAILED
 **/
uint8_t SchProcCrcInd(Pst *pst, CrcIndInfo *crcInd)
{
   uint16_t count=0;
   uint8_t  ueId=0;
   SchUlHqProcCb *hqP = NULLP;
   Inst  schInst = pst->dstInst - SCH_INST_START;
   SchCellCb *cell = schCb[schInst].cells[schInst];
   
   while(count  <crcInd->numCrcInd)
   {
      GET_UE_ID(crcInd->crnti, ueId);
      if (cell->raCb[ueId-1].raState == SCH_RA_STATE_MSG3_PENDING)
      {
         if (crcInd->crcInd[count])
         {
            /* failure case*/
            if (cell->raCb[ueId-1].msg3HqProc.tbInfo.txCntr < cell->maxMsg3Tx)
            {
               cell->api->SchCrcInd(cell, ueId);
               cell->raCb[ueId - 1].retxMsg3HqProc = &cell->raCb[ueId - 1].msg3HqProc;
            }
            else
            {
               /*Release all contexts of the UE RA*/
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
/**********************************************************************
  End of file
 **********************************************************************/
