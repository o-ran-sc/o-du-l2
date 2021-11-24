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

/* This functions contains main() for ric_app */
#include "common_def.h"
#include "ric_stub.h"
#include "ric_stub_sctp.h"
#include "du_log.h"

#ifdef O1_ENABLE
#include "CmInterface.h"
#endif

#define RIC_ID 1
#define RIC_NAME "ORAN_OAM_RIC"

#define DU_IP_V6_ADDR "0000:0000:0000:0000:0000:0000:0000:0001"
#define RIC_IP_V6_ADDR "0000:0000:0000:0000:0000:0000:0000:0011"

#ifndef O1_ENABLE

#define DU_IP_V4_ADDR "192.168.130.81"
#define RIC_IP_V4_ADDR "192.168.130.80"
#define DU_PORT 36421
#define RIC_PORT 36421

#endif

#define RRC_VER 0
#define EXT_RRC_VER 5
#define PLMN_MCC0 3
#define PLMN_MCC1 1
#define PLMN_MCC2 1
#define PLMN_MNC0 4
#define PLMN_MNC1 8
#define PLMN_MNC2 0

#ifdef O1_ENABLE
extern StartupConfig g_cfg;
#endif

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
	openlog("RIC_STUB",LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
}
/*******************************************************************
 *
 * @brief Main function of E2 APP
 *
 * @details
 *
 *    Function : main
 *
 *    Functionality:
 *         - Reads RIC related configurations
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
   init_log();   
 
   DU_LOG("\nINFO  --> RIC : Starting RIC_STUB");
   /* Read RIC configurations */
   readRicCfg();

   /* Start RIC-SCTP to listen on incoming connection */
   sctpCfgReq();
   sctpStartReq();

   return ROK;
}
/*******************************************************************
 *
 * @brief Read RIC related configuration
 *
 * @details
 *
 *    Function : readRicCfg
 *
 *    Functionality:
 *            - Read RIC related configuration
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

void readRicCfg()
{
   uint32_t ipv4_du, ipv4_ric;

   DU_LOG("\nINFO  --> RIC : Reading RIC configurations");

#ifdef O1_ENABLE
   if( getStartupConfigForStub(&g_cfg) != ROK )
   {
      DU_LOG("\nError  -->  RIC : Could not fetch startup "\
             "configurations from Netconf interface\n");
      exit(1);
   }

   cmInetAddr((S8*)g_cfg.DU_IPV4_Addr,  &ipv4_du);
   cmInetAddr((S8*)g_cfg.RIC_IPV4_Addr, &ipv4_ric);

   ricCfgParams.sctpParams.duPort = g_cfg.RIC_Port;
   ricCfgParams.sctpParams.ricPort = g_cfg.RIC_Port;
#else
   cmInetAddr((S8*)DU_IP_V4_ADDR, &ipv4_du);
   cmInetAddr((S8*)RIC_IP_V4_ADDR, &ipv4_ric);

   ricCfgParams.sctpParams.duPort = DU_PORT;
   ricCfgParams.sctpParams.ricPort = RIC_PORT;
#endif
    
   ricCfgParams.ricId = RIC_ID;
   strcpy(ricCfgParams.ricName, RIC_NAME);
 
   /* DU IP Address and Port*/
   ricCfgParams.sctpParams.duIpAddr.ipV4Addr = ipv4_du;
   ricCfgParams.sctpParams.duIpAddr.ipV6Pres = false;

   /* RIC IP Address and Port*/
   ricCfgParams.sctpParams.ricIpAddr.ipV4Addr = ipv4_ric;
   ricCfgParams.sctpParams.ricIpAddr.ipV6Pres = false;

   /*PLMN*/
   ricCfgParams.plmn.mcc[0] = PLMN_MCC0;
   ricCfgParams.plmn.mcc[1] = PLMN_MCC1;
   ricCfgParams.plmn.mcc[2] = PLMN_MCC2;
   ricCfgParams.plmn.mnc[0] = PLMN_MNC0;
   ricCfgParams.plmn.mnc[1] = PLMN_MNC1;
   ricCfgParams.plmn.mnc[2] = PLMN_MNC2;
  
} /* End of readCuCfg */
/**********************************************************************
         End of file
**********************************************************************/
