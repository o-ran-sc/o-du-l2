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
#include "common_def.h"
#include "lrg.h"
#include "lrg.x"
#include "rgu.h"
#include "rgu.x"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "lwr_mac_upr_inf.h"
#include "mac.h"
#include "mac_upr_inf_api.h"
#include "lwr_mac_fsm.h"
#include "mac_utils.h"

/* Function declarations */
extern uint16_t fillUlTtiReq(SlotIndInfo currTimingInfo);
extern uint16_t fillDlTtiReq(SlotIndInfo currTimingInfo);
extern uint16_t fillUlDciReq(SlotIndInfo currTimingInfo);

/* function pointers for packing slot ind from mac to sch */
MacSchSlotIndFunc macSchSlotIndOpts[] =
{
   packMacSchSlotInd,
   MacSchSlotInd,
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
 *  @return
 *      -# ROK 
 *      -# RFAILED 
 **/
uint8_t MacProcDlAlloc(Pst *pst, DlSchedInfo *dlSchedInfo)
{
   uint16_t  cellIdx;
   MacDlSlot *currDlSlot = NULLP;

   if(dlSchedInfo != NULLP)
   {
      GET_CELL_IDX(dlSchedInfo->cellId, cellIdx);
      if(dlSchedInfo->isBroadcastPres)
      {
	 currDlSlot = &macCb.macCell[cellIdx]->\
	    dlSlot[dlSchedInfo->schSlotValue.broadcastTime.slot];
	 currDlSlot->dlInfo.isBroadcastPres = true;
	 memcpy(&currDlSlot->dlInfo.brdcstAlloc, &dlSchedInfo->brdcstAlloc, sizeof(DlBrdcstAlloc));
      }

      if(dlSchedInfo->rarAlloc != NULLP)
      {
	 currDlSlot = &macCb.macCell[cellIdx]->\
	    dlSlot[dlSchedInfo->schSlotValue.rarTime.slot];
	 currDlSlot->dlInfo.rarAlloc = dlSchedInfo->rarAlloc;

	 /* MUXing of RAR */
	 fillRarPdu(&currDlSlot->dlInfo.rarAlloc->rarInfo);
      }

      if(dlSchedInfo->msg4Alloc != NULLP)
      {
	 Msg4Alloc *msg4Alloc = NULLP;
	 currDlSlot = &macCb.macCell[cellIdx]->\
	    dlSlot[dlSchedInfo->schSlotValue.msg4Time.slot];
	 currDlSlot->dlInfo.msg4Alloc = dlSchedInfo->msg4Alloc; /* copy msg4 alloc pointer in MAC slot info */
	 msg4Alloc = dlSchedInfo->msg4Alloc;
	 macCb.macCell[cellIdx]->macRaCb[0].msg4TbSize = msg4Alloc->msg4PdschCfg.codeword[0].tbSize;
      }
      if(dlSchedInfo->ulGrant != NULLP)
      {
	 currDlSlot = &macCb.macCell[cellIdx]->\
	    dlSlot[dlSchedInfo->schSlotValue.ulDciTime.slot];
         currDlSlot->dlInfo.ulGrant = dlSchedInfo->ulGrant;
      }
   }
   return ROK;
}

/**
 * @brief Forming anf filling the MUX Pdu
 * @details
 *
 *     Function : fillMsg4Pdu
 * 
 *      Forming and filling of Msg4Pdu
 *           
 *  @param[in]  Msg4Alloc  *msg4Alloc
 *  @return  void
 **/
void fillMsg4Pdu(uint16_t cellId, Msg4Alloc *msg4Alloc)
{
   uint16_t  cellIdx;
   MacDlData msg4DlData;
   MacCeInfo  macCeData;

   GET_CELL_IDX(cellId, cellIdx);

   memset(&msg4DlData, 0, sizeof(MacDlData));
   memset(&macCeData, 0, sizeof(MacCeInfo));

   if(macCb.macCell[cellIdx]->macRaCb[0].msg4Pdu != NULLP)
   {
      MAC_ALLOC(msg4DlData.pduInfo[0].dlPdu, \
	    macCb.macCell[cellIdx]->macRaCb[0].msg4PduLen);
      if(msg4DlData.pduInfo[0].dlPdu != NULLP)
      {
	 fillMsg4DlData(cellId, &msg4DlData, macCb.macCell[cellIdx]->macRaCb[0].msg4Pdu);
	 fillMacCe(&macCeData, macCb.macCell[cellIdx]->macRaCb[0].msg3Pdu);
	 /* Forming Mux Pdu */
	 macCb.macCell[cellIdx]->macRaCb[0].msg4TxPdu = NULLP;
	 MAC_ALLOC(macCb.macCell[cellIdx]->macRaCb[0].msg4TxPdu, \
	    macCb.macCell[cellIdx]->macRaCb[0].msg4TbSize);
	 if(macCb.macCell[cellIdx]->macRaCb[0].msg4TxPdu != NULLP)
	 {
	    memset(macCb.macCell[cellIdx]->macRaCb[0].msg4TxPdu, 0, \
	       macCb.macCell[cellIdx]->macRaCb[0].msg4TbSize);
	    macMuxPdu(&msg4DlData, &macCeData, macCb.macCell[cellIdx]->macRaCb[0].msg4TxPdu,\
		  macCb.macCell[cellIdx]->macRaCb[0].msg4TbSize);

	 }
	 else
	 {
	    DU_LOG("\nMAC: Failed allocating memory for msg4TxPdu");
	 }
	 /* Free memory allocated */
	 MAC_FREE(msg4DlData.pduInfo[0].dlPdu, macCb.macCell[cellIdx]->macRaCb[0].msg4PduLen);
      }
   }

   /* storing msg4 Pdu in macDlSlot */
   if(macCb.macCell[cellIdx]->macRaCb[0].msg4TxPdu)
   {
      msg4Alloc->msg4Info.msg4PduLen = macCb.macCell[cellIdx]->macRaCb[0].msg4TbSize;
      MAC_ALLOC(msg4Alloc->msg4Info.msg4Pdu, msg4Alloc->msg4Info.msg4PduLen);
      if(msg4Alloc->msg4Info.msg4Pdu != NULLP)
      {
	 memcpy(msg4Alloc->msg4Info.msg4Pdu, macCb.macCell[cellIdx]->macRaCb[0].msg4TxPdu, \
	       msg4Alloc->msg4Info.msg4PduLen);
      }
   }
   else
   {
      DU_LOG("\nMAC: Failed at macMuxPdu()");
   }
   /* TODO: Free all allocated memory, after the usage */
   /* MAC_FREE(macCb.macCell->macRaCb[0].msg4TxPdu, \
      macCb.macCell->macRaCb[0].msg4TbSize); // TODO: To be freed after re-transmission is successful.
      MAC_FREE(macCb.macCell->macRaCb[0].msg4Pdu, macCb.macCell->macRaCb[0].msg4PduLen); */
}

/**
 * @brief Builds and Send the Muxed Pdu to Lower MAC
 *
 * @details
 *
 *     Function : buildAndSendMuxPdu
 * 
 *      Build and Sends the Muxed Pdu to Lower MAC.
 *           
 *  @param[in]  SlotIndInfo    *slotInd
 *  @return  void
 **/

void buildAndSendMuxPdu(SlotIndInfo currTimingInfo)
{
   uint16_t  cellIdx;
   MacDlSlot *currDlSlot = NULLP;
   SlotIndInfo muxTimingInfo;
   memset(&muxTimingInfo, 0, sizeof(SlotIndInfo));

   GET_CELL_IDX(currTimingInfo.cellId, cellIdx);

   ADD_DELTA_TO_TIME(currTimingInfo, muxTimingInfo, PHY_DELTA);
   currDlSlot = &macCb.macCell[cellIdx]->dlSlot[muxTimingInfo.slot];
   if(currDlSlot->dlInfo.msg4Alloc)
   {
      fillMsg4Pdu(currTimingInfo.cellId, currDlSlot->dlInfo.msg4Alloc);
      currDlSlot = NULLP;
   }
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
 *  @return  
 *      -# ROK 
 *      -# RFAILED 
 **/
int sendSlotIndMacToSch(SlotIndInfo *slotInd)
{
   /* fill Pst structure to send to lwr_mac to MAC */
   Pst pst;

   FILL_PST_MAC_TO_SCH(pst, EVENT_SLOT_IND_TO_SCH);
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
   SlotIndInfo  *slotInfo;

   /*  Allocate sharable memory */
   MAC_ALLOC_SHRABL_BUF(slotInfo, sizeof(SlotIndInfo));
   if(!slotInfo)
   {
      DU_LOG("\nMAC : Slot Indication memory allocation failed");
      return RFAILED;
   }

   slotInfo->cellId = slotInd->cellId;
   slotInfo->sfn = slotInd->sfn;
   slotInfo->slot = slotInd->slot;

   /* Fill Pst */
   FILL_PST_MAC_TO_DUAPP(pst, EVENT_MAC_SLOT_IND);

   ret = MacDuAppSlotInd(&pst, slotInfo);
   if(ret != ROK)
   {
      DU_LOG("\nMAC: Failed to send slot indication to DU APP");
      MAC_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, slotInfo, sizeof(SlotIndInfo));
   }

   return ret;
} /* sendSlotIndMacToDuApp */

/*******************************************************************
 *
 * @brief Process slot indication at MAC
 *
 * @details
 *
 *    Function : macProcSlotInd
 *
 *    Functionality: Process slot indication at MAC
 *
 * @params[in] Slot indication info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t macProcSlotInd(SlotIndInfo slotInd)
{
   uint16_t  cellIdx;

   GET_CELL_IDX(slotInd.cellId, cellIdx);

   /* Store current time info */
   macCb.macCell[cellIdx]->currTime.cellId = slotInd.cellId;
   macCb.macCell[cellIdx]->currTime.slot = slotInd.slot;
   macCb.macCell[cellIdx]->currTime.sfn = slotInd.sfn;

   /* Mux Pdu for Msg4 */
   buildAndSendMuxPdu(slotInd);

   /* Trigger for DL TTI REQ */
   fillDlTtiReq(slotInd);

   /* Trigger for UL TTI REQ */
   fillUlTtiReq(slotInd);
   
   /* Trigger for UL DCI REQ */
   fillUlDciReq(slotInd);

   return ROK;
}  /* macProcSlotInd */

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
 *  @return  
 *      -# ROK 
 *      -# RFAILED 
 **/
uint8_t fapiMacSlotInd(Pst *pst, SlotIndInfo *slotInd)
{
   uint8_t               ret;
   VOLATILE uint32_t     startTime=0;

   DU_LOG("\nMAC : Slot Indication received");

   /*starting Task*/
   ODU_START_TASK(&startTime, PID_MAC_TTI_IND);

   /* send slot indication to scheduler */
   ret = sendSlotIndMacToSch(slotInd);
   if(ret != ROK)
   {
      DU_LOG("\nMAC : Sending of slot ind msg from MAC to SCH failed");
      return ret;
   }

   ret = macProcSlotInd(*slotInd);
   if(ret != ROK)
   {
      DU_LOG("\nMAC : macProcSlotInd failed");
      return ret;
   }

   /* send slot indication to du app */
   ret = sendSlotIndMacToDuApp(slotInd);
   if(ret != ROK)
   {
      DU_LOG("\nMAC :Sending of slot ind msg from MAC to DU APP failed");
      return ret;
   }

   /*stoping Task*/
   ODU_STOP_TASK(startTime, PID_MAC_TTI_IND);

   return ret;
}  /* fapiMacSlotInd */

/**********************************************************************
  End of file
 **********************************************************************/

