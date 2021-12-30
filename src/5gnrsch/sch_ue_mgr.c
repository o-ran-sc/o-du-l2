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

SchUeDeleteRspFunc SchUeDeleteRspOpts[] =
{
   packSchUeDeleteRsp,      /* LC */
   MacProcSchUeDeleteRsp,   /* TC */
   packSchUeDeleteRsp       /* LWLC */
};

SchCellDeleteRspFunc SchCellDeleteRspOpts[]=
{
   packSchCellDeleteRsp,      /* LC */
   MacProcSchCellDeleteRsp,   /* TC */
   packSchCellDeleteRsp       /* LWLC */
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
void SchSendUeCfgRspToMac(uint16_t event, SchUeCfg *ueCfg, Inst inst,\
      SchMacRsp result, SchUeCfgRsp *cfgRsp)
{
   Pst rspPst;

   cfgRsp->cellId = ueCfg->cellId;
   cfgRsp->crnti = ueCfg->crnti;
   GET_UE_ID(ueCfg->crnti, cfgRsp->ueId);
   cfgRsp->rsp = result;   

   /* Filling response post */
   memset(&rspPst, 0, sizeof(Pst));
   FILL_PST_SCH_TO_MAC(rspPst, inst);
   if(event == EVENT_ADD_UE_CONFIG_REQ_TO_SCH)
   {
      rspPst.event = EVENT_UE_CONFIG_RSP_TO_MAC;
      DU_LOG("\nINFO  -->  SCH :  Sending UE Config response to MAC");
   }
   else if(event == EVENT_MODIFY_UE_CONFIG_REQ_TO_SCH)
   {
      rspPst.event = EVENT_UE_RECONFIG_RSP_TO_MAC;
      DU_LOG("\nINFO  -->  SCH :  Sending UE Reconfig response to MAC");
   }
   SchUeCfgRspOpts[rspPst.selector](&rspPst, cfgRsp);
}

/*******************************************************************
 
 *
 * @brief Function to fill Dl Lc Context in SCH Ue Cb
 *
 * @details
 *
 *    Function : fillSchDlLcCtxt
 *
 *    Functionality: Function to fill Dl Lc Context in SCH Ue Cb
 *
 * @params[in] SchDlLcCtxt pointer,
 *             SchLcCfg pointer
 * @return void
 *
 * ****************************************************************/

void fillSchDlLcCtxt(SchDlLcCtxt *ueCbLcCfg, SchLcCfg *lcCfg)
{
   ueCbLcCfg->lcId = lcCfg->lcId;
   ueCbLcCfg->lcp = lcCfg->dlLcCfg.lcp;
   ueCbLcCfg->lcState = SCH_LC_STATE_ACTIVE;
   ueCbLcCfg->bo = 0;
   if(lcCfg->drbQos)
   {
     ueCbLcCfg->pduSessionId = lcCfg->drbQos->pduSessionId;
   }
   if(lcCfg->snssai)
   {
     if(ueCbLcCfg->snssai == NULLP)/*In CONFIG_MOD case, no need to allocate SNSSAI memory*/
     {
        SCH_ALLOC(ueCbLcCfg->snssai, sizeof(Snssai));
     }
     memcpy(ueCbLcCfg->snssai, lcCfg->snssai,sizeof(Snssai));
   }
}

/*******************************************************************
 *
 * @brief Function to fill Ul Lc Context in SCH Ue Cb
 *
 * @details
 *
 *    Function : fillSchUlLcCtxt
 *
 *    Functionality: Function to fill Ul Lc Context in SCH Ue Cb
 *
 * @params[in] SchUlLcCtxt pointer,
 *             SchLcCfg pointer
 * @return void
 *
 * ****************************************************************/

void fillSchUlLcCtxt(SchUlLcCtxt *ueCbLcCfg, SchLcCfg *lcCfg)
{
   ueCbLcCfg->lcId = lcCfg->lcId;
   ueCbLcCfg->lcState = SCH_LC_STATE_ACTIVE;
   ueCbLcCfg->priority = lcCfg->ulLcCfg.priority;
   ueCbLcCfg->lcGroup = lcCfg->ulLcCfg.lcGroup;
   ueCbLcCfg->schReqId = lcCfg->ulLcCfg.schReqId;
   ueCbLcCfg->pbr     = lcCfg->ulLcCfg.pbr;
   ueCbLcCfg->bsd     = lcCfg->ulLcCfg.bsd;

   if(lcCfg->drbQos)
   {
      ueCbLcCfg->pduSessionId = lcCfg->drbQos->pduSessionId;
   }
   if(lcCfg->snssai)
   {
     /*In CONFIG_MOD case, no need to allocate SNSSAI memory again*/
     if(ueCbLcCfg->snssai == NULLP)
     {
        SCH_ALLOC(ueCbLcCfg->snssai, sizeof(Snssai));
     }
     memcpy(ueCbLcCfg->snssai, lcCfg->snssai,sizeof(Snssai));
   }
}

/*******************************************************************
 *
 * @brief Function to update/allocate dedLC Info
 *
 * @details
 *
 *    Function : updateDedLcInfo
 *
 *    Functionality: Function to fill DLDedLcInfo
 *
 * @params[arg] snssai pointer,
 *              SchRrmPolicy pointer,
 *              SchLcPrbEstimate pointer , It will be filled
 *              isDedicated pointer,(Address of isDedicated flag in LC Context)
 *
 * @return ROK      >> This LC is part of RRM MemberList.
 *         RFAILED  >> FATAL Error
 *         ROKIGNORE >> This LC is not part of this RRM MemberList 
 *
 * ****************************************************************/

uint8_t updateDedLcInfo(Snssai *snssai, SchLcPrbEstimate *lcPrbEst,\
                         bool *isDedicated)
{
   uint8_t sliceCfgIdx =0;
   SchSliceCfg sliceCfg = schCb[0].sliceCfg;

   if(sliceCfg.numOfSliceConfigured)
   {
      for(sliceCfgIdx = 0; sliceCfgIdx<sliceCfg.numOfSliceConfigured; sliceCfgIdx++)
      {
         if(memcmp(snssai, &(sliceCfg.listOfConfirguration[sliceCfgIdx]->snssai), sizeof(Snssai)) == 0)
         {
            if(lcPrbEst->dedLcInfo == NULLP)
            {
               SCH_ALLOC(lcPrbEst->dedLcInfo, sizeof(DedicatedLCInfo));
               if(lcPrbEst->dedLcInfo == NULLP)
               {
                  DU_LOG("\nINFO  -->  SCH : Memory Allocation Failed");
                  return RFAILED;
               }
            }
            if(sliceCfg.listOfConfirguration[sliceCfgIdx]->rrmPolicyRatioInfo)
            {
               /*Updating latest RrmPolicy*/
               lcPrbEst->dedLcInfo->rsvdDedicatedPRB = \
               (uint16_t)(((sliceCfg.listOfConfirguration[sliceCfgIdx]->rrmPolicyRatioInfo->policyDedicatedRatio)*(MAX_NUM_RB))/100);
               *isDedicated = TRUE;
               DU_LOG("\nINFO  -->  SCH : Updated RRM policy, reservedPOOL:%d",lcPrbEst->dedLcInfo->rsvdDedicatedPRB);
            }
         }
      }
      /*case: This LcCtxt  is either a Default LC or this LC is part of someother RRM_MemberList*/
      if(*isDedicated != TRUE) 
      {
         DU_LOG("\nINFO  -->  SCH : This SNSSAI is not a part of this RRMPolicy");
      }
   }
   return ROK;	 
}

/*******************************************************************
 *
 * @brief Function to fill SchUeCb
 *
 * @details
 *
 *    Function : fillSchUeCb
 *
 *    Functionality: Function to fill SchUeCb
 *
 * @params[in] SchUeCb pointer,
 *             SchUeCfg pointer
 * @return ROK/RFAILED
 *
 * ****************************************************************/

uint8_t fillSchUeCb(SchUeCb *ueCb, SchUeCfg *ueCfg)
{
   uint8_t   lcIdx, ueLcIdx;
   uint8_t   freqDomainResource[FREQ_DOM_RSRC_SIZE] = {0};
   SchPdschCfgCmn pdschCfg;
   SchPucchDlDataToUlAck *dlDataToUlAck;
   CmLListCp *lcLL = NULLP;
   uint8_t retDL = ROK, retUL = ROK;
   bool isLcIdValid = FALSE;

   ueCb->ueCfg.cellId = ueCfg->cellId;
   ueCb->ueCfg.crnti = ueCfg->crnti;
   if(ueCfg->macCellGrpCfgPres == true)
   {
      memcpy(&ueCb->ueCfg.macCellGrpCfg , &ueCfg->macCellGrpCfg, sizeof(SchMacCellGrpCfg)); 
      ueCb->ueCfg.macCellGrpCfgPres = true;
   }

   if(ueCfg->phyCellGrpCfgPres == true)
   {
      memcpy(&ueCb->ueCfg.phyCellGrpCfg ,  &ueCfg->phyCellGrpCfg, sizeof(SchPhyCellGrpCfg));
      ueCb->ueCfg.phyCellGrpCfgPres = true;
   }

   if(ueCfg->spCellCfgPres == true)
   {
      memcpy(&ueCb->ueCfg.spCellCfg , &ueCfg->spCellCfg, sizeof(SchSpCellCfg));

      covertFreqDomRsrcMapToIAPIFormat(ueCb->ueCfg.spCellCfg.servCellCfg.initDlBwp.pdcchCfg.cRSetToAddModList[0].freqDomainRsrc,\
            freqDomainResource);
      memset(ueCb->ueCfg.spCellCfg.servCellCfg.initDlBwp.pdcchCfg.cRSetToAddModList[0].freqDomainRsrc, 0, FREQ_DOM_RSRC_SIZE);
      memcpy(ueCb->ueCfg.spCellCfg.servCellCfg.initDlBwp.pdcchCfg.cRSetToAddModList[0].freqDomainRsrc, freqDomainResource, FREQ_DOM_RSRC_SIZE);

      ueCb->ueCfg.spCellCfgPres = true;
      dlDataToUlAck = ueCfg->spCellCfg.servCellCfg.initUlBwp.pucchCfg.dlDataToUlAck;
      if(ueCb->cellCb)
      {
         if(dlDataToUlAck)
         {
            BuildK0K1Table(ueCb->cellCb, &ueCb->ueCfg.spCellCfg.servCellCfg.initDlBwp.k0K1InfoTbl, false, pdschCfg,\
                  ueCfg->spCellCfg.servCellCfg.initDlBwp.pdschCfg, dlDataToUlAck->dlDataToUlAckListCount,\
                  dlDataToUlAck->dlDataToUlAckList);
            ueCb->ueCfg.spCellCfg.servCellCfg.initDlBwp.k0K1TblPrsnt = true;
            BuildK2InfoTable(ueCb->cellCb, ueCfg->spCellCfg.servCellCfg.initUlBwp.puschCfg.timeDomRsrcAllocList,\
                  ueCfg->spCellCfg.servCellCfg.initUlBwp.puschCfg.numTimeDomRsrcAlloc,\
                  NULLP, &ueCb->ueCfg.spCellCfg.servCellCfg.initUlBwp.k2InfoTbl);
                  ueCb->ueCfg.spCellCfg.servCellCfg.initUlBwp.k2TblPrsnt = true;
         }
      }
   }

   ueCb->state = SCH_UE_STATE_ACTIVE;
   if(ueCfg->ambrCfg)
   {
      SCH_FREE(ueCb->ueCfg.ambrCfg, sizeof(SchAmbrCfg));
      ueCb->ueCfg.ambrCfg =  ueCfg->ambrCfg;
   }
   memcpy(&ueCb->ueCfg.dlModInfo,  &ueCfg->dlModInfo , sizeof(SchModulationInfo));
   memcpy(&ueCb->ueCfg.ulModInfo,  &ueCfg->ulModInfo , sizeof(SchModulationInfo));

   //Updating SchUlCb and SchDlCb DB in SchUeCb
   for(lcIdx = 0; lcIdx < ueCfg->numLcs; lcIdx++)
   {
      isLcIdValid = FALSE; /*Re-Initializing*/

      ueLcIdx = ueCfg->schLcCfg[lcIdx].lcId;
      CHECK_LCID(ueLcIdx, isLcIdValid);
      if(isLcIdValid == FALSE)
      {
         DU_LOG("ERROR --> SCH: LCID:%d is not Valid",ueLcIdx);
         continue;
      }
      if(ueCfg->schLcCfg[lcIdx].configType == CONFIG_ADD)
      {
         fillSchUlLcCtxt(&ueCb->ulInfo.ulLcCtxt[ueLcIdx], &ueCfg->schLcCfg[lcIdx]);
         fillSchDlLcCtxt(&ueCb->dlInfo.dlLcCtxt[ueLcIdx], &ueCfg->schLcCfg[lcIdx]);

         /*Checking whether this LC belong to Dedicated S-NSSAI 
          * and Create the Dedicated LC List & Update the Reserve PRB number*/
         if(ueCb->dlInfo.dlLcCtxt[ueLcIdx].snssai != NULLP)
         {
            retDL = updateDedLcInfo(ueCb->dlInfo.dlLcCtxt[ueLcIdx].snssai, &(ueCb->dlLcPrbEst),\
                  &(ueCb->dlInfo.dlLcCtxt[ueLcIdx].isDedicated));
         }
         if(ueCb->ulInfo.ulLcCtxt[ueLcIdx].snssai != NULLP)
         {
            retUL =  updateDedLcInfo(ueCb->ulInfo.ulLcCtxt[ueLcIdx].snssai, &(ueCb->ulLcPrbEst),\
                  &(ueCb->ulInfo.ulLcCtxt[ueLcIdx].isDedicated));
         }

         if(retUL == RFAILED  || retDL == RFAILED)/*FATAL error*/
         {
            DU_LOG("\nERROR  -->  SCH : Failure in updateDedLcInfo");
            return RFAILED;
         }
      }
      else
      {
         if(ueCb->ulInfo.ulLcCtxt[ueLcIdx].lcId == ueCfg->schLcCfg[lcIdx].lcId)
         {
            if(ueCfg->schLcCfg[lcIdx].configType == CONFIG_MOD)
            {
               fillSchUlLcCtxt(&ueCb->ulInfo.ulLcCtxt[ueLcIdx], &ueCfg->schLcCfg[lcIdx]);
               /*Updating the RRM reserved pool PRB count*/
               if(ueCb->ulInfo.ulLcCtxt[ueLcIdx].snssai != NULLP)
               {
                  retUL =  updateDedLcInfo(ueCb->ulInfo.ulLcCtxt[ueLcIdx].snssai, &(ueCb->ulLcPrbEst),\
                        &(ueCb->ulInfo.ulLcCtxt[ueLcIdx].isDedicated));
               }
               if(retUL == RFAILED)
               {
                  DU_LOG("\nERROR  -->  SCH : Failed in updating Ded Lc info");
                  return RFAILED;
               }
            }
            if(ueCfg->schLcCfg[lcIdx].configType == CONFIG_DEL)
            {
               /*Delete the LC node from the UL LC List*/
               if(ueCb->ulInfo.ulLcCtxt[ueLcIdx].isDedicated)
               {
                  if(ueCb->ulLcPrbEst.dedLcInfo != NULLP)
                  {
                     lcLL = &(ueCb->ulLcPrbEst.dedLcInfo->dedLcList);
                     handleLcLList(lcLL, ueCfg->schLcCfg[lcIdx].lcId, DELETE);
                     if(lcLL->count == 0)/*IF No Node in DedicateLCList to be deleted*/
                     {
                        /*Free the Dedicated LC Info structure*/
                        SCH_FREE(ueCb->ulLcPrbEst.dedLcInfo, sizeof(DedicatedLCInfo));
                     }
                  }
               }
               else/*Default LC list*/
               {
                  lcLL = &(ueCb->ulLcPrbEst.defLcList);
                  handleLcLList(lcLL, ueCfg->schLcCfg[lcIdx].lcId, DELETE);
               }
               SCH_FREE(ueCb->ulInfo.ulLcCtxt[ueLcIdx].snssai, sizeof(Snssai));
               memset(&ueCb->ulInfo.ulLcCtxt[ueLcIdx], 0, sizeof(SchUlLcCtxt));
            }
         }/*End of UL LC Ctxt*/

         if(ueCb->dlInfo.dlLcCtxt[ueLcIdx].lcId == ueCfg->schLcCfg[lcIdx].lcId)
         {
            if(ueCfg->schLcCfg[lcIdx].configType == CONFIG_MOD)
            {
               fillSchDlLcCtxt(&ueCb->dlInfo.dlLcCtxt[ueLcIdx], &ueCfg->schLcCfg[lcIdx]);
               /*Updating the RRM policy*/
               if(ueCb->dlInfo.dlLcCtxt[ueLcIdx].snssai != NULLP)
               {
                  retDL = updateDedLcInfo(ueCb->dlInfo.dlLcCtxt[ueLcIdx].snssai, &(ueCb->dlLcPrbEst), \
                        &(ueCb->dlInfo.dlLcCtxt[ueLcIdx].isDedicated));
               }
               if(retDL == RFAILED)
               {
                  DU_LOG("\nERROR  -->  SCH : Failed in updating Ded Lc info");
                  return RFAILED;
               }
            }
            if(ueCfg->schLcCfg[lcIdx].configType == CONFIG_DEL)
            {
               /*Delete the LC node from the DL LC List*/
               if(ueCb->dlInfo.dlLcCtxt[ueLcIdx].isDedicated)
               {
                  if(ueCb->dlLcPrbEst.dedLcInfo != NULLP)
                  {
                     lcLL = &(ueCb->dlLcPrbEst.dedLcInfo->dedLcList);
                     handleLcLList(lcLL, ueCfg->schLcCfg[lcIdx].lcId, DELETE);
                     if(lcLL->count == 0)/*Last Node in DedicateLCList to be deleted*/
                     {
                        /*Free the Dedicated LC Info structure*/
                        SCH_FREE(ueCb->dlLcPrbEst.dedLcInfo, sizeof(DedicatedLCInfo));
                     }
                  }
               }
               else
               {
                  lcLL = &(ueCb->dlLcPrbEst.defLcList);
                  handleLcLList(lcLL, ueCfg->schLcCfg[lcIdx].lcId, DELETE);
               }
               SCH_FREE(ueCb->dlInfo.dlLcCtxt[ueLcIdx].snssai, sizeof(Snssai));
               memset(&ueCb->dlInfo.dlLcCtxt[ueLcIdx], 0, sizeof(SchDlLcCtxt));
            }
         }/*End of DL LC ctxt*/
      }

      SCH_FREE(ueCfg->schLcCfg[lcIdx].drbQos, sizeof(SchDrbQosInfo));
      SCH_FREE(ueCfg->schLcCfg[lcIdx].snssai, sizeof(Snssai));

   }/* End of outer for loop */
   return ROK;
}

/*******************************************************************
 *
 * @brief Function to get SCH Cell Cb
 *
 * @details
 *
 *    Function : getSchCellCb
 *
 *    Functionality: Function to get SCH Cell Cb
 *
 * @params[in] event, SchUeCfg pointer 
 * @return schUeCb pointer  - success
 *         NULLP - failure
 *
 * ****************************************************************/

SchCellCb *getSchCellCb(uint16_t srcEvent, Inst inst, SchUeCfg *ueCfg)
{
   uint8_t      idx;
   SchCellCb    *cellCb = NULLP;
   SchUeCfgRsp  cfgRsp;
   memset(&cfgRsp, 0, sizeof(SchUeCfgRsp));

   /* Search of cell cb */
   for(idx = 0; idx < MAX_NUM_CELL; idx++)
   {
      cellCb = schCb[inst].cells[idx];
      if(cellCb->cellId == ueCfg->cellId)
	 break;
   }
   if(idx == MAX_NUM_CELL)
   {
      DU_LOG("\nERROR  -->  SCH : Ue create request failed. Invalid cell id %d", ueCfg->cellId);
      SchSendUeCfgRspToMac(srcEvent, ueCfg, inst, RSP_NOK, &cfgRsp);
      return NULLP;
   }

   /* Check if max number of UE configured */
   if(cellCb->numActvUe > MAX_NUM_UE)
   {
      DU_LOG("\nERROR  -->  SCH :  Max number of UE [%d] already configured", MAX_NUM_UE);
      SchSendUeCfgRspToMac(srcEvent, ueCfg, inst, RSP_NOK, &cfgRsp);
      return NULLP;
   }
   return cellCb;
}


/*******************************************************************
 *
 * @brief Function to Add Ue Config Request from MAC
 *
 * @details
 *
 *    Function : MacSchAddUeConfigReq
 *
 *    Functionality: Function to Add Ue config request from MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacSchAddUeConfigReq(Pst *pst, SchUeCfg *ueCfg)
{
   uint8_t ueId, lcIdx, ret = ROK;
   SchCellCb    *cellCb = NULLP;
   SchUeCb      *ueCb = NULLP;
   SchUeCfgRsp  cfgRsp;
   Inst         inst = pst->dstInst - 1;
   memset(&cfgRsp, 0, sizeof(SchUeCfgRsp));
  
#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTMAC -> ENTSCH : EVENT_ADD_UE_CONFIG_REQ_TO_SCH\n");
#endif

   if(!ueCfg)
   {
      DU_LOG("\nERROR  -->  SCH :  Adding UE Config Request failed at MacSchAddUeConfigReq()");
      return RFAILED;
   }
   DU_LOG("\nDEBUG  -->  SCH :  Adding UE Config Request for CRNTI[%d]", ueCfg->crnti);
   cellCb = getSchCellCb(pst->event, inst, ueCfg);

   /* Search if UE already configured */
   GET_UE_ID(ueCfg->crnti, ueId);
   ueCb = &cellCb->ueCb[ueId -1];
   if(ueCb)
   {
      if((ueCb->crnti == ueCfg->crnti) && (ueCb->state == SCH_UE_STATE_ACTIVE))
      {
         DU_LOG("\nDEBUG  -->  SCH : CRNTI %d already configured ", ueCfg->crnti);
         SchSendUeCfgRspToMac(pst->event, ueCfg, inst, RSP_OK, &cfgRsp);
         return ROK;
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  SCH : SchUeCb not found at MacSchAddUeConfigReq() ");
      SchSendUeCfgRspToMac(pst->event, ueCfg, inst, RSP_NOK, &cfgRsp);
      return RFAILED;
   }

   /* Fill received Ue Configuration in UeCb */
   memset(ueCb, 0, sizeof(SchUeCb));
   ueCb->ueId = ueId;
   ueCb->crnti = ueCfg->crnti;
   ueCb->state = SCH_UE_STATE_ACTIVE;
   ret = fillSchUeCb(ueCb, ueCfg);
   if(ret == ROK)
   {
      cellCb->numActvUe++;
      SET_ONE_BIT(ueCb->ueId, cellCb->actvUeBitMap);
      ueCb->cellCb = cellCb;
      ueCb->srRcvd = false;
      ueCb->bsrRcvd = false;
      for(lcIdx=0; lcIdx<MAX_NUM_LOGICAL_CHANNEL_GROUPS; lcIdx++)
         ueCb->bsrInfo[lcIdx].dataVol = 0;

      SchSendUeCfgRspToMac(pst->event, ueCfg, inst, RSP_OK, &cfgRsp);
   }
   return ret;
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
uint8_t schFillPuschAlloc(SchUeCb *ueCb, SlotTimingInfo puschTime, uint32_t tbSize, 
                            uint8_t startSymb, uint8_t symbLen, uint16_t startPrb)
{
  uint8_t  numRb          = 0;
  SchCellCb *cellCb       = NULLP;
  SchUlSlotInfo *schUlSlotInfo = NULLP;
  SchPuschInfo puschInfo;
  
  if(ueCb == NULLP)
  {
    DU_LOG("\nERROR --> SCH: UE CB is empty");
    return RFAILED;
  }

  cellCb = ueCb->cellCb;
  if(cellCb == NULLP)
  {
    DU_LOG("\nERROR --> SCH: CELL CB is empty");
    return RFAILED;
  }

  tbSize  +=  UL_TX_BUFFER_SIZE; /*  2 bytes header + some buffer */
  numRb   = schCalcNumPrb(tbSize, ueCb->ueCfg.ulModInfo.mcsIndex, symbLen);
  allocatePrbUl(cellCb, puschTime, startSymb, symbLen, &startPrb, numRb);

  puschInfo.crnti             = ueCb->crnti; 
  puschInfo.harqProcId        = SCH_HARQ_PROC_ID;
  puschInfo.resAllocType      = SCH_ALLOC_TYPE_1;
  puschInfo.fdAlloc.startPrb  = startPrb;
  puschInfo.fdAlloc.numPrb    = numRb;
  puschInfo.tdAlloc.startSymb = startSymb;
  puschInfo.tdAlloc.numSymb   = symbLen;
  puschInfo.tbInfo.qamOrder   = ueCb->ueCfg.ulModInfo.modOrder;
  puschInfo.tbInfo.mcs        = ueCb->ueCfg.ulModInfo.mcsIndex;
  puschInfo.tbInfo.mcsTable   = ueCb->ueCfg.ulModInfo.mcsTable;
  puschInfo.tbInfo.ndi        = 1; /* new transmission */
  puschInfo.tbInfo.rv         = 0;
  puschInfo.tbInfo.tbSize     = tbSize;
  puschInfo.dmrsMappingType   = DMRS_MAP_TYPE_A;  /* Setting Type-A */
  puschInfo.nrOfDmrsSymbols   = NUM_DMRS_SYMBOLS;
  puschInfo.dmrsAddPos        = DMRS_ADDITIONAL_POS;

  schUlSlotInfo = cellCb->schUlSlotInfo[puschTime.slot];
  SCH_ALLOC(schUlSlotInfo->schPuschInfo, sizeof(SchPuschInfo));
  if(!schUlSlotInfo->schPuschInfo)
  {
     DU_LOG("\nERROR  -->  SCH: Memory allocation failed in schAllocMsg3Pusch");
     return RFAILED;
  }
  memcpy(schUlSlotInfo->schPuschInfo, &puschInfo, sizeof(SchPuschInfo));

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
uint8_t schFillUlDci(SchUeCb *ueCb, SchPuschInfo *puschInfo, DciInfo *dciInfo)
{
   SchCellCb         *cellCb  = ueCb->cellCb;
   SchControlRsrcSet coreset1 ;
  
   memset(&coreset1, 0, sizeof(SchControlRsrcSet));
   if(ueCb->ueCfg.spCellCfgPres == true)
   {
     coreset1 = ueCb->ueCfg.spCellCfg.servCellCfg.initDlBwp.pdcchCfg.cRSetToAddModList[0];
   }
   
   dciInfo->cellId = cellCb->cellId;
   dciInfo->crnti  = ueCb->crnti;

   /* fill bwp cfg */
   dciInfo->bwpCfg.subcarrierSpacing  = cellCb->cellCfg.sib1SchCfg.bwp.subcarrierSpacing;
   dciInfo->bwpCfg.cyclicPrefix       = cellCb->cellCfg.sib1SchCfg.bwp.cyclicPrefix;
   dciInfo->bwpCfg.freqAlloc.startPrb = cellCb->cellCfg.schInitialDlBwp.bwp.freqAlloc.startPrb;
   dciInfo->bwpCfg.freqAlloc.numPrb   = cellCb->cellCfg.schInitialDlBwp.bwp.freqAlloc.numPrb; 

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
   dciInfo->format.format0_0.resourceAllocType   = puschInfo->resAllocType;
   dciInfo->format.format0_0.freqAlloc.startPrb  = puschInfo->fdAlloc.startPrb;
   dciInfo->format.format0_0.freqAlloc.numPrb    = puschInfo->fdAlloc.numPrb;
   dciInfo->format.format0_0.timeAlloc.startSymb = puschInfo->tdAlloc.startSymb;
   dciInfo->format.format0_0.timeAlloc.numSymb   = puschInfo->tdAlloc.numSymb;
   dciInfo->format.format0_0.rowIndex            = 0; /* row Index */
   dciInfo->format.format0_0.mcs                 = puschInfo->tbInfo.mcs;
   dciInfo->format.format0_0.harqProcId          = puschInfo->harqProcId;
   dciInfo->format.format0_0.puschHopFlag        = FALSE; /* disabled */
   dciInfo->format.format0_0.freqHopFlag         = FALSE; /* disabled */
   dciInfo->format.format0_0.ndi                 = puschInfo->tbInfo.ndi; /* new transmission */
   dciInfo->format.format0_0.rv                  = puschInfo->tbInfo.rv;
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
   dciInfo->dciInfo.pdschCfg                          = NULL; /* No DL data being sent */

   return ROK;
}

/*******************************************************************
 *
 * @brief Function to Modify Ue Config request from MAC
 *
 * @details
 *
 *    Function : MacSchModUeConfigReq
 *
 *    Functionality: Function to modify Ue Config request from MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacSchModUeConfigReq(Pst *pst, SchUeCfg *ueCfg)
{
   uint8_t ueId, lcIdx, ret = ROK;
   SchCellCb    *cellCb = NULLP;
   SchUeCb      *ueCb = NULLP;
   SchUeCfgRsp  cfgRsp;
   Inst         inst = pst->dstInst - 1;
   memset(&cfgRsp, 0, sizeof(SchUeCfgRsp));
  
#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTMAC -> ENTSCH : EVENT_MODIFY_UE_CONFIG_REQ_TO_SCH\n");
#endif

   if(!ueCfg)
   {
      DU_LOG("\nERROR  -->  SCH : Modifying Ue Config request failed at MacSchModUeConfigReq()");
      return RFAILED;
   }
   DU_LOG("\nDEBUG  -->  SCH : Modifying Ue Config Request for CRNTI[%d]", ueCfg->crnti);
   cellCb = getSchCellCb(pst->event, inst, ueCfg);

   /* Search if UE already configured */
   GET_UE_ID(ueCfg->crnti, ueId);
   ueCb = &cellCb->ueCb[ueId -1];
   
   if(!ueCb)
   {
      DU_LOG("\nERROR  -->  SCH : SchUeCb not found at MacSchModUeConfigReq() ");
      SchSendUeCfgRspToMac(pst->event, ueCfg, inst, RSP_NOK, &cfgRsp);
      return RFAILED;
   }
   if((ueCb->crnti == ueCfg->crnti) && (ueCb->state == SCH_UE_STATE_ACTIVE))
   {
      /* Found the UeCb to Reconfig */
      ret = fillSchUeCb(ueCb, ueCfg);
      if(ret == ROK)
      {
         ueCb->cellCb = cellCb;
         ueCb->srRcvd = false;
         ueCb->bsrRcvd = false;
         for(lcIdx=0; lcIdx<MAX_NUM_LOGICAL_CHANNEL_GROUPS; lcIdx++)
            ueCb->bsrInfo[lcIdx].dataVol = 0;

         SchSendUeCfgRspToMac(pst->event, ueCfg, inst, RSP_OK, &cfgRsp);
      }
   }
   return ret;
}

/*******************************************************************
*
* @brief Fill and send UE delete response to MAC
*
* @details
*
*    Function :  SchSendUeDeleteRspToMac
*
*    Functionality: Fill and send UE delete response to MAC
*
* @params[in] Inst inst, SchUeDelete  *ueDelete, SchMacRsp result, 
*              ErrorCause cause
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
void SchSendUeDeleteRspToMac(Inst inst, SchUeDelete  *ueDelete, SchMacRsp result, ErrorCause cause)
{
    Pst rspPst;
    SchUeDeleteRsp  delRsp;
    
    memset(&delRsp, 0, sizeof(SchUeDeleteRsp));
    delRsp.cellId = ueDelete->cellId;
    delRsp.crnti = ueDelete->crnti;
    delRsp.rsp = result; 
    delRsp.cause = cause; 

    /* Filling response post */
    memset(&rspPst, 0, sizeof(Pst));
    FILL_PST_SCH_TO_MAC(rspPst, inst);
    rspPst.event = EVENT_UE_DELETE_RSP_TO_MAC;
    SchUeDeleteRspOpts[rspPst.selector](&rspPst, &delRsp);
}

/*******************************************************************
*
* @brief Function to delete Sch Pucch ResrcCfg
*
* @details
*
*    Function : deleteSchPucchResrcCfg 
*
*    Functionality: Function to delete Sch Pucch ResrcCfg
*
* @params[in] SchPucchResrcCfg *resrc
* @return void 
*
* ****************************************************************/

void deleteSchPucchResrcCfg(SchPucchResrcCfg *resrc)
{
   uint8_t rsrcIdx=0;
   for(rsrcIdx=0; rsrcIdx < resrc->resrcToAddModListCount; rsrcIdx++)
   {
      switch(resrc->resrcToAddModList[rsrcIdx].pucchFormat)
      {
         case PUCCH_FORMAT_0:
         {
            SCH_FREE(resrc->resrcToAddModList[rsrcIdx].SchPucchFormat.format0,\
            sizeof(SchPucchFormat0));
            break;
         }
         case PUCCH_FORMAT_1:
         {
            SCH_FREE(resrc->resrcToAddModList[rsrcIdx].SchPucchFormat.format1,\
            sizeof(SchPucchFormat1));
            break;
         }
         case PUCCH_FORMAT_2:
         {
            SCH_FREE(resrc->resrcToAddModList[rsrcIdx].SchPucchFormat.format2,\
            sizeof(SchPucchFormat2_3));
            break;
         }
         case PUCCH_FORMAT_3:
         {
            SCH_FREE(resrc->resrcToAddModList[rsrcIdx].SchPucchFormat.format3,\
            sizeof(SchPucchFormat2_3));
            break;
         }
         case PUCCH_FORMAT_4:
         {
            SCH_FREE(resrc->resrcToAddModList[rsrcIdx].SchPucchFormat.format4,\
            sizeof(SchPucchFormat4));
            break;
         }
      }
   }
}

/*******************************************************************
*
* @brief Function to delete SCH Pdsch ServCellCfg
*
* @details
*
*    Function : deleteSchPdschServCellCfg
*
*    Functionality: Function to delete SCH Pdsch ServCellCfg
*
* @params[in] SchPdschServCellCfg *pdschServCellCfg
* @return void 
*
* ****************************************************************/

void deleteSchPdschServCellCfg(SchPdschServCellCfg *pdschServCellCfg)
{
   SCH_FREE(pdschServCellCfg->maxMimoLayers, sizeof(uint8_t));
   SCH_FREE(pdschServCellCfg->maxCodeBlkGrpPerTb, sizeof(SchMaxCodeBlkGrpPerTB));
   SCH_FREE(pdschServCellCfg->codeBlkGrpFlushInd, sizeof(bool));
   SCH_FREE(pdschServCellCfg->xOverhead, sizeof(SchPdschXOverhead));
}

/*******************************************************************
*
* @brief Function to  delete SCH UeCb
*
* @details
*
*    Function : deleteSchUeCb 
*
*    Functionality: Function to delete SCH UeCb
*
* @params[in]
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
void deleteSchUeCb(SchUeCb *ueCb) 
{
   uint8_t timeDomRsrcIdx = 0, ueLcIdx = 0;
   SchPucchCfg *pucchCfg = NULLP;
   SchPdschConfig *pdschCfg = NULLP;
   CmLListCp *lcLL  = NULLP;

   if(ueCb)
   {
      SCH_FREE(ueCb->ueCfg.ambrCfg, sizeof(SchAmbrCfg));
      if(ueCb->ueCfg.spCellCfgPres)
      {
         if(ueCb->ueCfg.spCellCfg.servCellCfg.initDlBwp.pdschCfgPres == true)
         {
            pdschCfg = &ueCb->ueCfg.spCellCfg.servCellCfg.initDlBwp.pdschCfg;
            for(timeDomRsrcIdx = 0; timeDomRsrcIdx < pdschCfg->numTimeDomRsrcAlloc; timeDomRsrcIdx++)
               SCH_FREE(pdschCfg->timeDomRsrcAllociList[timeDomRsrcIdx].k0, sizeof(uint8_t));
         }

         if(ueCb->ueCfg.spCellCfg.servCellCfg.initUlBwp.pucchCfgPres == true)
         {
            pucchCfg = &ueCb->ueCfg.spCellCfg.servCellCfg.initUlBwp.pucchCfg;
            SCH_FREE(pucchCfg->resrcSet,sizeof(SchPucchResrcSetCfg));
            if(pucchCfg->resrc)
            {
               deleteSchPucchResrcCfg(pucchCfg->resrc);
               SCH_FREE(pucchCfg->resrc, sizeof(SchPucchResrcCfg));
            }
            SCH_FREE(pucchCfg->format1, sizeof(SchPucchFormatCfg));
            SCH_FREE(pucchCfg->format2, sizeof(SchPucchFormatCfg));
            SCH_FREE(pucchCfg->format3, sizeof(SchPucchFormatCfg));
            SCH_FREE(pucchCfg->format4, sizeof(SchPucchFormatCfg));
            SCH_FREE(pucchCfg->schedReq, sizeof(SchPucchSchedReqCfg));
            SCH_FREE(pucchCfg->multiCsiCfg, sizeof(SchPucchMultiCsiCfg));
            SCH_FREE(pucchCfg->spatialInfo, sizeof(SchPucchSpatialCfg));  
            SCH_FREE(pucchCfg->dlDataToUlAck, sizeof(SchPucchDlDataToUlAck));
            SCH_FREE(pucchCfg->powerControl,sizeof(SchPucchPowerControl));
         }
         SCH_FREE(ueCb->ueCfg.spCellCfg.servCellCfg.bwpInactivityTmr, sizeof(uint8_t));
         deleteSchPdschServCellCfg(&ueCb->ueCfg.spCellCfg.servCellCfg.pdschServCellCfg);
      }
      /*Need to Free the memory allocated for S-NSSAI*/
      for(ueLcIdx = 0; ueLcIdx < MAX_NUM_LC; ueLcIdx++)
      {
         SCH_FREE(ueCb->ulInfo.ulLcCtxt[ueLcIdx].snssai, sizeof(Snssai));
         SCH_FREE(ueCb->dlInfo.dlLcCtxt[ueLcIdx].snssai, sizeof(Snssai));
      }

      /*Clearing out Dedicated LC list*/
      if(ueCb->dlLcPrbEst.dedLcInfo != NULLP)
      {
         lcLL = &(ueCb->dlLcPrbEst.dedLcInfo->dedLcList);
         deleteLcLL(lcLL);
         SCH_FREE(ueCb->dlLcPrbEst.dedLcInfo, sizeof(DedicatedLCInfo));
      }
      if(ueCb->ulLcPrbEst.dedLcInfo != NULLP)
      {
         lcLL = &(ueCb->ulLcPrbEst.dedLcInfo->dedLcList);
         deleteLcLL(lcLL);
         SCH_FREE(ueCb->ulLcPrbEst.dedLcInfo, sizeof(DedicatedLCInfo));
      }
      /*Deleteing the Default LC list*/
      lcLL = &(ueCb->dlLcPrbEst.defLcList);
      deleteLcLL(lcLL);

      lcLL = &(ueCb->ulLcPrbEst.defLcList);
      deleteLcLL(lcLL);

      memset(ueCb, 0, sizeof(SchUeCb));
   }
}

/*******************************************************************
*
* @brief Function for Ue Delete request from MAC to SCH
*
* @details
*
*    Function : MacSchUeDeleteReq 
*
*    Functionality: Function for Ue Delete request from MAC to SCH
*
* @params[in] Pst *pst, SchUeDelete  *ueDelete
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t MacSchUeDeleteReq(Pst *pst, SchUeDelete  *ueDelete)
{
    uint8_t      idx=0, ueId=0, ueIdToDel=0, ret=ROK;
    ErrorCause   result;
    SchCellCb    *cellCb = NULLP;
    Inst         inst = pst->dstInst - 1;
    CmLList      *node = NULL, *next = NULL;
   
#ifdef CALL_FLOW_DEBUG_LOG
    DU_LOG("\nCall Flow: ENTMAC -> ENTSCH : EVENT_UE_DELETE_REQ_TO_SCH\n");
#endif

    if(!ueDelete)
    {
       DU_LOG("\nERROR  -->  SCH : MacSchUeDeleteReq(): Ue Delete request failed");
       ret = RFAILED;
    }
    DU_LOG("\nDEBUG  -->  SCH : Ue Delete request received for crnti[%d]", ueDelete->crnti);
    
    cellCb = schCb[inst].cells[idx];

    if(cellCb->cellId != ueDelete->cellId)
    {
       DU_LOG("\nERROR  -->  SCH : MacSchUeDeleteReq(): cell Id is not available");
       result =  INVALID_CELLID;
    }
    else
    {
       GET_UE_ID(ueDelete->crnti, ueId);
       if(( cellCb->ueCb[ueId-1].crnti == ueDelete->crnti) && ( cellCb->ueCb[ueId-1].state == SCH_UE_STATE_ACTIVE))
       {
          deleteSchUeCb(&cellCb->ueCb[ueId-1]);
          ueIdToDel  = ueId;
          /* Remove UE from ueToBeScheduled list */
          node = cellCb->ueToBeScheduled.first;
          while(node)
          {
             next = node->next;
             ueId = *(uint8_t *)node->node;
             if(ueId == ueIdToDel)
             {
                SCH_FREE(node->node, sizeof(uint8_t));
                deleteNodeFromLList(&cellCb->ueToBeScheduled, node);
                break;
             }
             node = next;
          }
          cellCb->numActvUe--;
          result = NOT_APPLICABLE;
       }
       else
       {
          DU_LOG("\nERROR  -->  SCH : MacSchUeDeleteReq(): SchUeCb not found");
          result =  INVALID_UEID;
       }
    }
    
    if(result == NOT_APPLICABLE)
    {
       SchSendUeDeleteRspToMac(inst, ueDelete, RSP_OK, result);
    }
    else
    {
       SchSendUeDeleteRspToMac(inst, ueDelete, RSP_NOK, result);
       ret = RFAILED;
    }
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
uint8_t SchSendCellDeleteRspToMac(SchCellDelete  *ueDelete, Inst inst, SchMacRsp result)
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
   ret =  SchCellDeleteRspOpts[rspPst.selector](&rspPst, &delRsp);
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
   uint8_t sliceIdx=0, slotIdx=0;
   CmLListCp *list=NULL;
   CmLList *node=NULL, *next=NULL;

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

   if(cellCb->cellCfg.plmnInfoList.snssai)
   {
      for(sliceIdx=0; sliceIdx<cellCb->cellCfg.plmnInfoList.numSliceSupport; sliceIdx++)
      {
         SCH_FREE(cellCb->cellCfg.plmnInfoList.snssai[sliceIdx], sizeof(Snssai));
      }
      SCH_FREE(cellCb->cellCfg.plmnInfoList.snssai, cellCb->cellCfg.plmnInfoList.numSliceSupport*sizeof(Snssai*));
   }

   /* Remove all UE from ueToBeScheduled list and deallocate */
   node = cellCb->ueToBeScheduled.first;
   while(node)
   {
      next = node->next;
      SCH_FREE(node->node, sizeof(uint8_t));
      cmLListDelFrm(&cellCb->ueToBeScheduled, node);
      SCH_FREE(node, sizeof(CmLList));
      node = next;
   }

   memset(cellCb, 0, sizeof(SchCellCb));

}

/*******************************************************************
 *
 * @brief Function for cell Delete request from MAC to SCH
 *
 * @details
 *
 *    Function : MacSchCellDeleteReq
 *
 *    Functionality: Function for cell Delete request from MAC to SCH
 *
 * @params[in] Pst *pst, SchCellDelete  *cellDelete
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t MacSchCellDeleteReq(Pst *pst, SchCellDelete  *cellDelete)
{
   uint8_t   cellIdx=0, ret = RFAILED;
   Inst      inst = pst->dstInst - 1;
   SchMacRsp result= RSP_OK;
   
#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTMAC -> ENTSCH : EVENT_CELL_DELETE_REQ_TO_SCH\n");
#endif   

   if(!cellDelete)
   {
      DU_LOG("\nERROR  -->  SCH : MacSchCellDeleteReq(): Ue Delete request failed");
   }
   else
   {
      GET_CELL_IDX(cellDelete->cellId, cellIdx);
      if(schCb[inst].cells[cellIdx] == NULLP)
      { 
         DU_LOG("\nERROR  -->  SCH : MacSchCellDeleteReq(): cell Id[%d] is not available", cellDelete->cellId);
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
            DU_LOG("\nERROR  -->  SCH : MacSchCellDeleteReq(): cell Id[%d] is not available",cellDelete->cellId);
            result = RSP_NOK;
         }
      }

      if(SchSendCellDeleteRspToMac(cellDelete, inst, result)!=ROK)
      {
         DU_LOG("\nERROR  -->  SCH : MacSchCellDeleteReq(): failed to send Cell Delete response");
         ret =  RFAILED;
      }

   }
   return ret;   
}

/**********************************************************************
  End of file
 **********************************************************************/
