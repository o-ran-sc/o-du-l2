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
   int destIdx = 0, assocIdx = 0;

   sctpCb.sctpCfg = ricCb.ricCfgParams.sctpParams;   
   fillAddrLst(&sctpCb.localAddrLst, &sctpCb.sctpCfg.localIpAddr);
   memset(&sctpCb.e2LstnSockFd, -1, sizeof(CmInetFd));
   for(destIdx=0; destIdx < sctpCb.sctpCfg.numDestNode; destIdx++)
   {   
      sctpCb.assocCb[assocIdx].destPort = sctpCb.sctpCfg.destCb[destIdx].destPort;
      sctpCb.assocCb[assocIdx].bReadFdSet = ROK;
      memset(&sctpCb.assocCb[assocIdx].sockFd, -1, sizeof(CmInetFd));
      fillDestNetAddr(&sctpCb.assocCb[assocIdx].destIpNetAddr, &sctpCb.sctpCfg.destCb[destIdx].destIpAddr);
      sctpCb.assocCb[assocIdx].connUp = false;
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
   uint8_t assocIdx  = 0;
   uint8_t ret = ROK;

   socket_type = CM_INET_STREAM;

   if(sctpCb.numAssoc)
   {

      if((ret = cmInetSocket(socket_type, &sctpCb.e2LstnSockFd, IPPROTO_SCTP) != ROK))
      {
         DU_LOG("\nERROR  -->  SCTP : Socket[%d] coudnt open for listening", sctpCb.e2LstnSockFd.fd);
      } 
      else if((ret = cmInetSctpBindx(&sctpCb.e2LstnSockFd, &sctpCb.localAddrLst, sctpCb.sctpCfg.e2SctpPort)) != ROK)
      {
         DU_LOG("\nERROR  -->  SCTP: Binding failed at RIC");
      }
      else if(ret = cmInetListen(&sctpCb.e2LstnSockFd, 1) != ROK)
      {
         DU_LOG("\nERROR  -->  SCTP: Unable to accept the connection at CU");
         DU_LOG("\nERROR  -->  SCTP : Listening on socket failed");
         cmInetClose(&sctpCb.e2LstnSockFd);
         return RFAILED;
      }
      else
      {
         for(assocIdx=0; assocIdx < sctpCb.numAssoc; assocIdx++)
         {
            if((ret = sctpAccept(&sctpCb.assocCb[assocIdx])) != ROK)
            {
               DU_LOG("\nERROR  -->  SCTP: Unable to accept the connection at RIC");
            }
         }
      }
   }

   if(ret == ROK)
   {
      if(sctpSockPoll() != ROK)
      {
         DU_LOG("\nERROR  -->  SCTP: Polling failed to start at RIC");
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
 *       Here, DU-SCTP will initate connection towards RIC-SCTP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sctpAccept(RicSctpAssocCb *assocCb)
{
   uint8_t  ret;

   DU_LOG("\nINFO   -->  SCTP : Connecting");

   while(!assocCb->connUp)
   {
      ret = cmInetAccept(&sctpCb.e2LstnSockFd, &assocCb->peerAddr, &assocCb->sockFd);
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
uint8_t sctpNtfyHdlr(RicSctpAssocCb *assocCb, CmInetSctpNotification *ntfy)
{
   switch(ntfy->header.nType)
   {
      case CM_INET_SCTP_ASSOC_CHANGE :
         DU_LOG("\nINFO   -->  SCTP : Assoc change notification received");
         switch(ntfy->u.assocChange.state)
         {
            case CM_INET_SCTP_COMM_UP:
               DU_LOG("\nINFO   -->  Event : COMMUNICATION UP");
               assocCb->connUp = TRUE;
               break;
            case CM_INET_SCTP_COMM_LOST:
               DU_LOG("\nINFO   -->  Event : COMMUNICATION LOST");
               assocCb->connUp = FALSE;
               break;
            case CM_INET_SCTP_RESTART:
               DU_LOG("\nINFO   -->  Event : SCTP RESTART");
               assocCb->connUp = FALSE;
               break;
            case CM_INET_SCTP_SHUTDOWN_COMP: /* association gracefully shutdown */
               DU_LOG("\nINFO   -->  Event : SHUTDOWN COMPLETE");
               assocCb->connUp = FALSE;
               break;
            case CM_INET_SCTP_CANT_STR_ASSOC:
               DU_LOG("\nINFO   -->  Event : CANT START ASSOC");
               assocCb->connUp = FALSE;
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
         assocCb->connUp = FALSE;
         deleteE2NodeInfo(&ricCb.duInfo[0]);
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
   uint8_t            assocIdx;
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
      for(assocIdx = 0; assocIdx < sctpCb.numAssoc; assocIdx++)
      {
         if((ret = processPolling(&e2PollParams, &sctpCb.assocCb[assocIdx], timeoutPtr, &memInfo)) != ROK)
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

 
uint8_t processPolling(sctpSockPollParams *pollParams, RicSctpAssocCb *assocCb, uint32_t *timeoutPtr, CmInetMemInfo *memInfo)
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
         else if(assocCb->connUp)
         {  
            E2APMsgHdlr(&assocCb->duId, pollParams->mBuf);
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
uint8_t sctpSend(uint32_t duId, Buffer *mBuf)
{
   uint8_t          assocIdx;
   uint8_t          ret;
   MsgLen           len;          /* number of actually sent octets */
   CmInetMemInfo    memInfo;                        
   
   memInfo.region = RIC_APP_MEM_REG;               
   memInfo.pool   = RIC_POOL;

   for(assocIdx = 0; assocIdx < sctpCb.numAssoc; assocIdx++)
   {
      if(sctpCb.assocCb[assocIdx].duId == duId)
      {
         ret = cmInetSctpSendMsg(&sctpCb.assocCb[assocIdx].sockFd, &sctpCb.assocCb[assocIdx].destIpNetAddr, \
               sctpCb.assocCb[assocIdx].destPort, &memInfo, mBuf, &len, 0, FALSE, 0, 0/*SCT_PROTID_NONE*/, RWOULDBLOCK);

         if(ret != ROK && ret != RWOULDBLOCK)
         {
            DU_LOG("\nERROR  -->  SCTP : Send message failed");
            return RFAILED;
         }
      }
   }
   return ROK;
} /* End of sctpSend */
/**********************************************************************
         End of file
**********************************************************************/
