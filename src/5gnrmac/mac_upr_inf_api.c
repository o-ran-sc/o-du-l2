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

/* This file contains the definitions for Upper Interface APIs that are
 * invoked from MAC */

#include "mac_upr_inf_api.h"

/* Funtion pointer options for slot indication */
DuMacSlotInd packMacSlotIndOpts[] =
{
   packMacSlotInd,
   duHandleSlotInd,
   packMacSlotInd
};
/* Funtion pointer options for stop indication */
DuMacStopInd packMacStopIndOpts[] =
{
   packMacStopInd,
   duHandleStopInd,
   packMacStopInd
};

/*******************************************************************
 *
 * @brief Send slot indication to MAC
 *
 * @details
 *
 *    Function : MacDuAppSlotInd
 *
 *    Functionality:
 *       Select appropriate function using selector value and
 *       send to MAC
 *
 * @params[in]  Post structure pointer
 *              Slot info pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t MacDuAppSlotInd(Pst *pst, SlotInfo *slotInfo)
{
   return (*packMacSlotIndOpts[pst->selector])(pst, slotInfo);
}

/*******************************************************************
 *
 * @brief Send stop indication to MAC
 *
 * @details
 *
 *    Function : MacDuAppStopInd
 *
 *    Functionality:
 *       Sends Stop Indication to MAC
 *
 * @params[in]  Post structure pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t MacDuAppStopInd(Pst *pst, MacCellStopInfo *cellStopId)
{
   return (*packMacStopIndOpts[pst->selector])(pst, cellStopId);
}

/**********************************************************************
         End of file
**********************************************************************/

