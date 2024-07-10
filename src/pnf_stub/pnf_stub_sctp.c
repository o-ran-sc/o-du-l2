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
#include "nfapi_interface.h"
#include "pnf_stub_sctp.h"
#include "pnf_stub_p7_udp.h"
#include "pnf_stub.h"
#include "pnf_stub_p5_msg_hdl.h"
#include "pnf_stub_p7_msg_hdl.h"

uint8_t   socket_type;      /* Socket type */
PnfP5SctpGlobalCb pnfP5SctpCb;

/**************************************************************************
 * @brief Task Initiation callback function. 
 *
 * @details
 *
 *     Function : pnfP5SctpActvInit 
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
uint8_t pnfP5SctpActvInit()
{
   DU_LOG("DEBUG  -->  SCTP : Initializing");
   memset(&pnfP5SctpCb, 0, sizeof(PnfP5SctpGlobalCb));
   pnfP5SctpCb.pnfP5SctpCfg = pnfCb.pnfCfgParams.pnfP5SctpParams;
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

uint8_t fillAddrLst(CmInetNetAddrLst *addrLstPtr, PnfP5SctpIpAddr *ipAddr)
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
uint8_t fillDestNetAddr(CmInetNetAddr *destAddrPtr, PnfP5SctpIpAddr *dstIpPtr)
{
   /* Filling destination address */
   destAddrPtr->type = CM_INET_IPV4ADDR_TYPE;
   destAddrPtr->u.ipv4NetAddr = CM_INET_NTOH_UINT32(dstIpPtr->ipV4Addr);
   return ROK;
}

/**************************************************************************
 * @brief Function to configure the Sctp Params during config Request
 *
 * @details
 *
 *      Function : pnfP5SctpCfgReq
 * 
 *      Functionality:
 *           This function configures SCTP Params during the config Request
 *     
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/

uint8_t pnfP5SctpCfgReq()
{
   int destIdx = 0, assocIdx = 0;
  
   fillAddrLst(&pnfP5SctpCb.localAddrLst, &pnfP5SctpCb.pnfP5SctpCfg.pnfP5localIpAddr);
   memset(&pnfP5SctpCb.pnfP5LstnSockFd, -1, sizeof(CmInetFd));
   for(destIdx=0; destIdx < pnfP5SctpCb.pnfP5SctpCfg.numDestNode; destIdx++)
   {   
      pnfP5SctpCb.assocCb[assocIdx].destPort = pnfP5SctpCb.pnfP5SctpCfg.destCb[destIdx].destPort;
      memset(&pnfP5SctpCb.assocCb[assocIdx].sockFd, -1, sizeof(CmInetFd));
      fillDestNetAddr(&pnfP5SctpCb.assocCb[assocIdx].destIpNetAddr, &pnfP5SctpCb.pnfP5SctpCfg.destCb[destIdx].destIpAddr);
      pnfP5SctpCb.assocCb[assocIdx].connUp = false;
      assocIdx++;
   }   
   pnfP5SctpCb.numAssoc = assocIdx;
   return ROK;
}

/******************************************************************************
 *
 * @brief Eastablishes the Assoc Req for the received interface type
 *
 * @details
 *
 *    Function : pnfP5SctpStartReq
 *
 *    Functionality:
 *       Eastablishes the Socket Connection
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************************/

uint8_t pnfP5SctpStartReq()
{
   uint8_t assocIdx  = 0;
   uint8_t ret = ROK;

   socket_type = CM_INET_STREAM;

   if(pnfP5SctpCb.numAssoc)
   {

      if((ret = cmInetSocket(socket_type, &pnfP5SctpCb.pnfP5LstnSockFd, IPPROTO_SCTP) != ROK))
      {
         DU_LOG("ERROR  -->  SCTP : Socket[%d] coudnt open for listening", pnfP5SctpCb.pnfP5LstnSockFd.fd);
      } 
      else if((ret = cmInetSctpBindx(&pnfP5SctpCb.pnfP5LstnSockFd, &pnfP5SctpCb.localAddrLst, pnfP5SctpCb.pnfP5SctpCfg.pnfP5SctpPort)) != ROK)
      {
         DU_LOG("ERROR  -->  SCTP: Binding failed at PNF");
      }
      else if((ret = cmInetListen(&pnfP5SctpCb.pnfP5LstnSockFd, 1)) != ROK)
      {
         DU_LOG("ERROR  -->  SCTP: Unable to accept the connection at PNF");
         DU_LOG("ERROR  -->  SCTP : Listening on socket failed");
         cmInetClose(&pnfP5SctpCb.pnfP5LstnSockFd);
         return RFAILED;
      }
      else
      {
         for(assocIdx=0; assocIdx < pnfP5SctpCb.numAssoc; assocIdx++)
         {
            if((ret = pnfP5SctpAccept(&pnfP5SctpCb.assocCb[assocIdx])) != ROK)
            {
               DU_LOG("ERROR  -->  SCTP: Unable to accept the connection at PNF");
            }
         }
      }
   }

   /*Since Socket is UP and Client(VNF) has responded with CONNECT thus
    * initating PNF_READY_IND as part of PNF INITIALIZATION msg*/
   ret = buildAndSendPnfReadyInd();
   if(ret == ROK)
   {
      if(pnfP5SctpSockPoll() != ROK)
      {
         DU_LOG("ERROR  -->  SCTP: Polling failed to start at PNF");
      }
   }
   return (ret);
}

/*******************************************************************
 *
 * @brief Initiates connection with peer SCTP
 *
 * @details
 *
 *    Function : pnfP5SctpAccept
 *
 *    Functionality:
 *       Establishes SCTP connection with peer.
 *       Here, VNF-P5-SCTP will initate connection towards PNF-P5-SCTP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t pnfP5SctpAccept(PnfP5SctpAssocCb *assocCb)
{
   uint8_t  ret;

   DU_LOG("INFO   -->  SCTP : Connecting");

   while(!assocCb->connUp)
   {
      ret = cmInetAccept(&pnfP5SctpCb.pnfP5LstnSockFd, &assocCb->peerAddr, &assocCb->sockFd);
      if (ret == ROKDNA)
      {
         continue;
      }
      else if(ret != ROK)
      {
         DU_LOG("ERROR  -->  SCTP : Failed to accept connection");
         return RFAILED;
      }
      else
      {
         assocCb->connUp = TRUE;
         pnfP5SctpSetSockOpts(&assocCb->sockFd);
         break;
      }
   }
   DU_LOG("INFO   -->  SCTP : Connection established");

   return ROK;
}

/*******************************************************************
 *
 * @brief Sets socket options as per requirement
 *
 * @details
 *
 *    Function : pnfP5SctpSetSockOpts
 *
 *    Functionality: 
 *       Sets socket options as per requirement
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t pnfP5SctpSetSockOpts(CmInetFd *sock_Fd)
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
 * @brief Receives message on the socket
 *
 * @details
 *
 *    Function : pnfP5SctpSockPoll
 *
 *    Functionality:
 *      Receives message on the socket
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t pnfP5SctpSockPoll()
{
   uint8_t            assocIdx;
   uint16_t           ret = ROK;
   uint32_t           timeout;
   uint32_t           *timeoutPtr;
   uint64_t           numMsgRcvd = 0;
   Buffer             *pnfP7UdpBuf;
   MsgLen             pnfP7UdpBufLen;
   CmInetAddr         fromAddr;
   CmInetMemInfo      memInfo;
   PnfP5SctpSockPollParams pnfP5PollParams;
   
   memset(&pnfP5PollParams, 0, sizeof(PnfP5SctpSockPollParams));
    
   /* All sockets are non-blocking */
   timeout = 0;
   timeoutPtr = &timeout;
   memInfo.region = PNF_APP_MEM_REG;
   memInfo.pool   = PNF_POOL;

   CM_INET_FD_ZERO(&pnfP5PollParams.readFd);

   DU_LOG("INFO  -->  SCTP : Polling started at PNF\n");
   while(1)
   {
      /* Receiving SCTP data */
      for(assocIdx = 0; assocIdx < pnfP5SctpCb.numAssoc; assocIdx++)
      {
         if((ret = pnfP5ProcessPolling(&pnfP5PollParams, &pnfP5SctpCb.assocCb[assocIdx], timeoutPtr, &memInfo)) != ROK)
         {
            DU_LOG("ERROR  -->  P5_SCTP : Failed to RecvMsg for PNF at P5 Interface \n");
         }
      }

      /* Receiving UDP data */
      pnfP7UdpBufLen = -1;
      ret = cmInetRecvMsg(&(pnfP7Cb.sockFd), &fromAddr, &memInfo, &pnfP7UdpBuf, &pnfP7UdpBufLen, CM_INET_NO_FLAG);
      if(ret == ROK && pnfP7UdpBuf != NULLP)
      {
          if((fromAddr.port == pnfP7Cb.destAddr.port) && (fromAddr.address == pnfP7Cb.destAddr.address))
          {
               DU_LOG("INFO  -->  P7_UDP : Received P7 Message [%ld] \n", numMsgRcvd+1);
               numMsgRcvd++;
               pnfP7MsgHandler(pnfP7UdpBuf);
               ODU_PUT_MSG_BUF(pnfP7UdpBuf);
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
 *    Function : pnfP5ProcessPolling
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
 
uint8_t pnfP5ProcessPolling(PnfP5SctpSockPollParams *pollParams, PnfP5SctpAssocCb *assocCb, uint32_t *timeoutPtr, CmInetMemInfo *memInfo)
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
         DU_LOG("ERROR  -->  SCTP: Sctp Recv Failed at PNF P5 Interface");
      //   ret = RFAILED;
      }
      else
      {
         if(((pollParams->flag & CM_INET_SCTP_MSG_NOTIFICATION) != 0) && (ret == ROK))
         {
            ret = pnfP5SctpNtfyHdlr(assocCb, &pollParams->ntfy);
            if(ret != ROK)
            {
               DU_LOG("ERROR  -->  SCTP : Failed to process sctp notify msg\n");
               ret = RFAILED;
            }
         }
         else if(assocCb->connUp)
         {  
             /*TODO: Add the Handler of PNF P5 msgs*/
            if(p5MsgHandlerAtPnf(pollParams->mBuf) != ROK)
            {
               DU_LOG("ERROR  --> NFAPI_PNF: Failed to process SCTP msg received from VNF");
               ODU_PUT_MSG_BUF(pollParams->mBuf);
               return RFAILED;
            }
            ODU_PUT_MSG_BUF(pollParams->mBuf);
         }
         else
         {
            ODU_PUT_MSG_BUF(pollParams->mBuf);
         }
      } 
  }
  return ROK;
}/* End of pnfP5ProcessPolling() */

/*******************************************************************
 *
 * @brief Handles an SCTP notification message
 *
 * @details
 *
 *    Function : pnfP5SctpNtfyHdlr
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
uint8_t pnfP5SctpNtfyHdlr(PnfP5SctpAssocCb *assocCb, CmInetSctpNotification *ntfy)
{
   switch(ntfy->header.nType)
   {
      case CM_INET_SCTP_ASSOC_CHANGE :
         DU_LOG("INFO   -->  SCTP : Assoc change notification received");
         switch(ntfy->u.assocChange.state)
         {
            case CM_INET_SCTP_COMM_UP:
               DU_LOG("INFO   -->  Event : COMMUNICATION UP");
               assocCb->connUp = TRUE;
               break;
            case CM_INET_SCTP_COMM_LOST:
               DU_LOG("INFO   -->  Event : COMMUNICATION LOST");
               assocCb->connUp = FALSE;
               break;
            case CM_INET_SCTP_RESTART:
               DU_LOG("INFO   -->  Event : SCTP RESTART");
               assocCb->connUp = FALSE;
               break;
            case CM_INET_SCTP_SHUTDOWN_COMP: /* association gracefully shutdown */
               DU_LOG("INFO   -->  Event : SHUTDOWN COMPLETE");
               assocCb->connUp = FALSE;
               break;
            case CM_INET_SCTP_CANT_STR_ASSOC:
               DU_LOG("INFO   -->  Event : CANT START ASSOC");
               assocCb->connUp = FALSE;
               break;
            default:
               DU_LOG("ERROR   -->  Invalid event");
               break;
         }
         break;
      case CM_INET_SCTP_PEER_ADDR_CHANGE :
         DU_LOG("INFO   -->  SCTP : Peer Address Change notificarion received");
         /* Need to add handler */
         break;
      case CM_INET_SCTP_REMOTE_ERROR :
         DU_LOG("INFO   -->  SCTP : Remote Error notification received");
         break;
      case CM_INET_SCTP_SEND_FAILED :
         DU_LOG("INFO   -->  SCTP : Send Failed notification received\n");
         break;
      case CM_INET_SCTP_SHUTDOWN_EVENT : /* peer socket gracefully closed */
         DU_LOG("INFO   -->  SCTP : Shutdown Event notification received\n");
         assocCb->connUp = FALSE;
         /*TODO: delete vnf info or database*/
         //deleteE2NodeInfo(&ricCb.duInfo[0]);
         exit(0);
         break;
      case CM_INET_SCTP_ADAPTATION_INDICATION :
         DU_LOG("INFO   -->  SCTP : Adaptation Indication received\n");
         break;
      case CM_INET_SCTP_PARTIAL_DELIVERY_EVENT:
         DU_LOG("INFO   -->  SCTP : Partial Delivery Event received\n");
         break;
      default:
         DU_LOG("ERROR   -->  SCTP : Invalid notification type\n");
         break;
   }

   return ROK;
}/* End of pnfP5SctpNtfyHdlr */

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
//uint8_t pnfP5SctpSend(uint32_t vnfId, Buffer *mBuf) /*TODO: To check the VNF ID requirement here*/
uint8_t pnfP5SctpSend(Buffer *mBuf)
{
   uint8_t          assocIdx;
   uint8_t          ret;
   MsgLen           len;          /* number of actually sent octets */
   CmInetMemInfo    memInfo;                        
   
   memInfo.region = PNF_APP_MEM_REG;               
   memInfo.pool   = PNF_POOL;

   for(assocIdx = 0; assocIdx < pnfP5SctpCb.numAssoc; assocIdx++)
   {
      /*TODO: To check whether below check req using VNFID*/
     // if(sctpCb.assocCb[assocIdx].duId == duId)
      {
         ret = cmInetSctpSendMsg(&pnfP5SctpCb.assocCb[assocIdx].sockFd, &pnfP5SctpCb.assocCb[assocIdx].destIpNetAddr, \
               pnfP5SctpCb.assocCb[assocIdx].destPort, &memInfo, mBuf, &len, 0, FALSE, 0, 0/*SCT_PROTID_NONE*/, RWOULDBLOCK);

         if(ret != ROK && ret != RWOULDBLOCK)
         {
            DU_LOG("ERROR  -->  SCTP : Send message failed");
            return RFAILED;
         }
      }
   }
   return ROK;
} /* End of pnfP5SctpSend */
/**********************************************************************
         End of file
**********************************************************************/


