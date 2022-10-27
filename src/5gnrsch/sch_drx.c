
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
   hqDrxCb->retxExpDistance = -1;
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
   ueCb->drxUeCb.shortCycleDistance = -1;
   ueCb->drxUeCb.onDurationStartDistance = -1;
   ueCb->drxUeCb.onDurationExpiryDistance = -1;
   ueCb->drxUeCb.inActiveTmrExpiryDistance = -1;
   ueCb->drxUeCb.drxDlUeActiveStatus = false; 
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
    uint16_t tmrIdx = 0;
    SchDlHqProcCb *procs;
    CmLList  *node = NULLP;
   
    numHqPrcs = dlHqEnt->numHqPrcs;
    for(idx =0; idx<numHqPrcs; idx++)
    {
       procs = &dlHqEnt->procs[idx];
       tmrIdx = procs->drxHarqCb.retxStrtIndx;
       CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].dlRetransTmrStartList, node);
       if(node)
       {
          cmLListDelFrm(&cell->drxCb[tmrIdx].dlRetransTmrStartList, node);
          SCH_FREE(node, sizeof(CmLList));
       }

       tmrIdx = procs->drxHarqCb.rttIndx;
       CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].dlHarqRttExpiryList, node);
       if(node)
       {
          cmLListDelFrm(&cell->drxCb[tmrIdx].dlHarqRttExpiryList, node);
          SCH_FREE(node, sizeof(CmLList));
       }

       tmrIdx = procs->drxHarqCb.retxIndx;
       CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].dlRetransExpiryList, node);
       if(node)
       {
          cmLListDelFrm(&cell->drxCb[tmrIdx].dlRetransExpiryList, node);
          SCH_FREE(node, sizeof(CmLList));
       }
       schInitDrxHarqCb(&procs->drxHarqCb);
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
       tmrIdx = procs->drxHarqCb.retxStrtIndx;
       CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].ulRetransTmrStartList, node);
       if(node)
       {
          cmLListDelFrm(&cell->drxCb[tmrIdx].ulRetransTmrStartList, node);
          SCH_FREE(node, sizeof(CmLList));
       }

       tmrIdx = procs->drxHarqCb.rttIndx;
       CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].ulHarqRttExpiryList, node);
       if(node)
       {
          cmLListDelFrm(&cell->drxCb[tmrIdx].ulHarqRttExpiryList, node);
          SCH_FREE(node, sizeof(CmLList));
       }

       tmrIdx = procs->drxHarqCb.retxIndx;
       CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].ulRetransExpiryList, node);
       if(node)
       {
          cmLListDelFrm(&cell->drxCb[tmrIdx].ulRetransExpiryList, node);
          SCH_FREE(node, sizeof(CmLList));
       }
       schInitDrxHarqCb(&procs->drxHarqCb);
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
 *  @return  
 *      -# ROK
 *      -# RFAILED
 **/
void schDeleteUeDrxInfo(SchCellCb  *cell, SchUeCb *ueCb)
{
   uint16_t tmrIdx = 0;
   SchDrxUeCb *drxUeCb;
   CmLList  *node = NULLP;

   if(ueCb->ueDrxInfoPres == true)
   {
      drxUeCb = &ueCb->drxUeCb;
      
      /* delete on duration start timer from ueCb */
      tmrIdx = drxUeCb->onDurationStartIndex;
      CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].onDurationStartList, node);
      if(node)
      {
         cmLListDelFrm(&cell->drxCb[tmrIdx].onDurationStartList, node);
         SCH_FREE(node, sizeof(CmLList));
      }
      
      /* delete on duration expiry timer from ueCb */
      tmrIdx = drxUeCb->onDurationExpiryIndex;
      CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].onDurationExpiryList, node);
      if(node)
      {
         cmLListDelFrm(&cell->drxCb[tmrIdx].onDurationExpiryList, node);
         SCH_FREE(node, sizeof(CmLList));
      }
      
      /* delete inActv Expiry Index timer from ueCb */
      tmrIdx = drxUeCb->inActvExpiryIndex;
      CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].inActvTmrExpiryList, node);
      if(node)
      {
         cmLListDelFrm(&cell->drxCb[tmrIdx].inActvTmrExpiryList, node);
         SCH_FREE(node, sizeof(CmLList));
      }
      
      /* delete short cycle expiry timer from ueCb */
      tmrIdx = drxUeCb->shortCycleExpiryIndex;
      CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].shortCycleExpiryList, node);
      if(node)
      {
         cmLListDelFrm(&cell->drxCb[tmrIdx].shortCycleExpiryList, node);
         SCH_FREE(node, sizeof(CmLList));
      }

      schDeleteDlHarqDrxTimer(cell, &ueCb->dlHqEnt);//.numHqPrcs, &ueCb->dlHqEnt.procs);
      schDeleteUlHarqDrxTimer(cell, &ueCb->ulHqEnt);//.numHqPrcs, ueCb->ulHqEnt.procs);
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
   SCH_CNVRT_MS_TO_SLOT(drxUeCb->retransDlTimerLen, drxCfg.drxRetransmissionTimerDl, numerology);
   SCH_CNVRT_MS_TO_SLOT(drxUeCb->retransUlTimerLen, drxCfg.drxRetransmissionTimerUl, numerology);
   SCH_CNVRT_MS_TO_SLOT(drxUeCb->longCycleLen, drxCfg.drxLongCycleStartOffset.drxLongCycleStartOffsetChoice, numerology);
   drxUeCb->drxStartOffset =  drxCfg.drxLongCycleStartOffset.drxLongCycleStartOffsetVal;
   //drxCfg.shortDrxPres = false;
   if(drxCfg.shortDrxPres)
   {
      drxUeCb->shortCyclePresent = true;		   
      SCH_CNVRT_MS_TO_SLOT(drxUeCb->shortCycleLen, drxCfg.shortDrx.drxShortCycle, numerology);   
      drxUeCb->shortCycleTmrLen = drxCfg.shortDrx.drxShortCycleTimer;	
   }
   else
      drxUeCb->longCycleToBeUsed = true;
   SCH_CNVRT_MS_TO_SLOT(drxUeCb->drxSlotOffset, drxCfg.drxSlotOffset, numerology);  
   drxUeCb->drxSlotOffset = drxUeCb->drxSlotOffset>>5; 
   DU_LOG("\nDEBUG   -->  SCH : Drx timer information is stored successfully in ueCb");
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
uint8_t schAddDrxTimerIntoList(CmLListCp *drxTimerList,void * nodeInfo)
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

   DU_LOG("\nDEBUG   -->  SCH : Drx timer node is stored successfully");
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
   uint16_t   resultX, reusltY, idx;
   uint32_t   curTime, cycleLen, nxtDist;
   SlotTimingInfo  tmpTime;

   if (ueDrxCb->longCycleToBeUsed == true)
   {
      DU_LOG("\nNOT ShortCycleE  stored successfully");
      cycleLen = ueDrxCb->longCycleLen;
   }
   else
   {
      DU_LOG("\n ShortCycleE findNextOndurationOccurance SHORT");
      cycleLen = ueDrxCb->shortCycleLen;
   }

   /* Add delta to current time */
   ADD_DELTA_TO_TIME(cell->slotInfo, tmpTime, delta);
   
   /* Convert tmpTime to number of slots */
   curTime = ((tmpTime.sfn * MAX_SLOTS) + tmpTime.slot);
   
   /* as per 38.321,  if the criterion below is satisfied, then that sfn and
    * slot are the correct ones for the on-duration timer.
    * if the Short DRX Cycle is used, and [(SFN × 10) + subframe number] modulo
    * (drx-ShortCycle) = (drxStartOffset) modulo (drx-ShortCycle); or
    * if the Long DRX Cycle is used, and [(SFN × 10) + subframe number] modulo
    * (drx-LongCycle) = drxStartOffset  */
   for(idx =1; idx<MAX_DRX_SIZE; idx++)
   {
      resultX = (tmpTime.sfn *10+tmpTime.slot) % cycleLen;
      if (TRUE != ueDrxCb->shortCyclePresent)
         reusltY = ueDrxCb->drxStartOffset;
      else
         reusltY = ueDrxCb->drxStartOffset % cycleLen;
      if(resultX == reusltY)
      {
         memcpy(nxtOnDur, &tmpTime, sizeof(SlotTimingInfo));
         nxtDist = ((nxtOnDur->sfn * MAX_SLOTS) + nxtOnDur->slot);
         break;
      }
      else
      {
         ADD_DELTA_TO_TIME(tmpTime, tmpTime, 1);
      }
   }

   /* If slot offset is non-zero then Add slot offset to the calculated onDur
    * distance */
   if(ueDrxCb->drxSlotOffset)
   {
      nxtDist = nxtDist + ueDrxCb->drxSlotOffset;
      ADD_DELTA_TO_TIME(tmpTime, tmpTime, ueDrxCb->drxSlotOffset);
      memcpy(nxtOnDur, &tmpTime, sizeof(SlotTimingInfo));
   }
   /*If next On Duration is less than DL DELTA ahead, we will miss it and
    * hence need to move to the On-Duration after that.*/
   if((nxtDist - (curTime - delta)) <= SCH_DRX_MAX_DELTA)
   {
      ADD_DELTA_TO_TIME(tmpTime, tmpTime, cycleLen);
      memcpy(nxtOnDur, &tmpTime, sizeof(SlotTimingInfo));
   }
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
 *  @return  
 **/
void schDrxUeReCfgTimer(SchCellCb *cell, SchUeCb *ueCb)
{
   uint8_t  onDurIndx, currentSlotIndx;
   uint32_t onDurTime, onDurExpSlotTime, currentSlotTime;
   uint32_t tmrIdx, cycleLen;
   CmLList  *node = NULLP;
   SlotTimingInfo onDurationOccurance;

   if(ueCb->drxUeCb.shortCyclePresent == false)   
   {
      /* if short cycle configuration are not recived as a part of UE Recfg then if there is any entry present in short cycle timer list 
       * remove the entry from the list */ 
      tmrIdx = ueCb->drxUeCb.shortCycleExpiryIndex;
      CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].shortCycleExpiryList, node);
      if(node)
      {
         cmLListDelFrm(&cell->drxCb[tmrIdx].shortCycleExpiryList, node);
         SCH_FREE(node, sizeof(CmLList));
         ueCb->drxUeCb.shortCycleExpiryIndex = 0;
         ueCb->drxUeCb.shortCycleDistance = -1;
      }
   }
   /* If there is any entry present in on duration start list then remove the
    * entry from the list and recaluate the nect onduration cucurance */
   tmrIdx = ueCb->drxUeCb.onDurationStartIndex;
   CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].onDurationStartList, node);
   if(node)
   {
      cmLListDelFrm(&cell->drxCb[tmrIdx].onDurationStartList, node);
      SCH_FREE(node, sizeof(CmLList));
      ueCb->drxUeCb.onDurationStartIndex = 0;
      ueCb->drxUeCb.onDurationStartDistance = -1;
   }

   findNextOndurationOccurance(cell,  &ueCb->drxUeCb, &onDurationOccurance, 0);
   onDurTime = onDurationOccurance.sfn*MAX_SLOTS+onDurationOccurance.slot;
   onDurIndx = onDurTime%MAX_DRX_SIZE;
   /* If Onduration timer of old configuration is already running then next onduration 
    * starts once it expires*/
   if((ueCb->drxUeCb.onDurationExpiryDistance != -1) && (ueCb->drxUeCb.onDurationExpiryIndex == 0))
   {
       currentSlotTime = cell->slotInfo.sfn * MAX_SLOTS + cell->slotInfo.slot;
       currentSlotIndx = (currentSlotTime + 2)%512;
       if(currentSlotIndx >= ueCb->drxUeCb.onDurationExpiryIndex )
       {
         onDurExpSlotTime = currentSlotTime + ((ueCb->drxUeCb.onDurationExpiryDistance +1) * 512) + (ueCb->drxUeCb.onDurationExpiryIndex - currentSlotIndx + 2);
       }
       else
       {
         
         onDurExpSlotTime = currentSlotTime + ((ueCb->drxUeCb.onDurationExpiryDistance) * 512) + (ueCb->drxUeCb.onDurationExpiryIndex - currentSlotIndx + 2);
       }
       if(onDurTime <= onDurExpSlotTime)
       {
          if(ueCb->drxUeCb.longCycleToBeUsed = true)
             cycleLen = ueCb->drxUeCb.longCycleLen;
          else
             cycleLen = ueCb->drxUeCb.shortCycleLen;

          onDurTime = onDurTime + ((onDurExpSlotTime - onDurTime)/cycleLen + 1) * cycleLen;
       }
   }
   onDurIndx = onDurTime%MAX_DRX_SIZE;
   ueCb->drxUeCb.onDurationStartIndex = onDurIndx;
   ueCb->drxUeCb.onDurationStartDistance = SCH_CALC_SLOT_DIFF(onDurationOccurance, cell->slotInfo, MAX_SLOTS)/MAX_DRX_SIZE;
   schAddDrxTimerIntoList(&cell->drxCb[onDurIndx].onDurationStartList, ueCb);
   if((TRUE == ueCb->drxUeCb.shortCyclePresent) &&(ueCb->drxUeCb.longCycleToBeUsed == false))
   {
      /* Starting Short Cycle Timer */
      ueCb->drxUeCb.shortCycleDistance = (ueCb->drxUeCb.shortCycleTmrLen * ueCb->drxUeCb.shortCycleLen) / MAX_DRX_SIZE;
      ueCb->drxUeCb.shortCycleExpiryIndex = (onDurTime + (ueCb->drxUeCb.shortCycleTmrLen * ueCb->drxUeCb.shortCycleLen)) % MAX_DRX_SIZE;
      schAddDrxTimerIntoList(&cell->drxCb[ ueCb->drxUeCb.shortCycleExpiryIndex].shortCycleExpiryList, ueCb);
      DU_LOG("\nShortCycleE Added %d",ueCb->drxUeCb.shortCycleExpiryIndex);
   }
}
/**
 * @brief Add entry into  the on duration list and short cycle list
 *
 * @details
 *
 *     Function : schAddUeInOndurationAndShortCycleList
 *      
 *      This function is used to Add entry into  the on duration list and short
 *      cycle list
 *           
 *  @param[in] SchCellCb *cell, SchDrxUeCb *ueDrxCb, SlotTimingInfo  *nxtOnDur,
 *  uint8_t delta
 *  	       
 *  @return  
 *      -# void
 **/
void schAddUeInOndurationAndShortCycleList(SchCellCb *cell, SchUeCb *ueCb, uint8_t delta)
{
   uint8_t  onDurIndx;
   uint32_t onDurTime;
   SlotTimingInfo onDurationOccurance;

   if(ueCb->ueDrxInfoPres)
   {
      findNextOndurationOccurance(cell,  &ueCb->drxUeCb, &onDurationOccurance, delta);
      onDurTime = onDurationOccurance.sfn*MAX_SLOTS+onDurationOccurance.slot;
      onDurIndx = onDurTime%MAX_DRX_SIZE;
      ueCb->drxUeCb.onDurationStartIndex = onDurIndx;
      ueCb->drxUeCb.onDurationStartDistance = SCH_CALC_SLOT_DIFF(onDurationOccurance, cell->slotInfo, MAX_SLOTS)/MAX_DRX_SIZE;
      schAddDrxTimerIntoList(&cell->drxCb[onDurIndx].onDurationStartList, ueCb);

      if((TRUE == ueCb->drxUeCb.shortCyclePresent) &&(ueCb->drxUeCb.longCycleToBeUsed == false))
      {
         /* Starting Short Cycle Timer */
         ueCb->drxUeCb.shortCycleDistance = (ueCb->drxUeCb.shortCycleTmrLen * ueCb->drxUeCb.shortCycleLen) / MAX_DRX_SIZE;
         ueCb->drxUeCb.shortCycleExpiryIndex = (onDurTime + (ueCb->drxUeCb.shortCycleTmrLen * ueCb->drxUeCb.shortCycleLen)) % MAX_DRX_SIZE;
         schAddDrxTimerIntoList(&cell->drxCb[ ueCb->drxUeCb.shortCycleExpiryIndex].shortCycleExpiryList, ueCb);
         DU_LOG("\nShortCycleE Added %d",ueCb->drxUeCb.shortCycleExpiryIndex);
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
   uint16_t dlIndx = 0, tmrIdx=0, onDurationExpiry=0;
   CmLList  *dlNode, *tmpNode;
   SchUeCb *ueCb = NULLP;
   SlotTimingInfo dlSlotInfo, ulSlotInfo;

   ADD_DELTA_TO_TIME(cell->slotInfo, dlSlotInfo,  PHY_DELTA_DL + SCHED_DELTA);
   ADD_DELTA_TO_TIME(cell->slotInfo, ulSlotInfo,  PHY_DELTA_UL + SCHED_DELTA);
   
   dlIndx = (dlSlotInfo.sfn*MAX_SLOTS+dlSlotInfo.slot)%MAX_DRX_SIZE;
   dlNode = cell->drxCb[dlIndx].onDurationStartList.first;
   if(dlNode)
   {
      /* Handling of dl On duration drx start list */
      while(dlNode)
      {
         ueCb = (SchUeCb*)dlNode->node;
         dlNode = dlNode->next;
         ueCb->drxUeCb.onDurationStartDistance--;
         if(ueCb->drxUeCb.onDurationStartDistance != -1)
         {
            continue;
         }
         else
         {
            ueCb->drxUeCb.drxDlUeActiveStatus = true;
            /* If there is any entery present in onDurationExpiry list remove
             * the entery from the list and recalculate the
             * onDurationExpiry time and add it to list */
            tmrIdx = ueCb->drxUeCb.onDurationExpiryIndex;
            CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].onDurationExpiryList, tmpNode);
            if(tmpNode)
            {
               cmLListDelFrm(&cell->drxCb[tmrIdx].onDurationExpiryList, tmpNode);
               SCH_FREE(tmpNode, sizeof(CmLList));
               ueCb->drxUeCb.onDurationExpiryIndex = 0;
            }
            /* onDurationExpiry  = (current slot + onduration length) % MAX_DRX_SIZE*/
            onDurationExpiry = (dlIndx + ueCb->drxUeCb.onDurationLen)%MAX_DRX_SIZE;
            ueCb->drxUeCb.onDurationExpiryDistance =  (ueCb->drxUeCb.onDurationLen)/MAX_DRX_SIZE;
            schAddDrxTimerIntoList(&cell->drxCb[onDurationExpiry].onDurationExpiryList, ueCb);
            ueCb->drxUeCb.onDurationExpiryIndex = onDurationExpiry;

            /* if there is any entry present in onduration list remove the entry from the list  */
            DU_LOG("\nONDURATION START HIT %d %d startIndx %d [%d %d]",dlIndx, ueCb->drxUeCb.onDurationStartDistance,ueCb->drxUeCb.onDurationStartIndex, cell->slotInfo.sfn,cell->slotInfo.slot);
            tmrIdx = ueCb->drxUeCb.onDurationStartIndex;
            CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].onDurationStartList, tmpNode);
            if(tmpNode)
            {
               cmLListDelFrm(&cell->drxCb[tmrIdx].onDurationStartList, tmpNode);
               SCH_FREE(tmpNode, sizeof(CmLList));
               ueCb->drxUeCb.onDurationStartIndex = 0;
            }
            /* if there long cycle length is used as the cycle length for  onduration calculation then based on that calculating next onduration occurance 
             * other wise next onduration is calcuated based on short cycle
             * timer */
            //if(ueCb->drxUeCb.longCycleToBeUsed == 
            if(true  != ueCb->drxUeCb.shortCyclePresent)
            {
               schAddUeInOndurationAndShortCycleList(cell, ueCb, 0);
            }
         }
      }
   }

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
   schAddUeInOndurationAndShortCycleList(cell, ueCb, 3);
 }
/**
 * @brief Handling of the  DRX inactv timers start
 *
 * @details
 *
 *     Function : schHdlDrxInActvStrtTmr  
 *
 *      Handling of DRX in active timers start
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/
 void schHdlDrxInActvStrtTmr(SchCellCb  *cell,  SchUeCb *ueCb, uint8_t delta)
 {
    uint16_t slotIndx = 0, tmrIdx = 0;
    CmLList  *tmpNode;
    SlotTimingInfo tmpSlotInfo;
    if(ueCb->drxUeCb.inActvTimerLen == 0)
    {
       return;
    }
    ADD_DELTA_TO_TIME(cell->slotInfo, tmpSlotInfo,  delta);
    slotIndx = (tmpSlotInfo.sfn*MAX_SLOTS+tmpSlotInfo.slot)%MAX_DRX_SIZE;

    DU_LOG("\nIN ACTV START HIT %d %d", cell->slotInfo.sfn, cell->slotInfo.slot); 
    /* if there is nay old entry present in the list remove the entry*/
    tmrIdx = ueCb->drxUeCb.inActvExpiryIndex;
    CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].inActvTmrExpiryList, tmpNode);
    if(tmpNode)
    {
       cmLListDelFrm(&cell->drxCb[tmrIdx].inActvTmrExpiryList, tmpNode);
       SCH_FREE(tmpNode, sizeof(CmLList));
       ueCb->drxUeCb.inActvExpiryIndex= 0;
       ueCb->drxUeCb.inActiveTmrExpiryDistance= -1;
    }

    /* Adding the new entry in in activity timer list */
    ueCb->drxUeCb.inActvExpiryIndex = (slotIndx + ueCb->drxUeCb.inActvTimerLen) % MAX_DRX_SIZE;
    ueCb->drxUeCb.inActiveTmrExpiryDistance = (ueCb->drxUeCb.inActvTimerLen) / MAX_DRX_SIZE;
    schAddDrxTimerIntoList(&cell->drxCb[ueCb->drxUeCb.inActvExpiryIndex].inActvTmrExpiryList, ueCb);

    /* Set the UE active for UL And Dl transfer */
    ueCb->drxUeCb.drxDlUeActiveStatus = true;
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
   /* Handling the onduration start timea*/
   schHdlDrxOnDurStrtTimer(cell);
   //schHdlDrxInActvStrtTmr(cell);
}

/**
 * @brief Handling of the expiry ShortCycle DRX timers
 *
 * @details
 *
 *     Function : schHandleExpiryDrxTimer
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
   uint16_t dlIndx = 0, tmrIdx = 0;
   SlotTimingInfo dlSlotInfo, ulSlotInfo;
   CmLList  *dlNode,  *tmpNode;
   SchUeCb *ueCb = NULLP;

   ADD_DELTA_TO_TIME(cell->slotInfo, dlSlotInfo, PHY_DELTA_DL + SCHED_DELTA);
   ADD_DELTA_TO_TIME(cell->slotInfo, ulSlotInfo, PHY_DELTA_UL + SCHED_DELTA);

   dlIndx = (dlSlotInfo.sfn*MAX_SLOTS+dlSlotInfo.slot)%MAX_DRX_SIZE;
   dlNode = cell->drxCb[dlIndx].shortCycleExpiryList.first;
   if(dlNode)
   {
      /* Handling of dl On duration drx start list */
      while(dlNode)
      {
         ueCb = (SchUeCb*)dlNode->node;
         dlNode = dlNode->next;
         if(ueCb->ueDrxInfoPres)
         {
            ueCb->drxUeCb.shortCycleDistance--;

            if(ueCb->drxUeCb.shortCycleDistance== -1)
            {
               DU_LOG("\nShortCycleE EXP HIT %d",dlIndx);
                ueCb->drxUeCb.longCycleToBeUsed = true;
               /* Remove Short Cycle timer from the list */
               cmLListDelFrm(&cell->drxCb[dlIndx].shortCycleExpiryList, dlNode);
               SCH_FREE(dlNode, sizeof(CmLList));
               ueCb->drxUeCb.shortCycleExpiryIndex = 0;
               
               /* calculating the next duration event 
               If another entry is found in the On duration list, it must first be removed 
               before the next On duration occurrences may be calculated  */
               tmrIdx = ueCb->drxUeCb.onDurationStartIndex;
               CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].onDurationStartList, tmpNode);
               if(tmpNode)
               {
                  cmLListDelFrm(&cell->drxCb[tmrIdx].onDurationStartList, tmpNode);
                  SCH_FREE(tmpNode, sizeof(CmLList));
                  ueCb->drxUeCb.onDurationStartDistance = -1;
                  ueCb->drxUeCb.onDurationStartIndex = 0;
               }
               schAddUeInOndurationAndShortCycleList(cell, ueCb, PHY_DELTA_DL + SCHED_DELTA+1);
            }
            else
               continue;
         }
      }
   }
}


/**
 * @brief Handling of the expiry onduration DRX timers
 *
 * @details
 *
 *     Function : schHandleExpiryDrxTimer
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
   uint16_t dlIndx = 0, ulIndx = 0, tmrIdx = 0;
   SlotTimingInfo dlSlotInfo, ulSlotInfo;
   CmLList  *dlNode, *tmpNode;
   SchUeCb *ueCb = NULLP;

   ADD_DELTA_TO_TIME(cell->slotInfo, dlSlotInfo, PHY_DELTA_DL + SCHED_DELTA);
   ADD_DELTA_TO_TIME(cell->slotInfo, ulSlotInfo, PHY_DELTA_UL + SCHED_DELTA);

   dlIndx = (dlSlotInfo.sfn*MAX_SLOTS+dlSlotInfo.slot)%MAX_DRX_SIZE;
   dlNode = cell->drxCb[dlIndx].onDurationExpiryList.first;
   if(dlNode)
   {
      /* Handling of dl On duration drx start list */
      while(dlNode)
      {
         ueCb = (SchUeCb*)dlNode->node;
         dlNode = dlNode->next;
         if(ueCb->ueDrxInfoPres)
         {
            ueCb->drxUeCb.onDurationExpiryDistance--;

            if(ueCb->drxUeCb.onDurationExpiryDistance== -1)
            {

               DU_LOG("\nONDURATION EXP HIT %d[%d:%d]",dlIndx, cell->slotInfo.sfn, cell->slotInfo.slot);

               /* delete on duration expiry timer from ueCb */
               tmrIdx = ueCb->drxUeCb.onDurationExpiryIndex;
               CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].onDurationExpiryList, tmpNode);
               if(tmpNode)
               {
                  cmLListDelFrm(&cell->drxCb[tmrIdx].onDurationExpiryList, tmpNode);
                  SCH_FREE(tmpNode, sizeof(CmLList));
                  ueCb->drxUeCb.onDurationExpiryIndex = 0;
               }
               ueCb->drxUeCb.drxDlUeActiveStatus = false;
            }
            else
               continue;
         }
      }
   }
}


/**
 * @brief Handling of the expiry in active DRX timers
 *
 * @details
 *
 *     Function :schHdlDrxInActvExpiryTimer 
 *
 *      Handling of expiry in active  DRX timers
 *
 *  @param[in] SchCellCb  *cell
 *  @return
 *      -# ROK
 *      -# RFAILED
 **/
void schHdlDrxInActvExpiryTimer(SchCellCb  *cell)
{
   uint16_t dlIndx = 0, ulIndx = 0, tmrIdx = 0;
   SlotTimingInfo dlSlotInfo, ulSlotInfo;
   CmLList  *dlNode, *tmpNode;
   SchUeCb *ueCb = NULLP;

   ADD_DELTA_TO_TIME(cell->slotInfo, dlSlotInfo, PHY_DELTA_DL + SCHED_DELTA);
   ADD_DELTA_TO_TIME(cell->slotInfo, ulSlotInfo, PHY_DELTA_UL + SCHED_DELTA);

   dlIndx = (dlSlotInfo.sfn*MAX_SLOTS+dlSlotInfo.slot)%MAX_DRX_SIZE;
   dlNode = cell->drxCb[dlIndx].inActvTmrExpiryList.first;
   if(dlNode)
   {
      /* Handling of dl On duration drx start list */
      while(dlNode)
      {
         ueCb = (SchUeCb*)dlNode->node;
         dlNode = dlNode->next;
         ueCb->drxUeCb.inActiveTmrExpiryDistance--;

         if(ueCb->drxUeCb.onDurationExpiryDistance== -1)
         {
            DU_LOG("\nIN ACTV EXP HIT %d[%d:%d]",dlIndx, cell->slotInfo.sfn, cell->slotInfo.slot);
            /* Set the UE as INACTIVE */
            /*Delete the current entry from list */
            tmrIdx = ueCb->drxUeCb.inActvExpiryIndex;
            CM_LLIST_FIRST_NODE(&cell->drxCb[tmrIdx].inActvTmrExpiryList, tmpNode);
            if(tmpNode)
            {
               cmLListDelFrm(&cell->drxCb[tmrIdx].inActvTmrExpiryList, tmpNode);
               SCH_FREE(tmpNode, sizeof(CmLList));
            }
            
            /* If short cycle is configured then start the short cycle timer */
            if(ueCb->drxUeCb.shortCyclePresent)
            {
               schHdlDrxStartShortCycleTimer(cell, ueCb);
            }
            else
            {
               ueCb->drxUeCb.longCycleToBeUsed = true;
            }
            /* Set the drx ue status as inactive once the in active timer gets
             * expire */
            ueCb->drxUeCb.drxDlUeActiveStatus = false;
         }
         else
            continue;
      }
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
   /* Handling the onduration start timer */
   schHdlDrxShortCycleExpiryTimer(cell);
   schHdlDrxOnDurExpiryTimer(cell);
   schHdlDrxInActvExpiryTimer(cell);
}


#endif
/**********************************************************************
  End of file
 **********************************************************************/
