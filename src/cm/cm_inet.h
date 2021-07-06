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
 
        File:     cm_inet.h

*********************************************************************21*/
 

/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *      1000151     TCAP over TCP/IP
 */

#ifndef __CMINETH__
#define __CMINETH__

#define CM_INET_IPV4ADDR_SIZE      4
#define CM_INET_IPV6ADDR_SIZE      16

#define CM_INET_IPV4PORT_SIZE      2 
#define CM_INET_IPV6PORT_SIZE      2 

/* reusing the definition */
#define CM_INET_IPV4ADDR_TYPE CM_IPV4ADDR_TYPE
#define CM_INET_IPV6ADDR_TYPE CM_IPV6ADDR_TYPE

#ifdef WIN32
#define CM_INET_HIGH_VER     2
#define CM_INET_LOW_VER      2
#endif /* WIN32 */

/* Invalid socket flag */
#ifdef WIN32
#define CM_INET_INV_SOCKFD INVALID_SOCKET
#else
#define CM_INET_INV_SOCKFD -1
#endif

/* cm_inet_h_001.main_30 Poll Specific changes */
#define CM_INET_POLL_MAXFDSUPP 1024

#define CM_INET_IPV4_NUM_ADDR   16
#define CM_INET_IPV6_NUM_ADDR   4 

/*cm_inet_h_001.main_21 Updated for TUCL 2.1 Release (Kernel SCTP Support) */
#ifdef CM_LKSCTP
#define CM_INET_NUM_NET_ADDR    5 

#define CM_INET_SCTP_MSG_NOTIFICATION  0x8000
#define CM_INET_SCTP_MSG_EOR           0x80

/* sctp notification type */
#define CM_INET_SCTP_ASSOC_CHANGE              1
#define CM_INET_SCTP_PEER_ADDR_CHANGE          2
#define CM_INET_SCTP_SEND_FAILED               3
#define CM_INET_SCTP_REMOTE_ERROR              4
#define CM_INET_SCTP_SHUTDOWN_EVENT            5
#define CM_INET_SCTP_PARTIAL_DELIVERY_EVENT    6 
#define CM_INET_SCTP_ADAPTATION_INDICATION     7 

/* sctp association change state */
#define CM_INET_SCTP_COMM_UP          1
#define CM_INET_SCTP_COMM_LOST        2
#define CM_INET_SCTP_RESTART          3
#define CM_INET_SCTP_SHUTDOWN_COMP    4
#define CM_INET_SCTP_CANT_STR_ASSOC   5

/* sctp peer addr state */
#define CM_INET_SCTP_ADDR_AVAILABLE     1
#define CM_INET_SCTP_ADDR_UNREACHABLE   2
#define CM_INET_SCTP_ADDR_REMOVED       3
#define CM_INET_SCTP_ADDR_ADDED         4
#define CM_INET_SCTP_ADDR_MADE_PRIM     5
#define CM_INET_SCTP_ADDR_CONFIRMED     6

/* sctp assoc state */
#define CM_INET_SCTP_STA_EMPTY              0
#define CM_INET_SCTP_STA_CLOSED             1
#define CM_INET_SCTP_STA_COOKIE_WAIT        2
#define CM_INET_SCTP_STA_COOKIE_ECHOED      3
#define CM_INET_SCTP_STA_ESTABLISHED        4
#define CM_INET_SCTP_STA_SHUTDOWN_PENDING   5
#define CM_INET_SCTP_STA_SHUTDOWN_SENT      6
#define CM_INET_SCTP_STA_SHUTDOWN_RECEIVED  7 
#define CM_INET_SCTP_STA_SHUTDOWN_ACK_SENT  8
#endif

#ifdef IPV6_SUPPORTED 
#define CM_INET_IPV4_DOMAIN     AF_INET
#define CM_INET_IPV6_DOMAIN     AF_INET6
#endif /* IPV6_SUPPORTED */

/* domain */

/* cm_inet_h_001.main_31 : added macro for Debug print buffer size */
#ifdef CMINETDBG
#define CMINET_PRNT_BUF_SIZE 512
#endif
/* socket types */
#define CM_INET_STREAM  SOCK_STREAM   
#define CM_INET_DGRAM   SOCK_DGRAM
/*cm_inet_h_001.main_21 Updated for TUCL 2.1 Release (Kernel SCTP Support) */
#ifdef CM_LKSCTP
#define CM_INET_SEQPACKET   SOCK_SEQPACKET
#endif

/* cm_inet_h_001.main_18:Added CM_INET_AI_PASSIVE for getaddrinfo*/
#define CM_INET_AI_PASSIVE   AI_PASSIVE

#ifdef CM_INET2  
#define   CM_INET_RAW   SOCK_RAW
#endif  /* CM_INET2 */ 

/* socket options type */
#define CM_INET_OPT_BLOCK         0  /* blocking socket */ 
#define CM_INET_OPT_REUSEADDR     1  /* reuse socket address */
#define CM_INET_OPT_RX_BUF_SIZE   2  /* socket receive buffer size */
#define CM_INET_OPT_TX_BUF_SIZE   3  /* socket transmitt buffer size */
#define CM_INET_OPT_ADD_MCAST_MBR 4  /* add IP multicast group membership */
#define CM_INET_OPT_DRP_MCAST_MBR 5  /* drop IP multicast group membership */
#define CM_INET_OPT_TCP_NODELAY   6  /* disable TCP Nagle algorithm */

#ifdef SS_LINUX
#define CM_INET_OPT_BSD_COMPAT    7  /* BSD compatible option for Linux */
#endif /* SS_LINUX */

#define CM_INET_OPT_MCAST_LOOP    8  /* Multicast loop enable/disable */
#define CM_INET_OPT_MCAST_IF      9  /* specify local outgoing interface */
#define CM_INET_OPT_MCAST_TTL     14 /* Specify TTL value for multicast
                                      * applications */
#ifdef CM_INET2  
#define CM_INET_OPT_HDR_INCLD     10 /* Header Include */
#define CM_INET_OPT_DONTFRAGMENT  11 /* Don't Fragment  */
#define CM_INET_OPT_TOS           12 /* Type of service  */
#define CM_INET_OPT_TTL           13 /* Time to Live   */

#ifdef IPV6_SUPPORTED
#define CM_INET_OPT_ADD_MCAST6_MBR  15 /* Add IPV6 multicast member */
#define CM_INET_OPT_DRP_MCAST6_MBR  16 /* Drop IPV6 multicast member */
#define CM_INET_OPT_MCAST6_LOOP     17 /* Enable or disable multicast loop 
                                        * packets */
#define CM_INET_OPT_MCAST6_IF       18 /* Specify multicast interface */
#define CM_INET_OPT_MCAST6_HOPS     19 /* Specify multicast hop limit */

#define CM_INET_OPT_ICMP6_FILTER    20 /* Specify icmp V6 filter */
#define CM_INET_OPT_IPV6_TTL        21 /* Specify unicast hop limit */

/* IPv6 socket options */
#ifdef IPV6_OPTS_SUPPORTED
#define CM_INET_OPT_RECVIPV6_HOPLIM  22 /* Receive hop limit */
#define CM_INET_OPT_RECVIPV6_HBHOPTS 25  /* Receive HopByHop options */
#define CM_INET_OPT_RECVIPV6_DSTOPTS 26  /* Receive Destination options */
#define CM_INET_OPT_RECVIPV6_RTHDR   27  /* Receive Route header options */
#define CM_INET_ALL_IPV6_EXTHDRS_LEN 512 /* total length of 3 IPV6 ext hdrs */
#define CM_INET_OPT_IP_ROUTER_ALERT6 30
#endif /* IPV6_OPTS_SUPPORTED */
#define CM_INET_OPT_IPV6_PKTINFO     31
#endif /* IPV6_SUPPORTED */

/* IPv4 socket options */
#ifdef IPV4_OPTS_SUPPORTED
#define CM_INET_OPT_IP_OPTIONS       28 /* Router Alert in IPv4 */
#define CM_INET_OPT_IPV4_PKTINFO     32 /* IPv4 PKTINFO */
#define CM_INET_OPT_IP_ROUTER_ALERT  29 /* Router Alert to Intercept Linux */
#endif /* IPV4_OPTS_SUPPORTED */
#endif  /* CM_INET2 */ 

#define CM_INET_OPT_BROADCAST        23 /* Broadcasting enable/disable */
#define CM_INET_OPT_KEEPALIVE        24 /* KEEPALIVE enable/disable */

/*cm_inet_h_001.main_21 Updated for TUCL 2.1 Release (Kernel SCTP Support) */
#ifdef CM_LKSCTP
#define CM_INET_OPT_LINGER                 33 /* socket linger */
#define CM_INET_OPT_SCTP_EVENTS            34 /* sctp events */
#define CM_INET_OPT_SCTP_PRIM_ADDR         35 /* sctp primary address */
#define CM_INET_OPT_SCTP_PEERADDR_PARAMS   36 /* sctp peer address parameters */
#define CM_INET_OPT_SCTP_GET_ASSOC_STA     37
#define CM_INET_OPT_SCTP_GET_PADDR_INFO    38
/*cm_inet_h_001.main_22 Updated for the support of configurable RTO parameters, 
                        HBeat value Max retransmissions (Init, Path, Association)*/
#define CM_INET_OPT_SCTP_ASSOC_PARAMS      39
#define CM_INET_OPT_SCTP_RTO_INFO          40
#define CM_INET_OPT_SCTP_INIT_MSG          41
#endif


/* cm_inet_h_001.main_29: Added new macro to support filter for ICMP messages */
#ifdef CM_ICMP_FILTER_SUPPORT
#ifdef SS_LINUX
#define CM_INET_OPT_ICMP_FILTER    42 
#endif
#ifdef IPV6_SUPPORTED
#define CM_MAX_IPV6_FILTER  8
#endif
#endif

/* socket option value */
#define CM_INET_OPT_DISABLE  0      /* enable option */
#define CM_INET_OPT_ENABLE   1      /* disable option */
#ifdef CM_LKSCTP
/* cm_inet_h_001.main_23: changed the value from 0xffffffff to ~0 to fix TUCL
                          warnings */
#define CM_INET_OPT_DEFAULT  (~0) /* default option */
#endif

/* level for cmInetSetOpt */
#define CM_INET_LEVEL_SOCKET      SOL_SOCKET   /* socket level option */
#define CM_INET_LEVEL_IP          IPPROTO_IP   /* IP level option */
#define CM_INET_LEVEL_TCP         IPPROTO_TCP  /* TCP level option */
#define CM_INET_LEVEL_SCTP        IPPROTO_SCTP /* SCTP level option */
/* cm_inet_h_001.main_29: Added new macro to support filter for ICMP messages */
#ifdef CM_ICMP_FILTER_SUPPORT
#ifdef SS_LINUX
#define CM_INET_LEVEL_RAW         SOL_RAW   /* socket level option */
#endif
#endif

/* shutdown options */
#ifdef WIN32
#define CM_INET_SHTDWN_RECV  SD_RECEIVE
#define CM_INET_SHTDWN_SEND  SD_SEND
#define CM_INET_SHTDWN_BOTH  SD_BOTH 
#else
#define CM_INET_SHTDWN_RECV  0
#define CM_INET_SHTDWN_SEND  1
#define CM_INET_SHTDWN_BOTH  2
#endif /* WIN32 */

/* send/recv control flags */
#define CM_INET_NO_FLAG   0
#define CM_INET_MSG_PEEK  MSG_PEEK

#if !(defined(WIN32) || defined(SS_LINUX))
#define CM_INET_MAX_INFO 512 
#endif /* WIN32 || SS_LINUX */

#define CM_INET_MAX_DBUF  15 /* max. number of dBufs for a message */

/* cm_inet_h_001.main_24 : Inet message length is extended to 7fffffff when LONG_MSG is defined */

/* cm_inet_h_001.main_26  rss 1. Provided the user the flexibility to set CM_INET_MAX_MSG_LEN
 * value during compilation time */
#ifndef CM_INET_MAX_MSG_LEN 
#define CM_INET_MAX_MSG_LEN  0x7fff  /* max length of a message */
#endif

/* cm_inet_h_001.main_25 : ifndef flag added as this can be defined by the customer now*/
#ifndef CM_INET_MAX_UDPRAW_MSGSIZE
#define CM_INET_MAX_UDPRAW_MSGSIZE 2048
#endif

#define CM_INET_IPV6_ANCIL_DATA  512
#define CM_INET_IPV4_ANCIL_DATA  128

#define CM_INET_READ_ANY  -1  /* read any pending data */

/* cm_inet_h_001.main_19  1. Added new defines for the new interface - 
                             cmInetFlushRecvBuf() to flush the data 
                             from socket receive buffer. */
#ifdef CM_INET_FLUSH_RECV_BUF
#define CM_INET_READ_THROW -2  /* read and throw data from receive buffer */
#define CM_INET_MAX_BYTES_READ 1024
#endif /*CM_INET_FLUSH_RECV_BUF*/

/* cm_inet_h_001.main_20  Added new defines for returning unreachable failure */
#define RNETFAILED 27

#define CM_INET_INADDR_ANY  INADDR_ANY  /* accepts any address */

#ifdef CM_INET2  

/* protocol values */
#define   CM_INET_PROTO_IP    IPPROTO_IP   /* IP protocol */
#define   CM_INET_PROTO_ICMP  IPPROTO_ICMP /* ICMP protocol */
#define   CM_INET_PROTO_TCP   IPPROTO_TCP  /* TCP  protocol */
#define   CM_INET_PROTO_UDP   IPPROTO_UDP  /* UDP protocol */
#define   CM_INET_PROTO_RAW   IPPROTO_RAW  /* Raw protocol */
#define   CM_INET_PROTO_SCTP  132          /* SCTP protocol  */
/*cm_inet_h_001.main_21 Updated for TUCL 2.1 Release (Kernel SCTP Support) */
#ifdef CM_LKSCTP
#define   CM_INET_PROTO_LKSCTP  IPPROTO_SCTP          /* SCTP protocol  */
#endif
#define   CM_INET_PROTO_RSVP  46           /* RSVP protocol */

#ifdef IPV6_SUPPORTED
#define   CM_INET_LEVEL_IPV6   IPPROTO_IPV6   /* IP V6 protocol */
#define   CM_INET_PROTO_IPV6   IPPROTO_IPV6   /* IP V6 protocol */
#define   CM_INET_PROTO_ICMPV6 IPPROTO_ICMPV6 /* ICMP V6 protocol */
#endif /* IPV6_SUPPORTED */

#endif  /* CM_INET2 */ 

/* macros */

/* macros to manipulate and checking a socket file descriptor set */
#define CM_INET_FD_SET(_sockFd, _fdSet)    FD_SET((_sockFd)->fd, _fdSet) 
#define CM_INET_FD_CLR(_sockFd, _fdSet)    FD_CLR((_sockFd)->fd, _fdSet)
#define CM_INET_FD_ISSET(_sockFd, _fdSet)  FD_ISSET((_sockFd)->fd, _fdSet)
#define CM_INET_FD_ZERO(_fdSet)            FD_ZERO(_fdSet)

/* macros to convert from network to host byteorder and vice versa */
#define CM_INET_NTOH_UINT32(_long)  ntohl(_long)
#define CM_INET_HTON_UINT32(_long)  htonl(_long)
#define CM_INET_NTOH_UINT16(_word)  ntohs(_word)
#define CM_INET_HTON_UINT16(_word)  htons(_word)

/* peeks a uint8_t from the given position */
#define CM_INET_PEEK_UINT8(_sockFd, _fromAddr, _info, _pos, _octet, _ret)  \
   _ret = cmInetPeek(_sockFd, _fromAddr, _info, _pos, sizeof(uint8_t), &_octet)  
          
/* 
 * peeks a uint16_t from the given position (it is supposed that the uint16_t is 
 * represented in big endian representation within the data stream) 
 */
#define CM_INET_PEEK_UINT16(_sockFd, _fromAddr, _info, _pos, _word, _ret)  \
   {  \
      uint8_t _tempWord[2];  \
      \
      _ret = cmInetPeek(_sockFd, _fromAddr, _info, _pos, sizeof(uint16_t), _tempWord);  \
      if (_ret == ROK)  \
      {  \
         _word = ((_tempWord[0] << 8) + (_tempWord[1]));  \
      }  \
   }

/* 
 * peeks a uint32_t from the given position (it is supposed that the uint32_t is 
 * represented in big endian representation within the data stream)
 */
#define CM_INET_PEEK_UINT32(_sockFd, _fromAddr, _info, _pos, _long, _ret)  \
   {  \
      uint8_t _tempLong[4];  \
      \
      _ret = cmInetPeek(_sockFd, _fromAddr, _info, _pos, sizeof(uint32_t), _tempLong);  \
      if (_ret == ROK)  \
      {  \
         _long = ((_tempLong[0] << 24) + (_tempLong[1] << 16)  \
                + (_tempLong[2] << 8) + _tempLong[3]);  \
      }  \
   }

/* tests if socket descriptor is invalide */
#ifdef WIN32 
#define CM_INET_INV_SOCK_FD(_sockFd)  ((_sockFd)->fd == INVALID_SOCKET)
#else
#define CM_INET_INV_SOCK_FD(_sockFd)  ((_sockFd)->fd < 0)
#endif /* WIN32 */

/* tests if two socket descriptor are equal */
#define CM_INET_SOCK_SAME(_s1, _s2, _ret)  \
   {  \
      _ret = FALSE;  \
      if ((_s1->fd == _s2->fd) &&  \
          (_s1->blocking == _s2->blocking) &&  \
          (_s1->type == _s2->type))  \
      {  \
         _ret = TRUE;  \
      }  \
   }

/* set socket descriptor to an invalid (uninitialized) value */
#ifdef WIN32 
#define CM_INET_SET_INV_SOCK_FD(_sockFd)  ((_sockFd)->fd = INVALID_SOCKET)
#else
#define CM_INET_SET_INV_SOCK_FD(_sockFd)  ((_sockFd)->fd = -1)
#endif /* WIN32 */

/* This macro frees ipHdrParm structure memory allocated to hold ipv6
 * extension headers */

#ifdef IPV6_OPTS_SUPPORTED
#define CM_INET_FREE_IPV6_HDRPARM(_region, _pool, _hdrParmIpv6) \
{ \
   uint8_t numOpts; \
   if( _hdrParmIpv6->ipv6ExtHdr.hbhHdrPrsnt) \
   { \
      for(numOpts = _hdrParmIpv6->ipv6ExtHdr.hbhOptsArr.numHBHOpts;  \
          numOpts > 0; numOpts--) \
      { \
         if (_hdrParmIpv6->ipv6ExtHdr.hbhOptsArr.hbhOpts[numOpts - 1].length) \
            SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,_region, _pool, (Data *)_hdrParmIpv6->ipv6ExtHdr. \
                 hbhOptsArr.hbhOpts[numOpts - 1].value, (Size)(_hdrParmIpv6-> \
                 ipv6ExtHdr.hbhOptsArr.hbhOpts[numOpts - 1].length)); \
            SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,_region, _pool, (Data *)&_hdrParmIpv6->ipv6ExtHdr. \
                 hbhOptsArr.hbhOpts[numOpts - 1], \
                 (Size)sizeof(CmInetIpv6HBHHdr)); \
      } \
   } \
   if(_hdrParmIpv6->ipv6ExtHdr.destOptsPrsnt) \
   { \
      for(numOpts = _hdrParmIpv6->ipv6ExtHdr.destOptsArr.numDestOpts; \
          numOpts > 0; numOpts--) \
      { \
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,_region, _pool, (Data *)_hdrParmIpv6->ipv6ExtHdr. \
                destOptsArr.destOpts[numOpts - 1].value, (Size)(_hdrParmIpv6-> \
                ipv6ExtHdr.destOptsArr.destOpts[numOpts - 1].length)); \
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,_region, _pool, (Data *)&_hdrParmIpv6->ipv6ExtHdr. \
                destOptsArr.destOpts[numOpts - 1], \
                (Size)sizeof(CmInetIpv6DestOptsHdr)); \
      } \
   } \
   if( _hdrParmIpv6->ipv6ExtHdr.rtOptsPrsnt) \
   { \
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,_region, _pool, \
               (Data *)_hdrParmIpv6->ipv6ExtHdr.rtOptsArr.ipv6Addrs, \
               (Size)(_hdrParmIpv6->ipv6ExtHdr.rtOptsArr.numAddrs * 16)); \
   } \
}
/* Use the function for HBH options for destinations options as both ext
 * header has similar format */
  
#define cmInet6BuildRecvDstOptsArr(cmsgPtr, cmsg_len, destOptsArr, hdrId, info) \
        cmInet6BuildRecvHopOptsArr(cmsgPtr, cmsg_len, \
                              (CmInetIpv6HBHHdrArr *)destOptsArr, hdrId, info)

#define cmInet6BuildSendDestOpts(destOptsArr, cmsgData, curMsgIdx, hdrId) \
        cmInet6BuildSendHBHOpts((CmInetIpv6HBHHdrArr *)destOptsArr, \
                                 cmsgData, curMsgIdx, hdrId)
   
#endif /* IPV6_OPTS_SUPPORTED */

#ifdef IPV6_SUPPORTED
#define CM_INET_COPY_IPV6ADDR(_addrToFill, _fromAddr)   \
{                                                       \
   memcpy(_addrToFill, _fromAddr, sizeof(CmInetIpAddr6)); \
}

#if (defined(SUNOS) || defined(HPOS) || defined(SS_VW))

#define CM_INET_ICMP6_FILTER_SETPASSALL(_icmp6Filter)    \
{                                                        \
   ICMP6_FILTER_SETPASSALL(&_icmp6Filter);               \
}

#define CM_INET_ICMP6_FILTER_SETBLOCKALL(_icmp6Filter)   \
{                                                        \
   ICMP6_FILTER_SETBLOCKALL(&_icmp6Filter);              \
}

#define CM_INET_ICMP6_FILTER_SETPASS(msgType, _icmp6Filter)  \
{                                                            \
   ICMP6_FILTER_SETPASS(msgType, &_icmp6Filter);             \
}

#define CM_INET_ICMP6_FILTER_SETBLOCK(msgType, _icmp6Filter) \
{                                                            \
   ICMP6_FILTER_SETBLOCK(msgType, &_icmp6Filter);            \
}

#define CM_INET_ICMP6_FILTER_WILLPASS(msgType, _icmp6Filter) \
{                                                            \
   ICMP6_FILTER_WILLPASS(msgType, &_icmp6Filter);            \
}

#define CM_INET_ICMP6_FILTER_WILLBLOCK(msgType, _icmp6Filter) \
{                                                             \
   ICMP6_FILTER_WILLBLOCK(msgType, &_icmp6Filter);            \
}
#endif /* SUNOS || HPOS */
#endif /* IPV6_SUPPORTED */

#define cmPkCmInetIpAddr(x, mBuf)       oduUnpackUInt32(x, mBuf)  /* pack IP Address */
#define cmUnpkCmInetIpAddr(x, mBuf)     oduPackUInt32(x, mBuf)  /* unpacks IP Address */

#ifdef SS_VW
#define CM_COPY_VWIPADDR(vwIpAddr, addr) \
   { \
      memcpy(addr, &vwIpAddr, sizeof(S32)); \
   }
#endif

/* Changes for peeking into the file descriptor set */
#ifdef WIN32
#define CM_INET_FDSETINFO_RESET(_fdSetInfo) \
{ \
   _fdSetInfo->numFds = 0; \
}
#else
#define CM_INET_FDSETINFO_RESET(_fdSetInfo) \
{ \
   _fdSetInfo->arIdx = 0; \
}
#endif /* WIN32 */

/* convert a hex character in ASCII to int format */
#define CM_INET_ATOH(_intVal, _asciiVal)                                   \
{                                                                         \
   if ((_asciiVal >='a') && (_asciiVal <='f'))                            \
   {                                                                      \
     _intVal = (16 * _intVal) + (_asciiVal - 'a' +10 );                   \
   }                                                                      \
   else if ((_asciiVal >='A') && (_asciiVal <= 'F'))                      \
   {                                                                      \
     _intVal = (16 * _intVal) + (_asciiVal - 'A' +10 );                   \
   }                                                                      \
   else                                                                   \
   {                                                                      \
     _intVal = (16 * _intVal) + (_asciiVal - '0');                        \
   }                                                                      \
}

/* convert a decimal digit in ASCII to int format */
#define CM_INET_ATOI(_intVal, _asciiVal)                                  \
{                                                                         \
   _intVal = (10 * _intVal) + (_asciiVal - '0');                          \
}


#define CM_INET_GET_IPV4_ADDR_FRM_STRING(_value, _str)                    \
{                                                                         \
   uint16_t     _hiWord;                                                       \
   uint16_t     _loWord;                                                       \
                                                                          \
   _hiWord = 0;                                                           \
   _loWord = 0;                                                           \
   _hiWord = PutHiByte(_hiWord, (_str[0]));                               \
   _hiWord = PutLoByte(_hiWord, (_str[1]));                               \
   _loWord = PutHiByte(_loWord, (_str[2]));                               \
   _loWord = PutLoByte(_loWord, (_str[3]));                               \
   _value  = PutLoWord(_value, _loWord);                                  \
   _value  = PutHiWord(_value, _hiWord);                                  \
}

/* cm_inet_h_001.main_27: Added error codes*/
#define CMINETLOGERROR(errCls, errCode, errVal, errDesc) \
   SLogError(ENTNC, INSTNC, 0, __FILE__, __LINE__, \
	              errCls, errCode, errVal, errDesc )

#define ECMINET		0
#define ECMINET001	(ECMINET + 1)     /*cm_inet.c : 819*/
#define ECMINET002	(ECMINET + 2)     /*cm_inet.c : 871*/
#define ECMINET003	(ECMINET + 3)     /*cm_inet.c : 993*/
#define ECMINET004	(ECMINET + 4)     /*cm_inet.c : 997*/
#define ECMINET005	(ECMINET + 5)     /*cm_inet.c : 1081*/
#define ECMINET006	(ECMINET + 6)     /*cm_inet.c : 1188*/
#define ECMINET007	(ECMINET + 7)     /*cm_inet.c : 1226*/
#define ECMINET008	(ECMINET + 8)     /*cm_inet.c : 1335*/
#define ECMINET009	(ECMINET + 9)     /*cm_inet.c : 1423*/
#define ECMINET010	(ECMINET + 10)    /*cm_inet.c : 1566*/
#define ECMINET011	(ECMINET + 11)    /*cm_inet.c : 1659*/
#define ECMINET012	(ECMINET + 12)    /*cm_inet.c : 1783*/
#define ECMINET013	(ECMINET + 13)    /*cm_inet.c : 1867*/
#define ECMINET014	(ECMINET + 14)    /*cm_inet.c : 1987*/
#define ECMINET015	(ECMINET + 15)    /*cm_inet.c : 2292*/
#define ECMINET016	(ECMINET + 16)    /*cm_inet.c : 2310*/
#define ECMINET017	(ECMINET + 17)    /*cm_inet.c : 2571*/
#define ECMINET018	(ECMINET + 18)    /*cm_inet.c : 2811*/
#define ECMINET019	(ECMINET + 19)    /*cm_inet.c : 2955*/
#define ECMINET020	(ECMINET + 20)    /*cm_inet.c : 2960*/
#define ECMINET021	(ECMINET + 21)    /*cm_inet.c : 3020*/
#define ECMINET022	(ECMINET + 22)    /*cm_inet.c : 3119*/
#define ECMINET023	(ECMINET + 23)    /*cm_inet.c : 3412*/
#define ECMINET024	(ECMINET + 24)    /*cm_inet.c : 3606*/
#define ECMINET025	(ECMINET + 25)    /*cm_inet.c : 3709*/
#define ECMINET026	(ECMINET + 26)    /*cm_inet.c : 3723*/
#define ECMINET027	(ECMINET + 27)    /*cm_inet.c : 3747*/
#define ECMINET028	(ECMINET + 28)    /*cm_inet.c : 4148*/
#define ECMINET029	(ECMINET + 29)    /*cm_inet.c : 4216*/
#define ECMINET030	(ECMINET + 30)    /*cm_inet.c : 4227*/
#define ECMINET031	(ECMINET + 31)    /*cm_inet.c : 4240*/
#define ECMINET032	(ECMINET + 32)    /*cm_inet.c : 4343*/
#define ECMINET033	(ECMINET + 33)    /*cm_inet.c : 3932*/
#define ECMINET034	(ECMINET + 34)    /*cm_inet.c : 4788*/
#define ECMINET035	(ECMINET + 35)    /*cm_inet.c : 5071*/
#define ECMINET036	(ECMINET + 36)    /*cm_inet.c : 5616*/
#define ECMINET037	(ECMINET + 37)    /*cm_inet.c : 5735*/
#define ECMINET038	(ECMINET + 38)    /*cm_inet.c : 5806*/
#define ECMINET039	(ECMINET + 39)    /*cm_inet.c : 5910*/
#define ECMINET040	(ECMINET + 40)    /*cm_inet.c : 6797*/
#define ECMINET041	(ECMINET + 41)    /*cm_inet.c : 6842*/
#define ECMINET042	(ECMINET + 42)    /*cm_inet.c : 6971*/
#define ECMINET043	(ECMINET + 43)    /*cm_inet.c : 7090*/
#define ECMINET044	(ECMINET + 44)    /*cm_inet.c : 7166*/
#define ECMINET045	(ECMINET + 45)    /*cm_inet.c : 7175*/
#define ECMINET046	(ECMINET + 46)    /*cm_inet.c : 7201*/
#define ECMINET047	(ECMINET + 47)    /*cm_inet.c : 7218*/
#define ECMINET048	(ECMINET + 48)    /*cm_inet.c : 7322*/
#define ECMINET049	(ECMINET + 49)    /*cm_inet.c : 7347*/
#define ECMINET050	(ECMINET + 50)    /*cm_inet.c : 7371*/
#define ECMINET051	(ECMINET + 51)    /*cm_inet.c : 7810*/
#define ECMINET052	(ECMINET + 52)    /*cm_inet.c : 8416*/
#define ECMINET053	(ECMINET + 53)    /*cm_inet.c : 8422*/
#define ECMINET054	(ECMINET + 54)    /*cm_inet.c : 8610*/
#define ECMINET055	(ECMINET + 55)    /*cm_inet.c : 8703*/
#define ECMINET056	(ECMINET + 56)    /*cm_inet.c : 8768*/
#define ECMINET057	(ECMINET + 57)    /*cm_inet.c : 1104*/
#define ECMINET058	(ECMINET + 58)    /*cm_inet.c : 1125*/
#define ECMINET059	(ECMINET + 59)    /*cm_inet.c : 1209*/
#define ECMINET060	(ECMINET + 60)    /*cm_inet.c : 1313*/
#define ECMINET061	(ECMINET + 61)    /*cm_inet.c : 1355*/
#define ECMINET062	(ECMINET + 62)    /*cm_inet.c : 1444*/
#define ECMINET063	(ECMINET + 63)    /*cm_inet.c : 1534*/
#define ECMINET064	(ECMINET + 64)    /*cm_inet.c : 1803*/
#define ECMINET065	(ECMINET + 65)    /*cm_inet.c : 1967*/
#define ECMINET066	(ECMINET + 66)    /*cm_inet.c : 2627*/
#define ECMINET067	(ECMINET + 67)    /*cm_inet.c : 2757*/
#define ECMINET068	(ECMINET + 68)    /*cm_inet.c : 4845*/
#define ECMINET069	(ECMINET + 69)    /*cm_inet.c : 5385*/
/* cm_inet_h_001.main_28: Added new error code ECMINET070 */
#define ECMINET070	(ECMINET + 70)    /*cm_inet.c : 5385*/
/* cm_inet_h_001.main_29: Added new error code ECMINET070 */
#define ECMINET071	(ECMINET + 71)    /*cm_inet.c : xxxx*/
#define ECMINETXXX	 ECMINET          /*cm_inet.c : xxxx*/


#endif /* __CMINETH__ */
/**********************************************************************
         End of file
**********************************************************************/
