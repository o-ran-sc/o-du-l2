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
      case VesEventType::FAULT_NOTIFICATION:
         str = "faultNotification";
         break;
      case VesEventType::PM_NOTIFICATION:
         str = "pmNotification";
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
   string evntId = "";
   switch(mEventType)
   {
      case VesEventType::PNF_REGISTRATION:
         evntId = getSourceName() + "_" + MODEL_NUMBER_007_DEV;
         break;
      case VesEventType::HEARTBEAT:
         evntId = getEventTypeToStr() + "_" + getCurrentTime();
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
 * @brief create Ves Event Type
 *
 * @details
 *
 *    Function : getEventType
 *
 *    Functionality:
 *      - create Ves Event Type
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
         evntType = EVENT_TYPE_ORAN_COMPONENET;
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
         evntName = getEventTypeToStr() + "_" + EVENT_TYPE_ORAN_COMPONENET;
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
 *    Function : getReportingEntityName
 *
 *    Functionality:
 *      - create Ves Event Name from Ves Event type
 *
 * @params[in] IN - void
 * @return value of string     - success
 *         empty string        - failure
 * ****************************************************************/

string VesCommonHeader::getReportingEntityName()
{
  /*Currently PNF_REGISTRATION only supported. This function must be updated
    in later releases*/

   string evntName = "";
   switch(mEventType)
   {
      case VesEventType::PNF_REGISTRATION:
         evntName = getSourceName();
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
 * @brief create Ves Event Name from Ves Event type
 *
 * @details
 *
 *    Function : getReportingEntityName
 *
 *    Functionality:
 *      - create Ves Event Name from Ves Event type
 *
 * @params[in] IN - void
 * @return value of string     - success
 *         empty string        - failure
 * ****************************************************************/

string VesCommonHeader::getSourceName()
{
  /*Currently PNF_REGISTRATION only supported. This function need to be updated
    in later releases*/
   return ODU_HIGH;
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
  /*Currently PNF_REGISTRATION only supported. This function need to be updated
    in later releases*/
   return NAMING_CODE_ODU;
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
 * @brief get current date-time
 *
 * @details
 *
 *    Function : getCurrentTime
 *
 *    Functionality:
 *      - get current date-time
 *
 * @params[in] void
 * @return time-date     - success
 *
 * ****************************************************************/

string VesCommonHeader::getCurrentTime()
{
   time_t t = time(0);
   std::ostringstream oss;
   char *dateStr;
   strftime(dateStr, MAX_TIME_STR, "%F", localtime(&t));
    oss << dateStr;
    return oss.str();
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
 * @params[in] IN - VesEventType , OUT - Ves Event Name
 * @return ture     - success
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
   else if(JsonHelper::addNodeToObject(commonHeader, "sequence", \
                                       getSequenceNo()) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "priority", \
                                       getPriority().c_str()) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "reportingEntityId", \
                                       "") == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "reportingEntityName", \
                                       getReportingEntityName().c_str()) == 0)
   {
      ret = false;
   }
  else if(JsonHelper::addNodeToObject(commonHeader, "sourceId", \
                                      "") == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "sourceName", \
                                       getSourceName().c_str()) == 0)
   {
      ret = false;
   }
  else if(JsonHelper::addNodeToObject(commonHeader, "startEpochMicrosec", \
                                      (double) getEpochTime()) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "lastEpochMicrosec", \
                                       (double) getEpochTime()) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "nfNamingCode", \
                                       getNamingCode().c_str()) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "nfVendorName", \
                                       "") == 0)
   {
      ret = false;
   }
  else if(JsonHelper::addNodeToObject(commonHeader, "timeZoneOffset", \
                                      TIME_ZONE_00_00) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "version", \
                                       VERSION_4_0_1) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "vesEventListenerVersion", \
                                       VES_EVENT_LISTENER_7_2_1) == 0)
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
