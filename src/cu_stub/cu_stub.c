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

#ifdef O1_ENABLE
#include "CmInterface.h"
#endif

#define CU_ID 1
#define CU_NAME "ORAN_OAM_CU"

#define DU_IP_V6_ADDR "0000:0000:0000:0000:0000:0000:0000:0001"
#define CU_IP_V6_ADDR "0000:0000:0000:0000:0000:0000:0000:0011"

#ifndef O1_ENABLE

#define DU_IP_V4_ADDR "192.168.130.81"
#define CU_IP_V4_ADDR "192.168.130.82"
#define DU_PORT 38472
#define CU_PORT 38472 

#endif

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
   DU_LOG("\nINFO   -->  CU_STUB : Starting CU_STUB\n");

   /* Start thread to receive console input */
   pthread_attr_init(&attr);
   pthread_attr_setstacksize(&attr, (size_t)NULLD);
   pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   retVal = pthread_create(&conThrdId, &attr, cuConsoleHandler, NULLP);
   if(retVal != 0)
   {
      DU_LOG("\nERROR  -->  CU_STUB :  Thread creation failed. Cause %d", retVal);
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

   DU_LOG("\nDEBUG  -->  CU_STUB : Reading CU configurations");

#ifdef O1_ENABLE
   if( getStartupConfigForStub(&g_cfg) != ROK )
   {
      DU_LOG("\nError  -->  CU_STUB : Could not fetch startup "\
             "configurations from Netconf interface\n");
      exit(1);
   }
   
   cmInetAddr((S8*)g_cfg.DU_IPV4_Addr, &ipv4_du);
   cmInetAddr((S8*)g_cfg.CU_IPV4_Addr, &ipv4_cu);

   cuCfgParams.sctpParams.duPort = g_cfg.DU_Port;
   cuCfgParams.sctpParams.cuPort = g_cfg.CU_Port;
#else
   cmInetAddr((S8*)DU_IP_V4_ADDR, &ipv4_du);
   cmInetAddr((S8*)CU_IP_V4_ADDR, &ipv4_cu);
   cuCfgParams.sctpParams.duPort = DU_PORT;
   cuCfgParams.sctpParams.cuPort = CU_PORT;
#endif
    
   cuCfgParams.cuId = CU_ID;
   strcpy(cuCfgParams.cuName, CU_NAME);
 
   /* DU IP Address and Port*/
   cuCfgParams.sctpParams.duIpAddr.ipV4Addr = ipv4_du;
   cuCfgParams.sctpParams.duIpAddr.ipV6Pres = false;

   /* CU IP Address and Port*/
   cuCfgParams.sctpParams.cuIpAddr.ipV4Addr = ipv4_cu;
   cuCfgParams.sctpParams.cuIpAddr.ipV6Pres = false;

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
   cuCfgParams.egtpParams.minTunnelId = MIN_TEID;
   cuCfgParams.egtpParams.currTunnelId = cuCfgParams.egtpParams.minTunnelId;
   cuCfgParams.egtpParams.maxTunnelId = MAX_TEID;

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
   uint8_t teId = 0;
   uint8_t ret = ROK;
   uint8_t cnt = 0;

   /* This variable is taken for sending specific number of downlink data packet. 
    * Presently the code is supporting total 4500 data packets trasfer for 3 UEs only with sleep(1).
    * If you wants to pump data for 3 UE change the following macro values
    * NUM_TUNNEL_TO_PUMP_DATA = 9, NUM_DL_PACKETS = 1.
    * totalDataPacket = totalNumOfTestFlow * NUM_TUNNEL_TO_PUMP_DATA * NUM_DL_PACKETS 
    * totalDataPacket = [500*9*1] */
   int32_t totalNumOfTestFlow = 500; 

   while(true) 
   {
      /* Send DL user data to CU when user enters 'd' on console */
      if((ch = getchar()) == 'd')
      {

      /* Change #if 0 to #if 1 to take input from user */
#if 0
         DU_LOG("\n EGTP --> : Enter TEID id(1..10) where DL Data to be sent\n");
         scanf("%d",&teId);
         
         if(teId > MAX_TEID || teId < MIN_TEID)
         {
            DU_LOG("\nERROR  -->  EGTP : TEID(%x) OUT Of Range",teId);
            printf("\n");
            continue;
         }
         /* Start Pumping data from CU to DU */
         DU_LOG("\nDEBUG  -->  EGTP: Sending DL User Data(teId:%d)\n",teId);

         cnt =0;
         while(cnt < NUM_DL_PACKETS)
         {
            ret =  cuEgtpDatReq(teId);
            if(ret != ROK)
            {
               DU_LOG("\nERROR --> EGTP: Issue with teid=%d\n",teId);
               break;
            }
            cnt++;
         }
#else
         while(totalNumOfTestFlow)
         {
            for(teId = 1; teId <= NUM_TUNNEL_TO_PUMP_DATA; teId++)
            {
               DU_LOG("\nDEBUG  -->  EGTP: Sending DL User Data(teId:%d)\n",teId);
               cnt =0;
               while(cnt < NUM_DL_PACKETS)
               {
                  ret =  cuEgtpDatReq(teId);      
                  if(ret != ROK)
                  {
                     DU_LOG("\nERROR --> EGTP: Issue with teid=%d\n",teId);
                     break;
                  }
                  /* TODO : sleep(1) will be removed later once we will be able to
                   * support the continuous data pack transfer */
                  sleep(1);
                  cnt++;
               }
            }
            totalNumOfTestFlow--;
         }
#endif
         continue;
      } 
   }
}
/**********************************************************************
         End of file
**********************************************************************/
