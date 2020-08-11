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
#include "lrg.h"
#include "lrg.x"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "lwr_mac_upr_inf.h"
#include "mac.h"
#include "mac_upr_inf_api.h"

/*******************************************************************
 *
 * @brief Send stop indication to DU APP
 *
 * @details
 *
 *    Function : sendStopIndMacToDuApp
 *
 *    Functionality:
 *       Send stop indication to DU APP
 *
 * @params[in] Pst info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sendStopIndMacToDuApp(uint16_t cellId)
{
   Pst pst;
   uint8_t ret = ROK;
   MacCellStopInfo *cellStopInd;

   /*  Allocate sharable memory */
   MAC_ALLOC_SHRABL_BUF(cellStopInd, sizeof(MacCellStopInfo));
   if(!cellStopInd)
   {
      DU_LOG("\nMAC : Stop Indication memory allocation failed");
      return RFAILED;
   }
   cellStopInd->cellId = cellId;

   /* Fill Pst */
   FILL_PST_MAC_TO_DUAPP(pst, EVENT_MAC_STOP_IND);

   ret = MacDuAppStopInd(&pst, cellStopInd);
   if(ret != ROK)
   {
      DU_LOG("\nMAC: Failed to send stop indication to DU APP");
      MAC_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, cellStopInd, sizeof(MacCellStopInfo));
   }
   return ROK;
}

/*******************************************************************
 * @brief process Stop indication to MAC
 *
 * @details
 *
 *     Function : fapiMacStopInd 
 *      
 *  @param[in]  Pst            *pst
 *  @return 
 *      -# ROK 
 *      -# RFAILED 
 ******************************************************************/
uint8_t fapiMacStopInd(Pst *pst, uint16_t cellId) 
{
   uint8_t ret = ROK;
   ret = sendStopIndMacToDuApp(cellId);
   return ret;
}

/**********************************************************************
  End of file
 ***********************************************************************/
