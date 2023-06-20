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

/* This file contains message handling functionality for DU cell management */
#include "common_def.h"
#include "du_tmr.h"
#include "lrg.h"
#include "legtp.h"
#include "lrg.x"
#include "lkw.x"
#include "rgr.h"
#include "rgr.x"
#include "du_app_mac_inf.h"
#include "du_app_rlc_inf.h"
#include "du_e2ap_mgr.h"
#include "du_cfg.h"
#include "du_mgr.h"
#include "du_utils.h"
#include "du_cell_mgr.h"
#include "PCCH-Config.h"
#include "PagingUE-Identity.h"
#include "PagingRecord.h"
#include "PagingRecordList.h"
#include "PagingRrc.h"
#include "PCCH-MessageType.h"
#include "PCCH-Message.h"
#include "odu_common_codec.h"

#ifdef O1_ENABLE

#include "AlarmInterface.h"
#include "CmInterface.h"

#endif

DuMacCellDeleteReq packMacCellDeleteReqOpts[] =
{
   packDuMacCellDeleteReq,       /* Loose coupling */
   MacProcCellDeleteReq,         /* TIght coupling */
   packDuMacCellDeleteReq        /* Light weight-loose coupling */
};

DuMacDlPcchInd packMacDlPcchIndOpts[] =
{
   packDuMacDlPcchInd,       /* Loose coupling */
   MacProcDlPcchInd,         /* TIght coupling */
   packDuMacDlPcchInd        /* Light weight-loose coupling */
};

DuMacDlBroadcastReq packMacDlBroadcastReqOpts[] =
{
   packDuMacDlBroadcastReq,       /* Loose coupling */
   MacProcDlBroadcastReq,         /* TIght coupling */
   packDuMacDlBroadcastReq        /* Light weight-loose coupling */
};
/*******************************************************************
 *
 * @brief Processes cells to be activated
 *
 * @details
 *
 *    Function : duProcCellsToBeActivated
 *
 *    Functionality:
 *      - Processes cells to be activated list received in F1SetupRsp
 *
 * @params[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duProcCellsToBeActivated(uint8_t *plmnStr, uint16_t nci, uint16_t nRPci)
{
   uint8_t ret = ROK;
   DuCellCb *cellCb = NULLP;
   uint8_t cfgIdx, tmpPlmn[4];

   for(cfgIdx=0; cfgIdx<duCb.numCfgCells; cfgIdx++)
   {
      memset(tmpPlmn, 0, 4);
      buildPlmnId(duCb.cfgCellLst[cfgIdx]->cellInfo.nrEcgi.plmn, tmpPlmn);
      if(duCb.cfgCellLst[cfgIdx]->cellInfo.nrEcgi.cellId == nci &&
            (strcmp((const char*)tmpPlmn, (const char*)plmnStr) == 0))
      {
         cellCb = duCb.cfgCellLst[cfgIdx];
         break;
      }
      else
      {
         DU_LOG("\nERROR  -->  DU APP : No Cell found for NCI %d", nci);
         return RFAILED;
      }
   }

   cellCb->cellStatus = ACTIVATION_IN_PROGRESS; 
   cellCb->cellInfo.nrPci = nRPci;

   if((cmHashListInit(&(cellCb->pagingInfoMap), MAX_SFN, sizeof(DuPagInfoList),\
                FALSE, CM_HASH_KEYTYPE_CONID, DU_APP_MEM_REGION, DU_POOL)) != ROK)
   {
      DU_LOG("ERROR  --> DU APP: Failed to Initialize the Paging Hash Map");
      return RFAILED;             
   }
   duCb.actvCellLst[cellCb->cellId -1] = cellCb;
   duCb.numActvCells++;

   if(duBuildAndSendMacCellCfg(cellCb->cellId) != ROK)
   {
      DU_LOG("\nERROR  -->  DU APP : macCellCfg build and send failed");
      /* Delete cell from actvCellList */
      duCb.actvCellLst[cellCb->cellId -1] = NULLP;
      --(duCb.numActvCells);
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Handles DU F1Setup Rsp received in F1AP
 *
 * @details
 *
 *    Function : duProcF1SetupRsp
 *
 *    Functionality:
 *      - Handles DU F1Setup Rsp received in F1AP
 *
 * @params[in] Pointer to F1SetupRsp 
 * @return void
 *
 ******************************************************************/
void duProcF1SetupRsp()
{
   DU_LOG("\nINFO   -->  DU_APP : F1 Setup Response received");
   duCb.f1Status = TRUE; //Set F1 status as true
}

/*******************************************************************
*
* @brief Returns cellCb based on cell ID
*
* @details
*
*    Function : duGetCellCb
*
*    Functionality: Returns DU APP CellCb based on cell ID
*
* @params[in] F1AP_PDU_t ASN decoded F1AP message
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t duGetCellCb(uint16_t cellId, DuCellCb **cellCb)
{
   uint8_t cellIdx = 0;

   for(cellIdx=0; cellIdx < MAX_NUM_CELL; cellIdx++)
   {
      if(duCb.actvCellLst[cellIdx] && (duCb.actvCellLst[cellIdx]->cellId == cellId))
      {
         *cellCb = duCb.actvCellLst[cellIdx];
	      break;
      }
   }

   if(!*cellCb)
   {
      DU_LOG("\nERROR  -->  DU APP : Cell Id %d not found in DU APP", cellId);
      return RFAILED;
   }

   return ROK;
}

/******************************************************************************
* @brief Check Paging Record for a Particular Paging Frame
*
* @details
*
*   Function : checkPagingRecord
*
*   Functionality:
*           Check Paging Record for a Particular Paging Frame
* 
*  @params[in] DuCellCb *cellCb
*
*  @return ROK/RFAILURE (uint8_t return)
* 
* 
******************************************************************************/
uint8_t checkPagingRecord(DuCellCb *cellCb)
{
   uint16_t pf = 0;
   DuPagInfoList *pagInfoLLFromPF = NULLP;
   DuPagUeList *pagInfo = NULLP;

   /*DUAPP may send PagingReq to MAC for future SFN so that Schedular can 
    * schedule Paging on the exact occurence of the Slot.*/
   pf = (cellCb->currSlotInfo.sfn + PAGING_SCHED_DELTA) % MAX_SFN;

   pagInfoLLFromPF = findPagingInfoFromMap(pf, &(cellCb->pagingInfoMap));
   if(pagInfoLLFromPF == NULLP)
   {
      /*No Page is present for pf thus exiting*/
      return ROK;
   }
  
   do
   {
      pagInfo = handlePageInfoLL(pf, NULLD, &(pagInfoLLFromPF->pagInfoList), TRAVERSE_ALL);
      if(pagInfo != NULLP)
      {
         if(BuildAndSendDlPcchIndToMac(cellCb->cellId, pf, pagInfo->i_s, &(pagInfo->pagUeList)) != ROK)
         {
            DU_LOG("\nERROR  -->  DU APP: Issue in Building Page RRC PDU i_s:%d",pagInfo->i_s);
            return RFAILED; 
         }
         handlePageInfoLL(pf, pagInfo->i_s, &(pagInfoLLFromPF->pagInfoList), DELETE);
      }
      if(pagInfoLLFromPF->pagInfoList.first == NULLP)
      {
         break;
      }
   }while(pagInfo != NULLP);
   
   cmHashListDelete(&(cellCb->pagingInfoMap), (PTR)pagInfoLLFromPF);
   DU_FREE(pagInfoLLFromPF, sizeof(DuPagInfoList));
   return ROK;
}

/******************************************************************
 *
 * @brief Send pcch indication to MAC
 *
 * @details
 *
 *    Function : sendDlPcchIndToMac
 *
 *    Functionality: Send pcch indication to MAC
 *
 * @Params[in] DlPcchInd *pcchInd
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t sendDlPcchIndToMac(DlPcchInd *pcchInd)
{
   uint8_t ret = ROK;
   Pst pst;

   if(pcchInd)
   {
      /* Fill Pst */
      FILL_PST_DUAPP_TO_MAC(pst, EVENT_MAC_DL_PCCH_IND);
      
      ret = (*packMacDlPcchIndOpts[pst.selector])(&pst, pcchInd);
      if(ret == RFAILED)
      {
         DU_LOG("\nERROR  -->  DU APP : sendDlPcchIndToMac(): Failed to DL PCCH indication to MAC");
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  DU APP: sendDlPcchIndToMac(): Received pcchInd is NULLP");
      ret = RFAILED;
   }
   return ret;
}


/*****************************************************************
* @brief Handles slot indication from MAC
*
* @details
*
*   Function : duHandleSlotInd
*
*   Functionality:
*       Handles  slot indication from MAC
* 
*  @params[in] Post structure pointer
*              SlotTimingInfo *slotIndInfo
*  @return ROK     - success
*          RFAILED - failure
* 
* 
*****************************************************************/
uint8_t duHandleSlotInd(Pst *pst, SlotTimingInfo *slotIndInfo)
{
   uint8_t cellIdx = 0, ret = ROK;
   DuCellCb *duCellCb = NULLP;

   if(slotIndInfo)
   {
      GET_CELL_IDX(slotIndInfo->cellId, cellIdx);
      duCellCb = duCb.actvCellLst[cellIdx];

      if(duCellCb)
      {
         duCellCb->currSlotInfo.sfn = slotIndInfo->sfn;
         duCellCb->currSlotInfo.slot = slotIndInfo->slot;
         checkPagingRecord(duCellCb);
      }
      else
      {
         DU_LOG("\nERROR  -->  DU APP : CellId[%d] doesnot exist", slotIndInfo->cellId);
         ret = RFAILED;
      }
      DU_FREE_SHRABL_BUF(pst->region, pst->pool, slotIndInfo, sizeof(SlotTimingInfo));
   }
   else
   {
      DU_LOG("\nERROR  -->  DU APP : Recevied null pointer from MAC");
      ret = RFAILED;
   }
   return(ret);
}
/*******************************************************************
 *
 * @brief Handles cell up indication from MAC
 *
 * @details
 *
 *    Function : duHandleCellUpInd
 *
 *    Functionality:
 *      Handles cell up indication from MAC
 *
 * @params[in] Post structure pointer
 *             cell Up info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duHandleCellUpInd(Pst *pst, OduCellId *cellId)
{
   DuCellCb *cellCb = NULLP; 

   if(cellId->cellId <=0 || cellId->cellId > MAX_NUM_CELL)
   {
      DU_LOG("\nERROR  -->  DU APP : Invalid Cell Id %d in duHandleCellUpInd()", cellId->cellId);
      return RFAILED;
   }

   if(duGetCellCb(cellId->cellId, &cellCb) != ROK)
      return RFAILED;

   if((cellCb != NULL) && (cellCb->cellStatus == ACTIVATION_IN_PROGRESS))
   {
      DU_LOG("\nINFO   -->  DU APP : 5G-NR Cell %d is UP", cellId->cellId);
      cellCb->cellStatus = ACTIVATED;
      gCellStatus = CELL_UP;

      if(duCfgParam.tempSliceCfg.numOfRrmPolicy)
         BuildAndSendSliceConfigReq();
#ifdef O1_ENABLE
      DU_LOG("\nINFO   -->  DU APP : Raise cell UP alarm for cell id=%d", cellId->cellId);
      raiseCellAlrm(CELL_UP_ALARM_ID, cellId->cellId);
      setCellOpState(cellId->cellId, ENABLED, ACTIVE);
#endif
       duCfgParam.macCellCfg.cellCfg.opState = OP_ENABLED;
       duCfgParam.macCellCfg.cellCfg.cellState = CELL_ACTIVE;
   }

   if((pst->selector == ODU_SELECTOR_LWLC) || (pst->selector == ODU_SELECTOR_TC))
      DU_FREE_SHRABL_BUF(pst->region, pst->pool, cellId, sizeof(OduCellId));
   return ROK;
}

/*******************************************************************
 *
 * @brief Handle Cell delete response from MAC
 *
 * @details
 *
 *    Function : DuProcMacCellDeleteRsp
 *
 *    Functionality: Handle Cell delete response from MAC
 *
 * @params[in] Pointer to MacCellDeleteRsp and Pst
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t DuProcMacCellDeleteRsp(Pst *pst, MacCellDeleteRsp *deleteRsp)
{
   uint8_t ret = ROK;
   uint16_t cellIdx=0, pfIdx=0;
   DuPagInfoList *pagInfoLLFromPF=NULLP;

   if(deleteRsp)
   {
      if(deleteRsp->status == SUCCESSFUL)
      {
         GET_CELL_IDX(deleteRsp->cellId, cellIdx);
         DU_LOG("\nINFO   -->  DU APP : MAC CELL Delete Response : SUCCESS [CELL IDX : %d]", deleteRsp->cellId);
         if(duCb.actvCellLst[cellIdx] && (duCb.actvCellLst[cellIdx]->cellId == deleteRsp->cellId))
         {
            for(pfIdx =0; pfIdx < MAX_SFN; pfIdx++)
            {
               pagInfoLLFromPF = findPagingInfoFromMap(pfIdx, &(duCb.actvCellLst[cellIdx]->pagingInfoMap));
               if(pagInfoLLFromPF)
               {   
                  cmHashListDelete(&(duCb.actvCellLst[cellIdx]->pagingInfoMap), (PTR)pagInfoLLFromPF);
                  DU_FREE(pagInfoLLFromPF, sizeof(DuPagInfoList));
               }
            }

            memset(duCb.actvCellLst[cellIdx], 0, sizeof(DuCellCb));
            gCellStatus = CELL_DOWN;

#ifdef O1_ENABLE
            DU_LOG("\nINFO   -->  DU APP : Raise cell down alarm for cell id=%d", deleteRsp->cellId);
            raiseCellAlrm(CELL_DOWN_ALARM_ID, deleteRsp->cellId);
            setCellOpState(deleteRsp->cellId, DISABLED, INACTIVE);
#endif

            duCb.numActvCells--;
            duCb.numCfgCells--;
            DU_FREE(duCb.actvCellLst[cellIdx], sizeof(DuCellCb));

         }
         else
         {
            DU_LOG("\nERROR  -->  DU APP : DuProcMacCellDeleteRsp(): CellId [%d] doesnot exist", deleteRsp->cellId);
            ret = RFAILED;
         }
      }
      else
      {
         DU_LOG("\nERROR  -->  DU APP : DuProcMacCellDeleteRsp(): MAC CELL Delete Response : FAILED\
         [CELL IDX : %d]", deleteRsp->cellId);
         ret = RFAILED;
      }
      DU_FREE_SHRABL_BUF(pst->region, pst->pool, deleteRsp, sizeof(MacCellDeleteRsp));
   }
   else
   {
      DU_LOG("\nERROR  -->  DU APP : DuProcMacCellDeleteRsp(): Received MAC cell delete response is NULL");
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Sending Cell Delete Req To Mac
 *
 * @details
 *
 *    Function : sendCellDeleteReqToMac
 *
 *    Functionality:
 *     sending Cell Delete Req To Mac
 *
 *  @params[in]    uint16_t cellId
 *  @return ROK     - success
 *          RFAILED - failure
 *
 *
 *****************************************************************/

uint8_t sendCellDeleteReqToMac(uint16_t cellId)
{
   Pst pst;
   uint8_t ret=ROK;
   MacCellDeleteReq *cellDelete = NULLP;
   
   DU_ALLOC_SHRABL_BUF(cellDelete, sizeof(MacCellDeleteReq));
   if(cellDelete)
   {
      cellDelete->cellId = cellId;
      FILL_PST_DUAPP_TO_MAC(pst, EVENT_MAC_CELL_DELETE_REQ);

      DU_LOG("\nINFO   -->  DU APP : Sending Cell Delete Request to MAC");  
      /* Processing one Cell at a time to MAC */
      ret = (*packMacCellDeleteReqOpts[pst.selector])(&pst, cellDelete);
      if(ret == RFAILED)
      {
         DU_LOG("\nERROR  -->  DU APP : sendCellDeleteReqToMac(): Failed to send Cell delete Req to MAC");
         DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, cellDelete, sizeof(MacCellDeleteReq));
      }
   }
   else
   {
      DU_LOG("\nERROR  -->   DU APP : sendCellDeleteReqToMac(): Failed to allocate memory"); 
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief DU preocess Cell Delete Req to MAC 
 *
 * @details
 *
 *    Function : duSendCellDeletReq 
 *
 *    Functionality: DU process Cell Delete Req to MAC 
 *
 * @params[in] uint16_t cellId
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duSendCellDeletReq(uint16_t cellId)
{
   uint16_t cellIdx = 0;
   DU_LOG("\nINFO   -->  DU APP : Processing Cell Delete Request ");
   GET_CELL_IDX(cellId, cellIdx);

   if(duCb.actvCellLst[cellIdx] == NULLP)
   {
      DU_LOG("\nERROR  -->  DU APP : duSendCellDeletReq(): CellId[%d] is not found", cellId);
      return RFAILED;
   }
   
   if(duCb.actvCellLst[cellIdx]->cellId != cellId)
   {
      DU_LOG("\nERROR  -->  DU APP : duSendCellDeletReq(): CellId[%d] is not found", cellId);
      return RFAILED;

   }  
   
   if(duCb.actvCellLst[cellIdx]->cellStatus != DELETION_IN_PROGRESS)
   {
      DU_LOG("\nERROR  -->  DU APP : duSendCellDeletReq(): CellStatus[%d] of cellId[%d] is not correct.\
      Expected CellStatus is DELETION_IN_PROGRESS",duCb.actvCellLst[cellIdx]->cellStatus, cellId);
      return RFAILED;  
   }

   if(duBuildAndSendMacCellStop(cellId) == RFAILED)
   {
      DU_LOG("\nERROR  -->  DU APP : duSendCellDeletReq(): Failed to build and send cell stop request to MAC for\
            cellId[%d]",cellId);
      return RFAILED;
   }

   return ROK;
}

/*******************************************************************
 * @brief Free PCCH PDU structure
 *
 * @details
 *
 *    Function : freePcchPdu
 *
 *    Functionality:  Free PCCH PDU structure
 *
 * @params[in] PCCH_Message_t *pcchMsg 
 *
 * @return void
 *
 * ****************************************************************/
void freePcchPdu(PCCH_Message_t *pcchMsg)
{
   PagingRrc_t *pagingMsg = NULLP;
   uint8_t recordIdx = 0;

   if(pcchMsg != NULLP)
   {
      if(pcchMsg->message.choice.c1  != NULLP)
      {
         pagingMsg = pcchMsg->message.choice.c1->choice.paging;
         if(pagingMsg != NULLP)   
         {
            if(pagingMsg->pagingRecordList)
            {
               if(pagingMsg->pagingRecordList->list.array != NULLP)
               {
                  for(recordIdx = 0; recordIdx <  pagingMsg->pagingRecordList->list.count; recordIdx++)
                  {
                     if(pagingMsg->pagingRecordList->list.array[recordIdx] != NULLP)
                     {
                        DU_FREE(pagingMsg->pagingRecordList->list.array[recordIdx]->ue_Identity.choice.ng_5G_S_TMSI.buf,\
                                 pagingMsg->pagingRecordList->list.array[recordIdx]->ue_Identity.choice.ng_5G_S_TMSI.size);
                        DU_FREE(pagingMsg->pagingRecordList->list.array[recordIdx], sizeof(PagingRecord_t));
                     }
                  }
                  DU_FREE(pagingMsg->pagingRecordList->list.array, pagingMsg->pagingRecordList->list.size);
               }
               DU_FREE(pagingMsg->pagingRecordList, sizeof(PagingRecordList_t));
            }
            DU_FREE(pcchMsg->message.choice.c1->choice.paging, sizeof(PagingRrc_t));
         }
         DU_FREE(pcchMsg->message.choice.c1 , sizeof(PCCH_MessageType_t)); 
      }
      DU_FREE(pcchMsg , sizeof(PCCH_Message_t));
   }
}

/*******************************************************************
 * @brief Builds the Pcch RRC PDU and forwards it to MAC
 *
 * @details
 *
 *    Function : BuildAndSendDlPcchIndToMac
 *
 *    Functionality: Builds the Pcch RRC PDU[As per Spec 38.331, Annexure A]
 *                   and forwards it to MAC as Buffer along with PF and i_s
 *
 * @params[in] uint16_t cellId, uint16_t pf, uint8_t i_s,CmLListCp *pageUeLL
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendDlPcchIndToMac(uint16_t cellId, uint16_t pf, uint8_t i_s, CmLListCp *pageUeLL)
{
   CmLList        *node = NULLP, *next = NULLP;
   DuPagUeRecord  *ueRecord = NULLP;
   PCCH_Message_t *pcchMsg = NULLP;
   asn_enc_rval_t encRetVal;
   PagingRrc_t    *pagingMsg = NULLP;
   DlPcchInd     *macPcchInd = NULLP;
   uint8_t        recordIdx = 0, ret = RFAILED;
   
   /*As per 38.473 Sec 9.3.1.39,5G-S-TMSI :48 Bits >>  Bytes and 0 UnusedBits */
   uint8_t         totalByteInTmsi = 6, unusedBitsInTmsi = 0;

   if(pageUeLL == NULLP || pageUeLL->count == 0)
   {
      DU_LOG("\nERROR  -->  DU APP: UE Page Record LL is empty");
      return RFAILED;
   }

   while(true)
   {
      memset(&encRetVal, 0, sizeof(asn_enc_rval_t));
      DU_ALLOC(pcchMsg , sizeof(PCCH_Message_t));
      if(pcchMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  DU APP: BuildAndSendDlPcchIndToMac(): (pccchMsg) Memory Alloction failed!");
         break;
      }
      pcchMsg->message.present = PCCH_MessageType_PR_c1;
      DU_ALLOC(pcchMsg->message.choice.c1 , sizeof(PCCH_MessageType_t));
      if(pcchMsg->message.choice.c1 == NULLP)
      {
         DU_LOG("\nERROR  -->  DU APP: BuildAndSendDlPcchIndToMac(); (C1) Memory Alloction failed!");
         break;
      }
      pcchMsg->message.choice.c1->present = PCCH_MessageType__c1_PR_paging;
      DU_ALLOC(pcchMsg->message.choice.c1->choice.paging, sizeof(PagingRrc_t));

      pagingMsg = pcchMsg->message.choice.c1->choice.paging;
      if(pagingMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  DU APP: BuildAndSendDlPcchIndToMac(); (Paging) Memory Alloction failed!");
         break;
      }
      DU_ALLOC(pagingMsg->pagingRecordList, sizeof(PagingRecordList_t));
      if(pagingMsg->pagingRecordList == NULLP)
      {
         DU_LOG("\nERROR  -->  DU APP: BuildAndSendDlPcchIndToMac(); (Paging Record List) Memory Alloction failed!");
         break;
      }

      pagingMsg->pagingRecordList->list.count = pageUeLL->count;

       /*As per Spec 38.331, maxNrofPageRec : 32 [Maximum number of page records]*/
      if(pageUeLL->count > MAX_PAGING_UE_RECORDS)
      {
         pagingMsg->pagingRecordList->list.count = MAX_PAGING_UE_RECORDS;
      }

      pagingMsg->pagingRecordList->list.size = pageUeLL->count * (sizeof(PagingRecord_t *));
      DU_ALLOC(pagingMsg->pagingRecordList->list.array, pagingMsg->pagingRecordList->list.size);
      if(pagingMsg->pagingRecordList->list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  DU APP: BuildAndSendDlPcchIndToMac(); (Array) Memory Alloction failed!");
         break;
      }
      for(recordIdx = 0; recordIdx < pageUeLL->count; recordIdx++)
      {
         DU_ALLOC(pagingMsg->pagingRecordList->list.array[recordIdx], sizeof(PagingRecord_t));
         if(pagingMsg->pagingRecordList->list.array[recordIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  DU APP: BuildAndSendDlPcchIndToMac(); (Record) Memory Alloction failed!");
            break;
         }
      }
      recordIdx = 0;
      node = pageUeLL->first;
      while(node && (recordIdx < MAX_PAGING_UE_RECORDS))
      {
         next = node->next;
         ueRecord = (DuPagUeRecord *)node->node;
         if(ueRecord)
         {
            /*TODO : When Connected Mode Paging will be supported then I_RNTI will be introduced*/
            pagingMsg->pagingRecordList->list.array[recordIdx]->ue_Identity.present = PagingUE_Identity_PR_ng_5G_S_TMSI;
            pagingMsg->pagingRecordList->list.array[recordIdx]->ue_Identity.choice.ng_5G_S_TMSI.size = totalByteInTmsi*sizeof(uint8_t);
            DU_ALLOC(pagingMsg->pagingRecordList->list.array[recordIdx]->ue_Identity.choice.ng_5G_S_TMSI.buf,\
                  pagingMsg->pagingRecordList->list.array[recordIdx]->ue_Identity.choice.ng_5G_S_TMSI.size);
            if(pagingMsg->pagingRecordList->list.array[recordIdx]->ue_Identity.choice.ng_5G_S_TMSI.buf == NULLP)
            {
               DU_LOG("\nERROR  -->  DU APP: BuildAndSendDlPcchIndToMac(); (5gsTmsi buffer) Memory Allocation failed!");
               break;
            }
            fillBitString(&pagingMsg->pagingRecordList->list.array[recordIdx]->ue_Identity.choice.ng_5G_S_TMSI,\
                  unusedBitsInTmsi, totalByteInTmsi, ueRecord->sTmsi);
            recordIdx++;
         }

         if(duDelNodeFromLList(pageUeLL, node) == ROK)
             DU_FREE(ueRecord, sizeof(DuPagUeRecord));
         node = next;
      }
      if(node != NULLP && (recordIdx < MAX_PAGING_UE_RECORDS))
      {
         /*This leg is hit because Whole UE REcord List was not traversed as
          * some issue happened thus exiting the main while*/
         break;
      }
      xer_fprint(stdout, &asn_DEF_PCCH_Message, pcchMsg);
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
       /* Encode the PCCH RRC PDU as APER */
      encRetVal = uper_encode(&asn_DEF_PCCH_Message, 0, pcchMsg, PrepFinalEncBuf,\
            encBuf);

      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  F1AP : Could not encode Paging structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  F1AP : Created APER encoded buffer for RRC PDU for Pcch indication \n");
         
         DU_ALLOC_SHRABL_BUF(macPcchInd, sizeof(DlPcchInd));
         if(macPcchInd == NULLP)
         {
            DU_LOG("\nERROR  -->  DU APP: BuildAndSendDlPcchIndToMac(); (macPcchInd) Memory Alloction failed!");
            break;
         }
         
         macPcchInd->cellId = cellId;
         macPcchInd->pf = pf;
         macPcchInd->i_s = i_s;
         macPcchInd->pduLen = encBufSize;
         DU_ALLOC_SHRABL_BUF(macPcchInd->pcchPdu, macPcchInd->pduLen);
         if(macPcchInd->pcchPdu == NULLP)
         {
            DU_LOG("\nERROR  -->  DU APP: BuildAndSendDlPcchIndToMac(); (PcchPDU) Memory Alloction failed!");
            break;
         }
         memcpy(macPcchInd->pcchPdu, encBuf, macPcchInd->pduLen);
         ret = sendDlPcchIndToMac(macPcchInd);
         if(ret != ROK)
         {
            DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, macPcchInd->pcchPdu, macPcchInd->pduLen);
            DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, macPcchInd, sizeof(DlPcchInd));
            break;
         }
      }
      ret = ROK;
      break;
   }
   freePcchPdu(pcchMsg);
   return ret;
}

/*******************************************************************
 * @brief Keeping the record of Paging having a particular SFN and index associated 
 *
 * @details
 *
 *    Function : insertPagingRecord 
 *
 *    Functionality:  Insert record of Paging assoc with particular SFN and index associated 
 *
 * @params[in] DuCellCb* cellCb, uint16_t iterations
 *             DuPagingMsg  rcvdF1apPagingParam
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t insertPagingRecord(DuCellCb* cellCb, DuPagingMsg *rcvdF1apPagingParam, uint16_t iterations)
{
   uint16_t maxIterations = 0; 
   DuPagInfoList *pagInfoLLFromPF = NULLP;
   DuPagUeList  *pageUeLL = NULLP;
   DuPagUeRecord *ueRecord = NULLP;

#if 0
   printPageList(&(cellCb->pagingInfoMap));
#endif

   /*MAX Iteration : A UE can be paged at every T frames thus MAX determines
    *how many Paging Frame(s) can be considered for Paging this UE*/
   maxIterations = MAX_SFN/rcvdF1apPagingParam->T;
   if(iterations >= maxIterations)
   {
      DU_LOG("\nERROR  --> DU APP: MAX Iterations reached for UEID:%d, thus Paging is dropped!", rcvdF1apPagingParam->pagUeId);
      return RFAILED;
   }

   /*[Step1]: Extracting the PF from the HasMap between PF and PagingInfoList*/
   pagInfoLLFromPF = findPagingInfoFromMap(rcvdF1apPagingParam->pagingFrame, &(cellCb->pagingInfoMap));
   if(pagInfoLLFromPF != NULLP)
   {
      /*[Step2]: Extracting the PageInfo List for the Paging Indices(i_s)*/
      pageUeLL = handlePageInfoLL(rcvdF1apPagingParam->pagingFrame, rcvdF1apPagingParam->i_s, \
            &(pagInfoLLFromPF->pagInfoList), CREATE);

      if(pageUeLL != NULLP)
      {
         /*[Step3]: Check whether MAX UE Record against this PF and i_s has reached(Spec 38.331, Annexure A)*/
         if(pageUeLL->pagUeList.count < MAX_PAGING_UE_RECORDS)
         {
            /*[Step4]: Insert the Paging Record to the end of the UE record List*/
            ueRecord = handlePageUeLL(rcvdF1apPagingParam->pagUeId, rcvdF1apPagingParam->sTmsi,\
                  &(pageUeLL->pagUeList), CREATE);

            if(ueRecord == NULLP)
            {
               DU_LOG("\nERROR  --> DU APP: Unable to create UE Record in PagingList");
               return RFAILED;
            }
            DU_LOG("\nDEBUG  --> DU APP: UE Record created successfully in PagingList");
            return ROK;
         }
         else
         {
            /*Since MAX Page record has reached for this PF thus calculating and
             *moving this UE to next Paging Cycle*/
            DU_LOG("\nINFO   --> DU APP: Max Page Record reached for PagingFrame:%d",rcvdF1apPagingParam->pagingFrame);
            rcvdF1apPagingParam->pagingFrame = ((rcvdF1apPagingParam->pagingFrame + rcvdF1apPagingParam->T) % MAX_SFN);
            iterations++;

            return insertPagingRecord(cellCb, rcvdF1apPagingParam, iterations);
         }
      }
   }
   /*Reaching here means that PF has no entry in the PageLists,
    *Thus creating, Updating and inseritng the Paging Map, List and UE record.*/

   DU_ALLOC(pagInfoLLFromPF, sizeof(DuPagInfoList));

   if(pagInfoLLFromPF == NULLP)
   {
      DU_LOG("\nERROR  --> DU APP: PageInfo Map allocation failed.");
      return RFAILED;
   }
   pagInfoLLFromPF->pf = rcvdF1apPagingParam->pagingFrame;
   pageUeLL = handlePageInfoLL(rcvdF1apPagingParam->pagingFrame, rcvdF1apPagingParam->i_s, &(pagInfoLLFromPF->pagInfoList), CREATE);
   ueRecord = handlePageUeLL(rcvdF1apPagingParam->pagUeId, rcvdF1apPagingParam->sTmsi, &(pageUeLL->pagUeList), CREATE);
   if(cmHashListInsert(&(cellCb->pagingInfoMap), (PTR)pagInfoLLFromPF, (uint8_t *)&(pagInfoLLFromPF->pf), sizeof(uint16_t)) == RFAILED)
   {
      DU_LOG("\nERROR  --> DU APP: Hash Map Insertion Failed for PF:%d.",rcvdF1apPagingParam->pagingFrame);
   }

#if 0
   printPageList(&(cellCb->pagingInfoMap));
#endif

   return ROK;


}

/*******************************************************************
 * @brief Calculate and fill paging information of a UE belongs to a particular cell
 *
 * @details
 *
 *    Function : calcAndFillPagingInfoInCellCb
 *
 *    Functionality: Calculate PO and i_s and 
 *                   fill paging information of a UE in DuCellCb
 *
 * @params[in] DuCellCb* cellCb, uint8_t pagUeId, 
 *             DuPagingMsg  rcvdF1apPagingParam
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t calcAndFillPagingInfoInCellCb(DuCellCb* cellCb, DuPagingMsg *rcvdF1apPagingParam)
{
   uint8_t ns = 0;
   uint16_t T=0, N=0, pagingFrame = 0, n = 0;
   uint16_t currentSfn = 0, sfn = 0, newSfn = 0;
   PcchCfg   duPcchCfg;

   if(cellCb)
   {
      /* calculate paging frame and paging offset */
      duPcchCfg = duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg.pcchCfg;
      rcvdF1apPagingParam->pagingFrameOffset = duPcchCfg.pageFrameOffset;
      ns = duPcchCfg.ns;      

      /*
       * Fill the Value of T (DRX cycle of the UE)
       * T = min(UE Specific DRX value allocated by upper layers, default DRX
       * broadcast in System Information) */
      if((rcvdF1apPagingParam->pagingDrxPres) && (duPcchCfg.dfltPagingCycle > rcvdF1apPagingParam->pagingDrx))
      {
         T = rcvdF1apPagingParam->pagingDrx;
      } 
      else
      {
         T = duPcchCfg.dfltPagingCycle;
      }
      rcvdF1apPagingParam->T = T;

       /* N= number of total paging frames in T */

      switch(duPcchCfg.nAndPagingFrmOffsetType)
      {
         case PCCH_Config__nAndPagingFrameOffset_PR_oneT:
            N = T;
            break;
         case PCCH_Config__nAndPagingFrameOffset_PR_halfT:
            N = T/2;
            break;
         case PCCH_Config__nAndPagingFrameOffset_PR_quarterT:
            N = T/4;
            break;
         case PCCH_Config__nAndPagingFrameOffset_PR_oneEighthT:
            N = T/8;
            break;
         case PCCH_Config__nAndPagingFrameOffset_PR_oneSixteenthT:
            N = T/16;
            break;
         default:
            N = T;
            break;
      }

       /* calculate the Value of pagingFrame */
      /*Refer: 38.304 Sec 7.1: (SFN + PF_offset) mod T = (T div N)*(UE_ID mod N)*/
      //RHS of above formula
      pagingFrame = (T / N) * ((rcvdF1apPagingParam->pagUeId) % N);

      //LHS of above formula
      if(pagingFrame)
      {
         pagingFrame = (pagingFrame - rcvdF1apPagingParam->pagingFrameOffset)%T;
      }
      else /*Paging Frame = 0 so thus PF will be calculated on Paging Cycle*/
      {
         pagingFrame = (T - rcvdF1apPagingParam->pagingFrameOffset)%T;
      }

      /*Paging Frame(SFN for Paging) has to be determined from current SFN. */
      /*For eg: If T =128, PF(Calculated above) = 20; SFN can be 20,148,276,...
       * If currSFN is running as 200 then (newSFN % T) == (T/N)*(UE_ID%N)
       *Thus SFN at which paging has to be sent needs to be delayed and newSFN = 276*/

      currentSfn = cellCb->currSlotInfo.sfn;

      /*Multiplication Factor(x) to find the next best SFN to process paging*/

      /*Below calculation will determine in which nth cycle of T (DRX cycle),new PF
       * may fall(Delayed one)*/
      if(currentSfn > pagingFrame)
      {
         n  = ((currentSfn - pagingFrame) / T) + 1;
      }
      else
      {
         n  = ((pagingFrame - currentSfn) / T) + 1;
      }

      newSfn = pagingFrame + T * n;

      /*When pagingFrame is future from currSFN then pagingFrame will be used 
       * else pagingFrame is delayed thus newSFN will be used.*/
      if(currentSfn <= pagingFrame)
      {
         if(pagingFrame > currentSfn + PAGING_SCHED_DELTA)
         {
            sfn = pagingFrame;
         }
         else
         {
            sfn = newSfn;
         }
      }
      else
      {
         
         if(newSfn > currentSfn + PAGING_SCHED_DELTA)
         {
            sfn = newSfn;
         }
         else /*If newSFN is near to currSFN then delay it more by T*/
         {
            newSfn = newSfn + T;
            sfn = newSfn;
         }
      }
      rcvdF1apPagingParam->pagingFrame =  (sfn % MAX_SFN);
      rcvdF1apPagingParam->i_s = ((uint32_t)(floor(rcvdF1apPagingParam->pagUeId / N)) % ns);

      memcpy(&cellCb->tmpPagingInfoOfUe, rcvdF1apPagingParam, sizeof(DuPagingMsg));
   }
   else
   {
      DU_LOG("\nINFO  --> DU APP : calcAndFillPagingInfoInCellCb(): Received null pointer");
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 * @brief Paging Processing on a particular cell 
 *
 * @details
 *
 *    Function : processPagingMsg
 *
 *    Functionality: Process Paging on a particular cell 
 *
 * @params[in] uint16_t cellId
 *             DuPagingMsg  rcvdF1apPagingParam
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t processPagingMsg(uint16_t cellId, DuPagingMsg *rcvdF1apPagingParam)
{
   uint16_t cellIdx = 0, iteration = 0;

   GET_CELL_IDX(cellId, cellIdx);

   if(duCb.actvCellLst[cellIdx] == NULLP || duCb.actvCellLst[cellIdx]->cellId != cellId)
   {
      DU_LOG("\nERROR  -->  DU APP : processPagingMsg(): CellId[%d] is not found", cellId);
      return RFAILED;
   }
   
   if(calcAndFillPagingInfoInCellCb(duCb.actvCellLst[cellIdx], rcvdF1apPagingParam) != ROK)
   {
      DU_LOG("\nERROR  --> DU APP : CellCb:%d not present to fill UE Paging Information",cellId);
      return RFAILED;
   }
   if(insertPagingRecord(duCb.actvCellLst[cellIdx], rcvdF1apPagingParam, iteration) != ROK)
   {
      DU_LOG("\nERROR  --> DU APP : Insertion Failed ofUE Paging Information");
      return RFAILED;
   }
   return ROK;

}

/*******************************************************************
 *
 * @brief DU build and send dl broacast req  and send it to MAC
 *
 * @details
 *
 *    Function : duBuildAndSendDlBroadcastReq
 *
 *    Functionality: DU build and send dl broacast req and send to MAC
 *                   
 *
 * @params[in] cellId, crnti 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duBuildAndSendDlBroadcastReq()
{
   Pst pst;
   uint8_t ret =ROK;
   MacDlBroadcastReq *dlBroadcast=NULLP;

   DU_LOG("\nDEBUG  -->  DU_APP : Building Dl broadcast request");

   DU_ALLOC_SHRABL_BUF(dlBroadcast, sizeof(MacDlBroadcastReq));
   if(dlBroadcast)
   {
      /*TODO - fill MAC DL Broadcast Request*/
      
      FILL_PST_DUAPP_TO_MAC(pst, EVENT_MAC_DL_BROADCAST_REQ);

      DU_LOG("\nDEBUG  -->  DU_APP: Sending Dl broadcast  Request to MAC ");
      ret = (*packMacDlBroadcastReqOpts[pst.selector])(&pst, dlBroadcast);
      if(ret == RFAILED)
      {
         DU_LOG("\nERROR  -->  DU_APP: sendDlBroadcastReqToMac(): Failed to send Dl broadcast  Req to MAC");
         DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, dlBroadcast, sizeof(MacDlBroadcastReq));
      }
   }
   else
   {
      DU_LOG("\nERROR  -->   DU_APP: sendDlBroadcastReqToMac(): Failed to allocate memory"); 
      ret =  RFAILED;
   }

   return ret;
}

/**********************************************************************
  End of file
 **********************************************************************/
