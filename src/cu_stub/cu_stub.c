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
#include "common_def.h"
#include "cu_stub.h"
#include "cu_stub_sctp.h"
#include "cu_stub_egtp.h"
#include "du_log.h"

#define CU_ID 1
#define CU_NAME "ORAN_OAM_CU"
#define DU_IP_V4_ADDR "192.168.130.81"
#define CU_IP_V4_ADDR "192.168.130.82"
#define DU_IP_V6_ADDR "0000:0000:0000:0000:0000:0000:0000:0001"
#define CU_IP_V6_ADDR "0000:0000:0000:0000:0000:0000:0000:0011"
#define DU_PORT 38472
#define CU_PORT 38472 
#define DU_EGTP_PORT 39001
#define CU_EGTP_PORT 39002
#define RRC_VER 0
#define EXT_RRC_VER 5
#define PLMN_MCC0 3
#define PLMN_MCC1 1
#define PLMN_MCC2 1
#define PLMN_MNC0 4
#define PLMN_MNC1 8
#define PLMN_MNC2 0

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

void init_log()
{
	openlog("CU_STUB",LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
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

uint8_t tst()
{
   int retVal=0;
   pthread_t conThrdId;
   pthread_attr_t attr;

   init_log();   
   DU_LOG("\nStarting CU_STUB\n");

   /* Start thread to receive console input */
   pthread_attr_init(&attr);
   pthread_attr_setstacksize(&attr, (size_t)NULLD);
   pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   retVal = pthread_create(&conThrdId, &attr, cuConsoleHandler, NULLP);
   if(retVal != 0)
   {
      DU_LOG("\nCU_STUB: Thread creation failed. Cause %d", retVal);
   }
   pthread_attr_destroy(&attr);

   /* Read CU configurations */
   readCuCfg();

   /* Start CU-EGTP */
   egtpActvInit();
   egtpInitReq();

   /* Initializing SCTP global parameters */
   sctpActvInit();
 
   /* Start CU-SCTP to listen on incoming connection */
   sctpCfgReq();
   sctpStartReq();

   return ROK;
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
   uint32_t ipv4_du, ipv4_cu;

   DU_LOG("\nReading CU configurations");

   cmInetAddr((S8*)DU_IP_V4_ADDR, &ipv4_du);
   cmInetAddr((S8*)CU_IP_V4_ADDR, &ipv4_cu);
 
   cuCfgParams.cuId = CU_ID;
   strcpy(cuCfgParams.cuName, CU_NAME);
 
   /* DU IP Address and Port*/
   cuCfgParams.sctpParams.duIpAddr.ipV4Addr = ipv4_du;
   cuCfgParams.sctpParams.duIpAddr.ipV6Pres = false;
   cuCfgParams.sctpParams.duPort = DU_PORT;

   /* CU IP Address and Port*/
   cuCfgParams.sctpParams.cuIpAddr.ipV4Addr = ipv4_cu;
   cuCfgParams.sctpParams.cuIpAddr.ipV6Pres = false;
   cuCfgParams.sctpParams.cuPort = CU_PORT;

   /*PLMN*/
   cuCfgParams.plmn.mcc[0] = PLMN_MCC0;
   cuCfgParams.plmn.mcc[1] = PLMN_MCC1;
   cuCfgParams.plmn.mcc[2] = PLMN_MCC2;
   cuCfgParams.plmn.mnc[0] = PLMN_MNC0;
   cuCfgParams.plmn.mnc[1] = PLMN_MNC1;
   cuCfgParams.plmn.mnc[2] = PLMN_MNC2;

   /*RRC Version*/
   cuCfgParams.rrcVersion.rrcVer = RRC_VER;
   cuCfgParams.rrcVersion.extRrcVer = EXT_RRC_VER;


   /* EGTP Parameters */
   cuCfgParams.egtpParams.localIp.ipV4Pres = TRUE;
   cuCfgParams.egtpParams.localIp.ipV4Addr = ipv4_cu;
   cuCfgParams.egtpParams.localPort = CU_EGTP_PORT;
   cuCfgParams.egtpParams.destIp.ipV4Pres = TRUE;
   cuCfgParams.egtpParams.destIp.ipV4Addr = ipv4_du;
   cuCfgParams.egtpParams.destPort = DU_EGTP_PORT;
   cuCfgParams.egtpParams.minTunnelId = 0;
   cuCfgParams.egtpParams.maxTunnelId = 10;

} /* End of readCuCfg */

/*******************************************************************
 *
 * @brief Handles Console input
 *
 * @details
 *
 *    Function : cuConsoleHandler
 *
 *    Functionality: Handles Console input
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void *cuConsoleHandler(void *args)
{
   char ch;
   while(true) 
   {
      /* Send DL user data to CU when user enters 'd' on console */
      if((ch = getchar()) == 'd')
      {
         /* Start Pumping data from CU to DU */
         DU_LOG("\nEGTP: Sending DL User Data");
         cuEgtpDatReq();      
      } 
   }
}
/**********************************************************************
         End of file
**********************************************************************/
