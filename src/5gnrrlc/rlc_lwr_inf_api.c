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
/* This file defines API towards lower interface of RLC */

#include "common_def.h"
#include "rlc_mac_inf.h" 

RlcMacBoStatusFunc rlcMacSendBoStatusOpts[] =
{
   packRlcBoStatus,            /* 0 - loosely coupled */
   MacProcRlcBoStatus,      /* 1 - tightly coupled, MAC  */
   packRlcBoStatus,            /* 2 - LWLC loosely coupled */
};
RlcMacDlDataFunc rlcMacSendDlDataOpts[] =
{
   packRlcDlData,            /* 0 - loosely coupled */
   MacProcRlcDlData,      /* 1 - tightly coupled, MAC  */
   packRlcDlData,            /* 2 - loosely coupled */
};
/**
 *
 * @brief  
 *
 *        Handler for reporting the Buffer Occupancy to MAC 
 *        for logical channels.
 *
 * @b Description:
 *
 *        This function reports the Buffer Occupancy of one or more
 *         logical channels to MAC. 
 *
 *  @param[in] post         Post structure  
 *  @param[in] spId        Service Provider ID
 *  @param[in] boSta       BO Status Information 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
uint8_t RlcSendBoStatusToMac(Pst *pst, RlcBoStatus *boSta)
{
   /* jump to specific primitive depending on configured selector */
   return (*rlcMacSendBoStatusOpts[pst->selector])(pst, boSta);
 
} /* end of RlcMacSendBOStatus */
 
/*******************************************************************
 *
 * @brief Sends DL data to MAC
 *
 * @details
 *
 *    Function : RlcSendDlDataToMac
 *
 *    Functionality: Sends DL data to MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t RlcSendDlDataToMac(Pst *pst, RlcData *dlData)
{
   /* jump to specific primitive depending on configured selector */
   return (*rlcMacSendDlDataOpts[pst->selector])(pst, dlData);
 
}
/**********************************************************************
         End of file
**********************************************************************/
