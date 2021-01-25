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

uint8_t MacDuAppCellUpInd(Pst *pst, OduCellId *cellId);
uint8_t MacDuAppStopInd(Pst *pst, OduCellId *cellId);
uint8_t MacDuAppUlCcchInd(Pst *pst, UlCcchIndInfo *ulCcchIndInfo);
uint8_t MacSendUlDataToRlc(Pst *pst, RlcData *ulData);
uint8_t MacSendSchedResultRptToRlc(Pst *pst, RlcSchedResultRpt *schedRpt);

/**********************************************************************
         End of file
**********************************************************************/

