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
#include "ric_stub_sctp.h"
#include "ric_stub.h"

#ifdef O1_ENABLE
#include "CmInterface.h"
#endif

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
   /*Sleep is introduced for GDB to increase the waiting time for RIC Configuration from DUAPP*/
   sleep(1);
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
   uint8_t *numDu;
   uint32_t ipv4_du, ipv4_ric;

   DU_LOG("\nINFO  --> RIC : Reading RIC configurations");

   ricCfgParams.ricId = RIC_ID;
   strcpy(ricCfgParams.ricName, RIC_NAME);
#ifdef O1_ENABLE
   if( getStartupConfigForStub(&g_cfg) != ROK )
   {
      DU_LOG("\nError  -->  RIC : Could not fetch startup "\
             "configurations from Netconf interface\n");
      exit(1);
   }

   cmInetAddr((S8*)g_cfg.DU_IPV4_Addr,  &ipv4_du);
   cmInetAddr((S8*)g_cfg.RIC_IPV4_Addr, &ipv4_ric);
   
   ricCfgParams.sctpParams.sctpAssoc[0].duIpAddr.ipV4Addr = ipv4_du;
   ricCfgParams.sctpParams.sctpAssoc[0].duIpAddr.ipV6Pres = false;
   ricCfgParams.sctpParams.sctpAssoc[0].duPort = g_cfg.RIC_Port;

   ricCfgParams.sctpParams.sctpAssoc[0].ricIpAddr.ipV4Addr = ipv4_ric;
   ricCfgParams.sctpParams.sctpAssoc[0].ricIpAddr.ipV6Pres = false;
   ricCfgParams.sctpParams.sctpAssoc[0].ricPort = g_cfg.RIC_Port;

   ricCfgParams.sctpParams.numDu = 1;

#else
   ricCfgParams.sctpParams.numDu = 0;
   numDu = &ricCfgParams.sctpParams.numDu;
   while(*numDu < MAX_DU_SUPPORTED)
   {   
      /* DU IP Address and Port*/
      memset(&ipv4_du, 0, sizeof(uint32_t));
      cmInetAddr((S8*)DU_IP_V4_ADDR[*numDu], &ipv4_du);
      ricCfgParams.sctpParams.sctpAssoc[*numDu].duIpAddr.ipV4Addr = ipv4_du;
      ricCfgParams.sctpParams.sctpAssoc[*numDu].duIpAddr.ipV6Pres = false;
      ricCfgParams.sctpParams.sctpAssoc[*numDu].duPort = DU_SCTP_PORT[*numDu];

      /* RIC IP Address and Port*/
      memset(&ipv4_du, 0, sizeof(uint32_t));
      cmInetAddr((S8*)RIC_IP_V4_ADDR, &ipv4_ric);
      ricCfgParams.sctpParams.sctpAssoc[*numDu].ricIpAddr.ipV4Addr = ipv4_ric;
      ricCfgParams.sctpParams.sctpAssoc[*numDu].ricIpAddr.ipV6Pres = false;
      ricCfgParams.sctpParams.sctpAssoc[*numDu].ricPort = RIC_SCTP_PORT_TO_DU[*numDu];
      (*numDu)++;
   }   
#endif

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
