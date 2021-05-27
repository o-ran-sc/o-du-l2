/*******************************************************************************
################################################################################
#   Copyright (c) [2020-2021] [HCL Technologies Ltd.]                          #
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

/* This file contains C interface for ODU and stubs to get startup
   configuration
*/

#include "ConfigInterface.h"
#include "GlobalDefs.hpp"
#include "UnixSocketClient.hpp"
#include "SessionHandler.hpp"
#include "InitConfig.hpp"
#include "NrCellList.hpp"

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
 *      - Get the start up IP and port for DU
 *
 * @params[in] pointer to StartupConfig
 * @return O1::SUCCESS - success
 *         O1::FAILURE - failure
 ******************************************************************/
uint8_t getStartupConfig(StartupConfig *cfg)
{
   if ( InitConfig::instance().getCurrInterfaceConfig(*cfg) )
   {
      O1_LOG("\nO1 StartupConfig : "
             "cfg.DU_IPV4_Addr [%s]",
              cfg->DU_IPV4_Addr);
      O1_LOG("\nO1 StartupConfig : "
             "cfg.DU_Port [%d]", 
              cfg->DU_Port);
      O1_LOG("\nO1 StartupConfig : "
             "cfg.CU_IPV4_Addr [%s]", 
              cfg->CU_IPV4_Addr);
      O1_LOG("\nO1 StartupConfig : "
             "cfg.CU_Port [%d]", 
              cfg->CU_Port);
      O1_LOG("\nO1 StartupConfig : "
             "cfg.RIC_IPV4_Addr [%s]", 
              cfg->RIC_IPV4_Addr);
      O1_LOG("\nO1 StartupConfig : "
             "cfg.RIC_Port [%d]", 
              cfg->RIC_Port);
      return O1::SUCCESS;
   }
   return O1::FAILURE;
}

/*******************************************************************
 *
 * @brief Get the startup config from Netconf
 *
 * @details
 *
 *    Function : getStartupConfig
 *
 *    Functionality:
 *      - Get the start up IP and port for CU and RIC
 *
 * @params[in] pointer to StartupConfig
 * @return O1::SUCCESS - success
 *         O1::FAILURE - failure
 ******************************************************************/
uint8_t getStartupConfigForStub(StartupConfig *cfg)
{
#if 0
   UnixSocketClient uxClient(O1::ALARM_SOCK_PATH);
   O1_LOG("\nO1 CONFIG : getStartupConfig ------ \n");
   MsgHeader msg;
   msg.msgType = CONFIGURATION;
   msg.action = GET_STARTUP_CONFIG;
   if (uxClient.openSocket() == O1::FAILURE)
   {
      return O1::FAILURE;
   }
   if (uxClient.sendData(&msg,sizeof(msg)) < 0 )
   {
      uxClient.closeSocket();
      return O1::FAILURE;
   }
   if (uxClient.receiveData(cfg, sizeof(StartupConfig)) < 0)
   {
      uxClient.closeSocket();
      return O1::FAILURE;
   }
   O1_LOG("\nO1 CONFIG : ip du %s\n",cfg->DU_IPV4_Addr );
   O1_LOG("\nO1 CONFIG : ip cu %s\n",cfg->CU_IPV4_Addr );
   O1_LOG("\nO1 CONFIG : ip ric %s\n",cfg->RIC_IPV4_Addr );
   O1_LOG("\nO1 CONFIG : port cu %hu\n",cfg->CU_Port);
   O1_LOG("\nO1 CONFIG : port du %hu\n",cfg->DU_Port);
   O1_LOG("\nO1 CONFIG : port ric %hu\n",cfg->RIC_Port);

   uxClient.closeSocket();
#endif
   SessionHandler sessHdlr;
   if ( sessHdlr.init() )
   {
      if ( InitConfig::instance().getCurrInterfaceConfig(*cfg) )
      {
         O1_LOG("\nO1 StartupConfig : "
                "cfg.DU_IPV4_Addr [%s]",
                 cfg->DU_IPV4_Addr);
         O1_LOG("\nO1 StartupConfig : "
                "cfg.DU_Port [%d]", 
                 cfg->DU_Port);
         O1_LOG("\nO1 StartupConfig : "
                "cfg.CU_IPV4_Addr [%s]", 
                 cfg->CU_IPV4_Addr);
         O1_LOG("\nO1 StartupConfig : "
                "cfg.CU_Port [%d]", 
                 cfg->CU_Port);
         O1_LOG("\nO1 StartupConfig : "
                "cfg.RIC_IPV4_Addr [%s]", 
                 cfg->RIC_IPV4_Addr);
         O1_LOG("\nO1 StartupConfig : "
                "cfg.RIC_Port [%d]", 
                 cfg->RIC_Port);
        return O1::SUCCESS;
      }
   }
   return O1::FAILURE;
}

/*******************************************************************
 *
 * @brief update cell and operational state
 *
 * @details
 *
 *    Function : setCellOpState
 *
 *    Functionality:
 *      - send update of cell and operational state to it's handler
 *
 *
 * @params[in] cellId, opState, cellState
 * @return cell operational in string form
 ******************************************************************/


bool setCellOpState(uint16_t cellId, OpState opState, CellState cellState)
{
   O1_LOG("\nO1  ConfigInterface: Setting cellId = %d, opState=%d, \
cellState=%d\n", cellId, opState, cellState);
   return NrCellList::instance().setCellOpState(cellId, opState, \
                                                      cellState);
}

/**********************************************************************
         End of file
**********************************************************************/
