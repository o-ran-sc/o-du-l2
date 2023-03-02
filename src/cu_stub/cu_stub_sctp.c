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

/* This file contains all SCTP related functionality */

#include "common_def.h" 
#include "OCTET_STRING.h"
#include "cu_f1ap_msg_hdl.h"
#include "cu_stub_sctp.h"
#include "cu_stub_egtp.h"
#include "cu_stub.h"
#include "cm_inet.h"
#include "cm_tpt.h"

#include "cm_inet.x"
#include "cm_tpt.x"


/**************************************************************************
 * @brief Task Initiation callback function. 
 *
 * @details
 *
 *     Function : sctpActvInit 
 *    
 *     Functionality:
 *             This function is supplied as one of parameters during SCTP's 
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
uint8_t sctpActvInit()
{
   DU_LOG("\n\nDEBUG  -->  SCTP : Initializing");
   memset(&sctpCb, 0, sizeof(SctpGlobalCb));
   sctpCb.sctpCfg = cuCb.cuCfgParams.sctpParams;
   return ROK;
}

/**************************************************************************
 * @brief Task Activation callback function. 
 *
 * @details
 *
 *      Function : sctpActvTsk 
 * 
 *      Functionality:
 *           This function handles all SCTP messages received
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
uint8_t sctpActvTsk(Pst *pst, Buffer *mBuf)
{

//TODO: TBD
   return ROK;
}

/**************************************************************************
 * @brief Function to configure the Sctp Params during config Request
 *
 * @details
 *
 *      Function : duSctpCfgReq
 * 
 *      Functionality:
 *           This function configures SCTP Params during the config Request
 *     
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/

uint8_t sctpCfgReq()
{
   int destIdx = 0, assocIdx = 0;
   
   fillAddrLst(&sctpCb.localAddrLst, &sctpCb.sctpCfg.localIpAddr);
   memset(&sctpCb.f1LstnSockFd, -1, sizeof(CmInetFd));

   for(destIdx=0; destIdx < sctpCb.sctpCfg.f1SctpInfo.numDestNode; destIdx++)
   {
      sctpCb.assocCb[assocIdx].intf = F1_INTERFACE;
      sctpCb.assocCb[assocIdx].destPort = sctpCb.sctpCfg.f1SctpInfo.destCb[destIdx].destPort;
      sctpCb.assocCb[assocIdx].bReadFdSet = ROK;
      memset(&sctpCb.assocCb[assocIdx].sockFd, -1, sizeof(CmInetFd));
      fillDestNetAddr(&sctpCb.assocCb[assocIdx].destIpNetAddr, &sctpCb.sctpCfg.f1SctpInfo.destCb[destIdx].destIpAddr);
      assocIdx++;
   }

   sctpCb.localXnNodeType = sctpCb.sctpCfg.xnSctpInfo.localNodeType;
   for(destIdx=0; destIdx < sctpCb.sctpCfg.xnSctpInfo.numDestNode; destIdx++)
   {
      sctpCb.assocCb[assocIdx].intf = XN_INTERFACE;
      sctpCb.assocCb[assocIdx].destPort = sctpCb.sctpCfg.xnSctpInfo.destCb[destIdx].destPort;
      sctpCb.assocCb[assocIdx].bReadFdSet = ROK;
      memset(&sctpCb.assocCb[assocIdx].sockFd, -1, sizeof(CmInetFd));
      fillDestNetAddr(&sctpCb.assocCb[assocIdx].destIpNetAddr, &sctpCb.sctpCfg.xnSctpInfo.destCb[destIdx].destIpAddr);
      assocIdx++;
   }
   sctpCb.numAssoc = assocIdx;
   return ROK;
}


/*******************************************************************
 *
 * @brief Fills the address List of the source Ip Address
 *
 * @details
 *
 *    Function : fillAddrLst
 *
 *    Functionality:
 *       Fills the address List of source Ip Address
 *
 * @params[in] CmInetNetAddrLst *addrLstPtr, Address List pointer
 * @params[in] F1IpAddr *srcIpAddr, src Ip Adrress to be filled in the Address List
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t fillAddrLst(CmInetNetAddrLst *addrLstPtr, SctpIpAddr *ipAddr)
{ 
   addrLstPtr->addrs[addrLstPtr->count].type = CM_INET_IPV4ADDR_TYPE;
   addrLstPtr->addrs[addrLstPtr->count].u.ipv4NetAddr = CM_INET_NTOH_UINT32(ipAddr->ipV4Addr);
   addrLstPtr->count++;
   return ROK;
}

/******************************************************************************
 *
 * @brief Fills the address List of the source Ip Address
 *
 * @details
 *
 *    Function : fillDestNetAddr
 *
 *    Functionality:
 *       Fills the address List of destinatoion Ip Address
 *
 * @params[in] CmInetNetAddr *destAddrPtr, Address List pointer
 * @params[in] F1IpAddr *dstIpAddr, destIp Address to be filled in the Address List
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************************/
uint8_t fillDestNetAddr(CmInetNetAddr *destAddrPtr, SctpIpAddr *dstIpPtr)
{
   /* Filling destination address */
   destAddrPtr->type = CM_INET_IPV4ADDR_TYPE;
   destAddrPtr->u.ipv4NetAddr = CM_INET_NTOH_UINT32(dstIpPtr->ipV4Addr);
   return ROK;
}

/******************************************************************************
 *
 * @brief Eastablishes the Assoc Req for the received interface type
 *
 * @details
 *
 *    Function : sctpStartReq
 *
 *    Functionality:
 *       Eastablishes the Assoc Req for the received interface type
 *
 * @params[in] DuSctpDestCb *paramPtr
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************************/

uint8_t sctpStartReq()
{
   uint8_t assocIdx;
   uint8_t ret = ROK;
   CmInetFd sockFd;

   socket_type = CM_INET_STREAM;

   /* Establish SCTP association at XN interface */
   if(sctpCb.sctpCfg.xnSctpInfo.numDestNode)
   {
      if(sctpCb.localXnNodeType == SERVER)
      {
         if((ret = cmInetSocket(socket_type, &sctpCb.xnLstnSockFd, IPPROTO_SCTP) != ROK))
         {   
            DU_LOG("\nERROR  -->  SCTP : Socket[%d] coudnt open for listening", sctpCb.f1LstnSockFd.fd);
         }   
         else if((ret = cmInetSctpBindx(&sctpCb.xnLstnSockFd, &sctpCb.localAddrLst, sctpCb.sctpCfg.xnSctpInfo.port)) != ROK)
         {   
            DU_LOG("\nERROR  -->  SCTP: Binding failed at CU");
         }   
         else if(ret = cmInetListen(&sctpCb.xnLstnSockFd, 1) != ROK)
         {   
            DU_LOG("\nERROR  -->  SCTP : Listening on socket failed");
            cmInetClose(&sctpCb.xnLstnSockFd);
            return RFAILED;
         }   
         else
         {   
            for(assocIdx=0; assocIdx < sctpCb.numAssoc; assocIdx++)
            {   
               if(sctpCb.assocCb[assocIdx].intf == XN_INTERFACE)
               {   
                  if((ret = sctpAccept(&sctpCb.xnLstnSockFd, &sctpCb.assocCb[assocIdx])) != ROK)
                  {
                     DU_LOG("\nERROR  -->  SCTP: Unable to accept the connection at CU");
                  }
               }
            }
         }
      }
      else if(sctpCb.localXnNodeType == CLIENT)
      {
         for(assocIdx=0; assocIdx < sctpCb.numAssoc; assocIdx++)
         {
            if(sctpCb.assocCb[assocIdx].intf == XN_INTERFACE)
            {
               if((ret = cmInetSocket(socket_type, &sctpCb.assocCb[assocIdx].sockFd, IPPROTO_SCTP)) != ROK)
               {   
                  DU_LOG("\nERROR  -->  SCTP : Failed while opening a socket in ODU");
               }   
               else if((ret = cmInetSctpBindx(&sctpCb.assocCb[assocIdx].sockFd, &sctpCb.localAddrLst, sctpCb.sctpCfg.xnSctpInfo.port)) != ROK)
               {   
                  DU_LOG("\nERROR  -->  SCTP:  Failed during Binding in ODU");
               }   
               else if((ret = sctpSetSockOpts(&sctpCb.assocCb[assocIdx].sockFd)) != ROK)
               {   
                  DU_LOG("\nERROR  -->  SCTP : Failed to set Socket Opt in ODU");
               }    
               else
               {   
                  if(ret != ROK)
                  {   
                     DU_LOG("\nERROR  -->  SCTP : Failed while establishing Req at DU");
                     ret = RFAILED;
                  }   
                  else 
                  {   
                     ret = cmInetSctpConnectx(&sctpCb.assocCb[assocIdx].sockFd, &sctpCb.assocCb[assocIdx].destIpNetAddr, \
                              &sctpCb.assocCb[assocIdx].destAddrLst, sctpCb.assocCb[assocIdx].destPort);
                     /* 115 error_code indicates that Operation is in progress and hence ignored if SctpConnect failed due to this */
                     if(ret == 18)    
                     {
                        ret = ROK; 
                     }
                  }   
               }
            }
         }
      }
   }

   /* Establish SCTP association at F1 interface */
   if(sctpCb.sctpCfg.f1SctpInfo.numDestNode)
   {
      if((ret = cmInetSocket(socket_type, &sctpCb.f1LstnSockFd, IPPROTO_SCTP) != ROK))
      {
         DU_LOG("\nERROR  -->  SCTP : Socket[%d] coudnt open for listening", sctpCb.f1LstnSockFd.fd);
      } 
      else if((ret = cmInetSctpBindx(&sctpCb.f1LstnSockFd, &sctpCb.localAddrLst, sctpCb.sctpCfg.f1SctpInfo.port)) != ROK)
      {
         DU_LOG("\nERROR  -->  SCTP: Binding failed at CU");
      }
      else if(ret = cmInetListen(&sctpCb.f1LstnSockFd, 1) != ROK)
      {
         DU_LOG("\nERROR  -->  SCTP : Listening on socket failed");
         cmInetClose(&sctpCb.f1LstnSockFd);
         return RFAILED;
      }
      else
      {
         for(assocIdx=0; assocIdx < sctpCb.numAssoc; assocIdx++)
         {
            if(sctpCb.assocCb[assocIdx].intf == F1_INTERFACE)
            {
               if((ret = sctpAccept(&sctpCb.f1LstnSockFd, &sctpCb.assocCb[assocIdx])) != ROK)
               {
                  DU_LOG("\nERROR  -->  SCTP: Unable to accept the connection at CU");
               }
            }
         }
      }
   }

   if(ret == ROK)
   {
      if(sctpSockPoll() != ROK)
      {
         DU_LOG("\nERROR  -->  SCTP: Polling failed to start at CU");
      }
   }
   return (ret);
}

/*******************************************************************
 *
 * @brief Sets socket options as per requirement
 *
 * @details
 *
 *    Function : sctpSetSockOpts
 *
 *    Functionality: 
 *       Sets socket options as per requirement
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sctpSetSockOpts(CmInetFd *sock_Fd)
{
   S16 ret = ROK;
   CmSctpEvent sctpEvent;
  
   sctpEvent.dataIoEvent          = TRUE;
   sctpEvent.associationEvent     = TRUE;
   sctpEvent.addressEvent         = TRUE;
   sctpEvent.sendFailureEvent     = TRUE;
   sctpEvent.peerErrorEvent       = TRUE;
   sctpEvent.shutdownEvent        = TRUE;
   sctpEvent.partialDeliveryEvent = TRUE;
   sctpEvent.adaptationLayerEvent = TRUE;

   if((ret = cmInetSetOpt(sock_Fd, CM_SOCKOPT_LEVEL_SCTP, CM_SOCKOPT_OPT_SCTP_EVENTS, &sctpEvent)) != ROK)
   {
     ret = RFAILED;
   }
  
   return (ret);
}

/*******************************************************************
 *
 * @brief Initiates connection with peer SCTP
 *
 * @details
 *
 *    Function : sctpAccept
 *
 *    Functionality:
 *       Establishes SCTP connection with peer.
 *       Here, DU-SCTP will initate connection towards CU-SCTP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sctpAccept(CmInetFd *lstnSockFd, CuSctpAssocCb *assocCb)
{
   uint8_t  ret;

   DU_LOG("\nINFO  -->  SCTP : Connecting");

   while(!assocCb->connUp)
   {
      ret = cmInetAccept(lstnSockFd, &assocCb->peerAddr, &assocCb->sockFd);
      if (ret == ROKDNA)
      {
         continue;
      }
      else if(ret != ROK)
      {
         DU_LOG("\nERROR  -->  SCTP : Failed to accept connection");
         return RFAILED;
      }
      else
      {
         assocCb->connUp = TRUE;
         sctpSetSockOpts(&assocCb->sockFd);
         break;
      }
   }
   DU_LOG("\nINFO  -->  SCTP : Connection established");

   return ROK;
}

/*******************************************************************
 *
 * @brief Handles an SCTP notification message
 *
 * @details
 *
 *    Function : sctpNtfyHdlr
 *
 *    Functionality:
 *      Handles an SCTP notification message
 *
 * @params[in] Notify message
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sctpNtfyHdlr(CuSctpAssocCb *assocCb, CmInetSctpNotification *ntfy)
{
   switch(ntfy->header.nType)
   {
      case CM_INET_SCTP_ASSOC_CHANGE :
         DU_LOG("\nINFO  -->  SCTP : Assoc change notification received");
         switch(ntfy->u.assocChange.state)
         {
            case CM_INET_SCTP_COMM_UP:
               DU_LOG("DEBUG  -->  Event : COMMUNICATION UP");
               assocCb->connUp = TRUE;
               break;
            case CM_INET_SCTP_COMM_LOST:
               DU_LOG("DEBUG  -->  Event : COMMUNICATION LOST");
               assocCb->connUp = FALSE;
               break;
            case CM_INET_SCTP_RESTART:
               DU_LOG("DEBUG  -->  Event : SCTP RESTART");
               assocCb->connUp = FALSE;
               break;
            case CM_INET_SCTP_SHUTDOWN_COMP: /* association gracefully shutdown */
               DU_LOG("DEBUG  -->  Event : SHUTDOWN COMPLETE");
               assocCb->connUp = FALSE;
               break;
            case CM_INET_SCTP_CANT_STR_ASSOC:
               DU_LOG("DEBUG  -->  Event : CANT START ASSOC");
               assocCb->connUp = FALSE;
               break;
            default:
               DU_LOG("\nERROR  -->  Invalid event");
               break;
         }
         break;
      case CM_INET_SCTP_PEER_ADDR_CHANGE :
         DU_LOG("\nINFO  -->  SCTP : Peer Address Change notificarion received");
         /* Need to add handler */
         break;
      case CM_INET_SCTP_REMOTE_ERROR :
         DU_LOG("\nINFO  -->  SCTP : Remote Error notification received");
         break;
      case CM_INET_SCTP_SEND_FAILED :
         DU_LOG("\nINFO  -->  SCTP : Send Failed notification received\n");
         break;
      case CM_INET_SCTP_SHUTDOWN_EVENT : /* peer socket gracefully closed */
         DU_LOG("\nINFO  -->  SCTP : Shutdown Event notification received\n");
         assocCb->connUp = FALSE;
         exit(0);
         break;
      case CM_INET_SCTP_ADAPTATION_INDICATION :
         DU_LOG("\nINFO  -->  SCTP : Adaptation Indication received\n");
         break;
      case CM_INET_SCTP_PARTIAL_DELIVERY_EVENT:
         DU_LOG("\nINFO  -->  SCTP : Partial Delivery Event received\n");
         break;
      default:
         DU_LOG("\nERROR  -->  SCTP : Invalid notification type\n");
         break;
   }

   sctpNtfyInd(ntfy);
   return ROK;
}/* End of sctpNtfyHdlr */

/*******************************************************************
 *
 * @brief Receives message on the socket
 *
 * @details
 *
 *    Function : sctpSockPoll
 *
 *    Functionality:
 *      Receives message on the socket
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sctpSockPoll()
{
   uint8_t       assocIdx = 0, destIdx = 0;
   uint16_t      ret = ROK;
   uint32_t      timeout;
   uint32_t      *timeoutPtr;
   Buffer        *egtpBuf;
   MsgLen        egtpBufLen;
   CmInetMemInfo memInfo;
   sctpSockPollParams pollParams;
   uint64_t      numMsgRcvd = 0;
   CmInetAddr    fromAddr;

   memset(&pollParams, 0, sizeof(sctpSockPollParams));
    

   /* All sockets are non-blocking */
   timeout = 0;
   timeoutPtr = &timeout;
   memInfo.region = CU_APP_MEM_REG;
   memInfo.pool   = CU_POOL;
   
   CM_INET_FD_ZERO(&pollParams.readFd);

   while(1)
   {
      /* Receiving SCTP data */
      for(assocIdx = 0; assocIdx < sctpCb.numAssoc; assocIdx++)
      {
         if((ret = processPolling(&pollParams, &sctpCb.assocCb[assocIdx], timeoutPtr, &memInfo)) != ROK)
         {
            DU_LOG("\nERROR  -->  SCTP : Failed to RecvMsg for F1 at CU\n");
         }
      }

      /* Receiving EGTP data */
      egtpBufLen = -1;
      ret = cmInetRecvMsg(&(egtpCb.sockFd), &fromAddr, &memInfo, &egtpBuf, &egtpBufLen, CM_INET_NO_FLAG);
      if(ret == ROK && egtpBuf != NULLP)
      {
         for(destIdx = 0; destIdx < egtpCb.numDu; destIdx++)
         {
            if((fromAddr.port == egtpCb.dstCb[destIdx].dstAddr.port) && (fromAddr.address == egtpCb.dstCb[destIdx].dstAddr.address))
            {
               DU_LOG("\nINFO  -->  EGTP : Received UL Message [%ld] from DU Id [%d]\n", numMsgRcvd+1, egtpCb.dstCb[destIdx].duId);
               numMsgRcvd++;
               //ODU_PRINT_MSG(egtpBuf, 0 ,0);
               cuEgtpHdlRecvMsg(egtpBuf);
               break;
            }

         }
      }
   };
   return (ret);
}/* End of sctpSockPoll() */

/*******************************************************************
 *
 * @brief checks for valid readFd and process the InetSctpRecvMsg
 * during polling 
 *
 * @details
 *
 *    Function : processPolling
 *
 *    Functionality:
 *         checks for valid readFd and process the InetSctpRecvMsg
 *         during polling
 *
 * @params[in]  Params required for polling
 * @params[in]  SockFd for file descriptor
 * @params[in]  timeoutPtr indicates the timeout value
 * @params[in]  MemInfo indicates memory region
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t processPolling(sctpSockPollParams *pollParams, CuSctpAssocCb *assocCb, uint32_t *timeoutPtr, CmInetMemInfo *memInfo)
{
   uint16_t ret = ROK;
   CM_INET_FD_SET(&assocCb->sockFd, &pollParams->readFd);
   ret = cmInetSelect(&pollParams->readFd, NULLP, timeoutPtr, &pollParams->numFd);
   if(CM_INET_FD_ISSET(&assocCb->sockFd, &pollParams->readFd))
   {
      CM_INET_FD_CLR(&assocCb->sockFd, &pollParams->readFd);
      ret = cmInetSctpRecvMsg(&assocCb->sockFd, &pollParams->addr, &pollParams->port, memInfo, &pollParams->mBuf, \
         &pollParams->bufLen, &pollParams->info, &pollParams->flag, &pollParams->ntfy);
      if(assocCb->connUp & (ret != ROK))
      {
         assocCb->bReadFdSet = RFAILED;
      }
      else
      {
         if(((pollParams->flag & CM_INET_SCTP_MSG_NOTIFICATION) != 0) && (ret == ROK))
         {
            ret = sctpNtfyHdlr(assocCb, &pollParams->ntfy);
            if(ret != ROK)
            {
               DU_LOG("\nERROR  -->  SCTP : Failed to process sctp notify msg\n");
            }
         }
         else if(assocCb->connUp && assocCb->intf == F1_INTERFACE)
         {  
            F1APMsgHdlr(&assocCb->destId, pollParams->mBuf);
            ODU_PUT_MSG_BUF(pollParams->mBuf);
         }
         else if(assocCb->connUp && assocCb->intf == XN_INTERFACE)
         {
            DU_LOG("\nDEBUG  -->  SCTP : Received message at XN interface");
            ODU_PRINT_MSG(pollParams->mBuf, 0,0);
            XNAPMsgHdlr(&assocCb->destId, pollParams->mBuf);
            ODU_PUT_MSG_BUF(pollParams->mBuf);
         }
         else
         {
            ODU_PUT_MSG_BUF(pollParams->mBuf);
         }
      } 
  }
  return ROK;
}/* End of sctpSockPoll() */

/*******************************************************************
 *
 * @brief Send message on an SCTP Association
 *
 * @details
 *
 *    Function : sendOnSctpAssoc
 *
 *    Functionality:
 *        Send message on SCTP association
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sendOnSctpAssoc(CuSctpAssocCb *assocCb, Buffer *mBuf)
{
   uint8_t          ret = ROK;
   MsgLen           len = 0;          /* number of actually sent octets */
   CmInetMemInfo    memInfo;
   
   memset(&memInfo , 0, sizeof(CmInetMemInfo));
   memInfo.region = CU_APP_MEM_REG;    
   memInfo.pool   = CU_POOL;

   ret = cmInetSctpSendMsg(&assocCb->sockFd, &assocCb->destIpNetAddr, assocCb->destPort, &memInfo, mBuf, &len, 0, \
      FALSE, 0, 0/*SCT_PROTID_NONE*/, RWOULDBLOCK);

   if(ret != ROK && ret != RWOULDBLOCK)
   {
      DU_LOG("\nERROR  -->  SCTP : Send message failed");
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Send message on SCTP socket
 *
 * @details
 *
 *    Function : sctpSend 
 *
 *    Functionality:
 *        Send message on SCTP socket
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sctpSend(InterfaceType intf, uint32_t destId, Buffer *mBuf)
{
   uint8_t  assocIdx = 0;

   for(assocIdx=0; assocIdx < sctpCb.numAssoc; assocIdx++)
   {
      if((sctpCb.assocCb[assocIdx].intf == intf) && (sctpCb.assocCb[assocIdx].destId == destId))
      {
         if(sendOnSctpAssoc(&sctpCb.assocCb[assocIdx], mBuf) == ROK)
            return ROK;
         else
            break;
      }
   }
   DU_LOG("\nERROR  -->  SCTP : Dest ID [%d] at Interface [%d] not found in SCTP DestCb list. Failed to send message", destId, intf);
   return RFAILED;
} /* End of sctpSend */

/**********************************************************************
         End of file
**********************************************************************/
