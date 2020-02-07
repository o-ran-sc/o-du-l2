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

#include "cu_stub.h"
#include "cm_inet.h"
#include "cm_tpt.h"

#include "cm_inet.x"
#include "cm_tpt.x"

#define MAX_RETRY 5

/* Global variable declaration */
CmInetFd   lstnSockFd; /* Listening Socket file descriptor */
CmInetFd   sockFd;     /* Socket File descriptor */
U8   socket_type;      /* Socket type */
Bool nonblocking;      /* Blocking/Non-blocking socket */
Bool connUp;           /* Is connection up */
int  assocId;          /* Assoc Id of connected assoc */
  
CmInetNetAddrLst localAddrLst;
CmInetNetAddrLst remoteAddrLst;
  
SctpParams *sctpCfg;            /* SCTP configurations at DU */

EXTERN S16 sctpActvInit();
EXTERN void sctpStartReq();
EXTERN S16 sctpSend(Buffer *mBuf);

#endif

/**********************************************************************
         End of file
 **********************************************************************/
