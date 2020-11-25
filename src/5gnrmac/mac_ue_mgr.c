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
MacSchAddUeConfigReqFunc macSchAddUeConfigReqOpts[] =
{
   packMacSchAddUeConfigReq,    /* packing for loosely coupled */
   MacSchAddUeConfigReq,        /* packing for tightly coupled */
   packMacSchAddUeConfigReq     /* packing for light weight loosely coupled */
};

MacDuUeCfgRspFunc macDuUeCfgRspOpts[] =
{
   packDuMacUeCfgRsp,   /* packing for loosely coupled */
   DuProcMacUeCfgRsp,   /* packing for tightly coupled */
   packDuMacUeCfgRsp   /* packing for light weight loosly coupled */
};

MacSchModUeConfigReqFunc macSchModUeConfigReqOpts[] =
{
   packMacSchModUeConfigReq,    /* packing for loosely coupled */
   MacSchModUeConfigReq,        /* packing for tightly coupled */
   packMacSchModUeConfigReq     /* packing for light weight loosely coupled */
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
   if(macPdschCfg.maxMimoLayers)
   {
      if(!schPdschCfg->maxMimoLayers)
      {
         MAC_ALLOC_SHRABL_BUF(schPdschCfg->maxMimoLayers, sizeof(uint8_t));
	 if(!schPdschCfg->maxMimoLayers)
	 {
            DU_LOG("\nMAC :Memory Alloc MimoLayers Failed at fillPdschServCellCfg()");
	    return RFAILED;
	 }
      }
      *schPdschCfg->maxMimoLayers = *macPdschCfg.maxMimoLayers;
   }
   else
   {
      schPdschCfg->maxMimoLayers = NULLP;
   }

   schPdschCfg->numHarqProcForPdsch = \
      macPdschCfg.numHarqProcForPdsch;

   if(macPdschCfg.maxCodeBlkGrpPerTb)
   {
      if(!schPdschCfg->maxCodeBlkGrpPerTb)
      {
         MAC_ALLOC_SHRABL_BUF(schPdschCfg->maxCodeBlkGrpPerTb, sizeof(SchMaxCodeBlkGrpPerTB));
	 if(!schPdschCfg->maxCodeBlkGrpPerTb)
	 {
            DU_LOG("\nMAC :Memory Alloc for code Block Failed at fillPdschServCellCfg()");
	    return RFAILED;
	 }
      }
      *schPdschCfg->maxCodeBlkGrpPerTb = *macPdschCfg.maxCodeBlkGrpPerTb;
   }
   else
   {
      schPdschCfg->maxCodeBlkGrpPerTb = NULLP;
   }

   if(macPdschCfg.codeBlkGrpFlushInd)
   {
      if(!schPdschCfg->codeBlkGrpFlushInd)
      {
         MAC_ALLOC_SHRABL_BUF(schPdschCfg->codeBlkGrpFlushInd, sizeof(bool));
	 if(!schPdschCfg->codeBlkGrpFlushInd)
	 {
            DU_LOG("\nMAC :Memory Alloc for Flush Ind Failed at fillPdschServCellCfg()");
	    return RFAILED;
	 }
      }
      *schPdschCfg->codeBlkGrpFlushInd = *macPdschCfg.codeBlkGrpFlushInd;
   }
   else
   {
      schPdschCfg->codeBlkGrpFlushInd = NULLP;
   }

   if(macPdschCfg.xOverhead)
   {
      if(!schPdschCfg->xOverhead)
      {
         MAC_ALLOC_SHRABL_BUF(schPdschCfg->xOverhead, sizeof(SchPdschXOverhead));
	 if(!schPdschCfg->xOverhead)
	 {
            DU_LOG("\nMAC :Memory Alloc for xOverHead Failed at fillPdschServCellCfg()");
	    return RFAILED;
	 }
      }
      *schPdschCfg->xOverhead = *macPdschCfg.xOverhead;
   }
   else
   {
      schPdschCfg->xOverhead = NULLP;
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

   schPuschCfg->dataScramblingId = macPuschCfg.dataScramblingId;
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
   if(schPdschCfg->bundlingType == STATIC_BUNDLING_TYPE)
   {
      schPdschCfg->bundlingInfo.SchStaticBundling.size  = macPdschCfg.bundlingInfo.StaticBundling.size;
   }
   else if(schPdschCfg->bundlingType == DYNAMIC_BUNDLING_TYPE)
   {
      schPdschCfg->bundlingInfo.SchDynamicBundling.sizeSet1 = macPdschCfg.bundlingInfo.DynamicBundling.sizeSet1;
      schPdschCfg->bundlingInfo.SchDynamicBundling.sizeSet2 = macPdschCfg.bundlingInfo.DynamicBundling.sizeSet2;
   }
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
 * @brief Sends UE configuration to Scheduler
 *
 * @details
 *
 *    Function : sendUeReqToSch
 *
 *    Functionality: sends UE configuration to Scheduler
 *
 * @params[in] Pst and Ue configuration
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t sendUeReqToSch(Pst *pst, SchUeCfg *schUeCfg)
{
   Pst schPst;
   switch(pst->event)
   {
      case EVENT_MAC_UE_CREATE_REQ:
	 FILL_PST_MAC_TO_SCH(schPst, EVENT_ADD_UE_CONFIG_REQ_TO_SCH);
	 return(*macSchAddUeConfigReqOpts[schPst.selector])(&schPst, schUeCfg);

      case EVENT_MAC_UE_RECONFIG_REQ:
	 FILL_PST_MAC_TO_SCH(schPst, EVENT_MODIFY_UE_CONFIG_REQ_TO_SCH);
	 return(*macSchModUeConfigReqOpts[schPst.selector])(&schPst,schUeCfg);
      default: 
	 DU_LOG("\n Invalid Pst received %d", pst->event);
	 return RFAILED;
   }
}

/*******************************************************************
 *
 * @brief Fills Sch Drb Qos Information
 *
 * @details
 *
 *    Function : fillSchDrbQosInfo
 *
 *    Functionality: Fills Sch Drb Qos Information
 *
 * @params[in] macLcCfg : Logical channel Cfg at MAC
 *             schLcCfg : LC cfg to fill at scheduler
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void fillSchDrbQosInfo(DrbQosInfo *macDrbQos, SchDrbQosInfo *schDrbQos)
{
   schDrbQos->fiveQiType  = macDrbQos->fiveQiType;
   if(schDrbQos->fiveQiType == SCH_QOS_NON_DYNAMIC)
   {
      schDrbQos->u.nonDyn5Qi.fiveQi = macDrbQos->u.nonDyn5Qi.fiveQi;
      schDrbQos->u.nonDyn5Qi.avgWindow = macDrbQos->u.nonDyn5Qi.avgWindow;
      schDrbQos->u.nonDyn5Qi.maxDataBurstVol = macDrbQos->u.nonDyn5Qi.maxDataBurstVol;
      schDrbQos->u.nonDyn5Qi.priorLevel =  macDrbQos->u.nonDyn5Qi.priorLevel;
   }
   else if(schDrbQos->fiveQiType == SCH_QOS_DYNAMIC)
   {
      schDrbQos->u.dyn5Qi.priorLevel         = macDrbQos->u.dyn5Qi.priorLevel;
      schDrbQos->u.dyn5Qi.packetDelayBudget  = macDrbQos->u.dyn5Qi.packetDelayBudget;
      schDrbQos->u.dyn5Qi.packetErrRateScalar= macDrbQos->u.dyn5Qi.packetErrRateScalar;
      schDrbQos->u.dyn5Qi.packetErrRateExp   = macDrbQos->u.dyn5Qi.packetErrRateExp;
      schDrbQos->u.dyn5Qi.fiveQi             = macDrbQos->u.dyn5Qi.fiveQi;
      schDrbQos->u.dyn5Qi.delayCritical      = macDrbQos->u.dyn5Qi.delayCritical;
      schDrbQos->u.dyn5Qi.avgWindow          = macDrbQos->u.dyn5Qi.avgWindow;
      schDrbQos->u.dyn5Qi.maxDataBurstVol    = macDrbQos->u.dyn5Qi.maxDataBurstVol;
   }
   schDrbQos->ngRanRetPri.priorityLevel   = macDrbQos->ngRanRetPri.priorityLevel;
   schDrbQos->ngRanRetPri.preEmptionCap   = macDrbQos->ngRanRetPri.preEmptionCap;
   schDrbQos->ngRanRetPri.preEmptionVul   = macDrbQos->ngRanRetPri.preEmptionVul;
   schDrbQos->grbQosFlowInfo.maxFlowBitRateDl = macDrbQos->grbQosInfo.maxFlowBitRateDl;
   schDrbQos->grbQosFlowInfo.maxFlowBitRateUl = macDrbQos->grbQosInfo.maxFlowBitRateUl;
   schDrbQos->grbQosFlowInfo.guarFlowBitRateDl= macDrbQos->grbQosInfo.guarFlowBitRateDl;
   schDrbQos->grbQosFlowInfo.guarFlowBitRateUl= macDrbQos->grbQosInfo.guarFlowBitRateUl;
   schDrbQos->pduSessionId = macDrbQos->pduSessionId;
   schDrbQos->ulPduSessAggMaxBitRate = macDrbQos->ulPduSessAggMaxBitRate;
}

/*******************************************************************
 *
 * @brief Fill SCH UL logical channel configuration
 *
 * @details
 *
 *    Function : fillSchUlLcCfg
 *
 *    Functionality: Fills Sch Ul Lc configuration
 *
 * @params[in] macLcCfg : Logical channel Cfg at MAC
 *             schLcCfg : LC cfg to fill at scheduler
 * @return void
 *
 * ****************************************************************/

void fillSchUlLcCfg(SchUlLcCfg *schUlLcCfg, UlLcCfg *macUlLcCfg) 
{
   schUlLcCfg->priority= macUlLcCfg->priority;
   schUlLcCfg->lcGroup = macUlLcCfg->lcGroup;
   schUlLcCfg->schReqId= macUlLcCfg->schReqId;
   schUlLcCfg->pbr    = macUlLcCfg->pbr;    
   schUlLcCfg->bsd    = macUlLcCfg->bsd;    
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
uint8_t fillLogicalChannelCfg(SchLcCfg *schLcCfg, LcCfg *macLcCfg)
{
   uint8_t sdIdx;
   uint8_t ret = ROK;
   schLcCfg->lcId = macLcCfg->lcId;
   schLcCfg->configType = macLcCfg->configType;
   schLcCfg->dlLcCfg.lcp = macLcCfg->dlLcCfg.lcp;
   fillSchUlLcCfg(&schLcCfg->ulLcCfg, &macLcCfg->ulLcCfg);

   if(macLcCfg->drbQos)
   {
     if(!schLcCfg->drbQos)
     {
        MAC_ALLOC(schLcCfg->drbQos, sizeof(SchDrbQosInfo));
	if(!schLcCfg->drbQos)
        {
           DU_LOG("\nMAC : Memory alloc failed at drbQos at fillLogicalChannelCfg()");
	   ret = RFAILED;
	}
     }
     if(ret == ROK)
     {
        fillSchDrbQosInfo(macLcCfg->drbQos, schLcCfg->drbQos);
     }
     else
        return ret;
   }
   else
   {
      schLcCfg->drbQos = NULLP;
   }

   if(ret == ROK)
   {
      if(macLcCfg->snssai)
      {
         if(!schLcCfg->snssai)
         {
            MAC_ALLOC(schLcCfg->snssai, sizeof(SchSnssai));
            if(!schLcCfg->snssai)
            {
               DU_LOG("\nMAC : Memory alloc failed at snssai at fillLogicalChannelCfg()");
               ret = RFAILED;
            }
         }
         if(ret == ROK)
         {
            schLcCfg->snssai->sst = macLcCfg->snssai->sst;
            for(sdIdx = 0; sdIdx < SD_SIZE; sdIdx++)
            {
              schLcCfg->snssai->sd[sdIdx] = macLcCfg->snssai->sd[sdIdx];
            }
         }
	 else
	 {
            schLcCfg->snssai = NULLP;
            /*Freeing the previously allocated buffer in case of failure */
            if(schLcCfg->drbQos)
            {
               MAC_FREE(schLcCfg->drbQos, sizeof(SchDrbQosInfo));
	       schLcCfg->drbQos = NULLP;
            }
	    return ret;
	 }
      }
      else
      {
         schLcCfg->snssai = NULLP;
      }
   }

   return ret;
}

/*******************************************************************
 *
 * @brief Fills Logical channel Cfg List to Add/Mod/Del
 *
 * @details
 *
 *    Function : fillSchLcCfgList
 *
 *    Functionality: Fills Logical channel Cfg List to Add/Mod/Del
 *
 * @params[in] MAC UE Cb Cfg , MAC UE Configuration
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillSchLcCfgList(SchUeCfg *schUeCfg, MacUeCfg *ueCfg)
{
   uint8_t lcIdx;

   for(lcIdx = 0; lcIdx < ueCfg->numLcs; lcIdx++)
   {
      if(fillLogicalChannelCfg(&schUeCfg->schLcCfg[lcIdx], &ueCfg->lcCfgList[lcIdx]) != ROK)
      {
	 DU_LOG("\nMAC : fillLogicalChannelCfg() failed for lc Idx[%d]", lcIdx);
	 return RFAILED;
      }
      schUeCfg->numLcs++;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills and sends UE configuration to Scheduler
 *
 * @details
 *
 *    Function : fillSchUeCfg
 *
 *    Functionality: Fills and sends UE configuration to Scheduler
 *
 * @params[in] Ue configuration from DU APP
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillSchUeCfg(Pst *pst, SchUeCfg *schUeCfg, MacUeCfg *ueCfg)
{
   uint8_t ret = ROK;

   schUeCfg->cellId = ueCfg->cellId;
   schUeCfg->crnti = ueCfg->crnti;

   /* Copy MAC cell group config */
   memset(&schUeCfg->macCellGrpCfg, 0, sizeof(SchMacCellGrpCfg));
   if(fillMacCellGroupCfg(ueCfg->macCellGrpCfg, &schUeCfg->macCellGrpCfg) != ROK)
   {
      DU_LOG("\nMAC : fillMacCellGroupCfg() failed");
      return RFAILED;
   }

   /* Copy Physical cell group config */
   memset(&schUeCfg->phyCellGrpCfg, 0,sizeof(SchPhyCellGrpCfg));
   if(fillPhyCellGroupCfg(ueCfg->phyCellGrpCfg, &schUeCfg->phyCellGrpCfg) != ROK)
   {
      DU_LOG("\nMAC : fillPhyCellGroupCfg() failed");
      return RFAILED;
   }

   /* Copy sp cell config */
   memset(&schUeCfg->spCellCfg, 0, sizeof(SchSpCellCfg));
   if(fillSpCellCfg(ueCfg->spCellCfg, &schUeCfg->spCellCfg) != ROK)
   {
      DU_LOG("\nMAC : fillSpCellCfg() failed");
      return RFAILED;
   }

   if(ueCfg->ambrCfg != NULLP)
   {
      MAC_ALLOC(schUeCfg->ambrCfg, sizeof(SchAmbrCfg));
      if(!schUeCfg->ambrCfg)
      {
	 DU_LOG("\nMAC : Memory allocation failed in sendReconfigReqToSch");
	 return RFAILED;
      }
      schUeCfg->ambrCfg->ulBr = ueCfg->ambrCfg->ulBr;
      schUeCfg->ambrCfg->dlBr = ueCfg->ambrCfg->dlBr;
   }
   else
      schUeCfg->ambrCfg = NULLP;

   /* Fill sch Lc Cfg  to Add/ Mod/ Del */
   ret  = fillSchLcCfgList(schUeCfg, ueCfg);
   if(ret == RFAILED)
   {
      DU_LOG("\nMAC : Failed to copy LCs at fillSchUeCfg()");
      return ret;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Update UeUlCb Lc List
 *
 * @details
 *
 *    Function : updateMacUlCb
 *
 *    Functionality: Update UeUlCb Lc List
 *
 * @params[in] delIdx, UeUlCb pointer
 * @return void
 *
 * ****************************************************************/

void updateMacUlCb(uint8_t delIdx, UeUlCb *ulCb)
{
   uint8_t lcIdx = 0;

   for(lcIdx = delIdx; lcIdx < ulCb->numUlLc; lcIdx++)
   {
      /* Moving the array element to one step ahead */
      memcpy(&ulCb->lcCb[lcIdx], &ulCb->lcCb[lcIdx+1], sizeof(UlLcCb));
      memset(&ulCb->lcCb[lcIdx+1], 0, sizeof(UlLcCb));
   }
}

/*******************************************************************
 *
 * @brief Update UeDlCb Lc List
 *
 * @details
 *
 *    Function : updateMacDlCb
 *
 *    Functionality: Update UeDlCb Lc List
 *
 * @params[in] delIdx, UeDlCb pointer
 * @return void
 *
 * ****************************************************************/

void updateMacDlCb(uint8_t delIdx, UeDlCb *dlCb)
{
   uint8_t lcIdx = 0;

   for(lcIdx = delIdx; lcIdx < dlCb->numDlLc; lcIdx++)
   {
      /* Moving the array element to one step ahead */
      memcpy(&dlCb->lcCb[lcIdx], &dlCb->lcCb[lcIdx+1], sizeof(DlLcCb));
      memset(&dlCb->lcCb[lcIdx+1], 0, sizeof(DlLcCb));
   }
}

/*******************************************************************
 *
 * @brief Fills Logical channel Cfg List to Add/Mod/Del
 *
 * @details
 *
 *    Function : fillMacLcCfgList
 *
 *    Functionality: Fills Logical channel Cfg List to Add/Mod/Del
 *
 * @params[in] MAC UE Cb Cfg , MAC UE Configuration
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillMacLcCfgList(MacUeCb *ueCb, MacUeCfg *ueCfg)
{
   uint8_t lcIdx, ueLcIdx;

   for(lcIdx = 0; lcIdx < ueCfg->numLcs; lcIdx++)
   {
      if(ueCb->dlInfo.numDlLc < MAX_NUM_LC)
      {
         if(ueCfg->lcCfgList[lcIdx].configType == CONFIG_ADD)
         {
	    /*Filling DL LC CB */
            ueCb->dlInfo.lcCb[ueCb->dlInfo.numDlLc].lcId = ueCfg->lcCfgList[lcIdx].lcId;
            ueCb->dlInfo.lcCb[ueCb->dlInfo.numDlLc].lcState = MAC_LC_STATE_ACTIVE;
            ueCb->dlInfo.numDlLc++;
	    /*Filling UL LC CB */
            ueCb->ulInfo.lcCb[ueCb->ulInfo.numUlLc].lcId = ueCfg->lcCfgList[lcIdx].lcId;
            ueCb->ulInfo.lcCb[ueCb->ulInfo.numUlLc].lcGrpId = ueCfg->lcCfgList[lcIdx].ulLcCfg.lcGroup;
            ueCb->ulInfo.lcCb[ueCb->ulInfo.numUlLc].lcActive = MAC_LC_STATE_ACTIVE;
            ueCb->ulInfo.numUlLc++;
         }/*End of Add Config */
         else
         { 
	    //searching for Lc to be Mod
	    for(ueLcIdx = 0; ueLcIdx < ueCb->ulInfo.numUlLc; ueLcIdx++)
	    {
               if(ueCb->ulInfo.lcCb[ueLcIdx].lcId == ueCfg->lcCfgList[lcIdx].lcId)
               {
	          if(ueCfg->lcCfgList[lcIdx].configType == CONFIG_MOD)
		  {
	             /*Nothing to Modify in DL LC CB */
                     /*Modify UL LC CB */
                     ueCb->ulInfo.lcCb[ueLcIdx].lcGrpId = ueCfg->lcCfgList[lcIdx].ulLcCfg.lcGroup;
                     DU_LOG("\nMAC: Successfully Modified LC context for lcId[%d]", ueCfg->lcCfgList[lcIdx].lcId);
                     break;
		  }
	          if(ueCfg->lcCfgList[lcIdx].configType == CONFIG_DEL)
		  {
                     memset(&ueCb->dlInfo.lcCb[ueLcIdx], 0, sizeof(DlLcCb));
                     (ueCb->dlInfo.numDlLc)--;
                     updateMacDlCb(ueLcIdx, &ueCb->dlInfo);

                     memset(&ueCb->ulInfo.lcCb[ueLcIdx], 0, sizeof(UlLcCb));
                     (ueCb->ulInfo.numUlLc)--;
                     updateMacUlCb(ueLcIdx, &ueCb->ulInfo);
                     DU_LOG("\nMAC: Successfully Deleted LC context for lcId[%d]", ueCfg->lcCfgList[lcIdx].lcId);
                     break;
		  }
	       }
	    }
         }/*End of Mod Config */
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills MAC UE Cb Cfg
 *
 * @details
 *
 *    Function : fillMacUeCb
 *
 *    Functionality: Fills MAC UE Cb Cfg
 *
 * @params[in] MAC UE Cb Cfg , MAC UE Configuration
 *             cellIdx
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillMacUeCb(MacUeCb *ueCb, MacUeCfg *ueCfg, uint8_t cellIdx)
{
   uint8_t ret = ROK;

   ueCb->ueIdx = ueCfg->ueIdx;
   ueCb->crnti = ueCfg->crnti;
   ueCb->cellCb = macCb.macCell[cellIdx];
   ueCb->dlInfo.dlHarqEnt.numHarqProcs = \
      ueCfg->spCellCfg.servCellCfg.pdschServCellCfg.numHarqProcForPdsch; 
   ueCb->state = UE_STATE_ACTIVE;
   /*TODO: To check the bsr value during implementation */
   ueCb->bsrTmrCfg.periodicTimer = ueCfg->macCellGrpCfg.bsrTmrCfg.periodicTimer;
   ueCb->bsrTmrCfg.retxTimer     = ueCfg->macCellGrpCfg.bsrTmrCfg.retxTimer;
   ueCb->bsrTmrCfg.srDelayTimer  = ueCfg->macCellGrpCfg.bsrTmrCfg.srDelayTimer;
   ret = fillMacLcCfgList(ueCb, ueCfg);
   if(ret == RFAILED)
   {
      DU_LOG("\nMAC: Failed while filing MAC LC List at fillMacUeCb()");
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Function to update Mac Ra Cb
 *
 * @details
 *
 *    Function : updateMacRaCb
 *
 *    Functionality: Function to update Mac Ra Cb
 *
 * @params[in] cellIdx, Mac Ue Cb
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t updateMacRaCb(uint16_t cellIdx, MacUeCb *ueCb)
{
   uint8_t ueIdx;
   /* Copy RA Cb */
   for(ueIdx = 0; ueIdx < MAX_NUM_UE; ueIdx++)
   {
      if(macCb.macCell[cellIdx]->macRaCb[ueIdx].crnti == ueCb->crnti)
      {
         ueCb->raCb = &macCb.macCell[cellIdx]->macRaCb[ueIdx];
         break;
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Function to delete Mac Ra Cb
 *
 * @details
 *
 *    Function : deleteMacRaCb
 *
 *    Functionality: Function to delete Mac Ra Cb
 *
 * @params[in] cellIdx, Mac Ue Cb
 * @return void
 *
 * ****************************************************************/

void deleteMacRaCb(uint16_t cellIdx, MacUeCb *ueCb)
{
   uint8_t ueIdx;

   for(ueIdx = 0; ueIdx < MAX_NUM_UE; ueIdx++)
   {
      if(macCb.macCell[cellIdx]->macRaCb[ueIdx].crnti == ueCb->crnti)
      {
         if(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4Pdu)
	 {
	   MAC_FREE(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4Pdu, \
		     macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4PduLen);
         }
	 if(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TxPdu)
	 {
            MAC_FREE(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TxPdu, \
                      macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TbSize);
         }
	 memset(&macCb.macCell[cellIdx]->macRaCb[ueIdx], 0, sizeof(MacRaCbInfo));
         break;
      }
   }
	          
}

/*******************************************************************
 *
 * @brief Creates UE Cb
 *
 * @details
 *
 *    Function : createUeCb
 *
 *    Functionality: Creates UE Cb
 *
 * @params[in] MAC UE Configuration
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t createUeCb(uint8_t cellIdx, MacUeCb *ueCb, MacUeCfg *ueCfg)
{
   uint8_t ret =ROK;

   if((ueCb->ueIdx == ueCfg->ueIdx) && (ueCb->crnti == ueCfg->crnti)\
      &&(ueCb->state == UE_STATE_ACTIVE))
   {
      DU_LOG("\n MAC : CRNTI %d already configured ", ueCfg->crnti);
      return ROKDUP;
   }
   else
   {
      memset(ueCb, 0, sizeof(MacUeCb));
      ret = fillMacUeCb(ueCb, ueCfg, cellIdx);
      if(ret != ROK)
      {
         DU_LOG("\nMAC : Failed to create Ue Cb at createUeCb()");
         return ret;
      }
      else
      {
         macCb.macCell[cellIdx]->numActvUe++;
         updateMacRaCb(cellIdx, ueCb);
	 return ROK;
      }

   }

}

/*******************************************************************
 *
 * @brief Modify UE Cb Cfg
 *
 * @details
 *
 *    Function : modifyUeCb
 *
 *    Functionality: modify UE Cb
 *
 * @params[in] MAC UE Configuration
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t modifyUeCb(uint8_t cellIdx, MacUeCb *ueCb, MacUeCfg *ueCfg)
{
   uint8_t ret = ROK;

   if((ueCb->ueIdx == ueCfg->ueIdx) && (ueCb->crnti == ueCfg->crnti)\
      &&(ueCb->state == UE_STATE_ACTIVE))
   {
      DU_LOG("\n MAC : Modifying Ue config Req for CRNTI %d ", ueCfg->crnti);
      ret = fillMacUeCb(ueCb, ueCfg, cellIdx);
      if(ret != ROK)
      {
         DU_LOG("\nMAC : Failed to modify MacUeCb at modifyUeCb()");
         return ret;
      }
      else
      {
         deleteMacRaCb(cellIdx, ueCb);
	 return ROK;
      }
   }
   return RFAILED;
}


/*******************************************************************
 *
 * @brief Creates UE Cb and fills ueCfg
 *
 * @details
 *
 *    Function : procMacUeCfgData
 *
 *    Functionality: Creates UE Cb and fills ueCfg
 *
 * @params[in] MAC UE Configuration
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t procMacUeCfgData(Pst *pst, MacUeCfg *ueCfg)
{
   uint8_t ret = ROK;
   uint16_t  cellIdx;
   MacUeCb   *ueCb = NULLP;


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
   ueCb = &macCb.macCell[cellIdx]->ueCb[ueCfg->ueIdx -1];
   switch(pst->event)
   {
      case EVENT_UE_CONFIG_RSP_TO_MAC:
	 ret = createUeCb(cellIdx, ueCb, ueCfg);
	 if(ret != ROK)
            DU_LOG("\nMAC: AddUeConfigReq for cellIdx :%d failed in procMacUeCfgData()", cellIdx);
	 break;
      case EVENT_UE_RECONFIG_RSP_TO_MAC:
	 ret = modifyUeCb(cellIdx, ueCb, ueCfg);
	 if(ret != ROK)
            DU_LOG("\nMAC: ModifyUeConfigReq for cellIdx :%d failed at procMacUeCfgData()", cellIdx);
	 break;
      default:
	 break;
   }

   return ret;
}

/*******************************************************************
 *
 * @brief Function to store the UeCfg Data 
 *
 * @details
 *
 *    Function : copyToTmpData
 *
 *    Functionality: Function to store the UeCfg Data
 *
 * @params[in] MacUeCfg pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t copyToTmpData(MacUeCfg *ueCfg)
{
   uint8_t cellIdx;
   MacUeCfg *tmpData = NULLP;

   MAC_ALLOC(tmpData, sizeof(MacUeCfg));
   if(!tmpData)
   {
      DU_LOG("\nMAC: Memory Alloc Failed at copyToTmpData()");
      return RFAILED;
   }
   memcpy(tmpData, ueCfg, sizeof(MacUeCfg));
   GET_CELL_IDX(ueCfg->cellId, cellIdx);
   macCb.macCell[cellIdx]->ueCfgTmpData[ueCfg->ueIdx-1] = tmpData;
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
   uint8_t ret = ROK;
   SchUeCfg   schUeCfg;
   memset(&schUeCfg, 0, sizeof(SchUeCfg));

   DU_LOG("\nMAC : UE Create Request for CRNTI[%d]", ueCfg->crnti);

   if(ueCfg)
   {
      /*Storing received ueCfg in ueCfgTmpData */
      ret = copyToTmpData(ueCfg);
      if(ret == ROK)
      {
         /*Sending Cfg Req to SCH */
	 ret = fillSchUeCfg(pst, &schUeCfg, ueCfg);
	 if(ret != ROK)
	    DU_LOG("\nMAC : Failed to fill Sch Ue Cfg at MacProcUeCreateReq()");
	 else
	 {
            /* Fill event and send UE create request to SCH */
            ret = sendUeReqToSch(pst, &schUeCfg);
	    if(ret != ROK)
	       DU_LOG("\nMAC : Failed to send UE Create request to SCH");
	 }
      }
      else 
      {
	 DU_LOG("\nMAC : Failed to store MAC UE CFG ");
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
   return (*macDuUeCfgRspOpts[rspPst.selector])(&rspPst, cfgRsp); 
}

/*******************************************************************
 *
 * @brief Fill and Send UE Reconfig response from MAC to DU APP
 *
 * @details
 *
 *    Function : MacSendUeReconfigRsp
 *
 *    Functionality: Fill and Send UE Reconfig response from MAC to DUAPP
 *
 * @params[in] MAC UE create result
 *             SCH UE create response
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacSendUeReconfigRsp(MacRsp result, SchUeCfgRsp *schCfgRsp)
{
   MacUeCfgRsp   *cfgRsp;
   Pst        rspPst;

   MAC_ALLOC_SHRABL_BUF(cfgRsp, sizeof(MacUeCfgRsp));
   if(!cfgRsp)
   {
      DU_LOG("\nMAC: Memory allocation for UE Reconfig response failed");
      return RFAILED;
   }

   /* Filling UE Config response */
   memset(cfgRsp, 0, sizeof(MacUeCfgRsp));
   cfgRsp->cellId = schCfgRsp->cellId;
   cfgRsp->ueIdx = schCfgRsp->ueIdx;
   cfgRsp->result = result;

   /* Fill Post structure and send UE Create response*/
   memset(&rspPst, 0, sizeof(Pst));
   FILL_PST_MAC_TO_DUAPP(rspPst, EVENT_MAC_UE_RECONFIG_RSP);
   return (*macDuUeCfgRspOpts[rspPst.selector])(&rspPst, cfgRsp);
}

/*******************************************************************
 *
 * @brief  Function to return Mac Ue Cfg pointer
 *
 * @details
 *
 *    Function : getMacUeCfg
 *
 *    Functionality:
 *      Function to return Mac Ue Cfg pointer
 *
 * @params[in] cellIdx, ueIdx
 *
 * @return MacUeCfg pointer - success
 *         NULLP - failure
 *
 * ****************************************************************/

MacUeCfg *getMacUeCfg(uint16_t cellIdx, uint8_t ueIdx)
{
   MacUeCfg *ueCfg = NULLP;
   if(macCb.macCell[cellIdx])
   {
      ueCfg = macCb.macCell[cellIdx]->ueCfgTmpData[ueIdx-1];
   }
   else
   {
      DU_LOG("\nMAC: Failed to get macCellCb in getMacUeCfg()");
   }
   return ueCfg;
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
   MacUeCfg *ueCfg = NULLP;

   GET_CELL_IDX(schCfgRsp->cellId, cellIdx);
   ueCfg = getMacUeCfg(cellIdx, schCfgRsp->ueIdx);
   if(ueCfg == NULLP)
   {
      DU_LOG("\nMAC : Failed to find the Mac Ue Cfg for event [%d] in MacProcSchUeCfgRsp()", pst->event);
      ret = RFAILED;
   }

   switch(pst->event)
   {
      case EVENT_UE_CONFIG_RSP_TO_MAC:
      {
         if(schCfgRsp->rsp != RSP_NOK)
	 {
            DU_LOG("\nMAC: SCH UeConfigRsp for CRNTI[%d] is success in MacProcSchUeCfgRsp()", schCfgRsp->crnti);
	    if(ret == ROK)
	    {
	       ret = procMacUeCfgData(pst, ueCfg);
	       if(ret == ROK)
	       {
                  result = MAC_DU_APP_RSP_OK;
               }
	    }
	 }
	 else
	 {
            DU_LOG("\nMAC: SCH UeConfigRsp for CRNTI[%d] is failed in MacProcSchUeCfgRsp()", schCfgRsp->crnti);
	 }
         ret = MacSendUeCreateRsp(result, schCfgRsp);
      }
      break;

      case EVENT_UE_RECONFIG_RSP_TO_MAC:
      {
         if(schCfgRsp->rsp != RSP_NOK)
	 {
            DU_LOG("\nMAC: SCH UeReconfigRsp for CRNTI[%d] is success in MacProcSchUeCfgRsp()", schCfgRsp->crnti);
	    if(ret == ROK)
	    {
	       ret = procMacUeCfgData(pst, ueCfg);
	       if(ret == ROK)
	       {
                  result = MAC_DU_APP_RSP_OK;
               }
	    }
	 }
	 else
	 {
            DU_LOG("\nMAC: SCH UeReconfigRsp for CRNTI[%d] is failed in MacProcSchUeCfgRsp()", schCfgRsp->crnti);
	 }
         ret = MacSendUeReconfigRsp(result, schCfgRsp);
      }
      break;
      
      default:
      break;
   }
   MAC_FREE(ueCfg, sizeof(MacUeCfg));
   ueCfg = NULLP;
   
   return ret; 
}

/*******************************************************************
 *
 * @brief Handles UE Reconfig requst from DU APP
 *
 * @details
 *
 *    Function : MacProcUeReconfigReq
 *
 *    Functionality: Handles UE Reconfig requst from DU APP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacProcUeReconfigReq(Pst *pst, MacUeCfg *ueCfg)
{
   uint8_t ret = ROK;
   SchUeCfg   schUeCfg;
   memset(&schUeCfg, 0, sizeof(SchUeCfg));

   DU_LOG("\nMAC : UE Reconfig Request for CRNTI[%d]", ueCfg->crnti);

   if(ueCfg)
   {
      /*Storing received ueCfg in ueCfgTmpData */
      ret = copyToTmpData(ueCfg);
      if(ret == ROK)
      {
         /*Sending Cfg Req to SCH */
	 ret = fillSchUeCfg(pst, &schUeCfg, ueCfg);
	 if(ret != ROK)
	    DU_LOG("\nMAC : Failed to fill sch Ue Cfg at MacProcUeReconfigReq()");
         else
	 {
            /* Fill event and send UE create request to SCH */
            ret = sendUeReqToSch(pst, &schUeCfg);
	    if(ret != ROK)
	       DU_LOG("\nMAC : Failed to send UE Reconfig Request to SCH");
	 }
      }
      else 
      {
	 DU_LOG("\nMAC : Failed to store MAC UE Cb ");
      }
   }
   else
   {
      DU_LOG("\nMAC : MAC UE Create request processing failed");
      ret = RFAILED;
   }
   /* FREE shared memory */
   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, ueCfg, sizeof(MacUeCfg));
   return ROK;
}


/**********************************************************************
  End of file
 **********************************************************************/
