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

/* This functions contains main() for cu_app */

#include "cu_stub.h"
#include "cu_stub_sctp.h"

#define CU_ID 1
#define CU_NAME "ORAN_OAM_CU"
#define DU_IP_V4_ADDR "10.0.2.20"
#define CU_IP_V4_ADDR "10.0.2.25"
#define DU_IP_V6_ADDR "0000:0000:0000:0000:0000:0000:0000:0001"
#define CU_IP_V6_ADDR "0000:0000:0000:0000:0000:0000:0000:0011"
#define DU_PORT 38472
#define CU_PORT 38472 

/*******************************************************************
 *
 * @brief Handles SCTP notification 
 *
 * @details
 *
 *    Function : sctpNtfyInd
 *
 *    Functionality:
 *         Handles SCTP notification
 *
 * @params[in] sctp notification
 * @return void
 *
 ******************************************************************/
void sctpNtfyInd(CmInetSctpNotification *ntfy)
{
//TODO
}

/*******************************************************************
 *
 * @brief Main function of CU APP
 *
 * @details
 *
 *    Function : main
 *
 *    Functionality:
 *         - Reads CU related configurations
 *         - Initialize SCTP Parameters
 *         - Start SCTP receiver thread
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 tst()
{
   printf("\nStarting cu_app");
   
   /* Read CU configurations */
   readCuCfg();

   /* Initializing SCTP global parameters */
   sctpActvInit(0, 0, 0, 0);
 
   /* Start CU-SCTP to listen on incoming connection */
   sctpStartReq(); 

   RETVALUE(ROK);
}

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
   U32 ipv4_du, ipv4_cu;

   printf("\nReading CU configurations");

   cmInetAddr((S8*)DU_IP_V4_ADDR, &ipv4_du);
   cmInetAddr((S8*)CU_IP_V4_ADDR, &ipv4_cu);
   //U32 ipv6_int = inet_addr(DU_IP_V6_ADDR);
 
   cuCfgParams.cuId = CU_ID;
   strcpy(cuCfgParams.cuName, CU_NAME);
 
   /* DU IP Address and Port*/
   cuCfgParams.sctpParams.duIpAddr.ipV4Pres = true;
   cuCfgParams.sctpParams.duIpAddr.ipV4Addr = ipv4_du;
   cuCfgParams.sctpParams.duIpAddr.ipV6Pres = false;
   //strcpy(cuCfgParams.sctpParams.duIpAddr.ipV6Addr, (char*)DU_IP_V6_ADDR);
   cuCfgParams.sctpParams.duPort = DU_PORT;

   /* CU IP Address and Port*/
   cuCfgParams.sctpParams.cuIpAddr.ipV4Pres = true;
   cuCfgParams.sctpParams.cuIpAddr.ipV4Addr = ipv4_cu;
   cuCfgParams.sctpParams.cuIpAddr.ipV6Pres = false;
   //strcpy(cuCfgParams.sctpParams.cuIpAddr.ipV6Addr, DU_IP_V6_ADDR);
   cuCfgParams.sctpParams.cuPort = CU_PORT;
   
} /* End of readCuCfg */
/**********************************************************************
         End of file
**********************************************************************/
