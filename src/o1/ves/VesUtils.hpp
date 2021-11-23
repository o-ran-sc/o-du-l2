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
#define OAM_VES_CONFIG "config/oamVesConfig.json"
#define SMO_VES_CONFIG "config/smoVesConfig.json"

//Common Header Macros

//Event Type
#define EVENT_TYPE_5G "EventType5G"
#define EVENT_TYPE_ORAN_COMPONENT "O_RAN_COMPONENT"
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
#define VERSION_4_1 "4.1"

//Ves Event Listener Version
#define VES_EVENT_LISTENER_7_2_1 "7.2.1"

//PNF Registration Macros
#define PNF_REGISTRATION_VERSION_2_1 "2.1"
#define SOFTWARE_VERSION_2_3_5 "2.3.5"
#define UNIT_TYPE_7DEV "7DEV"
#define VENDER_NAME_VENDORB "VENDORB"
#define MODEL_NUMBER_007_DEV "007DEV"


//Additional fields macros for PNF Registration

#define NETCONF_DEFAULT_PORT "830"
#define NETCONF_PROTOCOL_SSH "SSH"
#define RECONNECT_ON_SCHEMA_CHANGE_FALSE "false"
#define SLEEP_FACTOR_1_5 "1.5"
#define TCP_ONLY_FALSE "false"
#define CONNECTION_TIMEOUT_20000 "20000"
#define MAX_CONNECTION_ATTEMPTS_100 "100"
#define BETWEEN_ATTEMPTS_TIMEOUT_2000 "2000"
#define KEEPALIVE_DELAY_120 "120"

// PM_SLICE Macros
#define PM_EVENT_ID "_1634181300_PM1min"
#define PM_REPORTING_ENTITY "ORAN-DEV"
#define EVENT_TYPE_ORAN_COMPONENT_PM "O_RAN_COMPONENT_PM1min"

enum class VesEventType
{ 
   PNF_REGISTRATION,
   FAULT_NOTIFICATION,
   PM_NOTIFICATION,
   HEARTBEAT,
   PM_SLICE
};

enum class MessageType
{
   ALARM,
   PERF_DATA,
};


#endif

/**********************************************************************
  End of file
 **********************************************************************/
