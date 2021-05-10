/*******************************************************************************
################################################################################
#   Copyright (c) [2021] [HCL Technologis]                                     #
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
#include "JsonHelper.hpp"
#include "VesCommonHeader.hpp"

static uint16_t seqNo=0;

/* Default constructor */
VesCommonHeader::VesCommonHeader()
{

}

/* Default Destructor */
VesCommonHeader::~VesCommonHeader()
{

}

/*******************************************************************
 *
 * @brief provide the next sequence number of VES event
 *
 * @details
 *
 *    Function : getSequenceNo
 *
 *    Functionality:
 *      - provide the next sequence number of VES event
 *
 * @params[in] void
 * @return next sequence number of event  - success
 *
 * ****************************************************************/

uint16_t VesCommonHeader::getSequenceNo()
{
   return seqNo++;
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
 * @params[in] IN - VesEventType , OUT - Ves Event String
 * @return ture     - success
 *         false    - failure
 *
 * ****************************************************************/

bool VesCommonHeader::getEventTypeToStr(VesEventType & evtType, \
                                        string & vesMsgStr)
{
   bool ret = true;
   switch(evtType)
   {
      case VesEventType::pnfRegistration:
         vesMsgStr = "pnfRegistration";
         break;
      case VesEventType::alarmNotification:
         vesMsgStr = "alarmNotification";
         break;
      case VesEventType::pmNotification:
         vesMsgStr = "pmNotification";
         break;
      default:
         O1_LOG("O1 VES : VES msg Type is not avilable");
         ret = false;
         break;
   }
   return ret;
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
 * @params[in] IN - VesEventType , OUT - Ves Event Id
 * @return ture     - success
 *         false    - failure
 *
 * ****************************************************************/

bool VesCommonHeader::getEventId(VesEventType &msgType, string & evntId)
{
   string vesMsgStr;
   bool ret = true;
   if(getEventTypeToStr(msgType, vesMsgStr))
   {
      //evntId = vesMsgStr + "_"+ std::to_string(getSequenceNo());
      evntId = EVENT_ID;
   }
   else
   {
      ret = false;
   }
   return ret;
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

bool VesCommonHeader::getEventName(VesEventType &msgType, string & evntId)
{
   string vesMsgStr;
   bool ret = true;
   if(getEventTypeToStr(msgType, vesMsgStr))
   {
      evntId = vesMsgStr + "_"+ EVENT_TYPE_5G;
   }
   else
   {
      ret = false;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief get current epoch time
 *
 * @details
 *
 *    Function : getEpochTime
 *
 *    Functionality:
 *      - get current epoch time
 *
 * @params[in] void
 * @return epoch time     - success
 *
 * ****************************************************************/

time_t VesCommonHeader::getEpochTime()
{
    struct timeval time_now{};
    gettimeofday(&time_now, nullptr);
    time_t msecs_time = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
    return msecs_time;
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

bool VesCommonHeader::prepareCommonHeader(cJSON *commonHeader, \
                                          VesEventType type)
{
   O1_LOG("O1 VES : createCommonHeader fuction started\n");
   bool ret=true;
   string typeStr;
   string evntId;
   string evntName;
   if(getEventTypeToStr(type, typeStr) && \
      (JsonHelper::addNodeToObject(commonHeader, "domain", \
       typeStr.c_str()) == 0)) {
      ret=false;
   }
   else if ( getEventId(type,  evntId) && \
             JsonHelper::addNodeToObject(commonHeader, "eventId", \
                                         evntId.c_str()) == 0)
   {
      ret =false;
   }
   else if(getEventName(type, evntName) && \
           JsonHelper::addNodeToObject(commonHeader, "eventName", \
                                       evntName.c_str()) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "eventType", \
                                       EVENT_TYPE_5G) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "sequence", \
                                       getSequenceNo()) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "priority", \
                                       PRIORITY) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "reportingEntityId", \
                                       "") == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "reportingEntityName", \
                                       REPORTING_ENTITY_NAME) == 0)
   {
      ret = false;
   }
  else if(JsonHelper::addNodeToObject(commonHeader, "sourceId", \
                                      "") == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "sourceName", \
                                       SOURCE_NAME) == 0)
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
                                       NAMING_CODE) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "nfVendorName", \
                                       VENDOR_NAME) == 0)
   {
      ret = false;
   }
  else if(JsonHelper::addNodeToObject(commonHeader, "timeZoneOffset", \
                                      TIME_ZONE_OFFSET) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "version", \
                                       VERSION) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(commonHeader, "vesEventListenerVersion", \
                                       VES_EVENT_LISTENER_VERSION) == 0)
   {
      ret = false;
   }
   else
   {
      O1_LOG("O1 VES : VES common Header prepared successfully \n");
   }
   return ret;

}

/**********************************************************************
  End of file
 **********************************************************************/
