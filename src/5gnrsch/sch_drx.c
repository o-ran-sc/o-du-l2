
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
#ifdef NR_DRX
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
#include "sch_drx.h"

/**
 * @brief set the final Ue drx status 
 *
 * @details
 *
 *     Function : setDrxUeStatusForDlandUl
 *      
 *    set the final Ue drx status to active or inactive 
 *           
 *  @param[in] SchDrxUeCb *drxUeCb 
 *  @return  
 *      -#void 
 **/

void setDrxUeStatusForDlandUl(SchDrxUeCb *drxUeCb)
{
    /* Setting the Dl Ue status */
    if(drxUeCb->drxDlUeActiveMask || drxUeCb->drxDlUeActiveMaskForHarq)
    {
       drxUeCb->drxDlUeActiveStatus = true;
    }
    else
    {
       drxUeCb->drxDlUeActiveStatus = false;
    }
    /* Setting the Ul Ue status */
    if(drxUeCb->drxUlUeActiveMask || drxUeCb->drxUlUeActiveMaskForHarq)
    {
       drxUeCb->drxUlUeActiveStatus = true;
    }
    else
    {
       drxUeCb->drxUlUeActiveStatus = false;
    }
}

/**
 * @brief intialize the SchDrxHarqCb structre 
 *
 * @details
 *
 *     Function :schInitDrxHarqCb 
 *      
 *     intialize the SchDrxHarqCb structre 
 *           
 *  @param[in] SchDrxHarqCb *hqCb 
 *  @return  
 *      -# ROK
 *      -# RFAILED
 **/

void schInitDrxHarqCb(SchDrxHarqCb *hqDrxCb)
{
   memset(hqDrxCb, 0, sizeof(SchDrxHarqCb));
   hqDrxCb->retxStrtIndex    = SCH_DRX_INVALID_INDEX;
   hqDrxCb->rttExpIndex         = SCH_DRX_INVALID_INDEX;
   hqDrxCb->retxExpIndex        = SCH_DRX_INVALID_INDEX;
}

/**
 * @brief intialize the SchDrxUeCb structre 
 *
 * @details
 *
 *     Function : schInitDrxUeCb 
 *      
 *     intialize the SchDrxUeCb structre 
 *           
 *  @param[in]  SchUeCb *ueCb 
 *  @return  
 *      -# ROK
 *      -# RFAILED
 **/

void schInitDrxUeCb(SchUeCb *ueCb)
{
   memset(&ueCb->drxUeCb, 0, sizeof(SchDrxUeCb));
   ueCb->drxUeCb.onDurationStartIndex  = SCH_DRX_INVALID_INDEX;
   ueCb->drxUeCb.onDurationExpiryIndex = SCH_DRX_INVALID_INDEX;
   ueCb->drxUeCb.inActvExpiryIndex     = SCH_DRX_INVALID_INDEX;
   ueCb->drxUeCb.shortCycleExpiryIndex = SCH_DRX_INVALID_INDEX;
   ueCb->drxUeCb.onDurationStartDistance = SCH_DRX_INVALID_DISTANCE;
   ueCb->drxUeCb.onDurationExpiryDistance = SCH_DRX_INVALID_DISTANCE;
   ueCb->drxUeCb.inActiveTmrExpiryDistance = SCH_DRX_INVALID_DISTANCE;
}

/**
 * @brief delete Dl harq drx timers and information 
 *
 * @details
 *
 *     Function : schDeleteDlHarqDrxTimer
 *      
 *     delete Dl harq drx timers and information 
 *           
 *  @param[in] SchCellCb  *cell, SchUeCb *ueCb 
 *  @return  
 *      -# ROK
 *      -# RFAILED
 **/

void schDeleteDlHarqDrxTimer(SchCellCb  *cell, SchDlHqEnt *dlHqEnt)
{
   uint8_t idx, numHqPrcs;
   SchDlHqProcCb *dlProc;

   numHqPrcs = dlHqEnt->numHqPrcs;
   for(idx =0; idx<numHqPrcs; idx++)
   {
      dlProc = &dlHqEnt->procs[idx];

      if(dlProc->dlDrxHarqCb.retxStrtIndex != SCH_DRX_INVALID_INDEX)
      {
         cmLListDelFrm(&cell->drxCb[dlProc->dlDrxHarqCb.retxStrtIndex].dlRetransTmrStartList, dlProc->dlDrxHarqCb.retxStrtNode);
         SCH_FREE(dlProc->dlDrxHarqCb.retxStrtNode, sizeof(CmLList));
      }

      if(dlProc->dlDrxHarqCb.rttExpIndex != SCH_DRX_INVALID_INDEX)
      {
         cmLListDelFrm(&cell->drxCb[dlProc->dlDrxHarqCb.rttExpIndex ].dlHarqRttExpiryList, dlProc->dlDrxHarqCb.rttExpNode);
         SCH_FREE(dlProc->dlDrxHarqCb.rttExpNode, sizeof(CmLList));
      }

      if(dlProc->dlDrxHarqCb.retxExpIndex != SCH_DRX_INVALID_INDEX)
      {
         cmLListDelFrm(&cell->drxCb[dlProc->dlDrxHarqCb.retxExpIndex].dlRetransExpiryList, dlProc->dlDrxHarqCb.retxExpNode);
         SCH_FREE(dlProc->dlDrxHarqCb.retxExpNode, sizeof(CmLList));
      }
      schInitDrxHarqCb(&dlProc->dlDrxHarqCb);
   }
}

/**
 * @brief delete UL harq drx timers and information 
 *
 * @details
 *
 *     Function : schDeleteUlHarqDrxTimer
 *      
 *     delete Ul harq drx timers and information 
 *           
 *  @param[in] SchCellCb  *cell, SchUeCb *ueCb 
 *  @return  
 *      -# ROK
 *      -# RFAILED
**/

void schDeleteUlHarqDrxTimer(SchCellCb  *cell, SchUlHqEnt *ulHqEnt)
{
   uint8_t idx, numHqPrcs;
   SchUlHqProcCb *ulProc;

   numHqPrcs = ulHqEnt->numHqPrcs;
   for(idx =0; idx<numHqPrcs; idx++)
   {
      ulProc = &ulHqEnt->procs[idx];

      if(ulProc->ulDrxHarqCb.retxStrtIndex != SCH_DRX_INVALID_INDEX)
      {
         cmLListDelFrm(&cell->drxCb[ulProc->ulDrxHarqCb.retxStrtIndex].ulRetransTmrStartList, ulProc->ulDrxHarqCb.retxStrtNode);
         SCH_FREE(ulProc->ulDrxHarqCb.retxStrtNode, sizeof(CmLList));
      }

      if(ulProc->ulDrxHarqCb.rttExpIndex != SCH_DRX_INVALID_INDEX)
      {
         cmLListDelFrm(&cell->drxCb[ulProc->ulDrxHarqCb.rttExpIndex ].ulHarqRttExpiryList, ulProc->ulDrxHarqCb.rttExpNode);
         SCH_FREE(ulProc->ulDrxHarqCb.rttExpNode, sizeof(CmLList));
      }

      if(ulProc->ulDrxHarqCb.retxExpIndex != SCH_DRX_INVALID_INDEX)
      {
         cmLListDelFrm(&cell->drxCb[ulProc->ulDrxHarqCb.retxExpIndex].ulRetransExpiryList, ulProc->ulDrxHarqCb.retxExpNode);
         SCH_FREE(ulProc->ulDrxHarqCb.retxExpNode, sizeof(CmLList));
      }
      schInitDrxHarqCb(&ulProc->ulDrxHarqCb);
   }
}

/**
 * @brief delete UE drx timers and information 
 *
 * @details
 *
 *     Function : schDeleteUeDrxInfo
 *      
 *     delete UE drx timers and information 
 *           
 *  @param[in] SchCellCb  *cell, SchUeCb *ueCb 
 *  @return  void
 *
 **/

void schDeleteUeDrxInfo(SchCellCb  *cell, SchUeCb *ueCb)
{
   SchDrxUeCb *drxUeCb;

   if(ueCb->ueDrxInfoPres == true)
   {
      drxUeCb = &ueCb->drxUeCb;
      
      /* delete on duration start timer from ueCb */
      if(drxUeCb->onDurationStartIndex != SCH_DRX_INVALID_INDEX)
      {
         cmLListDelFrm(&cell->drxCb[drxUeCb->onDurationStartIndex].onDurationStartList, drxUeCb->onDurationStartNodeInfo);
         SCH_FREE(drxUeCb->onDurationStartNodeInfo, sizeof(CmLList));
      }
      
      /* delete on duration expiry timer from ueCb */
      if(drxUeCb->onDurationExpiryIndex != SCH_DRX_INVALID_INDEX)
      {
         cmLListDelFrm(&cell->drxCb[drxUeCb->onDurationExpiryIndex].onDurationExpiryList, drxUeCb->onDurationExpiryNodeInfo);
         SCH_FREE(drxUeCb->onDurationExpiryNodeInfo, sizeof(CmLList));
      }
      
      /* delete inActv Expiry Index timer from ueCb */
      if(drxUeCb->inActvExpiryIndex != SCH_DRX_INVALID_INDEX)
      {
         cmLListDelFrm(&cell->drxCb[drxUeCb->inActvExpiryIndex].inActvTmrExpiryList, drxUeCb->inActvTimerExpiryNodeInfo);
         SCH_FREE(drxUeCb->inActvTimerExpiryNodeInfo, sizeof(CmLList));
      }
      
      /* delete short cycle expiry timer from ueCb */
      if(drxUeCb->shortCycleExpiryIndex != SCH_DRX_INVALID_INDEX)
      {
         cmLListDelFrm(&cell->drxCb[drxUeCb->shortCycleExpiryIndex].shortCycleExpiryList, drxUeCb->shortCycleTmrExpiryNodeInfo);
         SCH_FREE(drxUeCb->shortCycleTmrExpiryNodeInfo, sizeof(CmLList));
      }
      
      schDeleteDlHarqDrxTimer(cell, &ueCb->dlHqEnt);
      schDeleteUlHarqDrxTimer(cell, &ueCb->ulHqEnt);
      schInitDrxUeCb(ueCb);
   }
}

/**
 * @brief fill drxUeCb structure with the help of ue cfg/recfg information
 *
 * @details
 *
 *     Function : schFillDrxUeCb
 *      
 *      fill drxUeCb structure with the help of ue cfg/recfg information
 *           
 *  @param[in] SchDrxCfg drxCfg ->configuration received from ue cfg/recfg api
 *  	       SchDrxUeCb *drxUeCb -> structure that need to be fill
 *  @return  
 *      -# ROK
 *      -# RFAILED
 **/

void schFillDrxUeCb(uint8_t numerology, SchDrxCfg drxCfg, SchDrxUeCb *drxUeCb)
{
   if(drxCfg.drxOnDurationTimer.onDurationTimerValInMs) 
   {
      SCH_CNVRT_MS_TO_SLOT(drxUeCb->onDurationLen, drxCfg.drxOnDurationTimer.onDurationtimerValue.milliSeconds, numerology); 
   }
   else
   {
      SCH_CNVRT_MS_TO_SLOT(drxUeCb->onDurationLen, drxCfg.drxOnDurationTimer.onDurationtimerValue.subMilliSeconds, numerology);
      drxUeCb->onDurationLen = drxUeCb->onDurationLen >> 5;
   }   
   SCH_CNVRT_MS_TO_SLOT(drxUeCb->inActvTimerLen, drxCfg.drxInactivityTimer, numerology);
   SCH_CNVRT_SYMBL_TO_SLOT(drxUeCb->harqRttDlTimerLen, drxCfg.drxHarqRttTimerDl);
   SCH_CNVRT_SYMBL_TO_SLOT(drxUeCb->harqRttUlTimerLen, drxCfg.drxHarqRttTimerUl);
   drxUeCb->retransDlTimerLen = drxCfg.drxRetransmissionTimerDl;
   drxUeCb->retransUlTimerLen = drxCfg.drxRetransmissionTimerUl;
   SCH_CNVRT_MS_TO_SLOT(drxUeCb->longCycleLen, drxCfg.drxLongCycleStartOffset.drxLongCycleStartOffsetChoice, numerology);
   SCH_CNVRT_MS_TO_SLOT(drxUeCb->drxStartOffset, drxCfg.drxLongCycleStartOffset.drxLongCycleStartOffsetVal, numerology);
   if(drxCfg.shortDrxPres)
   {
      drxUeCb->shortCyclePresent = true;		   
      SCH_CNVRT_MS_TO_SLOT(drxUeCb->shortCycleLen, drxCfg.shortDrx.drxShortCycle, numerology);   
      drxUeCb->shortCycleTmrLen = drxUeCb->shortCycleLen*drxCfg.shortDrx.drxShortCycleTimer;	
   }
   drxUeCb->longCycleToBeUsed = true;
   SCH_CNVRT_MS_TO_SLOT(drxUeCb->drxSlotOffset, drxCfg.drxSlotOffset, numerology);  
   drxUeCb->drxSlotOffset = drxUeCb->drxSlotOffset>>5; 
}

/**
 * @brief Add new entry into the drx timer list
 *
 * @details
 *
 *     Function : schAddDrxTimerIntoList 
 *      
 *      Add new entry into the drx timer list
 *           
 *  @param[in] CmLListCp *drxTimerList -> List in which new entery have to add
 *  	       void * ueInfo -> ue information which is need to the added into list
 *  @return  
 *      -# ROK
 *      -# RFAILED
 **/

uint8_t schAddDrxTimerIntoList(CmLListCp *drxTimerList,void * nodeInfo, CmLList **drxNodeInfo)
{
   CmLList  *currentNodeInfo = NULLP;

   SCH_ALLOC(currentNodeInfo, sizeof(CmLList));
   if(!currentNodeInfo)
   {
      DU_LOG("\nERROR  --> SCH : schAddDrxTimerIntoList() : Memory allocation failed");
      return RFAILED;
   }

   currentNodeInfo->node = (PTR)nodeInfo;
   
   cmLListAdd2Tail(drxTimerList, currentNodeInfo);
   (*drxNodeInfo) = currentNodeInfo;
   DU_LOG("\nINFO --> SCH : Drx node added into the list");
   return ROK;
}

/**
 * @brief Add new entry into the drx harq timer list
 *
 * @details
 *
 *     Function : schAddDrxNodeIntoHarqTimerList 
 *      
 *      Add new entry into the drx harq timer list
 *           
 *  @param[in] CmLListCp *drxTimerList -> List in which new entery have to add
 *  	       void * nodeInfo-> ue information which is need to the added into list
 *  	        CmLList **drxNodeInfo -> pointer to the node
 *  @return  
 *      -# ROK
 *      -# RFAILED
 **/

uint8_t schAddDrxNodeIntoHarqTimerList(CmLListCp *drxTimerList,void * nodeInfo, CmLList **drxNodeInfo)
{
   CmLList  *currentNodeInfo = NULLP;

   SCH_ALLOC(currentNodeInfo, sizeof(CmLList));
   if(!currentNodeInfo)
   {
      DU_LOG("\nERROR  --> SCH : schAddDrxTimerIntoList() : Memory allocation failed");
      return RFAILED;
   }

   currentNodeInfo->node = (PTR)nodeInfo;
   
   cmLListAdd2Tail(drxTimerList, currentNodeInfo);
   (*drxNodeInfo) = currentNodeInfo;
   DU_LOG("\nINFO --> SCH : Drx node added into the list");

   return ROK;
}

/**
 * @brief This function is used to find the next onduration start timing
 *
 * @details
 *
 *     Function : findNextOndurationOccurance 
 *      
 *      This function is used to find the next onduration start timing
 *           
 *  @param[in] SchCellCb *cell, SchDrxUeCb *ueDrxCb, SlotTimingInfo  *nxtOnDur,
 *  uint8_t delta
 *  	       
 *  @return  
 *      -# void
 **/

void findNextOndurationOccurance(SchCellCb *cell, SchDrxUeCb *ueDrxCb, SlotTimingInfo  *nxtOnDur, uint8_t delta)
{
   uint16_t   tmpDistance, numOfCycles;
   uint32_t   curTime, cycleLen, nxtDist;
   SlotTimingInfo  tmpTime;
   
   if (ueDrxCb->longCycleToBeUsed == true)
   {
      cycleLen = ueDrxCb->longCycleLen;
   }
   else
   {
      cycleLen = ueDrxCb->shortCycleLen;
   }

   /* Add delta to current time */
   ADD_DELTA_TO_TIME(cell->slotInfo, tmpTime, delta, cell->numSlots);
   
   /* Convert tmpTime to number of slots */
   curTime = ((tmpTime.sfn * cell->numSlots) + tmpTime.slot);
   
   /* as per 38.321,  if the criterion below is satisfied, then that sfn and
    * slot are the correct ones for the on-duration timer.
    * if the Short DRX Cycle is used, and [(SFN × 10) + subframe number] modulo
    * (drx-ShortCycle) = (drxStartOffset) modulo (drx-ShortCycle); or
    * if the Long DRX Cycle is used, and [(SFN × 10) + subframe number] modulo
    * (drx-LongCycle) = drxStartOffset  */
   if ( curTime <= ueDrxCb->drxStartOffset)
   {
      /* offset is the nextOnDur */
      nxtDist = ((((ueDrxCb->drxStartOffset / cell->numSlots)) & (MAX_SFN - 1)) * cell->numSlots) + (ueDrxCb->drxStartOffset % cell->numSlots);
   }
   else
   {
      tmpDistance = curTime - ueDrxCb->drxStartOffset;
      numOfCycles = tmpDistance / cycleLen;

      if (0 == (tmpDistance % cycleLen))
      {
         /* Perfect match pick up the current time */
         nxtDist = ((((curTime / cell->numSlots)) & (MAX_SFN - 1)) * cell->numSlots) + (curTime % cell->numSlots);
      }
      else
      {
         nxtDist  = ueDrxCb->drxStartOffset + (numOfCycles + 1) * cycleLen;
      }
   }

   /* If slot offset is non-zero then Add slot offset to the calculated onDur
    * distance */
   if(ueDrxCb->drxSlotOffset)
   {
      nxtDist = nxtDist + ueDrxCb->drxSlotOffset;
   }
   /*If next On Duration is less than DL DELTA ahead, we will miss it and
    * hence need to move to the On-Duration after that.*/
   if((nxtDist - (curTime - delta)) <= SCH_DRX_MAX_DELTA)
   {
      nxtDist = nxtDist + cycleLen;
   }
   
   nxtOnDur->sfn = ((nxtDist / cell->numSlots) & (MAX_SFN - 1));
   nxtOnDur->slot = (nxtDist % cell->numSlots);
}

/**
 * @brief Add entry into  the on duration list and short cycle list
 *
 * @details
 *
 *     Function : schDrxUeReCfgTimer 
 *      
 *      This function is used to Add entry into  the on duration list and short
 *      cycle list
 *           
 *  @param[in] SchCellCb *cell,   SchUeCb *ueCb
 *  uint8_t delta
 *  	       
 *  @return void 
 **/

void schDrxUeReCfgTimer(SchCellCb *cell, SchUeCb *ueCb)
{
   uint8_t  currentSlotIndx;
   uint32_t onDurTime, onDurExpSlotTime, currentSlotTime;
   uint32_t cycleLen;
   SlotTimingInfo onDurationOccurance;
   
   if(ueCb->drxUeCb.shortCyclePresent == false)   
   {
      /* if short cycle configuration are not recived as a part of UE Recfg then if there is any entry present in short cycle timer list 
       * remove the entry from the list */ 
      if(ueCb->drxUeCb.shortCycleExpiryIndex != SCH_DRX_INVALID_INDEX)
      {
         cmLListDelFrm(&cell->drxCb[ueCb->drxUeCb.shortCycleExpiryIndex ].shortCycleExpiryList, ueCb->drxUeCb.shortCycleTmrExpiryNodeInfo);
         SCH_FREE(ueCb->drxUeCb.shortCycleTmrExpiryNodeInfo, sizeof(CmLList));
         ueCb->drxUeCb.shortCycleExpiryIndex = SCH_DRX_INVALID_INDEX;
         ueCb->drxUeCb.shortCycleDistance = SCH_DRX_INVALID_DISTANCE;
      }
   }
   /* If there is any entry present in on duration start list then remove the
    * entry from the list and recaluate the nect onduration cucurance */
   if(ueCb->drxUeCb.onDurationStartIndex != SCH_DRX_INVALID_INDEX)
   {
      cmLListDelFrm(&cell->drxCb[ueCb->drxUeCb.onDurationStartIndex].onDurationStartList, ueCb->drxUeCb.onDurationStartNodeInfo);
      SCH_FREE(ueCb->drxUeCb.onDurationStartNodeInfo, sizeof(CmLList));
      ueCb->drxUeCb.onDurationStartIndex= SCH_DRX_INVALID_INDEX; 
      ueCb->drxUeCb.onDurationStartDistance= SCH_DRX_INVALID_DISTANCE; 
      
   }

   findNextOndurationOccurance(cell,  &ueCb->drxUeCb, &onDurationOccurance, 0);
   onDurTime = onDurationOccurance.sfn*cell->numSlots+onDurationOccurance.slot;
   
   /* If Onduration timer of old configuration is already running then next onduration 
    * starts once it expires*/
   if((ueCb->drxUeCb.onDurationExpiryDistance != SCH_DRX_INVALID_DISTANCE) && (ueCb->drxUeCb.onDurationExpiryIndex != SCH_DRX_INVALID_INDEX))
   {
       currentSlotTime = cell->slotInfo.sfn * cell->numSlots + cell->slotInfo.slot;
       currentSlotIndx = (currentSlotTime + PHY_DELTA_DL + SCHED_DELTA)%MAX_DRX_SIZE;
       if(currentSlotIndx >= ueCb->drxUeCb.onDurationExpiryIndex )
       {
         onDurExpSlotTime = currentSlotTime + ((ueCb->drxUeCb.onDurationExpiryDistance +1) * MAX_DRX_SIZE) +\
         (ueCb->drxUeCb.onDurationExpiryIndex - currentSlotIndx + PHY_DELTA_DL + SCHED_DELTA);
       }
       else
       {
         
         onDurExpSlotTime = currentSlotTime + ((ueCb->drxUeCb.onDurationExpiryDistance) * MAX_DRX_SIZE) +\
         (ueCb->drxUeCb.onDurationExpiryIndex - currentSlotIndx + PHY_DELTA_DL + SCHED_DELTA);
       }
       if(onDurTime <= onDurExpSlotTime)
       {
          if(ueCb->drxUeCb.longCycleToBeUsed == true)
             cycleLen = ueCb->drxUeCb.longCycleLen;
          else
             cycleLen = ueCb->drxUeCb.shortCycleLen;

          onDurTime = onDurTime + ((onDurExpSlotTime - onDurTime)/cycleLen + 1) * cycleLen;
       }
   }
   SCH_CALCULATE_TIMER_INDEX(onDurTime, ueCb->drxUeCb.onDurationStartIndex);
   ueCb->drxUeCb.onDurationStartDistance = SCH_CALC_SLOT_DIFF(onDurationOccurance, cell->slotInfo, cell->numSlots)/MAX_DRX_SIZE;
   schAddDrxTimerIntoList(&cell->drxCb[ueCb->drxUeCb.onDurationStartIndex].onDurationStartList, ueCb, &ueCb->drxUeCb.onDurationStartNodeInfo);
}

/**
 * @brief Add entry into  the on duration list 
 *
 * @details
 *
 *     Function : schAddUeInOndurationList
 *      
 *      This function is used to Add entry into  the on duration list 
 *           
 *  @param[in] SchCellCb *cell, SchDrxUeCb *ueDrxCb, SlotTimingInfo  *nxtOnDur,
 *  uint8_t delta
 *  	       
 *  @return  
 *      -# void
 **/

void schAddUeInOndurationList(SchCellCb *cell, SchUeCb *ueCb, uint8_t delta)
{
   uint32_t onDurTime;
   SlotTimingInfo onDurationOccurance;

   if(ueCb->ueDrxInfoPres)
   {
      findNextOndurationOccurance(cell,  &ueCb->drxUeCb, &onDurationOccurance, delta);
      onDurTime = onDurationOccurance.sfn*cell->numSlots+onDurationOccurance.slot;
      SCH_CALCULATE_TIMER_INDEX(onDurTime, ueCb->drxUeCb.onDurationStartIndex);
      ueCb->drxUeCb.onDurationStartDistance = SCH_CALC_SLOT_DIFF(onDurationOccurance, cell->slotInfo, cell->numSlots)/MAX_DRX_SIZE;
      schAddDrxTimerIntoList(&cell->drxCb[ueCb->drxUeCb.onDurationStartIndex].onDurationStartList, ueCb, &ueCb->drxUeCb.onDurationStartNodeInfo);

   }
}

/**
 * @brief Handling of On duration drx start timer
 *
 * @details
 *
 *     Function : schHdlDrxOnDurStrtTimerForDlDirection
 *
 *      Handling of On duration drx start timer
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schHdlDrxOnDurStrtTimerForDlDirection(SchCellCb  *cell, uint16_t currListIndx)
{
   uint16_t onDurationExpiry=0;
   CmLList  *drxCurrNode = NULLP;
   SchUeCb *ueCb = NULLP;

   drxCurrNode = cell->drxCb[currListIndx].onDurationStartList.first;
   if(drxCurrNode)
   {
      /* Handling of dl On duration drx start list */
      while(drxCurrNode)
      {
         ueCb = (SchUeCb*)drxCurrNode->node;
         drxCurrNode = drxCurrNode->next;
         
         ueCb->drxUeCb.onDurationStartDistance--;
         
         if(ueCb->drxUeCb.onDurationStartDistance != SCH_DRX_INVALID_DISTANCE)
         {
            continue;
         }
         else
         {
            ueCb->drxUeCb.drxDlUeActiveMask |= UE_ACTIVE_FOR_ONDURATION;
            
            /* set the Ue status as active or inactive*/
            setDrxUeStatusForDlandUl(&ueCb->drxUeCb);
            
            /* If there is any entery present in onDurationExpiry list remove
             * the entery from the list and recalculate the
             * onDurationExpiry time and add it to list */
            if(ueCb->drxUeCb.onDurationExpiryIndex != SCH_DRX_INVALID_INDEX)
            {
               cmLListDelFrm(&cell->drxCb[ueCb->drxUeCb.onDurationExpiryIndex].onDurationExpiryList, ueCb->drxUeCb.onDurationExpiryNodeInfo);
               SCH_FREE(ueCb->drxUeCb.onDurationExpiryNodeInfo, sizeof(CmLList));
               ueCb->drxUeCb.onDurationExpiryIndex = SCH_DRX_INVALID_INDEX;
               ueCb->drxUeCb.onDurationExpiryDistance = SCH_DRX_INVALID_DISTANCE; 
            }

            /* onDurationExpiry  = (current slot + onduration length) % MAX_DRX_SIZE*/
            onDurationExpiry = (currListIndx + ueCb->drxUeCb.onDurationLen)%MAX_DRX_SIZE;
            ueCb->drxUeCb.onDurationExpiryDistance =  (ueCb->drxUeCb.onDurationLen)/MAX_DRX_SIZE;
            schAddDrxTimerIntoList(&cell->drxCb[onDurationExpiry].onDurationExpiryList, ueCb, &ueCb->drxUeCb.onDurationExpiryNodeInfo);
            ueCb->drxUeCb.onDurationExpiryIndex = onDurationExpiry;

         }
      }
   }
}

/**
 * @brief Handling of On duration drx start timer
 *
 * @details
 *
 *     Function : schHdlDrxOnDurStrtTimerForUlDirection
 *
 *      Handling of On duration drx start timer
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schHdlDrxOnDurStrtTimerForUlDirection(SchCellCb  *cell, uint16_t currListIndx)
{
   uint16_t onDurTime=0;
   CmLList  *drxCurrNode = NULLP;
   SchUeCb *ueCb = NULLP;

   drxCurrNode = cell->drxCb[currListIndx].onDurationStartList.first;
   if(drxCurrNode)
   {
      /* Handling of dl On duration drx start list */
      while(drxCurrNode)
      {
         ueCb = (SchUeCb*)drxCurrNode->node;
         drxCurrNode = drxCurrNode->next;
         
         if(ueCb->drxUeCb.onDurationStartDistance != SCH_DRX_INVALID_DISTANCE)
         {
            continue;
         }
         ueCb->drxUeCb.drxUlUeActiveMask |= UE_ACTIVE_FOR_ONDURATION;
         /* set the final Ue status as active or inactive */
         setDrxUeStatusForDlandUl(&ueCb->drxUeCb);
         
         /* if there short cycle length is used as the cycle length for onduration calculation then based on the short cycle else long cycle is used for calculating next onduration */
         cmLListDelFrm(&cell->drxCb[currListIndx].onDurationStartList, ueCb->drxUeCb.onDurationStartNodeInfo);
         SCH_FREE(ueCb->drxUeCb.onDurationStartNodeInfo, sizeof(CmLList));
         ueCb->drxUeCb.onDurationStartIndex= SCH_DRX_INVALID_INDEX; 

         if(ueCb->drxUeCb.longCycleToBeUsed)
         {
            onDurTime = currListIndx +  ueCb->drxUeCb.longCycleLen;
         }
         else
         {
            onDurTime = currListIndx +  ueCb->drxUeCb.shortCycleLen;
         }
         SCH_CALCULATE_TIMER_INDEX(onDurTime, ueCb->drxUeCb.onDurationStartIndex);
         ueCb->drxUeCb.onDurationStartDistance = ueCb->drxUeCb.longCycleLen/MAX_DRX_SIZE;
         schAddDrxTimerIntoList(&cell->drxCb[ueCb->drxUeCb.onDurationStartIndex].onDurationStartList, ueCb, &ueCb->drxUeCb.onDurationStartNodeInfo);
      }
   }
}

/**
 * @brief Handling of On duration drx start timer
 *
 * @details
 *
 *     Function : schHdlDrxOnDurStrtTimer
 *
 *      Handling of On duration drx start timer
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schHdlDrxOnDurStrtTimer(SchCellCb  *cell)
{
   uint16_t dlIndx = 0, ulIndx=0;
   SlotTimingInfo dlSlotInfo, ulSlotInfo;

   ADD_DELTA_TO_TIME(cell->slotInfo, dlSlotInfo,  PHY_DELTA_DL + SCHED_DELTA, cell->numSlots);
   ADD_DELTA_TO_TIME(cell->slotInfo, ulSlotInfo,  PHY_DELTA_UL + SCHED_DELTA, cell->numSlots);

   dlIndx = (dlSlotInfo.sfn*MAX_SLOTS+dlSlotInfo.slot)%MAX_DRX_SIZE;
   ulIndx = (ulSlotInfo.sfn*MAX_SLOTS+ulSlotInfo.slot)%MAX_DRX_SIZE;
   schHdlDrxOnDurStrtTimerForDlDirection(cell, dlIndx);
   schHdlDrxOnDurStrtTimerForUlDirection(cell, ulIndx);
}

/**
 * @brief Handling of the DRX in-active start timer
 *
 * @details
 *
 *     Function : 
 *
 *      Handling of DRX in-active start timer
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schHdlDrxInActvStrtTmr(SchCellCb  *cell,  SchUeCb *ueCb, uint8_t delta)
{
   uint16_t slotIndx = 0;
   SlotTimingInfo tmpSlotInfo;
   
   if(ueCb->drxUeCb.inActvTimerLen == 0)
   {
      return;
   }

   ADD_DELTA_TO_TIME(cell->slotInfo, tmpSlotInfo,  delta, cell->numSlots);
   slotIndx = (tmpSlotInfo.sfn*MAX_SLOTS+tmpSlotInfo.slot)%MAX_DRX_SIZE;

   /* if there is any entry present in the in-active exp list then remove the entry */
   if(ueCb->drxUeCb.inActvExpiryIndex != SCH_DRX_INVALID_INDEX)
   {
      cmLListDelFrm(&cell->drxCb[ueCb->drxUeCb.inActvExpiryIndex].inActvTmrExpiryList, ueCb->drxUeCb.inActvTimerExpiryNodeInfo);
      SCH_FREE(ueCb->drxUeCb.inActvTimerExpiryNodeInfo, sizeof(CmLList));
      ueCb->drxUeCb.inActvExpiryIndex= SCH_DRX_INVALID_INDEX;
      ueCb->drxUeCb.inActiveTmrExpiryDistance= SCH_DRX_INVALID_DISTANCE;
   }

   /* Adding the new entry in in-activity timer list */
   ueCb->drxUeCb.inActvExpiryIndex = (slotIndx + ueCb->drxUeCb.inActvTimerLen) % MAX_DRX_SIZE;
   ueCb->drxUeCb.inActiveTmrExpiryDistance = (ueCb->drxUeCb.inActvTimerLen) / MAX_DRX_SIZE;
   schAddDrxTimerIntoList(&cell->drxCb[ueCb->drxUeCb.inActvExpiryIndex].inActvTmrExpiryList, ueCb, &ueCb->drxUeCb.inActvTimerExpiryNodeInfo);

   /* Set the UE active for UL And Dl transfer */
   ueCb->drxUeCb.drxDlUeActiveMask |= UE_ACTIVE_FOR_INACTIVE_TIMER;
   ueCb->drxUeCb.drxUlUeActiveMask |= UE_ACTIVE_FOR_INACTIVE_TIMER;
   setDrxUeStatusForDlandUl(&ueCb->drxUeCb);
}

/**
 * @brief Handling of short cycle drx start timer
 *
 * @details
 *
 *     Function : schHdlDrxStartShortCycleTimer
 *
 *      Handling of short cycle drx start timer
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schHdlDrxStartShortCycleTimer(SchCellCb  *cell, SchUeCb *ueCb)
{
   ueCb->drxUeCb.longCycleToBeUsed = false;

   /* if there is any present in on-duration start list, remove the entry from on duration start list */
   if(ueCb->drxUeCb.onDurationStartIndex != SCH_DRX_INVALID_INDEX)
   {
      cmLListDelFrm(&cell->drxCb[ueCb->drxUeCb.onDurationStartIndex].onDurationStartList, ueCb->drxUeCb.onDurationStartNodeInfo);
      SCH_FREE(ueCb->drxUeCb.onDurationStartNodeInfo, sizeof(CmLList));
      ueCb->drxUeCb.onDurationStartIndex = SCH_DRX_INVALID_INDEX; 
      ueCb->drxUeCb.onDurationStartDistance = SCH_DRX_INVALID_DISTANCE; 
   } 

   /* recalculate the new index of on duration start based on short cycle */
   schAddUeInOndurationList(cell, ueCb, PHY_DELTA_DL + SCHED_DELTA);

   /* if any node is present in short cycle exp list then remove the node from list  */
   if(ueCb->drxUeCb.shortCycleExpiryIndex != SCH_DRX_INVALID_INDEX && ueCb->drxUeCb.shortCycleDistance != SCH_DRX_INVALID_DISTANCE)
   {
      cmLListDelFrm(&cell->drxCb[ueCb->drxUeCb.shortCycleExpiryIndex].shortCycleExpiryList, ueCb->drxUeCb.shortCycleTmrExpiryNodeInfo);
      SCH_FREE(ueCb->drxUeCb.shortCycleTmrExpiryNodeInfo, sizeof(CmLList));
      ueCb->drxUeCb.shortCycleExpiryIndex = SCH_DRX_INVALID_INDEX;
      ueCb->drxUeCb.shortCycleDistance = SCH_DRX_INVALID_DISTANCE;
   }

   /* recalculate the new index for shortCycleExpiryList */
   ueCb->drxUeCb.shortCycleExpiryIndex = (ueCb->drxUeCb.onDurationStartIndex + ueCb->drxUeCb.shortCycleTmrLen) % MAX_DRX_SIZE;
   ueCb->drxUeCb.shortCycleDistance = ueCb->drxUeCb.shortCycleTmrLen / MAX_DRX_SIZE;
   schAddDrxTimerIntoList(&cell->drxCb[ueCb->drxUeCb.shortCycleExpiryIndex].shortCycleExpiryList, ueCb, &ueCb->drxUeCb.shortCycleTmrExpiryNodeInfo);
}

/**
 * @brief Handling of the Dl harq DRX timers start for Dl scheduling
 *
 * @details
 *
 *     Function : schHdlDlHqRetxStrtTimerForDl
 *
 *      Handling of Dl harq DRX timers start for Dl scheduling
 *
 *  @param[in] SchCellCb *cell, uint16_t currIdx
 *  @return
 *     void
**/
 
void schHdlDlHqRetxStrtTimerForDl(SchCellCb *cell, uint16_t currIndx)
{
   uint32_t retxExpIndx;
   CmLList *currNode;
   SchDlHqProcCb *hqP;
   SchUeCb *ueCb;
   
   currNode = cell->drxCb[currIndx].dlRetransTmrStartList.first;
   
   while(currNode)
   {
      hqP  = (SchDlHqProcCb*)currNode->node;
      currNode = currNode->next;
      cell->api->SchAddUeToSchedule(hqP->hqEnt->cell, hqP->hqEnt->ue->ueId);
      ueCb = hqP->hqEnt->ue;
      
      /* calculate the retransmission exp index */
      retxExpIndx =  (currIndx + ueCb->drxUeCb.retransDlTimerLen)%MAX_DRX_SIZE;
      if(hqP->dlDrxHarqCb.retxExpIndex == SCH_DRX_INVALID_INDEX)
      {
         hqP->dlDrxHarqCb.retxExpIndex = retxExpIndx;
         schAddDrxNodeIntoHarqTimerList(&cell->drxCb[retxExpIndx].dlRetransExpiryList, hqP, &hqP->dlDrxHarqCb.retxExpNode);
      }
      
      /* Mark the UE active for downlink */
      ueCb->drxUeCb.drxDlUeActiveMaskForHarq |= (SCH_DRX_DL_HARQ_BITMASK << hqP->procId);
      setDrxUeStatusForDlandUl(&ueCb->drxUeCb);
   }
}

/**
 * @brief Handling of the Dl harq DRX timers start for Ul scheduling
 *
 * @details
 *
 *     Function : schHdlDlHqRetxStrtTimerForUl
 *
 *      Handling of Dl harq DRX timers start for Ul scheduling
 *
 *  @param[in] SchCellCb *cell, uint16_t currIdx
 *  @return
 *     void
 **/
 
void schHdlDlHqRetxStrtTimerForUl(SchCellCb *cell, uint16_t currIndx)
{
   CmLList *currNode;
   SchDlHqProcCb *hqP;
   SchUeCb *ueCb;

   currNode = cell->drxCb[currIndx].dlRetransTmrStartList.first;
   
   while(currNode)
   {
      hqP  = (SchDlHqProcCb*)currNode->node;
      currNode = currNode->next;
      ueCb = hqP->hqEnt->ue;
      
      /* Mark the UE active for uplink */
      ueCb->drxUeCb.drxUlUeActiveMaskForHarq |= (SCH_DRX_UL_HARQ_BITMASK << hqP->procId);
      setDrxUeStatusForDlandUl(&ueCb->drxUeCb);
      
      /* Delete the node */
      cmLListDelFrm(&cell->drxCb[currIndx].dlRetransTmrStartList, hqP->dlDrxHarqCb.retxStrtNode);
      SCH_FREE(hqP->dlDrxHarqCb.retxStrtNode, sizeof(CmLList));
      hqP->dlDrxHarqCb.retxStrtIndex = SCH_DRX_INVALID_INDEX;
   }
}

/**
 * @brief Handling of the Dl harq DRX timers start
 *
 * @details
 *
 *     Function : schHdlDrxDlHqRetxStrtTimer
 *
 *      Handling of Dl harq DRX timers start
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schHdlDrxDlHqRetxStrtTimer(SchCellCb  *cell)
{
   uint16_t dlIndx = 0, ulIndx=0;
   SlotTimingInfo dlSlotInfo, ulSlotInfo;

   ADD_DELTA_TO_TIME(cell->slotInfo, dlSlotInfo,  PHY_DELTA_DL + SCHED_DELTA, cell->numSlots);
   ADD_DELTA_TO_TIME(cell->slotInfo, ulSlotInfo,  PHY_DELTA_UL + SCHED_DELTA, cell->numSlots);

   dlIndx = (dlSlotInfo.sfn*MAX_SLOTS+dlSlotInfo.slot)%MAX_DRX_SIZE;
   ulIndx = (ulSlotInfo.sfn*MAX_SLOTS+ulSlotInfo.slot)%MAX_DRX_SIZE;

   schHdlDlHqRetxStrtTimerForDl(cell, dlIndx);
   schHdlDlHqRetxStrtTimerForUl(cell, ulIndx);
}

/**
 * @brief Handling of the  DRX timers start
 *
 * @details
 *
 *     Function : schHandleStartDrxTimer
 *
 *      Handling of DRX timers start
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schHandleStartDrxTimer(SchCellCb  *cell)
{
   /* Handling the onduration start timer */
   schHdlDrxOnDurStrtTimer(cell);
   schHdlDrxDlHqRetxStrtTimer(cell);
   schHdlDrxUlHqRetxStrtTimer(cell);
}

/**
 * @brief Handling of the Dl harq Rtt start DRX timers
 *
 * @details
 *
 *     Function : schDrxStrtDlHqRttTmr
 *
 *      Handling of the Dl harq Rtt start DRX timers
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schDrxStrtDlHqRttTmr(SchDlHqProcCb *hqP)
{
   uint16_t harqRttExpTimer=0;
   SchDrxUeCb *drxUeCb;
   
   drxUeCb = &hqP->hqEnt->ue->drxUeCb;
   
   harqRttExpTimer =  (hqP->pucchTime.sfn * MAX_SLOTS + hqP->pucchTime.slot + drxUeCb->harqRttDlTimerLen)%MAX_DRX_SIZE;
   hqP->dlDrxHarqCb.rttExpIndex = harqRttExpTimer;
   schAddDrxNodeIntoHarqTimerList(&hqP->hqEnt->cell->drxCb[harqRttExpTimer].dlHarqRttExpiryList, hqP, &hqP->dlDrxHarqCb.rttExpNode);
}

/**
 * @brief Handling of the Ul harq DRX timers start for Dl scheduling
 *
 * @details
 *
 *     Function : schHdlUlHqRetxStrtTimerForDl
 *
 *      Handling of Ul harq DRX timers start for Dl scheduling
 *
 *  @param[in] SchCellCb *cell, uint16_t currIdx
 *  @return
 *     void
**/
 
void schHdlUlHqRetxStrtTimerForDl(SchCellCb *cell, uint16_t currIndx)
{
   uint32_t retxExpIndx;
   CmLList *currNode;
   SchUlHqProcCb *hqP;
   SchUeCb *ueCb;
   
   currNode = cell->drxCb[currIndx].ulRetransTmrStartList.first;
   
   while(currNode)
   {
      hqP  = (SchUlHqProcCb*)currNode->node;
      currNode = currNode->next;
      cell->api->SchAddUeToSchedule(hqP->hqEnt->cell, hqP->hqEnt->ue->ueId);
      ueCb = hqP->hqEnt->ue;
      
      /* calculate the retransmission exp index */
      retxExpIndx =  (currIndx + ueCb->drxUeCb.retransUlTimerLen)%MAX_DRX_SIZE;
      if(hqP->ulDrxHarqCb.retxExpIndex == SCH_DRX_INVALID_INDEX)
      {
         hqP->ulDrxHarqCb.retxExpIndex = retxExpIndx;
         schAddDrxNodeIntoHarqTimerList(&cell->drxCb[retxExpIndx].ulRetransExpiryList, hqP, &hqP->ulDrxHarqCb.retxExpNode);
      }
      
      /* Mark the UE active for downlink */
      ueCb->drxUeCb.drxUlUeActiveMaskForHarq |= (SCH_DRX_UL_HARQ_BITMASK << hqP->procId);
      setDrxUeStatusForDlandUl(&ueCb->drxUeCb);
   }
}

/**
 * @brief Handling of the Ul harq DRX timers start for Ul scheduling
 *
 * @details
 *
 *     Function : schHdlUlHqRetxStrtTimerForUl
 *
 *      Handling of Ul harq DRX timers start for Ul scheduling
 *
 *  @param[in] SchCellCb *cell, uint16_t currIdx
 *  @return
 *     void
 **/
 
void schHdlUlHqRetxStrtTimerForUl(SchCellCb *cell, uint16_t currIndx)
{
   CmLList *currNode;
   SchUlHqProcCb *hqP;
   SchUeCb *ueCb;

   currNode = cell->drxCb[currIndx].ulRetransTmrStartList.first;
   
   while(currNode)
   {
      hqP  = (SchUlHqProcCb*)currNode->node;
      currNode = currNode->next;
      ueCb = hqP->hqEnt->ue;
      
      /* Mark the UE active for uplink */
      ueCb->drxUeCb.drxUlUeActiveMaskForHarq |= (SCH_DRX_UL_HARQ_BITMASK << hqP->procId);
      setDrxUeStatusForDlandUl(&ueCb->drxUeCb);
      
      /* Delete the node */
      cmLListDelFrm(&cell->drxCb[currIndx].ulRetransTmrStartList, hqP->ulDrxHarqCb.retxStrtNode);
      SCH_FREE(hqP->ulDrxHarqCb.retxStrtNode, sizeof(CmLList));
      hqP->ulDrxHarqCb.retxStrtIndex = SCH_DRX_INVALID_INDEX;
   }
}

/**
 * @brief Handling of the Ul harq DRX timers start
 *
 * @details
 *
 *     Function : schHdlDrxUlHqRetxStrtTimer
 *
 *      Handling of Ul harq DRX timers start
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schHdlDrxUlHqRetxStrtTimer(SchCellCb  *cell)
{
   uint16_t dlIndx = 0, ulIndx=0;
   SlotTimingInfo dlSlotInfo, ulSlotInfo;

   ADD_DELTA_TO_TIME(cell->slotInfo, dlSlotInfo,  PHY_DELTA_DL + SCHED_DELTA, cell->numSlots);
   ADD_DELTA_TO_TIME(cell->slotInfo, ulSlotInfo,  PHY_DELTA_UL + SCHED_DELTA, cell->numSlots);

   dlIndx = (dlSlotInfo.sfn*MAX_SLOTS+dlSlotInfo.slot)%MAX_DRX_SIZE;
   ulIndx = (ulSlotInfo.sfn*MAX_SLOTS+ulSlotInfo.slot)%MAX_DRX_SIZE;

   schHdlUlHqRetxStrtTimerForDl(cell, dlIndx);
   schHdlUlHqRetxStrtTimerForUl(cell, ulIndx);
}

/**
 * @brief Handling of the Ul harq Rtt start DRX timers
 *
 * @details
 *
 *     Function : schDrxStrtUlHqRttTmr
 *
 *      Handling of the Ul harq Rtt start DRX timers
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schDrxStrtUlHqRttTmr(SchUlHqProcCb *hqP)
{
   uint16_t harqRttExpTimer=0;
   SchDrxUeCb *drxUeCb;
   
   drxUeCb = &hqP->hqEnt->ue->drxUeCb;
   
   harqRttExpTimer =  (hqP->puschTime.sfn * MAX_SLOTS + hqP->puschTime.slot + drxUeCb->harqRttUlTimerLen)%MAX_DRX_SIZE;
   hqP->ulDrxHarqCb.rttExpIndex = harqRttExpTimer;
   schAddDrxNodeIntoHarqTimerList(&hqP->hqEnt->cell->drxCb[harqRttExpTimer].ulHarqRttExpiryList, hqP, &hqP->ulDrxHarqCb.rttExpNode);
}

/**
 * @brief Handling of the expiry onduration timer in dl direction
 *
 * @details
 *
 *     Function : schHdlDrxOnDurExpiryTimerForDlDirection
 *
 *      Handling of expiry onduration DRX timers in dl direction
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schHdlDrxOnDurExpiryTimerForDlDirection(SchCellCb  *cell, uint16_t currListIndx)
{
   CmLList  *drxCurrNode;
   SchUeCb *ueCb = NULLP;

   drxCurrNode = cell->drxCb[currListIndx].onDurationExpiryList.first;
   if(drxCurrNode)
   {
      /* Handling of dl On duration drx start list */
      while(drxCurrNode)
      {
         ueCb = (SchUeCb*)drxCurrNode->node;
         drxCurrNode = drxCurrNode->next;
         
         ueCb->drxUeCb.onDurationExpiryDistance--;

         if(ueCb->drxUeCb.onDurationExpiryDistance != SCH_DRX_INVALID_DISTANCE)
         {
            continue;
         }
         ueCb->drxUeCb.drxDlUeActiveMask &= ~UE_ACTIVE_FOR_ONDURATION;
         setDrxUeStatusForDlandUl(&ueCb->drxUeCb);
      }
   }
}

/**
 * @brief Handling of the expiry onduration DRX timers for Ul direction
 *
 * @details
 *
 *     Function  schHdlDrxOnDurExpiryTimerForUlDirection:
 *
 *      Handling of expiry onduration DRX timers in Ul direction
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schHdlDrxOnDurExpiryTimerForUlDirection(SchCellCb  *cell, uint16_t currListIndx)
{
   CmLList  *drxCurrNode;
   SchUeCb *ueCb = NULLP;

   drxCurrNode = cell->drxCb[currListIndx].onDurationExpiryList.first;
   if(drxCurrNode)
   {
      /* Handling of dl On duration drx start list */
      while(drxCurrNode)
      {
         ueCb = (SchUeCb*)drxCurrNode->node;
         drxCurrNode = drxCurrNode->next;

         if(ueCb->drxUeCb.onDurationExpiryDistance != SCH_DRX_INVALID_DISTANCE)
         {
            continue;
         }

         ueCb->drxUeCb.drxUlUeActiveMask &= ~UE_ACTIVE_FOR_ONDURATION;
         setDrxUeStatusForDlandUl(&ueCb->drxUeCb);
         cmLListDelFrm(&cell->drxCb[ueCb->drxUeCb.onDurationExpiryIndex].onDurationExpiryList, ueCb->drxUeCb.onDurationExpiryNodeInfo);
         SCH_FREE(ueCb->drxUeCb.onDurationExpiryNodeInfo, sizeof(CmLList));
         ueCb->drxUeCb.onDurationExpiryIndex = SCH_DRX_INVALID_INDEX; 
      }
   }
}

/**
 * @brief Handling of the expiry onduration DRX timers
 *
 * @details
 *
 *     Function : schHdlDrxOnDurExpiryTimer
 *
 *      Handling of expiry onduration DRX timers
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schHdlDrxOnDurExpiryTimer(SchCellCb  *cell)
{
   uint16_t dlIndx = 0, ulIndx = 0;
   SlotTimingInfo dlSlotInfo, ulSlotInfo;

   ADD_DELTA_TO_TIME(cell->slotInfo, dlSlotInfo, PHY_DELTA_DL + SCHED_DELTA, cell->numSlots);
   ADD_DELTA_TO_TIME(cell->slotInfo, ulSlotInfo, PHY_DELTA_UL + SCHED_DELTA, cell->numSlots);
   dlIndx = (dlSlotInfo.sfn*MAX_SLOTS+dlSlotInfo.slot)%MAX_DRX_SIZE;
   ulIndx = (ulSlotInfo.sfn*MAX_SLOTS+ulSlotInfo.slot)%MAX_DRX_SIZE;

   schHdlDrxOnDurExpiryTimerForDlDirection(cell, dlIndx);
   schHdlDrxOnDurExpiryTimerForUlDirection(cell, ulIndx);
}

/**
 * @brief Handling of the expiry of in-active DRX timers in Dl
 *
 * @details
 *
 *     Function : schHdlDrxInActvExpiryTimerForDlDirection
 *
 *      Handling of expiry of in-active DRX timers at Dl index
 *
 *  @param[in] SchCellCb  *cell,  uint16_t dlIndx
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schHdlDrxInActvExpiryTimerForDlDirection(SchCellCb  *cell, uint16_t dlIndx)
{
   CmLList  *drxNode;
   SchUeCb *ueCb = NULLP;

   drxNode = cell->drxCb[dlIndx].inActvTmrExpiryList.first;
   if(drxNode)
   {
      /* Handling of dl On duration drx start list */
      while(drxNode)
      {
         ueCb = (SchUeCb*)drxNode->node;
         drxNode = drxNode->next;
         ueCb->drxUeCb.inActiveTmrExpiryDistance--;

         if(ueCb->drxUeCb.onDurationExpiryDistance != SCH_DRX_INVALID_DISTANCE)
         {  
            continue;
         }
         
         ueCb->drxUeCb.drxDlUeActiveMask &= ~UE_ACTIVE_FOR_INACTIVE_TIMER;
         setDrxUeStatusForDlandUl(&ueCb->drxUeCb);
      }
   }
}

/**
 * @brief Handling of the expiry of in-active DRX timers in Ul
 *
 * @details
 *
 *     Function : schHdlDrxInActvExpiryTimerForUlDirection
 *
 *      Handling of expiry of in-active DRX timers at Ul index
 *
 *  @param[in] SchCellCb  *cell,  uint16_t ulIndx
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schHdlDrxInActvExpiryTimerForUlDirection(SchCellCb  *cell, uint16_t ulIndx)
{
   CmLList  *drxNode;
   SchUeCb *ueCb = NULLP;

   drxNode = cell->drxCb[ulIndx].inActvTmrExpiryList.first;
   if(drxNode)
   {
      /* Handling of dl On duration drx start list */
      while(drxNode)
      {
         ueCb = (SchUeCb*)drxNode->node;
         drxNode = drxNode->next;

         if(ueCb->drxUeCb.onDurationExpiryDistance != SCH_DRX_INVALID_DISTANCE)
         {  
            continue;
         }
         
         ueCb->drxUeCb.drxUlUeActiveMask &= ~UE_ACTIVE_FOR_INACTIVE_TIMER;
         setDrxUeStatusForDlandUl(&ueCb->drxUeCb);
         
         /* Remove the entry from the in-active exp timer list */
         cmLListDelFrm(&cell->drxCb[ulIndx].inActvTmrExpiryList, ueCb->drxUeCb.inActvTimerExpiryNodeInfo);
         SCH_FREE(ueCb->drxUeCb.inActvTimerExpiryNodeInfo, sizeof(CmLList));
         ueCb->drxUeCb.inActvExpiryIndex = SCH_DRX_INVALID_INDEX;

         if(ueCb->drxUeCb.shortCyclePresent)
         {  
            /* Start short cycle timer */
            schHdlDrxStartShortCycleTimer(cell, ueCb);
         }
      }
   }
}

/**
 * @brief Handling of the expiry of in-active DRX timers
 *
 * @details
 *
 *     Function :schHdlDrxInActvExpiryTimer
 *
 *      Handling of expiry  of in-active DRX timers
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schHdlDrxInActvExpiryTimer(SchCellCb  *cell)
{
   uint16_t dlIndx = 0, ulIndx = 0;
   SlotTimingInfo dlSlotInfo, ulSlotInfo;

   ADD_DELTA_TO_TIME(cell->slotInfo, dlSlotInfo, PHY_DELTA_DL + SCHED_DELTA, cell->numSlots);
   ADD_DELTA_TO_TIME(cell->slotInfo, ulSlotInfo, PHY_DELTA_UL + SCHED_DELTA, cell->numSlots);
   dlIndx = (dlSlotInfo.sfn*MAX_SLOTS+dlSlotInfo.slot)%MAX_DRX_SIZE;
   ulIndx = (ulSlotInfo.sfn*MAX_SLOTS+ulSlotInfo.slot)%MAX_DRX_SIZE;

   schHdlDrxInActvExpiryTimerForDlDirection(cell, dlIndx);
   schHdlDrxInActvExpiryTimerForUlDirection(cell, ulIndx);
}

/**
 * @brief Handling of the expiry ShortCycle DRX timers in DL
 *
 * @details
 *
 *     Function : schHdlDrxShortCycleExpiryTimerForDlDirection
 *
 *      Handling of expiry ShortCycle
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schHdlDrxShortCycleExpiryTimerForDlDirection(SchCellCb  *cell, uint16_t dlIndx)
{
   CmLList  *drxCurrNode = NULLP;
   SchUeCb *ueCb = NULLP;

   drxCurrNode = cell->drxCb[dlIndx].shortCycleExpiryList.first;
   if(drxCurrNode)
   {
      /* Handling of short cycle expiry in Dl */
      while(drxCurrNode)
      {
         ueCb = (SchUeCb*)drxCurrNode->node;
         drxCurrNode = drxCurrNode->next;

         ueCb->drxUeCb.shortCycleDistance--;

         if(ueCb->drxUeCb.shortCycleDistance != SCH_DRX_INVALID_DISTANCE)
         {
            continue;
         }

         /* once short cycle timer gets expire we will recalculate on-duration start with long cycle */ 
         ueCb->drxUeCb.longCycleToBeUsed = true;

         /* delete the entry from on-duration start */
         cmLListDelFrm(&cell->drxCb[ueCb->drxUeCb.onDurationStartIndex].onDurationStartList, ueCb->drxUeCb.onDurationStartNodeInfo);
         SCH_FREE(ueCb->drxUeCb.onDurationStartNodeInfo, sizeof(CmLList));
         ueCb->drxUeCb.onDurationStartIndex = SCH_DRX_INVALID_INDEX; 
         ueCb->drxUeCb.onDurationStartDistance = SCH_DRX_INVALID_DISTANCE; 

         /* Recalculate on-duration with  long cycle */
         schAddUeInOndurationList(cell, ueCb, PHY_DELTA_DL + SCHED_DELTA);
      }
   }
}

/**
 * @brief Handling of the expiry ShortCycle DRX timers in UL
 *
 * @details
 *
 *     Function : schHdlDrxShortCycleExpiryTimerForUlDirection
 *
 *      Handling of expiry ShortCycle
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schHdlDrxShortCycleExpiryTimerForUlDirection(SchCellCb  *cell, uint16_t ulIndx)
{
   CmLList  *drxCurrNode = NULLP;
   SchUeCb *ueCb = NULLP;

   drxCurrNode = cell->drxCb[ulIndx].shortCycleExpiryList.first;
   if(drxCurrNode)
   {
      /* Handling of short cycle expiry in Ul */
      while(drxCurrNode)
      {
         ueCb = (SchUeCb*)drxCurrNode->node;
         drxCurrNode = drxCurrNode->next;
         
         if(ueCb->drxUeCb.shortCycleDistance != SCH_DRX_INVALID_DISTANCE)
         {
            continue;
         }
         
         cmLListDelFrm(&cell->drxCb[ulIndx].shortCycleExpiryList, ueCb->drxUeCb.shortCycleTmrExpiryNodeInfo);
         SCH_FREE(ueCb->drxUeCb.shortCycleTmrExpiryNodeInfo, sizeof(CmLList));
         ueCb->drxUeCb.shortCycleExpiryIndex = SCH_DRX_INVALID_INDEX;

      }
   }
}

/**
 * @brief Handling of the expiry ShortCycle DRX timers
 *
 * @details
 *
 *     Function :schHdlDrxShortCycleExpiryTimer
 *
 *      Handling of expiry ShortCycle
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schHdlDrxShortCycleExpiryTimer(SchCellCb  *cell)
{
   uint16_t dlIndx = 0, ulIndx= 0;
   SlotTimingInfo dlSlotInfo, ulSlotInfo;

   ADD_DELTA_TO_TIME(cell->slotInfo, dlSlotInfo, PHY_DELTA_DL + SCHED_DELTA, cell->numSlots);
   ADD_DELTA_TO_TIME(cell->slotInfo, ulSlotInfo, PHY_DELTA_UL + SCHED_DELTA, cell->numSlots);
   dlIndx = (dlSlotInfo.sfn*MAX_SLOTS+dlSlotInfo.slot)%MAX_DRX_SIZE;
   ulIndx = (ulSlotInfo.sfn*MAX_SLOTS+ulSlotInfo.slot)%MAX_DRX_SIZE;

   schHdlDrxShortCycleExpiryTimerForDlDirection(cell, dlIndx);
   schHdlDrxShortCycleExpiryTimerForUlDirection(cell, ulIndx);
}

/**
 * @brief Handling of the expiry Dl harq rrt DRX timers
 *
 * @details
 *
 *     Function : schHdlDlHqRttExpiryTimerForDl
 *
 *      Handling of expiry Dl harq rrt DRX timers
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schHdlDlHqRttExpiryTimerForDl(SchCellCb  *cell, uint16_t currIdx)
{
   SchDlHqProcCb *hqP;
   CmLList *drxCurrNode;

   drxCurrNode = cell->drxCb[currIdx].dlHarqRttExpiryList.first;
   
   while(drxCurrNode)
   {
      hqP  = (SchDlHqProcCb*)drxCurrNode->node;
      drxCurrNode = drxCurrNode->next;
      
     /* Add ue to dlRetransTmrStartList list */
     if(hqP->dlDrxHarqCb.retxStrtIndex == SCH_DRX_INVALID_INDEX)
     {
       schAddDrxNodeIntoHarqTimerList(&cell->drxCb[currIdx + 1].dlRetransTmrStartList, hqP, &hqP->dlDrxHarqCb.retxStrtNode);
       hqP->dlDrxHarqCb.retxStrtIndex = currIdx + 1;
     }
   }
}

/**
 * @brief Handling of the expiry Dl harq retransmission DRX timers
 *
 * @details
 *
 *     Function : schHdlDlHqRetxExpiryTimerForDl
 *
 *      Handling of expiry Dl harq retransmission DRX timers
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
**/

void schHdlDlHqRetxExpiryTimerForDl(SchCellCb  *cell, uint16_t currIdx)
{
   SchDlHqProcCb *hqP;
   SchUeCb *ueCb;
   CmLList *drxCurrNode;

   drxCurrNode = cell->drxCb[currIdx].dlRetransExpiryList.first;
   
   while(drxCurrNode)
   {
      hqP  = (SchDlHqProcCb*)drxCurrNode->node;
      drxCurrNode = drxCurrNode->next;
      ueCb = hqP->hqEnt->ue;
      
      ueCb->drxUeCb.drxDlUeActiveMaskForHarq &= ~(SCH_DRX_DL_HARQ_BITMASK << hqP->procId);
      
      /* Set the Ue status as inactive */
      setDrxUeStatusForDlandUl(&ueCb->drxUeCb);
   } 
}

/**
 * @brief Handling of the expiry Dl harq rrt DRX timers for Ul scheduling
 *
 * @details
 *
 *     Function : schHdlDlHqRttExpiryTimerForUl
 *
 *      Handling of expiry Dl harq rrt DRX timers for Ul scheduling
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
**/

void schHdlDlHqRttExpiryTimerForUl(SchCellCb  *cell, uint16_t currIdx)
{
   SchDlHqProcCb *hqP;
   CmLList *drxCurrNode;

   drxCurrNode = cell->drxCb[currIdx].dlHarqRttExpiryList.first;
   
   while(drxCurrNode)
   {
      hqP  = (SchDlHqProcCb*)drxCurrNode->node;
      drxCurrNode = drxCurrNode->next;
      
      /* Delete the node from list */
      cmLListDelFrm(&cell->drxCb[currIdx].dlHarqRttExpiryList, hqP->dlDrxHarqCb.rttExpNode);
      SCH_FREE(hqP->dlDrxHarqCb.rttExpNode, sizeof(CmLList));
      hqP->dlDrxHarqCb.rttExpIndex = SCH_DRX_INVALID_INDEX;
   } 
}

/**
 * @brief Handling of the expiry Dl harq retransmission DRX timers for Ul scheduling
 *
 * @details
 *
 *     Function : schHdlDlHqRetxExpiryTimerForUl
 *
 *      Handling of expiry Dl harq retransmission DRX timers for Ul scheduling
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
**/

void schHdlDlHqRetxExpiryTimerForUl(SchCellCb  *cell, uint16_t currIdx)
{
   SchDlHqProcCb *hqP;
   SchUeCb *ueCb;
   CmLList *drxCurrNode;

   drxCurrNode = cell->drxCb[currIdx].dlRetransExpiryList.first;
   
   while(drxCurrNode)
   {
      hqP  = (SchDlHqProcCb*)drxCurrNode->node;
      ueCb = hqP->hqEnt->ue;
      drxCurrNode = drxCurrNode->next;
      
      /* Set the Ue status as inactive for uplink */
      ueCb->drxUeCb.drxUlUeActiveMaskForHarq &= ~(SCH_DRX_UL_HARQ_BITMASK << hqP->procId);
      setDrxUeStatusForDlandUl(&ueCb->drxUeCb);
      
      /* Delete the UE */
      cmLListDelFrm(&cell->drxCb[currIdx].dlRetransExpiryList, hqP->dlDrxHarqCb.retxExpNode);
      SCH_FREE(hqP->dlDrxHarqCb.retxExpNode, sizeof(CmLList));
      hqP->dlDrxHarqCb.retxExpIndex = SCH_DRX_INVALID_INDEX;
      
   } 
}

/**
 * @brief Handling of the expiry of Dl harq DRX timers
 *
 * @details
 *
 *     Function : schHdlDrxDlHarqExpireTimer
 *
 *      Handling of expiry of Dl harq DRX timers
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# void
 
 **/

void schHdlDrxDlHarqExpireTimer(SchCellCb  *cell)
{
   uint16_t dlIndx = 0, ulIndx = 0;
   SlotTimingInfo dlSlotInfo, ulSlotInfo;

   ADD_DELTA_TO_TIME(cell->slotInfo, dlSlotInfo, PHY_DELTA_DL + SCHED_DELTA, cell->numSlots);
   ADD_DELTA_TO_TIME(cell->slotInfo, ulSlotInfo, PHY_DELTA_UL + SCHED_DELTA, cell->numSlots);
   dlIndx = (dlSlotInfo.sfn*MAX_SLOTS+dlSlotInfo.slot)%MAX_DRX_SIZE;
   ulIndx = (ulSlotInfo.sfn*MAX_SLOTS+ulSlotInfo.slot)%MAX_DRX_SIZE;
   
   schHdlDlHqRttExpiryTimerForDl(cell, dlIndx);
   schHdlDlHqRetxExpiryTimerForDl(cell, dlIndx);
   schHdlDlHqRttExpiryTimerForUl(cell, ulIndx );
   schHdlDlHqRetxExpiryTimerForUl(cell, ulIndx);
}

/**
 * @brief Handling of the expiry of harq DRX timers
 *
 * @details
 *
 *     Function : schDrxStopDlHqRetxTmr
 *
 *      Handling of expiry  DRX timers
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schDrxStopDlHqRetxTmr(SchCellCb  *cell, SchUeCb *ueCb, SchDlHqProcCb **hqP)
{
    
   if((*hqP)->dlDrxHarqCb.retxExpIndex != SCH_DRX_INVALID_INDEX)
   {
      ueCb->drxUeCb.drxDlUeActiveMaskForHarq &= ~(SCH_DRX_DL_HARQ_BITMASK << (*hqP)->procId);
      ueCb->drxUeCb.drxUlUeActiveMaskForHarq &= ~(SCH_DRX_UL_HARQ_BITMASK << (*hqP)->procId);

      /* Change the UE status to Inactive */
      setDrxUeStatusForDlandUl(&ueCb->drxUeCb);
      
      /* If there is any node present in rtt list then remove the node from list */
      if((*hqP)->dlDrxHarqCb.rttExpIndex != SCH_DRX_INVALID_INDEX)
      {
         cmLListDelFrm(&cell->drxCb[(*hqP)->dlDrxHarqCb.rttExpIndex].dlHarqRttExpiryList, (*hqP)->dlDrxHarqCb.rttExpNode);
         SCH_FREE((*hqP)->dlDrxHarqCb.rttExpNode, sizeof(CmLList));
         (*hqP)->dlDrxHarqCb.rttExpIndex = SCH_DRX_INVALID_INDEX;
      }
      
      /* If there is any node present in retx list then remove the node from list */
      if((*hqP)->dlDrxHarqCb.retxStrtIndex != SCH_DRX_INVALID_INDEX)
      {
         cmLListDelFrm(&cell->drxCb[(*hqP)->dlDrxHarqCb.retxStrtIndex].dlRetransTmrStartList, (*hqP)->dlDrxHarqCb.retxStrtNode);
         SCH_FREE((*hqP)->dlDrxHarqCb.retxStrtNode, sizeof(CmLList));
         (*hqP)->dlDrxHarqCb.retxStrtIndex  = SCH_DRX_INVALID_INDEX;
      }

      cmLListDelFrm(&cell->drxCb[(*hqP)->dlDrxHarqCb.retxExpIndex].dlRetransExpiryList, (*hqP)->dlDrxHarqCb.retxExpNode);
      SCH_FREE((*hqP)->dlDrxHarqCb.retxExpNode, sizeof(CmLList));
      (*hqP)->dlDrxHarqCb.retxExpIndex  = SCH_DRX_INVALID_INDEX;
   }
}

/**
 * @brief Handling of the expiry Ul harq rrt DRX timers
 *
 * @details
 *
 *     Function : schHdlUlHqRttExpiryTimerForDl
 *
 *      Handling of expiry ul harq rrt DRX timers
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schHdlUlHqRttExpiryTimerForDl(SchCellCb  *cell, uint16_t currIdx)
{
   SchUlHqProcCb *hqP;
   CmLList *drxCurrNode;

   drxCurrNode = cell->drxCb[currIdx].ulHarqRttExpiryList.first;
   
   while(drxCurrNode)
   {
      hqP  = (SchUlHqProcCb*)drxCurrNode->node;
      drxCurrNode = drxCurrNode->next;
      
     /* Add ue to ulRetransTmrStartList list */
     if(hqP->ulDrxHarqCb.retxStrtIndex == SCH_DRX_INVALID_INDEX)
     {
       schAddDrxNodeIntoHarqTimerList(&cell->drxCb[currIdx + 1].ulRetransTmrStartList, hqP, &hqP->ulDrxHarqCb.retxStrtNode);
       hqP->ulDrxHarqCb.retxStrtIndex = currIdx + 1;
     }
   }
}

/**
 * @brief Handling of the expiry ul harq retransmission DRX timers
 *
 * @details
 *
 *     Function : schHdlUlHqRetxExpiryTimerForDl
 *
 *      Handling of expiry Ul harq retransmission DRX timers
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
**/

void schHdlUlHqRetxExpiryTimerForDl(SchCellCb  *cell, uint16_t currIdx)
{
   SchUlHqProcCb *hqP;
   SchUeCb *ueCb;
   CmLList *drxCurrNode;

   drxCurrNode = cell->drxCb[currIdx].ulRetransExpiryList.first;
   
   while(drxCurrNode)
   {
      hqP  = (SchUlHqProcCb*)drxCurrNode->node;
      drxCurrNode = drxCurrNode->next;
      ueCb = hqP->hqEnt->ue;
      
      ueCb->drxUeCb.drxUlUeActiveMaskForHarq &= ~(SCH_DRX_UL_HARQ_BITMASK << hqP->procId);
      
      /* Set the Ue status as inactive */
      setDrxUeStatusForDlandUl(&ueCb->drxUeCb);
   } 
}

/**
 * @brief Handling of the expiry ul harq rrt DRX timers for Ul scheduling
 *
 * @details
 *
 *     Function : schHdlUlHqRttExpiryTimerForUl
 *
 *      Handling of expiry Ul harq rrt DRX timers for Ul scheduling
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
**/

void schHdlUlHqRttExpiryTimerForUl(SchCellCb  *cell, uint16_t currIdx)
{
   SchUlHqProcCb *hqP;
   CmLList *drxCurrNode;

   drxCurrNode = cell->drxCb[currIdx].ulHarqRttExpiryList.first;
   
   while(drxCurrNode)
   {
      hqP  = (SchUlHqProcCb*)drxCurrNode->node;
      drxCurrNode = drxCurrNode->next;
      
      /* Delete the node from list */
      cmLListDelFrm(&cell->drxCb[currIdx].ulHarqRttExpiryList, hqP->ulDrxHarqCb.rttExpNode);
      SCH_FREE(hqP->ulDrxHarqCb.rttExpNode, sizeof(CmLList));
      hqP->ulDrxHarqCb.rttExpIndex = SCH_DRX_INVALID_INDEX;
   } 
}

/**
 * @brief Handling of the expiry Ul harq retransmission DRX timers for Ul scheduling
 *
 * @details
 *
 *     Function : schHdlUlHqRetxExpiryTimerForUl
 *
 *      Handling of expiry Ul harq retransmission DRX timers for Ul scheduling
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
**/

void schHdlUlHqRetxExpiryTimerForUl(SchCellCb  *cell, uint16_t currIdx)
{
   SchUlHqProcCb *hqP;
   SchUeCb *ueCb;
   CmLList *drxCurrNode;

   drxCurrNode = cell->drxCb[currIdx].ulRetransExpiryList.first;
   
   while(drxCurrNode)
   {
      hqP  = (SchUlHqProcCb*)drxCurrNode->node;
      ueCb = hqP->hqEnt->ue;
      drxCurrNode = drxCurrNode->next;
      
      /* Set the Ue status as inactive for uplink */
      ueCb->drxUeCb.drxUlUeActiveMaskForHarq &= ~(SCH_DRX_UL_HARQ_BITMASK << hqP->procId);
      setDrxUeStatusForDlandUl(&ueCb->drxUeCb);
      
      /* Delete the UE */
      cmLListDelFrm(&cell->drxCb[currIdx].ulRetransExpiryList, hqP->ulDrxHarqCb.retxExpNode);
      SCH_FREE(hqP->ulDrxHarqCb.retxExpNode, sizeof(CmLList));
      hqP->ulDrxHarqCb.retxExpIndex = SCH_DRX_INVALID_INDEX;
      
   } 
}

/**
 * @brief Handling of the expiry of Ul harq DRX timers
 *
 * @details
 *
 *     Function : schHdlDrxUlHarqExpireTimer
 *
 *      Handling of expiry of Ul harq DRX timers
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# void
 
 **/

void schHdlDrxUlHarqExpireTimer(SchCellCb  *cell)
{
   uint16_t dlIndx = 0, ulIndx = 0;
   SlotTimingInfo dlSlotInfo, ulSlotInfo;

   ADD_DELTA_TO_TIME(cell->slotInfo, dlSlotInfo, PHY_DELTA_DL + SCHED_DELTA, cell->numSlots);
   ADD_DELTA_TO_TIME(cell->slotInfo, ulSlotInfo, PHY_DELTA_UL + SCHED_DELTA, cell->numSlots);
   dlIndx = (dlSlotInfo.sfn*MAX_SLOTS+dlSlotInfo.slot)%MAX_DRX_SIZE;
   ulIndx = (ulSlotInfo.sfn*MAX_SLOTS+ulSlotInfo.slot)%MAX_DRX_SIZE;
   
   schHdlUlHqRttExpiryTimerForDl(cell, dlIndx);
   schHdlUlHqRetxExpiryTimerForDl(cell, dlIndx);
   schHdlUlHqRttExpiryTimerForUl(cell, ulIndx );
   schHdlUlHqRetxExpiryTimerForUl(cell, ulIndx);
}

/**
 * @brief Hanuling of the expiry of harq DRX timers
 *
 * @details
 *
 *     Function : schDrxStopUlHqRetxTmr
 *
 *      Hanuling of expiry DRX timers
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schDrxStopUlHqRetxTmr(SchCellCb  *cell, SchUeCb *ueCb, SchUlHqProcCb **hqP)
{
    
   if((*hqP)->ulDrxHarqCb.retxExpIndex != SCH_DRX_INVALID_INDEX)
   {
      ueCb->drxUeCb.drxDlUeActiveMaskForHarq &= ~(SCH_DRX_DL_HARQ_BITMASK << (*hqP)->procId);
      ueCb->drxUeCb.drxUlUeActiveMaskForHarq &= ~(SCH_DRX_UL_HARQ_BITMASK << (*hqP)->procId);

      /* Change the UE status to Inactive */
      setDrxUeStatusForDlandUl(&ueCb->drxUeCb);
      
      /* If there is any node present in rtt list then remove the node from list */
      if((*hqP)->ulDrxHarqCb.rttExpIndex != SCH_DRX_INVALID_INDEX)
      {
         cmLListDelFrm(&cell->drxCb[(*hqP)->ulDrxHarqCb.rttExpIndex].ulHarqRttExpiryList, (*hqP)->ulDrxHarqCb.rttExpNode);
         SCH_FREE((*hqP)->ulDrxHarqCb.rttExpNode, sizeof(CmLList));
         (*hqP)->ulDrxHarqCb.rttExpIndex = SCH_DRX_INVALID_INDEX;
      }
      
      /* If there is any node present in retx list then remove the node from list */
      if((*hqP)->ulDrxHarqCb.retxStrtIndex != SCH_DRX_INVALID_INDEX)
      {
         cmLListDelFrm(&cell->drxCb[(*hqP)->ulDrxHarqCb.retxStrtIndex].ulRetransTmrStartList, (*hqP)->ulDrxHarqCb.retxStrtNode);
         SCH_FREE((*hqP)->ulDrxHarqCb.retxStrtNode, sizeof(CmLList));
         (*hqP)->ulDrxHarqCb.retxStrtIndex  = SCH_DRX_INVALID_INDEX;
      }

      cmLListDelFrm(&cell->drxCb[(*hqP)->ulDrxHarqCb.retxExpIndex].ulRetransExpiryList, (*hqP)->ulDrxHarqCb.retxExpNode);
      SCH_FREE((*hqP)->ulDrxHarqCb.retxExpNode, sizeof(CmLList));
      (*hqP)->ulDrxHarqCb.retxExpIndex  = SCH_DRX_INVALID_INDEX;
   }
}

/**
 * @brief Handling of the expiry  DRX timers
 *
 * @details
 *
 *     Function : schHandleExpiryDrxTimer
 *
 *      Handling of expiry  DRX timers
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/

void schHandleExpiryDrxTimer(SchCellCb  *cell)
{
   schHdlDrxShortCycleExpiryTimer(cell);
   schHdlDrxOnDurExpiryTimer(cell);
   schHdlDrxInActvExpiryTimer(cell);
   schHdlDrxDlHarqExpireTimer(cell);
   schHdlDrxUlHarqExpireTimer(cell);
}

#endif
/**********************************************************************
  End of file
 **********************************************************************/
