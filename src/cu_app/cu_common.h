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

/* This file contains common defines for CU Config Params */

#ifndef __CU_COMMON_H__
#define __CU_COMMON_H__

#include "envdep.h"

#define MAX_IPV4_LEN 16
#define MAX_IPV6_LEN 40
#define CU_DU_NAME_LEN_MAX 150      /* Max length of CU/DU name string */

typedef struct ipAddr
{
 Bool ipV4Pres;
 char ipV4Addr[MAX_IPV4_LEN];
 Bool ipV6Pres;
 char ipV6Addr[MAX_IPV6_LEN];
}SctpIpAddr;

typedef struct sctpParams
{
   SctpIpAddr  duIpAddr;
   U16         duPort;
   SctpIpAddr  cuIpAddr;
   U16         cuPort;
}SctpParams;

typedef struct cuCfgParams
{
   U32              cuId;
   char             cuName[CU_DU_NAME_LEN_MAX];
   SctpParams       sctpParams;
}CuCfgParams;

extern CuCfgParams cuCfgParams;

#endif
/**********************************************************************
         End of file
**********************************************************************/
