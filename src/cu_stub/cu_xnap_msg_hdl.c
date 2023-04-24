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
#include "F1AP-PDU.h"
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

/*******************************************************************
 *
 * @brief Build And send dummy Handover request to Peer CU
 *
 * @details
 *
 *    Function : BuildAndSendHOReq
 *
 *    Functionality:
 *       Build And send dummy Handover request to Peer CU
 *
 * @params[in] Pointer to UE Cb
 *             Event type
 *             Message to be sent
 *             Message Length
 * @return void
 *
 ******************************************************************/
void BuildAndSendHOReq(CuUeCb *ueCb, char *xnMsg, MsgLen xnMsgLen)
{
   Buffer *mBuf = NULLP;

   if(ODU_GET_MSG_BUF(1, 1, &mBuf) == ROK)
   {
      if(ODU_ADD_POST_MSG_MULT((Data *)xnMsg, xnMsgLen, mBuf) == ROK)
      {  
         CMCHKPK(oduUnpackUInt8, ueCb->hoInfo.cuUeF1apIdSrc, mBuf);
         CMCHKPK(oduUnpackUInt32, ueCb->hoInfo.tgtCellId, mBuf);
         CMCHKPK(oduUnpackUInt8, HO_REQ, mBuf);
         if(sctpSend(XN_INTERFACE, ueCb->hoInfo.tgtNodeId, mBuf) != ROK)
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
 * @brief Process received Handover Request
 *
 * @details
 *
 *    Function : XNAPProcHandoverReq
 *
 *    Functionality:
 *       1. Unpack Cell Id from msg and find DU to which it belongs
 *       2. Create UE context for UE in handover and store in DU DB
 *       3. Decode the F1AP UE context modification msg received in 
 *          XNAP message buffer and extract UE configurations
 *       4. Send UE context setup request to DU with these configs 
 *
 * @params[in] Pointer to destination Id
 *             Pointer to message buffer
 * @return void
 *
 ******************************************************************/
void XNAPProcHandoverReq(uint32_t destId, Buffer *mBuf)
{
   uint8_t duIdx, cellIdx;
   uint8_t cuUeF1apIdSrc;
   uint32_t cellId;
   DuDb *duDb;
   CuCellCb *cellCb;

   DU_LOG("\nINFO  -->  CU STUB : Received Handover Request");
   
   /* Find DU Db and Cell Cb from cellId */
   CMCHKUNPK(oduPackUInt32, &(cellId), mBuf);
   for(duIdx = 0; duIdx < cuCb.numDu; duIdx++)
   {
      duDb = &cuCb.duInfo[duIdx];
      SEARCH_CELL_DB(cellIdx, duDb, cellId, cellCb)
      if(cellCb)
         break;
   }
   if(!cellCb)
   {
      DU_LOG("\nERROR  -->  CU_STUB: Failed to find Cell Id [%d] received in HO Request", cellId);
      return;
   }

   /* Fetch CU UE F1AP ID of UE in handover assigned by source CU */
   CMCHKUNPK(oduPackUInt8, &(cuUeF1apIdSrc), mBuf);

   /* Filling temporary UE context which will be used to create actual UE context at
    * CU later */
   CU_ALLOC(duDb->tempUeCtxtInHo, sizeof(CuUeCb));
   if(!duDb->tempUeCtxtInHo)
   {
      DU_LOG("\nERROR  -->  XNAP : Failed to allocate memory to temporary UE context for UE in handover");
      return;
   }
   memset(duDb->tempUeCtxtInHo, 0, sizeof(CuUeCb));
   duDb->tempUeCtxtInHo->cellCb = cellCb;
   duDb->tempUeCtxtInHo->gnbCuUeF1apId = ++cuCb.gnbCuUeF1apIdGenerator;
   duDb->tempUeCtxtInHo->state = UE_HANDOVER_IN_PROGRESS;
   duDb->tempUeCtxtInHo->hoInfo.HOType = Xn_Based_Inter_CU_HO;
   duDb->tempUeCtxtInHo->hoInfo.srcNodeId = destId;
   duDb->tempUeCtxtInHo->hoInfo.tgtNodeId = cuCb.cuCfgParams.cuId;
   duDb->tempUeCtxtInHo->hoInfo.tgtCellId = cellId;
   duDb->tempUeCtxtInHo->hoInfo.cuUeF1apIdSrc = cuUeF1apIdSrc;
   duDb->tempUeCtxtInHo->hoInfo.cuUeF1apIdTgt = duDb->tempUeCtxtInHo->gnbCuUeF1apId;

   /* Decode UE context modification response msg received in Xn Msg */
   char *recvBuf;
   MsgLen recvBufLen, copyLen;
   F1AP_PDU_t *f1apMsg = NULLP;
   F1AP_PDU_t f1apasnmsg ;

   f1apMsg = &f1apasnmsg;
   memset(f1apMsg, 0, sizeof(F1AP_PDU_t));
   if(F1APDecodeMsg(f1apMsg, mBuf, &recvBuf, &recvBufLen) != ROK)
   {
      DU_LOG("\nERROR  -->  F1AP : F1AP PDU decode failed");
      return;
   }
   CU_FREE(recvBuf, recvBufLen);
   procUeContextModificationResponse(duDb->duId, f1apMsg, NULL, 0);
}

/*******************************************************************
 *
 * @brief Build And send dummy Handover request ack to Peer CU
 *
 * @details
 *
 *    Function : BuildAndSendHOReqAck
 *
 *    Functionality:
 *       Build And send dummy Handover request ack to Peer CU
 *
 * @params[in] Pointer to UE Cb
 *             Message to be sent
 *             Message Length
 * @return void
 *
 ******************************************************************/
void BuildAndSendHOReqAck(CuUeCb *ueCb, char *xnMsg, MsgLen xnMsgLen)
{
   Buffer *mBuf = NULLP;

   if(ODU_GET_MSG_BUF(1, 1, &mBuf) == ROK)
   {   
      if(ODU_ADD_POST_MSG_MULT((Data *)xnMsg, xnMsgLen, mBuf) == ROK)
      {   
         CMCHKPK(oduUnpackUInt8, ueCb->gnbCuUeF1apId, mBuf);
         CMCHKPK(oduUnpackUInt8, ueCb->hoInfo.cuUeF1apIdSrc, mBuf);
         CMCHKPK(oduUnpackUInt8, HO_REQ_ACK, mBuf);
         if(sctpSend(XN_INTERFACE, ueCb->hoInfo.srcNodeId, mBuf) != ROK)
         {
            DU_LOG("\nERROR  -->  CU_STUB: Failed to send handover request ack to peer CU");
         }
      }
   }
}

/*******************************************************************
 *
 * @brief Process received Handover Request Ack
 *
 * @details
 *
 *    Function : XNAPProcHandoverReqAck
 *
 *    Functionality:
 *       1. Unpack CU UE F1AP ID (by SCU) and search for UE CB and
 *          the corresponding DU DB (SDU)
 *       2. Unpack CU UE F1AP ID (by TCU) and fill in UEcb->hoInfo
 *       3. Decode the F1AP UE context setup response msg received in 
 *          XNAP message buffer and extract UE configurations
 *       4. Create RRC reconfig msg with these configurations
 *       5. Send RRC Reconfig msg and Transmission Action = DO NOT TRANSMIT
 *          in UE context modification request to S DU
 *
 * @params[in] Pointer to destination Id
 *             Pointer to message buffer
 * @return void
 *
 ******************************************************************/
void XNAPProcHandoverReqAck(uint32_t destId, Buffer *mBuf)
{
   DU_LOG("\nINFO  -->  CU STUB : Received Handover Request Acknowledgement");
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
            XNAPProcHandoverReq(*destId, mBuf);
            break;
         }
      
      case HO_REQ_ACK:
         {
            XNAPProcHandoverReqAck(*destId, mBuf);
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
