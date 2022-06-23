/*******************************************************************************
################################################################################
#   Copyright (c) [2020-2022] [HCL Technologies Ltd.]                          #
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

#ifndef __CELL_STATE_CHANGE_STD_DEF_HPP__
#define __CELL_STATE_CHANGE_STD_DEF_HPP__


#include <iostream>

#include "VesEvent.hpp"
#include "AlarmMessages.h"
#include "Alarm.hpp"
#include "AlarmManager.hpp"
#include "Message.hpp"
#include "Notification.hpp"
#include "VesUtils.hpp"

using namespace std;

#define FAULT_SCHEMA "https://forge.3gpp.org/rep/sa5/MnS/blob/SA88-Rel16/OpenAPI/faultMnS.yaml#components/schemas/NotifyNewAlarm"
#define HREF "1"
#define URI "1"
#define NOTIFICATION_ID 1.0
#define NOTIFICATION_TYPE "notifyNewAlarm"
#define PROBABLE_CAUSE "device-issue"
#define TRND_INDICATION "MORE_SEVERE"
#define OBSRVED_MEASUREMENT "new"
#define OBSERVED_VALUE 123.2
#define NEW_ATT "new"
#define ALRAM_TYPE "COMMUNICATIONS_ALARM"
#define PROPOSED_REPAIR_ACTION "Config change"


class CellStateChangeStdDef : public Notification
{

    public:
    CellStateChangeStdDef();
    ~CellStateChangeStdDef();
    
    std::string getISOEventTime();
    std::string getEventSeverity(int severity);;
   
    protected:
    bool prepareEventFields(const Message* msg = NULL);


};


#endif

/**********************************************************************
  End of file
 **********************************************************************/
