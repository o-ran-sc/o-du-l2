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

#ifndef __CU_SCTP_H__
#define __CU_SCTP_H__

#include "cu_stub.h"
#include "cm_inet.h"
#include "cm_tpt.h"

#include "cm_inet.x"
#include "cm_tpt.x"

#define MAX_RETRY 5

/* Global variable declaration */
uint8_t   socket_type;      /* Socket type */
Bool nonblocking;      /* Blocking/Non-blocking socket */
Bool connUp;           /* Is connection up */
int  assocId;          /* Assoc Id of connected assoc */
  
CuSctpParams sctpCfg;            /* SCTP configurations at DU */

typedef struct
{
   S16              numFd;
   uint16_t         port;   
   uint32_t         flag;
   Buffer           *mBuf;
   MsgLen           bufLen; 
   CmInetNetAddr    addr;
   CmInetFdSet      readFd;
   CmInetSctpSndRcvInfo   info;
   CmInetSctpNotification ntfy;
}sctpSockPollParams;

typedef struct
{
   uint16_t         destPort;         /* DU PORTS */
   uint16_t         srcPort;
   Bool             bReadFdSet;
   CmInetFd         sockFd;           /* Socket file descriptor */
   CmInetAddr       peerAddr;
   CmInetFd         lstnSockFd;       /* Listening Socket file descriptor */
   CmInetNetAddrLst destAddrLst;      /* DU Ip address */
   CmInetNetAddrLst localAddrLst;
   CmInetNetAddr    destIpNetAddr;    /* DU Ip address */ 
}CuSctpDestCb;

S16 sctpActvInit();
S16 sctpStartReq();
S16 sctpSend(Buffer *mBuf);
S16 sctpCfgReq();

S16 fillAddrLst(CmInetNetAddrLst *addrLstPtr, SctpIpAddr *ipAddr);
S16 fillDestNetAddr(CmInetNetAddr *destAddrPtr, SctpIpAddr *dstIpPtr);
S16 sctpSetSockOpts(CmInetFd *sock_Fd);
S16 sctpSockPoll();
S16 sctpAccept(CmInetFd *lstnSock_Fd, CmInetAddr *peerAddr, CmInetFd *sock_Fd);
S16 processPolling(sctpSockPollParams *pollParams, CmInetFd *sockFd, uint32_t *timeoutPtr, CmInetMemInfo *memInfo);
#endif

/**********************************************************************
         End of file
 **********************************************************************/
