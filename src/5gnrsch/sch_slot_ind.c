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
#include "stdbool.h"
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_lte.h"        /* Common LTE Defines */
#include "tfu.h"
#include "lrg.h"

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"       /* common memory link list library */
#include "cm_lte.x"        /* Common LTE Defines */
#include "tfu.x"
#include "lrg.x"
#include "du_log.h"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "sch.h"

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
int sendDlAllocToMac(DlAlloc *dlAlloc, Inst inst)
{
	Pst pst;

   memset(&pst, 0, sizeof(Pst));
   SCH_FILL_RSP_PST(pst, inst);
	pst.event = EVENT_DL_ALLOC;

	return(*schMacDlAllocOpts[pst.selector])(&pst, dlAlloc);

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
	uint16_t sfn  = slotInd->sfn;
	uint16_t slot = slotInd->slot;
	uint16_t sfnSlot = 0;
	DlAlloc dlAlloc;
	memset(&dlAlloc,0,sizeof(DlAlloc));
   DlBrdcstAlloc *dlBrdcstAlloc = &dlAlloc.brdcstAlloc;
	RarAlloc *rarAlloc = &dlAlloc.rarAlloc;
	dlBrdcstAlloc->ssbTrans = NO_SSB;
   dlBrdcstAlloc->sib1Trans = NO_SIB1;
	

	SchCellCb *cell = schCb[schInst].cells[schInst];

#ifdef LTE_L2_MEAS
   glblTtiCnt++;
#endif
  
//   schDlResAlloc(cell, slotInd);

	ssb_rep = cell->cellCfg.ssbSchCfg.ssbPeriod;
	memcpy(&cell->slotInfo, slotInd, sizeof(SlotIndInfo));
	dlBrdcstAlloc->ssbIdxSupported = 1;

   if((slot + SCHED_DELTA) >= SCH_NUM_SLOTS)
	{
      sfn = (sfn+1)%SCH_MAX_SFN;
	}
	slot = ((slot + SCHED_DELTA) % SCH_NUM_SLOTS);
   sfnSlot = ((sfn * 10) + slot);

	dlAlloc.slotIndInfo.sfn = sfn;
	dlAlloc.slotIndInfo.slot = slot;
	dlAlloc.cellId = cell->cellId;

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
	   dlAlloc.isBroadcastPres = true;
	   ret = schBroadcastAlloc(cell, dlBrdcstAlloc,slot);
      if(ret != ROK)
      {
         DU_LOG("\nschBroadcastAlloc failed");
         RETVALUE(ret);
      }
   }

   /* check for RAR */
	if(cell->dlAlloc[slot]->rarPres == true)
	{
	   dlAlloc.isRarPres = true;
	   /* RAR info is copied, this was earlier filled in schProcessRachInd */
	   memcpy(&rarAlloc->rarInfo, &cell->dlAlloc[slot]->rarInfo, sizeof(RarInfo));

		/* pdcch and pdsch data is filled */
      schFillRar(rarAlloc,
		   cell->dlAlloc[slot]->rarInfo.raRnti,
		   cell->cellCfg.phyCellId,
		   cell->cellCfg.ssbSchCfg.ssbOffsetPointA);

     cell->dlAlloc[slot]->rarPres = false;
   }

	/* send msg to MAC */
   ret = sendDlAllocToMac(&dlAlloc, schInst);
   if(ret != ROK)
   {
      DU_LOG("\nSending DL Broadcast allocation from SCH to MAC failed");
      RETVALUE(ret);
   }
    
	schUlResAlloc(cell, schInst);

	return ret;
}

/**********************************************************************
  End of file
 **********************************************************************/


