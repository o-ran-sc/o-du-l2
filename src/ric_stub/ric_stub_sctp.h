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
#define MAX_IPV6_LEN 16
#define MAX_DU_SUPPORTED 2
#define MAX_ASSOC_SUPPORTED MAX_DU_SUPPORTED

/* Global variable declaration */
uint8_t   socket_type;      /* Socket type */

/* O-RAN.WG3.E2AP-R003-v03.00 : Section 9.2.30 */
typedef enum
{
   RIC_SERVICE,
   SUPPORT_FUNCTIONS,
   BOTH_FUNCTIONALITY
}AssocUsage;

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
   uint32_t              duId;
   uint16_t              destPort;         /* DU PORTS */
   bool                  bReadFdSet;
   CmInetFd              sockFd;           /* Socket file descriptor */
   CmInetAddr            peerAddr;
   CmInetNetAddrLst      destAddrLst;      /* DU Ip address */
   CmInetNetAddr         destIpNetAddr;    /* DU Ip address */ 
   Bool                  connUp;
}RicSctpAssocCb;

typedef struct ipAddr
{
 bool ipV4Pres;
 uint32_t  ipV4Addr;
 bool ipV6Pres;
 uint8_t   ipV6Addr[MAX_IPV6_LEN];
}SctpIpAddr;

typedef struct sctpDestInfo
{
   SctpIpAddr  destIpAddr;
   uint16_t    destPort;
}SctpDestInfo;

typedef struct ricSctpParams
{
   SctpIpAddr     localIpAddr;
   uint16_t       e2SctpPort;
   uint8_t        numDestNode;
   SctpDestInfo   destCb[MAX_DU_SUPPORTED];
   AssocUsage     usage;
}RicSctpParams;

typedef struct sctpGlobalCb
{
   RicSctpParams    sctpCfg;
   CmInetNetAddrLst localAddrLst;
   CmInetFd         e2LstnSockFd;       /* Listening Socket file descriptor for E2 association */
   uint8_t          numAssoc;
   RicSctpAssocCb   assocCb[MAX_ASSOC_SUPPORTED]; 
}SctpGlobalCb;

SctpGlobalCb sctpCb;

uint8_t sctpActvInit();
uint8_t sctpSend(uint32_t duId, Buffer *mBuf);
uint8_t sctpCfgReq();
uint8_t sctpStartReq();
uint8_t sctpSetSockOpts(CmInetFd *sock_Fd);
uint8_t sctpAccept(RicSctpAssocCb *assocCb);
uint8_t sctpSockPoll();
uint8_t processPolling(sctpSockPollParams *pollParams, RicSctpAssocCb *destCb, uint32_t *timeoutPtr, CmInetMemInfo *memInfo);
uint8_t fillAddrLst(CmInetNetAddrLst *addrLstPtr, SctpIpAddr *ipAddr);
uint8_t fillDestNetAddr(CmInetNetAddr *destAddrPtr, SctpIpAddr *dstIpPtr);

#endif

/**********************************************************************
         End of file
 **********************************************************************/
