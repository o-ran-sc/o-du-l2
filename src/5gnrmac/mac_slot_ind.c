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
#include "stdbool.h"
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
int MacProcDlAlloc(Pst *pst, DlSchedInfo *dlSchedInfo)
{
   MacDlSlot *currDlSlot = NULLP;

   if(dlSchedInfo != NULLP)
   {
		if(dlSchedInfo->isBroadcastPres)
		{
		   currDlSlot = &macCb.macCell->dlSlot[dlSchedInfo->schSlotValue.broadcastTime.slot];
		   currDlSlot->dlInfo.isBroadcastPres = true;
			memcpy(&currDlSlot->dlInfo.brdcstAlloc, &dlSchedInfo->brdcstAlloc, sizeof(DlBrdcstAlloc));
		}

		if(dlSchedInfo->rarAlloc != NULLP)
		{
		   currDlSlot = &macCb.macCell->dlSlot[dlSchedInfo->schSlotValue.rarTime.slot];
			currDlSlot->dlInfo.rarAlloc = dlSchedInfo->rarAlloc;

         /* MUXing of RAR */
			fillRarPdu(&currDlSlot->dlInfo.rarAlloc->rarInfo);
		}

		if(dlSchedInfo->msg4Alloc != NULLP)
      {
         Msg4Alloc *msg4Alloc = NULLP;
         MacDlData msg4DlData;
         MacCeInfo  macCeData;

		   currDlSlot = &macCb.macCell->dlSlot[dlSchedInfo->schSlotValue.msg4Time.slot];
			currDlSlot->dlInfo.msg4Alloc = dlSchedInfo->msg4Alloc; /* copy msg4 alloc pointer in MAC slot info */
			msg4Alloc = dlSchedInfo->msg4Alloc;

         memset(&msg4DlData, 0, sizeof(MacDlData));
         memset(&macCeData, 0, sizeof(MacCeInfo));

         macCb.macCell->macRaCb[0].msg4TbSize = msg4Alloc->msg4PdschCfg.codeword[0].tbSize;
 
         if(macCb.macCell->macRaCb[0].msg4Pdu != NULLP)
         {
            MAC_ALLOC(msg4DlData.pduInfo[0].dlPdu, \
              macCb.macCell->macRaCb[0].msg4PduLen);
            if(msg4DlData.pduInfo[0].dlPdu != NULLP)
            {
               fillMsg4DlData(&msg4DlData);
            }
         }

         /* MUXing for msg4 */
         fillMacCe(&macCeData);
         macMuxPdu(&msg4DlData, &macCeData, macCb.macCell->macRaCb[0].msg4TbSize);
      
         /* storing msg4 Pdu in macDlSlot */
         MAC_ALLOC(msg4Alloc->msg4Info.msg4Pdu, macCb.macCell->macRaCb[0].msg4PduLen);
         if(msg4Alloc->msg4Info.msg4Pdu != NULLP)
         {  
            msg4Alloc->msg4Info.msg4Pdu = macCb.macCell->macRaCb[0].msg4Pdu;
            msg4Alloc->msg4Info.msg4PduLen = macCb.macCell->macRaCb[0].msg4PduLen;
         }

         /* TODO: Free all allocated memory, after the usage */
         /* MAC_FREE(macCb.macCell->macRaCb[0].msg4TxPdu, \
              macCb.macCell->macRaCb[0].msg4TbSize); // TODO: To be freed after re-transmission is successful.
            MAC_FREE(dlSchedInfo->msg4Alloc->msg4Info.msg4Pdu,\
              macCb.macCell->macRaCb[0].msg4PduLen); //TODO: To be freed after lower-mac is succesfull
            MAC_FREE(msg4DlData.pduInfo[0].dlPdu, macCb.macCell->macRaCb[0].msg4PduLen);
            MAC_FREE(macCb.macCell->macRaCb[0].msg4Pdu, macCb.macCell->macRaCb[0].msg4PduLen); */
      }
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

