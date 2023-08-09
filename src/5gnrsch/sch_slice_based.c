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

Desc:     C source code for Slice based scheduling algorithm

File:     sch_slice_based.c

 **********************************************************************/

/** @file sch_slot_ind.c
  @brief This module processes slot indications
 */
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
#include "sch_slice_based.h"
#ifdef NR_DRX 
#include "sch_drx.h"
#endif

/*******************************************************************
 *
 * @brief Function to handle Cell configuration request
 *
 * @details
 *
 *    Function : schSliceBasedCellCfgReq
 *
 *    Functionality: This function configures cell paremeters
 *       required for Slice Based scheduling
 *
 * @params[in] SchCellCb *cellCb, Cell control block
 * @return ROK
 *         RFAILED
 *
 * ****************************************************************/
uint8_t schSliceBasedCellCfgReq(SchCellCb *cellCb)
{
   SchSliceBasedCellCb *schSpcCellCb = NULLP;
   
   SCH_ALLOC(schSpcCellCb, sizeof(SchSliceBasedCellCb));
   if(!schSpcCellCb)
   {
      DU_LOG("\nERROR  -->  SCH: Memory allocation failed in schSliceBasedCellCfgReq");
      return RFAILED;
   }
   cmLListInit(&schSpcCellCb->ueToBeScheduled);
   cellCb->schSpcCell = (void *)schSpcCellCb;
   return ROK;
}

/*******************************************************************
 *
 * @brief Function to handle Cell configuration request
 *
 * @details
 *
 *    Function : schSliceBasedCellDelReq
 *
 *    Functionality: This function deletes/frees cell params
 *       specific to Slice Based scheduling
 *
 * @params[in] Pointer to Cell control block
 * @return void
 *
 * ****************************************************************/
void schSliceBasedCellDelReq(SchCellCb *cellCb)
{
   SchSliceBasedCellCb *schSpcCellCb = NULLP;
   CmLList *node=NULL, *next=NULL;

   schSpcCellCb = (SchSliceBasedCellCb *)cellCb->schSpcCell;
   /* Remove all UE from ueToBeScheduled list and deallocate */
   node = schSpcCellCb->ueToBeScheduled.first;
   while(node)
   {
      next = node->next;
      SCH_FREE(node->node, sizeof(uint8_t));
      cmLListDelFrm(&schSpcCellCb->ueToBeScheduled, node);
      SCH_FREE(node, sizeof(CmLList));
      node = next;
   }
   SCH_FREE(schSpcCellCb, sizeof(SchSliceBasedCellCb));
   cellCb->schSpcCell = NULLP;
}

/*******************************************************************
 *
 * @brief Function to handle UE configuration request
 *
 * @details
 *
 *    Function : SchSliceBasedAddUeConfigReq
 *
 *    Functionality: Adds/Configures UE parameters required for
 *       Slice Based scheduling
 *
 * @params[in] Pointer to UE control block
 * @return ROK
 *         RFAILED
 *
 * ****************************************************************/
uint8_t SchSliceBasedAddUeConfigReq(SchUeCb *ueCb)
{
   SchSliceBasedUeCb *ueSliceBasedCb;

   SCH_ALLOC(ueSliceBasedCb, sizeof(SchSliceBasedHqCb));
   if(!ueSliceBasedCb)
   {
      DU_LOG("\nERROR  -->  SCH: Memory allocation failed in SchSliceBasedAddUeConfigReq");
      return RFAILED;
   }
   
   cmLListInit(&ueSliceBasedCb->hqRetxCb.ulRetxHqList);
   cmLListInit(&ueSliceBasedCb->hqRetxCb.dlRetxHqList);
   ueCb->schSpcUeCb = (void *)ueSliceBasedCb;
   
   return ROK;
}

/*******************************************************************
 *
 * @brief Handles UE reconfiguration request
 *
 * @details
 *
 *    Function : SchSliceBasedModUeConfigReq
 *
 *    Functionality: 
 *
 * @params[in] Pointer to UE control block
 * @return void
 *
 * ****************************************************************/
void SchSliceBasedModUeConfigReq(SchUeCb *ueCb)
{
   /*TBD: No action required for Slice Based*/
   return;
}

/*******************************************************************
 *
 * @brief Handles UE Delete Request
 *
 * @details
 *
 *    Function : SchSliceBasedUeDeleteReq 
 *
 *    Functionality: Deletes/Frees UE parameters specific to 
 *       Slice Based scheduling
 *
 * @params[in] Pointer to UE control block
 * @return void
 *
 * ****************************************************************/
void SchSliceBasedUeDeleteReq(SchUeCb *ueCb)
{
   SchSliceBasedCellCb *schSpcCellCb = NULLP;
   SchSliceBasedUeCb *ueSliceBasedCb = NULLP;
   CmLList *node=NULL, *next=NULL;
   uint8_t ueId = 0;

   schSpcCellCb = (SchSliceBasedCellCb *)ueCb->cellCb->schSpcCell;
   /* Remove all UE from ueToBeScheduled list and deallocate */
   node = schSpcCellCb->ueToBeScheduled.first;
   while(node)
   {
      next = node->next;
      ueId = *(uint8_t *)node->node;
      if(ueId == ueCb->ueId)
      {
        SCH_FREE(node->node, sizeof(uint8_t));
        cmLListDelFrm(&schSpcCellCb->ueToBeScheduled, node);
        SCH_FREE(node, sizeof(CmLList));
        break;
      }
      node = next;
   }
   
   ueSliceBasedCb = (SchSliceBasedUeCb *)ueCb->schSpcUeCb;
   cmLListDeleteLList(&ueSliceBasedCb->hqRetxCb.ulRetxHqList);
   cmLListDeleteLList(&ueSliceBasedCb->hqRetxCb.dlRetxHqList);

   SCH_FREE(ueSliceBasedCb, sizeof(SchSliceBasedUeCb));
   ueCb->schSpcUeCb = NULLP;
   return;
}

/*******************************************************************
 *
 * @brief Intializes HARQ Process control block
 *
 * @details
 *
 *    Function : schSliceBasedInitDlHqProcCb
 *
 *    Functionality: Intitialized parameters of HARQ process control
 *       block specific to Slice Based scheduling in Downlink
 *
 * @params[in] Pointer to Downlink HARQ Process control block
 * @return ROK
 *         RFAILED
 *
 * ****************************************************************/
uint8_t schSliceBasedInitDlHqProcCb(SchDlHqProcCb *hqP)
{
   SchSliceBasedHqProcCb *schSpcHqP;

   SCH_ALLOC(schSpcHqP, sizeof(SchSliceBasedHqProcCb));
   if(!schSpcHqP)
   {
      DU_LOG("\nERROR  -->  SCH: Memory allocation failed in schSliceBasedInitDlHqProcCb");
      return RFAILED;
   }

   cmLListInit(&schSpcHqP->lcCb.dedLcList);
   cmLListInit(&schSpcHqP->lcCb.defLcList);
   hqP->schSpcDlHqProcCb = (void *)schSpcHqP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Handles HARQ Process delete request
 *
 * @details 
 *
 *    Function : schSliceBasedDeleteDlHqProcCb
 *
 *    Functionality: Deletes HARQ Process parameters specific to 
 *       Slice Based scheduling in Downlink
 *
 * @params[in] Pointer to Downlink HARQ Process control block
 * @return void
 *
 * ****************************************************************/
void schSliceBasedDeleteDlHqProcCb(SchDlHqProcCb *hqP)
{
   SchSliceBasedHqProcCb *schSpcHqP = (SchSliceBasedHqProcCb *)hqP->schSpcDlHqProcCb;
   cmLListDeleteLList(&schSpcHqP->lcCb.dedLcList);
   cmLListDeleteLList(&schSpcHqP->lcCb.defLcList);
   SCH_FREE(schSpcHqP, sizeof(SchSliceBasedHqProcCb));
   hqP->schSpcDlHqProcCb = NULLP;
}

/*******************************************************************
 *
 * @brief Intializes HARQ Process control block
 *
 * @details
 *
 *    Function : schSliceBasedInitUlHqProcCb
 *
 *    Functionality: Intitialized parameters of HARQ process control
 *       block specific to Slice Based scheduling in Uplink
 *
 * @params[in] Pointer to Uplink HARQ Process control block
 * @return ROK
 *         RFAILED
 *
 * ****************************************************************/
uint8_t schSliceBasedInitUlHqProcCb(SchUlHqProcCb *hqP)
{
   SchSliceBasedHqProcCb *schSpcHqP;

   SCH_ALLOC(schSpcHqP, sizeof(SchSliceBasedHqProcCb));
   if(!schSpcHqP)
   {
      DU_LOG("\nERROR  -->  SCH: Memory allocation failed in schSliceBasedInitUlHqProcCb");
      return RFAILED;
   }
   cmLListInit(&schSpcHqP->lcCb.dedLcList);
   cmLListInit(&schSpcHqP->lcCb.defLcList);
   hqP->schSpcUlHqProcCb = (void *)schSpcHqP;
 
   return ROK;
}

/*******************************************************************
 *
 * @brief Handled Deletion of HARQ Process control block
 *
 * @details
 *
 *    Function : schSliceBasedDeleteUlHqProcCb
 *
 *    Functionality: Deletes HARQ Process parameters specific to 
 *       Slice Based scheduling in Uplink
 *
 * @params[in] Pointer to Uplink HARQ Process Control block
 * @return void
 *
 * ****************************************************************/
void schSliceBasedDeleteUlHqProcCb(SchUlHqProcCb *hqP)
{
   SchSliceBasedHqProcCb *schSpcHqP = (SchSliceBasedHqProcCb *)hqP->schSpcUlHqProcCb;
   cmLListDeleteLList(&schSpcHqP->lcCb.dedLcList);
   cmLListDeleteLList(&schSpcHqP->lcCb.defLcList);
   SCH_FREE(schSpcHqP, sizeof(SchSliceBasedHqProcCb));
   hqP->schSpcUlHqProcCb = NULLP;
}

/*******************************************************************
 *
 * @brief Handles freeing of HARQ Process
 *
 * @details
 *
 *    Function : schSliceBasedFreeDlHqProcCb
 *
 *    Functionality: Frees HARQ Process parameters specific to 
 *       Slice Based scheduling in Downlink when HARQ process becomes free
 *
 * @params[in] Pointer to HARQ process control block
 * @return void
 *
 * ****************************************************************/
void schSliceBasedFreeDlHqProcCb(SchDlHqProcCb *hqP)
{
   SchSliceBasedHqProcCb *schSpcHqP = (SchSliceBasedHqProcCb *)hqP->schSpcDlHqProcCb;

   cmLListDeleteLList(&schSpcHqP->lcCb.dedLcList);
   cmLListDeleteLList(&schSpcHqP->lcCb.defLcList);
}

/*******************************************************************
 *
 * @brief Handles freeing of HARQ Process
 *
 * @details
 *
 *    Function : schSliceBasedFreeUlHqProcCb
 *
 *    Functionality: Frees HARQ Process parameters specific to 
 *       Slice Based scheduling in Uplink when HARQ process becomes free
 *
 * @params[in] Pointer to HARQ process control block
 * @return void
 *
 * ****************************************************************/
void schSliceBasedFreeUlHqProcCb(SchUlHqProcCb *hqP)
{
   SchSliceBasedHqProcCb *schSpcHqP = (SchSliceBasedHqProcCb *)hqP->schSpcUlHqProcCb;

   cmLListDeleteLList(&schSpcHqP->lcCb.dedLcList);
   cmLListDeleteLList(&schSpcHqP->lcCb.defLcList);
}

/*******************************************************************
 *
 * @brief Adds HARQ process to retransmission list
 *
 * @details
 *
 *    Function : schSliceBasedAddToDlHqRetxList
 *
 *    Functionality: Adds HARQ process to retransmission list
 *
 * @params[in] Pointer to Downlink HARQ Process 
 * @return void
 *
 * ****************************************************************/
void schSliceBasedAddToDlHqRetxList(SchDlHqProcCb *hqP)
{
   SchSliceBasedUeCb *schSpcUeCb;

   schSpcUeCb = (SchSliceBasedUeCb *)hqP->hqEnt->ue->schSpcUeCb;
   cmLListAdd2Tail(&(schSpcUeCb->hqRetxCb.dlRetxHqList),&hqP->dlHqProcLink);  
#ifdef NR_DRX
   if(hqP->hqEnt->ue->ueDrxInfoPres == true)
   {
      schDrxStrtDlHqRttTmr(hqP);
   }
   else
#endif
   {
      schSliceBasedAddUeToSchedule(hqP->hqEnt->cell, hqP->hqEnt->ue->ueId);
   }
}

/*******************************************************************
 *
 * @brief Adds HARQ process to retransmission list
 *
 * @details
 *
 *    Function : schSliceBasedAddToUlHqRetxList
 *
 *    Functionality: Adds HARQ process to retransmission list
 *
 * @params[in] Pointer to Uplink HARQ Process 
 * @return void
 *
 * ****************************************************************/
void schSliceBasedAddToUlHqRetxList(SchUlHqProcCb *hqP)
{
   SchSliceBasedUeCb *schSpcUeCb;

   schSpcUeCb = (SchSliceBasedUeCb *)hqP->hqEnt->ue->schSpcUeCb;
   cmLListAdd2Tail(&(schSpcUeCb->hqRetxCb.ulRetxHqList),&hqP->ulHqProcLink);  
#ifdef NR_DRX
   if(hqP->hqEnt->ue->ueDrxInfoPres == true)
   {
      schDrxStrtUlHqRttTmr(hqP);
   }
   else
#endif
   {
      schSliceBasedAddUeToSchedule(hqP->hqEnt->cell, hqP->hqEnt->ue->ueId);
   }   
}

/*******************************************************************
 *
 * @brief Add UE to ueToBeScheduled List
 *
 * @details
 *
 *    Function : schSliceBasedAddUeToSchedule
 *
 *    Functionality:
 *      Search if UE entry present in the list
 *      If yes, return.
 *      If no, add UE to the list
 *
 * @params[in] Cell control block
 *             Ue Id to be added
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t schSliceBasedAddUeToSchedule(SchCellCb *cellCb, uint16_t ueIdToAdd)
{
   SchSliceBasedCellCb *schSpcCellCb = NULLP;
   uint8_t *ueId;
   CmLList *node;

   schSpcCellCb = (SchSliceBasedCellCb *)cellCb->schSpcCell;
   /* Search if UE entry is already present in ueToBeScheduled list.
    * If yes, another entry for same UE not needed. Hence, return */
   node = schSpcCellCb->ueToBeScheduled.first;
   while(node)
   {
      ueId = (uint8_t *)node->node;
      if(*ueId == ueIdToAdd)
         return ROK;
      node = node->next;
   }

   /* If UE entry not present already, add UE to the end of ueToBeScheduled list */
   SCH_ALLOC(ueId, sizeof(uint8_t));
   if(!ueId)
   {
      DU_LOG("\nERROR  -->  SCH : Memory allocation failure in schSliceBasedAddUeToSchedule");
      return RFAILED;
   }
   *ueId = ueIdToAdd;
   if(addNodeToLList(&schSpcCellCb->ueToBeScheduled, ueId, NULLP) != ROK)
   {
      DU_LOG("\nERROR  --> SCH : Failed to add ueId [%d] to cell->ueToBeScheduled list", *ueId);
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Handled CRC Indication
 *
 * @details
 *
 *    Function : schSliceBasedProcessCrcInd
 *
 *    Functionality: Processes CRC Indication as required for Slice Based
 *       scheduling
 *
 * @params[in] Pointer to Cell control block
 *             UE Id
 * @return void
 *
 * ****************************************************************/
void schSliceBasedProcessCrcInd(SchCellCb *cellCb, uint16_t ueId)
{
   schSliceBasedAddUeToSchedule(cellCb, ueId);   
}

/*******************************************************************
 *
 * @brief Processes Buffer Occupancy report from RLC
 *
 * @details
 *
 *    Function : schSliceBasedDlRlcBoInfo
 *
 *    Functionality: Process buffer occupany report 
 *
 * @params[in]
 * @return
 *
 * ****************************************************************/
void schSliceBasedDlRlcBoInfo(SchCellCb *cellCb, uint16_t ueId)
{
   schSliceBasedAddUeToSchedule(cellCb, ueId);   
}

/*******************************************************************
 *
 * @brief Processes BSR request
 *
 * @details
 *
 *    Function : schSliceBasedBsr
 *
 *    Functionality: Processes BSR as per Slice Based scheduling
 *
 * @params[in] Pointer to Cell 
 *             UE ID
 * @return void
 *
 * ****************************************************************/
void schSliceBasedBsr(SchCellCb *cellCb, uint16_t ueId)
{
   schSliceBasedAddUeToSchedule(cellCb, ueId);   
}

/*******************************************************************
 *
 * @brief Processed UCI Indication
 *
 * @details
 *
 *    Function : schSliceBasedSrUciInd
 *
 *    Functionality: Processing of UCI indication specific to 
 *       Slice Based scheduling
 *
 * @params[in] Pointer to Cell
 *             UE Id
 * @return void
 *
 * ****************************************************************/
void schSliceBasedSrUciInd(SchCellCb *cellCb, uint16_t ueId)
{
   schSliceBasedAddUeToSchedule(cellCb, ueId);   
}

/*******************************************************************
 *
 * @brief Processing of RACH Indication
 *
 * @details
 *
 *    Function : schSliceBasedProcessRachInd
 *
 *    Functionality: Processing of RACH Indication specific to 
 *       Slice Based scheduling
 *
 * @params[in] Pointer to Cell Cb
 *             UE Id
 * @return void
 *
 * ****************************************************************/
void schSliceBasedProcessRachInd(SchCellCb *cellCb, uint16_t ueId)
{
   schSliceBasedAddUeToSchedule(cellCb, ueId);   
}

/*******************************************************************
 *
 * @brief Processing of DL HARQ Indication
 *
 * @details
 *
 *    Function : SchSliceBasedDlHarqInd
 *
 *    Functionality:
 *
 * @params[in] 
 * @return void
 *
 * ****************************************************************/
void SchSliceBasedDlHarqInd()
{
   return;
}

/*******************************************************************
 *
 * @brief Processing of Paging indication
 *
 * @details
 *
 *    Function : schSliceBasedPagingInd
 *
 *    Functionality:
 *
 * @params[in]
 * @return void
 *
 * ****************************************************************/
void schSliceBasedPagingInd()
{
   return;
}

/*******************************************************************
 *
 * @brief Processing of RACH Resource Request
 *
 * @details
 *
 *    Function : schSliceBasedRachRsrcReq
 *
 *    Functionality:
 *
 * @params[in]
 * @return void
 *
 * ****************************************************************/
void schSliceBasedRachRsrcReq()
{
   return;
}

/*******************************************************************
 *
 * @brief Processing of RACH Resource Release
 *
 * @details
 *
 *    Function : schSliceBasedRachRsrcRel
 *
 *    Functionality:
 *
 * @params[in]
 * @return void
 *
 * ****************************************************************/
void schSliceBasedRachRsrcRel()
{
   return;
}

/*******************************************************************
 *
 * @brief Remove entry from HARQ retransmission list
 *
 * @details
 *
 *    Function : schSliceBasedRemoveFrmDlHqRetxList
 *
 *    Functionality: Remove entry from HARQ retransmission list
 *
 * @params[in] Pointer to UE
 *             Node to be deleted
 * @return void
 *
 * ****************************************************************/
void schSliceBasedRemoveFrmDlHqRetxList(SchUeCb *ueCb, CmLList *node)
{
   SchSliceBasedUeCb *schSpcUeCb;

   schSpcUeCb = (SchSliceBasedUeCb *)ueCb->schSpcUeCb;
   cmLListDelFrm(&schSpcUeCb->hqRetxCb.dlRetxHqList, node);
}

/*******************************************************************
 *
 * @brief Remove entry from HARQ retransmission list
 *
 * @details
 *
 *    Function : schSliceBasedRemoveFrmUlHqRetxList
 *
 *    Functionality: Remove entry from HARQ retransmission list
 *
 * @params[in] Pointer to UE
 *             Node to be deleted
 * @return void
 *
 * ****************************************************************/
void schSliceBasedRemoveFrmUlHqRetxList(SchUeCb *ueCb, CmLList *node)
{
   SchSliceBasedUeCb *schSpcUeCb;

   schSpcUeCb = (SchSliceBasedUeCb *)ueCb->schSpcUeCb;
   cmLListDelFrm(&schSpcUeCb->hqRetxCb.ulRetxHqList, node);
}

/*******************************************************************
 *
 * @brief Remove UE from Scheduling List
 *
 * @details
 *
 *    Function : schSliceBasedRemoveUeFrmScheduleLst
 *
 *    Functionality: Remove UE from Scheduling List
 *
 * @params[in] Pointer to Cell
 *             Node to be removed
 * @return void
 *
 * ****************************************************************/
void schSliceBasedRemoveUeFrmScheduleLst(SchCellCb *cell, CmLList *node)
{
   SchSliceBasedCellCb *schSpcCell;

   schSpcCell = (SchSliceBasedCellCb *)cell->schSpcCell;
   SCH_FREE(node->node, sizeof(uint8_t));
   deleteNodeFromLList(&schSpcCell->ueToBeScheduled, node);
}

/*******************************************************************
 *
 * @brief  Handler to calculate TBS size for BSR requested
 *
 * @details
 *
 *    Function :  schSliceBasedCalculateUlTbs
 *
 *    Functionality: Function will note the required TBS for each LCGIDX and use
 *    the Priority LCG List and RRM policy to allocate the TBS size
 *
 * @params [in] ueCb (Pointer to UE CB)
 *         [in] puschTime (Time slot where PUSCH will be sent)
 *         [in] symbLen (No of Symbols used for PUSCH transmission)
 *         [out] startPrb(Pointer to startPRB which will be calculated while
 *         finding the best Free Block)
 *         [out] totTBS(Pointer to total TBS size)
 *         [in] isRetx (to indicate retransmission)
 *         [in] hqP (UL Harq process pointer)
 *
 * @return uint8_t : ROK > Scheduling of UL grant is successful
 *                   RFAILED > vice versa
 *
 * ****************************************************************/
uint8_t schSliceBasedCalculateUlTbs(SchUeCb *ueCb, SlotTimingInfo puschTime, uint8_t symbLen,\
                          uint16_t *startPrb, uint32_t *totTBS, bool isRetx, SchUlHqProcCb *hqP, SchSliceBasedHqProcCb *schSpcHqP)
{
   uint16_t mcsIdx = 0;
   CmLListCp *lcLL = NULLP;
   uint16_t lcgIdx = 0, lcId =0, maxFreePRB = 0;
   uint16_t rsvdDedicatedPRB;
   *startPrb = 0;
   *totTBS = 0;

   /* check for BSR */
   for(lcgIdx=0; lcgIdx<MAX_NUM_LOGICAL_CHANNEL_GROUPS; lcgIdx++)
   {
      if(ueCb->bsrInfo[lcgIdx].dataVol == 0)
      {
         continue;
      }

      /*TODO: lcgIdx and LCID has been implemented as one to one mapping.
       * Need to check the mapping to figure out the LCID and lcgIdx once L2
       * spec specifies any logic*/
      lcId = lcgIdx;
      if(ueCb->ulInfo.ulLcCtxt[lcId].isDedicated)
      {
         lcLL = &(schSpcHqP->lcCb.dedLcList);
         rsvdDedicatedPRB = ueCb->ulInfo.ulLcCtxt[lcId].rsvdDedicatedPRB;
      }
      else
      {
         lcLL = &(schSpcHqP->lcCb.defLcList);
      }

      /*[Step2]: Update the reqPRB and Payloadsize for this LC in the appropriate List*/
      if(updateLcListReqPRB(lcLL, lcId, ueCb->bsrInfo[lcgIdx].dataVol) != ROK)
      {
         DU_LOG("\nERROR  --> SCH: LcgId:%d updation failed",lcId);         
         return RFAILED;
      }
   }

   if ((schSpcHqP->lcCb.defLcList.count == 0) && (schSpcHqP->lcCb.dedLcList.count == 0))
   {
      if( (ueCb->srRcvd) || (isRetx) )
      {
         *startPrb = MAX_NUM_RB;
         *totTBS = schCalcTbSize(UL_GRANT_SIZE);
      }
      /*Returning true when NO Grant is there for UE as this is not scheduling
       * error*/      
      return ROK;
   }

   maxFreePRB = searchLargestFreeBlock(ueCb->cellCb, puschTime, startPrb, DIR_UL);

   /*[Step4]: Estimation of PRB and BO which can be allocated to each LC in
    * the list based on RRM policy*/

   /*Either this UE contains no reservedPRB pool fir dedicated S-NSSAI or 
    * Num of Free PRB available is not enough to reserve Dedicated PRBs*/
   if(maxFreePRB != 0)
   {
      mcsIdx = ueCb->ueCfg.ulModInfo.mcsIndex;
      if((schSpcHqP->lcCb.dedLcList.count == 0) || ((maxFreePRB < rsvdDedicatedPRB)))
      {
         schSpcHqP->lcCb.sharedNumPrb = maxFreePRB;
         DU_LOG("\nDEBUG  -->  SCH : UL Only Default Slice is scheduled, sharedPRB Count:%d",\
               schSpcHqP->lcCb.sharedNumPrb);

         /*PRB Alloc for Default LCs*/
         prbAllocUsingRRMPolicy(&(schSpcHqP->lcCb.defLcList), FALSE, mcsIdx, symbLen,\
               &(schSpcHqP->lcCb.sharedNumPrb), NULLP, NULLP,&(ueCb->srRcvd));
      }
      else
      {
         schSpcHqP->lcCb.sharedNumPrb = maxFreePRB - rsvdDedicatedPRB;

         /*PRB Alloc for Dedicated LCs*/
         prbAllocUsingRRMPolicy(&(schSpcHqP->lcCb.dedLcList), TRUE, mcsIdx, symbLen,\
               &(schSpcHqP->lcCb.sharedNumPrb), &(rsvdDedicatedPRB),\
               NULLP, &(ueCb->srRcvd));

         /*PRB Alloc for Default LCs*/
         prbAllocUsingRRMPolicy(&(schSpcHqP->lcCb.defLcList), FALSE, mcsIdx, symbLen, \
               &(schSpcHqP->lcCb.sharedNumPrb), &(rsvdDedicatedPRB),\
               NULLP,&(ueCb->srRcvd));
      }
   }
   /*[Step5]:Traverse each LCID in LcList to calculate the exact Scheduled Bytes
    * using allocated BO per LC and Update dlMsgAlloc(BO report for MAC*/ 
   if(schSpcHqP->lcCb.dedLcList.count != 0)
      updateGrantSizeForBoRpt(&(schSpcHqP->lcCb.dedLcList), NULLP, ueCb->bsrInfo, totTBS);

   updateGrantSizeForBoRpt(&(schSpcHqP->lcCb.defLcList), NULLP, ueCb->bsrInfo, totTBS);

   /*Below case will hit if NO LC(s) are allocated due to resource crunch*/
   if (*totTBS == 0)
   {
      if(maxFreePRB == 0)
      {
         DU_LOG("\nERROR  --> SCH : NO FREE PRB!!");
      }
      else
      {
         /*Schedule the LC for next slot*/
         DU_LOG("\nDEBUG  -->  SCH : No LC has been scheduled");
      }      
      return RFAILED;
   }   
   return ROK;
}

/*******************************************************************
 *
 * @brief Grants resources to LC in uplink
 *
 * @details
 *
 *    Function : schProcessSrOrBsrReq
 *
 *    Functionality:
 *       Grants resources to LC in uplink
 *
 * @params[in] PDCCH Time
 *             PUSCH Time
 *             Start Symbol
 *             Number of symbols
 *             Is retransmission
 *             HARQ process
 * @return ROK
 *         RFAILED
 *
 *******************************************************************/
uint8_t schSliceBasedScheduleUlLc(SlotTimingInfo dciTime, SlotTimingInfo puschTime, uint8_t startSymb , uint8_t symbLen, bool isRetx, SchUlHqProcCb **hqP)
{
   SchCellCb *cell;
   SchSliceBasedHqProcCb *schSpcHqProcCb;
   uint8_t ret = RFAILED;
   uint16_t startPrb = 0;
   uint32_t totDataReq = 0; /* in bytes */
   SchUeCb *ueCb;
   SchPuschInfo *puschInfo;
   DciInfo  *dciInfo = NULLP;

   cell = (*hqP)->hqEnt->cell;
   ueCb = (*hqP)->hqEnt->ue;
   schSpcHqProcCb = (SchSliceBasedHqProcCb *)(*hqP)->schSpcUlHqProcCb;
   ret = schSliceBasedCalculateUlTbs(ueCb, puschTime, symbLen, &startPrb, &totDataReq, isRetx, *hqP, schSpcHqProcCb);

   if(totDataReq > 0 && ret == ROK)
   {
      SCH_ALLOC(dciInfo, sizeof(DciInfo));
      if(!dciInfo)
      {
         DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for dciInfo alloc");
         if(isRetx != TRUE)
         {
            if(schSpcHqProcCb->lcCb.dedLcList.count != 0)
               updateBsrAndLcList(&(schSpcHqProcCb->lcCb.dedLcList), ueCb->bsrInfo, RFAILED);

            updateBsrAndLcList(&(schSpcHqProcCb->lcCb.defLcList), ueCb->bsrInfo, RFAILED);
         }
         return RFAILED;
      }
      cell->schDlSlotInfo[dciTime.slot]->ulGrant = dciInfo;
      memset(dciInfo,0,sizeof(DciInfo));

      /* Update PUSCH allocation */
      if(schFillPuschAlloc(ueCb, puschTime, totDataReq, startSymb, symbLen, startPrb, isRetx, *hqP) == ROK)
      {
         if(cell->schUlSlotInfo[puschTime.slot]->schPuschInfo)
         {
            puschInfo = cell->schUlSlotInfo[puschTime.slot]->schPuschInfo;
            if(puschInfo != NULLP)
            {
               /* Fill DCI for UL grant */
               schFillUlDci(ueCb, puschInfo, dciInfo, isRetx, *hqP);
               ueCb->srRcvd = false;
               ueCb->bsrRcvd = false;
               cell->schUlSlotInfo[puschTime.slot]->puschUe = ueCb->ueId;
               if(schSpcHqProcCb->lcCb.dedLcList.count != 0)
                  updateBsrAndLcList(&(schSpcHqProcCb->lcCb.dedLcList), ueCb->bsrInfo, ROK);
               updateBsrAndLcList(&(schSpcHqProcCb->lcCb.defLcList), ueCb->bsrInfo, ROK);
               cmLListAdd2Tail(&(ueCb->hqUlmap[puschTime.slot]->hqList), &(*hqP)->ulSlotLnk);                  
               return ROK;
            }
         }
      }
      if(schSpcHqProcCb->lcCb.dedLcList.count != 0)
         updateBsrAndLcList(&(schSpcHqProcCb->lcCb.dedLcList), ueCb->bsrInfo, RFAILED);
      updateBsrAndLcList(&(schSpcHqProcCb->lcCb.defLcList), ueCb->bsrInfo, RFAILED);
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Grants resources to LC in downlink 
 *
 * @details
 *
 *    Function : schSliceBasedScheduleDlLc 
 *
 *    Functionality: Grants resources to LC in uplink
 *
 * @params[in] PDCCH Time
 *
 * @return ROK
 *         RFAILED
 *
 * ****************************************************************/
uint32_t schSliceBasedScheduleDlLc(SlotTimingInfo pdcchTime, SlotTimingInfo pdschTime, uint8_t pdschNumSymbols,\
                                    uint16_t *startPrb, bool isRetx, SchDlHqProcCb **hqP)
{
   SchSliceBasedHqProcCb *schSpcHqProcCb;
   SchUeCb *ueCb;
   uint8_t lcIdx = 0;
   uint16_t maxFreePRB = 0;
   uint16_t mcsIdx = 0;
   uint32_t accumalatedSize = 0;
   CmLListCp *lcLL = NULLP;
   uint16_t rsvdDedicatedPRB = 0;
   DlMsgSchInfo *dciSlotAlloc;

   /* TX_PAYLOAD_HDR_LEN: Overhead which is to be Added once for any UE while estimating Accumulated TB Size
    * Following flag added to keep the record whether TX_PAYLOAD_HDR_LEN is added to the first Node getting allocated.
    * If both Dedicated and Default LC lists are present then First LC in Dedicated List will include this overhead
    * else if only Default list is present then first node in this List will add this overhead len*/
   bool isTxPayloadLenAdded = FALSE;

   ueCb = (*hqP)->hqEnt->ue;
   dciSlotAlloc = (*hqP)->hqEnt->cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueCb->ueId -1];
   schSpcHqProcCb = (SchSliceBasedHqProcCb *)((*hqP)->schSpcDlHqProcCb);

   if (isRetx == FALSE)
   {
      /*Re-Initalization per UE*/
      /* scheduled LC data fill */
      dciSlotAlloc->transportBlock[0].numLc = 0;
      isTxPayloadLenAdded = FALSE; /*Re-initlaize the flag for every UE*/
      accumalatedSize = 0;

      for(lcIdx = 0; lcIdx < MAX_NUM_LC; lcIdx++)
      {
         if(ueCb->dlInfo.dlLcCtxt[lcIdx].bo)
         {
            /*Check the LC is Dedicated or default and accordingly LCList will
            * be used*/
            if(ueCb->dlInfo.dlLcCtxt[lcIdx].isDedicated)
            {
               lcLL = &(schSpcHqProcCb->lcCb.dedLcList);
               rsvdDedicatedPRB = ueCb->dlInfo.dlLcCtxt[lcIdx].rsvdDedicatedPRB;
            }
            else
            {
               lcLL = &(schSpcHqProcCb->lcCb.defLcList);
            }

            /*[Step2]: Update the reqPRB and Payloadsize for this LC in the appropriate List*/
            if(updateLcListReqPRB(lcLL, ueCb->dlInfo.dlLcCtxt[lcIdx].lcId,\
                     (ueCb->dlInfo.dlLcCtxt[lcIdx].bo + MAC_HDR_SIZE)) != ROK)
            {
               DU_LOG("\nERROR  --> SCH : Updation in LC List Failed");
               /* Free the dl ded msg info allocated in macSchDlRlcBoInfo */
               if(!dciSlotAlloc->dlMsgPdschCfg)
               {
                  SCH_FREE(dciSlotAlloc, sizeof(DlMsgSchInfo));
                  (*hqP)->hqEnt->cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueCb->ueId -1] = NULL;
               }
               return accumalatedSize;
            }
         }
      }//End of for loop

      if ((schSpcHqProcCb->lcCb.defLcList.count == 0) && (schSpcHqProcCb->lcCb.dedLcList.count == 0))
      {
         DU_LOG("\nDEBUG  -->  SCH : No pending BO for any LC id\n");
         UNSET_ONE_BIT((*hqP)->hqEnt->ue->ueId, (*hqP)->hqEnt->cell->boIndBitMap);

         /* Free the dl ded msg info allocated in macSchDlRlcBoInfo */
         if(!dciSlotAlloc->dlMsgPdschCfg)
         {
            SCH_FREE(dciSlotAlloc, sizeof(DlMsgSchInfo));
            (*hqP)->hqEnt->cell->schDlSlotInfo[pdcchTime.slot]->dlMsgAlloc[ueCb->ueId -1] = NULL;
         }
         /*TRUE because this UE has nothing to be scheduled*/
         return accumalatedSize;
      }
   }

   /*[Step3]: Calculate Best FREE BLOCK with MAX PRB count*/
   maxFreePRB = searchLargestFreeBlock((*hqP)->hqEnt->cell, pdschTime, startPrb, DIR_DL);

   /*[Step4]: Estimation of PRB and BO which can be allocated to each LC in
    * the list based on RRM policy*/

   /*Either this UE contains no reservedPRB pool fir dedicated S-NSSAI or 
    * Num of Free PRB available is not enough to reserve Dedicated PRBs*/
   if(isRetx == FALSE)
   {
      if(maxFreePRB != 0)
      {
         mcsIdx = ueCb->ueCfg.dlModInfo.mcsIndex;

         if((schSpcHqProcCb->lcCb.dedLcList.count == NULLP) || ((maxFreePRB < rsvdDedicatedPRB)))
         { 
            schSpcHqProcCb->lcCb.sharedNumPrb = maxFreePRB;
            DU_LOG("\nDEBUG  -->  SCH : DL Only Default Slice is scheduled, sharedPRB Count:%d",\
                  schSpcHqProcCb->lcCb.sharedNumPrb);

            /*PRB Alloc for Default LCs*/
            prbAllocUsingRRMPolicy(&(schSpcHqProcCb->lcCb.defLcList), FALSE, mcsIdx, pdschNumSymbols,\
                  &(schSpcHqProcCb->lcCb.sharedNumPrb), NULLP, &isTxPayloadLenAdded, NULLP);
         }
         else
         {
            schSpcHqProcCb->lcCb.sharedNumPrb = maxFreePRB - rsvdDedicatedPRB;
            /*PRB Alloc for Dedicated LCs*/
            prbAllocUsingRRMPolicy(&(schSpcHqProcCb->lcCb.dedLcList), TRUE, mcsIdx, pdschNumSymbols,\
                  &(schSpcHqProcCb->lcCb.sharedNumPrb), &(rsvdDedicatedPRB), &isTxPayloadLenAdded, NULLP);

            /*PRB Alloc for Default LCs*/
            prbAllocUsingRRMPolicy(&(schSpcHqProcCb->lcCb.defLcList), FALSE, mcsIdx, pdschNumSymbols, \
                  &(schSpcHqProcCb->lcCb.sharedNumPrb), &(rsvdDedicatedPRB), &isTxPayloadLenAdded, NULLP);
         }
      }
   }

   /*[Step5]:Traverse each LCID in LcList to calculate the exact Scheduled Bytes
    * using allocated BO per LC and Update dlMsgAlloc BO report for MAC */
   if (isRetx == FALSE)
   {
      if(schSpcHqProcCb->lcCb.dedLcList.count != 0)
         updateGrantSizeForBoRpt(&(schSpcHqProcCb->lcCb.dedLcList), dciSlotAlloc, NULLP, &(accumalatedSize));

      updateGrantSizeForBoRpt(&(schSpcHqProcCb->lcCb.defLcList), dciSlotAlloc, NULLP, &(accumalatedSize));
   }
   else
   {
      accumalatedSize = (*hqP)->tbInfo[0].tbSzReq;
   }

   /*Below case will hit if NO LC(s) are allocated due to resource crunch*/
   if (!accumalatedSize)
   {
      if(maxFreePRB == 0)
      {
         DU_LOG("\nERROR  --> SCH : NO FREE PRB!!");
      }
      else
      {
         /*Schedule the LC for next slot*/
         DU_LOG("\nDEBUG  -->  SCH : No LC has been scheduled");
      }
      /* Not Freeing dlMsgAlloc as ZERO BO REPORT to be sent to RLC so that
       * Allocation can be done in next slot*/
      accumalatedSize = 0;
   }

   return accumalatedSize;
}

/*******************************************************************
 *
 * @brief Scheduling of Slots in UL And DL 
 *
 * @details
 *
 *    Function : schSliceBasedScheduleSlot
 *
 *    Functionality: Scheduling of slots in UL and DL specific to 
 *       Slice Based scheduling
 *
 * @params[in] Pointer to Cell
 *             Slot timing info
 *             Scheduler instance
 * @return void
 *
 * ****************************************************************/
void schSliceBasedScheduleSlot(SchCellCb *cell, SlotTimingInfo *slotInd, Inst schInst)
{
   SchSliceBasedCellCb  *schSpcCell;
   SchSliceBasedUeCb    *schSpcUeCb;
   SchDlHqProcCb  *hqP = NULLP;
   SchUlHqProcCb  *ulHqP = NULLP;
   CmLList        *pendingUeNode;
   CmLList        *node;
   uint8_t        ueId;
   bool           isRarPending = false, isRarScheduled = false;
   bool           isMsg4Pending = false, isMsg4Scheduled = false;
   bool           isDlMsgPending = false, isDlMsgScheduled = false;
   bool           isUlGrantPending = false, isUlGrantScheduled = false;

   schSpcCell = (SchSliceBasedCellCb *)cell->schSpcCell;
   
   /* Select first UE in the linked list to be scheduled next */
   pendingUeNode = schSpcCell->ueToBeScheduled.first;
   if(pendingUeNode)
   {
      if(pendingUeNode->node)
      {
         ueId = *(uint8_t *)(pendingUeNode->node);
         schSpcUeCb = (SchSliceBasedUeCb *)cell->ueCb[ueId-1].schSpcUeCb;

         /* If RAR is pending for this UE, schedule PDCCH,PDSCH to send RAR and 
          * PUSCH to receive MSG3 as per k0-k2 configuration*/
         if(cell->raReq[ueId-1] != NULLP)
         {
            isRarPending = true;
            isRarScheduled = schProcessRaReq(schInst, cell, *slotInd, ueId);
         }

         /*MSG3 retransmisson*/
         if(cell->raCb[ueId-1].retxMsg3HqProc)
         {            
            schMsg3RetxSchedulingForUe(&(cell->raCb[ueId-1]));
         }

         /* If MSG4 is pending for this UE, schedule PDCCH,PDSCH to send MSG4 and
          * PUCCH to receive UL msg as per k0-k1 configuration  */
         if (cell->ueCb[ueId-1].retxMsg4HqProc) //should work from dlmap later tbd
         {
            /* Retransmission of MSG4 */
            isMsg4Pending = true;
            if(schProcessMsg4Req(cell, *slotInd, ueId, TRUE, &cell->ueCb[ueId-1].retxMsg4HqProc) == ROK)
               isMsg4Scheduled = true;
         }
         else
         {
            /* First transmission of MSG4 */
            if(cell->raCb[ueId-1].msg4recvd)
            {
               isMsg4Pending = true;
               if(schProcessMsg4Req(cell, *slotInd, ueId, FALSE, &cell->ueCb[ueId-1].msg4HqProc) == ROK)
                  isMsg4Scheduled = true;

               /* If MSG4 scheduling failed, free the newly assigned HARQ process */
               if(!isMsg4Scheduled)
                  schDlReleaseHqProcess(cell->ueCb[ueId-1].msg4HqProc);
            }
         }

         if(isRarPending || isMsg4Pending)
         {
            /* If RAR or MSG is successfully scheduled then
             * remove UE from linked list since no pending msgs for this UE */
            if(isRarScheduled || isMsg4Scheduled)
            {
               schSliceBasedRemoveUeFrmScheduleLst(cell, pendingUeNode);
            }
            /* If RAR/MSG4 is pending but couldnt be scheduled then,
             * put this UE at the end of linked list to be scheduled later */
            else
            {
               cmLListAdd2Tail(&schSpcCell->ueToBeScheduled, cmLListDelFrm(&schSpcCell->ueToBeScheduled, pendingUeNode));
            }
         }

#ifdef NR_DRX 
         if((cell->ueCb[ueId-1].ueDrxInfoPres == true) && (cell->ueCb[ueId-1].drxUeCb.drxDlUeActiveStatus != true))
         {
            if(pendingUeNode->node)
            {
               cmLListAdd2Tail(&schSpcCell->ueToBeScheduled, cmLListDelFrm(&schSpcCell->ueToBeScheduled, pendingUeNode));
            }
         }
         else 
#endif
         {

            /* DL Data */
            node = NULLP;
            if(schSpcUeCb)
               node = schSpcUeCb->hqRetxCb.dlRetxHqList.first;
            if(node != NULLP)
            {
               /* DL Data ReTransmisson */
               isDlMsgPending = true;
               isDlMsgScheduled = schFillBoGrantDlSchedInfo(cell, *slotInd, ueId, TRUE, ((SchDlHqProcCb**) &(node->node)));
               if(isDlMsgScheduled)
               {
#ifdef NR_DRX 
                  schDrxStopDlHqRetxTmr(cell, &cell->ueCb[ueId-1], ((SchDlHqProcCb**) &(node->node)));
#endif
                  schSliceBasedRemoveFrmDlHqRetxList(&cell->ueCb[ueId-1], node);
               }
            }
            else
            {
               /* DL Data new transmission */
               if((cell->boIndBitMap) & (1<<ueId))
               {
                  isDlMsgPending = true;               
                  isDlMsgScheduled = schFillBoGrantDlSchedInfo(cell, *slotInd, ueId, FALSE, &hqP);

                  /* If DL scheduling failed, free the newly assigned HARQ process */
                  if(!isDlMsgScheduled)
                     schDlReleaseHqProcess(hqP);
                  else
                  {
#ifdef NR_DRX
                     schHdlDrxInActvStrtTmr(cell, &cell->ueCb[ueId-1], PHY_DELTA_DL + SCHED_DELTA);
#endif
                  }
               }
            }

            /* Scheduling of UL grant */
            node = NULLP;
            if(schSpcUeCb)
               node = schSpcUeCb->hqRetxCb.ulRetxHqList.first;
            if(node != NULLP)
            {
               /* UL Data ReTransmisson */
               isUlGrantPending = true;
               isUlGrantScheduled = schProcessSrOrBsrReq(cell, *slotInd, ueId, TRUE, (SchUlHqProcCb**) &(node->node));
               if(isUlGrantScheduled)
               {
#ifdef NR_DRX 
                  schDrxStopUlHqRetxTmr(cell, &cell->ueCb[ueId-1], ((SchUlHqProcCb**) &(node->node)));
#endif
                  schSliceBasedRemoveFrmUlHqRetxList(&cell->ueCb[ueId-1], node);
               }
            }
            else
            {
               /* UL Data new transmission */
               if(cell->ueCb[ueId-1].srRcvd || cell->ueCb[ueId-1].bsrRcvd)
               {
                  isUlGrantPending = true;
                  isUlGrantScheduled = schProcessSrOrBsrReq(cell, *slotInd, ueId, FALSE, &ulHqP);
                  if(!isUlGrantScheduled)
                     schUlReleaseHqProcess(ulHqP, FALSE);
                  else
                  {
#ifdef NR_DRX
                     schHdlDrxInActvStrtTmr(cell, &cell->ueCb[ueId-1], PHY_DELTA_UL + SCHED_DELTA);
#endif
                  }
               }
            }

            if(!isUlGrantPending && !isDlMsgPending)
            {
               /* No action required */  
            }
            else if((isUlGrantPending && !isUlGrantScheduled) || (isDlMsgPending && !isDlMsgScheduled))
            {
               cmLListAdd2Tail(&schSpcCell->ueToBeScheduled, cmLListDelFrm(&schSpcCell->ueToBeScheduled, pendingUeNode));
            }
            else
            {
               schSliceBasedRemoveUeFrmScheduleLst(cell, pendingUeNode);
            }
         }
      }
   }
}

/*******************************************************************
 *
 * @brief Initializes all function pointers to Slice Based function handler
 *
 * @details
 *
 *    Function : schSliceBasedAllApisInit
 *
 *    Functionality: Initializes all function pointers to Slice Based 
 *       function handler
 *
 * @params[in] Function pointer array
 * @return void
 *
 * ****************************************************************/
void schSliceBasedAllApisInit(SchAllApis *allSliceBasedApi)
{
    /* Interface API function pointers */
    allSliceBasedApi->SchCellCfgReq = schSliceBasedCellCfgReq;
    allSliceBasedApi->SchCellDeleteReq = schSliceBasedCellDelReq;
    allSliceBasedApi->SchAddUeConfigReq = SchSliceBasedAddUeConfigReq;
    allSliceBasedApi->SchModUeConfigReq = SchSliceBasedModUeConfigReq;
    allSliceBasedApi->SchUeDeleteReq = SchSliceBasedUeDeleteReq; 
    allSliceBasedApi->SchDlHarqInd = SchSliceBasedDlHarqInd; 
    allSliceBasedApi->SchCrcInd = schSliceBasedProcessCrcInd;
    allSliceBasedApi->SchRachInd = schSliceBasedProcessRachInd;
    allSliceBasedApi->SchPagingInd = schSliceBasedPagingInd;
    allSliceBasedApi->SchRachRsrcReq = schSliceBasedRachRsrcReq; 
    allSliceBasedApi->SchRachRsrcRel = schSliceBasedRachRsrcRel;
    allSliceBasedApi->SchDlRlcBoInfo = schSliceBasedDlRlcBoInfo;
    allSliceBasedApi->SchSrUciInd = schSliceBasedSrUciInd;
    allSliceBasedApi->SchBsr = schSliceBasedBsr;

    /* Internal API function pointers */
    allSliceBasedApi->SchAddToDlHqRetxList = schSliceBasedAddToDlHqRetxList;
    allSliceBasedApi->SchAddToUlHqRetxList = schSliceBasedAddToUlHqRetxList;
    allSliceBasedApi->SchRemoveFrmDlHqRetxList = schSliceBasedRemoveFrmDlHqRetxList;
    allSliceBasedApi->SchRemoveFrmUlHqRetxList = schSliceBasedRemoveFrmUlHqRetxList;
    allSliceBasedApi->SchAddUeToSchedule = schSliceBasedAddUeToSchedule;
    allSliceBasedApi->SchRemoveUeFrmScheduleLst = schSliceBasedRemoveUeFrmScheduleLst;
    allSliceBasedApi->SchInitDlHqProcCb = schSliceBasedInitDlHqProcCb;
    allSliceBasedApi->SchInitUlHqProcCb = schSliceBasedInitUlHqProcCb;
    allSliceBasedApi->SchFreeDlHqProcCb = schSliceBasedFreeDlHqProcCb;
    allSliceBasedApi->SchFreeUlHqProcCb = schSliceBasedFreeUlHqProcCb;
    allSliceBasedApi->SchDeleteDlHqProcCb = schSliceBasedDeleteDlHqProcCb;
    allSliceBasedApi->SchDeleteUlHqProcCb = schSliceBasedDeleteUlHqProcCb;
    allSliceBasedApi->SchScheduleSlot = schSliceBasedScheduleSlot;
    allSliceBasedApi->SchScheduleDlLc = schSliceBasedScheduleDlLc;
    allSliceBasedApi->SchScheduleUlLc = schSliceBasedScheduleUlLc;
}
/**********************************************************************
    End of file
**********************************************************************/

