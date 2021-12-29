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
 
     Name:     sch_rach.c
  
     Type:     C source file
  
     Desc:     C source code for rach handling functions
  
     File:     sch_rach.c
  
**********************************************************************/

/** @file sch_rach.c
@brief This file implements the rach handling.
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

/**
 * @brief Checks if PRACH can be scheduled in current slot
 *
 * @details
 *
 *     Function : schCheckPrachOcc
 *
 *     This function checks if PRACH can be scheduled in 
 *     current slot
 *
 *  @param[in]  Cell Cb
 *              Slot timing
 *  @return  TRUE
 *           FALSE
 **/
bool schCheckPrachOcc(SchCellCb *cell, SlotTimingInfo prachOccasionTimingInfo)
{
   uint8_t  prachCfgIdx = 0;
   uint8_t  x = 0;
   uint8_t  y = 0;
   uint8_t  subFrame = 0;
   uint16_t prachSubframe = 0;

   prachCfgIdx      = cell->cellCfg.schRachCfg.prachCfgIdx;

   /* derive the prachCfgIdx table paramters */
   x                = prachCfgIdxTable[prachCfgIdx][1];
   y                = prachCfgIdxTable[prachCfgIdx][2];
   prachSubframe    = prachCfgIdxTable[prachCfgIdx][3];

   if((prachOccasionTimingInfo.sfn%x) == y)
   {
      subFrame = prachOccasionTimingInfo.slot/pow(2, cell->cellCfg.numerology);

      /* check for subFrame number */
      if ((1 << subFrame) & prachSubframe)
      {
         /* prach ocassion present in this subframe */
#ifdef NR_TDD
         if(UL_SLOT != schGetSlotSymbFrmt(prachOccasionTimingInfo.slot % cell->numSlotsInPeriodicity,\
         cell->slotFrmtBitMap))
         {
            DU_LOG("\nERROR  --> SCH : PrachCfgIdx %d doesn't support UL slot", prachCfgIdx);
            return FALSE;
         }
#endif
         return TRUE;
      }
   }
   return FALSE;
}

/**
 * @brief Calculate number of PRBs to be allocated for PRACH 
 *
 * @details
 *
 *     Function : schCalcPrachNumRb
 *
 *     Calculate number of PRBs to be allocated for PRACH
 *
 *  @param[in]  SchCellCb *cell, cell cb
 *  @return  Number of PRBs
 **/
uint8_t schCalcPrachNumRb(SchCellCb *cell)
{
   uint8_t tableIdx = 0;
   uint16_t puschScs = convertScsEnumValToScsVal(cell->cellCfg.schInitialUlBwp.bwp.scs);

   for(tableIdx=0; tableIdx < MAX_RACH_NUM_RB_IDX; tableIdx++)
   {
      if((numRbForPrachTable[tableIdx][0] == cell->cellCfg.schRachCfg.rootSeqLen) &&
            (numRbForPrachTable[tableIdx][1] == cell->cellCfg.schRachCfg.prachSubcSpacing) &&
            (numRbForPrachTable[tableIdx][2] == puschScs))
      {
         return numRbForPrachTable[tableIdx][3];
      }
   }
   return 0;
}

/**
 * @brief resource allocation for PRACH
 *
 * @details
 *
 *     Function : schPrachResAlloc
 *
 *     This function handles PRACH allocation
 *
 *  @param[in]  SchCellCb *cell, cell cb
 *  @param[in]  UlSchedInfo *ulSchedInfo, UL scheduling info
 *  @return  void
 **/
void schPrachResAlloc(SchCellCb *cell, UlSchedInfo *ulSchedInfo, SlotTimingInfo prachOccasionTimingInfo)
{
   uint8_t  numPrachRb = 0;
   uint8_t  numRa = 0;
   uint8_t  prachCfgIdx = 0;
   uint8_t  prachFormat = 0;
   uint8_t  prachStartSymbol = 0;
   uint8_t  prachDuration = 0;
   uint8_t  prachOcas = 0;
   uint8_t  dataType = 0;
   uint16_t freqStart = 0;

   if(cell == NULLP)
   {
      DU_LOG("\nERROR  --> SCH : schPrachResAlloc(): Received cellCb is null");
      return;
   }

   /* If this slot is not a PRACH occassion, return */
   if(!schCheckPrachOcc(cell, prachOccasionTimingInfo))
      return;

   prachCfgIdx      = cell->cellCfg.schRachCfg.prachCfgIdx;
   prachFormat      = prachCfgIdxTable[prachCfgIdx][0];
   prachStartSymbol = prachCfgIdxTable[prachCfgIdx][4];
   prachOcas        = prachCfgIdxTable[prachCfgIdx][6];
   prachDuration    = prachCfgIdxTable[prachCfgIdx][7];

   /* numRa determined as 𝑛 belonging {0,1,.., M − 1},
    * where M is given by msg1Fdm */
   numRa = (cell->cellCfg.schRachCfg.msg1Fdm - 1);

   /* freq domain resource determination for RACH*/
   freqStart = cell->cellCfg.schRachCfg.msg1FreqStart;
   numPrachRb = schCalcPrachNumRb(cell);
   /* Allocate PRACH resources from the UL resource bitmap */
   allocatePrbUl(cell, prachOccasionTimingInfo, prachStartSymbol, prachDuration, &freqStart, numPrachRb);

   /* prach info */
   dataType |= SCH_DATATYPE_PRACH;
   ulSchedInfo->dataType = dataType;
   ulSchedInfo->prachSchInfo.numPrachOcas   = prachOcas;
   ulSchedInfo->prachSchInfo.prachFormat    = prachFormat;
   ulSchedInfo->prachSchInfo.numRa          = numRa;
   ulSchedInfo->prachSchInfo.prachStartSymb = prachStartSymbol;
   DU_LOG("\nINFO  --> SCH : RACH occassion set for slot %d", prachOccasionTimingInfo.slot);
}

/**
 * @brief calculate ra-rnti function. 
 *
 * @details
 *
 *     Function : calculateRaRnti
 *     
 *     This function calculates ra-rnti
 *     
 *  @param[in]  symbol index
 *  @param[in]  slot index
 *  @param[in]  frequency index
 *  @return  ra-rnti
 **/
uint16_t calculateRaRnti(uint8_t symbolIdx, uint8_t slotIdx, uint8_t freqIdx)
{
   uint16_t raRnti = 0;
   uint8_t  ulCarrierIdx = 0; /* Uplink carrier used for MSG1 transmission. 0:NUL carrier; 1:SUL carrier */
   
   /* Refer to spec 38.321, section 5.1.3 */
   raRnti = (1 + symbolIdx + (14*slotIdx) + (14*80*freqIdx) + (14*80*8*ulCarrierIdx));
   return raRnti;
}

/**
 * @brief create raCb function. 
 *
 * @details
 *
 *     Function : createSchRaCb
 *     
 *     This function create raCb
 *     
 *  @param[in]  tcrnti
 *  @param[in]  shed instance
 *  @return  void
 **/
void createSchRaCb(uint16_t tcrnti, Inst schInst)
{
   uint8_t ueId = 0;

   GET_UE_ID(tcrnti, ueId);
   schCb[schInst].cells[schInst]->raCb[ueId -1].tcrnti = tcrnti;
   schCb[schInst].cells[schInst]->raCb[ueId -1].msg4recvd = FALSE;
}

/**
 * @brief resource allocation for msg3 PUSCH
 *
 * @details
 *
 *     Function : schAllocMsg3Pusch 
 *     
 *     This function handles msg3 PUSCH allocation
 *     
 *  @param[in]  Inst schInst, SCH instance
 *  @param[in]  slot, current slot
 *  @param[out]  msg3StartRb
 *  @param[out]  msg3NumRb
 *  @return  void
 **/
SchPuschInfo* schAllocMsg3Pusch(Inst schInst, uint16_t crnti, uint8_t k2Index, SlotTimingInfo msg3SlotTime)
{
   SchCellCb      *cell          = NULLP;
   SchUlSlotInfo  *schUlSlotInfo = NULLP;
   uint8_t    mcs       = DEFAULT_MCS;
   uint8_t    startSymb = 0;
   uint8_t    symbLen   = 0; 
   uint16_t   startRb   = 0;
   uint16_t   numRb     = 0;
   uint16_t   tbSize    = 0;

   cell = schCb[schInst].cells[schInst];
   if(cell == NULL)
   {
      DU_LOG("\n\nERROR  -->  SCH :  Failed to find cell in schAllocMsg3Pusch");
      return NULLP;
   }

   /* Allocate time-domain and frequency-domain resource for MSG3 PUSCH */
   startSymb = cell->cellCfg.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[k2Index].startSymbol;
   symbLen = cell->cellCfg.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[k2Index].symbolLength;

   startRb = MAX_NUM_RB;
   tbSize = schCalcTbSize(8); /* 6 bytes msg3 and 2 bytes header */
   numRb = schCalcNumPrb(tbSize, mcs, symbLen);
   numRb++; /* allocating 1 extra RB for now */
   allocatePrbUl(cell, msg3SlotTime, startSymb, symbLen, &startRb, numRb);

   /* Fill PUSCH scheduling details in Slot structure */
   schUlSlotInfo = cell->schUlSlotInfo[msg3SlotTime.slot];
   SCH_ALLOC(schUlSlotInfo->schPuschInfo, sizeof(SchPuschInfo));
   if(!schUlSlotInfo->schPuschInfo)
   {
      DU_LOG("\nERROR  -->  SCH :  Memory allocation failed in schAllocMsg3Pusch");
      return NULLP;
   }

   tbSize = 0;  /* since nPrb has been incremented, recalculating tbSize */
   tbSize = schCalcTbSizeFromNPrb(numRb, mcs, NUM_PDSCH_SYMBOL);
   tbSize = tbSize / 8 ; /*bits to byte conversion*/

   schUlSlotInfo->schPuschInfo->crnti             = crnti;
   schUlSlotInfo->schPuschInfo->harqProcId        = SCH_HARQ_PROC_ID;
   schUlSlotInfo->schPuschInfo->resAllocType      = SCH_ALLOC_TYPE_1;
   schUlSlotInfo->schPuschInfo->fdAlloc.startPrb  = startRb;
   schUlSlotInfo->schPuschInfo->fdAlloc.numPrb    = numRb;
   schUlSlotInfo->schPuschInfo->tdAlloc.startSymb = startSymb;
   schUlSlotInfo->schPuschInfo->tdAlloc.numSymb   = symbLen;
   schUlSlotInfo->schPuschInfo->tbInfo.qamOrder   = QPSK_MODULATION;  /* QPSK modulation */
   schUlSlotInfo->schPuschInfo->tbInfo.mcs	     = mcs;
   schUlSlotInfo->schPuschInfo->tbInfo.mcsTable   = SCH_MCS_TABLE_QAM_64;
   schUlSlotInfo->schPuschInfo->tbInfo.ndi        = NEW_TRANSMISSION; /* new transmission */
   schUlSlotInfo->schPuschInfo->tbInfo.rv	        = 0;
   schUlSlotInfo->schPuschInfo->tbInfo.tbSize     = tbSize;
   schUlSlotInfo->schPuschInfo->dmrsMappingType   = DMRS_MAP_TYPE_A;  /* Setting Type-A */
   schUlSlotInfo->schPuschInfo->nrOfDmrsSymbols   = NUM_DMRS_SYMBOLS;
   schUlSlotInfo->schPuschInfo->dmrsAddPos        = DMRS_ADDITIONAL_POS;

   return schUlSlotInfo->schPuschInfo;
}

/**
 * @brief Check if a time frame is in RA Response window
 *
 * @details
 *
 *     Function : isInRaRspWindow
 *
 *     Check if a time frame is in RA Response window
 *
 *  @param[in]  RA request
 *  @param[in]  Time frame to check
 *  @param[in]  Total number of slot per radio frame
 *  @return  true 
 *  @return  false
 **/
RaRspWindowStatus isInRaRspWindow(SchRaReq *raReq, SlotTimingInfo frameToCheck, uint16_t numSlotsPerSystemFrame)
{
   uint32_t winStartTime, winEndTime, timeToCheck;
   
   winStartTime = (raReq->winStartTime.sfn * numSlotsPerSystemFrame) + raReq->winStartTime.slot;
   winEndTime = (raReq->winEndTime.sfn * numSlotsPerSystemFrame) + raReq->winEndTime.slot;
   timeToCheck = (frameToCheck.sfn * numSlotsPerSystemFrame) + frameToCheck.slot;

   /* TODO : check how to handle the wrap around scenario of MAX_SFN */
   if((timeToCheck >= winStartTime) && (timeToCheck <= winEndTime))
      return WITHIN_WINDOW;
   else if(timeToCheck < winStartTime)
      return WINDOW_YET_TO_START;
      
   return WINDOW_EXPIRED;
}

/**
 * @brief Processes any pending RA request
 *
 * @details
 *
 *     Function : schProcessRaReq
 *
 *     This function process pending RA request
 *
 *  @param[in]  Current timing of the cell
 *  @return  ROK
 **/
bool schProcessRaReq(SchCellCb *cell, SlotTimingInfo currTime, uint8_t ueId)
{
   bool      k2Found = false;
   uint8_t   k0TblIdx = 0, k2TblIdx = 0;
   uint8_t   k0Index = 0, k2Index = 0;
   uint8_t   k0 = 0, k2 = 0;
   uint8_t   puschMu = 0;
   uint8_t   msg3Delta = 0, msg3MinSchTime = 0;
#ifdef NR_TDD
   uint8_t   totalCfgSlot = 0;
#endif
   uint16_t             dciSlot = 0, rarSlot = 0;
   SlotTimingInfo       dciTime, rarTime, msg3Time;
   RarAlloc             *dciSlotAlloc = NULLP;    /* Stores info for transmission of PDCCH for RAR */
   RarAlloc             *rarSlotAlloc = NULLP;    /* Stores info for transmission of RAR PDSCH */
   SchPuschInfo         *msg3PuschInfo = NULLP;   /* Stores MSG3 PUSCH scheduling information */
   SchK0K1TimingInfoTbl *k0K1InfoTbl=NULLP;    
   SchK2TimingInfoTbl   *msg3K2InfoTbl=NULLP;
   RaRspWindowStatus    windowStatus=0;

#ifdef NR_TDD
   totalCfgSlot = calculateSlotPatternLength(cell->cellCfg.ssbSchCfg.scsCommon, cell->cellCfg.tddCfg.tddPeriod);
#endif
   k0K1InfoTbl = &cell->cellCfg.schInitialDlBwp.k0K1InfoTbl;
   msg3K2InfoTbl = &cell->cellCfg.schInitialUlBwp.msg3K2InfoTbl;
   puschMu = cell->cellCfg.numerology;
   msg3Delta = puschDeltaTable[puschMu];
   msg3MinSchTime = minMsg3SchTime[cell->cellCfg.numerology];

   /* Calculating time frame to send DCI for RAR */
   ADD_DELTA_TO_TIME(currTime, dciTime, PHY_DELTA_DL + SCHED_DELTA);
   dciSlot = dciTime.slot;
#ifdef NR_TDD
   /* Consider this slot for sending DCI, only if it is a DL slot */
   if(schGetSlotSymbFrmt(dciSlot, cell->slotFrmtBitMap) == DL_SLOT)
#endif
   {
      /* If PDCCH is already scheduled on this slot, cannot schedule PDSCH for another UE here. */
      if(cell->schDlSlotInfo[dciSlot]->pdcchUe != 0)
         return false;

      /* Check if this slot is within RA response window */
      windowStatus = isInRaRspWindow(cell->raReq[ueId-1], dciTime, cell->numSlots);
      if(windowStatus == WITHIN_WINDOW)
      {
         /* For all k0 values, search for a suitable k2 value to schedule MSG3.
          * RAR DCI, RAR PDSCH and MSG3 is scheduled only if one such k0-k2 combination
          * is found. Else no scheduling happens. 
          */
         for(k0TblIdx = 0; k0TblIdx < k0K1InfoTbl->k0k1TimingInfo[dciSlot].numK0; k0TblIdx++)
         {
            k0Index = k0K1InfoTbl->k0k1TimingInfo[dciSlot].k0Indexes[k0TblIdx].k0Index;
            k0 = cell->cellCfg.schInitialDlBwp.pdschCommon.timeDomRsrcAllocList[k0Index].k0;

            /* Calculating time frame to send RAR PDSCH */
            ADD_DELTA_TO_TIME(dciTime, rarTime, k0);
            rarSlot = rarTime.slot;
            
            /* If PDSCH is already scheduled on this slot, cannot schedule PDSCH for another UE here. */
            if(cell->schDlSlotInfo[rarSlot]->pdschUe != 0)
               continue;

            for(k2TblIdx = 0; k2TblIdx < msg3K2InfoTbl->k2TimingInfo[rarSlot].numK2; k2TblIdx++)
            {
               k2Index = msg3K2InfoTbl->k2TimingInfo[rarSlot].k2Indexes[k2TblIdx];
               k2 = cell->cellCfg.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[k2Index].k2;

               /* Delta is added to the slot allocation for msg3 based on 38.214 section 6.1.2.1 */
               k2 = k2 + msg3Delta;
               if(k2 >= msg3MinSchTime)
               {
                  ADD_DELTA_TO_TIME(rarTime, msg3Time, k2);
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
            if(k2Found)
               break;
         }
      }
      else if(windowStatus == WINDOW_EXPIRED)
      {
         SCH_FREE(cell->raReq[ueId-1]->rachInd, sizeof(RachIndInfo));
         SCH_FREE(cell->raReq[ueId-1], sizeof(SchRaReq));
         return false;
      }

      /* If K0-K2 combination not found, no scheduling happens */
      if(!k2Found)
         return false;

      /* Allocate memory for RAR PDCCH slot, pointer will be checked at schProcessSlotInd() */
      SCH_ALLOC(dciSlotAlloc, sizeof(RarAlloc));
      if(dciSlotAlloc == NULLP)
      {
         DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for dciSlotAlloc");
         return false;
      }
      cell->schDlSlotInfo[dciSlot]->rarAlloc[ueId-1] = dciSlotAlloc;

      /* Fill PDCCH and PDSCH scheduling information for RAR */
      if((schFillRar(cell, rarTime, ueId, dciSlotAlloc, k0Index)) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH: Scheduling of RAR failed in slot [%d]", rarSlot);
         SCH_FREE(dciSlotAlloc, sizeof(RarAlloc));
         cell->schDlSlotInfo[dciSlot]->rarAlloc[ueId-1] = NULLP;
         return false;
      }

      /* Allocate resources for msg3 */
      msg3PuschInfo = schAllocMsg3Pusch(cell->instIdx, cell->raReq[ueId-1]->rachInd->crnti, k2Index, msg3Time);
      if(msg3PuschInfo)
      {
         /* Fill RAR info */
         dciSlotAlloc->rarInfo.raRnti = cell->raReq[ueId-1]->raRnti;
         dciSlotAlloc->rarInfo.tcrnti = cell->raReq[ueId-1]->rachInd->crnti;
         dciSlotAlloc->rarInfo.RAPID = cell->raReq[ueId-1]->rachInd->preambleIdx;
         dciSlotAlloc->rarInfo.ta = cell->raReq[ueId-1]->rachInd->timingAdv;
         dciSlotAlloc->rarInfo.ulGrant.bwpSize = cell->cellCfg.schInitialUlBwp.bwp.freqAlloc.numPrb;
         /* Spec 38.213, section 8.2, 0 : MSG3 PUSCH will be transmitted without frequency hopping */
         dciSlotAlloc->rarInfo.ulGrant.freqHopFlag = 0;
         dciSlotAlloc->rarInfo.ulGrant.msg3FreqAlloc.startPrb = msg3PuschInfo->fdAlloc.startPrb;
         dciSlotAlloc->rarInfo.ulGrant.msg3FreqAlloc.numPrb = msg3PuschInfo->fdAlloc.numPrb;
         dciSlotAlloc->rarInfo.ulGrant.k2Index = k2Index;
         dciSlotAlloc->rarInfo.ulGrant.mcs = msg3PuschInfo->tbInfo.mcs;
         dciSlotAlloc->rarInfo.ulGrant.tpc = 3;  /* TODO : Check appropriate value to be filled */
         /* Spec 38.213, section 8.2 : In a contention based random access
          * procedure, the CSI request field is reserved. */
         dciSlotAlloc->rarInfo.ulGrant.csiReq = 0;
      }

      /* Check if both DCI and RAR are sent in the same slot.
       * If not, allocate memory RAR PDSCH slot to store RAR info
       */
      if(dciSlot == rarSlot)
         dciSlotAlloc->pduPres = BOTH;
      else
      {
         /* Allocate memory to schedule rarSlot to send RAR, pointer will be checked at schProcessSlotInd() */
         SCH_ALLOC(rarSlotAlloc, sizeof(RarAlloc));
         if(rarSlotAlloc == NULLP)
         {
            DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for rarSlotAlloc");
            SCH_FREE(dciSlotAlloc, sizeof(RarAlloc));
            cell->schDlSlotInfo[dciSlot]->rarAlloc[ueId-1] = NULLP;
            return false;
         }
         cell->schDlSlotInfo[rarSlot]->rarAlloc[ueId-1] = rarSlotAlloc;

         /* Copy all RAR info */
         memcpy(rarSlotAlloc, dciSlotAlloc, sizeof(RarAlloc));
         rarSlotAlloc->rarPdcchCfg.dci.pdschCfg = &rarSlotAlloc->rarPdschCfg;

         /* Assign correct PDU types in corresponding slots */
         rarSlotAlloc->pduPres = PDSCH_PDU;
         dciSlotAlloc->pduPres = PDCCH_PDU;
         dciSlotAlloc->pdschSlot = rarSlot;  
      }

      cell->schDlSlotInfo[dciSlot]->pdcchUe = ueId;
      cell->schDlSlotInfo[rarSlot]->pdschUe = ueId;
      cell->schUlSlotInfo[msg3Time.slot]->puschUe = ueId;

      /* Create raCb at SCH */
      createSchRaCb(cell->raReq[ueId-1]->rachInd->crnti, cell->instIdx);

      /* Remove RachInd from pending RA request list */
      SCH_FREE(cell->raReq[ueId-1]->rachInd, sizeof(RachIndInfo));
      SCH_FREE(cell->raReq[ueId-1], sizeof(SchRaReq));
      
      return true;
   }
   return false;
}

/**
 * @brief process rach indication function. 
 *
 * @details
 *
 *     Function : schProcessRachInd
 *     
 *     This function process rach indication
 *     
 *  @param[in]  rachInd parameters
 *  @param[in]  shed instance
 *  @return  ROK
 **/
uint8_t schProcessRachInd(RachIndInfo *rachInd, Inst schInst)
{
   SchCellCb *cell = schCb[schInst].cells[schInst];
   SchRaReq  *raReq = NULLP;
   float    slotDuration;
   uint8_t  winNumSlots;
   uint8_t  ueId;

   if(cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH: Failed to find cell in schProcessRachInd");
      return RFAILED;
   }

   /* Storing RA request in cellCb */
   GET_UE_ID(rachInd->crnti, ueId);
   if(ueId <= 0)
   {
      DU_LOG("\nERROR  -->  SCH: Invalid CRNTI [%d]", rachInd->crnti);
      return RFAILED;
   }

   SCH_ALLOC(raReq, sizeof(SchRaReq));
   if(!raReq)
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation failure in schProcessRachInd");
      SCH_FREE(rachInd, sizeof(RachIndInfo));
      return RFAILED;
   }

   /* calculate the ra-rnti value */
   raReq->raRnti = calculateRaRnti(rachInd->symbolIdx, rachInd->slotIdx, rachInd->freqIdx);
   raReq->rachInd = rachInd;
   raReq->winStartTime.sfn = rachInd->timingInfo.sfn;
   raReq->winStartTime.slot = rachInd->timingInfo.slot;
  
   /* Converting window size from ms to number of slots */
   slotDuration = (1 / pow(2, cell->cellCfg.numerology));
   winNumSlots = (float)cell->cellCfg.schRachCfg.raRspWindow / slotDuration;
   
   /* Adding window size to window start time to get window end time */
   ADD_DELTA_TO_TIME(raReq->winStartTime, raReq->winEndTime, winNumSlots);
   cell->raReq[ueId -1] = raReq;

   /* Adding UE Id to list of pending UEs to be scheduled */
   addUeToBeScheduled(cell, ueId);

   return ROK;
}

/**
 * @brief fill RAR info function. 
 *
 * @details
 *
 *     Function : calculateRaRnti
 *     
 *     This function fills pdcch and pdsch info for RAR
 *     
 *  @param[in]  rar Allocation info
 *  @param[in]  ra-rnti
 *  @param[in]  PCI
 *  @param[in]  offset to pointA to determine freq alloc
 *  @return  ROK
 **/
uint8_t schFillRar(SchCellCb *cell, SlotTimingInfo rarTime, uint16_t ueId, RarAlloc *rarAlloc, uint8_t k0Index)
{
   uint8_t  coreset0Idx = 0;
   uint8_t  firstSymbol = 0, numSymbols = 0;
   uint8_t  mcs = DEFAULT_MCS;  /* MCS fixed to 4 */
   uint8_t  dmrsStartSymbol, startSymbol, numSymbol ;
   uint16_t numRbs = 0;
   uint16_t tbSize = 0;

   SchBwpDlCfg *initialBwp = &cell->cellCfg.schInitialDlBwp;
   PdcchCfg *pdcch = &rarAlloc->rarPdcchCfg;
   PdschCfg *pdsch = &rarAlloc->rarPdschCfg;
   BwpCfg *bwp = &rarAlloc->bwp;

   /* derive the sib1 coreset0 params from table 13-1 spec 38.213 */
   coreset0Idx     = initialBwp->pdcchCommon.commonSearchSpace.coresetId;
   numRbs     = coresetIdxTable[coreset0Idx][1]; 
   numSymbols = coresetIdxTable[coreset0Idx][2];

   /* calculate time domain parameters */
   // note: since slot value is made sl1, RAR can be sent at all slots
   uint16_t mask = 0x2000;
   for(firstSymbol=0; firstSymbol<MAX_SYMB_PER_SLOT; firstSymbol++)
   {
      if(initialBwp->pdcchCommon.commonSearchSpace.monitoringSymbol & mask)
	 break;
      else
	 mask = mask>>1;
   }

   /* fill BWP */
   bwp->freqAlloc.numPrb   = initialBwp->bwp.freqAlloc.numPrb;
   bwp->freqAlloc.startPrb = initialBwp->bwp.freqAlloc.startPrb;
   bwp->subcarrierSpacing  = initialBwp->bwp.scs;
   bwp->cyclicPrefix       = initialBwp->bwp.cyclicPrefix;

   /* fill the PDCCH PDU */
   pdcch->coresetCfg.startSymbolIndex = firstSymbol;
   pdcch->coresetCfg.durationSymbols = numSymbols;
   memcpy(pdcch->coresetCfg.freqDomainResource, \
      cell->cellCfg.schInitialDlBwp.pdcchCommon.commonSearchSpace.freqDomainRsrc, FREQ_DOM_RSRC_SIZE);

   pdcch->coresetCfg.cceRegMappingType = 1; /* coreset0 is always interleaved */
   pdcch->coresetCfg.regBundleSize = 6;    /* spec-38.211 sec 7.3.2.2 */
   pdcch->coresetCfg.interleaverSize = 2;  /* spec-38.211 sec 7.3.2.2 */
   pdcch->coresetCfg.coreSetType = 0;
   pdcch->coresetCfg.coreSetSize = numRbs;
   pdcch->coresetCfg.shiftIndex = cell->cellCfg.phyCellId;
   pdcch->coresetCfg.precoderGranularity = 0; /* sameAsRegBundle */
   pdcch->numDlDci = 1;
   pdcch->dci.rnti = cell->raReq[ueId-1]->raRnti; /* RA-RNTI */
   pdcch->dci.scramblingId = cell->cellCfg.phyCellId;
   pdcch->dci.scramblingRnti = 0;
   pdcch->dci.cceIndex = 4; /* considering SIB1 is sent at cce 0-1-2-3 */
   pdcch->dci.aggregLevel = 4;
   pdcch->dci.beamPdcchInfo.numPrgs = 1;
   pdcch->dci.beamPdcchInfo.prgSize = 1;
   pdcch->dci.beamPdcchInfo.digBfInterfaces = 0;
   pdcch->dci.beamPdcchInfo.prg[0].pmIdx = 0;
   pdcch->dci.beamPdcchInfo.prg[0].beamIdx[0] = 0;
   pdcch->dci.txPdcchPower.powerValue = 0;
   pdcch->dci.txPdcchPower.powerControlOffsetSS = 0;
   pdcch->dci.pdschCfg = pdsch;

   /* fill the PDSCH PDU */
   uint8_t cwCount = 0;
   pdsch->pduBitmap = 0; /* PTRS and CBG params are excluded */
   pdsch->rnti = cell->raReq[ueId-1]->raRnti; /* RA-RNTI */
   pdsch->pduIndex = 0;
   pdsch->numCodewords = 1;
   for(cwCount = 0; cwCount < pdsch->numCodewords; cwCount++)
   {
      pdsch->codeword[cwCount].targetCodeRate = 308;
      pdsch->codeword[cwCount].qamModOrder = 2;
      pdsch->codeword[cwCount].mcsIndex = mcs; /* mcs configured to 4 */
      pdsch->codeword[cwCount].mcsTable = 0;   /* notqam256 */
      pdsch->codeword[cwCount].rvIndex = 0;
      /* RAR PDU length and FAPI payload header length */
      tbSize = schCalcTbSize(RAR_PAYLOAD_SIZE + TX_PAYLOAD_HDR_LEN);
      pdsch->codeword[cwCount].tbSize = tbSize;
   }
   pdsch->dataScramblingId = cell->cellCfg.phyCellId;
   pdsch->numLayers = 1;
   pdsch->transmissionScheme = 0;
   pdsch->refPoint = 0;
   pdsch->dmrs.dlDmrsSymbPos = 4;  /* Bitmap value 00000000000100 i.e. using 3rd symbol for PDSCH DMRS */
   pdsch->dmrs.dmrsConfigType = 0; /* type-1 */
   pdsch->dmrs.dlDmrsScramblingId = cell->cellCfg.phyCellId;
   pdsch->dmrs.scid = 0;
   pdsch->dmrs.numDmrsCdmGrpsNoData = 1;
   pdsch->dmrs.dmrsPorts = 0;
   pdsch->dmrs.mappingType      = DMRS_MAP_TYPE_A;  /* Type-A */
   pdsch->dmrs.nrOfDmrsSymbols  = NUM_DMRS_SYMBOLS;
   pdsch->dmrs.dmrsAddPos       = DMRS_ADDITIONAL_POS;

   pdsch->pdschTimeAlloc.rowIndex = k0Index;
   pdsch->pdschTimeAlloc.timeAlloc.startSymb = initialBwp->pdschCommon.timeDomRsrcAllocList[k0Index].startSymbol;
   pdsch->pdschTimeAlloc.timeAlloc.numSymb = initialBwp->pdschCommon.timeDomRsrcAllocList[k0Index].lengthSymbol;

   pdsch->pdschFreqAlloc.vrbPrbMapping = 0; /* non-interleaved */
   pdsch->pdschFreqAlloc.resourceAllocType = 1; /* RAT type-1 RIV format */
   pdsch->pdschFreqAlloc.freqAlloc.startPrb = MAX_NUM_RB;
   pdsch->pdschFreqAlloc.freqAlloc.numPrb = \
      schCalcNumPrb(tbSize, mcs, initialBwp->pdschCommon.timeDomRsrcAllocList[k0Index].lengthSymbol);

   /* Find total symbols occupied including DMRS */
   dmrsStartSymbol = findDmrsStartSymbol(pdsch->dmrs.dlDmrsSymbPos);
   /* If there are no DRMS symbols, findDmrsStartSymbol() returns MAX_SYMB_PER_SLOT, 
    * in that case only PDSCH symbols are marked as occupied */
   if(dmrsStartSymbol == MAX_SYMB_PER_SLOT)
   {
      startSymbol = pdsch->pdschTimeAlloc.timeAlloc.startSymb;
      numSymbol = pdsch->pdschTimeAlloc.timeAlloc.numSymb;
   }
   /* If DMRS symbol is found, mark DMRS and PDSCH symbols as occupied */
   else
   {
      startSymbol = dmrsStartSymbol;
      numSymbol = pdsch->dmrs.nrOfDmrsSymbols + pdsch->pdschTimeAlloc.timeAlloc.numSymb;
   }

   /* Allocate the number of PRBs required for RAR PDSCH */
   if((allocatePrbDl(cell, rarTime, startSymbol, numSymbol,\
      &pdsch->pdschFreqAlloc.freqAlloc.startPrb, pdsch->pdschFreqAlloc.freqAlloc.numPrb)) != ROK)
   {
      DU_LOG("\nERROR  --> SCH : allocatePrbDl() failed for RAR");
      return RFAILED;
   }

   pdsch->beamPdschInfo.numPrgs = 1;
   pdsch->beamPdschInfo.prgSize = 1;
   pdsch->beamPdschInfo.digBfInterfaces = 0;
   pdsch->beamPdschInfo.prg[0].pmIdx = 0;
   pdsch->beamPdschInfo.prg[0].beamIdx[0] = 0;
   pdsch->txPdschPower.powerControlOffset = 0;
   pdsch->txPdschPower.powerControlOffsetSS = 0;

   return ROK;
}

/**********************************************************************
         End of file
**********************************************************************/
