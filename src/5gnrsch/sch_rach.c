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

uint16_t calculateRaRnti(uint8_t symbolIdx, uint8_t slotIdx, uint8_t freqIdx)
{
   uint16_t raRnti = 0;
	uint8_t ulCarrierIdx = 0; /* configured to 0 */
   raRnti = (1+symbolIdx+(14*slotIdx)+(14*80*freqIdx)+(14*80*8*ulCarrierIdx));
	return raRnti;
}

void createSchRaCb(uint16_t tcrnti, Inst schInst)
{
	schCb[schInst].cells[schInst]->raCb[0].tcrnti = tcrnti;
}

uint8_t schProcessRachInd(RachIndInfo *rachInd, Inst schInst)
{
   SchCellCb *cell = schCb[schInst].cells[schInst];

	uint16_t sfn  = rachInd->timingInfo.sfn;  /* get the current timing info */
	uint16_t slot = rachInd->timingInfo.slot;
	uint16_t sfnSlot = ((sfn * 10) + slot + 1); /* scheduled in the next slot */
	uint8_t rarDelay = 0;

	if(sfnSlot % cell->cellCfg.sib1SchCfg.sib1RepetitionPeriod == 0)
	{
	   /* the next slot has SIB1 occasion, so send it after 2 slot */
	   rarDelay = 2;
	}
	else
	{
	   /* send RAR in the next slot */
	   //rarDelay = 1;
		rarDelay = 2;
	}

	if((slot+rarDelay) % SCH_NUM_SLOTS == 0)
	{
	   sfn = (sfn+1) % SCH_MAX_SFN;
   }

	slot = ((slot + rarDelay) % SCH_NUM_SLOTS);

   SchDlAlloc *dlAlloc = cell->dlAlloc[slot]; /* RAR will sent in the next slot */
	RarInfo *rarInfo = &(dlAlloc->rarInfo);

   /* rar message presense in next slot ind and will be scheduled */
   dlAlloc->rarPres = true;

   /* calculate the ra-rnti value */
	uint16_t raRnti = 0;
	raRnti = calculateRaRnti(rachInd->symbolIdx,rachInd->slotIdx,rachInd->freqIdx);
   
	/* create raCb at SCH */
	createSchRaCb(rachInd->crnti,schInst);

	/* fill RAR info */
	rarInfo->raRnti = raRnti;
	rarInfo->tcrnti = rachInd->crnti;
	rarInfo->RAPID = rachInd->preambleIdx;
	rarInfo->ta = rachInd->timingAdv;
	rarInfo->msg3StartRb = 0; /* will be set during implementation of msg3 */
	rarInfo->msg3NumRb = 0; /* will be set during implementation of msg3 */

   return ROK;
}

uint8_t schFillRar(RarAlloc *rarAlloc, uint16_t raRnti, uint16_t pci, uint8_t offsetPointA)
{
   uint8_t coreset0Idx = 0;
   uint8_t searchSpace0Idx = 0;
   //uint8_t ssbMuxPattern = 0;
   uint8_t numRbs = 0;
   uint8_t numSymbols = 0;
   uint8_t offset = 0;
   //uint8_t oValue = 0;
   //uint8_t numSearchSpacePerSlot = 0;
   //uint8_t mValue = 0;
   uint8_t firstSymbol = 0; /* need to calculate using formula mentioned in 38.213 */
   uint8_t FreqDomainResource[6] = {0};

	PdcchCfg *pdcch = &rarAlloc->rarPdcchCfg;
	PdschCfg *pdsch = &rarAlloc->rarPdschCfg;

   coreset0Idx     = 0;
   searchSpace0Idx = 0;

   /* derive the sib1 coreset0 params from table 13-1 spec 38.213 */
   //ssbMuxPattern = coresetIdxTable[coreset0Idx][0];
   numRbs        = coresetIdxTable[coreset0Idx][1];
   numSymbols    = coresetIdxTable[coreset0Idx][2];
   offset        = coresetIdxTable[coreset0Idx][3];

   /* derive the search space params from table 13-11 spec 38.213 */
   //oValue                = searchSpaceIdxTable[searchSpace0Idx][0];
   //numSearchSpacePerSlot = searchSpaceIdxTable[searchSpace0Idx][1];
   //mValue                = searchSpaceIdxTable[searchSpace0Idx][2];
   firstSymbol           = searchSpaceIdxTable[searchSpace0Idx][3];

   /* calculate the PRBs */
   freqDomResourceAlloc( ((offsetPointA-offset)/6), (numRbs/6), FreqDomainResource);

   /* fill the PDCCH PDU */
   pdcch->pdcchBwpCfg.BWPSize = MAX_NUM_RB; /* whole of BW */
   pdcch->pdcchBwpCfg.BWPStart = 0;
   pdcch->pdcchBwpCfg.subcarrierSpacing = 0;         /* 15Khz */
   pdcch->pdcchBwpCfg.cyclicPrefix = 0;              /* normal */
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
   pdcch->dci.cceIndex = 0;
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
   pdsch->pdschBwpCfg.BWPSize = MAX_NUM_RB; /* whole of BW */
   pdsch->pdschBwpCfg.BWPStart = 0;
   pdsch->numCodewords = 1;
	for(cwCount = 0; cwCount < pdsch->numCodewords; cwCount++)
	{
      pdsch->codeword[cwCount].targetCodeRate = 308;
      pdsch->codeword[cwCount].qamModOrder = 2;
      pdsch->codeword[cwCount].mcsIndex = 4; /* msc configured to 4 */
      pdsch->codeword[cwCount].mcsTable = 0; /* notqam256 */
      pdsch->codeword[cwCount].rvIndex = 0;
      pdsch->codeword[cwCount].tbSize = 768;
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
   pdsch->timeAlloc.startSymbolIndex = 2; /* spec-38.214, Table 5.1.2.1-1 */
   pdsch->timeAlloc.numSymbols = 12;
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
