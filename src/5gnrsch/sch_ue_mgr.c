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
#include "sch_tmr.h"
#include "sch_utils.h"
#ifdef NR_DRX 
#include "sch_drx.h"
#endif

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
void SchSendUeCfgRspToMac(SchUeCfgReq *ueCfg, Inst inst,\
      SchMacRsp result, SchUeCfgRsp *cfgRsp)
{
   Pst rspPst;

   cfgRsp->cellId = ueCfg->cellId;
   cfgRsp->crnti = ueCfg->crnti;
   cfgRsp->rsp = result;   

   /* Filling response post */
   memset(&rspPst, 0, sizeof(Pst));
   FILL_PST_SCH_TO_MAC(rspPst, inst);
   rspPst.event = EVENT_UE_CONFIG_RSP_TO_MAC;
   DU_LOG("\nINFO  -->  SCH :  Sending UE Config response to MAC");
   MacMessageRouter(&rspPst, (void *)cfgRsp);
}

/*******************************************************************
 *
 * @brief Fill and send UE Recfg response to MAC
 *
 * @details
 *
 *    Function : SchSendUeRecfgRspToMac
 *
 *    Functionality: Fill and send UE Recfg response to MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void SchSendUeRecfgRspToMac(SchUeRecfgReq *ueRecfgReq, Inst inst,\
      SchMacRsp result, SchUeRecfgRsp *reCfgRsp)
{
   Pst rspPst;

   reCfgRsp->cellId = ueRecfgReq->cellId;
   reCfgRsp->crnti = ueRecfgReq->crnti;
   reCfgRsp->rsp = result;   

   /* Filling response post */
   memset(&rspPst, 0, sizeof(Pst));
   FILL_PST_SCH_TO_MAC(rspPst, inst);
   rspPst.event = EVENT_UE_RECONFIG_RSP_TO_MAC;
   DU_LOG("\nINFO  -->  SCH :  Sending UE Reconfig response to MAC");
    MacMessageRouter(&rspPst, (void *)reCfgRsp);
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
 * @params[arg] scheduler instance,
 *              snssai pointer,
 *              SchRrmPolicy pointer,
 *              SchLcPrbEstimate pointer , It will be filled
 *              isDedicated pointer,(Address of isDedicated flag in LC Context)
 *
 * @return ROK      >> This LC is part of RRM MemberList.
 *         RFAILED  >> FATAL Error
 *         ROKIGNORE >> This LC is not part of this RRM MemberList 
 *
 * ****************************************************************/

uint8_t updateDedLcInfo(Inst inst, Snssai *snssai, uint16_t *rsvdDedicatedPRB, bool *isDedicated)
{
   CmLList *sliceCfg = schCb[inst].sliceCfg.first;
   SchRrmPolicyOfSlice *rrmPolicyOfSlices;

   while(sliceCfg)
   {
      rrmPolicyOfSlices = (SchRrmPolicyOfSlice*)sliceCfg->node;
      if(rrmPolicyOfSlices && (memcmp(snssai, &(rrmPolicyOfSlices->snssai), sizeof(Snssai)) == 0))
      {
         /*Updating latest RrmPolicy*/
         *rsvdDedicatedPRB = \
                             (uint16_t)(((rrmPolicyOfSlices->rrmPolicyRatioInfo.dedicatedRatio)*(MAX_NUM_RB))/100);
         *isDedicated = TRUE;
         DU_LOG("\nINFO  -->  SCH : Updated RRM policy, reservedPOOL:%d",*rsvdDedicatedPRB);
         break;
      }
      sliceCfg = sliceCfg->next;
   }
   /*case: This LcCtxt  is either a Default LC or this LC is part of someother RRM_MemberList*/
   if(*isDedicated != TRUE) 
   {
      DU_LOG("\nINFO  -->  SCH : This SNSSAI is not a part of this RRMPolicy");
   }
   return ROK;	 
}

/*******************************************************************
 *
 * @brief Function to fill SpCellCfg in SchUeCb From SchUeCfg Req
 *
 * @details
 *
 *    Function : fillSpCellInSchCb
 *
 *    Functionality: Function to fill SpCellCfg in SchUeCb
 *
 * @params[in] Scheduler instance,
 *             SchUeCb pointer,
 *             SchUeCfg pointer
 * @return ROK/RFAILED
 *
 * ****************************************************************/

void fillSpCellInSchCb(SchSpCellRecfg *destSpCellCfg, SchSpCellCfg *srcSpCellCfg)
{
   destSpCellCfg->servCellIdx = srcSpCellCfg->servCellIdx;
   memcpy(&destSpCellCfg->servCellRecfg.initDlBwp, &srcSpCellCfg->servCellCfg.initDlBwp, sizeof(SchInitalDlBwp));
   destSpCellCfg->servCellRecfg.numDlBwpToAddOrMod =  srcSpCellCfg->servCellCfg.numDlBwpToAdd;
   if(destSpCellCfg->servCellRecfg.numDlBwpToAddOrMod > 0)
   {
      memcpy(destSpCellCfg->servCellRecfg.dlBwpToAddOrModList, srcSpCellCfg->servCellCfg.dlBwpToAddList,\
              (sizeof(SchDlBwpInfo) * srcSpCellCfg->servCellCfg.numDlBwpToAdd));
   }
   destSpCellCfg->servCellRecfg.firstActvDlBwpId = srcSpCellCfg->servCellCfg.firstActvDlBwpId;
   destSpCellCfg->servCellRecfg.defaultDlBwpId = srcSpCellCfg->servCellCfg.defaultDlBwpId;
   destSpCellCfg->servCellRecfg.bwpInactivityTmr = srcSpCellCfg->servCellCfg.bwpInactivityTmr;
   memcpy(&destSpCellCfg->servCellRecfg.pdschServCellCfg, &srcSpCellCfg->servCellCfg.pdschServCellCfg, sizeof(SchPdschServCellCfg));
   memcpy(&destSpCellCfg->servCellRecfg.initUlBwp, &srcSpCellCfg->servCellCfg.initUlBwp, sizeof(SchInitialUlBwp));
   
   destSpCellCfg->servCellRecfg.numDlBwpToAddOrMod =  srcSpCellCfg->servCellCfg.numDlBwpToAdd;
   if(destSpCellCfg->servCellRecfg.numDlBwpToAddOrMod > 0)
   {
      memcpy(destSpCellCfg->servCellRecfg.ulBwpToAddOrModList, srcSpCellCfg->servCellCfg.ulBwpToAddList,\
              (sizeof(SchUlBwpInfo) * srcSpCellCfg->servCellCfg.numUlBwpToAdd));
   }
   destSpCellCfg->servCellRecfg.firstActvUlBwpId = srcSpCellCfg->servCellCfg.firstActvUlBwpId;
   return;
}

/*******************************************************************
 *
 * @brief Function to fill SchUeCb From UeCfg Req
 *
 * @details
 *
 *    Function : fillSchUeCbFrmCfgReq
 *
 *    Functionality: Function to fill SchUeCb
 *
 * @params[in] Scheduler instance,
 *             SchUeCb pointer,
 *             SchUeCfg pointer
 * @return ROK/RFAILED
 *
 * ****************************************************************/

uint8_t fillSchUeCbFrmCfgReq(Inst inst, SchUeCb *ueCb, SchUeCfgReq *ueCfg)
{
   uint8_t   lcIdx, ueLcIdx, idx;
   SchPdschCfgCmn pdschCfg;
   SchPucchDlDataToUlAck *dlDataToUlAck;
   uint8_t retDL = ROK, retUL = ROK;
   bool isLcIdValid = FALSE;

   ueCb->ueCfg.cellId = ueCfg->cellId;
   ueCb->ueCfg.ueId = ueCb->ueId;
   ueCb->ueCfg.crnti = ueCfg->crnti;
   if(ueCfg->macCellGrpCfgPres == true)
   {
      memcpy(&ueCb->ueCfg.macCellGrpCfg , &ueCfg->macCellGrpCfg, sizeof(SchMacCellGrpCfg)); 
      ueCb->ueCfg.macCellGrpCfgPres = true;
#ifdef NR_DRX
      if(ueCfg->macCellGrpCfg.drxCfgPresent == true)
      {
         if(ueCb->ueDrxInfoPres == false)
         {
            ueCb->ueDrxInfoPres = true;
            /* intialize the drxUeCb */
            schInitDrxUeCb(ueCb);

            /* intialize the Dl drxHarqCb */
            for(idx =0; idx<ueCb->dlHqEnt.numHqPrcs; idx++)
            {
               schInitDrxHarqCb(&ueCb->dlHqEnt.procs[idx].dlDrxHarqCb);
            }
            /* intialize the Ul drxHarqCb */
            for(idx =0; idx<ueCb->ulHqEnt.numHqPrcs; idx++)
            {
               schInitDrxHarqCb(&ueCb->ulHqEnt.procs[idx].ulDrxHarqCb);
            }
            /* convert all the drx configuration recived in ms/subms into number of slots and store into the drxUeCb */
            schFillDrxUeCb(ueCb->cellCb->numerology, ueCfg->macCellGrpCfg.drxCfg, &ueCb->drxUeCb);
            /* Calculate the onduration timer and short cycle timer (if shortcycle configuration is present) as soon as we 
             * recived ueCfg request */
            schAddUeInOndurationList(ueCb->cellCb, ueCb, 0);

         }
         else
         {
            /* convert all the drx configuration recived in ms/subms into number
             * of slots and store into the drxUeCb */
            schFillDrxUeCb(ueCb->cellCb->numerology, ueCfg->macCellGrpCfg.drxCfg, &ueCb->drxUeCb);

            /* Recalculate/Restart timer based on their presence */
            schDrxUeReCfgTimer(ueCb->cellCb, ueCb);
         }
      }
#endif
   }

   if(ueCfg->phyCellGrpCfgPres == true)
   {
      memcpy(&ueCb->ueCfg.phyCellGrpCfg ,  &ueCfg->phyCellGrpCfg, sizeof(SchPhyCellGrpCfg));
      ueCb->ueCfg.phyCellGrpCfgPres = true;
   }

   if(ueCfg->spCellCfgPres == true)
   {
      if(ueCfg->spCellCfg.servCellCfg.initDlBwp.pdschCfgPres == true)
      {
         if(ueCb->ueCfg.spCellCfgPres && ueCb->ueCfg.spCellCfg.servCellRecfg.initDlBwp.pdschCfgPres == true)
         {
            for(idx = 0; idx < ueCfg->spCellCfg.servCellCfg.initDlBwp.pdschCfg.numTimeDomRsrcAlloc; idx++)
            {
               if(ueCb->ueCfg.spCellCfg.servCellRecfg.initDlBwp.pdschCfg.timeDomRsrcAllociList[idx].k0 && ueCfg->spCellCfg.servCellCfg.initDlBwp.pdschCfg.timeDomRsrcAllociList[idx].k0)
               {
                  SCH_FREE(ueCb->ueCfg.spCellCfg.servCellRecfg.initDlBwp.pdschCfg.timeDomRsrcAllociList[idx].k0, sizeof(uint8_t));  
               }
            }
         }
      }
      fillSpCellInSchCb(&ueCb->ueCfg.spCellCfg, &ueCfg->spCellCfg);

      ueCb->ueCfg.spCellCfgPres = true;
      dlDataToUlAck = ueCfg->spCellCfg.servCellCfg.initUlBwp.pucchCfg.dlDataToUlAck;
      if(ueCb->cellCb)
      {
         if(dlDataToUlAck)
         {
            BuildK0K1Table(ueCb->cellCb, &ueCb->k0K1InfoTbl, false, pdschCfg,\
                  ueCfg->spCellCfg.servCellCfg.initDlBwp.pdschCfg, dlDataToUlAck->dlDataToUlAckListCount,\
                  dlDataToUlAck->dlDataToUlAckList);
         }
         else
         {
            BuildK0K1Table(ueCb->cellCb, &ueCb->k0K1InfoTbl, false, pdschCfg,\
                  ueCfg->spCellCfg.servCellCfg.initDlBwp.pdschCfg, DEFAULT_UL_ACK_LIST_COUNT, defaultUlAckTbl);
         }
            ueCb->k0K1TblPrsnt = true;
            BuildK2InfoTable(ueCb->cellCb, ueCfg->spCellCfg.servCellCfg.initUlBwp.puschCfg.timeDomRsrcAllocList,\
                  ueCfg->spCellCfg.servCellCfg.initUlBwp.puschCfg.numTimeDomRsrcAlloc,\
                  NULLP, &ueCb->k2InfoTbl);
            ueCb->k2TblPrsnt = true;
      }
   }

   if(ueCfg->ambrCfg)
   {
      SCH_FREE(ueCb->ueCfg.ambrCfg, sizeof(SchAmbrCfg));
      ueCb->ueCfg.ambrCfg =  ueCfg->ambrCfg;
   }
   memcpy(&ueCb->ueCfg.dlModInfo,  &ueCfg->dlModInfo , sizeof(SchModulationInfo));
   memcpy(&ueCb->ueCfg.ulModInfo,  &ueCfg->ulModInfo , sizeof(SchModulationInfo));
   //Updating SchUlCb and SchDlCb DB in SchUeCb
   for(lcIdx = 0; lcIdx < ueCfg->numLcsToAdd; lcIdx++)
   {
      isLcIdValid = FALSE; /*Re-Initializing*/

      ueLcIdx = ueCfg->schLcCfg[lcIdx].lcId;
      CHECK_LCID(ueLcIdx, isLcIdValid);
      if(isLcIdValid == FALSE)
      {
         DU_LOG("ERROR --> SCH: LCID:%d is not Valid",ueLcIdx);
         continue;
      }
      fillSchUlLcCtxt(&ueCb->ulInfo.ulLcCtxt[ueLcIdx], &ueCfg->schLcCfg[lcIdx]);
      fillSchDlLcCtxt(&ueCb->dlInfo.dlLcCtxt[ueLcIdx], &ueCfg->schLcCfg[lcIdx]);

      /*Checking whether this LC belong to Dedicated S-NSSAI 
       * and Create the Dedicated LC List & Update the Reserve PRB number*/
      if(ueCb->dlInfo.dlLcCtxt[ueLcIdx].snssai != NULLP)
      {
         retDL = updateDedLcInfo(inst, ueCb->dlInfo.dlLcCtxt[ueLcIdx].snssai, &(ueCb->dlInfo.dlLcCtxt[ueLcIdx].rsvdDedicatedPRB),\
               &(ueCb->dlInfo.dlLcCtxt[ueLcIdx].isDedicated));
      }
      if(ueCb->ulInfo.ulLcCtxt[ueLcIdx].snssai != NULLP)
      {
         retUL =  updateDedLcInfo(inst, ueCb->ulInfo.ulLcCtxt[ueLcIdx].snssai, &(ueCb->ulInfo.ulLcCtxt[ueLcIdx].rsvdDedicatedPRB),\
               &(ueCb->ulInfo.ulLcCtxt[ueLcIdx].isDedicated));
      }

      if(retUL == RFAILED  || retDL == RFAILED)/*FATAL error*/
      {
         DU_LOG("\nERROR  -->  SCH : Failure in updateDedLcInfo");
         return RFAILED;
      }
      SCH_FREE(ueCfg->schLcCfg[lcIdx].drbQos, sizeof(SchDrbQosInfo));
      SCH_FREE(ueCfg->schLcCfg[lcIdx].snssai, sizeof(Snssai));

   }/* End of outer for loop */
   return ROK;
}

/*******************************************************************
 *
 * @brief Function to fill SchUeCb From UeReconfig Req
 *
 * @details
 *
 *    Function : fillSchUeCbFrmRecfgReq
 *
 *    Functionality: Function to fill SchUeCb
 *
 * @params[in] Scheduler instance,
 *             SchUeCb pointer,
 *             SchUeCfg pointer
 * @return ROK/RFAILED
 *
 * ****************************************************************/

uint8_t fillSchUeCbFrmRecfgReq(Inst inst, SchUeCb *ueCb, SchUeRecfgReq *ueRecfg)
{
   uint8_t   lcIdx, ueLcIdx, idx;
   SchPdschCfgCmn pdschCfg;
   SchPucchDlDataToUlAck *dlDataToUlAck;
   uint8_t retDL = ROK, retUL = ROK;
   bool isLcIdValid = FALSE;


   ueCb->ueCfg.cellId = ueRecfg->cellId;
   ueCb->ueCfg.crnti = ueRecfg->crnti;
   GET_UE_ID(ueRecfg->crnti, ueCb->ueCfg.ueId);
   ueCb->ueCfg.dataTransmissionAction = ueRecfg->dataTransmissionInfo;
   if(ueRecfg->macCellGrpRecfgPres == true)
   {
      memcpy(&ueCb->ueCfg.macCellGrpCfg , &ueRecfg->macCellGrpRecfg, sizeof(SchMacCellGrpCfg)); 
      ueCb->ueCfg.macCellGrpCfgPres = true;
#ifdef NR_DRX
      if(ueRecfg->macCellGrpRecfg.drxCfgPresent == true)
      {
         if(ueCb->ueDrxInfoPres == false)
         {
            ueCb->ueDrxInfoPres = true;
            /* intialize the drxUeCb */
            schInitDrxUeCb(ueCb);

            /* intialize the Dl drxHarqCb */
            for(idx =0; idx<ueCb->dlHqEnt.numHqPrcs; idx++)
            {
               schInitDrxHarqCb(&ueCb->dlHqEnt.procs[idx].dlDrxHarqCb);
            }
            /* intialize the Ul drxHarqCb */
            for(idx =0; idx<ueCb->ulHqEnt.numHqPrcs; idx++)
            {
               schInitDrxHarqCb(&ueCb->ulHqEnt.procs[idx].ulDrxHarqCb);
            }
            /* convert all the drx configuration recived in ms/subms into number of slots and store into the drxUeCb */
            schFillDrxUeCb(ueCb->cellCb->numerology, ueRecfg->macCellGrpRecfg.drxCfg, &ueCb->drxUeCb);
            /* Calculate the onduration timer and short cycle timer (if shortcycle configuration is present) as soon as we 
             * recived ueCfg request */
            schAddUeInOndurationList(ueCb->cellCb, ueCb, 0);

         }
         else
         {
            /* convert all the drx configuration recived in ms/subms into number
             * of slots and store into the drxUeCb */
            schFillDrxUeCb(ueCb->cellCb->numerology, ueRecfg->macCellGrpRecfg.drxCfg, &ueCb->drxUeCb);

            /* Recalculate/Restart timer based on their presence */
            schDrxUeReCfgTimer(ueCb->cellCb, ueCb);
         }
      }
#endif
   }

#ifdef NR_DRX
   if(ueRecfg->drxConfigIndicatorRelease == true)
   {
      if(ueCb->ueDrxInfoPres == true)
      {
         schDeleteUeDrxInfo(ueCb->cellCb, ueCb);
         ueCb->ueDrxInfoPres = false;
      }
   }
#endif

   if(ueRecfg->phyCellGrpRecfgPres == true)
   {
      memcpy(&ueCb->ueCfg.phyCellGrpCfg ,  &ueRecfg->phyCellGrpRecfg, sizeof(SchPhyCellGrpCfg));
      ueCb->ueCfg.phyCellGrpCfgPres = true;
   }

   if(ueRecfg->spCellRecfgPres == true)
   {
      if(ueRecfg->spCellRecfg.servCellRecfg.initDlBwp.pdschCfgPres == true)
      {
         if(ueCb->ueCfg.spCellCfgPres && ueCb->ueCfg.spCellCfg.servCellRecfg.initDlBwp.pdschCfgPres == true)
         {
            for(idx = 0; idx < ueRecfg->spCellRecfg.servCellRecfg.initDlBwp.pdschCfg.numTimeDomRsrcAlloc; idx++)
            {
               if(ueCb->ueCfg.spCellCfg.servCellRecfg.initDlBwp.pdschCfg.timeDomRsrcAllociList[idx].k0 && ueRecfg->spCellRecfg.servCellRecfg.initDlBwp.pdschCfg.timeDomRsrcAllociList[idx].k0)
               {
                  SCH_FREE(ueCb->ueCfg.spCellCfg.servCellRecfg.initDlBwp.pdschCfg.timeDomRsrcAllociList[idx].k0, sizeof(uint8_t));  
               }
            }
         }
      }
      memcpy(&ueCb->ueCfg.spCellCfg , &ueRecfg->spCellRecfg, sizeof(SchSpCellRecfg));

      ueCb->ueCfg.spCellCfgPres = true;
      dlDataToUlAck = ueRecfg->spCellRecfg.servCellRecfg.initUlBwp.pucchCfg.dlDataToUlAck;
      if(ueCb->cellCb)
      {
         if(dlDataToUlAck)
         {
            BuildK0K1Table(ueCb->cellCb, &ueCb->k0K1InfoTbl, false, pdschCfg,\
                  ueRecfg->spCellRecfg.servCellRecfg.initDlBwp.pdschCfg, dlDataToUlAck->dlDataToUlAckListCount,\
                  dlDataToUlAck->dlDataToUlAckList);
            ueCb->k0K1TblPrsnt = true;
            BuildK2InfoTable(ueCb->cellCb, ueRecfg->spCellRecfg.servCellRecfg.initUlBwp.puschCfg.timeDomRsrcAllocList,\
                  ueRecfg->spCellRecfg.servCellRecfg.initUlBwp.puschCfg.numTimeDomRsrcAlloc,\
                  NULLP, &ueCb->k2InfoTbl);
            ueCb->k2TblPrsnt = true;
         }
      }
   }

   if(ueRecfg->ambrRecfg)
   {
      SCH_FREE(ueCb->ueCfg.ambrCfg, sizeof(SchAmbrCfg));
      ueCb->ueCfg.ambrCfg =  ueRecfg->ambrRecfg;
   }
   memcpy(&ueCb->ueCfg.dlModInfo,  &ueRecfg->dlModInfo , sizeof(SchModulationInfo));
   memcpy(&ueCb->ueCfg.ulModInfo,  &ueRecfg->ulModInfo , sizeof(SchModulationInfo));
   //Updating Num of LC to ADD SchUlCb and SchDlCb DB in SchUeCb
   for(lcIdx = 0; lcIdx < ueRecfg->numLcsToAdd; lcIdx++)
   {
      isLcIdValid = FALSE; /*Re-Initializing*/

      ueLcIdx = ueRecfg->schLcCfgAdd[lcIdx].lcId;
      CHECK_LCID(ueLcIdx, isLcIdValid);
      if(isLcIdValid == FALSE)
      {
         DU_LOG("ERROR --> SCH: LCID:%d is not Valid",ueLcIdx);
         continue;
      }
      fillSchUlLcCtxt(&ueCb->ulInfo.ulLcCtxt[ueLcIdx], &ueRecfg->schLcCfgAdd[lcIdx]);
      fillSchDlLcCtxt(&ueCb->dlInfo.dlLcCtxt[ueLcIdx], &ueRecfg->schLcCfgAdd[lcIdx]);

      /*Checking whether this LC belong to Dedicated S-NSSAI 
       * and Create the Dedicated LC List & Update the Reserve PRB number*/
      if(ueCb->dlInfo.dlLcCtxt[ueLcIdx].snssai != NULLP)
      {
         retDL = updateDedLcInfo(inst, ueCb->dlInfo.dlLcCtxt[ueLcIdx].snssai, &(ueCb->dlInfo.dlLcCtxt[ueLcIdx].rsvdDedicatedPRB),\
               &(ueCb->dlInfo.dlLcCtxt[ueLcIdx].isDedicated));
      }
      if(ueCb->ulInfo.ulLcCtxt[ueLcIdx].snssai != NULLP)
      {
         retUL =  updateDedLcInfo(inst, ueCb->ulInfo.ulLcCtxt[ueLcIdx].snssai, &(ueCb->ulInfo.ulLcCtxt[ueLcIdx].rsvdDedicatedPRB),\
               &(ueCb->ulInfo.ulLcCtxt[ueLcIdx].isDedicated));
      }

      if(retUL == RFAILED  || retDL == RFAILED)/*FATAL error*/
      {
         DU_LOG("\nERROR  -->  SCH : Failure in updateDedLcInfo");
         return RFAILED;
      }
      SCH_FREE(ueRecfg->schLcCfgAdd[lcIdx].drbQos, sizeof(SchDrbQosInfo));
      SCH_FREE(ueRecfg->schLcCfgAdd[lcIdx].snssai, sizeof(Snssai));
   }
   //Updating Num of LC to DEL SchUlCb and SchDlCb DB in SchUeCb
   for(lcIdx = 0; lcIdx < ueRecfg->numLcsToDel; lcIdx++)
   {
      ueLcIdx = ueRecfg->lcIdToDel[lcIdx];

      if(ueCb->dlInfo.dlLcCtxt[ueLcIdx].lcId == ueRecfg->lcIdToDel[lcIdx])
      {
         /*Delete the LC node from the DL LC List*/
         if(ueCb->dlInfo.dlLcCtxt[ueLcIdx].isDedicated)
         {
            /*Remove from HARQ Transmission or retransmission*/
         }
         else
         {
         }
         SCH_FREE(ueCb->dlInfo.dlLcCtxt[ueLcIdx].snssai, sizeof(Snssai));
         memset(&ueCb->dlInfo.dlLcCtxt[ueLcIdx], 0, sizeof(SchDlLcCtxt));
      }
      if(ueCb->ulInfo.ulLcCtxt[ueLcIdx].lcId == ueRecfg->lcIdToDel[lcIdx])
      {
         /*Delete the LC node from the UL LC List*/
         if(ueCb->ulInfo.ulLcCtxt[ueLcIdx].isDedicated)
         {
            /*Remove from HARQ Transmission or retransmission*/
         }
         else/*Default LC list*/
         {

         }
         SCH_FREE(ueCb->ulInfo.ulLcCtxt[ueLcIdx].snssai, sizeof(Snssai));
         memset(&ueCb->ulInfo.ulLcCtxt[ueLcIdx], 0, sizeof(SchUlLcCtxt));
      }
   }
   //Updating Num of LC to DEL SchUlCb and SchDlCb DB in SchUeCb
   for(lcIdx = 0; lcIdx < ueRecfg->numLcsToMod; lcIdx++)
   {
      ueLcIdx = ueRecfg->schLcCfgMod[lcIdx].lcId;

      if(ueCb->ulInfo.ulLcCtxt[ueLcIdx].lcId == ueRecfg->schLcCfgMod[lcIdx].lcId)
      {
         fillSchUlLcCtxt(&ueCb->ulInfo.ulLcCtxt[ueLcIdx], &ueRecfg->schLcCfgMod[lcIdx]);
         /*Updating the RRM reserved pool PRB count*/
         if(ueCb->ulInfo.ulLcCtxt[ueLcIdx].snssai != NULLP)
         {
            retUL =  updateDedLcInfo(inst, ueCb->ulInfo.ulLcCtxt[ueLcIdx].snssai, &(ueCb->ulInfo.ulLcCtxt[ueLcIdx].rsvdDedicatedPRB),\
                  &(ueCb->ulInfo.ulLcCtxt[ueLcIdx].isDedicated));
         }
         if(retUL == RFAILED)
         {
            DU_LOG("\nERROR  -->  SCH : Failed in updating Ded Lc info");
            return RFAILED;
         }
      }/*End of UL LC Ctxt*/

      if(ueCb->dlInfo.dlLcCtxt[ueLcIdx].lcId == ueRecfg->schLcCfgMod[lcIdx].lcId)
      {
         fillSchDlLcCtxt(&ueCb->dlInfo.dlLcCtxt[ueLcIdx], &ueRecfg->schLcCfgMod[lcIdx]);
         /*Updating the RRM policy*/
         if(ueCb->dlInfo.dlLcCtxt[ueLcIdx].snssai != NULLP)
         {
            retDL = updateDedLcInfo(inst, ueCb->dlInfo.dlLcCtxt[ueLcIdx].snssai, &(ueCb->dlInfo.dlLcCtxt[ueLcIdx].rsvdDedicatedPRB), \
                  &(ueCb->dlInfo.dlLcCtxt[ueLcIdx].isDedicated));
         }
         if(retDL == RFAILED)
         {
            DU_LOG("\nERROR  -->  SCH : Failed in updating Ded Lc info");
            return RFAILED;
         }
      }/*End of DL LC ctxt*/

      SCH_FREE(ueRecfg->schLcCfgMod[lcIdx].drbQos, sizeof(SchDrbQosInfo));
      SCH_FREE(ueRecfg->schLcCfgMod[lcIdx].snssai, sizeof(Snssai));
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

SchCellCb *getSchCellCb(uint16_t srcEvent, Inst inst, uint16_t cellId)
{
   uint8_t      idx = 0;
   SchCellCb    *cellCb = NULLP;

   /* Search of cell cb */
   for(idx = 0; idx < MAX_NUM_CELL; idx++)
   {
      cellCb = schCb[inst].cells[idx];
      if((cellCb != NULLP) && (cellCb->cellId == cellId))
         break;
   }
   if((idx == MAX_NUM_CELL) || (cellCb == NULLP))
   {
      DU_LOG("\nERROR  -->  SCH : Ue create request failed. Invalid cell id %d", cellId);
      return NULLP;
   }

   /* Check if max number of UE configured */
   if(cellCb->numActvUe > MAX_NUM_UE)
   {
      DU_LOG("\nERROR  -->  SCH :  Max number of UE [%d] already configured", MAX_NUM_UE);
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
 *    Function : SchAddUeConfigReq
 *
 *    Functionality: Function to Add Ue config request from MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t SchAddUeConfigReq(Pst *pst, SchUeCfgReq *ueCfg)
{
   uint8_t      lcIdx = 0, ret = ROK, idx = 0;
   uint16_t     ueId = 0;
   SchCellCb    *cellCb = NULLP;
   SchUeCb      *ueCb = NULLP;
   SchUeCfgRsp  cfgRsp;
   Inst         inst = pst->dstInst - SCH_INST_START;
   memset(&cfgRsp, 0, sizeof(SchUeCfgRsp));
  
   if(!ueCfg)
   {
      DU_LOG("\nERROR  -->  SCH :  Adding UE Config Request failed at SchAddUeConfigReq()");
      return RFAILED;
   }
   DU_LOG("\nDEBUG  -->  SCH :  Adding UE Config Request for CRNTI[%d]", ueCfg->crnti);
   cellCb = getSchCellCb(pst->event, inst, ueCfg->cellId);

   if(cellCb == NULLP)
   {
      SchSendUeCfgRspToMac(ueCfg, inst, RSP_NOK, &cfgRsp);
      return RFAILED; 
   }
   /* Search if UE already configured */
   GET_UE_ID(ueCfg->crnti, ueId);
   ueCb = &cellCb->ueCb[ueId - 1];

   if((ueCb->crnti == ueCfg->crnti) && (ueCb->state == SCH_UE_STATE_ACTIVE))
   {
      DU_LOG("\nDEBUG  -->  SCH : CRNTI %d already configured ", ueCfg->crnti);
      SchSendUeCfgRspToMac(ueCfg, inst, RSP_OK, &cfgRsp);
      return ROK;
   }

   /* Fill received Ue Configuration in UeCb */
   memset(ueCb, 0, sizeof(SchUeCb));

   ueCb->crnti = ueCfg->crnti;
   GET_UE_ID(ueCb->crnti, ueCb->ueId);
   ueCb->cellCb = cellCb;
   schUlHqEntInit(cellCb, &cellCb->ueCb[ueCb->ueId-1]);
   schDlHqEntInit(cellCb, &cellCb->ueCb[ueCb->ueId-1]);
   SCH_ALLOC(ueCb->hqDlmap, sizeof(SchHqDlMap*)*(ueCb->cellCb->numSlots));
   SCH_ALLOC(ueCb->hqUlmap, sizeof(SchHqUlMap*)*(ueCb->cellCb->numSlots));

   if ( (ueCb->hqDlmap == NULLP) || (ueCb->hqUlmap == NULLP) )
   {
      DU_LOG("\nINFO  -->  SCH : Memory Allocation Failed");
      return RFAILED;
   }
   for (idx = 0; idx<ueCb->cellCb->numSlots; idx++)
   {
      SCH_ALLOC(ueCb->hqDlmap[idx], sizeof(SchHqDlMap));
      SCH_ALLOC(ueCb->hqUlmap[idx], sizeof(SchHqUlMap));
      
      if ( (ueCb->hqDlmap[idx] == NULLP) || (ueCb->hqUlmap[idx] == NULLP) )
      {
         DU_LOG("\nINFO  -->  SCH : Memory Allocation Failed");
         return RFAILED;
      }
      cmLListInit(&ueCb->hqDlmap[idx]->hqList);
      cmLListInit(&ueCb->hqUlmap[idx]->hqList);
   }
   ret = fillSchUeCbFrmCfgReq(inst, ueCb, ueCfg);
   
   if(fillUeCoresetAndSsInfo(ueCb) == RFAILED)
   {
       DU_LOG("\nERROR  -->  SCH : Memory Allocation Failed");
       return RFAILED;
   }
   if(ret == ROK)
   {
      /* If UE has initiated RACH and then UE context is created, it means UE is
       * active now.
       * Else if UE context is created before RACH, this means that UE is being
       * handed-in from source DU */
      if(cellCb->raCb[ueCb->ueId-1].tcrnti == ueCb->crnti)
      {
         cellCb->numActvUe++;
         SET_ONE_BIT(ueCb->ueId, cellCb->actvUeBitMap);
         ueCb->state = SCH_UE_STATE_ACTIVE;
      }
      else
      {
         ueCb->state = SCH_UE_HANDIN_IN_PROGRESS;
      }

      ueCb->srRcvd = false;
      ueCb->bsrRcvd = false;
      for(lcIdx=0; lcIdx<MAX_NUM_LOGICAL_CHANNEL_GROUPS; lcIdx++)
         ueCb->bsrInfo[lcIdx].dataVol = 0;

      SchSendUeCfgRspToMac(ueCfg, inst, RSP_OK, &cfgRsp);
   }

   cellCb->api->SchAddUeConfigReq(ueCb);
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
* @params[in] SchUeCb *ueCb, SlotTimingInfo puschTime, uint32_t tbSize
* @params[in] uint8_t startSymb, uint8_t symbLen, uint16_t startPrb
* @params[in] bool isRetx, SchUlHqProcCb *hq
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t schFillPuschAlloc(SchUeCb *ueCb, SlotTimingInfo puschTime, uint32_t tbSize, 
                          uint8_t startSymb, uint8_t symbLen, uint16_t startPrb, bool isRetx, SchUlHqProcCb *hqP)
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

   if (isRetx == FALSE)
   {
      puschInfo.harqProcId                       = SCH_HARQ_PROC_ID;
      puschInfo.fdAlloc.resAllocType             = SCH_ALLOC_TYPE_1;
      puschInfo.fdAlloc.resAlloc.type1.startPrb  = startPrb;
      puschInfo.fdAlloc.resAlloc.type1.numPrb    = numRb;
      puschInfo.tdAlloc.startSymb = startSymb;
      puschInfo.tdAlloc.numSymb   = symbLen;
      puschInfo.tbInfo.qamOrder   = ueCb->ueCfg.ulModInfo.modOrder;
      puschInfo.tbInfo.mcs        = ueCb->ueCfg.ulModInfo.mcsIndex;
      puschInfo.tbInfo.mcsTable   = ueCb->ueCfg.ulModInfo.mcsTable;
      puschInfo.tbInfo.ndi        = 1; /* new transmission */
      puschInfo.tbInfo.rv         = 0;
      puschInfo.tbInfo.tbSize     = tbSize;
#ifdef INTEL_FAPI      
      puschInfo.dmrsMappingType   = DMRS_MAP_TYPE_A;  /* Setting Type-A */
      puschInfo.nrOfDmrsSymbols   = NUM_DMRS_SYMBOLS;
      puschInfo.dmrsAddPos        = DMRS_ADDITIONAL_POS;
#endif      
      hqP->puschResType = puschInfo.fdAlloc.resAllocType;
      hqP->puschStartPrb = puschInfo.fdAlloc.resAlloc.type1.startPrb;
      hqP->puschNumPrb = puschInfo.fdAlloc.resAlloc.type1.numPrb;
      hqP->strtSymbl = puschInfo.tdAlloc.startSymb;
      hqP->numSymbl = puschInfo.tdAlloc.numSymb;
      hqP->tbInfo.qamOrder = puschInfo.tbInfo.qamOrder;
      hqP->tbInfo.iMcs = puschInfo.tbInfo.mcs;
      hqP->tbInfo.mcsTable = puschInfo.tbInfo.mcsTable;
      hqP->tbInfo.ndi = puschInfo.tbInfo.ndi;
      hqP->tbInfo.rv = puschInfo.tbInfo.rv;
      hqP->tbInfo.rvIdx = 0;
      hqP->tbInfo.tbSzReq = puschInfo.tbInfo.tbSize;
#ifdef INTEL_FAPI      
      hqP->dmrsMappingType = puschInfo.dmrsMappingType;
      hqP->nrOfDmrsSymbols = puschInfo.nrOfDmrsSymbols;
      hqP->dmrsAddPos = puschInfo.dmrsAddPos;
#endif
   }
   else
   {
      puschInfo.harqProcId                       = hqP->procId;
      puschInfo.fdAlloc.resAllocType             = hqP->puschResType;
      puschInfo.fdAlloc.resAlloc.type1.startPrb  = hqP->puschStartPrb;
      puschInfo.fdAlloc.resAlloc.type1.numPrb    = hqP->puschNumPrb;
      puschInfo.tdAlloc.startSymb = hqP->strtSymbl;
      puschInfo.tdAlloc.numSymb   = hqP->numSymbl;
      puschInfo.tbInfo.qamOrder   = hqP->tbInfo.qamOrder;
      puschInfo.tbInfo.mcs        = hqP->tbInfo.iMcs;
      puschInfo.tbInfo.mcsTable   = hqP->tbInfo.mcsTable;
      puschInfo.tbInfo.ndi        = hqP->tbInfo.ndi; /* retransmission */
      hqP->tbInfo.rvIdx = (hqP->tbInfo.rvIdx +1) & 0x3;
      puschInfo.tbInfo.rv         = schCmnDlRvTbl[hqP->tbInfo.rvIdx];
      puschInfo.tbInfo.tbSize     = hqP->tbInfo.tbSzReq;
#ifdef INTEL_FAPI      
      puschInfo.dmrsMappingType   = hqP->dmrsMappingType;  /* Setting Type-A */
      puschInfo.nrOfDmrsSymbols   = hqP->nrOfDmrsSymbols;
      puschInfo.dmrsAddPos        = hqP->dmrsAddPos;
#endif 
  }
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
* @brief Fills UL DCI information for MSG3 retransmission
*
* @details
*
*    Function : schFillUlDciForMsg3Retx
*
*    Functionality: fills UL DCI information for MSG3 retransmission
*
* @params[in]
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t schFillUlDciForMsg3Retx(SchRaCb *raCb, SchPuschInfo *puschInfo, DciInfo *dciInfo)
{
   SchCellCb         *cellCb  = raCb->cell;
   
   dciInfo->crnti  = raCb->tcrnti;
   SchUlHqProcCb *msg3HqProc = &raCb->msg3HqProc;
   if (msg3HqProc == NULLP)
   {
      return RFAILED;
   }

   /* fill bwp cfg */
   dciInfo->bwpCfg.subcarrierSpacing  = cellCb->sib1SchCfg.bwp.subcarrierSpacing;
   dciInfo->bwpCfg.cyclicPrefix       = cellCb->sib1SchCfg.bwp.cyclicPrefix;
   dciInfo->bwpCfg.freqAlloc.startPrb = cellCb->cellCfg.dlCfgCommon.schInitialDlBwp.bwp.freqAlloc.startPrb;
   dciInfo->bwpCfg.freqAlloc.numPrb   = cellCb->cellCfg.dlCfgCommon.schInitialDlBwp.bwp.freqAlloc.numPrb; 

   /*fill coreset cfg */
   //Considering number of RBs in coreset1 is same as coreset0
   dciInfo->coresetCfg.coreSetSize      = coresetIdxTable[0][1];
   //Considering coreset1 also starts from same symbol as coreset0
   dciInfo->coresetCfg.startSymbolIndex = searchSpaceIdxTable[0][3];
   dciInfo->coresetCfg.durationSymbols  = coresetIdxTable[0][2];
   memcpy(dciInfo->coresetCfg.freqDomainResource, cellCb->cellCfg.dlCfgCommon.schInitialDlBwp.pdcchCommon.commonSearchSpace.freqDomainRsrc, FREQ_DOM_RSRC_SIZE);
   
   dciInfo->coresetCfg.cceRegMappingType   = 1; /* coreset0 is always interleaved */
   dciInfo->coresetCfg.regBundleSize       = 6; /* spec-38.211 sec 7.3.2.2 */
   dciInfo->coresetCfg.interleaverSize     = 2; /* spec-38.211 sec 7.3.2.2 */
   dciInfo->coresetCfg.coreSetType         = 0;
   dciInfo->coresetCfg.coreSetSize         = coresetIdxTable[0][1];
   dciInfo->coresetCfg.shiftIndex          = cellCb->cellCfg.phyCellId;
   dciInfo->coresetCfg.precoderGranularity = 0;
   dciInfo->coresetCfg.cceIndex            = 0; /* 0-3 for UL and 4-7 for DL */
   dciInfo->coresetCfg.aggregationLevel    = 4; /* same as for sib1 */
   
   dciInfo->dciFormatInfo.formatType = FORMAT0_0;
   msg3HqProc->tbInfo.rvIdx++;
   msg3HqProc->tbInfo.rv = schCmnDlRvTbl[msg3HqProc->tbInfo.rvIdx & 0x03];
   /* fill UL grant */
   dciInfo->dciFormatInfo.format.format0_0.resourceAllocType                  = msg3HqProc->puschResType;
   dciInfo->dciFormatInfo.format.format0_0.freqAlloc.resAllocType             = msg3HqProc->puschResType;
   dciInfo->dciFormatInfo.format.format0_0.freqAlloc.resAlloc.type1.startPrb  = msg3HqProc->puschStartPrb;
   dciInfo->dciFormatInfo.format.format0_0.freqAlloc.resAlloc.type1.numPrb    = msg3HqProc->puschNumPrb;
   dciInfo->dciFormatInfo.format.format0_0.timeAlloc.startSymb = msg3HqProc->strtSymbl;
   dciInfo->dciFormatInfo.format.format0_0.timeAlloc.numSymb   = msg3HqProc->numSymbl;
   dciInfo->dciFormatInfo.format.format0_0.rowIndex            = 0; /* row Index */
   dciInfo->dciFormatInfo.format.format0_0.mcs                 = msg3HqProc->tbInfo.iMcs;
   dciInfo->dciFormatInfo.format.format0_0.harqProcId          = msg3HqProc->procId;
   dciInfo->dciFormatInfo.format.format0_0.freqHopFlag         = FALSE; /* disabled */
   dciInfo->dciFormatInfo.format.format0_0.ndi                 = msg3HqProc->tbInfo.ndi; /* new transmission */
   dciInfo->dciFormatInfo.format.format0_0.rvIndex             = msg3HqProc->tbInfo.rv;
   dciInfo->dciFormatInfo.format.format0_0.tpcCmd              = 0; //Sphoorthi TODO: check
   dciInfo->dciFormatInfo.format.format0_0.sulIndicator             = FALSE; /* SUL not configured */
   
   /* Fill DCI Structure */
   dciInfo->dciInfo.rnti                              = raCb->tcrnti;
   dciInfo->dciInfo.scramblingId                      = cellCb->cellCfg.phyCellId;
   dciInfo->dciInfo.scramblingRnti                    = 0;
   dciInfo->dciInfo.cceIndex                          = 0; /* 0-3 for UL and 4-7 for DL */
   dciInfo->dciInfo.aggregLevel                       = 4;
   dciInfo->dciInfo.beamPdcchInfo.numPrgs             = 1;
   dciInfo->dciInfo.beamPdcchInfo.prgSize             = 1;
   dciInfo->dciInfo.beamPdcchInfo.digBfInterfaces     = 0;
   dciInfo->dciInfo.beamPdcchInfo.prg[0].pmIdx        = 0;
   dciInfo->dciInfo.beamPdcchInfo.prg[0].beamIdx[0]   = 0;
   dciInfo->dciInfo.txPdcchPower.beta_pdcch_1_0       = 0;
   dciInfo->dciInfo.txPdcchPower.powerControlOffsetSS = 0;
   memset(&dciInfo->dciInfo.pdschCfg, 0, sizeof(PdschCfg));
   msg3HqProc->tbInfo.txCntr++;

  puschInfo->harqProcId                       = msg3HqProc->procId;
  puschInfo->fdAlloc.resAllocType             = msg3HqProc->puschResType;
  puschInfo->fdAlloc.resAlloc.type1.startPrb  = msg3HqProc->puschStartPrb;
  puschInfo->fdAlloc.resAlloc.type1.numPrb    = msg3HqProc->puschNumPrb;
  puschInfo->tdAlloc.startSymb = msg3HqProc->strtSymbl;
  puschInfo->tdAlloc.numSymb   = msg3HqProc->numSymbl;
  puschInfo->tbInfo.qamOrder   = msg3HqProc->tbInfo.qamOrder;
  puschInfo->tbInfo.mcs        = msg3HqProc->tbInfo.iMcs;
  puschInfo->tbInfo.mcsTable   = msg3HqProc->tbInfo.mcsTable;
  puschInfo->tbInfo.ndi        = msg3HqProc->tbInfo.ndi; /* retransmission */
  puschInfo->tbInfo.rv         = msg3HqProc->tbInfo.rvIdx;
  puschInfo->tbInfo.tbSize     = msg3HqProc->tbInfo.tbSzReq;
#ifdef INTEL_FAPI
  puschInfo->dmrsMappingType   = msg3HqProc->dmrsMappingType;  /* Setting Type-A */
  puschInfo->nrOfDmrsSymbols   = msg3HqProc->nrOfDmrsSymbols;
  puschInfo->dmrsAddPos        = msg3HqProc->dmrsAddPos;
#endif
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
 * @params[in] SchUeCb *ueCb, SchPuschInfo *puschInfo, DciInfo *dciInfo,
 * @params[in] bool isRetx, SchUlHqProcCb *hqP 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t schFillUlDci(SchUeCb *ueCb, SchPuschInfo *puschInfo, DciInfo *dciInfo, bool isRetx, SchUlHqProcCb *hqP)
{
   SchCellCb         *cellCb  = ueCb->cellCb;
   SchControlRsrcSet coreset1 ;
  
   memset(&coreset1, 0, sizeof(SchControlRsrcSet));
   if(ueCb->ueCfg.spCellCfgPres == true)
   {
     coreset1 = ueCb->ueCfg.spCellCfg.servCellRecfg.initDlBwp.pdcchCfg.cRSetToAddModList[0];
   }
   
   dciInfo->crnti  = ueCb->crnti;

   /* fill bwp cfg */
   dciInfo->bwpCfg.subcarrierSpacing  = cellCb->sib1SchCfg.bwp.subcarrierSpacing;
   dciInfo->bwpCfg.cyclicPrefix       = cellCb->sib1SchCfg.bwp.cyclicPrefix;
   dciInfo->bwpCfg.freqAlloc.startPrb = cellCb->cellCfg.dlCfgCommon.schInitialDlBwp.bwp.freqAlloc.startPrb;
   dciInfo->bwpCfg.freqAlloc.numPrb   = cellCb->cellCfg.dlCfgCommon.schInitialDlBwp.bwp.freqAlloc.numPrb; 

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
   
   dciInfo->dciFormatInfo.formatType = FORMAT0_0;
   
   /* fill UL grant */
   dciInfo->dciFormatInfo.format.format0_0.resourceAllocType   = puschInfo->fdAlloc.resAllocType;
   dciInfo->dciFormatInfo.format.format0_0.freqAlloc.resAllocType = puschInfo->fdAlloc.resAllocType;
   dciInfo->dciFormatInfo.format.format0_0.freqAlloc.resAlloc.type1.startPrb  = \
   puschInfo->fdAlloc.resAlloc.type1.startPrb;
   dciInfo->dciFormatInfo.format.format0_0.freqAlloc.resAlloc.type1.numPrb    = \
   puschInfo->fdAlloc.resAlloc.type1.numPrb;
   dciInfo->dciFormatInfo.format.format0_0.timeAlloc.startSymb = puschInfo->tdAlloc.startSymb;
   dciInfo->dciFormatInfo.format.format0_0.timeAlloc.numSymb   = puschInfo->tdAlloc.numSymb;
   dciInfo->dciFormatInfo.format.format0_0.rowIndex            = 0; /* row Index */
   dciInfo->dciFormatInfo.format.format0_0.mcs                 = puschInfo->tbInfo.mcs;
   dciInfo->dciFormatInfo.format.format0_0.harqProcId          = puschInfo->harqProcId;
   dciInfo->dciFormatInfo.format.format0_0.freqHopFlag         = FALSE; /* disabled */
   dciInfo->dciFormatInfo.format.format0_0.ndi                 = puschInfo->tbInfo.ndi; /* new transmission */
   dciInfo->dciFormatInfo.format.format0_0.rvIndex             = puschInfo->tbInfo.rv;
   dciInfo->dciFormatInfo.format.format0_0.tpcCmd              = 0; //Sphoorthi TODO: check
   dciInfo->dciFormatInfo.format.format0_0.sulIndicator             = FALSE; /* SUL not configured */
   
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
   dciInfo->dciInfo.txPdcchPower.beta_pdcch_1_0       = 0;
   dciInfo->dciInfo.txPdcchPower.powerControlOffsetSS = 0;
   memset(&dciInfo->dciInfo.pdschCfg, 0, sizeof(PdschCfg));

   return ROK;
}

/*******************************************************************
 *
 * @brief Function to Modify Ue Config request from MAC
 *
 * @details
 *
 *    Function : SchModUeConfigReq
 *
 *    Functionality: Function to modify Ue Config request from MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t SchModUeConfigReq(Pst *pst, SchUeRecfgReq *ueRecfg)
{
   uint8_t ueId, lcIdx, ret = ROK;
   SchCellCb    *cellCb = NULLP;
   SchUeCb      *ueCb = NULLP;
   SchUeRecfgRsp  recfgRsp;
   Inst         inst = pst->dstInst - SCH_INST_START;
   memset(&recfgRsp, 0, sizeof(SchUeRecfgRsp));

   if(!ueRecfg)
   {
      DU_LOG("\nERROR  -->  SCH : Modifying Ue Config request failed at SchModUeConfigReq()");
      return RFAILED;
   }
   DU_LOG("\nDEBUG  -->  SCH : Modifying Ue Config Request for CRNTI[%d]", ueRecfg->crnti);
   cellCb = getSchCellCb(pst->event, inst, ueRecfg->cellId);

   if(cellCb == NULLP)
   {
      SchSendUeRecfgRspToMac(ueRecfg, inst, RSP_NOK, &recfgRsp);
      return RFAILED;
   }

   /* Search if UE already configured */
   GET_UE_ID(ueRecfg->crnti, ueId);
   ueCb = &cellCb->ueCb[ueId -1];

   if(!ueCb)
   {
      DU_LOG("\nERROR  -->  SCH : SchUeCb not found at SchModUeConfigReq() ");
      SchSendUeRecfgRspToMac(ueRecfg, inst, RSP_NOK, &recfgRsp);
      return RFAILED;
   }
   if((ueCb->crnti == ueRecfg->crnti) && (ueCb->state == SCH_UE_STATE_ACTIVE))
   {
      /* Found the UeCb to Reconfig */
      ret = fillSchUeCbFrmRecfgReq(inst, ueCb, ueRecfg);
      if(fillUeCoresetAndSsInfo(ueCb) == RFAILED)
      {
         DU_LOG("\nERROR  -->  SCH : Memory Allocation Failed");
         return RFAILED;
      }
      if(ret == ROK)
      {
         ueCb->cellCb = cellCb;
         ueCb->srRcvd = false;
         ueCb->bsrRcvd = false;
         for(lcIdx=0; lcIdx<MAX_NUM_LOGICAL_CHANNEL_GROUPS; lcIdx++)
            ueCb->bsrInfo[lcIdx].dataVol = 0;

         SchSendUeRecfgRspToMac(ueRecfg, inst, RSP_OK, &recfgRsp);
      }
   }
   return ret;
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
   uint8_t timeDomRsrcIdx = 0, ueLcIdx = 0, slotIdx =0, cRSetIdx = 0;
   SchPucchCfg *pucchCfg = NULLP;
   SchPdschConfig *pdschCfg = NULLP;

   if(ueCb)
   {
      if(ueCb->hqDlmap)
      {
         for (slotIdx = 0; slotIdx<ueCb->cellCb->numSlots; slotIdx++)
         {
            if(ueCb->hqDlmap[slotIdx])
            {
               cmLListDeleteLList(&ueCb->hqDlmap[slotIdx]->hqList);
               SCH_FREE(ueCb->hqDlmap[slotIdx], sizeof(SchHqDlMap));
            }
         }
         SCH_FREE(ueCb->hqDlmap, sizeof(SchHqDlMap*)*(ueCb->cellCb->numSlots));
      }

      if(ueCb->hqUlmap)
      {
         for (slotIdx = 0; slotIdx<ueCb->cellCb->numSlots; slotIdx++)
         {
            if(ueCb->hqUlmap[slotIdx])
            {
               cmLListDeleteLList(&ueCb->hqUlmap[slotIdx]->hqList);
               SCH_FREE(ueCb->hqUlmap[slotIdx], sizeof(SchHqUlMap));
            }
         }
         SCH_FREE(ueCb->hqUlmap, sizeof(SchHqUlMap*)*(ueCb->cellCb->numSlots));
      }

      schDlHqEntDelete(ueCb);
      schUlHqEntDelete(ueCb);

      SCH_FREE(ueCb->ueCfg.ambrCfg, sizeof(SchAmbrCfg));
      if(ueCb->ueCfg.spCellCfgPres)
      {
         if(ueCb->ueCfg.spCellCfg.servCellRecfg.initDlBwp.pdschCfgPres == true)
         {
            pdschCfg = &ueCb->ueCfg.spCellCfg.servCellRecfg.initDlBwp.pdschCfg;
            for(timeDomRsrcIdx = 0; timeDomRsrcIdx < pdschCfg->numTimeDomRsrcAlloc; timeDomRsrcIdx++)
               SCH_FREE(pdschCfg->timeDomRsrcAllociList[timeDomRsrcIdx].k0, sizeof(uint8_t));
         }

         if(ueCb->ueCfg.spCellCfg.servCellRecfg.initUlBwp.pucchCfgPres == true)
         {
            pucchCfg = &ueCb->ueCfg.spCellCfg.servCellRecfg.initUlBwp.pucchCfg;
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
         SCH_FREE(ueCb->ueCfg.spCellCfg.servCellRecfg.bwpInactivityTmr, sizeof(uint8_t));
         deleteSchPdschServCellCfg(&ueCb->ueCfg.spCellCfg.servCellRecfg.pdschServCellCfg);
      }
      /*Need to Free the memory allocated for S-NSSAI*/
      for(ueLcIdx = 0; ueLcIdx < MAX_NUM_LC; ueLcIdx++)
      {
         SCH_FREE(ueCb->ulInfo.ulLcCtxt[ueLcIdx].snssai, sizeof(Snssai));
         SCH_FREE(ueCb->dlInfo.dlLcCtxt[ueLcIdx].snssai, sizeof(Snssai));
      }
#ifdef NR_DRX
      if(ueCb->ueDrxInfoPres)
      {
         /* Removing all the calculated drx configuration timer list */ 
         schDeleteUeDrxInfo(ueCb->cellCb, ueCb);
         ueCb->ueDrxInfoPres = false;
      }
#endif
 
      for(cRSetIdx=0; cRSetIdx < MAX_NUM_CRSET; cRSetIdx++)
      {
         SCH_FREE(ueCb->pdcchInfo[cRSetIdx].y, (sizeof(uint32_t) * ueCb->cellCb->numSlots));
      }
      memset(ueCb, 0, sizeof(SchUeCb));
   }
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
*              CauseOfResult  cause
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
void SchSendUeDeleteRspToMac(Inst inst, SchUeDelete  *ueDelete, SchMacRsp result, CauseOfResult  cause)
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
    MacMessageRouter(&rspPst, (void *)&delRsp);
}

/*******************************************************************
*
* @brief Function for Ue Delete request from MAC to SCH
*
* @details
*
*    Function : SchProcUeDeleteReq 
*
*    Functionality: Function for Ue Delete request from MAC to SCH
*
* @params[in] Pst *pst, SchUeDelete  *ueDelete
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t SchProcUeDeleteReq(Pst *pst, SchUeDelete  *ueDelete)
{
    uint8_t idx=0, ueId=0, ret=ROK;
    CauseOfResult  cause;
    SchCellCb *cellCb = NULLP;
    Inst         inst = pst->dstInst - SCH_INST_START;
   
    if(!ueDelete)
    {
       DU_LOG("\nERROR  -->  SCH : SchProcUeDeleteReq(): Ue Delete request failed");
       ret = RFAILED;
    }
    DU_LOG("\nDEBUG  -->  SCH : Ue Delete request received for crnti[%d]", ueDelete->crnti);
    
    cellCb = schCb[inst].cells[idx];

    if(cellCb->cellId != ueDelete->cellId)
    {
       DU_LOG("\nERROR  -->  SCH : SchProcUeDeleteReq(): cell Id is not available");
       cause =  CELLID_INVALID;
    }
    else
    {
       GET_UE_ID(ueDelete->crnti, ueId);
       if(( cellCb->ueCb[ueId-1].crnti == ueDelete->crnti) && ( cellCb->ueCb[ueId-1].state == SCH_UE_STATE_ACTIVE))
       {
          /* Remove UE from ueToBeScheduled list */
          cellCb->api->SchUeDeleteReq(&cellCb->ueCb[ueId-1]);
          deleteSchUeCb(&cellCb->ueCb[ueId-1]);
          cellCb->numActvUe--;
          cause = SUCCESSFUL;
       }
       else
       {
          DU_LOG("\nERROR  -->  SCH : SchProcUeDeleteReq(): SchUeCb not found");
          cause =  UEID_INVALID;
       }
    }
    
    if(cause == SUCCESSFUL)
    {
       SchSendUeDeleteRspToMac(inst, ueDelete, RSP_OK, cause);
    }
    else
    {
       SchSendUeDeleteRspToMac(inst, ueDelete, RSP_NOK, cause);
       ret = RFAILED;
    }
    return ret;
}

/*******************************************************************
 *
 * @brief Function updates DL HARQ Feedback
 *
 * @details
 *
 *    Function : schUpdateHarqFdbk
 *
 *    Functionality: Function updates DL HARQ feedback
 *
 * @params[in] SchUeCb *ueCb, UE cb struct pointer
 * @params[in] uint8_t numHarq, number of HARQ processes in feedback 
 * @params[in] uint8_t *harqPayload, harq feedback payload received
 * @params[in] SlotTimingInfo *slotInd, slot timing information
 * @return void
 *
 * ****************************************************************/
void schUpdateHarqFdbk(SchUeCb *ueCb, uint8_t numHarq, uint8_t *harqPayload, SlotTimingInfo *slotInd)
{
   SchDlHqProcCb *hqP;
   SchHqDlMap *hqDlMap;
   CmLList  *node;
   uint8_t fdbkPos = 0;

   hqDlMap = ueCb->hqDlmap[slotInd->slot];

   if (ueCb->cellCb->raCb[ueCb->ueId-1].raState == SCH_RA_STATE_MSG2_HANDLE)
   {
      return;
   }
   if (ueCb->cellCb->raCb[ueCb->ueId-1].raState != SCH_RA_STATE_MSG4_PENDING)
   {
      node = hqDlMap->hqList.first;
      while(node)
      {
         hqP = (SchDlHqProcCb*)node->node;
         node = node->next;
         cmLListDelFrm(&hqDlMap->hqList, &hqP->ulSlotLnk);
         /* 
            Decode harq feedback if needed post FAPI message decoding also or check how to decode this FAPI msg.
            case 1 semi static harq Ack/Nack codebook //Supported
            case 2 dynamic harq ACK/NACK codebook //Not supported
         */
         schDlHqFeedbackUpdate(hqP, harqPayload[fdbkPos++], HQ_TB_ACKED);//Marking 2nd TB as ACKED for now as only one TB to be used
      }
   }
   else
   {
      node = hqDlMap->hqList.first;
      hqP = (SchDlHqProcCb*)node->node;
      cmLListDelFrm(&hqDlMap->hqList, &hqP->ulSlotLnk);
      schMsg4FeedbackUpdate(hqP, harqPayload[fdbkPos++]);
   }
}
/**********************************************************************
  End of file
 **********************************************************************/
