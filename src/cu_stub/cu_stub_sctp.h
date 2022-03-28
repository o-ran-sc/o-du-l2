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

#define MAX_RETRY 5
#define MAX_IPV6_LEN 16
#define MAX_DU_SUPPORTED 1

/* Global variable declaration */
uint8_t   socket_type;      /* Socket type */

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
   uint32_t         duId;
   uint16_t         destPort;         /* DU PORTS */
   uint16_t         srcPort;
   Bool             bReadFdSet;
   CmInetFd         sockFd;           /* Socket file descriptor */
   CmInetAddr       peerAddr;
   CmInetFd         lstnSockFd;       /* Listening Socket file descriptor */
   CmInetNetAddrLst destAddrLst;      /* DU Ip address */
   CmInetNetAddrLst localAddrLst;
   CmInetNetAddr    destIpNetAddr;    /* DU Ip address */ 
   Bool             connUp;           /* Is connection up */
}CuSctpDestCb;

typedef struct ipAddr
{
 Bool      ipV4Pres;
 uint32_t  ipV4Addr;
 Bool      ipV6Pres;
 uint8_t   ipV6Addr[MAX_IPV6_LEN];
}SctpIpAddr;

typedef struct sctpAssocInfo
{
   SctpIpAddr  duIpAddr;
   uint16_t    duPort;
   SctpIpAddr  cuIpAddr;
   uint16_t    cuPort;
}SctpAssocInfo;

typedef struct cuSctpParams
{
   uint8_t        numDu;
   SctpAssocInfo  sctpAssoc[MAX_DU_SUPPORTED];
}CuSctpParams;

typedef struct
{
   CuSctpParams sctpCfg;
   uint8_t      numDu;
   CuSctpDestCb destCb[MAX_DU_SUPPORTED]; 
}SctpGlobalCb;

SctpGlobalCb sctpCb;

uint8_t sctpActvInit();
uint8_t sctpStartReq();
uint8_t sctpSend(uint32_t duId, Buffer *mBuf);
uint8_t sctpCfgReq();

uint8_t fillAddrLst(CmInetNetAddrLst *addrLstPtr, SctpIpAddr *ipAddr);
uint8_t fillDestNetAddr(CmInetNetAddr *destAddrPtr, SctpIpAddr *dstIpPtr);
uint8_t sctpSetSockOpts(CmInetFd *sock_Fd);
uint8_t sctpSockPoll();
uint8_t sctpAccept(CuSctpDestCb *destCb);
uint8_t processPolling(sctpSockPollParams *pollParams, CuSctpDestCb *destCb, uint32_t *timeoutPtr, CmInetMemInfo *memInfo);
#endif

/**********************************************************************
         End of file
 **********************************************************************/
