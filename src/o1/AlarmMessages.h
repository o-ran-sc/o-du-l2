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

/* This file contains definitions of Alarm structure */

#ifndef __ALARM_MESSAGES_H__
#define __ALARM_MESSAGES_H__

#include <string.h>
#include "CommonMessages.h"
#define ALRM_ID_SIZE 10
#define OBJ_INST_SIZE 15
#define TEXT_SIZE 50
#define DATE_TIME_SIZE 30

typedef enum
{
   CRITICAL = 3, 
   MAJOR = 4,
   MINOR = 5,
   WARNING = 6,
   INDETERMINATE = 7,
   CLEARED = 8
}SeverityLevel;

typedef enum
{
   COMMUNICATIONS_ALARM = 2,
   QUALITY_OF_SERVICE_ALARM = 3,
   PROCESSING_ERROR_ALARM = 4,
   EQUIPMENT_ALARM = 5,
   ENVIRONMENTAL_ALARM = 6,
   INTEGRITY_VIOLATION = 7,
   OPERATIONAL_VIOLATION = 8,
   PHYSICAL_VIOLATION = 9,
   SECURITY_SERVICE_OR_MECHANISM_VIOLATION = 10,
   TIME_DOMAIN_VIOLATION = 11
}EventType;


typedef struct
{
   MsgHeader msgHeader;
   EventType eventType;
   char objectClassObjectInstance[OBJ_INST_SIZE];
   char alarmId[ALRM_ID_SIZE];
   char alarmRaiseTime[DATE_TIME_SIZE];
   char alarmChangeTime[DATE_TIME_SIZE];
   char alarmClearTime[DATE_TIME_SIZE];
   char probableCause[TEXT_SIZE];
   SeverityLevel perceivedSeverity;
   char rootCauseIndicator[TEXT_SIZE];	
   char additionalText[TEXT_SIZE];
   char additionalInfo[TEXT_SIZE];
   char specificProblem[TEXT_SIZE];
}AlarmRecord;

#endif

/**********************************************************************
         End of file
**********************************************************************/
