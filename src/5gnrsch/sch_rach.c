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

#include "stdbool.h"
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_lte.h"        /* Common LTE Defines */
#include "tfu.h"
#include "lrg.h"

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"       /* common memory link list library */
#include "cm_lte.x"        /* Common LTE Defines */
#include "tfu.x"
#include "lrg.x"
#include "du_log.h"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "sch.h"
#include "sch_utils.h"

extern SchCb schCb[SCH_MAX_INST];
extern int8_t coresetIdxTable[MAX_CORESET_INDEX][4];
extern int8_t searchSpaceIdxTable[MAX_SEARCH_SPACE_INDEX][4];
extern uint8_t deltaValueTable[MAX_MU_PUSCH];

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
void schAllocMsg3Pusch(Inst schInst, uint16_t slot, uint16_t *msg3StartRb,
uint8_t *msg3NumRb)
{
	SchCellCb  *cell;
	SchUlAlloc *ulAlloc;
	uint8_t    puschMu;
	uint8_t    msg3SlotAlloc;
	uint8_t    delta;
	uint8_t    k2; 
	uint8_t    startSymb;
	uint8_t    symbLen; 
	uint8_t    startRb;
	uint8_t    numRb;
	uint8_t    idx;


   cell = schCb[schInst].cells[schInst];
	puschMu = cell->cellCfg.puschMu;
	delta = deltaValueTable[puschMu];
	k2 = cell->cellCfg.schInitialUlBwp.puschCommon.k2;
	startSymb = cell->cellCfg.schInitialUlBwp.puschCommon.startSymbol;
	symbLen = cell->cellCfg.schInitialUlBwp.puschCommon.lengthSymbol;

	/* Slot allocation for msg3 based on 38.214 section 6.1.2.1 */
	msg3SlotAlloc = slot + k2 + delta;

	startRb = PUSCH_START_RB;

	/* formula used for calculation of rbSize, 38.214 section 6.1.4.2 
	 * Ninfo = S.Nre.R.Qm.v
	 * Nre'  = Nsc.NsymPdsch-NdmrsSymb-Noh
	 * Nre   = min(156,Nre').nPrb */
	numRb = 1; /* based on above calculation */

	/* allocating 1 extra RB for now */
	numRb++;

	for(idx=startSymb; idx<symbLen; idx++)
	{
		cell->ulAlloc[msg3SlotAlloc]->assignedPrb[idx] = startRb + numRb;
	}
	ulAlloc = cell->ulAlloc[msg3SlotAlloc];

	ulAlloc->puschPres = true;
	ulAlloc->schPuschInfo.harqProcId = SCH_HARQ_PROC_ID;
	ulAlloc->schPuschInfo.resAllocType = SCH_ALLOC_TYPE_1;
	ulAlloc->schPuschInfo.fdAlloc.startPrb	= startRb;
	ulAlloc->schPuschInfo.fdAlloc.numPrb = numRb;
	ulAlloc->schPuschInfo.tdAlloc.startSymb = startSymb;
	ulAlloc->schPuschInfo.tdAlloc.numSymb = symbLen;
	ulAlloc->schPuschInfo.tbInfo.mcs	= 4;
	ulAlloc->schPuschInfo.tbInfo.ndi = 1; /* new transmission */
	ulAlloc->schPuschInfo.tbInfo.rv	= 0;
	ulAlloc->schPuschInfo.tbInfo.tbSize = 6; /*RRC conn req is 6 bytes long */

	*msg3StartRb = startRb;
	*msg3NumRb   = numRb;
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
	uint16_t raRnti = 0;
	uint16_t slot;
	uint16_t msg3StartRb;
	uint8_t  msg3NumRb;

   /* RAR will sent in the next slot */
	slot = (rachInd->timingInfo.slot+SCHED_DELTA+RAR_DELAY)%SCH_NUM_SLOTS;

   SchDlAlloc *dlAlloc =  cell->dlAlloc[slot];
	RarInfo *rarInfo = &(dlAlloc->rarInfo);

   /* rar message presense in next slot ind and will be scheduled */
   dlAlloc->rarPres = true;

   /* calculate the ra-rnti value */
	raRnti = calculateRaRnti(rachInd->symbolIdx,rachInd->slotIdx,rachInd->freqIdx);
   
	/* create raCb at SCH */
	createSchRaCb(rachInd->crnti,schInst);

	/* allocate resources for msg3 */
	schAllocMsg3Pusch(schInst, slot, &msg3StartRb, &msg3NumRb);

	/* fill RAR info */
	rarInfo->raRnti      = raRnti;
	rarInfo->tcrnti      = rachInd->crnti;
	rarInfo->RAPID       = rachInd->preambleIdx;
	rarInfo->ta          = rachInd->timingAdv;
	rarInfo->msg3StartRb = msg3StartRb;
	rarInfo->msg3NumRb   = msg3NumRb;

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
uint8_t schFillRar(RarAlloc *rarAlloc, uint16_t raRnti, uint16_t pci, uint8_t offsetPointA)
{
   Inst inst = 0;
   uint8_t coreset0Idx = 0;
   uint8_t numRbs = 0;
	uint8_t firstSymbol = 0;
   uint8_t numSymbols = 0;
   uint8_t offset = 0;
   uint8_t FreqDomainResource[6] = {0};
   SchBwpDlCfg *initialBwp = &schCb[inst].cells[inst]->cellCfg.schInitialDlBwp;

	PdcchCfg *pdcch = &rarAlloc->rarPdcchCfg;
	PdschCfg *pdsch = &rarAlloc->rarPdschCfg;

   coreset0Idx     = initialBwp->pdcchCommon.raSearchSpace.coresetId;

   /* derive the sib1 coreset0 params from table 13-1 spec 38.213 */
   numRbs        = coresetIdxTable[coreset0Idx][1];
   numSymbols    = coresetIdxTable[coreset0Idx][2];
   offset        = coresetIdxTable[coreset0Idx][3];

   /* calculate time domain parameters */
	// note: since slot value is made sl1, RAR can be sent at all slots
	uint16_t mask = 0x2000;
	for(firstSymbol=0; firstSymbol<14;firstSymbol++)
	{
	   if(initialBwp->pdcchCommon.raSearchSpace.monitoringSymbol & mask)
		   break;
		else
		   mask = mask>>1;
   }

   /* calculate the PRBs */
   freqDomResourceAlloc( ((offsetPointA-offset)/6), (numRbs/6), FreqDomainResource);

   /* fill the PDCCH PDU */
   pdcch->pdcchBwpCfg.BWPSize = initialBwp->bwp.numPrb;
   pdcch->pdcchBwpCfg.BWPStart = initialBwp->bwp.firstPrb;
   pdcch->pdcchBwpCfg.subcarrierSpacing = initialBwp->bwp.scs;
   pdcch->pdcchBwpCfg.cyclicPrefix = initialBwp->bwp.cyclicPrefix;
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
   pdsch->pdschBwpCfg.BWPSize = initialBwp->bwp.numPrb;
   pdsch->pdschBwpCfg.BWPStart = initialBwp->bwp.firstPrb;
   pdsch->numCodewords = 1;
	for(cwCount = 0; cwCount < pdsch->numCodewords; cwCount++)
	{
      pdsch->codeword[cwCount].targetCodeRate = 308;
      pdsch->codeword[cwCount].qamModOrder = 2;
      pdsch->codeword[cwCount].mcsIndex = 4; /* mcs configured to 4 */
      pdsch->codeword[cwCount].mcsTable = 0; /* notqam256 */
      pdsch->codeword[cwCount].rvIndex = 0;
      pdsch->codeword[cwCount].tbSize = 80/8; /* 38.214: Table 5.1.3.2-1,
		   devided by 8 to get the value in bytes */
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
   pdsch->freqAlloc.resourceAlloc = 1; /* RAT type-1 RIV format */
   pdsch->freqAlloc.rbStart = offset + SCH_SSB_PRB_DURATION; /* the RB numbering starts from coreset0, and PDSCH is always above SSB */ 
	/* formula used for calculation of rbSize, 38.213 section 5.1.3.2 *
	 * Ninfo = S . Nre . R . Qm . v                                   *
	 * Nre' = Nsc . NsymPdsch - NdmrsSymb - Noh                       *
	 * Nre = min(156,Nre') . nPrb                                     */
   pdsch->freqAlloc.rbSize = 1; /* This value is calculated from above formulae */
   pdsch->freqAlloc.vrbPrbMapping = 0; /* non-interleaved */
   pdsch->timeAlloc.startSymbolIndex = initialBwp->pdschCommon.startSymbol;
   pdsch->timeAlloc.numSymbols = initialBwp->pdschCommon.lengthSymbol;
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
