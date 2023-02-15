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
#define MAX_DU_SUPPORTED 2
#define MAX_REMOTE_CU_SUPPORTED 1
#define MAX_ASSOC_SUPPORTED (MAX_DU_SUPPORTED + MAX_REMOTE_CU_SUPPORTED)

/* Global variable declaration */
uint8_t   socket_type;      /* Socket type */

typedef enum
{
   F1_INTERFACE,
   XN_INTERFACE
}InterfaceType;

typedef enum
{
   SERVER,
   CLIENT
}NodeType;

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
   InterfaceType    intf;             /* F1 or Xn Interface */
   uint32_t         destId;           /* For F1 interface, this is DU ID. For Xn, this is remote CU ID */
   uint16_t         destPort;         /* DU PORTS */
   Bool             bReadFdSet;
   CmInetFd         sockFd;           /* Socket file descriptor */
   CmInetAddr       peerAddr;
   CmInetNetAddrLst destAddrLst;      /* Remote IP address list */
   CmInetNetAddr    destIpNetAddr;    /* Remote IP network address */ 
   Bool             connUp;           /* Is connection up */
}CuSctpAssocCb;

typedef struct ipAddr
{
 Bool      ipV4Pres;
 uint32_t  ipV4Addr;
 Bool      ipV6Pres;
 uint8_t   ipV6Addr[MAX_IPV6_LEN];
}SctpIpAddr;

typedef struct sctpDestInfo
{
   SctpIpAddr  destIpAddr;
   uint16_t    destPort;
}SctpDestInfo;

typedef struct sctpCfgPerIntf
{
   uint16_t       port;
   NodeType       localNodeType; /* Local node acts as Server or client while establishing SCTP assoc */
   uint8_t        numDestNode; 
   SctpDestInfo   destCb[MAX_ASSOC_SUPPORTED];
}SctpCfgPerIntf;

typedef struct cuSctpParams
{
   SctpIpAddr     localIpAddr;
   SctpCfgPerIntf f1SctpInfo;
   SctpCfgPerIntf xnSctpInfo;
}CuSctpParams;

typedef struct
{
   CuSctpParams     sctpCfg;
   CmInetNetAddrLst localAddrLst;
   CmInetFd         f1LstnSockFd;       /* Listening Socket file descriptor for F1 association */
   CmInetFd         xnLstnSockFd;       /* Listening Socket file descriptor for Xn association */
   NodeType         localXnNodeType;    /* Local node acts as Server or client while establishing SCTP assoc at Xn interface */
   uint8_t          numAssoc;
   CuSctpAssocCb    assocCb[MAX_ASSOC_SUPPORTED];
}SctpGlobalCb;

SctpGlobalCb sctpCb;

uint8_t sctpActvInit();
uint8_t sctpStartReq();
uint8_t sctpSend(InterfaceType intf, uint32_t destId, Buffer *mBuf);
uint8_t sctpCfgReq();

uint8_t fillAddrLst(CmInetNetAddrLst *addrLstPtr, SctpIpAddr *ipAddr);
uint8_t fillDestNetAddr(CmInetNetAddr *destAddrPtr, SctpIpAddr *dstIpPtr);
uint8_t sctpSetSockOpts(CmInetFd *sock_Fd);
uint8_t sctpSockPoll();
uint8_t sctpAccept(CmInetFd *lstnSockFd, CuSctpAssocCb *assocCb);
uint8_t processPolling(sctpSockPollParams *pollParams, CuSctpAssocCb *assocCb, uint32_t *timeoutPtr, CmInetMemInfo *memInfo);
#endif

/**********************************************************************
         End of file
 **********************************************************************/
