/*******************************************************************************
################################################################################
#   Copyright (c) [2021] [HCL Technologis]                                     #
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

#include<iostream>
#include "../GlobalDefs.hpp"

//config file path
#define NETCONF_CONFIG "config/netconfConfig.json"
#define SMO_CONFIG "config/smoConfig.json"

//Common Header Macros
#define EVENT_ID "PRTNILACQ01M017DEV01_7200DEV"
#define EVENT_TYPE_5G "EventType5G"
#define NAMING_CODE "7DEV"
#define VENDOR_NAME "VENDORB"
#define REPORTING_ENTITY_NAME "ODU"
#define PRIORITY "Low"
#define SOURCE_NAME "ODU-High"
#define TIME_ZONE_OFFSET "+00:00"
#define VERSION "4.0.1"
#define VES_EVENT_LISTENER_VERSION "7.0.1"

//PNF Registration Macros
#define PNF_REGISTRATION_VERSION "2.1"
#define MAC_ADDR "02:42:f7:d4:62:ce"
#define MODEL_NUMBER "7200DEV"
#define NETCONF_V4_SERVER_IP "127.0.0.1"
#define NETCONF_V6_SERVER_IP "0:0:0:0:0:ffff:a0a:013"
#define SERIAL_NUMBER "VENDORB-7DEV-192.168.56.101-7200DEV"
#define SOFTWARE_VERSION "2.3.5"
#define UNIT_FAMILY "VENDORB-7DEV"
#define UNIT_TYPE "7DEV"
#define VENDER_NAME "VENDORB"

//Additional fields of PNF 

#define OAM_PORT "830"
#define OAM_PROTOCOL "SSH"
#define USERNAME "netconf"
#define PASSWORD "netconf"
#define RECONNECT_ON_SCHEMA_CHANGE "false"
#define SLEEP_FACTOR "1.5"
#define TCP_ONLY "false"
#define CONNECTION_TIMEOUT "20000"
#define MAX_CONNECTION_ATTEMPTS "100"
#define BETWEEN_ATTEMPTS_TIMEOUT "2000"
#define KEEPALIVE_DELAY "120"


enum class VesEventType
{ 
   pnfRegistration,
   alarmNotification,
   pmNotification,
   none
};

#endif
/**********************************************************************
  End of file
 **********************************************************************/
