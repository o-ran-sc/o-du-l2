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
 
/* This file contains all EGTP related functionality */
#include "common_def.h"
#include "lrg.h"
#include "lkw.x"
#include "lrg.x"
#include "cm_inet.h"
#include "legtp.h"
#include "cm_inet.x"
#include "du_app_mac_inf.h"
#include "du_cfg.h"
#include "du_egtp.h"
#include "du_utils.h"


/**************************************************************************
 * @brief Task Initiation callback function. 
 *
 * @details
 *
 *     Function : egtpActvInit 
 *    
 *     Functionality:
 *             This function is supplied as one of parameters during EGTP's 
 *             task registration. SSI will invoke this function once, after
 *             it creates and attaches this TAPA Task to a system task.
 *     
 * @param[in]  Ent entity, the entity ID of this task.     
 * @param[in]  Inst inst, the instance ID of this task.
 * @param[in]  Region region, the region ID registered for memory 
 *              usage of this task.
 * @param[in]  Reason reason.
 * @return ROK     - success
 *         RFAILED - failure
 ***************************************************************************/
uint8_t egtpActvInit(Ent entity, Inst inst, Region region, Reason reason)
{
  DU_LOG("\n\nDEBUG   -->  EGTP : Initializing");

  memset (&egtpCb, 0, sizeof(EgtpGlobalCb));
  protType = CM_INET_PROTO_UDP;
  gDlDataRcvdCnt = 0;

  return ROK;
}

/**************************************************************************
* @brief Function prints the src dest and msg reached to egtp.
*
* @details
*
*      Function : callFlowEgtpActvTsk 
*
*      Functionality:
*           Function prints the src dest and msg reached to egtp.
*
* @param[in]  Pst     *pst, Post structure of the primitive.
*
* @return void 
*
***************************************************************************/

void callFlowEgtpActvTsk(Pst *pst)
{

   char sourceTask[50];
   char destTask[50]="ENTEGTP";
   char message[100];

   switch(pst->srcEnt)
   {
      case ENTDUAPP:
         {
            strcpy(sourceTask,"ENTDUAPP");
            switch(pst->event)
            {
               case EVTCFGREQ:
                  {
                     strcpy(message,"EVTCFGREQ");
                     break;
                  }
               case EVTSRVOPENREQ:
                  {
                     strcpy(message,"EVTSRVOPENREQ");
                     break;
                  }
               case EVTTNLMGMTREQ:
                  {
                     strcpy(message,"EVTTNLMGMTREQ");
                     break;
                  }
               default:
                  {
                     strcpy(message,"Invalid Event");
                     break;
                  }
            }
            break;
         }
      case ENTEGTP:
         {
            strcpy(sourceTask,"ENTEGTP");
            switch(pst->event)
            {
               case EVTSTARTPOLL:
                  {
                     strcpy(message,"EVTSTARTPOLL");
                     break;
                  }
               default:
                  {
                     strcpy(message,"Invalid Event");
                     break;
                  }
            }
            break;
         }
      case ENTRLC:
         {
            strcpy(sourceTask,"ENTRLC");
            switch(pst->event)
            {
               case EVTDATIND:
                  {
                     strcpy(message,"EVTDATIND");
                     break;
                  }
               default:
                  {
                     strcpy(message,"Invalid Event");
                     break;
                  }
            }
            break;
         }
      default:
         {
             strcpy(sourceTask,"Invalid Source Entity Id");
         }
   }
   DU_LOG("\nCall Flow: %s -> %s : %s\n", sourceTask, destTask, message);
}

/**************************************************************************
 * @brief Task Activation callback function. 
 *
 * @details
 *
 *      Function : egtpActvTsk 
 * 
 *      Functionality:
 *           This function handles all EGTP messages received
 *           This API is registered with SSI during the 
 *           Task Registration of DU APP.
 *     
 * @param[in]  Pst     *pst, Post structure of the primitive.     
 * @param[in]  Buffer *mBuf, Packed primitive parameters in the
 *  buffer.
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t egtpActvTsk(Pst *pst, Buffer *mBuf)
{
   uint8_t ret = ROK;
  
#ifdef CALL_FLOW_DEBUG_LOG
   callFlowEgtpActvTsk(pst);
#endif

   switch(pst->srcEnt)
   {
      case ENTDUAPP:
      {
         switch(pst->event)
         {
            case EVTCFGREQ:
            {
               ret = unpackEgtpCfgReq(egtpCfgReq, pst, mBuf);
               break;
            }
            case EVTSRVOPENREQ:
            {
               ret = unpackEgtpSrvOpenReq(egtpSrvOpenReq, pst, mBuf);
               break;
            }
            case EVTTNLMGMTREQ:
            {
               ret = unpackEgtpTnlMgmtReq(egtpTnlMgmtReq, pst, mBuf);
               break;
            }
            default:
            {
               DU_LOG("\nERROR  -->  EGTP : Invalid event %d", pst->event);
               ODU_PUT_MSG_BUF(mBuf);
               ret = RFAILED;
            }
         }
         break;
      }
      case ENTEGTP:
      {
         switch(pst->event)
         {
            case EVTSTARTPOLL:
            {
               DU_LOG("\nDEBUG   -->  EGTP : Starting Socket Polling");
               egtpRecvMsg();
               ODU_PUT_MSG_BUF(mBuf);
               break;
            }
            default:
            {
               DU_LOG("\nERROR  -->  EGTP : Invalid event %d", pst->event);
               ODU_PUT_MSG_BUF(mBuf);
               ret = RFAILED;
            }
        }
        break;
      }
      case ENTRLC:
      {
         switch(pst->event)
         {
            case EVTDATIND:
            {
               //TODO
               break;
            }
            default:
            {
               DU_LOG("\nERROR  -->  EGTP : Invalid event %d", pst->event);
               ret = RFAILED;
            }
         }
         break;
      }
      default:
      {
         DU_LOG("\nERROR  -->  EGTP : Invalid source entity %d", pst->srcEnt);
         ret = RFAILED;
      }
   }
   ODU_EXIT_TASK();
   return ret;
}

/**************************************************************************
 * @brief EGTP server configuration 
 *
 * @details
 *
 *      Function : egtpCfgReq
 * 
 *      Functionality:
 *           This function handles EGTP configuration request.
 *     
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ***********************************************************************/
uint8_t egtpCfgReq(Pst *pst, EgtpConfig egtpCfg)
{
   uint8_t  ret;          /* Return value */
   Pst      rspPst;      /* Response Pst structure */
   CmStatus cfgCfm; /* Configuration Confirm */

   memcpy(&egtpCb.egtpCfg, &egtpCfg, sizeof(EgtpConfig));

   egtpCb.recvTptSrvr.addr.address = CM_INET_NTOH_UINT32(egtpCb.egtpCfg.localIp.ipV4Addr);
   egtpCb.recvTptSrvr.addr.port = EGTP_DFLT_PORT;

   egtpCb.dstCb.dstIp = CM_INET_NTOH_UINT32(egtpCb.egtpCfg.destIp.ipV4Addr);
   egtpCb.dstCb.dstPort = egtpCb.egtpCfg.destPort;
   egtpCb.dstCb.sendTptSrvr.addr.address = CM_INET_NTOH_UINT32(egtpCb.egtpCfg.localIp.ipV4Addr);
   egtpCb.dstCb.sendTptSrvr.addr.port = egtpCb.egtpCfg.localPort;
   egtpCb.dstCb.numTunn = 0;

   ret = cmHashListInit(&(egtpCb.dstCb.teIdLst), 1024, sizeof(EgtpTeIdCb), FALSE, CM_HASH_KEYTYPE_UINT32_MOD, DU_APP_MEM_REGION, DU_POOL);

   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  EGTP : TeId hash list initialization failed");
      cfgCfm.status = LCM_PRIM_NOK;
      cfgCfm.reason = LCM_REASON_HASHING_FAILED;
   }
   else
   {
      DU_LOG("\nDEBUG   -->  EGTP : EGTP configuration successful");
      cfgCfm.status = LCM_PRIM_OK;
      cfgCfm.reason = LCM_REASON_NOT_APPL;
   }

   /* Fill response Pst */
   egtpFillRspPst(pst, &rspPst);
   rspPst.event = EVTCFGCFM;

   packEgtpCfgCfm(&rspPst, cfgCfm);

   return ROK;
}

/**************************************************************************
 * @brief Fills post structure to send response
 *
 * @details
 *
 *      Function : egtpFillRspPst
 * 
 *      Functionality:
 *           Fills post struture to send response
 *     
 * @return ROK     - success
 *         RFAILED - failure
 *
 *
 * ***********************************************************************/
uint8_t egtpFillRspPst(Pst *pst, Pst *rspPst)
{

   memset(rspPst, 0, sizeof(Pst));
   rspPst->srcEnt = pst->dstEnt;
   rspPst->srcInst = pst->dstInst;
   rspPst->srcProcId = pst->dstProcId;
   rspPst->dstEnt = pst->srcEnt;
   rspPst->dstInst = pst->srcInst;
   rspPst->dstProcId = pst->srcProcId;
   rspPst->selector = ODU_SELECTOR_LC;
   rspPst->pool= DU_POOL;
    
   return ROK;
}

/**************************************************************************
 * @brief EGTP server open request 
 *
 * @details
 *
 *      Function : egtpSrvOpenReq
 * 
 *      Functionality:
 *           This function handles EGTP open server request.
 *           It opens udp socket to receive/send msgs.
 *     
 * @param[in]  Pst *pst, post structure
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/

uint8_t egtpSrvOpenReq(Pst *pst)
{

   uint8_t  ret;       /* Return value */
   Pst      rspPst;    /* Response Pst structure */ 
   Pst      egtpPst;   /* Self post */
   CmStatus cfm;       /* Confirmation status */
   uint8_t  sockType;  /* Socket type */

   DU_LOG("\nDEBUG  -->  EGTP : Received EGTP open server request");
 
   sockType = CM_INET_DGRAM;
   ret = egtpSrvOpenPrc(sockType, &(egtpCb.recvTptSrvr));
	/* Opening and Binding receiver socket */
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  EGTP : Failed while opening receiver transport server");
      return ret;
   }
   /* Opening and Binding sender socket */
	ret = egtpSrvOpenPrc(sockType, &(egtpCb.dstCb.sendTptSrvr));
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  EGTP : Failed while opening sender transport server");
      return ret;
   }

   DU_LOG("\nDEBUG   -->  EGTP : Receiver socket[%d] and Sender socket[%d] open", egtpCb.recvTptSrvr.sockFd.fd,\
   egtpCb.dstCb.sendTptSrvr.sockFd.fd);

   /* Start Socket polling */
   memset(&egtpPst, 0, sizeof(egtpPst));
   egtpPst.srcEnt = (Ent)ENTEGTP;
   egtpPst.srcInst = (Inst)EGTP_INST;
   egtpPst.srcProcId = DU_PROC;
   egtpPst.dstEnt = (Ent)ENTEGTP;
   egtpPst.dstInst = (Inst)EGTP_INST;
   egtpPst.dstProcId = DU_PROC;
   egtpPst.event = EVTSTARTPOLL;
   egtpPst.selector = ODU_SELECTOR_LC;
   egtpPst.pool= DU_POOL;
   packEgtpStartPollingReq(&egtpPst);

   /* Filling and sending response */
   cfm.status = LCM_PRIM_OK;
   cfm.reason = LCM_REASON_NOT_APPL;

   egtpFillRspPst(pst, &rspPst);
   rspPst.event = EVTSRVOPENCFM;
   packEgtpSrvOpenCfm(&rspPst, cfm);

   return ret;
}

/*******************************************************************
 *
 * @brief Processing Sever open request
 *
 * @details
 *
 *    Function : egtpSrvOpenPrc
 *
 *    Functionality:
 *      
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t egtpSrvOpenPrc(uint8_t sockType, EgtpTptSrvr *server)
{
   S8 ret=ROK;
   ret = cmInetSocket(sockType, &(server->sockFd), protType); 
	if(ret != ROK)
   {  
      DU_LOG("\nERROR  -->  EGTP : Failed to open UDP socket");
      return ret;
   }
   ret = cmInetBind(&(server->sockFd), &(server->addr));  
   if(ret != ROK)
   {  
      DU_LOG("\nERROR  -->  EGTP : Failed to bind socket");
      return ret;
   }
   
   return ret;
}

/**************************************************************************
 * @brief EGTP tunnel management request
 *
 * @details
 *
 *      Function : egtpTnlMgmtReq
 * 
 *      Functionality:
 *           This function handles EGTP tunnel managament request
 *     
 * @param[in] Tunnel Eveny structure
 * @return ROK     - success
 *         RFAILED - failure
 *
 
 * ***************************************************************************/
uint8_t egtpTnlMgmtReq(Pst *pst, EgtpTnlEvt tnlEvt)
{
   uint8_t ret = ROK;

#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTDUAPP -> ENTEGTP : TNL_MGMT\n");
#endif

   DU_LOG("\nDEBUG   -->  EGTP : Received tunnel management request");
   switch(tnlEvt.action)
   {
      case EGTP_TNL_MGMT_ADD:
      {
         ret = egtpTnlAdd(tnlEvt);
         break;
      }
      case EGTP_TNL_MGMT_MOD:
      {
         ret = egtpTnlMod(tnlEvt);
         break;
      }
      case EGTP_TNL_MGMT_DEL:
      {
         ret = egtpTnlDel(tnlEvt);
         break;
      }
      default:
      {
         DU_LOG("\nERROR  -->  EGTP : Invalid tunnel management action[%d]", tnlEvt.action);
         ret = LCM_REASON_INVALID_ACTION;
      }
   }

   if(ret == ROK)
   {
      tnlEvt.cfmStatus.status = LCM_PRIM_OK;
      tnlEvt.cfmStatus.reason = LCM_REASON_NOT_APPL;
   }
   else
   {
      tnlEvt.cfmStatus.status = LCM_PRIM_NOK;
      tnlEvt.cfmStatus.reason = ret;
   }

   DU_LOG("\nDEBUG   -->  EGTP : Sending Tunnel management confirmation");
   duHdlEgtpTnlMgmtCfm(tnlEvt);

   return ret;
}

/**************************************************************************
 * @brief EGTP tunnel addition
 *
 * @details
 *
 *      Function : egtpTnlAdd
 * 
 *      Functionality:
 *           This function handles EGTP tunnel addition
 *     
 * @param[in]  Tunnel Event structure
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ***************************************************************************/
uint8_t egtpTnlAdd(EgtpTnlEvt tnlEvt)
{
   uint8_t    ret;
   EgtpTeIdCb *teidCb;    /* Tunnel endpoint control block */
   EgtpMsgHdr preDefHdr; /* pre-define header for this tunnel */

   DU_LOG("\nINFO   -->  EGTP : Tunnel addition : LocalTeid[%d] Remote Teid[%d]", tnlEvt.lclTeid, tnlEvt.remTeid);

   DU_ALLOC(teidCb, sizeof(EgtpTeIdCb));
   if(teidCb == NULLP)
   {
      DU_LOG("\nERROR  -->  EGTP : Memory allocation failed");
      return LCM_REASON_MEM_NOAVAIL;
   }

   memset(teidCb, 0, sizeof(EgtpTeIdCb));
   teidCb->teId = tnlEvt.lclTeid;
   teidCb->remTeId = tnlEvt.remTeid;

   ret = cmHashListInsert(&(egtpCb.dstCb.teIdLst), (PTR)teidCb, (uint8_t *)&(teidCb->teId), sizeof(uint32_t));
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  EGTP : Failed to insert in hash list");
      DU_FREE(teidCb, sizeof(EgtpTeIdCb));
      return LCM_REASON_HASHING_FAILED;
   }
   egtpCb.dstCb.numTunn++;

   /* Encoding pre-defined header */
   memset(&preDefHdr, 0, sizeof(EgtpMsgHdr));
   preDefHdr.msgType = EGTPU_MSG_GPDU;
   preDefHdr.teId = teidCb->remTeId;
   preDefHdr.extHdr.pdcpNmb.pres = FALSE;
   preDefHdr.extHdr.udpPort.pres = FALSE;
   preDefHdr.nPdu.pres = FALSE;
  
   egtpEncodeHdr((uint8_t *)teidCb->preEncodedHdr.hdr, &preDefHdr, &(teidCb->preEncodedHdr.cnt));

   return ROK;
} /* egtpTnlAdd */

/**************************************************************************
 * @brief EGTP tunnel modification
 *
 * @details
 *
 *      Function : egtpTnlMod
 * 
 *      Functionality:
 *           This function handles EGTP tunnel modification
 *     
 * @param[in]  Tunnel Event structure
 * @return ROK     - success
 *         RFAILED - failure
 * 
 * ***************************************************************************/
uint8_t egtpTnlMod(EgtpTnlEvt tnlEvt)
{
   EgtpTeIdCb     *teidCb = NULLP;

   DU_LOG("\nINFO   -->  EGTP : Tunnel modification : LocalTeid[%d] Remote Teid[%d]", tnlEvt.lclTeid, tnlEvt.remTeid);

   cmHashListFind(&(egtpCb.dstCb.teIdLst), (uint8_t *)&(tnlEvt.lclTeid), sizeof(uint32_t), 0, (PTR *)&teidCb);
   if(teidCb == NULLP)
   {
      DU_LOG("\nERROR  -->  EGTP : Tunnel id not found");
      return RFAILED;
   }  
   teidCb->teId = tnlEvt.remTeid;
   teidCb->remTeId = tnlEvt.remTeid;
   return ROK;
}

/**************************************************************************
 * @brief EGTP tunnel deletion
 *
 * @details
 *
 *     Function : egtpTnlDel
 * 
 *     Functionality:
 *         This function handles EGTP tunnel deletion
 *    
 * @param[in]  Tunnel Event structure
 * @return ROK     - success
 *         RFAILED - failure
 * 
 * ***************************************************************************/
uint8_t egtpTnlDel(EgtpTnlEvt tnlEvt)
{
   EgtpTeIdCb     *teidCb = NULLP;

   DU_LOG("\nINFO   -->  EGTP : Tunnel deletion : Local Teid[%d] Remote Teid[%d]", tnlEvt.lclTeid, tnlEvt.remTeid);
   
   cmHashListFind(&(egtpCb.dstCb.teIdLst), (uint8_t *)&(tnlEvt.lclTeid), sizeof(uint32_t), 0, (PTR *)&teidCb);
   if(teidCb == NULLP)
   {
      DU_LOG("\nERROR  -->  EGTP : Tunnel id[%d] not configured", tnlEvt.lclTeid);
      return LCM_REASON_INVALID_PAR_VAL;
   } 

   cmHashListDelete(&(egtpCb.dstCb.teIdLst), (PTR)teidCb);
   DU_FREE(teidCb, sizeof(EgtpTeIdCb));
   egtpCb.dstCb.numTunn--;
   return ROK;
}

/*******************************************************************
 *
 * @brief Handles data indication
 *
 * @details
 *
 *    Function : EgtpHdlDatInd
 *
 *    Functionality:
 *      Handles incoming data from peer to be passed 
 *      on from DU to CU
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t egtpHdlDatInd(EgtpMsg egtpMsg)
{
   EgtpTeIdCb  *teidCb = NULLP;
   uint16_t    tPduSize;
   uint8_t     hdrLen;
   uint32_t    msgLen;
   EgtpMsgHdr  *msgHdr;

#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTDUAPP -> ENTEGTP : DATA_INDICATION\n");
#endif
   
   DU_LOG("\nDEBUG  -->  EGTP : Received Data Indication");

   cmHashListFind(&(egtpCb.dstCb.teIdLst), (uint8_t *)&(egtpMsg.msgHdr.teId), sizeof(uint32_t), 0, (PTR *)&teidCb);
   if(teidCb == NULLP)
   {
      DU_LOG("\nERROR  -->  EGTP : Tunnel id[%d] not configured", egtpMsg.msgHdr.teId);
      return LCM_REASON_INVALID_PAR_VAL;
   }
   
   msgHdr = &(egtpMsg.msgHdr);

   hdrLen = teidCb->preEncodedHdr.cnt;

   if(msgHdr->extHdr.pdcpNmb.pres)
   {
      teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 1] |= EGTP_MASK_BIT3;
      teidCb->preEncodedHdr.hdr[hdrLen] = EGTP_EXT_HDR_PDCP_TYPE;
      teidCb->preEncodedHdr.hdr[--hdrLen] = 1;
      teidCb->preEncodedHdr.hdr[--hdrLen] = GetHiByte(msgHdr->extHdr.pdcpNmb.val);
      teidCb->preEncodedHdr.hdr[--hdrLen] = GetLoByte(msgHdr->extHdr.pdcpNmb.val);
      teidCb->preEncodedHdr.hdr[--hdrLen] = 0;
   }
   else
   {
      teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 1] &= ~(EGTP_MASK_BIT3);
   }

   ODU_GET_MSG_LEN(egtpMsg.msg, (int16_t *)&tPduSize);

   /*Adjust the header to fill the correct length*/
   msgLen = tPduSize +  (EGTP_MAX_HDR_LEN - hdrLen) - 0x08;

   /***********************************************
    * Fill the length field of the message header *
    ***********************************************/
   teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 3] = (uint8_t)GetHiByte(msgLen);
   teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 4] = (uint8_t)GetLoByte(msgLen);

   /*Update the sequence number*/
   if(egtpMsg.msgHdr.seqNum.pres)
   {
      teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 1] |= (EGTP_MASK_BIT2);
      teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 9] = (uint8_t)GetHiByte(egtpMsg.msgHdr.seqNum.val);
      teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 10] = (uint8_t)GetLoByte(egtpMsg.msgHdr.seqNum.val);
   }
   else
   {
      teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 1] &= ~(EGTP_MASK_BIT2);
   }

   DU_LOG("\nDEBUG  -->  EGTP : UL Data buffer before encoding header");
   ODU_PRINT_MSG(egtpMsg.msg, 0, 0);

   ODU_ADD_PRE_MSG_MULT(&teidCb->preEncodedHdr.hdr[hdrLen], (EGTP_MAX_HDR_LEN - hdrLen), egtpMsg.msg);


   DU_LOG("\nDEBUG  -->  EGTP : UL Data buffer after encoding header");
   ODU_PRINT_MSG(egtpMsg.msg, 0, 0);

   /* Send over UDP */
   egtpSendMsg(egtpMsg.msg);
   ODU_PUT_MSG_BUF(egtpMsg.msg);

   return ROK;
}/* EgtpHdlDatInd */

/*******************************************************************
 *
 * @brief Encodes outgoing message
 *
 * @details
 *
 *    Function : egtpEncodeMsg
 *
 *    Functionality:
 *       Encodes EGTP message to be sent
 *
 * @params[in] EGTP message
 *             Message Buffer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t egtpEncodeHdr(uint8_t *preEncodedHdr, EgtpMsgHdr *preDefHdr, uint8_t *hdrIdx)
{
   uint8_t    tmpByte = 0;                 /* Stores one byte of data for enc */
   uint8_t    cnt     = EGTP_MAX_HDR_LEN;  /* Stores the position */
   bool       extPres = FALSE;             /* Flag for indication of S, E or P presense flag */
   uint16_t   nwWord = 0;
   
   /* Encoding header */
   tmpByte |= EGTP_MASK_BIT6;   /* Setting 6th LSB of 1st byte as version */
   tmpByte |= EGTP_MASK_BIT5;   /* Setting 5th LSB of 1st byte as protocol type */

   if(preDefHdr->extHdr.udpPort.pres || preDefHdr->extHdr.pdcpNmb.pres)
   {
      tmpByte |= EGTP_MASK_BIT3;  /* Setting 3rd LSB of 1st byte if Extension heaeder is present */
   }

   if(preDefHdr->seqNum.pres)
   {
      tmpByte |= EGTP_MASK_BIT2;
   }

   if(preDefHdr->nPdu.pres)
   {
      tmpByte |= EGTP_MASK_BIT1;
   }

   if((tmpByte & EGTP_MASK_BIT1) || (tmpByte & EGTP_MASK_BIT2)||(tmpByte & EGTP_MASK_BIT3))
   {
      extPres = TRUE;
   }

   preEncodedHdr[--cnt] = tmpByte;
   preEncodedHdr[--cnt] = preDefHdr->msgType;

   /* Encode Tunnel endpoint */
   preEncodedHdr[--cnt] = 0;
   preEncodedHdr[--cnt] = 0;
   nwWord = (uint16_t)(GetHiWord(preDefHdr->teId));
   preEncodedHdr[--cnt] = (uint8_t)(GetHiByte(nwWord));
   preEncodedHdr[--cnt] = (uint8_t)(GetLoByte(nwWord));
   nwWord = (uint16_t)(GetLoWord(preDefHdr->teId));
   preEncodedHdr[--cnt] = (uint8_t)(GetHiByte(nwWord));
   preEncodedHdr[--cnt] = (uint8_t)(GetLoByte(nwWord));

   /* Encode sequence number */
   if(preDefHdr->seqNum.pres)
   {
      preEncodedHdr[--cnt] = GetHiByte(preDefHdr->seqNum.val);
      preEncodedHdr[--cnt] = GetLoByte(preDefHdr->seqNum.val);
   }
   else if(extPres)
   {
      preEncodedHdr[--cnt] = 0;
      preEncodedHdr[--cnt] = 0;
   }

   /* Encode nPdu number */
   if(preDefHdr->nPdu.pres)
   {
      preEncodedHdr[--cnt] = preDefHdr->nPdu.val;
   }
   else if(extPres)
   {
      preEncodedHdr[--cnt] = 0;
   }

   if(preDefHdr->extHdr.udpPort.pres)
   {
      preEncodedHdr[--cnt] = EGTP_EXT_HDR_UDP_TYPE;
      preEncodedHdr[--cnt] = 1;
      preEncodedHdr[--cnt] = GetHiByte(preDefHdr->extHdr.udpPort.val);
      preEncodedHdr[--cnt] = GetLoByte(preDefHdr->extHdr.udpPort.val);
   }
 
   if(preDefHdr->extHdr.pdcpNmb.pres)
   {
      preEncodedHdr[--cnt] = EGTP_EXT_HDR_PDCP_TYPE;
      preEncodedHdr[--cnt] = 1;
      preEncodedHdr[--cnt] = GetHiByte(preDefHdr->extHdr.pdcpNmb.val);
      preEncodedHdr[--cnt] = GetLoByte(preDefHdr->extHdr.pdcpNmb.val);
   }
 
   if(tmpByte & EGTP_MASK_BIT3)
   {
      preEncodedHdr[--cnt] = 0;
   }
   else if(extPres)
   {
      preEncodedHdr[--cnt] = 0;
   }

   *hdrIdx = cnt;
   return ROK;
} /* egtpEncodeHdr */

/*******************************************************************
 *
 * @brief Sends message over UDP
 *
 * @details
 *
 *    Function : egtpSendMsg
 *
 *    Functionality:
 *           Sends message over UDP
 *
 * @params[in] Message Buffer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t egtpSendMsg(Buffer *mBuf)
{
   uint8_t        ret;
   uint16_t       txLen;
   CmInetMemInfo  info;
   CmInetAddr     dstAddr;
   static uint64_t numDataSent = 0;

   info.region = DU_APP_MEM_REGION;
   info.pool = DU_POOL;

   dstAddr.port = EGTP_DFLT_PORT;
   dstAddr.address = egtpCb.dstCb.dstIp;

   ret = cmInetSendMsg(&(egtpCb.dstCb.sendTptSrvr.sockFd), &dstAddr, &info, \
      mBuf, (int16_t *)&txLen, CM_INET_NO_FLAG);
   if(ret != ROK && ret != RWOULDBLOCK)
   {
      DU_LOG("\nERROR  -->  EGTP : Failed sending the message");
      return RFAILED;
   }
   else
   {
      DU_LOG("\nDEBUG -->  EGTP : Sent UL Message [%ld]", numDataSent+1);
      numDataSent++;
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Receives EGTP message from UDP socket 
 *
 * @details
 *
 *    Function : egtpRecvMsg
 *
 *    Functionality:
 *      Receive incoming messages from UDP socket
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t egtpRecvMsg()
{
   uint8_t        ret;           /* Return value */
   uint16_t       bufLen;        /* Length of received buffer */
   Buffer         *recvBuf;      /* Received buffer */
   CmInetAddr     fromAddr;      /* Egtp data sender address */
   CmInetMemInfo  memInfo;       /* Buffer allocation info */

   memInfo.region = DU_APP_MEM_REGION;
   memInfo.pool   = DU_POOL;
    
   fromAddr.port = egtpCb.dstCb.dstPort;
   fromAddr.address = egtpCb.dstCb.dstIp;

   while(true)
   {
      bufLen = -1;
      ret = cmInetRecvMsg(&(egtpCb.recvTptSrvr.sockFd), &fromAddr, &memInfo, \
         &recvBuf, (int16_t *)&bufLen, CM_INET_NO_FLAG);
      if(ret == ROK && recvBuf != NULLP)
      {  
         DU_LOG("\nDEBUG  -->  EGTP : Received DL Message[%ld]\n", gDlDataRcvdCnt + 1);
         //ODU_PRINT_MSG(recvBuf, 0 ,0);
         egtpHdlRecvData(recvBuf);
         gDlDataRcvdCnt++;
         
      }
   }
   
   return ROK;
}

/*******************************************************************
 *
 * @brief Handles DL User data received from CU
 *
 * @details
 *
 *    Function : egtpHdlRecvData
 *
 *    Functionality: Handles DL User data received from CU
 *
 * @params[in] DL Usre data buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t egtpHdlRecvData(Buffer *mBuf)
{
   EgtpMsg  egtpMsg;

   /* Decode EGTP header */
   egtpDecodeHdr(mBuf, &egtpMsg);

   /* TODO : Send received message to RLC */
   duHdlEgtpDlData(&egtpMsg);

   return ROK;
}

/*******************************************************************
 *
 * @brief Decodes EGTP header from DL User data
 *
 * @details
 *
 *    Function : egtpDecodeHdr
 *
 *    Functionality: Decodes EGTP header from DL User data
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t egtpDecodeHdr(Buffer *mBuf, EgtpMsg  *egtpMsg)
{
   uint8_t    tmpByte[5];         /* Holds 5 byte of data after Decoding */
   uint8_t    version = 0;         /* Holds the version type, decoded */
   uint16_t   msgLen  = 0;         /* Holds the msgLen from the Hdr */
   uint16_t   bufLen  = 0;         /* Holds the total buffer length */
   uint8_t    extHdrType = 0;       /* Holds the Extension hdr type */
   uint8_t    extHdrLen = 0;        /* Extension hdr length */
   bool       extPres = FALSE;      /* Flag for indication of S, E or P presense flag */
 
   ODU_GET_MSG_LEN(mBuf, (int16_t *)&bufLen);
 
   /* Decode first byte and storing in temporary variable */
   ODU_REM_PRE_MSG(&tmpByte[0], mBuf);

   /* Extracting version fro 1st byte */
   version = tmpByte[0] >> 5;
   
   //DU_LOG("\nDEBUG   -->  EGTP : Version %d", version);
 
   /* Decode message type */
   ODU_REM_PRE_MSG((Data*)&(egtpMsg->msgHdr.msgType), mBuf);
   //DU_LOG("\nDEBUG   -->  EGTP : msgType %d", egtpMsg->msgHdr.msgType);

   /****************************************************************************
    * Message length param is 2 bytes. So decode next 2 bytes from msg hdr and
    * performing OR operation on these two bytes to calculate message length 
    ***************************************************************************/
   ODU_REM_PRE_MSG(&tmpByte[1], mBuf);
   ODU_REM_PRE_MSG(&tmpByte[2], mBuf);
   msgLen = (tmpByte[1] << 8) | tmpByte[2];
   UNUSED(msgLen);
   UNUSED(version);
   //DU_LOG("\nDEBUG   -->  EGTP : msgLen %d", msgLen);


   /****************************************************************************
    * Tunnel id param is 4 bytes. So decode next 4 bytes from msg hdr and 
    * perform OR operation on these 4 bytes to calculate tunnel id
    ***************************************************************************/
   ODU_REM_PRE_MSG(&tmpByte[1], mBuf);
   ODU_REM_PRE_MSG(&tmpByte[2], mBuf);
   ODU_REM_PRE_MSG(&tmpByte[3], mBuf);
   ODU_REM_PRE_MSG(&tmpByte[4], mBuf);
   egtpMsg->msgHdr.teId = (tmpByte[1] << 24) | (tmpByte[2] << 16) | (tmpByte[3] << 8) | tmpByte[4];
   //DU_LOG("\nDEBUG   -->  EGTP : teId %d",egtpMsg->msgHdr.teId);


   /* If any one of S, E or PN flag is set, set extension present as true. */
   if((tmpByte[0] & EGTP_MASK_BIT1) || (tmpByte[0] & EGTP_MASK_BIT2)||(tmpByte[0] & EGTP_MASK_BIT3))
   {
      extPres = TRUE;
   }

   /* Decode sequence number, if S flag is set in first byte */
   if (tmpByte[0] & EGTP_MASK_BIT2)
   {
      /************************************************************************
       * Sequence num is 2 bytes. So decode next 2 bytes from msg hdr and 
       * perform OR operation on them 
       ************************************************************************/
      egtpMsg->msgHdr.seqNum.pres = TRUE;
      ODU_REM_PRE_MSG(&tmpByte[1], mBuf);
      ODU_REM_PRE_MSG(&tmpByte[2], mBuf);
      egtpMsg->msgHdr.seqNum.val = (tmpByte[1] << 8) | tmpByte[2];
   }
   /**************************************************************************** 
    * If extPres is true, but S bit is not set, implies, either of PN or E bit 
    * was set during Encode so accordingly extract Byte fields for seqNum anyway 
    ***************************************************************************/
   else if(extPres)
   {
      /*************************************************************************
       * Sequence num is 2 bytes. So decode next 2 bytes from msg hdr and 
       * perform OR operation on them 
       ************************************************************************/
      egtpMsg->msgHdr.seqNum.pres = 0;
      ODU_REM_PRE_MSG(&tmpByte[1], mBuf);
      ODU_REM_PRE_MSG(&tmpByte[2], mBuf);
      egtpMsg->msgHdr.seqNum.val = (tmpByte[1] << 8) | tmpByte[2];
   }

   /* Decode N-PDU number if present flag is set */
   if (tmpByte[0] & EGTP_MASK_BIT1)
   {
      egtpMsg->msgHdr.nPdu.pres = TRUE;
      ODU_REM_PRE_MSG(&(egtpMsg->msgHdr.nPdu.val), mBuf);
   }
   /****************************************************************************
    * If extPres is true, but PN bit is not set, implies, either of S or E bit 
    * was set during Encode. Aaccordingly extract Byte fields of N-PDU num anyway 
    ***************************************************************************/
   else if(extPres)
   {
      egtpMsg->msgHdr.nPdu.pres = TRUE;
      ODU_REM_PRE_MSG(&(egtpMsg->msgHdr.nPdu.val), mBuf);
   }

   /* If E flag is set in first byte, decode extension header */ 
   if(tmpByte[0] & EGTP_MASK_BIT3)
   {
      ODU_REM_PRE_MSG(&extHdrType, mBuf);
      while( 0 != extHdrType)
      {
         switch (extHdrType)
         {
            case EGTP_EXT_HDR_UDP_TYPE:
            {
               ODU_REM_PRE_MSG(&extHdrLen, mBuf);
               if(extHdrLen == 0x01)
               {
                  /************************************************************
                   * UDP Port is 2 bytes. So decode next 2 bytes from msg hdr
                  * and perform OR operation on them 
                  *************************************************************/
                  egtpMsg->msgHdr.extHdr.udpPort.pres = TRUE;
                  ODU_REM_PRE_MSG(&tmpByte[1], mBuf);
                  ODU_REM_PRE_MSG(&tmpByte[2], mBuf);
                  egtpMsg->msgHdr.extHdr.udpPort.val = (tmpByte[1] << 8) | tmpByte[2];
               }
               break;
            }

            case EGTP_EXT_HDR_PDCP_TYPE:
            {
               ODU_REM_PRE_MSG(&extHdrLen, mBuf);
               if(extHdrLen == 0x01)
               {
                  /*************************************************************
                   * PDCP num is 2 bytes. So decode next 2 bytes from msg hdr
                   * and perform OR operation on them 
                   ************************************************************/
                  egtpMsg->msgHdr.extHdr.pdcpNmb.pres = TRUE;
                  ODU_REM_PRE_MSG(&tmpByte[1], mBuf);
                  ODU_REM_PRE_MSG(&tmpByte[2], mBuf);
                  egtpMsg->msgHdr.extHdr.pdcpNmb.val = (tmpByte[1] << 8) | tmpByte[2];
               }
               break;
            }
         } /* End of switch */
 
         ODU_REM_PRE_MSG(&extHdrType, mBuf);
 
      } /* End of while */
   }  
   /****************************************************************************
    * If extPres is true, but E bit is not set, implies, either of PN or S bit 
    * was set during Encode so accordingly extract Byte fields for extension
    * header anyway 
    ***************************************************************************/
   else if(extPres)
   {
      ODU_REM_PRE_MSG(&extHdrType, mBuf);
   }

   egtpMsg->msg = mBuf;

   //DU_LOG("\nDEBUG   -->  EGTP : DL Data Buffer after decoding header ");
   //ODU_PRINT_MSG(mBuf, 0, 0);

   /* Forward the data to duApp/RLC */
 
   return ROK;

}
/**********************************************************************
         End of file
**********************************************************************/
