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

/* This file contains functions to support the preparation of VES common header
   parameters*/

#include <stdio.h>
#include <typeinfo>
#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <sstream>
#include "JsonHelper.hpp"
#include "VesCommonHeader.hpp"

static uint16_t seqNo = 0;

#define MAX_TIME_STR 35

/*******************************************************************
 *
 * @brief provide next sequence number of VES event
 *
 * @details
 *
 *    Function : nextSequenceNo
 *
 *    Functionality:
 *      - provide next sequence number of VES event
 *
 * @params[in] void
 * @return next sequence number of event  - success
 *
 * ****************************************************************/

uint16_t VesCommonHeader::nextSequenceNo()
{
   return seqNo++;
}

/*******************************************************************
 *
 * @brief provide current sequence number of VES event
 *
 * @details
 *
 *    Function : getSequenceNo
 *
 *    Functionality:
 *      - provide current sequence number of VES event
 *
 * @params[in] void
 * @return current sequence number of event  - success
 *
 * ****************************************************************/

uint16_t VesCommonHeader::getSequenceNo()
{
   return seqNo;
}

/*******************************************************************
 *
 * @brief Convert Ves Event type to string
 *
 * @details
 *
 *    Function : getEventTypeToStr
 *
 *    Functionality:
 *      - Convert Ves Event type to String
 *
 * @params[in] IN - void
 * @return value of string     - success
 *         empty string        - failure
 *
 * ****************************************************************/

string VesCommonHeader::getEventTypeToStr()
{
   string str = "";
   switch(mEventType)
   {
      case VesEventType::PNF_REGISTRATION:
         str = "pnfRegistration";
         break;
      case VesEventType::VES_NOTIFICATION:
         str = "stndDefined";
         break;
      case VesEventType::PM_NOTIFICATION:
         str = "pmNotification";
         break;
      case VesEventType::PM_SLICE:
         str = "measurement";
         break;
      case VesEventType::HEARTBEAT:
         str = "heartbeat";
         break;
      default:
         O1_LOG("\nO1 VesCommonHeader : VES msg Type not supported");
         break;
   }
   return str;
}

/*******************************************************************
 *
 * @brief create Ves Event Id from Ves Event type
 *
 * @details
 *
 *    Function : getEventId
 *
 *    Functionality:
 *      - create Ves Event Id from Ves Event type
 *
 * @params[in] IN - void
 * @return value of string     - success
 *         empty string        - failure
 *
 * ****************************************************************/

string VesCommonHeader::getEventId()
{
  /*Currently PNF_REGISTRATION only supported. This function must be updated
    in later releases*/
   std::ostringstream oss;
   oss << mLastEpochTime;
   string stringEpochTime = oss.str().substr(0, 10);
   string evntId = "";
   switch(mEventType)
   {
      case VesEventType::PNF_REGISTRATION:
         evntId = getSourceName() + "_" + MODEL_NUMBER_007_DEV;
         break;
      case VesEventType::HEARTBEAT:
         evntId = getEventTypeToStr() + "_" + formatTime(getCurrentTime());
         break;
      case VesEventType::PM_SLICE:
         evntId = "_" + stringEpochTime + "_" + "PM1min";
     break;
      case VesEventType::VES_NOTIFICATION:
         evntId = FAULT_EVENTID;
     break;
      default:
         O1_LOG("\nO1 VesCommonHeader : this VES msg Type support in getEventId is \
not available");
         break;
   }
   return evntId;
}

/*******************************************************************
 *
 * @brief create Ves Event Type from Ves Event type
 *
 * @details
 *
 *    Function : getEventType
 *
 *    Functionality:
 *      - create Ves Event Type from Ves Event type
 *
 * @params[in] IN - void
 * @return value of string     - success
 *         empty string        - failure
 *
 * ****************************************************************/

string VesCommonHeader::getEventType()
{
  /*Currently PNF_REGISTRATION only supported. This function must be updated
    in later releases*/

   string evntType = "";
   switch(mEventType)
   {
      case VesEventType::PNF_REGISTRATION:
         evntType = EVENT_TYPE_5G;
         break;
      case VesEventType::HEARTBEAT:
         evntType = EVENT_TYPE_ORAN_COMPONENT;
         break;
      case VesEventType::PM_SLICE:
         evntType = EVENT_TYPE_ORAN_COMPONENT_PM;
         break;
      case VesEventType::VES_NOTIFICATION:
         evntType = FAULT_TYPE;
	 break;
      default:
         O1_LOG("\nO1 VesCommonHeader : this VES msg Type support in getEvenType is \
not available");
         break;
   }
   return evntType;
}

/*******************************************************************
 *
 * @brief return priority of event
 *
 * @details
 *
 *    Function : getPriority
 *
 *    Functionality:
 *      - send priority of event
 *
 * @params[in] IN - void
 * @return value of string     - success
 *         empty string        - failure
 *
 * ****************************************************************/

string VesCommonHeader::getPriority()
{
  /*Currently PNF_REGISTRATION only supported. This function must be updated
    in later releases*/

   string evntId = "";
   switch(mEventType)
   {
      case VesEventType::PNF_REGISTRATION:
         evntId = PRIORITY_LOW ;
         break;
      case VesEventType::HEARTBEAT:
         evntId = PRIORITY_LOW;
         break;
      case VesEventType::PM_SLICE:
         evntId = PRIORITY_LOW ;
         break;
      case VesEventType::VES_NOTIFICATION:
         evntId = PRIORITY_LOW ;
	 break;
      default:
         O1_LOG("\nO1 VesCommonHeader : This VES msg Type support in getPriority is \
not available");
         break;
   }
   return evntId;
}



/*******************************************************************
 *
 * @brief create Ves Event Name from Ves Event type
 *
 * @details
 *
 *    Function : getEventName
 *
 *    Functionality:
 *      - create Ves Event Name from Ves Event type
 *
 * @params[in] IN - void
 * @return value of string     - success
 *         empty string        - failure
 * ****************************************************************/

string VesCommonHeader::getEventName()
{
  /*Currently PNF_REGISTRATION only supported. This function must be updated
    in later releases*/

   string evntName = "";
   switch(mEventType)
   {
      case VesEventType::PNF_REGISTRATION:
         evntName = getEventTypeToStr() + "_" + EVENT_TYPE_5G;
         break;
      case VesEventType::HEARTBEAT:
         evntName = getEventTypeToStr() + "_" + EVENT_TYPE_ORAN_COMPONENT;
         break;
      case VesEventType::PM_SLICE:
         evntName = getEventTypeToStr() + "_" + EVENT_TYPE_ORAN_COMPONENT_PM;
         break;
      case VesEventType::VES_NOTIFICATION:
         evntName = FAULT_EVENT_NAME;
	 break;
      default:
         O1_LOG("\nO1 VesCommonHeader : This VES msg Type support in getEventName is \
         not available");
         break;
   }
   return evntName;
}

/*******************************************************************
 *
 * @brief create Ves Event Name from Ves Event type
 *
 * @details
 *
 *    Function : getReportingEntityId
 *
 *    Functionality:
 *      - create Ves Event Name from Ves Event type
 *
 * @params[in] IN - void
 * @return value of string     - success
 *         empty string        - failure
 * ****************************************************************/

string VesCommonHeader::getReportingEntityId()
{
  /*Currently PNF_REGISTRATION and PM_SLICE are only supported. 
   This function must be updated in later releases*/

   string evntName = "";
   switch(mEventType)
   {
      case VesEventType::PNF_REGISTRATION:
         evntName = ODU_HIGH;
         break;

      case VesEventType::VES_NOTIFICATION:
         evntName = ODU_HIGH;
	 break;
      default:
         break;
   }
   return evntName;
}

/*******************************************************************
 *
 * @brief create Reporting Entity Name from Ves Event type
 *
 * @details
 *
 *    Function : getReportingEntityName
 *
 *    Functionality:
 *      - create Reporting Entity Name from Ves Event type
 *
 * @params[in] IN - void
 * @return value of string     - success
 *         empty string        - failure
 * ****************************************************************/

string VesCommonHeader::getReportingEntityName()
{
   string evntName = "";
   switch(mEventType)
   {
      case VesEventType::PNF_REGISTRATION:
         evntName = getSourceName();
         break;
      case VesEventType::PM_SLICE:
         evntName = PM_REPORTING_ENTITY;
         break;
      case VesEventType::VES_NOTIFICATION:
         evntName = ODU_HIGH;
	 break;
      default:
         O1_LOG("\nO1 VesCommonHeader : This VES msg Type support in \
         getReportingEntityName is not available");
         break;
   }
   return evntName;
}

/*******************************************************************
 *
 * @brief create Source Name from Ves Event type
 *
 * @details
 *
 *    Function : getSourceName
 *
 *    Functionality:
 *      - create Source Name from Ves Event type
 *
 * @params[in] IN - void
 * @return value of string     - success
 *         empty string        - failure
 * ****************************************************************/

string VesCommonHeader::getSourceName()
{
   string sourceName = "";
   switch(mEventType)
   
   {
      case VesEventType::PNF_REGISTRATION:
         sourceName = ODU_HIGH;
         break;
      case VesEventType::VES_NOTIFICATION:
         sourceName = ODU_HIGH;
	 break;
      default:
         break;
   }
   return sourceName;
}

/*******************************************************************
 *
 * @brief create Ves Event SourceId from Ves Event type
 *
 * @details
 *
 *    Function : getSourceId
 *
 *    Functionality:
 *      - create Ves Event SourceId from Ves Event type
 *
 * @params[in] IN - void
 * @return value of string     - success
 *         empty string        - failure
 * ****************************************************************/
string VesCommonHeader::getSourceId()
{
   string sourceId = "";
   switch(mEventType)
   {
      case VesEventType::VES_NOTIFICATION:
         sourceId = SOURCE_ID;
	 break;
      default:
         break;
   }
   return sourceId;
}

/*******************************************************************
 *
 * @brief create Ves Event Name from Ves Event type
 *
 * @details
 *
 *    Function : getnfcNamingCode
 *
 *    Functionality: create Ves Event nfc naming code
 *
 * @params[in] IN - void
 * @return value of string     - success
 *         empty string        - failure
 * ****************************************************************/

string VesCommonHeader::getnfcNamingCode()
{
   string name = "";
   switch(mEventType)
   {
      case VesEventType::VES_NOTIFICATION:
         name = NFC_NAMING_CODE;
	 break;
      default:
         break;
   }
   return name;
}

/*******************************************************************
 *
 * @brief create Ves Event Name from Ves Event type
 *
 * @details
 *
 *    Function : getNamingCode
 *
 *    Functionality:
 *      - Network function type: 4 characters (aligned with vnf and pnf
           naming standards)
 *
 * @params[in] IN - void
 * @return value of string     - success
 *         empty string        - failure
 * ****************************************************************/

string VesCommonHeader::getNamingCode()
{

   string nammingcdoe = "";
   switch(mEventType)
   {
      case VesEventType::PNF_REGISTRATION:
         nammingcdoe = NAMING_CODE_ODU;
         break;
      case VesEventType::PM_SLICE:
         break;
      case VesEventType::VES_NOTIFICATION:
         nammingcdoe = NAMING_CODE_ODU;
	 break;
      default:
         O1_LOG("\nO1 VesCommonHeader : This VES msg Type support in \
         getReportingEntityName is not available");
         break;
   }
   return nammingcdoe;
}



/*******************************************************************
 *
 * @brief provide current epoch time
 *
 * @details
 *
 *    Function : getEpochTime
 *
 *    Functionality:
 *      - provide current epoch time
 *
 * @params[in] void
 * @return epoch time     - success
 *
 * ****************************************************************/

uint64_t VesCommonHeader::getEpochTime()
{
    uint64_t epochTimeInMicrosec = std::chrono::duration_cast \
            <std::chrono::microseconds> \
            (std::chrono::system_clock::now().time_since_epoch()).count();
    return epochTimeInMicrosec;
}

/*******************************************************************
 *
 * @brief get current time
 *
 * @details
 *
 *    Function : getCurrentTime
 *
 *    Functionality:
 *      - get current time
 *
 * @params[in] void
 * @return time     - success
 *
 * ****************************************************************/

time_t VesCommonHeader::getCurrentTime()
{
   time_t t;
   time(&t);
   return t;
}

/*******************************************************************
 *
 * @brief formats the current time like: 
 *           "Thu, 14 Oct 2021 03:15:00 +0000"
 *
 * @details
 *
 *    Function : formatTime
 *
 *    Functionality:
 *      - formats current time
 *
 * @params[in] void
 * @return formatted time - success
 *
 * ****************************************************************/

std::string VesCommonHeader::formatTime(time_t t) {
   std::ostringstream oss;
   char dateStr[MAX_TIME_STR];
   strftime(dateStr, sizeof(dateStr), "%a, %d %b %Y %T %z", gmtime(&t));
   oss << dateStr;
   return oss.str();
}


/*******************************************************************
 *
 * @brief create Ves stndDefinedNamespace from Ves Event type
 *
 * @details
 *
 *    Function : getstndDefinedNamespace
 *
 *    Functionality: create Ves tndDefinedNamespace
 *
 * @params[in] IN - void
 * @return value of string     - success
 *         empty string        - failure
 * ****************************************************************/

string VesCommonHeader::getstndDefinedNamespace()
{
   string stndDefinedNamespace="";
   switch(mEventType)
   {
      case VesEventType::VES_NOTIFICATION:
         stndDefinedNamespace = STND_DEFINED_NAMESPACE;
	 break;
      default:
         break;
   }
   return stndDefinedNamespace;

}

/*******************************************************************
 *
 * @brief Prepare VES Message
 *
 * @details
 *
 *    Function : prepare
 *
 *    Functionality:
 *      - prepare VES event
 *
 * @params[in] void
 * @return true     - success
 *         false    - failure
 *
 * ****************************************************************/


bool VesCommonHeader::prepare(cJSON *commonHeader, \
                              VesEventType type)
{
   bool ret=true;
   string typeStr;
   string evntId;
   string evntName;
   mEventType = type;
   seqNo=0;
   nextSequenceNo(); //update the sequence number for next message
   //local utility variables:
   time_t intervalStartTime = getCurrentTime();
   time_t intervalEndTime = intervalStartTime+60; /*adding 1 min to system time*/
   uint64_t startEpochTime = getEpochTime();
   mLastEpochTime = startEpochTime+60*100000; /*adding 1 min to epoch time*/

   if(JsonHelper::addNodeToObject(commonHeader, "domain", \
                                  getEventTypeToStr().c_str()) == 0) {
      ret=false;
   }
   else if ( JsonHelper::addNodeToObject(commonHeader, "eventId", \
                                         getEventId().c_str()) == 0)
   {
      ret =false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "eventName", \
                                       getEventName().c_str()) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "eventType", \
                                       getEventType().c_str()) == 0)
   {
      ret = false;
   }

   if (mEventType == VesEventType::PM_SLICE)
   {
      cJSON *internalHeaderFields = JsonHelper::createNode();
      if(internalHeaderFields == 0)
      {
         ret = false;
      }
      else if(JsonHelper::addJsonNodeToObject(commonHeader, "internalHeaderFields", \
                                          internalHeaderFields) == 0)
      {
         ret = false;
      }
      else if(JsonHelper::addNodeToObject(internalHeaderFields, "intervalEndTime", \
                                          formatTime(intervalEndTime).c_str()) == 0)
      {
         ret = false;
      }
      else if(JsonHelper::addNodeToObject(internalHeaderFields, "intervalStartTime", \
                                          formatTime(intervalStartTime).c_str()) == 0)
      {
         ret = false;
      }
   }
   
   if (mEventType == VesEventType::PNF_REGISTRATION)
   {

   }

   if(JsonHelper::addNodeToObject(commonHeader, "sequence", \
                                       (double)getSequenceNo()) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "priority", \
                                       getPriority().c_str()) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "reportingEntityId", \
                                       getReportingEntityId().c_str() ) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "reportingEntityName", \
                                       getReportingEntityName().c_str()) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "sourceId", \
                                      getSourceId().c_str() ) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "sourceName", \
                                       getSourceName().c_str()) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "startEpochMicrosec", \
                                      (double)startEpochTime) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "lastEpochMicrosec", \
                                       (double)mLastEpochTime) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "nfNamingCode", \
                                       getNamingCode().c_str() ) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "nfVendorName", \
                                       "POC") == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "nfcNamingCode", \
                                       getnfcNamingCode().c_str() ) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "timeZoneOffset", \
                                      TIME_ZONE_00_00) == 0)
   {
      ret = false;
   }

   if (mEventType == VesEventType::PM_SLICE)
   {
      if(JsonHelper::addNodeToObject(commonHeader, "version", \
                                          VERSION_4_1) == 0)
      {
         ret = false;
      }
   }
   else 
   {
      if(JsonHelper::addNodeToObject(commonHeader, "version", \
                                          "4.0.1") == 0)
      {
         ret = false;
      }
   }
   if(JsonHelper::addNodeToObject(commonHeader, "stndDefinedNamespace", \
                                       getstndDefinedNamespace().c_str())== 0)            
   {
      ret = false;
   }
   if(JsonHelper::addNodeToObject(commonHeader, "vesEventListenerVersion", \
                                       "7.2.1") == 0)
   {	
      ret = false;
   }
   else
   {
      O1_LOG("\nO1 VesCommonHeader : VES common Header prepared");
   }
   return ret;
}

/**********************************************************************
  End of file
 **********************************************************************/
