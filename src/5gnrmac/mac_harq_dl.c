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
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "lwr_mac_upr_inf.h"
#include "mac.h"
#include "mac_utils.h"
#include "mac_harq_dl.h"

/**
 * @brief Add HARQ process to UE's DL HARQ Entity
 *
 * @details
 *
 *     Function : addDlHqProcInUe
 *      
 *      This functions adds HARQ process to UE's DL HARQ entity
 *           
 *  @param[in]  Time of transmission on this HARQ process
 *  @param[in]  UE Cb
 *  @param[in]  DL Scheduling Information
 *  @return  
 *      -# Void
 **/
void addDlHqProcInUe(SlotTimingInfo dlMsgTime, MacUeCb *ueCb, DlMsgSchInfo schedInfo)
{
   uint8_t       hqProcIdx = 0, tbIdx = 0, cwIdx = 0;
   bool          hqProcFound = false;
   DlHarqEnt     *dlHqEnt = NULLP;
   DlHarqProcCb  *hqProcCb = NULLP;

   /* Check if harqProcId is already present in UE's DL HARQ Entity */
   dlHqEnt = &ueCb->dlInfo.dlHarqEnt;  
   for(hqProcIdx = 0; hqProcIdx < dlHqEnt->numHarqProcs; hqProcIdx++)
   {
      if(dlHqEnt->harqProcCb[hqProcIdx].procId == schedInfo.dlMsgInfo.harqProcNum)
      {
         /* Expected Behaviour:
          * If a HARQ proc is already present in DL HARQ entity, it means this HARQ proc 
          * is not free and SCH must not schedule on this process.
          *
          * Corner Case (occured if this line is hit):
          * HARQ proc is present in DL HARQ entity but SCH has scheduled a new data transmission on it.
          *
          * Action:
          * Free the process and schedule new data on it
          */
         hqProcCb = &dlHqEnt->harqProcCb[hqProcIdx];
         for(tbIdx = 0; tbIdx < hqProcCb->numTb; tbIdx++)
         {
            MAC_FREE(hqProcCb->tbInfo[tbIdx].tb, hqProcCb->tbInfo[tbIdx].tbSize);
         }
         memset(hqProcCb, 0, sizeof(DlHarqProcCb));
         hqProcFound = true;
         break;
      }
   }

   /* If HARQ proc is not found, add to DL HARQ Entity */
   if(!hqProcFound)
   {
      hqProcCb = &dlHqEnt->harqProcCb[dlHqEnt->numHarqProcs++];
   }

   /* Fill HARQ Proc Cb */
   hqProcCb->procId = schedInfo.dlMsgInfo.harqProcNum;
   for(cwIdx = 0; cwIdx < schedInfo.dlMsgPdschCfg.numCodewords; cwIdx++)
   {
      memcpy(&hqProcCb->tbInfo[hqProcCb->numTb].txTime, &dlMsgTime, sizeof(SlotTimingInfo));
      hqProcCb->tbInfo[hqProcCb->numTb].tbSize = schedInfo.dlMsgPdschCfg.codeword[cwIdx].tbSize;
      hqProcCb->numTb++;
   }
   return;
}

/**
 * @brief Adds multiplexes TB to DL HARQ Process Info
 *
 * @details
 *
 *     Function : updateNewTbInDlHqProcCb
 *     
 *      This function adds multiplxed TB to DL HARQ process.
 *      It will be used in case retransmission is required.
 *           
 *  @param[in]  Time on which TB will be transmitted
 *  @param[in]  UE CB
 *  @param[in]  Transport Block
 *  @return  
 *      -# ROK 
 *      -# RFAILED 
 **/
uint8_t updateNewTbInDlHqProcCb(SlotTimingInfo slotInfo, MacUeCb *ueCb, uint32_t tbSize, uint8_t *txPdu)
{
   uint8_t hqProcIdx = 0, tbIdx = 0;
   bool    tbUpdated = false;
   DlHarqEnt  *dlHqEnt = NULLP;
   DlTbInfo   *tbInfo = NULLP;

   dlHqEnt = &ueCb->dlInfo.dlHarqEnt;

   /* Search HARQ Proc Cb in DL HARQ Ent */
   for(hqProcIdx = 0; hqProcIdx < dlHqEnt->numHarqProcs; hqProcIdx++)
   {
      /* Search TB Info in a HARQ Proc Cb */
      for(tbIdx =0; tbIdx < dlHqEnt->harqProcCb[hqProcIdx].numTb; dlHqEnt++)
      {
         /* Store MAC PDU if a harqProcCb->tbInfo is found with 
          * a. same SFN/Slot on which incoming RLC DL Data is to be scheduled
          * b. same TB size as MAC PDU size
          */
         if((dlHqEnt->harqProcCb[hqProcIdx].tbInfo[tbIdx].txTime.sfn == slotInfo.sfn) &&
               (dlHqEnt->harqProcCb[hqProcIdx].tbInfo[tbIdx].txTime.slot == slotInfo.slot) &&
               (dlHqEnt->harqProcCb[hqProcIdx].tbInfo[tbIdx].tbSize == tbSize))
         {
            tbInfo = &dlHqEnt->harqProcCb[hqProcIdx].tbInfo[tbIdx];
            MAC_ALLOC(tbInfo->tb, tbSize);
            if(!tbInfo->tb)
            {
               DU_LOG("\nERROR  -->  MAC : Failed to allocate memory for TB in updateTbInDlHqProcCb");
               return RFAILED;
            }
            memcpy(tbInfo->tb, txPdu, tbSize);
            tbUpdated = true;
            break;
         }
      }

      if(tbUpdated)
         break;
   }
   return ROK;
}

/**
 * @brief Returns a transmission block from HARQ process Cb
 *
 * @details
 *
 *     Function : fetchTbfromDlHarqProc
 *      
 *      Returns a transmission block from HARQ process Cb
 *           
 *  @param[in]  Time of retransmission
 *  @param[in]  UE CB
 *  @param[in]  HARQ process Id
 *  @param[in]  TB size
 *  @return  
 *      -# Pointer to TB
 *      -# NULL
 **/
uint8_t* fetchTbfromDlHarqProc(SlotTimingInfo slotInfo, MacUeCb *ueCb, uint8_t hqProcId, uint32_t tbSize)
{
   uint8_t hqProcIdx = 0, tbIdx = 0;
   DlHarqEnt  *dlHqEnt = NULLP;

   dlHqEnt = &ueCb->dlInfo.dlHarqEnt;

   /* Search HARQ Proc Cb in DL HARQ Ent */
   for(hqProcIdx = 0; hqProcIdx < dlHqEnt->numHarqProcs; hqProcIdx++)
   {   
      if(dlHqEnt->harqProcCb[hqProcIdx].procId == hqProcId)
      {
         /* Search TB Info in a HARQ Proc Cb */
         for(tbIdx =0; tbIdx < dlHqEnt->harqProcCb[hqProcIdx].numTb; dlHqEnt++)
         {
            if(dlHqEnt->harqProcCb[hqProcIdx].tbInfo[tbIdx].tbSize == tbSize)
            {
               /* Update transmission time in TB Info */
               memset(&dlHqEnt->harqProcCb[hqProcIdx].tbInfo[tbIdx].txTime, 0, sizeof(SlotTimingInfo));
               memcpy(&dlHqEnt->harqProcCb[hqProcIdx].tbInfo[tbIdx].txTime, &slotInfo, sizeof(SlotTimingInfo));
               return dlHqEnt->harqProcCb[hqProcIdx].tbInfo[tbIdx].tb;
            }
         }
      }
   }
   return NULLP;
}

/**********************************************************************
  End of file
 **********************************************************************/
