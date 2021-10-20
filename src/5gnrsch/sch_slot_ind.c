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
PduTxOccsaion schCheckSsbOcc(SchCellCb *cell, SlotTimingInfo slotTime)
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
PduTxOccsaion schCheckSib1Occ(SchCellCb *cell, SlotTimingInfo slotTime)
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
   uint8_t ueIdx = 0, lcIdx = 0, pdschSymbols = 0;
   uint16_t slot = 0,  startPrb = 0, maxFreePRB = 0;
   uint16_t crnti = 0, mcsIdx = 0;
   uint32_t accumalatedSize = 0, tempBoBitMap = 0;
   SchUeCb *ueCb = NULLP;
   CmLListCp *lcLL = NULLP;
   SchPdschConfig pdschCfg;
   bool scheduleNextSlot = FALSE;
   SchDlSlotInfo *schDlSlotInfo = NULLP;
#ifdef NR_TDD
   uint16_t slotIdx = 0;
  #endif
   /* TX_PAYLOAD_HDR_LEN: Overhead which is to be Added once for any UE while estimating Accumulated TB Size
    * Following flag added to keep the record whether TX_PAYLOAD_HDR_LEN is added to the first Node getting allocated.
    * If both Dedicated and Default LC lists are present then First LC in Dedicated List will include this overhead
    * else if only Default list is present then first node in this List will add this overhead len*/

   bool isTxPayloadLenAdded = FALSE;

   DU_LOG("\nVS:Fill loop entry: cell->boIndBitMap:%d", cell->boIndBitMap);
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

      pdschCfg = ueCb->ueCfg.spCellCfg.servCellCfg.initDlBwp.pdschCfg; 
      mcsIdx = ueCb->ueCfg.dlModInfo.mcsIndex;
      pdschSymbols = pdschCfg.timeDomRsrcAllociList[0].symbolLength;
      isTxPayloadLenAdded = FALSE;
      accumalatedSize = 0;
      /* Dl ded Msg info is copied, this was earlier filled in macSchDlRlcBoInfo */
      memcpy(&dlMsgAlloc->dlMsgInfo, cell->schDlSlotInfo[slot]->dlMsgInfo, \
            sizeof(DlMsgInfo));

      /* scheduled LC data fill */
      dlMsgAlloc->numLc = 0;
      
      
      checkPendingLcInList(&(ueCb->dlLcPrbEst.defLcList), FALSE, dlMsgAlloc);
      if((ueCb->dlLcPrbEst.dedLcInfo != NULLP))
      {
         checkPendingLcInList(&(ueCb->dlLcPrbEst.dedLcInfo->dedLcList), TRUE, dlMsgAlloc);
      }

      DU_LOG("\nVS: sch: before entering loop: numLC:%d",dlMsgAlloc->numLc);
      for(lcIdx = 0; lcIdx < MAX_NUM_LC; lcIdx++)
      {
         if(ueCb->dlInfo.dlLcCtxt[lcIdx].bo)
         {
              if(ueCb->dlInfo.dlLcCtxt[lcIdx].isDedicated)
              {
                 lcLL = &(ueCb->dlLcPrbEst.dedLcInfo->dedLcList);
              }
              else
              {
                 lcLL = &(ueCb->dlLcPrbEst.defLcList);
              }

              /*Search for the LCID in the List. To add it in the dlMsgAlloc the
               * LCID should NOT be present in list because this LCID has
               * already been added during checkLcList*/
            if(searchOrCreateLcList(lcLL, ueCb->dlInfo.dlLcCtxt[lcIdx].lcId, SEARCH) == NULLP)
            {
              dlMsgAlloc->lcSchInfo[dlMsgAlloc->numLc].lcId = ueCb->dlInfo.dlLcCtxt[lcIdx].lcId;
              dlMsgAlloc->lcSchInfo[dlMsgAlloc->numLc].isDedicated = ueCb->dlInfo.dlLcCtxt[lcIdx].isDedicated;
            
               DU_LOG("\nINFO  --> VS :adding LC:%d,LCIDx:%disDedicated:%d,reqBO:%d",lcIdx,lcIdx,\
                         ueCb->dlInfo.dlLcCtxt[lcIdx].isDedicated, dlMsgAlloc->lcSchInfo[dlMsgAlloc->numLc].schBytes);
               dlMsgAlloc->numLc++;
            }
            /*Update the reqPRB and Payloadsize for this LC in List*/
            if(updateLcListReqPRB(lcLL, ueCb->dlInfo.dlLcCtxt[lcIdx].lcId,\
                     (ueCb->dlInfo.dlLcCtxt[lcIdx].bo + MAC_HDR_SIZE)) != ROK)
            {
               DU_LOG("\nERROR  --> SCH : Memory Allocation failure");
               /* Free the dl ded msg info allocated in macSchDlRlcBoInfo */
               SCH_FREE(dlMsgAlloc, sizeof(DlMsgAlloc));
               dlSchedInfo->dlMsgAlloc = NULLP;
               return RFAILED;
            }
         }
         ueCb->dlInfo.dlLcCtxt[lcIdx].bo = 0;
      }

      if (!dlMsgAlloc->numLc)
      {
         DU_LOG("\nDEBUG  -->  SCH : No pending BO for any LC id\n");
         /* Free the dl ded msg info allocated in macSchDlRlcBoInfo */
         SCH_FREE(dlMsgAlloc, sizeof(DlMsgAlloc));
         dlSchedInfo->dlMsgAlloc = NULLP;
         return ROK;
      }

      /*Calculate Best FREE BLOCK with MAX PRB count*/
      maxFreePRB = bestFreeBlockSearchDl(cell, dlSchedInfo->schSlotValue.dlMsgTime, &startPrb);
         DU_LOG("\nVS: SCH -> SCH :bestFreeBlockSearchDl maxFreeRB:%d,&startPrb:%d, sfn,slot[%d,%d]",\
               maxFreePRB,startPrb,dlSchedInfo->schSlotValue.dlMsgTime.sfn,dlSchedInfo->schSlotValue.dlMsgTime.slot);

      if(maxFreePRB == 0)
      {
         DU_LOG("\nERROR  --> SCH : NO FREE PRB!!");
         /* Free the dl ded msg info allocated in macSchDlRlcBoInfo */
         SCH_FREE(dlMsgAlloc, sizeof(DlMsgAlloc));
         dlSchedInfo->dlMsgAlloc = NULLP;
         return RFAILED;
      }

      if(ueCb->dlLcPrbEst.dedLcInfo != NULLP)/*To be removed*/
         DU_LOG("\nVS: SCH -> SCH : rsrvdPRB:%d",ueCb->dlLcPrbEst.dedLcInfo->rsvdDedicatedPRB);
      /*Either this UE contains no reservedPRB pool fir dedicated S-NSSAI or 
       * Num of Free PRB available is not enough to reserve Dedicated PRBs*/
      if((ueCb->dlLcPrbEst.dedLcInfo == NULLP) 
            || (maxFreePRB <  ueCb->dlLcPrbEst.dedLcInfo->rsvdDedicatedPRB))
      { 
         ueCb->dlLcPrbEst.sharedNumPrb = maxFreePRB;
         DU_LOG("\nWARNING  --> SCH : RESERVED PRB for Dedicated Slice is not available");
         DU_LOG("thus skipping PRB allocation for Dedicated Slice for this slot");
         DU_LOG("SharePRB number:%d.", ueCb->dlLcPrbEst.sharedNumPrb);

         /*PRB Alloc for Default LCs*/
         prbAllocUsingRRMPolicy(&(ueCb->dlLcPrbEst.defLcList), FALSE, mcsIdx, pdschSymbols,\
               &(ueCb->dlLcPrbEst.sharedNumPrb), NULLP, &accumalatedSize, &isTxPayloadLenAdded);

      }
      else
      {
         ueCb->dlLcPrbEst.sharedNumPrb = maxFreePRB - ueCb->dlLcPrbEst.dedLcInfo->rsvdDedicatedPRB;

         DU_LOG("\nVS: SCH -> SCH :DED LC processing rsrvdPRB:%d sharedPRB:%d",\
                     ueCb->dlLcPrbEst.dedLcInfo->rsvdDedicatedPRB,ueCb->dlLcPrbEst.sharedNumPrb);
         /*PRB Alloc for Dedicated LCs*/
         prbAllocUsingRRMPolicy(&(ueCb->dlLcPrbEst.dedLcInfo->dedLcList), TRUE, mcsIdx, pdschSymbols,\
               &(ueCb->dlLcPrbEst.sharedNumPrb), &(ueCb->dlLcPrbEst.dedLcInfo->rsvdDedicatedPRB),&accumalatedSize, &isTxPayloadLenAdded);
         DU_LOG("\nVS: SCH -> SCH :DEF LC processing SharedPRB:%d",ueCb->dlLcPrbEst.sharedNumPrb);
         /*PRB Alloc for Default LCs*/
         prbAllocUsingRRMPolicy(&(ueCb->dlLcPrbEst.defLcList), FALSE, mcsIdx, pdschSymbols, \
               &(ueCb->dlLcPrbEst.sharedNumPrb), &(ueCb->dlLcPrbEst.dedLcInfo->rsvdDedicatedPRB),&accumalatedSize, &isTxPayloadLenAdded);
      }

      DU_LOG("\nVS: INFO : schFillBoGrantDlSchedInfo numLC:%d", dlMsgAlloc->numLc);

      /*Traverse each LCID in dlMsgAlloc to calculate the exact Scheduled Bytes
       * using allocated BO per LC*/  
      checkAllocatedLcInList(ueCb, dlMsgAlloc);

      /* pdcch and pdsch data is filled */
      if((schDlRsrcAllocDlMsg(cell, dlSchedInfo->schSlotValue.dlMsgTime, \
                  crnti, accumalatedSize, dlMsgAlloc, startPrb)) != ROK)
      {
         DU_LOG("\nERROR  --> SCH : Scheduling of DL dedicated message failed");
         /* Free the dl ded msg info allocated in macSchDlRlcBoInfo */
         SCH_FREE(dlMsgAlloc, sizeof(DlMsgAlloc));
         dlSchedInfo->dlMsgAlloc = NULLP;
         return RFAILED;

      }

      /* TODO : Update the scheduling byte report for multiple LC based on QCI
       * and Priority */
      /* As of now, the total number of bytes scheduled for a slot is divided
       * equally amongst all LC with pending data. This is avoid starving of any
       * LC 
       * */
      #if 0
      accumalatedSize = accumalatedSize/dlMsgAlloc->numLc;
      for(lcIdx = 0; lcIdx < dlMsgAlloc->numLc; lcIdx ++)
         dlMsgAlloc->lcSchInfo[lcIdx].schBytes = accumalatedSize;
#endif
      /* PUCCH resource */
      schAllocPucchResource(cell, dlMsgAlloc->crnti, slot);

 
     DU_LOG("\nINFO --> SCH: Slot current :%d",slot); 

     if((ueCb->dlLcPrbEst.defLcList.count) ||
           ((ueCb->dlLcPrbEst.dedLcInfo != NULLP) && (ueCb->dlLcPrbEst.dedLcInfo->dedLcList.count)))
     {
           scheduleNextSlot = TRUE;
      }

    if(scheduleNextSlot) 
    {
      slot = (cell->slotInfo.slot + SCHED_DELTA + PHY_DELTA_DL + BO_DELTA) % cell->numSlots;
#ifdef NR_TDD
   while(schGetSlotSymbFrmt(cell->slotFrmtBitMap, slot) != DL_SLOT)
   {
      slot = (slot + 1)%cell->numSlots;
      slotIdx++;
      if(slotIdx==cell->numSlots)
      {
         DU_LOG("\nERROR  -->  SCH : No DL Slot available");
         return RFAILED;
      }
   }
#endif
     DU_LOG("\nINFO --> SCH: Slot scheduled :%d",slot); 

   schDlSlotInfo = cell->schDlSlotInfo[slot];

   if(schDlSlotInfo == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : MacSchDlRlcBoInfo(): schDlSlotInfo does not exists");
      return RFAILED;
   }
   SCH_ALLOC(schDlSlotInfo->dlMsgInfo, sizeof(DlMsgInfo));
   if(schDlSlotInfo->dlMsgInfo == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation failed for dlMsgInfo");
      schDlSlotInfo = NULL;
      return RFAILED;
   }

   schDlSlotInfo->dlMsgInfo->crnti = crnti;
   schDlSlotInfo->dlMsgInfo->ndi = 1;
   schDlSlotInfo->dlMsgInfo->harqProcNum = 0;
   schDlSlotInfo->dlMsgInfo->dlAssignIdx = 0;
   schDlSlotInfo->dlMsgInfo->pucchTpc = 0;
   schDlSlotInfo->dlMsgInfo->pucchResInd = 0;
   schDlSlotInfo->dlMsgInfo->harqFeedbackInd = 0;
   schDlSlotInfo->dlMsgInfo->dciFormatId = 1;
   SET_ONE_BIT(ueIdx, tempBoBitMap);
   }
      /* after allocation is done, unset the bo bit for that ue */
      UNSET_ONE_BIT(ueIdx, cell->boIndBitMap);
   }

   DU_LOG("\nVS:Fill loop : tempBoBitMap:%d,  cell->boIndBitMap:%d",tempBoBitMap, cell->boIndBitMap);
   cell->boIndBitMap = tempBoBitMap;
   DU_LOG("\nVS:Fill loop exit: cell->boIndBitMap:%d", cell->boIndBitMap);

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
   dlBrdcstAlloc->ssbTrans = schCheckSsbOcc(cell, dlSchedInfo.schSlotValue.broadcastTime); 
   if(dlBrdcstAlloc->ssbTrans)
   {
      if(schBroadcastSsbAlloc(cell, dlSchedInfo.schSlotValue.broadcastTime, dlBrdcstAlloc) != ROK)
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
   dlBrdcstAlloc->sib1Trans = schCheckSib1Occ(cell, dlSchedInfo.schSlotValue.broadcastTime);
   if(dlBrdcstAlloc->sib1Trans)
   {
      if(schBroadcastSib1Alloc(cell, dlSchedInfo.schSlotValue.broadcastTime, dlBrdcstAlloc) != ROK)
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
      if((schDlRsrcAllocMsg4(cell, dlSchedInfo.schSlotValue.dlMsgTime, msg4Alloc)) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : MSG4 scheduling failed");
         SCH_FREE(msg4Alloc, sizeof(DlMsgAlloc));
         dlSchedInfo.dlMsgAlloc = NULLP;
         SCH_FREE(cell->schDlSlotInfo[dlSchedInfo.schSlotValue.dlMsgTime.slot]->dlMsgInfo, sizeof(DlMsgInfo));
         return RFAILED;
      }

      /* PUCCH resource */
      schAllocPucchResource(cell, msg4Alloc->dlMsgInfo.crnti, dlSchedInfo.schSlotValue.dlMsgTime.slot);

      SCH_FREE(cell->schDlSlotInfo[dlSchedInfo.schSlotValue.dlMsgTime.slot]->dlMsgInfo, sizeof(DlMsgInfo));
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
      if((schFillBoGrantDlSchedInfo(cell, &dlSchedInfo, dlMsgAlloc)) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : DL MSG scheduling failed");
         SCH_FREE(cell->schDlSlotInfo[dlSchedInfo.schSlotValue.dlMsgTime.slot]->dlMsgInfo, sizeof(DlMsgInfo));
         return RFAILED;
      }

      /* Free the dl ded msg info allocated in macSchDlRlcBoInfo */
      SCH_FREE(cell->schDlSlotInfo[dlSchedInfo.schSlotValue.dlMsgTime.slot]->dlMsgInfo, sizeof(DlMsgInfo));
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


