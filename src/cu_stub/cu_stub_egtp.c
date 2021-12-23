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
#include "cu_f1ap_msg_hdl.h"
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
  DU_LOG("\n\nDEBUG  -->  EGTP : Initializing");
  memset (&egtpCb, 0, sizeof(EgtpGlobalCb));
  
  //Initializing with INVALID value
  memset(egtpCb.gCntPdu, 0xFF , sizeof(uint8_t)*(MAX_TEID+1)); 
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
uint8_t egtpInitReq()
{
   uint8_t ret = ROK;

   ret = cuEgtpCfgReq();
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  EGTP : Configuration failed");
      return (ret);
   }

   ret = cuEgtpSrvOpenReq();
   if(ret != ROK)
   {
       DU_LOG("\nERROR  -->  EGTP : Transport server open request failed");
       return (ret);
   }

   return ret;
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
   uint8_t ret;

   memcpy(&egtpCb.egtpCfg, &cuCfgParams.egtpParams, sizeof(EgtpParams));

   egtpCb.recvTptSrvr.addr.address = CM_INET_NTOH_UINT32(egtpCb.egtpCfg.localIp.ipV4Addr);
   egtpCb.recvTptSrvr.addr.port = EGTP_DFLT_PORT;

   egtpCb.dstCb.dstIp = CM_INET_NTOH_UINT32(egtpCb.egtpCfg.destIp.ipV4Addr);
   egtpCb.dstCb.dstPort = egtpCb.egtpCfg.destPort;
   egtpCb.dstCb.sendTptSrvr.addr.address = CM_INET_NTOH_UINT32(egtpCb.egtpCfg.localIp.ipV4Addr);
   egtpCb.dstCb.sendTptSrvr.addr.port = egtpCb.egtpCfg.localPort;
   egtpCb.dstCb.numTunn = 0;

   ret = cmHashListInit(&(egtpCb.dstCb.teIdLst), 1024, sizeof(EgtpTeIdCb), FALSE, CM_HASH_KEYTYPE_UINT32_MOD, CU_APP_MEM_REG, CU_POOL);

   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  EGTP : TeId hash list initialization failed");
      return RFAILED;
   }
   else
   {
      DU_LOG("\nINFO  -->  EGTP : Configuration successful");
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

   uint8_t ret;

   DU_LOG("\nINFO  -->  EGTP : Received open server request");
 
   sockType = CM_INET_DGRAM;
   if((ret = (cmInetSocket(sockType, &(egtpCb.recvTptSrvr.sockFd), protType))) != ROK)
   {
      DU_LOG("\nERROR  -->  EGTP : Failed to open UDP socket");
      return RFAILED;
   }

   ret = cmInetBind(&(egtpCb.recvTptSrvr.sockFd), &(egtpCb.recvTptSrvr.addr));
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  EGTP : Failed to bind socket");
      return RFAILED;
   }

   if(ret = (cmInetSocket(sockType, &(egtpCb.dstCb.sendTptSrvr.sockFd), protType)) != ROK)
   {  
      DU_LOG("\nERROR  -->  EGTP : Failed to open UDP socket");
      return RFAILED;
   }
      
   ret = cmInetBind(&(egtpCb.dstCb.sendTptSrvr.sockFd), &(egtpCb.dstCb.sendTptSrvr.addr));
   if(ret != ROK)
   {  
      DU_LOG("\nERROR  -->  EGTP : Failed to bind socket");
      return RFAILED;
   }
       
   /* TODO: set socket options */

   DU_LOG("\nINFO  -->  EGTP : Receiver socket[%d] and Sender socket[%d] open", egtpCb.recvTptSrvr.sockFd.fd,\
   egtpCb.dstCb.sendTptSrvr.sockFd.fd);
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

   DU_LOG("\nINFO  -->  EGTP : Received tunnel management request");
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
         DU_LOG("\nERROR  -->  EGTP : Invalid tunnel management action[%d]", tnlEvt.action);
         ret = RFAILED;
      }
   }

   return (ret);
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
   

   DU_LOG("\nINFO  -->  EGTP : Tunnel addition : LocalTeid[%d] Remote Teid[%d]", tnlEvt.lclTeid, tnlEvt.remTeid);

   CU_ALLOC(teidCb, (Size)sizeof(EgtpTeIdCb));

   if(teidCb == NULLP)
   {
      DU_LOG("\nERROR  -->  EGTP : Memory allocation failed");
      return RFAILED;
   }


   memset(teidCb, 0, sizeof(EgtpTeIdCb));
   teidCb->teId = tnlEvt.lclTeid;
   teidCb->remTeId = tnlEvt.remTeid;

   ret = cmHashListInsert(&(egtpCb.dstCb.teIdLst), (PTR)teidCb, (uint8_t *)&(teidCb->teId), sizeof(uint32_t));
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  EGTP : Failed to insert in hash list");
      CU_FREE(teidCb, (Size)sizeof(EgtpTeIdCb));
      return RFAILED;
   }
   egtpCb.dstCb.numTunn++;

   /* Encoding pre-defined header */
   memset(&preDefHdr, 0, sizeof(EgtpMsgHdr));
   preDefHdr.msgType = EGTPU_MSG_GPDU;
   preDefHdr.teId = teidCb->remTeId;
   preDefHdr.extHdr.pdcpNmb.pres = FALSE;
   preDefHdr.extHdr.udpPort.pres = FALSE;
   preDefHdr.nPdu.pres = TRUE; //Including nPdu when sending data
   preDefHdr.nPdu.val = 0;

   cuEgtpEncodeHdr((uint8_t *)teidCb->preEncodedHdr.hdr, &preDefHdr, &(teidCb->preEncodedHdr.cnt));
   egtpCb.gCntPdu[teidCb->remTeId] = 0;//Resetting the Cnt Value for this DRB which indicates its creation

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

   DU_LOG("\nDEBUG  -->  CU_STUB : Tunnel modification : LocalTeid[%d] Remote Teid[%d]", tnlEvt.lclTeid, tnlEvt.remTeid);

   cmHashListFind(&(egtpCb.dstCb.teIdLst), (uint8_t *)&(tnlEvt.teId), sizeof(uint32_t), 0, (PTR *)&teidCb);
   if(teidCb == NULLP)
   {
      DU_LOG("\nDEBUG --> CU_STUBTunnel id not found");
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

   DU_LOG("\nDEBUG  -->  EGTP : Tunnel deletion : Local Teid[%d] Remote Teid[%d]", tnlEvt.lclTeid, tnlEvt.remTeid);
   
   cmHashListFind(&(egtpCb.dstCb.teIdLst), (uint8_t *)&(tnlEvt.lclTeid), sizeof(uint32_t), 0, (PTR *)&teidCb);
   if(teidCb == NULLP)
   {
      DU_LOG("\nERROR  -->  EGTP : Tunnel id[%d] not configured", tnlEvt.lclTeid);
      return RFAILED;
   } 

   cmHashListDelete(&(egtpCb.dstCb.teIdLst), (PTR)teidCb);
   CU_FREE(teidCb, (Size)sizeof(EgtpTeIdCb));
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
S16 cuEgtpEncodeHdr(uint8_t *preEncodedHdr, EgtpMsgHdr *preDefHdr, uint8_t *hdrIdx)
{
   uint8_t         tmpByte = 0;                 /* Stores one byte of data for enc */
   uint8_t         cnt     = EGTP_MAX_HDR_LEN;  /* Stores the position */
   Bool       extPres = FALSE;             /* Flag for indication of S, E or P presense flag */
   uint16_t        nwWord = 0;

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
   nwWord = (uint16_t)(GetHiWord(preDefHdr->teId));
   preEncodedHdr[--cnt] = (uint8_t)(GetHiByte(nwWord));
   preEncodedHdr[--cnt] = (uint8_t)(GetLoByte(nwWord));
   nwWord = (uint16_t)(GetLoWord(preDefHdr->teId));
   preEncodedHdr[--cnt] = (uint8_t)(GetHiByte(nwWord));
   preEncodedHdr[--cnt] = (uint8_t)(GetLoByte(nwWord));
    
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
   //return (cuEgtpDatReq());

}

S16 cuEgtpDecodeHdr(Buffer *mBuf)
{
   EgtpMsg  egtpMsg;
   uint8_t       tmpByte[5];         /* Holds one byte of data after Dec */
   uint8_t       version = 0;         /* Holds the version type, decoded */
   MsgLen   msgLen  = 0;         /* Holds the msgLen from the Hdr */
   MsgLen   bufLen  = 0;         /* Holds the total buffer length */
   uint8_t       extHdrType = 0;       /* Holds the Extension hdr type */
   uint8_t       extHdrLen = 0;        /* Extension hdr length */
   Bool     extPres = FALSE;      /* Flag for indication of S, E or P presense flag */

   ODU_GET_MSG_LEN(mBuf, &bufLen);

   /* Decode version */
   ODU_REM_PRE_MSG(&tmpByte[0], mBuf);
   version = tmpByte[0] >> 5;

   /* Decode message type */
   ODU_REM_PRE_MSG((Data*)&(egtpMsg.msgHdr.msgType), mBuf);

   /* Decode message length */
   ODU_REM_PRE_MSG(&tmpByte[1], mBuf);
   ODU_REM_PRE_MSG(&tmpByte[2], mBuf);
   msgLen = (tmpByte[1] << 8) | tmpByte[2];

   /* Decode tunnel id */
   ODU_REM_PRE_MSG(&tmpByte[1], mBuf);
   ODU_REM_PRE_MSG(&tmpByte[2], mBuf);
   ODU_REM_PRE_MSG(&tmpByte[3], mBuf);
   ODU_REM_PRE_MSG(&tmpByte[4], mBuf);
   egtpMsg.msgHdr.teId = (tmpByte[1] << 24) | (tmpByte[2] << 16) | (tmpByte[3] << 8) | tmpByte[4];

   if((tmpByte[0] & EGTP_MASK_BIT1) || (tmpByte[0] & EGTP_MASK_BIT2)||(tmpByte[0] & EGTP_MASK_BIT3))
   {
      extPres = TRUE;
   }

   /* Decode sequence number */
   if ( tmpByte[0] & EGTP_MASK_BIT2 )
   {
      egtpMsg.msgHdr.seqNum.pres = TRUE;
      ODU_REM_PRE_MSG(&tmpByte[1], mBuf);
      ODU_REM_PRE_MSG(&tmpByte[2], mBuf);
      egtpMsg.msgHdr.seqNum.val = (tmpByte[1] << 8) | tmpByte[2];
   }
   else  if(extPres)
   {
      egtpMsg.msgHdr.seqNum.pres = 0;
      ODU_REM_PRE_MSG(&tmpByte[1], mBuf);
      ODU_REM_PRE_MSG(&tmpByte[2], mBuf);
      egtpMsg.msgHdr.seqNum.val = (tmpByte[1] << 8) | tmpByte[2];
   }

   /* Decode N-PDU number */
   if ( tmpByte[0] & EGTP_MASK_BIT1 )
   {
      egtpMsg.msgHdr.nPdu.pres = TRUE;
      ODU_REM_PRE_MSG(&(egtpMsg.msgHdr.nPdu.val), mBuf);
   }
   else if(extPres)
   {
      egtpMsg.msgHdr.nPdu.pres = TRUE;
      ODU_REM_PRE_MSG(&(egtpMsg.msgHdr.nPdu.val), mBuf);
   }

   if(extPres & EGTP_MASK_BIT1)
   {
      ODU_REM_PRE_MSG(&extHdrType, mBuf);
      while( 0 != extHdrType)
      {
         switch (extHdrType)
         {
            case EGTP_EXT_HDR_UDP_TYPE:
            {
               ODU_REM_PRE_MSG(&extHdrLen, mBuf);
               if(extHdrLen == 0x01)
               {
                  egtpMsg.msgHdr.extHdr.udpPort.pres = TRUE;
                  ODU_REM_PRE_MSG(&tmpByte[1], mBuf);
                  ODU_REM_PRE_MSG(&tmpByte[2], mBuf);
                  egtpMsg.msgHdr.extHdr.udpPort.val = (tmpByte[1] << 8) | tmpByte[2];
               }
               break;
            }

            case EGTP_EXT_HDR_PDCP_TYPE:
            {
               ODU_REM_PRE_MSG(&extHdrLen, mBuf);
               if(extHdrLen == 0x01)
               {
                  egtpMsg.msgHdr.extHdr.pdcpNmb.pres = TRUE;
                  ODU_REM_PRE_MSG(&tmpByte[1], mBuf);
                  ODU_REM_PRE_MSG(&tmpByte[2], mBuf);
                  egtpMsg.msgHdr.extHdr.pdcpNmb.val = (tmpByte[1] << 8) | tmpByte[2];
               }
               break;
            }
         } /* End of switch */

         ODU_REM_PRE_MSG(&extHdrType, mBuf);

      } /* End of while */
   } /* End of if(extPres & EGTP_MASK_BIT1) */
   else if(extPres)
   {
      ODU_REM_PRE_MSG(&extHdrType, mBuf);
   }

   DU_LOG("\nDEBUG  -->  EGTP : Message Buffer after decoding header [TEID:%d]",egtpMsg.msgHdr.teId);
   ODU_PRINT_MSG(mBuf, 0, 0);

   SPutMsg(mBuf);

   return ROK;
     
} /* End of cuEgtpDecodeHdr */

uint16_t cuEgtpDatReq(uint8_t teId)
{
   uint8_t ret = ROK, cnt = 0;
   EgtpMsg  egtpMsg;

   egtpMsg.msgHdr.teId = teId;
   
   if(egtpCb.gCntPdu[teId] == 0xFF) //DRB not created
   {
      DU_LOG("\nERROR  -->  EGTP : DRB  not created");
      return RFAILED ;
   }
   /* Build Application message that is supposed to come from app to egtp */
   ret = BuildAppMsg(&egtpMsg);
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  EGTP : Failed to build App Msg");
      return RFAILED;
   }

   /* Encode EGTP header to build final EGTP message */
   ret = BuildEgtpMsg(&egtpMsg);
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  EGTP : Failed to build EGTP Msg");
      return RFAILED;
   }
   cuEgtpSendMsg(egtpMsg.msg);
   ODU_PUT_MSG_BUF(egtpMsg.msg);

   return ROK;
}


S16 BuildAppMsg(EgtpMsg  *egtpMsg)
{
   char data[1215] = "In telecommunications, 5G is the fifth generation technology standard for broadband cellular"
   " networks, which cellular phone companies began deploying worldwide in 2019, and is the planned successor to the 4G "
   " networks which provide connectivity to most current cellphones. 5G networks are predicted to have more than 1.7"
   " billion subscribers worldwide by 2025, according to the GSM Association.Like its predecessors, 5G networks are"
   " cellular networks,in which the service area is divided into small geographical areas called cells.All 5G wireless"
   " devices in a cell are connected to the Internet and telephone network by radio waves through local antenna in the"
   " cell. The main advantage of the new networks is that they will have greater bandwidth, giving higher download"
   " speeds, eventually up to 10 gigabits per second(Gbit/s). Due to the increased bandwidth, it is expected the"
   " networks will not exclusively serve cellphones like existing cellular networks, but also be used as general"
   " internet service providers for laptops and desktop computers, competing with existing ISPs such as cable"
   " internet, and also will make possible new applications in internet of things (IoT) and machine to machine areas.";

   int datSize = 1215;
 
   Buffer   *mBuf;
 
   if(ODU_GET_MSG_BUF(CU_APP_MEM_REG, CU_POOL, &mBuf) == ROK)
   {
      if(ODU_ADD_POST_MSG_MULT((Data *)data, datSize, mBuf) != ROK)
      {
         DU_LOG("\nERROR  -->  EGTP : ODU_ADD_POST_MSG_MULT failed");
         ODU_PUT_MSG_BUF(mBuf);
         return RFAILED;
      }
   }
   else
   {
       DU_LOG("\nERROR  -->  EGTP : Failed to allocate memory");
       return RFAILED;
   }
 
   /* filling IPv4 header */
   CmIpv4Hdr ipv4Hdr;
   MsgLen    mLen;
 
   mLen = 0;
   ODU_GET_MSG_LEN(mBuf, &mLen);

   memset(&ipv4Hdr, 0, sizeof(CmIpv4Hdr));
   ipv4Hdr.length = CM_IPV4_HDRLEN + mLen;
   ipv4Hdr.hdrVer = 0x45;
   ipv4Hdr.proto = 1;
   ipv4Hdr.srcAddr = CM_INET_NTOH_UINT32(egtpCb.egtpCfg.localIp.ipV4Addr);
   ipv4Hdr.destAddr = CM_INET_NTOH_UINT32(egtpCb.egtpCfg.destIp.ipV4Addr);
 
   /* Packing IPv4 header into buffer */
   S16          ret, cnt, idx;
   Data         revPkArray[CM_IPV4_HDRLEN];
   Data         pkArray[CM_IPV4_HDRLEN];
 
   /* initialize locals */
   cnt = 0;
   memset(revPkArray, 0, CM_IPV4_HDRLEN);
   memset(pkArray, 0, CM_IPV4_HDRLEN);

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
   ret = ODU_ADD_PRE_MSG_MULT(revPkArray, (MsgLen)cnt, mBuf);
 
   egtpMsg->msgHdr.msgType = EGTPU_MSG_GPDU;
   egtpMsg->msgHdr.nPdu.pres = TRUE;

   if(egtpCb.gCntPdu[egtpMsg->msgHdr.teId] != NUM_DL_PACKETS)
     egtpCb.gCntPdu[egtpMsg->msgHdr.teId]++;
   else
     egtpCb.gCntPdu[egtpMsg->msgHdr.teId] = 1;

   egtpMsg->msgHdr.nPdu.val = egtpCb.gCntPdu[egtpMsg->msgHdr.teId];
   egtpMsg->msgHdr.seqNum.pres = FALSE;
   egtpMsg->msgHdr.extHdr.udpPort.pres = FALSE;
   egtpMsg->msgHdr.extHdr.pdcpNmb.pres = FALSE;
   egtpMsg->msg = mBuf;

   return ret;
}

S16 BuildEgtpMsg(EgtpMsg *egtpMsg)
{
   EgtpTeIdCb   *teidCb = NULLP;
   MsgLen tPduSize;
   uint8_t     hdrLen;
   uint32_t    msgLen;
   EgtpMsgHdr   *msgHdr;
 
   cmHashListFind(&(egtpCb.dstCb.teIdLst), (uint8_t *)&(egtpMsg->msgHdr.teId), sizeof(uint32_t), 0, (PTR *)&teidCb);
   if(teidCb == NULLP)
   {
      DU_LOG("\nERROR  -->  EGTP : Tunnel id[%d] not configured", egtpMsg->msgHdr.teId);
      return (LCM_REASON_INVALID_PAR_VAL);
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
 
   ODU_GET_MSG_LEN(egtpMsg->msg, &tPduSize);

   /*Adjust the header to fill the correct length*/
   msgLen = tPduSize +  (EGTP_MAX_HDR_LEN - hdrLen) - 0x08;

   /***********************************************
    * Fill the length field of the message header *
    ***********************************************/
   teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 3] = (uint8_t)GetHiByte(msgLen);
   teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 4] = (uint8_t)GetLoByte(msgLen);

   /*Update the sequence number*/
   if(egtpMsg->msgHdr.seqNum.pres)
   {
      teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 1] |= (EGTP_MASK_BIT2);
      teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 9] = (uint8_t)GetHiByte(egtpMsg->msgHdr.seqNum.val);
      teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 10] = (uint8_t)GetLoByte(egtpMsg->msgHdr.seqNum.val);
   }
   else
   {
      teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 1] &= ~(EGTP_MASK_BIT2);
   }

   /*Update the nPdU number*/
   if(egtpMsg->msgHdr.nPdu.pres)
   {
      teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 1] |= (EGTP_MASK_BIT1);
      teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 11] = egtpMsg->msgHdr.nPdu.val;
   }
   else
   {
      teidCb->preEncodedHdr.hdr[EGTP_MAX_HDR_LEN - 1] &= ~(EGTP_MASK_BIT1);
   }

   ODU_ADD_PRE_MSG_MULT(&teidCb->preEncodedHdr.hdr[hdrLen], (EGTP_MAX_HDR_LEN - hdrLen), egtpMsg->msg);
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
      DU_LOG("\nERROR  -->  EGTP : Message send failure");
      return RFAILED;
   }
   
   DU_LOG("\nDEBUG  -->  EGTP : Message Sent");
 
   return ROK;
}
