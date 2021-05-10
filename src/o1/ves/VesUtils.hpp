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

/* This file contains message handling functionality for DU APP */

#ifndef __VES_UTILS_HPP__
#define __VES_UTILS_HPP__

#include <iostream>
#include "GlobalDefs.hpp"

//config file path
#define NETCONF_CONFIG "config/netconfConfig.json"
#define VES_CONFIG "config/vesConfig.json"

//Common Header Macros

//Event Type
#define EVENT_TYPE_5G "EventType5G"
#define EVENT_TYPE_ORAN_COMPONENET "O_RAN_COMPONENT"
#define EVENT_TYPE_CONTROLLER "Controller"

//Priority
#define PRIORITY_LOW "Low"
#define PRIORITY_HIGH "High"

//Source Name
#define ODU_HIGH "ODU-High"

//Naming Code 
#define NAMING_CODE_ODU "7odu"

//Timezone Offset
#define TIME_ZONE_00_00 "+00:00"

//version
#define VERSION_4_0_1 "4.0.1"

//Ves Event Listener Version
#define VES_EVENT_LISTENER_7_2_1 "7.2.1"



//PNF Registration Macros
#define PNF_REGISTRATION_VERSION "2.1"
#define MAC_ADDR "02:42:f7:d4:62:ce"
#define MODEL_NUMBER "7200DEV"
#define SERIAL_NUMBER "VENDORB-7DEV-192.168.56.101-7200DEV"
#define SOFTWARE_VERSION "2.3.5"
#define UNIT_FAMILY "VENDORB-7DEV"
#define UNIT_TYPE "7DEV"
#define VENDER_NAME "VENDORB"

//Additional fields of PNF

#define NETCONF_DEFAULT_PORT "830"
#define NETCONF_DEFAULT_PROTOCOL "SSH"
#define RECONNECT_ON_SCHEMA_CHANGE "false"
#define SLEEP_FACTOR "1.5"
#define TCP_ONLY "false"
#define CONNECTION_TIMEOUT "20000"
#define MAX_CONNECTION_ATTEMPTS "100"
#define BETWEEN_ATTEMPTS_TIMEOUT "2000"
#define KEEPALIVE_DELAY "120"

enum class VesEventType
{ 
   PNF_REGISTRATION,
   FAULT_NOTIFICATION,
   PM_NOTIFICATION,
   HEARTBEAT
};


#endif
/**********************************************************************
  End of file
 **********************************************************************/
