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

/*This file contains stub for PHY to handle messages to/from MAC CL */ 

#include "envdep.h"
#include "gen.h"
#include "ssi.h"

#include "gen.x"
#include "ssi.x"

#include "rg_cl_phy.h"

EXTERN void phyToMac ARGS((U16 msgType, U32 msgLen,void *msg));

/*******************************************************************
 *
 * @brief Builds and sends config response to MAC CL
 *
 * @details
 *
 *    Function : l1BldAndSndCfgRsp
 *
 *    Functionality:
 *          - Builds and sends config response to MAC
 *
 * @params[in] Config request message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 l1BldAndSndCfgRsp(void *msg)
{
   L1L2ConfigReq *FAPIConfigReq;
   L1L2ConfigRsp *FAPIConfigRsp;
   U8 cci;

   FAPIConfigReq = (L1L2ConfigReq *)msg;
   cci = FAPIConfigReq->carrierId;
   SPutSBuf(0, 0, (Data *)msg, FAPIConfigReq->hdr.msgLen);

   if(SGetSBuf(0, 0, (Data **)&FAPIConfigRsp, sizeof(L1L2ConfigRsp)) != ROK)
   {
       printf("\nMemory allocation failed for PHY Config Response");
       RETVALUE(RFAILED);
   }
   
   FAPIConfigRsp->hdr.nMsg = 1;
   FAPIConfigRsp->hdr.msgType = MSG_TYPE_CONFIG_RSP;
   FAPIConfigRsp->hdr.msgLen = sizeof(L1L2ConfigRsp);

   FAPIConfigRsp->carrierId = cci;
   FAPIConfigRsp->status = MSG_OK;
   FAPIConfigRsp->numUnsuppTlv = 0;
   FAPIConfigRsp->unsuppTlvLst = NULLP;
   FAPIConfigRsp->numInvTlvForPhySta = 0;
   FAPIConfigRsp->phyIdleCfgTlvLst = NULLP;
   FAPIConfigRsp->phyRunCfgTlvLst = NULLP;
   FAPIConfigRsp->numMissingTlv = 0;
   FAPIConfigRsp->missingTlvLst = NULLP;

   phyToMac(MSG_TYPE_CONFIG_RSP, sizeof(L1L2ConfigRsp), FAPIConfigRsp);

   RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief Handles config request received from MAC 
 *
 * @details
 *
 *    Function : l1HndlConfigReq
 *
 *    Functionality:
 *          -Handles config request received from MAC
 *
 * @params[in]   Message length
 *               Config request message pointer
 *
 * @return void
 *
 * ****************************************************************/

void l1HndlConfigReq(U16 msgLen, void *msg)
{
    printf("\nReceived configuration request");

    /* TO DO : validate all received TLVs and send back any unsupported/missing TLV */

    if(l1BldAndSndCfgRsp(msg) != ROK)
    {
       printf("\nFailed Sending config response");
    }
}

/*******************************************************************
 *
 * @brief Receives message from MAC
 *
 * @details
 *
 *    Function : macToPhy
 *
 *    Functionality:
 *       - Receives message from MAC and calls handler
 *
 * @params[in] Message type
 *             Message length
 *             Message pointer
 *
 * @return void
 *
 * ****************************************************************/

void macToPhy(U16 msgType, U32 msgLen, void *msg)
{
   switch(msgType)
   {
      case MSG_TYPE_CONFIG_REQ:
         l1HndlConfigReq(msgLen, msg);
         break;
      default:
         printf("\nInvalid message type[%x] received at PHY", msgType);
   }

}

/**********************************************************************
         End of file
**********************************************************************/
