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

#define O1_LOG(...) ( {\
		printf(__VA_ARGS__);\
		syslog(LOG_DEBUG,__VA_ARGS__);\
		} )

#define ALARM_MODULE_NAME_3GPP "_3gpp-common-fm"
#define ALARM_MODULE_PATH_3GPP "/_3gpp-common-fm:AlarmListGrp"
#define ALARM_MODULE_NAME_ORAN "o-ran-sc-odu-alarm-v1"
#define ALARM_MODULE_PATH_ORAN "/o-ran-sc-odu-alarm-v1:odu"

#define MAX_ALARM_ID_LEN 10

class O1 
{
   public:
   static const short TCP_PORT;
   static const int   SUCCESS;
   static const int   FAILURE;
};

#endif

/**********************************************************************
         End of file
**********************************************************************/
