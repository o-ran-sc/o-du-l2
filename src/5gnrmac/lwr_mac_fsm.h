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
#ifndef _LWR_MAC_FSM_H_
#define _LWR_MAC_FSM_H_
#define FAPI_UINT_8   1
#define FAPI_UINT_16  2
#define FAPI_UINT_32  4
#define INVALID_VALUE -1

#include "lwr_mac.h"
#include "rg_cl_phy.h"
#include "fapi.h"

S16 lwr_mac_handleInvalidEvt(void *msg);
S16 lwr_mac_handleParamReqEvt(void *msg);
S16 lwr_mac_handleParamRspEvt(fapi_param_resp_t *);
S16 lwr_mac_handleConfigReqEvt(void *msg);
S16 lwr_mac_handleConfigRspEvt(fapi_config_resp_t *);
S16 lwr_mac_handleStartReqEvt(void *msg);
S16 lwr_mac_handleStopReqEvt(void *msg);
S16 lwr_mac_handleSlotIndReqEvt(fapi_slot_ind_t *);
S16 sendToLowerMac(U16, U32, void *);

typedef S16 (*lwrMacFsmHdlr)(void *);
#endif

/**********************************************************************
         End of file
**********************************************************************/

