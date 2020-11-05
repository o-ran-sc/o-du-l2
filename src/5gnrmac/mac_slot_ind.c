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
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "lwr_mac_upr_inf.h"
#include "rlc_mac_inf.h"
#include "mac.h"
#include "mac_upr_inf_api.h"
#include "lwr_mac_fsm.h"
#include "mac_utils.h"

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

      if(dlSchedInfo->dlMsgAlloc != NULLP)
      {
	 currDlSlot = &macCb.macCell[cellIdx]->\
	    dlSlot[dlSchedInfo->schSlotValue.dlMsgTime.slot];
	 currDlSlot->dlInfo.dlMsgAlloc = dlSchedInfo->dlMsgAlloc; /* copy msg4 alloc pointer in MAC slot info */
         currDlSlot->dlInfo.cellId = dlSchedInfo->cellId;

         /* Check if the downlink pdu is msg4 */
	 if(dlSchedInfo->dlMsgAlloc->dlMsgInfo.isMsg4Pdu)
	 {
	    macCb.macCell[cellIdx]->macRaCb[0].msg4TbSize = dlSchedInfo->dlMsgAlloc->dlMsgPdschCfg.codeword[0].tbSize;
	 }
	 else
	 {
	    memcpy(&currDlSlot->dlInfo.schSlotValue, &dlSchedInfo->schSlotValue, sizeof(SchSlotValue));
	    /* Send LC schedule result to RLC */
	    sendSchedRptToRlc(currDlSlot->dlInfo, dlSchedInfo->schSlotValue.dlMsgTime);
	 }
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
 *  @param[in]  DlMsgAlloc  *msg4Alloc
 *  @return  void
 **/
void fillMsg4Pdu(uint16_t cellId, DlMsgAlloc *msg4Alloc)
{
   uint8_t   ueIdx;
   uint16_t  cellIdx;
   MacDlData msg4DlData;
   MacCeInfo  macCeData;

   GET_CELL_IDX(cellId, cellIdx);

   memset(&msg4DlData, 0, sizeof(MacDlData));
   memset(&macCeData, 0, sizeof(MacCeInfo));

   GET_UE_IDX(msg4Alloc->dlMsgInfo.crnti, ueIdx);
   ueIdx = ueIdx -1;
   if(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4Pdu != NULLP)
   {
      MAC_ALLOC(msg4DlData.pduInfo[ueIdx].dlPdu, \
	    macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4PduLen);
      if(msg4DlData.pduInfo[ueIdx].dlPdu != NULLP)
      {
	 fillMsg4DlData(&msg4DlData, macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4PduLen,\
            macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4Pdu);
	 fillMacCe(&macCeData, macCb.macCell[cellIdx]->macRaCb[ueIdx].msg3Pdu);
	 /* Forming Mux Pdu */
	 macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TxPdu = NULLP;
	 MAC_ALLOC(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TxPdu, \
	    macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TbSize);
	 if(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TxPdu != NULLP)
	 {
	    memset(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TxPdu, 0, \
	       macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TbSize);
	    macMuxPdu(&msg4DlData, &macCeData, macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TxPdu,\
		  macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TbSize);

	 }
	 else
	 {
	    DU_LOG("\nMAC: Failed allocating memory for msg4TxPdu");
	 }
	 /* Free memory allocated */
	 MAC_FREE(msg4DlData.pduInfo[0].dlPdu, macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4PduLen);
      }
   }

   /* storing msg4 Pdu in macDlSlot */
   if(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TxPdu)
   {
      msg4Alloc->dlMsgInfo.dlMsgPduLen = macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TbSize;
      MAC_ALLOC(msg4Alloc->dlMsgInfo.dlMsgPdu, msg4Alloc->dlMsgInfo.dlMsgPduLen);
      if(msg4Alloc->dlMsgInfo.dlMsgPdu != NULLP)
      {
	 memcpy(msg4Alloc->dlMsgInfo.dlMsgPdu, macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TxPdu, \
	       msg4Alloc->dlMsgInfo.dlMsgPduLen);
      }
   }
   else
   {
      DU_LOG("\nMAC: Failed at macMuxPdu()");
   }
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
   if(currDlSlot->dlInfo.dlMsgAlloc)
   {
      if(currDlSlot->dlInfo.dlMsgAlloc->dlMsgInfo.isMsg4Pdu)
      {
         fillMsg4Pdu(currTimingInfo.cellId, currDlSlot->dlInfo.dlMsgAlloc);
         currDlSlot = NULLP;
      }
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
   uint8_t               ret = ROK;
   volatile uint32_t     startTime=0;

#ifdef ODU_SLOT_IND_DEBUG_LOG
   DU_LOG("\nMAC : Slot Indication received");
#endif
   /*starting Task*/
   ODU_START_TASK(&startTime, PID_MAC_TTI_IND);

   /* send slot indication to scheduler */
   ret = sendSlotIndMacToSch(slotInd);
   if(ret != ROK)
   {
      DU_LOG("\nMAC : Sending of slot ind msg from MAC to SCH failed");
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, slotInd, sizeof(SlotIndInfo));
      return ret;
   }

   ret = macProcSlotInd(*slotInd);
   if(ret != ROK)
   {
      DU_LOG("\nMAC : macProcSlotInd failed");
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, slotInd, sizeof(SlotIndInfo));
      return ret;
   }

   /* send slot indication to du app */
   ret = sendSlotIndMacToDuApp(slotInd);
   if(ret != ROK)
   {
      DU_LOG("\nMAC :Sending of slot ind msg from MAC to DU APP failed");
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, slotInd, sizeof(SlotIndInfo));
      return ret;
   }

   /*stoping Task*/
   ODU_STOP_TASK(startTime, PID_MAC_TTI_IND);
   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, slotInd, sizeof(SlotIndInfo));
   return ret;
}  /* fapiMacSlotInd */

/**********************************************************************
  End of file
 **********************************************************************/

