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
#include "common_def.h"	
#include "du_app_mac_inf.h"
#include "lrg.h"
#include "tfu.h"
#include "du_log.h"
#include "rgr.h"
#include "rg_sch_inf.h"
#include "rg_sch.h"

#include "tfu.x"           /* TFU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"         /* typedefs for Scheduler */
#include "mac_sch_interface.h"
#include "sch.h"
#include "sch_fcfs.h"
#include "sch_utils.h"

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
uint8_t schActvInit(Ent entity, Inst instId, Region region, Reason reason)
{
   Inst inst = (instId  - SCH_INST_START);

   /* Initialize the MAC TskInit structure to zero */
   memset ((uint8_t *)&schCb[inst], 0, sizeof(schCb));

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
   schCb[inst].schInit.procId = ODU_GET_PROCID();

   return ROK;
} /* schActvInit */

/**
 * @brief Scheduler All Apis initialized. 
 *
 * @details
 *
 *     Function : schAllApisInit
 *     
 *     This function initializes all Scheduler APIs/functionality for each kind
 *     of scheduler type. 
 *     
 *  @param[in]  Inst inst, the Scheduler instance 
 *  @return  void
 **/
void schAllApisInit(Inst inst)
{
    schFcfsAllApisInit(&schCb[inst].allApis[SCH_FCFS]);  
}

/**
 * @brief Scheduler instance Configuration Handler. 
 *
 * @details
 *
 *     Function : SchInstCfg
 *     
 *     This function in called by SchProcGenCfgReq(). It handles the
 *     general configurations of the scheduler instance. Returns
 *     reason for success/failure of this function.
 *     
 *  @param[in]  RgCfg *cfg, the Configuaration information 
 *  @return  uint16_t
 *      -# LCM_REASON_NOT_APPL 
 *      -# LCM_REASON_INVALID_MSGTYPE
 *      -# LCM_REASON_MEM_NOAVAIL
 **/
uint8_t SchInstCfg(RgCfg *cfg, Inst  dInst)
{
   uint16_t ret = LCM_REASON_NOT_APPL;
   Inst     inst = (dInst - SCH_INST_START);

   DU_LOG("\nDEBUG  -->  SCH : Entered SchInstCfg()");
   /* Check if Instance Configuration is done already */
   if (schCb[inst].schInit.cfgDone == TRUE)
   {
      return LCM_REASON_INVALID_MSGTYPE;
   }
   /* Update the Pst structure for LM interface */
   memcpy(&schCb[inst].schInit.lmPst, &cfg->s.schInstCfg.genCfg.lmPst, sizeof(Pst));

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

   /* Initialzie the timer queue */   
   memset(&schCb[inst].tmrTq, 0, sizeof(CmTqType) * SCH_TQ_SIZE);
   /* Initialize the timer control point */
   memset(&schCb[inst].tmrTqCp, 0, sizeof(CmTqCp));
   schCb[inst].tmrTqCp.tmrLen = RGSCH_TQ_SIZE;

   /* SS_MT_TMR needs to be enabled as schActvTmr needs instance information */
   /* Timer Registration request to system services */
   if (ODU_REG_TMR_MT(schCb[inst].schInit.ent, dInst, (int)schCb[inst].genCfg.tmrRes, schActvTmr) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : SchInstCfg(): Failed to "
	    "register timer.");
      return (LCM_REASON_MEM_NOAVAIL);
   }   
              
   /* Set Config done in TskInit */
   schCb[inst].schInit.cfgDone = TRUE;
   DU_LOG("\nINFO   -->  SCH : Scheduler gen config done");
   
   schAllApisInit(inst);
   return ret;
}

/**
 * @brief Layer Manager Configuration request handler. 
 *
 * @details
 *
 *     Function : SchProcGenCfgReq
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
uint8_t SchProcGenCfgReq(Pst *pst, RgMngmt *cfg)
{
   uint8_t   ret = LCM_PRIM_OK;
   uint16_t  reason = LCM_REASON_NOT_APPL;
   RgMngmt   cfm;
   Pst       cfmPst;

#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTMAC -> ENTSCH : GENERAL_CFG_REQ\n");
#endif

   if(pst->dstInst < SCH_INST_START)
   {
      DU_LOG("\nERROR  -->  SCH : Invalid inst ID");
      DU_LOG("\nERROR  -->  SCH : SchProcGenCfgReq(): "
	    "pst->dstInst=%d SCH_INST_START=%d", pst->dstInst,SCH_INST_START); 
      return ROK;
   }
   DU_LOG("\nINFO   -->  SCH : Received scheduler gen config");
   /* Fill the post structure for sending the confirmation */
   memset(&cfmPst, 0 , sizeof(Pst));
   SchFillCfmPst(pst, &cfmPst, cfg);

   memset(&cfm, 0, sizeof(RgMngmt));

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
	 DU_LOG("\nERROR  -->  SCH : Invalid Elmnt=%d", cfg->hdr.elmId.elmnt);
	 break;
   }

   if (reason != LCM_REASON_NOT_APPL)
   {
      ret = LCM_PRIM_NOK;
   }

   cfm.cfm.status = ret;
   cfm.cfm.reason = reason;

   SchSendCfgCfm(&cfmPst, &cfm);
   /*   SCH_FREE(pst->region, pst->pool, (Data *)cfg, sizeof(RgMngmt)); */

   return ROK;
}/*-- SchProcGenCfgReq --*/

#ifdef NR_TDD
/**
 *@brief Returns TDD periodicity in micro seconds
 *
 * @details
 * 
 * Function : schGetPeriodicityInMsec 
 * 
 * This API retunrs TDD periodicity in micro seconds
 * 
 * @param[in] DlUlTxPeriodicity 
 * @return  periodicityInMsec
 * **/

uint16_t schGetPeriodicityInMsec(DlUlTxPeriodicity tddPeriod)
{
   uint16_t  periodicityInMsec = 0;
   switch(tddPeriod)
   {
      case TX_PRDCTY_MS_0P5:
      {
         periodicityInMsec = 500;
         break;
      }
      case TX_PRDCTY_MS_0P625:
      {
         periodicityInMsec = 625;
         break;
      }
      case TX_PRDCTY_MS_1:
      {
         periodicityInMsec = 1000;
         break;
      }
      case TX_PRDCTY_MS_1P25:
      {
         periodicityInMsec = 1250;
         break;
      }
      case TX_PRDCTY_MS_2:
      {
         periodicityInMsec = 2000;
         break;
      }
      case TX_PRDCTY_MS_2P5:
      {
         periodicityInMsec = 2500;
         break;
      }
      case TX_PRDCTY_MS_5:
      {
         periodicityInMsec = 5000;
         break;
      }
      case TX_PRDCTY_MS_10:
      {
         periodicityInMsec = 10000;
         break;
      }
      default:
      {
	 DU_LOG("\nERROR  -->  SCH : Invalid DlUlTxPeriodicity:%d", tddPeriod);
      }
   }

   return periodicityInMsec;
}


/**
 * @brief init TDD slot config 
 *
 * @details
 *
 *     Function : schInitTddSlotCfg 
 *      
 *      This API is invoked after receiving schCellCfg
 *           
 *  @param[in]  schCellCb *cell
 *  @param[in]  SchCellCfg *schCellCfg
 *  @return  void
 **/
void schInitTddSlotCfg(SchCellCb *cell, SchCellCfg *schCellCfg)
{
   uint16_t periodicityInMicroSec = 0;
   int8_t slotIdx, symbIdx;

   periodicityInMicroSec = schGetPeriodicityInMsec(schCellCfg->tddCfg.tddPeriod);
   cell->numSlotsInPeriodicity = (periodicityInMicroSec * pow(2, schCellCfg->numerology))/1000;
   cell->slotFrmtBitMap = 0;
   cell->symbFrmtBitMap = 0;
   for(slotIdx = cell->numSlotsInPeriodicity-1; slotIdx >= 0; slotIdx--)
   {
      symbIdx = 0;
      /* If the first and last symbol are the same, the entire slot is the same type */
      if((schCellCfg->tddCfg.slotCfg[slotIdx][symbIdx] == schCellCfg->tddCfg.slotCfg[slotIdx][MAX_SYMB_PER_SLOT-1]) &&
              schCellCfg->tddCfg.slotCfg[slotIdx][symbIdx] != FLEXI_SLOT)
      {
         switch(schCellCfg->tddCfg.slotCfg[slotIdx][symbIdx])
         {
            case DL_SLOT:
            {
               /*BitMap to be set to 00 */
               cell->slotFrmtBitMap = (cell->slotFrmtBitMap<<2);
               break;
            }
            case UL_SLOT:
            {
               /*BitMap to be set to 01 */
               cell->slotFrmtBitMap = ((cell->slotFrmtBitMap<<2) | (UL_SLOT));
               break;
            }
            default:
               DU_LOG("\nERROR  -->  SCH : Invalid slot Config in schInitTddSlotCfg");
           }
         continue;
      }
      /* slot config is flexible. First set slotBitMap to 10 */
      cell->slotFrmtBitMap = ((cell->slotFrmtBitMap<<2) | (FLEXI_SLOT));

      /* Now set symbol bitmap */ 
      for(symbIdx = MAX_SYMB_PER_SLOT-1; symbIdx >= 0; symbIdx--)
      {
         switch(schCellCfg->tddCfg.slotCfg[slotIdx][symbIdx])
         {
            case DL_SLOT:
            {
               /*symbol BitMap to be set to 00 */
               cell->symbFrmtBitMap = (cell->symbFrmtBitMap<<2);
               break;
            }
            case UL_SLOT:
            {
               /*symbol BitMap to be set to 01 */
               cell->symbFrmtBitMap = ((cell->symbFrmtBitMap<<2) | (UL_SLOT));
               break;
            }
            case FLEXI_SLOT:
            {
               /*symbol BitMap to be set to 10 */
               cell->symbFrmtBitMap = ((cell->symbFrmtBitMap<<2) | (FLEXI_SLOT));
               break;
            }
            default:
	       DU_LOG("\nERROR  -->  SCH : Invalid slot Config in schInitTddSlotCfg");
         }
      }
   }
}
#endif

/**
 * @brief Fill SSB start symbol
 *
 * @details
 *
 *     Function : fillSsbStartSymb 
 *      
 *      This API stores SSB start index per beam
 *           
 *  @param[in]  SchCellCb     *cellCb
 *  @return  int
 *      -# ROK 
 *      -# RFAILED 
 **/
void fillSsbStartSymb(SchCellCb *cellCb)
{
   uint8_t cnt, scs, symbIdx, ssbStartSymbArr[SCH_MAX_SSB_BEAM];

   scs = cellCb->cellCfg.ssbSchCfg.scsCommon;

   memset(ssbStartSymbArr, 0, sizeof(SCH_MAX_SSB_BEAM));
   symbIdx = 0;
   /* Determine value of "n" based on Section 4.1 of 3GPP TS 38.213 */
   switch(scs)
   {
      case SCS_15KHZ:
	 {
            if(cellCb->cellCfg.dlFreq <= 300000)
	       cnt = 2;/* n = 0, 1 */
            else
               cnt = 4; /* n = 0, 1, 2, 3 */
	    for(uint8_t idx=0; idx<cnt; idx++)
	    {
	       /* start symbol determined using {2, 8} + 14n */
	       ssbStartSymbArr[symbIdx++] = 2 +  MAX_SYMB_PER_SLOT*idx;
	       ssbStartSymbArr[symbIdx++] = 8 +  MAX_SYMB_PER_SLOT*idx;
	    }
	 }
	 break;
      case SCS_30KHZ:
	 {
            if(cellCb->cellCfg.dlFreq <= 300000)
	       cnt = 1;/* n = 0 */
            else
               cnt = 2; /* n = 0, 1 */
	    for(uint8_t idx=0; idx<cnt; idx++)
	    {
	       /* start symbol determined using {4, 8, 16, 20} + 28n */
	       ssbStartSymbArr[symbIdx++] = 4 +  MAX_SYMB_PER_SLOT*idx;
	       ssbStartSymbArr[symbIdx++] = 8 +  MAX_SYMB_PER_SLOT*idx;
	       ssbStartSymbArr[symbIdx++] = 16 +  MAX_SYMB_PER_SLOT*idx;
	       ssbStartSymbArr[symbIdx++] = 20 +  MAX_SYMB_PER_SLOT*idx;
            }
	 }
	 break;
      default:
	 DU_LOG("\nERROR  -->  SCH : SCS %d is currently not supported", scs);
   }
   memset(cellCb->ssbStartSymbArr, 0, sizeof(SCH_MAX_SSB_BEAM));
   memcpy(cellCb->ssbStartSymbArr, ssbStartSymbArr, SCH_MAX_SSB_BEAM);

}


/**
 * @brief init cellCb based on cellCfg
 *
 * @details
 *
 *     Function : schInitCellCb 
 *      
 *      This API is invoked after receiving schCellCfg
 *           
 *  @param[in]  schCellCb *cell
 *  @param[in]  SchCellCfg *schCellCfg
 *  @return  int
 *      -# ROK 
 *      -# RFAILED 
 **/
uint8_t schInitCellCb(Inst inst, SchCellCfg *schCellCfg)
{
   SchCellCb *cell= NULLP;
   SCH_ALLOC(cell, sizeof(SchCellCb));
   if(!cell)
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation failed in schInitCellCb");
      return RFAILED;
   }

   cell->cellId = schCellCfg->cellId; 
   cell->instIdx = inst;
   switch(schCellCfg->numerology)
   {
      case SCH_NUMEROLOGY_0:
	 {
	    cell->numSlots = SCH_MU0_NUM_SLOTS;
	 }
	 break;
      case SCH_NUMEROLOGY_1:
	 {
	    cell->numSlots = SCH_MU1_NUM_SLOTS;
	 }
	 break;
      case SCH_NUMEROLOGY_2:
	 {
	    cell->numSlots = SCH_MU2_NUM_SLOTS;
	 }
	 break;
      case SCH_NUMEROLOGY_3:
	 {
	    cell->numSlots = SCH_MU3_NUM_SLOTS;
	 }
	 break;
      case SCH_NUMEROLOGY_4:
	 {
	    cell->numSlots = SCH_MU4_NUM_SLOTS;
	 }
	 break;
      default:
	 DU_LOG("\nERROR  -->  SCH : Numerology %d not supported", schCellCfg->numerology);
   }
#ifdef NR_TDD
   schInitTddSlotCfg(cell, schCellCfg);   
#endif

   SCH_ALLOC(cell->schDlSlotInfo, cell->numSlots * sizeof(SchDlSlotInfo*));
   if(!cell->schDlSlotInfo)
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation failed in schInitCellCb for schDlSlotInfo");
      return RFAILED;
   }

   SCH_ALLOC(cell->schUlSlotInfo, cell->numSlots * sizeof(SchUlSlotInfo*));
   if(!cell->schUlSlotInfo)
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation failed in schInitCellCb for schUlSlotInfo");
      return RFAILED;
   }

   for(uint8_t idx=0; idx<cell->numSlots; idx++)
   {
      SchDlSlotInfo *schDlSlotInfo;
      SchUlSlotInfo *schUlSlotInfo;

      /* DL Alloc */
      SCH_ALLOC(schDlSlotInfo, sizeof(SchDlSlotInfo));
      if(!schDlSlotInfo)
      {
         DU_LOG("\nERROR  -->  SCH : Memory allocation failed in schInitCellCb");
         return RFAILED;
      }

      /* UL Alloc */
      SCH_ALLOC(schUlSlotInfo, sizeof(SchUlSlotInfo));
      if(!schUlSlotInfo)
      {
	 DU_LOG("\nERROR  -->  SCH : Memory allocation failed in schInitCellCb");
	 return RFAILED;
      }

      schInitDlSlot(schDlSlotInfo);
      schInitUlSlot(schUlSlotInfo);

      cell->schDlSlotInfo[idx] = schDlSlotInfo;
      cell->schUlSlotInfo[idx] = schUlSlotInfo;

   }
   cell->firstSsbTransmitted = false;
   cell->firstSib1Transmitted = false;
   fillSsbStartSymb(cell);
   cmLListInit(&cell->ueToBeScheduled);

#ifdef NR_DRX
   memset(cell->drxCb, 0, MAX_DRX_SIZE*sizeof(SchDrxCb));
#endif   
   schCb[inst].cells[inst] = cell;

   DU_LOG("\nINFO  -->  SCH : Cell init completed for cellId:%d", cell->cellId);

   return ROK;   
}

/**
 * @brief Fill SIB1 configuration
 *
 * @details
 *
 *     Function : fillSchSib1Cfg
 *
 *     Fill SIB1 configuration
 *
 *  @param[in]  uint8_t bandwidth : total available bandwidth
 *              uint8_t numSlots : total slots per SFN
 *              SchSib1Cfg *sib1SchCfg : cfg to be filled
 *              uint16_t pci : physical cell Id
 *              uint8_t offsetPointA : offset
 *  @return  void
 **/
void fillSchSib1Cfg(uint8_t mu, uint8_t bandwidth, uint8_t numSlots, SchSib1Cfg *sib1SchCfg, uint16_t pci, uint8_t offsetPointA)
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
   uint8_t FreqDomainResource[FREQ_DOM_RSRC_SIZE] = {0};
   uint16_t tbSize = 0;
   uint8_t ssbIdx = 0;

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
   slotIndex = (int)((oValue*pow(2, mu)) + floor(ssbIdx*mValue))%numSlots;
   sib1SchCfg->n0 = slotIndex;

   /* fill BWP */
   switch(bandwidth)
   {
      case BANDWIDTH_20MHZ:
	 {
            bwp->freqAlloc.numPrb = TOTAL_PRB_20MHZ_MU0;
	 }
	 break;
      case BANDWIDTH_100MHZ:
	 {
            bwp->freqAlloc.numPrb = TOTAL_PRB_100MHZ_MU1;
	 }
	 break;
      default:
	 DU_LOG("\nERROR  -->  SCH : Bandwidth %d not supported", bandwidth);

   }
   bwp->freqAlloc.startPrb = 0;
   bwp->subcarrierSpacing  = 0;         /* 15Khz */
   bwp->cyclicPrefix       = 0;              /* normal */

   /* fill the PDCCH PDU */
   pdcch->coresetCfg.coreSetSize = numRbs;
   pdcch->coresetCfg.startSymbolIndex = firstSymbol;
   pdcch->coresetCfg.durationSymbols = numSymbols;
   
   /* Fill Bitmap for PRBs in coreset */
   fillCoresetFeqDomAllocMap(((offsetPointA-offset)/6), (numRbs/6), FreqDomainResource);
   covertFreqDomRsrcMapToIAPIFormat(FreqDomainResource, pdcch->coresetCfg.freqDomainResource);

   pdcch->coresetCfg.cceRegMappingType = 1; /* coreset0 is always interleaved */
   pdcch->coresetCfg.regBundleSize = 6;    /* spec-38.211 sec 7.3.2.2 */
   pdcch->coresetCfg.interleaverSize = 2;  /* spec-38.211 sec 7.3.2.2 */
   pdcch->coresetCfg.coreSetType = 0;
   pdcch->coresetCfg.shiftIndex = pci;
   pdcch->coresetCfg.precoderGranularity = 0; /* sameAsRegBundle */
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
      tbSize = schCalcTbSize(sib1SchCfg->sib1PduLen + TX_PAYLOAD_HDR_LEN);
      pdsch->codeword[cwCount].tbSize = tbSize;
   }
   pdsch->dataScramblingId                   = pci;
   pdsch->numLayers                          = 1;
   pdsch->transmissionScheme                 = 0;
   pdsch->refPoint                           = 0;
   pdsch->dmrs.dlDmrsSymbPos                 = 4; /* Bitmap value 00000000000100 i.e. using 3rd symbol for PDSCH DMRS */
   pdsch->dmrs.dmrsConfigType                = 0; /* type-1 */
   pdsch->dmrs.dlDmrsScramblingId            = pci;
   pdsch->dmrs.scid                          = 0;
   pdsch->dmrs.numDmrsCdmGrpsNoData          = 1;
   pdsch->dmrs.dmrsPorts                     = 0x0001;
   pdsch->dmrs.mappingType                   = DMRS_MAP_TYPE_A; /* Type-A */
   pdsch->dmrs.nrOfDmrsSymbols               = NUM_DMRS_SYMBOLS;
   pdsch->dmrs.dmrsAddPos                    = DMRS_ADDITIONAL_POS;

   pdsch->pdschFreqAlloc.resourceAllocType   = 1; /* RAT type-1 RIV format */
   /* the RB numbering starts from coreset0, and PDSCH is always above SSB */
   pdsch->pdschFreqAlloc.freqAlloc.startPrb  = offsetPointA + SCH_SSB_NUM_PRB;
   pdsch->pdschFreqAlloc.freqAlloc.numPrb    = schCalcNumPrb(tbSize,sib1SchCfg->sib1Mcs, NUM_PDSCH_SYMBOL);
   pdsch->pdschFreqAlloc.vrbPrbMapping       = 0; /* non-interleaved */
   pdsch->pdschTimeAlloc.rowIndex            = 1;
   /* This is Intel's requirement. PDSCH should start after PDSCH DRMS symbol */
   pdsch->pdschTimeAlloc.timeAlloc.startSymb = 3; /* spec-38.214, Table 5.1.2.1-1 */
   pdsch->pdschTimeAlloc.timeAlloc.numSymb   = NUM_PDSCH_SYMBOL;
   pdsch->beamPdschInfo.numPrgs              = 1;
   pdsch->beamPdschInfo.prgSize              = 1;
   pdsch->beamPdschInfo.digBfInterfaces      = 0;
   pdsch->beamPdschInfo.prg[0].pmIdx         = 0;
   pdsch->beamPdschInfo.prg[0].beamIdx[0]    = 0;
   pdsch->txPdschPower.powerControlOffset    = 0;
   pdsch->txPdschPower.powerControlOffsetSS  = 0;

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
uint8_t SchHdlCellCfgReq(Pst *pst, SchCellCfg *schCellCfg)
{
   uint8_t ret = ROK;
   SchCellCb *cellCb;
   SchCellCfgCfm schCellCfgCfm;
   Pst rspPst;
   Inst inst = pst->dstInst - SCH_INST_START; 
   uint8_t coreset0Idx = 0;
   uint8_t numRbs = 0;
   uint8_t offset = 0;
   uint8_t freqDomainResource[FREQ_DOM_RSRC_SIZE] = {0};
   SchPdschConfig pdschCfg;

#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTMAC -> ENTSCH : EVENT_SCH_CELL_CFG\n");
#endif 

   schInitCellCb(inst, schCellCfg);
   cellCb = schCb[inst].cells[inst]; //cells is of MAX_CELLS, why inst
   cellCb->macInst = pst->srcInst;

   /* derive the SIB1 config parameters */
   fillSchSib1Cfg(schCellCfg->numerology, schCellCfg->bandwidth, cellCb->numSlots,
	 &(schCellCfg->sib1SchCfg), schCellCfg->phyCellId,
	 schCellCfg->ssbSchCfg.ssbOffsetPointA);
   
   
   memcpy(&cellCb->cellCfg, schCellCfg, sizeof(SchCellCfg));
   schProcPagingCfg(cellCb);

   /* Fill coreset frequencyDomainResource bitmap */
   coreset0Idx = cellCb->cellCfg.schInitialDlBwp.pdcchCommon.commonSearchSpace.coresetId;
   numRbs = coresetIdxTable[coreset0Idx][1];
   offset = coresetIdxTable[coreset0Idx][3];
   fillCoresetFeqDomAllocMap(((cellCb->cellCfg.ssbSchCfg.ssbOffsetPointA - offset)/6), (numRbs/6), freqDomainResource);
   covertFreqDomRsrcMapToIAPIFormat(freqDomainResource, \
      cellCb->cellCfg.schInitialDlBwp.pdcchCommon.commonSearchSpace.freqDomainRsrc);

   /* Fill K0 - K1 table for common cfg*/ 
   BuildK0K1Table(cellCb, &cellCb->cellCfg.schInitialDlBwp.k0K1InfoTbl, true, cellCb->cellCfg.schInitialDlBwp.pdschCommon,
   pdschCfg, DEFAULT_UL_ACK_LIST_COUNT, defaultUlAckTbl);
   
   BuildK2InfoTable(cellCb, cellCb->cellCfg.schInitialUlBwp.puschCommon.timeDomRsrcAllocList,\
   cellCb->cellCfg.schInitialUlBwp.puschCommon.numTimeDomRsrcAlloc, &cellCb->cellCfg.schInitialUlBwp.msg3K2InfoTbl, \
   &cellCb->cellCfg.schInitialUlBwp.k2InfoTbl);
   /* Initializing global variables */
   cellCb->actvUeBitMap = 0;
   cellCb->boIndBitMap = 0;

   cellCb->cellCfg.schHqCfg.maxDlDataHqTx = SCH_MAX_NUM_DL_HQ_TX;
   cellCb->cellCfg.schHqCfg.maxMsg4HqTx = SCH_MAX_NUM_MSG4_TX;
   cellCb->cellCfg.schHqCfg.maxUlDataHqTx = SCH_MAX_NUM_UL_HQ_TX;
   cellCb->cellCfg.schRachCfg.maxMsg3Tx = SCH_MAX_NUM_MSG3_TX;

   cellCb->api = &schCb[inst].allApis[SCH_FCFS]; /* For FCFS */

   /* Fill and send Cell config confirm */
   memset(&rspPst, 0, sizeof(Pst));
   FILL_PST_SCH_TO_MAC(rspPst, pst->dstInst);
   rspPst.event = EVENT_SCH_CELL_CFG_CFM;

   schCellCfgCfm.cellId = schCellCfg->cellId; 
   schCellCfgCfm.rsp = RSP_OK;

   ret = MacMessageRouter(&rspPst, (void *)&schCellCfgCfm);
   return ret;

}

/*******************************************************************************
 *
 * @brief This function is used to send Slice Cfg rsp to MAC
 *
 * @details
 *
 *    Function : SchSendSliceCfgRspToMac
 *
 *    Functionality:
 *     function is used to send Slice Cfg rsp to MAC
 *
 * @params[in] Pst *pst, SchSliceCfgRsp sliceCfgRsp
 *
 * @return- void
 *
 * ********************************************************************************/
void SchSendSliceCfgRspToMac(Inst inst, SchSliceCfgRsp sliceCfgRsp)
{
   Pst rspPst;
   
   memset(&rspPst, 0, sizeof(Pst));
   FILL_PST_SCH_TO_MAC(rspPst, inst);
   rspPst.event = EVENT_SLICE_CFG_RSP_TO_MAC;
   
   MacMessageRouter(&rspPst, (void *)&sliceCfgRsp);
}

/*******************************************************************************
 *
 * @brief fill slice configuration response
 *
 * @details
 *
 *    Function : fillSliceCfgRsp
 *
 *    Functionality:
 *     fill slice configuration response
 *
 * @params[in] SchCellCb, SchSliceCfgReq, SchSliceCfgRsp,uint8_t  count
 *
 * @return
 *        ROK - Success
 *        RFAILED - Failure
 *
 * ********************************************************************************/
uint8_t fillSliceCfgRsp(bool sliceRecfg, SchSliceCfg *storedSliceCfg, SchCellCb *cellCb, SchSliceCfgReq *schSliceCfgReq, SchSliceCfgRsp *schSliceCfgRsp, uint8_t *count)
{
   bool sliceFound = false;
   uint8_t cfgIdx = 0, sliceIdx = 0;

   schSliceCfgRsp->numSliceCfgRsp  = schSliceCfgReq->numOfConfiguredSlice;
   SCH_ALLOC(schSliceCfgRsp->listOfSliceCfgRsp, schSliceCfgRsp->numSliceCfgRsp * sizeof(SliceRsp*));
   if(schSliceCfgRsp->listOfSliceCfgRsp == NULLP)
   {
      DU_LOG("\nERROR  --> SCH : Memory allocation failed at fillSliceCfgRsp");
      return RFAILED;
   }
   
   for(cfgIdx = 0; cfgIdx<schSliceCfgRsp->numSliceCfgRsp ; cfgIdx++)
   {
      sliceFound = false;
      /* Here comparing the slice cfg request with the slice stored in cellCfg */
      if(sliceRecfg != true)
      {
         for(sliceIdx = 0; sliceIdx<cellCb->cellCfg.plmnInfoList.numSliceSupport; sliceIdx++)
         {
            if(!memcmp(&schSliceCfgReq->listOfConfirguration[cfgIdx]->snssai, cellCb->cellCfg.plmnInfoList.snssai[sliceIdx], sizeof(Snssai)))
            {
               (*count)++;
               sliceFound = true;
               break;
            }
         }
      }
      else
      {
         /* Here comparing the slice cfg request with the slice stored in SchDb */
         if(storedSliceCfg->listOfConfirguration)
         {
            for(sliceIdx = 0; sliceIdx<storedSliceCfg->numOfSliceConfigured; sliceIdx++)
            {
               if(!memcmp(&schSliceCfgReq->listOfConfirguration[cfgIdx]->snssai, &storedSliceCfg->listOfConfirguration[sliceIdx]->snssai,\
                        sizeof(Snssai)))
               {
                  (*count)++;
                  sliceFound = true;
                  break;
               }
            }
         }
      }

      SCH_ALLOC(schSliceCfgRsp->listOfSliceCfgRsp[cfgIdx], sizeof(SliceRsp));
      if(schSliceCfgRsp->listOfSliceCfgRsp[cfgIdx] == NULLP)
      {
         DU_LOG("\nERROR  -->  SCH : Failed to allocate memory in fillSliceCfgRsp");
         return RFAILED;
      }

      
      schSliceCfgRsp->listOfSliceCfgRsp[cfgIdx]->snssai = schSliceCfgReq->listOfConfirguration[cfgIdx]->snssai;
      if(sliceFound == true)
         schSliceCfgRsp->listOfSliceCfgRsp[cfgIdx]->rsp    = RSP_OK;
      else
      {
         schSliceCfgRsp->listOfSliceCfgRsp[cfgIdx]->rsp    = RSP_NOK;
         schSliceCfgRsp->listOfSliceCfgRsp[cfgIdx]->cause  = SLICE_NOT_FOUND;
      }
   }
   return ROK;
}

/*******************************************************************************
 *
 * @brief This function is used to store the slice configuration Sch DB
 *
 * @details
 *
 *    Function : addSliceCfgInSchDb 
 *
 *    Functionality:
 *     function is used to store the slice configuration Sch DB
 *
 * @params[in] SchSliceCfg *storeSliceCfg, SchSliceCfgReq *cfgReq,
 * SchSliceCfgRsp cfgRsp, uint8_t count
 *
 * @return
 *        ROK - Success
 *        RFAILED - Failure
 *
 * ********************************************************************************/
uint8_t addSliceCfgInSchDb(SchSliceCfg *storeSliceCfg, SchSliceCfgReq *cfgReq, SchSliceCfgRsp cfgRsp, uint8_t count)
{
   uint8_t cfgIdx = 0, sliceIdx = 0; 
   
   if(count)
   {
      storeSliceCfg->numOfSliceConfigured = count;
      SCH_ALLOC(storeSliceCfg->listOfConfirguration, storeSliceCfg->numOfSliceConfigured * sizeof(SchRrmPolicyOfSlice*));
      if(storeSliceCfg->listOfConfirguration == NULLP)
      {
         DU_LOG("\nERROR  -->  SCH : Failed to allocate memory in addSliceCfgInSchDb");
         return RFAILED;
      }

      for(cfgIdx = 0; cfgIdx<storeSliceCfg->numOfSliceConfigured; cfgIdx++)
      {
         if(cfgRsp.listOfSliceCfgRsp[cfgIdx]->rsp == RSP_OK)
         {
            SCH_ALLOC(storeSliceCfg->listOfConfirguration[sliceIdx], sizeof(SchRrmPolicyOfSlice));
            if(storeSliceCfg->listOfConfirguration[sliceIdx] == NULLP)
            {
               DU_LOG("\nERROR  -->  SCH : Failed to allocate memory in addSliceCfgInSchDb");
               return RFAILED;
            }

            SCH_ALLOC(storeSliceCfg->listOfConfirguration[sliceIdx]->rrmPolicyRatioInfo, sizeof(SchRrmPolicyRatio));
            if(storeSliceCfg->listOfConfirguration[sliceIdx]->rrmPolicyRatioInfo == NULLP)
            {
               DU_LOG("\nERROR  -->  SCH : Failed to allocate memory in addSliceCfgInSchDb");
               return RFAILED;
            }

            memcpy(&storeSliceCfg->listOfConfirguration[sliceIdx]->snssai, &cfgReq->listOfConfirguration[sliceIdx]->snssai, sizeof(Snssai));
            memcpy(storeSliceCfg->listOfConfirguration[sliceIdx]->rrmPolicyRatioInfo, cfgReq->listOfConfirguration[sliceIdx]->rrmPolicyRatioInfo,
                      sizeof(SchRrmPolicyRatio));
            sliceIdx++;
         }
      }
   }
   return ROK;
}

/*******************************************************************************
 *
 * @brief This function is used to free the slice cfg and re cfg request pointer
 *
 * @details
 *
 *    Function : freeSchSliceCfgReq 
 *
 *    Functionality:
 *     function is used to free the slice cfg and re cfg request pointer
 *
 * @params[in] Pst *pst, SchSliceCfgReq *schSliceCfgReq
 *
 * @return
 *        ROK - Success
 *        RFAILED - Failure
 * ********************************************************************************/
void freeSchSliceCfgReq(SchSliceCfgReq *sliceCfgReq)
{
   uint8_t cfgIdx = 0;
   
   if(sliceCfgReq)
   {
      if(sliceCfgReq->numOfConfiguredSlice)
      {
         for(cfgIdx = 0; cfgIdx<sliceCfgReq->numOfConfiguredSlice; cfgIdx++)
         {
            if(sliceCfgReq->listOfConfirguration[cfgIdx])
            {
               SCH_FREE(sliceCfgReq->listOfConfirguration[cfgIdx]->rrmPolicyRatioInfo, sizeof(SchRrmPolicyRatio));
               SCH_FREE(sliceCfgReq->listOfConfirguration[cfgIdx], sizeof(SchRrmPolicyOfSlice));
            }
         }
         SCH_FREE(sliceCfgReq->listOfConfirguration, sliceCfgReq->numOfConfiguredSlice * sizeof(SchRrmPolicyOfSlice*));
      }
      SCH_FREE(sliceCfgReq, sizeof(SchSliceCfgReq));
   }
}
/*******************************************************************************
 *
 * @brief This function is used to store the slice configuration Sch DB
 *
 * @details
 *
 *    Function : MacSchSliceCfgReq 
 *
 *    Functionality:
 *     function is used to store the slice configuration Sch DB
 *
 * @params[in] Pst *pst, SchSliceCfgReq *schSliceCfgReq
 *
 * @return
 *        ROK - Success
 *        RFAILED - Failure
 *
 * ********************************************************************************/
uint8_t SchProcSliceCfgReq(Pst *pst, SchSliceCfgReq *schSliceCfgReq)
{
   uint8_t count = 0;
   Inst   inst = pst->dstInst - SCH_INST_START;
   SchSliceCfgRsp sliceCfgRsp;

   DU_LOG("\nINFO  -->  SCH : Received Slice Cfg request from MAC");
   if(schSliceCfgReq)
   {
      if(schSliceCfgReq->listOfConfirguration)
      {
         /* filling the slice configuration response of each slice */
         if(fillSliceCfgRsp(false, NULLP, schCb[inst].cells[0], schSliceCfgReq, &sliceCfgRsp, &count) != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Failed to fill the slice cfg rsp");
            return RFAILED;
         }

         if(addSliceCfgInSchDb(&schCb[inst].sliceCfg, schSliceCfgReq, sliceCfgRsp, count) != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Failed to add slice cfg in sch database");
            return RFAILED;
         }
         freeSchSliceCfgReq(schSliceCfgReq);
         SchSendSliceCfgRspToMac(inst, sliceCfgRsp);
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  SCH : Received SchSliceCfgReq is NULL");
   }
   return ROK;
}

/*******************************************************************************
 *
 * @brief This function is used to store the slice reconfiguration Sch DB
 *
 * @details
 *
 *    Function : modifySliceCfgInSchDb 
 *
 *    Functionality:
 *     function is used to store the slice re configuration Sch DB
 *
 * @params[in] Pst *pst, SchSliceCfgReq *schSliceCfgReq
 *
 * @return
 *        ROK - Success
 *        RFAILED - Failure
 *
 * ********************************************************************************/
uint8_t modifySliceCfgInSchDb(SchSliceCfg *storeSliceCfg, SchSliceRecfgReq *recfgReq, SchSliceRecfgRsp recfgRsp, uint8_t count)
{
   uint8_t cfgIdx = 0, sliceIdx = 0; 

   if(count)
   {
      if(storeSliceCfg->listOfConfirguration == NULLP)
      {
         DU_LOG("\nINFO  -->  SCH : Memory allocation failed in modifySliceCfgInSchDb");
         return RFAILED;
      }

      for(cfgIdx = 0; cfgIdx<recfgReq->numOfConfiguredSlice; cfgIdx++)
      {
         if(recfgRsp.listOfSliceCfgRsp[cfgIdx]->rsp == RSP_OK)
         {
            for(sliceIdx = 0; sliceIdx<storeSliceCfg->numOfSliceConfigured; sliceIdx++)
            {
               if(!memcmp(&storeSliceCfg->listOfConfirguration[sliceIdx]->snssai, &recfgReq->listOfConfirguration[cfgIdx]->snssai, sizeof(Snssai)))
               {
                  memcpy(storeSliceCfg->listOfConfirguration[sliceIdx]->rrmPolicyRatioInfo, recfgReq->listOfConfirguration[cfgIdx]->rrmPolicyRatioInfo,
                           sizeof(SchRrmPolicyRatio));
                  break;
               }
            }
         }
      }
   }
   freeSchSliceCfgReq(recfgReq);
   return ROK;
}
/*******************************************************************************
 *
 * @brief This function is used to send Slice re Cfg rsp to MAC
 *
 * @details
 *
 *    Function : SchSendSliceRecfgRspToMac
 *
 *    Functionality:
 *     function is used to send Slice re Cfg rsp to MAC
 *
 * @params[in] Pst *pst, SchSliceRecfgRsp schSliceRecfgRsp
 *
 * @return- void
 *
 * ********************************************************************************/
void SchSendSliceRecfgRspToMac(Inst inst, SchSliceRecfgRsp schSliceRecfgRsp)
{
   Pst rspPst;
   
   memset(&rspPst, 0, sizeof(Pst));
   FILL_PST_SCH_TO_MAC(rspPst, inst);
   rspPst.event = EVENT_SLICE_RECFG_RSP_TO_MAC;
   
   MacMessageRouter(&rspPst, (void *)&schSliceRecfgRsp);
}

/*******************************************************************************
 *
 * @brief This function is used to store the slice reconfiguration Sch DB
 *
 * @details
 *
 *    Function : SchSliceRecfgReq 
 *
 *    Functionality:
 *     function is used to store the slice re configuration Sch DB
 *
 * @params[in] Pst *pst, SchSliceRecfgReq *schSliceRecfgReq
 *
 * @return
 *        ROK - Success
 *        RFAILED - Failure
 *
 * ********************************************************************************/
uint8_t SchProcSliceRecfgReq(Pst *pst, SchSliceRecfgReq *schSliceRecfgReq)
{
   uint8_t count = 0;
   Inst   inst = pst->dstInst - SCH_INST_START;
   SchSliceRecfgRsp schSliceRecfgRsp;

   DU_LOG("\nINFO  -->  SCH : Received Slice ReCfg request from MAC");
   if(schSliceRecfgReq)
   {
      if(schSliceRecfgReq->listOfConfirguration)
      {
         /* filling the slice configuration response of each slice */
         if(fillSliceCfgRsp(true, &schCb[inst].sliceCfg, NULLP, schSliceRecfgReq, &schSliceRecfgRsp, &count) != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Failed to fill sch slice cfg response");
            return RFAILED;
         }
         
         /* Modify the slice configuration stored in schCb */
         if(modifySliceCfgInSchDb(&schCb[inst].sliceCfg, schSliceRecfgReq, schSliceRecfgRsp, count) != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Failed to modify slice cfg of SchDb");
            return RFAILED;
         }
         SchSendSliceRecfgRspToMac(inst, schSliceRecfgRsp);
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  SCH : Received SchSliceRecfgReq is NULL");
   }
   return ROK;
}

/****************************************************************************
 *
 * @brief Stores the Paging Configuration from DU APP in CellCb 
 *
 * @details
 *
 *    Function : schProcPagingParam
 *
 *    Functionality:
 *          Process the Paging Configuration when FirstPDCCHMonitoring for
 *          Paging Ocassion is not present.
 *
 *          As per 38.304 Sec 7.1,
 *          "When firstPDCCH-MonitoringOccasionOfPO is present, the
 *          starting PDCCH monitoring occasion number of (i_s + 1)th PO is the
 *          (i_s + 1)th value of the firstPDCCHMonitoringOccasionOfPO
 *          parameter; otherwise, it is equal to i_s * S."
 *          "S = number of actual transmitted SSBs determined according 
 *              to ssb-PositionsInBurst in SIB1"
 *
 * @params[in] SchCellCb *cell 
 *       
 * @return void 
 *        
 *************************************************************************/
void schProcPagingCfg(SchCellCb *cell)
{
   PageCfg *pageCfgRcvd = NULL;
   uint8_t i_sIdx = 0;

   pageCfgRcvd = &(cell->cellCfg.sib1SchCfg.pageCfg);

   if(pageCfgRcvd->poPresent == TRUE)
   {
      /*Fetching first Pdcch Monitoring Occasion for SFN (i_s + 1)th*/
      for(i_sIdx = 0; i_sIdx < pageCfgRcvd->numPO; i_sIdx++)
      {
         cell->pageCb.pagMonOcc[i_sIdx].pagingOccSlot = pageCfgRcvd->pagingOcc[i_sIdx] / MAX_SYMB_PER_SLOT ;
         if ((pageCfgRcvd->pagingOcc[i_sIdx] % MAX_SYMB_PER_SLOT) != 0 )
         {
            cell->pageCb.pagMonOcc[i_sIdx].pagingOccSlot++;
         }

         cell->pageCb.pagMonOcc[i_sIdx].frameOffset = 0;

      }
   }
   else
   {
      schCfgPdcchMonOccOfPO(cell);                  
   }
}

/****************************************************************************
 *
 * @brief Calculate PO if not present in Configuration 
 *
 * @details
 *
 *    Function : schCfgPdcchMonOccOfPO
 *
 *    Functionality: In this function, PO are calculated i_s * S because
 *    FirstPDCCHMonitoring_ForPO is not present.
 *
 * @params[in] SchCellCb *cellCb
 *       
 * @return void 
 *        
 *************************************************************************/
void schCfgPdcchMonOccOfPO(SchCellCb *cell)
{
   uint8_t         cnt = 0, incr = 1, i_sIdx = 0, frameOffSet = 0;
   uint8_t         nsValue = cell->cellCfg.sib1SchCfg.pageCfg.numPO;
   uint8_t         totalNumSsb = cell->cellCfg.ssbSchCfg.totNumSsb;
   SlotTimingInfo  tmpTimingInfo, pdcchTime; 

   /*Starting with First Sfn and slot*/
   tmpTimingInfo.sfn = 0;
   tmpTimingInfo.slot = 0;

   pdcchTime = tmpTimingInfo;

   while(i_sIdx < nsValue)
   {
      /*Increment frame Offset if PO falls on next SFN*/
      if(pdcchTime.sfn != tmpTimingInfo.sfn)
      {
         frameOffSet++;
      }
      pdcchTime = tmpTimingInfo;
      schIncrSlot(&(tmpTimingInfo), incr, cell->numSlots);

      if (i_sIdx == 0)
      {
         cell->pageCb.pagMonOcc[i_sIdx].pagingOccSlot = pdcchTime.slot;
         cell->pageCb.pagMonOcc[i_sIdx].frameOffset = frameOffSet;
         i_sIdx++;
      }
      else
      {
         cnt++;
         if((cnt == totalNumSsb) && (i_sIdx < MAX_PO_PER_PF)) 
         {
            cell->pageCb.pagMonOcc[i_sIdx].pagingOccSlot = pdcchTime.slot;
            cell->pageCb.pagMonOcc[i_sIdx].frameOffset = frameOffSet;
            cnt = 0;
            i_sIdx++;
         }
      }
   }
}


/***********************************************************
 *
 *     Func : SchFillCfmPst 
 *        
 *
 *     Desc : Fills the Confirmation Post Structure cfmPst using the reqPst 
 *            and the cfm->hdr.response.
 *            
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : rg_sch_lmm.c 
 *
 **********************************************************/
Void SchFillCfmPst
(
Pst           *reqPst,
Pst           *cfmPst,
RgMngmt       *cfm
)
{
   Inst inst;

   inst = (reqPst->dstInst - SCH_INST_START);

   cfmPst->srcEnt    = ENTMAC;
   cfmPst->srcInst   = (Inst) 1;
   cfmPst->srcProcId = schCb[inst].schInit.procId;
   cfmPst->dstEnt    = ENTMAC;
   cfmPst->dstInst   = (Inst) 0;
   cfmPst->dstProcId = reqPst->srcProcId;

   cfmPst->selector  = cfm->hdr.response.selector;
   cfmPst->region    = cfm->hdr.response.mem.region;
   cfmPst->pool      = cfm->hdr.response.mem.pool;

   return;
}

/**********************************************************************
  End of file
 **********************************************************************/
