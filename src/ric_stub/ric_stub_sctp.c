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
#include "ric_stub_sctp.h"
#include "ric_stub.h"
#include "ric_e2ap_msg_hdl.h"
#include "E2AP-PDU.h"

/**************************************************************************
 * @brief Function to configure the Sctp Params during config Request
 *
 * @details
 *
 *      Function : sctpCfgReq
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
   int destIdx;

   sctpCb.sctpCfg = ricCfgParams.sctpParams;   
   for(destIdx=0; destIdx < sctpCb.sctpCfg.numDu; destIdx++)
   {   
      sctpCb.destCb[destIdx].destPort = sctpCb.sctpCfg.sctpAssoc[destIdx].duPort;
      sctpCb.destCb[destIdx].srcPort  = sctpCb.sctpCfg.sctpAssoc[destIdx].ricPort;
      sctpCb.destCb[destIdx].bReadFdSet = ROK;
      memset(&sctpCb.destCb[destIdx].sockFd, -1, sizeof(CmInetFd));
      memset(&sctpCb.destCb[destIdx].lstnSockFd, -1, sizeof(CmInetFd));
      fillDestNetAddr(&sctpCb.destCb[destIdx].destIpNetAddr, &sctpCb.sctpCfg.sctpAssoc[destIdx].duIpAddr);
      fillAddrLst(&sctpCb.destCb[destIdx].localAddrLst, &sctpCb.sctpCfg.sctpAssoc[destIdx].ricIpAddr);
      sctpCb.destCb[destIdx].connUp = false;
   }   
   sctpCb.numDu = sctpCb.sctpCfg.numDu;
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
   uint8_t destIdx  = 0;
   uint8_t ret = ROK;

   socket_type = CM_INET_STREAM;

   for(destIdx=0; destIdx < sctpCb.numDu; destIdx++)
   {
      if((ret = cmInetSocket(socket_type, &sctpCb.destCb[destIdx].lstnSockFd, IPPROTO_SCTP) != ROK))
      {
         DU_LOG("\nERROR  -->  SCTP : Socket[%d] coudnt open for listening", sctpCb.destCb[destIdx].lstnSockFd.fd);
      } 
      else if((ret = cmInetSctpBindx(&sctpCb.destCb[destIdx].lstnSockFd, &sctpCb.destCb[destIdx].localAddrLst, \
         sctpCb.destCb[destIdx].srcPort)) != ROK)
      {
         DU_LOG("\nERROR  -->  SCTP: Binding failed at RIC");
      }
      else if((ret = sctpAccept(&sctpCb.destCb[destIdx])) != ROK)
      {
         DU_LOG("\nERROR  -->  SCTP: Unable to accept the connection at RIC");
      }
   }

   if(sctpSockPoll() != ROK)
   {
      DU_LOG("\nERROR  -->  SCTP: Polling failed to start at RIC");
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
 *       Here, DU-SCTP will initate connection towards RIC-SCTP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sctpAccept(RicSctpDestCb *destCb)
{
   uint8_t  ret;

   ret = cmInetListen(&destCb->lstnSockFd, 1);
   if (ret != ROK)
   {
      DU_LOG("\nERROR   -->  SCTP : Listening on socket failed");
      cmInetClose(&destCb->lstnSockFd);
      return RFAILED;
   }
   
   DU_LOG("\nINFO   -->  SCTP : Connecting");

   while(!destCb->connUp)
   {
      ret = cmInetAccept(&destCb->lstnSockFd, &destCb->peerAddr, &destCb->sockFd);
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
         destCb->connUp = TRUE;
         sctpSetSockOpts(&destCb->sockFd);
         break;
      }
   }
   DU_LOG("\nINFO   -->  SCTP : Connection established");

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
uint8_t sctpNtfyHdlr(RicSctpDestCb *destCb, CmInetSctpNotification *ntfy)
{
   switch(ntfy->header.nType)
   {
      case CM_INET_SCTP_ASSOC_CHANGE :
         DU_LOG("\nINFO   -->  SCTP : Assoc change notification received");
         switch(ntfy->u.assocChange.state)
         {
            case CM_INET_SCTP_COMM_UP:
               DU_LOG("\nINFO   -->  Event : COMMUNICATION UP");
               destCb->connUp = TRUE;
               break;
            case CM_INET_SCTP_COMM_LOST:
               DU_LOG("\nINFO   -->  Event : COMMUNICATION LOST");
               destCb->connUp = FALSE;
               break;
            case CM_INET_SCTP_RESTART:
               DU_LOG("\nINFO   -->  Event : SCTP RESTART");
               destCb->connUp = FALSE;
               break;
            case CM_INET_SCTP_SHUTDOWN_COMP: /* association gracefully shutdown */
               DU_LOG("\nINFO   -->  Event : SHUTDOWN COMPLETE");
               destCb->connUp = FALSE;
               break;
            case CM_INET_SCTP_CANT_STR_ASSOC:
               DU_LOG("\nINFO   -->  Event : CANT START ASSOC");
               destCb->connUp = FALSE;
               break;
            default:
               DU_LOG("\nERROR   -->  Invalid event");
               break;
         }
         break;
      case CM_INET_SCTP_PEER_ADDR_CHANGE :
         DU_LOG("\nINFO   -->  SCTP : Peer Address Change notificarion received");
         /* Need to add handler */
         break;
      case CM_INET_SCTP_REMOTE_ERROR :
         DU_LOG("\nINFO   -->  SCTP : Remote Error notification received");
         break;
      case CM_INET_SCTP_SEND_FAILED :
         DU_LOG("\nINFO   -->  SCTP : Send Failed notification received\n");
         break;
      case CM_INET_SCTP_SHUTDOWN_EVENT : /* peer socket gracefully closed */
         DU_LOG("\nINFO   -->  SCTP : Shutdown Event notification received\n");
         destCb->connUp = FALSE;
         exit(0);
         break;
      case CM_INET_SCTP_ADAPTATION_INDICATION :
         DU_LOG("\nINFO   -->  SCTP : Adaptation Indication received\n");
         break;
      case CM_INET_SCTP_PARTIAL_DELIVERY_EVENT:
         DU_LOG("\nINFO   -->  SCTP : Partial Delivery Event received\n");
         break;
      default:
         DU_LOG("\nERROR   -->  SCTP : Invalid notification type\n");
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
   uint8_t            destIdx;
   uint16_t           ret = ROK;
   uint32_t           timeout;
   uint32_t           *timeoutPtr;
   CmInetMemInfo      memInfo;
   sctpSockPollParams e2PollParams;

   memset(&e2PollParams, 0, sizeof(sctpSockPollParams));
    
   /* All sockets are non-blocking */
   timeout = 0;
   timeoutPtr = &timeout;
   memInfo.region = RIC_APP_MEM_REG;
   memInfo.pool   = RIC_POOL;

   CM_INET_FD_ZERO(&e2PollParams.readFd);

   DU_LOG("\nINFO  -->  SCTP : Polling started at RIC\n");
   while(1)
   {
      /* Receiving SCTP data */
      for(destIdx = 0; destIdx < sctpCb.numDu; destIdx++)
      {

         if((ret = processPolling(&e2PollParams, &sctpCb.destCb[destIdx], timeoutPtr, &memInfo)) != ROK)
         {
            DU_LOG("\nERROR  -->  SCTP : Failed to RecvMsg for E2 at RIC \n");
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

 
uint8_t processPolling(sctpSockPollParams *pollParams, RicSctpDestCb *destCb, uint32_t *timeoutPtr, CmInetMemInfo *memInfo)
{
   uint16_t ret = ROK;

   CM_INET_FD_SET(&destCb->sockFd, &pollParams->readFd);
   ret = cmInetSelect(&pollParams->readFd, NULLP, timeoutPtr, &pollParams->numFd);
   if(CM_INET_FD_ISSET(&destCb->sockFd, &pollParams->readFd))
   {
      CM_INET_FD_CLR(&destCb->sockFd, &pollParams->readFd);
      ret = cmInetSctpRecvMsg(&destCb->sockFd, &pollParams->addr, &pollParams->port, memInfo, &pollParams->mBuf, \
          &pollParams->bufLen, &pollParams->info, &pollParams->flag, &pollParams->ntfy);
      if(destCb->connUp & (ret != ROK))
      {
         destCb->bReadFdSet = RFAILED;
      }
      else
      {
         if(((pollParams->flag & CM_INET_SCTP_MSG_NOTIFICATION) != 0) && (ret == ROK))
         {
            ret = sctpNtfyHdlr(destCb, &pollParams->ntfy);
            if(ret != ROK)
            {
               DU_LOG("\nERROR  -->  SCTP : Failed to process sctp notify msg\n");
            }
         }
         else if(destCb->connUp & (pollParams->port == destCb->destPort))
         {  
            E2APMsgHdlr(pollParams->mBuf);
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
uint8_t sctpSend(Buffer *mBuf)
{
   uint8_t          ret;
   MsgLen           len;          /* number of actually sent octets */
   CmInetMemInfo    memInfo;                        
   
   memInfo.region = RIC_APP_MEM_REG;               
   memInfo.pool   = RIC_POOL;

   ret = cmInetSctpSendMsg(&sctpCb.destCb[0].sockFd, &sctpCb.destCb[0].destIpNetAddr, sctpCb.destCb[0].destPort, &memInfo, \
      mBuf, &len, 0, FALSE, 0, 0/*SCT_PROTID_NONE*/, RWOULDBLOCK);

   if(ret != ROK && ret != RWOULDBLOCK)
   {
      DU_LOG("\nERROR  -->  SCTP : Send message failed");
      return RFAILED;
   }

   return ROK;
} /* End of sctpSend */
/**********************************************************************
         End of file
**********************************************************************/
