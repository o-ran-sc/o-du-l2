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

/* This file contains UE management handling functionality for MAC */

/* header include files (.h) */
#include "common_def.h"
#include "lrg.h"
#include "lrg.x"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "lwr_mac_upr_inf.h"
#include "mac.h"
#include "mac_utils.h"

/* function pointers for packing slot ind from mac to sch */
MacSchUeCreateReqFunc macSchUeCreateReqOpts[] =
{
   packMacSchUeCreateReq,    /* packing for loosely coupled */
   macSchUeCreateReq,        /* packing for tightly coupled */
   packMacSchUeCreateReq     /* packing for light weight loosely coupled */
};

DuMacUeCreateRspFunc DuMacUeCreateRspOpts[] =
{
   packDuMacUeCreateRsp,     /* packing for loosely coupled */
   duHandleMacUeCreateRsp,   /* packing for tightly coupled */
   packDuMacUeCreateRsp,     /* packing for light weight loosly coupled */
};

/*******************************************************************
 *
 * @brief Fills mac cell group config to be sent to scheduler
 *
 * @details
 *
 *    Function : fillMacCellGroupCfg
 *
 *    Functionality: Fills mac cell group config to be sent to sch
 *
 * @params[in] macCellGrp : mac cell group config at MAC
 *             *macCellGrpCfg : mac cell group cfg to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillMacCellGroupCfg(MacCellGrpCfg macCellGrp, SchMacCellGrpCfg  *macCellGrpCfg)
{
   uint8_t           idx;
   SchSchedReqCfg    *schedReqCfg;
   SchTagCfg         *tagCfg;

   /* Copy scheduling request config */
   schedReqCfg = &macCellGrpCfg->schedReqCfg;
   schedReqCfg->addModListCount = macCellGrp.schReqCfg.addModListCount;
   if(schedReqCfg->addModListCount > MAX_NUM_SR_CFG_PER_CELL_GRP)
   {
      DU_LOG("\nMAC : Scheduling Request Add/Mod Count %d exceeds max limit %d", \
	    schedReqCfg->addModListCount, MAX_NUM_SR_CFG_PER_CELL_GRP);
      return RFAILED;
   }
   for(idx = 0; idx < schedReqCfg->addModListCount; idx++)
   {
      schedReqCfg->addModList[idx].schedReqId = \
         macCellGrp.schReqCfg.addModList[idx].schedReqId;
      schedReqCfg->addModList[idx].srProhibitTmr = \
         macCellGrp.schReqCfg.addModList[idx].srProhibitTmr;
      schedReqCfg->addModList[idx].srTransMax = \
	 macCellGrp.schReqCfg.addModList[idx].srTransMax;
   }
   schedReqCfg->relListCount = macCellGrp.schReqCfg.relListCount;

   /* copy TAG config */
   tagCfg = &macCellGrpCfg->tagCfg;
   tagCfg->addModListCount = macCellGrp.tagCfg.addModListCount;
   if(tagCfg->addModListCount > MAX_NUM_TAGS)
   {
      DU_LOG("\nMAC : Scheduling Request Add/Mod Count %d exceeds max limit %d", \
	    tagCfg->addModListCount, MAX_NUM_TAGS);
      return RFAILED;
   }
   for(idx = 0; idx < tagCfg->addModListCount; idx++)
   {
      tagCfg->addModList[idx].tagId = \
         macCellGrp.tagCfg.addModList[idx].tagId;
      tagCfg->addModList[idx].timeAlignmentTmr = \
	 macCellGrp.tagCfg.addModList[idx].timeAlignTimer;
   }
   tagCfg->relListCount = macCellGrp.tagCfg.relListCount;

   /* Copy PHR config */
   if(macCellGrp.phrCfgSetupPres)
   {
      macCellGrpCfg->phrCfg.periodicTmr = macCellGrp.phrCfg.periodicTimer;
      macCellGrpCfg->phrCfg.prohibitTmr =  macCellGrp.phrCfg.prohibitTimer;
      macCellGrpCfg->phrCfg.txpowerFactorChange = macCellGrp.phrCfg.txPowerFactor;
      macCellGrpCfg->phrCfg.multiplePhr = macCellGrp.phrCfg.multiplePHR;
      macCellGrpCfg->phrCfg.dummy = macCellGrp.phrCfg.dummy;
      macCellGrpCfg->phrCfg.type2OtherCell = macCellGrp.phrCfg.phrType2OtherCell;
      macCellGrpCfg->phrCfg.modeOtherCG = macCellGrp.phrCfg.phrOtherCG;
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills phy cell group config to be sent to scheduler
 *
 * @details
 *
 *    Function : fillPhyCellGroupCfg
 *
 *    Functionality: Fills phy cell group config to be sent to sch
 *
 * @params[in] macUeCfg : Phy cell group config at MAC
 *             *schPhyCellGrp : phy cell group config to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillPhyCellGroupCfg(PhyCellGrpCfg macUeCfg, SchPhyCellGrpCfg *schPhyCellGrp)
{
   schPhyCellGrp->pdschHarqAckCodebook = macUeCfg.pdschHarqAckCodebook;
   schPhyCellGrp->pNrFr1 = macUeCfg.pNrFr1;

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills PDSCh serving cell config to send to scheduler
 *
 * @details
 *
 *    Function : fillPdschServCellCfg 
 *
 *    Functionality: Fills PDSCh serving cell config to send to scheduler
 *
 * @params[in] macPdschCfg : Pdsch serving cell config at MAC
 *             *schPdschCfg : Pdsch serving cell config to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillPdschServCellCfg(PdschServCellCfg macPdschCfg, SchPdschServCellCfg *schPdschCfg) 
{
   schPdschCfg->maxMimoLayers = NULL;
   if(macPdschCfg.maxMimoLayers)
   {
      /* TODO : Optional parameter */
   }

   schPdschCfg->numHarqProcForPdsch = \
      macPdschCfg.numHarqProcForPdsch;

   schPdschCfg->maxCodeBlkGrpPerTb = NULL;
   if(macPdschCfg.maxCodeBlkGrpPerTb)
   {
      /* TODO : Optional parameter */
   }

   schPdschCfg->codeBlkGrpFlushInd = NULL;
   if(macPdschCfg.codeBlkGrpFlushInd)
   {
      /* TODO : Optional parameter */
   }

   schPdschCfg->xOverhead = NULL;
   if(macPdschCfg.xOverhead)
   {
      /* TODO : Optional parameter */
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills PUSCH cofig in initial UL BWP config for SCH UE Cfg
 *
 * @details
 *
 *    Function : fillInitalUlBwpPuschCfg
 *
 *    Functionality:
 *       Fills PUSCH cofig in initial UL BWP config for SCH UE Cfg
 *
 * @params[in] macPuschCfg : Initial UL-BWP PUSCH cfg at MAC
 *             schPuschCfg : Initial UL-BWP PUSCH cfg to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillInitalUlBwpPuschCfg(PuschCfg macPuschCfg, SchPuschCfg *schPuschCfg)
{
   uint8_t   idx;

   schPuschCfg->dmrsUlCfgForPuschMapTypeA.addPos = \
      macPuschCfg.dmrsUlCfgForPuschMapTypeA.addPos;
   schPuschCfg->dmrsUlCfgForPuschMapTypeA.transPrecodDisabled.scramblingId0 = \
      macPuschCfg.dmrsUlCfgForPuschMapTypeA.transPrecodDisabled.scramblingId0;
   schPuschCfg->resourceAllocType = macPuschCfg.resourceAllocType;

   schPuschCfg->numTimeDomRsrcAlloc = macPuschCfg.numTimeDomRsrcAlloc;
   if(schPuschCfg->numTimeDomRsrcAlloc > MAX_NUM_UL_ALLOC)
   {
      DU_LOG("\nMAC : Number of Time domain resource allocation [%d] exceeds max limit [%d]",\
	    schPuschCfg->numTimeDomRsrcAlloc, MAX_NUM_UL_ALLOC);
      return RFAILED;
   }    
   for(idx = 0; idx < schPuschCfg->numTimeDomRsrcAlloc; idx++)
   {
      schPuschCfg->timeDomRsrcAllocList[idx].k2 = \
         macPuschCfg.timeDomRsrcAllocList[idx].k2;
      schPuschCfg->timeDomRsrcAllocList[idx].mappingType = \
	 macPuschCfg.timeDomRsrcAllocList[idx].mappingType;
      schPuschCfg->timeDomRsrcAllocList[idx].startSymbol = \
         macPuschCfg.timeDomRsrcAllocList[idx].startSymbol; 
      schPuschCfg->timeDomRsrcAllocList[idx].symbolLength = \
	 macPuschCfg.timeDomRsrcAllocList[idx].symbolLength;
   }

   schPuschCfg->transformPrecoder = macPuschCfg.transformPrecoder;
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills initail UL BWP config to send to scheduler
 *
 * @details
 *
 *    Function : fillInitialUlBwp
 *
 *    Functionality: Fills initail UL BWP config to send to sch
 *
 * @params[in] macInitUlBwp : Initial UL BWP cfg at MAC
 *             schInitUlBwp : Initial UL BWP cfg to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillInitialUlBwp(InitialUlBwp macInitUlBwp, SchInitialUlBwp *schInitUlBwp)
{
   schInitUlBwp->pucchCfgPres = macInitUlBwp.pucchPresent;
   if(schInitUlBwp->pucchCfgPres)
   {
      /* TODO : Optional parameter */
   }

   schInitUlBwp->puschCfgPres = macInitUlBwp.puschPresent;
   if(schInitUlBwp->puschCfgPres)
   {
      if(fillInitalUlBwpPuschCfg(macInitUlBwp.puschCfg, &schInitUlBwp->puschCfg) != ROK)
      {
	 DU_LOG("\nMAC : fillInitalUlBwpPuschCfg() failed");
	 return RFAILED;
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill PDCCH cfg in Initial DL BWP for UE Cfg in Scheduler
 *
 * @details
 *
 *    Function : fillInitDlBwpPdcchCfg
 *
 *    Functionality:
 *        Fill PDCCH cfg in Initial DL BWP for UE Cfg in Scheduler
 *
 * @params[in] macPdcchCfg : Inital DL BWP PDCCH cfg in MAC
 *             schPdcchCfg : Inital DL BWP PDCCH cfg to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillInitDlBwpPdcchCfg(PdcchConfig macPdcchCfg, SchPdcchConfig *schPdcchCfg)
{
   uint8_t idx;

   /* Fill CORESET info */
   schPdcchCfg->numCRsetToAddMod = macPdcchCfg.numCRsetToAddMod;
   if(schPdcchCfg->numCRsetToAddMod > MAX_NUM_CRSET)
   {
      DU_LOG("\nMAC : Number of CORESET to ADD/MOD [%d] exceeds max limit [%d]",\
	    schPdcchCfg->numCRsetToAddMod, MAX_NUM_CRSET);
      return RFAILED;
   }

   for(idx = 0; idx < schPdcchCfg->numCRsetToAddMod; idx++)
   {
      schPdcchCfg->cRSetToAddModList[idx].cRSetId = \
         macPdcchCfg.cRSetToAddModList[idx].cRSetId;
      memcpy(&schPdcchCfg->cRSetToAddModList[idx].freqDomainRsrc,\
	 &macPdcchCfg.cRSetToAddModList[idx].freqDomainRsrc, FREQ_DOM_RSRC_SIZE);
      schPdcchCfg->cRSetToAddModList[idx].duration = \
         macPdcchCfg.cRSetToAddModList[idx].duration;
      schPdcchCfg->cRSetToAddModList[idx].cceRegMappingType = \
	 macPdcchCfg.cRSetToAddModList[idx].cceRegMappingType;
      schPdcchCfg->cRSetToAddModList[idx].precoderGranularity = \
	 macPdcchCfg.cRSetToAddModList[idx].precoderGranularity;
      schPdcchCfg->cRSetToAddModList[idx].dmrsScramblingId = \
	 macPdcchCfg.cRSetToAddModList[idx].dmrsScramblingId;
   }

   schPdcchCfg->numCRsetToRel = macPdcchCfg.numCRsetToRel;
   if(schPdcchCfg->numCRsetToAddMod > MAX_NUM_CRSET)
   {
      DU_LOG("\nMAC : Number of CORESET to release [%d] exceeds max limit [%d]",\
	    schPdcchCfg->numCRsetToRel, MAX_NUM_CRSET);
      return RFAILED;
   }

   for(idx = 0; idx < schPdcchCfg->numCRsetToRel; idx++)
   {
      /* TODO */
   }

   /* Fill Search space info */
   schPdcchCfg->numSearchSpcToAddMod = macPdcchCfg.numSearchSpcToAddMod;
   if(schPdcchCfg->numSearchSpcToAddMod > MAX_NUM_SEARCH_SPC)
   {
      DU_LOG("\nMAC : Number of search space to ADD/MOD [%d] exceeds max [%d]", \
	    schPdcchCfg->numSearchSpcToAddMod, MAX_NUM_SEARCH_SPC);
      return RFAILED;
   }
   for(idx = 0; idx < schPdcchCfg->numSearchSpcToAddMod; idx++)
   {
      schPdcchCfg->searchSpcToAddModList[idx].searchSpaceId = \
         macPdcchCfg.searchSpcToAddModList[idx].searchSpaceId;
      schPdcchCfg->searchSpcToAddModList[idx].cRSetId = \
	 macPdcchCfg.searchSpcToAddModList[idx].cRSetId;
      schPdcchCfg->searchSpcToAddModList[idx].mSlotPeriodicityAndOffset = \
	 macPdcchCfg.searchSpcToAddModList[idx].mSlotPeriodicityAndOffset;
      memcpy(&schPdcchCfg->searchSpcToAddModList[idx].mSymbolsWithinSlot,
	 &macPdcchCfg.searchSpcToAddModList[idx].mSymbolsWithinSlot, \
	 MONITORING_SYMB_WITHIN_SLOT_SIZE);
      schPdcchCfg->searchSpcToAddModList[idx].numCandidatesAggLevel1 = \
	 macPdcchCfg.searchSpcToAddModList[idx].numCandidatesAggLevel1;
      schPdcchCfg->searchSpcToAddModList[idx].numCandidatesAggLevel2 = \
	 macPdcchCfg.searchSpcToAddModList[idx].numCandidatesAggLevel2;
      schPdcchCfg->searchSpcToAddModList[idx].numCandidatesAggLevel4 = \
	 macPdcchCfg.searchSpcToAddModList[idx].numCandidatesAggLevel4;
      schPdcchCfg->searchSpcToAddModList[idx].numCandidatesAggLevel8 = \
	 macPdcchCfg.searchSpcToAddModList[idx].numCandidatesAggLevel8;
      schPdcchCfg->searchSpcToAddModList[idx].numCandidatesAggLevel16 = \
	 macPdcchCfg.searchSpcToAddModList[idx].numCandidatesAggLevel16;
      schPdcchCfg->searchSpcToAddModList[idx].searchSpaceType = \
	 macPdcchCfg.searchSpcToAddModList[idx].searchSpaceType;
      schPdcchCfg->searchSpcToAddModList[idx].ueSpecificDciFormat = \
	 macPdcchCfg.searchSpcToAddModList[idx].ueSpecificDciFormat;
   }

   schPdcchCfg->numSearchSpcToRel = macPdcchCfg.numSearchSpcToRel;
   if(schPdcchCfg->numSearchSpcToRel > MAX_NUM_SEARCH_SPC)
   {
      DU_LOG("\nMAC : Number of search space to release [%d] exceeds max [%d]", \
	    schPdcchCfg->numSearchSpcToRel, MAX_NUM_SEARCH_SPC);
      return RFAILED;
   }
   for(idx = 0; idx < schPdcchCfg->numSearchSpcToRel; idx++)
   {
      /* TODO */
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Fill PDSCH cfg in Initial DL BWP for UE Cfg in Scheduler
 *
 * @details
 *
 *    Function : fillInitDlBwpPdschCfg
 *
 *    Functionality:
 *        Fill PDSCH cfg in Initial DL BWP for UE Cfg in Scheduler
 *
 * @params[in] macPdschCfg : Inital DL BWP PDSCH cfg at  MAC
 *             schPdschCfg : Inital DL BWP PDSCH cfg to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillInitDlBwpPdschCfg(PdschConfig macPdschCfg, SchPdschConfig *schPdschCfg)
{
   uint8_t   idx;

   schPdschCfg->dmrsDlCfgForPdschMapTypeA.addPos = \
       macPdschCfg.dmrsDlCfgForPdschMapTypeA.addPos;
   schPdschCfg->resourceAllocType = macPdschCfg.resourceAllocType;
   schPdschCfg->numTimeDomRsrcAlloc = macPdschCfg.numTimeDomRsrcAlloc;
   if(schPdschCfg->numTimeDomRsrcAlloc > MAX_NUM_DL_ALLOC)
   {
      DU_LOG("\nMAC : Number of time domain resource allocation [%d], exceeds\
	    max limit [%d]", schPdschCfg->numTimeDomRsrcAlloc, MAX_NUM_DL_ALLOC);
      return RFAILED;
   }

   for(idx = 0; idx < schPdschCfg->numTimeDomRsrcAlloc; idx++)
   {
      schPdschCfg->timeDomRsrcAllociList[idx].mappingType = \
         macPdschCfg.timeDomRsrcAllociList[idx].mappingType;
      schPdschCfg->timeDomRsrcAllociList[idx].startSymbol = \
         macPdschCfg.timeDomRsrcAllociList[idx].startSymbol;
      schPdschCfg->timeDomRsrcAllociList[idx].symbolLength = \
         macPdschCfg.timeDomRsrcAllociList[idx].symbolLength;
   }

   schPdschCfg->rbgSize = macPdschCfg.rbgSize;
   schPdschCfg->numCodeWordsSchByDci = macPdschCfg.numCodeWordsSchByDci;
   schPdschCfg->bundlingType = macPdschCfg.bundlingType;

   return ROK;
}

/*******************************************************************
 *
 * @brief Fill Initial DL BWP for SCH UE config
 *
 * @details
 *
 *    Function : fillInitialDlBwp
 *
 *    Functionality: Fill Initial DL BWP for SCH UE config
 *
 * @params[in] macInitDlBwp : Inital DL BWP cfg at MAC
 *             schInitDlBwp : Inital DL BWP cfg to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillInitialDlBwp(InitialDlBwp macInitDlBwp, SchInitalDlBwp *schInitDlBwp)
{
   schInitDlBwp->pdcchCfgPres = macInitDlBwp.pdcchPresent;
   if(schInitDlBwp->pdcchCfgPres)
   {
      if(fillInitDlBwpPdcchCfg(macInitDlBwp.pdcchCfg, &schInitDlBwp->pdcchCfg) != ROK)
      {
	 DU_LOG("\nMAC : fillInitDlBwpPdcchCfg() failed");
	 return RFAILED;
      }
   }

   schInitDlBwp->pdschCfgPres = macInitDlBwp.pdschPresent;
   if(schInitDlBwp->pdschCfgPres)
   {
      if(fillInitDlBwpPdschCfg(macInitDlBwp.pdschCfg,&schInitDlBwp->pdschCfg) != ROK)
      {
	 DU_LOG("\nMAC : fillInitDlBwpPdschCfg() failed");
	 return RFAILED;
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills Sp Cell config to be sent to scheduler
 *
 * @details
 *
 *    Function : fillSpCellCfg 
 *
 *    Functionality: Fills Sp Cell config to be sent to scheduler
 *
 * @params[in] macSpCellCfg : SP cell cfg at MAC
 *             schSpCellCfg : SP cell cfg to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillSpCellCfg(SpCellCfg macSpCellCfg, SchSpCellCfg *schSpCellCfg)
{
   uint8_t   idx;
   SchServCellCfgInfo   *servCellCfg;

   schSpCellCfg->servCellIdx = macSpCellCfg.servCellIdx;
   servCellCfg = &schSpCellCfg->servCellCfg;

   /* Fill initial DL BWP */
   if(fillInitialDlBwp(macSpCellCfg.servCellCfg.initDlBwp, \
	    &servCellCfg->initDlBwp) != ROK )
   {
      DU_LOG("\nMAC : fillInitialDlBwp() failed");
      return RFAILED;
   }

   servCellCfg->numDlBwpToAdd = macSpCellCfg.servCellCfg.numDlBwpToAdd;
   if(servCellCfg->numDlBwpToAdd > MAX_NUM_BWP)
   {
      DU_LOG("\nMAC : Number of DL BWP to ADD/MOD [%d] exceeds max limit [%d]",\
	    servCellCfg->numDlBwpToAdd, MAX_NUM_BWP);
      return RFAILED;
   }
   for(idx = 0; idx < servCellCfg->numDlBwpToAdd; idx++)
   {
      /* TODO : As of now numDlBwpToAdd = 0 */
   }

   servCellCfg->firstActvDlBwpId =  macSpCellCfg.servCellCfg.firstActvDlBwpId;
   servCellCfg->defaultDlBwpId = macSpCellCfg.servCellCfg.defaultDlBwpId;
   servCellCfg->bwpInactivityTmr = NULL;
   if(macSpCellCfg.servCellCfg.bwpInactivityTmr)
   {
      /* TODO : This is an optional parameter, not filled currently */
   }

   /* Fill PDSCH serving cell config */
   if(fillPdschServCellCfg(macSpCellCfg.servCellCfg.pdschServCellCfg, \
	    &servCellCfg->pdschServCellCfg) != ROK)
   {
      DU_LOG("\nMAC : fillPdschServCellCfg() failed");
      return RFAILED;
   }

   /* Fill Initail UL BWP */
   if(fillInitialUlBwp(macSpCellCfg.servCellCfg.initUlBwp, \
	    &servCellCfg->initUlBwp) != ROK)
   {
      DU_LOG("\nMAC : fillInitialUlBwp() failed");
      return RFAILED;
   }

   servCellCfg->numUlBwpToAdd = macSpCellCfg.servCellCfg.numUlBwpToAdd;
   if(servCellCfg->numUlBwpToAdd > MAX_NUM_BWP)
   {
      DU_LOG("\nMAC : Number of UL BWP to ADD/MOD [%d] exceeds max limit [%d]",\
	    servCellCfg->numUlBwpToAdd, MAX_NUM_BWP);
      return RFAILED;
   }
   for(idx = 0; idx < servCellCfg->numUlBwpToAdd; idx++)
   {
      /* TODO : As of now numDlBwpToAdd = 0 */
   }
   servCellCfg->firstActvUlBwpId =  macSpCellCfg.servCellCfg.firstActvUlBwpId;

   return ROK;
}


/*******************************************************************
 *
 * @brief Fill logical channel configuration
 *
 * @details
 *
 *    Function : fillLogicalChannelCfg
 *
 *    Functionality: Fill logical channel configuration
 *
 * @params[in] macLcCfg : Logical channel Cfg at MAC
 *             schLcCfg : LC cfg to fill at scheduler
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillLogicalChannelCfg(LcCfg macLcCfg, SchLcCfg *schLcCfg)
{
   schLcCfg->lcId = macLcCfg.lcId;
   schLcCfg->dlLcCfg.lcp = macLcCfg.dlLcCfg.lcp;

   schLcCfg->drbQos = NULL;
   if(macLcCfg.drbQos)
   {
      /* TODO : Optional Parameter */
   } 

   schLcCfg->snssai = NULL;
   if(macLcCfg.snssai)
   {
      /* TODO : Optional Parameter */
   }

   schLcCfg->ulLcCfg = NULL;
   if(macLcCfg.ulLcCfg)
   {
      /* TODO : Optional Parameter */
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Fills and sends UE configuration to Scheduler
 *
 * @details
 *
 *    Function : sendAddUeCreateReqToSch
 *
 *    Functionality: Fills and sends UE configuration to Scheduler
 *
 * @params[in] Ue configuration from DU APP
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sendAddUeCreateReqToSch(MacUeCfg *ueCfg)
{
   Pst pst;
   uint8_t    idx;
   SchUeCfg   schUeCfg;

   schUeCfg.cellId = ueCfg->cellId;
   schUeCfg.crnti = ueCfg->crnti;

   /* Copy MAC cell group config */
   memset(&schUeCfg.macCellGrpCfg, 0, sizeof(SchMacCellGrpCfg));
   if(fillMacCellGroupCfg(ueCfg->macCellGrpCfg, &schUeCfg.macCellGrpCfg) != ROK)
   {
      DU_LOG("\nMAC : fillMacCellGroupCfg() failed");
      return RFAILED;
   }

   /* Copy Physical cell group config */
   memset(&schUeCfg.phyCellGrpCfg, 0,sizeof(SchPhyCellGrpCfg));
   if(fillPhyCellGroupCfg(ueCfg->phyCellGrpCfg, &schUeCfg.phyCellGrpCfg) != ROK)
   {
      DU_LOG("\nMAC : fillPhyCellGroupCfg() failed");
      return RFAILED;
   }

   /* Copy sp cell config */
   memset(&schUeCfg.spCellCfg, 0, sizeof(SchSpCellCfg));
   if(fillSpCellCfg(ueCfg->spCellCfg, &schUeCfg.spCellCfg) != ROK)
   {
      DU_LOG("\nMAC : fillSpCellCfg() failed");
      return RFAILED;
   }

   schUeCfg.aggrMaxBitRate = NULL;
   if(ueCfg->maxAggrBitRate != NULL)
   {

      MAC_ALLOC(schUeCfg.aggrMaxBitRate, sizeof(SchAggrMaxBitRate));
      if(!schUeCfg.aggrMaxBitRate)
      {
	 DU_LOG("\nMAC : Memory allocation failed in sendAddUeCreateReqToSch");
	 return RFAILED;
      }
      schUeCfg.aggrMaxBitRate->ulBitRate = ueCfg->maxAggrBitRate->ulBits;
      schUeCfg.aggrMaxBitRate->dlBitRate = ueCfg->maxAggrBitRate->dlBits;
   }

   schUeCfg.numLc = ueCfg->numLcs;
   if(schUeCfg.numLc > MAX_NUM_LOGICAL_CHANNELS)
   {
      DU_LOG("\nMAC : Number of Logical channels %d exceeds max limit %d",\
	    schUeCfg.numLc, MAX_NUM_LOGICAL_CHANNELS);
   }  
   for(idx = 0; idx < schUeCfg.numLc; idx++)
   {
      if(fillLogicalChannelCfg(ueCfg->lcCfgList[idx], &schUeCfg.lcCfgList[idx]) != ROK)
      {
	 DU_LOG("\nMAC : fillLogicalChannelCfg() failed");
	 return RFAILED;
      }
   }

   /* Fill event and send UE create request to SCH */
   FILL_PST_MAC_TO_SCH(pst, EVENT_UE_CREATE_REQ_TO_SCH);
   return(*macSchUeCreateReqOpts[pst.selector])(&pst, &schUeCfg);
}

/*******************************************************************
 *
 * @brief Creates UE Cb and fills ueCfg
 *
 * @details
 *
 *    Function : createUeCb
 *
 *    Functionality: Creates UE Cb and fills ueCfg
 *
 * @params[in] MAC UE Configuration
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t createUeCb(MacUeCfg *ueCfg)
{
   uint16_t  ueIdx, lcIdx, cellIdx;
   MacUeCb   *ueCb;

   GET_CELL_IDX(ueCfg->cellId, cellIdx);

   /* Validate cell id */
   if(macCb.macCell[cellIdx]->cellId != ueCfg->cellId)
   {
      DU_LOG("\nMAC : Cell Id %d not configured", ueCfg->cellId);
      return RFAILED;
   }

   /* Check if max number of UE configured */
   if(macCb.macCell[cellIdx]->numActvUe > MAX_NUM_UE)
   {
      DU_LOG("MAC : Max number of UE [%d] already configured", MAX_NUM_UE);
      return RFAILED;
   }

   /* Check if UE already configured */
   ueCb = &macCb.macCell[cellIdx]->ueCb[ueCfg->ueIdx];
   if(ueCb)
   {
      if((ueCb->ueIdx == ueCfg->ueIdx) && (ueCb->crnti == ueCfg->crnti) &&\
	    (ueCb->state == UE_STATE_ACTIVE))
      {
	 DU_LOG("\n MAC : CRNTI %d already configured ", ueCfg->crnti);
	 return ROKDUP;
      }
   }

   /* Fill received Ue Configuration in UeCb */
   memset(ueCb, 0, sizeof(MacUeCb));

   ueCb->crnti = ueCfg->crnti;
   ueCb->cellCb = macCb.macCell[cellIdx];
   ueCb->dlInfo.dlHarqEnt.numHarqProcs = \
      ueCfg->spCellCfg.servCellCfg.pdschServCellCfg.numHarqProcForPdsch; 
   ueCb->state = UE_STATE_ACTIVE;

   /* Fill BSR info */
   switch(ueCfg->macCellGrpCfg.bsrTmrCfg.periodicTimer)
   {
      case 0:
	 ueCb->bsrTmrCfg.periodicTimer = PERIODIC_BSR_TMR_1MS;
	 break;
      case 1:
	 ueCb->bsrTmrCfg.periodicTimer = PERIODIC_BSR_TMR_5MS;
	 break;
      case 2:
	 ueCb->bsrTmrCfg.periodicTimer = PERIODIC_BSR_TMR_10MS;
	 break;
      case 3:
	 ueCb->bsrTmrCfg.periodicTimer = PERIODIC_BSR_TMR_16MS;
	 break;
      case 4:
	 ueCb->bsrTmrCfg.periodicTimer = PERIODIC_BSR_TMR_20MS;
	 break;
      case 5:
	 ueCb->bsrTmrCfg.periodicTimer = PERIODIC_BSR_TMR_32MS;
	 break;
      case 6:
	 ueCb->bsrTmrCfg.periodicTimer = PERIODIC_BSR_TMR_40MS;
	 break;
      case 7:
	 ueCb->bsrTmrCfg.periodicTimer = PERIODIC_BSR_TMR_60MS;
	 break;
      case 8:
	 ueCb->bsrTmrCfg.periodicTimer = PERIODIC_BSR_TMR_80MS;
	 break;
      case 9:
	 ueCb->bsrTmrCfg.periodicTimer = PERIODIC_BSR_TMR_128MS;
	 break;
      case 10:
	 ueCb->bsrTmrCfg.periodicTimer = PERIODIC_BSR_TMR_160MS;
	 break;
      case 11:
	 ueCb->bsrTmrCfg.periodicTimer = PERIODIC_BSR_TMR_320MS;
	 break;
      case 12:
	 ueCb->bsrTmrCfg.periodicTimer = PERIODIC_BSR_TMR_640MS;
	 break;
      case 13:
	 ueCb->bsrTmrCfg.periodicTimer = PERIODIC_BSR_TMR_1280MS;
	 break;
      case 14:
	 ueCb->bsrTmrCfg.periodicTimer = PERIODIC_BSR_TMR_2560MS;
	 break;
      default:
	 DU_LOG("\nMAC : Invalid BSR Periodic Timer");
	 return RFAILED;
   }

   switch(ueCfg->macCellGrpCfg.bsrTmrCfg.retxTimer)
   {   
      case 0:
	 ueCb->bsrTmrCfg.retxTimer = RETX_BSR_TMR_10MS;
	 break;
      case 1:
	 ueCb->bsrTmrCfg.retxTimer = RETX_BSR_TMR_20MS;
	 break;
      case 2:
	 ueCb->bsrTmrCfg.retxTimer = RETX_BSR_TMR_40MS;
	 break;
      case 3:
	 ueCb->bsrTmrCfg.retxTimer = RETX_BSR_TMR_80MS;
	 break;
      case 4:
	 ueCb->bsrTmrCfg.retxTimer = RETX_BSR_TMR_160MS;
	 break;
      case 5:
	 ueCb->bsrTmrCfg.retxTimer = RETX_BSR_TMR_320MS;
	 break;
      case 6:
	 ueCb->bsrTmrCfg.retxTimer = RETX_BSR_TMR_640MS;
	 break;
      case 7:
	 ueCb->bsrTmrCfg.retxTimer = RETX_BSR_TMR_1280MS;
	 break;
      case 8:
	 ueCb->bsrTmrCfg.retxTimer = RETX_BSR_TMR_2560MS;
	 break;
      case 9:
	 ueCb->bsrTmrCfg.retxTimer = RETX_BSR_TMR_5120MS;
	 break;
      case 10:
	 ueCb->bsrTmrCfg.retxTimer = RETX_BSR_TMR_10240MS;
	 break;
      default:
	 DU_LOG("\nMAC : Invalid BSR retransmission timer");
	 break;
   }

   switch(ueCfg->macCellGrpCfg.bsrTmrCfg.srDelayTimer)
   {
      case 0:
	 ueCb->bsrTmrCfg.srDelayTimer = SR_DELAY_TMR_20MS;
	 break;
      case 1:
	 ueCb->bsrTmrCfg.srDelayTimer = SR_DELAY_TMR_40MS;
	 break;
      case 2:
	 ueCb->bsrTmrCfg.srDelayTimer = SR_DELAY_TMR_64MS;
	 break;
      case 3:
	 ueCb->bsrTmrCfg.srDelayTimer = SR_DELAY_TMR_128MS;
	 break;
      case 4:
	 ueCb->bsrTmrCfg.srDelayTimer = SR_DELAY_TMR_512MS;
	 break;
      case 5:
	 ueCb->bsrTmrCfg.srDelayTimer = SR_DELAY_TMR_1024MS;
	 break;
      case 6:
	 ueCb->bsrTmrCfg.srDelayTimer = SR_DELAY_TMR_2560MS;
	 break;
      default:
	 DU_LOG("\nMAC : Invalid SR delay timer");
	 return RFAILED;
   }

   /* Fill SRB1 info */
   if(ueCfg->numLcs > MAX_NUM_LOGICAL_CHANNELS)
   {
      DU_LOG("\nMAC : Number of LC to configure[%d] exceeds limit[%d]",\
	    ueCfg->numLcs, MAX_NUM_LOGICAL_CHANNELS);
      return RFAILED;
   }

   for(lcIdx = 0; lcIdx < ueCfg->numLcs; lcIdx++)
   {
      ueCb->dlInfo.lcCb[ueCb->dlInfo.numDlLc].lcId = ueCfg->lcCfgList[lcIdx].lcId;
      ueCb->dlInfo.lcCb[ueCb->dlInfo.numDlLc].lcState = LC_STATE_ACTIVE;
      ueCb->dlInfo.numDlLc++;
   }

   /* Copy RA Cb */
   for(ueIdx = 0; ueIdx < MAX_NUM_UE; ueIdx++)
   {
      if(macCb.macCell[cellIdx]->macRaCb[ueIdx].crnti == ueCb->crnti)
      {
	 ueCb->raCb = &macCb.macCell[cellIdx]->macRaCb[ueIdx];
	 break;
      }
   }

   macCb.macCell[cellIdx]->numActvUe++;

   return ROK;
}

/*******************************************************************
 *
 * @brief Handles UE create requst from DU APP
 *
 * @details
 *
 *    Function : MacProcUeCreateReq
 *
 *    Functionality: Handles UE create requst from DU APP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacProcUeCreateReq(Pst *pst, MacUeCfg *ueCfg)
{
   uint8_t ret;

   DU_LOG("\nMAC : UE Create Request for CRNTI[%d]", ueCfg->crnti);

   if(ueCfg)
   {
      ret = createUeCb(ueCfg);
      if(ret == ROK)
      {
	 ret = sendAddUeCreateReqToSch(ueCfg);
	 if(ret != ROK)
	 {
	    DU_LOG("\nMAC : Failed to send UE Create request to scheduler");
	 }
      }
      else 
      {
	 DU_LOG("\nMAC : Failed to create MAC UE Cb ");
      }
   }
   else
   {
      DU_LOG("\nMAC : MAC UE Create request processing failed");
      ret = RFAILED;
   }
   /* FREE shared memory */
   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, ueCfg, sizeof(MacUeCfg));

   return ret;
}

/*******************************************************************
 *
 * @brief Fill and Send UE create response from MAC to DU APP
 *
 * @details
 *
 *    Function : MacSendUeCreateRsp
 *
 *    Functionality: Fill and Send UE create response from MAC to DUAPP
 *
 * @params[in] MAC UE create result
 *             SCH UE create response
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacSendUeCreateRsp(MacRsp result, SchUeCfgRsp *schCfgRsp)
{
   MacUeCfgRsp   *cfgRsp;
   Pst        rspPst;

   MAC_ALLOC_SHRABL_BUF(cfgRsp, sizeof(MacUeCfgRsp));
   if(!cfgRsp)
   {
      DU_LOG("\nMAC: Memory allocation for UE config response failed");
      return RFAILED;
   }

   /* Filling UE Config response */
   memset(cfgRsp, 0, sizeof(MacUeCfgRsp));
   cfgRsp->cellId = schCfgRsp->cellId;
   cfgRsp->ueIdx = schCfgRsp->ueIdx;
   cfgRsp->result = result;

   /* Fill Post structure and send UE Create response*/
   memset(&rspPst, 0, sizeof(Pst));
   FILL_PST_MAC_TO_DUAPP(rspPst, EVENT_MAC_UE_CREATE_RSP);
   return DuMacUeCreateRspOpts[rspPst.selector](&rspPst, cfgRsp); 

}


/*******************************************************************
 *
 * @brief  Processes UE create response from scheduler
 *
 * @details
 *
 *    Function : MacProcSchUeCfgRsp
 *
 *    Functionality:
 *      Processes UE create response from scheduler
 *      Sends UE create response to DU APP
 *
 * @params[in] Pst : Post structure
 *             schCfgRsp : Scheduler UE cfg response
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacProcSchUeCfgRsp(Pst *pst, SchUeCfgRsp *schCfgRsp)
{
   uint8_t result = MAC_DU_APP_RSP_NOK;
   uint8_t ret = ROK;
   uint16_t cellIdx;

   GET_CELL_IDX(schCfgRsp->cellId, cellIdx);

   if(schCfgRsp->rsp == RSP_NOK)
   {
      DU_LOG("\nMAC : SCH UE Create Response : FAILURE [CRNTI %d]", schCfgRsp->crnti);
      memset(&macCb.macCell[cellIdx]->ueCb[schCfgRsp->ueIdx], 0, sizeof(MacUeCb));
      macCb.macCell[cellIdx]->numActvUe--;
      result = MAC_DU_APP_RSP_NOK;
   }
   else
   {
      DU_LOG("\nMAC : SCH UE Create Response : SUCCESS [CRNTI %d]", schCfgRsp->crnti);
      result = MAC_DU_APP_RSP_OK;
   }
   ret = MacSendUeCreateRsp(result, schCfgRsp);
   return ret; 
}


/**********************************************************************
  End of file
 **********************************************************************/
