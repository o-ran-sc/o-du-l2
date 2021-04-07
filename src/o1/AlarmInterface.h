/*******************************************************************************
################################################################################
#   Copyright (c) [2020-2021]      [HCL Technologies Ltd.]                     #
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

/* This file contains C interfaces for ODU to raise and clear alarms */

#ifndef __ALARM_INTERFACE_H__
#define __ALARM_INTERFACE_H__

#include <stdint.h>
#include "AlarmMessages.h"

#define CELL_UP_ALARM_ID   1009
#define CELL_DOWN_ALARM_ID 1010
#define BUFF_SIZE 20

#ifdef __cplusplus
extern "C"
{
#endif

uint8_t raiseAlarm(AlarmRecord* alrm);
uint8_t clearAlarm(AlarmRecord* alrm);
uint8_t raiseCellAlrm(uint16_t alrmId, uint16_t cellId);
uint8_t clearCellAlrm(uint16_t alrmId);

#ifdef __cplusplus
}
#endif

#endif

/**********************************************************************
         End of file
**********************************************************************/

