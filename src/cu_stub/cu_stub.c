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
   uint8_t  numDu, *numRemoteCu;
   uint32_t ipv4_du, ipv4_cu, ipv4_remote_cu;

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
   cuCb.cuCfgParams.sctpParams.f1SctpInfo.destCb[0].destIpAddr.ipV4Addr = ipv4_du;
   cuCb.cuCfgParams.sctpParams.f1SctpInfo.destCb[0].destIpAddr.ipV6Pres = false;
   
   cmInetAddr((S8*)g_cfg.CU_IPV4_Addr, &ipv4_cu);
   cuCb.cuCfgParams.sctpParams.localIpAddr.ipV4Addr = ipv4_cu;
   cuCb.cuCfgParams.sctpParams.localIpAddr.ipV6Pres = false;
   
   cuCb.cuCfgParams.sctpParams.f1SctpInfo.destCb[0].destPort = g_cfg.DU_Port;
   cuCb.cuCfgParams.sctpParams.f1SctpInfo.port = g_cfg.CU_Port; 
   cuCb.cuCfgParams.sctpParams.f1SctpInfo.numDestNode = 1;
   
   cuCb.cuCfgParams.egtpParams.localIp.ipV4Pres = TRUE;
   cuCb.cuCfgParams.egtpParams.localIp.ipV4Addr = ipv4_cu;
   cuCb.cuCfgParams.egtpParams.localPort = F1_EGTP_PORT;
   cuCb.cuCfgParams.egtpParams.dstCfg[0].dstIp.ipV4Pres = TRUE;
   cuCb.cuCfgParams.egtpParams.dstCfg[0].dstIp.ipV4Addr = ipv4_du;
   cuCb.cuCfgParams.egtpParams.dstCfg[0].dstPort = F1_EGTP_PORT;
   cuCb.cuCfgParams.egtpParams.minTunnelId = MIN_TEID;
   cuCb.cuCfgParams.egtpParams.currTunnelId = cuCb.cuCfgParams.egtpParams.minTunnelId;
   cuCb.cuCfgParams.egtpParams.maxTunnelId = MAX_TEID;
   cuCb.cuCfgParams.egtpParams.numDu = 1;

#else
   /* CU IP Address and Port*/
   memset(&ipv4_du, 0, sizeof(uint32_t));
   cmInetAddr((S8*)LOCAL_IP_CU, &ipv4_cu);

   cuCb.cuCfgParams.sctpParams.localIpAddr.ipV4Addr = ipv4_cu;
   cuCb.cuCfgParams.sctpParams.localIpAddr.ipV6Pres = false;

   /* SCTP and EGTP configuration for F1 Interface */
   cuCb.cuCfgParams.sctpParams.f1SctpInfo.port = F1_SCTP_PORT;

   cuCb.cuCfgParams.egtpParams.localIp.ipV4Pres = TRUE;
   cuCb.cuCfgParams.egtpParams.localIp.ipV4Addr = ipv4_cu;
   cuCb.cuCfgParams.egtpParams.localPort = F1_EGTP_PORT;

   cuCb.cuCfgParams.sctpParams.f1SctpInfo.numDestNode = 0;
   cuCb.cuCfgParams.egtpParams.numDu = 0;

   numDu = 0;
   while(numDu < NUM_F1_ASSOC)
   {
      /* DU IP Address */
      memset(&ipv4_du, 0, sizeof(uint32_t));
      cmInetAddr((S8*)REMOTE_IP_DU[numDu], &ipv4_du);
      
      /* SCTP Parameters */
      cuCb.cuCfgParams.sctpParams.f1SctpInfo.destCb[numDu].destIpAddr.ipV4Addr = ipv4_du;
      cuCb.cuCfgParams.sctpParams.f1SctpInfo.destCb[numDu].destIpAddr.ipV6Pres = false;
      cuCb.cuCfgParams.sctpParams.f1SctpInfo.destCb[numDu].destPort = F1_SCTP_PORT;

      /* EGTP Parameters */
      cuCb.cuCfgParams.egtpParams.dstCfg[numDu].dstIp.ipV4Pres = TRUE;
      cuCb.cuCfgParams.egtpParams.dstCfg[numDu].dstIp.ipV4Addr = ipv4_du;
      cuCb.cuCfgParams.egtpParams.dstCfg[numDu].dstPort = F1_EGTP_PORT;
      
      (numDu)++;
   }

   cuCb.cuCfgParams.egtpParams.minTunnelId = MIN_TEID;
   cuCb.cuCfgParams.egtpParams.currTunnelId = cuCb.cuCfgParams.egtpParams.minTunnelId;
   cuCb.cuCfgParams.egtpParams.maxTunnelId = MAX_TEID;
   cuCb.cuCfgParams.egtpParams.numDu = numDu;
   cuCb.cuCfgParams.sctpParams.f1SctpInfo.numDestNode = numDu;

   /* SCTP configuration for Xn interface */
   cuCb.cuCfgParams.sctpParams.xnSctpInfo.port = XN_SCTP_PORT;
   cuCb.cuCfgParams.sctpParams.xnSctpInfo.localNodeType = LOCAL_NODE_TYPE;
   numRemoteCu = &cuCb.cuCfgParams.sctpParams.xnSctpInfo.numDestNode;   
   (*numRemoteCu) = 0;
   while((*numRemoteCu) < NUM_XN_ASSOC)
   {
      /* Remote CU IP address */
      memset(&ipv4_remote_cu, 0, sizeof(uint32_t));
      cmInetAddr((S8*)REMOTE_IP_CU[*numRemoteCu], &ipv4_remote_cu);

      /* SCTP Parameters */
      cuCb.cuCfgParams.sctpParams.xnSctpInfo.destCb[*numRemoteCu].destIpAddr.ipV4Addr = ipv4_remote_cu;
      cuCb.cuCfgParams.sctpParams.xnSctpInfo.destCb[*numRemoteCu].destIpAddr.ipV6Pres = false;
      cuCb.cuCfgParams.sctpParams.xnSctpInfo.destCb[*numRemoteCu].destPort = XN_SCTP_PORT;

      (*numRemoteCu)++;
   }
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
void initiateInterDuHandover(uint32_t sourceDuId, uint32_t targetDuId, uint32_t duUeF1apId)
{
   uint8_t duIdx = 0;
   DuDb *duDb = NULLP;
   CuUeCb *ueCb = NULLP;

   DU_LOG("\nINFO  --> CU_STUB: Inter-DU Handover Started for ueId [%d] from DU ID [%d] to DU ID [%d]", \
         duUeF1apId, sourceDuId, targetDuId);

   SEARCH_DU_DB(duIdx, sourceDuId, duDb); 
   if(duDb)
      ueCb = &duDb->ueCb[duUeF1apId-1];
   if(ueCb)
   {
      ueCb->state = UE_HANDOVER_IN_PROGRESS;
      ueCb->hoInfo.HOType = Inter_DU_HO;
      ueCb->hoInfo.srcNodeId = sourceDuId;
      ueCb->hoInfo.tgtNodeId = targetDuId;

      BuildAndSendUeContextModificationReq(sourceDuId, ueCb, QUERY_CONFIG);
   }
   else
   {
      DU_LOG("\nINFO  --> CU_STUB: DU UE F1AP ID [%d] not found", duUeF1apId);
   }
}


/*******************************************************************
 *
 * @brief Initiates inter CU handover
 *
 * @details
 *
 *    Function : initiateInterCuHandover
 *
 *    Functionality: Initiates the first procedure of inter-CU
 *    handover
 *
 * @params[in] Source CU Id
 *             Target CU Id
 *             UE Id to be handed off 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void initiateInterCuHandover(uint32_t targetCuId, uint32_t cellId, uint32_t cuUeF1apId)
{
    uint8_t  duIdx, ueIdx;
    CuUeCb   *ueCb = NULLP;

    DU_LOG("\nINFO  --> CU_STUB: Inter-CU Handover Started for ueId [%d] to CU ID [%d]", \
          cuUeF1apId, targetCuId);

    for(duIdx = 0; duIdx < cuCb.numDu; duIdx++)
    {
       for(ueIdx = 0; ueIdx < (MAX_NUM_CELL * MAX_NUM_UE); ueIdx++)
       {
          ueCb = &cuCb.duInfo[duIdx].ueCb[ueIdx];
          if(cuCb.duInfo[duIdx].ueCb[ueIdx].gnbCuUeF1apId == cuUeF1apId)
          {
             ueCb = &cuCb.duInfo[duIdx].ueCb[ueIdx];
             break; 
          }
       }
       if(ueCb)
          break;
    }

    if(ueCb)
    {   
       ueCb->state = UE_HANDOVER_IN_PROGRESS;
       ueCb->hoInfo.HOType = Xn_Based_Inter_CU_HO;
       ueCb->hoInfo.srcNodeId = cuCb.cuCfgParams.cuId;
       ueCb->hoInfo.tgtNodeId = targetCuId;
       ueCb->hoInfo.tgtCellId = cellId;
       ueCb->hoInfo.cuUeF1apIdSrc = ueCb->gnbCuUeF1apId;

       BuildAndSendUeContextModificationReq(cuCb.duInfo[duIdx].duId, ueCb, QUERY_CONFIG);
    }   
    else
    {
       DU_LOG("\nINFO  --> CU_STUB: CU UE F1AP ID [%d] not found", cuUeF1apId);
    }
}

/*******************************************************************
 *
 * @brief start Dl data
 *
 * @details
 *
 *    Function : startDlData
 *
 *    Functionality: start the downlink data
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t startDlData()
{
   uint32_t teId = 0;
   uint32_t duId;
   uint8_t ret = ROK;
   uint8_t cnt = 0;
   int32_t totalNumOfTestFlow = 200; 
   EgtpTeIdCb *teidCb = NULLP;
   
   while(totalNumOfTestFlow)
   {
      for(duId = 1; duId <= cuCb.cuCfgParams.egtpParams.numDu; duId++)
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
   
   return ROK;
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
         
         startDlData();
#endif
         continue;
      } 

      /* Start Handover procedure towards DU when 'h' is received from console input */
      else if(ch == 'h')
      {
         HandoverType hoType;
         uint32_t srcNodeId, tgtNodeId, targetCellId, ueId;

         DU_LOG("\n\nChoose the type of handover to initiate : \nEnter 1 for Inter-CU Handover over Xn interface\nEnter 2 for Inter-DU Handover\n");
         scanf("%d", &hoType);

         if(hoType == Xn_Based_Inter_CU_HO)
         {
            DU_LOG("\nEnter Target CU ID for Inter-CU Handover : ");
            scanf("%d", &tgtNodeId);
            DU_LOG("\nEnter Target Physical Cell ID for Inter-CU Handover : ");
            scanf("%d", &targetCellId);
            DU_LOG("\nEnter CU UE F1AP ID to be handed over : ");
            scanf("%d", &ueId);

            initiateInterCuHandover(tgtNodeId, targetCellId, ueId);
         }
         else if(hoType == Inter_DU_HO)
         {
            DU_LOG("\nEnter Source DU ID for Inter-DU Handover : ");
            scanf("%d", &srcNodeId);
            DU_LOG("\nEnter Target DU ID for Inter-DU Handover : ");
            scanf("%d", &tgtNodeId);
            DU_LOG("\nEnter DU UE F1AP ID to be handed over : ");
            scanf("%d", &ueId);

            initiateInterDuHandover(srcNodeId, tgtNodeId, ueId);
         }
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
      else if(ch == 'm')
      {
         uint8_t  ueId  = 1;
         uint8_t  duId  = 0;
         uint8_t  duIdx = 0;
         DuDb    *duDb  = NULLP;
         CuUeCb  *ueCb  = NULLP;

         DU_LOG("\nEnter DU ID whose UE has to be modified");
         scanf("%d", &duId);
         DU_LOG("\nEnter UE ID to be modified");
         scanf("%d", &ueId);

         DU_LOG("\nINFO  --> CU_STUB: UE Context Mod for ueId [%d] at DU ID [%d]", \
                  ueId, duId);

         SEARCH_DU_DB(duIdx, duId, duDb); 
         if(duDb)
         {
            ueCb = &duDb->ueCb[ueId-1];
            BuildAndSendUeContextModificationReq(duId, ueCb, MODIFY_UE);
         }
         else
         {
            DU_LOG("ERROR --> DuDb is NULLP");
         }
         continue;

      }
      /*UE context release command from CU*/
      else if(ch == 'c')
      {
         uint32_t duId, cuUeF1apId, duUeF1apId;
         uint8_t  duIdx = 0;
         DuDb    *duDb  = NULLP;
         
         DU_LOG("\nEnter DU ID on which UE has to be released");
         scanf("%d", &duId);
         DU_LOG("\nEnter UE ID to be released");
         scanf("%d", &duUeF1apId);
        
         SEARCH_DU_DB(duIdx, duId, duDb); 
         if(duDb)
         {
            if(duDb->ueCb[duUeF1apId-1].gnbDuUeF1apId == duUeF1apId)
            {
               cuUeF1apId = duDb->ueCb[duUeF1apId-1].gnbCuUeF1apId;
               DU_LOG("INFO   -->  CU_STUB: Cu UeId: %d Du UeId:%d",cuUeF1apId, duUeF1apId);
               BuildAndSendUeContextReleaseCommand(duId, cuUeF1apId, duUeF1apId);
            }
            else
            {
               DU_LOG("ERROR  -->  CU_STUB: Du UeId:%d in UeCb mismatch",\
                        duDb->ueCb[duUeF1apId-1].gnbDuUeF1apId);
            }
         }

         sleep(5);
         continue;
      }
   }
}
/**********************************************************************
         End of file
**********************************************************************/
