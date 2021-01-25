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
#include "common_def.h"
#include "du_app_mac_inf.h"
#include "rlc_mac_inf.h"
#include "mac_upr_inf_api.h"

/* Funtion pointer options for cell up indication */
DuMacCellUpInd packMacCellUpIndOpts[] =
{
   packMacCellUpInd,
   duHandleCellUpInd,
   packMacCellUpInd
};
/* Funtion pointer options for stop indication */
DuMacStopInd packMacStopIndOpts[] =
{
   packMacStopInd,
   duHandleStopInd,
   packMacStopInd
};

/* Funtion pointer options for UL CCCH indication */
DuMacUlCcchInd packMacUlCcchIndOpts[] =
{
   packMacUlCcchInd,
   duHandleUlCcchInd,
   packMacUlCcchInd
};

/* Function pointer options for UL Data to RLC */
RlcMacUlDataFunc rlcMacSendUlDataOpts[] =
{
   packRlcUlData,
   RlcProcUlData,
   packRlcUlData
};

/* Funtion pointer options for schedule result reporting */
RlcMacSchedResultRptFunc rlcMacSchedResultRptOpts[] =
{
   packRlcSchedResultRpt,
   RlcProcSchedResultRpt,
   packRlcSchedResultRpt
};
	 

/*******************************************************************
 *
 * @brief Send cell up indication to DU APP
 *
 * @details
 *
 *    Function : MacDuAppCellUpInd
 *
 *    Functionality:
 *       Select appropriate function using selector value and
 *       send cell up indication to DU APP
 *
 * @params[in]  Post structure pointer
 *              Cell UP info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacDuAppCellUpInd(Pst *pst, OduCellId *cellId)
{
   return (*packMacCellUpIndOpts[pst->selector])(pst, cellId);
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
uint8_t MacDuAppStopInd(Pst *pst, OduCellId *cellId)
{
   return (*packMacStopIndOpts[pst->selector])(pst, cellId);
}

/*******************************************************************
 *
 * @brief Send UL CCCH Ind to DU APP
 *
 * @details
 *
 *    Function :MacDuAppUlCcchInd 
 *
 *    Functionality:
 *       Select appropriate function using selector value and
 *       send to DU APP
 *
 * @params[in]  Post structure pointer
 *              UlCcchInd pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacDuAppUlCcchInd(Pst *pst, UlCcchIndInfo *ulCcchIndInfo)
{
   return (*packMacUlCcchIndOpts[pst->selector])(pst, ulCcchIndInfo);
}

/*******************************************************************
 *
 * @brief Send UL data to RLC
 *
 * @details
 *
 *    Function : MacRlcSendUlData
 *
 *    Functionality: Send UL data to RLC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacSendUlDataToRlc(Pst *pst, RlcData *ulData)
{
   return (*rlcMacSendUlDataOpts[pst->selector])(pst, ulData);
}

/*******************************************************************
 *
 * @brief Send Schedule result report to RLC
 *
 * @details
 *
 *    Function : MacSendSchedResultRptToRlc
 *
 *    Functionality: Send Schedule result report to RLC
 *
 * @params[in] Post structure
 *             Schedule result report
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacSendSchedResultRptToRlc(Pst *pst, RlcSchedResultRpt *schedRpt)
{
   return (*rlcMacSchedResultRptOpts[pst->selector])(pst, schedRpt);
}

/**********************************************************************
  End of file
 **********************************************************************/

