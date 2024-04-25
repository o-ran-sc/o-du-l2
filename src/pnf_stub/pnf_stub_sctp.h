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

#ifndef __PNF_SCTP_H__
#define __PNF_SCTP_H__

#define MAX_RETRY 5
#define MAX_IPV6_LEN 16
#define MAX_VNF_SUPPORTED 2
#define MAX_ASSOC_SUPPORTED MAX_VNF_SUPPORTED

/* Global variable declaration */
extern uint8_t   socket_type;      /* Socket type */

typedef struct pnfP5SctpSockPollParams
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
}PnfP5SctpSockPollParams;

typedef struct pnfP5SctpAssocCb
{
   uint32_t              vnfId;
   uint16_t              destPort;         /* VNF PORTS */
   CmInetFd              sockFd;           /* Socket file descriptor */
   CmInetAddr            peerAddr;
   CmInetNetAddr         destIpNetAddr;    /* VNF Ip address */ 
   Bool                  connUp;
}PnfP5SctpAssocCb;

typedef struct pnfP5SctpIpAddr
{
 bool      ipV4Pres;
 uint32_t  ipV4Addr;
 bool      ipV6Pres;
 uint8_t   ipV6Addr[MAX_IPV6_LEN];
}PnfP5SctpIpAddr;

typedef struct pnfP5SctpDestInfo
{
   PnfP5SctpIpAddr  destIpAddr;
   uint16_t         destPort;
}PnfP5SctpDestInfo;

typedef struct pnfP5SctpParams
{
   PnfP5SctpIpAddr     pnfP5localIpAddr;
   uint16_t            pnfP5SctpPort;
   uint8_t             numDestNode;
   PnfP5SctpDestInfo   destCb[MAX_VNF_SUPPORTED];
}PnfP5SctpParams;

typedef struct pnfP5SctpGlobalCb
{
   PnfP5SctpParams    pnfP5SctpCfg;
   CmInetNetAddrLst   localAddrLst;
   CmInetFd           pnfP5LstnSockFd;       /* Listening Socket file descriptor*/
   uint8_t            numAssoc;
   PnfP5SctpAssocCb   assocCb[MAX_ASSOC_SUPPORTED]; 
}PnfP5SctpGlobalCb;

extern PnfP5SctpGlobalCb pnfP5SctpCb;

uint8_t pnfP5SctpActvInit();
uint8_t pnfP5SctpCfgReq();
uint8_t pnfP5SctpStartReq();
uint8_t pnfP5SctpAccept(PnfP5SctpAssocCb *assocCb);
uint8_t pnfP5SctpSetSockOpts(CmInetFd *sock_Fd);
uint8_t pnfP5SctpAccept(PnfP5SctpAssocCb *assocCb);
uint8_t pnfP5SctpSockPoll();
uint8_t pnfP5ProcessPolling(PnfP5SctpSockPollParams *pollParams, PnfP5SctpAssocCb *assocCb, uint32_t *timeoutPtr, CmInetMemInfo *memInfo);
uint8_t pnfP5SctpNtfyHdlr(PnfP5SctpAssocCb *assocCb, CmInetSctpNotification *ntfy);
uint8_t pnfP5SctpSend(Buffer *mBuf);

#endif

/**********************************************************************
         End of file
 **********************************************************************/
