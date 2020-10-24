/*******************************************************************************
################################################################################
#   Copyright (c) [2020]      [HCL Technologies Ltd.]                          #
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

/* This file contains interfaces to raise and clear alarms */

#ifndef __ALARM_INTERFACE_H__
#define __ALARM_INTERFACE_H__

#include "Alarm.h"
#include "ssi.h"
#include "GlobalDefs.h"

#define BUFF_SIZE 20

int raiseAlarm(AlarmRecord* alrm);
int clearAlarm(AlarmRecord* alrm);
int raiseCellAlrm(uint16_t alrmId, uint16_t cellId);
int clearCellAlrm(uint16_t alrmId);

#endif

/**********************************************************************
         End of file
**********************************************************************/

