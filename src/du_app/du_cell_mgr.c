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
#include "lrg.h"
#include "legtp.h"
#include "lrg.x"
#include "lkw.x"
#include "rgr.h"
#include "rgr.x"
#include "du_app_mac_inf.h"
#include "du_app_rlc_inf.h"
#include "du_cfg.h"
#include "du_mgr.h"
#include "du_utils.h"
#include "du_cell_mgr.h"
#include "PCCH-Config.h"

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
   DuCellCb *duCellCb;

   if(slotIndInfo)
   {
      GET_CELL_IDX(slotIndInfo->cellId, cellIdx);
      duCellCb = duCb.actvCellLst[cellIdx];

      if(duCellCb)
      {
         duCellCb->currSlotInfo.sfn = slotIndInfo->sfn;
         duCellCb->currSlotInfo.slot = slotIndInfo->slot;
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

      if(duCfgParam.tempSliceCfg.rrmPolicy)
         BuildAndSendSliceConfigReq(duCfgParam.tempSliceCfg.rrmPolicy, duCfgParam.tempSliceCfg.totalRrmPolicy, duCfgParam.tempSliceCfg.totalSliceCount);
#ifdef O1_ENABLE
      DU_LOG("\nINFO   -->  DU APP : Raise cell UP alarm for cell id=%d", cellId->cellId);
      raiseCellAlrm(CELL_UP_ALARM_ID, cellId->cellId);
      setCellOpState(cellId->cellId, ENABLED, ACTIVE);
#endif

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
   uint16_t cellIdx=0;
   
   if(deleteRsp)
   {
      if(deleteRsp->result == SUCCESSFUL_RSP)
      {
         GET_CELL_IDX(deleteRsp->cellId, cellIdx);
         DU_LOG("\nINFO   -->  DU APP : MAC CELL Delete Response : SUCCESS [CELL IDX : %d]", deleteRsp->cellId);
         if(duCb.actvCellLst[cellIdx] && (duCb.actvCellLst[cellIdx]->cellId == deleteRsp->cellId))
         {
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
   MacCellDelete *cellDelete = NULLP;
   
   DU_ALLOC_SHRABL_BUF(cellDelete, sizeof(MacCellDelete));
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
         DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, cellDelete, sizeof(MacCellDelete));
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
 * @brief fill paging information of a UE belongs to a particular cell
 *
 * @details
 *
 *    Function : FillPagingInfoInCellCb
 *
 *    Functionality: fill paging information of a UE in DuCellCb
 *
 * @params[in] DuCellCb* cellCb, uint8_t ueId, 
 *            PagingMsg  rcvdF1apPagingParam
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t FillPagingInfoInCellCb(DuCellCb* cellCb, PagingMsg *rcvdF1apPagingParam)
{
   uint16_t T=0, N=0, ns =0, pagingFrame = 0, n = 0;
   uint16_t currentSfn = 0, sfn = 0, newSfn = 0;
   PcchCfg   duPcchCfg;

   DU_LOG("\nINFO  --> DU APP : Start filling paging parameters in DuCellCb");
   if(cellCb)
   {
      /* calculate paging frame and paging offset */
      duPcchCfg = duCfgParam.sib1Params.srvCellCfgCommSib.dlCfg.pcchCfg;
      rcvdF1apPagingParam->pagingFrameOffset =duPcchCfg.pageFrameOffset;
      ns = duPcchCfg.ns;      

      /*
       * Fill the Value of T (DRX cycle of the UE)
       * T = min(UE Specific DRX value allocated by upper layers, default DRX
       * broadcast in System Information) */
      if((rcvdF1apPagingParam->pagingDrxPres) && (duPcchCfg.dfltPagingCycle > rcvdF1apPagingParam->pagingDrx))
      {
         GET_PAGING_CYCLE(rcvdF1apPagingParam->pagingDrx, T);
      } 
      else
      {
         GET_PAGING_CYCLE(duPcchCfg.dfltPagingCycle, T);
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
      pagingFrame = (T / N) * ((rcvdF1apPagingParam->ueId) % N);

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
      rcvdF1apPagingParam->pagingFrame =  (sfn % 1024);
      rcvdF1apPagingParam->i_s = ((uint32_t)(floor(rcvdF1apPagingParam->ueId / N)) % ns);

      DU_LOG("\nINFO  --> DU APP : Successfully filled paging parameter in DuCellCb");
      memcpy(&cellCb->tmpPagingInfoOfUe, rcvdF1apPagingParam, sizeof(PagingMsg));
   }
   else
   {
      DU_LOG("\nINFO  --> DU APP : FillPagingInfoInCellCb(): Received null pointer");
      return RFAILED;
   }
   return ROK;
}


/**********************************************************************
  End of file
 **********************************************************************/
