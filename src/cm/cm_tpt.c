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
  
     Name:     HCT Interface
  
     Type:     C file
  
     Desc:     This file contains the packing/unpacking functions
               for the H.225 primitives on hct interface

     File:     cm_tpt.c

*********************************************************************21*/

/* header include files (.h) */

#include "envopt.h"        /* environment options */  
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */

#include "cm_tpt.h"

/* header/extern include files (.x) */

#include "gen.x"           /* general layer */
#include "ssi.x"           /* system service interface */

/*#include "cm_tkns.x"*/
#include "cm_tpt.x"


/* local defines */

/* local typedefs */

/* local externs */
  
/* forward references */

/* local function definition */

/* functions in other modules */

/* public variable declarations */

/* control variables for testing */

/* private variable declarations */

/*
 * support functions
 */

/*  cm_tpt_c_001.main_17 - Guarded under LCEGT and LCLEG */
/*  cm_tpt_c_001.main_18 - Guarded under LWLCEGT */

#if    (   defined(LCHCT) || defined (LWLCHCT) || defined(LCHIT) \
        || defined(LCLHC) || defined(TRIL_HZ) \
        || defined(LCHRT) || defined(LCLHR)   \
        || defined(LCLMG) || defined(LCMGT)   \
        || defined(LCLHG) || defined(LCHGT)   \
        || defined(LCSCT) || defined(LCLSB)   \
        || defined(LCLNT) || defined(LCLLN)   \
        || defined(LCLSO) || defined(LCSOT)   \
        || defined(LCEGT) || defined(LCLEG)   || defined(LWLCEGT) \
        || defined(CM_COMPRESS)   \
       )

#ifdef __cplusplus
extern "C" {
#endif
   /* Add prototypes here  (MB) */

#ifdef __cplusplus
}
#endif

  

/*
 *      PACKING FUNCTIONS
 */
/* Moving IPv4 address packing functions from cm_inet.c */
/*
*
*       Fun:   cmPkCmIpv4TptAddr
*
*       Desc:  This function packs the IPv4 address 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmPkCmIpv4TptAddr
(
CmIpv4TptAddr            *pkParam,  /* IPv4 Address structure */
Buffer                   *mBuf      /* message buffer */
)
{

   CMCHKPK(cmPkCmIpv4NetAddr, pkParam->address, mBuf);
   CMCHKPK(oduUnpackUInt16, pkParam->port, mBuf);

   return ROK;
} /* cmPkCmIpv4TptAddr */

/*
*
*       Fun:   cmPkCmIpv6NetAddr
*
*       Desc:  This function packs the 16 bytes of IPv6 address 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
static S16 cmPkCmIpv6NetAddr
(
CmIpv6NetAddr  *pkParam,     
Buffer         *mBuf         /* message buffer */
)
{
   uint8_t num;
   uint8_t *ptr = (uint8_t*)pkParam;


   for(num = 0; num < CM_IPV6ADDR_SIZE; num++)
   {
      CMCHKPK(oduUnpackUInt8, *(ptr+num), mBuf);
   }

   return ROK;
} /* end of cmPkCmIpv6NetAddr */


/*
*
*       Fun:   cmPkCmIpv6TptAddr
*
*       Desc:  This function packs the IPv6 transport address
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
static S16 cmPkCmIpv6TptAddr
(
CmIpv6TptAddr  *pkParam,     /* IPv6 transport address */
Buffer         *mBuf         /* message buffer */
)
{

   CMCHKPK(cmPkCmIpv6NetAddr, &pkParam->ipv6NetAddr, mBuf);
   CMCHKPK(oduUnpackUInt16, pkParam->port, mBuf);

   return ROK;
}   /* cmPkCmIpv6TptAddr */


/*
*
*       Fun:   cmPkCmNetAddrTbl
*
*       Desc:  This function packs the 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmPkCmNetAddrTbl
(
CmNetAddrTbl   *pkParam,      /* Network Address Table */
Buffer         *mBuf          /* message buffer */
)
{
   uint16_t    idx;           /* Loop Index */
   CmNetAddr   *netAddr;      /* Network Address */


   if (pkParam->count > CM_MAX_NET_ADDR)
      return RFAILED;

   /* Pack All the addresses */
   for (idx = pkParam->count; idx; idx--)
   {
      netAddr = &(pkParam->netAddr[idx - 1]);

      if ((cmPkCmNetAddr(netAddr, mBuf)) != ROK)
         return RFAILED;
   }

   /* Pack the total number of addresses present in the table */
   CMCHKPK(oduUnpackUInt16, pkParam->count, mBuf);

   return ROK;

} /* end of cmPkCmNetAddrTbl() */

/*
*
*       Fun:   cmPkCmNetAddr
*
*       Desc:  This function packs the 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmPkCmNetAddr
(
CmNetAddr  *pkParam,     
Buffer     *mBuf         /* message buffer */
)
{

   switch (pkParam->type)
   {
      case CM_NETADDR_NOTPRSNT:
         break;

      case CM_NETADDR_IPV4:
         CMCHKPK(cmPkCmIpv4NetAddr, pkParam->u.ipv4NetAddr, mBuf);
         break;

      case CM_NETADDR_IPV6:
         CMCHKPK(cmPkCmIpv6NetAddr, &pkParam->u.ipv6NetAddr, mBuf);
         break;

      default:
         return RFAILED;
   }
   CMCHKPK(oduUnpackUInt8, pkParam->type, mBuf);

   return ROK;
}   /* cmPkCmNetAddr */


/*
*
*       Fun:   cmPkCmTptAddr
*
*       Desc:  This function packs the 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmPkCmTptAddr
(
CmTptAddr  *pkParam,     
Buffer     *mBuf         /* message buffer */
)
{

   switch (pkParam->type)
   {
      case CM_TPTADDR_NOTPRSNT:
         break;

      case CM_TPTADDR_IPV4:
         CMCHKPK(cmPkCmIpv4TptAddr, &pkParam->u.ipv4TptAddr, mBuf);
         break;

      case CM_TPTADDR_IPV6:
         CMCHKPK(cmPkCmIpv6TptAddr, &pkParam->u.ipv6TptAddr, mBuf);
         break;
   
      default:
         return RFAILED;
   }
   CMCHKPK(oduUnpackUInt8, pkParam->type, mBuf);

   return ROK;
}   /* cmPkCmTptAddr */

/* added new packing functions */
#ifdef LOCAL_INTF
/*
*
*       Fun:   cmPkCmTptLocalInf
*
*       Desc:  This function packs the local interface info on which IPV4/IPV6
*              packet was received on.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmPkCmTptLocalInf
(
CmTptLocalInf  *pkParam,   /* local interface info */
Buffer         *mBuf       /* message buffer */
)
{

   if (pkParam->intfPrsnt == TRUE)
   {  
      /* pack the actual interface address */ 
      CMCHKPK(cmPkCmNetAddr, &pkParam->localIfAddr, mBuf); 
      /* pack the interface index value */
      CMCHKPK(oduUnpackUInt32, pkParam->localIf, mBuf);
   }
   /* pack the boll which indicates if valid local intf is present or not */
   CMCHKPK(oduUnpackUInt8, pkParam->intfPrsnt, mBuf);
   return ROK;
}   /* cmPkCmTptLocalInf */
#endif /* LOCAL_INTF */

/* Moving IPv6 multicast information packing functions from cm_inet.c */
#ifdef IPV6_SUPPORTED
/*
*
*       Fun:   cmPkCmNetMCastInf6
*
*       Desc:  This function packs the IPv6 multicast information 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmPkCmNetMCastInf6
(
CmNetMCastInf6  *pkParam,     /* IPv6 multicast information */
Buffer          *mBuf         /* message buffer */
)
{

   CMCHKPK(cmPkCmIpv6NetAddr, &pkParam->mCastAddr, mBuf);
   CMCHKPK(oduUnpackUInt32, pkParam->localInf, mBuf);

   return ROK;
}   /* cmPkCmNetMCastInf6 */
#endif /* IPV6_SUPPORTED */


/*
*
*       Fun:   cmPkCmSockOpts
*
*       Desc:  This function packs the 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
static S16 cmPkCmSockOpts
(
CmSockOpts *pkParam,     
Buffer     *mBuf         /* message buffer */
)
{

   switch (pkParam->option)
   {
      case CM_SOCKOPT_OPT_ADD_MCAST_MBR:
      case CM_SOCKOPT_OPT_DRP_MCAST_MBR:
#ifdef CM_INET2
         CMCHKPK(cmPkCmNetAddr, &pkParam->optVal.mCastInfo.mCastAddr, mBuf);
         CMCHKPK(cmPkCmNetAddr, &pkParam->optVal.mCastInfo.localAddr, mBuf);
#else
         CMCHKPK(cmPkCmNetAddr, &pkParam->optVal.mCastAddr, mBuf);
#endif /* CM_INET2 */
         break;

      case CM_SOCKOPT_OPT_MCAST_IF:
         CMCHKPK(cmPkCmNetAddr, &pkParam->optVal.lclAddr, mBuf);
         break;

#ifdef IPV6_SUPPORTED
      case CM_SOCKOPT_OPT_ADD_MCAST6_MBR:
      case CM_SOCKOPT_OPT_DRP_MCAST6_MBR:
         CMCHKPK(cmPkCmNetMCastInf6, &pkParam->optVal.mCastInfo6, mBuf);
         break;

      case CM_SOCKOPT_OPT_MCAST6_IF:
         CMCHKPK(oduUnpackUInt32, pkParam->optVal.infId, mBuf);
         break;

#endif /* IPV6_SUPPORTED */

      default:
         CMCHKPK(oduUnpackUInt32, pkParam->optVal.value, mBuf);
         break;
   }
   CMCHKPK(oduUnpackUInt32, pkParam->option, mBuf);
   CMCHKPK(oduUnpackUInt32, pkParam->level, mBuf);

   return ROK;
}   /* cmPkCmSockOpts */


/*
*
*       Fun:   cmPkCmSockParam
*
*       Desc:  This function packs the 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
static S16 cmPkCmSockParam
(
CmSockParam *pkParam,     
Buffer      *mBuf         /* message buffer */
)
{
   uint32_t num;


   if( pkParam->numOpts > CM_MAX_SOCK_OPTS)
   {
      return RFAILED;
   }
   for(num = 0; num < pkParam->numOpts; num++)
   {
      CMCHKPK(cmPkCmSockOpts, &pkParam->sockOpts[num], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, pkParam->numOpts, mBuf);
   CMCHKPK(oduUnpackUInt8, pkParam->listenQSize, mBuf);

   return ROK;
}   /* cmPkCmSockParam */


#ifdef CM_TLS
/*
*
*       Fun:   cmPkTlsTptParam
*
*       Desc:  This function packs the 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
static S16 cmPkTlsTptParam
(
TlsTptParam *pkParam,     /**/
Buffer      *mBuf         /* message buffer */
)
{
   uint32_t num;


   if( pkParam->numOpts > CM_MAX_SOCK_OPTS)
   {
      return RFAILED;
   }
   for(num = 0; num < pkParam->numOpts; num++)
   {
      CMCHKPK(cmPkCmSockOpts, &pkParam->sockOpts[num], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, pkParam->numOpts, mBuf);
   CMCHKPK(oduUnpackUInt8, pkParam->listenQSize, mBuf);
   CMCHKPK(SPkS16, pkParam->ctxId, mBuf);

   return ROK;
}  /* cmPkTlsTptParam */

#endif  /* CM_TLS */


/*
*
*       Fun:   cmPkCmTptParam
*
*       Desc:  This function packs the 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmPkCmTptParam
(
CmTptParam *pkParam,     
Buffer     *mBuf         /* message buffer */
)
{

   switch (pkParam->type)
   {
      case CM_TPTPARAM_NOTPRSNT:
         break;

      case CM_TPTPARAM_SOCK:
         CMCHKPK(cmPkCmSockParam, &pkParam->u.sockParam, mBuf);
         break;

#ifdef CM_AAL
      case CM_TPTPARAM_AAL:
         CMCHKPK(cmPkAalConParam, &pkParam->u.aalParam, mBuf);
         break;
#endif

#ifdef CM_TLS
      case CM_TPTPARAM_TLS:
         CMCHKPK(cmPkTlsTptParam, &pkParam->u.tlsParam, mBuf);
         break;
#endif

      default:
         return RFAILED;
   }

   CMCHKPK(oduUnpackUInt8, pkParam->type, mBuf);

   return ROK;
}   /* cmPkCmTptParam */

  
/*
 *      UNPACKING FUNCTIONS
 */

/* Moving IPv4 address un-packing functions from cm_inet.c file */

/*
*
*       Fun:   cmUnpkCmIpv4TptAddr
*
*       Desc:  This function unpacks the IPv4 address
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmUnpkCmIpv4TptAddr 
(
CmIpv4TptAddr *unpkParam,     /* IPv4 Address */
Buffer        *mBuf           /* message buffer */
)
{

   CMCHKUNPK(oduPackUInt16, &unpkParam->port, mBuf);
   CMCHKUNPK(cmUnpkCmIpv4NetAddr, &unpkParam->address, mBuf);

   return ROK;
}   /* cmUnpkCmIpv4TptAddr */


/*
*
*       Fun:   cmUnpkCmIpv6NetAddr
*
*       Desc:  This function unpacks the 16 bytes of IPv6 address 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
static S16 cmUnpkCmIpv6NetAddr
(
CmIpv6NetAddr  *unpkParam,   /* IPv6 address */
Buffer         *mBuf         /* message buffer */
)
{
   uint32_t num;
   uint8_t *ptr = (uint8_t*)unpkParam;


   ptr += (CM_INET_IPV6ADDR_SIZE - 1);

   for(num = 0; num < CM_IPV6ADDR_SIZE; num++)
   {
      CMCHKUNPK(oduPackUInt8, (ptr-num), mBuf);
   }

   return ROK;
} /* end of cmUnpkCmIpv6NetAddr */


/*
*
*       Fun:   cmUnpkCmIpv6TptAddr
*
*       Desc:  This function unpacks the IPv6 transport address
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
static S16 cmUnpkCmIpv6TptAddr
(
CmIpv6TptAddr  *unpkParam,   /* IPv6 transport address */
Buffer         *mBuf         /* message buffer */
)
{

   CMCHKUNPK(oduPackUInt16, &unpkParam->port, mBuf);
   CMCHKUNPK(cmUnpkCmIpv6NetAddr, &unpkParam->ipv6NetAddr, mBuf);

   return ROK;
}   /* cmUnpkCmIpv6TptAddr */


/*
*
*       Fun:   cmUnpkCmNetAddrTbl
*
*       Desc:  This function unpacks the 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmUnpkCmNetAddrTbl
(
CmNetAddrTbl  *unpkParam,    /* Network Address Table */
Buffer        *mBuf          /* message buffer */
)
{
   uint16_t   idx;           /* Loop Index */
   CmNetAddr  *netAddr;      /* Network Address */
   
   /* Unpack the count */
   CMCHKUNPK(oduPackUInt16, &(unpkParam->count), mBuf);

   /* Unpack the addresses */
   for (idx = 0; idx < unpkParam->count; idx++)
   {
      netAddr = &(unpkParam->netAddr[idx]);

      if ((cmUnpkCmNetAddr(netAddr, mBuf)) != ROK)
         return RFAILED;
   }

   return ROK;

} /* end of cmUnpkCmNetAddrTbl() */


/*
*
*       Fun:   cmUnpkCmNetAddr
*
*       Desc:  This function unpacks the network address
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmUnpkCmNetAddr
(
CmNetAddr *unpkParam,   
Buffer    *mBuf         /* message buffer */
)
{

   CMCHKUNPK(oduPackUInt8, &unpkParam->type, mBuf);

   switch (unpkParam->type)
   {
      case CM_NETADDR_NOTPRSNT:
         break;

      case CM_NETADDR_IPV4:
         CMCHKUNPK(cmUnpkCmIpv4NetAddr, &unpkParam->u.ipv4NetAddr, mBuf);
         break;

      case CM_NETADDR_IPV6:
         CMCHKUNPK(cmUnpkCmIpv6NetAddr, &unpkParam->u.ipv6NetAddr, mBuf);
         break;

      default:
         return RFAILED;
   }

   return ROK;
}   /* cmUnpkCmNetAddr */


/*
*
*       Fun:   cmUnpkCmTptAddr
*
*       Desc:  This function unpacks the 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmUnpkCmTptAddr
(
CmTptAddr *unpkParam,   
Buffer    *mBuf         /* message buffer */
)
{

   CMCHKUNPK(oduPackUInt8, &unpkParam->type, mBuf);

   switch (unpkParam->type)
   {
      case CM_TPTADDR_NOTPRSNT:
         break;

      case CM_TPTADDR_IPV4:
         CMCHKUNPK(cmUnpkCmIpv4TptAddr, &unpkParam->u.ipv4TptAddr, mBuf);
         break;

      case CM_TPTADDR_IPV6:
         CMCHKUNPK(cmUnpkCmIpv6TptAddr, &unpkParam->u.ipv6TptAddr, mBuf);
         break;

      default:
         return RFAILED;
   }

   return ROK;
}   /* cmUnpkCmTptAddr */

/* Moving IPv6 multicast information unpacking functions from cm_inet.c */
#ifdef IPV6_SUPPORTED

/*
*
*       Fun:   cmUnpkCmNetMCastInf6
*
*       Desc:  This function unpacks the IPv6 multicast information 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmUnpkCmNetMCastInf6
(
CmNetMCastInf6 *unpkParam,   /* IPv6 multicast information */
Buffer         *mBuf         /* message buffer */
)
{

    CMCHKUNPK(oduPackUInt32, &unpkParam->localInf, mBuf);
    CMCHKUNPK(cmUnpkCmIpv6NetAddr, &unpkParam->mCastAddr, mBuf);

   return ROK;
}   /* cmUnpkCmNetMCastInf6 */
#endif /* IPV6_SUPPORTED */


/*
*
*       Fun:   cmUnpkCmSockOpts
*
*       Desc:  This function unpacks the socket options
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
static S16 cmUnpkCmSockOpts
(
CmSockOpts *unpkParam,   /* socket options */
Buffer     *mBuf         /* message buffer */
)
{

   CMCHKUNPK(oduPackUInt32, &unpkParam->level, mBuf);
   CMCHKUNPK(oduPackUInt32, &unpkParam->option, mBuf);

   switch (unpkParam->option)
   {
      case CM_SOCKOPT_OPT_ADD_MCAST_MBR:
      case CM_SOCKOPT_OPT_DRP_MCAST_MBR:
#ifdef CM_INET2
         CMCHKUNPK(cmUnpkCmNetAddr, &unpkParam->optVal.mCastInfo.localAddr, 
                   mBuf);
         CMCHKUNPK(cmUnpkCmNetAddr, &unpkParam->optVal.mCastInfo.mCastAddr, 
                   mBuf);
#else
         CMCHKUNPK(cmUnpkCmNetAddr, &unpkParam->optVal.mCastAddr, mBuf);
#endif /* CM_INET2 */
         break;

      case CM_SOCKOPT_OPT_MCAST_IF:
         CMCHKUNPK(cmUnpkCmNetAddr, &unpkParam->optVal.lclAddr, mBuf);
         break;

#ifdef IPV6_SUPPORTED
      case CM_SOCKOPT_OPT_ADD_MCAST6_MBR:
      case CM_SOCKOPT_OPT_DRP_MCAST6_MBR:
         CMCHKUNPK(cmUnpkCmNetMCastInf6, &unpkParam->optVal.mCastInfo6, mBuf);
         break;

      case CM_SOCKOPT_OPT_MCAST6_IF:
         CMCHKUNPK(oduPackUInt32, &unpkParam->optVal.infId, mBuf);
         break;
#endif /* IPV6_SUPPORTED */

      default:
         CMCHKUNPK(oduPackUInt32, &unpkParam->optVal.value, mBuf);
         break;
   }

   return ROK;
}   /* cmUnpkCmSockOpts */


/*
*
*       Fun:   cmUnpkCmSockParam
*
*       Desc:  This function unpacks the socket parameters
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
static S16 cmUnpkCmSockParam
(
CmSockParam *unpkParam,   /* socket parameters */
Buffer      *mBuf         /* message buffer */
)
{
   uint32_t num;


   CMCHKUNPK(oduPackUInt8, &unpkParam->listenQSize, mBuf);
   CMCHKUNPK(oduPackUInt8, &unpkParam->numOpts, mBuf);

   if( unpkParam->numOpts > CM_MAX_SOCK_OPTS)
   {
      return RFAILED;
   }

   for(num = 0; num < unpkParam->numOpts; num++)
   {
      CMCHKUNPK(cmUnpkCmSockOpts, &unpkParam->sockOpts[num], mBuf);
   }

   return ROK;
}   /* cmUnpkCmSockParam */


#ifdef CM_TLS
/*
*
*       Fun:   cmUnpkTlsTptParam
*
*       Desc:  This function unpacks the socket parameters
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
static S16 cmUnpkTlsTptParam
(
TlsTptParam *unpkParam,   /* TLS parameters */
Buffer      *mBuf         /* message buffer */
)
{
   uint32_t num;


   CMCHKUNPK(SUnpkS16, &unpkParam->ctxId, mBuf);
   CMCHKUNPK(oduPackUInt8, &unpkParam->listenQSize, mBuf);
   CMCHKUNPK(oduPackUInt8, &unpkParam->numOpts, mBuf);

   if( unpkParam->numOpts > CM_MAX_SOCK_OPTS)
   {
      return RFAILED;
   }

   for(num = 0; num < unpkParam->numOpts; num++)
   {
      CMCHKUNPK(cmUnpkCmSockOpts, &unpkParam->sockOpts[num], mBuf);
   }

   return ROK;
}  /* cmUnpkTlsTptParam */

#endif  /* CM_TLS */


/*
*
*       Fun:   cmUnpkCmTptParam
*
*       Desc:  This function unpacks the transport parameters 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmUnpkCmTptParam
(
CmTptParam  *unpkParam,   /* transport parameters */
Buffer      *mBuf         /* message buffer */
)
{

   CMCHKUNPK(oduPackUInt8, &unpkParam->type, mBuf);

   switch (unpkParam->type)
   {
      case CM_TPTPARAM_NOTPRSNT:
         break;

      case CM_TPTPARAM_SOCK:
         CMCHKUNPK(cmUnpkCmSockParam, &unpkParam->u.sockParam, mBuf);
         break;

#ifdef CM_AAL
      case CM_TPTPARAM_AAL:
         CMCHKUNPK(cmUnpkAalConParam, &unpkParam->u.aalParam, mBuf);
         break;
#endif

#ifdef CM_TLS
      case CM_TPTPARAM_TLS:
         CMCHKUNPK(cmUnpkTlsTptParam, &unpkParam->u.tlsParam, mBuf);
         break;
#endif

      default:
         return RFAILED;
   }

   return ROK;
}   /* cmUnpkCmTptParam */


#ifdef CM_INET2  

/*
*
*       Fun:   cmPkCmIpHdrParm
*
*       Desc:  This function packs the the IP hedear parameters for both 
*              IPV4 and IPV6. 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmPkCmIpHdrParm
(
CmIpHdrParm *pkParam,   /* IP hdr parameters */ 
Buffer      *mBuf       /* message buffer */
)
{

   switch (pkParam->type)
   {
      case CM_HDRPARM_NOTPRSNT:
      case CM_HDRPARM_ICMP6:
         break;

      case CM_HDRPARM_IPV4:

         /* call to pack ipv4 options */
#ifdef IPV4_OPTS_SUPPORTED
         /* ipv4 IP options */
         CMCHKPK(cmPkTknStr64, &pkParam->u.hdrParmIpv4.ipv4HdrOpt, mBuf);
#endif /* IPV4_OPTS_SUPPORTED */
         
         CMCHKPK(cmPkTknUInt8, &pkParam->u.hdrParmIpv4.ttl, mBuf);
         CMCHKPK(cmPkTknUInt8, &pkParam->u.hdrParmIpv4.tos, mBuf);
         CMCHKPK(cmPkTknUInt8, &pkParam->u.hdrParmIpv4.dfBit, mBuf); 
         CMCHKPK(cmPkTknUInt8, &pkParam->u.hdrParmIpv4.proto, mBuf);
         break;

      case CM_HDRPARM_IPV6:
#ifdef IPV6_SUPPORTED    
         /* call to pack ipv6 extn hdrs */
#ifdef IPV6_OPTS_SUPPORTED 
         /* pack IPV6 extension headers */
         CMCHKPK(cmPkCmIpv6ExtHdr, &pkParam->u.hdrParmIpv6.ipv6ExtHdr, mBuf);
#endif /* IPV6_OPTS_SUPPORTED */
         CMCHKPK(cmPkCmNetAddr, &pkParam->u.hdrParmIpv6.srcAddr6, mBuf); 
         CMCHKPK(cmPkTknUInt8, &pkParam->u.hdrParmIpv6.ttl, mBuf);          
#endif /* IPV6_SUPPORTED */ 
         break;
   
      default:
         return RFAILED;
   }
   CMCHKPK(oduUnpackUInt8, pkParam->type, mBuf);

   return ROK;
}   /* End of cmPkCmIpHdrParm */


/* added new unpack function for local interface */
#ifdef LOCAL_INTF
/*
*
*       Fun:   cmUnpkCmTptLocalInf
*
*       Desc:  This function unpacks the local interface info on which 
*              IPV4/IPV6 packet was received on.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmUnpkCmTptLocalInf
(
CmTptLocalInf *unpkParam, /* local interface info */
Buffer        *mBuf       /* message buffer */
)
{

   /* first unpack the bool intfPrsnt value which is always packed */
   CMCHKUNPK(oduUnpackBool, &unpkParam->intfPrsnt, mBuf);
   
   /* if the above unpacked bool is TRUE then we have to unpack further
    * to get the local intf index and intf address */   
   if (unpkParam->intfPrsnt == TRUE)
   {   
      CMCHKUNPK(oduPackUInt32, &unpkParam->localIf, mBuf);
      CMCHKUNPK(cmUnpkCmNetAddr, &unpkParam->localIfAddr, mBuf); 
   }
   return ROK;
}   /* cmUnpkCmTptLocalInf */
#endif /* LOCAL_INTF */


/* added new functions */
#ifdef IPV6_OPTS_SUPPORTED 
/*
*       Fun:   cmPkCmIpv6ExtHdr
*
*       Desc:  This function packs the IPV6 extension headers
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmPkCmIpv6ExtHdr
(
CmIpv6ExtHdr *pkParam,   /* IPV6 extension hdrs */
Buffer       *mBuf       /* message buffer */
)
{

   /* pack first Route hdr */   
   if (pkParam->rtOptsPrsnt)
      CMCHKPK(cmPkCmIpv6RtHdr, &pkParam->rtOpts, mBuf);  
   CMCHKPK(oduPackBool, pkParam->rtOptsPrsnt, mBuf);  

   /* pack Dest Opt hdr */
   if (pkParam->destOptsPrsnt)
      CMCHKPK(cmPkCmIpv6DestOptsArr, &pkParam->destOptsArr, mBuf);  
   CMCHKPK(oduPackBool, pkParam->destOptsPrsnt, mBuf);  

   /* pack HBH hdr */
   if (pkParam->hbhHdrPrsnt)
      CMCHKPK(cmPkCmIpv6HBHHdrArr, &pkParam->hbhOptsArr, mBuf);  
   CMCHKPK(oduPackBool, pkParam->hbhHdrPrsnt, mBuf);  

   return ROK;
} /* end of cmPkCmIpv6ExtHdr */


/*
*
*       Fun:   cmPkCmIpv6RtHdr
*
*       Desc:  This function packs the IPV6 route header
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmPkCmIpv6RtHdr
(
CmIpv6RtHdr *pkParam,   /* IPV6 Route hdr */
Buffer      *mBuf      /* message buffer */
)
{
   uint8_t idx;
   

   /* pack all IPV6 addrs in the route hdr */
   for(idx = 0; idx < pkParam->numAddrs; idx++)
   {
      CMCHKPK(cmPkCmIpv6NetAddr, &pkParam->ipv6Addrs[idx], mBuf);
   }
   /* pack reserve byte & strict/loose bit map */
   CMCHKPK(oduUnpackUInt32, pkParam->slMap, mBuf);
   CMCHKPK(oduUnpackUInt8, pkParam->numAddrs, mBuf);  

   return ROK;
} /* end of cmPkCmIpv6RtHdr */


/*
*
*       Fun:   cmUnpkCmIpv6RtHdr
*
*       Desc:  This function unpacks the IPV6 route header
*
*       Ret:   ROK
*              ROUTRES - out of resources
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmUnpkCmIpv6RtHdr
(
CmIpv6RtHdr *unpkParam,  /* IPV6 Route hdr */
Buffer      *mBuf,       /* message buffer */
Mem         *memInfo     /* meminfo to allocate for Route hdr */
)
{
   uint8_t idx;  /* array index */
   S32 retVal; /* temporary return value */
   
   
   CMCHKUNPK(oduPackUInt8, &unpkParam->numAddrs, mBuf);
   
   /* unpack reserve byte & strict/loose bit map */
   CMCHKUNPK(oduPackUInt32, &unpkParam->slMap, mBuf);
   
   retVal = SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,memInfo->region,    
                  memInfo->pool,                     
                 (Data **)&unpkParam->ipv6Addrs, 
                 (unpkParam->numAddrs * 16));
      
   if (retVal != ROK)
   {
      return (ROUTRES);
   }
   
   /* unpack all IPV6 addrs in the route hdr */
   for(idx = 0; idx < unpkParam->numAddrs; idx++)
   {
      CMCHKUNPK(cmUnpkCmIpv6NetAddr, &unpkParam->ipv6Addrs[idx], mBuf);
   }

   return ROK;
} /* end of cmUnpkCmIpv6RtHdr */


/*
*
*       Fun:   cmUnpkCmIpv6ExtHdr
*
*       Desc:  This function unpacks the IPv6 extension header
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmUnpkCmIpv6ExtHdr
(
CmIpv6ExtHdr *unpkParam,  /* IPV6 extension hdrs */
Buffer       *mBuf,       /* message buffer */
Mem          *memInfo     /* meminfo to allocate for IPV6 ext hdr */
)
{
      
   /* unpack HBH hdr first */
   CMCHKUNPK(oduUnpackBool, &unpkParam->hbhHdrPrsnt, mBuf); 
   if (unpkParam->hbhHdrPrsnt)
      cmUnpkCmIpv6HBHHdrArr(&unpkParam->hbhOptsArr, mBuf, memInfo);         
                                                         
   /* unpack DEST opt hdr */
   CMCHKUNPK(oduUnpackBool, &unpkParam->destOptsPrsnt, mBuf);
   if (unpkParam->destOptsPrsnt)
      cmUnpkCmIpv6DestOptsArr(&unpkParam->destOptsArr, mBuf, memInfo);   

   /* unpack Route hdr last */
   CMCHKUNPK(oduUnpackBool, &unpkParam->rtOptsPrsnt, mBuf); 
   if (unpkParam->rtOptsPrsnt)
      cmUnpkCmIpv6RtHdr(&unpkParam->rtOpts, mBuf, memInfo);  
                                                        
   return ROK;
} /* end of cmUnpkCmIpv6ExtHdr */


/*
*
*       Fun:   cmPkCmIpv6DestOptsArr
*
*       Desc:  This function packs the IPV6 Destination Option array
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmPkCmIpv6DestOptsArr
(
CmIpv6DestOptsArr *pkParam,  /* IPV6 Dest hdr array */
Buffer            *mBuf      /* message buffer */
)
{

   uint8_t numOptions;

   
   /* pack all HBH options */
   for(numOptions = 0; numOptions<pkParam->numDestOpts; numOptions++)  
      CMCHKPK(cmPkCmIpv6DestOptsHdr, &pkParam->destOpts[numOptions], mBuf);
   CMCHKPK(oduUnpackUInt8, pkParam->numDestOpts, mBuf);  

   return ROK;
} /* end of cmPkCmIpv6DestOptsArr */


/*
*
*       Fun:   cmPkCmIpv6DestOptsHdr
*
*       Desc:  This function packs individua IPV6 Destination Option
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmPkCmIpv6DestOptsHdr
(
CmIpv6DestOptsHdr *pkParam,  /* IPV6 Dest opt */
Buffer            *mBuf      /* message buffer */
)
{
   uint8_t optLen;
   

   for(optLen = 0; optLen < pkParam->length; optLen++)
   {
      CMCHKPK(oduUnpackUInt8, pkParam->value[optLen], mBuf);
   }
   CMCHKPK(oduUnpackUInt8, pkParam->length, mBuf);  
   CMCHKPK(oduUnpackUInt8, pkParam->type, mBuf);  

   return ROK;
} /* end of cmPkCmIpv6DestOptsHdr */


/*
*
*       Fun:   cmUnpkCmIpv6DestOptsHdr
*
*       Desc:  This function unpacks individual IPV6 Dest Option
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmUnpkCmIpv6DestOptsHdr
(
CmIpv6DestOptsHdr *unpkParam,/* IPV6 Dest Option */
Buffer            *mBuf,     /* message buffer */
Mem               *memInfo   /* meminfo to allocate mem for dest opt */
)
{
   S32 retVal;                        /* temporary return value */
   uint8_t optLen;                      /* length of value field */

  
   /* unpack type, length */
   CMCHKUNPK(oduPackUInt8, &unpkParam->type, mBuf);  
   CMCHKUNPK(oduPackUInt8, &unpkParam->length, mBuf);

   /* allocate static memory to hold the unpacked values */
   retVal = SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,memInfo->region,                   
                  memInfo->pool,                     
                  (Data **)&unpkParam->value, (Size)unpkParam->length);
   if (retVal != ROK)
   {
      return (ROUTRES);
   }
      
   /* unpack value fieldof this option */
   for(optLen = 0; optLen<unpkParam->length; optLen++)
   {
      CMCHKUNPK(oduPackUInt8, &unpkParam->value[optLen], mBuf);  
   }    
   
   return ROK;
} /* end of cmUnpkCmIpv6DestOptsHdr */


/*
*
*       Fun:   cmUnpkCmIpv6DestOptsArr
*
*       Desc:  This function unpacks the IPV6 Destination Option array
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmUnpkCmIpv6DestOptsArr
(
CmIpv6DestOptsArr *unpkParam, /* all IPV6 Destination Options */
Buffer            *mBuf,      /* message buffer */
Mem               *memInfo    /* meminfo to allocate mem for dest opts */
)
{
   uint8_t numOptions;
   S16 retVal;
   

   CMCHKUNPK(oduPackUInt8, &unpkParam->numDestOpts, mBuf); 
   if (unpkParam->numDestOpts)
   {
      /* allocate mem to hold all dest options */
      retVal = SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,memInfo->region,    
                     memInfo->pool,                     
                     (Data **)&unpkParam->destOpts, 
                     (unpkParam->numDestOpts * sizeof(CmIpv6DestOptsHdr)));
      
      if (retVal != ROK)
      {
         return (ROUTRES);
      }
   
      /* unpack all dest options */
      for(numOptions = 0; numOptions<unpkParam->numDestOpts; numOptions++)
         if ((retVal = cmUnpkCmIpv6DestOptsHdr(&unpkParam->destOpts[numOptions],
                                            mBuf, memInfo)) != ROK)
            return RFAILED;   
   
   }                                             
   return ROK;
} /* end of cmUnpkCmIpv6DestOptsArr */


/*
*
*       Fun:   cmPkCmIpv6HBHHdrArr
*
*       Desc:  This function packs the all IPV6 HopByHop options
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmPkCmIpv6HBHHdrArr
(
CmIpv6HBHHdrArr *pkParam,  /* all IPV6 HopByHop options */
Buffer          *mBuf      /* message buffer */
)
{
   uint8_t numOptions;
   

   for(numOptions = 0; numOptions<pkParam->numHBHOpts; numOptions++)
      CMCHKPK(cmPkCmIpv6HBHHdr, &pkParam->hbhOpts[numOptions], mBuf);
                          
   CMCHKPK(oduUnpackUInt8, pkParam->numHBHOpts, mBuf);  

   return ROK;
} /* end of cmPkCmIpv6HBHHdrArr */


/*
*
*       Fun:   cmPkCmIpv6HBHHdr
*
*       Desc:  This function packs individual IPV6 HBH options
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmPkCmIpv6HBHHdr
(
CmIpv6HBHHdr *pkParam,  /* individual IPV6 HBH options */
Buffer       *mBuf      /* message buffer */
)
{
   uint8_t optLen;


   /* pack value field */
   for(optLen = 0; optLen<pkParam->length; optLen++)
   {
      CMCHKPK(oduUnpackUInt8, pkParam->value[optLen], mBuf);
   }

   /* pack type, length fields */
   CMCHKPK(oduUnpackUInt8, pkParam->length, mBuf);  
   CMCHKPK(oduUnpackUInt8, pkParam->type, mBuf);  

   return ROK;
} /* end of cmPkCmIpv6HBHHdr */


/*
*
*       Fun:   cmUnpkCmIpv6HBHHdr
*
*       Desc:  This function unpacks individual IPV6 HBH Option
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmUnpkCmIpv6HBHHdr
(
CmIpv6HBHHdr *unpkParam,/* individual IPV6 HBH Option */
Buffer       *mBuf,     /* message buffer */
Mem          *memInfo   /* meminfo to allocate mem for HBH opt */
)
{
   S32 retVal;                     /* temporary return value */
   uint8_t optLen;                      /* length of value field */
   
   CMCHKUNPK(oduPackUInt8, &unpkParam->type, mBuf);   
   CMCHKUNPK(oduPackUInt8, &unpkParam->length, mBuf);

   /* allocate static memory to hold the unpacked values */
   if (unpkParam->length)
   {
      retVal = SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,memInfo->region,                   
                     memInfo->pool,                     
                     (Data **)&unpkParam->value, (Size)unpkParam->length); 
      if (retVal != ROK)
      {
         return (ROUTRES);
      }

      for(optLen = 0; optLen<unpkParam->length; optLen++)
         CMCHKUNPK(oduPackUInt8, &unpkParam->value[optLen], mBuf);  
   } 
      
   return ROK;
} /* end of cmUnpkCmIpv6HBHHdr */


/*
*
*       Fun:   cmUnpkCmIpv6HBHHdrArr
*
*       Desc:  This function unpacks all IPV6 HopByHop options
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmUnpkCmIpv6HBHHdrArr
(
CmIpv6HBHHdrArr *unpkParam,  /* all HBH options */
Buffer          *mBuf,       /* message buffer */
Mem             *memInfo     /* meminfo to allocate space for HBH opt */
)
{
   uint8_t numOptions; 
   S16 retVal;


   CMCHKUNPK(oduPackUInt8, &unpkParam->numHBHOpts, mBuf);   
   if (unpkParam->numHBHOpts)
   {
      /*  allocate space for all HBH options */
      retVal = SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,memInfo->region,    
                     memInfo->pool,                     
                     (Data **)&unpkParam->hbhOpts, 
                     (unpkParam->numHBHOpts * sizeof(CmIpv6HBHHdr)));
      
      if (retVal != ROK)
      {
         return (ROUTRES);
      }

      for(numOptions = 0; numOptions<unpkParam->numHBHOpts; numOptions++)
         cmUnpkCmIpv6HBHHdr(&unpkParam->hbhOpts[numOptions], mBuf, memInfo);
   }                                              
   return ROK;
} /* end of cmUnpkCmIpv6HBHHdrArr */
#endif /* IPV6_OPTS_SUPPORTED */

  
/*
*
*       Fun:   cmUnpkCmIPHdrParm
*
*       Desc:  This function unpacks the IP header parameters
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
/* added */
#ifdef IPV6_OPTS_SUPPORTED
S16 cmUnpkCmIpHdrParm
(
CmIpHdrParm              *unpkParam,/* ip hdr parameters */
Buffer                   *mBuf,     /* message buffer */
Mem                      *memInfo   /* meminfo to allocate mem for ipHdrParam */
)
#else
S16 cmUnpkCmIpHdrParm
(
CmIpHdrParm              *unpkParam,/* ip hdr parameters */
Buffer                   *mBuf     /* message buffer */
)
#endif /* IPV6_OPTS_SUPPORTED */
{

   CMCHKUNPK(oduPackUInt8, &unpkParam->type, mBuf);

   switch (unpkParam->type)
   {
      case CM_HDRPARM_NOTPRSNT:
      case CM_HDRPARM_ICMP6:
         break;

      case CM_HDRPARM_IPV4:
         CMCHKUNPK(cmUnpkTknUInt8, &unpkParam->u.hdrParmIpv4.proto, mBuf);
         CMCHKUNPK(cmUnpkTknUInt8, &unpkParam->u.hdrParmIpv4.dfBit, mBuf);
         CMCHKUNPK(cmUnpkTknUInt8, &unpkParam->u.hdrParmIpv4.tos, mBuf);
         CMCHKUNPK(cmUnpkTknUInt8, &unpkParam->u.hdrParmIpv4.ttl, mBuf);
#ifdef IPV4_OPTS_SUPPORTED
         CMCHKUNPK(cmUnpkTknStr64, &unpkParam->u.hdrParmIpv4.ipv4HdrOpt, mBuf);
#endif /* IPV4_OPTS_SUPPORTED */
         break;

      case CM_HDRPARM_IPV6:
#ifdef IPV6_SUPPORTED         
         CMCHKUNPK(cmUnpkTknUInt8, &unpkParam->u.hdrParmIpv6.ttl, mBuf);
   
         CMCHKUNPK(cmUnpkCmNetAddr, 
                   &unpkParam->u.hdrParmIpv6.srcAddr6, mBuf);
 
#ifdef IPV6_OPTS_SUPPORTED
         /* memInfo is passed forward to alloc mem to hold unpacked 
          * IPV6 etx hdr data */
         cmUnpkCmIpv6ExtHdr(&unpkParam->u.hdrParmIpv6.ipv6ExtHdr, 
                            mBuf, memInfo);
#endif /* IPV6_OPTS_SUPPORTED */         
#endif /* IPV6_SUPPORTED */
         break;
   
      default:
         return RFAILED;
   }
   
   return ROK;
}   /* End of cmUnpkCmIpHdrParm */


/*
*
*       Fun:   cmPkCmIcmpFilter
*
*       Desc:  This function packs the ICMP filter parameters
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmPkCmIcmpFilter
(
CmIcmpFilter *pkParam,   
Buffer       *mBuf       /* message buffer */
)
{
   /* Variable declaration */
   S16   idx;
  

   switch (pkParam->type)
   {
      case CM_ICMP_NO_FILTER:
         break;

      case CM_ICMPVER4_FILTER:
         for(idx = (pkParam->u.icmpv4Filter.num-1); idx >= 0; idx--)
         {
            CMCHKPK(oduUnpackUInt32, pkParam->u.icmpv4Filter.icmpError[idx].errCodeMask,
                    mBuf);
            CMCHKPK(oduUnpackUInt8, pkParam->u.icmpv4Filter.icmpError[idx].errType,
                    mBuf);
         }
         CMCHKPK(oduUnpackUInt8, pkParam->u.icmpv4Filter.num, mBuf);
         CMCHKPK(oduUnpackUInt8, pkParam->u.icmpv4Filter.protocol, mBuf);
         CMCHKPK(oduUnpackUInt8, pkParam->u.icmpv4Filter.allMsg, mBuf);
         CMCHKPK(oduUnpackUInt8, pkParam->u.icmpv4Filter.icmpMsgFlag, mBuf);
         break;

      case CM_ICMPVER6_FILTER:
     /* The structure is yet to be decided */
     /* CMCHKPK(cmPkCmIpv6HdrParm, &pkParam->u.ipv6HdrParm, mBuf); */
#ifdef IPV6_SUPPORTED
         for(idx = (pkParam->u.icmpv6Filter.num - 1); idx >= 0; idx--)
         {
            CMCHKPK(oduUnpackUInt32, pkParam->u.icmpv6Filter.icmpError[idx].errCodeMask,
                    mBuf);
            CMCHKPK(oduUnpackUInt8, pkParam->u.icmpv6Filter.icmpError[idx].errType,
                    mBuf);
         }
         CMCHKPK(oduUnpackUInt8, pkParam->u.icmpv6Filter.num, mBuf);
         CMCHKPK(oduUnpackUInt8, pkParam->u.icmpv4Filter.allMsg, mBuf); 
         CMCHKPK(oduUnpackUInt8, pkParam->u.icmpv4Filter.icmpMsgFlag, mBuf);
#endif /* IPV6_SUPPORTED */
         break;
   
      default:
         return RFAILED;
   }
   CMCHKPK(oduUnpackUInt8, pkParam->type, mBuf);

   return ROK;
}

  
/*
*
*       Fun:   cmUnpkCmIcmpFilter
*
*       Desc:  This function unpacks the 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  cm_tpt.c
*
*/
S16 cmUnpkCmIcmpFilter
(
CmIcmpFilter *unpkParam,   
Buffer       *mBuf         /* message buffer */
)
{
   /* Variable declaration */ 
   S16     idx;

   CMCHKUNPK(oduPackUInt8, &unpkParam->type, mBuf);

   switch (unpkParam->type)
   {
      case CM_ICMP_NO_FILTER:
         break;

      case CM_ICMPVER4_FILTER:
         CMCHKUNPK(oduPackUInt8, &unpkParam->u.icmpv4Filter.icmpMsgFlag, mBuf);
         CMCHKUNPK(oduPackUInt8, &unpkParam->u.icmpv4Filter.allMsg, mBuf);
         CMCHKUNPK(oduPackUInt8, &unpkParam->u.icmpv4Filter.protocol, mBuf);
         CMCHKUNPK(oduPackUInt8, &unpkParam->u.icmpv4Filter.num, mBuf);

         for(idx = 0; idx < (unpkParam->u.icmpv4Filter.num); idx++)
         {
            CMCHKUNPK(oduPackUInt8, 
                      &unpkParam->u.icmpv4Filter.icmpError[idx].errType, mBuf);
            CMCHKUNPK(oduPackUInt32, 
                      &unpkParam->u.icmpv4Filter.icmpError[idx].errCodeMask, 
                      mBuf);
         }
         break;

      case CM_ICMPVER6_FILTER:
#ifdef IPV6_SUPPORTED
         CMCHKUNPK(oduPackUInt8, &unpkParam->u.icmpv6Filter.icmpMsgFlag, mBuf);
         CMCHKUNPK(oduPackUInt8, &unpkParam->u.icmpv6Filter.allMsg, mBuf); 
         CMCHKUNPK(oduPackUInt8, &unpkParam->u.icmpv6Filter.num, mBuf);

         for(idx = 0; idx < (unpkParam->u.icmpv6Filter.num); idx++)
         {
            CMCHKUNPK(oduPackUInt8, 
                      &unpkParam->u.icmpv6Filter.icmpError[idx].errType, mBuf);
            CMCHKUNPK(oduPackUInt32, 
                      &unpkParam->u.icmpv6Filter.icmpError[idx].errCodeMask, 
                      mBuf);
         }
#endif /* IPV6_SUPPORTED */
         break;
   
      default:
         return RFAILED;

   }
   
   return ROK;
}   /* End of cmUnpkCmIcmpFilter */

#endif  /* CM_INET2 */ 

  
#endif /* if(LCHCT || LCHIT || LCLHC || LCHRT || LCLHR) */

  
/********************************************************************30**
         End of file
*********************************************************************31*/
