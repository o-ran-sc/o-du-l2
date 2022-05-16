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

SchMacDlPageAllocFunc schMacDlPageAllocOpts[] =
{
   packSchMacDlPageAlloc,
   MacProcDlPageAlloc,
   packSchMacDlPageAlloc
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
 * @brief Handles sending DL Page alloc to MAC 
 *
 * @details
 *
 *    Function : sendDlPAgeAllocToMac
 *
 *    Functionality:
 *     Sends DL Page Resource Allocation to MAC from SCH
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sendDlPageAllocToMac(DlPageAlloc *dlPageAlloc, Inst inst)
{
   Pst pst;

   memset(&pst, 0, sizeof(Pst));
   FILL_PST_SCH_TO_MAC(pst, inst);
   pst.event = EVENT_DL_PAGING_ALLOC;

   return(*schMacDlPageAllocOpts[pst.selector])(&pst, dlPageAlloc);

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
   ADD_DELTA_TO_TIME(slotInd, schSlotValue->ulDciTime, PHY_DELTA_DL + SCHED_DELTA);
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
 * @brief find correct combination of k0-k1 value
 *
 * @details
 *
 *    Function : findValidK0K1Value
 *
 *    Functionality:
 *       find correct combination of k0-k1 value
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************/
bool findValidK0K1Value(SchCellCb *cell, SlotTimingInfo currTime, uint8_t ueId, bool dedMsg, uint8_t *pdschStartSymbol, uint8_t *pdschSymblLen, SlotTimingInfo *pdcchTime,  SlotTimingInfo *pdschTime, SlotTimingInfo *pucchTime)
{
   uint8_t numK0 = 0, k0TblIdx = 0, k0Val = 0, k0Index =0 ;
   uint8_t k1TblIdx = 0, k1Index = 0, k1Val = 0, numK1 = 0;
   SchUeCb *ueCb = NULLP;
   SchK0K1TimingInfoTbl *k0K1InfoTbl;

   ADD_DELTA_TO_TIME(currTime, (*pdcchTime), PHY_DELTA_DL + SCHED_DELTA);
#ifdef NR_TDD
   if(schGetSlotSymbFrmt(pdcchTime->slot, cell->slotFrmtBitMap) != DL_SLOT)
   {
      /* If it is not a DL slot, cannot schedule PDCCH. Return from here. */
      return false;
   }
#endif

   if(cell->schDlSlotInfo[pdcchTime->slot]->pdcchUe != 0)
   {
      return false;
   }

   if(dedMsg == true)
   {
      ueCb = &cell->ueCb[ueId-1];
      k0K1InfoTbl = &ueCb->ueCfg.spCellCfg.servCellCfg.initDlBwp.k0K1InfoTbl;
   }
   else
   {
      k0K1InfoTbl = &cell->cellCfg.schInitialDlBwp.k0K1InfoTbl;
   }

   numK0 = k0K1InfoTbl->k0k1TimingInfo[pdcchTime->slot].numK0;
   for(k0TblIdx = 0; k0TblIdx < numK0; k0TblIdx++)
   {
      k0Index = k0K1InfoTbl->k0k1TimingInfo[pdcchTime->slot].k0Indexes[k0TblIdx].k0Index;
      if(dedMsg != true)
      {
         k0Val = cell->cellCfg.schInitialDlBwp.pdschCommon.timeDomRsrcAllocList[k0Index].k0;
         *pdschStartSymbol = cell->cellCfg.schInitialDlBwp.pdschCommon.timeDomRsrcAllocList[k0Index].startSymbol;
         *pdschSymblLen = cell->cellCfg.schInitialDlBwp.pdschCommon.timeDomRsrcAllocList[k0Index].lengthSymbol;
      }
      else
      {
         if(ueCb->ueCfg.spCellCfg.servCellCfg.initDlBwp.pdschCfg.timeDomRsrcAllociList[k0Index].k0 != NULLP)
         {
            k0Val = *(ueCb->ueCfg.spCellCfg.servCellCfg.initDlBwp.pdschCfg.timeDomRsrcAllociList[k0Index].k0);
            *pdschStartSymbol = ueCb->ueCfg.spCellCfg.servCellCfg.initDlBwp.pdschCfg.timeDomRsrcAllociList[k0Index].startSymbol;
            *pdschSymblLen = ueCb->ueCfg.spCellCfg.servCellCfg.initDlBwp.pdschCfg.timeDomRsrcAllociList[k0Index].symbolLength;
         }
      }

      ADD_DELTA_TO_TIME((*pdcchTime), (*pdschTime), k0Val);
#ifdef NR_TDD
      if(schGetSlotSymbFrmt(pdschTime->slot, cell->slotFrmtBitMap) != DL_SLOT)
      {
         continue;
      }
#endif
      if(cell->schDlSlotInfo[pdschTime->slot]->pdschUe != 0)
      {
         continue; 
      }

      numK1 = k0K1InfoTbl->k0k1TimingInfo[pdcchTime->slot].k0Indexes[k0TblIdx].k1TimingInfo.numK1;
      for(k1TblIdx = 0; k1TblIdx < numK1; k1TblIdx++)
      {
         k1Index = k0K1InfoTbl->k0k1TimingInfo[pdcchTime->slot].k0Indexes[k0TblIdx].k1TimingInfo.k1Indexes[k1TblIdx];
         if(dedMsg != true)
         {
            k1Val = defaultUlAckTbl[k1Index];
         }
         else
         {
            if(ueCb->ueCfg.spCellCfg.servCellCfg.initUlBwp.pucchCfg.dlDataToUlAck)
            {
               k1Val = ueCb->ueCfg.spCellCfg.servCellCfg.initUlBwp.pucchCfg.dlDataToUlAck->dlDataToUlAckList[k1Index];
            }
         }
         ADD_DELTA_TO_TIME((*pdschTime),(*pucchTime), k1Val);
#ifdef NR_TDD
         if(schGetSlotSymbFrmt(pucchTime->slot, cell->slotFrmtBitMap) == DL_SLOT)
         {
            continue;
         }
#endif
         if(cell->schUlSlotInfo[pucchTime->slot]->pucchUe != 0)
         {
            continue; 
         }
         return true;
      }
   }
   return false;
}

/*******************************************************************
 *
 * @brief 
 *
 * @details
 *
 *    Function : schFillBoGrantDlSchedInfo 
 *
 *    Functionality:
 
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
bool schFillBoGrantDlSchedInfo(SchCellCb *cell, SlotTimingInfo currTime, uint8_t ueId)
{
   uint8_t lcIdx = 0;
   uint8_t pdschNumSymbols = 0, pdschStartSymbol = 0;
   uint16_t startPrb = 0, maxFreePRB = 0;
   uint16_t crnti = 0, mcsIdx = 0;
   uint32_t accumalatedSize = 0;
   SchUeCb *ueCb = NULLP;
   CmLListCp *lcLL = NULLP;
   DlMsgAlloc *dciSlotAlloc, *dlMsgAlloc;
   SlotTimingInfo pdcchTime, pdschTime, pucchTime;

   /* TX_PAYLOAD_HDR_LEN: Overhead which is to be Added once for any UE while estimating Accumulated TB Size
    * Following flag added to keep the record whether TX_PAYLOAD_HDR_LEN is added to the first Node getting allocated.
    * If both Dedicated and Default LC lists are present then First LC in Dedicated List will include this overhead
    * else if only Default list is present then first node in this List will add this overhead len*/
   bool isTxPayloadLenAdded = FALSE;

   GET_CRNTI(crnti,ueId);
   ueCb = &cell->ueCb[ueId-1];

   if(findValidK0K1Value(cell, currTime, ueId, ueCb->ueCfg.spCellCfg.servCellCfg.initDlBwp.k0K1TblPrsnt,\
            &pdschStartSymbol, &pdschNumSymbols, &pdcchTime, &pdschTime, &pucchTime) != true )
   {
      /* If a valid combination of slots to scheduled PDCCH, PDSCH and PUCCH is
       * not found, do not perform resource allocation. Return from here. */
      return false;
   }

   /* allocate PDCCH and PDSCH resources for the ue */
   if(cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId-1] == NULL)
   {

      SCH_ALLOC(dciSlotAlloc, sizeof(DlMsgAlloc));
      if(!dciSlotAlloc)
      {
         DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for ded DL msg alloc");
         return false;
      }
      cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId -1] = dciSlotAlloc;
      memset(dciSlotAlloc, 0, sizeof(DlMsgAlloc));
      dciSlotAlloc->crnti = crnti;
   }
   else
   {
      dciSlotAlloc = cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId -1];
   }
   /* Dl ded Msg info is copied, this was earlier filled in macSchDlRlcBoInfo */
   fillDlMsgInfo(&dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo].dlMsgInfo, dciSlotAlloc->crnti);

   /*Re-Initalization per UE*/
   /* scheduled LC data fill */
   dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo].numLc = 0;
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
            if(dciSlotAlloc->numSchedInfo == 0)
            {
               SCH_FREE(dciSlotAlloc, sizeof(DlMsgAlloc));
               cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId -1] = NULL;
            }
            else
               memset(&dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo], 0, sizeof(DlMsgSchInfo));
            return false;
         }
      }
      ueCb->dlInfo.dlLcCtxt[lcIdx].bo = 0;
   }//End of for loop


   if ((ueCb->dlLcPrbEst.defLcList.count == 0) && \
         ((ueCb->dlLcPrbEst.dedLcInfo == NULL) || (ueCb->dlLcPrbEst.dedLcInfo->dedLcList.count == 0)))
   {
      DU_LOG("\nDEBUG  -->  SCH : No pending BO for any LC id\n");
      UNSET_ONE_BIT(ueId, cell->boIndBitMap);

      /* Free the dl ded msg info allocated in macSchDlRlcBoInfo */
      if(dciSlotAlloc->numSchedInfo == 0)
      {
         SCH_FREE(dciSlotAlloc, sizeof(DlMsgAlloc));
         cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId -1] = NULL;
      }
      else
         memset(&dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo], 0, sizeof(DlMsgSchInfo));

      /*TRUE because this UE has nothing to be scheduled*/
      return true;
   }

   /*[Step3]: Calculate Best FREE BLOCK with MAX PRB count*/
   maxFreePRB = searchLargestFreeBlock(cell, pdschTime, &startPrb, DIR_DL);

   /*[Step4]: Estimation of PRB and BO which can be allocated to each LC in
    * the list based on RRM policy*/

   /*Either this UE contains no reservedPRB pool fir dedicated S-NSSAI or 
    * Num of Free PRB available is not enough to reserve Dedicated PRBs*/
   if(maxFreePRB != 0)
   {
      mcsIdx = ueCb->ueCfg.dlModInfo.mcsIndex;
      if((ueCb->dlLcPrbEst.dedLcInfo == NULLP) 
            || ((maxFreePRB <  ueCb->dlLcPrbEst.dedLcInfo->rsvdDedicatedPRB)))
      { 
         ueCb->dlLcPrbEst.sharedNumPrb = maxFreePRB;
         DU_LOG("\nDEBUG  --> SCH : DL Only Default Slice is scheduled, sharedPRB Count:%d",\
               ueCb->dlLcPrbEst.sharedNumPrb);

         /*PRB Alloc for Default LCs*/
         prbAllocUsingRRMPolicy(&(ueCb->dlLcPrbEst.defLcList), FALSE, mcsIdx, pdschNumSymbols,\
               &(ueCb->dlLcPrbEst.sharedNumPrb), NULLP, &isTxPayloadLenAdded, NULLP);
      }
      else
      {
         ueCb->dlLcPrbEst.sharedNumPrb = maxFreePRB - ueCb->dlLcPrbEst.dedLcInfo->rsvdDedicatedPRB;

         /*PRB Alloc for Dedicated LCs*/
         prbAllocUsingRRMPolicy(&(ueCb->dlLcPrbEst.dedLcInfo->dedLcList), TRUE, mcsIdx, pdschNumSymbols,\
               &(ueCb->dlLcPrbEst.sharedNumPrb), &(ueCb->dlLcPrbEst.dedLcInfo->rsvdDedicatedPRB), &isTxPayloadLenAdded, NULLP);

         /*PRB Alloc for Default LCs*/
         prbAllocUsingRRMPolicy(&(ueCb->dlLcPrbEst.defLcList), FALSE, mcsIdx, pdschNumSymbols, \
               &(ueCb->dlLcPrbEst.sharedNumPrb), &(ueCb->dlLcPrbEst.dedLcInfo->rsvdDedicatedPRB), &isTxPayloadLenAdded, NULLP);
      }
   }

   /*[Step5]:Traverse each LCID in LcList to calculate the exact Scheduled Bytes
    * using allocated BO per LC and Update dlMsgAlloc(BO report for MAC*/ 
   if(ueCb->dlLcPrbEst.dedLcInfo != NULLP)
      updateGrantSizeForBoRpt(&(ueCb->dlLcPrbEst.dedLcInfo->dedLcList), dciSlotAlloc, NULLP, &(accumalatedSize));

   updateGrantSizeForBoRpt(&(ueCb->dlLcPrbEst.defLcList), dciSlotAlloc, NULLP, &(accumalatedSize));

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
      /* Not Freeing dlMsgAlloc as ZERO BO REPORT to be sent to RLC so that
       * Allocation can be done in next slot*/
      return false;
   }

   /*[Step6]: pdcch and pdsch data is filled */
   if((schDlRsrcAllocDlMsg(cell, pdschTime, crnti, accumalatedSize, dciSlotAlloc, startPrb, pdschStartSymbol, pdschNumSymbols)) != ROK)
   {
      DU_LOG("\nERROR  --> SCH : Scheduling of DL dedicated message failed");

      /* Free the dl ded msg info allocated in macSchDlRlcBoInfo */
      if(dciSlotAlloc->numSchedInfo == 0)
      {
         SCH_FREE(dciSlotAlloc, sizeof(DlMsgAlloc));
         cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId -1] = NULL;
      }
      else
         memset(&dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo], 0, sizeof(DlMsgSchInfo));
      return false;
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
   
   /* Check if both DCI and DL_MSG are sent in the same slot.
    * If not, allocate memory for DL_MSG PDSCH slot to store PDSCH info */

   if(pdcchTime.slot == pdschTime.slot)
   {
      dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo].pduPres = BOTH;
      dciSlotAlloc->numSchedInfo++;
   }
   else
   {
      /* Allocate memory to schedule dlMsgAlloc to send DL_Msg, pointer will be checked at schProcessSlotInd() */
      if(cell->schDlSlotInfo[pdschTime.slot]->dlMsgAlloc[ueId-1] == NULLP)
      {
         SCH_ALLOC(dlMsgAlloc, sizeof(DlMsgAlloc));
         if(dlMsgAlloc == NULLP)
         {
            DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for dlMsgAlloc");
            if(dciSlotAlloc->numSchedInfo == 0)
            {
               SCH_FREE(dciSlotAlloc, sizeof(DlMsgAlloc));
               cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId-1] = NULLP;
            }
            else
               memset(&dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo], 0, sizeof(DlMsgSchInfo));
            return false;
         }
         cell->schDlSlotInfo[pdschTime.slot]->dlMsgAlloc[ueId-1] = dlMsgAlloc;
         memset(dlMsgAlloc, 0, sizeof(DlMsgAlloc));
         dlMsgAlloc->crnti = dciSlotAlloc->crnti;
      }
      else
         dlMsgAlloc = cell->schDlSlotInfo[pdschTime.slot]->dlMsgAlloc[ueId-1];

      /* Copy all DL_MSG info */
      memcpy(&dlMsgAlloc->dlMsgSchedInfo[dlMsgAlloc->numSchedInfo], \
            &dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo], sizeof(DlMsgSchInfo));
      dlMsgAlloc->dlMsgSchedInfo[dlMsgAlloc->numSchedInfo].dlMsgPdcchCfg.dci.pdschCfg = \
            &dlMsgAlloc->dlMsgSchedInfo[dlMsgAlloc->numSchedInfo].dlMsgPdschCfg;

      /* Assign correct PDU types in corresponding slots */
      dlMsgAlloc->dlMsgSchedInfo[dlMsgAlloc->numSchedInfo].pduPres = PDSCH_PDU;
      dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo].pduPres = PDCCH_PDU;
      dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo].pdschSlot = pdschTime.slot;

      dciSlotAlloc->numSchedInfo++;
      dlMsgAlloc->numSchedInfo++;
   }

   schAllocPucchResource(cell, pucchTime, crnti);
   cell->schDlSlotInfo[pdcchTime.slot]->pdcchUe = ueId;
   cell->schDlSlotInfo[pdschTime.slot]->pdschUe = ueId;
   cell->schUlSlotInfo[pucchTime.slot]->pucchUe = ueId;

   /* after allocation is done, unset the bo bit for that ue */
   UNSET_ONE_BIT(ueId, cell->boIndBitMap);
 
   return true;
}

/*******************************************************************
*
* @brief Process DL Resource allocation for Page
*
* @details
*
*    Function : schProcDlPageAlloc
*
*    Functionality: Process DL Resource allocation for Page
*
* @params[in] SchCellCb *cell, SlotTimingInfo currTime, Inst schInst
*
* @return pointer to return Value(ROK, RFAILED)
*
* ****************************************************************/
uint8_t schProcDlPageAlloc(SchCellCb *cell, SlotTimingInfo currTime, Inst schInst)
{
   DlPageAlloc      dlPageAlloc;
   CmLList          *pageInfoNode = NULLP;
   SchPageInfo      *pageInfo = NULLP;
   SlotTimingInfo   pdschTime;
   uint32_t         tbSize = 0;
   uint16_t         startPrb = 0, maxFreePRB = 0, nPRB = 0;
   uint8_t          ret = RFAILED;

   pageInfoNode = schPageInfoSearchFromPageList(currTime, &(cell->pageCb.pageIndInfoRecord[currTime.sfn]));

   if(pageInfoNode == NULLP)
   {
      return ROK;
   }
   pageInfo = (SchPageInfo *)pageInfoNode->node;
   
   while(true)
   {
      dlPageAlloc.cellId = currTime.cellId;

      ADD_DELTA_TO_TIME(currTime, dlPageAlloc.dlPageTime, PHY_DELTA_DL + SCHED_DELTA);
      dlPageAlloc.shortMsgInd  = FALSE;
      pdschTime = dlPageAlloc.dlPageTime;

      /*Calculate Best FREE BLOCK with MAX PRB count*/
      maxFreePRB = searchLargestFreeBlock(cell, pdschTime, &startPrb, DIR_DL);

      if(maxFreePRB != 0)
      {
         tbSize = calculateEstimateTBSize(pageInfo->msgLen, pageInfo->mcs, NUM_PDSCH_SYMBOL, maxFreePRB, &nPRB);
      }
      else
      {
         DU_LOG("\nERROR  --> SCH: Unable to get any free block for Paging at SFN:%d, SLOT:%d",\
               pdschTime.sfn, pdschTime.slot);
         break;
      }
      /*Fill PDCCH: PDCCH Cfg is same as SIB1 as Paging will be a broadcast message*/
      memcpy(&dlPageAlloc.pagePdcchCfg, &cell->cellCfg.sib1SchCfg.sib1PdcchCfg, sizeof(PdcchCfg));
      dlPageAlloc.pagePdcchCfg.dci.rnti = P_RNTI;

      /*Fill BWP*/
      memcpy(&dlPageAlloc.bwp, &cell->cellCfg.sib1SchCfg.bwp, sizeof(BwpCfg)); 

      /*Fill PDSCH*/
      if(schFillPagePdschCfg(cell, &dlPageAlloc.pagePdschCfg, pdschTime, tbSize, pageInfo->mcs, startPrb) != ROK)
      {
         DU_LOG("\nERROR  --> SCH: Issue in PDSCH Allocation for Paging at SFN:%d, SLOT:%d",\
               pdschTime.sfn, pdschTime.slot);
         break;
      }
      dlPageAlloc.pagePdcchCfg.dci.pdschCfg = &dlPageAlloc.pagePdschCfg;

      /*Fill Page PDU information*/
      dlPageAlloc.dlPagePduLen = pageInfo->msgLen;

      SCH_ALLOC(dlPageAlloc.dlPagePdu, sizeof(dlPageAlloc.dlPagePduLen));

      if(dlPageAlloc.dlPagePdu == NULLP)
      {
         DU_LOG("\nERROR  --> SCH: Memory Allocation Failed during Page Resource allocation");
         break;
      }
      memcpy(dlPageAlloc.dlPagePdu, pageInfo->pagePdu, dlPageAlloc.dlPagePduLen);

      /* Send msg to MAC */
      if(sendDlPageAllocToMac(&dlPageAlloc, schInst) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : Sending DL Paging allocation from SCH to MAC failed");
         SCH_FREE(dlPageAlloc.dlPagePdu, sizeof(dlPageAlloc.dlPagePduLen));
         break;
      }
      ret = ROK;
      break;
   }

   /*Remove the Page Node*/
   SCH_FREE(pageInfo->pagePdu, pageInfo->msgLen);
   schDeleteFromPageInfoList(&(cell->pageCb.pageIndInfoRecord[currTime.sfn]), pageInfoNode);

   return(ret);

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
   uint8_t   ueId, ueIdx, ret = ROK;
   uint16_t  slot;
   bool      isRarPending = false, isRarScheduled = false;
   bool      isMsg4Pending = false, isMsg4Scheduled = false;
   bool      isUlGrantPending = false, isUlGrantScheduled = false;
   bool      isDlMsgPending = false, isDlMsgScheduled = false;
   CmLList       *pendingUeNode;
   DlSchedInfo   dlSchedInfo;
   DlBrdcstAlloc *dlBrdcstAlloc = NULLP;
   SchCellCb     *cell = NULLP;

   memset(&dlSchedInfo, 0, sizeof(DlSchedInfo));
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

   /*Process Paging Msg*/
   schProcDlPageAlloc(cell, *slotInd, schInst);

   /* Select first UE in the linked list to be scheduled next */
   pendingUeNode = cell->ueToBeScheduled.first;
   if(pendingUeNode)
   {
      if(pendingUeNode->node)
      {
         ueId = *(uint8_t *)(pendingUeNode->node);

         /* If RAR is pending for this UE, schedule PDCCH,PDSCH to send RAR and 
          * PUSCH to receive MSG3 as per k0-k2 configuration*/
         if(cell->raReq[ueId-1] != NULLP)
         {
            isRarPending = true;
            isRarScheduled = schProcessRaReq(schInst, cell, *slotInd, ueId);
         }

         /* If MSG4 is pending for this UE, schedule PDCCH,PDSCH to send MSG4 and
          * PUCCH to receive UL msg as per k0-k1 configuration  */
         if(cell->raCb[ueId-1].msg4recvd)
         {
            isMsg4Pending = true;
            isMsg4Scheduled = schProcessMsg4Req(cell, *slotInd, ueId);
         }

         if(isRarPending || isMsg4Pending)
         {
            /* If RAR or MSG is successfully scheduled then
             * remove UE from linked list since no pending msgs for this UE */
            if(isRarScheduled || isMsg4Scheduled)
            {
               SCH_FREE(pendingUeNode->node, sizeof(uint8_t));
               deleteNodeFromLList(&cell->ueToBeScheduled, pendingUeNode);
            }
            /* If RAR/MSG4 is pending but couldnt be scheduled then,
             * put this UE at the end of linked list to be scheduled later */
            else 
            {
               cmLListAdd2Tail(&cell->ueToBeScheduled, cmLListDelFrm(&cell->ueToBeScheduled, pendingUeNode));
            }
         }

         if(cell->ueCb[ueId-1].srRcvd || cell->ueCb[ueId-1].bsrRcvd)
         {
            isUlGrantPending = true;
            isUlGrantScheduled = schProcessSrOrBsrReq(cell, *slotInd, ueId);
         }

         if((cell->boIndBitMap) & (1<<ueId))
         {
            isDlMsgPending = true;
            isDlMsgScheduled = schFillBoGrantDlSchedInfo(cell, *slotInd, ueId);
         }
         if(!isUlGrantPending && !isDlMsgPending)
         {
            /* No action required */  
         }
         else if((isUlGrantPending && !isUlGrantScheduled) || (isDlMsgPending && !isDlMsgScheduled))
         {
            cmLListAdd2Tail(&cell->ueToBeScheduled, cmLListDelFrm(&cell->ueToBeScheduled, pendingUeNode));
         }
         else
         {
            SCH_FREE(pendingUeNode->node, sizeof(uint8_t));
            deleteNodeFromLList(&cell->ueToBeScheduled, pendingUeNode);
         }
      }
   }

   /* Check if any PDU is scheduled at this slot for any UE */
   for(ueIdx=0; ueIdx<MAX_NUM_UE; ueIdx++)
   {
      /* If RAR PDCCH/PDSCH is scheduled for a UE at this slot, fill RAR specific interface 
       * structure to send to MAC */
      if(cell->schDlSlotInfo[dlSchedInfo.schSlotValue.rarTime.slot]->rarAlloc[ueIdx] != NULLP)
      {
         slot = dlSchedInfo.schSlotValue.rarTime.slot;
         dlSchedInfo.rarAlloc[ueIdx] = cell->schDlSlotInfo[slot]->rarAlloc[ueIdx];
         cell->schDlSlotInfo[slot]->rarAlloc[ueIdx] = NULLP;
      }

      /* If DL-Msg PDCCH/PDSCH is scheduled for a UE at this slot, fill 
       * specific interface structure to send to MAC */
      if(cell->schDlSlotInfo[dlSchedInfo.schSlotValue.dlMsgTime.slot]->dlMsgAlloc[ueIdx] != NULLP)
      {
         slot = dlSchedInfo.schSlotValue.dlMsgTime.slot;
         dlSchedInfo.dlMsgAlloc[ueIdx] = cell->schDlSlotInfo[slot]->dlMsgAlloc[ueIdx];
         cell->schDlSlotInfo[slot]->dlMsgAlloc[ueIdx] = NULLP;
      }

   }

   if(cell->schDlSlotInfo[dlSchedInfo.schSlotValue.ulDciTime.slot]->ulGrant != NULLP)
   {
      slot = dlSchedInfo.schSlotValue.ulDciTime.slot;
      dlSchedInfo.ulGrant = cell->schDlSlotInfo[slot]->ulGrant;
      cell->schDlSlotInfo[slot]->ulGrant = NULLP;
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


