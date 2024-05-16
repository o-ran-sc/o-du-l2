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
#include "nfapi_interface.h"

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
   
   pnfCb.pnfCfgParams.pnfState=PNF_IDLE;
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

/*******************************************************************
 *
 * @Function Name: nFapiFillP5Hdr
 *
 *
 * @Functionality: 
 *    It Fills NFAPI P5 Msg Header[as per Table 2-3 " nFapi Header"]
 *
 *
 * @params 
 *         [OUT]: Msg Buffer to send in SCTP P5 Interface
 *
 * ****************************************************************/
void nfapiFillP5Hdr(Buffer *mBuf)
{
    CMCHKPK(oduPackPostUInt16, 0, mBuf);
    CMCHKPK(oduPackPostUInt8, 0, mBuf);
    CMCHKPK(oduPackPostUInt8, 0, mBuf);
    CMCHKPK(oduPackPostUInt32, 0, mBuf);
    return;
}

/*********************************************************************************
 *
 * @Function Name: nFapiFillMsgHdr
 *
 *
 * @Functionality: 
 *    It Fills Message Header[as per Table 2-4 " nFapi Msg Header"]
 *
 *
 * @params 
 *         [IN]: Phy_id, msgId, msgLen
 *         [OUT]: Msg Buffer to send in SCTP P5 Interface
 *
 * *******************************************************************************/

void nfapiFillMsgHdr(Buffer *mBuf, uint8_t phyId, uint16_t msgId, uint32_t msgLen)
{
    CMCHKPK(oduPackPostUInt8, 1, mBuf);
    CMCHKPK(oduPackPostUInt8, phyId, mBuf);
    CMCHKPK(oduPackPostUInt16, msgId, mBuf);
    CMCHKPK(oduPackPostUInt32, msgLen, mBuf);
    return;    
}

/*********************************************************************************
 *
 * @Function Name: nFapiExtractP5Hdr
 *
 *
 * @Functionality: 
 *    It extracts NFAPI P5 Message Header[as per Table 2-5 "P5 nFapi Header"]
 *
 *
 * @params 
 *         [IN]: Msg Buffer received in SCTP P5 Interface
 *         [OUT]: nFapi_p5_hdr *p5Hdr
 *
 * *******************************************************************************/
void nFapiExtractP5Hdr(nFapi_p5_hdr *p5Hdr, Buffer *mBuf)
{
   CMCHKPK(oduUnpackUInt16, &(p5Hdr->seg_len), mBuf);
   CMCHKPK(oduUnpackUInt8, &(p5Hdr->more_segNum), mBuf);
   CMCHKPK(oduUnpackUInt8, &(p5Hdr->seq_num), mBuf);
   CMCHKPK(oduUnpackUInt32, &(p5Hdr->timeStamp), mBuf);
   DU_LOG("\nINFo   --> NFAPI_PNF: seqLen:%d, moreSegNum:%d, seqNum:%d, timeStamp:%d",
        p5Hdr->seg_len,p5Hdr->more_segNum,p5Hdr->seq_num,p5Hdr->timeStamp);
   return;
}

/*********************************************************************************
 *
 * @Function Name: nFapiExtractMsgHdr
 *
 *
 * @Functionality: 
 *    It extracts Message Header[as per Table 2-4 "P5 nFapi Msg Header"]
 *
 *
 * @params 
 *         [IN]: Msg Buffer received in SCTP P5 Interface
 *         [OUT]: nFapi_msg_header *msgHdr
 *
 * *******************************************************************************/
void nFapiExtractMsgHdr(nFapi_msg_header *msgHdr, Buffer *mBuf)
{

    CMCHKPK(oduUnpackUInt8, &(msgHdr->sRU_termination_type), mBuf);
    CMCHKPK(oduUnpackUInt8, &(msgHdr->phy_id), mBuf);
    CMCHKPK(oduUnpackUInt16, &(msgHdr->msg_id), mBuf);
    CMCHKPK(oduUnpackUInt32, &(msgHdr->length), mBuf);

    DU_LOG("\nINFO  -->  NFAPI_PNF: RUType:%d, phy_id:%d, msgId:%d, len:%d",\
            msgHdr->sRU_termination_type,msgHdr->phy_id,msgHdr->msg_id,msgHdr->length );
    return;
}

/*********************************************************************************
 *
 * @Function Name: buildAndSendPnfReadyInd 
 *
 *
 * @Functionality: 
 *   Builds and Sends PNF_READY_IND(Ref: SCF 225, Sec 3.1.0)
 *
 *
 * *******************************************************************************/
uint8_t buildAndSendPnfReadyInd()
{
   uint8_t ret = ROK;
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> NFAPI_PNF : Memory allocation failed in pnf_readyInd");
      return RFAILED;
   }
   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, 0, TAG_NFAPI_PNF_READY_IND, 0);
   CMCHKPK(oduPackPostUInt32, 123, mBuf); //version
   ret = pnfP5SctpSend(mBuf);
   if(ret == RFAILED)
   {
      ODU_PUT_MSG_BUF(mBuf);
   }
   return ret;
}

/*********************************************************************************
 *
 * @Function Name: buildAndSendPnfParamResp 
 *
 * @Functionality: 
 *    Build Pnf Param rsp as per  5G nFAPI Specification 3.1.2 PNF_PARAM.response 
 *    And Send to VNF
 *
 * *******************************************************************************/

uint8_t buildAndSendPnfParamResp()
{
   uint8_t ret = ROK,idx=0, array_size=0;
   Buffer *mBuf = NULLP;
   nFapi_pnf_param_general paramGeneral;

   memset(&paramGeneral, 0, sizeof(nFapi_pnf_param_general));
   array_size = sizeof(paramGeneral.loc_coordinates) / sizeof(paramGeneral.loc_coordinates[0]);
   paramGeneral.max_num_phy=1;

   if (ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> NFAPI_PNF : Memory allocation failed in pnf_readyInd");
      return RFAILED;
   }
   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, 0, TAG_NFAPI_PNF_PARAM_RESP, 0);
   CMCHKPK(oduPackPostUInt8, 0, mBuf); 
   CMCHKPK(oduPackPostUInt8, 1, mBuf); 
   CMCHKPK(oduPackPostUInt16, TAG_NFAPI_PNF_PARAM_GENERAL, mBuf);
   CMCHKPK(oduPackPostUInt16, sizeof(nFapi_pnf_param_general), mBuf);
   CMCHKPK(oduPackPostUInt8, paramGeneral.nFapi_sync_mode, mBuf); 
   CMCHKPK(oduPackPostUInt8, paramGeneral.loc_mode, mBuf);
   CMCHKPK(oduPackPostUInt16, paramGeneral.max_num_phy, mBuf); 
   for(idx =0; idx < 3; idx++)
   {
      CMCHKPK(oduPackPostUInt8, paramGeneral.oui[idx], mBuf); 
   }
   CMCHKPK(oduPackPostUInt16, paramGeneral.numRfInstances, mBuf); 
   CMCHKPK(oduPackPostUInt16, paramGeneral.numDfeInstances, mBuf); 

   ret = pnfP5SctpSend(mBuf);
   if(ret == RFAILED)
   {
      ODU_PUT_MSG_BUF(mBuf);
   }
   return ret;
}

/*********************************************************************************
 *
 * @Function Name: buildAndSendPnfConfigResp
 *
 *
 * @Functionality:
 *   Builds and Sends PNF_CONFIG_RSP(Ref: SCF 225, Sec 3.1.4)
 *
 *
 * *******************************************************************************/
uint8_t buildAndSendPnfConfigResp()
{
   uint8_t ret = ROK;
   Buffer *mBuf = NULLP;

   DU_LOG("\nINFO   --> NFAPI_PNF: Building PNF_CONFIG_RSP");
   pnfCb.pnfCfgParams.pnfState=PNF_CONFIGURED;
   
   if(ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> NFAPI_PNF : Memory allocation failed in pnf config rsp");
      return RFAILED;
   }
   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, 0, TAG_NFAPI_PNF_CONFIG_RESP, 0);
   CMCHKPK(oduPackPostUInt32, 0, mBuf); //Error Code
   ret = pnfP5SctpSend(mBuf);
   if(ret == RFAILED)
   {
      ODU_PUT_MSG_BUF(mBuf);
   }
   return ret;
}

/*********************************************************************************
 *
 * @Function Name: buildAndSendPnfStartResp
 *
 *
 * @Functionality:
 *   Builds and Sends PNF_START_RSP(Ref: SCF 225, Sec 3.1.6)
 *
 *
 * *******************************************************************************/
uint8_t buildAndSendPnfStartResp()
{
   uint8_t ret = ROK;
   Buffer *mBuf = NULLP;

   DU_LOG("\nINFO   --> NFAPI_PNF: Building PNF_START_RSP");
   pnfCb.pnfCfgParams.pnfState=PNF_RUNNING;
   
   if(ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> NFAPI_PNF : Memory allocation failed in pnf start rsp");
      return RFAILED;
   }
   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, 0, TAG_NFAPI_PNF_START_RESP, 0);
   CMCHKPK(oduPackPostUInt32, 0, mBuf); //Error Code
   ret = pnfP5SctpSend(mBuf);
   if(ret == RFAILED)
   {
      ODU_PUT_MSG_BUF(mBuf);
   }
   return ret;
}

/*********************************************************************************
 *
 * @Function Name: buildAndSendPnfStopResp
 *
 *
 * @Functionality:
 *   Builds and Sends PNF_STOP_RSP(Ref: SCF 225, Sec 3.1.8)
 *
 *
 * *******************************************************************************/
uint8_t buildAndSendPnfStopResp()
{
   uint8_t ret = ROK;
   Buffer *mBuf = NULLP;

   DU_LOG("\nINFO   --> NFAPI_PNF: Building PNF_STOP_RSP");
   pnfCb.pnfCfgParams.pnfState=PNF_CONFIGURED;

   if(ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> NFAPI_PNF : Memory allocation failed in pnf stop rsp");
      return RFAILED;
   }
   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, 0, TAG_NFAPI_PNF_STOP_RESP, 0);
   CMCHKPK(oduPackPostUInt32, 0, mBuf); //Error Code
   ret = pnfP5SctpSend(mBuf);
   if(ret == RFAILED)
   {
      ODU_PUT_MSG_BUF(mBuf);
   }
   return ret;
}

/*********************************************************************************
 *
 * @Function Name: p5MsgHandlerAtPnf
 *
 *
 * @Functionality: 
 *    Handles the P5 MEssages at PNF SIM
 *
 * @Params [IN]: Message Buffer received at SCTP NFAPI P5 Interface
 *
 * *******************************************************************************/
uint8_t  p5MsgHandlerAtPnf(Buffer *mBuf)
{
   nFapi_p5_hdr p5Hdr;
   nFapi_msg_header msgHdr;
   uint8_t ret = ROK;

   nFapiExtractP5Hdr(&p5Hdr, mBuf);
   nFapiExtractMsgHdr(&msgHdr, mBuf);

   switch(msgHdr.msg_id)
   {
      case TAG_NFAPI_PNF_PARAM_REQ:
         {
            DU_LOG("\nINFO   --> NFAPI_PNF: PNF_PARAM_REQ recevied.");
            ret = buildAndSendPnfParamResp();
            break;
         }
      case TAG_NFAPI_PNF_CONFIG_REQ:
         {
            DU_LOG("\nINFO   --> NFAPI_PNF: PNF_CONFIG_REQ recevied.");
            ret = buildAndSendPnfConfigResp();
            break;
         }
      case TAG_NFAPI_PNF_START_REQ:
         {
            DU_LOG("\nINFO   --> NFAPI_PNF: PNF_START_REQ recevied.");
            ret = buildAndSendPnfStartResp();
            break;
         }
      case TAG_NFAPI_PNF_STOP_REQ:
         {
            DU_LOG("\nINFO   --> NFAPI_PNF: PNF_STOP_REQ recevied.");
            ret = buildAndSendPnfStopResp();
            break;
         }
      default:
      {
         DU_LOG("\nERROR  --> NFAPI_PNF: Wrong MSGID of NFAPI P5 Message:%d",msgHdr.msg_id);
      }
   }

   if(ret == RFAILED)
   {
      return RFAILED;
   }
   return ret;
}
