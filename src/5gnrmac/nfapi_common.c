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
 *    It Fills NFAPI P5 Msg Header[as per Table 2-3 " nFapi Header"]
 *
 *
 * @params 
 *         [OUT]: Msg Buffer to send in SCTP P5 Interface
 *
 * *******************************************************************************/

void nfapiFillP5Hdr(Buffer *mBuf)
{
   uint8_t moreSeqNum = 0;
   moreSeqNum = NFAPI_MORE_SEG_NUM(NFAPI_P5_MORE, NFAPI_P5_SEG_NUM);

   CMCHKPK(oduPackPostUInt16, NFAPI_P5_SEG_NUM, mBuf);
   CMCHKPK(oduPackPostUInt8, moreSeqNum, mBuf);
   CMCHKPK(oduPackPostUInt8, vnfDb.p5Info.seqNum, mBuf);
   CMCHKPK(oduPackPostUInt32, NFAPI_P5_TIMESTAMP, mBuf);

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
   DU_LOG("\nINFo   --> NFAPI_VNF: seqLen:%d, moreSegNum:%d, seqNum:%d, timeStamp:%d",
        p5Hdr->seg_len,p5Hdr->more_segNum,p5Hdr->seq_num,p5Hdr->timeStamp);
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

    DU_LOG("\nINFO  -->  NFAPI_VNF: RUType:%d, phy_id:%d, msgId:%d, len:%d",\
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
 *         [OUT]: NfapiPnfEvent
 *
 * *******************************************************************************/
NfapiPnfEvent convertNfapiP5TagValToMsgId(uint16_t tagVal)
{

   switch(tagVal)
   {
      case TAG_NFAPI_PNF_READY_IND:
        {
           return PNF_READY_IND;
        }
      case TAG_NFAPI_PNF_PARAM_REQ:
        {
           return PNF_PARAM_REQ;
        }
      case TAG_NFAPI_PNF_PARAM_RESP:
        {
           return PNF_PARAM_RESP;
        }
      case TAG_NFAPI_PNF_CONFIG_REQ:
        {
           return PNF_CONFIG_REQ;
        }
      case TAG_NFAPI_PNF_CONFIG_RESP:
        {
           return PNF_CONFIG_RESP;
        }
      case TAG_NFAPI_PNF_START_REQ:
        {
           return PNF_START_REQ;
        }
      case TAG_NFAPI_PNF_START_RESP:
        {
           return PNF_START_RESP;
        }
      case TAG_NFAPI_PNF_STOP_REQ:
        {
           return PNF_STOP_REQ;
        }
      case TAG_NFAPI_PNF_STOP_RESP:
        {
           return PNF_STOP_RESP;
        }
      case FAPI_PARAM_REQUEST:
        {
           return PARAM_REQ;
        }
      case FAPI_PARAM_RESPONSE: 
        {
           return PARAM_RESP;
        }
      case FAPI_CONFIG_REQUEST:
        {
           return CONFIG_REQ;
        }
      case FAPI_CONFIG_RESPONSE: 
        {
           return CONFIG_RESP;
        }
      case FAPI_START_REQUEST:
        {
           return START_REQ;
        }
      case FAPI_STOP_REQUEST:
        {
           return STOP_REQ;
        }
      case FAPI_STOP_INDICATION: 
        {
           return STOP_IND;
        }
      case FAPI_ERROR_INDICATION: 
        {
           return ERROR_IND;
        }
      default:
        {
           DU_LOG("\n Incorrect TAG VALUE of NFAPI P5 Messages:%d",tagVal);
           return PNF_MAX_EVENT;
        }
   }
   return PNF_MAX_EVENT;
}
