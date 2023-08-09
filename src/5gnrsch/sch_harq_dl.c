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
#include "common_def.h"
#include "tfu.h"
#include "lrg.h"
#include "tfu.x"
#include "lrg.x"
#include "du_log.h"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "sch.h"
#include "sch_tmr.h"
#include "sch_utils.h"
#include "cm_llist.h"
#ifdef NR_DRX
#include "sch_drx.h"
#endif

typedef struct schCellCb SchCellCb;
typedef struct schUeCb SchUeCb;
void schDlHqEntReset(SchCellCb *cellCb, SchUeCb *ueCb, SchDlHqEnt *hqE);
void schDlHqAddToFreeList(SchDlHqProcCb *hqP);

/**
 * @brief DL Harq entity intialization
 *
 * @details
 *
 *     Function : schDlHqEntInit
 *      
 *      This function intialize DL Harq entity
 *           
 *  @param[in]  SchCellCb *cellCb, cell cb pointer
 *  @param[in]  SchUeCb *ueCb, ue cb pointer
 *  @return  
 *      -# void
 **/
void schDlHqEntInit(SchCellCb *cellCb, SchUeCb *ueCb)
{
   ueCb->dlHqEnt.numHqPrcs = SCH_MAX_NUM_DL_HQ_PROC;
   ueCb->dlHqEnt.maxHqTx  = cellCb->schHqCfg.maxDlDataHqTx;
   ueCb->dlHqEnt.cell = cellCb;
   ueCb->dlHqEnt.ue =ueCb;
   schDlHqEntReset(cellCb, ueCb, &ueCb->dlHqEnt);
}
/**
 * @brief DL Harq entity Reset
 *
 * @details
 *
 *     Function : schDlHqEntReset
 *      
 *      This function Reset DL Harq entity
 *           
 *  @param[in]  SchCellCb *cellCb, cell cb pointer
 *  @param[in]  SchUeCb *ueCb, ue cb pointer
 *  @param[in]  SchDlHqEnt *hqE, Dl Harq entity pointer
 *  @return  
 *      -# void
 **/
void schDlHqEntReset(SchCellCb *cellCb, SchUeCb *ueCb, SchDlHqEnt *hqE)
{
   uint8_t count = 0;
   SchDlHqProcCb *hqP = NULL;
   cmLListInit(&hqE->free);
   cmLListInit(&hqE->inUse);

   for(count=0; count < hqE->numHqPrcs; count++)
   {
      hqP = &(hqE->procs[count]);
      hqP->procId = count;
      hqP->hqEnt = hqE;
      hqP->maxHqTxPerHqP = hqE->maxHqTx;
      hqP->dlHqEntLnk.node = (PTR)hqP;
      hqP->dlHqProcLink.node = (PTR)hqP;
      hqP->ulSlotLnk.node = (PTR)hqP;
      cellCb->api->SchInitDlHqProcCb(hqP);
      schDlHqAddToFreeList(hqP);
   }
}
/**
 * @brief Add hq process to free list of DL Harq entity
 *
 * @details
 *
 *     Function : schDlHqAddToFreeList
 *      
 *      This function adds hq process to free list of DL Harq entity
 *           
 *  @param[in]  SchDlHqProcCb *hqP, DL harq process pointer
 *  @return  
 *      -# void
 **/
void schDlHqAddToFreeList(SchDlHqProcCb *hqP)
{   
   cmLListAdd2Tail(&(hqP->hqEnt->free), &hqP->dlHqEntLnk);
}
/**
 * @brief Delete hq process from free list of DL Harq entity
 *
 * @details
 *
 *     Function : schDlHqDeleteFromFreeList
 *      
 *      This function deletes hq process to free list of DL Harq entity
 *           
 *  @param[in]  SchDlHqProcCb *hqP, DL harq process pointer
 *  @return  
 *      -# void
 **/
void schDlHqDeleteFromFreeList(SchDlHqProcCb *hqP)
{
   cmLListDelFrm(&(hqP->hqEnt->free), &hqP->dlHqEntLnk);
}
/**
 * @brief Add hq process to in use list of DL Harq entity
 *
 * @details
 *
 *     Function : schDlHqAddToInUseList
 *      
 *      This function adds hq process to in use list of DL Harq entity
 *           
 *  @param[in]  SchDlHqProcCb *hqP, DL harq process pointer
 *  @return  
 *      -# void
 **/
void schDlHqAddToInUseList(SchDlHqProcCb *hqP)
{
   cmLListAdd2Tail(&(hqP->hqEnt->inUse), &hqP->dlHqEntLnk);
}
/**
 * @brief Delete hq process from in use list of DL Harq entity
 *
 * @details
 *
 *     Function : schDlHqDeleteFromInUseList
 *      
 *      This function deletes hq process to in use list of DL Harq entity
 *           
 *  @param[in]  SchDlHqProcCb *hqP, DL harq process pointer
 *  @return  
 *      -# void
 **/
void schDlHqDeleteFromInUseList(SchDlHqProcCb *hqP)
{   
   cmLListDelFrm(&(hqP->hqEnt->inUse), &hqP->dlHqEntLnk);
}
/**
 * @brief Get available Harq process from Harq entity
 *
 * @details
 *
 *     Function : schDlGetAvlHqProcess
 *      
 *      This function fetches hq process from free list and puts in in use list
 *           
 *  @param[in]  SchCellCb *cellCb, cell cb pointer
 *  @param[in]  SchUeCb *ueCb, ue cb pointer
 *  @param[in]  SchDlHqProcCb **hqP, Address of DL harq process pointer
 *  @return  
 *      -# ROK
 *      -# RFAILED
 **/
uint8_t schDlGetAvlHqProcess(SchCellCb *cellCb, SchUeCb *ueCb, SchDlHqProcCb **hqP)
{
   SchDlHqProcCb *tmp;
   if (ueCb->dlHqEnt.free.count == 0)
   {
      return RFAILED;
   }
   tmp = (SchDlHqProcCb*)(cmLListFirst(&(ueCb->dlHqEnt.free))->node);
   if (NULLP == tmp)
   {
      return RFAILED;
   }
   schDlHqDeleteFromFreeList(tmp);
   schDlHqAddToInUseList(tmp);
   *hqP = tmp;
   (*hqP)->maxHqTxPerHqP = ueCb->dlHqEnt.maxHqTx;
   return ROK;
}
/**
 * @brief Release Harq process from the DL Harq entity
 *
 * @details
 *
 *     Function : schDlReleaseHqProcess
 *      
 *      This function releases Harq process from DL Harq entity
 *           
 *  @param[in]  SchDlHqProcCb *hqP, DL harq process pointer
 *  @return  
 *      -# void
 **/
void schDlReleaseHqProcess(SchDlHqProcCb *hqP)
{
   SchCellCb  *cellCb = NULLP;
   if(hqP)
   {
      cellCb = hqP->hqEnt->cell;
      cellCb->api->SchFreeDlHqProcCb(hqP);
      schDlHqDeleteFromInUseList(hqP);
      schDlHqAddToFreeList(hqP);
   }
}

/*******************************************************************
 *
 * @brief Handles sending DL HARQ process release to MAC 
 *
 * @details
 *
 *    Function : sendDlHarqProcReleaseToMac
 *
 *    Functionality:
 *     Sends DL DL HARQ process release to MAC from SCH
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sendDlHarqProcReleaseToMac(SchDlHqProcCb *hqP, Inst inst)
{
   Pst pst;
   SchRlsHqInfo *rlsHqInfo;
   memset(&pst, 0, sizeof(Pst));
   FILL_PST_SCH_TO_MAC(pst, inst);
   pst.event = EVENT_DL_REL_HQ_PROC;

   SCH_ALLOC(rlsHqInfo, sizeof(SchRlsHqInfo));
   rlsHqInfo->cellId = hqP->hqEnt->cell->cellId;
   rlsHqInfo->numUes = 1;

   SCH_ALLOC(rlsHqInfo->ueHqInfo, sizeof(SchUeHqInfo)*rlsHqInfo->numUes);
   rlsHqInfo->ueHqInfo[0].crnti = hqP->hqEnt->ue->crnti;
   rlsHqInfo->ueHqInfo[0].hqProcId = hqP->procId;   

   return(MacMessageRouter(&pst, (void *)rlsHqInfo));
}
/**
 * @brief Release Harq process TB from the DL Harq process
 *
 * @details
 *
 *     Function : schDlReleaseHqPTb
 *      
 *      This function releases Harq process TB from DL Harq proces
 *           
 *  @param[in]  SchDlHqProcCb *hqP, DL harq process pointer
 *  @param[in]  uint8_t tbIdx, TB index
 *  @param[in]  bool togNdi, indication to toggle NDI bit
 *  @return  
 *      -# void
 **/
void schDlReleaseHqPTb(SchDlHqProcCb *hqP, uint8_t tbIdx, bool togNdi)
{
   if (TRUE == togNdi)
   {
      hqP->tbInfo[tbIdx].ndi ^= 1;
   }

   {
      hqP->tbInfo[tbIdx].isAckNackDtx = HQ_ACK;
      hqP->tbInfo[tbIdx].isEnabled = FALSE;
      hqP->tbInfo[tbIdx].state = HQ_TB_ACKED;
      hqP->tbInfo[tbIdx].txCntr = 0;
      if (HQ_TB_ACKED == hqP->tbInfo[tbIdx^1].state)
      {
         schDlReleaseHqProcess(hqP);
         sendDlHarqProcReleaseToMac(hqP, hqP->hqEnt->cell->instIdx);
      }
   }
}
/**
 * @brief Handles failure of HARQ process TB
 *
 * @details
 *
 *     Function : schDlHqTbFail
 *      
 *      This function handles failure of HARQ process TB
 *           
 *  @param[in]  SchDlHqProcCb *hqP, DL harq process pointer
 *  @param[in]  uint8_t tbIdx, TB index
 *  @param[in]  bool isMaxRetx, indicates max retransmission
 *  @return  
 *      -# void
 **/
void schDlHqTbFail(SchDlHqProcCb *hqP, uint8_t tbIdx, bool isMaxRetx)
{
   if (isMaxRetx)
   {
      schDlReleaseHqPTb(hqP, tbIdx, TRUE);
   }
   else
   {
      hqP->tbInfo[tbIdx].state = HQ_TB_NACKED;
      if (HQ_TB_WAITING == hqP->tbInfo[tbIdx^1].state)
      {
         hqP->hqEnt->cell->api->SchAddToDlHqRetxList(hqP);
      }
   }
}
/**
 * @brief Handles Harq feedback for MSG4
 *
 * @details
 *
 *     Function : schMsg4FeedbackUpdate
 *      
 *      This function handles Harq feedback for MSG4
 *           
 *  @param[in]  SchDlHqProcCb *hqP, DL harq process pointer
 *  @param[in]  uint8_t fdbk, Received feedback
 *  @return  
 *      -# void
 **/
void schMsg4FeedbackUpdate(SchDlHqProcCb *hqP, uint8_t fdbk)
{
   hqP->tbInfo[0].isAckNackDtx = fdbk;
   hqP->tbInfo[1].isAckNackDtx = HQ_TB_ACKED;
   if (HQ_TB_ACKED == hqP->tbInfo[0].isAckNackDtx)
   {
      schDlReleaseHqPTb(hqP, 0, TRUE);
      schDlReleaseHqPTb(hqP, 1, TRUE);
      schMsg4Complete(hqP->hqEnt->ue);
   }
   else
   {
      if( hqP->tbInfo[0].txCntr >= hqP->hqEnt->cell->schHqCfg.maxMsg4HqTx)
      {
         schDlReleaseHqProcess(hqP);
         hqP->hqEnt->ue->msg4HqProc = NULLP;
         hqP->hqEnt->ue->retxMsg4HqProc = NULLP;
         /* Delete UE and RA context */
      }
      hqP->hqEnt->cell->api->SchAddUeToSchedule(hqP->hqEnt->cell,hqP->hqEnt->ue->ueId);
      hqP->hqEnt->ue->retxMsg4HqProc = hqP;
   }
}

/**
 * @brief Handles Harq feedback for DL Data
 *
 * @details
 *
 *     Function : schDlHqFeedbackUpdate
 *      
 *      This function handles Harq feedback for DL data
 *           
 *  @param[in]  SchDlHqProcCb *hqP, DL harq process pointer
 *  @param[in]  uint8_t fdbk1, Received feedback for TB -0
 *  @param[in]  uint8_t fdbk2, Received feedback for TB -1
 *  @return  
 *      -# void
 **/
void schDlHqFeedbackUpdate(SchDlHqProcCb *hqP, uint8_t fdbk1, uint8_t fdbk2)
{
   uint8_t tbIdx;
   for (tbIdx = 0; tbIdx <2; tbIdx++)
   {
      if (HQ_TB_WAITING == hqP->tbInfo[tbIdx].state)
      {
         hqP->tbInfo[tbIdx].isAckNackDtx = (0 == tbIdx)?fdbk1:fdbk2;
      }
      if (TRUE == hqP->tbInfo[tbIdx].isEnabled)
      {
         if (HQ_TB_ACKED == hqP->tbInfo[tbIdx].isAckNackDtx)
         {
            schDlReleaseHqPTb(hqP, tbIdx, TRUE);
         }
         else
         {
            if(hqP->tbInfo[tbIdx].txCntr >= hqP->maxHqTxPerHqP)
            {
               schDlHqTbFail(hqP, tbIdx, TRUE);
            }
            else
            {
               schDlHqTbFail(hqP, tbIdx, FALSE);
            }
         }
      }
   }
}

/**
 * @brief Deletes HARQ Entity
 *
 * @details
 *
 *     Function : schDlHqEntDelete
 *      
 *      This function deletes HARQ entity and its member paremeters
 *           
 *  @param[in]  Pointer to UE
 *  @return  
 *      -# void
 **/
void schDlHqEntDelete(SchUeCb *ueCb)
{
  uint8_t count;
  SchDlHqProcCb *hqP;

  cmLListDeleteLList(&ueCb->dlHqEnt.free);
  cmLListDeleteLList(&ueCb->dlHqEnt.inUse);
  for(count=0; count < ueCb->dlHqEnt.numHqPrcs; count++)
  {
     hqP = &(ueCb->dlHqEnt.procs[count]);
     ueCb->cellCb->api->SchDeleteDlHqProcCb(hqP);
  }
  memset(&ueCb->dlHqEnt, 0, sizeof(SchDlHqEnt));
}

/**********************************************************************
  End of file
 **********************************************************************/
