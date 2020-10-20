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

/********************************************************************20**
  
        Name:     common Internet socket library
    
        Type:     header file 
  
        Desc:     common library for Internet sockets
                    
        File:     cm_inet.x
  
*********************************************************************21*/
 

/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *      1000151     TCAP over TCP/IP   
 */

/* cm_inet_x_001.main_29: SS_4GMX_LCORE changes */
#ifdef SS_4GMX_LCORE
#define __CMINETX__ 1
#endif
#ifndef __CMINETX__
#define __CMINETX__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef WIN32
#ifdef SS_WINCE
#ifndef IN
#define IN
#include <winsock2.h>
#undef IN
#else /*IN*/
#include <winsock2.h>
#endif /*IN*/
#else  /*SS_WINCE*/
   /* cm_inet_x_001.main_28: Added #define IN */
#ifndef IN
#define IN
#include <winsock2.h>
#undef IN
#else /*IN*/
#include <winsock2.h>
#endif /*IN*/
#endif /* SS_WINCE */
#else
#include <string.h>
#ifdef SS_LINUX
#include <sys/select.h>
#include <sys/poll.h>
#endif /* SS_LINUX */
#include <sys/types.h>
#ifdef SS_PS
#include <pna.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif /* SS_PS */
#endif /* WIN32 */

#ifdef IPV6_SUPPORTED
#if (defined(SUNOS) || defined(HPOS)) 
#include <netinet/icmp6.h>
#endif /* SUNOS || HPOS */
#endif /* IPV6_SUPPORTED */

 /* cm_inet_x_001.main_27 : Added header file inclusion */
#if (defined(SS_VW) && defined(SS_VW6_7)) 
#include <ipcom_inet.h>
#include <ipcom_sock6.h>
#include <netinet/icmp6.h>
#endif

/* cm_inet_x_001.main_21:Added wrapper function for getaddrinfo and freeaddrinfo */
#if (!defined(SS_VW) && !defined(SS_PS) && !defined(WIN32))
#include <netdb.h>
#endif

/* Redifining the CmInetIpAddr &  CmInetIpAddr6 */
typedef CmIpAddr CmInetIpAddr;        /* 4 byte IP address */
#ifdef IPV6_SUPPORTED
typedef CmIpAddr6 CmInetIpAddr6;   /* 16 byte IPV6 address */
#endif /* IPV6_SUPPORTED */

typedef struct cmInetIpAddrTbl
{
   U16             count;           /* Number of addresses in table */
   CmInetIpAddr    netAddr[CM_INET_IPV4_NUM_ADDR];  /* Address table */
}CmInetIpAddrTbl;

typedef CmInetIpAddrTbl CmInetIpv4AddrArr;

#ifdef IPV6_SUPPORTED
typedef struct cmInetIpv6AddrArr
{
   U16             count;           /* Number of addresses in array */
   CmInetIpAddr6   netAddr[CM_INET_IPV6_NUM_ADDR];  /* Address array */
}CmInetIpv6AddrArr;
#endif /* IPV6_SUPPORTED */

typedef struct cmInetIpAddrArr
{
   U8       type;
   union
   {
      CmInetIpv4AddrArr   ipv4AddrArr;   
#ifdef IPV6_SUPPORTED
      CmInetIpv6AddrArr   ipv6AddrArr;   
#endif /* IPV6_SUPPORTED */
   } u;

} CmInetIpAddrArr;


/* Adding a type for socket Address */
typedef struct cmInetCmnSockAddr
{
   U32 type;
   U32 len;
   union
   {
      struct sockaddr_in addr;
#ifdef IPV6_SUPPORTED
      struct sockaddr_in6 addr6;
#endif
   }u;
}CmInetCmnSockAddr;



/* Adding a type for socket descriptors */
#ifdef WIN32
typedef SOCKET CmInetFdType;
#else
#if (defined(SUNOS) || defined(HPOS)) 
typedef S32 CmInetFdType;
#else
#ifdef SS_LINUX
typedef S32 CmInetFdType;
#else
typedef S16 CmInetFdType;
#endif /* SS_LINUX */
#endif /* SUNOS || HPOS */
#endif /* WIN32 */
/* cm_inet_x_001.main_29 Poll Implementation Changes */
typedef struct pollfd CmInetPollFd;

typedef struct cmInetFd          /* trillium socket file descriptor */
{
   CmInetFdType fd;              /* socket descriptor */
   U8     blocking;              /* true if socket is blocking */
   U8     type;                  /* socket type (stream|datagram) */

#ifdef IPV6_SUPPORTED
   U8     protType;              /* indicates whether IPv4 or IPv6 socket */
#endif /* IPV6_SUPPORTED */  
}CmInetFd;

typedef fd_set CmInetFdSet;      /* socket file descriptor set */

#ifdef IPV6_SUPPORTED 
/* IPV4 Address */
typedef struct cmInetIpv4Addr
{
   U16             port;
   CmInetIpAddr    address;
} CmInetIpv4Addr;

/* IPV6 Address */
typedef struct cmInetIpv6Addr
{
   U16             port;
   CmInetIpAddr6   ipv6NetAddr;
} CmInetIpv6Addr;

typedef struct CmInetAddr        /* Internet address */
{
   U8           type;            /* type of address present in the union */
   union
   {
      CmInetIpv4Addr    ipv4Addr;   /* IPV4 Address */
      CmInetIpv6Addr    ipv6Addr;   /* IPV6 Address */
   }u;
}CmInetAddr;

typedef struct cmInetMCastInf6   /* multicast interface information */
{
   CmInetIpAddr6 mCastAddr;      /* multicast address */
   U32           localInf;       /* local interface */
} CmInetMCastInf6;
#else
/* IPV4 Address */
typedef struct cmInetAddr1    
{
   U16             port;
   CmInetIpAddr    address;
} CmInetAddr;
typedef CmInetAddr CmInetIpv4Addr; 
#endif /* IPV6_SUPPORTED */

typedef struct cmInetMemInfo     /* memory information */
{
   Region   region;              /* memory region */
   Pool     pool;                /* memory pool */
} CmInetMemInfo;

typedef struct cmInetMCastInf    /* multicast information */
{
   CmInetIpAddr  mCastAddr;      /* multicast class D address */
   CmInetIpAddr  localAddr;      /* local interface address */
}CmInetMCastInf;

/* this is the same structure as cmNetAddr in cm_tpt.x
   used here for cmInetConvertStrToIpAddr */
typedef struct cmInetNetAddr
{
   U8   type;      /* type of network address */
   union
   {
      CmInetIpAddr   ipv4NetAddr; /* IP network address */
#ifdef IPV6_SUPPORTED
      CmInetIpAddr6  ipv6NetAddr; /* IPv6 network address */
#endif /* IPV6_SUPPORTED */
   }u;
} CmInetNetAddr;

/*cm_inet_x_001.main_23 Updated for TUCL 2.1 Release (Kernel SCTP Support) */
#ifdef CM_LKSCTP
typedef struct cmInetNetAddrLst
{
   U8   count;
   CmInetNetAddr   addrs[CM_INET_NUM_NET_ADDR];
} CmInetNetAddrLst;
#endif

#ifdef LOCAL_INTF
/* used to pass local interface (on which packet was received) to upper user */
typedef struct cmInetLocalInf
{
   Bool          intfPrsnt;   /* bool to indicate if this is a valid loc intf */
   U32           localIf;     /* interface index IPv4(32 bit) or IPv6(32 bit) */
   CmInetNetAddr localIfAddr; /* interface address */
}CmInetLocalInf;
#endif /* LOCAL_INTF */

#ifdef IPV6_SUPPORTED
#if (defined(SUNOS) || defined(HPOS) || defined(SS_VW))
typedef struct icmp6_filter CmInetIcmp6Filter;
#endif /* SUNOS || HPOS */
#endif /* IPV6_SUPPORTED */

/* New data structures needed to support 3 types of IPV6 extension 
 * headers - HBH, Destination Option & Route Header */
#ifdef IPV6_SUPPORTED
#ifdef IPV6_OPTS_SUPPORTED

/* structure to hold TLV of each HBH option */ 
typedef struct cmInetIpv6HBHHdr
{
   U8 type;
   U8 length;
   U8 *value;
} CmInetIpv6HBHHdr;

/* structure to hold TLV of each Destination option */
typedef struct cmInetIpv6DestOptsHdr
{
   U8 type;
   U8 length;
   U8 *value;
} CmInetIpv6DestOptsHdr;

/* structure to hold IPV6 addresses of the Route header */
typedef struct cmInetIpv6RtHdr
{
   U8 numAddrs;
   U32 slMap;
   CmInetIpAddr6 *ipv6Addrs; 
} CmInetIpv6RtHdr;

/* array of all HBH options */
typedef struct cmInetIpv6HBHHdrArr
{
   U8 numHBHOpts;
   CmInetIpv6HBHHdr *hbhOpts;
} CmInetIpv6HBHHdrArr;

/* array of all Destination options */
typedef struct cmInetIpv6DestOptsArr
{
   U8 numDestOpts;
   CmInetIpv6DestOptsHdr *destOpts;
} CmInetIpv6DestOptsArr;

/* structure having 3 substructures for 3 types of ext headers */
typedef struct cmInetIpv6ExtHdr
{
   Bool hbhHdrPrsnt;
   CmInetIpv6HBHHdrArr hbhOptsArr;
   Bool destOptsPrsnt;
   CmInetIpv6DestOptsArr destOptsArr;
   Bool rtOptsPrsnt;
   CmInetIpv6RtHdr rtOptsArr;
} CmInetIpv6ExtHdr;

/* structure for type 0 Route Header */
typedef struct cmInetIpv6RtHdr0 
{
   U8 ip6r0_nextHdr;
   U8 ip6r0_hdrExtLen;
   U8 ip6r0_type;
   U8 ip6r0_segLeft;
   U32 ip6r0_resrvAndSLmap; /* first byte reserved, last 3 srtict/loose map */
} CmInetIpv6RtHdr0;
#endif /* IPV6_OPTS_SUPPORTED */

typedef struct cmInetIpv6HdrParm
{
   TknU8 ttl;
   CmInetNetAddr srcAddr6;/* used to set src addr on sending pkt(IPv6) */
#ifdef IPV6_OPTS_SUPPORTED   
   CmInetIpv6ExtHdr ipv6ExtHdr;
#endif /* IPV6_OPTS_SUPPORTED */
} CmInetIpv6HdrParm;
#endif /* IPV6_SUPPORTED */

typedef struct cmInetIpv4HdrParm 
{
   TknU8    proto;                 /* Protocol value */
   TknU8    dfBit;                 /* Don't fragment flag */
   TknU8    tos;                   /* Type of Service */
   TknU8    ttl;                   /* Time to Live */
   /* added new field */
#ifdef IPV4_OPTS_SUPPORTED 
   TknStr64 ipv4HdrOpt;            /* IPV4 hdr opt */
#endif /* IPV4_OPTS_SUPPORTED */
} CmInetIpv4HdrParm;
typedef struct cmInetIpHdrParm
{
   U8 type;
   union
   {
      CmInetIpv4HdrParm  hdrParmIpv4;  /* IPv4 header parameters */
#ifdef IPV6_SUPPORTED
      CmInetIpv6HdrParm ipv6HdrParm;
#endif /* IPV6_SUPPORTED */    
   } u;
} CmInetIpHdrParm;

/* New data structures to peek into the file descriptor set. */
/* fdSetInfo structure */
#ifdef WIN32
typedef struct cmInetFdSetInfo
{
   Bool           initDone;      /* Initialisation done */
   U32            numFds;        /* Number of file descriptors scanned */
} CmInetFdSetInfo;
#else
#if (defined(SUNOS) || defined(SS_LINUX) || defined(SS_VW) || defined(HPOS)) 
typedef struct cmInetFdSetInfo
{
   Bool        initDone;         /* Initialisation done */
   Bool        bigEndian;        /* Big endian architecture */
   U16         arIdx;            /* Current index in fd_set array */
   U32         numArElems;       /* Number of array elements */
   U8          ar[256];          /* Array of bit positions */
} CmInetFdSetInfo;
#endif /* SUNOS || SS_LINUX || SS_VW */
#endif /* WIN32 */

/*cm_inet_x_001.main_23 Updated for TUCL 2.1 Release (Kernel SCTP Support) */
#ifdef CM_LKSCTP
typedef struct cmInetSctpSndRcvInfo
{
   U16   stream;
   U16   ssn;
   U16   flags;
   U32   ppid;
   U32   context;
   U32   timetolive;
   U32   tsn;
   U32   cumtsn;
   U32   assocId;
} CmInetSctpSndRcvInfo;

typedef struct cmInetSctpNotification 
{
   struct 
   {
      U16 nType;  
      U16 nFlags;
      U32 nLen;
   } header;

   union 
   {
      struct  
      {
         U16 state;
         U16 error;
         U16 outStreams;
         U16 inStreams;
         U32 assocId;
         U8  *info;
      }assocChange;
      struct  
      {
         CmInetNetAddr addr;
         S32           state;
         S32           error;
         U32           assocId;
      }paddrChange;
      struct 
      {
         U16 error;
         U32 assocId;
         U8  *data;
      }remoteErr;
      struct 
      {
         U32 error;
         CmInetSctpSndRcvInfo info;
         U32 assocId;
         U8  *data;
      }sndFailed;
      struct 
      {
         U32 assocId;
      }shutdownEvt;
      struct 
      {
         U32 adaptationInd;
         U32 assocId;
      }adaptationEvt;
      struct 
      {
         U32 indication;
         U32 assocId;
      }pdapiEvt;
   }u;
}CmInetSctpNotification;

typedef struct cmInetSockLinger
{
   Bool   enable;
   U32    lingerTime;
} CmInetSockLinger;

typedef struct cmInetSctpSockEvent
{
   Bool   dataIoEvent;
   Bool   associationEvent;
   Bool   addressEvent;
   Bool   sendFailureEvent;
   Bool   peerErrorEvent;
   Bool   shutdownEvent;
   Bool   partialDeliveryEvent;
   Bool   adaptationLayerEvent;
} CmInetSctpSockEvent;

typedef struct cmInetSctpPeerAddrParams
{
   U32            assocId;
   struct
   {
      Bool           addrPres;
      CmInetNetAddr  addr;
      U16            port;
   }s;
   U32            pathMaxRxt;
   U32            pathMtu;
   U32            sackDelay;
   U8             pmtudFlag;
   U8             sackDelayFlag;
   U8             hbEnblFlag;
   U32            hbInterval;
} CmInetSctpPeerAddrParams;

typedef struct cmInetSctpPrimAddr 
{
   U32            assocId;
   CmInetNetAddr  addr;
   U16            port;
}CmInetSctpPrimAddr;

typedef struct cmInetSctpPeerAddrInfo
{
   U32           assocId;
   CmInetNetAddr addr;
   U16           port;
   Bool          isActive;
   U32           cwnd;
   U32           srtt;
   U32           rto;
   U32           mtu;
}CmInetSctpPeerAddrInfo;

typedef struct cmInetSctpStatus
{
   U32   assocId;
   S32   state;
   U32   rwnd;
   U16   unackdata;
   U16   penddata;
   U16   instrms;
   U16   outstrms;
   U32   fragPoint; 
   CmInetSctpPeerAddrInfo primary;
}CmInetSctpStatus;

/*cm_inet_x_001.main_24 Updated for the support of configurable RTO parameters, 
                        HBeat value Max retransmissions (Init, Path, Association)*/
typedef struct cmInetSctpRtoInfo
{
   U32   assocId;
   U32   rtoInitial;
   U32   rtoMax;
   U32   rtoMin;
}CmInetSctpRtoInfo;

typedef struct cmInetSctpInitMsg
{
   U16  maxInitReTx;
   U16  maxInitTimeout;
   U16  maxInstreams;
   U16  numOstreams;
}CmInetSctpInitMsg;

typedef struct cmInetSctpAssocParams
{
   U32   assocId;
   U16   assocMaxReTx; 
   U16   numberOfPeerDest;
   U32   peerRwnd;
   U32   localRwnd;
   U32   cookieLife;
}CmInetSctpAssocParams;

#endif

/* added a new type CmInetSockAddr */
#ifdef SUNOS
#ifdef SS_LINUX
typedef struct iovec CmInetIovec;
#else
typedef iovec_t CmInetIovec;
#endif /* SS_LINUX */
typedef struct sockaddr CmInetSockAddr;
#else
#ifdef HPOS
typedef struct iovec CmInetIovec;
typedef struct sockaddr CmInetSockAddr;
#else
#ifdef SS_VW
typedef struct iovec CmInetIovec;
typedef struct sockaddr CmInetSockAddr;
#else
#ifdef WIN32
typedef struct sockaddr CmInetSockAddr;
#else
#ifdef SS_PS
typedef struct iovec CmInetIovec;
typedef struct sockaddr_in CmInetSockAddr;
#endif /* SS_PS */
#endif /* WIN32 */
#endif /* SS_VW */
#endif /* HPOS */
#endif /* SUNOS */

#ifdef SUNOS
typedef struct sockaddr_in6 CmInet6SockAddr;
typedef struct sockaddr_in CmInet4SockAddr;
#endif /* SUNOS */

/* addrInfo structure */ 
#if (!defined(SS_VW) && !defined(SS_PS) && !defined(WIN32))
typedef struct addrinfo  CmInetAddrInfo;
#endif /* SS_VW | SS_PS | WIN32*/

/* socket function prototypes */

#ifdef CM_INET2  
#ifdef IPV6_SUPPORTED
S16 cmInetSocket ARGS((U8 type, CmInetFd *sockFd, U8 protocol,
                              U8 family));
#else
S16 cmInetSocket ARGS((U8 type, CmInetFd *sockFd, U8 protocol));
#endif /* IPV6_SUPPORTED */
#else   /* CM_INET2 */ 
S16 cmInetSocket ARGS((U8 type, CmInetFd *sockFd));
#endif  /* CM_INET2 */ 

/* cm_inet_x_001.main_22 1. Added new interface - cmInetFlushRecvBuf()
                               to flush the data from socket receive buffer. */
#ifdef CM_INET_FLUSH_RECV_BUF
S16 cmInetFlushRecvBuf ARGS((CmInetFd *sockFd,
                               MsgLen *len,
                               S32 flags));
#endif /* CM_INET_FLUSH_RECV_BUF*/

S16 cmInetBind ARGS((CmInetFd *sockFd, CmInetAddr *myAddr));
S16 cmInetConnect ARGS((CmInetFd *sockFd, CmInetAddr *servAddr));
S16 cmInetListen ARGS((CmInetFd *sockFd, S16 backLog));
S16 cmInetAccept ARGS((CmInetFd *sockFd, CmInetAddr *fromAddr, 
                              CmInetFd *newSockFd));
#ifdef IPV6_OPTS_SUPPORTED
#ifdef LOCAL_INTF
S16 cmInetRecvMsg ARGS((CmInetFd *sockFd, CmInetAddr *dstAddr, 
                               CmInetMemInfo *info, Buffer **mPtr, 
                               MsgLen *len, CmInetIpHdrParm *ipHdrParams,
                               CmInetLocalInf  *localIf, S32 flags));
#else
S16 cmInetRecvMsg ARGS((CmInetFd *sockFd, CmInetAddr *dstAddr, 
                               CmInetMemInfo *info, Buffer **mPtr, 
                               MsgLen *len, CmInetIpHdrParm *ipHdrParams,
                               S32 flags));
#endif /* LOCAL_INTF */
#else
#ifdef LOCAL_INTF
S16 cmInetRecvMsg ARGS((CmInetFd *sockFd, CmInetAddr *dstAddr, 
                               CmInetMemInfo *info, Buffer **mPtr, 
                               MsgLen *len, CmInetLocalInf  *localIf,
                               S32 flags));
#else
S16 cmInetRecvMsg ARGS((CmInetFd *sockFd, CmInetAddr *dstAddr, 
                               CmInetMemInfo *info, Buffer **mPtr, 
                               MsgLen *len, S32 flags));
#endif /* LOCAL_INTF */
#endif /* IPV6_OPTS_SUPPORTED */
S16 cmInetSendDscpMsg ARGS((CmInetFd *sockFd, CmInetAddr *dstAddr, 
                               CmInetMemInfo *info, Buffer *mBuf, MsgLen *len,
                               CmInetIpHdrParm *ipHdrParams, S16 flags));

#ifdef IPV6_OPTS_SUPPORTED
S16 cmInetSendMsg ARGS((CmInetFd *sockFd, CmInetAddr *dstAddr, 
                               CmInetMemInfo *info, Buffer *mBuf, MsgLen *len,
                               CmInetIpHdrParm *ipHdrParams, S16 flags));
#else
S16 cmInetSendMsg ARGS((CmInetFd *sockFd, CmInetAddr *dstAddr, 
                               CmInetMemInfo *info, Buffer *mBuf, MsgLen *len,
                               S16 flags));
#endif /* IPV6_OPTS_SUPPORTED */
S16 cmInetPeek ARGS((CmInetFd *sockFd, CmInetAddr *fromAddr, 
                            CmInetMemInfo *info, MsgLen dataPos, 
                            MsgLen dataLen, U8 *data));
/* cm_inet_x_001.main_26: Added new function declaration cmInetPeekNew() */ 
S16 cmInetPeekNew ARGS((CmInetFd *sockFd, CmInetAddr *fromAddr, 
                            CmInetMemInfo *info, MsgLen dataPos, 
                            MsgLen dataLen, U8 *data)); 
S16 cmInetClose ARGS((CmInetFd *sockFd));
S16 cmInetShutdown ARGS((CmInetFd *sockFd, S32 howTo));
S16 cmInetSelect ARGS((CmInetFdSet *readFdS, CmInetFdSet *writeFdS, 
                              U32 *mSecTimeout, S16 *numFdS));
S16 cmInetSetOpt ARGS((CmInetFd *sockFd, U32 level, U32 type, 
                             Ptr value));
S16 cmInetGetNumRead ARGS((CmInetFd *sockFd, U32 *dataLen));
#ifndef SS_PS
S16 cmInetGetHostByName ARGS((S8 *hostName, CmInetIpAddrTbl *addrTbl));
S16 cmInetGetIpNodeByName ARGS((S8 *hostName, CmInetIpAddrArr *addrArr));
S16 cmInetAddr ARGS((S8 *asciiAddr, CmInetIpAddr *address));
S16 cmInetNtoa ARGS((CmInetIpAddr address, S8 **asciiAddr));
S16 cmInetPton ARGS((CmInetIpAddr *address, S8 *asciiAddr));
#ifdef IPV6_SUPPORTED 
S16 cmInetPton6 ARGS((CmInetIpAddr6 *address6, S8 *asciiAddr));
#endif /* IPV6_SUPPORTED */
#endif /*  SS_PS */
/* Function prototypes to peek into file descriptor set. */
#if (defined(WIN32) || defined(SUNOS) || defined(SS_LINUX) || defined(SS_VW) \
     || defined(HPOS))
S16 cmInetFdSetInfoInit ARGS((CmInetFdSetInfo *fdSetInfo));
S16 cmInetGetFd ARGS((CmInetFdSetInfo *fdSetInfo, CmInetFdSet *fdSet,
                             CmInetFdType *sockFd));
#endif /* WIN32 | SUNOS | SS_LINUX | SS_VW | HPOS */

S16 cmInetGetMemSize      ARGS((S32 *size));
S16 cmInetInit            ARGS((Void));
S16 cmInetDeInit          ARGS((Void));
S16 cmInetGetSockName     ARGS((CmInetFd *sockFd, CmInetAddr *locAddr));

S16 cmInetConvertStrToIpAddr ARGS((U16 len, U8 *val,
                                          CmInetNetAddr *address));
S16 cmInetAsciiToIpv4 ARGS((U8 numBytes, U8 *ipv4Addr,
                                   U16 len, U8 *val));


/* cm_inet_x_001.main_29 Poll Implementation Changes */
S16 cmInetPoll ARGS((CmInetPollFd  *pollFdArr,U32 idx,S16  *numFdS,U32  timeout));
S16 cmInetPollSetFd ARGS((CmInetFd  *sockFd,CmInetPollFd  *pollFdArr,S16  idx, U16  eventMask));
S16 cmInetPollFdIsSet ARGS((CmInetPollFd  *pollFdArr, S16 idx, U16  eventMask));
S16 cmInetPollClearFdREvent ARGS((CmInetPollFd  *pollFdArr, S16 idx, U16 eventMask));
S16 cmInetPollClearFdEvent ARGS((CmInetPollFd  *pollFdArr,S16 idx, U16 eventMask));
S16 cmInetPollDelFd ARGS((CmInetPollFd  *pollFdArr, S16 delIdx, S16  crntIdx));
S16 cmInetPollInitFdArr ARGS((CmInetPollFd  *pollFdArr));
S16 cmInetNtop ARGS((U8 type,Void *address,S8 *asciiAddr,U32 len));



/* cm_inet_x_001.main_21:Added wrapper function for getaddrinfo and freeaddrinfo */
#if (!defined(SS_VW) && !defined(SS_PS) && !defined(WIN32))
S32 cmInetGetAddrInfo ARGS((const S8* node, const S8* service, 
                                   const CmInetAddrInfo *hints, CmInetAddrInfo **res));
Void cmInetFreeAddrInfo ARGS((CmInetAddrInfo *res));
#endif /* SS_VW | SS_PS | WIN32 */

/*cm_inet_x_001.main_23 Updated for TUCL 2.1 Release (Kernel SCTP Support) */
#ifdef CM_LKSCTP
S16 cmInetSctpBindx     ARGS((CmInetFd *sockFd, 
                                     CmInetNetAddrLst *addrLst, 
                                     U16 port));
S16 cmInetSctpConnectx  ARGS((CmInetFd *sockFd, CmInetNetAddr *primAddr, 
                                     CmInetNetAddrLst *addrLst, 
                                     U16 port));
S16 cmInetSctpPeelOff   ARGS((CmInetFd *sockFd, U32 assocId, 
                                     CmInetFdType *assocFd));
S16 cmInetSctpSendMsg   ARGS((CmInetFd *sockFd, CmInetNetAddr *dstAddr, 
                                     U16 port, CmInetMemInfo *info, 
                                     Buffer *mBuf, MsgLen *len, U16 strmId,
                                     Bool unorderFlg, U16 ttl, U32 ppId, 
                                     U32 context));
S16 cmInetSctpRecvMsg  ARGS((CmInetFd *sockFd, CmInetNetAddr *srcAddr, 
                                    U16 *port, CmInetMemInfo *info, 
                                    Buffer **mBuf, MsgLen *len, 
                                    CmInetSctpSndRcvInfo *sinfo, U32 *flag,
                                    CmInetSctpNotification *ntfy));
S16 cmInetSctpGetPAddrs ARGS((CmInetFd *sockFd, U32 assocId, 
                                     CmInetNetAddrLst *addrlst));
S16 cmInetGetOpt        ARGS((CmInetFd *sockFd, U32 level, U32 type,
                                     Ptr value)); 

/*cm_inet_x_001.main_25: Added new funcion */
S16 cmInetShutDownSctp ARGS((CmInetFd *sockFd));
/*cm_inet_x_001.main_30: Added new function */
S16 cmInetAbortSctpAssoc ARGS((CmInetFd *sockFd, UConnId assocId));
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __CMINETX__ */

/**********************************************************************
         End of file
**********************************************************************/
