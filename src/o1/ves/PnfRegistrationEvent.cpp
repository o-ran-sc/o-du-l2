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

/* This file contains macros and functions to support the preparation of pnf
   Registration VES Event*/

#include <ctime>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include "PnfRegistrationEvent.hpp"
#include "JsonHelper.hpp"
#include "VesUtils.hpp"

/* Default constructor*/
PnfRegistrationEvent::PnfRegistrationEvent()
               : VesEvent(VesEventType::PNF_REGISTRATION)
{
}

/* Default Destructor*/
PnfRegistrationEvent::~PnfRegistrationEvent()
{
}

 
/*******************************************************************
 *
 * @brief get today's date in YYYY-MM-DD format
 *
 * @details
 *
 *    Function : getCurrentDate
 *
 *    Functionality:
 *      - get today's date in YYYY-MM-DD format
 *
 * @params[in] OUT - date string pointer
 * @return true     - success
 *         false    - failure
 *
 * ****************************************************************/

string PnfRegistrationEvent::getCurrentDate()
{
   time_t t = time(0);
   char dateStr[MAX_TIME_STR];
   strftime(dateStr, MAX_TIME_STR, "%F", localtime(&t));
   std::ostringstream oss;
   oss<< dateStr;
   return oss.str();
}

/*******************************************************************
 *
 * @brief provide mac address of netconf server
 *
 * @details
 *
 *    Function : getMacAddr
 *
 *    Functionality:
 *      - provide mac address of netconf server
 *
 * @params[in] void
 * @return mac address as string - success
 *
 * ****************************************************************/

string PnfRegistrationEvent::getNetconfMacAddr()
{
   if(mNetconfMacAddr != "") {
      return mNetconfMacAddr;
   }
   else {
      O1_LOG("\nO1 PnfRegistrationEvent : could not get Netconf Mac Address");
      return "";
   }
}


/*******************************************************************
 *
 * @brief provide ipv4 address of netconf server
 *
 * @details
 *
 *    Function : getNetconfV4ServerIP
 *
 *    Functionality:
 *      - provide ipv4 address of netconf server
 *
 * @params[in] void
 * @return ipv4 address as string - success
 *
 * ****************************************************************/

string PnfRegistrationEvent::getNetconfV4ServerIP()
{
   if(mNetconfIpv4 != "") {
      return mNetconfIpv4;
   }
   else {
      O1_LOG("\nO1 PnfRegistrationEvent : could not get Netconf IPv4 ip");
      return "";
   }
}

/*******************************************************************
 *
 * @brief provides Netconf Server Port number
 *
 * @details
 *
 *    Function : getNetconfPort
 *
 *    Functionality:
 *      - provides Netconf Server Port number
 *      - provide default port if not provided
 *
 * @params[in] void
 * @return port number - success
 *
 * ****************************************************************/

string PnfRegistrationEvent::getNetconfPort()
{
   if(mNetconfPort != "") {
      return mNetconfPort;
   }
   else {
      O1_LOG("\nO1 PnfRegistrationEvent : Could not get Netconf Port number");
      return NETCONF_DEFAULT_PORT;
   }
}

/*******************************************************************
 *
 *
 * @brief provides Netconf Server Username
 *
 * @details
 *
 *    Function : getUsername
 *
 *    Functionality:
 *      - provides Netconf Server Username
 *
 * @params[in] void
 * @return port number - success
 *
 * ****************************************************************/


string PnfRegistrationEvent::getUsername()
{
   if(mNetconfUsername != "") {
      return mNetconfUsername;
   }
   else {
      O1_LOG("\nO1 PnfRegistrationEvent : could not get Netconf username");
      return "";
   }
}

/*******************************************************************
 *
 *
 * @brief provides Netconf Server Password
 *
 * @details
 *
 *    Function : getPassword
 *
 *    Functionality:
 *      - provides Netconf Server password
 *
 * @params[in] void
 * @return port number - success
 *
 * ****************************************************************/

string PnfRegistrationEvent::getPassword()
{
   if(mNetconfPassword != "") {
      return mNetconfPassword;
   }
   else {
      O1_LOG("\nO1 PnfRegistrationEvent : could not get Netconf password");
      return "";
   }
}

/*******************************************************************
 *
 * @brief provides ipv6 address of netconf server
 *
 * @details
 *
 *    Function : getNetconfV6ServerIP
 *
 *    Functionality:
 *      - provides ipv6 address of netconf server
 *
 * @params[in] void
 * @return ipv6 address as string - success
 *
 * ****************************************************************/

string PnfRegistrationEvent::getNetconfV6ServerIP()
{
   if(mNetconfIpv6 != "") {
      return mNetconfIpv6;
   }
   else {
      O1_LOG("\nO1 PnfRegistrationEvent : could not get Netconf IPv6 ip");
      return "";
   }
}

/*******************************************************************
 *
 * @brief provide Serial Number
 *
 * @details
 *
 *    Function : getSerialNumber
 *
 *    Functionality:
 *      - provide  Serial Number
 *
 * @params[in] void
 * @return serial number - success
 *         empty string  - failure
 *
 * ****************************************************************/

string PnfRegistrationEvent::getSerialNumber()
{
      string serialNum;
      serialNum.append(VENDER_NAME_VENDORB).append("-").append(UNIT_TYPE_7DEV);
      serialNum.append("-").append(getNetconfV4ServerIP()).append("-");
      serialNum.append(MODEL_NUMBER_007_DEV);
      //string serialNum = VENDER_NAME_VENDORB + "-" + UNIT_TYPE_7DEV + "-" \
                         + getNetconfV4ServerIP() + "-" + MODEL_NUMBER_007_DEV;
   return serialNum;
}

/*******************************************************************
 *
 * @brief provide Unit Family
 *
 * @details
 *
 *    Function : getUnitFamily
 *
 *    Functionality:
 *      - provide  Unit Family
 *
 * @params[in] void
 * @return serial number - success
 *         empty string  - failure
 *
 * ****************************************************************/

string PnfRegistrationEvent::getUnitFamily()
{
   string unitFamily;
   unitFamily.append(VENDER_NAME_VENDORB).append("-").append(UNIT_TYPE_7DEV);
   return unitFamily;
}


/*******************************************************************
 *
 * @brief prepare PNF registration Fields
 *
 * @details
 *
 *    Function : preparePnfRegistrationEventFields
 *
 *    Functionality:
 *      - prepare PNF registration Fields in json format
 *
 * @params[in] IN   - pointer to pnfFields 
 * @return true     - success
 *         false    - failure
 *
 * ****************************************************************/

bool PnfRegistrationEvent::prepareEventFields(const Message* msg)
{
   bool ret = true;
   cJSON* pnfFields = this->mVesEventFields;
   
   getNetconfConfig();
   
   if(JsonHelper::addNodeToObject(pnfFields, "pnfRegistrationFieldsVersion", \
                                       PNF_REGISTRATION_VERSION_2_1) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(pnfFields, \
                       "lastServiceDate", getCurrentDate().c_str()) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(pnfFields, "macAddress", \
                                       getNetconfMacAddr().c_str()) == 0) {
      ret = false;
   }
   if(JsonHelper::addNodeToObject(pnfFields, \
                  "manufactureDate", getCurrentDate().c_str()) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(pnfFields, "modelNumber", \
                                       MODEL_NUMBER_007_DEV) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(pnfFields, "oamV4IpAddress", \
                                       getNetconfV4ServerIP().c_str()) == 0) {
      ret = false;
   }
   else if(getNetconfV6ServerIP() != "" && (JsonHelper::addNodeToObject( \
				       pnfFields, "oamV6IpAddress", \
                                       getNetconfV6ServerIP().c_str()) == 0)) {
         ret = false;
   }
   else if(JsonHelper::addNodeToObject(pnfFields, "serialNumber", \
                                       getSerialNumber().c_str()) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(pnfFields, "softwareVersion", \
                                       SOFTWARE_VERSION_2_3_5) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(pnfFields, "unitFamily", \
                                       getUnitFamily().c_str()) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(pnfFields, "unitType", \
                                       UNIT_TYPE_7DEV) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(pnfFields, "vendorName", \
                                       VENDER_NAME_VENDORB) == 0) {
      ret = false;
   }
   else
   {
      cJSON *addFields = JsonHelper::createNode();
      if(addFields == 0) {
         O1_LOG("\nO1 PnfRegistrationEvent : could not create additional fields JSON object");
         return false;
      }

      if(prepareAdditionalFields(addFields))
      {
         if(addFields == 0) {
            O1_LOG("\nO1 PnfRegistrationEvent : could not prepare additional fields cJSON object");
            JsonHelper::deleteNode(pnfFields);
            return false;
         }

         if(JsonHelper::addJsonNodeToObject(pnfFields, "additionalFields", \
                               addFields) == 0) {
            O1_LOG("\nO1 PnfRegistrationEvent : could not add additional fields");
            JsonHelper::deleteNode(pnfFields);
            return false;
         }
      }
      O1_LOG("\nO1 PnfRegistrationEvent : Event fields prepared for PNF registration");
   }
   return ret;
}

/*******************************************************************
 *
 * @brief prepare PNF registration additional Fields
 *
 * @details
 *
 *    Function : prepareAdditionalFields
 *
 *    Functionality:
 *      - prepare PNF registration additional Fields in json formate
 *
 * @params[in] IN   - pointer to addField
 * @return true     - success
 *         false    - failure
 *
 * ****************************************************************/

bool PnfRegistrationEvent::prepareAdditionalFields(cJSON *addFields)
{
   bool ret = true;
   if(JsonHelper::addNodeToObject(addFields, "oamPort", getNetconfPort().c_str()) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(addFields, "protocol", \
                                       NETCONF_PROTOCOL_SSH) == 0) {
      ret = false;
   }

   else if(JsonHelper::addNodeToObject(addFields, "username", \
                                       getUsername().c_str()) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(addFields, "password", \
                                       getPassword().c_str()) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(addFields, "reconnectOnChangedSchema", \
                                       RECONNECT_ON_SCHEMA_CHANGE_FALSE) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(addFields, "sleep-factor", \
                                       SLEEP_FACTOR_1_5) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(addFields, "tcpOnly", \
                                       TCP_ONLY_FALSE) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(addFields, "connectionTimeout", \
                                       CONNECTION_TIMEOUT_20000) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(addFields, "maxConnectionAttempts", \
                                       MAX_CONNECTION_ATTEMPTS_100) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(addFields, "betweenAttemptsTimeout", \
                                       BETWEEN_ATTEMPTS_TIMEOUT_2000) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(addFields, "keepaliveDelay", \
                                       KEEPALIVE_DELAY_120) == 0) {
      ret = false;
   }
   O1_LOG("\nO1 PnfRegistrationEvent : Additional fields prepared for PNF registration");
   return ret;
}
/*******************************************************************
 *
 * @brief Read json file
 *
 * @details
 *
 *    Function : readConfigFile
 *
 *    Functionality:
 *      - Read json file
 *
 *
 * @params[in] void
 * @return true  : success
 *         false : failure
 ******************************************************************/

void PnfRegistrationEvent::getNetconfConfig()
{
   mNetconfMacAddr = ConfigLoader::instance().getNetconfConfigFile().getNetconfMacAddr();
   mNetconfIpv4 = ConfigLoader::instance().getNetconfConfigFile().getNetconfIpv4();
   mNetconfIpv6 = ConfigLoader::instance().getNetconfConfigFile().getNetconfIpv6();
   mNetconfPort = ConfigLoader::instance().getNetconfConfigFile().getNetconfPort();
   mNetconfUsername =ConfigLoader::instance().getNetconfConfigFile().getNetconfUsername();
   mNetconfPassword = ConfigLoader::instance().getNetconfConfigFile().getNetconfPassword();
}


/**********************************************************************
  End of file
 **********************************************************************/
