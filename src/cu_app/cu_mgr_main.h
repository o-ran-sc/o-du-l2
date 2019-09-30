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

#ifndef __CU_MGR_MAIN_H__
#define __CU_MGR_MAIN_H__


#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "stdbool.h"
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */

#include "cu_common.h"

#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm_lib.x"

CuCfgParams cuCfgParams; //global variable to hold all configs

void readCuCfg();
void cuAppInmsgHdlr(char *recvBuf,U8 len);

#endif

/**********************************************************************
         End of file
**********************************************************************/
