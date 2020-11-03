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
//#include "E2AP-PDU.h"
//#include<ProtocolIE-Field.h>
//#include "F1AP-PDU.h"
//#include "du_e2ap_msg_hdl.h"
#include "du_app_mac_inf.h"
#include "du_app_rlc_inf.h"
#include "du_cfg.h"
#include "du_mgr.h"
//#include "odu_common_codec.h"
#include "du_cell_mgr.h"
#include "du_f1ap_msg_hdl.h"


/*******************************************************************
 *
 * @brief Processes cells to be activated
 *
 * @details
 *
 *    Function : procCellsToBeActivated
 *
 *    Functionality:
 *      - Processes cells to be activated list received in F1SetupRsp
 *
 * @params[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procCellsToBeActivated(uint16_t nci, uint16_t nRPci)
{
   uint8_t ret = ROK;
   DuCellCb *cellCb = NULLP;

   cellCb = duCb.cfgCellLst[nci-1];

   if(!cellCb)
   {
      DU_LOG("\nDU APP : No Cell found for NCI %d", nci);
      return RFAILED;
   }

   cellCb->cellStatus = ACTIVATION_IN_PROGRESS; 
   cellCb->cellInfo.nrPci = nRPci;

   /* Now remove this cell from configured list and move to active list */
   duCb.cfgCellLst[nci-1] = NULL;
   duCb.actvCellLst[nci-1] = cellCb;
   duCb.numActvCells++;
   /* Build and send Mac Cell Cfg for the number of active cells */
   ret = duBuildAndSendMacCellCfg();
   if(ret != ROK)
   {
      DU_LOG("\nDU APP : macCellCfg build and send failed");
      return RFAILED;
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
void duProcF1SetupRsp(F1SetupRsp *f1SetupRspDb)
{
   if(f1SetupRspDb)
   {
      DU_LOG("\nF1AP : F1 Setup Response received");
      duCb.f1Status = TRUE; //Set F1 status as true
   }
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
void duProcGnbDuCfgUpdAckMsg(F1GnbDuCfgUpdAck *gnbDuAckDb)
{
   if(gnbDuAckDb)
   {
      DU_LOG("\nDU APP: GNB-DU config update Ack received for transId %d", gnbDuAckDb->transId);
   }
}

/**********************************************************************
  End of file
 **********************************************************************/
