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

Desc:     C source code for scheduling based on First Come First Serve
          algorithm

File:     sch_fcfs.c

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
#include "sch_fcfs.h"
#ifdef NR_DRX 
#include "sch_drx.h"
#endif

/*******************************************************************
 *
 * @brief Function to handle Cell configuration request
 *
 * @details
 *
 *    Function : schFcfsCellCfgReq
 *
 *    Functionality: This function configures cell paremeters
 *       required for FCFS scheduling
 *
 * @params[in] SchCellCb *cellCb, Cell control block
 * @return ROK
 *         RFAILED
 *
 * ****************************************************************/
uint8_t schFcfsCellCfgReq(SchCellCb *cellCb)
{
   SchFcfsCellCb *fcfsCellCb = NULLP;
   
   SCH_ALLOC(fcfsCellCb, sizeof(SchFcfsCellCb));
   if(!fcfsCellCb)
   {
      DU_LOG("\nERROR  -->  SCH: Memory allocation failed in schFcfsCellCfgReq");
      return RFAILED;
   }
   cmLListInit(&fcfsCellCb->ueToBeScheduled);
   cellCb->schSpcCell = (void *)fcfsCellCb;
   return ROK;
}

/*******************************************************************
 *
 * @brief Function to handle Cell configuration request
 *
 * @details
 *
 *    Function : schFcfsCellDelReq
 *
 *    Functionality: This function deletes/frees cell params
 *       specific to FCFS scheduling
 *
 * @params[in] Pointer to Cell control block
 * @return void
 *
 * ****************************************************************/
void schFcfsCellDelReq(SchCellCb *cellCb)
{
   SchFcfsCellCb *fcfsCellCb = NULLP;
   CmLList *node=NULL, *next=NULL;

   fcfsCellCb = (SchFcfsCellCb *)cellCb->schSpcCell;
   /* Remove all UE from ueToBeScheduled list and deallocate */
   node = fcfsCellCb->ueToBeScheduled.first;
   while(node)
   {
      next = node->next;
      SCH_FREE(node->node, sizeof(uint8_t));
      cmLListDelFrm(&fcfsCellCb->ueToBeScheduled, node);
      SCH_FREE(node, sizeof(CmLList));
      node = next;
   }
   SCH_FREE(fcfsCellCb, sizeof(SchFcfsCellCb));
   cellCb->schSpcCell = NULLP;
}

/*******************************************************************
 *
 * @brief Function to handle UE configuration request
 *
 * @details
 *
 *    Function : SchFcfsAddUeConfigReq
 *
 *    Functionality: Adds/Configures UE parameters required for
 *       FCFS scheduling
 *
 * @params[in] Pointer to UE control block
 * @return ROK
 *         RFAILED
 *
 * ****************************************************************/
uint8_t SchFcfsAddUeConfigReq(SchUeCb *ueCb)
{
   SchFcfsUeCb *ueFcfsCb;

   SCH_ALLOC(ueFcfsCb, sizeof(SchFcfsHqCb));
   if(!ueFcfsCb)
   {
      DU_LOG("\nERROR  -->  SCH: Memory allocation failed in SchFcfsAddUeConfigReq");
      return RFAILED;
   }
   
   cmLListInit(&ueFcfsCb->hqRetxCb.ulRetxHqList);
   cmLListInit(&ueFcfsCb->hqRetxCb.dlRetxHqList);
   ueCb->schSpcUeCb = (void *)ueFcfsCb;
   
   return ROK;
}

/*******************************************************************
 *
 * @brief Handles UE reconfiguration request
 *
 * @details
 *
 *    Function : SchFcfsModUeConfigReq
 *
 *    Functionality: 
 *
 * @params[in] Pointer to UE control block
 * @return void
 *
 * ****************************************************************/
void SchFcfsModUeConfigReq(SchUeCb *ueCb)
{
   /*TBD: No action required for FCFS*/
   return;
}

/*******************************************************************
 *
 * @brief Handles UE Delete Request
 *
 * @details
 *
 *    Function : SchFcfsUeDeleteReq 
 *
 *    Functionality: Deletes/Frees UE parameters specific to 
 *       FCFS scheduling
 *
 * @params[in] Pointer to UE control block
 * @return void
 *
 * ****************************************************************/
void SchFcfsUeDeleteReq(SchUeCb *ueCb)
{
   SchFcfsCellCb *fcfsCellCb = NULLP;
   SchFcfsUeCb *ueFcfsCb = NULLP;
   CmLList *node=NULL, *next=NULL;
   uint8_t ueId = 0;

   fcfsCellCb = (SchFcfsCellCb *)ueCb->cellCb->schSpcCell;
   /* Remove all UE from ueToBeScheduled list and deallocate */
   node = fcfsCellCb->ueToBeScheduled.first;
   while(node)
   {
      next = node->next;
      ueId = *(uint8_t *)node->node;
      if(ueId == ueCb->ueId)
      {
        SCH_FREE(node->node, sizeof(uint8_t));
        cmLListDelFrm(&fcfsCellCb->ueToBeScheduled, node);
        SCH_FREE(node, sizeof(CmLList));
        break;
      }
      node = next;
   }
   
   ueFcfsCb = (SchFcfsUeCb *)ueCb->schSpcUeCb;
   cmLListDeleteLList(&ueFcfsCb->hqRetxCb.ulRetxHqList);
   cmLListDeleteLList(&ueFcfsCb->hqRetxCb.dlRetxHqList);

   SCH_FREE(ueFcfsCb, sizeof(SchFcfsUeCb));
   ueCb->schSpcUeCb = NULLP;
   return;
}

/*******************************************************************
 *
 * @brief Intializes HARQ Process control block
 *
 * @details
 *
 *    Function : schFcfsInitDlHqProcCb
 *
 *    Functionality: Intitialized parameters of HARQ process control
 *       block specific to FCFS scheduling in Downlink
 *
 * @params[in] Pointer to Downlink HARQ Process control block
 * @return ROK
 *         RFAILED
 *
 * ****************************************************************/
uint8_t schFcfsInitDlHqProcCb(SchDlHqProcCb *hqP)
{
   SchFcfsHqProcCb *fcfsHqP;

   SCH_ALLOC(fcfsHqP, sizeof(SchFcfsHqProcCb));
   if(!fcfsHqP)
   {
      DU_LOG("\nERROR  -->  SCH: Memory allocation failed in schFcfsInitDlHqProcCb");
      return RFAILED;
   }

   cmLListInit(&fcfsHqP->lcCb.dedLcList);
   cmLListInit(&fcfsHqP->lcCb.defLcList);
   hqP->schSpcDlHqProcCb = (void *)fcfsHqP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Handles HARQ Process delete request
 *
 * @details 
 *
 *    Function : schFcfsDeleteDlHqProcCb
 *
 *    Functionality: Deletes HARQ Process parameters specific to 
 *       FCFS scheduling in Downlink
 *
 * @params[in] Pointer to Downlink HARQ Process control block
 * @return void
 *
 * ****************************************************************/
void schFcfsDeleteDlHqProcCb(SchDlHqProcCb *hqP)
{
   SchFcfsHqProcCb *fcfsHqP = (SchFcfsHqProcCb *)hqP->schSpcDlHqProcCb;
   cmLListDeleteLList(&fcfsHqP->lcCb.dedLcList);
   cmLListDeleteLList(&fcfsHqP->lcCb.defLcList);
   SCH_FREE(fcfsHqP, sizeof(SchFcfsHqProcCb));
   hqP->schSpcDlHqProcCb = NULLP;
}

/*******************************************************************
 *
 * @brief Intializes HARQ Process control block
 *
 * @details
 *
 *    Function : schFcfsInitUlHqProcCb
 *
 *    Functionality: Intitialized parameters of HARQ process control
 *       block specific to FCFS scheduling in Uplink
 *
 * @params[in] Pointer to Uplink HARQ Process control block
 * @return ROK
 *         RFAILED
 *
 * ****************************************************************/
uint8_t schFcfsInitUlHqProcCb(SchUlHqProcCb *hqP)
{
   SchFcfsHqProcCb *fcfsHqP;

   SCH_ALLOC(fcfsHqP, sizeof(SchFcfsHqProcCb));
   if(!fcfsHqP)
   {
      DU_LOG("\nERROR  -->  SCH: Memory allocation failed in schFcfsInitUlHqProcCb");
      return RFAILED;
   }
   cmLListInit(&fcfsHqP->lcCb.dedLcList);
   cmLListInit(&fcfsHqP->lcCb.defLcList);
   hqP->schSpcUlHqProcCb = (void *)fcfsHqP;
 
   return ROK;
}

/*******************************************************************
 *
 * @brief Handled Deletion of HARQ Process control block
 *
 * @details
 *
 *    Function : schFcfsDeleteUlHqProcCb
 *
 *    Functionality: Deletes HARQ Process parameters specific to 
 *       FCFS scheduling in Uplink
 *
 * @params[in] Pointer to Uplink HARQ Process Control block
 * @return void
 *
 * ****************************************************************/
void schFcfsDeleteUlHqProcCb(SchUlHqProcCb *hqP)
{
   SchFcfsHqProcCb *fcfsHqP = (SchFcfsHqProcCb *)hqP->schSpcUlHqProcCb;
   cmLListDeleteLList(&fcfsHqP->lcCb.dedLcList);
   cmLListDeleteLList(&fcfsHqP->lcCb.defLcList);
   SCH_FREE(fcfsHqP, sizeof(SchFcfsHqProcCb));
   hqP->schSpcUlHqProcCb = NULLP;
}

/*******************************************************************
 *
 * @brief Handles freeing of HARQ Process
 *
 * @details
 *
 *    Function : schFcfsFreeDlHqProcCb
 *
 *    Functionality: Frees HARQ Process parameters specific to 
 *       FCFS scheduling in Downlink when HARQ process becomes free
 *
 * @params[in] Pointer to HARQ process control block
 * @return void
 *
 * ****************************************************************/
void schFcfsFreeDlHqProcCb(SchDlHqProcCb *hqP)
{
   SchFcfsHqProcCb *fcfsHqP = (SchFcfsHqProcCb *)hqP->schSpcDlHqProcCb;

   cmLListDeleteLList(&fcfsHqP->lcCb.dedLcList);
   cmLListDeleteLList(&fcfsHqP->lcCb.defLcList);
}

/*******************************************************************
 *
 * @brief Handles freeing of HARQ Process
 *
 * @details
 *
 *    Function : schFcfsFreeUlHqProcCb
 *
 *    Functionality: Frees HARQ Process parameters specific to 
 *       FCFS scheduling in Uplink when HARQ process becomes free
 *
 * @params[in] Pointer to HARQ process control block
 * @return void
 *
 * ****************************************************************/
void schFcfsFreeUlHqProcCb(SchUlHqProcCb *hqP)
{
   SchFcfsHqProcCb *fcfsHqP = (SchFcfsHqProcCb *)hqP->schSpcUlHqProcCb;

   cmLListDeleteLList(&fcfsHqP->lcCb.dedLcList);
   cmLListDeleteLList(&fcfsHqP->lcCb.defLcList);
}

/*******************************************************************
 *
 * @brief Adds HARQ process to retransmission list
 *
 * @details
 *
 *    Function : schFcfsAddToDlHqRetxList
 *
 *    Functionality: Adds HARQ process to retransmission list
 *
 * @params[in] Pointer to Downlink HARQ Process 
 * @return void
 *
 * ****************************************************************/
void schFcfsAddToDlHqRetxList(SchDlHqProcCb *hqP)
{
   SchFcfsUeCb *fcfsUeCb;

   fcfsUeCb = (SchFcfsUeCb *)hqP->hqEnt->ue->schSpcUeCb;
   cmLListAdd2Tail(&(fcfsUeCb->hqRetxCb.dlRetxHqList),&hqP->dlHqProcLink);  
#ifdef NR_DRX
   if(hqP->hqEnt->ue->ueDrxInfoPres == true)
   {
      schDrxStrtDlHqRttTmr(hqP);
   }
   else
#endif
   {
      schFcfsAddUeToSchedule(hqP->hqEnt->cell, hqP->hqEnt->ue->ueId);
   }
}

/*******************************************************************
 *
 * @brief Adds HARQ process to retransmission list
 *
 * @details
 *
 *    Function : schFcfsAddToUlHqRetxList
 *
 *    Functionality: Adds HARQ process to retransmission list
 *
 * @params[in] Pointer to Uplink HARQ Process 
 * @return void
 *
 * ****************************************************************/
void schFcfsAddToUlHqRetxList(SchUlHqProcCb *hqP)
{
   SchFcfsUeCb *fcfsUeCb;

   fcfsUeCb = (SchFcfsUeCb *)hqP->hqEnt->ue->schSpcUeCb;
   cmLListAdd2Tail(&(fcfsUeCb->hqRetxCb.ulRetxHqList),&hqP->ulHqProcLink);  
#ifdef NR_DRX
   if(hqP->hqEnt->ue->ueDrxInfoPres == true)
   {
      schDrxStrtUlHqRttTmr(hqP);
   }
   else
#endif
   {
      schFcfsAddUeToSchedule(hqP->hqEnt->cell, hqP->hqEnt->ue->ueId);
   }   
}

/*******************************************************************
 *
 * @brief Add UE to ueToBeScheduled List
 *
 * @details
 *
 *    Function : schFcfsAddUeToSchedule
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
uint8_t schFcfsAddUeToSchedule(SchCellCb *cellCb, uint16_t ueIdToAdd)
{
   SchFcfsCellCb *fcfsCellCb = NULLP;
   uint8_t *ueId;
   CmLList *node;

   fcfsCellCb = (SchFcfsCellCb *)cellCb->schSpcCell;
   /* Search if UE entry is already present in ueToBeScheduled list.
    * If yes, another entry for same UE not needed. Hence, return */
   node = fcfsCellCb->ueToBeScheduled.first;
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
      DU_LOG("\nERROR  -->  SCH : Memory allocation failure in schFcfsAddUeToSchedule");
      return RFAILED;
   }
   *ueId = ueIdToAdd;
   if(addNodeToLList(&fcfsCellCb->ueToBeScheduled, ueId, NULLP) != ROK)
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
 *    Function : schFcfsProcessCrcInd
 *
 *    Functionality: Processes CRC Indication as required for FCFS
 *       scheduling
 *
 * @params[in] Pointer to Cell control block
 *             UE Id
 * @return void
 *
 * ****************************************************************/
void schFcfsProcessCrcInd(SchCellCb *cellCb, uint16_t ueId)
{
   schFcfsAddUeToSchedule(cellCb, ueId);   
}

/*******************************************************************
 *
 * @brief Processes Buffer Occupancy report from RLC
 *
 * @details
 *
 *    Function : schFcfsDlRlcBoInfo
 *
 *    Functionality: Process buffer occupany report 
 *
 * @params[in]
 * @return
 *
 * ****************************************************************/
void schFcfsDlRlcBoInfo(SchCellCb *cellCb, uint16_t ueId)
{
   schFcfsAddUeToSchedule(cellCb, ueId);   
}

/*******************************************************************
 *
 * @brief Processes BSR request
 *
 * @details
 *
 *    Function : schFcfsBsr
 *
 *    Functionality: Processes BSR as per FCFS scheduling
 *
 * @params[in] Pointer to Cell 
 *             UE ID
 * @return void
 *
 * ****************************************************************/
void schFcfsBsr(SchCellCb *cellCb, uint16_t ueId)
{
   schFcfsAddUeToSchedule(cellCb, ueId);   
}

/*******************************************************************
 *
 * @brief Processed UCI Indication
 *
 * @details
 *
 *    Function : schFcfsSrUciInd
 *
 *    Functionality: Processing of UCI indication specific to 
 *       FCFS scheduling
 *
 * @params[in] Pointer to Cell
 *             UE Id
 * @return void
 *
 * ****************************************************************/
void schFcfsSrUciInd(SchCellCb *cellCb, uint16_t ueId)
{
   schFcfsAddUeToSchedule(cellCb, ueId);   
}

/*******************************************************************
 *
 * @brief Processing of RACH Indication
 *
 * @details
 *
 *    Function : schFcfsProcessRachInd
 *
 *    Functionality: Processing of RACH Indication specific to 
 *       FCFS scheduling
 *
 * @params[in] Pointer to Cell Cb
 *             UE Id
 * @return void
 *
 * ****************************************************************/
void schFcfsProcessRachInd(SchCellCb *cellCb, uint16_t ueId)
{
   schFcfsAddUeToSchedule(cellCb, ueId);   
}

/*******************************************************************
 *
 * @brief Processing of DL HARQ Indication
 *
 * @details
 *
 *    Function : SchFcfsDlHarqInd
 *
 *    Functionality:
 *
 * @params[in] 
 * @return void
 *
 * ****************************************************************/
void SchFcfsDlHarqInd()
{
   return;
}

/*******************************************************************
 *
 * @brief Processing of Paging indication
 *
 * @details
 *
 *    Function : schFcfsPagingInd
 *
 *    Functionality:
 *
 * @params[in]
 * @return void
 *
 * ****************************************************************/
void schFcfsPagingInd()
{
   return;
}

/*******************************************************************
 *
 * @brief Processing of RACH Resource Request
 *
 * @details
 *
 *    Function : schFcfsRachRsrcReq
 *
 *    Functionality:
 *
 * @params[in]
 * @return void
 *
 * ****************************************************************/
void schFcfsRachRsrcReq()
{
   return;
}

/*******************************************************************
 *
 * @brief Processing of RACH Resource Release
 *
 * @details
 *
 *    Function : schFcfsRachRsrcRel
 *
 *    Functionality:
 *
 * @params[in]
 * @return void
 *
 * ****************************************************************/
void schFcfsRachRsrcRel()
{
   return;
}

/*******************************************************************
 *
 * @brief Remove entry from HARQ retransmission list
 *
 * @details
 *
 *    Function : schFcfsRemoveFrmDlHqRetxList
 *
 *    Functionality: Remove entry from HARQ retransmission list
 *
 * @params[in] Pointer to UE
 *             Node to be deleted
 * @return void
 *
 * ****************************************************************/
void schFcfsRemoveFrmDlHqRetxList(SchUeCb *ueCb, CmLList *node)
{
   SchFcfsUeCb *fcfsUeCb;

   fcfsUeCb = (SchFcfsUeCb *)ueCb->schSpcUeCb;
   cmLListDelFrm(&fcfsUeCb->hqRetxCb.dlRetxHqList, node);
}

/*******************************************************************
 *
 * @brief Remove entry from HARQ retransmission list
 *
 * @details
 *
 *    Function : schFcfsRemoveFrmUlHqRetxList
 *
 *    Functionality: Remove entry from HARQ retransmission list
 *
 * @params[in] Pointer to UE
 *             Node to be deleted
 * @return void
 *
 * ****************************************************************/
void schFcfsRemoveFrmUlHqRetxList(SchUeCb *ueCb, CmLList *node)
{
   SchFcfsUeCb *fcfsUeCb;

   fcfsUeCb = (SchFcfsUeCb *)ueCb->schSpcUeCb;
   cmLListDelFrm(&fcfsUeCb->hqRetxCb.ulRetxHqList, node);
}

/*******************************************************************
 *
 * @brief Remove UE from Scheduling List
 *
 * @details
 *
 *    Function : schFcfsRemoveUeFrmScheduleLst
 *
 *    Functionality: Remove UE from Scheduling List
 *
 * @params[in] Pointer to Cell
 *             Node to be removed
 * @return void
 *
 * ****************************************************************/
void schFcfsRemoveUeFrmScheduleLst(SchCellCb *cell, CmLList *node)
{
   SchFcfsCellCb *fcfsCell;

   fcfsCell = (SchFcfsCellCb *)cell->schSpcCell;
   SCH_FREE(node->node, sizeof(uint8_t));
   deleteNodeFromLList(&fcfsCell->ueToBeScheduled, node);
}

/*******************************************************************
 *
 * @brief  Handler to calculate TBS size for BSR requested
 *
 * @details
 *
 *    Function :  schFcfsCalculateUlTbs
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
uint8_t schFcfsCalculateUlTbs(SchUeCb *ueCb, SlotTimingInfo puschTime, uint8_t symbLen,\
                          uint16_t *startPrb, uint32_t *totTBS, bool isRetx, SchUlHqProcCb *hqP, SchFcfsHqProcCb *fcfsHqP)
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
         lcLL = &(fcfsHqP->lcCb.dedLcList);
         rsvdDedicatedPRB = ueCb->ulInfo.ulLcCtxt[lcId].rsvdDedicatedPRB;
      }
      else
      {
         lcLL = &(fcfsHqP->lcCb.defLcList);
      }

      /*[Step2]: Update the reqPRB and Payloadsize for this LC in the appropriate List*/
      if(updateLcListReqPRB(lcLL, lcId, ueCb->bsrInfo[lcgIdx].dataVol) != ROK)
      {
         DU_LOG("\nERROR  --> SCH: LcgId:%d updation failed",lcId);         
         return RFAILED;
      }
   }

   if ((fcfsHqP->lcCb.defLcList.count == 0) && (fcfsHqP->lcCb.dedLcList.count == 0))
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
      if((fcfsHqP->lcCb.dedLcList.count == 0) || ((maxFreePRB < rsvdDedicatedPRB)))
      {
         fcfsHqP->lcCb.sharedNumPrb = maxFreePRB;
         DU_LOG("\nDEBUG  -->  SCH : UL Only Default Slice is scheduled, sharedPRB Count:%d",\
               fcfsHqP->lcCb.sharedNumPrb);

         /*PRB Alloc for Default LCs*/
         prbAllocUsingRRMPolicy(&(fcfsHqP->lcCb.defLcList), FALSE, mcsIdx, symbLen,\
               &(fcfsHqP->lcCb.sharedNumPrb), NULLP, NULLP,&(ueCb->srRcvd));
      }
      else
      {
         fcfsHqP->lcCb.sharedNumPrb = maxFreePRB - rsvdDedicatedPRB;

         /*PRB Alloc for Dedicated LCs*/
         prbAllocUsingRRMPolicy(&(fcfsHqP->lcCb.dedLcList), TRUE, mcsIdx, symbLen,\
               &(fcfsHqP->lcCb.sharedNumPrb), &(rsvdDedicatedPRB),\
               NULLP, &(ueCb->srRcvd));

         /*PRB Alloc for Default LCs*/
         prbAllocUsingRRMPolicy(&(fcfsHqP->lcCb.defLcList), FALSE, mcsIdx, symbLen, \
               &(fcfsHqP->lcCb.sharedNumPrb), &(rsvdDedicatedPRB),\
               NULLP,&(ueCb->srRcvd));
      }
   }
   /*[Step5]:Traverse each LCID in LcList to calculate the exact Scheduled Bytes
    * using allocated BO per LC and Update dlMsgAlloc(BO report for MAC*/ 
   if(fcfsHqP->lcCb.dedLcList.count != 0)
      updateGrantSizeForBoRpt(&(fcfsHqP->lcCb.dedLcList), NULLP, ueCb->bsrInfo, totTBS);

   updateGrantSizeForBoRpt(&(fcfsHqP->lcCb.defLcList), NULLP, ueCb->bsrInfo, totTBS);

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
uint8_t schFcfsScheduleUlLc(SlotTimingInfo dciTime, SlotTimingInfo puschTime, uint8_t startSymb , uint8_t symbLen, bool isRetx, SchUlHqProcCb **hqP)
{
   SchCellCb *cell;
   SchFcfsHqProcCb *fcfsHqProcCb;
   uint8_t ret = RFAILED;
   uint16_t startPrb = 0;
   uint32_t totDataReq = 0; /* in bytes */
   SchUeCb *ueCb;
   SchPuschInfo *puschInfo;
   DciInfo  *dciInfo = NULLP;

   cell = (*hqP)->hqEnt->cell;
   ueCb = (*hqP)->hqEnt->ue;
   fcfsHqProcCb = (SchFcfsHqProcCb *)(*hqP)->schSpcUlHqProcCb;
   ret = schFcfsCalculateUlTbs(ueCb, puschTime, symbLen, &startPrb, &totDataReq, isRetx, *hqP, fcfsHqProcCb);

   if(totDataReq > 0 && ret == ROK)
   {
      SCH_ALLOC(dciInfo, sizeof(DciInfo));
      if(!dciInfo)
      {
         DU_LOG("\nERROR  -->  SCH : Memory Allocation failed for dciInfo alloc");
         if(isRetx != TRUE)
         {
            if(fcfsHqProcCb->lcCb.dedLcList.count != 0)
               updateBsrAndLcList(&(fcfsHqProcCb->lcCb.dedLcList), ueCb->bsrInfo, RFAILED);

            updateBsrAndLcList(&(fcfsHqProcCb->lcCb.defLcList), ueCb->bsrInfo, RFAILED);
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
               if(fcfsHqProcCb->lcCb.dedLcList.count != 0)
                  updateBsrAndLcList(&(fcfsHqProcCb->lcCb.dedLcList), ueCb->bsrInfo, ROK);
               updateBsrAndLcList(&(fcfsHqProcCb->lcCb.defLcList), ueCb->bsrInfo, ROK);
               cmLListAdd2Tail(&(ueCb->hqUlmap[puschTime.slot]->hqList), &(*hqP)->ulSlotLnk);                  
               return ROK;
            }
         }
      }
      if(fcfsHqProcCb->lcCb.dedLcList.count != 0)
         updateBsrAndLcList(&(fcfsHqProcCb->lcCb.dedLcList), ueCb->bsrInfo, RFAILED);
      updateBsrAndLcList(&(fcfsHqProcCb->lcCb.defLcList), ueCb->bsrInfo, RFAILED);
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Grants resources to LC in downlink 
 *
 * @details
 *
 *    Function : schFcfsScheduleDlLc 
 *
 *    Functionality: Grants resources to LC in uplink
 *
 * @params[in] PDCCH Time
 *
 * @return ROK
 *         RFAILED
 *
 * ****************************************************************/
uint32_t schFcfsScheduleDlLc(SlotTimingInfo pdcchTime, SlotTimingInfo pdschTime, uint8_t pdschNumSymbols, \
                               uint16_t *startPrb, bool isRetx, SchDlHqProcCb **hqP)
{
   SchFcfsHqProcCb *fcfsHqProcCb;
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
   fcfsHqProcCb = (SchFcfsHqProcCb *)((*hqP)->schSpcDlHqProcCb);

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
               lcLL = &(fcfsHqProcCb->lcCb.dedLcList);
               rsvdDedicatedPRB = ueCb->dlInfo.dlLcCtxt[lcIdx].rsvdDedicatedPRB;
            }
            else
            {
               lcLL = &(fcfsHqProcCb->lcCb.defLcList);
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

      if ((fcfsHqProcCb->lcCb.defLcList.count == 0) && (fcfsHqProcCb->lcCb.dedLcList.count == 0))
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

         if((fcfsHqProcCb->lcCb.dedLcList.count == NULLP) || ((maxFreePRB < rsvdDedicatedPRB)))
         { 
            fcfsHqProcCb->lcCb.sharedNumPrb = maxFreePRB;
            DU_LOG("\nDEBUG  -->  SCH : DL Only Default Slice is scheduled, sharedPRB Count:%d",\
                  fcfsHqProcCb->lcCb.sharedNumPrb);

            /*PRB Alloc for Default LCs*/
            prbAllocUsingRRMPolicy(&(fcfsHqProcCb->lcCb.defLcList), FALSE, mcsIdx, pdschNumSymbols,\
                  &(fcfsHqProcCb->lcCb.sharedNumPrb), NULLP, &isTxPayloadLenAdded, NULLP);
         }
         else
         {
            fcfsHqProcCb->lcCb.sharedNumPrb = maxFreePRB - rsvdDedicatedPRB;
            /*PRB Alloc for Dedicated LCs*/
            prbAllocUsingRRMPolicy(&(fcfsHqProcCb->lcCb.dedLcList), TRUE, mcsIdx, pdschNumSymbols,\
                  &(fcfsHqProcCb->lcCb.sharedNumPrb), &(rsvdDedicatedPRB), &isTxPayloadLenAdded, NULLP);

            /*PRB Alloc for Default LCs*/
            prbAllocUsingRRMPolicy(&(fcfsHqProcCb->lcCb.defLcList), FALSE, mcsIdx, pdschNumSymbols, \
                  &(fcfsHqProcCb->lcCb.sharedNumPrb), &(rsvdDedicatedPRB), &isTxPayloadLenAdded, NULLP);
         }
      }
   }

   /*[Step5]:Traverse each LCID in LcList to calculate the exact Scheduled Bytes
    * using allocated BO per LC and Update dlMsgAlloc BO report for MAC */
   if (isRetx == FALSE)
   {
      if(fcfsHqProcCb->lcCb.dedLcList.count != 0)
         updateGrantSizeForBoRpt(&(fcfsHqProcCb->lcCb.dedLcList), dciSlotAlloc, NULLP, &(accumalatedSize));

      updateGrantSizeForBoRpt(&(fcfsHqProcCb->lcCb.defLcList), dciSlotAlloc, NULLP, &(accumalatedSize));
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
 *    Function : schFcfsScheduleSlot
 *
 *    Functionality: Scheduling of slots in UL and DL specific to 
 *       FCFS scheduling
 *
 * @params[in] Pointer to Cell
 *             Slot timing info
 *             Scheduler instance
 * @return void
 *
 * ****************************************************************/
void schFcfsScheduleSlot(SchCellCb *cell, SlotTimingInfo *slotInd, Inst schInst)
{
   SchFcfsCellCb  *fcfsCell;
   SchFcfsUeCb    *fcfsUeCb;
   SchDlHqProcCb  *hqP = NULLP;
   SchUlHqProcCb  *ulHqP = NULLP;
   CmLList        *pendingUeNode;
   CmLList        *node;
   uint8_t        ueId;
   bool           isRarPending = false, isRarScheduled = false;
   bool           isMsg4Pending = false, isMsg4Scheduled = false;
   bool           isDlMsgPending = false, isDlMsgScheduled = false;
   bool           isUlGrantPending = false, isUlGrantScheduled = false;
   bool           isNodeFreed = false;

   fcfsCell = (SchFcfsCellCb *)cell->schSpcCell;
   
   /* Select first UE in the linked list to be scheduled next */
   pendingUeNode = fcfsCell->ueToBeScheduled.first;
   while(pendingUeNode)
   {
      if(pendingUeNode->node)
      {
         ueId = *(uint8_t *)(pendingUeNode->node);
         fcfsUeCb = (SchFcfsUeCb *)cell->ueCb[ueId-1].schSpcUeCb;

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
               schFcfsRemoveUeFrmScheduleLst(cell, pendingUeNode);
               isNodeFreed = true;
            }
            /* If RAR/MSG4 is pending but couldnt be scheduled then,
             * put this UE at the end of linked list to be scheduled later */
            else
            {
               cmLListAdd2Tail(&fcfsCell->ueToBeScheduled, cmLListDelFrm(&fcfsCell->ueToBeScheduled, pendingUeNode));
            }
         }

#ifdef NR_DRX 
         if((cell->ueCb[ueId-1].ueDrxInfoPres == true) && (cell->ueCb[ueId-1].drxUeCb.drxDlUeActiveStatus != true))
         {
            if(pendingUeNode->node)
            {
               cmLListAdd2Tail(&fcfsCell->ueToBeScheduled, cmLListDelFrm(&fcfsCell->ueToBeScheduled, pendingUeNode));
            }
         }
         else 
#endif
         {

            /* DL Data */
            node = NULLP;
            if(fcfsUeCb)
               node = fcfsUeCb->hqRetxCb.dlRetxHqList.first;
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
                  schFcfsRemoveFrmDlHqRetxList(&cell->ueCb[ueId-1], node);
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
            if(fcfsUeCb)
               node = fcfsUeCb->hqRetxCb.ulRetxHqList.first;
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
                  schFcfsRemoveFrmUlHqRetxList(&cell->ueCb[ueId-1], node);
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
               cmLListAdd2Tail(&fcfsCell->ueToBeScheduled, cmLListDelFrm(&fcfsCell->ueToBeScheduled, pendingUeNode));
            }
            else
            {
               schFcfsRemoveUeFrmScheduleLst(cell, pendingUeNode);
               isNodeFreed = true;
            }
         }
      }
      if(cell->schDlSlotInfo[slotInd->slot]->prbAlloc.numPrbAlloc >= MAX_NUM_RB)
      {
        DU_LOG("\nINFO   -->  SCH: No PRB available to proceed with next UE");
        return;     
      }
      if(isNodeFreed == false)
      {
         pendingUeNode= pendingUeNode->next;
      }
      else
      {
        pendingUeNode = fcfsCell->ueToBeScheduled.first;
      }
   }
}

/*******************************************************************
 *
 * @brief Initializes all function pointers to FCFS function handler
 *
 * @details
 *
 *    Function : schFcfsAllApisInit
 *
 *    Functionality: Initializes all function pointers to FCFS 
 *       function handler
 *
 * @params[in] Function pointer array
 * @return void
 *
 * ****************************************************************/
void schFcfsAllApisInit(SchAllApis *allFcfsApi)
{
    /* Interface API function pointers */
    allFcfsApi->SchCellCfgReq = schFcfsCellCfgReq;
    allFcfsApi->SchCellDeleteReq = schFcfsCellDelReq;
    allFcfsApi->SchAddUeConfigReq = SchFcfsAddUeConfigReq;
    allFcfsApi->SchModUeConfigReq = SchFcfsModUeConfigReq;
    allFcfsApi->SchUeDeleteReq = SchFcfsUeDeleteReq; 
    allFcfsApi->SchDlHarqInd = SchFcfsDlHarqInd; 
    allFcfsApi->SchCrcInd = schFcfsProcessCrcInd;
    allFcfsApi->SchRachInd = schFcfsProcessRachInd;
    allFcfsApi->SchPagingInd = schFcfsPagingInd;
    allFcfsApi->SchRachRsrcReq = schFcfsRachRsrcReq; 
    allFcfsApi->SchRachRsrcRel = schFcfsRachRsrcRel;
    allFcfsApi->SchDlRlcBoInfo = schFcfsDlRlcBoInfo;
    allFcfsApi->SchSrUciInd = schFcfsSrUciInd;
    allFcfsApi->SchBsr = schFcfsBsr;

    /* Internal API function pointers */
    allFcfsApi->SchAddToDlHqRetxList = schFcfsAddToDlHqRetxList;
    allFcfsApi->SchAddToUlHqRetxList = schFcfsAddToUlHqRetxList;
    allFcfsApi->SchRemoveFrmDlHqRetxList = schFcfsRemoveFrmDlHqRetxList;
    allFcfsApi->SchRemoveFrmUlHqRetxList = schFcfsRemoveFrmUlHqRetxList;
    allFcfsApi->SchAddUeToSchedule = schFcfsAddUeToSchedule;
    allFcfsApi->SchRemoveUeFrmScheduleLst = schFcfsRemoveUeFrmScheduleLst;
    allFcfsApi->SchInitDlHqProcCb = schFcfsInitDlHqProcCb;
    allFcfsApi->SchInitUlHqProcCb = schFcfsInitUlHqProcCb;
    allFcfsApi->SchFreeDlHqProcCb = schFcfsFreeDlHqProcCb;
    allFcfsApi->SchFreeUlHqProcCb = schFcfsFreeUlHqProcCb;
    allFcfsApi->SchDeleteDlHqProcCb = schFcfsDeleteDlHqProcCb;
    allFcfsApi->SchDeleteUlHqProcCb = schFcfsDeleteUlHqProcCb;
    allFcfsApi->SchScheduleSlot = schFcfsScheduleSlot;
    allFcfsApi->SchScheduleDlLc = schFcfsScheduleDlLc;
    allFcfsApi->SchScheduleUlLc = schFcfsScheduleUlLc;
}
/**********************************************************************
    End of file
**********************************************************************/

