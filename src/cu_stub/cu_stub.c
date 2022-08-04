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
#include "cu_stub_sctp.h"
#include "cu_stub_egtp.h"
#include "OCTET_STRING.h"
#include "cu_f1ap_msg_hdl.h"
#include "cu_stub.h"

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
   uint8_t  numDu;
   uint32_t ipv4_du, ipv4_cu;

   DU_LOG("\nDEBUG  -->  CU_STUB : Reading CU configurations");

   cuCb.cuCfgParams.cuId = CU_ID;
   strcpy(cuCb.cuCfgParams.cuName, CU_NAME);

#ifdef O1_ENABLE
   if( getStartupConfigForStub(&g_cfg) != ROK )
   {
      DU_LOG("\nError  -->  CU_STUB : Could not fetch startup "\
             "configurations from Netconf interface\n");
      exit(1);
   }
   
   cmInetAddr((S8*)g_cfg.DU_IPV4_Addr, &ipv4_du);
   cuCb.cuCfgParams.sctpParams.sctpAssoc[0].duIpAddr.ipV4Addr = ipv4_du;
   cuCb.cuCfgParams.sctpParams.sctpAssoc[0].duIpAddr.ipV6Pres = false;
   
   cmInetAddr((S8*)g_cfg.CU_IPV4_Addr, &ipv4_cu);
   cuCb.cuCfgParams.sctpParams.sctpAssoc[0].cuIpAddr.ipV4Addr = ipv4_cu;
   cuCb.cuCfgParams.sctpParams.sctpAssoc[0].cuIpAddr.ipV6Pres = false;
   
   cuCb.cuCfgParams.sctpParams.sctpAssoc[0].duPort = g_cfg.DU_Port;
   cuCb.cuCfgParams.sctpParams.sctpAssoc[0].cuPort = g_cfg.CU_Port; 
   cuCb.cuCfgParams.sctpParams.numDu = 1;
   
   cuCb.cuCfgParams.egtpParams.egtpAssoc[0].localIp.ipV4Pres = TRUE;
   cuCb.cuCfgParams.egtpParams.egtpAssoc[0].localIp.ipV4Addr = ipv4_cu;
   cuCb.cuCfgParams.egtpParams.egtpAssoc[0].localPort = CU_EGTP_PORT[0];
   cuCb.cuCfgParams.egtpParams.egtpAssoc[0].destIp.ipV4Pres = TRUE;
   cuCb.cuCfgParams.egtpParams.egtpAssoc[0].destIp.ipV4Addr = ipv4_du;
   cuCb.cuCfgParams.egtpParams.egtpAssoc[0].destPort = DU_EGTP_PORT[0];
   cuCb.cuCfgParams.egtpParams.minTunnelId = MIN_TEID;
   cuCb.cuCfgParams.egtpParams.currTunnelId = cuCb.cuCfgParams.egtpParams.minTunnelId;
   cuCb.cuCfgParams.egtpParams.maxTunnelId = MAX_TEID;
   cuCb.cuCfgParams.egtpParams.numDu = 1;

#else
   cuCb.cuCfgParams.sctpParams.numDu = 0;
   cuCb.cuCfgParams.egtpParams.numDu = 0;
   numDu = 0;
   while(numDu < MAX_DU_SUPPORTED)
   {
      /* DU IP Address and Port*/
      memset(&ipv4_du, 0, sizeof(uint32_t));
      cmInetAddr((S8*)DU_IP_V4_ADDR[numDu], &ipv4_du);
      cuCb.cuCfgParams.sctpParams.sctpAssoc[numDu].duIpAddr.ipV4Addr = ipv4_du;
      cuCb.cuCfgParams.sctpParams.sctpAssoc[numDu].duIpAddr.ipV6Pres = false;
      cuCb.cuCfgParams.sctpParams.sctpAssoc[numDu].duPort = DU_SCTP_PORT[numDu];

      /* CU IP Address and Port*/
      memset(&ipv4_du, 0, sizeof(uint32_t));
      cmInetAddr((S8*)CU_IP_V4_ADDR, &ipv4_cu);
      cuCb.cuCfgParams.sctpParams.sctpAssoc[numDu].cuIpAddr.ipV4Addr = ipv4_cu;
      cuCb.cuCfgParams.sctpParams.sctpAssoc[numDu].cuIpAddr.ipV6Pres = false;
      cuCb.cuCfgParams.sctpParams.sctpAssoc[numDu].cuPort = CU_SCTP_PORT_TO_DU[numDu];

      /* EGTP Parameters */
      memset(&ipv4_du, 0, sizeof(uint32_t));
      cmInetAddr((S8*)DU_IP_V4_ADDR[numDu], &ipv4_du);
      cuCb.cuCfgParams.egtpParams.egtpAssoc[numDu].localIp.ipV4Pres = TRUE;
      cuCb.cuCfgParams.egtpParams.egtpAssoc[numDu].localIp.ipV4Addr = ipv4_cu;
      cuCb.cuCfgParams.egtpParams.egtpAssoc[numDu].localPort = CU_EGTP_PORT[numDu];
      cuCb.cuCfgParams.egtpParams.egtpAssoc[numDu].destIp.ipV4Pres = TRUE;
      cuCb.cuCfgParams.egtpParams.egtpAssoc[numDu].destIp.ipV4Addr = ipv4_du;
      cuCb.cuCfgParams.egtpParams.egtpAssoc[numDu].destPort = DU_EGTP_PORT[numDu];
      
      (numDu)++;
   }

   cuCb.cuCfgParams.egtpParams.minTunnelId = MIN_TEID;
   cuCb.cuCfgParams.egtpParams.currTunnelId = cuCb.cuCfgParams.egtpParams.minTunnelId;
   cuCb.cuCfgParams.egtpParams.maxTunnelId = MAX_TEID;
   cuCb.cuCfgParams.egtpParams.numDu = numDu;
   cuCb.cuCfgParams.sctpParams.numDu = numDu;
#endif

   /*PLMN*/
   cuCb.cuCfgParams.plmn.mcc[0] = PLMN_MCC0;
   cuCb.cuCfgParams.plmn.mcc[1] = PLMN_MCC1;
   cuCb.cuCfgParams.plmn.mcc[2] = PLMN_MCC2;
   cuCb.cuCfgParams.plmn.mnc[0] = PLMN_MNC0;
   cuCb.cuCfgParams.plmn.mnc[1] = PLMN_MNC1;
   cuCb.cuCfgParams.plmn.mnc[2] = PLMN_MNC2;

   /*RRC Version*/
   cuCb.cuCfgParams.rrcVersion.rrcVer = RRC_VER;
   cuCb.cuCfgParams.rrcVersion.extRrcVer = EXT_RRC_VER;



} /* End of readCuCfg */

/*******************************************************************
 *
 * @brief Initiates inter DU handover
 *
 * @details
 *
 *    Function : initiateInterDuHandover
 *
 *    Functionality: Initiates the first procedure of inter-DU
 *    handover i.eG GNB-DU configuration query to source DU
 *
 * @params[in] Source DU Id
 *             Target DU Id
 *             UE Id to be handed off 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void initiateInterDuHandover(uint32_t sourceDuId, uint32_t targetDuId, uint32_t ueId)
{
    uint8_t duIdx = 0;
    DuDb *duDb = NULLP;
    CuUeCb *ueCb = NULLP;
   
    SEARCH_DU_DB(duIdx, sourceDuId, duDb); 
    if(duDb)
       ueCb = &duDb->ueCb[ueId-1];
    if(ueCb)
    {
       ueCb->state = UE_HANDOVER_IN_PROGRESS;
       ueCb->hoInfo.sourceDuId = sourceDuId;
       ueCb->hoInfo.targetDuId = targetDuId;
    }

    DU_LOG("\nINFO  --> CU_STUB: Inter-DU Handover Started for ueId [%d] from DU ID [%d] to DU ID [%d]", \
          ueId, sourceDuId, targetDuId);
    BuildAndSendUeContextModificationReq(sourceDuId, ueCb, QUERY_CONFIG);
}

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
   uint32_t teId = 0;
   uint32_t duId;
   uint8_t ret = ROK;
   uint8_t cnt = 0;
   EgtpTeIdCb *teidCb = NULLP;

   while(true) 
   {
      ch = getchar();
      /* Send DL user data to CU when user enters 'd' on console */
      if(ch == 'd')
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

         /* This variable is taken for sending specific number of downlink data packet. 
          * Presently the code is supporting total 4500 data packets trasfer for 3 UEs only with sleep(1).
          * If you wants to pump data for 3 UE change the following macro values
          * NUM_TUNNEL_TO_PUMP_DATA = 9, NUM_DL_PACKETS = 1.
          * totalDataPacket = totalNumOfTestFlow * NUM_TUNNEL_TO_PUMP_DATA * NUM_DL_PACKETS 
          * totalDataPacket = [500*9*1] */
         int32_t totalNumOfTestFlow = 2; 

         while(totalNumOfTestFlow)
         {
            for(duId = 1; duId<=MAX_DU_SUPPORTED; duId++)
            {
               for(teId = 1; teId <= NUM_TUNNEL_TO_PUMP_DATA; teId++)
               {
                  teidCb = NULLP;
                  cmHashListFind(&(egtpCb.dstCb[duId-1].teIdLst), (uint8_t *)&(teId), sizeof(uint32_t), 0, (PTR *)&teidCb);
                  if(teidCb)
                  {
                     cnt =0;
                     DU_LOG("\nDEBUG  -->  EGTP: Sending DL User Data(duId %d, teId:%d)\n", duId, teId);
                     while(cnt < NUM_DL_PACKETS)
                     {
                        ret =  cuEgtpDatReq(duId, teId);      
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
                  else
                  {
                     DU_LOG("\nDEBUG  -->  EGTP: TunnelId Not Found for (duId %d, teId:%d)\n", duId, teId);
                  }
               }
            }
            totalNumOfTestFlow--;
         }
#endif
         continue;
      } 

      /* Start Handover procedure towards DU when 'h' is received from console input */
      else if(ch == 'h')
      {
         uint32_t sourceDuId, targetDuId, ueId;

         DU_LOG("\nEnter Source DU ID for Inter-DU Handover");
         scanf("%d", &sourceDuId);
         DU_LOG("\nEnter Target DU ID for Inter-DU Handover");
         scanf("%d", &targetDuId);
         DU_LOG("\nEnter DU UE F1AP ID to be handed over");
         scanf("%d", &ueId);

         initiateInterDuHandover(sourceDuId, targetDuId, ueId);
      }
      /* Start Idle mode paging when 'p' is received from console input */
      else if(ch == 'p')
      {
         uint64_t sTmsi = 0;
         uint8_t duId = 0;

         DU_LOG("\nEnter DU ID on which this UE to be pagged");
         scanf("%d", &duId);
         DU_LOG("\nEnter 5g-S-TMSI");
         scanf("%lu", &sTmsi);

         if(BuildAndSendPagingMsg(sTmsi, duId) != ROK)
         {
            DU_LOG("\nERROR --> EGTP: Failed to build and send paging message for 5gsTmsi[%lu]\n", sTmsi);   
         }
         continue;
      }
   }
}
/**********************************************************************
         End of file
**********************************************************************/
