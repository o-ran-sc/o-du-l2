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

#ifndef __CELL_STATE_CHANGE_HPP__
#define __CELL_STATE_CHANGE_HPP__


#include <iostream>

#include "VesEvent.hpp" 
#include "AlarmMessages.h"
#include "Alarm.hpp"
#include "AlarmManager.hpp"
#include "Message.hpp"
#include "Notification.hpp"

using namespace std;

//macros
#define FAULT_FIELDS_VERSION "4.0"
#define ALARM_INTERFACE_A "Slot-0-CELL-1"
#define EQUIP_TYPE "O-RAN-DU"
#define VENDOR "Melacon"
#define MODEL "ODU Device"

class CellStateChange : public Notification
{
    
    public:
    CellStateChange();
    ~CellStateChange();

    std::string getISOEventTime();
    std::string getEventSeverity(int severity);

    protected:
    bool prepareEventFields(const Message* msg = NULL);

};


#endif 

/**********************************************************************
  End of file
 **********************************************************************/
