
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
   hqDrxCb->retxExpDistance  = SCH_DRX_INVALID_DISTANCE;
   hqDrxCb->retxStrtIndex    = SCH_DRX_INVALID_INDEX;
   hqDrxCb->rttIndex         = SCH_DRX_INVALID_INDEX;
   hqDrxCb->retxIndex        = SCH_DRX_INVALID_INDEX;
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
   ueCb->drxUeCb.drxDlUeActiveStatus = 0; 
   ueCb->drxUeCb.drxUlUeActiveStatus = 0; 
}

/* will uncomment this function in next gerrit */
#if 0
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
    uint16_t tmrIdx = 0;
    SchDlHqProcCb *procs;
    CmLList  *node = NULLP;
   
    numHqPrcs = dlHqEnt->numHqPrcs;
    for(idx =0; idx<numHqPrcs; idx++)
    {
       procs = &dlHqEnt->procs[idx];
       tmrIdx = procs->dlDrxHarqCb.retxStrtIndex;
       CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].dlRetransTmrStartList, node);
       if(node)
       {
          cmLListDelFrm(&cell->drxCb[tmrIdx].dlRetransTmrStartList, node);
          SCH_FREE(node, sizeof(CmLList));
       }

       tmrIdx = procs->dlDrxHarqCb.rttIndex;
       CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].dlHarqRttExpiryList, node);
       if(node)
       {
          cmLListDelFrm(&cell->drxCb[tmrIdx].dlHarqRttExpiryList, node);
          SCH_FREE(node, sizeof(CmLList));
       }

       tmrIdx = procs->dlDrxHarqCb.retxIndex;
       CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].dlRetransExpiryList, node);
       if(node)
       {
          cmLListDelFrm(&cell->drxCb[tmrIdx].dlRetransExpiryList, node);
          SCH_FREE(node, sizeof(CmLList));
       }
       schInitDrxHarqCb(&procs->dlDrxHarqCb);
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
    uint16_t tmrIdx = 0;
    CmLList  *node = NULLP;
    SchUlHqProcCb *procs;

    numHqPrcs = ulHqEnt->numHqPrcs;
    for(idx =0; idx<numHqPrcs; idx++)
    {
       procs = &ulHqEnt->procs[idx];
       tmrIdx = procs->ulDrxHarqCb.retxStrtIndex;
       CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].ulRetransTmrStartList, node);
       if(node)
       {
          cmLListDelFrm(&cell->drxCb[tmrIdx].ulRetransTmrStartList, node);
          SCH_FREE(node, sizeof(CmLList));
       }

       tmrIdx = procs->ulDrxHarqCb.rttIndex;
       CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].ulHarqRttExpiryList, node);
       if(node)
       {
          cmLListDelFrm(&cell->drxCb[tmrIdx].ulHarqRttExpiryList, node);
          SCH_FREE(node, sizeof(CmLList));
       }

       tmrIdx = procs->ulDrxHarqCb.retxIndex;
       CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].ulRetransExpiryList, node);
       if(node)
       {
          cmLListDelFrm(&cell->drxCb[tmrIdx].ulRetransExpiryList, node);
          SCH_FREE(node, sizeof(CmLList));
       }
       schInitDrxHarqCb(&procs->ulDrxHarqCb);
    }
 }
#endif

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
      /* TODO - will uncomment this function in next gerrit */
      //schDeleteDlHarqDrxTimer(cell, &ueCb->dlHqEnt);
      //schDeleteUlHarqDrxTimer(cell, &ueCb->ulHqEnt);
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

uint8_t schAddDrxTimerIntoList(CmLListCp *drxTimerList,void * nodeInfo, CmLList *drxNodeInfo)
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
   drxNodeInfo = currentNodeInfo;
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
   schAddDrxTimerIntoList(&cell->drxCb[ueCb->drxUeCb.onDurationStartIndex].onDurationStartList, ueCb, ueCb->drxUeCb.onDurationStartNodeInfo);
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
      schAddDrxTimerIntoList(&cell->drxCb[ueCb->drxUeCb.onDurationStartIndex].onDurationStartList, ueCb, ueCb->drxUeCb.onDurationStartNodeInfo);

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

void schHdlDrxOnDurStrtTimerForDlDirection(SchCellCb  *cell, uint16_t currListIndx, bool isDlDirection)
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
            ueCb->drxUeCb.drxDlUeActiveStatus |= UE_ACTIVE_FOR_ONDURATION;
            
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
            schAddDrxTimerIntoList(&cell->drxCb[onDurationExpiry].onDurationExpiryList, ueCb, ueCb->drxUeCb.onDurationExpiryNodeInfo);
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

void schHdlDrxOnDurStrtTimerForUlDirection(SchCellCb  *cell, uint16_t currListIndx, bool isDlDirection)
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
         ueCb->drxUeCb.drxUlUeActiveStatus |= UE_ACTIVE_FOR_ONDURATION;


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
         schAddDrxTimerIntoList(&cell->drxCb[ueCb->drxUeCb.onDurationStartIndex].onDurationStartList, ueCb, ueCb->drxUeCb.onDurationStartNodeInfo);
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
   schHdlDrxOnDurStrtTimerForDlDirection(cell, dlIndx, true);
   schHdlDrxOnDurStrtTimerForUlDirection(cell, ulIndx, false);
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
void schHdlDrxOnDurExpiryTimerForDlDirection(SchCellCb  *cell, uint16_t currListIndx, bool isDlDirection)
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
         ueCb->drxUeCb.drxDlUeActiveStatus &= ~UE_ACTIVE_FOR_ONDURATION;
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
void schHdlDrxOnDurExpiryTimerForUlDirection(SchCellCb  *cell, uint16_t currListIndx, bool isDlDirection)
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

         ueCb->drxUeCb.drxUlUeActiveStatus &= ~UE_ACTIVE_FOR_ONDURATION;
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

   schHdlDrxOnDurExpiryTimerForDlDirection(cell, dlIndx, true);
   schHdlDrxOnDurExpiryTimerForUlDirection(cell, ulIndx, false);
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
   /* Handling the onduration start timer */
   schHdlDrxOnDurExpiryTimer(cell);
}

#endif
/**********************************************************************
  End of file
 **********************************************************************/
