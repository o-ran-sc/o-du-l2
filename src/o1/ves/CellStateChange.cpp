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
#include "CellStateChange.hpp"
#include "JsonHelper.hpp"

/* Default constructor*/
CellStateChange::CellStateChange() : Notification(VesEventType::FAULT_NOTIFICATION)
{
   
}

/* Default Destructor*/
CellStateChange::~CellStateChange()
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

std::string CellStateChange::getISOEventTime() {
   auto now = std::chrono::system_clock::now();
   auto itt = std::chrono::system_clock::to_time_t(now);
   std::ostringstream os;
   os << std::put_time(gmtime(&itt), "%FT%TZ");
   return os.str();
}

/*******************************************************************
 *
 * @brief prepare CellStateChange Fields
 *
 * @details
 *
 *    Function : prepareCellStateChangeEventFields
 *
 *    Functionality:
 *      - prepare CellStateChange Fields in json format
 *
 * @params[in] IN   - pointer to pnfFields
 * @return true     - success
 *         false    - failure
 *
 * ****************************************************************/

bool CellStateChange::prepareEventFields(const Message* msg) {
   const Alarm* alrm = dynamic_cast<const Alarm*> (msg);
   if(!alrm) {
      O1_LOG("\nO1 CellStateChange : Casting failed in prepareEventFields");
      return false;
   }
   bool ret = true;


   cJSON* faultFields = this->mVesEventFields;

   if(JsonHelper::addNodeToObject(faultFields, "faultFieldsVersion", FAULT_FIELDS_VERSION) == 0) {
      ret = false;
   }
   if(JsonHelper::addNodeToObject(faultFields, "alarmCondition", ALARM_CONDITION) == 0) {
      ret = false;
   }
   if(JsonHelper::addNodeToObject(faultFields, "alarmInterfaceA", ALARM_INTERFACE_A) == 0) {
      ret = false;
   }
   if(JsonHelper::addNodeToObject(faultFields, "eventSourceType", EVENT_SOURCE_TYPE) == 0) {
      ret = false;
   }
   if(JsonHelper::addNodeToObject(faultFields, "specificProblem", SPECIFIC_PROBLEM) == 0) {
      ret = false;
   }
   if(JsonHelper::addNodeToObject(faultFields, "eventSeverity", EVENT_SEVERITY) == 0) {
      ret = false;
   }   
   if(JsonHelper::addNodeToObject(faultFields, "vfStatus", VF_STATUS) == 0) {
      ret = false;
   }
   
   cJSON* alarmAdditionalInformation = JsonHelper::createNode();
   if(alarmAdditionalInformation == 0) {
      O1_LOG("\nO1 CellStateChange : could not create alarmAdditionalInformation JSON object");
      return false;
   }
   else if(JsonHelper::addJsonNodeToObject(faultFields, "alarmAdditionalInformation", \
                                      alarmAdditionalInformation ) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(alarmAdditionalInformation, "eventTime", getISOEventTime().c_str()) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(alarmAdditionalInformation, "equipType", EQUIP_TYPE) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(alarmAdditionalInformation, "vendor", VENDOR) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(alarmAdditionalInformation, "model", MODEL) == 0) {
      ret = false;
   }
   
   
   return ret;
}


/**********************************************************************
  End of file
 **********************************************************************/
