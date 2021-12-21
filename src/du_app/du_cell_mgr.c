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

   duCb.actvCellLst[duCb.numActvCells++] = cellCb;

   if(duBuildAndSendMacCellCfg(cellCb->cellId) != ROK)
   {
      DU_LOG("\nERROR  -->  DU APP : macCellCfg build and send failed");
      /* Delete cell from actvCellList */
      duCb.actvCellLst[--(duCb.numActvCells)] = NULLP;
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
   for(cellIdx=0; cellIdx<duCb.numActvCells; cellIdx++)
   {
      if(duCb.actvCellLst[cellIdx]->cellId == cellId)
         *cellCb = duCb.actvCellLst[cellIdx];
	 break;
   }

   if(!*cellCb)
   {
      DU_LOG("\nERROR  -->  DU APP : Cell Id %d not found in DU APP", cellId);
      return RFAILED;
   }

   return ROK;
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

#ifndef O1_ENABLE

   /*Note: Static Configuration, when O1 is not configuring the RRM policy*/
   RrmPolicy *rrmPolicy;
   DU_ALLOC(rrmPolicy, sizeof(RrmPolicy));
   rrmPolicy->rsrcType = RSRC_PRB;
   rrmPolicy->numMemberList = 1;
   DU_ALLOC(rrmPolicy->memberList, sizeof(PolicyMemberList *));
   DU_ALLOC(rrmPolicy->memberList[0], sizeof(PolicyMemberList));
   
   memset(&rrmPolicy->memberList[0]->plmn, 0, sizeof(Plmn)); 
   rrmPolicy->memberList[0]->snssai.sst = 1;
   rrmPolicy->memberList[0]->snssai.sd[0] = 2;
   rrmPolicy->memberList[0]->snssai.sd[1] = 3;
   rrmPolicy->memberList[0]->snssai.sd[2] = 4;
   rrmPolicy->policyMinRatio = 30;
   rrmPolicy->policyMaxRatio = 90;
   rrmPolicy->policyDedicatedRatio = 10;
#endif

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

#ifdef O1_ENABLE
      if(duCfgParam.tempSliceCfg.rrmPolicy)
         BuildAndSendSliceConfigReq(duCfgParam.tempSliceCfg.rrmPolicy, duCfgParam.tempSliceCfg.totalRrmPolicy, duCfgParam.tempSliceCfg.totalSliceCount);
      DU_LOG("\nINFO   -->  DU APP : Raise cell UP alarm for cell id=%d", cellId->cellId);
      raiseCellAlrm(CELL_UP_ALARM_ID, cellId->cellId);
      setCellOpState(cellId->cellId, ENABLED, ACTIVE);
#else
      BuildAndSendSliceConfigReq(&rrmPolicy,1, rrmPolicy->numMemberList);
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
         if(duCb.actvCellLst[cellIdx]->cellId == deleteRsp->cellId)
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

/**********************************************************************
  End of file
 **********************************************************************/
