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
#include "common_def.h"

extern SchCb schCb[SCH_MAX_INST];
extern uint16_t prachCfgIdxTable[MAX_PRACH_CONFIG_IDX][8];
extern uint16_t numRbForPrachTable[MAX_RACH_NUM_RB_IDX][5];

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
		ssbStartPrb = cell->cellCfg.ssbSchCfg.ssbOffsetPointA;
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
		schDlSlotInfo->sib1Pres = true;
		for(idx=0; idx<SCH_SYMBOL_PER_SLOT; idx++)
		{
			schDlSlotInfo->assignedPrb[idx] = ssbStartPrb + SCH_SSB_NUM_PRB + 1 + 10; /* 10 PRBs for sib1 */
		}
	   memcpy(&dlBrdcstAlloc->sib1Alloc.bwp, &cell->cellCfg.sib1SchCfg.bwp, sizeof(BwpCfg)); 
	   memcpy(&dlBrdcstAlloc->sib1Alloc.sib1PdcchCfg, &cell->cellCfg.sib1SchCfg.sib1PdcchCfg, sizeof(PdcchCfg)); 
	   memcpy(&dlBrdcstAlloc->sib1Alloc.sib1PdschCfg, &cell->cellCfg.sib1SchCfg.sib1PdschCfg, sizeof(PdschCfg)); 
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
   SCH_FILL_RSP_PST(pst, inst);
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
int schPrachResAlloc(SchCellCb *cell, UlSchedInfo *ulSchedInfo, SlotIndInfo prachOccasionTimingInfo)
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
	
	if((prachOccasionTimingInfo.sfn%x) != y)
	{
	   /* prach occasion does not lie in this SFN */
		DU_LOG("\nPRACH ocassion doesn't lie in this SFN");
	   return RFAILED;
	}
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
   SchUlSlotInfo  *schUlSlotInfo;
   SlotIndInfo ulTimingInfo;

   /* add PHY delta */
   ADD_DELTA_TO_TIME(cell->slotInfo,ulTimingInfo,PHY_DELTA+SCHED_DELTA);

	ulSchedInfo.cellId = cell->cellId;
	ulSchedInfo.slotIndInfo.sfn = ulTimingInfo.sfn;
   ulSchedInfo.slotIndInfo.slot = ulTimingInfo.slot;

   /* Schedule resources for PRACH */
	schPrachResAlloc(cell, &ulSchedInfo, ulTimingInfo);

	schUlSlotInfo = cell->schUlSlotInfo[ulTimingInfo.slot]; 
	
	if(schUlSlotInfo->schPuschInfo)
	{
      ulSchedInfo.crnti = cell->raCb[0].tcrnti;
		ulSchedInfo.dataType |= SCH_DATATYPE_PUSCH;
		memcpy(&ulSchedInfo.schPuschInfo, schUlSlotInfo->schPuschInfo,
				sizeof(SchPuschInfo));
		SCH_FREE(schUlSlotInfo->schPuschInfo, sizeof(SchPuschInfo));
		schUlSlotInfo->schPuschInfo = NULL;
	}

	//send msg to MAC
   ret = sendUlSchInfoToMac(&ulSchedInfo, schInst);
   if(ret != ROK)
   {
      DU_LOG("\nSending UL Sch info from SCH to MAC failed");
   }
    
	memset(cell->schUlSlotInfo[ulTimingInfo.slot], 0, sizeof(SchUlSlotInfo));

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
uint8_t schDlRsrcAllocMsg4(Msg4Alloc *msg4Alloc, SchCellCb *cell, uint16_t slot)
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

   PdcchCfg *pdcch = &msg4Alloc->msg4PdcchCfg;
   PdschCfg *pdsch = &msg4Alloc->msg4PdschCfg;
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
   schAllocFreqDomRsc( ((offsetPointA-offset)/6), (numRbs/6), FreqDomainResource);

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
   pdcch->coreset0Cfg.coreSet0Size = numRbs;
   pdcch->coreset0Cfg.shiftIndex = cell->cellCfg.phyCellId;
   pdcch->coreset0Cfg.precoderGranularity = 0; /* sameAsRegBundle */
   pdcch->numDlDci = 1;
   pdcch->dci.rnti = cell->schDlSlotInfo[slot]->msg4Info->crnti;
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
   pdsch->rnti = cell->schDlSlotInfo[slot]->msg4Info->crnti;
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

/**********************************************************************
  End of file
 **********************************************************************/

