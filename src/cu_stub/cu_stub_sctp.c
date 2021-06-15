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
#include "cu_f1ap_msg_hdl.h"
#include "cu_stub_sctp.h"
#include "cu_stub_egtp.h"
#include "du_log.h"

CuSctpDestCb f1Params;
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
S16 sctpActvInit()
{
   DU_LOG("\n\nDEBUG  -->  SCTP : Initializing");
   connUp = FALSE;
   assocId = 0;
   nonblocking = FALSE;
   sctpCfg = cuCfgParams.sctpParams;
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
S16 sctpActvTsk(Pst *pst, Buffer *mBuf)
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

S16 sctpCfgReq()
{

/* Fill F1 Params */
   f1Params.destPort             = sctpCfg.duPort;
   f1Params.srcPort              = sctpCfg.cuPort;
   f1Params.bReadFdSet           = ROK;
   memset(&f1Params.sockFd, -1, sizeof(CmInetFd));
   memset(&f1Params.lstnSockFd, -1, sizeof(CmInetFd));
   fillDestNetAddr(&f1Params.destIpNetAddr, &sctpCfg.duIpAddr);

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

S16 fillAddrLst(CmInetNetAddrLst *addrLstPtr, SctpIpAddr *ipAddr)
{ 
   addrLstPtr->count++;
   addrLstPtr->addrs[(addrLstPtr->count - 1)].type = CM_INET_IPV4ADDR_TYPE;
   addrLstPtr->addrs[(addrLstPtr->count - 1)].u.ipv4NetAddr = CM_INET_NTOH_UINT32(ipAddr->ipV4Addr);

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
S16 fillDestNetAddr(CmInetNetAddr *destAddrPtr, SctpIpAddr *dstIpPtr)
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

S16 sctpStartReq()
{
   S16 ret = ROK;
   socket_type = CM_INET_STREAM;
   fillAddrLst(&f1Params.localAddrLst, &sctpCfg.cuIpAddr);

   if((ret = cmInetSocket(socket_type, &f1Params.lstnSockFd, IPPROTO_SCTP) != ROK))
   {
      DU_LOG("\nERROR  -->  SCTP : Socket[%d] coudnt open for listening", f1Params.lstnSockFd.fd);
   } 
   else if((ret = cmInetSctpBindx(&f1Params.lstnSockFd, &f1Params.localAddrLst, f1Params.srcPort)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCTP: Binding failed at CU");
   }
   else if((ret = sctpAccept(&f1Params.lstnSockFd, &f1Params.peerAddr, &f1Params.sockFd)) != ROK)
   {
      DU_LOG("\nERROR  -->  SCTP: Unable to accept the connection at CU");
   }
   else if(sctpSockPoll() != ROK)
   {
      DU_LOG("\nERROR  -->  SCTP: Polling failed to start at CU");
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
S16 sctpSetSockOpts(CmInetFd *sock_Fd)
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
S16 sctpAccept(CmInetFd *lstnSock_Fd, CmInetAddr *peerAddr, CmInetFd *sock_Fd)
{
   uint8_t  ret;

   ret = cmInetListen(lstnSock_Fd, 1);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  SCTP : Listening on socket failed");
      cmInetClose(lstnSock_Fd);
      return RFAILED;
   }
   
   DU_LOG("\nINFO  -->  SCTP : Connecting");

   while(!connUp)
   {
      ret = cmInetAccept(lstnSock_Fd, peerAddr, sock_Fd);
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
         connUp = TRUE;
         sctpSetSockOpts(sock_Fd);
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
S16 sctpNtfyHdlr(CmInetSctpNotification *ntfy)
{
   switch(ntfy->header.nType)
   {
      case CM_INET_SCTP_ASSOC_CHANGE :
         DU_LOG("\nINFO  -->  SCTP : Assoc change notification received");
         switch(ntfy->u.assocChange.state)
         {
            case CM_INET_SCTP_COMM_UP:
               DU_LOG("DEBUG  -->  Event : COMMUNICATION UP");
               connUp = TRUE;
               break;
            case CM_INET_SCTP_COMM_LOST:
               DU_LOG("DEBUG  -->  Event : COMMUNICATION LOST");
               connUp = FALSE;
               break;
            case CM_INET_SCTP_RESTART:
               DU_LOG("DEBUG  -->  Event : SCTP RESTART");
               connUp = FALSE;
               break;
            case CM_INET_SCTP_SHUTDOWN_COMP: /* association gracefully shutdown */
               DU_LOG("DEBUG  -->  Event : SHUTDOWN COMPLETE");
               connUp = FALSE;
               break;
            case CM_INET_SCTP_CANT_STR_ASSOC:
               DU_LOG("DEBUG  -->  Event : CANT START ASSOC");
               connUp = FALSE;
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
         connUp = FALSE;
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
S16 sctpSockPoll()
{
   uint16_t      ret = ROK;
   uint32_t      timeout;
   uint32_t      *timeoutPtr;
   Buffer        *egtpBuf;
   MsgLen        egtpBufLen;
   CmInetAddr    egtpFromAddr;
   CmInetMemInfo memInfo;
   sctpSockPollParams f1PollParams;

   memset(&f1PollParams, 0, sizeof(sctpSockPollParams));
    
   egtpFromAddr.port = egtpCb.dstCb.dstPort;
   egtpFromAddr.address = egtpCb.dstCb.dstIp;

   if(f1Params.sockFd.blocking)
   {
      /* blocking */
      timeoutPtr = NULLP;
   }
   else
   {
      /* non-blocking */
      timeout = 0;
      timeoutPtr = &timeout;
   }
   memInfo.region = CU_APP_MEM_REG;
   memInfo.pool   = CU_POOL;
   
   CM_INET_FD_ZERO(&f1PollParams.readFd);

   while(1)
   {
      if((ret = processPolling(&f1PollParams, &f1Params.sockFd, timeoutPtr, &memInfo)) != ROK)
      {
         DU_LOG("\nERROR  -->  SCTP : Failed to RecvMsg for F1 at CU\n");
      }

      /* Receiving EGTP data */
      egtpBufLen = -1;
      ret = cmInetRecvMsg(&(egtpCb.recvTptSrvr.sockFd), &egtpFromAddr, &memInfo, &egtpBuf, &egtpBufLen, CM_INET_NO_FLAG);
      if(ret == ROK && egtpBuf != NULLP)
      {
         DU_LOG("\nINFO  -->  EGTP : Received message \n");
         ODU_PRINT_MSG(egtpBuf, 0 ,0);
         cuEgtpHdlRecvMsg(egtpBuf);

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

S16 processPolling(sctpSockPollParams *pollParams, CmInetFd *sockFd, uint32_t *timeoutPtr, CmInetMemInfo *memInfo)
{
   uint16_t ret = ROK;
   CM_INET_FD_SET(sockFd, &pollParams->readFd);
   ret = cmInetSelect(&pollParams->readFd, NULLP, timeoutPtr, &pollParams->numFd);
   if(CM_INET_FD_ISSET(sockFd, &pollParams->readFd))
   {
      CM_INET_FD_CLR(sockFd, &pollParams->readFd);
      ret = cmInetSctpRecvMsg(sockFd, &pollParams->addr, &pollParams->port, memInfo, &pollParams->mBuf, &pollParams->bufLen, &pollParams->info, &pollParams->flag, &pollParams->ntfy);
      if(connUp & (ret != ROK))
      {
         f1Params.bReadFdSet = RFAILED;
      }
      else
      {
         if(((pollParams->flag & CM_INET_SCTP_MSG_NOTIFICATION) != 0) && (ret == ROK))
         {
            ret = sctpNtfyHdlr(&pollParams->ntfy);
            if(ret != ROK)
            {
               DU_LOG("\nERROR  -->  SCTP : Failed to process sctp notify msg\n");
            }
         }
         else if(connUp & (pollParams->port == f1Params.destPort))
         {  
            F1APMsgHdlr(pollParams->mBuf);
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
S16 sctpSend(Buffer *mBuf)
{
   uint8_t          ret = ROK;
   MsgLen           len = 0;          /* number of actually sent octets */
   CmInetMemInfo    memInfo;                        
  
   memset(&memInfo , 0, sizeof(CmInetMemInfo));
   memInfo.region = CU_APP_MEM_REG;               
   memInfo.pool   = CU_POOL;

   ret = cmInetSctpSendMsg(&f1Params.sockFd, &f1Params.destIpNetAddr, f1Params.destPort, &memInfo, mBuf, &len, 0, FALSE, 0, 0/*SCT_PROTID_NONE*/, RWOULDBLOCK);

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
