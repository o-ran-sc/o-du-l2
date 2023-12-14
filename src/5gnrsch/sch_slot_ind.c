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
#include "sch_tmr.h"
#include "sch_utils.h"
#ifdef NR_DRX 
#include "sch_drx.h"
#endif

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

   return(MacMessageRouter(&pst, (void *)dlSchedInfo));
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
 * @params[in] SchCellCb *cell, SlotTimingInfo currTime, uint8_t ueId
 * @params[in] bool isRetx, SchDlHqProcCb **hqP
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
bool schFillBoGrantDlSchedInfo(SchCellCb *cell, SlotTimingInfo currTime, uint8_t ueId, bool isRetx, SchDlHqProcCb **hqP)
{
   uint8_t pdschNumSymbols = 0, pdschStartSymbol = 0;
   uint8_t lcIdx = 0;
   uint16_t startPrb = 0;
   uint16_t crnti = 0;
   uint32_t accumalatedSize = 0;
   SchUeCb *ueCb = NULLP;
   DlMsgSchInfo *dciSlotAlloc, *dlMsgAlloc;
   SlotTimingInfo pdcchTime, pdschTime, pucchTime;
   SchPdcchAllocInfo pdcchAllocInfo;

   GET_CRNTI(crnti,ueId);
   ueCb = &cell->ueCb[ueId-1];

   if (isRetx == FALSE)
   {
      if(schDlGetAvlHqProcess(cell, ueCb, hqP) != ROK)
      {
         return false;
      }
   }

   memset(&pdcchAllocInfo,0,sizeof(SchPdcchAllocInfo));
   if(findValidK0K1Value(cell, currTime, ueId, ueCb->k0K1TblPrsnt,\
            &pdschStartSymbol, &pdschNumSymbols, &pdcchTime, &pdschTime, \
            &pucchTime, isRetx, *hqP, &pdcchAllocInfo) != true )
   {
      /* If a valid combination of slots to scheduled PDCCH, PDSCH and PUCCH is
       * not found, do not perform resource allocation. Return from here. */
      return false;
   }
   
   /* allocate PDCCH and PDSCH resources for the ue */
   if(cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId-1] == NULL)
   {

      SCH_ALLOC(dciSlotAlloc, sizeof(DlMsgSchInfo));
      if(!dciSlotAlloc)
      {
         DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for ded DL msg alloc");
         return false;
      }
      cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId -1] = dciSlotAlloc;
      memset(dciSlotAlloc, 0, sizeof(DlMsgSchInfo));
   }
   else
   {
      dciSlotAlloc = cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId -1];
   }
   /* Dl ded Msg info is copied, this was earlier filled in macSchDlRlcBoInfo */
   fillDlMsgInfo(dciSlotAlloc, crnti, isRetx, *hqP);
   dciSlotAlloc->transportBlock[0].ndi = isRetx;

   accumalatedSize = cell->api->SchScheduleDlLc(pdcchTime, pdschTime, pdschNumSymbols, &startPrb, isRetx, hqP);

   /*Below case will hit if NO LC(s) are allocated due to resource crunch*/
   if (!accumalatedSize)
      return false;

   /*[Step6]: pdcch and pdsch data is filled */
   if((schDlRsrcAllocDlMsg(cell, pdschTime, crnti, accumalatedSize, dciSlotAlloc, startPrb,\
                           pdschStartSymbol, pdschNumSymbols, isRetx, *hqP, pdcchAllocInfo)) != ROK)
   {
      DU_LOG("\nERROR  --> SCH : Scheduling of DL dedicated message failed");

      /* Free the dl ded msg info allocated in macSchDlRlcBoInfo */
      if(!dciSlotAlloc->dlMsgPdschCfg)
      {
         SCH_FREE(dciSlotAlloc, sizeof(DlMsgSchInfo));
         cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId -1] = NULL;
      }
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
      SCH_ALLOC(dciSlotAlloc->dlMsgPdschCfg, sizeof(PdschCfg));
      if(!dciSlotAlloc->dlMsgPdschCfg)
      {
         DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for dciSlotAlloc->dlMsgPdschCfg");
         SCH_FREE(dciSlotAlloc->dlMsgPdcchCfg, sizeof(PdcchCfg));
         SCH_FREE(dciSlotAlloc, sizeof(DlMsgSchInfo));
         cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId-1] = NULLP;
         return false;
      }
      memcpy(dciSlotAlloc->dlMsgPdschCfg,\
         &dciSlotAlloc->dlMsgPdcchCfg->dci[dciSlotAlloc->dlMsgPdcchCfg->numDlDci - 1].pdschCfg,  sizeof(PdschCfg));
   }
   else
   {
      /* Allocate memory to schedule dlMsgAlloc to send DL_Msg, pointer will be checked at schProcessSlotInd() */
      if(cell->schDlSlotInfo[pdschTime.slot]->dlMsgAlloc[ueId-1] == NULLP)
      {
         SCH_ALLOC(dlMsgAlloc, sizeof(DlMsgSchInfo));
         if(dlMsgAlloc == NULLP)
         {
            DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for dlMsgAlloc");
            SCH_FREE(dciSlotAlloc->dlMsgPdcchCfg, sizeof(PdcchCfg));
            if(dciSlotAlloc->dlMsgPdschCfg == NULLP)
            {
               SCH_FREE(dciSlotAlloc, sizeof(DlMsgSchInfo));
               cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId-1] = NULLP;
            }
            return false;
         }
         cell->schDlSlotInfo[pdschTime.slot]->dlMsgAlloc[ueId-1] = dlMsgAlloc;
         memset(dlMsgAlloc, 0, sizeof(DlMsgSchInfo));
      }
      else
         dlMsgAlloc = cell->schDlSlotInfo[pdschTime.slot]->dlMsgAlloc[ueId-1];

      /* Copy all DL_MSG info */
      dlMsgAlloc->crnti =crnti;
      dlMsgAlloc->bwp = dciSlotAlloc->bwp;
      SCH_ALLOC(dlMsgAlloc->dlMsgPdschCfg, sizeof(PdschCfg));
      if(dlMsgAlloc->dlMsgPdschCfg)
      {
         memcpy(dlMsgAlloc->dlMsgPdschCfg,\
                &dciSlotAlloc->dlMsgPdcchCfg->dci[dciSlotAlloc->dlMsgPdcchCfg->numDlDci - 1].pdschCfg, sizeof(PdschCfg));
      }
      else
      {
         SCH_FREE(dciSlotAlloc->dlMsgPdcchCfg, sizeof(PdcchCfg));    
         if(dciSlotAlloc->dlMsgPdschCfg == NULLP)
         {
            SCH_FREE(dciSlotAlloc, sizeof(DlMsgSchInfo));
            cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId-1] = NULLP;

         }
         SCH_FREE(dlMsgAlloc, sizeof(DlMsgSchInfo));
         cell->schDlSlotInfo[pdschTime.slot]->dlMsgAlloc[ueId-1] = NULLP;
         DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for dlMsgAlloc->dlMsgPdschCfg");
         return false;
      }
   }

   schAllocPucchResource(cell, pucchTime, crnti, ueCb, isRetx, *hqP);

   cell->schUlSlotInfo[pucchTime.slot]->pucchUe = ueId;

   /*Re-setting the BO's of all DL LCs in this UE*/
   for(lcIdx = 0; lcIdx < MAX_NUM_LC; lcIdx++)
   {
      ueCb->dlInfo.dlLcCtxt[lcIdx].bo = 0;
   }

   /* after allocation is done, unset the bo bit for that ue */
   UNSET_ONE_BIT(ueId, cell->boIndBitMap);
   return true;
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

   return(MacMessageRouter(&pst, (void *)dlPageAlloc));
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
void schCalcSlotValues(SlotTimingInfo slotInd, SchSlotValue *schSlotValue, uint16_t numOfSlots)
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

   ADD_DELTA_TO_TIME(slotInd, schSlotValue->currentTime, PHY_DELTA_DL, numOfSlots);
   ADD_DELTA_TO_TIME(slotInd, schSlotValue->broadcastTime, PHY_DELTA_DL + SCHED_DELTA, numOfSlots);
   ADD_DELTA_TO_TIME(slotInd, schSlotValue->rarTime, PHY_DELTA_DL + SCHED_DELTA, numOfSlots);
   ADD_DELTA_TO_TIME(slotInd, schSlotValue->dlMsgTime, PHY_DELTA_DL + SCHED_DELTA, numOfSlots);
   ADD_DELTA_TO_TIME(slotInd, schSlotValue->ulDciTime, PHY_DELTA_DL + SCHED_DELTA, numOfSlots);
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

   ssb_rep = cell->cellCfg.ssbPeriod;

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
      if((slotTime.sfn % (SIB1_REPETITION_PERIOD/10) == 0) &&
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
 * @params[in] SchCellCb *cell, SlotTimingInfo currTime
 * @params[in] uint8_t ueId, bool dedMsg
 * @params[in] uint8_t *pdschStartSymbol, uint8_t *pdschSymblLen
 * @params[in] SlotTimingInfo *pdcchTime, SlotTimingInfo *pdschTime
 * @params[in] SlotTimingInfo *pucchTime, bool isRetx, SchDlHqProcCb *hqP
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************/
bool findValidK0K1Value(SchCellCb *cell, SlotTimingInfo currTime, uint8_t ueId, bool dedMsg,
                        uint8_t *pdschStartSymbol, uint8_t *pdschSymblLen, SlotTimingInfo *pdcchTime,
                        SlotTimingInfo *pdschTime, SlotTimingInfo *pucchTime, bool isRetx, SchDlHqProcCb *hqP,
                        SchPdcchAllocInfo *pdcchAllocInfo)
{
   uint8_t numK0 = 0, k0TblIdx = 0, k0Val = 0, k0Index =0 ;
   uint8_t k1TblIdx = 0, k1Index = 0, k1Val = 0, numK1 = 0;
   SchUeCb *ueCb = NULLP;
   SchK0K1TimingInfoTbl *k0K1InfoTbl;

   ADD_DELTA_TO_TIME(currTime, (*pdcchTime), PHY_DELTA_DL + SCHED_DELTA, cell->numSlots);
#ifdef NR_TDD
   if(schGetSlotSymbFrmt(pdcchTime->slot, cell->slotFrmtBitMap) != DL_SLOT)
   {
      /* If it is not a DL slot, cannot schedule PDCCH. Return from here. */
      return false;
   }
#endif

   if(dedMsg == true)
   {
      ueCb = &cell->ueCb[ueId-1];
      k0K1InfoTbl = &ueCb->k0K1InfoTbl;
      if(schDlCandidateSelection(ueCb, *pdcchTime, pdcchAllocInfo) == false)
      {
        DU_LOG("\nDEBUG  --> SCH: DL candidate Selection failed bcz PDCCH is unavailable for this slot");
        return false;     
      }
   }
   else
   {
      k0K1InfoTbl = &cell->k0K1InfoTbl;
   }

   numK0 = k0K1InfoTbl->k0k1TimingInfo[pdcchTime->slot].numK0;
   for(k0TblIdx = 0; k0TblIdx < numK0; k0TblIdx++)
   {
      k0Index = k0K1InfoTbl->k0k1TimingInfo[pdcchTime->slot].k0Indexes[k0TblIdx].k0Index;
      if(dedMsg != true)
      {
         k0Val = cell->cellCfg.dlCfgCommon.schInitialDlBwp.pdschCommon.timeDomRsrcAllocList[k0Index].k0;
         *pdschStartSymbol = cell->cellCfg.dlCfgCommon.schInitialDlBwp.pdschCommon.timeDomRsrcAllocList[k0Index].startSymbol;
         *pdschSymblLen = cell->cellCfg.dlCfgCommon.schInitialDlBwp.pdschCommon.timeDomRsrcAllocList[k0Index].lengthSymbol;
      }
      else
      {
         if(ueCb->ueCfg.spCellCfg.servCellRecfg.initDlBwp.pdschCfg.timeDomRsrcAllociList[k0Index].k0 != NULLP)
         {
            k0Val = *(ueCb->ueCfg.spCellCfg.servCellRecfg.initDlBwp.pdschCfg.timeDomRsrcAllociList[k0Index].k0);
            *pdschStartSymbol = ueCb->ueCfg.spCellCfg.servCellRecfg.initDlBwp.pdschCfg.timeDomRsrcAllociList[k0Index].startSymbol;
            *pdschSymblLen = ueCb->ueCfg.spCellCfg.servCellRecfg.initDlBwp.pdschCfg.timeDomRsrcAllociList[k0Index].symbolLength;
         }
         else
         {
            k0Val = cell->cellCfg.dlCfgCommon.schInitialDlBwp.pdschCommon.timeDomRsrcAllocList[k0Index].k0;
            *pdschStartSymbol = cell->cellCfg.dlCfgCommon.schInitialDlBwp.pdschCommon.timeDomRsrcAllocList[k0Index].startSymbol;
            *pdschSymblLen = cell->cellCfg.dlCfgCommon.schInitialDlBwp.pdschCommon.timeDomRsrcAllocList[k0Index].lengthSymbol;
         }
      }

      ADD_DELTA_TO_TIME((*pdcchTime), (*pdschTime), k0Val, cell->numSlots);
#ifdef NR_TDD
      if(schGetSlotSymbFrmt(pdschTime->slot, cell->slotFrmtBitMap) != DL_SLOT)
      {
         continue;
      }
#endif

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
            if(ueCb->ueCfg.spCellCfg.servCellRecfg.initUlBwp.pucchCfg.dlDataToUlAck)
            {
               k1Val = ueCb->ueCfg.spCellCfg.servCellRecfg.initUlBwp.pucchCfg.dlDataToUlAck->dlDataToUlAckList[k1Index];
            }
            else
            {
               k1Val = defaultUlAckTbl[k1Index];
            }
         }
         ADD_DELTA_TO_TIME((*pdschTime),(*pucchTime), k1Val, cell->numSlots);
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
         if(hqP)
         {
            ADD_DELTA_TO_TIME((*pucchTime), hqP->pucchTime, 0, cell->numSlots);
         }
         pdcchTime->cellId = cell->cellId;
         pdschTime->cellId = cell->cellId;

         return true;
      }
   }
   /*
    * Number of symbols in case of retransmisson should be same as it was in
    * original transmisson. Symbol availablity checks need to be added.
    */
   return false;
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

      ADD_DELTA_TO_TIME(currTime, dlPageAlloc.dlPageTime, PHY_DELTA_DL + SCHED_DELTA, cell->numSlots);
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
      memcpy(dlPageAlloc.pageDlDci.freqDomainResource, cell->sib1SchCfg.sib1PdcchCfg.coresetCfg.freqDomainResource, 6 * sizeof(uint8_t));
      dlPageAlloc.pageDlDci.durationSymbols = cell->sib1SchCfg.sib1PdcchCfg.coresetCfg.durationSymbols;
      dlPageAlloc.pageDlDci.cceRegMappingType = INTERLEAVED_CCE_REG_MAPPING;
      dlPageAlloc.pageDlDci.cceReg.interleaved.regBundleSize = cell->sib1SchCfg.sib1PdcchCfg.coresetCfg.regBundleSize;
      dlPageAlloc.pageDlDci.cceReg.interleaved.interleaverSize = cell->sib1SchCfg.sib1PdcchCfg.coresetCfg.interleaverSize;
      dlPageAlloc.pageDlDci.cceReg.interleaved.shiftIndex = cell->sib1SchCfg.sib1PdcchCfg.coresetCfg.shiftIndex;
      dlPageAlloc.pageDlDci.ssStartSymbolIndex = cell->sib1SchCfg.sib1PdcchCfg.coresetCfg.startSymbolIndex;
      dlPageAlloc.pageDlDci.cceIndex = cell->sib1SchCfg.sib1PdcchCfg.dci[0].cceIndex;
      dlPageAlloc.pageDlDci.aggregLevel = cell->sib1SchCfg.sib1PdcchCfg.dci[0].aggregLevel;
      dlPageAlloc.pageDlDci.precoderGranularity = cell->sib1SchCfg.sib1PdcchCfg.coresetCfg.precoderGranularity;
      dlPageAlloc.pageDlDci.coreSetSize = cell->sib1SchCfg.sib1PdcchCfg.coresetCfg.coreSetSize;
      /*Fill BWP*/
      memcpy(&dlPageAlloc.bwp, &cell->sib1SchCfg.bwp, sizeof(BwpCfg)); 

      /*Fill PDSCH*/
      if(schFillPagePdschCfg(cell, &dlPageAlloc.pageDlSch, pdschTime, tbSize, pageInfo->mcs, startPrb) != ROK)
      {
         DU_LOG("\nERROR  --> SCH: Issue in PDSCH Allocation for Paging at SFN:%d, SLOT:%d",\
               pdschTime.sfn, pdschTime.slot);
         break;
      }

      /*Fill Page PDU information*/
      dlPageAlloc.pageDlSch.dlPagePduLen = pageInfo->msgLen;

      SCH_ALLOC(dlPageAlloc.pageDlSch.dlPagePdu, sizeof(dlPageAlloc.pageDlSch.dlPagePduLen));

      if(dlPageAlloc.pageDlSch.dlPagePdu == NULLP)
      {
         DU_LOG("\nERROR  --> SCH: Memory Allocation Failed during Page Resource allocation");
         break;
      }
      memcpy(dlPageAlloc.pageDlSch.dlPagePdu, pageInfo->pagePdu, dlPageAlloc.pageDlSch.dlPagePduLen);

      /* Send msg to MAC */
      if(sendDlPageAllocToMac(&dlPageAlloc, schInst) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : Sending DL Paging allocation from SCH to MAC failed");
         SCH_FREE(dlPageAlloc.pageDlSch.dlPagePdu, sizeof(dlPageAlloc.pageDlSch.dlPagePduLen));
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
 *    Function : SchProcSlotInd
 *
 *    Functionality:
 *     Handles TTI indication received from PHY
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t SchProcSlotInd(Pst *pst, SlotTimingInfo *slotInd)
{
   uint8_t        ueIdx, ret = ROK;
   uint16_t       slot;
   DlSchedInfo    dlSchedInfo;
   DlBrdcstAlloc  *dlBrdcstAlloc = NULLP;
   SchCellCb      *cell = NULLP;
   Inst           schInst = pst->dstInst-SCH_INST_START;
   CmLList        *node = NULLP;
   TotalPrbUsage  *dlTotalPrbUsage = NULLP;

   cell = schCb[schInst].cells[schInst];
   if(cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : Cell Does not exist");
      return RFAILED;
   }
   memset(&dlSchedInfo, 0, sizeof(DlSchedInfo));
   schCalcSlotValues(*slotInd, &dlSchedInfo.schSlotValue, cell->numSlots);
   dlBrdcstAlloc = &dlSchedInfo.brdcstAlloc;
   dlBrdcstAlloc->ssbTransmissionMode = NO_TRANSMISSION;
   dlBrdcstAlloc->sib1TransmissionMode = NO_TRANSMISSION;

   memcpy(&cell->slotInfo, slotInd, sizeof(SlotTimingInfo));
   dlBrdcstAlloc->ssbIdxSupported = SSB_IDX_SUPPORTED;

   dlSchedInfo.cellId = cell->cellId;
   slot = dlSchedInfo.schSlotValue.broadcastTime.slot;

#ifdef NR_DRX 
   schHandleStartDrxTimer(cell);
#endif
   
   /* Check for SSB occassion */
   dlBrdcstAlloc->ssbTransmissionMode = schCheckSsbOcc(cell, dlSchedInfo.schSlotValue.broadcastTime); 
   if(dlBrdcstAlloc->ssbTransmissionMode)
   {
      if(schBroadcastSsbAlloc(cell, dlSchedInfo.schSlotValue.broadcastTime, dlBrdcstAlloc) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : schBroadcastSsbAlloc failed");
         dlBrdcstAlloc->ssbTransmissionMode = NO_TRANSMISSION;
      }
      else 
      {
         dlSchedInfo.isBroadcastPres = true;
         if((dlBrdcstAlloc->ssbTransmissionMode == NEW_TRANSMISSION) && (!cell->firstSsbTransmitted))
            cell->firstSsbTransmitted = true;
      }
   }

   /* Check for SIB1 occassion */
   dlBrdcstAlloc->sib1TransmissionMode = schCheckSib1Occ(cell, dlSchedInfo.schSlotValue.broadcastTime);
   if(dlBrdcstAlloc->sib1TransmissionMode)
   {
      if(schBroadcastSib1Alloc(cell, dlSchedInfo.schSlotValue.broadcastTime, dlBrdcstAlloc) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH : schBroadcastSib1Alloc failed");
         dlBrdcstAlloc->sib1TransmissionMode = NO_TRANSMISSION;
      }
      else 
      {
         dlSchedInfo.isBroadcastPres = true;
         if((dlBrdcstAlloc->sib1TransmissionMode == NEW_TRANSMISSION) && (!cell->firstSib1Transmitted))
            cell->firstSib1Transmitted = true;
      }
   }

   /*Process Paging Msg*/
   schProcDlPageAlloc(cell, *slotInd, schInst);

   cell->api->SchScheduleSlot(cell, slotInd, schInst);

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

   /* Update DL PRB Usage for all stats group which requested for DL Total PRB Usage */
   node = cmLListFirst(&schCb[schInst].statistics.activeKpiList.dlTotPrbUseList);
   while(node)
   {
      dlTotalPrbUsage = (TotalPrbUsage *)node->node;
      if(dlTotalPrbUsage)
      {
         dlTotalPrbUsage->numPrbUsedForTx += cell->schDlSlotInfo[slot]->prbAlloc.numPrbAlloc;
         dlTotalPrbUsage->totalPrbAvailForTx += MAX_NUM_RB;
      }
      node = node->next;
   }
   
   /* Re-initialize DL slot */
   schInitDlSlot(cell->schDlSlotInfo[slot]);

   /* Send UL Resource allocation to MAC */
   schUlResAlloc(cell, schInst);

#ifdef NR_DRX 
   schHandleExpiryDrxTimer(cell);
#endif   

   return ret;
}

/**********************************************************************
  End of file
 **********************************************************************/


