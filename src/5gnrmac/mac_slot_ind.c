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
#include "lwr_mac.h"
#include "lwr_mac_fsm.h"
#include "mac_utils.h"
#include "mac_harq_dl.h"
#include "lwr_mac_phy.h"

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
   uint8_t   schInfoIdx = 0, cwIdx = 0;
   uint8_t   ueId = 0, ueIdx = 0;
   uint16_t  cellIdx = 0;
   uint8_t   *retxTb = NULLP, *txPdu = NULLP;
   uint16_t  txPduLen = 0;
   MacDlSlot      *currDlSlot = NULLP;
   DlMsgSchInfo   *schedInfo;
   DlHarqProcCb   *hqProcCb = NULLP;

#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTSCH -> ENTMAC : EVENT_DL_SCH_INFO\n");
#endif
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

      for(ueIdx=0; ueIdx<MAX_NUM_UE; ueIdx++)
      {
         if(dlSchedInfo->rarAlloc[ueIdx] != NULLP)
         {
            currDlSlot = &macCb.macCell[cellIdx]->dlSlot[dlSchedInfo->schSlotValue.rarTime.slot];
            currDlSlot->dlInfo.rarAlloc[ueIdx] = dlSchedInfo->rarAlloc[ueIdx];

            /* MUXing of RAR */
            fillRarPdu(&currDlSlot->dlInfo.rarAlloc[ueIdx]->rarInfo);
         }

         if(dlSchedInfo->dlMsgAlloc[ueIdx] != NULLP)
         {
            currDlSlot = &macCb.macCell[cellIdx]->\
                         dlSlot[dlSchedInfo->schSlotValue.dlMsgTime.slot];
            currDlSlot->dlInfo.dlMsgAlloc[ueIdx] = dlSchedInfo->dlMsgAlloc[ueIdx]; /* copy msg4 alloc pointer in MAC slot info */
            currDlSlot->dlInfo.cellId = dlSchedInfo->cellId;
            
            if(dlSchedInfo->dlMsgAlloc[ueIdx]->dlMsgPdcchCfg && dlSchedInfo->dlMsgAlloc[ueIdx]->dlMsgPdcchCfg->coresetCfg.coreSetType == CORESET_TYPE0)
            {
               MAC_ALLOC(macCb.macCell[cellIdx]->macRaCb[ueIdx].macMsg4Status, sizeof(bool));
            }

            /* Check if the downlink pdu is msg4 */
            if((macCb.macCell[cellIdx]->macRaCb[ueIdx].macMsg4Status))
            {
               GET_UE_ID(dlSchedInfo->dlMsgAlloc[ueIdx]->crnti, ueId);
               ueIdx = ueId -1;
               schedInfo = dlSchedInfo->dlMsgAlloc[ueIdx];
               hqProcCb = &macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4HqInfo;

               if(!dlSchedInfo->dlMsgAlloc[ueIdx]->transportBlock[0].ndi)
               {
                  /* First transmission of MSG4 */
                  hqProcCb->procId = schedInfo->harqProcNum;
                  for(cwIdx = 0; cwIdx < schedInfo->dlMsgPdschCfg->numCodewords; cwIdx++)
                  {
                     memcpy(&hqProcCb->tbInfo[hqProcCb->numTb].txTime, &dlSchedInfo->schSlotValue.dlMsgTime, \
                           sizeof(SlotTimingInfo));
                     hqProcCb->tbInfo[hqProcCb->numTb].tbSize = schedInfo->dlMsgPdschCfg->codeword[cwIdx].tbSize;
                     hqProcCb->numTb++;
                  }
               }
               else
               {
                  /* MSG4 retransmission */
                  if(hqProcCb->procId == schedInfo->harqProcNum)
                  {
                     memcpy(&hqProcCb->tbInfo[0].txTime, &dlSchedInfo->schSlotValue.dlMsgTime, \
                           sizeof(SlotTimingInfo));
                  }
               }
            }
            else
            {
               memcpy(&currDlSlot->dlInfo.schSlotValue, &dlSchedInfo->schSlotValue, sizeof(SchSlotValue));

               if(!dlSchedInfo->dlMsgAlloc[ueIdx]->transportBlock[0].ndi)
               {
                  /* If new data transmission is scheduled, send schedule results to RLC */
                  if(dlSchedInfo->dlMsgAlloc[ueIdx]->dlMsgPdschCfg) 
                  {
                     sendSchedRptToRlc(currDlSlot->dlInfo, dlSchedInfo->schSlotValue.dlMsgTime, ueIdx, schInfoIdx);

                     /* Add HARQ Proc to DL HARQ Proc Entity in UE */
                     addDlHqProcInUe(currDlSlot->dlInfo.schSlotValue.dlMsgTime, &macCb.macCell[cellIdx]->ueCb[ueIdx], \
                           dlSchedInfo->dlMsgAlloc[ueIdx]);
                  }
               }
               else
               {
                  /* For retransmission, fetch PDU to be retransmitted from DL HARQ entity and schedule on corresponding slot */

                  /* As of now this loop will run only once for one TB. 
                   * TODO : update handling of fetched TB appropriately when support for two TB is added 
                   */
                  for(cwIdx = 0; cwIdx < dlSchedInfo->dlMsgAlloc[ueIdx]->dlMsgPdschCfg->numCodewords; cwIdx++)
                  {
                     /* Fetch TB to be retransmitted */
                     txPduLen = dlSchedInfo->dlMsgAlloc[ueIdx]->dlMsgPdschCfg->codeword[cwIdx].tbSize;
                     retxTb = fetchTbfromDlHarqProc(currDlSlot->dlInfo.schSlotValue.dlMsgTime, \
                           &macCb.macCell[cellIdx]->ueCb[ueIdx], \
                           dlSchedInfo->dlMsgAlloc[ueIdx]->harqProcNum, txPduLen);

                     /* Store PDU in corresponding DL slot */
                     MAC_ALLOC(txPdu, txPduLen);
                     if(!txPdu)
                     {
                        DU_LOG("\nERROR  -->  MAC : Memory allocation failed in MacProcDlAlloc");
                        return RFAILED;
                     }   
                     memcpy(txPdu, retxTb,  txPduLen);

                     currDlSlot->dlInfo.dlMsgAlloc[ueIdx]->dlMsgPduLen = txPduLen;
                     currDlSlot->dlInfo.dlMsgAlloc[ueIdx]->dlMsgPdu = txPdu;
                  }
               }
            }
         }
      }

      if(dlSchedInfo->ulGrant != NULLP)
      {
         currDlSlot = &macCb.macCell[cellIdx]->dlSlot[dlSchedInfo->schSlotValue.ulDciTime.slot];
         currDlSlot->dlInfo.ulGrant = dlSchedInfo->ulGrant;
      }
   }
   return ROK;
}

/**
 * @brief process DL Paging allocation from scheduler
 *
 * @details
 *
 *     Function : MacProcDlPageAlloc 
 *      
 *      This function copied dl Pag info in the mac slot info
 *           
 *  @param[in]  Pst            *pst
 *  @param[in]  DL Paging allocation from scheduler
 *  @return
 *      -# ROK 
 *      -# RFAILED 
 **/
uint8_t MacProcDlPageAlloc(Pst *pst, DlPageAlloc *dlPageAlloc)
{
   uint16_t  cellIdx = 0;
   MacDlSlot *currDlSlot = NULLP;

#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTSCH -> ENTMAC : EVENT_DL_PAGING_ALLOC\n");
#endif
   if(dlPageAlloc != NULLP)
   {
      GET_CELL_IDX(dlPageAlloc->cellId, cellIdx);

      currDlSlot = &macCb.macCell[cellIdx]->dlSlot[dlPageAlloc->dlPageTime.slot];
      MAC_ALLOC(currDlSlot->pageAllocInfo, sizeof(DlPageAlloc));
      if(currDlSlot->pageAllocInfo == NULLP)
      {
         DU_LOG("\nERROR  --> MAC : MacProcDlPageAlloc : Memory Allocation is failed!");
         return RFAILED;
      }
      memcpy(currDlSlot->pageAllocInfo, dlPageAlloc, sizeof(DlPageAlloc));
   }
   else
   {
      DU_LOG("\nERROR  --> MAC : DL Paging Allocation is failed!");
      return RFAILED;
   }
   return ROK;
}

/**
 * @brief Forming and filling the MUX Pdu
 * @details
 *
 *     Function : fillMsg4Pdu
 * 
 *      Forming and filling of Msg4Pdu
 *           
 *  @param[in]  DlMsgAlloc  *msg4Alloc
 *  @return  void
 **/
void fillMsg4Pdu(uint16_t cellId, DlMsgSchInfo *msg4SchInfo)
{
   uint8_t   ueId = 0, ueIdx = 0;
   uint16_t  cellIdx;
   uint16_t  msg4TxPduLen;
   MacDlData msg4DlData;
   MacCeInfo  macCeData;
   DlHarqProcCb *hqProcCb;

   GET_CELL_IDX(cellId, cellIdx);

   memset(&msg4DlData, 0, sizeof(MacDlData));
   memset(&macCeData, 0, sizeof(MacCeInfo));

   GET_UE_ID(msg4SchInfo->crnti, ueId);
   ueIdx = ueId -1;

   if(macCb.macCell[cellIdx] == NULLP)
   {
      DU_LOG("\nERROR -->  MAC: Cell Id[%d] not found", cellId);
      return;
   }

   hqProcCb = &macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4HqInfo;
   msg4TxPduLen = hqProcCb->tbInfo[0].tbSize - TX_PAYLOAD_HDR_LEN;

   if(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4Pdu != NULLP)
   {
      MAC_ALLOC(msg4DlData.pduInfo[msg4DlData.numPdu].dlPdu, macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4PduLen);
      if(msg4DlData.pduInfo[msg4DlData.numPdu].dlPdu != NULLP)
      {
         fillMsg4DlData(&msg4DlData, macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4PduLen, \
            macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4Pdu);
         fillMacCe(&macCeData, macCb.macCell[cellIdx]->macRaCb[ueIdx].msg3Pdu);

         /* Forming Mux Pdu */
         hqProcCb->tbInfo[0].tb = NULLP;
         MAC_ALLOC(hqProcCb->tbInfo[0].tb, msg4TxPduLen);
         if(hqProcCb->tbInfo[0].tb != NULLP)
         {
            memset(hqProcCb->tbInfo[0].tb, 0, msg4TxPduLen);
            macMuxPdu(&msg4DlData, &macCeData, hqProcCb->tbInfo[0].tb, msg4TxPduLen);
         }
         else
         {
            DU_LOG("\nERROR  -->  MAC: Failed allocating memory for msg4TxPdu");
         }
         /* Free memory allocated */
         MAC_FREE(msg4DlData.pduInfo[msg4DlData.numPdu-1].dlPdu, macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4PduLen);
         MAC_FREE(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4Pdu, macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4PduLen);
         macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4Pdu = NULLP;
         macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4PduLen = 0;
         msg4DlData.numPdu--;

      }
   }

   /* storing msg4 Pdu in macDlSlot */
   if(hqProcCb->tbInfo[0].tb)
   {
      msg4SchInfo->dlMsgPduLen = msg4TxPduLen;
      MAC_ALLOC(msg4SchInfo->dlMsgPdu, msg4SchInfo->dlMsgPduLen);
      if(msg4SchInfo->dlMsgPdu != NULLP)
      {
         memcpy(msg4SchInfo->dlMsgPdu, hqProcCb->tbInfo[0].tb, \
               msg4SchInfo->dlMsgPduLen);
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Failed at fillMsg4Pdu()");
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
 *  @param[in]  SlotTimingInfo    *slotInd
 *  @return  void
 **/

void buildAndSendMuxPdu(SlotTimingInfo currTimingInfo)
{
   uint8_t   ueIdx;
   uint16_t  cellIdx;
   MacDlSlot *currDlSlot = NULLP;
   SlotTimingInfo muxTimingInfo;
   memset(&muxTimingInfo, 0, sizeof(SlotTimingInfo));

   GET_CELL_IDX(currTimingInfo.cellId, cellIdx);

   ADD_DELTA_TO_TIME(currTimingInfo, muxTimingInfo, PHY_DELTA_DL, macCb.macCell[cellIdx]->numOfSlots);
   currDlSlot = &macCb.macCell[cellIdx]->dlSlot[muxTimingInfo.slot];

   for(ueIdx=0; ueIdx<MAX_NUM_UE; ueIdx++)
   {
      if(currDlSlot->dlInfo.dlMsgAlloc[ueIdx])
      {
         if((macCb.macCell[cellIdx]->macRaCb[ueIdx].macMsg4Status)&& (currDlSlot->dlInfo.dlMsgAlloc[ueIdx]->dlMsgPdschCfg))
         {
            fillMsg4Pdu(currTimingInfo.cellId, currDlSlot->dlInfo.dlMsgAlloc[ueIdx]);
         }
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
 *  @param[in]  SlotTimingInfo    *slotInd
 *  @return  
 *      -# ROK 
 *      -# RFAILED 
 **/
int sendSlotIndMacToSch(SlotTimingInfo *slotInd)
{
   /* fill Pst structure to send to lwr_mac to MAC */
   Pst pst;

   FILL_PST_MAC_TO_SCH(pst, EVENT_SLOT_IND_TO_SCH);
   return(SchMessageRouter(&pst, (void *)slotInd));
}

/*******************************************************************
 *
 * @brief Send cell up indication to DU APP
 *
 * @details
 *
 *    Function : sendCellUpIndMacToDuApp
 *
 *    Functionality:
 *       Send cell up indication to DU APP
 *
 * @params[in] Cell Up indication info 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int sendCellUpIndMacToDuApp(uint16_t cellId)
{
   Pst pst;
   uint16_t ret;
   OduCellId *oduCellId;

   /*  Allocate sharable memory */
   MAC_ALLOC_SHRABL_BUF(oduCellId, sizeof(OduCellId));
   if(!oduCellId)
   {
      DU_LOG("\nERROR  -->  MAC : Memory allocation failed for cell up indication");
      return RFAILED;
   }
   oduCellId->cellId = cellId;

   /* Fill Pst */
   FILL_PST_MAC_TO_DUAPP(pst, EVENT_MAC_CELL_UP_IND);

   ret = MacDuAppCellUpInd(&pst, oduCellId);
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  MAC: Failed to send cell up indication to DU APP");
      MAC_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, oduCellId, sizeof(OduCellId));
   }

   return ret;
} /* sendCellUpIndMacToDuApp */

/*******************************************************************
 *
 * @brief Send slot indication to DU APP
 *
 * @details
 *
 *    Function : sendSlotIndToDuApp
 *
 *    Functionality:
 *       Send cell up indication to DU APP
 *
 * @params[in] Cell Up indication info 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sendSlotIndToDuApp(SlotTimingInfo *slotInd)
{
   Pst pst;
   uint16_t ret;
   SlotTimingInfo *slotIndInfo;

   /*  Allocate sharable memory */
   MAC_ALLOC_SHRABL_BUF(slotIndInfo, sizeof(SlotTimingInfo));
   if(!slotIndInfo)
   {
      DU_LOG("\nERROR  -->  MAC : Memory allocation failed for slot indication");
      return RFAILED;
   }
   memcpy(slotIndInfo, slotInd,sizeof(SlotTimingInfo));

   /* Fill Pst */
   FILL_PST_MAC_TO_DUAPP(pst, EVENT_MAC_SLOT_IND);

   ret = MacDuAppSlotInd(&pst, slotIndInfo);
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  MAC: Failed to send slot up indication to DU APP");
      MAC_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, slotIndInfo, sizeof(SlotTimingInfo));
   }

   return ret;
}
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
uint8_t macProcSlotInd(SlotTimingInfo slotInd)
{
   uint16_t  cellIdx = 0;

   GET_CELL_IDX(slotInd.cellId, cellIdx);
   
   if(macCb.macCell[cellIdx] == NULLP)
   {
      DU_LOG("ERROR  --> MAC : macProcSlotInd(): CellId[%d] does not exist. Error occurred at SFN [%d] Slot [%d]",\
      slotInd.cellId, slotInd.sfn, slotInd.slot);
      return RFAILED;
   }
   /* Store current time info */
   macCb.macCell[cellIdx]->currTime.cellId = slotInd.cellId;
   macCb.macCell[cellIdx]->currTime.slot = slotInd.slot;
   macCb.macCell[cellIdx]->currTime.sfn = slotInd.sfn;

   /* Mux Pdu for Msg4 */
   buildAndSendMuxPdu(slotInd);

   /* Trigger for DL TTI REQ */
   fillDlTtiReq(slotInd);

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
 *  @param[in]  SlotTimingInfo    *slotInd
 *  @return  
 *      -# ROK 
 *      -# RFAILED 
 **/
uint8_t fapiMacSlotInd(Pst *pst, SlotTimingInfo *slotInd)
{
   uint8_t               ret = ROK;
   uint16_t              cellIdx;
   volatile uint32_t     startTime=0;

#ifdef ODU_SLOT_IND_DEBUG_LOG
   DU_LOG("\nDEBUG  -->  MAC : Slot Indication received. [%d : %d]", slotInd->sfn, slotInd->slot);
#endif
   /*starting Task*/
   ODU_START_TASK(&startTime, PID_MAC_TTI_IND);
   gSlotCount++;

   if(gSlotCount == 1)
   {
	   GET_CELL_IDX(slotInd->cellId, cellIdx);
	   macCb.macCell[cellIdx]->state = CELL_STATE_UP;
   }

/* When testing L2 with Intel-L1, any changes specific to 
 * timer mode testing must be guarded under INTEL_TIMER_MODE*/
#ifndef INTEL_TIMER_MODE
   /* send slot indication to scheduler */
   ret = sendSlotIndMacToSch(slotInd);
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : Sending of slot ind msg from MAC to SCH failed");
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, slotInd, sizeof(SlotTimingInfo));
      return ret;
   }

   ret = macProcSlotInd(*slotInd);
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : macProcSlotInd failed");
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, slotInd, sizeof(SlotTimingInfo));
      return ret;
   }
#endif

   /*First Slot Ind is for CellUp. Any other Slot, will be notified to DUAPP as
    * SLOT_IND*/
   if(gSlotCount == 1)   
   {
      /* send cell up indication to du app */
      ret = sendCellUpIndMacToDuApp(slotInd->cellId);

   }
   else
   {
      /* send slot indication to du app */
      ret = sendSlotIndToDuApp(slotInd);
   }
   
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  MAC :Sending of slot ind msg from MAC to DU APP failed");
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, slotInd, sizeof(SlotTimingInfo));
      return ret;
   }

   /*stoping Task*/
   ODU_STOP_TASK(startTime, PID_MAC_TTI_IND);
   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, slotInd, sizeof(SlotTimingInfo));

#ifdef INTEL_WLS_MEM
   lwrMacCb.phySlotIndCntr++;
   if(lwrMacCb.phySlotIndCntr > WLS_MEM_FREE_PRD)
   {
      lwrMacCb.phySlotIndCntr = 1;
   }
   freeWlsBlockList(lwrMacCb.phySlotIndCntr - 1);
#endif

   return ret;
}  /* fapiMacSlotInd */

/**********************************************************************
  End of file
 **********************************************************************/

