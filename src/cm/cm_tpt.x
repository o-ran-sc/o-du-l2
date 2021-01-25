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
  
     Name:     Common transport file
  
     Type:     C include file
  
     Desc:     Common file for transport related data structures
  
     File:     cm_tpt.x
  
*********************************************************************21*/
 
#ifdef SS_4GMX_LCORE
#define __CMTPTX__ 1
#endif

#ifndef __CMTPTX__
#define __CMTPTX__


#include "cm_inet.x"

#ifdef CM_AAL
#include "cm_atm.x"
#endif

#ifdef __cplusplus
extern "C" {
#endif /* _cplusplus */

#ifdef __CMINETX__  /* Put cm_inet dependencies here */

typedef CmInetIpv4Addr  CmIpv4TptAddr;
typedef CmInetIpAddr    CmIpv4NetAddr;

#ifndef IPV6_SUPPORTED
typedef uint8_t  CmInetIpAddr6[16];   /* 16 byte IPV6 address */

/* IPV6 Address */
typedef struct cmInetIpv6Addr
{
   uint16_t             port;
   CmInetIpAddr6   ipv6NetAddr;
} CmInetIpv6Addr;
#endif /* IPV6_SUPPORTED */

/* IPV6 Network address */
typedef CmInetIpv6Addr  CmIpv6TptAddr;
typedef CmInetIpAddr6   CmIpv6NetAddr;

#endif /* __CMINETX__ */

/* Network address */
typedef struct cmNetAddr
{
   uint8_t   type;      /* type of network address */
   union
   {
      CmIpv4NetAddr  ipv4NetAddr; /* IP network address */
      CmIpv6NetAddr  ipv6NetAddr; /* IPv6 network address */
   }u;
} CmNetAddr;

/* list of addresses */
typedef struct _cmNetAddrTbl
{
   uint16_t             count;              /* Number of Table Entries */
   CmNetAddr       netAddr[CM_MAX_NET_ADDR]; /* IPV4/IPV6 addresses */
}CmNetAddrTbl;

/* Added new structure for Local Interface */
#ifdef LOCAL_INTF
/* used to pass local interface (on which packet was received) to upper user */
typedef struct cmTptLocalInf
{
   Bool      intfPrsnt;   /* valid intf is present or not */
   uint32_t       localIf;     /* interface index IPv4(32 bit) or IPv6(32 bit) */
   CmNetAddr localIfAddr; /* interface address */
}CmTptLocalInf;
#endif /* LOCAL_INTF */

#ifdef CM_INET2
/* Multicast information */
typedef struct cmNetMCastInf
{
   CmNetAddr         mCastAddr;   /* Multicast adddress */
   CmNetAddr         localAddr;   /* Local address */
} CmNetMCastInf;

#ifdef IPV6_SUPPORTED 
typedef CmInetMCastInf6 CmNetMCastInf6;
#endif /* IPV6_SUPPORTED */
#endif /* CM_INET2 */

/* Transport address */
typedef struct cmTptAddr
{
   uint8_t   type;                     /* type of transport address */
   union
   {
      CmIpv4TptAddr  ipv4TptAddr; /* IP transport address */
      CmIpv6TptAddr  ipv6TptAddr; /* IPv6 transport address */
#ifdef CM_AAL
      AtmVccId       aalTptAddr;  /* AAL transport address */
#endif
   }u;
} CmTptAddr;
 
/*cm_tpt_x_001.main_12 Updated for TUCL 2.1 Release (Kernel SCTP Support) */
#ifdef CM_LKSCTP
/* Array of Transport Addresses */
typedef struct cmTptAddrLst 
{
   uint8_t            nmb;                        /* Number of Network Addresses */
   CmTptAddr     tptAddr[CM_MAX_NET_ADDR];   /* List of Network Addresses */
} CmTptAddrLst;

typedef CmInetSockLinger           CmSockLinger;
typedef CmInetSctpSockEvent        CmSctpEvent;
typedef CmInetSctpPeerAddrParams   CmSctpPeerAddrParams;
typedef CmInetSctpPrimAddr         CmSctpPrimAddr;
typedef CmInetSctpPeerAddrInfo     CmSctpPeerAddrInfo;
typedef CmInetSctpStatus           CmSctpStatus;
typedef CmInetSctpRtoInfo          CmSctpRtoInfo;
typedef CmInetSctpInitMsg          CmSctpInitMsg;
typedef CmInetSctpAssocParams      CmSctpAssocParams;
#endif

/* Socket Options */
typedef struct cmSockOpts
{
   uint32_t  level;                    /* option level */
   uint32_t  option;                   /* option name */
   union                          /* option parameters */
   {
#ifdef CM_INET2
#ifdef IPV6_SUPPORTED 
      CmNetMCastInf6 mCastInfo6; /* IPV6 multicast information */
      uint32_t            infId;      /* IPV6 multicast outgoing interface */
#endif /* IPV6_SUPPORTED */
      CmNetMCastInf  mCastInfo;  /* multicast information */
#else
      CmNetAddr     mCastAddr;   /* multicast information */
#endif /* CM_INET2 */
      CmNetAddr     lclAddr;     /* local outgoing interface */
      uint32_t           value;       /* option value */
/*cm_tpt_x_001.main_12 Updated for TUCL 2.1 Release (Kernel SCTP Support) */
#ifdef CM_LKSCTP
      CmSockLinger           sockLinger;
      CmSctpEvent            sctpEvent;
      CmSctpPeerAddrParams   sctpPeerAddrParams;
      CmSctpPrimAddr         sctpPrimAddr;
      CmSctpPeerAddrInfo     sctpPeerAddrInfo;
      CmSctpStatus           sctpStatus;
/*cm_tpt_x_001.main_13 Updated for the support of configurable RTO parameters, 
                        HBeat value Max retransmissions (Init, Path, Association)*/
      CmSctpRtoInfo          sctpRtoInfo;
      CmSctpInitMsg          sctpInitMsg;
      CmSctpAssocParams      sctpPeerAssocParams;
#endif
   }optVal;
}CmSockOpts;

/* socket parameters */
typedef struct cmSockParam
{
   uint8_t             listenQSize;    /* listen queue size */
   uint8_t             numOpts;        /* number of socket options */
   CmSockOpts sockOpts[CM_MAX_SOCK_OPTS]; /* socket options */
} CmSockParam;

#ifdef CM_TLS
typedef struct tlsTptParam
{
   S16          ctxId;
   uint8_t           listenQSize;
   uint8_t           numOpts;
   CmSockOpts   sockOpts[CM_MAX_SOCK_OPTS];

} TlsTptParam;
#endif /* CM_TLS */

/*cm_tpt_x_001.main_12 Updated for TUCL 2.1 Release (Kernel SCTP Support) */
#ifdef CM_LKSCTP
typedef struct sctpSockParam
{
   uint8_t           numOpts;        /* number of socket options */
   CmSockOpts   sockOpts[CM_MAX_SOCK_OPTS]; /* socket options */
} SctpSockParam;
#endif

/* Transport parameters */
typedef struct cmTptParam
{
   uint8_t   type;                      /* type of transport parameters */

   union
   {
      CmSockParam  sockParam;      /* socket parameters */
#ifdef CM_AAL
      AalConParam  aalParam;       /* AAL connection parameters */
#endif
#ifdef CM_TLS
      TlsTptParam  tlsParam;       /* TLS parameters */
#endif
/*cm_tpt_x_001.main_12 Updated for TUCL 2.1 Release (Kernel SCTP Support) */
#ifdef CM_LKSCTP
      SctpSockParam  sctpParam;    /* LKSCTP parameters */
#endif
   } u;

} CmTptParam;

#ifdef CM_INET2  
/* IPv4 header parameters */
typedef struct cmIpv4HdrParm 
{
   TknUInt8    proto;                 /* Protocol value */
   TknUInt8    dfBit;                 /* Don't fragment flag */
   TknUInt8    tos;                   /* Type of Service */
   TknUInt8    ttl;                   /* Time to Live */
   /* added new field */
#ifdef IPV4_OPTS_SUPPORTED 
   TknStr64 ipv4HdrOpt;            /* IPV4 hdr opt */
#endif /* IPV4_OPTS_SUPPORTED */
} CmIpv4HdrParm;

#ifdef IPV6_SUPPORTED 
/* added new structures for IPv6 ext hdr support */
#ifdef IPV6_OPTS_SUPPORTED
/* structure to hold TLV of each HBH option */
typedef struct cmIpv6HBHHdr
{
   uint8_t type;
   uint8_t length;
   uint8_t *value;
} CmIpv6HBHHdr;

/* structure to hold TLV of each Destination option */
typedef struct cmIpv6DestOptsHdr
{
   uint8_t type;
   uint8_t length;
   uint8_t *value;
} CmIpv6DestOptsHdr;

/* structure to hold IPV6 addresses of the Route header */
typedef struct cmIpv6RtHdr
{
   uint8_t numAddrs;               
   uint32_t slMap;
   CmIpv6NetAddr *ipv6Addrs;   
} CmIpv6RtHdr;

/* array of all HBH options */
typedef struct cmIpv6HBHHdrArr
{
   uint8_t numHBHOpts;
   CmIpv6HBHHdr *hbhOpts; 
} CmIpv6HBHHdrArr;

/* array of all Destination options */
typedef struct cmIpv6DestOptsArr
{
   uint8_t numDestOpts;
   CmIpv6DestOptsHdr *destOpts;
} CmIpv6DestOptsArr;

/* structure having 3 substructures for 3 types of ext headers */
typedef struct cmIpv6ExtHdr
{
   Bool hbhHdrPrsnt;
   CmIpv6HBHHdrArr hbhOptsArr;

   Bool destOptsPrsnt;
   CmIpv6DestOptsArr destOptsArr;

   Bool rtOptsPrsnt;
   CmIpv6RtHdr rtOpts;
} CmIpv6ExtHdr;
#endif /* IPV6_OPTS_SUPPORTED */

/* IPV6 header parameters */
typedef struct cmIpv6HdrParm
{
   TknUInt8    ttl;                   /* Set the hop limit */
   
   /* added new field */
   CmNetAddr srcAddr6; /* src addr to set on send pkt(IPv6) */

   /* added new field */
#ifdef IPV6_OPTS_SUPPORTED 
   CmIpv6ExtHdr ipv6ExtHdr;      
#endif /* IPV6_OPTS_SUPPORTED */  
} CmIpv6HdrParm;
#endif /* IPV6_SUPPORTED */

/* IP header paramters */
typedef struct cmIpHdrParm 
{ 
   uint8_t       type;                  /* Type of IP header parameters */
   union 
   {
      CmIpv4HdrParm  hdrParmIpv4;  /* IPv4 header parameters */
#ifdef IPV6_SUPPORTED 
      CmIpv6HdrParm  hdrParmIpv6;  /* IPv6 header parameters */
#endif /* IPV6_SUPPORTED */
   }u;
   
} CmIpHdrParm;

/* IPv4 header */
typedef struct cmIpv4Hdr 
{
   uint8_t    hdrVer;                   /* Header and Version */
   uint8_t    tos;                      /* Type Of Service */
   S16   length;                   /* Total length */
   uint16_t   id;                       /* Identification */
   S16   off;                      /* Flags and Offset */
   uint8_t    ttl;                      /* Time to Live */
   uint8_t    proto;                    /* Protocol */
   uint16_t   chkSum;                   /* Checksum */
   uint32_t   srcAddr;                  /* Source Address */
   uint32_t   destAddr;                 /* Destination Address */

} CmIpv4Hdr;

#ifdef IPV6_SUPPORTED 
/* IPv6 header */
typedef struct cmIpv6Hdr
{
   union
   {
      struct ip6_hdrctl
      {
         uint32_t      ip6_un1_flow;
         uint16_t      ip6_un1_plen;
         uint8_t       ip6_un1_nxt;
         uint8_t       ip6_un1_hlim;
      } ip6_un1;
      uint8_t    ip6_un2_vfc;
   } ip6_ctlun;
   CmIpv6NetAddr  ip6_src;
   CmIpv6NetAddr  ip6_dst;
} CmIpv6Hdr;
#endif /* IPV6_SUPPORTED */

typedef struct cmIcmpError 
{
   uint8_t    errType;                  /* ICMP Error Type */
   uint32_t   errCodeMask;              /* ICMP Error Code Mask */

} CmIcmpError;

/* ICMP v4 filter parameters */
typedef struct cmIcmpv4Filter
{
   uint8_t    icmpMsgFlag;              /* Flag to listen to any ICMP msgs */
   uint8_t    allMsg;                   /* Flag to listen to ICMP messages */
   uint8_t    protocol;                 /* ICMP packets with protocol only */
   uint8_t    num;                      /* Number of valid type - code 
                                    * combinations in the error array */
   CmIcmpError icmpError[CM_MAX_ICMP_ERROR]; /* Error type & Code array */

}CmIcmpv4Filter;

#ifdef IPV6_SUPPORTED
typedef struct cmIcmpv6Filter
{
   uint8_t    icmpMsgFlag;              /* Flag to listen to any ICMP msgs */
   uint8_t    allMsg;                   /* Flag to listen to all ICMP messages */
   uint8_t    num;                      /* Number of valid type - code 
                                    * combinations in the error array */
   CmIcmpError icmpError[CM_MAX_ICMP_ERROR]; /* Error type & Code array */

} CmIcmpv6Filter; 
#endif /* IPV6_SUPPORTED */

/* ICMP filter paramters */
typedef struct cmIcmpFilter 
{
  uint8_t type;                        /* ICMP version */
  union 
  {
    CmIcmpv4Filter icmpv4Filter;  /* ICMPv4 filter structure */
#ifdef IPV6_SUPPORTED
    CmIcmpv6Filter icmpv6Filter;  /* ICMPv6 filter structure */
#endif /* IPV6_SUPPORTED */
  }u;

} CmIcmpFilter; 

/* ICMP header */
typedef struct cmIcmpv4Hdr 
{
   uint8_t    icmpType;                /* Type of message */
   uint8_t    icmpCode;                /* Message code */
   uint16_t   chkSum;                  /* Ones complement cksum of struct */
   
   union 
   {
      uint8_t   ihPptr;                /* ICMP parameter problem */
      uint32_t  rdrctAddr;             /* ICMP redirect address */
      struct idSeq 
      {
         S16   icdId;             /* Identifier */                          
         S16   icdSeq;            /* Sequence Number */
      } u1;
      uint32_t ihVoid;                 
   } u2;
   
   union 
   {
      struct idTime
      {
         uint32_t  itOtime;            /* Original time stamp */
         uint32_t  itRtime;            /* Received time stamp */
         uint32_t  itTtime;            /* Transmit time stamp */
      } s;
      CmIpv4Hdr icmpIpHdr;        /* IP header */
      uint32_t     id_mask;
   } u3;

} CmIcmpv4Hdr;

#ifdef IPV6_SUPPORTED
typedef struct cmIcmpv6Hdr
{
   uint8_t    icmp6_type;              /* type field */
   uint8_t    icmp6_code;              /* code field */
   uint16_t   icmp6_cksum;             /* checksum field */

   union
   {
      uint32_t   icmp6_un_data32[1];   /* type-specific field */
      uint16_t   icmp6_un_data16[2];   /* type-specific field */ 
      uint8_t    icmp6_un_data8[4];    /* type-specific field */
   } icmp6_dataun;
} CmIcmpv6Hdr;
#endif /* IPV6_SUPPORTED */

#endif  /* CM_INET2 */ 

/* packing/unpacking function prototypes */
/* Added packing and unpacking function prototypes */
S16 cmPkCmIpv4TptAddr    ARGS((CmIpv4TptAddr *pkParam, Buffer *mBuf));
S16 cmPkCmNetAddrTbl     ARGS((CmNetAddrTbl *pkParam, Buffer *mBuf));
S16 cmPkCmNetAddr        ARGS((CmNetAddr *pkParam, Buffer *mBuf));
S16 cmPkCmTptAddr        ARGS((CmTptAddr *pkParam, Buffer *mBuf));
#ifdef IPV6_SUPPORTED
S16 cmPkCmNetMCastInf6   ARGS((CmNetMCastInf6 *pkParam, Buffer *mBuf));
#endif /* IPV6_SUPPORTED */
S16 cmPkCmTptParam       ARGS((CmTptParam *pkParam, Buffer *mBuf));
S16 cmUnpkCmNetAddrTbl   ARGS((CmNetAddrTbl *unpkParam, Buffer *mBuf));
S16 cmUnpkCmIpv4TptAddr  ARGS((CmIpv4TptAddr *unpkParam, Buffer *mBuf));
S16 cmUnpkCmNetAddr      ARGS((CmNetAddr *unpkParam, Buffer *mBuf));
S16 cmUnpkCmTptAddr      ARGS((CmTptAddr *unpkParam, Buffer *mBuf));
#ifdef IPV6_SUPPORTED
S16 cmUnpkCmNetMCastInf6 ARGS((CmNetMCastInf6 *unpkParam, Buffer *mBuf));
#endif /* IPV6_SUPPORTED */

S16 cmUnpkCmTptParam     ARGS((CmTptParam *unpkParam, Buffer *mBuf));

#ifdef CM_INET2  
S16 cmPkCmIpHdrParm     ARGS((CmIpHdrParm *pkParam, Buffer *mBuf));

/* changed to include meminfo required to hold IPv6 
                         extension headers */
#ifdef IPV6_OPTS_SUPPORTED
S16 cmUnpkCmIpHdrParm    ARGS((CmIpHdrParm *unpkParam, Buffer *mBuf, 
                                     Mem *memInfo));
#else
S16 cmUnpkCmIpHdrParm    ARGS((CmIpHdrParm *unpkParam, Buffer *mBuf));
#endif

/* added new packing/unpacking function */
#ifdef LOCAL_INTF
S16 cmPkCmTptLocalInf    ARGS((CmTptLocalInf *pkParam, Buffer *mBuf));
S16 cmUnpkCmTptLocalInf  ARGS((CmTptLocalInf *unpkParam, Buffer *mBuf));
#endif /* LOCAL_INTF */

S16 cmPkCmIcmpFilter     ARGS((CmIcmpFilter *pkParam, Buffer *mBuf));
S16 cmUnpkCmIcmpFilter   ARGS((CmIcmpFilter *unpkParam, Buffer *mBuf));

/* added new packing/unpacking functions */
#ifdef IPV6_OPTS_SUPPORTED
S16 cmPkCmIpv6ExtHdr     ARGS((CmIpv6ExtHdr *pkParam, Buffer *mBuf));
S16 cmPkCmIpv6RtHdr      ARGS((CmIpv6RtHdr *pkParam, Buffer *mBuf));
S16 cmUnpkCmIpv6RtHdr    ARGS((CmIpv6RtHdr *unpkParam, Buffer *mBuf, 
                                     Mem *memInfo));
S16 cmUnpkCmIpv6ExtHdr   ARGS((CmIpv6ExtHdr *unpkParam, Buffer *mBuf, 
                                     Mem *memInfo));
S16 cmPkCmIpv6DestOptsArr ARGS((CmIpv6DestOptsArr *pkParam, 
                                      Buffer *mBuf));
S16 cmPkCmIpv6DestOptsHdr ARGS((CmIpv6DestOptsHdr *pkParam, 
                                      Buffer *mBuf));
S16 cmUnpkCmIpv6DestOptsHdr ARGS((CmIpv6DestOptsHdr *unpkParam, 
                                      Buffer *mBuf, Mem *memInfo));
S16 cmUnpkCmIpv6DestOptsArr ARGS((CmIpv6DestOptsArr *unpkParam, 
                                      Buffer *mBuf, Mem *memInfo));
S16 cmPkCmIpv6HBHHdrArr  ARGS((CmIpv6HBHHdrArr *pkParam, Buffer *mBuf));
S16 cmPkCmIpv6HBHHdr     ARGS((CmIpv6HBHHdr *pkParam, Buffer *mBuf));
S16 cmUnpkCmIpv6HBHHdr   ARGS((CmIpv6HBHHdr *unpkParam, Buffer *mBuf, 
                                     Mem *memInfo));
S16 cmUnpkCmIpv6HBHHdrArr ARGS((CmIpv6HBHHdrArr *unpkParam, 
                                      Buffer *mBuf, Mem *memInfo));
#endif /* IPV6_OPTS_SUPPORTED */
#endif  /* CM_INET2 */ 

#ifdef __cplusplus
}
#endif

#endif /* __CMTPTX__ */

/********************************************************************30**
         End of file
*********************************************************************31*/
