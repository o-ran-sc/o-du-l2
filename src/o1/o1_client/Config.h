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

/* This file contains definitions of startup configuration structure */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>
#include <CommonMessages.h>

#define IPV4_LEN 16
#define PORT_LEN 10

typedef struct
{
   char DU_IPV4_Addr[IPV4_LEN];
   char CU_IPV4_Addr[IPV4_LEN];
   char RIC_IPV4_Addr[IPV4_LEN];
   uint16_t CU_Port;
   uint16_t DU_Port;
   uint16_t RIC_Port;
}StartupConfig;

uint8_t getStartupConfig();

#endif

/**********************************************************************
         End of file
**********************************************************************/
