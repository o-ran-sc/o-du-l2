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
#include "nfapi_interface.h"
#include "pnf_stub_sctp.h"
#include "pnf_stub_p7_udp.h"
#include "pnf_stub.h"
#include "pnf_stub_p5_msg_hdl.h"


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
 
   /*P7 UDP Transport Cfg*/
   pnfCb.pnfCfgParams.pnfP7UdpParams.srcIpv4P7Addr = ipv4_pnf;
   pnfCb.pnfCfgParams.pnfP7UdpParams.srcIpv4Port = PNF_P7_UDP_PORT;
   pnfCb.pnfCfgParams.pnfP7UdpParams.destIpv4P7Addr = ipv4_vnf;
   pnfCb.pnfCfgParams.pnfP7UdpParams.destIpv4Port = PNF_P7_UDP_PORT;

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
 
   pnfP5SctpCfgReq();
  
   /* Initializing UDP global parameters */
   pnfP7UdpActvInit();
   pnfP7UdpCfgReq();

   /*Sleep is introduced for GDB to increase the waiting time for PNF Configuration from VNF*/
   sleep(1);
   
   /* Start PNF-P5-SCTP to listen on incoming connection */
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

/**********************************************************************
End of file
**********************************************************************/
