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

/* This file contains F1AP message handler functions */
#include "common_def.h"
#include "cu_stub_sctp.h"
#include "cu_stub_egtp.h"
#include "OCTET_STRING.h"
#include "cu_f1ap_msg_hdl.h"
#include "cu_stub.h"

/*******************************************************************
 *
 * @brief Build And send dummy Xn Setup request
 *
 * @details
 *
 *    Function : BuildAndSendXnSetupReq
 *
 *    Functionality:
 *       Build And send dummy Xn Setup request
 *
 * @params[in]
 * @return void
 *
 ******************************************************************/
void BuildAndSendXnSetupReq()
{
   uint8_t assocIdx = 0;
   Buffer *mBuf = NULLP;
   CuSctpAssocCb *assocCb = NULLP;

   for(assocIdx = 0; assocIdx < sctpCb.numAssoc; assocIdx++)
   {
      assocCb = &sctpCb.assocCb[assocIdx];
      if(assocCb->connUp && assocCb->intf == XN_INTERFACE && assocCb->destId == 0)
      {
         if(ODU_GET_MSG_BUF(1, 1, &mBuf) == ROK)
         {
            CMCHKPK(oduUnpackUInt8,(Data *)CU_ID, mBuf);
            CMCHKPK(oduUnpackUInt8,XN_SETUP_REQ, mBuf);
            ODU_PRINT_MSG(mBuf, 0,0);
            if(sendOnSctpAssoc(assocCb, mBuf) != ROK)
            {
               DU_LOG("\nERROR  -->  CU_STUB: Failed to send XN setup request to peer CU");
            }
         }
         break;
      }
   }
}

/*******************************************************************
 *
 * @brief Process received Xn Setup request
 *
 * @details
 *
 *    Function : XNAPProcXnSetupReq
 *
 *    Functionality:
 *       Fetch peer CU ID and store in assocCb
 *
 * @params[in] Pointer to destination Id
 *             Pointer to message buffer
 * @return void
 *
 ******************************************************************/
void XNAPProcXnSetupReq(uint32_t *destId, Buffer *mBuf)
{
   uint8_t cuId;

   DU_LOG("\nINFO  -->  CU STUB : Received XN Setup Request");
   CMCHKUNPK(oduPackUInt8, &(cuId), mBuf)
   *destId = cuId;

   BuildAndSendXnSetupRsp(*destId);
}

/*******************************************************************
 *
 * @brief Build And send dummy Xn Setup response
 *
 * @details
 *
 *    Function : BuildAndSendXnSetupRsp
 *
 *    Functionality:
 *       Build And send dummy Xn Setup response
 *
 * @params[in]
 * @return void
 *
 ******************************************************************/
void BuildAndSendXnSetupRsp(uint32_t destId)
{
   Buffer *mBuf = NULLP;

   if(ODU_GET_MSG_BUF(1, 1, &mBuf) == ROK)
   {
      CMCHKPK(oduUnpackUInt8, CU_ID, mBuf);
      CMCHKPK(oduUnpackUInt8, XN_SETUP_RSP, mBuf);
      if(sctpSend(XN_INTERFACE, destId, mBuf))
      {
         DU_LOG("\nERROR  -->  CU_STUB: Failed to send XN setup response to peer CU");
      }   
   }   
}

/*******************************************************************
 *
 * @brief Process received Xn Setup response
 *
 * @details
 *
 *    Function : XNAPProcXnSetupRsp
 *
 *    Functionality:
 *       Fetch peer CU ID and store in assocCb
 *
 * @params[in] Pointer to destination Id
 *             Pointer to message buffer
 * @return void
 *
 ******************************************************************/
void XNAPProcXnSetupRsp(uint32_t *destId, Buffer *mBuf)
{
   uint8_t cuId;

   DU_LOG("\nINFO  -->  CU STUB : Received XN Setup Response");
   CMCHKUNPK(oduPackUInt8, &(cuId), mBuf)
   *destId = cuId;
}

void BuildAndSendHOReq(CuUeCb *ueCb, XnEventType event, char *xnMsg, MsgLen xnMsgLen)
{
   Buffer *mBuf = NULLP;

   if(ODU_GET_MSG_BUF(1, 1, &mBuf) == ROK)
   {
      if(ODU_ADD_POST_MSG_MULT((Data *)xnMsg, xnMsgLen, mBuf) == ROK)
      {     
         CMCHKPK(oduUnpackUInt8, event, mBuf);
         if(sctpSend(XN_INTERFACE, ueCb->hoInfo.targetId, mBuf) != ROK)
         {
            DU_LOG("\nERROR  -->  CU_STUB: Failed to send handover request to peer CU");
            ueCb->state = UE_ACTIVE;
            memset(&ueCb->hoInfo, 0, sizeof(HandoverInfo));
         }
      }     
   }
}

/*******************************************************************
 *
 * @brief Handle incoming messages at Xn interface
 *
 * @details
 *
 *    Function : XNAPMsgHdlr
 *
 *    Functionality:
 *       Fetch event type from message buffer and call appropriate
 *       handler
 *
 * @params[in] Pointer to destination Id
 *             Pointer to message buffer
 * @return void
 *
 ******************************************************************/
void XNAPMsgHdlr(uint32_t *destId, Buffer *mBuf)
{
   XnEventType event;

   CMCHKUNPK(oduPackUInt8, &event, mBuf);

   switch(event)
   {
      case XN_SETUP_REQ:
         {
             XNAPProcXnSetupReq(destId, mBuf);
             break;
         }

    case XN_SETUP_RSP:
         {
             XNAPProcXnSetupRsp(destId, mBuf);
             break;
         }

      case HO_REQ:
         {
            DU_LOG("\nINFO  -->  CU STUB : Received Handover Request at XNAP");

//Handling of HO Request to be added in next gerrit
#if 0
            char *recvBuf;
            MsgLen copyCnt;
            MsgLen recvBufLen;
            F1AP_PDU_t *f1apMsg = NULLP;
            asn_dec_rval_t rval; /* Decoder return value */
            F1AP_PDU_t f1apasnmsg ;

            ODU_PRINT_MSG(mBuf, 0,0); 

            /* Copy mBuf into char array to decode it */
            ODU_GET_MSG_LEN(mBuf, &recvBufLen);
            CU_ALLOC(recvBuf, (Size)recvBufLen);

            if(recvBuf == NULLP)
            {
               DU_LOG("\nERROR  -->  F1AP : Memory allocation failed");
               return;
            }
            if(ODU_COPY_MSG_TO_FIX_BUF(mBuf, 0, recvBufLen, (Data *)recvBuf, &copyCnt) != ROK)
            {
               DU_LOG("\nERROR  -->  F1AP : Failed while copying %d", copyCnt);
               return;
            }

            DU_LOG("\nDEBUG  -->  F1AP : Received flat buffer to be decoded : ");
            for(i=0; i< recvBufLen; i++)
            {
               DU_LOG("%x",recvBuf[i]);
            }

            /* Decoding flat buffer into F1AP messsage */
            f1apMsg = &f1apasnmsg;
            memset(f1apMsg, 0, sizeof(F1AP_PDU_t));
            rval = aper_decode(0, &asn_DEF_F1AP_PDU, (void **)&f1apMsg, recvBuf, recvBufLen, 0, 0);
            CU_FREE(recvBuf, (Size)recvBufLen);

            if(rval.code == RC_FAIL || rval.code == RC_WMORE)
            {
               DU_LOG("\nERROR  -->  F1AP : ASN decode failed");
               return;
            }
            DU_LOG("\n");
            xer_fprint(stdout, &asn_DEF_F1AP_PDU, f1apMsg);

            procUeContextModificationResponse(0, f1apMsg, recvBuf, recvBufLen);      
#endif            
            break;
         }
      default:
         DU_LOG("\nERROR  --> CU_STUB : Invalid event [%d] received at XN interface", event);
         break;
   }
}

/**********************************************************************
  End of file
 **********************************************************************/
