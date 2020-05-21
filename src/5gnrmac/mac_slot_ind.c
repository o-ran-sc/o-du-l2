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
/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_lte.h"        /* Common LTE Defines */
#include "rgu.h"
#include "tfu.h"           /* RGU Interface includes */
#include "lrg.h"
#include "crg.h"           /* layer management defines for LTE-MAC */
#include "rg_sch_inf.h"           /* layer management defines for LTE-MAC */
#include "rg_env.h"   
#include "rg.h"

#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"       /* common memory link list library */
#include "cm_lte.x"        /* Common LTE Defines */
#include "rgu.x"
#include "tfu.x"           /* RGU Interface includes */
#include "lrg.x"
#include "crg.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* SCH interface typedefs */
#include "rg_prg.x"
#include "du_app_mac_inf.h"
#include "mac.h"
#include "du_log.h"
#include "rg.x"
#include "mac_upr_inf_api.h"

/* function pointers for packing slot ind from mac to sch */
MacSchSlotIndFunc macSchSlotIndOpts[] =
{
   packMacSchSlotInd,
   macSchSlotInd,
   packMacSchSlotInd
};

/**
 * @brief process DL allocation from scheduler
 *
 * @details
 *
 *     Function : MacProcDlAlloc 
 *      
 *      This function copied dl sch info in the mac slot info
 *           
 *  @param[in]  Pst            *pst
 *  @param[in]  DL allocation from scheduler
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
int MacProcDlAlloc(Pst *pst, DlAlloc *dlAlloc)
{
   if(dlAlloc != NULLP)
   {
		MacDlSlot *currDlSlot =
		&macCb.macCell->dlSlot[dlAlloc->slotIndInfo.slot % MAX_SLOT_SUPPORTED];
      memcpy(&currDlSlot->dlInfo, dlAlloc, sizeof(DlAlloc)); 
   }
   return ROK;
}

/**
 * @brief Transmission time interval indication from PHY.
 *
 * @details
 *
 *     Function : sendSlotIndMacToSch
 * 
 *      This API is invoked by MAC to send slot ind to scheduler.
 *           
 *  @param[in]  SlotIndInfo    *slotInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
int sendSlotIndMacToSch(SlotIndInfo *slotInd)
{
   /* fill Pst structure to send to lwr_mac to MAC */
   Pst pst;

   fillMacToSchPst(&pst);
   pst.event = EVENT_SLOT_IND_TO_SCH;

   return(*macSchSlotIndOpts[pst.selector])(&pst,slotInd);
}

/*******************************************************************
 *
 * @brief Send slot indication to DU APP
 *
 * @details
 *
 *    Function : sendSlotIndMacToDuApp
 *
 *    Functionality:
 *       Send slot indication to DU APP
 *
 * @params[in] Slot indication info 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int sendSlotIndMacToDuApp(SlotIndInfo *slotInd)
{
   Pst pst;
   uint16_t ret;
   SlotInfo  *slotInfo;
 
   /*  Allocate sharable memory */
   MAC_ALLOC_SHRABL_BUF(slotInfo, sizeof(SlotInfo));
   if(!slotInfo)
   {
      DU_LOG("\nMAC : Slot Indication memory allocation failed");
      return RFAILED;
  }
 
  slotInfo->cellId = macCb.macCell->cellId;
  slotInfo->sfn = slotInd->sfn;
  slotInfo->slot = slotInd->slot;
 
  /* Fill Pst */
  pst.selector  = DU_MAC_LWLC;
  pst.srcEnt    = ENTRG;
  pst.dstEnt    = ENTDUAPP;
  pst.dstInst   = 0;
  pst.srcInst   = macCb.macInst;
  pst.dstProcId = rgCb[pst.srcInst].rgInit.procId;
  pst.srcProcId = rgCb[pst.srcInst].rgInit.procId;
  pst.region = MAC_MEM_REGION;
  pst.pool = MAC_POOL;
  pst.event = EVENT_MAC_SLOT_IND;
  pst.route = 0;
  pst.prior = 0;
  pst.intfVer = 0;
 
  ret = MacDuAppSlotInd(&pst, slotInfo);
  if(ret != ROK)
  {
     DU_LOG("\nMAC: Failed to send slot indication to DU APP");
     MAC_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, slotInfo, sizeof(SlotInfo));
  }
 
  return ret;
} /* sendSlotIndMacToDuApp */


/**
 * @brief Transmission time interval indication from PHY.
 *
 * @details
 *
 *     Function : fapiMacSlotInd 
 *      
 *      This API is invoked by PHY to indicate TTI indication to MAC for a cell.
 *           
 *  @param[in]  Pst            *pst
 *  @param[in]  SuId           suId 
 *  @param[in]  SlotIndInfo    *slotInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
PUBLIC S16 fapiMacSlotInd 
(
Pst                 *pst, 
SlotIndInfo         *slotInd
)
{
   S16              ret;
   VOLATILE U32     startTime=0;
   Inst             inst;

   DU_LOG("\nMAC : Slot Indication received");
   
   inst = pst->dstInst;
   /*starting Task*/
   SStartTask(&startTime, PID_MAC_TTI_IND);

   /* send slot indication to scheduler */
   ret = sendSlotIndMacToSch(slotInd);
   if(ret != ROK)
   {
      DU_LOG("\nMAC : Sending of slot ind msg from MAC to SCH failed");
      RETVALUE(ret);
   }

   ret = macProcessSlotInd(inst,*slotInd);
   if(ret != ROK)
   {
      DU_LOG("\nMAC : macProcessSlotInd failed");
      RETVALUE(ret);
   }

   /* send slot indication to du app */
   ret = sendSlotIndMacToDuApp(slotInd);
   if(ret != ROK)
   {
      DU_LOG("\nMAC :Sending of slot ind msg from MAC to DU APP failed");
      RETVALUE(ret);
   }

   /*stoping Task*/
   SStopTask(startTime, PID_MAC_TTI_IND);

   RETVALUE(ret);
}  /* fapiMacSlotInd */

/**********************************************************************
  End of file
 **********************************************************************/

