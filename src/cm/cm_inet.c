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
    
        Type:     C Source file 
  
        Desc:     common library for Internet sockets,
                  always link this library with following libraries:
                       Trillium's libos.a
                       Solaris    nsl, socket 
 
        File:     cm_inet.c
  
*********************************************************************21*/
 

/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *      1000151     TCAP over TCP/IP   
 */

  
/* header include files (.h) */
  
#include "envopt.h"             /* environment options */  
#include "envdep.h"             /* environment dependent */
#include "envind.h"             /* environment independent */

#include "gen.h"                /* general */
#include "ssi.h"                /* system services interface */
#include "cm_inet.h"            /* socket library file */

/*cm_inet_c_001.main_35 : Updated for C++ compilation */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* environment dependent include files */
#ifdef WIN32
#ifndef IN
#define IN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#ifdef WIN2K
#include <Mswsock.h>
#endif /* WIN2K */
#else /* WIN32 */
#include <errno.h>
#if (!defined(SS_VW) && !defined(SS_PS))
#include <netdb.h>
#endif
#ifndef SS_PS
#include <unistd.h>
#endif
#include <string.h>
#include <sys/types.h>
#ifdef SS_PS
#include <pna.h>
#else
#include <sys/socket.h>
#include <sys/ioctl.h>
#endif /* SS_PS */
#ifdef SS_VW
#include <sys/times.h>
#include <ioLib.h>
#include <sockLib.h>
#include <selectLib.h>
#include <hostLib.h>
#else
#if (!defined (SS_PS) && !defined(HPOS))
#include <sys/select.h>
#include <sys/time.h>
#ifdef SS_LINUX
#include <sys/uio.h>
#else
#include <sys/filio.h>
#endif /* SS_LINUX */
#endif /* SS_PS && HPOS */
#ifdef HPOS
#include <sys/time.h>
#endif /* HPOS */
#endif /* SS_VW */
#ifndef SS_PS
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#ifdef IPV6_SUPPORTED 

/* cm_inet_c_001.main_54: Fix for vxworks 6.7 adding the new header includes */
#if (defined(SS_VW) && defined(SS_VW6_7)) 
#include <ipcom_inet.h>
#include <ipcom_sock6.h>
#include <netinet/icmp6.h>
#else 
#include <netinet/icmp6.h>
#endif    

#endif /* IPV6_SUPPORTED */
#endif /* SS_PS */
#endif /* WIN32 */

/*cm_inet_c_001.main_38 Updated for TUCL 2.1 Release (Kernel SCTP Support) */
#ifdef CM_LKSCTP
#include <netinet/sctp.h>
#endif

   /* cm_inet_c_001.main_58: Added new header files to support filteration 
    * of ICMP messages */
#ifdef SS_LINUX
#ifdef CM_ICMP_FILTER_SUPPORT
#include <asm/types.h>
#include <linux/icmp.h>
#endif
#endif
 /* cm_inet_c_001.main_62:Warning fix */
#if (!defined(SS_VW) && !defined(SS_PS) && !defined(WIN32))
#include <netdb.h>
#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* header/extern include files (.x) */

#include "gen.x"                /* general */
#include "ssi.x"                /* system services interface */
#include "cm_inet.x"            /* socket library file */
#include "cm_lib.x"             /* lib library file */
#ifdef NTL_LIB
#include "ntl_lib.h"
#endif 
#include "du_log.h"

  
/* local defines */

/* BSD and Winsock error handling is different */
#ifdef WIN32
#define INET_ERR             SOCKET_ERROR
#define INET_ERR_CODE        WSAGetLastError()
#define ERR_INPROGRESS       WSAEINPROGRESS  
#define ERR_ISCONN           WSAEISCONN
#define ERR_WOULDBLOCK       WSAEWOULDBLOCK
#define ERR_INADDRNONE       INADDR_NONE
#define ERR_NOTCONN          WSAENOTCONN
#define ERR_ALREADY          WSAEALREADY
#define ERR_AGAIN            WSAEWOULDBLOCK 
#define ERR_INVAL            WSAEINVAL
#define ERR_CONNREFUSED       WSAECONNREFUSED
#define ERR_PIPE             WSAENOTCONN
/* Changed ERR_TIMEOUT for pSos compilation */
#define ERR_TIMEDOUT         WSAETIMEDOUT
#define ERR_CONNRESET        WSAECONNRESET
#define ERR_CONNABORTED      WSAECONNABORTED
/* cm_inet_c_001.main_36 Do select again in case of interrupt */
#define ERR_EINTR            WSAEINTR
/* cm_inet_c_001.main_37 network unreacheble error is added */
#define ERR_NETUNREACH       WSAENETUNREACH
/* cm_inet_c_001.main_61: host unreachable is added */
#define ERR_HOSTUNREACH      WSAEHOSTUNREACH
#else
#define INET_ERR             -1
#define INET_ERR_CODE        errno
#define ERR_INPROGRESS       EINPROGRESS
#define ERR_ISCONN           EISCONN
#define ERR_WOULDBLOCK       EWOULDBLOCK
#define ERR_INADDRNONE       -1
#define ERR_NOTCONN          ENOTCONN       
#define ERR_ALREADY          EALREADY
#define ERR_AGAIN            EAGAIN
/* cm_inet_c_001.main_36 Do select again in case of interrupt */
#define ERR_EINTR            EINTR
/* EINVAL is not mapped because it is a valid error code here */
#define ERR_INVAL            0 
#define ERR_CONNREFUSED      ECONNREFUSED
#define ERR_PIPE             EPIPE 
/* Changed ERR_TIMEOUT for pSos compilation */
#define ERR_TIMEDOUT         ETIMEDOUT
#define ERR_CONNRESET        ECONNRESET
#define ERR_CONNABORTED      ECONNABORTED
/* cm_inet_c_001.main_37 network unreacheble error is added */
#define ERR_NETUNREACH       ENETUNREACH
/* cm_inet_c_001.main_61: host unreachable is added */
#define ERR_HOSTUNREACH      EHOSTUNREACH
#endif /* WIN32 */

/* back log range */
#define MIN_BACK_LOG  0

/* added a win2k specific defines in. */
#ifdef WIN32
#ifdef WIN2K
#ifndef SIO_UDP_CONNRESET
#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR, 12)
#endif 
#endif /* WIN2K */
#define MAX_BACK_LOG  1
#else
#define MAX_BACK_LOG  5
#endif /* WIN32 */

#ifdef IPV6_OPTS_SUPPORTED
#ifndef IPV6_SUPPORTED
#error "Enable IPV6_SUPPORTED flag if IPV6_OPTS_SUPPORTED is defined."
#endif 
#if (!defined(SS_LINUX) && !defined(_XPG4_2))
#error "Enable _XPG4_2 or SS_LINUX if IPV6_OPTS_SUPPORTED is defined."
#endif /* SS_LINUX || _XPG4_2 */
#endif /* IPV6_OPTS_SUPPORTED */

#ifdef LOCAL_INTF
#if (!defined(SS_LINUX) && !defined(_XPG4_2))
#error "Enable _XPG4_2 or SS_LINUX if LOCAL_INTF is defined."
#endif /* SS_LINUX || _XPG4_2 */
#endif /* LOCAL_INTF */

/* local typedefs */

/* local externs */
  
/* forward references */

/* added !(defined(CMINETFLATBUF) */
#if (!(defined(WIN32)) && !(defined(CMINETFLATBUF)))
/* Added another function parameter */
static S16 buildRecvBuf ARGS((CmInetMemInfo *info, MsgLen len, 
                               CmInetIovec rxArr[], Buffer *dBuf[], uint16_t maxSize,
                               struct msghdr *msg, Bool isStrmMsg));
static S16 buildRecvMsg ARGS((CmInetMemInfo *info, CmInetIovec rxArr[], 
                               S16 numBduf, MsgLen msgLen, Buffer *dBufs[], 
                               Buffer **mPtr));
/* cm_inet_c_001.main_50 - Added parameter to get length of dbufs packed for partial
 *               send handling 
 */
static S16 buildSendIovec ARGS((Buffer *mBuf, MsgLen msgLen, 
                                 CmInetIovec txArr[], S16 numDBuf, 
                                 S16 *numIovElems, uint32_t *strtEndDBufNum,
                                 MsgLen *ioLen)); 
#endif /* (defined(WIN32)) && !(defined(CMINETFLATBUF)) */

/* prototypes of new functions needed to send and 
 * process after receiving the extension headers through ancillary data */

#ifdef IPV6_SUPPORTED
#ifdef IPV6_OPTS_SUPPORTED
static S16 cmInet6BuildSendHBHOpts    ARGS((CmInetIpv6HBHHdrArr *hbhOptsArr, 
                                            uint8_t *cmsgBuf, uint32_t *curMsgIdx, 
                                            uint8_t hdrId));
static S16 cmInet6BuildSendRouteOpts  ARGS((CmInetIpv6RtHdr *rtOptsArr, 
                                            uint8_t *cmsgBuf, uint32_t *curMsgIdx));

static S16 cmInet6BuildRecvRtHdr      ARGS((uint8_t *cmsgData, uint32_t rtDataLen, 
                                            CmInetIpv6RtHdr0 *rtHdr0, 
                                            CmInetIpv6RtHdr *rtOptsArr,
                                            CmInetMemInfo *info));
static S16 cmInet6BuildRecvHopOptsArr ARGS((uint8_t *cmsgData, uint32_t hbhDataLen, 
                                            CmInetIpv6HBHHdrArr *hbhOptsArr, 
                                            uint8_t hdrId, CmInetMemInfo *info));
static S16 cmInet6GetHopLimitValue    ARGS((uint8_t *cmsgData, uint32_t hopLimitDataLen,
                                            CmInetIpv6HdrParm *ipv6HdrParam));

#ifdef SS_LINUX
static S16 cmInetBuildSendHoplimit        ARGS((uint32_t hoplimit, uint8_t *cmsgBuf, 
                                            uint32_t *curMsgIdx));
#endif /* SS_LINUX */
#ifdef LOCAL_INTF
static S16 cmInet6BuildSendPktinfo         ARGS((CmInetIpAddr6 *srcAddr,
                                            uint8_t *cmsgBuf, uint32_t *curMsgIdx,
                                            uint8_t protType));
#endif /* LOCAL_INTF */
#endif /* IPV6_OPTS_SUPPORTED */
#endif /* IPV6_SUPPORTED */

/* public variable declarations */

/* private variable declarations */

#ifdef CMINETDBG
/* Global buffer for debug prints */
/*cm_inet_c_001.main_62:Warning fix*/
Txt   prntBuf[CMINET_PRNT_BUF_SIZE];
#endif /* CMINETDBG */

/* cm_inet_c_001.main_60 POLL Specific Functions defined */

/*
*
*      Fun:   cmInetPoll
*
*      Desc:  Poll on pollfdarr
*
*      Ret:   Number of File Descriptor Selected
*
*      Notes: None
*
*      File:  cm_inet.c
*
*/

S16 cmInetPoll
(
CmInetPollFd  *pollFdArr,               /* poll FD Array */
uint32_t               numFds,               /* Number of Fds to be monitored */
S16              *numRdyFds,            /* number of ready descriptors */
uint32_t              timeout                /* timeout value for Poll */
)
{
   S32  ret;

   ret = 0;
   *numRdyFds = 0;

   if(numFds > CM_INET_POLL_MAXFDSUPP)
   {
#ifdef CMINETDBG
/* cm_inet_c_001.main_62:Warning fix */
#ifndef ALIGN_64BIT
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPoll() : number of file descriptor (%lu) invalid \n",numFds);
#else
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPoll() : number of file descriptor (%u) invalid \n",numFds);
#endif
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#endif /* CMINETDBG */      
      return RFAILED;
   } 

#if (ERRCLASS & ERRCLS_INT_PAR)
      /* error check on parameters */
      if (pollFdArr == NULLP)
      {
#ifdef CMINETDBG
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPoll() : Invalid Parameter (pollFdArr is NULL)");
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#endif /* CMINETDBG */      
         return RFAILED;
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   ret = poll(pollFdArr,numFds,timeout);
   if (ret == 0)
   { 
      return (RTIMEOUT);
   }
   if (ret == INET_ERR)
   {
      switch(INET_ERR_CODE)
      {
         case ERR_EINTR:
            return (ROKDNA);

         default:
#ifdef CMINETDBG
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "File: %s, cmInetPoll() failed on line: %d \n\
                    error(%d)\n", __FILE__, __LINE__, INET_ERR_CODE);
            SPrint(prntBuf);
#endif /* CMINETDBG */
            return RFAILED;

      } /* end of switch */
   }
   
   *numRdyFds = (S16)ret;
  
   return ROK;

}

/*
*
*      Fun:   cmInetPollSetFd
*
*      Desc:  Set the selected fd in pollFdArr with event eventMask
*
*      Ret:   RFAILED : if file descriptor is out of range
*             ROK     : if pollFdArr is set.
*
*      Notes: None
*
*      File:  cm_inet.c
*
*/

S16 cmInetPollSetFd
(
CmInetFd         *sockFd,               /* socket file descriptor */
CmInetPollFd  *pollFdArr,               /* poll FD Array */
S16               idx,                  /* poll Fd Array Index */
uint16_t               eventMask             /* Event Mask to be set */
)
{
   

   if ((idx) >= CM_INET_POLL_MAXFDSUPP || (idx) < 0)
   {
#ifdef CMINETDBG
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollSetFd() : Invalid idx(%d) \n",idx);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#endif /* CMINETDBG */

      return RFAILED;
   }

#if (ERRCLASS & ERRCLS_INT_PAR)
      /* error check on parameters */
      if (pollFdArr == NULLP)
      {
#ifdef CMINETDBG
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollSetFd() : Invalid Parameter (pollFdArr is NULL)");
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#endif /* CMINETDBG */      
         return RFAILED;
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, 
               "cmInetPollSetFd() Before Setting fd : sockFd->fd(%d) Index(%d) Event(%d) \n Fd and event to be set   fd(%ld) event(%d) \n",
               pollFdArr[idx].fd,idx, pollFdArr[idx].events,sockFd->fd,eventMask);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf); 
#else
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,
               "cmInetPollSetFd() Before Setting fd: sockFd->fd(%d) Index(%d) Event(%d) \n Fd and event to be set   fd(%d) event(%d) \n",
               pollFdArr[idx].fd,idx, pollFdArr[idx].events,sockFd->fd,eventMask);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#endif /*ALIGN_64BIT */
#endif /* CMINETDBG */

/* Setting fd and events with eventMask */
   pollFdArr[idx].fd = sockFd->fd;
   pollFdArr[idx].events |= eventMask;


#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollSetFd() After Setting fd: sockFd->fd(%d) Index(%d) Event(%d) \n",
            pollFdArr[idx].fd,idx, pollFdArr[idx].events);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollSetFd() After Setting fd: sockFd->fd(%d) Index(%d) Event(%d) \n",
            pollFdArr[idx].fd,idx, pollFdArr[idx].events);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#endif /*ALIGN_64BIT */
#endif /* CMINETDBG */

   return ROK;

}


/*
*
*      Fun:   cmInetPollFdIsSet
*
*      Desc:  Checks whether fd is selected
*
*      Ret:   TRUE : If Fd is Selected
*             FALSE: If Fd is not Selected
*
*      Notes: None
*
*      File:  cm_inet.c
*
*/

S16 cmInetPollFdIsSet
(
CmInetPollFd  *pollFdArr,            /* poll FD Array */
S16               idx,               /* poll Fd Array Index */
uint16_t               eventMask          /* Event Mask to be set */
)
{
   S16  ret;


   if((idx < 0) || (idx > CM_INET_POLL_MAXFDSUPP))
   {
#ifdef CMINETDBG
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollFdIsSet() : Invalid Index (%d) \n",idx);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#endif /* CMINETDBG */
      return RFAILED;
   }

#if (ERRCLASS & ERRCLS_INT_PAR)
      /* error check on parameters */
      if (pollFdArr == NULLP)
      {
#ifdef CMINETDBG
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollFdIsSet() : Invalid Parameter (pollFdArr is NULL)");
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#endif /* CMINETDBG */      
         return RFAILED;
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   ret = (pollFdArr[idx].revents & eventMask);

   return (ret);
}

/*
*
*      Fun:   cmInetPollClearFdREvent
*
*      Desc:  clears the reventMask in revent of the givent FD.
*
*      Ret:   ROK
*             
*
*      Notes: None
*
*      File:  cm_inet.c
*
*/

S16 cmInetPollClearFdREvent
(
CmInetPollFd  *pollFdArr,            /* poll FD Array */
S16               idx,               /* poll Fd Array Index */
uint16_t               eventMask          /* Event Mask to be set */
)
{



   if((idx < 0) || (idx > CM_INET_POLL_MAXFDSUPP))
   {
#ifdef CMINETDBG
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollClearFdREvent() : Invalid Index (%d) \n",idx);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#endif /* CMINETDBG */
      return RFAILED;
   }

#if (ERRCLASS & ERRCLS_INT_PAR)
      /* error check on parameters */
      if (pollFdArr == NULLP)
      {
#ifdef CMINETDBG
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollClearFdREvent() : Invalid Parameter (pollFdArr is NULL)");
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#endif /* CMINETDBG */      
         return RFAILED;
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollClearFdREvent() Before clearing fd revents with eventMask: \n sockFd->fd(%d) Index(%d) REvent(%d) EventMask(%d) \n",
            pollFdArr[idx].fd,idx, pollFdArr[idx].revents,eventMask);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollClearFdREvent() Before clearing fd revents with eventMask: \n sockFd->fd(%d) Index(%d) REvent(%d) EventMask(%d) \n",
            pollFdArr[idx].fd,idx, pollFdArr[idx].revents,eventMask);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#endif /*ALIGN_64BIT */
#endif /* CMINETDBG */

/* Clearing the events with eventMask */
   pollFdArr[idx].revents = (pollFdArr[idx].revents & (~(eventMask)));

#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollClearFdREvent() After clearing fd revents with eventMask: \n sockFd->fd(%d) Index(%d) REvent(%d) EventMask(%d) \n",
            pollFdArr[idx].fd,idx, pollFdArr[idx].revents,eventMask);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollClearFdREvent() After clearing fd revents with eventMask: \n sockFd->fd(%d) Index(%d) REvent(%d) EventMask(%d) \n",
            pollFdArr[idx].fd,idx, pollFdArr[idx].revents,eventMask);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#endif /*ALIGN_64BIT */
#endif /* CMINETDBG */

   return ROK;

}


/*
*
*      Fun:   cmInetPollClearFdEvent
*
*      Desc:  clears the eventMask in event of the givent FD.
*
*      Ret:   ROK
*             
*
*      Notes: None
*
*      File:  cm_inet.c
*
*/

S16 cmInetPollClearFdEvent
(
CmInetPollFd  *pollFdArr,            /* poll FD Array */
S16               idx,               /* poll Fd Array Index */
uint16_t               eventMask          /* Event Mask to be set */
)
{



   if((idx < 0) || (idx > CM_INET_POLL_MAXFDSUPP))
   {
#ifdef CMINETDBG
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollClearFdEvent() : Invalid Index (%d) \n",idx);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#endif /* CMINETDBG */
      return RFAILED;
   }

#if (ERRCLASS & ERRCLS_INT_PAR)
      /* error check on parameters */
      if (pollFdArr == NULLP)
      {
#ifdef CMINETDBG
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollClearFdEvent() : Invalid Parameter (pollFdArr is NULL)");
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#endif /* CMINETDBG */      
         return RFAILED;
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollClearFdEvent() Before clearing fd events with eventMask: \n sockFd->fd(%d) Index(%d) Event(%d) EventMask(%d) \n",
            pollFdArr[idx].fd,idx, pollFdArr[idx].events,eventMask);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollClearFdEvent() Before clearing fd events with eventMask:\n  sockFd->fd(%d) Index(%d) Event(%d) EventMask(%d) \n",
            pollFdArr[idx].fd,idx, pollFdArr[idx].events,eventMask);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#endif /*ALIGN_64BIT */
#endif /* CMINETDBG */

/* Clearing events with eventMask */
   pollFdArr[idx].events = (pollFdArr[idx].events & (~(eventMask)));

#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollClearFdEvent() After clearing fd events with eventMask: sockFd->fd(%d) Index(%d) Event(%d) EventMask(%d) \n",
            pollFdArr[idx].fd,idx, pollFdArr[idx].events,eventMask);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollClearFdEvent() After clearing fd events with eventMask: sockFd->fd(%d) Index(%d) Event(%d) EventMask(%d) \n",
            pollFdArr[idx].fd,idx, pollFdArr[idx].events,eventMask);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#endif /*ALIGN_64BIT */
#endif /* CMINETDBG */

   return ROK;

}


/*
*
*      Fun:   cmInetPollDelFd
*
*      Desc:  Delete the given FD from the pollFdArray
*             delIdx : Poll Fd Array Index at which fd has to be deleted.
*             lastIdx: Last index of poll fd array.
*
*             It deletes fd from array by swapping lastIdx pollFd
*             values to index to be deleted and deinitializes the 
*             lastIdx values.
*
*      Ret:   ROK
*             
*
*      Notes: It does not decrement the lastIdx and it has to be 
*             decremented by the caller of this function.
*
*      File:  cm_inet.c
*
*/

S16 cmInetPollDelFd
(
CmInetPollFd  *pollFdArr,             /* poll FD Array */
S16               delIdx,             /* poll Fd Array Index for which fd has to be deleted*/
S16               lastIdx             /* Last index of poll Fd Array */
)
{


   if(lastIdx < delIdx || lastIdx < 0 || delIdx < 0)
   {
#ifdef CMINETDBG
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollDelFd() : Invalid Index \n Current Index (%d) Delete Index (%d) \n",lastIdx,delIdx);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#endif /* CMINETDBG */
      return RFAILED;
   }


#if (ERRCLASS & ERRCLS_INT_PAR)
      /* error check on parameters */
      if (pollFdArr == NULLP)
      {
#ifdef CMINETDBG
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollDelFd() : Invalid Parameter (pollFdArr is NULL)");
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#endif /* CMINETDBG */      
         return RFAILED;
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollDelFd() Deleting the sockFd->fd(%d) Index(%d) Event(%d) revent(%d) \n",
            pollFdArr[delIdx].fd,delIdx, pollFdArr[delIdx].events,pollFdArr[delIdx].revents);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollDelFd() After clearing fd events with eventMask: sockFd->fd(%d) Index(%d) Event(%d) EventMask(%d) \n",
            pollFdArr[delIdx].fd,delIdx, pollFdArr[delIdx].events,pollFdArr[delIdx].revents);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#endif /*ALIGN_64BIT */
#endif /* CMINETDBG */

   pollFdArr[delIdx].fd = pollFdArr[lastIdx].fd;
   pollFdArr[delIdx].events = pollFdArr[lastIdx].events;
   pollFdArr[delIdx].revents = pollFdArr[lastIdx].revents;

   pollFdArr[lastIdx].fd = -1;
   pollFdArr[lastIdx].events = 0;
   pollFdArr[lastIdx].revents = 0;
   
   return ROK;

}

/*
*
*      Fun:   cmInetPollInitFdArr
*
*      Desc: Cleans all elements of fd array. 
*
*      Ret:   ROK
*             
*
*      Notes:  It does not allocates/deallocates memory for Poll Fd Array.
*              Caller of function has to allocate/deallocate memory for 
*              array.
*
*      File:  cm_inet.c
*
*/

S16 cmInetPollInitFdArr
(
CmInetPollFd  *pollFdArr             /* poll FD Array */
)
{
   S16 idx;

/* Sets each element of pollFdArr to initial value
   fd = -1
   events = 0
   revents = 0
*/
#if (ERRCLASS & ERRCLS_INT_PAR)
      /* error check on parameters */
      if (pollFdArr == NULLP)
      {
#ifdef CMINETDBG
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetPollInitFdArr() : Invalid Parameter (pollFdArr is NULL)");
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINETXXX, 0, prntBuf);
#endif /* CMINETDBG */      
         return RFAILED;
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   for(idx=0; idx < CM_INET_POLL_MAXFDSUPP; idx++)
   {
      pollFdArr[idx].fd = -1;
      pollFdArr[idx].events = 0;
      pollFdArr[idx].revents = 0;
   }
   return ROK;
}

#if (!(defined(WIN32)) && !(defined(CMINETFLATBUF)))
/*
*
*      Fun:   buildRecvBuf
*
*      Desc:  Allocates dBufs to receive an entire message.
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*             ROUTRES - failed, out of resources
*
*      Notes: None
*
*      File:  cm_inet.c
*
*/

static S16 buildRecvBuf
(
CmInetMemInfo  *info,           /* buffer allocation info */
MsgLen          len,            /* message length */
CmInetIovec     rxArr[],        /* gather array */
Buffer         *dBuf[],         /* allocated dBufs */
uint16_t       maxSize,        /* size of rxArr/dBuf array */       
struct msghdr  *msg,            /* message header for recvmsg() */           
Bool           isStrmMsg        /* Is a TCP message */
)
{
   S16     ret;                 /* temporary return value */
   uint16_t numBuf;              /* number of dBufs */
   uint16_t i;                   /* dBuf index counter */
   Data   *dPtr;                /* data pointer */
   /* cm_inet_c_001.main_47: 102069 Changed from S32 to MsgLen for bufLen*/
   MsgLen  bufLen;              /* entire receive buffer length, if S16 
                                   could wrap to negative number */
   MsgLen  dLen;                /* buffer length */ 

   numBuf = 0;
   bufLen = 0; 

   /* Initialise ret and part of msg here */
   ret = ROK;

   /* added defined(_XPG4_2) */
   /* Moved initialisation of msg here. */

#if (defined(SS_LINUX) || defined(_XPG4_2))
   msg->msg_control    = NULLP;
   msg->msg_controllen  = 0;
#else
   msg->msg_accrights     = NULLP;
   msg->msg_accrightslen  = 0;
#endif /* SS_LINUX */   

   /* Check if maxSize if enough to hold the entire message length before 
    * going into the loop. If the boolean isStrmMsg is TRUE then the recv 
    * buf is built even if the whole message cannot be accomodated. */

#ifdef T2K_MEM_LEAK_DBG
   char * file = __FILE__;
   uint32_t  line   = __LINE__;
   ret = SGetDBuf(info->region, info->pool, &dBuf[numBuf]); 
#else
   ret = SGetDBuf(info->region, info->pool, &dBuf[numBuf]); 
#endif
   if (ret != ROK)
      return (ret);

   /* Get the data part */
   ret = SGetDataRx(dBuf[numBuf], 0, &dPtr, &dLen);
   if (ret != ROK)
   {
      numBuf++;           /* because of cleanup */
      goto cleanup;
   }

   if (!isStrmMsg)
   {
      /* The assumption here is that all dBuf's from a given region and 
       * pool have a constance size */
      if (len > (maxSize * dLen))
      {
         ret = RNA;
         numBuf++;           /* because of cleanup */
         goto cleanup;
      }
   }

#ifdef SS_LINUX
  rxArr[numBuf].iov_base = (Void*)dPtr;  
  rxArr[numBuf].iov_len = (uint32_t)dLen;    
#else
  rxArr[numBuf].iov_base = (S8*)dPtr;
  rxArr[numBuf].iov_len = dLen;
#endif /* SS_LINUX */

  bufLen += dLen;
  numBuf++;  

   /* allocate buffer space for entire message length */
   while (bufLen < len)
   {
      if (numBuf >= maxSize)
      {
         /* to big to fit in gather vector array */ 
         ret = RNA;
         break;
      }
      ret = SGetDBuf(info->region, info->pool, &dBuf[numBuf]); 
      if (ret != ROK)
      {
         goto cleanup;
      } 
      ret = SGetDataRx(dBuf[numBuf], 0, &dPtr, &dLen);
      if (ret != ROK)
      {
         numBuf++;           /* because of cleanup */
         goto cleanup;
      }
#ifdef SS_LINUX
      rxArr[numBuf].iov_base = (Void*)dPtr;  
      rxArr[numBuf].iov_len = (uint32_t)dLen;    
#else
      rxArr[numBuf].iov_base = (S8*)dPtr;
      rxArr[numBuf].iov_len = dLen;
#endif /* SS_LINUX */

      bufLen += dLen;
      numBuf++;  
   }
   /* adjust last buffer length */
   /*  check if we broke out because numBuf >= maxSize */
   if (bufLen < len)
      rxArr[numBuf - 1].iov_len = dLen;
   else
      rxArr[numBuf - 1].iov_len = dLen - (bufLen - len); 

   /* setup recvmsg() message header */
   msg->msg_iov           = rxArr;
   msg->msg_iovlen        = numBuf;

   return (ret);

cleanup:
   /* cleanup */
   for (i = 0; i < numBuf; i++)
      SPutDBuf(info->region, info->pool, dBuf[i]);

   msg->msg_iovlen = 0;

   return (ret);
} /* end of buildRecvBuf */

/*
*
*      Fun:   buildRecvMsg
*
*      Desc:  Builds a message out of the received dBufs.
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*             ROUTRES - failed, out of resources
*
*      Notes: None
*
*      File:  cm_inet.c
*
*/

static S16 buildRecvMsg
(
CmInetMemInfo  *info,           /* buffer allocation info */
CmInetIovec     rxArr[],        /* scatter array */  
S16             numBuf,         /* number of allocated dBufs */
MsgLen          msgLen,         /* message length */
Buffer         *dBufs[],        /* dBufs */
Buffer        **mPtr            /* message built from dBufs */     
)
{
   S16      ret;                 /* return value */ 
   S16      i;                   /* dBuf index counter */
   MsgLen   bufLen;              /* length of one particular dBuf */
   /* cm_inet_c_001.main_54: Fix for Klockworks issue */
   Buffer  *mBuf = NULLP;        /* allocated message */  

   ret = RFAILED;   
   i   = 0;

   /* build message */
   ret = SGetMsg(info->region, info->pool, &mBuf);
   if (ret != ROK)
   {
      goto cleanup;
   }

   /* link buffers to message */
   while (i < numBuf)
   {
      /* cm_inet_c_001.main_58: fix for klockwork issue */
      bufLen = (MsgLen)rxArr[i].iov_len; 
      if (msgLen < bufLen)
      {
         bufLen = msgLen;
      }
      ret = SUpdMsg(mBuf, dBufs[i], bufLen);
      if (ret != ROK)
      { 
         SPutMsg(mBuf);
         goto cleanup;
      }
      msgLen -= bufLen;
      i++;
      if (msgLen <= 0)
      {
         ret = ROK;      
         break;
      }
   }

   *mPtr = mBuf;

cleanup:
   /* cleanup unused buffers */
   while (i < numBuf)
   {
#ifdef T2K_MEM_LEAK_DBG
   char * file = __FILE__;
   uint32_t  line   = __LINE__;
   SPutDBuf(info->region, info->pool, dBufs[i]);
#else
   SPutDBuf(info->region, info->pool, dBufs[i]);
#endif
    i++;
   }

   return (ret);
} /* end of buildRecvMsg */



/*
*
*      Fun:   buildSendIovec 
*
*      Desc:  Builds a io vector to send a message.
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*             ROUTRES - failed, out of resources
*             RNA     - failed, not available, indicates that the
*                       maximum number of dBufs are not sufficient
*                       to hold the entire message.
*
*      Notes: None
*
*      File:  cm_inet.c
*
*/
static S16 buildSendIovec
(
Buffer         *mBuf,           /* Message buffer */
MsgLen         msgLen,          /* Length of mBuf */
CmInetIovec    txArr[],         /* transmit scatter vector array */ 
S16            numDBufs,        /* Maximum number of dBufs to use */
S16            *numIovElems,    /* Number of iov elements in array */
uint32_t       *strtEndDBufNum, /* dBuf number to start and end */
MsgLen         *ioLen           /* cm_inet_c_001.main_50 - Len of dbuf packed into IO-vector */ 
)
{
   S16         ret;
   MsgLen      dLen;
   S16         iovIdx;       
   Buffer      *dBuf;
   Data        *dPtr;
   MsgLen      allocLen;
   uint32_t         dBufsToSkip;

   /* Initialisations */
   (*numIovElems) = 0;
   iovIdx = 0;
   allocLen = 0;

   /* cm_inet_c_001.main_50 - Intialize the newly added parameter */
   *ioLen = 0;

   /* Set up vector for gathering send */
   ret = SInitNxtDBuf(mBuf);
   if (ret != ROK)
   {
      return RFAILED;
   }

   iovIdx = 0;
   txArr[iovIdx].iov_len = 0;

   if ((*strtEndDBufNum != 0))
   {
      /* Skip through the required number of dBufs */
      dBufsToSkip = *strtEndDBufNum;

      while(dBufsToSkip)
      {
         ret = SGetNxtDBuf(mBuf, &dBuf);
         if (ret != ROK)
            return RFAILED;
         dBufsToSkip --;
      }
   }

   for (;;)
   {
      ret = SGetNxtDBuf(mBuf, &dBuf);
      if (ret == ROK)
      {
         ret = SGetDataTx(dBuf, &dPtr, &dLen);
         if (ret != ROK)
         {  
            ret = RFAILED;
            break;
         }

         txArr[iovIdx].iov_base = (S8 *)dPtr;
         txArr[iovIdx].iov_len = dLen;

         allocLen += dLen;
      }
      else if  (ret == ROKDNA)
      {  
         ret = ROK;
         break;
      }
      else
      {
         ret = RFAILED;
         break;
      }

      iovIdx += 1;

      if (iovIdx >= numDBufs)
      {
         if (allocLen >= msgLen)
            ret = ROK;
         else
            ret = RNA;
         break;
      }
   }

   (*numIovElems) = iovIdx;
   (*strtEndDBufNum) += iovIdx;

   /* cm_inet_c_001.main_50 - Assign the value of dbufs packed in IO-vector */
   *ioLen = allocLen;

   return (ret);

} /* end of buildSendIovec */
#endif /* (defined(WIN32)) && !(defined(CMINETFLATBUF)) */


/*
*
*      Fun:   cmInetSocket
*
*      Desc:  Creates an Internet socket descriptor.
*             On default the socket is non-blocking ( can be changed 
*             with the function cmInetSetOpt()).
*             Values for type:
*
*             CM_INET_STREAM   (TCP)
*             CM_INET_DGRAM    (UDP)
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: None
*
*      File:  cm_inet.c
*
*/
#ifdef CM_INET2  
#ifdef IPV6_SUPPORTED
S16 cmInetSocket
(
uint8_t        type,                 /* socket type */
CmInetFd *sockFd,               /* socket file descriptor */
uint8_t       protocol,              /* protocol value */
uint8_t       domain                 /* domain */
)
#else
S16 cmInetSocket
(
uint8_t        type,                 /* socket type */
CmInetFd *sockFd,               /* socket file descriptor */
uint8_t       protocol               /* protocol value */
)
#endif /* IPV6_SUPPORTED */
#else   /* CM_INET2 */ 
S16 cmInetSocket
(
uint8_t        type,                 /* socket type */
CmInetFd *sockFd                /* socket file descriptor */
)
#endif /* CM_INET2 */ 
{
   S32 ret;                     /* temporary return value */

   uint32_t optVal;

#if (defined(WIN32) && defined(WIN2K))
   S32 bytesReturned;
   Bool bNewBehavior;
#endif /* WIN2K && WIN32 */


#if (defined(WIN32) && defined(WIN2K))
   bytesReturned = 0;
   bNewBehavior = FALSE;
#endif /* WIN32 && WIN2K */  

   /* create socket */
#ifdef CM_INET2  
#ifdef IPV6_SUPPORTED
   sockFd->fd = socket(domain, type, protocol);
#else
   sockFd->fd = socket(AF_INET, type, protocol);
#endif /* IPV6_SUPPORTED */
#else   /* CM_INET2 */ 
   sockFd->fd = socket(AF_INET, type, 0);
#endif  /* CM_INET2 */ 
   if (CM_INET_INV_SOCK_FD(sockFd))
   {   
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSocket() Failed : errno(%d), sockFd->fd(%ld)\n", 
            INET_ERR_CODE, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET001, 0, prntBuf);
#else
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSocket() Failed : errno(%d), sockFd->fd(%d)\n", 
            INET_ERR_CODE, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET001, 0, prntBuf);
#endif /*ALIGN_64BIT */
#endif /* CMINETDBG */
      /* Set sockFd->fd to invalid socket */
      sockFd->fd = CM_INET_INV_SOCKFD;
      return RFAILED;   
   }

   /* set socket type */
   sockFd->type = type;

   /* set socket protocol type (IPv4/IPv6) */
#ifdef IPV6_SUPPORTED   
   sockFd->protType = domain; 
#endif /* IPV6_SUPPORTED */

   /*cm_inet_c_001.main_38 Updated for TUCL 2.1 Release (Kernel SCTP Support) */
#ifdef CM_LKSCTP
   if (protocol != IPPROTO_SCTP)
   {
#endif
   /* set default options */
   optVal = CM_INET_OPT_DISABLE;
   ret = cmInetSetOpt(sockFd, SOL_SOCKET, CM_INET_OPT_BLOCK, (Ptr)&optVal); 
   if (ret != ROK) 
   {
      ret = cmInetClose(sockFd);
      return RFAILED;
   }

#ifdef SS_LINUX
#ifndef CMINET_BSDCOMPAT
   optVal = CM_INET_OPT_ENABLE;
   ret = cmInetSetOpt(sockFd, SOL_SOCKET, CM_INET_OPT_BSD_COMPAT, (Ptr)&optVal);
   if (ret != ROK) 
   {
      ret = cmInetClose(sockFd);
      return RFAILED;
   }
#endif /* CMINET_BSDCOMPAT */
#endif /* SS_LINUX */

#if (defined(WIN32) && defined(WIN2K))   
   if(type == CM_INET_DGRAM)
      {
         ret = WSAIoctl(sockFd->fd, SIO_UDP_CONNRESET, &bNewBehavior,
               sizeof(bNewBehavior), NULLP, 0, &bytesReturned,
               NULLP, NULLP);
         if(ret == INET_ERR)
         {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
            /* cm_inet_c_001.main_54: CMINETLOGERROR added insted of SDisp */
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "WSAIoctl() Failed : error(%d), sockFd->fd(%ld)\n", 
                  INET_ERR_CODE, sockFd->fd);
            CMINETLOGERROR(ERRCLS_DEBUG, ECMINET002, 0, prntBuf);
#else
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "WSAIoctl() Failed : error(%d), sockFd->fd(%d)\n", 
                  INET_ERR_CODE, sockFd->fd);
            CMINETLOGERROR(ERRCLS_DEBUG, ECMINET002, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */      
            ret = cmInetClose(sockFd);
            return RFAILED;
         }
      }
#endif /* WIN2K && WIN32 */   
      /*cm_inet_c_001.main_38 Updated for TUCL 2.1 Release (Kernel SCTP Support) */
#ifdef CM_LKSCTP
   }
#ifdef CM_LKSCTP_NONBLOCK
   else
   {
      /* cm_inet_c_001.main_47:if non-blocking SCTP socket compile time
       *        * flag is set then even for kernel SCTP make the socket
       *               * non-blocking
       *                      */
      optVal = CM_INET_OPT_DISABLE;
      ret = cmInetSetOpt(sockFd, SOL_SOCKET, CM_INET_OPT_BLOCK, (Ptr)&optVal);
      if (ret != ROK)
      {
         ret = cmInetClose(sockFd);
         return RFAILED;
      }
   }
#endif  /* CM_LKSCTP_NONBLOCK ends */
#endif
   return ROK;
} /* end of cmInetSocket */


/*
*
*      Fun:   cmInetBind 
*
*      Desc:  Binds a socket file descriptor to a local Internet 
*             address/port.
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: None.
*
*      File:  cm_inet.c
*
*/

S16 cmInetBind
(
CmInetFd   *sockFd,             /* socket file descriptor */ 
CmInetAddr *myAddr              /* locale Internet address/port */
)
{
   S32 ret;                     /* temporary return value */
   struct sockaddr_in srcAddr;  /* local Internet address/port */
#ifdef IPV6_SUPPORTED 
   struct sockaddr_in6 srcAddr6; /* local IPV6 address/port */
#ifdef CMINETDBG
   uint16_t    port;
#endif /* CMINETDBG */
#endif /* IPV6_SUPPORTED */
   uint32_t    sizeOfAddr;            /* sizeof address passed to the bind call */
   CmInetSockAddr *sockAddrPtr; 


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd) ||
       (myAddr == NULLP))
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

#ifdef IPV6_SUPPORTED 
   if (myAddr->type == CM_INET_IPV6ADDR_TYPE)
   {
      memset(&srcAddr6, 0, sizeof(srcAddr6));
      srcAddr6.sin6_family      = AF_INET6;
      srcAddr6.sin6_port        = CM_INET_HTON_UINT16(myAddr->u.ipv6Addr.port);
      CM_INET_COPY_IPV6ADDR(&srcAddr6.sin6_addr, 
                            &myAddr->u.ipv6Addr.ipv6NetAddr);
      sizeOfAddr               = sizeof(struct sockaddr_in6);
      sockAddrPtr              = (CmInetSockAddr *)&srcAddr6;
   }
   else 
   {
      memset(&srcAddr, 0, sizeof(srcAddr));
      srcAddr.sin_family      = AF_INET;
      srcAddr.sin_port        = CM_INET_HTON_UINT16(myAddr->u.ipv4Addr.port);
      srcAddr.sin_addr.s_addr = CM_INET_HTON_UINT32(myAddr->u.ipv4Addr.address);
      sizeOfAddr               = sizeof(struct sockaddr_in);
      sockAddrPtr              = (CmInetSockAddr *)&srcAddr;
   }
#else 
   memset(&srcAddr, 0, sizeof(srcAddr));
   srcAddr.sin_family      = AF_INET;
   srcAddr.sin_port        = CM_INET_HTON_UINT16(myAddr->port);
   srcAddr.sin_addr.s_addr = CM_INET_HTON_UINT32(myAddr->address);
   sizeOfAddr              = sizeof(struct sockaddr_in);
   sockAddrPtr             = (CmInetSockAddr *)&srcAddr;
#endif /* IPV6_SUPPORTED */

   ret = bind(sockFd->fd, sockAddrPtr, sizeOfAddr); 
   if (ret == INET_ERR)
   {
#ifdef CMINETDBG
#ifdef IPV6_SUPPORTED 
      if (myAddr->type == CM_INET_IPV6ADDR_TYPE)
         port = myAddr->u.ipv6Addr.port;
      else
         port = myAddr->u.ipv4Addr.port;
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetBind() Failed : error(%d), addrType(%d)," 
            " port(%d), sockFd->fd(%ld)\n", 
            INET_ERR_CODE , myAddr->type, port, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET003, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetBind() Failed : error(%d), addrType(%d)," 
            " port(%d), sockFd->fd(%d)\n ", 
            INET_ERR_CODE , myAddr->type, port, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET003, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#else
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetBind() Failed : error(%d), addr(0x%lx), port(%d),"
            "sockFd->fd(%ld)\n",
            INET_ERR_CODE , myAddr->address, myAddr->port, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET004, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetBind() Failed : error(%d), addr(0x%x), port(%d),"
            " sockFd->fd(%d)\n",
            INET_ERR_CODE , myAddr->address, myAddr->port, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET004, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* IPV6_SUPPORTED */
#endif /* CMINETDBG */
      return RFAILED;
   }

   return ROK; 
} /* end of cmInetBind */

/*cm_inet_c_001.main_38 Updated for TUCL 2.1 Release (Kernel SCTP Support) */
/* cm_inet_c_001.main_51 Added Ipv6 support to KSCtP implementation */
#ifdef CM_LKSCTP
/*
*
*      Fun:   cmInetSctpBindx 
*
*      Desc:  Binds a SCTP socket file descriptor to local Internet 
*             address list
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: None.
*
*      File:  cm_inet.c
*
*/
S16 cmInetSctpBindx
(
CmInetFd          *sockFd,       /* socket file descriptor */ 
CmInetNetAddrLst  *addrLst,      /* local Internet address list */
uint16_t                port          /* port number */
)
{
   S32    ret;                     /* temporary return value */
   S32    idx;
   S32    idx4 = 0;
   uint32_t    ipv4_array_size = 0;
   struct sockaddr_in  addrs[CM_INET_NUM_NET_ADDR];
#ifndef IPV6_SUPPORTED
   Data   address_array[(CM_INET_NUM_NET_ADDR * sizeof(struct sockaddr_in))];
#else
   Data   address_array[(CM_INET_NUM_NET_ADDR * sizeof(struct sockaddr_in6))];
#endif /* IPV6_SUPPORTED */

#ifdef SUN_KSCTP
   Data   *tempAddrPtr = NULLP;
#endif 
   uint32_t    addresses_array_size = 0;
#ifdef IPV6_SUPPORTED 
#ifdef SUN_KSCTP
   S8     *addrString = NULLP;
   uint32_t    addrLen = 0;
   S8     ipv4Format[23] = "::ffff:";
#endif /* SUN_KSCTP */
   S32    idx6 = 0;
   uint32_t    ipv6_array_size = 0;
   struct sockaddr_in6 addrs6[CM_INET_NUM_NET_ADDR];
#endif /* IPV6_SUPPORTED */
   struct sockaddr *sockAddrPtr = NULLP;
   uint32_t    sockAddrLen = 0;

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd) ||
       (addrLst == NULLP))
   {
      return RFAILED;
   }

   if(addrLst->count > CM_INET_NUM_NET_ADDR)
   {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "No of address(%d) is greater than Max(%d),"
            " sockFd->fd(%ld)\n",
            addrLst->count, CM_INET_NUM_NET_ADDR, sockFd->fd);
      CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET005, 0, prntBuf);   
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "No of address(%d) is greater than Max(%d),"
            " sockFd->fd(%d)\n", 
            addrLst->count, CM_INET_NUM_NET_ADDR, sockFd->fd);
      CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET005, 0, prntBuf);   
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   memset(&addrs, 0, (sizeof(struct sockaddr_in) * CM_INET_NUM_NET_ADDR));
#ifdef IPV6_SUPPORTED 
   memset(&addrs6, 0, (sizeof(struct sockaddr_in6) * CM_INET_NUM_NET_ADDR));
#endif /* IPV6_SUPPORTED */

   for (idx = 0; idx < addrLst->count; idx++)
   {
#ifdef IPV6_SUPPORTED 
      if (addrLst->addrs[idx].type == CM_INET_IPV6ADDR_TYPE)
      {
         ipv6_array_size += sizeof(struct sockaddr_in6);
         addresses_array_size += sizeof(struct sockaddr_in6);
         if (sockFd->protType == AF_INET)
         {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
            /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "can't bind IPV6 address on IPV4 socket,"
                  " sockFd->fd(%ld)\n", sockFd->fd);
            CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET057, 0, prntBuf);
#else
           /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "can't bind IPV6 address on IPV4 socket," 
                  " sockFd->fd(%d)\n", sockFd->fd);
            CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET057, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
            return RFAILED;
         }

         addrs6[idx6].sin6_family      = AF_INET6;
         addrs6[idx6].sin6_port        = CM_INET_HTON_UINT16(port);
         CM_INET_COPY_IPV6ADDR((addrs6[idx6].sin6_addr.s6_addr), &(addrLst->addrs[idx].u.ipv6NetAddr));
         idx6++;
      }
      else 
      {
#ifdef SUN_KSCTP

         ipv6_array_size += sizeof(struct sockaddr_in6);
         addresses_array_size += sizeof(struct sockaddr_in6);
         if (sockFd->protType == AF_INET)
         {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
            /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "can't bind IPV6 address on IPV4 socket,"
                  " sockFd->fd(%ld)\n", sockFd->fd);
            CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET058, 0, prntBuf);
#else
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "can't bind IPV6 address on IPV4 socket," 
                  " sockFd->fd(%d)\n", sockFd->fd);
            CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET058, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
            return RFAILED;
         }

         addrs6[idx6].sin6_family      = AF_INET6;
         addrs6[idx6].sin6_port        = CM_INET_HTON_UINT16(port);
         addrLst->addrs[idx].u.ipv4NetAddr = CM_INET_HTON_UINT32(addrLst->addrs[idx].u.ipv4NetAddr);
         cmInetNtoa(addrLst->addrs[idx].u.ipv4NetAddr, &addrString);
         addrLen = cmStrlen((uint8_t*)addrString);
         memcpy((ipv4Format+7), addrString, addrLen);
         ipv4Format[7+addrLen] = '\0';
         cmInetPton6((CmInetIpAddr6*)(addrs6[idx6].sin6_addr.s6_addr), ipv4Format);
         idx6++;
#else
         ipv4_array_size += sizeof(struct sockaddr_in);
         addresses_array_size += sizeof(struct sockaddr_in);
         addrs[idx4].sin_family      = AF_INET;
         addrs[idx4].sin_port        = CM_INET_HTON_UINT16(port);
         addrs[idx4].sin_addr.s_addr = CM_INET_HTON_UINT32(addrLst->addrs[idx].u.ipv4NetAddr);
         idx4++;
#endif /* SUN_KSCTP */
      }
#else 
      ipv4_array_size += sizeof(struct sockaddr_in);
      addresses_array_size += sizeof(struct sockaddr_in);
      addrs[idx4].sin_family      = AF_INET;
      addrs[idx4].sin_port        = CM_INET_HTON_UINT16(port);
      addrs[idx4].sin_addr.s_addr = CM_INET_HTON_UINT32(addrLst->addrs[idx].u.ipv4NetAddr);
      idx4++;
#endif /* IPV6_SUPPORTED */
   }


   if(ipv4_array_size > 0)
   {
       sockAddrPtr = (struct sockaddr*)address_array;
      sockAddrLen = sizeof(struct sockaddr_in);
      memcpy(address_array, addrs, ipv4_array_size); 
   }
#ifdef IPV6_SUPPORTED
   else
   {
       sockAddrPtr = (struct sockaddr*)address_array;
      sockAddrLen = sizeof(struct sockaddr_in6);
   }

   if(ipv6_array_size > 0)
   {
      memcpy((address_array + ipv4_array_size), addrs6, ipv6_array_size); 
   }
#endif /* IPV6_SUPPORTED */


#ifdef SUN_KSCTP
   ret = bind(sockFd->fd, sockAddrPtr, sockAddrLen); 
   if (ret == INET_ERR)
   {

#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSctpBindx() Failed : error(%d), port(%d),"
            " sockFd->fd(%ld)\n",INET_ERR_CODE, port, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET006, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSctpBindx() Failed : error(%d), port(%d),"
            " sockFd->fd(%d)\n",INET_ERR_CODE, port, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET006, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
      return RFAILED;
   }

   if (addrLst->count > 1)
   {
      if(((struct sockaddr*)address_array)->sa_family == AF_INET)
      {
         tempAddrPtr = address_array + (sizeof(struct sockaddr_in));
      }
      else if(((struct sockaddr*)address_array)->sa_family == AF_INET6)
      {
         tempAddrPtr = address_array + (sizeof(struct sockaddr_in6));
      }
      else
      {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
         /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSctpBindx(Invalid address Type) failed:"
               " sockFd->fd(%ld),  error(%d), port(%d)\n ",
               INET_ERR_CODE, port, sockFd->fd);
         CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET059, 0, prntBuf);
#else
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSctpBindx(Invalid address Type) failed:"
               " sockFd->fd(%d), error(%d), port(%d)\n ", 
               INET_ERR_CODE, port, sockFd->fd);
         CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET059, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
         return RFAILED;
      }

      ret = sctp_bindx(sockFd->fd, (Void*)tempAddrPtr, addrLst->count - 1, SCTP_BINDX_ADD_ADDR); 
   }
#else
   /* linux */
   ret = sctp_bindx(sockFd->fd, (struct sockaddr*)address_array, addrLst->count, SCTP_BINDX_ADD_ADDR); 
	UNUSED(sockAddrPtr);
	UNUSED(sockAddrLen);
#endif
   if (ret == INET_ERR)
   {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_54: CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSctpBindx() Failed : error(%d), port(%d),"
            " sockFd->fd(%ld)\n",INET_ERR_CODE, port, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET007, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSctpBindx() Failed : error(%d), port(%d),"
            " sockFd->fd(%d)\n",INET_ERR_CODE, port, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET007, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
      return RFAILED;
   }

   return ROK; 
}

/*
*
*      Fun:   cmInetSctpConnectx 
*
*      Desc:  Establishes a sctp connection with remote addresses
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: None.
*
*      File:  cm_inet.c
*
*/
S16 cmInetSctpConnectx
(
CmInetFd          *sockFd,       /* socket file descriptor */ 
CmInetNetAddr     *primAddr,     /* primary destination Internet address */
CmInetNetAddrLst  *addrLst,      /* destination Internet address list */
uint16_t          port          /* port number */
)
{
   S32   ret;   
   uint32_t   cnt;
   /* cm_inet_c_001.main_46: Removed SS_LINUX flag */
   S32   idx;

/* cm_inet_c_001.main_64: New variable used as an argument for sctp_connectx */
#ifdef SCTP_CONNECTX_NEW
   uint32_t    assocId = 0;
#endif   
   uint32_t    addresses_array_size = 0;
   uint32_t    idx4 = 0;
   struct sockaddr_in  addrs[CM_INET_NUM_NET_ADDR];
   uint32_t    ipv4_array_size = 0;

#ifndef IPV6_SUPPORTED
   Data   address_array[(CM_INET_NUM_NET_ADDR * sizeof(struct sockaddr_in))];
#else
   Data   address_array[(CM_INET_NUM_NET_ADDR * sizeof(struct sockaddr_in6))];
#endif /* IPV6_SUPPORTED */

#ifdef IPV6_SUPPORTED
#ifdef SUN_KSCTP
   S8     *addrString = NULLP;
   uint32_t    addrLen = 0;
   S8     ipv4Format[23] = "::ffff:";
   CmInetIpAddr ipv4NetAddr;
#endif /* SUN_KSCTP */
   uint32_t    idx6 = 0;
   struct sockaddr_in6  addrs6[CM_INET_NUM_NET_ADDR];
   uint32_t    ipv6_array_size = 0;
#endif /* IPV6_SUPPORTED */
#ifndef SS_LINUX
   uint32_t    sockAddrLen = 0;
#endif /* sockAddrLen */

#ifndef SS_LINUX
   CmInetSockAddr *sockAddrPtr = NULLP;
#endif /* SS_LINUX */
#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd) ||
         (primAddr == NULLP))
   {
      return RFAILED;
   }
    /* cm_inet_c_001.main_58 : Added check for addrLst to fix klockwork issue */
   if (addrLst == NULLP)
   {
      return RFAILED;
   }
   /* cm_inet_c_001.main_46: Included check for no of address aginst max */
   if( addrLst->count > CM_INET_NUM_NET_ADDR )
   {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "No of address(%d) is greater than Max(%d),"
            " sockFd->fd(%ld)\n", 
            addrLst->count, CM_INET_NUM_NET_ADDR, sockFd->fd);
      CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET060, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "No of address(%d) is greater than Max(%d),"
            " sockFd->fd(%d)\n", 
            addrLst->count, CM_INET_NUM_NET_ADDR, sockFd->fd);
      CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET060, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */


   memset(&addrs, 0, (sizeof(struct sockaddr_in) * CM_INET_NUM_NET_ADDR));
#ifdef IPV6_SUPPORTED
   memset(&addrs6, 0, (sizeof(struct sockaddr_in6) * CM_INET_NUM_NET_ADDR));
#endif /* IPV6_SUPPORTED */

   cnt = 0;

#ifdef IPV6_SUPPORTED 
   if (primAddr->type == CM_INET_IPV6ADDR_TYPE)
   {
      if (sockFd->protType == AF_INET)
      {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
         /* cm_inet_c_001.main_54: CMINETLOGERROR added insted of SDisp */
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "Can't connect to IPV6 address through IPV4 socket,"
               " sockFd->fd(%ld)\n", sockFd->fd);
         CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET008, 0, prntBuf);
#else
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "Can't connect to IPV6 address through IPV4 socket,"
               " sockFd->fd(%d)\n",  sockFd->fd);
         CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET008, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
         return RFAILED;
      }

      addrs6[idx6].sin6_family      = AF_INET6;
      addrs6[idx6].sin6_port        = CM_INET_HTON_UINT16(port);
      CM_INET_COPY_IPV6ADDR(&(addrs6[idx6].sin6_addr.s6_addr), &(primAddr->u.ipv6NetAddr));
      addresses_array_size += sizeof(struct sockaddr_in6);
      ipv6_array_size += sizeof(struct sockaddr_in6);
      idx6++;
   }
   else 
   {
#ifdef SUN_KSCTP
      if (sockFd->protType == AF_INET)
      {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
         /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "can't connect to IPV6 address through IPV4 socket,"
               " sockFd->fd(%ld)\n", sockFd->fd);
         CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET061, 0, prntBuf);
#else
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "can't connect to IPV6 address through IPV4 socket,"
               " sockFd->fd(%d)\n", sockFd->fd);
         CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET061, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
         return RFAILED;
      }
      addrs6[idx6].sin6_family      = AF_INET6;
      addrs6[idx6].sin6_port        = CM_INET_HTON_UINT16(port);
      ipv4NetAddr = CM_INET_HTON_UINT32(primAddr->u.ipv4NetAddr);
      cmInetNtoa(ipv4NetAddr, &addrString);
      addrLen = cmStrlen((uint8_t*)addrString);
      memcpy((ipv4Format+7), addrString, addrLen);
      ipv4Format[7+addrLen] = '\0';
      cmInetPton6((CmInetIpAddr6*)&(addrs6[idx6].sin6_addr), ipv4Format);
      addresses_array_size += sizeof(struct sockaddr_in6);
      ipv6_array_size += sizeof(struct sockaddr_in6);
      idx6++;
#else
      addrs[idx4].sin_family      = AF_INET;
      addrs[idx4].sin_port        = CM_INET_HTON_UINT16(port);
      addrs[idx4].sin_addr.s_addr = CM_INET_HTON_UINT32(primAddr->u.ipv4NetAddr);
      addresses_array_size += sizeof(struct sockaddr_in);
      ipv4_array_size += sizeof(struct sockaddr_in);
      idx4++;
#endif 
   }
#else 
   addrs[idx4].sin_family      = AF_INET;
   addrs[idx4].sin_port        = CM_INET_HTON_UINT16(port);
   addrs[idx4].sin_addr.s_addr = CM_INET_HTON_UINT32(primAddr->u.ipv4NetAddr);
   addresses_array_size += sizeof(struct sockaddr_in);
   ipv4_array_size += sizeof(struct sockaddr_in);
   idx4++;
#endif /* IPV6_SUPPORTED */

   cnt++;

   /* cm_inet_c_001.main_46: Moved the SS_LINUX flag down, 
    * copy addresses in Solaris also */
   if (addrLst != NULLP)
   {
      for (idx = 0; idx < addrLst->count; idx++)
      {

         /* cm_inet_c_001.main_46: Don't include the primary address 
          * if its prersent in list */
         if ( addrLst->addrs[idx].type == CM_INET_IPV4ADDR_TYPE )
         {
            if ( addrLst->addrs[idx].u.ipv4NetAddr == primAddr->u.ipv4NetAddr )
            {
               continue;
            }
         }
#ifdef IPV6_SUPPORTED
         else if ( addrLst->addrs[idx].type == CM_INET_IPV6ADDR_TYPE )
         {
            if (( cmMemcmp(addrLst->addrs[idx].u.ipv6NetAddr,
                   primAddr->u.ipv6NetAddr, sizeof(CmInetIpAddr6) )) == 0 )
            {
               continue;
            }
         }

         if (addrLst->addrs[idx].type == CM_INET_IPV6ADDR_TYPE)
         {
            if (sockFd->protType == AF_INET)
            {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
               /* cm_inet_c_001.main_54: CMINETLOGERROR added insted of SDisp */
               /* cm_inet_c_001.main_62:Warning fix */
               snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "Can't connect to IPV6 address through IPV4 socket,"
                     " sockFd->fd(%ld)\n", sockFd->fd);
               CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET009, 0, prntBuf);
#else
               /* cm_inet_c_001.main_62:Warning fix */
               snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "Can't connect to IPV6 address through IPV4 socket,"
                     " sockFd->fd(%d)\n", sockFd->fd);
               CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET009, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
               return RFAILED;
            }

            addrs6[idx6].sin6_family      = AF_INET6;
            addrs6[idx6].sin6_port        = CM_INET_HTON_UINT16(port);
            CM_INET_COPY_IPV6ADDR(&(addrs6[idx6].sin6_addr.s6_addr),
                                  &(addrLst->addrs[idx].u.ipv6NetAddr));
            addresses_array_size += sizeof(struct sockaddr_in6);
            ipv6_array_size += sizeof(struct sockaddr_in6);
            idx6++;
         }
         else 
         {
#ifdef SUN_KSCTP
            if (sockFd->protType == AF_INET)
            {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
               /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
               /* cm_inet_c_001.main_62:Warning fix */
               snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "can't connect to IPV6 address through IPV4 socket,"
                     " sockFd->fd(%ld)\n", sockFd->fd);
               CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET062, 0, prntBuf);
#else
               /* cm_inet_c_001.main_62:Warning fix */
               snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "can't connect to IPV6 address through IPV4 socket,"
                     " sockFd->fd(%d)\n", sockFd->fd);
               CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET062, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
               return RFAILED;
            }
            addrs6[idx6].sin6_family      = AF_INET6;
            addrs6[idx6].sin6_port        = CM_INET_HTON_UINT16(port);
            ipv4NetAddr = CM_INET_HTON_UINT32(addrLst->addrs[idx].u.ipv4NetAddr);
            cmInetNtoa(ipv4NetAddr, &addrString);
            addrLen = cmStrlen((uint8_t*)addrString);
            memcpy((ipv4Format+7), addrString, addrLen);
            ipv4Format[7+addrLen] = '\0';
            cmInetPton6((CmInetIpAddr6*)(addrs6[idx6].sin6_addr.s6_addr), ipv4Format);
            addresses_array_size += sizeof(struct sockaddr_in6);
            ipv6_array_size += sizeof(struct sockaddr_in6);
            idx6++;
#else
            addrs[idx4].sin_family      = AF_INET;
            addrs[idx4].sin_port        = CM_INET_HTON_UINT16(port);
            addrs[idx4].sin_addr.s_addr = CM_INET_HTON_UINT32(addrLst->addrs[idx].u.ipv4NetAddr);
            addresses_array_size += sizeof(struct sockaddr_in);
            ipv4_array_size += sizeof(struct sockaddr_in);
            idx4++;
#endif /* SUN_KSCTP */
         }
#else 
         addrs[idx4].sin_family      = AF_INET;
         addrs[idx4].sin_port        = CM_INET_HTON_UINT16(port);
         addrs[idx4].sin_addr.s_addr = CM_INET_HTON_UINT32(addrLst->addrs[idx].u.ipv4NetAddr);
         addresses_array_size += sizeof(struct sockaddr_in);
         ipv4_array_size += sizeof(struct sockaddr_in);
         idx4++;
#endif /* IPV6_SUPPORTED */
         /*cm_inet_c_001.main_39 */
         cnt++;
      }
   }

   /* cm_inet_c_001.main_46: Moved SS_LINUX flag to here */
#ifdef SS_LINUX
   /*cm_inet_c_001.main_58 : Added check array_size to fix klockwork issue */
   if((ipv4_array_size > 0) && (ipv4_array_size <= (CM_INET_NUM_NET_ADDR * \
               sizeof(struct sockaddr_in))))
   {
      memcpy(address_array, &addrs[0], ipv4_array_size); 
   }
   else
   {
      return RFAILED;
   }

#ifdef IPV6_SUPPORTED
   if((ipv6_array_size > 0) && (ipv6_array_size <= (CM_INET_NUM_NET_ADDR * \
               sizeof(struct sockaddr_in))))
   {
      memcpy((address_array + ipv4_array_size), addrs6, ipv6_array_size); 
   }
   else
   {
      return RFAILED;
   }
#endif /* IPV6_SUPPORTED */

/* cm_inet_c_001.main_64: Support for new definition of sctp_connectx */
#ifndef SCTP_CONNECTX_NEW   
   ret = sctp_connectx(sockFd->fd, (struct sockaddr*)address_array, cnt);
#else
   ret = sctp_connectx(sockFd->fd, (struct sockaddr*)address_array, cnt, (sctp_assoc_t *)&assocId);
#endif

#else
   /* solaris */
   /* cm_inet_c_001.main_46: Use next provided address to connect if 
    * first one fails */

#ifdef CMINET_SUN_CONNECTX
   idx4 = 0;
#ifdef IPV6_SUPPORTED
   idx6 = 0;
#endif /* IPV6_SUPPORTED */
   for (idx = 0; idx < cnt; idx++)
   {
      if( addrs[idx4].sin_family == AF_INET)
      {
        sockAddrPtr = (CmInetSockAddr *)&addrs[idx4];
        sockAddrLen = sizeof(struct sockaddr_in);
        idx4++;
      }
#ifdef IPV6_SUPPORTED
      else 
      {
        sockAddrPtr = (CmInetSockAddr *)&addrs6[idx6];
        sockAddrLen = sizeof(struct sockaddr_in6);
        idx6++;
      }
#endif/* IPV6_SUPPORTED */

      ret = connect(sockFd->fd, sockAddrPtr, sockAddrLen);

        if ( ret != INET_ERR )
        {
            break;
        }
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSctpConnectx() failed:error(%d), port(0x%1x),"
            " sockFd->fd(%ld)\n", INET_ERR_CODE, port, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET063, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSctpConnectx() failed:error(%d), port(0x%1x),"
            " sockFd->fd(%d)\n", INET_ERR_CODE, port, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET063, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */

   }
#else

   if( addrs[0].sin_family == AF_INET)
   {
     sockAddrPtr = (CmInetSockAddr*)&addrs[0];
     sockAddrLen = sizeof(struct sockaddr_in);
     idx4++;
   }
#ifdef IPV6_SUPPORTED
   else 
   {
     sockAddrPtr = (CmInetSockAddr*)&addrs6[0];
     sockAddrLen = sizeof(struct sockaddr_in6);
     idx6++;
   }
#endif/* IPV6_SUPPORTED */

   ret = connect(sockFd->fd, sockAddrPtr, sockAddrLen);

#endif /* CMINET_SUN_CONNECTX */
#endif /* SS_LINUX */

   if (ret == INET_ERR)
   {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_54: CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "CmInetSctpConnectx() Failed : error(%d), port(0x%1x),"
            " sockFd->fd(%ld)\n", INET_ERR_CODE, port, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET010, 0, prntBuf);
#else
      DU_LOG("\nCmInetSctpConnectx() Failed : error(%d), port(0x%1x),\
		   sockFd->fd(%d)\n", INET_ERR_CODE, port, sockFd->fd);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */

      switch (INET_ERR_CODE)
      {
         /* non-blocking: connection is in progress */
         case ERR_INPROGRESS:
            return (RINPROGRESS);
            break;   

         /* 
          * non-blocking: connection is established 
          * blocking    : connection is already established
          */
         case ERR_ISCONN:
            return (RISCONN);
            break;               

         /* resource temporarily unavailable */
         case ERR_WOULDBLOCK:
            return (ROKDNA);
            break;

         /* non-blocking: connection is in progress */
         case ERR_ALREADY:
            return (RINPROGRESS);
            break;

         case ERR_INVAL:
            return (RINPROGRESS);
            break;

         /*  Check for connection refused and timeout errors */
         case ERR_CONNREFUSED:
         case ERR_TIMEDOUT:
            return (RCLOSED);
            break;

         /* it is a real error */ 
         default:
            return RFAILED;
            break;
      }
   }

   return ROK;
}

/*
*
*      Fun:   cmInetSctpPeelOff 
*
*      Desc:  Branches an existing sctp association off to a seperate socket 
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: None.
*
*      File:  cm_inet.c
*
*/
S16 cmInetSctpPeelOff
(
CmInetFd          *sockFd,       /* socket file descriptor */ 
uint32_t          assocId,      /* association id */
CmInetFdType      *assocFd       /* association fd */
)
{
   S32 ret;

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd) || (assocFd == NULLP)) 
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */


   ret = sctp_peeloff(sockFd->fd, assocId);
   if (ret == INET_ERR)
   {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_54: CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSctpPeelOff() Failed : error(%d) assocId(%ld),"
            " sockFd->fd(%ld)\n", INET_ERR_CODE, assocId, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET011, 0, prntBuf);
#else
      /* cm_inet_c_001.main_55: Fix for compilation warning */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSctpPeelOff() Failed : error(%d) assocId(%d),"
            " sockFd->fd(%d)\n", INET_ERR_CODE, assocId, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET011, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */

      return RFAILED;
   }

   *assocFd = ret;

   return ROK;
}

/*
*
*      Fun:   cmInetSctpSendMsg 
*
*      Desc:  invokes sctp socket API to send message to the remote addresses
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: None.
*
*      File:  cm_inet.c
*
*/
S16 cmInetSctpSendMsg
(
CmInetFd        *sockFd,       /* socket file descriptor */ 
CmInetNetAddr   *dstAddr,      /* destination Internet address/port */
uint16_t        port,         /* destination port no. */
CmInetMemInfo   *info,         /* buffer allocation info */
Buffer          *mBuf,         /* buffer structure to send */
MsgLen          *len,          /* number of actually sent octets */
uint16_t        strmId,       /* sctp stream identifier */
Bool            unorderFlg,   /* flag to enable the unordered delivery */
uint16_t        ttl,          /* time to live */
uint32_t        ppId,         /* opaque value passed along with the message */
uint32_t        context       /* value to be passed back, if error occurs */
)
{
   S32     ret;   
 /* cm_inet_c_001.main_58 : Fix for klockwork issue */
   MsgLen  msgLen = 0;              /* message length */    
   MsgLen  bufLen = 0;              /* send buffer length */     
   Data   *sendBuf = NULLP;             /* plain send buffer */
   uint32_t     flags =0;
   CmInetSockAddr *sockAddrPtr = NULLP;
   /* cm_inet_c_001.main_58 : Fix for klockwork issue */
   MsgLen          sockAddrLen = 0; 
   struct sockaddr_in  addr ={0};
#ifdef IPV6_SUPPORTED
#ifdef SUN_KSCTP
   S8     *addrString = NULLP;
   uint32_t    addrLen = 0;
   S8     ipv4Format[23] = "::ffff:";
   CmInetIpAddr ipv4NetAddr = {0};
#endif /* SUN_KSCTP */
   struct sockaddr_in6  addr6 ={0};
#endif /* IPV6_SUPPORTED */
#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd)
       || (info == NULLP) || (mBuf == NULLP) || (len == NULLP))
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */


   memset(&addr, 0, sizeof(struct sockaddr_in));
#ifdef IPV6_SUPPORTED
   memset(&addr6, 0, sizeof(struct sockaddr_in6));
#endif /* IPV6_SUPPORTED */

   /* copy message to a flat buffer */
   ret = SFndLenMsg(mBuf, &bufLen);
   if (ret != ROK)
   {
      return RFAILED;
   }
   /* max message length is limited to control the memory usage */
   /* casting bufLen to avoid warnings */
   if ((bufLen > 0) && ((uint32_t)bufLen > CM_INET_MAX_MSG_LEN))
   {
      return RFAILED;
   }
   ret = SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, &sendBuf, bufLen);                  
   if (ret != ROK)
   {
      return (ROUTRES);
   }
   ret = SCpyMsgFix(mBuf, 0, bufLen, sendBuf, &msgLen);
   if ((ret != ROK) || (msgLen != bufLen)) 
   {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, sendBuf, bufLen);       
      return RFAILED;
   }

   if ( dstAddr != NULLP)
   {
#ifdef IPV6_SUPPORTED 
      if (dstAddr->type == CM_INET_IPV6ADDR_TYPE)
      {
         if (sockFd->protType == AF_INET)
         {
            SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, sendBuf, bufLen);       
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
            /* cm_inet_c_001.main_54: CMINETLOGERROR added insted of SDisp */
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "Can't send message to IPV6 address through"
                  " IPV4 socket, sockFd->fd(%ld)\n", sockFd->fd);
            CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET012, 0, prntBuf);
#else
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "Can't send message to IPV6 address through"
                  " IPV4 socket, sockFd->fd(%d)\n", sockFd->fd);
            CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET012, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
            return RFAILED;
         }

         addr6.sin6_family      = AF_INET6;
         addr6.sin6_port        = CM_INET_HTON_UINT16(port);
         CM_INET_COPY_IPV6ADDR(&addr6.sin6_addr.s6_addr, &dstAddr->u.ipv6NetAddr); 
         sockAddrLen = sizeof(struct sockaddr_in6);
         sockAddrPtr = (CmInetSockAddr*)&addr6;
      }
      else 
      {

#ifdef SUN_KSCTP
         if (sockFd->protType == AF_INET)
         {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
            /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "can't connect to IPV6 address through IPV4"
                  " socket, sockFd->fd(%ld)\n", sockFd->fd);
            CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET064, 0, prntBuf);
#else
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "can't connect to IPV6 address through IPV4"
                  " socket, sockFd->fd(%d)\n", sockFd->fd);
            CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET064, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
            return RFAILED;
         }
         addr6.sin6_family      = AF_INET6;
         addr6.sin6_port        = CM_INET_HTON_UINT16(port);
         ipv4NetAddr = CM_INET_HTON_UINT32(dstAddr->u.ipv4NetAddr);
         cmInetNtoa(ipv4NetAddr, &addrString);
         addrLen = cmStrlen((uint8_t*)addrString);
         memcpy((ipv4Format+7), addrString, addrLen);
         ipv4Format[7+addrLen] = '\0';
         cmInetPton6((CmInetIpAddr6*)(addr6.sin6_addr.s6_addr), ipv4Format);
         sockAddrLen = sizeof(struct sockaddr_in6);
         sockAddrPtr = (CmInetSockAddr*)&addr6;
#else
         addr.sin_family      = AF_INET;
         addr.sin_port        = CM_INET_HTON_UINT16(port);
         addr.sin_addr.s_addr = CM_INET_HTON_UINT32(dstAddr->u.ipv4NetAddr);
         sockAddrLen = sizeof(struct sockaddr_in);
         sockAddrPtr = (CmInetSockAddr*)&addr;
#endif /* SUN_KSCTP */
      }
#else 
      addr.sin_family      = AF_INET;
      addr.sin_port        = CM_INET_HTON_UINT16(port);
      addr.sin_addr.s_addr = CM_INET_HTON_UINT32(dstAddr->u.ipv4NetAddr);
      /* cm_inet_c_001.main_58 : Fix for Klockwork issue */
      sockAddrLen = (MsgLen)sizeof(struct sockaddr_in);
      sockAddrPtr = (CmInetSockAddr*)&addr;
#endif /* IPV6_SUPPORTED */
   }
   else
   {
#ifdef SUN_KSCTP
      sockAddrPtr = NULL;
#else
      sockAddrPtr = (CmInetSockAddr*)&addr;
#endif
      /* cm_inet_c_001.main_58 : initialized sockAddrLen properly */
#ifdef IPV6_SUPPORTED 
      sockAddrLen = sizeof(struct sockaddr_in6);
#else
      sockAddrLen = sizeof(struct sockaddr_in);
#endif
   }

   /* Not validating the address, whether addr is a valid address or not */

   *len  = 0;
   flags = 0x00000000;

   if (unorderFlg == TRUE)
   {
#ifdef SUN_KSCTP 
      flags |= MSG_UNORDERED;
#else
      /* linux */
      flags |= SCTP_UNORDERED;
#endif
   }
   /*cm_inet_c_001.main_54: converting ppid to network*/
   ppId = CM_INET_HTON_UINT32(ppId);
   ret = sctp_sendmsg(sockFd->fd, (Void*)sendBuf, bufLen, 
         (struct sockaddr*)sockAddrPtr, (size_t)sockAddrLen, 
         ppId, flags, strmId, ttl, context);
   if (ret == INET_ERR)
   {
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, sendBuf, bufLen);       
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_54: CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSctpSendMsg() Failed : error(%d) ppId(%ld),"
            " strmId(%u),sockFd->fd(%ld)\n",
            INET_ERR_CODE, ppId, strmId, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET013, 0, prntBuf);
#else
      /* cm_inet_c_001.main_55: Fix for compilation warning */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSctpSendMsg() Failed : error(%d) ppId(%d),"
            " strmId(%u),sockFd->fd(%d)\n",
            INET_ERR_CODE, ppId, strmId, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET013, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */

      if ((INET_ERR_CODE == ERR_AGAIN) || (INET_ERR_CODE == ERR_WOULDBLOCK))
         return (RWOULDBLOCK);
      else if (INET_ERR_CODE == ERR_PIPE)
         return (RCLOSED);
      else 
         return RFAILED;
   }

   /* cm_inet_c_001.main_58 : Fix for klockwork issue */
   *len = (MsgLen)ret; 

   /* cleanup */
   SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, sendBuf, bufLen);       

   return ROK;
}

/*
*
*      Fun:   cmInetSctpRecvMsg 
*
*      Desc:  invokes sctp API to get the message received at sctp socket
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: None.
*
*      File:  cm_inet.c
*
*/
S16 cmInetSctpRecvMsg
(
CmInetFd               *sockFd,       /* socket file descriptor */ 
CmInetNetAddr          *srcAddr,      /* source Internet address/port */
uint16_t               *port,         /* source port no. */
CmInetMemInfo          *meminfo,      /* buffer allocation info */
Buffer                 **mBuf,         /* buffer structure received */
MsgLen                 *len,          /* number of octets received */
CmInetSctpSndRcvInfo   *sinfo,        /* sctp send-receive info */ 
uint32_t               *flag,         /* flags */
CmInetSctpNotification *ntfy        /* notification parameters */
)
{
   S32                        ret;   
   S32                        msgFlags;
   struct sctp_sndrcvinfo     info;
   struct sockaddr_storage    addr;
   struct sockaddr_in        *pAddr = NULLP;
#ifdef IPV6_SUPPORTED
   struct sockaddr_in6       *pAddr6 = NULLP;
#endif 
   socklen_t                  addrlen;
   Data                      *recvbuf = NULLP;
   MsgLen                     buflen;
   union sctp_notification   *sctpNtfy = NULLP;
   /* cm_inet_c_001.main_46: Defined new variable to store length of data */
#ifdef SS_LINUX
   MsgLen                     datlen;
#endif /* SS_LINUX */

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd) || 
       (srcAddr == NULLP) || (port == NULLP) || (meminfo == NULLP) || 
       (mBuf == NULLP) || (len == NULLP) || (sinfo == NULLP) || (flag == NULLP))
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */


   *mBuf = NULLP;
   *len  = 0;
   memset(ntfy, 0, sizeof(CmInetSctpNotification));

   buflen = CM_INET_MAX_MSG_LEN;

   /* allocate flat receive buffer */
   ret = SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,meminfo->region, meminfo->pool, &recvbuf, buflen);
   if (ret != ROK)
   {
#ifdef CMINETDBG
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "SGetSBuf failed to allocate memory\n");
      CMINETLOGERROR(ERRCLS_ADD_RES, ECMINET065, 0, prntBuf);
#endif /* CMINETDBG */
      return RFAILED;
   }

   addrlen = sizeof(struct sockaddr_storage);
   msgFlags = 0;
   memset(&addr, 0, sizeof(struct sockaddr_storage));
   memset(&info, 0, sizeof(struct sctp_sndrcvinfo));

   ret = sctp_recvmsg(sockFd->fd, (Void *)recvbuf, (size_t)buflen, 
                      (struct sockaddr*)&addr, &addrlen, &info, 
                      (int*)&msgFlags);
   if (ret == INET_ERR)
   {
      /* cleanup */
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,meminfo->region, meminfo->pool, recvbuf, buflen);       
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_54: CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSctpRecvMsg() Failed : error(%d),"
            " sockFd->fd(%ld)\n", INET_ERR_CODE, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET014, 0, prntBuf);
#else
      DU_LOG("\ncmInetSctpRecvMsg() Failed : error(%d), sockFd->fd(%d)", \
         INET_ERR_CODE, sockFd->fd);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */

      return RFAILED;
   }

   /* save the length of the received message */
   /* cm_inet_c_001.main_58 : Fix for klockwork issue */
   *len = (MsgLen)ret;

#ifdef IPV6_SUPPORTED 
   if (addr.ss_family == AF_INET6)
   {
       uint8_t ipv4Format[12] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xff,0xff};
       pAddr6 = (struct sockaddr_in6*)&addr;
       *port = CM_INET_NTOH_UINT16(pAddr6->sin6_port);

      if((cmMemcmp(ipv4Format, pAddr6->sin6_addr.s6_addr, 12)) == 0)
      {
         srcAddr->type          = CM_INET_IPV4ADDR_TYPE;
         memcpy(&srcAddr->u.ipv4NetAddr, ((pAddr6->sin6_addr.s6_addr) + 12), sizeof(uint32_t));
         srcAddr->u.ipv4NetAddr = CM_INET_HTON_uint32_t(srcAddr->u.ipv4NetAddr);
      }

      else
      {
         srcAddr->type = CM_INET_IPV6ADDR_TYPE;
         CM_INET_COPY_IPV6ADDR(&srcAddr->u.ipv6NetAddr, &pAddr6->sin6_addr.s6_addr); 
      }
   }
   else 
   {
      pAddr = (struct sockaddr_in*)&addr;
      *port = CM_INET_NTOH_UINT16(pAddr->sin_port);
      srcAddr->type          = CM_INET_IPV4ADDR_TYPE;
      srcAddr->u.ipv4NetAddr = CM_INET_NTOH_UINT32(pAddr->sin_addr.s_addr);
   }
#else 
   pAddr = (struct sockaddr_in*)&addr;
   *port = CM_INET_NTOH_UINT16(pAddr->sin_port);
   srcAddr->type          = CM_INET_IPV4ADDR_TYPE;
   srcAddr->u.ipv4NetAddr = CM_INET_NTOH_UINT32(pAddr->sin_addr.s_addr);
#endif /* IPV6_SUPPORTED */

   /* fill sndrcv info */
   sinfo->stream     = info.sinfo_stream;
   sinfo->ssn        = info.sinfo_ssn;
   sinfo->flags      = info.sinfo_flags;
   /*cm_inet_c_001.main_54: converting ppid to host*/
   sinfo->ppid       = CM_INET_NTOH_UINT32(info.sinfo_ppid);
   sinfo->context    = info.sinfo_context;
   sinfo->timetolive = info.sinfo_timetolive;
   sinfo->tsn        = info.sinfo_tsn;
   sinfo->cumtsn     = info.sinfo_cumtsn;
   sinfo->assocId    = info.sinfo_assoc_id;

   /* fill message flags */
   *flag = 0;
   if ((msgFlags & MSG_EOR) != 0)
      *flag |= CM_INET_SCTP_MSG_EOR;

   if ((msgFlags & MSG_NOTIFICATION) != 0)
   {
      *flag |= CM_INET_SCTP_MSG_NOTIFICATION;
      *mBuf = NULLP;

      sctpNtfy = (union sctp_notification*)recvbuf;

      ntfy->header.nFlags = sctpNtfy->sn_header.sn_flags;
      ntfy->header.nLen   = sctpNtfy->sn_header.sn_length;

      switch(sctpNtfy->sn_header.sn_type)
      {
         case SCTP_ASSOC_CHANGE:
            ntfy->header.nType  = CM_INET_SCTP_ASSOC_CHANGE;
            switch(sctpNtfy->sn_assoc_change.sac_state)
            {
                case SCTP_COMM_UP:
                     ntfy->u.assocChange.state = CM_INET_SCTP_COMM_UP;
                     break;
                case SCTP_COMM_LOST:
                     ntfy->u.assocChange.state = CM_INET_SCTP_COMM_LOST;
                     break;
                case SCTP_RESTART:
                     ntfy->u.assocChange.state = CM_INET_SCTP_RESTART;
                     break;
                case SCTP_SHUTDOWN_COMP:
                     ntfy->u.assocChange.state = CM_INET_SCTP_SHUTDOWN_COMP;
                     break;
                case SCTP_CANT_STR_ASSOC:
                     ntfy->u.assocChange.state = CM_INET_SCTP_CANT_STR_ASSOC;
                     break;
                default:
                     break;
            }
            ntfy->u.assocChange.error      = sctpNtfy->sn_assoc_change.sac_error;
            ntfy->u.assocChange.outStreams = sctpNtfy->sn_assoc_change.sac_outbound_streams;
            ntfy->u.assocChange.inStreams  = sctpNtfy->sn_assoc_change.sac_inbound_streams;
            ntfy->u.assocChange.assocId    = sctpNtfy->sn_assoc_change.sac_assoc_id;
#ifdef SS_LINUX
            ntfy->u.assocChange.info       = sctpNtfy->sn_assoc_change.sac_info;
#endif
            break;
         case SCTP_PEER_ADDR_CHANGE:
            ntfy->header.nType  = CM_INET_SCTP_PEER_ADDR_CHANGE;
            switch(sctpNtfy->sn_paddr_change.spc_state)
            {
                case SCTP_ADDR_AVAILABLE:
                     ntfy->u.paddrChange.state = CM_INET_SCTP_ADDR_AVAILABLE;
                     break;
                case SCTP_ADDR_UNREACHABLE:
                     ntfy->u.paddrChange.state = CM_INET_SCTP_ADDR_UNREACHABLE;
                     break;
                case SCTP_ADDR_REMOVED:
                     ntfy->u.paddrChange.state = CM_INET_SCTP_ADDR_REMOVED;
                     break;
                case SCTP_ADDR_ADDED:
                     ntfy->u.paddrChange.state = CM_INET_SCTP_ADDR_ADDED;
                     break;
                case SCTP_ADDR_MADE_PRIM:
                     ntfy->u.paddrChange.state = CM_INET_SCTP_ADDR_MADE_PRIM;
                     break;
#ifdef SS_LINUX
                case SCTP_ADDR_CONFIRMED:
                     ntfy->u.paddrChange.state = CM_INET_SCTP_ADDR_CONFIRMED;
                     break;
#endif
                default:
                break;
            }

#ifdef IPV6_SUPPORTED 
            if (sctpNtfy->sn_paddr_change.spc_aaddr.ss_family == AF_INET6)
            {
               pAddr6 = (struct sockaddr_in6*)&(sctpNtfy->sn_paddr_change.spc_aaddr);
               ntfy->u.paddrChange.addr.type = CM_INET_IPV6ADDR_TYPE;
               CM_INET_COPY_IPV6ADDR(&ntfy->u.paddrChange.addr.u.ipv6NetAddr, 
                                                    &pAddr6->sin6_addr.s6_addr); 
            }
            else 
            {
               pAddr = (struct sockaddr_in*)&(sctpNtfy->sn_paddr_change.spc_aaddr);
               ntfy->u.paddrChange.addr.type = CM_INET_IPV4ADDR_TYPE;
               ntfy->u.paddrChange.addr.u.ipv4NetAddr = CM_INET_NTOH_UINT32(pAddr->sin_addr.s_addr);
            }
#else 
            pAddr = (struct sockaddr_in*)&(sctpNtfy->sn_paddr_change.spc_aaddr);
            ntfy->u.paddrChange.addr.type = CM_INET_IPV4ADDR_TYPE;
            ntfy->u.paddrChange.addr.u.ipv4NetAddr = CM_INET_NTOH_UINT32(pAddr->sin_addr.s_addr);
#endif /* IPV6_SUPPORTED */

            ntfy->u.paddrChange.error   = sctpNtfy->sn_paddr_change.spc_error;
            ntfy->u.paddrChange.assocId = sctpNtfy->sn_paddr_change.spc_assoc_id;
            break;
         case SCTP_REMOTE_ERROR:
            ntfy->header.nType  = CM_INET_SCTP_REMOTE_ERROR;

            ntfy->u.remoteErr.error   = sctpNtfy->sn_remote_error.sre_error;
            ntfy->u.remoteErr.assocId = sctpNtfy->sn_remote_error.sre_assoc_id;
#ifdef SS_LINUX
            /* cm_inet_c_001.main_46: Allocate memory for data before copying */
            datlen = cmStrlen(sctpNtfy->sn_remote_error.sre_data) + 1;

            ret = SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__, meminfo->region, meminfo->pool, \
                  &ntfy->u.remoteErr.data, datlen );
            if( ret != ROK )
            {
               ntfy->u.remoteErr.data = NULLP;
               break;
            }
            memcpy(ntfy->u.remoteErr.data,\
                  sctpNtfy->sn_remote_error.sre_data, datlen);
#endif
            break;
         case SCTP_SEND_FAILED:
            ntfy->header.nType = CM_INET_SCTP_SEND_FAILED;

            ntfy->u.sndFailed.error           = sctpNtfy->sn_send_failed.ssf_error;
            ntfy->u.sndFailed.assocId         = sctpNtfy->sn_send_failed.ssf_assoc_id;
#ifdef SS_LINUX
            /* cm_inet_c_001.main_46: Allocate memory for data before copying */
            datlen = cmStrlen(sctpNtfy->sn_send_failed.ssf_data) + 1;

            ret = SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__, meminfo->region, meminfo->pool, \
                  &ntfy->u.sndFailed.data, datlen );
            if( ret != ROK )
            {
               ntfy->u.sndFailed.data = NULLP;
               break;
            }
            memcpy(ntfy->u.sndFailed.data,\
                  sctpNtfy->sn_send_failed.ssf_data, datlen );
#endif
            ntfy->u.sndFailed.info.stream     = sctpNtfy->sn_send_failed.ssf_info.sinfo_stream;
            ntfy->u.sndFailed.info.ssn        = sctpNtfy->sn_send_failed.ssf_info.sinfo_ssn;
            ntfy->u.sndFailed.info.flags      = sctpNtfy->sn_send_failed.ssf_info.sinfo_flags;
            ntfy->u.sndFailed.info.ppid       = sctpNtfy->sn_send_failed.ssf_info.sinfo_ppid;
            ntfy->u.sndFailed.info.context    = sctpNtfy->sn_send_failed.ssf_info.sinfo_context;
            ntfy->u.sndFailed.info.timetolive = sctpNtfy->sn_send_failed.ssf_info.sinfo_timetolive;
            ntfy->u.sndFailed.info.tsn        = sctpNtfy->sn_send_failed.ssf_info.sinfo_tsn;
            ntfy->u.sndFailed.info.cumtsn     = sctpNtfy->sn_send_failed.ssf_info.sinfo_cumtsn;
            ntfy->u.sndFailed.info.assocId    = sctpNtfy->sn_send_failed.ssf_info.sinfo_assoc_id;
            break;
         case SCTP_SHUTDOWN_EVENT:
            ntfy->header.nType  = CM_INET_SCTP_SHUTDOWN_EVENT;

            ntfy->u.shutdownEvt.assocId = sctpNtfy->sn_shutdown_event.sse_assoc_id;
            break;
#ifdef SUN_KSCTP
         case SCTP_ADAPTION_INDICATION :
#else
            /* linux */
         case SCTP_ADAPTATION_INDICATION :
#endif
            ntfy->header.nType  = CM_INET_SCTP_ADAPTATION_INDICATION;

#ifdef SUN_KSCTP
            ntfy->u.adaptationEvt.adaptationInd = sctpNtfy->sn_adaption_event.sai_adaption_ind;
            ntfy->u.adaptationEvt.assocId       = sctpNtfy->sn_adaption_event.sai_assoc_id;
#else
            /* linux */
            ntfy->u.adaptationEvt.adaptationInd = sctpNtfy->sn_adaptation_event.sai_adaptation_ind;
            ntfy->u.adaptationEvt.assocId       = sctpNtfy->sn_adaptation_event.sai_assoc_id;
#endif
            break;
         case SCTP_PARTIAL_DELIVERY_EVENT:
            ntfy->header.nType  = CM_INET_SCTP_PARTIAL_DELIVERY_EVENT;

            ntfy->u.pdapiEvt.indication = sctpNtfy->sn_pdapi_event.pdapi_indication;
            ntfy->u.pdapiEvt.assocId    = sctpNtfy->sn_pdapi_event.pdapi_assoc_id;
            break;
         default:
            break;
      }
   }
   else
   {
      /* get a message buffer */
      ret = SGetMsg(meminfo->region, meminfo->pool, mBuf);
      if (ret != ROK)
      {
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,meminfo->region, meminfo->pool, recvbuf, buflen);
         return RFAILED;
      }

      ret = SAddPstMsgMult(recvbuf, *len, *mBuf);
      if (ret != ROK)
      {
         SPutMsg(*mBuf);
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,meminfo->region, meminfo->pool, recvbuf, buflen);
         return RFAILED;
      }
   }

   /* cleanup */
   SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,meminfo->region, meminfo->pool, recvbuf, buflen);       

   return ROK;
}

/*
*
*      Fun:   cmInetSctpGetPAddrs 
*
*      Desc:  returns the list of peer addresses 
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: None.
*
*      File:  cm_inet.c
*
*/
S16 cmInetSctpGetPAddrs
(
CmInetFd             *sockFd,       /* socket file descriptor */ 
uint32_t             assocId,      /* association id */
CmInetNetAddrLst     *addrlst       /* peer address list */
)
{
   /* cm_inet_c_001.main_58 : Fix for Klockwork issue */
   S32   cnt;
   S32   idx;
   uint8_t    *byteAddress;   
   struct sockaddr    *peerAddrList;
   struct sockaddr_in *pAddr;
#ifdef IPV6_SUPPORTED
   struct sockaddr_in6 *pAddr6;
#endif /* IPV6_SUPPORTED */

#ifdef SUN_KSCTP
   if((cnt = sctp_getpaddrs(sockFd->fd, assocId, (Void**)&peerAddrList)) == -1)
#else
      if((cnt = sctp_getpaddrs(sockFd->fd, assocId, &peerAddrList)) == -1)
#endif
      {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
         /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSctpGetPAddrs() Failed : error(%d),"
               " sockFd->fd(%ld), assocId(%ld)\n", 
               INET_ERR_CODE, sockFd->fd, assocId);
         CMINETLOGERROR(ERRCLS_DEBUG, ECMINET015, 0, prntBuf);
#else
      /* cm_inet_c_001.main_55: Fix for compilation warning */
      /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSctpGetPAddrs() Failed : error(%d),"
               " sockFd->fd(%d),assocId(%d)\n", 
               INET_ERR_CODE, sockFd->fd, assocId);
         CMINETLOGERROR(ERRCLS_DEBUG, ECMINET015, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */

         return RFAILED;
      }

   byteAddress = (uint8_t*)peerAddrList;
   for (idx = 0; idx < cnt; idx++)
   {
#ifdef IPV6_SUPPORTED 

      if (((struct sockaddr*)byteAddress)->sa_family == AF_INET6)
      {
         if (sockFd->protType == AF_INET)
         {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
            /* cm_inet_c_001.main_54: CMINETLOGERROR added insted of SDisp */
            sprintf(prntBuf, "cmInetSctpGetPAddrs() Failed : Invalid address"
                  " sockFd->fd(%ld)", sockFd->fd);
            CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET016, 0, prntBuf);
#else
            sprintf(prntBuf, "cmInetSctpGetPAddrs() Failed : Invalid address"
                  " sockFd->fd(%d)", sockFd->fd);
            CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET016, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */

            sctp_freepaddrs(peerAddrList);
            return RFAILED;
         }

         pAddr6 = (struct sockaddr_in6*)byteAddress;

         addrlst->addrs[idx].type = CM_INET_IPV6ADDR_TYPE;
         CM_INET_COPY_IPV6ADDR(&(addrlst->addrs[idx].u.ipv6NetAddr), &(pAddr6->sin6_addr.s6_addr));
         byteAddress += sizeof(struct sockaddr_in6);
      }
      else 
      {
         pAddr = (struct sockaddr_in*)byteAddress;
         addrlst->addrs[idx].type          = CM_INET_IPV4ADDR_TYPE;
         addrlst->addrs[idx].u.ipv4NetAddr = CM_INET_NTOH_UINT32(pAddr->sin_addr.s_addr);
         byteAddress += sizeof(struct sockaddr_in);
      }
#else 
      pAddr = (struct sockaddr_in*)byteAddress;
      addrlst->addrs[idx].type          = CM_INET_IPV4ADDR_TYPE;
      addrlst->addrs[idx].u.ipv4NetAddr = CM_INET_NTOH_UINT32(pAddr->sin_addr.s_addr);
      byteAddress += sizeof(struct sockaddr_in);
#endif /* IPV6_SUPPORTED */
   }

   /* cm_inet_c_001.main_58 : Fix for klockwork issue */
   addrlst->count = (uint8_t)cnt;   

   sctp_freepaddrs(peerAddrList);

   return ROK;
}

/*
*
*      Fun:    cmInetGetOpt 
*
*      Desc:   invokes socket API to retrive specified socket options
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes:
*
*      File:   cm_inet.c
*
*/
S16 cmInetGetOpt
(
CmInetFd *sockFd,               /* socket file descriptor */ 
uint32_t level,                /* option level */
uint32_t type,                 /* option type */
Ptr      value                 /* option value */ 
) 
{
   socklen_t                len;
   struct sctp_status       status;
   struct sctp_paddrinfo    addrInfo;
   struct sockaddr_in      *pAddr;
#ifdef IPV6_SUPPORTED
   struct sockaddr_in6     *pAddr6;
#endif /* IPV6_SUPPORTED */
   struct sctp_assocparams  assocParams;
/*cm_inet_c_001.main_40 Updated for the support of configurable RTO parameters, 
                        HBeat value Max retransmissions (Init, Path, Association)*/
   struct sctp_initmsg      initMsg;
   struct sctp_rtoinfo      rtoInfo;
   struct sctp_paddrparams  addrParams;
   CmInetSctpStatus        *pSctpStatus;
   CmInetSctpPeerAddrInfo  *pPeerAddrInfo;
   CmInetSctpInitMsg       *pInitMsg;
   CmInetSctpAssocParams   *pAssocParams;
   CmInetSctpRtoInfo       *pRtoInfo;
   CmInetSctpPeerAddrParams *pPeerAddrParams;
   /*cm_inet_c_001.main_58  : fix for klockwork issue */
   S32                     ret;


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd))
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   switch (type) 
   {
      case CM_INET_OPT_SCTP_GET_ASSOC_STA:
         pSctpStatus = (CmInetSctpStatus*)value;
         memset(&status, 0, sizeof(struct sctp_status));
         len = sizeof(status);
         status.sstat_assoc_id = pSctpStatus->assocId;

         ret = getsockopt(sockFd->fd, level, SCTP_STATUS, &status, &len);

         pSctpStatus->rwnd      = status.sstat_rwnd;
         pSctpStatus->unackdata = status.sstat_unackdata;
         pSctpStatus->penddata  = status.sstat_penddata;
         pSctpStatus->instrms   = status.sstat_instrms;
         pSctpStatus->outstrms  = status.sstat_outstrms;
         pSctpStatus->fragPoint = status.sstat_fragmentation_point; 

         switch (status.sstat_state)
         {
#ifdef SUN_KSCTP
            case SCTPS_IDLE:
            case SCTPS_BOUND:
            case SCTPS_LISTEN:
#else
            case SCTP_EMPTY:
#endif
               {
                  pSctpStatus->state = CM_INET_SCTP_STA_EMPTY;
               }
               break;

#ifdef SS_LINUX
            case SCTP_CLOSED:
               {
                  pSctpStatus->state = CM_INET_SCTP_STA_CLOSED;
               }
               break;
#endif

#ifdef SUN_KSCTP
            case SCTPS_COOKIE_WAIT:
#else
            case SCTP_COOKIE_WAIT:
#endif
               {
                  pSctpStatus->state = CM_INET_SCTP_STA_COOKIE_WAIT;
               }
               break;

#ifdef SUN_KSCTP
            case SCTPS_COOKIE_ECHOED:
#else
            case SCTP_COOKIE_ECHOED:
#endif
               {
                  pSctpStatus->state = CM_INET_SCTP_STA_COOKIE_ECHOED;
               }
               break;

#ifdef SUN_KSCTP
            case SCTPS_ESTABLISHED:
#else
            case SCTP_ESTABLISHED:
#endif
               {
                  pSctpStatus->state = CM_INET_SCTP_STA_ESTABLISHED;
               }
               break;

#ifdef SUN_KSCTP
            case SCTPS_SHUTDOWN_PENDING:
#else
            case SCTP_SHUTDOWN_PENDING:
#endif
               {
                  pSctpStatus->state = CM_INET_SCTP_STA_SHUTDOWN_PENDING;
               }
               break;

#ifdef SUN_KSCTP
            case SCTPS_SHUTDOWN_SENT:
#else
            case SCTP_SHUTDOWN_SENT:
#endif
               {
                  pSctpStatus->state = CM_INET_SCTP_STA_SHUTDOWN_SENT;
               }
               break;

#ifdef SUN_KSCTP
            case SCTPS_SHUTDOWN_RECEIVED:
#else
            case SCTP_SHUTDOWN_RECEIVED:
#endif
               {
                  pSctpStatus->state = CM_INET_SCTP_STA_SHUTDOWN_RECEIVED;
               }
               break;

#ifdef SUN_KSCTP
            case SCTPS_SHUTDOWN_ACK_SENT:
#else
            case SCTP_SHUTDOWN_ACK_SENT:
#endif
               {
                  pSctpStatus->state = CM_INET_SCTP_STA_SHUTDOWN_ACK_SENT;
               }
               break;

            default:
               {
                  return RFAILED;
               }
               break;
         }

#ifdef IPV6_SUPPORTED 
         if (status.sstat_primary.spinfo_address.ss_family == AF_INET6)
         {
            pAddr6 = (struct sockaddr_in6*)&(status.sstat_primary.spinfo_address);
            pSctpStatus->primary.port = CM_INET_NTOH_UINT16(pAddr6->sin6_port);

            pSctpStatus->primary.addr.type = CM_INET_IPV6ADDR_TYPE;
            CM_INET_COPY_IPV6ADDR(&pSctpStatus->primary.addr.u.ipv6NetAddr, 
                  &pAddr6->sin6_addr.s6_addr); 
         }
         else 
         {
            pAddr = (struct sockaddr_in*)&(status.sstat_primary.spinfo_address);
            pSctpStatus->primary.port = CM_INET_NTOH_UINT16(pAddr->sin_port);
            pSctpStatus->primary.addr.type          = CM_INET_IPV4ADDR_TYPE;
            pSctpStatus->primary.addr.u.ipv4NetAddr = CM_INET_NTOH_UINT32(pAddr->sin_addr.s_addr);
         }
#else 
         pAddr = (struct sockaddr_in*)&(status.sstat_primary.spinfo_address);
         pSctpStatus->primary.port = CM_INET_NTOH_UINT16(pAddr->sin_port);
         pSctpStatus->primary.addr.type          = CM_INET_IPV4ADDR_TYPE;
         pSctpStatus->primary.addr.u.ipv4NetAddr = CM_INET_NTOH_UINT32(pAddr->sin_addr.s_addr);
#endif /* IPV6_SUPPORTED */

         pSctpStatus->primary.assocId = status.sstat_primary.spinfo_assoc_id;
         if (status.sstat_primary.spinfo_state == SCTP_ACTIVE)
             pSctpStatus->primary.isActive = TRUE;
         else
			{
             pSctpStatus->primary.isActive = FALSE;
             pSctpStatus->primary.cwnd = status.sstat_primary.spinfo_cwnd;
             pSctpStatus->primary.srtt = status.sstat_primary.spinfo_srtt;
             pSctpStatus->primary.rto  = status.sstat_primary.spinfo_rto;
             pSctpStatus->primary.mtu  = status.sstat_primary.spinfo_mtu;
         }
			break;

      case CM_INET_OPT_SCTP_GET_PADDR_INFO:
         pPeerAddrInfo = (CmInetSctpPeerAddrInfo*)value;
         memset(&addrInfo, 0, sizeof(struct sctp_paddrinfo));
         len = sizeof(addrInfo);
         addrInfo.spinfo_assoc_id = pPeerAddrInfo->assocId;

#ifdef IPV6_SUPPORTED 
         if (pPeerAddrInfo->addr.type == CM_INET_IPV6ADDR_TYPE)
         {
            if (sockFd->protType == AF_INET)
            {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
               /* cm_inet_c_001.main_54: CMINETLOGERROR added insted of SDisp */
               /* cm_inet_c_001.main_62:Warning fix */
               snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetGetOpt() Failed : Invalid address,"
                     " sockFd->fd(%ld)\n", sockFd->fd);
               CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET017, 0, prntBuf);
#else
               /* cm_inet_c_001.main_62:Warning fix */
               snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetGetOpt() Failed : Invalid address,"
                     " sockFd->fd(%d)\n", sockFd->fd);
               CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET017, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
               return RFAILED;
            }

            pAddr6 = (struct sockaddr_in6*)&(addrInfo.spinfo_address);
            pAddr6->sin6_family      = AF_INET6;
            pAddr6->sin6_port        = CM_INET_HTON_UINT16(pPeerAddrInfo->port);
            CM_INET_COPY_IPV6ADDR(&pAddr6->sin6_addr.s6_addr, &pPeerAddrInfo->addr.u.ipv6NetAddr); 
         }
         else 
         {
            pAddr = (struct sockaddr_in*)&(addrInfo.spinfo_address);
            pAddr->sin_family      = AF_INET;
            pAddr->sin_port        = CM_INET_HTON_UINT16(pPeerAddrInfo->port);
            pAddr->sin_addr.s_addr = CM_INET_HTON_UINT32(pPeerAddrInfo->addr.u.ipv4NetAddr);
         }
#else 
         pAddr = (struct sockaddr_in*)&(addrInfo.spinfo_address);
         pAddr->sin_family      = AF_INET;
         pAddr->sin_port        = CM_INET_HTON_UINT16(pPeerAddrInfo->port);
         pAddr->sin_addr.s_addr = CM_INET_HTON_UINT32(pPeerAddrInfo->addr.u.ipv4NetAddr); 
#endif /* IPV6_SUPPORTED */

         /* Not validating the address, whether Addr is a valid address or not */

         ret = getsockopt(sockFd->fd, level, SCTP_GET_PEER_ADDR_INFO, &addrInfo, &len);

         if (addrInfo.spinfo_state == SCTP_ACTIVE)
            pPeerAddrInfo->isActive = TRUE;
         else
            pPeerAddrInfo->isActive = FALSE;
         pPeerAddrInfo->cwnd = addrInfo.spinfo_cwnd;
         pPeerAddrInfo->srtt = addrInfo.spinfo_srtt;
         pPeerAddrInfo->rto  = addrInfo.spinfo_rto;
         pPeerAddrInfo->mtu  = addrInfo.spinfo_mtu;
         break;

      case CM_INET_OPT_SCTP_PEERADDR_PARAMS:

         pPeerAddrParams = (CmInetSctpPeerAddrParams *)value;

         memset(&addrParams, 0, sizeof(struct sctp_paddrparams));

         addrParams.spp_assoc_id = pPeerAddrParams->assocId;

         if (pPeerAddrParams->s.addrPres == TRUE)
         {
#ifdef IPV6_SUPPORTED 
            if (pPeerAddrParams->s.addr.type == CM_INET_IPV6ADDR_TYPE)
            {
               if (sockFd->protType == AF_INET)
               {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
                  /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
                  /* cm_inet_c_001.main_62:Warning fix */
                  snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "invalid address line:sockFd->fd(%ld)\n",
                        sockFd->fd);
                  CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET066, 0, prntBuf);
#else
                  /* cm_inet_c_001.main_62:Warning fix */
                  snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "invalid address line:sockFd->fd(%d)\n",
                        sockFd->fd);
                  CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET066, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
                  return RFAILED;
               }

               pAddr6 = (struct sockaddr_in6*)&(addrParams.spp_address);
               pAddr6->sin6_family      = AF_INET6;
               pAddr6->sin6_port        = CM_INET_HTON_UINT16(pPeerAddrParams->s.port);
               CM_INET_COPY_IPV6ADDR(&pAddr6->sin6_addr.s6_addr, &pPeerAddrParams->s.addr.u.ipv6NetAddr); 
            }
            else 
            {
               pAddr = (struct sockaddr_in*)&(addrParams.spp_address);
               pAddr->sin_family      = AF_INET;
               pAddr->sin_port        = CM_INET_HTON_UINT16(pPeerAddrParams->s.port);
               pAddr->sin_addr.s_addr = CM_INET_HTON_UINT32(pPeerAddrParams->s.addr.u.ipv4NetAddr);
            }
#else 
            pAddr = (struct sockaddr_in*)&(addrParams.spp_address);
            pAddr->sin_family      = AF_INET;
            pAddr->sin_port        = CM_INET_HTON_UINT16(pPeerAddrParams->s.port);
            pAddr->sin_addr.s_addr = CM_INET_HTON_UINT32(pPeerAddrParams->s.addr.u.ipv4NetAddr);
#endif /* IPV6_SUPPORTED */
         }
         else
         {
#ifdef IPV6_SUPPORTED 
            if (sockFd->protType == AF_INET6)
               addrParams.spp_address.ss_family = AF_INET6;
            else
               addrParams.spp_address.ss_family = AF_INET;
#else
            addrParams.spp_address.ss_family = AF_INET;
#endif
         }

         len = sizeof(addrParams);

         ret = getsockopt(sockFd->fd, level, SCTP_PEER_ADDR_PARAMS, &addrParams, &len);
         /* cm_inet_c_001.main_41 : Fixed the Solaris compilation problem */
#ifndef SUN_KSCTP

         pPeerAddrParams->hbInterval    = addrParams.spp_hbinterval;
         pPeerAddrParams->pathMaxRxt    = addrParams.spp_pathmaxrxt;
         pPeerAddrParams->assocId       = addrParams.spp_assoc_id;
         pPeerAddrParams->pathMtu       = addrParams.spp_pathmtu;
         pPeerAddrParams->sackDelay     = addrParams.spp_sackdelay;

         if (addrParams.spp_flags & SPP_HB_ENABLE)
            pPeerAddrParams->hbEnblFlag    = CM_INET_OPT_ENABLE;
         else
            pPeerAddrParams->hbEnblFlag    = CM_INET_OPT_DISABLE;

         if (addrParams.spp_flags & SPP_PMTUD_ENABLE)
            pPeerAddrParams->pmtudFlag     = CM_INET_OPT_ENABLE;
         else
            pPeerAddrParams->pmtudFlag     = CM_INET_OPT_DISABLE;

         if (addrParams.spp_flags & SPP_SACKDELAY_ENABLE)
            pPeerAddrParams->sackDelayFlag = CM_INET_OPT_ENABLE;
         else
            pPeerAddrParams->sackDelayFlag = CM_INET_OPT_DISABLE;
#endif 

         break;

      case CM_INET_OPT_SCTP_ASSOC_PARAMS:

         pAssocParams = (CmInetSctpAssocParams *)value;

         memset(&assocParams, 0, sizeof(struct sctp_assocparams));

         assocParams.sasoc_assoc_id = pAssocParams->assocId;

         len = sizeof(assocParams);

         ret = getsockopt(sockFd->fd, level, SCTP_ASSOCINFO, &assocParams, &len);

         pAssocParams->assocMaxReTx      = assocParams.sasoc_asocmaxrxt;
         pAssocParams->cookieLife        = assocParams.sasoc_cookie_life;
         pAssocParams->assocId           = assocParams.sasoc_assoc_id;
         pAssocParams->numberOfPeerDest  = assocParams.sasoc_number_peer_destinations;
         pAssocParams->peerRwnd          = assocParams.sasoc_peer_rwnd;
         pAssocParams->localRwnd         = assocParams.sasoc_local_rwnd;

         break;

      case CM_INET_OPT_SCTP_RTO_INFO:

         pRtoInfo = (CmInetSctpRtoInfo *)value;

         memset(&rtoInfo, 0, sizeof(struct sctp_rtoinfo));

         len = sizeof(rtoInfo);

         ret = getsockopt(sockFd->fd, level, SCTP_RTOINFO, &rtoInfo, &len);

         pRtoInfo->assocId    = rtoInfo.srto_assoc_id;
         pRtoInfo->rtoInitial = rtoInfo.srto_initial;
         pRtoInfo->rtoMax     = rtoInfo.srto_max;
         pRtoInfo->rtoMin     = rtoInfo.srto_min;

         break;

      case CM_INET_OPT_SCTP_INIT_MSG:

         pInitMsg = (CmInetSctpInitMsg *)value;

         memset(&initMsg, 0, sizeof(struct sctp_initmsg));

         len = sizeof(initMsg);

         ret = getsockopt(sockFd->fd, level, SCTP_INITMSG, &initMsg, &len);

         pInitMsg->maxInitReTx    = initMsg.sinit_max_attempts;
         pInitMsg->maxInitTimeout = initMsg.sinit_max_init_timeo;
         pInitMsg->numOstreams    = initMsg.sinit_num_ostreams;
         pInitMsg->maxInstreams   = initMsg.sinit_max_instreams;

         break;

      default:
         return RFAILED;
   }

   if (ret == INET_ERR)
   {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetGetOpt() failed on line:"
            " error(%d), sockFd->fd(%ld)\n", INET_ERR_CODE, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET067, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetGetOpt() failed on line:"
            " error(%d), sockFd->fd(%d)\n", INET_ERR_CODE, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET067, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
      return RFAILED;
   }          

   return ROK;
}

/* cm_inet_c_001.main_54: Added new function cmInetShutDownSctp()*/
/*
 *
 *      Fun:   cmInetShutDownSctp 
 *
 *      Desc:  Shutdown the SCTP association gracefully.
 *
 *      Ret:   ROK     - successful
 *             RFAILED - failed
 *
 *      Notes: None.
 *
 *      File:  cm_inet.c
 *
 */
S16 cmInetShutDownSctp
(
CmInetFd *sockFd       /* socket file descriptor */ 
)
{
   /*cm_inet_c_001.main_58  : fix for klockwork issue */
   S32                    ret;
   struct sctp_sndrcvinfo sndRcvInfo;


   memset(&sndRcvInfo, 0, sizeof(sndRcvInfo));

#ifdef SUN_KSCTP
   sndRcvInfo.sinfo_flags = MSG_EOF;
#else
   sndRcvInfo.sinfo_flags = SCTP_EOF;
#endif

   /* Call the sctp_send with flag set to termiante the association */

   ret = sctp_send(sockFd->fd, NULLP, 0, &sndRcvInfo, sndRcvInfo.sinfo_flags);

   if(ret == INET_ERR)
   {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetShutDownSctp() Failed : error(%d), sockFd->fd(%ld)\n",
            INET_ERR_CODE, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET018, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetShutDownSctp() Failed : error(%d), sockFd->fd(%d)\n",
            INET_ERR_CODE, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET018, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */

      return RFAILED;
   }

   return ROK;
}

/* cm_inet_c_001.main_61: Added new function cmInetAbortSctpAssoc()*/
/*
 *
 *      Fun:   cmInetAbortSctpAssoc
 *
 *      Desc:  ABORT the association.
 *
 *      Ret:   ROK     - successful
 *             RFAILED - failed
 *
 *      Notes: None.
 *
 *      File:  cm_inet.c
 *
 */
S16 cmInetAbortSctpAssoc
(
CmInetFd          *sockFd,       /* socket file descriptor */
UConnId           assocId          /* Association ID */
)
{
   S32    ret;
   struct sctp_sndrcvinfo sndRcvInfo;


   memset(&sndRcvInfo, 0, sizeof(sndRcvInfo));

#ifdef SUN_KSCTP
   sndRcvInfo.sinfo_flags = MSG_ABORT;
#else
   sndRcvInfo.sinfo_flags = SCTP_ABORT;
#endif

   sndRcvInfo.sinfo_assoc_id = assocId;

   /* Call the sctp_send with flag set to termiante the association */

   ret = sctp_send(sockFd->fd, NULLP, 0, &sndRcvInfo, sndRcvInfo.sinfo_flags);

   if(ret == INET_ERR)
   {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetAbortSctpAssoc() Failed : error(%d), sockFd->fd(%ld)\n",
            INET_ERR_CODE, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET018, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetAbortSctpAssoc() Failed : error(%d), sockFd->fd(%d)\n",
            INET_ERR_CODE, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET018, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */

      return RFAILED;
   }

   return ROK;
}

#endif


/*
 *
 *      Fun:   cmInetConnect
 *
 *      Desc:  Establishs a connection to a foreign address (TCP) or associates
 *             a UDP socket to a foreign address.
 *
 *      Ret:   ROK         - successful
*             ROKDNA      - resource temporarily unavaiable
*             RINPROGRESS - connection is in progress (only non-blocking)
*             RISCONN     - connection is established (only non-blocking)
*             RFAILED     - failed
*
*      Notes: None.
*
*      File:  cm_inet.c
*
*/

S16 cmInetConnect
(
CmInetFd   *sockFd,             /* socket file descriptor */
CmInetAddr *servAddr            /* foreign Internet address/port */  
)
{
   S32 ret;                     /* temporary return value */
   struct sockaddr_in dstAddr;  /* foreign Internet address/port */
#ifdef IPV6_SUPPORTED 
   struct sockaddr_in6 dstAddr6; /* foreign Internet IPV6 address/port */
#ifdef CMINETDBG
   uint16_t            port;
#endif /* CMINETDBG */
#endif /* IPV6_SUPPORTED */
   S32    sizeOfAddr;
   CmInetSockAddr *sockAddrPtr;  


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd) ||
         (servAddr == NULLP))
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

#ifdef IPV6_SUPPORTED 
   if (servAddr->type == CM_INET_IPV6ADDR_TYPE)
   {
      memset(&dstAddr6, 0, sizeof(dstAddr6));
      dstAddr6.sin6_family     = AF_INET6;
      dstAddr6.sin6_port       = CM_INET_HTON_UINT16(servAddr->u.ipv6Addr.port);
      CM_INET_COPY_IPV6ADDR(&dstAddr6.sin6_addr, 
                            &servAddr->u.ipv6Addr.ipv6NetAddr);
      sizeOfAddr              = sizeof(struct sockaddr_in6);
      sockAddrPtr             = (CmInetSockAddr *)&dstAddr6;
   }
   else
   {
      memset(&dstAddr, 0, sizeof(dstAddr));
      dstAddr.sin_family      = AF_INET;
      dstAddr.sin_port        = CM_INET_HTON_UINT16(servAddr->u.ipv4Addr.port);
      dstAddr.sin_addr.s_addr = CM_INET_HTON_UINT32(servAddr->u.ipv4Addr.address);
      sizeOfAddr              = sizeof(struct sockaddr_in);
      sockAddrPtr             = (CmInetSockAddr *)&dstAddr;
   }
#else
   memset(&dstAddr, 0, sizeof(dstAddr));
   dstAddr.sin_family      = AF_INET;
   dstAddr.sin_port        = CM_INET_HTON_UINT16(servAddr->port);
   dstAddr.sin_addr.s_addr = CM_INET_HTON_UINT32(servAddr->address);
   sizeOfAddr              = sizeof(struct sockaddr_in);
   sockAddrPtr             = (CmInetSockAddr *)&dstAddr;
#endif /* IPV6_SUPPORTED */

   ret = connect(sockFd->fd, sockAddrPtr, sizeOfAddr);
   if (ret == INET_ERR)
   {
      switch (INET_ERR_CODE)
      {
         /* non-blocking: connection is in progress */
         case ERR_INPROGRESS:
            return (RINPROGRESS);
            break;   

            /* 
             * non-blocking: connection is established 
             * blocking    : connection is already established
             */
         case ERR_ISCONN:
            return (RISCONN);
            break;               

            /* resource temporarily unavailable */
         case ERR_WOULDBLOCK:
            return (ROKDNA);
            break;

            /* non-blocking: connection is in progress */
         case ERR_ALREADY:
            return (RINPROGRESS);
            break;

         case ERR_INVAL:
            return (RINPROGRESS);
            break;

            /*  Check for connection refused and timeout errors */
         case ERR_CONNREFUSED:
         case ERR_TIMEDOUT:
            return (RCLOSED);
            break;

            /* it is a real error */ 
         default:
#ifdef CMINETDBG
#ifdef IPV6_SUPPORTED 
            if (servAddr->type == CM_INET_IPV6ADDR_TYPE)
               port = servAddr->u.ipv6Addr.port;
            else
               port = servAddr->u.ipv4Addr.port;

            /* cm_inet_c_001.main_54: CMINETLOGERROR added insted of SDisp */
#ifndef ALIGN_64BIT
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetConnect() Failed : error(%d),"
                  " addrtype(0x%x), port(0x%1x), sockFd->fd(%ld)\n", 
                  INET_ERR_CODE, servAddr->type, port, sockFd->fd);
            CMINETLOGERROR(ERRCLS_ADD_RES, ECMINET019, 0, prntBuf);
#else
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetConnect() Failed : error(%d),"
                  " addrtype(0x%x), port(0x%1x), sockFd->fd(%d)\n", 
                  INET_ERR_CODE, servAddr->type, port, sockFd->fd);
            CMINETLOGERROR(ERRCLS_ADD_RES, ECMINET019, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#else
#ifndef ALIGN_64BIT
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetConnect() Failed : error(%d), addr(0x%lx),"
                  "port(0x%1x), sockFd->fd(%ld)\n", INET_ERR_CODE , 
                  servAddr->address, servAddr->port, sockFd->fd);
            CMINETLOGERROR(ERRCLS_ADD_RES, ECMINET020, 0, prntBuf);
#else
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetConnect() Failed : error(%d), addr(0x%x),"
                  "port(0x%x), sockFd->fd(%d)\n", INET_ERR_CODE , 
                  servAddr->address, servAddr->port, sockFd->fd);
            CMINETLOGERROR(ERRCLS_ADD_RES, ECMINET020, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* IPV6_SUPPORTED */
#endif /* CMINETDBG */
            return RFAILED;
            break;
      }
   }

   return ROK;
} /* end of cmInetConnect */


/*
*
*      Fun:   cmInetListen
*
*      Desc:  Indicates the willingness of a socket to listen for incomming 
*             connection requests.
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: The backLog value has to be within 0..5
*
*      File:  cm_inet.c
*
*/

S16 cmInetListen
(
CmInetFd *sockFd,               /* socket file descriptor */ 
S16       backLog               /* max. number of outstandig connections 0..5 */
)
{
   S32 ret;                     /* temporary return value */


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd) ||
         (backLog < MIN_BACK_LOG) || (backLog > MAX_BACK_LOG))
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   ret = listen(sockFd->fd, backLog);
   if (ret == INET_ERR) 
   {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetListen() Failed : error(%d), backLog(%d),"
            " sockFd->fd(%ld)\n", INET_ERR_CODE, backLog, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET021, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetListen() Failed : error(%d), backLog(%d),"
            " sockFd->fd(%d)\n", INET_ERR_CODE, backLog, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET021, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
      return RFAILED;
   }

   return ROK;
} /* end of cmInetListen */


/*
*
*      Fun:   cmInetAccept 
*
*      Desc:  Accepts an incoming connection.
*             On default the new socket is non-blocking. The options can be 
*             changed with the function cmInetSetOpt().
*
*      Ret:   ROK     - successful
*             ROKDNA  - there is no connection present to accept (only 
*                       non-blocking) 
*             RFAILED - failed
*
*      Notes: None.
*
*      File:  cm_inet.c
*
*/

S16 cmInetAccept
(
CmInetFd   *sockFd,     /* socket file descriptor */ 
CmInetAddr *fromAddr,   /* calling Internet address/port */
CmInetFd   *newSockFd   /* socket file descriptor for new connection*/
)
{
   S32 ret;                         /* temporary return value */
   S32 addrLen;                     /* address structure length */
   struct sockaddr_in  *peerAddr;   /* calling Internet address/port */
#ifdef IPV6_SUPPORTED 
   struct sockaddr_in6 *peerAddr6;  /* calling Internet address/port */
   struct sockaddr_in6 sockAddr;
#else
   CmInetSockAddr sockAddr;  
#endif /* IPV6_SUPPORTED */

   uint32_t optVal;

   /* added */

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd))
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   /* change CmInetSockAddr to sockAddr */
   addrLen = sizeof(sockAddr);   

   /* INSURE fix */
#if ( defined(SUNOS) || defined(SS_LINUX)) 
   newSockFd->fd = accept(sockFd->fd, (CmInetSockAddr*)&sockAddr, 
                          (socklen_t *)&addrLen);
#else
   newSockFd->fd = accept(sockFd->fd, (CmInetSockAddr*)&sockAddr, 
                          (int*)&addrLen);
#endif /* SUNOS || SS_LINUX */   

  /* cm_inet_c_001.main_58: Moved setting of protType below */

   if (CM_INET_INV_SOCK_FD(newSockFd))
   {
      if (INET_ERR_CODE == ERR_WOULDBLOCK)
      {
         /* no connection present to accept */ 
         return (ROKDNA);
      }
      else
      {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
         /* cm_inet_c_001.main_54: CMINETLOGERROR added insted of SDisp */
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetAccept() Failed : error(%d),"
              " sockFd->fd(%ld)\n", INET_ERR_CODE, sockFd->fd);
         CMINETLOGERROR(ERRCLS_DEBUG, ECMINET022, 0, prntBuf);
#else
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetAccept() Failed : error(%d),"
               " sockFd->fd(%d)\n", INET_ERR_CODE, sockFd->fd);
         CMINETLOGERROR(ERRCLS_DEBUG, ECMINET022, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
         return RFAILED;
      }
   }     


  /* cm_inet_c_001.main_58: Fix for g++ compilation warning */
   /* added for IPv6/IPv4 socket distinguishing */
#ifdef IPV6_SUPPORTED   
   if (addrLen == sizeof(struct sockaddr_in))
      newSockFd->protType = AF_INET;
   else if(addrLen == sizeof(struct sockaddr_in6))
      newSockFd->protType = AF_INET6;
   else
   {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetAccept() Failed : sockFd->fd(%ld)\n", sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET071, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetAccept() Failed : sockFd->fd(%d)\n", sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET071, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
         return RFAILED;
   }
#endif /* IPV6_SUPPORTED */      

   /* set the new socket file descriptor type */
   newSockFd->type = CM_INET_STREAM;

   /* set default options for new socket file descriptor */
   optVal = CM_INET_OPT_DISABLE;
   ret = cmInetSetOpt(newSockFd, SOL_SOCKET, CM_INET_OPT_BLOCK, &optVal); 
   if ( ret != ROK) 
   {
      ret = cmInetClose(newSockFd);
      return RFAILED;
   }

#ifdef IPV6_SUPPORTED
   memset(fromAddr, 0, sizeof(fromAddr));
   if (addrLen == sizeof(struct sockaddr_in))
   {
      peerAddr = (struct sockaddr_in *)&sockAddr;
      fromAddr->type = CM_INET_IPV4ADDR_TYPE;
      fromAddr->u.ipv4Addr.port    = CM_INET_NTOH_UINT16(peerAddr->sin_port);
      fromAddr->u.ipv4Addr.address = 
                              CM_INET_NTOH_UINT32(peerAddr->sin_addr.s_addr);
   }
   else if (addrLen == sizeof(struct sockaddr_in6))
   {
      peerAddr6 = (struct sockaddr_in6 *)&sockAddr;
      fromAddr->type = CM_INET_IPV6ADDR_TYPE;
      fromAddr->u.ipv6Addr.port    = CM_INET_NTOH_UINT16(peerAddr6->sin6_port);
      CM_INET_COPY_IPV6ADDR(&fromAddr->u.ipv6Addr.ipv6NetAddr, 
                            &peerAddr6->sin6_addr);
   }
#else
   peerAddr = (struct sockaddr_in *)&sockAddr;
   fromAddr->port    = CM_INET_NTOH_UINT16(peerAddr->sin_port);
   fromAddr->address = CM_INET_NTOH_UINT32(peerAddr->sin_addr.s_addr);
#endif /* IPV6_SUPPORTED */
   return ROK;
} /* end of cmInetAccept */ 


/*
*
*      Fun:   cmInet4FillTos 
*
*      Desc:  This function inserts tos (into ancillary data) which 
*             will be used to fill tos value in ip header in outgoing IP packet
*             when sending that packet using sendmsg()function.
*
*      Ret:   ROK   
*
*      Notes:  
*
*      File:  cm_inet.c
*
*/

static S16 cmInet4FillTos
(
uint8_t  tos,        /* tos value to be filled in ipheader */
uint8_t  *cmsgBuf,   /* flat buffer where to build ext hdrs */
uint32_t *curMsgIdx  /* idx in cmsgBuf where HBH/Dest ext hdr ends */
)
{
   struct cmsghdr *tempHdr;
   uint8_t     len;


      len = 0;


   /* cmsghdr struc will appear before data in the ancillary data object. 
    * So put cmsghdr struc in flat buffer first. */

   /* cmsghdr struc points to flat buffer's starting address */
   tempHdr = (struct cmsghdr *)cmsgBuf;  

   /* fill up level & type of cmsghdr structure */
   tempHdr->cmsg_level = IPPROTO_IPV6;
   tempHdr->cmsg_type = IP_TOS;
   (*(uint8_t*)CMSG_DATA(tempHdr)) = tos;
   len = CMSG_SPACE(sizeof tos); 


   /* fill up the length of cmsghdr structure */
   tempHdr->cmsg_len = len;  
   *curMsgIdx += len;

   return ROK;

}/* end of cmInet4FillTos */ 
/*
*
*      Fun:   cmInetSendDscpMsg
*
*      Desc:  Sends the message data hold by mBuf. 
*             The len paramter gives the actual written octets. If the socket
*             is non-blocking this value can be differ from the mBuf length 
*             because there was not enough transmit buffer space available. If 
*             this occurs, RWOULDBLOCK is returned and only a part of the mBuf
*             is sent.
*             Values for flag parameter:
*  
*             CM_INET_NO_FLAG - no additional control flag
*
*      Ret:   ROK         - successful
*             RWOULDBLOCK - no or not entire mBuf sent because would block
*             ROUTRES     - failed, out of resources
*             RCLOSED     - connection was closed by the peer
*             RFAILED     - failed
*                           
*      Notes: The successful completion of a send call does not indicate that 
*             the data has been successfully delivered! 
*
*             This function does not free any sent buffers.  
*
*   
*      File:  cm_inet.c
*
*/

S16 cmInetSendDscpMsg
(
CmInetFd       *sockFd,         /* socket file descriptor */
CmInetAddr     *dstAddr,        /* destination Internet address/port */
CmInetMemInfo  *info,           /* buffer allocation info */
Buffer         *mBuf,           /* buffer structure to send */
MsgLen         *len,            /* number of actually sent octets */
/* added for IPv6 ext hdr */
CmInetIpHdrParm *ipHdrParams,   /* IPv6 extensions headers */
S16             flags           /* additional control flags, unused */
)
{
#if (defined(WIN32) || defined(CMINETFLATBUF))
   S32     ret =0;                 /* temporary return value */
   MsgLen  msgLen =0;              /* message length */ 
   MsgLen  bufLen =0;              /* send buffer length */
   Data   *sendBuf =0;             /* plain send buffer */
#else
   S32 ret =0;                     /* temporary return value */
   S32 retVal =0;                  /* temporary return value */
   S16 i =0;                       /* loop index */
   CmInetIovec  txArr[CM_INET_MAX_DBUF] = {{0}}; /* scatter vector */
   S16      numDBufs =0;           /* number of dBufs in message */
   struct   msghdr msg ={0};         /* sendmsg() message header */
   MsgLen   msgLen =0;             /* message length */ 
   uint32_t      strtEndDBufNum =0;     /* starting/ending DBuf number */ 
   MsgLen   unSentLen =0;          /* sent len */
#ifdef IPV6_SUPPORTED 
   uint32_t    curMsgIdx =0;            /* indx in cmsgData where to write an ext hdr */
   /* added for IPv6 ext hdr */
#if (defined(SS_LINUX) || defined(_XPG4_2))
   /* alloc from stack for IPv6 ancill data */
   uint8_t     cmsgData[CM_INET_IPV6_ANCIL_DATA]= {0};
#endif /* SS_LINUX || _XPG4_2 */
#else
   uint32_t    curMsgIdx =0;            /* indx in cmsgData where to write an ext hdr */
#if (defined(SS_LINUX) || defined(_XPG4_2))
   /* alloc from stack for IPv4 ancill data */ 
    uint8_t     cmsgData[CM_INET_IPV4_ANCIL_DATA]={0};
#endif /* SS_LINUX || _XPG4_2 */
#endif /* IPV6_SUPPORTED */   
#endif /* WIN32 | CMINETFLATBUF */  

   struct  sockaddr_in remAddr ={0}; /* remote Internet address */   
#ifdef IPV6_SUPPORTED
   struct   sockaddr_in6  remAddr6 = {0}; /* remote Internet address */   
#endif /* IPV8_SUPPORTED */
   CmInetSockAddr *sockAddrPtr = NULLP;
   /* cm_inet_c_001.main_58 : Fix for klockwork issue */
   uint32_t            sizeOfAddr =0;    

   /* cm_inet_c_001.main_50 - Added for partial send handling */
   /* cm_inet_c_001.main_59: Protected under if not defined WIN32*/
#if (!defined(WIN32)) 
   MsgLen         ioLen =0; 
#endif


      UNUSED(flags);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd) ||
         (info == NULLP) || (len == NULLP))
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   /* added for IPv6 ext hdr */
#if !(defined(WIN32) || defined(CMINETFLATBUF))
#if (defined(SS_LINUX) || defined(_XPG4_2))
/*   memset(cmsgData, 0, sizeof(cmsgData));    */
#endif /* SS_LINUX || _XPG4_2 */
   curMsgIdx   = 0;
#endif /* WIN32 | CMINETFLATBUF */

   msgLen = 0;  /* need by CC to pass without warning */
   sockAddrPtr = NULLP;
   sizeOfAddr = 0;

   /* setup remote address */
   if (dstAddr != NULLP)
   {
#ifdef IPV6_SUPPORTED
      if (dstAddr->type == CM_INET_IPV6ADDR_TYPE)
      {
         memset(&remAddr6, 0, sizeof(remAddr6));
         remAddr6.sin6_family = AF_INET6;
         remAddr6.sin6_port   = CM_INET_HTON_UINT16(dstAddr->u.ipv6Addr.port);
         CM_INET_COPY_IPV6ADDR(&remAddr6.sin6_addr, 
               &dstAddr->u.ipv6Addr.ipv6NetAddr); 
         sizeOfAddr = sizeof(remAddr6);
         sockAddrPtr = (CmInetSockAddr *)&remAddr6;
      }
      else
      {
         memset(&remAddr, 0, sizeof(remAddr));
         remAddr.sin_family = AF_INET;
         remAddr.sin_port   = CM_INET_HTON_UINT16(dstAddr->u.ipv4Addr.port);
         remAddr.sin_addr.s_addr = 
            CM_INET_HTON_UINT32(dstAddr->u.ipv4Addr.address);
         sizeOfAddr = sizeof(remAddr);
         sockAddrPtr = (CmInetSockAddr *)&remAddr;
      }
#else
/*      memset(&remAddr, 0, sizeof(remAddr)); */
      remAddr.sin_family      = AF_INET;
      remAddr.sin_port        = CM_INET_HTON_UINT16(dstAddr->port);
      remAddr.sin_addr.s_addr = CM_INET_HTON_UINT32(dstAddr->address);
      sizeOfAddr = sizeof(remAddr);
      sockAddrPtr = (CmInetSockAddr *)&remAddr;
#endif /* IPV6_SUPPORTED */
   }

#if (defined(WIN32) || defined(CMINETFLATBUF))
   /* copy message to a flat buffer */
   ret = SFndLenMsg(mBuf, &bufLen);
   if (ret != ROK)
   {
      return RFAILED;
   }
   /* max message length is limited to control the memory usage */
   /* casting bufLen to avoid warnings */
   if ((bufLen > 0) && ((uint32_t)bufLen > CM_INET_MAX_MSG_LEN))
   {
      return RFAILED;
   }
   ret = SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, &sendBuf, bufLen);                  
   if (ret != ROK)
   {
      return (ROUTRES);
   }
   ret = SCpyMsgFix(mBuf, 0, bufLen, sendBuf, &msgLen);
   if ((ret != ROK) || (msgLen != bufLen)) 
   {
      /* cleanup */
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, sendBuf, bufLen);       
      return RFAILED;
   }

   if (dstAddr == NULLP)
   {
      /* VxWorks sendto has some problem
       * with connected UDP socket, use send */
#ifndef SS_VW
      ret = sendto(sockFd->fd, (S8 *)sendBuf, bufLen, 0, 
            NULLP, sizeOfAddr);
#else
      ret = send(sockFd->fd, (S8 *)sendBuf, bufLen, 0);
#endif /* end of SS_VW */
   }
   else
      /* cm_inet_c_001.main_54: Fix for vxworks 6.7 sending data on TCP sockets */
   {
#if (defined(SS_VW) && defined(SS_VW6_7)) 
      if ((sockFd->type  == CM_INET_STREAM) || (sockFd->type == SOCK_RDM) )
      {
         ret = send(sockFd->fd, (S8 *)sendBuf, bufLen, 0);
      }
      else
#endif /* end of SS_VW6_7 and SS_VW */
      {
         ret = sendto(sockFd->fd, (S8 *)sendBuf, bufLen, 0, 
               sockAddrPtr, sizeOfAddr); 
      }
   }
   if (ret == INET_ERR)
   {
      /* cleanup */
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, sendBuf, bufLen);      

      if(INET_ERR_CODE == ERR_AGAIN)
      {
         *len = 0;
         return (RWOULDBLOCK);
      }

      /* Check for ERR_WOULDBLOCK */
      if(INET_ERR_CODE == ERR_WOULDBLOCK)
      {
         *len = 0;
         return (RWOULDBLOCK);
      }

#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSendDscpMsg() Failed : error(%d), msgLen(%d),"
            " sockFd->fd(%ld)\n", INET_ERR_CODE, bufLen, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET023, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSendDscpMsg() Failed : error(%d), msgLen(%d),"
            " sockFd->fd(%d)\n", INET_ERR_CODE, bufLen, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET023, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */

      /* cm_inet_c_001.main_37 network unreacheble error is added */
      /* check if network is reacheble*/
      if ((INET_ERR_CODE == ERR_NETUNREACH))
      {
         return (RNETFAILED);
      }


      /*  Check if connection was closed */
      if ((INET_ERR_CODE == ERR_PIPE) ||
            (INET_ERR_CODE == ERR_CONNABORTED) || 
            (INET_ERR_CODE == ERR_CONNRESET))
      {
         *len = 0;
         return (RCLOSED);
      }

      return RFAILED;
   }

   *len = ret;

   /* check if entire message could be sent */

   if (ret < bufLen) 
   {   
      /* cleanup */
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, sendBuf, bufLen);      
      return (RWOULDBLOCK);
   }

   /* cleanup */
   SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, sendBuf, bufLen);      

#else /* end of Win NT/flat buffer specific part */
   ret = SFndLenMsg(mBuf, &msgLen);
   if (ret != ROK)
   {
      return RFAILED;
   }

   /* added */
/*   memset(&msg, 0, sizeof(msg)); */
   msg.msg_flags = 0;

   if (dstAddr != NULLP)
   {
#ifdef SS_LINUX
      msg.msg_name    = (Void*)sockAddrPtr;
#else
#ifdef SS_PS
      msg.msg_name    = (char *)sockAddrPtr;
#else
      msg.msg_name    = (caddr_t)sockAddrPtr;
#endif /* SS_PS */
#endif /* SS_LINUX */
      msg.msg_namelen = sizeOfAddr;
   }
   else
   {
      msg.msg_name    = NULLP;         
      msg.msg_namelen = 0;
   }
   /* added  defined(_XPG4_2) */
#if (defined(SS_LINUX) || defined(_XPG4_2))
   msg.msg_control    = NULLP;
   msg.msg_controllen  = 0;
#else
   msg.msg_accrights     = 0;
   msg.msg_accrightslen  = NULLP; 
#endif /* SS_LINUX */

   /* allocate scatter vector */
   numDBufs = CM_INET_MAX_DBUF;
   retVal = RNA;
   ret = ROK;
   unSentLen = msgLen;
   strtEndDBufNum = 0;
   *len = 0;
   if (ipHdrParams != NULLP && (ipHdrParams->type == CM_INET_IPV4ADDR_TYPE))
      if((ipHdrParams->u.hdrParmIpv4.tos.pres == TRUE)&& \
            (ipHdrParams->u.hdrParmIpv4.tos.val != 0))
      {
         cmInet4FillTos(ipHdrParams->u.hdrParmIpv4.tos.val, 
               (uint8_t *)(cmsgData + curMsgIdx), &curMsgIdx);
         msg.msg_control = cmsgData;     /* pointer to Ancillary Data */
         msg.msg_controllen = curMsgIdx; /* total length of ancillary Data */
      } 
   /* if the sender wants to send Ipv6 exten. headers */
#ifdef IPV6_OPTS_SUPPORTED
   if (ipHdrParams != NULLP && (ipHdrParams->type == CM_INET_IPV6ADDR_TYPE))
   {     
#ifdef SS_LINUX
      if(ipHdrParams->u.ipv6HdrParm.ttl.pres == TRUE)
      {
         cmInetBuildSendHoplimit((uint32_t)ipHdrParams->u.ipv6HdrParm.ttl.val,
               (uint8_t *)(cmsgData + curMsgIdx), &curMsgIdx);  
      }
#endif /* SS_LINUX */

#ifdef LOCAL_INTF      
      /* have to decide how to get the src addr to add in in6_pktinfo */
      if(ipHdrParams->u.ipv6HdrParm.srcAddr6.type == 6)
      {  
         cmInet6BuildSendPktinfo(
               &ipHdrParams->u.ipv6HdrParm.srcAddr6.u.ipv6NetAddr, 
               (uint8_t *)(cmsgData + curMsgIdx), &curMsgIdx,
               sockFd->protType);
      }
#endif /* LOCAL_INTF */

      /* copy each ipv6 ext header from ipHdrParams to the flat buffer
       * cmsgData one by one. */

      if (ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.hbhHdrPrsnt == TRUE)
         /* build HBH ext header in cmsgData starting at indx 0 */
         cmInet6BuildSendHBHOpts(
               &ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.hbhOptsArr, 
               (uint8_t *)(cmsgData + curMsgIdx), &curMsgIdx, 0);                           

      /* now copy the elements from the Destination Option array one by
       * one to the Flat Buffer cmsgData. Start filling at indx curMsgIdx 
       * which is the end of HBH hdr. */          
      if (ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.destOptsPrsnt == TRUE)
         /* build Dest opt hdr starting at (cmsgData + curMsgIdx) */
         cmInet6BuildSendDestOpts(
               &(ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.destOptsArr),
               (uint8_t *)(cmsgData + curMsgIdx), &curMsgIdx, 1);

      /* copy Route header to to the Flat Buffer cmsgData */
      if (ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.rtOptsPrsnt == TRUE)
         /* curMsgIdx will be the indx where Dest opt ends in cmsgData */
         cmInet6BuildSendRouteOpts(
               &ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.rtOptsArr,
               (uint8_t *)(cmsgData + curMsgIdx), &curMsgIdx);

      /* msghrd struc's msg_control will point cmsgData and msg_controllen
       * will be the curMsgIdx */ 
      msg.msg_control = cmsgData;     /* pointer to Ancillary Data */
      msg.msg_controllen = curMsgIdx; /* total length of ancillary Data */

   }
#endif /* IPV6_OPTS_SUPPORTED */

   /* Loop till all the data is sent or till the sendmsg call cannot send 
    * any more data. */
   do
   {
      /* build the send vector */ 
      /* cm_inet_c_001.main_50 - Partial send handling. Added variable to hold
         total length of the packed dbufs */
      retVal = buildSendIovec(mBuf, unSentLen, txArr, numDBufs, &i, 
            &strtEndDBufNum, &ioLen);
      if (retVal != ROK)
      {
         if (retVal == RNA)
         {
            /* Incase of UDP/RAW messages call SCompressMsg. */
            if (sockFd->type != CM_INET_STREAM)
            {
               /* Compress the message into a single dBuf */
               ret = SCompressMsg(mBuf);
               if (ret != ROK)
                  return RFAILED;

               strtEndDBufNum = 0;
               /* Rebuild the send vector */
               /* cm_inet_c_001.main_50 - Partial send handling. Added variable to hold
                  total length of the packed dbuf */
               ret = buildSendIovec(mBuf, unSentLen, txArr, numDBufs, &i,
                     &strtEndDBufNum, &ioLen);
               if (ret != ROK)
                  return RFAILED;

               retVal = ROK;
            }
         }
         else
            return RFAILED;
      }
      msg.msg_iov           = txArr;
      msg.msg_iovlen        = i;


#ifdef NTL_LIB
     {
         int ntl_hLib;
         if ( sockFd->fd >= 0xD001)
             ret = ntl_sendmsg(ntl_hLib, sockFd->fd, &msg, 0);
   	     else
             ret = sendmsg(sockFd->fd, &msg, 0);
     }
#else 
     ret = sendmsg(sockFd->fd, &msg, 0);
#endif 
      /* cm_inet_c_001.main_50 - Update the length only in successful cases */
      if (ret == INET_ERR)
      {
         if((INET_ERR_CODE == ERR_AGAIN) ||
               (INET_ERR_CODE == ERR_WOULDBLOCK))
         {
            /* cm_inet_c_001.main_50 - Return without making length 0, if in case the partial 
               message was sent earlier */
            return (RWOULDBLOCK);
         }
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
         /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSendDscpMsg() Failed : error(%d),"
               " sockFd->fd(%ld)\n", INET_ERR_CODE, sockFd->fd);
         CMINETLOGERROR(ERRCLS_DEBUG, ECMINET024, 0, prntBuf);
#else
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSendDscpMsg() Failed : error(%d),"
               " sockFd->fd(%d)\n", INET_ERR_CODE, sockFd->fd);
         CMINETLOGERROR(ERRCLS_DEBUG, ECMINET024, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */

         /* cm_inet_c_001.main_37 network unreacheble error is added */
         /* check if network is reacheble or not */
         if ((INET_ERR_CODE == ERR_NETUNREACH))      
         {
            return (RNETFAILED);
         }

         /*  Check if connection was closed by the peer */
         if ((INET_ERR_CODE == ERR_PIPE) ||
               (INET_ERR_CODE == ERR_CONNREFUSED) ||
               (INET_ERR_CODE == ERR_CONNABORTED))
         {
            *len = 0;
            return (RCLOSED);
         }
         return RFAILED;
      }

      /* cm_inet_c_001.main_50 - Update the length only in successful cases */
      *len += ret;

      /* cm_inet_c_001.main_50 - if what is actually sent is less than what is attemped
       * to be sent, then return WOULDBLOCK
       */
      if (ret < ioLen)
         return (RWOULDBLOCK);

      unSentLen -= ret;

   } while (*len < msgLen);
#endif /* WIN32 | CMINETFLATBUF */

   return ROK;

} /* end of cmInetSendDscpMsg */

/*
*
*      Fun:   cmInetSendMsg
*
*      Desc:  Sends the message data hold by mBuf. 
*             The len paramter gives the actual written octets. If the socket
*             is non-blocking this value can be differ from the mBuf length 
*             because there was not enough transmit buffer space available. If 
*             this occurs, RWOULDBLOCK is returned and only a part of the mBuf
*             is sent.
*             Values for flag parameter:
*  
*             CM_INET_NO_FLAG - no additional control flag
*
*      Ret:   ROK         - successful
*             RWOULDBLOCK - no or not entire mBuf sent because would block
*             ROUTRES     - failed, out of resources
*             RCLOSED     - connection was closed by the peer
*             RFAILED     - failed
*                           
*      Notes: The successful completion of a send call does not indicate that 
*             the data has been successfully delivered! 
*
*             This function does not free any sent buffers.  
*
*   
*      File:  cm_inet.c
*
*/

S16 cmInetSendMsg
(
CmInetFd       *sockFd,         /* socket file descriptor */
CmInetAddr     *dstAddr,        /* destination Internet address/port */
CmInetMemInfo  *info,           /* buffer allocation info */
Buffer         *mBuf,           /* buffer structure to send */
MsgLen         *len,            /* number of actually sent octets */
/* added for IPv6 ext hdr */
#ifdef IPV6_OPTS_SUPPORTED
CmInetIpHdrParm *ipHdrParams,   /* IPv6 extensions headers */
#endif /* IPV6_OPTS_SUPPORTED */
S16             flags           /* additional control flags, unused */
)
{
#if (defined(WIN32) || defined(CMINETFLATBUF))
   S32     ret;                 /* temporary return value */
   MsgLen  msgLen;              /* message length */ 
   MsgLen  bufLen;              /* send buffer length */
   Data   *sendBuf;             /* plain send buffer */
#else
   S32 ret;                     /* temporary return value */
   S32 retVal;                  /* temporary return value */
   S16 i;                       /* loop index */
   CmInetIovec  txArr[CM_INET_MAX_DBUF] ={{0}}; /* scatter vector */
   S16      numDBufs;           /* number of dBufs in message */
   struct   msghdr msg;         /* sendmsg() message header */
   MsgLen   msgLen;             /* message length */ 
   uint32_t      strtEndDBufNum;     /* starting/ending DBuf number */ 
   MsgLen   unSentLen;          /* sent len */
#ifdef IPV6_SUPPORTED 
   /* added for IPv6 ext hdr */
#ifdef IPV6_OPTS_SUPPORTED
   uint32_t    curMsgIdx;            /* indx in cmsgData where to write an ext hdr */
#if (defined(SS_LINUX) || defined(_XPG4_2))
   /* alloc from stack for IPv6 ancill data */
   uint8_t     cmsgData[CM_INET_IPV6_ANCIL_DATA];
#endif /* SS_LINUX || _XPG4_2 */
#endif /* IPV6_OPTS_SUPPORTED */
#else
#if (defined(SS_LINUX) || defined(_XPG4_2))
   /* alloc from stack for IPv4 ancill data */ 
   /* uint8_t     cmsgData[CM_INET_IPV4_ANCIL_DATA];*/
#endif /* SS_LINUX || _XPG4_2 */
#endif /* IPV6_SUPPORTED */   
#endif /* WIN32 | CMINETFLATBUF */  

   struct  sockaddr_in remAddr; /* remote Internet address */   
#ifdef IPV6_SUPPORTED
   struct   sockaddr_in6  remAddr6; /* remote Internet address */   
#endif /* IPV8_SUPPORTED */
   CmInetSockAddr *sockAddrPtr;
   /* cm_inet_c_001.main_58 : Fix for klockwork issue */
   uint32_t            sizeOfAddr;    

   /* cm_inet_c_001.main_50 - Added for partial send handling */
   /* cm_inet_c_001.main_59: Protected under if not defined WIN32*/
#if (!defined(WIN32)) 
   MsgLen         ioLen; 
#endif


      UNUSED(flags);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd) ||
         (info == NULLP) || (len == NULLP))
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   /* added for IPv6 ext hdr */
#if !(defined(WIN32) || defined(CMINETFLATBUF))
#if (defined(SS_LINUX) || defined(_XPG4_2))
/*   memset(cmsgData, 0, sizeof(cmsgData));    */
#endif /* SS_LINUX || _XPG4_2 */
#ifdef IPV6_OPTS_SUPPORTED
   curMsgIdx   = 0;
#endif /* IPV6_SUPPORTED */ 
#endif /* WIN32 | CMINETFLATBUF */

   msgLen = 0;  /* need by CC to pass without warning */
   sockAddrPtr = NULLP;
   sizeOfAddr = 0;

   /* setup remote address */
   if (dstAddr != NULLP)
   {
#ifdef IPV6_SUPPORTED
      if (dstAddr->type == CM_INET_IPV6ADDR_TYPE)
      {
         memset(&remAddr6, 0, sizeof(remAddr6));
         remAddr6.sin6_family = AF_INET6;
         remAddr6.sin6_port   = CM_INET_HTON_UINT16(dstAddr->u.ipv6Addr.port);
         CM_INET_COPY_IPV6ADDR(&remAddr6.sin6_addr, 
               &dstAddr->u.ipv6Addr.ipv6NetAddr); 
         sizeOfAddr = sizeof(remAddr6);
         sockAddrPtr = (CmInetSockAddr *)&remAddr6;
      }
      else
      {
         memset(&remAddr, 0, sizeof(remAddr));
         remAddr.sin_family = AF_INET;
         remAddr.sin_port   = CM_INET_HTON_UINT16(dstAddr->u.ipv4Addr.port);
         remAddr.sin_addr.s_addr = 
            CM_INET_HTON_UINT32(dstAddr->u.ipv4Addr.address);
         sizeOfAddr = sizeof(remAddr);
         sockAddrPtr = (CmInetSockAddr *)&remAddr;
      }
#else
/*      memset(&remAddr, 0, sizeof(remAddr)); */
      remAddr.sin_family      = AF_INET;
      remAddr.sin_port        = CM_INET_HTON_UINT16(dstAddr->port);
      remAddr.sin_addr.s_addr = CM_INET_HTON_UINT32(dstAddr->address);
      sizeOfAddr = sizeof(remAddr);
      sockAddrPtr = (CmInetSockAddr *)&remAddr;
#endif /* IPV6_SUPPORTED */
   }

#if (defined(WIN32) || defined(CMINETFLATBUF))
   /* copy message to a flat buffer */
   ret = SFndLenMsg(mBuf, &bufLen);
   if (ret != ROK)
   {
      return RFAILED;
   }
   /* max message length is limited to control the memory usage */
   /* casting bufLen to avoid warnings */
   if ((bufLen > 0) && ((uint32_t)bufLen > CM_INET_MAX_MSG_LEN))
   {
      return RFAILED;
   }
   ret = SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, &sendBuf, bufLen);                  
   if (ret != ROK)
   {
      return (ROUTRES);
   }
   ret = SCpyMsgFix(mBuf, 0, bufLen, sendBuf, &msgLen);
   if ((ret != ROK) || (msgLen != bufLen)) 
   {
      /* cleanup */
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, sendBuf, bufLen);       
      return RFAILED;
   }

   if (dstAddr == NULLP)
   {
      /* VxWorks sendto has some problem
       * with connected UDP socket, use send */
#ifndef SS_VW
      ret = sendto(sockFd->fd, (S8 *)sendBuf, bufLen, 0, 
            NULLP, sizeOfAddr);
#else
      ret = send(sockFd->fd, (S8 *)sendBuf, bufLen, 0);
#endif /* end of SS_VW */
   }
   else
      /* cm_inet_c_001.main_54: Fix for vxworks 6.7 sending data on TCP sockets */
   {
#if (defined(SS_VW) && defined(SS_VW6_7)) 
      if ((sockFd->type  == CM_INET_STREAM) || (sockFd->type == SOCK_RDM) )
      {
         ret = send(sockFd->fd, (S8 *)sendBuf, bufLen, 0);
      }
      else
#endif /* end of SS_VW6_7 and SS_VW */
      {
         ret = sendto(sockFd->fd, (S8 *)sendBuf, bufLen, 0, 
               sockAddrPtr, sizeOfAddr); 
      }
   }
   if (ret == INET_ERR)
   {
      /* cleanup */
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, sendBuf, bufLen);      

      if(INET_ERR_CODE == ERR_AGAIN)
      {
         *len = 0;
         return (RWOULDBLOCK);
      }

      /* Check for ERR_WOULDBLOCK */
      if(INET_ERR_CODE == ERR_WOULDBLOCK)
      {
         *len = 0;
         return (RWOULDBLOCK);
      }

#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSendMsg() Failed : error(%d), msgLen(%d),"
            " sockFd->fd(%ld)\n", INET_ERR_CODE, bufLen, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET023, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSendMsg() Failed : error(%d), msgLen(%d),"
            " sockFd->fd(%d)\n", INET_ERR_CODE, bufLen, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET023, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */

      /* cm_inet_c_001.main_37 network unreacheble error is added */
      /* check if network is reacheble*/
      if ((INET_ERR_CODE == ERR_NETUNREACH))
      {
         return (RNETFAILED);
      }


      /*  Check if connection was closed */
      if ((INET_ERR_CODE == ERR_PIPE) ||
            (INET_ERR_CODE == ERR_CONNABORTED) || 
            (INET_ERR_CODE == ERR_CONNRESET))
      {
         *len = 0;
         return (RCLOSED);
      }

      return RFAILED;
   }

   *len = ret;

   /* check if entire message could be sent */

   if (ret < bufLen) 
   {   
      /* cleanup */
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, sendBuf, bufLen);      
      return (RWOULDBLOCK);
   }

   /* cleanup */
   SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, sendBuf, bufLen);      

#else /* end of Win NT/flat buffer specific part */
   ret = SFndLenMsg(mBuf, &msgLen);
   if (ret != ROK)
   {
      return RFAILED;
   }

   /* added */
/*   memset(&msg, 0, sizeof(msg)); */
   msg.msg_flags = 0;

   if (dstAddr != NULLP)
   {
#ifdef SS_LINUX
      msg.msg_name    = (Void*)sockAddrPtr;
#else
#ifdef SS_PS
      msg.msg_name    = (char *)sockAddrPtr;
#else
      msg.msg_name    = (caddr_t)sockAddrPtr;
#endif /* SS_PS */
#endif /* SS_LINUX */
      msg.msg_namelen = sizeOfAddr;
   }
   else
   {
      msg.msg_name    = NULLP;         
      msg.msg_namelen = 0;
   }
   /* added  defined(_XPG4_2) */
#if (defined(SS_LINUX) || defined(_XPG4_2))
   msg.msg_control    = NULLP;
   msg.msg_controllen  = 0;
#else
   msg.msg_accrights     = 0;
   msg.msg_accrightslen  = NULLP; 
#endif /* SS_LINUX */

   /* allocate scatter vector */
   numDBufs = CM_INET_MAX_DBUF;
   retVal = RNA;
   ret = ROK;
   unSentLen = msgLen;
   strtEndDBufNum = 0;
   *len = 0;

   /* if the sender wants to send Ipv6 exten. headers */
#ifdef IPV6_OPTS_SUPPORTED
   if (ipHdrParams != NULLP && (ipHdrParams->type == CM_INET_IPV6ADDR_TYPE))
   {     
#ifdef SS_LINUX
      if(ipHdrParams->u.ipv6HdrParm.ttl.pres == TRUE)
      {
         cmInetBuildSendHoplimit((uint32_t)ipHdrParams->u.ipv6HdrParm.ttl.val,
               (uint8_t *)(cmsgData + curMsgIdx), &curMsgIdx);  
      }
#endif /* SS_LINUX */

#ifdef LOCAL_INTF      
      /* have to decide how to get the src addr to add in in6_pktinfo */
      if(ipHdrParams->u.ipv6HdrParm.srcAddr6.type == 6)
      {  
         cmInet6BuildSendPktinfo(
               &ipHdrParams->u.ipv6HdrParm.srcAddr6.u.ipv6NetAddr, 
               (uint8_t *)(cmsgData + curMsgIdx), &curMsgIdx,
               sockFd->protType);
      }
#endif /* LOCAL_INTF */

      /* copy each ipv6 ext header from ipHdrParams to the flat buffer
       * cmsgData one by one. */

      if (ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.hbhHdrPrsnt == TRUE)
         /* build HBH ext header in cmsgData starting at indx 0 */
         cmInet6BuildSendHBHOpts(
               &ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.hbhOptsArr, 
               (uint8_t *)(cmsgData + curMsgIdx), &curMsgIdx, 0);                           

      /* now copy the elements from the Destination Option array one by
       * one to the Flat Buffer cmsgData. Start filling at indx curMsgIdx 
       * which is the end of HBH hdr. */          
      if (ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.destOptsPrsnt == TRUE)
         /* build Dest opt hdr starting at (cmsgData + curMsgIdx) */
         cmInet6BuildSendDestOpts(
               &(ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.destOptsArr),
               (uint8_t *)(cmsgData + curMsgIdx), &curMsgIdx, 1);

      /* copy Route header to to the Flat Buffer cmsgData */
      if (ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.rtOptsPrsnt == TRUE)
         /* curMsgIdx will be the indx where Dest opt ends in cmsgData */
         cmInet6BuildSendRouteOpts(
               &ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.rtOptsArr,
               (uint8_t *)(cmsgData + curMsgIdx), &curMsgIdx);

      /* msghrd struc's msg_control will point cmsgData and msg_controllen
       * will be the curMsgIdx */ 
      msg.msg_control = cmsgData;     /* pointer to Ancillary Data */
      msg.msg_controllen = curMsgIdx; /* total length of ancillary Data */

   }
#endif /* IPV6_OPTS_SUPPORTED */

   /* Loop till all the data is sent or till the sendmsg call cannot send 
    * any more data. */
   do
   {
      /* build the send vector */ 
      /* cm_inet_c_001.main_50 - Partial send handling. Added variable to hold
         total length of the packed dbufs */
      retVal = buildSendIovec(mBuf, unSentLen, txArr, numDBufs, &i, 
            &strtEndDBufNum, &ioLen);
      if (retVal != ROK)
      {
         if (retVal == RNA)
         {
            /* Incase of UDP/RAW messages call SCompressMsg. */
            if (sockFd->type != CM_INET_STREAM)
            {
               /* Compress the message into a single dBuf */
               ret = SCompressMsg(mBuf);
               if (ret != ROK)
                  return RFAILED;

               strtEndDBufNum = 0;
               /* Rebuild the send vector */
               /* cm_inet_c_001.main_50 - Partial send handling. Added variable to hold
                  total length of the packed dbuf */
               ret = buildSendIovec(mBuf, unSentLen, txArr, numDBufs, &i,
                     &strtEndDBufNum, &ioLen);
               if (ret != ROK)
                  return RFAILED;

               retVal = ROK;
            }
         }
         else
            return RFAILED;
      }
      msg.msg_iov           = txArr;
      msg.msg_iovlen        = i;


#ifdef NTL_LIB
     {
         int ntl_hLib;
         if ( sockFd->fd >= 0xD001)
             ret = ntl_sendmsg(ntl_hLib, sockFd->fd, &msg, 0);
   	     else
             ret = sendmsg(sockFd->fd, &msg, 0);
     }
#else 
     ret = sendmsg(sockFd->fd, &msg, 0);
#endif 
      /* cm_inet_c_001.main_50 - Update the length only in successful cases */
      if (ret == INET_ERR)
      {
         if((INET_ERR_CODE == ERR_AGAIN) ||
               (INET_ERR_CODE == ERR_WOULDBLOCK))
         {
            /* cm_inet_c_001.main_50 - Return without making length 0, if in case the partial 
               message was sent earlier */
            return (RWOULDBLOCK);
         }
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
         /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSendMsg() Failed : error(%d),"
               " sockFd->fd(%ld)\n", INET_ERR_CODE, sockFd->fd);
         CMINETLOGERROR(ERRCLS_DEBUG, ECMINET024, 0, prntBuf);
#else
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSendMsg() Failed : error(%d),"
               " sockFd->fd(%d)\n", INET_ERR_CODE, sockFd->fd);
         CMINETLOGERROR(ERRCLS_DEBUG, ECMINET024, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */

         /* cm_inet_c_001.main_37 network unreacheble error is added */
         /* check if network is reacheble or not */
         if ((INET_ERR_CODE == ERR_NETUNREACH))      
         {
            return (RNETFAILED);
         }

         /*  Check if connection was closed by the peer */
         if ((INET_ERR_CODE == ERR_PIPE) ||
               (INET_ERR_CODE == ERR_CONNREFUSED) ||
               (INET_ERR_CODE == ERR_CONNABORTED))
         {
            *len = 0;
            return (RCLOSED);
         }
         return RFAILED;
      }

      /* cm_inet_c_001.main_50 - Update the length only in successful cases */
      *len += ret;

      /* cm_inet_c_001.main_50 - if what is actually sent is less than what is attemped
       * to be sent, then return WOULDBLOCK
       */
      if (ret < ioLen)
         return (RWOULDBLOCK);

      unSentLen -= ret;

   } while (*len < msgLen);
#endif /* WIN32 | CMINETFLATBUF */

   return ROK;

} /* end of cmInetSendMsg */


/* added new functions for IPv6 extension headers */
#ifdef IPV6_OPTS_SUPPORTED
#ifdef LOCAL_INTF
/*
*
*      Fun:   cmInet6BuildSendPktinfo
*
*      Desc:  This function inserts src address (into ancillary data) which 
*             will be used as the src addr in outgoing IP packet when sending
*             that packet using sendmsg()function.
*
*      Ret:   ROK   
*
*      Notes:  
*
*      File:  cm_inet.c
*
*/

static S16 cmInet6BuildSendPktinfo
(
CmInetIpAddr6 *srcAddr, /* src ip addr to set on outgoing packet */
uint8_t  *cmsgBuf,   /* flat buffer where to build ext hdrs */
uint32_t *curMsgIdx, /* idx in cmsgBuf where HBH/Dest ext hdr ends */
uint8_t   protType   /* whether IPv4/IPv6 socket */
)
{
   struct cmsghdr *tempHdr;
   struct in6_pktinfo *ipv6Pktinfo;
   struct in6_addr lpBkAddr;
   uint8_t     len;


      len = 0;

   lpBkAddr = in6addr_loopback;

   /* cmsghdr struc will appear before data in the ancillary data object. 
    * So put cmsghdr struc in flat buffer first. */

   /* cmsghdr struc points to flat buffer's starting address */
   tempHdr = (struct cmsghdr *)cmsgBuf;  

   /* fill up level & type of cmsghdr structure */
   if (protType == AF_INET6)
   {   
      tempHdr->cmsg_level = IPPROTO_IPV6;
      tempHdr->cmsg_type = IPV6_PKTINFO;
   }
#ifdef CMINETDBG   
   else if(protType == AF_INET)
   {
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "Invalid socket type in cmInet6BuildPktinfo(),"
            "protType(%d)\n", protType);
      CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET025, 0, prntBuf);

   }
#endif
   /* skip length of cmsghdr structure - 12 bytes */
   len += sizeof(struct cmsghdr); 

   if(protType == AF_INET6)
      ipv6Pktinfo = (struct in6_pktinfo *)(cmsgBuf + len);
#ifdef CMINETDBG   
   else
   {
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "Invalid socket type in cmInet6BuildPktinfo(),"
            "protType(%d)\n",  protType);
      CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET026, 0, prntBuf);
   }
#endif

   /* insert the hoplimit. This will override the kernel's
    * default hoplimit value */
   if(protType == AF_INET6)
   {  
      /* store ipv6 src addr */ 
      memcpy(&(ipv6Pktinfo->ipi6_addr), srcAddr, 16);
      len += 16;

      /* store interface index */
      /* 0 is invalid intf indx it tells kernel to chose any intf it likes to
       * send this pkt. if we use nozero intf indx then kernel will send this
       * pkt only through that intf */
      ipv6Pktinfo->ipi6_ifindex = 0;
      len += sizeof(int);
   }   
#ifdef CMINETDBG   
   else if(protType == AF_INET)
   {
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "Invalid socket type in cmInet6BuildPktinfo(),"
            "protType(%d)\n", protType);
      CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET027, 0, prntBuf);
   }
#endif

   /* fill up the length of cmsghdr structure */
   tempHdr->cmsg_len = len;  
   *curMsgIdx += len;

   return ROK;

}/* end of cmInet6BuildSendPktinfo */ 
#endif /* LOCAL_INTF */


#ifdef SS_LINUX
/*
*
*      Fun:   cmInetBuildSendHoplimit
*
*      Desc:  This function inserts hoplimit value to be sent out by ancillary
*             data by calling sendmsg()function.
*
*      Ret:   ROK   
*
*      Notes:  
*
*      File:  cm_inet.c
*
*/

static S16 cmInetBuildSendHoplimit
(
uint32_t   hoplimit,  /* the hoplimit value to be set on outgoing packet */
uint8_t  *cmsgBuf,   /* flat buffer where to build ext hdrs */
uint32_t *curMsgIdx  /* idx in cmsgBuf where HBH/Dest ext hdr ends */
)
{
   struct cmsghdr *tempHdr; 
   uint8_t    len;


      len = 0;

   /* cmsghdr struc will appear before data in the ancillary data object. 
    * So put cmsghdr struc in flat buffer first. */

   /* cmsghdr struc points to flat buffer's starting address */
   tempHdr = (struct cmsghdr *)cmsgBuf;  

   /* fill up level & type of cmsghdr structure */
   tempHdr->cmsg_level = IPPROTO_IPV6;
   tempHdr->cmsg_type = IPV6_HOPLIMIT;

   /* skip cmsghdr struc (length of cmsghdr structure) */
   len += sizeof(struct cmsghdr); 

   /* insert the hoplimit. This will override the kernel's
    * default hoplimit value */ 
   *(cmsgBuf + len) = hoplimit;
   len += sizeof(hoplimit);

   /* fill up the length of cmsghdr structure */
   tempHdr->cmsg_len = len;
   *curMsgIdx += len;

   return ROK;
} /* end of cmInetBuildSendHoplimit  */
#endif /* SS_LINUX */


/*
*
*      Fun:   cmInet6BuildSendHBHOpts
*
*      Desc:  This function builds the HopByHop option which will be put
*             in the data portion of the ancillary data object. To build  
*             the HopByHop option this function takes an array of 
*             individual HopByHop option and fill them in a flat buffer.
*             cmsghdr struc always appear before HopBYHop Options, Dest 
*             Options and Route header option.
*
*             The address of the flat Buffer *cmsgBuf is passed to this
*             function from cmInetSendMsg. This buffer will have all extension
*             headers. This buffer is passed as ancillary data by sendmsg()
*     
*      Ret:   
*
*      Notes: This function will also be used for Destination options 
*
*      File:  cm_inet.c
*
*/

static S16 cmInet6BuildSendHBHOpts
(
CmInetIpv6HBHHdrArr *hbhOptsArr,/* IPv6 extensions headers HBH/Dest opts */
uint8_t *cmsgBuf,                    /* flat buffer where to build ext hdrs */
uint32_t *curMsgIdx,                 /* idx in cmsgBuf where HBH/Dest ext hdr ends */
uint8_t hdrId                        /* 0: HBH hdr, 1:Dest Hdr */
)
{
   struct cmsghdr *tempHdr; 
   uint8_t    len;
   uint8_t    optsIdx;


      len = 0;
   optsIdx = 0;

   /* cmsghdr struc will appear before data in the ancillary data object. 
    * So put cmsghdr struc in flat buffer first. */

   /* cmsghdr struc points to flat buffer's starting address */
   tempHdr = (struct cmsghdr *)cmsgBuf;  

   /* fill up level & type of cmsghdr structure */
   if (hdrId == 0)
   {   
      tempHdr->cmsg_level = IPPROTO_IPV6;
      tempHdr->cmsg_type = IPV6_HOPOPTS;
   }
   else if (hdrId == 1)
   {
      tempHdr->cmsg_level = IPPROTO_IPV6;
      tempHdr->cmsg_type = IPV6_DSTOPTS;
   }

   /* skip cmsghdr struc (length of cmsghdr structure) */
   len += (sizeof(tempHdr->cmsg_level) + sizeof(tempHdr->cmsg_len) + 
         sizeof(tempHdr->cmsg_type));

   /* Next Hdr: will be fill up accordingly by Kernel */ 
   *(cmsgBuf + len) = 0x00;
   len += 1;

   /* Header Ext Length: will be fill up by us. In units of 8-byte excluding 
    * first 8 bytes starting from Next Header field. */       
   *(cmsgBuf + len) = 0x00; 
   len += 1;

   /* fillup all HBH/dest options' TLV. Here, we assume that all the HBH/dest
    * options are packed inside 1 HBH option header. */      
   for (optsIdx = 0; optsIdx < hbhOptsArr->numHBHOpts; 
         optsIdx ++)
   {
      /* Copy the TLV into cmsgBuf data portion */
      /* copy type field of every HBH/dest option */
      *(cmsgBuf + len) = hbhOptsArr->hbhOpts[optsIdx].type;
      len += sizeof(hbhOptsArr->hbhOpts[optsIdx].type);

      /* copy length field of every HBH/dest option */
      *(cmsgBuf + len) = hbhOptsArr->hbhOpts[optsIdx].length; 
      len += sizeof(hbhOptsArr->hbhOpts[optsIdx].length);         

      /* copy all value bytes of current HBH/dest option to the flat buffer */
      memcpy((cmsgBuf + len),
            (hbhOptsArr->hbhOpts[optsIdx].value), 
            hbhOptsArr->hbhOpts[optsIdx].length);
      len += hbhOptsArr->hbhOpts[optsIdx].length; 
   }

   /* cuMsgIdx will have the total length of HBH options array */
   /* add this length to the length of cmsgHdr struc */

   /* Padding: Different header has different padding requirement(xn+y). For
    * HBH Router Alert we need 2 bytes of padding. As this same function is
    * used for Destination option also and there is no option for it is yet
    * proposed, we are passing padN options - 6 bytes to make the Dest Option
    * hdr a multiple of 8 bytes. */

   /* HBH: padN of 2 bytes needed for Router Alert */
   /* This logic is present currently to support router alert which is the 
    * only supported HBH option today. For other, generic method needed */ 
   if (hdrId == 0)
   {       
      *(cmsgBuf + len) = 0x01;
      len += 1;
      *(cmsgBuf + len) = 0x00;
      len += 1;
   }

   /* fill up the length of cmsghdr structure */
   tempHdr->cmsg_len = len;
   *curMsgIdx += len;

   return ROK;    
} /* end of cmInet6BuildSendHBHOpts */


/*
*
*      Fun:   cmInet6BuildSendRouteOpts
*
*      Desc:  This function transfers bytes from the Route hdr array to the 
*             flat buffer. First the top cmsghdr structure will be filled in
*             the flat buffer, then route hdr type 0 will be added after 
*             cmsghdr struc in the flat buffer. Then all IPV6 addresses will
*             be filled up.
*     
*      Ret:   
*
*      Notes: None
*
*      File:  cm_inet.c
*
*/

static S16 cmInet6BuildSendRouteOpts
(
CmInetIpv6RtHdr *rtOptsArr,  /* IPv6 destination options array */
uint8_t *cmsgBuf,                 /* flat buffer where to build ext hdrs */
uint32_t *curMsgIdx               /* idx in cmsgBuf where to start building RT hdr */
)
{
   struct cmsghdr *tempHdr;
   CmInetIpv6RtHdr0 *tempRtHdr;
   uint8_t    len; 
   uint8_t    addrIdx;


   len = 0;
   addrIdx = 0;

   /* cmsghdr struc will appear before data in the ancillary data object. 
    * So put cmsghdr struc in flat buffer first */

   /* cmsghdr struc points to flat buffer */
   tempHdr = (struct cmsghdr *)(cmsgBuf); 

   tempHdr->cmsg_level = IPPROTO_IPV6;
   tempHdr->cmsg_type = IPV6_RTHDR;

   /* skip cmsghdr structure */
   len += sizeof(struct cmsghdr);

   /* we know the total size of Route hdr if we know the num of ipv6 addrs */
   tempHdr->cmsg_len = len + sizeof(CmInetIpv6RtHdr0)
      + rtOptsArr->numAddrs * sizeof(CmInetIpAddr6);

   /* attach route hdr type 0 after cmsghdr structure */
   tempRtHdr = (CmInetIpv6RtHdr0 *)(cmsgBuf + len);

   /* fill up fields of route hdr type 0 */

   /* will be filled up by Kernel */
   tempRtHdr->ip6r0_nextHdr = 0x00;  

   tempRtHdr->ip6r0_hdrExtLen = (2 * rtOptsArr->numAddrs); 

   /* only type supported today */
   tempRtHdr->ip6r0_type = 0x00;        

   tempRtHdr->ip6r0_segLeft = rtOptsArr->numAddrs; 

   /* Note: rfc 2292(1998) mentions 1 reserve byte & 3 strict/loose bytes 
    * restricting total 23 ipv6 addresses can be added to the route header.
    * But rfc 2292(2002) mentions all 4 bytes are reserved which allows 
    * as many ipv6 addresses as wishes to be added to the route header */

   tempRtHdr->ip6r0_resrvAndSLmap = rtOptsArr->slMap;

   /* move pointer in the flat buffer to the end of this structure */
   len +=  sizeof(CmInetIpv6RtHdr0); 

   /* fill up all IPV6 addresses from rtOptsArr in the flat buffer */
   for (addrIdx = 0; addrIdx < rtOptsArr->numAddrs; addrIdx++)
   {   
      memcpy((cmsgBuf + len),
            (rtOptsArr->ipv6Addrs[addrIdx]), 16);
      len += 16;
   }

   *curMsgIdx += len;  
   return ROK;
} /* end of cmInet6BuildSendRouteOpts */


/*
*
*      Fun:   cmInet6BuildRecvHopOptsArr
*
*      Desc:  This function fills up the HopByHop Array of ipHdrParam from 
*             the ancillary data received through recvmsg() call. The memory
*             to hold the extension headers is allocated here. All received 
*             ext hdr info will be passed to upper user as ipHdrParam.
*     
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: None
*
*      File:  cm_inet.c
*
*/

static S16 cmInet6BuildRecvHopOptsArr
(
uint8_t *cmsgData,                    /* flat buffer where to build ext hdrs */
uint32_t hbhDataLen,                  /* byte len of cmsghdr + hbh ancil data */
CmInetIpv6HBHHdrArr *hbhOptsArr, /* IPv6 extensions headers */
uint8_t hdrId,                        /* 0: HBH, 1: DEST */
CmInetMemInfo   *info            /* Memory information */
)
{
   uint32_t curDataIdx;       /* to keep track where we are in the hbh Data */
   uint8_t  optsIdx;          /* how many hbh opts present in data */
   uint8_t  numOpts;          /* number of hbh opts present in data */
   uint8_t  tempLen;
   uint8_t  tempType;
   S16 ret;


      /* get length of actual hbh ancillary data */
      hbhDataLen -= sizeof(struct cmsghdr); 

   curDataIdx = 0;                
   optsIdx = 0;
   numOpts = 0;

   /* skip Next Hdr byte & Hdr Ext Length byte */
   curDataIdx += 2;               

   /* First find out how many hop-by-hop headers we need to allocate */
   for (;;)   
   {
      /* break when all HBH data is copied to hbhOptsArr */
      if (curDataIdx >= hbhDataLen)
         break;   

      numOpts += 1;

      /* get type */ 
      tempType = *(uint8_t *)(cmsgData + curDataIdx);
      curDataIdx += 1;               

      /* take care of pad1 option */
      if (tempType == 0) 
      {
         /* not considering the pad1 as valid option */
         numOpts -= 1; 
         continue;
      }

      /* get length */
      tempLen = *(uint8_t *)(cmsgData + curDataIdx);

      /* 1 is to skip length. tempLen to skip the value field */
      curDataIdx += (1 + tempLen);

      /* considering the padN as valid option for Dest Opt Hdr!!! As this is
       * the "only" valid option today. Ignore for HBH hdr */
      if (hdrId != 1)
         if (tempType == 1) 
            numOpts -= 1;
   }

   /* allocate mem needed to hold all HBH/Dest options */
   ret = SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, 
         (Data **)&hbhOptsArr->hbhOpts, 
         (Size)((sizeof(CmInetIpv6HBHHdr)) * numOpts)); 
   if (ret != ROK)
   {
#ifdef CMINETDBG      
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "SGetSBuf failure 1 in cmInet6BuildRecvHopOptsArr\n");
      CMINETLOGERROR(ERRCLS_ADD_RES, ECMINET028, 0, prntBuf);
#endif /* CMINETDBG */     
      return (ROUTRES);
   }   

   curDataIdx = 0;                
   optsIdx = 0;

   /* skip Next Hdr byte & Hdr Ext Length byte */
   curDataIdx += 2;               

   hbhOptsArr->numHBHOpts = numOpts;

   /* fill up HBH/dest opt array from recvd ancillary data */
   for (;;)   
   {
      /* break when all HBH data is copied to hbhOptsArr */
      if (curDataIdx >= hbhDataLen)
         break;   

      /* only copy Router Alert HBH option part which has type 5. Otherwise,
       * skip it when it is a PAD1, PADN or Jumbogram option for HBH. But 
       * consider padN as valid option for dest opt hdr. */

      /* get the type of current HBH/dest option */
      tempType = *(cmsgData + curDataIdx);
      curDataIdx += 1;

      /* ignore PAD1 for both HBH/dest by skipping to next option */
      if (tempType == 0)
         continue; 

      /* calculate how much to skip for padN in case of HBH */
      if (hdrId != 1)
      {   
         if (tempType == 1)
         {
            /* get the length field of padN option */
            tempLen = *(cmsgData + curDataIdx);
            curDataIdx += 1;

            /* move pointer forward to skip value field */
            curDataIdx += tempLen;
            continue;
         }
      } 
      hbhOptsArr->hbhOpts[optsIdx].type = tempType; 

      /* copy the length */
      hbhOptsArr->hbhOpts[optsIdx].length = *(cmsgData + curDataIdx);
      curDataIdx += 1;

      /* take care of PADN = 2 when value field empty. We also don't need
       * to allocate memory for empty value field */
      if (hbhOptsArr->hbhOpts[optsIdx].length == 0)
         hbhOptsArr->hbhOpts[optsIdx].value = NULLP;
      else
      {   
         /* take care of all other options having valid value field
          * such as Router Alert, PADN >= 3 bytes and Jumbo */
         ret = SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, 
               (Data **)&hbhOptsArr->hbhOpts[optsIdx].value, 
               (Size)hbhOptsArr->hbhOpts[optsIdx].length);
         if (ret != ROK)
         {
#ifdef CMINETDBG            
            /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "SGetSBuf failure 2 cmInet6BuildRecvHopOptsArr\n");
            CMINETLOGERROR(ERRCLS_ADD_RES, ECMINET029, 0, prntBuf);
#endif /* CMINETDBG */           
            /* now go inside every separate HBH option and free the memory
             * allocated for its value field */
            for (; optsIdx > 0; optsIdx --)
            {
               if (hbhOptsArr->hbhOpts[optsIdx - 1].value != NULLP)
               {
#ifdef CMINETDBG                  
                  /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
                  /* cm_inet_c_001.main_62:Warning fix */
                  snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "SPutSBuf call 1 in BuildRecvHopOptsArr\n");
                  CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET030, 0, prntBuf);
#endif /* CMINETDBG */                  
                  SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, 
                        (Data *)hbhOptsArr->hbhOpts[optsIdx - 1].value,
                        (Size)hbhOptsArr->hbhOpts[optsIdx - 1].length);
               }
            }
            /* clean up all CmInetIpv6HBHHdr structures allocated for all
             * arrived HBH options OR numOpts CmInetIpv6HBHHdr structures
             * allocated after counting numOpts */
#ifdef CMINETDBG                                     
            /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "SPutSBuf call 2 in BuildRecvHopOptsArr\n");
            CMINETLOGERROR(ERRCLS_DEBUG, ECMINET031, 0, prntBuf);
#endif /* CMINETDBG */            
            SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, 
                  (Data *)hbhOptsArr->hbhOpts, numOpts * sizeof(CmInetIpv6HBHHdr));
            hbhOptsArr->numHBHOpts = 0;
            hbhOptsArr->hbhOpts = NULLP;
            return (ROUTRES);
         }
         /* copy the value bytes */
         memcpy(hbhOptsArr->hbhOpts[optsIdx].value, 
               (cmsgData + curDataIdx),
               hbhOptsArr->hbhOpts[optsIdx].length);
         curDataIdx += hbhOptsArr->hbhOpts[optsIdx].length;      
      }  

      /* get next option */
      optsIdx++; 
   }
   return ROK;
} /* end of cmInet6BuildRecvHopOptsArr() */


/*
*
*      Fun:   cmInet6BuildRecvRtHdr
*
*      Desc:  This function fills up the Route Header in the cmInetIpv6HdrParm
*             from the recvd ancillary data from recvmsg system call.
*     
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: None
*
*      File:  cm_inet.c
*
*/

static S16 cmInet6BuildRecvRtHdr
(
uint8_t *cmsgData,              /* flat buffer where to build Route hdr */ 
uint32_t rtDataLen,             /* byte len of cmsghdr struc+rtHdr ancil data */
CmInetIpv6RtHdr0 *rtHdr0,  /* rtHeader0 struct that precedes IPV6 addrss */
CmInetIpv6RtHdr *rtOptsArr,/* IPv6 extensions headers */
CmInetMemInfo   *info      /* Memory information */
)
{
   uint32_t curDataIdx;         /* to keep track where we are in hbh Data */
   uint8_t i;                   /* loop counter */
   S16 ret;                /* temporary return value */

   
   /* byte len of actual rtHdr ancil data */
   rtDataLen -= sizeof(struct cmsghdr);

   /* start from beginning */
   curDataIdx = 0;                

   /* copy next header byte */
   rtHdr0->ip6r0_nextHdr = *(cmsgData + curDataIdx);
   curDataIdx += 1;  

   /* copy header extension length byte */
   rtHdr0->ip6r0_hdrExtLen = *(cmsgData + curDataIdx);
   curDataIdx += 1;

   /* copy type byte (always 0) */
   rtHdr0->ip6r0_type = 0x00;
   curDataIdx += 1;

   /* copy segment left byte */
   rtHdr0->ip6r0_segLeft = *(cmsgData + curDataIdx);
   curDataIdx += 1;

   /* copy 1 reserve byte + 3 strict/loose bytes */  
   memcpy((&rtOptsArr->slMap),
         (cmsgData + curDataIdx), 4);
   curDataIdx += 4;

   /* also save reserv byte + 3 sl bytes to rtHdro struc */
   rtHdr0->ip6r0_resrvAndSLmap = rtOptsArr->slMap;

   /* subtract 8 bytes for Next Hdr, Hdr Ext Len, .... + SL bit map */
   rtOptsArr->numAddrs = (rtDataLen - 8)/16;

   ret = SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, 
         (Data **)&rtOptsArr->ipv6Addrs, 
         (Size)rtOptsArr->numAddrs * 16);
   if (ret != ROK)
   {
#ifdef CMINETDBG      
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "SGetSBuf failure 1 in cmInet6BuildRecvRtHdr\n");
      CMINETLOGERROR(ERRCLS_ADD_RES, ECMINET032, 0, prntBuf);
#endif /* CMINETDBG */     
      return (ROUTRES);
   }

   /* copy all the ipv6 addresses */
   for(i=0; i < rtOptsArr->numAddrs; i++)
   {
      memcpy((rtOptsArr->ipv6Addrs[i]),
            (cmsgData + curDataIdx), 16);
      curDataIdx += 16;
   }

   return ROK;
} /* end of cmInet6BuildRecvRtHdr() */


/*
*
*      Fun:   cmInet6GetHopLimitValue
*
*      Desc:  This function extracts the hop limit value(ttl) of from the 
*             ancillary data received through recvmsg() call. Then this
*             hoplimit value will be passed to upper user as ipHdrParam.
*     
*      Ret:   ROK     - successful
*
*      Notes: None
*
*      File:  cm_inet.c
*
*/

static S16 cmInet6GetHopLimitValue
(
uint8_t *cmsgData,        /* flat buffer where to build ext hdrs */
uint32_t hopLimitDataLen, /* byte len of cmsghdr + hbh ancil data */
CmInetIpv6HdrParm *ipv6HdrParam /* ipv6 header parameters */ 
)
{
   uint16_t curDataIdx;       /* to keep track where we are in the ancillary Data */
   uint32_t *hopLimitValue;   /* ttl/hoplimit value */

   hopLimitValue = NULL;
   curDataIdx = 0;                

   /* get length of actual hbh ancillary data */
   hopLimitDataLen -= sizeof(struct cmsghdr);

   /* go to the first byte of hop limit which present after cmsghdr struc */
   curDataIdx += sizeof(struct cmsghdr);

   /* mark that hoplimit(ttl) is present */
   ipv6HdrParam->ttl.pres = TRUE;

   /* the first byte will be the HopLimit value */
   hopLimitValue = (uint32_t *)(cmsgData);
   ipv6HdrParam->ttl.val = (uint8_t)(*hopLimitValue);

   return ROK;
}
#endif /* IPV6_OPTS_SUPPORTED */


/*
*
*      Fun:   cmInetRecvMsg
*
*      Desc:  Reads data from a socket into a message. 
*             The buffers for the message  are allocated within the 
*             cmInetRead() function from the pool and region Id set in the 
*             info struct.  
*             If the number of octets given by the paramter len is not 
*             available the function immediately returns with RKDNA. 
*             If the len parameter is set to CM_INET_READ_ANY, the currently 
*             available data is read. 
*             Values for flag parameter:
*  
*             CM_INET_NO_FLAG  - no additional control flag
*             CM_INET_MSG_PEEK - do not destroy data on receive buffer
*
*      Ret:   ROK     - successful
*             ROKDNA  - ok, data not available
*             RCLOSED - connection closed by peer
*             ROUTRES - failed, out of resources
*             RFAILED - failed
*
*      Notes: None.
*
*      File:  cm_inet.c
*
*/
S16 cmInetRecvMsg
(
CmInetFd        *sockFd,        /* socket file descriptor */ 
CmInetAddr      *fromAddr,      /* sender Internet address/port */ 
CmInetMemInfo   *info,          /* buffer allocation info */
Buffer         **mPtr,          /* received buffer structure */
MsgLen          *len,           /* number of octets to read */ 
/*  added for IPv6 */
#ifdef IPV6_OPTS_SUPPORTED
CmInetIpHdrParm *ipHdrParams,    /* IPv6 extensions headers */
#endif /* IPV6_OPTS_SUPPORTED */
#ifdef LOCAL_INTF
CmInetLocalInf  *localIf,       /* local interface on which pkt was recvd */
#endif /* LOCAL_INTF */ 
S32              flags          /* additional control flags */
)
{
#if (defined(WIN32) || defined(CMINETFLATBUF))
   S32          ret = 0;            /* temporary return value */
   uint32_t     pendLen =0;        /* pending data length */
   S32          recvLen =0;        /* number of received octets by recvmsg() */
   MsgLen       bufLen =0;         /* entire number of received octets */
   MsgLen       curLen =0;         /* current number of octets in buffer */ 
   Data         *recvBuf =NULLP;        /* receive buffer */
   Data         *bufPtr =NULLP;         /* current buffer position */   
   Buffer       *mBuf = NULLP;           /* received message */ 
   uint32_t     remAddrLen =0;     /* length of remote address */
   struct sockaddr_in  *remAddr = {0};    /* remote Internet address */       
#ifdef IPV6_SUPPORTED 
   struct sockaddr_in6  *remAddr6 = {0};  /* remote Internet address */       
   struct sockaddr_in6 remSockAddr ={0}; /* to get packet's source IP address */
#else
   CmInetSockAddr  remSockAddr ={0};     /* to get packet's source IP address */
#endif /* IPV6_SUPPORTED */
#else
   S32           ret =0;            /* temporary return value */
  /* cm_inet_c_001.main_58: Fix for g++ compilation warning */
   uint16_t      i =0;              /* index */
   uint32_t      pendLen =0;        /* pending data length */
   S32           numBuf =0;         /* number of allocated dBufs */
   S32           recvLen =0;        /* number of received octets by recvmsg() */
   MsgLen        bufLen =0;         /* entire number of received octets */
   struct msghdr msg = {0};            /* message header */ 
   /* cm_inet_c_001.main_54: Fix for Klockworks issue */
   Buffer       *tempMsg = NULLP;        /* temporary message */
   CmInetIovec  rxArr[CM_INET_MAX_DBUF]= {{0}};  /* dynamic gather array */
   Buffer      **dBufs = NULLP;   /* dynamic array with allocated dBufs */
   S16           numDBufs =0;       /* number of allocated dBufs */

   /* cm_inet_c_001.main_55: As remAddrLen is only being used when
    * WIN32 or CMINETFLATBUF is defined, then Removed variable
    * in else part*/
   struct sockaddr_in *remAddr = {0};  /* remote Internet address */       
#ifdef IPV6_SUPPORTED 
   struct sockaddr_in6 *remAddr6 ={0};  /* remote Internet address */       
   struct sockaddr_in6 remSockAddr ={0};/* to get packet's source IP address */
   /* added for IPv6 ext headers support */
#ifdef IPV6_OPTS_SUPPORTED
   CmInetIpv6RtHdr0     rtHdr0 ={0};          /* type 0 route header */      
#endif /* IPV6_OPTS_SUPPORTED */

#ifdef LOCAL_INTF
   struct in6_pktinfo  *pkt6Info = {0};        /* IPv6 IP_PKTINFO */
#endif /* LOCAL_INTF */   

#if (defined(SS_LINUX) || defined(_XPG4_2))
   uint8_t                   ancillData[CM_INET_IPV6_ANCIL_DATA];
   /* from stack for IPv6 ancill data */
#endif
#else
   CmInetSockAddr       remSockAddr ={0};     /* to get packet's src IP address */
#if (defined(SS_LINUX) || defined(_XPG4_2))
   uint8_t                   ancillData[CM_INET_IPV4_ANCIL_DATA];
   /* from stack for IPv4 ancill data */
#endif
#endif /* IPV6_SUPPORTED */
   /* added new definitions */
   Bool                 allocFlatBuf;    /* allocate a flat buffer */
   /* cm_inet_c_001.main_54: Fix for Klockworks issue */
   Data                 *recvBuf = NULLP;        /* receive buffer */
#ifdef SS_LINUX
#ifdef LOCAL_INTF
   struct in_pktinfo    *pkt4Info;       /* IPv4 IP_PKTINFO */
#endif
#endif /* SS_LINUX */
#if (defined(IPV6_OPTS_SUPPORTED) || defined(LOCAL_INTF))   
   struct               cmsghdr *cmsgptr;/* pointer to struct cmsghdr */
#endif
#endif /* WIN32 | CMINETFLATBUF */
   /* used by getsockopt */

   /* cm_inet_c_001.main_55:Removed unused variables errValue and optLen */


#if (ERRCLASS & ERRCLS_INT_PAR)
      /* error check on parameters */
      if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd) ||
            (info == NULLP) || (mPtr == NULLP) || (len == NULLP))
      {
         return RFAILED;
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   *mPtr = NULLP;

   /*cm_inet_c_001.main_48 variables declaration */
#if !((defined(WIN32) || defined(CMINETFLATBUF)))
   numBuf = 0;
   numDBufs = 0;
#endif

#if (defined(WIN32) || defined(CMINETFLATBUF))
   remAddr = NULLP;  
#ifdef IPV6_SUPPORTED 
   remAddr6 = NULLP;
#endif /* IPV6_SUPPORTED */   
#else
#ifdef IPV6_SUPPORTED 
   remAddr = NULLP;
   remAddr6 = NULLP;
#endif /* IPV6_SUPPORTED */  

#if (defined(SS_LINUX) || defined(_XPG4_2))
   memset(ancillData, 0, sizeof(ancillData));
#endif /* SS_LINUX || _XPG4_2 */

#endif /* (WIN32 | CMINETFLATBUF) */

   /* clear the structure */   
   memset(&remSockAddr, 0, sizeof(remSockAddr));

   /* get number of pending data */
   /* removed 3rd arg memInfo. MemInfo is no longer
      needed as we call ioctl for all sockets */

   /* cm_inet_c_001.main_48 : call ioctl only for STREAM 
    * sockets now. For Non-Stream sockets(Raw & UDP), fix
    * pending length to CM_INET_MAX_UDPRAW_MSGSIZE
    */
   if(sockFd->type == CM_INET_STREAM)
   {
      ret = cmInetGetNumRead(sockFd, &pendLen);
      if (ret != ROK)
      {
         /* ret may be RFAILED or ROUTRES */
         return (ret);
      }
   }
   else 
   {  
      /* cm_inet_c_001.main_48 : pendLen is set 1 greater 
       * than the #defined value. If recvFrom/recvMsg 
       * returns the len == pendLen, we would drop the 
       * message as the msg len is larger than the largest 
       * msg we are willing to accept.
       */
      pendLen = CM_INET_MAX_UDPRAW_MSGSIZE+1;
   } 


   /* check if connection got closed */
   if (pendLen == 0)
   {
      if (sockFd->type == CM_INET_STREAM)
      {
         /* cm_inet_c_001.main_50: 
          * cm_inet_c_001.main_56: Removed comment for cm_inet_c_001.main_50 as
          * the current patch changes its functionality */
         uint8_t  readBuf[1]; /* declaration of variable for Peek */

         /* 
          * cm_inet_c_001.main_56:
          * We are peeking the socket buffer again with peek as on some machines
          * like solaris, there is a latency observed in ioctl. In such cases, 
          * ioctl may return 0, even though there are bytes available to read. 
          * We reconfirm through peek whether 0 means EOF or its ioctl latency
          * issue.
          */
         ret = cmInetPeekNew(sockFd, NULLP, info, 0, 1, readBuf);
         if (ret == RCLOSED)
         {
            return (ret);
         }
         /* cm_inet_c_001.main_56:
          * Returning ROKDNA even cmInetPeekNew returns ROK. Because currently
          * we are not sure about pending length. Anyway socket FD already set,
          * we do another iteration to get exact pendLen value. We cannot call 
          * cmInetGetNumRead at this point because of latency between the ioctl
          * call and recvfrom call issues on some machines ioctl call may 
          * return ZERO even their a data to read.  */
         return (ROKDNA);
      }
   } 
   /* cm_inet_c_001.main_52:  Support for partial reception */
   /* cm_inet_c_001.main_59: Fix for compilation warning */
   if ((sockFd->type == CM_INET_STREAM) && (*len > (MsgLen)pendLen)) 
   {
      /* cm_inet_c_001.main_54: Fix for Klockworks issue */
      *len = (MsgLen)pendLen;
   }

   /* check if there are enough pending data to read */
   if ((*len == CM_INET_READ_ANY) || ((uint32_t)*len <= pendLen))
   {
      if (*len == CM_INET_READ_ANY)
      {
         /* added check for TCP socket. Pending data length in 
            the socket recv buffer is determined by ioctl call in 
            cmInetGetNumRead. 
            For TCP it can't be > CM_INET_MAX_MSG_LEN. */
         if (sockFd->type == CM_INET_STREAM) 
         {
            /* max message length is limited to control the memory usage */
            if (pendLen > CM_INET_MAX_MSG_LEN)
               pendLen = CM_INET_MAX_MSG_LEN;
         }
         /* cm_inet_c_001.main_48 : removed the check for 
          * Non Stream sockets (pendLen < MAX_UDPRAW_MSGSIZE)
          * as we are hardcoding pendLen for Non-Stream sockets.
          */

         /* read all pending data */ 
         /* cm_inet_c_001.main_54: Fix for Klockworks issue */
         bufLen = (MsgLen)pendLen;
         *len = (MsgLen)pendLen; 
      }
      else
      {
         /*  cm_inet_c_001.main_45- Returning  CM_INET_MAX_MSG_LEN when input is larger than
          * this */
#ifdef LONG_MSG
         /* max message length is limited to control the memory usage */
         if ((*len) > CM_INET_MAX_MSG_LEN)
         {
            (*len) = CM_INET_MAX_MSG_LEN;
         }
#endif
         /* read data length given by user */ 
         bufLen = *len;
      }

#if (defined(WIN32) || defined(CMINETFLATBUF))

      /* set destination Internet address structure */
      if (fromAddr != NULLP)
      {
         remAddrLen = sizeof(remSockAddr); 
      }
      else
      {
         remAddrLen = 0;
      }

      /* allocate flat receive buffer */
      ret = SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, &recvBuf, bufLen);
      if (ret != ROK)
      {
         return (ROUTRES);
      }          
      curLen = bufLen;
      bufPtr = recvBuf;

      /* 
       * maybe needs more than one recvfrom() call to read an entire 
       * message from a stream socket (TCP)
       */
      while (curLen > 0)
      {
         /* added separate recvfrom calls different OS */

         /*cm_inet_c_001.main_42   1. In Vx-Works the 5th and 6th parameter of recvfrom
           system call are either NULL or should be valid pointer.*/
#if( defined(SS_VW) || defined(HPOS) || defined(SS_PS))
         if (remAddrLen)
            recvLen = recvfrom(sockFd->fd, (S8 *)bufPtr, curLen, 0, 
                  (struct sockaddr *)&remSockAddr, (int *)&remAddrLen);
         else
            recvLen = recvfrom(sockFd->fd, (S8 *)bufPtr, curLen, 0, 
                  NULLP, (int *)&remAddrLen);
#else         
#if ( defined(SUNOS) || defined(SS_LINUX))
         if (remAddrLen)
            recvLen = recvfrom(sockFd->fd, (S8 *)bufPtr, curLen, 0, 
                  (struct sockaddr *)&remSockAddr, (socklen_t *)&remAddrLen);
         else
            recvLen = recvfrom(sockFd->fd, (S8 *)bufPtr, curLen, 0, 
                  NULLP, (socklen_t *)&remAddrLen); 
#else
         if (remAddrLen)
            recvLen = recvfrom(sockFd->fd, (S8 *)bufPtr, curLen, 0, 
                  &remSockAddr, (S32 *)&remAddrLen);
         else
            recvLen = recvfrom(sockFd->fd, (S8 *)bufPtr, curLen, 0, 
                  NULLP, (S32 *)&remAddrLen); 

#endif /* defined(SUNOS) || defined(SS_LINUX) */
#endif /* defined(SS_VW) || defined(HPOS) || defined(SS_PS) */ 

         if (recvLen == INET_ERR)
         {
            /* cleanup */
            /* moved cleanup here */
            SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, recvBuf, bufLen); 

            /*  added check ERR_WOULDBLOCK */
            if ((INET_ERR_CODE == ERR_AGAIN) ||
                  (INET_ERR_CODE == ERR_WOULDBLOCK))
            {
               *len = 0;
               return (ROKDNA);
            }


            /*  In Windows the recvfrom function fails
             *  with error code which maps to either WSAECONNABORTED. If
             *  this happens then cmInetRecvMsg must return RCLOSED */
            if ((INET_ERR_CODE == ERR_CONNABORTED) || 
                  (INET_ERR_CODE == ERR_CONNRESET))
            {
               *len = 0;
               return (RCLOSED);
            }

#ifdef CMINETDBG
#ifndef ALIGN_64BIT
            /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetRecvMsg() Failed : error(%d),"
                  " sockFd->fd(%ld)\n", INET_ERR_CODE, sockFd->fd);
            CMINETLOGERROR(ERRCLS_DEBUG, ECMINET034, 0, prntBuf);
#else
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetRecvMsg() Failed : error(%d),"
                  " sockFd->fd(%d)\n", INET_ERR_CODE, sockFd->fd);
            CMINETLOGERROR(ERRCLS_DEBUG, ECMINET034, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */

            return RFAILED;
         } 
         curLen -= recvLen;
         bufPtr += recvLen;

         /* 
          * a message is always read atomically on a datagram socket,
          * therefore it's ok to read less than pending data!
          */
#ifdef CM_INET2  
         if ((sockFd->type == CM_INET_RAW) || 
               (sockFd->type == CM_INET_DGRAM))
         {
            *len = recvLen;
            break; 
         }
#else /* CM_INET2 */ 
         if (sockFd->type == CM_INET_DGRAM)
         {
            *len = recvLen;
            break; 
         }
#endif /* CM_INET2 */ 
      } /* while (curLen > 0) (only for stream sockets) */ 

      /* For UDP, it is possible to receive
       * a 0 byte datagram, in this case just return ROKDNA.
       */ 
#ifdef CM_INET2
      if ((sockFd->type == CM_INET_DGRAM || sockFd->type == CM_INET_RAW) 
            && (*len == 0))
#else
         if ((sockFd->type == CM_INET_DGRAM) && (*len == 0))
#endif
         {
            SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, recvBuf, bufLen);
            return (ROKDNA);
         }

      /* cm_inet_c_001.main_48 : If Received 
       * len == CM_INET_MAX_UDPRAW_MSGSIZE+1
       * Drop this message 
       */ 
#ifdef CM_INET2
      if ((sockFd->type == CM_INET_DGRAM || sockFd->type == CM_INET_RAW) 
            && (*len == (CM_INET_MAX_UDPRAW_MSGSIZE+1)))
#else
         if ((sockFd->type == CM_INET_DGRAM) 
               && (*len == (CM_INET_MAX_UDPRAW_MSGSIZE+1)))
#endif
         {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
            /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetRecvMsg(),recevied a message"
                  " > than allowed(%lu), sockFd->fd(%ld) \n", 
                  CM_INET_MAX_UDPRAW_MSGSIZE, sockFd->fd);
            CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET068, 0, prntBuf);
#else
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetRecvMsg(),recevied a message"
                  " > than allowed(%lu), sockFd->fd(%d) \n", 
                  CM_INET_MAX_UDPRAW_MSGSIZE, sockFd->fd);
            CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET068, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif
            SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, recvBuf, bufLen);
            return (ROKDNA);
         }

      /* cm_inet_c_001.main_48 : copy data to a message structure */
      ret = SGetMsg(info->region, info->pool, &mBuf);
      if (ret != ROK)
      {
         /* cleanup */
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, recvBuf, bufLen);       
         return (ret);
      }

#ifdef CM_INET2  
      if ((sockFd->type == CM_INET_DGRAM) ||
            (sockFd->type == CM_INET_RAW))
      {
         ret = SAddPstMsgMult(recvBuf, *len, mBuf);        
      }
      else
      {
         ret = SAddPstMsgMult(recvBuf, bufLen, mBuf);        
      }

#else /* CM_INET2 */ 
      if (sockFd->type == CM_INET_DGRAM)
      {
         ret = SAddPstMsgMult(recvBuf, *len, mBuf);        
      }
      else
      {
         ret = SAddPstMsgMult(recvBuf, bufLen, mBuf);        
      }
#endif /* CM_INET2 */ 

      if (ret != ROK)
      {
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, recvBuf, bufLen);    
         SPutMsg(mBuf); 
         return (ret);
      }
      *mPtr = mBuf;

      /* setup return destination Internet address */
      /* added the check of (remAddrLen > 0) */
      if ((fromAddr != NULLP) && (remAddrLen > 0))
      {
#ifdef IPV6_SUPPORTED
         if (remAddrLen == sizeof(struct sockaddr_in6))
         {
            remAddr6 = (struct sockaddr_in6 *)&remSockAddr;
            fromAddr->type = CM_INET_IPV6ADDR_TYPE;
            fromAddr->u.ipv6Addr.port = CM_INET_NTOH_UINT16(remAddr6->sin6_port);
            CM_INET_COPY_IPV6ADDR(&fromAddr->u.ipv6Addr.ipv6NetAddr, 
                  &remAddr6->sin6_addr);
         }
         else
         {
            remAddr = (struct sockaddr_in *)&remSockAddr;
            fromAddr->type = CM_INET_IPV4ADDR_TYPE;
            fromAddr->u.ipv4Addr.port = CM_INET_NTOH_UINT16(remAddr->sin_port);
            fromAddr->u.ipv4Addr.address = CM_INET_NTOH_UINT32(remAddr->sin_addr.s_addr);
         }
#else
         remAddr = (struct sockaddr_in *)&remSockAddr;
         fromAddr->port    = CM_INET_NTOH_UINT16(remAddr->sin_port);
         fromAddr->address = CM_INET_NTOH_UINT32(remAddr->sin_addr.s_addr);
#endif /* IPV6_SUPPORTED */
      }   

      /* cleanup */
      SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, recvBuf, bufLen);      

#else  /* end of Win NT/flat buffer specific part */

      /* Initialise variable */
      allocFlatBuf = FALSE;

      /* 
       * maybe needs more than one recvmsg() call to read entire message 
       * on a stream socket 
       */
      while (bufLen > 0)
      {
         /* allocate gather vector, it's a dynamic array */    
         numDBufs =  CM_INET_MAX_DBUF;

         ret = SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, (Data**)&dBufs, 
               numDBufs*sizeof(Buffer*));
         if (ret != ROK)
         {
            return (ROUTRES);
         }                     

         /* Allocate dBufs for gather read */ 
         /* allocate dBufs for gathering read */
         if (sockFd->type == CM_INET_STREAM)
            ret = buildRecvBuf(info, bufLen, rxArr, dBufs, numDBufs, &msg,
                  TRUE);
         else
            ret = buildRecvBuf(info, bufLen, rxArr, dBufs, numDBufs, &msg,
                  FALSE);
         if (ret != ROK)
         {
            /* check if the function returned RNA */ 
            if (ret == RNA)
            {
               /* Incase of UDP/RAW messages allocate a flat buffer. Incase
                * of TCP ignore this error condition. The user will call 
                * cmInetRecvMsg again */
               /* cm_inet_c_001.main_62:Warning fix */
               if (sockFd->type != (uint8_t)CM_INET_STREAM)/* G++ */
               {

#ifdef T2K_MEM_LEAK_DBG
		       char * file = __FILE__;
		       uint32_t  line   = __LINE__;
#endif

                  /* cleanup  the dBuf array */
                  for (i = 0; i < msg.msg_iovlen; i++)
                     SPutDBuf(info->region, info->pool, dBufs[i]);   

                  SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, (Data*)dBufs, 
                        numDBufs * sizeof(Buffer*)); 

                  /* allocate flat receive buffer */
                  ret = SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, &recvBuf, bufLen);
                  if (ret != ROK)
                     return (ROUTRES);

                  allocFlatBuf = TRUE;

                  /* update the message structure */
#ifdef SS_LINUX
                  rxArr[0].iov_base = (Void*)recvBuf;  
                  rxArr[0].iov_len = (uint32_t)bufLen;    
#else
                  rxArr[0].iov_base = (S8*)recvBuf;
                  rxArr[0].iov_len = bufLen;
#endif /* SS_LINUX */
                  msg.msg_iov           = rxArr;
                  msg.msg_iovlen        = 1;
               }
            }
            else
            {
               SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, (Data*)dBufs, 
                     numDBufs*sizeof(Buffer*)); 
               return (ret);
            }
         }

         numBuf =  msg.msg_iovlen;

         /* setup destination Internet address structure */
         if (fromAddr != NULLP)
         {
#ifdef SS_LINUX
            msg.msg_name    = (Void*)&remSockAddr;
#else
#ifdef SS_PS
            msg.msg_name    = (char *)&remSockAddr;
#else
            msg.msg_name    = (caddr_t)&remSockAddr;
#endif /* SS_PS */
#endif /* SS_LINUX */
            msg.msg_namelen = sizeof(remSockAddr);
         }
         else
         {
            msg.msg_name    = NULLP;
            msg.msg_namelen = 0;
         }

         /* added defined(_XPG4_2). Also changed the
          * assignments */
#if (defined(SS_LINUX) || defined(_XPG4_2))
         msg.msg_control      = ancillData;
         msg.msg_controllen   = sizeof(ancillData);
#else
         msg.msg_accrights     = NULLP;
         msg.msg_accrightslen  = 0;
#endif /* SS_LINUX */

         recvLen = recvmsg(sockFd->fd, &msg, flags);
         if ((recvLen == INET_ERR) || (recvLen > CM_INET_MAX_MSG_LEN))
         {
            /* Moved up the cleanup precedures here before returning */
            /* Cleanup flat buffer if allocated */
            if (allocFlatBuf)
               SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, recvBuf, bufLen);
            else
            {
               /* cleanup */
               for (i = 0; i < numBuf; i++)
               { 
                #ifdef T2K_MEM_LEAK_DBG
                   char * file = __FILE__;
                   uint32_t  line   = __LINE__;
                #endif

                  SPutDBuf(info->region, info->pool, dBufs[i]);   
               }
               SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, (Data*)dBufs, 
                     numDBufs*sizeof(Buffer*)); 
            }

            /* cm_inet_c_001.main_50 - Free the buffer only when valid, it might be that
             * it has partially received data
             */
            /* added check ERR_AGAIN when CMINETFLATBUF is not defined. 
               added check ERR_WOULDBLOCK */
            if ((INET_ERR_CODE == ERR_AGAIN) ||
                  (INET_ERR_CODE == ERR_WOULDBLOCK))
            {
               /* cm_inet_c_001.main_50 : If message is read partially then just return
                * OK without freeing the mPtr. This will gaurd us
                * against unexpected WOULDBLOCKS observed in solaris
                */
               if (*mPtr != NULLP)
                  return ROK;

               return (ROKDNA);
            }

            /* cm_inet_c_001.main_50 - Free the buffer only when valid, it might be that
             * it has partially received data
             */
            if (*mPtr != NULLP)
            {
               SPutMsg(*mPtr);
            }
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
            /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetRecvMsg() Failed : error(%d),"
                  " sockFd->fd(%ld)\n", INET_ERR_CODE, sockFd->fd);
            CMINETLOGERROR(ERRCLS_DEBUG, ECMINET035, 0, prntBuf);
#else
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetRecvMsg() Failed : error(%d),"
                  " sockFd->fd(%d)\n", INET_ERR_CODE, sockFd->fd);
            CMINETLOGERROR(ERRCLS_DEBUG, ECMINET035, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */

            /*  If this happens then cmInetRecvMsg must return RCLOSED. 
             *  Needed for getting icmp msgs */
            if (INET_ERR_CODE == ERR_CONNABORTED)
            {
               *len = 0;
               return (RCLOSED);
            }
            return RFAILED; 
         } 

         bufLen -= recvLen;

         /* added for IPv6 extn headers */
#if (defined(IPV6_OPTS_SUPPORTED) || defined(LOCAL_INTF))

         /* check if ancillary data has been received. 
          * Return the allocated memory when no ancillary data received */
#if (defined(SS_LINUX) || defined(_XPG4_2))         
         if (msg.msg_controllen)
         {   
            cmsgptr = CMSG_FIRSTHDR(&msg);
         }   
         else 
            cmsgptr = NULLP;
#else
         cmsgptr = NULLP;         
#endif  /* SS_LINUX || _XPG4_2 */        

         if (cmsgptr != NULLP) 
         {
#ifdef IPV6_OPTS_SUPPORTED            
            if(ipHdrParams != NULLP)
            {   
               ipHdrParams->u.ipv6HdrParm.ttl.pres = FALSE;
               ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.hbhHdrPrsnt = FALSE;
               ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.destOptsPrsnt = FALSE;
               ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.rtOptsPrsnt = FALSE;

               /* get all ancillary data objects recvd one by one */
               for (cmsgptr = CMSG_FIRSTHDR(&msg); cmsgptr != NULLP; 
                     cmsgptr = CMSG_NXTHDR(&msg, cmsgptr))
               {
                  if (cmsgptr->cmsg_level == IPPROTO_IPV6)
                  {
                     /* Initialise ipHdrParams properly */
                     ipHdrParams->type = CM_INET_IPV6ADDR_TYPE;   

                     if (cmsgptr->cmsg_type == IPV6_HOPOPTS) 
                     {
                        /* build up HBH opt array from recvd ancillary data */
                        ret = cmInet6BuildRecvHopOptsArr(
                              (uint8_t *)CMSG_DATA(cmsgptr), cmsgptr->cmsg_len, 
                              &ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.hbhOptsArr,
                              0, info);
                        if (ret != ROK)
                           return (ret);
                        ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.hbhHdrPrsnt = 
                           TRUE; 
                     }
#ifdef SS_LINUX
                     else if(cmsgptr->cmsg_type == IPV6_DSTOPTS)
#else
                     else if ((cmsgptr->cmsg_type == IPV6_DSTOPTS) ||
                           (cmsgptr->cmsg_type == IPV6_RTHDRDSTOPTS))
#endif /* SS_LINUX */  
                     {
                        /* build up Dest opt array from recvd ancillary data */
                        ret = cmInet6BuildRecvDstOptsArr(
                              (uint8_t *)CMSG_DATA(cmsgptr), cmsgptr->cmsg_len, 
                              &ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.destOptsArr,
                              1, info); 
                        if (ret != ROK)
                           return (ret);
                        ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.destOptsPrsnt = 
                           TRUE;
                     }
                     else if (cmsgptr->cmsg_type == IPV6_RTHDR)
                     {
                        /* build up Route Hdr from recvd ancillary data */
                        ret = cmInet6BuildRecvRtHdr(
                              (uint8_t *)CMSG_DATA(cmsgptr), cmsgptr->cmsg_len, &rtHdr0,
                              &ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.rtOptsArr, 
                              info);
                        if (ret != ROK)
                           return (ret);
                        ipHdrParams->u.ipv6HdrParm.ipv6ExtHdr.rtOptsPrsnt = 
                           TRUE; 
                     }
                     else if(cmsgptr->cmsg_type == IPV6_HOPLIMIT)
                     {
                        /* get the received hoplimit */
                        ret = cmInet6GetHopLimitValue((uint8_t *)CMSG_DATA(cmsgptr),
                              cmsgptr->cmsg_len, &ipHdrParams->u.ipv6HdrParm);
                        if (ret != ROK)
                           return (ret);
                     }
                  }
               }  /* for */            
            } /* ipHdrParams */
#endif /* IPV6_OPTS_SUPPORTED */

#ifdef IPV6_SUPPORTED
#ifdef LOCAL_INTF 
            for (cmsgptr = CMSG_FIRSTHDR(&msg); cmsgptr != NULLP; 
                  cmsgptr = CMSG_NXTHDR(&msg, cmsgptr))
            {   
               if(cmsgptr->cmsg_type == IPV6_PKTINFO)
               {
                  pkt6Info = (struct in6_pktinfo *)CMSG_DATA(cmsgptr);
                  localIf->intfPrsnt = TRUE;
                  localIf->localIf = pkt6Info->ipi6_ifindex;
                  localIf->localIfAddr.type =  CM_INET_IPV6ADDR_TYPE;
                  memcpy(&localIf->localIfAddr.u.ipv6NetAddr,
                        &pkt6Info->ipi6_addr, 16);
               }
            }   
#endif /* LOCAL_INTF */
#endif            

#if (defined(SS_LINUX) && defined(LOCAL_INTF))
#ifdef IPV6_SUPPORTED        
            if(sockFd->protType == AF_INET) 
            { 
#endif               
               for (cmsgptr = CMSG_FIRSTHDR(&msg); cmsgptr != NULL; 
                     cmsgptr = CMSG_NXTHDR(&msg, cmsgptr))
               {
                  if (cmsgptr->cmsg_level == IPPROTO_IP && 
                        cmsgptr->cmsg_type == IP_PKTINFO)
                  {
                     pkt4Info = (struct in_pktinfo *)CMSG_DATA(cmsgptr);
                     localIf->intfPrsnt = TRUE;
                     localIf->localIf = pkt4Info->ipi_ifindex;
                     localIf->localIfAddr.type =  CM_INET_IPV4ADDR_TYPE;
                     localIf->localIfAddr.u.ipv4NetAddr = 
                        ntohl(*(int *)&pkt4Info->ipi_addr);     
                  }
               }
#ifdef IPV6_SUPPORTED               
            }
#endif 
#endif /* SS_LINUX */ 
         }
#endif /* IPV6_OPTS_SUPPORTED || LOCAL_INTF */

         /* setup return destination Internet address */
         if (fromAddr != NULLP)
         {
#ifdef IPV6_SUPPORTED
            if (msg.msg_namelen == sizeof(struct sockaddr_in6))
            {
               remAddr6 = (struct sockaddr_in6 *)&remSockAddr;
               fromAddr->type = CM_INET_IPV6ADDR_TYPE;
               fromAddr->u.ipv6Addr.port = 
                  CM_INET_NTOH_UINT16(remAddr6->sin6_port);
               CM_INET_COPY_IPV6ADDR(&fromAddr->u.ipv6Addr.ipv6NetAddr, 
                     &remAddr6->sin6_addr);
            }
            else
            {
               remAddr = (struct sockaddr_in *)&remSockAddr;
               fromAddr->type = CM_INET_IPV4ADDR_TYPE;
               fromAddr->u.ipv4Addr.port = CM_INET_NTOH_UINT16(remAddr->sin_port);
               fromAddr->u.ipv4Addr.address = 
                  CM_INET_NTOH_UINT32(remAddr->sin_addr.s_addr);
            }
#else
            remAddr = (struct sockaddr_in *)&remSockAddr;
            fromAddr->port    = CM_INET_NTOH_UINT16(remAddr->sin_port);
            fromAddr->address = CM_INET_NTOH_UINT32(remAddr->sin_addr.s_addr);
#endif /* IPV6_SUPPORTED */
         }

         /* Incase a flat buffer was allocated get
          * a message to pass up */
         if (allocFlatBuf)
         {
            bufLen += recvLen;

            /* Get a message */
            ret = SGetMsg(info->region, info->pool, &tempMsg);
            if (ret != ROK)
            {
               /* cleanup */
               SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, recvBuf, bufLen);       
               return (ret);
            }

            /* cm_inet_c_001.main_48 : A 0 len UDP packet could be received */
            if ( recvLen > 0)
            {
               ret = SAddPstMsgMult(recvBuf, recvLen, tempMsg);        
               if (ret != ROK)
               {
                  SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, recvBuf, bufLen);    
                  SPutMsg(tempMsg); 
                  return (ret);
               }
            }

            *mPtr = tempMsg;

            SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, recvBuf, bufLen);    
            /* cm_inet_c_001.main_48 :flat buffers are allocated 
             * for non -TCP sockets. On these sockets we can receive 
             * only one message at a time 
             */
            /* cm_inet_c_001.main_54: Fix for Klockworks issue */
            *len = (MsgLen)recvLen;
            break;
         }
         else
         {
            /* build message out of dBufs */
            ret = buildRecvMsg(info, rxArr, numBuf, recvLen, dBufs, &tempMsg);
            if (ret != ROK)
            {
               /* Deallocate previously allocated
                * mBuf */
               if (*mPtr != NULLP)
                  SPutMsg(*mPtr);
               SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, (Data*)dBufs, 
                     numDBufs*sizeof(Buffer*)); 
               return (ret);
            }
         }

         if (*mPtr == NULLP)
         {
            /* it's first recvmsg() call */ 
            *mPtr = tempMsg;
         }
         else
         {
            /* concatenate messages */  
            ret = SCatMsg(*mPtr, tempMsg, M1M2);
            if (ret != ROK)
            {
               /* cleanup */
               SPutMsg(*mPtr);
               SPutMsg(tempMsg);
               SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, (Data*)dBufs, 
                     numDBufs*sizeof(Buffer*)); 
               return RFAILED;
            }
            SPutMsg(tempMsg);
         }

         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, (Data*)dBufs, 
               numDBufs*sizeof(Buffer*)); 

         /* 
          * a message is always read atomically on a datagram socket,
          * therefore it's ok to read less than pending data!
          */
#ifdef CM_INET2  
         if ((sockFd->type == CM_INET_DGRAM) ||
               (sockFd->type == CM_INET_RAW))
         {
            /* cm_inet_c_001.main_54: Fix for Klockworks issue */
            *len = (MsgLen)recvLen;
            break; 
         }
#else /* CM_INET2 */ 
         if (sockFd->type == CM_INET_DGRAM)
         {
            /* cm_inet_c_001.main_54: Fix for Klockworks issue */
            *len = (MsgLen)recvLen;
            break; 
         }
#endif /* CM_INET2 */ 
      } /* while(bufLen > 0) (only for stream sockets) */

      /* cm_inet_c_001.main_48 : For UDP, it is possible to receive
       * a 0 byte datagram, in this case just return ROKDNA 
       */

#ifdef CM_INET2
      if ((sockFd->type == CM_INET_DGRAM || sockFd->type == CM_INET_RAW)
            && (*len == 0))
#else
         if ((sockFd->type == CM_INET_DGRAM) && (*len == 0))
#endif
         {
            *len = 0;
            if (*mPtr != NULLP)
            {
               SPutMsg(*mPtr);
            }
            return (ROKDNA);

         }

      /* Received len == CM_INET_MAX_UDPRAW_MSGSIZE+1
       * Drop this message 
       */

#ifdef CM_INET2
      if ((sockFd->type == CM_INET_DGRAM || sockFd->type == CM_INET_RAW)
            && (*len == (CM_INET_MAX_UDPRAW_MSGSIZE+1)))
#else
         if ((sockFd->type == CM_INET_DGRAM) 
               && (*len == (CM_INET_MAX_UDPRAW_MSGSIZE+1)))
#endif
         {
            *len = 0;
            if (*mPtr != NULLP)
            {
               SPutMsg(*mPtr);
            }

#ifdef CMINETDBG
#ifndef ALIGN_64BIT
            /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetRecvMsg() recevied a message > than"
                  " allowed(%d),sockFd->fd(%ld)\n", 
                  CM_INET_MAX_UDPRAW_MSGSIZE, sockFd->fd);
            CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET069, 0, prntBuf);
#else
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetRecvMsg() recevied a message > than"
                  " allowed(%d),sockFd->fd(%d)\n", 
                  CM_INET_MAX_UDPRAW_MSGSIZE, sockFd->fd);
            CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET069, 0, prntBuf);
#endif
#endif
            return (ROKDNA);

         }

#endif /* WIN32 | CMINETFLATBUF  */
   }
   else
   {
      /* not enough data pending yet */
      return (ROKDNA);
   }

   return ROK;
} /* end of cmInetRecvMsg */


/* cm_inet_c_001.main_56: Added new function cmInetPeekNew() */

/*
 *
 *      Fun:   cmInetPeekNew
 *
 *      Desc:  Reads some data from the socket without destroying the socket
 *             receive buffer.
 *             The data is specified by the byte positon (first byte is at
 *             position 0) and the length.  
 *
 *      Ret:   ROK     - successful
 *             ROKDNA  - ok, data not available
 *             RCLOSED - connection closed by peer
 *             RFAILED - failed
 *
 *      Notes: Following are the differences from the cmInetPeek to cmInetPeekNew.
 *       This primitive does not call the select function as this is already
 *       taken care by the called primitive. This primitive will not use any 
 *       ioctl calls, because on some machines due to latency in ioctl call 
 *       length may return as ZERO, even there is some data to be read from 
 *       the socket and this primitive only peek buffer using recvfrom. 
 *       
 *       Caller of this function need to allocate the sufficient memory to hold
 *       the data peeked from the socket i.e. dataPos + dataLen. Socket data 
 *       will be copied in the "data" starting from dataPos offset.
 *
 *       For example, caller passed the following values to this function. 
 *       dataPos = 2 and dataLen = 10,then size of data buffer received should
 *       be minimum of (dataPos + dataLen)12 bytes and socket data will be 
 *       copied in the data buffer from offset 2 (dataPos) onwards.   
 *
 *             
 *      File:  cm_inet.c
 */

S16 cmInetPeekNew
(
CmInetFd        *sockFd,        /* socket file descriptor */ 
CmInetAddr      *fromAddr,      /* sender Internet address/port */ 
CmInetMemInfo   *info,          /* buffer allocation info */
MsgLen           dataPos,       /* position of data */
MsgLen           dataLen,       /* length of read data */
Data            *data           /* read data */
)
{
   /* cm_inet_c_001.main_57 - Fix for validation and compilation warning */
   S32          recvLen;           /* number of received octets */
   S32          remAddrLen;        /* length of remote address length */
   struct sockaddr_in  *remAddr;    /* remote Internet address */      
#ifdef IPV6_SUPPORTED
   struct sockaddr_in6  *remAddr6;  /* remote Internet IPV6 address */      
   struct sockaddr_in6 remSockAddr; /* to get packet's source IP address */
#else
   CmInetSockAddr  remSockAddr;     /* to get packet's source IP address */
#endif /* IPV6_SUPPORTED */


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd) ||
         (info == NULLP) || (data == NULLP) ||
         (dataPos < 0) || (dataLen < 0))
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   /* check if fromAddr is present or not */
   if (fromAddr != NULLP)
   {
      remAddrLen = sizeof(remSockAddr); 
   }
   else
   {
      remAddrLen = 0;
   }

   /* added different recvfrom calls with different 6th arg for 
    * different OS If remAddrLen is 0, pass NULLP */
#if( defined(SS_VW) || defined(HPOS) || defined(SS_PS))
   if(remAddrLen)
      recvLen = recvfrom(sockFd->fd,(S8*)(data + dataPos), (dataLen), 
            CM_INET_MSG_PEEK, &remSockAddr, (int*)&remAddrLen);
   else
      recvLen = recvfrom(sockFd->fd,(S8*)(data + dataPos), (dataLen), 
            CM_INET_MSG_PEEK, NULLP, (int*)&remAddrLen);
#else
#if ( defined(SUNOS) || defined(SS_LINUX))
   if(remAddrLen)
      recvLen = recvfrom(sockFd->fd, (S8*)(data + dataPos),(dataLen), 
            CM_INET_MSG_PEEK, (struct sockaddr *)&remSockAddr, 
            (socklen_t *)&remAddrLen);
   else
      recvLen = recvfrom(sockFd->fd, (S8*)(data + dataPos),(dataLen), 
            CM_INET_MSG_PEEK, NULLP, (socklen_t *)&remAddrLen);
#else
   if(remAddrLen)
      recvLen = recvfrom(sockFd->fd,(S8*)(data + dataPos), (dataLen), 
            CM_INET_MSG_PEEK, &remSockAddr, (S32*)&remAddrLen);
   else
      recvLen = recvfrom(sockFd->fd,(S8*)(data + dataPos), (dataLen), 
            CM_INET_MSG_PEEK, NULLP, (S32*)&remAddrLen);
#endif /* defined(SUNOS) || defined(SS_LINUX) */
#endif /* defined(SS_VW) || defined(HPOS) || defined(SS_PS) */

   /* removed the check of returned remAddrLen */ 
   if (recvLen == INET_ERR)
   {
      /* added check ERR_WOULDBLOCK */
      if ((INET_ERR_CODE == ERR_AGAIN) ||
            (INET_ERR_CODE == ERR_WOULDBLOCK))
      {
         recvLen = 0;
         return (ROKDNA);
      }
      /* cm_inet_c_001.main_61: added host unreachable check */
      if ((INET_ERR_CODE == ERR_CONNABORTED) ||
            (INET_ERR_CODE == ERR_CONNRESET) ||
            (INET_ERR_CODE == ERR_HOSTUNREACH) ||
            (INET_ERR_CODE == ERR_CONNREFUSED))
      {
         recvLen = 0;
         return (RCLOSED);
      }
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetPeekNew() Failed : error(%d), sockFd->fd(%ld)\n", 
            INET_ERR_CODE, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET070, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetPeekNew() Failed : error(%d), sockFd->fd(%d)\n", 
            INET_ERR_CODE, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET070, 0, prntBuf);
#endif
#endif /* CMINETDBG */

      return RFAILED;
   } 
   else if (recvLen == 0)
   {
      return (RCLOSED);
   }

   /* cm_inet_c_001.main_57 - Fix for validation */
   if (recvLen < (S32)dataLen)  /* maybe happen */
   {
      return (ROKDNA);
   } 

   /* setup return destination Internet address */
   /* added the check of (remAddLen > 0) */
   if ((fromAddr != NULLP) && (remAddrLen > 0))
   {
#ifdef IPV6_SUPPORTED
      memset(fromAddr, 0, sizeof(fromAddr));
      if (remAddrLen == sizeof(struct sockaddr_in6))
      {
         remAddr6 = (struct sockaddr_in6 *)&remSockAddr;
         fromAddr->type = CM_INET_IPV6ADDR_TYPE;
         fromAddr->u.ipv6Addr.port = 
            CM_INET_NTOH_UINT16(remAddr6->sin6_port);
         CM_INET_COPY_IPV6ADDR(&fromAddr->u.ipv6Addr.ipv6NetAddr, 
               &remAddr6->sin6_addr);
      }
      else
      {
         remAddr = (struct sockaddr_in *)&remSockAddr;
         fromAddr->type = CM_INET_IPV4ADDR_TYPE;
         fromAddr->u.ipv4Addr.port = CM_INET_NTOH_UINT16(remAddr->sin_port);
         fromAddr->u.ipv4Addr.address = 
            CM_INET_NTOH_UINT32(remAddr->sin_addr.s_addr);
      } 
#else
      remAddr = (struct sockaddr_in *)&remSockAddr;
      fromAddr->port    = CM_INET_NTOH_UINT16(remAddr->sin_port);
      fromAddr->address = CM_INET_NTOH_UINT32(remAddr->sin_addr.s_addr);
#endif /* IPV6_SUPPORTED */
   }

   return ROK;
} /* end of cmInetPeeknew */


/*
*
*      Fun:   cmInetPeek
*
*      Desc:  Reads some data from the socket without destroying the socket
*             receive buffer.
*             The data is specified by the byte positon (first byte is at
*             position 0) and the length.  
*     
*      Ret:   ROK     - successful
*             ROKDNA  - ok, data not available
*             RCLOSED - connection closed by peer
*             RFAILED - failed
*
*      Notes: None.
*
*      File:  cm_inet.c
*
*/

S16 cmInetPeek
(
CmInetFd        *sockFd,        /* socket file descriptor */ 
CmInetAddr      *fromAddr,      /* sender Internet address/port */ 
CmInetMemInfo   *info,          /* buffer allocation info */
MsgLen           dataPos,       /* position of data */
MsgLen           dataLen,       /* length of read data */
Data            *data           /* read data */
)
{
   /* cm_inet_c_001.main_54: Fix for Klockworks issue */
   Data        *recvBuf = NULLP;   /* plain receive buffer */
   /* cm_inet_c_001.main_47: 102069 Changed from S32 to MsgLen for bufLen*/
   MsgLen       bufLen;            /* buffer length */ 
   MsgLen       i;                 /* index */
   MsgLen       j;                 /* index */
   S32          ret;               /* temporary return value */
   uint32_t          timeout;           /* timeout for cmInetSelect() */ 
   uint32_t         *timeoutPtr;        /* pointer to timeout */
   S16          numFdS;            /* number of ready descriptors */
   /* cm_inet_c_001.main_45 - fixing the UMR issue in 64bit linux */
   uint32_t          pendLen = 0;           /* pending data length */
   S32          recvLen;           /* number of received octets */
   S32          remAddrLen;        /* length of remote address length */
   CmInetFdSet  readFdS;           /* socket file descriptor set */
   struct sockaddr_in  *remAddr;    /* remote Internet address */      
#ifdef IPV6_SUPPORTED
   struct sockaddr_in6  *remAddr6;  /* remote Internet IPV6 address */      
   struct sockaddr_in6 remSockAddr; /* to get packet's source IP address */
#else
   CmInetSockAddr  remSockAddr;     /* to get packet's source IP address */
#endif /* IPV6_SUPPORTED */


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd) ||
         (info == NULLP) || (data == NULLP) ||
         (dataPos < 0) || (dataLen < 0))
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   /* check if there are some datas */
   if (sockFd->blocking) 
   {
      /* blocking */ 
      timeoutPtr = NULLP;  
   } 
   else 
   {
      /* poll (non-blocking) */ 
      timeout = 0;
      timeoutPtr = &timeout;
   }
   CM_INET_FD_ZERO(&readFdS);
   CM_INET_FD_SET(sockFd, &readFdS);

   ret = cmInetSelect(&readFdS, NULLP, timeoutPtr, &numFdS);
   if (CM_INET_FD_ISSET(sockFd, &readFdS))
   {
      /* get number of pending data */
      /* removed 3rd arg memInfo. MemInfo is no longer needed as we 
         call ioctl for all sockets */
      ret = cmInetGetNumRead(sockFd, &pendLen);
      if (ret != ROK)
      {
         /* cm_inet_c_001.main_50
          * Return RCLOSED if cmInetGetNumRead returns RCLOSED. For other
          * errors just return RFAILED.
          */
         if (ret == RCLOSED)
            return (RCLOSED);

         return RFAILED;
      }

      /* check if connection got closed */
      if (pendLen == 0)
      {

         /* cm_inet_c_001.main_50 
          * Due to latency (mostly in solaris) sometimes ioctl(FIONREAD)
          * (inside cmInetGetNumRead) returns pend length as 0 on a TCP 
          * socket that select says is ready to read. This should not be 
          * considered as connection closed. So return ROKDNA instead of 
          * RCLOSED even for TCP sockets
          */
         return (ROKDNA);
      }
      /* added check for TCP/UDP socket. Pending data len in the socket 
         recv buffer is determined by ioctl call in cmInetGetNumRead. 
         For TCP it can't be > CM_INET_MAX_MSG_LEN. 
         For UDP it can't be > CM_INET_MAX_UDPRAW_MSGSIZE. */ 
      if (sockFd->type == CM_INET_STREAM) 
      {
         /* max message length is limited to control the memory usage */
         if (pendLen > CM_INET_MAX_MSG_LEN)
            pendLen = CM_INET_MAX_MSG_LEN;
         /* In STREAM remote address is not required */
         remAddrLen = 0;
      }
      else
      {
         if (pendLen > CM_INET_MAX_UDPRAW_MSGSIZE)
            pendLen = CM_INET_MAX_UDPRAW_MSGSIZE;

         remAddrLen = sizeof(CmInetSockAddr);
      }

      /* check if there are enough pending data to read */
      bufLen = dataPos + dataLen;

      /* check if fromAddr is present or not */
      if (fromAddr != NULLP)
      {
         remAddrLen = sizeof(remSockAddr); 
      }
      else
      {
         remAddrLen = 0;
      }

      /* cm_inet_c_001.main_58: Fix for g++ compilation warning */
      if ((MsgLen)pendLen >= bufLen)
      {        
         /* allocate receive buffer (flat structure) */
         ret = SGetSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, &recvBuf, bufLen);                  
         if (ret != ROK)
         {
            return (ROUTRES);
         }          

         /* added different recvfrom calls with 
          * different 6th arg for different OS */

         /* If remAddrLen is 0, pass NULLP */
#if( defined(SS_VW) || defined(HPOS) || defined(SS_PS))
         if(remAddrLen)
            recvLen = recvfrom(sockFd->fd,(S8*)recvBuf, bufLen, 
                  CM_INET_MSG_PEEK, &remSockAddr, (int*)&remAddrLen);
         else
            recvLen = recvfrom(sockFd->fd,(S8*)recvBuf, bufLen, 
                  CM_INET_MSG_PEEK, NULLP, (int*)&remAddrLen);
#else
#if ( defined(SUNOS) || defined(SS_LINUX))
         if(remAddrLen)
            recvLen = recvfrom(sockFd->fd, (S8*)recvBuf,bufLen, 
                  CM_INET_MSG_PEEK, (struct sockaddr *)&remSockAddr, 
                  (socklen_t *)&remAddrLen);
         else
            recvLen = recvfrom(sockFd->fd, (S8*)recvBuf,bufLen, 
                  CM_INET_MSG_PEEK, NULLP, (socklen_t *)&remAddrLen);
#else         
         if(remAddrLen)
            recvLen = recvfrom(sockFd->fd,(S8*)recvBuf, bufLen, 
                  CM_INET_MSG_PEEK, &remSockAddr, (S32*)&remAddrLen);
         else
            recvLen = recvfrom(sockFd->fd,(S8*)recvBuf, bufLen, 
                  CM_INET_MSG_PEEK, NULLP, (S32*)&remAddrLen);
#endif /* defined(SUNOS) || defined(SS_LINUX) */
#endif /* defined(SS_VW) || defined(HPOS) || defined(SS_PS) */

         /* removed the check of returned remAddrLen */ 
         if (recvLen == INET_ERR)
         {
            /* cleanup */
            /* moved cleanup here */
            SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, recvBuf, bufLen); 

            /* added check ERR_WOULDBLOCK */
            if ((INET_ERR_CODE == ERR_AGAIN) ||
                  (INET_ERR_CODE == ERR_WOULDBLOCK))
            {
               recvLen = 0;
               return (ROKDNA);
            }

            /* moved up the cleanup */

#ifdef CMINETDBG
#ifndef ALIGN_64BIT
            /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetPeek() Failed : error(%d), sockFd->fd(%ld)\n", 
                  INET_ERR_CODE, sockFd->fd);
            CMINETLOGERROR(ERRCLS_DEBUG, ECMINET036, 0, prntBuf);
#else
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetPeek() Failed : error(%d), sockFd->fd(%d)\n", 
                  INET_ERR_CODE, sockFd->fd);
            CMINETLOGERROR(ERRCLS_DEBUG, ECMINET036, 0, prntBuf);
#endif
#endif /* CMINETDBG */

            if ((INET_ERR_CODE == ERR_CONNABORTED) ||
                  (INET_ERR_CODE == ERR_CONNRESET))
            {
               recvLen = 0;
               return (RCLOSED);
            }
            return RFAILED;
         } 

         if (recvLen < (S32)bufLen)  /* maybe happen */
         {
            /* cleanup */
            SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, recvBuf, bufLen);                            
            return (ROKDNA);
         } 

         /* copy data */
         for (j = 0, i = dataPos; i < bufLen; j++, i++)
            data[j] = recvBuf[i];             

         /* setup return destination Internet address */
         /* added the check of (remAddLen > 0) */
         if ((fromAddr != NULLP) && (remAddrLen > 0))
         {
#ifdef IPV6_SUPPORTED
            memset(fromAddr, 0, sizeof(fromAddr));
            if (remAddrLen == sizeof(struct sockaddr_in6))
            {
               remAddr6 = (struct sockaddr_in6 *)&remSockAddr;
               fromAddr->type = CM_INET_IPV6ADDR_TYPE;
               fromAddr->u.ipv6Addr.port = 
                  CM_INET_NTOH_UINT16(remAddr6->sin6_port);
               CM_INET_COPY_IPV6ADDR(&fromAddr->u.ipv6Addr.ipv6NetAddr, 
                     &remAddr6->sin6_addr);
            }
            else
            {
               remAddr = (struct sockaddr_in *)&remSockAddr;
               fromAddr->type = CM_INET_IPV4ADDR_TYPE;
               fromAddr->u.ipv4Addr.port = CM_INET_NTOH_UINT16(remAddr->sin_port);
               fromAddr->u.ipv4Addr.address = 
                  CM_INET_NTOH_UINT32(remAddr->sin_addr.s_addr);
            } 
#else
            remAddr = (struct sockaddr_in *)&remSockAddr;
            fromAddr->port    = CM_INET_NTOH_UINT16(remAddr->sin_port);
            fromAddr->address = CM_INET_NTOH_UINT32(remAddr->sin_addr.s_addr);
#endif /* IPV6_SUPPORTED */
         }   

         /* cleanup */
         SPutSBufNewForDebug(__FILE__,__FUNCTION__,__LINE__,info->region, info->pool, recvBuf, bufLen);                            
      }
      else
      {
         /* not enough data pending yet */
         return (ROKDNA);
      }
   }
   else
   {
      /* no data pending */ 
      return (ROKDNA);
   }   

   return ROK;
} /* end of cmInetPeek */


/*
*
*      Fun:   cmInetClose 
*
*      Desc:  Close a socket gracefully. 
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: None.
*
*      File:  cm_inet.c
*
*/

S16 cmInetClose
(
CmInetFd *sockFd                /* socket file descriptor */
)
{
   S32 ret;                     /* temporary return value */


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd))
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

#ifdef WIN32
   ret = closesocket(sockFd->fd);
#else
   ret = close(sockFd->fd);
#endif /* WIN32 */
   if (ret == INET_ERR) 
   {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetClose() Failed : error(%d), sockFd->fd(%ld)\n",
            INET_ERR_CODE, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET037, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetClose() Failed : error(%d), sockFd->fd(%d)\n",
            INET_ERR_CODE, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET037, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
      return RFAILED;
   }

   return ROK;
} /* end of cmInetClose */


/*
*
*      Fun:   cmInetShutdown
*
*      Desc:  Close an Internet connection with more control over the data of 
*             the full-duplex connection.
*             Values for the howTo parameter:
*
*             CM_INET_SHTDWN_RECV - discard data in receive buffer
*             CM_INET_SHTDWN_SEND - discard data in transmit buffer
*             CM_INET_SHTDWN_BOTH - discard data in receive and transmit buffer      
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: This function does not free the socket descriptor but only closes the 
*             connection (cmInetClose() has to be called afterwards).
*             No error is returned if the socket is not connected while calling
*             this function. 
*
*      File:  cm_inet.c
*
*/

S16 cmInetShutdown
(
CmInetFd *sockFd,               /* socket file descriptor */
S32       howTo                 /* operation flag */
)
{
   S32 ret;                     /* temporary return value */


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd))
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   ret = shutdown(sockFd->fd, howTo);
   if (ret == INET_ERR)
   {
      if (INET_ERR_CODE == ERR_NOTCONN)
      {
         /* socket is not connected */ 
         return ROK; 
      }
      else
      {
         /* real problem */ 
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
         /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetShutdown() Failed : error(%d),"
               " sockFd->fd(%ld)\n", INET_ERR_CODE, sockFd->fd);
         CMINETLOGERROR(ERRCLS_DEBUG, ECMINET038, 0, prntBuf);
#else
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetShutdown() Failed : error(%d),"
               " sockFd->fd(%d)\n", INET_ERR_CODE, sockFd->fd);
         CMINETLOGERROR(ERRCLS_DEBUG, ECMINET038, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
         return RFAILED;
      }
   }   

   return ROK;
} /* end of cmInetShutdown */


/*
*
*      Fun:   cmInetSelect   
*
*      Desc:  Allows multiplex i/o requests among multiple sockets.
*             If the parameter mSecTimeout points to a value of zero the 
*             call immediatley returns (poll), if it is a null pointer, the
*             timeout is set to infinit.
*             numFdS returns the number of ready file  descriptors  contained  
*             in  the  file  descriptor  sets 
*
*      Ret:   ROK      - successful
*             RTIMEOUT - timout expired
*             RFAILED  - failed
*
*      Notes: None.
*
*      File:  cm_inet.c
*
*/

S16 cmInetSelect
(
CmInetFdSet *readFdS,           /* read socket descriptor file set */  
CmInetFdSet *writeFdS,          /* write socket descriptor file set */
uint32_t    *mSecTimeout,       /* timeout in msecs */
S16         *numFdS             /* number of ready descriptors */
)
{
   S32 ret;                     /* temporary return value */
   struct timeval  timeout;     /* timeout structure */
   struct timeval *timeoutPtr;
   S32 errCode;

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if (numFdS == NULLP)
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   *numFdS = 0;

   if (mSecTimeout != NULLP)
   {
      timeout.tv_sec  = *mSecTimeout / 1000;
      timeout.tv_usec = (*mSecTimeout % 1000) * 1000;
      timeoutPtr      = &timeout;
   }
   else
   {
      /* infinite timeout */ 
      timeoutPtr = NULLP;
   }

#ifdef TUCL_TTI_RCV
   timeout.tv_sec  = 0;
   timeout.tv_usec = 1;
#endif

   /* cm_inet_c_001.main_53 - Removed do-while loop */
   ret = select(FD_SETSIZE, readFdS, writeFdS, (fd_set*)0, timeoutPtr);

   /* cm_inet_c_001.main_53 -  Return ROKDNA in case select was interrupted */
   if ((ret == INET_ERR) && (INET_ERR_CODE == ERR_EINTR))
   {
      return (ROKDNA);
   }

   /* timeout occured */
   if (ret == 0)
   { 
      return (RTIMEOUT);
   }

   if (ret == INET_ERR)
   {
      /* asa: Added a check for ERR_INVAL to return ROK
       * readFdS and writeFdS may be passed as NULL to
       * cmInetSelect() call
       */
      switch(errCode = INET_ERR_CODE)
      {
         case ERR_INVAL:
            return ROK;

         default:
#ifdef CMINETDBG
            /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSelect() Failed : error(%d)\n",
                  INET_ERR_CODE);
            CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET039, 0, prntBuf);
#endif /* CMINETDBG */
            return RFAILED;

      } /* end of switch */
   }

   /* return number of ready file descriptors */
   /* cm_inet_c_001.main_54: Fix for Klockworks issue */
   *numFdS = (S16)ret;   

   return ROK; 
} /* end of  cmInetSelect */


/*
*
*      Fun:   cmInetSetOpt
*
*      Desc:  Sets a socket option.
*             The function supports following options:
*
*             CM_INET_OPT_BLOCK:
*                value: CM_INET_OPT_DISABLE  non-blocking
*                value: CM_INET_OPT_ENABLE   blocking
*
*             CM_INET_OPT_REUSEADDR:   
*                value: CM_INET_OPT_ENABLE   reuse address 
*
*             CM_INET_OPT_BROADCAST:
*                value: CM_INET_OPT_DISABLE
*                value: CM_INET_OPT_ENABLE
*
*             CM_INET_OPT_KEEPALIVE:
*                value: CM_INET_OPT_DISABLE
*                value: CM_INET_OPT_ENABLE
*
*             CM_INET_OPT_RX_BUF_SIZE:
*                value: receive buffer size in bytes
*
*             CM_INET_OPT_TX_BUF_SIZE:
*                value: transmitter buffer size in bytes
*
*             CM_INET_OPT_ADD_MCAST_MBR:
*                value: address of CmInetMCastInf structure
*
*             CM_INET_OPT_DRP_MCAST_MBR:
*                value: address of CmInetMCastInf structure
*
*             CM_INET_OPT_TCP_NODELAY:  
*                value: CM_INET_OPT_DISABLE
*                value: CM_INET_OPT_ENABLE
*
*             CM_INET_OPT_BSD_COMPAT: For Linux only
*                value: CM_INET_OPT_ENABLE   
*                value: CM_INET_OPT_DISABLE 
*
*             CM_INET_OPT_HDR_INCLD: 
*                value: CM_INET_ENABLE
*                value: CM_INET_DISABLE
*
*             CM_INET_OPT_DONT_FRAGMENT:
*                value: CM_INET_OPT_ENABLE
*                value: CM_INET_DISABLE
*
*             CM_INET_OPT_TOS:
*                value: Type of Service.
* 
*             CM_INET_OPT_TTL:
*                value: Time To Live.
*
*             CM_INET_OPT_IP_OPTIONS:
*                value: IPv4 header option value 
*                ENABLE/DISABLE.
*
*             CM_INET_OPT_IP_ROUTER_ALERT:
*                value: CM_INET_OPT_DISABLE
*                value: CM_INET_OPT_ENABLE
*
*             CM_INET_OPT_IPV4_PKTINFO
*                value: CM_INET_OPT_ENABLE
*                value: CM_INET_OPT_DISABLE
*
*             CM_INET_OPT_MCAST_LOOP:  
*                value: CM_INET_OPT_DISABLE
*                value: CM_INET_OPT_ENABLE
*
*             CM_INET_OPT_MCAST_IF:
*                value: Address of interface.
*
*             CM_INET_OPT_MCAST_TTL:
*                value: TTL of the outgoing multicast packet.
*
*             The next  options are defined only if IPV6 is 
*             supported.
*
*             CM_INET_OPT_ADD_MCAST6_MBR:
*                value: address of CmInetMCastInf6 structure
*
*             CM_INET_OPT_DRP_MCAST6_MBR:
*                value: address of CmInetMCastInf6 structure
*
*             CM_INET_OPT_MCAST6_LOOP:  
*                value: CM_INET_OPT_DISABLE
*                value: CM_INET_OPT_ENABLE
*
*             CM_INET_OPT_MCAST6_IF:
*                value: Interface index
*
*             CM_INET_OPT_MCAST6_HOPS:  
*                value: multicast hop limit 
*
*             CM_INET_OPT_RECVIPV6_HOPLIM:
*                value: CM_INET_OPT_ENABLE   hop limit will be returned
*                                            on the socket.
*                value: CM_INET_OPT_DISABLE  hop limit wont be returned 
*                                            on the socket.
*
*             CM_INET_OPT_RECVIPV6_HBHOPTS:
*                value: CM_INET_OPT_ENABLE   HBH Options will be returned
*                                            on the socket.
*                value: CM_INET_OPT_DISABLE  HBH Options wont be returned 
*                                            on the socket. 
*                                            
*             CM_INET_OPT_RECVIPV6_DSTOPTS:
*                value: CM_INET_OPT_ENABLE   Dest Options will be returned
*                                            on the socket.
*                value: CM_INET_OPT_DISABLE  Dest Options wont be returned 
*                                            on the socket.                     
*                                            
*             CM_INET_OPT_RECVIPV6_RTHDR:
*                value: CM_INET_OPT_ENABLE   Route Hdr Opt will be turned
*                                            ON on the socket.
*                value: CM_INET_OPT_DISABLE  Route Hdr Opt will be turned 
*                                            OFF on the socket.
*
*             CM_INET_OPT_IP_ROUTER_ALERT6  
*                value: CM_INET_OPT_ENABLE
*                value: CM_INET_OPT_DISABLE
* 
*             CM_INET_OPT_IPV6_PKTINFO
*                value: CM_INET_OPT_ENABLE   Enable sending and receiving
*                                            of packet info
*                value: CM_INET_OPT_DISABLE  Disable sending and receiving
*                                            of packet info
* 
*             CM_INET_OPT_LINGER
*                value: address of CmInetSockLinger structure
*
*             CM_INET_OPT_SCTP_EVENTS
*                value: address of CmInetSctpSockEvent structure
*
*             CM_INET_OPT_SCTP_PRIM_ADDR
*                value: address of CmInetSctpPrimAddr structure
*
*             CM_INET_OPT_SCTP_PEERADDR_PARAMS
*                value: address of CmInetSctpPeerAddrParams structure
*
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*             RNA     - failed, option not available
*             (Only when CM_INET2 is defined)
*
*      Notes: The send and receive buffer size may be system
*             specific. The cmInetSetOpt() call may return
*             successfuly although not the entire buffer size 
*             could be set!
*
*      File:  cm_inet.c
*
*/
S16 cmInetSetOpt
(
CmInetFd *sockFd,               /* socket file descriptor */ 
uint32_t level,                /* option level */
uint32_t type,                 /* option type */
Ptr      value                 /* option value */ 
) 
{
   S32  ret = ROK;              /* temporary return value */
   uint32_t  disable = 0;            /* disable option */
   uint32_t  enable = 1;             /* enable option */

   /* added for IPv4 options */
#ifdef IPV4_OPTS_SUPPORTED
#if((!defined (SS_VW)) && (!defined(SS_LINUX)))   
   TknStr64 *tempTknStr64;      /* points TknStr64 structure */
   /* which has value for IPv4 hdr options.*/
#endif /* SS_VW && SS_LINUX */   
#ifdef WIN32   
   int disableOpt = 0;
#endif /* WIN32 */   
#endif /* IPV4_OPTS_SUPPORTED */ 

#if (defined(SUNOS)|| defined(WIN32) || defined(SS_PS) || defined(SS_VW_MCAST)\
      || defined(HPOS))
   uint8_t   lpEnable = 1;           /* multicast loop enable */
   uint8_t   lpDisable = 0;          /* multicast loop disable */
#endif /* SUNOS || WIN32 || SS_PS || SS_VW_MCAST || HPOS */

#ifdef WIN32
   BOOL boolEnable = TRUE;      /* enable option */
   BOOL boolDisable = FALSE;    /* disable option */
#endif /* WIN32 */

#if (defined(SUNOS) || defined(WIN32) || defined(SS_PS) || \
      defined(SS_VW_MCAST) || defined(HPOS))
   struct ip_mreq stMreq;
   CmInetMCastInf *mCast;
#endif /* SUNOS || WIN32  || SS_PS || SS_VW_MCAST || HPOS */

#ifdef IPV6_SUPPORTED
   uint32_t    loopEna = 1;     /* IPv6 multicast loop enable */
   uint32_t    loopDis = 0;     /* IPv6 multicast loop disable */
   struct ipv6_mreq     *stMreq6Ptr;
   /* cm_inet_c_001.main_44 : some operating system doesnt have icmp6_filter. so 
      this flag is gaurded under ICMPV6_FILTER_SUPPORTED. so if user want this 
      support he has to enable the above flag.*/
   /* cm_inet_c_001.main_58 : Protaected under flag CM_ICMP_FILTER_SUPPORT
    * to support filteration  of ICMP messages */
#if (defined(ICMPV6_FILTER_SUPPORTED) || defined(CM_ICMP_FILTER_SUPPORT))
   struct icmp6_filter  *icmp6Filter; 
#endif /* ICMPV6_FILTER_SUPPORTED */
#endif /* IPV6_SUPPORTED */

   /* cm_inet_c_001.main_58 : Added new local variables to support filteration 
    * of ICMP messages */
#ifdef SS_LINUX
#ifdef CM_ICMP_FILTER_SUPPORT
  struct icmp_filter icmpFilter; 
#endif  
#endif  

   /*cm_inet_c_001.main_38 Updated for TUCL 2.1 Release (Kernel SCTP Support) */
#ifdef CM_LKSCTP
   struct linger               lngr;
   struct sctp_event_subscribe event; 
   struct sctp_paddrparams     addrParams;
   struct sctp_setprim         setPrim;
   struct sockaddr_in         *pAddr;
   struct sctp_assocparams     assocParams;
   struct sctp_initmsg         initmsg;
   struct sctp_rtoinfo         rtoinfo;
#ifdef IPV6_SUPPORTED
   struct sockaddr_in6        *pAddr6;
#endif /* IPV6_SUPPORTED */

   CmInetSockLinger           *pSockLinger;
   CmInetSctpSockEvent        *pSctpEvent;
   CmInetSctpPrimAddr         *pSctpPrimAddr;
   CmInetSctpPeerAddrParams   *pSctpPAddrParams;
   CmInetSctpRtoInfo          *pSctpRtoInfo;
   CmInetSctpInitMsg          *pSctpInitMsg;
   CmInetSctpAssocParams      *pSctpAssocParams;
#endif

   uint32_t    *optVal;


   /* cm_inet_c_001.main_58 : Added NULL check for value field */ 
   if(value == NULLP)
   {
      return RFAILED;
   }

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd))
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   switch (type)
   {
      case CM_INET_OPT_BLOCK:
         optVal = (uint32_t*)value;
         switch(*optVal)
         {
            case CM_INET_OPT_ENABLE:

#ifdef WIN32
               /* cm_inet_c_001.main_59: Fix for compilation warning */
               ret = ioctlsocket(sockFd->fd, FIONBIO, (uint32_t *)&disable);
#else
#ifdef SS_PS
               ret = ioctl(sockFd->fd, FIONBIO, (char*)&disable);
#else
#ifdef SS_VW
               ret = ioctl(sockFd->fd, (S32)FIONBIO, (S32)&disable);
#else
               ret = ioctl(sockFd->fd, (S32)FIONBIO, &disable);

#endif /* SS_VW */
#endif /* SS_PS */
#endif /* WIN32 */
               sockFd->blocking = 1;
               break;

            case CM_INET_OPT_DISABLE:
#ifdef WIN32
              /* cm_inet_c_001.main_59: Fix for compilation warning */
              ret = ioctlsocket(sockFd->fd, FIONBIO, (uint32_t *)&enable); 
#else
#ifdef SS_PS
               ret = ioctl(sockFd->fd, FIONBIO, (char*)&enable);
#else
#ifdef SS_VW
               ret = ioctl(sockFd->fd, (S32)FIONBIO, (S32)&enable);
#else
               ret = ioctl(sockFd->fd, (S32)FIONBIO, &enable);
#endif /* SS_VW */
#endif /* SS_PS */
#endif /* WIN32 */
               sockFd->blocking = 0;
               break;

            default:
               /* wrong value */
               return RFAILED;
               break;
         }
         break;

      case CM_INET_OPT_REUSEADDR:
         optVal = (uint32_t*)value;
         if (*optVal == CM_INET_OPT_ENABLE)
         {
#ifdef WIN32
            ret = setsockopt(sockFd->fd, level, SO_REUSEADDR,
                  (char*)&boolEnable, sizeof(boolEnable));
#else
            ret = setsockopt(sockFd->fd, level, SO_REUSEADDR,
                  (char*)&enable, sizeof(enable));
#ifdef SS_VW
            setsockopt(sockFd->fd, level, SO_REUSEPORT,
                  (char*)&enable, sizeof(enable));
#endif /* SS_VW */
#endif /* WIN32 */
         }
         else if (*optVal == CM_INET_OPT_DISABLE)
         {
#ifdef WIN32
            ret = setsockopt(sockFd->fd, level, SO_REUSEADDR,
                  (char*)&boolDisable, sizeof(boolDisable));
#else
            ret = setsockopt(sockFd->fd, level, SO_REUSEADDR,
                  (char*)&disable, sizeof(disable));
#ifdef SS_VW
            ret = setsockopt(sockFd->fd, level, SO_REUSEPORT,
                  (char*)&disable, sizeof(disable));
#endif /* SS_VW */
#endif /* WIN32 */
         }
         break;

      case CM_INET_OPT_BROADCAST:
         optVal = (uint32_t*)value;
         if (*optVal == CM_INET_OPT_ENABLE)
         {
#ifdef WIN32
            ret = setsockopt(sockFd->fd, level, SO_BROADCAST,
                  (char*)&boolEnable, sizeof(boolEnable));
#else
            ret = setsockopt(sockFd->fd, level, SO_BROADCAST,
                  (char*)&enable, sizeof(enable));
#endif /* WIN32 */
         }
         else if (*optVal == CM_INET_OPT_DISABLE)
         {
#ifdef WIN32
            ret = setsockopt(sockFd->fd, level, SO_BROADCAST,
                  (char*)&boolDisable, sizeof(boolDisable));
#else
            ret = setsockopt(sockFd->fd, level, SO_BROADCAST,
                  (char*)&disable, sizeof(disable));
#endif /* WIN32 */
         }
         break; 

      case CM_INET_OPT_KEEPALIVE:
         optVal = (uint32_t*)value;
         if (*optVal == CM_INET_OPT_ENABLE)
         {
#ifdef WIN32
            ret = setsockopt(sockFd->fd, level, SO_KEEPALIVE,
                  (char*)&boolEnable, sizeof(boolEnable));
#else
            ret = setsockopt(sockFd->fd, level, SO_KEEPALIVE,
                  (char*)&enable, sizeof(enable));
#endif /* WIN32 */
         }
         else if (*optVal == CM_INET_OPT_DISABLE)
         {
#ifdef WIN32
            ret = setsockopt(sockFd->fd, level, SO_KEEPALIVE,
                  (char*)&boolDisable, sizeof(boolDisable));
#else
            ret = setsockopt(sockFd->fd, level, SO_KEEPALIVE,
                  (char*)&disable, sizeof(disable));
#endif /* WIN32 */
         }
         break;

      case CM_INET_OPT_RX_BUF_SIZE:
         optVal = (uint32_t*)value;
         ret = setsockopt(sockFd->fd, level, SO_RCVBUF, 
               (char*)optVal, sizeof(*optVal));
         break;

      case CM_INET_OPT_TX_BUF_SIZE:
         optVal = (uint32_t*)value;
         ret = setsockopt(sockFd->fd, level, SO_SNDBUF, 
               (char*)optVal, sizeof(*optVal));
         break;

      case CM_INET_OPT_TCP_NODELAY:
         optVal = (uint32_t*)value;
         if (*optVal == CM_INET_OPT_ENABLE)
         {
#ifdef WIN32
#ifndef SS_WINCE
            ret = setsockopt(sockFd->fd, level, TCP_NODELAY,
                  (char*)&boolEnable, sizeof(boolEnable));
#endif /* SS_WINCE */
#else
            ret = setsockopt(sockFd->fd, level, TCP_NODELAY,
                  (char*)&enable, sizeof(enable)); 
#endif /* WIN32 */
         }
         else if (*optVal == CM_INET_OPT_DISABLE)
         {
#ifdef WIN32
#ifndef SS_WINCE
            ret = setsockopt(sockFd->fd, level, TCP_NODELAY,
                  (char*)&boolDisable, sizeof(boolDisable));
#endif /* SS_WINCE */
#else
            ret = setsockopt(sockFd->fd, level, TCP_NODELAY,
                  (char*)&disable, sizeof(disable));
#endif /* WIN32 */
         }
         break;

#if (defined(SUNOS)|| defined(WIN32) || defined(SS_PS) || \
      defined(SS_VW_MCAST) || defined(HPOS))

      case CM_INET_OPT_ADD_MCAST_MBR:
         mCast = (CmInetMCastInf*)value;

         /* Copy the addresses to stMreq structure */
#ifdef SS_PS
         stMreq.imr_mcastaddr.s_addr = CM_INET_HTON_UINT32(mCast->mCastAddr);
#else
         stMreq.imr_multiaddr.s_addr = CM_INET_HTON_UINT32(mCast->mCastAddr);
#endif
         stMreq.imr_interface.s_addr = CM_INET_HTON_UINT32(mCast->localAddr);

         ret = setsockopt(sockFd->fd, level, IP_ADD_MEMBERSHIP,
               (char*)&stMreq, sizeof(stMreq));
         break;

      case CM_INET_OPT_DRP_MCAST_MBR:
         mCast = (CmInetMCastInf*)value;

         /* Copy the addresses to stMreq structure */
#ifdef SS_PS
         stMreq.imr_mcastaddr.s_addr = CM_INET_HTON_UINT32(mCast->mCastAddr);
#else
         stMreq.imr_multiaddr.s_addr = CM_INET_HTON_UINT32(mCast->mCastAddr);
#endif
         stMreq.imr_interface.s_addr = CM_INET_HTON_UINT32(mCast->localAddr);

         ret = setsockopt(sockFd->fd, level, IP_DROP_MEMBERSHIP,
               (char*)&stMreq, sizeof(stMreq));
         break;

#endif /* SUNOS || WIN32 || SS_PS || SS_VW_MCAST || HPOS */

#ifdef SS_LINUX
         /* cm_inet_c_001.main_37 - Enable CMINET_BSDCOMPAT flag if system doesnt
            support CM_INET_OPT_BSD_COMPAT */
#ifndef CMINET_BSDCOMPAT
      case CM_INET_OPT_BSD_COMPAT:
         optVal = (uint32_t*)value;
         if (*optVal == CM_INET_OPT_ENABLE)
         {
            ret = setsockopt(sockFd->fd, level, SO_BSDCOMPAT,
                  &enable, sizeof(enable));
         }
         else if (*optVal == CM_INET_OPT_DISABLE)
         {
            ret = setsockopt(sockFd->fd, level, SO_BSDCOMPAT,
                  &disable, sizeof(disable));
         }
         break;
#endif /* CMINET_BSDCOMPAT */
#endif /* SS_LINUX */

#ifdef CM_INET2  
         /* Added for support of Raw socket  modify according to the 
          * option available on different platform  */
#if (defined(SUNOS)|| defined(WIN32) || defined(SS_PS) || defined(SS_VW) \
      || defined(HPOS))
      case CM_INET_OPT_HDR_INCLD:
         optVal = (uint32_t*)value;
         if (*optVal == CM_INET_OPT_ENABLE)
         {
#ifdef WIN32 
            return (RNA);    
#else
            ret = setsockopt(sockFd->fd, level, IP_HDRINCL,
                  (char*)&enable, sizeof(enable));
#endif /* WIN32 */
         }
         else if (*optVal == CM_INET_OPT_DISABLE)
         {
#ifdef WIN32
            return (RNA);    
#else
            ret = setsockopt(sockFd->fd, level, IP_HDRINCL,
                  (char*)&disable, sizeof(disable));
#endif /* WIN32 */
         }
         break;

         /* added new options */
#ifdef IPV4_OPTS_SUPPORTED
#ifdef SS_LINUX
         /* Linux: set Router Alert socket option to Intercept RAW RSVP 
            packets at the Intermediate node(Router) with Router Alert SET.
            This socket option is MUST be set (when this server is opened)
            if the RSVP server wants to intercept raw RSVP packets. */
      case CM_INET_OPT_IP_ROUTER_ALERT:
         optVal = (uint32_t*)value;
         if (*optVal == CM_INET_OPT_ENABLE)
         {   
            ret = setsockopt(sockFd->fd, level, IP_ROUTER_ALERT,
                  (char*)&enable, sizeof(enable));
            if (ret != ROK)
               return RFAILED;
         }
         else if (*optVal == CM_INET_OPT_DISABLE)
         {   
            ret = setsockopt(sockFd->fd, level, IP_ROUTER_ALERT,
                  (char*)&disable, sizeof(disable));
            if (ret != ROK)
               return RFAILED;
         }   
         break;         
#endif /* SS_LINUX */

         /* set Router Alert socket option */
      case CM_INET_OPT_IP_OPTIONS:
#if (defined (SS_VW) || defined(SS_LINUX))
         return (RNA);
#else  
         tempTknStr64=(TknStr64 *)value;
         if (tempTknStr64->pres == TRUE)
         {
            if (tempTknStr64->len == 0)
            {
               /* disable the IP_OPTIONS for Router Alert.  */
#ifdef WIN32                          
               ret = setsockopt(sockFd->fd, level, IP_OPTIONS, 
                     (const char *)&disableOpt, sizeof(int));
#else
               ret = setsockopt(sockFd->fd, level, IP_OPTIONS, NULL, 0);
#endif /* WIN32 */                  
            }  
            else
               /* enable the IP_OPTIONS for Router Alert */
               ret = setsockopt(sockFd->fd, level, IP_OPTIONS,
                     (char *)tempTknStr64->val, tempTknStr64->len);
         }
         else
            return RFAILED; /* Trying to set IPv4 Hdr option
                                * without giving option values*/
#endif /* SS_VW || SS_LINUX */
         break;
#endif /* IPV4_OPTS_SUPPORTED */

         /* added new options */
#if (defined(SS_LINUX) && (!defined(SS_VW) && !defined(WIN32)))
#ifdef LOCAL_INTF
      case CM_INET_OPT_IPV4_PKTINFO:
         optVal = (uint32_t*)value;
         if (*optVal == CM_INET_OPT_ENABLE)
         {   
            /* set IP_PKTINFO option when IP_ROUTER_ALERT is set in linux */
            ret = setsockopt(sockFd->fd, level, IP_PKTINFO,
                  (char*)&enable, sizeof(enable));

            if (ret != ROK)
               return RFAILED;
         }
         else if (*optVal == CM_INET_OPT_DISABLE)
         {   
            /* disable IP_PKTINFO when IP_ROUTER_ALERT is set in linux */ 
            ret = setsockopt(sockFd->fd, level, IP_PKTINFO,
                  (char*)&disable, sizeof(disable));

            if (ret != ROK)
               return RFAILED;
         }   
         break;   
#endif /* LOCAL_INTF */            
#endif /* SS_LINUX */

#endif /* SUNOS || WIN32 || SS_PS || SS_VW || HPOS */

      case CM_INET_OPT_DONTFRAGMENT:
         optVal = (uint32_t*)value;
         if (*optVal == CM_INET_OPT_ENABLE)
         {
#ifdef WIN32
            ret = setsockopt(sockFd->fd, level, IP_DONTFRAGMENT,
                  (char*)&boolEnable, sizeof(boolEnable));
#endif /* WIN32 */
         }
         else if (*optVal == CM_INET_OPT_DISABLE)
         {
#ifdef WIN32
            ret = setsockopt(sockFd->fd, level, IP_DONTFRAGMENT,
                  (char*)&boolDisable, sizeof(boolDisable));
#endif /* WIN32 */
         }
         break;

         /* also add these 2 options for VxWorks */         
#if (defined(SUNOS)|| defined(WIN32) || defined(HPOS) || defined(SS_VW))
      case CM_INET_OPT_TOS:
         optVal = (uint32_t*)value;
         ret = setsockopt(sockFd->fd, level, IP_TOS,
               (char*)optVal, sizeof(*optVal));
         break;

      case CM_INET_OPT_TTL:
         optVal = (uint32_t*)value;
         ret = setsockopt(sockFd->fd, level, IP_TTL,
               (char*)optVal, sizeof(*optVal));
         break;
#endif /* SUNOS || WIN32 || HPOS || SS_VW */
#endif  /* CM_INET2 */ 

#if (defined(SUNOS)|| defined(WIN32) || defined(SS_PS) || defined(SS_VW_MCAST) \
      || defined(HPOS))
      case CM_INET_OPT_MCAST_LOOP:
         optVal = (uint32_t*)value;
         if (*optVal == CM_INET_OPT_ENABLE)
         {
#ifdef SS_VW            
            ret = setsockopt(sockFd->fd, level, IP_MULTICAST_LOOP,
                  (char *)&lpEnable, sizeof(lpEnable));
#else
            ret = setsockopt(sockFd->fd, level, IP_MULTICAST_LOOP,
                  (const char *)&lpEnable, sizeof(lpEnable));
#endif /* SS_VW */           
         }
         else
         {
#ifdef SS_VW            
            ret = setsockopt(sockFd->fd, level, IP_MULTICAST_LOOP,
                  (char *)&lpDisable, sizeof(lpDisable));
#else
            ret = setsockopt(sockFd->fd, level, IP_MULTICAST_LOOP,
                  (const char *)&lpDisable, sizeof(lpDisable));
#endif /* SS_VW */            
         }
         break;

      case CM_INET_OPT_MCAST_IF:
         optVal = (uint32_t*)value;
         *optVal = CM_INET_HTON_UINT32((uint32_t)*optVal); 
         ret = setsockopt(sockFd->fd, level, IP_MULTICAST_IF,
               (char *)optVal, sizeof(struct in_addr));
         break;

      case CM_INET_OPT_MCAST_TTL:
         optVal = (uint32_t*)value;
         /* remove const in setsockopt for VW */
#ifdef SS_VW      
         ret = setsockopt(sockFd->fd, level, IP_MULTICAST_TTL,
               (char *)optVal, sizeof(uint8_t));
#else
         ret = setsockopt(sockFd->fd, level, IP_MULTICAST_TTL,
               (const char *)optVal, sizeof(uint8_t));
#endif /* SS_VW */         
         break;
#endif /* SUNOS || WIN32 || SS_PS || SS_VW_MCAST || HPOS */

#ifdef IPV6_SUPPORTED
      case CM_INET_OPT_IPV6_TTL:
         optVal = (uint32_t*)value;
         ret = setsockopt(sockFd->fd, level, IPV6_UNICAST_HOPS,
               (char*)optVal, sizeof(*optVal));
         break;

      case CM_INET_OPT_ADD_MCAST6_MBR:
         stMreq6Ptr = (struct ipv6_mreq *)value;
         ret = setsockopt(sockFd->fd, level, IPV6_JOIN_GROUP,
               (char*)stMreq6Ptr, sizeof(struct ipv6_mreq));
         break;

      case CM_INET_OPT_DRP_MCAST6_MBR:
         stMreq6Ptr = (struct ipv6_mreq *)value;
         ret = setsockopt(sockFd->fd, level, IPV6_LEAVE_GROUP,
               (char*)stMreq6Ptr, sizeof(struct ipv6_mreq));
         break;

      case CM_INET_OPT_MCAST6_LOOP:  
         optVal = (uint32_t*)value;
         if (*optVal == CM_INET_OPT_ENABLE)
         {
            ret = setsockopt(sockFd->fd, level, IPV6_MULTICAST_LOOP,
                  &loopEna, sizeof(loopEna));
         }
         else
         {
            ret = setsockopt(sockFd->fd, level, IPV6_MULTICAST_LOOP,
                  &loopDis, sizeof(loopDis));
         }
         break;

      case CM_INET_OPT_MCAST6_IF:
         ret = setsockopt(sockFd->fd, level, IPV6_MULTICAST_IF,
               (uint32_t *)value, sizeof(uint32_t));
         break;

      case CM_INET_OPT_MCAST6_HOPS:
         optVal = (uint32_t*)value;
         ret = setsockopt(sockFd->fd, level, IPV6_MULTICAST_HOPS,
               (char *)optVal, sizeof(uint32_t));
         break;

         /* cm_inet_c_001.main_44 : some operating system doesnt have icmp6_filter. so 
            this flag is gaurded under ICMPV6_SUPPORTED. so if user want this 
            support he has to enable the above flag.*/
         /* cm_inet_c_001.main_58 : Protaected under flag CM_ICMP_FILTER_SUPPORT
          * to support filteration  of ICMP messages */
#if (defined(ICMPV6_FILTER_SUPPORTED) || defined(CM_ICMP_FILTER_SUPPORT))
      case CM_INET_OPT_ICMP6_FILTER:  
         icmp6Filter = (struct icmp6_filter *)value;
         ret = setsockopt(sockFd->fd, level, ICMP6_FILTER,
               (char *)icmp6Filter, sizeof(struct icmp6_filter));
         break;
#endif /* ICMPV6_FILTER_SUPPORTED */

         /* added new options */
#ifdef IPV6_OPTS_SUPPORTED
      case CM_INET_OPT_RECVIPV6_HOPLIM:
         optVal = (uint32_t*)value;
#ifdef SS_LINUX         
         ret = setsockopt(sockFd->fd, level, IPV6_HOPLIMIT,
               (char *)optVal, sizeof(uint32_t)); 
#else
         ret = setsockopt(sockFd->fd, level, IPV6_HOPLIMIT,
               (char *)optVal, sizeof(uint32_t)); 
         /* enable the reception of IPv6 HopLimit value as ancillary data */
         ret = setsockopt(sockFd->fd, level, IPV6_RECVHOPLIMIT,
               (char*)&enable, sizeof(enable)); 
#endif /* SS_LINUX */

         break;

      case CM_INET_OPT_RECVIPV6_HBHOPTS:
         optVal = (uint32_t*)value;
#ifdef SS_LINUX
         ret = setsockopt(sockFd->fd, level, IPV6_HOPOPTS,
               (char *)optVal, sizeof(uint32_t));
#else
         ret = setsockopt(sockFd->fd, level, IPV6_RECVHOPOPTS,
               (char *)optVal, sizeof(uint32_t)); 
#endif /* SS_LINUX */          
         break;

      case CM_INET_OPT_RECVIPV6_DSTOPTS:
         optVal = (uint32_t*)value;
#ifdef SS_LINUX         
         ret = setsockopt(sockFd->fd, level, IPV6_DSTOPTS,
               (char *)optVal, sizeof(uint32_t));
#else         
         ret = setsockopt(sockFd->fd, level, IPV6_RECVDSTOPTS,
               (char *)optVal, sizeof(uint32_t));
#endif /* SS_LINUX */
         break;

      case CM_INET_OPT_RECVIPV6_RTHDR:
         optVal = (uint32_t*)value;
#ifdef SS_LINUX         
         ret = setsockopt(sockFd->fd, level, IPV6_RTHDR,
               (char *)optVal, sizeof(uint32_t));
#else
         ret = setsockopt(sockFd->fd, level, IPV6_RECVRTHDR,
               (char *)optVal, sizeof(uint32_t));
#endif /* SS_LINUX */         
         break;      

         /* works ONLY for IPPROTO_RAW type socket. so if it this socket
          * option is tried to set for IPPROTO_RSVP, then it is supposed
          * to fail with EINVAL according to net/ipv6/ipv6_sockglue.c 
          *
          * if HI_SRVC_RAW_RAW is not used during ServOpenReq as the server 
          * type, then it will fail here due to above reason */
#ifdef SS_LINUX
      case CM_INET_OPT_IP_ROUTER_ALERT6:
         optVal = (uint32_t*)value;
         if(*optVal == CM_INET_OPT_ENABLE)
            ret = setsockopt(sockFd->fd, IPPROTO_IPV6, IPV6_ROUTER_ALERT,
                  (char *)&enable, sizeof(enable));          
         else
            ret = setsockopt(sockFd->fd, level, IPV6_ROUTER_ALERT,
                  (char *)&disable, sizeof(disable));

         break;
#endif /* SS_LINUX */
#endif /* IPV6_OPTS_SUPPORTED */

#ifdef LOCAL_INTF         
      case CM_INET_OPT_IPV6_PKTINFO:
         optVal = (uint32_t*)value;
#ifdef SS_LINUX         
         ret = setsockopt(sockFd->fd, level, IPV6_PKTINFO,
               (char *)optVal, sizeof(uint32_t));
#else         
         ret = setsockopt(sockFd->fd, level, IPV6_RECVPKTINFO,
               (char *)&enable, sizeof(enable));
#endif /* SS_LINUX */
         break;
#endif /* LOCAL_INTF */

#endif /* IPV6_SUPPORTED */

         /*cm_inet_c_001.main_38 Updated for TUCL 2.1 Release (Kernel SCTP Support) */
#ifdef CM_LKSCTP
      case CM_INET_OPT_LINGER:
         pSockLinger = (CmInetSockLinger *)value;

         memset(&lngr, 0, sizeof(struct linger));

         if (pSockLinger->enable == TRUE)
            lngr.l_onoff = 1;
         else 
            lngr.l_onoff = 0;

         lngr.l_linger = pSockLinger->lingerTime;
         ret = setsockopt(sockFd->fd, level, SO_LINGER, &lngr, sizeof(lngr));
         break;

      case CM_INET_OPT_SCTP_EVENTS:
         pSctpEvent = (CmInetSctpSockEvent *)value;

         memset(&event, 0, sizeof(struct sctp_event_subscribe));

         if (pSctpEvent->dataIoEvent == TRUE)
            event.sctp_data_io_event = 1;

         if (pSctpEvent->associationEvent == TRUE)
            event.sctp_association_event = 1;

         if (pSctpEvent->addressEvent == TRUE)
            event.sctp_address_event = 1;

         if (pSctpEvent->sendFailureEvent == TRUE)
            event.sctp_send_failure_event = 1;

         if (pSctpEvent->peerErrorEvent == TRUE)
            event.sctp_peer_error_event = 1;

         if (pSctpEvent->shutdownEvent == TRUE)
            event.sctp_shutdown_event = 1;

         if (pSctpEvent->partialDeliveryEvent == TRUE)
            event.sctp_partial_delivery_event = 1;

         if (pSctpEvent->adaptationLayerEvent == TRUE)
#ifdef SUN_KSCTP
            event.sctp_adaption_layer_event = 1;
#else
         event.sctp_adaptation_layer_event = 1;
#endif

         ret = setsockopt(sockFd->fd, level, SCTP_EVENTS, &event, sizeof(event));
         break;

      case CM_INET_OPT_SCTP_PRIM_ADDR:
         pSctpPrimAddr = (CmInetSctpPrimAddr *)value;

         memset(&setPrim, 0, sizeof(struct sctp_setprim));

#ifdef IPV6_SUPPORTED 
         if (pSctpPrimAddr->addr.type == CM_INET_IPV6ADDR_TYPE)
         {
            if (sockFd->protType == AF_INET)
            {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
               /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
               /* cm_inet_c_001.main_62:Warning fix */
               snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "Invalid address: sockFd->protType(%d),"
                     " sockFd->fd(%ld)\n", sockFd->protType, sockFd->fd);
               CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET040, 0, prntBuf);
#else
               /* cm_inet_c_001.main_62:Warning fix */
               snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "Invalid address: sockFd->protType(%d),"
                     " sockFd->fd(%d)\n", sockFd->protType, sockFd->fd);
               CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET040, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
               return RFAILED;
            }

            pAddr6 = (struct sockaddr_in6*)&(setPrim.ssp_addr);
            pAddr6->sin6_family      = AF_INET6;
            pAddr6->sin6_port        = CM_INET_HTON_UINT16(pSctpPrimAddr->port);
            CM_INET_COPY_IPV6ADDR(&pAddr6->sin6_addr.s6_addr, &pSctpPrimAddr->addr.u.ipv6NetAddr); 
         }
         else 
         {
            pAddr = (struct sockaddr_in*)&(setPrim.ssp_addr);
            pAddr->sin_family      = AF_INET;
            pAddr->sin_port        = CM_INET_HTON_UINT16(pSctpPrimAddr->port);
            pAddr->sin_addr.s_addr = CM_INET_HTON_UINT32(pSctpPrimAddr->addr.u.ipv4NetAddr);
         }
#else 
         pAddr = (struct sockaddr_in*)&(setPrim.ssp_addr);
         pAddr->sin_family      = AF_INET;
         pAddr->sin_port        = CM_INET_HTON_UINT16(pSctpPrimAddr->port);
         pAddr->sin_addr.s_addr = CM_INET_HTON_UINT32(pSctpPrimAddr->addr.u.ipv4NetAddr);
#endif /* IPV6_SUPPORTED */

         setPrim.ssp_assoc_id   = pSctpPrimAddr->assocId;

         ret = setsockopt(sockFd->fd, level, SCTP_PRIMARY_ADDR, &setPrim, sizeof(setPrim));
         break;

      case CM_INET_OPT_SCTP_PEERADDR_PARAMS:
         pSctpPAddrParams = (CmInetSctpPeerAddrParams *)value;

         memset(&addrParams, 0, sizeof(struct sctp_paddrparams));


         if (pSctpPAddrParams->s.addrPres == TRUE)
         {
#ifdef IPV6_SUPPORTED 
            if (pSctpPAddrParams->s.addr.type == CM_INET_IPV6ADDR_TYPE)
            {
               if (sockFd->protType == AF_INET)
               {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
                  /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
                  /* cm_inet_c_001.main_62:Warning fix */
                  snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "Invalid address: sockFd->protType(%d),"
                        " sockFd->fd(%ld)\n", sockFd->protType, sockFd->fd);
                  CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET041, 0, prntBuf);
#else
                  /* cm_inet_c_001.main_62:Warning fix */
                  snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "Invalid address: sockFd->protType(%d),"
                        " sockFd->fd(%d)\n", sockFd->protType, sockFd->fd);
                  CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET041, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */

                  return RFAILED;
               }

               pAddr6 = (struct sockaddr_in6*)&(addrParams.spp_address);
               pAddr6->sin6_family      = AF_INET6;
               pAddr6->sin6_port        = CM_INET_HTON_UINT16(pSctpPAddrParams->s.port);
               CM_INET_COPY_IPV6ADDR(&pAddr6->sin6_addr.s6_addr, &pSctpPAddrParams->s.addr.u.ipv6NetAddr); 
            }
            else 
            {
               pAddr = (struct sockaddr_in*)&(addrParams.spp_address);
               pAddr->sin_family      = AF_INET;
               pAddr->sin_port        = CM_INET_HTON_UINT16(pSctpPAddrParams->s.port);
               pAddr->sin_addr.s_addr = CM_INET_HTON_UINT32(pSctpPAddrParams->s.addr.u.ipv4NetAddr);
            }
#else 
            pAddr = (struct sockaddr_in*)&(addrParams.spp_address);
            pAddr->sin_family      = AF_INET;
            pAddr->sin_port        = CM_INET_HTON_UINT16(pSctpPAddrParams->s.port);
            pAddr->sin_addr.s_addr = CM_INET_HTON_UINT32(pSctpPAddrParams->s.addr.u.ipv4NetAddr);
#endif /* IPV6_SUPPORTED */
         }
         else
         {
#ifdef IPV6_SUPPORTED 
            if (sockFd->protType == AF_INET6)
               addrParams.spp_address.ss_family = AF_INET6;
            else
               addrParams.spp_address.ss_family = AF_INET;
#else
            addrParams.spp_address.ss_family = AF_INET;
#endif
         }

         /* Not validating the address, whether addr is a valid address or not */

         addrParams.spp_assoc_id   = pSctpPAddrParams->assocId;
         /*cm_inet_c_001.main_58  : fix for klockwork issue */
         addrParams.spp_pathmaxrxt = (uint16_t)pSctpPAddrParams->pathMaxRxt;
#ifdef SUN_KSCTP
         if (pSctpPAddrParams->hbEnblFlag == CM_INET_OPT_ENABLE)
            addrParams.spp_hbinterval = pSctpPAddrParams->hbInterval;
         else
            addrParams.spp_hbinterval = 0;
#else
         /* linux */
         addrParams.spp_flags = 0;

         if (pSctpPAddrParams->pmtudFlag == CM_INET_OPT_ENABLE)
         {
            addrParams.spp_flags     |= SPP_PMTUD_ENABLE;
            addrParams.spp_pathmtu    = pSctpPAddrParams->pathMtu;
         }
         else if(pSctpPAddrParams->pmtudFlag == CM_INET_OPT_DISABLE)
            addrParams.spp_flags     |= SPP_PMTUD_DISABLE;

         if (pSctpPAddrParams->sackDelayFlag == CM_INET_OPT_ENABLE)
         {
            addrParams.spp_flags     |= SPP_SACKDELAY_ENABLE;
            addrParams.spp_sackdelay  = pSctpPAddrParams->sackDelay;
         }
         else if(pSctpPAddrParams->sackDelayFlag == CM_INET_OPT_DISABLE)
            addrParams.spp_flags     |= SPP_SACKDELAY_DISABLE;

         if (pSctpPAddrParams->hbEnblFlag == CM_INET_OPT_ENABLE)
         {
            addrParams.spp_flags     |= SPP_HB_ENABLE;
            addrParams.spp_hbinterval = pSctpPAddrParams->hbInterval;
         }
         else if(pSctpPAddrParams->hbEnblFlag == CM_INET_OPT_DISABLE)
            addrParams.spp_flags |= SPP_HB_DISABLE;
#endif
         ret = setsockopt(sockFd->fd, level, SCTP_PEER_ADDR_PARAMS, &addrParams, sizeof(addrParams));
         break;

      case CM_INET_OPT_SCTP_ASSOC_PARAMS:
         pSctpAssocParams = (CmInetSctpAssocParams *)value;

         memset(&assocParams, 0, sizeof(struct sctp_assocparams));

         assocParams.sasoc_cookie_life              = pSctpAssocParams->cookieLife;
         assocParams.sasoc_asocmaxrxt               = pSctpAssocParams->assocMaxReTx;
         assocParams.sasoc_assoc_id                 = pSctpAssocParams->assocId;
         assocParams.sasoc_number_peer_destinations = pSctpAssocParams->numberOfPeerDest;
         assocParams.sasoc_peer_rwnd                = pSctpAssocParams->peerRwnd;
         assocParams.sasoc_local_rwnd               = pSctpAssocParams->localRwnd;

         ret = setsockopt(sockFd->fd, level, SCTP_ASSOCINFO, &assocParams, sizeof(assocParams));
         break;

      case CM_INET_OPT_SCTP_RTO_INFO:
         pSctpRtoInfo = (CmInetSctpRtoInfo *)value;

         memset(&rtoinfo, 0, sizeof(struct sctp_rtoinfo));

         rtoinfo.srto_assoc_id = pSctpRtoInfo->assocId;
         rtoinfo.srto_initial  = pSctpRtoInfo->rtoInitial;
         rtoinfo.srto_max      = pSctpRtoInfo->rtoMax;
         rtoinfo.srto_min      = pSctpRtoInfo->rtoMin;

         ret = setsockopt(sockFd->fd, level, SCTP_RTOINFO, &rtoinfo, sizeof(rtoinfo));
         break;

      case CM_INET_OPT_SCTP_INIT_MSG:
         pSctpInitMsg = (CmInetSctpInitMsg *)value;

         memset(&initmsg, 0, sizeof(struct sctp_initmsg));

         initmsg.sinit_max_attempts   = pSctpInitMsg->maxInitReTx;
         initmsg.sinit_max_init_timeo = pSctpInitMsg->maxInitTimeout;
         initmsg.sinit_num_ostreams   = pSctpInitMsg->numOstreams;
         initmsg.sinit_max_instreams  = pSctpInitMsg->maxInstreams;

         ret = setsockopt(sockFd->fd, level, SCTP_INITMSG, &initmsg, sizeof(initmsg));
         break;

#endif /*CM_LKSCTP*/

         /* cm_inet_c_001.main_58 : Added to support filteration  of ICMP 
          * messages and protected under CM_ICMP_FILTER_SUPPORT flag. Its a
          *  partial implementaion for icmp filter done for TUCL */
#ifdef SS_LINUX
#ifdef CM_ICMP_FILTER_SUPPORT
      case CM_INET_OPT_ICMP_FILTER:
         optVal = (uint32_t*)value;
         ret = setsockopt(sockFd->fd, level, ICMP_FILTER,
               optVal, sizeof(icmpFilter));
         break;
#endif
#endif

      default:  
         /* wrong socket option type */
         return RFAILED;
         break;
   }

   if (ret == INET_ERR)
   {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSetOpt() Failed : error(%d), sockFd->fd(%ld)\n",
            INET_ERR_CODE, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET042, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetSetOpt() Failed : error(%d), sockFd->fd(%d)\n",
            INET_ERR_CODE, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET042, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
      return RFAILED;
   }          
   return ROK;
} /* end of cmInetSetOpt */



/*
*
*      Fun:   cmInetGetNumRead
*
*      Desc:  Gives the number of pending octets in the socket receive buffer.
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*             
*      Notes: None.
*
*      File:  cm_inet.c
*
*/

S16 cmInetGetNumRead
(
CmInetFd *sockFd,               /* socket file descriptor */
uint32_t      *dataLen               /* number of pending octets */
/* removed 3rd argument memInfo */
)
{
   S32 ret;                     /* temporary return value */

   /* removed local variables added for recvfrom call */


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd) ||
         (dataLen == NULLP))
   {
      return RFAILED;
   }
#endif

   /* use ioctl call for all types of socket to get length of 
      pending data in the socket recv buffer */
#ifdef WIN32
   /* cm_inet_c_001.main_59: Fix for compilation warning */
   ret = ioctlsocket(sockFd->fd, FIONREAD, (uint32_t *)dataLen);  
#else 
#ifdef SS_PS
   ret = ioctl(sockFd->fd, FIOREAD, (char*)dataLen);
#else
#ifdef SS_VW
   ret = ioctl(sockFd->fd, FIONREAD, (S32)dataLen);
#else
   ret = ioctl(sockFd->fd, FIONREAD, dataLen);
#endif /* SS_VW */
#endif /* SS_PS */
#endif /* WIN32 */

   /* For UDP socket assign the length of pending data in the 
      socket recv buffer to largest datagram size. 
      Removed recvfrom call & necessary processing for it. */

   if (ret == INET_ERR)
   {
      /* removed error check CONABORTED added for recvfrom call. 
         Also return value changed from RCLOSED to ROK */
      /*  Check for reset connection */
      /* cm_inet_c_001.main_45: Close the TCP connection only when err is one of these*/
      if ((INET_ERR_CODE == ERR_CONNREFUSED) ||
            (INET_ERR_CODE == ERR_CONNABORTED) ||
            (INET_ERR_CODE == ERR_TIMEDOUT))
      {
         *dataLen = 0;

         /* cm_inet_c_001.main_50 
          * Return RCLOSED instead of ROK to initiate connection closure.
          * ROK will be returned only if the ioctl call above returns ROK.
          * The routines calling this function have been modified to not
          * return RCLOSED when this function returns ROK with pending data 
          * length value of 0. This modification is needed because:
          * Due to latency (mostly in solaris) sometimes ioctl(FIONREAD)
          * returns successfully with pend length as 0 on a TCP socket that 
          * select says is ready to read. This should not be considered as 
          * connection closed.
          */
         return (RCLOSED);
      }

      /* removed error check ERR_WOULDBLOCK */ 
      /* cm_inet_c_001.main_45: Dont close the connection in case of ERR_CONNRESET */
      if ((INET_ERR_CODE == ERR_AGAIN) ||
            (INET_ERR_CODE == ERR_CONNRESET))
      {
         *dataLen = 0;
         return (ROKDNA);
      }

#ifdef SS_LINUX
      /* cm_inet_c_001.main_45: Change 2048 to CM_INET_MAX_UDPRAW_MSGSIZE */
      *dataLen = CM_INET_MAX_UDPRAW_MSGSIZE;
      return ROK;
#endif /* SS_LINUX */

      /* removed error debug printing added for recvfrom call. */

#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetGetNumRead() Failed : error(%d),"
            " sockFd->fd(%ld)\n", INET_ERR_CODE, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET043, 0, prntBuf);
#else
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetGetNumRead() Failed : error(%d),"
            " sockFd->fd(%d)\n", INET_ERR_CODE, sockFd->fd);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET043, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
      return RFAILED;
   }

   return ROK;
} /* end of cmInetGetNumRead */


#ifndef SS_PS
/*
*
*      Fun:   cmInetGetHostByName
*
*      Desc:  Resolves a host name into the appropriate 4 byte Internet 
*             address.     
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: None.
*
*      File:  cm_inet.c
*
*/
 
S16 cmInetGetHostByName
(
S8              *hostName,         /* host name */  
CmInetIpAddrTbl *addrTbl           /* Address Table of IPV4 Addresses */
)
{
#ifndef SS_VW
   uint8_t            numAddrs;       /* Number of Addresses */
#endif /* SS_VW */

#if (defined(WIN32) || defined(SS_LINUX) || defined(HPOS))
   struct hostent *hostid;       /* pointer to host information */
#else
#ifndef SS_VW
   struct hostent hostid;        /* host information */
   S8 infoBuf[CM_INET_MAX_INFO]; /* info buffer */
   S32 err;                      /* error code */
#endif /* SS_VW */
#endif /* WIN32 || SS_LINUX || HPOS  */


#if (ERRCLASS & ERRCLS_INT_PAR)
      /* error check on parameters */
      if ((hostName == NULLP) || (addrTbl == NULLP))
      {
         return RFAILED;
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   /* Initialise */
#ifndef SS_VW
   numAddrs       = 0;
#endif /* SS_VW */

   addrTbl->count = 0;

#if (defined(WIN32) || defined(SS_LINUX) || defined(HPOS))
   hostid = gethostbyname(hostName);
   if (hostid == 0) 
   {
#ifdef CMINETDBG
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetGetHostByName() Failed : error(%d),"
            " hostName(%p)\n", INET_ERR_CODE, hostName);
      CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET044, 0, prntBuf);
#endif /* CMINETDBG */
      return RFAILED;
   }
   if (hostid->h_addrtype != AF_INET)
   {
#ifdef CMINETDBG
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetGetHostByName() Failed : error(%d),"
            " hostName(%p), hostid->h_addrtype(%d)\n",
            INET_ERR_CODE, hostName, hostid->h_addrtype);
      CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET045, 0, prntBuf);
#endif /* CMINETDBG */
      return RFAILED;
   }
   else
   {
      while ((numAddrs < CM_INET_IPV4_NUM_ADDR) &&
            (hostid->h_addr_list[numAddrs] != NULLP))
      {
         addrTbl->netAddr[addrTbl->count++] = 
            CM_INET_NTOH_UINT32 (*((uint32_t *) hostid->h_addr_list[numAddrs]));
         numAddrs += 1;
      }
   }
#else

#ifdef SS_VW
   {
      S32 vwIpAddr;

      vwIpAddr = hostGetByName(hostName);
      if (vwIpAddr == INET_ERR)
      {
#ifdef CMINETDBG
         /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetGetHostByName() Failed : error(%d),"
               " hostName(%p)\n", INET_ERR_CODE, hostName);
         CMINETLOGERROR(ERRCLS_DEBUG, ECMINET046, 0, prntBuf);
#endif /* CMINETDBG */
         return RFAILED;
      }
      CM_COPY_VWIPADDR(vwIpAddr, &(addrTbl->netAddr[addrTbl->count]));
      addrTbl->count++;
   }
#else

   err = 0;                     /* err is not reset by gethostnyname_r()! */

   gethostbyname_r(hostName, &hostid, infoBuf, CM_INET_MAX_INFO, (int*)&err);
   if ((hostid.h_addrtype != AF_INET) || (err < 0))
   {
#ifdef CMINETDBG
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetGetHostByName() Failed : error(%d), hostName(%p),"
            " hostid.h_addrtype(%d)\n",
            INET_ERR_CODE, hostName, hostid.h_addrtype);
      CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET047, 0, prntBuf);
#endif /* CMINETDBG */
      return RFAILED;
   }
   else
   {
      while ((numAddrs < CM_INET_IPV4_NUM_ADDR) &&
            (hostid.h_addr_list[numAddrs] != NULLP))
      {
         addrTbl->netAddr[addrTbl->count++] = 
            CM_INET_NTOH_UINT32 (*((uint32_t *) hostid.h_addr_list[numAddrs]));
         numAddrs += 1;
      }
   }
#endif /* SS_VW */

#endif /* WIN32  || SS_LINUX || HPOS */

   return ROK;

} /* end of cmInetGetHostByName */


/* The getipnodebyname is not supported on all the Solaris Operating system
 * versions. This has to be supported on operating systems that support IPV6
 * as per the RFC on the IPV6 socket interface. Hence this function is moved
 * under the IPV6_SUPPORTED flag */

/* This function now can be called for both IPv4 and IPv6. However, we will 
 * call cmInetGetHostByName inside for IPv4. Move all flag dependencies 
 * inside this function. */
/*
*
*      Fun:   cmInetGetIpNodeByName
*
*      Desc:  Resolves a host name into the appropriate 4 byte Internet 
*             address or into the appropriate 16 byte IPV6 address.
*             This function is expected to be thread safe and should be used
*             instead of the cmInetGetHostByName function.
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: None.
*
*      File:  cm_inet.c
*
*/
S16 cmInetGetIpNodeByName
(
S8              *hostName,         /* host name */  
CmInetIpAddrArr *addrArr           /* Array of addressed filled in */
)
{
   /* for return value from cmInetGetHostByName */
#ifndef IPV6_SUPPORTED   
   S16    ret; 
#else   
#ifdef SUNOS
#ifndef SS_LINUX
   uint8_t     numAddrs=0;              /* Number of addresses */
   int    err=0;                   /* error code */
   struct hostent *hostid;       /* host information */
#endif /* SS_LINUX */
#endif /* SUNOS */
#endif /* IPV6_SUPPORTED */



#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((hostName == NULLP) || (addrArr == NULLP))
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

#ifdef IPV6_SUPPORTED
#ifdef SUNOS
#ifndef SS_LINUX

#ifdef IPV6_SUPPORTED
   if (addrArr->type == CM_INET_IPV6ADDR_TYPE)
      hostid = getipnodebyname(hostName, AF_INET6, 0, &err);
   else
#endif /* IPV6_SUPPORTED */
      hostid = getipnodebyname(hostName, AF_INET, 0, &err);
   if (!hostid)
   {
#ifdef CMINETDBG
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetGetIpNodeByName() Failed : error(%d),"
            " hostName(%p), addrArr->type(%d)n", 
            err, hostName, addrArr->type);
      CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET048, 0, prntBuf);
#endif /* CMINETDBG */
      return RFAILED;
   }

#ifdef IPV6_SUPPORTED
   if (addrArr->type == CM_INET_IPV6ADDR_TYPE)
   {
      if (hostid->h_addrtype == AF_INET6)
      {
         while ((numAddrs < CM_INET_IPV6_NUM_ADDR) &&
               (hostid->h_addr_list[numAddrs] != NULLP))
         {
            /* Use the cminet fill macro here */
            CM_INET_COPY_IPV6ADDR(&addrArr->u.ipv6AddrArr.netAddr[numAddrs],
                  hostid->h_addr_list[numAddrs]);
            addrArr->u.ipv6AddrArr.count++; 
            numAddrs += 1;
         }
      }
      else
      {
#ifdef CMINETDBG
         /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetGetIpNodeByName() Failed : error(%d),"
               " hostName(%p), addrArr->type(%d),hostid->h_addrtype(%d) \n",
               err, hostName, addrArr->type, hostid->h_addrtype);
         CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET049, 0, prntBuf);
#endif /* CMINETDBG */
         return RFAILED;
      }
   }
   else
#endif /* IPV6_SUPPORTED */
   {
      if (hostid->h_addrtype == AF_INET)
      {
         while ((numAddrs < CM_INET_IPV4_NUM_ADDR) &&
               (hostid->h_addr_list[numAddrs] != NULLP))
         {
            addrArr->u.ipv4AddrArr.count ++;
            addrArr->u.ipv4AddrArr.netAddr[numAddrs] =
               CM_INET_NTOH_UINT32 (*((uint32_t *) hostid->h_addr_list[numAddrs]));
            numAddrs += 1;
         }
      }
      else
      {
#ifdef CMINETDBG
         /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetGetIpNodeByName() Failed : error(%d),"
               " hostName(%p), hostid->h_addrtype(%d), addrArr->type(%d)\n",
               err, hostName, hostid->h_addrtype, addrArr->type);
         CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET050, 0, prntBuf);
#endif /* CMINETDBG */
         return RFAILED;
      }
   }
#endif /* SS_LINUX */
#endif /* SUNOS */

   return ROK;
#else
   ret = cmInetGetHostByName(hostName, &addrArr->u.ipv4AddrArr); 
   return (ret);
#endif /* IPV6_SUPPORTED */

} /* end of cmInetGetIpNodeByName */


/*
*
*      Fun:   cmInetAddr
*
*      Desc:  Converts an ASCII string containig an internet address
*             ("xxx.xxx.xxx.xxx") into a CmInetIpAddr (uint32_t) format.
*             This function is a wrapper for the inet_addr() call.
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: None.
*
*      File:  cm_inet.c
*
*/

S16 cmInetAddr(
S8           *asciiAddr,        /* ascii address representation */
CmInetIpAddr *address           /* 4 byte interent address */
)
{

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if (asciiAddr == NULLP)
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   *address = inet_addr(asciiAddr);
   if (*address == (uint32_t)ERR_INADDRNONE)
   {
      /* asciiAddr does not contain a valid internet address */ 
      return RFAILED;
   }

   return ROK;
}


/*
*
*      Fun:   cmInetNtoa
*
*      Desc:  Converts an CmInetIPAddr based IP address into a string 
*             of the format "xxx.xxx.xxx.xxx".
*             This function is a wrapper for the inet_ntoa() call.
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: This function delivers a pointer to a static buffer 
*             within the system. Therefore the string has to be copied 
*             by the caller before another call is made!
*
*      File:  cm_inet.c
*
*/

S16 cmInetNtoa(
CmInetIpAddr   address,         /* 4 byte interent address */
S8           **asciiAddr        /* ascii address representation */
)
{
   struct in_addr inetAddr;     /* internet address structure */


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if (asciiAddr == NULLP)
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   inetAddr.s_addr = address;

   *asciiAddr = inet_ntoa(inetAddr);
   if (*asciiAddr == NULL)
   { 
      return RFAILED;
   }

   return ROK;
}
/*
*
*      Fun:   cmInetNtop
*
*      Desc:  Converts an network address into a string. 
*             This function is a wrapper for the inet_ntop() call.
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: This function copies the resulting string to the buffer pointed to
*              by asciiaddr,which must be a non NULL pointer.The caller specifies
*              the number of bytes available in this buffer in the argument len.
*     
*      File:  cm_inet.c
*
*/

S16 cmInetNtop(
uint8_t             type,              /* ip address type */
Void           *address,         /* 4/16 byte interent address */
S8             *asciiAddr,      /* ascii adress representation */
uint32_t            len
)
{
                             
   S32  domain = 0;

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if (asciiAddr == NULLP || address == NULLP || len == 0 )
   {
      return RFAILED;
   }
   
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   switch(type)
   {
      case CM_INET_IPV4ADDR_TYPE :
         domain = AF_INET;
         break;
      case CM_INET_IPV6ADDR_TYPE :
         domain = AF_INET6;
         break;     
   }
   if(inet_ntop(domain,address,asciiAddr,len) == NULL)
   {
      return RFAILED;
   }

   return ROK;
}


/* The inet_pton is not supported on all the Solaris Operating system 
 * versions. This has to be supported on operating systems that support 
 * IPV6 as per the RFC on the IPV6 socket interface. Hence this function
 *is moved under the IPV6_SUPPORTED flag */
#ifdef IPV6_SUPPORTED
#ifdef SUNOS

/*
*
*      Fun:   cmInetPton
*
*      Desc:  Converts a IP address string to address.
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: 
*
*      File:  cm_inet.c
*
*/

S16 cmInetPton(
CmInetIpAddr  *address,         /* 4 byte interent address */
S8           *asciiAddr         /* ascii address representation */
)
{
   S16    ret;


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((asciiAddr == NULLP) || (address == NULLP))
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   ret = inet_pton(AF_INET, asciiAddr, (void *)address);
   if (ret != 1)
   { 
      return RFAILED;
   }

   return ROK;
} /* end of cmInetPton */
#endif /* SUNOS */
#endif /* IPV6_SUPPORTED */

#ifdef IPV6_SUPPORTED

/*
*
*      Fun:   cmInetPton6
*
*      Desc:  Converts a IP address string to IPV6 address suitable 
*             to be used in bind.
*
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: 
*
*      File:  cm_inet.c
*
*/
S16 cmInetPton6(
CmInetIpAddr6  *address6,       /* 16 byte interent address */
S8             *asciiAddr       /* ascii address representation */
)
{
   S16    ret;

#ifdef WIN32
   struct sockaddr_storage ss;
   uint32_t    sslen = sizeof(ss);
#endif /* WIN32 */

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((asciiAddr == NULLP) || (address6 == NULLP))
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

#ifndef WIN32 
   ret = inet_pton(AF_INET6, asciiAddr, (void *)address6);
   if (ret != 1)
   { 
      return RFAILED;
   }
#else
   /* cm_inet_c_001.main_44 : In windows inet_pton is not implemented. so we are using the below function
    * to convert the ipv6 address string to appropriate form */
   WSAStringToAddressA((LPTSTR)asciiAddr, AF_INET6, NULL, (struct sockaddr*)&ss, &sslen);
   memcpy(address6, &(((struct sockaddr_in6 *)&ss)->sin6_addr), sizeof(CmInetIpAddr6));
#endif /* WIN32 */

   return ROK;
} /* end of cmInetPton6 */
#endif /* IPV6_SUPPORTED */
#endif /* SS_PS */


/*
*
*      Fun:   cmInetGetMemSize
*
*      Desc:  This function gives the max number of static buffer space that
*             the internet library will allocate. 
*
*      Ret:   ROK - successful
*
*      Notes: None.
*
*      File:  cm_inet.c
*
*/

S16 cmInetGetMemSize(
S32 *size                       /* max used memory size */
)
{
#ifdef WIN32
   /* max static memory size depends on max flat buffer size */
   *size = CM_INET_MAX_MSG_LEN;
#else   
   /* max static memory size depends on max flat buffer or iovect size */
   *size = CM_INET_MAX_MSG_LEN;
#endif 

   return ROK;
}



/*
*
*      Fun:   cmInetInit
*
*      Desc:  This function initializes the socket library.
*
*      Ret:   ROK - successful
*
*      Notes: Required only for Winsock and not for 4.3BSD
*
*      File:  cm_inet.c
*
*/
 
S16 cmInetInit(Void)
{
#ifdef WIN32
   uint16_t     version;
   S32     err;
   WSADATA data;

   version = MAKEWORD(CM_INET_HIGH_VER, CM_INET_LOW_VER);
   err = WSAStartup(version, &data);
   if (err != 0)
   {
      return RFAILED;
   }
#endif

   return ROK;
}


/*
*
*      Fun:   cmInetDeInit
*
*      Desc:  This function de initializes the socket library. The
*             WINSOCK implementation de registers the application and
*             releases any resources allocated on behalf of the
*             application.
*
*      Ret:   ROK - successful
*
*      Notes: Required only for Winsock and not for 4.3BSD
*
*      File:  cm_inet.c
*
*/
 
S16 cmInetDeInit(Void)
{
#ifdef WIN32
   S32     err;

   err = WSACleanup();
   if (err != 0)
   {
      return RFAILED;
   }
#endif

   return ROK;
}/* end of cmInetDeInit() */


/*
*
*      Fun:   cmInetGetSockName
*
*      Desc:  This function is used to retireve the current name 
*             for the specified socket descriptor. It returns the 
*             local association(address and port) for the socket.
* 
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: Please note if the socket was bound to CM_INET_INADDR_ANY
*             cmInetGetSockName() will not necessarily return the local
*             address information unless the socket has been connected.
*
*      File:  cm_inet.c
*
*/

S16 cmInetGetSockName
(
CmInetFd *sockFd,               /* socket file descriptor */ 
CmInetAddr *locAddr
) 
{
   struct sockaddr_in *sockAddr; 
#ifdef IPV6_SUPPORTED
   struct sockaddr_in6 *sockAddr6;
   struct sockaddr_in6 lclSockAddr;
#else
   CmInetSockAddr lclSockAddr;
#endif /* IPV6_SUPPORTED */
#ifdef UNIX
   socklen_t size;
#else
   uint32_t  size;
#endif /* SS_LINUX */
   S32  errCode;
   /*cm_inet_c_001.main_58  : fix for klockwork issue */
   S32  ret;


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd) ||
         (locAddr == NULLP))
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   memset(&lclSockAddr, 0, sizeof(lclSockAddr));
   size = sizeof(lclSockAddr);

#ifdef UNIX
   ret = getsockname(sockFd->fd, (CmInetSockAddr*)&lclSockAddr, 
         (socklen_t *)&size);
#else
   ret = getsockname(sockFd->fd, (CmInetSockAddr*)&lclSockAddr, (int*)&size);
#endif /* SS_LINUX */

   if(ret == INET_ERR)
   {
      switch(errCode = INET_ERR_CODE)
      {
         case ERR_INVAL:
            sockAddr = (struct sockaddr_in *)&lclSockAddr;
#ifdef IPV6_SUPPORTED
            locAddr->type = CM_INET_IPV4ADDR_TYPE;
            locAddr->u.ipv4Addr.port = CM_INET_NTOH_UINT16(sockAddr->sin_port);
#else
            locAddr->port = CM_INET_NTOH_UINT16(sockAddr->sin_port);
#endif /* IPV6_SUPPORTED */
            return ROK;

         default:
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
            /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetGetHostByName() Failed : error(%d),"
                  " sockFd->fd(%ld)\n", INET_ERR_CODE, sockFd->fd);
            CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET051, 0, prntBuf);
#else
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetGetHostByName() Failed : error(%d),"
                  " sockFd->fd(%d)\n", INET_ERR_CODE, sockFd->fd);
            CMINETLOGERROR(ERRCLS_INT_PAR, ECMINET051, 0, prntBuf);
#endif /* ALIGN_64BIT */
#endif /* CMINETDBG */
            return RFAILED;
      }/* end of switch */

   }/* end if */

   /* Fill the returned address in to locAddr */
#ifdef IPV6_SUPPORTED
   memset(locAddr, 0, sizeof(CmInetAddr));
   if (size == sizeof(struct sockaddr_in6))
   {
      sockAddr6 = (struct sockaddr_in6 *)&lclSockAddr;
      locAddr->type = CM_INET_IPV6ADDR_TYPE;
      locAddr->u.ipv6Addr.port = CM_INET_NTOH_UINT16(sockAddr6->sin6_port);
      CM_INET_COPY_IPV6ADDR(&locAddr->u.ipv6Addr.ipv6NetAddr, 
            &sockAddr6->sin6_addr);
   }
   else
   {
      sockAddr = (struct sockaddr_in *)&lclSockAddr;
      locAddr->type = CM_INET_IPV4ADDR_TYPE;
      locAddr->u.ipv4Addr.port = CM_INET_NTOH_UINT16(sockAddr->sin_port);
      locAddr->u.ipv4Addr.address = 
         CM_INET_NTOH_UINT32(sockAddr->sin_addr.s_addr);
   }
#else
   sockAddr = (struct sockaddr_in *)&lclSockAddr;
   locAddr->port    = CM_INET_NTOH_UINT16(sockAddr->sin_port);
   locAddr->address = CM_INET_NTOH_UINT32(sockAddr->sin_addr.s_addr);
#endif /* IPV6_SUPPORTED */
   return ROK;
}/* end of cmInetGetSockName() */

/*  New functions to peek into the file descriptor 
 * set */
#if (defined(SUNOS) || defined(WIN32) || defined(SS_LINUX) || defined(SS_VW) \
      || defined(HPOS))

/*
*
*      Fun:   cmInetFdSetInfoInit
*
*      Desc:  This function is used to initialise operating system specific
*             data that will be used to peek into the file descriptor lists
*             to get the sockets that are set 
* 
*      Ret:   ROK     - successful
*             RFAILED - failed
*
*      Notes: 
*
*      File:  cm_inet.c
*
*/

S16 cmInetFdSetInfoInit
(
CmInetFdSetInfo *fdSetInfo
) 
{
#if (defined(SUNOS) || defined(SS_LINUX) || defined(SS_VW) || defined(HPOS))
   uint16_t   arIdx;
   uint8_t    curByte;
   uint8_t    bitPos;
   CmInetFdSet *fdSet;
#endif /* SUNOS || SS_LINUX || SS_VW */

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (fdSetInfo == NULLP)
      return RFAILED;
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   if (fdSetInfo->initDone == TRUE)
      return ROK;

#ifdef WIN32
   fdSetInfo->numFds = 0;
#endif /* WIN32 */

#if (defined(SUNOS) || defined(SS_LINUX) || defined(SS_VW)|| defined(HPOS))
   /* Check if we are on a big endian machine */
   arIdx = 0x01;
   if (*(uint8_t *)&arIdx)
      fdSetInfo->bigEndian = FALSE;
   else
      fdSetInfo->bigEndian = TRUE;

   fdSetInfo->arIdx = 0;
   fdSetInfo->ar[0] = 0xff;

   /* Initialise the array */
   /* The array contains bit positions for the first bit 
    * for each integer from 1 to 2^8. 
    */
   for (arIdx = 1; arIdx < 256; arIdx++)
   {
      /* cm_inet_c_001.main_54: Fix for Klockworks issue */
      curByte = (uint8_t)arIdx;
      bitPos = 0;

      while(bitPos < 8)
      {
         if (curByte & 0x01)
         {
            fdSetInfo->ar[arIdx] = bitPos;
            break;
         }
         bitPos += 1;
         curByte = curByte >> 1;
      }
   }
   /* Calculate the number of array elements in this fd_set */
#if (defined(SS_LINUX)  &&  !defined(_GNU_SOURCE))
   fdSetInfo->numArElems = sizeof(CmInetFdSet)/sizeof(fdSet->__fds_bits[0]);
#else
   fdSetInfo->numArElems = sizeof(CmInetFdSet)/sizeof(fdSet->fds_bits[0]);
#endif /* SS_LINUX */
#endif /* SUNOS  || SS_LINUX || SS_VW || HPOS */   

   fdSetInfo->initDone = TRUE;
   return ROK;
}/* end of cmInetFdSetInfoInit() */


/*
*
*      Fun:   cmInetGetFd
*
*      Desc:  This function is used to get the file descriptor from the
*             file descriptor set.
* 
*      Ret:   ROK     - successful
*             ROKDNA  - socket not found
*             RFAILED - failed
*             RNA     - failed, initialisation not done
*
*      Notes: If the application modifies fdSet between calls to this
*             function then the results are undefined. This function should
*             be called in a loop till either it returns - not ROK, or if 
*             all sockets in the file descriptor set are processed. 
*
*      File:  cm_inet.c
*
*/

S16 cmInetGetFd
(
CmInetFdSetInfo *fdSetInfo,
CmInetFdSet     *fdSet,
CmInetFdType    *sockFd
) 
{
  /*cm_inet_c_001.main_58 : Fix for klockwork issue */
#if (!defined (WIN32))
   uint32_t sizOfFdSetArElem;  
   uint8_t bytesScanned;
   Bool found;
   uint32_t curIdx;
   uint8_t *tempByte;
   uint8_t bitPos;
#endif /* !defined (WIN32) */

#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((fdSetInfo == NULLP) || (fdSet == NULLP) || (sockFd == NULLP))
      return RFAILED;

   if (fdSetInfo->initDone != TRUE)
      return (RNA);
#endif /* ERRCLASS & ERRCLS_INT_PAR */

#ifdef WIN32
#if (ERRCLASS & ERRCLS_DEBUG)
   if (fdSetInfo->numFds > FD_SETSIZE)
      return RFAILED;
#endif /* ERRCLASS & ERRCLS_DEBUG */
   /* cm_inet_c_001.main_32 :  Corrected check for number of fd set in
      a fdset for WIN32*/
   if (fdSetInfo->numFds >= fdSet->fd_count)
      return (ROKDNA);

   *sockFd = fdSet->fd_array[fdSetInfo->numFds];
   fdSetInfo->numFds += 1;
   return ROK;
#endif /* WIN32 */

   /* cm_inet_c_001.main_59: Protected under if not defined WIN32 */
#if (!defined (WIN32))
   /* Start with arIdx and continue upto number of array elements. */
   curIdx = fdSetInfo->arIdx;
   found = FALSE;

#if (defined(SS_LINUX)  &&  !defined(_GNU_SOURCE))
   sizOfFdSetArElem = sizeof(fdSet->__fds_bits[0]);
#else
   sizOfFdSetArElem = sizeof(fdSet->fds_bits[0]);
#endif /* SS_LINUX */

   for (curIdx = fdSetInfo->arIdx; curIdx < fdSetInfo->numArElems;
         curIdx ++)
   {
#if (defined(SS_LINUX)  &&  !defined(_GNU_SOURCE))
      if (fdSet->__fds_bits[curIdx])
#else
         if (fdSet->fds_bits[curIdx])
#endif /* SS_LINUX */
         {
            /* Walk through the bytes in this element */
#if (defined(SS_LINUX)  &&  !defined(_GNU_SOURCE))
            tempByte = (uint8_t *)&fdSet->__fds_bits[curIdx];
#else
            tempByte = (uint8_t *)&fdSet->fds_bits[curIdx];
#endif /* SS_LINUX */

            /* Set the starting byte offset */
            if (fdSetInfo->bigEndian)
               tempByte += sizOfFdSetArElem - 1;

            for (bytesScanned = 0; bytesScanned < sizOfFdSetArElem; 
                  bytesScanned ++)
            {
               if (*tempByte)
               {
                  bitPos = fdSetInfo->ar[*tempByte];
                  /* cm_inet_c_001.main_54: Fix for Klockworks issue */
                  fdSetInfo->arIdx = (uint16_t)curIdx;
                  /* Calculate fd depending on where we are */
                  *sockFd = ((bytesScanned << 3) + bitPos);
                  *sockFd += (curIdx  * (sizOfFdSetArElem << 3));
                  /* Clear the file descriptor */
                  *tempByte &= ~(1 << bitPos);
                  return ROK;
               }
               if (fdSetInfo->bigEndian)
                  tempByte -= 1;
               else
                  tempByte += 1;
            }
            break;
         }
   }

   if (!found)
      return (ROKDNA);

   return ROK;
#endif /* SUNOS || SS_LINUX || SS_VW || HPOS */
} /* end of cmInetGetFd */

#endif /* SUNOS || WIN32  || SS_LINUX || SS_VW || HPOS  */ 


/* add cmInetConvertStrToIpAddr and
 * cmInetAsciiToIpv4 functions */
/*
*
*       Fun:   cmInetConvertStrToIpAddr
*
*       Desc:  This function parses the input string for an IPV4/IPV6 address.
*              formats:
*              1) IPV4 in dot number format:
*                    206.216.108.253
*              2) IPV6, in uncompressed, compressed, and IPV4 embedded format 
*                    10:20:30:40:502:610:70C:80ad
*                    A5::34:45
*                    45::AB:34:123.34.5.667
*
*       Ret:   ROK     - SUCCESS
*              RFAILED - FAILURE
*
*       Notes: 
*
*       File:  cm_inet.c
*
*/

S16 cmInetConvertStrToIpAddr
(
uint16_t                len,                /* Length of IP address */
uint8_t                 *val,               /* Domain Name String */
CmInetNetAddr      *address            /* IP Address */
)
{
   uint8_t              idx;                /* Index for string*/
   uint8_t              ipv4[CM_INET_IPV4ADDR_SIZE]; /* IPV4 Address bytes */
#ifdef IPV6_SUPPORTED
   uint16_t             *ipv6;                 /* IPV6 Address bytes */
   uint16_t             ipv6Reg[8];           /* regular IPV6 Address bytes */
   uint16_t             ipv6Cmp[8];           /* compressed IPV6 Address bytes */
   uint8_t              numBlk;               /* number of blocks in IPV6 addr */
   Bool            compressed;           /* IPV6 in compressed format */
   uint8_t              ipv6Idx;              /* counter for IPV6 */
   uint8_t              blkBeginIdx;          /* IPV6, char index for the 
                                            beginning of the block */
   uint8_t              i;                    /* counter for IPV6 */
   S16             retVal;               /* return value */
   Bool            embedIPV4 = FALSE;    /* IPV4 embedded in IPV6 ? */
#endif /* IPV6_SUPPORTED*/


      idx = 0;
#ifdef IPV6_SUPPORTED
   numBlk = 0;
   ipv6Idx = 0;
   compressed = FALSE;
   embedIPV4 = FALSE;
   ipv6 = ipv6Reg; /* assign pointer to IPV6 regular, uncompressed */
   memset(ipv6Reg, 0, CM_INET_IPV6ADDR_SIZE);
   memset(ipv6Cmp, 0, CM_INET_IPV6ADDR_SIZE);
#endif /* IPV6_SUPPORTED*/

   memset(ipv4, 0, CM_INET_IPV4ADDR_SIZE);

   /* Check for IP Address */
   while ((val[idx] != '.') && (val[idx] != ':') && 
         (idx < len))
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      if (((val[idx] < '0') || (val[idx] > '9')) &&
            ((val[idx] < 'a') || (val[idx] > 'f')) &&
            ((val[idx] < 'A') || (val[idx] > 'F')))
      {
         /* Not a digit */
         return RFAILED;
      }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

      /* Convert Ascii to integer */
      CM_INET_ATOI(ipv4[0], val[idx]);

#ifdef IPV6_SUPPORTED
      /* convert Ascii to hex */
      CM_INET_ATOH(ipv6[0], val[idx]);
#endif /* IPV6_SUPPORTED */

      idx++; /* move to the next character */
   } /* while, try to determine IPV4 or IPV6 */

#if (ERRCLASS & ERRCLS_DEBUG)
   if ((val[idx] != '.') && (val[idx] != ':'))
   {
      /* Not a digit */
      return RFAILED;
   } /* if, couldn't determine IPV4 or IPV6 */
#endif /* (ERRCLASS & ERRCLS_DEBUG) */


   if (val[idx] == '.')
   {
      idx++;
      cmInetAsciiToIpv4(3, &(ipv4[1]), (uint16_t)(len - idx), &(val[idx]));

      address->type = CM_INET_IPV4ADDR_TYPE;
      CM_INET_GET_IPV4_ADDR_FRM_STRING(address->u.ipv4NetAddr, ipv4);
   } /* if, IPV4 */
#ifdef IPV6_SUPPORTED
   else 
   {
      numBlk = 1; /* already converted the 1st block */
      ipv6Idx = 0;
      while ((val[idx] != '\0') && (idx < len) && (numBlk <= 8))
      {
         idx++; /* go to the next char, either a number or the 2nd : */
         if (val[idx] == ':')
         {
#if (ERRCLASS & ERRCLS_DEBUG)
            if (compressed == TRUE)
            {
               /* can't have 2 :: */
               return RFAILED;
            } /* if, 2 :: */
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

            compressed = TRUE;
            idx++; /* skip the : */
            ipv6 = ipv6Cmp;
            ipv6Idx = 0;
         } /* if, IPV6 in compressed format :: */
         else
         {
            ipv6Idx++;
         } /* else, uncompressed, convert next block */

         numBlk++; /* increase number of blocks */

         /* assign the index the beginning of the block */
         blkBeginIdx = idx;

         while(val[idx] != ':' && val[idx] != '\0' && idx < len)
         {
            if (val[idx] == '.')
            {
               /* convert number to IPV4 */
               ipv6[ipv6Idx] = 0; /* clear out whatever we did */
               memset(ipv4, 0, CM_INET_IPV4ADDR_SIZE);
               retVal = cmInetAsciiToIpv4(4, ipv4, len - blkBeginIdx, 
                     &(val[blkBeginIdx]));
               /* stop the loop, embedded IPV4 is the last part of
                  an IPV6 address */
               if (retVal != ROK)
               {
                  return (retVal);
               }
               embedIPV4 = TRUE;
               break;
            } /* if, '.' means IPV4 address embedded in IPV6 */

#if (ERRCLASS & ERRCLS_DEBUG)
            if (((val[idx] < '0') || (val[idx] > '9')) &&
                  ((val[idx] < 'a') || (val[idx] > 'f')) &&
                  ((val[idx] < 'A') || (val[idx] > 'F')))
            {
               /* Not a digit */
               return RFAILED;
            }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

            /* Convert Ascii to integer */
            CM_INET_ATOH(ipv6[ipv6Idx], val[idx]);

            /* move to the next index */
            idx++;
         } /* while, convert a block of 16 bits Hex number */
         if (embedIPV4 == TRUE)
         {
            ipv6Idx--; /* deccrease in case of compressed IPV6 */
            break; /* stop the while look */
         } /* if, IPV4 embedded in IPV6 */
      } /* while, IPV6 parsing */
      if (compressed == TRUE)
      {
         if (embedIPV4 == TRUE)
         {
            numBlk = 5; /* the last 2 blocks are IPV4 */
         } /* if, IPV4 embedded */
         else
         {
            numBlk = 7; /* copy from the last block */
         } /* else, no embedded IPV4 */

         /* type cast uint8_t over -1 becasue we want to copy the last block,
            ipv6Cmp[0]
            */
         for (i = ipv6Idx; i != (uint8_t) (-1); i --)
         {
            ipv6Reg[numBlk] = ipv6Cmp[i];
            numBlk--;
         } /* for, copying compress IPV6 to regular IPV6 */
      } /* if, compressed format */

      if (embedIPV4 == TRUE)
      {
         ipv6Reg[6] = PutHiByte(ipv6Reg[6], ipv4[0]);
         ipv6Reg[6] = PutLoByte(ipv6Reg[6], ipv4[1]);
         ipv6Reg[7] = PutHiByte(ipv6Reg[7], ipv4[2]);
         ipv6Reg[7] = PutLoByte(ipv6Reg[7], ipv4[3]);
      } /* if, IPV4 embedded */

      /* convert IPV6 to cmInetIpv6 */
      address->type = CM_INET_IPV6ADDR_TYPE;
      memcpy(address->u.ipv6NetAddr,
             ipv6Reg,  CM_INET_IPV6ADDR_SIZE);
   } /* else, IPV6 */
#endif /* IPV6_SUPPORTED */

   return ROK;
} /* cmInetConvertStrToIpAddr */


/*
*
*       Fun:   cmInetAsciiToIpv4
*
*       Desc:  This function parses the input string to an IPV4 address.
*              The input string can be 
*              - the whole IPV4 address, '123.43.45.56', or
*              - a part of it. '34.56.454'
*              numBytes: number of bytes needs to be converted, IPV4 has
*                        4 bytes. If we are only converting the end of an
*                        address, this number needs to be adjusted. For
*                        example, when converting '34.56.454]', the number
*                        is 3.
*
*       Ret:   ROK     - SUCCESS
*              RFAILED - FAILURE
*
*       Notes: 
*
*       File:  cm_inet.c
*
*/
S16  cmInetAsciiToIpv4
(
uint8_t   numBytes,           /* number of Byte to convert */
uint8_t   *ipv4Addr,          /* IPV4 Address */
uint16_t  len,                /* Length of IP address */
uint8_t   *val                /* Domain Name String */
)
{
   uint8_t byteCount;          /* Byte Count */
   uint8_t idx;                /* Index for string*/


      idx = 0;
   for (byteCount = 0; byteCount < numBytes; byteCount++)
   {
      while((val[idx] != '.') && (idx < len))
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         if (val[idx] < '0' || val[idx] > '9')
         {
            /* Not a digit */
            return RFAILED;
         }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

         /* Convert Ascii to integer */
         CM_INET_ATOI(ipv4Addr[byteCount], val[idx]);

         /* move to the next index */
         idx++;
      }
      idx++;
   }

   return ROK;
} /* cmInetAsciiToIpv4 */

/* cm_inet_c_001.main_34:Added wrapper function for getaddrinfo and freeaddrinfo */
#if (!defined(SS_VW) && !defined(SS_PS) && !defined(WIN32))

/*
*
*      Fun:   cmInetGetAddrInfo 
*
*      Desc:  a socket file descriptor to a local Internet 
*             address/port.
*
*      Ret:   Value returned by getaddrinfo 
*
*      Notes: None.
*
*      File:  cm_inet.c
*
*/

S32 cmInetGetAddrInfo
(
const S8              *node,          /* Network addr which has to be resolved */
const S8              *service,       /* Sets the port number in network addr */
const CmInetAddrInfo  *hints,         /* Specifies preferred socket type or protocol */
CmInetAddrInfo           **res           /* Link list of addrInfo structure */
)
{
   S32 ret;
   ret = ROK;

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if ((node == NULLP) || (hints == NULLP))
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   ret = getaddrinfo(node,service,hints,res); 
   if (ret != ROK)
   {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetGetAddrInfo() Failed : error(%ld), node(%p),"
            " service(%p)\n",  ret, node, service);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET052, 0, prntBuf);
#else
      /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
      /* cm_inet_c_001.main_62:Warning fix */
      snprintf(prntBuf, CMINET_PRNT_BUF_SIZE, "cmInetGetAddrInfo() Failed : error(%d), node(%p),"
            " service(%p)\n ", ret,  node, service);
      CMINETLOGERROR(ERRCLS_DEBUG, ECMINET053, 0, prntBuf);
#endif /* ALIGN_64BIT */
#endif /* CMINETDBG */
   }
   return (ret); 
} /* end of cmInetGetAddrInfo */


/*
*
*      Fun:   cmInetFreeAddrInfo 
*
*      Desc:  Free the dynamically allocated addrinfo structure 
*
*      Ret:   None 
*
*      Notes: None.
*
*      File:  cm_inet.c
*
*/

Void cmInetFreeAddrInfo
(
CmInetAddrInfo *res           /* Link list of addrInfo structure */
)
{

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* error check on parameters */
   if (res == NULLP) 
      return;   
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   freeaddrinfo(res); 
} /* end of cmInetFreeAddrInfo */

#endif /* SS_VW | SS_PS | WIN32*/

/* cm_inet_c_001.main_36 : 1. Added new interface - cmInetFlushRecvBuf()
   to flush the data from socket receive buffer. */
#ifdef CM_INET_FLUSH_RECV_BUF

/*
*
*      Fun:   cmInetFlushRcvBuf
*
*      Desc:  Reads all the data from a socket and throw it!!
*             The buffers for the receive buffer for recvfrom() are allocated from the stack. 
*
*      Ret:   ROK     - successful
*             ROKDNA  - ok, data not available
*             RCLOSED - connection closed by peer
*             ROUTRES - failed, out of resources
*             RFAILED - failed
*
*      Notes: None.
*
*      File:  cm_inet.c
*
*/
S16 cmInetFlushRecvBuf
(
CmInetFd        *sockFd,        /* socket file descriptor */ 
MsgLen          *len,           /* number of octects to be flushed */
S32              flags          /* additional control flags */
)
{

   Data recvTempBuf[CM_INET_MAX_BYTES_READ];

#if (defined(WIN32) || defined(CMINETFLATBUF))
   S32           ret;            /* temporary return value */
   uint32_t           pendLen;        /* pending data length */
   S32           recvLen;        /* number of received octets by recvmsg() */
   MsgLen        curLen;         /* current number of octets in buffer */ 
   uint32_t           remAddrLen;     /* length of remote address */
   struct sockaddr_in  *remAddr;    /* remote Internet address */       
#ifdef IPV6_SUPPORTED 
   struct sockaddr_in6 remSockAddr; /* to get packet's source IP address */
#else
   CmInetSockAddr  remSockAddr;     /* to get packet's source IP address */
#endif /* IPV6_SUPPORTED */
#else
   S32           ret;            /* temporary return value */
   MsgLen        curLen;         /* current number of octets in buffer */ 
   uint32_t           pendLen;        /* pending data length */
   S32           recvLen;        /* number of received octets by recvmsg() */
   struct msghdr msg;            /* message header */ 
   CmInetIovec  rxArr[CM_INET_MAX_DBUF]; /* dynamic gather array */
   uint32_t           remAddrLen;     /* length of remote address */
#ifdef IPV6_SUPPORTED 
   struct sockaddr_in6 remSockAddr;/* to get packet's source IP address */

#if (defined(SS_LINUX) || defined(_XPG4_2))
   uint8_t                   ancillData[CM_INET_IPV6_ANCIL_DATA];
   /* from stack for IPv6 ancill data */
#endif
#else
   CmInetSockAddr       remSockAddr;     /* to get packet's src IP address */
#if (defined(SS_LINUX) || defined(_XPG4_2))
   uint8_t                   ancillData[CM_INET_IPV4_ANCIL_DATA];
   /* from stack for IPv4 ancill data */
#endif
#endif /* IPV6_SUPPORTED */
#endif /* WIN32 | CMINETFLATBUF */

   /* used by getsockopt */
   uint32_t          errValue;                /* error value */
   uint32_t          optLen;                  /* option length */


#if (ERRCLASS & ERRCLS_INT_PAR)
      /* error check on parameters */
      if ((sockFd == NULLP) || CM_INET_INV_SOCK_FD(sockFd))
      {
         return RFAILED;
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */


#if (defined(WIN32) || defined(CMINETFLATBUF))
   remAddr = NULLP;  
#endif /* (WIN32 | CMINETFLATBUF) */

   /* clear the structure */   
   memset(&remSockAddr, 0, sizeof(remSockAddr));

   /* get number of pending data */
   ret = cmInetGetNumRead(sockFd, &pendLen);
   if (ret != ROK)
   {
      /* ret may be RFAILED or ROUTRES */
      return (ret);
   }

   /* check if connection got closed */
   if (pendLen == 0)
   {
      if (sockFd->type == CM_INET_STREAM)
      {

         /* cm_inet_c_001.main_50 
          * Due to latency (mostly in solaris) sometimes ioctl(FIONREAD)
          * (inside cmInetGetNumRead) returns pend length as 0 on a TCP 
          * socket that select says is ready to read. This should not be 
          * considered as connection closed. So return ROKDNA instead of 
          * RCLOSED
          */
         return (ROKDNA);
      }
      else
         /* clear error if there is any, because if there is internal error
          * here it will cause infinite loop in TUCL */
      {
         errValue = 0;
         optLen = sizeof(int);
#ifdef UNIX
         ret = getsockopt(sockFd->fd, SOL_SOCKET, SO_ERROR,
               (char*)&errValue, (socklen_t *)&optLen);
#else
#if (defined(SS_VW) || defined(SS_PS))
         ret = getsockopt(sockFd->fd, SOL_SOCKET, SO_ERROR,
               (char*)&errValue, (int *)&optLen);
#else
#ifndef SS_WINCE
         ret = getsockopt(sockFd->fd, SOL_SOCKET, SO_ERROR,
               (char*)&errValue, (int *)&optLen);
#endif /* SS_WINCE */
#endif /* SS_VW */
#endif /* SS_LINUX */
         if (ret == INET_ERR)
         {
#ifdef CMINETDBG
#ifndef ALIGN_64BIT
            /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetFlushRecvBuf() Failed : error(%d),"
                  " sockFd->fd(%ld)\n", INET_ERR_CODE, sockFd->fd);
            CMINETLOGERROR(ERRCLS_DEBUG, ECMINET054, 0, prntBuf);
#else
            /* cm_inet_c_001.main_62:Warning fix */
            snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetFlushRecvBuf() Failed : error(%d),"
                  " sockFd->fd(%d)\n", INET_ERR_CODE, sockFd->fd);
            CMINETLOGERROR(ERRCLS_DEBUG, ECMINET054, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */
            return RFAILED;
         }
         else
         {
            uint8_t tempBuf;
            /* added separate recvfrom calls different OS */
#if( defined(SS_VW) || defined(HPOS) || defined(SS_PS))
            recvLen = recvfrom(sockFd->fd, (S8 *)&tempBuf, pendLen, 0, 
                  (struct sockaddr *)&remSockAddr, (int *)&remAddrLen);
#else
#if ( defined(SUNOS) || defined(SS_LINUX))
            recvLen = recvfrom(sockFd->fd, (S8 *)&tempBuf, pendLen, 0,
                  NULLP, (socklen_t *)&remAddrLen);
#else
            recvLen = recvfrom(sockFd->fd, (S8 *)&tempBuf, pendLen, 0,
                  NULLP, (S32 *)&remAddrLen);

#endif /* defined(SUNOS) || defined(SS_LINUX) */
#endif /* defined(SS_VW) || defined(HPOS) || defined(SS_PS) */

            return (ROKDNA);
         }      
      }
   }/* if (pendLen == 0)*/


   if((*len == CM_INET_READ_THROW) || (*len >= CM_INET_MAX_BYTES_READ))
   {
      curLen = CM_INET_MAX_BYTES_READ;
   }
   else
   {
      curLen = *len; /*set to given number of messasges to be flushed */
   }

   if((*len != CM_INET_READ_THROW) && (*len < pendLen))
   {
      pendLen = *len;
   }

#if (defined(WIN32) || defined(CMINETFLATBUF))

   remAddrLen = 0;
   /* 
    * maybe needs more than one recvfrom() call to read an entire 
    * message 
    */
   while (curLen > 0)
   {
      memset(recvTempBuf, 0, CM_INET_MAX_BYTES_READ);
      /* added separate recvfrom calls different OS */

#if( defined(SS_VW) || defined(HPOS) || defined(SS_PS))
      recvLen = recvfrom(sockFd->fd, (S8 *)recvTempBuf, curLen, 0, 
            (struct sockaddr *)&remSockAddr, (int *)&remAddrLen);
#else         
#if ( defined(SUNOS) || defined(SS_LINUX))
      recvLen = recvfrom(sockFd->fd, (S8 *)recvTempBuf, curLen, 0, 
            (struct sockaddr *)&remSockAddr, (socklen_t *)&remAddrLen); 
#else
      recvLen = recvfrom(sockFd->fd, (S8 *)recvTempbuf, curLen, 0, 
            &remSockAddr, (S32 *)&remAddrLen); 

#endif /* defined(SUNOS) || defined(SS_LINUX) */
#endif /* defined(SS_VW) || defined(HPOS) || defined(SS_PS) */ 

      if (recvLen == INET_ERR)
      {

         /*  added check ERR_WOULDBLOCK */
         if ((INET_ERR_CODE == ERR_AGAIN) ||
               (INET_ERR_CODE == ERR_WOULDBLOCK))
         {
            *len = 0; 
            return (ROKDNA);
         }


         /*  In Windows the recvfrom function fails
          *  with error code which maps to either WSAECONNABORTED. If
          *  this happens then cmInetFlushRecvBuf must return RCLOSED */
         if ((INET_ERR_CODE == ERR_CONNABORTED) || 
               (INET_ERR_CODE == ERR_CONNRESET))
         {
            *len = 0;
            return (RCLOSED);
         }

#ifdef CMINETDBG
#ifndef ALIGN_64BIT
         /* cm_inet_c_001.main_54 : CMINETLOGERROR added insted of SDisp */
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetFlushRecvBuf() Failed : error(%d),"
               " sockFd->fd(%ld)\n", INET_ERR_CODE, sockFd->fd);
         CMINETLOGERROR(ERRCLS_DEBUG, ECMINET055, 0, prntBuf);
#else
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetFlushRecvBuf() Failed : error(%d),"
               " sockFd->fd(%d)\n", INET_ERR_CODE, sockFd->fd);
         CMINETLOGERROR(ERRCLS_DEBUG, ECMINET055, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */

         return RFAILED;
      } 

      if(recvLen < curLen)
         break;

      pendLen -= recvLen;

      if(pendLen < curLen)
         curLen = pendLen;

   } /* while (curLen > 0)  */ 

#else  /* end of Win NT/flat buffer specific part */

   /* 
    * maybe needs more than one recvmsg() call to read entire message 
    * on a stream socket 
    */
   while (curLen > 0)
   {
      memset(recvTempBuf, 0, CM_INET_MAX_BYTES_READ);
      /* update the message structure */
#ifdef SS_LINUX
      rxArr[0].iov_base = (Void*)recvTempBuf;  
      rxArr[0].iov_len = (uint32_t)curLen;    
#else
      rxArr[0].iov_base = (S8*)recvTempBuf;
      rxArr[0].iov_len = curLen; 
#endif /* SS_LINUX */
      msg.msg_iov           = rxArr;
      msg.msg_iovlen        = 1;

      msg.msg_name    = NULLP;
      msg.msg_namelen = 0;

      /* added defined(_XPG4_2). Also changed the
       * assignments */
#if (defined(SS_LINUX) || defined(_XPG4_2))
      msg.msg_control      = ancillData;
      msg.msg_controllen   = sizeof(ancillData);
      msg.msg_flags   = 0; 
#else
      msg.msg_accrights     = NULLP;
      msg.msg_accrightslen  = 0;
#endif /* SS_LINUX */

      recvLen = recvmsg(sockFd->fd, &msg, flags);
      if ((recvLen == INET_ERR) || (recvLen > CM_INET_MAX_MSG_LEN))
      {
         /* added check ERR_AGAIN when CMINETFLATBUF is not defined. 
            added check ERR_WOULDBLOCK */
         if ((INET_ERR_CODE == ERR_AGAIN) ||
               (INET_ERR_CODE == ERR_WOULDBLOCK))
         {
            *len = 0;  
            return (ROKDNA);
         }

#ifdef CMINETDBG
#ifndef ALIGN_64BIT
         /* cm_inet_c_001.main_54: CMINETLOGERROR added insted of SDisp */
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetFlushRecvBuf() Failed : error(%d),"
               " sockFd->fd(%ld)\n", INET_ERR_CODE, sockFd->fd);
         CMINETLOGERROR(ERRCLS_DEBUG, ECMINET056, 0, prntBuf);
#else
         /* cm_inet_c_001.main_62:Warning fix */
         snprintf(prntBuf, CMINET_PRNT_BUF_SIZE,"cmInetFlushRecvBuf() Failed : error(%d),"
               " sockFd->fd(%d)\n", INET_ERR_CODE, sockFd->fd);
         CMINETLOGERROR(ERRCLS_DEBUG, ECMINET056, 0, prntBuf);
#endif /*ALIGN_64BIT*/
#endif /* CMINETDBG */

         /*  If this happens then cmInetFlushRecvBuf must return RCLOSED. 
          *  Needed for getting icmp msgs */
         if (INET_ERR_CODE == ERR_CONNABORTED)
         {
            *len = 0;
            return (RCLOSED);
         }
         return RFAILED; 
      }/* if ((recvLen == INET_ERR) || (recvLen > CM_INET_MAX_MSG_LEN))*/ 

      if(recvLen < curLen)
         break;

      pendLen -= recvLen;

      if(pendLen < curLen)
         curLen = pendLen;

   } /* while(curLen > 0) */

#endif /* WIN32 | CMINETFLATBUF  */


   return ROK;
} /* end of cmInetFlushRecvBuf */

#endif /* CM_INET_FLUSH_RECV_BUF*/

/**********************************************************************
         End of file
**********************************************************************/
