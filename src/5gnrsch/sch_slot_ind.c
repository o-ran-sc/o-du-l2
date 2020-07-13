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
#include "common_def.h"

SchMacDlAllocFunc schMacDlAllocOpts[] =
{
	packSchMacDlAlloc,
	MacProcDlAlloc,
	packSchMacDlAlloc
};

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
int sendDlAllocToMac(DlSchedInfo *dlSchedInfo, Inst inst)
{
	Pst pst;

   memset(&pst, 0, sizeof(Pst));
   SCH_FILL_RSP_PST(pst, inst);
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

   ADD_DELTA_TO_TIME(slotInd,schSlotValue->currentTime,PHY_DELTA);
   ADD_DELTA_TO_TIME(slotInd,schSlotValue->broadcastTime,PHY_DELTA+SCHED_DELTA);
   ADD_DELTA_TO_TIME(slotInd,schSlotValue->rarTime,PHY_DELTA+SCHED_DELTA);
   ADD_DELTA_TO_TIME(slotInd,schSlotValue->msg4Time,PHY_DELTA+SCHED_DELTA);
   ADD_DELTA_TO_TIME(slotInd,schSlotValue->dlMsgTime,PHY_DELTA+SCHED_DELTA+BO_DELTA);
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
   int ret = ROK;
	uint8_t ssb_rep;
	uint16_t slot, sfnSlot = 0;
	DlSchedInfo dlSchedInfo;
	memset(&dlSchedInfo,0,sizeof(DlSchedInfo));
   DlBrdcstAlloc *dlBrdcstAlloc = &dlSchedInfo.brdcstAlloc;
	RarAlloc  *rarAlloc;
   Msg4Alloc *msg4Alloc;
	dlBrdcstAlloc->ssbTrans = NO_SSB;
   dlBrdcstAlloc->sib1Trans = NO_SIB1;
	
	SchCellCb *cell = schCb[schInst].cells[schInst];

   schCalcSlotValues(*slotInd, &dlSchedInfo.schSlotValue);

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
         RETVALUE(ret);
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
	    SCH_ALLOC(msg4Alloc, sizeof(Msg4Alloc));
		 if(!msg4Alloc)
		 {
		    DU_LOG("\nMAC: Memory Allocation failed for msg4 alloc");
			 return RFAILED;
		 }
		 
		 dlSchedInfo.msg4Alloc = msg4Alloc;

       /* Msg4 info is copied, this was earlier filled in macSchDlRlcBoInfo */
       memcpy(&msg4Alloc->msg4Info, cell->schDlSlotInfo[slot]->msg4Info, \
          sizeof(Msg4Info));
             
       /* pdcch and pdsch data is filled */
       schDlRsrcAllocMsg4(msg4Alloc, cell, slot); 
		 SCH_FREE(cell->schDlSlotInfo[slot]->msg4Info, sizeof(Msg4Info));
		 cell->schDlSlotInfo[slot]->msg4Info = NULL;
   }


	/* send msg to MAC */
   ret = sendDlAllocToMac(&dlSchedInfo, schInst);
   if(ret != ROK)
   {
      DU_LOG("\nSending DL Broadcast allocation from SCH to MAC failed");
      RETVALUE(ret);
   }
    
	schUlResAlloc(cell, schInst);

	memset(cell->schDlSlotInfo[slot], 0, sizeof(SchDlSlotInfo));

	return ret;
}

/**********************************************************************
  End of file
 **********************************************************************/


