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

/* This file contains global definitions for O1 interface modules */

#ifndef __GLOBAL_DEFS_HPP__
#define __GLOBAL_DEFS_HPP__

#include <syslog.h>
#include <string>

using std::string;

#define O1_LOG(...) ( {\
		printf(__VA_ARGS__);\
		syslog(LOG_DEBUG,__VA_ARGS__);\
		} )

#define ALARM_MODULE_NAME_3GPP "_3gpp-common-fm"
#define ALARM_MODULE_PATH_3GPP "/_3gpp-common-fm:AlarmListGrp"
#define ALARM_MODULE_NAME_ORAN "o-ran-sc-odu-alarm-v1"
#define ALARM_MODULE_PATH_ORAN "/o-ran-sc-odu-alarm-v1:odu"
#define CELL_STATE_MODULE_NAME "o-ran-sc-du-hello-world"
#define CELL_STATE_MODULE_PATH "/o-ran-sc-du-hello-world:network-function"
#define IETF_NACM_MODULE_NAME "ietf-netconf-acm"
#define IETF_NACM_MODULE_PATH "/ietf-netconf-acm:nacm"
#define MANAGED_ELEMENT_MODULE_NAME "_3gpp-common-managed-element"
#define MANAGED_ELEMENT_MODULE_PATH "/_3gpp-common-managed-element:ManagedElement"
#define GNB_DU_FUNTION_MODULE_NAME "_3gpp-nr-nrm-gnbdufunction"
#define NR_CELL_DU_MODULE_NAME "_3gpp-nr-nrm-nrcelldu"
#define RRMPOLICY_MODULE_NAME "_3gpp-nr-nrm-rrmpolicy"
#define RRMPOLICY_MODULE_PATH "/_3gpp-nr-nrm-rrmpolicy:RRMPolicyRatio"
#define MAX_ALARM_ID_LEN 10

#define ROK 0

class O1
{
   public:
   static const int   SUCCESS;
   static const int   FAILURE;
   static const string ALARM_SOCK_PATH;
   static const int CPU_CORE;
};

#endif

/**********************************************************************
         End of file
**********************************************************************/
