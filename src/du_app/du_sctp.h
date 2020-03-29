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

#ifndef __DU_SCTP_H__
#define __DU_SCTP_H__

#include "du_mgr.h"
#include "du_log.h"
#include "cm_inet.h"
#include "cm_tpt.h"

#include "cm_inet.x"
#include "cm_tpt.x"

#define DU_SCTP_DOWN 0
#define DU_SCTP_UP 1
#define DU_SCTP_CONNECTING 2
#define MAX_RETRY 5

/* Global variable declaration */
U8   socket_type;      /* Socket type */
Bool nonblocking;      /* Blocking/Non-blocking socket */
Bool connUp;           /* Is connection up */
Bool pollingState; 
CmInetNetAddrLst localAddrLst;
CmInetNetAddrLst remoteAddrLst;

typedef struct
{
   S16           numFds;            /* Total count number of receivers socket Fd */
   U16           port;              /* Filled by the InetSctpRecvMsg during polling */              
   U32           flag;              /* Refers to the notifyHandler Flag during successful InetSctpRecvMsg*/
   Buffer        *mBuf;             /* Buffer filled during Socket polling*/
   MsgLen        bufLen;            /* Determines the length of the Buffer filled during socket polling */
   CmInetNetAddr addr;              /* Determines the destination port filled during socket polling */
   CmInetFdSet      readFd;         /* Refers to the Read Fd socket */
   CmInetSctpSndRcvInfo   info;     /* Refers to Send Receivers Info*/
   CmInetSctpNotification ntfy;     /* Refers to the sctp Notification Message */
}sctpSockPollParams;

typedef struct
{
   U8               itfState;         /* determines the interface State*/
   U16              destPort;         /* Refers to the destination port of F1/E2 */
   U16              srcPort;          /* Refers to respective src port of DU */
   U32              assocId;          
   Bool             recvMsgSet;
   F1IpAddr         destIpAddr;       /* Refers to te destination Ip Address */
   CmInetFd         sockFd;           /* Socket file descriptor */
   CmInetNetAddrLst destAddrLst;      /* Refers to the destinaiton Addr Lst in CmInetNetAddrLst format */
   CmInetNetAddr    destIpNetAddr;    /* Refers to the destination Addr in CmInetNet Addr format */
}DuSctpDestCb;

/* Global variable declaration */
DuSctpDestCb f1Params;     /* SCTP configurations at DU */ 
DuSctpDestCb ricParams;    /* SCTP configurations at DU */ 


S16 sctpActvInit(Ent entity, Inst inst, Region region, Reason reason);
S16 sctpActvTsk(Pst *pst, Buffer *mBuf);
void sctpAssocReq();
void sendToDuApp(Buffer *mBuf, Event event);
S16 sctpSend(Buffer *mBuf, U8 itfType);
typedef S16 (*SctpNtfy) ARGS((Buffer *mBuf, CmInetSctpNotification *ntfy));
S16 duSctpCfgReq(SctpParams sctpCfg);
S16 fillAddrLst(CmInetNetAddrLst *addrLstPtr, F1IpAddr *ipAddr);
S16 fillDestNetAddr(CmInetNetAddr *destAddrPtr, F1IpAddr *dstIpPtr);
S16 establishReq(DuSctpDestCb *paramPtr);
S16 duSctpAssocReq(U8 itfType);
S16 duFillSctpPst(Pst *pst, Event event);
S16 sctpSetSockOpts(CmInetFd *sock_Fd);
S16 processPolling(sctpSockPollParams *pollParams, CmInetFd *sockFd, U32 *timeoutPtr, CmInetMemInfo *memInfo, Bool recvMsgSet);
S16 sctpSockPoll();

#endif

/**********************************************************************
         End of file
**********************************************************************/
