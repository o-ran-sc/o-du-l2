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

SchMacDlBrdcstAllocFunc schMacDlBrdcstAllocOpts[] =
{
	packSchMacDlBrdcstAlloc,
	MacProcDlBrdcstAlloc,
	packSchMacDlBrdcstAlloc
};

extern SchCb schCb[SCH_MAX_INST];


/*******************************************************************
 *
 * @brief Handles sending DL broadcast alloc to MAC 
 *
 * @details
 *
 *    Function : sendDlBrdcstAllocToMac
 *
 *    Functionality:
 *     Sends DL Broadcast Resource Allocation to MAC from SCH
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int sendDlBrdcstAllocToMac(DlBrdcstAlloc *dlBrdcstAlloc, Inst inst)
{
	Pst pst;

   memset(&pst, 0, sizeof(Pst));
   SCH_FILL_RSP_PST(pst, inst);
	pst.event = EVENT_DL_BRDCST_ALLOC;

	return(*schMacDlBrdcstAllocOpts[pst.selector])(&pst, dlBrdcstAlloc);

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
int schProcessSlotInd(SlotIndInfo *slotInd, Inst schInst)
{
   int ret = ROK;
	uint8_t ssb_rep;
	DlBrdcstAlloc dlBrdcstAlloc;
	dlBrdcstAlloc.ssbTrans = NO_SSB;
	SchCellCb *cell;

#ifdef LTE_L2_MEAS
   glblTtiCnt++;
#endif
  
	cell = schCb[schInst].cells[schInst];
	ssb_rep = cell->cellCfg.ssbPeriod;
	memcpy(&cell->slotInfo, slotInd, sizeof(SlotIndInfo));
   memcpy(&dlBrdcstAlloc.slotIndInfo, slotInd, sizeof(SlotIndInfo));
	dlBrdcstAlloc.cellId = cell->cellId;
	dlBrdcstAlloc.ssbIdxSupported = 1;

	/* Identify SSB ocassion*/
	if ((slotInd->sfn % SCH_MIB_TRANS == 0))// && (slotInd.slot == 0))
	{
		dlBrdcstAlloc.ssbTrans = SSB_TRANSMISSION;
	}
	if ((slotInd->sfn % ssb_rep == 0))// && (slotInd.slot == 0))
	{
		dlBrdcstAlloc.ssbTrans = SSB_REPEAT;
	}
	//call file to allocate resource and then send response to MAC
	schCmnDlAlloc(cell, &dlBrdcstAlloc);
	//send msg to MAC
   ret = sendDlBrdcstAllocToMac(&dlBrdcstAlloc, schInst);
   if(ret != ROK)
   {
      DU_LOG("\nSending DL Broadcast allocation from SCH to MAC failed");
      RETVALUE(ret);
   }
	return ret;
}

/**********************************************************************
  End of file
 **********************************************************************/


