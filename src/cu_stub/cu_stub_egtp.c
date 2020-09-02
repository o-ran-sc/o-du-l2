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
 
/* This file contains all EGTP related functionality */

#include "common_def.h"
#include "cu_stub_egtp.h"
#include "du_log.h"

/* Global variable declaration */
EgtpGlobalCb egtpCb;

/**************************************************************************
 * @brief Task Initiation callback function. 
 *
 * @details
 *
 *     Function : egtpActvInit 
 *    
 *     Functionality:
 *             This function is supplied as one of parameters during EGTP's 
 *             task registration. SSI will invoke this function once, after
 *             it creates and attaches this TAPA Task to a system task.
 *     
 * @param[in]  Ent entity, the entity ID of this task.     
 * @param[in]  Inst inst, the instance ID of this task.
 * @param[in]  Region region, the region ID registered for memory 
 *              usage of this task.
 * @param[in]  Reason reason.
 * @return ROK     - success
 *         RFAILED - failure
 ***************************************************************************/
S16 egtpActvInit()
{
  DU_LOG("\n\nEGTP : Initializing");
  cmMemset ((U8 *)&egtpCb, 0, sizeof(EgtpGlobalCb));
  protType = CM_INET_PROTO_UDP;
  return ROK;
}


/**************************************************************************
 * @brief Task Activation callback function. 
 *
 * @details
 *
 *      Function : egtpActvTsk 
 * 
 *      Functionality:
 *           This function handles all EGTP messages received
 *           This API is registered with SSI during the 
 *           Task Registration of DU APP.
 *     
 * @param[in]  Pst     *pst, Post structure of the primitive.     
 * @param[in]  Buffer *mBuf, Packed primitive parameters in the
 *  buffer.
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
S16 egtpInitReq()
{
   S16 ret = ROK;
   EgtpTnlEvt tnlEvt;

   ret = cuEgtpCfgReq();
   if(ret != ROK)
   {
      DU_LOG("\nEGTP : Configuration failed");
      RETVALUE(ret);
   }

   ret = cuEgtpSrvOpenReq();
   if(ret != ROK)
   {
       DU_LOG("\nEGTP : Transport server open request failed");
       RETVALUE(ret);
   }

   tnlEvt.action = EGTP_TNL_MGMT_ADD;
   tnlEvt.lclTeid = 10;
   tnlEvt.remTeid = 1;
   ret = cuEgtpTnlMgmtReq(tnlEvt);
   if(ret != ROK)
   {
      DU_LOG("\n EGTP : Tunnel management request failed");
      return RFAILED;
   }

   RETVALUE(ret);

} /* egtpInitReq */

/**************************************************************************
 * @brief EGTP server configuration 
 *
 * @details
 *
 *      Function : egtpCfgReq
 * 
 *      Functionality:
 *           This function handles EGTP configuration request.
 *     
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ***********************************************************************/
S16 cuEgtpCfgReq()
{
   U8 ret;

   cmMemcpy((U8 *)&egtpCb.egtpCfg, (U8 *)&cuCfgParams.egtpParams, (PTR)sizeof(EgtpParams));

   egtpCb.recvTptSrvr.addr.address = CM_INET_NTOH_U32(egtpCb.egtpCfg.localIp.ipV4Addr);
   egtpCb.recvTptSrvr.addr.port = EGTP_DFLT_PORT;

   egtpCb.dstCb.dstIp = CM_INET_NTOH_U32(egtpCb.egtpCfg.destIp.ipV4Addr);
   egtpCb.dstCb.dstPort = egtpCb.egtpCfg.destPort;
   egtpCb.dstCb.sendTptSrvr.addr.address = CM_INET_NTOH_U32(egtpCb.egtpCfg.localIp.ipV4Addr);
   egtpCb.dstCb.sendTptSrvr.addr.port = egtpCb.egtpCfg.localPort;
   egtpCb.dstCb.numTunn = 0;

   ret = cmHashListInit(&(egtpCb.dstCb.teIdLst), 1024, sizeof(EgtpTeIdCb), FALSE, CM_HASH_KEYTYPE_U32MOD, CU_APP_MEM_REG, CU_POOL);

   if(ret != ROK)
   {
      DU_LOG("\nEGTP : TeId hash list initialization failed");
      return RFAILED;
   }
   else
   {
      DU_LOG("\nEGTP : Configuration successful");
   }

   return ROK;
} /* cuEgtpCfgReq */

/**************************************************************************
 * @brief EGTP server open request 
 *
 * @details
 *
 *      Function : egtpSrvOpenReq
 * 
 *      Functionality:
 *           This function handles EGTP open server request.
 *           It opens udp socket to receive/send msgs.
 *     
 * @param[in]  Pst *pst, post structure
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/

S16 cuEgtpSrvOpenReq(Pst *pst)
{

   U8 ret;

   DU_LOG("\nEGTP : Received open server request");
 
   sockType = CM_INET_DGRAM;
   if(ret = (cmInetSocket(sockType, &(egtpCb.recvTptSrvr.sockFd), protType)) != ROK)
   {
      DU_LOG("\nEGTP : Failed to open UDP socket");
      return RFAILED;
   }

   ret = cmInetBind(&(egtpCb.recvTptSrvr.sockFd), &(egtpCb.recvTptSrvr.addr));
   if(ret != ROK)
   {
      DU_LOG("\nEGTP : Failed to bind socket");
      return RFAILED;
   }

   if(ret = (cmInetSocket(sockType, &(egtpCb.dstCb.sendTptSrvr.sockFd), protType)) != ROK)
   {  
      DU_LOG("\nEGTP : Failed to open UDP socket");
      return RFAILED;
   }
      
   ret = cmInetBind(&(egtpCb.dstCb.sendTptSrvr.sockFd), &(egtpCb.dstCb.sendTptSrvr.addr));
   if(ret != ROK)
   {  
      DU_LOG("\nEGTP : Failed to bind socket");
      return RFAILED;
   }
       
   /* TODO: set socket options */

   DU_LOG("\nEGTP : Receiver socket[%d] and Sender socket[%d] open", egtpCb.recvTptSrvr.sockFd.fd, egtpCb.dstCb.sendTptSrvr.sockFd.fd);
   return ROK;
} /* cuEgtpSrvOpenReq */


/**************************************************************************
 * @brief EGTP tunnel management request
 *
 * @details
 *
 *      Function : cuEgtpTnlMgmtReq
 * 
 *      Functionality:
 *           This function handles EGTP tunnel managament request
 *     
 * @param[in]  Pst *pst, post structure
 *             Tunnel Eveny structure
 * @return ROK     - success
 *         RFAILED - failure
 *
 
 * ***************************************************************************/
S16 cuEgtpTnlMgmtReq(EgtpTnlEvt tnlEvt)
{
   S8 ret;

   DU_LOG("\nEGTP : Received tunnel management request");
   switch(tnlEvt.action)
   {
      case EGTP_TNL_MGMT_ADD:
      {
         ret = cuEgtpTnlAdd(tnlEvt);
         break;
      }
      case EGTP_TNL_MGMT_MOD:
      {
         ret = cuEgtpTnlMod(tnlEvt);
         break;
      }
      case EGTP_TNL_MGMT_DEL:
      {
         ret = cuEgtpTnlDel(tnlEvt);
         break;
      }
      default:
      {
         DU_LOG("\nEGTP : Invalid tunnel management action[%d]", tnlEvt.action);
         ret = RFAILED;
      }
   }

   RETVALUE(ret);
}

/**************************************************************************
 * @brief EGTP tunnel addition
 *
 * @details
 *
 *      Function : cuEgtpTnlAdd
 * 
 *      Functionality:
 *           This function handles EGTP tunnel addition
 *     
 * @param[in]  Tunnel Event structure
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ***************************************************************************/
S16 cuEgtpTnlAdd(EgtpTnlEvt tnlEvt)
{
   S16   ret;
   EgtpTeIdCb     *teidCb;
   EgtpMsgHdr   preDefHdr; /* pre-define header for this tunnel */
   

   DU_LOG("\nEGTP : Tunnel addition : LocalTeid[%d] Remote Teid[%d]", tnlEvt.lclTeid, tnlEvt.remTeid);

   ret = SGetSBuf(CU_APP_MEM_REG, CU_POOL, (Data **)&teidCb, (Size)sizeof(EgtpTeIdCb));
   if(ret != ROK)
   {
      DU_LOG("\nEGTP : Memory allocation failed");
      return RFAILED;
   }


   cmMemset((U8 *)teidCb, 0, sizeof(EgtpTeIdCb));
   teidCb->teId = tnlEvt.lclTeid;
   teidCb->remTeId = tnlEvt.remTeid;

   ret = cmHashListInsert(&(egtpCb.dstCb.teIdLst), (PTR)teidCb, (U8 *)&(teidCb->teId), sizeof(U32));
   if(ret != ROK)
   {
      DU_LOG("\nEGTP : Failed to insert in hash list");
      SPutSBuf(CU_APP_MEM_REG, CU_POOL, (Data *)teidCb, (Size)sizeof(EgtpTeIdCb));
      return RFAILED;
   }
   egtpCb.dstCb.numTunn++;

   /* Encoding pre-defined header */
   cmMemset((U8*)&preDefHdr, 0, sizeof(EgtpMsgHdr));
   preDefHdr.msgType = EGTPU_MSG_GPDU;
   preDefHdr.teId = teidCb->remTeId;
   preDefHdr.extHdr.pdcpNmb.pres = FALSE;
   preDefHdr.extHdr.udpPort.pres = FALSE;
   preDefHdr.nPdu.pres = FALSE;
   
   cuEgtpEncodeHdr((U8 *)teidCb->preEncodedHdr.hdr, &preDefHdr, &(teidCb->preEncodedHdr.cnt));

/*   SPutSBuf(CU_APP_MEM_REG, CU_POOL, (Data *)teidCb, (Size)sizeof(EgtpTeIdCb));*/

   return ROK;
} /* cuEgtpTnlAdd */

/**************************************************************************
 * @brief EGTP tunnel modification
 *
 * @details
 *
 *      Function : cuEgtpTnlMod
 * 
 *      Functionality:
 *           This function handles EGTP tunnel modification
 *     
 * @param[in]  Tunnel Event structure
 * @return ROK     - success
 *         RFAILED - failure
 * 
 * ***************************************************************************/
S16 cuEgtpTnlMod(EgtpTnlEvt tnlEvt)
{
#if 0
   S16   ret;
   EgtpTeIdCb     *teidCb = NULLP;

   printf("\nTunnel modification : LocalTeid[%d] Remote Teid[%d]", tnlEvt.lclTeid, tnlEvt.remTeid);

   cmHashListFind(&(egtpCb.dstCb.teIdLst), (U8 *)&(tnlEvt.teId), sizeof(U32), 0, (PTR *)&teidCb);
   if(teidCb == NULLP)
   {
      printf("\nTunnel id not found");
      return RFAILED;
   }  
   
   teidCb->teId = tnlEvt.lclTeid;
   teidCb->remTeId = tnlEvt.remTeid;
#endif
   return ROK;
}

/**************************************************************************
 * @brief EGTP tunnel deletion
 *
 * @details
 *
 *     Function : cuEgtpTnlDel
 * 
 *     Functionality:
 *         This function handles EGTP tunnel deletion
 *    
 * @param[in]  Tunnel Event structure
 * @return ROK     - success
 *         RFAILED - failure
 * 
 * ***************************************************************************/
S16 cuEgtpTnlDel(EgtpTnlEvt tnlEvt)
{
   EgtpTeIdCb     *teidCb = NULLP;

   DU_LOG("\nEGTP : Tunnel deletion : Local Teid[%d] Remote Teid[%d]", tnlEvt.lclTeid, tnlEvt.remTeid);
   
   cmHashListFind(&(egtpCb.dstCb.teIdLst), (U8 *)&(tnlEvt.lclTeid), sizeof(U32), 0, (PTR *)&teidCb);
   if(teidCb == NULLP)
   {
      DU_LOG("\nEGTP : Tunnel id[%d] not configured", tnlEvt.lclTeid);
      return RFAILED;
   } 

   cmHashListDelete(&(egtpCb.dstCb.teIdLst), (PTR)teidCb);
   SPutSBuf(CU_APP_MEM_REG, CU_POOL, (Data *)teidCb, (Size)sizeof(EgtpTeIdCb));
   egtpCb.dstCb.numTunn--;

   return ROK;
} /* cuEgtpTnlDel */

/*******************************************************************
 *
 * @brief Encodes message header
 *
 * @details
 *
 *    Function : cuEgtpEncodeHdr
 * 
 *    Functionality:
 *       Encodes EGTP message haeder
 *
 * @params[in] EGTP message
 *             Message Buffer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 cuEgtpEncodeHdr(U8 *preEncodedHdr, EgtpMsgHdr *preDefHdr, U8 *hdrIdx)
{
   U8         tmpByte = 0;                 /* Stores one byte of data for enc */
   U8         cnt     = EGTP_MAX_HDR_LEN;  /* Stores the position */
   Bool       extPres = FALSE;             /* Flag for indication of S, E or P presense flag */
   U16        nwWord = 0;

   /* Encoding header */
   tmpByte |= EGTP_MASK_BIT6;   /* Setting 6th LSB of 1st byte as version */
   tmpByte |= EGTP_MASK_BIT5;   /* Setting 5th LSB of 1st byte as protocol type */
   
   if(preDefHdr->extHdr.udpPort.pres || preDefHdr->extHdr.pdcpNmb.pres)
   {
      tmpByte |= EGTP_MASK_BIT3;  /* Setting 3rd LSB of 1st byte if Extension heaeder is present */
   }
    
   if(preDefHdr->seqNum.pres)
   {
      tmpByte |= EGTP_MASK_BIT2;
   }
   
   if(preDefHdr->nPdu.pres)
   {
      tmpByte |= EGTP_MASK_BIT1;
   }
   
   if((tmpByte & EGTP_MASK_BIT1) || (tmpByte & EGTP_MASK_BIT2)||(tmpByte & EGTP_MASK_BIT3))
   {
      extPres = TRUE;
   }
   preEncodedHdr[--cnt] = tmpByte;
   preEncodedHdr[--cnt] = preDefHdr->msgType;
    
   /* Encode Tunnel endpoint */
   preEncodedHdr[--cnt] = 0;
   preEncodedHdr[--cnt] = 0;
   nwWord = (U16)(GetHiWord(preDefHdr->teId));
   preEncodedHdr[--cnt] = (U8)(GetHiByte(nwWord));
   preEncodedHdr[--cnt] = (U8)(GetLoByte(nwWord));
   nwWord = (U16)(GetLoWord(preDefHdr->teId));
   preEncodedHdr[--cnt] = (U8)(GetHiByte(nwWord));
   preEncodedHdr[--cnt] = (U8)(GetLoByte(nwWord));
    
   /* Encode sequence number */
   if(preDefHdr->seqNum.pres)
   {
      preEncodedHdr[--cnt] = GetHiByte(preDefHdr->seqNum.val);
      preEncodedHdr[--cnt] = GetLoByte(preDefHdr->seqNum.val);
   }
   else if(extPres)
   {
      preEncodedHdr[--cnt] = 0;
      preEncodedHdr[--cnt] = 0;
   }

   if(preDefHdr->nPdu.pres)
   {
      preEncodedHdr[--cnt] = preDefHdr->nPdu.val;
   }
   else if(extPres)
   {
      preEncodedHdr[--cnt] = 0;
   }
    
   if(preDefHdr->extHdr.udpPort.pres)
   {
      preEncodedHdr[--cnt] = EGTP_EXT_HDR_UDP_TYPE;
      preEncodedHdr[--cnt] = 1;
      preEncodedHdr[--cnt] = GetHiByte(preDefHdr->extHdr.udpPort.val);
      preEncodedHdr[--cnt] = GetLoByte(preDefHdr->extHdr.udpPort.val);
   }
   
   if(preDefHdr->extHdr.pdcpNmb.pres)
   {
      preEncodedHdr[--cnt] = EGTP_EXT_HDR_PDCP_TYPE;
      preEncodedHdr[--cnt] = 1;
      preEncodedHdr[--cnt] = GetHiByte(preDefHdr->extHdr.pdcpNmb.val);
      preEncodedHdr[--cnt] = GetLoByte(preDefHdr->extHdr.pdcpNmb.val);
   }
    
   if(tmpByte & EGTP_MASK_BIT3)
   {
      preEncodedHdr[--cnt] = 0;
   }
   else if(extPres)
   {
      preEncodedHdr[--cnt] = 0;
   } 
   
   *hdrIdx = cnt;
   return ROK;

} /* egtpEncodeHdr */

S16 cuEgtpHdlRecvMsg(Buffer *mBuf)
{
   /*Decoding of EGTP message header */
   cuEgtpDecodeHdr(mBuf);

   /* Start Pumping data from CU to DU */
   RETVALUE(cuEgtpDatReq());

}

S16 cuEgtpDecodeHdr(Buffer *mBuf)
{
   EgtpMsg  egtpMsg;
   S16      retVal  = ROK;       /* Holds the return value */
   U8       tmpByte[5];         /* Holds one byte of data after Dec */
   U8       version = 0;         /* Holds the version type, decoded */
   MsgLen   msgLen  = 0;         /* Holds the msgLen from the Hdr */
   MsgLen   bufLen  = 0;         /* Holds the total buffer length */
   U8       extHdrType = 0;       /* Holds the Extension hdr type */
   U8       extHdrLen = 0;        /* Extension hdr length */
   Bool     extPres = FALSE;      /* Flag for indication of S, E or P presense flag */

   SFndLenMsg(mBuf, &bufLen);

   /* Decode version */
   SRemPreMsg(&tmpByte[0], mBuf);
   version = tmpByte[0] >> 5;

   /* Decode message type */
   SRemPreMsg((Data*)&(egtpMsg.msgHdr.msgType), mBuf);

   /* Decode message length */
   SRemPreMsg(&tmpByte[1], mBuf);
   SRemPreMsg(&tmpByte[2], mBuf);
   msgLen = (tmpByte[1] << 8) | tmpByte[2];

   /* Decode tunnel id */
   SRemPreMsg(&tmpByte[1], mBuf);
   SRemPreMsg(&tmpByte[2], mBuf);
   SRemPreMsg(&tmpByte[3], mBuf);
   SRemPreMsg(&tmpByte[4], mBuf);
   egtpMsg.msgHdr.teId = (tmpByte[1] << 24) | (tmpByte[2] << 16) | (tmpByte[3] << 8) | tmpByte[4];

   if((tmpByte[0] & EGTP_MASK_BIT1) || (tmpByte[0] & EGTP_MASK_BIT2)||(tmpByte[0] & EGTP_MASK_BIT3))
   {
      extPres = TRUE;
   }

   /* Decode sequence number */
   if ( tmpByte[0] & EGTP_MASK_BIT2 )
   {
      egtpMsg.msgHdr.seqNum.pres = TRUE;
      SRemPreMsg(&tmpByte[1], mBuf);
      SRemPreMsg(&tmpByte[2], mBuf);
      egtpMsg.msgHdr.seqNum.val = (tmpByte[1] << 8) | tmpByte[2];
   }
   else  if(extPres)
   {
      egtpMsg.msgHdr.seqNum.pres = 0;
      SRemPreMsg(&tmpByte[1], mBuf);
      SRemPreMsg(&tmpByte[2], mBuf);
      egtpMsg.msgHdr.seqNum.val = (tmpByte[1] << 8) | tmpByte[2];
   }

   /* Decode N-PDU number */
   if ( tmpByte[0] & EGTP_MASK_BIT1 )
   {
      egtpMsg.msgHdr.nPdu.pres = TRUE;
      SRemPreMsg(&(egtpMsg.msgHdr.nPdu.val), mBuf);
   }
   else if(extPres)
   {
      egtpMsg.msgHdr.nPdu.pres = TRUE;
      SRemPreMsg(&(egtpMsg.msgHdr.nPdu.val), mBuf);
   }

   if(extPres & EGTP_MASK_BIT1)
   {
      SRemPreMsg(&extHdrType, mBuf);
      while( 0 != extHdrType)
      {
         switch (extHdrType)
         {
            case EGTP_EXT_HDR_UDP_TYPE:
            {
               SRemPreMsg(&extHdrLen, mBuf);
               if(extHdrLen == 0x01)
               {
                  egtpMsg.msgHdr.extHdr.udpPort.pres = TRUE;
                  SRemPreMsg(&tmpByte[1], mBuf);
                  SRemPreMsg(&tmpByte[2], mBuf);
                  egtpMsg.msgHdr.extHdr.udpPort.val = (tmpByte[1] << 8) | tmpByte[2];
               }
               break;
            }

            case EGTP_EXT_HDR_PDCP_TYPE:
            {
               SRemPreMsg(&extHdrLen, mBuf);
               if(extHdrLen == 0x01)
               {
                  egtpMsg.msgHdr.extHdr.pdcpNmb.pres = TRUE;
                  SRemPreMsg(&tmpByte[1], mBuf);
                  SRemPreMsg(&tmpByte[2], mBuf);
                  egtpMsg.msgHdr.extHdr.pdcpNmb.val = (tmpByte[1] << 8) | tmpByte[2];
               }
               break;
            }
         } /* End of switch */

         SRemPreMsg(&extHdrType, mBuf);

      } /* End of while */
   } /* End of if(extPres & EGTP_MASK_BIT1) */
   else if(extPres)
   {
      SRemPreMsg(&extHdrType, mBuf);
   }

   DU_LOG("\nEGTP : Message Buffer after decoding header ");
   SPrntMsg(mBuf, 0, 0);

   return ROK;
     
} /* End of cuEgtpDecodeHdr */

S16 cuEgtpDatReq()
{
   U8 cnt = 0;
   EgtpMsg  egtpMsg;

   /* Build Application message that is supposed to come from app to egtp */
   BuildAppMsg(&egtpMsg);

   /* Encode EGTP header to build final EGTP message */
   BuildEgtpMsg(&egtpMsg);

   /* Send Message to peer */
   while(cnt < 1)
   {
      DU_LOG("\nEGTP : Sending message[%d]", cnt+1);
      cuEgtpSendMsg(egtpMsg.msg);
      cnt++;
   }

   SPutMsg(egtpMsg.msg);

   return ROK;
}


S16 BuildAppMsg(EgtpMsg  *egtpMsg)
{
   char data[30] = "This is EGTP data from CU";
   int datSize = 30;
 
   Buffer   *mBuf;
 
   if(SGetMsg(CU_APP_MEM_REG, CU_POOL, &mBuf) == ROK)
   {
      if(SAddPstMsgMult((Data *)data, datSize, mBuf) != ROK)
      {
         DU_LOG("\nEGTP : SAddPstMsgMult failed");
         SPutMsg(mBuf);
         return RFAILED;
      }
   }
   else
   {
       DU_LOG("\nEGTP : Failed to allocate memory");
       return RFAILED;
   }
 
   /* filling IPv4 header */
   CmIpv4Hdr ipv4Hdr;
   MsgLen    mLen;
 
   mLen = 0;
   SFndLenMsg(mBuf, &mLen);

   cmMemset((U8 *)&ipv4Hdr, 0, sizeof(CmIpv4Hdr));
   ipv4Hdr.length = CM_IPV4_HDRLEN + mLen;
   ipv4Hdr.hdrVer = 0x45;
   ipv4Hdr.proto = 1;
   ipv4Hdr.srcAddr = CM_INET_NTOH_U32(egtpCb.egtpCfg.localIp.ipV4Addr);
   ipv4Hdr.destAddr = CM_INET_NTOH_U32(egtpCb.egtpCfg.destIp.ipV4Addr);
 
   /* Packing IPv4 header into buffer */
   S16          ret, cnt, idx;
   Data         revPkArray[CM_IPV4_HDRLEN];
   Data         pkArray[CM_IPV4_HDRLEN];
 
   /* initialize locals */
   cnt = 0;
   cmMemset(revPkArray, 0, CM_IPV4_HDRLEN);
   cmMemset(pkArray, 0, CM_IPV4_HDRLEN);

   /* Pack Header Version */
   pkArray[cnt++] = ipv4Hdr.hdrVer;
 
   /* Pack TOS */
   pkArray[cnt++] = ipv4Hdr.tos;
 
   pkArray[cnt++] = (Data)GetHiByte(ipv4Hdr.length);
   pkArray[cnt++] = (Data)GetLoByte(ipv4Hdr.length);
 
   /* Pack Id */
   pkArray[cnt++] = (Data) GetHiByte(ipv4Hdr.id);
   pkArray[cnt++] = (Data) GetLoByte(ipv4Hdr.id);
 
   /* Pack Offset */
   pkArray[cnt++] = (Data)GetHiByte(ipv4Hdr.off);
   pkArray[cnt++] = (Data)GetLoByte(ipv4Hdr.off);
 
   /* Pack TTL */
   pkArray[cnt++] = ipv4Hdr.ttl;
 
   /* Pack Protocol */
   pkArray[cnt++] = ipv4Hdr.proto;
 
   /* Pack Checksum */
   pkArray[cnt++] = (Data)GetHiByte(ipv4Hdr.chkSum);
   pkArray[cnt++] = (Data)GetLoByte(ipv4Hdr.chkSum);
                                                        
   /* Pack Source Address */
   pkArray[cnt++] = (Data)GetHiByte(GetHiWord(ipv4Hdr.srcAddr));
   pkArray[cnt++] = (Data)GetLoByte(GetHiWord(ipv4Hdr.srcAddr));
   pkArray[cnt++] = (Data)GetHiByte(GetLoWord(ipv4Hdr.srcAddr));
   pkArray[cnt++] = (Data)GetLoByte(GetLoWord(ipv4Hdr.srcAddr));
 
   /* Pack Destination Address */
   pkArray[cnt++] = (Data)GetHiByte(GetHiWord(ipv4Hdr.destAddr));
   pkArray[cnt++] = (Data)GetLoByte(GetHiWord(ipv4Hdr.destAddr));
   pkArray[cnt++] = (Data)GetHiByte(GetLoWord(ipv4Hdr.destAddr));
   pkArray[cnt++] = (Data)GetLoByte(GetLoWord(ipv4Hdr.destAddr));
 
   for (idx = 0;  idx < CM_IPV4_HDRLEN;  idx++)
      revPkArray[idx] = pkArray[CM_IPV4_HDRLEN - idx -1];
 
   /* this function automatically reverses revPkArray */
   ret = SAddPreMsgMult(revPkArray, (MsgLen)cnt, mBuf);
 
   egtpMsg->msgHdr.msgType = EGTPU_MSG_GPDU;
   egtpMsg->msgHdr.nPdu.pres = FALSE;
   egtpMsg->msgHdr.seqNum.pres = FALSE;
   egtpMsg->msgHdr.extHdr.udpPort.pres = FALSE;
   egtpMsg->msgHdr.extHdr.pdcpNmb.pres = FALSE;
   egtpMsg->msgHdr.teId = 10;
   egtpMsg->msg = mBuf;

   return ROK;
}


S16 BuildEgtpMsg(EgtpMsg *egtpMsg)
{
   EgtpTeIdCb   *teidCb = NULLP;
   MsgLen tPduSize;
   U8     hdrLen;
   U32    msgLen;
   EgtpMsgHdr   *msgHdr;
 
   cmHashListFind(&(egtpCb.dstCb.teIdLst), (U8 *)&(egtpMsg->msgHdr.teId), sizeof(U32), 0, (PTR *)&teidCb);
   if(teidCb == NULLP)
   {
      DU_LOG("\nEGTP : Tunnel id[%d] not configured", egtpMsg->msgHdr.teId);
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   msgHdr = &(egtpMsg->msgHdr);

   hdrLen = teidCb->preEncodedHdr.cnt;

   if(msgHdr->extHdr.pdcpNmb.pres)
   {
      teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 1] |= EGTP_MASK_BIT3;
      teidCb->preEncodedHdr.hdr[hdrLen] = EGTP_EXT_HDR_PDCP_TYPE;
      teidCb->preEncodedHdr.hdr[--hdrLen] = 1;
      teidCb->preEncodedHdr.hdr[--hdrLen] = GetHiByte(msgHdr->extHdr.pdcpNmb.val);
      teidCb->preEncodedHdr.hdr[--hdrLen] = GetLoByte(msgHdr->extHdr.pdcpNmb.val);
      teidCb->preEncodedHdr.hdr[--hdrLen] = 0;
   }
   else
   {
      teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 1] &= ~(EGTP_MASK_BIT3);
   }
 
   SFndLenMsg(egtpMsg->msg, &tPduSize);

   /*Adjust the header to fill the correct length*/
   msgLen = tPduSize +  (EGTP_MAX_HDR_LEN - hdrLen) - 0x08;

   /***********************************************
    * Fill the length field of the message header *
    ***********************************************/
   teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 3] = (U8)GetHiByte(msgLen);
   teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 4] = (U8)GetLoByte(msgLen);

   /*Update the sequence number*/
   if(egtpMsg->msgHdr.seqNum.pres)
   {
      teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 1] |= (EGTP_MASK_BIT2);
      teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 9] = (U8)GetHiByte(egtpMsg->msgHdr.seqNum.val);
      teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 10] = (U8)GetLoByte(egtpMsg->msgHdr.seqNum.val);
   }
   else
   {
      teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 1] &= ~(EGTP_MASK_BIT2);
   }

   SAddPreMsgMult(&teidCb->preEncodedHdr.hdr[hdrLen], (EGTP_MAX_HDR_LEN - hdrLen), egtpMsg->msg);

   DU_LOG("\nEGTP : Sending message buffer");
   SPrntMsg(egtpMsg->msg, 0, 0);

   return ROK;
}

S16 cuEgtpSendMsg(Buffer *mBuf)
{
   S16            ret;
   MsgLen         txLen;
   CmInetMemInfo  info;
   CmInetAddr     dstAddr;

   info.region = CU_APP_MEM_REG;
   info.pool = CU_POOL;
 
   dstAddr.port = EGTP_DFLT_PORT;
   dstAddr.address = egtpCb.dstCb.dstIp;
 
   ret = cmInetSendMsg(&(egtpCb.dstCb.sendTptSrvr.sockFd), &dstAddr, &info, mBuf, &txLen, CM_INET_NO_FLAG);
   if(ret != ROK && ret != RWOULDBLOCK)
   {
      DU_LOG("\nEGTP : Message send failure");
      return RFAILED;
   }
   
   DU_LOG("\nEGTP : Message Sent");
 
   return ROK;
}
