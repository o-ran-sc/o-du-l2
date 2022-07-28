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

/* This file sends/receives FAPI messages to/from UE Sim/PHY */

#include "common_def.h"
#include "fapi_cl.h"
#include "fapi_cl_utils.h"

/*******************************************************************
 *
 * @brief Receives message from UE simulator
 *
 * @details
 *
 *    Function :  recvMsgFromUeSim
 *
 *    Functionality:
 *       - Receives message from UE simulator
 *
 * @return void
 *
 * ****************************************************************/
void recvMsgFromUeSim()
{
   struct sockaddr remSockAddr;
   uint32_t remAddrLen = 0;
   ssize_t recvLen = 0;
   uint8_t buf[MAX_BUF_LEN];

   while(true)
   {
      recvLen = 0;
      remAddrLen = sizeof(remSockAddr);
      memset((uint8_t*)&remSockAddr, 0, sizeof(remSockAddr));

      /*read the data from socket*/
      if((recvLen = recvfrom(clCb.socket.sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&remSockAddr, \
                  (socklen_t *)&remAddrLen)) == -1)
      {
         DU_LOG("\nERROR  -->  FAPI CL: recvfrom() error\n");
         continue;
      }
      else if(recvLen != 0)
      {
         fapiClProcessUeSim2MacFapiMsg(buf);
         memset(buf, 0, MAX_BUF_LEN * sizeof(uint8_t));
      }
   }
}

/*******************************************************************
 *
 * @brief Start receiver thread
 *
 * @details
 *
 *    Function :  sendStartReceiverReq
 *
 *    Functionality:
 *       - Start a thread to receive messages from UE simulator
 *
 * @return void
 *
 * ****************************************************************/
void sendStartReceiverReq()
{
    Pst pst;
    Buffer *mBuf = NULLP;
 
    DU_LOG("\nINFO  --> FAPI_CL: Sending start receiving event to self");
    memset(&pst, 0, sizeof(Pst));
    FILL_PST_CL_TO_CL(pst, FAPI_CL_INST_0, FAPI_CL_INST_0, EVT_FAPI_CL_START_RECEIVER);
    ODU_GET_MSG_BUF(pst.region, pst.pool, &mBuf);
    ODU_POST_TASK(&pst, mBuf);
}

/*******************************************************************
 *
 * @brief Configure FAPI CL
 *
 * @details
 *
 *    Function :  configureFapiCl
 *
 *    Functionality:
 *       - Configure FAPI CL
 *
 * @params[in] void
 *
 * @return void
 *
 * ****************************************************************/
void configureFapiCl()
{
   if(clCb.configured == false)
   {
      if((clCb.socket.sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
         DU_LOG("\nERROR  -->  FAPI CL: Socket creation [Failure]");
      else
         DU_LOG("\nINFO  -->  FAPI CL: Socket creation [Success]");

      memset((uint8_t *)&clCb.socket.srcAddr, 0, sizeof(clCb.socket.srcAddr));
      clCb.socket.srcAddr.sin_family      = AF_INET;
      clCb.socket.srcAddr.sin_port        = htons(SELF_PORT);
      clCb.socket.srcAddr.sin_addr.s_addr = inet_addr((const char *)SELF_IP);
      if(bind(clCb.socket.sockfd, (struct sockaddr *)&clCb.socket.srcAddr, sizeof(struct sockaddr)) == -1)
         DU_LOG("\nERROR  -->  FAPI CL: Socket bind [Failure]");
      else
         DU_LOG("\nINFO  -->  FAPI CL: Socket bind [Success]");

      memset((uint8_t *)&clCb.socket.dstAddr, 0, sizeof(clCb.socket.dstAddr));
      clCb.socket.dstAddr.sin_family      = AF_INET;
      clCb.socket.dstAddr.sin_port        = htons(UE_SIM_PORT);
      clCb.socket.dstAddr.sin_addr.s_addr = inet_addr((const char *)UE_SIM_IP);

      clCb.configured = true;
      sendStartReceiverReq();
      DU_LOG("\nINFO  -->  FAPI_CL: Configuration complete");
   }
}

/*******************************************************************
 *
 * @brief Sends message to UE simulator
 *
 * @details
 *
 *    Function :  sendMsgToUeSim
 *
 *    Functionality:
 *       - Sends message to UE simulator
 *
 * @params[in] Event
 *             Message pointer
 *             Message length
 *
 * @return void
 *
 * ****************************************************************/
void sendMsgToUeSim(uint8_t event, Data *msg, uint16_t msgLen)
{
   uint8_t  ret = ROK;
   Buffer   *mBuf = NULLP;

   ODU_GET_MSG_BUF(DFLT_REGION, DFLT_POOL, &mBuf);
   if(mBuf == NULLP)
   {
      DU_LOG("\nERROR  -->  FAPI CL: Failure in allocating memory for mBuf");
      return;
   }

   ret = ODU_ADD_POST_MSG((Data )event, mBuf);
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  FAPI CL: Failed to pack event into buffer"); 
   }

   ret = ODU_ADD_POST_MSG_MULT(msg, msgLen, mBuf);
   if(ret == ROK)
   {
      uint8_t  sendBuf[MAX_BUF_LEN];
      MsgLen   cpMsgLen;            /* copied message length */
      MsgLen   bufLen;              /* send buffer length */

      ret = ODU_GET_MSG_LEN(mBuf, &bufLen);
      if (ret != ROK)
      {
         DU_LOG("ERROR  -->  FAPI CL: Failed to find buffer length");
         return;
      }

      if (bufLen >= MAX_BUF_LEN)
      {
         DU_LOG("\nERROR  -->  FAPI CL: Length Buffer to be copied [%d] bytes is more than MAX buffer length supported [%d] bytes",\
         bufLen, MAX_BUF_LEN);
         return;
      }

      ret = ODU_COPY_MSG_TO_FIX_BUF(mBuf, 0, bufLen, sendBuf, &cpMsgLen);
      if ((ret != ROK) || (cpMsgLen != bufLen))
      {
         DU_LOG("\nERROR  -->  FAPI CL: Failed to copy buffer into fixed message");
         return;
      }

      if(sendto(clCb.socket.sockfd, sendBuf, bufLen, 0, (struct sockaddr *)&clCb.socket.dstAddr, sizeof(clCb.socket.dstAddr)) == -1)
         DU_LOG("\nERROR  -->  FAPI CL: Sending message over UDP socket [Failure]");
      else
         DU_LOG("\nDEBUG  -->  FAPI CL: Sending message over UDP socket [Success]");

      ODU_PUT_MSG_BUF(mBuf);
   }
}

/**********************************************************************
  End of file
 **********************************************************************/
