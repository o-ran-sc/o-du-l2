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

/* This file contains APIs to send/receive messages from PHY */

#include "stdio.h"
#include "envdep.h"
#include "ssi.h"

#include "rg_cl_phy.h"
#include "fapi.h"


EXTERN S16 rgClHndlCfgReq ARGS((void *msg));
EXTERN void macToPhy ARGS((U16 msgType, U32 msgLen, void *msg));
EXTERN void processRequest ARGS((U16 msgType, U32 msgLen, void *msg));
/******************************************************************
 *
 * @brief Receives message to PHY
 *
 * @details
 *
 *    Function : phyToMac
 *
 *    Functionality:
 *         -Receives Msg from PHY
 *
 * @params[in] Message Pointer
 *
 * @return void
 *
 ******************************************************************/

void phyToMac(U16 msgType, U32 msgLen,void *msg)
{
   switch(msgType)
   {
      case MSG_TYPE_CONFIG_RSP:
         rgClHndlCfgReq(msg);
         break;

      default:
         printf("\nInvalid message type[%x] from PHY", msgType);
   }
}

/*******************************************************************
 *
 * @brief Sends message to PHY
 *
 * @details
 *
 *    Function : sendToPhy
 *
 *    Functionality:
 *         -Sends message to PHY
 *
 * @params[in] Message Type
 *             Message Length
 *             Messaga Pointer
 *
 * @return void
 *
******************************************************************/

PUBLIC S16 sendToPhy(U16 msgType, U32 msgLen, void *msg)
{
#ifdef WLS_MEM
   S8 ret;
   void *pMsg;

   pMsg = (void *)WLS_VA2PA(mtGetWlsHdl(), msg);
   ret = WLS_put(mtGetWlsHdl(), (PTR)pMsg, msgLen, msgType, 0);

   if(ret != 0)
   {
      printf("\nFailure in sending message to PHY");
      RETVALUE(RFAILED);
   }
#else

   processRequest(msgType, msgLen, msg);

#endif
}

/**********************************************************************
         End of file
**********************************************************************/
