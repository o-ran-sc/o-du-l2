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

#ifndef __ALARM_NOTIFICATION_HPP__
#define __ALARM_NOTIFICATION_HPP__


#include <iostream>

#include "VesEvent.hpp" 
#include "AlarmMessages.h"
#include "Alarm.hpp"
#include "AlarmManager.hpp"
#include "Message.hpp"
#include "VesNotification.hpp"

using namespace std;

#define ALARM_NOTIFICATION_SCHEMA "https://forge.3gpp.org/rep/sa5/MnS/blob/SA88-Rel16/OpenAPI/faultMnS.yaml#components/schemas/NotifyNewAlarm"
#define HREF 1.0
#define URI "1"
#define NOTIFICATION_ID 1.0
#define NOTIFICATION_TYPE "notifyNewAlarm"
#define PROBABLE_CAUSE 1.0
#define TRND_INDICATION "MORE_SEVERE"
#define OBSRVED_MEASUREMENT "new"
#define OBSERVED_VALUE 123.0
#define NEW_ATT "new"
#define ADD_INFO "new"
#define ALARM_ID "1"
#define ALRAM_TYPE "COMMUNICATIONS_ALARM"
#define STND_DEFINED_FEILD_VERSION "1.0"


class AlarmNotification : public VesNotification
{
    
    public:
    // Default Constructor and Destructor
    AlarmNotification();
    ~AlarmNotification();

    bool readAlarmParams();

    protected:
    bool prepareEventFields(const Message* msg = NULL);


};


#endif 

/**********************************************************************
  End of file
 **********************************************************************/
