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
#include "mac_ue_mgr.h"

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
void addDlHqProcInUe(SlotTimingInfo dlMsgTime, MacUeCb *ueCb, DlMsgSchInfo *schedInfo)
{
   uint8_t       hqProcId = 0, tbIdx = 0, cwIdx = 0;
   DlHarqEnt     *dlHqEnt = NULLP;
   DlHarqProcCb  *hqProcCb = NULLP;

   dlHqEnt = &ueCb->dlInfo.dlHarqEnt;  
   hqProcId = schedInfo->harqProcNum;
   hqProcCb = &dlHqEnt->harqProcCb[hqProcId];

   /* Check if harqProcId is already present in UE's DL HARQ Entity */
   if(hqProcCb->procId == schedInfo->harqProcNum)
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
      for(tbIdx = 0; tbIdx < hqProcCb->numTb; tbIdx++)
      {
         MAC_FREE(hqProcCb->tbInfo[tbIdx].tb, hqProcCb->tbInfo[tbIdx].tbSize);
      }
      memset(hqProcCb, 0, sizeof(DlHarqProcCb));
   }

   /* Fill HARQ Proc Cb */
   hqProcCb->procId = hqProcId;
   for(cwIdx = 0; cwIdx < schedInfo->dlMsgPdschCfg->numCodewords; cwIdx++)
   {
      memcpy(&hqProcCb->tbInfo[hqProcCb->numTb].txTime, &dlMsgTime, sizeof(SlotTimingInfo));
      hqProcCb->tbInfo[hqProcCb->numTb].tbSize = schedInfo->dlMsgPdschCfg->codeword[cwIdx].tbSize;
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
   DlHarqEnt  *dlHqEnt = NULLP;
   DlTbInfo   *tbInfo = NULLP;

   dlHqEnt = &ueCb->dlInfo.dlHarqEnt;

   /* Search HARQ Proc Cb in DL HARQ Ent */
   for(hqProcIdx = 0; hqProcIdx < MAX_NUM_HARQ_PROC; hqProcIdx++)
   {
      /* Search TB Info in a HARQ Proc Cb */
      for(tbIdx =0; tbIdx < dlHqEnt->harqProcCb[hqProcIdx].numTb; tbIdx++)
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

            return ROK;
         }
      }
   }
   return RFAILED;
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
   uint8_t    tbIdx = 0;
   DlHarqEnt  *dlHqEnt = NULLP;
   DlHarqProcCb  *hqProcCb = NULLP;

   dlHqEnt = &ueCb->dlInfo.dlHarqEnt;
   hqProcCb = &dlHqEnt->harqProcCb[hqProcId];

   /* Search HARQ Proc Cb in DL HARQ Ent */
   if(hqProcCb->procId == hqProcId)
   {
      /* Search TB Info in a HARQ Proc Cb */
      for(tbIdx =0; tbIdx < hqProcCb->numTb; tbIdx++)
      {
         if(hqProcCb->tbInfo[tbIdx].tbSize == tbSize)
         {
            /* Update transmission time in TB Info */
            memset(&hqProcCb->tbInfo[tbIdx].txTime, 0, sizeof(SlotTimingInfo));
            memcpy(&hqProcCb->tbInfo[tbIdx].txTime, &slotInfo, sizeof(SlotTimingInfo));

            return hqProcCb->tbInfo[tbIdx].tb;
         }
      }
   }
   return NULLP;
}
/**
 * @brief Release Dl Harq process
 *
 * @details
 *
 *     Function : fetchTbfromDlHarqProc
 *      
 *      Release Dl Harq process
 *           
 *  @param[in]  Pst *pst, the post structure
 *  @param[in]  SchRlsHqInfo *hqIndo, release hq info structure
  *  @return  
 *      -# ROK
 *      -# RFAILED
 **/
uint8_t MacSchReleaseDlHarqProc(Pst *pst, SchRlsHqInfo *hqInfo)
{
   uint8_t   hqProcId, tbIdx = 0;
   uint16_t  cellIdx = 0, hqInfoIdx = 0, ueId = 0;
   MacCellCb *cellCb = NULLP;
   MacUeCb   *ueCb = NULLP;
   DlHarqEnt     *dlHqEnt = NULLP;
   DlHarqProcCb  *hqProcCb = NULLP;

   GET_CELL_IDX(hqInfo->cellId, cellIdx);
   cellCb = macCb.macCell[cellIdx];

   for(hqInfoIdx = 0; hqInfoIdx < hqInfo->numUes; hqInfoIdx++)
   {
      GET_UE_ID(hqInfo->ueHqInfo[hqInfoIdx].crnti, ueId)
      ueCb = &cellCb->ueCb[ueId -1];
      dlHqEnt = &ueCb->dlInfo.dlHarqEnt;
      hqProcId = hqInfo->ueHqInfo[hqInfoIdx].hqProcId;

      /* First check if the HARQ process to be released belong to msg 4 */
      if ((ueCb->raCb) && (ueCb->raCb->msg4HqInfo.procId == hqProcId))
      {
            deleteMacRaCb(cellIdx, ueCb);
      }
      else
      {
         /* Search harqProcId in UE's DL HARQ Entity */
         hqProcCb = &dlHqEnt->harqProcCb[hqProcId];
         if(hqProcCb->procId == hqProcId)
         {
            /* Free HARQ process */
            for(tbIdx = 0; tbIdx < hqProcCb->numTb; tbIdx++)
            {
               MAC_FREE(hqProcCb->tbInfo[tbIdx].tb, hqProcCb->tbInfo[tbIdx].tbSize);
            }
            memset(hqProcCb, 0, sizeof(DlHarqProcCb));
            hqProcCb->procId =  MAX_NUM_HARQ_PROC;
         }
      }
   }
   
   MAC_FREE(hqInfo->ueHqInfo, (sizeof(SchUeHqInfo) * hqInfo->numUes)); 
   MAC_FREE(hqInfo, sizeof(SchRlsHqInfo));
   return ROK;
}

/**********************************************************************
  End of file
 **********************************************************************/
