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


/*******************************************************************
 *
 * @brief Read the data from the connected client application
 *
 * @details
 *
 *    Function : update
 *
 *    Functionality:
 *      - Reads the data from the connected client application
 *
 *
 * @params[in] char*
 * @return void
 ******************************************************************/

void AlarmManager::update(char* recvBuf)
{
   O1_LOG("\nO1 AlarmManager update received");
   AlarmRecord *alrmRec = NULL;
   Alarm alrm;
   MsgHeader *msgHdr = (MsgHeader*)recvBuf;
   O1_LOG("\nO1 AlarmManager: MsgType %d",msgHdr->msgType);


      if ( msgHdr->msgType == ALARM ){
         uint16_t alrmId;
         alrmRec = (AlarmRecord*) recvBuf;
         O1_LOG("\nO1 AlarmManager:Alarm info\n"
                   "                Action           %d\n"
                   "                Alarm ID         %s\n"
                   "                Severity         %d\n"
                   "                Additional Text  %s\n"
                   "                Specific Problem %s\n"
                   "                Additional Info  %s\n"
                   "                Alarm Raise Time %s\n",
                   alrmRec->msgHeader.action,
                   alrmRec->alarmId,
                   alrmRec->perceivedSeverity,
                   alrmRec->additionalText,
                   alrmRec->specificProblem,
                   alrmRec->additionalInfo,
                   alrmRec->alarmRaiseTime
                );

         /*Fill the alarm structure */
         sscanf(alrmRec->alarmId,"%hu",&alrmId);
         alrm.setAlarmId(alrmId);
         alrm.setPerceivedSeverity(alrmRec->perceivedSeverity);
         alrm.setAdditionalText(alrmRec->additionalText);
         alrm.setEventType(alrmRec->eventType);
         alrm.setSpecificProblem(alrmRec->specificProblem);
         alrm.setAdditionalInfo(alrmRec->additionalInfo);
      }

      switch(msgHdr->action)
      {
         case RAISE_ALARM:

            if(raiseAlarm(alrm))
            {
                VesEventHandler vesEventHandler;
                if (!vesEventHandler.prepare(VesEventType::FAULT_NOTIFICATION, &alrm))
                    return ;//O1::FAILURE;

                O1_LOG("\nO1 AlarmManager : Sending Alarm Data");
                if ( !vesEventHandler.send() )
                    return ;//O1::FAILURE;

   	            O1_LOG("\nO1 AlarmManager : "
                               "Alarm raised for alarm Id %s",
                                alrmRec->alarmId);
            }

            else
            {
                O1_LOG("\nO1 AlarmManager : "
                        "Error in raising alarm for alrm Id %s",
                        alrmRec->alarmId);
            }
            break;

         case CLEAR_ALARM: 
                     if(clearAlarm(alrm))
                     {
                        O1_LOG("\nO1 AlarmManager : "
                               "Alarm cleared for alarm Id %s",
                                alrmRec->alarmId);

                     }
                     else
                     {
                        O1_LOG("\nO1 AlarmManager : "
                               "Error in clearing alarm for alarm Id %s",
                                alrmRec->alarmId);
                     }
                     break;
         default:
                     O1_LOG("\nO1 AlarmManager : No action performed");
                     break;
      }

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
