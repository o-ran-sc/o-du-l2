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

/* This file is the entry point for UDP P7 */
#ifdef NFAPI_ENABLED

#include "common_def.h"
#include "lwr_mac.h"
#include "du_app_p7udp_inf.h"
#include "nfapi_interface.h"
#include "nfapi_common.h"
#include "nfapi_p7_msg_hdl.h"

/**************************************************************************
 * @brief Task Initiation callback function. 
 *
 * @details
 *
 *     Function : udpP7ActvInit 
 *    
 *     Functionality:
 *             This function is supplied as one of parameters during UDP P7
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
uint8_t udpP7ActvInit(Ent entity, Inst inst, Region region, Reason reason)
{
   return ROK;
}

/**************************************************************************
 * @brief Task Activation callback function. 
 *
 * @details
 *
 *      Function : udpP7ActvTsk 
 * 
 *      Functionality:
 *           Primitives invoked by UDP P7 users/providers through
 *           a loosely coupled interface arrive here by means of 
 *           SSI's message handling. This API is registered with
 *           SSI during the Task Registration of DU APP.
 *     
 * @param[in]  Pst     *pst, Post structure of the primitive.     
 * @param[in]  Buffer *mBuf, Packed primitive parameters in the
 *  buffer.
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t udpP7ActvTsk(Pst *pst, Buffer *mBuf)
{
   uint8_t ret = ROK;

   switch(pst->srcEnt)
   {
      case ENTDUAPP:
      {
         switch(pst->event)
         {
            case EVENT_NFAPI_P7_UDP_CFG:
            {
                unpackDuNfapiP7UdpCfg(NfapiProcP7UdpCfg, pst, mBuf);
                break;
            }
            default:
            {
                DU_LOG("ERROR  -> NFAPI_VNF: UDP Act task received Incorrect event:%d",\
                                                                  pst->event);
                ret = RFAILED;
                break;
            }
          }
          break;
       }
       case ENTUDPP7:
       {
           switch(pst->event)
           {
              case EVTSTARTNFAPIP7POLL:
              {
                 DU_LOG("INFO   --> VNF_NFAPI:Start Polling");
                 nfapiP7UdpRecvMsg();
                 ODU_PUT_MSG_BUF(mBuf);
                 break;
              }
              default:
              {
                 DU_LOG("ERROR  -> NFAPI_VNF: UDP Act task received Incorrect event:%d",\
                                                                  pst->event);
                 ret = RFAILED;
                 break;
              }
           }
           break;
        }
       default:
       {
          DU_LOG("ERROR  --> NFAPI_VNF: UDP Act task received from wrong Entity:%d",\
                                                                  pst->srcEnt);
          ret = RFAILED;
          break;
       }
   }
   
   ODU_EXIT_TASK();
   return ret;
}

/**************************************************************************
 * @brief Handles the NFAPI_UDP_P7 Configuration received from DUAPP 
 *
 * @details
 *
 *      Function : NfapiProcP7UdpCfg 
 * 
 *      Functionality: UDP P7 Configuration recevied from DUAPP has to be stored
 *      in Database of NFAPI which will be used during UDP Socket Establishment
 *     
 * @param[in]  Pst     *pst, Post structure of the primitive.     
 * @param[in]  Buffer *mBuf, Packed primitive parameters in the
 *  buffer.
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t NfapiProcP7UdpCfg(Pst *pst, NfapiP7UdpCfg *nfapiP7UdpCfg)
{
   uint8_t ret = ROK;
   
   DU_LOG("INFO --> NFAPI_VNF: Received UDP P7 Configuration from DUAPP");
   if(nfapiP7UdpCfg)
   {
      if(nfapiP7UdpCfg->ipv4P7VnfPres)
      {
         vnfDb.p7TransInfo.srcIpv4Address = nfapiP7UdpCfg->ipv4P7VnfAddr;
         vnfDb.p7TransInfo.srcIpv4Port = nfapiP7UdpCfg->p7VnfPort;
         vnfDb.p7TransInfo.destIpv4Address = nfapiP7UdpCfg->ipv4P7PnfAddr;
         vnfDb.p7TransInfo.destIpv4Port = nfapiP7UdpCfg->p7PnfPort;
         vnfDb.p7TransInfo.srcIpNetAddr.address = CM_INET_NTOH_UINT32(vnfDb.p7TransInfo.srcIpv4Address);
         vnfDb.p7TransInfo.srcIpNetAddr.port = vnfDb.p7TransInfo.srcIpv4Port;
         vnfDb.p7TransInfo.destIpNetAddr.address = CM_INET_NTOH_UINT32(vnfDb.p7TransInfo.destIpv4Address);
         vnfDb.p7TransInfo.destIpNetAddr.port = vnfDb.p7TransInfo.destIpv4Port;
      }

      NFAPI_UDP_P7_FREE_SHRABL_BUF(pst->region, pst->pool, nfapiP7UdpCfg, sizeof(NfapiP7UdpCfg));
   }
   else
   {
      DU_LOG("INFO  -->  MAC : Received NfapiP7UdpCfg is NULL");
      ret =  RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Processing Client open request
 *
 * @details
 *
 *    Function : nfapiP7UdpCliOpenPrc
 *
 *    Functionality: Processing UDP P7 Client open request
 *      
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t nfapiP7UdpCliOpenPrc(uint8_t sockType)
{
   S8 ret=ROK;
   ret = cmInetSocket(sockType, &(vnfDb.p7TransInfo.sockFd), CM_INET_PROTO_UDP); 
	if(ret != ROK)
   {  
      DU_LOG("ERROR  -->  NFAPI_VNF : Failed to open UDP socket");
      return ret;
   }
   ret = cmInetBind(&(vnfDb.p7TransInfo.sockFd), &(vnfDb.p7TransInfo.srcIpNetAddr));  
   if(ret != ROK)
   {  
      DU_LOG("ERROR  -->  NFAPI_VNF : Failed to bind socket");
      return ret;
   }
   
   return ret;
}

/*******************************************************************
 *
 * @brief Packs Polling requesting and sends to NFAPI P7 UDP
 *
 * @details
 *
 *    Function : packNfapiP7UdpStartPollingReq
 *
 *    Functionality:
 *       Packs polling request and sends to NFAPI P7 UDP
 *
 * @params[in] Post structure
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************/
uint8_t packNfapiP7UdpStartPollingReq(Pst *pst)
{
   Buffer *mBuf;

   if(ODU_GET_MSG_BUF(DFLT_REGION, pst->pool, &mBuf) != ROK)
   {
      DU_LOG("ERROR  -->  NFAPI_VNF : Failed to allocate memory");
      return RFAILED;
   }

   ODU_POST_TASK(pst, mBuf);
   return ROK;
}

/**************************************************************************
 * @brief NFAPI P7 Interface's UDP open request 
 *
 * @details
 *
 *      Function : nfapiP7UdpOpenReq
 * 
 *      Functionality:
 *           This function handles NFAPI P7 UDP open request.
 *           It opens udp socket to receive/send msgs.
 *     
 * @param[in]  Pst *pst, post structure
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/

uint8_t nfapiP7UdpOpenReq()
{
   uint8_t  ret = ROK;       /* Return value */
   Pst      nfapiP7Pst;   /* Self post */
   uint8_t  sockType;  /* Socket type */

   DU_LOG("DEBUG  -->  NFAPI_VNF : Received NFAPI P7's UDP open Client request");
 
   sockType = CM_INET_DGRAM;
   ret = nfapiP7UdpCliOpenPrc(sockType);
   /* Opening and Binding receiver socket */
   if(ret != ROK)
   {
      DU_LOG("ERROR  -->  NFAPI_VNF : Failed while opening receiver transport server");
      return ret;
   }

   DU_LOG("DEBUG  -->  NFAPI_VNF : Socket [%d] is open", vnfDb.p7TransInfo.sockFd.fd);

   /* Start Socket polling */
   memset(&nfapiP7Pst, 0, sizeof(nfapiP7Pst));
   nfapiP7Pst.srcEnt = (Ent)ENTUDPP7;
   nfapiP7Pst.srcInst = (Inst)NFAPI_UDP_P7_INST;
   nfapiP7Pst.srcProcId = NFAPI_UDP_P7_PROC;
   nfapiP7Pst.dstEnt = (Ent)ENTUDPP7;
   nfapiP7Pst.dstInst = (Inst)NFAPI_UDP_P7_INST;
   nfapiP7Pst.dstProcId = NFAPI_UDP_P7_PROC;
   nfapiP7Pst.event = EVTSTARTNFAPIP7POLL;
   nfapiP7Pst.selector = ODU_SELECTOR_LC;
   nfapiP7Pst.pool= NFAPI_UDP_P7_POOL;
   packNfapiP7UdpStartPollingReq(&nfapiP7Pst);

   /*Trigger P7 Clock as well */
   DU_LOG("DEBUG  -->  NFAPI_VNF : Triggering Clock");
   nfapiTriggerP7Clock();
   return ret;
}

/*******************************************************************
 *
 * @brief Receives NFAPI P7 message from UDP socket 
 *
 * @details
 *
 *    Function : nfapiP7UdpRecvMsg
 *
 *    Functionality:
 *      Receive incoming messages from UDP socket
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t nfapiP7UdpRecvMsg()
{
   uint8_t        ret = ROK;           /* Return value */
   uint16_t       bufLen;        /* Length of received buffer */
   Buffer         *recvBuf;      /* Received buffer */
   CmInetMemInfo  memInfo;       /* Buffer allocation info */
   memInfo.region = NFAPI_UDP_P7_MEM_REGION;
   memInfo.pool   = NFAPI_UDP_P7_POOL;

   while(true)
   {
      bufLen = -1;
      ret = RFAILED;
      recvBuf = NULLP;
      ret = cmInetRecvMsg(&vnfDb.p7TransInfo.sockFd, &vnfDb.p7TransInfo.destIpNetAddr, &memInfo, &recvBuf, (int16_t *)&bufLen, CM_INET_NO_FLAG);
      if(ret == ROK && recvBuf != NULLP)
      {
         DU_LOG("DEBUG  -->  NFAPI_VNF : Received P7 Message\n");
  
         nfapiP7MsgHandler(recvBuf);
         ODU_PUT_MSG_BUF(recvBuf);
      }
   }
 
   return ROK;
}

/*******************************************************************
 *
 * @brief Sends message over UDP
 *
 * @details
 *
 *    Function : nfapiP7UdpSendMsg
 *
 *    Functionality:
 *           Sends message over UDP
 *
 * @params[in] Message Buffer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t nfapiP7UdpSendMsg(Buffer *mBuf)
{
   uint8_t        ret;
   uint16_t       txLen;
   CmInetMemInfo  info;
   static uint64_t numDataSent = 0;

   info.region = NFAPI_UDP_P7_MEM_REGION;
   info.pool = NFAPI_UDP_P7_POOL;

   DU_LOG(" udp sending: add:0x%x, port:%d",  vnfDb.p7TransInfo.destIpNetAddr.address,  vnfDb.p7TransInfo.destIpNetAddr.port );
   ret = cmInetSendMsg(&vnfDb.p7TransInfo.sockFd, &vnfDb.p7TransInfo.destIpNetAddr, &info, mBuf, (int16_t *)&txLen, CM_INET_NO_FLAG);
   if(ret != ROK && ret != RWOULDBLOCK)
   {
      DU_LOG("ERROR  -->  NFAPI_VNF : Failed sending the message");
      return RFAILED;
   }
   else
   {
      DU_LOG("DEBUG -->  NFAPI_VNF : Sent NFAPI P7 Message [%ld]", numDataSent+1);
      numDataSent++;
   }

   return ROK;
}

#endif
/**********************************************************************
         End of file
**********************************************************************/
