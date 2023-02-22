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

Desc:     C source code for Entry point fucntions

File:     sch_common.c

 **********************************************************************/

/** @file sch_common.c
  @brief This module performs common scheduling
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
 * @brief common resource allocation for SSB
 *
 * @details
 *
 *     Function : schBroadcastSsbAlloc
 *     
 *     This function handles common scheduling for SSB
 *     
 *  @param[in]  SchCellCb *cell, cell cb
 *  @param[in]  DlBrdcstAlloc *dlBrdcstAlloc, DL brdcst allocation
 *  @return  void
 **/
uint8_t schBroadcastSsbAlloc(SchCellCb *cell, SlotTimingInfo slotTime, DlBrdcstAlloc *dlBrdcstAlloc)
{
   /* schedule SSB */
   uint8_t ssbStartSymb, idx;
   uint16_t ssbStartPrb;
   SchDlSlotInfo *schDlSlotInfo;
   SsbInfo ssbInfo;

   if(cell == NULL)
   {
      DU_LOG("\nERROR  -->  SCH: schBroadcastSsbAlloc() : Cell is NULL");
      return RFAILED;
   }

   if(dlBrdcstAlloc == NULL)
   {
      DU_LOG("\nERROR  -->  SCH: schBroadcastSsbAlloc() : dlBrdcstAlloc is NULL");
      return RFAILED;
   }

   schDlSlotInfo = cell->schDlSlotInfo[slotTime.slot];
   ssbStartPrb = cell->cellCfg.ssbSubcOffset; //+Kssb
   ssbStartSymb = cell->ssbStartSymbArr[dlBrdcstAlloc->ssbIdxSupported-1]; /*since we are supporting only 1 ssb beam */

   /* Assign interface structure */
   for(idx=0; idx<dlBrdcstAlloc->ssbIdxSupported; idx++)
   {
      ssbInfo.ssbIdx              = idx;
      ssbInfo.fdAlloc.startPrb    = ssbStartPrb;
      ssbInfo.fdAlloc.numPrb      = SCH_SSB_NUM_PRB;
      ssbInfo.tdAlloc.startSymb   = ssbStartSymb;
      ssbInfo.tdAlloc.numSymb     = SCH_SSB_NUM_SYMB;
      dlBrdcstAlloc->ssbInfo[idx] = ssbInfo;
      schDlSlotInfo->ssbInfo[idx] = ssbInfo;
   }

   if((allocatePrbDl(cell, slotTime, ssbStartSymb, SCH_SSB_NUM_SYMB, &ssbInfo.fdAlloc.startPrb, ssbInfo.fdAlloc.numPrb)) != ROK)
   {
       DU_LOG("\nERROR  -->  SCH: PRB allocation failed for SSB in SFN:SLOT [%d : %d]", slotTime.sfn, slotTime.slot);
       return RFAILED;
   }


   schDlSlotInfo->ssbPres = true;
   schDlSlotInfo->ssbIdxSupported = dlBrdcstAlloc->ssbIdxSupported;
   return ROK;
}

/**
 * @brief common resource allocation for SIB1
 *
 * @details
 *
 *     Function : schBroadcastSib1Alloc
 *     
 *     This function handles common scheduling for SIB1
 *     
 *  @param[in]  SchCellCb *cell, cell cb
 *  @param[in]  DlBrdcstAlloc *dlBrdcstAlloc, DL brdcst allocation
 *  @return  void
 **/
uint8_t schBroadcastSib1Alloc(SchCellCb *cell, SlotTimingInfo slotTime, DlBrdcstAlloc *dlBrdcstAlloc)
{
   uint8_t dmrsStartSymbol, startSymbol, numSymbol ;
   DmrsInfo dmrs;
   PdschFreqAlloc freqAlloc;
   PdschTimeAlloc timeAlloc;
   SchDlSlotInfo *schDlSlotInfo = NULLP;

   if(cell == NULL)
   {
      DU_LOG("\nERROR  -->  SCH: schBroadcastSsbAlloc() : Cell is NULL");
      return RFAILED;
   }

   if(dlBrdcstAlloc == NULL)
   {
      DU_LOG("\nERROR  -->  SCH: schBroadcastSsbAlloc() : dlBrdcstAlloc is NULL");
      return RFAILED;
   }
   
   dlBrdcstAlloc->crnti = SI_RNTI;
   dmrs = cell->sib1SchCfg.sib1PdcchCfg.dci.pdschCfg->dmrs;
   freqAlloc = cell->sib1SchCfg.sib1PdcchCfg.dci.pdschCfg->pdschFreqAlloc;
   timeAlloc = cell->sib1SchCfg.sib1PdcchCfg.dci.pdschCfg->pdschTimeAlloc;
   schDlSlotInfo = cell->schDlSlotInfo[slotTime.slot];

   /* Find total symbols used including DMRS */
   /* If there are no DRMS symbols, findDmrsStartSymbol() returns MAX_SYMB_PER_SLOT,
    * in that case only PDSCH symbols are marked as occupied */
   dmrsStartSymbol = findDmrsStartSymbol(dmrs.dlDmrsSymbPos);   
   if(dmrsStartSymbol == MAX_SYMB_PER_SLOT)
   {
      startSymbol = timeAlloc.startSymb;
      numSymbol = timeAlloc.numSymb;
   }
   /* If DMRS symbol is found, mark DMRS and PDSCH symbols as occupied */
   else
   {
      startSymbol = dmrsStartSymbol;
      numSymbol = dmrs.nrOfDmrsSymbols + timeAlloc.numSymb;
   }

   /* Allocate PRB */
   if((allocatePrbDl(cell, slotTime, startSymbol, numSymbol, &freqAlloc.startPrb, freqAlloc.numPrb)) != ROK)
   {
       DU_LOG("\nERROR  -->  SCH: PRB allocation failed for SIB1 in SFN:Slot [%d : %d]", slotTime.sfn, slotTime.slot);
       return RFAILED;
   }

   memcpy(&dlBrdcstAlloc->sib1Alloc.bwp, &cell->sib1SchCfg.bwp, sizeof(BwpCfg)); 
   memcpy(&dlBrdcstAlloc->sib1Alloc.sib1PdcchCfg, &cell->sib1SchCfg.sib1PdcchCfg, sizeof(PdcchCfg)); 
   schDlSlotInfo->sib1Pres = true;
   return ROK;
}

/*******************************************************************
 *
 * @brief Handles sending UL scheduler info to MAC 
 *
 * @details
 *
 *    Function : sendUlSchInfoToMac
 *
 *    Functionality:
 *     Sends UL Sch info to MAC from SCH
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int sendUlSchInfoToMac(UlSchedInfo *ulSchedInfo, Inst inst)
{
   Pst pst;

   memset(&pst, 0, sizeof(Pst));
   FILL_PST_SCH_TO_MAC(pst, inst);
   pst.event = EVENT_UL_SCH_INFO;

   return(MacMessageRouter(&pst, (void *)ulSchedInfo));
}

/**
 * @brief Function to fill Pucch Format 0
 *
 * @details
 *
 *     Function : fillPucchFormat0
 *     
 *     Function to fill Pucch format 0
 *     
 *  @param[in]  SchPucchInfo pointer, SchPucchResrcInfo pointer
 *  @return  void
 **/

void fillPucchFormat0(SchPucchInfo *ulSchedPucch, SchPucchResrcInfo *resrcInfo)
{
   if(resrcInfo->SchPucchFormat.format0)
   {
      ulSchedPucch->fdAlloc.resAlloc.type1.numPrb = PUCCH_NUM_PRB_FORMAT_0_1_4;
      ulSchedPucch->pucchFormat  = PUCCH_FORMAT_0;
      ulSchedPucch->initialCyclicShift =  resrcInfo->SchPucchFormat.format0->initialCyclicShift;
      ulSchedPucch->tdAlloc.numSymb = resrcInfo->SchPucchFormat.format0->numSymbols;
      ulSchedPucch->tdAlloc.startSymb = resrcInfo->SchPucchFormat.format0->startSymbolIdx;
   }
}

/**
 * @brief Function to fill Pucch Format 1
 *
 * @details
 *
 *     Function : fillPucchFormat1
 *     
 *     Function to fill Pucch format 1
 *     
 *  @param[in]  SchPucchInfo pointer, SchPucchResrcInfo pointer
 *  @return  void
 **/

void fillPucchFormat1(SchPucchInfo *ulSchedPucch, SchPucchResrcInfo *resrcInfo)
{
   if(resrcInfo->SchPucchFormat.format1)
   {
      ulSchedPucch->fdAlloc.resAlloc.type1.numPrb = PUCCH_NUM_PRB_FORMAT_0_1_4;
      ulSchedPucch->pucchFormat  = PUCCH_FORMAT_1;
      ulSchedPucch->initialCyclicShift =  resrcInfo->SchPucchFormat.format1->initialCyclicShift;
      ulSchedPucch->tdAlloc.numSymb = resrcInfo->SchPucchFormat.format1->numSymbols;
      ulSchedPucch->tdAlloc.startSymb = resrcInfo->SchPucchFormat.format1->startSymbolIdx;
      ulSchedPucch->timeDomOCC =  resrcInfo->SchPucchFormat.format1->timeDomOCC;
  }
}

/**
 * @brief Function to fill Pucch format for UL Sched Info
 *
 * @details
 *
 *     Function : fillUlSchedPucchFormat
 *     
 *     Function to fill Pucch format for UL Sched Info
 *     
 *  @param[in]  pucchFormat , SchPucchInfo pointer,
 *  @param[in]  SchPucchFormatCfg pointer, SchPucchResrcInfo pointer
 *  @return  void
 **/

uint8_t fillUlSchedPucchFormat(uint8_t pucchFormat, SchPucchInfo *ulSchedPucch,\
   SchPucchResrcInfo *resrcInfo, SchPucchFormatCfg *formatCfg)
{
   uint8_t ret = ROK;

   switch(pucchFormat)
   {
      case PUCCH_FORMAT_0:
         {
            if(resrcInfo)
               fillPucchFormat0(ulSchedPucch, resrcInfo);
            return ret;
         }
      case PUCCH_FORMAT_1:
         {
            if(resrcInfo)
            {
               fillPucchFormat1(ulSchedPucch, resrcInfo);
            }
            if(formatCfg)
            {
               ulSchedPucch->addDmrs = formatCfg->addDmrs;
               ulSchedPucch->pi2BPSK = formatCfg->pi2BPSK;
            }
            return ret;
         }/* To Add support for more Pucch Format */

      default:
         DU_LOG("\nERROR  --> SCH : Invalid PUCCH format[%d] in fillUlSchedPucchFormatCfg()", pucchFormat);
	 ret = RFAILED;
         return ret;
   }
   return ret;
}

/**
 * @brief Function to fill Pucch Dedicated Cfg for UL Sched Info
 *
 * @details
 *
 *     Function : fillUlSchedPucchDedicatedCfg
 *     
 *     Function to fill Pucch Dedicated Cfg for UL Sched Info
 *     
 *  @param[in]  pucchFormat to be filled
 *  @param[in]  SchPucchFormatCfg pointer, SchPucchCfg pointer
 *  @return  void
 **/

uint8_t fillUlSchedPucchDedicatedCfg(SchCellCb *cell, SchPucchCfg *pucchDedCfg,\
   SlotTimingInfo *slotInfo, SchPucchInfo *ulSchedPucch)
{
   uint8_t ret, resrcSetIdx, resrcIdx, schedReqIdx, srPeriodicity = 0;
   uint16_t srOffset = 0;
   uint16_t numSlots = cell->numSlots;
   bool isAllocated = false;
   uint16_t pucchStartPrb;
   ret = ROK;
   if(pucchDedCfg->resrcSet && pucchDedCfg->resrc)
   {
      //Assuming one entry in the list
      for(resrcSetIdx = 0; resrcSetIdx < pucchDedCfg->resrcSet->resrcSetToAddModListCount; resrcSetIdx++)
      {
         for(resrcIdx = 0; resrcIdx < pucchDedCfg->resrc->resrcToAddModListCount; resrcIdx++)
         {
            if(pucchDedCfg->resrcSet->resrcSetToAddModList[resrcSetIdx].resrcList[resrcSetIdx] ==\
                  pucchDedCfg->resrc->resrcToAddModList[resrcIdx].resrcId)
            {
               ulSchedPucch->intraFreqHop = pucchDedCfg->resrc->resrcToAddModList[resrcIdx].intraFreqHop;
               ulSchedPucch->secondPrbHop = pucchDedCfg->resrc->resrcToAddModList[resrcIdx].secondPrbHop;
               ulSchedPucch->fdAlloc.resAlloc.type1.startPrb = pucchDedCfg->resrc->resrcToAddModList[resrcIdx].startPrb;
               ulSchedPucch->pucchFormat = pucchDedCfg->resrc->resrcToAddModList[resrcIdx].pucchFormat;
               ret = fillUlSchedPucchFormat(ulSchedPucch->pucchFormat, ulSchedPucch,\
                     &pucchDedCfg->resrc->resrcToAddModList[resrcIdx], NULLP);
               if(ret == RFAILED)
                  return ret;

               pucchStartPrb = pucchDedCfg->resrc->resrcToAddModList[resrcIdx].startPrb;
               ret = allocatePrbUl(cell, *slotInfo, ulSchedPucch->tdAlloc.startSymb, ulSchedPucch->tdAlloc.numSymb, &pucchStartPrb, PUCCH_NUM_PRB_FORMAT_0_1_4);
               if(ret == ROK)
               {
                  isAllocated = true;
                  break;
               }
            }
         }
         if(isAllocated)
         break;
      }
   }

   if(pucchDedCfg->format1)
   {
      ret = fillUlSchedPucchFormat(ulSchedPucch->pucchFormat, ulSchedPucch, NULLP, pucchDedCfg->format1);
      if(ret == RFAILED)
         return ret;
   }

   if(!isAllocated)
   {
      return RFAILED;
   }

   /* setting SR and UCI flag */
   if(pucchDedCfg->schedReq)
   {
      for(schedReqIdx = 0; schedReqIdx < pucchDedCfg->schedReq->schedAddModListCount; schedReqIdx++)
      {
         srPeriodicity = pucchDedCfg->schedReq->schedAddModList[schedReqIdx].periodicity;
         srOffset      = pucchDedCfg->schedReq->schedAddModList[schedReqIdx].offset;
         break;
      }
      if(((numSlots * slotInfo->sfn + slotInfo->slot - srOffset) % srPeriodicity) == 0)
      {
         ulSchedPucch->srFlag  = true;
      }
   }
   return ret;
}

/**
 * @brief Function to fill Pucch Resource Info
 *
 * @details
 *
 *     Function : fillPucchResourceInfo
 *     
 *     Function to fill Pucch Resource Info
 *     
 *  @param[in]  SchPucchInfo *schPucchInfo, Inst inst
 *  @return  ROK/RFAILED
 **/

uint16_t fillPucchResourceInfo(uint8_t ueId, SchPucchInfo *schPucchInfo, Inst inst, SlotTimingInfo slotInfo)
{
   uint8_t ret = ROK,  ueIdx = 0, pucchIdx = 0;
   SchCellCb  *cell = schCb[inst].cells[inst];
   SchPucchCfgCmn *pucchCfg = NULLP;
   SchBwpParams *ulBwp = NULLP;
#ifdef NR_DRX 
   SchUeCb *ueCb = NULLP;
#endif
   uint16_t startPrb;

   ueIdx = ueId -1;
#ifdef NR_DRX 
   ueCb = &(cell->ueCb[ueIdx]); 
   if(ueCb->ueDrxInfoPres)
   {
      if(!ueCb->drxUeCb.drxUlUeActiveStatus)
         return RFAILED;
   }
#endif
   if(cell->ueCb[ueIdx].ueCfg.spCellCfg.servCellRecfg.initUlBwp.pucchCfgPres)
   {
      /* fill pucch dedicated cfg */
      ret = fillUlSchedPucchDedicatedCfg(cell,\
       &cell->ueCb[ueIdx].ueCfg.spCellCfg.servCellRecfg.initUlBwp.pucchCfg, &slotInfo, schPucchInfo);
      if(ret == RFAILED)
      {
         memset(schPucchInfo, 0, sizeof(SchPucchInfo));
         DU_LOG("\nERROR  --> SCH : Filling PUCCH dedicated cfg failed at fillPucchResourceInfo()");
	 return ret;
      }
   }
   else
   {
      /* fill pucch common cfg */
      /* derive pucchResourceSet from schCellCfg */
      pucchCfg = &cell->cellCfg.ulCfgCommon.schInitialUlBwp.pucchCommon;
      pucchIdx = pucchCfg->pucchResourceCommon;
      ulBwp = &cell->cellCfg.ulCfgCommon.schInitialUlBwp.bwp;
      startPrb = ulBwp->freqAlloc.startPrb + pucchResourceSet[pucchIdx][3];
      ret = allocatePrbUl(cell, slotInfo, pucchResourceSet[pucchIdx][1], pucchResourceSet[pucchIdx][2],\
            &startPrb, PUCCH_NUM_PRB_FORMAT_0_1_4);
      if (ret == ROK)
      {
         schPucchInfo->fdAlloc.resAlloc.type1.startPrb = ulBwp->freqAlloc.startPrb + pucchResourceSet[pucchIdx][3];
         schPucchInfo->fdAlloc.resAlloc.type1.numPrb = PUCCH_NUM_PRB_FORMAT_0_1_4;
         schPucchInfo->tdAlloc.startSymb = pucchResourceSet[pucchIdx][1];
         schPucchInfo->tdAlloc.numSymb = pucchResourceSet[pucchIdx][2];
         schPucchInfo->pucchFormat = pucchResourceSet[pucchIdx][0];

         /* set SR and UCI flag to false */
         schPucchInfo->srFlag  = true;
      }
   }
   return ROK;
}

/**
 * @brief resource allocation for UL
 *
 * @details
 *
 *     Function : schUlResAlloc
 *     
 *     This function handles UL Resource allocation
 *     
 *  @param[in]  SchCellCb *cell, cellCb
 *  @return  void
 **/
uint8_t schUlResAlloc(SchCellCb *cell, Inst schInst)
{
   int ret = ROK;
#ifdef NR_DRX 
   SchUeCb   *ueCb;
#endif
   UlSchedInfo ulSchedInfo;
   SchUlSlotInfo  *schUlSlotInfo = NULLP;
   SlotTimingInfo ulTimingInfo;
   memset(&ulSchedInfo, 0, sizeof(UlSchedInfo));

   /* add PHY delta */
   ADD_DELTA_TO_TIME(cell->slotInfo,ulTimingInfo,PHY_DELTA_UL+SCHED_DELTA, cell->numSlots);

   ulSchedInfo.cellId = cell->cellId;
   ulSchedInfo.slotIndInfo.cellId = ulSchedInfo.cellId;
   ulSchedInfo.slotIndInfo.sfn = ulTimingInfo.sfn;
   ulSchedInfo.slotIndInfo.slot = ulTimingInfo.slot;

   /* Schedule resources for PRACH */
   if(cell->firstSib1Transmitted)
    schPrachResAlloc(cell, &ulSchedInfo, ulTimingInfo);

   schUlSlotInfo = cell->schUlSlotInfo[ulTimingInfo.slot]; 
   if(schUlSlotInfo->schPuschInfo)
   {
      GET_CRNTI(ulSchedInfo.crnti, schUlSlotInfo->puschUe);
      /* Check the ue drx status if the UE is active for uplink scheduling or not  */
#ifdef NR_DRX 
      ueCb = schGetUeCb(cell, ulSchedInfo.crnti);
      if(ueCb->ueDrxInfoPres)
      {
         if(!ueCb->drxUeCb.drxUlUeActiveStatus)
            return RFAILED;
      }
#endif
      ulSchedInfo.dataType |= SCH_DATATYPE_PUSCH;
      memcpy(&ulSchedInfo.schPuschInfo, schUlSlotInfo->schPuschInfo,
	    sizeof(SchPuschInfo));
      SCH_FREE(schUlSlotInfo->schPuschInfo, sizeof(SchPuschInfo));
      schUlSlotInfo->schPuschInfo = NULL;
   }

   if(schUlSlotInfo->pucchPres)
   {
      GET_CRNTI(ulSchedInfo.crnti, schUlSlotInfo->pucchUe);
      ret = fillPucchResourceInfo(schUlSlotInfo->pucchUe, &schUlSlotInfo->schPucchInfo, schInst, ulTimingInfo);
      if (ret == ROK)
      {
         ulSchedInfo.dataType |= SCH_DATATYPE_UCI;
         memcpy(&ulSchedInfo.schPucchInfo, &schUlSlotInfo->schPucchInfo,
               sizeof(SchPucchInfo));
      }
      else
      {
         return RFAILED;
      }
      memset(&schUlSlotInfo->schPucchInfo, 0, sizeof(SchPucchInfo));
   }

   //send msg to MAC
   ret = sendUlSchInfoToMac(&ulSchedInfo, schInst);
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : Sending UL Sch info from SCH to MAC failed");
   }

   schInitUlSlot(schUlSlotInfo);
   return ret;
}

/*******************************************************************
 *
 * @brief Fills pdcch and pdsch info for msg4
 *
 * @details
 *
 *    Function : schDlRsrcAllocMsg4
 *
 *    Functionality:
 *       Fills pdcch and pdsch info for msg4
 *
 * @params[in] SchCellCb *cell, SlotTimingInfo msg4Time
 * @params[in] uint8_t ueId, DlMsgAlloc *dlMsgAlloc
 * @params[in] uint8_t pdschStartSymbol, uint8_t pdschNumSymbols
 * @params[in] bool isRetx, SchDlHqProcCb *hqP
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t schDlRsrcAllocMsg4(SchCellCb *cell, SlotTimingInfo msg4Time, uint8_t ueId, DlMsgAlloc *dlMsgAlloc,\
                           uint8_t pdschStartSymbol, uint8_t pdschNumSymbols, bool isRetx, SchDlHqProcCb *hqP)
{
   uint8_t coreset0Idx = 0;
   uint8_t firstSymbol = 0;
   uint8_t numSymbols = 0;
   uint8_t mcs = DEFAULT_MCS;                         /* MCS fixed to 4 */
   uint8_t dmrsStartSymbol = 0, startSymbol = 0, numSymbol = 0;
   uint16_t tbSize = 0;
   uint16_t numRbs;
   SchBwpDlCfg *initialBwp = NULLP;
   PdcchCfg *pdcch = NULLP;
   PdschCfg *pdsch = NULLP;
   BwpCfg *bwp = NULLP;
   DlMsgSchInfo *msg4Alloc = NULLP;

   if(cell == NULL)
   {
      DU_LOG("\nERROR  -->  SCH: schDlRsrcAllocMsg4() : Cell is NULL");
      return RFAILED;
   }

   if(dlMsgAlloc == NULL)
   {
      DU_LOG("\nERROR  -->  SCH: schDlRsrcAllocMsg4() :  dlMsgAlloc is NULL");
      return RFAILED;
   }

   msg4Alloc = &dlMsgAlloc->dlMsgSchedInfo[dlMsgAlloc->numSchedInfo];
   initialBwp   = &cell->cellCfg.dlCfgCommon.schInitialDlBwp;
   pdcch = &msg4Alloc->dlMsgPdcchCfg;
   pdsch = &msg4Alloc->dlMsgPdschCfg;
   bwp = &msg4Alloc->bwp;
   coreset0Idx  = initialBwp->pdcchCommon.commonSearchSpace.coresetId;

   fillDlMsgInfo(&msg4Alloc->dlMsgInfo, cell->raCb[ueId-1].tcrnti, isRetx, hqP);
   msg4Alloc->dlMsgInfo.dlMsgPduLen = cell->raCb[ueId-1].dlMsgPduLen;

   /* derive the sib1 coreset0 params from table 13-1 spec 38.213 */
   numRbs     = coresetIdxTable[coreset0Idx][1];
   numSymbols = coresetIdxTable[coreset0Idx][2];

   /* calculate time domain parameters */
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
   pdcch->dci.rnti = cell->raCb[ueId-1].tcrnti;
   pdcch->dci.scramblingId = cell->cellCfg.phyCellId;
   pdcch->dci.scramblingRnti = 0;
   pdcch->dci.cceIndex = 4; /* considering SIB1 is sent at cce 0-1-2-3 */
   pdcch->dci.aggregLevel = 4;
   pdcch->dci.beamPdcchInfo.numPrgs = 1;
   pdcch->dci.beamPdcchInfo.prgSize = 1;
   pdcch->dci.beamPdcchInfo.digBfInterfaces = 0;
   pdcch->dci.beamPdcchInfo.prg[0].pmIdx = 0;
   pdcch->dci.beamPdcchInfo.prg[0].beamIdx[0] = 0;
   pdcch->dci.txPdcchPower.beta_pdcch_1_0 = 0;
   pdcch->dci.txPdcchPower.powerControlOffsetSS = 0;
   pdcch->dci.pdschCfg = pdsch;

   /* fill the PDSCH PDU */
   uint8_t cwCount = 0;
   pdsch->pduBitmap = 0; /* PTRS and CBG params are excluded */
   pdsch->rnti = cell->raCb[ueId-1].tcrnti;
   pdsch->pduIndex = 0;
   pdsch->numCodewords = 1;
   for(cwCount = 0; cwCount < pdsch->numCodewords; cwCount++)
   {
      pdsch->codeword[cwCount].targetCodeRate = 308;
      pdsch->codeword[cwCount].qamModOrder = 2;
      pdsch->codeword[cwCount].mcsIndex = mcs; /* mcs configured to 4 */
      pdsch->codeword[cwCount].mcsTable = 0; /* notqam256 */
      if(isRetx != TRUE)
      {
         tbSize = schCalcTbSize(msg4Alloc->dlMsgInfo.dlMsgPduLen + TX_PAYLOAD_HDR_LEN); /* MSG4 size + FAPI header size*/
         hqP->tbInfo[cwCount].tbSzReq = tbSize;
         pdsch->codeword[cwCount].rvIndex = 0;
      }
      else
      {
         pdsch->codeword[cwCount].rvIndex = (pdsch->codeword[cwCount].rvIndex +1) & 0x03;
         tbSize = hqP->tbInfo[cwCount].tbSzReq;
      }
      pdsch->codeword[cwCount].tbSize = tbSize;
   }
   pdsch->dataScramblingId = cell->cellCfg.phyCellId;
   pdsch->numLayers = 1;
   pdsch->transmissionScheme = 0;
   pdsch->refPoint = 0;
   pdsch->dmrs.dlDmrsSymbPos = 4; /* Bitmap value 00000000000100 i.e. using 3rd symbol for PDSCH DMRS */
   pdsch->dmrs.dmrsConfigType = 0; /* type-1 */
   pdsch->dmrs.dlDmrsScramblingId = cell->cellCfg.phyCellId;
   pdsch->dmrs.scid = 0;
   pdsch->dmrs.numDmrsCdmGrpsNoData = 1;
   pdsch->dmrs.dmrsPorts = 0;
   pdsch->dmrs.mappingType      = DMRS_MAP_TYPE_A; /* Setting to Type-A */
   pdsch->dmrs.nrOfDmrsSymbols  = NUM_DMRS_SYMBOLS;
   pdsch->dmrs.dmrsAddPos       = DMRS_ADDITIONAL_POS;

   pdsch->pdschTimeAlloc.startSymb = pdschStartSymbol; 
   pdsch->pdschTimeAlloc.numSymb = pdschNumSymbols;

   pdsch->pdschFreqAlloc.resourceAllocType = 1; /* RAT type-1 RIV format */
   pdsch->pdschFreqAlloc.startPrb = MAX_NUM_RB;
   pdsch->pdschFreqAlloc.numPrb = schCalcNumPrb(tbSize, mcs, pdschNumSymbols);
   pdsch->pdschFreqAlloc.vrbPrbMapping = 0; /* non-interleaved */

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
   if((allocatePrbDl(cell, msg4Time, startSymbol, numSymbol,\
      &pdsch->pdschFreqAlloc.startPrb, pdsch->pdschFreqAlloc.numPrb)) != ROK)
   {
      DU_LOG("\nERROR  --> SCH : Resource allocation failed for MSG4");
      return RFAILED;
   }

   pdsch->beamPdschInfo.numPrgs = 1;
   pdsch->beamPdschInfo.prgSize = 1;
   pdsch->beamPdschInfo.digBfInterfaces = 0;
   pdsch->beamPdschInfo.prg[0].pmIdx = 0;
   pdsch->beamPdschInfo.prg[0].beamIdx[0] = 0;
   pdsch->txPdschPower.powerControlOffset = 0;
   pdsch->txPdschPower.powerControlOffsetSS = 0;

   msg4Alloc->dlMsgInfo.isMsg4Pdu = true;
   return ROK;
}
 
/*******************************************************************
 *
 * @brief Scheduling for Pucch Resource
 *
 * @details
 *
 *    Function : schAllocPucchResource
 *
 *    Functionality:
 *       Scheduling for Pucch Resource
 *
 * @params[in] SchCellCb *cell, SlotTimingInfo pucchTime, crnti
 * @params[in] SchUeCb *ueCb, bool isRetx, SchDlHqProcCb *hqP
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************/

uint16_t schAllocPucchResource(SchCellCb *cell, SlotTimingInfo pucchTime, uint16_t crnti,
                               SchUeCb *ueCb, bool isRetx, SchDlHqProcCb *hqP)
{
   uint16_t pucchSlot = 0;
   SchUlSlotInfo  *schUlSlotInfo = NULLP;

   pucchSlot = pucchTime.slot;
   schUlSlotInfo = cell->schUlSlotInfo[pucchSlot];
   memset(&schUlSlotInfo->schPucchInfo, 0, sizeof(SchPucchInfo));

   schUlSlotInfo->pucchPres = true;
   if(ueCb != NULLP)
   {
      /* set HARQ flag to true */
      schUlSlotInfo->schPucchInfo.harqInfo.harqBitLength = 1; /* 1 bit for HARQ */
      ADD_DELTA_TO_TIME(pucchTime, pucchTime, 3, cell->numSlots); /* SLOT_DELAY=3 */
      cmLListAdd2Tail(&(ueCb->hqDlmap[pucchTime.slot]->hqList), &hqP->ulSlotLnk);
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills pdcch and pdsch info for dedicated DL msg
 *
 * @details
 *
 *    Function : schDlRsrcAllocDlMsg
 *
 *    Functionality:
 *       Fills pdcch and pdsch info for dl msg
 *
 * @params[in] SchCellCb *cell, SlotTimingInfo slotTime
 * @params[in] uint16_t crnti, uint32_t tbSize
 * @params[in] DlMsgAlloc *dlMsgAlloc, uint16_t startPRB
 * @params[in] uint8_t pdschStartSymbol, uint8_t pdschNumSymbols
 * @params[in] bool isRetx, SchDlHqProcCb *hqP
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t schDlRsrcAllocDlMsg(SchCellCb *cell, SlotTimingInfo slotTime, uint16_t crnti,
                uint32_t tbSize, DlMsgAlloc *dlMsgAlloc, uint16_t startPRB, uint8_t pdschStartSymbol,
                uint8_t pdschNumSymbols, bool isRetx, SchDlHqProcCb *hqP)
{
   uint8_t ueId=0;
   PdcchCfg *pdcch = NULLP;
   PdschCfg *pdsch = NULLP;
   BwpCfg *bwp = NULLP;
   SchUeCb ueCb;
   SchControlRsrcSet coreset1;
   SchPdschConfig pdschCfg;
   uint8_t dmrsStartSymbol, startSymbol, numSymbol;

   pdcch = &dlMsgAlloc->dlMsgSchedInfo[dlMsgAlloc->numSchedInfo].dlMsgPdcchCfg;
   pdsch = &dlMsgAlloc->dlMsgSchedInfo[dlMsgAlloc->numSchedInfo].dlMsgPdschCfg;
   bwp = &dlMsgAlloc->dlMsgSchedInfo[dlMsgAlloc->numSchedInfo].bwp;

   GET_UE_ID(crnti, ueId);
   ueCb  = cell->ueCb[ueId-1];
   coreset1 = ueCb.ueCfg.spCellCfg.servCellRecfg.initDlBwp.pdcchCfg.cRSetToAddModList[0];
   pdschCfg = ueCb.ueCfg.spCellCfg.servCellRecfg.initDlBwp.pdschCfg;

   /* fill BWP */
   bwp->freqAlloc.numPrb = MAX_NUM_RB;
   bwp->freqAlloc.startPrb = 0;
   bwp->subcarrierSpacing = cell->sib1SchCfg.bwp.subcarrierSpacing;
   bwp->cyclicPrefix = cell->sib1SchCfg.bwp.cyclicPrefix;

   /* fill the PDCCH PDU */
   //Considering coreset1 also starts from same symbol as coreset0
   pdcch->coresetCfg.startSymbolIndex = coresetIdxTable[0][3];
   pdcch->coresetCfg.durationSymbols = coreset1.duration;
   memcpy(pdcch->coresetCfg.freqDomainResource, coreset1.freqDomainRsrc, FREQ_DOM_RSRC_SIZE);
   pdcch->coresetCfg.cceRegMappingType = coreset1.cceRegMappingType; /* non-interleaved */
   pdcch->coresetCfg.regBundleSize = 6;   /* must be 6 for non-interleaved */
   pdcch->coresetCfg.interleaverSize = 0; /* NA for non-interleaved */
   pdcch->coresetCfg.coreSetType = 1; /* non PBCH coreset */
   //Considering number of RBs in coreset1 is same as coreset0
   pdcch->coresetCfg.coreSetSize = coresetIdxTable[0][1];
   pdcch->coresetCfg.shiftIndex = cell->cellCfg.phyCellId;
   pdcch->coresetCfg.precoderGranularity =  coreset1.precoderGranularity;
   pdcch->numDlDci = 1;
   pdcch->dci.rnti = ueCb.crnti;
   pdcch->dci.scramblingId = cell->cellCfg.phyCellId;
   pdcch->dci.scramblingRnti = 0;
   pdcch->dci.cceIndex = 0; /* 0-3 for UL and 4-7 for DL */
   pdcch->dci.aggregLevel = 4;
   pdcch->dci.beamPdcchInfo.numPrgs = 1;
   pdcch->dci.beamPdcchInfo.prgSize = 1;
   pdcch->dci.beamPdcchInfo.digBfInterfaces = 0;
   pdcch->dci.beamPdcchInfo.prg[0].pmIdx = 0;
   pdcch->dci.beamPdcchInfo.prg[0].beamIdx[0] = 0;
   pdcch->dci.txPdcchPower.beta_pdcch_1_0 = 0;
   pdcch->dci.txPdcchPower.powerControlOffsetSS = 0;

   /* fill the PDSCH PDU */
   uint8_t cwCount = 0;
   pdsch->pduBitmap = 0; /* PTRS and CBG params are excluded */
   pdsch->rnti = ueCb.crnti;
   pdsch->pduIndex = 0;
   pdsch->numCodewords = 1;
   for(cwCount = 0; cwCount < pdsch->numCodewords; cwCount++)
   {
      pdsch->codeword[cwCount].targetCodeRate = 308;
      pdsch->codeword[cwCount].qamModOrder = ueCb.ueCfg.dlModInfo.modOrder;
      pdsch->codeword[cwCount].mcsIndex = ueCb.ueCfg.dlModInfo.mcsIndex;
      pdsch->codeword[cwCount].mcsTable = ueCb.ueCfg.dlModInfo.mcsTable;
      pdsch->codeword[cwCount].rvIndex = 0;

      if (isRetx != TRUE)
      {
         tbSize +=TX_PAYLOAD_HDR_LEN;
         hqP->tbInfo[cwCount].tbSzReq = tbSize;
      }
      pdsch->codeword[cwCount].tbSize = tbSize;
   }
   pdsch->dataScramblingId = cell->cellCfg.phyCellId;
   pdsch->numLayers = 1;
   pdsch->transmissionScheme = 0;
   pdsch->refPoint = 0;
   pdsch->dmrs.dlDmrsSymbPos = 4; /* Bitmap value 00000000000100 i.e. using 3rd symbol for PDSCH DMRS */
   pdsch->dmrs.dmrsConfigType = 0; /* type-1 */
   pdsch->dmrs.dlDmrsScramblingId = cell->cellCfg.phyCellId;
   pdsch->dmrs.scid = 0;
   pdsch->dmrs.numDmrsCdmGrpsNoData = 1;
   pdsch->dmrs.dmrsPorts = 0;
   pdsch->dmrs.mappingType      = DMRS_MAP_TYPE_A; /* Setting to Type-A */
   pdsch->dmrs.nrOfDmrsSymbols  = NUM_DMRS_SYMBOLS;
   pdsch->dmrs.dmrsAddPos       = pdschCfg.dmrsDlCfgForPdschMapTypeA.addPos;

   pdsch->pdschTimeAlloc.startSymb = pdschStartSymbol; 
   pdsch->pdschTimeAlloc.numSymb = pdschNumSymbols;

   pdsch->pdschFreqAlloc.vrbPrbMapping = 0; /* non-interleaved */
   pdsch->pdschFreqAlloc.resourceAllocType = 1; /* RAT type-1 RIV format */
   pdsch->pdschFreqAlloc.startPrb = startPRB; /*Start PRB will be already known*/
   pdsch->pdschFreqAlloc.numPrb = schCalcNumPrb(tbSize, ueCb.ueCfg.dlModInfo.mcsIndex, pdschNumSymbols);

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

   /* Allocate the number of PRBs required for DL PDSCH */
   if((allocatePrbDl(cell, slotTime, startSymbol, numSymbol,\
      &pdsch->pdschFreqAlloc.startPrb, pdsch->pdschFreqAlloc.numPrb)) != ROK)
   {
      DU_LOG("\nERROR  --> SCH : allocatePrbDl() failed for DL MSG");
      return RFAILED;
   }

   pdsch->beamPdschInfo.numPrgs = 1;
   pdsch->beamPdschInfo.prgSize = 1;
   pdsch->beamPdschInfo.digBfInterfaces = 0;
   pdsch->beamPdschInfo.prg[0].pmIdx = 0;
   pdsch->beamPdschInfo.prg[0].beamIdx[0] = 0;
   pdsch->txPdschPower.powerControlOffset = 0;
   pdsch->txPdschPower.powerControlOffsetSS = 0;

   pdcch->dci.pdschCfg = pdsch;
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills k0 and k1 information table for FDD 
 *
 * @details
 *
 *    Function : BuildK0K1TableForFdd 
 *
 *    Functionality:
 *      Fills k0 and k1 information table for FDD
 *
 * @params[in] SchCellCb *cell,SchK0K1TimingInfoTbl *k0K1InfoTbl,bool
 * pdschCfgCmnPres,uint8_t numTimeDomAlloc, SchPdschCfgCmnTimeDomRsrcAlloc
 * cmnTimeDomRsrcAllocList[], SchPdschTimeDomRsrcAlloc
 * dedTimeDomRsrcAllocList[], uint8_t ulAckListCount, uint8_t *UlAckTbl
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void BuildK0K1TableForFdd(SchCellCb *cell, SchK0K1TimingInfoTbl *k0K1InfoTbl, bool pdschCfgCmnPres,SchPdschCfgCmn pdschCmnCfg,\
SchPdschConfig pdschDedCfg, uint8_t ulAckListCount, uint8_t *UlAckTbl)
{
   
   uint8_t k1TmpVal =0, cfgIdx=0;
   uint8_t slotIdx=0, k0Index=0, k1Index=0, numK0=0, numK1=0, numTimeDomAlloc=0;
   
   /* TODO Commented these below lines for resolving warnings. Presently these variable are not 
    * required but this will require for harq processing */
   // uint8_t k0TmpVal = 0; 
   // SchPdschCfgCmnTimeDomRsrcAlloc cmnTimeDomRsrcAllocList[MAX_NUM_DL_ALLOC];
   // SchPdschTimeDomRsrcAlloc dedTimeDomRsrcAllocList[MAX_NUM_DL_ALLOC];

   /* Initialization the structure and storing the total slot values. */
   memset(k0K1InfoTbl, 0, sizeof(SchK0K1TimingInfoTbl));
   k0K1InfoTbl->tblSize = cell->numSlots;
   
   /* Storing time domain resource allocation list based on common or dedicated configuration. */
   if(pdschCfgCmnPres == true)
   {
      numTimeDomAlloc = pdschCmnCfg.numTimeDomAlloc;
      for(cfgIdx = 0; cfgIdx<numTimeDomAlloc; cfgIdx++)
      {
         /*TODO uncomment this line during harq processing */
         //cmnTimeDomRsrcAllocList[cfgIdx] = pdschCmnCfg.timeDomRsrcAllocList[cfgIdx];
      }
   }
   else
   {
      numTimeDomAlloc = pdschDedCfg.numTimeDomRsrcAlloc;
      for(cfgIdx = 0; cfgIdx<numTimeDomAlloc; cfgIdx++)
      {
         /*TODO uncomment this line during harq processing */
         //dedTimeDomRsrcAllocList[cfgIdx] = pdschDedCfg.timeDomRsrcAllociList[cfgIdx];
      }
   }
   
   /* Checking all the slots for K0 and K1 values. */
   for(slotIdx = 0; slotIdx < cell->numSlots; slotIdx++)
   {
      numK0 = 0;
      /* Storing the values of k0 based on time domain resource
       * allocation list. If the value is unavailable then fill default values,
       * As per 38.331 PDSCH-TimeDomainResourceAllocation field descriptions. */
      for(k0Index = 0; ((k0Index < numTimeDomAlloc) && (k0Index < MAX_NUM_K0_IDX));  k0Index++)
      {
         /* TODO These if 0 we will remove during harq processing */
#if 0
         if(pdschCfgCmnPres == true)
         {
            k0TmpVal = cmnTimeDomRsrcAllocList[k0Index].k0;
         }
         else
         {
            if(dedTimeDomRsrcAllocList[k0Index].k0 != NULLP)
            {
               k0TmpVal = *(dedTimeDomRsrcAllocList[k0Index].k0);
            }
            else
            { 
               k0TmpVal = DEFAULT_K0_VALUE;
            }
         }
#endif         
         /* Checking all the Ul Alloc values. If value is less than MIN_NUM_K1_IDX
          * then skip else continue storing the values. */
         numK1 = 0;
         for(k1Index = 0; k1Index < ulAckListCount; k1Index++)
         {
            k1TmpVal = UlAckTbl[k1Index];
            if(k1TmpVal <= MIN_NUM_K1_IDX)
            {
               continue;
            }

            k0K1InfoTbl->k0k1TimingInfo[slotIdx].k0Indexes[numK0].k1TimingInfo.k1Indexes[numK1++] = k1Index;
            /* TODO Store K1 index where harq feedback will be received in harq table. */ 
         }
         if(numK1)
         {
            k0K1InfoTbl->k0k1TimingInfo[slotIdx].k0Indexes[numK0].k1TimingInfo.numK1 = numK1;
            k0K1InfoTbl->k0k1TimingInfo[slotIdx].k0Indexes[numK0].k0Index = k0Index;
            numK0++;
         }
      }
      if(numK0)
      {
         k0K1InfoTbl->k0k1TimingInfo[slotIdx].numK0 = numK0;
      }
   }
}

/*******************************************************************
 *
 * @brief Fills k0 and k1 information table  
 *
 * @details
 *
 *    Function : BuildK0K1Table
 *
 *    Functionality:
 *       Fills K0 and k1 information table 
 *
 * @params[in] SchCellCb *cell,SchK0K1TimingInfoTbl *k0K1InfoTbl,bool
 * pdschCfgCmnPres,uint8_t numTimeDomAlloc, SchPdschCfgCmnTimeDomRsrcAlloc
 * cmnTimeDomRsrcAllocList[], SchPdschTimeDomRsrcAlloc
 * dedTimeDomRsrcAllocList[], uint8_t ulAckListCount, uint8_t *UlAckTbl
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void BuildK0K1Table(SchCellCb *cell, SchK0K1TimingInfoTbl *k0K1InfoTbl, bool pdschCfgCmnPres, SchPdschCfgCmn pdschCmnCfg,\
SchPdschConfig pdschDedCfg, uint8_t ulAckListCount, uint8_t *UlAckTbl)
{

#ifdef NR_TDD
   SlotConfig  slotCfg;
   bool ulSlotPresent = false;
   uint8_t k0TmpVal = 0, k1TmpVal =0, tmpSlot=0, startSymbol=0, endSymbol=0, checkSymbol=0;
   uint8_t slotIdx=0, k0Index=0, k1Index=0, numK0=0, numK1=0, cfgIdx=0, numTimeDomAlloc =0, totalCfgSlot =0;
   SchPdschCfgCmnTimeDomRsrcAlloc cmnTimeDomRsrcAllocList[MAX_NUM_DL_ALLOC];
   SchPdschTimeDomRsrcAlloc dedTimeDomRsrcAllocList[MAX_NUM_DL_ALLOC];
#endif

   if(cell->cellCfg.dupMode == DUPLEX_MODE_FDD)
   {
      BuildK0K1TableForFdd(cell, k0K1InfoTbl, pdschCfgCmnPres, pdschCmnCfg, pdschDedCfg, ulAckListCount, UlAckTbl);
   }
   else
   {
#ifdef NR_TDD
      
      /* Initialization the K0K1 structure, total num of slot and calculating the slot pattern length. */
      memset(k0K1InfoTbl, 0, sizeof(SchK0K1TimingInfoTbl));
      k0K1InfoTbl->tblSize = cell->numSlots;
      totalCfgSlot = calculateSlotPatternLength(cell->cellCfg.scsCommon, cell->cellCfg.tddCfg.tddPeriod);
      
      /* Storing time domain resource allocation list based on common or 
       * dedicated configuration availability. */
      if(pdschCfgCmnPres == true)
      {
         numTimeDomAlloc = pdschCmnCfg.numTimeDomAlloc;
         for(cfgIdx = 0; cfgIdx<numTimeDomAlloc; cfgIdx++)
         {
            cmnTimeDomRsrcAllocList[cfgIdx] = pdschCmnCfg.timeDomRsrcAllocList[cfgIdx];
         }
      }
      else
      {
         numTimeDomAlloc = pdschDedCfg.numTimeDomRsrcAlloc;
         for(cfgIdx = 0; cfgIdx<numTimeDomAlloc; cfgIdx++)
         {
            dedTimeDomRsrcAllocList[cfgIdx] = pdschDedCfg.timeDomRsrcAllociList[cfgIdx];
         }
      }

      /* Checking all possible indexes for K0 and K1 values. */
      for(slotIdx = 0; slotIdx < cell->numSlots; slotIdx++)
      {
         /* If current slot is UL or FLEXI then Skip because PDCCH is sent only in DL slots. */
         slotCfg = schGetSlotSymbFrmt(slotIdx%totalCfgSlot, cell->slotFrmtBitMap);
         if(slotCfg == UL_SLOT || slotCfg == FLEXI_SLOT)
         {
            continue;
         }
        
         ulSlotPresent = false;
         /* Storing K0 , start symbol and length symbol for further processing.
          * If K0 value is not available then we can fill the default values
          * given in spec 38.331. */
         numK0 = 0;
         for(k0Index = 0; ((k0Index < numTimeDomAlloc) && (k0Index < MAX_NUM_K0_IDX)); k0Index++)
         {
            if(pdschCfgCmnPres == true)
            {
               k0TmpVal = cmnTimeDomRsrcAllocList[k0Index].k0;
               startSymbol = cmnTimeDomRsrcAllocList[k0Index].startSymbol;
               endSymbol = startSymbol + cmnTimeDomRsrcAllocList[k0Index].lengthSymbol;
            }
            else
            {
               if(dedTimeDomRsrcAllocList[k0Index].k0 != NULLP)
               {
                  k0TmpVal =  *(dedTimeDomRsrcAllocList[k0Index].k0);
               }
               else
               {
                  k0TmpVal = DEFAULT_K0_VALUE;
               }
               startSymbol = dedTimeDomRsrcAllocList[k0Index].startSymbol;
               endSymbol = startSymbol + dedTimeDomRsrcAllocList[k0Index].symbolLength;
            }
            
            /* If current slot + k0 is UL then skip the slot
             * else if it is DL slot then continue the next steps
             * else if it is a FLEXI slot then check symbols of slot, It should not
             * contain any UL slot. */
            tmpSlot = (slotIdx+k0TmpVal) % totalCfgSlot;
            slotCfg = schGetSlotSymbFrmt(tmpSlot, cell->slotFrmtBitMap);
            if(slotCfg == UL_SLOT)
            {
               continue;
            }
            if(slotCfg == FLEXI_SLOT)
            {
               for(checkSymbol = startSymbol; checkSymbol<endSymbol; checkSymbol ++)
               {
                  slotCfg = cell->slotCfg[tmpSlot][checkSymbol];
                  if(slotCfg == UL_SLOT)
                  {
                     ulSlotPresent = true;
                     break;
                  }
               }
               if(ulSlotPresent == true)
               {
                  continue;
               }
            }

             ulSlotPresent = false; //Re-initializing

            /* If current slot + k0 + k1 is a DL slot then skip the slot
             * else if it is UL slot then store the information 
             * else if it is FLEXI slot then check the symbols, it must have
             * at least one UL symbol. */
            numK1 = 0;
            for(k1Index = 0; k1Index < ulAckListCount; k1Index++)
            {
               k1TmpVal = UlAckTbl[k1Index];
               if(k1TmpVal > MIN_NUM_K1_IDX)
               {
                  tmpSlot = (slotIdx+k0TmpVal+k1TmpVal) % totalCfgSlot;
                  slotCfg =  schGetSlotSymbFrmt(tmpSlot, cell->slotFrmtBitMap);
                  if(slotCfg == DL_SLOT) 
                  {
                     continue;
                  }   
                  if(slotCfg == FLEXI_SLOT)
                  {
                     for(checkSymbol = 0; checkSymbol< MAX_SYMB_PER_SLOT;checkSymbol++)
                     {
                        if(cell->slotCfg[tmpSlot][checkSymbol] == UL_SYMBOL)
                        {
                           ulSlotPresent = true;
                           break;
                        }
                     }
                  }
                  if(ulSlotPresent == true || slotCfg ==  UL_SLOT)
                  {
                     k0K1InfoTbl->k0k1TimingInfo[slotIdx].k0Indexes[numK0].k1TimingInfo.k1Indexes[numK1++] = k1Index;
                     /* TODO Store K1 index where harq feedback will be received
                      * in harq table. */
                  }
               }
            }
            
            /* Store all the values if all condition satisfies. */
            if(numK1)
            {
               k0K1InfoTbl->k0k1TimingInfo[slotIdx].k0Indexes[numK0].k1TimingInfo.numK1 = numK1;
               k0K1InfoTbl->k0k1TimingInfo[slotIdx].k0Indexes[numK0].k0Index = k0Index;
               numK0++;
            }
         }
         if(numK0)
         {
            k0K1InfoTbl->k0k1TimingInfo[slotIdx].numK0 = numK0;
         }
      }
#endif
   }
}

/*******************************************************************
*
* @brief Fills K2 information table for FDD
*
* @details
*
*    Function : BuildK2InfoTableForFdd 
*
*    Functionality:
*       Fills K2 information table for FDD
*
* @params[in] SchCellCb *cell,SchPuschTimeDomRsrcAlloc timeDomRsrcAllocList[],
* uint16_t puschSymTblSize,SchK2TimingInfoTbl *k2InfoTbl
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
void BuildK2InfoTableForFdd(SchCellCb *cell, SchPuschTimeDomRsrcAlloc timeDomRsrcAllocList[], uint16_t puschSymTblSize,\
SchK2TimingInfoTbl *msg3K2InfoTbl, SchK2TimingInfoTbl *k2InfoTbl)
{
   uint16_t slotIdx=0, k2Index=0, k2TmpIdx=0, msg3K2TmpIdx=0;

   /* Initialization the structure and storing the total slot values. */
   memset(k2InfoTbl, 0, sizeof(SchK2TimingInfoTbl));
   k2InfoTbl->tblSize = cell->numSlots;
   if(msg3K2InfoTbl)
      msg3K2InfoTbl->tblSize = cell->numSlots;
   
   /* Checking all possible indexes for K2. */
   for(slotIdx = 0; slotIdx < cell->numSlots; slotIdx++)
   {
      /* Storing K2 values. */
      for(k2Index = 0; ((k2Index < puschSymTblSize) && (k2Index < MAX_NUM_K2_IDX)); k2Index++)
      {
         k2TmpIdx= k2InfoTbl->k2TimingInfo[slotIdx].numK2;
         k2InfoTbl->k2TimingInfo[slotIdx].k2Indexes[k2TmpIdx] = k2Index;
         k2InfoTbl->k2TimingInfo[slotIdx].numK2++;

         /* Updating K2 values for MSG3 */
         if(msg3K2InfoTbl)
         {
            msg3K2TmpIdx = msg3K2InfoTbl->k2TimingInfo[slotIdx].numK2;
            msg3K2InfoTbl->k2TimingInfo[slotIdx].k2Indexes[msg3K2TmpIdx] = k2Index;
            msg3K2InfoTbl->k2TimingInfo[slotIdx].numK2++;
         }
      }
   }
}

/*******************************************************************
 *
 * @brief Fills K2 information table
 *
 * @details
 *
 *    Function : BuildK2InfoTable 
 *
 *    Functionality:
 *       Fills K2 information table
 *
 * @params[in] SchCellCb *cell,SchPuschTimeDomRsrcAlloc timeDomRsrcAllocList[],
 * uint16_t puschSymTblSize, SchK2TimingInfoTbl *k2InfoTbl
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void BuildK2InfoTable(SchCellCb *cell, SchPuschTimeDomRsrcAlloc timeDomRsrcAllocList[], uint16_t puschSymTblSize,\
SchK2TimingInfoTbl *msg3K2InfoTbl, SchK2TimingInfoTbl *k2InfoTbl)
{

#ifdef NR_TDD
   bool dlSymbolPresent = false;
   uint8_t slotIdx=0, k2Index=0, k2Val=0, k2TmpVal=0, msg3K2TmpVal=0, msg3Delta=0, numK2 =0, currentSymbol =0;
   uint8_t startSymbol =0, endSymbol =0, checkSymbol=0, totalCfgSlot=0, slotCfg=0;
   SlotConfig currentSlot;
#endif

   if(cell->cellCfg.dupMode == DUPLEX_MODE_FDD)
   {
      BuildK2InfoTableForFdd(cell, timeDomRsrcAllocList, puschSymTblSize, msg3K2InfoTbl, k2InfoTbl);
   }
   else
   {
#ifdef NR_TDD

      /* Initialization the structure and storing the total slot values. */
      memset(k2InfoTbl, 0, sizeof(SchK2TimingInfoTbl));
      k2InfoTbl->tblSize = cell->numSlots;
      if(msg3K2InfoTbl)
         msg3K2InfoTbl->tblSize = cell->numSlots;
      totalCfgSlot = calculateSlotPatternLength(cell->cellCfg.scsCommon, cell->cellCfg.tddCfg.tddPeriod);

      /* Checking all possible indexes for K2. */
      for(slotIdx = 0; slotIdx < cell->numSlots; slotIdx++)
      {
         currentSlot = schGetSlotSymbFrmt(slotIdx % totalCfgSlot, cell->slotFrmtBitMap);
         
         /* If current slot is UL then skip because PDCCH is sent only in DL slots */
         if(currentSlot != UL_SLOT)
         {
            for(k2Index = 0; ((k2Index < puschSymTblSize) && (k2Index < MAX_NUM_K2_IDX)); k2Index++)
            {
               /* Storing k2, startSymbol, endSymbol information for further processing.
                * If k2 is absent then fill the default values given in spec 38.331
                * PUSCH-TimeDomainResourceAllocationList field descriptions */
               k2Val = timeDomRsrcAllocList[k2Index].k2;
               if(!k2Val)
               {
                  switch(cell->cellCfg.scsCommon)
                  {
                     case SCS_15KHZ:
                        k2Val = DEFAULT_K2_VALUE_FOR_SCS15;
                        break;
                     case SCS_30KHZ:
                        k2Val = DEFAULT_K2_VALUE_FOR_SCS30;
                        break;
                     case SCS_60KHZ:
                        k2Val = DEFAULT_K2_VALUE_FOR_SCS60;
                        break;
                     case SCS_120KHZ:
                        k2Val = DEFAULT_K2_VALUE_FOR_SCS120;
                        break;
                  }
               }
               
               /* Current slot + k2 should be either UL or FLEXI slot.
                * If slot is FLEXI then check all the symbols of that slot,
                * it should not contain any DL or FLEXI slot */
               k2TmpVal = (slotIdx + k2Val) % totalCfgSlot;
               slotCfg = schGetSlotSymbFrmt(k2TmpVal, cell->slotFrmtBitMap);
               if(slotCfg != DL_SLOT)
               {
                  if(slotCfg == FLEXI_SLOT)
                  {
                     startSymbol =  timeDomRsrcAllocList[k2Index].startSymbol;
                     endSymbol   =  startSymbol+ timeDomRsrcAllocList[k2Index].symbolLength;
                     dlSymbolPresent = false;
                     for(checkSymbol= startSymbol; checkSymbol<endSymbol; checkSymbol++)
                     {
                        currentSymbol = cell->slotCfg[k2TmpVal][checkSymbol];
                        if(currentSymbol == DL_SLOT || currentSymbol == FLEXI_SLOT)
                        {
                           dlSymbolPresent = true;
                           break;
                        }
                     }
                  }
                  /* Store all the values if all condition satisfies. */
                  if(dlSymbolPresent != true || slotCfg == UL_SLOT)
                  {
                     numK2 = k2InfoTbl->k2TimingInfo[slotIdx].numK2;
                     k2InfoTbl->k2TimingInfo[slotIdx].k2Indexes[numK2] = k2Index;
                     k2InfoTbl->k2TimingInfo[slotIdx].numK2++;
                  }
               }

               if(msg3K2InfoTbl)
               {
                   msg3Delta = puschDeltaTable[cell->cellCfg.numerology];

                  /* Check for K2 for MSG3 */
                  /* Current slot + k2 should be either UL or FLEXI slot.
                   * If slot is FLEXI then check all the symbols of that slot,
                   * it should not contain any DL or FLEXI slot */
                  msg3K2TmpVal = (slotIdx + k2Val + msg3Delta) % totalCfgSlot;
                  slotCfg = schGetSlotSymbFrmt(msg3K2TmpVal, cell->slotFrmtBitMap);
                  if(slotCfg != DL_SLOT)
                  {
                     if(slotCfg == FLEXI_SLOT)
                     {
                        startSymbol =  timeDomRsrcAllocList[k2Index].startSymbol;
                        endSymbol   =  startSymbol+ timeDomRsrcAllocList[k2Index].symbolLength;
                        dlSymbolPresent = false;
                        for(checkSymbol= startSymbol; checkSymbol<endSymbol; checkSymbol++)
                        {
                           currentSymbol = cell->slotCfg[msg3K2TmpVal][checkSymbol];
                           if(currentSymbol == DL_SLOT || currentSymbol == FLEXI_SLOT)
                           {
                              dlSymbolPresent = true;
                              break;
                           }
                        }
                     }
                     /* Store all the values if all condition satisfies. */
                     if(dlSymbolPresent != true || slotCfg == UL_SLOT)
                     {
                        numK2 = msg3K2InfoTbl->k2TimingInfo[slotIdx].numK2;
                        msg3K2InfoTbl->k2TimingInfo[slotIdx].k2Indexes[numK2] = k2Index;
                        msg3K2InfoTbl->k2TimingInfo[slotIdx].numK2++;
                     }
                  }
               }
            }
         }
      }
#endif
   }
}

/*******************************************************************************************
 *
 * @brief Allocate the PRB using RRM policy
 *
 * @details
 *
 *    Function : prbAllocUsingRRMPolicy
 *
 *    Functionality:
 *      [Step1]: Traverse each Node in the LC list
 *      [Step2]: Check whether the LC has ZERO requirement then clean this LC
 *      [Step3]: Calcualte the maxPRB for this LC.
 *              a. For Dedicated LC, maxPRB = sum of remainingReservedPRB and
 *              sharedPRB
 *              b. For Default, just SharedPRB count
 *      [Step4]: If the LC is the First one to be allocated for this UE then add
 *      TX_PAYLODN_LEN to reqBO 
 *      [Step5]: Calculate the estimate PRB and estimate BO to be allocated
 *               based on reqBO and maxPRB left.
 *      [Step6]: Based on calculated PRB, Update Reserved PRB and Shared PRB counts
 *      [Step7]: Deduce the reqBO based on allocBO and move the LC node to last.
 *      [Step8]: Continue the next loop from List->head
 *
 *      [Loop Exit]:
 *        [Exit1]: If all the LCs are allocated in list
 *        [Exit2]: If PRBs are exhausted
 *
 * @params[in] I/P > lcLinkList pointer (LcInfo list)
 *             I/P > IsDedicatedPRB (Flag to indicate that RESERVED PRB to use 
 *             I/P > mcsIdx and PDSCH symbols count 
 *             I/P & O/P > Shared PRB , reserved PRB Count
 *             I/P & O/P > Total TBS size accumulated
 *             I/P & O/P > isTxPayloadLenAdded[For DL] : Decision flag to add the TX_PAYLOAD_HDR_LEN
 *             I/P & O/P > srRcvd Flag[For UL] : Decision flag to add UL_GRANT_SIZE
 *
 * @return void
 *
 * *******************************************************************************************/
void prbAllocUsingRRMPolicy(CmLListCp *lcLL, bool isDedicatedPRB, uint16_t mcsIdx,uint8_t numSymbols,\
                  uint16_t *sharedPRB, uint16_t *reservedPRB, bool *isTxPayloadLenAdded, bool *srRcvd)
{
   CmLList *node = NULLP;
   LcInfo *lcNode = NULLP;
   uint16_t remReservedPRB = 0, estPrb = 0, maxPRB = 0;

   if(lcLL == NULLP)
   {
      DU_LOG("\nERROR --> SCH: LcList not present");
      return;
   }
   node = lcLL->first;

   /*Only for Dedicated LcList, Valid value will be assigned to remReservedPRB
    * For Other LcList, remReservedPRB = 0*/
   if(reservedPRB != NULLP && isDedicatedPRB == TRUE)
   {
      remReservedPRB = *reservedPRB;
   }

   /*[Step1]*/
   while(node)
   {
#if 0
      /*For Debugging purpose*/
      printLcLL(lcLL);
#endif
      lcNode = (LcInfo *)node->node;

      /* [Step2]: Below condition will hit in rare case as it has been taken care during the cleaning 
       * process of LCID which was fully allocated. Check is just for safety purpose*/
      if(lcNode->reqBO == 0 && lcNode->allocBO == 0)
      {
         DU_LOG("\nERROR --> SCH: LCID:%d has no requirement, clearing this node",\
               lcNode->lcId);
         deleteNodeFromLList(lcLL, node);
         SCH_FREE(lcNode, sizeof(LcInfo));
         node = lcLL->first; 
         continue;
      }

      /*[Exit1]: All LCs are allocated(allocBO = 0 for fully unallocated LC)*/
      if(lcNode->allocBO != 0)
      {
         DU_LOG("\nDEBUG -->  SCH: All LC are allocated [SharedPRB:%d]",*sharedPRB);
         return;
      }

      /*[Exit2]: If PRBs are exhausted*/
      if(isDedicatedPRB)
      {
         /*Loop Exit: All resources exhausted*/
         if(remReservedPRB == 0 && *sharedPRB == 0)
         {
            DU_LOG("\nDEBUG  -->  SCH: Dedicated resources exhausted for LC:%d",lcNode->lcId);
            return;
         }
      }
      else
      {
         /*Loop Exit: All resources exhausted*/
         if(*sharedPRB == 0)
         {
            DU_LOG("\nDEBUG  --> SCH: Default resources exhausted for LC:%d",lcNode->lcId);
            return;
         }
      }

      /*[Step3]*/
      maxPRB = remReservedPRB + *sharedPRB;

      /*[Step4]*/
      if((isTxPayloadLenAdded != NULLP) && (*isTxPayloadLenAdded == FALSE))
      {
         DU_LOG("\nDEBUG  --> SCH: LC:%d is the First node to be allocated which includes TX_PAYLOAD_HDR_LEN",\
               lcNode->lcId);
         *isTxPayloadLenAdded = TRUE;
         lcNode->allocBO = calculateEstimateTBSize((lcNode->reqBO + TX_PAYLOAD_HDR_LEN),\
               mcsIdx, numSymbols, maxPRB, &estPrb);
         lcNode->allocBO -=TX_PAYLOAD_HDR_LEN;
      }
      else if((srRcvd != NULLP) && (*srRcvd == TRUE))
      {
         DU_LOG("\nDEBUG  --> SCH: LC:%d is the First node to be allocated which includes UL_GRANT_SIZE",\
               lcNode->lcId);
         *srRcvd = FALSE;
         lcNode->reqBO += UL_GRANT_SIZE;
         lcNode->allocBO = calculateEstimateTBSize(lcNode->reqBO, mcsIdx, numSymbols, maxPRB, &estPrb);
      }
      else
      {
         /*[Step4]*/
         lcNode->allocBO = calculateEstimateTBSize(lcNode->reqBO,\
               mcsIdx, numSymbols, maxPRB, &estPrb);
      }

      /*[Step6]:Re-adjust the reservedPRB pool count and *SharedPRB Count based on
       * estimated PRB allocated*/
      if((isDedicatedPRB == TRUE) && (estPrb <= remReservedPRB))
      {
         remReservedPRB = remReservedPRB - estPrb;
      }
      else   /*LC requirement need PRB share from SharedPRB*/
      {
         if(*sharedPRB <=  (estPrb - remReservedPRB))
         {
            DU_LOG("\nDEBUG  --> SCH: SharedPRB is less");
            *sharedPRB = 0;
         }
         else
         {
            *sharedPRB = *sharedPRB - (estPrb - remReservedPRB);
         }
         remReservedPRB = 0;
      }

      /*[Step7]*/
      lcNode->reqBO -= lcNode->allocBO;  /*Update the reqBO with remaining bytes unallocated*/
      lcNode->allocPRB = estPrb;
      cmLListAdd2Tail(lcLL, cmLListDelFrm(lcLL, node));

      /*[Step8]:Next loop: First LC to be picked from the list
       * because Allocated Nodes are moved to the last*/
      node = lcLL->first; 

   }
   return;
}

/*******************************************************************************************
 *
 * @brief Check the LC List and fill the LC and GrantSize to be sent to MAC as
 * BO Report
 *
 * @details
 *
 *    Function : updateGrantSizeForBoRpt
 *
 *    Functionality:
 *             Check the LC List and fill the LC and GrantSize to be sent to MAC as
 *             BO Report in dlMsgAlloc Pointer
 *
 * @params[in] I/P > lcLinkList pointer (LcInfo list)
 *             I/P & O/P > dlMsgAlloc[for DL](Pending LC to be added in this context) 
 *             I/P & O/P > BsrInfo (applicable for UL)
 *             I/P & O/P > accumalatedBOSize
 * @return void
 *
 * *******************************************************************************************/
void updateGrantSizeForBoRpt(CmLListCp *lcLL, DlMsgAlloc *dlMsgAlloc,\
                                BsrInfo *bsrInfo, uint32_t *accumalatedBOSize)
{
   CmLList *node = NULLP, *next = NULLP;
   LcInfo *lcNode = NULLP;
   DlMsgSchInfo *dlMsgSchInfo = NULLP;

   if(lcLL == NULLP)
   {
      DU_LOG("\nERROR --> SCH: LcList not present");
      return;
   }

   if(lcLL->count)
   {
      node = lcLL->first;
   }
   else
   {
      /*lcLL is empty*/
      return;
   }

   /*Traverse List*/
   while(node)
   {
      next = node->next;
      lcNode = (LcInfo *)node->node;
      if(lcNode != NULLP)
      {
         DU_LOG("\nINFO   --> SCH : LcID:%d, [reqBO, allocBO, allocPRB]:[%d,%d,%d]",\
               lcNode->lcId, lcNode->reqBO, lcNode->allocBO, lcNode->allocPRB);
         if(dlMsgAlloc != NULLP)
         {
            dlMsgSchInfo = &dlMsgAlloc->dlMsgSchedInfo[dlMsgAlloc->numSchedInfo];

            /*Add this LC to dlMsgAlloc so that if this LC gets allocated, BO
             * report for allocation can be sent to MAC*/
            dlMsgSchInfo->lcSchInfo[dlMsgSchInfo->numLc].lcId = lcNode->lcId;
            dlMsgSchInfo->lcSchInfo[dlMsgSchInfo->numLc].schBytes = lcNode->allocBO;

            /*Calculate the Total Payload/BO size allocated*/
            *accumalatedBOSize += dlMsgSchInfo->lcSchInfo[dlMsgSchInfo->numLc].schBytes; 

            DU_LOG("\nINFO   --> SCH: Added in MAC BO report: LCID:%d,reqBO:%d,Idx:%d, TotalBO Size:%d",\
                  lcNode->lcId,lcNode->reqBO, dlMsgSchInfo->numLc, *accumalatedBOSize);

            dlMsgSchInfo->numLc++;
            handleLcLList(lcLL, lcNode->lcId, DELETE);
         }
         else if(bsrInfo != NULLP)
         {
            *accumalatedBOSize += lcNode->allocBO;   
            DU_LOG("\nINFO   --> SCH: UL : LCID:%d,reqBO:%d, TotalBO Size:%d",\
                  lcNode->lcId,lcNode->reqBO, *accumalatedBOSize);
         }
      }
      node = next;
   }/*End of while*/
   return;
}

/*******************************************************************
*
* @brief fill DL message information for MSG4 and Dedicated DL Msg
*
* @details
*
*    Function : fillDlMsgInfo
*
*    Functionality:
*       fill DL message information for MSG4 and Dedicated DL Msg
*
* @params[in] DlMsgInfo *dlMsgInfo,  uint8_t crnti
* @params[in] bool isRetx, SchDlHqProcCb *hqP
* @return void
*
*******************************************************************/
void fillDlMsgInfo(DlMsgInfo *dlMsgInfo, uint8_t crnti, bool isRetx, SchDlHqProcCb *hqP)
{
   hqP->tbInfo[0].isEnabled = TRUE;
   hqP->tbInfo[0].state = HQ_TB_WAITING;
   hqP->tbInfo[0].txCntr++;
   hqP->tbInfo[1].isEnabled = TRUE;
   hqP->tbInfo[1].state = HQ_TB_WAITING;
   hqP->tbInfo[1].txCntr++;
   dlMsgInfo->crnti = crnti;
   dlMsgInfo->ndi = hqP->tbInfo[0].ndi; /*How to handle two tb case?TBD*/
   dlMsgInfo->harqProcNum = hqP->procId;
   dlMsgInfo->dlAssignIdx = 0;
   dlMsgInfo->pucchTpc = 0;
   dlMsgInfo->pucchResInd = 0;
   dlMsgInfo->harqFeedbackInd = hqP->k1;
   dlMsgInfo->dciFormatId = 1;
}

/*******************************************************************
 *
 * @brief sch Process pending Msg4 Req
 *
 * @details
 *
 *    Function : schProcessMsg4Req
 *
 *    Functionality:
 *       sch Process pending Msg4 Req
 *
 * @params[in] SchCellCb *cell, cell cb struct pointer
 * @params[in] SlotTimingInfo currTime, current timing info
 * @params[in] uint8_t ueId, ue ID
 * @params[in] bool isRetxMsg4, indicator to MSG4 retransmission
 * @params[in] SchDlHqProcCb **msg4HqProc, address of MSG4 HARQ proc pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************/

uint8_t schProcessMsg4Req(SchCellCb *cell, SlotTimingInfo currTime, uint8_t ueId, bool isRetxMsg4, SchDlHqProcCb **msg4HqProc)
{
   uint8_t pdschStartSymbol = 0, pdschNumSymbols = 0;
   SlotTimingInfo pdcchTime, pdschTime, pucchTime;
   DlMsgAlloc *dciSlotAlloc = NULLP;    /* Stores info for transmission of PDCCH for Msg4 */
   DlMsgAlloc  *msg4SlotAlloc = NULLP;   /* Stores info for transmission of PDSCH for Msg4 */

   if(cell == NULL)
   {
      DU_LOG("\nERROR  -->  SCH: schDlRsrcAllocMsg4() : Cell is NULL");
      return RFAILED;
   }

   if (isRetxMsg4 == FALSE)
   {
      if (RFAILED == schDlGetAvlHqProcess(cell, &cell->ueCb[ueId - 1], msg4HqProc))
      {
         DU_LOG("\nERROR  -->  SCH: schDlRsrcAllocMsg4() : No process");
         return RFAILED;
      }
   }

   if(findValidK0K1Value(cell, currTime, ueId, false, &pdschStartSymbol, &pdschNumSymbols, &pdcchTime, &pdschTime,\
            &pucchTime, isRetxMsg4, *msg4HqProc) != true )
   {
      DU_LOG("\nERROR  -->  SCH: schDlRsrcAllocMsg4() : k0 k1 not found");
      return RFAILED;
   }

   if(cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId-1] == NULL)
   {
      SCH_ALLOC(dciSlotAlloc, sizeof(DlMsgAlloc));
      if(dciSlotAlloc == NULLP)
      {
         DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for dciSlotAlloc");
         return RFAILED;
      }
      cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId-1] = dciSlotAlloc;
      memset(dciSlotAlloc, 0, sizeof(DlMsgAlloc));
      GET_CRNTI(dciSlotAlloc->crnti, ueId);
   }
   else
      dciSlotAlloc = cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId-1];

   /* Fill PDCCH and PDSCH scheduling information for Msg4 */
   if((schDlRsrcAllocMsg4(cell, pdschTime, ueId, dciSlotAlloc, pdschStartSymbol, pdschNumSymbols, isRetxMsg4, *msg4HqProc)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH: Scheduling of Msg4 failed in slot [%d]", pdschTime.slot);
      if(dciSlotAlloc->numSchedInfo == 0)
      {
         SCH_FREE(dciSlotAlloc, sizeof(DlMsgAlloc));
         cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId-1] = NULLP;
      }
      else
         memset(&dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo], 0, sizeof(DlMsgSchInfo));
      return RFAILED;
   }

   /* Check if both DCI and RAR are sent in the same slot.
    * If not, allocate memory RAR PDSCH slot to store RAR info
    */
   if(pdcchTime.slot == pdschTime.slot)
   {
      dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo].pduPres = BOTH;
      dciSlotAlloc->numSchedInfo++;
   }
   else
   {
      /* Allocate memory to schedule rarSlot to send RAR, pointer will be checked at schProcessSlotInd() */
      if(cell->schDlSlotInfo[pdschTime.slot]->dlMsgAlloc[ueId-1] == NULL)
      {
         SCH_ALLOC(msg4SlotAlloc, sizeof(DlMsgAlloc));
         if(msg4SlotAlloc == NULLP)
         {
            DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for msg4SlotAlloc");
            if(dciSlotAlloc->numSchedInfo == 0)
            {
               SCH_FREE(dciSlotAlloc, sizeof(DlMsgAlloc));
               cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueId-1] = NULLP;
            }
            else
               memset(&dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo], 0, sizeof(DlMsgSchInfo));
            return RFAILED;
         }
         cell->schDlSlotInfo[pdschTime.slot]->dlMsgAlloc[ueId-1] = msg4SlotAlloc;
         memset(msg4SlotAlloc, 0, sizeof(DlMsgAlloc));
         msg4SlotAlloc->crnti = dciSlotAlloc->crnti;
      }
      else
         msg4SlotAlloc = cell->schDlSlotInfo[pdschTime.slot]->dlMsgAlloc[ueId-1];

      /* Copy all RAR info */
      memcpy(&msg4SlotAlloc->dlMsgSchedInfo[msg4SlotAlloc->numSchedInfo], \
         &dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo], sizeof(DlMsgSchInfo));
      msg4SlotAlloc->dlMsgSchedInfo[msg4SlotAlloc->numSchedInfo].dlMsgPdcchCfg.dci.pdschCfg = \
         &msg4SlotAlloc->dlMsgSchedInfo[msg4SlotAlloc->numSchedInfo].dlMsgPdschCfg;

      /* Assign correct PDU types in corresponding slots */
      msg4SlotAlloc->dlMsgSchedInfo[msg4SlotAlloc->numSchedInfo].pduPres = PDSCH_PDU;
      dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo].pduPres = PDCCH_PDU;
      dciSlotAlloc->dlMsgSchedInfo[dciSlotAlloc->numSchedInfo].pdschSlot = pdschTime.slot;

      dciSlotAlloc->numSchedInfo++;
      msg4SlotAlloc->numSchedInfo++;
   }

   /* PUCCH resource */
   schAllocPucchResource(cell, pucchTime, cell->raCb[ueId-1].tcrnti, &cell->ueCb[ueId-1], isRetxMsg4, *msg4HqProc);

   cell->schDlSlotInfo[pdcchTime.slot]->pdcchUe = ueId;
   cell->schDlSlotInfo[pdschTime.slot]->pdschUe = ueId;
   cell->schUlSlotInfo[pucchTime.slot]->pucchUe = ueId;
   cell->raCb[ueId-1].msg4recvd = FALSE;
   if(isRetxMsg4)
   {
      cell->ueCb[ueId-1].retxMsg4HqProc= NULLP;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief sch Process pending Sr or Bsr Req
 *
 * @details
 *
 *    Function : updateBsrAndLcList
 *
 *    Functionality:
 *       Updating the BSRInfo in UECB and Lclist
 *
 * @params[in] SchCellCb *cell,  SlotTimingInfo currTime 
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************/
void updateBsrAndLcList(CmLListCp *lcLL, BsrInfo *bsrInfo, uint8_t status)
{
   CmLList *node = NULLP, *next = NULLP;
   LcInfo *lcNode = NULLP;

   if(lcLL == NULLP)
   {
      DU_LOG("\nERROR --> SCH: LcList not present");
      return;
   }

   if(lcLL->count)
   {
      node = lcLL->first;
   }
   else
   {
      /*lcLL is empty*/
      return;
   }

   while(node)
   {
      next = node->next;
      lcNode = (LcInfo *)node->node;
      if(lcNode != NULLP)
      {
          /*Only when Status is OK then allocation is marked as ZERO and reqBO
           * is updated in UE's DB. If Failure, then allocation is added to reqBO 
           * and same is updated in Ue's DB inside BSR Info structure*/
         if(status == ROK)
         {
            lcNode->allocBO = 0;
         }

         lcNode->reqBO += lcNode->allocBO;
         bsrInfo[lcNode->lcId].dataVol = lcNode->reqBO;
         if(lcNode->reqBO == 0)
         {
            handleLcLList(lcLL, lcNode->lcId, DELETE);
         }
      }
      node = next;
   }
}

/*******************************************************************
 *
 * @brief sch Process pending Sr or Bsr Req
 *
 * @details
 *
 *    Function : schProcessSrOrBsrReq
 *
 *    Functionality:
 *       sch Process pending Sr or Bsr Req
 *
 * @params[in] SchCellCb *cell,  SlotTimingInfo currTime
 * @params[in] uint8_t ueId, Bool isRetx, SchUlHqProcCb **hqP
 * @return true  - success
 *         false - failure
 *
 *******************************************************************/
bool schProcessSrOrBsrReq(SchCellCb *cell, SlotTimingInfo currTime, uint8_t ueId, bool isRetx, SchUlHqProcCb **hqP)
{
   bool k2Found = FALSE;
   uint8_t startSymb = 0, symbLen = 0;
   uint8_t k2TblIdx = 0, k2Index = 0, k2Val = 0;
   SchUeCb *ueCb;
   SchK2TimingInfoTbl *k2InfoTbl=NULLP;
   SlotTimingInfo dciTime, puschTime;
   
   if(cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH: schProcessSrOrBsrReq() : Cell is NULL");
      return false;
   }

   ueCb = &cell->ueCb[ueId-1];

   if(ueCb == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH: schProcessSrOrBsrReq() : UE is NULL");
      return false;
   }

   if (isRetx == FALSE)
   {
      if (schUlGetAvlHqProcess(cell, ueCb, hqP) != ROK)
      {
         return RFAILED;
      }
   }

   /* Calculating time frame to send DCI for SR */
   ADD_DELTA_TO_TIME(currTime, dciTime, PHY_DELTA_DL + SCHED_DELTA, cell->numSlots);
#ifdef NR_TDD
   if(schGetSlotSymbFrmt(dciTime.slot, cell->slotFrmtBitMap) == DL_SLOT)
#endif
   {     
      if(ueCb->ueCfg.spCellCfg.servCellRecfg.initUlBwp.k2TblPrsnt)
         k2InfoTbl = &ueCb->ueCfg.spCellCfg.servCellRecfg.initUlBwp.k2InfoTbl;
      else
         k2InfoTbl =  &cell->k2InfoTbl;

      for(k2TblIdx = 0; k2TblIdx < k2InfoTbl->k2TimingInfo[dciTime.slot].numK2; k2TblIdx++)
      {
         k2Index = k2InfoTbl->k2TimingInfo[dciTime.slot].k2Indexes[k2TblIdx];

         if(!ueCb->ueCfg.spCellCfg.servCellRecfg.initUlBwp.k2TblPrsnt)
         {
            k2Val = cell->cellCfg.ulCfgCommon.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[k2Index].k2;
            startSymb = cell->cellCfg.ulCfgCommon.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[k2Index].startSymbol;
            symbLen = cell->cellCfg.ulCfgCommon.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[k2Index].symbolLength;
         }
         else
         {
            k2Val = ueCb->ueCfg.spCellCfg.servCellRecfg.initUlBwp.puschCfg.timeDomRsrcAllocList[k2Index].k2;
            startSymb =  ueCb->ueCfg.spCellCfg.servCellRecfg.initUlBwp.puschCfg.timeDomRsrcAllocList[k2Index].startSymbol;
            symbLen =  ueCb->ueCfg.spCellCfg.servCellRecfg.initUlBwp.puschCfg.timeDomRsrcAllocList[k2Index].symbolLength;
         }
         /* Check for number of Symbol of PUSCH should be same as original in case of transmisson*/
         /* Calculating time frame to send PUSCH for SR */
         ADD_DELTA_TO_TIME(dciTime, puschTime, k2Val, cell->numSlots);
#ifdef NR_TDD
         if(schGetSlotSymbFrmt(puschTime.slot, cell->slotFrmtBitMap) == DL_SLOT)
            continue;
#endif
         if(cell->schUlSlotInfo[puschTime.slot]->puschUe != 0)
         {
            continue;
         }
         k2Found = true;
         if(hqP)
         {
            ADD_DELTA_TO_TIME(puschTime, (*hqP)->puschTime, 0, cell->numSlots);
         }
         break;
      }
   }
   
   if(k2Found == true)
   {
      if(cell->api->SchScheduleUlLc(dciTime, puschTime, startSymb, symbLen, isRetx, hqP) != ROK)
         return false;
   }
   else
   {
      DU_LOG("\nERROR  -->  SCH : schProcessSrOrBsrReq(): K2 value is not found");
      return false;     
   }
   return true;
}

/********************************************************************************
 *
 * @brief Increment the Slot by a input factor
 *
 * @details
 *
 *    Function : schIncrSlot
 *
 *    Functionality:
 *       Increment the slot by a input factor till num of Slots configured in a
 *       Radio Frame. If it exceeds, move to next sfn.
 *
 * @params[in/out] SlotTimingInfo timingInfo
 *        [in]     uint8_t incr [Increment factor]
 *        [in]     numSlotsPerRF [Number of Slots configured per RF as per
 *                                numerology]
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************/
void schIncrSlot(SlotTimingInfo *timingInfo, uint8_t incr, uint16_t numSlotsPerRF)
{
   timingInfo->slot += incr;
   if(timingInfo->slot >= numSlotsPerRF)
   {
      timingInfo->sfn += timingInfo->slot/numSlotsPerRF;
      timingInfo->slot %= numSlotsPerRF;
      if(timingInfo->sfn >  MAX_SFN)
      {
         timingInfo->sfn %= MAX_SFN;
      }
   }
}

/*******************************************************************
*
* @brief   Fill PDSCH info in Page Alloc
*
* @details
*
*    Function : schFillPagePdschCfg 
*
*    Functionality: Fill PDSCH info in Page Alloc
*
* @params[in] SchCellCb *cell, PdschCfg *pagePdschCfg, SlotTimingInfo slotTime, 
*             uint16_t tbsSize, uint8_t mcs, uint16_t startPrb
*
* @return pointer to return Value(ROK, RFAILED) 
*
* ****************************************************************/
uint8_t schFillPagePdschCfg(SchCellCb *cell, PdschCfg *pagePdschCfg, SlotTimingInfo slotTime, uint16_t tbSize, uint8_t mcs, uint16_t startPrb)
{
   uint8_t cwCount = 0;
   uint8_t dmrsStartSymbol, startSymbol, numSymbol;

   /* fill the PDSCH PDU */

   pagePdschCfg->pduBitmap = 0; /* PTRS and CBG params are excluded */
   pagePdschCfg->rnti = P_RNTI; /* SI-RNTI */
   pagePdschCfg->pduIndex = 0;
   pagePdschCfg->numCodewords = 1;
   for(cwCount = 0; cwCount < pagePdschCfg->numCodewords; cwCount++)
   {
      pagePdschCfg->codeword[cwCount].targetCodeRate = 308;
      pagePdschCfg->codeword[cwCount].qamModOrder = 2;
      pagePdschCfg->codeword[cwCount].mcsIndex = mcs;
      pagePdschCfg->codeword[cwCount].mcsTable = 0; /* notqam256 */
      pagePdschCfg->codeword[cwCount].rvIndex = 0;
      tbSize = tbSize + TX_PAYLOAD_HDR_LEN;
      pagePdschCfg->codeword[cwCount].tbSize = tbSize;
   }
   pagePdschCfg->dataScramblingId                   = cell->cellCfg.phyCellId;
   pagePdschCfg->numLayers                          = 1;
   pagePdschCfg->transmissionScheme                 = 0;
   pagePdschCfg->refPoint                           = 0;
   pagePdschCfg->dmrs.dlDmrsSymbPos                 = 4; /* Bitmap value 00000000000100 i.e. using 3rd symbol for PDSCH DMRS */
   pagePdschCfg->dmrs.dmrsConfigType                = 0; /* type-1 */
   pagePdschCfg->dmrs.dlDmrsScramblingId            = cell->cellCfg.phyCellId;
   pagePdschCfg->dmrs.scid                          = 0;
   pagePdschCfg->dmrs.numDmrsCdmGrpsNoData          = 1;
   pagePdschCfg->dmrs.dmrsPorts                     = 0x0001;
   pagePdschCfg->dmrs.mappingType                   = DMRS_MAP_TYPE_A; /* Type-A */
   pagePdschCfg->dmrs.nrOfDmrsSymbols               = NUM_DMRS_SYMBOLS;
   pagePdschCfg->dmrs.dmrsAddPos                    = DMRS_ADDITIONAL_POS;

   pagePdschCfg->pdschFreqAlloc.resourceAllocType   = 1; /* RAT type-1 RIV format */
   /* the RB numbering starts from coreset0, and PDSCH is always above SSB */
   pagePdschCfg->pdschFreqAlloc.startPrb  = startPrb;
   pagePdschCfg->pdschFreqAlloc.numPrb    = schCalcNumPrb(tbSize, mcs, NUM_PDSCH_SYMBOL);
   pagePdschCfg->pdschFreqAlloc.vrbPrbMapping       = 0; /* non-interleaved */
   pagePdschCfg->pdschTimeAlloc.rowIndex            = 1;
   /* This is Intel's requirement. PDSCH should start after PDSCH DRMS symbol */
   pagePdschCfg->pdschTimeAlloc.startSymb = 3; /* spec-38.214, Table 5.1.2.1-1 */
   pagePdschCfg->pdschTimeAlloc.numSymb   = NUM_PDSCH_SYMBOL;

   /* Find total symbols occupied including DMRS */
   dmrsStartSymbol = findDmrsStartSymbol(pagePdschCfg->dmrs.dlDmrsSymbPos);
   /* If there are no DRMS symbols, findDmrsStartSymbol() returns MAX_SYMB_PER_SLOT,
    * in that case only PDSCH symbols are marked as occupied */
   if(dmrsStartSymbol == MAX_SYMB_PER_SLOT)
   {
      startSymbol = pagePdschCfg->pdschTimeAlloc.startSymb;
      numSymbol = pagePdschCfg->pdschTimeAlloc.numSymb;
   }
   /* If DMRS symbol is found, mark DMRS and PDSCH symbols as occupied */
   else
   {
      startSymbol = dmrsStartSymbol;
      numSymbol = pagePdschCfg->dmrs.nrOfDmrsSymbols + pagePdschCfg->pdschTimeAlloc.numSymb;
   }

   /* Allocate the number of PRBs required for DL PDSCH */
   if((allocatePrbDl(cell, slotTime, startSymbol, numSymbol,\
               &pagePdschCfg->pdschFreqAlloc.startPrb, pagePdschCfg->pdschFreqAlloc.numPrb)) != ROK)
   {
      DU_LOG("\nERROR  --> SCH : allocatePrbDl() failed for DL MSG");
      return RFAILED;
   }

   pagePdschCfg->beamPdschInfo.numPrgs              = 1;
   pagePdschCfg->beamPdschInfo.prgSize              = 1;
   pagePdschCfg->beamPdschInfo.digBfInterfaces      = 0;
   pagePdschCfg->beamPdschInfo.prg[0].pmIdx         = 0;
   pagePdschCfg->beamPdschInfo.prg[0].beamIdx[0]    = 0;
   pagePdschCfg->txPdschPower.powerControlOffset    = 0;
   pagePdschCfg->txPdschPower.powerControlOffsetSS  = 0;

   return ROK;
}

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
   uint16_t             dciSlot = 0;
   SlotTimingInfo       dciTime, msg3Time;
   SchCellCb            *cell = NULLP;
   SlotTimingInfo       currTime;
   DciInfo  *dciInfo = NULLP;
   cell = raCb->cell;
   currTime = cell->slotInfo;

   /* Calculating time frame to send DCI for MSG3 Retx*/
   ADD_DELTA_TO_TIME(currTime, dciTime, PHY_DELTA_DL + SCHED_DELTA, cell->numSlots);
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

      numK2 = cell->k2InfoTbl.k2TimingInfo[dlTime].numK2;
      msg3K2InfoTbl = &cell->msg3K2InfoTbl;
      msg3MinSchTime = 0;
      msg3Delta = 0;
   }
   else
   {
      numK2 = cell->msg3K2InfoTbl.k2TimingInfo[dlTime].numK2;
      msg3K2InfoTbl = &cell->k2InfoTbl;
      msg3MinSchTime = minMsg3SchTime[cell->cellCfg.numerology];
      msg3Delta = puschDeltaTable[puschMu];
   }

   for(k2TblIdx = 0; k2TblIdx < numK2; k2TblIdx++)
   {
      k2Index = msg3K2InfoTbl->k2TimingInfo[dlTime].k2Indexes[k2TblIdx];

      k2 = cell->cellCfg.ulCfgCommon.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[k2Index].k2;
      if (isRetx)
      {
         if ((msg3HqProc->strtSymbl != cell->cellCfg.ulCfgCommon.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[k2Index].startSymbol) ||
            (msg3HqProc->numSymbl != cell->cellCfg.ulCfgCommon.schInitialUlBwp.puschCommon.timeDomRsrcAllocList[k2Index].symbolLength))
         {
            continue;
         }
      }
      /* Delta is added to the slot allocation for msg3 based on 38.214 section 6.1.2.1 */
      k2 = k2 + msg3Delta;
      if(k2 >= msg3MinSchTime)
      {
         ADD_DELTA_TO_TIME(currTime, msg3TempTime, k2, cell->numSlots);
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

