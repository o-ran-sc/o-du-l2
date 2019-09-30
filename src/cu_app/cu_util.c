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

/* This file contains configurations for CU */

#include "cu_mgr_main.h"

#define CU_ID 1
#define CU_NAME "Oran_OAM_CU"
#define DU_IP_V4_ADDR "127.0.0.1"
#define CU_IP_V4_ADDR "127.0.0.2"
#define DU_IP_V6_ADDR "0000:0000:0000:0000:0000:0000:0000:0001"
#define CU_IP_V6_ADDR "0000:0000:0000:0000:0000:0000:0000:0011"
#define DU_PORT 38472
#define CU_PORT 38472 

/*******************************************************************
 *
 * @brief Read CU related configuration
 *
 * @details
 *
 *    Function : readCuCfg
 *
 *    Functionality:
 *            - RRead CU related configuration
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

void readCuCfg()
{
   printf("\nReading CU configurations");
   
   cuCfgParams.cuId = CU_ID;
   strcpy(cuCfgParams.cuName, CU_NAME);
 
   /* DU IP Address and Port*/
   cuCfgParams.sctpParams.duIpAddr.ipV4Pres = true;
   strcpy(cuCfgParams.sctpParams.duIpAddr.ipV4Addr, (char*)DU_IP_V4_ADDR);
   cuCfgParams.sctpParams.duIpAddr.ipV6Pres = false;
   strcpy(cuCfgParams.sctpParams.duIpAddr.ipV6Addr, (char*)DU_IP_V6_ADDR);
   cuCfgParams.sctpParams.duPort = DU_PORT;

   /* CU IP Address and Port*/
   cuCfgParams.sctpParams.cuIpAddr.ipV4Pres = true;
   strcpy(cuCfgParams.sctpParams.cuIpAddr.ipV4Addr, (char*)CU_IP_V4_ADDR);
   printf("\nCU IP Address %s",cuCfgParams.sctpParams.cuIpAddr.ipV4Addr);
   cuCfgParams.sctpParams.cuIpAddr.ipV6Pres = false;
   printf("\nCU IP Address %s",cuCfgParams.sctpParams.cuIpAddr.ipV4Addr);
   strcpy(cuCfgParams.sctpParams.cuIpAddr.ipV6Addr, DU_IP_V6_ADDR);
   cuCfgParams.sctpParams.cuPort = CU_PORT;
   
} /* End of readCuCfg */

/**********************************************************************
         End of file
**********************************************************************/
