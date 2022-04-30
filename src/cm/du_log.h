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
//Regular text
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"
#define RESET "\e[0m"
/*MACROS*/
#define DU_LOG(...) ({\
		})
#define HQ_LOG(dir,pid,...) ({\
		if(0==dir){\
		printf(YEL);\
		printf(__VA_ARGS__);\
		printf(RESET);\
		}\
		else if (1==dir)\
		{\
		printf(BLU);\
		printf(__VA_ARGS__);\
		printf(RESET);\
		}\
		else\
		{\
		printf(WHT);\
		printf(__VA_ARGS__);\
		printf(RESET);\
		}\
		syslog(LOG_DEBUG,__VA_ARGS__);\
		})
#endif

