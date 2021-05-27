/*******************************************************************************
################################################################################
#   Copyright (c) [2020-2021] [HCL Technologies Ltd.]                          #
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

/* This file contains the C interface for ODU to start the O1 module */

#ifndef __O1_INTERFACE_H__
#define __O1_INTERFACE_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
   INACTIVE,
   ACTIVE,
   IDLE
}CellState;

typedef enum {
   DISABLED,
   ENABLED
}OpState;

typedef enum {
   LOCKED,
   UNLOCKED,
   SHUTTING_DOWN
}AdminState;

#ifndef ODU_TEST_STUB
bool updateCellOpState(uint16_t cellId, OpState opState, CellState cellState);

//Defined in odu high
bool updateAdminState(uint16_t cellId, AdminState adminState);
#endif
int start_O1_module(void);

#ifdef __cplusplus
}
#endif


#endif
