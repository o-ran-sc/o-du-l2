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

/*Reference: SCF225_5G_NFAPI_SPECIFICATION, v225.2.1, Issue Date: 23 Nov 2021*/

/* header include files -- defines (.h) */
#ifdef NFAPI_ENABLED

#include "common_def.h"
#include "mac_utils.h"
#include "lwr_mac.h"
#include "nfapi_interface.h"
#include "nfapi_vnf_fsm.h"
#include "nfapi_common.h"

/*********************************************************************************
 *
 * @Function Name: nFapiFillP5Hdr
 *
 *
 * @Functionality: 
 *    It Fills NFAPI P5 Msg Header[as per Table 2-3 " P5 nFapi Header"]
 *
 *
 * @params 
 *         [OUT]: Msg Buffer to send in SCTP P5 Interface
 *
 * *******************************************************************************/

void nfapiFillP5Hdr(Buffer *mBuf)
{
   uint8_t moreSegNum = 0;
   moreSegNum = NFAPI_MORE_SEG_NUM(NFAPI_P5_MORE, NFAPI_P5_SEG_NUM);

   CMCHKPK(oduPackPostUInt16, NFAPI_P5_SEG_NUM, mBuf);
   CMCHKPK(oduPackPostUInt8, moreSegNum, mBuf);
   CMCHKPK(oduPackPostUInt8, vnfDb.p5Info.seqNum, mBuf);
   CMCHKPK(oduPackPostUInt32, NFAPI_P5_TIMESTAMP, mBuf);

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
 * @params [IN]:Buffer *mBuf,uint32_t totSduLen, uint32_t byteOffset, uint32_t time
 *         [OUT]: void
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
    CMCHKPK(oduPackPostUInt8, NFAPI_P5_P7_SRU_TYPE, mBuf);
    CMCHKPK(oduPackPostUInt8, phyId, mBuf);
    CMCHKPK(oduPackPostUInt16, msgId, mBuf);
    CMCHKPK(oduPackPostUInt32, msgLen, mBuf);
    
}

/*********************************************************************************
 *
 * @Function Name: nFapiExtractP5Hdr
 *
 *
 * @Functionality: 
 *    It extracts NFAPI P5 Message Header[as per Table 2-3 "P5 nFapi Header"]
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
   DU_LOG("INFo   --> NFAPI_VNF: seqLen:%d, moreSegNum:%d, seqNum:%d, timeStamp:%d",
        p5Hdr->seg_len,p5Hdr->more_segNum,p5Hdr->seq_num,p5Hdr->timeStamp);
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

    DU_LOG("INFO  -->  NFAPI_VNF: RUType:%d, phy_id:%d, msgId:%d, len:%d",\
            msgHdr->sRU_termination_type,msgHdr->phy_id,msgHdr->msg_id,msgHdr->length );
}

/*********************************************************************************
 * @Function Name: convertNfapiP5TagValToMsgId
 *
 *
 * @Functionality: 
 *    It converts NFAPI_P5 Tag VALUE [as per Table 2-7 "Dedicated nFAPI messages"]
 *    to MSG_EVENT (NfapiPnfEvent in nfapi_vnf_fsm.h)
 *
 *
 * @params 
 *         [IN]: tagVal from (SCF 225 Table 2-7 "Dedicated nFAPI messages")
 *         [IN]: Nfapi Pnf Event
 *         [IN]: Phy Event State
 *         [OUT]: ROK or RFAILED 
 *
 * *******************************************************************************/
uint8_t convertNfapiP5TagValToMsgId(uint16_t tagVal, NfapiPnfEvent *nfapiPnfEvent, EventState *phyEvent)
{
   *nfapiPnfEvent=PNF_MAX_EVENT;
   *phyEvent=MAX_EVENT;

   switch(tagVal)
   {
      case TAG_NFAPI_PNF_READY_IND:
        {
           *(nfapiPnfEvent)  = PNF_READY_IND;
           break;
        }
      case TAG_NFAPI_PNF_PARAM_REQ:
        {
           *(nfapiPnfEvent)  = PNF_PARAM_REQ;
           break;
        }
      case TAG_NFAPI_PNF_PARAM_RESP:
        {
           *(nfapiPnfEvent)  = PNF_PARAM_RESP;
           break;
        }
      case TAG_NFAPI_PNF_CONFIG_REQ:
        {
           *(nfapiPnfEvent)  = PNF_CONFIG_REQ;
           break;
        }
      case TAG_NFAPI_PNF_CONFIG_RESP:
        {
           *(nfapiPnfEvent)  = PNF_CONFIG_RESP;
           break;
        }
      case TAG_NFAPI_PNF_START_REQ:
        {
           *(nfapiPnfEvent)  = PNF_START_REQ;
           break;
        }
      case TAG_NFAPI_PNF_START_RESP:
        {
           *(nfapiPnfEvent)  = PNF_START_RESP;
           break;
        }
      case TAG_NFAPI_PNF_STOP_REQ:
        {
           *(nfapiPnfEvent)  = PNF_STOP_REQ;
           break;
        }
      case TAG_NFAPI_PNF_STOP_RESP:
        {
           *nfapiPnfEvent = PNF_STOP_RESP;
           break;
        }
      case FAPI_PARAM_REQUEST:
        {
           *(phyEvent) = PARAM_REQUEST;
           break;
        }
      case FAPI_PARAM_RESPONSE: 
        {
           *(phyEvent) = PARAM_RESPONSE;
           break;
        }
      case FAPI_CONFIG_REQUEST:
        {
           *(phyEvent) = CONFIG_REQUEST;
           break;
        }
      case FAPI_CONFIG_RESPONSE: 
        {
           *(phyEvent) = CONFIG_RESPONSE;
           break;
        }
      case FAPI_START_REQUEST:
        {
           *(phyEvent) = START_REQUEST;
           break;
        }
      case FAPI_STOP_REQUEST:
        {
           *(phyEvent) = STOP_REQUEST;
           break;
        }
      case TAG_NFAPI_START_RESPONSE:
        {
           *(phyEvent) = START_RESPONSE;
           break;
        }
      default:
        {
           DU_LOG(" Incorrect TAG VALUE of NFAPI P5 Messages:%d",tagVal);
           return RFAILED;
        }
   }
   return ROK;
}
#endif

