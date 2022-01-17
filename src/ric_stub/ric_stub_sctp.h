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

#ifndef __RIC_SCTP_H__
#define __RIC_SCTP_H__
#define MAX_RETRY 5

/* Global variable declaration */
uint8_t   socket_type;      /* Socket type */
  
typedef struct
{
   uint16_t           numFd;
   uint16_t           port;   
   uint32_t           flag;
   Buffer            *mBuf;
   MsgLen             bufLen; 
   CmInetNetAddr      addr;
   CmInetFdSet        readFd;
   CmInetSctpSndRcvInfo   info;
   CmInetSctpNotification ntfy;
}sctpSockPollParams;

typedef struct
{
   uint16_t              destPort;         /* DU PORTS */
   uint16_t              srcPort;
   bool                  bReadFdSet;
   CmInetFd              sockFd;           /* Socket file descriptor */
   CmInetAddr            peerAddr;
   CmInetFd              lstnSockFd;       /* Listening Socket file descriptor */
   CmInetNetAddrLst      destAddrLst;      /* DU Ip address */
   CmInetNetAddrLst      localAddrLst;
   CmInetNetAddr         destIpNetAddr;    /* DU Ip address */ 
   Bool                  connUp;
}RicSctpDestCb;

typedef struct sctpGlobalCb
{
   RicSctpParams sctpCfg;
   uint8_t      numDu;
   RicSctpDestCb destCb[MAX_DU_SUPPORTED]; 
}SctpGlobalCb;

SctpGlobalCb sctpCb;

uint8_t sctpActvInit();
uint8_t sctpSend(Buffer *mBuf);
uint8_t sctpCfgReq();
uint8_t sctpStartReq();
uint8_t sctpSetSockOpts(CmInetFd *sock_Fd);
uint8_t sctpAccept(RicSctpDestCb *destCb);
uint8_t sctpSockPoll();
uint8_t processPolling(sctpSockPollParams *pollParams, RicSctpDestCb *destCb, uint32_t *timeoutPtr, CmInetMemInfo *memInfo);
uint8_t fillAddrLst(CmInetNetAddrLst *addrLstPtr, SctpIpAddr *ipAddr);
uint8_t fillDestNetAddr(CmInetNetAddr *destAddrPtr, SctpIpAddr *dstIpPtr);

#endif

/**********************************************************************
         End of file
 **********************************************************************/
