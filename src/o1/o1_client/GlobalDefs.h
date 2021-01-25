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

/* Class for global defines and constants for O1 interface */

#ifndef __GLOBAL_DEFS_H__
#define __GLOBAL_DEFS_H__

#include <syslog.h>

#define O1_LOG(...) ({\
		printf(__VA_ARGS__);\
		syslog(LOG_DEBUG,__VA_ARGS__);\
		})

#define TCP_PORT           8282
#define TCP_SERVER_IP      "127.0.0.1"
#define CELL_UP_ALARM_ID   1009
#define CELL_DOWN_ALARM_ID 1010

#endif

/**********************************************************************
         End of file
**********************************************************************/
