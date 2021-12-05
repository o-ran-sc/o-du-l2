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

/* This file contains Alarm class defining Netconf Yang based alarm fields */

#ifndef __ALARM_HPP__
#define __ALARM_HPP__

#include <string>
#include <string.h>
#include "AlarmMessages.h"
#include "Message.hpp"

using std::string; 


class Alarm : public Message
{
   private:
   EventType mEventType;
   string mObjectClassObjectInstance;
   uint16_t mAlarmId;
   string mAlarmRaiseTime;
   string mAlarmChangeTime;
   string mAlarmClearTime;
   string mProbableCause;
   SeverityLevel mPerceivedSeverity;
   string mRootCauseIndicator;	
   string mAdditionalText;
   string mAdditionalInfo;
   string mSpecificProblem;

   public:
   Alarm() {}
   ~Alarm(){}
   /* Getter functions */
   inline const EventType& getEventType()const 
   { 
      return mEventType; 
   }
   inline const string& getObjectClassObjectInstance()const 
   { 
      return mObjectClassObjectInstance; 
   }
   inline const uint16_t& getAlarmId()const 
   { 
      return mAlarmId; 
   }
   inline const string& getAlarmRaiseTime()const 
   { 
      return mAlarmRaiseTime; 
   }
   inline const string& getAlarmChangeTime()const 
   { 
      return mAlarmChangeTime; 
   }
   inline const string& getAlarmClearTime()const 
   { 
      return mAlarmClearTime; 
   }
   inline const string& getProbableCause()const 
   { 
      return mProbableCause; 
   }
   inline const SeverityLevel getPerceivedSeverity()const 
   { 
      return mPerceivedSeverity; 
   }
   inline const string getRootCauseIndicator()const 
   { 
      return mRootCauseIndicator; 
   }
   inline const string getAdditionalText()const 
   { 
      return mAdditionalText; 
   }
   inline const string getAdditionalInfo()const 
   { 
      return mAdditionalInfo; 
   }
   inline const string getSpecificProblem()const 
   { 
      return mSpecificProblem; 
   }
   
   /* Setter functions */
   inline void setEventType(const EventType& eventType) 
   { 
      mEventType = eventType; 
   }
   inline void setObjectClassObjectInstance(const string& objectClassObjectInstance )
   { 
      mObjectClassObjectInstance = objectClassObjectInstance; 
   }
   inline void setObjectClassObjectInstance(const char* objectClassObjectInstance )
   { 
      mObjectClassObjectInstance = objectClassObjectInstance; 
   }
   inline void setAlarmId(const uint16_t& alarmId) 
   {  
      mAlarmId = alarmId; 
   }
   inline void setAlarmRaiseTime(const string& alarmRaiseTime) 
   { 
      mAlarmRaiseTime = alarmRaiseTime; 
   }
   inline void setAlarmRaiseTime(const char* alarmRaiseTime)
   { 
      mAlarmRaiseTime = alarmRaiseTime; 
   }
   inline void setAlarmChangeTime(const string& alarmChangeTime) 
   { 
      mAlarmChangeTime = alarmChangeTime; 
   }
   inline void setAlarmChangeTime(const char* alarmChangeTime) 
   { 
      mAlarmChangeTime = alarmChangeTime; 
   }
   inline void setAlarmClearTime(const string& alarmClearTime) 
   { 
      mAlarmClearTime = alarmClearTime; 
   }
   inline void setAlarmClearTime(const char* alarmClearTime) 
   { 
      mAlarmClearTime = alarmClearTime; 
   }
   inline void setProbableCause(const string& probableCause) 
   { 
      mProbableCause = probableCause; 
   }
   inline void setProbableCause(const char* probableCause) 
   { 
      mProbableCause = probableCause; 
   }
   inline void setPerceivedSeverity(const SeverityLevel& perceivedSeverity) 
   { 
      mPerceivedSeverity = perceivedSeverity; 
   }
   inline void setRootCauseIndicator(const string& rootCauseIndicator) 
   { 
      mRootCauseIndicator = rootCauseIndicator; 
   }
   inline void setRootCauseIndicator(const char* rootCauseIndicator) 
   { 
      mRootCauseIndicator = rootCauseIndicator; 
   }
   inline void setAdditionalText(const string& additionalText) 
   { 
      mAdditionalText = additionalText; 
   }
   inline void setAdditionalText(const char* additionalText) 
   { 
      mAdditionalText = additionalText; 
   }
   inline void setAdditionalInfo(const string& additionalInfo) 
   { 
      mAdditionalInfo = additionalInfo; 
   }
   inline void setAdditionalInfo(const char* additionalInfo) 
   { 
      mAdditionalInfo = additionalInfo; 
   }
   inline void setSpecificProblem(const string& specificProblem) 
   { 
      mSpecificProblem = specificProblem; 
   }
   inline void setSpecificProblem(const char* specificProblem) 
   { 
      mSpecificProblem = specificProblem; 
   }
  

};

#endif

/**********************************************************************
         End of file
**********************************************************************/
