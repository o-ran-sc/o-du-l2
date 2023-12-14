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
#include "sch_tmr.h"
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

   prachCfgIdx      = cell->cellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.prachCfgGeneric.prachCfgIdx;

   /* derive the prachCfgIdx table paramters */
   x                = prachCfgIdxTable[prachCfgIdx][1];
   y                = prachCfgIdxTable[prachCfgIdx][2];
   prachSubframe    = prachCfgIdxTable[prachCfgIdx][3];

   if((prachOccasionTimingInfo.sfn%x) == y)
   {
      subFrame = prachOccasionTimingInfo.slot/pow(2, cell->numerology);

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
   uint16_t puschScs = convertScsEnumValToScsVal(cell->cellCfg.ulCfgCommon.schInitialUlBwp.bwp.scs);

   for(tableIdx=0; tableIdx < MAX_RACH_NUM_RB_IDX; tableIdx++)
   {
      if((numRbForPrachTable[tableIdx][0] == cell->cellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.rootSeqLen) &&
            (numRbForPrachTable[tableIdx][1] == cell->cellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.msg1SubcSpacing) &&
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

   prachCfgIdx      = cell->cellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.prachCfgGeneric.prachCfgIdx;
   prachFormat      = prachCfgIdxTable[prachCfgIdx][0];
   prachStartSymbol = prachCfgIdxTable[prachCfgIdx][4];
   prachOcas        = prachCfgIdxTable[prachCfgIdx][6];
   prachDuration    = prachCfgIdxTable[prachCfgIdx][7];

   /* numRa determined as 𝑛 belonging {0,1,.., M − 1},
    * where M is given by msg1Fdm */
   numRa = (cell->cellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.prachCfgGeneric.msg1Fdm - 1);

   /* freq domain resource determination for RACH*/
   freqStart = cell->cellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.prachCfgGeneric.msg1FreqStart;
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
   DU_LOG("\nINFO   -->  SCH : RACH occassion set for slot %d", prachOccasionTimingInfo.slot);
}

/**
 * @brief Process RACH resource request for CFRA
 *
 * @details
 *
 *     Function : SchProcRachRsrcReq
 *     
 *     This function processes RACH resorce request 
 *     from MAC for CFRA. It assigns a dedicated preamble
 *     to the UE and sends the same in RACH resource
 *     response
 *     
 *  @param[in]  Post structure
 *  @param[in]  RACH resource request
 *  @return     ROK
 *              RFAILED
 **/
uint8_t SchProcRachRsrcReq(Pst *pst, SchRachRsrcReq *schRachRsrcReq)
{
   uint8_t      ssbIdx = 0, cfraSsbIdx = 0;
   uint8_t      firstCFPreambleIndex = 0, lastCFPreambleIndex = 0;
   uint16_t     cellIdx = 0;
   uint64_t     mask = 0;
   Pst          rspPst;
   Inst         inst = pst->dstInst - SCH_INST_START;
   SchCellCb    *cellCb = NULLP;
   SchUeCb      *ueCb = NULLP;
   SchRachRsrcRsp *rachRsrcRsp = NULLP;

   DU_LOG("\nINFO  -->  SCH : Received RACH resource request for Cell ID [%d] CRNTI [%d]", \
         schRachRsrcReq->cellId, schRachRsrcReq->crnti);

 /* Fill RACH resource response to MAC */
   SCH_ALLOC(rachRsrcRsp, sizeof(SchRachRsrcRsp));
   if(!rachRsrcRsp)
   {   
      DU_LOG("\nERROR  -->  SCH : Memory allocation failed for RACH resource response");
      return RFAILED;
   }   
   rachRsrcRsp->cellId = schRachRsrcReq->cellId;
   rachRsrcRsp->crnti = schRachRsrcReq->crnti;
   rachRsrcRsp->result = RSP_OK;

   /* Fill SCH to MAC Pst structure */
   memset(&rspPst, 0, sizeof(Pst));
   FILL_PST_SCH_TO_MAC(rspPst, inst);
   rspPst.event = EVENT_RACH_RESOURCE_RESPONSE_TO_MAC;

   /* Fetch Cell CB */
   for(cellIdx = 0; cellIdx < MAX_NUM_CELL; cellIdx++)
   {
      if((schCb[inst].cells[cellIdx]) && (schCb[inst].cells[cellIdx]->cellId == schRachRsrcReq->cellId))
      {
         cellCb = schCb[inst].cells[cellIdx];
         break;
      }
   }
   
   if(cellCb)
   {
      /* Fetch UE CB */
      ueCb = schGetUeCb(cellCb, schRachRsrcReq->crnti);
      if(ueCb->crnti != schRachRsrcReq->crnti)
      {
         DU_LOG("\nERROR  -->  SCH : CRNTI [%d] not found" ,schRachRsrcReq->crnti);
         rachRsrcRsp->result = RSP_NOK;
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  SCH : Cell ID [%d] not found" ,schRachRsrcReq->cellId);
      rachRsrcRsp->result = RSP_NOK;
   }

   /* Allocate SSB resource if no failure has occurred until this step */
   if(rachRsrcRsp->result == RSP_OK)
   {
      /* Find first free preamble index from the pool CF preambles 
       * Preamble index from 0 to (numCbPreamblePerSsb-1) is used for CBRA 
       * Preamble index from numCbPreamblePerSsb to totalNumOfRAPreamble
       * is used for CFRA */
      firstCFPreambleIndex = cellCb->cellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.numCbPreamblePerSsb;
      lastCFPreambleIndex = cellCb->cellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.totalNumRaPreamble;

      /* Allocate resource for each SSB index requested */
      for(ssbIdx = 0; ssbIdx < schRachRsrcReq->numSsb; ssbIdx++)
      {
         /* Find the first CF Preamble index not dedicated to any UE currently */
         while(firstCFPreambleIndex <= lastCFPreambleIndex)
         {
            mask = 1 << firstCFPreambleIndex;
            if(cellCb->dedPreambleBitMap & mask)
            {
               firstCFPreambleIndex++;
               continue;
            }
            else
               break;
         }

         /* If firstCFPreambleIndex > lastCFPreambleIndex, it means all
          * dedicated preambles are in use currently. In such a case, CBRA
          * should be initiated. 
          * If a dedicated preamble is found, use this for CFRA and mark it as
          * IN-USE in the bitmap.
          * Considering only CFRA scenario for now. */
         if(firstCFPreambleIndex <= lastCFPreambleIndex)
         {
            ueCb->cfraResource.ssbResource[cfraSsbIdx].ssbIdx = schRachRsrcReq->ssbIdx[ssbIdx]; 
            ueCb->cfraResource.ssbResource[cfraSsbIdx].raPreambleIdx = firstCFPreambleIndex;
            SET_ONE_BIT(firstCFPreambleIndex, cellCb->dedPreambleBitMap);
            cfraSsbIdx++;
            firstCFPreambleIndex++;
         }
         else
         {
            DU_LOG("\nINFO : SCH : No dedicated preameble availble to assign to ssbIdx[%d]", schRachRsrcReq->ssbIdx[ssbIdx]);
            /* Breaking out of for loop since no dedicated preambles are available
             * for remaining ssbIdx too */
            break;
         }
      } /* End of for */

      ueCb->cfraResource.numSsb = cfraSsbIdx;

      if(ueCb->cfraResource.numSsb == 0)
      {
         /* If numSsb is 0, it means no CFRA resource was alloacted for any of the
          * SSB Idx, hence send a negative response */
         rachRsrcRsp->result = RSP_NOK;
      }
      else
      {   
         /* Send ssb resource information to MAC in RACH resource response */
         rachRsrcRsp->cfraResource.numSsb = ueCb->cfraResource.numSsb;
         memcpy(rachRsrcRsp->cfraResource.ssbResource, ueCb->cfraResource.ssbResource, \
            ueCb->cfraResource.numSsb * sizeof(SchCfraSsbResource));
      }
   } /* End of if */

   /* Free RACH resource request memory allocated by MAC */
   SCH_FREE(schRachRsrcReq, sizeof(SchRachRsrcReq));

   /* Send RACH resource response to MAC */
   return(MacMessageRouter(&rspPst, (void *)rachRsrcRsp));
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
 *  @param[in]  crnti
 *  @param[in]  shed instance
 *  @return  void
 **/
void createSchRaCb(uint8_t ueId, SchRaReq *raReq, Inst schInst)
{
   if(raReq->isCFRA)
   {
      /* If a UE in handover has triggered CFRA, its UE CB context is already present in SCH, 
       * Hence, no need to create raCb */
      if(raReq->ueCb && (raReq->ueCb->state == SCH_UE_HANDIN_IN_PROGRESS))
      {
         schCb[schInst].cells[schInst]->numActvUe++;
         SET_ONE_BIT(raReq->ueCb->ueId, schCb[schInst].cells[schInst]->actvUeBitMap);
         raReq->ueCb->state = SCH_UE_STATE_ACTIVE;
         schCb[schInst].cells[schInst]->raCb[ueId -1].raState = SCH_RA_STATE_MSG4_DONE;
      }
   }
   else
   {
      /* Create RA CB only for CB-RA to use for msg3 and msg4 processing */
      GET_UE_ID(raReq->rachInd->crnti, ueId);
      schCb[schInst].cells[schInst]->raCb[ueId -1].tcrnti = raReq->rachInd->crnti;
      schCb[schInst].cells[schInst]->raCb[ueId -1].msg4recvd = FALSE;
      schCb[schInst].cells[schInst]->raCb[ueId -1].raState = SCH_RA_STATE_MSG3_PENDING;
   }
   schCb[schInst].cells[schInst]->raCb[ueId -1].cell = schCb[schInst].cells[schInst];
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
SchPuschInfo* schAllocMsg3Pusch(Inst schInst, uint16_t crnti, uint8_t k2Index, SlotTimingInfo msg3SlotTime, SchUlHqProcCb* msg3HqProc, bool isRetx)
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
   startSymb = cell->cellCfg.ulCfgCommon.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[k2Index].startSymbol;
   symbLen = cell->cellCfg.ulCfgCommon.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[k2Index].symbolLength;

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

   schUlSlotInfo->schPuschInfo->harqProcId        = msg3HqProc->procId;
   schUlSlotInfo->schPuschInfo->fdAlloc.resAllocType      = SCH_ALLOC_TYPE_1;
   schUlSlotInfo->schPuschInfo->fdAlloc.resAlloc.type1.startPrb  = startRb;
   schUlSlotInfo->schPuschInfo->fdAlloc.resAlloc.type1.numPrb    = numRb;
   schUlSlotInfo->schPuschInfo->tdAlloc.startSymb = startSymb;
   schUlSlotInfo->schPuschInfo->tdAlloc.numSymb   = symbLen;
   schUlSlotInfo->schPuschInfo->tbInfo.qamOrder   = QPSK_MODULATION;  /* QPSK modulation */
   schUlSlotInfo->schPuschInfo->tbInfo.mcs	     = mcs;
   schUlSlotInfo->schPuschInfo->tbInfo.mcsTable   = SCH_MCS_TABLE_QAM_64;
   schUlSlotInfo->schPuschInfo->tbInfo.ndi        = NEW_TRANSMISSION; /* new transmission */
   schUlSlotInfo->schPuschInfo->tbInfo.rv	        = 0;
   schUlSlotInfo->schPuschInfo->tbInfo.tbSize     = tbSize;
#ifdef INTEL_FAPI   
   schUlSlotInfo->schPuschInfo->dmrsMappingType   = DMRS_MAP_TYPE_A;  /* Setting Type-A */
   schUlSlotInfo->schPuschInfo->nrOfDmrsSymbols   = NUM_DMRS_SYMBOLS;
   schUlSlotInfo->schPuschInfo->dmrsAddPos        = DMRS_ADDITIONAL_POS;
#endif   
   
   if(!isRetx)
   {
      msg3HqProc->strtSymbl = startSymb;
      msg3HqProc->numSymbl = symbLen;
      msg3HqProc->puschResType = schUlSlotInfo->schPuschInfo->fdAlloc.resAllocType;
      msg3HqProc->puschStartPrb = schUlSlotInfo->schPuschInfo->fdAlloc.resAlloc.type1.startPrb;
      msg3HqProc->puschNumPrb = schUlSlotInfo->schPuschInfo->fdAlloc.resAlloc.type1.numPrb;
      msg3HqProc->tbInfo.qamOrder = schUlSlotInfo->schPuschInfo->tbInfo.qamOrder;
      msg3HqProc->tbInfo.iMcs = schUlSlotInfo->schPuschInfo->tbInfo.mcs;
      msg3HqProc->tbInfo.mcsTable = schUlSlotInfo->schPuschInfo->tbInfo.mcsTable;
      msg3HqProc->tbInfo.ndi = schUlSlotInfo->schPuschInfo->tbInfo.ndi;
      msg3HqProc->tbInfo.rv = schUlSlotInfo->schPuschInfo->tbInfo.rv;
      msg3HqProc->tbInfo.tbSzReq = schUlSlotInfo->schPuschInfo->tbInfo.tbSize;
#ifdef INTEL_FAPI      
      msg3HqProc->dmrsMappingType = schUlSlotInfo->schPuschInfo->dmrsMappingType;
      msg3HqProc->nrOfDmrsSymbols = schUlSlotInfo->schPuschInfo->nrOfDmrsSymbols;
      msg3HqProc->dmrsAddPos = schUlSlotInfo->schPuschInfo->dmrsAddPos;
#endif
   }
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
bool schProcessRaReq(Inst schInst, SchCellCb *cell, SlotTimingInfo currTime, uint8_t ueId)
{
   bool      k1Found = false, k2Found = false;
   uint8_t   k0TblIdx = 0, k1TblIdx = 0, k2TblIdx = 0;
   uint8_t   k0Index = 0, k1Index = 0, k2Index = 0;
   uint8_t   k0 = 0, k1 = 0, k2 = 0;
   uint8_t   numK1 = 0;
   uint8_t   puschMu = 0;
   uint8_t   msg3Delta = 0, msg3MinSchTime = 0;
#ifdef NR_TDD
   uint8_t   totalCfgSlot = 0;
#endif
   uint16_t             dciSlot = 0, rarSlot = 0;
   SlotTimingInfo       dciTime, rarTime, msg3Time, pucchTime;
   RarAlloc             *dciSlotAlloc = NULLP;    /* Stores info for transmission of PDCCH for RAR */
   RarAlloc             *rarSlotAlloc = NULLP;    /* Stores info for transmission of RAR PDSCH */
   SchPuschInfo         *msg3PuschInfo = NULLP;   /* Stores MSG3 PUSCH scheduling information */
   SchK0K1TimingInfoTbl *k0K1InfoTbl=NULLP;    
   SchK2TimingInfoTbl   *msg3K2InfoTbl=NULLP;
   RaRspWindowStatus    windowStatus=0;
   
#ifdef NR_TDD
   totalCfgSlot = calculateSlotPatternLength(cell->cellCfg.ssbScs, cell->cellCfg.tddCfg.tddPeriod);
#endif
   k0K1InfoTbl    = &cell->k0K1InfoTbl;
   if(cell->raReq[ueId-1]->isCFRA == false)
   {
      msg3K2InfoTbl  = &cell->msg3K2InfoTbl;
      puschMu        = cell->numerology;
      msg3Delta      = puschDeltaTable[puschMu];
      msg3MinSchTime = minMsg3SchTime[cell->numerology];
   }

   /* Calculating time frame to send DCI for RAR */
   ADD_DELTA_TO_TIME(currTime, dciTime, PHY_DELTA_DL + SCHED_DELTA, cell->numSlots);
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
            k0 = cell->cellCfg.dlCfgCommon.schInitialDlBwp.pdschCommon.timeDomRsrcAllocList[k0Index].k0;

            /* Calculating time frame to send RAR PDSCH */
            ADD_DELTA_TO_TIME(dciTime, rarTime, k0, cell->numSlots);
            rarSlot = rarTime.slot;
            
            /* If Contention-FREE RA is in progress, allocate resources for
             * PUCCH for next UL message */
            if(cell->raReq[ueId-1]->isCFRA)
            {
               numK1 = k0K1InfoTbl->k0k1TimingInfo[dciTime.slot].k0Indexes[k0TblIdx].k1TimingInfo.numK1;
               for(k1TblIdx = 0; k1TblIdx < numK1; k1TblIdx++)
               {   
                  k1Index = k0K1InfoTbl->k0k1TimingInfo[dciTime.slot].k0Indexes[k0TblIdx].k1TimingInfo.k1Indexes[k1TblIdx];
                  if(cell->raReq[ueId-1]->ueCb->ueCfg.spCellCfg.servCellRecfg.initUlBwp.pucchCfg.dlDataToUlAck)
                  {
                     k1 = cell->raReq[ueId-1]->ueCb->ueCfg.spCellCfg.servCellRecfg.initUlBwp.pucchCfg.dlDataToUlAck->\
                        dlDataToUlAckList[k1Index];
                  }
                  else
                  {
                     k1 = defaultUlAckTbl[k1Index];
                  }

                  ADD_DELTA_TO_TIME(rarTime, pucchTime, k1, cell->numSlots);
#ifdef NR_TDD
                  if(schGetSlotSymbFrmt(pucchTime.slot, cell->slotFrmtBitMap) == DL_SLOT)
                     continue;
#endif
                  if(cell->schUlSlotInfo[pucchTime.slot]->pucchUe != 0)
                     continue;
                  k1Found = true;
                  break;
               }
            }
            else
            {
               /* Else if contention-based RA is in progress, allocate resources for MSG3 */
               for(k2TblIdx = 0; k2TblIdx < msg3K2InfoTbl->k2TimingInfo[rarSlot].numK2; k2TblIdx++)
               {
                  k2Index = msg3K2InfoTbl->k2TimingInfo[rarSlot].k2Indexes[k2TblIdx];
                  k2 = cell->cellCfg.ulCfgCommon.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[k2Index].k2;

                  /* Delta is added to the slot allocation for msg3 based on 38.214 section 6.1.2.1 */
                  k2 = k2 + msg3Delta;
                  if(k2 >= msg3MinSchTime)
                  {
                     ADD_DELTA_TO_TIME(rarTime, msg3Time, k2, cell->numSlots);
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
            if(k1Found || k2Found)
               break;
         }
      }
      else if(windowStatus == WINDOW_EXPIRED)
      {
         SCH_FREE(cell->raReq[ueId-1]->rachInd, sizeof(RachIndInfo));
         SCH_FREE(cell->raReq[ueId-1], sizeof(SchRaReq));
         return false;
      }

      /* If K0-K2 and K0-K1 combination not found, no scheduling happens */
      if(!k1Found && !k2Found)
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
         if(!dciSlotAlloc->rarPdschCfg)
         {
            SCH_FREE(dciSlotAlloc, sizeof(RarAlloc));
            cell->schDlSlotInfo[dciSlot]->rarAlloc[ueId-1] = NULLP;
         }
         return false;
      }

      /* Fill RAR info */
      dciSlotAlloc->raRnti = cell->raReq[ueId-1]->raRnti;
      dciSlotAlloc->rarInfo.tcrnti = cell->raReq[ueId-1]->rachInd->crnti;
      dciSlotAlloc->rarInfo.RAPID = cell->raReq[ueId-1]->rachInd->preambleIdx;
      dciSlotAlloc->rarInfo.ta = cell->raReq[ueId-1]->rachInd->timingAdv;

      if(cell->raReq[ueId-1]->isCFRA)
      {
         /* Allocate resources for PUCCH */
         schAllocPucchResource(cell, pucchTime, cell->raReq[ueId-1]->rachInd->crnti,NULLP, FALSE, NULLP);
      }
      else
      {
         /* Allocate resources for msg3 */
         msg3PuschInfo = schAllocMsg3Pusch(schInst, cell->raReq[ueId-1]->rachInd->crnti, k2Index, msg3Time, &(cell->raCb[ueId-1].msg3HqProc), FALSE);
         if(msg3PuschInfo)
         {
            dciSlotAlloc->rarInfo.ulGrant.bwpSize = cell->cellCfg.ulCfgCommon.schInitialUlBwp.bwp.freqAlloc.numPrb;
            /* Spec 38.213, section 8.2, 0 : MSG3 PUSCH will be transmitted without frequency hopping */
            dciSlotAlloc->rarInfo.ulGrant.freqHopFlag = 0;
            dciSlotAlloc->rarInfo.ulGrant.msg3FreqAlloc.startPrb = msg3PuschInfo->fdAlloc.resAlloc.type1.startPrb;
            dciSlotAlloc->rarInfo.ulGrant.msg3FreqAlloc.numPrb = msg3PuschInfo->fdAlloc.resAlloc.type1.numPrb;
            dciSlotAlloc->rarInfo.ulGrant.k2Index = k2Index;
            dciSlotAlloc->rarInfo.ulGrant.mcs = msg3PuschInfo->tbInfo.mcs;
            dciSlotAlloc->rarInfo.ulGrant.tpc = 3;  /* TODO : Check appropriate value to be filled */
            /* Spec 38.213, section 8.2 : In a contention based random access
             * procedure, the CSI request field is reserved. */
            dciSlotAlloc->rarInfo.ulGrant.csiReq = 0;
         }
      }

      /* Check if both DCI and RAR are sent in the same slot.
       * If not, allocate memory RAR PDSCH slot to store RAR info
       */
      if(dciSlot == rarSlot)
      {
         SCH_ALLOC(dciSlotAlloc->rarPdschCfg, sizeof(PdschCfg));
         if(dciSlotAlloc->rarPdschCfg)
         {
             memcpy(dciSlotAlloc->rarPdschCfg, &dciSlotAlloc->rarPdcchCfg->dci[0].pdschCfg, sizeof(PdschCfg));
         }
         else
         {
            SCH_FREE(dciSlotAlloc->rarPdcchCfg, sizeof(PdcchCfg));
            SCH_FREE(dciSlotAlloc, sizeof(RarAlloc));
            cell->schDlSlotInfo[dciSlot]->rarAlloc[ueId-1] = NULLP;
            DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for dciSlotAlloc->rarPdschCfg");
            return false;
         }

      }
      else
      {
         /* Allocate memory to schedule rarSlot to send RAR, pointer will be checked at schProcessSlotInd() */
         SCH_ALLOC(rarSlotAlloc, sizeof(RarAlloc));
         if(rarSlotAlloc == NULLP)
         {
            DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for rarSlotAlloc");
            SCH_FREE(dciSlotAlloc->rarPdcchCfg, sizeof(PdcchCfg));
            if(!dciSlotAlloc->rarPdschCfg)
            {
               SCH_FREE(dciSlotAlloc, sizeof(RarAlloc));
               cell->schDlSlotInfo[dciSlot]->rarAlloc[ueId-1] = NULLP;
            }
            return false;
         }
         cell->schDlSlotInfo[rarSlot]->rarAlloc[ueId-1] = rarSlotAlloc;

         /* Copy all RAR info */
         rarSlotAlloc->raRnti = dciSlotAlloc->raRnti;
         rarSlotAlloc->bwp = dciSlotAlloc->bwp;
         SCH_ALLOC(rarSlotAlloc->rarPdschCfg, sizeof(PdschCfg));
         if(rarSlotAlloc->rarPdschCfg)
         {
            memcpy(rarSlotAlloc->rarPdschCfg, &dciSlotAlloc->rarPdcchCfg->dci[0].pdschCfg,sizeof(PdschCfg));
         }
         else
         {
            DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for rarSlotAlloc->rarPdschCfg");
            SCH_FREE(dciSlotAlloc->rarPdcchCfg, sizeof(PdcchCfg));
            if(!dciSlotAlloc->rarPdschCfg)
            {
               SCH_FREE(dciSlotAlloc, sizeof(RarAlloc));
               cell->schDlSlotInfo[dciSlot]->rarAlloc[ueId-1] = NULLP;
            }
            SCH_FREE(rarSlotAlloc, sizeof(RarAlloc));
            cell->schDlSlotInfo[rarSlot]->rarAlloc[ueId-1] = NULLP;
            return false;
         }
      }

      cell->schDlSlotInfo[dciSlot]->pdcchUe = ueId;
      if(cell->raReq[ueId-1]->isCFRA)
         cell->schUlSlotInfo[pucchTime.slot]->pucchUe = ueId;
      else
         cell->schUlSlotInfo[msg3Time.slot]->puschUe = ueId;

      /* Create raCb at SCH */
      createSchRaCb(ueId, cell->raReq[ueId-1], schInst);

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
 *     Function : SchProcRachInd
 *     
 *     This function process rach indication
 *     
 *  @param[in]  rachInd parameters
 *  @param[in]  shed instance
 *  @return  ROK
 **/
uint8_t SchProcRachInd(Pst *pst, RachIndInfo *rachInd)
{
   SchRaReq  *raReq = NULLP;
   float     slotDuration;
   uint8_t   winNumSlots;
   uint8_t   ueId;
   Inst      schInst = pst->dstInst-SCH_INST_START;
   SchCellCb *cell = schCb[schInst].cells[schInst];

   DU_LOG("\nINFO  -->  SCH : Received Rach indication");

   if(cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH: Failed to find cell in SchProcRachInd");
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
      DU_LOG("\nERROR  -->  SCH : Memory allocation failure in SchProcRachInd");
      SCH_FREE(rachInd, sizeof(RachIndInfo));
      return RFAILED;
   }

   /* calculate the ra-rnti value */
   raReq->raRnti = calculateRaRnti(rachInd->symbolIdx, rachInd->slotIdx, rachInd->freqIdx);
   raReq->rachInd = rachInd;
   if((cell->ueCb[ueId-1].crnti == rachInd->crnti) && (cell->ueCb[ueId-1].state == SCH_UE_HANDIN_IN_PROGRESS))
   {
      raReq->isCFRA = true;
      raReq->ueCb = &cell->ueCb[ueId-1];
   }
   else
      raReq->isCFRA = false;
   raReq->winStartTime.sfn = rachInd->timingInfo.sfn;
   raReq->winStartTime.slot = rachInd->timingInfo.slot;
  
   /* Converting window size from ms to number of slots */
   slotDuration = (1 / pow(2, cell->numerology));
   winNumSlots = (float)cell->cellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.prachCfgGeneric.raRspWindow / slotDuration;
   
   /* Adding window size to window start time to get window end time */
   ADD_DELTA_TO_TIME(raReq->winStartTime, raReq->winEndTime, winNumSlots, cell->numSlots);
   cell->raReq[ueId -1] = raReq;

   /* Adding UE Id to list of pending UEs to be scheduled */
   cell->api->SchRachInd(cell, ueId);
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
   PdschCfg *pdsch;

   SchBwpDlCfg *initialBwp = &cell->cellCfg.dlCfgCommon.schInitialDlBwp;
   SCH_ALLOC(rarAlloc->rarPdcchCfg, sizeof(PdcchCfg));
   if(rarAlloc->rarPdcchCfg == NULLP)
   {
      DU_LOG("\nERROR  --> SCH : Memory allocation failed in %s",__func__);
      return RFAILED;
   }
   PdcchCfg *pdcch = rarAlloc->rarPdcchCfg;
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
      cell->cellCfg.dlCfgCommon.schInitialDlBwp.pdcchCommon.commonSearchSpace.freqDomainRsrc, FREQ_DOM_RSRC_SIZE);

   pdcch->coresetCfg.cceRegMappingType = 1; /* coreset0 is always interleaved */
   pdcch->coresetCfg.regBundleSize = 6;    /* spec-38.211 sec 7.3.2.2 */
   pdcch->coresetCfg.interleaverSize = 2;  /* spec-38.211 sec 7.3.2.2 */
   pdcch->coresetCfg.coreSetType = 0;
   pdcch->coresetCfg.coreSetSize = numRbs;
   pdcch->coresetCfg.shiftIndex = cell->cellCfg.phyCellId;
   pdcch->coresetCfg.precoderGranularity = 0; /* sameAsRegBundle */
   pdcch->numDlDci = 1;
   pdcch->dci[0].rnti = cell->raReq[ueId-1]->raRnti; /* RA-RNTI */
   pdcch->dci[0].scramblingId = cell->cellCfg.phyCellId;
   pdcch->dci[0].scramblingRnti = 0;
   pdcch->dci[0].cceIndex = 4; /* considering SIB1 is sent at cce 0-1-2-3 */
   pdcch->dci[0].aggregLevel = 4;
   pdcch->dci[0].beamPdcchInfo.numPrgs = 1;
   pdcch->dci[0].beamPdcchInfo.prgSize = 1;
   pdcch->dci[0].beamPdcchInfo.digBfInterfaces = 0;
   pdcch->dci[0].beamPdcchInfo.prg[0].pmIdx = 0;
   pdcch->dci[0].beamPdcchInfo.prg[0].beamIdx[0] = 0;
   pdcch->dci[0].txPdcchPower.beta_pdcch_1_0 = 0;
   pdcch->dci[0].txPdcchPower.powerControlOffsetSS = 0;

   pdsch = &pdcch->dci[0].pdschCfg;
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
   pdsch->dmrs.dlDmrsSymbPos = DL_DMRS_SYMBOL_POS; 
   pdsch->dmrs.dmrsConfigType = 0; /* type-1 */
   pdsch->dmrs.dlDmrsScramblingId = cell->cellCfg.phyCellId;
   pdsch->dmrs.scid = 0;
   pdsch->dmrs.numDmrsCdmGrpsNoData = 1;
   pdsch->dmrs.dmrsPorts = 0;
   pdsch->dmrs.mappingType      = DMRS_MAP_TYPE_A;  /* Type-A */
   pdsch->dmrs.nrOfDmrsSymbols  = NUM_DMRS_SYMBOLS;
   pdsch->dmrs.dmrsAddPos       = DMRS_ADDITIONAL_POS;

   pdsch->pdschTimeAlloc.rowIndex = k0Index;
   pdsch->pdschTimeAlloc.startSymb = initialBwp->pdschCommon.timeDomRsrcAllocList[k0Index].startSymbol;
   pdsch->pdschTimeAlloc.numSymb = initialBwp->pdschCommon.timeDomRsrcAllocList[k0Index].lengthSymbol;

   pdsch->pdschFreqAlloc.vrbPrbMapping = 0; /* non-interleaved */
   pdsch->pdschFreqAlloc.resourceAllocType = 1; /* RAT type-1 RIV format */
   pdsch->pdschFreqAlloc.startPrb = MAX_NUM_RB;
   pdsch->pdschFreqAlloc.numPrb = \
      schCalcNumPrb(tbSize, mcs, initialBwp->pdschCommon.timeDomRsrcAllocList[k0Index].lengthSymbol);

   /* Find total symbols occupied including DMRS */
   dmrsStartSymbol = findDmrsStartSymbol(pdsch->dmrs.dlDmrsSymbPos);
   /* If there are no DRMS symbols, findDmrsStartSymbol() returns MAX_SYMB_PER_SLOT, 
    * in that case only PDSCH symbols are marked as occupied */
   if(dmrsStartSymbol == MAX_SYMB_PER_SLOT)
   {
      startSymbol = pdsch->pdschTimeAlloc.startSymb;
      numSymbol = pdsch->pdschTimeAlloc.numSymb;
   }
   /* If DMRS symbol is found, mark DMRS and PDSCH symbols as occupied */
   else
   {
      startSymbol = dmrsStartSymbol;
      numSymbol = pdsch->dmrs.nrOfDmrsSymbols + pdsch->pdschTimeAlloc.numSymb;
   }

   /* Allocate the number of PRBs required for RAR PDSCH */
   if((allocatePrbDl(cell, rarTime, startSymbol, numSymbol,\
      &pdsch->pdschFreqAlloc.startPrb, pdsch->pdschFreqAlloc.numPrb)) != ROK)
   {
      DU_LOG("\nERROR  --> SCH : allocatePrbDl() failed for RAR");
      SCH_FREE(rarAlloc->rarPdcchCfg, sizeof(PdcchCfg));
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

 /* @brief Process RACH resource release after CFRA
 *
 * @details
 *
 *     Function : SchProcRachRsrcRel
 *     
 *     This function processes RACH resorce release
 *     from MAC after CFRA. It releases the dedicated 
 *     preamble alloted to the UE
 *     
 *  @param[in]  Post structure
 *  @param[in]  RACH resource release
 *  @return     ROK
 *              RFAILED
 */
uint8_t SchProcRachRsrcRel(Pst *pst, SchRachRsrcRel *schRachRsrcRel)
{
   uint8_t      ret = ROK;
   uint8_t      ssbIdx = 0, cfraSsbIdx = 0;
   uint16_t     cellIdx = 0;
   Inst         inst = pst->dstInst - SCH_INST_START;
   SchCellCb    *cellCb = NULLP;
   SchUeCb      *ueCb = NULLP;

   DU_LOG("\nINFO  -->  SCH : Received RACH resource release for Cell ID [%d] CRNTI [%d]", \
         schRachRsrcRel->cellId, schRachRsrcRel->crnti);

   /* Fetch Cell CB */
   for(cellIdx = 0; cellIdx < MAX_NUM_CELL; cellIdx++)
   {
      if((schCb[inst].cells[cellIdx]) && (schCb[inst].cells[cellIdx]->cellId == schRachRsrcRel->cellId))
      {
         cellCb = schCb[inst].cells[cellIdx];
         break;
      }
   }
   
   if(cellCb)
   {
      /* Fetch UE CB */
      ueCb = schGetUeCb(cellCb, schRachRsrcRel->crnti);
      if(ueCb->crnti != schRachRsrcRel->crnti)
      {
         DU_LOG("\nERROR  -->  SCH : CRNTI [%d] not found", schRachRsrcRel->crnti);
         ret = RFAILED;
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  SCH : Cell ID [%d] not found", schRachRsrcRel->cellId);
      ret = RFAILED;
   }

   /* Free SSB resource if no failure has occurred until this step */
   if(ret == ROK)
   {
      for(ssbIdx = 0; ssbIdx < schRachRsrcRel->cfraResource.numSsb; ssbIdx++)
      {
         /* Search each ssbIdx entry in UE Cb */
         for(cfraSsbIdx = 0; cfraSsbIdx < ueCb->cfraResource.numSsb; cfraSsbIdx++)
         {
            if(ueCb->cfraResource.ssbResource[cfraSsbIdx].ssbIdx == schRachRsrcRel->cfraResource.ssbResource[ssbIdx].ssbIdx)
            {
               /* If ssbIdx entry is found in UE CB, free dedicated resources
                * for this ssbIdx */
               UNSET_ONE_BIT(ueCb->cfraResource.ssbResource[cfraSsbIdx].raPreambleIdx, cellCb->dedPreambleBitMap);
               memset(&ueCb->cfraResource.ssbResource[cfraSsbIdx], 0, sizeof(SchCfraSsbResource));
               ueCb->cfraResource.numSsb--;
               break;
            }
         }
      } /* End of for */
   } /* End of if */

   /* Free RACH resource release memory allocated by MAC */
   SCH_FREE(schRachRsrcRel, sizeof(SchRachRsrcRel));
   return ret;
}
 /* @brief process MSG4 completion
 *
 * @details
 *
 *     Function : schMsg4Complete
 *     
 *     This function updates ra state and msg4 Hqrq 
 *     proc upon MSG4 completion     
 *  @param[in]  SchUeCb *ueCb, UE cb pointer
 *  @return     VOID
 */
void schMsg4Complete(SchUeCb *ueCb)
{
   DU_LOG("\nINFO --> SCH: State change for ueId[%2d] to SCH_RA_STATE_MSG4_DONE\n",ueCb->ueId);
   ueCb->cellCb->raCb[ueCb->ueId-1].raState = SCH_RA_STATE_MSG4_DONE;
   ueCb->msg4HqProc = ueCb->retxMsg4HqProc = NULLP;
}
/**********************************************************************
         End of file
**********************************************************************/
