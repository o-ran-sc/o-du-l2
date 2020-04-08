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

#include "mac_main.h"
#include "fapi_interface.h"
#include "du_app_mac_inf.h"

/* This file contains message handling functionality for MAC */

/*******************************************************************
 *
 * @brief Handles cell start reuqest from DU APP
 *
 * @details
 *
 *    Function : MacHdlCellStartReq
 *
 *    Functionality:
 *      Handles cell start reuqest from DU APP
 *
 * @params[in] Post structure pointer
 *             Cell Start Request info pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t MacHdlCellStartReq(Pst *pst, MacCellStartInfo  *cellStartInfo)
{
   DU_LOG("\nMAC : Handling cell start request");
   //sendToLowerMac(START_REQ, 0, cellStartInfo);

   SPutSBuf(pst->region, pst->pool, (Data *)cellStartInfo,\
      sizeof(MacCellStartInfo));
   cellStartInfo = NULLP;

   return ROK;
}

/*******************************************************************
 *
 * @brief Handles cell stop request from DU APP
 *
 * @details
 *
 *    Function : MacHdlCellStartReq
 *
 *    Functionality:
 *        Handles cell stop request from DU APP
 *
 * @params[in] Post structure pointer
 *             Mac Cell stop information
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t MacHdlCellStopReq(Pst *pst, MacCellStopInfo  *cellStopInfo)
{
   DU_LOG("\nMAC : Handling cell start request");
   //sendToLowerMac(STOP_REQ, 0, cellStopInfo);
 
   SPutSBuf(pst->region, pst->pool, (Data *)cellStopInfo,\
      sizeof(MacCellStopInfo));
   cellStopInfo = NULLP;
 
   return ROK;
}

/**********************************************************************
         End of file
**********************************************************************/

