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
   Bool ipV4Pres;
   U32  ipV4Addr;
}EgtpIpAddr;

typedef struct egtpConfig
{
   EgtpIpAddr  localIp;
   U16         localPort;
   EgtpIpAddr  destIp;
   U16         destPort;
   U32       minTunnelId;
   U32       maxTunnelId;
}EgtpConfig;

typedef struct egtpTnlEvt
{
   U8        action;
   U32       lclTeid;
   U32       remTeid;
   CmStatus  cfmStatus;
}EgtpTnlEvt;

/*Extension Header */
typedef struct egtpExtHdr
{
   TknU16   udpPort;                       /**< UDP Port */
   TknU16   pdcpNmb;                       /**< PDCP Number */
}EgtpExtHdr;

/*Structure for MsgHdr */
typedef struct egtpMsgHdr
{
   TknU8      nPdu;                      /**< N-PDU Number */
   TknU32     seqNum;                    /**< Sequence Number */
   EgtpExtHdr extHdr;                       /**< Extension headers present flag */
   U32        teId;                         /**< Tunnel Endpoint Id */
   U8         msgType;                      /**< eGTP-U Message Type */
}EgtpMsgHdr;

typedef struct egtpMsg
{
   EgtpMsgHdr msgHdr;
   Buffer     *msg;
}EgtpMsg;

typedef S16 (*EgtpCfgReq) ARGS((Pst *pst, EgtpConfig egtpCfg));
typedef S16 (*EgtpCfgCfm) ARGS((CmStatus cfm));
typedef S16 (*EgtpInitReq) ARGS((Buffer *mBuf));
typedef S16 (*EgtpSrvOpenReq) ARGS(());
typedef S16 (*EgtpSrvOpenCfm) ARGS((CmStatus cfm));
typedef S16 (*EgtpTnlMgmtReq) ARGS((Pst *pst, EgtpTnlEvt tnlEvt));
typedef S16 (*EgtpTnlMgmtCfm) ARGS((EgtpTnlEvt tnlEvt));
typedef S16 (*EgtpSlotInd) ARGS(());
  
S16 packEgtpCfgReq(Pst *pst, EgtpConfig  egtpCfg);
S16 unpackEgtpCfgReq(EgtpCfgReq func , Pst *pst, Buffer *mBuf);
S16 packEgtpCfgCfm(Pst *pst, CmStatus cfm);
S16 unpackEgtpCfgCfm(EgtpCfgCfm func, Buffer *mBuf);
S16 packEgtpSrvOpenReq(Pst *pst);
S16 unpackkEgtpSrvOpenReq(EgtpSrvOpenReq func, Pst *pst, Buffer *mBuf);
S16 packEgtpSrvOpenCfm(Pst *pst, CmStatus cfm);
S16 unpackEgtpSrvOpenCfm(EgtpSrvOpenCfm func, Buffer *mBuf);
S16 packEgtpTnlMgmtReq(Pst *pst, EgtpTnlEvt tnlEvt);
S16 unpackEgtpTnlMgmtReq(EgtpTnlMgmtReq func, Pst *pst, Buffer *mBuf);
S16 packEgtpTnlMgmtCfm(Pst *pst, EgtpTnlEvt tnlEvt);
S16 unpackEgtpTnlMgmtCfm(EgtpTnlMgmtCfm func, Buffer *mBuf);
S16 packEgtpSlotInd(Pst *pst);
S16 unpackEgtpSlotInd(EgtpSlotInd func, Pst *pst, Buffer *mBuf);

#endif
