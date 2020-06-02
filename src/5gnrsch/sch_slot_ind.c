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

SchMacDlAllocFunc schMacDlAllocOpts[] =
{
	packSchMacDlAlloc,
	MacProcDlAlloc,
	packSchMacDlAlloc
};

extern SchCb schCb[SCH_MAX_INST];
extern int schFillRar(RarAlloc *rarAlloc, uint16_t raRnti, uint16_t pci, uint8_t
offsetPointA);

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
int sendDlAllocToMac(DlAlloc *dlAlloc, Inst inst)
{
	Pst pst;

   memset(&pst, 0, sizeof(Pst));
   SCH_FILL_RSP_PST(pst, inst);
	pst.event = EVENT_DL_ALLOC;

	return(*schMacDlAllocOpts[pst.selector])(&pst, dlAlloc);

}

/*******************************************************************
 *
 * @brief Fills pdcch and pdsch info for msg4
 *
 * @details
 *
 *    Function : schFillMsg4
 *
 *    Functionality:
 *       Fills pdcch and pdsch info for msg4
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t schFillMsg4(Msg4Alloc *msg4Alloc, uint16_t crnti, uint16_t pci, uint8_t offsetPointA)
{
   Inst inst = 0;
   uint8_t coreset0Idx = 0;
   uint8_t numRbs = 0;
   uint8_t firstSymbol = 0;
   uint8_t numSymbols = 0;
   uint8_t offset = 0;
   uint8_t FreqDomainResource[6] = {0};
   SchBwpDlCfg *initialBwp = &schCb[inst].cells[inst]->cellCfg.schInitialBwp;

   PdcchCfg *pdcch = &msg4Alloc->msg4PdcchCfg;
   PdschCfg *pdsch = &msg4Alloc->msg4PdschCfg;
 
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
   pdcch->dci.rnti = crnti;
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
   pdsch->rnti = crnti;
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
      /* 38.214: Table 5.1.3.2-1,  divided by 8 to get the value in bytes */
      /* TODO : Calculate tbSize based of DL CCCH msg size */
      pdsch->codeword[cwCount].tbSize = 2216/8;
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
   /* the RB numbering starts from coreset0, and PDSCH is always above SSB */
   pdsch->freqAlloc.rbStart = offset + SCH_SSB_PRB_DURATION;
   /* formula used for calculation of rbSize, 38.213 section 5.1.3.2 *
    * Ninfo = S . Nre . R . Qm . v                                   *
    * Nre' = Nsc . NsymPdsch - NdmrsSymb - Noh                       *
    * Nre = min(156,Nre') . nPrb                                     */
   /* TODO : Calculate rbSize based on tbSize calculated */
   pdsch->freqAlloc.rbSize = 28;
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
uint8_t schProcessSlotInd(SlotIndInfo *slotInd, Inst schInst)
{
   int ret = ROK;
	uint8_t ssb_rep;
	uint16_t sfn  = slotInd->sfn;
	uint16_t slot = slotInd->slot;
	uint16_t sfnSlot = 0;
	DlAlloc dlAlloc;
	memset(&dlAlloc,0,sizeof(DlAlloc));
   DlBrdcstAlloc *dlBrdcstAlloc = &dlAlloc.brdcstAlloc;
	RarAlloc *rarAlloc = &dlAlloc.rarAlloc;
   Msg4Alloc *msg4Alloc = &dlAlloc.msg4Alloc;
	dlBrdcstAlloc->ssbTrans = NO_SSB;
   dlBrdcstAlloc->sib1Trans = NO_SIB1;
	

	SchCellCb *cell = schCb[schInst].cells[schInst];

#ifdef LTE_L2_MEAS
   glblTtiCnt++;
#endif
  
//   schDlResAlloc(cell, slotInd);

	ssb_rep = cell->cellCfg.ssbSchCfg.ssbPeriod;
	memcpy(&cell->slotInfo, slotInd, sizeof(SlotIndInfo));
	dlBrdcstAlloc->ssbIdxSupported = 1;

   if((slot + SCHED_DELTA) >= SCH_NUM_SLOTS)
	{
      sfn = (sfn+1)%SCH_MAX_SFN;
	}
	slot = ((slot + SCHED_DELTA) % SCH_NUM_SLOTS);
   sfnSlot = ((sfn * 10) + slot);

	dlAlloc.slotIndInfo.sfn = sfn;
	dlAlloc.slotIndInfo.slot = slot;
	dlAlloc.cellId = cell->cellId;

	/* Identify SSB ocassion*/
	if (sfnSlot % SCH_MIB_TRANS == 0)
	{
		dlBrdcstAlloc->ssbTrans = SSB_TRANSMISSION;
	}
	else if (sfnSlot % ssb_rep == 0)
	{
		dlBrdcstAlloc->ssbTrans = SSB_REPEAT;
	}
	else
	{
	   /* not SSB occassion */
	}

   /* Identify SIB1 occasions */
   if(sfnSlot % cell->cellCfg.sib1SchCfg.sib1NewTxPeriod == 0)
	{
	   dlBrdcstAlloc->sib1Trans = SIB1_TRANSMISSION;
	}
	else if (sfnSlot % cell->cellCfg.sib1SchCfg.sib1RepetitionPeriod == 0)
	{
	   dlBrdcstAlloc->sib1Trans = SIB1_REPITITION;
	}
	else
	{
	   /* not SIB1 occassion */
	}

	if(dlBrdcstAlloc->ssbTrans || dlBrdcstAlloc->sib1Trans)
	{
	   dlAlloc.isBroadcastPres = true;
	   ret = schBroadcastAlloc(cell, dlBrdcstAlloc,slot);
      if(ret != ROK)
      {
         DU_LOG("\nschBroadcastAlloc failed");
         RETVALUE(ret);
      }
   }

   /* check for RAR */
	if(cell->dlAlloc[slot]->rarPres == true)
	{
	   dlAlloc.isRarPres = true;
	   /* RAR info is copied, this was earlier filled in schProcessRachInd */
	   memcpy(&rarAlloc->rarInfo, &cell->dlAlloc[slot]->rarInfo, sizeof(RarInfo));

		/* pdcch and pdsch data is filled */
      schFillRar(rarAlloc,
		   cell->dlAlloc[slot]->rarInfo.raRnti,
		   cell->cellCfg.phyCellId,
		   cell->cellCfg.ssbSchCfg.ssbOffsetPointA);

     cell->dlAlloc[slot]->rarPres = false;
   }

   /* check for MSG4 */
   if(cell->dlAlloc[slot]->msg4Pres == true)
   {
       dlAlloc.isMsg4Pres = true;

       /* Msg4 info is copied, this was earlier filled in macSchDlRlcBoInfo */
       memcpy(&msg4Alloc->msg4Info, &cell->dlAlloc[slot]->msg4Info, \
          sizeof(Msg4Info));
             
       /* pdcch and pdsch data is filled */
       schFillMsg4(msg4Alloc, 
          cell->dlAlloc[slot]->msg4Info.crnti,
          cell->cellCfg.phyCellId,
          cell->cellCfg.ssbSchCfg.ssbOffsetPointA);

       cell->dlAlloc[slot]->msg4Pres = false;
   }


	/* send msg to MAC */
   ret = sendDlAllocToMac(&dlAlloc, schInst);
   if(ret != ROK)
   {
      DU_LOG("\nSending DL Broadcast allocation from SCH to MAC failed");
      RETVALUE(ret);
   }
    
	schUlResAlloc(cell, schInst);

	return ret;
}

/**********************************************************************
  End of file
 **********************************************************************/


