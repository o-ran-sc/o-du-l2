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

extern SchCb schCb[SCH_MAX_INST];
extern uint8_t puschDeltaTable[MAX_MU_PUSCH];

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
	uint8_t ulCarrierIdx = 0; /* configured to 0 */
   raRnti = (1+symbolIdx+(14*slotIdx)+(14*80*freqIdx)+(14*80*8*ulCarrierIdx));
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
	schCb[schInst].cells[schInst]->raCb[0].tcrnti = tcrnti;
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
uint8_t schAllocMsg3Pusch(Inst schInst, uint16_t slot, uint16_t *msg3StartRb,
uint8_t *msg3NumRb)
{
	SchCellCb      *cell         = NULLP;
	SchUlSlotInfo  *schUlSlotInfo    = NULLP;
	uint8_t    puschMu       = 0;
	uint8_t    msg3SlotAlloc = 0;
	uint8_t    delta         = 0;
	uint8_t    k2            = 0; 
	uint8_t    startSymb     = 0;
	uint8_t    symbLen       = 0; 
	uint8_t    startRb       = 0;
	uint8_t    numRb         = 0;
	uint8_t    idx           = 0;
	uint8_t    mcs            = 4;
	uint8_t    numPdschSymbols= 14;
	uint16_t   tbSize         = 0;


   cell = schCb[schInst].cells[schInst];
//	puschMu = cell->cellCfg.puschMu;
	delta = puschDeltaTable[puschMu];
	k2 = cell->cellCfg.schInitialUlBwp.puschCommon.k2;
	startSymb = cell->cellCfg.schInitialUlBwp.puschCommon.startSymbol;
	symbLen = cell->cellCfg.schInitialUlBwp.puschCommon.lengthSymbol;

	/* Slot allocation for msg3 based on 38.214 section 6.1.2.1 */
	msg3SlotAlloc = slot + k2 + delta;
	msg3SlotAlloc = msg3SlotAlloc % SCH_NUM_SLOTS; 

        startRb = cell->schUlSlotInfo[msg3SlotAlloc]->puschCurrentPrb;
        tbSize = schCalcTbSize(8); /* 6 bytes msg3  and 2 bytes header */
	numRb = schCalcNumPrb(tbSize, mcs, numPdschSymbols);

	/* allocating 1 extra RB for now */
	numRb++;
	/* increment PUSCH PRB */
	cell->schUlSlotInfo[msg3SlotAlloc]->puschCurrentPrb += numRb;

	for(idx=startSymb; idx<symbLen; idx++)
	{
		cell->schUlSlotInfo[msg3SlotAlloc]->assignedPrb[idx] = startRb + numRb;
	}
	schUlSlotInfo = cell->schUlSlotInfo[msg3SlotAlloc];

   SCH_ALLOC(schUlSlotInfo->schPuschInfo, sizeof(SchPuschInfo));
	if(!schUlSlotInfo->schPuschInfo)
	{
      DU_LOG("SCH: Memory allocation failed in schAllocMsg3Pusch");
		return RFAILED;
	}
	schUlSlotInfo->schPuschInfo->harqProcId        = SCH_HARQ_PROC_ID;
	schUlSlotInfo->schPuschInfo->resAllocType      = SCH_ALLOC_TYPE_1;
	schUlSlotInfo->schPuschInfo->fdAlloc.startPrb  = startRb;
	schUlSlotInfo->schPuschInfo->fdAlloc.numPrb    = numRb;
	schUlSlotInfo->schPuschInfo->tdAlloc.startSymb = startSymb;
	schUlSlotInfo->schPuschInfo->tdAlloc.numSymb   = symbLen;
	schUlSlotInfo->schPuschInfo->tbInfo.mcs	       = mcs;
	schUlSlotInfo->schPuschInfo->tbInfo.ndi        = 1; /* new transmission */
	schUlSlotInfo->schPuschInfo->tbInfo.rv	        = 0;
	schUlSlotInfo->schPuschInfo->tbInfo.tbSize     = 24; /*Considering 2 PRBs */
	schUlSlotInfo->schPuschInfo->dmrsMappingType   = DMRS_MAP_TYPE_A;  /* Setting Type-A */
	schUlSlotInfo->schPuschInfo->nrOfDmrsSymbols   = NUM_DMRS_SYMBOLS;
	schUlSlotInfo->schPuschInfo->dmrsAddPos        = DMRS_ADDITIONAL_POS;

	*msg3StartRb = startRb;
	*msg3NumRb   = numRb;

	return ROK;
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
	RarInfo *rarInfo = NULLP;
	uint16_t raRnti = 0;
	uint16_t rarSlot = 0;
	uint16_t msg3StartRb;
	uint8_t  msg3NumRb;
   uint8_t  ret = ROK;

   /* RAR will sent with a delay of RAR_DELAY */
   rarSlot = (rachInd->timingInfo.slot+RAR_DELAY+PHY_DELTA)%SCH_NUM_SLOTS;

   SchDlSlotInfo *schDlSlotInfo = cell->schDlSlotInfo[rarSlot]; /* RAR will sent in the next slot */

   /* Allocate the rarInfo, this pointer will be checked at schProcessSlotInd function */
	SCH_ALLOC(rarInfo, sizeof(RarInfo));
	if(rarInfo == NULLP)
	{
      DU_LOG("\nMAC: Memory Allocation failed for rarInfo");
      return RFAILED;
	}

	schDlSlotInfo->rarInfo = rarInfo;
   
   /* calculate the ra-rnti value */
	raRnti = calculateRaRnti(rachInd->symbolIdx,rachInd->slotIdx,rachInd->freqIdx);
   
	/* create raCb at SCH */
	createSchRaCb(rachInd->crnti,schInst);

	/* allocate resources for msg3 */
	ret = schAllocMsg3Pusch(schInst, rarSlot, &msg3StartRb, &msg3NumRb);
	if(ret == ROK)
	{
		/* fill RAR info */
		rarInfo->raRnti                 = raRnti;
		rarInfo->tcrnti                 = rachInd->crnti;
		rarInfo->RAPID                  = rachInd->preambleIdx;
		rarInfo->ta                     = rachInd->timingAdv;
		rarInfo->msg3FreqAlloc.startPrb = msg3StartRb;
		rarInfo->msg3FreqAlloc.numPrb   = msg3NumRb;
	}
   return ret;
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
uint8_t schFillRar(RarAlloc *rarAlloc, uint16_t raRnti, uint16_t pci, uint8_t offsetPointA)
{
   Inst inst = 0;
   uint8_t coreset0Idx = 0;
   uint8_t numRbs = 0;
	uint8_t firstSymbol = 0;
   uint8_t numSymbols = 0;
   uint8_t offset = 0;
   uint8_t FreqDomainResource[6] = {0};
   uint16_t tbSize = 0;
	uint8_t numPdschSymbols = 12; /* considering pdsch region from 2 to 13 */
   uint8_t mcs = 4;  /* MCS fixed to 4 */

   SchBwpDlCfg *initialBwp = &schCb[inst].cells[inst]->cellCfg.schInitialDlBwp;

	PdcchCfg *pdcch = &rarAlloc->rarPdcchCfg;
	PdschCfg *pdsch = &rarAlloc->rarPdschCfg;
   BwpCfg *bwp = &rarAlloc->bwp;

   coreset0Idx     = initialBwp->pdcchCommon.commonSearchSpace.coresetId;

   /* derive the sib1 coreset0 params from table 13-1 spec 38.213 */
   numRbs        = coresetIdxTable[coreset0Idx][1];
   numSymbols    = coresetIdxTable[coreset0Idx][2];
   offset        = coresetIdxTable[coreset0Idx][3];

   /* calculate time domain parameters */
	// note: since slot value is made sl1, RAR can be sent at all slots
	uint16_t mask = 0x2000;
	for(firstSymbol=0; firstSymbol<14;firstSymbol++)
	{
	   if(initialBwp->pdcchCommon.commonSearchSpace.monitoringSymbol & mask)
		   break;
		else
		   mask = mask>>1;
   }

   /* calculate the PRBs */
   schAllocFreqDomRscType0(((offsetPointA-offset)/6), (numRbs/6), FreqDomainResource);

   /* fill BWP */
   bwp->freqAlloc.numPrb   = initialBwp->bwp.freqAlloc.numPrb;
   bwp->freqAlloc.startPrb = initialBwp->bwp.freqAlloc.startPrb;
   bwp->subcarrierSpacing  = initialBwp->bwp.scs;
   bwp->cyclicPrefix       = initialBwp->bwp.cyclicPrefix;

   /* fill the PDCCH PDU */
   pdcch->coreset0Cfg.startSymbolIndex = firstSymbol;
   pdcch->coreset0Cfg.durationSymbols = numSymbols;
   memcpy(pdcch->coreset0Cfg.freqDomainResource,FreqDomainResource,6);
   pdcch->coreset0Cfg.cceRegMappingType = 1; /* coreset0 is always interleaved */
   pdcch->coreset0Cfg.regBundleSize = 6;    /* spec-38.211 sec 7.3.2.2 */
   pdcch->coreset0Cfg.interleaverSize = 2;  /* spec-38.211 sec 7.3.2.2 */
   pdcch->coreset0Cfg.coreSetType = 0;
   pdcch->coreset0Cfg.shiftIndex = pci;
   pdcch->coreset0Cfg.precoderGranularity = 0; /* sameAsRegBundle */
   pdcch->numDlDci = 1;
   pdcch->dci.rnti = raRnti; /* RA-RNTI */
   pdcch->dci.scramblingId = pci;
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
   pdsch->rnti = raRnti; /* RA-RNTI */
   pdsch->pduIndex = 0;
   pdsch->numCodewords = 1;
	for(cwCount = 0; cwCount < pdsch->numCodewords; cwCount++)
	{
      pdsch->codeword[cwCount].targetCodeRate = 308;
      pdsch->codeword[cwCount].qamModOrder = 2;
      pdsch->codeword[cwCount].mcsIndex = mcs; /* mcs configured to 4 */
      pdsch->codeword[cwCount].mcsTable = 0;   /* notqam256 */
      pdsch->codeword[cwCount].rvIndex = 0;
		tbSize = schCalcTbSize(10); /* 8 bytes RAR and 2 bytes padding */
      pdsch->codeword[cwCount].tbSize = tbSize;
   }
   pdsch->dataScramblingId = pci;
   pdsch->numLayers = 1;
   pdsch->transmissionScheme = 0;
   pdsch->refPoint = 0;
   pdsch->dmrs.dlDmrsSymbPos = 2;
   pdsch->dmrs.dmrsConfigType = 0; /* type-1 */
   pdsch->dmrs.dlDmrsScramblingId = pci;
   pdsch->dmrs.scid = 0;
   pdsch->dmrs.numDmrsCdmGrpsNoData = 1;
   pdsch->dmrs.dmrsPorts = 0;
	pdsch->dmrs.mappingType      = DMRS_MAP_TYPE_A;  /* Type-A */
	pdsch->dmrs.nrOfDmrsSymbols  = NUM_DMRS_SYMBOLS;
	pdsch->dmrs.dmrsAddPos       = DMRS_ADDITIONAL_POS;
   pdsch->pdschFreqAlloc.resourceAllocType = 1; /* RAT type-1 RIV format */
   pdsch->pdschFreqAlloc.freqAlloc.startPrb = offset + SCH_SSB_NUM_PRB; /* the RB numbering starts from coreset0, and PDSCH is always above SSB */ 
   pdsch->pdschFreqAlloc.freqAlloc.numPrb = schCalcNumPrb(tbSize,mcs,numPdschSymbols);
   pdsch->pdschFreqAlloc.vrbPrbMapping = 0; /* non-interleaved */
   pdsch->pdschTimeAlloc.timeAlloc.startSymb = initialBwp->pdschCommon.startSymbol;
   pdsch->pdschTimeAlloc.timeAlloc.numSymb = initialBwp->pdschCommon.lengthSymbol;
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
