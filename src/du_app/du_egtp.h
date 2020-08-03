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
 
#ifndef __DU_EGTP_H__
#define __DU_EGTP_H__

#define EGTP_DFLT_PORT 2152
#define EGTP_MAX_HDR_LEN 40
#define EGTP_MAX_MSG_RECV 10

#define EGTP_EXT_HDR_UDP_TYPE 64
#define EGTP_EXT_HDR_PDCP_TYPE 192

#define EGTP_MASK_BIT1                   0x01
#define EGTP_MASK_BIT2                   0x02
#define EGTP_MASK_BIT3                   0x04
#define EGTP_MASK_BIT4                   0x08
#define EGTP_MASK_BIT5                   0x10
#define EGTP_MASK_BIT6                   0x20
#define EGTP_MASK_BIT7                   0x40
#define EGTP_MASK_BIT8                   0x80

S16 unpackEgtpSrvOpenReq(EgtpSrvOpenReq func, Pst *pst, Buffer *mBuf);
U8         protType;

typedef struct egtpTptSrvr
{
   CmInetAddr  addr; 
   CmInetFd    sockFd;     /* Socket file descriptor */
}EgtpTptSrvr;

typedef struct EgtpTeIdCb
{
   U32 teId;              /* Local tunnel id */
   U32 remTeId;           /* Remote tunnel id */
   struct
   {
      U8 hdr[EGTP_MAX_HDR_LEN];
      U8 cnt;
   }preEncodedHdr;        /* Pre-encoded header for PDUs on this tunnel */
}EgtpTeIdCb;

typedef struct egtpDstCb
{
   CmInetIpAddr  dstIp;          /* destination IP */
   U16           dstPort;        /* destination port that sends data */
   EgtpTptSrvr   sendTptSrvr;    /* Transport server for sending UDP msg to */
   U32           numTunn;        /* Number of tunnels */
   CmHashListCp  teIdLst;        /* Tunnel Id list for this destination */
}EgtpDstCb;

typedef struct egtpGlobalCb
{
   EgtpConfig   egtpCfg;         /* EGTP configuration */
   EgtpTptSrvr  recvTptSrvr;     /* Transport server for receiving UDP msg */
   EgtpDstCb    dstCb;           /* Destination endpoint */
}EgtpGlobalCb;
extern EgtpGlobalCb egtpCb;   /* EGTP global control block */

uint8_t duHdlEgtpDlData(EgtpMsg  *egtpMsg);
S16 egtpActvInit(Ent entity, Inst inst, Region region, Reason reason);
S16 egtpActvTsk(Pst *pst, Buffer *mBuf);
S16 egtpFillRspPst(Pst *pst, Pst *rspPst);
S16 egtpCfgReq(Pst *pst, EgtpConfig egtpCfg);
S16 egtpSrvOpenReq(Pst *pst);
S16 egtpSrvOpenPrc(U8 sockType, EgtpTptSrvr *server);
S16 egtpTnlMgmtReq(Pst *pst, EgtpTnlEvt tnlEvt);
S16 egtpTnlAdd(EgtpTnlEvt tnlEvt);
S16 egtpTnlMod(EgtpTnlEvt tnlEvt);
S16 egtpTnlDel(EgtpTnlEvt tnlEvt);
S16 egtpHdlDatInd(EgtpMsg msg);
S16 egtpEncodeHdr(U8 *preEncodedHdr, EgtpMsgHdr *preDefHdr, U8 *hdrIdx);
S16 egtpSendMsg(Buffer *mBuf);
S16 egtpSlotInd();
S16 egtpRecvMsg();
S16 egtpHdlRecvData(Buffer *mBuf);
S16 egtpDecodeHdr(Buffer *mBuf, EgtpMsg  *egtpMsg);

#endif
