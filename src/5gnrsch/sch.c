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
#include "sch_utils.h"
#include "sch_fcfs.h"
#include "sch_slice_based.h"

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
    schSliceBasedAllApisInit(&schCb[inst].allApis[SCH_SLICE_BASED]);
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
 *@brief Fills the slotCfg from CellCfg
 *
 * @details
 * 
 * Function : schFillSlotConfig 
 * 
 * This API Fills the slotCfg from CellCfg
 * 
 * @param[in] SchCellCb *cell, TDDCfg tddCfg
 * @return  void
 * **/
void schFillSlotConfig(SchCellCb *cell, TDDCfg tddCfg)
{
   uint8_t slotIdx = 0, symbolIdx = 0;

   for(slotIdx =0 ;slotIdx < MAX_TDD_PERIODICITY_SLOTS; slotIdx++) 
   {
      for(symbolIdx = 0; symbolIdx < MAX_SYMB_PER_SLOT; symbolIdx++)
      {
         /*Fill Full-DL Slots as well as DL symbols ini 1st Flexi Slo*/
         if(slotIdx < tddCfg.nrOfDlSlots || \
               (slotIdx == tddCfg.nrOfDlSlots && symbolIdx < tddCfg.nrOfDlSymbols)) 
         {
              cell->slotCfg[slotIdx][symbolIdx] = DL_SYMBOL; 
         }

         /*Fill Full-FLEXI SLOT and as well as Flexi Symbols in 1 slot preceding FULL-UL slot*/ 
         else if(slotIdx < (MAX_TDD_PERIODICITY_SLOTS - tddCfg.nrOfUlSlots -1) ||  \
               (slotIdx == (MAX_TDD_PERIODICITY_SLOTS - tddCfg.nrOfUlSlots -1) && \
                symbolIdx < (MAX_SYMB_PER_SLOT - tddCfg.nrOfUlSymbols)))
         {
              cell->slotCfg[slotIdx][symbolIdx] = FLEXI_SYMBOL; 
         }
         /*Fill Partial UL symbols and Full-UL slot*/
         else
         {
              cell->slotCfg[slotIdx][symbolIdx] = UL_SYMBOL; 
         }
      }
   }
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
   schFillSlotConfig(cell, schCellCfg->tddCfg);
   for(slotIdx = cell->numSlotsInPeriodicity-1; slotIdx >= 0; slotIdx--)
   {
      symbIdx = 0;
      /* If the first and last symbol are the same, the entire slot is the same type */
      if((cell->slotCfg[slotIdx][symbIdx] == cell->slotCfg[slotIdx][MAX_SYMB_PER_SLOT-1]) &&
              cell->slotCfg[slotIdx][symbIdx] != FLEXI_SYMBOL)
      {
         switch(cell->slotCfg[slotIdx][symbIdx])
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

   scs = cellCb->cellCfg.scsCommon;

   memset(ssbStartSymbArr, 0, sizeof(SCH_MAX_SSB_BEAM));
   symbIdx = 0;
   /* Determine value of "n" based on Section 4.1 of 3GPP TS 38.213 */
   switch(scs)
   {
      case SCS_15KHZ:
         {
            if(cellCb->cellCfg.ssbFrequency <= 300000)
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
            if(cellCb->cellCfg.ssbFrequency <= 300000)
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
uint8_t fillSchSib1Cfg(uint8_t mu, uint8_t bandwidth, uint8_t numSlots,SchPdcchConfigSib1 *pdcchCfgSib1,\
               SchSib1Cfg *sib1SchCfg, uint16_t pci, uint8_t offsetPointA, uint16_t sib1PduLen)
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
   PdcchCfg *pdcch;
   PdschCfg *pdsch;
   BwpCfg *bwp;

   pdcch = &(sib1SchCfg->sib1PdcchCfg);
   bwp = &(sib1SchCfg->bwp);

   coreset0Idx     = pdcchCfgSib1->coresetZeroIndex;
   searchSpace0Idx = pdcchCfgSib1->searchSpaceZeroIndex;

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
   pdcch->dci.txPdcchPower.beta_pdcch_1_0= 0;
   pdcch->dci.txPdcchPower.powerControlOffsetSS = 0;
   /* Storing pdschCfg pointer here. Required to access pdsch config while
      fillig up pdcch pdu */
   pdsch = &pdcch->dci.pdschCfg; 

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
      pdsch->codeword[cwCount].mcsIndex = DEFAULT_MCS;
      pdsch->codeword[cwCount].mcsTable = 0; /* notqam256 */
      pdsch->codeword[cwCount].rvIndex = 0;
      tbSize = schCalcTbSize(sib1PduLen + TX_PAYLOAD_HDR_LEN);
      pdsch->codeword[cwCount].tbSize = tbSize;
   }
   pdsch->dataScramblingId                   = pci;
   pdsch->numLayers                          = 1;
   pdsch->transmissionScheme                 = 0;
   pdsch->refPoint                           = 0;
   pdsch->dmrs.dlDmrsSymbPos                 = DL_DMRS_SYMBOL_POS; 
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
   pdsch->pdschFreqAlloc.startPrb  = offsetPointA + SCH_SSB_NUM_PRB;
   pdsch->pdschFreqAlloc.numPrb    = schCalcNumPrb(tbSize, DEFAULT_MCS, NUM_PDSCH_SYMBOL);
   pdsch->pdschFreqAlloc.vrbPrbMapping       = 0; /* non-interleaved */
   pdsch->pdschTimeAlloc.rowIndex            = 1;
   /* This is Intel's requirement. PDSCH should start after PDSCH DRMS symbol */
   pdsch->pdschTimeAlloc.startSymb = 3; /* spec-38.214, Table 5.1.2.1-1 */
   pdsch->pdschTimeAlloc.numSymb   = NUM_PDSCH_SYMBOL;
   pdsch->beamPdschInfo.numPrgs              = 1;
   pdsch->beamPdschInfo.prgSize              = 1;
   pdsch->beamPdschInfo.digBfInterfaces      = 0;
   pdsch->beamPdschInfo.prg[0].pmIdx         = 0;
   pdsch->beamPdschInfo.prg[0].beamIdx[0]    = 0;
   pdsch->txPdschPower.powerControlOffset    = 0;
   pdsch->txPdschPower.powerControlOffsetSS  = 0;

   return ROK;
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
uint8_t SchProcCellCfgReq(Pst *pst, SchCellCfg *schCellCfg)
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

   schInitCellCb(inst, schCellCfg);
   cellCb = schCb[inst].cells[inst]; //cells is of MAX_CELLS, why inst
   cellCb->macInst = pst->srcInst;

   /* derive the SIB1 config parameters */
   ret = fillSchSib1Cfg(schCellCfg->numerology, schCellCfg->dlBandwidth, cellCb->numSlots,
	 &(schCellCfg->pdcchCfgSib1), &(cellCb->sib1SchCfg), schCellCfg->phyCellId,
	 schCellCfg->dlCfgCommon.schFreqInfoDlSib.offsetToPointA, schCellCfg->sib1PduLen);
   
   if(ret != ROK)
   {
      DU_LOG("\nERROR --> SCH : Failed to fill sib1 configuration");
      return RFAILED;
   }
   memcpy(&cellCb->cellCfg, schCellCfg, sizeof(SchCellCfg));
   schProcPagingCfg(cellCb);

   /* Fill coreset frequencyDomainResource bitmap */
   coreset0Idx = cellCb->cellCfg.dlCfgCommon.schInitialDlBwp.pdcchCommon.commonSearchSpace.coresetId;
   numRbs = coresetIdxTable[coreset0Idx][1];
   offset = coresetIdxTable[coreset0Idx][3];
   fillCoresetFeqDomAllocMap(((cellCb->cellCfg.dlCfgCommon.schFreqInfoDlSib.offsetToPointA - offset)/6), \
                                  (numRbs/6), freqDomainResource);
   covertFreqDomRsrcMapToIAPIFormat(freqDomainResource, \
      cellCb->cellCfg.dlCfgCommon.schInitialDlBwp.pdcchCommon.commonSearchSpace.freqDomainRsrc);

   /* Fill K0 - K1 table for common cfg*/ 
   BuildK0K1Table(cellCb, &cellCb->k0K1InfoTbl, true, cellCb->cellCfg.dlCfgCommon.schInitialDlBwp.pdschCommon,
                   pdschCfg, DEFAULT_UL_ACK_LIST_COUNT, defaultUlAckTbl);
   
   BuildK2InfoTable(cellCb, cellCb->cellCfg.ulCfgCommon.schInitialUlBwp.puschCommon.timeDomRsrcAllocList,\
   cellCb->cellCfg.ulCfgCommon.schInitialUlBwp.puschCommon.numTimeDomRsrcAlloc, &cellCb->msg3K2InfoTbl, \
   &cellCb->k2InfoTbl);
   
   /*As per Spec 38.211, Sec 6.3.3.2; RootSeq Len(Lra) where Lra=839 or Lra=139,
    *depending on the PRACH preamble format as given by Tables 6.3.3.1-1 and 6.3.3.1-2.*/
   if(prachCfgIdxTable[cellCb->cellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.prachCfgGeneric.prachCfgIdx][0] <= 3)
   {
      cellCb->cellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.rootSeqLen = ROOT_SEQ_LEN_1;
   }
   else
   {
      cellCb->cellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.rootSeqLen = ROOT_SEQ_LEN_2;
   }
   /* Initializing global variables */
   cellCb->actvUeBitMap = 0;
   cellCb->boIndBitMap = 0;

   cellCb->schHqCfg.maxDlDataHqTx = SCH_MAX_NUM_DL_HQ_TX;
   cellCb->schHqCfg.maxMsg4HqTx = SCH_MAX_NUM_MSG4_TX;
   cellCb->schHqCfg.maxUlDataHqTx = SCH_MAX_NUM_UL_HQ_TX;
   cellCb->maxMsg3Tx = SCH_MAX_NUM_MSG3_TX;

   cellCb->schAlgoType = SCH_FCFS;
   cellCb->api = &schCb[inst].allApis[cellCb->schAlgoType]; /* For FCFS */
   cellCb->api->SchCellCfgReq(cellCb);
   
   /* Fill and send Cell config confirm */
   memset(&rspPst, 0, sizeof(Pst));
   FILL_PST_SCH_TO_MAC(rspPst, pst->dstInst);
   rspPst.event = EVENT_SCH_CELL_CFG_CFM;

   schCellCfgCfm.cellId = schCellCfg->cellId; 
   schCellCfgCfm.rsp = RSP_OK;

   ret = MacMessageRouter(&rspPst, (void *)&schCellCfgCfm);
   return ret;

}

/*******************************************************************
 *
 * @brief Fill and send Cell delete response to MAC
 *
 * @details
 *
 *    Function :  SchSendCellDeleteRspToMac
 *
 *    Functionality: Fill and send Cell delete response to MAC
 *
 * @params[in] SchCellDelete  *ueDelete, Inst inst, SchMacRsp result
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t SchSendCellDeleteRspToMac(SchCellDeleteReq  *ueDelete, Inst inst, SchMacRsp result)
{
   Pst rspPst;
   uint8_t ret=0;
   
   SchCellDeleteRsp  delRsp;

   DU_LOG("\nINFO   --> SCH : Filling Cell Delete response");
   memset(&delRsp, 0, sizeof(SchCellDeleteRsp));
   delRsp.cellId = ueDelete->cellId;
   delRsp.rsp = result;

   /* Filling response post */
   memset(&rspPst, 0, sizeof(Pst));
   FILL_PST_SCH_TO_MAC(rspPst, inst);
   rspPst.event = EVENT_CELL_DELETE_RSP_TO_MAC;
   ret = MacMessageRouter(&rspPst, (void *)&delRsp);
   if(ret == RFAILED)
   {
      DU_LOG("\nERROR  -->  SCH : SchSendCellDeleteRspToMac(): failed to send the Cell Delete response");
      return ret;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Function for cellCb Deletion 
 *
 * @details
 *
 *    Function : deleteSchCellCb 
 *
 *    Functionality: Function for cellCb Deletion 
 *
 * @params[in] SchCellDelete  *cellDelete
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void deleteSchCellCb(SchCellCb *cellCb)
{
   uint8_t sliceIdx=0, slotIdx=0, plmnIdx = 0;
   CmLListCp *list=NULL;
   CmLList *node=NULL, *next=NULL;
   SchPageInfo *tempNode = NULLP;

   if(cellCb->schDlSlotInfo)
   {
      for(slotIdx=0; slotIdx<cellCb->numSlots; slotIdx++)
      {
         list = &cellCb->schDlSlotInfo[slotIdx]->prbAlloc.freePrbBlockList;
         node = list->first;
         while(node)
         {
            next = node->next;
            SCH_FREE(node->node, sizeof(FreePrbBlock));
            deleteNodeFromLList(list, node);
            node = next;
         }
         SCH_FREE(cellCb->schDlSlotInfo[slotIdx], sizeof(SchDlSlotInfo));
      }
      SCH_FREE(cellCb->schDlSlotInfo, cellCb->numSlots *sizeof(SchDlSlotInfo*));
   }

   if(cellCb->schUlSlotInfo)
   {
      for(slotIdx=0; slotIdx<cellCb->numSlots; slotIdx++)
      {
         list = &cellCb->schUlSlotInfo[slotIdx]->prbAlloc.freePrbBlockList;
         node = list->first;
         while(node)
         {
            next = node->next;
            SCH_FREE(node->node, sizeof(FreePrbBlock));
            deleteNodeFromLList(list, node);
            node = next;
         }
         SCH_FREE(cellCb->schUlSlotInfo[slotIdx], sizeof(SchUlSlotInfo));  
      }
      SCH_FREE(cellCb->schUlSlotInfo,  cellCb->numSlots * sizeof(SchUlSlotInfo*));
   }

   for(plmnIdx = 0; plmnIdx < MAX_PLMN; plmnIdx++)
   {
      if(cellCb->cellCfg.plmnInfoList[plmnIdx].snssai)
      {
         for(sliceIdx=0; sliceIdx<cellCb->cellCfg.plmnInfoList[plmnIdx].numSliceSupport; sliceIdx++)
         {
            SCH_FREE(cellCb->cellCfg.plmnInfoList[plmnIdx].snssai[sliceIdx], sizeof(Snssai));
         }
         SCH_FREE(cellCb->cellCfg.plmnInfoList[plmnIdx].snssai, cellCb->cellCfg.plmnInfoList[plmnIdx].numSliceSupport*sizeof(Snssai*));
      }
   }

   for(uint16_t idx =0; idx<MAX_SFN; idx++)
   {
      list = &cellCb->pageCb.pageIndInfoRecord[idx];
      node = list->first;
      while(node)
      {
         next = node->next;
         if(node->node)
         {
            tempNode = (SchPageInfo*)(node->node);
            SCH_FREE(tempNode->pagePdu, tempNode->msgLen);
            SCH_FREE(node->node,  sizeof(SchPageInfo));
         }
         deleteNodeFromLList(list, node);
         node = next;
      }
   }

   cellCb->api->SchCellDeleteReq(cellCb);

   memset(cellCb, 0, sizeof(SchCellCb));
}

/*******************************************************************
 *
 * @brief Function for cell Delete request from MAC to SCH
 *
 * @details
 *
 *    Function : SchProcCellDeleteReq
 *
 *    Functionality: Function for cell Delete request from MAC to SCH
 *
 * @params[in] Pst *pst, SchCellDelete  *cellDelete
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t SchProcCellDeleteReq(Pst *pst, SchCellDeleteReq  *cellDelete)
{
   uint8_t   cellIdx=0, ret = RFAILED;
   Inst      inst = pst->dstInst - SCH_INST_START;
   SchMacRsp result= RSP_OK;
   
   if(!cellDelete)
   {
      DU_LOG("\nERROR  -->  SCH : SchProcCellDeleteReq(): Ue Delete request failed");
   }
   else
   {
      GET_CELL_IDX(cellDelete->cellId, cellIdx);
      if(schCb[inst].cells[cellIdx] == NULLP)
      { 
         DU_LOG("\nERROR  -->  SCH : SchProcCellDeleteReq(): cell Id[%d] is not available", cellDelete->cellId);
         result = RSP_NOK;
      }
      else
      {
         if(schCb[inst].cells[cellIdx]->cellId == cellDelete->cellId)
         {
            deleteSchCellCb(schCb[inst].cells[cellIdx]);
            result = RSP_OK;
            ret = ROK;
            SCH_FREE(schCb[inst].cells[cellIdx], sizeof(SchCellCb));
            DU_LOG("\nINFO   -->  SCH : Sending Cell Delete response to MAC");
         }
         else
         {
            DU_LOG("\nERROR  -->  SCH : SchProcCellDeleteReq(): cell Id[%d] is not available",cellDelete->cellId);
            result = RSP_NOK;
         }
      }

      if(SchSendCellDeleteRspToMac(cellDelete, inst, result)!=ROK)
      {
         DU_LOG("\nERROR  -->  SCH : SchProcCellDeleteReq(): failed to send Cell Delete response");
         ret =  RFAILED;
      }
   }
   return ret;   
}

/*******************************************************************
 *
 * @brief Processes DL RLC BO info from MAC
 *
 * @details
 *
 *    Function : SchProcDlRlcBoInfo
 *
 *    Functionality:
 *       Processes DL RLC BO info from MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t SchProcDlRlcBoInfo(Pst *pst, DlRlcBoInfo *dlBoInfo)
{
   uint8_t  lcId = 0;
   uint16_t ueId = 0;
   bool isLcIdValid = false;
   SchUeCb *ueCb = NULLP;
   SchCellCb *cell = NULLP;
   Inst  inst = pst->dstInst-SCH_INST_START;   

   DU_LOG("\nDEBUG  -->  SCH : Received RLC BO Status indication LCId [%d] BO [%d]", dlBoInfo->lcId, dlBoInfo->dataVolume);
   cell = schCb[inst].cells[inst];

   if(cell == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : SchProcDlRlcBoInfo(): Cell does not exists");
      return RFAILED;
   }

   GET_UE_ID(dlBoInfo->crnti, ueId);
   ueCb = &cell->ueCb[ueId-1];
   if(ueCb->ueCfg.dataTransmissionAction == STOP_DATA_TRANSMISSION)
   {
      DU_LOG("INFO  --> SCH : DL Data transmission not allowed for UE %d", ueCb->ueCfg.ueId);
      return ROK;
   }
   
   lcId  = dlBoInfo->lcId;
   CHECK_LCID(lcId, isLcIdValid);
   if(isLcIdValid == FALSE)
   {
      DU_LOG("ERROR --> SCH: LCID:%d is not valid", lcId);
      return RFAILED;
   }

   /*Expected when theres a case of Retransmission Failure or Resetablishment
    *By Zero BO, the RLC is informing that previous data can be cleared out
    *Thus clearing out the LC from the Lc priority list*/
   if(dlBoInfo->dataVolume == 0)
   {
      /* TODO : Check the LC is Dedicated or default and accordingly LCList
       * will be used*/
      return ROK;
   }

   if(lcId == SRB0_LCID)
   {
      cell->raCb[ueId -1].msg4recvd = true;
      cell->raCb[ueId -1].dlMsgPduLen = dlBoInfo->dataVolume;      
   }
   else
   {
      /* TODO : These part of changes will be corrected during DL scheduling as
       * per K0 - K1 -K2 */
      SET_ONE_BIT(ueId, cell->boIndBitMap);
      if(ueCb->dlInfo.dlLcCtxt[lcId].lcId == lcId)
      {
         ueCb->dlInfo.dlLcCtxt[lcId].bo = dlBoInfo->dataVolume;
      }
      else
      {
         DU_LOG("ERROR --> SCH: LCID:%d is not configured in SCH Cb",lcId);
         return RFAILED;
      }
   }
   /* Adding UE Id to list of pending UEs to be scheduled */
   cell->api->SchDlRlcBoInfo(cell, ueId);
   return ROK;
}

/*******************************************************************
 *
 * @brief Processes BSR indiation from MAC
 *
 * @details
 *
 *    Function : SchProcBsr
 *
 *    Functionality:
 *       Processes DL BSR from MAC
 *
 * @params[in]    Pst pst
 *                UlBufferStatusRptInd bsrInd
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t SchProcBsr(Pst *pst, UlBufferStatusRptInd *bsrInd)
{
   Inst           schInst       = pst->dstInst-SCH_INST_START;
   SchCellCb      *cellCb       = NULLP;
   SchUeCb        *ueCb         = NULLP;
   uint8_t        lcgIdx = 0;

   DU_LOG("\nDEBUG  -->  SCH : Received BSR");
   if(bsrInd == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : BSR Ind is empty");
      return RFAILED;
   }
   cellCb = schCb[schInst].cells[schInst];
   if(cellCb == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : CellCb is empty");
      return RFAILED;
   }
   ueCb = schGetUeCb(cellCb, bsrInd->crnti);

   if(ueCb == NULLP)
   {
      DU_LOG("\nERROR  -->  SCH : UeCB is empty");
      return RFAILED;
   }

   if(ueCb->ueCfg.dataTransmissionAction == STOP_DATA_TRANSMISSION)
   {
      DU_LOG("\nINFO --> SCH: UL Data transmission not allowed for UE %d", ueCb->ueCfg.ueId);
      return ROK;
   }
   
   ueCb->bsrRcvd = true;
   /* store dataVolume per lcg in uecb */
   for(lcgIdx = 0; lcgIdx < bsrInd->numLcg; lcgIdx++)
   {
      ueCb->bsrInfo[bsrInd->dataVolInfo[lcgIdx].lcgId].priority = 1; //TODO: determining LCG priority?
      ueCb->bsrInfo[bsrInd->dataVolInfo[lcgIdx].lcgId].dataVol = bsrInd->dataVolInfo[lcgIdx].dataVol;
   }
   
   /* Adding UE Id to list of pending UEs to be scheduled */
   cellCb->api->SchBsr(cellCb, ueCb->ueId);
   return ROK;
}

/*******************************************************************
 *
 * @brief Processes SR UCI indication from MAC 
 *
 * @details
 *
 *    Function : SchProcSrUciInd
 *
 *    Functionality:
 *      Processes SR UCI indication from MAC
 *
 * @params[in] Post structure
 *             UCI Indication
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t SchProcSrUciInd(Pst *pst, SrUciIndInfo *uciInd)
{
   Inst  inst = pst->dstInst-SCH_INST_START;

   SchUeCb   *ueCb; 
   SchCellCb *cellCb = schCb[inst].cells[inst];

   DU_LOG("\nDEBUG  -->  SCH : Received SR");

   ueCb = schGetUeCb(cellCb, uciInd->crnti);
   
   if(ueCb->state == SCH_UE_STATE_INACTIVE)
   {
      DU_LOG("\nERROR  -->  SCH : Crnti %d is inactive", uciInd->crnti);
      return ROK;  
   }
   if(ueCb->ueCfg.dataTransmissionAction == STOP_DATA_TRANSMISSION)
   {
      DU_LOG("\nINFO --> SCH: UL Data transmission not allowed for UE %d", ueCb->ueCfg.ueId);
      return ROK;
   }
   if(uciInd->numSrBits)
   {
      ueCb->srRcvd = true;      
      /* Adding UE Id to list of pending UEs to be scheduled */
      cellCb->api->SchSrUciInd(cellCb, ueCb->ueId);
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Processes DL HARQ indication from MAC 
 *
 * @details
 *
 *    Function : SchProcDlHarqInd
 *
 *    Functionality:
 *      Processes DL HARQ indication from MAC
 *
 * @params[in] Post structure
 *             DL HARQ Indication
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t SchProcDlHarqInd(Pst *pst, DlHarqInd *dlHarqInd)
{
   Inst  inst = pst->dstInst-SCH_INST_START;
   SchUeCb   *ueCb;
   SchCellCb *cellCb = schCb[inst].cells[inst];

   DU_LOG("\nDEBUG  -->  SCH : Received HARQ");

   ueCb = schGetUeCb(cellCb, dlHarqInd->crnti);

   if(ueCb->state == SCH_UE_STATE_INACTIVE)
   {
      DU_LOG("\nERROR  -->  SCH : Crnti %d is inactive", dlHarqInd->crnti);
      return ROK;
   }

   schUpdateHarqFdbk(ueCb, dlHarqInd->numHarq, dlHarqInd->harqPayload, &dlHarqInd->slotInd);

   return ROK;
}

/*******************************************************************
 *
 * @brief Allocates requested PRBs for DL
 *
 * @details
 *
 *    Function : allocatePrbDl
 *
 *    Functionality:
 *      Allocates requested PRBs in DL
 *      Keeps track of allocated PRB (using bitmap) and remaining PRBs
 *
 * @params[in] prbAlloc table
 *             Start symbol
 *             Number of symbols
 *             Start PRB
 *             Number of PRBs
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t allocatePrbDl(SchCellCb *cell, SlotTimingInfo slotTime, \
   uint8_t startSymbol, uint8_t symbolLength, uint16_t *startPrb, uint16_t numPrb)
{
   uint8_t        symbol = 0;
   uint16_t       broadcastPrbStart=0, broadcastPrbEnd=0;
   FreePrbBlock   *freePrbBlock = NULLP;
   CmLList        *freePrbNode = NULLP;
   PduTxOccsaion  ssbOccasion=0, sib1Occasion=0;
   SchDlSlotInfo  *schDlSlotInfo = cell->schDlSlotInfo[slotTime.slot];
   SchPrbAlloc    *prbAlloc = &schDlSlotInfo->prbAlloc;

   /* If startPrb is set to MAX_NUM_RB, it means startPrb is not known currently.
    * Search for an appropriate location in PRB grid and allocate requested resources */
   if(*startPrb == MAX_NUM_RB)
   {
      /* Check if SSB/SIB1 is also scheduled in this slot  */
      ssbOccasion = schCheckSsbOcc(cell, slotTime);
      sib1Occasion = schCheckSib1Occ(cell, slotTime);

      if(ssbOccasion && sib1Occasion)
      {
         broadcastPrbStart = cell->cellCfg.dlCfgCommon.schFreqInfoDlSib.offsetToPointA; 
         broadcastPrbEnd = broadcastPrbStart + SCH_SSB_NUM_PRB + cell->sib1SchCfg.sib1PdcchCfg.dci.pdschCfg.pdschFreqAlloc.numPrb -1;
      }
      else if(ssbOccasion)
      {
         broadcastPrbStart = cell->cellCfg.dlCfgCommon.schFreqInfoDlSib.offsetToPointA;
         broadcastPrbEnd = broadcastPrbStart + SCH_SSB_NUM_PRB -1;
      }
      else if(sib1Occasion)
      {
         broadcastPrbStart = cell->sib1SchCfg.sib1PdcchCfg.dci.pdschCfg.pdschFreqAlloc.startPrb;
         broadcastPrbEnd = broadcastPrbStart + cell->sib1SchCfg.sib1PdcchCfg.dci.pdschCfg.pdschFreqAlloc.numPrb -1;
      }

      /* Iterate through all free PRB blocks */
      freePrbNode = prbAlloc->freePrbBlockList.first; 
      while(freePrbNode)
      {
         freePrbBlock = (FreePrbBlock *)freePrbNode->node; 

         /* If broadcast message is scheduled in this slot, then check if its PRBs belong to the current free block.
          * Since SSB/SIB1 PRB location is fixed, these PRBs cannot be allocated to other message in same slot */
         if((ssbOccasion || sib1Occasion) && 
            ((broadcastPrbStart >= freePrbBlock->startPrb) && (broadcastPrbStart <= freePrbBlock->endPrb)) && \
            ((broadcastPrbEnd >= freePrbBlock->startPrb) && (broadcastPrbEnd <= freePrbBlock->endPrb)))
         {
            /* Implmentation is done such that highest-numbered free-RB is allocated first */ 
            if((freePrbBlock->endPrb > broadcastPrbEnd) && ((freePrbBlock->endPrb - broadcastPrbEnd) >= numPrb))
            {
               /* If sufficient free PRBs are available above bradcast message then,
                * endPrb = freePrbBlock->endPrb
                * startPrb = endPrb - numPrb +1;
                */
               *startPrb = freePrbBlock->endPrb - numPrb +1;
               break;
            }
            else if((broadcastPrbStart > freePrbBlock->startPrb) && ((broadcastPrbStart - freePrbBlock->startPrb) >= numPrb))
            {
               /* If free PRBs are available below broadcast message then,
                * endPrb = broadcastPrbStart - 1
                * startPrb = endPrb - numPrb +1
                */
               *startPrb = broadcastPrbStart - numPrb; 
               break;
            }
            else
            {
               freePrbNode = freePrbNode->next;
               continue;
            }
         }
         else
         {
            /* Check if requested number of blocks can be allocated from the current block */ 
            if (freePrbBlock->numFreePrb < numPrb)
            {
               freePrbNode = freePrbNode->next;
               continue;
            }
            *startPrb = freePrbBlock->endPrb - numPrb +1;
            break;  
         }
      }

      /* If no free block can be used to allocated request number of RBs */
      if(*startPrb == MAX_NUM_RB)
         return RFAILED;
   }

   /* If startPrb is known already, check if requested PRBs are available for allocation */
   else
   {
      freePrbNode = isPrbAvailable(&prbAlloc->freePrbBlockList, *startPrb, numPrb);
      if(!freePrbNode)
      {
         DU_LOG("\nERROR  -->  SCH: Requested DL PRB unavailable");
         return RFAILED;
      }
   }

   /* Update bitmap to allocate PRBs */
   for(symbol=startSymbol; symbol < (startSymbol+symbolLength); symbol++)
   {
      if(fillPrbBitmap(prbAlloc->prbBitMap[symbol], *startPrb, numPrb) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH: fillPrbBitmap() failed for symbol [%d] in DL", symbol);
         return RFAILED;
      }
   }

   /* Update the remaining number for free PRBs */
   removeAllocatedPrbFromFreePrbList(&prbAlloc->freePrbBlockList, freePrbNode, *startPrb, numPrb);

   return ROK;
}

/*******************************************************************
 *
 * @brief Allocates requested PRBs for UL
 *
 * @details
 *
 *    Function : allocatePrbUl
 *
 *    Functionality:
 *      Allocates requested PRBs in UL
 *      Keeps track of allocated PRB (using bitmap) and remaining PRBs
 *
 * @params[in] prbAlloc table
 *             Start symbol
 *             Number of symbols
 *             Start PRB
 *             Number of PRBs
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t allocatePrbUl(SchCellCb *cell, SlotTimingInfo slotTime, \
   uint8_t startSymbol, uint8_t symbolLength, uint16_t *startPrb, uint16_t numPrb)
{
   uint8_t        symbol = 0;
   uint16_t       prachStartPrb, prachNumPrb, prachEndPrb;
   bool           isPrachOccasion;
   FreePrbBlock   *freePrbBlock = NULLP;
   CmLList        *freePrbNode = NULLP;
   SchPrbAlloc    *prbAlloc = NULLP;

   if(cell == NULLP)
   {
      DU_LOG("\nERROR  --> SCH : allocatePrbUl(): Received cellCb is null");
      return RFAILED;
   }
   
   prbAlloc =   &cell->schUlSlotInfo[slotTime.slot]->prbAlloc;
   /* If startPrb is set to MAX_NUM_RB, it means startPrb is not known currently.
    * Search for an appropriate location in PRB grid and allocate requested resources */
   if(*startPrb == MAX_NUM_RB)
   {
      /* Check if PRACH is also scheduled in this slot */
      isPrachOccasion = schCheckPrachOcc(cell, slotTime);
      if(isPrachOccasion)
      {
         prachStartPrb =  cell->cellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.prachCfgGeneric.msg1FreqStart;
         prachNumPrb = schCalcPrachNumRb(cell);
         prachEndPrb = prachStartPrb + prachNumPrb -1;
      }

      /* Iterate through all free PRB blocks */
      freePrbNode = prbAlloc->freePrbBlockList.first; 
      while(freePrbNode)
      {
         freePrbBlock = (FreePrbBlock *)freePrbNode->node; 

         /* If PRACH is scheduled in this slot, then check if its PRBs belong to the current free block.
          * PRBs required for PRACH cannot be allocated to any other message */
         if((isPrachOccasion) &&
            ((prachStartPrb >= freePrbBlock->startPrb) && (prachStartPrb <= freePrbBlock->endPrb)) &&
            ((prachEndPrb >= freePrbBlock->startPrb) && (prachEndPrb <= freePrbBlock->endPrb)))
         {
            /* Implmentation is done such that highest-numbered free-RB is allocated first */ 
            if((freePrbBlock->endPrb > prachEndPrb) && ((freePrbBlock->endPrb - prachEndPrb) >= numPrb))
            {
               /* If sufficient free PRBs are available above PRACH message then,
                * endPrb = freePrbBlock->endPrb
                * startPrb = endPrb - numPrb +1;
                */
               *startPrb = freePrbBlock->endPrb - numPrb +1;
               break;
            }
            else if((prachStartPrb > freePrbBlock->startPrb) && ((prachStartPrb - freePrbBlock->startPrb) >= numPrb))
            {
               /* If free PRBs are available below PRACH message then,
                * endPrb = prachStartPrb - 1
                * startPrb = endPrb - numPrb +1
                */
               *startPrb = prachStartPrb - numPrb; 
               break;
            }
            else
            {
               freePrbNode = freePrbNode->next;
               continue;
            } 
         }
         else
         {
            /* Check if requested number of PRBs can be allocated from currect block */
            if(freePrbBlock->numFreePrb < numPrb)
            {
               freePrbNode = freePrbNode->next;
               continue;
            }
            *startPrb = freePrbBlock->endPrb - numPrb +1;
            break;
         }
      }

      /* If no free block can be used to allocated requested number of RBs */
      if(*startPrb == MAX_NUM_RB)
         return RFAILED;
   }
   else
   {
      /* If startPrb is known already, check if requested PRBs are available for allocation */
      freePrbNode = isPrbAvailable(&prbAlloc->freePrbBlockList, *startPrb, numPrb);
      if(!freePrbNode)
      {
         DU_LOG("\nERROR  -->  SCH: Requested UL PRB unavailable");
         return RFAILED;
      }
   }

   /* Update bitmap to allocate PRBs */
   for(symbol=startSymbol; symbol < (startSymbol+symbolLength); symbol++)
   {
      if(fillPrbBitmap(prbAlloc->prbBitMap[symbol], *startPrb, numPrb) != ROK)
      {
         DU_LOG("\nERROR  -->  SCH: fillPrbBitmap() failed for symbol [%d] in UL", symbol);
         return RFAILED;
      }
   }

   /* Update the remaining number for free PRBs */
   removeAllocatedPrbFromFreePrbList(&prbAlloc->freePrbBlockList, freePrbNode, *startPrb, numPrb);

   return ROK;
}

/*******************************************************************************
 *
 * @brief Try to find Best Free Block with Max Num PRB 
 *
 * @details
 *
 *    Function : searchLargestFreeBlock
 *
 *    Functionality:
 *     Finds the FreeBlock with MaxNum of FREE PRB considering SSB/SIB1 ocassions.
 *
 * @params[in] I/P > prbAlloc table (FreeBlock list)
 *             I/P > Slot timing Info
 *             O/P > Start PRB
 *             I/P > Direction (UL/DL)
 *       
 *
 * @return Max Number of Free PRB 
 *         If 0, then no Suitable Free Block
 *
 * ********************************************************************************/

uint16_t searchLargestFreeBlock(SchCellCb *cell, SlotTimingInfo slotTime,uint16_t *startPrb, Direction dir)
{
   uint16_t       reservedPrbStart=0, reservedPrbEnd=0, maxFreePRB = 0;
   FreePrbBlock   *freePrbBlock = NULLP;
   CmLList        *freePrbNode = NULLP;
   SchPrbAlloc    *prbAlloc = NULLP;
   bool           checkOccasion = FALSE;

   *startPrb = 0; /*Initialize the StartPRB to zero*/

   /*Based on Direction, Reserved Messsages will differi.e.
    * DL >> SSB and SIB1 ocassions wheres for UL, PRACH ocassions to be checked
    * and reserved before allocation for dedicated DL/UL msg*/
   if(dir == DIR_DL)
   {
      SchDlSlotInfo  *schDlSlotInfo = cell->schDlSlotInfo[slotTime.slot];
      PduTxOccsaion  ssbOccasion=0, sib1Occasion=0;

      prbAlloc = &schDlSlotInfo->prbAlloc;

      ssbOccasion = schCheckSsbOcc(cell, slotTime);
      sib1Occasion = schCheckSib1Occ(cell, slotTime);

      checkOccasion = TRUE;
      if(ssbOccasion && sib1Occasion)
      {
         reservedPrbStart = cell->cellCfg.dlCfgCommon.schFreqInfoDlSib.offsetToPointA; 
         reservedPrbEnd = reservedPrbStart + SCH_SSB_NUM_PRB + \
                          cell->sib1SchCfg.sib1PdcchCfg.dci.pdschCfg.pdschFreqAlloc.numPrb -1;
      }
      else if(ssbOccasion)
      {
         reservedPrbStart = cell->cellCfg.dlCfgCommon.schFreqInfoDlSib.offsetToPointA;
         reservedPrbEnd = reservedPrbStart + SCH_SSB_NUM_PRB -1;
      }
      else if(sib1Occasion)
      {
         reservedPrbStart = cell->sib1SchCfg.sib1PdcchCfg.dci.pdschCfg.pdschFreqAlloc.startPrb;
         reservedPrbEnd = reservedPrbStart + cell->sib1SchCfg.sib1PdcchCfg.dci.pdschCfg.pdschFreqAlloc.numPrb -1;
      }
      else
      {
         checkOccasion = FALSE;  
      }
   }
   else if(dir == DIR_UL)
   {
      prbAlloc = &cell->schUlSlotInfo[slotTime.slot]->prbAlloc;

      /* Check if PRACH is also scheduled in this slot */
      checkOccasion = schCheckPrachOcc(cell, slotTime);
      if(checkOccasion)
      {
         reservedPrbStart =  cell->cellCfg.ulCfgCommon.schInitialUlBwp.schRachCfg.prachCfgGeneric.msg1FreqStart;
         reservedPrbEnd = reservedPrbStart + (schCalcPrachNumRb(cell)) -1;
      }
   }
   else
   {
      DU_LOG("\nERROR --> SCH: Invalid Direction!");
      return (maxFreePRB);
   }

   freePrbNode = prbAlloc->freePrbBlockList.first; 
   while(freePrbNode)
   {
      freePrbBlock = (FreePrbBlock *)freePrbNode->node;

      /*For block with same numFreeBlocks, choose the one with HighestPRB range
       *Since FreeBLockList are arranged in Descending order of PRB range thus Skipping this block*/
      if(maxFreePRB >= freePrbBlock->numFreePrb) 
      {
         //skip this block
         freePrbNode = freePrbNode->next;
         continue;
      }

      /* If Broadcast/Prach message is scheduled in this slot, then check if its PRBs belong to the current free block.
       * Since SSB/SIB1 PRB location is fixed, these PRBs cannot be allocated to other message in same slot */
      if(checkOccasion && 
            ((reservedPrbStart >= freePrbBlock->startPrb) && (reservedPrbStart <= freePrbBlock->endPrb)) && \
            ((reservedPrbEnd >= freePrbBlock->startPrb) && (reservedPrbEnd <= freePrbBlock->endPrb)))
      {

         /* Implmentation is done such that highest-numbered free-RB is Checked first
            and freePRB in this block is greater than Max till now */
         if((freePrbBlock->endPrb > reservedPrbEnd) && ((freePrbBlock->endPrb - reservedPrbEnd) > maxFreePRB))
         {
            /* If sufficient free PRBs are available above reserved message*/
            *startPrb = reservedPrbEnd + 1;
            maxFreePRB = (freePrbBlock->endPrb - reservedPrbEnd);		 
         }
         /*Also check the other freeBlock (i.e. Above the reserved message) for MAX FREE PRB*/
         if((reservedPrbStart > freePrbBlock->startPrb) && ((reservedPrbStart - freePrbBlock->startPrb) > maxFreePRB))
         {
            /* If free PRBs are available below reserved message*/
            *startPrb = freePrbBlock->startPrb;
            maxFreePRB = (reservedPrbStart - freePrbBlock->startPrb);
         }
      }
      else  //Best Block
      {
         if(maxFreePRB < freePrbBlock->numFreePrb)
         {
            *startPrb = freePrbBlock->startPrb;
            maxFreePRB = freePrbBlock->numFreePrb;
         }

      }
      freePrbNode = freePrbNode->next;
   }  
   return(maxFreePRB);
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
 * @brief This function is used to store or modify the slice configuration Sch DB
 *
 * @details
 *
 *    Function : addOrModifySliceCfgInSchDb 
 *
 *    Functionality:
 *     function is used to store or modify the slice configuration Sch DB
 *
 * @params[in] SchSliceCfg *storeSliceCfg, SchSliceCfgReq *cfgReq,
 * SchSliceCfgRsp cfgRsp, uint8_t count
 *
 * @return
 *        ROK - Success
 *        RFAILED - Failure
 *
 * ********************************************************************************/
uint8_t addSliceCfgInSchDb(CmLListCp *sliceCfgInDb, SchRrmPolicyOfSlice *cfgReq)
{
   SchRrmPolicyOfSlice *sliceToStore;

   SCH_ALLOC(sliceToStore, sizeof(SchRrmPolicyOfSlice));
   if(sliceToStore)
   {
      memcpy(&sliceToStore->snssai, &cfgReq->snssai, sizeof(Snssai));  
      memcpy(&sliceToStore->rrmPolicyRatioInfo, &cfgReq->rrmPolicyRatioInfo, sizeof(SchRrmPolicyRatio));  
      addNodeToLList(sliceCfgInDb, sliceToStore, NULL);
   }
   else
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation failed in addOrModifySliceCfgInSchDb");
      return RFAILED;
   }
   return ROK;
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
uint8_t fillSliceCfgRsp(Inst inst, CmLListCp *storedSliceCfg, SchCellCb *cellCb, SchSliceCfgReq *schSliceCfgReq)
{
   SchMacRsp sliceFound;
   uint8_t cfgIdx = 0, sliceIdx = 0, plmnIdx = 0, ret =ROK;
   SchSliceCfgRsp schSliceCfgRsp;

   for(cfgIdx = 0; cfgIdx<schSliceCfgReq->numOfConfiguredSlice; cfgIdx++)
   {
      sliceFound = RSP_NOK;
      /* Here comparing the slice cfg request with the slice stored in cellCfg */
      for(plmnIdx = 0; plmnIdx < MAX_PLMN; plmnIdx++)
      {
         for(sliceIdx = 0; sliceIdx<cellCb->cellCfg.plmnInfoList[plmnIdx].numSliceSupport; sliceIdx++)
         {
            /* If we find the SliceCfgReq's SNSSAI in CellCb's SNSSAI DB, we mark this slice as configured and add it to Sch's DB. */ 
            if(!memcmp(&schSliceCfgReq->listOfSlices[cfgIdx]->snssai, cellCb->cellCfg.plmnInfoList[plmnIdx].snssai[sliceIdx], sizeof(Snssai)))
            {
               if(addSliceCfgInSchDb(storedSliceCfg, schSliceCfgReq->listOfSlices[cfgIdx]) == ROK)
               {
                  sliceFound = RSP_OK;
                  schSliceCfgRsp.cause = SUCCESSFUL;
               }
               else
               {
                  DU_LOG("\nERROR  --> SCH : Failed to store slice configuration in SchDb");
                  schSliceCfgRsp.cause = RESOURCE_UNAVAILABLE;
                  ret = RFAILED;
               }
               plmnIdx = MAX_PLMN;
               break;
            }
         }
      }
      
      if((sliceFound == RSP_NOK) && (schSliceCfgRsp.cause != RESOURCE_UNAVAILABLE))
         schSliceCfgRsp.cause = SLICE_NOT_FOUND;
      
      schSliceCfgRsp.snssai = schSliceCfgReq->listOfSlices[cfgIdx]->snssai;
      schSliceCfgRsp.rsp    = sliceFound;
      SchSendSliceCfgRspToMac(inst, schSliceCfgRsp);
   }
   return ret;
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
            if(sliceCfgReq->listOfSlices[cfgIdx])
            {
               SCH_FREE(sliceCfgReq->listOfSlices[cfgIdx], sizeof(SchRrmPolicyOfSlice));
            }
         }
         SCH_FREE(sliceCfgReq->listOfSlices, sliceCfgReq->numOfConfiguredSlice * sizeof(SchRrmPolicyOfSlice*));
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
 *    Function : SchProcSliceCfgReq 
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
   uint8_t ret = ROK;
   Inst   inst = pst->dstInst - SCH_INST_START;

   DU_LOG("\nINFO  -->  SCH : Received Slice Cfg request from MAC");
   if(schSliceCfgReq)
   {
      if(schSliceCfgReq->listOfSlices)
      {
         /* filling the slice configuration response of each slice */
         if(fillSliceCfgRsp(inst, &schCb[inst].sliceCfg, schCb[inst].cells[0], schSliceCfgReq) != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Failed to fill the slice cfg rsp");
            ret = RFAILED;
         }
         freeSchSliceCfgReq(schSliceCfgReq);
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  SCH : Received SchSliceCfgReq is NULL");
      ret = RFAILED;
   }
   return ret;
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
 * @brief fill slice configuration response
 *
 * @details
 *
 *    Function : fillSliceRecfgRsp
 *
 *    Functionality: fill slice reconfiguration response
 *
 * @params[in] SchCellCb, SchSliceCfgReq, SchSliceCfgRsp,uint8_t  count
 *
 * @return
 *        ROK - Success
 *        RFAILED - Failure
 *
 * ********************************************************************************/

uint8_t fillSliceRecfgRsp(Inst inst, CmLListCp *storedSliceCfg, SchSliceRecfgReq *schSliceRecfgReq)
{
   SchMacRsp sliceFound;
   uint8_t cfgIdx = 0;
   SchRrmPolicyOfSlice *rrmPolicyOfSlices;
   SchSliceRecfgRsp schSliceRecfgRsp;

   for(cfgIdx = 0; cfgIdx<schSliceRecfgReq->numOfConfiguredSlice; cfgIdx++)
   {
      sliceFound = RSP_NOK;
      /* Here comparing the slice recfg request with the StoredSliceCfg */
      CmLList *sliceCfg = storedSliceCfg->first;

      while(sliceCfg)
      {
         rrmPolicyOfSlices = (SchRrmPolicyOfSlice*)sliceCfg->node;
         
         if(rrmPolicyOfSlices && (memcmp(&schSliceRecfgReq->listOfSlices[cfgIdx]->snssai, &(rrmPolicyOfSlices->snssai), sizeof(Snssai)) == 0))
         {
            memcpy(&rrmPolicyOfSlices->rrmPolicyRatioInfo, &schSliceRecfgReq->listOfSlices[cfgIdx]->rrmPolicyRatioInfo, sizeof(SchRrmPolicyRatio));
            sliceFound = RSP_OK;
            break;
         }
         sliceCfg = sliceCfg->next;
      }

      schSliceRecfgRsp.snssai = schSliceRecfgReq->listOfSlices[cfgIdx]->snssai;
      schSliceRecfgRsp.rsp    = sliceFound;
      if(schSliceRecfgRsp.rsp == RSP_OK)
         schSliceRecfgRsp.cause = SUCCESSFUL;
      else
         schSliceRecfgRsp.cause = SLICE_NOT_FOUND;
      SchSendSliceRecfgRspToMac(inst, schSliceRecfgRsp);
   }
   return ROK;
}
/*******************************************************************************
 *
 * @brief This function is used to store the slice reconfiguration Sch DB
 *
 * @details
 *
 *    Function : SchProcSliceRecfgReq 
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
   uint8_t ret = ROK;
   Inst   inst = pst->dstInst - SCH_INST_START;

   DU_LOG("\nINFO  -->  SCH : Received Slice ReCfg request from MAC");
   if(schSliceRecfgReq)
   {
      if(schSliceRecfgReq->listOfSlices)
      {
         /* filling the slice configuration response of each slice */
         if(fillSliceRecfgRsp(inst, &schCb[inst].sliceCfg, schSliceRecfgReq) != ROK)
         {
            DU_LOG("\nERROR  -->  SCH : Failed to fill sch slice cfg response");
            ret = RFAILED;
         }
         freeSchSliceCfgReq(schSliceRecfgReq);
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  SCH : Received SchSliceRecfgReq is NULL");

   }
   return ret;
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
   SchPcchCfg *pageCfgRcvd = NULL;
   uint8_t i_sIdx = 0;

   pageCfgRcvd = &(cell->cellCfg.dlCfgCommon.schPcchCfg);

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
   uint8_t         nsValue = cell->cellCfg.dlCfgCommon.schPcchCfg.numPO;
   uint8_t         totalNumSsb = countSetBits(cell->cellCfg.ssbPosInBurst[0]);
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

/****************************************************************************
 *
 * @brief Storing the paging information in SCH database 
 *
 * @details
 *
 *    Function : schAddPagingIndtoList
 *
 *    Functionality: Storing the paging information in SCH database
 *
 * @params[in] CmLListCp *storedPageList, CmLList *pageIndInfo
 *       
 * @return ROK - sucess
 *         RFAILED - failure
 *        
 *************************************************************************/
uint8_t schAddPagingIndtoList(CmLListCp *storedPageList,void * pageIndInfo)
{
   CmLList  *firstNodeOfList = NULLP;
   CmLList  *currentNodeInfo = NULLP;
   SchPageInfo *tempNode = NULLP, *recvdNode = NULLP;
   
   recvdNode = (SchPageInfo*) pageIndInfo;
   CM_LLIST_FIRST_NODE(storedPageList,firstNodeOfList);
   
   SCH_ALLOC(currentNodeInfo, sizeof(CmLList));
   if(!currentNodeInfo)
   {  
      DU_LOG("\nERROR  --> SCH : schAddPagingIndtoList() : Memory allocation failed");
      return RFAILED;
   }
   
   currentNodeInfo->node = (PTR)pageIndInfo;
   while(firstNodeOfList)
   {
      tempNode = (SchPageInfo*)(firstNodeOfList->node);
      if ((recvdNode->pageTxTime.slot < tempNode->pageTxTime.slot))
      {
         cmLListInsCrnt(storedPageList, currentNodeInfo);
         break;
      }
      else if ((recvdNode->pageTxTime.slot == tempNode->pageTxTime.slot))
      {
         DU_LOG("\nERROR  --> SCH : schAddPagingIndtoList() : Slot[%d] is already present in the list", recvdNode->pageTxTime.slot);
         return RFAILED;
      }
      else
      {
         CM_LLIST_NEXT_NODE(storedPageList, firstNodeOfList);
      }
   } 
   
   if(!firstNodeOfList)
   {
      cmLListAdd2Tail(storedPageList, currentNodeInfo);
   }
   DU_LOG("\nDEBUG   -->  SCH : Paging information is stored successfully for PF:%d, Slot:%d",\
              recvdNode->pageTxTime.sfn, recvdNode->pageTxTime.slot);
   return ROK;
}

/****************************************************************************
 *
 * @brief Process paging indication at  SCH recevied form MAC 
 *
 * @details
 *
 *    Function : SchProcPagingInd
 *
 *    Functionality: Process paging indication at SCH recevied form MAC 
 *
 * @params[in] Pst *pst,  SchPageInd *pageInd 
 *       
 * @return void 
 *        
 *************************************************************************/
uint8_t SchProcPagingInd(Pst *pst,  SchPageInd *pageInd)
{
   uint8_t ret = RFAILED;
   uint16_t cellIdx = 0;
   Inst  inst = pst->dstInst - SCH_INST_START;
   SchCellCb *cellCb = NULLP;
   SchPageInfo *pageInfo = NULLP;

   if(pageInd)
   {
      DU_LOG("\nDEBUG   -->  SCH : Received paging indication from MAC for cellId[%d]",\
                  pageInd->cellId);

      /* Fetch Cell CB */
      for(cellIdx = 0; cellIdx < MAX_NUM_CELL; cellIdx++)
      {
         if((schCb[inst].cells[cellIdx]) && (schCb[inst].cells[cellIdx]->cellId == pageInd->cellId))
         {
            cellCb = schCb[inst].cells[cellIdx];
            break;
         }
      }
      if(cellCb)
      {
         if(pageInd->i_s > cellCb->cellCfg.dlCfgCommon.schPcchCfg.numPO)
         {
            DU_LOG("\nERROR --> SCH : SchProcPagingInd(): i_s should not be greater than number of paging occasion");
         }
         else
         {
            SCH_ALLOC(pageInfo, sizeof(SchPageInfo));
            if(pageInfo)
            {
               pageInfo->pf = pageInd->pf; 
               pageInfo->i_s = pageInd->i_s;
               pageInfo->pageTxTime.cellId = pageInd->cellId;
               pageInfo->pageTxTime.sfn = (pageInd->pf +  cellCb->pageCb.pagMonOcc[pageInd->i_s].frameOffset) % MAX_SFN;
               pageInfo->pageTxTime.slot = cellCb->pageCb.pagMonOcc[pageInd->i_s].pagingOccSlot;
               pageInfo->mcs = DEFAULT_MCS;
               pageInfo->msgLen =  pageInd->pduLen;
               SCH_ALLOC(pageInfo->pagePdu, pageInfo->msgLen);
               if(!pageInfo->pagePdu)
               {
                  DU_LOG("\nERROR  --> SCH : SchProcPagingInd(): Failed to allocate memory");
               }
               else
               {
                  memcpy(pageInfo->pagePdu, pageInd->pagePdu, pageInfo->msgLen);
                  ret = schAddPagingIndtoList(&cellCb->pageCb.pageIndInfoRecord[pageInfo->pageTxTime.sfn], pageInfo);
                  if(ret != ROK)
                  {
                     DU_LOG("\nERROR  --> SCH : SchProcPagingInd(): Failed to store paging record");
                  }
               }
            }
            else
            {
               DU_LOG("\nERROR  --> SCH : SchProcPagingInd(): Failed to allocate memory");
            }
         }
      }
      else
      {
         DU_LOG("\nERROR  -->  SCH : Cell ID [%d] not found", pageInd->cellId);
      }
      SCH_FREE(pageInd->pagePdu, pageInd->pduLen);
      SCH_FREE(pageInd, sizeof(SchPageInd));
   }
   else
   {
      DU_LOG("\nERROR  --> SCH : SchProcPagingInd(): Received null pointer");
   }
   return ret;
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

/*******************************************************************
 *
 * @brief Processes DL CQI ind from MAC
 *
 * @details
 *
 *    Function : SchProcDlCqiInd
 *
 *    Functionality:
 *       Processes DL CQI ind from MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t SchProcDlCqiInd(Pst *pst, SchDlCqiInd *dlCqiInd)
{
   uint8_t  ret = ROK;
   uint16_t ueId = 0, cellIdx = 0;
   SchUeCb *ueCb = NULLP;
   SchCellCb *cell = NULLP;
   Inst  inst = pst->dstInst-SCH_INST_START;   

   if(!dlCqiInd)
   {
      DU_LOG("\nERROR  -->  SCH : SchProcDlCqiInd(): CQI Ind is empty");
      ret = RFAILED;
   }
   else
   {
      GET_CELL_IDX(dlCqiInd->cellId, cellIdx);
      cell = schCb[inst].cells[cellIdx];
      if(cell == NULLP)
      { 
         DU_LOG("\nERROR  -->  SCH : SchProcDlCqiInd(): cell Id[%d] not found", dlCqiInd->cellId);
         ret = RFAILED;
      }
      else
      {
         if(cell->cellId == dlCqiInd->cellId)
         {
            GET_UE_ID(dlCqiInd->crnti, ueId);
            ueCb = &cell->ueCb[ueId-1];
            if(ueCb->crnti != dlCqiInd->crnti)
            {
               DU_LOG("\nERROR  -->  SCH : SchProcDlCqiInd(): UeCb for received crnti[%d] not found", dlCqiInd->crnti);
               ret = RFAILED;
            }
            else
            {
               /*TODO: complete the processing of DL CQI Ind*/ 
            }
         }
         else
         {
            DU_LOG("\nERROR  -->  SCH : SchProcDlCqiInd(): Received cell Id[%d] from MAC is not matching with CellID[%d] in SCH Cb",\
                    dlCqiInd->cellId, cell->cellId);
            ret = RFAILED;
         }
      }
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Processes UL CQI ind from MAC
 *
 * @details
 *
 *    Function : SchProcUlCqiInd
 *
 *    Functionality:
 *       Processes UL CQI ind from MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t SchProcUlCqiInd(Pst *pst, SchUlCqiInd *ulCqiInd)
{
   uint8_t  ret = ROK;
   uint16_t ueId = 0, cellIdx = 0;
   SchUeCb *ueCb = NULLP;
   SchCellCb *cell = NULLP;
   Inst  inst = pst->dstInst-SCH_INST_START;   

   if(!ulCqiInd)
   {
      DU_LOG("\nERROR  -->  SCH : SchProcUlCqiInd(): CQI Ind is empty");
      ret = RFAILED;
   }
   else
   {
      GET_CELL_IDX(ulCqiInd->cellId, cellIdx);
      cell = schCb[inst].cells[cellIdx];
      if(cell == NULLP)
      { 
         DU_LOG("\nERROR  -->  SCH : SchProcUlCqiInd(): cell Id[%d] not found", ulCqiInd->cellId);
         ret = RFAILED;
      }
      else
      {
         if(cell->cellId == ulCqiInd->cellId)
         {
            GET_UE_ID(ulCqiInd->crnti, ueId);
            ueCb = &cell->ueCb[ueId-1];
            if(ueCb->crnti != ulCqiInd->crnti)
            {
               DU_LOG("\nERROR  -->  SCH : SchProcUlCqiInd(): UeCb for received crnti[%d] not found",ulCqiInd->crnti);
               ret = RFAILED;
            }
            else
            {
               /*TODO: complete the processing of UL CQI Ind*/ 
            }
         }
         else
         {
            DU_LOG("\nERROR  -->  SCH : SchProcUlCqiInd(): Received cell Id[%d] from MAC is not matching with CellId[%d] in SCH Cb",\
                    ulCqiInd->cellId, cell->cellId);
            ret = RFAILED;
         }
      }
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Processes PHR ind from MAC
 *
 * @details
 *
 *    Function : SchProcPhrInd
 *
 *    Functionality:
 *       Processes PHR ind from MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t SchProcPhrInd(Pst *pst, SchPwrHeadroomInd *schPhrInd)
{
   uint8_t  ret = ROK;
   uint16_t ueId = 0, cellIdx = 0;
   SchUeCb *ueCb = NULLP;
   SchCellCb *cell = NULLP;
   Inst  inst = pst->dstInst-SCH_INST_START;   

   if(!schPhrInd)
   {
      DU_LOG("\nERROR  -->  SCH : SchProcPhrInd(): PHR is empty");
      ret = RFAILED;
   }
   else
   {
      GET_CELL_IDX(schPhrInd->cellId, cellIdx);
      cell = schCb[inst].cells[cellIdx];
      if(cell == NULLP)
      { 
         DU_LOG("\nERROR  -->  SCH : schProcPhrInd(): cell Id[%d] is not found", schPhrInd->cellId);
         ret = RFAILED;
      }
      else
      {
         if(cell->cellId == schPhrInd->cellId)
         {
            GET_UE_ID(schPhrInd->crnti, ueId);
            ueCb = &cell->ueCb[ueId-1];
            if(ueCb->crnti != schPhrInd->crnti)
            {
               DU_LOG("\nERROR  -->  SCH : SchProcPhrInd(): UeCb for received crnti[%d] not found",schPhrInd->crnti);
               ret = RFAILED;
            }
            else
            {
               /*TODO: complete the processing of PHR Ind*/ 
            }
         }
         else
         {
            DU_LOG("\nERROR  -->  SCH : SchProcPhrInd(): Mismatch between Received cell Id[%d] from MAC and CellID[%d] in SCH CB ",\
                    schPhrInd->cellId, cell->cellId);
            ret = RFAILED;
         }
      }
   }
   return ret;
}

/**********************************************************************
  End of file
 **********************************************************************/
