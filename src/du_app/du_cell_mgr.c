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
#include "du_cell_mgr.h"

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
      buildPlmnId(duCb.cfgCellLst[cfgIdx]->cellInfo.nrEcgi.plmn, tmpPlmn);
      if(duCb.cfgCellLst[cfgIdx]->cellInfo.nrEcgi.cellId == nci &&
	    (strcmp(tmpPlmn, plmnStr) == 0))
      {
	 cellCb = duCb.cfgCellLst[cfgIdx];
	 break;
      }
      else
      {
	 DU_LOG("\nDU APP : No Cell found for NCI %d", nci);
	 return RFAILED;
      }
   }

   cellCb->cellStatus = ACTIVATION_IN_PROGRESS; 
   cellCb->cellInfo.nrPci = nRPci;

   duCb.actvCellLst[duCb.numActvCells] = cellCb;
   duCb.numActvCells++;

   if(duBuildAndSendMacCellCfg(cellCb->cellId) != ROK)
   {
      DU_LOG("\nDU APP : macCellCfg build and send failed");
      /* Move cellCb back to cfgCellList */
      duCb.cfgCellLst[nci-1] = duCb.actvCellLst[nci-1];
      duCb.actvCellLst[nci-1] = NULLP;
      duCb.numActvCells--;
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
   DU_LOG("\nDU_APP : F1 Setup Response received");
   duCb.f1Status = TRUE; //Set F1 status as true
}

/*******************************************************************
 *
 * @brief Handles GNB DU Cfg Update Ack received in F1AP
 *
 * @details
 *
 *    Function : duProcGnbDuCfgUpdAckMsg
 *
 *    Functionality:
 *      - Handles GNB DU Cfg Update Ack received in F1AP
 *
 * @params[in] Pointer to F1GnbDuCfgUpdAck
 * @return void
 *  
 ******************************************************************/
void duProcGnbDuCfgUpdAckMsg()
{
   DU_LOG("\nDU APP: GNB-DU config update Ack received ");
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
   uint8_t idx;
   for(idx=0; idx<duCb.numActvCells; idx++)
   {
      if(duCb.actvCellLst[idx]->cellId == cellId)
         *cellCb = duCb.actvCellLst[idx];
   }

   if(!*cellCb)
   {
      DU_LOG("\nDU APP : Cell Id %d not found in DU APP", cellId);
      return RFAILED;
   }

   return ROK;
}

/**********************************************************************
  End of file
 **********************************************************************/
