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
#include <fcntl.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>  
#include <unistd.h>

#include "du_sctp.h"
#include "du_common.h"

/* Global variable declaration */
S8   sockFd;           /* Socket file descriptor */
U8   socket_type;      /* Socket type */
Bool nonblocking;      /* Blocking/Non-blocking socket */
Bool connUp;           /* Is connection up */
int  assocId;          /* Assoc Id of connected assoc */

sockaddr_storage_t local_addr;    /* Local Address */
sockaddr_storage_t remote_addr;   /* Remote Address */
U8   la_len;                      /* Local Address length */
U8   ra_len;                      /* Remote Address length */
F1SctpParams *sctpCfg;            /* SCTP configurations at DU */

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
   sockFd = 0;
   connUp = FALSE;
   assocId = 0;
   nonblocking = FALSE;
   sctpCfg = &(ducfgparam.sctpParams);
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

/*******************************************************************
 *
 * @brief Converts internet address to sockaddr type
 *
 * @details
 *
 *    Function : getSockAddr 
 *
 *    Functionality:
 *        Converts internet address to sockaddr type
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 getSockAddr(char *hostIp, U16 portNum, Bool ipv4Pres, Bool local)
{
   sockaddr_storage_t address;
   struct hostent *host;
   void *la_raw;

   /* Getting the transport address for local host name */
   if(ipv4Pres)
   {
      host = gethostbyname(hostIp);
      if (host == NULL || host->h_length < 1)
      {
         printf("\nBad hostname: %s", hostIp);
         RETVALUE(NULLP);
      }
      la_raw = &address.v4.sin_addr;
      address.v4.sin_family = AF_INET;
      address.v4.sin_port = htons(portNum);
   }
   else
   {
      host = gethostbyname2(hostIp, AF_INET6);
      if (host == NULL || host->h_length < 1)
      {
         printf("\n Bad hostname: %s", hostIp);
         RETVALUE(RFAILED);
      }
      la_raw = &address.v6.sin6_addr;
      address.v6.sin6_family = AF_INET6;
      address.v6.sin6_port = htons(portNum);
      address.v6.sin6_scope_id = 0;
   }

   memcpy((U8 *)la_raw, (U8 *)host->h_addr_list[0], host->h_length);
   
   if(local)
   {
      local_addr = address;
   }
   else
   {
      remote_addr = address;
   }

   RETVALUE(ROK);
} /* End of getSockAddr() */

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
  sa_family_t la_family;
  U8   error;


  /* Getting the transport address for local host name */
  if(sctpCfg->duIpAddr.ipV4Pres)
  {
     if(getSockAddr(sctpCfg->duIpAddr.ipV4Addr, sctpCfg->duPort, TRUE, TRUE) != ROK )
     {
        printf("\nUnable to get local address");
        RETVALUE(RFAILED);
     }
     la_family = AF_INET;
  }
  else
  {
     if(getSockAddr(sctpCfg->duIpAddr.ipV6Addr, sctpCfg->duPort, FALSE, TRUE) != ROK )
     {
	printf("\nUnable to get local address");
        RETVALUE(RFAILED);
     }
     la_family = AF_INET6;
  }

  socket_type = SOCK_STREAM;
  
  /* Creating new end point */
  sockFd = socket(la_family, socket_type, IPPROTO_SCTP);  
  if (sockFd < 0) 
  {
     printf("\n Failed to create socket  %s", strerror(errno));
     RETVALUE(RFAILED);
  }

  /* Binding socket to local address and port */
  error = bind(sockFd, &local_addr.sa, la_len); 
  if(error != 0)
  {
     printf("\n Failed to bind to socket. Error [%s]", strerror(errno));
     RETVALUE(RFAILED);
  }

  /* Setting socket as non-blocking*/
  error = fcntl(sockFd, F_SETFL, O_NONBLOCK);
  if (error != 0) 
  {
     printf("\n Failed to set socket as non blocking. Error [%s]", strerror(errno));
     RETVALUE(RFAILED);
  }
  else
  {
     nonblocking = TRUE;
  }

  RETVALUE(ROK);

} /* End of openSctpEndp() */

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
   U8  error;
   U8  ret;

   /* Getting the transport address for remote host name */
   if(sctpCfg->cuIpAddr.ipV4Pres)
   {
      ret = getSockAddr(sctpCfg->cuIpAddr.ipV4Addr, sctpCfg->cuPort, TRUE, FALSE);
   }
   else
   {
      ret = getSockAddr(sctpCfg->cuIpAddr.ipV6Addr, sctpCfg->cuPort, FALSE, FALSE);
   }
   if(ret != ROK)
   {
      printf("\nUnable to get remote address");
      RETVALUE(RFAILED);
   }

   /* Initiating connection establishment with remote */
   if(!connUp)
   {
      error = sctp_connectx(sockFd, &remote_addr.sa, 1, &assocId);
      if(error != 0)
      {
         printf("\nError connecting to peer. Error[%s]", strerror(errno));
         RETVALUE(RFAILED);
      }
      else
      {
         /* Mark SCTP connection UP */
         connUp = TRUE;
      }
   }

   RETVALUE(ROK);
}/* End of sctpConnect() */

/*******************************************************************
 *
 * @brief Handles an incoming message
 *
 * @details
 *
 *    Function : sctpInmsgHdlr
 *
 *    Functionality:
 *      Handles an incoming message
 *
 * @params[in] Socket file descriptor
 *             Incoming message header
 *             Message Length
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 sctpInmsgHdlr(struct msghdr *msg, size_t msgLen)
{
   sctp_cmsg_data_t *data;
   struct cmsghdr *cmsg;
   union sctp_notification *sn;
   Buffer *mBuf;
   Pst    *pst;

   for (cmsg = CMSG_FIRSTHDR(msg); cmsg != NULL; cmsg = CMSG_NXTHDR(msg, cmsg)) 
   {
      data = (sctp_cmsg_data_t *)CMSG_DATA(cmsg);
   }

   printf("\nReceived Message");

   /* if incoming message is data */
   if (!(MSG_NOTIFICATION & msg->msg_flags)) 
   {
      /* Extract message */
      U8 index = 0;
      char *recvBuf;
      char *temp = recvBuf;
      U8 len = msgLen;
      U8 temp_len;
      U8 i;
    
      while( msgLen > 0)
      {
         temp = msg->msg_iov[index].iov_base;
         temp_len = msg->msg_iov[index].iov_len;
        
         if(temp_len > msgLen) 
         {
            temp[(temp_len = msgLen) - 1] = '\0';
         }
         
         if((msgLen -= temp_len) > 0) 
         { 
           index++;
         }
         for (i = 0; i < temp_len-1 ; ++i) 
         {
            if (!isprint(temp[i])) 
               temp[i] = '.';
         }
         printf("\nPrinting temp %s", temp);
         temp = temp + temp_len;
         index++;
      }
      recvBuf[len - 1] = '\0';
      printf("\n Message: %s temp %s", recvBuf, temp);

      /* Post received message to du_App */
      if(SGetMsg(0, 0, &mBuf) == ROK )
      {
         if(SAddPstMsgMult((Data *)recvBuf, len, mBuf) == ROK)
         {
            pst->dstProcId;              
            pst->srcProcId;              
            pst->dstEnt = ENTDUAPP;                 
            pst->dstInst = 0;                
            pst->srcEnt = ENTSCTP;                 
            pst->srcInst = 0;                
            pst->prior;                  
            pst->route;                  
            pst->event = EVTSCTPUP;                 
            pst->region = 0;               
            pst->pool = 0;                
            pst->selector = 0;           
            pst->intfVer;          

            SPstTsk(pst, mBuf);
         }
      }


   }
   else /* If the incoming message is notification */
   {
      /* Extract and perform necessary action 
         Change the connUp state accordingly */
      union sctp_notification *notify;
      notify = (union sctp_notification *)msg->msg_iov->iov_base;
        
      if (SCTP_ASSOC_CHANGE != notify->sn_header.sn_type) 
      {
         printf("\nReceived unexpected notification: %d", notify->sn_header.sn_type);
         RETVALUE(RFAILED);
      }

      switch(notify->sn_assoc_change.sac_state)
      {
         case SCTP_COMM_UP:
                printf("Received SCTP_COMM_UP\n");
                break;
         case SCTP_COMM_LOST:
                printf("Received SCTP_COMM_LOST\n");
                break;
         case SCTP_RESTART:
                printf("Received SCTP_RESTART\n");
                break;
         case SCTP_SHUTDOWN_COMP:
                printf("Received SCTP_SHUTDOWN_COMP\n");
                break;
         case SCTP_CANT_STR_ASSOC:
                printf("Received SCTP_CANT_STR_ASSOC\n");
                break;
      }
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
   U8   error;
   struct msghdr inmessage;
   struct iovec iov;
   char incmsg[CMSG_SPACE(sizeof(_sctp_cmsg_data_t))];
   sockaddr_storage_t msgname;

   /* Initialize inmessage with enough space for DATA... */
   memset(&inmessage, 0, sizeof(inmessage));
   if ((iov.iov_base = malloc(REALLY_BIG)) == NULL) 
   {
      printf("\n malloc not enough memory!!!");
      close(sockFd);
      connUp = FALSE;
      RETVALUE(RFAILED);
   }
   iov.iov_len = REALLY_BIG;
   inmessage.msg_iov = &iov;
   inmessage.msg_iovlen = 1;
   /* or a control message.  */
   inmessage.msg_control = incmsg;
   inmessage.msg_controllen = sizeof(incmsg);
   inmessage.msg_name = &msgname;
   inmessage.msg_namelen = sizeof(msgname);
    
   while (connUp) 
   {
      error = recvmsg(sockFd, &inmessage, MSG_WAITALL);
      if (error < 0) 
      {
         if (nonblocking && (EAGAIN == errno)) 
         {
            error = 0;
            continue;
         }
         if (socket_type == SOCK_STREAM) 
         {
            if (ENOTCONN != errno)
            {
               break;
            }
            printf("No association is present now!!\n");
            close(sockFd);
            sockFd = 0;
            connUp = FALSE;
          }
          break; 
      }
      
      sctpInmsgHdlr(&inmessage, error);

      inmessage.msg_control = incmsg;
      inmessage.msg_controllen = sizeof(incmsg);
      inmessage.msg_name = &msgname;
      inmessage.msg_namelen = sizeof(msgname);
      iov.iov_len = REALLY_BIG;   

   }/* End of while(connUp) */
   
   RETVALUE(ROK);
}/* End of sctpSockPoll() */

/*******************************************************************
 *
 * @brief Send message on SCTP socket
 *
 * @details
 *
 *    Function : sctpOutMsgSend 
 *
 *    Functionality:
 *        Send message on SCTP socket
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 sctpOutMsgSend(char *message, U8 msglen)
{
   struct msghdr outmsg;
   struct iovec iov;
   int error = 0;

   do{
       if(connUp && msglen != 0)
       {
          iov.iov_base = message;
          iov.iov_len = msglen;
   
          outmsg.msg_iov = &iov;
          outmsg.msg_iovlen = 1;
          outmsg.msg_control = NULL;
          outmsg.msg_controllen = 0;
          outmsg.msg_name = &remote_addr;
          outmsg.msg_namelen = ra_len;
          outmsg.msg_flags = 0;
 
          error = sendmsg(sockFd, &outmsg, 0);
        }
        
        if(error != msglen)
        {
           if(nonblocking && EAGAIN == errno) 
           {
              continue;
           }
           else
           {
              printf("\n Error [%s] while sending message on SCTP assoc", strerror(errno));
              RETVALUE(RFAILED);
           }
        }        
        else
        {
           break;
        }
   }while(error != msglen);

   RETVALUE(ROK);

} /* End of sctpOutMsgSend */

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
   /* Open SCTP socket and bind to local address */
   if(openSctpEndp() != ROK)
   {
      printf("\nFailed while opening SCTP endpoint");
      if(sockFd > 0 )
      {
         close(sockFd);
         sockFd = 0;
      }
      /* TODO : Send Assoc establishment failure to du_app if needed*/
   }
   else if(sctpConnect() != ROK) /* send connection request */
   {
      printf("\nFailed while connecting to peer");
      close(sockFd);
      sockFd = 0;
      assocId = 0;
      nonblocking = FALSE;
      /* TODO : Send Assoc establishment failure to du_app */
   }
   else
   {
      /* Send AssocCfm to du_app */
      if(sctpSockPoll() != ROK)
      {
         printf("\nFailed while polling");
         /* Send failure to du_app */
      }
   }
} /* End of sctpAssocReq */

/**********************************************************************
         End of file
**********************************************************************/
