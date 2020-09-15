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
/* This file contains UE management handling functionality for SCH */

#include "common_def.h"
#include "tfu.h"
#include "lrg.h"

#include "tfu.x"
#include "lrg.x"
#include "du_log.h"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "sch.h"
#include "sch_utils.h"

/* local defines */
SchUeCfgRspFunc SchUeCfgRspOpts[] =
{
   packSchUeCfgRsp,      /* LC */
   MacProcSchUeCfgRsp,   /* TC */
   packSchUeCfgRsp       /* LWLC */
};


/*******************************************************************
 *
 * @brief Fill and send UE cfg response to MAC
 *
 * @details
 *
 *    Function : SchSendUeCfgRspToMac
 *
 *    Functionality: Fill and send UE cfg response to MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void SchSendUeCfgRspToMac(SchUeCfg *ueCfg, Inst inst,\
      SchMacRsp result, SchUeCfgRsp *cfgRsp)
{
   Pst rspPst;

   DU_LOG("\nSCH: Sending UE Create response to MAC");

   cfgRsp->cellId = ueCfg->cellId;
   cfgRsp->crnti = ueCfg->crnti;
   GET_UE_IDX(ueCfg->crnti, cfgRsp->ueIdx);
   cfgRsp->rsp = result;   

   /* Filling response post */
   memset(&rspPst, 0, sizeof(Pst));
   FILL_PST_SCH_TO_MAC(rspPst, inst);
   rspPst.event = EVENT_UE_CREATE_RSP_TO_MAC;

   SchUeCfgRspOpts[rspPst.selector](&rspPst, cfgRsp);
}

/*******************************************************************
 *
 * @brief Hanles Ue create request from MAC
 *
 * @details
 *
 *    Function : MacSchUeCreateReq
 *
 *    Functionality: Hanles Ue create request from MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacSchUeCreateReq(Pst *pst, SchUeCfg *ueCfg)
{
   uint8_t      idx;
   uint16_t     ueIdx;
   SchCellCb    *cellCb;
   SchUeCb      *ueCb;
   SchUeCfgRsp  cfgRsp;
   Inst         inst = pst->dstInst - 1;

   DU_LOG("\nSCH : UE Create Request for CRNTI[%d]", ueCfg->crnti);

   memset(&cfgRsp, 0, sizeof(SchUeCfgRsp));

   if(!ueCfg)
   {
      DU_LOG("\nSCH : UE create request failed");
      return RFAILED;
   }

   /* Search of cell cb */
   for(idx = 0; idx < MAX_NUM_CELL; idx++)
   {
      cellCb = schCb[inst].cells[idx];
      if(cellCb->cellId == ueCfg->cellId)
	 break;
   }
   if(idx == MAX_NUM_CELL)
   {
      DU_LOG("\nSCH : Ue create request failed. Invalid cell id %d", ueCfg->cellId);
      SchSendUeCfgRspToMac(ueCfg, inst, RSP_NOK, &cfgRsp);
      return ROK;
   }

   /* Check if max number of UE configured */
   if(cellCb->numActvUe > MAX_NUM_UE)
   {
      DU_LOG("SCH : Max number of UE [%d] already configured", MAX_NUM_UE);
      SchSendUeCfgRspToMac(ueCfg, inst, RSP_NOK, &cfgRsp);
      return ROK;
   }

   /* Search if UE already configured */
   GET_UE_IDX(ueCfg->crnti, ueIdx);
   ueCb = &cellCb->ueCb[ueIdx -1];
   if(ueCb)
   {
      if((ueCb->crnti == ueCfg->crnti) && (ueCb->state == SCH_UE_STATE_ACTIVE))
      {
	 DU_LOG("\n SCH : CRNTI %d already configured ", ueCfg->crnti);
	 SchSendUeCfgRspToMac(ueCfg, inst, RSP_OK, &cfgRsp);
	 return ROK;
      }
   }

   /* Fill received Ue Configuration in UeCb */
   memset(ueCb, 0, sizeof(SchUeCb));

   GET_UE_IDX(ueCfg->crnti, ueCb->ueIdx);
   ueCb->crnti = ueCfg->crnti;
   memcpy(&ueCb->ueCfg, ueCfg, sizeof(SchUeCfg));
   ueCb->state = SCH_UE_STATE_ACTIVE;
   cellCb->numActvUe++;

   ueCb->cellCb = cellCb;
   ueCb->srRcvd = false;
   for(idx=0; idx<MAX_NUM_LOGICAL_CHANNEL_GROUPS; idx++)
      ueCb->bsrInfo[idx].dataVol = 0;

   SchSendUeCfgRspToMac(ueCfg, inst, RSP_OK, &cfgRsp);
   return ROK;
}

/*******************************************************************
*
* @brief Fills PUSCH UL allocation
*
* @details
*
*    Function : schFillPuschAlloc
*
*    Functionality: fills PUSCH info
*
* @params[in]
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t schFillPuschAlloc(SchUeCb *ueCb, uint16_t pdcchSlot, uint32_t dataVol, SchPuschInfo *puschInfo)
{
  uint16_t puschSlot      = 0;
  uint16_t startRb        = 0;
  uint8_t  numRb          = 0;
  uint8_t  mcs            = 4;
  uint8_t  numPdschSymbols= 14;
  uint16_t tbSize         = 0;
  uint8_t  buffer         = 5;
  uint8_t  idx            = 0;
  SchCellCb *cellCb       = ueCb->cellCb;
  SchUlSlotInfo *schUlSlotInfo = NULLP;
  uint8_t  k2 = ueCb->ueCfg.spCellCfg.servCellCfg.initUlBwp.puschCfg.timeDomRsrcAllocList[0].k2;
  uint8_t  startSymb = ueCb->ueCfg.spCellCfg.servCellCfg.initUlBwp.puschCfg.timeDomRsrcAllocList[0].startSymbol;
  uint8_t  symbLen = ueCb->ueCfg.spCellCfg.servCellCfg.initUlBwp.puschCfg.timeDomRsrcAllocList[0].symbolLength;

  puschSlot = (pdcchSlot + k2) % SCH_NUM_SLOTS;

  startRb = cellCb->schUlSlotInfo[puschSlot]->puschCurrentPrb;
  tbSize  = schCalcTbSize(dataVol + buffer); /*  2 bytes header + some buffer */
  numRb   = schCalcNumPrb(tbSize, mcs, numPdschSymbols);
  /* increment PUSCH PRB */

  cellCb->schUlSlotInfo[puschSlot]->puschCurrentPrb += numRb;

  puschInfo->harqProcId        = SCH_HARQ_PROC_ID;
  puschInfo->resAllocType      = SCH_ALLOC_TYPE_1;
  puschInfo->fdAlloc.startPrb  = startRb;
  puschInfo->fdAlloc.numPrb    = numRb;
  puschInfo->tdAlloc.startSymb = startSymb;
  puschInfo->tdAlloc.numSymb   = symbLen;
  puschInfo->tbInfo.mcs        = mcs;
  puschInfo->tbInfo.ndi        = 1; /* new transmission */
  puschInfo->tbInfo.rv         = 0;
  puschInfo->tbInfo.tbSize     = tbSize;
  puschInfo->dmrsMappingType   = DMRS_MAP_TYPE_A;  /* Setting Type-A */
  puschInfo->nrOfDmrsSymbols   = NUM_DMRS_SYMBOLS;
  puschInfo->dmrsAddPos        = DMRS_ADDITIONAL_POS;

  /* Update pusch in cell */
  for(idx=startSymb; idx<symbLen; idx++)
  {
     cellCb->schUlSlotInfo[puschSlot]->assignedPrb[idx] = startRb + numRb;
  }

  schUlSlotInfo = cellCb->schUlSlotInfo[puschSlot];

  SCH_ALLOC(schUlSlotInfo->schPuschInfo, sizeof(SchPuschInfo));
  if(!schUlSlotInfo->schPuschInfo)
  {
     DU_LOG("SCH: Memory allocation failed in schAllocMsg3Pusch");
     return RFAILED;
  }
  memcpy(schUlSlotInfo->schPuschInfo, puschInfo, sizeof(SchPuschInfo));

  return ROK;
}

/*******************************************************************
 *
 * @brief Fills DCI for UL grant
 *
 * @details
 *
 *    Function : schFillUlDci
 *
 *    Functionality: fills DCI for UL grant in response to BSR
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t schFillUlDci(SchUeCb *ueCb, SchPuschInfo puschInfo, DciInfo *dciInfo)
{
   SchCellCb         *cellCb  = ueCb->cellCb;
   SchControlRsrcSet coreset1 = ueCb->ueCfg.spCellCfg.servCellCfg.initDlBwp.pdcchCfg.cRSetToAddModList[0];

   dciInfo->cellId = cellCb->cellId;
   dciInfo->crnti  = ueCb->crnti;

   /* fill bwp cfg */
   dciInfo->bwpCfg.subcarrierSpacing  = cellCb->cellCfg.sib1SchCfg.bwp.subcarrierSpacing;
   dciInfo->bwpCfg.cyclicPrefix       = cellCb->cellCfg.sib1SchCfg.bwp.cyclicPrefix;
   dciInfo->bwpCfg.freqAlloc.startPrb = 0;
   dciInfo->bwpCfg.freqAlloc.numPrb   = MAX_NUM_RB; /* whole of BW */

   /*fill coreset cfg */
   //Considering number of RBs in coreset1 is same as coreset0
   dciInfo->coresetCfg.coreSetSize      = coresetIdxTable[0][1];
   //Considering coreset1 also starts from same symbol as coreset0
   dciInfo->coresetCfg.startSymbolIndex = searchSpaceIdxTable[0][3];
   dciInfo->coresetCfg.durationSymbols  = coreset1.duration;
   memcpy(dciInfo->coresetCfg.freqDomainResource, coreset1.freqDomainRsrc, FREQ_DOM_RSRC_SIZE);
   
   dciInfo->coresetCfg.cceRegMappingType   = coreset1.cceRegMappingType; /* non-interleaved */
   dciInfo->coresetCfg.regBundleSize       = 6; /* must be 6 for non-interleaved */
   dciInfo->coresetCfg.interleaverSize     = 0; /* NA for non-interleaved */
   dciInfo->coresetCfg.coreSetType         = 1; /* non PBCH coreset */
   dciInfo->coresetCfg.shiftIndex          = cellCb->cellCfg.phyCellId;
   dciInfo->coresetCfg.precoderGranularity = coreset1.precoderGranularity;
   dciInfo->coresetCfg.cceIndex            = 0; /* 0-3 for UL and 4-7 for DL */
   dciInfo->coresetCfg.aggregationLevel    = 4; /* same as for sib1 */
   
   dciInfo->formatType = FORMAT0_0;
   
   /* fill UL grant */
   dciInfo->format.format0_0.resourceAllocType   = puschInfo.resAllocType;
   dciInfo->format.format0_0.freqAlloc.startPrb  = puschInfo.fdAlloc.startPrb;
   dciInfo->format.format0_0.freqAlloc.numPrb    = puschInfo.fdAlloc.numPrb;
   dciInfo->format.format0_0.timeAlloc.startSymb = puschInfo.tdAlloc.startSymb;
   dciInfo->format.format0_0.timeAlloc.numSymb   = puschInfo.tdAlloc.numSymb;
   dciInfo->format.format0_0.rowIndex            = 0; /* row Index */
   dciInfo->format.format0_0.mcs                 = puschInfo.tbInfo.mcs;
   dciInfo->format.format0_0.harqProcId          = puschInfo.harqProcId;
   dciInfo->format.format0_0.puschHopFlag        = FALSE; /* disabled */
   dciInfo->format.format0_0.freqHopFlag         = FALSE; /* disabled */
   dciInfo->format.format0_0.ndi                 = puschInfo.tbInfo.ndi; /* new transmission */
   dciInfo->format.format0_0.rv                  = puschInfo.tbInfo.rv;
   dciInfo->format.format0_0.tpcCmd              = 0; //Sphoorthi TODO: check
   dciInfo->format.format0_0.sUlCfgd             = FALSE; /* SUL not configured */
   
   /* Fill DCI Structure */
   dciInfo->dciInfo.rnti                              = ueCb->crnti;
   dciInfo->dciInfo.scramblingId                      = cellCb->cellCfg.phyCellId;
   dciInfo->dciInfo.scramblingRnti                    = 0;
   dciInfo->dciInfo.cceIndex                          = 0; /* 0-3 for UL and 4-7 for DL */
   dciInfo->dciInfo.aggregLevel                       = 4;
   dciInfo->dciInfo.beamPdcchInfo.numPrgs             = 1;
   dciInfo->dciInfo.beamPdcchInfo.prgSize             = 1;
   dciInfo->dciInfo.beamPdcchInfo.digBfInterfaces     = 0;
   dciInfo->dciInfo.beamPdcchInfo.prg[0].pmIdx        = 0;
   dciInfo->dciInfo.beamPdcchInfo.prg[0].beamIdx[0]   = 0;
   dciInfo->dciInfo.txPdcchPower.powerValue           = 0;
   dciInfo->dciInfo.txPdcchPower.powerControlOffsetSS = 0;
   dciInfo->dciInfo.pdschCfg                          = NULLP; /* No DL data being sent */

   return ROK;
}


/**********************************************************************
  End of file
 **********************************************************************/
