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

/* This file contains system logging functionality for DU */

#ifndef __DU_LOGS_H__
#define __DU_LOGS_H__

#include<syslog.h>

char* getFormattedTime(void);

/*MACROS*/
#ifndef LOG_TIMESTAMP
#define DU_LOG(...) ({\
		printf(__VA_ARGS__);\
		syslog(LOG_DEBUG,__VA_ARGS__);\
		})
#else
#define __LOG__(format,...) ({\
		printf(" %s " format "\n", getFormattedTime(), ## __VA_ARGS__);\
		})

#define DU_LOG(format,...) __LOG__(format, ## __VA_ARGS__)
#endif

#endif
			
