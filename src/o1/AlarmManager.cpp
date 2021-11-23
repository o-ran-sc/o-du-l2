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

/* This file contains AlarmManager singleton class which is responsible for 
   storing and managing alarms. 
*/ 

#include <iostream>
#include "AlarmManager.hpp"

using std::pair;

/* Default constructor */
AlarmManager::AlarmManager()
{

}

/* Destructor */
AlarmManager::~AlarmManager()
{

}



/********************************************************************** 
   Description : Raise an Alarm and store it in alarm list
   Params[In]  : Alarm
   Return      : true  - insert in map successful
                 false - insert in map unsuccessful 
**********************************************************************/
bool AlarmManager::raiseAlarm(const Alarm& alarm)
{

   pair<map<uint16_t,Alarm>::iterator,bool> ret;
   ret = mAlarmList.insert(pair<uint16_t,Alarm>(alarm.getAlarmId(),alarm));
   return ret.second;

   
}



/********************************************************************** 
   Description : Clear an Alarm and delete it from alarm list 
   Params[In]  : Alarm instance
   Return      : true  - delete successful
                 false - delete unsuccessful 
**********************************************************************/
bool AlarmManager::clearAlarm(const Alarm& alarm)
{
   map<uint16_t,Alarm>::iterator it;
   bool ret = false;
   it = mAlarmList.find(alarm.getAlarmId());
   if( it != mAlarmList.end() )
   {
      mAlarmList.erase(it);
      ret = true;
   }    
   return ret;
}


/********************************************************************** 
   Description : Clear an Alarm and delete it from alarm list with
                 alarmId 
   Params[In]  : Alarm Id
   Return      : true  - delete successful
                 false - delete unsuccessful 
**********************************************************************/
bool AlarmManager::clearAlarm(const uint16_t& alarmId)
{
   return (mAlarmList.erase(alarmId) > 0);
}


/********************************************************************** 
   Description : Return the list of active alarms
   Params[In]  : None
   Return      : constant reference to the map<int, Alarm> 
**********************************************************************/
const map<uint16_t, Alarm>& AlarmManager::getAlarmList()const
{
   return mAlarmList;
}


/**********************************************************************
         End of file
**********************************************************************/
