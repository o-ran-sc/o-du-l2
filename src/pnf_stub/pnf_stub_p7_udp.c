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
 
/* This file contains all P7 UDP related functionality */

#include "common_def.h"
#include "cm_inet.x"
#include "nfapi_interface.h"
#include "pnf_stub_p7_udp.h"
#include "pnf_stub_sctp.h"
#include "pnf_stub.h"

uint8_t sockType;
uint8_t protType;

/**************************************************************************
 * @brief Task Initiation callback function. 
 *
 * @details
 *
 *     Function :  pnfP7UdpActvInit
 *    
 *     Functionality:
 *             This function is supplied as one of parameters during UDP P7's 
 *             task registration. SSI will invoke this function once, after
 *             it creates and attaches this TAPA Task to a system task.
 *     
 * @param[in]    
 * @return ROK     - success
 *         RFAILED - failure
 ***************************************************************************/
uint8_t pnfP7UdpActvInit()
{
  DU_LOG("DEBUG  -->  P7_UDP : Initializing ");
  memset (&pnfP7Cb, 0, sizeof(PnfP7UdpGlobalCb));
  
  //Initializing with INVALID value
  pnfP7Cb.gCntMsg =  0xFFFF; 
  protType = CM_INET_PROTO_UDP;
  return ROK;
}

/**************************************************************************
 * @brief UDP P7 server open request 
 *
 * @details
 *
 *      Function : pnfP7UdpSrvOpenReq
 * 
 *      Functionality:
 *           This function handles P7 UDP open server request.
 *           It opens udp socket to receive/send msgs.
 *     
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/

uint8_t pnfP7UdpSrvOpenReq()
{
   uint8_t ret = ROK;

   DU_LOG("INFO  -->  P7_UDP: Received open server request");

   sockType = CM_INET_DGRAM;
   if((ret = (cmInetSocket(sockType, &(pnfP7Cb.sockFd), protType))) != ROK)
   {
      DU_LOG("ERROR  -->  P7_UDP : Failed to open UDP socket");
      return RFAILED;
   }

   ret = cmInetBind(&(pnfP7Cb.sockFd), &(pnfP7Cb.srcAddr));
   if(ret != ROK)
   {
      DU_LOG("ERROR  -->  P7_UDP : Failed to bind socket");
      return RFAILED;
   }

   DU_LOG("INFO  -->  P7_UDP : Socket[%d] is open", pnfP7Cb.sockFd.fd);
   return ROK;
} /* pnfP7UdpSrvOpenReq */

/**************************************************************************
 * @brief Task Activation callback function. 
 *
 * @details
 *
 *      Function : pnfP7UdpCfgReq 
 * 
 *      Functionality: Fills the PnfP7 Database with UDP Client/Server Addresses
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t pnfP7UdpCfgReq()
{
   uint8_t ret = ROK;
   
   pnfP7Cb.srcAddr.address = CM_INET_NTOH_UINT32(pnfCb.pnfCfgParams.pnfP7UdpParams.srcIpv4P7Addr);
   pnfP7Cb.srcAddr.port = pnfCb.pnfCfgParams.pnfP7UdpParams.srcIpv4Port;
   pnfP7Cb.destAddr.address = CM_INET_NTOH_UINT32(pnfCb.pnfCfgParams.pnfP7UdpParams.destIpv4P7Addr);
   pnfP7Cb.destAddr.port = pnfCb.pnfCfgParams.pnfP7UdpParams.destIpv4Port;

   ret = pnfP7UdpSrvOpenReq();
   if(ret != ROK)
   {
       DU_LOG("ERROR  -->  P7_UDP : Transport server open request failed");
       return (ret);
   }

   return ret;
} /* pnfP7UdpCfgReq */

/*******************************************************************
 *
 * @brief Send the UDP P7 message to the destination VNF
 *
 * @details
 *
 *    Function : pnfP7UdpSendMsg 
 * 
 *    Functionality:
 *       Send the UDP P7 message to the destination DU
 *
 * @params[in]  Message Buffer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 pnfP7UdpSendMsg(Buffer *mBuf)
{
   S16            ret;
   MsgLen         txLen;
   CmInetMemInfo  info;

   info.region = PNF_APP_MEM_REG;
   info.pool = PNF_POOL;
 
   
   ret = cmInetSendMsg(&(pnfP7Cb.sockFd), &pnfP7Cb.destAddr, &info, mBuf, &txLen, CM_INET_NO_FLAG);
   if(ret != ROK && ret != RWOULDBLOCK)
   {
      DU_LOG("ERROR  -->  P7_UDP : Message send failure");
      return RFAILED;
   }
   
   DU_LOG("DEBUG  -->  P7_UDP : Message Sent");
 
   return ROK;
}
