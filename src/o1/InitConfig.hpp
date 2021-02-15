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

/* This file contains InitConfig class header*/

#ifndef __INIT_CONFIG_HPP__
#define __INIT_CONFIG_HPP__

#include <utility>
#include <string>
#include <iostream>
#include "sysrepo-cpp/Session.hpp"
#include <map>
#include "Singleton.hpp"
#include "Config.h"

#include "GlobalDefs.hpp"
#define IP_ADDRESS "interface-address" 
#define PORT "port"
#define INTERFACE_MODULE_NAME_ORAN "/o-ran-sc-odu-interface-v1:odu"
#define MAX_XPATH 100
#define NETCONF_STARTUP_CFG "/etc/netconf_startup.cfg"

#define DEFAULT_DU_IPV4_ADDR "192.168.130.81"
#define DEFAULT_DU_PORT 38472

#define DEFAULT_CU_IPV4_ADDR "192.168.130.82"
#define DEFAULT_CU_PORT 38472

#define DEFAULT_RIC_IPV4_ADDR "192.168.130.80"
#define DEFAULT_RIC_PORT 36422

enum class Interface 
{ ODU,
  OCU,
  RIC
};


class InitConfig : public Singleton<InitConfig>
{

   friend Singleton<InitConfig>;

   public:
      
     //member variable
      typedef std::pair<std::string, uint16_t> Address;
      typedef std::map<Interface, Address> InterfaceMap;
     
      InitConfig();
      ~InitConfig();
      bool init(sysrepo::S_Session sess);
      bool getCurrInterfaceConfig(StartupConfig & cfg);

   private:

      InterfaceMap mInterfaceList;
      char xpath[MAX_XPATH];
      sysrepo::S_Session mSess;
      //string mVal;
      /* function to get the data of Interfaces param*/
      InterfaceMap getInterfaceConfig(sysrepo::S_Session sess);
      Address getInterfaceData(sysrepo::S_Session sess, Interface inf);
      char * getInterfaceXpath( std::string sInf, std::string param);
      std::string getData(sysrepo::S_Session sess,char* xpath);
      std::string interfaceToString(Interface inf);
      bool printInterfaceConfig();
      bool writeInterfaceConfig();

};

#endif

/**********************************************************************
         End of file
**********************************************************************/
