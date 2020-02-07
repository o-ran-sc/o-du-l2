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

#include <stdio.h> 
#include "du_sctp.h"
#include "lsctp.h"

/* Global variable declaration */
F1SctpParams sctpCfg;            /* SCTP configurations at DU */

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
S16 sctpActvInit(Ent entity, Inst inst, Region region, Reason reason)
{
   DU_LOG("\n\nSCTP : Initializing");
   SSetProcId(DU_PROC);
   connUp = FALSE;
   assocId = 0;
   nonblocking = FALSE;
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
   switch(pst->srcEnt)
   {
      case ENTDUAPP:
         {
            switch(pst->event)
            {
               case EVTSCTPSTRT:
               {
                  cmUnpkSctpAssocReq(mBuf);
                  SPutMsg(mBuf);
                  sctpAssocReq();
                  break;
               }
            }
            break;
         }
   }
   SExitTsk();
   return ROK;
}

/*******************************************************************
 *
 * @brief Opens a non-blocking socket and binds to local address
 *
 * @details
 *
 *    Function : openSctpEndp
 *
 *    Functionality:
 *         Opens a non-blocking socket and binds to local address
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 openSctpEndp()
{
   U8 ret;
   U8 numRetry = 0;

   /* Opening a non-blocking SCTP socket */
   socket_type = CM_INET_STREAM;

   do{
      ret = cmInetSocket(socket_type, &sockFd, IPPROTO_SCTP);
      if (ret != ROK)
      {
         numRetry++;
         if(numRetry >= MAX_RETRY)
         {
            DU_LOG("\nSCTP : All attempts to open socket failed.");
            /* Send indication to du_app */
            RETVALUE(RFAILED);
         }
         else
         {
            DU_LOG("\nSCTP : Retrying socket opening"); 
         }
      }
      else
      {
         DU_LOG("\nSCTP : Socket[%d] open",sockFd.fd);
         break;
      }
   }while(numRetry < MAX_RETRY);

   RETVALUE(ROK);
} /* End of openSctpEndp */


/*******************************************************************
 *
 * @brief Bind socket to local Ip address and port
 *
 * @details
 *
 *    Function : bindSctpEndp
 *
 *    Functionality:
 *      -Bind socket to local Ip address and port
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 bindSctpEndp()
{

   U8 ret;
   U8 numRetry = 0;

   /* Binding the socket with local address */
   localAddrLst.count = 1;
   if(sctpCfg.duIpAddr.ipV4Pres)
   {
      localAddrLst.addrs[0].type = CM_INET_IPV4ADDR_TYPE;
      localAddrLst.addrs[0].u.ipv4NetAddr = CM_INET_NTOH_U32(sctpCfg.duIpAddr.ipV4Addr);
   }
   else
   {
      localAddrLst.addrs[0].type = CM_INET_IPV4ADDR_TYPE;
      localAddrLst.addrs[0].u.ipv4NetAddr = 0;
   } 

   do{
      ret = cmInetSctpBindx(&sockFd, &localAddrLst, sctpCfg.duPort);
      if (ret != ROK)
      {
         numRetry++;
         if(numRetry >= MAX_RETRY)
         {
            DU_LOG("\nSCTP : All attempts to bind socket failed.");
            cmInetClose(&sockFd);
            /* Send indication to du_app */
            RETVALUE(RFAILED);
         }
         else
         {
            DU_LOG("\nSCTP : Retrying socket binding");
         }
      }
      else
      {
         DU_LOG("\nSCTP : Socket bind successful");
         break;
      }
   }while(numRetry < MAX_RETRY);

  RETVALUE(ROK);

} /* End of bindSctpEndp() */

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
S16 sctpSetSockOpts()
{
   CmSctpEvent sctpEvent;

   sctpEvent.dataIoEvent          = TRUE;
   sctpEvent.associationEvent     = TRUE;
   sctpEvent.addressEvent         = TRUE;
   sctpEvent.sendFailureEvent     = TRUE;
   sctpEvent.peerErrorEvent       = TRUE;
   sctpEvent.shutdownEvent        = TRUE;
   sctpEvent.partialDeliveryEvent = TRUE;
   sctpEvent.adaptationLayerEvent = TRUE;

   cmInetSetOpt(&sockFd, CM_SOCKOPT_LEVEL_SCTP, CM_SOCKOPT_OPT_SCTP_EVENTS, &sctpEvent);
   RETVALUE(ROK);

   RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief Initiates connection with peer SCTP
 *
 * @details
 *
 *    Function : sctpConnect
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
S16 sctpConnect()
{
   U8  ret;
   U8  numRetry = 0;
   CmInetNetAddr  primDstAddr; /* primary destination address */

   /* Filling primary destination address */
   if(sctpCfg.cuIpAddr.ipV4Pres)
   {
      primDstAddr.type = CM_INET_IPV4ADDR_TYPE;
      primDstAddr.u.ipv4NetAddr = CM_INET_NTOH_U32(sctpCfg.cuIpAddr.ipV4Addr);
   }
   else
   {
      primDstAddr.type = CM_INET_IPV4ADDR_TYPE;
      primDstAddr.u.ipv4NetAddr = 0;
   }

   /* Filling destination address list */
   remoteAddrLst.count = 1;
   if(sctpCfg.cuIpAddr.ipV4Pres)
   {
      remoteAddrLst.addrs[0].type = CM_INET_IPV4ADDR_TYPE;
      remoteAddrLst.addrs[0].u.ipv4NetAddr = CM_INET_NTOH_U32(sctpCfg.cuIpAddr.ipV4Addr);
   }
   else
   {
      remoteAddrLst.addrs[0].type = CM_INET_IPV4ADDR_TYPE;
      remoteAddrLst.addrs[0].u.ipv4NetAddr = 0;
   }

   /* Sending connect request to remote */
   do{
      ret = cmInetSctpConnectx(&sockFd, &primDstAddr, &remoteAddrLst, sctpCfg.cuPort);
      if (ret == RFAILED || ret == ROKDNA || ret == RCLOSED)
      {
         numRetry++;
         if(numRetry >= MAX_RETRY)
         {
            DU_LOG("\nSCTP : All attempts to connect failed.");
            cmInetClose(&sockFd);
            /* Send indication to du_app */
            RETVALUE(RFAILED);
         }
         else
         {
            DU_LOG("\nSCTP : Retrying connection");
         }
      }
      else if(ret == RINPROGRESS)
      {
         DU_LOG("\nSCTP : Connection in progess");
         break;
      }
      else
      {
         connUp = TRUE;
         DU_LOG("\nSCTP : Connect successful");
         break;
      }
   }while(numRetry < MAX_RETRY);

   RETVALUE(ROK);
}/* End of sctpConnect() */

/*******************************************************************
 *
 * @brief Post received data/notification to DU APP 
 *
 * @details
 *
 *    Function : sendToDuApp 
 *
 *    Functionality:
 *         Post received data/notification to DU APP
 *
 * @params[in]  Message buffer
 *              Message event
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void sendToDuApp(Buffer *mBuf, Event event)
{
   Pst pst;
   DU_LOG("\nSCTP : Forwarding received message to duApp");
   SPrntMsg(mBuf, 0, 0);


   cmMemset((U8 *)&(pst), 0, sizeof(Pst));
   pst.srcEnt = (Ent)ENTSCTP;
   pst.srcInst = (Inst)SCTP_INST;
   pst.srcProcId = DU_PROC;
   pst.dstEnt = (Ent)ENTDUAPP;
   pst.dstInst = (Inst)DU_INST;
   pst.dstProcId = pst.srcProcId;
   pst.event = event;
   pst.selector = DU_SELECTOR_LC;
   pst.pool= DU_POOL;
   pst.region = DFLT_REGION;

   if (SPstTsk(&pst, mBuf) != ROK)
   {
      DU_LOG("\nSCTP : SPstTsk failed in duReadCfg");
   }
}

/*******************************************************************
 *
 * @brief Handles an SCTP notification message
 *
 * @Sending *
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
   Pst pst;

   switch(ntfy->header.nType)
   {
      case CM_INET_SCTP_ASSOC_CHANGE :
         DU_LOG("\nSCTP : Assoc change notification received");
         switch(ntfy->u.assocChange.state)
         {
            case CM_INET_SCTP_COMM_UP:
               DU_LOG("Event : COMMUNICATION UP");
               connUp = TRUE;
               break;
            case CM_INET_SCTP_COMM_LOST:
               DU_LOG("Event : COMMUNICATION LOST");
               connUp = FALSE;
               break;
            case CM_INET_SCTP_RESTART:
               DU_LOG("Event : SCTP RESTART");
               connUp = FALSE;
               break;
            case CM_INET_SCTP_SHUTDOWN_COMP: /* association gracefully shutdown */
               DU_LOG("Event : SHUTDOWN COMPLETE");
               connUp = FALSE;
               break;
            case CM_INET_SCTP_CANT_STR_ASSOC:
               DU_LOG("Event : CANT START ASSOC");
               connUp = FALSE;
               break;
            default:
               DU_LOG("\nInvalid event");
               break;
         }
         break;
      case CM_INET_SCTP_PEER_ADDR_CHANGE :
         DU_LOG("\nSCTP : Peer Address Change notificarion received");
         /* Need to add handler */
         break;
      case CM_INET_SCTP_REMOTE_ERROR :
         DU_LOG("\nSCTP : Remote Error notification received");
         break;
      case CM_INET_SCTP_SEND_FAILED :
         DU_LOG("\nSCTP : Send Failed notification received\n");
         break;
      case CM_INET_SCTP_SHUTDOWN_EVENT : /* peer socket gracefully closed */
         DU_LOG("\nSCTP : Shutdown Event notification received\n");
         connUp = FALSE;
         break;
      case CM_INET_SCTP_ADAPTATION_INDICATION :
         DU_LOG("\nSCTP : Adaptation Indication received\n");
         break;
      case CM_INET_SCTP_PARTIAL_DELIVERY_EVENT:
         DU_LOG("\nSCTP : Partial Delivery Event received\n");
         break;
      default:
         DU_LOG("\nSCTP : Invalid sctp notification type\n");
         break;
   }

   /* Pack notification and send to APP */
   DU_LOG("\nSCTP : Forwarding received message to duApp");
    
   cmMemset((U8 *)&(pst), 0, sizeof(Pst));
   pst.srcEnt = (Ent)ENTSCTP;
   pst.srcInst = (Inst)SCTP_INST;
   pst.srcProcId = DU_PROC;
   pst.dstEnt = (Ent)ENTDUAPP;
   pst.dstInst = (Inst)DU_INST;
   pst.dstProcId = pst.srcProcId;
   pst.event = EVTSCTPNTFY;
   pst.selector = DU_SELECTOR_LC;
   pst.pool= DU_POOL;
   pst.region = DFLT_REGION;

   if(cmPkSctpNtfy(&pst, ntfy) != ROK)
   {
      DU_LOG("\nSCTP : Failed to pack SCTP notification");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

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
   U8   ret;
   S16  numFds;
   CmInetFdSet   readFd;
   U16           port;
   U32           timeout;           /* timeout for cmInetSelect() */
   U32           *timeoutPtr;        /* pointer to timeout */
   U32            flag;
   Buffer        *mBuf;
   MsgLen        bufLen;
   CmInetMemInfo memInfo;      /* buffer allocation info */
   CmInetNetAddr addr;
   CmInetSctpSndRcvInfo   info;
   CmInetSctpNotification ntfy;

   if (sockFd.blocking)
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
   memInfo.region = DU_APP_MEM_REGION;
   memInfo.pool   = DU_POOL;
   CM_INET_FD_ZERO(&readFd);

   while(1)
   {
      CM_INET_FD_SET(&sockFd, &readFd);
      ret = cmInetSelect(&readFd, NULLP, timeoutPtr, &numFds);
      if (CM_INET_FD_ISSET(&sockFd, &readFd))
      {
         CM_INET_FD_CLR(&sockFd, &readFd);
         ret = cmInetSctpRecvMsg(&sockFd, &addr, &port, &memInfo, &mBuf, &bufLen, &info, &flag, &ntfy);
         if (connUp && ret != ROK)
         {
            DU_LOG("\nFailed to receive sctp msg\n");
         }
         else
         {
            /* If SCTP notification received */
            if ((flag & CM_INET_SCTP_MSG_NOTIFICATION) != 0)
            {
               ret = sctpNtfyHdlr(&ntfy);
               if(ret != ROK)
               {
                  DU_LOG("\nSCTP : Failed to process sctp notify msg\n");
               }
            }
            else if(connUp) /* If data received */
            {
               sendToDuApp(mBuf, EVTSCTPDATA);
            }
            else
            {
               SPutMsg(mBuf);
            }
         }
      }
   };

   RETVALUE(ROK);
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
   U8               ret;
   MsgLen           len;          /* number of actually sent octets */
   CmInetNetAddr    peerAddr;     /* destination port address */
   CmInetNetAddr    *dstAddr;
   CmInetMemInfo    memInfo;                        
   
   memInfo.region = DU_APP_MEM_REGION;               
   memInfo.pool   = DU_POOL;


   if(sctpCfg.cuIpAddr.ipV4Pres)
   {
      peerAddr.type = CM_INET_IPV4ADDR_TYPE;
      peerAddr.u.ipv4NetAddr = CM_INET_NTOH_U32(sctpCfg.cuIpAddr.ipV4Addr);
      dstAddr = &peerAddr;
   }
   else
   {
      dstAddr = NULLP;
   }

   ret = cmInetSctpSendMsg(&sockFd, dstAddr, sctpCfg.cuPort, &memInfo, mBuf, &len, 0, FALSE, 0, 0/*SCT_PROTID_NONE*/, RWOULDBLOCK);
   if(ret != ROK && ret != RWOULDBLOCK)
   {
      DU_LOG("\nSCTP : Failed sending the message");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* End of sctpSend */

/*******************************************************************
 *
 * @brief SCTP Assoc establishment request from DU
 *
 * @details
 *
 *    Function : sctpAssocReq
 *
 *    Functionality:
 *          This function opens a socket at DU and
 *          intiates SCTP connection.
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void sctpAssocReq()
{
   if(openSctpEndp() != ROK)
   {
      DU_LOG("\nSCTP : Failed while opening socket");
   }
   else if(bindSctpEndp() != ROK)
   {
      DU_LOG("\nSCTP : Failed while binding socket");
   }
   else if(sctpSetSockOpts() != ROK)
   {
      DU_LOG("\nSCTP : Failed while setting socket options");
   }
   else if(sctpConnect() != ROK)
   {
      DU_LOG("\nSCTP : Failed while connecting to peer");
   }
   else if(sctpSockPoll() != ROK)
   {
      DU_LOG("\nSCTP : Failed while polling");
   }
} /* End of sctpAssocReq */

/**********************************************************************
         End of file
**********************************************************************/
