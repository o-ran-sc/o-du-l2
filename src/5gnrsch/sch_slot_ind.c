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

Name:     5G NR SCH layer

Type:     C source file

Desc:     C source code for Entry point fucntions for slot indications

File:     sch_slot_ind.c

 **********************************************************************/

/** @file sch_slot_ind.c
  @brief This module processes slot indications
 */
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

SchMacDlAllocFunc schMacDlAllocOpts[] =
{
   packSchMacDlAlloc,
   MacProcDlAlloc,
   packSchMacDlAlloc
};

extern uint32_t gBoIndBitMap;
extern SchCb schCb[SCH_MAX_INST];
extern int schFillRar(RarAlloc *rarAlloc, uint16_t raRnti, uint16_t pci, uint8_t
      offsetPointA);

/*******************************************************************
 *
 * @brief Handles sending DL broadcast alloc to MAC 
 *
 * @details
 *
 *    Function : sendDlAllocToMac
 *
 *    Functionality:
 *     Sends DL Broadcast Resource Allocation to MAC from SCH
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sendDlAllocToMac(DlSchedInfo *dlSchedInfo, Inst inst)
{
   Pst pst;

   memset(&pst, 0, sizeof(Pst));
   FILL_PST_SCH_TO_MAC(pst, inst);
   pst.event = EVENT_DL_ALLOC;

   return(*schMacDlAllocOpts[pst.selector])(&pst, dlSchedInfo);

}


/*******************************************************************
 *
 * @brief Handles slot indication at SCH 
 *
 * @details
 *
 *    Function : schCalcSlotValues
 *
 *    Functionality:
 *     Handles TTI indication received from PHY
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void schCalcSlotValues(SlotIndInfo slotInd, SchSlotValue *schSlotValue)
{
   /****************************************************************
    * PHY_DELTA - the physical layer delta                         * 
    * SCHED_DELTA - scheduler schedules one slot ahead             *
    * BO_DELTA - this delay is considered for BO response and      *
    *            RLC buffer packet to received at MAC              *
    * lower-mac (FAPI filling) will be working on PHY_DELTA        *
    * brdcast scheduler will working on PHY_DELTA + SCHED_DELTA    *
    * RAR scheduler will working on PHY_DELTA + SCHED_DELTA        *
    * msg4 scheduler will working on PHY_DELTA + SCHED_DELTA       *
    * dedicated DL msg scheduler will working                      *
    *        on PHY_DELTA + SCHED_DELTA + BO_DELTA                 *
    ****************************************************************/

   ADD_DELTA_TO_TIME(slotInd, schSlotValue->currentTime, PHY_DELTA);
   ADD_DELTA_TO_TIME(slotInd, schSlotValue->broadcastTime, PHY_DELTA + SCHED_DELTA);
   ADD_DELTA_TO_TIME(slotInd, schSlotValue->rarTime, PHY_DELTA + SCHED_DELTA);
   ADD_DELTA_TO_TIME(slotInd, schSlotValue->msg4Time, PHY_DELTA + SCHED_DELTA);
   ADD_DELTA_TO_TIME(slotInd, schSlotValue->dlMsgTime, PHY_DELTA + SCHED_DELTA);
}

/*******************************************************************
 *
 * @brief Handles slot indication at SCH 
 *
 * @details
 *
 *    Function : schProcessSlotInd
 *
 *    Functionality:
 *     Handles TTI indication received from PHY
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t schProcessSlotInd(SlotIndInfo *slotInd, Inst schInst)
{
   uint8_t  ret = ROK;
   uint8_t  ssb_rep, ueIdx, lcIdx, lcgIdx;
   uint16_t slot, sfnSlot = 0;
   DlSchedInfo dlSchedInfo;
   DlBrdcstAlloc *dlBrdcstAlloc = NULLP;
   RarAlloc   *rarAlloc = NULLP;
   Msg4Alloc  *msg4Alloc = NULLP;
   DlMsgAlloc *dlMsgAlloc = NULLP;
   SchCellCb  *cell = NULLP;

   memset(&dlSchedInfo,0,sizeof(DlSchedInfo));
   schCalcSlotValues(*slotInd, &dlSchedInfo.schSlotValue);
   dlBrdcstAlloc = &dlSchedInfo.brdcstAlloc;
   dlBrdcstAlloc->ssbTrans = NO_SSB;
   dlBrdcstAlloc->sib1Trans = NO_SIB1;

   cell = schCb[schInst].cells[schInst];
   ssb_rep = cell->cellCfg.ssbSchCfg.ssbPeriod;
   memcpy(&cell->slotInfo, slotInd, sizeof(SlotIndInfo));
   dlBrdcstAlloc->ssbIdxSupported = 1;

   sfnSlot = ((dlSchedInfo.schSlotValue.broadcastTime.sfn * 10) +
	 dlSchedInfo.schSlotValue.broadcastTime.slot);

   slot = dlSchedInfo.schSlotValue.currentTime.slot;

   dlSchedInfo.cellId = cell->cellId;

   /* Identify SSB ocassion*/
   if (sfnSlot % SCH_MIB_TRANS == 0)
   {
      dlBrdcstAlloc->ssbTrans = SSB_TRANSMISSION;
   }
   else if (sfnSlot % ssb_rep == 0)
   {
      dlBrdcstAlloc->ssbTrans = SSB_REPEAT;
   }
   else
   {
      /* not SSB occassion */
   }

   /* Identify SIB1 occasions */
   if(sfnSlot % cell->cellCfg.sib1SchCfg.sib1NewTxPeriod == 0)
   {
      dlBrdcstAlloc->sib1Trans = SIB1_TRANSMISSION;
   }
   else if (sfnSlot % cell->cellCfg.sib1SchCfg.sib1RepetitionPeriod == 0)
   {
      dlBrdcstAlloc->sib1Trans = SIB1_REPITITION;
   }
   else
   {
      /* not SIB1 occassion */
   }

   if(dlBrdcstAlloc->ssbTrans || dlBrdcstAlloc->sib1Trans)
   {
      dlSchedInfo.isBroadcastPres = true;
      slot = dlSchedInfo.schSlotValue.broadcastTime.slot;
      ret = schBroadcastAlloc(cell,dlBrdcstAlloc,slot);
      if(ret != ROK)
      {
	 DU_LOG("\nschBroadcastAlloc failed");
	 return (ret);
      }
   }

   /* check for RAR */
   if(cell->schDlSlotInfo[dlSchedInfo.schSlotValue.rarTime.slot]->rarInfo != NULLP)
   {
      slot = dlSchedInfo.schSlotValue.rarTime.slot;
      SCH_ALLOC(rarAlloc, sizeof(RarAlloc));
      if(!rarAlloc)
      {
	 DU_LOG("\nMAC: Memory Allocation failed for RAR alloc");
	 return RFAILED;
      }

      dlSchedInfo.rarAlloc = rarAlloc;

      /* RAR info is copied, this was earlier filled in schProcessRachInd */
      memcpy(&rarAlloc->rarInfo,cell->schDlSlotInfo[slot]->rarInfo, sizeof(RarInfo));

      /* pdcch and pdsch data is filled */
      schFillRar(rarAlloc,
	    cell->schDlSlotInfo[slot]->rarInfo->raRnti,
	    cell->cellCfg.phyCellId,
	    cell->cellCfg.ssbSchCfg.ssbOffsetPointA);

      SCH_FREE(cell->schDlSlotInfo[slot]->rarInfo,sizeof(RarAlloc));
      cell->schDlSlotInfo[slot]->rarInfo = NULLP;
   }

   /* check for MSG4 */
   if(cell->schDlSlotInfo[dlSchedInfo.schSlotValue.msg4Time.slot]->msg4Info != NULLP)
   {
      slot = dlSchedInfo.schSlotValue.msg4Time.slot;
      DU_LOG("\nSCH: Current slot [%d]. MSG4 time slot [%d] *********", slotInd->slot, slot);

      SCH_ALLOC(msg4Alloc, sizeof(Msg4Alloc));
      if(!msg4Alloc)
      {
	 DU_LOG("\nMAC: Memory Allocation failed for msg4 alloc");
	 return RFAILED;
      }

      dlSchedInfo.msg4Alloc = msg4Alloc;

      /* Msg4 info is copied, this was earlier filled in macSchDlRlcBoInfo */
      memcpy(&msg4Alloc->msg4Info, cell->schDlSlotInfo[slot]->msg4Info, \
	    sizeof(DlMsgInfo));

      /* pdcch and pdsch data is filled */
      schDlRsrcAllocMsg4(msg4Alloc, cell, dlSchedInfo.schSlotValue.msg4Time.slot); 

      /* PUCCH resource */
      schAllocPucchResource(cell, msg4Alloc->msg4Info.crnti, dlSchedInfo.schSlotValue.msg4Time.slot);

      SCH_FREE(cell->schDlSlotInfo[dlSchedInfo.schSlotValue.msg4Time.slot]->msg4Info, sizeof(DlMsgInfo));
      cell->schDlSlotInfo[dlSchedInfo.schSlotValue.msg4Time.slot]->msg4Info = NULL;
   }
   /* check if UL grant must be sent in this slot for a SR/BSR that had been received */
   for(ueIdx=0; ueIdx<cell->numActvUe; ueIdx++)
   {
      uint32_t totDataReq = 0; /* in bytes */
      DciInfo  *dciInfo = NULLP;
      SchUeCb *ueCb = NULLP;
   
      ueCb = &cell->ueCb[ueIdx];
      /* check for SR */
      if(ueCb->srRcvd)
      {
         totDataReq = UL_GRANT_SIZE; /*fixing so that all control msgs can be handled in SR */
         ueCb->srRcvd = false;
      }
      /* check for BSR */
      for(lcgIdx=0; lcgIdx<MAX_NUM_LOGICAL_CHANNEL_GROUPS; lcgIdx++)
      {
        totDataReq+= ueCb->bsrInfo[lcgIdx].dataVol;
	ueCb->bsrInfo[lcgIdx].dataVol = 0;
      }
      if(totDataReq > 0) /* UL grant must be provided for this UE in this slot */
      {
         SchPuschInfo schPuschInfo;
         memset(&schPuschInfo, 0, sizeof(SchPuschInfo));

	 SCH_ALLOC(dciInfo, sizeof(DciInfo));
	 if(!dciInfo)
	 {
	    DU_LOG("\nMAC: Memory Allocation failed for dciInfo alloc");
	    return RFAILED;
	 }
	 memset(dciInfo,0,sizeof(DciInfo));
	 /* update the SFN and SLOT */
	 memcpy(&dlSchedInfo.schSlotValue.ulDciTime, slotInd, sizeof(SlotIndInfo));
	 slot = dlSchedInfo.schSlotValue.ulDciTime.slot;
	 /* Update PUSCH allocation */
	 schFillPuschAlloc(ueCb, slot, totDataReq, &schPuschInfo);
	 /* Fill DCI for UL grant */
	 schFillUlDci(ueCb, schPuschInfo, dciInfo);
	 memcpy(&dciInfo->slotIndInfo, &dlSchedInfo.schSlotValue.ulDciTime, sizeof(SlotIndInfo));
         dlSchedInfo.ulGrant = dciInfo;
      }
   }

   /* Check for pending BO grant for LC */
   while(gBoIndBitMap)
   {
      slot = dlSchedInfo.schSlotValue.dlMsgTime.slot;
      
      DU_LOG("\nSCH: Current slot [%d]. DL MSG time slot [%d] *********", slotInd->slot, slot);
      if(cell->schDlSlotInfo[slot]->dedMsgInfo == NULL)
      {
         break;
      }

      uint16_t crnti = 0;
      uint16_t accumalatedSize = 0;
      SchUeCb *ueCb = NULLP;

      GET_RIGHT_MOST_SET_BIT(gBoIndBitMap,ueIdx);
      GET_CRNTI(crnti,ueIdx);
      ueCb = &cell->ueCb[ueIdx-1];

      /* allocate PDCCH and PDSCH resources for the ue */
      SCH_ALLOC(dlMsgAlloc, sizeof(DlMsgAlloc));
      if(!dlMsgAlloc)
      {
	 DU_LOG("\nMAC: Memory Allocation failed for ded DL msg alloc");
	 return RFAILED;
      }
      memset(dlMsgAlloc, 0, sizeof(DlMsgAlloc));
      dlSchedInfo.dlMsgAlloc = dlMsgAlloc;
      dlMsgAlloc->crnti = crnti;

      /* Dl ded Msg info is copied, this was earlier filled in macSchDlRlcBoInfo */
       memcpy(&dlMsgAlloc->dedMsgInfo, cell->schDlSlotInfo[slot]->dedMsgInfo, \
             sizeof(DlMsgInfo));

      /* scheduled LC data fill */
      dlMsgAlloc->numLc = 0;
      for(lcIdx = 0; lcIdx < MAX_NUM_LC; lcIdx++)
      {
	 if(ueCb->dlLcCtxt[lcIdx].bo)
	 {
	    dlMsgAlloc->lcSchInfo[dlMsgAlloc->numLc].lcId = ueCb->dlLcCtxt[lcIdx].lcId;

	    /* calculation for BO includse RLC and MAC header size */
	    dlMsgAlloc->lcSchInfo[dlMsgAlloc->numLc].schBytes = \
               ueCb->dlLcCtxt[lcIdx].bo + RLC_HDR_SIZE + MAC_HDR_SIZE;
	    accumalatedSize += dlMsgAlloc->lcSchInfo[dlMsgAlloc->numLc].schBytes;
	    dlMsgAlloc->numLc++;
	 }
	 ueCb->dlLcCtxt[lcIdx].bo = 0;
      }

      /* pdcch and pdsch data is filled */
      schDlRsrcAllocDlMsg(dlMsgAlloc, cell, crnti, accumalatedSize, slot);

      /* Free the dl ded msg info allocated in macSchDlRlcBoInfo */ 
      SCH_FREE(cell->schDlSlotInfo[dlSchedInfo.schSlotValue.dlMsgTime.slot]->dedMsgInfo, \
         sizeof(DlMsgInfo));
      cell->schDlSlotInfo[dlSchedInfo.schSlotValue.dlMsgTime.slot]->dedMsgInfo = NULL;

      /* after allocation is done, unset the bo bit for that ue */
      UNSET_ONE_BIT(ueIdx,gBoIndBitMap);
   }

   /* send msg to MAC */
   ret = sendDlAllocToMac(&dlSchedInfo, schInst);
   if(ret != ROK)
   {
      DU_LOG("\nSending DL Broadcast allocation from SCH to MAC failed");
      return (ret);
   }

   schInitDlSlot(cell->schDlSlotInfo[slot]);
   schUlResAlloc(cell, schInst);


   return ret;
}

/**********************************************************************
  End of file
 **********************************************************************/


