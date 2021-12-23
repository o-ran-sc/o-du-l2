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

uint8_t unpackEgtpSrvOpenReq(EgtpSrvOpenReq func, Pst *pst, Buffer *mBuf);
uint8_t         protType;

typedef struct egtpTptSrvr
{
   CmInetAddr  addr; 
   CmInetFd    sockFd;     /* Socket file descriptor */
}EgtpTptSrvr;

typedef struct EgtpTeIdCb
{
   uint32_t teId;              /* Local tunnel id */
   uint32_t remTeId;           /* Remote tunnel id */
   struct
   {
      uint8_t hdr[EGTP_MAX_HDR_LEN];
      uint8_t cnt;
   }preEncodedHdr;        /* Pre-encoded header for PDUs on this tunnel */
}EgtpTeIdCb;

typedef struct egtpDstCb
{
   CmInetIpAddr  dstIp;          /* destination IP */
   uint16_t      dstPort;        /* destination port that sends data */
   EgtpTptSrvr   sendTptSrvr;    /* Transport server for sending UDP msg to */
   uint32_t      numTunn;        /* Number of tunnels */
   CmHashListCp  teIdLst;        /* Tunnel Id list for this destination */
}EgtpDstCb;

typedef struct egtpGlobalCb
{
   EgtpConfig   egtpCfg;         /* EGTP configuration */
   EgtpTptSrvr  recvTptSrvr;     /* Transport server for receiving UDP msg */
   EgtpDstCb    dstCb;           /* Destination endpoint */
}EgtpGlobalCb;

EgtpGlobalCb egtpCb;   /* EGTP global control block */

uint8_t duHdlEgtpDlData(EgtpMsg  *egtpMsg);
uint8_t egtpActvInit(Ent entity, Inst inst, Region region, Reason reason);
uint8_t egtpActvTsk(Pst *pst, Buffer *mBuf);
uint8_t egtpFillRspPst(Pst *pst, Pst *rspPst);
uint8_t egtpCfgReq(Pst *pst, EgtpConfig egtpCfg);
uint8_t egtpSrvOpenReq(Pst *pst);
uint8_t egtpSrvOpenPrc(uint8_t sockType, EgtpTptSrvr *server);
uint8_t egtpTnlMgmtReq(Pst *pst, EgtpTnlEvt tnlEvt);
uint8_t egtpTnlAdd(EgtpTnlEvt tnlEvt);
uint8_t egtpTnlMod(EgtpTnlEvt tnlEvt);
uint8_t egtpTnlDel(EgtpTnlEvt tnlEvt);
uint8_t egtpHdlDatInd(EgtpMsg msg);
uint8_t egtpEncodeHdr(uint8_t *preEncodedHdr, EgtpMsgHdr *preDefHdr, uint8_t *hdrIdx);
uint8_t egtpSendMsg(Buffer *mBuf);
uint8_t egtpSlotInd();
uint8_t egtpRecvMsg();
uint8_t egtpHdlRecvData(Buffer *mBuf);
uint8_t egtpDecodeHdr(Buffer *mBuf, EgtpMsg  *egtpMsg);

#endif
