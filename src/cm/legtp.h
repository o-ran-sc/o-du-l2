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
 
/* Defines API exchanged between DU_APP and EGTP */
#ifndef __LEGTP_H___
#define __LEGTP_H___

#include "gen.h"

#include "gen.x"

#define EGTP_TNL_MGMT_ADD 1
#define EGTP_TNL_MGMT_MOD 2
#define EGTP_TNL_MGMT_DEL 3

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

typedef struct egtpIpAddr
{
   Bool      ipV4Pres;
   uint32_t  ipV4Addr;
}EgtpIpAddr;

typedef struct egtpConfig
{
   EgtpIpAddr  localIp;
   uint16_t    localPort;
   EgtpIpAddr  destIp;
   uint16_t    destPort;
   uint32_t    minTunnelId;
   uint32_t    maxTunnelId;
}EgtpConfig;

typedef struct egtpTnlEvt
{
   uint8_t   action;
   uint32_t  lclTeid;
   uint32_t  remTeid;
   CmStatus  cfmStatus;
}EgtpTnlEvt;

/*Extension Header */
typedef struct egtpExtHdr
{
   TknUInt16   udpPort;                       /**< UDP Port */
   TknUInt16   pdcpNmb;                       /**< PDCP Number */
}EgtpExtHdr;

/*Structure for MsgHdr */
typedef struct egtpMsgHdr
{
   TknUInt8      nPdu;         /**< N-PDU Number */
   TknUInt32     seqNum;       /**< Sequence Number */
   EgtpExtHdr    extHdr;       /**< Extension headers present flag */
   uint32_t      teId;         /**< Tunnel Endpoint Id */
   uint8_t       msgType;      /**< eGTP-U Message Type */
}EgtpMsgHdr;

typedef struct egtpMsg
{
   EgtpMsgHdr msgHdr;
   Buffer     *msg;
}EgtpMsg;

typedef uint8_t (*EgtpCfgReq) ARGS((Pst *pst, EgtpConfig egtpCfg));
typedef uint8_t (*EgtpCfgCfm) ARGS((CmStatus cfm));
typedef uint8_t (*EgtpInitReq) ARGS((Buffer *mBuf));
typedef uint8_t (*EgtpSrvOpenReq) ARGS(());
typedef uint8_t (*EgtpSrvOpenCfm) ARGS((CmStatus cfm));
typedef uint8_t (*EgtpTnlMgmtReq) ARGS((Pst *pst, EgtpTnlEvt tnlEvt));
typedef uint8_t (*EgtpTnlMgmtCfm) ARGS((EgtpTnlEvt tnlEvt));
typedef uint8_t (*EgtpSlotInd) ARGS(());
  
uint8_t packEgtpCfgReq(Pst *pst, EgtpConfig  egtpCfg);
uint8_t unpackEgtpCfgReq(EgtpCfgReq func , Pst *pst, Buffer *mBuf);
uint8_t packEgtpCfgCfm(Pst *pst, CmStatus cfm);
uint8_t unpackEgtpCfgCfm(EgtpCfgCfm func, Buffer *mBuf);
uint8_t packEgtpSrvOpenReq(Pst *pst);
uint8_t unpackkEgtpSrvOpenReq(EgtpSrvOpenReq func, Pst *pst, Buffer *mBuf);
uint8_t packEgtpSrvOpenCfm(Pst *pst, CmStatus cfm);
uint8_t unpackEgtpSrvOpenCfm(EgtpSrvOpenCfm func, Buffer *mBuf);
uint8_t packEgtpTnlMgmtReq(Pst *pst, EgtpTnlEvt tnlEvt);
uint8_t unpackEgtpTnlMgmtReq(EgtpTnlMgmtReq func, Pst *pst, Buffer *mBuf);
uint8_t egtpTnlMgmtReq(Pst *pst, EgtpTnlEvt tnlEvt);
uint8_t packEgtpTnlMgmtCfm(Pst *pst, EgtpTnlEvt tnlEvt);
uint8_t duHdlEgtpTnlMgmtCfm(EgtpTnlEvt tnlEvtCfm);
uint8_t unpackEgtpTnlMgmtCfm(EgtpTnlMgmtCfm func, Buffer *mBuf);
uint8_t packEgtpStartPollingReq(Pst *pst);
#endif
