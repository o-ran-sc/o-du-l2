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

SchCb schCb[SCH_MAX_INST];
uint16_t prachCfgIdxTable[MAX_PRACH_CONFIG_IDX][8];
uint16_t numRbForPrachTable[MAX_RACH_NUM_RB_IDX][5];
uint8_t pucchResourceSet[MAX_PUCCH_RES_SET_IDX][4];

SchMacUlSchInfoFunc schMacUlSchInfoOpts[] =
{
   packSchMacUlSchInfo,
   MacProcUlSchInfo,
   packSchMacUlSchInfo
};

/**
 * @brief common resource allocation for SSB
 *
 * @details
 *
 *     Function : schBroadcastAlloc
 *     
 *     This function handles common scheduling for DL
 *     
 *  @param[in]  SchCellCb *cell, cell cb
 *  @param[in]  DlBrdcstAlloc *dlBrdcstAlloc, DL brdcst allocation
 *  @return  void
 **/
uint8_t schBroadcastAlloc(SchCellCb *cell, DlBrdcstAlloc *dlBrdcstAlloc,
      uint16_t slot)
{
   /* schedule SSB */
   uint8_t ssbStartPrb, ssbStartSymb, idx;
   SchDlSlotInfo *schDlSlotInfo;
   SsbInfo ssbInfo;

   schDlSlotInfo = cell->schDlSlotInfo[slot];
   if(dlBrdcstAlloc->ssbTrans)
   {
      ssbStartPrb = cell->cellCfg.ssbSchCfg.ssbOffsetPointA; //+Kssb
      ssbStartSymb = cell->ssbStartSymbArr[dlBrdcstAlloc->ssbIdxSupported-1]; /*since we are
										supporting only 1 ssb beam */

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

      schDlSlotInfo->ssbPres = true;
      schDlSlotInfo->ssbIdxSupported = dlBrdcstAlloc->ssbIdxSupported;
      for(idx=ssbStartSymb; idx<ssbStartSymb+SCH_SSB_NUM_SYMB; idx++)
      {
	 schDlSlotInfo->assignedPrb[idx] = ssbStartPrb + SCH_SSB_NUM_PRB + 1; /* +1 for kSsb */
      }
   }

   /* SIB1 allocation */
   if(dlBrdcstAlloc->sib1Trans)
   {
      uint16_t tbSize         = 0;
      uint8_t numPdschSymbols = 12; /* considering pdsch region from 2 to 13 */
      uint8_t mcs             = 4;  /* MCS fixed to 4 */
      uint8_t numSib1Prb      = 0;
      schDlSlotInfo->sib1Pres = true;

      tbSize = schCalcTbSize(cell->cellCfg.sib1SchCfg.sib1PduLen); /* send this value to the func in bytes when considering sib1 size */
      numSib1Prb = schCalcNumPrb(tbSize,mcs,numPdschSymbols);
      for(idx=0; idx<SCH_SYMBOL_PER_SLOT; idx++)
      {
	 schDlSlotInfo->assignedPrb[idx] = ssbStartPrb + SCH_SSB_NUM_PRB + 1 + numSib1Prb; /* 10 PRBs for sib1 */
      }
      memcpy(&dlBrdcstAlloc->sib1Alloc.bwp, &cell->cellCfg.sib1SchCfg.bwp, sizeof(BwpCfg)); 
      memcpy(&dlBrdcstAlloc->sib1Alloc.sib1PdcchCfg, &cell->cellCfg.sib1SchCfg.sib1PdcchCfg, sizeof(PdcchCfg)); 
      memcpy(&dlBrdcstAlloc->sib1Alloc.sib1PdschCfg, &cell->cellCfg.sib1SchCfg.sib1PdschCfg, sizeof(PdschCfg)); 
      dlBrdcstAlloc->sib1Alloc.sib1PdcchCfg.dci.pdschCfg = &dlBrdcstAlloc->sib1Alloc.sib1PdschCfg;
   }
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

   return(*schMacUlSchInfoOpts[pst.selector])(&pst, ulSchedInfo);
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
void schPrachResAlloc(SchCellCb *cell, UlSchedInfo *ulSchedInfo, SlotIndInfo prachOccasionTimingInfo)
{
   uint8_t  puschScs;
   uint8_t  numPrachRb = 0;
   uint8_t  numRa = 0;
   uint8_t  freqStart = 0;
   uint8_t  prachCfgIdx = 0;
   uint8_t  prachFormat = 0;
   uint8_t  x = 0;
   uint8_t  y = 0;
   uint16_t prachSubframe = 0;
   uint8_t  prachStartSymbol = 0;
   uint8_t  prachOcas = 0;
   uint8_t  dataType = 0;
   uint8_t  idx = 0;
   SchUlSlotInfo *schUlSlotInfo = NULLP;

   puschScs      = cell->cellCfg.schInitialUlBwp.bwp.scs;
   schUlSlotInfo = cell->schUlSlotInfo[prachOccasionTimingInfo.slot];
   prachCfgIdx   = cell->cellCfg.schRachCfg.prachCfgIdx;

   /* derive the prachCfgIdx table paramters */
   x                = prachCfgIdxTable[prachCfgIdx][1];
   y                = prachCfgIdxTable[prachCfgIdx][2];
   prachSubframe    = prachCfgIdxTable[prachCfgIdx][3];

   if((prachOccasionTimingInfo.sfn%x) == y)
   {
      /* check for subFrame number */
      if ((1 << prachOccasionTimingInfo.slot) & prachSubframe)
      {
	 /* prach ocassion present in this subframe */

	 prachFormat      = prachCfgIdxTable[prachCfgIdx][0];
	 prachStartSymbol = prachCfgIdxTable[prachCfgIdx][4];
	 prachOcas        = prachCfgIdxTable[prachCfgIdx][6];

	 /* freq domain resource determination for RACH*/
	 freqStart = cell->cellCfg.schRachCfg.msg1FreqStart;
	 /* numRa determined as 𝑛 belonging {0,1,.., M − 1}, 
	  * where M is given by msg1Fdm */
	 numRa = (cell->cellCfg.schRachCfg.msg1Fdm - 1);
	 for(idx=0; idx<MAX_RACH_NUM_RB_IDX; idx++)
	 {
	    if(numRbForPrachTable[idx][0] == cell->cellCfg.schRachCfg.rootSeqLen)
	    {
	       if(numRbForPrachTable[idx][1] == cell->cellCfg.schRachCfg.prachSubcSpacing)
	       {
		  if(numRbForPrachTable[idx][2] == puschScs)
		  {
		     break;
		  }
	       }
	    }
	 }
	 numPrachRb = numRbForPrachTable[idx][3];
	 dataType |= SCH_DATATYPE_PRACH;
	 /* Considering first slot in the frame for PRACH */
	 idx = 0;
	 schUlSlotInfo->assignedPrb[idx] = freqStart+numPrachRb;
      }
      ulSchedInfo->dataType = dataType;
      /* prach info */
      ulSchedInfo->prachSchInfo.numPrachOcas   = prachOcas;
      ulSchedInfo->prachSchInfo.prachFormat    = prachFormat;
      ulSchedInfo->prachSchInfo.numRa          = numRa;
      ulSchedInfo->prachSchInfo.prachStartSymb = prachStartSymbol;
   }
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
      ulSchedPucch->fdAlloc.numPrb = PUCCH_NUM_PRB_FORMAT_0_1_4;
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
      ulSchedPucch->fdAlloc.numPrb = PUCCH_NUM_PRB_FORMAT_0_1_4;
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
               memcpy(&ulSchedPucch->cmnFormatCfg, formatCfg, sizeof(SchPucchFormatCfg));
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

uint8_t fillUlSchedPucchDedicatedCfg(uint16_t numSlots, SchPucchCfg *pucchDedCfg,\
   SlotIndInfo *slotInfo, SchPucchInfo *ulSchedPucch)
{
   uint8_t ret, resrcSetIdx, resrcIdx, schedReqIdx, srPeriodicity = 0;
   uint16_t srOffset = 0;

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
               ulSchedPucch->fdAlloc.startPrb = pucchDedCfg->resrc->resrcToAddModList[resrcIdx].startPrb;
	       ulSchedPucch->pucchFormat = pucchDedCfg->resrc->resrcToAddModList[resrcIdx].pucchFormat;
	       ret = fillUlSchedPucchFormat(ulSchedPucch->pucchFormat, ulSchedPucch,\
	                &pucchDedCfg->resrc->resrcToAddModList[resrcIdx], NULLP);
	       if(ret == RFAILED)
	          return ret;
  	    }
         }
      }
   }
   if(pucchDedCfg->format1)
   {
      memset(&ulSchedPucch->cmnFormatCfg, 0, sizeof(SchPucchFormatCfg));
      ret = fillUlSchedPucchFormat(ulSchedPucch->pucchFormat, ulSchedPucch, NULLP, pucchDedCfg->format1);
      if(ret == RFAILED)
         return ret;
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
         ulSchedPucch->uciFlag = true;
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

uint16_t fillPucchResourceInfo(SchPucchInfo *schPucchInfo, Inst inst)
{
   uint8_t ret = ROK, ueIdx = 0, pucchIdx = 0;
   SchCellCb  *cell = schCb[inst].cells[inst];
   SchPucchCfgCmn *pucchCfg = NULLP;
   SchBwpParams *ulBwp = NULLP;

   GET_UE_IDX(schPucchInfo->rnti, ueIdx);
   if(cell->ueCb[ueIdx].ueCfg.spCellCfg.servCellCfg.initUlBwp.pucchCfgPres)
   {
      /* fill pucch dedicated cfg */
      ret = fillUlSchedPucchDedicatedCfg(cell->numSlots,\
       &cell->ueCb[ueIdx].ueCfg.spCellCfg.servCellCfg.initUlBwp.pucchCfg, &cell->slotInfo, schPucchInfo);
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
      pucchCfg = &cell->cellCfg.schInitialUlBwp.pucchCommon;
      pucchIdx = pucchCfg->pucchResourceCommon;
      ulBwp = &cell->cellCfg.schInitialUlBwp.bwp;
      schPucchInfo->fdAlloc.startPrb = ulBwp->freqAlloc.startPrb + pucchResourceSet[pucchIdx][3];
      schPucchInfo->fdAlloc.numPrb = PUCCH_NUM_PRB_FORMAT_0_1_4;
      schPucchInfo->tdAlloc.startSymb = pucchResourceSet[pucchIdx][1];
      schPucchInfo->tdAlloc.numSymb = pucchResourceSet[pucchIdx][2];
      schPucchInfo->pucchFormat = pucchResourceSet[pucchIdx][0];

      /* set SR and UCI flag to false */
      schPucchInfo->srFlag  = true;
      schPucchInfo->uciFlag = true;
   }
   /* set HARQ flag to true */
   schPucchInfo->harqFlag = true;
   schPucchInfo->numHarqBits = 1; /* 1 bit for HARQ */

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
   UlSchedInfo ulSchedInfo;
   SchUlSlotInfo  *schUlSlotInfo = NULLP;
   SlotIndInfo ulTimingInfo;
   memset(&ulSchedInfo, 0, sizeof(UlSchedInfo));

   /* add PHY delta */
   ADD_DELTA_TO_TIME(cell->slotInfo,ulTimingInfo,PHY_DELTA+SCHED_DELTA);

   ulSchedInfo.cellId = cell->cellId;
   ulSchedInfo.slotIndInfo.cellId = ulSchedInfo.cellId;
   ulSchedInfo.slotIndInfo.sfn = ulTimingInfo.sfn;
   ulSchedInfo.slotIndInfo.slot = ulTimingInfo.slot;

   /* Schedule resources for PRACH */
   schPrachResAlloc(cell, &ulSchedInfo, ulTimingInfo);

   schUlSlotInfo = cell->schUlSlotInfo[ulTimingInfo.slot]; 
   if(schUlSlotInfo->schPuschInfo)
   {
      ulSchedInfo.crnti = schUlSlotInfo->schPuschInfo->crnti;
      ulSchedInfo.dataType |= SCH_DATATYPE_PUSCH;
      memcpy(&ulSchedInfo.schPuschInfo, schUlSlotInfo->schPuschInfo,
	    sizeof(SchPuschInfo));
      SCH_FREE(schUlSlotInfo->schPuschInfo, sizeof(SchPuschInfo));
      schUlSlotInfo->schPuschInfo = NULL;
   }

   if(schUlSlotInfo->pucchPres)
   {
      ulSchedInfo.dataType |= SCH_DATATYPE_UCI;
      fillPucchResourceInfo(&schUlSlotInfo->schPucchInfo, schInst);
      memcpy(&ulSchedInfo.schPucchInfo, &schUlSlotInfo->schPucchInfo,
	    sizeof(SchPucchInfo));
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
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t schDlRsrcAllocMsg4(DlMsgAlloc *msg4Alloc, SchCellCb *cell, uint16_t slot)
{
   uint8_t coreset0Idx = 0;
   uint8_t numRbs = 0;
   uint8_t firstSymbol = 0;
   uint8_t numSymbols = 0;
   uint8_t offset = 0;
   uint8_t offsetPointA;
   uint8_t FreqDomainResource[6] = {0};
   uint16_t tbSize = 0;
   uint8_t numPdschSymbols = 12; /* considering pdsch region from 2 to 13 */
   uint8_t mcs = 4;  /* MCS fixed to 4 */
   SchBwpDlCfg *initialBwp;

   PdcchCfg *pdcch = &msg4Alloc->dlMsgPdcchCfg;
   PdschCfg *pdsch = &msg4Alloc->dlMsgPdschCfg;
   BwpCfg *bwp = &msg4Alloc->bwp;

   initialBwp   = &cell->cellCfg.schInitialDlBwp;
   offsetPointA = cell->cellCfg.ssbSchCfg.ssbOffsetPointA;
   coreset0Idx  = initialBwp->pdcchCommon.commonSearchSpace.coresetId;

   /* derive the sib1 coreset0 params from table 13-1 spec 38.213 */
   numRbs        = coresetIdxTable[coreset0Idx][1];
   numSymbols    = coresetIdxTable[coreset0Idx][2];
   offset        = coresetIdxTable[coreset0Idx][3];

   /* calculate time domain parameters */
   uint16_t mask = 0x2000;
   for(firstSymbol=0; firstSymbol<14;firstSymbol++)
   {
      if(initialBwp->pdcchCommon.commonSearchSpace.monitoringSymbol & mask)
	 break;
      else
	 mask = mask>>1;
   }

   /* calculate the PRBs */
   freqDomRscAllocType0(((offsetPointA-offset)/6), (numRbs/6), FreqDomainResource);

   /* fill BWP */
   bwp->freqAlloc.numPrb   = initialBwp->bwp.freqAlloc.numPrb;
   bwp->freqAlloc.startPrb = initialBwp->bwp.freqAlloc.startPrb;
   bwp->subcarrierSpacing  = initialBwp->bwp.scs;
   bwp->cyclicPrefix       = initialBwp->bwp.cyclicPrefix;

   /* fill the PDCCH PDU */
   pdcch->coresetCfg.startSymbolIndex = firstSymbol;
   pdcch->coresetCfg.durationSymbols = numSymbols;
   memcpy(pdcch->coresetCfg.freqDomainResource,FreqDomainResource,6);
   pdcch->coresetCfg.cceRegMappingType = 1; /* coreset0 is always interleaved */
   pdcch->coresetCfg.regBundleSize = 6;    /* spec-38.211 sec 7.3.2.2 */
   pdcch->coresetCfg.interleaverSize = 2;  /* spec-38.211 sec 7.3.2.2 */
   pdcch->coresetCfg.coreSetType = 0;
   pdcch->coresetCfg.coreSetSize = numRbs;
   pdcch->coresetCfg.shiftIndex = cell->cellCfg.phyCellId;
   pdcch->coresetCfg.precoderGranularity = 0; /* sameAsRegBundle */
   pdcch->numDlDci = 1;
   pdcch->dci.rnti = cell->schDlSlotInfo[slot]->dlMsgInfo->crnti;
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

   /* fill the PDSCH PDU */
   uint8_t cwCount = 0;
   pdsch->pduBitmap = 0; /* PTRS and CBG params are excluded */
   pdsch->rnti = cell->schDlSlotInfo[slot]->dlMsgInfo->crnti;
   pdsch->pduIndex = 0;
   pdsch->numCodewords = 1;
   for(cwCount = 0; cwCount < pdsch->numCodewords; cwCount++)
   {
      pdsch->codeword[cwCount].targetCodeRate = 308;
      pdsch->codeword[cwCount].qamModOrder = 2;
      pdsch->codeword[cwCount].mcsIndex = mcs; /* mcs configured to 4 */
      pdsch->codeword[cwCount].mcsTable = 0; /* notqam256 */
      pdsch->codeword[cwCount].rvIndex = 0;
      /* 38.214: Table 5.1.3.2-1,  divided by 8 to get the value in bytes */
      /* TODO : Calculate tbSize based of DL CCCH msg size */
      tbSize = schCalcTbSize(2664/8); /* send this value to the func in bytes when considering msg4 size */
      pdsch->codeword[cwCount].tbSize = tbSize;
   }
   pdsch->dataScramblingId = cell->cellCfg.phyCellId;
   pdsch->numLayers = 1;
   pdsch->transmissionScheme = 0;
   pdsch->refPoint = 0;
   pdsch->dmrs.dlDmrsSymbPos = 2;
   pdsch->dmrs.dmrsConfigType = 0; /* type-1 */
   pdsch->dmrs.dlDmrsScramblingId = cell->cellCfg.phyCellId;
   pdsch->dmrs.scid = 0;
   pdsch->dmrs.numDmrsCdmGrpsNoData = 1;
   pdsch->dmrs.dmrsPorts = 0;
   pdsch->dmrs.mappingType      = DMRS_MAP_TYPE_A; /* Setting to Type-A */
   pdsch->dmrs.nrOfDmrsSymbols  = NUM_DMRS_SYMBOLS;
   pdsch->dmrs.dmrsAddPos       = DMRS_ADDITIONAL_POS;
   pdsch->pdschFreqAlloc.resourceAllocType = 1; /* RAT type-1 RIV format */
   /* the RB numbering starts from coreset0, and PDSCH is always above SSB */
   pdsch->pdschFreqAlloc.freqAlloc.startPrb = offset + SCH_SSB_NUM_PRB;
   pdsch->pdschFreqAlloc.freqAlloc.numPrb = schCalcNumPrb(tbSize,mcs,numPdschSymbols);
   pdsch->pdschFreqAlloc.vrbPrbMapping = 0; /* non-interleaved */
   pdsch->pdschTimeAlloc.timeAlloc.startSymb = 2; /* spec-38.214, Table 5.1.2.1-1 */
   pdsch->pdschTimeAlloc.timeAlloc.numSymb = 12;
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
 
uint16_t schAllocPucchResource(SchCellCb *cell, uint16_t crnti, uint16_t slot)
{
   uint8_t k1 = SCH_DEFAULT_K1, ueIdx = 0, dlToUlAckIdx;
   uint16_t pucchSlot = 0;
   SchUlSlotInfo  *schUlSlotInfo = NULLP;
   SchPucchCfg    *schPucchCfg = NULLP;

   GET_UE_IDX(crnti, ueIdx);
   if(cell->ueCb[ueIdx].ueCfg.spCellCfg.servCellCfg.initUlBwp.pucchCfgPres)
   {
      schPucchCfg = &(cell->ueCb[ueIdx].ueCfg.spCellCfg.servCellCfg.initUlBwp.pucchCfg);
     if(schPucchCfg->dlDataToUlAck)
     {
        for(dlToUlAckIdx = 0; dlToUlAckIdx < schPucchCfg->dlDataToUlAck->dlDataToUlAckListCount; dlToUlAckIdx++)
	{
           //For now considering only the first value in the list
           k1 = schPucchCfg->dlDataToUlAck->dlDataToUlAckList[dlToUlAckIdx];
	   break;
	}
     }
   }
   
   pucchSlot = (slot + k1)  % cell->numSlots;
   schUlSlotInfo = cell->schUlSlotInfo[pucchSlot];
   memset(&schUlSlotInfo->schPucchInfo, 0, sizeof(SchPucchInfo));

   schUlSlotInfo->pucchPres = true;
   schUlSlotInfo->schPucchInfo.rnti = crnti;

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
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t schDlRsrcAllocDlMsg(DlMsgAlloc *dlMsgAlloc, SchCellCb *cell, uint16_t crnti,
      uint16_t accumalatedSize, uint16_t slot)
{
   uint8_t ueIdx;
   uint16_t tbSize = 0;
   uint8_t numPdschSymbols = 12; /* considering pdsch region from 2 to 13 */
   PdcchCfg *pdcch = NULLP;
   PdschCfg *pdsch = NULLP;
   BwpCfg *bwp = NULLP;
   SchUeCb ueCb;
   SchControlRsrcSet coreset1;
   SchPdschConfig pdschCfg;

   pdcch = &dlMsgAlloc->dlMsgPdcchCfg;
   pdsch = &dlMsgAlloc->dlMsgPdschCfg;
   bwp = &dlMsgAlloc->bwp;

   GET_UE_IDX(crnti, ueIdx);
   ueCb  = cell->ueCb[ueIdx-1];
   coreset1 = ueCb.ueCfg.spCellCfg.servCellCfg.initDlBwp.pdcchCfg.cRSetToAddModList[0];
   pdschCfg = ueCb.ueCfg.spCellCfg.servCellCfg.initDlBwp.pdschCfg;

   /* fill BWP */
   bwp->freqAlloc.numPrb = MAX_NUM_RB;
   bwp->freqAlloc.startPrb = 0;
   bwp->subcarrierSpacing = cell->cellCfg.sib1SchCfg.bwp.subcarrierSpacing;
   bwp->cyclicPrefix = cell->cellCfg.sib1SchCfg.bwp.cyclicPrefix;

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
   pdcch->dci.txPdcchPower.powerValue = 0;
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
      tbSize = schCalcTbSize(accumalatedSize);
      pdsch->codeword[cwCount].tbSize = tbSize;
   }
   pdsch->dataScramblingId = cell->cellCfg.phyCellId;
   pdsch->numLayers = 1;
   pdsch->transmissionScheme = 0;
   pdsch->refPoint = 0;
   pdsch->dmrs.dlDmrsSymbPos = 2;
   pdsch->dmrs.dmrsConfigType = 0; /* type-1 */
   pdsch->dmrs.dlDmrsScramblingId = cell->cellCfg.phyCellId;
   pdsch->dmrs.scid = 0;
   pdsch->dmrs.numDmrsCdmGrpsNoData = 1;
   pdsch->dmrs.dmrsPorts = 0;
   pdsch->dmrs.mappingType      = DMRS_MAP_TYPE_A; /* Setting to Type-A */
   pdsch->dmrs.nrOfDmrsSymbols  = NUM_DMRS_SYMBOLS;
   pdsch->dmrs.dmrsAddPos       = pdschCfg.dmrsDlCfgForPdschMapTypeA.addPos;
   pdsch->pdschFreqAlloc.resourceAllocType = 1; /* RAT type-1 RIV format */
   pdsch->pdschFreqAlloc.freqAlloc.startPrb = 1;
   pdsch->pdschFreqAlloc.freqAlloc.numPrb = schCalcNumPrb(tbSize, ueCb.ueCfg.dlModInfo.mcsIndex, numPdschSymbols);
   pdsch->pdschFreqAlloc.vrbPrbMapping = 0; /* non-interleaved */
   pdsch->pdschTimeAlloc.timeAlloc.startSymb = pdschCfg.timeDomRsrcAllociList[0].startSymbol;
   pdsch->pdschTimeAlloc.timeAlloc.numSymb = pdschCfg.timeDomRsrcAllociList[0].symbolLength;
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

/**********************************************************************
  End of file
 **********************************************************************/

