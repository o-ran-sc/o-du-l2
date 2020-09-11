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
#include "common_def.h"
#include "du_app_rlc_inf.h"

/* Function pointer array for UL RRC Msg Transfer */
RlcUlRrcMsgToDuFunc rlcSendUlRrcMsgToDuOpts[] = 
{
   packRlcUlRrcMsgToDu,       /* 0 - Loosely coupled */
   DuProcRlcUlRrcMsgTrans,    /* 1 - Tightly coupled */
   packRlcUlRrcMsgToDu        /* 2 - Light weight loosely coupled */
};

/*******************************************************************
 *
 * @brief Sends UL RRC Message Info to DU APP
 *
 * @details
 *
 *    Function : rlcSendUlRrcMsgToDu
 *
 *    Functionality:  Sends UL RRC Message Info to DU APP
 *
 * @params[in] Pst structure
 *             UL RRC Msg Info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t rlcSendUlRrcMsgToDu(Pst *pst, RlcUlRrcMsgInfo *ulRrcMsgInfo)
{
   return (*rlcSendUlRrcMsgToDuOpts[pst->selector])(pst, ulRrcMsgInfo);
}

/**********************************************************************
         End of file
**********************************************************************/
