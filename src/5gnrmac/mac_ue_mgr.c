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

MacSchUeDeleteReqFunc macSchUeDeleteReqOpts[] =
{
   packMacSchUeDeleteReq,    /* packing for loosely coupled */
   MacSchUeDeleteReq,        /* packing for tightly coupled */
   packMacSchUeDeleteReq     /* packing for light weight loosely coupled */
};

MacDuUeDeleteRspFunc macDuUeDeleteRspOpts[] =
{
   packDuMacUeDeleteRsp,   /* packing for loosely coupled */
   DuProcMacUeDeleteRsp,   /* packing for tightly coupled */
   packDuMacUeDeleteRsp   /* packing for light weight loosly coupled */
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
      DU_LOG("\nERROR  -->  MAC : Scheduling Request Add/Mod Count %d exceeds max limit %d", \
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
      DU_LOG("\nERROR  -->  MAC : Scheduling Request Add/Mod Count %d exceeds max limit %d", \
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
         MAC_ALLOC(schPdschCfg->maxMimoLayers, sizeof(uint8_t));
	 if(!schPdschCfg->maxMimoLayers)
	 {
            DU_LOG("\nERROR  -->  MAC :Memory Alloc MimoLayers Failed at fillPdschServCellCfg()");
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
         MAC_ALLOC(schPdschCfg->maxCodeBlkGrpPerTb, sizeof(SchMaxCodeBlkGrpPerTB));
	 if(!schPdschCfg->maxCodeBlkGrpPerTb)
	 {
            DU_LOG("\nERROR  -->  MAC :Memory Alloc for code Block Failed at fillPdschServCellCfg()");
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
         MAC_ALLOC(schPdschCfg->codeBlkGrpFlushInd, sizeof(bool));
	 if(!schPdschCfg->codeBlkGrpFlushInd)
	 {
            DU_LOG("\nERROR  -->  MAC :Memory Alloc for Flush Ind Failed at fillPdschServCellCfg()");
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
         MAC_ALLOC(schPdschCfg->xOverhead, sizeof(SchPdschXOverhead));
	 if(!schPdschCfg->xOverhead)
	 {
            DU_LOG("\nERROR  -->  MAC :Memory Alloc for xOverHead Failed at fillPdschServCellCfg()");
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
      DU_LOG("\nERROR  -->  MAC : Number of Time domain resource allocation [%d] exceeds max limit [%d]",\
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
 * @brief Function to fillResrcSetList sent by DU
 *
 * @details
 *
 *    Function : fillResrcSetList
 *
 *    Functionality: Function to fillResrcSetList sent by DU
 *
 * @params[in] PucchResrcSetCfg pointer,
 *             SchPucchResrcSetCfg pointer
 * @return void
 *
 * ****************************************************************/

void fillResrcSetList(PucchResrcSetCfg *macRsrcSetList, SchPucchResrcSetCfg *schRsrcSetList)
{
   uint8_t arrIdx, rsrcListIdx;

   /* ResrcSetToAddModList */
   schRsrcSetList->resrcSetToAddModListCount  = macRsrcSetList->resrcSetToAddModListCount;
   for(arrIdx = 0; arrIdx < macRsrcSetList->resrcSetToAddModListCount; arrIdx++)
   {
     schRsrcSetList->resrcSetToAddModList[arrIdx].resrcSetId =\
        macRsrcSetList->resrcSetToAddModList[arrIdx].resrcSetId;
     
     schRsrcSetList->resrcSetToAddModList[arrIdx].maxPayLoadSize =\
        macRsrcSetList->resrcSetToAddModList[arrIdx].maxPayLoadSize;
     
     schRsrcSetList->resrcSetToAddModList[arrIdx].resrcListCount =\
        macRsrcSetList->resrcSetToAddModList[arrIdx].resrcListCount;
     for(rsrcListIdx = 0; rsrcListIdx < macRsrcSetList->resrcSetToAddModList[arrIdx].\
         resrcListCount; rsrcListIdx++)
      {
         schRsrcSetList->resrcSetToAddModList[arrIdx].resrcList[rsrcListIdx] =\
         macRsrcSetList->resrcSetToAddModList[arrIdx].resrcList[rsrcListIdx];
      }
   }
   /* ResrcSetToRelList */
   schRsrcSetList->resrcSetToRelListCount = macRsrcSetList->resrcSetToRelListCount;
   for(arrIdx = 0; arrIdx < macRsrcSetList->resrcSetToRelListCount; arrIdx++)
   {
      schRsrcSetList->resrcSetToRelList[arrIdx] = macRsrcSetList->resrcSetToRelList[arrIdx]; 
   }
}

/*******************************************************************
 *
 * @brief Function to fillResrcList sent by DU
 *
 * @details
 *
 *    Function : fillResrcList
 *
 *    Functionality: Function to fillResrcList sent by DU
 *
 * @params[in] PucchResrcCfg pointer,
 *             SchPucchResrcCfg pointer
 * @return ROK/RFAILED;
 *
 * ****************************************************************/
uint8_t fillResrcList(PucchResrcCfg *macResrcCfg, SchPucchResrcCfg *schResrcCfg)
{
   uint8_t arrIdx;
   
   schResrcCfg->resrcToAddModListCount = macResrcCfg->resrcToAddModListCount;
   for(arrIdx=0; arrIdx < macResrcCfg->resrcToAddModListCount; arrIdx++)
   {
      schResrcCfg->resrcToAddModList[arrIdx].resrcId =\
         macResrcCfg->resrcToAddModList[arrIdx].resrcId;
      schResrcCfg->resrcToAddModList[arrIdx].startPrb =\
         macResrcCfg->resrcToAddModList[arrIdx].startPrb;
      schResrcCfg->resrcToAddModList[arrIdx].intraFreqHop =\
         macResrcCfg->resrcToAddModList[arrIdx].intraFreqHop;
      schResrcCfg->resrcToAddModList[arrIdx].secondPrbHop =\
         macResrcCfg->resrcToAddModList[arrIdx].secondPrbHop;
      schResrcCfg->resrcToAddModList[arrIdx].pucchFormat =\
         macResrcCfg->resrcToAddModList[arrIdx].pucchFormat;
      if(macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format0)
      {
         MAC_ALLOC(schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format0, sizeof(SchPucchFormat0));
         if(schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format0 == NULLP)
         {
             DU_LOG("\nERROR  -->  MAC : Failed to allocate memory for Format0 in fillResrcList()");
             return RFAILED;
         }
         schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format0->initialCyclicShift =\
            macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format0->initialCyclicShift;
         schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format0->numSymbols =\
            macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format0->numSymbols;
         schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format0->startSymbolIdx =\
            macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format0->startSymbolIdx;
      }
         
      if(macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format1)
      {
         MAC_ALLOC(schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format1, sizeof(SchPucchFormat1));
         if(schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format1 == NULLP)
         {
             DU_LOG("\nERROR  -->  MAC : Failed to allocate memory for Format1 in fillResrcList()");
             return RFAILED;
         }
         schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format1->initialCyclicShift =\
            macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format1->initialCyclicShift;
         schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format1->numSymbols =\
            macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format1->numSymbols;
         schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format1->startSymbolIdx =\
            macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format1->startSymbolIdx;
        	schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format1->timeDomOCC =\
            macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format1->timeDomOCC;
      }

      if(macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format2)
      {
         MAC_ALLOC(schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format2, sizeof(SchPucchFormat2_3));
         if(schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format2 == NULLP)
         {
             DU_LOG("\nERROR  --> MAC : Failed to allocate memory for Format2 in fillResrcList()");
             return RFAILED;
         }
         schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format2->numPrbs =\
            macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format2->numPrbs;
         schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format2->numSymbols =\
            macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format2->numSymbols;
         schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format2->startSymbolIdx =\
            macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format2->startSymbolIdx;
      }

      if(macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format3)
      {
         MAC_ALLOC(schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format3, sizeof(SchPucchFormat2_3));
         if(schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format3 == NULLP)
         {
             DU_LOG("\nERROR  --> MAC : Failed to allocate memory for Format3 in fillResrcList()");
             return RFAILED;
         }
         schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format3->numPrbs =\
            macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format3->numPrbs;
         schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format3->numSymbols =\
            macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format3->numSymbols;
         schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format3->startSymbolIdx =\
            macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format3->startSymbolIdx;
      }

      if(macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format4)
      {
         MAC_ALLOC(schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format4, sizeof(SchPucchFormat4));
         if(schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format4 == NULLP)
         {
             DU_LOG("\nERROR  --> MAC : Failed to allocate memory for Format4 in fillResrcList()");
             return RFAILED;
         }
         schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format4->numSymbols =\
            macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format4->numSymbols;
         schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format4->startSymbolIdx =\
            macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format4->startSymbolIdx;
         schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format4->occLen =\
            macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format4->occLen;
         schResrcCfg->resrcToAddModList[arrIdx].SchPucchFormat.format4->occIdx =\
            macResrcCfg->resrcToAddModList[arrIdx].PucchFormat.format4->occIdx;
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Function to fillPucchFormatCfg sent by DU
 *
 * @details
 *
 *    Function : fillPucchFormatCfg
 *
 *    Functionality: Function to fillPucchFormatCfg
 *
 * @params[in] PucchFormatCfg pointer,
 *             SchPucchFormatCfg pointer
 * @return void
 *
 * ****************************************************************/

void fillPucchFormatCfg(PucchFormatCfg *macFormatCfg, SchPucchFormatCfg *schFormatCfg)
{
   schFormatCfg->interSlotFreqHop  = macFormatCfg->interSlotFreqHop; 
   schFormatCfg->addDmrs           = macFormatCfg->addDmrs;        
   schFormatCfg->maxCodeRate       = macFormatCfg->maxCodeRate;
   schFormatCfg->numSlots          = macFormatCfg->numSlots;
   schFormatCfg->pi2BPSK           = macFormatCfg->pi2BPSK;
   schFormatCfg->harqAckCSI        = macFormatCfg->harqAckCSI;
}

/*******************************************************************
 *
 * @brief Function to fillPucchSchedReqCfg sent by DU
 *
 * @details
 *
 *    Function : fillPucchSchedReqCfg
 *
 *    Functionality: Function to fillPucchSchedReqCfg
 *
 * @params[in] PucchSchedReqCfg pointer,
 *             SchPucchSchedReqCfg pointer
 * @return void
 *
 * ****************************************************************/
void fillPucchSchedReqCfg(PucchSchedReqCfg *macSchedReqCfg, SchPucchSchedReqCfg *schSchedReqCfg)
{
   uint8_t arrIdx;

   schSchedReqCfg->schedAddModListCount = macSchedReqCfg->schedAddModListCount;
   for(arrIdx=0; arrIdx < schSchedReqCfg->schedAddModListCount; arrIdx++)
   {
      schSchedReqCfg->schedAddModList[arrIdx].resrcId = macSchedReqCfg->schedAddModList[arrIdx].resrcId;
      schSchedReqCfg->schedAddModList[arrIdx].requestId = macSchedReqCfg->schedAddModList[arrIdx].requestId;
      schSchedReqCfg->schedAddModList[arrIdx].periodicity =\
         macSchedReqCfg->schedAddModList[arrIdx].periodicity;
      schSchedReqCfg->schedAddModList[arrIdx].offset =\
         macSchedReqCfg->schedAddModList[arrIdx].offset;
      schSchedReqCfg->schedAddModList[arrIdx].resrc =\
         macSchedReqCfg->schedAddModList[arrIdx].resrc;
   }
   schSchedReqCfg->schedRelListCount = macSchedReqCfg->schedRelListCount;
   for(arrIdx=0; arrIdx < schSchedReqCfg->schedRelListCount; arrIdx++)
   {
      schSchedReqCfg->schedRelList[arrIdx] = macSchedReqCfg->schedRelList[arrIdx];
   }
}

/*******************************************************************
 *
 * @brief Function to fillMultiCsiCfg sent by DU
 *
 * @details
 *
 *    Function : fillMultiCsiCfg
 *
 *    Functionality: Function to fillMultiCsiCfg
 *
 * @params[in] PucchMultiCsiCfg pointer,
 *             SchPucchMultiCsiCfg pointer
 * @return void
 *
 * ****************************************************************/

 void fillMultiCsiCfg(PucchMultiCsiCfg *macCsiCfg, SchPucchMultiCsiCfg *schCsiCfg)
 {
    uint8_t arrIdx;

    schCsiCfg->multiCsiResrcListCount = macCsiCfg->multiCsiResrcListCount;
    for(arrIdx =0; arrIdx < schCsiCfg->multiCsiResrcListCount; arrIdx++)
    {
       schCsiCfg->multiCsiResrcList[arrIdx] = macCsiCfg->multiCsiResrcList[arrIdx]; 
    }
 }

/*******************************************************************
 *
 * @brief Function to fillDlToUlAck sent by DU
 *
 * @details
 *
 *    Function : fillDlToUlAck
 *
 *    Functionality: Function to fillDlToUlAck
 *
 * @params[in] PucchDlDataToUlAck pointer,
 *             SchPucchDlDataToUlAck pointer
 * @return void
 *
 * ****************************************************************/

void fillDlToUlAck(PucchDlDataToUlAck *macUlAck, SchPucchDlDataToUlAck *schUlAck)
{
   uint8_t arrIdx;

   schUlAck->dlDataToUlAckListCount = macUlAck->dlDataToUlAckListCount;
   for(arrIdx =0; arrIdx < macUlAck->dlDataToUlAckListCount; arrIdx++)
   {
      schUlAck->dlDataToUlAckList[arrIdx] = macUlAck->dlDataToUlAckList[arrIdx];
   }
}

/*******************************************************************
 *
 * @brief Function to fillPucchPowerControl sent by DU
 *
 * @details
 *
 *    Function : fillPucchPowerControl
 *
 *    Functionality: Function to fillPucchPowerControl
 *
 * @params[in] PucchPowerControl pointer,
 *             SchPucchPowerControl pointer
 * @return void
 *
 * ****************************************************************/

void fillPucchPowerControl(PucchPowerControl *macPwrCtrl, SchPucchPowerControl *schPwrCtrl)
{
   uint8_t arrIdx;

   schPwrCtrl->deltaF_Format0 = macPwrCtrl->deltaF_Format0;
   schPwrCtrl->deltaF_Format1 = macPwrCtrl->deltaF_Format1;
   schPwrCtrl->deltaF_Format2 = macPwrCtrl->deltaF_Format2;
   schPwrCtrl->deltaF_Format3 = macPwrCtrl->deltaF_Format3;
   schPwrCtrl->deltaF_Format4 = macPwrCtrl->deltaF_Format4;
   schPwrCtrl->p0SetCount = macPwrCtrl->p0SetCount;
   for(arrIdx =0; arrIdx <  macPwrCtrl->p0SetCount; arrIdx++)
   {
      schPwrCtrl->p0Set[arrIdx].p0PucchId = macPwrCtrl->p0Set[arrIdx].p0PucchId;
      schPwrCtrl->p0Set[arrIdx].p0PucchVal = macPwrCtrl->p0Set[arrIdx].p0PucchVal;
   }
   schPwrCtrl->pathLossRefRSListCount = macPwrCtrl->pathLossRefRSListCount;
   for(arrIdx = 0; arrIdx < macPwrCtrl->pathLossRefRSListCount; arrIdx++)
   {
      schPwrCtrl->pathLossRefRSList[arrIdx].pathLossRefRSId =\
      macPwrCtrl->pathLossRefRSList[arrIdx].pathLossRefRSId;
   }
}

/*******************************************************************
 *
 * @brief Function to fillOtherPucchFormatCfg sent by DU
 *
 * @details
 *
 *    Function : fillOtherPucchFormatCfg
 *
 *    Functionality: Function to fillOtherPucchFormatCfg
 *
 * @params[in] PucchFormat
 *             PucchFormatCfg pointer,
 *             SchPucchCfg pointer
 * @return void
 *
 * ****************************************************************/

uint8_t fillOtherPucchFormatCfg(uint8_t pucchFormat, PucchFormatCfg *macFormatCfg, SchPucchCfg *schPucchCfg)
{

   switch(pucchFormat)
   {
      case PUCCH_FORMAT_1:
      {
         if(macFormatCfg)
	 {
            MAC_ALLOC(schPucchCfg->format1, sizeof(SchPucchFormatCfg));
            if(schPucchCfg->format1 == NULLP)
            {
               DU_LOG("\nERROR  --> MAC : Failed to allocate Mem for Format 1 in fillOtherPucchFormatCfg()");
               return RFAILED;
            }
            fillPucchFormatCfg(macFormatCfg, schPucchCfg->format1);            
	 }
         break;
      }
      case PUCCH_FORMAT_2:
      {
         if(macFormatCfg)
	 {
            MAC_ALLOC(schPucchCfg->format2, sizeof(SchPucchFormatCfg));
            if(schPucchCfg->format2 == NULLP)
            {
               DU_LOG("\nERROR --> MAC : Failed to allocate Mem for Format 2 in fillOtherPucchFormatCfg()");
               return RFAILED;
            }
            fillPucchFormatCfg(macFormatCfg, schPucchCfg->format2);            
	 }
         break;
      }
      case PUCCH_FORMAT_3:
      {
         if(macFormatCfg)
         {
            MAC_ALLOC(schPucchCfg->format3, sizeof(SchPucchFormatCfg));
            if(schPucchCfg->format3 == NULLP)
            {
               DU_LOG("\nERROR  --> MAC : Failed to allocate Mem for Format 3 in fillOtherPucchFormatCfg()");
               return RFAILED;
            }
            fillPucchFormatCfg(macFormatCfg, schPucchCfg->format3);
         }
         break;
      }
      case PUCCH_FORMAT_4:
      {
         if(macFormatCfg)
         {
            MAC_ALLOC(schPucchCfg->format4, sizeof(SchPucchFormatCfg));
            if(schPucchCfg->format4 == NULLP)
            {
               DU_LOG("\nERROR  --> MAC : Failed to allocate Mem for Format 4 in fillOtherPucchFormatCfg()");
               return RFAILED;
            }
            fillPucchFormatCfg(macFormatCfg, schPucchCfg->format4);
        }
        break;
      }
      default:
         DU_LOG("\nERROR --> MAC : Invalid Format Cfg %d in fillInitialUlBwpPucchCfg()", pucchFormat);
         return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills PUCCH config in initial UL BWP config for SCH UE Cfg
 *
 * @details
 *
 *    Function : fillInitalUlBwpPucchCfg
 *
 *    Functionality:
 *       Fills PUCCH cofig in initial UL BWP config for SCH UE Cfg
 *
 * @params[in] macPucchCfg : Initial UL-BWP PUCCH cfg at MAC
 *             schPucchCfg : Initial UL-BWP PUCCH cfg to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillInitialUlBwpPucchCfg(PucchCfg *macPucchCfg, SchPucchCfg *schPucchCfg)
{
   if(macPucchCfg->resrcSet)
   {
      MAC_ALLOC(schPucchCfg->resrcSet, sizeof(SchPucchResrcSetCfg));
      if(schPucchCfg->resrcSet == NULLP)
      {
         DU_LOG("\nERROR  --> MAC : Failed to allocate Mem for Resrc set List in fillInitialUlBwpPucchCfg()");
         return RFAILED;
      }
      memset(schPucchCfg->resrcSet, 0, sizeof(SchPucchResrcSetCfg));
      fillResrcSetList(macPucchCfg->resrcSet, schPucchCfg->resrcSet);
   }

   if(macPucchCfg->resrc)
   {
      MAC_ALLOC(schPucchCfg->resrc, sizeof(SchPucchResrcCfg));
      if(schPucchCfg->resrc == NULLP)
      {
         DU_LOG("\nERROR  --> MAC : Failed to allocate Mem for Resrc List in fillInitialUlBwpPucchCfg()");
         return RFAILED;
      }
      memset(schPucchCfg->resrc, 0, sizeof(SchPucchResrcCfg));
      if((fillResrcList(macPucchCfg->resrc, schPucchCfg->resrc)) != ROK)
      {
         DU_LOG("\nERROR  --> MAC : Failed in fillResrcList() at fillInitialUlBwpPucchCfg()");
         return RFAILED;
      }
   }

   /* valid for format 1, 2, 3, 4 */
   fillOtherPucchFormatCfg(PUCCH_FORMAT_1, macPucchCfg->format1, schPucchCfg);
   fillOtherPucchFormatCfg(PUCCH_FORMAT_2, macPucchCfg->format2, schPucchCfg);
   fillOtherPucchFormatCfg(PUCCH_FORMAT_3, macPucchCfg->format3, schPucchCfg);
   fillOtherPucchFormatCfg(PUCCH_FORMAT_4, macPucchCfg->format4, schPucchCfg);

   if(macPucchCfg->schedReq)
   {
      MAC_ALLOC(schPucchCfg->schedReq, sizeof(SchPucchSchedReqCfg));
      if(schPucchCfg->schedReq == NULLP)
      {
         DU_LOG("\nERROR  --> MAC : Failed to allocate Mem for  PucchSchedReqCfg in fillInitialUlBwpPucchCfg()");
         return RFAILED;
      }
      memset(schPucchCfg->schedReq, 0, sizeof(SchPucchSchedReqCfg));
      fillPucchSchedReqCfg(macPucchCfg->schedReq, schPucchCfg->schedReq);
   }

   if(macPucchCfg->multiCsiCfg)
   {
      MAC_ALLOC(schPucchCfg->multiCsiCfg, sizeof(SchPucchMultiCsiCfg));
      if(schPucchCfg->multiCsiCfg == NULLP)
      {
         DU_LOG("\nERROR  --> MAC : Failed to allocate Mem for multiCsiCfg in fillInitialUlBwpPucchCfg()");
         return RFAILED;
      }
      memset(schPucchCfg->multiCsiCfg, 0, sizeof(SchPucchMultiCsiCfg));
      fillMultiCsiCfg(macPucchCfg->multiCsiCfg, schPucchCfg->multiCsiCfg);
   }

   //TODO: To add the support for spatial Config
   schPucchCfg->spatialInfo = NULLP;

   if(macPucchCfg->dlDataToUlAck)
   {
      MAC_ALLOC(schPucchCfg->dlDataToUlAck, sizeof(SchPucchDlDataToUlAck));
      if(schPucchCfg->dlDataToUlAck == NULLP)
      {
         DU_LOG("\nERROR  --> MAC : Failed to allocate Mem for dlDataToUlAck in fillInitialUlBwpPucchCfg()");
         return RFAILED;
      }
      memset(schPucchCfg->dlDataToUlAck, 0, sizeof(SchPucchDlDataToUlAck));
      fillDlToUlAck(macPucchCfg->dlDataToUlAck, schPucchCfg->dlDataToUlAck);

   }

   if(macPucchCfg->powerControl)
   {
      MAC_ALLOC(schPucchCfg->powerControl, sizeof(SchPucchPowerControl));
      if(schPucchCfg->powerControl == NULLP)
      {
         DU_LOG("\nERROR  --> MAC : Failed to allocate Mem for powerControl in fillInitialUlBwpPucchCfg()");
         return RFAILED;
      }
      memset(schPucchCfg->powerControl, 0, sizeof(SchPucchPowerControl));
      fillPucchPowerControl(macPucchCfg->powerControl, schPucchCfg->powerControl);
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief function to free Pucch Format
 *
 * @details
 *
 *    Function : freeSchPucchFormat
 *
 *    Functionality:
 *       function to free Pucch Format
 *
 * @params[in] pucchFormat, SchPucchResrcInfo Pointer, 
 *             SchPucchResrcInfo pointer
 * @return void
 * ****************************************************************/

void freeSchPucchFormat(uint8_t pucchFormat, SchPucchResrcInfo *resrcInfo, SchPucchFormatCfg *formatCfg)
{
   switch(pucchFormat)
   {
      case PUCCH_FORMAT_0 :
         if(resrcInfo->SchPucchFormat.format0)
         {
            MAC_FREE(resrcInfo->SchPucchFormat.format0, sizeof(SchPucchFormat0)); 
            resrcInfo->SchPucchFormat.format0 = NULLP;
         }
         break;

      case PUCCH_FORMAT_1 :
         if(resrcInfo->SchPucchFormat.format1)
         {
            MAC_FREE(resrcInfo->SchPucchFormat.format1, sizeof(SchPucchFormat1)); 
            resrcInfo->SchPucchFormat.format1 = NULLP;
         }
         if(formatCfg)
	 {
            memset(formatCfg, 0, sizeof(SchPucchFormatCfg));
	    MAC_FREE(formatCfg, sizeof(SchPucchFormatCfg));
	 }
         break;

      case PUCCH_FORMAT_2 :
         if(resrcInfo->SchPucchFormat.format2)
         {
            MAC_FREE(resrcInfo->SchPucchFormat.format2, sizeof(SchPucchFormat2_3)); 
            resrcInfo->SchPucchFormat.format2 = NULLP;
         }
         if(formatCfg)
	 {
            memset(formatCfg, 0, sizeof(SchPucchFormatCfg));
	    MAC_FREE(formatCfg, sizeof(SchPucchFormatCfg));
	 }
         break;

      case PUCCH_FORMAT_3 :
         if(resrcInfo->SchPucchFormat.format3)
         {
            MAC_FREE(resrcInfo->SchPucchFormat.format3, sizeof(SchPucchFormat2_3)); 
            resrcInfo->SchPucchFormat.format3 = NULLP;
         }
         if(formatCfg)
	 {
            memset(formatCfg, 0, sizeof(SchPucchFormatCfg));
	    MAC_FREE(formatCfg, sizeof(SchPucchFormatCfg));
	 }
         break;

      case PUCCH_FORMAT_4 :
         if(resrcInfo->SchPucchFormat.format4)
         {
            MAC_FREE(resrcInfo->SchPucchFormat.format4, sizeof(SchPucchFormat4)); 
            resrcInfo->SchPucchFormat.format4 = NULLP;
         }
         if(formatCfg)
	 {
            memset(formatCfg, 0, sizeof(SchPucchFormatCfg));
	    MAC_FREE(formatCfg, sizeof(SchPucchFormatCfg));
	 }
         break;

      default:
        break;
   }
}

/*******************************************************************
 *
 * @brief function to free Pucch Resrc Cfg
 *
 * @details
 *
 *    Function : freePucchResrcCfg
 *
 *    Functionality:
 *       function to free Pucch Resrc Cfg
 *
 * @params[in] SchPucchResrcCfg pointer, 
 * @return void
 * ****************************************************************/

void freePucchResrcCfg(SchPucchResrcCfg *schRsrcList)
{
   uint8_t resrcIdx;

   /* free Resrc To AddMod List */
   for(resrcIdx = 0; resrcIdx < schRsrcList->resrcToAddModListCount; resrcIdx++)
   {
      freeSchPucchFormat(schRsrcList->resrcToAddModList[resrcIdx].pucchFormat,
      &schRsrcList->resrcToAddModList[resrcIdx], NULLP);
      memset(&schRsrcList->resrcToAddModList[resrcIdx], 0, sizeof(SchPucchResrcInfo));
   }
   memset(schRsrcList, 0, sizeof(SchPucchResrcCfg));
   MAC_FREE(schRsrcList, sizeof(SchPucchResrcCfg));
}

/*******************************************************************
 *
 * @brief function to free Pucch Sched Req Cfg
 *
 * @details
 *
 *    Function : freePucchSchedReqCfg
 *
 *    Functionality:
 *       function to free Pucch Sched Req Cfg
 *
 * @params[in] SchPucchSchedReqCfg pointer, 
 * @return void
 * ****************************************************************/

void freePucchSchedReqCfg(SchPucchSchedReqCfg *schedReqCfg)
{
   uint8_t schedReqIdx;

   for(schedReqIdx = 0; schedReqIdx < schedReqCfg->schedAddModListCount; schedReqIdx++)
   {
      memset(&schedReqCfg->schedAddModList[schedReqIdx], 0, sizeof(SchSchedReqResrcInfo));
   }
   memset(schedReqCfg, 0, sizeof(SchPucchSchedReqCfg));
   MAC_FREE(schedReqCfg, sizeof(SchPucchSchedReqCfg));
}

/*******************************************************************
 *
 * @brief function to free Ul Bwp Pucch Cfg
 *
 * @details
 *
 *    Function : freeUlBwpPucchCfg
 *
 *    Functionality:
 *       function to free Ul Bwp Pucch Cfg
 *
 * @params[in] SchPucchCfg pointer, 
 * @return void
 * ****************************************************************/

void freeUlBwpPucchCfg(SchPucchCfg *schPucchCfg)
{
   if(schPucchCfg->resrcSet)
   {
      memset(schPucchCfg->resrcSet, 0, sizeof(SchPucchResrcSetCfg));
      MAC_FREE(schPucchCfg->resrcSet, sizeof(SchPucchResrcSetCfg));
   }
   if(schPucchCfg->resrc)
   {
      freePucchResrcCfg(schPucchCfg->resrc);
   }
   if(schPucchCfg->format1)
   {
      freeSchPucchFormat(PUCCH_FORMAT_1, NULLP, schPucchCfg->format1);
   }
   if(schPucchCfg->format2)
   {
      freeSchPucchFormat(PUCCH_FORMAT_2, NULLP, schPucchCfg->format2);
   }
   if(schPucchCfg->format3)
   {
      freeSchPucchFormat(PUCCH_FORMAT_3, NULLP, schPucchCfg->format3);
   }
   if(schPucchCfg->format4)
   {
      freeSchPucchFormat(PUCCH_FORMAT_4, NULLP, schPucchCfg->format4);
   }
   if(schPucchCfg->schedReq)
   {
      freePucchSchedReqCfg(schPucchCfg->schedReq);
   }
   if(schPucchCfg->spatialInfo)
   {
      memset(schPucchCfg->spatialInfo, 0, sizeof(SchPucchSpatialCfg));
      MAC_FREE(schPucchCfg->spatialInfo, sizeof(SchPucchSpatialCfg));
   }
   if(schPucchCfg->multiCsiCfg)
   {
      memset(schPucchCfg->multiCsiCfg, 0, sizeof(SchPucchMultiCsiCfg));
      MAC_FREE(schPucchCfg->multiCsiCfg, sizeof(SchPucchMultiCsiCfg));
   }
   if(schPucchCfg->dlDataToUlAck)
   {
      memset(schPucchCfg->dlDataToUlAck, 0, sizeof(SchPucchDlDataToUlAck));
      MAC_FREE(schPucchCfg->dlDataToUlAck, sizeof(SchPucchDlDataToUlAck));
   }
   if(schPucchCfg->powerControl)
   {
      memset(schPucchCfg->powerControl, 0, sizeof(SchPucchPowerControl));
      MAC_FREE(schPucchCfg->powerControl, sizeof(SchPucchPowerControl));
   }
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
      memset(&schInitUlBwp->pucchCfg, 0, sizeof(SchPucchCfg));
      if(fillInitialUlBwpPucchCfg(&macInitUlBwp.pucchCfg, &schInitUlBwp->pucchCfg) != ROK)
      {
         DU_LOG("\nERROR  --> MAC : Failed to fill Pucch Cfg in fillInitialUlBwpPucchCfg()");
	 freeUlBwpPucchCfg(&schInitUlBwp->pucchCfg);
         return RFAILED; 
      }
   }
   schInitUlBwp->puschCfgPres = macInitUlBwp.puschPresent;
   if(schInitUlBwp->puschCfgPres)
   {
      memset(&schInitUlBwp->puschCfg, 0, sizeof(SchPuschCfg));
      if(fillInitalUlBwpPuschCfg(macInitUlBwp.puschCfg, &schInitUlBwp->puschCfg) != ROK)
      {
	 DU_LOG("\nERROR  --> MAC : Failed to fill Pusch Cfg in fillInitalUlBwpPuschCfg()");
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
      DU_LOG("\nERROR  -->  MAC : Number of CORESET to ADD/MOD [%d] exceeds max limit [%d]",\
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
      DU_LOG("\nERROR  -->  MAC : Number of CORESET to release [%d] exceeds max limit [%d]",\
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
      DU_LOG("\nERROR  -->  MAC : Number of search space to ADD/MOD [%d] exceeds max [%d]", \
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
      DU_LOG("\nERROR  -->  MAC : Number of search space to release [%d] exceeds max [%d]", \
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
      DU_LOG("\nERROR  -->  MAC : Number of time domain resource allocation [%d], exceeds\
	    max limit [%d]", schPdschCfg->numTimeDomRsrcAlloc, MAX_NUM_DL_ALLOC);
      return RFAILED;
   }

   for(idx = 0; idx < schPdschCfg->numTimeDomRsrcAlloc; idx++)
   {
      if(macPdschCfg.timeDomRsrcAllociList[idx].k0)
      {
         MAC_ALLOC(schPdschCfg->timeDomRsrcAllociList[idx].k0, sizeof(uint8_t));
         if(!schPdschCfg->timeDomRsrcAllociList[idx].k0)
         {
            DU_LOG("\nERROR  -->  MAC : Memory allocation failed for K0 in fillInitDlBwpPdschCfg()");
            return RFAILED;
         }
         *(schPdschCfg->timeDomRsrcAllociList[idx].k0) = *(macPdschCfg.timeDomRsrcAllociList[idx].k0);
      }
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
	 DU_LOG("\nERROR  -->  MAC : fillInitDlBwpPdcchCfg() failed");
	 return RFAILED;
      }
   }

   schInitDlBwp->pdschCfgPres = macInitDlBwp.pdschPresent;
   if(schInitDlBwp->pdschCfgPres)
   {
      if(fillInitDlBwpPdschCfg(macInitDlBwp.pdschCfg,&schInitDlBwp->pdschCfg) != ROK)
      {
	 DU_LOG("\nERROR  -->  MAC : fillInitDlBwpPdschCfg() failed");
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
      DU_LOG("\nERROR  -->  MAC : fillInitialDlBwp() failed");
      return RFAILED;
   }

   servCellCfg->numDlBwpToAdd = macSpCellCfg.servCellCfg.numDlBwpToAdd;
   if(servCellCfg->numDlBwpToAdd > MAX_NUM_BWP)
   {
      DU_LOG("\nERROR  -->  MAC : Number of DL BWP to ADD/MOD [%d] exceeds max limit [%d]",\
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
      DU_LOG("\nERROR  -->  MAC : fillPdschServCellCfg() failed");
      return RFAILED;
   }

   /* Fill Initail UL BWP */
   if(fillInitialUlBwp(macSpCellCfg.servCellCfg.initUlBwp, \
	    &servCellCfg->initUlBwp) != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : fillInitialUlBwp() failed");
      return RFAILED;
   }

   servCellCfg->numUlBwpToAdd = macSpCellCfg.servCellCfg.numUlBwpToAdd;
   if(servCellCfg->numUlBwpToAdd > MAX_NUM_BWP)
   {
      DU_LOG("\nERROR  -->  MAC : Number of UL BWP to ADD/MOD [%d] exceeds max limit [%d]",\
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
	 DU_LOG("\nERROR  -->  Invalid Pst received %d", pst->event);
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
            DU_LOG("\nERROR  -->  MAC : Memory alloc failed at drbQos at fillLogicalChannelCfg()");
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
            MAC_ALLOC(schLcCfg->snssai, sizeof(Snssai));
            if(!schLcCfg->snssai)
            {
               DU_LOG("\nERROR  -->  MAC : Memory alloc failed at snssai at fillLogicalChannelCfg()");
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
	 DU_LOG("\nERROR  -->  MAC : fillLogicalChannelCfg() failed for lc Idx[%d]", lcIdx);
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
   if(ueCfg->macCellGrpCfgPres == true)
   {
      schUeCfg->macCellGrpCfgPres = true;
      memset(&schUeCfg->macCellGrpCfg, 0, sizeof(SchMacCellGrpCfg));
      if(fillMacCellGroupCfg(ueCfg->macCellGrpCfg, &schUeCfg->macCellGrpCfg) != ROK)
      {
         DU_LOG("\nERROR  -->  MAC : fillMacCellGroupCfg() failed");
         return RFAILED;
      }
   }
   if(ueCfg->phyCellGrpCfgPres == true)
   {
     schUeCfg->phyCellGrpCfgPres = true;
     /* Copy Physical cell group config */
      memset(&schUeCfg->phyCellGrpCfg, 0,sizeof(SchPhyCellGrpCfg));
      if(fillPhyCellGroupCfg(ueCfg->phyCellGrpCfg, &schUeCfg->phyCellGrpCfg) != ROK)
      {
         DU_LOG("\nERROR  -->  MAC : fillPhyCellGroupCfg() failed");
         return RFAILED;
      }
   }

   if(ueCfg->spCellCfgPres == true)
   {
      schUeCfg->spCellCfgPres = true;
      /* Copy sp cell config */
      memset(&schUeCfg->spCellCfg, 0, sizeof(SchSpCellCfg));
      if(fillSpCellCfg(ueCfg->spCellCfg, &schUeCfg->spCellCfg) != ROK)
      {
         DU_LOG("\nERROR  -->  MAC : fillSpCellCfg() failed");
         return RFAILED;
      }
   }
   if(ueCfg->ambrCfg != NULLP)
   {
      MAC_ALLOC(schUeCfg->ambrCfg, sizeof(SchAmbrCfg));
      if(!schUeCfg->ambrCfg)
      {
         DU_LOG("\nERROR  -->  MAC : Memory allocation failed in sendReconfigReqToSch");
         return RFAILED;
      }
      schUeCfg->ambrCfg->ulBr = ueCfg->ambrCfg->ulBr;
   }
   else
   {
      schUeCfg->ambrCfg = NULLP;
   }
   /* Fill DL modulation infor */
   schUeCfg->dlModInfo.modOrder = ueCfg->dlModInfo.modOrder;
   schUeCfg->dlModInfo.mcsIndex = ueCfg->dlModInfo.mcsIndex;
   schUeCfg->dlModInfo.mcsTable = ueCfg->dlModInfo.mcsTable;

   /* Fill UL modulation infor */
   schUeCfg->ulModInfo.modOrder = ueCfg->ulModInfo.modOrder;
   schUeCfg->ulModInfo.mcsIndex = ueCfg->ulModInfo.mcsIndex;
   schUeCfg->ulModInfo.mcsTable = ueCfg->ulModInfo.mcsTable;

   /* Fill sch Lc Cfg  to Add/ Mod/ Del */
   ret  = fillSchLcCfgList(schUeCfg, ueCfg);
   if(ret == RFAILED)
   {
      DU_LOG("\nERROR  -->  MAC : Failed to copy LCs at fillSchUeCfg()");
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
   /*Commenting as S-NSSAI and PDU session will be stored in MAC DB in future scope*/
#if 0
   /*Checking the Memory Leakage of Last Index*/
   if(ulCb->lcCb[ulCb->numUlLc].snssai != NULLP)
   {
      DU_LOG("ERROR  --> MAC: updateMacUlCb Last index deleted :%d  memory is leaking",\
            ulCb->numUlLc);
      MAC_FREE(ulCb->lcCb[ulCb->numUlLc].snssai, sizeof(Snssai));
   }
   else
   {
      DU_LOG("INFO  --> MAC: updateMacUlCb Last index:%d (before deletion) memory is freed successfully",\
            ulCb->numUlLc);
   }
#endif
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
   /*Commenting as S-NSSAI and PDU session will be stored in MAC DB in future scope*/
#if 0
   /*Checking the Memory Leakage of Last Index*/
   if(dlCb->lcCb[dlCb->numDlLc].snssai != NULLP)
   {
      DU_LOG("ERROR  --> MAC: updateMacDlCb Last Deleted index:%d memory is leaking",\
            dlCb->numDlLc);
      MAC_FREE(dlCb->lcCb[dlCb->numDlLc].snssai, sizeof(Snssai));
   }
   else
   {
      DU_LOG("INFO  --> MAC: updateMacDlCb Last index:%d (before deletion) memory is freed successfully",\
            dlCb->numDlLc);
   }
#endif
}

/*******************************************************************
 *
 * @brief Update Mac UL Lc List based on CONFIG_MOD/CONFIG_DEL
 *
 * @details
 *
 *    Function : updateMacUlLcCtxt
 *
 *    Functionality: Update UeUlCb Lc List
 *
 * @params[in]  UeUlCb pointer, ueLcCfg(received from DUAPP)
 * @return void
 *
 * ****************************************************************/
void updateMacUlLcCtxt(UeUlCb *ulInfo, LcCfg *ueLcCfg)
{
   uint8_t ueLcIdx = 0; 

   /*Traversing UL LC to be updated/Deleted*/
   for(ueLcIdx = 0; ueLcIdx < ulInfo->numUlLc; ueLcIdx++)
   {
      if(ulInfo->lcCb[ueLcIdx].lcId == ueLcCfg->lcId)
      {
         if(ueLcCfg->configType == CONFIG_MOD)
         {
            /*Modify UL LC CB */
            ulInfo->lcCb[ueLcIdx].lcGrpId = ueLcCfg->ulLcCfg.lcGroup;

            /*Commenting as S-NSSAI and PDU session will be stored in MAC DB in future scope*/
#if 0
            /*Modifying/Copying PduSession ID and S-NSSAI into MAC's UECB*/
            if(ueLcCfg->drbQos)
            {
               ulInfo->lcCb[ueLcIdx].pduSessionId = ueLcCfg->drbQos->pduSessionId;
            }
            if(ueLcCfg->snssai)
            {
               if(ulInfo->lcCb[ueLcIdx].snssai == NULLP)
               {
                  MAC_ALLOC(ulInfo->lcCb[ueLcIdx].snssai, sizeof(Snssai));
               }

               memcpy(ulInfo->lcCb[ueLcIdx].snssai, ueLcCfg->snssai, sizeof(Snssai));
            }
#endif
            DU_LOG("\nINFO  -->  MAC: Successfully Modified LC context for lcId[%d], ueLcIdx:%d",\
                  ueLcCfg->lcId,ueLcIdx);
            break;
         }
         if(ueLcCfg->configType == CONFIG_DEL)
         {
            memset(&ulInfo->lcCb[ueLcIdx], 0, sizeof(UlLcCb));
            (ulInfo->numUlLc)--;
            updateMacUlCb(ueLcIdx, ulInfo);
            DU_LOG("\nINFO  -->  MAC: Successfully Deleted LC context for lcId[%d]", ueLcCfg->lcId);
            break;
         }
      }
   }
}

/*******************************************************************
 *
 * @brief Update Mac DL Lc List based on CONFIG_MOD/CONFIG_DEL
 *
 * @details
 *
 *    Function : updateMacDlLcCtxt
 *
 *    Functionality: Update UeDlCb Lc List
 *
 * @params[in]  UeDlCb pointer, ueLcCfg(received from DUAPP)
 * @return void
 *
 * ****************************************************************/
void updateMacDlLcCtxt(UeDlCb *dlInfo, LcCfg *ueLcCfg)
{
   uint8_t ueLcIdx = 0; 

   /*Traversing DL LC to be updated/Deleted*/
   for(ueLcIdx = 0; ueLcIdx < dlInfo->numDlLc; ueLcIdx++)
   {
      if(dlInfo->lcCb[ueLcIdx].lcId == ueLcCfg->lcId)
      {
         if(ueLcCfg->configType == CONFIG_MOD)
         {
            /*Commenting as S-NSSAI and PDU session will be stored in MAC DB in future scope*/
#if 0
            /*Modifying/Copying PduSession ID and S-NSSAI into MAC's UECB*/
            if(ueLcCfg->drbQos)
            {
               dlInfo->lcCb[ueLcIdx].pduSessionId = ueLcCfg->drbQos->pduSessionId;
            }
            if(ueLcCfg->snssai)
            {
               if(dlInfo->lcCb[ueLcIdx].snssai == NULLP)
               {
                  MAC_ALLOC(dlInfo->lcCb[ueLcIdx].snssai, sizeof(Snssai));
               }

               memcpy(dlInfo->lcCb[ueLcIdx].snssai, ueLcCfg->snssai, sizeof(Snssai));
            }
#endif
            DU_LOG("\nINFO  -->  MAC: Successfully Modified LC context for lcId[%d], ueLcIdx:%d",\
                  ueLcCfg->lcId,ueLcIdx);
            break;
         }
         if(ueLcCfg->configType == CONFIG_DEL)
         {
            memset(&dlInfo->lcCb[ueLcIdx], 0, sizeof(DlLcCb));
            (dlInfo->numDlLc)--;
            updateMacDlCb(ueLcIdx, dlInfo);
            DU_LOG("\nINFO  -->  MAC: Successfully Deleted LC context for lcId[%d]", ueLcCfg->lcId);
            break;
         }
      }
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
   uint8_t lcIdx = 0;

   for(lcIdx = 0; lcIdx < ueCfg->numLcs; lcIdx++)
   {
      if(ueCb->dlInfo.numDlLc < MAX_NUM_LC)
      {
         if(ueCfg->lcCfgList[lcIdx].configType == CONFIG_ADD)
         {
            /*Commenting as S-NSSAI and PDU session will be stored in MAC DB in future scope*/
#if 0
            /*Copying PduSession ID and S-NSSAI into MAC's UECB*/
            if(ueCfg->lcCfgList[lcIdx].drbQos)
            {
               ueCb->dlInfo.lcCb[ueCb->dlInfo.numDlLc].pduSessionId = \
                                                                      ueCfg->lcCfgList[lcIdx].drbQos->pduSessionId;

               ueCb->ulInfo.lcCb[ueCb->ulInfo.numUlLc].pduSessionId = \
                                                                      ueCfg->lcCfgList[lcIdx].drbQos->pduSessionId;
            }
            if(ueCfg->lcCfgList[lcIdx].snssai)
            {
               if(ueCb->dlInfo.lcCb[ueCb->dlInfo.numDlLc].snssai == NULLP)
               {
                  MAC_ALLOC(ueCb->dlInfo.lcCb[ueCb->dlInfo.numDlLc].snssai, sizeof(Snssai));
               }
               if(ueCb->ulInfo.lcCb[ueCb->ulInfo.numUlLc].snssai == NULLP)
               {
                  MAC_ALLOC(ueCb->ulInfo.lcCb[ueCb->ulInfo.numUlLc].snssai, sizeof(Snssai));
               }

               memcpy(ueCb->dlInfo.lcCb[ueCb->dlInfo.numDlLc].snssai, \
                     ueCfg->lcCfgList[lcIdx].snssai, sizeof(Snssai));

               memcpy(ueCb->ulInfo.lcCb[ueCb->ulInfo.numUlLc].snssai, \
                     ueCfg->lcCfgList[lcIdx].snssai, sizeof(Snssai));

            }
#endif
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
            updateMacUlLcCtxt(&ueCb->ulInfo, &ueCfg->lcCfgList[lcIdx]);            
            updateMacDlLcCtxt(&ueCb->dlInfo, &ueCfg->lcCfgList[lcIdx]);            
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

   ueCb->ueId = ueCfg->ueId;
   ueCb->crnti = ueCfg->crnti;
   ueCb->cellCb = macCb.macCell[cellIdx];
   if(ueCfg->spCellCfgPres)
   {
      ueCb->dlInfo.dlHarqEnt.numHarqProcs = \
      ueCfg->spCellCfg.servCellCfg.pdschServCellCfg.numHarqProcForPdsch; 
   }
   ueCb->state = UE_STATE_ACTIVE;
   /*TODO: To check the bsr value during implementation */
   if(ueCfg->macCellGrpCfgPres)
   {
      ueCb->bsrTmrCfg.periodicTimer = ueCfg->macCellGrpCfg.bsrTmrCfg.periodicTimer;
      ueCb->bsrTmrCfg.retxTimer     = ueCfg->macCellGrpCfg.bsrTmrCfg.retxTimer;
      ueCb->bsrTmrCfg.srDelayTimer  = ueCfg->macCellGrpCfg.bsrTmrCfg.srDelayTimer;
   }
   ret = fillMacLcCfgList(ueCb, ueCfg);
   if(ret == RFAILED)
   {
      DU_LOG("\nERROR  -->  MAC: Failed while filing MAC LC List at fillMacUeCb()");
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

   if((ueCb->ueId == ueCfg->ueId) && (ueCb->crnti == ueCfg->crnti)\
      &&(ueCb->state == UE_STATE_ACTIVE))
   {
      DU_LOG("\nERROR  -->  MAC : CRNTI %d already configured ", ueCfg->crnti);
      return ROKDUP;
   }
   else
   {
      memset(ueCb, 0, sizeof(MacUeCb));
      ret = fillMacUeCb(ueCb, ueCfg, cellIdx);
      if(ret != ROK)
      {
         DU_LOG("\nERROR  -->  MAC : Failed to create Ue Cb at createUeCb()");
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

   if((ueCb->ueId == ueCfg->ueId) && (ueCb->crnti == ueCfg->crnti)\
      &&(ueCb->state == UE_STATE_ACTIVE))
   {
      DU_LOG("\nINFO  -->  MAC : Modifying Ue config Req for CRNTI %d ", ueCfg->crnti);
      ret = fillMacUeCb(ueCb, ueCfg, cellIdx);
      if(ret != ROK)
      {
         DU_LOG("\nERROR  -->  MAC : Failed to modify MacUeCb at modifyUeCb()");
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
      DU_LOG("\nERROR  -->  MAC : Cell Id %d not configured", ueCfg->cellId);
      return RFAILED;
   }

   /* Check if max number of UE configured */
   if(macCb.macCell[cellIdx]->numActvUe > MAX_NUM_UE)
   {
      DU_LOG("\nERROR  -->  MAC : Max number of UE [%d] already configured", MAX_NUM_UE);
      return RFAILED;
   }

   /* Check if UE already configured */
   ueCb = &macCb.macCell[cellIdx]->ueCb[ueCfg->ueId -1];
   switch(pst->event)
   {
      case EVENT_UE_CONFIG_RSP_TO_MAC:
	 ret = createUeCb(cellIdx, ueCb, ueCfg);
	 if(ret != ROK)
            DU_LOG("\nERROR  -->  MAC: AddUeConfigReq for cellIdx :%d failed in procMacUeCfgData()", cellIdx);
	 break;
      case EVENT_UE_RECONFIG_RSP_TO_MAC:
	 ret = modifyUeCb(cellIdx, ueCb, ueCfg);
	 if(ret != ROK)
            DU_LOG("\nERROR  -->  MAC: ModifyUeConfigReq for cellIdx :%d failed at procMacUeCfgData()", cellIdx);
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
      DU_LOG("\nERROR  -->  MAC: Memory Alloc Failed at copyToTmpData()");
      return RFAILED;
   }
   memcpy(tmpData, ueCfg, sizeof(MacUeCfg));
   GET_CELL_IDX(ueCfg->cellId, cellIdx);
   macCb.macCell[cellIdx]->ueCfgTmpData[ueCfg->ueId-1] = tmpData;
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

   DU_LOG("\nINFO  -->  MAC : UE Create Request for CRNTI[%d]", ueCfg->crnti);

   if(ueCfg)
   {
      /*Storing received ueCfg in ueCfgTmpData */
      ret = copyToTmpData(ueCfg);
      if(ret == ROK)
      {
         /*Sending Cfg Req to SCH */
	 ret = fillSchUeCfg(pst, &schUeCfg, ueCfg);
	 if(ret != ROK)
	    DU_LOG("\nERROR  -->  MAC : Failed to fill Sch Ue Cfg at MacProcUeCreateReq()");
	 else
	 {
            /* Fill event and send UE create request to SCH */
            ret = sendUeReqToSch(pst, &schUeCfg);
	    if(ret != ROK)
	       DU_LOG("\nERROR  -->  MAC : Failed to send UE Create request to SCH");
	 }
      }
      else 
      {
	 DU_LOG("\nERROR  -->  MAC : Failed to store MAC UE CFG ");
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC : MAC UE Create request processing failed");
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
      DU_LOG("\nERROR  -->  MAC: Memory allocation for UE config response failed");
      return RFAILED;
   }

   /* Filling UE Config response */
   memset(cfgRsp, 0, sizeof(MacUeCfgRsp));
   cfgRsp->cellId = schCfgRsp->cellId;
   cfgRsp->ueId = schCfgRsp->ueId;
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
      DU_LOG("\nERROR  -->  MAC: Memory allocation for UE Reconfig response failed");
      return RFAILED;
   }

   /* Filling UE Config response */
   memset(cfgRsp, 0, sizeof(MacUeCfgRsp));
   cfgRsp->cellId = schCfgRsp->cellId;
   cfgRsp->ueId = schCfgRsp->ueId;
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
 * @params[in] cellIdx, ueId
 *
 * @return MacUeCfg pointer - success
 *         NULLP - failure
 *
 * ****************************************************************/

MacUeCfg *getMacUeCfg(uint16_t cellIdx, uint8_t ueId)
{
   MacUeCfg *ueCfg = NULLP;
   if(macCb.macCell[cellIdx])
   {
      ueCfg = macCb.macCell[cellIdx]->ueCfgTmpData[ueId-1];
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Failed to get macCellCb in getMacUeCfg()");
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

#ifdef CALL_FLOW_DEBUG_LOG
   switch(pst->event)
   {
      case EVENT_UE_CONFIG_RSP_TO_MAC:
         DU_LOG("\nCall Flow: ENTSCH -> ENTMAC : EVENT_UE_CONFIG_RSP_TO_MAC\n");
         break;
      case EVENT_UE_RECONFIG_RSP_TO_MAC:
         DU_LOG("\nCall Flow: ENTSCH -> ENTMAC : EVENT_UE_RECONFIG_RSP_TO_MAC\n");
         break;
      default:
         DU_LOG("\nCall Flow: ENTSCH -> ENTMAC : Invalid Event\n");
         break;
   }
#endif
   
   GET_CELL_IDX(schCfgRsp->cellId, cellIdx);
   ueCfg = getMacUeCfg(cellIdx, schCfgRsp->ueId);
   if(ueCfg == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : Failed to find the Mac Ue Cfg for event [%d] in MacProcSchUeCfgRsp()", pst->event);
      ret = RFAILED;
   }

   switch(pst->event)
   {
      case EVENT_UE_CONFIG_RSP_TO_MAC:
         {
            if(schCfgRsp->rsp != RSP_NOK)
            {
               DU_LOG("\nINFO  -->  MAC: SCH UeConfigRsp for CRNTI[%d] is success in MacProcSchUeCfgRsp()", schCfgRsp->crnti);
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
               DU_LOG("\nERROR  -->  MAC: SCH UeConfigRsp for CRNTI[%d] is failed in MacProcSchUeCfgRsp()", schCfgRsp->crnti);
            }
            ret = MacSendUeCreateRsp(result, schCfgRsp);
         }
         break;

      case EVENT_UE_RECONFIG_RSP_TO_MAC:
         {

            if(schCfgRsp->rsp != RSP_NOK)
            {
               DU_LOG("\nINFO  -->  MAC: SCH UeReconfigRsp for CRNTI[%d] is success in MacProcSchUeCfgRsp()",\
                     schCfgRsp->crnti);
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
               DU_LOG("\nERROR  -->  MAC: SCH UeReconfigRsp for CRNTI[%d] is failed in MacProcSchUeCfgRsp()",\
               schCfgRsp->crnti);
            }
            ret = MacSendUeReconfigRsp(result, schCfgRsp);
         }
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

   DU_LOG("\nINFO  -->  MAC : UE Reconfig Request for CRNTI[%d]", ueCfg->crnti);

   if(ueCfg)
   {
      /*Storing received ueCfg in ueCfgTmpData */
      ret = copyToTmpData(ueCfg);
      if(ret == ROK)
      {
         /*Sending Cfg Req to SCH */
         ret = fillSchUeCfg(pst, &schUeCfg, ueCfg);
         if(ret != ROK)
            DU_LOG("\nERROR  -->  MAC : Failed to fill sch Ue Cfg at MacProcUeReconfigReq()");
         else
         {
            /* Fill event and send UE create request to SCH */
            ret = sendUeReqToSch(pst, &schUeCfg);
            if(ret != ROK)
               DU_LOG("\nERROR  -->  MAC : Failed to send UE Reconfig Request to SCH");
         }
      }
      else 
      {
         DU_LOG("\nERROR  -->  MAC : Failed to store MAC UE Cb ");
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC : MAC UE Create request processing failed");
      ret = RFAILED;
   }
   /* FREE shared memory */
   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, ueCfg, sizeof(MacUeCfg));
   return ROK;
}

/*******************************************************************
*
* @brief Fill and Send UE Delete response from MAC to DU APP
*
* @details
*
*    Function : MacSendUeDeleteRsp 
*
*    Functionality: Fill and Send UE Delete response from MAC to DUAPP
*
* @params[in] MAC UE delete result
*             SCH UE delete response
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t MacSendUeDeleteRsp(uint16_t cellId, uint16_t crnti, UeDeleteStatus result)
{
   MacUeDeleteRsp *deleteRsp;
   Pst            rspPst;

   MAC_ALLOC_SHRABL_BUF(deleteRsp, sizeof(MacUeDeleteRsp));
   if(!deleteRsp)
   {
      DU_LOG("\nERROR  -->  MAC : Memory allocation for UE delete response failed");
      return RFAILED;
   }

   /* Filling UE delete response */
   deleteRsp->cellId = cellId;
   GET_UE_ID(crnti, deleteRsp->ueId);
   deleteRsp->result = result;

   /* Fill Post structure and send UE delete response*/
   memset(&rspPst, 0, sizeof(Pst));
   FILL_PST_MAC_TO_DUAPP(rspPst, EVENT_MAC_UE_DELETE_RSP);
   return (*macDuUeDeleteRspOpts[rspPst.selector])(&rspPst, deleteRsp);
}

/*******************************************************************
*
* @brief freeing the Pucch Resrc Cfg 
*
* @details
*
*    Function : deletePucchResourcesCfg 
*
*    Functionality: freeing the Pucch Resrc Cfg
*
* @params[in] PucchResrcCfg *resrcCfg
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

void deletePucchResourcesCfg(PucchResrcCfg *resrcCfg)
{
   uint8_t resrcIdx;
   for(resrcIdx =0; resrcIdx< resrcCfg->resrcToAddModListCount;resrcIdx++)
   {
      switch(resrcCfg->resrcToAddModList[resrcIdx].pucchFormat)
      {
         case PUCCH_FORMAT_0:
         {
            MAC_FREE(resrcCfg->resrcToAddModList[resrcIdx].PucchFormat.format0, sizeof(PucchFormat0));
            break;
         }
         case PUCCH_FORMAT_1:
         {
            MAC_FREE(resrcCfg->resrcToAddModList[resrcIdx].PucchFormat.format1, sizeof(PucchFormat1));
            break;
         }
         case PUCCH_FORMAT_2:
         {
            MAC_FREE(resrcCfg->resrcToAddModList[resrcIdx].PucchFormat.format2, sizeof(PucchFormat2_3));
            break;
         }
         case PUCCH_FORMAT_3:
         {
            MAC_FREE(resrcCfg->resrcToAddModList[resrcIdx].PucchFormat.format3, sizeof(PucchFormat2_3));
            break;
         }
         case PUCCH_FORMAT_4:
         {
            MAC_FREE(resrcCfg->resrcToAddModList[resrcIdx].PucchFormat.format4, sizeof(PucchFormat4));
            break;
         }
      }
   }

}

/*******************************************************************
*
* @brief  Processes UE delete response from scheduler
*
* @details
*
*    Function : MacProcSchUeCfgRsp
*
*    Functionality:
*      Processes UE create delete from scheduler
*
* @params[in] Pst : Post structure
*             schUeDelRsp : Scheduler UE delete respons
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t MacProcSchUeDeleteRsp(Pst *pst, SchUeDeleteRsp *schUeDelRsp)
{
   uint8_t ueId =0, isCrntiValid = 0;
   uint16_t cellIdx=0;
   uint8_t ret = RFAILED;
   UeDeleteStatus result;

#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTSCH -> ENTMAC : EVENT_UE_DELETE_RSP_TO_MAC\n");
#endif   

   if(schUeDelRsp)
   {
      if(schUeDelRsp->rsp == RSP_OK)
      {
         DU_LOG("\nINFO   -->  MAC : SCH UE Delete response for CRNTI[%d] is successful", schUeDelRsp->crnti);
         GET_CELL_IDX(schUeDelRsp->cellId, cellIdx);
         if(macCb.macCell[cellIdx])
         {
            CHECK_CRNTI(schUeDelRsp->crnti, isCrntiValid);
            if(!isCrntiValid)
            {
               /*C-RNTI value is out of Acceptable range*/
               DU_LOG("\nERROR  -->  MAC : MacProcSchUeDeleteRsp(): Invalid crnti[%d] ",schUeDelRsp->crnti);
               result = UEID_INVALID;
            }
            else
            {
               GET_UE_ID(schUeDelRsp->crnti, ueId);
               if(macCb.macCell[cellIdx]->ueCb[ueId -1].crnti == schUeDelRsp->crnti)
               {

                  /*Commenting as S-NSSAI and PDU session will be stored in MAC DB in future scope*/
#if 0
                  /*Looping around LCs to free S-NSSAI memory*/
                  for(lcIdx = 0; lcIdx < (macCb.macCell[cellIdx]->ueCb[ueId -1].ulInfo.numUlLc); lcIdx++)
                  {
                     MAC_FREE(macCb.macCell[cellIdx]->ueCb[ueId -1].ulInfo.lcCb[lcIdx].snssai, sizeof(Snssai));
                  }
                  for(lcIdx = 0; lcIdx < (macCb.macCell[cellIdx]->ueCb[ueId -1].dlInfo.numDlLc); lcIdx++)
                  {
                     MAC_FREE(macCb.macCell[cellIdx]->ueCb[ueId -1].dlInfo.lcCb[lcIdx].snssai, sizeof(Snssai));
                  }
#endif
                  memset(&macCb.macCell[cellIdx]->ueCb[ueId -1], 0, sizeof(MacUeCb));
                  macCb.macCell[cellIdx]->numActvUe--;
                  result = SUCCESS;
                  ret = ROK;
               }
               else
               {
                  DU_LOG("\nERROR  -->  MAC : MacProcSchUeDeleteRsp(): crnti[%d] does not exist ",schUeDelRsp->crnti);
                  result = UEID_INVALID;
               }
            }
         }
         else
         {
            DU_LOG("\nERROR  -->  MAC : MacProcSchUeDeleteRsp(): cellId[%d] does not exist ",schUeDelRsp->cellId);
            result = CELLID_INVALID;
         }
      }
      else
      {
         result = (schUeDelRsp->cause == INVALID_CELLID) ? CELLID_INVALID : UEID_INVALID;
      }
      if(MacSendUeDeleteRsp(schUeDelRsp->cellId, schUeDelRsp->crnti, result) != ROK)
      {
         DU_LOG("\nERROR  -->  MAC: MacProcSchUeDeleteRsp(): Failed to send UE delete response");
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: MacProcSchUeDeleteRsp(): Failed to receive UE delete response by SCH");
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
*
* @brief Sends UE delete req to Scheduler
*
* @details
*
*    Function : sendUeDelReqToSch
*
*    Functionality: sends UE delete req to Scheduler
*
* @params[in] Pst *pst,  MacUeDelete *ueDelete
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t sendUeDelReqToSch(Pst *pst, MacUeDelete *ueDelete)
{
   if(ueDelete != NULLP)
   {
      Pst schPst;
      SchUeDelete schUeDel;

      memset(&schUeDel, 0, sizeof(SchUeDelete));
      schUeDel.cellId = ueDelete->cellId;
      schUeDel.crnti  = ueDelete->crnti;
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, ueDelete, sizeof(MacUeDelete));
      FILL_PST_MAC_TO_SCH(schPst, EVENT_UE_DELETE_REQ_TO_SCH);
      return(*macSchUeDeleteReqOpts[schPst.selector])(&schPst, &schUeDel);
   }
   else
   {
      DU_LOG("\nERROR  --> SCH: sendUeDelReqToSch():MAC Ue Delete is NULL");
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Handles UE Delete requst from DU APP
 *
 * @details
 *
 *    Function : MacProcUeDeleteReq
 *
 *    Functionality: Handles UE Delete requst from DU APP
 *
 * @params[in] Pst *pst, MacUeDelete *ueDelete
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t MacProcUeDeleteReq(Pst *pst, MacUeDelete *ueDelete)
{
   uint8_t ret = ROK;
   uint8_t cellIdx=0;
   UeDeleteStatus result=SUCCESS;
   MacUeCb  *ueCb = NULLP;
   MacCellCb *cellCb = NULLP;

   DU_LOG("\nINFO   -->  MAC : UE Delete Request received for ueId[%d]", ueDelete->ueId);

   if(ueDelete)
   {
      GET_CELL_IDX(ueDelete->cellId, cellIdx);     
      cellCb = macCb.macCell[cellIdx];
      if(cellCb)
      {
         ueCb = &cellCb->ueCb[ueDelete->ueId-1];
         if(ueCb->crnti == ueDelete->crnti)
         {
            ret = sendUeDelReqToSch(pst, ueDelete);
            if(ret != ROK)
            {
               DU_LOG("\nERROR  -->  MAC : MacProcUeDeleteReq(): Failed to send UE Delete Request to SCH");
               ret = RFAILED;
            }
         }
         else
         {
            DU_LOG("\nERROR  -->  MAC : MacProcUeDeleteReq(): CRNTI is not matched");
            result = UEID_INVALID;
         }
      }
      else
      {
         DU_LOG("\nERROR  -->  MAC : MacProcUeDeleteReq(): Failed to find the MacUeCb of UeId = %d",ueDelete->ueId);
         result = CELLID_INVALID;
      }

      if(result != SUCCESS)
      {
         MacSendUeDeleteRsp(ueDelete->cellId, ueDelete->crnti, result);
         MAC_FREE_SHRABL_BUF(pst->region, pst->pool, ueDelete, sizeof(MacUeDelete));
         ret = RFAILED;
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC : MacProcUeDeleteReq(): MAC UE delete request processing failed");
      ret = RFAILED;
   }
   return ret;
}

/**********************************************************************
  End of file
 **********************************************************************/
