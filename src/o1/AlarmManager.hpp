/*******************************************************************************
################################################################################
#   Copyright (c) [2020] [HCL Technologies Ltd.]                               #
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

/* This file contains AlarmManager singleton class responsible for 
   storing and managing alarms. 
*/ 

#ifndef __ALARM_MANAGER_HPP__
#define __ALARM_MANAGER_HPP__

#include <map>
#include "Alarm.hpp"
#include "Singleton.hpp"

using std::map;


class AlarmManager : public Singleton<AlarmManager>
{

   friend Singleton<AlarmManager>;

   private:
   map<uint16_t,Alarm> mAlarmList; 	    

   protected:
   AlarmManager();    
   ~AlarmManager();

   public:
   bool raiseAlarm(const Alarm& alarm);
   bool clearAlarm(const uint16_t& alarmId);
   bool clearAlarm(const Alarm& alarm );
   const map<uint16_t, Alarm>& getAlarmList()const;

};


#endif

/**********************************************************************
         End of file
**********************************************************************/
