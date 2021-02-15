/*******************************************************************************
################################################################################
#   Copyright (c) [2020] [HCL Technologies Ltd.]                               #
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

/* This file contains definitions of startup configuration structure */

#include "Config.h"
#include "ssi.h"
#include "GlobalDefs.h"
#include "TcpClient.h"

StartupConfig g_cfg;

/*******************************************************************
 *
 * @brief Get the startup config from Netconf
 *
 * @details
 *
 *    Function : getStartupConfig
 *
 *    Functionality:
 *      - Get the start up IP and port for DU,CU and RIC
 *
 * @params[in] pointer to StartupConfig
 * @return ROK     - success
 *         RFAILED - failure
 ******************************************************************/
uint8_t getStartupConfig(StartupConfig *cfg)
{
   O1_LOG("\nCONFIG : getStartupConfig ------ \n");
   MsgHeader msg;
   msg.msgType = CONFIGURATION;
   msg.action = GET_STARTUP_CONFIG;
   if (openSocket(TCP_SERVER_IP,TCP_PORT) == RFAILED)
   {
      return RFAILED;
   }
   if (sendData(&msg,sizeof(msg)) < 0 )
   {
      closeSocket();
      return RFAILED;
   }
   if (receiveData(cfg, sizeof(StartupConfig)) < 0)
   {
      closeSocket();
      return RFAILED;
   }
   O1_LOG("\nCONFIG : ip du %s\n",cfg->DU_IPV4_Addr );
   O1_LOG("\nCONFIG : ip cu %s\n",cfg->CU_IPV4_Addr );
   O1_LOG("\nCONFIG : ip ric %s\n",cfg->RIC_IPV4_Addr );
   O1_LOG("\nCONFIG : port cu %hu\n",cfg->CU_Port);
   O1_LOG("\nCONFIG : port du %hu\n",cfg->DU_Port);
   O1_LOG("\nCONFIG : port ric %hu\n",cfg->RIC_Port);

   closeSocket();
   return ROK;
}


/**********************************************************************
         End of file
**********************************************************************/
