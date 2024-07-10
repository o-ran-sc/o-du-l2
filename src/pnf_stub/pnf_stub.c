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

extern uint32_t PER_TTI_TIME_USEC;
extern uint8_t  NUM_SLOTS_PER_SUBFRAME;
extern PnfGlobalCb pnfCb;

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

   DU_LOG("DEBUG  -->  PNF_STUB : Reading PNF P5 configurations");
   
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
   pnfCb.pnfCfgParams.pnfP7UdpParams.destIpv4Port = VNF_P7_UDP_PORT;

   pnfCb.pnfSlotInfo.sfn = 0xFFFF;
   pnfCb.pnfSlotInfo.slot = 0xFF;

} /* End of readPnfCfg */

/*******************************************************************
 *
 * @brief Trigger Pnf Clock from this function
 *
 * @details
 *
 *    Function : pnfClock
 *
 *    Functionality:
 *            - This function will trigger the PNF clock generation 
 *              and 
 *
 * @params[in] 
 * @return void
 *
 * ****************************************************************/
void *pnfClock()
{
   float  slotDur_ms = 0;
   struct timespec tti_req = {0}, currTime = {0};
   uint8_t ratio = 1;
   uint32_t currTime_ns = 0;

   /* Currently the code takes longer that one slot indication to execute.
    * Hence, multiplying slot time interval by 2 in order to give enough time 
    * for L2 to complete one slot processing.
    * The ratio must be removed once code optimization is complete */

   while(true)
   {
      if((pnfCb.pnfSlotInfo.slot == 0xFF && pnfCb.pnfSlotInfo.sfn == 0xFFFF))
      {
         /*Calculating the Slot Duration*/
         slotDur_ms = 1/pow(2, NUMEROLOGY);
         tti_req.tv_sec = 0;
         tti_req.tv_nsec = slotDur_ms * 1000000L * ratio;

         clock_gettime(CLOCK_REALTIME, &currTime);
         currTime_ns = currTime.tv_sec * 1000000000 +  currTime.tv_nsec;
         pnfCb.pnfP7Info.t_ref_ns = currTime_ns;
         
         PER_TTI_TIME_USEC = slotDur_ms * 1000;
         NUM_SLOTS_PER_SUBFRAME = (pow(2,NUMEROLOGY) * 10);
      
         pnfCb.pnfSlotInfo.sfn++;
         pnfCb.pnfSlotInfo.slot++;
         DU_LOG("PNF_NFAPI : Starting to generate slot indications t_ref:%llu, slotDur:%f, perTTi:%u, slotsPerFrame:%d, nanoSec:%d",\
               pnfCb.pnfP7Info.t_ref_ns, slotDur_ms, PER_TTI_TIME_USEC, NUM_SLOTS_PER_SUBFRAME, tti_req.tv_nsec);
      }
      else
      {
         CALC_NEXT_SFN_SLOT(pnfCb.pnfSlotInfo);
      }

#ifdef ODU_SLOT_IND_DEBUG_LOG
      DU_LOG("VNF_NFAPI -->  DEBUG:  SFN/Slot:%d,%d",\
               pnfCb.pnfSlotInfo.sfn, pnfCb.pnfSlotInfo.slot);
#endif
      clock_nanosleep(CLOCK_REALTIME, 0, &tti_req, NULL); 
   }
}

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
#if 1
   int retVal=0;
   pthread_t conThrdId;
   pthread_attr_t attr;
#endif

   init_log();   
   DU_LOG("INFO   -->  PNF_STUB : Starting PNF_STUB\n");

   /* TODO: Start thread to receive console input */
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
   
#if 1
   pthread_attr_init(&attr);
   pthread_attr_setstacksize(&attr, (size_t)NULLD);
   pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   retVal = pthread_create(&conThrdId, &attr, pnfClock, NULLP);
   if(retVal != 0)
   {
      DU_LOG("ERROR  -->  PNF_STUB :  Thread creation failed. Cause %d", retVal);
   }
   pthread_attr_destroy(&attr);
#endif

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
   DU_LOG("INFo   --> NFAPI_PNF: seqLen:%d, moreSegNum:%d, seqNum:%d, timeStamp:%d",
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

    DU_LOG("INFO  -->  NFAPI_PNF: RUType:%d, phy_id:%d, msgId:%d, len:%d",\
            msgHdr->sRU_termination_type,msgHdr->phy_id,msgHdr->msg_id,msgHdr->length );
    return;
}

/*********************************************************************************
 *
 * @Function Name: nFapiFillP7Hdr
 *
 *
 * @Functionality: 
 *    It Fills NFAPI P7 Msg Header[as per Table 2-5 " P7 nFapi Header"]
 *
 *
 * @params 
 *         [OUT]: Msg Buffer to send in UDP P7 Interface
 *
 * *******************************************************************************/

void nfapiFillP7Hdr(Buffer *mBuf,uint32_t totSduLen, uint32_t byteOffset, uint32_t time)
{
   CMCHKPK(oduPackPostUInt16, 0, mBuf);
   CMCHKPK(oduPackPostUInt32, totSduLen, mBuf);
   CMCHKPK(oduPackPostUInt32, byteOffset, mBuf);
   CMCHKPK(oduPackPostUInt32, time, mBuf);

}

/*********************************************************************************
 *
 * @Function Name: nFapiExtractP7Hdr
 *
 *
 * @Functionality: 
 *    It extracts NFAPI P7 Message Header[as per Table 2-5 "P7 nFapi Header"]
 *
 *
 * @params 
 *         [IN]: Msg Buffer received in UDP P7 Interface
 *         [OUT]: nFapi_p7_hdr *p7Hdr
 *
 * *******************************************************************************/

void nFapiExtractP7Hdr(nFapi_p7_hdr *p7Hdr, Buffer *mBuf)
{
   CMCHKPK(oduUnpackUInt16, &(p7Hdr->seq_num), mBuf);
   CMCHKPK(oduUnpackUInt32, &(p7Hdr->tot_SDU_len), mBuf);
   CMCHKPK(oduUnpackUInt32, &(p7Hdr->byteOffset), mBuf);
   CMCHKPK(oduUnpackUInt32, &(p7Hdr->timeStamp), mBuf);
   DU_LOG("INFo   --> NFAPI_VNF: se1_num:%d, totSdu len:%u, byteOffset:%u, timeStamp:%u",
        p7Hdr->seq_num,p7Hdr->tot_SDU_len, p7Hdr->byteOffset, p7Hdr->timeStamp);
}

/*********************************************************************************
 *
 * @Function Name: convertIpStringToUInt8
 *
 *
 * @Functionality: 
 *    convert the ip address and store in uint8_t array[4]
 *
 * @params 
 *         [IN]: Ip address which needs to convert
 *         [IN]: Array to store the result
 *
 * *******************************************************************************/
void convertIpStringToUInt8(const char *ip_str, uint8_t ip_address[]) 
{
    char ip_copy[16];
    strncpy(ip_copy, ip_str, sizeof(ip_copy) - 1);
    ip_copy[sizeof(ip_copy) - 1] = '\0';  // Ensure null-terminated

    char *token = strtok(ip_copy, ".");
    for (int i = 0; token != NULL && i < 4; i++) {
        ip_address[i] = (uint8_t)atoi(token);  // Convert each token to an integer
        token = strtok(NULL, ".");
    }
}

/*********************************************************************************
 *
 * @Function Name: fillTlvOfArrayOfUint8 
 *
 *
 * @Functionality: 
 *    fill tlv of array of size uint8 
 *
 * @params 
 *         [IN]: Buffer, tag, length, value
 *
 * *******************************************************************************/
void fillTlvOfArrayOfUint8(Buffer *mBuf, uint16_t tag, uint16_t length, uint8_t *value)
{
   uint8_t arraySize=4;
   CMCHKPK(oduPackPostUInt16, tag, mBuf);
   CMCHKPK(oduPackPostUInt16, length, mBuf);
   for(uint8_t idx=0;idx<arraySize;idx++)
   {
      CMCHKPK(oduPackPostUInt8, value[idx], mBuf);
   }
}

/*********************************************************************************
 *
 * @Function Name: fillTlvOfSizeUint8 
 *
 *
 * @Functionality: 
 *    fill tlv of size uint8 
 *
 * @params 
 *         [IN]: Buffer, tag, length, value
 *
 * *******************************************************************************/
void fillTlvOfSizeUint8(Buffer *mBuf, uint16_t tag, uint16_t length, uint8_t value)
{
   CMCHKPK(oduPackPostUInt16, tag, mBuf);
   CMCHKPK(oduPackPostUInt16, length, mBuf);
   CMCHKPK(oduPackPostUInt8, value, mBuf);
}

/*********************************************************************************
 *
 * @Function Name: fillTlvOfSizeUint16 
 *
 *
 * @Functionality: 
 *    fill tlv of size uint16 
 *
 * @params 
 *         [IN]: Buffer, tag, length, value
 *
 * *******************************************************************************/
void fillTlvOfSizeUint16(Buffer *mBuf, uint16_t tag, uint16_t length, uint16_t value)
{
   CMCHKPK(oduPackPostUInt16, tag, mBuf);
   CMCHKPK(oduPackPostUInt16, length, mBuf);
   CMCHKPK(oduPackPostUInt16, value, mBuf);
}

/*********************************************************************************
 *
 * @Function Name: fillTlvOfSizeUint32 
 *
 *
 * @Functionality: 
 *    fill tlv of size uint32 
 *
 * @params 
 *         [IN]: Buffer, tag, length, value
 *
 * *******************************************************************************/
void fillTlvOfSizeUint32(Buffer *mBuf, uint16_t tag, uint16_t length, uint32_t value)
{
   CMCHKPK(oduPackPostUInt16, tag, mBuf);
   CMCHKPK(oduPackPostUInt16, length, mBuf);
   CMCHKPK(oduPackPostUInt32, value, mBuf);
}

/**********************************************************************
End of file
**********************************************************************/
