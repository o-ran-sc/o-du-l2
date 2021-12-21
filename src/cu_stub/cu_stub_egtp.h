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
 
#ifndef __CU_STUB_EGTP_H__
#define __CU_STUB_EGTP_H__

#include "cu_stub.h"
#include "cm_inet.h"
#include "cm_tpt.h"
 
#include "cm_inet.x"
#include "cm_tpt.x"

#define EGTP_DFLT_PORT 2152
#define EGTP_TNL_MGMT_ADD 1
#define EGTP_TNL_MGMT_MOD 2
#define EGTP_TNL_MGMT_DEL 3

#define EGTP_MAX_HDR_LEN 40
 
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

#define NUM_TUNNEL_TO_PUMP_DATA 9
#define NUM_DL_PACKETS 1

uint8_t         sockType;
uint8_t         protType;

typedef enum egtpMsgType
{
   EGTP_MSG_INVALID,
   EGTPU_MSG_ECHO_REQ,
   EGTPU_MSG_ECHO_RSP,
   EGTPU_MSG_ERROR_IND          = 26,
   EGTPU_MSG_SUPP_EXT_HDR_NTF   = 31,
   EGTPU_MSG_END_MARKER         = 254,
   EGTPU_MSG_GPDU,
   EGTP_MSG_MAX
}EgtpMsgType;

/*Extension Header */
typedef struct egtpExtHdr
{
   TknUInt16   udpPort;                       /**< UDP Port */
   TknUInt16   pdcpNmb;                       /**< PDCP Number */
}EgtpExtHdr;
 
/*Structure for MsgHdr */
typedef struct egtpMsgHdr
{
   TknUInt8   nPdu;                      /**< N-PDU Number */
   TknUInt32  seqNum;                    /**< Sequence Number */
   EgtpExtHdr extHdr;                       /**< Extension headers present flag */
   uint32_t   teId;                         /**< Tunnel Endpoint Id */
   uint8_t    msgType;                      /**< eGTP-U Message Type */
}EgtpMsgHdr;
 
typedef struct egtpMsg
{
   EgtpMsgHdr msgHdr;
   Buffer     *msg;
}EgtpMsg;

typedef struct egtpTnlEvt
{
   uint8_t  action;
   uint32_t lclTeid;
   uint32_t remTeid;
}EgtpTnlEvt;

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
   uint16_t      dstPort;        /* Remote port that sends data */
   EgtpTptSrvr   sendTptSrvr;    /* Transport server for sending UDP msg to */
   uint32_t      numTunn;        /* Number of tunnels */
   CmHashListCp  teIdLst;        /* Tunnel Id list for this destination */
}EgtpDstCb;

typedef struct egtpGlobalCb
{
   EgtpParams   egtpCfg;         /* EGTP configuration */
   EgtpTptSrvr  recvTptSrvr;     /* Transport server for receiving UDP msg */
   EgtpDstCb    dstCb;           /* Destination endpoint */
   uint8_t      gCntPdu[MAX_TEID+1]; /* Maintaining PDU count for each bearer */
}EgtpGlobalCb;
EgtpGlobalCb egtpCb;   /* EGTP global control block */

S16 egtpActvInit();
S16 cuEgtpCfgReq();
S16 cuEgtpSrvOpenReq();
S16 cuEgtpTnlMgmtReq(EgtpTnlEvt tnlEvt);
S16 cuEgtpTnlAdd(EgtpTnlEvt tnlEvt);
S16 cuEgtpTnlMod(EgtpTnlEvt tnlEvt);
S16 cuEgtpTnlDel(EgtpTnlEvt tnlEvt);
S16 cuEgtpEncodeHdr(uint8_t *preEncodedHdr, EgtpMsgHdr *preDefHdr, uint8_t *hdrIdx);
S16 cuEgtpHdlRecvMsg(Buffer *mBuf);
uint16_t cuEgtpDatReq(uint8_t teId);
S16 BuildAppMsg(EgtpMsg  *egtpMsg);
S16 BuildEgtpMsg(EgtpMsg *egtpMsg);
S16 cuEgtpSendMsg(Buffer *mBuf);
S16 cuEgtpDecodeHdr(Buffer *mBuf);

#endif
