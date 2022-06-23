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

#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "CellStateChangeStdDef.hpp"
#include "JsonHelper.hpp"
/* Default constructor*/
CellStateChangeStdDef::CellStateChangeStdDef() : Notification(VesEventType::FAULT_NOTIFICATION)
{

}

/* Default Destructor*/
CellStateChangeStdDef::~CellStateChangeStdDef()
{

}


/*******************************************************************
 *
 * @brief Returns ISO time String 
 *
 * @details
 *
 *    Function :getISOEventTime
 *
 *    Functionality:
 *      - Returns ISO time String
 *
 * @params[in] IN - void
 * @return value of string     - success
 *         empty string        - failure
 *
 * ****************************************************************/

std::string CellStateChangeStdDef::getISOEventTime() {
   auto now = std::chrono::system_clock::now();
   auto itt = std::chrono::system_clock::to_time_t(now);
   std::ostringstream os;
   os << std::put_time(gmtime(&itt), "%FT%TZ");
   return os.str();
}

/*******************************************************************
 *
 * @brief Returns Event Severity String
 *
 * @details
 *
 *    Function :getEventSeverity
 *
 *    Functionality:
 *      - Returns Event Severity String
 *
 * @params[in] IN - int severity
 * @return value of string     - success
 *         empty string        - failure
 *
 * ****************************************************************/


std::string CellStateChangeStdDef::getEventSeverity(int severity) {
   string str;
   switch(severity)
   {
      case 3:
         str = "CRITICAL";
         break;
      case 4:
         str = "MAJOR";
         break;
      case 5:
         str = "MINOR";
         break;
      case 6:
         str = "WARNING";
         break;
      case 7:
         str = "INDETERMINATE";
         break;
      case 8:
         str = "CLEARED";
         break;
      default:
         O1_LOG("\nO1 CellStateChangeStdDef: severity was not mentioned");
         break;
   }
   return str;
}

/*******************************************************************
 *
 * @brief prepare CellStateChangeStdDef Fields
 *
 * @details
 *
 *    Function : prepareCellStateChangeStdDefEventFields
 *
 *    Functionality:
 *      - prepare CellStateChangeStdDef Fields in json format
 *
 * @params[in] IN   - pointer to pnfFields
 * @return true     - success
 *         false    - failure
 *
 * ****************************************************************/

bool CellStateChangeStdDef::prepareEventFields(const Message* msg) {
   const Alarm* alarm = dynamic_cast<const Alarm*> (msg);
   if(!alarm) {
      O1_LOG("\nO1 CellStateChangeStdDef : Casting failed in prepareEventFields");
      return false;
   }
   bool ret = true;


   cJSON* stndDefinedFields = this->mVesEventFields;

   if(JsonHelper::addNodeToObject(stndDefinedFields, "schemaReference", FAULT_SCHEMA) == 0) {
      ret = false;
   }

   cJSON* data = JsonHelper::createNode();
   if(data == 0) {
      O1_LOG("\nO1 CellStateChangeStdDef : could not create data JSON object");
      return false;
   }
   else if(JsonHelper::addJsonNodeToObject(stndDefinedFields, "data", \
                                       data) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "href", HREF) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "uri", URI) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "notificationId", NOTIFICATION_ID) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "notificationType", NOTIFICATION_TYPE) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "eventTime", getISOEventTime().c_str()) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "systemDN", "") == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "probableCause", PROBABLE_CAUSE) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "perceivedSeverity", getEventSeverity(alarm->getPerceivedSeverity()).c_str()) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "rootCauseIndicator", (bool)false ) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "specificProblem", alarm->getAdditionalInfo().c_str()) == 0) {
      ret = false;
   }

   cJSON*  correlatedNotificationsArr= cJSON_CreateArray();
   if (correlatedNotificationsArr == NULL)
   {
      ret = false;
   }
   cJSON_AddItemToObject(data, "correlatedNotifications", correlatedNotificationsArr);

   if(JsonHelper::addNodeToObject(data, "backedUpStatus", (bool)true) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "backUpObject", "") == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "trendIndication", TRND_INDICATION) == 0) {
      ret = false;
   }

   cJSON* thresholdInfo = JsonHelper::createNode();
   if(thresholdInfo == 0) {
      O1_LOG("\nO1 CellStateChange : could not create thresholdInfo JSON object");
      return false;
   }
   else if(JsonHelper::addJsonNodeToObject(data, "thresholdInfo", \
                                      thresholdInfo ) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(thresholdInfo, "observedMeasurement", OBSRVED_MEASUREMENT) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(thresholdInfo, "observedValue", OBSERVED_VALUE) == 0) {
      ret = false;
   }

   cJSON*  stateChangeDefinitionArr= cJSON_CreateArray();
   if (stateChangeDefinitionArr == NULL)
   {
      ret = false;
   }
   cJSON_AddItemToObject(data, "stateChangeDefinition", stateChangeDefinitionArr);

   cJSON* monitoredAttributes = JsonHelper::createNode();
   if( monitoredAttributes == 0) {
      O1_LOG("\nO1 CellStateChange : could not create monitoredAttributes JSON object");
      return false;
   }
   else if(JsonHelper::addJsonNodeToObject(data , "monitoredAttributes", \
                                       monitoredAttributes ) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(monitoredAttributes, "newAtt", NEW_ATT) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "proposedRepairActions", PROPOSED_REPAIR_ACTION) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "additionalText", alarm->getAdditionalText().c_str())== 0) {
      ret = false;
   }

   cJSON* additionalInformation = JsonHelper::createNode();
   if(additionalInformation == 0) {
      O1_LOG("\nO1 CellStateChangeStdDef : could not create additionalInformation JSON object");
      return false;
   }
   else if(JsonHelper::addJsonNodeToObject(data , "additionalInformation", \
                                       additionalInformation ) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject( additionalInformation, "addInfo", alarm->getAdditionalInfo().c_str()) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data , "alarmId", to_string(alarm->getAlarmId()).c_str()) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data , "alarmType",ALRAM_TYPE ) == 0) {
      ret = false;
   }

   else if(JsonHelper::addNodeToObject(stndDefinedFields, "stndDefinedFieldsVersion", STND_DEFINED_FEILD_VERSION) == 0) {
      ret = false;
   }



   return ret;
}


/**********************************************************************
  End of file
 **********************************************************************/
