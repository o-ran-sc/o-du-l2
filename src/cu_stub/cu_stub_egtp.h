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

/*VALID Tunnel ID*/
#define MIN_TEID 1   /*[Spec 29.281,Sec 5.1]: All Zero TEIDs are never assigned for setting up GTP-U Tunnel*/
#define MAX_TEID MAX_NUM_DRB * MAX_NUM_UE /*[Spec 29.281]: Max limit is not mentioned but as per GTP-U Header Format, TEID occupies 4 octets */

#define NUM_TUNNEL_TO_PUMP_DATA 4
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
   uint32_t      duId;
   CmInetAddr    dstAddr;
   uint32_t      numTunn;        /* Number of tunnels */
   CmHashListCp  teIdLst;        /* Tunnel Id list for this destination */
}EgtpDstCb;

typedef struct egtpDstCfg 
{
   SctpIpAddr  dstIp;
   uint16_t    dstPort;
}EgtpDstCfg;

typedef struct cuEgtpParams
{
   uint32_t       currTunnelId;
   uint32_t       minTunnelId;
   uint32_t       maxTunnelId;
   uint8_t        numDu;
   SctpIpAddr     localIp;
   uint16_t       localPort;
   EgtpDstCfg     dstCfg[MAX_DU_SUPPORTED];
}CuEgtpParams;

typedef struct egtpGlobalCb
{
   CuEgtpParams egtpCfg;         /* EGTP configuration */
   CmInetAddr   localAddr;
   CmInetFd     sockFd;     /* Socket file descriptor */
   uint8_t      numDu;
   EgtpDstCb    dstCb[MAX_DU_SUPPORTED];          /* Destination endpoint */
   uint8_t      gCntPdu[MAX_TEID+1]; /* Maintaining PDU count for each bearer */
}EgtpGlobalCb;
EgtpGlobalCb egtpCb;   /* EGTP global control block */

S16 egtpActvInit();
S16 cuEgtpCfgReq();
S16 cuEgtpSrvOpenReq();
S16 cuEgtpTnlMgmtReq(uint32_t duId,EgtpTnlEvt tnlEvt);
S16 cuEgtpTnlAdd(uint32_t duId, EgtpTnlEvt tnlEvt);
S16 cuEgtpTnlMod(uint32_t duId,EgtpTnlEvt tnlEvt);
S16 cuEgtpTnlDel(uint32_t duId,EgtpTnlEvt tnlEvt);
S16 cuEgtpEncodeHdr(uint8_t *preEncodedHdr, EgtpMsgHdr *preDefHdr, uint8_t *hdrIdx);
S16 cuEgtpHdlRecvMsg(Buffer *mBuf);
uint16_t cuEgtpDatReq(uint32_t duId, uint8_t teId);
S16 BuildAppMsg(uint32_t duId, EgtpMsg  *egtpMsg);
S16 BuildEgtpMsg(uint32_t duId, EgtpMsg *egtpMsg);
S16 cuEgtpSendMsg(uint32_t duId, Buffer *mBuf);
S16 cuEgtpDecodeHdr(Buffer *mBuf);

#endif
