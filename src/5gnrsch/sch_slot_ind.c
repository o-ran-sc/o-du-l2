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
 *    Function : schProcessPendingBoIndReq 
 *
 *    Functionality:
 
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t schProcessPendingBoIndReq(SchCellCb *cell, SlotTimingInfo currTime)
{
   bool k1Found = FALSE;
   uint8_t numK0 = 0, k0TblIdx = 0, k0Index = 0, k0Val = 0;
   uint8_t k1TblIdx = 0, k1Index = 0, k1Val = 0, numK1 = 0;
   uint8_t ueIdx = 0, lcIdx = 0, pdschSymbols = 0;
   uint16_t startPrb = 0, maxFreePRB = 0;
   uint16_t crnti = 0, mcsIdx = 0;
   uint16_t pdcchSlot = 0, pdschSlot = 0;
   uint32_t accumalatedSize = 0;
   CmLListCp *lcLL = NULLP;
   SchUeCb *ueCb = NULLP;
   SchK0K1TimingInfoTbl *k0K1InfoTbl;
   SchPdschConfig pdschCfg;
   DlMsgAlloc *dciSlotAlloc, *dlMsgAlloc;  
   SlotTimingInfo pdcchTime, pdschTime, pucchTime;

   /* TX_PAYLOAD_HDR_LEN: Overhead which is to be Added once for any UE while
    estimating Accumulated TB Size
    Following flag added to keep the record whether TX_PAYLOAD_HDR_LEN
    is added to the first Node getting allocated.
    If both Dedicated and Default LC lists are present then First LC
    in Dedicated List will include this overhead
    else if only Default list is present then first node in this
    List will add this overhead len*/
   bool isTxPayloadLenAdded = FALSE;

   if(cell == NULL)
   {
      DU_LOG("\nERROR  -->  SCH: schProcessPendingBoIndReq() : Cell is NULL");
      return RFAILED;
   }

   while(cell->boIndication.boIndBitMap)
   {

      GET_RIGHT_MOST_SET_BIT(cell->boIndication.boIndBitMap, ueIdx);
      GET_CRNTI(crnti,ueIdx);
      ueCb = &cell->ueCb[ueIdx-1];

      ADD_DELTA_TO_TIME(currTime, pdcchTime, PHY_DELTA_DL + SCHED_DELTA);
      pdcchSlot = pdcchTime.slot;
#ifdef NR_TDD
      if(schGetSlotSymbFrmt(pdcchSlot, cell->slotFrmtBitMap) != DL_SLOT)
      {
         continue;
      }
#endif
      k0K1InfoTbl = &cell->cellCfg.schInitialDlBwp.k0K1InfoTbl;
      numK0 = k0K1InfoTbl->k0k1TimingInfo[pdcchSlot].numK0;
      for(k0TblIdx = 0; k0TblIdx < numK0; k0TblIdx++)
      {
         k0Index = k0K1InfoTbl->k0k1TimingInfo[pdcchSlot].k0Indexes[k0TblIdx].k0Index;
         if(ueCb->ueCfg.spCellCfg.servCellCfg.initDlBwp.pdschCfg.timeDomRsrcAllociList[k0Index].k0 != NULLP)
            k0Val = *(ueCb->ueCfg.spCellCfg.servCellCfg.initDlBwp.pdschCfg.timeDomRsrcAllociList[k0Index].k0);

         ADD_DELTA_TO_TIME(pdcchTime, pdschTime, k0Val);
         pdschSlot = pdschTime.slot;
#ifdef NR_TDD
         if(schGetSlotSymbFrmt(pdschSlot, cell->slotFrmtBitMap) != DL_SLOT)
         {
            continue;
         }
#endif
         numK1 = k0K1InfoTbl->k0k1TimingInfo[pdcchSlot].k0Indexes[k0TblIdx].k1TimingInfo.numK1;
         for(k1TblIdx = 0; k1TblIdx < numK1; k1TblIdx++)
         {
            k1Index = k0K1InfoTbl->k0k1TimingInfo[pdcchSlot].k0Indexes[k0TblIdx].k1TimingInfo.k1Indexes[k1TblIdx];
            if(ueCb->ueCfg.spCellCfg.servCellCfg.initUlBwp.pucchCfg.dlDataToUlAck)
            k1Val = ueCb->ueCfg.spCellCfg.servCellCfg.initUlBwp.pucchCfg.dlDataToUlAck->dlDataToUlAckList[k1Index];

            ADD_DELTA_TO_TIME(pdschTime, pucchTime, k1Val);
#ifdef NR_TDD
            if(schGetSlotSymbFrmt(pucchTime.slot, cell->slotFrmtBitMap) == DL_SLOT)
            {
               continue;
            }
#endif
            k1Found = true;
            break;
         }
         if(k1Found)
            break;
      }

      /* If K0-K1 combination not found, no scheduling happens */
      if(!k1Found)
      {
         ueIdx++;
         continue;
      }

      SCH_ALLOC(dciSlotAlloc, sizeof(DlMsgAlloc));
      if(!dciSlotAlloc)
      {
         DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for ded DL msg alloc");
         return RFAILED;
      }
      cell->schDlSlotInfo[pdcchSlot]->dlMsgAlloc = dciSlotAlloc;
      memset(dciSlotAlloc, 0, sizeof(DlMsgAlloc));
      //dlSchedInfo->dciSlotAlloc = dciSlotAlloc;
      dciSlotAlloc->crnti = crnti;

      pdschCfg = ueCb->ueCfg.spCellCfg.servCellCfg.initDlBwp.pdschCfg;
      mcsIdx = ueCb->ueCfg.dlModInfo.mcsIndex;


      pdschSymbols = pdschCfg.timeDomRsrcAllociList[k0Index].symbolLength;
      /* Dl ded Msg info is copied, this was earlier filled in macSchDlRlcBoInfo */
      memcpy(&dciSlotAlloc->dlMsgInfo, &cell->boIndication.dlMsgInfo, sizeof(DlMsgInfo));


      /*Re-Initalization per UE*/
      /* scheduled LC data fill */
      dciSlotAlloc->numLc = 0;
      isTxPayloadLenAdded = FALSE; /*Re-initlaize the flag for every UE*/
      accumalatedSize = 0;

      for(lcIdx = 0; lcIdx < MAX_NUM_LC; lcIdx++)
      {
         if(ueCb->dlInfo.dlLcCtxt[lcIdx].bo)
         {
            /*Check the LC is Dedicated or default and accordingly LCList will
             * be used*/
            if(ueCb->dlInfo.dlLcCtxt[lcIdx].isDedicated)
            {
               lcLL = &(ueCb->dlLcPrbEst.dedLcInfo->dedLcList);
            }
            else
            {
               lcLL = &(ueCb->dlLcPrbEst.defLcList);
            }

            /*[Step2]: Update the reqPRB and Payloadsize for this LC in the appropriate List*/
            if(updateLcListReqPRB(lcLL, ueCb->dlInfo.dlLcCtxt[lcIdx].lcId,\
                     (ueCb->dlInfo.dlLcCtxt[lcIdx].bo + MAC_HDR_SIZE)) != ROK)
            {
               DU_LOG("\nERROR  --> SCH : Updation in LC List Failed");
               /* Free the dl ded msg info allocated in macSchDlRlcBoInfo */
               SCH_FREE(dciSlotAlloc, sizeof(DlMsgAlloc));
               return RFAILED;
            }
         }
         ueCb->dlInfo.dlLcCtxt[lcIdx].bo = 0;
      }//End of for loop


      if ((ueCb->dlLcPrbEst.defLcList.count == 0) && \
            ((ueCb->dlLcPrbEst.dedLcInfo != NULL) && (ueCb->dlLcPrbEst.dedLcInfo->dedLcList.count == 0)))
      {
         DU_LOG("\nDEBUG  -->  SCH : No pending BO for any LC id\n");
         /* Free the dl ded msg info allocated in macSchDlRlcBoInfo */
         SCH_FREE(dciSlotAlloc, sizeof(DlMsgAlloc));
         return ROK;
      }

      /*[Step3]: Calculate Best FREE BLOCK with MAX PRB count*/
      maxFreePRB = searchLargestFreeBlockDL(cell, pdschTime, &startPrb);

      /*[Step4]: Estimation of PRB and BO which can be allocated to each LC in
       * the list based on RRM policy*/

      /*Either this UE contains no reservedPRB pool fir dedicated S-NSSAI or
       * Num of Free PRB available is not enough to reserve Dedicated PRBs*/
      if(maxFreePRB != 0)
      {
         if((ueCb->dlLcPrbEst.dedLcInfo == NULLP)
               || ((maxFreePRB <  ueCb->dlLcPrbEst.dedLcInfo->rsvdDedicatedPRB)))
         {
            ueCb->dlLcPrbEst.sharedNumPrb = maxFreePRB;
            DU_LOG("\nWARNING  --> SCH : Only Default Slice is scheduled, sharedPRB Count:%d",\
                  ueCb->dlLcPrbEst.sharedNumPrb);

            /*PRB Alloc for Default LCs*/
            prbAllocUsingRRMPolicy(&(ueCb->dlLcPrbEst.defLcList), FALSE, mcsIdx, pdschSymbols,\
                  &(ueCb->dlLcPrbEst.sharedNumPrb), NULLP, &isTxPayloadLenAdded);
         }
         else
         {
            ueCb->dlLcPrbEst.sharedNumPrb = maxFreePRB - ueCb->dlLcPrbEst.dedLcInfo->rsvdDedicatedPRB;

            /*PRB Alloc for Dedicated LCs*/
            prbAllocUsingRRMPolicy(&(ueCb->dlLcPrbEst.dedLcInfo->dedLcList), TRUE, mcsIdx, pdschSymbols,\
                  &(ueCb->dlLcPrbEst.sharedNumPrb), &(ueCb->dlLcPrbEst.dedLcInfo->rsvdDedicatedPRB), &isTxPayloadLenAdded);

            /*PRB Alloc for Default LCs*/
            prbAllocUsingRRMPolicy(&(ueCb->dlLcPrbEst.defLcList), FALSE, mcsIdx, pdschSymbols, \
                  &(ueCb->dlLcPrbEst.sharedNumPrb), &(ueCb->dlLcPrbEst.dedLcInfo->rsvdDedicatedPRB), &isTxPayloadLenAdded);
         }
      }

      /*[Step5]:Traverse each LCID in LcList to calculate the exact Scheduled Bytes
       * using allocated BO per LC and Update dciSlotAlloc(BO report for MAC*/
      if(ueCb->dlLcPrbEst.dedLcInfo != NULLP)
         updateGrantSizeForBoRpt(&(ueCb->dlLcPrbEst.dedLcInfo->dedLcList), dciSlotAlloc, &(accumalatedSize));

      updateGrantSizeForBoRpt(&(ueCb->dlLcPrbEst.defLcList), dciSlotAlloc, &(accumalatedSize));

      /*Below case will hit if NO LC(s) are allocated due to resource crunch*/
      if (!accumalatedSize)
      {
         if(maxFreePRB == 0)
         {
            DU_LOG("\nERROR  --> SCH : NO FREE PRB!!");
         }
         else
         {
            /*Schedule the LC for next slot*/
            DU_LOG("\nDEBUG  -->  SCH : No LC has been scheduled");
         }
         /*Not Freeing dciSlotAlloc as ZERO BO REPORT to be sent to RLC so that
          * Allocation can be done in next slot*/
         return ROK;
      }

      /*[Step6]: pdcch and pdsch data is filled */
      if((schDlRsrcAllocDlMsg(cell, pdschTime, crnti, accumalatedSize, dciSlotAlloc, startPrb)) != ROK)
      {
         DU_LOG("\nERROR  --> SCH : Scheduling of DL dedicated message failed");
         /* Free the dl ded msg info allocated in macSchDlRlcBoInfo */
         SCH_FREE(dciSlotAlloc, sizeof(DlMsgAlloc));
         return RFAILED;
      }
      dciSlotAlloc->pduPres = BOTH;

      /* TODO : Update the scheduling byte report for multiple LC based on QCI
       * and Priority */
      /* As of now, the total number of bytes scheduled for a slot is divided
       * equally amongst all LC with pending data. This is avoid starving of any
       * LC
       * */
#if 0
      accumalatedSize = accumalatedSize/dciSlotAlloc->numLc;
      for(lcIdx = 0; lcIdx < dciSlotAlloc->numLc; lcIdx ++)
         dciSlotAlloc->lcSchInfo[lcIdx].schBytes = accumalatedSize;
#endif

      /* Check if both DCI and RAR are sent in the same slot.
       * If not, allocate memory RAR PDSCH slot to store RAR info
       */
      if(pdcchSlot == pdschSlot)
         dciSlotAlloc->pduPres = BOTH;
      else
      {
         /* Allocate memory to schedule rarSlot to send RAR, pointer will be checked at schProcessSlotInd() */
         SCH_ALLOC(dlMsgAlloc, sizeof(DlMsgAlloc));
         if(dlMsgAlloc == NULLP)
         {
            DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for dlMsgAlloc");
            SCH_FREE(dciSlotAlloc, sizeof(DlMsgAlloc));
            cell->schDlSlotInfo[pdcchSlot]->dlMsgAlloc = NULLP;
            return RFAILED;
         }
         cell->schDlSlotInfo[pdschSlot]->dlMsgAlloc = dlMsgAlloc;

         /* Copy all RAR info */
         memcpy(dlMsgAlloc, dciSlotAlloc, sizeof(DlMsgAlloc));
         dlMsgAlloc->dlMsgPdcchCfg.dci.pdschCfg = &dlMsgAlloc->dlMsgPdschCfg;

         /* Assign correct PDU types in corresponding slots */
         dlMsgAlloc->pduPres = PDSCH_PDU;
         dciSlotAlloc->pduPres = PDCCH_PDU;
         dciSlotAlloc->pdschSlot = pdschSlot;
      }

      schAllocPucchResource(cell, pucchTime, crnti);

      /* after allocation is done, unset the bo bit for that ue */
      UNSET_ONE_BIT(ueIdx, cell->boIndication.boIndBitMap);
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
   
   schProcessMsg4Req(cell, *slotInd);

   /* Check for MSG4 */
   if((cell->schDlSlotInfo[dlSchedInfo.schSlotValue.dlMsgTime.slot]->dlMsgAlloc != NULLP) &&
      (cell->schDlSlotInfo[dlSchedInfo.schSlotValue.dlMsgTime.slot]->dlMsgAlloc->dlMsgInfo.isMsg4Pdu))
   {
      slot = dlSchedInfo.schSlotValue.dlMsgTime.slot;
      dlSchedInfo.dlMsgAlloc = cell->schDlSlotInfo[slot]->dlMsgAlloc;
      cell->schDlSlotInfo[slot]->dlMsgAlloc = NULLP;
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

         /* Update PUSCH allocation */
         schFillPuschAlloc(ueCb, dlSchedInfo.schSlotValue.ulDciTime, totDataReq, &schPuschInfo);

         /* Fill DCI for UL grant */
         schFillUlDci(ueCb, schPuschInfo, dciInfo);
         memcpy(&dciInfo->slotIndInfo, &dlSchedInfo.schSlotValue.ulDciTime, sizeof(SlotTimingInfo));
         dlSchedInfo.ulGrant = dciInfo;
      }
   }
  
   schProcessPendingBoIndReq(cell, *slotInd);

   /* Check for pending BO grant for LC */
   if((cell->schDlSlotInfo[dlSchedInfo.schSlotValue.dlMsgTime.slot]->dlMsgAlloc != NULLP) &&
      (!cell->schDlSlotInfo[dlSchedInfo.schSlotValue.dlMsgTime.slot]->dlMsgAlloc->dlMsgInfo.isMsg4Pdu))
   {
      slot = dlSchedInfo.schSlotValue.dlMsgTime.slot;
      dlSchedInfo.dlMsgAlloc = cell->schDlSlotInfo[slot]->dlMsgAlloc;
      cell->schDlSlotInfo[slot]->dlMsgAlloc = NULLP;
#if 0
      if((schFillBoGrantDlSchedInfo(cell, &dlSchedInfo, dlMsgAlloc)) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : DL MSG scheduling failed");
         SCH_FREE(cell->schDlSlotInfo[dlSchedInfo.schSlotValue.dlMsgTime.slot]->dlMsgAlloc, sizeof(DlMsgAlloc));
         return RFAILED;
      }
#endif
      /* Free the dl ded msg info allocated in macSchDlRlcBoInfo */
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


