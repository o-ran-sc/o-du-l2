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

#include "AlarmNotification.hpp"
#include "JsonHelper.hpp"
/* Default constructor*/
AlarmNotification::AlarmNotification() : VesNotification(VesEventType::VES_NOTIFICATION)
{
   
}

/* Default Destructor*/
AlarmNotification::~AlarmNotification()
{

}

/*******************************************************************
 *
 * @brief prepare AlarmNotification Fields
 *
 * @details
 *
 *    Function : prepareAlarmNotificationEventFields
 *
 *    Functionality:
 *      - prepare AlarmNotifications Fields in json format
 *
 * @params[in] IN   - pointer to pnfFields
 * @return true     - success
 *         false    - failure
 *
 * ****************************************************************/

bool AlarmNotification::prepareEventFields(const Message* msg) {
   const Alarm* alrm = dynamic_cast<const Alarm*> (msg);
   if(!alrm) {
      std::cout << "Casting failed in AlarmNotification::prepareEventFields" << std::endl;
      return false;
   }
   bool ret = true;


   cJSON* stndDefinedFields = this->mVesEventFields;

   if(JsonHelper::addNodeToObject(stndDefinedFields, "schemaReference", ALARM_NOTIFICATION_SCHEMA) == 0) {
      ret = false;
   }

   cJSON* data = JsonHelper::createNode();
   if(data == 0) {
      O1_LOG("\nO1 AlarmNotification : could not create data JSON object");
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
   else if(JsonHelper::addNodeToObject(data, "eventTime", "") == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "systemDN", "") == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "probableCause", PROBABLE_CAUSE) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "perceivedSeverity", PRIORITY_LOW) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "rootCauseIndicator", (bool)false ) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "specificProblem", "") == 0) {
      ret = false;
   }
   
   cJSON*  arr= cJSON_CreateArray();
   if (arr == NULL)
   {
      ret = false;
   }
   cJSON_AddItemToObject(data, "correlatedNotifications", arr);
  
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
      O1_LOG("\nO1 AlarmNotification : could not create thresholdInfo JSON object");
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

   cJSON* stateChangeDefinition = JsonHelper::createNode();
   if(stateChangeDefinition == 0) {
      O1_LOG("\nO1 AlarmNotification : could not create stateChangeDefinition JSON object");
      return false;
   }
   else if(JsonHelper::addJsonNodeToObject(data , "stateChangeDefinition", \
                                       stateChangeDefinition ) == 0) {
      ret = false;
   }

   cJSON* monitoredAttributes = JsonHelper::createNode();
   if( monitoredAttributes == 0) {
      O1_LOG("\nO1 AlarmNotification : could not create monitoredAttributes JSON object");
      return false;
   }
   else if(JsonHelper::addJsonNodeToObject(data , "monitoredAttributes", \
                                       monitoredAttributes ) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(monitoredAttributes, "newAtt", NEW_ATT) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "proposedRepairActions", "") == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "additionalText", "") == 0) {
      ret = false;
   }

   cJSON* additionalInformation = JsonHelper::createNode();
   if(additionalInformation == 0) {
      O1_LOG("\nO1 AlarmNotification : could not create additionalInformation JSON object");
      return false;
   }
   else if(JsonHelper::addJsonNodeToObject(data , "additionalInformation", \
                                       additionalInformation ) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject( additionalInformation, "addInfo", ADD_INFO) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data , "alarmId", ALARM_ID) == 0) {
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
