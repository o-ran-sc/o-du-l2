/*******************************************************************************
 * ################################################################################
 * #   Copyright (c) [2017-2019] [Radisys]
 * #
 * #
 * #
 * #   Licensed under the Apache License, Version 2.0 (the "License");
 * #
 * #   you may not use this file except in compliance with the License.
 * #
 * #   You may obtain a copy of the License at
 * #
 * #
 * #
 * #       http://www.apache.org/licenses/LICENSE-2.0
 * #
 * #
 * #
 * #   Unless required by applicable law or agreed to in writing, software
 * #
 * #   distributed under the License is distributed on an "AS IS" BASIS,
 * #
 * #   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * #
 * #   See the License for the specific language governing permissions and
 * #
 * #   limitations under the License.
 * #
 * ################################################################################
 * *******************************************************************************/

/* This functions contains main() for pnf simulator */
#include "common_def.h"
#include <unistd.h>
#include "pnf_stub_sctp.h"
#include "pnf_stub.h"

PnfGlobalCb pnfCb;

void init_log()
{
	openlog("RIC_STUB",LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
}

/*******************************************************************
 *
 * @brief Read PNF related configuration
 *
 * @details
 *
 *    Function : readPnfCfg
 *
 *    Functionality:
 *            - RRead PNF related configuration
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

void readPnfCfg()
{
   uint8_t  cntNumVnf = 0;
   uint32_t ipv4_vnf = 0, ipv4_pnf = 0;

   DU_LOG("\nDEBUG  -->  PNF_STUB : Reading PNF P5 configurations");

   pnfCb.pnfCfgParams.pnfId = PNF_ID;
   strcpy(pnfCb.pnfCfgParams.pnfName, PNF_NAME);

   /* PNF IP Address and Port*/
   memset(&ipv4_pnf, 0, sizeof(uint32_t));
   cmInetAddr((S8*)LOCAL_IP_PNF, &ipv4_pnf);

   /*PNF SCTP Configuration at PNF P5 Interface*/
   pnfCb.pnfCfgParams.pnfP5SctpParams.pnfP5localIpAddr.ipV4Pres = true;
   pnfCb.pnfCfgParams.pnfP5SctpParams.pnfP5localIpAddr.ipV4Addr = ipv4_pnf;
   pnfCb.pnfCfgParams.pnfP5SctpParams.pnfP5localIpAddr.ipV6Pres = false;
   pnfCb.pnfCfgParams.pnfP5SctpParams.pnfP5SctpPort = PNF_P5_SCTP_PORT;
   
   cntNumVnf = 0;
   while(cntNumVnf < NUM_PNF_P5_ASSOC)
   {
      /* VNF IP Address */
      memset(&ipv4_vnf, 0, sizeof(uint32_t));
      cmInetAddr((S8*)REMOTE_IP_DU[cntNumVnf], &ipv4_vnf);
      
      /* VNF SCTP Parameters */
      pnfCb.pnfCfgParams.pnfP5SctpParams.destCb[cntNumVnf].destIpAddr.ipV4Addr = ipv4_vnf;
      pnfCb.pnfCfgParams.pnfP5SctpParams.destCb[cntNumVnf].destIpAddr.ipV6Pres = false;
      pnfCb.pnfCfgParams.pnfP5SctpParams.destCb[cntNumVnf].destPort = PNF_P5_SCTP_PORT;

           
      (cntNumVnf)++;
   }

   pnfCb.pnfCfgParams.pnfP5SctpParams.numDestNode = cntNumVnf;
  
} /* End of readPnfCfg */


/*******************************************************************
 *
 * @brief Main function of PNF SIMULATOR
 *
 * @details
 *
 *    Function : main
 *
 *    Functionality:
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
#if 0
   int retVal=0;
   pthread_t conThrdId;
   pthread_attr_t attr;
#endif

   init_log();   
   DU_LOG("\nINFO   -->  PNF_STUB : Starting PNF_STUB\n");

   /* TODO: Start thread to receive console input */
#if 0
   pthread_attr_init(&attr);
   pthread_attr_setstacksize(&attr, (size_t)NULLD);
   pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   retVal = pthread_create(&conThrdId, &attr, vnfConsoleHandler, NULLP);
   if(retVal != 0)
   {
      DU_LOG("\nERROR  -->  PNF_STUB :  Thread creation failed. Cause %d", retVal);
   }
   pthread_attr_destroy(&attr);
#endif
   /* Read PNF configurations */
   readPnfCfg();

   /* Initializing SCTP global parameters */
   pnfP5SctpActvInit();
 
   /* Start PNF-P5-SCTP to listen on incoming connection */
   pnfP5SctpCfgReq();
   
   /*Sleep is introduced for GDB to increase the waiting time for PNF Configuration from VNF*/
   sleep(1);
   
   pnfP5SctpStartReq();

   return ROK;
}



