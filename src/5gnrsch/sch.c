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
 
     Name:     sch.c
  
     Type:     C source file
  
     Desc:     C source code for scheduler fucntions
  
     File:     sch.c
  
**********************************************************************/

/** @file sch.c
@brief This file implements the schedulers main access to MAC layer code.
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
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rg_sch_inf.h"
#include "rg_sch.h"
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"       /* common memory link list library */
#include "cm_lte.x"        /* Common LTE Defines */
#include "tfu.x"           /* TFU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"         /* typedefs for Scheduler */
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "sch.h"
#include "sch_utils.h"
#include "du_log.h"
#include "common_def.h"

extern SchCb schCb[SCH_MAX_INST];
void SchFillCfmPst(Pst *reqPst,Pst *cfmPst,RgMngmt *cfm);
/* local defines */
SchCellCfgCfmFunc SchCellCfgCfmOpts[] = 
{
	packSchCellCfgCfm,     /* LC */
	MacProcSchCellCfgCfm,  /* TC */
	packSchCellCfgCfm      /* LWLC */
};


/**
 * @brief Task Initiation function. 
 *
 * @details
 *
 *     Function : schActvInit
 *     
 *     This function is supplied as one of parameters during MAC's 
 *     task registration. MAC will invoke this function once, after
 *     it creates and attaches this TAPA Task to a system task.
 *     
 *  @param[in]  Ent Entity, the entity ID of this task.     
 *  @param[in]  Inst Inst, the instance ID of this task.
 *  @param[in]  Region Region, the region ID registered for memory 
 *              usage of this task.
 *  @param[in]  Reason Reason.
 *  @return  int
 *      -# ROK
 **/
int schActvInit
(
Ent    entity,            /* entity */
Inst   instId,             /* instance */
Region region,         /* region */
Reason reason          /* reason */
)
{
   Inst inst = (instId  - SCH_INST_START);

   /* Initialize the MAC TskInit structure to zero */
   cmMemset ((uint8_t *)&schCb[inst], 0, sizeof(schCb));

   /* Initialize the MAC TskInit with received values */
   schCb[inst].schInit.ent = entity;
   schCb[inst].schInit.inst = inst;
   schCb[inst].schInit.region = region;
   schCb[inst].schInit.pool = 0;
   schCb[inst].schInit.reason = reason;
   schCb[inst].schInit.cfgDone = FALSE;
   schCb[inst].schInit.acnt = FALSE;
   schCb[inst].schInit.usta = FALSE;
   schCb[inst].schInit.trc = FALSE;
   schCb[inst].schInit.procId = SFndProcId();

   RETVALUE(ROK);
} /* schActvInit */

/**
 * @brief Scheduler instance Configuration Handler. 
 *
 * @details
 *
 *     Function : SchInstCfg
 *     
 *     This function in called by HandleSchGenCfgReq(). It handles the
 *     general configurations of the scheduler instance. Returns
 *     reason for success/failure of this function.
 *     
 *  @param[in]  RgCfg *cfg, the Configuaration information 
 *  @return  U16
 *      -# LCM_REASON_NOT_APPL 
 *      -# LCM_REASON_INVALID_MSGTYPE
 *      -# LCM_REASON_MEM_NOAVAIL
 **/
PUBLIC U16 SchInstCfg
(
RgCfg *cfg,            /* Configuaration information */
Inst  dInst
)
{
   uint16_t ret = LCM_REASON_NOT_APPL;
   Inst     inst = (dInst - SCH_INST_START);

   printf("\nEntered SchInstCfg()");
   /* Check if Instance Configuration is done already */
   if (schCb[inst].schInit.cfgDone == TRUE)
   {
      RETVALUE(LCM_REASON_INVALID_MSGTYPE);
   }
   /* Update the Pst structure for LM interface */
   cmMemcpy((U8 *)&schCb[inst].schInit.lmPst,
            (U8 *)&cfg->s.schInstCfg.genCfg.lmPst,
             sizeof(Pst));
   
   schCb[inst].schInit.inst = inst;
   schCb[inst].schInit.lmPst.srcProcId = schCb[inst].schInit.procId;
   schCb[inst].schInit.lmPst.srcEnt = schCb[inst].schInit.ent;
   schCb[inst].schInit.lmPst.srcInst = schCb[inst].schInit.inst +
   SCH_INST_START;
   schCb[inst].schInit.lmPst.event = EVTNONE;

   schCb[inst].schInit.region = cfg->s.schInstCfg.genCfg.mem.region;
   schCb[inst].schInit.pool = cfg->s.schInstCfg.genCfg.mem.pool;
   schCb[inst].genCfg.tmrRes = cfg->s.schInstCfg.genCfg.tmrRes;
#ifdef LTE_ADV
   schCb[inst].genCfg.forceCntrlSrbBoOnPCel =  cfg->s.schInstCfg.genCfg.forceCntrlSrbBoOnPCel;
   schCb[inst].genCfg.isSCellActDeactAlgoEnable =  cfg->s.schInstCfg.genCfg.isSCellActDeactAlgoEnable;
#endif
   schCb[inst].genCfg.startCellId    = cfg->s.schInstCfg.genCfg.startCellId;
#if 0
   /* Initialzie the timer queue */   
   cmMemset((U8 *)&schCb[inst].tmrTq, 0, sizeof(CmTqType)*RGSCH_TQ_SIZE);
   /* Initialize the timer control point */
   cmMemset((U8 *)&schCb[inst].tmrTqCp, 0, sizeof(CmTqCp));
   schCb[inst].tmrTqCp.tmrLen = RGSCH_TQ_SIZE;

   /* SS_MT_TMR needs to be enabled as schActvTmr needs instance information */
   /* Timer Registration request to SSI */
   if (SRegTmrMt(schCb[inst].schInit.ent, dInst,
      (S16)schCb[inst].genCfg.tmrRes, schActvTmr) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst, "SchInstCfg(): Failed to "
             "register timer.");
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }   
#endif               
   /* Set Config done in TskInit */
   schCb[inst].schInit.cfgDone = TRUE;
   printf("\nScheduler gen config done");
   
   RETVALUE(ret);
}

/**
 * @brief Layer Manager Configuration request handler. 
 *
 * @details
 *
 *     Function : HandleSchGenCfgReq
 *     
 *     This function handles the configuration
 *     request received at scheduler instance from the Layer Manager.
 *     -# Based on the cfg->hdr.elmId.elmnt value it invokes one of the
 *        functions rgHdlGenCfg() or rgHdlSapCfg().
 *     -# Invokes RgMiLrgSchCfgCfm() to send back the confirmation to the LM.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfg, the configuration parameter's structure
 *  @return  S16
 *      -# ROK
 **/
int HandleSchGenCfgReq
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfg     /* config structure  */
)
{
   uint16_t       ret = LCM_PRIM_OK;
   uint16_t       reason = LCM_REASON_NOT_APPL;
   RgMngmt   cfm;
   Pst       cfmPst;

   if(pst->dstInst < SCH_INST_START)
   {
      DU_LOG("\nInvalid inst ID");
      DU_LOG("\nHandleSchGenCfgReq(): "
                "pst->dstInst=%d SCH_INST_START=%d", pst->dstInst,SCH_INST_START); 
      RETVALUE(ROK);
   }
   printf("\nReceived scheduler gen config");
   /* Fill the post structure for sending the confirmation */
	memset(&cfmPst, 0 , sizeof(Pst));
   SchFillCfmPst(pst, &cfmPst, cfg);

   cmMemset((U8 *)&cfm, 0, sizeof(RgMngmt));

#ifdef LMINT3
   cfm.hdr.transId =
      cfg->hdr.transId;
#endif

   cfm.hdr.elmId.elmnt = cfg->hdr.elmId.elmnt;
   switch(cfg->hdr.elmId.elmnt)
   {
      case STSCHINST:
         reason = SchInstCfg(&cfg->t.cfg,pst->dstInst );
         break;
      default:
         ret = LCM_PRIM_NOK;
         reason = LCM_REASON_INVALID_ELMNT;
         DU_LOG("\nInvalid Elmnt=%d", cfg->hdr.elmId.elmnt);
         break;
   }

   if (reason != LCM_REASON_NOT_APPL)
   {
      ret = LCM_PRIM_NOK;
   }

   cfm.cfm.status = ret;
   cfm.cfm.reason = reason;

   SchSendCfgCfm(&cfmPst, &cfm);
   /*   SPutSBuf(pst->region, pst->pool, (Data *)cfg, sizeof(RgMngmt)); */
   
   RETVALUE(ROK);
}/*-- HandleSchGenCfgReq --*/

/**
 * @brief slot indication from MAC to SCH.
 *
 * @details
 *
 *     Function : macSchSlotInd 
 *      
 *      This API is invoked by PHY to indicate slot indication to Scheduler for
 *      a cell.
 *           
 *  @param[in]  Pst            *pst
 *  @param[in]  SlotIndInfo    *slotInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
int macSchSlotInd 
(
Pst                 *pst, 
SlotIndInfo         *slotInd
)
{
   Inst  inst = pst->dstInst-SCH_INST_START;

   /* Now call the TOM (Tfu ownership module) primitive to process further */
   schProcessSlotInd(slotInd, inst);

   RETVALUE(ROK);
}  /* macSchSlotInd */

/*******************************************************************
 *
 * @brief Processes Rach indication from MAC 
 *
 * @details
 *
 *    Function : macSchRachInd
 *
 *    Functionality:
 *      Processes Rach indication from MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int macSchRachInd(Pst *pst, RachIndInfo *rachInd)
{
   Inst  inst = pst->dstInst-SCH_INST_START;
   DU_LOG("\nSCH : Received Rach indication");
   schProcessRachInd(rachInd, inst);
   return ROK;
}

/*******************************************************************
 *
 * @brief Processes CRC indication from MAC 
 *
 * @details
 *
 *    Function : macSchCrcInd
 *
 *    Functionality:
 *      Processes CRC indication from MAC
 *
 * @params[in] Post structure
 *             Crc Indication
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int macSchCrcInd(Pst *pst, CrcIndInfo *crcInd)
{
   switch(crcInd->crcInd[0])
	{
      case CRC_FAILED:
         DU_LOG("\nSCH : Received CRC indication. CRC Status [FAILURE]");
			break;
      case CRC_PASSED:
         DU_LOG("\nSCH : Received CRC indication. CRC Status [PASS]");
			break;
	   default:
			DU_LOG("\nSCH : Invalid CRC state %d", crcInd->crcInd[0]);
			return RFAILED;
	}
   return ROK;
}


/**
 * @brief inti cellCb based on cellCfg
 *
 * @details
 *
 *     Function : InitSchCellCb 
 *      
 *      This API is invoked after receiving schCellCfg
 *           
 *  @param[in]  schCellCb *cell
 *  @param[in]  SchCellCfg *schCellCfg
 *  @return  int
 *      -# ROK 
 *      -# RFAILED 
 **/
int InitSchCellCb(Inst inst, SchCellCfg *schCellCfg)
{
   SchCellCb *cell;
   SCH_ALLOC(cell, sizeof(SchCellCb));
	if(!cell)
	{
      DU_LOG("\nMemory allocation failed in InitSchCellCb");
		return RFAILED;
	}

	cell->cellId = schCellCfg->cellId; 
	cell->instIdx = inst;
	switch(schCellCfg->ssbSchCfg.scsCommon)
	{
	   case SCH_SCS_15KHZ:
		{
			cell->numSlots = SCH_NUM_SLOTS;
		}
		break;
		default:
		   DU_LOG("\nSCS %d not supported", schCellCfg->ssbSchCfg.scsCommon);
	}
  
   for(uint8_t idx=0; idx<SCH_NUM_SLOTS; idx++)
	{
		SchDlSlotInfo *schDlSlotInfo;
		SchUlSlotInfo *schUlSlotInfo;

      /* DL Alloc */
		SCH_ALLOC(schDlSlotInfo, sizeof(SchDlSlotInfo));
		if(!schDlSlotInfo)
		{
			DU_LOG("\nMemory allocation failed in InitSchCellCb");
			return RFAILED;
		}

      /* UL Alloc */
		SCH_ALLOC(schUlSlotInfo, sizeof(SchUlSlotInfo));
		if(!schUlSlotInfo)
		{
			DU_LOG("\nMemory allocation failed in InitSchCellCb");
			return RFAILED;
		}

      schDlSlotInfo->totalPrb = schUlSlotInfo->totalPrb = MAX_NUM_RB;

		for(uint8_t itr=0; itr<SCH_SYMBOL_PER_SLOT; itr++)
		{
			schDlSlotInfo->assignedPrb[itr] = 0;
			schUlSlotInfo->assignedPrb[itr] = 0;
		}
		schUlSlotInfo->schPuschInfo = NULLP;

		for(uint8_t itr=0; itr<MAX_SSB_IDX; itr++)
		{
			memset(&schDlSlotInfo->ssbInfo[itr], 0, sizeof(SsbInfo));
		}

		cell->schDlSlotInfo[idx] = schDlSlotInfo;
		cell->schUlSlotInfo[idx] = schUlSlotInfo;

	}
	schCb[inst].cells[inst] = cell;

   DU_LOG("\nCell init completed for cellId:%d", cell->cellId);

   return ROK;   
}

void fillSchSib1Cfg(
Inst         schInst,
SchSib1Cfg   *sib1SchCfg,
uint16_t     pci,
uint8_t      offsetPointA
)
{
   uint8_t coreset0Idx = 0;
   uint8_t searchSpace0Idx = 0;
   //uint8_t ssbMuxPattern = 0;
   uint8_t numRbs = 0;
   uint8_t numSymbols = 0;
   uint8_t offset = 0;
   uint8_t oValue = 0;
   //uint8_t numSearchSpacePerSlot = 0;
   uint8_t mValue = 0;
   uint8_t firstSymbol = 0; /* need to calculate using formula mentioned in 38.213 */
   uint8_t slotIndex = 0;
   uint8_t FreqDomainResource[6] = {0};
   uint16_t tbSize = 0;
	uint8_t numPdschSymbols = 12; /* considering pdsch region from 2 to 13 */

   PdcchCfg *pdcch = &(sib1SchCfg->sib1PdcchCfg);
   PdschCfg *pdsch = &(sib1SchCfg->sib1PdschCfg);
   BwpCfg *bwp = &(sib1SchCfg->bwp);

   coreset0Idx     = sib1SchCfg->coresetZeroIndex;
   searchSpace0Idx = sib1SchCfg->searchSpaceZeroIndex;

   /* derive the sib1 coreset0 params from table 13-1 spec 38.213 */
   //ssbMuxPattern = coresetIdxTable[coreset0Idx][0];
   numRbs        = coresetIdxTable[coreset0Idx][1];
   numSymbols    = coresetIdxTable[coreset0Idx][2];
   offset        = coresetIdxTable[coreset0Idx][3];

   /* derive the search space params from table 13-11 spec 38.213 */
   oValue                = searchSpaceIdxTable[searchSpace0Idx][0];
   //numSearchSpacePerSlot = searchSpaceIdxTable[searchSpace0Idx][1];
   mValue                = searchSpaceIdxTable[searchSpace0Idx][2];
   firstSymbol           = searchSpaceIdxTable[searchSpace0Idx][3];

   /* calculate the n0, need to add the formulae, as of now the value is 0 
    * Need to add the even and odd values of i during configuration 
    * [(O . 2^u + i . M )  ] mod numSlotsPerSubframe 
    * assuming u = 0, i = 0, numSlotsPerSubframe = 10
    * Also, from this configuration, coreset0 is only on even subframe */
   slotIndex = ((oValue * 1) + (0 * mValue)) % 10; 
   sib1SchCfg->n0 = slotIndex;
 
   /* calculate the PRBs */
   calculatePRB( ((offsetPointA-offset)/6), (numRbs/6), FreqDomainResource);

   /* fill BWP */
   bwp->BWPSize = MAX_NUM_RB; /* whole of BW */
   bwp->BWPStart = 0;
   bwp->subcarrierSpacing = 0;         /* 15Khz */
   bwp->cyclicPrefix = 0;              /* normal */

   /* fill the PDCCH PDU */
   pdcch->coreset0Cfg.coreSet0Size = numRbs;
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
   pdcch->dci.rnti = SI_RNTI;
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
	/* Storing pdschCfg pointer here. Required to access pdsch config while
	fillig up pdcch pdu */
   pdcch->dci.pdschCfg = pdsch; 

   /* fill the PDSCH PDU */
	uint8_t cwCount = 0;
   pdsch->pduBitmap = 0; /* PTRS and CBG params are excluded */
   pdsch->rnti = 0xFFFF; /* SI-RNTI */
   pdsch->pduIndex = 0;
   pdsch->numCodewords = 1;
	for(cwCount = 0; cwCount < pdsch->numCodewords; cwCount++)
	{
      pdsch->codeword[cwCount].targetCodeRate = 308;
      pdsch->codeword[cwCount].qamModOrder = 2;
      pdsch->codeword[cwCount].mcsIndex = sib1SchCfg->sib1Mcs;
      pdsch->codeword[cwCount].mcsTable = 0; /* notqam256 */
      pdsch->codeword[cwCount].rvIndex = 0;
      tbSize = schCalcTbSize(sib1SchCfg->sib1PduLen);
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
   pdsch->freqAlloc.resourceAlloc = 1; /* RAT type-1 RIV format */
   pdsch->freqAlloc.rbStart = offset + SCH_SSB_PRB_DURATION; /* the RB numbering starts from coreset0, and PDSCH is always above SSB */ 
   pdsch->freqAlloc.rbSize = schCalcNumPrb(tbSize,sib1SchCfg->sib1Mcs,numPdschSymbols);
   pdsch->freqAlloc.vrbPrbMapping = 0; /* non-interleaved */
   pdsch->timeAlloc.rowIndex = 1;
   pdsch->timeAlloc.startSymbolIndex = 2; /* spec-38.214, Table 5.1.2.1-1 */
   pdsch->timeAlloc.numSymbols = numPdschSymbols;
   pdsch->beamPdschInfo.numPrgs = 1;
   pdsch->beamPdschInfo.prgSize = 1;
   pdsch->beamPdschInfo.digBfInterfaces = 0;
   pdsch->beamPdschInfo.prg[0].pmIdx = 0;
   pdsch->beamPdschInfo.prg[0].beamIdx[0] = 0;
   pdsch->txPdschPower.powerControlOffset = 0;
   pdsch->txPdschPower.powerControlOffsetSS = 0;

}

/**
 * @brief cell config from MAC to SCH.
 *
 * @details
 *
 *     Function : macSchCellCfgReq
 *      
 *      This API is invoked by MAC to send cell config to SCH
 *           
 *  @param[in]  Pst            *pst
 *  @param[in]  SchCellCfg     *schCellCfg
 *  @return  int
 *      -# ROK 
 *      -# RFAILED 
 **/
int SchHdlCellCfgReq
(
Pst                 *pst, 
SchCellCfg          *schCellCfg
)
{
   int ret = ROK;
   SchCellCb *cellCb;
	SchCellCfgCfm schCellCfgCfm;
	Pst rspPst;
	Inst inst = pst->dstInst-1; 

	InitSchCellCb(inst, schCellCfg);
	cellCb = schCb[inst].cells[inst]; //cells is of MAX_CELLS, why inst
   cellCb->macInst = pst->srcInst;

   /* derive the SIB1 config parameters */
	fillSchSib1Cfg(
	   inst,
	   &(schCellCfg->sib1SchCfg),
		schCellCfg->phyCellId,
		schCellCfg->ssbSchCfg.ssbOffsetPointA);
   memcpy(&cellCb->cellCfg, schCellCfg, sizeof(SchCellCfg));

   memset(&rspPst, 0, sizeof(Pst));
   SCH_FILL_RSP_PST(rspPst, inst);
	rspPst.event = EVENT_SCH_CELL_CFG_CFM;
	schCellCfgCfm.rsp = RSP_OK;
  
   ret = (*SchCellCfgCfmOpts[rspPst.selector])(&rspPst, &schCellCfgCfm);

   return ret;

}

/*******************************************************************
 *
 * @brief Processes DL RLC BO info from MAC
 *
 * @details
 *
 *    Function : macSchDlRlcBoInfo
 *
 *    Functionality:
 *       Processes DL RLC BO info from MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t macSchDlRlcBoInfo(Pst *pst, DlRlcBOInfo *dlBoInfo)
{
   uint16_t  lcIdx;
   Inst  inst = pst->dstInst-SCH_INST_START;
   DU_LOG("\nSCH : Received RLC BO Status indication");

   SchCellCb *cell = schCb[inst].cells[inst];
   SchDlSlotInfo *schDlSlotInfo = \
      cell->schDlSlotInfo[(cell->slotInfo.slot + SCHED_DELTA + PHY_DELTA) % SCH_NUM_SLOTS]; 
  
   for(lcIdx = 0; lcIdx < dlBoInfo->numLc; lcIdx++)
	{
	   if(dlBoInfo->boInfo[lcIdx].lcId == CCCH_LCID)
		{
	      SCH_ALLOC(schDlSlotInfo->msg4Info, sizeof(Msg4Info));
	      if(!schDlSlotInfo->msg4Info)
	      {
	         DU_LOG("\nSCH : Memory allocation failed for msg4Info");
		      schDlSlotInfo = NULL;
		      return RFAILED;
	      }
         schDlSlotInfo->msg4Info->crnti = dlBoInfo->crnti;
			schDlSlotInfo->msg4Info->ndi = 1;
			schDlSlotInfo->msg4Info->harqProcNum = 0;
			schDlSlotInfo->msg4Info->dlAssignIdx = 0;
			schDlSlotInfo->msg4Info->pucchTpc = 0;
			schDlSlotInfo->msg4Info->pucchResInd = 0;
			schDlSlotInfo->msg4Info->harqFeedbackInd = 0;
			schDlSlotInfo->msg4Info->dciFormatId = 1;
	   }
	}

   return ROK;
}

/**********************************************************************
         End of file
**********************************************************************/
