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

/************************************************************************

Name:     5G NR SCH layer

Type:     C source file

Desc:     C source code for Entry point fucntions for slot indications

File:     sch_slot_ind.c

 **********************************************************************/

/** @file sch_slot_ind.c
  @brief This module processes slot indications
 */
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

SchMacDlAllocFunc schMacDlAllocOpts[] =
{
   packSchMacDlAlloc,
   MacProcDlAlloc,
   packSchMacDlAlloc
};

SchCb schCb[SCH_MAX_INST];

/*******************************************************************
 *
 * @brief Handles sending DL broadcast alloc to MAC 
 *
 * @details
 *
 *    Function : sendDlAllocToMac
 *
 *    Functionality:
 *     Sends DL Broadcast Resource Allocation to MAC from SCH
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sendDlAllocToMac(DlSchedInfo *dlSchedInfo, Inst inst)
{
   Pst pst;

   memset(&pst, 0, sizeof(Pst));
   FILL_PST_SCH_TO_MAC(pst, inst);
   pst.event = EVENT_DL_SCH_INFO;

   return(*schMacDlAllocOpts[pst.selector])(&pst, dlSchedInfo);

}

/*******************************************************************
 *
 * @brief Handles slot indication at SCH 
 *
 * @details
 *
 *    Function : schCalcSlotValues
 *
 *    Functionality:
 *     Handles TTI indication received from PHY
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void schCalcSlotValues(SlotTimingInfo slotInd, SchSlotValue *schSlotValue)
{
   /****************************************************************
    * PHY_DELTA - the physical layer delta                         * 
    * SCHED_DELTA - scheduler schedules one slot ahead             *
    * BO_DELTA - this delay is considered for BO response and      *
    *            RLC buffer packet to received at MAC              *
    * lower-mac (FAPI filling) will be working on PHY_DELTA        *
    * brdcast scheduler will working on PHY_DELTA + SCHED_DELTA    *
    * RAR scheduler will working on PHY_DELTA + SCHED_DELTA        *
    * msg4 scheduler will working on PHY_DELTA + SCHED_DELTA       *
    * dedicated DL msg scheduler will working                      *
    *        on PHY_DELTA + SCHED_DELTA + BO_DELTA                 *
    ****************************************************************/

   ADD_DELTA_TO_TIME(slotInd, schSlotValue->currentTime, PHY_DELTA_DL);
   ADD_DELTA_TO_TIME(slotInd, schSlotValue->broadcastTime, PHY_DELTA_DL + SCHED_DELTA);
   ADD_DELTA_TO_TIME(slotInd, schSlotValue->rarTime, PHY_DELTA_DL + SCHED_DELTA);
   ADD_DELTA_TO_TIME(slotInd, schSlotValue->dlMsgTime, PHY_DELTA_DL + SCHED_DELTA);
}

/*******************************************************************
 *
 * @brief Checks if a slot is to be scheduled for SSB transmission
 *
 * @details
 *
 *    Function : schCheckSsbOcc 
 *
 *    Functionality:
 *       Checks if a slot is to be scheduled for SSB transmission
 *
 * @params[in] SlotTimingInfo slotTime
 *             SchCellCb *cell 
 * @return  Pdu transmission 
 *
 * ****************************************************************/
PduTxOccsaion schCheckSsbOcc(SlotTimingInfo slotTime, SchCellCb *cell)
{
   uint8_t  ssb_rep;

   ssb_rep = cell->cellCfg.ssbSchCfg.ssbPeriod;

   /* Identify SSB ocassion*/
   if ((slotTime.sfn % SCH_MIB_TRANS == 0) && (slotTime.slot ==0))
   {
      return NEW_TRANSMISSION;
   }
   else if(cell->firstSsbTransmitted) 
   {
      if((ssb_rep == 5) && ((slotTime.slot == 0 || slotTime.slot == 10)))
         return REPEATITION;
      else if((slotTime.sfn % (ssb_rep/10) == 0) && slotTime.slot == 0)
         return REPEATITION;
   }
   /* not SSB occassion */
   return NO_TRANSMISSION;
}

/*******************************************************************
 *
 * @brief Checks if a slot is to be scheduled for SIB1 transmission
 *
 * @details
 *
 *    Function : schCheckSib1Occ
 *
 *    Functionality:
 *       Checks if a slot is to be scheduled for SIB1 transmission
 *
 * @params[in] SlotTimingInfo slotTime
 *             SchCellCb *cell
 * @return  Pdu transmission
 *
 * ****************************************************************/
PduTxOccsaion schCheckSib1Occ(SlotTimingInfo slotTime, SchCellCb *cell)
{
   /* Identify SIB1 occasions */
   if((slotTime.sfn % SCH_SIB1_TRANS == 0) && (slotTime.slot ==0))
   {
      return NEW_TRANSMISSION;
   }
   else if(cell->firstSib1Transmitted) 
   {
      if((slotTime.sfn % (cell->cellCfg.sib1SchCfg.sib1RepetitionPeriod/10) == 0) &&
            (slotTime.slot == 0))
      {
         return REPEATITION;
      }
   }
   /* not SIB1 occassion */
   return NO_TRANSMISSION;
}

/*******************************************************************
 *
 * @brief 
 *
 * @details
 *
 *    Function : 
 *
 *    Functionality:
 
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t schFillBoGrantDlSchedInfo(SchCellCb *cell, DlSchedInfo *dlSchedInfo, DlMsgAlloc *dlMsgAlloc)
{
   uint8_t ueIdx, lcIdx;
   uint16_t slot;
   uint16_t crnti = 0;
   uint32_t accumalatedSize = 0;
   SchUeCb *ueCb = NULLP;

   while(cell->boIndBitMap)
   {
      slot = dlSchedInfo->schSlotValue.dlMsgTime.slot;

      GET_RIGHT_MOST_SET_BIT(cell->boIndBitMap, ueIdx);
      GET_CRNTI(crnti,ueIdx);
      ueCb = &cell->ueCb[ueIdx-1];

      /* allocate PDCCH and PDSCH resources for the ue */
      SCH_ALLOC(dlMsgAlloc, sizeof(DlMsgAlloc));
      if(!dlMsgAlloc)
      {
         DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for ded DL msg alloc");
         return RFAILED;
      }
      memset(dlMsgAlloc, 0, sizeof(DlMsgAlloc));
      dlSchedInfo->dlMsgAlloc = dlMsgAlloc;
      dlMsgAlloc->crnti = crnti;

      /* Dl ded Msg info is copied, this was earlier filled in macSchDlRlcBoInfo */
      memcpy(&dlMsgAlloc->dlMsgInfo, cell->schDlSlotInfo[slot]->dlMsgInfo, \
	    sizeof(DlMsgInfo));

      /* scheduled LC data fill */
      dlMsgAlloc->numLc = 0;
      for(lcIdx = 0; lcIdx < MAX_NUM_LC; lcIdx++)
      {
         if(ueCb->dlInfo.dlLcCtxt[lcIdx].bo)
         {
            dlMsgAlloc->lcSchInfo[dlMsgAlloc->numLc].lcId = lcIdx;

            /* calculation for BO includse RLC and MAC header size */
            dlMsgAlloc->lcSchInfo[dlMsgAlloc->numLc].schBytes = \
                                                                ueCb->dlInfo.dlLcCtxt[lcIdx].bo + MAC_HDR_SIZE;
            accumalatedSize += dlMsgAlloc->lcSchInfo[dlMsgAlloc->numLc].schBytes;
            dlMsgAlloc->numLc++;
         }
         ueCb->dlInfo.dlLcCtxt[lcIdx].bo = 0;
      }
      
      if (!dlMsgAlloc->numLc)
      {
         DU_LOG("\nDEBUG  -->  SCH : No pending BO for any LC id\n");
         /* Free the dl ded msg info allocated in macSchDlRlcBoInfo */
         SCH_FREE(cell->schDlSlotInfo[dlSchedInfo->schSlotValue.dlMsgTime.slot]->dlMsgInfo, \
               sizeof(DlMsgInfo));
         return ROK;
      }

      /* pdcch and pdsch data is filled */
      schDlRsrcAllocDlMsg(dlMsgAlloc, cell, crnti, &accumalatedSize, slot);

      /* TODO : Update the scheduling byte report for multiple LC based on QCI
       * and Priority */
      /* As of now, the total number of bytes scheduled for a slot is divided
       * equally amongst all LC with pending data. This is avoid starving of any
       * LC 
       * */
      accumalatedSize = accumalatedSize/dlMsgAlloc->numLc;
      for(lcIdx = 0; lcIdx < dlMsgAlloc->numLc; lcIdx ++)
         dlMsgAlloc->lcSchInfo[lcIdx].schBytes = accumalatedSize;

      /* PUCCH resource */
      schAllocPucchResource(cell, dlMsgAlloc->crnti, slot);

      /* Free the dl ded msg info allocated in macSchDlRlcBoInfo */
      SCH_FREE(cell->schDlSlotInfo[dlSchedInfo->schSlotValue.dlMsgTime.slot]->dlMsgInfo, \
	    sizeof(DlMsgInfo));
      cell->schDlSlotInfo[dlSchedInfo->schSlotValue.dlMsgTime.slot]->dlMsgInfo = NULL;

      /* after allocation is done, unset the bo bit for that ue */
      UNSET_ONE_BIT(ueIdx, cell->boIndBitMap);
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Handles slot indication at SCH 
 *
 * @details
 *
 *    Function : schProcessSlotInd
 *
 *    Functionality:
 *     Handles TTI indication received from PHY
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t schProcessSlotInd(SlotTimingInfo *slotInd, Inst schInst)
{
   uint8_t  ueIdx, lcgIdx, ret = ROK;
   uint16_t slot;
   DlSchedInfo dlSchedInfo;
   DlBrdcstAlloc *dlBrdcstAlloc = NULLP;
   DlMsgAlloc  *msg4Alloc = NULLP;
   DlMsgAlloc *dlMsgAlloc = NULLP;
   SchCellCb  *cell = NULLP;

   memset(&dlSchedInfo,0,sizeof(DlSchedInfo));
   dlSchedInfo.dlMsgAlloc = NULLP;
   schCalcSlotValues(*slotInd, &dlSchedInfo.schSlotValue);
   dlBrdcstAlloc = &dlSchedInfo.brdcstAlloc;
   dlBrdcstAlloc->ssbTrans = NO_TRANSMISSION;
   dlBrdcstAlloc->sib1Trans = NO_TRANSMISSION;

   cell = schCb[schInst].cells[schInst];
   if(cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : Cell Does not exist");
      return RFAILED;
   }
   memcpy(&cell->slotInfo, slotInd, sizeof(SlotTimingInfo));
   dlBrdcstAlloc->ssbIdxSupported = SSB_IDX_SUPPORTED;

   dlSchedInfo.cellId = cell->cellId;

   slot = dlSchedInfo.schSlotValue.broadcastTime.slot;

   /* Check for SSB occassion */
   dlBrdcstAlloc->ssbTrans = schCheckSsbOcc(dlSchedInfo.schSlotValue.broadcastTime, cell); 
   if(dlBrdcstAlloc->ssbTrans)
   {
      if(schBroadcastSsbAlloc(cell,dlBrdcstAlloc,slot) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : schBroadcastSsbAlloc failed");
         dlBrdcstAlloc->ssbTrans = NO_TRANSMISSION;
      }
      else 
      {
         dlSchedInfo.isBroadcastPres = true;
         if((dlBrdcstAlloc->ssbTrans == NEW_TRANSMISSION) && (!cell->firstSsbTransmitted))
            cell->firstSsbTransmitted = true;
      }
   }

   /* Check for SIB1 occassion */
   dlBrdcstAlloc->sib1Trans = schCheckSib1Occ(dlSchedInfo.schSlotValue.broadcastTime, cell);
   if(dlBrdcstAlloc->sib1Trans)
   {
      if(schBroadcastSib1Alloc(cell,dlBrdcstAlloc,slot) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : schBroadcastSib1Alloc failed");
         dlBrdcstAlloc->sib1Trans = NO_TRANSMISSION;
      }
      else 
      {
         dlSchedInfo.isBroadcastPres = true;
         if((dlBrdcstAlloc->sib1Trans == NEW_TRANSMISSION) && (!cell->firstSib1Transmitted))
            cell->firstSib1Transmitted = true;
      }
   }

   /* Check for Pending RA Requests */
   schProcessRaReq(*slotInd, cell);

   /* Check for RAR */
   if(cell->schDlSlotInfo[dlSchedInfo.schSlotValue.rarTime.slot]->rarAlloc != NULLP)
   {
      slot = dlSchedInfo.schSlotValue.rarTime.slot;
      dlSchedInfo.rarAlloc = cell->schDlSlotInfo[slot]->rarAlloc;
      cell->schDlSlotInfo[slot]->rarAlloc = NULLP;
   }

   /* Check for MSG4 */
   if((cell->schDlSlotInfo[dlSchedInfo.schSlotValue.dlMsgTime.slot]->dlMsgInfo != NULLP) &&
      (cell->schDlSlotInfo[dlSchedInfo.schSlotValue.dlMsgTime.slot]->dlMsgInfo->isMsg4Pdu))
   {
      slot = dlSchedInfo.schSlotValue.dlMsgTime.slot;

      SCH_ALLOC(msg4Alloc, sizeof(DlMsgAlloc));
      if(!msg4Alloc)
      {
         DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for msg4 alloc");
         return RFAILED;
      }

      dlSchedInfo.dlMsgAlloc = msg4Alloc;

      /* Msg4 info is copied, this was earlier filled in macSchDlRlcBoInfo */
      memcpy(&msg4Alloc->dlMsgInfo, cell->schDlSlotInfo[slot]->dlMsgInfo, \
	    sizeof(DlMsgInfo));

      /* pdcch and pdsch data is filled */
      schDlRsrcAllocMsg4(msg4Alloc, cell, dlSchedInfo.schSlotValue.dlMsgTime.slot, dlBrdcstAlloc->ssbTrans, dlBrdcstAlloc->sib1Trans); 

      /* PUCCH resource */
      schAllocPucchResource(cell, msg4Alloc->dlMsgInfo.crnti, dlSchedInfo.schSlotValue.dlMsgTime.slot);

      SCH_FREE(cell->schDlSlotInfo[dlSchedInfo.schSlotValue.dlMsgTime.slot]->dlMsgInfo, sizeof(DlMsgInfo));
      cell->schDlSlotInfo[dlSchedInfo.schSlotValue.dlMsgTime.slot]->dlMsgInfo = NULL;
   }

   /* Check if UL grant must be sent in this slot for a SR/BSR that had been received */
   for(ueIdx=0; ueIdx<cell->numActvUe; ueIdx++)
   {
      uint32_t totDataReq = 0; /* in bytes */
      DciInfo  *dciInfo = NULLP;
      SchUeCb *ueCb = NULLP;
   
      ueCb = &cell->ueCb[ueIdx];
      /* check for SR */
      if(ueCb->srRcvd)
      {
         totDataReq = UL_GRANT_SIZE; /*fixing so that all control msgs can be handled in SR */
         ueCb->srRcvd = false;
      }
      /* check for BSR */
      for(lcgIdx=0; lcgIdx<MAX_NUM_LOGICAL_CHANNEL_GROUPS; lcgIdx++)
      {
        totDataReq+= ueCb->bsrInfo[lcgIdx].dataVol;
        ueCb->bsrInfo[lcgIdx].dataVol = 0;
      }
      if(totDataReq > 0) /* UL grant must be provided for this UE in this slot */
      {
         SchPuschInfo schPuschInfo;
         memset(&schPuschInfo, 0, sizeof(SchPuschInfo));

         SCH_ALLOC(dciInfo, sizeof(DciInfo));
         if(!dciInfo)
         {
            DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for dciInfo alloc");
            return RFAILED;
         }
         memset(dciInfo,0,sizeof(DciInfo));
         /* update the SFN and SLOT */
         memcpy(&dlSchedInfo.schSlotValue.ulDciTime, slotInd, sizeof(SlotTimingInfo));
         slot = dlSchedInfo.schSlotValue.ulDciTime.slot;
         /* Update PUSCH allocation */
         schFillPuschAlloc(ueCb, slot, totDataReq, &schPuschInfo);
         /* Fill DCI for UL grant */
         schFillUlDci(ueCb, schPuschInfo, dciInfo);
         memcpy(&dciInfo->slotIndInfo, &dlSchedInfo.schSlotValue.ulDciTime, sizeof(SlotTimingInfo));
         dlSchedInfo.ulGrant = dciInfo;
      }
   }

   /* Check for pending BO grant for LC */
   if((cell->schDlSlotInfo[dlSchedInfo.schSlotValue.dlMsgTime.slot]->dlMsgInfo != NULLP) &&
      (!cell->schDlSlotInfo[dlSchedInfo.schSlotValue.dlMsgTime.slot]->dlMsgInfo->isMsg4Pdu))
   {
      schFillBoGrantDlSchedInfo(cell, &dlSchedInfo, dlMsgAlloc);
   }

   /* Send msg to MAC */
   ret = sendDlAllocToMac(&dlSchedInfo, schInst);
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Sending DL Broadcast allocation from SCH to MAC failed");
      return (ret);
   }

   schInitDlSlot(cell->schDlSlotInfo[slot]);
   schUlResAlloc(cell, schInst);


   return ret;
}

/**********************************************************************
  End of file
 **********************************************************************/


