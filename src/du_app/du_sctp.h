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

#ifndef __DU_SCTP_H__
#define __DU_SCTP_H__

#include <netinet/in.h>
#include <netinet/sctp.h>
#include "du_mgr.h"

S16 sctpActvInit(Ent entity, Inst inst, Region region, Reason reason);
S16 sctpActvTsk(Pst *pst, Buffer *mBuf);

#define REALLY_BIG 65536

/* Convenience structure to determine space needed for cmsg. */
typedef union 
{
   struct sctp_initmsg init;
   struct sctp_sndrcvinfo sndrcvinfo;
}_sctp_cmsg_data_t;

#define CMSG_SPACE_INITMSG (CMSG_SPACE(sizeof(struct sctp_initmsg)))
#define CMSG_SPACE_SNDRCV (CMSG_SPACE(sizeof(struct sctp_sndrcvinfo)))

typedef union 
{
   struct sockaddr_storage ss;
   struct sockaddr_in v4;
   struct sockaddr_in6 v6;
   struct sockaddr sa;
} sockaddr_storage_t;

#endif

/**********************************************************************
         End of file
**********************************************************************/
