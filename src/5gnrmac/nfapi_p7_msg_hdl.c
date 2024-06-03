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

/* This file is the entry point for P7 Msg Handling */
/*Reference SCF225 version: 225.2.1 */

#include "common_def.h"
#include "nfapi_interface.h"
#include "nfapi_common.h"
#include "nfapi_vnf_fsm.h"

extern NfapiVnfDb vnfDb;

/*Following parameters can be referred to Table 4-1 Dl Node Sync Parameters*/
void buildAndSendDlNodeSync()
{
   uint32_t t1 = 0; /*Offset from VNF SFN/Slot 0/0 to the DL Node Sync Tx*/
   int32_t  delta_sfn_slot = 0;

   
     
}


